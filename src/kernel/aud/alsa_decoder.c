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
 * Focused reconstruction of the sound-output helpers in alsa/alsa_decoder.c.
 * Image-5.4.268-329.decompressed, 5.4.268-329.ptl4tv.5, SHA256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Evidence: build/scratch/seek-chain/{decoder_set_output_mode,
 * decoder_set_multi_pcm_out}.asm. Unlike the published output_drop enum, the
 * shipped decoder has independent main/bypass enable bytes +0x55/+0x56.
 */
#include <linux/mutex.h>
#include <linux/types.h>
#include <sound/control.h>
#include "block.h"
#include "block_manager.h"
#include "input.h"
#include "decoder.h"
#include "blind.h"
#include "hal/hal.h"
#include "log.h"

/* Partial layouts, with padding for unrecovered fields; not replacements for
 * the complete ALSA decoder/input declarations in their original header. */
struct sndout_decoder_state
{
    struct aud_block_info *dec_info;  /* +0x00 */
    bool is_open;                    /* +0x08 */
    unsigned char gap_09;
    bool is_started;                 /* +0x0a, "decoder is running" */
    unsigned char gap_0b;
    unsigned int input;              /* +0x0c */
    unsigned char gap_10[4];
    int codec;                       /* +0x14, ALSA codec (enum alsadec_codec) */
    unsigned char gap_18[0x3d];
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
struct sndout_renderer_state
{
    struct aud_block_info *ren_info;  /* +0x00 */
    bool is_open;                    /* +0x08 */
    unsigned char gap_09[0xf];
};                                  /* stride 0x18 */
extern struct sndout_decoder_state alsa_decoder_info[2];
extern struct sndout_input_state alsa_input_info[2];
extern struct sndout_renderer_state alsa_renderer_info[2];
extern struct mutex alsa_decoder_lock;

logm_define (alsadec, log_level_info);
#define debug(fmt,args...) logm_debug(alsadec,fmt,##args)
#define info(fmt,args...) logm_info(alsadec,fmt,##args)
#define noti(fmt,args...) logm_noti(alsadec,fmt,##args)
#define error(fmt,args...) logm_error(alsadec,fmt,##args)
#define debug_call() debug("ALSA_CALL# %s(): %ld %ld %ld", __func__, ucontrol->value.integer.value[0], ucontrol->value.integer.value[1], ucontrol->value.integer.value[2])

/* ALSA "Adec Codec" values; names from the shipped 25-entry name table
 * @0xffffffc0115eda48 (our enum name). */
enum alsadec_codec
{
    ALSADEC_CODEC_UNKNOWN = 0x00,
    ALSADEC_CODEC_PCM = 0x01,
    ALSADEC_CODEC_AC3 = 0x10,
    ALSADEC_CODEC_EAC3 = 0x11,
    ALSADEC_CODEC_EAC3_ATMOS = 0x12,
    ALSADEC_CODEC_AC4 = 0x13,
    ALSADEC_CODEC_AC4_ATMOS = 0x14,
    ALSADEC_CODEC_MAT = 0x15,
    ALSADEC_CODEC_MAT_ATMOS = 0x16,
    ALSADEC_CODEC_TRUEHD = 0x17,
    ALSADEC_CODEC_TRUEHD_ATMOS = 0x18,
    ALSADEC_CODEC_AAC = 0x19,
    ALSADEC_CODEC_HEAAC = 0x1a,
    ALSADEC_CODEC_MPEG = 0x20,
    ALSADEC_CODEC_MPEG_H = 0x21,
    ALSADEC_CODEC_DRA = 0x22,
    ALSADEC_CODEC_DTS = 0x30,
    ALSADEC_CODEC_DTS_HD_MA = 0x31,
    ALSADEC_CODEC_DTS_EXPRESS = 0x32,
    ALSADEC_CODEC_DTS_HD = 0x33,
    ALSADEC_CODEC_DTS_X_P1 = 0x34,
    ALSADEC_CODEC_DTS_X_P2 = 0x35,
    ALSADEC_CODEC_SIF = 0x40,
    ALSADEC_CODEC_SIF_BTSC = 0x41,
    ALSADEC_CODEC_SIF_A2 = 0x42,
};

/* input_get_type() values seen here and in alsa_decoder_start; names inferred. */
#define ALSADEC_INPUT_SIF  0
#define ALSADEC_INPUT_HDMI 2

/* Inlined name lookup (table @0xffffffc0115eda48, "UNKNOWN" when absent). */
static const char *alsadec_codec_str (int codec)
{
    switch (codec)
    {
        case ALSADEC_CODEC_UNKNOWN: return "UNKNOWN";
        case ALSADEC_CODEC_PCM: return "PCM";
        case ALSADEC_CODEC_AC3: return "AC3";
        case ALSADEC_CODEC_EAC3: return "EAC3";
        case ALSADEC_CODEC_EAC3_ATMOS: return "EAC3_ATMOS";
        case ALSADEC_CODEC_AC4: return "AC4";
        case ALSADEC_CODEC_AC4_ATMOS: return "AC4_ATMOS";
        case ALSADEC_CODEC_MAT: return "MAT";
        case ALSADEC_CODEC_MAT_ATMOS: return "MAT_ATMOS";
        case ALSADEC_CODEC_TRUEHD: return "TRUEHD";
        case ALSADEC_CODEC_TRUEHD_ATMOS: return "TRUEHD_ATMOS";
        case ALSADEC_CODEC_AAC: return "AAC";
        case ALSADEC_CODEC_HEAAC: return "HEAAC";
        case ALSADEC_CODEC_MPEG: return "MPEG";
        case ALSADEC_CODEC_MPEG_H: return "MPEG_H";
        case ALSADEC_CODEC_DRA: return "DRA";
        case ALSADEC_CODEC_DTS: return "DTS";
        case ALSADEC_CODEC_DTS_HD_MA: return "DTS_HD_MA";
        case ALSADEC_CODEC_DTS_EXPRESS: return "DTS_EXPRESS";
        case ALSADEC_CODEC_DTS_HD: return "DTS_HD";
        case ALSADEC_CODEC_DTS_X_P1: return "DTS_X_P1";
        case ALSADEC_CODEC_DTS_X_P2: return "DTS_X_P2";
        case ALSADEC_CODEC_SIF: return "SIF";
        case ALSADEC_CODEC_SIF_BTSC: return "SIF_BTSC";
        case ALSADEC_CODEC_SIF_A2: return "SIF_A2";
        default: return "UNKNOWN";
    }
}

/* Inlined 0x42-byte table @0xffffffc0115ed9d8 (index codec-1). Every DTS value,
 * like any unlisted one, maps to AC3: this build has no DTS decoder codec. */
static enum audio_decoder_codec alsadec_convert_codec (int codec)
{
    switch (codec)
    {
        case ALSADEC_CODEC_PCM:
        case ALSADEC_CODEC_SIF:
        case ALSADEC_CODEC_SIF_BTSC:
        case ALSADEC_CODEC_SIF_A2:
            return AUDIO_DECODER_CODEC_PCM;
        case ALSADEC_CODEC_EAC3:
        case ALSADEC_CODEC_EAC3_ATMOS:
            return AUDIO_DECODER_CODEC_EAC3;
        case ALSADEC_CODEC_AC4:
        case ALSADEC_CODEC_AC4_ATMOS:
            return AUDIO_DECODER_CODEC_AC4;
        case ALSADEC_CODEC_MAT:
        case ALSADEC_CODEC_MAT_ATMOS:
        case ALSADEC_CODEC_TRUEHD:          /* MAT, not TRUE_HD */
        case ALSADEC_CODEC_TRUEHD_ATMOS:
            return AUDIO_DECODER_CODEC_MAT;
        case ALSADEC_CODEC_AAC:
        case ALSADEC_CODEC_HEAAC:
            return AUDIO_DECODER_CODEC_AAC;
        case ALSADEC_CODEC_MPEG:
            return AUDIO_DECODER_CODEC_MP3;
        case ALSADEC_CODEC_MPEG_H:
            return AUDIO_DECODER_CODEC_MPEG_H;
        case ALSADEC_CODEC_DRA:
            return AUDIO_DECODER_CODEC_DRA;
        default:
            return AUDIO_DECODER_CODEC_AC3;
    }
}

/* "Adec Codec" kcontrol .put (MIXER iface, entry @0xffffffc0115ede50; value[0] =
 * index, value[1] = enum alsadec_codec). Sole caller of aud_hal_dts_supported():
 * gate 0 -> DTS..DTS_X_P2 rejected like codec 0 (-1, stored codec reset to 0);
 * gate 1 -> accepted, stored as-is (Adec Codec get / Adec Info / proc show it)
 * but the decoder is still set to AUDIO_DECODER_CODEC_AC3. The codec is stored
 * before the open checks and kept on those failures. SIF input + non-PCM -> 0.
 * "ADEC codec 48/51" (dts_restore_plus) = ALSADEC_CODEC_DTS / _DTS_HD here; the shipped
 * aud_hal_dts_supported() returns 0, so they are rejected and stored as 0. The value is
 * also kept at +0x14 for alsa_decoder_start, which uses it for input_set_hdmi(forced_pcm = (== PCM)). */
/* @0xffffffc010ccd124 */
static int alsa_decoder_codec (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    int index = ucontrol->value.integer.value[0];
    int codec = ucontrol->value.integer.value[1];
    struct sndout_decoder_state *dec;
    enum audio_decoder_codec dec_codec;
    int ret = -1;

    debug_call ();
    ablind_post ("Auto Adec Codec", 2, index, codec);
    if ((unsigned int) index > 1)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }

    mutex_lock (&alsa_decoder_lock);
    dec = &alsa_decoder_info[index];
    dec->codec = codec;
    if (!dec->is_open)
    {
        error ("can not find audio decoder, index %d .\n", index);
        goto out;
    }
    if (!alsa_renderer_info[index].is_open)
    {
        error ("can not find audio renderer, index %d .\n", index);
        goto out;
    }
    if (codec == ALSADEC_CODEC_UNKNOWN
        || (!aud_hal_dts_supported ()
            && (unsigned int) (codec - ALSADEC_CODEC_DTS) <= ALSADEC_CODEC_DTS_X_P2 - ALSADEC_CODEC_DTS))
    {
        error ("codec %d (%s) is not suppported\n", codec, alsadec_codec_str (codec));
        dec->codec = ALSADEC_CODEC_UNKNOWN;
        goto out;
    }
    if (decoder_is_created (dec->dec_info) && dec->is_started)
    {
        error ("decoder is running, index %d .\n", index);
        goto out;
    }

    dec_codec = alsadec_convert_codec (codec);
    ret = 0;
    if (alsa_input_info[index].input_info
        && input_get_type (alsa_input_info[index].input_info) == ALSADEC_INPUT_SIF
        && dec_codec != AUDIO_DECODER_CODEC_PCM)
    {
        noti ("sif codec error index %d codec %d (%s) .\n", index, codec, alsadec_codec_str (codec));
        goto out;
    }
    decoder_set_codec (dec->dec_info, dec_codec);
    if (alsa_input_info[index].input_info
        && input_get_type (alsa_input_info[index].input_info) == ALSADEC_INPUT_HDMI)
        bm_connect_block (dec->dec_info, 0x100, alsa_renderer_info[index].ren_info, 0x100);
    else
        bm_connect_block (dec->dec_info, 1, alsa_renderer_info[index].ren_info, 0x100);
    info ("alsa_decoder_codec index %d codec %d (%s) .\n", index, codec, alsadec_codec_str (codec));
out:
    mutex_unlock (&alsa_decoder_lock);
    return ret;
}

enum decoder_output_mode
{
    DECODER_OUTPUT_MODE_MAIN = 0,
    DECODER_OUTPUT_MODE_BYPASS = 1,
};

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
