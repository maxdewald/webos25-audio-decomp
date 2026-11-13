/*
 * dsp2 generic module core, the part not already in renderer_path.c: the locked
 * chain wrapper (block-module wait), event push, sync, the property setters
 * (where "drop-output-data" is or isn't found), default command/event handlers.
 * aud_module_push_data (@0x600724d8) and aud_module_chain_locked (@0x600746d4)
 * are tagged in renderer_path.c and are only described here. 0x600746d4 is the
 * variant used by foreach_down_peer_push_data (normal module-to-module push);
 * it has the same block-module wait (+0x96, line 0x599) as aud_module_chain below.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Linked at 0x60000000. Matches the structure of the kernel-side module.c (names only).
 *
 * FLIX: each op0=0xE/0xF bundle is 8 bytes. For branch-shaped bundles the target is
 *   A + 4 + sext18((((w1 >> 8) & 0x3fff) << 4) | (w0 >> 28))   (w0/w1 little-endian)
 * (checked on all 263 bundles in lipsync/delay/core: every target is an instruction
 * start; tools/flix_annot.py). tools/xtobjdump.sh now decodes the branch condition
 * (validated, build/scratch/flixdec/README.md); [FLIX?] marks below predate it and
 * were inferred from the target block. Bundles whose target is a
 * logmm_printl call right after a log-mask load are log guards and are dropped.
 *
 * Log levels: logmm_printl(level n) is gated by bit n of a per-source GLOBAL logm
 * word (core 0x4ff9004c, lipsync 0x4ff900e8, delay 0x4ff900b4), not by the
 * module's "log-mask" prop (+0x08). Runtime values of those words are unknown;
 * levels 0/1 are error/warning, 3..5 info/debug/trace (logm convention).
 *
 * Evidence: build/decomp/audio_a0_dsp2.main_60000000.bin.c, build/scratch/evidence/dsp2-connect/,
 *           build/scratch/evidence/runtime/idle-state.txt (instance props), build/scratch/a13/ listings.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ------------------------------------------------------------------------
 * Types (offsets read from the firmware)
 * ---------------------------------------------------------------------- */
enum aud_event_type {            /* aud_event +0x0c */
    EV_ENABLE_DISABLE = 0,
    EV_FIRST_AU = 1,
    EV_LAST_AU = 2,
    EV_MARK = 5,
    EV_DISCONNECT = 6,
    EV_FLUSH = 7,
    EV_CUSTOM = 13,
};

enum aud_data_type { DATA_PCM, DATA_ES, DATA_META, DATA_META_EXT }; /* aud_data +0x10 */

enum prop_type { PROP_STR0, PROP_BOOL = 1, PROP_INT = 2, PROP_STRING = 4, PROP_BLOB = 5 };

struct aud_module;
struct aud_port;
struct aud_event { int type; unsigned flags; int param_size; void *param; };
struct aud_data { int type; int index; uint32_t ts; void *buf; unsigned size; };

/* property descriptor, 0x28 bytes */
struct aud_module_property {
    const char *name;            /* +0x00 */
    int         type;            /* +0x04 enum prop_type */
    unsigned    offset;          /* +0x08 byte offset into the module, 0 = none */
    unsigned    flags;           /* +0x0c bit0 read-only */
    int (*set)(struct aud_module *, const struct aud_module_property *,
               const char *val, unsigned size);                  /* +0x10 */
    void       *gets, *geti;     /* +0x14, +0x18 */
};

struct aud_prop_list_node {      /* runtime-registered property, list link at +0x20 */
    struct aud_module_property prop;
    struct aud_prop_list_node *next;
};

struct aud_module_impl {
    const char *name;                                                 /* +0x04 */
    int (*chain)(struct aud_port *, struct aud_data *);               /* +0x10 */
    int (*event)(struct aud_module *, struct aud_port *, struct aud_event *); /* +0x14 */
    int (*cmd)(struct aud_module *, unsigned, void *, unsigned);      /* +0x18 */
    void (*prop_updated)(struct aud_module *, const struct aud_module_property *); /* +0x1c */
    const struct aud_module_property *props;                          /* +0x30 */
    struct aud_prop_list_node *prop_list;                             /* +0x38 */
    unsigned flag;                                                    /* +0x4c bit0 SKIP_SYNC */
};

struct aud_port {
    struct aud_module *module;   /* +0x04 */
    unsigned  flag;              /* +0x08 bit0 OUTPUT, bit1 DISABLED */
    int       stream_lock;       /* +0x0c */
    struct aud_port  *up_peer;   /* +0xec */
    void **pending;              /* +0xf0 owned object references */
    unsigned pending_num;        /* +0xf8 */
    int       throwing;          /* +0x12c */
    struct aud_event *current_first; /* +0x140 */
    unsigned  passed;            /* +0x144 */
    uint64_t  last_clock;        /* +0x148 */
    unsigned  flag_work;         /* +0x150 bit0 IN_CHAIN */
};

struct aud_module {
    const char *name;                       /* +0x00 */
    unsigned    log_mask;                   /* +0x08 property "log-mask" (not the gate) */
    const struct aud_module_impl *impl;     /* +0x0c */
    void       *pending_root;               /* +0x3c output ports (foreach root) */
    void       *imc;                        /* +0x68 */
    int         uid;                        /* +0x04 */
    int         thread_priority;            /* +0x90 property "thread-priority" */
    bool        drop_output_data;           /* +0x94 property "drop-output-data" */
    bool        drop_output_meta;           /* +0x95 property "drop-output-meta" */
    bool        block_module;               /* +0x96 property "block-module" */
    bool        do_chain_profile;           /* +0x97 property "do-chain-profile" */
    uint64_t    chain_profile;              /* +0x98 */
    struct aud_event *first_event_out;      /* +0xa8 last FIRST seen at an input */
    bool        othread_running;            /* +0xac */
    int (*othread_fn)(struct aud_module *, void *); /* +0xb0 */
    void       *othread_arg;                /* +0xb4 */
    void       *othread_mutex;              /* +0xb8 held while othread_fn runs */
    void       *othread_cond;               /* +0xbc signalled on destroy only */
    void       *othread;                    /* +0xc0 */
    int         prop_access_lock;           /* +0x74 */
    int         block_cond;                 /* +0xc4 (0x100-0x3c) */
    int         block_mutex;                /* +0xd4 (0x100-0x2c) */
};
/* sizeof(struct aud_module) == 0xf0 (bypass class instance size, @0x60101db4). */

/* ------------------------------------------------------------------------
 * Externals
 * ---------------------------------------------------------------------- */
#define LOG_ON(m, lvl) ((*(m)) & (1u << (lvl)))
extern volatile unsigned *core_log_mask;            /* 0x4ff9004c */
extern void logmm_printl(void *ctx, int lvl, const char *fn, int line, const char *fmt, ...);
extern void *core_log_ctx;                          /* 0x601010a0 */

extern void aud_mutex_lock(void *);
extern void aud_mutex_unlock(void *);
extern void aud_cond_signal(void *);
extern void aud_cond_wait(void *cond, void *mutex);
extern void *aud_thread_create(const char *name, unsigned stack, int prio,
                               void *fn, void *arg);                          /* @0x600a6634 */
extern void aud_thread_destroy(void *);                                      /* @0x600a66ac */
extern void *aud_object_get(void *);
extern void aud_object_put(void *);
extern struct aud_port *aud_module_get_output(struct aud_module *, int);      /* @0x60073984 */
extern struct aud_port *aud_module_new_output(struct aud_module *, int);      /* @0x60073c98 */
extern int  aud_port_sync_locked(void *, struct aud_port *);                 /* @0x600741d8 foreach_sync */
extern void aud_link_foreach_cb(void *root, int arg, int (*cb)(void *, struct aud_port *));
extern int  foreach_down_peer_push_event(struct aud_event *, struct aud_port *); /* @0x60072860 */
extern int  aud_module_propagate_event(struct aud_module *, struct aud_event *); /* @0x600741b0 */
extern int  aud_module_event(struct aud_module *, struct aud_port *, struct aud_event *); /* @0x60075024 */
extern int  aud_module_cmd(struct aud_module *, unsigned, void *, unsigned);  /* @0x600750a8 */
extern int  aud_module_get_prop(struct aud_module *, const char *name);      /* @0x60075b74 */
extern struct aud_event *aud_event_new_first(void);                          /* @0x60069e20 */
extern struct aud_event *aud_event_deserialize(void *, unsigned);            /* @0x6006aa98 */
extern void aimc_raise_named_event(void *imc, int uid, const char *name, void *p, int n);
extern void adjust_thread_priority(struct aud_module *);                     /* @0x60074674 */
extern uint64_t aud_thread_cycle(void);
extern uint32_t aud_system_cycle(void);
extern int  set_prop(struct aud_module *, const struct aud_module_property *,
                     const char *val, unsigned size);                        /* @0x600750f4 */
extern const struct aud_module_property aud_module_default_prop[];           /* @0x60002360 */
extern int  aud_module_chain_locked(struct aud_port *, struct aud_data *);   /* @0x600746d4 */

/* IMC command ids (aimc cmd word = id << 16 | param size) */
#define ADEC_CMD_START            0x10010000u
#define ADEC_CMD_STOP             0x10020000u
#define ADEC_CMD_FLUSH_LIKE       0x10150000u   /* answered 0, name unknown */
#define ADEC_CMD_GET_PROP         0x10170020u
#define ADEC_CMD_SET_PROP         0x10180004u   /* kernel aimc_set_property() */
#define ADEC_CMD_ASYNC_EVENT      0x10190000u
#define ADEC_CMD_SYNC             0x101a0000u
#define ADEC_CMD_SET_PROP_SINGLE  0x101c0004u
#define ADEC_CMD_SET_BASETIME_ACK 0x10200008u   /* answered 0 */

/* ------------------------------------------------------------------------
 * Data path
 *
 * aud_module_push_data @0x600724d8 (tagged in renderer_path.c), observed order:
 *   1. data == NULL                       -> "%s: no data" (lvl 4), return 0
 *   2. m->drop_output_data (+0x94)        -> "%s: drop output data." (lvl 4), put, 0
 *   3. m->drop_output_meta (+0x95) && data->type == META(2)
 *                                         -> same string (lvl 4), put, 0
 *   4. no output port <n>                 -> "%s: no output port %d" (lvl 4), put, 0
 *   5. lock stream_lock, add_pending_obj_locked(), unlock; if the global
 *      sync_at_push (0x6005d9b8) is set, aud_module_sync(m, n).
 * All drop logs are level 4 (debug; bit 4 of 0x4ff9004c); there is no counter.
 * NOTE for renderer_path.c: Ghidra's "param_1 + 0x25" there is a uint32 index,
 * i.e. byte +0x94 == drop_output_data, not a separate "drop_all" word.
 * Events never pass through this function: aud_module_push_event below has no
 * drop-output-* check, so a FIRST/LAST event is forwarded even while data is dropped.
 * ---------------------------------------------------------------------- */

/* @0x6007496c aud_module_chain: port stream lock + an inlined copy of
 * aud_module_chain_locked. Caller: bin_chain (0x6007131d), i.e. data entering a
 * bin from outside. Same checks, same order as 0x600746d4. */
int aud_module_chain(struct aud_port *port, struct aud_data *data)
{
    struct aud_module *m = port->module;
    int ret;

    aud_mutex_lock(&port->stream_lock);
    aud_mutex_lock(&m->block_mutex);
    if (m->block_module) {
        /* "%s : blocked.. goto sleep" / "%s : awaken from block!!" (lvl 3) */
        aud_cond_wait(&m->block_cond, &m->block_mutex);
    }
    aud_mutex_unlock(&m->block_mutex);

    if (port->flag & 2) {                             /* DISABLED: "port disconnecting." */
        aud_object_put(data);
        ret = -1;
        goto out;
    }
    if (port->throwing) {                             /* "data index %6d, Throwing...%s" */
        aud_object_put(data);
        ret = -1;
        goto out;
    }
    if (m->thread_priority)
        adjust_thread_priority(m);

    if (!port->current_first) {                       /* same replay as @0x600746d4 */
        struct aud_event *e = NULL;
        if (port->up_peer && port->up_peer->current_first)
            e = aud_object_get(port->up_peer->current_first);
        if (!e && !(e = aud_event_new_first())) {     /* "NULL e." (lvl 0) */
            ret = -1;
            goto out;
        }
        port->current_first = e;
        m->impl->event(m, port, aud_object_get(e));
    }

    port->last_clock = aud_system_cycle();
    port->passed += data->size;
    port->flag_work |= 1;
    if (m->do_chain_profile) {
        uint64_t t0 = aud_thread_cycle();
        ret = m->impl->chain(port, data);
        m->chain_profile += aud_thread_cycle() - t0;
    } else {
        ret = m->impl->chain(port, data);
    }
    port->flag_work &= ~1u;

    /* @0x60074b34 [FLIX?]: with no output thread and impl->flag bit0 clear, every
     * output port with pending data is synced (aud_link_foreach -> 0x600741d8). */
    if (!m->othread && !(m->impl->flag & 1))
        aud_link_foreach_cb(&m->pending_root, 0, aud_port_sync_locked);
out:
    aud_mutex_unlock(&port->stream_lock);
    return ret;
}

/* @0x60072b38 aud_module_push_event: no drop-output-* test here. */
int aud_module_push_event(struct aud_module *m, int out_port_num, struct aud_event *e)
{
    struct aud_port *out;

    if (!e)
        return -1;                                    /* "no event" (lvl 4) */
    out = aud_module_new_output(m, out_port_num);     /* creates the port if missing */
    if (!out) {
        /* "no mem for port %d on %s" (lvl 0) */
        aud_object_put(e);
        return -1;
    }
    foreach_down_peer_push_event(e, out);             /* stores FIRST in out->current_first */
    aud_object_put(e);
    return 0;
}

/* Inlined release tail of aud_port_sync_locked, 0x600742b3..742db, not a
 * standalone firmware function. Reloads the array before each put and count
 * after each put; does not detach a batch before callbacks. The logged
 * foreach_sync+0xf4 is the return PC 0x600742cc of put at 0x600742c9.
 * Whether this capture mutated the pending array is unresolved. */
void reconstructed_port_release_pending(struct aud_port *port)
{
    for (unsigned index = 0; index < port->pending_num; ++index)
        aud_object_put(port->pending[index]);
    port->pending_num = 0;
}

/* @0x60072bb0 aud_module_sync */
int aud_module_sync(struct aud_module *m, int out_port_num)
{
    struct aud_port *out;

    if (out_port_num < 0) {
        aud_link_foreach_cb(&m->pending_root, 0, aud_port_sync_locked);
        return 0;
    }
    out = aud_module_get_output(m, out_port_num);
    if (!out)
        return 0;                                     /* "no output port %d on %s" (lvl 4) */
    aud_port_sync_locked(NULL, out);
    return 0;
}

/* @0x6007242c aud_module_unblock */
void aud_module_unblock(struct aud_module *m)
{
    aud_mutex_lock(&m->block_mutex);
    if (m->block_module) {
        /* "trying to unblock module %s" (lvl 3) */
        m->block_module = false;
        aud_cond_signal(&m->block_cond);
    }
    aud_mutex_unlock(&m->block_mutex);
}

/* @0x60073adc aud_module_have_active_output: any output port with a down peer. */
int aud_module_have_active_output(struct aud_module *m)
{
    extern struct aud_port *first_output(struct aud_module *), *next_output(struct aud_port *);
    extern bool port_has_down_peer(struct aud_port *);
    int active = 0;

    aud_mutex_lock((char *)m + 0x44);
    for (struct aud_port *p = first_output(m); p; p = next_output(p))
        if (port_has_down_peer(p)) { active = 1; break; }
    aud_mutex_unlock((char *)m + 0x44);
    return active;
}

/* ------------------------------------------------------------------------
 * Properties. Every lookup is an exact strcmp (0x600ea588 is plain strcmp):
 * there is no '_' <-> '-' folding anywhere. The default table @0x60002360 holds
 * thread-priority, drop-output-data, drop-output-meta, block-module, log-mask,
 * log-level, do-chain-profile, chain-profile. No dsp2 string contains
 * "drop_output" at all, so the kernel's renderer_preconnect/postconnect
 * "drop_output_data=1/0" (literal in the shipped Image) matches nothing.
 * ---------------------------------------------------------------------- */

/* @0x60075410 search_prop_set */
int search_prop_set(struct aud_module *m, const struct aud_module_property *props,
                    const char *name, const char *val, unsigned size)
{
    extern int strcmp(const char *, const char *);

    if (!props)
        return -1;
    for (; props && props->name; props++) {
        if (strcmp(props->name, name))
            continue;
        if ((props->offset || props->set) && !(props->flags & 1))
            return set_prop(m, props, val, size);
    }
    return -1;
}

/* @0x60075454 set_prop_list: runtime-registered properties (impl +0x34/+0x38 list) */
int set_prop_list(struct aud_module *m, const char *name, const char *val, unsigned size)
{
    extern int strcmp(const char *, const char *);

    for (struct aud_prop_list_node *n = m->impl->prop_list; n; n = n->next) {
        if (strcmp(n->prop.name, name))
            continue;
        if (!n->prop.offset && !n->prop.set)
            return -1;
        if (n->prop.flags & 1)
            return -1;
        return set_prop(m, &n->prop, val, size);
    }
    return -1;
}

/* @0x600754e8 set_block_module: "block-module" setter */
int set_block_module(struct aud_module *m, const struct aud_module_property *prop,
                     const char *val, unsigned size)
{
    extern int strcmp(const char *, const char *);
    bool on = !strcmp(val, "true") || !strcmp(val, "yes") || !strcmp(val, "1");
    (void)size;

    aud_mutex_lock(&m->block_mutex);
    if (m->block_module == on) {
        /* "setting same value. ignore. %s: %s" (lvl 3) */
        (void)prop;
    } else {
        bool was = m->block_module;
        m->block_module = on;                         /* "%s: set block as %s" (lvl 3) */
        if (was && !on) {                             /* "%s : unblock!" (lvl 3) */
            m->block_module = false;                  /* inlined aud_module_unblock_locked */
            aud_cond_signal(&m->block_cond);
        }
    }
    aud_mutex_unlock(&m->block_mutex);
    return 0;
}

/* @0x600759cc aud_module_set_prop: ADEC_CMD_SET_PROP payload "name=val[\name=val...]".
 * Unknown names are skipped SILENTLY (no log on this path). */
int aud_module_set_prop(struct aud_module *m, const char *str, unsigned len)
{
    extern void *aud_malloc(unsigned);
    extern void aud_free(void *);
    extern void *memcpy(void *, const void *, size_t);
    extern char *strtok_r(char *, const char *, char **);
    static const char delim[] = "=\\";                /* 0x60043d40 */
    extern size_t strnlen(const char *, size_t);      /* @0x600fba98 */
    char *buf, *save, *name, *val;
    int n = (int)strnlen(str, len);

    buf = aud_malloc((unsigned)n + 1);
    if (!buf)
        return 0;                                     /* "nomem." (lvl 0) */
    memcpy(buf, str, (size_t)n + 1);
    buf[n] = 0;

    for (name = strtok_r(buf, delim, &save); name; name = strtok_r(NULL, delim, &save)) {
        val = strtok_r(NULL, delim, &save);
        if (!val) {                                   /* "no data for data. str %s" (lvl 0) */
            break;
        }
        if (search_prop_set(m, m->impl->props, name, val, 0) < 0 &&
            search_prop_set(m, aud_module_default_prop, name, val, 0) < 0)
            set_prop_list(m, name, val, 0);           /* result ignored: no warning */
    }
    aud_free(buf);
    return 0;
}

/* @0x60076444 aud_module_default_command */
int aud_module_default_command(struct aud_module *m, unsigned cmd, void *param, unsigned len)
{
    switch (cmd) {
    case ADEC_CMD_GET_PROP:                           /* FLIX @0x6007644f -> 0x600765b4 */
        return aud_module_get_prop(m, param);
    case ADEC_CMD_SET_PROP:                           /* FLIX @0x60076457 -> 0x6007652a */
        aud_module_set_prop(m, (const char *)param + 4, *(unsigned *)param);
        return 0;
    case ADEC_CMD_SET_PROP_SINGLE: {                  /* one "name=value" */
        extern void *memcpy(void *, const void *, size_t);
        const int *p = param;
        const char *s = (const char *)(p + 1);
        int i = 0;

        while (i < p[0] && s[i] != '=')
            i++;
        if (i >= p[0]) {
            /* "no =" (lvl 1) */
            return 0;
        }
        {
            char name[i + 1];
            const char *val = s + i + 1;
            unsigned vlen = (unsigned)(p[0] - i - 1);

            memcpy(name, s, (size_t)i);
            name[i] = 0;
            if (search_prop_set(m, m->impl->props, name, val, vlen) < 0 &&
                search_prop_set(m, aud_module_default_prop, name, val, vlen) < 0 &&
                set_prop_list(m, name, val, vlen) < 0) {
                /* "no such property. name %s, on %s" (lvl 1, warning) */
            }
        }
        return 0;
    }
    case ADEC_CMD_ASYNC_EVENT: {                      /* "ADEC_CMD_ASYNC_EVENT received at %s" */
        struct aud_event *e = aud_event_deserialize(param, len);
        if (e)
            aud_module_event(m, NULL, e);
        return 0;
    }
    case ADEC_CMD_SYNC:
    case ADEC_CMD_FLUSH_LIKE:
    case ADEC_CMD_SET_BASETIME_ACK:
        return 0;
    default:
        /* "command not handled. %08x(%s) at %s" (lvl 3) */
        return -1;
    }
}

/* @0x600766f4 othread_loop: a dedicated XOS thread (priority 10), not a scheduler
 * queue job. The callback owns the loop body and must wait on its own cond. */
int othread_loop(struct aud_module *m)
{
    aud_mutex_lock(m->othread_mutex);
    while (m->othread_running)
        m->othread_fn(m, m->othread_arg);
    aud_mutex_unlock(m->othread_mutex);
    return 0;
}

/* @0x60076728 aud_module_output_thread_create */
int aud_module_output_thread_create(struct aud_module *m,
                                    int (*fn)(struct aud_module *, void *), void *arg,
                                    void *mutex, void *cond)
{
    if (m->othread) {
        if (LOG_ON(core_log_mask, 1))
            logmm_printl(core_log_ctx, 1, "aud_module_output_thread_create", 0xa4b,
                         "%s: output thread already running.\n", m->name);
        return -1;
    }
    m->othread_fn = fn;
    m->othread_arg = arg;
    m->othread_running = true;
    m->othread_mutex = mutex;
    m->othread_cond = cond;
    if (LOG_ON(core_log_mask, 3))
        logmm_printl(core_log_ctx, 3, "aud_module_output_thread_create", 0xa55,
                     "%s: create output thread.\n", m->name);
    m->othread = aud_thread_create(m->name, 0x10000, 10, (void *)othread_loop, m);
    if (!m->othread) {
        if (LOG_ON(core_log_mask, 1))
            logmm_printl(core_log_ctx, 1, "aud_module_output_thread_create", 0xa5a,
                         "%s: cannot make output thread.\n", m->name);
        return -1;
    }
    return 0;
}

/* @0x600767c4 aud_module_output_thread_destroy: wakes othread_cond only. A callback
 * parked on any other cond (block_cond, a downstream chain wait) is not released. */
int aud_module_output_thread_destroy(struct aud_module *m)
{
    if (!m->othread) {
        if (LOG_ON(core_log_mask, 1))
            logmm_printl(core_log_ctx, 1, "aud_module_output_thread_destroy", 0xa65,
                         "%s: output thread not running..\n", m->name);
        return -1;
    }
    aud_mutex_lock(m->othread_mutex);
    if (LOG_ON(core_log_mask, 3))
        logmm_printl(core_log_ctx, 3, "aud_module_output_thread_destroy", 0xa6a,
                     "%s: stop output thread.\n", m->name);
    m->othread_running = false;
    aud_cond_signal(m->othread_cond);
    aud_mutex_unlock(m->othread_mutex);
    aud_thread_destroy(m->othread);
    m->othread = NULL;
    return 0;
}

/* @0x60076608 aud_module_default_event */
int aud_module_default_event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    switch (e->type) {
    case EV_FIRST_AU:
        /* "first event received at input%d of %s" (lvl 3) */
        if (m->first_event_out)
            aud_object_put(m->first_event_out);
        m->first_event_out = aud_object_get(e);
        break;
    case EV_LAST_AU:
        /* "last event received at input%d of %s" (lvl 3) */
        aimc_raise_named_event(m->imc, m->uid, "last", NULL, 0);
        break;
    case EV_MARK:
        /* "mark event with param size %d" (lvl 3) */
        aimc_raise_named_event(m->imc, m->uid, "mark", e->param, e->param_size);
        break;
    default:
        /* FLIX @0x6007660d / @0x60076615 [FLIX?]: two more types issue
         * aud_module_cmd(START) / aud_module_cmd(STOP) before propagating. */
        break;
    }
    return aud_module_propagate_event(m, e);
}
