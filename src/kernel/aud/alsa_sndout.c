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
 * Base: lg-opensource/kdriver/core/aud/alsa/alsa_sndout.c (GPL-2.0),
 * complete. Every tagged function was compared with the shipped disassembly
 * (callees, constants, strings, __LINE__). Build configuration seen in the
 * image: SUPPORT_DAFC off (ALSA_SNDOUT_MIX_NUM 8), MS12_VER < 26,
 * INCLUDE_KDRV_HDMI20 on (HDMI21_Rx_Set_ARC called).
 * Log lines drift +41..+192 against the published file (e.g. +163 in the
 * kcontrol handlers), so code was both added and removed.
 *
 * webOS25-shipped differences (all observed):
 *  - built WITHOUT DTS: enum audio_decoder_codec has no DTS/DTS_HD
 *    (OPUS=17, AAC_ELD=19, RA8=20), enum sndout_input_codec has 10 entries
 *    (OTHERS = 9), enum output_codec_t stops at MAT (see output.c);
 *    _set_dtsx_decoder_enc_mode() and every DTS branch of _get_output_format,
 *    _hdmi_convert_input_codec, _is_decoder_bypass, the codec converters and
 *    the latency lookups are gone; aud_hal_dts_supported() returns 0.
 *  - sndout_earc_output_type / sndout_wowcast_output_type_put reject
 *    codec_type > 9 (SNDOUT_CODEC_DTSHD_MA, SNDOUT_CODEC_DTSX_P1 -> -EINVAL);
 *    sndout_arc_output_type / sndout_optic_output_type no longer accept
 *    SNDOUT_CODEC_DTS ("invalid codec_type").
 *  - decoder_set_output_drop() is replaced by decoder_set_output_mode()
 *    (main/bypass output enable flags) plus a new mixer_connected_output
 *    bitmask in ALSA_SNDOUT_INFO_T (+0x210, "ConnectedMixerPort=0x%x" in
 *    /proc); see _set_output, sndout_open/close/connect/disconnect.
 *  - sndout_input_downmix_enable_put also calls decoder_set_downmix().
 *  - the bypass decision itself (_get_output_format -> bypass ->
 *    renderer BYPASS_PORT -> mixer BYPASS_PORT -> output) is the published
 *    one minus DTS; see the notes at _get_output_format / _set_output.
 */


#include <linux/module.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>

#include "os_util.h"
#include "base_device.h"
#include "debug_util.h"

#include "log.h"
#include "audio_kapi.h"

#include "block.h"
#include "input.h"
#include "decoder.h"
#include "renderer.h"
#include "mixer.h"
#include "se.h"
#include "output.h"
#include "capture.h"
#include "encoder.h"
#include "notifier.h"
#include "block_manager.h"
#include "clock.h"

#include "linux/alsa-ext/alsa-ext-renderer.h"
#include "linux/alsa-ext/alsa-ext-soundengine.h"
#include "alsa_decoder.h"
#include "alsa_se.h"
#include "alsa_platform.h"
#include "alsa_support_sndout.h"

#include "hal/hal_output.h"
#include "blind.h"
#include "common/adec_version.h"

logm_define (alsasndout, log_level_noti);
#define trace(fmt,args...)  logm_trace(alsasndout,fmt,##args)
#define debug(fmt,args...)  logm_debug(alsasndout,fmt,##args)
#define info(fmt,args...)   logm_info(alsasndout,fmt,##args)
#define noti(fmt,args...)   logm_noti(alsasndout,fmt,##args)
#define warn(fmt,args...)   logm_warning(alsasndout,fmt,##args)
#define error(fmt,args...)  logm_error(alsasndout,fmt,##args)

#define debug_call() debug("ALSA_CALL# %s(): %ld %ld %ld", __func__, ucontrol->value.integer.value[0], ucontrol->value.integer.value[1], ucontrol->value.integer.value[2])

/* webOS25-shipped: alsa_decoder.c exports decoder_set_output_mode()
 * (@0xffffffc010ccf4a0) instead of the published decoder_set_output_drop().
 * It keeps two flags per decoder (+0x55 = main output enabled, +0x56 =
 * bypass output enabled; "SAME output_mode %d, enable %d ." when unchanged)
 * and, for an HDMI decoder, calls input_set_hdmi_bypass() from them. The
 * mode names are ours (inferred from the call sites). */
enum decoder_output_mode
{
    DECODER_OUTPUT_MODE_MAIN = 0,
    DECODER_OUTPUT_MODE_BYPASS = 1,
};
extern int decoder_set_output_mode (int index, enum decoder_output_mode mode, bool enable);
extern int decoder_set_downmix (int index, bool enable);

#define ALSA_SNDOUT_DEC_NUM    4
#ifdef SUPPORT_DAFC
#define ALSA_SNDOUT_MIX_NUM    9
#else
#define ALSA_SNDOUT_MIX_NUM    8
#endif
#define ALSA_SNDOUT_INPUT_NUM  (ALSA_SNDOUT_DEC_NUM + ALSA_SNDOUT_MIX_NUM)
#define ALSA_DELAY_MIN 0
#define ALSA_INPUT_DELAY_MAX 250
#define ALSA_OUTPUT_DELAY_MAX 400
#define ALSA_INTEGER_MAX 0x7FFFFFFF
#define ALSA_INTEGER_GAIN_MAX 0x0FFFFFFF        /* +30dB, 0xFCFB724 */

#define ALSA_CAP_HW_WOW         14


#define ALSA_SNDOUT_OUTPUT_COMMON_OPTIC (COMMON_OPTIC | COMMON_OPTIC_LG | COMMON_ARC | COMMON_SE_ARC| COMMON_WOWCAST)

static DEFINE_MUTEX (_sndout_lock);

#define SNDOUT_LOCK()   mutex_lock(&_sndout_lock)
#define SNDOUT_UNLOCK() mutex_unlock(&_sndout_lock)

extern ALSA_DECODER_INFO_T alsa_decoder_info[ALSA_ADEC_MAX];

typedef enum
{
    ALSA_SNDOUT_OUTPUT_SPK,
    ALSA_SNDOUT_OUTPUT_OPTIC,
    ALSA_SNDOUT_OUTPUT_OPTIC_LG,
    ALSA_SNDOUT_OUTPUT_BLUETOOTH,
    ALSA_SNDOUT_OUTPUT_HP,
    ALSA_SNDOUT_OUTPUT_ARC,
    ALSA_SNDOUT_OUTPUT_WISA,
    ALSA_SNDOUT_OUTPUT_SE_BT,
    ALSA_SNDOUT_OUTPUT_CAPTURE,
    ALSA_SNDOUT_OUTPUT_SE_ARC,
    ALSA_SNDOUT_OUTPUT_SE_BT_SUR,
    ALSA_SNDOUT_OUTPUT_WOWCAST,
    ALSA_SNDOUT_OUTPUT_DAP_HP_BT,
#ifdef SUPPORT_DAFC
    ALSA_SNDOUT_OUTPUT_DAFC,
#endif
    ALSA_SNDOUT_OUTPUT_MAX
} ALSA_SNDOUT_OUTPUT_TYPE_T;

/**
 * AUDIO Sound Bar(Canvas) Command Get Info
 *
*/
struct aud_soundbar
{
    int id;
    int volume;
    bool mute;
    bool power_on;
};

//AUD_SOUNDBAR_INFO_T gSoundBarInfo = { 0xF048A6, };

struct ALSA_SNDOUT_GAIN_DURATION_INFO
{
    int gain;
    int duration;
};

static struct ALSA_SNDOUT_GAIN_DURATION_INFO gAlsaSndoutGainDurationInfo[ALSA_SNDOUT_INPUT_NUM];

#define GAIN_MIN 3              // when main dB Volume is 0x0 and fine dB Volume is 0x0
#define GAIN_PLUS_8DB 0x141857E
#define GAIN_MINUS_8DB 0x032F52C

#define COMMON_OUTPUT_MAX COMMON_MAX_OUTPUT

/**
 * ALSA SNDOUT Info.
 */

struct alsa_sndout_param
{
    int gain;
    int mute;
    int delay;
};

struct sndout_output_format
{
    enum output_codec_t output_codec;
    bool bypass;
    bool forced_5_1;
    int main_audio;
    int channel;                // for pcm, earc, multi channel.
};

struct alsa_sndout_list_output
{
    struct list_head list;

    bool mixer_connected;
    bool bypass_connected;
    common_output_ext_type_t output;
    struct aud_block_info *output_info;
    alsa_se_handle_t alsa_se;
    struct aud_block_info *encoder_info;
    bool encoder_connected;
    bool output_set;
    struct sndout_output_format set_output;
};

/**
 * ALSA SNDOUT Connection Info.
 */
struct alsa_sndout_list_connect
{
    struct list_head list;

    common_output_ext_type_t output;
    common_input_ext_type_t input;
    int port;
};

/**
 * ALSA SNDOUT EARC Output Type.
 */
struct alsa_sndout_output_type
{
    sndout_output_set_type_t set_type;
    sndout_output_codec_type_t codec_type;
    sndout_output_channel_num_t channel_num;
    sndout_output_sample_rate_t sample_rate;
    sndout_output_mix_option_t mix_option;
};

typedef struct
{
    bool isOpen;

    struct aud_block_info *mixer_info;
    struct list_head list_output;       ///< @see alsa_sndout_list_output
    struct list_head list_connection;   ///< @see alsa_sndout_list_connect

    sndout_optic_mode_ext_type_t optic_output_type;
    sndout_arc_mode_ext_type_t arc_output_type;

    int optic_category_code;
    int arc_category_code;

    sndout_optic_copyprotection_ext_type_t optic_copy_protection;
    sndout_arc_copyprotection_ext_type_t arc_copy_protection;

    struct alsa_sndout_param param_input[ALSA_SNDOUT_INPUT_NUM];        ///< @see alsa_sndout_param
    struct alsa_sndout_param param_output[ALSA_SNDOUT_OUTPUT_MAX];      ///< @see alsa_sndout_param
    bool input_downmix_enable[ALSA_SNDOUT_INPUT_NUM];   ///< @see input_downmix_enable

    bool optic_light_on;
    bool arc_on;
    bool earc_on;
    bool wowcast_on;
#if (MS12_VER >= 26)
    bool earc_ms12_sp50_on;
#endif

    unsigned int opened_output;
    unsigned int close_output;

    int bypass_master_index;

    enum sndout_spk_output_ext_type spk_output_type;

    common_input_ext_type_t main_audio_input;
    int main_audio_port;

    int ad_on;
    int ad_index;
    int ad_gain;
    bool eac3_atmos_encode_on;

    struct aud_soundbar soundbar;
    struct alsa_sndout_output_type earc;
    struct alsa_sndout_output_type arc;
    struct alsa_sndout_output_type optic;
    struct alsa_sndout_output_type wowcast;

    int ott;
    int atmos_locking;
    /* webOS25-shipped: new field (+0x210), bitmask of outputs fed from the
     * mixer (PCM/encoder path); printed as "ConnectedMixerPort=0x%x". */
    unsigned int mixer_connected_output;
} ALSA_SNDOUT_INFO_T;

ALSA_SNDOUT_INFO_T alsa_sndout_info[2] = { };

//static bool spdif_bypass = false;
//static bool arc_bypass = false;

struct alsa_sndout_audio_latency
{
    common_input_ext_type_t input;
    int port;
    delay_input_src_type_t src;
    common_input_ext_type_t output;
    adec_src_codec_ext_type_t codec;
};

static struct alsa_sndout_audio_latency _gSndoutAudioLatencyQuery = { 0, };

enum sndout_input_codec
{
    SNDOUT_INPUT_CODEC_AAC,
    SNDOUT_INPUT_CODEC_AC3,
    SNDOUT_INPUT_CODEC_EAC3,
    SNDOUT_INPUT_CODEC_HEAAC,   /* table is diffent from AAC, BUT IMPLEMENTATION IS SAME AS. */
    SNDOUT_INPUT_CODEC_AC4,
    SNDOUT_INPUT_CODEC_MAT,
    SNDOUT_INPUT_CODEC_MPEG_H,
    SNDOUT_INPUT_CODEC_OPUS,
    SNDOUT_INPUT_CODEC_MPEG4_AAC,
    /* webOS25-shipped: no DTS, DTS_HD, DTS_EXPRESS, DTS_HD_MA, DTS_X_1,
     * DTS_X_2 entries; OTHERS = 9 (published 15). Observed in the inlined
     * _hdmi_convert_input_codec table @0xffffffc0115ef488 (values 0..9) and
     * the optic/arc table stride (10 entries). */
    SNDOUT_INPUT_CODEC_OTHERS,
    SNDOUT_INPUT_CODEC_NUM,
};

enum sndout_input
{
    SNDOUT_INPUT_DTV,
    SNDOUT_INPUT_HDMI,
    SNDOUT_INPUT_SYSTEM,
    SNDOUT_INPUT_OTHERS,
    SNDOUT_INPUT_NUM,
};

static const struct sndout_output_format optic_connection_tb[][SNDOUT_INPUT_CODEC_NUM] = {
    /* *INDENT-OFF* */
    [SNDOUT_OPTIC_PCM] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_OPTIC_AUTO] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_OPTIC_AUTO_AAC] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AAC},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_OPTIC_FORCED_AC3_5_1] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
        },
    [SNDOUT_OPTIC_BYPASS] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_OPTIC_BYPASS_AAC] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AAC, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    /* *INDENT-ON* */
};

int _gAlsaCaptureSource;
static bool _gNewInterfaceOpticArc = false;
static unsigned int _resampling_freq = 0;
static int _gAlsaSndoutLowLatencyInfo[ALSA_SNDOUT_INPUT_NUM];

static const struct sndout_output_format *_optic_get_table (sndout_optic_mode_ext_type_t
    optic_output_type, enum sndout_input_codec input_codec)
{
    if (optic_output_type == SNDOUT_OPTIC_NONE)
        optic_output_type = SNDOUT_OPTIC_PCM;
    else if (optic_output_type == SNDOUT_OPTIC_HALF_AUTO)
        optic_output_type = SNDOUT_OPTIC_AUTO;
    else if (optic_output_type == SNDOUT_OPTIC_HALF_AUTO_AAC)
        optic_output_type = SNDOUT_OPTIC_AUTO_AAC;

    return &optic_connection_tb[optic_output_type][input_codec];
}

static enum output_codec_t _optic_get_output_codec (sndout_optic_mode_ext_type_t
    optic_output_type, enum sndout_input_codec input_codec)
{
    const struct sndout_output_format *optic_table =
        _optic_get_table (optic_output_type, input_codec);

    return optic_table->output_codec;
}

static bool _optic_get_bypass (sndout_optic_mode_ext_type_t optic_output_type,
    enum sndout_input_codec input_codec)
{
    const struct sndout_output_format *optic_table =
        _optic_get_table (optic_output_type, input_codec);

    return optic_table->bypass;
}

static bool _optic_get_forced_5_1 (sndout_optic_mode_ext_type_t optic_output_type,
    enum sndout_input_codec input_codec)
{
    const struct sndout_output_format *optic_table =
        _optic_get_table (optic_output_type, input_codec);

    return optic_table->forced_5_1;
}


static const struct sndout_output_format arc_connection_tb[][SNDOUT_INPUT_CODEC_NUM] = {
    /* *INDENT-OFF* */
    [SNDOUT_ARC_PCM] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_PCM},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_ARC_AUTO] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_ARC_AUTO_AAC] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AAC},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AAC},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_ARC_AUTO_EAC3] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_EAC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_EAC3},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_ARC_AUTO_EAC3_AAC] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AAC},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_EAC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_EAC3},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AAC},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_ARC_FORCED_AC3] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_AC3, false, true}, /* forced 5.1 ch */
        },
    [SNDOUT_ARC_FORCED_EAC3] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
        },
    [SNDOUT_ARC_BYPASS] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_ARC_BYPASS_AAC] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AAC, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AAC},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_ARC_BYPASS_EAC3] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3,  true}, /* bypass */
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_EAC3, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_ARC_BYPASS_EAC3_AAC] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AAC, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AAC},
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    [SNDOUT_ARC_BYPASS_EAC3_MPEG4_AAC] =
        {
            [SNDOUT_INPUT_CODEC_AAC]        = {OUTPUT_CODEC_AAC, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_AC3]        = {OUTPUT_CODEC_AC3, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_EAC3]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_HEAAC]      = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_AC4]        = {OUTPUT_CODEC_EAC3, false, true}, /* forced 5.1 ch */
            [SNDOUT_INPUT_CODEC_MPEG_H]     = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_OPUS]       = {OUTPUT_CODEC_AC3},
            [SNDOUT_INPUT_CODEC_MPEG4_AAC]  = {OUTPUT_CODEC_AAC, true}, /* bypass */
            [SNDOUT_INPUT_CODEC_OTHERS]     = {OUTPUT_CODEC_PCM},
        },
    /* *INDENT-ON* */
};


static const struct sndout_output_format *sndout_arc_get_table (sndout_arc_mode_ext_type_t
    optic_output_type, enum sndout_input_codec input_codec)
{
    if (optic_output_type == SNDOUT_ARC_NONE)
        optic_output_type = SNDOUT_ARC_PCM;
    else if (optic_output_type == SNDOUT_ARC_HALF_AUTO)
        optic_output_type = SNDOUT_ARC_AUTO;
    else if (optic_output_type == SNDOUT_ARC_HALF_AUTO_AAC)
        optic_output_type = SNDOUT_ARC_AUTO_AAC;
    else if (optic_output_type == SNDOUT_ARC_HALF_AUTO_EAC3)
        optic_output_type = SNDOUT_ARC_AUTO_EAC3;
    else if (optic_output_type == SNDOUT_ARC_HALF_AUTO_EAC3_AAC)
        optic_output_type = SNDOUT_ARC_AUTO_EAC3_AAC;

    return &arc_connection_tb[optic_output_type][input_codec];
}

static enum output_codec_t sndout_arc_get_output_codec (sndout_arc_mode_ext_type_t
    optic_output_type, enum sndout_input_codec input_codec)
{
    const struct sndout_output_format *arc_table =
        sndout_arc_get_table (optic_output_type, input_codec);

    return arc_table->output_codec;
}

static bool sndout_arc_get_bypass (sndout_arc_mode_ext_type_t optic_output_type,
    enum sndout_input_codec input_codec)
{
    const struct sndout_output_format *arc_table =
        sndout_arc_get_table (optic_output_type, input_codec);

    return arc_table->bypass;
}

static bool sndout_arc_get_forced_5_1 (sndout_arc_mode_ext_type_t optic_output_type,
    enum sndout_input_codec input_codec)
{
    const struct sndout_output_format *arc_table =
        sndout_arc_get_table (optic_output_type, input_codec);

    return arc_table->forced_5_1;
}

struct sndout_convert
{
    int common_output;
    const char *common_name;
    int alsa_output;
    const char *alsa_name;
    bool se;
};

#define define_sndout_convert(common,sndout) {common, #common, sndout, #sndout}
const static struct sndout_convert table_common_to_alsa[] = {
    /* *INDENT-OFF* */
    define_sndout_convert (COMMON_SPK,          ALSA_SNDOUT_OUTPUT_SPK),
    define_sndout_convert (COMMON_OPTIC,        ALSA_SNDOUT_OUTPUT_OPTIC),
    define_sndout_convert (COMMON_OPTIC_LG,     ALSA_SNDOUT_OUTPUT_OPTIC_LG),
    define_sndout_convert (COMMON_BLUETOOTH,    ALSA_SNDOUT_OUTPUT_BLUETOOTH),
    define_sndout_convert (COMMON_HP,           ALSA_SNDOUT_OUTPUT_HP),
    define_sndout_convert (COMMON_ARC,          ALSA_SNDOUT_OUTPUT_ARC),
    define_sndout_convert (COMMON_WISA,         ALSA_SNDOUT_OUTPUT_WISA),
    define_sndout_convert (COMMON_SE_BT,        ALSA_SNDOUT_OUTPUT_SE_BT),
    define_sndout_convert (COMMON_SE_BT_SUR,    ALSA_SNDOUT_OUTPUT_SE_BT_SUR),
    define_sndout_convert (COMMON_CAPTURE,      ALSA_SNDOUT_OUTPUT_CAPTURE),
    define_sndout_convert (COMMON_SE_ARC,       ALSA_SNDOUT_OUTPUT_SE_ARC),
    define_sndout_convert (COMMON_WOWCAST,      ALSA_SNDOUT_OUTPUT_WOWCAST),
    define_sndout_convert (COMMON_DAP_HP_BT,    ALSA_SNDOUT_OUTPUT_DAP_HP_BT),
#ifdef SUPPORT_DAFC
    define_sndout_convert (COMMON_DAFC,         ALSA_SNDOUT_OUTPUT_DAFC),
#endif
    define_sndout_convert (COMMON_NO_OUTPUT,    ALSA_SNDOUT_OUTPUT_MAX),
    {},
    /* *INDENT-ON* */
};

#define OUTPUT_OTHERS (OUTPUT_MAX + 1)
#define define_sndout_convert2(common,sndout) {common, #common, sndout, #sndout}
const static struct sndout_convert table_common_to_output[] = {
    /* *INDENT-OFF* */
    define_sndout_convert2 (COMMON_SPK,          OUTPUT_SPEAKER),
    define_sndout_convert2 (COMMON_OPTIC,        OUTPUT_OPTIC),
    define_sndout_convert2 (COMMON_OPTIC_LG,     OUTPUT_OPTIC),
    define_sndout_convert2 (COMMON_BLUETOOTH,    OUTPUT_OTHERS),
    define_sndout_convert2 (COMMON_HP,           OUTPUT_HP),
    define_sndout_convert2 (COMMON_ARC,          OUTPUT_ARC),
    define_sndout_convert2 (COMMON_WISA,         OUTPUT_OTHERS),
    define_sndout_convert2 (COMMON_SE_BT,        OUTPUT_OTHERS),
    define_sndout_convert2 (COMMON_SE_BT_SUR,    OUTPUT_OTHERS),
    define_sndout_convert2 (COMMON_CAPTURE,      OUTPUT_OTHERS),
    define_sndout_convert2 (COMMON_SE_ARC,       OUTPUT_ARC),
    define_sndout_convert2 (COMMON_WOWCAST,      OUTPUT_ARC),
    define_sndout_convert2 (COMMON_DAP_HP_BT,    OUTPUT_OTHERS),
#ifdef SUPPORT_DAFC
    define_sndout_convert2 (COMMON_DAFC,         OUTPUT_OTHERS),
#endif
    {},
    /* *INDENT-ON* */
};

/* @0xffffffc010cd1854 */
static int convert_type (int from, const struct sndout_convert *table)
{
    for (; table->common_name; table++)
    {
        if (table->common_output == from)
        {
            if (table != table_common_to_alsa)
                trace ("from %s(0x%x) --> to %s(0x%x)\n", table->common_name,
                    table->common_output, table->alsa_name, table->alsa_output);
            return table->alsa_output;
        }
    }

    error ("invalid parameter: 0x%x\n", from);
    return -1;
}

static int common_output_convert_output (common_output_ext_type_t output)
{
    return convert_type (output, table_common_to_output);
}

static ALSA_SNDOUT_OUTPUT_TYPE_T common_output_convert_sndout_output (common_output_ext_type_t
    output)
{
    int ret;
    ret = convert_type (output, table_common_to_alsa);
    if (ret < 0)
        return ALSA_SNDOUT_OUTPUT_MAX;
    return ret;
}

/* @0xffffffc010cd1a80 */
static int common_input_convert_index (common_input_ext_type_t input, int port)
{
    int master_index = -1;

    switch (input)
    {
        case COMMON_INPUT_ADEC:
            if (port < 0 || port >= ALSA_SNDOUT_DEC_NUM)
            {
                error ("port(0x%x) is invalid!!!\n", port);
                return -1;
            }
            master_index = port;
            break;
        case COMMON_INPUT_AMIXER:
            if (port < 0 || port >= ALSA_SNDOUT_MIX_NUM)
            {
                error ("port(0x%x) is invalid!!!\n", port);
                return -1;
            }
            master_index = port + ALSA_SNDOUT_DEC_NUM;
            break;
        case COMMON_NO_INPUT:
        default:
            error ("input(0x%x) port(0x%x) is invalid!!!\n", input, port);
            return -1;
    }

    if (input == COMMON_INPUT_ADEC)
        info ("COMMON_INPUT_ADEC port %d --> master_index %d\n", port, master_index);
    else if (input == COMMON_INPUT_AMIXER)
        info ("COMMON_INPUT_AMIXER port %d --> master_index %d\n", port, master_index);

    return master_index;
}

static void default_hp_connection (void);

/* @0xffffffc010cd3f30: sndout_init.part.0 */
static void sndout_init (void)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    if (sndout_info->isOpen != true)
    {
        sndout_info->mixer_info = bm_create_block ("mixer", 0);
        INIT_LIST_HEAD (&sndout_info->list_output);
        INIT_LIST_HEAD (&sndout_info->list_connection);

        sndout_info->isOpen = true;
        sndout_info->bypass_master_index = -1;

        sndout_info->opened_output = 0;
        sndout_info->close_output = 0;

        sndout_info->optic_output_type = SNDOUT_OPTIC_NONE;
        sndout_info->arc_output_type = SNDOUT_ARC_NONE;

        sndout_info->eac3_atmos_encode_on = ON;

        sndout_info->soundbar.id = 0xF048A6;

        sndout_info->earc.set_type = SNDOUT_OUTPUT_SET_NONE;
        sndout_info->earc.codec_type = SNDOUT_CODEC_PCM;
        sndout_info->earc.channel_num = SNDOUT_CHNUM_STEREO;
        sndout_info->earc.sample_rate = SNDOUT_SAMPLERATE_48;
        sndout_info->earc.mix_option = SNDOUT_MIXOPTION_ALL;

        sndout_info->arc.set_type = SNDOUT_OUTPUT_SET_NONE;
        sndout_info->arc.codec_type = SNDOUT_CODEC_PCM;
        sndout_info->arc.channel_num = SNDOUT_CHNUM_STEREO;
        sndout_info->arc.sample_rate = SNDOUT_SAMPLERATE_48;
        sndout_info->arc.mix_option = SNDOUT_MIXOPTION_ALL;

        sndout_info->optic.set_type = SNDOUT_OUTPUT_SET_NONE;
        sndout_info->optic.codec_type = SNDOUT_CODEC_PCM;
        sndout_info->optic.channel_num = SNDOUT_CHNUM_STEREO;
        sndout_info->optic.sample_rate = SNDOUT_SAMPLERATE_48;
        sndout_info->optic.mix_option = SNDOUT_MIXOPTION_ALL;

        sndout_info->wowcast.set_type = SNDOUT_OUTPUT_SET_NONE;
        sndout_info->wowcast.codec_type = SNDOUT_CODEC_PCM;
        sndout_info->wowcast.channel_num = SNDOUT_CHNUM_STEREO;
        sndout_info->wowcast.sample_rate = SNDOUT_SAMPLERATE_48;
        sndout_info->wowcast.mix_option = SNDOUT_MIXOPTION_ALL;

        // input_downmix_enable default true
        {
            int i;

            for (i = 0; i < ALSA_SNDOUT_INPUT_NUM; i++)
                sndout_info->input_downmix_enable[i] = true;
        }

        // default hp connection
        default_hp_connection ();
    }
}

/* @0xffffffc010cd4060 */
static struct alsa_sndout_list_output *output_get (common_output_ext_type_t output)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = NULL;

    if (sndout_info->isOpen != true)
        sndout_init ();

    list_for_each_entry (list_output, &sndout_info->list_output, list)
    {
        if (list_output->output == output)
        {
            debug ("found!");
            return list_output;
        }
    }

    return NULL;
}

/* @0xffffffc010cd4124 */
bool output_is_added (common_output_ext_type_t output)
{
    if (output_get (output) != NULL)
        return true;
    else
        return false;
}

/* @0xffffffc010cd3954 */
int copy_protection_convert_scms (unsigned int copy_protection)
{
    int convert_copy_protection[] = {
        SNDOUT_OPTIC_COPY_FREE, SNDOUT_OPTIC_COPY_NO_MORE, SNDOUT_OPTIC_COPY_ONCE,
        SNDOUT_OPTIC_COPY_NEVER,
    };
    unsigned int copy_protection_num;
    unsigned int scms;
    int i;

    copy_protection_num = sizeof (convert_copy_protection) / sizeof (convert_copy_protection[0]);

    for (i = 0; i < copy_protection_num; i++)
    {
        if (convert_copy_protection[i] == copy_protection)
            break;
    }

    if (i < copy_protection_num)
        scms = i;
    else
    {
        error ("invalid copy protection(%d)\n", copy_protection);
        return -1;
    }

    return scms;
}

/* @0xffffffc010cd3a00: output_add.constprop.0 */
static int output_add (common_output_ext_type_t output, bool mixer_connect)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output;
    int output_type = OUTPUT_MAX;
    ALSA_SNDOUT_OUTPUT_TYPE_T sndout_output = ALSA_SNDOUT_OUTPUT_MAX;
    int main_audio;

    if (output_is_added (output) == true)
    {
        error ("sndout_open failed. already used output 0x%x.", output);
        return -(EALREADY);
    }

    list_output = kcalloc (1, sizeof (struct alsa_sndout_list_output), GFP_KERNEL);
    if (!list_output)
    {
        error ("no mem\n");
        return -(EBUSY);
    }

    output_type = common_output_convert_output (output);
    if (output_type < 0)
    {
        kfree (list_output);
        error ("invalid output 0x%x.\n", output);
        return -(EOPNOTSUPP);
    }

    // first, find in block list
    if (output_type < OUTPUT_MAX)
    {
        list_output->output_info = bm_get_block_info_by_index ("output", output_type);
        if (list_output->output_info)
        {
            kfree (list_output);
            error ("already used output 0x%x, type %d.\n", output, output_type);
            return -(EALREADY);
        }

        list_output->output_info = bm_create_block ("output", output_type);
        if (!list_output->output_info)
        {
            kfree (list_output);
            error ("block create failed 0x%x, type %d.\n", output, output_type);
            return -(ENODEV);
        }

        output_set_type (list_output->output_info, output_type);
    }

    list_output->output = output;
    list_output->mixer_connected = mixer_connect;
    list_output->bypass_connected = false;

    if (output == COMMON_SPK)
    {
        list_output->alsa_se = alsa_se_get_handle ("COMMON_SPK");

        alsa_se_connect_pipeline (list_output->alsa_se);

        /* webOS25-shipped: the published "ARC bypass connected ->
         * decoder_set_output_drop (bypass_master_index, DROP_NONE)" block is
         * gone; the HDMI decoder output mode is driven by
         * decoder_set_output_mode() from sndout_open/_set_output instead. */
    }
    else if ((output == COMMON_ARC) || (output == COMMON_SE_ARC))
    {
        if (output == COMMON_SE_ARC)
        {
            list_output->alsa_se = alsa_se_get_handle ("COMMON_SE_ARC");
            alsa_se_set_type (list_output->alsa_se, "LGSE_ARC");
        }

        output_set_earc_on (list_output->output_info, sndout_info->earc_on);
    }
    else if (output == COMMON_OPTIC || output == COMMON_OPTIC_LG)
    {
        output_set_on (list_output->output_info, sndout_info->optic_light_on);
    }
    else
    {
        list_output->alsa_se = NULL;
    }

    sndout_output = common_output_convert_sndout_output (output);
    if (sndout_output >= ALSA_SNDOUT_OUTPUT_MAX)
    {
        error (" wrong sndout_output 0x%x \n", output);
        return -1;
    }

    if (list_output->output_info)
    {
        main_audio =
            common_input_convert_index (sndout_info->main_audio_input,
            sndout_info->main_audio_port);
        if (main_audio < 0)
            error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
                sndout_info->main_audio_port);

        output_set_input_index (list_output->output_info, main_audio);
        output_set_mute (list_output->output_info, sndout_info->param_output[sndout_output].mute);
        output_set_volume (list_output->output_info, sndout_info->param_output[sndout_output].gain);
        output_set_delay (list_output->output_info, sndout_info->param_output[sndout_output].delay);
        if (output == COMMON_OPTIC || output == COMMON_OPTIC_LG)
        {
            int scms;

            scms = copy_protection_convert_scms (sndout_info->optic_copy_protection);
            if (scms >= 0)
                output_set_scms (list_output->output_info, scms);
            output_set_category_code (list_output->output_info, sndout_info->optic_category_code);
        }
        else if ((output == COMMON_ARC) || (output == COMMON_SE_ARC) || (output == COMMON_WOWCAST))
        {
            int scms;

            scms = copy_protection_convert_scms (sndout_info->arc_copy_protection);
            if (scms >= 0)
                output_set_scms (list_output->output_info, scms);
            output_set_category_code (list_output->output_info, sndout_info->arc_category_code);
        }
    }

    list_add_tail (&list_output->list, &sndout_info->list_output);

    info ("output 0x%x opened.\n", output);

    return 0;
}

/* webOS25-shipped: _set_dtsx_decoder_enc_mode() is not built (its strings
 * "codec is not DTS", "output: %d, earc %s, set decoder enc mode: %d" are not
 * in the image and no caller references aud_encoder_set_dtsx_enc_mode). All
 * call sites below are removed. */

/* @0xffffffc010ccfd40 */
static int sndout_open_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = COMMON_OUTPUT_MAX;
    return 0;
}

/* @0xffffffc010cdb734 */
static int sndout_open (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    common_output_ext_type_t output = COMMON_NO_OUTPUT;
    int ret = -1;

    debug_call ();

    output = ucontrol->value.integer.value[0];

    noti ("Start Open output 0x%x.\n", output);

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    if (output != COMMON_HP)
    {
        ret = output_add (output, false);
        if (ret)
        {
            SNDOUT_UNLOCK ();
            error ("output 0x%x open fail. error in add output %d", output, ret);
            return ret;
        }
    }

    sndout_info->opened_output |= output;
    sndout_info->close_output &= ~output;

    /* webOS25-shipped: as soon as an output other than OPTIC/ARC/WOWCAST
     * (i.e. one that always plays the mixed PCM) is open, enable the main
     * output of the main decoder (shipped +0xb4..+0x1a4). */
    if (sndout_info->opened_output & ~(COMMON_OPTIC | COMMON_ARC | COMMON_WOWCAST))
    {
        sndout_info->mixer_connected_output |= output;
        decoder_set_output_mode (common_input_convert_index (sndout_info->main_audio_input,
                sndout_info->main_audio_port), DECODER_OUTPUT_MODE_MAIN, true);
    }

    SNDOUT_UNLOCK ();

    noti ("output 0x%x opened.\n", output);

    return 0;
}

/* @0xffffffc010cd0c90 */
static int sndout_open_get (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->opened_output;

    info ("opened output 0x%lx.\n", ucontrol->value.integer.value[0]);

    return 0;
}

/* @0xffffffc010cd6d40 */
static int sndout_close (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    common_output_ext_type_t output = COMMON_NO_OUTPUT;
    struct alsa_sndout_list_output *list_output = NULL;
    struct alsa_sndout_list_connect *sndoutConnectInfo = NULL, *nextSndoutConnectInfo = NULL;

    debug_call ();

    output = ucontrol->value.integer.value[0];

    noti ("Start Close output 0x%x.\n", output);

    SNDOUT_LOCK ();
    if (output_is_added (output) == false)
    {
        SNDOUT_UNLOCK ();
        error ("can not find output\n");
        return -(EALREADY);
    }

    //AUDIO_RestartLock (__func__, __LINE__);

    if (output != COMMON_HP)
    {
        list_for_each_entry_safe (sndoutConnectInfo, nextSndoutConnectInfo,
            &sndout_info->list_connection, list)
        {
            if (sndoutConnectInfo->output == output)
            {
                list_del (&sndoutConnectInfo->list);
                kfree (sndoutConnectInfo);
            }
        }

        list_output = output_get (output);
        if (list_output)
        {
            if ((output == COMMON_SE_BT) || (output == COMMON_SE_BT_SUR))
            {
                if (output_is_added (COMMON_SPK))
                {
                    bm_disconnect_block (list_output->output_info, 0);
                }
            }
            else if (output == COMMON_DAP_HP_BT)
            {
                info ("DAP_HP_BT pipeline is destroied at acapture_close().\n");
            }
            else
            {
                if (list_output->alsa_se)
                {
                    alsa_se_set_type (list_output->alsa_se, NULL);
                }
            }

            /* webOS25-shipped: no SPK special case; any output that has the
             * bypass path connected switches the bypass (sub) output of the
             * main decoder off (shipped +0x168 / +0x2c8). */
            if (list_output->bypass_connected)
            {
                SNDOUT_UNLOCK ();
                decoder_set_output_mode (sndout_info->bypass_master_index,
                    DECODER_OUTPUT_MODE_BYPASS, false);
                SNDOUT_LOCK ();
            }

            if (list_output->output_info)
                bm_destroy_block (list_output->output_info);

            if (list_output->encoder_info)
                bm_destroy_block (list_output->encoder_info);

            list_del (&list_output->list);
            kfree (list_output);
        }
    }

    //AUDIO_RestartUnlock (__func__, __LINE__);

    sndout_info->opened_output &= ~output;
    sndout_info->close_output |= output;

    /* webOS25-shipped: replaces the published DTS enc-mode block
     * ("main_audio: %d, codec: %d" is not in the image). When no output
     * uses the mixed (main) path any more, the main output of the decoder
     * is switched off. */
    sndout_info->mixer_connected_output &= ~output;
    if (!sndout_info->mixer_connected_output)
        decoder_set_output_mode (common_input_convert_index (sndout_info->main_audio_input,
                sndout_info->main_audio_port), DECODER_OUTPUT_MODE_MAIN, false);

    SNDOUT_UNLOCK ();

    noti ("output 0x%x closed.\n", output);

    return 0;
}

static bool _is_output_input_connected (common_output_ext_type_t output,
    common_input_ext_type_t input, int port)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_connect *sndoutConnectInfo = NULL;
    bool connected = false;

    list_for_each_entry (sndoutConnectInfo, &sndout_info->list_connection, list)
    {
        if (sndoutConnectInfo->output == output
            && sndoutConnectInfo->input == input && sndoutConnectInfo->port == port)
        {
            connected = true;
        }
    }

    debug ("connected %s (output:0x%x, input:0x%x, port:0x%x)", (connected) ? "TRUE" : "FALSE",
        output, input, port);
    return connected;
}


static int _del_connection (common_output_ext_type_t output, common_input_ext_type_t input,
    int port)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_connect *sndoutConnectInfo = NULL;

    list_for_each_entry (sndoutConnectInfo, &sndout_info->list_connection, list)
    {
        if (sndoutConnectInfo->output == output
            && sndoutConnectInfo->input == input && sndoutConnectInfo->port == port)
        {
            debug ("delete connected (output:0x%x, input:0x%x, port:0x%x)", output, input, port);

            list_del (&sndoutConnectInfo->list);
            kfree (sndoutConnectInfo);
            return 0;
        }
    }

    error ("no connection (output:0x%x, input:0x%x, port:0x%x)", output, input, port);
    return -(EALREADY);
}


static int _add_connection (common_output_ext_type_t output, common_input_ext_type_t input,
    int port)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_connect *sndoutConnectInfo = NULL;

    sndoutConnectInfo = kcalloc (1, sizeof (struct alsa_sndout_list_connect), GFP_KERNEL);
    if (!sndoutConnectInfo)
    {
        error ("no mem\n");
        return -(EBUSY);
    }

    sndoutConnectInfo->output = output;
    sndoutConnectInfo->input = input;
    sndoutConnectInfo->port = port;

    list_add_tail (&sndoutConnectInfo->list, &sndout_info->list_connection);

    return 0;
}

static bool _input_connected_any (common_input_ext_type_t input, int port)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_connect *sndoutConnectInfo = NULL;
    list_for_each_entry (sndoutConnectInfo, &sndout_info->list_connection, list)
    {
        if (sndoutConnectInfo->input == input && sndoutConnectInfo->port == port)
            return true;
    }

    return false;
}

static bool _output_connected_any (common_output_ext_type_t output)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_connect *sndoutConnectInfo = NULL;

    list_for_each_entry (sndoutConnectInfo, &sndout_info->list_connection, list)
    {
        if (sndoutConnectInfo->output == output)
            return true;
    }

    return false;
}

/* @0xffffffc010cdbb64 */
bool sndout_output_connected_any (common_output_ext_type_t output)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    bool connected_any;

    if (sndout_info->isOpen != true)
        sndout_init ();

    SNDOUT_LOCK ();
    connected_any = _output_connected_any (output);
    SNDOUT_UNLOCK ();

    return connected_any;
}

/* @0xffffffc010cd0990 */
static int sndout_close_get (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    ucontrol->value.integer.value[0] = sndout_info->close_output;

    info ("closed output 0x%lx.\n", ucontrol->value.integer.value[0]);

    return 0;
}

/* @0xffffffc010cdbc10 */
int sndout_get_capture_gain (void)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    ALSA_SNDOUT_OUTPUT_TYPE_T sndout_output = ALSA_SNDOUT_OUTPUT_MAX;
    sndout_output = common_output_convert_sndout_output (COMMON_OPTIC);

    if (sndout_info->isOpen != true)
        sndout_init ();

    if (_output_connected_any (COMMON_SE_BT))
    {
        info ("Capture SE_BT\n");
        sndout_output = common_output_convert_sndout_output (COMMON_SE_BT);
    }
    else if (_output_connected_any (COMMON_SE_BT_SUR))
    {
        info ("Capture SE_BT_SUR\n");
        sndout_output = common_output_convert_sndout_output (COMMON_SE_BT_SUR);
    }
    else if (_output_connected_any (COMMON_BLUETOOTH))
    {
        info ("Capture for BT\n");
        sndout_output = common_output_convert_sndout_output (COMMON_BLUETOOTH);
    }
    else if (_output_connected_any (COMMON_WISA))
    {
        info ("Capture for WiSA\n");
        sndout_output = common_output_convert_sndout_output (COMMON_WISA);
    }
    else if (_output_connected_any (COMMON_CAPTURE))
    {
        info ("Capture for Capture Device\n");
        sndout_output = common_output_convert_sndout_output (COMMON_CAPTURE);
    }
    else if (_output_connected_any (COMMON_DAP_HP_BT))
    {
        info ("Capture for DAP_HP_BT\n");
        sndout_output = common_output_convert_sndout_output (COMMON_DAP_HP_BT);
    }
#ifdef SUPPORT_DAFC
    else if (_output_connected_any (COMMON_DAFC))
    {
        info ("Capture for DAFC Device\n");
        sndout_output = common_output_convert_sndout_output (COMMON_DAFC);
    }
#endif
    else
    {
        info ("Capture for Music recognition\n");
        return 0x800000;
    }

    return sndout_info->param_output[sndout_output].gain;
}

/* @0xffffffc010cdc000 */
bool sndout_get_capture_mute (void)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    ALSA_SNDOUT_OUTPUT_TYPE_T sndout_output = ALSA_SNDOUT_OUTPUT_MAX;
    sndout_output = common_output_convert_sndout_output (COMMON_OPTIC);

    if (sndout_info->isOpen != true)
        sndout_init ();

    if (_output_connected_any (COMMON_SE_BT))
    {
        info ("Capture SE_BT\n");
        sndout_output = common_output_convert_sndout_output (COMMON_SE_BT);
    }
    else if (_output_connected_any (COMMON_SE_BT_SUR))
    {
        info ("Capture SE_BT_SUR\n");
        sndout_output = common_output_convert_sndout_output (COMMON_SE_BT_SUR);
    }
    else if (_output_connected_any (COMMON_BLUETOOTH))
    {
        info ("Capture for BT\n");
        sndout_output = common_output_convert_sndout_output (COMMON_BLUETOOTH);
    }
    else if (_output_connected_any (COMMON_WISA))
    {
        info ("Capture for WiSA\n");
        sndout_output = common_output_convert_sndout_output (COMMON_WISA);
    }
    else if (_output_connected_any (COMMON_CAPTURE))
    {
        info ("Capture for Capture Device\n");
        sndout_output = common_output_convert_sndout_output (COMMON_CAPTURE);
    }
    else if (_output_connected_any (COMMON_DAP_HP_BT))
    {
        info ("Capture for DAP_HP_BT Device\n");
        sndout_output = common_output_convert_sndout_output (COMMON_DAP_HP_BT);
    }
#ifdef SUPPORT_DAFC
    else if (_output_connected_any (COMMON_DAFC))
    {
        info ("Capture for DAFC Device\n");
        sndout_output = common_output_convert_sndout_output (COMMON_DAFC);
    }
#endif
    else
    {
        info ("Capture for Music recognition\n");
        return false;
    }

    return sndout_info->param_output[sndout_output].mute;
}

/* @0xffffffc010cd4240 */
struct aud_block_info *sndout_get_se_info (common_output_ext_type_t output)
{
    struct aud_block_info *se_info = NULL;
    struct alsa_sndout_list_output *list_output = output_get (output);
    if (list_output)
    {
        if (list_output->alsa_se)
            se_info = alsa_se_get_block (list_output->alsa_se);
        else
            error ("list_output->alsa_se is NULL\n");
    }
    else
        error ("list_output is NULL\n");

    return se_info;
}

EXPORT_SYMBOL (sndout_get_se_info);

/* @0xffffffc010cd2870 */
static void _connect_pcm (struct alsa_sndout_list_output *list_output)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct aud_block_info *front_block = sndout_info->mixer_info;
    int main_audio;

    if (list_output->alsa_se)
    {
        front_block = alsa_se_get_block (list_output->alsa_se);

        if (front_block != NULL)
        {
            bm_connect_block (sndout_info->mixer_info, 0, front_block, 0);
            bm_connect_block (sndout_info->mixer_info, 2, front_block, 1);
        }
        else
        {
            warn ("can't get block info for se\n");
            front_block = sndout_info->mixer_info;
        }
    }

    // if output earc , codec_type pcm, channel_num 2, 6 or 8, mix_option on,
    // connect mixer 2nd port (PCM 8ch).
    main_audio =
        common_input_convert_index (sndout_info->main_audio_input, sndout_info->main_audio_port);

    if (((list_output->output == COMMON_ARC) && sndout_info->earc_on) &&
        (sndout_info->earc.codec_type == SNDOUT_CODEC_PCM) && sndout_info->earc.mix_option)
    {
        if ((sndout_info->earc.channel_num == 6) || (sndout_info->earc.channel_num == 8))
        {
            if ((main_audio >= 0) && (main_audio < ALSA_ADEC_MAX))
                decoder_set_multi_pcm_out (main_audio, true);
            bm_connect_block (front_block, 2, list_output->output_info, 0);
        }
        else if (sndout_info->earc.channel_num == 2)
        {
            if ((main_audio >= 0) && (main_audio < ALSA_ADEC_MAX))
                decoder_set_multi_pcm_out (main_audio, true);
            bm_connect_block (front_block, 0, list_output->output_info, 0);
        }
        else
        {
            if ((main_audio >= 0) && (main_audio < ALSA_ADEC_MAX))
                decoder_set_multi_pcm_out (main_audio, false);
            bm_connect_block (front_block, 0, list_output->output_info, 0);
        }
    }
    else if (((list_output->output == COMMON_WOWCAST) && sndout_info->wowcast_on) &&
        (sndout_info->wowcast.codec_type == SNDOUT_CODEC_PCM) && sndout_info->wowcast.mix_option)
    {
        if ((sndout_info->wowcast.channel_num == 6) || (sndout_info->wowcast.channel_num == 8))
        {
            if ((main_audio >= 0) && (main_audio < ALSA_ADEC_MAX))
                decoder_set_multi_pcm_out (main_audio, true);
            bm_connect_block (front_block, 2, list_output->output_info, 0);
        }
        else if (sndout_info->wowcast.channel_num == 2)
        {
            if ((main_audio >= 0) && (main_audio < ALSA_ADEC_MAX))
                decoder_set_multi_pcm_out (main_audio, true);
            bm_connect_block (front_block, 0, list_output->output_info, 0);
        }
        else
        {
            if ((main_audio >= 0) && (main_audio < ALSA_ADEC_MAX))
                decoder_set_multi_pcm_out (main_audio, false);
            bm_connect_block (front_block, 0, list_output->output_info, 0);
        }
    }
    else
    {
        if ((main_audio >= 0) && (main_audio < ALSA_ADEC_MAX))
            decoder_set_multi_pcm_out (main_audio, false);

        if (list_output->output_info)
            bm_connect_block (front_block, 0, list_output->output_info, 0);
    }
}

static void default_hp_connection (void)
{
    struct alsa_sndout_list_output *list_output = NULL;

    if (output_add (COMMON_HP, false))
    {
        error ("Default HP connection fails.\n");
        return;
    }

    list_output = output_get (COMMON_HP);
    if (list_output)
        _connect_pcm (list_output);
}

/*
 * webOS25-shipped: no DTS cases (published has DTS/DTS_HD/DTS_EXPRESS/DTS_HD_MA/
 * DTS_X_1/DTS_X_2 via decoded_info->dts_detail). decoded_info->codec is at +0x4.
 */
/* @0xffffffc010ccfd60 */
static enum sndout_input_codec _decoded_info_convert_input_codec (struct decoded_info *decoded_info)
{
    switch (decoded_info->codec)
    {
        case AUDIO_DECODER_CODEC_AC3:
            return decoded_info->eac3 ? SNDOUT_INPUT_CODEC_EAC3 : SNDOUT_INPUT_CODEC_AC3;
        case AUDIO_DECODER_CODEC_EAC3:
            return SNDOUT_INPUT_CODEC_EAC3;
        case AUDIO_DECODER_CODEC_AC4:
            return SNDOUT_INPUT_CODEC_AC4;
        case AUDIO_DECODER_CODEC_MAT:
        case AUDIO_DECODER_CODEC_TRUE_HD:
            return SNDOUT_INPUT_CODEC_MAT;
        case AUDIO_DECODER_CODEC_AAC:
            return decoded_info->version == 0 ? SNDOUT_INPUT_CODEC_AAC : SNDOUT_INPUT_CODEC_HEAAC;
        case AUDIO_DECODER_CODEC_MPEG_H:
            return SNDOUT_INPUT_CODEC_MPEG_H;
        case AUDIO_DECODER_CODEC_OPUS:  /* 0x11 in the shipped enum */
            return SNDOUT_INPUT_CODEC_OPUS;
        default:
            return SNDOUT_INPUT_CODEC_OTHERS;
    }
}

static enum sndout_input_codec _hdmi_convert_input_codec (enum audio_decoder_codec dec_codec)
{
    enum sndout_input_codec input_codec = SNDOUT_INPUT_CODEC_OTHERS;

    switch (dec_codec)
    {
        case AUDIO_DECODER_CODEC_AC3:
            input_codec = SNDOUT_INPUT_CODEC_AC3;
            break;

        case AUDIO_DECODER_CODEC_EAC3:
            input_codec = SNDOUT_INPUT_CODEC_EAC3;
            break;

        case AUDIO_DECODER_CODEC_AAC:
            input_codec = SNDOUT_INPUT_CODEC_AAC;
            break;

        case AUDIO_DECODER_CODEC_AC4:
            input_codec = SNDOUT_INPUT_CODEC_AC4;
            break;

        case AUDIO_DECODER_CODEC_MPEG_H:
            input_codec = SNDOUT_INPUT_CODEC_MPEG_H;
            break;

        case AUDIO_DECODER_CODEC_OPUS:
            input_codec = SNDOUT_INPUT_CODEC_OPUS;
            break;

        case AUDIO_DECODER_CODEC_MAT:
        case AUDIO_DECODER_CODEC_TRUE_HD:
            input_codec = SNDOUT_INPUT_CODEC_MAT;
            break;

        /* webOS25-shipped: no DTS / DTS_HD cases (inlined table
         * @0xffffffc0115ef488, all non-listed codecs -> OTHERS). */

        default:
            break;
    }

    return input_codec;
}

static enum sndout_input _decoder_input_convert_input (enum audio_decoder_input decoder_input)
{
    enum sndout_input input = SNDOUT_INPUT_OTHERS;

    switch (decoder_input)
    {
        case AUDIO_DECODER_INPUT_TP0:
        case AUDIO_DECODER_INPUT_TP1:
            input = SNDOUT_INPUT_DTV;
            break;

        case AUDIO_DECODER_INPUT_HDMI0:
        case AUDIO_DECODER_INPUT_HDMI1:
        case AUDIO_DECODER_INPUT_HDMI2:
        case AUDIO_DECODER_INPUT_HDMI3:
            input = SNDOUT_INPUT_HDMI;
            break;

        case AUDIO_DECODER_INPUT_SYSTEM:
        case AUDIO_DECODER_INPUT_SYSTEM_CLIP:
            input = SNDOUT_INPUT_SYSTEM;
            break;

        default:
            break;
    }

    return input;
}

/* @0xffffffc010cd1c70 */
static void _optic_convert_output_codec (sndout_optic_mode_ext_type_t optic_output_type,
    enum output_codec_t *output_codec, bool * bypass, bool * forced_5_1)
{
    enum sndout_input_codec input_codec = SNDOUT_INPUT_CODEC_OTHERS;
    enum sndout_input input = SNDOUT_INPUT_OTHERS;

    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int main_audio;
    struct aud_block_info *decoder_block = NULL;
    struct decoded_info decoded_info = { };
    enum audio_decoder_input decoder_input;

    main_audio =
        common_input_convert_index (sndout_info->main_audio_input, sndout_info->main_audio_port);
    if (main_audio < 0)
        error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
            sndout_info->main_audio_port);
    else
        decoder_block = bm_get_block_info_by_index ("decoder", main_audio);

    if (decoder_block)
    {
        decoder_get_decoded_info (decoder_block, &decoded_info);
        decoder_input = decoder_get_input (decoder_block);

        input = _decoder_input_convert_input (decoder_input);

        if (input == SNDOUT_INPUT_HDMI)
        {
            struct aud_block_info *input_block;
            enum audio_decoder_codec dec_codec;

            input_block = bm_get_block_info_by_index ("input", main_audio);
            if (input_block)
            {
                dec_codec = input_get_hdmi_codec (input_block);
                input_codec = _hdmi_convert_input_codec (dec_codec);
            }
        }
        else
            input_codec = _decoded_info_convert_input_codec (&decoded_info);

        // optic input_codec exceptional case
        if (input_codec == SNDOUT_INPUT_CODEC_MAT)
            input_codec = SNDOUT_INPUT_CODEC_AC4;
    }

    *output_codec = _optic_get_output_codec (optic_output_type, input_codec);
    *bypass = _optic_get_bypass (optic_output_type, input_codec);
    *forced_5_1 = _optic_get_forced_5_1 (optic_output_type, input_codec);

    // optic input & output_type exceptional case
    if ((input == SNDOUT_INPUT_SYSTEM) && (input_codec == SNDOUT_INPUT_CODEC_AAC))
    {
        if ((optic_output_type == SNDOUT_OPTIC_AUTO_AAC)
            || (optic_output_type == SNDOUT_OPTIC_HALF_AUTO_AAC)
            || (optic_output_type == SNDOUT_OPTIC_BYPASS_AAC))
        {
            *output_codec = OUTPUT_CODEC_AC3;
            *bypass = false;
            *forced_5_1 = false;
        }
    }

    debug ("optic_output_type %d input_codec %d.\n", optic_output_type, input_codec);
    debug ("output_codec %d bypass %d forced_5_1 %d.\n", *output_codec, *bypass, *forced_5_1);
    return;
}

/* @0xffffffc010cd2000 */
static void sndout_arc_convert_output_codec (sndout_arc_mode_ext_type_t arc_output_type,
    enum output_codec_t *output_codec, bool * bypass, bool * forced_5_1)
{
    enum sndout_input_codec input_codec = SNDOUT_INPUT_CODEC_OTHERS;
    enum sndout_input input = SNDOUT_INPUT_OTHERS;

    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int main_audio;
    struct aud_block_info *decoder_block = NULL;
    struct decoded_info decoded_info = { };
    enum audio_decoder_input decoder_input;

    main_audio =
        common_input_convert_index (sndout_info->main_audio_input, sndout_info->main_audio_port);
    if (main_audio < 0)
        error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
            sndout_info->main_audio_port);
    else
        decoder_block = bm_get_block_info_by_index ("decoder", main_audio);

    if (decoder_block)
    {
        decoder_get_decoded_info (decoder_block, &decoded_info);
        decoder_input = decoder_get_input (decoder_block);

        input = _decoder_input_convert_input (decoder_input);

        if (input == SNDOUT_INPUT_HDMI)
        {
            struct aud_block_info *input_block;
            enum audio_decoder_codec dec_codec;

            input_block = bm_get_block_info_by_index ("input", main_audio);
            if (input_block)
            {
                dec_codec = input_get_hdmi_codec (input_block);
                input_codec = _hdmi_convert_input_codec (dec_codec);
            }
        }
        else
            input_codec = _decoded_info_convert_input_codec (&decoded_info);

        // arc input_codec exceptional case
        if (input_codec == SNDOUT_INPUT_CODEC_MAT)
            input_codec = SNDOUT_INPUT_CODEC_AC4;
        else if (input_codec == SNDOUT_INPUT_CODEC_AAC)
        {
            struct aud_block_info *ren_block = NULL;

            ren_block = bm_get_block_info_by_index ("renderer", main_audio);
            if (ren_block)
            {
                bool bJP4K = false;

                bJP4K = renderer_get_jp4k (ren_block);
                if (bJP4K)
                    input_codec = SNDOUT_INPUT_CODEC_MPEG4_AAC;
            }
        }
    }

    *output_codec = sndout_arc_get_output_codec (arc_output_type, input_codec);
    *bypass = sndout_arc_get_bypass (arc_output_type, input_codec);
    *forced_5_1 = sndout_arc_get_forced_5_1 (arc_output_type, input_codec);

    // arc input & output_type exceptional case
    if ((input == SNDOUT_INPUT_SYSTEM) && (input_codec == SNDOUT_INPUT_CODEC_AAC))
    {
        if ((arc_output_type == SNDOUT_ARC_AUTO_AAC)
            || (arc_output_type == SNDOUT_ARC_AUTO_EAC3_AAC)
            || (arc_output_type == SNDOUT_ARC_HALF_AUTO_AAC)
            || (arc_output_type == SNDOUT_ARC_HALF_AUTO_EAC3_AAC)
            || (arc_output_type == SNDOUT_ARC_BYPASS_AAC)
            || (arc_output_type == SNDOUT_ARC_BYPASS_EAC3_AAC))
        {
            *output_codec = OUTPUT_CODEC_AC3;
            *bypass = false;
            *forced_5_1 = false;
        }
        else if (arc_output_type == SNDOUT_ARC_BYPASS_EAC3_MPEG4_AAC)
        {
            *output_codec = OUTPUT_CODEC_AAC;
            *bypass = true;
            *forced_5_1 = false;
        }
    }
    else if (((input == SNDOUT_INPUT_HDMI) || (input == SNDOUT_INPUT_SYSTEM))
        && (input_codec == SNDOUT_INPUT_CODEC_HEAAC))
    {
        if ((arc_output_type == SNDOUT_ARC_AUTO_EAC3)
            || (arc_output_type == SNDOUT_ARC_AUTO_EAC3_AAC)
            || (arc_output_type == SNDOUT_ARC_HALF_AUTO_EAC3)
            || (arc_output_type == SNDOUT_ARC_HALF_AUTO_EAC3_AAC)
            || (arc_output_type == SNDOUT_ARC_BYPASS_EAC3)
            || (arc_output_type == SNDOUT_ARC_BYPASS_EAC3_AAC))
        {
            *output_codec = OUTPUT_CODEC_EAC3;
            *bypass = false;
            *forced_5_1 = false;
        }
    }

    debug ("arc_output_type %d input_codec %d.\n", arc_output_type, input_codec);
    debug ("output_codec %d bypass %d forced_5_1 %d.\n", *output_codec, *bypass, *forced_5_1);
    return;
}

/*
 * webOS25-shipped: no DTS outputs. SNDOUT_CODEC_DTS(4), DTSHD(8),
 * DTSHD_MA(10), DTSX_P1(11) all fall into the default ("Not supported
 * output(%d). Default PCM outputs.") -> OUTPUT_CODEC_PCM. Note that 7
 * (MAT2PCM) and 6 (TRUEHD) are handled by a single "<= 7" range test.
 */
/* @0xffffffc010cd1990 */
static enum output_codec_t sndout_codec_convert_output_codec (sndout_output_codec_type_t output_type)
{
    switch (output_type)
    {
        case SNDOUT_CODEC_PCM:
            return OUTPUT_CODEC_PCM;
        case SNDOUT_CODEC_AC3:
            return OUTPUT_CODEC_AC3;
        case SNDOUT_CODEC_AAC:
        case SNDOUT_CODEC_MPEG4_AAC:
            return OUTPUT_CODEC_AAC;
        case SNDOUT_CODEC_EAC3:
            return OUTPUT_CODEC_EAC3;
        case SNDOUT_CODEC_TRUEHD:
        case SNDOUT_CODEC_MAT2PCM:
            return OUTPUT_CODEC_MAT;
        default:
            error ("Not supported output(%d). Default PCM outputs.\n", output_type);
            return OUTPUT_CODEC_PCM;
    }
}

static sndout_output_codec_type_t output_codec_convert_sndout_output_codec (enum output_codec_t
    output_codec)
{
    sndout_output_codec_type_t sndout_output_codec = SNDOUT_CODEC_PCM;
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = NULL;
    struct sndout_output_format *set_output = &list_output->set_output;
    struct aud_block_info *decoder_block = NULL;
    struct decoded_info decoded_info = { };
    int main_audio;

    list_output = output_get (COMMON_WOWCAST);
    if (list_output)
        set_output = &list_output->set_output;

    main_audio =
        common_input_convert_index (sndout_info->main_audio_input, sndout_info->main_audio_port);
    if (main_audio < 0)
    {
        error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
            sndout_info->main_audio_port);
    }
    else
    {
        decoder_block = bm_get_block_info_by_index ("decoder", main_audio);

        if (decoder_block)
        {
            enum audio_decoder_input decoder_input;
            decoder_input = decoder_get_input (decoder_block);
            if ((decoder_input == AUDIO_DECODER_INPUT_SIF)
                || (decoder_input == AUDIO_DECODER_INPUT_ADC))
            {
                //   codec = ADEC_SRC_CODEC_PCM;
            }
            else
            {
                decoder_get_decoded_info (decoder_block, &decoded_info);
            }
        }
    }

    switch (output_codec)
    {
        case OUTPUT_CODEC_PCM:
            sndout_output_codec = SNDOUT_CODEC_PCM;
            break;
        case OUTPUT_CODEC_AC3:
            sndout_output_codec = SNDOUT_CODEC_AC3;
            break;
        case OUTPUT_CODEC_EAC3:
            sndout_output_codec = SNDOUT_CODEC_EAC3;
            break;
        case OUTPUT_CODEC_AAC:
            sndout_output_codec = SNDOUT_CODEC_AAC;
            break;
        case OUTPUT_CODEC_MAT:
            if (list_output && set_output->bypass && decoded_info.truehd)
                sndout_output_codec = SNDOUT_CODEC_TRUEHD;
            else
                sndout_output_codec = SNDOUT_CODEC_MAT2PCM;
            break;
        /* webOS25-shipped: no OUTPUT_CODEC_DTS* (enum output_codec_t stops
         * at MAT); inlined in sndout_*_output_type_get. */

        default:
            error ("Not supported output(%d). Default PCM outputs.\n", output_codec);
            break;
    }

    return sndout_output_codec;
}

/* webOS25-shipped: no DTS input codecs. */
/* @0xffffffc010cd17a0 */
static enum output_codec_t input_codec_convert_output_codec (enum sndout_input_codec input_codec)
{
    switch (input_codec)
    {
        case SNDOUT_INPUT_CODEC_AAC:
        case SNDOUT_INPUT_CODEC_MPEG4_AAC:
            return OUTPUT_CODEC_AAC;
        case SNDOUT_INPUT_CODEC_AC3:
            return OUTPUT_CODEC_AC3;
        case SNDOUT_INPUT_CODEC_EAC3:
            return OUTPUT_CODEC_EAC3;
        case SNDOUT_INPUT_CODEC_MAT:
            return OUTPUT_CODEC_MAT;
        default:
            error ("input_codec(%d). Default PCM outputs.\n", input_codec);
            return OUTPUT_CODEC_PCM;
    }
}

static sndout_output_codec_type_t input_codec_convert_sndout_codec (enum sndout_input_codec
    input_codec)
{
    sndout_output_codec_type_t sndout_codec = SNDOUT_CODEC_PCM;

    switch (input_codec)
    {
        case SNDOUT_INPUT_CODEC_AAC:
            sndout_codec = SNDOUT_CODEC_AAC;
            break;
        case SNDOUT_INPUT_CODEC_MPEG4_AAC:
            sndout_codec = SNDOUT_CODEC_MPEG4_AAC;
            break;
        case SNDOUT_INPUT_CODEC_AC3:
            sndout_codec = SNDOUT_CODEC_AC3;
            break;
        case SNDOUT_INPUT_CODEC_EAC3:
            sndout_codec = SNDOUT_CODEC_EAC3;
            break;
        case SNDOUT_INPUT_CODEC_MAT:
            sndout_codec = SNDOUT_CODEC_MAT2PCM;
            break;
        /* webOS25-shipped: no DTS input codecs (inlined in
         * _get_output_format_set_type, which only maps 0,1,2,5,8). */
        default:
            error ("input_codec(%d). Default PCM outputs.\n", input_codec);
            break;
    }

    return sndout_codec;
}

static void _use_output (struct alsa_sndout_output_type *output,
    enum sndout_input_codec input_codec, enum output_codec_t *output_codec, bool * bypass)
{
    if (output->mix_option)
    {
        *output_codec = sndout_codec_convert_output_codec (output->codec_type);
        *bypass = false;
    }
    else
    {
        sndout_output_codec_type_t sndout_output_codec =
            input_codec_convert_sndout_codec (input_codec);

        *output_codec = sndout_codec_convert_output_codec (output->codec_type);
        if (sndout_output_codec == output->codec_type)
            *bypass = true;
        else if (((sndout_output_codec == SNDOUT_CODEC_TRUEHD)
                || (sndout_output_codec == SNDOUT_CODEC_MAT2PCM))
            && ((output->codec_type == SNDOUT_CODEC_TRUEHD
                    || (output->codec_type == SNDOUT_CODEC_MAT2PCM))))
            *bypass = true;
        else
        {
            *output_codec = OUTPUT_CODEC_PCM;
            *bypass = false;
        }
    }
}

static void _org_output (struct alsa_sndout_output_type *output,
    enum sndout_input_codec input_codec, enum output_codec_t *output_codec, bool * bypass)
{
    if (output->mix_option)
    {
        *output_codec = input_codec_convert_output_codec (input_codec);
        *bypass = false;
    }
    else
    {
        *output_codec = input_codec_convert_output_codec (input_codec);
        if (*output_codec == OUTPUT_CODEC_PCM)
            *bypass = false;
        else
            *bypass = true;
    }
}

/* @0xffffffc010cd2af4 */
static void _get_output_format_set_type (struct alsa_sndout_output_type *output,
    enum sndout_input_codec input_codec, enum output_codec_t *output_codec, bool * bypass)
{
    if (output->set_type == SNDOUT_OUTPUT_SET_USE)
        _use_output (output, input_codec, output_codec, bypass);
    else
        _org_output (output, input_codec, output_codec, bypass);
}

static bool _is_decoder_bypass (struct decoded_info *decoded_info)
{
    bool is_bypass = false;

    switch (decoded_info->codec)
    {
        case AUDIO_DECODER_CODEC_AC3:
        case AUDIO_DECODER_CODEC_EAC3:
        case AUDIO_DECODER_CODEC_MAT:
        case AUDIO_DECODER_CODEC_TRUE_HD:
        case AUDIO_DECODER_CODEC_AAC:
            /* webOS25-shipped: no DTS / DTS_HD (shipped: codec in 2..3 or 5..7) */
            is_bypass = true;
            break;

            //case AUDIO_DECODER_CODEC_PCM:
            //case AUDIO_DECODER_CODEC_AC4:
            //case AUDIO_DECODER_CODEC_MP3:
            //case AUDIO_DECODER_CODEC_MPEG_H:
            //case AUDIO_DECODER_CODEC_DRA:
            //case AUDIO_DECODER_CODEC_WMA_PRO:
            //case AUDIO_DECODER_CODEC_VORBIS:
            //case AUDIO_DECODER_CODEC_AMR_WB:
            //case AUDIO_DECODER_CODEC_AMR_NB:
            //case AUDIO_DECODER_CODEC_ADPCM:
            //case AUDIO_DECODER_CODEC_FLAC:
            //case AUDIO_DECODER_CODEC_OPUS:
            //case AUDIO_DECODER_CODEC_AAC_BYPASS:
            //case AUDIO_DECODER_CODEC_AAC_ELD:
            //case AUDIO_DECODER_CODEC_RA8:
        default:
            break;
    }

    debug ("%d is_bypas : %s.\n", decoded_info->codec, is_bypass ? "true" : "false");

    return is_bypass;
}

/*
 * webOS25-shipped (@0xffffffc010cd3010, 0x570 bytes, read instruction by
 * instruction): the published decision minus every DTS special case.
 * _decoder_input_convert_input, _hdmi_convert_input_codec (16-entry table
 * @0xffffffc0115ef488), _is_decoder_bypass (codec in {AC3,EAC3} or
 * {MAT,TRUE_HD,AAC}) and _get_output_format_set_type's helpers are inlined.
 *
 * The bypass flag decides, in _set_output, whether
 *   bm_connect_block (mixer, BYPASS_PORT, output, 0) and
 *   bm_connect_block_reserved_out ("renderer", main_audio, BYPASS_PORT,
 *                                  mixer, BYPASS_PORT)
 * are made. For a renderer without a matching "decoder" block (lgaudiosink:
 * only /dev/lg/aren is opened, no ADEC for ES) decoder_block == NULL, so
 * *bypass = false, and only the "Cloud Game PCM Multi Channel Bypass" case
 * (earc_on || wowcast_on, !mix_option, renderer_get_pcm_bypass() true) can
 * set it again. renderer_get_pcm_bypass() is true only for the PCM bypass
 * head (pcm_bypass/pcm_multi_ch), so an ES stream never gets bypass == true
 * from this function.
 */
/* @0xffffffc010cd3010 */
static void _get_output_format (struct alsa_sndout_output_type *output,
    enum output_codec_t *output_codec, bool * bypass)
{
    enum sndout_input_codec input_codec = SNDOUT_INPUT_CODEC_OTHERS;
    enum sndout_input input = SNDOUT_INPUT_OTHERS;

    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int main_audio;
    struct aud_block_info *decoder_block = NULL;
    struct decoded_info decoded_info = { };
    enum audio_decoder_input decoder_input = AUDIO_DECODER_INPUT_NONE;

    main_audio =
        common_input_convert_index (sndout_info->main_audio_input, sndout_info->main_audio_port);
    if (main_audio < 0)
        error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
            sndout_info->main_audio_port);
    else
        decoder_block = bm_get_block_info_by_index ("decoder", main_audio);

    if (decoder_block)
    {
        decoder_get_decoded_info (decoder_block, &decoded_info);
        decoder_input = decoder_get_input (decoder_block);

        input = _decoder_input_convert_input (decoder_input);

        if (input == SNDOUT_INPUT_HDMI)
        {
            struct aud_block_info *input_block;
            enum audio_decoder_codec dec_codec;

            input_block = bm_get_block_info_by_index ("input", main_audio);
            if (input_block)
            {
                dec_codec = input_get_hdmi_codec (input_block);
                input_codec = _hdmi_convert_input_codec (dec_codec);
            }
        }
        else
            input_codec = _decoded_info_convert_input_codec (&decoded_info);

        if (input_codec == SNDOUT_INPUT_CODEC_AAC)
        {
            struct aud_block_info *ren_block = NULL;

            ren_block = bm_get_block_info_by_index ("renderer", main_audio);
            if (ren_block)
            {
                bool bJP4K = false;

                bJP4K = renderer_get_jp4k (ren_block);
                if (bJP4K)
                    input_codec = SNDOUT_INPUT_CODEC_MPEG4_AAC;
            }
        }

        _get_output_format_set_type (output, input_codec, output_codec, bypass);

        // add checking if this decoder is bypass.
        if (*bypass)
        {
            *bypass = _is_decoder_bypass (&decoded_info);
        }

        // earc exceptional case
        // if hdmi and pcm input and bypass, bypass.
        // not earc (arc, optic)
        // if pcm input and bypass, mix.
        if (sndout_info->earc_on && (input == SNDOUT_INPUT_HDMI
                && input_codec == SNDOUT_INPUT_CODEC_OTHERS) && !output->mix_option)
        {
            *output_codec = OUTPUT_CODEC_PCM;
            *bypass = true;
        }

        /* webOS25-shipped: the published "if input media, dts, output pass
         * through, but can not pass through -> *bypass = false" check
         * (SYSTEM input, output DTS/DTSHD/DTSHD_MA/DTSX_P1,
         * !decoded_info.pass_through) is not compiled in. */

        // wowcast case
        if (sndout_info->wowcast_on && (input == SNDOUT_INPUT_HDMI
                && input_codec == SNDOUT_INPUT_CODEC_OTHERS) && !output->mix_option)
        {
            *output_codec = OUTPUT_CODEC_PCM;
            *bypass = true;
        }

        /* webOS25-shipped: the published "input hdmi dts_hd -> earc dts_hd /
         * dts_hd_ma / dtsx_p1 pass thru" and "dts_express, dtsx_p2 -> pcm mix"
         * blocks are not compiled in. */

        // arc/earc jp exceptional case
        // input codec  output_codec    mix_option_type expected_output_codec
        // MPEG2-AAC    SNDOUT_MPEG2-AAC        SNDOUT_MIXOPTION_ALL    MPEG2-AAC
        // MPEG2-AAC    SNDOUT_MPEG4-AAC        SNDOUT_MIXOPTION_ALL    CODEC_PCM
        // MPEG2-AAC    SNDOUT_MPEG2-AAC        SNDOUT_MIXOPTION_NONE   MPEG2-AAC
        // MPEG2-AAC    SNDOUT_MPEG4-AAC        SNDOUT_MIXOPTION_NONE   MPEG2-AAC

        // input codec  output_codec    mix_option_type expected_output_codec
        // MPEG4-AAC    SNDOUT_MPEG2-AAC        SNDOUT_MIXOPTION_ALL    MPEG2-AAC
        // MPEG4-AAC    SNDOUT_MPEG4-AAC        SNDOUT_MIXOPTION_ALL    CODEC_PCM
        // MPEG4-AAC    SNDOUT_MPEG2-AAC        SNDOUT_MIXOPTION_NONE   MPEG2-AAC
        // MPEG4-AAC    SNDOUT_MPEG4-AAC        SNDOUT_MIXOPTION_NONE   MPEG4-AAC
        if ((input_codec == SNDOUT_INPUT_CODEC_AAC)
            && (output->codec_type == SNDOUT_CODEC_MPEG4_AAC) && (output->mix_option))
        {
            *output_codec = OUTPUT_CODEC_PCM;
            *bypass = false;
        }
        else if ((input_codec == SNDOUT_INPUT_CODEC_AAC)
            && (output->codec_type == SNDOUT_CODEC_MPEG4_AAC) && (!output->mix_option))
        {
            *output_codec = OUTPUT_CODEC_AAC;
            *bypass = true;
        }
        else if ((input_codec == SNDOUT_INPUT_CODEC_MPEG4_AAC)
            && (output->codec_type == SNDOUT_CODEC_MPEG4_AAC) && (output->mix_option))
        {
            *output_codec = OUTPUT_CODEC_PCM;
            *bypass = false;
        }
        else if ((input_codec == SNDOUT_INPUT_CODEC_MPEG4_AAC)
            && (output->codec_type == SNDOUT_CODEC_AAC) && (!output->mix_option))
        {
            *output_codec = OUTPUT_CODEC_AAC;
            *bypass = false;
        }

        // optic/arc/earc exceptional case
        // if input codec is heaac, can't pass through.
        if (input_codec == SNDOUT_INPUT_CODEC_HEAAC && !output->mix_option)
        {
            *output_codec = OUTPUT_CODEC_PCM;
            *bypass = false;
        }
    }
    else
    {
        if (output->mix_option)
            _get_output_format_set_type (output, input_codec, output_codec, bypass);
        else
            *output_codec = OUTPUT_CODEC_PCM;

        *bypass = false;
    }

    // earc exceptional case
    // Cloud Game PCM Multi Channel Bypass
    if ((sndout_info->earc_on || sndout_info->wowcast_on) && !output->mix_option)
    {
        struct aud_block_info *ren_block = NULL;

        ren_block = bm_get_block_info_by_index ("renderer", main_audio);
        if (ren_block)
        {
            bool bPcmBypass = false;

            bPcmBypass = renderer_get_pcm_bypass (ren_block);
            if (bPcmBypass)
                *bypass = true;
            debug ("bPcmBypass %d *bypass %d.\n", bPcmBypass, *bypass);
        }
    }

    /* Shipped +0xb8..+0xc8: this final override wins over codec selection
     * and PCM-bypass exceptions. It affects routing, not output->bypass in
     * output.c, since the downstream output_set_format is logging-only. */
    if (notifier_get_play_speed (main_audio) != 1000)
    {
        *output_codec = OUTPUT_CODEC_PCM;
        *bypass = false;
        debug ("play speed is not 1x.\n");
    }

    debug ("output->codec_type %d input_codec %d.\n", output->codec_type, input_codec);
    debug ("output_codec %d bypass %d.\n", *output_codec, *bypass);
    return;
}

// default on,
// when atmos_encode_on off, eac3 encoding, DDP output for Not supported ATMOS AMP.
/* @0xffffffc010cd4340 */
static bool _get_atmos_encode_on (void)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = NULL;
    struct sndout_output_format *set_output = &list_output->set_output;
    bool atmos_encode_on = true;

    if (sndout_info->opened_output & COMMON_ARC)
        list_output = output_get (COMMON_ARC);
    else if (sndout_info->opened_output & COMMON_WOWCAST)
        list_output = output_get (COMMON_WOWCAST);

    if (list_output)
    {
        set_output = &list_output->set_output;

        if (!sndout_info->eac3_atmos_encode_on && list_output->encoder_connected
            && ((set_output->output_codec == OUTPUT_CODEC_EAC3) && !set_output->bypass))
            atmos_encode_on = false;
        else
            atmos_encode_on = true;
    }

    return atmos_encode_on;
}

/* @0xffffffc010cdc404 */
bool sndout_get_atmos_encode_on (void)
{
    bool atmos_encode_on = true;

    SNDOUT_LOCK ();
    atmos_encode_on = _get_atmos_encode_on ();
    SNDOUT_UNLOCK ();

    return atmos_encode_on;
}

/* @0xffffffc010cd43b4 */
static void _set_atmos_encode_on (int on)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int main_audio;
    struct alsa_sndout_list_output *list_output = NULL;
    struct aud_block_info *decoder = NULL;

    main_audio =
        common_input_convert_index (sndout_info->main_audio_input, sndout_info->main_audio_port);
    if ((main_audio >= 0) && (main_audio < ALSA_ADEC_MAX))
    {
        decoder = bm_get_block_info_by_index ("decoder", main_audio);
        if (decoder != NULL)
        {
            if (_get_atmos_encode_on ())
                decoder_set_param (decoder, "legacy-ddp-out=0");
            else
                decoder_set_param (decoder, "legacy-ddp-out=1");
        }
    }
    else
        error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
            sndout_info->main_audio_port);

    if (sndout_info->opened_output & COMMON_ARC)
        list_output = output_get (COMMON_ARC);
    else if (sndout_info->opened_output & COMMON_WOWCAST)
        list_output = output_get (COMMON_WOWCAST);

    if (list_output && list_output->encoder_info)
        aud_encoder_set_atmos_on (list_output->encoder_info, on);
}

/*
 * webOS25-shipped (read instruction by instruction, 0x980 bytes):
 *  - no _set_dtsx_decoder_enc_mode() calls, no MS12_VER >= 26 encoder/SP50
 *    calls, no DTS_HD/DTS_HD_MA/DTSX_P1 8ch/192k case (only EAC3 -> 192k and
 *    MAT -> 8ch/192k).
 *  - decoder_set_output_drop(idx, DROP_*) is replaced by
 *    decoder_set_output_mode(idx, MAIN|BYPASS, on/off) and the new
 *    sndout_info->mixer_connected_output bitmask:
 *      BYPASS: decoder_set_output_mode(main_audio, BYPASS, true) only when
 *              the main input is connected (+0x360..+0x400); the published
 *              "SPK open ? DROP_NONE : DROP_MAIN" selection is gone;
 *      leaving bypass for non-bypass: (main_audio, BYPASS, false) (published:
 *              DROP_SUB on bypass_master_index);
 *      PCM / ENCODING: mixer_connected_output |= output and
 *              (main_audio, MAIN, true);
 *      on every disconnect of the output: mixer_connected_output &= ~output,
 *              when it becomes 0 -> (main_audio, MAIN, false).
 *  - The renderer BYPASS_PORT -> mixer BYPASS_PORT link is still only made
 *    when bypass == true AND the main audio input is connected
 *    (_input_connected_any), exactly as published.
 */
/* @0xffffffc010cd44e0 */
static void _set_output (int main_audio, struct alsa_sndout_list_output *list_output,
    enum output_codec_t output_codec, bool bypass, bool forced_5_1)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct sndout_output_format *set_output = &list_output->set_output;
    unsigned int channel = 2;
    unsigned int sample_rate = 48000;

    // TO-DO : restart시 처리 고려 필요.
    if (list_output->output == COMMON_WOWCAST)
    {

        if (list_output->output_set && (set_output->output_codec == output_codec) &&
            (set_output->bypass == bypass) && (set_output->forced_5_1 == forced_5_1)
            && ((set_output->main_audio == main_audio) || !set_output->bypass) &&
            !(((list_output->output == COMMON_WOWCAST) && sndout_info->wowcast_on)
                && (output_codec == OUTPUT_CODEC_PCM)
                && (set_output->channel != sndout_info->wowcast.channel_num)
                && !set_output->bypass))
        {
            info ("params(%d %d %d %d) to be set are the same as params(%d %d %d %d) set.\n",
                output_codec, bypass, forced_5_1, set_output->channel, set_output->output_codec,
                set_output->bypass, set_output->forced_5_1, sndout_info->wowcast.channel_num);
            return;
        }
    }
    else
    {
        if (list_output->output_set && (set_output->output_codec == output_codec) &&
            (set_output->bypass == bypass) && (set_output->forced_5_1 == forced_5_1)
            && ((set_output->main_audio == main_audio) || !set_output->bypass) &&
            !(((list_output->output == COMMON_ARC) && sndout_info->earc_on)
                && (output_codec == OUTPUT_CODEC_PCM)
                && (set_output->channel != sndout_info->earc.channel_num) && !set_output->bypass))
        {
            info ("params(%d %d %d %d) to be set are the same as params(%d %d %d %d) set.\n",
                output_codec, bypass, forced_5_1, set_output->channel, set_output->output_codec,
                set_output->bypass, set_output->forced_5_1, sndout_info->earc.channel_num);
            return;
        }
    }

    if (list_output->output != COMMON_HP)
    {
        // disconncet bypass renderer - mixer
        if (list_output->bypass_connected)
        {
            mixer_set_mute (sndout_info->mixer_info, BYPASS_PORT, true);
            // from bypass to bypass, not call drop.
            if (!bypass)
            {
                SNDOUT_UNLOCK ();
                decoder_set_output_mode (main_audio, DECODER_OUTPUT_MODE_BYPASS, false);
                SNDOUT_LOCK ();
            }
            if (!list_output)
            {
                info ("list_output was NULL while setting output.\n");
                return;
            }
            bm_disconnect_block (sndout_info->mixer_info, BYPASS_PORT);
            bm_remove_reserved (sndout_info->mixer_info, BYPASS_PORT, false);
            list_output->bypass_connected = false;
            sndout_info->bypass_master_index = -1;
        }

        // disconncet mixer - encoder
        if (list_output->encoder_connected)
        {
            bm_disconnect_block (list_output->encoder_info, 0);
            list_output->encoder_connected = false;
            _set_atmos_encode_on (true);        // default on
        }

        // disconnect mixer - output
        bm_disconnect_block (list_output->output_info, 0);

        sndout_info->mixer_connected_output &= ~list_output->output;
        if (!sndout_info->mixer_connected_output)
            decoder_set_output_mode (main_audio, DECODER_OUTPUT_MODE_MAIN, false);
    }

    if (output_codec == OUTPUT_CODEC_EAC3)
        sample_rate = 192000;
    else if (output_codec == OUTPUT_CODEC_MAT)
    {
        channel = 8;
        sample_rate = 192000;
    }

    // update set_output
    list_output->output_set = true;
    set_output->output_codec = output_codec;
    set_output->bypass = bypass;
    set_output->forced_5_1 = forced_5_1;
    set_output->main_audio = main_audio;
    if (((list_output->output == COMMON_ARC) && sndout_info->earc_on)
        && (output_codec == OUTPUT_CODEC_PCM))
        set_output->channel = sndout_info->earc.channel_num;
    else if (((list_output->output == COMMON_WOWCAST) && sndout_info->wowcast_on)
        && (output_codec == OUTPUT_CODEC_PCM))
        set_output->channel = sndout_info->wowcast.channel_num;
    else
        set_output->channel = 0;


    if (bypass)
    {
        bool mute;
        int delay;

        if ((main_audio >= 0) && (main_audio < ALSA_SNDOUT_INPUT_NUM))
        {
            mute = sndout_info->param_input[main_audio].mute;
            delay = sndout_info->param_input[main_audio].delay;
        }
        else
        {
            mute = sndout_info->param_input[0].mute;
            delay = sndout_info->param_input[0].delay;
        }

        info ("BYPASS\n");

        // connection이 되면 바로 callback 되어 이전 설정으로 mixed ip를 설정되어
        // connection 전에 param을 설정함.
        output_set_format (list_output->output_info, output_codec, channel, sample_rate, true);
        // mixer - ouput
        bm_connect_block (sndout_info->mixer_info, BYPASS_PORT, list_output->output_info, 0);
        list_output->bypass_connected = true;
        if (_input_connected_any (sndout_info->main_audio_input, sndout_info->main_audio_port))
        {
            // renderer - mixer
            bm_connect_block_reserved_out ("renderer", main_audio, BYPASS_PORT,
                sndout_info->mixer_info, BYPASS_PORT);
            sndout_info->bypass_master_index = main_audio;
            mixer_set_delay (sndout_info->mixer_info, BYPASS_PORT, delay);
            mixer_set_mute (sndout_info->mixer_info, BYPASS_PORT, mute);
            SNDOUT_UNLOCK ();
            decoder_set_output_mode (main_audio, DECODER_OUTPUT_MODE_BYPASS, true);
            SNDOUT_LOCK ();
        }
    }
    else if (output_codec == OUTPUT_CODEC_PCM)
    {
        info ("PCM\n");

        output_set_format (list_output->output_info, OUTPUT_CODEC_PCM, 2, 48000, false);
        if (list_output->output != COMMON_HP)
            _connect_pcm (list_output);

        sndout_info->mixer_connected_output |= list_output->output;
        decoder_set_output_mode (main_audio, DECODER_OUTPUT_MODE_MAIN, true);
    }
    else
    {
        info ("ENCODING codec %d\n", output_codec);

        if (list_output->encoder_info == NULL)
        {
            enum output_type_t output_type = common_output_convert_output (list_output->output);

            list_output->encoder_info = bm_create_block ("encoder", output_type);
        }

        if (list_output->encoder_info)
        {
            aud_encoder_set_codec (list_output->encoder_info, output_codec, 8);
            _set_atmos_encode_on (sndout_info->eac3_atmos_encode_on);
            aud_encoder_set_ch_lock_mode (list_output->encoder_info, forced_5_1);
            aud_encoder_set_atmos_locking_mode (list_output->encoder_info,
                sndout_info->atmos_locking);

            output_set_format (list_output->output_info, output_codec, channel, sample_rate, false);

            bm_connect_block (sndout_info->mixer_info, 1, list_output->encoder_info, 0);
            bm_connect_block (list_output->encoder_info, 0, list_output->output_info, 0);
            list_output->encoder_connected = true;
        }

        sndout_info->mixer_connected_output |= list_output->output;
        decoder_set_output_mode (main_audio, DECODER_OUTPUT_MODE_MAIN, true);
    }

    noti ("update params(%d %d %d %d) from params(%d %d %d %d).\n", output_codec, bypass,
        forced_5_1, main_audio, set_output->output_codec, set_output->bypass,
        set_output->forced_5_1, set_output->main_audio);
}

static void _old_optic_connect_output (sndout_optic_mode_ext_type_t optic_output_type)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    enum output_codec_t output_codec;
    bool bypass, forced_5_1;
    struct alsa_sndout_list_output *list_output = NULL;

    _optic_convert_output_codec (optic_output_type, &output_codec, &bypass, &forced_5_1);

    if (sndout_info->opened_output & COMMON_OPTIC)
        list_output = output_get (COMMON_OPTIC);
    else if (sndout_info->opened_output & COMMON_OPTIC_LG)
        list_output = output_get (COMMON_OPTIC_LG);

    if (list_output)
    {
        int main_audio = 0;

        main_audio =
            common_input_convert_index (sndout_info->main_audio_input,
            sndout_info->main_audio_port);
        if (main_audio < 0)
            error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
                sndout_info->main_audio_port);

        if (list_output->output == COMMON_OPTIC_LG)
        {
            output_codec = OUTPUT_CODEC_PCM;
            bypass = false;
        }

        _set_output (main_audio, list_output, output_codec, bypass, forced_5_1);
    }
}

/* @0xffffffc010cd4f64 */
static void _old_arc_connect_output (sndout_arc_mode_ext_type_t arc_output_type)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    enum output_codec_t output_codec;
    bool bypass, forced_5_1;
    struct alsa_sndout_list_output *list_output = NULL;

    sndout_arc_convert_output_codec (arc_output_type, &output_codec, &bypass, &forced_5_1);

    if (sndout_info->opened_output & COMMON_ARC)
        list_output = output_get (COMMON_ARC);
    else if (sndout_info->opened_output & COMMON_SE_ARC)
        list_output = output_get (COMMON_SE_ARC);
    else if (sndout_info->opened_output & COMMON_WOWCAST)
        list_output = output_get (COMMON_WOWCAST);

    if (list_output)
    {
        int main_audio = 0;

        main_audio =
            common_input_convert_index (sndout_info->main_audio_input,
            sndout_info->main_audio_port);
        if (main_audio < 0)
            error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
                sndout_info->main_audio_port);

        if (list_output->output == COMMON_SE_ARC)
        {
            output_codec = OUTPUT_CODEC_PCM;
            bypass = false;
        }

        _set_output (main_audio, list_output, output_codec, bypass, forced_5_1);
    }
}

/* @0xffffffc010cdc444 */
void sndout_set_dolby_ottmode (int ott, int atmos_locking)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = NULL;

    SNDOUT_LOCK ();

    sndout_info->ott = ott;
    sndout_info->atmos_locking = atmos_locking;

    if (sndout_info->opened_output & COMMON_ARC)
    {
        list_output = output_get (COMMON_ARC);
        if (list_output && list_output->encoder_info)
        {
            aud_encoder_set_ch_lock_mode (list_output->encoder_info, ott);
            aud_encoder_set_atmos_locking_mode (list_output->encoder_info, atmos_locking);
#if (MS12_VER >= 26)
            mixer_set_atmos_locking_mode (sndout_info->mixer_info, atmos_locking);
#endif
        }
    }

    if (sndout_info->opened_output & COMMON_WOWCAST)
    {
        list_output = output_get (COMMON_WOWCAST);
        if (list_output && list_output->encoder_info)
        {
            aud_encoder_set_ch_lock_mode (list_output->encoder_info, ott);
            aud_encoder_set_atmos_locking_mode (list_output->encoder_info, atmos_locking);
        }
    }

    if (sndout_info->opened_output & COMMON_OPTIC)
    {
        list_output = output_get (COMMON_OPTIC);
        if (list_output && list_output->encoder_info)
        {
            aud_encoder_set_ch_lock_mode (list_output->encoder_info, ott);
            aud_encoder_set_atmos_locking_mode (list_output->encoder_info, atmos_locking);
#if (MS12_VER >= 26)
            mixer_set_atmos_locking_mode (sndout_info->mixer_info, atmos_locking);
#endif
        }
    }

    SNDOUT_UNLOCK ();
}

/* @0xffffffc010cd5090 */
static void _old_connect_output (struct alsa_sndout_output_type *output,
    sndout_arc_mode_ext_type_t arc_output_type)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    enum output_codec_t output_codec;
    bool bypass = false;
    bool forced_5_1 = false;
    struct alsa_sndout_list_output *list_output = NULL;

    if (output->set_type == SNDOUT_OUTPUT_SET_NONE)
    {
        _old_arc_connect_output (arc_output_type);
        return;
    }

    // ddp locking mode
    if (sndout_info->ott)
        forced_5_1 = true;
    else
        forced_5_1 = false;

    _get_output_format (output, &output_codec, &bypass);

    if (sndout_info->opened_output & COMMON_ARC)
        list_output = output_get (COMMON_ARC);
    else if (sndout_info->opened_output & COMMON_SE_ARC)
        list_output = output_get (COMMON_SE_ARC);
    else if (sndout_info->opened_output & COMMON_WOWCAST)
        list_output = output_get (COMMON_WOWCAST);

    if (list_output)
    {
        int main_audio = 0;

        main_audio =
            common_input_convert_index (sndout_info->main_audio_input,
            sndout_info->main_audio_port);
        if (main_audio < 0)
            error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
                sndout_info->main_audio_port);

        if (list_output->output == COMMON_SE_ARC)
        {
            output_codec = OUTPUT_CODEC_PCM;
            bypass = false;
        }

        _set_output (main_audio, list_output, output_codec, bypass, forced_5_1);
    }
}

/* @0xffffffc010cd4e60 */
static void _connect_output (struct alsa_sndout_list_output *list_output,
    struct alsa_sndout_output_type *output)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    enum output_codec_t output_codec;
    bool bypass, forced_5_1;

    // ddp locking mode
    if (sndout_info->ott)
        forced_5_1 = true;
    else
        forced_5_1 = false;

    _get_output_format (output, &output_codec, &bypass);

    if (list_output)
    {
        int main_audio = 0;

        main_audio =
            common_input_convert_index (sndout_info->main_audio_input,
            sndout_info->main_audio_port);
        if (main_audio < 0)
            error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
                sndout_info->main_audio_port);

        if ((list_output->output == COMMON_OPTIC_LG) || (list_output->output == COMMON_SE_ARC))
        {
            output_codec = OUTPUT_CODEC_PCM;
            bypass = false;
        }

        _set_output (main_audio, list_output, output_codec, bypass, forced_5_1);
    }
}

/* @0xffffffc010cd51f0 */
static void _connect_output_all (void)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    sndout_optic_mode_ext_type_t optic_output_type = sndout_info->optic_output_type;
    sndout_arc_mode_ext_type_t arc_output_type = sndout_info->arc_output_type;
    struct alsa_sndout_list_output *list_output = NULL;

    if (sndout_info->opened_output & COMMON_OPTIC || sndout_info->opened_output & COMMON_OPTIC_LG)
    {
        if (_gNewInterfaceOpticArc)
        {
            if (sndout_info->opened_output & COMMON_OPTIC)
                list_output = output_get (COMMON_OPTIC);
            else if (sndout_info->opened_output & COMMON_OPTIC_LG)
                list_output = output_get (COMMON_OPTIC_LG);

            _connect_output (list_output, &sndout_info->optic);
        }
        else
            _old_optic_connect_output (optic_output_type);
    }

    if (sndout_info->opened_output & COMMON_ARC || sndout_info->opened_output & COMMON_SE_ARC
        || sndout_info->opened_output & COMMON_WOWCAST)
    {
        if (_gNewInterfaceOpticArc)
        {
            if (sndout_info->opened_output & COMMON_ARC)
                list_output = output_get (COMMON_ARC);
            else if (sndout_info->opened_output & COMMON_SE_ARC)
                list_output = output_get (COMMON_SE_ARC);
            else if (sndout_info->opened_output & COMMON_WOWCAST)
                list_output = output_get (COMMON_WOWCAST);

            if (sndout_info->wowcast_on)
                _connect_output (list_output, &sndout_info->wowcast);
            else if (sndout_info->earc_on)
                _connect_output (list_output, &sndout_info->earc);
            else
                _connect_output (list_output, &sndout_info->arc);
        }
        else
        {
            if (sndout_info->opened_output & COMMON_SE_ARC)
                arc_output_type = SNDOUT_ARC_PCM;

            if (sndout_info->wowcast_on)
                _old_connect_output (&sndout_info->wowcast, arc_output_type);
            else if (sndout_info->earc_on)
                _old_connect_output (&sndout_info->earc, arc_output_type);
            else
                _old_arc_connect_output (arc_output_type);
        }
    }
}

/* @0xffffffc010cdc580 */
void sndout_connect_output (void)
{
    SNDOUT_LOCK ();
    _connect_output_all ();
    SNDOUT_UNLOCK ();
}

/* @0xffffffc010ccfe00 */
static int sndout_connect_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 9;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = COMMON_OUTPUT_MAX;
    return 0;
}

static unsigned int adjust_input_ad_gain (int ad_index, int ad_gain)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    unsigned int gain = 0x800000;
    unsigned int input_gain = sndout_info->param_input[ad_index].gain;

    if (input_gain <= GAIN_MIN || ad_gain <= GAIN_MIN)
        gain = GAIN_MIN;
    else
        gain = ((unsigned long long) input_gain * (unsigned long long) ad_gain) >> 23;

    return gain;
}

/* @0xffffffc010cd0340 */
static void _set_input_gain (int index, int gain)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    if (sndout_info->ad_on && index == sndout_info->ad_index)
        gain = adjust_input_ad_gain (index, sndout_info->ad_gain);

    mixer_set_volume (sndout_info->mixer_info, index, gain);
}

/* @0xffffffc010cd6530 */
static int sndout_connect (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    //struct alsa_sndout_list_connect *sndoutConnectInfo = NULL;
    common_output_ext_type_t output = COMMON_NO_OUTPUT;
    common_input_ext_type_t input = COMMON_NO_INPUT;
    int port = 0;

    int master_index = 0;
    int master_output = OUTPUT_MAX;

    int ret = 0;

    debug_call ();

    output = ucontrol->value.integer.value[0];
    input = ucontrol->value.integer.value[1];
    port = ucontrol->value.integer.value[2];

    noti ("Start Connect output 0x%x input 0x%x port %d.\n", output, input, port);

    master_index = common_input_convert_index (input, port);
    master_output = common_output_convert_output (output);
    //if (!_gbDefaultConnectHP)
    //    sndout_default_settings ();

    if (master_output < 0)
    {
        error ("invalid output(0x%x)\n", output);
        return -(EINVAL);
    }
    if (master_index < 0)
    {
        error ("invalid input(input:0x%x, port:%d)\n", input, port);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    // check if master_index or master_output is not open
    if (output_is_added (output) == false)
    {
        error ("can not find output\n");
        ret = -(EACCES);
        goto exit;
    }

    //AUDIO_RestartLock (__func__, __LINE__);

    // if OPTIC LG output is connected first time, then enable sound bar mode
    if (output == COMMON_OPTIC_LG && !_output_connected_any (output))
    {
        struct alsa_sndout_list_output *list_output = output_get (output);

        if (list_output)
            output_set_sound_bar_on (list_output->output_info, true);
    }

    if (master_output <= OUTPUT_OTHERS)
    {
        struct alsa_sndout_list_output *list_output = output_get (output);

        if (_is_output_input_connected (output, input, port))
        {
            error ("already connected (output:0x%x, input:0x%x, port:0x%x).", output, input, port);
            ret = -(EALREADY);
            goto exit;
        }

        if (!_input_connected_any (input, port))
        {
#ifdef SUPPORT_DAFC
            if (input == 0x10 && port == 0x8)   // playback for COMMON_DAFC
                bm_connect_block_reserved_out ("renderer", master_index, 0,
                    list_output->output_info, 0);
            else
#endif
                bm_connect_block_reserved_out ("renderer", master_index, 0, sndout_info->mixer_info,
                    master_index);

            debug ("connect input port%d of mixer", master_index);
        }

        if (list_output->bypass_connected)
        {
            if ((sndout_info->main_audio_input == input) && (sndout_info->main_audio_port == port))
            {
                int main_audio = 0;
                main_audio = common_input_convert_index (input, port);
                if ((main_audio >= 0) && (main_audio <= ALSA_SNDOUT_INPUT_NUM))
                {
                    bool mute;
                    int delay;

                    mute = sndout_info->param_input[main_audio].mute;
                    delay = sndout_info->param_input[main_audio].delay;

                    // renderer - mixer
                    bm_connect_block_reserved_out ("renderer", main_audio, BYPASS_PORT,
                        sndout_info->mixer_info, BYPASS_PORT);
                    sndout_info->bypass_master_index = main_audio;
                    mixer_set_delay (sndout_info->mixer_info, BYPASS_PORT, delay);
                    mixer_set_mute (sndout_info->mixer_info, BYPASS_PORT, mute);
                    SNDOUT_UNLOCK ();
                    /* webOS25-shipped: was DROP_NONE/DROP_MAIN by SPK state */
                    decoder_set_output_mode (main_audio, DECODER_OUTPUT_MODE_BYPASS, true);
                    SNDOUT_LOCK ();

                    debug ("connect input bypass port%d of mixer", master_index);
                }
            }
        }

        if (!_output_connected_any (output))
        {
            /* SPDIF/ARC output */
            if (master_output == OUTPUT_OPTIC || master_output == OUTPUT_ARC)
            {
                _connect_output_all ();
            }
#ifdef SUPPORT_DAFC
            else if (input == 0x10 && port == 0x8)
            {
                // To_DO for speaker out on DAFC mode.
            }
#endif
            /* SPK/HP output */
            else
            {
                if (list_output)
                {
                    if (list_output->output != COMMON_HP)
                        _connect_pcm (list_output);
                }
            }
        }
    }
    else
    {
        error (" wrong output 0x%x (mst_output=%d), input 0x%x \n", output, master_output, input);
        ret = -(EINVAL);
        goto exit;
    }

    // add connection info into list
    ret = _add_connection (output, input, port);
    if (ret < 0)
    {
        error ("error in _add_connection");
        goto exit;
    }

    _set_input_gain (master_index, sndout_info->param_input[master_index].gain);
    mixer_set_mute (sndout_info->mixer_info, master_index,
        sndout_info->param_input[master_index].mute);

    /* webOS25-shipped: the published DTS enc-mode block ("main_audio: %d,
     * codec: %d") is not built. */

    noti ("output 0x%x input 0x%x port %d .\n", output, input, port);

  exit:
    SNDOUT_UNLOCK ();

    notifier_output_changed ();

    return ret;
}

/* @0xffffffc010cd0880 */
static int sndout_connect_get (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_connect *sndoutConnectInfo = NULL;
    int index = 1;

    list_for_each_entry_reverse (sndoutConnectInfo, &sndout_info->list_connection, list)
    {
        if (sndoutConnectInfo->output != COMMON_NO_OUTPUT
            && sndoutConnectInfo->input != COMMON_NO_INPUT)
        {
            ucontrol->value.integer.value[0] |= sndoutConnectInfo->output;
            ucontrol->value.integer.value[index++] = sndoutConnectInfo->input;
            ucontrol->value.integer.value[index++] = sndoutConnectInfo->port;
            info ("output 0x%x input 0x%x port %d .\n", sndoutConnectInfo->output,
                sndoutConnectInfo->input, sndoutConnectInfo->port);
        }
    }

    return 0;
}


static struct alsa_sndout_list_connect gPreSndoutListDisconnect;

/* @0xffffffc010ccfe20 */
static int sndout_disconnect_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 3;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = COMMON_OUTPUT_MAX;
    return 0;
}

/* @0xffffffc010cd1240 */
static int sndout_disconnect_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ucontrol->value.integer.value[0] = gPreSndoutListDisconnect.output;
    ucontrol->value.integer.value[1] = gPreSndoutListDisconnect.input;
    ucontrol->value.integer.value[2] = gPreSndoutListDisconnect.port;
    info ("output 0x%lx input 0x%lx port %ld.\n", ucontrol->value.integer.value[0],
        ucontrol->value.integer.value[1], ucontrol->value.integer.value[2]);

    return 0;
}

/* @0xffffffc010cd5e54 */
static int sndout_disconnect (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    common_output_ext_type_t output = COMMON_NO_OUTPUT;
    common_input_ext_type_t input = COMMON_NO_INPUT;
    int port = 0;

    int master_output = OUTPUT_MAX;
    int master_index = 0;

    struct alsa_sndout_list_output *list_output;

    int ret = 0;

    debug_call ();

    //LX_AUD_MASTER_SOUNDBAR_T soundbarparam = { FALSE, FALSE, FALSE, FALSE, FALSE };

    output = ucontrol->value.integer.value[0];
    input = ucontrol->value.integer.value[1];
    port = ucontrol->value.integer.value[2];

    noti ("Start Disconnect output 0x%x input 0x%x port %d .\n", output, input, port);

    master_output = common_output_convert_output (output);
    if (master_output < 0)
    {
        error ("invalid output(0x%x)\n", output);
        return -(EINVAL);
    }

    master_index = common_input_convert_index (input, port);
    if (master_index < 0)
    {
        error ("invalid input(input:0x%x, port:%d)\n", input, port);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    // check if master_index or master_output is not open
    if (output_is_added (output) == false)
    {
        error ("can not find output\n");
        ret = -(EACCES);
        goto exit;
    }

    ret = _del_connection (output, input, port);
    if (ret < 0)
    {
        error ("can not find sndoutConnectInfo (output:0x%x, input:0x%x, port:0x%x).", output,
            input, port);
        goto exit;
    }

    list_output = output_get (output);

    //AUDIO_RestartLock (__func__, __LINE__);
    if (!_input_connected_any (input, port))
    {
        // there is no input anymore..
        mixer_set_mute (sndout_info->mixer_info, master_index, true);

        // disconnect renderer - mixer
        bm_disconnect_block (sndout_info->mixer_info, master_index);
        bm_remove_reserved (sndout_info->mixer_info, master_index, false);
    }

    if (list_output)
    {
        // disconncet bypass renderer - mixer
        if (list_output->bypass_connected)
        {
            if (sndout_info->bypass_master_index == master_index)
            {
                mixer_set_mute (sndout_info->mixer_info, BYPASS_PORT, true);

                SNDOUT_UNLOCK ();
                /* webOS25-shipped: was DROP_SUB */
                decoder_set_output_mode (master_index, DECODER_OUTPUT_MODE_BYPASS, false);
                SNDOUT_LOCK ();
                if (!list_output)
                {
                    info ("list_output was NULL while setting output.\n");
                    goto exit;
                }
                bm_disconnect_block (sndout_info->mixer_info, BYPASS_PORT);
                bm_remove_reserved (sndout_info->mixer_info, BYPASS_PORT, false);
                sndout_info->bypass_master_index = -1;
            }
        }
    }

    if (output != COMMON_HP)
    {
        // if OPTIC LG or BLUETOOTH or WISA or SE BT or CAPTURE output is not connected with any input,
        // then disable sound bar mode
        if (!_output_connected_any (output) && (output == COMMON_OPTIC_LG))
        {
            if (list_output)
                output_set_sound_bar_on (list_output->output_info, false);
        }

        if (!_output_connected_any (output) && master_output < OUTPUT_MAX)
        {
            //int mainoutput = 0;
            // disconnect mixer-output
            if (list_output)
            {
                if (list_output->output_info)
                    bm_disconnect_block (list_output->output_info, 0);

                if (list_output->alsa_se)
                {
                    struct aud_block_info *block_info = alsa_se_get_block (list_output->alsa_se);

                    bm_disconnect_block (block_info, 0);
                    bm_disconnect_block (block_info, 1);
                }

                if (list_output->encoder_connected)
                {
#if (MS12_VER >= 26)
                    mixer_set_encoder_connection (sndout_info->mixer_info, false, false);
                    if ((ADEC_MS12_MAJOR_VER > 2)
                        || ((ADEC_MS12_MAJOR_VER == 2) && (ADEC_MS12_MINOR_VER >= 6)))
                        mixer_set_sp50 (sndout_info->mixer_info, false);
#endif
                    bm_disconnect_block (list_output->encoder_info, 0);
                    list_output->encoder_connected = false;
                    _set_atmos_encode_on (true);        // default on
                }
            }
        }
    }

    // disconnect시 output_set을 초기화함.
    // if bypass, bypass_master_index,
    // or all input disconnected, mix,
    if (list_output)
    {
        if ((list_output->bypass_connected && (sndout_info->bypass_master_index == master_index))
            || !_output_connected_any (output))
            list_output->output_set = false;
    }

    gPreSndoutListDisconnect.output = output;
    gPreSndoutListDisconnect.input = input;
    gPreSndoutListDisconnect.port = port;

    /* webOS25-shipped: the published DTS enc-mode block ("main_audio: %d,
     * codec: %d") is not built. */

    noti ("output 0x%x input 0x%x port %d .\n", output, input, port);

  exit:
    SNDOUT_UNLOCK ();
    return ret;
}


/* @0xffffffc010ccfe40 */
static int sndout_mainaudio_output_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 2;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_MAX;
    return 0;
}

/* @0xffffffc010cd0f30 */
static int sndout_mainaudio_output_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->main_audio_input;
    ucontrol->value.integer.value[1] = sndout_info->main_audio_port;

    info ("main audio input %d portnum %d .\n", sndout_info->main_audio_input,
        sndout_info->main_audio_port);
    return 0;
}

/* @0xffffffc010cda1e0 */
static int sndout_mainaudio_output (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    int master_index = 0;
    common_input_ext_type_t input = COMMON_NO_INPUT;
    int port = 0;
    struct alsa_sndout_list_output *list_output = NULL;

    debug_call ();

    input = ucontrol->value.integer.value[0];
    port = ucontrol->value.integer.value[1];

    noti ("Start main audio input %d, port %d master_index %d.\n", input, port, master_index);

    master_index = common_input_convert_index (input, port);
    if (master_index < 0)
    {
        error ("invalid master_index %d\n", master_index);
        return -1;
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->main_audio_input = input;
    sndout_info->main_audio_port = port;

    mixer_set_main_audio_index (sndout_info->mixer_info, master_index);
    SNDOUT_UNLOCK ();
    decoder_set_main_audio_index (master_index);
    SNDOUT_LOCK ();

    //Adjust a Audio Guidance mixer gain
    //sndout_adjust_audio_guidance_gain (master_index);
    _connect_output_all ();

    list_for_each_entry (list_output, &sndout_info->list_output, list)
    {
        if (list_output->output_info)
            output_set_input_index (list_output->output_info, master_index);
    }

    SNDOUT_UNLOCK ();

    noti ("main audio input %d, port %d master_index %d.\n", input, port, master_index);
    return 0;
}

/* @0xffffffc010cd05c4 */
static int dolby_ms12_iidk_version_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 2;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_MAX;
    return 0;
}

/* @0xffffffc010cd0ea0 */
static int dolby_ms12_iidk_version_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ucontrol->value.integer.value[0] = ADEC_MS12_MAJOR_VER;
    ucontrol->value.integer.value[1] = ADEC_MS12_MINOR_VER;

    noti ("ms12 version %lx.%lx.\n", ucontrol->value.integer.value[0],
        ucontrol->value.integer.value[1]);
    return 0;
}

/* @0xffffffc010ccfe60 */
static int sndout_gain_output_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = ALSA_SNDOUT_OUTPUT_MAX;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_GAIN_MAX;
    return 0;
}

/* @0xffffffc010ccfe80 */
static int sndout_gain_output_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int i;

    for (i = 0; i < ALSA_SNDOUT_OUTPUT_MAX; i++)
        ucontrol->value.integer.value[i] = sndout_info->param_output[i].gain;

    return 0;
}


/* @0xffffffc010cd3580 */
static void _adjust_output_gain (struct alsa_sndout_list_output *list_output, unsigned int gain,
    enum output_codec_t output_codec, bool bypass)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int main_audio = 0;

    main_audio =
        common_input_convert_index (sndout_info->main_audio_input, sndout_info->main_audio_port);
    if (main_audio < 0)
        error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
            sndout_info->main_audio_port);

    // Input : DTV, DRC Mode : RF, SPDIF/ARC/E-ARC : PCM
    if ((main_audio >= 0) && (main_audio < ALSA_ADEC_MAX))
    {
        struct aud_block_info *dec = bm_get_block_info_by_index ("decoder", main_audio);
        enum audio_decoder_drc drc = AUDIO_DECODER_DRC_LINE;

        if (dec)
            drc = decoder_get_dolby_drc_mode (dec);

        if ((alsa_decoder_info[main_audio].input == ADEC_SRC_ATP0
                || alsa_decoder_info[main_audio].input == ADEC_SRC_ATP1)
            && drc == AUDIO_DECODER_DRC_RF && (output_codec == OUTPUT_CODEC_PCM && !bypass))
        {
            if (gain < GAIN_MIN)
                gain = GAIN_MIN;
            else
                gain = ((unsigned long long) gain * (unsigned long long) GAIN_MINUS_8DB) >> 23;

            info ("SPDIF/ARC PCM Gain -8dB Adjust : output 0x%x gain 0x%06x\n", list_output->output,
                gain);
        }
    }

    output_set_volume (list_output->output_info, gain);

    return;
}

/* @0xffffffc010cdaf40 */
static int sndout_gain_output (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    common_output_ext_type_t output = COMMON_NO_OUTPUT;
    int master_output = OUTPUT_MAX;
    ALSA_SNDOUT_OUTPUT_TYPE_T sndout_output = ALSA_SNDOUT_OUTPUT_MAX;
    int gain = 0;
    int ret = 0;

    debug_call ();

    output = ucontrol->value.integer.value[0];
    gain = ucontrol->value.integer.value[1];

    if (gain > ALSA_INTEGER_GAIN_MAX)
    {
        error ("invalid parameter: gain(%d)\n", gain);
        return -(EINVAL);
    }

    sndout_output = common_output_convert_sndout_output (output);
    if (sndout_output >= ALSA_SNDOUT_OUTPUT_MAX)
    {
        error (" wrong sndout_output 0x%x \n", output);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->param_output[sndout_output].gain = gain;

    if (output == COMMON_OPTIC_LG)
    {
        // if not opened then no action, else set gain of SPDIF
        if (!(sndout_info->opened_output & COMMON_OPTIC_LG))
        {
            error ("OPTIC LG is not opened(gain=0x%X)\n", gain);
            goto exit;
        }
    }
    else if (output == COMMON_BLUETOOTH || output == COMMON_WISA || output == COMMON_CAPTURE
        || output == COMMON_SE_BT || output == COMMON_SE_BT_SUR || output == COMMON_DAP_HP_BT
#ifdef SUPPORT_DAFC
        || output == COMMON_DAFC
#endif
        )
    {
        extern int acapture_set_gain (common_output_ext_type_t output, int gain);
        SNDOUT_UNLOCK ();
        acapture_set_gain (output, gain);
        SNDOUT_LOCK ();
        info ("output 0x%x gain 0x%x .\n", output, gain);
        goto exit;
    }

    master_output = common_output_convert_output (output);
    if (master_output < OUTPUT_MAX)
    {
        struct alsa_sndout_list_output *list_output = output_get (output);
        if (list_output)
        {
            // Adjust SPDIF/ARC Output PCM Volume for for DVB EBU r128 Spec.
            if ((output == COMMON_OPTIC) || (output == COMMON_ARC) || (output == COMMON_WOWCAST))
                _adjust_output_gain (list_output, gain, OUTPUT_CODEC_PCM, false);
            else
                output_set_volume (list_output->output_info, gain);
        }
    }
    else
    {
        ret = -(EINVAL);
        error ("wrong master_output 0x%x \n", output);
        goto exit;
    }

    noti ("output 0x%x gain 0x%x .\n", output, gain);

  exit:
    SNDOUT_UNLOCK ();

    return ret;
}

/* @0xffffffc010ccfeb0 */
static int sndout_gain_duration_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = ALSA_SNDOUT_INPUT_NUM * 3;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_GAIN_MAX;
    return 0;
}

/* @0xffffffc010cd0500 */
static int sndout_gain_duration_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int i = 0;

    for (i = 0; i < ALSA_SNDOUT_INPUT_NUM; i++)
    {
        unsigned int gain = 0;
        if (i < ALSA_SNDOUT_DEC_NUM)
            ucontrol->value.integer.value[i * 3] = i;
        else
            ucontrol->value.integer.value[i * 3] = i - ALSA_SNDOUT_DEC_NUM;

        mixer_get_volume (sndout_info->mixer_info, i, &gain);
        ucontrol->value.integer.value[i * 3 + 1] = gain;
        ucontrol->value.integer.value[i * 3 + 2] = gAlsaSndoutGainDurationInfo[i].duration;
    }

    return 0;
}

/* @0xffffffc010cdb510 */
static int sndout_gain_duration (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int input = 0, port = 0, gain = 0, duration = 0;
    int master_index = 0;

    debug_call ();

    input = ucontrol->value.integer.value[0];
    port = ucontrol->value.integer.value[1];
    gain = ucontrol->value.integer.value[2];
    duration = ucontrol->value.integer.value[3];

    if (gain > ALSA_INTEGER_GAIN_MAX)
    {
        error ("invalid parameter: gain(%d)\n", gain);
        return -(EINVAL);
    }

    master_index = common_input_convert_index (input, port);
    if (master_index < 0 || master_index >= ALSA_SNDOUT_INPUT_NUM)
    {
        error ("wrong input 0x%x, port %d \n", input, port);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    gAlsaSndoutGainDurationInfo[master_index].gain = gain;
    gAlsaSndoutGainDurationInfo[master_index].duration = duration;

    mixer_set_ease_volume (sndout_info->mixer_info, master_index, gain, duration,
        MIXER_EASE_LINEAR);

    SNDOUT_UNLOCK ();

    noti ("input 0x%x Port %d Gain %d Duration %d.\n", input, port, gain, duration);
    return 0;
}

/* @0xffffffc010ccfed0 */
static int sndout_gain_input_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = ALSA_SNDOUT_INPUT_NUM * 2;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_GAIN_MAX;
    return 0;
}

/* @0xffffffc010ccfef0 */
static int sndout_gain_input_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int i = 0;

    for (i = 0; i < ALSA_SNDOUT_INPUT_NUM; i++)
    {
        if (i < ALSA_SNDOUT_DEC_NUM)
            ucontrol->value.integer.value[i * 2] = i;
        else
            ucontrol->value.integer.value[i * 2] = i - ALSA_SNDOUT_DEC_NUM;

        ucontrol->value.integer.value[i * 2 + 1] = sndout_info->param_input[i].gain;
    }

    return 0;
}

/* @0xffffffc010cdb300 */
static int sndout_gain_input (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int input = 0, port = 0, gain = 0;
    int master_index = 0;

    debug_call ();

    input = ucontrol->value.integer.value[0];
    port = ucontrol->value.integer.value[1];
    gain = ucontrol->value.integer.value[2];

    if (gain > ALSA_INTEGER_GAIN_MAX)
    {
        error ("invalid parameter: gain(%d)\n", gain);
        return -(EINVAL);
    }

    master_index = common_input_convert_index (input, port);
    if (master_index < 0 || master_index >= ALSA_SNDOUT_INPUT_NUM)
    {
        error ("wrong input 0x%x, port %d \n", input, port);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->param_input[master_index].gain = gain;

    _set_input_gain (master_index, gain);

    SNDOUT_UNLOCK ();

    noti ("input 0x%x port %d gain %d.\n", input, port, gain);
    return 0;
}

/* @0xffffffc010ccff50 */
static int sndout_mute_input_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = ALSA_SNDOUT_INPUT_NUM * 2;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010ccff70 */
static int sndout_mute_input_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int i = 0;

    for (i = 0; i < ALSA_SNDOUT_INPUT_NUM; i++)
    {
        if (i < ALSA_SNDOUT_DEC_NUM)
            ucontrol->value.integer.value[i * 2] = i;
        else
            ucontrol->value.integer.value[i * 2] = i - ALSA_SNDOUT_DEC_NUM;

        ucontrol->value.integer.value[i * 2 + 1] = sndout_info->param_input[i].mute;
    }

    return 0;
}

/* @0xffffffc010cdab44 */
static int sndout_mute_input (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int input = 0, port = 0, mute = 0;
    int master_index;

    debug_call ();

    input = ucontrol->value.integer.value[0];
    port = ucontrol->value.integer.value[1];
    mute = ucontrol->value.integer.value[2];

    if (mute > 1 || mute < 0)
    {
        error ("invalid parameter: mute(%d)\n", mute);
        return -(EINVAL);
    }

    master_index = common_input_convert_index (input, port);
    if (master_index < 0 || master_index >= ALSA_SNDOUT_INPUT_NUM)
    {
        error (" wrong input 0x%x, port %d \n", input, port);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->param_input[master_index].mute = mute;

    mixer_set_mute (sndout_info->mixer_info, master_index, mute);
    if (master_index == sndout_info->bypass_master_index)
        mixer_set_mute (sndout_info->mixer_info, BYPASS_PORT, mute);

    SNDOUT_UNLOCK ();

    noti ("input 0x%x port %d mute %d.\n", input, port, mute);
    return 0;
}

/* @0xffffffc010ccffd0 */
static int sndout_mute_output_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = ALSA_SNDOUT_OUTPUT_MAX;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010ccfff0 */
static int sndout_mute_output_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int i;

    for (i = 0; i < ALSA_SNDOUT_OUTPUT_MAX; i++)
        ucontrol->value.integer.value[i] = sndout_info->param_output[i].mute;

    return 0;
}

/* @0xffffffc010cda7b4 */
static int sndout_mute_output (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    int output = 0, mute = 0;
    int master_output = OUTPUT_MAX;
    ALSA_SNDOUT_OUTPUT_TYPE_T sndout_output = ALSA_SNDOUT_OUTPUT_MAX;
    int ret = 0;

    debug_call ();

    output = ucontrol->value.integer.value[0];
    mute = ucontrol->value.integer.value[1];

    if (mute > 1 || mute < 0)
    {
        error ("invalid parameter: mute(%d)\n", mute);
        return -(EINVAL);
    }

    sndout_output = common_output_convert_sndout_output (output);
    if (sndout_output >= ALSA_SNDOUT_OUTPUT_MAX)
    {
        error (" wrong sndout_output 0x%x \n", output);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->param_output[sndout_output].mute = mute;

    if (output == COMMON_OPTIC_LG)
    {
        // if not opened then no action, else set mute of SPDIF
        if (!(sndout_info->opened_output & COMMON_OPTIC_LG))
        {
            error ("OPTIC LG is not opened(mute=%d)\n", mute);
            goto exit;
        }
    }
    else if (output == COMMON_BLUETOOTH || output == COMMON_WISA || output == COMMON_CAPTURE
        || output == COMMON_SE_BT || output == COMMON_SE_BT_SUR || output == COMMON_DAP_HP_BT
#ifdef SUPPORT_DAFC
        || output == COMMON_DAFC
#endif
        )
    {
        extern int acapture_set_mute (common_output_ext_type_t output, int mute);
        SNDOUT_UNLOCK ();
        acapture_set_mute (output, mute);
        SNDOUT_LOCK ();
        noti ("output 0x%x mute %d.\n", output, mute);
        goto exit;
    }

    master_output = common_output_convert_output (output);
    if (master_output < OUTPUT_MAX)
    {
        struct alsa_sndout_list_output *list_output = output_get (output);
        if (list_output)
            output_set_mute (list_output->output_info, mute);
    }
    else
    {
        error ("wrong master_output 0x%x \n", output);
        ret = -(EINVAL);
        goto exit;
    }

    noti ("output 0x%x mute %d.\n", output, mute);

  exit:
    SNDOUT_UNLOCK ();
    return ret;
}

/* @0xffffffc010cd0020 */
static int sndout_delay_inoutput_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = ALSA_SNDOUT_INPUT_NUM * 2 + ALSA_SNDOUT_OUTPUT_MAX;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd0040 */
static int sndout_delay_inoutput_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int i = 0;

    for (i = 0; i < ALSA_SNDOUT_INPUT_NUM; i++)
    {
        if (i < ALSA_SNDOUT_DEC_NUM)
            ucontrol->value.integer.value[i * 2] = i;
        else
            ucontrol->value.integer.value[i * 2] = i - ALSA_SNDOUT_DEC_NUM;

        ucontrol->value.integer.value[i * 2 + 1] = sndout_info->param_input[i].delay;
    }

    for (i = 0; i < ALSA_SNDOUT_OUTPUT_MAX; i++)
        ucontrol->value.integer.value[ALSA_SNDOUT_INPUT_NUM * 2 + i] =
            sndout_info->param_output[i].delay;

    return 0;
}

/* @0xffffffc010cda414 */
static int sndout_delay_inoutput (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    common_input_ext_type_t input = COMMON_NO_INPUT;
    int port = 0;
    int input_delay = 0;
    int mixer_index = 0;
    common_output_ext_type_t output = COMMON_NO_OUTPUT;
    int output_delay = 0;

    struct alsa_sndout_list_output *list_output = output_get (output);
    ALSA_SNDOUT_OUTPUT_TYPE_T sndout_output = ALSA_SNDOUT_OUTPUT_MAX;
    int ret = 0;

    debug_call ();

    input = ucontrol->value.integer.value[0];
    port = ucontrol->value.integer.value[1];
    input_delay = ucontrol->value.integer.value[2];
    output = ucontrol->value.integer.value[3];
    output_delay = ucontrol->value.integer.value[4];

    if (input_delay < ALSA_DELAY_MIN || input_delay > ALSA_INPUT_DELAY_MAX)
    {
        error ("invalid parameter: input_delay(%d)\n", input_delay);
        return -(EINVAL);
    }

    // unit : ms
    if (output_delay < ALSA_DELAY_MIN || output_delay > ALSA_OUTPUT_DELAY_MAX)
    {
        error ("invalid parameter: output_delay(%d)\n", output_delay);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    mixer_index = common_input_convert_index (input, port);
    if ((mixer_index < 0) || (mixer_index >= ALSA_SNDOUT_INPUT_NUM))
    {
        error (" wrong input 0x%x, port %d \n", input, port);
    }
    else
    {
        int main_audio;

        sndout_info->param_input[mixer_index].delay = input_delay;
        mixer_set_delay (sndout_info->mixer_info, mixer_index, input_delay);

        main_audio =
            common_input_convert_index (sndout_info->main_audio_input,
            sndout_info->main_audio_port);
        if (mixer_index == sndout_info->bypass_master_index)
            mixer_set_delay (sndout_info->mixer_info, BYPASS_PORT, input_delay);
    }

    sndout_output = common_output_convert_sndout_output (output);
    if (sndout_output < ALSA_SNDOUT_OUTPUT_MAX)
    {
        sndout_info->param_output[sndout_output].delay = output_delay;

        // if not opened then no action, else set mute of SPDIF
        if (!(sndout_info->opened_output & output))
        {
            noti ("0x%x is not opened(delay=%d)\n", output, output_delay);
            goto exit;
        }

        list_output = output_get (output);
        if (list_output && list_output->output_info)
            output_set_delay (list_output->output_info, output_delay);
    }

    noti ("port %d input  0x%x delay %d.\n", port, input, input_delay);
    noti ("output 0x%x delay %d.\n", output, output_delay);

  exit:
    SNDOUT_UNLOCK ();
    return ret;
}

/* @0xffffffc010cd00c0 */
static int sndout_spdif_output_type_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 2;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd0e10 */
static int sndout_spdif_output_type_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    sndout_optic_mode_ext_type_t optic_output_type = 0;
    sndout_arc_mode_ext_type_t arc_output_type = 0;

    optic_output_type = sndout_info->optic_output_type;
    arc_output_type = sndout_info->arc_output_type;

    ucontrol->value.integer.value[0] = optic_output_type;
    ucontrol->value.integer.value[1] = arc_output_type;

    info ("optic_output_type %d arc_output_type %d.\n", optic_output_type, arc_output_type);
    return 0;
}

/* @0xffffffc010cd9e64 */
static int sndout_spdif_output_type (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    sndout_optic_mode_ext_type_t optic_output_type = 0;
    sndout_optic_mode_ext_type_t pre_optic_output_type = 0;
    sndout_arc_mode_ext_type_t arc_output_type = 0;
    sndout_arc_mode_ext_type_t pre_arc_output_type = 0;
    int main_audio;
    struct alsa_sndout_list_output *list_output = NULL;
    enum output_codec_t output_codec;
    bool bypass = false;
    bool forced_5_1 = false;
    ALSA_SNDOUT_OUTPUT_TYPE_T sndout_output = ALSA_SNDOUT_OUTPUT_MAX;
    unsigned int gain;

    _gNewInterfaceOpticArc = false;

    optic_output_type = ucontrol->value.integer.value[0];
    arc_output_type = ucontrol->value.integer.value[1];

    noti ("Start optic_output_type %d arc_output_type %d.\n", optic_output_type, arc_output_type);

    // validation check
    if (optic_output_type > SNDOUT_OPTIC_BYPASS_AAC)
    {
        error ("optic_output_type %d is invalid.\n", optic_output_type);
        return -(EINVAL);
    }

    if (arc_output_type > SNDOUT_ARC_BYPASS_EAC3_MPEG4_AAC)
    {
        error ("arc_output_type %d is invalid.\n", arc_output_type);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    pre_optic_output_type = sndout_info->optic_output_type;
    pre_arc_output_type = sndout_info->arc_output_type;

    sndout_info->optic_output_type = optic_output_type;
    sndout_info->arc_output_type = arc_output_type;

    main_audio =
        common_input_convert_index (sndout_info->main_audio_input, sndout_info->main_audio_port);
    if (main_audio < 0)
        error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
            sndout_info->main_audio_port);

    if ((pre_optic_output_type != optic_output_type) && (sndout_info->opened_output & COMMON_OPTIC))
    {
        list_output = output_get (COMMON_OPTIC);
        _optic_convert_output_codec (optic_output_type, &output_codec, &bypass, &forced_5_1);
        if (list_output)
        {
            _set_output (main_audio, list_output, output_codec, bypass, forced_5_1);

            sndout_output = common_output_convert_sndout_output (COMMON_OPTIC);
            gain = sndout_info->param_output[sndout_output].gain;
            _adjust_output_gain (list_output, gain, output_codec, bypass);
        }
    }

    if ((pre_arc_output_type != arc_output_type) && (sndout_info->opened_output & COMMON_ARC))
    {
        list_output = output_get (COMMON_ARC);
        sndout_arc_convert_output_codec (arc_output_type, &output_codec, &bypass, &forced_5_1);
        if (list_output)
        {
            if (!sndout_info->earc_on)
            {
                _set_output (main_audio, list_output, output_codec, bypass, forced_5_1);

                sndout_output = common_output_convert_sndout_output (COMMON_ARC);
                gain = sndout_info->param_output[sndout_output].gain;
                _adjust_output_gain (list_output, gain, output_codec, bypass);
            }
        }
    }

    SNDOUT_UNLOCK ();

    notifier_output_changed ();

    noti ("optic_output_type %d arc_output_type %d.\n", optic_output_type, arc_output_type);
    return 0;
}

/* @0xffffffc010cd05e0 */
static int sndout_spdif_category_code_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 2;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd0fc0 */
static int sndout_spdif_category_code_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->optic_category_code;
    ucontrol->value.integer.value[1] = sndout_info->arc_category_code;

    info ("optic_category_code %d arc_category_code %d.\n", sndout_info->optic_category_code,
        sndout_info->arc_category_code);
    return 0;
}

/* @0xffffffc010cd9cd0 */
static int sndout_spdif_category_code (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *spdif_list_output = NULL;
    struct alsa_sndout_list_output *arc_list_output = NULL;
    int optic_category_code = 0;
    int arc_category_code = 0;

    optic_category_code = ucontrol->value.integer.value[0];
    arc_category_code = ucontrol->value.integer.value[1];

    if (optic_category_code < 0 || arc_category_code < 0)
    {
        error ("invalid parameter: optic_category_code(%d), arc_category_code(%d)\n",
            optic_category_code, arc_category_code);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->optic_category_code = optic_category_code;
    sndout_info->arc_category_code = arc_category_code;

    if (sndout_info->opened_output & COMMON_OPTIC)
        spdif_list_output = output_get (COMMON_OPTIC);
    else if (sndout_info->opened_output & COMMON_OPTIC_LG)
        spdif_list_output = output_get (COMMON_OPTIC_LG);

    if (spdif_list_output)
        output_set_category_code (spdif_list_output->output_info, sndout_info->optic_category_code);

    if (sndout_info->opened_output & COMMON_ARC)
        arc_list_output = output_get (COMMON_ARC);
    else if (sndout_info->opened_output & COMMON_SE_ARC)
        arc_list_output = output_get (COMMON_SE_ARC);
    else if (sndout_info->opened_output & COMMON_WOWCAST)
        arc_list_output = output_get (COMMON_WOWCAST);

    if (arc_list_output)
        output_set_category_code (arc_list_output->output_info, sndout_info->arc_category_code);

    SNDOUT_UNLOCK ();

    noti ("optic_category_code %d arc_category_code %d.\n", sndout_info->optic_category_code,
        sndout_info->arc_category_code);
    return 0;
}

/* @0xffffffc010cd0600 */
static int sndout_spdif_copy_protection_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 2;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd1050 */
static int sndout_spdif_copy_protection_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->optic_copy_protection;
    ucontrol->value.integer.value[1] = sndout_info->arc_copy_protection;

    info ("optic_copy_protection %d arc_copy_protection %d.\n", sndout_info->optic_copy_protection,
        sndout_info->arc_copy_protection);
    return 0;
}

/* @0xffffffc010cdb950 */
static int sndout_spdif_copy_protection (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *spdif_list_output = NULL;
    struct alsa_sndout_list_output *arc_list_output = NULL;
    int optic_scms;
    int arc_scms;
    sndout_optic_copyprotection_ext_type_t optic_copy_protection = 0;
    sndout_arc_copyprotection_ext_type_t arc_copy_protection = 0;

    optic_copy_protection = ucontrol->value.integer.value[0];
    arc_copy_protection = ucontrol->value.integer.value[1];

    optic_scms = copy_protection_convert_scms (optic_copy_protection);
    if (optic_scms < 0)
    {
        error ("invalid optic copy protection(%d)\n", optic_copy_protection);
        return -(EINVAL);
    }

    arc_scms = copy_protection_convert_scms (arc_copy_protection);
    if (arc_scms < 0)
    {
        error ("invalid arc copy protection(%d)\n", arc_copy_protection);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->optic_copy_protection = optic_copy_protection;
    sndout_info->arc_copy_protection = arc_copy_protection;

    if (sndout_info->opened_output & COMMON_OPTIC)
        spdif_list_output = output_get (COMMON_OPTIC);
    else if (sndout_info->opened_output & COMMON_OPTIC_LG)
        spdif_list_output = output_get (COMMON_OPTIC_LG);

    if (spdif_list_output)
        output_set_scms (spdif_list_output->output_info, optic_scms);

    if (sndout_info->opened_output & COMMON_ARC)
        arc_list_output = output_get (COMMON_ARC);
    else if (sndout_info->opened_output & COMMON_SE_ARC)
        arc_list_output = output_get (COMMON_SE_ARC);
    else if (sndout_info->opened_output & COMMON_WOWCAST)
        arc_list_output = output_get (COMMON_WOWCAST);

    if (arc_list_output)
        output_set_scms (arc_list_output->output_info, arc_scms);

    SNDOUT_UNLOCK ();

    noti ("optic_copy_protection %d arc_copy_protection %d.\n", optic_copy_protection,
        arc_copy_protection);

    return 0;
}

/* @0xffffffc010cd00e0 */
static int sndout_optic_light_onoff_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 1;
    return 0;
}

/* @0xffffffc010cd0b90 */
static int sndout_optic_light_onoff_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->optic_light_on;

    info ("light_onoff %d .\n", sndout_info->optic_light_on);
    return 0;
}

/* @0xffffffc010cd9b10 */
static int sndout_optic_light_onoff (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = NULL;

    debug_call ();

    if (ucontrol->value.integer.value[0] < 0 || ucontrol->value.integer.value[0] > 1)
    {
        error ("invalid parameter: optic_light_onoff(%ld)\n", ucontrol->value.integer.value[0]);
        return -1;
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    if (sndout_info->opened_output & COMMON_OPTIC)
        list_output = output_get (COMMON_OPTIC);
    else if (sndout_info->opened_output & COMMON_OPTIC_LG)
        list_output = output_get (COMMON_OPTIC_LG);

    sndout_info->optic_light_on = ucontrol->value.integer.value[0];

    if (list_output)
        output_set_on (list_output->output_info, sndout_info->optic_light_on);

    SNDOUT_UNLOCK ();

    noti ("optic_light_onoff %d \n", sndout_info->optic_light_on);
    return 0;
}

/* @0xffffffc010cd0620 */
static int sndout_arc_onoff_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 1;
    return 0;
}

/* @0xffffffc010cd0b10 */
static int sndout_arc_onoff_get (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->arc_on;

    info ("arc_onoff %d .\n", sndout_info->arc_on);
    return 0;
}

#ifndef CHIP_TYPE_FPGA
extern int HDMI21_Rx_Set_ARC (UINT8 port, BOOLEAN bOnOff);
#endif
static int hdmi_set_hdmiarc (unsigned int port, bool bOnOff)
{
#ifndef CHIP_TYPE_FPGA
#ifdef INCLUDE_KDRV_HDMI20
    return HDMI21_Rx_Set_ARC (port, bOnOff);
#else
    return FALSE;
#endif
#else
    return FALSE;
#endif
}

/* @0xffffffc010cd98e4 */
static int sndout_arc_onoff (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int arc_onoff = 0;

    debug_call ();

    arc_onoff = ucontrol->value.integer.value[0];

    noti ("Start arc_onoff %d \n", arc_onoff);

    if (arc_onoff < 0)
    {
        error ("invalid parameter: arc_onoff(%d)\n", arc_onoff);
        return -1;
    }

    hdmi_set_hdmiarc (0, arc_onoff);

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    if (sndout_info->arc_on != arc_onoff)
    {
        if (_gNewInterfaceOpticArc)
        {
            struct alsa_sndout_list_output *list_output = NULL;

            if (sndout_info->opened_output & COMMON_ARC)
                list_output = output_get (COMMON_ARC);
            else if (sndout_info->opened_output & COMMON_SE_ARC)
                list_output = output_get (COMMON_SE_ARC);

            if (arc_onoff)
                _connect_output (list_output, &sndout_info->arc);
        }
        else
        {
            if (arc_onoff)
                _old_arc_connect_output (sndout_info->arc_output_type);
        }
    }

    sndout_info->arc_on = arc_onoff;

    SNDOUT_UNLOCK ();

    notifier_output_changed ();

    noti ("arc_onoff %d \n", arc_onoff);
    return 0;
}

/* @0xffffffc010cd0100 */
static int sndout_optic_lg_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 4;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd12e0 */
static int sndout_optic_lg_get (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->soundbar.id;
    ucontrol->value.integer.value[1] = sndout_info->soundbar.volume;
    ucontrol->value.integer.value[2] = sndout_info->soundbar.mute;
    ucontrol->value.integer.value[3] = sndout_info->soundbar.power_on;

    info ("sndout_optic_lg_get  0x%lx 0x%lx 0x%lx 0x%lx.\n", ucontrol->value.integer.value[0],
        ucontrol->value.integer.value[1], ucontrol->value.integer.value[2],
        ucontrol->value.integer.value[3]);
    return 0;
}

/* @0xffffffc010cd9690 */
static int sndout_optic_lg (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int id = ucontrol->value.integer.value[0];
    int volume = ucontrol->value.integer.value[1];
    int mute = ucontrol->value.integer.value[2];
    int power_on = ucontrol->value.integer.value[3];
    int ret = 0;

    debug_call ();

    if (volume > 100 || mute > 1 || mute < 0 || power_on > 1 || power_on < 0)
    {
        error ("invalid parameter: id(%d) volume(%d), bMuteOnOff(%d), bPowerOnOff(%d)\n", id,
            volume, mute, power_on);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    if (sndout_info->opened_output & COMMON_OPTIC_LG)
    {
        struct alsa_sndout_list_output *list_output = output_get (COMMON_OPTIC_LG);
        struct sound_bar_param sound_bar_pram = { };

        /* Set a sound bar ID and Data */
        sound_bar_pram.id = (unsigned int) id;
        sound_bar_pram.data = (unsigned int) ((volume & 0x7F) | ((unsigned int) mute << 7));

        if (list_output)
        {
            output_set_sound_bar_param (list_output->output_info, &sound_bar_pram);
            output_set_sound_bar_power (list_output->output_info, power_on);
        }
    }
    else
    {
        noti ("OPTIC_LG output is not opend\n");
        ret = -(ENODEV);
        goto exit;
    }

    sndout_info->soundbar.id = id;
    sndout_info->soundbar.volume = volume;
    sndout_info->soundbar.mute = mute;
    sndout_info->soundbar.power_on = power_on;

    noti ("sndout_optic_lg_get  id 0x%x volume %d mute %d power %d.\n", id, volume, mute, power_on);

  exit:
    SNDOUT_UNLOCK ();
    return ret;
}

/* @0xffffffc010cd0120 */
static int sndout_gain_audio_desc_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_GAIN_MAX;
    return 0;
}

/* @0xffffffc010cd0c10 */
static int sndout_gain_audio_desc_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->ad_gain;

    info ("ad_gain 0x%x.\n", sndout_info->ad_gain);

    return 0;
}

/* @0xffffffc010cdc5c0 */
void sndout_set_audio_desc (int ad_index, bool prev_ad_on, bool ad_on)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    bool should_set = false;
    unsigned int gain = 0x800000;

    if (ad_index >= ALSA_ADEC_MAX)
    {
        error ("invalid ad_index %d .\n", ad_index);
        return;
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->ad_index = ad_index;
    sndout_info->ad_on = ad_on;

    if (prev_ad_on == false && ad_on == true)
    {
        should_set = true;
        gain = adjust_input_ad_gain (ad_index, sndout_info->ad_gain);
    }
    else if (prev_ad_on == true && ad_on == false)
    {
        should_set = true;
        gain = sndout_info->param_input[ad_index].gain;
    }

    if (should_set)
    {
        mixer_set_volume (sndout_info->mixer_info, ad_index, gain);
        info ("ad_index %d gain %d\n", ad_index, gain);
    }

    SNDOUT_UNLOCK ();
}

/* @0xffffffc010cdad80 */
static int sndout_gain_audio_desc (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int ad_gain = 0;
    int gain = 0;

    debug_call ();

    ad_gain = ucontrol->value.integer.value[0];

    if (ad_gain > ALSA_INTEGER_GAIN_MAX)
    {
        error ("invalid parameter: ad_gain(%d)\n", ad_gain);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->ad_gain = ad_gain;

    gain = adjust_input_ad_gain (sndout_info->ad_index, ad_gain);
    mixer_set_volume (sndout_info->mixer_info, sndout_info->ad_index, gain);

    SNDOUT_UNLOCK ();

    noti ("ad_index : %d, ad_gain: 0x%x, gain: 0x%x.\n", sndout_info->ad_index, ad_gain, gain);

    return 0;
}

/* @0xffffffc010cd0140 */
static int sndout_spk_output_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 4;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd0d10 */
static int sndout_spk_output_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->spk_output_type;

    info ("spk_output_type %d\n", sndout_info->spk_output_type);

    return 0;
}

/* @0xffffffc010cd94c0 */
static int sndout_spk_output (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    enum sndout_spk_output_ext_type spk_output_type = ucontrol->value.integer.value[0];
    alsa_se_handle_t alsa_se;

    debug_call ();

    if (spk_output_type < 0 || spk_output_type > SNDOUT_SPK_OUTPUT_4CHANNEL)
    {
        error ("spk_output_type(%d) is invalid.\n", spk_output_type);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->spk_output_type = spk_output_type;

    aud_hal_output_set_number_of_i2s (spk_output_type);

    SNDOUT_UNLOCK ();

    alsa_se = alsa_se_get_handle ("COMMON_SPK");
    if (alsa_se == NULL)
    {
        error ("alsa_se is NULL.\n");
        return -(ENODEV);
    }

    alsa_se_set_number_of_output_channels (alsa_se, spk_output_type * 2);

    noti ("spk_output_type %d\n", spk_output_type);
    return 0;
}

/* @0xffffffc010cd0640 */
static int sndout_earc_onoff_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd0d90 */
static int sndout_earc_onoff_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->earc_on;

    info ("earc_on %d\n", sndout_info->earc_on);

    return 0;
}

/* @0xffffffc010cd9260 */
static int sndout_earc_onoff (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = NULL;
    bool earcOnOff = false;
    bool preEarcOnOff = sndout_info->earc_on;

    debug_call ();

    if (ucontrol->value.integer.value[0] < 0 || ucontrol->value.integer.value[0] > 1)
    {
        error ("invalid parameter: earcOnOff (%ld)\n", ucontrol->value.integer.value[0]);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    earcOnOff = ucontrol->value.integer.value[0];
    sndout_info->earc_on = earcOnOff;

    noti ("Start earcOnOff (%d)\n", earcOnOff);

    if (preEarcOnOff != earcOnOff)
    {
        if (sndout_info->opened_output & COMMON_ARC)
            list_output = output_get (COMMON_ARC);
        else if (sndout_info->opened_output & COMMON_SE_ARC)
            list_output = output_get (COMMON_SE_ARC);

        if (list_output)
        {
            output_set_earc_on (list_output->output_info, earcOnOff);
            if (_gNewInterfaceOpticArc)
            {
                if (earcOnOff)
                    _connect_output (list_output, &sndout_info->earc);
                else
                    _connect_output (list_output, &sndout_info->arc);
            }
            else
            {
                if (earcOnOff)
                    _old_connect_output (&sndout_info->earc, sndout_info->arc_output_type);
                else
                    _old_arc_connect_output (sndout_info->arc_output_type);
            }
        }
    }

    SNDOUT_UNLOCK ();

    notifier_output_changed ();

    noti ("earcOnOff (%d)\n", earcOnOff);

    return 0;
}

/* @0xffffffc010cd0160 */
static int sndout_earc_output_type_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 5;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd1450 */
static int sndout_earc_output_type_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->earc.set_type;
    ucontrol->value.integer.value[1] = sndout_info->earc.codec_type;
    ucontrol->value.integer.value[2] = sndout_info->earc.channel_num;
    ucontrol->value.integer.value[3] = sndout_info->earc.sample_rate;
    ucontrol->value.integer.value[4] = sndout_info->earc.mix_option;

    info ("Earc Output Type : st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n",
        sndout_info->earc.set_type, sndout_info->earc.codec_type,
        sndout_info->earc.channel_num, sndout_info->earc.sample_rate, sndout_info->earc.mix_option);

    return 0;
}

/* @0xffffffc010cd8fe0: verified Image +0xc4..+0xe4; unsigned 32-bit
 * limits {2,9,8,192000,1}. Reject before lock/init; successful settings are
 * cached even with ARC off, then notify after unlocking. */
static int sndout_earc_output_type (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = output_get (COMMON_ARC);

    sndout_output_set_type_t set_type = ucontrol->value.integer.value[0];
    sndout_output_codec_type_t codec_type = ucontrol->value.integer.value[1];
    sndout_output_channel_num_t channel_num = ucontrol->value.integer.value[2];
    sndout_output_sample_rate_t sample_rate = ucontrol->value.integer.value[3];
    sndout_output_mix_option_t mix_option = ucontrol->value.integer.value[4];

    debug_call ();

    noti ("Start Earc Output Type: st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n", set_type, codec_type,
        channel_num, sample_rate, mix_option);

    /* webOS25-shipped: codec_type limit is 9 (SNDOUT_CODEC_MPEG4_AAC), not
     * SNDOUT_CODEC_DTSX_P1 (11): DTSHD_MA (10) and DTSX_P1 (11) -> -EINVAL. */
    if (set_type > SNDOUT_OUTPUT_SET_USE || codec_type > SNDOUT_CODEC_MPEG4_AAC ||
        channel_num > SNDOUT_CHNUM_8 || sample_rate > SNDOUT_SAMPLERATE_192 ||
        mix_option > SNDOUT_MIXOPTION_ALL)
    {
        error ("invalid parameter: st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n", set_type, codec_type,
            channel_num, sample_rate, mix_option);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->earc.set_type = set_type;
    sndout_info->earc.codec_type = codec_type;
    sndout_info->earc.channel_num = channel_num;
    sndout_info->earc.sample_rate = sample_rate;
    sndout_info->earc.mix_option = mix_option;

    if ((sndout_info->opened_output & COMMON_ARC) && sndout_info->earc_on)
    {
        if (_gNewInterfaceOpticArc)
            _connect_output (list_output, &sndout_info->earc);
        else
            _old_connect_output (&sndout_info->earc, sndout_info->arc_output_type);
    }

    SNDOUT_UNLOCK ();

    notifier_output_changed ();

    noti ("Earc Output Type: st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n", set_type, codec_type,
        channel_num, sample_rate, mix_option);

    return 0;
}

/* @0xffffffc010cd0660 */
static int sndout_arc_output_type_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 5;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd1510 */
static int sndout_arc_output_type_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->arc.set_type;
    ucontrol->value.integer.value[1] = sndout_info->arc.codec_type;
    ucontrol->value.integer.value[2] = sndout_info->arc.channel_num;
    ucontrol->value.integer.value[3] = sndout_info->arc.sample_rate;
    ucontrol->value.integer.value[4] = sndout_info->arc.mix_option;

    info ("Arc Output Type : st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n",
        sndout_info->arc.set_type, sndout_info->arc.codec_type,
        sndout_info->arc.channel_num, sndout_info->arc.sample_rate, sndout_info->arc.mix_option);

    return 0;
}

/* @0xffffffc010cd5ba0 */
static int sndout_arc_output_type (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = output_get (COMMON_ARC);

    sndout_output_set_type_t set_type = ucontrol->value.integer.value[0];
    sndout_output_codec_type_t codec_type = ucontrol->value.integer.value[1];
    sndout_output_channel_num_t channel_num = ucontrol->value.integer.value[2];
    sndout_output_sample_rate_t sample_rate = ucontrol->value.integer.value[3];
    sndout_output_mix_option_t mix_option = ucontrol->value.integer.value[4];

    debug_call ();

    noti ("Start Arc Output Type: st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n", set_type, codec_type,
        channel_num, sample_rate, mix_option);

    _gNewInterfaceOpticArc = true;

    if (set_type > SNDOUT_OUTPUT_SET_USE || channel_num > SNDOUT_CHNUM_8
        || sample_rate > SNDOUT_SAMPLERATE_192 || mix_option > SNDOUT_MIXOPTION_ALL)
    {
        error ("invalid parameter: st(%d), cn(%d), sr(%d), mo(%d).\n", set_type, channel_num,
            sample_rate, mix_option);
        return -1;
    }

    if (!(codec_type == SNDOUT_CODEC_PCM || codec_type == SNDOUT_CODEC_AC3
            || codec_type == SNDOUT_CODEC_AAC || codec_type == SNDOUT_CODEC_EAC3
            || codec_type == SNDOUT_CODEC_MPEG4_AAC))   /* webOS25-shipped: no SNDOUT_CODEC_DTS */
    {
        error ("invalid codec_type : %d.\n", codec_type);
        return -1;
    }

    SNDOUT_LOCK ();

    sndout_info->arc.set_type = set_type;
    sndout_info->arc.codec_type = codec_type;
    sndout_info->arc.channel_num = channel_num;
    sndout_info->arc.sample_rate = sample_rate;
    sndout_info->arc.mix_option = mix_option;

    if ((sndout_info->opened_output & COMMON_ARC) && !sndout_info->earc_on)
        _connect_output (list_output, &sndout_info->arc);

    SNDOUT_UNLOCK ();

    notifier_output_changed ();

    noti ("Arc Output Type: st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n", set_type, codec_type,
        channel_num, sample_rate, mix_option);

    return 0;
}

/* @0xffffffc010cd0680 */
static int sndout_optic_output_type_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 5;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd1390 */
static int sndout_optic_output_type_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->optic.set_type;
    ucontrol->value.integer.value[1] = sndout_info->optic.codec_type;
    ucontrol->value.integer.value[2] = sndout_info->optic.channel_num;
    ucontrol->value.integer.value[3] = sndout_info->optic.sample_rate;
    ucontrol->value.integer.value[4] = sndout_info->optic.mix_option;

    info ("Optic Output Type : st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n",
        sndout_info->optic.set_type, sndout_info->optic.codec_type,
        sndout_info->optic.channel_num, sndout_info->optic.sample_rate,
        sndout_info->optic.mix_option);

    return 0;
}

/* @0xffffffc010cd58f4 */
static int sndout_optic_output_type (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = output_get (COMMON_OPTIC);

    sndout_output_set_type_t set_type = ucontrol->value.integer.value[0];
    sndout_output_codec_type_t codec_type = ucontrol->value.integer.value[1];
    sndout_output_channel_num_t channel_num = ucontrol->value.integer.value[2];
    sndout_output_sample_rate_t sample_rate = ucontrol->value.integer.value[3];
    sndout_output_mix_option_t mix_option = ucontrol->value.integer.value[4];

    debug_call ();

    noti ("Start Optic Output Type: st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n", set_type,
        codec_type, channel_num, sample_rate, mix_option);

    _gNewInterfaceOpticArc = true;

    if (set_type > SNDOUT_OUTPUT_SET_USE || channel_num > SNDOUT_CHNUM_8
        || sample_rate > SNDOUT_SAMPLERATE_48 || mix_option > SNDOUT_MIXOPTION_ALL)
    {
        error ("invalid parameter: st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n", set_type, codec_type,
            channel_num, sample_rate, mix_option);
        return -1;
    }

    if (!(codec_type == SNDOUT_CODEC_PCM || codec_type == SNDOUT_CODEC_AC3
            || codec_type == SNDOUT_CODEC_AAC))   /* webOS25-shipped: no SNDOUT_CODEC_DTS */
    {
        error ("invalid codec_type : %d.\n", codec_type);
        return -1;
    }

    SNDOUT_LOCK ();

    sndout_info->optic.set_type = set_type;
    sndout_info->optic.codec_type = codec_type;
    sndout_info->optic.channel_num = channel_num;
    sndout_info->optic.sample_rate = sample_rate;
    sndout_info->optic.mix_option = mix_option;

    if (sndout_info->opened_output & COMMON_OPTIC)
        _connect_output (list_output, &sndout_info->optic);

    SNDOUT_UNLOCK ();

    notifier_output_changed ();

    noti ("Optic Output Type: st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n", set_type, codec_type,
        channel_num, sample_rate, mix_option);

    return 0;
}

/* @0xffffffc010cd06a0 */
static int sndout_timecount_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 2;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_MAX;
    return 0;
}

/* @0xffffffc010cd2710 */
static int sndout_timecount_get (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    unsigned int int_count = 0;
    unsigned int clock_count = 0;
    int main_audio;
    extern ALSA_INPUT_INFO_T alsa_input_info[ALSA_ADEC_MAX];
    ALSA_INPUT_INFO_T *inp = NULL;

    main_audio =
        common_input_convert_index (sndout_info->main_audio_input, sndout_info->main_audio_port);
    if ((main_audio >= 0) && (main_audio < ALSA_ADEC_MAX))
    {
        inp = &alsa_input_info[main_audio];
        if (inp->input_info)
            input_get_int_count (inp->input_info, &int_count, &clock_count);
    }
    else
        error ("invalid main_audio(input:0x%x, port:%d)\n", sndout_info->main_audio_input,
            sndout_info->main_audio_port);

    ucontrol->value.integer.value[0] = int_count;
    ucontrol->value.integer.value[1] = clock_count;

    info ("interrupt count %u, clock_count %u\n", int_count, clock_count);

    return 0;
}

/* @0xffffffc010cd0180 */
static int sndout_resampling_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0x7FFFFFFF;
    return 0;
}

/* @0xffffffc010cd10e0 */
static int sndout_resampling_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    unsigned int outputFreq = 0;
    struct aud_block_info *capture_info = NULL;

    capture_info = bm_get_block_info_by_index ("capture", 10);

    if (capture_info)
        outputFreq = aud_capture_get_sampling_freq (capture_info);
    else
        outputFreq = _resampling_freq;

    ucontrol->value.integer.value[0] = outputFreq;
    info ("get output freq=%u\n", outputFreq);

    return 0;
}

/* @0xffffffc010cd11a0 */
static int sndout_resampling_put (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    unsigned int outputFreq = 0;
    struct aud_block_info *capture_info = NULL;
    outputFreq = ucontrol->value.integer.value[0];

    capture_info = bm_get_block_info_by_index ("capture", 10);

    if (capture_info)
        aud_capture_set_sampling_freq (capture_info, outputFreq);

    _resampling_freq = outputFreq;

    info ("set output freq=%u\n", outputFreq);

    return 0;
}

/* @0xffffffc010cdc760 */
unsigned int sndout_get_resampling_freq (void)
{
    return _resampling_freq;
}

/* @0xffffffc010cdc770 */
void sndout_set_resampling_freq (unsigned int rate)
{
    _resampling_freq = rate;
}

// http://collab.lge.com/main/pages/viewpage.action?pageId=940358726#id-[ALSA]SNDOUT(ENG)-%22SndouteAC3ATMOSEncodeOnOff%22
/* @0xffffffc010cd06c0 */
static int sndout_eac3_atmos_encode_onoff_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_MAX;
    return 0;
}

/* @0xffffffc010cd0a10 */
static int sndout_eac3_atmos_encode_onoff_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->eac3_atmos_encode_on;
    info ("get beAC3AtmosEncodeOn=%d\n", sndout_info->eac3_atmos_encode_on);

    return 0;
}

// refered to SCDCR-2889
/* @0xffffffc010cd8e60 */
static int sndout_eac3_atmos_encode_onoff_put (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int beAC3AtmosEncodeOn = ucontrol->value.integer.value[0];

    debug_call ();

    if (beAC3AtmosEncodeOn < OFF || beAC3AtmosEncodeOn > ON)
    {
        error ("invalid parameter: beAC3AtmosEncodeOn (%d)\n", beAC3AtmosEncodeOn);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->eac3_atmos_encode_on = beAC3AtmosEncodeOn;

    _set_atmos_encode_on (beAC3AtmosEncodeOn);

    SNDOUT_UNLOCK ();

    noti ("set beAC3AtmosEncodeOn(%d)\n", beAC3AtmosEncodeOn);

    return 0;
}

#if (SE_VER >= 82)
#include "audio_delay_table_se82.h"
#elif (SE_VER == 81)
#include "audio_delay_table_se81.h"
#else
#include "audio_delay_table.h"
#endif
static int _lookup_delay_table (int src, int codec, int type)
{
    int delay = 0;
    int i = 0;
    for (i = 0; i < ARRAY_SIZE (delay_table_ms); i++)
    {
        if (src == delay_table_ms[i].src && codec == delay_table_ms[i].codec
            && type == delay_table_ms[i].type)
            return delay_table_ms[i].delay;
    }

    return delay;
}

static delay_output_type_t _get_current_output (common_input_ext_type_t input, int port,
    common_output_ext_type_t output, int acmod)
{
    struct alsa_sndout_list_output *list_output = NULL;
    delay_output_type_t ret = DELAY_OUTPUT_MAX;

    if (output == COMMON_SPK)
    {
        alsa_se_handle_t alsa_se;
        enum alse_se_mode mode;

        alsa_se = alsa_se_get_handle ("COMMON_SPK");
        mode = alsa_se_get_mode (alsa_se);

        if (mode == ALSA_SE_MODE_LGSE_ONLY)
            ret = DELAY_OUTPUT_SPK;
        else if (mode == ALSA_SE_MODE_LGSE_ATMOS)
            ret = DELAY_OUTPUT_SPK_DAP;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_GEN1_GEN2)
            if (acmod == 0 || acmod == 2)
                ret = DELAY_OUTPUT_SPK_AISE_GEN_1_2;
            else
                ret = DELAY_OUTPUT_SPK_AISE_BYPASS;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_GEN2)
            if (acmod == 0 || acmod == 2)
                ret = DELAY_OUTPUT_SPK_AISE_GEN_2;
            else
                ret = DELAY_OUTPUT_SPK_AISE_BYPASS;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_GEN1)
            if (acmod == 0 || acmod == 2)
                ret = DELAY_OUTPUT_SPK_AISE_GEN_1;
            else
                ret = DELAY_OUTPUT_SPK_AISE_BYPASS;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_BYPASS)
            ret = DELAY_OUTPUT_SPK_AISE_BYPASS;
        else
            ret = DELAY_OUTPUT_SPK;
    }
    else if (output == COMMON_OPTIC)
    {
        list_output = output_get (COMMON_OPTIC);

        if (list_output)
        {
            struct sndout_output_format *set_output = &list_output->set_output;

            if (set_output->bypass)
                ret = DELAY_OUTPUT_SPDIF_PASSTHRU;
            else
            {
                switch (set_output->output_codec)
                {
                    case OUTPUT_CODEC_PCM:
                        ret = DELAY_OUTPUT_SPDIF_PCM;
                        break;
                    case OUTPUT_CODEC_AC3:
                    case OUTPUT_CODEC_EAC3:
                        ret = DELAY_OUTPUT_SPDIF_DD;
                        break;
                    case OUTPUT_CODEC_AAC:
                        ret = DELAY_OUTPUT_SPDIF_AAC;
                        break;
                    /* webOS25-shipped: no OUTPUT_CODEC_DTS */
                    default:
                        ret = DELAY_OUTPUT_MAX;
                        break;
                }
            }
        }
        else
            ret = DELAY_OUTPUT_SPDIF_PCM;
    }
    else if (output == COMMON_ARC)
    {
        list_output = output_get (COMMON_ARC);

        if (list_output)
        {
            struct sndout_output_format *set_output = &list_output->set_output;

            if (set_output->bypass)
                ret = DELAY_OUTPUT_ARC_PASSTHRU;
            else
            {
                switch (set_output->output_codec)
                {
                    case OUTPUT_CODEC_PCM:
                        ret = DELAY_OUTPUT_ARC_PCM;
                        break;
                    case OUTPUT_CODEC_AC3:
                    case OUTPUT_CODEC_EAC3:
                        ret = DELAY_OUTPUT_ARC_DDP;
                        break;
                    case OUTPUT_CODEC_AAC:
                        ret = DELAY_OUTPUT_ARC_AAC;
                        break;
                    case OUTPUT_CODEC_MAT:
                        ret = DELAY_OUTPUT_ARC_MAT;
                        break;
                    /* webOS25-shipped: no OUTPUT_CODEC_DTS* -> DELAY_OUTPUT_ARC_DTS unused */
                    default:
                        ret = DELAY_OUTPUT_MAX;
                        break;
                }
            }
        }
        else
            ret = DELAY_OUTPUT_ARC_PCM;
    }
    else if (output == COMMON_WOWCAST)
    {
        list_output = output_get (COMMON_WOWCAST);

        if (list_output)
        {
            struct sndout_output_format *set_output = &list_output->set_output;

            if (set_output->bypass)
                ret = DELAY_OUTPUT_ARC_PASSTHRU;
            else
            {
                switch (set_output->output_codec)
                {
                    case OUTPUT_CODEC_PCM:
                        ret = DELAY_OUTPUT_ARC_PCM;
                        break;
                    case OUTPUT_CODEC_AC3:
                    case OUTPUT_CODEC_EAC3:
                        ret = DELAY_OUTPUT_ARC_DDP;
                        break;
                    case OUTPUT_CODEC_AAC:
                        ret = DELAY_OUTPUT_ARC_AAC;
                        break;
                    case OUTPUT_CODEC_MAT:
                        ret = DELAY_OUTPUT_ARC_MAT;
                        break;
                    /* webOS25-shipped: no OUTPUT_CODEC_DTS* -> DELAY_OUTPUT_ARC_DTS unused */
                    default:
                        ret = DELAY_OUTPUT_MAX;
                        break;
                }
            }
        }
        else
            ret = DELAY_OUTPUT_ARC_PCM;
    }
    else if (output == COMMON_OPTIC_LG)
    {
        ret = DELAY_OUTPUT_SPDIF_PCM;
    }
    else if (output == COMMON_BLUETOOTH || output == COMMON_WISA || output == COMMON_CAPTURE)
    {
        ret = DELAY_OUTPUT_BT;
    }
    else if (output == COMMON_HP)
    {
        ret = DELAY_OUTPUT_HP;
    }
    else if (output == COMMON_SE_BT || output == COMMON_SE_BT_SUR || output == COMMON_DAP_HP_BT)
    {
        alsa_se_handle_t alsa_se;
        enum alse_se_mode mode;

        if ((output == COMMON_SE_BT) || (output == COMMON_SE_BT_SUR))
            alsa_se = alsa_se_get_handle ("COMMON_SE_BT");
        else if (output == COMMON_DAP_HP_BT)
            alsa_se = alsa_se_get_handle ("COMMON_DAP_HP_BT");

        mode = alsa_se_get_mode (alsa_se);

        if (mode == ALSA_SE_MODE_LGSE_ONLY)
            ret = DELAY_OUTPUT_SE_BT;
        else if (mode == ALSA_SE_MODE_LGSE_ATMOS)
            ret = DELAY_OUTPUT_SE_BT_DAP;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_GEN1_GEN2)
            if (acmod == 0 || acmod == 2)
                ret = DELAY_OUTPUT_SE_BT_AISE_GEN_1_2;
            else
                ret = DELAY_OUTPUT_SE_BT_AISE_BYPASS;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_GEN2)
            if (acmod == 0 || acmod == 2)
                ret = DELAY_OUTPUT_SE_BT_AISE_GEN_2;
            else
                ret = DELAY_OUTPUT_SE_BT_AISE_BYPASS;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_GEN1)
            if (acmod == 0 || acmod == 2)
                ret = DELAY_OUTPUT_SE_BT_AISE_GEN_1;
            else
                ret = DELAY_OUTPUT_SE_BT_AISE_BYPASS;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_BYPASS)
            ret = DELAY_OUTPUT_SE_BT_AISE_BYPASS;
        else
            ret = DELAY_OUTPUT_SE_BT;
    }
    else if (output == COMMON_SE_ARC)
    {
        alsa_se_handle_t alsa_se;
        enum alse_se_mode mode;

        alsa_se = alsa_se_get_handle ("COMMON_SE_ARC");
        mode = alsa_se_get_mode (alsa_se);

        if (mode == ALSA_SE_MODE_LGSE_ONLY)
            ret = DELAY_OUTPUT_SE_ARC;
        else if (mode == ALSA_SE_MODE_LGSE_ATMOS)
            ret = DELAY_OUTPUT_SE_ARC_DAP;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_GEN1_GEN2)
            if (acmod == 0 || acmod == 2)
                ret = DELAY_OUTPUT_SE_ARC_AISE_GEN_1_2;
            else
                ret = DELAY_OUTPUT_SE_ARC_AISE_BYPASS;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_GEN2)
            if (acmod == 0 || acmod == 2)
                ret = DELAY_OUTPUT_SE_ARC_AISE_GEN_2;
            else
                ret = DELAY_OUTPUT_SE_ARC_AISE_BYPASS;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_GEN1)
            if (acmod == 0 || acmod == 2)
                ret = DELAY_OUTPUT_SE_ARC_AISE_GEN_1;
            else
                ret = DELAY_OUTPUT_SE_ARC_AISE_BYPASS;
        else if (mode == ALSA_SE_MODE_LGSE_AISOUND_BYPASS)
            ret = DELAY_OUTPUT_SE_ARC_AISE_BYPASS;
        else
            ret = DELAY_OUTPUT_SE_ARC;
    }
    else if (output == COMMON_WOWCAST)
    {
        list_output = output_get (COMMON_WOWCAST);

        if (list_output)
        {
            struct sndout_output_format *set_output = &list_output->set_output;

            if (set_output->bypass)
                ret = DELAY_OUTPUT_WOWCAST_PASSTHRU;
            else
            {
                switch (set_output->output_codec)
                {
                    case OUTPUT_CODEC_PCM:
                        ret = DELAY_OUTPUT_WOWCAST_PCM;
                        break;
                    case OUTPUT_CODEC_AC3:
                    case OUTPUT_CODEC_EAC3:
                        ret = DELAY_OUTPUT_WOWCAST_DDP;
                        break;
                    case OUTPUT_CODEC_AAC:
                        ret = DELAY_OUTPUT_WOWCAST_AAC;
                        break;
                    case OUTPUT_CODEC_MAT:
                        ret = DELAY_OUTPUT_WOWCAST_MAT;
                        break;
                    /* webOS25-shipped: no OUTPUT_CODEC_DTS* -> DELAY_OUTPUT_WOWCAST_DTS unused */
                    default:
                        ret = DELAY_OUTPUT_MAX;
                        break;
                }
            }
        }
        else
            ret = DELAY_OUTPUT_WOWCAST_PCM;
    }

    return ret;
}

static int encoder_processing_delay[OUTPUT_CODEC_MAX] = {
    [OUTPUT_CODEC_AC3] = 30,
    [OUTPUT_CODEC_EAC3] = 30,
    [OUTPUT_CODEC_AAC] = 20,
    [OUTPUT_CODEC_MAT] = 20,
};

module_param_array (encoder_processing_delay, int, NULL, 0644);

static int speaker_processing_delay[] = {
    [LGSE_MODE_LGSE_ONLY] = 10,
    [LGSE_MODE_LGSE_ATMOS] = 15,
    [LGSE_MODE_LGSE_AISOUND] = 20,
};

module_param_array (speaker_processing_delay, int, NULL, 0644);

/* @0xffffffc010cd01a0 */
static int sndout_audio_latency_time_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 8;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_MAX;
    return 0;
}

static adec_src_codec_ext_type_t _decoded_info_convert_src_codec (struct decoded_info *decoded_info)
{
    adec_src_codec_ext_type_t src_codec = ADEC_SRC_CODEC_PCM;

    switch (decoded_info->codec)
    {
        case AUDIO_DECODER_CODEC_AC3:
            if (decoded_info->atmos)
                src_codec = ADEC_SRC_CODEC_EAC3_ATMOS;
            else if (decoded_info->eac3)
                src_codec = ADEC_SRC_CODEC_EAC3;
            else
                src_codec = ADEC_SRC_CODEC_AC3;
            break;

        case AUDIO_DECODER_CODEC_EAC3:
            src_codec = ADEC_SRC_CODEC_EAC3;
            break;

        case AUDIO_DECODER_CODEC_AAC:
            if (decoded_info->version == 0)
                src_codec = ADEC_SRC_CODEC_AAC;
            else
                src_codec = ADEC_SRC_CODEC_HEAAC;
            break;

        case AUDIO_DECODER_CODEC_AC4:
            if (decoded_info->atmos)
                src_codec = ADEC_SRC_CODEC_AC4_ATMOS;
            else
                src_codec = ADEC_SRC_CODEC_AC4;
            break;

        case AUDIO_DECODER_CODEC_MP3:
            src_codec = ADEC_SRC_CODEC_MPEG;
            break;

        case AUDIO_DECODER_CODEC_MPEG_H:
            src_codec = ADEC_SRC_CODEC_MPEG_H;
            break;

        case AUDIO_DECODER_CODEC_DRA:
            src_codec = ADEC_SRC_CODEC_DRA;
            break;

        case AUDIO_DECODER_CODEC_MAT:
        case AUDIO_DECODER_CODEC_TRUE_HD:
            if (decoded_info->truehd)
            {
                if (decoded_info->atmos)
                    src_codec = ADEC_SRC_CODEC_TRUEHD_ATMOS;
                else
                    src_codec = ADEC_SRC_CODEC_TRUEHD;
            }
            else
            {
                if (decoded_info->atmos)
                    src_codec = ADEC_SRC_CODEC_MAT_ATMOS;
                else
                    src_codec = ADEC_SRC_CODEC_MAT;
            }
            break;

        /* webOS25-shipped: no AUDIO_DECODER_CODEC_DTS/DTS_HD */

        default:
            break;
    }

    return src_codec;
}

static adec_src_codec_ext_type_t _decoder_codec_convert_src_codec (enum audio_decoder_codec
    decoder_codec)
{
    adec_src_codec_ext_type_t src_codec = ADEC_SRC_CODEC_PCM;

    switch (decoder_codec)
    {
        case AUDIO_DECODER_CODEC_AC3:
            src_codec = ADEC_SRC_CODEC_AC3;
            break;

        case AUDIO_DECODER_CODEC_EAC3:
            src_codec = ADEC_SRC_CODEC_EAC3;
            break;

        case AUDIO_DECODER_CODEC_AAC:
            src_codec = ADEC_SRC_CODEC_AAC;
            break;

        case AUDIO_DECODER_CODEC_AC4:
            src_codec = ADEC_SRC_CODEC_AC4;
            break;

        case AUDIO_DECODER_CODEC_MP3:
            src_codec = ADEC_SRC_CODEC_MPEG;
            break;

        case AUDIO_DECODER_CODEC_MPEG_H:
            src_codec = ADEC_SRC_CODEC_MPEG_H;
            break;

        case AUDIO_DECODER_CODEC_DRA:
            src_codec = ADEC_SRC_CODEC_DRA;
            break;

        case AUDIO_DECODER_CODEC_MAT:
        case AUDIO_DECODER_CODEC_TRUE_HD:
            src_codec = ADEC_SRC_CODEC_MAT;
            break;

        /* webOS25-shipped: no AUDIO_DECODER_CODEC_DTS/DTS_HD */

        default:
            break;
    }

    return src_codec;
}

/* @0xffffffc010cd8550 */
static int sndout_audio_latency_time_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    int delay = 0;
    delay_output_type_t output;

    int master_output = OUTPUT_MAX;
    int master_index = 0;
    adec_src_codec_ext_type_t codec = ADEC_SRC_CODEC_PCM;
    int acmod = 2;

    master_index =
        common_input_convert_index (_gSndoutAudioLatencyQuery.input,
        _gSndoutAudioLatencyQuery.port);
    if (master_index < 0)
        error ("invalid input(input:0x%x, port:%d)\n", _gSndoutAudioLatencyQuery.input,
            _gSndoutAudioLatencyQuery.port);

    master_output = common_output_convert_output (_gSndoutAudioLatencyQuery.output);

    // decoder codec info
    if (_gSndoutAudioLatencyQuery.input == COMMON_INPUT_ADEC)
    {
        struct decoded_info decoded_info = { };
        struct aud_block_info *decoder_block = NULL;

        decoder_block = bm_get_block_info_by_index ("decoder", master_index);
        if (decoder_block)
        {
            enum audio_decoder_input decoder_input;

            decoder_input = decoder_get_input (decoder_block);
            if ((decoder_input == AUDIO_DECODER_INPUT_SIF)
                || (decoder_input == AUDIO_DECODER_INPUT_ADC))
                codec = ADEC_SRC_CODEC_PCM;
            else
            {
                decoder_get_decoded_info (decoder_block, &decoded_info);
                if (decoded_info.codec == AUDIO_DECODER_CODEC_UNKNOWN)
                {
                    enum audio_decoder_codec decoder_codec;

                    decoder_codec = decoder_get_codec (decoder_block);
                    codec = _decoder_codec_convert_src_codec (decoder_codec);
                }
                else
                    codec = _decoded_info_convert_src_codec (&decoded_info);

                acmod = decoded_info.acmod;
            }
        }
        else
            codec = _gSndoutAudioLatencyQuery.codec;
    }
    _gSndoutAudioLatencyQuery.codec = codec;

    // output info
    output =
        _get_current_output (_gSndoutAudioLatencyQuery.input, _gSndoutAudioLatencyQuery.port,
        _gSndoutAudioLatencyQuery.output, acmod);

    delay = _lookup_delay_table (_gSndoutAudioLatencyQuery.src, codec, output);

    noti ("src:%d, codec:0x%x, acmod:0x%x, output:%d, delay:%d.\n", _gSndoutAudioLatencyQuery.src,
        codec, acmod, output, delay);

    ucontrol->value.integer.value[0] = _gSndoutAudioLatencyQuery.input;
    ucontrol->value.integer.value[1] = _gSndoutAudioLatencyQuery.port;
    ucontrol->value.integer.value[2] = _gSndoutAudioLatencyQuery.src;
    ucontrol->value.integer.value[3] = _gSndoutAudioLatencyQuery.output;
    ucontrol->value.integer.value[4] = 0;
    ucontrol->value.integer.value[5] = 0;
    ucontrol->value.integer.value[6] = 0;
    ucontrol->value.integer.value[7] = delay;
    return 0;
}

/* @0xffffffc010cd3730 */
static int sndout_audio_latency_time_put (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    int input_type = ucontrol->value.integer.value[0];
    int port_index = ucontrol->value.integer.value[1];
    int input_source = ucontrol->value.integer.value[2];
    int output_type = ucontrol->value.integer.value[3];

    int master_output = OUTPUT_MAX;
    int master_index = 0;

    debug_call ();

    master_index = common_input_convert_index (input_type, port_index);
    if (master_index < 0)
    {
        error ("invalid input(input:0x%x, port:%d)\n", input_type, port_index);
        return -(EINVAL);
    }
    master_output = common_output_convert_output (output_type);
    if (master_output < 0)
    {
        error ("invalid output(0x%x)\n", output_type);
        return -(EINVAL);
    }
    if (input_source < DELAY_INPUT_SRC_UNKNOWN || input_source >= DELAY_INPUT_SRC_MAX)
    {
        error ("invalid input_source(0x%x)\n", input_source);
        return -(EINVAL);
    }

    _gSndoutAudioLatencyQuery.input = input_type;
    _gSndoutAudioLatencyQuery.port = port_index;
    _gSndoutAudioLatencyQuery.src = input_source;
    _gSndoutAudioLatencyQuery.output = output_type;

    info ("input 0x%x port %d input_source %d output 0x%x .\n", input_type, port_index,
        input_source, output_type);

    return 0;
}

/* @0xffffffc010cd06e0 */
static int sndout_capture_path_info (struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_MAX;
    return 0;
}

/* @0xffffffc010cd01c0 */
static int sndout_capture_path_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ucontrol->value.integer.value[0] = _gAlsaCaptureSource;

    return 0;
}

/* @0xffffffc010cdc780 */
struct aud_block_info *_get_capture_source_block (int capture_source)
{
/*
 * SNDOUT_CAPTURE_PATH_ADEC0 = 0x01
Set capture path to ADEC0

SNDOUT_CAPTURE_PATH_ADEC1 = 0x02
Set capture path to ADEC1

SNDOUT_CAPTURE_PATH_MIXER = 0x03
Set capture path to mixer output ( SE output)

SNDOUT_CAPTURE_PATH_OUT_SPK = 0x04
Set capture path to Speaker output (SE output)

SNDOUT_CAPTURE_PATH_OUT_OPTICAL = 0x05
Set capture path to Optical output

SNDOUT_CAPTURE_PATH_OUT_ARC = 0x06
Set capture path to ARC output

SNDOUT_CAPTURE_PATH_OUT_HP = 0x07
Set capture path to HP output
*/
    struct alsa_sndout_list_output *list_output = NULL;
    struct aud_block_info *block = NULL;
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    switch (capture_source)
    {
        case SNDOUT_CAPTURE_PATH_ADEC0:
            block = bm_get_block_info_by_index ("decoder", 0);
            break;
        case SNDOUT_CAPTURE_PATH_ADEC1:
            block = bm_get_block_info_by_index ("decoder", 1);
            break;
        case SNDOUT_CAPTURE_PATH_DEFAULT:
        case SNDOUT_CAPTURE_PATH_MIXER:
            block = sndout_info->mixer_info;
            break;
        case SNDOUT_CAPTURE_PATH_OUT_SPK:
            list_output = output_get (COMMON_SPK);
            if (list_output)
                block = list_output->output_info;
            break;
        case SNDOUT_CAPTURE_PATH_OUT_OPTICAL:
            list_output = output_get (COMMON_OPTIC);
            if (list_output)
                block = list_output->output_info;
            break;
        case SNDOUT_CAPTURE_PATH_OUT_ARC:
            list_output = output_get (COMMON_ARC);
            if (list_output)
                block = list_output->output_info;
            break;
        case SNDOUT_CAPTURE_PATH_OUT_HP:
            list_output = output_get (COMMON_HP);
            if (list_output)
                block = list_output->output_info;
            break;
        case SNDOUT_CAPTURE_PATH_OUT_WOWCAST:
            list_output = output_get (COMMON_WOWCAST);
            if (list_output)
                block = list_output->output_info;
            break;
        default:
            break;
    }

    return block;
}

/* @0xffffffc010cd2a64 */
static int sndout_capture_path_put (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    int capture_source = ucontrol->value.integer.value[0];

    if (capture_source < SNDOUT_CAPTURE_PATH_DEFAULT
        || capture_source > SNDOUT_CAPTURE_PATH_OUT_WOWCAST)
    {
        error ("invalid capture source:%d", capture_source);
        return -(EINVAL);
    }

    _gAlsaCaptureSource = capture_source;
    return 0;
}

/* @0xffffffc010cd0700 */
static int sndout_wowcast_onoff_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 1;
    return 0;
}

/* @0xffffffc010cd0a90 */
static int sndout_wowcast_onoff_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->wowcast_on;

    info ("wowcast_on %d\n", sndout_info->wowcast_on);

    return 0;
}

/* @0xffffffc010cd2cc0 */
static int sndout_wowcast_onoff_put (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    bool wowcastOnOff = false;
    bool preWowcastOnOff = sndout_info->wowcast_on;
    struct aud_block_info *block = NULL;

    debug_call ();

    if (ucontrol->value.integer.value[0] < 0 || ucontrol->value.integer.value[0] > 1)
    {
        error ("invalid parameter: wowcastOnOff (%ld)\n", ucontrol->value.integer.value[0]);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    wowcastOnOff = ucontrol->value.integer.value[0];
    sndout_info->wowcast_on = wowcastOnOff;

    if (preWowcastOnOff != wowcastOnOff)
    {
        block = bm_get_block_info_by_index ("capture", ALSA_CAP_HW_WOW);
        if (block)
            aud_capture_set_disable (block, !wowcastOnOff);

    }

    SNDOUT_UNLOCK ();

    noti ("wowcastOnOff (%d)\n", wowcastOnOff);

    return 0;
}

/* @0xffffffc010cd0720 */
static int sndout_wowcast_output_type_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 5;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 0xFFFFFF;
    return 0;
}

/* @0xffffffc010cd5650 */
static int sndout_wowcast_output_type_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = NULL;
    struct output_format output_format = { 0, };

    list_output = output_get (COMMON_WOWCAST);

    if (list_output)
    {
        output_get_format (list_output->output_info, &output_format);

        ucontrol->value.integer.value[0] = sndout_info->wowcast.set_type;
        ucontrol->value.integer.value[1] =
            output_codec_convert_sndout_output_codec (output_format.codec);
        ucontrol->value.integer.value[2] = output_format.channel;
        ucontrol->value.integer.value[3] = output_format.sample_rate;
        ucontrol->value.integer.value[4] = sndout_info->wowcast.mix_option;
    }

    info ("wowcast Output Type : st(%ld), ct(%ld), cn(%ld), sr(%ld), mo(%ld).\n",
        ucontrol->value.integer.value[0],
        ucontrol->value.integer.value[1],
        ucontrol->value.integer.value[2],
        ucontrol->value.integer.value[3], ucontrol->value.integer.value[4]);

    return 0;
}

/* @0xffffffc010cd8c40 */
static int sndout_wowcast_output_type_put (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = output_get (COMMON_WOWCAST);

    sndout_output_set_type_t set_type = ucontrol->value.integer.value[0];
    sndout_output_codec_type_t codec_type = ucontrol->value.integer.value[1];
    sndout_output_channel_num_t channel_num = ucontrol->value.integer.value[2];
    sndout_output_sample_rate_t sample_rate = ucontrol->value.integer.value[3];
    sndout_output_mix_option_t mix_option = ucontrol->value.integer.value[4];

    debug_call ();

    /* webOS25-shipped: codec_type limit is 9 (SNDOUT_CODEC_MPEG4_AAC), not
     * SNDOUT_CODEC_DTSX_P1 (11): DTSHD_MA (10) and DTSX_P1 (11) -> -EINVAL. */
    if (set_type > SNDOUT_OUTPUT_SET_USE || codec_type > SNDOUT_CODEC_MPEG4_AAC ||
        channel_num > SNDOUT_CHNUM_8 || sample_rate > SNDOUT_SAMPLERATE_192 ||
        mix_option > SNDOUT_MIXOPTION_ALL)
    {
        error ("invalid parameter: st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n", set_type, codec_type,
            channel_num, sample_rate, mix_option);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
        sndout_init ();

    sndout_info->wowcast.set_type = set_type;
    sndout_info->wowcast.codec_type = codec_type;
    sndout_info->wowcast.channel_num = channel_num;
    sndout_info->wowcast.sample_rate = sample_rate;
    sndout_info->wowcast.mix_option = mix_option;

    if (sndout_info->opened_output & COMMON_WOWCAST)
    {
        if (_gNewInterfaceOpticArc)
            _connect_output (list_output, &sndout_info->wowcast);
        else
            _old_connect_output (&sndout_info->wowcast, sndout_info->arc_output_type);
    }

    SNDOUT_UNLOCK ();

    notifier_output_changed ();

    noti ("Wowcast Output Type: st(%d), ct(%d), cn(%d), sr(%d), mo(%d).\n", set_type, codec_type,
        channel_num, sample_rate, mix_option);

    return 0;
}

/* @0xffffffc010cd01d4 */
static int sndout_low_latency_mode_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = ALSA_SNDOUT_INPUT_NUM * 2;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_MAX;
    return 0;
}

/* @0xffffffc010cd01f4 */
static int sndout_low_latency_mode_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    int i = 0;

    for (i = 0; i < ALSA_SNDOUT_INPUT_NUM; i++)
    {
        if (i < ALSA_SNDOUT_DEC_NUM)
            ucontrol->value.integer.value[i * 2] = i;
        else
            ucontrol->value.integer.value[i * 2] = i - ALSA_SNDOUT_DEC_NUM;

        ucontrol->value.integer.value[i * 2 + 1] = _gAlsaSndoutLowLatencyInfo[i];
    }

    return 0;
}

/* @0xffffffc010cd2e60 */
static int sndout_low_latency_mode_put (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    common_input_ext_type_t input_type = COMMON_NO_INPUT;
    int input_port_num = 0;
    low_latency_mode_type_t low_latency_mode = LOW_LATENCY_MODE_OFF;
    int alsa_index = 0;

    input_type = ucontrol->value.integer.value[0];
    input_port_num = ucontrol->value.integer.value[1];
    low_latency_mode = ucontrol->value.integer.value[2];

    if (low_latency_mode < LOW_LATENCY_MODE_OFF || low_latency_mode > LOW_LATENCY_MODE_BYPASS_FADER)
    {
        error ("invalid parameter: low_latency_mode(%d)\n", low_latency_mode);
        return -1;
    }

    alsa_index = common_input_convert_index (input_type, input_port_num);
    if (alsa_index < 0 || alsa_index >= ALSA_SNDOUT_INPUT_NUM)
    {
        error ("wrong input 0x%x, port %d \n", input_type, input_port_num);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    mixer_set_bypass_fader (sndout_info->mixer_info, alsa_index, low_latency_mode);
    _gAlsaSndoutLowLatencyInfo[alsa_index] = low_latency_mode;

    SNDOUT_UNLOCK ();

    noti ("low_latency_mode[%d] = %d\n", input_port_num, low_latency_mode);

    return 0;
}

#if (MS12_VER >= 26)
static int sndout_earc_ms12_sp50_onoff_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 1;
    return 0;
}

static int sndout_earc_ms12_sp50_onoff_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    ucontrol->value.integer.value[0] = sndout_info->earc_ms12_sp50_on;

    info ("earc_ms12_sp50_on %d\n", sndout_info->earc_ms12_sp50_on);

    return 0;
}

static int sndout_earc_ms12_sp50_onoff_put (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = NULL;
    bool sp50OnOff = false;
    bool preSp50OnOff = sndout_info->earc_ms12_sp50_on;

    debug_call ();

    if ((ADEC_MS12_MAJOR_VER < 2) || ((ADEC_MS12_MAJOR_VER == 2) && (ADEC_MS12_MINOR_VER < 6)))
    {
        error ("MS12 Ver %d.%d is not support.\n", ADEC_MS12_MAJOR_VER, ADEC_MS12_MINOR_VER);
        return -1;
    }

    if (ucontrol->value.integer.value[0] < 0 || ucontrol->value.integer.value[0] > 1)
    {
        error ("invalid parameter: sp50OnOff (%ld)\n", ucontrol->value.integer.value[0]);
        return -(EINVAL);
    }

    SNDOUT_LOCK ();

    sp50OnOff = ucontrol->value.integer.value[0];
    sndout_info->earc_ms12_sp50_on = sp50OnOff;

    if (preSp50OnOff != sp50OnOff)
    {
        if (sndout_info->opened_output & COMMON_ARC)
            list_output = output_get (COMMON_ARC);
        else if (sndout_info->opened_output & COMMON_WOWCAST)
            list_output = output_get (COMMON_WOWCAST);

        if (list_output)
        {
            if (((list_output->output == COMMON_ARC)
                    && (sndout_info->earc.codec_type == SNDOUT_CODEC_MAT2PCM))
                || ((list_output->output == COMMON_WOWCAST)
                    && (sndout_info->wowcast.codec_type == SNDOUT_CODEC_MAT2PCM)))
                mixer_set_sp50 (sndout_info->mixer_info, sp50OnOff);
        }
    }

    SNDOUT_UNLOCK ();

    noti ("sp50OnOff (%d)\n", sp50OnOff);

    return 0;
}
#endif

/* @0xffffffc010cd0740 */
static int sndout_input_downmix_enable_info (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = ALSA_SNDOUT_INPUT_NUM * 2;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = ALSA_INTEGER_MAX;
    return 0;
}

/* @0xffffffc010cd0250 */
static int sndout_input_downmix_enable_get (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int i = 0;

    for (i = 0; i < ALSA_SNDOUT_INPUT_NUM; i++)
    {
        if (i < ALSA_SNDOUT_DEC_NUM)
            ucontrol->value.integer.value[i * 2] = i;
        else
            ucontrol->value.integer.value[i * 2] = i - ALSA_SNDOUT_DEC_NUM;

        ucontrol->value.integer.value[i * 2 + 1] = sndout_info->input_downmix_enable[i];
    }

    return 0;
}

/* @0xffffffc010cd2580 */
static int sndout_input_downmix_enable_put (struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int input = 0, port = 0;
    bool downmix = true;
    int master_index = 0;

    input = ucontrol->value.integer.value[0];
    port = ucontrol->value.integer.value[1];
    downmix = ucontrol->value.integer.value[2];

    debug_call ();

    master_index = common_input_convert_index (input, port);
    if (master_index < 0)
    {
        error ("invalid input(input:0x%x, port:%d)\n", input, port);
        return -(EINVAL);
    }

    /* webOS25-shipped: for a decoder input the flag is also pushed to the
     * decoder ("downmix-out-ch") via alsa_decoder.c (shipped +0x90/+0xfc). */
    if (input == COMMON_INPUT_ADEC)
        decoder_set_downmix (port, downmix);

    sndout_info->input_downmix_enable[master_index] = downmix;

    noti ("input(%d) port(%d) downmix enable(%d)\n", input, port, downmix);

    return 0;
}

/* @0xffffffc010cdc880 */
bool sndout_get_input_downmix_enable (int input, int port)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int master_index = 0;
    bool downmix = true;

    master_index = common_input_convert_index (input, port);
    if (master_index < 0)
    {
        error ("invalid input(input:0x%x, port:%d)\n", input, port);
        return downmix;
    }

    downmix = sndout_info->input_downmix_enable[master_index];

    return downmix;
}

static const struct snd_kcontrol_new sndout_controls[] = {
    /* *INDENT-OFF* */
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_OPEN,
            .info = sndout_open_info,
            .get = sndout_open_get,
            .put = sndout_open,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_CLOSE,
            .info = sndout_open_info,
            .get = sndout_close_get,
            .put = sndout_close,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_CONNECT,
            .info = sndout_connect_info,
            .get = sndout_connect_get,
            .put = sndout_connect,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_DISCONNECT,
            .info = sndout_disconnect_info,
            .get = sndout_disconnect_get,
            .put = sndout_disconnect,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = GAIN_INPUT_DURATION,
            .info = sndout_gain_duration_info,
            .get = sndout_gain_duration_get,
            .put = sndout_gain_duration,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = GAIN_INPUT,
            .info = sndout_gain_input_info,
            .get = sndout_gain_input_get,
            .put = sndout_gain_input,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = GAIN_OUTPUT,
            .info = sndout_gain_output_info,
            .get = sndout_gain_output_get,
            .put = sndout_gain_output,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = GAIN_AUDIODESCRIPTION,
            .info = sndout_gain_audio_desc_info,
            .get = sndout_gain_audio_desc_get,
            .put = sndout_gain_audio_desc,
        },

    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = MUTE_INPUT,
            .info = sndout_mute_input_info,
            .get = sndout_mute_input_get,
            .put = sndout_mute_input,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = MUTE_OUTPUT,
            .info = sndout_mute_output_info,
            .get = sndout_mute_output_get,
            .put = sndout_mute_output,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = DELAY_INPUTOUTPUT,
            .info = sndout_delay_inoutput_info,
            .get = sndout_delay_inoutput_get,
            .put = sndout_delay_inoutput,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_MAINAUDIO_OUTPUT,
            .info = sndout_mainaudio_output_info,
            .get = sndout_mainaudio_output_get,
            .put = sndout_mainaudio_output,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_SPDIF_OUTPUTTYPE,
            .info = sndout_spdif_output_type_info,
            .get = sndout_spdif_output_type_get,
            .put = sndout_spdif_output_type,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_SPDIF_CATERGORYCODE,
            .info = sndout_spdif_category_code_info,
            .get = sndout_spdif_category_code_get,
            .put = sndout_spdif_category_code,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_SPDIF_COPYPROTECTIONINFO,
            .info = sndout_spdif_copy_protection_info,
            .get = sndout_spdif_copy_protection_get,
            .put = sndout_spdif_copy_protection,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_OPTIC_LIGHTONOFF,
            .info = sndout_optic_light_onoff_info,
            .get = sndout_optic_light_onoff_get,
            .put = sndout_optic_light_onoff,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_ARC_ONOFF,
            .info = sndout_arc_onoff_info,
            .get = sndout_arc_onoff_get,
            .put = sndout_arc_onoff,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_OPTIC_LG,
            .info = sndout_optic_lg_info,
            .get = sndout_optic_lg_get,
            .put = sndout_optic_lg,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_SPK_OUTPUT,
            .info = sndout_spk_output_info,
            .get = sndout_spk_output_get,
            .put = sndout_spk_output,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_EARC_ONOFF,
            .info = sndout_earc_onoff_info,
            .get = sndout_earc_onoff_get,
            .put = sndout_earc_onoff,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_EARC_OUTPUT_TYPE,
            .info = sndout_earc_output_type_info,
            .get = sndout_earc_output_type_get,
            .put = sndout_earc_output_type,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = DOLBY_MS12_IIDK_VERSION,
            .info = dolby_ms12_iidk_version_info,
            .get = dolby_ms12_iidk_version_get,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_CAPTURE_INPUT_TIMECLOCK,
            .info = sndout_timecount_info,
            .get = sndout_timecount_get,
            .access = SNDRV_CTL_ELEM_ACCESS_READ,
        },
    {
             .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
             .name = SNDOUT_CAPTURE_OUTPUT_RESAMPLING,
             .info = sndout_resampling_info,
             .get = sndout_resampling_get,
             .put = sndout_resampling_put,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_EAC3_ATMOS_ENCODE_ONOFF,
            .info = sndout_eac3_atmos_encode_onoff_info,
            .get = sndout_eac3_atmos_encode_onoff_get,
            .put = sndout_eac3_atmos_encode_onoff_put,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = AUDIO_LATENCY_TIME,
            .info = sndout_audio_latency_time_info,
            .get = sndout_audio_latency_time_get,
            .put = sndout_audio_latency_time_put,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
#ifndef SNDOUT_AUDIO_CAPTURE_PATH
#define SNDOUT_AUDIO_CAPTURE_PATH "Sndout Audio Capture Path"
#endif
            .name = SNDOUT_AUDIO_CAPTURE_PATH,
            .info = sndout_capture_path_info,
            .get = sndout_capture_path_get,
            .put = sndout_capture_path_put,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_ARC_OUTPUT_TYPE,
            .info = sndout_arc_output_type_info,
            .get = sndout_arc_output_type_get,
            .put = sndout_arc_output_type,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_OPTIC_OUTPUT_TYPE,
            .info = sndout_optic_output_type_info,
            .get = sndout_optic_output_type_get,
            .put = sndout_optic_output_type,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_WOWCAST_ONOFF,
            .info = sndout_wowcast_onoff_info,
            .get = sndout_wowcast_onoff_get,
            .put = sndout_wowcast_onoff_put,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_WOWCAST_OUTPUT_TYPE,
            .info = sndout_wowcast_output_type_info,
            .get = sndout_wowcast_output_type_get,
            .put = sndout_wowcast_output_type_put,
        },
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = LOW_LATENCY_MODE,
            .info = sndout_low_latency_mode_info,
            .get = sndout_low_latency_mode_get,
            .put = sndout_low_latency_mode_put,
        },

#if (MS12_VER >= 26)
    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = SNDOUT_EARC_MS12_SP50_ONOFF,
            .info = sndout_earc_ms12_sp50_onoff_info,
            .get = sndout_earc_ms12_sp50_onoff_get,
            .put = sndout_earc_ms12_sp50_onoff_put,
        },
#endif

    {
            .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
            .name = INPUT_DOWNMIX_ENABLE,
            .info = sndout_input_downmix_enable_info,
            .get = sndout_input_downmix_enable_get,
            .put = sndout_input_downmix_enable_put,
        },
    /* *INDENT-ON* */
};

/* @0xffffffc010cdc930 */
int sndout_get_input_info (int index, int *gain, int *mute, int *delay)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];

    if (index < 0 || index >= ALSA_SNDOUT_INPUT_NUM)
    {
        error ("invalid index(%d)\n", index);
        return -1;
    }

    if (gain)
        *gain = sndout_info->param_input[index].gain;
    if (mute)
        *mute = sndout_info->param_input[index].mute;
    if (delay)
        *delay = sndout_info->param_input[index].delay;

    return 0;
}

/* @0xffffffc010cdca20 */
int sndout_get_gain_duration_info (int index, int *gain, int *duration)
{
    if (index < 0 || index >= ALSA_SNDOUT_INPUT_NUM)
    {
        error ("invalid index(%d)\n", index);
        return -1;
    }

    if (gain)
        *gain = gAlsaSndoutGainDurationInfo[index].gain;
    if (duration)
        *duration = gAlsaSndoutGainDurationInfo[index].duration;

    return 0;
}

/*
   to fix pop noise of speaker output when changing se mode.
*/
/* @0xffffffc010cdcae0 */
void sndout_speaker_onoff (bool on)
{
    struct alsa_sndout_list_output *list_output = NULL;
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    ALSA_SNDOUT_OUTPUT_TYPE_T sndout_output = ALSA_SNDOUT_OUTPUT_MAX;

    SNDOUT_LOCK ();

    if (sndout_info->isOpen != true)
    {
        SNDOUT_UNLOCK ();
        return;
    }

    sndout_output = common_output_convert_sndout_output (COMMON_SPK);

    list_output = output_get (COMMON_SPK);
    if (list_output)
    {
        if (on)
        {
            output_set_delay (list_output->output_info,
                sndout_info->param_output[sndout_output].delay);
            output_set_mute (list_output->output_info,
                sndout_info->param_output[sndout_output].mute);
        }
        else
        {
            output_set_mute (list_output->output_info, true);
            output_set_delay (list_output->output_info, 0);
        }
    }

    SNDOUT_UNLOCK ();
}

/* @0xffffffc010cd0410 */
static int _proc_amixer_status_show (struct seq_file *m, void *data)
{
    char renOpened[64];
    char openedIndex[8];
    int i;

    renOpened[0] = '\0';
    openedIndex[0] = '\0';

    // check render opend
    for (i = 0; i < ALSA_SNDOUT_MIX_NUM; i++)
    {
        struct aud_block_info *ren_info;

        ren_info = bm_get_block_info_by_index ("renderer", i + ALSA_SNDOUT_DEC_NUM);
        if (!ren_info)
            continue;

        if (renOpened[0] != '\0')
            sprintf (openedIndex, ",%d", i);
        else
            sprintf (openedIndex, "%d", i);

        strcat (renOpened, openedIndex);
    }

    seq_printf (m, "openedPort %s\n", renOpened);

    return 0;
}


/* @0xffffffc010cd03e4 */
static int _proc_amixer_status_open (struct inode *inode, struct file *file)
{
    int ret;

    ret = single_open (file, _proc_amixer_status_show, NULL);
    if (ret < 0)
        return ret;

    return ret;
}

static const char *_get_copy_protection_string (int copy_protection)
{
    switch (copy_protection)
    {
        case SNDOUT_OPTIC_COPY_FREE:
            return "FREE";
        case SNDOUT_OPTIC_COPY_NO_MORE:
            return "NO_MORE";
        case SNDOUT_OPTIC_COPY_ONCE:
            return "ONCE";
        case SNDOUT_OPTIC_COPY_NEVER:
            return "NEVER";
        default:
            return "UNKNOWN";
    }
}

/* @0xffffffc010cd70e0 */
static int _proc_sndout_status_show (struct seq_file *m, void *data)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    struct alsa_sndout_list_output *list_output = NULL;
    struct sndout_output_format *set_output = NULL;
    struct alsa_sndout_list_connect *sndoutConnectInfo = NULL;
    int main_input_delay = 0;
    int main_audio_index = 0;
    unsigned int opticCh = 0;
    unsigned int arcCh = 0;
    char mainOutput[64];
    char decConnected[64];
    char renConnected[64];
    char connectedIndex[8];
    unsigned int underflowCount = 0;

    const char *opticOutType[] = {
        "NONE", "PCM", "AUTO", "AUTO_AAC", "HALF_AUTO", "HALF_AUTO_AAC", "FORCED_AC3_5_1", "BYPASS",
        "BYPASS_AAC"
    };
    const char *arcOutType[] = {
        "NONE", "PCM", "AUTO", "AUTO_AAC", "AUTO_EAC3", "AUTO_EAC3_AAC", "HALF_AUTO",
        "HALF_AUTO_AAC", "HALF_AUTO_EAC3", "HALF_AUTO_EAC3_AAC", "FORCED_AC3", "FORCED_EAC3",
        "BYPASS", "BYPASS_AAC", "BYPASS_EAC3", "BYPASS_EAC3_AAC", "BYPASS_EAC3_MPEG4_AAC"
    };
    const char *opticArcOuputCode[] = {
        "PCM", "DD", "DDP", "AAC", "MAT"
    };

    main_audio_index =
        common_input_convert_index (sndout_info->main_audio_input, sndout_info->main_audio_port);
    sndout_get_input_info (main_audio_index, NULL, NULL, &main_input_delay);

    seq_printf (m, "[SPK]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_SPK ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_SPK) ? 1 : 0);
    seq_printf (m, "SpkOutputChannel=%u\n", sndout_info->spk_output_type);
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_SPK].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_SPK].mute);
    seq_printf (m, "Delay=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_SPK].delay);
    seq_printf (m, "ProcessingDelay=%d\n",
        main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_SPK].delay);
    underflowCount = 0;
    list_output = output_get (COMMON_SPK);
    if (list_output)
        output_get_underflow (list_output->output_info, &underflowCount);
    seq_printf (m, "Underflow=%d\n", underflowCount);
    seq_printf (m, "\n");

    seq_printf (m, "[OPTIC]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_OPTIC ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_OPTIC) ? 1 : 0);
    seq_printf (m, "Light=%d\n", sndout_info->optic_light_on);
    seq_printf (m, "OpticMode=%s\n", opticOutType[sndout_info->optic_output_type]);
    list_output = output_get (COMMON_OPTIC);
    if (list_output)
    {
        set_output = &list_output->set_output;

        if (set_output->bypass)
            seq_printf (m, "OutputCodec=%s\n", "Pass-Through");
        else
            seq_printf (m, "OutputCodec=%s\n", opticArcOuputCode[set_output->output_codec]);
    }
    else
        seq_printf (m, "OutputCodec=%s\n", "PCM");
    // TODO ???
    seq_printf (m, "OutputChannel=%u\n", opticCh);
    seq_printf (m, "SrcCopyProtection=%s\n",
        _get_copy_protection_string (sndout_info->optic_copy_protection));
    seq_printf (m, "OutputCopyProtection=%s\n",
        _get_copy_protection_string (sndout_info->optic_copy_protection));
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC].mute);
    seq_printf (m, "Delay=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC].delay);
    seq_printf (m, "Set_type=%d\n", sndout_info->optic.set_type);
    seq_printf (m, "Codec_type=%d\n", sndout_info->optic.codec_type);
    seq_printf (m, "Channel_num=%d\n", sndout_info->optic.channel_num);
    seq_printf (m, "Sample_rate=%d\n", sndout_info->optic.sample_rate);
    seq_printf (m, "Mix_option=%d\n", sndout_info->optic.mix_option);
    if (list_output)
    {
        set_output = &list_output->set_output;

        if (set_output->bypass)
            seq_printf (m, "ProcessingDelay=%d\n",
                main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC].delay);
        else
        {
            if (set_output->output_codec == OUTPUT_CODEC_PCM)
                seq_printf (m, "ProcessingDelay=%d\n",
                    main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC].delay);
            else
                seq_printf (m, "ProcessingDelay=%d\n",
                    main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC].delay +
                    encoder_processing_delay[set_output->output_codec]);
        }
    }
    else
        seq_printf (m, "ProcessingDelay=%d\n",
            main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC].delay);
    underflowCount = 0;
    if (list_output)
        output_get_underflow (list_output->output_info, &underflowCount);
    seq_printf (m, "Underflow=%d\n", underflowCount);
    seq_printf (m, "\n");

    seq_printf (m, "[OPTIC_LG]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_OPTIC_LG ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_OPTIC_LG) ? 1 : 0);
    seq_printf (m, "Light=%d\n", sndout_info->optic_light_on);
    seq_printf (m, "OutputCodec=%s\n", "PCM");
    seq_printf (m, "OutputChannel=%d\n", 2);
    seq_printf (m, "SrcCopyProtection=%s\n",
        _get_copy_protection_string (sndout_info->optic_copy_protection));
    seq_printf (m, "OutputCopyProtection=%s\n",
        _get_copy_protection_string (sndout_info->optic_copy_protection));
    seq_printf (m, "SoundBarID=0x%X\n", sndout_info->soundbar.id);
    seq_printf (m, "SoundBarVolume=%d\n", sndout_info->soundbar.volume);
    seq_printf (m, "SoundBarMute=%d\n", sndout_info->soundbar.mute);
    seq_printf (m, "SoundBarPower=%d\n", sndout_info->soundbar.power_on);
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC_LG].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC_LG].mute);
    seq_printf (m, "Delay=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC_LG].delay);
    seq_printf (m, "ProcessingDelay=%d\n",
        main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_OPTIC_LG].delay);
    list_output = output_get (COMMON_OPTIC_LG);
    underflowCount = 0;
    if (list_output)
        output_get_underflow (list_output->output_info, &underflowCount);
    seq_printf (m, "Underflow=%d\n", underflowCount);
    seq_printf (m, "\n");

    seq_printf (m, "[ARC]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_ARC ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_ARC) ? 1 : 0);
    seq_printf (m, "ArcOnOff=%d\n", sndout_info->arc_on);
    seq_printf (m, "ArcMode=%s\n", arcOutType[sndout_info->arc_output_type]);
    list_output = output_get (COMMON_ARC);
    if (list_output)
    {
        set_output = &list_output->set_output;

        if (set_output->bypass)
            seq_printf (m, "OutputCodec=%s\n", "Pass-Through");
        else
            seq_printf (m, "OutputCodec=%s\n", opticArcOuputCode[set_output->output_codec]);
    }
    else
        seq_printf (m, "OutputCodec=%s\n", "PCM");
    // TODO ???
    seq_printf (m, "OutputChannel=%u\n", arcCh);
    seq_printf (m, "SrcCopyProtection=%s\n",
        _get_copy_protection_string (sndout_info->arc_copy_protection));
    seq_printf (m, "OutputCopyProtection=%s\n",
        _get_copy_protection_string (sndout_info->arc_copy_protection));
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_ARC].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_ARC].mute);
    seq_printf (m, "Delay=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_ARC].delay);
    seq_printf (m, "Set_type=%d\n", sndout_info->arc.set_type);
    seq_printf (m, "Codec_type=%d\n", sndout_info->arc.codec_type);
    seq_printf (m, "Channel_num=%d\n", sndout_info->arc.channel_num);
    seq_printf (m, "Sample_rate=%d\n", sndout_info->arc.sample_rate);
    seq_printf (m, "Mix_option=%d\n", sndout_info->arc.mix_option);
    if (list_output)
    {
        set_output = &list_output->set_output;

        if (set_output->bypass)
            seq_printf (m, "ProcessingDelay=%d\n",
                main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_ARC].delay);
        else
        {
            if (set_output->output_codec == OUTPUT_CODEC_PCM)
                seq_printf (m, "ProcessingDelay=%d\n",
                    main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_ARC].delay);
            else
                seq_printf (m, "ProcessingDelay=%d\n",
                    main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_ARC].delay +
                    encoder_processing_delay[set_output->output_codec]);
        }
    }
    else
        seq_printf (m, "ProcessingDelay=%d\n",
            main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_ARC].delay);
    underflowCount = 0;
    if (list_output)
        output_get_underflow (list_output->output_info, &underflowCount);
    seq_printf (m, "Underflow=%d\n", underflowCount);
    seq_printf (m, "\n");

    seq_printf (m, "[EARC]\n");
    seq_printf (m, "EArcOnOff=%s\n", sndout_info->earc_on ? "On" : "Off");
    seq_printf (m, "Set_type=%d\n", sndout_info->earc.set_type);
    seq_printf (m, "Codec_type=%d\n", sndout_info->earc.codec_type);
    seq_printf (m, "Channel_num=%d\n", sndout_info->earc.channel_num);
    seq_printf (m, "Sample_rate=%d\n", sndout_info->earc.sample_rate);
    seq_printf (m, "Mix_option=%d\n", sndout_info->earc.mix_option);
    seq_printf (m, "Underflow=%d\n", underflowCount);
    seq_printf (m, "\n");

    seq_printf (m, "[BLUETOOTH]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_BLUETOOTH ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_BLUETOOTH) ? 1 : 0);
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_BLUETOOTH].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_BLUETOOTH].mute);
    seq_printf (m, "Delay=NA\n");
    seq_printf (m, "ProcessingDelay=%d\n", main_input_delay);
    seq_printf (m, "\n");

    seq_printf (m, "[HP]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_HP ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_HP) ? 1 : 0);
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_HP].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_HP].mute);
    seq_printf (m, "Delay=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_HP].delay);
    seq_printf (m, "ProcessingDelay=%d\n",
        main_input_delay + sndout_info->param_output[ALSA_SNDOUT_OUTPUT_HP].delay);
    list_output = output_get (COMMON_HP);
    underflowCount = 0;
    if (list_output)
        output_get_underflow (list_output->output_info, &underflowCount);
    seq_printf (m, "Underflow=%d\n", underflowCount);
    seq_printf (m, "\n");

    seq_printf (m, "[WISA]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_WISA ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_WISA) ? 1 : 0);
    seq_printf (m, "OutputChannel=%d\n", sndout_info->opened_output & COMMON_WISA ? 6 : 0);
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_WISA].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_WISA].mute);
    seq_printf (m, "Delay=NA\n");
    seq_printf (m, "ProcessingDelay=%d\n", main_input_delay);
    seq_printf (m, "\n");

    seq_printf (m, "[SE_BT]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_SE_BT ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_SE_BT) ? 1 : 0);
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_SE_BT].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_SE_BT].mute);
    seq_printf (m, "Delay=NA\n");
    seq_printf (m, "ProcessingDelay=%d\n", main_input_delay);
    seq_printf (m, "\n");

    seq_printf (m, "[SE_BT_SUR]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_SE_BT_SUR ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_SE_BT_SUR) ? 1 : 0);
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_SE_BT_SUR].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_SE_BT_SUR].mute);
    seq_printf (m, "Delay=NA\n");
    seq_printf (m, "ProcessingDelay=%d\n", main_input_delay);
    seq_printf (m, "\n");

    seq_printf (m, "[DAP_HP_BT]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_DAP_HP_BT ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_DAP_HP_BT) ? 1 : 0);
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_DAP_HP_BT].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_DAP_HP_BT].mute);
    seq_printf (m, "Delay=NA\n");
    seq_printf (m, "ProcessingDelay=%d\n", main_input_delay);
    seq_printf (m, "\n");

    seq_printf (m, "[COMMON_CAPTURE]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_CAPTURE ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_CAPTURE) ? 1 : 0);
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_CAPTURE].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_CAPTURE].mute);
    seq_printf (m, "Delay=NA\n");
    seq_printf (m, "ProcessingDelay=%d\n", main_input_delay);
    seq_printf (m, "\n");

    seq_printf (m, "[SE_ARC]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_SE_ARC ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_SE_ARC) ? 1 : 0);
    seq_printf (m, "ArcOnOff=%d\n", sndout_info->arc_on);
    seq_printf (m, "ArcMode=%s\n", arcOutType[sndout_info->arc_output_type]);
    seq_printf (m, "OutputCopyProtection=%s\n",
        _get_copy_protection_string (sndout_info->arc_copy_protection));
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_SE_ARC].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_SE_ARC].mute);
    seq_printf (m, "Delay=NA\n");
    seq_printf (m, "ProcessingDelay=%d\n", main_input_delay);
    seq_printf (m, "\n");

    seq_printf (m, "[WOWCAST]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_WOWCAST ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_WOWCAST) ? 1 : 0);
    seq_printf (m, "WowcastOnOff=%d\n", sndout_info->wowcast_on);
    seq_printf (m, "OutputCopyProtection=%s\n",
        _get_copy_protection_string (sndout_info->arc_copy_protection));
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_WOWCAST].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_WOWCAST].mute);
    seq_printf (m, "Delay=NA\n");
    seq_printf (m, "ProcessingDelay=%d\n", main_input_delay);
    seq_printf (m, "Set_type=%d\n", sndout_info->wowcast.set_type);
    seq_printf (m, "Codec_type=%d\n", sndout_info->wowcast.codec_type);
    seq_printf (m, "Channel_num=%d\n", sndout_info->wowcast.channel_num);
    seq_printf (m, "Sample_rate=%d\n", sndout_info->wowcast.sample_rate);
    seq_printf (m, "Mix_option=%d\n", sndout_info->wowcast.mix_option);
    seq_printf (m, "\n");

#ifdef SUPPORT_DAFC
    seq_printf (m, "[DAFC]\n");
    seq_printf (m, "Open=%d\n", sndout_info->opened_output & COMMON_DAFC ? 1 : 0);
    seq_printf (m, "Connect=%d\n", _output_connected_any (COMMON_DAFC) ? 1 : 0);
    seq_printf (m, "Gain=0x%X\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_DAFC].gain);
    seq_printf (m, "Mute=%d\n", sndout_info->param_output[ALSA_SNDOUT_OUTPUT_DAFC].mute);
    seq_printf (m, "Delay=NA\n");
    seq_printf (m, "ProcessingDelay=%d\n", main_input_delay);
    seq_printf (m, "\n");
#endif

    decConnected[0] = '\0';
    renConnected[0] = '\0';
    connectedIndex[0] = '\0';

    // check if decoder is connected
    list_for_each_entry (sndoutConnectInfo, &sndout_info->list_connection, list)
    {
        if (sndoutConnectInfo->input == COMMON_INPUT_ADEC)
        {
            if (decConnected[0] != '\0')
                sprintf (connectedIndex, ",%d", sndoutConnectInfo->port);
            else
                sprintf (connectedIndex, "%d", sndoutConnectInfo->port);

            strcat (decConnected, connectedIndex);
        }
    }

    // check if renderer is connected
    list_for_each_entry (sndoutConnectInfo, &sndout_info->list_connection, list)
    {
        if (sndoutConnectInfo->input == COMMON_INPUT_AMIXER)
        {
            if (renConnected[0] != '\0')
                sprintf (connectedIndex, ",%d", sndoutConnectInfo->port);
            else
                sprintf (connectedIndex, "%d", sndoutConnectInfo->port);

            strcat (renConnected, connectedIndex);
        }
    }


    // get main output port
    if (main_audio_index < ALSA_SNDOUT_DEC_NUM)
        sprintf (mainOutput, "ADEC%d", main_audio_index);
    else
        sprintf (mainOutput, "AMIXER%d", main_audio_index - ALSA_SNDOUT_DEC_NUM);

    seq_printf (m, "[Misc]\n");
    seq_printf (m, "eAC3 ATMOS Encode On=%d\n", sndout_info->eac3_atmos_encode_on);
    seq_printf (m, "ConnectedAdec=%s\n", decConnected);
    seq_printf (m, "ConnectedAmixer=%s\n", renConnected);
    seq_printf (m, "MainAudioOutput=%s\n", mainOutput);
    seq_printf (m, "MainAudioInput=%d\n", sndout_info->main_audio_input);
    seq_printf (m, "MainAudioPort=%d\n", sndout_info->main_audio_port);
    /* webOS25-shipped */
    seq_printf (m, "ConnectedMixerPort=0x%x\n", sndout_info->mixer_connected_output);
    seq_printf (m, "\n");

    return 0;
}

/* @0xffffffc010cd03c0 */
static int _proc_sndout_status_open (struct inode *inode, struct file *file)
{
    int ret;

    ret = single_open (file, _proc_sndout_status_show, NULL);
    if (ret < 0)
        return ret;

    return ret;
}

static struct file_operations _proc_amixer_status_fops = {
    .open = _proc_amixer_status_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static struct file_operations _proc_sndout_status_fops = {
    .open = _proc_sndout_status_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

extern struct proc_dir_entry *aproc_seetv;

/* @0xffffffc010cdcbd0 */
int register_sndout_kcontrols (struct snd_soc_card *card)
{
    ALSA_SNDOUT_INFO_T *sndout_info = &alsa_sndout_info[0];
    int ret = 0;
    int i = 0;

    ret = snd_soc_add_card_controls (card, sndout_controls, ARRAY_SIZE (sndout_controls));
    if (ret != RET_OK)
    {
        error ("add control is failed (ret %d)\n", ret);
        return ret;
    }

    for (i = 0; i < ALSA_SNDOUT_INPUT_NUM; i++)
    {
        sndout_info->param_input[i].gain = 0x800000;
        sndout_info->param_input[i].mute = 0;
        sndout_info->param_input[i].delay = 0;

    }

    for (i = 0; i < ALSA_SNDOUT_OUTPUT_MAX; i++)
    {
        sndout_info->param_output[i].gain = 0x800000;
        sndout_info->param_output[i].mute = 1;
        sndout_info->param_output[i].delay = 0;

    }

    // create proc node for debug status
    proc_create_data ("amixer", 0440, aproc_seetv, &_proc_amixer_status_fops, NULL);
    proc_create_data ("sndout", 0440, aproc_seetv, &_proc_sndout_status_fops, NULL);

    noti ("completed(ms12 version %d.%d.) \n", ADEC_MS12_MAJOR_VER, ADEC_MS12_MINOR_VER);

    return ret;
}
