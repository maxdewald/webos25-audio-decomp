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
 * Source-assisted reconstruction, kernel 5.4.268-329.ptl4tv.5.
 * Image SHA256: 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Tagged functions checked against build/scratch/seek-chain/ instruction evidence.
 * Focused excerpt: surrounding original-file implementation remains external.
 */
#include <linux/types.h>
#include <linux/list.h>
#include "block.h"
#include "input.h"
#include "kaimc.h"
#include "log.h"
#include "imc/cmd/module_cmd_hdmi.h"
logm_define (aud_input, log_level_noti);
#define noti(fmt,args...) logm_noti(aud_input,fmt,##args)
#define error(fmt,args...) logm_error(aud_input,fmt,##args)

struct aud_input_info
{
    struct aud_block_info parent;

    struct apipeline *pipeline_queue;
    struct apipeline *pipeline_bypass;

    int dec_index;

    enum input_type input;
    int index;
    unsigned int channel_num;
    unsigned int sampling_freq;
    unsigned int org_hdmi_freq;
    unsigned int org_hdmi_ch;

    int uid;
    int uid_queue;
    int uid_queue_bypass;

    bool is_suspended;
    bool is_start;

    enum audio_decoder_codec tp_codec;
    bool clock_x4;
    enum audio_decoder_codec hdmi_codec;
    bool forced_pcm;
    bool data_drop;
    int output_port;

    bool drop;
    bool hdmi_multi_pcm_out;

    struct list_head list_event;
};

/* HAL payloads used only by this recovered call site. Unknown tail fields
 * remain zero. Layouts/size arguments are proven; public typedef names are not. */
struct recovered_hdmi_mute
{
    int port;
    int mute;
    int unrecovered_08;
    int mode;
};
struct recovered_hdmi_audio_info
{
    int port;
    int type;
    int sampling_freq;
    int channels;
    unsigned char unrecovered_10[24];
};
extern int KDRV_HDMI21_HAL_Rx_Set_AudioMute (int port, void *data, int size);
extern int KDRV_HDMI21_HAL_Rx_GetAudioInfo (int port, void *data, int size);

/* @0xffffffc010ca48f0: complete property/state effects, log-only code omitted.
 * Configure the INPUT's HDMI DSP uid, never the decoder uid. HAL type, kernel
 * audio_decoder_codec, ALSA UserCodec and DSP codec are distinct namespaces.
 * In particular HDMI DTS=7 / DTS_HD=202 / UNKNOWN_DTS=1004 /
 * UNKNOWN_DTS_HD=1202 all take mode=1, DSP codec=0, kernel codec=1 (PCM).
 * There is NO aud_hal_dts_supported() call and NO DTS/HBR setup here.
 * MAT=16 is mode=4/DSP codec=4/kernel MAT=5, not DTS_HD=202.
 * Preserves ignored HAL/property errors, untouched non-PCM channel_num,
 * original-rate bookkeeping, conditional queue writes and forced-PCM oddity:
 * forced_pcm skips the PCM branch; it does not unconditionally select PCM mode.
 * Does not write start/drop/data-drop-mode/drop-output-data, or emit FIRST.
 * Valid block/uid and matching HAL ABI are preconditions; no null validation.
 */
int input_set_hdmi (struct aud_block_info *block, bool forced_pcm)
{
    struct aud_input_info *input = (struct aud_input_info *) block;
    struct recovered_hdmi_mute mute = { .port = input->index, .mode = 2 };
    struct recovered_hdmi_audio_info audio = { .port = input->index };
    int mode, codec, kernel_codec;
    bool resize_queues = false;

    KDRV_HDMI21_HAL_Rx_Set_AudioMute (input->index, &mute, 16);
    KDRV_HDMI21_HAL_Rx_GetAudioInfo (input->index, &audio, 40);
    input->org_hdmi_freq = audio.sampling_freq;
    input->org_hdmi_ch = audio.channels;
    aimc_set_property (input->uid, "hdmi-port=%d", input->index);
    input->sampling_freq = (unsigned int) audio.sampling_freq - 32000 <= 160000
        ? audio.sampling_freq : 48000;
    aimc_set_property (input->uid, "sampling-freq=%d", input->sampling_freq);

    if (!forced_pcm && audio.type == 2)
    {
        switch (audio.channels)
        {
            case 2: case 4: case 6: case 8:
                input->channel_num = audio.channels;
                break;
            case 3: case 5: case 7:
                input->channel_num = audio.channels + 1;
                break;
            default:
                input->channel_num = 2;
                break;
        }
        aimc_set_property (input->uid, "channel-num=%d", input->channel_num);
        mode = codec = 0;
        kernel_codec = 1;
        resize_queues = audio.sampling_freq > 48000 || input->channel_num > 2;
    }
    else
    {
        /* Raw HAL numbers are deliberate: no borrowed DTS-enabled enums. */
        switch (audio.type)
        {
            case 3:    mode = 1; codec = 1; kernel_codec = 2; break;
            case 5:    mode = 1; codec = 3; kernel_codec = 7; break;
            case 14:   mode = 1; codec = 2; kernel_codec = 3; resize_queues = true; break;
            case 16:   mode = 4; codec = 4; kernel_codec = 5; resize_queues = true; break;
            case 200: case 201:
                mode = 6; codec = 0; kernel_codec = 1; break;
            case 1003: mode = 6; codec = 1; kernel_codec = 2; break;
            case 1005: mode = 6; codec = 3; kernel_codec = 7; break;
            case 1014: mode = 6; codec = 2; kernel_codec = 3; resize_queues = true; break;
            case 1016: mode = 7; codec = 4; kernel_codec = 5; resize_queues = true; break;
            default:   mode = 1; codec = 0; kernel_codec = 1; break;
        }
    }
    aimc_set_property (input->uid, "mode=%d", mode);
    input->hdmi_codec = kernel_codec;
    aimc_set_property (input->uid, "codec=%d", codec);
    if (resize_queues)
    {
        aimc_set_property (input->uid_queue, "max-length=%d", 0x20000);
        aimc_set_property (input->uid_queue_bypass, "max-length=%d", 0x20000);
    }
    input->forced_pcm = forced_pcm;
    return 0;
}

/* @0xffffffc010ca3be0 */
enum audio_decoder_codec input_get_hdmi_codec (struct aud_block_info *block)
{
    struct aud_input_info *input = (struct aud_input_info *) block;
    return input->hdmi_codec;
}

/* These helpers only send properties; neither sends FIRST nor updates clocks.
 * Shipped uid +0x44, data_drop +0x61, output_port +0x64, multi-pcm-out +0x69. */
/* @0xffffffc010ca4dd0 */
int input_set_hdmi_bypass (struct aud_block_info *block, bool data_drop, int output_port)
{
    struct aud_input_info *input = (struct aud_input_info *) block;

    int data_drop_mode;

    if (data_drop == true)
    {
        switch (output_port)
        {
            case 0:
                data_drop_mode = HDMI_DATA_DROP_MODE_MAIN_PATH_DROP;
                break;

            case 1:
                data_drop_mode = HDMI_DATA_DROP_MODE_SUB_PATH_DROP;
                break;

            default:
                data_drop_mode = HDMI_DATA_DROP_MODE_NONE;
                error ("Set data drop port & flag but this is not drop mode. \n");
                break;
        }
    }
    else
        data_drop_mode = HDMI_DATA_DROP_MODE_NONE;

    aimc_set_property (input->uid, "data-drop-mode=%d", data_drop_mode);

    input->data_drop = data_drop;
    input->output_port = output_port;

    noti ("data_drop = %d output_port = %d mode = %d\n", data_drop, output_port, data_drop_mode);
    return 0;
}

/* @0xffffffc010ca5110 */
int input_set_hdmi_multi_pcm_out (struct aud_block_info *block, bool enable)
{
    struct aud_input_info *input = (struct aud_input_info *) block;

    aimc_set_property (input->uid, "multi-pcm-out=%d", enable);
    input->hdmi_multi_pcm_out = enable;

    noti ("enable = %d\n", enable);
    return 0;
}
