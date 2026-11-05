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
 *    and drop_count to discard_param. With LipsyncOnOff == 1 the first idle_time
 *    frames after renderer_postconnect's "state=1" are discarded; with sync off
 *    _lipsync_state returns LIPSYNC_OFF and data is output at once.
 *  - No output port connected -> all input thrown away ("so throw away..").
 *  - FIRST is held back and only forwarded with the first real output buffer;
 *    LAST is swallowed if nothing was ever output.
 *  - The kernel's "drop_output_data=1/0" (underscores) is not a dsp2 property
 *    name (core uses "drop-output-data"), so it changes nothing (module_core.c).
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

struct aud_event { int pad[3]; int type; };
struct aud_data {
    int pad[4];
    int type;                    /* +0x10 0 pcm, 1 es, 2 meta, 3 meta_ext */
    int pad1;
    uint32_t timestamp;          /* +0x18 */
    int pad2[6];
    unsigned size;               /* +0x34 */
    void *buf;                   /* +0x38 */
};
struct aud_port { int pad; struct lipsync *module; };

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
    int   autotest_mode;         /* +0x144 "autotest_mode": AuiLogger dump */
    int   repeat_cnt, skip_cnt, cnt150; /* +0x148/+0x14c/+0x150 */
    int   idle_cnt;              /* +0x154 */
    int   last_diff;             /* +0x158 */
    int   action;                /* +0x15c previous action */
    int   frame_bytes, frame_bytes_ch; /* +0x160/+0x164 */
    int   print_interval;        /* +0x168 */
    int   speed_mode;            /* +0x17c speed != 1000 */
    int   speed_in;              /* +0x180 */
    int   is_pcm;                /* +0x184 */
    int   idle_time;             /* +0x188 */
    int   idle_total;            /* +0x18c */
    int   print_cnt;             /* +0x194 frames since last action log (vs print-interval) */
    int   first_output_sent;     /* +0x198 LIPSYNC_EVT_FIRST_OUTPUT raised */
    int   discontinuity;         /* +0x1c8 */
    uint32_t out_ts;             /* +0x1d0 */
    int   dataType;              /* +0x1d4 0 pcm, 1 es */
    int   channels, fs, bytePerSample; /* +0x1d8/+0x1dc/+0x1e0 */
    int   discard_param;         /* +0x1e4 */
    int   LipsyncOnOff;          /* +0x1e8 */
    int   upperbound, lowerbound;/* +0x1ec/+0x1f0 */
    int   offset;                /* +0x1f4 "offset"/"ConstantOffset" */
    int   clockType;             /* +0x200 0x100 pcr, 0x200 audio master */
    int   BaseCT, BaseST;        /* +0x204/+0x208 */
    int   NodelayOnOff;          /* +0x214 */
    int   over_protection;       /* +0x218 */
    int   gap_handling;          /* +0x220 */
    int   start_pts, end_pts;    /* +0x224/+0x228 */
    int   gap_correction;        /* +0x22c */
    bool  offset_changed;        /* +0x230 */
    bool  state_changed;         /* +0x231 also set by BaseCT/BaseST/SET_BASE */
    bool  speed_changed;         /* +0x232 */
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
extern int  aud_data_adapter_peep(void *, uint32_t *ts);                      /* @0x6006bf7c */
extern int  msf_process_metadata(void *, void *, unsigned);                   /* @0x6008de48 */
extern void msf_flush_metadata(void *);                                       /* @0x6008e1dc */
extern void aud_mutex_lock(void *), aud_mutex_unlock(void *);
extern void aud_cond_signal(void *);
extern void aud_cond_wait_timeout(void *, void *, int ms);
extern int  strcmp(const char *, const char *);
extern void lipsync_fadein(int ch, void *buf, unsigned size);                 /* @0x600a2138 */
extern void lipsync_fadeout(int ch, void *buf, unsigned size);                /* @0x600a2218 */
extern void _lipsync_write_lipsync_state(struct lipsync *, int action);       /* @0x600a22ec */
extern void AuiLogger_dump(void *buf, int id, unsigned size);                 /* @0x600fb998 */

int _lipsync_state(struct lipsync *ls);
int _lipsync_core(struct lipsync *ls);

/* ------------------------------------------------------------------------ */

/* @0x600a10a4 _lipsync_flush: "do flush..." (lvl 3) */
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

static void lipsync_send_first(struct lipsync *ls)
{
    aud_module_push_event(ls, 0, aud_object_get(ls->first));  /* "send first event (%s)" lvl 3 */
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

/* @0x600a2554 _lipsync_write_output */
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
        AuiLogger_dump(d->buf, 0x1f, d->size);
    /* "push %u" (lvl 5) */
    aud_module_push_data(ls, 0, d);                   /* core drop-output-data check here */
    aud_module_sync(ls, 0);
    return 0;
}

/* @0x600a1cec _lipsync_event */
int _lipsync_event(struct lipsync *ls, struct aud_port *port, struct aud_event *e)
{
    (void)port;
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
            ls->print_interval = (unsigned)(ls->fs * 2000) / (unsigned)ls->nSamplePerFrame;
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

    if (e->type == 2) {                               /* FLIX @0x600a1d48 -> 0x600a1e90 [FLIX?] LAST */
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

    if (e->type == 7) {                               /* FLUSH: FLIX -> 0x600a1e78 "flush event" */
        ls->flush_async = true;                       /* breaks the core loop */
    } else {
        /* FLIX @0x600a1d65 -> 0x600a1e70 [FLIX?]: one more type (likely DISCONNECT 6
         * or STOP 4) runs _lipsync_flush before the default handler. */
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
 * Each dispatch site starts with a FLIX whose condition is [FLIX?] "a full frame is
 * queued"; if not, return 1. print_cnt (+0x194) counts frames while
 * over_protection == 0. */
int _lipsync_core(struct lipsync *ls)
{
    uint32_t pts;
    int prev = ls->action, action;
    unsigned frame = (unsigned)(ls->nSamplePerFrame * ls->channels * ls->bytePerSample);
    struct aud_data *d;

    if (!aud_data_adapter_peep(ls->adapter, &pts)) {
        /* no timestamp ("_lipsync_get_timestamp" lvl 3): pts = 0x0fffffff */
        pts = 0x0fffffff;
    }
    /* stc read from 0xc1000684, diff = pts - stc - offset, discontinuity and
     * PCR-change handling, "STC not changed" after 10 equal reads [FLIX] */
    action = _lipsync_state(ls);
    /* "[%s] p:[%10u][%10u] s:[%10u][%10u] d:%4d(ms) action:%s[%d]->%s[%d] r:%d(ms)"
     * (lvl 2) when print-interval elapsed or action changed */

    switch (action) {
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
    default:                                          /* output a frame */
        d = aud_data_adapter_get_data(ls->adapter, frame);
        if (!d) {                                     /* "get data error" (lvl 0) */
            if (!ls->over_protection)
                ls->print_cnt++;
            return 1;
        }
        if (ls->dataType != 1) {
            if (action == LS_TONORM)
                lipsync_fadein(ls->channels, d->buf, d->size);
            else if (action == LS_E_NORM)
                lipsync_fadeout(ls->channels, d->buf, d->size);
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

/* @0x600a2644 _lipsync_state: next action. ~57 FLIX bundles hide most comparisons;
 * the control skeleton below is observed, comparisons marked [FLIX?] are inferred.
 *  - LipsyncOnOff != 1 or NodelayOnOff != 0: never reaches the action table;
 *    returns LIPSYNC_OFF(0) (NODLAY/SKIP when NodelayOnOff == 1). LIPSYNC_OFF is
 *    an output action in _lipsync_core, so with sync off data flows immediately
 *    and a "state=1" only re-arms drop_count = discard_param (0 by default).
 *  - LipsyncOnOff == 1: jump table @0x60007380 on the previous action. IDLE(6)
 *    @0x600a2b57 counts idle_cnt up to idle_time ("Set default escape num" when 0,
 *    lvl 2), then escapes ("[%s] first gap : %d(ms) idle->%d", lvl 4) to REPEAT(4)
 *    or FR-RUN(5); NORMAL/ESNORM(2/8) @0x600a2af1, SKIP(3) @0x600a2a48 -> IDLE when
 *    discontinuity, etc.
 *  - clockType 0x200 (audio master): "base line Noti(audio master mode)" raises
 *    "adec_master_info" and returns FREERUN_WITHNOTI(12); no-PCR baseline raises
 *    LIPSYNC_EVT_NOPCR_BASELINE.
 *  - Returned values seen: 0,2,3,4,5,6,7,9,10,11,12,13 (never ESNORM 8). */
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
    /* count_transition (+0x21c) != 0 takes a separate gradual path @0x600a2705 [GAP].
     * 0x600a26ff/0x600a28f4: the action table is only used when LipsyncOnOff == 1,
     * NodelayOnOff == 0 and clockType != 0x200. Otherwise (0x600a2723): */
    if (ls->LipsyncOnOff != 1 || ls->NodelayOnOff) {
        if (ls->clockType == 0x200)
            return LS_FREERUN_WITHNOTI;               /* [FLIX?] 0x600a285c */
        if (ls->NodelayOnOff != 1)
            return LS_OFF;                            /* 0x600a277a */
        return LS_NODELAY;                            /* or SKIP by queue level [FLIX?] */
    }
    switch (ls->action) {                             /* table @0x60007380 */
    case LS_IDLE:                                     /* 0x600a2b57 */
        if (ls->idle_cnt == 0 && ls->idle_time == 0)
            ls->idle_time = 0xb;                      /* "Set default escape num" */
        if (++ls->idle_cnt <= ls->idle_time)          /* [FLIX?] @0x600a2b64 */
            return LS_IDLE;
        return LS_REPEAT;                             /* or FR-RUN by gap sign [FLIX?] */
    default:
        return LS_NORMAL;                             /* bounds vs diff [GAP] */
    }
}

/* @0x600a1100 _lipsync_cmd: LIPSYNC_CMD_* (SET_BASE logs "clockbase : %d" and sets
 * state_changed) and 0x10000000 (reset debug counters); else default command. */
int _lipsync_cmd(struct lipsync *ls, unsigned cmd, void *param, unsigned len)
{
    aud_mutex_lock(ls->mutex);
    aud_cond_signal(ls->cond);
    aud_mutex_unlock(ls->mutex);
    /* [FLIX] dispatch of LIPSYNC_CMD_* ids */
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

/* @0x600a4458 "dataType" (kernel sends 1 for lipsync_bypass) */
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
