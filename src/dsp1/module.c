// SPDX-License-Identifier: GPL-2.0-only
/*
 * DSP1 module.c FIRST delivery, source-assisted from LG's GPL-2.0 common
 * module/module.c. The source license is preserved below.
 * Firmware audio_a0_dsp1.bin SHA256
 * 8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4.
 * Evidence: build/scratch/seek-chain/dsp1-{connect,chain,event,forward}.asm.
 * Recovered branch slots were checked with tools/xtobjdump.sh. Profiling
 * arithmetic is retained from published source; not instruction-matched.
 * Diagnostic logging is abbreviated; recovery covers functional control flow.
 * Uses original public framework types rather than invented packed structs.
 * The FIRST producers upstream of a particular seek remain runtime-dependent.
 */
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


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "common/link.h"
#include "buffer/adec_buf.h"
#include "buffer/adec_buf_helper.h"
#include "buffer/data.h"
#include "module.h"
#include "module/uid.h"
#include "imc/adec_imc.h"
#include "imc/adec_imc_cmd.h"
#include "imc/aimc.h"
#include "starter.h"
#include "util/utils.h"

#include "common/log.h"

logm_define (commod, log_level_noti);
#define info(fmt,args...) logm_info(commod,fmt,##args)
#define debug(fmt,args...) logm_debug(commod,fmt,##args)
#define warn(fmt,args...) logm_warning(commod,fmt,##args)
#define error(fmt,args...) logm_error(commod,fmt,##args)

static int foreach_sync (void *arg, void *data)
{
    struct aud_port *port = data;
    aud_mutex_lock (&port->stream_lock);
    aud_link_node_unlock (&port->module_node);
    aud_port_sync_locked (port);
    aud_mutex_unlock (&port->stream_lock);
    return 0;
}

static int aud_module_event_locked (struct aud_module *, struct aud_port *, struct aud_event *);

/* @0x600fc780 */
static void del_input_port (struct aud_port *port)
{
    aud_mutex_lock (&port->stream_lock);
    aud_mutex_lock (&port->async_down_lock);
    if (port->current_first)
        aud_event_put (port->current_first);
    ADEC_FREE (NULL, port);
}

/* Disconnect drops/frees the INPUT and its cached FIRST. It deliberately keeps
 * the OUTPUT port and its FIRST for replay into the replacement input. This is
 * the link between _set_output's reroute and the next data-triggered FIRST. */
/* @0x600fc9b8 */
void aud_module_disconnect (struct aud_port *out_port, struct aud_port *in_port)
{
    struct aud_module *out_module = out_port->module;
    struct aud_module *in_module = in_port->module;
    int peer_count = aud_link_node_count (&out_port->down_peer_root);

    if (peer_count == 1 && out_module->impl->deleting_port)
        out_module->impl->deleting_port (out_module, out_port);
    aud_module_unblock (in_module);
    aud_module_unblock (out_module);
    aud_module_event (in_module, in_port, aud_event_new_disconnect ());
    aud_link_del (&in_port->down_peer_node);
    aud_link_node_lock (&in_port->down_peer_node);
    aud_mutex_lock (&in_port->stream_lock);
    aud_mutex_lock (&in_port->async_down_lock);
    aud_link_node_unlock (&in_port->down_peer_node);
    aud_mutex_lock (&in_port->async_up_lock);
    in_port->up_peer = NULL;
    aud_mutex_unlock (&in_port->async_up_lock);
    aud_mutex_unlock (&in_port->async_down_lock);
    aud_mutex_unlock (&in_port->stream_lock);
    aud_module_event (in_module, in_port, aud_event_new_disconnect_sync ());
    aud_link_del (&in_port->module_node);
    del_input_port (in_port);
}

/* Connect only links peers. It does not itself invoke out_event or send FIRST. */
/* @0x600fcac4 */
int aud_module_connect (struct aud_port *out_port, struct aud_port *in_port)
{
    struct aud_module *out_module = out_port->module;
    struct aud_module *in_module = in_port->module;

    info ("module connect %s.%d -> %s.%d\n",
        out_module->name, out_port->number, in_module->name, in_port->number);

    aud_mutex_lock (&in_port->async_up_lock);
    in_port->up_peer = out_port;
    aud_mutex_unlock (&in_port->async_up_lock);

    aud_link_add_tail (&out_port->down_peer_root, &in_port->down_peer_node);

    return 0;
}

static int adjust_thread_priority (struct aud_module *module)
{
    // permit only lower thread_priority than value of default thread priority.
    // thread priority shall not be greater than preempt_pri.
    if (module->thread_priority < 0)
    {
        // xt->preempt_pri is maximum thread priority.
        // xt->preempt_pri is set as given value when thread was created.
        // so, thread_priority will be minus from xt->preempt_pri.c
        int preempt_pri = aud_thread_get_preempt_priority ();
        int cur_pri = aud_thread_get_priority ();
        int thread_pri = preempt_pri + module->thread_priority;

        info ("%s : change thread priority %d to %d\n", module->name, cur_pri, thread_pri);
        aud_thread_set_priority (thread_pri);
        module->thread_priority = 0;
    }

    return 0;
}

/* The data-path caller of output_arc.c::out_event: if current_first is NULL,
 * retain upstream current_first, or allocate aud_event_new_first if absent,
 * cache it and invoke impl->event BEFORE impl->chain. The fallback can use
 * out_event defaults PCM/2ch/48000/acmod=2; it is not evidence of source format.
 * Reconnect makes a new input eligible for replay; it need not be a new event
 * object. LAST clears the cached FIRST. FLUSH only toggles throwing here. */
/* @0x600fd044 */
static int aud_module_chain_locked (struct aud_port *port, struct aud_data *data)
{
    int ret;
    struct aud_module *module = port->module;

    aud_mutex_lock (&module->block_mutex);
    if (module->block_module)
        aud_cond_wait (&module->block_cond, &module->block_mutex);
    aud_mutex_unlock (&module->block_mutex);

    if (AUD_PORT_IS_DISABLED (port) || port->throwing)
    {
        aud_data_put (data);
        return -1;
    }
    if (module->thread_priority)
        adjust_thread_priority (module);

    if (!port->current_first)
    {
        struct aud_event *e = NULL;

        if (port->up_peer && port->up_peer->current_first)
            e = aud_event_get (port->up_peer->current_first);
        if (!e)
        {
            e = aud_event_new_first ();
            if (!e)
                return -1;
        }
        port->current_first = e;
        module->impl->event (module, port, aud_event_get (e));
    }

    port->last_clock = aud_system_cycle ();
    port->passed += data->size;
    port->flag_work |= AUD_PORT_WORK_CHAIN;
    if (module->do_chain_profile)
    {
        unsigned long long start = aud_thread_cycle ();
        ret = module->impl->chain (port, data);
        module->chain_profile += aud_thread_cycle () - start;
    }
    else
        ret = module->impl->chain (port, data);
    port->flag_work &= ~AUD_PORT_WORK_CHAIN;
    if (!module->othread && !(module->impl->flag & AUD_IMPL_HAVE_OUTPUT_THREAD)
        && !(module->impl->flag & AUD_IMPL_SKIP_SYNC))
        aud_link_foreach_node (&module->output_port_root, NULL, foreach_sync);
    return ret;
}

/* @0x600fd594 */
static int aud_module_event_flush_locked (struct aud_module *module, struct aud_port *port,
    struct aud_event *event)
{
    struct aud_port *in_port;

    if (!AUD_EVENT_IS_SYNC (event))
    {
        info ("(async) flush event received:%s\n", module->impl->name);
        aud_link_foreach (&module->input_port_root, in_port)
        {
            aud_mutex_lock (&in_port->check_free_mutex);
            in_port->throwing = 1;
            aud_mutex_unlock (&in_port->check_free_mutex);
        }
        aud_link_foreach_end (&module->input_port_root);
        aimc_raise_named_event (module->aimc, module->uid, "flush_start", NULL, 0);
    }
    else
    {
        info ("(sync) flush event received:%s\n", module->name);
        aud_link_foreach (&module->input_port_root, in_port)
        {
            aud_mutex_lock (&in_port->check_free_mutex);
            in_port->throwing = 0;
            aud_mutex_unlock (&in_port->check_free_mutex);
        }
        aud_link_foreach_end (&module->input_port_root);
        aimc_raise_named_event (module->aimc, module->uid, "flush_done", NULL, 0);
    }

    return 0;
}

static int aud_module_event_disconnect_locked (struct aud_module *module, struct aud_port *port,
    struct aud_event *event, bool * event_done)
{
    if (!AUD_EVENT_IS_SYNC (event))
    {
        int ret;
        info ("(async) disconnecting.\n");
        port->flag |= AUD_PORT_DISABLED;
        if (port->throwing)
        {
            info ("%s : port is throwing.. so unset\n", port->module->name);
            port->throwing = 0;
        }

        if (module->impl->disconnect_port)
        {
            ret = module->impl->disconnect_port (module, port);
            if (ret >= 0)
            {
                info ("disconnect event handled at input%d of %s\n", port->number, module->name);
                *event_done = true;
            }
        }
    }
    else if (AUD_EVENT_IS_SYNC (event))
    {
        info ("(sync) disconnecting.\n");
        if (port->flag & AUD_PORT_DISABLED)
        {
            info ("input%d of %sx. disconnect done.\n", port->number, module->name);
            port->flag &= ~AUD_PORT_DISABLED;
        }
        else
        {
            info ("input%d of %s. no more disconnecting.\n", port->number, module->name);
        }
    }

    return 0;
}

static int aud_module_event_first_au_locked (struct aud_module *module, struct aud_port *port,
    struct aud_event *event)
{
    if (port->current_first)
        aud_event_put (port->current_first);
    port->current_first = aud_event_get (event);

    adjust_thread_priority (module);

    return 0;
}

static int aud_module_event_last_au_locked (struct aud_module *module, struct aud_port *port,
    struct aud_event *event)
{
    if (port->current_first)
    {
        aud_event_put (port->current_first);
        port->current_first = NULL;
    }
    else
        warn ("last without first.. at input%d of %s\n", port->number, module->impl->name);

    return 0;
}

/* Explicit-event caller: FIRST caches the event, LAST clears it, then
 * impl->event dispatches to output_arc.c::out_event. Not a kernel call. */
/* @0x600fd680 */
static int aud_module_event_locked (struct aud_module *module, struct aud_port *port,
    struct aud_event *event)
{
    int ret;

    if (!module || !event)
        return -1;
    event = aud_event_get (event);
    if (port && !AUD_EVENT_IS_SYNC (event) && !AUD_EVENT_IS_DOWNWARD (event))
        aud_mutex_lock (&port->async_up_lock);
    if (port)
    {
        if (event->type != AUD_EVENT_TYPE_DISCONNECT && AUD_PORT_IS_DISABLED (port))
        {
            aud_event_put (event);
            ret = -1;
            goto done;
        }
        if (event->type == AUD_EVENT_TYPE_FLUSH)
            aud_module_event_flush_locked (module, port, event);
        else if (event->type == AUD_EVENT_TYPE_DISCONNECT)
        {
            bool event_done = false;
            aud_module_event_disconnect_locked (module, port, event, &event_done);
            if (event_done)
            {
                if (!AUD_EVENT_IS_SYNC (event) && !AUD_EVENT_IS_DOWNWARD (event))
                    aud_mutex_unlock (&port->async_up_lock);
                aud_event_put (event);
                return 0;
            }
        }
        else if (event->type == AUD_EVENT_TYPE_FIRST_AU)
            aud_module_event_first_au_locked (module, port, event);
        else if (event->type == AUD_EVENT_TYPE_LAST_AU)
            aud_module_event_last_au_locked (module, port, event);
    }
    ret = module->impl->event (module, port, event);
done:
    if (port && !AUD_EVENT_IS_SYNC (event) && !AUD_EVENT_IS_DOWNWARD (event))
        aud_mutex_unlock (&port->async_up_lock);
    return ret;
}

/* @0x600fd994 */
int aud_module_event (struct aud_module *module, struct aud_port *port, struct aud_event *event)
{
    int ret;

    if (!event)
    {
        warn ("no event.\n");
        return -1;
    }

    if (port)
    {
        if (AUD_EVENT_IS_SYNC (event))
            aud_mutex_lock (&port->stream_lock);
        else if (AUD_EVENT_IS_DOWNWARD (event))
            aud_mutex_lock (&port->async_down_lock);
    }

    ret = aud_module_event_locked (module, port, event);

    if (port)
    {
        if (AUD_EVENT_IS_SYNC (event))
            aud_mutex_unlock (&port->stream_lock);
        else if (AUD_EVENT_IS_DOWNWARD (event))
            aud_mutex_unlock (&port->async_down_lock);
    }

    aud_event_put (event);
    return ret;
}

/* @0x600faf3c */
static int foreach_down_peer_push_event (void *arg, void *node_data)
{
    struct aud_event *event = arg;
    struct aud_port *peer = node_data;
    struct aud_port *port = peer->up_peer;

    if (AUD_EVENT_IS_SYNC (event))
        aud_mutex_lock (&peer->stream_lock);
    else if (AUD_EVENT_IS_DOWNWARD (event))
        aud_mutex_lock (&peer->async_down_lock);

    aud_link_node_unlock (&peer->down_peer_node);

    debug ("%s.%s%d : write event to %s.%s%d\n",
        port->module->name,
        port->flag & AUD_PORT_OUTPUT ? "out" : "in",
        port->number,
        peer->module->name, peer->flag & AUD_PORT_OUTPUT ? "out" : "in", peer->number);

    event = aud_event_get (event);
    aud_module_event_locked (peer->module, peer, event);

    if (AUD_EVENT_IS_SYNC (event))
        aud_mutex_unlock (&peer->stream_lock);
    else if (AUD_EVENT_IS_DOWNWARD (event))
        aud_mutex_unlock (&peer->async_down_lock);

    aud_event_put (event);
    return 0;
}
