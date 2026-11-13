// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2017 by SIC Lab. LG Electronics Inc.
 *
 * This program or software including the accompanying associated documentation
 * (“Software”) is the proprietary software of LG Electronics Inc. and or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate written license agreement between you
 * and LG Electronics Inc. (“Authorized License”). Except as set forth in an
 * Authorized License, LG Electronics Inc. grants no license (express or implied),
 * rights to use, or waiver of any kind with respect to the Software, and LG
 * Electronics Inc. expressly reserves all rights in and to the Software and all
 * intellectual property therein. If you have no Authorized License, then you have
 * no rights to use the Software in any ways, and should immediately notify LG
 * Electronics Inc. and discontinue all use of the Software.
 *
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 2.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 2.0 requirements will be
 * met: https://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 */



/*
 * Named-event registration and callback branch, kernel 5.4.268-329.ptl4tv.5.
 * Image SHA256: 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Source-assisted from LG common/imc/adec_imc.c under its GPL-2.0 option.
 * Evidence: build/scratch/seek-chain/{IMC_ProcessHeader,IMC_RegisterNamedEvent,
 * add_named_event}.asm. No standalone process_named_event_handle symbol:
 * it is inlined in IMC_ProcessHeader @0xffffffc010d0b1e0 +0x664 onward.
 * This excerpt does NOT claim reconstruction of other IMC_ProcessHeader arms.
 */
#include "imc/adec_imc.h"
#include "imc/aimc_internal.h"
#include "ipc/adec_ipc.h"
#include "common/adec_common.h"
#include "common/log.h"
#include "module/uid.h"

logm_define (com_imc, log_level_noti);
#define info(fmt,args...) logm_info(com_imc,fmt,##args)
#define error(fmt,args...) logm_error(com_imc,fmt,##args)
enum imc_action { IMC_ACTION_NEVENT_ADD = 6 };

struct named_event
{
    /* linked at struct _ImcInfo::event_list */
    struct link list;

    /* informations from event handler */
    unsigned int event_id;
    unsigned int processor_from;
    int once;

    /* target that the event registered on */
    unsigned int target_on;
    char event_name[32];

    int (*callback_func) (void *arg, void *param, unsigned int param_size);
    void *callback_arg;
};

/* @0xffffffc010d099b0 */
static struct named_event *add_named_event (ImcInfo * imc, int event_id, int once,
    const char *name, unsigned int target_on, unsigned int processor_from,
    int (*callback_func) (void *arg, void *param, unsigned int param_size), void *callback_arg)
{
    struct named_event *e;

    info ("add \"%s\" on %x. id %x from %d, %sonce\n", name, target_on,
        event_id, processor_from, once ? "" : "not ");

    e = ADEC_CALLOC (NULL, sizeof (*e), void);
    if (!e)
    {
        error ("no mem\n");
        return NULL;
    }

    e->event_id = event_id;
    e->processor_from = processor_from;
    e->target_on = target_on;
    strncpy (e->event_name, name, sizeof (e->event_name));
    e->event_name[sizeof (e->event_name) - 1] = 0;
    e->callback_func = callback_func;
    e->callback_arg = callback_arg;
    e->once = once;

    link_add (&imc->event_list, &e->list);

    return e;
}

/* @0xffffffc010d09b30 */
int IMC_RegisterNamedEvent (ImcInfo * imc, const char *name, unsigned int target, int once,
    int (*callback_func) (void *arg, void *param, unsigned int param_size), void *callback_arg)
{
    struct named_event *e;
    int event_id;

    aud_mutex_lock (&imc->lock);

    event_id = imc->last_event_id + 1;
    if (event_id < 0)
        event_id = 1;
    imc->last_event_id = event_id;

    e = add_named_event (imc, event_id, once, name, target, 0, callback_func, callback_arg);
    if (!e)
    {
        aud_mutex_unlock (&imc->lock);
        return -1;
    }

    if (AUD_MODID_PROC (target) != imc->core)
    {
        IpcHeader ipc_header = { };

        ipc_header.senderInfo.sender = imc->core;
        ipc_header.targetInfo.action = IMC_ACTION_NEVENT_ADD;
        ipc_header.targetInfo.target = target;
        if (once)
            ipc_header.targetInfo.optionFlag |= 1;
        ipc_header.extField = event_id;
        ipc_header.paramLength = sizeof (e->event_name);

        /* not in my processor. send IPC to the processor */
        aimc_ipc_send (imc, AUD_MODID_PROC (target), &ipc_header, e->event_name);
    }

    aud_mutex_unlock (&imc->lock);
    return event_id;
}

/* IMC_ACTION_NEVENT_HANDLE (action 8) in IMC_ProcessHeader matches sender
 * and event_id, then invokes selected->callback_func (+0x40) with callback_arg
 * (+0x48), packet+0x20 and packet length (+0x1c). For output_status that
 * function is kaimc.c::cb_func_local, NOT output.c::_cb_output_status.
 * The call is at 0xffffffc010d0b8fc; once=false preserves the registration. */
static int process_named_event_handle (ImcInfo * imc, IpcHeader * ipc_header)
{
    struct named_event *e, *selected;
    unsigned int event_source = ipc_header->senderInfo.sender;
    int event_id = ipc_header->extField;

    aud_mutex_lock (&imc->lock);
    selected = NULL;
    for_each_link_entry (e, &imc->event_list, list)
    {
        if (e->target_on == event_source && e->event_id == event_id)
        {
            selected = e;
            break;
        }
    }

    if (!selected)
    {
        error ("unknown event id %x from %x\n", event_id, event_source);
        aud_mutex_unlock (&imc->lock);
        return 0;
    }

    info ("call event callback function %p, event \"%s\" on %x\n",
        selected->callback_func, selected->event_name, event_source);
    selected->callback_func (selected->callback_arg,
        (void *) ipc_header->param, (unsigned int) ipc_header->paramLength);

    if (selected->once)
    {
        link_del (&selected->list);
        ADEC_FREE (NULL, selected);
    }
    aud_mutex_unlock (&imc->lock);

    return 0;
}
