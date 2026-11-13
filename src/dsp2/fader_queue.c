/*
 * dsp2 "fader" module, also registered as "queue" and "input_queue" (fader_init):
 * input queue with back-pressure, the wait/fadein/running/fadeout/stop output state
 * machine, pending-event ordering, the block-queue gate and the low-delay bypass thread.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 * Main segment linked at 0x60000000. Every op0=0xE/0xF FLIX bundle is 8 bytes and
 * OPAQUE. Branch-shaped bundles were read with the hypothesis decoder in
 * tools/flix_annot.py; conditions that depend on it are marked [FLIX?].
 * Log-guard bundles (l32i aN,<mask 0x4ff90090> or module debug word +0x8) are dropped.
 * Evidence: build/decomp/audio_a0_dsp2.main_60000000.bin.c (0x60084b90..0x60087770),
 *           property table @0x60101ec0, class @0x60101e10, variant descriptors
 *           @0x60101e70 ("fader"), @0x60101e88 ("queue"), @0x60101ea0 ("input_queue").
 * Names: firmware symbol table at 0x6011e780 (func,name) pairs. It names 0x600855e8
 * fadeout_bypass, 0x60085610 _fader_process_data_fadeout and 0x60085464
 * fadeout_32bit_4ch (functions.tsv has fader_event_first / underflow / fader_cmd there).
 * queue_module_unblock_locked (__func__ slot 0x6005f134, L32R at 0x600862ee and
 * 0x600863eb) was inlined into queue_module_unblock and set_block_queue.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------------
 * Framework types
 * ---------------------------------------------------------------------- */
enum { AUD_DATA_PCM, AUD_DATA_ES, AUD_DATA_META, AUD_DATA_META_EXT };
enum { AUD_EVENT_FIRST_AU = 1, AUD_EVENT_LAST_AU = 2, AUD_EVENT_FLUSH = 7,
       AUD_EVENT_PASSING = 9, AUD_EVENT_CUSTOM = 0xd };
#define AUD_EVENT_PROP_SYNC 0x2
#define PORT_DISABLED       0x2

struct aud_data {
    int      type;               /* +0x10 */
    int      index;              /* +0x14 */
    uint32_t ts;                 /* +0x18 */
    unsigned size;               /* +0x34 */
    void    *ptr;                /* +0x38 */
};
struct aud_event {
    int      type;               /* +0x0c */
    unsigned prop;               /* +0x10 */
};
struct aud_mutex { int opaque; };
struct aud_cond  { int opaque[4]; };
struct aud_list  { struct aud_list *prev, *next; };    /* prev at +0, next at +4 */

/* aud_data_adapter: a byte FIFO of aud_data. total-offset = queued bytes. */
struct aud_data_adapter {
    uint8_t  priv[0x10];
    unsigned total;              /* +0x10 (module +0x10c) */
    unsigned offset;             /* +0x14 (module +0x110) */
};

/* Variant descriptor (class +0x50): defaults and the queue flag. */
struct fader_desc {
    int wait_len, stop_len, max_len, fade_len, chunk_len;
    int is_queue;                /* +0x14: 0 "fader", 1 "queue"/"input_queue" */
};
struct aud_module_class {
    const char *name;            /* +0x04 */
    const struct fader_desc *desc; /* +0x50 */
};

struct pending_event {           /* calloc(1, 0x10) */
    struct aud_list   link;
    struct aud_event *event;     /* +0x08 */
    int               at_bytes;  /* +0x0c bytes still queued in front of it */
};

enum fader_state { FS_WAIT, FS_FADEIN, FS_RUNNING, FS_FADEOUT, FS_STOP };

struct fader;
typedef int  (*fader_state_fn)(struct fader *);
typedef void (*fader_fade_fn)(struct fader *, int32_t *dst, const int32_t *src, int frames);

struct fader {                   /* private size 0x298 */
    const char *name;            /* +0x000 */
    uint32_t    uid;             /* +0x004 */
    uint32_t    debug;           /* +0x008 bit0 err, 2 lvl2, 3 info, 4 lvl4, 5 lvl5 */
    const struct aud_module_class *cls; /* +0x00c */
    void       *aimc;            /* +0x068 */
    void       *output_thread;   /* +0x0c0 */
    int         channels;        /* +0x0f0 */
    int         bits;            /* +0x0f4 */
    int         frame_bytes;     /* +0x0f8 */
    struct aud_data_adapter q;   /* +0x0fc */
    struct aud_list pe;          /* +0x118 pending events (sentinel) */
    struct aud_mutex pe_lock;    /* +0x120 */
    struct aud_mutex lock;       /* +0x13c main lock */
    struct aud_cond  in_cond;    /* +0x158 producer waits here when full */
    struct aud_cond  out_cond;   /* +0x168 output thread waits here */
    struct aud_cond  block_cond; /* +0x178 output thread parks here while blocked */
    struct aud_mutex block_lock; /* +0x188 */
    struct { int dummy; } md_in; /* +0x1a4 msf reader */
    struct { int dummy; } md_out;/* +0x1c0 msf writer */
    int         state;           /* +0x1c8 enum fader_state */
    fader_state_fn process;      /* +0x1cc */
    fader_fade_fn  fadein;       /* +0x1d0 */
    fader_fade_fn  fadeout;      /* +0x1d4 */
    int         wait_len;        /* +0x1d8 "wait-length" (frames) */
    int         stop_len;        /* +0x1dc "stop-length" */
    int         max_len;         /* +0x1e0 "max-length" */
    int         fade_len;        /* +0x1e4 "fade-length" */
    int         fade_pos;        /* +0x1e8 */
    int         padding_len;     /* +0x1ec "padding-length" */
    int         chunk_len;       /* +0x1f0 "chunk-length" */
    unsigned    chunk_bytes;     /* +0x1f4 */
    int         es;              /* +0x1fc FIRST_AU "es" */
    int         zero_left;       /* +0x200 front padding frames still queued */
    int         flushing;        /* +0x204 async FLUSH seen: drop input */
    bool        enable_underflow;/* +0x208 "enable_underflow" */
    int         send_underflow;  /* +0x20c "send_underflow" */
    int         underflow_count; /* +0x210 "underflow-count" */
    int         queued_len;      /* +0x214 "queued-length" (ro) */
    unsigned    max_queued_len;  /* +0x218 "max-queued-length" */
    bool        first_sent;      /* +0x21c FIRST_AU went out; wait state then sends LAST */
    int         dropped_frames;  /* +0x220 frames dropped by fader_chain */
    bool        set_flush;       /* +0x224 "set-flush" */
    struct aud_mutex md_lock;    /* +0x228 */
    int         dot_on_wait;     /* +0x244 "draw-dot-on-wait" */
    bool        low_delay;       /* +0x248 FIRST_AU "low-delay" */
    bool        dolby;           /* +0x249 */
    bool        bypass;          /* +0x24a current bypass */
    bool        bypass_req;      /* +0x24b "bypass" */
    bool        block;           /* +0x24c "block-queue" */
    bool        pe_signal;       /* +0x24d */
    struct aud_event *first;     /* +0x250 FIRST_AU kept for the output side */
    struct aud_data  *meta_ext;  /* +0x254 */
    void       *bypass_thread;   /* +0x258 */
    bool        bypass_run;      /* +0x25c */
    struct aud_mutex bypass_lock;/* +0x260 */
    struct aud_cond  bypass_cond;/* +0x27c */
    uint64_t    last_input;      /* +0x290 cycle of last bypass input; 1 = LAST sent */
};

#define QUEUED_BYTES(f)  ((f)->q.total - (f)->q.offset)
#define QUEUED(f)        ((int)(QUEUED_BYTES(f) / (unsigned)(f)->frame_bytes))

/* ------------------------------------------------------------------------
 * Callees
 * ---------------------------------------------------------------------- */
extern volatile uint32_t fader_log_mask;          /* *0x4ff90090 */
extern const void *const fader_log;               /* 0x60101e00 */
extern void logmm_printl(const void *log, int lvl, const char *fn, int line, const char *fmt, ...);
extern void logmm_printlm(const char *mod, int lvl, const char *fn, int line, const char *fmt, ...);
extern void logm_message(const char *fmt, ...);

extern void *aud_object_get(void *o);
extern void  aud_object_put(void *o);
extern struct aud_event *aud_event_dup(struct aud_event *e);
extern struct aud_event *aud_event_new_last(void);
extern struct aud_event *aud_event_newv_custom(const char *name, int a, int b, int c);
extern const char *aud_event_name(struct aud_event *e);
extern int   aud_event_value_get_int(struct aud_event *e, const char *k, int def);
extern bool  aud_event_value_get_bool(struct aud_event *e, const char *k, bool def);
extern void  aud_event_value_set_int(struct aud_event *e, const char *k, int v);
extern void  aud_event_value_del(struct aud_event *e, const char *k);
extern int   aud_module_default_event(struct fader *f, void *port, struct aud_event *e);
extern int   aud_module_default_command(struct fader *f, uint32_t cmd, void *arg, unsigned len);
extern int   aud_module_push_data(struct fader *f, int port, struct aud_data *d);
extern int   aud_module_push_event(struct fader *f, int port, struct aud_event *e);
extern void  aud_module_sync(struct fader *f, int port);
extern int   aud_module_output_thread_create(struct fader *f, void *fn, void *arg,
                                             struct aud_mutex *mx, struct aud_cond *c);
extern void  aud_module_output_thread_destroy(struct fader *f);
/* @0x600f2c50: requeue the current XOS thread at its priority and switch; symtab's
 * "aud_port_push_event" at this address is a look-alike. */
extern void  xos_thread_yield(void);
extern struct aud_data *aud_data_new(unsigned size);
extern struct aud_data *aud_data_new_zero(unsigned size);
extern struct aud_data *aud_data_new_metadata(void *md);
extern void  aud_data_adapter_init(struct aud_data_adapter *a);
extern void  aud_data_adapter_cleanup(struct aud_data_adapter *a);
extern void  aud_data_adapter_push(struct aud_data_adapter *a, struct aud_data *d);
extern void  aud_data_adapter_push_front(struct aud_data_adapter *a, struct aud_data *d);
extern void  aud_data_adapter_flush(struct aud_data_adapter *a, unsigned bytes);
extern void *aud_data_adapter_map(struct aud_data_adapter *a, unsigned bytes);
extern void  aud_data_adapter_unmap(struct aud_data_adapter *a, void *p);
extern struct aud_data *aud_data_adapter_dequeue(struct aud_data_adapter *a);
extern void  aud_mutex_init(struct aud_mutex *m);
extern void  aud_mutex_lock(struct aud_mutex *m);
extern void  aud_mutex_unlock(struct aud_mutex *m);
extern void  aud_cond_init(struct aud_cond *c);
extern void  aud_cond_wait(struct aud_cond *c, struct aud_mutex *m);
extern int   aud_cond_wait_timeout(struct aud_cond *c, struct aud_mutex *m, unsigned us);
extern void  aud_cond_signal(struct aud_cond *c);
extern void *aud_thread_create(const char *name, unsigned stack, int prio, void *fn, void *arg);
extern void  aud_thread_destroy(void *t);
extern uint64_t aud_system_cycle(void);
extern void  aimc_raise_named_event(void *aimc, uint32_t uid, const char *name, const void *p, int n, ...);
extern void  dot_module_connection(int a, int b);
extern int   msf_get_curr_num_md(void *md);
extern int   msf_get_avail_samples_read(void *md);
extern void *msf_get_metadata(void *md, int frames);
extern void  msf_add_put_metadata(void *md, void *meta);
extern void  aud_meta_data_free(void *meta);
extern void  msf_add_update_write_buf(void *md, int frames);
extern void  msf_update_read_buf(void *md, int frames);
extern int   msf_process_metadata(void *md, void *ptr, unsigned size);
extern void  msf_flush_metadata(void *md);
extern void *audutil_set_opts(const void *opts, const char *arg);
extern char *audutil_strdup_printf(const char *fmt, ...);
extern bool  fader_trace_enabled(const char *module_name, int in_not_out);  /* [GAP] byte flags @0x60167f88 + name match */
extern void  ModuleRegistry_RegisterNamef(const void *cls, const void *desc, const char *name);

extern const struct aud_module_class fader_class;       /* 0x60101e10 */
extern const struct fader_desc fader_desc_fader;        /* 0x60101e70 */
extern const struct fader_desc fader_desc_queue;        /* 0x60101e88 */
extern const struct fader_desc fader_desc_input_queue;  /* 0x60101ea0 */
extern const void *const fader_debug_opts;              /* 0x60003ce0 */

#define CMD_SET_DELAY_A   0x57040008u
#define CMD_SET_DELAY_B   0x5802000cu
#define WAIT_TIMEOUT_US   500000u          /* 0x6005f0d4 */
#define BYPASS_IDLE_US    5000u            /* 0x6005f190 */

static const char *state_name(int state)
{
    static const char *const names[] = { "wait", "fadein", "running", "fadeout", "stop" };
    return (unsigned)state < 5 ? names[state] : "unknown";
}

static bool is_queue_variant(const struct fader *f) { return f->cls->desc->is_queue == 1; }

int _fader_process_data_wait(struct fader *f);
int _fader_process_data_fadein(struct fader *f);
int _fader_process_data_running(struct fader *f);
int _fader_process_data_fadeout(struct fader *f);
int process_data_stop(struct fader *f);
int pending_events_check(struct fader *f);
int fader_start(struct fader *f);

/* ------------------------------------------------------------------------
 * Helpers
 * ---------------------------------------------------------------------- */

/* @0x60084b90: debug-console handler "fader input queue data" */
int debug_fader_input_queue_data(void *unused, const char *args)
{
    struct { int kind; const char *name; void *value; } *o;
    (void)unused;
    o = audutil_set_opts(fader_debug_opts, args + 4);
    if (!o) {
        logm_message("unknown..\n");
        return 0;
    }
    if (o->kind == 1)
        logm_message("%s %s\n", o->name, *(const char *)o->value ? "on" : "off");
    else if (o->kind == 0)
        logm_message("%s %d\n", o->name, *(const int *)o->value);
    return 0;
}

/* @0x60084be8: forward metadata covering `frames` frames ahead of the PCM buffer. */
int _fader_write_metadata(struct fader *f, int frames, int consume, uint32_t ts)
{
    struct aud_data *md;

    if (f->meta_ext) {
        logmm_printl(fader_log, 5, "_fader_write_metadata", 0x124, "%s: push metadata ext", f->name);
        aud_module_push_data(f, 0, f->meta_ext);
        aud_module_sync(f, 0);
        f->meta_ext = NULL;
    }
    if (f->debug & 0x10)
        logmm_printlm(f->name, 4, "_fader_write_metadata", 0x12f,
                      "num_md %d num_sam %d process ...", msf_get_curr_num_md(&f->md_in),
                      msf_get_avail_samples_read(&f->md_in), consume);
    /* [FLIX?] @0x60084c18: with no metadata at all it logs "no operation" (0x134), returns */
    if (consume) {
        void *m;
        while ((m = msf_get_metadata(&f->md_in, frames)) != NULL) {
            msf_add_put_metadata(&f->md_out, m);
            aud_meta_data_free(m);
        }
    }
    msf_add_update_write_buf(&f->md_out, frames);
    if (consume)
        msf_update_read_buf(&f->md_in, frames);
    md = aud_data_new_metadata(&f->md_out);
    if (!md)
        return 0;
    md->ts = ts;
    aud_module_push_data(f, 0, md);
    aud_module_sync(f, 0);
    return 0;
}

/* @0x60084d1c: the output thread delivers synchronously. aud_module_sync runs the
 * downstream chain (delay -> mixerbin) in THIS thread, so any wait there stalls the queue. */
int _fader_push_data(struct fader *f, struct aud_data *d)
{
    if (!d) {
        logmm_printlm(f->name, 0, "_fader_push_data", 0x16c, "no data");
        return -1;
    }
    if (f->debug & 0x10)
        logmm_printlm(f->name, 4, "_fader_push_data", 0x170, "push data %d remained %d",
                      d->size / (unsigned)f->frame_bytes, QUEUED(f));
    aud_module_push_data(f, 0, d);
    aud_module_sync(f, 0);
    if (fader_trace_enabled(f->cls->name, 0))
        logm_message("[FADER] OUT id=%5d, sz=%d, ts=%10u\n", d->index, d->size, d->ts);
    if (f->debug & 0x20)
        logmm_printlm(f->name, 5, "_fader_push_data", 0x177, "done.");
    return 0;
}

static void list_del(struct aud_list *n)
{
    if (n->prev) {
        n->prev->next = n->next;
        n->next->prev = n->prev;
    }
    free(n);
}

static void list_add_head(struct aud_list *head, struct aud_list *n)
{
    n->prev = head->prev;
    n->next = head;
    head->prev->next = n;
    head->prev = n;
}

/* Shared by pending_events_add and _fader_add_pe_and_signal: one pending event per type.
 * @0x60084dd8..60084e7e: release all same-type entries, append the replacement at
 * the tail with the current byte count; take ownership without another get/copy. */
static int pe_add_locked(struct fader *f, struct aud_event *e)
{
    struct aud_list *it, *nx;
    struct pending_event *pe;

    for (it = f->pe.next; it != &f->pe; it = nx) {
        nx = it->next;
        pe = (struct pending_event *)it;
        if (pe->event->type == e->type) {
            if (f->debug & 8)
                logmm_printlm(f->name, 3, "pending_events_check_duplicated", 0x185,
                              "remove previous event %s", aud_event_name(pe->event));
            aud_object_put(pe->event);
            list_del(it);
        }
    }
    if (f->debug & 8)
        logmm_printlm(f->name, 3, "pending_events_add", 0x19a, "add pending event %s after %d bytes",
                      aud_event_name(e), (int)QUEUED_BYTES(f));
    pe = calloc(1, sizeof *pe);
    if (!pe) {
        if (f->debug & 1)
            logmm_printlm(f->name, 0, "pending_events_add", 0x19f, "no mem..");
        return -1;
    }
    pe->event = e;
    pe->at_bytes = (int)QUEUED_BYTES(f);
    list_add_head(&f->pe, &pe->link);
    return 0;
}

/* @0x60084dd8: queue event e to be sent after the bytes queued right now. */
int pending_events_add(struct fader *f, struct aud_event *e)
{
    int r;
    aud_mutex_lock(&f->pe_lock);
    r = pe_add_locked(f, e);
    aud_mutex_unlock(&f->pe_lock);
    return r;
}

/* @0x60084ee0: `bytes` were just output. */
int pending_events_update(struct fader *f, int bytes)
{
    aud_mutex_lock(&f->pe_lock);
    for (struct aud_list *it = f->pe.next; it != &f->pe; it = it->next)
        ((struct pending_event *)it)->at_bytes -= bytes;
    aud_mutex_unlock(&f->pe_lock);
    return 0;
}

/* @0x60084f1c: scan past held events, delivering ready events in insertion order.
 * @0x60084f50 (formerly labelled 60084f45): signed blti at_bytes,1 selects <= 0.
 * Ownership moves to the ready list, then downstream, with the main lock released. */
int pending_events_check(struct fader *f)
{
    struct aud_list ready = { &ready, &ready };
    struct aud_list *it, *nx;

    aud_mutex_lock(&f->pe_lock);
    f->pe_signal = false;
    for (it = f->pe.next; it != &f->pe; it = nx) {
        struct pending_event *pe = (struct pending_event *)it, *mv;
        nx = it->next;
        if (pe->at_bytes > 0)
            continue;
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "pending_events_check", 0x1c8, "ready %s",
                          aud_event_name(pe->event));
        mv = calloc(1, sizeof *mv);
        if (!mv) {
            if (f->debug & 1)
                logmm_printlm(f->name, 0, "pending_events_check", 0x1ce, "no mem..");
            aud_mutex_unlock(&f->pe_lock);
            return -1;
        }
        mv->event = pe->event;
        list_add_head(&ready, &mv->link);
        list_del(it);
    }
    aud_mutex_unlock(&f->pe_lock);

    for (it = ready.next; it != &ready; it = nx) {
        struct pending_event *pe = (struct pending_event *)it;
        nx = it->next;
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "pending_events_check", 0x1e0, "push sending event %s",
                          aud_event_name(pe->event));
        aud_mutex_unlock(&f->lock);
        aud_module_push_event(f, 0, pe->event);
        aud_module_sync(f, 0);
        aud_mutex_lock(&f->lock);
        list_del(it);
    }
    return 0;
}

/* @0x60085078 */
int pending_events_empty(struct fader *f)
{
    struct aud_list *it, *nx;
    aud_mutex_lock(&f->pe_lock);
    for (it = f->pe.next; it != &f->pe; it = nx) {
        struct pending_event *pe = (struct pending_event *)it;
        nx = it->next;
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "pending_events_empty", 0x1f6, "remove event %s (%d)",
                          aud_event_name(pe->event), pe->at_bytes);
        aud_object_put(pe->event);
        list_del(it);
    }
    aud_mutex_unlock(&f->pe_lock);
    return 0;
}

static void pe_shift(struct fader *f, int bytes)
{
    aud_mutex_lock(&f->pe_lock);
    for (struct aud_list *it = f->pe.next; it != &f->pe; it = it->next)
        ((struct pending_event *)it)->at_bytes -= bytes;
    aud_mutex_unlock(&f->pe_lock);
}

static void set_state(struct fader *f, int s, fader_state_fn fn)
{
    f->state = s;
    f->process = fn;
}

/* ------------------------------------------------------------------------
 * Output states (run by fader_output_work with f->lock held)
 * ---------------------------------------------------------------------- */

/* @0x600850ec: set-flush discards audio without shifting pending positions
 * (60085128..60085147). Otherwise positions advance before downstream sync;
 * delivery waits for sync AND the block gate to return to fader_output_work. */
int _fader_process_data_running(struct fader *f)
{
    struct aud_data *d;

    if (f->debug & 0x10)
        logmm_printlm(f->name, 4, "_fader_process_data_running", 0x205, "running..");

    aud_mutex_lock(&f->md_lock);
    if (f->set_flush) {                                         /* "set-flush" */
        logmm_printl(fader_log, 3, "_fader_process_data_running", 0x20a,
                     "%s: flushing size %d", f->name, (int)QUEUED_BYTES(f));
        if (QUEUED_BYTES(f))
            aud_data_adapter_flush(&f->q, QUEUED_BYTES(f));
        f->set_flush = false;
        set_state(f, FS_WAIT, _fader_process_data_wait);
        aud_cond_signal(&f->in_cond);
        aud_mutex_unlock(&f->md_lock);
        return 0;
    }
    aud_mutex_unlock(&f->md_lock);

    d = aud_data_adapter_dequeue(&f->q);
    if (d) {
        pending_events_update(f, (int)d->size);
        if (QUEUED(f) <= f->max_len)                            /* 0x6008527d */
            aud_cond_signal(&f->in_cond);
        aud_mutex_unlock(&f->lock);
        _fader_write_metadata(f, (int)(d->size / (unsigned)f->frame_bytes), 1, d->ts);
        _fader_push_data(f, d);

        /* 0x600852af..0x600852fd: the block-queue gate. While "block-queue" is true and
         * the queue is not full, the output thread sleeps after each buffer until
         * queue_module_unblock / set_block_queue(false) signals block_cond. */
        aud_mutex_lock(&f->block_lock);
        if (f->block) {
            logmm_printl(fader_log, 2, "_fader_process_data_running", 0,
                         "%s: %s : queue %d/%d\n", f->name, f->cls->name, QUEUED(f), f->max_len);
            if (QUEUED(f) < f->max_len) {
                logmm_printl(fader_log, 2, "_fader_process_data_running", 0,
                             "%s: %s : blocked.. goto sleep\n", f->name, f->cls->name);
                aud_cond_wait(&f->block_cond, &f->block_lock);
                logmm_printl(fader_log, 2, "_fader_process_data_running", 0,
                             "%s: %s : awaken from block!!\n", f->name, f->cls->name);
            }
        }
        aud_mutex_unlock(&f->block_lock);
        aud_mutex_lock(&f->lock);
    }

    if (QUEUED(f) - f->fade_len <= f->stop_len) {               /* 0x600851d5 */
        logmm_printlm(f->name, 3, "_fader_process_data_running", 0, "yield before fadeout.. queued %d\n", QUEUED(f));
        aud_cond_wait_timeout(&f->out_cond, &f->lock, 10);
        logmm_printlm(f->name, 3, "_fader_process_data_running", 0, "done queued %d\n", QUEUED(f));
        if (QUEUED(f) - f->fade_len <= f->stop_len) {
            if (f->fade_len == 0) {
                if (f->debug & 8)
                    logmm_printlm(f->name, 3, "_fader_process_data_running", 0x24b,
                                  "waiting.. queued %d\n", QUEUED(f));
                set_state(f, FS_WAIT, _fader_process_data_wait);
            } else {
                logmm_printlm(f->name, 3, "_fader_process_data_running", 0, "fadeout.. queued %d\n", QUEUED(f));
                set_state(f, FS_FADEOUT, _fader_process_data_fadeout);
            }
        }
    }
    if (f->send_underflow) {                                    /* [FLIX?] @0x60085237 */
        logmm_printlm(f->name, 2, "_fader_process_data_running", 0x255,
                      "raise underflow event: uid=0x%X\n", f->uid);
        aimc_raise_named_event(f->aimc, f->uid, "underflow", &f->uid, 4, f->send_underflow);
        f->send_underflow = 0;
    }
    return 0;
}

/* Fade kernels: one 32-bit sample = sample / fade_len * fade_pos. 10ch uses 40-byte frames. */
static void fade_frames(struct fader *f, int32_t *dst, const int32_t *src, int frames, int ch,
                        int dir)
{
    for (int i = 0; i < frames; i++) {
        for (int c = 0; c < ch; c++)
            dst[c] = f->fade_pos * (src[c] / f->fade_len);
        dst += ch;
        src += ch;
        if (dir < 0 && f->fade_pos > 0)
            f->fade_pos--;
        if (dir > 0 && f->fade_pos < f->fade_len)
            f->fade_pos++;
    }
}

/* @0x60085424 */
void fadeout_32bit_2ch(struct fader *f, int32_t *d, const int32_t *s, int n) { fade_frames(f, d, s, n, 2, -1); }
/* @0x60085464 fadeout_32bit_4ch (functions.tsv: "fader_cmd") */
void fadeout_32bit_4ch(struct fader *f, int32_t *d, const int32_t *s, int n) { fade_frames(f, d, s, n, 4, -1); }
/* @0x600854c4 */
void fadeout_32bit_8ch(struct fader *f, int32_t *d, const int32_t *s, int n) { fade_frames(f, d, s, n, 8, -1); }
/* @0x60085564: 10 slots per frame, only the first 4 are faded */
void fadeout_32bit_10ch(struct fader *f, int32_t *d, const int32_t *s, int n)
{
    for (int i = 0; i < n; i++, d += 10, s += 10) {
        for (int c = 0; c < 4; c++)
            d[c] = f->fade_pos * (s[c] / f->fade_len);
        if (f->fade_pos > 0)
            f->fade_pos--;
    }
}

/* @0x600855e8 fadeout_bypass (functions.tsv: "fader_event_first"): ES / non-PCM, copy only */
void fadeout_bypass(struct fader *f, int32_t *d, const int32_t *s, int n)
{
    memcpy(d, s, (size_t)f->frame_bytes * (size_t)n);
    f->fade_pos -= n;
    if (f->fade_pos < 0)
        f->fade_pos = 0;
}

/* @0x60085610 _fader_process_data_fadeout (functions.tsv: "underflow") */
int _fader_process_data_fadeout(struct fader *f)
{
    struct aud_data *d = aud_data_new(f->chunk_bytes);
    unsigned n;

    if (f->debug & 0x10)
        logmm_printlm(f->name, 4, "_fader_process_data_fadeout", 0x2c9, "fadeout.. %d\n", f->fade_pos);
    if (!d) {
        logmm_printlm(f->name, 0, "_fader_process_data_fadeout", 0x2cf, "no mem..");
        return -1;
    }
    n = QUEUED_BYTES(f) < f->chunk_bytes ? QUEUED_BYTES(f) : f->chunk_bytes;
    if (n == 0) {
        logmm_printlm(f->name, 0, "_fader_process_data_fadeout", 0x2e3, "no fadeout data??\n");
        memset(d->ptr, 0, d->size);
    } else {
        void *src = aud_data_adapter_map(&f->q, n);
        f->fadeout(f, d->ptr, src, (int)n / f->frame_bytes);
        aud_data_adapter_unmap(&f->q, src);
        aud_data_adapter_flush(&f->q, n);
        pe_shift(f, (int)n);
    }
    if (QUEUED(f) <= f->max_len)                                /* 0x600856e2 */
        aud_cond_signal(&f->in_cond);
    if (n < f->chunk_bytes)                                     /* 0x600856ed */
        memset((unsigned char *)d->ptr + n, 0, f->chunk_bytes - n);
    aud_mutex_unlock(&f->lock);
    _fader_write_metadata(f, (int)(d->size / (unsigned)f->frame_bytes), 1, d->ts);
    _fader_push_data(f, d);
    aud_mutex_lock(&f->lock);
    if (f->dropped_frames) {
        logmm_printl(fader_log, 3, "_fader_process_data_fadeout", 0x2f6,
                     "%s: free lost %d samples of metadata...\n", f->name, f->dropped_frames);
        msf_update_read_buf(&f->md_in, f->dropped_frames);
        f->dropped_frames = 0;
    }
    if (f->fade_pos != 0)
        return 0;

    /* Fade finished: one zero chunk, two for Dolby (0x60085626/0x6008574a). */
    for (int i = 0, extra = f->dolby ? 2 : 1; i < extra; i++) {
        struct aud_data *z = aud_data_new_zero(f->chunk_bytes);
        if (!z) {
            if (f->debug & 1)
                logmm_printlm(f->name, 0, "_fader_process_data_fadeout", 0x30d, "no mem..");
            continue;
        }
        aud_mutex_unlock(&f->lock);
        _fader_push_data(f, z);
        aud_mutex_lock(&f->lock);
        if (f->debug & 4)
            logmm_printlm(f->name, 2, "_fader_process_data_fadeout", 0x315, "send zero data..");
    }
    if (f->debug & 8)
        logmm_printlm(f->name, 3, "_fader_process_data_fadeout", 0x318, "waiting..\n");
    set_state(f, FS_WAIT, _fader_process_data_wait);
    return 0;
}

/* @0x60085908 */
void fadein_32bit_2ch(struct fader *f, int32_t *d, const int32_t *s, int n) { fade_frames(f, d, s, n, 2, +1); }
/* @0x60085948 */
void fadein_32bit_4ch(struct fader *f, int32_t *d, const int32_t *s, int n) { fade_frames(f, d, s, n, 4, +1); }
/* @0x600859ac */
void fadein_32bit_8ch(struct fader *f, int32_t *d, const int32_t *s, int n) { fade_frames(f, d, s, n, 8, +1); }
/* @0x60085a50 */
void fadein_32bit_10ch(struct fader *f, int32_t *d, const int32_t *s, int n)
{
    for (int i = 0; i < n; i++, d += 10, s += 10) {
        for (int c = 0; c < 4; c++)
            d[c] = f->fade_pos * (s[c] / f->fade_len);
        if (f->fade_pos < f->fade_len)
            f->fade_pos++;
    }
}

/* @0x60085ad8: ES / non-PCM fade-in is a plain copy */
void fadein_bypass(struct fader *f, int32_t *d, const int32_t *s, int n)
{
    memcpy(d, s, (size_t)f->frame_bytes * (size_t)n);
    f->fade_pos += n;
    if (f->fade_pos > f->fade_len)
        f->fade_pos = f->fade_len;
}

/* @0x60085b00 */
int _fader_process_data_fadein(struct fader *f)
{
    struct aud_data *d = aud_data_new(f->chunk_bytes);
    unsigned n;

    if (!d) {
        logmm_printlm(f->name, 0, "_fader_process_data_fadein", 0, "no mem..");
        return -1;
    }
    n = QUEUED_BYTES(f) < f->chunk_bytes ? QUEUED_BYTES(f) : f->chunk_bytes;
    if (n) {
        void *src = aud_data_adapter_map(&f->q, n);
        f->fadein(f, d->ptr, src, (int)n / f->frame_bytes);
        aud_data_adapter_unmap(&f->q, src);
        aud_data_adapter_flush(&f->q, n);
        pe_shift(f, (int)n);
    }
    if (QUEUED(f) <= f->max_len)                                /* 0x60085bb5 */
        aud_cond_signal(&f->in_cond);
    if (n < f->chunk_bytes)                                     /* 0x60085bc0 */
        memset((unsigned char *)d->ptr + n, 0, f->chunk_bytes - n);
    aud_mutex_unlock(&f->lock);
    _fader_write_metadata(f, (int)(d->size / (unsigned)f->frame_bytes), 1, d->ts);
    _fader_push_data(f, d);
    aud_mutex_lock(&f->lock);
    if (QUEUED(f) <= f->stop_len)
        set_state(f, FS_WAIT, _fader_process_data_wait);
    else if (f->fade_len == f->fade_pos)
        set_state(f, FS_RUNNING, _fader_process_data_running);
    return 0;
}

/* @0x60085cdc: queue `frames` zero frames in front (start-up padding). */
int add_zero(struct fader *f, int frames)
{
    if (f->debug & 8)
        logmm_printlm(f->name, 3, "add_zero", 0x3c5, "max %d, queued %d, zero %d",
                      f->max_len, QUEUED(f), frames);
    if (frames < 1 || f->fade_len < 1)
        return -1;
    f->zero_left = frames;
    do {
        int n = f->fade_len < frames ? f->fade_len : frames;
        aud_data_adapter_push_front(&f->q, aud_data_new_zero((unsigned)(f->frame_bytes * n)));
        frames -= n;
    } while (frames > 0);
    return 0;
}

/* @0x60085d50: wait for MORE than wait-length frames, then send the held FIRST and
 * the front padding. Only fader_chain (crossing wait-length), FIRST, CUSTOM/FLUSH
 * (_fader_add_pe_and_signal) and fader_stop signal out_cond. Nothing downstream does.
 * Queue variants never send LAST here and never time out into "underflow_input".
 * @0x60085e38/60085e09: a pending wake checks ready events, then rechecks the same
 * strict threshold; CUSTOM behind held audio cannot release WAIT, even after LAST. */
int _fader_process_data_wait(struct fader *f)
{
    bool first_wait = f->first_sent;

    if (f->first_sent && !is_queue_variant(f)) {                /* 0x60085d8c/0x60085d9d */
        if (f->dot_on_wait > 0 && --f->dot_on_wait == 0)
            dot_module_connection(0, 0);
        aud_mutex_unlock(&f->lock);
        aud_module_push_event(f, 0, aud_event_new_last());
        aud_module_sync(f, 0);
        aud_mutex_lock(&f->lock);
        f->first_sent = false;
    }

    for (;;) {
        if (f->state != FS_WAIT) {                              /* 0x60085df4 */
            f->enable_underflow = true;
            return -1;
        }
        if (QUEUED(f) > f->wait_len)                            /* 0x60085e09 */
            break;
        if (first_wait && f->enable_underflow) {
            if (aud_cond_wait_timeout(&f->out_cond, &f->lock, WAIT_TIMEOUT_US) == 1 &&
                f->enable_underflow && !f->low_delay && !f->bypass && !f->flushing &&
                !is_queue_variant(f)) {
                struct aud_event *e;
                aimc_raise_named_event(f->aimc, f->uid, "underflow_input", &f->uid, 4);
                f->underflow_count++;
                e = aud_event_newv_custom("underflow_input", 1, 1, 0);
                if (!e)
                    return -1;
                logmm_printl(fader_log, 2, "_fader_process_data_wait", 0x428,
                             "%s: push custom event (input underflow)", f->name);
                aud_mutex_unlock(&f->lock);
                aud_module_push_event(f, 1, e);
                aud_module_sync(f, 1);
                aud_mutex_lock(&f->lock);
            }
            first_wait = false;
        } else {
            aud_cond_wait(&f->out_cond, &f->lock);
        }
        if (f->pe_signal)
            pending_events_check(f);
    }

    f->enable_underflow = true;
    if (!f->first_sent) {
        struct aud_event *e = aud_object_get(f->first);
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "_fader_process_data_wait", 0x44e, "send first event..");
        aud_mutex_unlock(&f->lock);
        aud_module_push_event(f, 0, e);
        aud_module_sync(f, 0);
        aud_mutex_lock(&f->lock);
        f->first_sent = true;
    }
    if (f->fade_len == 0) {
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "_fader_process_data_wait", 0x463, "running..");
        set_state(f, FS_RUNNING, _fader_process_data_running);
    } else {
        f->fade_pos = 0;
        set_state(f, FS_FADEIN, _fader_process_data_fadein);
    }
    if (f->zero_left == 0)
        add_zero(f, f->padding_len);
    if (QUEUED_BYTES(f) == 0) {
        logmm_printlm(f->name, 0, "_fader_process_data_wait", 0x484, "?? zero %d", f->zero_left);
        return 0;
    }
    while (f->zero_left > 0) {                                  /* 0x60085fb8..0x60086063 */
        struct aud_data *d = aud_data_adapter_dequeue(&f->q);
        if (!d)
            break;
        pe_shift(f, (int)d->size);
        f->zero_left -= (int)(d->size / (unsigned)f->frame_bytes);
        aud_cond_signal(&f->in_cond);
        aud_mutex_unlock(&f->lock);
        _fader_write_metadata(f, (int)(d->size / (unsigned)f->frame_bytes), 0, d->ts);
        _fader_push_data(f, d);
        aud_mutex_lock(&f->lock);
    }
    return 0;
}

/* @0x60086200 */
int process_data_stop(struct fader *f)
{
    aud_cond_wait(&f->out_cond, &f->lock);
    return 0;
}

/* @0x60086218: output thread body (called with f->lock held) */
int fader_output_work(void *thread, struct fader *f)
{
    (void)thread;
    if (f->state != FS_STOP && f->state != FS_WAIT && QUEUED_BYTES(f) == 0) {
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "fader_output_work", 0x49e, "empty.. current state %s",
                          state_name(f->state));
        set_state(f, FS_WAIT, _fader_process_data_wait);
    }
    f->process(f);
    f->zero_left = 0;
    pending_events_check(f);
    return 0;
}

/* Inlined "queue_module_unblock_locked" (__func__ 0x6005f134, line 0x4af). */
static void queue_module_unblock_locked(struct fader *f)
{
    if (fader_log_mask & 4)
        logmm_printl(fader_log, 2, "queue_module_unblock_locked", 0x4af,
                     "%s: trying to unblock module %s\n", f->name, f->cls->name);
    f->block = false;
    aud_cond_signal(&f->block_cond);
}

/* @0x600862a8 */
void queue_module_unblock(struct fader *f)
{
    aud_mutex_lock(&f->block_lock);
    if (f->block)
        queue_module_unblock_locked(f);
    aud_mutex_unlock(&f->block_lock);
}

struct aud_prop_desc { const char *name; };

/* @0x60086308: "block-queue" property setter */
int set_block_queue(struct fader *f, const struct aud_prop_desc *p, const char *v)
{
    bool nv = !strcmp(v, "true") || !strcmp(v, "yes") || !strcmp(v, "1");
    bool old;

    aud_mutex_lock(&f->block_lock);
    old = f->block;
    if (old == nv) {
        if (fader_log_mask & 4)
            logmm_printl(fader_log, 2, "set_block_queue", 0x4cb,
                         "%s: setting same value. ignore. %s: %s\n", f->name, p->name,
                         nv ? "true" : "false");
    } else {
        f->block = nv;
        if (fader_log_mask & 4)
            logmm_printl(fader_log, 2, "set_block_queue", 0x4d2, "%s: %s: set block as %s\n",
                         f->name, f->cls->name, nv ? "true" : "false");
        if (old && !nv)
            queue_module_unblock_locked(f);
    }
    aud_mutex_unlock(&f->block_lock);
    return 0;
}

/* @0x60086404 */
int fader_start(struct fader *f)
{
    int r;
    if (f->output_thread) {
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "fader_start", 0x4e4, "already running..");
        return 0;
    }
    if (f->debug & 8)
        logmm_printlm(f->name, 3, "fader_start", 0x4e8, "start thread..");
    set_state(f, FS_WAIT, _fader_process_data_wait);
    r = aud_module_output_thread_create(f, (void *)fader_output_work, f, &f->lock, &f->out_cond);
    if (r < 0 && (f->debug & 1))
        logmm_printlm(f->name, 0, "fader_start", 0x4ef, "can't create thread");
    return r;
}

/* @0x60086494 */
int fader_stop(struct fader *f)
{
    if (!f->output_thread) {
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "fader_stop", 0x4f8, "no output thread");
        return 0;
    }
    aud_mutex_lock(&f->lock);
    if (f->debug & 8)
        logmm_printlm(f->name, 3, "fader_stop", 0x4fe, "stop output..");
    set_state(f, FS_STOP, process_data_stop);
    aud_cond_signal(&f->out_cond);
    aud_mutex_unlock(&f->lock);
    aud_module_output_thread_destroy(f);
    queue_module_unblock(f);
    return 0;
}

/* @0x60086514: bypass watchdog. When no bypass input came for 50 ms it sends LAST_AU. */
int fader_bypass_work(struct fader *f)
{
    while (f->bypass_run) {
        aud_mutex_lock(&f->bypass_lock);
        if (f->last_input > 1) {                                /* [FLIX?] @0x600865d9 */
            /* [FLIX] 0x60086689..0x600866a4: diff = now - last_input in us (__udivdi3) */
            uint64_t diff_us = 0;
            if (diff_us > 50000) {                              /* 0x6008652c: 0xc350 */
                logmm_printlm(f->name, 2, "fader_bypass_work", 0x521,
                              "no input more than 50 ms (%llu us). send last event\n",
                              (unsigned long long)diff_us);
                aud_module_push_event(f, 0, aud_event_new_last());
                f->last_input = 1;
                logmm_printlm(f->name, 4, "fader_bypass_work", 0x527, "sleep...\n");
                aud_cond_wait(&f->bypass_cond, &f->bypass_lock);
            }
        }
        aud_cond_wait_timeout(&f->bypass_cond, &f->bypass_lock, BYPASS_IDLE_US);
        aud_mutex_unlock(&f->bypass_lock);
    }
    return 0;
}

/* @0x600866e4 */
int fader_bypass_start(struct fader *f)
{
    aud_mutex_lock(&f->bypass_lock);
    if (f->bypass_thread) {
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "fader_bypass_start", 0x53b, "already bypass thread exist..");
    } else {
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "fader_bypass_start", 0x53f, "start bypass thread..");
        f->bypass_run = true;
        f->bypass_thread = aud_thread_create("fader_bypass", 0x8000, 5, (void *)fader_bypass_work, f);
        if (!f->bypass_thread && (f->debug & 1))
            logmm_printlm(f->name, 0, "fader_bypass_start", 0x545, "can't create thread");
    }
    aud_mutex_unlock(&f->bypass_lock);
    return 0;
}

/* Inlined fader_bypass_stop (__func__ 0x6005f1c8). */
static void fader_bypass_stop(struct fader *f)
{
    aud_mutex_lock(&f->bypass_lock);
    if (!f->bypass_thread) {
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "fader_bypass_stop", 0x552, "no bypass thread\n");
        aud_mutex_unlock(&f->bypass_lock);
        return;
    }
    f->bypass_run = false;
    aud_cond_signal(&f->bypass_cond);
    aud_mutex_unlock(&f->bypass_lock);
    aud_thread_destroy(f->bypass_thread);
    f->bypass_thread = NULL;
}

/* ------------------------------------------------------------------------
 * Module callbacks
 * ---------------------------------------------------------------------- */
struct aud_port {
    struct fader *module;        /* +0x04 */
    uint32_t      flag;          /* +0x08 bit1 DISABLED */
};

/* @0x60086778: input side */
int fader_chain(struct aud_port *port, struct aud_data *d)
{
    struct fader *f = port->module;
    int r;

    if (fader_trace_enabled(f->cls->name, 1))
        logm_message("[FADER] IN  id=%5d, sz=%d, ts=%10u\n", d->index, d->size, d->ts);

    if (f->bypass != f->bypass_req && !f->low_delay) {          /* 0x600867a4 */
        f->bypass = f->bypass_req;
        if (f->debug & 4)
            logmm_printlm(f->name, 2, "fader_chain", 0x572, "set bypass %s\n",
                          f->bypass ? "True" : "False");
        if (!f->bypass) {
            aud_cond_signal(&f->out_cond);
            fader_bypass_stop(f);
            fader_start(f);
        } else {
            fader_stop(f);
            pending_events_empty(f);
            aud_mutex_lock(&f->lock);
            aud_data_adapter_flush(&f->q, QUEUED_BYTES(f));
            aud_mutex_unlock(&f->lock);
            fader_bypass_start(f);
        }
    }

    if (f->low_delay || f->bypass) {
        /* Low-delay / bypass: no queue at all. First event goes out with the first data. */
        if (f->debug & 0x10)
            logmm_printlm(f->name, 4, "fader_chain", 0x588, "push\n");
        if (f->last_input == 1) {           /* watchdog sent LAST: re-open with FIRST */
            if (f->debug & 8)
                logmm_printlm(f->name, 3, "fader_chain", 0x58c, "send first event..");
            aud_module_push_event(f, 0, aud_object_get(f->first));
            aud_module_sync(f, 0);
        }
        aud_mutex_lock(&f->bypass_lock);
        f->last_input = aud_system_cycle();
        aud_cond_signal(&f->bypass_cond);
        aud_mutex_unlock(&f->bypass_lock);
        return aud_module_push_data(f, 0, d);
    }

    if (d->type == AUD_DATA_META) {                             /* [FLIX?] @0x600868bd */
        r = msf_process_metadata(&f->md_in, d->ptr, d->size);
        if (r && (f->debug & 1))
            logmm_printlm(f->name, 0, "fader_chain", 0x5a0, "msf_process_metadata failed");
        aud_object_put(d);
        return r;
    }
    if (d->type == AUD_DATA_META_EXT) {                         /* [FLIX?] @0x600868c8 */
        if (f->meta_ext) {
            logmm_printl(fader_log, 3, "fader_chain", 0x5b2,
                         "%s: meta ext is already exist.. so put and get\n", f->name);
            aud_object_put(f->meta_ext);
        }
        f->meta_ext = aud_object_get(d);
        aud_object_put(d);
        return 0;
    }

    if (f->debug & 0x20)
        logmm_printlm(f->name, 5, "fader_chain", 0x5bc, "chain. length %d(size %d)\n",
                      (int)(d->size / (unsigned)f->frame_bytes), d->size);
    aud_mutex_lock(&f->lock);
    /* 0x600868fa..0x60086925: back-pressure. The upstream thread blocks while the queue
     * holds MORE than max-length frames, unless the port is disabled or a flush is running.
     * Only the output thread's dequeue/fade (<= max-length), FLUSH and disconnect wake it. */
    while (!(port->flag & PORT_DISABLED) && QUEUED(f) > f->max_len && !f->flushing) {
        if (f->debug & 0x20)
            logmm_printlm(f->name, 5, "fader_chain", 0x5c1, "queue full. wait.. (%d/%d)\n",
                          QUEUED(f), f->max_len);
        aud_cond_wait(&f->in_cond, &f->lock);
    }

    aud_mutex_lock(&f->md_lock);
    if (f->set_flush) {                                         /* 0x60086966 */
        if (QUEUED_BYTES(f))                                    /* 0x60086980 */
            aud_data_adapter_flush(&f->q, QUEUED_BYTES(f));
        msf_flush_metadata(&f->md_in);
        f->set_flush = false;
    }
    aud_mutex_unlock(&f->md_lock);

    if ((port->flag & PORT_DISABLED) || f->flushing) {
        /* 0x600869ab: DROP. Disconnected port or async FLUSH in progress. */
        f->dropped_frames += (int)(d->size / (unsigned)f->frame_bytes);
        aud_object_put(d);
        r = -1;
    } else {
        int before = QUEUED(f);
        aud_data_adapter_push(&f->q, d);
        if ((unsigned)QUEUED(f) > f->max_queued_len)
            f->max_queued_len = (unsigned)QUEUED(f);
        if (before <= f->wait_len && QUEUED(f) > f->wait_len)
            aud_cond_signal(&f->out_cond);
        r = 0;
    }
    if (f->debug & 0x10)
        logmm_printlm(f->name, 4, "fader_chain", 0x5e9, "queued length %d(%d)\n",
                      QUEUED(f), (int)QUEUED_BYTES(f));
    aud_mutex_unlock(&f->lock);
    if (is_queue_variant(f))                                    /* 0x600869d9 -> 0x60086b88 */
        xos_thread_yield();
    return r;
}

/* @0x60086bc8: like pending_events_add but under the main lock and waking the output. */
int _fader_add_pe_and_signal(struct fader *f, struct aud_event *e)
{
    aud_mutex_lock(&f->lock);
    aud_mutex_lock(&f->pe_lock);
    pe_add_locked(f, e);
    aud_mutex_unlock(&f->pe_lock);
    f->pe_signal = true;
    aud_cond_signal(&f->out_cond);
    aud_mutex_unlock(&f->lock);
    return 0;
}

/* FIRST_AU part of fader_event (__func__ "fader_event_first", 0x6005f210). */
static int fader_event_first(struct fader *f, void *port, struct aud_event *e)
{
    const struct fader_desc *desc = f->cls->desc;

    aud_mutex_lock(&f->lock);
    msf_flush_metadata(&f->md_in);
    if (desc->is_queue) {
        int size = aud_event_value_get_int(e, "queue-size", 0);
        int count = aud_event_value_get_int(e, "queue-count", 1);
        aud_event_value_del(e, "queue-size");
        aud_event_value_del(e, "queue-count");
        if (size) {
            f->max_len = size * count;
            f->wait_len = size * (count - 1);
        }
        f->low_delay = false;
    } else {
        f->low_delay = aud_event_value_get_int(e, "low-delay", 0) != 0;
    }
    f->es = aud_event_value_get_bool(e, "es", false);
    f->channels = aud_event_value_get_int(e, "channels", 8);
    f->bits = aud_event_value_get_int(e, "bits-per-sample", 32);
    if (f->channels > 8 || f->channels < 1) {
        int fixed = f->es ? 2 : 8;
        logmm_printlm(f->name, 3, "fader_event_first", 0, "channels is not right.. %d -> %d\n",
                      f->channels, fixed);
        f->channels = fixed;
    }
    if (f->bits == 0) {
        int fixed = f->es ? 16 : 32;
        logmm_printlm(f->name, 3, "fader_event_first", 0, "bps is not right.. %d -> %d\n", 0, fixed);
        f->bits = fixed;
    }
    f->frame_bytes = f->channels * f->bits / 8;
    f->padding_len = f->max_len - f->stop_len;      /* 0x60086f4a */
    f->chunk_bytes = (unsigned)(f->chunk_len * f->frame_bytes);
    f->dolby = aud_event_value_get_bool(e, "dolby", false);
    if (!desc->is_queue)
        f->padding_len += aud_event_value_get_int(e, "front-padding", 0);

    if (f->es) {
        /* ES: fades are byte copies. No PCM scaling on a bitstream. */
        f->fadein = fadein_bypass;
        f->fadeout = fadeout_bypass;
    } else if (f->bits == 32) {
        switch (f->channels) {
        case 2:  f->fadein = fadein_32bit_2ch;  f->fadeout = fadeout_32bit_2ch;  break;
        case 4:  f->fadein = fadein_32bit_4ch;  f->fadeout = fadeout_32bit_4ch;  break;
        case 8:  f->fadein = fadein_32bit_8ch;  f->fadeout = fadeout_32bit_8ch;  break;
        case 10: f->fadein = fadein_32bit_10ch; f->fadeout = fadeout_32bit_10ch; break;
        default:
            logmm_printlm(f->name, 3, "fader_event_first", 0, "not implemented channel. %d\n", f->channels);
            f->fadein = fadein_bypass;
            f->fadeout = fadeout_bypass;
        }
    } else {
        logmm_printlm(f->name, 3, "fader_event_first", 0, "not implemented bits-per-sample. %d\n", f->bits);
        f->fadein = fadein_bypass;
        f->fadeout = fadeout_bypass;
    }
    if (f->debug & 8)
        logmm_printlm(f->name, 3, "fader_event_first", 0x659, "start %s%dch, %dbpp, ch_x_bps %d\n",
                      f->es ? "es, " : "", f->channels, f->bits, f->frame_bytes);

    if (f->first) {
        aud_object_put(f->first);
        f->first = NULL;
    }
    f->enable_underflow = true;                                  /* 0x60086fd2 */
    if (f->bypass != f->bypass_req) {
        f->bypass = f->bypass_req;
        if (f->debug & 4)
            logmm_printlm(f->name, 2, "fader_event_first", 0x665, "set bypass %s\n",
                          f->bypass ? "True" : "False");
    }
    if (f->low_delay || f->bypass) {
        aud_mutex_unlock(&f->lock);
        fader_stop(f);
        pending_events_empty(f);
        fader_bypass_start(f);
        if (f->bypass)
            f->first = aud_event_dup(e);
        return aud_module_default_event(f, port, e);
    }

    /* Queued path (0x6008711c). Idle (WAIT): keep the event; the wait state sends it
     * when wait-length frames are queued. Mid-stream: pad the queue to max-length with
     * zeros and queue the event behind the data already queued. */
    f->first = e;
    f->low_delay = false;
    if (f->state == FS_WAIT) {
        f->first_sent = false;
        if (f->zero_left == 0)
            add_zero(f, f->padding_len);
    } else {
        add_zero(f, f->max_len - QUEUED(f));
        pending_events_add(f, aud_object_get(e));
    }
    aud_cond_signal(&f->out_cond);
    aud_mutex_unlock(&f->lock);
    fader_start(f);
    return 0;
}

/* @0x60086d0c */
int fader_event(struct fader *f, void *port, struct aud_event *e)
{
    switch (e->type) {
    case AUD_EVENT_FIRST_AU:                                    /* [FLIX?] @0x60086d16 */
        return fader_event_first(f, port, e);

    case AUD_EVENT_LAST_AU:
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "fader_event", 0x6a1, "last\n");
        aud_mutex_lock(&f->lock);
        f->enable_underflow = false;
        aud_mutex_unlock(&f->lock);
        if (!f->low_delay && !f->bypass) {
            /* @0x60086d6c/600870be: no wake, padding or pending LAST. Only a normal
             * fader regenerates LAST on entering WAIT; queues consume it outright. */
            aud_object_put(e);
            return 0;
        }
        return aud_module_default_event(f, port, e);

    case AUD_EVENT_FLUSH:
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "fader_event", 0x6b1, "%s", aud_event_name(e));
        aud_mutex_lock(&f->lock);
        /* Async flush (no SYNC prop) turns on input dropping in fader_chain. */
        f->flushing = !(e->prop & AUD_EVENT_PROP_SYNC);
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "fader_event", 0x6b9, "state: %s disable underflow event\n",
                          state_name(f->state));
        f->enable_underflow = false;
        aud_cond_signal(&f->in_cond);
        aud_mutex_unlock(&f->lock);
        queue_module_unblock(f);
        if (is_queue_variant(f)) {
            if (!(e->prop & AUD_EVENT_PROP_SYNC))               /* 0x60086e1b -> 0x60087098 */
                return aud_module_default_event(f, port, e);
            logmm_printl(fader_log, 3, "fader_event", 0x6c5, "%s: set_flush = true\n", f->name);
            f->set_flush = true;
            return _fader_add_pe_and_signal(f, e);
        }
        aud_object_put(e);
        return 0;

    case AUD_EVENT_CUSTOM:                                      /* beq.w18 @0x60086d2c -> 60086e41 */
        logmm_printl(fader_log, 3, "fader_event", 0x6d4, "%s: got CUSTOM EVENT!!!!", f->name);
        return _fader_add_pe_and_signal(f, e);

    case AUD_EVENT_PASSING:                                     /* [FLIX?] @0x60086d36 */
        aud_event_value_set_int(e, "passing-length",
                                aud_event_value_get_int(e, "passing-length", 0) + QUEUED(f));
        return aud_module_default_event(f, port, e);

    default:
        return aud_module_default_event(f, port, e);
    }
}

/* @0x60087288: delay command sets wait/stop/max from fs*delay_ms */
int fader_cmd(struct fader *f, uint32_t cmd, int32_t *arg, unsigned len)
{
    int wait, prod;

    if (cmd != CMD_SET_DELAY_A && cmd != CMD_SET_DELAY_B) {
        /* [FLIX?] @0x60087290/0x60087298: 0x10010000/0x10020000 return 0 first */
        return aud_module_default_command(f, cmd, arg, len);
    }
    if (f->debug & 8)
        logmm_printlm(f->name, 3, "fader_cmd", 0x6f6, "fs %dHz, delay %dms", arg[1], arg[0]);
    prod = arg[0] * arg[1];
    wait = prod > 0x3e7ff ? prod / 1000 : 0x100;
    f->wait_len = wait;
    f->stop_len = wait - f->chunk_len > 0 ? wait - f->chunk_len : 0;
    f->max_len = wait + f->chunk_len;
    if (f->debug & 8)
        logmm_printlm(f->name, 3, "fader_cmd", 0x701, "stop %d, wait %d, max %d",
                      f->stop_len, f->wait_len, f->max_len);
    return 0;
}

struct aud_in_port {
    int      index;              /* +0x00 */
    uint32_t pad;
    uint32_t flag;               /* +0x08 */
};

/* @0x60087360 */
int fader_disconnect_port(struct fader *f, struct aud_in_port *p)
{
    if (f->debug & 8)
        logmm_printlm(f->name, 3, "fader_disconnect_port", 0x713, "disconnecting input port%d..", p->index);
    if (f->low_delay || f->bypass) {
        if (f->debug & 8)
            logmm_printlm(f->name, 3, "fader_disconnect_port", 0x717, "low delay.. propagate event..");
        return -1;
    }
    aud_mutex_lock(&f->lock);
    p->flag |= PORT_DISABLED;
    f->flushing = 0;
    f->enable_underflow = false;
    aud_cond_signal(&f->in_cond);
    aud_mutex_unlock(&f->lock);
    queue_module_unblock(f);
    if (!is_queue_variant(f))
        return 0;
    if (f->debug & 8)
        logmm_printlm(f->name, 3, "fader_disconnect_port", 0x728, "queue.. propagate event..");
    return -1;
}

/* @0x60087410 */
int fader_deleting_port(struct fader *f, struct aud_in_port *p)
{
    if ((p->flag & 1) && (f->debug & 8))
        logmm_printlm(f->name, 3, "fader_deleting_port", 0x737, "disable output port%d..", p->index);
    return 0;
}

/* @0x6008743c */
void fader_constructor(struct fader *f)
{
    const struct fader_desc *desc = f->cls->desc;

    aud_data_adapter_init(&f->q);
    f->wait_len = desc->wait_len;
    f->stop_len = desc->stop_len;
    f->max_len = desc->max_len;
    f->fade_len = desc->fade_len;
    f->chunk_len = desc->chunk_len;
    f->pe.prev = f->pe.next = &f->pe;
    /* [FLIX?] @0x60087474: padding only for the "fader" variant */
    f->padding_len = desc->max_len - desc->stop_len;
    f->frame_bytes = 32;
    f->bits = 32;
    f->channels = 8;
    f->chunk_bytes = (unsigned)f->chunk_len << 5;
    f->state = FS_WAIT;
    f->dropped_frames = 0;
    f->queued_len = 0;
    f->max_queued_len = 0;
    f->set_flush = false;
    f->dolby = false;
    f->bypass = false;
    f->bypass_req = false;
    f->enable_underflow = true;
    aud_mutex_init(&f->lock);
    aud_mutex_init(&f->md_lock);
    aud_mutex_init(&f->pe_lock);
    aud_cond_init(&f->in_cond);
    aud_cond_init(&f->out_cond);
    aud_cond_init(&f->block_cond);
    aud_mutex_init(&f->block_lock);
    /* +0x1a4/+0x1c0 msf reader/writer list heads; +0x1ac=0x1000, +0x1b4=0xc00,
     * +0x1b8=0x6800 (metadata buffer limits) */
    aud_mutex_init(&f->bypass_lock);
    aud_cond_init(&f->bypass_cond);
    f->last_input = 0;
}

/* @0x60087540 */
void fader_destructor(struct fader *f)
{
    fader_stop(f);                                   /* inlined copy in the binary */
    aud_data_adapter_cleanup(&f->q);
    pending_events_empty(f);                         /* inlined copy */
    if (f->first)
        aud_object_put(f->first);
    msf_flush_metadata(&f->md_in);
}

/* @0x60087650 */
int fader_prop_geti_queued_length(struct fader *f)
{
    f->queued_len = QUEUED(f);
    return f->queued_len;
}

/* @0x6008766c */
char *fader_prop_gets_queued_length(struct fader *f)
{
    f->queued_len = QUEUED(f);
    return audutil_strdup_printf("(int)%d", f->queued_len);
}

/* @0x60087690 */
char *fader_prop_gets_state(struct fader *f)
{
    return strdup(state_name(f->state));
}

/* @0x600876d0: "underflow" property setter */
int send_underflow_event(struct fader *f)
{
    struct aud_event *e = aud_event_newv_custom("underflow_input", 1, 1, 0);
    if (!e) {
        if (fader_log_mask & 1)
            logmm_printl(fader_log, 0, "send_underflow_event", 0x7ba, "%s: custom event is NULL", f->name);
        return -1;
    }
    if (fader_log_mask & 4)
        logmm_printl(fader_log, 2, "send_underflow_event", 0x7be,
                     "%s: push custom event (input underflow)", f->name);
    aud_module_push_event(f, 1, e);
    aud_module_sync(f, 1);
    return 0;
}

/* @0x60087740 */
void fader_init(void)
{
    ModuleRegistry_RegisterNamef(&fader_class, &fader_desc_fader, "fader");
    ModuleRegistry_RegisterNamef(&fader_class, &fader_desc_queue, "queue");
    ModuleRegistry_RegisterNamef(&fader_class, &fader_desc_input_queue, "input_queue");
}
