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
