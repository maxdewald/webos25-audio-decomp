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
 * Source-assisted reconstruction of Image-5.4.268-329.decompressed, kernel
 * 5.4.268-329.ptl4tv.5, SHA256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Offline instruction evidence: build/scratch/seek-chain/image-evidence.json.
 * Address-tagged bodies were checked against the shipped AArch64 instructions.
 * Kernel allocation, locking, logging and workqueue primitives remain external.
 */
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include "kaimc.h"
#include "imc/adec_imc.h"
#include "imc/aimc_internal.h"
#include "module/adec_module_list.h"
#include "log.h"

/* Focused excerpt of kaimc.c; other transport code is not reconstructed here.
 * These globals/locks belong to that same original translation unit. */
extern struct mutex _gaimc_mutex, _gaimc_wq_mutex;
#define AUDIO_IMC_LOCK() mutex_lock(&_gaimc_mutex)
#define AUDIO_IMC_UNLOCK() mutex_unlock(&_gaimc_mutex)
#define AUDIO_IMC_WQ_LOCK() mutex_lock(&_gaimc_wq_mutex)
#define AUDIO_IMC_WQ_UNLOCK() mutex_unlock(&_gaimc_wq_mutex)
logm_define (aud_kaimc, log_level_noti);
#define info(fmt,args...) logm_info(aud_kaimc,fmt,##args)
#define error(fmt,args...) logm_error(aud_kaimc,fmt,##args)

struct aimc
{
    struct _ImcInfo imc_info;
    struct list_head noti_list;

    bool dead[NUM_OF_DSP];
    int evt_id[NUM_OF_DSP];
};

struct noti_info
{
    struct list_head list;

    int (*callback_func) (void *, int, void *);
    void *callback_param;
    struct work_struct ws;

    unsigned int target;
    unsigned int event;
    bool repeat;
    unsigned int action_id;

    struct mutex param_lock;
    struct list_head param_list;
};

struct noti_param
{
    struct list_head list;

    int size;
    unsigned char param[0];
};

extern struct aimc *gaimc;
/* Initialized by the omitted aimc_init, not by this excerpt. */
static struct aud_mod_owner *gowner;
extern int aud_inst_connect (struct aud_mod_owner *, unsigned int, int, unsigned int, int, bool);

/* No event/clock operation here: module_list.c handles same-DSP connector
 * commands or cross-DSP glue creation and connection. */
/* @0xffffffc010c8efd4 */
int aimc_module_connect (unsigned int uid1, int port1, unsigned int uid2, int port2)
{
    return aud_inst_connect (gowner, uid1, port1, uid2, port2, false);
}

/* The actual indirect kernel caller of output.c::_cb_output_status:
 * noti+0x10 = callback, +0x18 = callback argument, +0x20 = work;
 * queued param+0x10 = length, +0x14 = copied bytes. Work drains FIFO order:
 * dequeue while locked, unlock at 0xffffffc010c8cce8, load the callback/arg
 * pair at ...cd34 and call at ...cd40, then free the payload at ...cd48.
 * The worker acquires neither global IMC lock nor imc_info.lock; param_lock
 * is not held across the callback and does not protect callback replacement.
 * repeat=true leaves noti ownership with unregister; repeat=false self-frees
 * at ...cdb0, having been detached by cb_func_local before queueing. */
/* @0xffffffc010c8cc50 */
static void noti_handler_work (struct work_struct *work)
{
    struct noti_info *noti = container_of (work, struct noti_info, ws);
    struct noti_param *noti_param;

    mutex_lock (&noti->param_lock);
    while ((noti_param = list_first_entry_or_null (&noti->param_list, struct noti_param, list)))
    {
        list_del (&noti_param->list);
        mutex_unlock (&noti->param_lock);

        info ("call: action %x(event %x, module %x). %ps(%p, %d)\n",
            noti->action_id, noti->event, noti->target,
            noti->callback_func, noti_param->param, noti_param->size);

        noti->callback_func (noti_param->param, noti_param->size, noti->callback_param);
        kfree (noti_param);

        mutex_lock (&noti->param_lock);
    }
    mutex_unlock (&noti->param_lock);

    if (noti->repeat == false)
    {
        info ("action once. free\n");
        kfree (noti);
    }
}

/* IMC callback: copy payload, queue under param_lock, queue_work(system_wq).
 * Never invokes _cb_output_status inline. No deduplication/coalescing of
 * payloads: coalesced work still drains all queued notifications.
 * param_lock is released at 0xffffffc010c8d11c, before queue_work_on at
 * ...d180; a one-shot noti is first unlinked at ...d150..d16c. The enclosing
 * local dispatch holds the WQ lock and imc_info.lock, not the eventual worker.
 * Allocation failure returns -1 without queueing; the named-event dispatch
 * ignores that return. The zero-length case still allocates/queues a record. */
/* @0xffffffc010c8d060 */
static int cb_func_local (void *cbParam, void *param, unsigned int param_size)
{
    struct noti_info *noti = (struct noti_info *) cbParam;
    struct noti_param *noti_param;

    if (!noti)
    {
        return ADEC_ERR_NONE;
    }

    info ("noti: action %x(event %x, module %x). %ps\n",
        noti->action_id, noti->event, noti->target, noti->callback_func);

    noti_param = kzalloc (sizeof (struct noti_param) + param_size, GFP_KERNEL);
    if (!noti_param)
    {
        error ("no memory %ps. param_size %d\n", noti->callback_func, param_size);
        return -1;
    }

    if (param_size > 0)
    {
        memcpy (noti_param->param, param, param_size);
    }
    noti_param->size = param_size;

    mutex_lock (&noti->param_lock);
    list_add_tail (&noti_param->list, &noti->param_list);
    mutex_unlock (&noti->param_lock);

    if (noti->repeat == false)
    {
        info ("action once. remove from list\n");
        list_del (&noti->list);
    }
    queue_work (system_wq, &noti->ws);

    return ADEC_ERR_NONE;
}

// <= 0 : error
//  > 0 : success, event_id
/* @0xffffffc010c8d240: adds an independent noti/listener and lower named event.
 * This exported API is not an atomic replacement of an existing callback. */
int aimc_register_named_event (void *cb_param, int (*cb_func) (void *, int, void *),
    unsigned int target, const char *event_name, bool repeat)
{
    int event_id = 0;
    struct noti_info *noti;

    /* allocate event noti info structure */
    noti = kzalloc (sizeof (struct noti_info), GFP_KERNEL);
    if (!noti)
    {
        error ("no memory.\n");
        return 0;
    }

    INIT_LIST_HEAD (&noti->list);
    noti->callback_func = cb_func;
    noti->callback_param = cb_param;
    noti->target = target;
    noti->repeat = repeat;
    INIT_LIST_HEAD (&noti->param_list);
    INIT_WORK (&noti->ws, noti_handler_work);
    mutex_init (&noti->param_lock);

    /* register */
    AUDIO_IMC_WQ_LOCK ();
    AUDIO_IMC_LOCK ();

    event_id =
        IMC_RegisterNamedEvent (&gaimc->imc_info, event_name, target, repeat ? 0 : 1, cb_func_local,
        noti);

    noti->event = event_id;
    if (event_id < ADEC_ERR_NONE)
    {
        error ("IMC_RegisterNamedEvent : [%d]\n", event_id);
        kfree (noti);
    }
    else
    {
        list_add (&noti->list, &gaimc->noti_list);

        info ("register event ID %x(event %s, module %x)\n", noti->event, event_name, noti->target);
    }

    AUDIO_IMC_UNLOCK ();
    AUDIO_IMC_WQ_UNLOCK ();

    return event_id;
}

EXPORT_SYMBOL (aimc_register_named_event);

/* @0xffffffc010c8c660: verified Image. Locks ctx+0x10 (WQ) then +0x30,
 * IMC_UnregisterNamedEvent, first noti->event (+0x44) match on noti_list
 * (gaimc+0x78), list_del with poison, unlock both, then flush_work(+0x20),
 * drain param_list (+0x70) under param_lock (+0x50), kfree. Not found: only
 * unlocks. process_dspirq_workfunc/aimc_process_header hold the +0x10 lock
 * around IMC_ProcessHeader, so no new cb_func_local can queue onto a noti
 * once both removals are done; the work is drained after the locks drop.
 *
 * Locking and teardown: aimc_unregister_named_event unlinks the matching noti
 * under AUDIO_IMC_WQ_LOCK and AUDIO_IMC_LOCK, drops both locks, then flushes
 * pending work (+0x20) and drains param_list before freeing the noti struct.
 * Competing callers cannot find an unlinked noti on noti_list.
 * Exact boundary: unlink at 0xffffffc010c8c6ec..c70c, global unlocks at
 * ...c710/...c718, flush at ...c720, final noti free at ...c770. */
void aimc_unregister_named_event (int event_id)
{
    struct noti_info *noti = NULL;
    bool found = false;

    AUDIO_IMC_WQ_LOCK ();
    AUDIO_IMC_LOCK ();

    IMC_UnregisterNamedEvent (&gaimc->imc_info, event_id);

    list_for_each_entry (noti, &gaimc->noti_list, list)
    {
        if (noti->event == event_id)
        {
            list_del (&noti->list);
            found = true;
            break;
        }
    }

    AUDIO_IMC_UNLOCK ();
    AUDIO_IMC_WQ_UNLOCK ();

    if (found)
    {
        struct noti_param *noti_param;

        flush_work (&noti->ws);

        mutex_lock (&noti->param_lock);
        while ((noti_param = list_first_entry_or_null (&noti->param_list, struct noti_param, list)))
        {
            list_del (&noti_param->list);
            kfree (noti_param);
        }
        mutex_unlock (&noti->param_lock);

        kfree (noti);
    }
}

EXPORT_SYMBOL (aimc_unregister_named_event);
