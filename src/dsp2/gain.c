/*
 * dsp2 "gain" module: volume/mute/fade on PCM and the ES ("non-PCM") policy, i.e.
 * when gain forwards, zero-fills or drops IEC 61937 bitstream data.
 * Also aud_lmodule_skip_nonpcm (legacy lmodule helper living next to the module code).
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 * Main segment linked at 0x60000000. Branch-slot FLIX conditions are decoded by
 * tools/xtobjdump.sh (Zephyr intel_tgl_adsp overlay, independently checked branch
 * targets/log guards). Non-branch SIMD/float bundles are NOT validated by those
 * checks: remaining [FLIX] markers concern PCM math, not the ES path.
 * ES does not enter gain_control or gain_set_fade_gain. No bits-per-sample test
 * exists here; channels only sets channel and the pre-dispatch frame division.
 * All ES-only buffers are pushed (original or same-size zeros); this module has
 * no ES queue or wait. Allocation failure can discard the input with return -1.
 * Log guard bundles (l32i aN,<mask 0x4ff90098>) are dropped: they only gate logmm_printl.
 * Evidence: build/decomp/audio_a0_dsp2.main_60000000.bin.c (gain_chain and its
 *           __blk_ fragments, FUN_60088930/FUN_60088aa9 = mid-function blocks of
 *           gain_chain), build/scratch/evidence/dsp2-gain/gain-properties.json (property table
 *           @0x601023b0), build/scratch/evidence/dsp2-gain/gain.asm.
 *
 * gain_handle_nonpcm has NO function of its own: its __func__ slot 0x6005f3e0 is
 * loaded once, at 0x6008889b inside gain_chain, right where the data-type switch
 * starts. The section runs from 0x6008889b to 0x60088d6a, plus the out-of-line tails
 * 0x60088d6d..0x60088da2, 0x60088e05..0x60088f10 and 0x60088fcc..0x60089040.
 * Every "ES drop?/META drop?/ES ONLY.." log line (0x178..0x1af) belongs to it. It is
 * rebuilt below as an inline helper without a tag.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ------------------------------------------------------------------------
 * Framework types (offsets read from the firmware)
 * ---------------------------------------------------------------------- */
enum aud_data_type {             /* aud_data +0x10 */
    AUD_DATA_PCM = 0,
    AUD_DATA_ES = 1,             /* IEC 61937 / compressed bitstream */
    AUD_DATA_META = 2,
    AUD_DATA_META_EXT = 3,
};

struct aud_data {
    int      type;               /* +0x10 enum aud_data_type */
    int      index;              /* +0x14 */
    uint32_t ts;                 /* +0x18 */
    unsigned size;               /* +0x34 bytes */
    void    *ptr;                /* +0x38 */
};

enum aud_event_type {            /* aud_event +0x0c */
    AUD_EVENT_FIRST_AU = 1,
    AUD_EVENT_LAST_AU = 2,
    AUD_EVENT_FLUSH = 7,
    AUD_EVENT_CUSTOM = 0xd,
};

struct aud_event {
    int      type;               /* +0x0c */
    unsigned prop;               /* +0x10 bit0 DOWNWARD, bit1 SYNC */
};

struct aud_port {
    int      index;              /* +0x00 */
    struct aud_module *module;   /* +0x04 */
    uint32_t flag;               /* +0x08 bit1 DISABLED */
};

struct aud_mutex { int opaque; };

/* Module-private part of a "gain" instance. Property names from the table @0x601023b0. */
struct gain_module {
    const char *name;            /* +0x000 */
    uint32_t    debug;           /* +0x008 per-module log bits */
    struct aud_mutex lock;       /* +0x074 */
    int32_t  current_gain;       /* +0x0f0 "current-gain" (ro), Q23: 0x800000 = 1.0 */
    int32_t  fade_gain;          /* +0x0f4 "fade-gain" (ro) */
    int32_t  final_gain;         /* +0x0f8 "final-gain" (ro) */
    int32_t  gain;               /* +0x0fc "gain": pending new gain, -1 = none */
    int32_t  channel;            /* +0x100 "channel" */
    bool     mute_input;         /* +0x104 "mute-input"  (constructor: true) */
    bool     mute_output;        /* +0x105 "mute-output" (constructor: true) */
    int32_t  prev_muted;         /* +0x108 mute state of the previous buffer (init 1) */
    bool     got_first_data;     /* +0x10c */
    int32_t  fadetime_ms;        /* +0x110 "fadetime-ms", -1 = no ease request */
    int32_t  fade_target;        /* +0x114 */
    int32_t  target_gain;        /* +0x118 "target-gain" */
    int32_t  fade_start_req;     /* +0x11c */
    bool     gain_changed;       /* +0x120 */
    int32_t  delta_s;            /* +0x124 "delta-s" (ro) */
    int32_t  delta_g;            /* +0x128 "delta-g" (ro) */
    int32_t  fade_pos;           /* +0x12c */
    int32_t  stop_after;         /* +0x130 "stop-after" (ro) */
    int32_t  fade_from;          /* +0x134 */
    int32_t  fade_samples;       /* +0x138 */
    bool     es_mode;            /* +0x13c FIRST_AU "es" -- the ES pass/drop switch */
    int32_t  es_zero_state;      /* +0x140 0 = pass ES, 2 = replace ES by zeros */
    int32_t  es_pkt_pos;         /* +0x144 buffers sent inside the current packet */
    int32_t  es_unit_per_packet; /* +0x148 buffers per ES packet (init 1) */
    struct aud_event *first;     /* +0x14c FIRST_AU as received */
    struct aud_event *first_muted; /* +0x150 dup of it with "acmod"=2 */
    bool     delay_changed;      /* +0x154 */
    int32_t  fade_type;          /* +0x158 "fade-type" */
};

/* ------------------------------------------------------------------------
 * Callees
 * ---------------------------------------------------------------------- */
extern volatile uint32_t gain_log_mask;          /* *0x4ff90098 */
extern void logmm_printl(const void *log, int lvl, const char *fn, int line, const char *fmt, ...);
extern const void *const gain_log;               /* 0x60102340 */

extern void *aud_object_get(void *o);
extern void  aud_object_put(void *o);
extern struct aud_event *aud_event_dup(struct aud_event *e);
extern int   aud_event_value_get_int(struct aud_event *e, const char *k, int def);
extern bool  aud_event_value_get_bool(struct aud_event *e, const char *k, bool def);
extern const char *aud_event_value_get_str(struct aud_event *e, const char *k, const char *def);
extern void  aud_event_value_set_int(struct aud_event *e, const char *k, int v);
extern int   aud_module_default_event(struct gain_module *m, struct aud_port *p, struct aud_event *e);
extern int   aud_module_default_command(struct gain_module *m, uint32_t cmd, void *arg, unsigned len);
extern int   aud_module_push_data(struct gain_module *m, int port, struct aud_data *d);
extern int   aud_module_push_event(struct gain_module *m, int port, struct aud_event *e);
extern void  aud_module_sync(struct gain_module *m, int port);
extern struct aud_data *aud_data_new(unsigned size);
extern struct aud_data *aud_data_new_zero(unsigned size);
extern void  aud_mutex_lock(struct aud_mutex *mx);
extern void  aud_mutex_unlock(struct aud_mutex *mx);
extern int   aud_prop_get_value_int(void *prop, const char *k, int def);

#define GAIN_UNITY 0x800000
#define SAMPLE_RATE 48000

/* Commands (literal pool 0x6005d25c/0x6005d2c8/0x6005d2bc/0x6005f488/0x6005f48c) */
#define CMD_NOP_A        0x10010000u
#define CMD_NOP_B        0x10020000u
#define CMD_NOP_C        0x10000000u
#define CMD_SET_GAIN     0x66000004u
#define CMD_SET_CHANNEL  0x66010004u

/* ------------------------------------------------------------------------
 * PCM kernels
 * ---------------------------------------------------------------------- */

/* 0x60088118: ONE 32-bit PCM frame, at src/dst + word_offset, not n frames.
 * Verified caller @60088ba2..60088bad: a2=src, a3=word_offset, a4=dst,
 * a5=channels, a6=effective Q23 gain. Dispatch is 2/4/6/8 channels.
 * Each channel decodes as mulf32s -> slaisq56s 8 -> sat48s -> trunca32q48;
 * exact fixed-point saturation/rounding semantics remain unvalidated. */
/* [FLIX] Unresolved SIMD semantics: declaration only, not a no-op substitute. */
extern void gain_control(const int32_t *src, int word_offset, int32_t *dst,
                         int channels, int32_t effective_gain);

/* @0x600883ec */
int gcd(int a, int b)
{
    return b == 0 ? a : gcd(b, a % b);
}

/* 0x60088400: reads +0x114/+0x130/+0x134/+0x138/+0x158 (target,
 * stop-after, start, length, fade-type). [FLIX] Float/fixed curve bundles from
 * 0x6008841e remain unresolved. Declaration only; not counted as recovered. */
extern void gain_set_fade_gain(struct gain_module *m);

/* @0x600886e8: re-send the stored FIRST_AU downstream when the mute state flips.
 * muted=1 sends the "acmod=2" copy, muted=0 sends the original event. */
void gain_resend_first_event_in_transition(struct gain_module *m, int muted)
{
    if (gain_log_mask & 8)
        logmm_printl(gain_log, 3, "gain_resend_first_event_in_transition", 0x165,
                     "%s : Send %s to fader..\n", m->name, muted ? "MUTE" : "UNMUTE");
    if (!m->first || !m->first_muted)
        return;
    aud_module_push_event(m, 0, aud_object_get(muted ? m->first_muted : m->first));
    aud_module_sync(m, 0);
}

/* ------------------------------------------------------------------------
 * gain_handle_nonpcm (inlined into gain_chain, see header)
 * Returns gain_chain's return value. The caller has already computed
 * muted = mute-input || mute-output || (uint32_t)current_gain < 4.
 * 0x6008893e loads current_gain; the bundle at 0x60088941 is bltui a14,4.
 * Gain values 0..3 therefore mute ES even with both mute properties clear;
 * a negative gain is NOT below this unsigned threshold.
 * ---------------------------------------------------------------------- */
static void es_finish(struct gain_module *m, int muted, bool transition)
{
    if (transition)                       /* 0x60088d50: beqi muted,1 -> mute event */
        gain_resend_first_event_in_transition(m, muted == 1);
    if (transition)
        m->prev_muted = muted;            /* 0x60088d60; equal already otherwise */
    m->delay_changed = false;
}

static inline int gain_handle_nonpcm(struct gain_module *m, struct aud_data *d, int muted)
{
    const bool transition = muted != m->prev_muted;          /* 0x600888b1..0x600888bc */

    /* 0x6008889e/0x600888a6: decoded beqi type,2 / beqi type,3. */
    if (d->type == AUD_DATA_META) {
        /* 0x60088a6a beqi muted,1; 0x60088a85 bnei muted,1 -> push. */
        logmm_printl(gain_log, 4, "gain_handle_nonpcm", 0x178, "META drop? %s",
                     muted == 1 ? "YES" : "NO");
        if (muted == 1) {
            aud_object_put(d);
            return 0;
        }
        aud_module_push_data(m, 0, d);
        return 0;
    }
    if (d->type == AUD_DATA_META_EXT) {                       /* 0x60088b20 */
        logmm_printl(gain_log, 4, "gain_handle_nonpcm", 0, "bypass meta ext");
        aud_module_push_data(m, 0, d);
        return 0;
    }
    if (d->type != AUD_DATA_ES) {                             /* 0x600888ae bnei 1 */
        aud_module_push_data(m, 0, d);
        return 0;
    }

    if (!m->es_mode) {
        /* 0x60088d6d: ES while the stream was NOT announced as ES (+0x13c == 0).
         * 0x60088d7a beqi muted,1; 0x60088d95 bnei muted,1:
         * YES -> aud_object_put (0x60088d9d), NO -> push unchanged (0x60088e78). */
        logmm_printl(gain_log, 4, "gain_handle_nonpcm", 0x18a, "ES drop? %s",
                     muted == 1 ? "YES" : "NO");
        if (muted == 1)
            aud_object_put(d);
        else
            aud_module_push_data(m, 0, d);
        es_finish(m, muted, transition);
        return 0;
    }

    /* ES-only stream (es_mode). Muting turns ES into same-size zero buffers, but only on
     * packet boundaries (es_pkt_pos == 0). With a valid positive unit count,
     * unmute resumes original bytes at the next boundary; it never holds data
     * waiting for the boundary, only sends zeros until then. No unit-count
     * validation is present; zero would reach the signed remainder divisor. */
    if (muted == 1) {                                         /* decoded beqi @0x600888c5 */
        if (m->es_zero_state == 0 && m->es_pkt_pos == 0)      /* 0x60088fe5 */
            m->es_zero_state = 2;
        if (m->es_zero_state == 2)
            logmm_printl(gain_log, 4, "gain_handle_nonpcm", 0x19a,
                         "MUTE : ES ONLY.. so push zero data");
        else
            logmm_printl(gain_log, 4, "gain_handle_nonpcm", 0x19d,
                         "MUTE : ES ONLY.. wait until sending whole packet.. %d/%d",
                         m->es_pkt_pos + 1, m->es_unit_per_packet);
    } else if (m->es_zero_state == 2) {                       /* decoded beqi @0x600888d2 */
        if (m->es_pkt_pos == 0) {                             /* 0x60088ec8 */
            logmm_printl(gain_log, 4, "gain_handle_nonpcm", 0x1a5,
                         "UNMUTE : ES ONLY.. so stop sending zero data");
            m->es_zero_state = 0;
        } else {
            logmm_printl(gain_log, 4, "gain_handle_nonpcm", 0x1aa,
                         "UNMUTE : ES ONLY.. wait until passing whole packet.. %d/%d",
                         m->es_pkt_pos + 1, m->es_unit_per_packet);
        }
    } else {
        logmm_printl(gain_log, 4, "gain_handle_nonpcm", 0x1af,
                     "ES ONLY.. push packets.. %d/%d", m->es_pkt_pos + 1, m->es_unit_per_packet);
        m->es_zero_state = 0;                                 /* 0x600888e6 */
    }

    if (m->es_zero_state != 0) {                              /* 0x600888e9 */
        struct aud_data *z = aud_data_new_zero(d->size);
        if (!z) {
            logmm_printl(gain_log, 0, "gain_handle_nonpcm", 0x1bd, "no outdata. %d\n", d->size);
            aud_object_put(d);
            return -1;
        }
        z->type = AUD_DATA_ES;                                /* 0x60088d2e */
        aud_object_put(d);
        aud_module_push_data(m, 0, z);
    } else {
        aud_module_push_data(m, 0, d);                        /* 0x60088e5e */
    }
    m->es_pkt_pos = (m->es_pkt_pos + 1) % m->es_unit_per_packet;    /* 0x60088d40 */
    es_finish(m, muted, transition);
    return 0;
}

/* ------------------------------------------------------------------------
 * chain / event / command
 * ---------------------------------------------------------------------- */

/* Inlined "gain_calc_fade_delta" (__func__ 0x6005f3dc): turn an ease request into a
 * sample count and slope. */
static void gain_calc_fade_delta(struct gain_module *m)
{
    m->fade_target = m->target_gain;
    if (m->fadetime_ms == 0) {                                 /* 0x60088ae3 */
        m->fade_gain = m->target_gain;
        logmm_printl(gain_log, 3, "gain_calc_fade_delta", 0,
                     "%s : auto gain transition with 0 ms is done. target fade gain is %d(0x%x)\n",
                     m->name, m->fade_gain, m->fade_gain);
    } else {
        int samples = (int)((int64_t)m->fadetime_ms * SAMPLE_RATE / 1000);
        m->fade_from = m->fade_gain;
        m->delta_s = m->stop_after = m->fade_samples = samples;
        m->delta_g = m->fade_target - m->fade_from;
        if (m->delta_g == 0) {
            m->delta_s = m->stop_after = 0;
        } else if (m->fade_type == 0) {                        /* 0x60088b58: reduce slope */
            int g = gcd(m->delta_s, m->delta_g % m->delta_s);
            m->delta_s /= g;
            m->delta_g /= g;
        }
    }
    m->fade_start_req = GAIN_UNITY;                            /* 0x60088823 */
    m->fadetime_ms = -1;
    m->fade_pos = 0;
    m->gain_changed = true;
}

/* [FLIX] PCM transitions/fade arithmetic remain unresolved. */
extern int gain_chain_pcm_unrecovered(struct gain_module *, struct aud_data *, int muted);

/* @0x6008874c: ES-complete gain_chain. Steady muted PCM with a settled fade
 * selects aud_data_new_zero(size) @0x600889ca/60088b7b, copies index/timestamp
 * @0x600889da..e8, skips sample writes @0x60088bd3, releases input then pushes
 * zeros @0x60088cd5..d06. FIRST.es does not select this PCM path. Muting is not
 * a PCM drop (except allocation failure); transitions/fade math remain [FLIX]. */
int gain_chain(struct aud_port *port, struct aud_data *d)
{
    struct gain_module *m = (struct gain_module *)port->module;
    int muted;

    if (!m->got_first_data) {                                  /* 0x60088926 */
        m->got_first_data = true;
    }

    aud_mutex_lock(&m->lock);
    if (m->gain != -1) {                                       /* 0x60088780 */
        logmm_printl(gain_log, 3, "gain_chain", 0, "gain is changed. now=0x%x, future=0x%x\n",
                     m->current_gain, m->gain);
        m->gain_changed = true;
        m->current_gain = m->gain;
        m->gain = -1;
    }
    if (m->fadetime_ms >= 0)                                   /* 0x600887a2 */
        gain_calc_fade_delta(m);
    aud_mutex_unlock(&m->lock);

    /* 0x60088847..0x6008894b */
    muted = m->mute_input || m->mute_output || (uint32_t)m->current_gain < 4u;

    if (d->type != AUD_DATA_PCM)                               /* 0x60088898 */
        return gain_handle_nonpcm(m, d, muted);

    if (muted && m->prev_muted == 1 && !m->gain_changed &&
        m->delta_g == 0 && m->fade_gain == m->fade_target && m->channel > 0 &&
        d->size >= sizeof(int32_t) && d->size <= INT32_MAX) {
        struct aud_data *out = aud_data_new_zero(d->size);
        if (!out) {
            aud_object_put(d);
            return -1;
        }
        out->index = d->index;
        out->ts = d->ts;
        aud_object_put(d);
        aud_module_push_data(m, 0, out);
        m->final_gain = 0;
        m->prev_muted = muted;
        m->gain_changed = false;
        m->delay_changed = false;
        return 0;
    }

    /* PCM scalar gates decoded (tools/xtobjdump.sh):
     * 60088955: delay_changed && prev_muted==1 && !muted skips one unmute,
     *           keeping prev_muted for this PCM buffer; does NOT affect ES.
     * 600889c2: current_gain==UNITY enters further fast-path tests at 60088f13:
     *           fade_gain==UNITY, delta_g==0, !gain_changed, !prev_muted, !muted
     *           => forward original, final_gain=UNITY.
     * 600889ec: signed word-count <1 skips frame processing.
     * 60088bb5: processed-word-offset >= total-word-count exits frame loop.
     * 60088d21 stores a5, the last effective Q23 gain, NOT the mute boolean.
     * [FLIX] Per-frame fade math and SIMD rounding remain unresolved. */
    return gain_chain_pcm_unrecovered(m, d, muted);
}

/* @0x60089084: FIRST reads es @0x60089173, not codec, to select ES zero/drop
 * policy. It caches/replaces FIRSTs and channels but does not reset gain/fades,
 * mute properties, or ES packet position/zero state. No mute snapshot/restore. */
int gain_event(struct gain_module *m, struct aud_port *port, struct aud_event *e)
{
    if (gain_log_mask & 8)
        logmm_printl(gain_log, 3, "gain_event", 0, "%s : port%d, event %s",
                     m->name, port ? port->index : 0xff, "");
    if (!port)
        return aud_module_default_event(m, port, e);

    if (e->type == AUD_EVENT_FIRST_AU) {                       /* 0x6008913e */
        int need_mute = 1;
        if (!m->mute_input && !m->mute_output) {
            /* Both decoded bltui <4 (unsigned); pending -1 is not a mute.
             * If neither gain is <4, prev_muted==1 keeps the muted FIRST. */
            need_mute = (uint32_t)m->current_gain < 4u ||
                        (uint32_t)m->gain < 4u || m->prev_muted == 1;
        }
        logmm_printl(gain_log, 3, "gain_event", 0x2ae, "got first event\n");
        m->channel = aud_event_value_get_int(e, "channels", 2);
        m->es_mode = aud_event_value_get_bool(e, "es", false);           /* 0x60089173 */
        if (m->es_mode)
            logmm_printl(gain_log, 3, "gain_event", 0x2b5, "got es only mode codec %s\n",
                         aud_event_value_get_str(e, "codec", ""));
        if (m->first)
            aud_object_put(m->first);
        if (m->first_muted)
            aud_object_put(m->first_muted);
        m->first = aud_object_get(e);
        m->first_muted = aud_event_dup(e);
        if (!m->first_muted) {
            logmm_printl(gain_log, 0, "gain_event", 0x2d1,
                         "error in duplicate first event (insufficient memory?)");
        } else {
            aud_event_value_set_int(m->first_muted, "acmod", 2);
            if (need_mute == 1) {
                logmm_printl(gain_log, 3, "gain_event", 0x2c9,
                             "mute.. so modify first event ( acmod %d -> 2)",
                             aud_event_value_get_int(e, "acmod", 0));
                aud_object_put(e);
                return aud_module_default_event(m, port, aud_object_get(m->first_muted));
            }
        }
    }

    if (e->type == AUD_EVENT_LAST_AU) {                        /* decoded beqi type,2 @0x600890d0 */
        logmm_printl(gain_log, 3, "gain_event", 0x2d8, "port%d, last event\n", port->index);
        m->got_first_data = false;
    }

    if (e->type == AUD_EVENT_CUSTOM && aud_event_value_get_bool(e, "delay-changed", false)) {
        bool is_delay = aud_event_value_get_bool(e, "is-delay", false);
        if (aud_event_value_get_bool(e, "es", false)) {
            m->es_unit_per_packet = is_delay ? aud_event_value_get_int(e, "unit-per-packet", 6) : 1;
            m->es_pkt_pos = 0;
        } else if (is_delay) {
            logmm_printl(gain_log, 3, "gain_event", 0x2f8, "got delay-changed in pcm mode\n");
            m->delay_changed = true;
            aud_object_put(e);
            return 0;
        }
    }
    return aud_module_default_event(m, port, e);
}

/* @0x6008930c: gain_command */
int gain_command(struct gain_module *m, uint32_t cmd, int32_t *arg, unsigned len)
{
    /* With debug bit 5 set it first hex-dumps arg ("dump_arg", line 0x327). */
    logmm_printl(gain_log, 3, "gain_command", 0x339, "command %08x\n", cmd);
    switch (cmd) {
    case CMD_NOP_A:
    case CMD_NOP_B:
    case CMD_NOP_C:
        return 0;
    case CMD_SET_GAIN:                                          /* decoded beq cmd,0x66000004 @0x60089375 */
        m->gain = arg[0];
        return 0;
    case CMD_SET_CHANNEL:
        m->channel = arg[0];
        return 0;
    default:
        return aud_module_default_command(m, cmd, arg, len);
    }
}

/* @0x600894a0 */
int gain_constructor(struct gain_module *m)
{
    logmm_printl(gain_log, 3, "gain_constructor", 0x357, "create..\n");
    m->channel = 2;
    m->fade_start_req = m->target_gain = m->fade_target = m->fade_gain = GAIN_UNITY;
    m->gain = -1;
    m->final_gain = m->current_gain = 0;
    m->mute_input = m->mute_output = true;
    m->fadetime_ms = -1;
    m->prev_muted = 1;
    m->gain_changed = false;
    m->first = m->first_muted = NULL;
    m->es_mode = false;
    m->es_zero_state = 0;
    m->es_pkt_pos = 0;
    m->es_unit_per_packet = 1;
    return 0;
}

/* @0x60089510 */
void gain_destructor(struct gain_module *m)
{
    logmm_printl(gain_log, 3, "gain_destructor", 0x37d, "destroy..\n");
    if (m->first)
        aud_object_put(m->first);
    if (m->first_muted)
        aud_object_put(m->first_muted);
    m->first = m->first_muted = NULL;
}

struct aud_prop_desc {
    const char *name;
    int         type;            /* 1 bool, 2 int */
    int         offset;
};

/* @0x60089554: logging only, no side effects */
int gain_prop_updated(struct gain_module *m, const struct aud_prop_desc *p)
{
    const char *base = (const char *)m;
    if (p->type == 2)
        logmm_printl(gain_log, 3, "gain_prop_updated", 0x38e, "%s: updated %s %d",
                     m->name, p->name, *(const int32_t *)(base + p->offset));
    if (p->type == 1)
        logmm_printl(gain_log, 3, "gain_prop_updated", 0x393, "%s: updated %s %s",
                     m->name, p->name, *(base + p->offset) ? "true" : "false");
    return 0;
}

/* @0x600895c4: "{ease-volume}" property setter */
int gain_set_ease_volume(struct gain_module *m, void *unused, void *prop)
{
    (void)unused;
    m->target_gain = aud_prop_get_value_int(prop, "target-gain", m->target_gain);
    m->fade_type = aud_prop_get_value_int(prop, "fade-type", m->fade_type);
    m->fadetime_ms = aud_prop_get_value_int(prop, "fadetime-ms", m->fadetime_ms);
    if (m->fadetime_ms == 0)
        m->fade_start_req = m->target_gain;
    logmm_printl(gain_log, 2, "gain_set_ease_volume", 0x3ae, "%s: target_gain 0x%08x ms %d %d",
                 m->name, m->target_gain, m->fadetime_ms, m->fade_type);
    return 0;
}

/* ------------------------------------------------------------------------
 * Legacy lmodule helper (AdecModule-style buffer API)
 * ---------------------------------------------------------------------- */
struct adec_au_info {
    unsigned size;               /* +0x00 */
    uint8_t  pad[0x18];
    int      data_type;          /* +0x1c: 0 = PCM */
};

struct adec_buf {
    /* +0xb8 */ int (*get_current_info)(int idx, int *offset, struct adec_au_info *au, struct adec_buf *b);
    /* +0xc0 */ int (*read_data)(void **dst, unsigned *size, int flag, struct adec_buf *b);
    /* +0xc4 */ int (*flush_data)(unsigned size, struct adec_buf *b);
};

typedef void (*skip_cb_t)(void *lmod, struct adec_au_info *au, void *data, unsigned *size);

extern volatile uint32_t lmodule_log_mask;       /* *0x4ff90084 */
extern const void *const lmodule_log;            /* 0x60101c40 */

/* @0x60084350: drop (or hand to cb, then drop) every leading non-PCM AU in buf.
 * Name for logs at lmod+0x228. No callx/L32R user exists in dsp0/1/2, only the symbol
 * table entry 0x6011e790. */
int aud_lmodule_skip_nonpcm(void *lmod, struct adec_buf *buf, skip_cb_t cb)
{
    for (;;) {
        int offset = 0;
        struct adec_au_info au;

        if (buf->get_current_info(0, &offset, &au, buf) < 0 || au.data_type == 0)
            return 0;
        if (offset != 0 && (lmodule_log_mask & 1))
            logmm_printl(lmodule_log, 0, "aud_lmodule_skip_nonpcm", 0xb4, "non-zero offset. %d", offset);
        if (lmodule_log_mask & 0x20)
            logmm_printl(lmodule_log, 5, "aud_lmodule_skip_nonpcm", 0xb6, "%s: drop type %d",
                         (const char *)lmod + 0x228, au.data_type);
        if (cb) {
            void *dst = NULL;
            unsigned size = au.size;
            int r = buf->read_data(&dst, &size, 0, buf);
            if (r != 0) {
                if (lmodule_log_mask & 1)
                    logmm_printl(lmodule_log, 0, "aud_lmodule_skip_nonpcm", 0xc0,
                                 "read_data Err (result:%d)", r);
                return -1;
            }
            cb(lmod, &au, dst, &size);
        }
        int r = buf->flush_data(au.size + (unsigned)offset, buf);
        if (r != 0) {
            if (lmodule_log_mask & 1)
                logmm_printl(lmodule_log, 0, "aud_lmodule_skip_nonpcm", 0xca,
                             "flush_data Err (result:%d)", r);
            return -1;
        }
    }
}
