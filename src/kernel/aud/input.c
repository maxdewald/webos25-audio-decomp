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
#define info(fmt,args...) logm_info(aud_input,fmt,##args)
#define noti(fmt,args...) logm_noti(aud_input,fmt,##args)
#define error(fmt,args...) logm_error(aud_input,fmt,##args)

struct aud_input_info
{
    struct aud_block_info parent;

    struct apipeline *pipeline_queue;   /* +0x10, written by input_set_type */
    void *unknown_18;                   /* not recovered */
    struct apipeline *pipeline_bypass;  /* +0x20 */

    int dec_index;                      /* +0x28 */

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

/* input->input values (+0x2c); names from the "%s%d" log table @0xffffffc0115e9378. */
enum
{
    INPUT_SIF = 0,
    INPUT_TP = 1,
    INPUT_HDMI = 2,
    INPUT_ADC = 3,
};
static const char *const input_type_str[] = { "SIF", "TP", "HDMI", "ADC" };

extern int aimc_set_property (int uid, const char *fmt, ...);
extern int aud_hal_bc_buffer_init (const char *name);
extern int aud_clock_disable_input (unsigned int index);
extern int AUDIO_RestartAad (void);
extern int AUDIO_ResumeAad (bool drop);
extern int input_set_type (struct aud_block_info *block, int dec_index, int type, int sub_index,
    bool unknown_arg4);
extern void input_set_clock (struct aud_input_info *input);

/* @0xffffffc010ca3be0: +0x5c, written only by input_set_hdmi: PCM, AC3, EAC3, AAC or MAT, never DTS. */
enum audio_decoder_codec input_get_hdmi_codec (struct aud_block_info *block)
{
    struct aud_input_info *input = (struct aud_input_info *) block;
    return input->hdmi_codec;
}

/* @0xffffffc010ca3bd0 */
int input_get_type (struct aud_block_info *block)
{
    struct aud_input_info *input = (struct aud_input_info *) block;
    return input->input;
}

/* @0xffffffc010ca3c00 */
int input_get_uid (struct aud_block_info *block)
{
    struct aud_input_info *input = (struct aud_input_info *) block;
    return input->uid;
}

/* Suspend path and pdec-error restart: drop everything, stop, flush both queues. */
/* @0xffffffc010ca3090 */
static void _input_set_stop_property (struct aud_input_info *input)
{
    aimc_set_property (input->uid, "drop-output-data=%d", 1);
    aimc_set_property (input->uid, "drop=%d", 1);
    aimc_set_property (input->uid, "start=%d", 0);
    aimc_set_property (input->uid_queue, "set-flush=%d", 1);
    if (input->input != INPUT_HDMI)
        return;
    aimc_set_property (input->uid_queue_bypass, "set-flush=%d", 1);
    if (input->hdmi_codec == AUDIO_DECODER_CODEC_PCM)
        aimc_set_property (input->uid, "multi-pcm-out=%d", 0);
}

/* Input event 2 (registered by alsa_decoder_start for TP); re-applies the saved drop. */
/* @0xffffffc010ca3680 */
int input_tp_pdec_error_info_cb (void *param, int len, void *cb_param)
{
    struct aud_input_info *input = cb_param;
    int *info = param;

    if (len != 8)
    {
        error ("Param Length Error[Expected:%zd][Input:%d]\n", (size_t) 8, len);
        return -1;
    }
    _input_set_stop_property (input);
    aud_hal_bc_buffer_init (input->index ? "bc-tp1" : "bc-tp0");
    aimc_set_property (input->uid, "start=%d", 1);
    aimc_set_property (input->uid, "drop-output-data=%d", input->drop);
    aimc_set_property (input->uid, "drop=%d", input->drop);
    noti ("PDEC Error Info(%d) : codec(%d) error(%d)\n", input->uid, info[0], info[1]);
    return 0;
}

/* The only setter of +0x68 besides resume; sends both properties with the same value.
 * On C5 alsa_decoder_start never reaches the false call for HDMI DTS (see alsa_decoder.c). */
/* @0xffffffc010ca37f0 */
int input_drop_data (struct aud_block_info *block, bool drop)
{
    struct aud_input_info *input = (struct aud_input_info *) block;

    input->drop = drop;
    aimc_set_property (input->uid, "drop-output-data=%d", drop);
    aimc_set_property (input->uid, "drop=%d", drop);
    info ("adec : %d, drop : %d(%s%d)\n", input->dec_index, drop, input_type_str[input->input],
        input->index);
    return 0;
}

/* @0xffffffc010ca38c0 */
bool input_is_drop_data (struct aud_block_info *block)
{
    struct aud_input_info *input = (struct aud_input_info *) block;
    return input->drop;
}

/* bc buffers are initialised only for dec_index 0/1; SIF restarts AAD instead of "start". */
/* @0xffffffc010ca38d0 */
int input_start (struct aud_block_info *block)
{
    struct aud_input_info *input = (struct aud_input_info *) block;

    if (input->dec_index <= 1)
    {
        switch (input->input)
        {
            case INPUT_TP:
                aud_hal_bc_buffer_init (input->index ? "bc-tp1" : "bc-tp0");
                break;
            case INPUT_HDMI:
                if (input->hdmi_codec == AUDIO_DECODER_CODEC_PCM
                    && input->org_hdmi_freq - 32000 > 160000)
                {
                    noti ("org_hdmi_freq is error : port = %d freq = %d ch = %d\n", input->index,
                        input->org_hdmi_freq, input->org_hdmi_ch);
                    return -1;
                }
                aud_hal_bc_buffer_init (input->dec_index ? "bc-sai1" : "bc-sai0");
                break;
            case INPUT_ADC:
                aud_hal_bc_buffer_init ("bc-sai2");
                break;
        }
    }
    if (input->input == INPUT_SIF)
        AUDIO_RestartAad ();
    else
        aimc_set_property (input->uid, "start=%d", 1);
    input_set_clock (input);
    input->is_start = true;
    info ("adec : %d(%s%d)\n", input->dec_index, input_type_str[input->input], input->index);
    return 0;
}

/* @0xffffffc010ca3ab0 */
int input_stop (struct aud_block_info *block)
{
    struct aud_input_info *input = (struct aud_input_info *) block;

    if (input->is_start && input->input != INPUT_SIF)
        aimc_set_property (input->uid, "start=%d", 0);
    aud_clock_disable_input (input->dec_index);
    input->is_start = false;
    if (input->input == INPUT_TP)
        aimc_set_property (input->uid_queue, "set-flush=%d", INPUT_TP);
    else if (input->input == INPUT_HDMI)
    {
        aimc_set_property (input->uid_queue, "set-flush=%d", 1);
        aimc_set_property (input->uid_queue_bypass, "set-flush=%d", 1);
    }
    info ("adec : %d(%s%d)\n", input->dec_index, input_type_str[input->input], input->index);
    return 0;
}

/* Block ops .resume: always comes back dropped; alsa_decoder_start must clear it. */
/* @0xffffffc010ca40a4 */
static int input_resume (struct aud_block_info *block)
{
    struct aud_input_info *input = (struct aud_input_info *) block;

    info ("is_start = %d drop = %d\n", input->is_start, input->drop);
    if (input->input == INPUT_SIF)
        AUDIO_ResumeAad (true);
    else
        input_set_type (block, input->dec_index, input->input, input->index, false);
    aimc_set_property (input->uid, "drop-output-data=%d", 1);
    aimc_set_property (input->uid, "drop=%d", 1);
    input->is_suspended = false;
    return 0;
}

/* HDMI21_HAL_Rx_Get_AudioType @0xffffffc010a1fb24 result; names from its
 * "HDMI[%d] Audio Type = ..." logs. IEC 61937 Pc 11..13 -> DTS; Pc 17 -> DTS
 * (Pc word 0x411/0xa11), DTS_HD_HRA (0x211/0x311) or DTS_HD. The "Unknown X"
 * logs return X + 1000. */
enum hdmi_audio_type
{
    HDMI_AUDIO_DVI = 0,
    HDMI_AUDIO_NON_PCM = 1,
    HDMI_AUDIO_PCM = 2,
    HDMI_AUDIO_AC3 = 3,
    HDMI_AUDIO_DTS = 4,
    HDMI_AUDIO_AAC = 5,
    HDMI_AUDIO_MPEG = 10,
    HDMI_AUDIO_AC3_ENHANCED = 14,
    HDMI_AUDIO_MAT = 16,
    HDMI_AUDIO_DTS_HD = 202,
    HDMI_AUDIO_DTS_HD_HRA = 203,
    HDMI_AUDIO_UNKNOWN = 1000,
};

/* Partial kdrv argument layouts (our names); only the used fields are known. */
struct hdmi_audio_mute_arg
{
    unsigned int port;
    unsigned int unknown_04[2];
    unsigned int unknown_0c;            /* always 2 here, meaning not recovered */
};
struct hdmi_audio_info_arg
{
    unsigned int port;
    int type;                           /* enum hdmi_audio_type */
    int sampling_freq;
    unsigned int channel;
    unsigned char unknown_10[0x18];
};
extern int KDRV_HDMI21_HAL_Rx_Set_AudioMute (unsigned char port, void *arg, int size);
extern int KDRV_HDMI21_HAL_Rx_GetAudioInfo (unsigned char port, void *arg, int size);

/* The only writer of the dsp0 hdmi "codec" property ("codec=%d") and of hdmi_codec.
 * Caller alsa_decoder_start passes forced_pcm = (ALSA "Adec Codec" == PCM).
 *   HDMI type                          mode  hdmi_codec  dsp0 codec  max-length
 *   PCM, or forced_pcm                 0     PCM         0 pcm       if >48 kHz or >2 ch
 *   AC3 / Unknown AC3                  1/6   AC3         1 ac3       -
 *   AC3_ENHANCED / Unknown             1/6   EAC3        2 eac3      0x20000
 *   AAC / Unknown AAC                  1/6   AAC         3 aac       -
 *   MAT / Unknown MAT                  4/7   MAT         4 mat       0x20000
 *   DTS, DTS_HD, DTS_HD_HRA (+1000),   1     PCM         0 pcm       -
 *   MPEG, NON_PCM, DVI, other
 * dsp0 codec 5 ("dts") and 6 ("dts_hd_ma") are never sent by this kernel. The
 * meaning of "mode" (200/201 also give 6) is not recovered.
 * 6.12 C5 (@0xffffffc080f293f0) has the same table. G3 5.4 (@0xffffffc010ca49b0) adds DTS:
 * 4/1004 -> mode 1/6 codec 5; 202/1202 -> mode 4/7 codec 6 + max-length; 203/1203 -> mode
 * 5/8, codec 6 if freq > 191999 else 5; hdmi_codec 0x11 (5) / 0x12 (6). */
/* @0xffffffc010ca48f0 */
int input_set_hdmi (struct aud_block_info *block, bool forced_pcm)
{
    struct aud_input_info *input = (struct aud_input_info *) block;
    struct hdmi_audio_mute_arg mute = { .port = input->index, .unknown_0c = 2 };
    struct hdmi_audio_info_arg ai = { .port = input->index };
    bool max_length = false;
    int mode, codec;

    KDRV_HDMI21_HAL_Rx_Set_AudioMute (input->index, &mute, sizeof (mute));
    KDRV_HDMI21_HAL_Rx_GetAudioInfo (input->index, &ai, sizeof (ai));
    input->org_hdmi_freq = ai.sampling_freq;
    input->org_hdmi_ch = ai.channel;
    aimc_set_property (input->uid, "hdmi-port=%d", input->index);
    input->sampling_freq =
        ((unsigned int) ai.sampling_freq - 32000 <= 160000) ? ai.sampling_freq : 48000;
    aimc_set_property (input->uid, "sampling-freq=%d", input->sampling_freq);

    if (forced_pcm || ai.type == HDMI_AUDIO_PCM)
    {
        unsigned int ch = ai.channel;

        if (ch == 3 || ch == 5 || ch == 7)
            ch++;
        else if (ch != 2 && ch != 4 && ch != 6 && ch != 8)
            ch = 2;
        input->channel_num = ch;
        aimc_set_property (input->uid, "channel-num=%d", ch);
        info ("ch = %d\n", input->channel_num);
        mode = 0;
        codec = 0;
        aimc_set_property (input->uid, "mode=%d", mode);
        input->hdmi_codec = AUDIO_DECODER_CODEC_PCM;
        aimc_set_property (input->uid, "codec=%d", codec);
        max_length = ai.sampling_freq > 48000 || input->channel_num > 2;
    }
    else
    {
        switch (ai.type)
        {
            case HDMI_AUDIO_AC3:
            case HDMI_AUDIO_UNKNOWN + HDMI_AUDIO_AC3:
                mode = ai.type == HDMI_AUDIO_AC3 ? 1 : 6;
                input->hdmi_codec = AUDIO_DECODER_CODEC_AC3;
                codec = 1;
                break;
            case HDMI_AUDIO_AC3_ENHANCED:
            case HDMI_AUDIO_UNKNOWN + HDMI_AUDIO_AC3_ENHANCED:
                mode = ai.type == HDMI_AUDIO_AC3_ENHANCED ? 1 : 6;
                input->hdmi_codec = AUDIO_DECODER_CODEC_EAC3;
                codec = 2;
                max_length = true;
                break;
            case HDMI_AUDIO_AAC:
            case HDMI_AUDIO_UNKNOWN + HDMI_AUDIO_AAC:
                mode = ai.type == HDMI_AUDIO_AAC ? 1 : 6;
                input->hdmi_codec = AUDIO_DECODER_CODEC_AAC;
                codec = 3;
                break;
            case HDMI_AUDIO_MAT:
            case HDMI_AUDIO_UNKNOWN + HDMI_AUDIO_MAT:
                mode = ai.type == HDMI_AUDIO_MAT ? 4 : 7;
                input->hdmi_codec = AUDIO_DECODER_CODEC_MAT;
                codec = 4;
                max_length = true;
                break;
            default:
                mode = (ai.type == 200 || ai.type == 201) ? 6 : 1;
                input->hdmi_codec = AUDIO_DECODER_CODEC_PCM;
                codec = 0;
                break;
        }
        aimc_set_property (input->uid, "mode=%d", mode);
        aimc_set_property (input->uid, "codec=%d", codec);
    }

    if (max_length)
    {
        aimc_set_property (input->uid_queue, "max-length=%d", 0x20000);
        aimc_set_property (input->uid_queue_bypass, "max-length=%d", 0x20000);
    }
    input->forced_pcm = forced_pcm;
    noti ("port = %d mode = %d codec = %d freq = %d\n", input->index, mode, codec,
        input->sampling_freq);
    return 0;
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
