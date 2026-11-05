// SPDX-License-Identifier: GPL-2.0-only
/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2020 by LG Electronics Inc.

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
 * Base: lg-opensource/kdriver/core/aud/driver.c (GPL-2.0), complete.
 * Every function was compared with the shipped disassembly (called
 * functions, ioctl numbers, log strings and __LINE__ values).
 * Log lines from driver_aclk_open on are +14 lines vs the published file
 * (+13 in aud_cdev_init), so ~14 lines were added in the adec part.
 *
 * webOS25-shipped differences found:
 *  - driver_adec_ioctl: the DTS ioctls ADEC_IOR_GET_DTS_SUPPORTED ('D' 24)
 *    and ADEC_IOR_GET_DTS_BITSTREAM_PASSTHROUGH ('D' 25) are NOT handled
 *    (-> "unknown ioctl", -ENOIOCTLCMD); two new commands exist:
 *    _IOW('D', 30, bool) -> adecoder_set_downmix, _IOW('D', 31, uint) ->
 *    adecoder_set_sound_system.
 *  - adec/aren/aclk/adbg open use kzalloc (kmem_cache_alloc_trace) - the
 *    published kcalloc(1, ...) compiles to the same thing.
 */




/** @file	driver.c
 *
 *  main driver implementation for  audio device.
 *  audio device will teach you how to make device driver with new platform.
 *
 *  author	Jihoon Lee
 *  version	0.1
 *  date	2020.09.25
 *  note	Additional information.
 *
 */


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>    /**< For isr */
#include <linux/irq.h>          /**< For isr */
#include <linux/ioport.h>       /**< For request_region, check_region etc */
#include <linux/rmap.h>
#include <linux/kthread.h>
#include <asm/io.h>             /**< For ioremap_nocache */
#include <asm/memory.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/version.h>

#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"
#include "hma_alloc.h"
#include "proc.h"
#include "log.h"
#include "hal/hal.h"

#include "aud_kapi.h"
#include "aud_kapi_decoder.h"
#include "aud_kapi_clock.h"
#include "aud_kapi_debug.h"
#include "driver.h"
#include "aapi.h"
#include "block_manager.h"

logm_define (auddrv, log_level_noti);
#define trace(fmt,args...)	logm_trace(auddrv,fmt,##args)
#define debug(fmt,args...)	logm_debug(auddrv,fmt,##args)
#define info(fmt,args...)	logm_info(auddrv,fmt,##args)
#define noti(fmt,args...)	logm_noti(auddrv,fmt,##args)
#define error(fmt,args...)	logm_error(auddrv,fmt,##args)

#define AUDIO_MODULE                "audio"

static DEFINE_MUTEX (aud_drv_lock);

#define DRIVER_LOCK()		mutex_lock(&aud_drv_lock)
#define DRIVER_UNLOCK()	mutex_unlock(&aud_drv_lock)

static int g_AudMajor = AUDIO_MAJOR;
static int g_AudMinor = AUDIO_MINOR;

/**
 * AUDIO Device Information.
 */
typedef struct
{
    dev_t devNo;                /* device number */
    struct cdev cdev;           /* charactor device struct */
    void *devInfo;              /* device info */
} AUD_DEVICE_T;

/**
 * AUDIO Event Information.
 */
typedef struct
{
    unsigned int event;
    unsigned int actionID;
    unsigned int moduleID;      // ADEC_MODULE_ID
} AUD_EVENT_T;

/**
 * AUDIO DECODER Decoding Parameter.
 */
struct audio_decoder_info
{
    struct list_head list;

    unsigned int index;
};


/**
 * AUDIO Renderer Information.
 */
struct audio_renderer_info
{
    struct list_head list;

    unsigned int index;
};

static AUD_DEVICE_T *g_AudDev;


/**
 * open handler for audio decoder device
 *
 */
/* @0xffffffc010cb76e0 */
SINT32 driver_adec_open (struct inode *inode, struct file *filp)
{
    struct audio_decoder_info *info;

    info ("open audio decoder\n");

    info =
        (struct audio_decoder_info *) kcalloc (1, sizeof (struct audio_decoder_info), GFP_KERNEL);
    if (!info)
    {
        error ("no mem\n");
        return -ENOMEM;
    }

    info->index = -1;

    filp->private_data = (void *) info;

    info ("decoder for media is opened.\n");

    return 0;
}

/**
 * close handler for audio decoder device
 *
 */
/* @0xffffffc010cb74a0 */
SINT32 driver_adec_close (struct inode * inode, struct file * filp)
{
    SINT32 retVal = 0;
    struct audio_decoder_info *info = filp->private_data;

    info ("close decoder %d\n", info->index);

    adecoder_close (info->index);

    /* remove from the list */
    DRIVER_LOCK ();
    kfree (info);
    DRIVER_UNLOCK ();

    return retVal;
}

/* webOS25-shipped: ioctls not in the published aud_kapi_decoder.h
 * (numbers from the shipped driver_adec_ioctl compare tree). */
#ifndef ADEC_IOW_SET_DOWNMIX
#define ADEC_IOW_SET_DOWNMIX                        _IOW(ADEC_IOC_MAGIC, 30, bool)
#define ADEC_IOW_SET_SOUND_SYSTEM                   _IOW(ADEC_IOC_MAGIC, 31, unsigned int)
#endif
int adecoder_set_downmix (int index, bool enable);
int adecoder_set_sound_system (int index, unsigned int sound_system);

union audio_ioctl_args
{
    int index;
    enum audio_decoder_codec codec;
    struct adec_codec_param codec_param;
    unsigned int uid;
    unsigned int gstc;
    unsigned int val_uint32;
    unsigned long long val_uint64;
    bool val_bool;
    unsigned int volume;
    unsigned int output_mode;
    enum audio_decoder_input input;
    struct adec_mpegh_ui_param ui_param;
    struct gap_info dec_gap_info;

    struct aren_base_time base_time;
    struct aren_lipsync_param lipsync_param;
    struct decoded_info decoded_info;
    aren_ease_volume_t ease_volume;
    aren_gap_info_t gap_info;
    aren_rendered_status_t rendered_status;
    aren_rts_queue_info_t rts_queue_info;
    aren_rts_param_t rts_param;
    struct input_info input_info;
    aren_pcm_info_t pcm_info;
    aren_queue_info_t queue_info;
    aren_vdec_current_pts_t vdec_current_pts;

    struct adbg_open_block open_block;
    struct adbg_ren_param ren_param;
};

struct adec_convert_table
{
    int adec_type;
    const char *adec_name;
    int aapi_type;
    const char *aapi_name;
};

#define define_convert_table(adec,aapi) {adec, #adec, aapi, #aapi}

const static struct adec_convert_table table_clock[] = {
    /* *INDENT-OFF* */
    define_convert_table (AUD_CLK_TYPE_PCR_M,      MD_CLK_TYPE_PCR_M),
    define_convert_table (AUD_CLK_TYPE_PCR_A,      MD_CLK_TYPE_PCR_A),
    define_convert_table (AUD_CLK_TYPE_NO_PCR,     MD_CLK_TYPE_NO_PCR),
    define_convert_table (AUD_CLK_TYPE_GSTC,       MD_CLK_TYPE_GSTC),
    define_convert_table (AUD_CLK_TYPE_GSTC_1,     MD_CLK_TYPE_GSTC_1),
    define_convert_table (AUD_CLK_TYPE_MASTER,     MD_CLK_TYPE_MASTER),
    {},
    /* *INDENT-ON* */
};

static int convert_types (int from, const struct adec_convert_table *table)
{
    for (; table->adec_name; table++)
    {
        if (table->adec_type == from)
        {
            debug ("from %s(0x%x) --> to %s(0x%x)\n", table->adec_name, table->adec_type,
                table->aapi_name, table->aapi_type);
            return table->aapi_type;
        }
    }

    error ("invalid parameter: 0x%x\n", from);
    return -1;
}

static enum md_clock_type adec_convert_clock_type (enum aud_clock_type clock_type)
{
    return convert_types (clock_type, table_clock);
}


/**
 * ioctl handler for audio decoder device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
/* @0xffffffc010cb7d54 */
long driver_adec_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
    union audio_ioctl_args args;
    struct audio_decoder_info *info = filp->private_data;
    int ret = -EFAULT;

    if (_IOC_SIZE (cmd) > sizeof (union audio_ioctl_args))
    {
        error ("invalid ioctl size. size %d max_size %ld\n", _IOC_SIZE (cmd),
            sizeof (union audio_ioctl_args));
        return ret;
    }

    /* copy input data from user layer */
    if (_IOC_DIR (cmd) & _IOC_WRITE)
    {
        ret = copy_from_user (&args, (void *) arg, _IOC_SIZE (cmd));
        if (ret)
        {
            error ("copy_from_user() failed. cmd %x, arg %lx\n", cmd, arg);
            return -EFAULT;
        }
    }

    switch (cmd)
    {
        case ADEC_IOW_SET_INDEX:
            info->index = args.index;
            if (args.index < 0)
            {
                error ("invalid decoder index, %d\n", args.index);
                break;
            }
            ret = adecoder_open (args.index);
            break;
        case ADEC_IOW_SET_CODEC:
            ret = adecoder_codec (info->index, args.codec);
            break;
        case ADEC_IOW_SET_CODEC_PARAM:
            ret = adecoder_codec_param_user (info->index, &args.codec_param);
            break;
        case ADEC_IOR_GET_UID:
        {
            unsigned int uid = 0;
            ret = adecoder_get_uid (info->index, &uid);
            if (ret == 0)
                args.uid = uid;
            break;
        }
        case ADEC_IO_START:
        {
            ret = adecoder_start (info->index);
            break;
        }
        case ADEC_IO_STOP:
        {
            ret = adecoder_stop (info->index);
            break;
        }
        case ADEC_IOW_SET_OUTPUT_MODE:
        {
            ret = adecoder_set_output_mode (info->index, args.output_mode);
            break;
        }
        case ADEC_IOR_GET_DECODED_INFO:
        {
            struct decoded_info decoded_info = { };

            ret = adecoder_get_decoded_info (info->index, &decoded_info);
            if (ret == 0)
                memcpy (&args.decoded_info, &decoded_info, sizeof (struct decoded_info));

            break;
        }
        case ADEC_IOW_SET_INPUT:
        {
            ret = adecoder_set_input (info->index, args.input);
            break;
        }
        case ADEC_IOR_GET_INPUT:
        {
            enum audio_decoder_input input = 0;

            ret = adecoder_get_input (args.input_info.index, &input);
            if (ret == 0)
                args.input_info.input = input;
            break;
        }
        case ADEC_IOW_SET_UI_PARAM:
        {
            ret = adecoder_set_mpegh_ui_param (info->index, &args.ui_param);
            break;
        }
        case ADEC_IOR_GET_UI_PARAM:
        {
            ret = copy_from_user (&args, (void *) arg, _IOC_SIZE (cmd));
            if (ret)
            {
                error ("copy_from_user() failed. cmd %x, arg %lx\n", cmd, arg);
                return -EFAULT;
            }
            ret = adecoder_get_mpegh_ui_param (info->index, &args.ui_param);
            break;
        }
        case ADEC_IOW_SET_BASETIME:
        {
            ret = adecoder_set_basetime (info->index, args.val_uint64);
            break;
        }
        case ADEC_IOW_SET_PRESENTATION_GROUP_INDEX:
        {
            ret = adecoder_set_presentation_group_index (info->index, args.val_uint32);
            break;
        }
        case ADEC_IOW_SET_DIALOG_ENHANCEMENT_GAIN:
        {
            ret = adecoder_set_dialog_enhancement_gain (info->index, args.val_uint32);
            break;
        }
        case ADEC_IOW_SET_AAC_FORMAT_TYPE:
        {
            ret = adecoder_set_aac_format_type (info->index, args.val_uint32);
            break;
        }
        case ADEC_IOR_GET_DIALOG_ENHANCEMENT_GAIN:
        {
            unsigned int gain = 0;
            ret = adecoder_get_dialog_enhancement_gain (info->index, &gain);
            if (ret == 0)
                args.val_uint32 = gain;
            break;
        }
        case ADEC_IOR_GET_DIALOG_ENHANCEMENT_LIMIT:
        {
            unsigned int limit = 0;
            ret = adecoder_get_dialog_enhancement_limit (info->index, &limit);
            if (ret == 0)
                args.val_uint32 = limit;
            break;
        }
        case ADEC_IOW_SET_DEFAULT_PRL:
        {
            ret = adecoder_set_default_prl (info->index, args.val_uint32);
            break;
        }
        case ADEC_IOW_SET_GAP_INFO:
        {
            ret = adecoder_set_gap_info (info->index, &args.dec_gap_info);
            break;
        }
        /* webOS25-shipped: no case for ADEC_IOR_GET_DTS_SUPPORTED (0x80014418)
         * nor ADEC_IOR_GET_DTS_BITSTREAM_PASSTHROUGH (0x80014419): both end in
         * the "unknown ioctl" default (-ENOIOCTLCMD). The compare tree of the
         * shipped function has no 'D' 24/25 node and there is no
         * adecoder_get_dts_* symbol. */
        case ADEC_IOW_SET_DOLBY_DRC_MODE:
        {
            ret = adecoder_set_dolby_drc_mode (info->index, args.val_uint32);
            break;
        }
        case ADEC_IOW_SET_PROC_ID:
        {
            ret = adecoder_set_proc_id (info->index, args.val_uint32);
            break;
        }
        case ADEC_IOW_SET_NORM_CLOSED:
        {
            ret = adecoder_set_normal_closed (info->index);
            break;
        }
        /* webOS25-shipped: new (shipped +0x430 / +0x4d4). */
        case ADEC_IOW_SET_DOWNMIX:
        {
            ret = adecoder_set_downmix (info->index, args.val_bool);
            break;
        }
        case ADEC_IOW_SET_SOUND_SYSTEM:
        {
            ret = adecoder_set_sound_system (info->index, args.val_uint32);
            break;
        }

        default:
            /* redundant check but it seems more readable */
            error ("unknown ioctl. %x\n", cmd);
            ret = -ENOIOCTLCMD;
            break;
    }

    if (ret >= 0 && _IOC_DIR (cmd) & _IOC_READ)
    {
        int r;

        r = copy_to_user ((void *) arg, &args, _IOC_SIZE (cmd));
        if (r)
        {
            error ("copy_to_user() failed. cmd %x, arg %lx\n", cmd, arg);
            return -EFAULT;
        }
    }

    return ret;
}

#ifdef CONFIG_COMPAT
/* @0xffffffc010cb83f0 */
long driver_adec_ioctl_compat (struct file *filp, unsigned int cmd, unsigned long arg)
{
    long ret;

    switch (cmd)
    {
        case ADEC_IOW_SET_CODEC_PARAM:
        {
            struct adec_codec_param param;

            ret = copy_from_user (&param, (void __user *) arg, sizeof (struct adec_codec_param));
            if (ret)
            {
                return -EINVAL;
            }

            param.param = compat_ptr (param.compat_param);

            ret = copy_to_user ((void __user *) arg, &param, sizeof (struct adec_codec_param));
            if (ret)
            {
                return -EINVAL;
            }

            break;
        }
        case ADEC_IOW_SET_UI_PARAM:
        case ADEC_IOR_GET_UI_PARAM:
        {
            struct adec_mpegh_ui_param param;

            ret = copy_from_user (&param, (void __user *) arg, sizeof (struct adec_mpegh_ui_param));
            if (ret)
            {
                return -EINVAL;
            }

            param.param = compat_ptr (param.compat_param);

            ret = copy_to_user ((void __user *) arg, &param, sizeof (struct adec_mpegh_ui_param));
            if (ret)
            {
                return -EINVAL;
            }

            break;
        }

        default:
            break;
    }

    return driver_adec_ioctl (filp, cmd, arg);
}
#endif

/**
 * open handler for audio render device
 *
 */
/* @0xffffffc010cb7604 */
SINT32 driver_aren_open (struct inode * inode, struct file * filp)
{
    struct audio_renderer_info *info;

    info ("open renderer\n");

    filp->private_data = kcalloc (1, sizeof (struct audio_renderer_info), GFP_KERNEL);
    info = filp->private_data;
    if (!info)
    {
        error ("no mem\n");
        return -ENOMEM;
    }

    return RET_OK;
}

/**
 * close handler for audio render device
*
 */
/* @0xffffffc010cb7400 */
SINT32 driver_aren_close (struct inode * inode, struct file * filp)
{
    SINT32 retVal = 0;
    struct audio_renderer_info *info = filp->private_data;

    info ("close renderer %d\n", info->index);

    aren_close (info->index);

    DRIVER_LOCK ();
    kfree (info);
    DRIVER_UNLOCK ();

    return retVal;
}


/**
 * ioctl handler for audio render device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
/*
 * Shipped command set, traced through the compare tree of the shipped
 * driver_aren_ioctl ('R' = 0x52) for the commands on the ES path:
 *   0x40045201 SET_INDEX           -> aren_open                     (+0x684)
 *   0x80045202 GET_UID             -> aren_get_uid(idx, &uid, 0)    (+0x35c..+0x378)
 *   0x80045203 GET_BYPASS_UID      -> aren_get_uid(idx, &uid, 1)    (+0x314 -> +0x590)
 *   0x80045222 GET_TAIL_UID / 0x80045223 GET_BYPASS_TAIL_UID -> aren_get_tail_uid
 *   0x40205220 SET_PCM_INFO (32 bytes copied, then aren_set_pcm_info) (+0x3a0..+0x3c0)
 *   0x521e START -> aren_start, 0x521f STOP -> aren_stop, 0x5221 ADD_QUEUE
 *   0x40015224 SET_BYPASS_FADER -> aren_set_bypass_fader (+0xa4),
 *   0x40045227 SET_QUEUE_MAX_LENGTH, 0x522a SET_VSP_MODE, 0x5229 SET_NORM_CLOSED,
 *   0xc0085226 GET_VDEC_CURRENT_PTS, 0x80085225 GET_QUEUE_INFO,
 *   0x40085207 SET_BASETIME, 0x4008520e SET_GAP_INFO, 0x400c520b SET_EASE_VOLUME.
 * The other cases are present with the numbers of aud_kapi_renderer.h (call
 * targets all observed: set_clock_type, enable_lipsync, set_lipsync_param,
 * set_speed, set_mute, get_ease_volume, rendered_status, rts_*, set_jp4k,
 * set_proc_id) but their constants were not individually traced.
 * Traced numbers match src/libgstlgaudiosink/lgao.c AREN_* definitions.
 * Not handled (-> default / RET_INVALID_IOCTL): 0x5215 CONNECT_ES_OUTPUT,
 * 0x5216 DISCONNECT_ES_OUTPUT (declared in aud_kapi_renderer.h, never handled
 * in published or shipped code), 0x4008522b (sink's SET_MSGPID_TO_SIGNAL).
 * No ioctl in this file looks at the codec; the only ES/PCM decision is the
 * pcm_bypass/pcm_multi_ch test in aren_get_uid().
 */
/* @0xffffffc010cb8494 */
long driver_aren_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
    union audio_ioctl_args args;
    struct audio_renderer_info *info = filp->private_data;
    long ret = -EFAULT;

    if (_IOC_SIZE (cmd) > sizeof (union audio_ioctl_args))
    {
        error ("invalid ioctl size. size %d max_size %ld\n", _IOC_SIZE (cmd),
            sizeof (union audio_ioctl_args));
        return ret;
    }

    /* copy input data from user layer */
    if (_IOC_DIR (cmd) & _IOC_WRITE)
    {
        ret = copy_from_user (&args, (void *) arg, _IOC_SIZE (cmd));
        if (ret)
        {
            error ("copy_from_user() failed. cmd %x, arg %lx\n", cmd, arg);
            return -EFAULT;
        }
    }

    switch (cmd)
    {
        case AREN_IOW_SET_INDEX:
        {
            if (args.index < 0)
            {
                noti ("invalid renderer index, %d\n", args.index);
                noti ("try to use renderer index 0\n");
                args.index = 0;
            }
            ret = aren_open (args.index);
            info->index = args.index;
            break;
        }

        case AREN_IO_ADD_QUEUE:
        {
            ret = aren_add_queue (info->index);
            break;
        }

        case AREN_IOR_GET_UID:
        {
            unsigned int uid = 0;
            ret = aren_get_uid (info->index, &uid, false);
            if (ret == 0)
                args.uid = uid;
            break;
        }

        case AREN_IOR_GET_BYPASS_UID:
        {
            unsigned int uid = 0;
            ret = aren_get_uid (info->index, &uid, true);
            if (ret == 0)
                args.uid = uid;
            break;
        }

        case AREN_IOR_GET_TAIL_UID:
        {
            unsigned int uid = 0;
            ret = aren_get_tail_uid (info->index, &uid, false);
            if (ret == 0)
                args.uid = uid;
            break;
        }

        case AREN_IOR_GET_BYPASS_TAIL_UID:
        {
            unsigned int uid = 0;
            ret = aren_get_tail_uid (info->index, &uid, true);
            if (ret == 0)
                args.uid = uid;
            break;
        }

        case AREN_IOR_GET_GSTC:
        {
            /* Read a audio gstc reg. */
            args.gstc = aud_hal_get_gstc ();
            ret = 0;
            break;
        }

        case AREN_IOW_SET_CLOCKTYPE:
        {
            ret = aren_set_clock_type (info->index, adec_convert_clock_type (args.val_uint32));
            break;
        }

        case AREN_IOW_SET_ENALBE_LIPSYNC:
        {
            ret = aren_enable_lipsync (info->index, args.val_bool);
            break;
        }

        case AREN_IOW_SET_BASETIME:
        {
            unsigned int clock_base = 0;
            unsigned int stream_base = 0;

            clock_base = args.base_time.clock_base_time;
            stream_base = args.base_time.stream_base_time;

            ret = aren_set_basetime (info->index, clock_base, stream_base);
            break;
        }

        case AREN_IOW_SET_LIPSYNC_PARAM:
        {
            struct aren_lipsync_param *lipsync_param;
            unsigned int lower_bound, upper_bound;
            unsigned int offset;
            unsigned int freerun_lbound, freerun_ubound;

            lipsync_param = &(args.lipsync_param);
            lower_bound = lipsync_param->lower_bound;
            upper_bound = lipsync_param->upper_bound;
            offset = lipsync_param->offset;
            freerun_lbound = lipsync_param->freerun_lbound;
            freerun_ubound = lipsync_param->freerun_ubound;

            ret =
                aren_set_lipsync_param (info->index, lower_bound, upper_bound, offset,
                freerun_lbound, freerun_ubound);
            break;
        }

        case AREN_IOW_SET_SPEED:
        {
            ret = aren_set_speed (info->index, args.val_uint32);
            break;
        }

        case AREN_IOW_SET_MUTE:
        {
            ret = aren_set_mute (info->index, args.val_uint32);
            break;
        }

        case AREN_IOW_SET_EASE_VOLUME:
        {
            ret = aren_set_ease_volume (info->index, args.ease_volume);
            break;
        }

        case AREN_IOR_GET_EASE_VOLUME:
        {
            unsigned int volume;
            ret = aren_get_ease_volume (info->index, &volume);
            args.volume = volume;
            break;
        }

        case AREN_IOR_GET_STATUS:
            /* TODO */
            break;

        case AREN_IOW_SET_GAP_INFO:
        {
            ret = aren_set_gap_info (info->index, args.gap_info);
            break;
        }
#if 0
        case AREN_IOW_SET_MIXER_ACMOD:
        {
            ret = aren_set_mixer_acmod (info->index, args.val_uint32);
            break;
        }
#endif
        case AREN_IOR_GET_RENDERED_STATUS:
        {
            aren_rendered_status_t status;
            ret = aren_get_rendered_status (info->index, &status);
            memcpy (&args.rendered_status, &status, sizeof (aren_rendered_status_t));
            break;
        }

        case AREN_IOR_GET_RTS_QUEUE_INFO:
        {
            aren_rts_queue_info_t rts_queue_info;
            ret = aren_get_rts_queue_info (info->index, &rts_queue_info);
            memcpy (&args.rts_queue_info, &rts_queue_info, sizeof (aren_rts_queue_info_t));
            break;
        }

        case AREN_IOW_SET_RTS_PARAM:
        {
            ret = aren_set_rts_param (info->index, args.rts_param);
            break;
        }

        case AREN_IOW_ADD_RTS:
        {
            ret = aren_add_rts (info->index);
            break;
        }

        case AREN_IOW_SET_JP4K_STREAM:
        {
            ret = aren_set_jp4k (info->index, args.val_bool);
            break;
        }

        case AREN_IO_START:
        {
            ret = aren_start (info->index);
            break;
        }

        case AREN_IO_STOP:
        {
            ret = aren_stop (info->index);
            break;
        }

        case AREN_IOW_SET_PCM_INFO:
        {
            ret = aren_set_pcm_info (info->index, args.pcm_info);
            break;
        }

        case AREN_IOW_SET_BYPASS_FADER:
        {
            ret = aren_set_bypass_fader (info->index, args.val_bool);
            break;
        }

        case AREN_IOR_GET_QUEUE_INFO:
        {
            aren_queue_info_t queue_info;
            ret = aren_get_queue_info (info->index, &queue_info);
            memcpy (&args.queue_info, &queue_info, sizeof (aren_queue_info_t));
            break;
        }

        case AREN_IOR_GET_VDEC_CURRENT_PTS:
        {
            ret = aren_get_vdec_current_pts (info->index, &args.vdec_current_pts);
            break;
        }

        case AREN_IOW_SET_QUEUE_MAX_LENGTH:
        {
            ret = aren_set_queue_max_length (info->index, args.val_uint32);
            break;
        }

        case AREN_IOW_SET_PROC_ID:
        {
            ret = aren_set_proc_id (info->index, args.val_uint32);
            break;
        }

        case AREN_IOW_SET_NORM_CLOSED:
        {
            ret = aren_set_normal_closed (info->index);
            break;
        }

        case AREN_IOW_SET_VSP_MODE:
        {
            ret = aren_set_vsp_mode (info->index);
            break;
        }

        default:
        {
            /* redundant check but it seems more readable */
            error ("Invalid IOCTL Call!!! %x\n", cmd);
            ret = RET_INVALID_IOCTL;
            break;
        }
    }

    if (ret >= 0 && _IOC_DIR (cmd) & _IOC_READ)
    {
        int r;

        r = copy_to_user ((void *) arg, &args, _IOC_SIZE (cmd));
        if (r)
        {
            error ("copy_from_user() failed. cmd %x, arg %lx\n", cmd, arg);
            return -EFAULT;
        }
    }

    return ret;
}

#ifdef CONFIG_COMPAT
/* @0xffffffc010cb8cf0 */
long driver_aren_ioctl_compat (struct file *filp, unsigned int cmd, unsigned long arg)
{
    long ret;

    switch (cmd)
    {
        default:
            break;
    }

    ret = driver_aren_ioctl (filp, cmd, arg);

    return ret;
}
#endif


/**
 * open handler for audio clock device
 *
 */
/* @0xffffffc010cb7390 */
SINT32 driver_aclk_open (struct inode * inode, struct file * filp)
{
    info ("open clock\n");

    return RET_OK;
}

/**
 * close handler for audio clock device
*
 */
/* @0xffffffc010cb71a0 */
SINT32 driver_aclk_close (struct inode * inode, struct file * filp)
{
    return RET_OK;
}


/**
 * ioctl handler for audio clock device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
/* @0xffffffc010cb7904 */
long driver_aclk_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
    union audio_ioctl_args args;
    //struct audio_clock_info *info = filp->private_data;
    long ret = -EFAULT;

    if (_IOC_SIZE (cmd) > sizeof (union audio_ioctl_args))
    {
        error ("invalid ioctl size. size %d max_size %ld\n", _IOC_SIZE (cmd),
            sizeof (union audio_ioctl_args));
        return ret;
    }

    /* copy input data from user layer */
    if (_IOC_DIR (cmd) & _IOC_WRITE)
    {
        ret = copy_from_user (&args, (void *) arg, _IOC_SIZE (cmd));
        if (ret)
        {
            error ("copy_from_user() failed. cmd %x, arg %lx\n", cmd, arg);
            return -EFAULT;
        }
    }

    switch (cmd)
    {
        case ACLK_IOR_GET_GSTC:
        {
            /* Read a audio gstc reg. */
            args.gstc = aud_hal_get_gstc ();
            ret = 0;
            break;
        }

        default:
            /* redundant check but it seems more readable */
            error ("Invalid IOCTL Call!!! %x\n", cmd);
            ret = RET_INVALID_IOCTL;
            break;
    }

    if (ret >= 0 && _IOC_DIR (cmd) & _IOC_READ)
    {
        int r;

        r = copy_to_user ((void *) arg, &args, _IOC_SIZE (cmd));
        if (r)
        {
            error ("copy_from_user() failed. cmd %x, arg %lx\n", cmd, arg);
            return -EFAULT;
        }
    }

    return ret;
}

#ifdef CONFIG_COMPAT
/* @0xffffffc010cb7b00 */
long driver_aclk_ioctl_compat (struct file *filp, unsigned int cmd, unsigned long arg)
{
    long ret;

    switch (cmd)
    {
        default:
            break;
    }

    ret = driver_aclk_ioctl (filp, cmd, arg);

    return ret;
}
#endif

/**
 * open handler for audio debug
 *
 */
/* @0xffffffc010cb7540 */
SINT32 driver_adbg_open (struct inode * inode, struct file * filp)
{
    struct adebug_info_t *info;

    info ("open debug\n");

    info = adebug_open ();
    if (!info)
    {
        error ("no mem\n");
        return -ENOMEM;
    }

    filp->private_data = (void *) info;

    return RET_OK;
}


/**
 * close handler for audio debug
*
 */
/* @0xffffffc010cb7800 */
SINT32 driver_adbg_close (struct inode * inode, struct file * filp)
{
    SINT32 retVal = 0;
    struct adebug_info_t *info = filp->private_data;

    info ("close debug %d\n", info->index);
    if (info->is_created)
        info ("info->block_info %p, %s\n", info->block_info, info->block_info->block_type);

    DRIVER_LOCK ();
    retVal = adebug_close (info);
    filp->private_data = NULL;
    DRIVER_UNLOCK ();

    return retVal;
}


/**
 * ioctl handler for audio debug.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
/* @0xffffffc010cb7b14 */
long driver_adbg_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
    union audio_dbg_ioctl_args args;
    struct adebug_info_t *info = filp->private_data;
    long ret = -EFAULT;

    if (_IOC_SIZE (cmd) > sizeof (union audio_ioctl_args))
    {
        error ("invalid ioctl size. size %d max_size %ld\n", _IOC_SIZE (cmd),
            sizeof (union audio_ioctl_args));
        return ret;
    }

    /* copy input data from user layer */
    if (_IOC_DIR (cmd) & _IOC_WRITE)
    {
        ret = copy_from_user (&args, (void *) arg, _IOC_SIZE (cmd));
        if (ret)
        {
            error ("copy_from_user() failed. cmd %x, arg %lx\n", cmd, arg);
            return -EFAULT;
        }
    }

    ret = adebug_ioctl (info, cmd, &args);

    if (ret >= 0 && _IOC_DIR (cmd) & _IOC_READ)
    {
        int r;

        r = copy_to_user ((void *) arg, &args, _IOC_SIZE (cmd));
        if (r)
        {
            error ("copy_from_user() failed. cmd %x, arg %lx\n", cmd, arg);
            return -EFAULT;
        }
    }

    return ret;
}

#ifdef CONFIG_COMPAT
/* @0xffffffc010cb7d40 */
long driver_adbg_ioctl_compat (struct file *filp, unsigned int cmd, unsigned long arg)
{
    return driver_adbg_ioctl (filp, cmd, arg);
}
#endif

static struct
{
    const char *name;
    struct file_operations fops;
    struct file_operations *fops_ptr;
} driver_descs[] =
{
    /* *INDENT-OFF* */
    {
        // audio decoder
        .name = "adec0",
        .fops =
        {
            .open = driver_adec_open,
            .release = driver_adec_close,
            .unlocked_ioctl = driver_adec_ioctl,
#ifdef CONFIG_COMPAT
            .compat_ioctl = driver_adec_ioctl_compat,
#endif
        },
    },
    {
        // audio renderer
        .name = "aren0",
        .fops =
        {
            .open = driver_aren_open,
            .release = driver_aren_close,
            .unlocked_ioctl = driver_aren_ioctl,
#ifdef CONFIG_COMPAT
            .compat_ioctl = driver_aren_ioctl_compat,
#endif
        },
    },
    {
        // audio clock
        .name = "aclk0",
        .fops =
        {
            .open = driver_aclk_open,
            .release = driver_aclk_close,
            .unlocked_ioctl = driver_aclk_ioctl,
#ifdef CONFIG_COMPAT
            .compat_ioctl = driver_aclk_ioctl_compat,
#endif
        },
    },

    {
        // audio debug
        .name = "adbg0",
        .fops =
        {
            .open = driver_adbg_open,
            .release = driver_adbg_close,
            .unlocked_ioctl = driver_adbg_ioctl,
#ifdef CONFIG_COMPAT
            .compat_ioctl = driver_adbg_ioctl_compat,
#endif
            //.poll = KDRV_AUDIO_PollRenderer,
        },
    },

    /* *INDENT-ON* */
};


#define AUDIO_DEVICE_MAX        ARRAY_SIZE(driver_descs)

/* @0xffffffc010cb8d04 */
int aud_cdev_init (void)
{
    SINT32 retVal = 0;
    SINT32 i;
    dev_t dev;

    g_AudDev = (AUD_DEVICE_T *) OS_KMalloc (sizeof (AUD_DEVICE_T) * AUDIO_DEVICE_MAX);
    if (g_AudDev == NULL)
    {
        error ("out of memory. can't allocate %zd bytes\n",
            sizeof (AUD_DEVICE_T) * AUDIO_DEVICE_MAX);
        return -ENOMEM;
    }

    memset (g_AudDev, 0x0, sizeof (AUD_DEVICE_T) * AUDIO_DEVICE_MAX);

    if (g_AudMajor)
    {
        dev = MKDEV (g_AudMajor, g_AudMinor);
        retVal = register_chrdev_region (dev, AUDIO_DEVICE_MAX, AUDIO_MODULE);
    }
    else
    {
        retVal = alloc_chrdev_region (&dev, g_AudMinor, AUDIO_DEVICE_MAX, AUDIO_MODULE);
        g_AudMajor = MAJOR (dev);
    }

    if (retVal < 0)
    {
        error ("can't register audio device\n");
        return -EIO;
    }

    for (i = 0; i < AUDIO_DEVICE_MAX; i++)
    {
        /* initialize cdev structure with predefined variable */
        dev = MKDEV (g_AudMajor, g_AudMinor + i);
        cdev_init (&(g_AudDev[i].cdev), &driver_descs[i].fops);

        g_AudDev[i].devNo = dev;
        g_AudDev[i].cdev.owner = THIS_MODULE;
        if (driver_descs[i].fops_ptr)
            g_AudDev[i].cdev.ops = driver_descs[i].fops_ptr;
        else
            g_AudDev[i].cdev.ops = &driver_descs[i].fops;

        retVal = cdev_add (&g_AudDev[i].cdev, dev, 1);
        if (retVal)
        {
            error ("error (%d) while adding audio capture device (%d.%d)\n", retVal,
                MAJOR (dev), MINOR (dev));
            return -EIO;
        }

        OS_CreateDeviceClass (dev, "%s", driver_descs[i].name);
    }

    printk ("audio driver initialized\n");
    return retVal;
}

/* @0xffffffc010cb8f60 */
void aud_cdev_exit (void)
{
    SINT32 i;

    dev_t dev = MKDEV (g_AudMajor, g_AudMinor);

    /* remove all minor devicies and unregister current device */
    for (i = 0; i < AUDIO_DEVICE_MAX; i++)
    {
        cdev_del (&(g_AudDev[i].cdev));
    }

    unregister_chrdev_region (dev, AUDIO_DEVICE_MAX);
    OS_Free (g_AudDev);

    return;
}

MODULE_AUTHOR ("LGE");
MODULE_DESCRIPTION ("base driver");
MODULE_LICENSE ("GPL");
