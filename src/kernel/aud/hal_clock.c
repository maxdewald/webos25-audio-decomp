// SPDX-License-Identifier: GPL-2.0-only
/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2020 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/*
 * ARC sample-rate callback from clock.c, O22 HAL, kernel 5.4.268-329.ptl4tv.5.
 * Image SHA256: 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * The settings lookup and aud_set_earc_frpll are INLINED in the tagged body.
 * All nine 28-byte settings rows @0xffffffc0122d31e0 match LG's public table.
 * The HAL powers down/reprograms/powers up FRPLL even if the rate is unchanged;
 * it has no bypass test. Unknown rates use the 48000 settings but the requested
 * rate is still cached. Static reconstruction, no live register observation.
 * Evidence: build/scratch/seek-chain/aud_hal_set_arc_sample_rate.asm.
 */
#include <linux/kernel.h>
#include <linux/delay.h>
#include "sys_regs.h"
#include "os_util.h"
#include "log.h"
#include "hal/hal_clock.h"

logm_define (aud_hal_clock, log_level_noti);
#define info(fmt,args...) logm_info(aud_hal_clock,fmt,##args)
static unsigned int gArcSampleRate = 48000;

struct frpll_fout_settings
{
    unsigned int sample_rate;
    unsigned int divint;
    unsigned int fcw;
    unsigned int out_div;
    unsigned int step;
    unsigned int icp;
    unsigned int icp_g;
};

static struct frpll_fout_settings frpll_fout_table[] = {
    /* *INDENT-OFF* */
    {48000,   0x3E,   0xCAAB8A,   0x1FF,  0x55BB, 0x0, 0x1},
    {44100,   0x39,   0xB09A67,   0x1FF,  0x55BB, 0x0, 0x1},
    {32000,   0x29,   0xDC725C,   0x1FF,  0x55BB, 0x0, 0x1},
    {192000,  0x40,   0x639D5E,   0x83,   0x15FA, 0x0, 0x1},
    {176400,  0x40,   0x93964A,   0x8F,   0x17FE, 0x0, 0x1},
    {128000,  0x40,   0x8D8EC9,   0xC5,   0x210D, 0x0, 0x1},
    {96000,   0x40,   0xA2877E,   0x107,  0x2C20, 0x0, 0x1},
    {88200,   0x40,   0x93964A,   0x11E,  0x3027, 0x0, 0x1},
    {64000,   0x40,   0xB78034,   0x18B,  0x4245, 0x0, 0x1},
    /* *INDENT-ON* */
};

static struct frpll_fout_settings *_aud_get_frpll_fout_settings (unsigned int sample_rate)
{
    int i;

    for (i = 0; i < sizeof (frpll_fout_table) / sizeof (frpll_fout_table[0]); i++)
    {
        if (frpll_fout_table[i].sample_rate == sample_rate)
            return &frpll_fout_table[i];
    }

    return &frpll_fout_table[0];
}

static int aud_set_earc_frpll (unsigned int sample_rate)
{
    struct frpll_fout_settings *pSettings = NULL;

    pSettings = _aud_get_frpll_fout_settings (sample_rate);

    CTOP_CTRL_O22Ax_RdFL (BND_CTRL_EDID, earc_frpll_03);
    CTOP_CTRL_O22Ax_Wr01 (BND_CTRL_EDID, earc_frpll_03, reg_earc_frpll_pdb, 0); //Analog & Digital power down
    CTOP_CTRL_O22Ax_WrFL (BND_CTRL_EDID, earc_frpll_03);

    CTOP_CTRL_O22Ax_RdFL (BND_CTRL_EDID, earc_frpll_02);
    CTOP_CTRL_O22Ax_Wr01 (BND_CTRL_EDID, earc_frpll_02, reg_earc_frpll_out_div, pSettings->out_div);
    CTOP_CTRL_O22Ax_WrFL (BND_CTRL_EDID, earc_frpll_02);

    CTOP_CTRL_O22Ax_RdFL (BND_CTRL_EDID, earc_frpll_00);
    CTOP_CTRL_O22Ax_Wr01 (BND_CTRL_EDID, earc_frpll_00, reg_earc_frpll_icp, pSettings->icp);
    CTOP_CTRL_O22Ax_Wr01 (BND_CTRL_EDID, earc_frpll_00, reg_earc_frpll_divint, pSettings->divint);
    CTOP_CTRL_O22Ax_Wr01 (BND_CTRL_EDID, earc_frpll_00, reg_earc_frpll_icp_g, pSettings->icp_g);
    CTOP_CTRL_O22Ax_WrFL (BND_CTRL_EDID, earc_frpll_00);

    CTOP_CTRL_O22Ax_RdFL (BND_CTRL_EDID, earc_frpll_01);
    CTOP_CTRL_O22Ax_Wr01 (BND_CTRL_EDID, earc_frpll_01, reg_earc_frpll_fcw, pSettings->fcw);
    CTOP_CTRL_O22Ax_WrFL (BND_CTRL_EDID, earc_frpll_01);

    CTOP_CTRL_O22Ax_RdFL (BND_CTRL_EDID, earc_frpll_02);
    CTOP_CTRL_O22Ax_Wr01 (BND_CTRL_EDID, earc_frpll_02, reg_earc_frpll_fout_en, 0x1);
    CTOP_CTRL_O22Ax_WrFL (BND_CTRL_EDID, earc_frpll_02);

    udelay (1);

    CTOP_CTRL_O22Ax_RdFL (BND_CTRL_EDID, earc_frpll_03);
    CTOP_CTRL_O22Ax_Wr01 (BND_CTRL_EDID, earc_frpll_03, reg_earc_frpll_pdb, 1); //Analog & Digital power down
    CTOP_CTRL_O22Ax_WrFL (BND_CTRL_EDID, earc_frpll_03);

    udelay (1);

    CTOP_CTRL_O22Ax_RdFL (BND_CTRL_EDID, earc_frpll_00);
    CTOP_CTRL_O22Ax_Wr01 (BND_CTRL_EDID, earc_frpll_00, reg_earc_frpll_syncon, 0x1);
    CTOP_CTRL_O22Ax_WrFL (BND_CTRL_EDID, earc_frpll_00);

    udelay (100);

    info ("sample_rate %d Hz divint(%x) fcw(%x) outdiv(%x)\n", sample_rate,
        pSettings->divint, pSettings->fcw, pSettings->out_div);

    return RET_OK;
}

/* @0xffffffc010cf1614 */
int aud_hal_set_arc_sample_rate (unsigned int sample_rate)
{
    aud_set_earc_frpll (sample_rate);

    gArcSampleRate = sample_rate;

    info ("sample_rate %d Hz.\n", sample_rate);

    return 0;
}
