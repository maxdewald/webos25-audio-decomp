// SPDX-License-Identifier: GPL-2.0-only
/*
 * Source-assisted reconstruction of Image-5.4.268-329.decompressed, kernel
 * 5.4.268-329.ptl4tv.5, SHA256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Offline instruction evidence: build/scratch/seek-chain/image-evidence.json.
 * Address-tagged bodies were checked against the shipped AArch64 instructions.
 * Kernel allocation, locking, logging and workqueue primitives remain external.
 */
/* Base: lg-opensource/kdriver/core/aud/clock.c (GPL-2.0).
 * Tagged output APIs match the public control flow, stores and indirect call.
 * Untagged clock-worker/input bodies are retained public-source dependencies;
 * they are NOT claimed binary-verified or counted as recovered coverage.
 */
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


/* Clock state for speaker, headphone, optical and ARC/eARC outputs. */
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/mutex.h>

#include "log.h"
#include "block.h"
#include "block_manager.h"
#include "kaimc.h"
#include "common/imc/cmd/module_cmd_clock.h"
#include "util/utils.h"

#include "clock.h"
#include "hal/hal_clock.h"

/*------------------------------------------------------------------------------
    Constant Definitions
------------------------------------------------------------------------------*/
#define TICKS_PER_SEC           (90000)
#define BOUND_IN_TICK           (TICKS_PER_SEC / 1000)  // 1ms
#define RUN_PERIOD_IN_SEC       (5)
#define RUN_PERIOD_IN_MSEC      (RUN_PERIOD_IN_SEC * 1000)
#define RUN_PERIOD_IN_TICK      (RUN_PERIOD_IN_MSEC * BOUND_IN_TICK)
#define INVALID_INPUT_ID        ((unsigned int)-1)
#define DEFAULT_NUM_OF_CHANNELS (2)
#define DEFAULT_SAMPLE_RATE     (48000)
#define DEFAULT_DTO_RATE        (CLOCK_CENTER)

#define DIFF_BOUND              2
#define MAX_DIFF                20

/*------------------------------------------------------------------------------
    Macro Definitions
------------------------------------------------------------------------------*/
#define trace(fmt,args...)  logm_trace(aud_clock,fmt,##args)
#define debug(fmt,args...)  logm_debug(aud_clock,fmt,##args)
#define info(fmt,args...)   logm_info(aud_clock,fmt,##args)
#define noti(fmt,args...)   logm_noti(aud_clock,fmt,##args)
#define warn(fmt,args...)   logm_warning(aud_clock,fmt,##args)
#define error(fmt,args...)  logm_error(aud_clock,fmt,##args)

#define _INITIAL_OUTPUT_INFO(_name, _func) \
    { _name, false, false, NULL, 0, 0, \
      aud_hal_set_##_func##_sample_rate, aud_hal_set_##_func##_dto_rate, }

#define _GET_CLOCK_SOURCE_NAME(_src)    (       \
    (_src < CLOCK_SOURCE_MAX) ? clock_table[_src].name : "unknown")
#define _GET_CLOCK_SOURCE_TYPE(_src)    (       \
    (_src < CLOCK_SOURCE_MAX) ? clock_table[_src].type : CLOCK_SOURCE_TYPE_MAX)

/*------------------------------------------------------------------------------
    Type Definitions
------------------------------------------------------------------------------*/
struct _input_info
{
    struct list_head list;

    int uid;
    unsigned int index;
    enum aud_clock_source source;
    unsigned int channels;
    unsigned int sample_rate;
    bool is_first;
};

struct _count_info
{
    unsigned int irq;
    unsigned int clock;
};

struct _module_info
{
    struct _count_info start;
    struct _count_info last;
    struct _count_info curr;

    unsigned int frames_per_irq;
};

struct _clock_info
{
    int input_uid;
    CLOCK_SOURCE_TYPE source;
    unsigned int dto_rate;

    int total_diff_slope;

    struct _module_info reference;
    struct _module_info target;
};

/* Shipped LP64: channel +0x08, rate +0x0c, bypass +0x10, clock_info +0x14,
 * thread +0x60, wait +0x68, output +0x80; allocation size 0x88. */
struct _output_info
{
    int uid;
    unsigned int input;
    unsigned int channel;
    unsigned int sample_rate;
    bool bypass;
    struct _clock_info clock_info;

    struct task_struct *thread;
    wait_queue_head_t wait;

    struct _output *output;
};

/* Shipped LP64 stride 0x60: initialized +8, enable +9, info +0x10,
 * sample_rate +0x18, set_sample_rate +0x20, lock +0x38, change flags +0x58..5a.
 * output_list @0xffffffc0122cd3a0; ARC is element 3, not a separate eARC slot. */
struct _output
{
    const char *name;

    bool initialized;
    bool enable;
    struct _output_info *info;

    unsigned int sample_rate;
    unsigned int dto_rate;
    int (*set_sample_rate) (unsigned int);
    int (*set_dto_rate) (unsigned int);

    struct mutex lock;

    bool input_changed;
    bool output_changed;
    bool input_index_changed;
};

/*------------------------------------------------------------------------------
    global Variables
------------------------------------------------------------------------------*/
logm_define (aud_clock, log_level_noti);

/*------------------------------------------------------------------------------
    Static Variables
------------------------------------------------------------------------------*/
static const struct
{
    CLOCK_SOURCE_TYPE type;
    const char *name;
} clock_table[] =
{
    /* *INDENT-OFF* */
    [CLOCK_SOURCE_UNKNOWN] = { CLOCK_SOURCE_TYPE_UNKNOWN, "unknown" },
    [CLOCK_SOURCE_DTV0]    = { CLOCK_SOURCE_TYPE_DTV0,    "DTV0"    },
    [CLOCK_SOURCE_DTV1]    = { CLOCK_SOURCE_TYPE_DTV1,    "DTV1"    },
    [CLOCK_SOURCE_ATV]     = { CLOCK_SOURCE_TYPE_ATV,     "ATV"     },
    [CLOCK_SOURCE_HDMI]    = { CLOCK_SOURCE_TYPE_HDMI,    "HDMI"    },
    [CLOCK_SOURCE_ADC]     = { CLOCK_SOURCE_TYPE_ADC,     "ADC"     },
    [CLOCK_SOURCE_GSTC0]   = { CLOCK_SOURCE_TYPE_GSTC0,   "GSTC0"   },
    [CLOCK_SOURCE_GSTC1]   = { CLOCK_SOURCE_TYPE_GSTC1,   "GSTC1"   },
    /* *INDENT-ON* */
};

static DEFINE_MUTEX (list_lock);
static struct list_head input_list = LIST_HEAD_INIT (input_list);
static struct _output output_list[] = {
    /* *INDENT-OFF* */
    [CLOCK_OUTPUT_SPEAKER] = _INITIAL_OUTPUT_INFO ("speaker", speaker),
    [CLOCK_OUTPUT_HP]      = _INITIAL_OUTPUT_INFO ("hp",      speaker), // hp and speaker user same clock source
    [CLOCK_OUTPUT_OPTIC]   = _INITIAL_OUTPUT_INFO ("optic",   spdif),
    [CLOCK_OUTPUT_ARC]     = _INITIAL_OUTPUT_INFO ("arc",     arc),
    /* *INDENT-ON* */
};

static DEFINE_MUTEX (clock_thread_lock);

/*==============================================================================
    Implementation Group
==============================================================================*/
static struct _input_info *_find_input_info (unsigned int index)
{
    struct _input_info *input_info;

    if (index == INVALID_INPUT_ID)
        return NULL;

    mutex_lock (&list_lock);
    list_for_each_entry (input_info, &input_list, list)
    {
        if (input_info->index == index)
            return input_info;
    }
    mutex_unlock (&list_lock);

    return NULL;
}

static bool _get_input_info (unsigned int index, struct _input_info *input_info)
{
    struct _input_info *input_info_source = _find_input_info (index);

    if (input_info_source == NULL)
        return false;

    memcpy (input_info, input_info_source, sizeof (struct _input_info));
    mutex_unlock (&list_lock);

    return true;
}

static void _update_output_dto_rate (struct _output *output, unsigned int rate)
{
    output->dto_rate = rate;

    if (output->set_dto_rate)
        output->set_dto_rate (output->dto_rate);
}

static void _update_output_sample_rate (struct _output *output, unsigned int rate)
{
    output->sample_rate = rate;

    if (output->set_sample_rate)
        output->set_sample_rate (output->sample_rate);
}

/* Inlined in the tagged output APIs. Success RETURNS WITH output.lock HELD.
 * The shipped NULL-info error path also leaves that lock held; do not silently
 * repair it in a reconstruction. initialized/enable normally guarantee info. */
static struct _output_info *_get_output_info (enum aud_clock_output output)
{
    if (output < 0 || output >= CLOCK_OUTPUT_MAX)
        return NULL;

    if (output_list[output].initialized == false)
        return NULL;

    mutex_lock (&output_list[output].lock);
    if (output_list[output].enable == false)
    {
        mutex_unlock (&output_list[output].lock);
        return NULL;
    }

    return output_list[output].info;
}

/* No standalone symbol: inlined at aud_clock_update_output +0x7c..+0x94.
 * channel is unchanged (the compiler elides the same-value store); rate +0x0c,
 * bypass +0x10, DTO +0x1c = 71. This helper does NOT call the HAL, change input
 * index, or reset the reference/target counters. */
static void _update_output_info (struct _output_info *output_info, unsigned int channel,
    unsigned int sample_rate, bool bypass)
{
    output_info->channel = channel;
    output_info->sample_rate = sample_rate;
    output_info->bypass = bypass;
    output_info->clock_info.dto_rate = DEFAULT_DTO_RATE;
}

static void _reset_clock_info (struct _output_info *output_info)
{
    const char *name = output_info->output->name;
    struct _clock_info *clock_info = &output_info->clock_info;
    ClockCmdGetClockInfo module_clock_info;

    clock_info->dto_rate = DEFAULT_DTO_RATE;

    if (clock_info->input_uid)
    {
        aimc_send_command_return (NULL, CLOCK_CMD_GET_CLOCK_INFO, 0,
            clock_info->input_uid, (void *) &module_clock_info, sizeof (ClockCmdGetClockInfo));

        clock_info->reference.start.clock = module_clock_info.clock_count;
        clock_info->reference.start.irq = module_clock_info.irq_count;
    }
    else
    {
        clock_info->reference.start.clock = 0;
        clock_info->reference.start.irq = 0;
    }

    aimc_send_command_return (NULL, CLOCK_CMD_GET_CLOCK_INFO, 0,
        output_info->uid, (void *) &module_clock_info, sizeof (ClockCmdGetClockInfo));

    clock_info->target.start.clock = module_clock_info.clock_count;
    clock_info->target.start.irq = module_clock_info.irq_count;

    clock_info->reference.last.clock = clock_info->reference.start.clock;
    clock_info->reference.last.irq = clock_info->reference.start.irq;
    clock_info->target.last.clock = clock_info->target.start.clock;
    clock_info->target.last.irq = clock_info->target.start.irq;

    clock_info->total_diff_slope = 0;

    info ("[%s]reset clock info %u(%u) %u(%u)\n", name,
        clock_info->reference.last.clock, clock_info->reference.last.irq,
        clock_info->target.last.clock, clock_info->target.last.irq);
}

static bool _update_status (struct _output_info *output_info)
{
    const char *name = output_info->output->name;
    struct _clock_info *clock_info = &output_info->clock_info;
    bool changed = false;
    int input_uid = 0;
    CLOCK_SOURCE_TYPE clock_source_type = _GET_CLOCK_SOURCE_TYPE (CLOCK_SOURCE_GSTC0);
    const char *clock_name = _GET_CLOCK_SOURCE_NAME (CLOCK_SOURCE_GSTC0);
    struct _input_info input_info = { 0, };

    if (_get_input_info (output_info->input, &input_info))
    {
        clock_source_type = _GET_CLOCK_SOURCE_TYPE (input_info.source);
        clock_name = _GET_CLOCK_SOURCE_NAME (input_info.source);

        if ((clock_source_type == CLOCK_SOURCE_TYPE_ATV)
            || (clock_source_type == CLOCK_SOURCE_TYPE_HDMI))
        {
            input_uid = input_info.uid;
        }
    }

    if (clock_info->source != clock_source_type)
    {
        ClockCmdSetClockSource clock_source = {.clock_source_type = clock_source_type };

        aimc_send_command (NULL, CLOCK_CMD_SET_CLOCK_SOURCE, 0,
            output_info->uid, (void *) &clock_source, sizeof (ClockCmdSetClockSource));

        clock_info->source = clock_source_type;
        changed = true;
        info ("[%s]change clock source %s\n", name, clock_name);
    }

    if (clock_info->input_uid != input_uid)
    {
        clock_info->input_uid = input_uid;
        changed = true;
        info ("[%s]change input module 0x%08X\n", name, input_uid);
    }

    if (output_info->output->input_changed)
    {
        output_info->output->input_changed = false;
        changed = true;
        info ("[%s] input changed.\n", name);
    }

    if (output_info->output->output_changed)
    {
        output_info->output->output_changed = false;
        changed = true;
        info ("[%s] output changed.\n", name);
    }

    if (output_info->output->input_index_changed)
    {
        output_info->output->input_index_changed = false;
        changed = true;
        info ("[%s] input index changed.\n", name);
    }

    if (changed == false)
    {
        ClockCmdGetClockInfo module_clock_info;

        if (clock_info->input_uid != 0)
        {
            aimc_send_command_return (NULL, CLOCK_CMD_GET_CLOCK_INFO, 0,
                clock_info->input_uid, (void *) &module_clock_info, sizeof (ClockCmdGetClockInfo));

            clock_info->reference.last.clock = clock_info->reference.curr.clock;
            clock_info->reference.last.irq = clock_info->reference.curr.irq;
            clock_info->reference.curr.clock = module_clock_info.clock_count;
            clock_info->reference.curr.irq = module_clock_info.irq_count;
            clock_info->reference.frames_per_irq = module_clock_info.frames_per_irq;
        }
        else
        {
            clock_info->reference.curr.clock = 0;
            clock_info->reference.curr.irq = 0;
            clock_info->reference.frames_per_irq = 0;
        }

        aimc_send_command_return (NULL, CLOCK_CMD_GET_CLOCK_INFO, 0,
            output_info->uid, (void *) &module_clock_info, sizeof (ClockCmdGetClockInfo));

        clock_info->target.last.clock = clock_info->target.curr.clock;
        clock_info->target.last.irq = clock_info->target.curr.irq;
        clock_info->target.curr.clock = module_clock_info.clock_count;
        clock_info->target.curr.irq = module_clock_info.irq_count;
        clock_info->target.frames_per_irq = module_clock_info.frames_per_irq;

        info ("[%s] reference[%s] : uid:%x freq:%u clock:%u irq:%u frames/irq:%u\n", name,
            clock_name, clock_info->input_uid, input_info.sample_rate,
            clock_info->reference.curr.clock, clock_info->reference.curr.irq,
            clock_info->reference.frames_per_irq);
        info ("[%s] target : uid:%x freq:%u clock:%u irq:%u frames/irq:%u\n", name,
            output_info->uid, output_info->sample_rate,
            clock_info->target.curr.clock, clock_info->target.curr.irq,
            clock_info->target.frames_per_irq);

        if ((clock_info->reference.curr.clock < clock_info->reference.last.clock) ||
            (clock_info->reference.curr.irq < clock_info->reference.last.irq) ||
            (clock_info->target.curr.clock < clock_info->target.last.clock) ||
            (clock_info->target.curr.irq < clock_info->target.last.irq))
        {
            changed = true;
            info ("[%s]wrap-around count ref_clk:%u(%u) ref_irq:%u(%u), tar_clk:%u(%u) tar_irq:%u(%u)\n", name, clock_info->reference.curr.clock, clock_info->reference.last.clock, clock_info->reference.curr.irq, clock_info->reference.last.irq, clock_info->target.curr.clock, clock_info->target.last.clock, clock_info->target.curr.irq, clock_info->target.last.irq);
        }
    }

    return changed;
}

static bool _update_dto_rate (struct _output_info *output_info)
{
    const char *name = output_info->output->name;
    struct _clock_info *clock_info = &output_info->clock_info;
    unsigned int ref_curr_clocks, ref_curr_frames, ref_curr_frame_rate;
    unsigned int tar_curr_clocks, tar_curr_frames, tar_curr_frame_rate;
    int ref_curr_slope, tar_curr_slope;
    struct _input_info input_info = {.channels = 2,.sample_rate = DEFAULT_SAMPLE_RATE, };
    int curr_diff_slope;

    _get_input_info (output_info->input, &input_info);

    if (clock_info->input_uid != 0)
    {
        ref_curr_frames =
            (clock_info->reference.curr.irq -
            clock_info->reference.last.irq) * clock_info->reference.frames_per_irq;
        ref_curr_clocks = clock_info->reference.curr.clock - clock_info->reference.last.clock;
    }
    else
    {
        ref_curr_frames = input_info.sample_rate * RUN_PERIOD_IN_SEC;
        ref_curr_clocks = TICKS_PER_SEC * RUN_PERIOD_IN_SEC;
    }

    debug ("[%s] ref : irq(%d/%d) clock(%d/%d)\n", name,
        clock_info->reference.curr.irq, clock_info->reference.last.irq,
        clock_info->reference.curr.clock, clock_info->reference.last.clock);

    tar_curr_frames =
        (clock_info->target.curr.irq -
        clock_info->target.last.irq) * clock_info->target.frames_per_irq;
    tar_curr_clocks = clock_info->target.curr.clock - clock_info->target.last.clock;

    debug ("[%s] tar : irq(%d/%d) clock(%d/%d)\n", name,
        clock_info->target.curr.irq, clock_info->target.last.irq, clock_info->target.curr.clock,
        clock_info->target.last.clock);

    if ((ref_curr_clocks == 0) || (tar_curr_clocks == 0))
        return false;

    // 5 초 동안 frame count
    ref_curr_frame_rate =
        (unsigned long long) ref_curr_frames *TICKS_PER_SEC * RUN_PERIOD_IN_SEC / ref_curr_clocks;
    tar_curr_frame_rate =
        (unsigned long long) tar_curr_frames *TICKS_PER_SEC * RUN_PERIOD_IN_SEC / tar_curr_clocks;

    // 48KHz 기준으로 1 초 동안 frame count
    ref_curr_slope =
        (unsigned long long) ref_curr_frame_rate *DEFAULT_SAMPLE_RATE * 10 /
        (input_info.sample_rate * RUN_PERIOD_IN_SEC);
    tar_curr_slope =
        (unsigned long long) tar_curr_frame_rate *DEFAULT_SAMPLE_RATE * 10 /
        (output_info->sample_rate * RUN_PERIOD_IN_SEC);

    curr_diff_slope = tar_curr_slope - ref_curr_slope;

    if (curr_diff_slope > 1000 || curr_diff_slope < -1000)
        return false;

    clock_info->total_diff_slope += curr_diff_slope;

    debug ("[%s] ref : frame_rate(%d/%d) clock(%d)\n", name, ref_curr_frames, ref_curr_frame_rate,
        ref_curr_clocks);
    debug ("[%s] tar : frame_rate(%d/%d) clock(%d)\n", name, tar_curr_frames, tar_curr_frame_rate,
        tar_curr_clocks);

    info ("[%s] rate %d : diff %d slope (%d/%d)\n", name, clock_info->dto_rate,
        clock_info->total_diff_slope, ref_curr_slope, tar_curr_slope);

    if (tar_curr_slope > ref_curr_slope)
    {
        if (clock_info->total_diff_slope > 0)
        {
            if (clock_info->dto_rate <= CLOCK_MIN)
                noti ("[%s] not change rate min %d\n", name, clock_info->dto_rate);
            else
            {
                clock_info->dto_rate -= 1;      //< diff > 0 -> tar_frame_rate > ref_frame_rate -> run slower
                noti ("[%s] change rate to %d\n", name, clock_info->dto_rate);
            }
        }
    }
    else if (tar_curr_slope < ref_curr_slope)
    {
        if (clock_info->total_diff_slope < 0)
        {
            if (clock_info->dto_rate >= CLOCK_MAX)
                noti ("[%s] not change rate max %d\n", name, clock_info->dto_rate);
            else
            {
                clock_info->dto_rate += 1;      //< diff < 0 -> tar_frame_rate < ref_frame_rate -> run faster
                noti ("[%s] change rate to %d\n", name, clock_info->dto_rate);
            }
        }
    }

    return true;
}

static int _thread_func (void *arg)
{
    struct _output_info *output_info = (struct _output_info *) arg;
    const char *name = output_info->output->name;
    bool reset_start = true;

    debug ("[%s]start thread\n", name);

    while (!kthread_should_stop ())
    {
        unsigned int dto_rate;

        if (reset_start)
        {
            _reset_clock_info (output_info);

            _update_output_dto_rate (output_info->output, DEFAULT_DTO_RATE);

            reset_start = false;
            continue;
        }

        //msleep_interruptible (RUN_PERIOD_IN_MSEC);
        wait_event_interruptible_timeout (output_info->wait, kthread_should_stop (),
            msecs_to_jiffies (RUN_PERIOD_IN_MSEC));
        if (kthread_should_stop ())
        {
            break;
        }

        mutex_lock (&clock_thread_lock);
        mutex_lock (&output_info->output->lock);

        if ((output_info->uid == 0) || (output_info->sample_rate == 0)
            || (output_info->output->enable == false))
        {
            mutex_unlock (&output_info->output->lock);
            mutex_unlock (&clock_thread_lock);
            warn ("no output info %d %d\n", output_info->uid, output_info->sample_rate);
            continue;
        }

        if (_update_status (output_info))
        {
            reset_start = true;
            mutex_unlock (&output_info->output->lock);
            mutex_unlock (&clock_thread_lock);
            continue;
        }

        if (_update_dto_rate (output_info) == false)
        {
            reset_start = true;
            mutex_unlock (&output_info->output->lock);
            mutex_unlock (&clock_thread_lock);
            continue;
        }

        dto_rate = output_info->clock_info.dto_rate;
        mutex_unlock (&output_info->output->lock);
        mutex_unlock (&clock_thread_lock);

        _update_output_dto_rate (output_info->output, dto_rate);
    }

    debug ("[%s]stop thread\n", name);

    return 0;
}

int aud_clock_enable_input (unsigned int index, enum aud_clock_source source, int uid)
{
    struct _input_info *input_info;

    input_info = (struct _input_info *) kmalloc (sizeof (struct _input_info), GFP_KERNEL);
    if (input_info == NULL)
    {
        error ("no mem\n");
        return -1;
    }

    input_info->uid = uid;
    input_info->index = index;
    input_info->source = source;

    mutex_lock (&list_lock);
    list_add (&input_info->list, &input_list);
    mutex_unlock (&list_lock);

    info ("enable input uid:0x%x, index:%d, source:%d\n", input_info->uid,
        input_info->index, input_info->source);
    return 0;
}

int aud_clock_disable_input (unsigned int index)
{
    struct _input_info *input_info = _find_input_info (index);

    if (input_info == NULL)
    {
        info ("unknown input %d\n", index);
        return -1;
    }

    list_del (&input_info->list);
    mutex_unlock (&list_lock);

    kfree (input_info);

    info ("disable input index:%d\n", index);
    return 0;
}

int aud_clock_update_input (unsigned int index, unsigned int channel, unsigned int sample_rate)
{
    struct _input_info *input_info = _find_input_info (index);
    int i;

    if (input_info == NULL)
    {
        error ("unknown input %d\n", index);
        return -1;
    }

    input_info->channels = channel;
    input_info->sample_rate = sample_rate;

    noti ("update input index:%d, channels:%d, sample_rate:%d\n", input_info->index,
        input_info->channels, input_info->sample_rate);

    mutex_unlock (&list_lock);

    for (i = 0; i < CLOCK_OUTPUT_MAX; i++)
    {
        struct _output *output = &output_list[i];

        if (output->initialized == false)
        {
            continue;
        }

        mutex_lock (&output->lock);
        if (!output->enable || (output->info == NULL))
        {
            mutex_unlock (&output->lock);
            continue;
        }

        if (input_info->index != output->info->input)
        {
            mutex_unlock (&output->lock);
            continue;
        }

        output->input_changed = true;

        mutex_unlock (&output->lock);
    }

    return 0;
}

/* @0xffffffc010cb6754 */
int aud_clock_enable_output (enum aud_clock_output output, int uid)
{
    struct _output_info *output_info = NULL;
    ClockCmdSetClockSource clock_source = { 0, };

    if (output < 0 || output >= CLOCK_OUTPUT_MAX)
    {
        return -1;
    }

    mutex_lock (&list_lock);
    if (output_list[output].initialized == false)
    {
        mutex_init (&output_list[output].lock);
        output_list[output].initialized = true;
    }
    mutex_unlock (&list_lock);

    mutex_lock (&output_list[output].lock);
    if (output_list[output].enable)
    {
        mutex_unlock (&output_list[output].lock);
        return -1;
    }

    if (output_list[output].info == NULL)
    {
        output_info = kcalloc (1, sizeof (struct _output_info), GFP_KERNEL);
        if (output_info == NULL)
        {
            output_list[output].enable = false;
            mutex_unlock (&output_list[output].lock);
            return -1;
        }

        output_info->input = INVALID_INPUT_ID;
        output_info->channel = DEFAULT_NUM_OF_CHANNELS;
        output_info->sample_rate = DEFAULT_SAMPLE_RATE;
        output_info->output = &output_list[output];
        output_info->clock_info.source = CLOCK_SOURCE_TYPE_UNKNOWN;

        output_list[output].info = output_info;
    }
    else
    {
        output_info = output_list[output].info;
        clock_source.clock_source_type = output_info->clock_info.source;
        aimc_send_command (NULL, CLOCK_CMD_SET_CLOCK_SOURCE, 0,
            uid, (void *) &clock_source, sizeof (ClockCmdSetClockSource));
    }

    output_info->uid = uid;
    output_info->clock_info.dto_rate = DEFAULT_DTO_RATE;

    if (output != CLOCK_OUTPUT_SPEAKER)
    {
        init_waitqueue_head (&output_info->wait);       // _thread_func 실행 전 초기화.

        output_info->thread =
            (struct task_struct *) kthread_run (_thread_func, output_info,
            output_list[output].name);

        if (IS_ERR (output_info->thread))
        {
            output_list[output].enable = false;
            output_info->thread = NULL;
            mutex_unlock (&output_list[output].lock);

            return -1;
        }
    }

    output_list[output].enable = true;
    mutex_unlock (&output_list[output].lock);

    info ("[%s]enable output %d\n", output_list[output].name, output);

    return 0;
}

/* @0xffffffc010cb69b0 */
int aud_clock_disable_output (enum aud_clock_output output)
{
    struct _output_info *output_info = _get_output_info (output);
    struct task_struct *thread_to_stop = NULL;

    if (output_info == NULL)
    {
        error ("unknown output %d\n", output);
        return -1;
    }

    if (output_info->thread)
    {
        wake_up_interruptible (&output_info->wait);
        thread_to_stop = output_info->thread;
        output_info->thread = NULL;
    }

    output_list[output].enable = false;
    mutex_unlock (&output_list[output].lock);

    if (thread_to_stop)
        kthread_stop (thread_to_stop);

    info ("[%s]disable output\n", output_list[output].name);

    return 0;
}

// input index가 변경이 되면
// bypass 인 경우 output sample_rate를input sample_rate로 변경
/* @0xffffffc010cb6b34 */
int aud_clock_set_input_index (enum aud_clock_output output, unsigned int index)
{
    struct _output_info *output_info = _get_output_info (output);

    if (output_info == NULL)
    {
        error ("unknown output %d\n", output);
        return -1;
    }

    output_info->input = index;
    output_info->output->input_index_changed = true;

    mutex_unlock (&output_list[output].lock);

    info ("[%s]set input %d\n", output_list[output].name, index);

    return 0;
}

/* On success: cache rate/bypass, reset requested DTO to CLOCK_CENTER (71),
 * mark output_changed, UNLOCK, then call output->set_sample_rate(rate).
 * ARC's function pointer is aud_hal_set_arc_sample_rate @0xffffffc010cf1614.
 * bypass is only stored here; it is NOT a guard on the immediate HAL call.
 * The worker is not woken here; its normal polling observes output_changed. */
/* @0xffffffc010cb6ca0 */
int aud_clock_update_output (enum aud_clock_output output, unsigned int sample_rate, bool bypass)
{
    struct _output_info *output_info = _get_output_info (output);

    if (output_info == NULL)
    {
        error ("unknown output %d\n", output);
        return -1;
    }

    _update_output_info (output_info, output_info->channel, sample_rate, bypass);
    output_info->output->output_changed = true;
    mutex_unlock (&output_list[output].lock);

    _update_output_sample_rate (output_info->output, sample_rate);

    noti ("[%s]update %d %d\n", output_list[output].name, sample_rate, bypass);

    return 0;
}

/* @0xffffffc010cb6e30 */
unsigned int aud_clock_get_input_sample_rate (unsigned int input)
{
    struct _input_info input_info = { 0, };
    unsigned int sample_rate = 48000;

    if (_get_input_info (input, &input_info))
        sample_rate = input_info.sample_rate;

    return sample_rate;
}
