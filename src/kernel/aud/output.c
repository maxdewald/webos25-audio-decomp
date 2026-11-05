// SPDX-License-Identifier: GPL-2.0-only
/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2017 by LG Electronics Inc.
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
 * Base: lg-opensource/kdriver/core/aud/output.c (GPL-2.0).
 * aud_output_info layout in the shipped image: output_type +0x2c,
 * uid_outctrl +0x30, uid_out +0x34, mute +0x38, volume +0x3c, delay +0x40,
 * codec +0x44, channel +0x48, sample_rate +0x4c, acmod +0x50, bypass +0x54,
 * earc_on +0x81, event_id +0x84 (block_info_size 0x88, impl
 * preffered_processor = 3 = ADEC_CORE_DSP1).
 *
 * Verification depth: _set_format, _cb_output_status, output_set_type,
 * output_pre/post_connect, output_pre/post_disconnect, output_set_format,
 * output_set_volume, output_set_earc_on, output_set_on were read instruction
 * by instruction; output_set_mute was also verified against the Image.
 * Other tagged functions were checked only by symbol + strings.
 *
 * webOS25-shipped differences found:
 *  - enum output_codec_t / FW_AUD_OUTPUT_CODEC_T stop at MAT: the codec name
 *    table has 6 entries {"pcm","ac3","eac3","aac","mat","???"} and
 *    _adec_codec_convert_output_codec (inlined in _cb_output_status) accepts
 *    only codec <= 4. All DTS codecs (5..9) take the "Not supproted adec codec"
 *    path and are recorded as OUTPUT_CODEC_PCM.
 *  - extra exported helpers output_get_latency / output_get_bc_write_time.
 */


/**
 *  @file output.c
 *
 *  Audio driver implementation for  audio output block.
 *  Output block manages Speaker, Headphone(HP), Optic(SPDIF), and ARC output
 *  (include eARC).
 *
 *  speaker output : 48000/96000Hz rate, 2/4 channels pcm.
 *  out_speaker - pcm
 *
 *  headphone output : only 48000Hz, 2 channels pcm.
 *  out_hp - pcmsub.
 *
 *  spdif output : 32000/44100/48000Hz rate, multi channels, pcm/aac/ac3 codec.
 *  pcm output : out_spdif - spdif
 *  es output : outes_spdif - spdif
 *
 *  arc output : 32000/44100/48000/64000/88200/96000/128000/17400/192000Hz rate,
 *  multi channels, pcm/aac/ac3/eac3/mat codec.
 *  pcm output : out_arc - arc
 *  es output : outes_arc - arc
 *
 *  @author	wonchang.shin (wonchang.shin@lge.com)
 *  @version    0.1
 *  @date       2020/06/29
 *
 *  @addtogroup output
 *  @{
 */
#include <linux/slab.h>

#include "log.h"
#include "block.h"
#include "block_manager.h"
#include "output.h"
#include "clock.h"

#include "kaimc.h"
#include "imc/cmd/module_cmd_spdifout.h"
#include "imc/cmd/module_cmd_arcout.h"
#include "imc/cmd/module_cmd_outctrl.h"
#include "imc/cmd/module_cmd_esoutctrl.h"
#include "imc/evt/module_evt_spdifout.h"
#include "module/adec_module_list.h"

#include "hal/hal_output.h"
#include "hal/hal_clock.h"

#ifndef CHIP_TYPE_FPGA
#ifdef INCLUDE_KDRV_HDMI20
extern void HDMI21_ISRUNIT_Rx_Set_RegisterEarcDmacIsrCallBack (void (*function) (unsigned int
        intStatus));
#endif
#endif

bool gOutputConnectSpdif;

/* @0xffffffc010cb2680 */
void output_earc_dmac_irq (unsigned int intStatus);

logm_define (aud_output, log_level_noti);
#define trace(fmt,args...)  logm_trace(aud_output,fmt,##args)
#define debug(fmt,args...)  logm_debug(aud_output,fmt,##args)
#define info(fmt,args...)   logm_info(aud_output,fmt,##args)
#define noti(fmt,args...)   logm_noti(aud_output,fmt,##args)
#define error(fmt,args...)  logm_error(aud_output,fmt,##args)

static DEFINE_MUTEX (_output_lock);

#define AUD_OUTPUT_LOCK()		mutex_lock(&_output_lock)
#define AUD_OUTPUT_UNLOCK()	mutex_unlock(&_output_lock)

#define OUTPUT_TO_STR(x) (output_type_str[(x)->output_type])

#define CLOCK_TYPE(_out) ( \
            (_out == OUTPUT_SPEAKER) ? CLOCK_OUTPUT_SPEAKER : \
            (_out == OUTPUT_HP) ? CLOCK_OUTPUT_HP : \
            (_out == OUTPUT_OPTIC) ? CLOCK_OUTPUT_OPTIC : \
            (_out == OUTPUT_ARC) ? CLOCK_OUTPUT_ARC : CLOCK_OUTPUT_MAX)

static const char *output_type_str[] = {
    "SPK", "HP", "OPTIC", "ARC", "OTHERS", "MAX", "????", "????", "????", "????", "????", "????"
};

/* webOS25-shipped: table @0xffffffc0115ea500 (output_create-relative +0xe8)
 * holds only these six entries; no DTS names. */
static const char *output_codec_str[] = {
    "pcm", "ac3", "eac3", "aac", "mat", "???"
};


struct sound_bar_status
{
    unsigned int subframe_id;   ///< subframe id.
    unsigned int subframe_data; ///< subframe data.
    unsigned int subframe_checksum;     ///< subframe checksum.
};

/**
 * AUDIO output Information.
 */
struct aud_output_info
{
    struct aud_block_info parent;

    struct list_head list_module;

    bool suspended;
    enum output_type_t output_type;
    int uid_outctrl;
    int uid_out;

    bool mute;
    unsigned int volume;
    unsigned int delay;

    enum output_codec_t codec;
    unsigned int channel;
    unsigned int sample_rate;
    unsigned int acmod;
    unsigned int bypass;

    unsigned int scms;
    unsigned int category_code;
    bool on;
    bool sound_bar_on;
    struct sound_bar_param sound_bar_param;
    struct sound_bar_command sound_bar_command;
    struct sound_bar_status sound_bar_status;
    bool sound_bar_power;

    bool earc_on;
    int event_id;
};

/**
 * Set Output Volume
 *
 * @param type [in] output type
 * @param volume [in] output volume
 * @return int
 * @see
*/
/* @0xffffffc010cb2fa0 */
int output_set_volume (struct aud_block_info *block, unsigned int volume)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (output->codec == OUTPUT_CODEC_PCM)
        aimc_set_property (output->uid_outctrl, "gain=%d", volume);
    else
        aimc_set_property (output->uid_outctrl, "gain=%d", 0x800000);

    output->volume = volume;

    info ("%s volume : 0x%x.\n", OUTPUT_TO_STR (output), volume);

    return 0;
}

/**
 * Set Output Mute
 *
 * @param type [in] output type
 * @param on [in] output mute
 * @return int
 * @see
*/
/* @0xffffffc010cb3070: verified Image +0x18..+0x34; set both DSP mute
 * properties, ignore delivery status, then cache mute (no eARC HAL call). */
int output_set_mute (struct aud_block_info *block, bool on)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    aimc_set_property (output->uid_outctrl, "mute-output=%d\\mute-input=%d", on, on);

    output->mute = on;

    info ("%s mute : %s.\n", OUTPUT_TO_STR (output), on ? "ON" : "OFF");

    return 0;
}

/**
 * Set Output Delay
 *
 * @param type [in] output type
 * @param delay [in] output delay
 * @return int
 * @see
*/
/* @0xffffffc010cb3130 */
int output_set_delay (struct aud_block_info *block, unsigned int delay)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    aimc_set_property (output->uid_outctrl, "delay=%d", delay);

    output->delay = delay;

    info ("%s delay : %d.\n", OUTPUT_TO_STR (output), delay);

    return 0;
}

/* @0xffffffc010cb2694: verified Image; clock first, then ARC mute/channel/
 * format/unmute/on; cache codec/channel/rate/bypass, but not acmod. */
static void _set_format (struct aud_output_info *output, enum output_codec_t codec,
    unsigned int channel, unsigned int acmod, unsigned int sample_rate, bool bypass)
{
    aud_clock_update_output (output->output_type, sample_rate, bypass);

    if (output->output_type == OUTPUT_ARC)
    {
        audio_hal_earc_set_mute (true);

        audio_hal_earc_set_channel (channel, acmod);

        if (codec == OUTPUT_CODEC_PCM)
            audio_hal_earc_set_output_format (AUDIO_HAL_OUTPUT_PCM, sample_rate);
        else
            audio_hal_earc_set_output_format (AUDIO_HAL_OUTPUT_ES, sample_rate);

        audio_hal_earc_set_mute (false);

        audio_hal_earc_set_on (output->earc_on);
    }

    output->codec = codec;
    output->channel = channel;
    output->sample_rate = sample_rate;
    output->bypass = bypass;

    return;
}

/**
 * Set Output Format about codec, channel, sample_rate.
 *
 * @param block [in] output block information
 * @param channel [in] output codec
 * @param channel [in] output channel
 * @param sample_rate [in] output sample_rate
 * @return int
 * @see
*/
/* @0xffffffc010cb3230 */
int output_set_format (struct aud_block_info *block, enum output_codec_t codec,
    unsigned int channel, unsigned int sample_rate, bool bypass)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    //_set_format (output, codec, channel, output->acmod, sample_rate, bypass);

    noti ("%s %d, %d, %d %d.\n", OUTPUT_TO_STR (output), codec, channel, sample_rate, bypass);

    return 0;
}

/**
 * Set Output SCMS
 *
 * @param type [in] output type
 * @param scms [in] output scms
 * @return int
 * @see
*/
/* @0xffffffc010cb32e0 */
int output_set_scms (struct aud_block_info *block, unsigned int scms)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (output->output_type == OUTPUT_OPTIC)
    {
        SpdifCmdSetScms set_spdif_scms = { };

        set_spdif_scms.scms_type = scms;
        aimc_send_command (NULL, SPDIF_CMD_SET_SCMS, 0, output->uid_out, &set_spdif_scms,
            sizeof (set_spdif_scms));
    }
    else if (output->output_type == OUTPUT_ARC)
    {
        ArcCmdSetScms set_arc_scms = { };

        set_arc_scms.scms_type = scms;
        aimc_send_command (NULL, ARC_CMD_SET_SCMS, 0, output->uid_out, &set_arc_scms,
            sizeof (set_arc_scms));

        audio_hal_earc_set_scms (scms);
    }
    else
    {
        debug ("%s is not supported.\n", OUTPUT_TO_STR (output));
        return -1;
    }

    output->scms = scms;

    noti ("%s %x\n", OUTPUT_TO_STR (output), scms);

    return 0;
}

/**
 * Set Output Category Code
 *
 * @param type [in] output type
 * @param category_code [in] output category code
 * @return int
 * @see
*/
/* @0xffffffc010cb3440 */
int output_set_category_code (struct aud_block_info *block, unsigned int category_code)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (output->output_type == OUTPUT_OPTIC)
    {
        SpdifCmdSetCategoryCode set_spdif_category_code = { };

        set_spdif_category_code.category_code = category_code;
        aimc_send_command (NULL, SPDIF_CMD_SET_CATEGORY_CODE, 0, output->uid_out,
            &set_spdif_category_code, sizeof (set_spdif_category_code));
    }
    else if (output->output_type == OUTPUT_ARC)
    {
        ArcCmdSetCategoryCode set_arc_category_code = { };

        set_arc_category_code.category_code = category_code;
        aimc_send_command (NULL, ARC_CMD_SET_CATEGORY_CODE, 0, output->uid_out,
            &set_arc_category_code, sizeof (set_arc_category_code));

        audio_hal_earc_set_category_code (category_code);
    }
    else
    {
        debug ("%s is not supported.\n", OUTPUT_TO_STR (output));
        return -1;
    }

    output->category_code = category_code;

    noti ("%s %x\n", OUTPUT_TO_STR (output), category_code);

    return 0;
}

/**
 * Set Output On/Off
 *
 * @param type [in] output type
 * @param on [in] output on
 * @return int
 * @see
*/
/* @0xffffffc010cb35a0 */
int output_set_on (struct aud_block_info *block, bool on)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    output->on = on;

    if (output->output_type == OUTPUT_OPTIC)
    {
        SpdifCmdSetLight set_light = { };

        set_light.onoff = on;
        aimc_send_command (NULL, SPDIF_CMD_SET_LIGHT, 0, output->uid_out, &set_light,
            sizeof (set_light));

        aud_hal_output_optic_light_on (on);
    }
    else if (output->output_type == OUTPUT_ARC)
        ;
    else
    {
        debug ("%s is not supported.\n", OUTPUT_TO_STR (output));
        return -1;
    }

    noti ("%s %s\n", OUTPUT_TO_STR (output), on ? "on" : "off");

    return 0;
}

/**
 * Set Sound Bar On/Off
 *
 * @param on [in] sound bar on
 * @return int
 * @see
*/
/* @0xffffffc010cb36f0 */
int output_set_sound_bar_on (struct aud_block_info *block, bool on)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (on == false)
    {
        aimc_send_command (NULL, SPDIF_CMD_CLEAR_FMT_FOR_SOUNDBAR, 0, output->uid_out, NULL, 0);
        memset (&(output->sound_bar_param), 0, sizeof (struct sound_bar_param));
    }

    output->sound_bar_on = on;

    noti ("soundbar %s.\n", on ? "ON" : "OFF");

    return 0;
}

/* @0xffffffc010cb2be0 */
static int _cb_sound_bar_status (void *_param, int _paramLen, void *_cbParam)
{
    struct aud_output_info *output = (struct aud_output_info *) _cbParam;
    SpdifEvtGetFmtForSoundbar *pSoundBarStatus = (SpdifEvtGetFmtForSoundbar *) _param;

    if (_paramLen != sizeof (SpdifEvtGetFmtForSoundbar))
    {
        error ("Param Length Error[Expected:%zd][Input:%d]\n", sizeof (SpdifEvtGetFmtForSoundbar),
            _paramLen);
        return -1;
    }

    output->sound_bar_status.subframe_id = pSoundBarStatus->id;
    output->sound_bar_status.subframe_data = pSoundBarStatus->data;
    output->sound_bar_status.subframe_checksum = pSoundBarStatus->checksum;
    output->sound_bar_status.subframe_data |= (pSoundBarStatus->reserved << 16);

    noti ("id(0x%x), data(0x%x), res(0x%x), checksum(0x%x)\n",
        pSoundBarStatus->id, pSoundBarStatus->data, pSoundBarStatus->reserved,
        pSoundBarStatus->checksum);
    debug ("subframe_data(0x%x)\n", output->sound_bar_status.subframe_data);

    return 0;
}

/**
 * Set Sound Bar Parameters
 *
 * @param param [in] sound bar parameters
 * @return int
 * @see
*/
/* @0xffffffc010cb37e0 */
int output_set_sound_bar_param (struct aud_block_info *block, struct sound_bar_param *param)
{
    struct aud_output_info *output = (struct aud_output_info *) block;
    SpdifCmdSetFmtForSoundbar setSoundbarFormat = { };

    setSoundbarFormat.id = param->id;
    setSoundbarFormat.mute = (param->data >> 7) & 0x1;
    setSoundbarFormat.volume = param->data & 0x7F;
    aimc_send_command (NULL, SPDIF_CMD_SET_FMT_FOR_SOUNDBAR, 0, output->uid_out, &setSoundbarFormat,
        sizeof (SpdifCmdSetFmtForSoundbar));

    memcpy (&(output->sound_bar_param), param, sizeof (struct sound_bar_param));

    noti ("mute:%d vol:%d.\n", setSoundbarFormat.mute, setSoundbarFormat.volume);

    return 0;
}

/**
 * Set Sound Bar Commands
 *
 * @param command [in] sound bar commands
 * @return int
 * @see
*/
/* @0xffffffc010cb38a0 */
int output_set_sound_bar_command (struct aud_block_info *block, struct sound_bar_command *command)
{
#if 0
    struct aud_output_info *output = (struct aud_output_info *) block;
    SpdifCmdSetEqbypassForSoundbar setSoundBarEqBypass;
    SpdifCmdSetWooferlevelForSoundbar setSoundBarWooferLevel;
    SpdifCmdSetWirelessresetForSoundbar setSoundBarWirelessReset;
    aimc_register_named_event (output, _cb_sound_bar_status, output->uid_out, "sound_bar_format",
        false);

    /* Set a Soundbar Command Mode */
    if (command->mode == LX_AUD_MASTER_SB_CMD_AUTO_VOLUME)
    {
        setSoundBarEqBypass.onoff = pSoundbarCmd->ui32CmdParam & 0x1;
        AUDIO_IMC_SendCmdParam (SPDIF_CMD_SET_EQBYPASS_FOR_SOUNDBAR,
            gAdecOutMod[LX_AUD_MASTER_OUTPUT_SPDIF], sizeof (SpdifCmdSetEqbypassForSoundbar),
            &setSoundBarEqBypass);
    }
    else if (command->mode == LX_AUD_MASTER_SB_CMD_WOOFER_LEVEL)
    {
        setSoundBarWooferLevel.level = pSoundbarCmd->ui32CmdParam & 0xF;
        AUDIO_IMC_SendCmdParam (SPDIF_CMD_SET_WOOFERLEVEL_FOR_SOUNDBAR,
            gAdecOutMod[LX_AUD_MASTER_OUTPUT_SPDIF], sizeof (SpdifCmdSetWooferlevelForSoundbar),
            &setSoundBarWooferLevel);
    }
    else if (command->mode == LX_AUD_MASTER_SB_CMD_WIRELESS_RESET)
    {
        setSoundBarWirelessReset.reset = pSoundbarCmd->ui32CmdParam & 0x1;
        AUDIO_IMC_SendCmdParam (SPDIF_CMD_SET_WIRELESSRESET_FOR_SOUNDBAR,
            gAdecOutMod[LX_AUD_MASTER_OUTPUT_SPDIF], sizeof (SpdifCmdSetWirelessresetForSoundbar),
            &setSoundBarWirelessReset);
    }
    else
    {
        error ("(cmd : %d, param : %d)\n", command->mode, command->param);
        return -1;
    }
#endif
    noti ("cmd : %x, param : %x\n", command->mode, command->param);
    return 0;
}

/**
 * Set Sound Bar Power On/Off
 *
 * @param on [in] sound bar power on
 * @return int
 * @see
*/
/* @0xffffffc010cb3914 */
int output_set_sound_bar_power (struct aud_block_info *block, bool on)
{
    struct aud_output_info *output = (struct aud_output_info *) block;
    SpdifCmdOnoffSoundbar setSoundBarPower = { };

    aimc_register_named_event (output, _cb_sound_bar_status, output->uid_out, "sound_bar_format",
        false);

    setSoundBarPower.onoff = on;

    aimc_send_command (NULL, SPDIF_CMD_ONOFF_SOUNDBAR, 0, output->uid_out, &setSoundBarPower,
        sizeof (SpdifCmdOnoffSoundbar));

    output->sound_bar_power = on;

    noti ("power %s.\n", on ? "ON" : "OFF");

    return 0;
}

/**
 * Set Sound Frame On/Off
 *
 * @param on [in] sound bar on
 * @return int
 * @see
*/
/* @0xffffffc010cb39f0 */
int output_set_sound_frame_on (struct aud_block_info *block, bool on)
{
    return 0;
}

/**
 * Set Wisa Speaker On/Off
 *
 * @param on [in] sound bar on
 * @return int
 * @see
*/
/* @0xffffffc010cb3a00 */
int output_set_wisa_on (struct aud_block_info *block, bool on)
{
    return 0;
}

/**
 * Set Blue Tooth Speaker On/Off
 *
 * @param on [in] sound bar on
 * @return int
 * @see
*/
/* @0xffffffc010cb3a10 */
int output_set_bt_on (struct aud_block_info *block, bool on)
{
    return 0;
}

/**
 * Set Blue Tooth Surround Speaker On/Off
 *
 * @param on [in] sound bar on
 * @return int
 * @see
*/
/* @0xffffffc010cb3a20 */
int output_set_bt_surround_on (struct aud_block_info *block, bool on)
{
    return 0;
}

/**
 * Set eARC On/Off
 *
 * @param on [in] earc on
 * @return int
 * @see
*/
/* @0xffffffc010cb3a30 */
int output_set_earc_on (struct aud_block_info *block, bool on)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (output->output_type != OUTPUT_ARC)
    {
        debug ("%s is not supported.\n", OUTPUT_TO_STR (output));
        return -1;
    }

    output->earc_on = on;

    aud_clock_update_output (output->output_type, output->sample_rate, output->bypass);
    audio_hal_earc_set_on (on);

    noti ("earc %s\n", on ? "on" : "off");

    return 0;
}

/* @0xffffffc010cb2680 */
void output_earc_dmac_irq (unsigned int intStatus)
{
    audio_hal_earc_clear_interrupt (intStatus);

    return;
}

/* @0xffffffc010cb3b60 */
int output_set_input_index (struct aud_block_info *block, int index)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    aud_clock_set_input_index (CLOCK_TYPE (output->output_type), index);

    return 0;
}

/*
 * webOS25-shipped: inlined into _cb_output_status (+0x44..+0x88, log line 660).
 * The shipped build has no DTS output codecs: FW codec values 0..4
 * (PCM, AC3, EAC3, AAC, MAT) map 1:1, everything else, including the dsp1
 * output_arc DTS codes 5..9 (dts, dts_hd, dts_hd_ma/dts_ma, dtsx_p1, dtsx_p2,
 * see src/dsp1/output_arc.c codec_from_str), is logged and mapped to PCM.
 */
static enum output_codec_t _adec_codec_convert_output_codec (FW_AUD_OUTPUT_CODEC_T adec_codec)
{
    if ((unsigned int) adec_codec <= FW_AUD_OUTPUT_CODEC_MAT)
        return (enum output_codec_t) adec_codec;

    error ("Not supproted adec codec(%d). default pcm output.\n", adec_codec);
    return OUTPUT_CODEC_PCM;
}

/*
 * "output_status" event from dsp1 output_arc/output_spdif (raised on the first
 * AU after codec/rate/channels/acmod change, payload {codec, channels,
 * data_rate, acmod}).
 *
 * Consequence for DTS ES on eARC (observed in shipped code): a dsp1 report of
 * codec 5..9 becomes OUTPUT_CODEC_PCM here, so if anything differs from the
 * stored state, _set_format() runs with codec == PCM and calls
 * audio_hal_earc_set_output_format(AUDIO_HAL_OUTPUT_PCM, rate)
 * (@0xffffffc010cb2694 +0x80: cbz w22 -> w0 = 0), i.e. the eARC TX DMAC is put
 * in 2/8-ch L-PCM mode with the "audio" channel-status bit, while dsp1 writes
 * IEC 61937 bursts with its own ES/HBR lane setup (HAL_SetArcEsMode).
 * The ARM-side eARC link layer therefore advertises PCM for DTS bitstreams.
 */
/* @0xffffffc010cb2d54: verified Image; ignores payload length, preserves
 * acmod for ES, and indexes the log codec table with the original event codec
 * (+0x10c), not the sanitized codec. Unchanged ARC state still reapplies on. */
static int _cb_output_status (void *_param, int _paramLen, void *_cbParam)
{
    struct aud_output_info *output = (struct aud_output_info *) _cbParam;
    OutputEvtFirstAu *evtFirstAu = (OutputEvtFirstAu *) _param;
    enum output_codec_t pre_codec = output->codec;
    unsigned int pre_channel = output->channel;
    unsigned int pre_sample_rate = output->sample_rate;
    unsigned int pre_acmod = output->acmod;

    if ((output->output_type == OUTPUT_OPTIC) || (output->output_type == OUTPUT_ARC))
    {
        output->codec = _adec_codec_convert_output_codec (evtFirstAu->codec);
        output->channel = evtFirstAu->channel;
        output->sample_rate = evtFirstAu->sample_rate;
        if (output->codec == OUTPUT_CODEC_PCM)
            output->acmod = evtFirstAu->acmod;
        else if (output->codec >= OUTPUT_CODEC_MAX)
        {
            output->channel = 2;
            output->sample_rate = 48000;
            output->acmod = 2;
            error ("invalid codec(%d). default pcm 2ch 48000 output.\n", output->codec);
            output->codec = OUTPUT_CODEC_PCM;
        }
    }
    else
    {
        error ("wrong callback %s.\n", OUTPUT_TO_STR (output));
        return -1;
    }

    if ((pre_codec != output->codec) || (pre_channel != output->channel)
        || (pre_sample_rate != output->sample_rate) || (pre_acmod != output->acmod))
        _set_format (output, output->codec, output->channel, output->acmod, output->sample_rate,
            output->bypass);
    else
    {
        if (output->output_type == OUTPUT_ARC)
            audio_hal_earc_set_on (output->earc_on);
    }

    noti ("%s %s %d ch %d acmod %d Hz.\n", OUTPUT_TO_STR (output),
        output_codec_str[evtFirstAu->codec], evtFirstAu->channel, evtFirstAu->acmod,
        evtFirstAu->sample_rate);

    return 0;
}

/* @0xffffffc010cb3b90 */
int output_set_type (struct aud_block_info *block, enum output_type_t output_type)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    block->isOpened = true;

    output->output_type = output_type;

    block->pipeline = apipeline_new ("output_%s", OUTPUT_TO_STR (output));
    output->uid_outctrl = apipeline_appendf (block->pipeline, "outctrlbin");
    if (output_type != OUTPUT_HP)
        apipeline_prependf (block->pipeline, "queue");
    if (output_type == OUTPUT_SPEAKER)
        output->uid_out = apipeline_appendf (block->pipeline, "output_speaker");
    else if (output_type == OUTPUT_HP)
        output->uid_out = apipeline_appendf (block->pipeline, "output_hp");
    else if (output_type == OUTPUT_OPTIC)
    {
        output->uid_out = apipeline_appendf (block->pipeline, "output_spdif");
        gOutputConnectSpdif = true;
        aud_hal_switch_spdif_clock (true);
    }
    else if (output_type == OUTPUT_ARC)
    {
        output->uid_out = apipeline_appendf (block->pipeline, "output_arc");

#ifndef CHIP_TYPE_FPGA
#ifdef INCLUDE_KDRV_HDMI20
        HDMI21_ISRUNIT_Rx_Set_RegisterEarcDmacIsrCallBack (output_earc_dmac_irq);
#endif
#endif
    }

    if ((output_type == OUTPUT_OPTIC) || (output_type == OUTPUT_ARC))
    {
        if (output->event_id > 0)
            aimc_unregister_named_event (output->event_id);

        output->event_id =
            aimc_register_named_event (output, _cb_output_status, output->uid_out, "output_status",
            true);
        if (output->event_id <= 0)
            error ("fail to register evnet %d\n", output->event_id);
    }

    output_set_mute (block, output->mute);

    // clock info 초기화
    output->channel = 2;
    output->sample_rate = 48000;
    aud_clock_enable_output (CLOCK_TYPE (output->output_type), output->uid_out);

    info ("output : %s.\n", OUTPUT_TO_STR (output));
    return 0;
}

/**
 * create output block
 *
 * @param block [in] context of this block
 * @return pointer of output info
*/
/* @0xffffffc010cb2874 */
int output_create (struct aud_block_info *block)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    info ("create output\n");

    block->isOpened = true;
    output->mute = false;
    output->volume = 0;
    output->suspended = false;

    info ("done\n");
    return 0;
}

/* @0xffffffc010cb3f00 */
int output_get_underflow (struct aud_block_info *block, unsigned int *underflow)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    aimc_get_property (output->uid_out, "underflow-count", underflow);

    info ("%s underflow : 0x%x.\n", OUTPUT_TO_STR (output), *underflow);

    return 0;
}

/**
 * destroy output block
 *
 * @param block [in] context of this block
 * @return void
 * @see aud_output_info
*/
/* @0xffffffc010cb2a40 */
static void output_destroy (struct aud_block_info *block)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    AUD_OUTPUT_LOCK ();

    if (block == NULL)
    {
        error ("output_info is NULL!!\n");
        goto exit;
    }

    if (block->isOpened == false)
    {
        error ("output is already closed.\n");
        goto exit;
    }

    if (output->event_id > 0)
        aimc_unregister_named_event (output->event_id);

    aud_clock_disable_output (CLOCK_TYPE (output->output_type));

    // destroy pipeline (inside modules are destroyed automatically)
    if (block->pipeline)
        apipeline_del (block->pipeline);

    if (output->output_type == OUTPUT_OPTIC)
    {
        gOutputConnectSpdif = false;
        aud_hal_switch_spdif_clock (false);
    }

#ifndef CHIP_TYPE_FPGA
#ifdef INCLUDE_KDRV_HDMI20
    if (output->output_type == OUTPUT_ARC)
        HDMI21_ISRUNIT_Rx_Set_RegisterEarcDmacIsrCallBack (NULL);
#endif
#endif

    // Clear a output resource.
    block->isOpened = false;

  exit:
    AUD_OUTPUT_UNLOCK ();
}

/* @0xffffffc010cb2950 */
static int output_suspend (struct aud_block_info *block)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    output->suspended = true;

    if (output->output_type == OUTPUT_SPEAKER)
        audio_hal_speaker_start (false);
    else if (output->output_type == OUTPUT_HP)
        audio_hal_hp_start (false);
    else if (output->output_type == OUTPUT_OPTIC)
        audio_hal_spdif_start (false);
    else if (output->output_type == OUTPUT_ARC)
        audio_hal_arc_start (false);

    aud_clock_disable_output (CLOCK_TYPE (output->output_type));

    if (block->pipeline)
        apipeline_del (block->pipeline);

    block->pipeline = NULL;

    noti ("suspend output\n");

    return 0;
}

/* @0xffffffc010cb3de0 */
static int output_resume (struct aud_block_info *block)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (output->suspended)
    {
        output_set_type (block, output->output_type);   // 함수내 aud_clock_enable_output() 호출
        output_set_earc_on (block, output->earc_on);
        output_set_on (block, output->on);
        output_set_category_code (block, output->category_code);
        output_set_scms (block, output->scms);
        _set_format (output, output->codec, output->channel, output->acmod, output->sample_rate,
            output->bypass);
        output_set_delay (block, output->delay);
        output_set_mute (block, output->mute);
        output_set_volume (block, output->volume);
    }

    if (output->output_type == OUTPUT_SPEAKER)
        audio_hal_speaker_start (true);

    output->suspended = false;

    noti ("resume output\n");

    return 0;
}

/* @0xffffffc010cb2800: verified Image; input side only. Temporary DSP/HAL
 * mute and zero delay leave cached output->mute/output->delay unchanged. */
static int output_pre_disconnect (struct aud_block_info *block, int port, bool in)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (in)
    {
        aimc_set_property (output->uid_outctrl, "mute-output=%d\\mute-input=%d", true, true);
        if (output->output_type == OUTPUT_ARC)
            audio_hal_earc_set_mute (true);

        aimc_set_property (output->uid_outctrl, "delay=%d", 0);
    }

    return 0;
}


/* @0xffffffc010cb2760: verified Image; input ARC sends STOP (0x10020000). */
static int output_post_disconnect (struct aud_block_info *block, int port, bool in)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (in)
    {
        if (output->output_type == OUTPUT_ARC)
            aimc_send_command (NULL, ADEC_CMD_STOP, 0, output->uid_out, NULL, 0);
    }

    return 0;
}

/* @0xffffffc010cb27b0: verified Image; input ARC sends START (0x10010000). */
static int output_pre_connect (struct aud_block_info *block, int port, bool in)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (in)
    {
        if (output->output_type == OUTPUT_ARC)
            aimc_send_command (NULL, ADEC_CMD_START, 0, output->uid_out, NULL, 0);
    }

    return 0;
}

/* @0xffffffc010cb31d4: verified Image; input side restores cached delay,
 * unmutes ARC HAL, then restores cached DSP mute, irrespective of port. */
static int output_post_connect (struct aud_block_info *block, int port, bool in)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (in)
    {
        output_set_delay (block, output->delay);

        if (output->output_type == OUTPUT_ARC)
            audio_hal_earc_set_mute (false);
        output_set_mute (block, output->mute);
    }

    return 0;
}

static struct audio_block_impl block_impl = {
    .block_info_size = sizeof (struct aud_output_info),
    .block_type = "output",
    .preffered_processor = ADEC_CORE_DSP1,
    .constructor = output_create,
    .destructor = output_destroy,
    .suspend = output_suspend,
    .resume = output_resume,
    .pre_disconnect = output_pre_disconnect,
    .post_disconnect = output_post_disconnect,
    .pre_connect = output_pre_connect,
    .post_connect = output_post_connect,
};

/* @0xffffffc010cb4090 */
void output_initializer (void)
{
    bm_block_registry (&block_impl);
}

/* @0xffffffc010cb40b0 */
int output_get_format (struct aud_block_info *block, struct output_format *output_format)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (!output_format)
    {
        error ("output_format is NULL.\n");
        return -1;
    }

    output_format->codec = output->codec;
    if (output->acmod == 7)
        output_format->channel = 6;
    else
        output_format->channel = output->channel;

    output_format->sample_rate = output->sample_rate;

    /* webOS25-shipped: output_codec_str[] has only pcm..mat + "???" */
    info ("%s codec : %s, ch : %d, sample_rate : %d.\n", OUTPUT_TO_STR (output),
        output_codec_str[output_format->codec], output_format->channel, output_format->sample_rate);

    return 0;
}

/* webOS25-shipped: not in the published source. */
/* @0xffffffc010cb3fa0 */
int output_get_latency (struct aud_block_info *block, unsigned int *latency)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (!output)
    {
        error ("output is null\n");
        return -1;
    }

    aimc_get_property (output->uid_out, "latency", latency);
    info ("%s latency : %d usec\n", OUTPUT_TO_STR (output), *latency);

    return 0;
}

/* webOS25-shipped: not in the published source. The command id and the
 * response layout of aimc_send_command_return() were not recovered. */
/* @0xffffffc010cb41d0 */
int output_get_bc_write_time (struct aud_block_info *block, unsigned long long *bc_write_time)
{
    struct aud_output_info *output = (struct aud_output_info *) block;

    if (!output)
    {
        error ("output is null\n");
        return -1;
    }

    /* [GAP] aimc_send_command_return (.., output->uid_out, .., bc_write_time, ..) */
    info ("%s bc_write_time : %llu\n", OUTPUT_TO_STR (output), *bc_write_time);

    return 0;
}
