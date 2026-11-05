/*
 * dsp2 "msmixer" / "admix" / "sysmix": the Dolby MS12 mixer classes built on
 * basemixer (see mixer.c). Inside "mixerbin" the instance is "admix"; it takes
 * the bin inputs whose FIRST event carries dolby=true and feeds "pcmren".
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Linked at 0x60000000. Evidence: ../../decomp/audio_a0_dsp2.main_60000000.bin.c
 * (0x60093c88..0x60096270), command table @0x60006370, listing
 * tmp/a18/asm/msmixer_do_mix.asm, tmp/a18/asm/msmixer_can_mix.asm.
 *
 * ES (observed): no msmixer function reads the "es" key or tests aud_data.type
 * other than msmixer_input_metadata's "type != META(2) -> 'not metadata', -1".
 * Inputs are fed to ms_mixer_main_process_run as PCM. The ES bypass chain
 * ("mixer_byin" / "mixer_bypass") never connects to admix, so ES is not
 * touched here.
 *
 * Three MS12 input slots: 0 = main (= "main-audio-port"), 1 = associated /
 * secondary, 2 = system. Port p maps to slot 0 if p == main, 2 if p == 2, else 1.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct aud_event;
struct aud_data { char _h[0x10]; int type; char _p[0x20]; int size; void *buf; };
struct aud_adapter { char _h[0x10]; int wr; int rd; int _18; };
#define ADAPTER_AVAIL(a) ((a)->wr - (a)->rd)

struct mix_input {                       /* basemixer input head, see mixer.c */
    struct mix_input *next, *prev;
    int      port;                       /* +0x08 */
    bool     enabled;                    /* +0x0c */
    bool     got_last;                   /* +0x0d */
    int      channels;                   /* +0x10 */
    int      bps;                        /* +0x14 */
    int      _18;
    int      frame_bytes;                /* +0x1c */
    bool     has_data;                   /* +0x20 */
    int      frames_in;                  /* +0x24 */
    int      drop_au;                    /* +0x28 */
    int      insert_zero;                /* +0x2c */
    bool     low_delay;                  /* +0x30 */
    bool     zero_mixing;                /* +0x31 */
    struct aud_event *first;             /* +0x34 */
    struct aud_adapter fifo;             /* +0x38 */
    int      acmod;                      /* +0x54 */
    int      _58;
    unsigned zero_logs;                  /* +0x5c */
};

struct msf_connector { int _c[7]; };     /* 0x1c bytes, per input slot */

struct msmixer {
    char     core[0x150];                /* struct basemixer */
    const char *mixing_mode;             /* +0x150 "mixing-mode" (string) */
    int      mixing_mode_int;            /* +0x154 "mixing-mode-int" */
    bool     mode_dirty;                 /* +0x158 */
    int      underflow_count;            /* +0x15c "underflow-count" */
    const char *fading[3];               /* +0x160 "fading-primary/secondary/system" */
    bool     enable_limiter;             /* +0x16c "enable-limiter" */
    int      main_audio_port;            /* +0x170 "main-audio-port" */
    int      main_alt;                   /* +0x174 used when main_audio_port > 1 */
    struct mix_input *in_head, *in_tail; /* +0x17c */
    char     lib[0x8];                   /* +0x184 MS12 mixer library handle (ms_mixer_*) */
    struct msf_connector msf[3];         /* +0x18c metadata connectors */
    char     md_out[0x1c];               /* +0x1fc output metadata */
    int      _218;                       /* +0x218 lib work buffer (cleared on reconfig) */
    int      buffered, consumed;         /* +0x220, +0x224 "lib-buf-info" */
    int      _228;
    int      lib_work_size;              /* +0x22c */
    int      fade_ok;                    /* +0x230 */
    int      mixing_type;                /* +0x240 MSMIX_CMD_SET_MIXING_TYPE */
    int      _244;
    int      frames_per_slot[3];         /* +0x248 */
    bool     lib_ready;                  /* +0x2bc */
    int      _2c0[3];
    bool     bypass_admix;               /* +0x2cc FIRST "bypass-admix" */
    struct aud_adapter md_fifo;          /* +0x2d0 metadata FIFO for bypass-admix */
    int      ext_pcm_acmod;              /* +0x2ec "ext-pcm-acmod" */
    bool     downmix;                    /* +0x2f0 */
    bool     ch_mapping_lfe_c;           /* +0x2f1 */
    unsigned first_info;                 /* +0x2f4 */
};

extern volatile unsigned *msmixer_log_mask;             /* 0x4ff900c4 */
extern void *msmixer_log_ctx;                           /* 0x60102ce4 */
extern void logmm_printl(void *ctx, int lvl, const char *fn, int line, const char *fmt, ...);
#define SLOG(lvl, ...) \
    do { if (*msmixer_log_mask & (1u << (lvl))) \
        logmm_printl(msmixer_log_ctx, lvl, __func__, __LINE__, __VA_ARGS__); } while (0)

extern void *calloc(size_t, size_t);
extern void  aud_free(void *);
extern char *strdup(const char *);
extern int   strcmp(const char *, const char *);
extern unsigned long _Stoul(const char *, char **, int);
extern void *aud_object_get(void *);
extern void  aud_object_put(void *);
extern int   aud_event_value_get_int(struct aud_event *, const char *, int);
extern bool  aud_event_value_get_bool(struct aud_event *, const char *, bool);
extern void  aud_event_value_set_int(struct aud_event *, const char *, int);
extern void  aud_event_value_set_bool(struct aud_event *, const char *, bool);
extern struct aud_event *aud_event_new_first(void);
extern struct aud_data *aud_data_new(void);
extern struct aud_data *aud_data_new_zero(int size);
extern struct aud_data *aud_data_new_metadata(void *md);
extern void  aud_data_adapter_init(struct aud_adapter *);
extern void  aud_data_adapter_cleanup(struct aud_adapter *);
extern void  aud_data_adapter_push(struct aud_adapter *, struct aud_data *);
extern struct aud_data *aud_data_adapter_dequeue(struct aud_adapter *);
extern void *aud_data_adapter_map(struct aud_adapter *, int);
extern void  aud_data_adapter_unmap(struct aud_adapter *, void *);
extern void  aud_data_adapter_flush(struct aud_adapter *, int);
extern void  AuiLogger_Init(void *dst, int c, unsigned n);   /* memset */
extern void  AuiLogger_AppendData(void *dst, const void *src, unsigned n); /* memcpy */
extern int   aud_basemixer_layout_from_acmod(int);
extern int   aud_basemixer_init(void *);
extern void  aud_basemixer_cleanup(void *);
extern void  __assert_failed(const char *fn, int line, const char *expr);
extern void  list_add_tail_(void *head, void *node);
extern void  list_del_(void *node);
extern char *audutil_strdup_printf(const char *, ...);
extern void  aud_module_impl_add_prop(void *, const char *, int, unsigned, void *, void *, int);
extern void  aud_object_class_register(const void *);
extern void  aud_mregistry_add_class(const void *);

struct prop_desc { const char *n; int t; unsigned off; };
struct mixerbin_view { char m[0x118]; void *admix; void *pcmren; void *mux; void *mixer[4]; };

/* MS12 mixer library (ms_mixer_*, @0x600b8380..) */
extern int  ms_mixer_main_process_open(void *), ms_mixer_main_process_close(void *);
extern int  ms_mixer_main_process_getsize(void *);
extern int  ms_mixer_main_process_init(void *, void *cfg, int size);
extern int  ms_mixer_main_process_run(void *, void *io, int size);
extern int  ms_mixer_main_process_setparam(void *, unsigned id, void *);
extern int  ms_mixer_main_connect_msf(void *, int slot), ms_mixer_main_disconnect_msf(void *, int slot);
extern int  ms_mixer_get_mixingmode(void *), ms_mixer_set_mixingmode(void *, int);
extern int  ms_mixer_set_limiterenable(void *, int), ms_mixer_get_limiterenable(void *, int *);
extern int  ms_mixer_set_acmod(void *, int slot, int), ms_mixer_get_acmod(void *, int *, int *);
extern int  ms_mixer_set_lfeenable(void *, int slot, int), ms_mixer_get_lfeenable(void *, int *, int *);
extern int  ms_mixer_set_PortEnable(void *, int slot, int), ms_mixer_get_PortEnable(void *, int slot);
extern int  ms_mixer_set_SamplingRate(void *, int), ms_mixer_get_SamplingRate(void *, int *);
extern int  ms_mixer_set_Channel(void *, int slot, int), ms_mixer_get_Channel(void *, int *, int *);
extern int  ms_mixer_get_MixingBalance(void *, int *), ms_mixer_get_fadeinout(void *, int *, int *, int *);
extern int  ms_mixer_set_fade_primaryInput(void *, void *), ms_mixer_set_fade_secondaryInput(void *, void *);
extern int  ms_mixer_set_fade_systemInput(void *, void *);
extern unsigned ms_mixer_get_lib_version(void);
extern int  msf_update_read_buf(void *, int, const char *), msf_update_write_buf(void *, int);
extern int  msf_process_metadata(void *, void *buf, int size);
extern void msf_flush_metadata(void *), msf_print_metadata(void *);
extern void logm_message(const char *, ...);

#define MS_PARAM_PES_METADATA   0x0              /* DAT_600604cc [GAP] value not read */
#define MS_PARAM_MIXING_MD      0x0              /* DAT_60060490 [GAP] */
#define MS_FRAME_BYTES          0x2000           /* DAT_600603bc: zero output size */
#define MS_DEFAULT_RATE         48000            /* DAT_6005efc4 */

static const int acmod_from_channels[9] = { 2, 1, 2, 3, 4, 5, 6, 7, 21 }; /* [GAP] table @"msmixer_event_first"+0x1c not decoded */

static inline int msmixer_slot(const struct msmixer *mm, unsigned port)
{
    unsigned main = mm->main_audio_port;

    if (main > 1)
        main = (unsigned)mm->main_alt;
    if (port == main)
        return 0;
    return port == 2 ? 2 : 1;
}

/* @0x60093c88 msmixer_get_input */
struct mix_input *msmixer_get_input(struct msmixer *mm, int port)
{
    struct mix_input *in;

    for (in = mm->in_head; in; in = in->next)
        if (in->port == port)
            return in;
    in = calloc(sizeof(*in), 1);
    if (!in) {
        SLOG(0, "no mem\n");
        return NULL;
    }
    in->port = port;
    in->enabled = false;
    in->channels = 8;
    aud_data_adapter_init(&in->fifo);
    list_add_tail_(&mm->in_head, in);
    return in;
}

/* @0x60093d0c msmixer_do_mix. Slot-0 handling, the bypass-admix short cut, the
 * "aggregate without main" zero output, the library run and the post-run flush
 * are observed. Slots 1/2 are prepared by the same code as slot 0 inside FLIX
 * loops (Ghidra split them into FUN_60093ebe / FUN_60094105 / FUN_60094162,
 * which are parts of this function, not separate functions). [FLIX] */
struct aud_data *msmixer_do_mix(struct msmixer *mm, uint32_t *layout)
{
    void *lib = mm->lib;
    int size = ms_mixer_main_process_getsize(lib);
    struct mix_input *slot[3];
    void *buf[3];
    struct aud_data *out;
    int ret;

    if (mm->mode_dirty) {
        ms_mixer_get_mixingmode(lib);                  /* [FLIX] -> ms_mixer_set_mixingmode */
        mm->mode_dirty = false;
    }
    if (!mm->lib_ready) {
        int cfg[3] = {0, 0, 0};
        if (ms_mixer_main_process_init(lib, cfg, size) != 4)
            SLOG(0, "init failed. %d\n", 0);           /* [FLIX] */
        mm->lib_ready = true;
    }

    for (int p = 0; p < 3; p++) {
        struct mix_input *in = msmixer_get_input(mm, p);
        int s = msmixer_slot(mm, (unsigned)p);

        slot[p] = in;
        buf[p] = NULL;
        if (!in->enabled)
            continue;
        if (!in->low_delay) {
            if (in->zero_mixing) {
                SLOG(in->zero_logs < 64 ? 2 : 3, "port%d, low delay, mix normal input..\n", p);
                in->zero_mixing = false;
            }
            buf[p] = aud_data_adapter_map(&in->fifo, mm->frames_per_slot[s]);
        } else {
            if (!in->zero_mixing) {
                if (in->zero_logs < 64) {
                    SLOG(2, "port%d, low delay, mix zero input.. %d\n", p, in->zero_logs);
                    if (++in->zero_logs == 64)
                        SLOG(2, "port%d, no more print..\n", p);
                }
                in->zero_mixing = true;
            }
            mm->underflow_count++;
        }
    }

    if (mm->bypass_admix)                              /* FIRST "bypass-admix" */
        return aud_data_new_zero(MS_FRAME_BYTES);

    SLOG(4, "mix : %p %d, %p %d, %p %d\n", buf[0], mm->frames_per_slot[0], buf[1],
         mm->frames_per_slot[1], buf[2], mm->frames_per_slot[2]);
    if (mm->main_audio_port > 1 && !mm->downmix) {
        /* no main input selected: flush what is mapped and emit silence */
        for (int p = 0; p < 3; p++)
            if (slot[p]->enabled) {
                int s = msmixer_slot(mm, (unsigned)p);
                aud_data_adapter_unmap(&slot[p]->fifo, buf[p]);
                aud_data_adapter_flush(&slot[p]->fifo, mm->frames_per_slot[s]);
            }
        return aud_data_new_zero(MS_FRAME_BYTES);
    }

    {
        void *io[3] = { buf[0], buf[1], buf[2] };
        ret = ms_mixer_main_process_run(lib, io, size);
    }
    for (int p = 0; p < 3; p++) {
        struct mix_input *in = slot[p];
        int s = msmixer_slot(mm, (unsigned)p);

        if (!in->enabled)
            continue;
        if (!in->zero_mixing) {
            aud_data_adapter_unmap(&in->fifo, buf[p]);
            aud_data_adapter_flush(&in->fifo, mm->frames_per_slot[s]);
        }
        SLOG(4, "port%d, remains %d samples\n", in->port, ADAPTER_AVAIL(&in->fifo) / in->frame_bytes);
    }
    SLOG(4, "dec result %d\n", ret);

    /* tail (FUN_6009409e): copy the library output into a new aud_data, layout
     * from the main slot's acmod */
    out = aud_data_new();
    if (!out) {
        SLOG(0, "no mem\n");
        return NULL;
    }
    {
        int acmod = 2;
        for (int p = 0; p < 3; p++) {
            struct mix_input *in = msmixer_get_input(mm, p);
            SLOG(4, "in%d enable : %d, acmod : %d\n", p, in->enabled, in->acmod);
            if (p == mm->main_audio_port)
                acmod = in->acmod;
        }
        *layout = (uint32_t)aud_basemixer_layout_from_acmod(acmod) | 0x80000000u;
    }
    return out;
}

/* @0x60094400 msmixer_can_mix: only ports 0..2; the threshold test against
 * the per-port frame table @0x60006110 {0x2000,0x2000,0x400} is a FLIX bundle */
int msmixer_can_mix(struct msmixer *mm, struct mix_input *in)
{
    static const int need[3] = { 0x2000, 0x2000, 0x400 };

    (void)mm;
    if ((unsigned)in->port < 3)
        return ADAPTER_AVAIL(&in->fifo) >= need[in->port];   /* [FLIX] comparison */
    SLOG(0, "wrong port. %d\n", in->port);
    return 0;
}

/* @0x60094448 msmixer_get_first_event: output FIRST (acmod of the main slot) */
struct aud_event *msmixer_get_first_event(struct msmixer *mm, struct aud_event *e)
{
    int acmod = 2;

    for (int p = 0; p < 3; p++) {
        struct mix_input *in = msmixer_get_input(mm, p);
        SLOG(3, "in%d enable : %d, acmod : %d\n", p, in->enabled, in->acmod);
        if (p == mm->main_audio_port)
            acmod = in->acmod;
    }
    SLOG(3, "main_audio_port %d acmod_out %d downmix %d\n", mm->main_audio_port, acmod, mm->downmix);
    if (!e && !(e = aud_event_new_first()))
        return NULL;
    aud_event_value_set_int(e, "acmod", acmod);
    aud_event_value_set_bool(e, "downmix", mm->downmix);
    aud_event_value_set_bool(e, "ch-mapping-lfe-c", mm->ch_mapping_lfe_c);
    return e;
}

/* @0x600944f0 msmixer_input */
struct mix_input *msmixer_input(struct msmixer *mm, unsigned port, struct aud_data *d)
{
    int s = msmixer_slot(mm, port);
    struct mix_input *in = msmixer_get_input(mm, (int)port);
    int frames;

    if (!in)
        return NULL;
    frames = d->size / in->frame_bytes;
    if (in->drop_au) {
        msf_update_read_buf(&mm->msf[s], frames, "msmixer_input");
        in->drop_au--;
        return NULL;
    }
    while (in->insert_zero) {
        SLOG(3, "in%d: insert initial zero. %d, sample %d\n", in->port, in->insert_zero, frames);
        msf_update_write_buf(&mm->msf[s], frames);
        aud_data_adapter_push(&in->fifo, aud_data_new_zero(d->size));
        in->insert_zero--;
    }
    aud_data_adapter_push(&in->fifo, d);
    SLOG(4, "in%d: push %d samples. pending %d samples\n", in->port, frames,
         ADAPTER_AVAIL(&in->fifo) / in->frame_bytes);
    return in;
}

/* @0x60094614 msmixer_input_metadata: the only data-type test in msmixer */
int msmixer_input_metadata(struct msmixer *mm, unsigned port, struct aud_data *d)
{
    if (d->type != 2) {
        SLOG(3, "not metadata. %d\n", d->type);
        return -1;
    }
    if (mm->bypass_admix) {
        aud_data_adapter_push(&mm->md_fifo, aud_object_get(d));
        return 0;
    }
    if (!mm->downmix && msf_process_metadata(&mm->msf[msmixer_slot(mm, port)], d->buf, d->size))
        SLOG(0, "msf_process_metadata() Err, port: %d\n", port);
    return 0;
}

/* @0x600946d0 msmixer_output_metadata */
struct aud_data *msmixer_output_metadata(struct msmixer *mm)
{
    struct aud_data *md;

    if (mm->bypass_admix)
        return ADAPTER_AVAIL(&mm->md_fifo) ? aud_data_adapter_dequeue(&mm->md_fifo) : NULL;
    md = aud_data_new_metadata(mm->md_out);
    if (md)
        SLOG(4, "push metadata - size=%d\n", md->size);
    return md;
}

/* @0x6009472c msmixer_get_nullgen_port */
int msmixer_get_nullgen_port(struct msmixer *mm)
{
    (void)mm;
    return -1;
}

/* @0x60094734 msmixer_mixing_mode_to_string */
char *msmixer_mixing_mode_to_string(int mode)
{
    switch (mode) {
    case 0: return strdup("stringent");
    case 1: return strdup("lenient");
    case 2: return strdup("aggregate");
    case 3: return strdup("stringent_dualmain");
    default: return NULL;
    }
}

int msmixer_toggle_first_event_info(struct msmixer *mm, unsigned bit);

/* @0x6009477c msmixer_event_first: configure the library slot from FIRST.
 * Keys read: acmod, downmix, ad-mixing, lfe-enable, rate, bypass-admix,
 * ch-mapping-lfe-c. No "es". */
int msmixer_event_first(struct msmixer *mm, struct mix_input *in, struct aud_event *e)
{
    void *lib = mm->lib;
    int s = msmixer_slot(mm, (unsigned)in->port);
    int acmod = aud_event_value_get_int(e, "acmod", -1);
    bool downmix, lfe;

    if (acmod < 0) {
        acmod = (unsigned)in->channels < 9 ? acmod_from_channels[in->channels] : 2;
        SLOG(3, "port%d, no acmod, select from ch %d\n", in->port, in->channels);
    }
    downmix = aud_event_value_get_bool(e, "downmix", false);
    ms_mixer_get_mixingmode(lib);
    (void)aud_event_value_get_bool(e, "ad-mixing", false); /* [FLIX] picks mixing mode 0/2/3 */
    lfe = aud_event_value_get_bool(e, "lfe-enable", false);
    if (!mm->lib_ready) {
        int cfg[3] = {0, 0, 0};
        int r;

        SLOG(3, "port%d, init library..\n", in->port);
        ms_mixer_set_SamplingRate(lib, MS_DEFAULT_RATE);
        r = ms_mixer_main_process_init(lib, cfg, ms_mixer_main_process_getsize(lib));
        if (r != 4)
            SLOG(0, "init failed. %d\n", r);
        mm->lib_ready = true;
    }
    ms_mixer_set_SamplingRate(lib, aud_event_value_get_int(e, "rate", MS_DEFAULT_RATE));
    ms_mixer_set_Channel(lib, s, in->channels);
    ms_mixer_set_PortEnable(lib, s, 1);
    ms_mixer_main_connect_msf(lib, s);
    if (lfe || acmod > 2)
        ms_mixer_set_lfeenable(lib, s, 1);
    ms_mixer_set_acmod(lib, s, acmod);
    mm->bypass_admix = aud_event_value_get_bool(e, "bypass-admix", false);
    in->zero_logs = 0;
    in->acmod = acmod;
    if (mm->downmix != downmix) {
        mm->downmix = downmix;
        msmixer_toggle_first_event_info(mm, 2);
    }
    mm->ch_mapping_lfe_c = aud_event_value_get_bool(e, "ch-mapping-lfe-c", false);
    SLOG(3, "msmixer port : %d, acmod : %d\n", s, acmod);
    return 0;
}

/* @0x60094aa4 msmixer_event_last: disable the slot; when no input is left,
 * reset all slots */
int msmixer_event_last(struct msmixer *mm, struct mix_input *in)
{
    void *lib = mm->lib;
    int s = msmixer_slot(mm, (unsigned)in->port);
    int count = 0, enabled = 0;

    ms_mixer_set_PortEnable(lib, s, 0);
    ms_mixer_set_acmod(lib, s, 2);
    msf_flush_metadata(&mm->msf[s]);
    for (struct mix_input *x = mm->in_head; x; x = x->next) {
        count++;
        if (x->enabled)
            enabled++;
    }
    SLOG(3, "count : %d, enable : %d\n", count, enabled);
    if (enabled)
        return 0;
    for (int i = 0; i < 3; i++) {
        msf_flush_metadata(&mm->msf[i]);
        ms_mixer_set_acmod(lib, i, 2);
        ms_mixer_set_lfeenable(lib, i, 0);
    }
    AuiLogger_Init((void *)(intptr_t)mm->_218, 0, (unsigned)mm->lib_work_size);
    return 0;
}

/* @0x60094c24 msmixer_event_mixing_metadata: custom "setting-mixing-metadata" */
int msmixer_event_mixing_metadata(struct msmixer *mm, struct aud_event *e)
{
    int p[5];
    int r;

    p[0] = aud_event_value_get_int(e, "fade", 0);      /* key names @0x60060478/7c [GAP] */
    p[4] = aud_event_value_get_int(e, "pan", 0);
    p[1] = aud_event_value_get_int(e, "center", 0);
    p[2] = aud_event_value_get_int(e, "front", 0);
    p[3] = aud_event_value_get_int(e, "surround", 0);
    SLOG(3, "set mixing metadata. fade: %d, pan: %d, gain_center: %d, gain_front: %d, gain_surround: %d\n",
         p[0], p[4], p[1], p[2], p[3]);
    r = ms_mixer_main_process_setparam(mm->lib, MS_PARAM_MIXING_MD, p);
    if (r == 0) {
        SLOG(2, "success set pes metadata\n");
        return 0;
    }
    SLOG(0, "fail set pes metadata\n");
    return r;
}

/* @0x60094cf4 msmixer_toggle_first_event_info */
int msmixer_toggle_first_event_info(struct msmixer *mm, unsigned bit)
{
    mm->first_info ^= 1u << (bit & 31);
    return 0;
}

/* @0x60094d0c msmixer_clear_first_event_info */
int msmixer_clear_first_event_info(struct msmixer *mm, unsigned bit)
{
    mm->first_info &= ~(1u << (bit & 31));
    return 0;
}

/* @0x60094d2c msmixer_get_first_event_info */
unsigned msmixer_get_first_event_info(struct msmixer *mm)
{
    return mm->first_info;
}

/*
 * Command handlers. Their names in functions.tsv/dsp2.syms.tsv were assigned
 * from the neighbouring name string, which is one row off: the table @0x60006370
 * {name, cmd, fn} maps them as tagged below (C names follow the log __func__).
 */

/* @0x60094d34 ADEC_CMD_INIT handler ("MSMIX_CMD_SET_CONFIG" in syms) */
int _MsMixerCmd_Init(struct msmixer *mm, void *param)
{
    (void)param;
    mm->_244 = 0;
    AuiLogger_Init((void *)(intptr_t)mm->_218, 0, (unsigned)mm->lib_work_size);
    return 0;
}

/* @0x60094d4c MSMIX_CMD_SET_LFE_ENABLE 0x61110008 ("MSMIX_CMD_SET_FADEINOUT" in syms) */
int _MsMixerCmd_SetLfeEable(struct msmixer *mm, const int *p)
{
    int r = ms_mixer_set_lfeenable(mm->lib, p[0], p[1]);

    if (r == 0) {
        SLOG(3, "success set lfe : port[%d], lfe_enable[%d]\n", p[0], p[1]);
        return 0;
    }
    SLOG(0, "fail set lfe : port[%d], lfe_enable[%d], error[0x%x]\n", p[0], p[1], r);
    return r;
}

/* @0x60094dac MSMIX_CMD_SET_FADEINOUT 0x61050010 ("MSMIX_CMD_SET_DDP_METADATA" in syms) */
int _MsMixerCmd_SetFadeInOut(struct msmixer *mm, const int *p)
{
    extern int msmixer_max_fade_ms;                    /* DAT_600604b8 */

    if ((unsigned)p[0] >= 3) {
        SLOG(0, "port: %d\n", p[0]);
        return 2;
    }
    if (p[1] < -96 || p[1] > 0) {
        SLOG(0, "target_gain: %d\n", p[1]);
        return 2;
    }
    if (p[2] < 0 || p[2] > msmixer_max_fade_ms) {
        SLOG(0, "Invalid parameter : duration: %d\n", p[2]);
        return 2;
    }
    if ((unsigned)p[3] >= 3) {
        SLOG(0, "Invalid parameter : mix_type: %d\n", p[3]);
        return 2;
    }
    SLOG(3, "port: %d gain: %d duration: %d mix type: %d\n", p[0], p[1], p[2], p[3]);
    if (mm->fade_ok != 1)
        return 0;
    if (ms_mixer_get_mixingmode(mm->lib) == 0) {
        SLOG(0, "Fading is Not Supported for Stringent (AD type)\n");
        return 0xf;
    }
    if (p[0] == 0)
        ms_mixer_set_fade_primaryInput(mm->lib, (void *)(p + 1));
    else if (p[0] == 1)
        ms_mixer_set_fade_secondaryInput(mm->lib, (void *)(p + 1));
    else
        ms_mixer_set_fade_systemInput(mm->lib, (void *)(p + 1));
    return 0;
}

/* @0x60094ed8 MSMIX_CMD_SET_DDP_METADATA 0x6107002c ("MSMIX_CMD_SET_PES_METADATA" in syms) */
int _MsMixerCmd_SetDdpMetadata(struct msmixer *mm, void *p)
{
    ms_mixer_main_process_setparam(mm->lib, MS_PARAM_PES_METADATA, p);
    return 0;
}

/* @0x60094ef0 MSMIX_CMD_SET_PES_METADATA 0x61080014 ("MSMIX_CMD_SET_MIXING_TYPE" in syms) */
int _MsMixerCmd_SetPesMetadata(struct msmixer *mm, const int *p)
{
    int r = ms_mixer_main_process_setparam(mm->lib, MS_PARAM_MIXING_MD, (void *)p);

    if (r == 0) {
        SLOG(3, "success set pes metadata : fade_byte[%d], gain{center[%d], front[%d], surr[%d]}, pan[%d] \n",
             p[0], p[1], p[2], p[3], p[4]);
        return 0;
    }
    SLOG(0, "fail set pes metadata : fade_byte[%d], gain{center[%d], front[%d], surr[%d]}, pan[%d] error[0x%x] \n",
         p[0], p[1], p[2], p[3], p[4], r);
    return r;
}

/* @0x60094f68 MSMIX_CMD_SET_MIXING_TYPE 0x61090004 ("MSMIX_CMD_SET_MIXING_MODE" in syms) */
int _MsMixerCmd_SetMixingType(struct msmixer *mm, const int *p)
{
    SLOG(3, "mixing type        : %d\n", p[0]);
    mm->mixing_type = p[0];
    return 0;
}

/* @0x60094f9c _MsMixerCmd_Reopen: close/open the library keeping every setting */
int _MsMixerCmd_Reopen(struct msmixer *mm)
{
    void *lib = mm->lib;
    int en[3], ch[3], ch_out, rate, lfe[3], lfe_out, acmod[3], acmod_out, lim, mode;

    for (int i = 0; i < 3; i++)
        en[i] = ms_mixer_get_PortEnable(lib, i);
    ms_mixer_get_Channel(lib, ch, &ch_out);
    ms_mixer_get_SamplingRate(lib, &rate);
    ms_mixer_get_lfeenable(lib, lfe, &lfe_out);
    ms_mixer_get_acmod(lib, acmod, &acmod_out);
    ms_mixer_get_limiterenable(lib, &lim);
    mode = ms_mixer_get_mixingmode(lib);
    for (int i = 0; i < 3; i++)
        SLOG(4, "port enable %d : %d\n", i, en[i]);
    for (int i = 0; i < 3; i++)
        msf_flush_metadata(&mm->msf[i]);
    if (ms_mixer_main_process_close(lib) != 0)
        __assert_failed("_MsMixerCmd_Reopen", 0x431, "result == 0");
    if (ms_mixer_main_process_open(lib) != 0)
        __assert_failed("_MsMixerCmd_Reopen", 0x434, "result == 0");
    for (int i = 0; i < 3; i++) {
        ms_mixer_set_PortEnable(lib, i, en[i]);
        ms_mixer_set_Channel(lib, i, ch[i]);
        ms_mixer_set_lfeenable(lib, i, lfe[i]);
        ms_mixer_set_acmod(lib, i, acmod[i]);
    }
    ms_mixer_set_SamplingRate(lib, rate);
    ms_mixer_set_limiterenable(lib, lim);
    ms_mixer_set_mixingmode(lib, mode);
    mm->_2c0[0] = mm->_2c0[1] = mm->_2c0[2] = 0;
    AuiLogger_Init((void *)(intptr_t)mm->_218, 0, (unsigned)mm->lib_work_size);
    return 0;
}

/* @0x6009536c MSMIX_CMD_SET_MIXING_MODE 0x61100004 ("MSMIX_CMD_SET_LIMITER_ENABLE" in syms) */
int _MsMixerCmd_SetMixingMode(struct msmixer *mm, const int *p)
{
    int r;

    if (p[0] == ms_mixer_get_mixingmode(mm->lib))
        return 0;
    _MsMixerCmd_Reopen(mm);
    r = ms_mixer_set_mixingmode(mm->lib, p[0]);
    if (r == 0) {
        SLOG(3, "success set mixingmode [%d]\n", p[0]);
        mm->mixing_mode_int = p[0];
        mm->lib_ready = false;
        return 0;
    }
    SLOG(0, "fail set mixingmode[%d], error[0x%x] \n", p[0], r);
    return r;
}

/* @0x600953f4 MSMIX_CMD_SET_LIMITER_ENABLE 0x61120004 ("ADEC_CMD_GET_DECINFO" in syms) */
int _MsMixerCmd_SetlimiterEable(struct msmixer *mm, const int *p)
{
    int cur = 0, r;

    ms_mixer_get_limiterenable(mm->lib, &cur);
    if (p[0] == cur)
        return 0;
    _MsMixerCmd_Reopen(mm);
    r = ms_mixer_set_limiterenable(mm->lib, p[0]);
    if (r == 0) {
        SLOG(3, "success set limiter : enable %d\n", p[0]);
        mm->enable_limiter = p[0] != 0;
        mm->lib_ready = false;
        return 0;
    }
    SLOG(0, "fail set limiter : enable %d, error 0x%x\n", p[0], r);
    return r;
}

/* @0x60095494 ADEC_CMD_CONNECT_METADATA 0x10150000 ("ADEC_CMD_SET_DEBUGCNT" in syms) */
int _MsMixerCmd_ConnectMetadata(struct msmixer *mm)
{
    for (int i = 0; i < 3; i++)
        if (ms_mixer_main_disconnect_msf(mm->lib, i) != 0)
            __assert_failed("_MsMixerCmd_ConnectMetadata", 0x4a2, "result == 0");
    for (int i = 0; i < 3; i++)
        if (ms_mixer_get_PortEnable(mm->lib, i) == 1 && ms_mixer_main_connect_msf(mm->lib, i) != 0)
            __assert_failed("_MsMixerCmd_ConnectMetadata", 0x4aa, "result == 0");
    return 0;
}

/* @0x600954f8 ADEC_CMD_SET_DEBUGCNT 0x10060004 ("ADEC_CMD_MODULE_STATUS" in syms) */
int _MsMixerCmd_DebugPrintCount(struct msmixer *mm)
{
    (void)mm;
    SLOG(0, "FIXME!\n");
    return 0;
}

/* @0x60095520 ADEC_CMD_MODULE_STATUS 0x10070000 ("ADEC_CMD_SHOW_METADATA" in syms) */
int _MsMixerCmd_ModuleStatus(struct msmixer *mm)
{
    void *lib = mm->lib;
    int rate, lim, bal, acmod[3], acmod_out, fade[3][3], lfe[3], lfe_out, ch[3], ch_out, en[3];
    struct mix_input *in[3];
    int mode;

    ms_mixer_get_SamplingRate(lib, &rate);
    mode = ms_mixer_get_mixingmode(lib);
    ms_mixer_get_limiterenable(lib, &lim);
    ms_mixer_get_MixingBalance(lib, &bal);
    ms_mixer_get_acmod(lib, acmod, &acmod_out);
    ms_mixer_get_fadeinout(lib, fade[0], fade[1], fade[2]);
    ms_mixer_get_lfeenable(lib, lfe, &lfe_out);
    ms_mixer_get_Channel(lib, ch, &ch_out);
    for (int i = 0; i < 3; i++) {
        en[i] = ms_mixer_get_PortEnable(lib, i);
        in[i] = msmixer_get_input(mm, i);
    }
    logm_message("==================================\n");
    logm_message("= MSMIXER Library Version : 0x%x\n", ms_mixer_get_lib_version());
    logm_message("= Sampling Frequency : %d\n", rate);
    logm_message("= Mixing Mode (0:AD, 1:SYS, 2:DM) : %d\n", mode);
    logm_message("= Mixing Type (0:M+S, 1:S Only) : %d\n", mm->mixing_type);
    logm_message("= Limiter Enabled : %d\n", lim);
    logm_message("= Mixing Balance : %d\n", bal);
    for (int i = 0; i < 3; i++) {
        logm_message("= IN PORT %d Params\n", i);
        logm_message("= Port Enable : %d\n", en[i]);
        logm_message("= Number of channel : %d\n", ch[i]);
        logm_message("= Acmod (Working) : %d\n", acmod[i]);
        logm_message("= lfe enable : %d\n", lfe[i]);
        logm_message("= Fade InOut (Gain/Duration/Type) : %d/%d/%d\n", fade[i][0], fade[i][1], fade[i][2]);
        logm_message("= Input Enable / Size : %d / %d\n", in[i]->enabled, ADAPTER_AVAIL(&in[i]->fifo));
    }
    logm_message("= OUPUT Params\n");
    logm_message("= Number of channel : %d\n", ch_out);
    logm_message("= Acmod : %d\n", acmod_out);
    logm_message("= lfe enable : %d\n", lfe_out);
    return 0;
}

/* @0x60095718 ADEC_CMD_SHOW_METADATA 0x10140000 ("MSMIX_CMD_SET_GEN_MUTE_SIGNAL" in syms) */
int _MsMixerCmd_ShowMetadata(struct msmixer *mm)
{
    for (int i = 0; i < 3; i++)
        msf_print_metadata(&mm->msf[i]);
    return 0;
}

/* @0x6009573c _MsMixerCmd_NotSupported: SET_CONFIG/ENABLE/VOLUME/ACMOD,
 * GET_DECINFO, SET_GEN_MUTE_SIGNAL all land here ("MSMIX_CMD_SET_ENABLE" in syms) */
int _MsMixerCmd_NotSupported(struct msmixer *mm)
{
    (void)mm;
    SLOG(2, "not supported\n");
    return 0;
}

struct msmix_cmd { const char *name; unsigned cmd; int (*fn)(struct msmixer *, void *); };
extern const struct msmix_cmd msmixer_cmd_table[];     /* @0x60006370, NULL-terminated */

/* @0x60095764 msmixer_command: linear search of the table; the loop compare
 * is FLIX, the first entry test and the dispatch/dump are observed.
 * Unknown command -> -1 (mixer_command then tries aud_module_default_command). */
int msmixer_command(struct msmixer *mm, unsigned cmd, void *param, unsigned len)
{
    for (const struct msmix_cmd *c = msmixer_cmd_table; c->name; c++) {
        if (c->cmd != cmd)
            continue;
        /* optional hex dump of the argument words ("dump_arg", log level 5) */
        (void)len;
        c->fn(mm, param);
        return 0;
    }
    return -1;
}

/* @0x60095878 msmixer_prop_updated: "mixing-mode" string <-> "mixing-mode-int" */
int msmixer_prop_updated(struct msmixer *mm, const struct prop_desc *prop)
{
    if (prop->off == 0x150) {
        const char *s = mm->mixing_mode;

        if (!s)
            return 0;
        mm->mode_dirty = true;
        if (!strcmp(s, "lenient") || !strcmp(s, "sysmix"))
            mm->mixing_mode_int = 1;
        else if (!strcmp(s, "aggregate"))
            mm->mixing_mode_int = 2;
        else
            mm->mixing_mode_int = !strcmp(s, "dual-main") ? 3 : 0;   /* unknown -> stringent */
        return 0;
    }
    if (prop->off == 0x154) {
        mm->mixing_mode = msmixer_mixing_mode_to_string(mm->mixing_mode_int);
        mm->mode_dirty = true;
    }
    /* [FLIX] four more offset tests follow (fading-* / enable-limiter) */
    return 0;
}

/* @0x60095aa8 msmixer_set_main_audio_port: "main-audio-port" setter; re-maps
 * all three library slots from the stored FIRST events */
int msmixer_set_main_audio_port(struct msmixer *mm, const void *prop, const char *val)
{
    void *lib = mm->lib;
    int main = (int)_Stoul(val, NULL, 0);

    (void)prop;
    SLOG(3, "set main audio port\n");
    for (int i = 0; i < 3; i++)
        msf_flush_metadata(&mm->msf[i]);
    for (int p = 0; p < 3; p++) {
        struct mix_input *in = msmixer_get_input(mm, p);
        int s = p == main ? 0 : p == 2 ? 2 : 1;

        if (!in->enabled) {
            SLOG(3, "in%d : disabled\n", p);
            ms_mixer_set_PortEnable(lib, s, 0);
            continue;
        }
        SLOG(3, "in%d : enabled\n", p);
        ms_mixer_set_PortEnable(lib, s, 1);
        ms_mixer_main_connect_msf(lib, s);
        {
            int acmod = aud_event_value_get_int(in->first, "acmod", -1);
            if (acmod < 0)
                acmod = (unsigned)in->channels < 9 ? acmod_from_channels[in->channels] : 2;
            ms_mixer_set_acmod(lib, s, acmod);
        }
        ms_mixer_set_Channel(lib, s, in->channels);
        if (aud_event_value_get_bool(in->first, "lfe-enable", false))
            ms_mixer_set_lfeenable(lib, s, 1);
        mm->bypass_admix = aud_event_value_get_bool(in->first, "bypass-admix", false);
    }
    AuiLogger_Init((void *)(intptr_t)mm->_218, 0, (unsigned)mm->lib_work_size);
    mm->main_audio_port = main;
    return 0;
}

/* @0x60095cb0 msmixer_constructor */
int msmixer_constructor(struct msmixer *mm)
{
    void *lib = mm->lib;
    int r;

    mm->in_head = mm->in_tail = NULL;
    r = ms_mixer_main_process_open(lib);
    if (r)
        SLOG(0, "open() failed. %d\n", r);
    for (int i = 0; i < 3; i++)
        ms_mixer_set_lfeenable(lib, i, 0);
    mm->mixing_mode_int = 3;
    ms_mixer_set_mixingmode(lib, mm->mixing_mode_int);
    mm->mixing_mode = msmixer_mixing_mode_to_string(mm->mixing_mode_int);
    SLOG(3, "implement name %s. mixing_mode %s\n", "msmixer", mm->mixing_mode);
    aud_data_adapter_init(&mm->md_fifo);
    if (aud_basemixer_init(mm) < 0) {
        SLOG(0, "Oops\n");
        return -1;
    }
    mm->ext_pcm_acmod = 0;
    mm->ch_mapping_lfe_c = false;
    mm->downmix = false;
    mm->first_info = 0;
    return 0;
}

/* @0x60095d8c msmixer_destructor */
void msmixer_destructor(struct msmixer *mm)
{
    struct mix_input *in;

    for (int i = 0; i < 3; i++)
        msf_flush_metadata(&mm->msf[i]);
    aud_basemixer_cleanup(mm);
    aud_data_adapter_cleanup(&mm->md_fifo);
    ms_mixer_main_process_close(mm->lib);
    while ((in = mm->in_head)) {
        list_del_(in);
        aud_data_adapter_cleanup(&in->fifo);
        if (in->first)
            aud_object_put(in->first);
        aud_free(in);
    }
}

/* @0x60095df4 msmixer_gets_lib_buf_info */
char *msmixer_gets_lib_buf_info(struct msmixer *mm)
{
    return audutil_strdup_printf("buffered:%i consumed:%i", mm->buffered, mm->consumed);
}

extern const char msmixer_class[], admix_class[], sysmix_class[]; /* 0x60060610/14/18 */

/* @0x60095e0c __aud_object_register_msmixer */
void __aud_object_register_msmixer(void)
{
    aud_object_class_register(msmixer_class);
}

/* @0x60095e1c __aud_object_register_admix */
void __aud_object_register_admix(void)
{
    aud_object_class_register(admix_class);
}

/* @0x60095e2c __aud_object_register_sysmix */
void __aud_object_register_sysmix(void)
{
    aud_object_class_register(sysmix_class);
}

static void msmixer_fill_impl(void **impl)
{
    impl[0x60 / 4] = (void *)msmixer_get_input;
    impl[0x64 / 4] = (void *)msmixer_do_mix;
    impl[0x68 / 4] = (void *)msmixer_can_mix;
    impl[0x6c / 4] = (void *)msmixer_get_first_event;
    impl[0x70 / 4] = (void *)msmixer_input;
    impl[0x74 / 4] = (void *)msmixer_input_metadata;
    impl[0x78 / 4] = (void *)msmixer_output_metadata;
    impl[0x7c / 4] = (void *)msmixer_event_first;
    impl[0x80 / 4] = (void *)msmixer_event_last;
    impl[0x84 / 4] = (void *)msmixer_event_mixing_metadata;
    impl[0x88 / 4] = (void *)msmixer_command;
    impl[0x8c / 4] = (void *)msmixer_get_nullgen_port;
    impl[0x98 / 4] = (void *)msmixer_toggle_first_event_info;
    impl[0x9c / 4] = (void *)msmixer_clear_first_event_info;
    impl[0xa0 / 4] = (void *)msmixer_get_first_event_info;
    impl[0x1c / 4] = (void *)msmixer_prop_updated;
    impl[0x08 / 4] = (void *)msmixer_constructor;
    impl[0x0c / 4] = (void *)msmixer_destructor;
    impl[0x2c / 4] = (void *)(uintptr_t)0x2f8;
    impl[0x04 / 4] = ((void **)impl[0])[0x18 / 4];
    aud_module_impl_add_prop(impl, "mixing-mode", 4, 0x150, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "mixing-mode-int", 2, 0x154, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "enable-limiter", 1, 0x16c, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "underflow-count", 2, 0x15c, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "main-audio-port", 2, 0x170, (void *)msmixer_set_main_audio_port, NULL, 0);
    aud_module_impl_add_prop(impl, "ext-pcm-acmod", 2, 0x2ec, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "lib-buf-info", 4, 0, NULL, (void *)msmixer_gets_lib_buf_info, 1);
    aud_module_impl_add_prop(impl, "fading-primary", 4, 0x160, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "fading-secondary", 4, 0x164, NULL, NULL, 0);
    aud_module_impl_add_prop(impl, "fading-system", 4, 0x168, NULL, NULL, 0);
}

/* @0x60095e3c aud_object_class_init_msmixer */
int aud_object_class_init_msmixer(void **impl)
{
    msmixer_fill_impl(impl);
    return 0;
}

/* @0x60095f90 aud_object_class_init_admix: identical body (separate copy) */
int aud_object_class_init_admix(void **impl)
{
    msmixer_fill_impl(impl);
    return 0;
}

/* @0x600960e8 aud_object_class_init_sysmix: identical body (separate copy) */
int aud_object_class_init_sysmix(void **impl)
{
    msmixer_fill_impl(impl);
    return 0;
}

/* @0x60096240 __aud_mreg_class_msmixer */
void __aud_mreg_class_msmixer(void)
{
    aud_mregistry_add_class(msmixer_class);
}

/* @0x60096250 __aud_mreg_class_admix */
void __aud_mreg_class_admix(void)
{
    aud_mregistry_add_class(admix_class);
}

/* @0x60096260 __aud_mreg_class_sysmix */
void __aud_mreg_class_sysmix(void)
{
    aud_mregistry_add_class(sysmix_class);
}

/* @0x60098108 msmixer_get_prop: mixerbin property forwarded to admix (+0x118) */
char *msmixer_get_prop(struct mixerbin_view *mb, const char *const *prop)
{
    extern int aud_module_get_prop(void *, const char *);
    int v;

    if (!mb->admix || !prop[0])
        return NULL;
    v = aud_module_get_prop(mb->admix, prop[0]);
    SLOG(3, "%s get %s : %d\n", "mixerbin", prop[0], v);
    return audutil_strdup_printf("%d", v);
}

/* @0x60098158 msmixer_set_prop: mixerbin "<name>=<val>" forwarded verbatim to
 * admix; "main-audio-port" is additionally sent to all four LG mixers */
int msmixer_set_prop(struct mixerbin_view *mb, const char *const *prop, const char *val)
{
    extern int aud_module_set_prop(void *, const char *, unsigned);
    extern size_t strlen(const char *);
    char *s;

    SLOG(3, "%s: set %s : %s\n", "mixerbin", prop[0], val);
    if (mb->admix && (s = audutil_strdup_printf("%s=%s", prop[0], val))) {
        aud_module_set_prop(mb->admix, s, (unsigned)strlen(s));
        aud_free(s);
    }
    if (!strcmp(prop[0], "main-audio-port"))
        for (int i = 0; i < 4; i++)
            if (mb->mixer[i] && (s = audutil_strdup_printf("%s=%s", prop[0], val))) {
                aud_module_set_prop(mb->mixer[i], s, (unsigned)strlen(s));
                aud_free(s);
            }
    return 0;
}
