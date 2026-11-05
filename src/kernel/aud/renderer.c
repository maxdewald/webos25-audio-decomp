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
 * orig/boot/Image-5.4.268-329.decompressed, 5.4.268-329.ptl4tv.5, aarch64).
 *
 * Base: lg-opensource/kdriver/core/aud/renderer.c (GPL-2.0, k25lp-5.4.268-292).
 * Addresses: kallsyms of the shipped image (build/scratch/a4/kallsyms.txt), loaded at
 * 0xffffffc010088000 (_stext = 0xffffffc010089000).
 * Each function tagged @0x... was compared against the shipped disassembly
 * (call sequence, string literals, constants, struct offsets). Struct offsets of
 * aud_renderer_info in the shipped image are identical to the published layout
 * (block_info_size 0x90; pipeline_bypass +0x18, uid_lipsync +0x3c,
 * uid_lipsync_bypass +0x40, is_bypass_created +0x4c, is_bypass_rts_PCM +0x4d,
 * is_lipsync_set_idle +0x51, list_param +0x70).
 * Bypass/connect/idle/default/upperbound/set_param paths were read instruction
 * by instruction; rts_*, pcm_format, event and basetime helpers by strings,
 * log line numbers and call targets only.
 * Shipped-only changes are marked "webOS25-shipped:".
 */

/** @defgroup Renderer
  * @{
  *
  * @brief
  * Renderer module for audio kernel driver
  * @detail
  *
  *  srcbin (mssrc/pcmcvt)
  *  lipsync
  *  gain
  *  delay ( for es path )
  *  sola  ( for media path )
  *
  *  possible input of renderer
  *    - decoder PCM
  *    - decoder ES
  *    - appsrc
  *
  *  possible output of renderer
  *    - mixer
  *    - output ( es path )
  *
  * @}
  */

/** @file
  * @ingroup Renderer
  *
  *  implementation for renderer
  *
  *  author	Jihoon Lee
  *  version	0.1
  *  date	2020.06.30
  *  note	Additional information.
  *
  */

//#include <linux/spinlock.h>
#include <linux/slab.h>

#include "log.h"
#include "kaimc.h"
#include "block.h"
#include "block_manager.h"
#include "renderer.h"
#include "hal/hal.h"
#include "imc/evt/module_evt_lipsync.h"
#include "imc/cmd/module_cmd_lipsync.h"
#include "imc/cmd/module_cmd_pcmcvt.h"
#include "imc/cmd/module_cmd_rts.h"
#include "ipc_regs.h"

logm_define (arender, log_level_noti);
#define trace(fmt,args...)	logm_trace(arender,fmt,##args)
#define debug(fmt,args...)	logm_debug(arender,fmt,##args)
#define info(fmt,args...)	logm_info(arender,fmt,##args)
#define noti(fmt,args...)	logm_noti(arender,fmt,##args)
#define error(fmt,args...)	logm_error(arender,fmt,##args)

#define ADEC_MAX    4
#define ALSA_PLAYBACK_DEC_NUM   4
#define ALSA_PLAYBACK_DAFC_HW   8
#define RENDERER_DAFC_INDEX     (ALSA_PLAYBACK_DEC_NUM + ALSA_PLAYBACK_DAFC_HW)
#define TIMESTAMP_MASK_NO_PCR   0x7FFFFFFF

static DEFINE_MUTEX (_ren_lock);

#define AUD_REN_LOCK()		mutex_lock(&_ren_lock)
#define AUD_REN_UNLOCK()	mutex_unlock(&_ren_lock)

#define NULL_CHECK(a, fmt, __if_action) { if (a == NULL) { error (fmt); __if_action;}}
#define IS_MEDIA(renderer) (renderer->clock_type == RENDERER_CLK_TYPE_GSTC || renderer->clock_type == RENDERER_CLK_TYPE_GSTC_1)

struct renderer_param
{
    const char *name;
    const char *alt_name;

    // function pointer
    int (*set_func) (struct aud_block_info *, unsigned int);
    int (*get_func) (struct aud_block_info *, unsigned int);
};

struct renderer_module_param
{
    struct list_head list;
    const char *name;
    struct renderer_param *param;
    int uid;
    int uid2;
};

struct renderer_list_param
{
    struct list_head list;

    int uid;                    // target module uid                 (+0x10)
    char *param;                // param name                        (+0x18)
    unsigned int val;           // param value                       (+0x20)
    /* webOS25-shipped: string-valued parameters were added; kzalloc size is
     * 0x30 and the string lives at +0x28 (NULL for numeric params). */
    char *str_val;              //                                   (+0x28)
};

struct renderer_list_uid_set
{
    struct list_head list;

    int prev;                   // previous uid of target module
    int next;                   // next uid of target module
};

struct aud_renderer_event_info
{
    char *name;
    int id;
    int (*cb_func) (void *, int, void *);
    int repeat;

    struct list_head list;
};

/**
 * AUDIO Renderer Information.
 */
struct aud_renderer_info
{
    struct aud_block_info parent;

    struct apipeline *pipeline_bypass;

    bool suspended;             // for suspend / resume
    bool sync;
    enum RENDERER_CLK_TYPE clock_type;

    int uid_rts;
    int uid_rts_bypass;
    int uid_src;
    int uid_pcmcvt_bypass;
    int uid_sola;
    int uid_lipsync;
    int uid_lipsync_bypass;
    int uid_gain;
    int uid_queue;

    bool is_bypass_created;
    bool is_bypass_rts_PCM;
    bool is_queue_created;
    bool is_rts_created;

    bool is_jp4k;

    bool is_lipsync_set_idle;

    int upperbound;
    int upperbound_bypass;

    struct list_head list_module_param; // parameter name for modules.. see @renderer_module_param
    struct list_head list_param;        // list of param which is set to module... see @renderer_list_param
    struct list_head list_event;
};

static int renderer_clear_prop_list (struct aud_renderer_info *renderer)
{
    struct renderer_list_param *param_list, *tmp;

    list_for_each_entry_safe (param_list, tmp, &renderer->list_param, list)
    {
        info ("free %s...\n", param_list->param);
        list_del (&param_list->list);
        kfree (param_list->param);
        /* webOS25-shipped: renderer_destroy (inlined copy) does three kfree()s */
        kfree (param_list->str_val);
        kfree (param_list);
    }

    return 0;
}

static int renderer_update_prop_list (struct aud_renderer_info *renderer, struct list_head *head)
{
    struct renderer_list_param *param_list;
    struct renderer_list_uid_set *list_uid_set;

    info ("called.\n");

    list_for_each_entry (param_list, &renderer->list_param, list)
    {
        trace ("%s : uid 0x%x\n", param_list->param, param_list->uid);
        list_for_each_entry (list_uid_set, head, list)
        {
            if (param_list->uid == list_uid_set->prev)
            {
                debug ("update uid 0x%x -> 0x%x\n", param_list->uid, list_uid_set->next);
                param_list->uid = list_uid_set->next;
                break;
            }
        }
    }

    return 0;
}

static int renderer_clear_uid_set (struct list_head *head)
{
    struct renderer_list_uid_set *list_uid_set, *tmp;

    list_for_each_entry_safe (list_uid_set, tmp, head, list)
    {
        list_del (&list_uid_set->list);
        kfree (list_uid_set);
    }

    return 0;
}

/* @0xffffffc010ca8700 */
static int renderer_add_uid_set (struct aud_renderer_info *renderer, int prev, int next,
    struct list_head *head)
{
    struct renderer_list_uid_set *list_uid_set;

    info ("0x%x - > 0x%x\n", prev, next);

    if (prev)
    {
        list_uid_set = kzalloc (sizeof (*list_uid_set), GFP_KERNEL);
        list_uid_set->prev = prev;
        list_uid_set->next = next;
        INIT_LIST_HEAD (&list_uid_set->list);
        list_add (&list_uid_set->list, head);
    }

    return 0;
}

int renderer_find_param (struct aud_block_info *block, char *name, int *val)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    struct renderer_list_param *param_list;

    info ("find param %s in param list...\n", name);

    // setting params....
    list_for_each_entry (param_list, &renderer->list_param, list)
    {
        trace ("param_list->param = %s\n", param_list->param);
        if (!strcmp (param_list->param, name))
        {
            info ("get %s = %u\n", param_list->param, param_list->val);
            *val = param_list->val;
            return 0;
        }
    }

    info ("can't find param %s\n", name);

    return -1;
}

// list of values whicth shall not be ignored...
const char *renderer_prop_whitelist[] = {
    "BaseCT",
    "BaseST",
    "target-gain",
    "fadetime-ms",
    "mute-input",
    "mute-output",
    "start-pts",
    "end-pts",
    "gap-handling",
    NULL
};

// find if property name is in white list....
/* @0xffffffc010ca8aa0 */
static bool renderer_in_whitelist (struct aud_renderer_info *renderer, char *prop)
{
    int n = 0;

    debug ("search prop %s in while list.. \n", prop);

    while (renderer_prop_whitelist[n])
    {
        trace ("renderer_prop_whitelist[%d] = %s\n", n, renderer_prop_whitelist[n]);
        if (!strcmp (renderer_prop_whitelist[n], prop))
        {
            debug ("%s found!\n", prop);
            return true;
        }
        n++;
    }

    return false;
}

/*
 * webOS25-shipped: the published renderer_set_property() was split into a
 * string-aware worker (_renderer_set_property.isra.0) plus a thin numeric
 * wrapper. The cache entry (list_param) keeps either `val` or `str_val`; it is
 * replayed by renderer_resume(). Properties set with aimc_set_property()
 * directly (drop_output_data, state, upperbound/count_transition) are NOT cached.
 */
/* @0xffffffc010ca8ee4 */
static int _renderer_set_property (struct aud_renderer_info *renderer, unsigned int target,
    const char *prop, unsigned int val, const char *str)
{
    struct renderer_list_param *param_list;

    if (str)
        debug ("set %s=%s to 0x%x\n", prop, str, target);
    else
        debug ("set %s=%u to 0x%x\n", prop, val, target);

    list_for_each_entry (param_list, &renderer->list_param, list)
    {
        if (target != param_list->uid || strcmp (prop, param_list->param))
            continue;

        if (str)
        {
            if (param_list->str_val && !strcmp (param_list->str_val, str)
                && !renderer_in_whitelist (renderer, (char *) prop))
            {
                debug ("same value (%s = %s)\n", prop, str);
                return 0;
            }
            debug ("update %s to 0x%x (%s -> %s)\n", prop, target, param_list->str_val, str);
            kfree (param_list->str_val);
            param_list->str_val = kstrdup (str, GFP_KERNEL);
            goto set;
        }

        if (param_list->val == val && !renderer_in_whitelist (renderer, (char *) prop))
        {
            debug ("same value (%s = %u)\n", prop, val);
            return 0;
        }
        debug ("update %s to 0x%x (%d -> %u)\n", prop, target, param_list->val, val);
        param_list->val = val;
        goto set;
    }

    param_list = kzalloc (sizeof (*param_list), GFP_KERNEL);
    param_list->param = kstrdup (prop, GFP_KERNEL);
    param_list->uid = target;
    param_list->val = val;
    if (str)
    {
        debug ("new %s to 0x%x (%s)\n", prop, target, str);
        param_list->str_val = kstrdup (str, GFP_KERNEL);
    }
    else
    {
        debug ("new %s to 0x%x (%u)\n", prop, target, val);
        param_list->str_val = NULL;
    }
    list_add (&param_list->list, &renderer->list_param);

  set:
    if (str)
        aimc_set_property (target, "%s=%s", prop, str);
    else
        aimc_set_property (target, "%s=%u", prop, val);

    return 0;
}

/* @0xffffffc010ca9390 */
static int renderer_set_property (struct aud_renderer_info *renderer, unsigned int target,
    char *prop, unsigned int val)
{
    debug ("set %s=%u to 0x%x\n", prop, val, target);

    _renderer_set_property (renderer, target, prop, val, NULL);

    return 0;
}


/* @0xffffffc010caa2e4 */
int renderer_set_gap_info (struct aud_block_info *block, unsigned int start_pts,
    unsigned int end_pts)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("set gap info %d ~ %d\n", start_pts, end_pts);

    NULL_CHECK (block, "block is NULLL\n", return -1);

    renderer_set_property (renderer, renderer->uid_lipsync, "start-pts", start_pts);
    renderer_set_property (renderer, renderer->uid_lipsync, "end-pts", end_pts);
    renderer_set_property (renderer, renderer->uid_lipsync, "gap-handling", 1);

    if (renderer->uid_lipsync_bypass > 0)
    {
        renderer_set_property (renderer, renderer->uid_lipsync_bypass, "start-pts", start_pts);
        renderer_set_property (renderer, renderer->uid_lipsync_bypass, "end-pts", end_pts);
        renderer_set_property (renderer, renderer->uid_lipsync_bypass, "gap-handling", 1);
    }

    return 0;
}

/* @0xffffffc010caa460 */
int renderer_set_endian_sign (struct aud_block_info *block, enum renderer_endian_t endian,
    enum renderer_sign_t sign)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("set endian = %d, sign = %d \n", endian, sign);

    NULL_CHECK (block, "block is NULLL\n", return -1);

    renderer_set_property (renderer, renderer->uid_src, "endian", (unsigned int) endian);
    renderer_set_property (renderer, renderer->uid_src, "sign", (unsigned int) sign);

    return 0;
}

/* @0xffffffc010caa560 */
int renderer_set_pcm_format (struct aud_block_info *block, struct renderer_pcm_format in,
    struct renderer_pcm_format out)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    PcmcvtCmdSetAllfmt param = {
        .InFs = 48000,.InCh = 2,.InFormat = 32,.InEndian = renderer_endian_le,.InSigned =
            renderer_sign_signed,
        .OutFs = 48000,.OutCh = 2,.OutFormat = 32
    };

    info ("set pcm format (set default in:%d out:%d\n", in.set_default, out.set_default);

    NULL_CHECK (block, "block is NULLL\n", return -1);

    if (in.set_default == false)
    {
        info ("in : rate %d %d ch %d %s %s\n", in.sample_rate, in.channel, in.bit_per_sample,
            (in.endian) ? "BE" : "LE", (in.sign) ? "UNSIGNED" : "SIGNED");
        param.InFs = in.sample_rate;
        param.InCh = in.channel;
        param.InEndian = in.endian;
        param.InFormat = in.bit_per_sample;
        param.InSigned = in.sign;
    }

    if (out.set_default == false)
    {
        info ("out: rate %d %d ch %d bit\n", out.sample_rate, out.channel, out.bit_per_sample);
        param.OutFs = out.sample_rate;
        param.OutCh = out.channel;
        param.OutFormat = out.bit_per_sample;
    }

    info ("param in : rate %d %d ch %d %s %s\n", param.InFs, param.InCh, param.InFormat,
        (param.InEndian) ? "BE" : "LE", (param.InSigned) ? "UNSIGNED" : "SIGNED");
    info ("param out: rate %d %d ch %d bit\n", param.OutFs, param.OutCh, param.OutFormat);

    aimc_send_command (NULL, PCMCVT_CMD_SET_ALLFMT, 0, renderer->uid_src, &param, sizeof (param));

    return 0;
}

/* @0xffffffc010caa884 */
int renderer_set_rts_out_freq (struct aud_block_info *block, unsigned int fast_freq,
    unsigned int normal_freq, unsigned int slow_freq)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    RtsCmdSetOutFreq param = { };

    info ("set rts out freq f:%d n:%d s:%d\n", fast_freq, normal_freq, slow_freq);

    NULL_CHECK (block, "block is NULLL\n", return -1);

    if (renderer->uid_rts == -1)
    {
        error ("rts is not created yet!");
        return -1;
    }

    param.fast_freq = fast_freq;
    param.normal_freq = normal_freq;
    param.slow_freq = slow_freq;

    aimc_send_command (NULL, RTS_CMD_SET_OUT_FREQ, 0, renderer->uid_rts, &param, sizeof (param));

    return 0;
}

/* @0xffffffc010caa9f4 */
int renderer_set_rts_skipping (struct aud_block_info *block, unsigned int skipping_recovery,
    unsigned int skipping_threshold)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    RtsCmdSetSkippingConfig param = { };

    info ("set rts skipping r:%d t:%d\n", skipping_recovery, skipping_threshold);

    NULL_CHECK (block, "block is NULLL\n", return -1);

    if (renderer->uid_rts == -1)
    {
        error ("rts is not created yet!");
        return -1;
    }

    param.skipping_recovery = skipping_recovery;
    param.skipping_threshold = skipping_threshold;

    aimc_send_command (NULL, RTS_CMD_SET_SKIPPING_CONFIG, 0, renderer->uid_rts, &param,
        sizeof (param));

    return 0;
}

/* @0xffffffc010caab44 */
int renderer_set_rts_threshold (struct aud_block_info *block, unsigned int upper_th,
    unsigned int lower_th, unsigned int recover_upper, unsigned int recover_lower)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    RtsCmdSetThreashold param = { };

    if (renderer->uid_rts == -1)
    {
        error ("rts is not created yet!");
        return -1;
    }

    param.upper_th = upper_th;
    param.lower_th = lower_th;
    param.recover_upper = recover_upper;
    param.recover_lower = recover_lower;

    aimc_send_command (NULL, RTS_CMD_SET_THRESHOLD, 0, renderer->uid_rts, &param, sizeof (param));

    return 0;
}

/* @0xffffffc010caac00 */
int renderer_set_rts_opmode (struct aud_block_info *block, unsigned int opmode)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    RtsCmdSetOperationMode param = { };

    info ("set rts opmode:%d \n", opmode);

    NULL_CHECK (block, "block is NULLL\n", return -1);

    if (renderer->uid_rts == -1)
    {
        error ("rts is not created yet!");
        return -1;
    }

    param.op_mode = opmode;

    aimc_send_command (NULL, RTS_CMD_SET_OPERATION_MODE, 0, renderer->uid_rts, &param,
        sizeof (param));

    return 0;
}

/* @0xffffffc010caad50 */
int renderer_set_rts_port_multiplier (struct aud_block_info *block, unsigned int ref_port,
    unsigned int numerator, unsigned int denominator)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    RtsCmdSetPortMultiplier param = { };

    info ("set rts ref_port:%d, numerator:%d, denominator:%d\n", ref_port, numerator, denominator);

    NULL_CHECK (block, "block is NULLL\n", return -1);

    if (renderer->uid_rts == -1)
    {
        error ("rts is not created yet!");
        return -1;
    }

    param.ref_port = ref_port;
    param.numerator = numerator;
    param.denominator = denominator;

    aimc_send_command (NULL, RTS_CMD_SET_PORT_MULTPLIER, 0, renderer->uid_rts, &param,
        sizeof (param));

    return 0;
}

/* @0xffffffc010caaec0 */
int renderer_set_rts_start (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("set rts start\n");

    NULL_CHECK (block, "block is NULLL\n", return -1);

    if (renderer->uid_rts == -1)
    {
        error ("rts is not created yet!");
        return -1;
    }

    aimc_send_command (NULL, ADEC_CMD_START, 0, renderer->uid_rts, NULL, 0);

    return 0;
}

/* @0xffffffc010caaff0 */
int renderer_set_jp4k (struct aud_block_info *block, bool is_jp4k)
{

    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("jp4k stream = %s\n", (is_jp4k) ? "TRUE" : "FALSE");

    renderer->is_jp4k = is_jp4k;

    return 0;
}

/* @0xffffffc010cab090 */
bool renderer_get_jp4k (struct aud_block_info * block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    return renderer->is_jp4k;
}

/* @0xffffffc010cab0a0 */
bool renderer_get_pcm_bypass (struct aud_block_info * block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    return renderer->is_bypass_rts_PCM;
}

/* @0xffffffc010cab0b0 */
int renderer_set_pcm_bypass_rts_out_channels (struct aud_block_info *block, unsigned int channels)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    if (renderer->is_bypass_rts_PCM)
    {
        renderer_set_property (renderer, renderer->uid_pcmcvt_bypass, "out-channels", channels);
        info ("set pcm bypass rts, out channels: %d\n", channels);
    }

    return 0;
}

/* @0xffffffc010ca94f0 */
static int renderer_set_sync (struct aud_block_info *block, unsigned int sync)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("sync = %s\n", (sync) ? "TRUE" : "FALSE");

    renderer_set_property (renderer, renderer->uid_lipsync, "LipsyncOnOff", sync);
    if (renderer->uid_lipsync_bypass > 0)
        renderer_set_property (renderer, renderer->uid_lipsync_bypass, "LipsyncOnOff", sync);

    renderer->sync = sync;
    return 0;
}

/* @0xffffffc010ca8c20 */
static int renderer_set_upperbound (struct aud_block_info *block, bool recover)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    if (recover)
    {
        info ("recover upperbound and count_transition\n");

        info ("upperbound = %d, %d\n", renderer->upperbound, renderer->upperbound_bypass);
        aimc_set_property (renderer->uid_lipsync, "upperbound=%d", renderer->upperbound);
        aimc_set_property (renderer->uid_lipsync, "count_transition=0");

        if (renderer->uid_lipsync_bypass > 0)
        {
            aimc_set_property (renderer->uid_lipsync_bypass, "upperbound=%d",
                renderer->upperbound_bypass);
            aimc_set_property (renderer->uid_lipsync_bypass, "count_transition=0");
        }
    }
    else
    {
#define SUB_UPPERBOUND 1
#define SUB_COUNT_TRANSITION 2
        info ("set upperbound %d, count_transition %d\n", SUB_UPPERBOUND, SUB_COUNT_TRANSITION);

        aimc_set_property (renderer->uid_lipsync, "upperbound=%d", SUB_UPPERBOUND);
        aimc_set_property (renderer->uid_lipsync, "count_transition=%d", SUB_COUNT_TRANSITION);

        if (renderer->uid_lipsync_bypass > 0)
        {
            aimc_set_property (renderer->uid_lipsync_bypass, "upperbound=%d", SUB_UPPERBOUND);
            aimc_set_property (renderer->uid_lipsync_bypass, "count_transition=%d",
                SUB_COUNT_TRANSITION);
        }
    }

    return 0;
}

/* @0xffffffc010ca9c80 */
static int renderer_set_clock_type (struct aud_block_info *block, unsigned int clock_type)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("clock_type = %u\n", clock_type);

    renderer_set_property (renderer, renderer->uid_lipsync, "clockType", (int) clock_type);
    if (renderer->uid_lipsync_bypass > 0)
        renderer_set_property (renderer, renderer->uid_lipsync_bypass, "clockType",
            (int) clock_type);

    if (clock_type == RENDERER_CLK_TYPE_MASTER)
    {
        renderer_set_property (renderer, renderer->uid_lipsync, "lowerbound", 100);
        renderer_set_property (renderer, renderer->uid_lipsync, "upperbound", 100);
        renderer_set_property (renderer, renderer->uid_lipsync, "freerunlowerbound", 120000);
        renderer_set_property (renderer, renderer->uid_lipsync, "freerunupperbound", 5000);

        if (renderer->uid_lipsync_bypass > 0)
        {
            renderer_set_property (renderer, renderer->uid_lipsync_bypass, "lowerbound", 100);
            renderer_set_property (renderer, renderer->uid_lipsync_bypass, "upperbound", 100);
            renderer_set_property (renderer, renderer->uid_lipsync_bypass, "freerunlowerbound",
                120000);
            renderer_set_property (renderer, renderer->uid_lipsync_bypass, "freerunupperbound",
                5000);
        }
        noti ("set audio master bound :: 100, 100, 120000, 5000\n");
    }

    renderer->clock_type = (enum RENDERER_CLK_TYPE) clock_type;

    if (!bm_is_connected (block, 0, PORT_OUT))
        renderer_set_upperbound (block, false);
    else
        renderer_set_upperbound (block, true);

    return 0;
}

/* @0xffffffc010ca9450 */
static int renderer_set_discard_param (struct aud_block_info *block, unsigned int discard_param)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("discard_param = %u\n", discard_param);

    renderer_set_property (renderer, renderer->uid_lipsync, "discard_param", discard_param);

    return 0;
}

/* @0xffffffc010ca9b74 */
static int renderer_set_upperbound_param (struct aud_block_info *block, unsigned int upperbound)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("set upperbound = %u\n", upperbound);

    renderer_set_property (renderer, renderer->uid_lipsync, "upperbound", upperbound);
    if (renderer->uid_lipsync_bypass)
        renderer_set_property (renderer, renderer->uid_lipsync_bypass, "upperbound", upperbound);

    renderer->upperbound = upperbound;
    renderer->upperbound_bypass = upperbound;

    if (!bm_is_connected (block, 0, PORT_OUT))
        renderer_set_upperbound (block, false);

    return 0;
}

/**
 * set lipsync module as idle
 *
 * audio sturred by lipsync go to REPEAT state
 * 1 second after language change or renderer-mixer disconnect/connect.
 * so, set lipsync as idle state to prevent audio sturred.
 *
 * set_idle is called by @alsa_decoder_start () and @rendere_postconnect ()
 * and cleared by @renderer_lipsync_clear_idle ()
 *
 * @param renderer [in] pointer of renderer info
 * @return 0 : OK
 * @see aud_renderer_info
*/
/* @0xffffffc010cab150 */
int renderer_lipsync_set_idle (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("called. already set? : %s\n", (renderer->is_lipsync_set_idle) ? "TRUE" : "FALSE");

    if (!renderer->is_lipsync_set_idle)
    {
        info ("set idle\n");

        aimc_set_property (renderer->uid_lipsync, "state=1");
        if (renderer->uid_lipsync_bypass)
            aimc_set_property (renderer->uid_lipsync_bypass, "state=1");

        renderer->is_lipsync_set_idle = true;
    }

    return 0;
}

/**
 * clear idle flag
 *
 * to prevent double-setting flag.
 * called by @renderer_postdisconnect (), @alsa_decoder_stop ()
 *
 * see @renderer_lipsync_set_idle ()
 *
 * @param renderer [in] pointer of renderer info
 * @return 0 : OK
 * @see aud_renderer_info
*/
/* @0xffffffc010cab410 */
int renderer_lipsync_clear_idle (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("called. flag : %s\n", (renderer->is_lipsync_set_idle) ? "TRUE" : "FALSE");

    renderer->is_lipsync_set_idle = false;

    return 0;
}

/* @0xffffffc010cab4a0 */
int renderer_lipsync_set_idle_time (struct aud_block_info *block, unsigned int msec)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("set idle time : %d\n", msec);

    renderer_set_property (renderer, renderer->uid_lipsync, "idle_time", msec);

    return 0;
}

/* @0xffffffc010cab540 */
void renderer_set_basetime_ipc (unsigned int index, unsigned int clock_base_time,
    unsigned int stream_base_time, bool is_set, bool is_all_set)
{
    unsigned int offs[][2] = {
        {AUD_IPCOFFS_DEC00_CLOCK_BASE_TIME, AUD_IPCOFFS_DEC00_STREAM_BASE_TIME},
        {AUD_IPCOFFS_DEC01_CLOCK_BASE_TIME, AUD_IPCOFFS_DEC01_STREAM_BASE_TIME},
        {AUD_IPCOFFS_DEC02_CLOCK_BASE_TIME, AUD_IPCOFFS_DEC02_STREAM_BASE_TIME},
        {AUD_IPCOFFS_DEC03_CLOCK_BASE_TIME, AUD_IPCOFFS_DEC03_STREAM_BASE_TIME},
    };

    if (index >= ARRAY_SIZE (offs))
    {
        error ("wrong index(%d)\n", index);
        return;
    }

    if (is_all_set)
    {
        int i;

        for (i = 0; i < ADEC_MAX; i++)
        {
            AUD_IPCREG_OFFS (offs[i][0]) = clock_base_time;
            AUD_IPCREG_OFFS (offs[i][1]) = stream_base_time;
        }
    }
    else
    {
        if (is_set)
            AUD_IPCREG (FIRST_BASETIME_VALID) = AUD_IPCREG (FIRST_BASETIME_VALID) | (1 << index);
        else
            AUD_IPCREG (FIRST_BASETIME_VALID) = AUD_IPCREG (FIRST_BASETIME_VALID) & ~(1 << index);

        AUD_IPCREG_OFFS (offs[index][0]) = clock_base_time;
        AUD_IPCREG_OFFS (offs[index][1]) = stream_base_time;
    }
}

/**
 * get lipsync master info from renderer module
 *
 * @param
 * @return
*/
/* @0xffffffc010cab6c0 */
int renderer_master_info_cb (void *param, int cb_param_len, void *cb_param)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) cb_param;
    LipsyncEvtNopcrBaseline *pBasetime = (LipsyncEvtNopcrBaseline *) param;
    int index;

    if (!cb_param && !param && cb_param_len != sizeof (LipsyncEvtNopcrBaseline))
    {
        error ("Master Info : Param Length Error[Expected:%zd][Input:%d]\n",
            sizeof (LipsyncEvtNopcrBaseline), cb_param_len);
        return -1;
    }

    index = bm_get_index_by_block_info ((struct aud_block_info *) renderer);

    //Write clock_base_time and stream_base_time Reg for VDEC lipsync.
    renderer_set_basetime_ipc (index, pBasetime->baseCT, pBasetime->baseST, true, false);

    //Send clock_base_time and stream_base_time.
    renderer_set_property (renderer, renderer->uid_lipsync, "BaseCT",
        pBasetime->baseCT & TIMESTAMP_MASK_NO_PCR);
    renderer_set_property (renderer, renderer->uid_lipsync, "BaseST",
        pBasetime->baseST & TIMESTAMP_MASK_NO_PCR);

    noti ("idx(%d) : clock(%u)(0x%x), stream(%u)(0x%x)\n", index, pBasetime->baseCT,
        pBasetime->baseCT, pBasetime->baseST, pBasetime->baseST);

    return 0;
}

static const char *_renderer_event_name[AUDIO_RENDERER_EVENT_MAX] = {
    [AUDIO_RENDERER_EVENT_MASTER_INFO] = "adec_master_info",
};

/**
 * register renderer event
 *
 * @param block [in] context of this block
 * @return pointer of renderer info
*/
/* @0xffffffc010cab824 */
int renderer_register_event (struct aud_block_info *block, enum audio_renderer_event event,
    int (*cb_func) (void *, int, void *), int repeat)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    int event_id = 0;
    struct aud_renderer_event_info *event_info = NULL;

    list_for_each_entry (event_info, &renderer->list_event, list)
    {
        if (!strcmp (_renderer_event_name[event], event_info->name))
        {
            info ("already registered event, %s\n", _renderer_event_name[event]);
            return 0;
        }
    }

    event_info =
        (struct aud_renderer_event_info *) kzalloc (sizeof (struct aud_renderer_event_info),
        GFP_KERNEL);
    if (!event_info)
    {
        error ("no mem\n");
        return -1;
    }

    event_id =
        aimc_register_named_event (renderer, cb_func, renderer->uid_lipsync,
        _renderer_event_name[event], repeat);

    event_info->name = kstrdup (_renderer_event_name[event], GFP_KERNEL);
    event_info->id = event_id;
    event_info->cb_func = cb_func;
    event_info->repeat = repeat;

    list_add (&event_info->list, &renderer->list_event);

    noti ("renderer register event, %s\n", _renderer_event_name[event]);
    return 0;
}

/**
 * unregister renderer event
 *
 * @param block [in] context of this block
 * @return pointer of decoder info
*/
/* @0xffffffc010caba30 */
int renderer_unregister_event (struct aud_block_info *block, enum audio_renderer_event event)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    int event_id = -1;
    struct aud_renderer_event_info *event_info = NULL;

    list_for_each_entry (event_info, &renderer->list_event, list)
    {
        if (!strcmp (_renderer_event_name[event], event_info->name))
        {
            event_id = event_info->id;
            break;
        }
    }

    if (event_id > 0)
    {
        aimc_unregister_named_event (event_id);

        list_del (&event_info->list);
        kfree (event_info->name);
        kfree (event_info);

        noti ("renderer unregister event, %s\n", _renderer_event_name[event]);
    }
    else
    {
        noti ("event is not registered, %s\n", _renderer_event_name[event]);
    }

    return 0;
}

/* @0xffffffc010ca95d0 */
static int renderer_set_default (struct aud_block_info *block, unsigned int id)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    int uid_lipsync = 0, uid_rts_bypass = 0, uid_pcmcvt_bypass = 0;

    info ("id = %u\n", id);

    switch (id)
    {
        case 0:
            // lipsync default setting
            renderer_set_property (renderer, renderer->uid_lipsync, "lowerbound", 32);
            renderer_set_property (renderer, renderer->uid_lipsync, "upperbound", 32);
            renderer->upperbound = 32;

            // gain default setting
            renderer_set_property (renderer, renderer->uid_gain, "gain", 0x800000);
            renderer_set_property (renderer, renderer->uid_gain, "mute-input", 0);
            renderer_set_property (renderer, renderer->uid_gain, "mute-output", 0);

            // sola defalut setting
            renderer_set_property (renderer, renderer->uid_sola, "rate-in", 1000);
            renderer_set_property (renderer, renderer->uid_sola, "rate-out", 1000);
            break;
        case 1:
            uid_lipsync = renderer->uid_lipsync_bypass;

            // lipsync default setting
            renderer_set_property (renderer, uid_lipsync, "LipsyncOnOff", renderer->sync);
            renderer_set_property (renderer, uid_lipsync, "clockType", renderer->clock_type);
            renderer_set_property (renderer, uid_lipsync, "dataType", 1);
            renderer_set_property (renderer, uid_lipsync, "bytePerSample", 2);
            renderer_set_property (renderer, uid_lipsync, "enableNoti", 0);
            renderer_set_property (renderer, uid_lipsync, "lowerbound", 150);
            renderer_set_property (renderer, uid_lipsync, "upperbound", 150);
            renderer_set_property (renderer, uid_lipsync, "idle_time", 3);

            renderer->upperbound_bypass = 150;

            break;

        case 2:
            uid_rts_bypass = renderer->uid_rts_bypass;
            uid_pcmcvt_bypass = renderer->uid_pcmcvt_bypass;

            renderer_set_property (renderer, uid_pcmcvt_bypass, "out-channels", 2);
            //renderer_set_property (renderer, uid_rts_bypass, "clockType", renderer->clock_type);

            break;

        default:
            error ("not proper id : %u\n", id);
            break;
    }

    return 0;
}

struct renderer_param param_lipsync[] = {
    /* lipsync */
    {"LipsyncOnOff"},
    {"clockType"},
    {"lowerbound"},
    {"upperbound",.set_func = renderer_set_upperbound_param},
    {"freerunlowerbound"},
    {"freerunupperbound"},
    {"ConstantOffset"},
    {"BaseCT"},
    {"BaseST"},
    {"ui32BaseCT"},
    {"ui32BaseST"},
    {"fs"},
    {"offset"},
    {"speed-in"},
    {"param_channel"},
    {"dataType"},
    {"idle_time"},
    {"debug-skip-count"},
    {"debug-repeat-count"},
    {"debug-discontinuity-count"},
    {"debug-free-run-count"},
    {"autotest_mode"},
    {"presented-pts-index"},
    {"count_transition"},
    {"discard_param",.set_func = renderer_set_discard_param},
    {"block-lipsync", "block-module"},
    {NULL},
};

struct renderer_param param_gain[] = {
    {"gain"},
    {"final-gain"},
    {"current-gain"},
    {"channel"},
    {"mute-input"},
    {"mute-output"},
    {"fadetime-ms"},
    {"target-gain"},
    {"fade-type"},
    {NULL},
};

struct renderer_param param_sola[] = {
    {"rate-in"},
    {NULL},
};

struct renderer_param param_src[] = {
    /* pcmcvt */
    {"in-sampling-rate"},
    {"out-sampling-rate"},
    {"in-channels"},
    {"out-channels"},
    {"in-bit-per-sample"},
    {"out-bit-per-sample"},
    {"generate-lfe"},
    {"over-protect"},
    {"out-samples"},
    {"running-mode"},
    {"acmod"},
    {"pcm_gain", "gain"},
    {"gain-enable"},
    {"endian"},
    {"sign"},

    /* mssrc */
    {"sampling-rate"},
    {"channels"},
    {"channel-mode"},
    {NULL},
};

struct renderer_param param_rts[] = {
    /* rts */
    {"queued-byte"},
    {"max-length"},
    {"stride-shift"},
    {"buffering-ms"},
    {"op-mode"},
    {NULL},
};

struct renderer_param param_queue[] = {
    /* queue */
    {"queue-max-length", "max-length"},
    {"queued-length"},
    {"padding-length"},
    {"max-queued-length"},
    {"set-flush"},
    {"bypass"},
    {"block-queue"},
    {NULL},
};


struct renderer_param param_global[] = {
    {"default",.set_func = renderer_set_default},
    {"sync",.set_func = renderer_set_sync},
    {"clock_type",.set_func = renderer_set_clock_type},
    {NULL},
};

/*
 * webOS25-shipped: an extra `str` argument (x3) carries a non-numeric value from
 * renderer_set_param(); arguments are (block, name, data, str, ret_value, is_set).
 * With a string and no set_func the value goes through
 * _renderer_set_property(.., -1, str) for uid and uid2 ("renderer_set_property2"
 * log tag). Numeric path is unchanged.
 */
/* @0xffffffc010ca9ec0 */
static int renderer_run_param (struct aud_block_info *block, char *name, unsigned int data,
    const char *str, int *ret_value, bool is_set)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    struct renderer_module_param *mp;
    int ret = -1;

    if (is_set)
        info ("set param : %s = %u\n", name, data);
    else
        info ("get param : %s\n", name);

    list_for_each_entry (mp, &renderer->list_module_param, list)
    {
        struct renderer_param *param_cmd = NULL;
        int i = 0;

        param_cmd = mp->param;

        while (param_cmd[i].name)
        {
            trace ("param = %s\n", param_cmd[i].name);
            if (!strcmp (name, param_cmd[i].name))
            {
                debug ("%s(%d)\n", mp->name, mp->uid);

                // if there is alter name, use this.
                // ex) pcmcvt and gain have same prop name, 'gain'
                //   so, pcmcvt use alt name, 'pcm_gain'
                if (param_cmd[i].alt_name)
                    name = (char *) param_cmd[i].alt_name;

                if (is_set == true)
                {
                    if (param_cmd[i].set_func)
                        ret = param_cmd[i].set_func (block, data);
                    else if (str)
                    {
                        debug ("set %s=%s to 0x%x\n", name, str, mp->uid);
                        ret = _renderer_set_property (renderer, mp->uid, name, -1, str);

                        if (mp->uid2 > 0)
                        {
                            debug ("set %s=%s to 0x%x\n", name, str, mp->uid2);
                            _renderer_set_property (renderer, mp->uid2, name, -1, str);
                        }
                    }
                    else
                    {
                        ret = renderer_set_property (renderer, mp->uid, name, data);

                        if (mp->uid2 > 0)
                            renderer_set_property (renderer, mp->uid2, name, data);
                    }
                }
                else
                {
                    if (param_cmd[i].get_func)
                        ret = param_cmd[i].get_func (block, data);
                    else
                        ret = aimc_get_property (mp->uid, name, ret_value);
                }

                goto exit;
            }
            i++;
        }
    }

  exit:
    return ret;
}

/**
 * get parameter to renderer
 *
 * renderer_get_param(renderer, &gain, sizeof(int), "gain");
 *
 * @param renderer [in] pointer of renderer info
 * @param name [in] name of param
 * @param val [out] pointer of returned value
 * @return 0 : OK, other : NOT_OK
 * @see aud_renderer_info
*/
/* @0xffffffc010cabba0 */
int renderer_get_param (struct aud_block_info *block, char *name, int *val)
{
    int ret = -1;

    info ("get %s\n", name);

    ret = renderer_run_param (block, name, 0, NULL, val, false);
    if (ret)
    {
        error ("error in renderer_run_param()\n");
        goto exit;
    }

    info ("%s = %d\n", name, *val);

  exit:
    return ret;
}

#define RENDERER_PARAM_DELIMITER " =\\"

/*
 * webOS25-shipped: inlined classifier (log tag "renderer_is_number"): "0x"/"0X"
 * prefix -> "hexa", up to 6 leading decimal digits -> "decimal", else "string".
 */
static const char *renderer_is_number (const char *data, unsigned int *base)
{
    size_t len, i;

    if (data[0] == '0' && (data[1] | 0x20) == 'x')
    {
        *base = 16;
        return "hexa";
    }

    len = strlen (data);
    if (len > 6)
        len = 6;
    for (i = 0; i < len && data[i]; i++)
    {
        if (data[i] < '0' || data[i] > '9')
        {
            *base = 0;
            return "string";
        }
    }
    *base = 10;
    return "decimal";
}

/*
 * webOS25-shipped: values are no longer required to be decimal. Hex and
 * decimal go through kstrtoull(); anything else is forwarded as a string
 * (renderer_run_param str argument). The published error path
 * "error in strtoul! %d" is gone.
 */
/* @0xffffffc010cabd00 */
int renderer_set_param (struct aud_block_info *block, char *pStrParam, ...)
{
    va_list ap;
    char *param, *name, *str;

    NULL_CHECK (block, "block is NULLL\n", return -1);

    va_start (ap, pStrParam);
    param = kvasprintf (GFP_KERNEL, pStrParam, ap);
    va_end (ap);

    str = param;

    while ((name = strsep (&str, RENDERER_PARAM_DELIMITER)))
    {
        char *data;
        unsigned long long ull_data = 0;
        unsigned int base = 0;
        const char *kind;
        const char *str_data = NULL;

        data = strsep (&str, RENDERER_PARAM_DELIMITER);
        if (!data)
        {
            error ("no data for data. str %s\n", str);
            break;
        }

        kind = renderer_is_number (data, &base);
        debug ("string is %s\n", kind);
        if (base)
        {
            kstrtoull (data, base, &ull_data);
            debug (" %s = %s, %lu\n", name, data, (unsigned long) ull_data);
            info ("%s = %s\n", name, data);
        }
        else
        {
            debug (" %s = %s\n", name, data);
            str_data = data;
        }

        renderer_run_param (block, name, (unsigned int) ull_data, str_data, NULL, true);
    }

    kfree (param);

    info ("done\n");
    return 0;
}

/* @0xffffffc010ca87c0 */
static int renderer_registry_bypass (struct aud_renderer_info *renderer, char *name, int uid)
{
    struct renderer_module_param *mp;

    list_for_each_entry (mp, &renderer->list_module_param, list)
    {
        debug ("module %s\n", mp->name);

        if (!strcmp (name, mp->name))
        {
            debug ("add uid %d to %s\n", uid, mp->name);
            mp->uid2 = uid;
            break;
        }
    }
    return 0;
}

/* @0xffffffc010cac1c4 */
void renderer_module_registry (struct aud_renderer_info *renderer, const char *name,
    struct renderer_param *param, int uid)
{
    struct renderer_module_param *mp;

    info ("%s : registry\n", name);

    mp = kzalloc (sizeof (*mp), GFP_KERNEL);

    mp->name = name;
    mp->param = param;
    mp->uid = uid;
    mp->uid2 = -1;

    INIT_LIST_HEAD (&mp->list);
    list_add (&mp->list, &renderer->list_module_param);
}

static void renderer_module_update_uid (struct aud_renderer_info *renderer, const char *name,
    int uid)
{
    struct renderer_module_param *mp;

    info ("%s : registry update\n", name);

    list_for_each_entry (mp, &renderer->list_module_param, list)
    {
        debug ("module %s\n", mp->name);

        if (!strcmp (name, mp->name))
        {
            debug ("update uid %d to %s\n", uid, mp->name);
            mp->uid = uid;
            break;
        }
    }
}

/* @0xffffffc010cac290 */
void renderer_clear_registry (struct aud_renderer_info *renderer)
{
    struct renderer_module_param *mp, *next;

    list_for_each_entry_safe (mp, next, &renderer->list_module_param, list)
    {
        trace ("remove %s registry..\n", mp->name);
        list_del (&mp->list);
        kfree (mp);
    }
}

/* @0xffffffc010cac670 */
int renderer_add_rts (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    NULL_CHECK (block, "block is NULLL\n", return -1);

    info ("add rts module into renderer\n");

    if (renderer->uid_rts < 0)
        renderer->uid_rts = apipeline_prependf (block->pipeline, "rts");

    renderer->is_rts_created = true;

    renderer_module_update_uid (renderer, "rts", renderer->uid_rts);

    debug ("rts module 0x%x added.\n", renderer->uid_rts);

    return 0;
}

/* @0xffffffc010cac8f0 */
int renderer_add_queue (struct aud_block_info *block, bool forced)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    NULL_CHECK (block, "block is NULLL\n", return -1);

    if (aud_hal_get_number_of_dsp () > 2 && forced == false)
    {
        // dec(dsp0) - glue - renderer(dsp2)
        // glue can substitue queue
        // but media can forcely add queue..
        info ("4 dsp don't need queue.. we have glue..\n");
        return 0;
    }

    info ("add queue module into renderer\n");

    if (renderer->uid_queue < 0)
        renderer->uid_queue = apipeline_prependf (block->pipeline, "queue");

    renderer_module_registry (renderer, "queue", param_queue, renderer->uid_queue);
    renderer->is_queue_created = true;

    debug ("queue module 0x%x added.\n", renderer->uid_queue);

    return 0;
}

/*
 * Shipped code matches the published source exactly (same call order, same
 * fields). Notes for the ES path (rtsPCM == false):
 *  - chain on DSP2 (renderer impl preffered_processor = 4 = ADEC_CORE_DSP2):
 *      "renderer_bypass": queue -> lipsync
 *  - the ES "queue" uid is NOT stored and NOT registered in list_module_param,
 *    so renderer_set_param("block-queue=..") / "queue-max-length=.." from
 *    aren_start/aren_stop/aren_set_queue_max_length never reach it; it stays at
 *    the DSP default.
 *  - "lipsync" is registered as uid2 of the main "lipsync" entry, so every
 *    lipsync param set via renderer_set_param (LipsyncOnOff, clockType,
 *    BaseCT/BaseST, lowerbound/upperbound...) is mirrored to the bypass lipsync.
 *  - renderer_set_default(1) sets dataType=1 (ES) and bytePerSample=2 on the
 *    bypass lipsync, lowerbound/upperbound 150, idle_time 3, enableNoti 0.
 *  - in/out port BYPASS_PORT (0x100) of the renderer block forward to port 0 of
 *    this pipeline.
 */
/* @0xffffffc010ca9894 */
static void renderer_create_bypass_pipeline (struct aud_block_info *block, bool rtsPCM)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    int preferred_proc = bm_get_preffered_processor (block);

    if (renderer->pipeline_bypass)
        apipeline_del (renderer->pipeline_bypass);

    renderer->pipeline_bypass = apipeline_new2 (preferred_proc, "renderer_bypass");

    if (rtsPCM)
    {
        renderer->uid_rts_bypass = apipeline_appendf (renderer->pipeline_bypass, "rts");
        renderer->uid_pcmcvt_bypass = apipeline_appendf (renderer->pipeline_bypass, "srcbin");
        renderer_registry_bypass (renderer, "rts", renderer->uid_rts_bypass);

        renderer_set_default (block, 2);
    }
    else
    {
        apipeline_appendf (renderer->pipeline_bypass, "queue");
    }

    renderer->uid_lipsync_bypass = apipeline_appendf (renderer->pipeline_bypass, "lipsync");

    /* register bypass path to lipsync in main pipeline */
    renderer_registry_bypass (renderer, "lipsync", renderer->uid_lipsync_bypass);
    renderer_set_default (block, 1);

    bm_register_port_forward (block, PORT_IN, BYPASS_PORT, renderer->pipeline_bypass, 0);
    bm_register_port_forward (block, PORT_OUT, BYPASS_PORT, renderer->pipeline_bypass, 0);

    renderer->is_bypass_created = true;
    renderer->is_bypass_rts_PCM = rtsPCM;
}

/* @0xffffffc010cacab0 */
static void renderer_create_pipeline (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    int preferred_proc = bm_get_preffered_processor (block);
    int index = bm_get_index_by_block_info (block);

    info ("initialize renderer\n");

    block->pipeline = apipeline_new2 (preferred_proc, "renderer");

    renderer->uid_src = apipeline_appendf (block->pipeline, "srcbin");

    INIT_LIST_HEAD (&renderer->list_module_param);

    if (index != RENDERER_DAFC_INDEX)
    {
        renderer->uid_sola = apipeline_appendf (block->pipeline, "sola");
        renderer->uid_lipsync = apipeline_appendf (block->pipeline, "lipsync");
        renderer->uid_gain = apipeline_appendf (block->pipeline, "gain");
        renderer->uid_queue = -1;
        renderer->uid_rts = -1;

        if (renderer->is_queue_created)
            renderer_add_queue (block, true);
        if (renderer->is_rts_created)
            renderer_add_rts (block);

        renderer_module_registry (renderer, "lipsync", param_lipsync, renderer->uid_lipsync);
        renderer_module_registry (renderer, "gain", param_gain, renderer->uid_gain);
        renderer_module_registry (renderer, "sola", param_sola, renderer->uid_sola);
        renderer_module_registry (renderer, "rts", param_rts, renderer->uid_rts);
    }
    renderer_module_registry (renderer, "src", param_src, renderer->uid_src);
    renderer_module_registry (renderer, "global", param_global, -1);

    renderer_set_default (block, 0);

    if (renderer->is_bypass_created)
    {
        renderer_create_bypass_pipeline (block, renderer->is_bypass_rts_PCM);
    }
}


/* @0xffffffc010cad1d0 */
int renderer_get_uid_pcm_bypass_head (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    if (renderer->is_bypass_created && renderer->is_bypass_rts_PCM)
    {
        debug ("already created! uid : 0x%x\n", renderer->uid_rts_bypass);
        goto exit;
    }

    renderer_create_bypass_pipeline (block, true);

    info ("PCM bypass pipeline is created. head uid : 0x%x\n", renderer->uid_rts_bypass);

  exit:
    return renderer->uid_rts_bypass;
}

/* @0xffffffc010cad2c0 */
int renderer_get_uid_bypass_tail (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    if (!renderer->is_bypass_created)
    {
        info ("bypass is not yet created..\n");
        return -1;
    }

    return apipeline_tail (renderer->pipeline_bypass);
}

/*
 * Called from aren_get_uid(is_bypass=true) when pcm_bypass == pcm_multi_ch == 0
 * (AREN_GET_BYPASS_UID). Only creates on first use (is_bypass_created), so a
 * PCM bypass pipeline created earlier is returned unchanged for ES too.
 * The "bypass is not yet created.." line is logged at error level (published:
 * error too) but is the normal first-use path.
 */
/* @0xffffffc010cad340 */
int renderer_get_uid_es_bypass_head (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    if (!renderer->is_bypass_created)
    {
        error ("bypass is not yet created..\n");
        renderer_create_bypass_pipeline (block, false);
    }

    return apipeline_head (renderer->pipeline_bypass);
}

/* @0xffffffc010ca8e00 */
static int renderer_predisconnect (struct aud_block_info *block, int port, bool in)
{
    NULL_CHECK (block, "block is NULLL\n", return -1);

    info ("called\n");

    // prevent renderer to run excessively
    if (in == false)
        renderer_set_upperbound (block, false);

    return 0;
}

/* @0xffffffc010ca8910 */
static int renderer_postdisconnect (struct aud_block_info *block, int port, bool in)
{
    NULL_CHECK (block, "block is NULLL\n", return -1);

    info ("called\n");

    if (in == false)
    {
        // clear idle flag to set lipsync to idle state on post disconnect...
        info ("post disconnect clear idle flag (to set lipsync to idle)\n");

        renderer_lipsync_clear_idle (block);
    }

    return 0;
}

/*
 * Called by _bm_connect_block_locked (@0xffffffc010c9f544, via impl+0x30) BEFORE
 * apipeline_connect, for both ends: (out_block, port_out, PORT_OUT=false) and
 * (in_block, port_in, PORT_IN=true).
 *
 * drop_output_data set sites (shipped, string xrefs to "drop_output_data=1"):
 * only here (+0x98, +0x128). Set for every OUTPUT-side connect of the
 * renderer block, regardless of which port: a connect of main port 0 to the
 * mixer also sets it on the bypass lipsync if uid_lipsync_bypass != 0.
 *
 * Note: the kernel string is "drop_output_data" (underscores). The DSP-side
 * common module property is "drop-output-data" (dsp0/1/2 images, libaa
 * module.c:105) and DSP2 aud_module_set_prop (@0x600759cc) matches names with
 * strcmp(), so on DSP2 this property is most likely silently ignored by the
 * lipsync module, unless lipsync defines its own "drop_output_data" property
 * (no such string exists in the dsp2 image). See report.
 */
/* @0xffffffc010ca99b0 */
static int renderer_preconnect (struct aud_block_info *block, int port, bool in)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    NULL_CHECK (block, "block is NULLL\n", return -1);

    info ("called\n");

    /* if got connection to bypass port, make! */
    if (port == BYPASS_PORT && !renderer->pipeline_bypass)
    {
        info ("bypass path.. so make bypass path!\n");

        renderer_create_bypass_pipeline (block, false);
    }

    if (in == false)
    {
        info ("preconnect drop output data\n");

        // use aimc_set_property because don't want leave record for suspend/resume..
        aimc_set_property (renderer->uid_lipsync, "drop_output_data=1");
        if (renderer->uid_lipsync_bypass)
            aimc_set_property (renderer->uid_lipsync_bypass, "drop_output_data=1");

        renderer_set_upperbound (block, true);
    }

    return 0;
}


/*
 * Called by _bm_connect_block_locked only after apipeline_connect() returned 0
 * (impl+0x38). If apipeline_connect fails, the function returns early and the
 * preceding "drop_output_data=1" is never cleared (see report).
 * Only clear site of drop_output_data ("drop_output_data=0" xref +0xd8).
 */
/* @0xffffffc010cab264 */
static int renderer_postconnect (struct aud_block_info *block, int port, bool in)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    NULL_CHECK (block, "block is NULLL\n", return -1);

    info ("called\n");

    if (in == false)
    {
        info ("postconnect change lipsync state to IDLE forcely\n");

        // forcely set as idle when connection
        renderer_lipsync_clear_idle (block);
        renderer_lipsync_set_idle (block);

        // use aimc_set_property because don't want leave record for suspend/resume..
        aimc_set_property (renderer->uid_lipsync, "drop_output_data=0");
        if (renderer->uid_lipsync_bypass)
            aimc_set_property (renderer->uid_lipsync_bypass, "drop_output_data=0");
    }

    return 0;
}

/* @0xffffffc010cac5d0 */
static int renderer_suspend (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("suspend renderer..\n");

    if (block->pipeline)
        apipeline_del (block->pipeline);

    if (renderer->pipeline_bypass)
        apipeline_del (renderer->pipeline_bypass);

    renderer_clear_registry (renderer);

    block->pipeline = NULL;
    renderer->pipeline_bypass = NULL;

    renderer->suspended = true;

    return 0;
}



/* @0xffffffc010cacdd0 */
static int renderer_resume (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;
    struct renderer_list_param *param_list;
    struct list_head list_uid_set;

    int uid_rts;
    int uid_rts_bypass;
    int uid_src;
    int uid_pcmcvt_bypass;
    int uid_sola;
    int uid_lipsync;
    int uid_lipsync_bypass;
    int uid_gain;
    int uid_queue;
    int index = bm_get_index_by_block_info (block);

    info ("resume renderer..\n");

    if (renderer->suspended == false)
    {
        renderer_suspend (block);
    }

    // backup uids
    uid_src = renderer->uid_src;
    if (index != RENDERER_DAFC_INDEX)
    {
        uid_rts = renderer->uid_rts;
        uid_rts_bypass = renderer->uid_rts_bypass;
        uid_pcmcvt_bypass = renderer->uid_pcmcvt_bypass;
        uid_sola = renderer->uid_sola;
        uid_lipsync = renderer->uid_lipsync;
        uid_lipsync_bypass = renderer->uid_lipsync_bypass;
        uid_gain = renderer->uid_gain;
        uid_queue = renderer->uid_queue;
    }

    renderer_create_pipeline (block);

    INIT_LIST_HEAD (&list_uid_set);
    renderer_add_uid_set (renderer, uid_src, renderer->uid_src, &list_uid_set);
    if (index != RENDERER_DAFC_INDEX)
    {
        renderer_add_uid_set (renderer, uid_rts, renderer->uid_rts, &list_uid_set);
        renderer_add_uid_set (renderer, uid_rts_bypass, renderer->uid_rts_bypass, &list_uid_set);
        renderer_add_uid_set (renderer, uid_pcmcvt_bypass, renderer->uid_pcmcvt_bypass,
            &list_uid_set);
        renderer_add_uid_set (renderer, uid_sola, renderer->uid_sola, &list_uid_set);
        renderer_add_uid_set (renderer, uid_lipsync, renderer->uid_lipsync, &list_uid_set);
        renderer_add_uid_set (renderer, uid_lipsync_bypass, renderer->uid_lipsync_bypass,
            &list_uid_set);
        renderer_add_uid_set (renderer, uid_gain, renderer->uid_gain, &list_uid_set);
        renderer_add_uid_set (renderer, uid_queue, renderer->uid_queue, &list_uid_set);
    }

    renderer_update_prop_list (renderer, &list_uid_set);
    renderer_clear_uid_set (&list_uid_set);

    // setting params....
    /* webOS25-shipped: string params are replayed as "%s=%s".
     * drop_output_data / state / upperbound / count_transition are not in
     * list_param and are not replayed; the bypass lipsync gets whatever the DSP
     * module default is after resume until the next connect re-runs
     * pre/postconnect (bm_resume reconnects pipelines via apipeline_connect
     * directly, without calling pre_connect/post_connect). */
    list_for_each_entry (param_list, &renderer->list_param, list)
    {
        debug ("set %s(%u,%s) to 0x%x..\n", param_list->param, param_list->val,
            param_list->str_val, param_list->uid);
        if (param_list->str_val)
            aimc_set_property (param_list->uid, "%s=%s", param_list->param, param_list->str_val);
        else
            aimc_set_property (param_list->uid, "%s=%u", param_list->param, param_list->val);
    }

    renderer->suspended = false;
    return 0;
}

/**
 * create renderer
 *
 * Media path - (buffer) - srcbin (mssrc/pcmcvt/sola) - lipsync - gain
 *
 * @param index [in] index of renderer
 * @return pointer of renderer info
*/
/* @0xffffffc010caccc0 */
int renderer_create (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("create renderer\n");

    renderer->sync = false;
    renderer->clock_type = RENDERER_CLK_TYPE_GSTC;
    renderer->suspended = false;
    renderer->is_bypass_created = false;
    renderer->is_rts_created = false;
    renderer->is_queue_created = false;
    renderer->upperbound = 32;
    renderer->upperbound_bypass = 150;

    INIT_LIST_HEAD (&renderer->list_param);
    INIT_LIST_HEAD (&renderer->list_event);

    renderer_create_pipeline (block);

    block->isOpened = true;

    info ("done\n");
    return 0;
}


/**
 * close renderer
 *
 * @param renderer [in] pointer of renderer info
 * @return void
 * @see aud_renderer_info
*/
/* @0xffffffc010cac374 */
void renderer_destroy (struct aud_block_info *block)
{
    struct aud_renderer_info *renderer = (struct aud_renderer_info *) block;

    info ("called\n");

    AUD_REN_LOCK ();

    if (block == NULL)
    {
        error ("block is NULL!!\n");
        goto exit;
    }

    if (block->isOpened == false)
    {
        error ("renderer is already closed. \n");
        goto exit;
    }

    // destroy pipeline (inside modules are destroyed automatically)
    if (block->pipeline)
        apipeline_del (block->pipeline);

    // destroy pipeline (inside modules are destroyed automatically)
    if (renderer->pipeline_bypass)
        apipeline_del (renderer->pipeline_bypass);
    renderer->is_bypass_created = false;

    // clear module param registry
    renderer_clear_registry (renderer);
    renderer_clear_prop_list (renderer);

    block->isOpened = false;

  exit:
    AUD_REN_UNLOCK ();
    info ("done\n");
}

static struct audio_block_impl block_impl = {
    .block_info_size = sizeof (struct aud_renderer_info),
    .block_type = "renderer",
    .preffered_processor = ADEC_CORE_DSP2,
    .constructor = renderer_create,
    .destructor = renderer_destroy,
    .pre_connect = renderer_preconnect,
    .post_connect = renderer_postconnect,
    .pre_disconnect = renderer_predisconnect,
    .post_disconnect = renderer_postdisconnect,
    .suspend = renderer_suspend,
    .resume = renderer_resume,
};

/* @0xffffffc010cad3d0 */
void renderer_initializer (void)
{
    bm_block_registry (&block_impl);
}

/* @0xffffffc010cad3f0 */
void renderer_change_output (void)
{

}
