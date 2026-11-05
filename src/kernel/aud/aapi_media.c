// SPDX-License-Identifier: GPL-2.0-only
/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
*/

/*
 * Modified to match the shipped kernel Image (sha256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db,
 * orig/boot/Image-5.4.268-329.decompressed).
 *
 * Base: lg-opensource/kdriver/core/aud/aapi_media.c (GPL-2.0), complete.
 * All functions below were compared with the shipped disassembly:
 * ARENDERER_INFO_T stride 0x58 (arenderer_info @ 0xffffffc0122cded0),
 * is_open +0x28, clock_type +0x2c, pcm_info +0x30..+0x4f
 * (pcm_bypass +0x48, pcm_multi_ch +0x4c), log object "amedia".
 * No functional differences from the published source were found in the
 * aren_* functions; log line numbers moved (e.g. aren_get_uid lines
 * 1214..1239 vs published 1157..1185).
 *
 * adecoder_* (ADECODER_INFO_T stride 0x38: dec_info +0, lock +8, is_open
 * +0x28, is_started +0x29, codec +0x2c, proc_id +0x30). webOS25-shipped:
 *  - the kernel enum audio_decoder_codec has no DTS / DTS_HD (OPUS = 17,
 *    AAC_ELD = 19, RA8 = 20; see the shipped _codec_name table): the DTS
 *    case of adecoder_codec_param_user (bitstream-passthrough / enc-mode via
 *    aud_encoder_get_dtsx_enc_mode) is not compiled in, and
 *    adecoder_get_dts_supported / adecoder_get_dts_bitstream_passthrough do
 *    not exist (no symbol, no ioctl in driver_adec_ioctl).
 *  - the AC3/EAC3, AC4, AAC, MPEG-H and OPUS user parameter structs carry an
 *    extra trailing bool ("Downmix 2ch (Multiview)") which sends
 *    "downmix-out-ch=1" to the decoder.
 *  - new adecoder_set_downmix / adecoder_set_sound_system (not in the
 *    published file; reconstructed from the shipped code, no tag since they
 *    are not in the symbol table).
 *  - adecoder_get_uid/get_decoded_info.. log lines are +26..+30 vs the
 *    published file, i.e. ~25 lines were added in adecoder_codec_param_user.
 */


#include <linux/module.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <asm/io.h>

#include "os_util.h"
#include "base_device.h"
#include "debug_util.h"

#include "log.h"

#include "aud_kapi.h"
#include "aapi.h"
#include "block.h"
#include "block_manager.h"
#include "decoder.h"
#include "renderer.h"
#include "clock.h"
#include "mixer.h"
#include "output.h"
#include "encoder.h"
#include "hal/hal.h"

#include "notifier.h"
#include "module_list.h"

logm_define (amedia, log_level_noti);
#define trace(fmt,args...)  logm_trace(amedia,fmt,##args)
#define debug(fmt,args...)  logm_debug(amedia,fmt,##args)
#define info(fmt,args...)   logm_info(amedia,fmt,##args)
#define noti(fmt,args...)   logm_noti(amedia,fmt,##args)
#define warn(fmt,args...)   logm_warning(amedia,fmt,##args)
#define error(fmt,args...)  logm_error(amedia,fmt,##args)

#define ADEC_MAX 2
#define AREN_MAX 10

#define DEFAULT_PRL_MIN (-31)
#define DEFAULT_PRL_MAX (0)
#define INVALID_DEFAULT_PRL (0x7FFFFFFF)
#define MOD_REF_PORT(__X__)             (((__X__)&0xff) | 0x200)

#if defined(CHIP_NAME_m23) || defined(CHIP_NAME_o22) || defined(CHIP_NAME_o24)
#define VDEC_AV_LIPSYNC_BASE (0xCE0008DC)
#else
#error "Please Define Chip Definition"
#endif

typedef struct
{
    struct aud_block_info *dec_info;
    struct mutex lock;

    bool is_open;
    bool is_started;
    enum audio_decoder_codec codec;

    int proc_id;
} ADECODER_INFO_T;


typedef struct
{
    struct aud_block_info *ren_info;
    struct mutex lock;

    bool is_open;

    enum md_clock_type clock_type;

    aren_pcm_info_t pcm_info;

    bool vsp_mode;

    int proc_id;
} ARENDERER_INFO_T;

static ADECODER_INFO_T adecoder_info[ADEC_MAX] = {
    {.lock = __MUTEX_INITIALIZER (adecoder_info[0].lock)},
    {.lock = __MUTEX_INITIALIZER (adecoder_info[1].lock)},
};

static ARENDERER_INFO_T arenderer_info[AREN_MAX] = {
    {.lock = __MUTEX_INITIALIZER (arenderer_info[0].lock)},
    {.lock = __MUTEX_INITIALIZER (arenderer_info[1].lock)},
    {.lock = __MUTEX_INITIALIZER (arenderer_info[2].lock)},
    {.lock = __MUTEX_INITIALIZER (arenderer_info[3].lock)},
    {.lock = __MUTEX_INITIALIZER (arenderer_info[4].lock)},
    {.lock = __MUTEX_INITIALIZER (arenderer_info[5].lock)},
    {.lock = __MUTEX_INITIALIZER (arenderer_info[6].lock)},
    {.lock = __MUTEX_INITIALIZER (arenderer_info[7].lock)},
    {.lock = __MUTEX_INITIALIZER (arenderer_info[8].lock)},
    {.lock = __MUTEX_INITIALIZER (arenderer_info[9].lock)},
};

#define CODEC_NAME(id)  {AUDIO_DECODER_CODEC_##id, #id}

#define ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK(_index) \
    if (_index < 0 || _index >= ADEC_MAX) \
    { \
        error ("invalid audio decoder index, index %d .\n", _index); \
        return -1; \
    } \
    dec = &adecoder_info[_index]; \
    mutex_lock (&dec->lock); \
    if (!dec->is_open) \
    { \
        error ("can not find audio decoder .\n"); \
	    mutex_unlock (&dec->lock); \
        return -1; \
    }

#define ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK(_index) \
    if (_index < 0 || _index >= AREN_MAX) \
    { \
        error ("invalid audio renderer index, index %d .\n", _index); \
        return -1; \
    } \
    ren = &arenderer_info[_index]; \
    mutex_lock (&ren->lock); \
    if (!ren->is_open) \
    { \
        error ("can not find audio renderer .\n"); \
	    mutex_unlock (&ren->lock); \
        return -1; \
    }

struct str_table
{
    int id;
    const char *name;
};

static const struct str_table _codec_name[] = {
    CODEC_NAME (UNKNOWN),
    CODEC_NAME (PCM),
    CODEC_NAME (AC3),
    CODEC_NAME (EAC3),
    CODEC_NAME (AC4),
    CODEC_NAME (MAT),
    CODEC_NAME (TRUE_HD),
    CODEC_NAME (AAC),
    CODEC_NAME (MP3),
    CODEC_NAME (MPEG_H),
    CODEC_NAME (DRA),
    CODEC_NAME (WMA_PRO),
    CODEC_NAME (VORBIS),
    CODEC_NAME (AMR_WB),
    CODEC_NAME (AMR_NB),
    CODEC_NAME (ADPCM),
    CODEC_NAME (FLAC),
    CODEC_NAME (OPUS),
    CODEC_NAME (AAC_ELD),
    CODEC_NAME (RA8),
    {}
};

static const char *_get_codec_name (enum audio_decoder_codec codec)
{
    int i;

    for (i = 0; i < sizeof (_codec_name) / sizeof (_codec_name[0]); i++)
    {
        if (_codec_name[i].id == codec)
            return _codec_name[i].name;
    }

    return _codec_name[0].name;
}

/* @0xffffffc010cb9110 */
int adecoder_open (int index)
{
    ADECODER_INFO_T *dec = NULL;
    int ret = -1;

    if (index < 0 || index >= ADEC_MAX)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }

    dec = &adecoder_info[index];

    mutex_lock (&dec->lock);

    if (dec->is_open || (bm_get_block_info_by_index ("decoder", index) != NULL))
    {
        error ("can not open audio decoder. already opened index %d", index);
        goto exit;
    }

    dec->dec_info = bm_create_block ("decoder", index);
    dec->is_open = TRUE;
    dec->proc_id = -1;

    info ("adecoder_open index %d .\n", index);

    ret = 0;
  exit:
    mutex_unlock (&dec->lock);

    return ret;
}

/* @0xffffffc010cb92c4 */
int adecoder_close (int index)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    if (dec->is_started)
    {
        decoder_unregister_event (dec->dec_info, AUDIO_DECODER_EVENT_INFO);

        if (dec->codec == AUDIO_DECODER_CODEC_MPEG_H)
            decoder_unregister_event (dec->dec_info, AUDIO_DECODER_EVENT_MPEGH_UI);
    }

    bm_destroy_block (dec->dec_info);

    dec->is_open = FALSE;
    dec->is_started = FALSE;
    dec->codec = AUDIO_DECODER_CODEC_UNKNOWN;
    dec->dec_info = NULL;

    mutex_unlock (&dec->lock);

    info ("adecoder_close index %d .\n", index);

    return 0;
}

/* @0xffffffc010cb94b0 */
int adecoder_start (int index)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_register_event (dec->dec_info, AUDIO_DECODER_EVENT_INFO, decoder_decoded_info_cb, true);

    decoder_connect (dec->dec_info);

    if (dec->codec == AUDIO_DECODER_CODEC_MPEG_H)
    {
        decoder_register_event (dec->dec_info, AUDIO_DECODER_EVENT_MPEGH_UI,
            decoder_mpegh_ui_param_info_cb, true);
    }

    dec->is_started = TRUE;

    mutex_unlock (&dec->lock);

    info ("adecoder_start index %d .\n", index);

    return 0;
}

/* @0xffffffc010cb96a0 */
int adecoder_stop (int index)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_unregister_event (dec->dec_info, AUDIO_DECODER_EVENT_INFO);

    if (dec->codec == AUDIO_DECODER_CODEC_MPEG_H)
    {
        decoder_unregister_event (dec->dec_info, AUDIO_DECODER_EVENT_MPEGH_UI);
    }

    dec->is_started = FALSE;

    mutex_unlock (&dec->lock);

    info ("adecoder_stop index %d .\n", index);

    return 0;
}


/* @0xffffffc010cb9870 */
int adecoder_codec (int index, enum audio_decoder_codec codec)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    dec->codec = codec;

    decoder_set_codec (dec->dec_info, codec);

    mutex_unlock (&dec->lock);

    info ("adecoder_codec index %d codec %d (%s) .\n", index, codec, _get_codec_name (codec));

    return 0;
}

/* webOS25-shipped: user parameter layouts as copied by the shipped
 * adecoder_codec_param_user: the published structs plus one trailing bool
 * "Downmix 2ch (Multiview)" (copy_from_user size limits 0x1c / 0x28 / 0x24 /
 * 0xc; field offsets checked against the stack loads). */
struct adec_ac3_param_ws25
{
    unsigned int sampling_rate;
    unsigned int channels;
    unsigned int drc_mode;
    unsigned int downmix_mode;
    bool audio_description;
    bool atmos_decoding;
    bool legacy_ddp_out;
    unsigned int play_speed;
    bool downmix_2ch;           /* +0x18 */
};

struct adec_ac4_param_ws25
{
    unsigned int sampling_rate;
    unsigned int channels;
    unsigned int drc_mode;
    unsigned int downmix_mode;
    unsigned int audio_description;
    bool atmos_decoding;
    unsigned int de_gain;
    bool legacy_ddp_out;
    unsigned int play_speed;
    bool downmix_2ch;           /* +0x24 */
};

struct adec_aac_param_ws25
{
    unsigned int sampling_rate;
    unsigned int channels;
    unsigned int drc_mode;
    unsigned int downmix_mode;
    bool audio_description;
    unsigned int iec_format;
    bool transcoding;
    unsigned int play_speed;
    bool downmix_2ch;           /* +0x20 */
};

/* MPEG-H and OPUS */
struct adec_2ch_param_ws25
{
    unsigned int sampling_rate;
    unsigned int channels;
    bool downmix_2ch;           /* +0x8 */
};

static enum pcmren_downmix_mode adecoder_get_pcmren_downmix_mode (unsigned int downmix_mode)
{
    if (downmix_mode == 0)
        return PCMREN_DOWNMIX_LORO_MODE;
    else
        return PCMREN_DOWNMIX_LTRT_MODE;

    return PCMREN_DOWNMIX_LTRT_MODE;
}

/* @0xffffffc010cb9a60 */
int adecoder_codec_param_user (int index, struct adec_codec_param *codec_param)
{
    ADECODER_INFO_T *dec = NULL;
    struct aud_block_info *mixer = NULL;
    int main_audio_index = -1;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    info ("codec: %d (%s)\n", dec->codec, _get_codec_name (dec->codec));

    mixer = bm_get_block_info_by_index ("mixer", 0);
    if (mixer)
        main_audio_index = mixer_get_main_audio_index (mixer);

    switch (dec->codec)
    {
        case AUDIO_DECODER_CODEC_PCM:
        case AUDIO_DECODER_CODEC_ADPCM:
        {
            struct adec_pcm_param param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);
            info ("bits per sample: %d\n", param.bits_per_sample);
            info ("byte align: %d\n", param.byte_align);
            info ("sign: %d\n", param.sign);

            decoder_set_param (dec->dec_info,
                "rate=%d channels=%d bits-per-sample=%d endian=%d sign=%d",
                param.sampling_rate, param.channels, param.bits_per_sample, param.byte_align,
                param.sign);

            break;
        }

        case AUDIO_DECODER_CODEC_AC3:
        case AUDIO_DECODER_CODEC_EAC3:
        {
            struct adec_ac3_param_ws25 param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);
            info ("drc mode: %d\n", param.drc_mode);
            info ("downmix mode: %d\n", param.downmix_mode);
            info ("audio description: %d\n", param.audio_description);
            info ("atmos decoding: %d\n", param.atmos_decoding);
            info ("legacy ddp out: %d\n", param.legacy_ddp_out);
            info ("play speed: %d\n", param.play_speed);
            info ("Downmix 2ch (Multiview): %d\n", param.downmix_2ch);

            decoder_set_param (dec->dec_info, "dec-id=%d", index);
            decoder_set_param (dec->dec_info,
                "drc-mode=%d downmix-mode=%d audio-description=%d atmos-decoding=%d",
                param.drc_mode, param.downmix_mode, param.audio_description, param.atmos_decoding);
            /* webOS25-shipped */
            if (param.downmix_2ch)
                decoder_set_param (dec->dec_info, "downmix-out-ch=%d", 1);

            if (mixer && (index == main_audio_index))
            {
                mixer_set_pcmren_drc_mode (mixer, param.drc_mode);
                mixer_set_pcmren_downmix_mode (mixer,
                    adecoder_get_pcmren_downmix_mode (param.downmix_mode));

                // 0: DD/DD+, 1: AAC, 2: AC4, 3: MAT, 4: ETC (EXTERNAL_PCM)
                mixer_set_pcmren_metadata_type (mixer, 0);
            }

            break;
        }

        case AUDIO_DECODER_CODEC_AC4:
        {
            struct adec_ac4_param_ws25 param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);
            info ("drc mode: %d\n", param.drc_mode);
            info ("downmix mode: %d\n", param.downmix_mode);
            info ("audio description: %d\n", param.audio_description);
            info ("atmos decoding: %d\n", param.atmos_decoding);
            info ("de gain: %d\n", param.de_gain);
            info ("legacy ddp out: %d\n", param.legacy_ddp_out);
            info ("play speed: %d\n", param.play_speed);
            info ("Downmix 2ch (Multiview): %d\n", param.downmix_2ch);

            decoder_set_param (dec->dec_info,
                "de-gain=%d legacy-ddp-out=%d atmos-decoding=%d",
                param.de_gain, param.legacy_ddp_out, param.atmos_decoding);
            decoder_set_param (dec->dec_info, "main-decoder=1");
            /* webOS25-shipped */
            if (param.downmix_2ch)
                decoder_set_param (dec->dec_info, "downmix-out-ch=%d", 1);

            if (mixer && (index == main_audio_index))
            {
                mixer_set_pcmren_drc_mode (mixer, param.drc_mode);
                mixer_set_pcmren_downmix_mode (mixer,
                    adecoder_get_pcmren_downmix_mode (param.downmix_mode));

                // 0: DD/DD+, 1: AAC, 2: AC4, 3: MAT, 4: ETC (EXTERNAL_PCM)
                mixer_set_pcmren_metadata_type (mixer, 2);
            }

            break;
        }

        case AUDIO_DECODER_CODEC_AAC:
        {
            struct adec_aac_param_ws25 param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);
            info ("drc mode: %d\n", param.drc_mode);
            info ("downmix mode: %d\n", param.downmix_mode);
            info ("audio description: %d\n", param.audio_description);
            info ("iec format: %d\n", param.iec_format);
            info ("transcoding: %d\n", param.transcoding);
            info ("play speed: %d\n", param.play_speed);
            info ("Downmix 2ch (Multiview): %d\n", param.downmix_2ch);

#if (MS12_VER >= 28)
            decoder_set_param (dec->dec_info, "dec-id=%d", index);
#endif
            decoder_set_param (dec->dec_info,
                "drc-mode=%d downmix-mode=%d audio-description=%d iec-format=%d transcoding=%d",
                param.drc_mode, param.downmix_mode, param.audio_description,
                param.iec_format, param.transcoding, param.play_speed);
            /* webOS25-shipped */
            if (param.downmix_2ch)
                decoder_set_param (dec->dec_info, "downmix-out-ch=%d", 1);

            if (mixer && (index == main_audio_index))
            {
                mixer_set_pcmren_drc_mode (mixer, param.drc_mode);
                mixer_set_pcmren_downmix_mode (mixer,
                    adecoder_get_pcmren_downmix_mode (param.downmix_mode));

                // 0: DD/DD+, 1: AAC, 2: AC4, 3: MAT, 4: ETC (EXTERNAL_PCM)
                mixer_set_pcmren_metadata_type (mixer, 1);
            }

            break;
        }

        case AUDIO_DECODER_CODEC_MP3:
        {
            struct adec_mp3_param param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);
            info ("drc mode: %d\n", param.drc_mode);
            info ("audio description: %d\n", param.audio_description);

            decoder_set_param (dec->dec_info,
                "drc-mode=%d audio-description=%d", param.drc_mode, param.audio_description);

            break;
        }

        case AUDIO_DECODER_CODEC_MPEG_H:
        {
            struct adec_2ch_param_ws25 param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);
            info ("Downmix 2ch (Multiview): %d\n", param.downmix_2ch);

            /* webOS25-shipped */
            if (param.downmix_2ch)
                decoder_set_param (dec->dec_info, "downmix-out-ch=%d", 1);

            break;
        }

        case AUDIO_DECODER_CODEC_DRA:
        {
            struct adec_dra_param param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);

            break;
        }

        case AUDIO_DECODER_CODEC_WMA_PRO:
        {
            struct adec_wma_param param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);

            info ("bits per sample: %d\n", param.bits_per_sample);
            info ("wma format tag: %d\n", param.wma_format_tag);
            info ("block align: %d\n", param.block_align);
            info ("avg bps: %d\n", param.avg_bps);
            info ("channel mask: %d\n", param.channel_mask);
            info ("encode opt: %d\n", param.encode_opt);
            info ("adv encode opt: %d\n", param.adv_encode_opt);
            info ("adv encode opt2: %d\n", param.adv_encode_opt2);

            decoder_set_param (dec->dec_info,
                "sampling-rate=%d channels=%d bits-per-sample=%d wma-format-tag=%d block-align=%d avg-bps=%d channel-mask=%d encode-opt=%d adv-encode-opt=%d adv-encode-opt2=%d",
                param.sampling_rate, param.channels, param.bits_per_sample, param.wma_format_tag,
                param.block_align, param.avg_bps, param.channel_mask, param.encode_opt,
                param.adv_encode_opt, param.adv_encode_opt2);

            break;
        }

        case AUDIO_DECODER_CODEC_VORBIS:
        {
            struct adec_vorbis_param param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);

            break;
        }

        case AUDIO_DECODER_CODEC_AMR_WB:
        case AUDIO_DECODER_CODEC_AMR_NB:
        {
            struct adec_amr_param param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);

            break;
        }

        case AUDIO_DECODER_CODEC_FLAC:
        {
            struct adec_flac_param param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);

            decoder_set_param (dec->dec_info, "channels=%d", param.channels);

            break;
        }

        case AUDIO_DECODER_CODEC_OPUS:
        {
            struct adec_2ch_param_ws25 param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);
            info ("Downmix 2ch (Multiview): %d\n", param.downmix_2ch);

            decoder_set_param (dec->dec_info, "sampling-rate=%d channels=%d", param.sampling_rate,
                param.channels);
            /* webOS25-shipped */
            if (param.downmix_2ch)
                decoder_set_param (dec->dec_info, "downmix-out-ch=%d", 1);

            break;
        }

        /* webOS25-shipped: no AUDIO_DECODER_CODEC_DTS case (the published one
         * copied adec_dtsx_param, set "bitstream-passthrough=%d" and an
         * "enc-mode=%d" read from the ARC/OPTIC encoder). None of its strings
         * exist in the image; a DTS codec id ends in "unknown codec". */

        case AUDIO_DECODER_CODEC_AAC_ELD:
        {
            struct adec_aac_eld_param param = { };

            if (copy_from_user (&param, codec_param->param, codec_param->size))
            {
                error ("copy_from_user() failed\n");
                goto failed;
            }

            info ("sampling rate: %d\n", param.sampling_rate);
            info ("channels: %d\n", param.channels);

            break;
        }

        default:
        {
            error ("unknown codec. codec: %d\n", dec->codec);
            goto failed;
        }
    }
    mutex_unlock (&dec->lock);

    return 0;

  failed:
    mutex_unlock (&dec->lock);

    return -1;
}

/* @0xffffffc010cbb7b0 */
int adecoder_get_uid (int index, unsigned int *uid)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    *uid = bm_get_block_head (dec->dec_info);
    if (!*uid)
    {
        error ("error to get uid from decoder\n");
        mutex_unlock (&dec->lock);
        return -1;
    }

    mutex_unlock (&dec->lock);

    return 0;
}

//pioneer debug function only
/* @0xffffffc010cbb940 */
int adecoder_get_decoded_info (int index, struct decoded_info *info)
{
    ADECODER_INFO_T *dec = NULL;

    if (index < 0 || index >= ADEC_MAX)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }

    dec = &adecoder_info[index];

    if (!info)
    {
        error ("decoded_info parameter is null\n");
        return -1;
    }

    mutex_lock (&dec->lock);
    if (dec->dec_info)
    {
        decoder_get_decoded_info (dec->dec_info, info);
    }
    else
    {
        struct aud_block_info *dec = NULL;
        dec = bm_get_block_info_by_index ("decoder", index);
        if (dec)
        {
            decoder_get_decoded_info (dec, info);
        }
        else
        {
            debug ("can not find decoder block, index: %d\n", index);
        }
    }
    mutex_unlock (&dec->lock);

    debug ("codec_type = %d, sampling_rate = %d, channel = %d, bit_per_sample = %d acmod = %d \n",
        info->codec, info->sampling_rate, info->source_channels, info->bit_per_sample, info->acmod);

    return 0;
}

/* @0xffffffc010cbbb34 */
int adecoder_set_input (int index, enum audio_decoder_input input)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_input (dec->dec_info, input);

    mutex_unlock (&dec->lock);

    return 0;
}

//pioneer debug function only
/* @0xffffffc010cbbc80 */
int adecoder_get_input (int index, unsigned int *input)
{
    ADECODER_INFO_T *dec = NULL;

    if (index < 0 || index >= ADEC_MAX)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }

    if (!input)
    {
        error ("input is null .\n");
        return -1;
    }

    dec = &adecoder_info[index];

    mutex_lock (&dec->lock);
    if (dec->dec_info)
    {
        *input = decoder_get_input (dec->dec_info);
    }
    else
    {
        struct aud_block_info *dec = NULL;
        dec = bm_get_block_info_by_index ("decoder", index);
        if (dec)
        {
            *input = decoder_get_input (dec);
        }
        else
        {
            debug ("can not find decoder block, index: %d\n", index);
        }
    }
    mutex_unlock (&dec->lock);

    debug ("decoder#%d input: %d\n", index, *input);

    return 0;
}

/* @0xffffffc010cbbe60 */
int adecoder_set_mpegh_ui_param (int index, struct adec_mpegh_ui_param *ui_param)
{
    ADECODER_INFO_T *dec = NULL;

    if (!ui_param)
    {
        error ("param is null .\n");
        return -1;
    }

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_mpegh_ui_param (dec->dec_info, ui_param);

    mutex_unlock (&dec->lock);

    return 0;
}

/* @0xffffffc010cbbff0 */
int adecoder_get_mpegh_ui_param (int index, struct adec_mpegh_ui_param *ui_param)
{
    ADECODER_INFO_T *dec = NULL;

    if (!ui_param)
    {
        error ("param is null .\n");
        return -1;
    }

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_get_mpegh_ui_param (dec->dec_info, ui_param);

    mutex_unlock (&dec->lock);

    return 0;
}

/* @0xffffffc010cbc180 */
int adecoder_set_output_mode (int index, unsigned int output_mode)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_param (dec->dec_info, "output-mode=%d", output_mode);

    mutex_unlock (&dec->lock);

    return 0;
}

/* @0xffffffc010cbc2d0 */
int adecoder_set_basetime (int index, unsigned long long basetime)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_param (dec->dec_info, "basetime=%llu", basetime);

    mutex_unlock (&dec->lock);

    return 0;
}

/* @0xffffffc010cbc420 */
int adecoder_set_presentation_group_index (int index, unsigned int pres_group_index)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_param (dec->dec_info, "pres-group-index=%d", pres_group_index);

    mutex_unlock (&dec->lock);

    return 0;
}

/* @0xffffffc010cbc570 */
int adecoder_set_dialog_enhancement_gain (int index, unsigned int gain)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_param (dec->dec_info, "de-gain=%d", gain);

    mutex_unlock (&dec->lock);

    return 0;
}

/* @0xffffffc010cbc6c0 */
int adecoder_get_dialog_enhancement_gain (int index, unsigned int *gain)
{
    ADECODER_INFO_T *dec = NULL;

    if (index < 0 || index >= ADEC_MAX)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }

    dec = &adecoder_info[index];

    if (!dec->dec_info)
    {
        error ("decoder is not opened\n");
        return -1;
    }

    decoder_get_param (dec->dec_info, "de-gain", gain);

    info ("dialog enhancement gain: %d\n", *gain);

    return 0;
}

/* @0xffffffc010cbc814 */
int adecoder_get_dialog_enhancement_limit (int index, unsigned int *limit)
{
    ADECODER_INFO_T *dec = NULL;

    if (index < 0 || index >= ADEC_MAX)
    {
        error ("invalid audio decoder index, index %d .\n", index);
        return -1;
    }

    dec = &adecoder_info[index];

    if (!dec->dec_info)
    {
        error ("decoder is not opened\n");
        return -1;
    }

    decoder_get_param (dec->dec_info, "de-limit", limit);

#if (MS12_VER == 24)
    //http://source.lge.com/dsw/#/c/DTV/media/gst-plugins-bsp/+/155712/
    *limit = *limit * 3;
#endif

    info ("dialog enhancement limit: %d\n", *limit);

    return 0;
}

/* @0xffffffc010cbc974 */
int adecoder_set_default_prl (int index, unsigned int default_prl)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_param (dec->dec_info, "default-prl=%d", default_prl);

    mutex_unlock (&dec->lock);

    return 0;
}

/* @0xffffffc010cbcac4 */
int adecoder_set_gap_info (int index, struct gap_info *gap_info)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_param (dec->dec_info, "start-pts=%d end-pts=%d", gap_info->start_pts,
        gap_info->end_pts);

    mutex_unlock (&dec->lock);

    return 0;
}

/* webOS25-shipped: adecoder_get_dts_supported() and
 * adecoder_get_dts_bitstream_passthrough() are not built (no symbols, no
 * ioctl). */

/* @0xffffffc010cbcc14 */
int adecoder_set_dolby_drc_mode (int index, unsigned int drc_mode)
{
    ADECODER_INFO_T *dec = NULL;
    struct aud_block_info *mixer = NULL;
    int main_audio_index = -1;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_param (dec->dec_info, "drc-mode=%d", drc_mode);

    decoder_set_dolby_drc_mode (dec->dec_info, drc_mode);

    mixer = bm_get_block_info_by_index ("mixer", 0);

    if (mixer)
    {
        main_audio_index = mixer_get_main_audio_index (mixer);
        if (index == main_audio_index)
        {
            if (dec->codec == AUDIO_DECODER_CODEC_MP3)
                drc_mode = AUDIO_DECODER_DRC_LINE;

            mixer_set_pcmren_drc_mode (mixer, drc_mode);
        }
    }

    mutex_unlock (&dec->lock);

    return 0;
}

/* @0xffffffc010cbcdc0 */
int adecoder_set_aac_format_type (int index, unsigned int format_type)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_param (dec->dec_info, "format-type=%d", format_type);

    mutex_unlock (&dec->lock);

    info ("aac format type %d .\n", format_type);

    return 0;
}

/* @0xffffffc010cbcf60 */
int adecoder_set_proc_id (int index, unsigned int proc_id)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    dec->proc_id = proc_id;
    mutex_unlock (&dec->lock);

    info ("set processor id as %d(0x%0x)\n", proc_id, proc_id);

    return 0;
}

/* @0xffffffc010cbd0f0 */
int adecoder_set_normal_closed (int index)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    mutex_unlock (&dec->lock);

    info ("normal closed.\n");

    return 0;
}

/* webOS25-shipped: new, not in the published file (@0xffffffc010cbd270,
 * not in the symbol table). Called by driver_adec_ioctl 'D' 30. */
int adecoder_set_downmix (int index, bool enable)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_param (dec->dec_info, "downmix-out-ch=%d", enable);

    mutex_unlock (&dec->lock);

    info ("downmix : %s.\n", enable ? "enable" : "disable");

    return 0;
}

/* webOS25-shipped: new (@0xffffffc010cbd424, not in the symbol table).
 * Called by driver_adec_ioctl 'D' 31. */
int adecoder_set_sound_system (int index, unsigned int sound_system)
{
    ADECODER_INFO_T *dec = NULL;

    ADECODER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    decoder_set_param (dec->dec_info, "sound_system=%d", sound_system);

    mutex_unlock (&dec->lock);

    info ("sound_system = %d\n", sound_system);

    return 0;
}

/* @0xffffffc010cbd5c4 */
int aren_open (int index)
{
    ARENDERER_INFO_T *ren = NULL;

    if (index < 0 || index >= AREN_MAX)
    {
        error ("invalid audio renderer index, index %d .\n", index);
        return -1;
    }

    ren = &arenderer_info[index];

    mutex_lock (&ren->lock);

    if (ren->is_open)
    {
        error ("can not open audio renderer. already opened index %d", index);
        mutex_unlock (&ren->lock);
        return -1;
    }

    ren->ren_info = bm_create_block ("renderer", index);

    ren->is_open = TRUE;
    ren->proc_id = -1;

    mutex_unlock (&ren->lock);

    info ("aren_open index %d .\n", index);

    return 0;
}

/* @0xffffffc010cbd790 */
int aren_close (int index)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    bm_disconnect_block (ren->ren_info, 0);

    bm_destroy_block (ren->ren_info);

    ren->is_open = FALSE;
    ren->clock_type = MD_CLK_TYPE_UNKNOWN;

    memset (&ren->pcm_info, 0x0, sizeof (aren_pcm_info_t));

    ren->ren_info = NULL;

    mutex_unlock (&ren->lock);

    info ("aren_close index %d .\n", index);

    return 0;
}

/* @0xffffffc010cbd960 */
int aren_add_queue (int index)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_add_queue (ren->ren_info, true);

    mutex_unlock (&ren->lock);

    return 0;
}

/* @0xffffffc010cbdab0 */
int aren_get_uid (int index, unsigned int *uid, bool is_bypass)
{
    ARENDERER_INFO_T *ren = NULL;
    aren_pcm_info_t *info;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    info = &ren->pcm_info;

    if (is_bypass)
    {
        /* shipped @+0x74..+0x80: ldr [ren+0x48] (pcm_bypass) / [ren+0x4c]
         * (pcm_multi_ch); both 0 -> ES head (queue -> lipsync on DSP2). No codec
         * or sample-rate check: the ES/PCM choice is made only from the last
         * AREN_SET_PCM_INFO; if SET_PCM_INFO comes after GET_BYPASS_UID the old
         * (zeroed by aren_close) values are used. */
        if (info->pcm_bypass || info->pcm_multi_ch)
        {
            *uid = renderer_get_uid_pcm_bypass_head (ren->ren_info);
            info ("pcm bypass uid = 0x%08X\n", *uid);
        }
        else
        {
            *uid = renderer_get_uid_es_bypass_head (ren->ren_info);
            info ("es bypass uid = 0x%08X\n", *uid);
        }
    }
    else
    {
        *uid = bm_get_block_head (ren->ren_info);
        info ("uid = 0x%08X\n", *uid);
    }

    if (!*uid)
    {
        error ("error to get uid from %s\n", (is_bypass) ? "bypass_renderer" : "renderer");
        mutex_unlock (&ren->lock);
        return -1;
    }

    mutex_unlock (&ren->lock);

    return 0;
}

/* @0xffffffc010cbdda0 */
int aren_get_tail_uid (int index, unsigned int *uid, bool is_bypass)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    if (is_bypass)
        *uid = renderer_get_uid_bypass_tail (ren->ren_info);
    else
        *uid = bm_get_block_tail (ren->ren_info);

    if (!*uid)
    {
        error ("error to get uid from renderer\n");
        mutex_unlock (&ren->lock);
        return -1;
    }

    mutex_unlock (&ren->lock);

    return 0;
}

/* @0xffffffc010cbdf50 */
int aren_set_clock_type (int index, enum md_clock_type clock_type)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_set_param (ren->ren_info, "clockType=%d", clock_type);

    if (clock_type == MD_CLK_TYPE_GSTC)
    {
        renderer_set_param (ren->ren_info, "idle_time=0");
    }

    ren->clock_type = clock_type;

    mutex_unlock (&ren->lock);

    return 0;
}


/* @0xffffffc010cbe0e4 */
int aren_enable_lipsync (int index, bool on)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_set_param (ren->ren_info, "LipsyncOnOff=%d", on);

    mutex_unlock (&ren->lock);

    info ("aren_enable_lipsync index %d on_off %d .\n", index, on);

    return 0;
}

/* @0xffffffc010cbe290 */
int aren_set_basetime (int index, unsigned int clock_base, unsigned int stream_base)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_set_param (ren->ren_info, "BaseCT=%d BaseST=%d", clock_base, stream_base);

    mutex_unlock (&ren->lock);

    info ("aren_set_basetime index %d 0x%x 0x%x\n", index, clock_base, stream_base);

    return 0;
}


/* @0xffffffc010cbe460 */
int aren_set_lipsync_param (int index, unsigned int lower_bound, unsigned int upper_bound,
    unsigned int offset, unsigned int freerun_lbound, unsigned int freerun_ubound)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_set_param (ren->ren_info, "lowerbound=%d upperbound=%d", lower_bound, upper_bound);
    renderer_set_param (ren->ren_info, "offset=%d", offset);

    mutex_unlock (&ren->lock);

    info ("aren_set_lipsync_param index %d done\n", index);

    return 0;
}

/* @0xffffffc010cbe640 */
int aren_set_speed (int index, unsigned int speed)
{
    ARENDERER_INFO_T *ren = NULL;
    unsigned int rate_in = 1000;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    // trick speed adjust ex) 2x: InSample = 2000
    if (speed > 2000 || speed <= 0)
    {
        rate_in = 1000;
    }
    else
    {
        rate_in = speed;
    }

    if (!ren->vsp_mode)
    {
        notifier_set_play_speed (index, rate_in);
    }

    renderer_set_param (ren->ren_info, "rate-in=%d", rate_in);
    renderer_set_param (ren->ren_info, "speed-in=%d", rate_in);

    mutex_unlock (&ren->lock);

    info ("aren_set_speed index %d done\n", index);

    return 0;
}

/* @0xffffffc010cbe840 */
int aren_set_mute (int index, unsigned int mute)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_set_param (ren->ren_info, "mute-input=%d", mute);

    mutex_unlock (&ren->lock);

    info ("%s done. mute = %d \n", __func__, mute);

    return 0;
}

/* @0xffffffc010cbe9f0 */
int aren_set_ease_volume (int index, aren_ease_volume_t easevolume)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_set_param (ren->ren_info, "target-gain=%d", easevolume.volume);
    renderer_set_param (ren->ren_info, "fade-type=%d", easevolume.type);
    renderer_set_param (ren->ren_info, "fadetime-ms=%d", easevolume.duration);

    mutex_unlock (&ren->lock);

    info ("volume:%d, duration:%d, type:%d \n", easevolume.volume, easevolume.duration,
        easevolume.type);

    return 0;
}

/* @0xffffffc010cbebc4 */
int aren_get_ease_volume (int index, unsigned int *volume)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_get_param (ren->ren_info, "final-gain", volume);

    mutex_unlock (&ren->lock);

    info ("done. volume = %d \n", *volume);

    return 0;
}


/* @0xffffffc010cbed70 */
int aren_set_gap_info (int index, aren_gap_info_t gap_info)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_set_gap_info (ren->ren_info, gap_info.start_pts, gap_info.end_pts);

    mutex_unlock (&ren->lock);

    info ("done. start_pts:%d, end_pts:%d \n", gap_info.start_pts, gap_info.end_pts);

    return 0;
}

/* @0xffffffc010cbef30 */
int aren_get_rendered_status (int index, aren_rendered_status_t * status)
{
    ARENDERER_INFO_T *ren = NULL;

    if (index < 0 || index >= AREN_MAX)
    {
        error ("invalid audio renderer index, index %d .\n", index);
        return -1;
    }

    ren = &arenderer_info[index];

    return 0;
}

/* @0xffffffc010cbefc0 */
int aren_get_rts_queue_info (int index, aren_rts_queue_info_t * info)
{
    ARENDERER_INFO_T *ren = NULL;
    unsigned int queued_byte = 0;
    unsigned int max_length = 0;
    unsigned int stride_shift = 0;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_get_param (ren->ren_info, "queued-byte", &queued_byte);
    renderer_get_param (ren->ren_info, "max-length", &max_length);
    renderer_get_param (ren->ren_info, "stride-shift", &stride_shift);

    info->queue_max = max_length << stride_shift;
    info->queued_byte = queued_byte;

    mutex_unlock (&ren->lock);

    trace ("queued-byte :%d\n", queued_byte);
    trace ("max-length :%d\n", max_length);
    trace ("stride-shift :%d\n", stride_shift);
    trace ("queue_max = %d\n", info->queue_max);

    return 0;
}

/* @0xffffffc010cbf280 */
int aren_add_rts (int index)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_add_rts (ren->ren_info);

    mutex_unlock (&ren->lock);

    info ("renderer_add_rts done\n");
    return 0;
}

/* @0xffffffc010cbf414 */
int aren_set_rts_param (int index, aren_rts_param_t rts_param)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_set_rts_port_multiplier (ren->ren_info, MOD_REF_PORT (0), 384000,
        (unsigned int) (ren->pcm_info.sampling_rate * ren->pcm_info.channels *
            (ren->pcm_info.bits_per_sample >> 3)));
    renderer_set_rts_threshold (ren->ren_info, rts_param.upper_threshold, rts_param.lower_threshold,
        rts_param.recover_upper, rts_param.recover_lower);
    renderer_set_rts_out_freq (ren->ren_info, rts_param.fast_freq, rts_param.normal_freq,
        rts_param.slow_freq);
    renderer_set_rts_skipping (ren->ren_info, rts_param.recover_skipping,
        rts_param.skipping_threshold);
    renderer_set_rts_opmode (ren->ren_info, rts_param.opmode);
    renderer_set_param (ren->ren_info, "buffering-ms=%d", rts_param.buffering_level);
    renderer_set_rts_start (ren->ren_info);

    mutex_unlock (&ren->lock);

    return 0;
}

/* @0xffffffc010cbf5d4 */
int aren_set_jp4k (int index, bool is_jp4k)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_set_jp4k (ren->ren_info, is_jp4k);

    mutex_unlock (&ren->lock);

    return 0;
}

/* @0xffffffc010cbf724 */
int aren_start (int index)
{
    ARENDERER_INFO_T *ren = NULL;
    bool update = TRUE;
    aren_pcm_info_t *info;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    info = &ren->pcm_info;

    if (ren->clock_type == MD_CLK_TYPE_GSTC)
        aud_clock_enable_input (index, CLOCK_SOURCE_GSTC0, 0);
    else if (ren->clock_type == MD_CLK_TYPE_GSTC_1)
        aud_clock_enable_input (index, CLOCK_SOURCE_GSTC1, 0);
    else
        update = FALSE;

    if (update == TRUE)
    {
        if (ren->pcm_info.clock_x4)
            aud_clock_update_input (index, ren->pcm_info.channels, ren->pcm_info.sampling_rate * 4);
        else
            aud_clock_update_input (index, ren->pcm_info.channels, ren->pcm_info.sampling_rate);
    }

    renderer_set_param (ren->ren_info, "endian=%d sign=%d", info->byte_align, info->sign);
    /* "block-queue" is routed by renderer_run_param through param_queue, i.e.
     * only to the MAIN pipeline queue (uid_queue, added by AREN_ADD_QUEUE). The
     * ES bypass "queue" is not registered, so it is never (un)blocked here. */
    renderer_set_param (ren->ren_info, "block-queue=%d", 0);

    if (info->pcm_bypass)
    {
        unsigned int channels = (info->channels <= 2) ? 2 : 8;
        renderer_set_pcm_bypass_rts_out_channels (ren->ren_info, channels);
    }

    if (info->pcm_multi_ch)
    {
        unsigned int channels = (info->channels <= 2) ? 2 : 8;
        renderer_set_pcm_bypass_rts_out_channels (ren->ren_info, channels);
        renderer_set_param (ren->ren_info, "out-channels=%d", channels);
    }

    mutex_unlock (&ren->lock);

    return 0;
}

/* @0xffffffc010cbf990 */
int aren_stop (int index)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    aud_clock_disable_input (index);
    renderer_set_param (ren->ren_info, "block-queue=%d", 1);

    mutex_unlock (&ren->lock);

    info ("aren_stop index %d .\n", index);

    return 0;
}

/* @0xffffffc010cbfb40 */
int aren_set_pcm_info (int index, aren_pcm_info_t pcm_info)
{
    ARENDERER_INFO_T *ren = NULL;
    aren_pcm_info_t *info;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    info = &ren->pcm_info;

    info->bits_per_sample = pcm_info.bits_per_sample;
    info->channels = pcm_info.channels;
    info->sampling_rate = pcm_info.sampling_rate;
    info->byte_align = pcm_info.byte_align;
    info->sign = pcm_info.sign;
    info->clock_x4 = pcm_info.clock_x4;
    info->pcm_bypass = pcm_info.pcm_bypass;
    info->pcm_multi_ch = pcm_info.pcm_multi_ch;
    /* shipped: two 16-byte ldp/stp copies of the 32-byte struct into
     * ren+0x30; no validation, no renderer call. Only stored. */

    mutex_unlock (&ren->lock);

    info ("renderer set pcm info. bits per sample: %d, channels: %d, sampling rate: %d, byte align: %d, signed: %d, clock_x4 (eac3): %d, pcm-bypass(google): %d, pcm-multi-ch(netflix): %d\n", info->bits_per_sample, info->channels, info->sampling_rate, info->byte_align, info->sign, info->clock_x4, info->pcm_bypass, info->pcm_multi_ch);

    return 0;
}

/* @0xffffffc010cbfd30 */
int aren_set_bypass_fader (int index, bool game_mode)
{
    struct aud_block_info *mixer = NULL;

    mixer = bm_get_block_info_by_index ("mixer", 0);
    if (mixer)
    {
        info ("Set bypass fader : %d\n", game_mode);
        mixer_set_bypass_fader (mixer, index, game_mode);
    }
    else
    {
        info ("mixer is not connected.\n");
        return -1;
    }

    return 0;
}

/* @0xffffffc010cbfe20 */
int aren_get_queue_info (int index, aren_queue_info_t * info)
{
    ARENDERER_INFO_T *ren = NULL;
    unsigned int queued_length = 0;
    unsigned int max_length = 0;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_get_param (ren->ren_info, "queued-length", &queued_length);
    renderer_get_param (ren->ren_info, "queue-max-length", &max_length);

    info->queued_length = queued_length;
    info->max_length = max_length;

    mutex_unlock (&ren->lock);

    trace ("queued-length :%d\n", queued_length);
    trace ("max-length :%d\n", max_length);

    return 0;
}

/* @0xffffffc010cc0030 */
int aren_get_vdec_current_pts (int index, aren_vdec_current_pts_t * vdec_current_pts)
{
    ARENDERER_INFO_T *ren = NULL;
    volatile unsigned int *reg = NULL;
    unsigned int vdec_pts_reg = 0;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    vdec_pts_reg = (VDEC_AV_LIPSYNC_BASE + (vdec_current_pts->port * 8) + 4);
    reg = (volatile unsigned int *) ioremap (vdec_pts_reg, sizeof (unsigned int));
    if (reg == NULL)
    {
        error ("Failed ioremap\n");
        return -EFAULT;
    }

    vdec_current_pts->pts = readl (reg);
    iounmap (reg);

    mutex_unlock (&ren->lock);

    info ("vdec_port = 0x%08X\n", vdec_current_pts->port);
    info ("pts_reg   = 0x%08X\n", vdec_pts_reg);
    info ("vdec_current_pts->pts = 0x%08X\n", vdec_current_pts->pts);

    return 0;
}

/* @0xffffffc010cc0310 */
int aren_set_queue_max_length (int index, unsigned int queue_max_length)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    renderer_set_param (ren->ren_info, "queue-max-length=%d", queue_max_length);

    mutex_unlock (&ren->lock);

    info ("queue_max_length = 0x%08X(%d)\n", queue_max_length, queue_max_length);
    return 0;
}

/* @0xffffffc010cc04c0 */
int aren_set_proc_id (int index, unsigned int proc_id)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    ren->proc_id = proc_id;
    mutex_unlock (&ren->lock);

    info ("set processor id as %d(0x%0x)\n", proc_id, proc_id);

    return 0;
}

/* @0xffffffc010cc0660 */
int aren_set_normal_closed (int index)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    mutex_unlock (&ren->lock);

    info ("normal closed.\n");

    return 0;
}

/* @0xffffffc010cc07f0 */
int aren_set_vsp_mode (int index)
{
    ARENDERER_INFO_T *ren = NULL;

    ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK (index);

    ren->vsp_mode = true;

    mutex_unlock (&ren->lock);

    info ("set vsp(variable speed playback) mode.\n");

    return 0;
}
