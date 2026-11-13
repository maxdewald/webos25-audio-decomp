/*
 * dsp2 "lipsync" module (class uid 0x0423). The kernel builds two of these per
 * renderer: the PCM lipsync and the ES "lipsync_bypass" on the bypass chain
 * (bypass_appsrc -> queue/fader -> lipsync_bypass -> ... -> delay -> gain -> bypass).
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Linked at 0x60000000. Full listing: build/scratch/evidence/dsp2-lipsync/dsp2-lipsync.asm
 * (0x600a0ed8..0x600a46a0); property table @0x60106300 in
 * build/scratch/evidence/dsp2-lipsync/dsp2-properties.json.
 *
 * FLIX: see module_core.c for the target formula. "[FLIX?] cond" means the branch
 * target is exact but the condition is inferred from context. Log levels: lvl n
 * prints only when bit n of the global lipsync logm word 0x4ff900e8 is set.
 *
 * Summary of what happens to ES (IEC 61937) data here (observed unless noted):
 *  - ES mode (dataType=1) is set by the FIRST event codec != "pcm" (and by the
 *    "dataType" prop the kernel also sends). ES is buffered in the data adapter and
 *    goes through the same pts/stc state machine as PCM, minus fades and metadata.
 *  - Data is DISCARDED (adapter consumed, nothing pushed) in actions SKIP(3) and
 *    IDLE(6); REPEAT(4) holds it; over_protection!=0 or a pending drop count
 *    (+0x140 = discard_param after a state/offset change) drops in write_output.
 *  - Every "state="/"offset="/"BaseCT="/"BaseST=" write resets the action to IDLE
 *    and drop_count to discard_param. With LipsyncOnOff == 1, IDLE actions discard
 *    frames; the escape after idle_time frames is recovered (see _lipsync_state). With
 *    sync off, _lipsync_state returns LIPSYNC_OFF (subject to clock/no-delay paths).
 *  - No output port connected -> all input thrown away ("so throw away..").
 *  - FIRST is held back and only forwarded with the first real output buffer;
 *    LAST is swallowed if nothing was ever output.
 *  - The kernel's "drop_output_data=1/0" (underscores) is not a dsp2 property
 *    name (core uses "drop-output-data"), so it changes nothing (module_core.c).
 */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

enum lipsync_action {            /* names from the table @0x601062c0 */
    LS_OFF = 0,                  /* "LIPSYNC_OFF" */
    LS_RUN = 1,                  /* "LIPSYNC_RUN" */
    LS_NORMAL = 2,
    LS_SKIP = 3,
    LS_REPEAT = 4,
    LS_FREE_RUN = 5,             /* "FR-RUN" */
    LS_IDLE = 6,
    LS_EXCEPT = 7,
    LS_ESNORM = 8,
    LS_TONORM = 9,
    LS_NODELAY = 10,             /* "NODLAY" */
    LS_WRONG = 11,
    LS_FREERUN_WITHNOTI = 12,
    LS_E_NORM = 13,              /* "E-NORM" */
};

#define LIPSYNC_EVT_FIRST_OUTPUT 0xca04u

struct aud_event { int pad[3]; int type; unsigned flags; /* +0x10 */ };
struct aud_data {
    int pad[4];
    int type;                    /* +0x10 0 pcm, 1 es, 2 meta, 3 meta_ext */
    int pad1;
    uint32_t timestamp;          /* +0x18 */
    int pad2[6];
    unsigned size;               /* +0x34 */
    void *buf;                   /* +0x38 */
};
struct aud_port {
    int pad;
    struct lipsync *module;
    unsigned flags;              /* +0x08 bit 1: "port is disabled" (chain @0x600a4049) */
};

struct lipsync {
    const char *name;            /* +0x000 */
    int   uid;                   /* +0x004 */
    unsigned log_mask;           /* +0x008 */
    /* aud_module core up to 0xf0 */
    void *imc;                   /* +0x068 */
    char  adapter[0x10];         /* +0x0f0 aud_data_adapter */
    unsigned adapter_in;         /* +0x100 */
    unsigned adapter_out;        /* +0x104 */
    void *imc_local;             /* +0x10c */
    int   first_sent;            /* +0x110 FIRST forwarded downstream */
    char  msf[0x24];             /* +0x114 metadata ctx */
    unsigned bytes_in;           /* +0x138 consumed bytes */
    unsigned bytes_out;          /* +0x13c bytes for every action except SKIP */
    int   drop_count;            /* +0x140 frames to drop in write_output */
    int   autotest_mode;         /* +0x144 "autotest_mode": fill payload with 0x1f */
    int   repeat_cnt, skip_cnt, cnt150; /* +0x148/+0x14c/+0x150 */
    int   idle_cnt;              /* +0x154 */
    int   last_diff;             /* +0x158 */
    int   action;                /* +0x15c previous action */
    int   frame_bytes, frame_bytes_ch; /* +0x160/+0x164 */
    int   print_interval;        /* +0x168 */
    int   unknown_16c, unknown_170, unknown_174; /* no-delay queue thresholds/scale; no writer in lipsync */
    int   speed_mode;            /* +0x17c speed != 1000 */
    int   speed_in;              /* +0x180 */
    int   is_pcm;                /* +0x184 */
    int   idle_time;             /* +0x188 */
    int   idle_total;            /* +0x18c */
    int   print_cnt;             /* +0x194 frames since last action log (vs print-interval) */
    int   unknown_190;
    int   first_output_sent;     /* +0x198 LIPSYNC_EVT_FIRST_OUTPUT raised */
    int   unknown_19c;
    unsigned debug_count[4];     /* +0x1a0..1ac */
    uint32_t last_stc, last_pts; /* +0x1b0/+0x1b4 */
    int   pcr_change_count;      /* +0x1b8 */
    unsigned unchanged_stc;      /* +0x1bc */
    int   unknown_1c0;
    int32_t diff;               /* +0x1c4 signed 90 kHz clock difference */
    int   discontinuity;         /* +0x1c8 */
    uint32_t repeat_stc;         /* +0x1cc */
    uint32_t out_ts;             /* +0x1d0 */
    int   dataType;              /* +0x1d4 0 pcm, 1 es */
    int   channels, fs, bytePerSample; /* +0x1d8/+0x1dc/+0x1e0 */
    int   discard_param;         /* +0x1e4 */
    int   LipsyncOnOff;          /* +0x1e8 */
    int   upperbound, lowerbound;/* +0x1ec/+0x1f0 */
    int   offset;                /* +0x1f4 "offset"/"ConstantOffset" */
    int   freerunupperbound, freerunlowerbound; /* +0x1f8/+0x1fc ms, ctor 5000 */
    int   clockType;             /* +0x200 0x100 pcr, 0x200 audio master */
    int   BaseCT, BaseST;        /* +0x204/+0x208 */
    int   BaseCT_copy, BaseST_copy; /* +0x20c/+0x210 */
    int   NodelayOnOff;          /* +0x214 */
    int   over_protection;       /* +0x218 */
    int   count_transition;      /* +0x21c */
    int   gap_handling;          /* +0x220 */
    int   start_pts, end_pts;    /* +0x224/+0x228 */
    int   gap_correction;        /* +0x22c */
    bool  offset_changed;        /* +0x230 */
    bool  state_changed;         /* +0x231 also set by BaseCT/BaseST/SET_BASE */
    bool  speed_changed;         /* +0x232 */
    int   additional_bound;      /* +0x234 ms, widens lowerbound */
    struct aud_event *first;     /* +0x238 held FIRST event */
    char  mutex[0x1c];           /* +0x23c */
    char  cond[0x10];            /* +0x258 */
    bool  first_received;        /* +0x268 flush adapter on next chain */
    bool  disconnecting;         /* +0x269 */
    int   nSamplePerFrame;       /* +0x274 */
    int   state;                 /* +0x278 written only */
    bool  flush_async;           /* +0x27c */
    bool  seamless;              /* +0x27d */
    struct aud_data *meta_ext;   /* +0x280 */
};

extern void *aud_object_get(void *);
extern void aud_object_put(void *);
extern int  aud_module_push_data(void *m, int port, struct aud_data *);       /* @0x600724d8 */
extern int  aud_module_push_event(void *m, int port, struct aud_event *);     /* @0x60072b38 */
extern int  aud_module_sync(void *m, int port);                               /* @0x60072bb0 */
extern int  aud_module_have_active_output(void *m);                           /* @0x60073adc */
extern int  aud_module_default_event(void *, struct aud_port *, struct aud_event *);
extern int  aud_module_default_command(void *, unsigned, void *, unsigned);
extern void aimc_raise_named_event(void *imc, int uid, const char *n, void *p, int sz);
extern int  IMC_RaiseEvent(void *imc, unsigned evt, void *dst, void *p);      /* @0x6006d318 */
extern void *IMC_GetLocalImc(int);
extern int  aud_event_value_get_int(struct aud_event *, const char *, int def);
extern const char *aud_event_value_get_str(struct aud_event *, const char *, const char *def);
extern int  aud_event_value_get_bool(struct aud_event *, const char *, int def);
extern int  aud_data_adapter_push(void *, struct aud_data *);                 /* @0x6006b72c */
extern void aud_data_adapter_flush(void *, unsigned);                         /* @0x6006b814 */
extern struct aud_data *aud_data_adapter_get_data(void *, unsigned);          /* @0x6006bbd0 */
extern struct aud_data *aud_data_adapter_peep(void *);                       /* @0x6006bf7c */
extern int  msf_process_metadata(void *, void *, unsigned);                   /* @0x6008de48 */
extern void msf_flush_metadata(void *);                                       /* @0x6008e1dc */
extern void aud_mutex_lock(void *), aud_mutex_unlock(void *);
extern void aud_cond_signal(void *);
/* @0x600a6950: timeout in microseconds, raised to >= 1000; 0 returns at once.
 * Clears the signal count first, so only signals after the call wake it. */
extern void aud_cond_wait_timeout(void *cond, void *mutex, int us);
/* Byte size / channels / 4 sets the frame count, unlike delay's +0xfc divisor. */
extern void lipsync_fadein(int ch, void *dst, const void *src, unsigned size); /* @0x600a2138 */
extern void lipsync_fadeout(int ch, void *dst, const void *src, unsigned size);/* @0x600a2218 */
extern void _lipsync_write_lipsync_state(struct lipsync *, int action);       /* @0x600a22ec */
extern uint32_t _lipsync_get_stc(int clock_type);
extern uint32_t _lipsync_get_pts(void *, int clock_type, uint32_t pts);
extern uint32_t _lipsync_get_maxpts(int clock_type);
extern int _lipsync_core_clock_unrecovered(struct lipsync *, uint32_t);
extern volatile uint32_t *stc_regs;

int _lipsync_state(struct lipsync *ls);
int _lipsync_core(struct lipsync *ls);

/* ------------------------------------------------------------------------ */

/* @0x600a2078: BaseCT + (pts - BaseST), modulo 32 bits. With speed-in != 1000
 * the delta is scaled by 1000/speed-in in signed 64 bits (@0x600a20ab..20bd);
 * outside seamless mode a negative delta first gets +mask (@0x600a2108..2136).
 * 0x600a2119 is undecoded; its only possible output, the high word, is cleared
 * by the mask at 0x600a2123. [GAP] speed-in == 0 (__divdi3 by zero). */
uint32_t _lipsync_get_pts_for_media(void *module, uint32_t pts)
{
    struct lipsync *ls = module;
    uint32_t mask = ls->clockType == 0x11 ? 0x7fffffffu : 0x0fffffffu;
    int64_t delta;

    pts &= mask;
    if (!ls->speed_mode)
        return (uint32_t)ls->BaseCT + pts - (uint32_t)ls->BaseST;
    delta = (int32_t)(pts - (uint32_t)ls->BaseST);
    if (!ls->seamless)
        delta = (delta < 0 ? (uint32_t)delta + mask : (uint32_t)delta) & mask;
    /* "BaseCT %9d + (ts %9d - BaseST %9d) / %.2f = %9d, %9x" (lvl 4) */
    return (uint32_t)ls->BaseCT + (uint32_t)(delta * 1000 / ls->speed_in);
}

/* @0x600a10a4: Flush bytes/metadata and rearm FIRST replay. Retains the event,
 * codec-derived dataType/format, action, drop_count and first-output latch. */
void _lipsync_flush(struct lipsync *ls)
{
    aud_data_adapter_flush(ls->adapter, ls->adapter_in - ls->adapter_out);
    msf_flush_metadata(ls->msf);
    ls->end_pts = 0;
    ls->start_pts = 0;
    ls->gap_handling = 0;
    ls->first_sent = 0;
}

/* @0x600a2034 _lipsync_put_data: consume n bytes from the adapter ("put_size", lvl 5) */
void _lipsync_put_data(struct lipsync *ls, unsigned n)
{
    aud_data_adapter_flush(ls->adapter, n);
}

/* Inlined at 0x600a2401..2424 and 0x600a2609..2624: get a reference to the
 * retained event, not a duplicate; no FIRST fields are rewritten. aud_event_str
 * and its free @0x600e9550 are omitted logging-only allocation/cleanup. The push
 * consumes the extra reference (@0x60072b38); +0x238 keeps the original one. */
static void lipsync_send_first(struct lipsync *ls)
{
    aud_module_push_event(ls, 0, aud_object_get(ls->first));
    ls->first_sent = 1;
}

/* @0x600a2378 _lipsync_write_metadata. discard!=0 is used by SKIP/IDLE. */
void _lipsync_write_metadata(struct lipsync *ls, int nspf, int discard)
{
    (void)nspf;
    if (!ls->over_protection && ls->drop_count <= 0 && !discard &&
        !ls->first_sent && ls->first)
        lipsync_send_first(ls);

    if (ls->dataType == 1 || (!ls->seamless && (ls->channels < 3 || ls->speed_mode == 1))) {
        /* "this lipsync module is ES mode(%d) or not MS codec(%d) or trick mode(%d).
         *  drop MD" (lvl 5): no metadata for ES */
        return;
    }
    /* MS12 metadata path: msf_* dequeue/attach, FLIX-heavy, not needed for ES [GAP] */
}

/* @0x600a2554 _lipsync_write_output: drop precedes FIRST, typing and payload
 * access (@0x600a2563..2569). Otherwise only timestamp (+0x18) and type (+0x10)
 * change (@0x600a257e..2589): exactly dataType==1 produces ES, all others PCM.
 * No payload conversion/copy or IEC framing here. Exception: autotest calls
 * @0x600fb998 with (buf,0x1f,size) @0x600a258d..2593. Despite the export's
 * AuiLogger_Init name, byte replication @0x600fb99b..9a7 and the store loop
 * @0x600fb9b5..9c0 prove this is memset, not an observational logger.
 * Push/sync @0x600a25a6..25af submit the same object; downstream can still drop. */
int _lipsync_write_output(struct lipsync *ls, struct aud_data *d)
{
    if (ls->over_protection || ls->drop_count >= 1) {
        /* "%s: drop data (count = %d)" (lvl 3) */
        aud_object_put(d);
        return 0;
    }
    if (!ls->first_sent && ls->first)
        lipsync_send_first(ls);                       /* 0x600a25fb */

    d->timestamp = ls->out_ts;
    d->type = (ls->dataType == 1) ? 1 : 0;            /* ES stays ES */
    if (ls->autotest_mode)
        memset(d->buf, 0x1f, d->size);
    /* "push %u" (lvl 5) */
    aud_module_push_data(ls, 0, d);                   /* core drop-output-data check here */
    aud_module_sync(ls, 0);
    return 0;
}

/* @0x600a1cec _lipsync_event: literal @0x600a083c -> "pcm" is both the
 * codec default and exact strcmp operand (@0x600a1de3..1df9). PCM forces 4-byte
 * internal samples / 256 samples per frame; any other codec forces ES / 2-byte
 * samples / sample-number (default 1536). The incoming bits-per-sample is read
 * but overwritten in BOTH branches (@0x600a1dfb,1e2e).
 * Reads channels/rate, NOT es/data-rate/data-channels/data-bps/acmod/clock-x4;
 * those fields survive in the retained event but do not choose this format.
 * FIRST replaces the owned +0x238 reference (@0x600a1d80..1d86,1e40), without
 * duplication or mutation, and is normally retained until output. If +0x110 is
 * already set, a reference to this SAME new FIRST is pushed immediately
 * (@0x600a1e43..1e56). +0x268 requests a flush on the next chain; FIRST does not
 * flush/reset +0x110 itself. LAST before any FIRST was sent is swallowed and
 * leaves the retained FIRST/state intact (@0x600a1ea5 ->1eef).
 * Validated FLIX: @0x600a1d48 selects LAST(2); @0x600a1d65 tests FLUSH flags
 * bit 1, selecting synchronous _lipsync_flush rather than the async flag. */
int _lipsync_event(struct lipsync *ls, struct aud_port *port, struct aud_event *e)
{
    /* "port%d, event %s" (lvl 3) */
    aud_mutex_lock(ls->mutex);
    aud_cond_signal(ls->cond);                        /* wake a sleeping _lipsync_core */
    aud_mutex_unlock(ls->mutex);

    if (e->type == 1) {                               /* FIRST_AU */
        const char *codec;

        if (ls->first)
            aud_object_put(ls->first);
        ls->channels = aud_event_value_get_int(e, "channels", 8);
        ls->bytePerSample = aud_event_value_get_int(e, "bits-per-sample", 32) / 8;
        ls->fs = aud_event_value_get_int(e, "rate", 48000) / 1000;
        ls->gap_correction = aud_event_value_get_int(e, "gap-correction", 0);
        ls->seamless = aud_event_value_get_bool(e, "seamless-mode", 0);
        /* Default "pcm": a FIRST without a codec key (lgao's main appsrc) is PCM. */
        codec = aud_event_value_get_str(e, "codec", "pcm");
        if (!strcmp(codec, "pcm")) {
            ls->bytePerSample = 4;
            ls->is_pcm = 1;
            ls->dataType = 0;
            ls->nSamplePerFrame = 0x100;
        } else {
            ls->dataType = 1;                         /* 0x600a1e12: ES */
            ls->nSamplePerFrame = aud_event_value_get_int(e, "sample-number", 0x600);
            ls->bytePerSample = 2;
            ls->is_pcm = 0;
            /* @0x600a1e31..1e37: low-word multiply then unsigned quotient.
             * [GAP] sample-number==0: DSP divide-by-zero behavior is not modeled. */
            ls->print_interval = ((uint32_t)ls->fs * 2000u) / (unsigned)ls->nSamplePerFrame;
        }
        ls->first = e;                                /* held, not forwarded */
        if (ls->first_sent)
            aud_module_push_event(ls, 0, aud_object_get(e));
        if (ls->seamless) {
            /* "first-event, set seamless mode" (lvl 2) */
        }
        ls->first_received = true;
        return 0;
    }

    if (e->type == 2) {                               /* LAST */
        aimc_raise_named_event(ls->imc, ls->uid, "last", NULL, 0);
        if (!ls->first_sent) {
            aud_object_put(e);                        /* nothing output yet: swallowed */
            return 0;
        }
        if (ls->first) {
            aud_object_put(ls->first);
            ls->first = NULL;
        }
        ls->start_pts = ls->end_pts = 0;
        ls->gap_handling = 0;
        ls->gap_correction = 0;
        ls->first_sent = 0;
        return aud_module_default_event(ls, port, e);
    }

    if (e->type == 7) {                               /* FLUSH */
        if (e->flags & 2u)                            /* @0x600a1d61..1d65 */
            _lipsync_flush(ls);                       /* @0x600a1e70; no async store */
        else
            ls->flush_async = true;                   /* @0x600a1d6d */
    }
    return aud_module_default_event(ls, port, e);
}

/* @0x600a3eb8 _lipsync_chain */
int _lipsync_chain(struct aud_port *port, struct aud_data *d)
{
    struct lipsync *ls = port->module;
    int action = LS_IDLE, ret;

    if (ls->first_received) {                         /* first chain after FIRST */
        _lipsync_flush(ls);
        ls->first_received = false;
    }

    if (!aud_module_have_active_output(ls)) {
        /* "lipsync not have output.. so throw away.." (lvl 5) */
        aud_object_put(d);
        if (ls->adapter_in - ls->adapter_out)
            aud_data_adapter_flush(ls->adapter, ls->adapter_in - ls->adapter_out);
        return 0;
    }

    if (!ls->first_sent) {
        /* "lipsync:off" or "lipsync:on, bound:%d/%d, freerun bound:%d/%d, offset:%u,
         *  ct:%s%s%s" + "BaseST/BaseCT" (lvl 3), logging only */
    }

    if (d->type == 3) {                               /* meta_ext */
        if (ls->meta_ext)
            aud_object_put(ls->meta_ext);
        ls->meta_ext = aud_object_get(d);
        aud_object_put(d);
    } else if (d->type == 2) {                        /* FLIX @0x600a3ef4 -> 0x600a419f [FLIX?] META */
        if (!(ls->dataType == 1 ||
              (!ls->seamless && (ls->channels < 3 || ls->speed_mode == 1))))
            msf_process_metadata(ls->msf, d->buf, d->size);
        aud_object_put(d);
    } else {                                          /* PCM and ES */
        aud_data_adapter_push(ls->adapter, d);        /* "queue data: size=%u, total=%d" lvl 5 */
    }

    /* seamless: compare a global dsp port's state (0x6009fa98) with 9/10 [GAP] */

    for (;;) {
        if (ls->offset_changed || ls->state_changed) {
            /* "%s is changed." naming "state" or "offset" (lvl 3) */
            ls->offset_changed = ls->state_changed = false;
            ls->idle_cnt = 0;
            ls->drop_count = ls->discard_param;
            if (ls->seamless && ls->speed_changed) {
                ls->speed_changed = false;            /* "lipsync state keep stay" (lvl 2) */
            } else {
                ls->action = action;                  /* = LS_IDLE */
            }
        }
        ret = _lipsync_core(ls);
        if (ret)
            break;
        if (ls->action == LS_REPEAT && ls->flush_async) {
            /* "flush(async) event received, exit process" / "breaking the loop" */
            ret = 1;
            break;
        }
        /* FLIX @0x600a3fa9 -> 0x600a405a [FLIX?]: stop (ret 1) when the adapter
         * holds less than channels*nSamplePerFrame*bytePerSample; FLIX @0x600a3f96
         * -> 0x600a41f2 returns 8 [FLIX?] (frame size 0). */
        if ((ls->adapter_in - ls->adapter_out) <
            (unsigned)(ls->nSamplePerFrame * ls->channels * ls->bytePerSample)) {
            ret = 1;
            break;
        }
    }
    if (ls->speed_mode && ls->seamless)
        msf_flush_metadata(ls->msf);
    /* chain returns the core's result (1 = need more data, 0xff = abort); the
     * core-side caller ignores it. "[%s] bytes done..." (lvl 4) only on ret 0. */
    return ret;
}

/* @0x600a3040 _lipsync_core: one frame. Returns 0 to continue, 1 "need more data",
 * 0xff abort. Action dispatch through jump table @0x600073d0 (observed):
 *   OFF/NORMAL/FR-RUN/EXCEPT/NODLAY/WRONG/FREERUN_WITHNOTI -> 0x600a3904 output
 *   RUN(1)/REPEAT(4)/>=14 -> 0x600a3796: nothing consumed (data held)
 *   SKIP(3)   -> 0x600a38a4 -> 0x600a3b69: write_metadata(discard) + put_data: DISCARD
 *   IDLE(6)   -> 0x600a38e7 -> 0x600a3b23: write_metadata(discard) + put_data: DISCARD
 *   ESNORM(8) -> 0x600a38d3: "ESNORMAL state is impossible" (lvl 1), return 0xff
 *   TONORM(9) -> 0x600a38b9 -> 0x600a39f2: fadein (PCM only) + output
 *   E-NORM(13)-> 0x600a388b -> 0x600a39b8: fadeout (PCM only) + output
 * Validated bge.w18 @0x600a388b/38a7/38b9/38ea/3904 compares signed queued
 * bytes against the internal frame size; short frames return 1 without consume,
 * FIRST, or fades. RUN/REPEAT/out-of-range actions have no such gate.
 * print_cnt (+0x194) counts frames while over_protection == 0.
 * @0x600a3558..3768: REPEAT waits ceil(diff/90)*1000 microseconds, rereads
 * STC and retries without consuming. Eleven unchanged reads reset the stall
 * counter and return 1, still holding data. Disconnect/async FLUSH break the
 * retry after the wait; the outer chain handles the pending FLUSH. */
int _lipsync_core(struct lipsync *ls)
{
     uint32_t pts, stc;
    int prev = ls->action, action;
    unsigned frame = (uint32_t)ls->nSamplePerFrame *
        (uint32_t)ls->channels * (uint32_t)ls->bytePerSample;
    struct aud_data *d;

retry:
    /* @0x600a308b..30b3: peek borrows the first object, including a consumed
     * prefix. It neither returns a region nor acquires another reference. */
    d = aud_data_adapter_peep(ls->adapter);
    pts = d ? d->timestamp : 0x0fffffffu;
    /* @0x600a30bd..30f4: output keeps the raw timestamp; only the clock input
     * advances by consumed bytes. Products are low-word, quotient is signed. */
    ls->out_ts = pts;
    if (ls->adapter_out) {
        int32_t bytes_per_ms = (int32_t)((uint32_t)ls->fs *
            (uint32_t)ls->channels * (uint32_t)ls->bytePerSample);
        if (bytes_per_ms)
            pts += (uint32_t)((int64_t)(int32_t)(ls->adapter_out * 90u) / bytes_per_ms);
    }
    /* @0x600a3207..3224 ->3d01..3de0: first master baseline precedes the
     * sync/seamless checks. Copies/event retain raw clocks; working bases
     * use 31 bits. Clock 0x11 then invalidates its working bases. */
    if ((ls->clockType == 0x11 || ls->clockType == 0x200) && ls->unknown_190 == 1) {
        int32_t base[2];
        stc = stc_regs[0x84 / 4];
        ls->unknown_190 = 0;
        ls->BaseST_copy = (int32_t)pts;
        ls->BaseCT_copy = (int32_t)stc;
        ls->BaseST = (int32_t)(pts & 0x7fffffffu);
        ls->BaseCT = (int32_t)(stc & 0x7fffffffu);
        base[0] = (int32_t)pts;
        base[1] = (int32_t)stc;
        aimc_raise_named_event(ls->imc, ls->uid, "adec_master_info", base, sizeof(base));
        if (ls->clockType == 0x11)
            ls->BaseST = ls->BaseCT = -1;
        else
            ls->unknown_19c = 1;
        return 1;
    }
    /* @0x600a312b..315e/33d3..3502: non-seamless clock path. The negative
     * correction compares against -mask, NOT -(mask/2), and adds mask, not
     * mask+1. Baseline initialization and seamless delay remain [GAP]. */
    if (ls->LipsyncOnOff == 1 && ls->BaseST == -1 && ls->clockType != 0x200)
        return 12;                                   /* @0x600a3248 ->3cfd */
    if (ls->seamless || ls->clockType == 0x200) {
        return _lipsync_core_clock_unrecovered(ls, pts);
    } else {
        uint32_t clock_pts = _lipsync_get_pts(ls, ls->clockType, pts);
        stc = _lipsync_get_stc(ls->clockType);
        uint32_t mask = _lipsync_get_maxpts(ls->clockType);
        int32_t upper = (int32_t)((uint32_t)ls->upperbound * 90u);
        int32_t lower = (int32_t)((uint32_t)ls->lowerbound * 90u);
        ls->diff = (int32_t)((uint32_t)ls->offset * 90u + clock_pts - stc - 0x1734u);
        if (ls->diff > (int32_t)(mask >> 1))
            ls->diff = (int32_t)((uint32_t)ls->diff - mask);
        else if (ls->diff < -(int32_t)mask)
            ls->diff = (int32_t)((uint32_t)ls->diff + mask);
        /* +0x1c8 also receives bit 3 of the local timestamp record at +70.
         * That record is zero-initialized @0x600a3076..3086 on this path. */
        ls->discontinuity = 0;
        if ((int32_t)(stc - ls->last_stc) > 0xafc8 &&
            (ls->diff < lower || ls->diff > upper))
            ls->discontinuity = 1;
        if ((int32_t)(ls->last_stc - stc) > upper && (int32_t)stc > 0x2000)
            ls->pcr_change_count = 100;
        ls->last_stc = stc;
        if (ls->pcr_change_count && clock_pts < ls->last_pts) {
            ls->discontinuity = 1;
            ls->pcr_change_count = 0;
        }
        ls->last_pts = clock_pts;
        if (ls->pcr_change_count)
            ls->pcr_change_count = (int32_t)((uint32_t)ls->pcr_change_count - 1u);
    }
    if (ls->gap_handling && (uint32_t)ls->end_pts < ls->out_ts + (uint32_t)ls->gap_correction) {
        action = LS_REPEAT;
        ls->repeat_stc = stc_regs[0x84 / 4];
    } else {
        action = _lipsync_state(ls);
    }
    /* "[%s] p:[%10u][%10u] s:[%10u][%10u] d:%4d(ms) action:%s[%d]->%s[%d] r:%d(ms)"
     * (lvl 2) when print-interval elapsed or action changed */

    if (action == LS_REPEAT) {
        int timeout;
        bool stop = false;
        if (ls->gap_handling &&
            (uint32_t)ls->end_pts < ls->out_ts + (uint32_t)ls->gap_correction) {
            timeout = (int32_t)(((uint32_t)ls->end_pts - (uint32_t)ls->start_pts) / 90u * 1000u);
            ls->gap_handling = ls->start_pts = ls->end_pts = 0;
        } else {
            timeout = (int32_t)((uint32_t)((int32_t)((uint32_t)ls->diff + 89u) / 90) * 1000u);
        }
        aud_mutex_lock(ls->mutex);
        aud_cond_wait_timeout(ls->cond, ls->mutex, timeout);
        aud_mutex_unlock(ls->mutex);
        if (ls->action != action)
            _lipsync_write_lipsync_state(ls, action);
        ls->action = action;
        ls->last_stc = _lipsync_get_stc(ls->clockType);
        if (ls->last_stc == stc) {
            ls->unchanged_stc++;
            if (ls->unchanged_stc > 10) {
                ls->unchanged_stc = 0;
                stop = true;
            }
        } else {
            ls->unchanged_stc = 0;
        }
        if (!stop && ls->disconnecting) {
            ls->disconnecting = false;
            stop = true;
        }
        if (!stop && !ls->flush_async)
            goto retry;
        prev = action;
        if (stop) {
            ls->state_changed = true;
            ls->bytes_out += frame;
            if (!ls->over_protection)
                ls->print_cnt++;
            return 1;
        }
    }

    prev = ls->action;
    if ((unsigned)action < 14 && action != LS_RUN && action != LS_REPEAT &&
        action != LS_ESNORM &&
        (int32_t)(ls->adapter_in - ls->adapter_out) < (int32_t)frame) {
        if (!ls->over_protection)
            ls->print_cnt++;
        return 1;
    }
    switch (action) {
    default:
    case LS_RUN:
    case LS_REPEAT:
        break;                                        /* hold: adapter untouched */
    case LS_SKIP:
    case LS_IDLE:
        _lipsync_write_metadata(ls, ls->nSamplePerFrame, 1);
        _lipsync_put_data(ls, frame);                 /* input dropped, no output */
        if (action == LS_SKIP && ls->seamless) {
            aud_mutex_lock(ls->mutex);
            aud_cond_wait_timeout(ls->cond, ls->mutex, 1000);
            aud_mutex_unlock(ls->mutex);
        }
        if (!ls->over_protection)
            ls->print_cnt++;
        break;
    case LS_ESNORM:
        if (!ls->over_protection)
            ls->print_cnt++;
        return 0xff;
    case LS_OFF:
    case LS_NORMAL:
    case LS_FREE_RUN:
    case LS_EXCEPT:
    case LS_TONORM:
    case LS_NODELAY:
    case LS_WRONG:
    case LS_FREERUN_WITHNOTI:
    case LS_E_NORM:
        d = aud_data_adapter_get_data(ls->adapter, frame);
        if (!d) {                                     /* "get data error" (lvl 0) */
            if (!ls->over_protection)
                ls->print_cnt++;
            return 1;
        }
        if (ls->dataType != 1) {
            if (action == LS_TONORM)
                lipsync_fadein(ls->channels, d->buf, d->buf, d->size);
            else if (action == LS_E_NORM)
                lipsync_fadeout(ls->channels, d->buf, d->buf, d->size);
        }
        _lipsync_write_metadata(ls, ls->nSamplePerFrame, 0);
        _lipsync_write_output(ls, d);
        _lipsync_put_data(ls, frame);
        if (!ls->first_output_sent) {                 /* 0x600a3de2 */
            struct { uint32_t pts; } p = { ls->out_ts };
            IMC_RaiseEvent(IMC_GetLocalImc(0), LIPSYNC_EVT_FIRST_OUTPUT, ls->imc_local, &p);
            /* "[%s] Send Noti - LIPSYNC_EVT_FIRST_OUTPUT (pts=0x%x)" (lvl 4) */
            ls->first_output_sent = 1;
        }
        break;
    }

    if (action != LS_REPEAT) {
        ls->bytes_in += frame;
        if (ls->drop_count >= 1)
            ls->drop_count--;
    }
    if (action != LS_SKIP)
        ls->bytes_out += frame;
    if (action != prev)
        _lipsync_write_lipsync_state(ls, action);     /* shared regs 0xc1010200/0xc100fe00 */
    ls->action = action;
    return 0;
}

/* @0x600a2644 _lipsync_state: next action from the previous one and diff (ticks).
 * Bounds are ms*90; recovery = lowerbound + additional_bound. Arithmetic and
 * counters wrap at 32 bits. @0x600a2750/2945/2a9f/2b4e/2c3e/2c8d are 24-bit
 * SALT/SALTU, not FLIX. clockType 0x200 always yields FREERUN_WITHNOTI and
 * re-baselines on diff drift; the NODELAY thresholds have no writer here.
 * PCM (0x100, LipsyncOnOff=1): REPEAT holds while 0 < diff <= freerun_upper
 * until the STC passes repeat_stc + freerun_upper (a frozen STC never does);
 * then WRONG, which outputs and leaves only via discontinuity or reset. */
int _lipsync_state(struct lipsync *ls)
{
    /* 0x600a265b: same "%s is changed" reset as in _lipsync_chain */
    if (ls->offset_changed || ls->state_changed) {
        ls->offset_changed = ls->state_changed = false;
        ls->idle_cnt = 0;
        ls->drop_count = ls->discard_param;
        if (ls->seamless && ls->speed_changed)
            ls->speed_changed = false;
        else
            ls->action = LS_IDLE;
    }
    if (ls->idle_total < 100)
        ls->idle_total = (int32_t)((uint32_t)ls->idle_total + 1u);
    int skip_limit = ls->idle_total < 0 ? 10 :
        (ls->clockType == 0x100 || ls->clockType == 0x101 ? 5 : 200);
    int repeat_limit = ls->idle_total < 0 ? 10 : 200;
    if (ls->count_transition)
        skip_limit = repeat_limit = ls->count_transition;
    int32_t upper = (int32_t)((uint32_t)ls->upperbound * 90u);
    int32_t freerun_upper = (int32_t)((uint32_t)ls->freerunupperbound * 90u);
    int32_t recovery = (int32_t)(((uint32_t)ls->lowerbound +
                                  (uint32_t)ls->additional_bound) * 90u);
    int32_t freerun_lower = (int32_t)((uint32_t)ls->freerunlowerbound * 90u);
    if (ls->clockType == 0x200) {                     /* @0x600a285c/2905/27f3 */
        int32_t drift = (int32_t)((uint32_t)ls->diff - (uint32_t)ls->last_diff);
        bool rebase = false;
        if (drift > upper) {
            ls->skip_cnt = (int32_t)((uint32_t)ls->skip_cnt + 1u);
            if (ls->skip_cnt > skip_limit)
                rebase = true;                        /* "[instead repeat]" */
            else
                ls->repeat_cnt = 0;
        } else if ((int32_t)((uint32_t)drift + (uint32_t)recovery) >= 0) {
            ls->skip_cnt = ls->repeat_cnt = 0;
        } else {
            ls->repeat_cnt = (int32_t)((uint32_t)ls->repeat_cnt + 1u);
            if (ls->repeat_cnt > repeat_limit)
                rebase = true;                        /* "[instead skip]" */
            else
                ls->skip_cnt = 0;
        }
        if (rebase) {                                 /* @0x600a2888..28f2 */
            int32_t base[2] = {ls->BaseST_copy, ls->BaseCT_copy};
            ls->skip_cnt = ls->repeat_cnt = 0;
            ls->last_diff = ls->diff;
            aimc_raise_named_event(ls->imc, ls->uid, "adec_master_info", base, sizeof(base));
            ls->unknown_19c = 1;
        }
        return LS_FREERUN_WITHNOTI;
    }
    if (ls->LipsyncOnOff != 1 || ls->NodelayOnOff) {
        if (ls->NodelayOnOff != 1)
            return LS_OFF;
        uint32_t queued = (uint32_t)ls->unknown_174 * (ls->adapter_in - ls->adapter_out);
        if (ls->action == LS_NODELAY)                 /* @0x600a293e..294b */
            return (uint32_t)ls->unknown_16c < queued ? LS_SKIP : LS_NODELAY;
        if (ls->action == LS_SKIP)                    /* @0x600a274b..2756 */
            return (uint32_t)ls->unknown_170 < queued ? LS_SKIP : LS_NODELAY;
        return LS_NODELAY;
    }
    int32_t recovered_gap = (int32_t)((uint32_t)ls->diff + (uint32_t)recovery);
    int32_t negative_gap = (int32_t)((uint32_t)ls->diff + (uint32_t)freerun_lower);
    int action;
    switch (ls->action) {
    case LS_IDLE:                                     /* @0x600a2b57/2c33/2e1e */
        if (ls->idle_cnt == 0 && ls->idle_time == 0 && ls->clockType == 0x100 &&
            (ls->out_ts & 0x0fffffffu) == 0x0fffffffu)
            ls->idle_time = 0xb;
        ls->idle_cnt = (int32_t)((uint32_t)ls->idle_cnt + 1u);
        if (ls->idle_cnt < ls->idle_time)
            action = LS_IDLE;
        else if (ls->diff > 0)                        /* @0x600a2c3e "first gap" */
            action = ls->diff > freerun_upper ? LS_FREE_RUN : LS_REPEAT;
        else if (ls->diff == 0)
            action = LS_TONORM;
        else
            action = negative_gap < 0 ? LS_FREE_RUN : LS_SKIP;
        break;
    case LS_NORMAL:
    case LS_ESNORM:                                   /* @0x600a2af1 */
        if (ls->diff > upper) {
            ls->skip_cnt = (int32_t)((uint32_t)ls->skip_cnt + 1u);
            if (ls->skip_cnt <= skip_limit) {
                ls->repeat_cnt = 0;
                action = LS_NORMAL;
            } else {
                ls->skip_cnt = 0;
                action = ls->diff <= freerun_upper ? LS_REPEAT : LS_FREE_RUN;
            }
        } else if (recovered_gap >= 0) {
            ls->skip_cnt = ls->repeat_cnt = 0;
            action = LS_NORMAL;
        } else {
            ls->repeat_cnt = (int32_t)((uint32_t)ls->repeat_cnt + 1u);
            if (ls->repeat_cnt <= repeat_limit) {
                ls->skip_cnt = 0;
                action = LS_NORMAL;
            } else {                                 /* @0x600a2da8..2e59 */
                uint32_t bytes_per_ms = (uint32_t)ls->fs *
                    (uint32_t)ls->channels * (uint32_t)ls->bytePerSample;
                if (!bytes_per_ms)
                    bytes_per_ms = 0x180;
                int32_t queued = (int32_t)(ls->adapter_in - ls->adapter_out);
                int32_t queued_ticks = (int32_t)((uint32_t)queued * 90u);
                int32_t needed = (int32_t)(0u - (uint32_t)ls->diff * bytes_per_ms);
                ls->repeat_cnt = 0;
                action = queued_ticks < needed && queued < ls->frame_bytes ?
                    LS_EXCEPT : LS_SKIP;
            }
        }
        break;
    case LS_SKIP:                                     /* @0x600a2a48/2d2f */
        if (ls->diff > 0)
            action = LS_TONORM;
        else if (negative_gap < 0)
            action = LS_FREE_RUN;
        else {
            ls->repeat_cnt = (int32_t)((uint32_t)ls->repeat_cnt + 1u);
            if (ls->repeat_cnt <= repeat_limit)
                action = LS_SKIP;
            else {
                ls->cnt150 = 0;
                action = LS_EXCEPT;
            }
        }
        break;
    case LS_REPEAT:                                   /* @0x600a2b9a/2c7b */
        if (ls->diff <= 0)
            action = LS_NORMAL;
        else if (ls->diff > freerun_upper)
            action = LS_FREE_RUN;
        else                                          /* @0x600a2c7b..2c90 raw STC */
            action = ls->repeat_stc + (uint32_t)freerun_upper < stc_regs[0x84 / 4] ?
                LS_WRONG : LS_REPEAT;
        break;
    case LS_FREE_RUN:                                 /* @0x600a2b7b/2c96 */
        if (recovered_gap > 0 && ls->diff < upper) {
            ls->repeat_cnt = ls->skip_cnt = 0;
            action = LS_NORMAL;
        } else if (ls->diff > 0 && ls->diff < freerun_upper)
            action = LS_REPEAT;
        else if (negative_gap > 0 && ls->diff < 0) {
            ls->repeat_cnt = 0;
            action = LS_SKIP;
        } else
            action = LS_FREE_RUN;
        break;
    case LS_TONORM:                                   /* @0x600a2ad7/2c17 */
        if (ls->diff > upper || recovered_gap < 0)
            action = LS_E_NORM;
        else {
            ls->skip_cnt = ls->repeat_cnt = 0;
            action = LS_NORMAL;
        }
        break;
    case LS_E_NORM:                                   /* @0x600a2a94/2ab6/2d7e */
        if (ls->diff > upper)                         /* @0x600a2a9f */
            action = ls->diff > freerun_upper ? LS_FREE_RUN : LS_REPEAT;
        else if (recovered_gap >= 0) {
            ls->skip_cnt = ls->repeat_cnt = 0;
            action = LS_TONORM;
        } else
            action = negative_gap < 0 ? LS_FREE_RUN : LS_SKIP;
        break;
    case LS_EXCEPT:                                   /* @0x600a2b29/2deb */
        if (ls->diff > upper) {
            ls->skip_cnt = (int32_t)((uint32_t)ls->skip_cnt + 1u);
            if (ls->skip_cnt > skip_limit) {          /* @0x600a2b4e */
                ls->skip_cnt = 0;
                action = ls->diff > freerun_upper ? LS_FREE_RUN : LS_REPEAT;
            } else {
                ls->repeat_cnt = 0;
                action = LS_EXCEPT;
            }
        } else if (recovered_gap >= 0) {
            ls->skip_cnt = ls->repeat_cnt = 0;
            action = LS_NORMAL;
        } else {
            ls->skip_cnt = 0;
            ls->cnt150 = (int32_t)((uint32_t)ls->cnt150 + 1u);
            action = ls->cnt150 <= repeat_limit ? LS_EXCEPT : LS_SKIP;
            if (action == LS_SKIP)
                ls->repeat_cnt = 0;
        }
        break;
    case LS_WRONG:                                    /* @0x600a2acb */
        action = LS_WRONG;
        break;
    default:
        action = LS_NORMAL;                           /* @0x600a2966 */
        break;
    }
    if (ls->discontinuity == 1) {                      /* @0x600a2a59 */
        ls->idle_cnt = 0;
        return LS_IDLE;
    }
    if (action == LS_REPEAT && ls->action != LS_REPEAT)
        ls->repeat_stc = stc_regs[0x84 / 4];           /* @0x600a2762..2775 */
    return action;
}

/* @0x600a1100: Scalar reset dispatch validated with xtobjdump. START
 * @0x600a13a7..13ed resets action/counters, not FIRST, dataType or format;
 * discard_param==0 preserves an existing drop_count. STOP @0x600a1670 and
 * 0x10150000 @0x600a1500 are no-ops. 0x1003002c @0x600a163f..1647 requests
 * next-chain flush (first_received), not a format reset or immediate flush.
 * Debug reset @0x600a1564..1582 clears gap/debug fields, not cached FIRST.
 * SET_BASE(0x52030008) @0x600a119a..11bf reads two 32-bit words: clockbase,
 * streambase, without length validation or conversion. Only a changed pair sets
 * state_changed (@0x600a11a3/11a9); both copies are refreshed even if unchanged
 * (@0x600a164e..1657). It retains queued data, FIRST, format and output latches;
 * the state/chain reset is deferred. ADEC_CMD_SET_GAP_INFO(0x10210010), selected
 * @0x600a13f4/1402, reads start/end as low/high word pairs. @0x600a1457..1489
 * multiplies each by 9 (64-bit wrap), unsigned-divides by 100000, stores the low
 * 32 bits and enables gap handling. Neither command checks len; gap info has no
 * explicit null check. Other LIPSYNC_CMD_* dispatch is [GAP]. */
int _lipsync_cmd(struct lipsync *ls, unsigned cmd, void *param, unsigned len)
{
    aud_mutex_lock(ls->mutex);
    aud_cond_signal(ls->cond);
    aud_mutex_unlock(ls->mutex);
    switch (cmd) {
    case 0x10210010: {
        const uint32_t *timestamps = param;
        uint64_t start = (uint64_t)timestamps[0] | ((uint64_t)timestamps[1] << 32);
        uint64_t end = (uint64_t)timestamps[2] | ((uint64_t)timestamps[3] << 32);
        ls->start_pts = (int32_t)(uint32_t)((start * 9u) / 100000u);
        ls->end_pts = (int32_t)(uint32_t)((end * 9u) / 100000u);
        ls->gap_handling = 1;
        return 0;
    }
    case 0x52030008: {
        const int32_t *base = param;
        assert(base != NULL);
        if (ls->BaseCT != base[0] || ls->BaseST != base[1])
            ls->state_changed = true;
        ls->BaseCT = ls->BaseCT_copy = base[0];
        ls->BaseST = ls->BaseST_copy = base[1];
        return 0;
    }
    case 0x10010000:
        ls->idle_cnt = ls->idle_total = 0;
        ls->action = LS_IDLE;
        ls->disconnecting = ls->seamless = false;
        ls->state_changed = true;
        ls->unknown_190 = 1;
        ls->first_output_sent = ls->unknown_19c = 0;
        ls->repeat_cnt = ls->skip_cnt = ls->cnt150 = ls->last_diff = ls->print_cnt = 0;
        if (ls->discard_param)
            ls->drop_count = ls->discard_param;
        ls->bytes_in = ls->bytes_out = 0;
        return 0;
    case 0x10020000:
    case 0x10150000:
        return 0;
    case 0x1003002c:
        ls->first_received = true;
        return 0;
    case 0x10000000:
        memset(ls->debug_count, 0, sizeof(ls->debug_count));
        ls->start_pts = ls->end_pts = ls->gap_handling = ls->gap_correction = 0;
        ls->additional_bound = 0;
        return 0;
    }
    return aud_module_default_command(ls, cmd, param, len);
}

/* @0x600a1efc _lipsync_constructor: "create LIPSYNC" (lvl 3) */
int _lipsync_constructor(struct lipsync *ls)
{
    ls->frame_bytes = ls->frame_bytes_ch = 0x1000;
    ls->is_pcm = 1;
    ls->nSamplePerFrame = 0x100;
    ls->bytePerSample = 4;
    ls->idle_time = 0xb;
    ls->print_interval = 0x177;
    ls->channels = 2;
    ls->fs = 0x30;
    ls->offset = 0x96;
    ls->speed_in = 1000;
    /* +0x11c=0x1000, +0x128=0x6800, +0x124=0xc00, +0x1c0=-1, freerun bounds 5000 */
    return 0;
}

/* @0x600a1ff0 _lipsync_destructor: "destroy LIPSYNC" */
void _lipsync_destructor(struct lipsync *ls)
{
    msf_flush_metadata(ls->msf);
    aud_data_adapter_flush(ls->adapter, ls->adapter_in - ls->adapter_out);
    if (ls->first)
        aud_object_put(ls->first);
}

/* ---- property setters (value already parsed to int by set_prop) ---- */

/* @0x600a4344 "offset" */
int _lipsync_set_prop_offset(struct lipsync *ls, int v)
{
    if (ls->offset != v)
        ls->offset_changed = true;
    ls->offset = v;
    return 0;
}

/* @0x600a439c "speed-in" */
int _lipsync_set_prop_speed(struct lipsync *ls, int v)
{
    if (ls->speed_in != v)
        ls->speed_changed = true;
    ls->speed_in = v;
    ls->speed_mode = (v != 1000);
    return 0;
}

/* @0x600a440c "param_channel" */
int _lipsync_set_prop_channel(struct lipsync *ls, int ch)
{
    ls->frame_bytes = ls->frame_bytes_ch = ch << 11;
    if (ls->channels == 0)
        ls->channels = ch;
    return 0;
}

/* @0x600a4458 "dataType" (kernel sends 1 for lipsync_bypass).
 * Parsed integer v is stored verbatim @0x600a4472; exactly 1 selects 2-byte ES,
 * all other values select 4-byte PCM (@0x600a4475..4484,449f..44ab).
 * Internal configuration only: no FIRST mutation/re-emission, no payload work,
 * no nSamplePerFrame/print_interval reset. A later FIRST overwrites these three
 * fields from its codec; conversely a property after FIRST can change output
 * buffer typing without changing the retained event's codec or format fields. */
int _lipsync_set_prop_datatype(struct lipsync *ls, int v)
{
    ls->dataType = v;
    if (v == 1) {
        ls->bytePerSample = 2;
        ls->is_pcm = 0;
    } else {
        ls->bytePerSample = 4;
        ls->is_pcm = 1;
    }
    return 0;
}

/* @0x600a44b0 "idle_time": float conversion of the value [FLIX] */
int _lipsync_set_prop_idletime(struct lipsync *ls, int v)
{
    ls->idle_time = v;
    return 0;
}

/* @0x600a4520 "BaseCT" */
int _lipsync_set_baseCT(struct lipsync *ls, int v)
{
    ls->BaseCT = v;
    ls->state_changed = true;
    return 0;
}

/* @0x600a457c "BaseST" */
int _lipsync_set_baseST(struct lipsync *ls, int v)
{
    ls->BaseST = v;
    ls->state_changed = true;
    return 0;
}

/* @0x600a45d8 "state": the value itself is never read again; the side effect is
 * state_changed -> next chain resets the action to IDLE. "%s: change state to %d"
 * (lvl 3). */
int _lipsync_set_state(struct lipsync *ls, int v)
{
    ls->state = v;
    ls->state_changed = true;
    ls->disconnecting = false;
    return 0;
}

/* @0x600a4630 "seamless-mode": ignored ("seamless mode is set only by the first event") */
int _lipsync_set_seamless_mode(struct lipsync *ls, int v)
{
    (void)ls;
    (void)v;
    return 0;
}

/* @0x600a4658 _lipsync_disconnect_port */
int _lipsync_disconnect_port(struct lipsync *ls)
{
    aud_mutex_lock(ls->mutex);
    ls->disconnecting = true;                         /* "mark disconnecting" */
    aud_cond_signal(ls->cond);
    aud_mutex_unlock(ls->mutex);
    return -1;
}
