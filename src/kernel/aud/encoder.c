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
 * Focused call-chain reconstruction, kernel 5.4.268-329.ptl4tv.5.
 * Image SHA256: 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Base: LG's GPL-2.0 source; shipped instructions in build/scratch/seek-chain/.
 * Only the tagged helpers are reconstructed; unrelated original-file bodies
 * and generic kernel/IMC/pipeline allocation machinery remain external.
 */
#include <linux/slab.h>

#include "log.h"
#include "kaimc.h"

#include "block.h"
#include "block_manager.h"
#include "imc/cmd/module_cmd_ddpenc.h"
#include "module/adec_module_list.h"
#include "encoder.h"

/*------------------------------------------------------------------------------
    Constant Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Macro Definitions
------------------------------------------------------------------------------*/
/// @brief Print log message for trace level
#define trace(fmt,args...)      logm_trace (aencoder, fmt, ##args)
/// @brief Print log message for debug level
#define debug(fmt,args...)      logm_debug (aencoder, fmt, ##args)
/// @brief Print log message for info level
#define info(fmt,args...)       logm_info (aencoder, fmt, ##args)
/// @brief Print log message for noti level
#define noti(fmt,args...)       logm_noti (aencoder, fmt, ##args)
/// @brief Print log message for warning level
#define warn(fmt,args...)       logm_warning (aencoder, fmt, ##args)
/// @brief Print log message for error level
#define error(fmt,args...)      logm_error (aencoder, fmt, ##args)

#define AUD_DEC_LOCK()		    mutex_lock(&_enc_lock)
#define AUD_DEC_UNLOCK()	    mutex_unlock(&_enc_lock)

#define set_property(_prop, _value) ({                      \
    if (_prop == NULL) {                                    \
        _prop = kmalloc (sizeof (*(_prop)), GFP_KERNEL);    \
        if (_prop == NULL) return -1;                       \
    }                                                       \
    *(_prop) = _value;                                      \
})

/*------------------------------------------------------------------------------
    Type Definitions
------------------------------------------------------------------------------*/
/**
 * AUDIO Encoder Information.
 */
struct aud_encoder_info
{
    struct aud_block_info parent;

    unsigned int proc_id;

    unsigned int uid_gain;
    unsigned int uid_enc;

    /* Shipped codec pointer is +0x28: the published dts_enc_mode at +0x24
     * occupies only alignment padding here; no DTS functionality is present. */
    unsigned char gap_24[4];

    // 상태 변수들
    enum output_codec_t *codec;
    int *acmod;
    int *gain;
    int *channel;
    bool *atmos_on;
    bool *atmos_lock_mode;
    bool *ch_lock_mode;
};

/*------------------------------------------------------------------------------
    External Function Prototype Declarations
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    External Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    global Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Static Function Prototypes Declarations
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Static Variables
------------------------------------------------------------------------------*/
logm_define (aencoder, log_level_noti);

//TODO: wrapper 적용 안했으니 ddp/mat encoder instance 는 1개만 만들어져야 함
static DEFINE_MUTEX (_enc_lock);

/*==============================================================================
    Implementation Group
==============================================================================*/

/* Shipped codec switch accepts AC3/EAC3/AAC/MAT only; no DTS arms. */

/* @0xffffffc010cb4e44 */
int aud_encoder_set_codec (struct aud_block_info *block, enum output_codec_t codec, int channel)
{
    struct aud_encoder_info *encoder = (struct aud_encoder_info *) block;

    info ("set codec %d", codec);

    set_property (encoder->codec, codec);
    set_property (encoder->channel, channel);

    if (!encoder->uid_enc)
    {
        encoder->uid_enc = apipeline_appendf (block->pipeline, "encoderbin");
    }

    switch (codec)
    {
        case OUTPUT_CODEC_AC3:
            aimc_set_property (encoder->uid_enc, "codec=ac3");
            break;
        case OUTPUT_CODEC_EAC3:
            aimc_set_property (encoder->uid_enc, "codec=eac3");
            break;
        case OUTPUT_CODEC_AAC:
            if (channel == 2)
            {
                if (!encoder->uid_gain)
                {
                    encoder->uid_gain = apipeline_prependf (block->pipeline, "gain");
                    apipeline_prependf (block->pipeline, "queue");
                }
                aimc_set_property (encoder->uid_enc, "codec=aac");
            }
            else
                aimc_set_property (encoder->uid_enc, "codec=aacmch");
            break;
        case OUTPUT_CODEC_MAT:
            aimc_set_property (encoder->uid_enc, "codec=mat");
            break;
        default:
            error ("invalid codec %d\n", codec);
            break;
    }

    return 0;
}

/* @0xffffffc010cb51a0 */
int aud_encoder_set_atmos_on (struct aud_block_info *block, bool atmos_on)
{
    struct aud_encoder_info *encoder = (struct aud_encoder_info *) block;
    enum output_codec_t codec;

    set_property (encoder->atmos_on, atmos_on);

    if (encoder->codec == NULL)
    {
        return -1;
    }

    codec = *encoder->codec;

    if (codec == OUTPUT_CODEC_EAC3)
        aimc_set_property (encoder->uid_enc, "atmos-on=%d", atmos_on);

    info ("codec : %d atmos_on : %d\n", codec, atmos_on);
    return 0;
}

/* @0xffffffc010cb5280 */
int aud_encoder_set_atmos_locking_mode (struct aud_block_info *block, bool atmos_lock_mode)
{
    struct aud_encoder_info *encoder = (struct aud_encoder_info *) block;
    enum output_codec_t codec;

    set_property (encoder->atmos_lock_mode, atmos_lock_mode);

    if (encoder->codec == NULL)
    {
        return -1;
    }

    codec = *encoder->codec;

    if (codec == OUTPUT_CODEC_EAC3 || codec == OUTPUT_CODEC_MAT)
        aimc_set_property (encoder->uid_enc, "atmos-lock=%d", atmos_lock_mode);

    info ("codec : %d atmos_lock_mode : %d\n", codec, atmos_lock_mode);
    return 0;
}

/* @0xffffffc010cb5364 */
int aud_encoder_set_ch_lock_mode (struct aud_block_info *block, bool ch_lock_mode)
{
    struct aud_encoder_info *encoder = (struct aud_encoder_info *) block;
    enum output_codec_t codec;

    set_property (encoder->ch_lock_mode, ch_lock_mode);

    if (encoder->codec == NULL)
    {
        return -1;
    }

    codec = *encoder->codec;

    if (codec == OUTPUT_CODEC_EAC3 || codec == OUTPUT_CODEC_AC3 || codec == OUTPUT_CODEC_MAT)
        aimc_set_property (encoder->uid_enc, "channel-lock=%d", ch_lock_mode);

    info ("codec : %d ch_lock_mode : %d\n", codec, ch_lock_mode);
    return 0;
}
