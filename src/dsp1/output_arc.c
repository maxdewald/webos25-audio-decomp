/*
 * dsp1 "output" module (instances output_arc / output_spdif / output_hp / output_speaker):
 * first-event handling, ES/HBR lane writers and eARC channel-status setup.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp1.bin
 *   sha256 8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4
 * Cadence HiFi (RG-2018.9), main segment linked at 0x60000000 (file offset 0x5b0).
 * Symbol table pairs are (func_ptr, name_ptr) -- verified by each function's own
 * logmm "__func__" string. Every op0=0xE/0xF FLIX bundle is 8 bytes and OPAQUE, so
 * conditions hidden inside bundles are marked [FLIX].
 * Evidence: ../../decomp/audio_a0_dsp1.main_60000000.bin.c,
 *           ../../evidence/dsp1-out-event/out_event.lin.asm
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../include/lg_audio_policy.h"

struct out_cfg {                 /* module +0xf0 */
    const char *name;            /* "output_arc", ... */
    uint32_t    lane_stride;     /* +0x44: byte distance between the 4 I2S lane buffers */
};

struct out_module {
    const char *name;
    struct out_cfg *cfg;
    int  (*writer)(struct out_module *, uint32_t *dst, const void *src, unsigned bytes); /* +0x130 */
    int   channels;              /* +0x100 */
    int   bits_per_sample;       /* +0x104 */
    int   data_rate;             /* +0x110 */
    bool  ch_mapping_lfe_c;      /* +0x118 */
    bool  es;                    /* +0x164 */
    int   codec;                 /* enum dsp1_out_codec */
    int   acmod;
};

extern int  aud_event_value_get_int(void *ev, const char *key, int def);
extern bool aud_event_value_get_bool(void *ev, const char *key, bool def);
extern const char *aud_event_value_get_str(void *ev, const char *key, const char *def);
extern void aud_event_value_set_int(void *ev, const char *key, int v);
extern void aimc_raise_named_event(const char *name, const void *payload, int len);
extern void out_stop(struct out_module *m);
extern void out_start(struct out_module *m);
extern void HAL_SetIEC1958ValidBit(int valid, int port, int x);
extern void HAL_SetArcPcmMode(void);
extern void HAL_SetSpdifEsMode(int data_rate);
extern volatile uint32_t *aud_regs;           /* DAT_600e924c */
extern volatile uint32_t *arc_lane_regs;      /* DAT_600e9248 */

/* ---- lane writers (selected into m->writer) ------------------------------ */

/* @0x60122a38 cpy_es8: 8 x 16-bit words -> 4 lanes x 2 slots, each word byte-swapped
 * into bits 8..23 ((w & 0xff) << 16 | (w & 0xff00)). The 8-channel HBR/ES path, so the
 * input must be IEC 61937 in big-endian 16-bit words. */
static int cpy_es8(struct out_module *m, uint32_t *lane0, const uint16_t *src, unsigned bytes)
{
    uint32_t *l1 = (uint32_t *)((char *)lane0 + m->cfg->lane_stride);
    uint32_t *l2 = (uint32_t *)((char *)l1 + m->cfg->lane_stride);
    uint32_t *l3 = (uint32_t *)((char *)l2 + m->cfg->lane_stride);
    unsigned frames = bytes >> 3;
#define SW(w) ((uint32_t)((w) & 0xff) << 16 | ((w) & 0xff00))
    for (unsigned f = 0; f < frames; f++, src += 8) {
        lane0[2 * f] = SW(src[0]); lane0[2 * f + 1] = SW(src[1]);
        l1[2 * f]    = SW(src[2]); l1[2 * f + 1]    = SW(src[3]);
        l2[2 * f]    = SW(src[4]); l2[2 * f + 1]    = SW(src[5]);
        l3[2 * f]    = SW(src[6]); l3[2 * f + 1]    = SW(src[7]);
    }
#undef SW
    return (int)(frames << 4);
}

/* @0x601229ac cpy_es: 2-channel byte-swapping ES writer (single lane). */
static int cpy_es(struct out_module *m, uint32_t *dst, const uint16_t *src, unsigned bytes)
{
    (void)m;
    unsigned n = bytes >> 2;
    for (unsigned i = 0; i < n * 2; i++)
        dst[i] = (uint32_t)(src[i] & 0xff) << 16 | (src[i] & 0xff00);
    return (int)(n << 1);
}

/* @0x60122af8: 8 x 32-bit PCM -> 4 lanes x 2 slots, unchanged. */
extern int cpy_8(struct out_module *, uint32_t *, const void *, unsigned);
/* @0x60122bf8: same, swapping C and LFE; out_event then sets "cap-ch-remap"=1. */
extern int cpy_8_swap_c_lfe(struct out_module *, uint32_t *, const void *, unsigned);
/* @0x60122f90: debug sink (AuiLogger_AppendData). */
extern int cpy_00(struct out_module *, uint32_t *, const void *, unsigned);

/* ---- eARC channel-status / lane-rate programming ------------------------- */

/* @0x60121588. Writes the same lane code into 4 per-lane registers
 * (+0x134/+0x154/+0x174/+0x194) and sets bit1 in +0x130/+0x150/+0x170/+0x190.
 * Thresholds are literal-pool constants. Some branches cross [FLIX] bundles, so other
 * codes may exist; these are the visible ones. */
void HAL_SetArcEsMode(int data_rate)
{
    uint32_t code;
    if (data_rate > 22049) {
        if (data_rate > 88199)
            code = (data_rate == 768000) ? 9 : 2;      /* 768 kHz HBR -> lane code 9 */
        else
            code = (data_rate <= 44100) ? 0 : 2;
    } else {
        code = (data_rate == 768) ? 9 : 2;            /* kHz-scaled caller */
    }
    for (int lane = 0; lane < 4; lane++) {
        arc_lane_regs[(0x130 + lane * 0x20) / 4] |= 2;
        arc_lane_regs[(0x134 + lane * 0x20) / 4] =
            (arc_lane_regs[(0x134 + lane * 0x20) / 4] & 0x0ffffff0) | code;
    }
}

/* ---- first-event handler ------------------------------------------------- */

/* Inlined into out_event's first-event blocks. */
static int codec_from_str(const char *c)
{
    static const struct { const char *s; int id; } t[] = {
        { "pcm", OUT_CODEC_PCM },        { "ac3", OUT_CODEC_AC3 },
        { "eac3", OUT_CODEC_EAC3 },      { "aac", OUT_CODEC_AAC },
        { "mat", OUT_CODEC_MAT },        { "dts_hd_ma", OUT_CODEC_DTS_MA },
        { "dtsx_p1", OUT_CODEC_DTSX_P1 },{ "dtsx_p2", OUT_CODEC_DTSX_P2 },
        { "dts_ma", OUT_CODEC_DTS_MA },  { "dts_hd", OUT_CODEC_DTS_HD },
        { "dts", OUT_CODEC_DTS },
    };
    for (unsigned i = 0; i < sizeof(t) / sizeof(t[0]); i++)
        if (strncmp(c, t[i].s, strlen(t[i].s)) == 0)
            return t[i].id;
    return OUT_CODEC_PCM;
}

/* @0x60123814 out_event (+ synthetic blocks 0x601238f4 .. 0x60123de2).
 * Event type 1 = FIRST, 2 = LAST, 0xd = custom ("is_first_data"). */
int out_event(struct out_module *m, void *port, void *ev, int type)
{
    (void)port;
    if (type == 1) {
        const char *codec = aud_event_value_get_str(ev, "codec", "pcm");
        int ch    = aud_event_value_get_int(ev, "channels", 2);
        int rate  = aud_event_value_get_int(ev, "data-rate", 48000);
        int acmod = aud_event_value_get_int(ev, "acmod", 2);
        m->es               = aud_event_value_get_bool(ev, "es", false);
        m->ch_mapping_lfe_c = aud_event_value_get_bool(ev, "ch-mapping-lfe-c", false);
        m->bits_per_sample  = aud_event_value_get_int(ev, "bits-per-sample", 32);
        m->channels = ch;
        m->data_rate = rate;

        int new_codec = OUT_CODEC_PCM;
        if (!m->es) {
            if (strcmp(m->name, "output_spdif") == 0) {
                aud_regs[0x324 / 4] &= ~1u;
                aud_regs[0x330 / 4] &= ~2u;
                aud_regs[0x334 / 4] = (aud_regs[0x334 / 4] & ~0xfu) | 2;
            } else if (strcmp(m->name, "output_arc") == 0) {
                HAL_SetIEC1958ValidBit(0, 5, 0);
                HAL_SetArcPcmMode();
                /* ARC PCM keeps C/LFE order only if the producer already did. */
                if (m->ch_mapping_lfe_c) {
                    m->writer = cpy_8;
                } else {
                    m->writer = cpy_8_swap_c_lfe;
                    aud_event_value_set_int(ev, "cap-ch-remap", 1);
                }
            } else {
                m->writer = cpy_00;
            }
        } else {
            new_codec = codec_from_str(codec);
            /* [FLIX] chooses between the 8-ch HBR writer and the 2-ch writer. */
            m->writer = (ch > 2) ? (void *)cpy_es8 : (void *)cpy_es;
            if (strcmp(m->name, "output_spdif") == 0) {
                aud_regs[0x324 / 4] |= 1;
                HAL_SetSpdifEsMode(rate);
            } else if (strcmp(m->name, "output_arc") == 0) {
                HAL_SetIEC1958ValidBit(1, 5, 0);       /* non-audio */
                HAL_SetArcEsMode(rate);
            }
        }

        /* Restart only if codec, data-rate, channels or acmod changed. */
        if (new_codec != m->codec || rate != m->data_rate || ch != m->channels ||
            (acmod != m->acmod && new_codec == OUT_CODEC_PCM)) {
            out_stop(m);
            out_start(m);
            struct { int codec, channels, data_rate, acmod; } st = { new_codec, ch, rate, acmod };
            if (!strcmp(m->name, "output_spdif") || !strcmp(m->name, "output_arc"))
                aimc_raise_named_event("output_status", &st, sizeof(st));
        }
        m->codec = new_codec;
        m->acmod = acmod;
        /* No preamble is generated: ES input must already be complete IEC 61937 bursts. */
    } else if (type == 2) {
        out_stop(m);
        aimc_raise_named_event("last", NULL, 0);
    }
    return 0;
}
