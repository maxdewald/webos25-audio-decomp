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
 * Focused call-chain reconstruction, kernel 5.4.268-329.ptl4tv.5.
 * Image SHA256: 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Base: LG's GPL-2.0 source; shipped instructions in build/scratch/seek-chain/.
 * Only the tagged helpers are reconstructed; unrelated original-file bodies
 * and generic kernel/IMC/pipeline allocation machinery remain external.
 */
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include "block.h"
#include "decoder.h"
#include "kaimc.h"
#include "log.h"
logm_define (adecoder, log_level_noti);
#define info(fmt,args...) logm_info(adecoder,fmt,##args)
#define error(fmt,args...) logm_error(adecoder,fmt,##args)
#define debug(fmt,args...) logm_debug(adecoder,fmt,##args)

/* Shipped input +0x34, es_info +0x40 (0x44 bytes), property list +0xa0. */
struct aud_decoder_info
{
    struct aud_block_info parent;

    struct apipeline *pipeline_bypass;

    unsigned int uid_decbin;
    unsigned int uid_dec;
    unsigned int uid_queue;

    unsigned int uid_bypass;

    bool is_bypass_created;
    bool is_dummy_codec;

    enum audio_decoder_input input;
    enum audio_decoder_codec codec;
    enum audio_decoder_drc drc;

    struct decoded_info es_info;

    struct mpegh_ui_param ui_param;

    struct list_head list_prop;
    struct list_head list_event;
};

struct aud_decoder_property_info
{
    char *name;
    char *data;

    struct list_head list;
};

static DEFINE_MUTEX (_dec_prop_lock);
#define AUD_DEC_PROP_LOCK() mutex_lock(&_dec_prop_lock)
#define AUD_DEC_PROP_UNLOCK() mutex_unlock(&_dec_prop_lock)

static int decoder_set_property_list (struct aud_block_info *block, char *name, char *data)
{
    struct aud_decoder_info *decoder = (struct aud_decoder_info *) block;
    struct aud_decoder_property_info *prop_info = NULL;
    int ret = 0;

    AUD_DEC_PROP_LOCK ();

    list_for_each_entry (prop_info, &decoder->list_prop, list)
    {
        if (!strcmp (name, prop_info->name))
        {
            kfree (prop_info->data);
            prop_info->data = kstrdup (data, GFP_KERNEL);

            info ("update property, %s = %s\n", name, data);
            goto exit;
        }
    }

    prop_info =
        (struct aud_decoder_property_info *) kzalloc (sizeof (struct aud_decoder_property_info),
        GFP_KERNEL);
    if (!prop_info)
    {
        ret = -1;
        error ("no mem\n");
        goto exit;
    }

    prop_info->name = kstrdup (name, GFP_KERNEL);
    prop_info->data = kstrdup (data, GFP_KERNEL);

    list_add (&prop_info->list, &decoder->list_prop);


    debug ("decoder property list add, %s = %s\n", name, data);

  exit:
    AUD_DEC_PROP_UNLOCK ();
    return ret;
}

static int decoder_run_param (struct aud_block_info *block, char *name, char *data, int *ret_value,
    bool is_set)
{
    int ret = -1;
    struct aud_decoder_info *decoder = (struct aud_decoder_info *) block;

    if (decoder->uid_dec)
    {
        if (is_set)
        {
            decoder_set_property_list (block, name, data);

            ret = aimc_set_property (decoder->uid_dec, "%s=%s", name, data);
        }
        else
            ret = aimc_get_property (decoder->uid_dec, name, ret_value);
    }
    else
    {
        error ("decoder_run_param failed. can not find decoder\n");
    }

    return ret;
}

#define DECODER_PARAM_DELIMITER " =\\"

/* es_info.codec (+0x44, decoded_info +0x4) is enum audio_decoder_codec 0..21 (names
 * @0xffffffc0115e9538, IAMF 21), set by set_decoded_info.isra.0 from the DSP
 * media_type via table @0xffffffc0115e96d8 (media_type 1..24, else 0). There is no
 * DTS value; decoder->codec (+0x38) uses the same enum. */
/* @0xffffffc010ca6950 */
int decoder_get_decoded_info (struct aud_block_info *block, struct decoded_info *info)
{
    struct aud_decoder_info *decoder = (struct aud_decoder_info *) block;

    if (info == NULL)
    {
        error ("info is NULL\n");
        return -1;
    }

    memcpy (info, &decoder->es_info, sizeof (struct decoded_info));

    return 0;
}

/* @0xffffffc010ca8690 */
enum audio_decoder_input decoder_get_input (struct aud_block_info *block)
{
    struct aud_decoder_info *decoder = (struct aud_decoder_info *) block;

    return decoder->input;
}

/* @0xffffffc010ca82d0 */
int decoder_set_param (struct aud_block_info *block, char *pStrParam, ...)
{
    va_list ap;
    char *param, *name, *str;

    va_start (ap, pStrParam);
    param = kvasprintf (GFP_KERNEL, pStrParam, ap);
    va_end (ap);

    str = param;

    while ((name = strsep (&str, DECODER_PARAM_DELIMITER)))
    {
        char *data;
        int ret = -1;

        data = strsep (&str, DECODER_PARAM_DELIMITER);
        if (!data)
        {
            error ("no data for data. str %s\n", str);
            break;
        }

        info ("%s = %s\n", name, data);
        ret = decoder_run_param (block, name, data, NULL, true);

        if (ret < 0)
            error ("decoder_run_param () failed\n");
    }

    kfree (param);

    return 0;
}
