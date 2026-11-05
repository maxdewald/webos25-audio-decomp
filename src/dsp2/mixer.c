/*
 * dsp2 mixer internals: the generic "basemixer" engine (chain / event / mixing
 * thread), the LG software mixer class "mixer" (lgmix_* / lgmixer_*), the
 * "mixerbin" bin that the kernel's mixer block instantiates, and its "mux".
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Linked at 0x60000000. Evidence: build/decomp/audio_a0_dsp2.main_60000000.bin.c,
 * instruction listings exported with tools/ListInsns.java into build/scratch/a18/asm/
 * (basemixer, mixer_event, check_input, processing_loop, do_bypass, lgmix_do_mix,
 * mixerbin_event, mux_chain), FLIX branch targets from tools/flix_annot.py,
 * build/scratch/evidence/runtime/idle-state.txt (live graph).
 *
 * Where ES can and cannot go (observed):
 *  - The kernel builds the ES bypass as separate pipelines "mixer_byin"
 *    (delay -> gain) and "mixer_bypass" (bypass), see src/kernel/aud/mixer.c.
 *    None of the code in this file is on that path: mixerbin only gets the PCM
 *    input pipelines (idle graph: gain_041b000{9,7,5,4} -> mixerbin in0/in1/in11/in10,
 *    gain_041b0001 -> bypass_04160001 in0, bypass has no out port at idle).
 *  - No function in 0x600913e0..0x600999ff loads the "es" key (its only literal
 *    slots 0x6005c800/0x6005f1f4/0x6005f458/0x6005fef0 are used by debug_buf,
 *    fader_event, gain_event and delay only). The mixer never looks at
 *    aud_data.type except META(2) vs. everything else, so an ES buffer that did
 *    reach a mixer input would be treated as PCM frames (summed / saturated by
 *    mix_signals, or copied by do_bypass), i.e. corrupted, never dropped by type.
 *  - mixerbin chooses its routing per input from the FIRST event's "dolby" and
 *    "dts" bools, not from "es".
 *
 * Log gating: basemixer uses the module's own "log-mask" (+0x08) via logmm_printlm;
 * lgmix/mixerbin/mux use per-source globals (0x4ff900bc / 0x4ff900c0 / 0x4ff900d0).
 * Log guards (FLIX bundles that only skip a log call) are dropped silently.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ------------------------------------------------------------------------
 * Shared core types (offsets are firmware offsets; the host build only needs
 * them to be syntactically valid)
 * ---------------------------------------------------------------------- */
enum aud_event_type { EV_FIRST = 1, EV_LAST = 2, EV_CUSTOM = 13 };
enum aud_data_type  { DATA_PCM = 0, DATA_ES = 1, DATA_META = 2 };

struct aud_event { char _h[0x0c]; int type; };
struct aud_data  { char _h[0x10]; int type; int index; uint32_t pts; char _p[0x18]; int size; void *buf; };
struct aud_port  { int num; struct aud_module *module; unsigned flag; /* bit1 DISABLED */ };

/* aud_data_adapter: FIFO of aud_data, wr/rd byte counters at +0x10/+0x14 */
struct aud_adapter { char _h[0x10]; int wr; int rd; int _18; };
#define ADAPTER_AVAIL(a) ((a)->wr - (a)->rd)

typedef struct { int _m[7]; } aud_mutex_t;
typedef struct { int _c[4]; } aud_cond_t;

struct aud_module {
    const char *name;                    /* +0x00 */
    int         uid;                     /* +0x04 */
    unsigned    log_mask;                /* +0x08 "log-mask" */
    const struct mixer_impl *impl;       /* +0x0c */
    char        _core[0x68 - 0x10];
    void       *imc;                     /* +0x68 */
    char        _core2[0xf0 - 0x6c];
};

/* ------------------------------------------------------------------------
 * basemixer
 * ---------------------------------------------------------------------- */

/* One mixer input, created on demand by impl->get_input. Common head (0x54 bytes). */
struct mix_input {
    struct mix_input *next, *prev;       /* +0x00 */
    int      port;                       /* +0x08 */
    bool     enabled;                    /* +0x0c set by FIRST, cleared by LAST/disconnect */
    bool     got_last;                   /* +0x0d */
    int      channels;                   /* +0x10 "channels" (default 2) */
    int      bps;                        /* +0x14 "bits-per-sample" (default 32) */
    int      _18;
    int      frame_bytes;                /* +0x1c (channels*bps+7)/8 */
    bool     has_data;                   /* +0x20 */
    int      frames_in;                  /* +0x24 */
    int      drop_au;                    /* +0x28 next N AUs are dropped (latency check) */
    int      insert_zero;                /* +0x2c zero AUs to insert before real data */
    bool     low_delay;                  /* +0x30 FIRST "low-delay" */
    bool     zero_mixing;                /* +0x31 currently mixing zeros for this input */
    struct aud_event *first;             /* +0x34 */
    struct aud_adapter fifo;             /* +0x38 */
};

struct basemixer;
struct mixer_impl {
    void *_generic[0x60 / 4];            /* aud_module_impl: name +0x04, chain +0x10, ... */
    struct mix_input *(*get_input)(struct basemixer *, int port);                         /* +0x60 */
    struct aud_data  *(*do_mix)(struct basemixer *, uint32_t *layout, int todo, int frames); /* +0x64 */
    int  (*can_mix)(struct basemixer *, struct mix_input *);                              /* +0x68 */
    struct aud_event *(*get_first_event)(struct basemixer *, struct aud_event *);         /* +0x6c */
    struct mix_input *(*input)(struct basemixer *, int port, struct aud_data *);          /* +0x70 */
    int  (*input_metadata)(struct basemixer *, int port, struct aud_data *);              /* +0x74 */
    struct aud_data  *(*output_metadata)(struct basemixer *);                             /* +0x78 */
    int  (*event_first)(struct basemixer *, struct mix_input *, struct aud_event *);      /* +0x7c */
    int  (*event_last)(struct basemixer *, struct mix_input *);                           /* +0x80 */
    int  (*event_mixing_metadata)(struct basemixer *, struct aud_event *);                /* +0x84 */
    int  (*command)(struct basemixer *, unsigned, void *, unsigned);                      /* +0x88 */
    int  (*get_nullgen_port)(struct basemixer *);                                         /* +0x8c */
    bool (*get_dtsx_changed)(struct basemixer *);                                         /* +0x90 */
    int  (*set_dtsx_changed)(struct basemixer *, bool);                                   /* +0x94 */
    int  (*toggle_first_info)(struct basemixer *, unsigned bit);                          /* +0x98 */
    int  (*clear_first_info)(struct basemixer *, unsigned bit);                           /* +0x9c */
    unsigned (*get_first_info)(struct basemixer *);                                       /* +0xa0 */
};

struct basemixer {
    struct aud_module m;                 /* +0x000 (input link root +0x10, input lock +0x18) */
    int        mixing_frames;            /* +0x0f0 frames all ready inputs can give */
    int        n_ready;                  /* +0x0f4 */
    int        n_main_ready;             /* +0x0f8 */
    aud_mutex_t lock;                    /* +0x0fc */
    aud_cond_t data_cond;                /* +0x118 new data / state change for the thread */
    uint32_t   cur_layout;               /* +0x128 layoutch of the last output (0 = none) */
    void      *thread;                   /* +0x12c */
    int        running;                  /* +0x130 */
    bool       idle;                     /* +0x134 thread waits for data */
    aud_cond_t done_cond;                /* +0x138 thread consumed data */
    uint32_t   low_delay_ports;          /* +0x148 bit per port */
    int        follow_input;             /* +0x14c "follow-input" (init -1) */
};

enum mix_todo { TODO_NOTHING = 0, TODO_MIXING = 1, TODO_LAST_EVENT = 2 }; /* names @0x60102c90 */

/* layoutch word: bits 28..31 channel count, bits 0..3 layout index */
#define LAYOUT_CH(l)   ((unsigned)(l) >> 28)
#define LAYOUT_IDX(l)  ((l) & 0x0fffffffu)

struct layout_desc { const char *name; uint32_t info; };   /* table @0x60005a40, 13 entries */

extern const struct layout_desc aud_basemixer_layouts[13];
extern volatile unsigned *mixer_log_mask;                 /* 0x4ff900bc */
extern void *mixer_log_ctx;                               /* 0x60102c50 */
extern unsigned mixer_latency_limit;                      /* 0x60102c60, 900 ticks = 10 ms */
extern int low_delay_drop_aus;                            /* 0x60168460 */

extern void logmm_printl(void *ctx, int lvl, const char *fn, int line, const char *fmt, ...);
extern void logmm_printlm(const char *name, int lvl, const char *fn, int line, const char *fmt, ...);
#define MLOG(bm, lvl, ...) \
    do { if ((bm)->m.log_mask & (1u << (lvl))) \
        logmm_printlm((bm)->m.name, lvl, __func__, __LINE__, __VA_ARGS__); } while (0)
#define GLOG(lvl, ...) \
    do { if (*mixer_log_mask & (1u << (lvl))) \
        logmm_printl(mixer_log_ctx, lvl, __func__, __LINE__, __VA_ARGS__); } while (0)

extern int  strcmp(const char *, const char *);
extern void *calloc(size_t, size_t);
extern void  aud_free(void *);                            /* _BufMonitor_RemoveBufferIndex */
extern char *strdup(const char *);
extern unsigned long _Stoul(const char *, char **, int);
extern void aud_mutex_init(void *), aud_mutex_lock(void *), aud_mutex_unlock(void *);
extern void aud_cond_init(void *), aud_cond_signal(void *), aud_cond_wait(void *, void *);
extern void *aud_thread_create(const char *, int prio, int stack, void (*fn)(void *), void *);
extern void aud_thread_destroy(void *);
extern void *aud_object_get(void *);
extern void aud_object_put(void *);
extern const char *aud_event_name(struct aud_event *);
extern int  aud_event_value_get_int(struct aud_event *, const char *, int def);
extern bool aud_event_value_get_bool(struct aud_event *, const char *, bool def);
extern const char *aud_event_value_get_str(struct aud_event *, const char *, const char *def);
extern bool aud_event_has_name(struct aud_event *, const char *);
extern void aud_event_value_set_int(struct aud_event *, const char *, int);
extern void aud_event_value_set_bool(struct aud_event *, const char *, bool);
extern struct aud_event *aud_event_new_first(void);
extern struct aud_event *aud_event_new_last(void);
extern struct aud_data *aud_data_new(int size, ...);
extern struct aud_data *aud_data_new_zero(int size);
extern void aud_data_adapter_init(struct aud_adapter *);
extern void aud_data_adapter_cleanup(struct aud_adapter *);
extern void aud_data_adapter_push(struct aud_adapter *, struct aud_data *);
extern struct aud_data *aud_data_adapter_dequeue(struct aud_adapter *);
extern struct aud_data *aud_data_adapter_get_data(struct aud_adapter *, int bytes);
extern void *aud_data_adapter_map(struct aud_adapter *, int bytes);
extern void aud_data_adapter_flush(struct aud_adapter *, int bytes);
extern int  aud_module_default_event(struct aud_module *, struct aud_port *, struct aud_event *);
extern int  aud_module_default_command(struct aud_module *, unsigned, void *, unsigned);
extern int  aud_module_push_data(struct aud_module *, int port, struct aud_data *);
extern int  aud_module_push_event(struct aud_module *, int port, struct aud_event *);
extern int  aud_module_sync(struct aud_module *, int port);
extern void aud_module_impl_add_prop(void *impl, const char *name, int type, unsigned off,
                                     void *set, void *gets, int ro);
extern void aud_object_class_register(const void *);
extern void aud_mregistry_add_class(const void *);
extern char *audutil_strdup_printf(const char *, ...);
extern int  CommonBuffer_InsertAuInternal(char *dst, const char *fmt, ...); /* snprintf-like */
extern void AuiLogger_Init(void *dst, int c, unsigned n);                    /* memset */
extern uint32_t stc_now(void);                            /* *(0xc1000684) */

/* @0x600913e0 aud_basemixer_layout_info: channel/mask word of a layoutch */
uint32_t aud_basemixer_layout_info(uint32_t layoutch)
{
    uint32_t idx = LAYOUT_IDX(layoutch);

    if (idx < 13)
        return aud_basemixer_layouts[idx].info;
    GLOG(0, "wrong layout. %d\n", idx);
    return aud_basemixer_layouts[0].info;
}

/* @0x6009141c aud_layoutch_name */
const char *aud_layoutch_name(uint32_t layoutch)
{
    uint32_t idx = LAYOUT_IDX(layoutch);

    if (idx > 12) {
        GLOG(0, "wrong layout. %d\n", idx);
        return "unknown_layout";
    }
    if (aud_basemixer_layouts[idx].name)
        return aud_basemixer_layouts[idx].name;
    GLOG(0, "not defined layout. %d\n", idx);
    return NULL;
}

/* @0x60091478 aud_basemixer_layout_from_acmod */
int aud_basemixer_layout_from_acmod(int acmod)
{
    if (acmod > 0) {
        if (acmod < 8)
            return acmod;
        if (acmod == 21)
            return 8;
        if (acmod == 22)
            return 10;
        if (acmod == 28)
            return 9;
    }
    return 2;                                          /* "lr" */
}

/* @0x600914a4 aud_basemixer_layout_from_name: exact strcmp over the table */
int aud_basemixer_layout_from_name(const char *name)
{
    for (int i = 0; i < 13; i++)
        if (aud_basemixer_layouts[i].name && !strcmp(name, aud_basemixer_layouts[i].name))
            return i;
    GLOG(0, "unknown layout name. %s\n", name);
    return 2;
}

/* @0x600914f4 aud_basemixer_acmod_from_layout */
int aud_basemixer_acmod_from_layout(unsigned layout)
{
    if (layout != 0) {
        if (layout < 8)
            return (int)layout;
        if (layout == 8)
            return 21;
        if (layout == 9)
            return 28;
        if (layout == 10)
            return 22;
    }
    return 2;
}

extern struct mix_input *first_input_link(struct basemixer *);   /* m +0x14 */
extern struct mix_input *next_input_link(struct mix_input *);
extern int input_link_port(struct mix_input *);                   /* link +0x28 -> port num */

/* @0x6009151c check_input: decide what the mixing thread does next.
 * The per-input loop is fully observed; the minimum-frames bookkeeping for a
 * low-delay input without data sits behind FLIX @0x600916f0 [FLIX?]. */
int check_input(struct basemixer *bm)
{
    const struct mixer_impl *impl = bm->m.impl;
    int main_port = impl->get_nullgen_port(bm);
    int n_enabled = 0;
    bool blocked = false;

    bm->mixing_frames = 0x7fffffff;
    bm->n_ready = 0;
    bm->n_main_ready = 0;

    aud_mutex_lock((char *)&bm->m + 0x18);
    for (struct mix_input *l = first_input_link(bm); l; l = next_input_link(l)) {
        struct mix_input *in = impl->get_input(bm, input_link_port(l));
        int avail, frames;

        MLOG(bm, 5, "%s: port%d\n", bm->m.name, in->port);
        if (!in->enabled || in->got_last) {
            MLOG(bm, 5, "%s: port%d, enabled %d, got_last %d\n", bm->m.name, in->port,
                 in->enabled, in->got_last);
            continue;
        }
        n_enabled++;
        if (!impl->can_mix(bm, in) && !in->low_delay) {
            MLOG(bm, 5, "%s: port%d, cannot mix\n", bm->m.name, in->port);
            blocked = true;
            break;
        }
        avail = ADAPTER_AVAIL(&in->fifo);
        bm->n_ready++;
        if (main_port != -1 && main_port == in->port)
            bm->n_main_ready++;
        if (!in->low_delay || avail) {
            frames = avail / in->frame_bytes;
            if (frames < bm->mixing_frames)
                bm->mixing_frames = frames;
        } else if (bm->mixing_frames > 0x100) {
            bm->mixing_frames = 0x100;                 /* [FLIX?] @0x600916f0 */
        }
        MLOG(bm, 5, "%s: port%d, samples %d, mixing frames %d\n", bm->m.name, in->port,
             avail / in->frame_bytes, bm->mixing_frames);
    }
    aud_mutex_unlock((char *)&bm->m + 0x18);

    /* tail @0x600916fb..0x6009172d, observed */
    if (bm->cur_layout) {
        if (!n_enabled)
            return TODO_LAST_EVENT;
        if (blocked)
            return TODO_NOTHING;
        return bm->n_ready ? TODO_MIXING : TODO_LAST_EVENT;
    }
    if (!n_enabled || blocked)
        return TODO_NOTHING;
    return bm->n_ready ? TODO_MIXING : TODO_NOTHING;
}

/* @0x60091758 processing_loop: the mixer thread. Outer loop, idle wait and the
 * final push/sync are observed. The first-event / layout-change branch
 * (0x60091956..0x60091bd3) is reached only through FLIX bundles; its content is
 * listed but the branch conditions are opaque. [FLIX] */
void processing_loop(void *arg)
{
    struct basemixer *bm = arg;
    const struct mixer_impl *impl = bm->m.impl;

    MLOG(bm, 3, "%s: mixing thread started.\n", bm->m.name);
    while (bm->running) {
        struct aud_data *out = NULL, *md = NULL;
        uint32_t layout = 0;
        int todo;

        aud_mutex_lock(&bm->lock);
        if (!bm->running) {
            if (impl->get_first_info)
                impl->get_first_info(bm);
            aud_mutex_unlock(&bm->lock);
            break;
        }
        do {
            todo = check_input(bm);
            MLOG(bm, 5, "%s: todo %s(%d), cur %s(%dch)\n", bm->m.name,
                 todo == TODO_MIXING ? "mixing" : todo ? "last_event" : "nothing", todo,
                 aud_layoutch_name(bm->cur_layout), LAYOUT_CH(bm->cur_layout));
            if (todo == TODO_NOTHING) {
                bm->idle = true;
                aud_cond_signal(&bm->done_cond);
                aud_cond_wait(&bm->data_cond, &bm->lock);
                bm->idle = false;
            }
        } while (bm->running && todo == TODO_NOTHING);

        if (todo == TODO_MIXING) {
            /* [FLIX] when the impl's first-event info changed (dtsx-enc-mode, decoder
             * downmix, channel mapping: clear_first_info(1/2/3)) or the output layout
             * changed ("output layout changed. from ..."), a new FIRST is built with
             * get_first_event(), stamped with "channels", "bits-per-sample",
             * "processing-size" and pushed with aud_module_push_event(m, 0, e)
             * ("generate first event. layout:\"%s\"(%dch)"). */
            out = impl->do_mix(bm, &layout, todo, bm->mixing_frames);
            md = impl->output_metadata(bm);
            if (layout)
                bm->cur_layout = layout;
        } else if (todo == TODO_LAST_EVENT) {
            /* "%s: send last event": push LAST (aud_event_new_last @0x60069fe8) */
            aud_module_push_event(&bm->m, 0, aud_event_new_last());
            bm->cur_layout = 0;
        }
        aud_mutex_unlock(&bm->lock);

        if (md)
            aud_module_push_data(&bm->m, 0, md);
        if (out)
            aud_module_push_data(&bm->m, 0, out);   /* "%s: output metadata %4d bytes, pcm %4d bytes" */
        aud_module_sync(&bm->m, 0);
    }
}

/* @0x60091bfc basemixer: impl->chain of every basemixer class ("mixer_chain").
 * Back-pressure: the upstream thread waits here until the mixing thread has
 * consumed the input (or the input is low-delay / the mixer is idle). */
int basemixer(struct aud_port *port, struct aud_data *data)
{
    struct basemixer *bm = (struct basemixer *)port->module;
    const struct mixer_impl *impl = bm->m.impl;
    struct mix_input *in;

    aud_mutex_lock(&bm->lock);
    /* [FLIX?] @0x60091c16 -> 0x60091d8c and @0x60091c1e -> 0x60091d00 test
     * data->type (loaded at 0x60091c0e); both targets call impl->input_metadata,
     * put the buffer and return 0, i.e. META / META_EXT */
    if (data->type == DATA_META || data->type == 3) {
        impl->input_metadata(bm, port->num, data);
        aud_object_put(data);
        aud_mutex_unlock(&bm->lock);
        return 0;
    }
    in = impl->input(bm, port->num, data);
    if (!in) {
        aud_object_put(data);
        MLOG(bm, 3, "%s: drop..\n", bm->m.name);
        aud_mutex_unlock(&bm->lock);
        return -1;
    }
    in->has_data = true;
    in->frames_in += data->size / in->frame_bytes;
    aud_cond_signal(&bm->data_cond);

    while (!(port->flag & 2)) {
        if (impl->can_mix(bm, in)) {
            MLOG(bm, 5, "%s: port%d, can mix\n", bm->m.name, in->port);
        } else {
            MLOG(bm, 5, "%s: port%d, cannot mix\n", bm->m.name, in->port);
            if (in->low_delay || bm->idle)
                break;
        }
        aud_cond_wait(&bm->done_cond, &bm->lock);
    }
    aud_mutex_unlock(&bm->lock);
    return 0;
}

/* @0x60091dac debug_mixer ("debug_mixe" in functions.tsv): set the latency limit
 * from a debug option string; the value is given in ms and stored *90. */
int debug_mixer(void *m, const char *arg)
{
    extern void *audutil_set_opts(void *opts, const char *str);
    extern void audutil_print_opt(void);
    extern void *mixer_debug_opts;                     /* 0x600600e4 */
    void *o = audutil_set_opts(mixer_debug_opts, arg + 4);

    (void)m;
    if (!o)
        return 0;
    audutil_print_opt();
    if (*(void **)((char *)o + 8) == &mixer_latency_limit)
        mixer_latency_limit *= 90;
    return 0;
}

/* @0x60091de4 mixer_event: impl->event of every basemixer class.
 * FIRST/LAST/custom handling observed; the type tests for LAST, the latency
 * check and CUSTOM are FLIX bundles @0x60091e2c/36/40 [FLIX], their target
 * blocks are fully listed. */
int mixer_event(struct basemixer *bm, struct aud_port *port, struct aud_event *e)
{
    const struct mixer_impl *impl = bm->m.impl;
    struct mix_input *in;

    MLOG(bm, 3, "%s: port%d, event %s\n", bm->m.name, port ? port->num : -1, aud_event_name(e));
    if (!port)
        return 0;

    if (e->type == EV_FIRST) {
        aud_mutex_lock(&bm->lock);
        in = impl->get_input(bm, port->num);
        if (in) {
            MLOG(bm, 3, "%s: port%d, first event\n", bm->m.name, in->port);
            in->insert_zero = 0;
            /* [FLIX?] @0x60091ea5 -> 0x600920ec: "low-delay" handling */
            {
                int ld = aud_event_value_get_int(e, "low-delay", 0);
                in->zero_mixing = false;
                in->low_delay = ld != 0;
                if (ld) {
                    bm->low_delay_ports |= 1u << (port->num & 31);
                    in->drop_au = low_delay_drop_aus;
                    if (!in->enabled)
                        in->insert_zero = 1;
                } else {
                    bm->low_delay_ports &= ~(1u << (port->num & 31));
                }
            }
            in->has_data = false;
            in->got_last = false;
            in->enabled = true;
            in->channels = aud_event_value_get_int(e, "channels", 2);
            in->bps = aud_event_value_get_int(e, "bits-per-sample", 32);
            in->frame_bytes = (in->channels * in->bps + 7) / 8;
            if (in->first)
                aud_object_put(in->first);
            in->first = aud_object_get(e);
            aud_data_adapter_flush(&in->fifo, ADAPTER_AVAIL(&in->fifo));
            impl->event_first(bm, in, e);
            MLOG(bm, 3, "%s: port%d, enable %dch, %dbps, low_delay 0x%08x\n", bm->m.name,
                 port->num, in->channels, in->bps, bm->low_delay_ports);
        }
        aud_object_put(e);
        aud_mutex_unlock(&bm->lock);
        return 0;
    }

    if (e->type == EV_LAST) {                          /* [FLIX] @0x60091e2c -> 0x60091f32 */
        aud_mutex_lock(&bm->lock);
        if (port->num < 32)
            bm->low_delay_ports &= ~(1u << port->num);
        in = impl->get_input(bm, port->num);
        if (in && in->enabled) {
            MLOG(bm, 3, "%s: port%d, last event\n", bm->m.name, in->port);
            aud_object_put(in->first);
            in->first = NULL;
            in->enabled = false;
            in->low_delay = false;
            in->zero_mixing = false;
            if (in->has_data)
                in->got_last = true;
            impl->event_last(bm, in);
            MLOG(bm, 3, "%s: port%d, mark \"got_last\"\n", bm->m.name, in->port);
        }
        aud_object_put(e);
        aud_cond_signal(&bm->data_cond);
        aud_mutex_unlock(&bm->lock);
        return 0;
    }

    /* [FLIX?] @0x60091e36 -> 0x60091fc4: type 9 (movi a12,9 feeds the bundle),
     * the AU latency-check event */
    if (e->type == 9) {
        uint32_t now = stc_now();
        int src = aud_event_value_get_int(e, "source", 0);
        uint32_t gstc = (uint32_t)aud_event_value_get_int(e, "gstc", 0);
        uint32_t diff = now - gstc;

        (void)src;
        if (diff > mixer_latency_limit) {
            logmm_printlm(bm->m.name, 2, "mixer_event_latency_check", 0x292,
                          "drop an AU from 0x%08x, mixer input%d, diff %dms\n",
                          gstc, port->num, diff / 90);
            in = impl->get_input(bm, port->num);
            if (in)
                in->drop_au = 1;
        }
        aud_object_put(e);
        return 0;
    }

    if (e->type == EV_CUSTOM) {                        /* [FLIX?] @0x60091e40 -> 0x60092026, movi a13,13 */
        const char *name = aud_event_value_get_str(e, "name", "");

        if (!strcmp(name, "setting-mixing-metadata")) {
            if (impl->event_mixing_metadata)
                impl->event_mixing_metadata(bm, e);
            aud_object_put(e);
            return 0;
        }
        if (aud_event_has_name(e, "delay-changed")) {
            logmm_printl(mixer_log_ctx, 2, "mixer_event_custom", 0x272,
                         "throw away delauy-changed custom event...\n");
            aud_object_put(e);
            return 0;
        }
    }
    MLOG(bm, 3, "%s: port%d, do default event handler..\n", bm->m.name, port->num);
    aud_module_default_event(&bm->m, port, e);
    return 0;
}

/* @0x600921c4 mixer_command: impl command first; ADEC START/STOP/0x1003002c are
 * not passed on to the default handler when the impl refused them. */
int mixer_command(struct basemixer *bm, unsigned cmd, void *param, unsigned len)
{
    int ret;

    MLOG(bm, 3, "%s: command %08x\n", bm->m.name, cmd);
    aud_mutex_lock(&bm->lock);
    ret = bm->m.impl->command(bm, cmd, param, len);
    aud_mutex_unlock(&bm->lock);
    if (ret >= 0)
        return ret;
    if (cmd == 0x10010000u || cmd == 0x10020000u || cmd == 0x1003002cu)
        return ret;
    return aud_module_default_command(&bm->m, cmd, param, len);
}

/* @0x60092240 mixer_disconnect_port: a disconnected input counts as "got last" */
int mixer_disconnect_port(struct basemixer *bm, struct aud_port *port)
{
    struct mix_input *in;

    aud_mutex_lock(&bm->lock);
    in = bm->m.impl->get_input(bm, port->num);
    if (!in) {
        MLOG(bm, 2, "%s: no input for port%d\n", bm->m.name, port->num);
        aud_mutex_unlock(&bm->lock);
        return 0;
    }
    MLOG(bm, 3, "%s: port%d, disconnected\n", bm->m.name, port->num);
    if (in->enabled) {
        aud_object_put(in->first);
        in->first = NULL;
        in->enabled = false;
        if (in->has_data)
            in->got_last = true;
        bm->m.impl->event_last(bm, in);
        MLOG(bm, 3, "%s: port%d, mark \"got_last\"\n", bm->m.name, in->port);
    }
    aud_cond_signal(&bm->done_cond);
    aud_cond_signal(&bm->data_cond);
    aud_mutex_unlock(&bm->lock);
    return 0;
}

/* @0x60092340 mixer_deleting_port */
int mixer_deleting_port(struct basemixer *bm, struct aud_port *port)
{
    if ((port->flag & 1) && (bm->m.log_mask & 8))
        logmm_printlm(bm->m.name, 3, "mixer_deleting_port", 0x382,
                      "%s: disable output port%d..\n", bm->m.name, port->num);
    return 0;
}

/* @0x60092370 aud_basemixer_init ("mixer_start"): create or resume the thread */
int aud_basemixer_init(struct basemixer *bm)
{
    bm->follow_input = -1;
    aud_mutex_init(&bm->lock);
    aud_cond_init(&bm->data_cond);
    aud_cond_init(&bm->done_cond);
    MLOG(bm, 3, "%s: start..\n", bm->m.name);
    if (!bm->running) {
        bm->running = 1;
        MLOG(bm, 3, "%s: create thread..\n", bm->m.name);
        bm->thread = aud_thread_create(bm->m.name, 0x10000, 10, processing_loop, bm);
        if (!bm->thread) {
            MLOG(bm, 0, "%s: cannot create thread for mixer\n", bm->m.name);
            bm->running = 0;
        }
        return 0;
    }
    aud_mutex_lock(&bm->lock);
    MLOG(bm, 3, "%s: resume thread..\n", bm->m.name);
    aud_cond_signal(&bm->data_cond);
    aud_mutex_unlock(&bm->lock);
    return 0;
}

/* @0x60092458 aud_basemixer_cleanup */
void aud_basemixer_cleanup(struct basemixer *bm)
{
    MLOG(bm, 3, "%s: destroy thread...\n", bm->m.name);
    aud_mutex_lock(&bm->lock);
    bm->running = 0;
    aud_cond_signal(&bm->data_cond);
    aud_mutex_unlock(&bm->lock);
    aud_thread_destroy(bm->thread);
    bm->thread = NULL;
    MLOG(bm, 3, "stop done.\n");
}

/* @0x600924c0 mixer_gets_internal_buf_size: "internal-buf-size" getter,
 * "0x<port>:<pending bytes> " for every enabled input */
char *mixer_gets_internal_buf_size(struct basemixer *bm)
{
    char buf[512];
    int n = 0;

    AuiLogger_Init(buf, 0, sizeof(buf));
    aud_mutex_lock((char *)&bm->m + 0x18);
    for (struct mix_input *l = first_input_link(bm); l; l = next_input_link(l)) {
        int port = input_link_port(l);
        struct mix_input *in = bm->m.impl->get_input(bm, port);

        if (in->enabled && !in->got_last)
            n += CommonBuffer_InsertAuInternal(buf + n, "0x%X:%u ", port, ADAPTER_AVAIL(&in->fifo));
    }
    aud_mutex_unlock((char *)&bm->m + 0x18);
    return strdup(buf);
}

extern const char basemixer_class[];                   /* @0x60060190 */

/* @0x60092540 __aud_object_register_basemixer */
void __aud_object_register_basemixer(void)
{
    aud_object_class_register(basemixer_class);
}

/* @0x60092550 aud_object_class_init_basemixer */
int aud_object_class_init_basemixer(void **impl)
{
    impl[0x10 / 4] = (void *)basemixer;
    impl[0x14 / 4] = (void *)mixer_event;
    impl[0x18 / 4] = (void *)mixer_command;
    impl[0x20 / 4] = (void *)mixer_disconnect_port;
    impl[0x24 / 4] = (void *)mixer_deleting_port;
    impl[0x2c / 4] = (void *)(uintptr_t)0x150;         /* instance size */
    impl[0x4c / 4] = (void *)(uintptr_t)1;             /* SKIP_SYNC: own thread pushes */
    impl[0x04 / 4] = (void *)"basemixer";
    aud_module_impl_add_prop(impl, "follow-input", 2, 0x14c, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "internal-buf-size", 4, 0, NULL,
                             (void *)mixer_gets_internal_buf_size, 1);
    return 0;
}

/* ------------------------------------------------------------------------
 * "mixer": LG software mixer (lgmix_*), 4 instances inside mixerbin
 * ---------------------------------------------------------------------- */
struct lgmix_input {
    struct mix_input c;
    uint32_t layoutch;                   /* +0x54 */
    int      bps_in;                     /* +0x58 */
    int32_t *samples;                    /* +0x5c */
    void   (*get_samples)(struct basemixer *, struct lgmix_input *, int32_t *dst); /* +0x60 */
    int      out_ch;                     /* +0x64 */
    int      n_ch;                       /* +0x68 min(in, out) */
    int      in_ch;                      /* +0x6c */
    int32_t  matrix[(0x2b0 - 0x70) / 4]; /* +0x70 build_transform_matrix */
    void    *mapped;                     /* +0x2b0 */
    unsigned zero_logs;                  /* +0x2b4 */
};

struct port_info { struct port_info *next, *prev; int port; int target; };

struct lgmixer {
    struct basemixer bm;
    struct lgmix_input *in_head, *in_tail; /* +0x150 list of struct lgmix_input */
    struct port_info  *pi_head, *pi_tail;  /* +0x158 "port-info" list */
    aud_mutex_t pi_lock;                 /* +0x160 */
    uint32_t out_layout;                 /* +0x17c */
    const char *out_layout_name;         /* +0x180 "out-layout" */
    int      out_channels;               /* +0x184 "out-channels" */
    int      main_audio_port;            /* +0x188 "main-audio-port" / "port-info" */
    int      nullgen_port;               /* +0x18c "nullgen-port" */
    bool     output_connect;             /* +0x190 "output-connect" */
    int      underflow_count;            /* +0x194 "underflow-count" */
    struct aud_adapter md;               /* +0x198 metadata FIFO */
    int      dtsx_enc_mode;              /* +0x1b4 */
    bool     dtsx_changed;               /* +0x1b8 */
    bool     downmix;                    /* +0x1b9 */
    bool     ch_mapping_lfe_c;           /* +0x1ba */
    unsigned first_info;                 /* +0x1bc */
};

extern void get_samples_default32(struct basemixer *, struct lgmix_input *, int32_t *); /* @0x600925b8 */
extern void get_samples_default16(struct basemixer *, struct lgmix_input *, int32_t *); /* @0x60092660 */
extern void build_transform_matrix(struct lgmixer *, struct lgmix_input *);           /* @0x60093134 */
extern struct lgmix_input *lgmix_list_first(struct lgmixer *);
extern struct lgmix_input *lgmix_list_next(struct lgmixer *, struct lgmix_input *);
extern void list_add_tail_(void *head, void *node);
extern void list_del_(void *node);
int lgmix_toggle_first_event_info(struct basemixer *bm, unsigned bit);
struct prop_desc { const char *n; int t; unsigned off; };

/* @0x600925b8 get_samples_default32 ("lgmix_update_getsamples" in functions.tsv;
 * it is the 32-bit sample fetch installed as get_samples): [FLIX] body (copy loop is
 * FLIX-only, Ghidra: halt_baddata). */

/* @0x60092660 get_samples_default16: [FLIX] same, 16-bit input. When the input
 * has no output channels left (out_ch < 1) the read pointer is just advanced. */

/* @0x600926f4 lgmix_get_input */
struct mix_input *lgmix_get_input(struct basemixer *bm, int port)
{
    struct lgmixer *lm = (struct lgmixer *)bm;
    struct lgmix_input *in;

    for (in = lgmix_list_first(lm); in; in = lgmix_list_next(lm, in))
        if (in->c.port == port)
            return &in->c;
    in = calloc(sizeof(*in), 1);
    if (!in) {
        GLOG(0, "no mem\n");
        return NULL;
    }
    in->c.port = port;
    in->c.enabled = false;
    in->c.channels = 8;
    aud_data_adapter_init(&in->c.fifo);
    list_add_tail_(&lm->in_head, in);
    return &in->c;
}

/* @0x6009277c lgmix_get_output_acmod: acmod of the main port, else of the port
 * mapped by "port-info" */
int lgmix_get_output_acmod(struct lgmixer *lm)
{
    struct lgmix_input *in;
    int port = -1;

    for (in = lgmix_list_first(lm); in; in = lgmix_list_next(lm, in))
        if (in->c.enabled && lm->main_audio_port == in->c.port)
            port = lm->main_audio_port;
    if (port == -1) {
        aud_mutex_lock(&lm->pi_lock);
        for (struct port_info *pi = lm->pi_head; pi; pi = pi->next)
            if (pi->port == lm->main_audio_port) {
                GLOG(4, "found for port%d : 0x%04X\n", lm->main_audio_port, pi->target);
                port = pi->target;
            }
        aud_mutex_unlock(&lm->pi_lock);
    }
    in = (struct lgmix_input *)lgmix_get_input(&lm->bm, port);
    if (!in)
        return 0;
    {
        int acmod = aud_basemixer_acmod_from_layout(in->layoutch & 0xf);
        GLOG(4, "port_input: %d, min->in_layoutch: 0x%08X, acmod_out: %d\n", port,
             in->layoutch, acmod);
        return acmod;
    }
}

/* @0x600928a4 do_bypass: copy one input straight through when its layout equals
 * the output layout and it is 32-bit. Called only when fewer than 3 inputs are
 * active. Not an ES bypass: the data is still taken as PCM frames. */
struct aud_data *do_bypass(struct lgmixer *lm, int todo)
{
    struct lgmix_input *in;

    GLOG(5, "bypass..\n");
    for (in = lgmix_list_first(lm); in; in = lgmix_list_next(lm, in))
        if (in->c.enabled && (in->c.port != lm->nullgen_port || todo != 2))
            break;
    if (!in) {
        GLOG(0, "Oops??\n");
        return NULL;
    }
    GLOG(4, "ch %x -> %x, bps %d -> %d\n", in->layoutch, lm->out_layout, in->bps_in, 32);
    if (in->layoutch != lm->out_layout || in->bps_in != 32) {
        GLOG(4, "different in/out layout. donot bypass\n");
        return NULL;
    }
    GLOG(4, "port%d, bypass %d bytes\n", in->c.port, ADAPTER_AVAIL(&in->c.fifo));

    if (!in->c.low_delay || ADAPTER_AVAIL(&in->c.fifo)) {
        if (in->c.zero_mixing) {
            GLOG(in->zero_logs < 64 ? 2 : 3, "port%d, low delay, mix normal input..\n", in->c.port);
            in->c.zero_mixing = false;
        }
        lm->underflow_count++;
        return aud_data_adapter_get_data(&in->c.fifo, ADAPTER_AVAIL(&in->c.fifo));
    }
    /* low-delay input with nothing queued: output 256 frames of silence */
    if (!in->c.zero_mixing) {
        if (in->zero_logs < 64) {
            GLOG(2, "port%d, low delay, mix zero input.. %d\n", in->c.port, in->zero_logs);
            if (++in->zero_logs == 64)
                GLOG(2, "port%d, no more print..\n", in->c.port);
        } else {
            GLOG(3, "port%d, low delay, mix zero input..\n", in->c.port);
        }
        in->c.zero_mixing = true;
    }
    return aud_data_new_zero(in->c.frame_bytes << 8);
}

/* @0x60092a98 mix_signals: sum all enabled inputs into a 32-bit buffer of the
 * output layout and saturate. Input mapping and the first frame of the outer
 * loop are observed; loop bounds/increments are FLIX [FLIX]. */
struct aud_data *mix_signals(struct lgmixer *lm, int todo, int frames)
{
    unsigned out_ch = LAYOUT_CH(lm->out_layout);
    unsigned used_ch = (aud_basemixer_layout_info(lm->out_layout) >> 24) & 0xf;
    struct aud_data *out = aud_data_new(frames * (int)out_ch * 4, out_ch, "mix_signals");
    int32_t zeros[10] = {0};
    int32_t tmp[12];

    (void)todo;
    if (!out)
        return NULL;
    for (struct lgmix_input *in = lgmix_list_first(lm); in; in = lgmix_list_next(lm, in)) {
        if (!in->c.enabled)
            continue;
        if (!in->c.low_delay || frames <= ADAPTER_AVAIL(&in->c.fifo) / in->c.frame_bytes) {
            if (in->c.zero_mixing) {
                GLOG(2, "port%d, low delay, mix normal input..\n", in->c.port);
                in->c.zero_mixing = false;
            }
            in->mapped = aud_data_adapter_map(&in->c.fifo, in->c.frame_bytes * frames);
            if (!in->mapped)
                GLOG(0, "cannot map. %d\n", in->c.frame_bytes * frames);
            in->samples = in->mapped;
        } else {
            if (!in->c.zero_mixing) {
                GLOG(2, "port%d, low delay, mix zero input..\n", in->c.port);
                in->c.zero_mixing = true;
            }
            in->mapped = NULL;
            in->samples = NULL;
        }
    }

    for (int f = 0; f < frames; f++) {
        int32_t *o = (int32_t *)out->buf + f * (int)out_ch;
        int n = 0;

        for (struct lgmix_input *in = lgmix_list_first(lm); in; in = lgmix_list_next(lm, in)) {
            if (!in->c.enabled)
                continue;
            if (!in->mapped)
                in->samples = zeros;
            if (n++ == 0) {
                in->get_samples(&lm->bm, in, o);
            } else {
                in->get_samples(&lm->bm, in, tmp);
                for (unsigned c = 0; c < used_ch && c < out_ch; c++)
                    o[c] += tmp[c];
            }
        }
        /* saturate the used channels (Q-format headroom: >0x7fff -> max,
         * < -0x8000 -> min, else <<16) and zero the unused ones */
        for (unsigned c = 0; c < used_ch; c++) {
            if (o[c] > 0x7fff)
                o[c] = 0x7fffffff;
            else if (o[c] > -0x8000)
                o[c] = (int32_t)((uint32_t)o[c] << 16);
            else
                o[c] = (int32_t)0x80000000;
        }
        for (unsigned c = used_ch; c < out_ch; c++)
            o[c] = 0;
    }
    return out;
}

/* @0x60092e28 lgmix_do_mix */
struct aud_data *lgmix_do_mix(struct basemixer *bm, uint32_t *layout, int todo, int frames)
{
    struct lgmixer *lm = (struct lgmixer *)bm;
    struct aud_data *out = NULL;

    lgmix_get_output_acmod(lm);
    if (*layout == 0)
        *layout = lm->out_layout;
    else
        lm->out_layout = *layout;

    if (!lm->output_connect) {
        /* output disconnected ("disconnect-output"): discard queued metadata */
        while (ADAPTER_AVAIL(&lm->md))
            aud_object_put(aud_data_adapter_dequeue(&lm->md));
    } else {
        /* observed: +0xf8 (n_main_ready) != 0 && todo < 3 -> try do_bypass;
         * the n_main_ready == 0 side is a FLIX bundle @0x60092e88 [FLIX] */
        if (lm->bm.n_main_ready && todo < 3)
            out = do_bypass(lm, todo);
        if (!out)
            out = mix_signals(lm, todo, frames);
    }

    for (struct lgmix_input *in = lgmix_list_first(lm); in; in = lgmix_list_next(lm, in)) {
        int avail;

        if (!in->c.enabled)
            continue;
        avail = ADAPTER_AVAIL(&in->c.fifo);
        if (in->c.frame_bytes * frames != avail)
            GLOG(0, "abnormal input size. input_size: %d, mixing_frame: %d\n",
                 avail, in->c.frame_bytes * frames);
        if (!in->c.zero_mixing) {
            aud_data_adapter_flush(&in->c.fifo, in->c.frame_bytes * frames);
            GLOG(4, "port%d, remains %d frames\n", in->c.port,
                 ADAPTER_AVAIL(&in->c.fifo) / in->c.frame_bytes);
        }
    }
    return out;
}

/* @0x60092fe8 lgmix_can_mix: pending = wr - rd; the comparison against the
 * required frame count is a FLIX bundle @0x60092ff6 [FLIX] */
int lgmix_can_mix(struct basemixer *bm, struct mix_input *in)
{
    (void)bm;
    return ADAPTER_AVAIL(&in->fifo) > 0;               /* [FLIX] threshold unknown */
}

/* @0x60092ffc lgmix_get_first_event: output FIRST of an LG mixer */
struct aud_event *lgmix_get_first_event(struct basemixer *bm, struct aud_event *e)
{
    struct lgmixer *lm = (struct lgmixer *)bm;
    int acmod = lgmix_get_output_acmod(lm);

    if (!e && !(e = aud_event_new_first()))
        return NULL;
    if (lm->out_channels == 2) {
        GLOG(4, "out channels:2, acmod_out=2\n");
        acmod = 2;
    }
    aud_event_value_set_int(e, "acmod", acmod);
    aud_event_value_set_int(e, "dtsx-enc-mode", lm->dtsx_enc_mode);
    aud_event_value_set_bool(e, "downmix", lm->downmix);
    aud_event_value_set_bool(e, "ch-mapping-lfe-c", lm->ch_mapping_lfe_c);
    return e;
}

/* @0x6009307c lgmix_input: queue one AU; returns NULL (caller drops it) while
 * drop_au > 0, prepends insert_zero silent AUs */
struct mix_input *lgmix_input(struct basemixer *bm, int port, struct aud_data *d)
{
    struct mix_input *in = lgmix_get_input(bm, port);

    if (!in)
        return NULL;
    if (in->drop_au) {
        in->drop_au--;
        return NULL;
    }
    while (in->insert_zero) {
        GLOG(3, "insert initial zero. %d, size %d\n", in->insert_zero, d->size);
        aud_data_adapter_push(&in->fifo, aud_data_new_zero(d->size));
        in->insert_zero--;
    }
    aud_data_adapter_push(&in->fifo, d);
    return in;
}

/* @0x600930f0 lgmix_input_metadata */
int lgmix_input_metadata(struct basemixer *bm, int port, struct aud_data *d)
{
    (void)port;
    aud_data_adapter_push(&((struct lgmixer *)bm)->md, aud_object_get(d));
    return 0;
}

/* @0x60093110 lgmix_output_metadata */
struct aud_data *lgmix_output_metadata(struct basemixer *bm)
{
    struct lgmixer *lm = (struct lgmixer *)bm;

    if (!ADAPTER_AVAIL(&lm->md))
        return NULL;
    return aud_data_adapter_dequeue(&lm->md);
}

/* @0x60093134 build_transform_matrix: Ghidra could not decode (FLIX-heavy).
 * [FLIX] Not reconstructed. */

static void lgmix_setup_input(struct lgmixer *lm, struct lgmix_input *in)
{
    unsigned in_ch, lim;

    if (in->bps_in == 16) {
        in->get_samples = get_samples_default16;
    } else {
        if (in->bps_in != 32)
            GLOG(0, "unknown bps. %d\n", in->bps_in);
        in->get_samples = get_samples_default32;
    }
    in_ch = LAYOUT_CH(in->layoutch);
    in->out_ch = (int)LAYOUT_CH(lm->out_layout);
    in->in_ch = (int)in_ch;
    lim = (aud_basemixer_layout_info(in->layoutch) >> 24) & 0xf;
    in->n_ch = (int)(in_ch < lim ? in_ch : lim);
    build_transform_matrix(lm, in);
}

/* @0x600934d4 lgmix_update_getsamples (2nd symbol of that name): refresh the
 * sample fetcher and matrix of every input that has a FIRST */
int lgmix_update_getsamples(struct lgmixer *lm)
{
    for (struct lgmix_input *in = lgmix_list_first(lm); in; in = lgmix_list_next(lm, in))
        if (in->c.first)
            lgmix_setup_input(lm, in);
    return 0;
}

/* @0x60093554 lgmix_event_first */
int lgmix_event_first(struct basemixer *bm, struct mix_input *mi, struct aud_event *e)
{
    struct lgmixer *lm = (struct lgmixer *)bm;
    struct lgmix_input *in = (struct lgmix_input *)mi;
    int ch = aud_event_value_get_int(e, "channels", 2);
    int acmod = aud_event_value_get_int(e, "acmod", -1);
    bool dts, downmix, lfe_c;
    int out_acmod;

    in->bps_in = aud_event_value_get_int(e, "bits-per-sample", 32);
    aud_event_value_get_int(e, "dtsx-enc-mode", 2);    /* value goes to a FLIX path [FLIX] */
    dts = aud_event_value_get_bool(mi->first, "dts", false);
    downmix = aud_event_value_get_bool(mi->first, "downmix", false);
    lfe_c = aud_event_value_get_bool(mi->first, "ch-mapping-lfe-c", false);

    if (acmod < 0)
        in->layoutch = ((uint32_t)ch << 28) | 2;       /* [FLIX] ch != 2 picks another layout */
    else
        in->layoutch = (uint32_t)aud_basemixer_layout_from_acmod(acmod) | ((uint32_t)ch << 28);
    in->zero_logs = 0;

    out_acmod = lgmix_get_output_acmod(lm);
    if ((unsigned)out_acmod != (lm->out_layout & 0xf))
        lm->out_layout = (uint32_t)aud_basemixer_layout_from_acmod(out_acmod) |
                         (lm->out_layout & 0xf0000000u);
    if (dts) {
        /* [FLIX] @0x600936be: dtsx-enc-mode bookkeeping */
    }
    if (lm->downmix != downmix) {
        lm->downmix = downmix;
        lgmix_toggle_first_event_info(bm, 2);
    }
    if (lm->ch_mapping_lfe_c != lfe_c) {
        lm->ch_mapping_lfe_c = lfe_c;
        lgmix_toggle_first_event_info(bm, 3);
    }
    lgmix_update_getsamples(lm);
    return 0;
}

/* @0x6009375c lgmix_event_last */
int lgmix_event_last(struct basemixer *bm, struct mix_input *mi)
{
    (void)bm;
    ((struct lgmix_input *)mi)->layoutch = 0;
    GLOG(3, "last event, min->in_layoutch = 0\n");
    return 0;
}

/* @0x60093788 lgmix_command */
int lgmix_command(struct basemixer *bm, unsigned cmd, void *p, unsigned len)
{
    (void)bm; (void)cmd; (void)p; (void)len;
    return -1;
}

/* @0x60093790 lgmix_get_nullgen_port */
int lgmix_get_nullgen_port(struct basemixer *bm)
{
    return ((struct lgmixer *)bm)->nullgen_port;
}

/* @0x60093798 lgmix_get_dtsx_enc_mode_changed */
bool lgmix_get_dtsx_enc_mode_changed(struct basemixer *bm)
{
    return ((struct lgmixer *)bm)->dtsx_changed;
}

/* @0x600937a4 lgmix_set_dtsx_enc_mode_changed */
int lgmix_set_dtsx_enc_mode_changed(struct basemixer *bm, bool v)
{
    ((struct lgmixer *)bm)->dtsx_changed = v;
    return 0;
}

/* @0x600937b4 lgmix_toggle_first_event_info */
int lgmix_toggle_first_event_info(struct basemixer *bm, unsigned bit)
{
    ((struct lgmixer *)bm)->first_info ^= 1u << (bit & 31);
    return 0;
}

/* @0x600937cc lgmix_clear_first_event_info */
int lgmix_clear_first_event_info(struct basemixer *bm, unsigned bit)
{
    ((struct lgmixer *)bm)->first_info &= ~(1u << (bit & 31));
    return 0;
}

/* @0x600937ec lgmix_get_first_event_info */
unsigned lgmix_get_first_event_info(struct basemixer *bm)
{
    return ((struct lgmixer *)bm)->first_info;
}

/* @0x600937f4 lgmixer_prop_updated: "out-layout" changed */
int lgmixer_prop_updated(struct lgmixer *lm, const struct prop_desc *prop)
{
    if (prop->off != 0x180)
        return 0;
    lm->out_layout = (uint32_t)aud_basemixer_layout_from_name(lm->out_layout_name) |
                     ((uint32_t)lm->out_channels << 28);
    lgmix_update_getsamples(lm);
    return 0;
}

/* @0x600938bc lgmixer_set_port_info: "port-info=<port<<8 | target>", target 0xff removes */
int lgmixer_set_port_info(struct lgmixer *lm, const void *prop, const char *val)
{
    unsigned v = (unsigned)_Stoul(val, NULL, 0);
    unsigned target = v & 0xff;
    int port = (int)v >> 8;
    struct port_info *pi;
    int ret = 0;

    (void)prop;
    aud_mutex_lock(&lm->pi_lock);
    for (pi = lm->pi_head; pi; pi = pi->next) {
        if (pi->port != port)
            continue;
        if (target == 0xff) {
            GLOG(3, "remove port%d infos\n", port);
            list_del_(pi);
            aud_free(pi);
        } else {
            GLOG(3, "find and set port mixer %d\n", target);
            pi->target = (int)target;
        }
        goto out;
    }
    if (target != 0xff) {
        pi = calloc(sizeof(*pi), 1);
        if (!pi) {
            GLOG(0, "no mem\n");
            ret = -1;
            goto out;
        }
        pi->target = (int)target;
        pi->port = port;
        list_add_tail_(&lm->pi_head, pi);
    }
out:
    aud_mutex_unlock(&lm->pi_lock);
    return ret;
}

/* @0x600939e8 lgmixer_constructor */
int lgmixer_constructor(struct lgmixer *lm)
{
    GLOG(3, "create uid %08x, %s\n", lm->bm.m.uid, lm->bm.m.name);
    lm->in_head = lm->in_tail = NULL;
    lm->pi_head = lm->pi_tail = NULL;
    aud_mutex_init(&lm->pi_lock);
    lm->dtsx_enc_mode = -1;
    lm->nullgen_port = -1;
    lm->ch_mapping_lfe_c = false;
    lm->downmix = false;
    lm->dtsx_changed = false;
    lm->output_connect = true;
    lm->out_layout = 0x20000002u;                      /* 2ch "lr" (DAT_60060300) */
    lm->out_channels = 2;
    lm->first_info = 0;
    aud_data_adapter_init(&lm->md);
    if (aud_basemixer_init(&lm->bm) >= 0)
        return 0;
    GLOG(0, "Oops\n");
    return -1;
}

/* @0x60093a90 lgmixer_destructor */
void lgmixer_destructor(struct lgmixer *lm)
{
    struct lgmix_input *in;
    struct port_info *pi;

    aud_basemixer_cleanup(&lm->bm);
    aud_data_adapter_cleanup(&lm->md);
    while ((in = lgmix_list_first(lm))) {
        list_del_(in);
        aud_data_adapter_cleanup(&in->c.fifo);
        if (in->c.first)
            aud_object_put(in->c.first);
        aud_free(in);
    }
    aud_mutex_lock(&lm->pi_lock);
    while ((pi = lm->pi_head)) {
        list_del_(pi);
        aud_free(pi);
    }
    aud_mutex_unlock(&lm->pi_lock);
}

extern const char lgmixer_class[];                     /* @0x60060314 */

/* @0x60093b50 __aud_object_register_mixer */
void __aud_object_register_mixer(void)
{
    aud_object_class_register(lgmixer_class);
}

/* @0x60093b60 aud_object_class_init_mixer: class "mixer" (inherits basemixer) */
int aud_object_class_init_mixer(void **impl)
{
    impl[0x60 / 4] = (void *)lgmix_get_input;
    impl[0x64 / 4] = (void *)lgmix_do_mix;
    impl[0x68 / 4] = (void *)lgmix_can_mix;
    impl[0x6c / 4] = (void *)lgmix_get_first_event;
    impl[0x70 / 4] = (void *)lgmix_input;
    impl[0x74 / 4] = (void *)lgmix_input_metadata;
    impl[0x78 / 4] = (void *)lgmix_output_metadata;
    impl[0x7c / 4] = (void *)lgmix_event_first;
    impl[0x80 / 4] = (void *)lgmix_event_last;
    impl[0x88 / 4] = (void *)lgmix_command;
    impl[0x8c / 4] = (void *)lgmix_get_nullgen_port;
    impl[0x90 / 4] = (void *)lgmix_get_dtsx_enc_mode_changed;
    impl[0x94 / 4] = (void *)lgmix_set_dtsx_enc_mode_changed;
    impl[0x98 / 4] = (void *)lgmix_toggle_first_event_info;
    impl[0x9c / 4] = (void *)lgmix_clear_first_event_info;
    impl[0xa0 / 4] = (void *)lgmix_get_first_event_info;
    impl[0x04 / 4] = (void *)"mixer";
    impl[0x08 / 4] = (void *)lgmixer_constructor;
    impl[0x0c / 4] = (void *)lgmixer_destructor;
    impl[0x1c / 4] = (void *)lgmixer_prop_updated;
    impl[0x2c / 4] = (void *)(uintptr_t)0x1c0;
    aud_module_impl_add_prop(impl, "underflow-count", 2, 0x194, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "out-layout", 4, 0x180, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "out-channels", 2, 0x184, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "main-audio-port", 2, 0x188, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "port-info", 2, 0x188, (void *)lgmixer_set_port_info, NULL, 0);
    aud_module_impl_add_prop(impl, "nullgen-port", 2, 0x18c, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "output-connect", 1, 0x190, NULL, NULL, 0);
    return 0;
}

/* @0x60093c7c __aud_mreg_class_mixer */
void __aud_mreg_class_mixer(void)
{
    aud_mregistry_add_class(lgmixer_class);
}

/* ------------------------------------------------------------------------
 * "mixerbin": the module the kernel adds to the "mixer" pipeline
 *
 * mixerbin_constructor (observed):
 *   children: mux (+0x120), admix (+0x118, msmixer class), pcmren (+0x11c),
 *             nullgen (+0x134), mixer[0..3] (+0x124.., class "mixer")
 *   admix.out0 -> pcmren.in0
 *   for i in 0..3:
 *     mixer[i] props "out-channels=%d\out-layout=%s" with {2,10,8,6} / {"lr","acmod7","acmod7","acmod7"}
 *     mux.out(((0x10+i)<<8)|i) -> mixer[i].in0x1f      (PCM renderer output i)
 *     pcmren.out i            -> mux.in(0x10+i)
 *     mux.out(0xf00+i)        -> mixer[i].in0xf        (nullgen silence)
 *     mixer[i] "nullgen-port=0xf", "output-connect=1"; bin output i = mixer[i].out0
 *   nullgen.out0 -> mux.in0xf, "nullgen-start=1"
 * External input port N is attached on its FIRST event (mixerbin_event):
 *   "dolby" == false -> bin input N = mux.in N; mux.out(N<<8|i) -> mixer[i].in N
 *   "dolby" == true  -> bin input N = admix.in N (MS12 mixer -> pcmren -> mux)
 * ---------------------------------------------------------------------- */
struct mb_input { struct mb_input *next, *prev; int port; bool dolby; bool dts; };

struct mixerbin {
    struct aud_module m;
    struct aud_module *admix;            /* +0x118 */
    struct aud_module *pcmren;           /* +0x11c */
    struct aud_module *mux;              /* +0x120 */
    struct aud_module *mixer[4];         /* +0x124 */
    struct aud_module *nullgen;          /* +0x134 */
    struct mb_input  *in_head, *in_tail; /* +0x138 */
    int      connect_in_out;             /* +0x140 "connect-in-out" */
    int      ext_pcm_acmod;              /* +0x144 "ext-pcm-acmod" */
};

extern void aud_module_bin_unset_input(struct mixerbin *, int port);
extern void aud_module_bin_set_input(struct mixerbin *, int port, struct aud_module *, int child_port);
extern void aud_module_bin_set_output(struct mixerbin *, int port, struct aud_module *, int child_port);
extern void aud_module_bin_unset_output_no_del_port(struct mixerbin *, int port);
extern struct aud_module *aud_module_bin_add(struct mixerbin *, const char *impl);
extern int  aud_module_bin_connect(struct mixerbin *, struct aud_module *out, int oport,
                                   struct aud_module *in, int iport);
extern void aud_module_bin_add_cmd_propagate(struct mixerbin *, struct aud_module *);
extern void aud_module_bin_propagate_pending(struct mixerbin *);
extern int  aud_connector_disconnect(struct aud_module *out, int oport, struct aud_module *in, int iport);
extern struct aud_port *aud_module_get_input(struct aud_module *, int port);
extern int  aimc_set_property(void *imc, int uid, const char *fmt, ...);
extern volatile unsigned *mixerbin_log_mask;            /* 0x4ff900c0 */
extern void *mixerbin_log_ctx;                          /* 0x60102d60.. */
#define BLOG(lvl, ...) \
    do { if (*mixerbin_log_mask & (1u << (lvl))) \
        logmm_printl(mixerbin_log_ctx, lvl, __func__, __LINE__, __VA_ARGS__); } while (0)

static const int mixer_out_channels[4] = { 2, 10, 8, 6 };            /* @0x601031e0 */
static const char *const mixer_out_layout[4] = { "lr", "acmod7", "acmod7", "acmod7" };

static void send_port_info_mixer_lg(struct mixerbin *mb, unsigned info)
{
    BLOG(3, "send port info 0x%04X to lg mixer\n", info);
    for (int i = 0; i < 4; i++)
        if (mb->mixer[i])
            aimc_set_property(mb->m.imc, mb->mixer[i]->uid, "port-info=%d", info);
}

/* @0x60097770 mixerbin_initializer ("initializer" in syms): ModuleRegistry entry */
void mixerbin_initializer(void)
{
    extern int ModuleRegistry_Register(const void *);
    extern const char mixerbin_registry[];             /* @0x60102d70 area */

    ModuleRegistry_Register(mixerbin_registry);
}

/* @0x60097780 input_del: detach external input <port> */
void input_del(struct mixerbin *mb, int port)
{
    struct mb_input *bi;

    for (bi = mb->in_head; bi && bi->port != port; bi = bi->next)
        ;
    if (!bi)
        return;
    BLOG(3, "delete input%d\n", port);
    aud_module_bin_unset_input(mb, port);
    if (!bi->dolby && !bi->dts) {
        for (int i = 0; i < 4; i++)
            if (mb->mixer[i]) {
                BLOG(3, "disconnect mux.%04x -> omix[%d]\n", (port << 8) + i, i);
                aud_connector_disconnect(mb->mux, (port << 8) + i, mb->mixer[i], port);
            }
    }
    send_port_info_mixer_lg(mb, ((unsigned)port << 8) | 0xff);
    list_del_(bi);
    aud_free(bi);
}

/* @0x60097894 mixerbin_event (FIRST handling inlined as "event_first",
 * send_last_before_del inlined). Port lookup and the FIRST flow are observed;
 * several conditional moves are FLIX [FLIX?]. */
int mixerbin_event(struct mixerbin *mb, struct aud_port *port, struct aud_event *e)
{
    struct mb_input *bi;
    int pnum;
    bool dolby, dts, seamless, changed = true;

    BLOG(3, "%s : port%d, event %s\n", mb->m.name, port ? port->num : -1, aud_event_name(e));
    if (!port)
        return ((int (*)(struct mixerbin *, struct aud_port *, struct aud_event *))
                ((void **)mb->m.impl)[0x68 / 4])(mb, port, e);   /* bin default event */
    pnum = port->num;

    if (e->type != EV_FIRST) {
        for (bi = mb->in_head; bi; bi = bi->next)
            if (bi->port == pnum)
                return ((int (*)(struct mixerbin *, struct aud_port *, struct aud_event *))
                        ((void **)mb->m.impl)[0x68 / 4])(mb, port, e);
        BLOG(1, "input port (%d) is not connected. event (%d)\n", pnum, e->type);
        aud_object_put(e);
        return 0;
    }

    dolby = aud_event_value_get_bool(e, "dolby", false);
    dts = aud_event_value_get_bool(e, "dts", false);
    seamless = aud_event_value_get_bool(e, "seamless-mode", false);
    for (bi = mb->in_head; bi && bi->port != pnum; bi = bi->next)
        ;
    if (bi) {
        changed = !(bi->dolby == dolby && bi->dts == dts);
        BLOG(3, "dts   : %d -> %d\n", bi->dts, dts);
    }
    if (bi && !changed)
        goto done;                                     /* same routing: nothing to rewire */
    if (bi) {
        /* send_last_before_del: give the old target a LAST before unplugging */
        struct aud_module *tgt = bi->dolby ? mb->admix : mb->mux;

        BLOG(3, "send last before delete port%04x..\n", pnum);
        if (tgt) {
            struct aud_port *ip = aud_module_get_input(tgt, pnum);
            if (ip && tgt->impl) {
                ((int (*)(struct aud_module *, struct aud_port *, struct aud_event *))
                 ((void **)tgt->impl)[0x14 / 4])(tgt, ip, aud_event_new_last());
                BLOG(3, "done.\n");
            }
        }
        input_del(mb, pnum);
    }

    bi = calloc(1, sizeof(*bi));
    if (!bi) {
        BLOG(0, "no mem\n");
        goto done;
    }
    bi->port = pnum;
    bi->dolby = aud_event_value_get_bool(e, "dolby", false);
    bi->dts = aud_event_value_get_bool(e, "dts", false);

    if (!dolby) {
        for (int i = 0; i < 4; i++)
            if (mb->mixer[i]) {
                aud_module_bin_connect(mb, mb->mux, (pnum << 8) + i, mb->mixer[i], pnum);
                aimc_set_property(mb->m.imc, mb->mux->uid, "add_input=%d", pnum);
            }
        aud_module_bin_set_input(mb, pnum, mb->mux, pnum);
        send_port_info_mixer_lg(mb, ((unsigned)pnum << 8) | (unsigned)pnum);
    } else {
        int n = 0;

        aud_module_bin_set_input(mb, pnum, mb->admix, pnum);
        send_port_info_mixer_lg(mb, ((unsigned)pnum << 8) | 0x1f);
        if (seamless)
            for (struct mb_input *x = mb->in_head; x; x = x->next)
                if (!x->dolby)
                    n++;
        aimc_set_property(mb->m.imc, mb->pcmren->uid, "mixing-num=%d", n);
    }
    list_add_tail_(&mb->in_head, bi);
done:
    aud_module_bin_propagate_pending(mb);
    BLOG(4, "done\n");
    return 0;
}

/* @0x60097e6c mixerbin_constructor (topology above) */
int mixerbin_constructor(struct mixerbin *mb)
{
    int (*parent_ctor)(struct mixerbin *) = ((void **)mb->m.impl)[0x60 / 4];

    if (parent_ctor)
        parent_ctor(mb);
    mb->in_head = mb->in_tail = NULL;
    mb->mux = aud_module_bin_add(mb, "mux");
    mb->admix = aud_module_bin_add(mb, "admix");
    mb->pcmren = aud_module_bin_add(mb, "pcmren");
    mb->nullgen = aud_module_bin_add(mb, "nullgen");
    aud_module_bin_connect(mb, mb->admix, 0, mb->pcmren, 0);
    aud_module_bin_add_cmd_propagate(mb, mb->mux);
    aud_module_bin_add_cmd_propagate(mb, mb->admix);
    aud_module_bin_add_cmd_propagate(mb, mb->pcmren);
    aud_module_bin_add_cmd_propagate(mb, mb->nullgen);

    for (int i = 0; i < 4; i++) {
        struct aud_module *mx = aud_module_bin_add(mb, "mixer");

        mb->mixer[i] = mx;
        if (mx)
            aimc_set_property(mb->m.imc, mx->uid, "out-channels=%d\\out-layout=%s",
                              mixer_out_channels[i], mixer_out_layout[i]);
        aud_module_bin_connect(mb, mb->mux, ((0x10 + i) << 8) + i, mx, 0x1f);
        aud_module_bin_connect(mb, mb->pcmren, i, mb->mux, 0x10 + i);
        aud_module_bin_connect(mb, mb->mux, 0xf00 + i, mb->mixer[i], 0xf);
        aimc_set_property(mb->m.imc, mb->mixer[i]->uid, "nullgen-port=0xf");
        aimc_set_property(mb->m.imc, mb->mixer[i]->uid, "output-connect=1");
        aud_module_bin_set_output(mb, i, mb->mixer[i], 0);
    }
    if (mb->nullgen) {
        aud_module_bin_connect(mb, mb->nullgen, 0, mb->mux, 0xf);
        aimc_set_property(mb->m.imc, mb->nullgen->uid, "nullgen-start=1");
    }
    mb->ext_pcm_acmod = 0;
    return 0;
}

/* @0x60097fdc mixerbin_destructor */
void mixerbin_destructor(struct mixerbin *mb)
{
    void (*parent_dtor)(struct mixerbin *) = ((void **)mb->m.impl)[0x64 / 4];
    struct mb_input *bi = mb->in_head;

    /* only the first listed input is unplugged here (observed) */
    if (bi) {
        BLOG(3, "delete input%d\n", bi->port);
        if (!bi->dolby) {
            for (int i = 0; i < 4; i++) {
                if (mb->mixer[i]) {
                    BLOG(3, "disconnect mux.%04x -> omix[%d]\n", (bi->port << 8) + i, i);
                    aud_connector_disconnect(mb->mux, (bi->port << 8) + i, mb->mixer[i], bi->port);
                }
                aud_connector_disconnect(mb->pcmren, i, mb->mux, 0x10 + i);
                aud_connector_disconnect(mb->mux, ((0x10 + i) << 8) + i, mb->mixer[i], 0x1f);
            }
        }
        list_del_(bi);
        aud_free(bi);
    }
    aud_connector_disconnect(mb->admix, 0, mb->pcmren, 0);
    if (parent_dtor)
        parent_dtor(mb);
}

/* @0x600984cc mixerbin_set_prop: "connect-in-out" selects what drives bin out4 */
int mixerbin_set_prop(struct mixerbin *mb, const char *const *prop, const char *val)
{
    unsigned type = (unsigned)_Stoul(val, NULL, 0);
    struct aud_module *src;
    int child_port;

    BLOG(3, "%s: set %s : %s\n", mb->m.name, prop[0], val);
    switch (type) {
    case 1: src = mb->admix;  child_port = 0; break;
    case 2: src = mb->pcmren; child_port = 0; break;
    case 3: src = mb->pcmren; child_port = 1; break;
    case 4: src = mb->pcmren; child_port = 2; break;
    default:
        BLOG(0, "unknown input type(%u)\n", type);
        return 0;
    }
    if (mb->connect_in_out)
        aud_module_bin_unset_output_no_del_port(mb, 4);
    if (src)
        aud_module_bin_set_output(mb, 4, src, child_port);
    mb->connect_in_out = (int)type;
    return 0;
}

/* @0x6009858c mixerbin_set_ext_pcm_acmod */
int mixerbin_set_ext_pcm_acmod(struct mixerbin *mb, const void *prop, const char *val)
{
    (void)prop;
    mb->ext_pcm_acmod = (int)_Stoul(val, NULL, 0);
    return 0;
}

extern const char mixerbin_class[];                    /* @0x60060acc */

/* @0x600985a4 __aud_object_register_mixerbin */
void __aud_object_register_mixerbin(void)
{
    aud_object_class_register(mixerbin_class);
}

/* @0x600985b4 aud_object_class_init_mixerbin: props table @0x60102ee0 (stride 0x28):
 * mixing-mode, mixing-mode-int, enable-limiter, main-audio-port, fading-* ->
 * msmixer_set/get_prop (forward to admix); drc-mode, dmx-mode, metadata-type,
 * output-type, atmos-lock -> pcmren_set/get_prop; enc-mode -> dtsren (no-op);
 * nullgen-start -> ullgen; disconnect-output/connect-output -> lgmixer_set/get_prop;
 * connect-in-out (+0x140) -> mixerbin_set_prop; ext-pcm-acmod (+0x144). */
int aud_object_class_init_mixerbin(void **impl)
{
    extern const void *mixerbin_props;                 /* @0x60102ee0 */

    impl[0x0c / 4] = (void *)mixerbin_destructor;
    impl[0x08 / 4] = (void *)mixerbin_constructor;
    impl[0x14 / 4] = (void *)mixerbin_event;
    impl[0x30 / 4] = (void *)&mixerbin_props;
    impl[0x2c / 4] = (void *)(uintptr_t)0x148;
    impl[0x04 / 4] = ((void **)impl[0])[0x18 / 4];     /* name from the class object */
    return 0;
}

/* @0x600985dc __aud_mreg_class_mixerbin */
void __aud_mreg_class_mixerbin(void)
{
    aud_mregistry_add_class(mixerbin_class);
}

/* @0x600981f4 lgmixer_get_prop: "connect-output"/"disconnect-output" -> each
 * mixer's "output-connect" as "%d " list */
char *lgmixer_get_prop(struct mixerbin *mb, const char *const *prop)
{
    extern int aud_module_get_prop(struct aud_module *, const char *);
    char buf[64] = {0};
    int n = 0;

    if (prop[0] && (!strcmp(prop[0], "disconnect-output") || !strcmp(prop[0], "connect-output")))
        for (int i = 0; i < 4; i++)
            if (mb->mixer[i]) {
                int v = aud_module_get_prop(mb->mixer[i], "output-connect");
                BLOG(3, "%s.i get %s : %d\n", mb->m.name, prop[0], v);
                n += CommonBuffer_InsertAuInternal(buf + n, "%d ", v);
            }
    return strdup(buf);
}

/* @0x60098288 lgmixer_set_prop: "connect-output=<i>" / "disconnect-output=<i>"
 * -> mixer[i] "output-connect=1/0". Any other name: silently ignored. */
int lgmixer_set_prop(struct mixerbin *mb, const char *const *prop, const char *val)
{
    extern int aud_module_set_prop(struct aud_module *, const char *, unsigned);
    extern size_t strlen(const char *);
    int idx = (int)_Stoul(val, NULL, 0);
    char *s;

    BLOG(3, "%s: set %s to %d\n", mb->m.name, prop[0], idx);
    if (idx >= 4) {
        BLOG(0, "wrong index of lg mixer. %d\n", idx);
        return 0;
    }
    if (!mb->mixer[idx])
        return 0;
    if (!strcmp(prop[0], "disconnect-output"))
        s = audutil_strdup_printf("output-connect=0");
    else if (!strcmp(prop[0], "connect-output"))
        s = audutil_strdup_printf("output-connect=1");
    else
        return 0;
    if (s) {
        BLOG(3, "send to %d as %s\n", idx, s);
        aud_module_set_prop(mb->mixer[idx], s, (unsigned)strlen(s));
        aud_free(s);
    }
    return 0;
}

/* ------------------------------------------------------------------------
 * "mux": fans every bin input out to the 4 LG mixers. Most of mux_chain is
 * FLIX-scheduled; constructor/buffers/locking helpers are observed.
 * ---------------------------------------------------------------------- */
struct mux_ren_buf { int is_dts; uint64_t full_mask; int _c; struct aud_data **data, **meta, **ext; };

struct mux {
    struct aud_module m;
    int      n_output;                   /* +0xf0 "n_output" (4) */
    int      pcmren_in_start;            /* +0xf4 "pcmren_in_start" (0x10) */
    int      _f8;                        /* +0xf8 (0x20) */
    int      last_pending;               /* +0x108 */
    aud_mutex_t last_lock;               /* +0x10c */
    aud_cond_t  last_cond;               /* +0x128 */
    int      chains;                     /* +0x138 chains in progress */
    aud_mutex_t chain_lock;              /* +0x13c */
    aud_cond_t  chain_cond;              /* +0x158 */
    int      waiters;                    /* +0x168 */
    aud_mutex_t wait_lock;               /* +0x16c */
    aud_cond_t  wait_cond;               /* +0x188 */
    struct mux_ren_buf pcmrend;          /* +0x198 */
    struct mux_ren_buf dtsrend;          /* +0x1b8 */
};

extern volatile unsigned *mux_log_mask;                 /* 0x4ff900d0 */
extern void *mux_log_ctx;                               /* 0x60103200 */
extern int mux_wait_timeout;                            /* DAT_6005f190 */
extern void aud_cond_wait_timeout(void *, void *, int);
#define XLOG(lvl, ...) \
    do { if (*mux_log_mask & (1u << (lvl))) \
        logmm_printl(mux_log_ctx, lvl, __func__, __LINE__, __VA_ARGS__); } while (0)

/* @0x600985e8 mux_wait_last_output */
void mux_wait_last_output(struct mux *x)
{
    aud_mutex_lock(&x->last_lock);
    if (x->last_pending) {
        XLOG(5, "[%d] wait until last number 0..\n", x->last_pending);
        aud_cond_wait(&x->last_cond, &x->last_lock);
        XLOG(5, "[%d] wake !!!!\n", x->last_pending);
    }
    aud_mutex_unlock(&x->last_lock);
}

/* @0x60098658 mux_wait_chain_done: port numbers >= 0x30 are rejected
 * ("port number %d is wrong!!") [FLIX] for the in-range test */
void mux_wait_chain_done(struct mux *x, int use_chain, int port)
{
    if (port >= 0x30) {
        XLOG(0, "port number %d is wrong!!\n", port);
    }
    if (use_chain != 1)
        return;
    aud_mutex_lock(&x->wait_lock);
    x->waiters++;
    aud_mutex_unlock(&x->wait_lock);
    aud_mutex_lock(&x->chain_lock);
    XLOG(5, "port%04x use_chain %d enabled %d\n", port, x->chains, 0);
    while (x->chains > 0) {
        XLOG(5, "port%04x use_chain %d\n", port, x->chains);
        aud_cond_wait_timeout(&x->chain_cond, &x->chain_lock, mux_wait_timeout);
    }
    XLOG(5, "port%04x use_chain %d wake!!! lock chains...\n", port, x->chains);
    /* chain_lock stays held; released by mux_unlock_chain */
}

/* @0x60098764 mux_unlock_chain */
void mux_unlock_chain(struct mux *x, int unused, int port)
{
    (void)unused;
    aud_mutex_unlock(&x->chain_lock);
    aud_mutex_lock(&x->wait_lock);
    x->waiters--;
    aud_mutex_unlock(&x->wait_lock);
    while (x->waiters && x->chains == 0) {
        XLOG(5, "port%04x send signal... %d\n", port, x->waiters);
        aud_cond_signal(&x->chain_cond);
        aud_mutex_lock(&x->wait_lock);
        aud_cond_wait_timeout(&x->wait_cond, &x->wait_lock, mux_wait_timeout);
        aud_mutex_unlock(&x->wait_lock);
        XLOG(5, "port%04x awake !!! %d\n", port, x->waiters);
    }
    aud_cond_signal(&x->wait_cond);
}

/* @0x6009886c mux_chain ("mux_chai" in functions.tsv): copy each AU to every
 * enabled output (port<<8 | i), metadata first ("push metadata to 0x%04x"),
 * then data ("push buffer to 0x%04x") + aud_module_sync per output. Only the
 * entry, the per-output loop and its push/sync calls are observed; the PCMREND
 * vs DTSREND buffer selection and the output-enable mask tests are FLIX. [FLIX]
 * There is no data-type test besides META routing. */
int mux_chain(struct aud_port *port, struct aud_data *d)
{
    struct mux *x = (struct mux *)port->module;
    struct mux_ren_buf *rb = x->_f8 ? &x->pcmrend : &x->dtsrend;   /* [FLIX?] */
    int base = port->num;

    aud_mutex_lock(&x->chain_lock);
    x->chains++;
    aud_mutex_unlock(&x->chain_lock);
    (void)rb;
    for (int i = 0; i < x->n_output; i++) {
        int out = (base << 8) + i;

        aud_module_push_data(&x->m, out, aud_object_get(d));   /* [FLIX] enable test */
        aud_module_sync(&x->m, out);
    }
    aud_object_put(d);
    aud_mutex_lock(&x->chain_lock);
    x->chains--;
    aud_mutex_unlock(&x->chain_lock);
    return 0;
}

/* @0x60098ca0 mux_event: [FLIX] beyond the log; NULL event -> -1 */
int mux_event(struct mux *x, struct aud_port *port, struct aud_event *e)
{
    if (!e)
        return -1;
    XLOG(3, "%s : port%04x, event %s\n", x->m.name, port ? port->num : -1, aud_event_name(e));
    /* [FLIX] remainder (per-output FIRST/LAST fan-out) not decoded */
    return aud_module_default_event(&x->m, port, e);
}

/* @0x60098f50 mux_cmd */
int mux_cmd(struct mux *x, unsigned cmd, void *param, unsigned len)
{
    return aud_module_default_command(&x->m, cmd, param, len);
}

static int mux_alloc_bufs(struct mux_ren_buf *b, int n)
{
    extern uint64_t __ashldi3(uint64_t, int);

    aud_free(b->meta);
    aud_free(b->data);
    aud_free(b->ext);
    b->data = calloc(1, (size_t)n * 4);
    b->meta = calloc(1, (size_t)n * 4);
    b->ext = calloc(1, (size_t)n * 4);
    for (int i = 0; i < n; i++)
        b->full_mask |= __ashldi3(1, i);
    if (b->meta && b->data && b->ext) {
        XLOG(4, "%s : bits_out_full is 0x%04x\n", b->is_dts ? "DTSREND" : "PCMREND",
             (unsigned)b->full_mask);
        return 0;
    }
    aud_free(b->meta);
    aud_free(b->data);
    aud_free(b->ext);
    b->meta = b->data = b->ext = NULL;
    return -1;
}

/* @0x60098f68 mux_ren_buf_init */
int mux_ren_buf_init(struct mux *x, struct mux_ren_buf *b, int n)
{
    (void)x;
    return mux_alloc_bufs(b, n);
}

/* @0x60099030 mux_set_n_output: "n_output" setter (rebuilds the PCMREND buffers) */
int mux_set_n_output(struct mux *x, const void *prop, const char *val)
{
    int n = (int)_Stoul(val, NULL, 0);

    (void)prop;
    if (mux_alloc_bufs(&x->pcmrend, n))
        return -1;
    XLOG(3, "%s: n_output %u -> %u\n", x->m.name, x->n_output, n);
    x->n_output = n;
    return 0;
}

/* @0x60099158 mux_send_last: "send_last=<port>" pushes a LAST into that input.
 * [FLIX] the call into the port's event path is not decoded. */
int mux_send_last(struct mux *x, const void *prop, const char *val)
{
    extern struct aud_port *aud_module_get_input_port(struct aud_module *, int);
    int p = (int)_Stoul(val, NULL, 0);
    struct aud_port *ip = aud_module_get_input_port(&x->m, p);

    (void)prop;
    if (ip) {
        struct aud_event *e = aud_event_new_last();
        if (e)
            mux_event(x, ip, e);                       /* [FLIX] */
    }
    return 0;
}

/* @0x60099438 mux_constructor */
int mux_constructor(struct mux *x)
{
    aud_mutex_init(&x->last_lock);
    aud_mutex_init(&x->chain_lock);
    aud_mutex_init(&x->wait_lock);
    aud_cond_init(&x->last_cond);
    aud_cond_init(&x->chain_cond);
    aud_cond_init(&x->wait_cond);
    aud_mutex_lock(&x->chain_lock);
    x->chains = 0;
    aud_mutex_unlock(&x->chain_lock);
    x->pcmren_in_start = 0x10;
    x->n_output = 4;
    x->_f8 = 0x20;
    aud_mutex_lock(&x->wait_lock);
    x->waiters = 0;
    aud_mutex_unlock(&x->wait_lock);
    mux_ren_buf_init(x, &x->pcmrend, x->n_output);
    mux_ren_buf_init(x, &x->dtsrend, x->n_output);
    x->pcmrend.is_dts = 0;
    return 0;
}

/* @0x600994c8 mux_destructor */
void mux_destructor(struct mux *x)
{
    aud_cond_signal(&x->last_cond);
    aud_cond_signal(&x->chain_cond);
    aud_free(x->dtsrend.meta);
    aud_free(x->dtsrend.data);
    aud_free(x->dtsrend.ext);
    x->pcmrend = (struct mux_ren_buf){0};
    x->dtsrend.data = x->dtsrend.meta = x->dtsrend.ext = NULL;
}

/* @0x6009951c mux_init */
void mux_init(void)
{
    extern int ModuleRegistry_Register(const void *);
    extern const char mux_class[];                     /* @0x60103210: n_output, pcmren_in_start, send_last */

    ModuleRegistry_Register(mux_class);
}
