/*
 * dsp1 output HAL: IRQ masking, IEC 60958 valid bit, ARC/eARC lane channel-status
 * (PCM vs non-audio + sampling-frequency code), S/PDIF ES mode, ARC/eARC enables,
 * buffer-controller channel set, cache writeback, interrupt clear.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp1.bin
 *   sha256 8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4
 * Main segment linked at 0x60000000. Rate ladders were decoded with the 18-bit FLIX
 * branch decoder (tools/flix_annot.py): every
 * threshold literal (0x5621/0x5622, 0x7cff/0x7d00, 0xac43/0xac44, 0x15887/0x15888,
 * 0x17700, 0x2b10f/0x2b110, 0x2edff/0x2ee00, 0xbb800 and kHz 0x16/0x18/0x20/0x2c/0x58/
 * 0x60/0xb0/0xc0/0x300) reaches exactly one code, see iec_rate_code() in out_module.h.
 * Evidence: build/decomp/audio_a0_dsp1.main_60000000.bin.c (0x60101b60..0x60102180,
 *           0x601212a0..0x60121754).
 */
#include "out_module.h"

extern void xthal_set_intclear(uint32_t mask);
extern void xthal_dcache_region_writeback(void *p, unsigned len);
extern uint32_t xt_rsr_ps(void);
extern void xt_wsr_ps(uint32_t ps);

/* @0x601212a0: raise PS.INTLEVEL to 3 (unless already higher); returns the old PS. */
uint32_t irq_save(void)
{
    uint32_t ps = xt_rsr_ps();
    if ((ps & 0xf) < 3)
        xt_wsr_ps((ps & ~0xfu) | 3);
    return ps;
}

/* @0x601212c0 */
void irq_restore(uint32_t ps)
{
    xt_wsr_ps(ps);
}

/* @0x601212cc: system time counter high word, forced to 0 on chip rev 0x26a0 with the
 * CHIP_REGS+0x368 flag set. */
uint32_t HAL_GetCurrentStch(void)
{
    if (REG(CHIP_REGS, 0x368) != 0 && REG(CHIP_ID, 0xe8) == 0x26a0)
        return 0;
    return REG(AOC_REGS, 0x80);
}

/* @0x601212f8: IEC 60958 "validity" (non-audio) flag. port 3 = legacy ARC, 5 = eARC. */
void HAL_SetIEC1958ValidBit(int valid, int port)
{
    if (port == 3) {
        uint32_t v = REG(AUD_REGS, 0x3b4) & ~1u;
        REG(AUD_REGS, 0x3b4) = v | (valid == 1);
    } else if (port == 5) {
        uint32_t v = REG(ARC_REGS, 0x124) & ~1u;
        REG(ARC_REGS, 0x124) = v | (valid == 1);
    }
}

/* @0x6012134c: 4 eARC lanes back to linear PCM, 48 kHz. Per lane k:
 * +0x130+0x20k bit1 (non-audio) cleared, +0x134+0x20k low nibble (fs code) = 2. */
void HAL_SetArcPcmMode(void)
{
    for (int k = 0; k < 4; k++)
        REG(ARC_REGS, 0x130 + 0x20 * k) &= ~2u;
    for (int k = 0; k < 4; k++)
        REG(ARC_REGS, 0x134 + 0x20 * k) = (REG(ARC_REGS, 0x134 + 0x20 * k) & ~0xfu) | 2;
}

/* @0x601213e0: S/PDIF channel status for ES: +0x330 bit1 (non-audio), +0x334 low nibble
 * = fs code (the ES data rate, e.g. 192000 -> 0xe for E-AC-3). Mask 0x0ffffff0. */
void HAL_SetSpdifEsMode(int data_rate)
{
    uint32_t code = iec_rate_code(data_rate);
    REG(AUD_REGS, 0x330) |= 2;
    REG(AUD_REGS, 0x334) = (REG(AUD_REGS, 0x334) & 0x0ffffff0u) | code;
}

/* @0x60121588: the same for all 4 eARC lanes. The ES data rate selects the code, so a
 * 768000 Hz HBR stream (DTS-HD MA / TrueHD / MAT) gets 9 and 192000 (E-AC-3) gets 0xe;
 * unknown rates fall back to 2 (48 kHz). The 4 ctl and 4 cs registers are read first and
 * written back afterwards. */
void HAL_SetArcEsMode(int data_rate)
{
    uint32_t code = iec_rate_code(data_rate);
    uint32_t ctl[4], cs[4];
    for (int k = 0; k < 4; k++) {
        ctl[k] = REG(ARC_REGS, 0x130 + 0x20 * k);
        cs[k]  = REG(ARC_REGS, 0x134 + 0x20 * k);
    }
    for (int k = 0; k < 4; k++)
        REG(ARC_REGS, 0x130 + 0x20 * k) = ctl[k] | 2;
    for (int k = 0; k < 4; k++)
        REG(ARC_REGS, 0x134 + 0x20 * k) = (cs[k] & 0x0ffffff0u) | code;
}

/* @0x60101b60: irq < 28 goes through a 28-entry jump table (0x60002d10) of W1C writes to
 * ARC_REGS+0x3a0..0x3ac (bit 1/2/4/8/0x10/0x20 per source); entry 27 clears the
 * per-DSP status words +0x330..0x34c (aud_who_am_i selects the set). Others use
 * xthal_set_intclear. */
void HAL_ClearInterrupt(unsigned irq)
{
    if (irq >= 0x1c) {
        xthal_set_intclear(1u << (irq & 31));
        return;
    }
    /* [GAP] table-driven: register/bit per irq is data in the firmware image. */
}

/* @0x60102064: buffer-controller slot map for 8/4/2 channels. */
void HAL_SetMAOCChannelSet_BufferController(int ch)
{
    if (ch == 8) {
        REG(ARC_REGS, 0x48) = 7;
        REG(ARC_REGS, 0x4c) = 0x54763210;  /* slot order 0,1,2,3,6,7,4,5 */
    } else if (ch == 4) {
        REG(ARC_REGS, 0x48) = 3;
        REG(ARC_REGS, 0x4c) = 0x3210;
    } else {
        REG(ARC_REGS, 0x48) = 1;
        REG(ARC_REGS, 0x4c) = 0x10;
    }
}

/* @0x601020a8 */
void HAL_EnableSpdifCtrl(int en)
{
    uint32_t v = REG(AUD_REGS, 0x324) & ~2u;
    if (en == 1)
        v |= 2;
    REG(AUD_REGS, 0x324) = v | 8;
}

/* @0x601020d0 */
void HAL_EnableArcCtrl(int en, int port)
{
    if (port == 3) {
        uint32_t v = REG(AUD_REGS, 0x3b4) & ~2u;
        REG(AUD_REGS, 0x3b4) = v | (en ? 2u : 0u);
    } else if (port == 5) {
        uint32_t v = REG(ARC_REGS, 0x124) & ~2u;
        REG(ARC_REGS, 0x124) = v | (en ? 2u : 0u);
    }
}

/* @0x6010211c */
void HAL_SetArcCtrl(uint32_t v)
{
    REG(AOC_REGS, 0x24) = v;
}

/* @0x6010212c */
void HAL_SetEarcCtrl(uint32_t v)
{
    REG(AOC_REGS, 0x28) = v;
}

/* @0x6010213c */
void HAL_EnableArcOut(uint32_t v, int port)
{
    if (port == 4)
        REG(ARC_REGS, 0x100) = v;
    else if (port == 5)
        REG(ARC_REGS, 0x120) = v;
    else
        REG(AUD_REGS, 0x3b0) = v;
}

/* @0x60102168 */
void HAL_CacheRegionWriteback(void *p, unsigned len)
{
    xthal_dcache_region_writeback(p, len);
}
