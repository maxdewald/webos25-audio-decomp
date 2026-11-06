/*
 * dsp1 "output" module (instances output_arc / output_spdif / output_hp / output_speaker):
 * first-event handling (codec id, writer choice, IEC channel status, restart,
 * "output_status") and the buffer-controller lane writers (ES/HBR and PCM up/down-mix).
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp1.bin
 *   sha256 8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4
 * Cadence HiFi (RG-2018.9), main segment linked at 0x60000000 (file offset 0x5b0).
 * Symbol table pairs are (func_ptr, name_ptr) at 0x602bfb08. Each was checked against
 * the function's own logmm "__func__" string. Every op0=0xE/0xF FLIX bundle is 8 bytes.
 * Control-flow slots rechecked with tools/xtobjdump.sh on the saved main image;
 * validated .w18 branches supersede the earlier flix_annot.py guesses. SIMD/float
 * slots remain unrecovered; these scalar lane writers contain no such slots.
 * Evidence: build/decomp/audio_a0_dsp1.main_60000000.bin.c,
 *           build/scratch/evidence/dsp1-out-event/out_event.lin.asm
 * Corrections to the earlier version of this file: the ES writer test is
 * channels == 2 -> cpy_es (not channels > 2). Channels, rate and bps are stored before
 * the change test, which compares against the values from before the event. Codec
 * strings are tried in firmware order. ARC 2-channel PCM uses cpy_00. Non-arc PCM uses
 * the 9-entry in/out table. A zero pre-fill is queued on FIFO outputs. FIRST/LAST still
 * reach aud_module_default_event.
 */
#include <string.h>
#include "../include/lg_audio_policy.h"
#include "out_module.h"

/* ---- buffer-controller lane writers (m->bc_writer, called by fill_bc) ---------------
 * dst_bytes is the BC byte count for lane 0. Lane k is at dst + k * cfg->bc_size. The
 * return value is the number of SOURCE bytes consumed.
 * ES writers take 16-bit IEC 61937 words and put each one, byte-swapped, into bits
 * 8..23 of a 32-bit slot: (w & 0xff) << 16 | (w & 0xff00). Nothing is added: no Pa/Pb
 * preamble, no burst padding, no validity or channel-status bits. The input must already
 * be complete IEC 61937 bursts in big-endian word order.
 * ------------------------------------------------------------------------------------ */

static inline uint32_t es_slot(uint16_t w)
{
    return (uint32_t)(w & 0xff) << 16 | (w & 0xff00);
}

static inline uint32_t *lane(struct out_module *m, void *dst, int k)
{
    return (uint32_t *)((uint8_t *)dst + (size_t)k * m->cfg->bc_size);
}

/* @0x601229ac cpy_es (functions.tsv: "output_status"): 2-channel ES, one lane, one
 * 16-bit word per 32-bit slot. */
int cpy_es(struct out_module *m, void *dst, const void *src, unsigned dst_bytes)
{
    (void)m;
    uint32_t *d = dst;
    const uint16_t *s = src;
    unsigned n = dst_bytes >> 2;
    for (unsigned i = 0; i < n; i++)
        d[i] = es_slot(s[i]);
    return (int)(n << 1);
}

/* @0x60122a38 cpy_es8 (functions.tsv: "output_spdif"): 8-channel HBR ES. Each 8-word
 * frame goes to 4 lanes x 2 slots (words 0-1 lane 0, 2-3 lane 1, 4-5 lane 2, 6-7 lane 3).
 * xtobjdump 0x60122a46: frames=dst_bytes>>3; 0x60122acd advances source by 16;
 * 0x60122aec returns frames<<4. A lane byte is NOT a source byte (ratio 1:2).
 * These are scalar loads/stores; neither Pa/Pb synthesis nor SIMD is hidden here. */
int cpy_es8(struct out_module *m, void *dst, const void *src, unsigned dst_bytes)
{
    uint32_t *l0 = lane(m, dst, 0), *l1 = lane(m, dst, 1);
    uint32_t *l2 = lane(m, dst, 2), *l3 = lane(m, dst, 3);
    const uint16_t *s = src;
    unsigned frames = dst_bytes >> 3;
    for (unsigned f = 0; f < frames; f++, s += 8) {
        l0[2 * f] = es_slot(s[0]); l0[2 * f + 1] = es_slot(s[1]);
        l1[2 * f] = es_slot(s[2]); l1[2 * f + 1] = es_slot(s[3]);
        l2[2 * f] = es_slot(s[4]); l2[2 * f + 1] = es_slot(s[5]);
        l3[2 * f] = es_slot(s[6]); l3[2 * f + 1] = es_slot(s[7]);
    }
    return (int)(frames << 4);
}

/* @0x60122af8: 8 x 32-bit PCM -> 4 lanes x 2 slots, unchanged. */
int cpy_8(struct out_module *m, void *dst, const void *src, unsigned dst_bytes)
{
    const uint32_t *s = src;
    unsigned frames = dst_bytes >> 3;
    for (unsigned f = 0; f < frames; f++, s += 8)
        for (int k = 0; k < 4; k++) {
            lane(m, dst, k)[2 * f]     = s[2 * k];
            lane(m, dst, k)[2 * f + 1] = s[2 * k + 1];
        }
    return (int)(frames << 5);
}

/* @0x60122bf8: like cpy_8 but lane 1 gets words 3,2 (C and LFE swapped). */
int cpy_8_swap_c_lfe(struct out_module *m, void *dst, const void *src, unsigned dst_bytes)
{
    const uint32_t *s = src;
    unsigned frames = dst_bytes >> 3;
    for (unsigned f = 0; f < frames; f++, s += 8) {
        lane(m, dst, 0)[2 * f] = s[0]; lane(m, dst, 0)[2 * f + 1] = s[1];
        lane(m, dst, 1)[2 * f] = s[3]; lane(m, dst, 1)[2 * f + 1] = s[2];
        lane(m, dst, 2)[2 * f] = s[4]; lane(m, dst, 2)[2 * f + 1] = s[5];
        lane(m, dst, 3)[2 * f] = s[6]; lane(m, dst, 3)[2 * f + 1] = s[7];
    }
    return (int)(frames << 5);
}

/* Interleaved single-lane up/down-mix: copy `keep` of `in` input words into an `out`-word
 * frame and zero-fill the rest. Inlined into each cpy_IO writer. */
static inline int remix(void *dst, const void *src, unsigned dst_bytes, int in, int out, int keep)
{
    uint32_t *d = dst;
    const uint32_t *s = src;
    unsigned frames = dst_bytes / (unsigned)(out * 4);
    for (unsigned f = 0; f < frames; f++, d += out, s += in)
        for (int i = 0; i < out; i++)
            d[i] = i < keep ? s[i] : 0;
    return (int)(frames * (unsigned)in * 4);
}

/* @0x60122cf4: 2 -> 4 */
int cpy_24(struct out_module *m, void *dst, const void *src, unsigned n)
{ (void)m; return remix(dst, src, n, 2, 4, 2); }
/* @0x60122d64: 2 -> 8 */
int cpy_28(struct out_module *m, void *dst, const void *src, unsigned n)
{ (void)m; return remix(dst, src, n, 2, 8, 2); }
/* @0x60122dd0: 4 -> 2 (L/R only) */
int cpy_42(struct out_module *m, void *dst, const void *src, unsigned n)
{ (void)m; return remix(dst, src, n, 4, 2, 2); }
/* @0x60122e2c: 4 -> 8 */
int cpy_48(struct out_module *m, void *dst, const void *src, unsigned n)
{ (void)m; return remix(dst, src, n, 4, 8, 4); }
/* @0x60122ea4: 8 -> 2 (L/R only) */
int cpy_82(struct out_module *m, void *dst, const void *src, unsigned n)
{ (void)m; return remix(dst, src, n, 8, 2, 2); }
/* @0x60122f30: 8 -> 4 (first 4) */
int cpy_84(struct out_module *m, void *dst, const void *src, unsigned n)
{ (void)m; return remix(dst, src, n, 8, 4, 4); }

/* @0x60122f90: plain memcpy (same in/out layout). */
int cpy_00(struct out_module *m, void *dst, const void *src, unsigned dst_bytes)
{
    (void)m;
    memcpy(dst, src, dst_bytes);
    return (int)dst_bytes;
}

/* ---- first-event handler ------------------------------------------------------------ */

/* Inlined ceil(log2(x)) used for both stride shifts. */
static inline int ceil_log2(int x)
{
    int n = 0;
    if (x > 0)
        x -= 1;
    while (x) {
        n++;
        x >>= 1;
    }
    return n;
}

/* Inlined strncmp ladder (0x60123982..0x60123e36). Lengths are the literal lengths, so
 * these are prefix matches. "dts_hd_ma" is tried before "dts_ma"/"dts_hd"/"dts", so
 * the longest name wins. Anything else stays PCM (0). There is no DTS handling beyond
 * this label: the payload goes through cpy_es/cpy_es8 like any other ES. */
static inline int out_codec_from_str(const char *c)
{
    if (!strncmp(c, "pcm", 3))       return OUT_CODEC_PCM;
    if (!strncmp(c, "ac3", 3))       return OUT_CODEC_AC3;
    if (!strncmp(c, "eac3", 4))      return OUT_CODEC_EAC3;
    if (!strncmp(c, "aac", 3))       return OUT_CODEC_AAC;
    if (!strncmp(c, "mat", 3))       return OUT_CODEC_MAT;
    if (!strncmp(c, "dts_hd_ma", 9)) return OUT_CODEC_DTS_MA;      /* 7 */
    if (!strncmp(c, "dtsx_p1", 7))   return OUT_CODEC_DTSX_P1;     /* 8 */
    if (!strncmp(c, "dtsx_p2", 7))   return OUT_CODEC_DTSX_P2;     /* 9 */
    if (!strncmp(c, "dts_ma", 6))    return OUT_CODEC_DTS_MA;      /* 7 */
    if (!strncmp(c, "dts_hd", 6))    return OUT_CODEC_DTS_HD;      /* 6 */
    if (!strncmp(c, "dts", 3))       return OUT_CODEC_DTS;         /* 5 */
    return OUT_CODEC_PCM;
}

struct out_pcm_map { out_bc_writer fn; int in_ch, out_ch; };

/* @0x600067f0 (copied to the stack per event): writer per (input channels, output
 * slots). Used for every output except output_arc. */
static const struct out_pcm_map out_pcm_map[9] = {
    { cpy_00, 2, 2 }, { cpy_24, 2, 4 }, { cpy_28, 2, 8 },
    { cpy_42, 4, 2 }, { cpy_00, 4, 4 }, { cpy_48, 4, 8 },
    { cpy_82, 8, 2 }, { cpy_84, 8, 4 }, { cpy_00, 8, 8 },
};

/* 16-byte "output_status" payload (sp+0x20 in out_event). Read by the kernel's
 * _cb_output_status. */
struct out_status_msg {
    int32_t codec;      /* enum dsp1_out_codec 0..9 */
    int32_t channels;
    int32_t data_rate;
    int32_t acmod;
};

/* @0x60123814 out_event. Event types: 1 = FIRST, 2 = LAST, 0xd = custom ("is_first_data").
 * FIRST and LAST are also passed on to aud_module_default_event. Custom events are
 * consumed. xtobjdump 0x60123aa4..abf compares OLD codec/rate/channels and PCM acmod;
 * bps, ES flag and C/LFE mapping alone do not trigger restart. Writer changes before
 * stop/start (0x60123b11); neither this handler nor stop clears old FIFO nodes.
 * 0x60123d23 is beq.w18 output slots, not the old inferred bany table match. */
int out_event(struct out_module *m, struct aud_port *port, struct aud_event *ev)
{
    int old_codec = m->codec, old_rate = m->data_rate;
    int old_ch = m->channels, old_acmod = m->acmod;
    int type = ev->type;

    if (type == AUD_EVENT_FIRST) {                    /* 0x60123830 beqi.w18 1 */
        struct out_status_msg st = { OUT_CODEC_PCM, 0, 0, 0 };
        const char *codec = aud_event_value_get_str(ev, "codec", "pcm");
        st.channels  = aud_event_value_get_int(ev, "channels", 2);
        st.data_rate = aud_event_value_get_int(ev, "data-rate", 48000);
        st.acmod     = aud_event_value_get_int(ev, "acmod", 2);
        m->es               = aud_event_value_get_bool(ev, "es", false);
        m->ch_mapping_lfe_c = aud_event_value_get_bool(ev, "ch-mapping-lfe-c", false);
        m->channels         = st.channels;
        m->bits_per_sample  = aud_event_value_get_int(ev, "bits-per-sample", 32);
        m->latency_us       = 0;
        m->data_rate        = st.data_rate;

        if (m->es) {
            /* Codec is only parsed for ES. */
            st.codec = out_codec_from_str(codec);
            /* 0x6012399b beqi.w18 channels, 2: 2 -> one lane, anything else -> 4 lanes. */
            m->bc_writer = m->channels == 2 ? cpy_es : cpy_es8;
            if (strncmp(m->name, "output_spdif", 12) == 0) {
                REG(AUD_REGS, 0x324) |= 1;            /* S/PDIF validity = non-audio */
                HAL_SetSpdifEsMode(st.data_rate);
            } else if (strncmp(m->name, "output_arc", 10) == 0) {
                HAL_SetIEC1958ValidBit(1, 5);         /* eARC validity = non-audio */
                HAL_SetArcEsMode(st.data_rate);       /* 768000 -> fs code 9 (HBR) */
            }
            m->stride_shift = ceil_log2(m->channels * m->bits_per_sample / 8);
            m->output_stride_shift = ceil_log2(m->stride_ch * 32 / 8);
            /* "%8s: start %s, codec %d, %dch, %drate, %dbpp, stride_shift %d, output_stride_shift %d" */
        } else {
            bool is_arc = false;
            if (strncmp(m->name, "output_spdif", 12) == 0) {
                REG(AUD_REGS, 0x324) &= ~1u;
                REG(AUD_REGS, 0x330) &= ~2u;
                REG(AUD_REGS, 0x334) = (REG(AUD_REGS, 0x334) & ~0xfu) | 2;
            } else if (strncmp(m->name, "output_arc", 10) == 0) {
                HAL_SetIEC1958ValidBit(0, 5);
                HAL_SetArcPcmMode();
            }
            m->stride_shift = ceil_log2(m->channels * m->bits_per_sample / 8);
            m->output_stride_shift = ceil_log2(m->stride_ch * 32 / 8);
            is_arc = strncmp(m->name, "output_arc", 10) == 0;

            if (is_arc) {
                if (m->channels == 2) {               /* 0x60123c25 beqi.w18 2 */
                    m->bc_writer = cpy_00;
                } else if (m->ch_mapping_lfe_c) {
                    m->bc_writer = cpy_8;
                } else {
                    m->bc_writer = cpy_8_swap_c_lfe;
                    aud_event_value_set_int(ev, "cap-ch-remap", 1);
                }
            } else {
                m->bc_writer = NULL;
                for (int i = 0; i < 9; i++) {
                    if (out_pcm_map[i].in_ch == m->channels &&
                        out_pcm_map[i].out_ch == m->stride_ch) {   /* 0x60123d23 beq.w18 */
                        m->bc_writer = out_pcm_map[i].fn;
                        break;
                    }
                }
                if (!m->bc_writer) {
                    /* "%8s: unknown in/out channel. %d/%d" */
                    m->bc_writer = cpy_00;
                }
            }
            /* "%8s: start %s, %dch, %dbpp, stride_shift %d, output_stride_shift %d" */
        }

        m->codec = st.codec;
        m->acmod = st.acmod;

        /* Restart on codec / rate / channel change, or an acmod change while PCM. */
        bool changed = st.codec != old_codec || st.data_rate != old_rate ||
                       st.channels != old_ch ||
                       (st.acmod != old_acmod && st.codec == OUT_CODEC_PCM);
        if (changed) {
            out_stop(m);
            out_start(m);
        }

        if (m->cfg->bc_size == 0) {
            /* FIFO output: top the queue up with silence. */
            uint32_t ps = irq_save();
            int zero = m->max_len - m->queued;
            irq_restore(ps);
            /* "%8s: max %d, queued %d, zero %d" */
            if (zero >= 1) {                          /* 0x60123d78 blti.w18 1 */
                struct aud_data *z = aud_data_new_zero((unsigned)zero << m->stride_shift);
                /* "%8s: zero_data %p, stride_shift %d" */
                if (z)
                    out_chain(port, z);
            }
        }

        if (changed && (strncmp(m->name, "output_spdif", 12) == 0 ||
                        strncmp(m->name, "output_arc", 10) == 0))
            aimc_raise_named_event(m->aimc_ctx, m->aimc_id, "output_status", &st, sizeof(st));
        type = ev->type;
    }

    if (type == AUD_EVENT_LAST) {
        /* "%8s: stop" */
        m->latency_us = 0;
        out_stop(m);
        aimc_raise_named_event(m->aimc_ctx, m->aimc_id, "last", NULL, 0);
        type = ev->type;
    }

    if (type != AUD_EVENT_CUSTOM)
        return aud_module_default_event(m, port, ev);

    /* "%8s: received custom event" */
    if (aud_event_has_name(ev, "is_first_data")) {
        /* "%8s: get is_first_data event" */
        m->first_data = true;
        m->irq_time = 0;
        m->write_time = 0;
        m->first_write_time = 0;
    }
    aud_object_put(ev);
    return 0;
}
