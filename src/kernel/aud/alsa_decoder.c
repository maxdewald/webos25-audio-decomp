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
#include "renderer.h"
#include "blind.h"
#include "hal/hal.h"
#include "log.h"

/* Partial layouts, with padding for unrecovered fields; not replacements for
 * the complete ALSA decoder/input declarations in their original header. */
struct sndout_decoder_state
{
    struct aud_block_info *dec_info;  /* +0x00 */
    bool is_open;                    /* +0x08 */
    bool is_main_dec;                /* +0x09, set by decoder_set_main_audio_index */
    bool is_started;                 /* +0x0a, "decoder is running" */
    unsigned char gap_0b;
    unsigned int input;              /* +0x0c, enum alsadec_input */
    unsigned char gap_10[4];
    int codec;                       /* +0x14, ALSA codec (enum alsadec_codec) */
    unsigned char gap_18[4];
    int iso_type;                    /* +0x1c */
    int first_lang;                  /* +0x20 */
    unsigned char gap_24[4];
    int second_lang;                 /* +0x28 */
    int ad_type;                     /* +0x2c */
    int prio_ad_type;                /* +0x30 */
    int ac4_group_index;             /* +0x34, 0x20 is sent as -1 */
    int dialog_gain;                 /* +0x38 */
    int dualmono_mode;               /* +0x3c */
    int drc;                        /* +0x40 */
    int downmix;                    /* +0x44 */
    int ac4_ad_on_off;               /* +0x48 */
    int default_prl;                 /* +0x4c, 0x7fffffff = not set */
    int associate_audio_volume;      /* +0x50 */
    bool unknown_54;                 /* +0x54, set -> sub input is never dropped */
    bool main_output;                /* +0x55 */
    bool bypass_output;              /* +0x56 */
    bool hdmi_multi_pcm_out;         /* +0x57 */
    bool is_connected;               /* +0x58, decoder->renderer link made */
    unsigned char gap_59[7];
};                                  /* stride 0x60 */
struct sndout_input_state
{
    struct aud_block_info *input_info;
    int index;                       /* +0x08, sub index (ATP0/1, HDMI port) */
};                                  /* LP64 stride 0x10 */
struct sndout_renderer_state
{
    struct aud_block_info *ren_info;  /* +0x00 */
    bool is_open;                    /* +0x08 */
    bool sync;                       /* +0x09, "sync=%d" */
    unsigned char gap_0a[6];
    bool is_play_paused;             /* +0x10, one-shot: next TP start uses 300 ms idle, not 60 */
    bool master_syncmode;            /* +0x11, registers renderer_master_info_cb */
    unsigned char gap_12[2];
    int offset;                      /* +0x14, "offset=%d" */
};                                  /* stride 0x18 */
/* alsa_input_info @0xffffffc0126051f8, alsa_decoder_info +0x20, alsa_renderer_info +0xe0. */
extern struct sndout_decoder_state alsa_decoder_info[2];
extern struct sndout_input_state alsa_input_info[2];
extern struct sndout_renderer_state alsa_renderer_info[2];
extern struct mutex alsa_decoder_lock;                 /* @0xffffffc0122d1f20 */
extern int main_audio_index;                           /* @0xffffffc0122d1f0c, initially -1 */
extern int adc_port_index;                             /* @0xffffffc0122d1f14, initially 0xff */
extern bool audio_drop_enable;                         /* @0xffffffc0122d1f1c, initially 1 */
extern bool atmos_decoding_enable;                     /* @0xffffffc0122d1fc8, initially 1 */
extern bool ad_on_off;                                 /* @0xffffffc012605318 */

extern enum input_type input_get_type (struct aud_block_info *block);
extern enum audio_decoder_codec input_get_hdmi_codec (struct aud_block_info *block);
extern int input_set_hdmi (struct aud_block_info *block, bool forced_pcm);
extern int input_set_tp (struct aud_block_info *block, enum audio_decoder_codec codec, bool clock_x4);
extern int input_set_type (struct aud_block_info *block, int dec_index, enum input_type type_input,
    int index, bool update);
extern int input_register_event (struct aud_block_info *block, enum input_evt event,
    int (*cb_func) (void *, int, void *), struct aud_block_info *cb_target_block);
extern int input_unregister_event (struct aud_block_info *block, enum input_evt event);
extern int input_get_uid (struct aud_block_info *block);
extern int input_start (struct aud_block_info *block);
extern int input_stop (struct aud_block_info *block);
extern int input_drop_data (struct aud_block_info *block, bool drop);
extern bool input_is_drop_data (struct aud_block_info *block);
extern int input_set_hdmi_bypass (struct aud_block_info *block, bool enable, int port);
extern int input_set_hdmi_multi_pcm_out (struct aud_block_info *block, bool enable);
extern int input_tp_pdec_error_info_cb (void *param, int len, void *cb_param);
extern int decoder_decoded_info_cb (void *param, int len, void *cb_param);
extern int decoder_hdmi_pcm_format_info_cb (void *param, int len, void *cb_param);
extern int decoder_tp_es_exist_info_cb (void *param, int len, void *cb_param);
extern int decoder_set_param (struct aud_block_info *block, char *fmt, ...);
extern int decoder_register_event (struct aud_block_info *block, enum audio_decoder_event event,
    int (*cb_func) (void *, int, void *), int repeat);
extern int decoder_unregister_event (struct aud_block_info *block, enum audio_decoder_event event);
extern int decoder_connect (struct aud_block_info *block);
extern int decoder_set_input (struct aud_block_info *block, enum audio_decoder_input input);
extern enum audio_decoder_input decoder_convert_input (int alsa_input);
extern int decoder_set_main_decoder_property (int index);
extern int decoder_get_downmix_mode (int index);
extern int decoder_get_drc_mode (int index);
extern int renderer_set_param (struct aud_block_info *block, char *fmt, ...);
extern int renderer_lipsync_set_idle (struct aud_block_info *block);
extern int renderer_lipsync_clear_idle (struct aud_block_info *block);
extern int renderer_lipsync_set_idle_time (struct aud_block_info *block, unsigned int msec);
extern int renderer_master_info_cb (void *param, int len, void *cb_param);
extern int renderer_register_event (struct aud_block_info *block, enum audio_renderer_event event,
    int (*cb_func) (void *, int, void *), int repeat);
extern int renderer_unregister_event (struct aud_block_info *block, enum audio_renderer_event event);
extern void renderer_set_basetime_ipc (unsigned int index, unsigned int clock_base_time,
    unsigned int stream_base_time, bool is_set, bool is_all_set);
extern int mixer_set_pcmren_drc_mode (struct aud_block_info *block, int drc_mode);
extern int mixer_set_pcmren_downmix_mode (struct aud_block_info *block, int mode);
extern int mixer_set_pcmren_metadata_type (struct aud_block_info *block, int type);
extern struct aud_block_info *bm_get_block_info_by_index (const char *block_type, int index);
extern struct aud_block_info *bm_create_block (const char *block_type, int index);
extern int bm_destroy_block (struct aud_block_info *block_info);
extern int bm_disconnect_block (struct aud_block_info *block_info_in, int port_in);
extern int aimc_set_property (int uid, const char *fmt, ...);
extern bool sndout_get_atmos_encode_on (void);
extern bool acapture_is_encoder_connected (void);
extern int aud_hal_set_adc_port_number (int port);

logm_define (alsadec, log_level_info);
#define debug(fmt,args...) logm_debug(alsadec,fmt,##args)
#define info(fmt,args...) logm_info(alsadec,fmt,##args)
#define noti(fmt,args...) logm_noti(alsadec,fmt,##args)
#define error(fmt,args...) logm_error(alsadec,fmt,##args)
#define debug_call() debug("ALSA_CALL# %s(): %ld %ld %ld", __func__, ucontrol->value.integer.value[0], ucontrol->value.integer.value[1], ucontrol->value.integer.value[2])

/* ALSA "Adec Codec" values match adec_src_codec_ext_type_t; private aliases for
 * the shipped 25-entry name table @0xffffffc0115eda48. */
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

/* ALSA "Adec Connect" input values; names from alsadec_input_name. */
enum alsadec_input
{
    ALSADEC_IN_UNKNOWN = 0,
    ALSADEC_IN_ATP0 = 1,
    ALSADEC_IN_ATP1 = 2,
    ALSADEC_IN_ADC = 3,
    ALSADEC_IN_AAD = 4,
    ALSADEC_IN_HDMI_PORT0 = 5,
    ALSADEC_IN_HDMI_PORT3 = 8,
    ALSADEC_IN_DISPLAY_PORT0 = 9,       /* rejected by alsa_decoder_connect */
    ALSADEC_IN_DISPLAY_PORT1 = 10,      /* rejected by alsa_decoder_connect */
    ALSADEC_IN_HDMI_FF = 0xff,          /* HDMI sub 0; its log indexes past the name table */
};

/* @0xffffffc0122d2018 */
static const char *alsadec_input_name[12] = {
    "UNKNOWN", "ATP0", "ATP1", "ADC", "AAD", "HDMI_PORT0", "HDMI_PORT1",
    "HDMI_PORT2", "HDMI_PORT3", "DISPLAY_PORT0", "DISPLAY_PORT1", "HDMI_DEFAULT",
};

/* pcmren metadata type by codec - AC3 (table @0xffffffc0115ed9a8, AC3..HEAAC):
 * 0 DD/DD+, 1 AAC, 2 AC4, 3 MAT. */
static const int alsadec_metadata_type[ALSADEC_CODEC_HEAAC - ALSADEC_CODEC_AC3 + 1] = {
    0, 0, 0, 2, 2, 3, 3, 3, 3, 1, 1,
};

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

/* Inlined into alsa_decoder_start; name from its error log. */
static void decoder_set_codec_param (int index)
{
    struct sndout_decoder_state *dec = &alsa_decoder_info[index];
    struct aud_block_info *mixer = bm_get_block_info_by_index ("mixer", 0);

    if (!dec->dec_info)
    {
        error ("decoder is not opened\n");
        return;
    }

    decoder_set_main_decoder_property (index);
    decoder_set_param (dec->dec_info,
        "drc-mode=%d downmix-mode=%d audio-description=%d output-mode=%d",
        dec->drc, dec->downmix, ad_on_off, dec->dualmono_mode);

    switch (dec->codec)
    {
        case ALSADEC_CODEC_AC3:
            decoder_set_param (dec->dec_info, "dec-id=%d", index);
            decoder_set_param (dec->dec_info, "atmos-decoding=%d", 0);
            break;
        case ALSADEC_CODEC_EAC3:
            decoder_set_param (dec->dec_info, "dec-id=%d", index);
            decoder_set_param (dec->dec_info, "atmos-decoding=%d", atmos_decoding_enable);
            goto legacy_ddp_out;
        case ALSADEC_CODEC_AC4:
            decoder_set_param (dec->dec_info, "atmos-decoding=%d", atmos_decoding_enable);
            decoder_set_param (dec->dec_info,
                "iso-type=%d first-lang=%d second-lang=%d ad-type=%d prio-ad-type=%d ad-mixing=%d associate-volume=%d de-gain=%d",
                dec->iso_type, dec->first_lang, dec->second_lang, dec->ad_type,
                dec->prio_ad_type, dec->ac4_ad_on_off, dec->associate_audio_volume, dec->dialog_gain);
            decoder_set_param (dec->dec_info, "pres-group-index=%d",
                dec->ac4_group_index == 0x20 ? -1 : dec->ac4_group_index);
legacy_ddp_out:
            if (sndout_get_atmos_encode_on ())
                decoder_set_param (dec->dec_info, "legacy-ddp-out=0");
            else
                decoder_set_param (dec->dec_info, "legacy-ddp-out=1");
            break;
        case ALSADEC_CODEC_MAT:
            decoder_set_param (dec->dec_info, "dec-id=%d", index);
            break;
        case ALSADEC_CODEC_AAC:
        case ALSADEC_CODEC_HEAAC:
            decoder_set_param (dec->dec_info, "iec-format=1");
            if (dec->default_prl != 0x7fffffff)
                decoder_set_param (dec->dec_info, "default-prl=%d", -(dec->default_prl * 4));
            break;
        case ALSADEC_CODEC_DRA:
            if (alsa_renderer_info[index].is_open)
                renderer_set_param (alsa_renderer_info[index].ren_info,
                    "lowerbound=%d upperbound=%d", 50, 64);
            break;
        default:
            break;
    }

    if (mixer && index == main_audio_index)
    {
        int downmix_mode = decoder_get_downmix_mode (index);

        mixer_set_pcmren_drc_mode (mixer, decoder_get_drc_mode (index));
        mixer_set_pcmren_downmix_mode (mixer, downmix_mode == 0);
        if ((unsigned int) (dec->codec - ALSADEC_CODEC_AC3) <= ALSADEC_CODEC_HEAAC - ALSADEC_CODEC_AC3)
        {
            int type = alsadec_metadata_type[dec->codec - ALSADEC_CODEC_AC3];

            mixer = bm_get_block_info_by_index ("mixer", 0);
            if (mixer)
                mixer_set_pcmren_metadata_type (mixer, type);
        }
    }
}

/* Inlined into alsa_decoder_start; name from its logs. */
static void decoder_config (int index)
{
    struct sndout_decoder_state *dec = &alsa_decoder_info[index];
    struct aud_block_info *input = alsa_input_info[index].input_info;
    enum input_type type = INPUT_MAX;

    if (input)
        type = input_get_type (input);

    decoder_set_codec_param (index);

    if (type == INPUT_HDMI)
    {
        if (!input)
            return;
        input_set_hdmi (input, dec->codec == ALSADEC_CODEC_PCM);
        input_register_event (input, INPUT_EVT_HDMI_PCM_FORMAT, decoder_hdmi_pcm_format_info_cb,
            dec->dec_info);
    }
    else if (type == INPUT_TP)
    {
        if (!input)
            return;
        input_set_tp (input, alsadec_convert_codec (dec->codec),
            (unsigned int) (dec->codec - ALSADEC_CODEC_EAC3) <= ALSADEC_CODEC_EAC3_ATMOS - ALSADEC_CODEC_EAC3);
        input_register_event (input, INPUT_EVT_TP_ES_EXIST_INFO, decoder_tp_es_exist_info_cb,
            dec->dec_info);
        input_register_event (input, INPUT_EVT_TP_PDEC_ERROR_INFO, input_tp_pdec_error_info_cb,
            input);
        if (ad_on_off && index != main_audio_index)
        {
            aimc_set_property (input_get_uid (input), "discard-packet=%d", 7);
            noti ("index %d, discard-packet %d .\n", index, 7);
        }
    }
    else if (type == INPUT_ADC)
    {
        noti ("adc_port_index %d .\n", adc_port_index);
        aud_hal_set_adc_port_number (adc_port_index);
    }
}

/* "Adec Start" kcontrol .put (value[0] = index). Needs open + connected input + codec set. */
/* @0xffffffc010ccc750 */
static int alsa_decoder_start (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    int index = ucontrol->value.integer.value[0];
    struct sndout_decoder_state *dec;
    struct sndout_renderer_state *ren;
    struct aud_block_info *input;
    int ret = -1;

    debug_call ();
    if ((unsigned int) index > 1)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }

    mutex_lock (&alsa_decoder_lock);
    dec = &alsa_decoder_info[index];
    if (!dec->is_open)
    {
        error ("can not find audio decoder, index %d .\n", index);
        goto out;
    }
    if (!dec->input)
    {
        error ("audio decoder is not connected to input, index %d .\n", index);
        goto out;
    }
    if (!dec->codec)
    {
        error ("audio decoder codec is not setted, index %d .\n", index);
        goto out;
    }
    if (dec->is_started)
    {
        error ("audio decoder is already started, index %d .\n", index);
        goto out;
    }

    info ("dec%d is starting..\n", index);
    decoder_config (index);

    decoder_register_event (dec->dec_info, AUDIO_DECODER_EVENT_INFO, decoder_decoded_info_cb, true);
    decoder_connect (dec->dec_info);
    if (!dec->is_connected)
    {
        bm_connect_block (dec->dec_info, 0, alsa_renderer_info[index].ren_info, 0);
        dec->is_connected = true;
    }

    input = alsa_input_info[index].input_info;
    if (input)
    {
        enum audio_decoder_codec dec_codec = alsadec_convert_codec (dec->codec);
        enum audio_decoder_codec input_codec = AUDIO_DECODER_CODEC_PCM;
        bool valid_codec = false;
        enum input_type type = input_get_type (input);

        /* HDMI DTS: hdmi_codec stays PCM and DTS converts to AC3, so it is never started/un-dropped. */
        if (type == INPUT_TP)
            valid_codec = true;
        else
        {
            if (type == INPUT_HDMI)
                input_codec = input_get_hdmi_codec (input);
            if (input_codec != AUDIO_DECODER_CODEC_PCM || dec_codec == AUDIO_DECODER_CODEC_PCM)
                valid_codec = true;
        }

        if (valid_codec)
        {
            input_start (input);
            input_drop_data (input, false);
        }
        else
            noti ("decoder_start index %d valid_codec %d dec_codec %d input_codec %d.\n",
                index, valid_codec, dec_codec, input_codec);
    }

    ren = &alsa_renderer_info[index];
    if (ren->is_open)
    {
        if (dec->input == ALSADEC_IN_ATP0 || dec->input == ALSADEC_IN_ATP1)
        {
            renderer_set_param (ren->ren_info, "sync=%d", ren->sync);
            renderer_set_param (ren->ren_info, "presented-pts-index=%d", index);
            if (ren->is_play_paused)
            {
                renderer_lipsync_set_idle_time (ren->ren_info, 300);
                ren->is_play_paused = false;
            }
            else
                renderer_lipsync_set_idle_time (ren->ren_info, 60);
            if (ren->master_syncmode)
            {
                renderer_set_param (ren->ren_info, "offset=%d", ren->offset);
                renderer_set_param (ren->ren_info, "clock_type=%d", RENDERER_CLK_TYPE_MASTER);
                renderer_register_event (ren->ren_info, AUDIO_RENDERER_EVENT_MASTER_INFO,
                    renderer_master_info_cb, true);
            }
        }
        if (sndout_decoder_is_hdmi (dec->input))
            renderer_set_param (ren->ren_info, "discard_param=%d", 11);
        renderer_lipsync_set_idle (ren->ren_info);
    }

    dec->is_started = true;
    info ("decoder_start index %d .\n", index);
    ret = 0;
out:
    mutex_unlock (&alsa_decoder_lock);
    return ret;
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
        && input_get_type (alsa_input_info[index].input_info) == INPUT_SIF
        && dec_codec != AUDIO_DECODER_CODEC_PCM)
    {
        noti ("sif codec error index %d codec %d (%s) .\n", index, codec, alsadec_codec_str (codec));
        goto out;
    }
    decoder_set_codec (dec->dec_info, dec_codec);
    if (alsa_input_info[index].input_info
        && input_get_type (alsa_input_info[index].input_info) == INPUT_HDMI)
        bm_connect_block (dec->dec_info, 0x100, alsa_renderer_info[index].ren_info, 0x100);
    else
        bm_connect_block (dec->dec_info, 1, alsa_renderer_info[index].ren_info, 0x100);
    info ("alsa_decoder_codec index %d codec %d (%s) .\n", index, codec, alsadec_codec_str (codec));
out:
    mutex_unlock (&alsa_decoder_lock);
    return ret;
}

/* Body of decoder_set_input_drop (outer wrapper's checks not recovered); caller holds
 * alsa_decoder_lock. drop = audio_drop_enable. A main decoder is always un-dropped. */
/* @0xffffffc010ccd624 */
static void decoder_set_input_drop (int index, bool drop)
{
    struct sndout_decoder_state *dec = &alsa_decoder_info[index];
    struct sndout_renderer_state *ren = &alsa_renderer_info[index];
    struct aud_block_info *input = alsa_input_info[index].input_info;

    if (!dec->is_open)
    {
        info ("can not find audio decoder, index %d .\n", index);
        return;
    }
    if (!input)
    {
        info ("decoder is not connected yet .\n");
        return;
    }
    if (!drop)
    {
        input_drop_data (input, false);
        return;
    }

    if (dec->is_main_dec)
    {
        if (main_audio_index == -1)
            return;
        if ((input_get_type (input) == INPUT_TP || input_get_type (input) == INPUT_HDMI)
            && input_is_drop_data (input))
        {
            if (ren->is_open)
                renderer_lipsync_clear_idle (ren->ren_info);
            input_stop (input);
            input_start (input);
            if (ren->is_open)
                renderer_lipsync_set_idle (ren->ren_info);
            noti ("restart input, drop on to off \n");
        }
        input_drop_data (input, false);
        noti ("main adec:%d, drop : false\n", index);
        return;
    }

    if (input_get_type (input) == INPUT_SIF && main_audio_index <= 1)
    {
        struct aud_block_info *main_input;

        if (!atmos_decoding_enable || acapture_is_encoder_connected ())
            return;
        /* main_audio_index -1 is used as an index here too (shipped). */
        main_input = alsa_input_info[main_audio_index].input_info;
        if (!main_input || input_get_type (main_input) == INPUT_SIF || dec->unknown_54)
            return;
        noti ("INPUT_SIF is dropped by atmos_decoding_enable.(sub adec:%d)\n", index);
        input_drop_data (input, true);
        return;
    }

    if (ad_on_off || dec->unknown_54)
        return;
    input_drop_data (input, true);
    noti ("sub audio(%d) is dropped by alsa_decoder_ad_on_off(%d).\n", index, ad_on_off);
}

/* "Adec Audio Drop Enable" kcontrol .put (value[0] != 0); applies to both decoders. */
/* @0xffffffc010ccda14 */
static int alsa_decoder_audio_drop_enable (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    bool enable = ucontrol->value.integer.value[0] != 0;

    debug_call ();
    if (audio_drop_enable == enable)
    {
        info ("same, audio drop enable : %s\n", enable ? "true" : "false");
        return 0;
    }

    mutex_lock (&alsa_decoder_lock);
    decoder_set_input_drop (0, enable);
    decoder_set_input_drop (1, enable);
    audio_drop_enable = enable;
    mutex_unlock (&alsa_decoder_lock);
    info ("alsa_decoder_audio_drop_enable %d.\n", enable);
    return 0;
}

/* "Adec Stop" kcontrol .put (value[0] = index). A SIF input shared with the encoder or
 * the main decoder is left running and undropped. */
/* @0xffffffc010ccdbc0 */
static int alsa_decoder_stop (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    int index = ucontrol->value.integer.value[0];
    struct sndout_decoder_state *dec;
    struct sndout_renderer_state *ren;
    struct aud_block_info *input;
    int ret = -1;

    debug_call ();
    if ((unsigned int) index > 1)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }

    mutex_lock (&alsa_decoder_lock);
    dec = &alsa_decoder_info[index];
    if (!dec->is_open)
    {
        error ("can not find audio decoder, index %d .\n", index);
        goto out;
    }
    if (!dec->is_started)
    {
        error ("audio decoder is not started, index %d .\n", index);
        goto out;
    }

    input = alsa_input_info[index].input_info;
    if (input)
    {
        enum input_type type = input_get_type (input);

        if (type == INPUT_HDMI)
            input_unregister_event (input, INPUT_EVT_HDMI_PCM_FORMAT);
        else if (type == INPUT_TP)
        {
            input_unregister_event (input, INPUT_EVT_TP_ES_EXIST_INFO);
            input_unregister_event (input, INPUT_EVT_TP_PDEC_ERROR_INFO);
        }
        else if (type == INPUT_SIF)
        {
            if (main_audio_index == index)
            {
                if (acapture_is_encoder_connected ())
                {
                    noti ("MAIN INPUT_SIF is used by encoder module.\n");
                    goto input_done;
                }
            }
            else if ((unsigned int) main_audio_index <= 1
                && alsa_input_info[main_audio_index].input_info
                && input_get_type (alsa_input_info[main_audio_index].input_info) == INPUT_SIF)
            {
                noti ("SUB INPUT_SIF is used by MAIN INPUT_SIF.\n");
                goto input_done;
            }
        }
        input_stop (input);
        input_drop_data (input, true);
    }
input_done:

    if ((unsigned int) (dec->codec - ALSADEC_CODEC_AC4) <= ALSADEC_CODEC_AC4_ATMOS - ALSADEC_CODEC_AC4)
        decoder_set_param (dec->dec_info, "pres-group-index=-1");

    ren = &alsa_renderer_info[index];
    renderer_lipsync_clear_idle (ren->ren_info);
    if (ren->master_syncmode)
    {
        renderer_unregister_event (ren->ren_info, AUDIO_RENDERER_EVENT_MASTER_INFO);
        renderer_set_basetime_ipc (index, -1, -1, false, false);
    }

    decoder_unregister_event (dec->dec_info, AUDIO_DECODER_EVENT_INFO);
    if (dec->is_connected)
    {
        bm_disconnect_block (ren->ren_info, 0);
        bm_disconnect_block (ren->ren_info, 0x100);
        dec->is_connected = false;
    }

    dec->is_started = false;
    info ("alsa_decoder_stop index %d .\n", index);
    ret = 0;
out:
    mutex_unlock (&alsa_decoder_lock);
    return ret;
}

/* "Adec Connect" kcontrol .put (value[0] = index, value[1] = enum alsadec_input).
 * dec->input and decoder_set_input are already set when the input type is rejected. */
/* @0xffffffc010cce650 */
static int alsa_decoder_connect (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    int index = ucontrol->value.integer.value[0];
    unsigned int alsa_input = ucontrol->value.integer.value[1];
    struct sndout_decoder_state *dec;
    struct sndout_renderer_state *ren;
    struct aud_block_info *input;
    enum input_type type;
    int sub_index = alsa_input - 1;
    enum RENDERER_CLK_TYPE clock_type;
    bool is_hdmi = false;
    int ret = -1;

    debug_call ();
    ablind_post ("Auto Adec Connect", 2, index, alsa_input);
    if ((unsigned int) index > 1)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }

    mutex_lock (&alsa_decoder_lock);
    dec = &alsa_decoder_info[index];
    ren = &alsa_renderer_info[index];
    if (!dec->is_open)
    {
        error ("can not find audio decoder, index %d .\n", index);
        goto out;
    }
    if (!ren->is_open)
    {
        error ("can not find audio renderer, index %d .\n", index);
        goto out;
    }
    if (dec->input)
    {
        error ("audio decoder is already connected to input, index %d input %d .\n", index, dec->input);
        goto out;
    }
    if (alsa_input - 1 > 0xfe)
    {
        error ("audio decoder input is error, index %d input %d .\n", index, alsa_input);
        goto out;
    }

    dec->input = alsa_input;
    decoder_set_input (dec->dec_info, decoder_convert_input (alsa_input));

    if (alsa_input == ALSADEC_IN_AAD)
    {
        /* SIF block 100 is owned by the sif module, not created here. */
        input = bm_get_block_info_by_index ("input", INPUT_SIF_INDEX);
        alsa_input_info[index].input_info = input;
        if (!input)
        {
            error ("error in get input block by index\n");
            goto out;
        }
        sub_index = 0;
        input_set_type (input, index, INPUT_SIF, 0, true);
    }
    else
    {
        if (alsa_input <= ALSADEC_IN_ATP1)
            type = INPUT_TP;
        else if (alsa_input == ALSADEC_IN_ADC)
        {
            type = INPUT_ADC;
            sub_index = 0;
        }
        else if (alsa_input <= ALSADEC_IN_HDMI_PORT3)
        {
            type = INPUT_HDMI;
            sub_index = alsa_input - ALSADEC_IN_HDMI_PORT0;
            is_hdmi = true;
        }
        else if (alsa_input == ALSADEC_IN_HDMI_FF)
        {
            type = INPUT_HDMI;
            sub_index = 0;
            is_hdmi = true;
        }
        else
        {
            error ("invalid input type, input %d .\n", alsa_input);
            goto out;
        }

        input = alsa_input_info[index].input_info;
        if (input)
        {
            noti ("input%d (%p) already exist!!\n", index, input);
            if (input_get_type (input) == INPUT_SIF)
                debug ("INPUT_SIF is allocated in sif module.\n");
            else
                bm_destroy_block (input);
        }
        input = bm_create_block ("input", index);
        alsa_input_info[index].input_info = input;
        if (!input)
        {
            error ("error in create input block\n");
            goto out;
        }
        input_set_type (input, index, type, sub_index, false);
    }

    alsa_input_info[index].index = sub_index;
    input = alsa_input_info[index].input_info;
    clock_type = RENDERER_CLK_TYPE_GSTC;
    if (input)
    {
        if (input_get_type (input) != INPUT_SIF)
            input_drop_data (input, true);
        else if (main_audio_index == index)
        {
            if (acapture_is_encoder_connected ())
                noti ("MAIN INPUT_SIF is used by encoder module.\n");
            else
                input_drop_data (input, true);
        }
        else if ((unsigned int) main_audio_index <= 1
            && alsa_input_info[main_audio_index].input_info
            && input_get_type (alsa_input_info[main_audio_index].input_info) == INPUT_SIF)
            noti ("SUB INPUT_SIF is used by MAIN INPUT_SIF.\n");
        else
            input_drop_data (input, true);

        input = alsa_input_info[index].input_info;
        if (input && input_get_type (input) == INPUT_TP)
            clock_type = RENDERER_CLK_TYPE_PCR_M + alsa_input_info[index].index;
    }
    renderer_set_param (ren->ren_info, "clock_type=%d", clock_type);

    if (!dec->is_connected)
    {
        input = alsa_input_info[index].input_info;
        bm_connect_block (input, 0, dec->dec_info, 0);
        bm_connect_block (dec->dec_info, 0, ren->ren_info, 0);
        if (is_hdmi)
        {
            bm_connect_block (input, 0x100, dec->dec_info, 0x100);
            /* Differs from decoder_set_output_mode: main-only (1,0) and none (0,0) send nothing. */
            if (dec->main_output)
            {
                if (dec->bypass_output)
                    input_set_hdmi_bypass (input, false, 0);
            }
            else if (dec->bypass_output)
                input_set_hdmi_bypass (input, true, 0);
            input_set_hdmi_multi_pcm_out (input, dec->hdmi_multi_pcm_out);
        }
        dec->is_connected = true;
    }

    info ("alsa_decoder_connect index %d input %d (%s) .\n", index, alsa_input,
        alsadec_input_name[alsa_input]);
    ret = 0;
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
