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
 * Modified to match the shipped kernel Image (sha256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db,
 * orig/boot/Image-5.4.268-329.decompressed).
 *
 * Base: lg-opensource/kdriver/core/aud/hal/o22/hal_output.c (GPL-2.0), the
 * eARC / output HAL. The shipped image uses the o22 variant: the string
 * "O22 I2S Number(%d)" is present and every log __LINE__ is the published
 * one +2 (e.g. "eARC Output Format : %d, %d" at 988, published 986), with
 * CHIP_TYPE_FPGA off. aud_hal_dts_supported()/aud_hal_get_number_of_dsp()
 * from hal/o22/hal.c are appended at the end.
 *
 * webOS25-shipped differences: audio_hal_earc_monitor_lock (see there);
 * aud_hal_dts_supported() returns false (NOT_SUPPORTED_DTS defined, i.e. the
 * Makefile's "y22_board" feature set), aud_hal_get_number_of_dsp() == 4.
 * audio_hal_earc_set_es_mode/set_pcm_mode, holding_differential_signal and
 * sending_noise_signal are inlined in the image (no symbol, no tag).
 * Statics are made global so the tagged functions are kept.
 */


/** @file
 *
 *  audio output hal driver
 *
 *  author      Youngwoo Jin ( youngwoo.jin@lge.com )
 *  version     1.0
 *  date        2020.08.13
 *
 */

/*------------------------------------------------------------------------------
    Control Constants
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    File Inclusions
------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/delay.h>

#ifndef CHIP_TYPE_FPGA
#include "sys_regs.h"
#endif
#include "os_util.h"
#include "logm.h"

#include "hal/reg_util.h"
#include "hal/hal_output.h"
#include "hal/o22/regs.h"

/*------------------------------------------------------------------------------
    Constant Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Macro Definitions
------------------------------------------------------------------------------*/
#define trace(fmt,args...)  logm_trace(aud_hal_output,fmt,##args)
#define debug(fmt,args...)  logm_debug(aud_hal_output,fmt,##args)
#define info(fmt,args...)   logm_info(aud_hal_output,fmt,##args)
#define noti(fmt,args...)   logm_noti(aud_hal_output,fmt,##args)
#define error(fmt,args...)  logm_error(aud_hal_output,fmt,##args)

/*------------------------------------------------------------------------------
    Type Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    External Function Prototype Declarations
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    External Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    global Variables
------------------------------------------------------------------------------*/
logm_define (aud_hal_output, log_level_noti);

/*------------------------------------------------------------------------------
    Static Function Prototypes Declarations
------------------------------------------------------------------------------*/
#ifndef CHIP_TYPE_FPGA
#define EARC_I2C_REG_BASE           0xC890E800
#define EARC_I2C_REG_SIZE           0x1000

#define EARC_LINK0_REG_BASE          0xC8923900
#define EARC_LINK0_REG_SIZE          0x100

#define EARC_LINK_REG_BASE          0xC8924000
#define EARC_LINK_REG_SIZE          0x1000

#define EARC_INT_REG_BASE           0xC8925300
#define EARC_INT_REG_SIZE           0x1000

#define EARC_PHY_REG_BASE           0xC8940000
#define EARC_PHY_REG_SIZE           0x1000

static volatile unsigned int *earc_i2c_reg;
static volatile unsigned int *earc_link_reg;
static volatile unsigned int *earc_link0_reg;
static volatile unsigned int *earc_int_reg;
static volatile unsigned int *earc_phy_reg;

#define EARC_I2C_RD(offset)             *(volatile unsigned int *)((void *)earc_i2c_reg + offset)
#define EARC_I2C_WR(offset,value)       EARC_I2C_RD(offset) = (value)
#define EARC_LINK0_RD(offset)             *(volatile unsigned int *)((void *)earc_link0_reg + offset)
#define EARC_LINK0_WR(offset,value)       EARC_LINK0_RD(offset) = (value)
#define EARC_LINK_RD(offset)             *(volatile unsigned int *)((void *)earc_link_reg + offset)
#define EARC_LINK_WR(offset,value)       EARC_LINK_RD(offset) = (value)
#define EARC_INT_RD(offset)             *(volatile unsigned int *)((void *)earc_int_reg + offset)
#define EARC_INT_WR(offset,value)       EARC_INT_RD(offset) = (value)
#define EARC_PHY_RD(offset)              *(volatile unsigned int *)((void *)earc_phy_reg + offset)
#define EARC_PHY_WR(offset,value)        EARC_PHY_RD(offset) = (value)

/*------------------------------------------------------------------------------
    Static Variables
------------------------------------------------------------------------------*/
static bool gAudHalEarcOn = false;
static unsigned int gEarcIec958csd0 = 0x00000000;
static unsigned int gEarcIec958csd1 = 0x00000000;
static unsigned int gEarcIec958csd4 = 0x00000000;
static unsigned int gEarcDmacConfig0 = 0x00008010;      // I2S 24bits, SPDIF IF(up to 8 ch)
static unsigned int gEarcDmacConfig1 = 0x00010001;      // phy differential driver enable, phy biphase clock enable
static unsigned int gEarcPllBpClk = 0x60;
static unsigned int gEarcOutputFormat = 0;
static unsigned int gEarcOutputSamplingFreq = 48000;
static unsigned int gEarcChannel = 2;
static unsigned int gEarcAcmod = 2;

/* The only in-kernel layout-B selector: aud.audio_compressed_layout (kernel_param
 * @0xffffffc011a15e50, perm 0644, default 0); no code in the Image stores to it. */
static int audio_compressed_layout = 0; // 0:compressed layout A, 1:compressed layout B
module_param (audio_compressed_layout, int, 0644);
#endif

#define MIXEDIP_RESET_DELAY_100US      (100+1)  // 100us

/*==============================================================================
    Implementation Group
==============================================================================*/
/* webOS25-shipped: built without CHIP_TYPE_FPGA (the published FPGA stubs are
 * omitted); the eARC code below is what is in the image. */
/* @0xffffffc010cf43c4 */
irqreturn_t audio_hal_earc_irq (int num, void *arg)
{
    unsigned int status = 0;

    /* Get ISR Status */
    status = EARC_INT_RD (0x000);

    if (!status)
    {
        error ("earc int status : %08x\n", status);
        return IRQ_NONE;
    }
    status &= 0x7;

    /* Clear ISR */
    EARC_INT_WR (0x008, status);

    audio_hal_earc_clear_interrupt (status);

    return IRQ_HANDLED;
}

/* @0xffffffc010cf2e10 */
int aud_hal_output_init_earc (void)
{
    unsigned int earc_i2c_reset = 0;
    unsigned int earc_user_pdb = 0;

    // PHY i2c ON
    earc_i2c_reset = EARC_I2C_RD (0x34);
    if ((earc_i2c_reset & 0x00000111) != 0x00000111)
    {
        //EARC_I2C_WR (0x34, 0x00000000); // i2c reset 0
        EARC_I2C_WR (0x34, 0x00000111); // i2c reset 1

        udelay (MIXEDIP_RESET_DELAY_100US);

        noti ("earc_i2c_reset : %x\n", earc_i2c_reset);
    }

    earc_user_pdb = EARC_PHY_RD (0x004);
    if (earc_user_pdb != 0x11)
    {
        EARC_PHY_WR (0x004, 0x00);      // USER_PDB, USER_APHY_DMAC_DRV_8PHASE_EN
        EARC_PHY_WR (0x004, 0x11);      // USER_PDB, USER_APHY_DMAC_DRV_8PHASE_EN

        noti ("earc_user_pdb : %x\n", earc_user_pdb);
    }

    noti ("set registers for earc is complete\n");
    return 0;
}

/* @0xffffffc010cf2f80 */
int aud_hal_output_init (void)
{
    int ret;

    earc_i2c_reg = (volatile unsigned int *) ioremap (EARC_I2C_REG_BASE, EARC_I2C_REG_SIZE);
    if (!earc_i2c_reg)
    {
        error ("earc_i2c_reg is not ioremaped.\n");
        return -EIO;
    }

    earc_link_reg = (volatile unsigned int *) ioremap (EARC_LINK_REG_BASE, EARC_LINK_REG_SIZE);
    if (!earc_link_reg)
    {
        error ("earc_link_reg is not ioremaped.\n");
        return -EIO;
    }

    earc_link0_reg = (volatile unsigned int *) ioremap (EARC_LINK0_REG_BASE, EARC_LINK0_REG_SIZE);
    if (!earc_link0_reg)
    {
        error ("earc_link0_reg is not ioremaped.\n");
        return -EIO;
    }

    earc_int_reg = (volatile unsigned int *) ioremap (EARC_INT_REG_BASE, EARC_INT_REG_SIZE);
    if (!earc_int_reg)
    {
        error ("earc_int_reg is not ioremaped.\n");
        return -EIO;
    }

    earc_phy_reg = (volatile unsigned int *) ioremap (EARC_PHY_REG_BASE, EARC_PHY_REG_SIZE);
    if (!earc_phy_reg)
    {
        error ("earc_phy_reg is not ioremaped.\n");
        return -EIO;
    }

    aud_hal_output_init_earc ();

    ret =
        request_irq (O22_IRQ_INTR_HDMIRX_PRT1_EARC, audio_hal_earc_irq, IRQF_TRIGGER_NONE,
        "eARC", NULL);
    if (ret < 0)
        error ("request_irq() failed for eARC\n");

    noti ("ioremap is complete for earc.\n");
    return 0;
}

/* @0xffffffc010cf3394 */
void aud_hal_output_exit (void)
{
    free_irq (O22_IRQ_INTR_HDMIRX_PRT1_EARC, NULL);

    if (earc_i2c_reg)
    {
        iounmap (earc_i2c_reg);
        earc_i2c_reg = NULL;
    }

    if (earc_link_reg)
    {
        iounmap (earc_link_reg);
        earc_link_reg = NULL;
    }

    if (earc_link0_reg)
    {
        iounmap (earc_link0_reg);
        earc_link0_reg = NULL;
    }

    if (earc_int_reg)
    {
        iounmap (earc_int_reg);
        earc_int_reg = NULL;
    }

    if (earc_phy_reg)
    {
        iounmap (earc_phy_reg);
        earc_phy_reg = NULL;
    }

    noti ("iounmap is complete for earc.\n");
}

/* @0xffffffc010cf3464 */
void aud_hal_output_set_number_of_i2s (int number)
{
    if (number >= 1)
    {
        // I2S 2 CH
        // reg_pad_daclrck_mux_sel   :  4,    //  15:12, 0: micom_jtag_tms , 2:daclrck
        // reg_pad_dacsck_mux_sel    :  4,    //  11: 8, 0: micom_jtag_tdo , 2:dacsck
        // reg_pad_daclrch_mux_sel   :  4,    //   7: 4, 0: micom_jtag_tdi , 2:daclrch
        CTOP_CTRL_O22Ax_RdFL (FUNC_IOMUX_WOV, pad_wov69);
        CTOP_CTRL_O22Ax_Wr01 (FUNC_IOMUX_WOV, pad_wov69, reg_pad_daclrck_mux_sel, 0x2);
        CTOP_CTRL_O22Ax_Wr01 (FUNC_IOMUX_WOV, pad_wov69, reg_pad_dacsck_mux_sel, 0x2);
        CTOP_CTRL_O22Ax_Wr01 (FUNC_IOMUX_WOV, pad_wov69, reg_pad_daclrch_mux_sel, 0x2);
        CTOP_CTRL_O22Ax_WrFL (FUNC_IOMUX_WOV, pad_wov69);
    }

    if (number >= 2)
    {
        // I2S 2 CH(3 D-AMP)
        // [11:08] , 0: dacslrch , 1: gpio5
        CTOP_CTRL_O22Ax_RdFL (FUNC_IOMUX_FMS, pad_fms64);
        CTOP_CTRL_O22Ax_Wr01 (FUNC_IOMUX_FMS, pad_fms64, reg_pad_dacslrch_mux_sel, 0x0);
        CTOP_CTRL_O22Ax_WrFL (FUNC_IOMUX_FMS, pad_fms64);
    }

    if (number == 0)
    {
        AUD_REG_RdFL (PERI_CTRL, ocma);
        AUD_REG_WrFd (PERI_CTRL, ocma, dack_mask, 0);
        AUD_REG_WrFL (PERI_CTRL, ocma);
    }
    else
    {
        AUD_REG_RdFL (PERI_CTRL, ocma);
        AUD_REG_WrFd (PERI_CTRL, ocma, dack_mask, 1);
        AUD_REG_WrFL (PERI_CTRL, ocma);
    }

    noti ("O22 I2S Number(%d)\n", number);
}

/* @0xffffffc010cf3590 */
int aud_hal_output_optic_light_on (bool on)
{
    AUD_REG_RdFL (PERI_CTRL, aud_spdifout_sel);
    AUD_REG_WrFd (PERI_CTRL, aud_spdifout_sel, spdifoutsel_dsp, on << 1);
    AUD_REG_WrFL (PERI_CTRL, aud_spdifout_sel);

    noti ("onptic light %d.\n", on);
    return 0;
}

/* webOS25-shipped: polls with usleep_range(200)/(100) instead of udelay,
 * sleeps 100us per iteration, counts iterations and logs
 * "[%d] frpll_lock:%x, earc_phy_lock:%x" (error at line 412, info at 414). */
/* @0xffffffc010cf2a70 */
void audio_hal_earc_monitor_lock (void)
{
    int frpll_lock = 0, earc_phy_lock = 0;
    signed long jiffies_start = jiffies;
    int count = 0;              /* webOS25-shipped */

    do
    {
        CTOP_CTRL_O22Ax_RdFL (BND_CTRL_EDID, earc_frpll_06);
        CTOP_CTRL_O22Ax_Rd01 (BND_CTRL_EDID, earc_frpll_06, reg_earc_frpll_lock, frpll_lock);
        earc_phy_lock = EARC_PHY_RD (0x05C);
        earc_phy_lock &= 0x10;
        if (frpll_lock && !earc_phy_lock)
        {
            EARC_PHY_WR (0x054, 0x12);  // BPCLOCK_EN low
            usleep_range (200, 200);
            EARC_PHY_WR (0x054, 0x10);  // BPCLOCK_EN link control
        }
        else
        {
            if (frpll_lock && earc_phy_lock)
                break;
        }
        usleep_range (100, 100);
        count++;
    }
    while (jiffies_to_msecs (jiffies - jiffies_start) < 100);

    if (!frpll_lock || !earc_phy_lock)
        error ("[%d] frpll_lock:%x, earc_phy_lock:%x\n", count, frpll_lock, earc_phy_lock);
    else
        info ("[%d] frpll_lock:%x, earc_phy_lock:%x\n", count, frpll_lock, earc_phy_lock);
}

/* @0xffffffc010cf28f0 */
void audio_hal_earc_sending_differential_signal (struct timer_list *unused)
{
    EARC_PHY_WR (0x05C, 0x00);  // MONITOR_DMAC_BIPHASE_CLK_UP USER_DMAC_DRV_EN_FORCE : 0 : controlled by link
    //EARC_PHY_WR (0x058, 0x30);  // USER_DMAC_PHY_CLK_UP_WAIT  USER_DMAC_BIPHASE_PHY_CLK_UP_FORCE
    debug ("Start Sending.\n");
}

static DEFINE_TIMER (_gHoldingTimer, audio_hal_earc_sending_differential_signal);

static void audio_hal_earc_holding_differential_signal (void)
{
    /* holding time more than 100ms */
    EARC_PHY_WR (0x05C, 0x02);  // MONITOR_DMAC_BIPHASE_CLK_UP USER_DMAC_DRV_EN_FORCE : 2 : force to low
    //EARC_PHY_WR (0x058, 0x32);  // USER_DMAC_PHY_CLK_UP_WAIT  USER_DMAC_BIPHASE_PHY_CLK_UP_FORCE
    mod_timer (&_gHoldingTimer, jiffies + msecs_to_jiffies (150));
    debug ("Stop Sending.\n");
}

// for SONY eARC devices compatibiltiy
/* @0xffffffc010cf2964 */
void audio_hal_earc_holding_noise_signal (struct timer_list *unused)
{
    EARC_PHY_WR (0x05C, 0x00);  // MONITOR_DMAC_BIPHASE_CLK_UP USER_DMAC_DRV_EN_FORCE : 0 : controlled by link
    EARC_PHY_WR (0x058, 0x30);  // USER_DMAC_PHY_CLK_UP_WAIT  USER_DMAC_BIPHASE_PHY_CLK_UP_FORCE
    debug ("Start Sending.\n");
}

static DEFINE_TIMER (_gNoiseTimer, audio_hal_earc_holding_noise_signal);

static void audio_hal_earc_sending_noise_signal (void)
{
    /* holding time more than 100ms */
    EARC_PHY_WR (0x05C, 0x00);  // MONITOR_DMAC_BIPHASE_CLK_UP USER_DMAC_DRV_EN_FORCE : 2 : force to low
    EARC_PHY_WR (0x058, 0x32);  // USER_DMAC_PHY_CLK_UP_WAIT  USER_DMAC_BIPHASE_PHY_CLK_UP_FORCE
    mod_timer (&_gNoiseTimer, jiffies + msecs_to_jiffies (150));
    debug ("Stop Sending.\n");
}

/* @0xffffffc010cf3630 */
void audio_hal_earc_set_on (bool on)
{
    if (on == ON)
    {
        if (lx_chip_rev () >= LX_CHIP_REV (O20, B0))
            EARC_PHY_WR (0x008, 0x43);  // DMAC Driver Current Control (default : 0x43)
        else
            EARC_PHY_WR (0x008, 0x63);  // DMAC Driver Current Control (default : 0x43)

        EARC_PHY_WR (0x06C, 0x00);
        //EARC_PHY_WR (0x05C, 0x02);      // MONITOR_DMAC_BIPHASE_CLK_UP USER_DMAC_DRV_EN_FORCE /
        EARC_PHY_WR (0x004, 0x11);
        EARC_PHY_WR (0x04C, 0x00);
        //EARC_PHY_WR (0x010, 0xC5);      // ODT(ARC MODE), PLL_AUTO_RSTB_OFF,PLL_RSTB
        EARC_PHY_WR (0x014, 0x00);
        EARC_PHY_WR (0x030, gEarcPllBpClk);

        EARC_PHY_WR (0x150, 0x03);      // DMAC Slew Control
        //EARC_PHY_WR (0x05C, 0x00);      // MONITOR_DMAC_BIPHASE_CLK_UP USER_DMAC_DRV_EN_FORCE /

        EARC_LINK0_WR (0x000, gEarcIec958csd0);
        EARC_LINK0_WR (0x004, gEarcIec958csd1);
        EARC_LINK0_WR (0x008, 0x00000000);
        EARC_LINK0_WR (0x00C, 0x00000000);
        EARC_LINK0_WR (0x010, gEarcIec958csd4);
        EARC_LINK0_WR (0x014, 0x00000000);

        EARC_LINK_WR (0x010, gEarcIec958csd0);
        EARC_LINK_WR (0x014, gEarcIec958csd1);
        EARC_LINK_WR (0x018, 0x00000000);
        EARC_LINK_WR (0x01C, 0x00000000);
        EARC_LINK_WR (0x020, gEarcIec958csd4);
        EARC_LINK_WR (0x024, 0x00000000);

        EARC_LINK_WR (0x004, gEarcDmacConfig0);
        EARC_LINK_WR (0x008, gEarcDmacConfig1);
        EARC_LINK_WR (0x00C, 0x00000000);       // dmac audio data off fifo reset ??
        EARC_LINK_WR (0x00C, 0x00000001);

        //EARC_PHY_WR (0x010, 0xC5);       // ODT(ARC MODE), PLL_AUTO_RSTB_OFF,PLL_RSTB
        //EARC_PHY_WR (0x040, 0x01);
    }
    else
    {
        EARC_LINK_WR (0x004, gEarcDmacConfig0);
        EARC_LINK_WR (0x008, gEarcDmacConfig1);
        EARC_LINK_WR (0x00C, 0x00000000);       // dmac audio data en

        EARC_PHY_WR (0x008, 0x43);      // DMAC Driver Current Control (default : 0x43)
        //EARC_PHY_WR (0x010, 0xB1);       // ODT(ARC MODE), PLL_AUTO_RSTB_OFF,PLL_RSTB
        //EARC_PHY_WR (0x040, 0x02);       // USER_EARC_ARC_SEL(0:earc 2:arc)
    }

    {
        EARC_PHY_WR (0x034, 0x01);      // USER_HDMI_AUD_CLK_SEL(1:extern)
        EARC_PHY_WR (0x038, 0x2f);      // USER_AUD_SPDIF_DATA_SEL(1:aoc 2:extern) USER_AUD_EXTERN_SPDIF_DATA_CTRL
        EARC_PHY_WR (0x03C, 0x2f);      // USER_AUD_I2S_DATA_SEL(1:aoc 2:extern) USER_AUD_EXTERN_I2S_DATA_CTRL

        EARC_PHY_WR (0x050, 0x00);      // USER_DMAC_BP_MODE USER_DMAC_FEM_IN_SE
        //EARC_PHY_WR (0x054, 0x10);      // USER_DMAC_FEM_MODE  USER_DMAC_BIPHASE_PHY_CLK_EN_FORCE
        EARC_PHY_WR (0x058, 0x30);      // USER_DMAC_PHY_CLK_UP_WAIT  USER_DMAC_BIPHASE_PHY_CLK_UP_FORCE

        audio_hal_earc_monitor_lock ();
    }

    // Set eARC TX Interrupt Mask.
    {
        unsigned int earcIntMask = 0;

        earcIntMask = EARC_INT_RD (0x004);

        if (on == ON)
            earcIntMask |= 0x7;
        else
            earcIntMask &= ~(0x7);

        EARC_INT_WR (0x004, earcIntMask);
    }

    // for SONY eARC devices compatibiltiy eARC Off -> On
    if ((gAudHalEarcOn == OFF) && (on == ON))
        audio_hal_earc_sending_noise_signal ();
    else
        audio_hal_earc_holding_differential_signal ();

    gAudHalEarcOn = on;

    noti ("eARC On : %d\n", on);

    return;
}

/* @0xffffffc010cf38f0 */
void audio_hal_earc_set_scms (unsigned int scms)
{
    unsigned int iec958csd0;

    iec958csd0 = gEarcIec958csd0;

    // bit 2 : CP-bit
    // bit 15 : L-bit
    if (scms == 0)              //copy free
    {
        iec958csd0 |= (0x1 << 2);       // set bit 2 (CP)
        iec958csd0 &= ~(0x1 << 15);     // clear bit 15 (L)
    }
    else if (scms == 2)         //copy once
    {
        iec958csd0 &= ~(0x1 << 2);      // clear bit 2
        iec958csd0 &= ~(0x1 << 15);     // clear bit 15 (L)
    }
    else if (scms == 3 || scms == 1)    //copy no more copy never
    {
        iec958csd0 &= ~(0x1 << 2);      // clear bit 2
        iec958csd0 |= (0x1 << 15);      // set bit 15 (L)
    }

    EARC_LINK0_WR (0x000, iec958csd0);
    EARC_LINK_WR (0x010, iec958csd0);
    EARC_LINK0_WR (0x014, 0x0);
    EARC_LINK_WR (0x024, 0x0);

    gEarcIec958csd0 = iec958csd0;

    info ("eARC SCMS : 0x%x\n", scms);
}

/* @0xffffffc010cf39d0 */
void audio_hal_earc_set_category_code (unsigned int category_code)
{
    unsigned int iec958csd0;

    iec958csd0 = gEarcIec958csd0;
    iec958csd0 &= ~(0xFF << 8);
    iec958csd0 |= ((category_code & 0xFF) << 8);
    EARC_LINK0_WR (0x000, iec958csd0);
    EARC_LINK_WR (0x010, iec958csd0);
    EARC_LINK0_WR (0x014, 0x0);
    EARC_LINK_WR (0x024, 0x0);
    gEarcIec958csd0 = iec958csd0;

    info ("eARC category_code : 0x%x\n", category_code);
}

/* @0xffffffc010cf2c50 */
void audio_hal_earc_set_pll_bp_clock (unsigned int sampling_freq)
{
    unsigned int earc_pll_bp_clk = 0;

    earc_pll_bp_clk = EARC_PHY_RD (0x030);
    earc_pll_bp_clk &= ~(0xF << 4);
    if (sampling_freq == 32000)
        earc_pll_bp_clk |= (0x0 << 4);
    else if (sampling_freq == 64000)
        earc_pll_bp_clk |= (0x1 << 4);
    else if (sampling_freq == 128000)
        earc_pll_bp_clk |= (0x2 << 4);
    else if (sampling_freq == 44100)
        earc_pll_bp_clk |= (0x3 << 4);
    else if (sampling_freq == 88200)
        earc_pll_bp_clk |= (0x4 << 4);
    else if (sampling_freq == 176400)
        earc_pll_bp_clk |= (0x5 << 4);
    else if (sampling_freq == 48000)
        earc_pll_bp_clk |= (0x6 << 4);
    else if (sampling_freq == 96000)
        earc_pll_bp_clk |= (0x7 << 4);
    else if (sampling_freq == 192000)
        earc_pll_bp_clk |= (0x8 << 4);
    else
    {
        error ("sampling freq(%d) is invalid.\n", sampling_freq);
        earc_pll_bp_clk |= (0x6 << 4);
        sampling_freq = 48000;
    }
    EARC_PHY_WR (0x030, earc_pll_bp_clk);
    gEarcPllBpClk = earc_pll_bp_clk;
}

/* @0xffffffc010cf2630 */
unsigned int audio_hal_earc_get_dmac_sampling_bit (unsigned int sampling_freq)
{
    unsigned int sampling_bit = 0;

    switch (sampling_freq)
    {
        case 32000:
            sampling_bit = 0x0;
            break;
        case 44100:
            sampling_bit = 0x1;
            break;
        case 48000:
            sampling_bit = 0x2;
            break;
        case 64000:
            sampling_bit = 0x3;
            break;
        case 88200:
            sampling_bit = 0x8;
            break;
        case 96000:
            sampling_bit = 0x9;
            break;
        case 128000:
            sampling_bit = 0xa;
            break;
        case 176400:
            sampling_bit = 0xb;
            break;
        case 192000:
            sampling_bit = 0xc;
            break;
        default:
            sampling_bit = 0x2;
            break;
    }

    return sampling_bit;
}

/*
   refer to HDMISpecification2.1-final.pdf
   9.2 Supported Audio Rates
   Table 9-2:Allowed Values for channel status bit 24 to 27, 30, and 31
*/
/* @0xffffffc010cf26e4 */
unsigned int audio_hal_earc_get_sampling_bit (unsigned int sampling_freq)
{
    unsigned int sampling_bit = 0;

    if (audio_compressed_layout == 1)
        sampling_freq *= 4;
    else
    {
        if ((gEarcChannel == 8) || (gEarcChannel == 6))
            sampling_freq *= 4;
    }

    switch (sampling_freq)
    {
        case 32000:
            sampling_bit = 0x3;
            break;
        case 64000:
            sampling_bit = 0xb;
            break;
        case 128000:
            sampling_bit = 0x8b;
            break;
        case 256000:
            sampling_bit = 0x4b;
            break;
        case 512000:
            sampling_bit = 0xcb;
            break;
        case 1024000:
            sampling_bit = 0xc5;
            break;
        case 22050:
            sampling_bit = 0x4;
            break;
        case 44100:
            sampling_bit = 0x0;
            break;
        case 88200:
            sampling_bit = 0x8;
            break;
        case 176400:
            sampling_bit = 0xc;
            break;
        case 352800:
            sampling_bit = 0xd;
            break;
        case 705600:
            sampling_bit = 0x8d;
            break;
        case 1411200:
            sampling_bit = 0x4d;
            break;
        case 24000:
            sampling_bit = 0x6;
            break;
        case 48000:
            sampling_bit = 0x2;
            break;
        case 96000:
            sampling_bit = 0xa;
            break;
        case 192000:
            sampling_bit = 0xe;
            break;
        case 384000:
            sampling_bit = 0x5;
            break;
        case 768000:
            sampling_bit = 0x9;
            break;
        case 1536000:
            sampling_bit = 0x45;
            break;
        default:
            sampling_bit = 0x2;
            break;
    }

    return sampling_bit;
}

/* Inlined: audio_hal_earc_set_output_format +0x168..+0x218 (0xffffffc010cf3bdc..3c8c),
 * enable at +0x21c; verified. get_dmac_sampling_bit is hoisted to +0x38, before
 * the DMAC-disable store. Mode 6 (layout A) unless audio_compressed_layout == 1. */
static void audio_hal_earc_set_es_mode (unsigned int sampling_freq)
{
    unsigned int sampling_bit = 0;
    unsigned int iec958csd0 = 0;
    unsigned int iec958csd1 = 0;
    unsigned int iec958csd4 = 0;
    unsigned int earc_dmac_config0 = 0;
    unsigned int earc_dmac_config1 = 0;
#if 0
    unsigned int earc_pll_bp_clk = 0;
#endif
    // Disable eARC TX DMAC audio datapath.
    EARC_LINK_WR (0x00C, 0x0);

    // Config eARC TX DMAC audio mode
    earc_dmac_config0 = gEarcDmacConfig0;
    earc_dmac_config0 |= 1 << 28;
    //earc_dmac_config0 |= 1 << 25;
    //earc_dmac_config0 |= 1 << 24;
    //earc_dmac_config0 |= 1 << 20;
    //earc_dmac_config0 |= 1 << 16;
    earc_dmac_config0 &= ~(1 << 8);     // EARCTX_DMAC_AUDIO_SOURCE_LAYOUT_QST
    if ((gEarcChannel == 8) || (gEarcChannel == 6))
        earc_dmac_config0 |= (1 << 8);  // 0:2-channel, 1:8-Channel
    earc_dmac_config0 &= ~(7);  // EARCTX_DMAC_AUDIO_MODE_QST   0:2-channel L-PCM, 6:Compressed Layout A, 7:Compressed Layout B
    if (audio_compressed_layout == 1)
    {
        earc_dmac_config0 |= 1 << 8;    // audio source layout 0 : 2-channel, 1 : 8-channel
        earc_dmac_config0 |= 7; // compressed layout B
    }
    else
        earc_dmac_config0 |= 6; // compressed layout A
    EARC_LINK_WR (0x004, earc_dmac_config0);
    gEarcDmacConfig0 = earc_dmac_config0;

    earc_dmac_config1 = gEarcDmacConfig1;
    earc_dmac_config1 &= ~(0xF << 8);
    sampling_bit = audio_hal_earc_get_dmac_sampling_bit (sampling_freq);
    earc_dmac_config1 |= (sampling_bit << 8);
    earc_dmac_config1 &= ~(0x1);        // EARCTX_DMAC_PHY_BIPHASE_CLK_EN 0: disable
    EARC_LINK_WR (0x008, earc_dmac_config1);
    earc_dmac_config1 |= 0x1;   // EARCTX_DMAC_PHY_BIPHASE_CLK_EN 1:enable
    EARC_LINK_WR (0x008, earc_dmac_config1);
    gEarcDmacConfig1 = earc_dmac_config1;

    iec958csd0 = gEarcIec958csd0;
    iec958csd0 |= (0x1 << 1);   // 0:pcm, 1:es
    iec958csd0 &= ~(0x7 << 3);
    iec958csd0 &= ~(0xF << 20);
    iec958csd0 &= ~(0xCF << 24);        // 24-27, 30, 31
    sampling_bit = audio_hal_earc_get_sampling_bit (sampling_freq);
    iec958csd0 |= sampling_bit << 24;
    EARC_LINK0_WR (0x000, iec958csd0);
    EARC_LINK_WR (0x010, iec958csd0);
    gEarcIec958csd0 = iec958csd0;

    // compressed layout A/B
    iec958csd1 = gEarcIec958csd1;
    iec958csd1 &= ~(0xF << 12); // 0: compressed layout A, 7:compressed layout B
    if (audio_compressed_layout == 1)
        iec958csd1 |= 0x7 << 12;        // 0: compressed layout A, 7:compressed layout B
    EARC_LINK0_WR (0x004, iec958csd1);
    EARC_LINK_WR (0x014, iec958csd1);
    gEarcIec958csd1 = iec958csd1;

    iec958csd4 = gEarcIec958csd4;
    iec958csd4 &= ~(0xFF << 8);
    EARC_LINK0_WR (0x010, iec958csd4);
    EARC_LINK_WR (0x020, iec958csd4);
    gEarcIec958csd4 = iec958csd4;

    EARC_LINK0_WR (0x014, 0x0);
    EARC_LINK_WR (0x024, 0x0);

#if 0
    earc_pll_bp_clk = EARC_PHY_RD (0x030);
    if (audio_compressed_layout == 1)
    {
        earc_pll_bp_clk |= 0x1; // 0:2-channel , 1:8-channel
    }
    EARC_PHY_WR (0x030, earc_pll_bp_clk);
    gEarcPllBpClk = earc_pll_bp_clk;
#endif
    // Enable eARC TX DMAC audio datapath.
    if (gAudHalEarcOn)
        EARC_LINK_WR (0x00C, 0x1);
}

/* Inlined: audio_hal_earc_set_output_format +0x6c..+0x110 and +0x254..+0x338
 * (0xffffffc010cf3ae0..3b84, 0xffffffc010cf3cc8..3dac); verified. */
static void audio_hal_earc_set_pcm_mode (unsigned int sampling_freq)
{
    unsigned int sampling_bit = 0;
    unsigned int iec958csd0 = 0;
    unsigned int iec958csd1 = 0;
    unsigned int iec958csd4 = 0;
    unsigned int earc_dmac_config0 = 0;
    unsigned int earc_dmac_config1 = 0;
#if 0
    unsigned int earc_pll_bp_clk = 0;
#endif
    // Disable eARC TX DMAC audio datapath.
    EARC_LINK_WR (0x00C, 0x0);

    // Config eARC TX DMAC audio mode
    earc_dmac_config0 = gEarcDmacConfig0;
    earc_dmac_config0 &= ~(1 << 28);    // EARCTX_DMAC_ECC_CUV_SWAP_QST
    //earc_dmac_config0 &= ~(1 << 25);  // EARCTX_DMAC_ECC_OUTPUT_BITORDER_QST
    //earc_dmac_config0 &= ~(1 << 24);  // EARCTX_DMAC_ECC_CALC_BITORDER_QST
    //earc_dmac_config0 &= ~(3 << 20);  // EARCTX_DMAC_COMP_B_VBIT_OVR_VALUE_QST
    //earc_dmac_config0 &= ~(1 << 16);    // EARCTX_DMAC_I2S_BPCUV_EN_QST
    earc_dmac_config0 &= ~(1 << 8);     // EARCTX_DMAC_AUDIO_SOURCE_LAYOUT_QST
    if ((gEarcChannel == 8) || (gEarcChannel == 6))
        earc_dmac_config0 |= (1 << 8);  // 0:2-channel, 1:8-Channel
    earc_dmac_config0 &= ~(7);  // EARCTX_DMAC_AUDIO_MODE_QST   0:2-channel L-PCM, 6:Compressed Layout A, 7:Compressed Layout B
    if ((gEarcChannel == 8) || (gEarcChannel == 6))
        earc_dmac_config0 |= (1);       // 0:2-channel, 1:8-Channel
    EARC_LINK_WR (0x004, earc_dmac_config0);
    gEarcDmacConfig0 = earc_dmac_config0;

    earc_dmac_config1 = gEarcDmacConfig1;
    earc_dmac_config1 &= ~(0xF << 8);
    sampling_bit = audio_hal_earc_get_dmac_sampling_bit (sampling_freq);
    earc_dmac_config1 |= (sampling_bit << 8);
    earc_dmac_config1 &= ~(0x1);        // EARCTX_DMAC_PHY_BIPHASE_CLK_EN 0: disable
    EARC_LINK_WR (0x008, earc_dmac_config1);
    earc_dmac_config1 |= 0x1;   // EARCTX_DMAC_PHY_BIPHASE_CLK_EN 1:enable
    EARC_LINK_WR (0x008, earc_dmac_config1);
    gEarcDmacConfig1 = earc_dmac_config1;

    iec958csd0 = gEarcIec958csd0;
    iec958csd0 &= ~(0x7 << 3);
    if ((gEarcChannel == 8) || (gEarcChannel == 6))
        iec958csd0 |= (0x4 << 3);
    iec958csd0 &= ~(0xF << 20);
    if ((gEarcChannel == 8) || (gEarcChannel == 6))
    {
        if ((gEarcAcmod == 0) || (gEarcAcmod == 2))
            iec958csd0 |= (0x2 << 20);
        else if (gEarcAcmod == 7)
            iec958csd0 |= (0x6 << 20);
        else
            iec958csd0 |= (0x8 << 20);
    }
    else
        iec958csd0 |= (0x2 << 20);
    iec958csd0 &= ~(0x1 << 1);
    iec958csd0 &= ~(0xCF << 24);        // 24-27, 30, 31
    sampling_bit = audio_hal_earc_get_sampling_bit (sampling_freq);
    iec958csd0 |= sampling_bit << 24;   // set bit 0-3(24-27 bit0011000)
    EARC_LINK0_WR (0x000, iec958csd0);
    EARC_LINK_WR (0x010, iec958csd0);
    gEarcIec958csd0 = iec958csd0;

    // compressed layout A/B
    iec958csd1 = gEarcIec958csd1;
    iec958csd1 &= ~(0xF << 12); // 0: compressed layout A, 7:compressed layout B
    if ((gEarcChannel == 8) || (gEarcChannel == 6))
        iec958csd1 |= (0x7 << 12);      // 0x0: 0x2, 7:8, 0xB:16, 0x3:32
    else if (gEarcChannel == 16)
        iec958csd1 |= (0xB << 12);      // 0x0: 0x2, 7:8, 0xB:16, 0x3:32
    else if (gEarcChannel == 32)
        iec958csd1 |= (0x3 << 12);      // 0x0: 0x2, 7:8, 0xB:16, 0x3:32
    EARC_LINK0_WR (0x004, iec958csd1);
    EARC_LINK_WR (0x014, iec958csd1);
    gEarcIec958csd1 = iec958csd1;

    iec958csd4 = gEarcIec958csd4;
    iec958csd4 &= ~(0xFF << 8);
    if ((gEarcChannel == 8) || (gEarcChannel == 6))
    {
        if ((gEarcAcmod == 0) || (gEarcAcmod == 2))
            iec958csd4 |= (0x0 << 8);
        else if (gEarcAcmod == 7)
            iec958csd4 |= (0x0B << 8);
        else
            iec958csd4 |= (0x13 << 8);
    }
    EARC_LINK0_WR (0x010, iec958csd4);
    EARC_LINK_WR (0x020, iec958csd4);
    gEarcIec958csd4 = iec958csd4;

    EARC_LINK0_WR (0x014, 0x0);
    EARC_LINK_WR (0x024, 0x0);
#if 0
    earc_pll_bp_clk = EARC_PHY_RD (0x030);
    earc_pll_bp_clk &= ~(0x1);  // 0:2-channel , 1:8-channel
    EARC_PHY_WR (0x030, earc_pll_bp_clk);
    gEarcPllBpClk = earc_pll_bp_clk;
#endif
    // Enable eARC TX DMAC audio datapath.
    if (gAudHalEarcOn)
        EARC_LINK_WR (0x00C, 0x1);
}

/*
 * Verified instruction by instruction. In-Image callers: only _set_format +0x88/+0xc0.
 * Uses the cached gEarcChannel from the preceding set_channel; does not touch
 * PHY 0x30 bit0 (2/8-ch bypass), which only set_channel sets.
 * Final link state (layout param 0) after _set_format's set_channel + this:
 *  PCM 2ch fs:  DMAC mode 0, src layout 2ch, CS bit1=0, CS fs=fs, CS1[15:12]=0.
 *  ES 2ch fs:   mode 6 (compressed layout A), 2ch, ECC_CUV_SWAP, CS bit1=1, CS fs=fs.
 *  ES 8ch 192k: mode 6, src layout 8ch, CS bit1=1, CS fs code 0x9 (768 kHz),
 *               CS1[15:12]=0, DMAC fs 0xc, PLL 192k + 8-ch bypass. Same as stock MAT.
 * Layout B (mode 7, CS1[15:12]=7) only with audio_compressed_layout=1 or
 * set_audio_mode(7); nothing in the Image does either.
 */
/* @0xffffffc010cf3a74 */
void audio_hal_earc_set_output_format (enum audio_hal_output_format output_format,
    unsigned int sampling_freq)
{
    gEarcOutputFormat = output_format;
    gEarcOutputSamplingFreq = sampling_freq;

    if (output_format == AUDIO_HAL_OUTPUT_PCM)
        audio_hal_earc_set_pcm_mode (sampling_freq);
    else
        audio_hal_earc_set_es_mode (sampling_freq);

    audio_hal_earc_set_pll_bp_clock (sampling_freq);

    audio_hal_earc_monitor_lock ();

    noti ("eARC Output Format : %d, %d\n", output_format, sampling_freq);
}

/* @0xffffffc010cf3db0 */
void audio_hal_earc_set_mute (bool on)
{
    unsigned int iec958csd4 = 0;

    // Assert Channel Status Mute bit.
    iec958csd4 = gEarcIec958csd4;
    if (on)
        iec958csd4 |= 1 << 18;
    else
        iec958csd4 &= ~(1 << 18);

    EARC_LINK0_WR (0x010, iec958csd4);
    EARC_LINK_WR (0x020, iec958csd4);
    gEarcIec958csd4 = iec958csd4;
    EARC_LINK0_WR (0x014, 0);
    EARC_LINK_WR (0x024, 0);

    noti ("eARC Mute : %d\n", on);
}

/* Verified. No caller in the Image (BL/B, ADRP+ADD, 64-bit literal and PREL32
 * scans) and not exported; dead code unless a module calls it by address. */
/* @0xffffffc010cf3e60 */
void audio_hal_earc_set_audio_mode (unsigned int audio_mode)
{
    unsigned int iec958csd1 = 0;
    unsigned int earc_dmac_config0 = 0;

    // Config eARC TX DMAC audio mode
    earc_dmac_config0 = gEarcDmacConfig0;
    earc_dmac_config0 &= ~(7);  // EARCTX_DMAC_AUDIO_MODE_QST   0:2-channel L-PCM, 6:Compressed Layout A, 7:Compressed Layout B
    earc_dmac_config0 |= audio_mode;
    EARC_LINK_WR (0x004, earc_dmac_config0);
    gEarcDmacConfig0 = earc_dmac_config0;

    // compressed layout A/B
    iec958csd1 = gEarcIec958csd1;
    iec958csd1 &= ~(0xF << 12); // 0: compressed layout A, 7:compressed layout B
    if (audio_mode == 7)
        iec958csd1 |= (0x7 << 12);      // 0: compressed layout A, 7:compressed layout B
    EARC_LINK0_WR (0x004, iec958csd1);
    EARC_LINK_WR (0x014, iec958csd1);
    gEarcIec958csd1 = iec958csd1;

    EARC_LINK0_WR (0x014, 0);
    EARC_LINK_WR (0x024, 0x0);

    audio_hal_earc_monitor_lock ();

    noti ("eARC config : am(%d)\n", audio_mode);
}

/* Verified. Only caller: _set_format +0x78. Uses the PREVIOUS gEarcOutputFormat
 * and gEarcOutputSamplingFreq: PCM->ES 8ch passes through mode 1 (8-ch L-PCM)
 * until set_output_format follows. */
/* @0xffffffc010cf3f24 */
void audio_hal_earc_set_channel (unsigned int channel, unsigned int acmod)
{
    unsigned int iec958csd0 = 0;
    unsigned int iec958csd1 = 0;
    unsigned int iec958csd4 = 0;
    unsigned int earc_dmac_config0 = 0;
    unsigned int earc_dmac_config1 = 0;
    unsigned int earc_pll_bp_clk = 0;
    unsigned int sampling_bit = 0;

    gEarcChannel = channel;
    gEarcAcmod = acmod;

    EARC_LINK_WR (0x00C, 0x00000000);   // dmac audio data en

    // Config eARC TX DMAC audio mode
    earc_dmac_config0 = gEarcDmacConfig0;
    earc_dmac_config0 &= ~(1 << 8);     // EARCTX_DMAC_AUDIO_SOURCE_LAYOUT_QST
    if ((channel == 8) || (channel == 6))
        earc_dmac_config0 |= (1 << 8);  // 0:2-channel, 1:8-Channel
    earc_dmac_config0 &= ~(0x7);        // EARCTX_DMAC_AUDIO_MODE_QST
    if (gEarcOutputFormat == 0)
    {
        if ((channel == 8) || (channel == 6))
            earc_dmac_config0 |= (1);   // 0:2-channel, 1:8-Channel
    }
    else
    {
        if (audio_compressed_layout == 1)
        {
            earc_dmac_config0 |= 1 << 8;        // audio source layout 0 : 2-channel, 1 : 8-channel
            earc_dmac_config0 |= 7;     // compressed layout B
        }
        else
            earc_dmac_config0 |= 6;     // compressed layout A
    }
    EARC_LINK_WR (0x004, earc_dmac_config0);
    gEarcDmacConfig0 = earc_dmac_config0;

    earc_dmac_config1 = gEarcDmacConfig1;
    earc_dmac_config1 &= ~(0xf << 12);
    if ((channel == 8) || (channel == 6))
    {
        if ((acmod == 0) || (acmod == 2))
            earc_dmac_config1 |= (0x0 << 12);   // 0:2, 2:6, 3:8, 4:12, 5:16, 6:32
        else if (acmod == 7)
            earc_dmac_config1 |= (0x2 << 12);   // 0:2, 2:6, 3:8, 4:12, 5:16, 6:32
        else
            earc_dmac_config1 |= (0x3 << 12);   // 0:2, 2:6, 3:8, 4:12, 5:16, 6:32
    }
    earc_dmac_config1 &= ~(0x1);        // EARCTX_DMAC_PHY_BIPHASE_CLK_EN 0: disable
    EARC_LINK_WR (0x008, earc_dmac_config1);
    earc_dmac_config1 |= 0x1;   // EARCTX_DMAC_PHY_BIPHASE_CLK_EN 1:enable
    EARC_LINK_WR (0x008, earc_dmac_config1);
    gEarcDmacConfig1 = earc_dmac_config1;

    iec958csd0 = gEarcIec958csd0;
    iec958csd0 &= ~(0x7 << 3);
    if (gEarcOutputFormat == 0)
    {
        if ((channel == 8) || (channel == 6))
            iec958csd0 |= (0x4 << 3);
    }
    iec958csd0 &= ~(0xF << 20);
    if (gEarcOutputFormat == 0)
    {
        if ((channel == 8) || (channel == 6))
        {
            if ((acmod == 0) || (acmod == 2))
                iec958csd0 |= (0x2 << 20);
            else if (acmod == 7)
                iec958csd0 |= (0x6 << 20);
            else
                iec958csd0 |= (0x8 << 20);
        }
        else
            iec958csd0 |= (0x2 << 20);
    }
    iec958csd0 &= ~(0xCF << 24);        // 24-27, 30, 31
    sampling_bit = audio_hal_earc_get_sampling_bit (gEarcOutputSamplingFreq);
    iec958csd0 |= sampling_bit << 24;   // set bit 0-3(24-27 bit0011000)
    EARC_LINK0_WR (0x000, iec958csd0);
    EARC_LINK_WR (0x010, iec958csd0);
    gEarcIec958csd0 = iec958csd0;
    iec958csd1 = gEarcIec958csd1;
    iec958csd1 &= ~(0xF << 12);
    if (gEarcOutputFormat == 0)
    {
        if ((channel == 8) || (channel == 6))
            iec958csd1 |= (0x7 << 12);  // 0x0: 0x2, 7:8, 0xB:16, 0x3:32
        else if (channel == 16)
            iec958csd1 |= (0xB << 12);  // 0x0: 0x2, 7:8, 0xB:16, 0x3:32
        else if (channel == 32)
            iec958csd1 |= (0x3 << 12);  // 0x0: 0x2, 7:8, 0xB:16, 0x3:32
    }
    else
    {
        if (audio_compressed_layout == 1)
            iec958csd1 |= (0x7 << 12);  // 0: compressed layout A, 7:compressed layout B
    }
    EARC_LINK0_WR (0x004, iec958csd1);
    EARC_LINK_WR (0x014, iec958csd1);
    gEarcIec958csd1 = iec958csd1;
    iec958csd4 = gEarcIec958csd4;
    iec958csd4 &= ~(0xFF << 8);
    if (gEarcOutputFormat == 0)
    {
        if ((channel == 8) || (channel == 6))
        {
            if ((acmod == 0) || (acmod == 2))
                iec958csd4 |= (0x0 << 8);
            else if (acmod == 7)
                iec958csd4 |= (0x0B << 8);
            else
                iec958csd4 |= (0x13 << 8);
        }
    }
    EARC_LINK0_WR (0x010, iec958csd4);
    EARC_LINK_WR (0x020, iec958csd4);
    gEarcIec958csd4 = iec958csd4;
    EARC_LINK0_WR (0x014, 0x0);
    EARC_LINK_WR (0x024, 0x0);

    earc_pll_bp_clk = EARC_PHY_RD (0x030);
    earc_pll_bp_clk &= ~(0x1);  // 0:2-channel , 1:8-channel
    if ((channel == 8) || (channel == 6))
        earc_pll_bp_clk |= 0x1; // 0:2-channel , 1:8-channel
    EARC_PHY_WR (0x030, earc_pll_bp_clk);
    gEarcPllBpClk = earc_pll_bp_clk;

    audio_hal_earc_monitor_lock ();

    noti ("eARC channel : (ch)%d, acmod : %d\n", channel, acmod);
    debug ("eARC dmac config: %08x, %08x\n", earc_dmac_config0, earc_dmac_config1);
}

/* @0xffffffc010cf29e4 */
void audio_hal_earc_enable_dmac (struct timer_list *unused)
{
    // Enable eARC TX DMAC audio datapath.
    if (gAudHalEarcOn)
    {
        EARC_LINK_WR (0x00C, 0x1);
        debug ("Enable DMAC.\n");
    }
}

static DEFINE_TIMER (_gEnableDmacTimer, audio_hal_earc_enable_dmac);

/* @0xffffffc010cf4324 */
void audio_hal_earc_clear_interrupt (unsigned int int_status)
{
    if (int_status & 0x2)
    {
        // Disable eARC TX DMAC audio datapath.
        EARC_LINK_WR (0x00C, 0x0);

        // Enable eARC TX DMAC audio datapath.
        mod_timer (&_gEnableDmacTimer, jiffies + msecs_to_jiffies (1));
    }
    debug ("eARC Int status: %08x\n", int_status);
}

/* @0xffffffc010cf4470 */
void audio_hal_earc_set (void)
{
    audio_hal_earc_set_pll_bp_clock (gEarcOutputSamplingFreq);
    audio_hal_earc_set_on (gAudHalEarcOn);
}

/* @0xffffffc010cf44a0 */
void audio_hal_speaker_start (bool on)
{
    if (on == false)
    {
        //SPK I2S enable
        CTOP_CTRL_O22Ax_RdFL (FUNC_IOMUX_WOV, pad_wov69);
        CTOP_CTRL_O22Ax_Wr01 (FUNC_IOMUX_WOV, pad_wov69, reg_pad_daclrck_mux_sel, 0x1);
        CTOP_CTRL_O22Ax_Wr01 (FUNC_IOMUX_WOV, pad_wov69, reg_pad_dacsck_mux_sel, 0x1);
        CTOP_CTRL_O22Ax_Wr01 (FUNC_IOMUX_WOV, pad_wov69, reg_pad_daclrch_mux_sel, 0x1);
        CTOP_CTRL_O22Ax_WrFL (FUNC_IOMUX_WOV, pad_wov69);

        AUD_REG_Wr (PERI_CTRL, pcmoutcmda, (on) ? 1 : 0);
        AUD_REG_WrFL (PERI_CTRL, pcmoutcmda);
        AUD_REG_Wr (PERI_CTRL, pcmstarta, (on) ? 1 : 0);
        AUD_REG_WrFL (PERI_CTRL, pcmstarta);
    }
    else
    {
        //SPK I2S enable
        CTOP_CTRL_O22Ax_RdFL (FUNC_IOMUX_WOV, pad_wov69);
        CTOP_CTRL_O22Ax_Wr01 (FUNC_IOMUX_WOV, pad_wov69, reg_pad_daclrck_mux_sel, 0x2);
        CTOP_CTRL_O22Ax_Wr01 (FUNC_IOMUX_WOV, pad_wov69, reg_pad_dacsck_mux_sel, 0x2);
        CTOP_CTRL_O22Ax_Wr01 (FUNC_IOMUX_WOV, pad_wov69, reg_pad_daclrch_mux_sel, 0x2);
        CTOP_CTRL_O22Ax_WrFL (FUNC_IOMUX_WOV, pad_wov69);
    }
}

/* @0xffffffc010cf4530 */
void audio_hal_hp_start (bool on)
{
    AUD_REG_Wr (PERI_CTRL, pcmsuboutcmda, (on) ? 1 : 0);
    AUD_REG_WrFL (PERI_CTRL, pcmsuboutcmda);
    AUD_REG_Wr (PERI_CTRL, pcmsubstarta, (on) ? 1 : 0);
    AUD_REG_WrFL (PERI_CTRL, pcmsubstarta);
}

/* @0xffffffc010cf4560 */
void audio_hal_spdif_start (bool on)
{
    AUD_REG_Wr (PERI_CTRL, iec0starta, (on) ? 1 : 0);
    AUD_REG_WrFL (PERI_CTRL, iec0starta);
}

/* @0xffffffc010cf4584 */
void audio_hal_arc_start (bool on)
{
    AUD_REG_RdFL (PERI_CTRL, earc_s_ctrla);
    AUD_REG_WrFd (PERI_CTRL, earc_s_ctrla, earc_spdif_out, (on) ? 1 : 0);
    AUD_REG_WrFL (PERI_CTRL, earc_s_ctrla);
    AUD_REG_RdFL (PERI_CTRL, earc_s_starta);
    AUD_REG_WrFd (PERI_CTRL, earc_s_starta, earc_spdif_start, (on) ? 1 : 0);
    AUD_REG_WrFL (PERI_CTRL, earc_s_starta);
}


/* From lg-opensource/kdriver/core/aud/hal/o22/hal.c (GPL-2.0). */
#include "dsp_control.h"

/* @0xffffffc010cef130 */
int aud_hal_get_number_of_dsp (void)
{
    return NUM_OF_DSP;          /* 4 for CHIP_NAME_o22 */
}

/* webOS25-shipped: returns false (mov w0, #0; ret) -> NOT_SUPPORTED_DTS. */
/* Only caller: alsa_decoder_codec (BL @0xffffffc010ccd1f0); 1 = "Adec Codec" accepts 0x30..0x35 (DTS*), decoded as AC3. */
/* @0xffffffc010cef140 */
bool aud_hal_dts_supported (void)
{
#ifdef NOT_SUPPORTED_DTS
    return false;
#else
    return true;
#endif
}
