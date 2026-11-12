// SPDX-License-Identifier: GPL-2.0-only
/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation. This program is distributed WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 */
/*
 * Decoder/UserCodec conversion and sound-output helpers from alsa/alsa_decoder.c.
 * Image-5.4.268-329.decompressed, 5.4.268-329.ptl4tv.5, SHA256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Evidence: build/scratch/kernel-hdmi-setup/{alsa_decoder_codec,
 * alsa_decoder_start}.asm and build/scratch/seek-chain/{decoder_set_output_mode,
 * decoder_set_multi_pcm_out}.asm. Unlike the published output_drop enum, the
 * shipped decoder has independent main/bypass enable bytes +0x55/+0x56.
 */
#include <linux/mutex.h>
#include <linux/types.h>
#include "block.h"
#include "input.h"
#include "log.h"

/* Partial layouts, with padding for unrecovered fields; not replacements for
 * the complete ALSA decoder/input declarations in their original header. */
struct sndout_decoder_state
{
    struct aud_block_info *dec_info;  /* +0x00 */
    bool is_open;                    /* +0x08 */
    unsigned char gap_09[3];
    unsigned int input;              /* +0x0c */
    unsigned char gap_10[0x45];
    bool main_output;                /* +0x55 */
    bool bypass_output;              /* +0x56 */
    bool hdmi_multi_pcm_out;         /* +0x57 */
    unsigned char gap_58[8];
};                                  /* stride 0x60 */
struct sndout_input_state
{
    struct aud_block_info *input_info;
    int index;
};                                  /* LP64 stride 0x10 */
extern struct sndout_decoder_state alsa_decoder_info[2];
extern struct sndout_input_state alsa_input_info[2];
extern struct mutex alsa_decoder_lock;

logm_define (alsadec, log_level_info);
#define info(fmt,args...) logm_info(alsadec,fmt,##args)
#define error(fmt,args...) logm_error(alsadec,fmt,##args)

enum decoder_output_mode
{
    DECODER_OUTPUT_MODE_MAIN = 0,
    DECODER_OUTPUT_MODE_BYPASS = 1,
};

/* Recovered inline conversion in alsa_decoder_codec (+0x110..+0x124) and
 * alsa_decoder_start, NOT a separate native symbol or a DSP codec converter.
 * The load uses (unsigned int)(UserCodec - 1) <= 65 and the byte table at
 * 0xffffffc0115ed9d8 (ADRP 0xffffffc0115ec000 + 0xd70 + 0xc68).
 * The earlier scratch label 0xffffffc0115ee9d8 was wrong by one page.
 * Exhaustive ADRP/ADD scan of kernel text: only users are alsa_decoder_codec
 * @0xffffffc010ccd244 and alsa_decoder_start @0xffffffc010cccaa8 (start
 * check) and @0xffffffc010cccd34 (TP codec); all use the same bound.
 * Numeric kernel results deliberately do not borrow a DTS-enabled enum:
 * UserCodec 48..53 (DTS variants) all map to kernel AC3=2 even if the separate
 * aud_hal_dts_supported() rejection is bypassed. This does not enable DTS/HBR.
 */
static inline enum audio_decoder_codec alsa_decoder_kernel_codec (int user_codec)
{
    switch (user_codec)
    {
        case 1: case 64: case 65: case 66: return 1;
        case 17: case 18: return 3;
        case 19: case 20: return 4;
        case 21: case 22: case 23: case 24: return 5;
        case 25: case 26: return 7;
        case 32: return 8;
        case 33: return 9;
        case 34: return 10;
        default: return 2;
    }
}

/* This is the shipped unsigned range test, not a single HDMI enum comparison. */
static bool sndout_decoder_is_hdmi (unsigned int input)
{
    return input - 5 <= 250;
}

/* Invalid index, unchanged flag, closed decoder or missing input -> -1.
 * An unknown mode still reaches input selection without changing either flag.
 * main/bypass = 00: drop sub; 10: drop sub; 01: drop main; 11: drop neither.
 * It changes HDMI input data-drop-mode, NOT renderer drop-output-data and NOT
 * the aud_output_info bypass flag. SYSTEM/media input does not send a command. */
/* @0xffffffc010ccf4a0 */
int decoder_set_output_mode (int index, enum decoder_output_mode mode, bool enable)
{
    struct sndout_decoder_state *dec;
    struct aud_block_info *input;
    int ret = -1;

    if ((unsigned int) index > 1)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }

    mutex_lock (&alsa_decoder_lock);
    dec = &alsa_decoder_info[index];
    if ((mode == DECODER_OUTPUT_MODE_MAIN && dec->main_output == enable)
        || (mode == DECODER_OUTPUT_MODE_BYPASS && dec->bypass_output == enable))
    {
        info ("SAME output_mode %d, enable %d .\n", mode, enable);
        goto done;
    }
    if (mode == DECODER_OUTPUT_MODE_MAIN)
        dec->main_output = enable;
    else if (mode == DECODER_OUTPUT_MODE_BYPASS)
        dec->bypass_output = enable;

    if (!dec->is_open)
        goto done;
    input = alsa_input_info[index].input_info;
    if (!input)
    {
        error ("audio decoder is not connected to input, index %d .\n", index);
        goto done;
    }

    ret = 0;
    if (sndout_decoder_is_hdmi (dec->input))
    {
        if (!dec->bypass_output)
            input_set_hdmi_bypass (input, true, 1);
        else if (!dec->main_output)
            input_set_hdmi_bypass (input, true, 0);
        else
            input_set_hdmi_bypass (input, false, 0);
    }
done:
    mutex_unlock (&alsa_decoder_lock);
    return ret;
}

/* Stores the flag even when closed; no lock or unchanged-value shortcut. */
/* @0xffffffc010ccf700 */
int decoder_set_multi_pcm_out (int index, bool enable)
{
    struct sndout_decoder_state *dec;
    struct aud_block_info *input;

    if ((unsigned int) index > 1)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }
    dec = &alsa_decoder_info[index];
    dec->hdmi_multi_pcm_out = enable;
    if (!dec->is_open)
    {
        error ("can not find audio decoder, index %d .\n", index);
        return -1;
    }
    input = alsa_input_info[index].input_info;
    if (!input)
    {
        error ("audio decoder is not connected to input, index %d .\n", index);
        return -1;
    }
    if (sndout_decoder_is_hdmi (dec->input))
        input_set_hdmi_multi_pcm_out (input, enable);
    return 0;
}
