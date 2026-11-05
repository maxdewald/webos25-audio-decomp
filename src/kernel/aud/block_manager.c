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
 * Modified to match the shipped kernel Image (sha256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db,
 * orig/boot/Image-5.4.268-329.decompressed).
 *
 * Base: lg-opensource/kdriver/core/aud/block_manager.c (GPL-2.0), complete.
 * All functions were compared with the shipped disassembly (callees, log
 * strings, __LINE__ values: identical line numbers to the published file,
 * so the file is unchanged in this build). Several helpers are inlined in
 * the shipped image (_bm_connect_port, _bm_del_ports,
 * _bm_disconnect_blocks_without_update, _bm_reconnect_blocks) and
 * bm_create_block_peer inlines bm_get_preffered_processor/bm_create_block.
 * audio_block_impl layout in the shipped image: constructor +0x20,
 * destructor +0x28, pre_connect +0x30, post_connect +0x38,
 * pre_disconnect +0x40, post_disconnect +0x48, suspend +0x50, resume +0x58,
 * pre_reconnect +0x60, post_reconnect +0x68 (renderer impl @0xffffffc0122cc918,
 * mixer impl @0xffffffc0122cc9e8, output impl @0xffffffc0122cd1e8).
 * No functional differences from the published source were found.
 */




/** @file
 *
 *  interface for audio block manager
 *
 *  @author Jihoon Lee (gaius.lee@lge.com)
 *  @version    0.1
 *  @date       2020.06.30
 *
 *  @addtogroup base
 *  @{
 */
#include <linux/slab.h>

#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include "proc.h"

#include "hal/hal.h"
#include "log.h"
#include "block.h"
#include "pipeline.h"
#include "block_manager.h"

#include "farm.h"
#include "input.h"
#include "decoder.h"
#include "renderer.h"
#include "mixer.h"
#include "se.h"
#include "output.h"
#include "encoder.h"
#include "capture.h"

logm_define (aud_bm, log_level_noti);
#define trace(fmt,args...)  logm_trace(aud_bm,fmt,##args)
#define debug(fmt,args...)  logm_debug(aud_bm,fmt,##args)
#define info(fmt,args...)   logm_info(aud_bm,fmt,##args)
#define noti(fmt,args...)   logm_noti(aud_bm,fmt,##args)
#define error(fmt,args...)  logm_error(aud_bm,fmt,##args)

#define BLOCK_INDEX_TEMP_NUMBER 0x1000
#define BLOCK_INDEX_MAX_NUMBER 0x1FFFFFFF

#define BLOCK_PORT_IN 0x1
#define BLOCK_PORT_CONNECTED 0x2

#define IS_BLOCK_PORT_INPUT(x) (x->flag & BLOCK_PORT_IN)
#define SET_BLOCK_PORT_INPUT(x) (x->flag |= BLOCK_PORT_IN)
#define UNSET_BLOCK_PORT_INPUT(x) (x->flag &= ~BLOCK_PORT_IN)

#define IS_BLOCK_PORT_CONNECTED(x) (x->flag & BLOCK_PORT_CONNECTED)
#define SET_BLOCK_PORT_CONNECTED(x) (x->flag |= BLOCK_PORT_CONNECTED)
#define UNSET_BLOCK_PORT_CONNECTED(x) (x->flag &= ~BLOCK_PORT_CONNECTED)

#define STR_INOUT(in) ((in)?"IN":"OUT")

struct bm_list_block
{
    struct list_head list;

    const char *block_type;     ///< block type ( "decoder", "renderer"...)
    int index;                  ///< index of block
    int processor;              ///< processor that modules of block is created
    bool load_balanced;         ///< block is load balanced already..
    struct list_head list_port_input;   ///< list of input port. see @bm_list_block_port
    struct list_head list_port_output;  ///< list of output port. see @bm_list_block_port

    struct list_head list_port_forward; ///< list of port forwarding see @bm_list_port_forward

    struct aud_block_info *block_info;  ///< information context of block
    struct audio_block_impl *block_impl;

};

struct bm_list_block_port
{
    struct list_head list;

    char *caps;                 ///< (PCM, ES, Channel 갯수)
    unsigned int flag;          ///< output port or input port, is connected.. etc
    struct bm_list_block *owner;        ///< port 가 속한 block pointer
    int module_port;            ///< port number of linked f/w module

    struct list_head list_connected_port;       ///< list of connected next or prev port. see @bm_list_connected_port
};

struct bm_list_connected_port
{
    struct list_head list;

    struct bm_list_block_port *connected_port;  ///< connected next or prev port
};


struct bm_list_port_forward
{
    struct list_head list;

    bool in;

    struct apipeline *pipeline;

    int port_pipeline;
    int port_block;
};

struct bm_list_reserved_conncection
{
    struct list_head list;

    const char *block_type;
    int index;
    int port;
    bool in;                    ///< reservee is in module or not

    struct bm_list_block *reserver;
    struct apipeline *pipeline_reserver;        ///< pipeline of reserver
    int port_reserver;
};

struct bm_list_work_farm
{
    struct list_head list;

    struct bm_list_block *master_block;
    struct bm_list_block *farm_block;   ///< block of workerbin
};

struct aud_bm_info
{
    struct list_head list_block_impl;   ///< list of implementation of block
    struct list_head list_block;        ///< list of blocks. see @bm_list_block
    struct list_head list_reserved_connection;  ///< list of reserved connection. see @bm_list_reserved_conncection
    struct list_head list_work_farm;    ///< list of workerbin see @bm_list_work_farm

    bool init;
    bool suspended;             ///< is DSP suspended?
    unsigned int last_index;
    unsigned int main_audio_index;

    struct mutex bm_lock;
    struct mutex list_block_lock;
};

struct aud_bm_info _bm_info = { };

/* @0xffffffc010c9e464 */
struct audio_block_impl *_bm_find_block_impl_by_type (const char *block_type)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct audio_block_impl *block_impl = NULL;

    debug ("create block : %s\n", block_type);

    // find block impl in registered block list @see bm_init()
    list_for_each_entry (block_impl, &bm_info->list_block_impl, list)
    {
        trace ("type in block_impl list : %s\n", block_impl->block_type);
        if (!strcmp (block_type, block_impl->block_type))
        {
            debug ("matched in block_impl list.\n");
            return block_impl;
        }
    }

    error ("can't find %s in block_impl list!!\n", block_type);
    return NULL;
}


/* @0xffffffc010c9e2c4 */
static struct bm_list_reserved_conncection *_bm_find_block_in_reserved_list (const char *block_type,
    int index, int port)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_reserved_conncection *rvd_con = NULL;
    // find block in connect reserved list
    list_for_each_entry (rvd_con, &bm_info->list_reserved_connection, list)
    {
        trace ("type : %s (%d:%d)\n", rvd_con->block_type, rvd_con->index, rvd_con->port);
        if (!strcmp (block_type, rvd_con->block_type) && index == rvd_con->index)
        {
            if (port == -1 || port == rvd_con->port)
            {
                debug ("reserved matched! port %d, in %d, reserver is %s\n", rvd_con->port,
                    rvd_con->in, (rvd_con->reserver)->block_type);
                return rvd_con;
            }
        }
    }

    return false;
}

/* @0xffffffc010c9d320 */
static struct bm_list_work_farm *_bm_find_farm_in_workfarm_list (struct aud_block_info *block_info)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_work_farm *farm = NULL;

    // find block in farm list
    list_for_each_entry (farm, &bm_info->list_work_farm, list)
    {
        if (farm->master_block && farm->farm_block && farm->master_block->load_balanced)
        {
            trace ("type : %s (%d)\n", farm->master_block->block_type, farm->master_block->index);
            if (farm->master_block->block_info == block_info)
            {
                return farm;
            }
        }
    }

    return NULL;
}


/* @0xffffffc010c9dce0 */
static struct bm_list_block *_bm_find_block_by_index (const char *block_type, int index)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block = NULL;

    mutex_lock (&bm_info->list_block_lock);
    list_for_each_entry (block, &bm_info->list_block, list)
    {
        trace ("type  : %s (%p)\n", block->block_type, block->block_info);
        if (!strcmp (block_type, block->block_type) && index == block->index)
        {
            trace ("matched!!\n");
            goto exit;
        }
    }
    block = NULL;
  exit:
    mutex_unlock (&bm_info->list_block_lock);
    return block;
}

/* @0xffffffc010c9db50 */
static struct bm_list_block *_bm_find_block_by_block_info (struct aud_block_info *block_info)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block = NULL;
    debug ("find block %s\n", block_info->block_type);

    mutex_lock (&bm_info->list_block_lock);
    list_for_each_entry (block, &bm_info->list_block, list)
    {
        trace ("type  : %s %d(%p)\n", block->block_type, block->index, block->block_info);
        if (block->block_info == block_info)
        {
            debug ("matched!!\n");
            goto exit;
        }
    }
    block = NULL;
  exit:
    mutex_unlock (&bm_info->list_block_lock);
    return block;
}

/* @0xffffffc010c9de54 */
struct aud_block_info *bm_get_block_info_by_index (const char *block_type, int index)
{
    struct bm_list_block *block = NULL;
    struct aud_block_info *block_info = NULL;
    trace ("get block_info : %s %d\n", block_type, index);
    block = _bm_find_block_by_index (block_type, index);
    if (block)
    {
        trace ("find\n");
        block_info = block->block_info;
    }

    return block_info;
}

EXPORT_SYMBOL (bm_get_block_info_by_index);

/* @0xffffffc010c9e640 */
struct aud_block_info *bm_get_master_by_block_info (struct aud_block_info *block_info)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_work_farm *farm = NULL;

    // find block in connect reserved list
    list_for_each_entry (farm, &bm_info->list_work_farm, list)
    {
        trace ("type : %s (%d)\n", farm->master_block->block_type, farm->master_block->index);
        if (farm->farm_block->block_info == block_info && farm->master_block->load_balanced == true)
        {
            info ("find master.. %s.%d\n", farm->master_block->block_type,
                farm->master_block->index);
            return farm->master_block->block_info;
        }
    }

    return NULL;
}

/* @0xffffffc010c9d424 */
static struct bm_list_block_port *_bm_get_block_port (struct bm_list_block *block, int port,
    bool in)
{
    struct bm_list_block_port *block_port = NULL;
    struct list_head *list_port = NULL;
    debug ("get %s port %d from %s module\n", STR_INOUT (in), port, block->block_type);
    list_port = (in) ? &block->list_port_input : &block->list_port_output;
    list_for_each_entry (block_port, list_port, list)
    {
        trace ("port %d\n", block_port->module_port);
        if (block_port->module_port == port)
            return block_port;
    }

    return NULL;
}

/* @0xffffffc010c9e7a0 */
struct bm_list_port_forward *bm_get_port_forward (struct aud_block_info *block_info,
    bool in, int port_block)
{
    struct bm_list_block *block;
    struct bm_list_port_forward *port_forward;

    block = _bm_find_block_by_block_info (block_info);
    if (block == NULL)
    {
        error ("can't find block of %s\n", block_info->block_type);
        return NULL;
    }

    info ("get port forward : %s.%d (%s)\n", block_info->block_type, port_block, STR_INOUT (in));

    list_for_each_entry (port_forward, &block->list_port_forward, list)
    {
        if (port_forward->port_block == port_block && port_forward->in == in)
        {
            debug ("matched\n");
            return port_forward;
        }
    }

    return NULL;
}

/* @0xffffffc010c9d280: _bm_add_port_connection.isra.0 */
static int _bm_add_port_connection (struct bm_list_block_port *block_port,
    struct bm_list_block_port *connected_port)
{
    struct bm_list_connected_port *l;
    l = (struct bm_list_connected_port *) kzalloc (sizeof (struct bm_list_connected_port),
        GFP_KERNEL);
    if (!l)
    {
        error ("no mem\n\n");
        return -1;
    }

    l->connected_port = connected_port;
    list_add (&l->list, &block_port->list_connected_port);
    SET_BLOCK_PORT_CONNECTED (block_port);
    return 0;
}

static int _bm_connect_port (struct bm_list_block *block_out, int port_out,
    struct bm_list_block *block_in, int port_in)
{
    struct bm_list_block_port *block_port_out, *block_port_in;
    int ret = -1;
    block_port_out = _bm_get_block_port (block_out, port_out, PORT_OUT);
    if (!block_port_out)
    {
        error ("can't find %s (%d) port\n", block_out->block_type, port_out);
        goto exit;
    }

    block_port_in = _bm_get_block_port (block_in, port_in, PORT_IN);
    if (!block_port_in)
    {
        error ("can't find %s (%d) port\n", block_in->block_type, port_in);
        goto exit;
    }

    _bm_add_port_connection (block_port_out, block_port_in);
    _bm_add_port_connection (block_port_in, block_port_out);
    debug ("OK\n");
    ret = 0;
  exit:
    return ret;
}

/* @0xffffffc010c9d830: _bm_del_port.isra.0 */
static int _bm_del_port (struct bm_list_block_port *block_port)
{
    debug ("delete port %d from %s module\n", block_port->module_port,
        block_port->owner->block_type);
    if (IS_BLOCK_PORT_CONNECTED (block_port))
    {
        struct bm_list_connected_port *connected_port, *next;
        list_for_each_entry_safe (connected_port, next, &block_port->list_connected_port, list)
        {
            struct bm_list_block_port *block_port_connected = connected_port->connected_port;
            struct bm_list_connected_port *p, *n;
            debug ("connected : delete port %d from %s module\n",
                block_port_connected->module_port, block_port_connected->owner->block_type);
            // delete connected ports, too
            list_for_each_entry_safe (p, n, &block_port_connected->list_connected_port, list)
            {
                if (p->connected_port == block_port)
                {
                    debug ("delete port. from %s module\n",
                        block_port_connected->owner->block_type);
                    list_del (&p->list);
                    kfree (p);
                }
            }

            // unset connected in port flag if no connection
            if (list_empty (&block_port_connected->list_connected_port))
            {
                UNSET_BLOCK_PORT_CONNECTED (block_port_connected);
                debug ("unset connected in flag %x\n", block_port_connected->flag);
            }

            list_del (&connected_port->list);
            kfree (connected_port);
        }
    }

    list_del (&block_port->list);
    kfree (block_port);
    debug ("done\n");
    return 0;
}

static int _bm_del_ports (struct bm_list_block *block)
{
    struct bm_list_block_port *block_port, *next;
    debug ("delete ports from %s module\n", block->block_type);
    list_for_each_entry_safe (block_port, next, &block->list_port_input, list)
    {
        trace ("input port %d\n", block_port->module_port);
        _bm_del_port (block_port);
    }

    list_for_each_entry_safe (block_port, next, &block->list_port_output, list)
    {
        trace ("output port %d\n", block_port->module_port);
        _bm_del_port (block_port);
    }

    return 0;
}

/* @0xffffffc010c9df40: _bm_add_port.isra.0 */
static int _bm_add_port (struct bm_list_block *block, int port, bool in)
{
    struct bm_list_block_port *block_port;
    struct list_head *list_port = NULL;
    debug ("add %s port %d to %s module\n", STR_INOUT (in), port, block->block_type);
    // find port in block
    if (in)
        list_port = &block->list_port_input;
    else
        list_port = &block->list_port_output;
    list_for_each_entry (block_port, list_port, list)
    {
        trace ("port %d\n", block_port->module_port);
        if (block_port->module_port == port)
        {
            debug ("port %d is already added. add to connected module list\n", port);
            return 0;
        }
    }

    block_port =
        (struct bm_list_block_port *) kzalloc (sizeof (struct bm_list_block_port), GFP_KERNEL);
    if (!block_port)
    {
        error ("no mem\n");
        return -1;
    }

    block_port->owner = block;
    block_port->module_port = port;
    INIT_LIST_HEAD (&block_port->list_connected_port);
    if (in)
    {
        SET_BLOCK_PORT_INPUT (block_port);
        list_add (&block_port->list, &block->list_port_input);
    }
    else
        list_add (&block_port->list, &block->list_port_output);
    return 0;
}

/* @0xffffffc010ca0584 */
unsigned int bm_get_block_head (struct aud_block_info *block_info)
{
    struct apipeline *pipe;
    if (block_info == NULL)
    {
        error ("block_info is NULL\n");
        return 0;
    }

    info ("get head of block %s\n", block_info->block_type);
    pipe = block_info->pipeline;
    return apipeline_head (pipe);
}

/* @0xffffffc010c9e1f0 */
unsigned int bm_get_block_tail (struct aud_block_info *block_info)
{
    struct apipeline *pipe;
    if (block_info == NULL)
    {
        error ("block_info is NULL\n");
        return 0;
    }

    info ("get tail of block %s\n", block_info->block_type);
    pipe = block_info->pipeline;
    return apipeline_tail (pipe);
}

EXPORT_SYMBOL (bm_get_block_tail);

/* @0xffffffc010ca0660 */
bool bm_is_connected (struct aud_block_info * block_info, int port, bool in)
{
    struct bm_list_block *block = NULL;
    struct bm_list_block_port *block_port = NULL;
    bool connected = false;

    info ("block %s port%d is connected?\n", block_info->block_type, port);

    block = _bm_find_block_by_block_info (block_info);
    if (!block)
    {
        error ("can't find block\n");
        goto exit;
    }

    block_port = _bm_get_block_port (block, port, in);
    if (block_port)
        connected = true;

  exit:
    info ("connected = %s\n", (connected) ? "TRUE" : "FALSE");
    return connected;
}

/* @0xffffffc010ca07f0 */
int bm_get_index_by_block_info (struct aud_block_info *block_info)
{
    struct bm_list_block *block;

    block = _bm_find_block_by_block_info (block_info);
    if (block)
        return block->index;

    error ("not found.\n");
    return -1;
}

/* @0xffffffc010ca0870 */
int bm_get_preffered_processor (struct aud_block_info *block_info)
{
    struct bm_list_block *block;

    // only for 4 dsp model
    if (aud_hal_get_number_of_dsp () > 2)
    {
        block = _bm_find_block_by_block_info (block_info);
        if (block)
            return block->processor;
    }

    return -1;
}

/* @0xffffffc010ca08b0 */
int bm_set_preffered_processor (struct aud_block_info *block_info, int proc)
{
    struct bm_list_block *block;

    block = _bm_find_block_by_block_info (block_info);
    if (block)
        block->processor = proc;

    return 0;
}

/* @0xffffffc010c9e960: pre_disconnect runs for every connected output, then
 * the input; disconnect uses the forwarded input pipeline/port. Failure
 * (+0x180) preserves the recorded links and skips all post callbacks. Success
 * calls output posts, input post, then deletes the input port and peer links
 * (+0x1bc/+0x1e4/+0x1ec). Callback return values are ignored.
 */
static int _bm_disconnect_block_locked (struct aud_block_info *block_info_in, int port_in)
{
    struct apipeline *pipe_in;
    struct bm_list_block *block_in;
    struct bm_list_block_port *block_port;
    struct bm_list_port_forward *port_forward = NULL;
    struct bm_list_connected_port *c_port;
    int port_in_final;
    int ret = -1;

    if (block_info_in == NULL)
    {
        error ("block_info_in is NULL\n");
        return 0;
    }

    info ("disconnect block ??? - %s\n", block_info_in->block_type);

    block_in = _bm_find_block_by_block_info (block_info_in);
    if (!block_in)
    {
        error ("can't find %s block!!!\n", block_info_in->block_type);
        goto exit;
    }

    block_port = _bm_get_block_port (block_in, port_in, PORT_IN);
    if (!block_port)
    {
        info ("can't find %s (%d) port\n", block_in->block_type, port_in);
        goto exit;
    }

    if (list_empty (&block_port->list_connected_port))
    {
        info ("no connected block with %s %d port %d\n", block_in->block_type, block_in->index,
            port_in);
        goto exit;
    }

    // call pre-disconnect for out block
    list_for_each_entry (c_port, &block_port->list_connected_port, list)
    {
        if (c_port && c_port->connected_port)
        {
            struct bm_list_block_port *connected_port = c_port->connected_port;

            if (connected_port->owner)
            {
                struct audio_block_impl *block_impl = connected_port->owner->block_impl;
                struct aud_block_info *block_info_out = connected_port->owner->block_info;
                int port_out = connected_port->module_port;

                // call pre connect function
                if (block_impl->pre_disconnect)
                    block_impl->pre_disconnect (block_info_out, port_out, PORT_OUT);
            }
        }
    }

    // call pre-disconnect for in block
    if (block_in->block_impl->pre_disconnect)
        block_in->block_impl->pre_disconnect (block_info_in, port_in, PORT_IN);

    // find in port forward list
    port_forward = bm_get_port_forward (block_info_in, PORT_IN, port_in);
    if (port_forward)
    {
        pipe_in = port_forward->pipeline;
        port_in_final = port_forward->port_pipeline;
        info ("port fowarding %s.%d -> (%s.%d)\n", block_info_in->block_type, port_in,
            apipeline_get_name (pipe_in), port_in_final);
    }
    else
    {
        pipe_in = block_info_in->pipeline;
        port_in_final = port_in;
    }

    // disconnect!!!
    ret = apipeline_disconnect (pipe_in, port_in_final);
    if (ret)
    {
        error ("error in disconnect ??? - %s (%d)!!\n", apipeline_get_name (pipe_in),
            port_in_final);
        goto exit;
    }

    // call post-disconnect for out block
    list_for_each_entry (c_port, &block_port->list_connected_port, list)
    {
        if (c_port && c_port->connected_port)
        {
            struct bm_list_block_port *connected_port = c_port->connected_port;

            if (connected_port->owner)
            {
                struct audio_block_impl *block_impl = connected_port->owner->block_impl;
                struct aud_block_info *block_info_out = connected_port->owner->block_info;
                int port_out = connected_port->module_port;

                // call pre connect function
                if (block_impl->post_disconnect)
                    block_impl->post_disconnect (block_info_out, port_out, PORT_OUT);
            }
        }
    }

    // call post-disconnect for in block
    if (block_in->block_impl->post_disconnect)
        block_in->block_impl->post_disconnect (block_info_in, port_in, PORT_IN);

    // delete port of this block and connected block
    _bm_del_port (block_port);

    info ("OK\n");

  exit:
    return ret;
}

/* @0xffffffc010c9edc0: NULL is a successful no-op; otherwise bm_lock covers
 * callbacks, pipeline disconnect and bookkeeping, and the helper result wins. */
int bm_disconnect_block (struct aud_block_info *block_info_in, int port_in)
{
    struct aud_bm_info *bm_info = &_bm_info;
    int ret = -1;

    if (block_info_in == NULL)
    {
        error ("block_info_in is NULL\n");
        return 0;
    }

    mutex_lock (&bm_info->bm_lock);
    ret = _bm_disconnect_block_locked (block_info_in, port_in);
    mutex_unlock (&bm_info->bm_lock);
    return ret;
}

EXPORT_SYMBOL (bm_disconnect_block);

/* @0xffffffc010ca08e0 */
int bm_disconnect_block_reserved (struct aud_block_info *block_info_in, int port_in)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block_in;
    struct bm_list_reserved_conncection *rvd_con = NULL, *next;
    int ret = -1;
    info ("disconnect pipeline ??? - %s\n", block_info_in->block_type);
    block_in = _bm_find_block_by_block_info (block_info_in);
    if (!block_in)
    {
        error ("can't find %s block!!!\n", block_info_in->block_type);
        goto exit;
    }

    // find block in connecting reservation list
    list_for_each_entry_safe (rvd_con, next, &bm_info->list_reserved_connection, list)
    {
        trace ("type : %s (%d)\n", rvd_con->block_type, rvd_con->index);
        if (rvd_con->in)
        {
            if (rvd_con->block_type == block_in->block_type && rvd_con->port == port_in
                && rvd_con->index == block_in->index)
            {
                debug ("connection reserved for %s (%d) is deleted\n", rvd_con->block_type,
                    rvd_con->index);
                list_del (&rvd_con->list);
                kfree (rvd_con);
            }
        }
        else
        {
            if (rvd_con->reserver == block_in && rvd_con->port_reserver == port_in)
            {
                debug ("connection reserved for %s (%d) is deleted\n", rvd_con->block_type,
                    rvd_con->index);
                list_del (&rvd_con->list);
                kfree (rvd_con);
            }
        }
    }

    bm_disconnect_block (block_info_in, port_in);
    info ("OK\n");
  exit:
    return ret;
}

/* @0xffffffc010ca0be0 */
int bm_remove_reserved (struct aud_block_info *block_info, int port, bool reserved_as_output)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *list_block;
    struct bm_list_reserved_conncection *rvd_con = NULL, *next;
    int ret = -1;

    mutex_lock (&bm_info->bm_lock);

    info ("disconnect pipeline ??? - %s\n", block_info->block_type);

    list_block = _bm_find_block_by_block_info (block_info);
    if (!list_block)
    {
        error ("can't find %s block!!!\n", block_info->block_type);
        goto exit;
    }

    // find block in connecting reservation list
    list_for_each_entry_safe (rvd_con, next, &bm_info->list_reserved_connection, list)
    {
        trace ("type : %s (%d)\n", rvd_con->block_type, rvd_con->index);
        if (rvd_con->in == reserved_as_output)
        {
            if (rvd_con->reserver == list_block && rvd_con->port_reserver == port)
            {
                debug ("connection reserved for %s (%d) is deleted\n", rvd_con->block_type,
                    rvd_con->index);
                list_del (&rvd_con->list);
                kfree (rvd_con);
            }
        }
    }

    info ("OK\n");
  exit:
    mutex_unlock (&bm_info->bm_lock);

    return ret;
}

/*
 * Shipped (@0xffffffc010c9f544, 0x4fc bytes) matches the published logic:
 *   pre_connect(out, port_out, PORT_OUT)  [impl+0x30, +0xb4]
 *   pre_connect(in,  port_in,  PORT_IN)   [impl+0x30, +0xd0]
 *   resolve port forwards, apipeline_connect()          [+0x1cc]
 *   on error: log "error in connect ..." and RETURN (no post_connect,
 *             no _bm_add_port, nothing undone)           [+0x1d4 -> +0x230]
 *   _bm_add_port x2, port link (inlined _bm_connect_port); their allocation
 *   failures and all callback return values do not change the pipeline result.
 *   post_connect(out, port_out, PORT_OUT) [impl+0x38, +0x308]
 *   post_connect(in,  port_in,  PORT_IN)  [impl+0x38, +0x324]
 * So post_connect is skipped only when apipeline_connect fails (e.g. a head or
 * tail uid of 0, or aimc_module_connect error); in that case
 * renderer_preconnect's "drop_output_data=1" is left in place.
 * It is never skipped for a successful BYPASS_PORT connect.
 */
/* @0xffffffc010c9f544 */
static int _bm_connect_block_locked (struct aud_block_info *block_info_out, int port_out,
    struct aud_block_info *block_info_in, int port_in)
{
    struct bm_list_block *block_out, *block_in;
    struct apipeline *pipe_out, *pipe_in;
    struct bm_list_port_forward *port_forward = NULL;
    int port_out_final, port_in_final;
    int ret = -1;

    info ("connect %s (%d) - %s (%d)\n", block_info_out->block_type, port_out,
        block_info_in->block_type, port_in);
    // add ports to block
    block_out = _bm_find_block_by_block_info (block_info_out);
    if (!block_out)
    {
        error ("can't find %s block!!!\n", block_info_out->block_type);
        goto exit;
    }

    block_in = _bm_find_block_by_block_info (block_info_in);
    if (!block_in)
    {
        error ("can't find %s block!!!\n", block_info_in->block_type);
        goto exit;
    }

    // call pre connect function
    if (block_out->block_impl->pre_connect)
        block_out->block_impl->pre_connect (block_info_out, port_out, PORT_OUT);
    if (block_in->block_impl->pre_connect)
        block_in->block_impl->pre_connect (block_info_in, port_in, PORT_IN);

    // find in port forward list
    port_forward = bm_get_port_forward (block_info_out, PORT_OUT, port_out);
    if (port_forward)
    {
        pipe_out = port_forward->pipeline;
        port_out_final = port_forward->port_pipeline;
        info ("port fowarding %s.%d -> (%s.%d)\n", block_info_out->block_type, port_out,
            apipeline_get_name (pipe_out), port_out_final);
    }
    else
    {
        pipe_out = block_info_out->pipeline;
        port_out_final = port_out;
    }

    port_forward = bm_get_port_forward (block_info_in, PORT_IN, port_in);
    if (port_forward)
    {
        pipe_in = port_forward->pipeline;
        port_in_final = port_forward->port_pipeline;
        info ("port fowarding %s.%d -> (%s.%d)\n", block_info_in->block_type, port_in,
            apipeline_get_name (pipe_in), port_in_final);
    }
    else
    {
        pipe_in = block_info_in->pipeline;
        port_in_final = port_in;
    }

    // connect pipeline
    ret = apipeline_connect (pipe_out, port_out_final, pipe_in, port_in_final);
    if (ret)
    {
        error ("error in connect %s (%d) - %s (%d)!!\n", apipeline_get_name (pipe_out),
            port_out, apipeline_get_name (pipe_in), port_in);
        goto exit;
    }

    _bm_add_port (block_out, port_out, PORT_OUT);
    _bm_add_port (block_in, port_in, PORT_IN);
    _bm_connect_port (block_out, port_out, block_in, port_in);

    // call post connect function
    if (block_out->block_impl->post_connect)
        block_out->block_impl->post_connect (block_info_out, port_out, PORT_OUT);
    if (block_in->block_impl->post_connect)
        block_in->block_impl->post_connect (block_info_in, port_in, PORT_IN);

    info ("OK\n");

  exit:
    return ret;
}

/* @0xffffffc010c9fa40 */
int bm_connect_block (struct aud_block_info *block_info_out, int port_out,
    struct aud_block_info *block_info_in, int port_in)
{
    struct aud_bm_info *bm_info = &_bm_info;
    int ret = -1;

    mutex_lock (&bm_info->bm_lock);
    ret = _bm_connect_block_locked (block_info_out, port_out, block_info_in, port_in);
    mutex_unlock (&bm_info->bm_lock);

    return ret;
}

EXPORT_SYMBOL (bm_connect_block);


/* @0xffffffc010ca0e70 */
int bm_connect_block_reserved (struct aud_block_info *block_info_in, int port_in,
    const char *block_type_out, int index_out, int port_out)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block_in, *block_out;
    struct aud_block_info *block_info_out;
    struct bm_list_reserved_conncection *rvd_con = NULL;
    int ret = -1;

    mutex_lock (&bm_info->bm_lock);

    info ("connect %s (%d) - %s (%d)\n", block_type_out, port_out, block_info_in->block_type,
        port_in);
    block_in = _bm_find_block_by_block_info (block_info_in);
    if (!block_in)
    {
        error ("can't find %s block!!!\n", block_info_in->block_type);
        goto exit;
    }

    // find block in connect reserved list
    rvd_con = _bm_find_block_in_reserved_list (block_type_out, index_out, port_out);
    if (!rvd_con)
    {
        struct audio_block_impl *block_impl = NULL;
        info ("reserve %s block..\n", block_type_out);
        block_impl = _bm_find_block_impl_by_type (block_type_out);
        if (block_impl == NULL)
        {
            error ("error in _bm_find_block_impl_by_type (%s)\n", block_type_out);
            goto exit;
        }

        // reserve connection
        rvd_con =
            (struct bm_list_reserved_conncection *) kzalloc (sizeof (struct
                bm_list_reserved_conncection), GFP_KERNEL);
        if (!rvd_con)
        {
            error ("no mem\n");
            goto exit;
        }

        rvd_con->block_type = block_impl->block_type;
        rvd_con->port = port_out;
        rvd_con->index = index_out;
        rvd_con->reserver = block_in;
        rvd_con->port_reserver = port_in;
        list_add (&rvd_con->list, &bm_info->list_reserved_connection);
    }

    block_out = _bm_find_block_by_index (block_type_out, index_out);
    if (!block_out)
    {
        info ("%s block is not created yet.. so not connect\n", block_type_out);
        goto exit;
    }

    block_info_out = block_out->block_info;
    _bm_connect_block_locked (block_info_out, port_out, block_info_in, port_in);

    info ("OK\n");

  exit:
    mutex_unlock (&bm_info->bm_lock);
    return ret;
}

/* @0xffffffc010c9fab4: reservations persist even after an immediate connect.
 * The lookup key is type/index/port, not direction or reserver; an existing
 * entry is not replaced. The connect result is discarded and every exit
 * returns -1 (+0x128), including successful and deferred connections.
 */
static int _bm_connect_block_reserved (struct aud_block_info *block_info, int port,
    const char *block_type_to, int index_to, int port_to, bool to_in)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block_from, *block_to;
    struct aud_block_info *block_info_in, *block_info_out;
    struct bm_list_reserved_conncection *rvd_con = NULL;
    int port_in, port_out;
    int ret = -1;

    mutex_lock (&bm_info->bm_lock);

    if (to_in)
        info ("connect %s (%d) - %s (%d)\n", block_info->block_type, port, block_type_to, port_to);
    else
        info ("connect %s (%d) - %s (%d)\n", block_type_to, port_to, block_info->block_type, port);

    block_from = _bm_find_block_by_block_info (block_info);
    if (!block_from)
    {
        error ("can't find %s block!!!\n", block_info->block_type);
        goto exit;
    }

    // find block in connect reserved list
    rvd_con = _bm_find_block_in_reserved_list (block_type_to, index_to, port_to);
    if (!rvd_con)
    {
        struct audio_block_impl *block_impl = NULL;
        info ("reserve %s block..\n", block_type_to);
        block_impl = _bm_find_block_impl_by_type (block_type_to);
        if (block_impl == NULL)
        {
            error ("error in _bm_find_block_impl_by_type (%s)\n", block_type_to);
            goto exit;
        }

        // reserve connection
        rvd_con =
            (struct bm_list_reserved_conncection *) kzalloc (sizeof (struct
                bm_list_reserved_conncection), GFP_KERNEL);
        if (!rvd_con)
        {
            error ("no mem\n");
            goto exit;
        }

        rvd_con->block_type = block_impl->block_type;
        rvd_con->port = port_to;
        rvd_con->index = index_to;
        rvd_con->reserver = block_from;
        rvd_con->port_reserver = port;
        rvd_con->in = to_in;
        list_add (&rvd_con->list, &bm_info->list_reserved_connection);
    }

    block_to = _bm_find_block_by_index (block_type_to, index_to);
    if (!block_to)
    {
        info ("%s block is not created yet.. so not connect\n", block_type_to);
        goto exit;
    }

    if (to_in)
    {
        block_info_in = block_to->block_info;
        port_in = port_to;
        block_info_out = block_info;
        port_out = port;
    }
    else
    {
        block_info_in = block_info;
        port_in = port;
        block_info_out = block_to->block_info;
        port_out = port_to;
    }

    _bm_connect_block_locked (block_info_out, port_out, block_info_in, port_in);

    info ("OK\n");

  exit:
    mutex_unlock (&bm_info->bm_lock);
    return ret;
}


/* @0xffffffc010ca11b4 */
int bm_connect_block_reserved_in (struct aud_block_info *block_info_out, int port_out,
    const char *block_type_in, int index_in, int port_in)
{
    return _bm_connect_block_reserved (block_info_out, port_out, block_type_in, index_in, port_in,
        true);
}

/* @0xffffffc010ca11d0: reserve the missing output for this input (to_in=false);
 * returns the shared helper's -1, not the pipeline connection status. */
int bm_connect_block_reserved_out (const char *block_type_out, int index_out, int port_out,
    struct aud_block_info *block_info_in, int port_in)
{
    return _bm_connect_block_reserved (block_info_in, port_in, block_type_out, index_out, port_out,
        false);
}


/* @0xffffffc010ca1210 */
int bm_unregister_port_forward (struct aud_block_info *block_info, bool in, int port_block)
{
    struct bm_list_block *block;
    struct bm_list_port_forward *port_forward;
    info ("unregister port forwarding : %s.%d (%s)\n", block_info->block_type, port_block,
        STR_INOUT (in));
    block = _bm_find_block_by_block_info (block_info);
    port_forward = bm_get_port_forward (block_info, in, port_block);
    if (port_forward)
    {
        debug ("free port_forward list\n");
        list_del (&port_forward->list);
        kfree (port_forward);
    }

    return 0;
}

/* @0xffffffc010ca1354 */
int bm_set_main_audio_decoder_index (int index)
{
    struct aud_bm_info *bm_info = &_bm_info;

    info ("Main Audio index is chagned %d -> %d\n", bm_info->main_audio_index, index);

    bm_info->main_audio_index = index;

    return 0;
}

/* @0xffffffc010ca13e0 */
bool bm_is_main_audio (struct aud_block_info * block_info)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block;

    if (bm_info->main_audio_index == -1)
    {
        noti ("main audio index is not setted yet.. so you're only and main audio\n");
        return true;
    }

    block = _bm_find_block_by_block_info (block_info);
    if (block)
    {
        info ("%s index %d, main audio index : %d\n", block->block_type, block->index,
            bm_info->main_audio_index);
        if (block->index == bm_info->main_audio_index)
            return true;
        else
            return false;
    }

    return false;
}

/*
block 의 input/output ( bool in ) port int port_block 에 연결이 오면 struct apipeline *pipeline 의 port int port_pipeline 연결하도록 한다.
만일 이게 없으면 그냥 1 : 1 매핑 ( 기본 pipeline 의 0번 포트로 연결 )

/* @0xffffffc010ca1510 */
int bm_register_port_forward (struct aud_block_info *block_info, bool in,
    int port_block, struct apipeline *pipeline, int port_pipeline)
{
    struct bm_list_block *block;
    struct bm_list_port_forward *port_forward;

    info ("register port forwarding : %s.%d (%s) - %s.%d\n", block_info->block_type, port_block,
        STR_INOUT (in), block_info->block_type, port_pipeline);

    block = _bm_find_block_by_block_info (block_info);
    if (block == NULL)
    {
        error ("can't find block of %s\n", block_info->block_type);
        return -1;
    }

    list_for_each_entry (port_forward, &block->list_port_forward, list)
    {
        if (port_forward->port_block == port_block &&
            port_forward->port_pipeline == port_pipeline && port_forward->in == in)
        {
            // 100% same...
            if (port_forward->pipeline == pipeline)
            {
                error ("already registered\n");
                return -1;
            }

            // same information exist but pipeline pointer...
            // update pipeline pointer only...
            debug ("same but needed to update....\n");
            port_forward->pipeline = pipeline;
            return 0;
        }
    }

    // register port forwarding
    port_forward =
        (struct bm_list_port_forward *) kzalloc (sizeof (struct bm_list_port_forward), GFP_KERNEL);
    if (!port_forward)
    {
        error ("no mem\n");
        return -1;
    }

    port_forward->in = in;
    port_forward->port_block = port_block;
    port_forward->port_pipeline = port_pipeline;
    port_forward->pipeline = pipeline;
    INIT_LIST_HEAD (&port_forward->list);
    list_add (&port_forward->list, &block->list_port_forward);
    return 0;
}


/* Inlined in bm_suspend @0xffffffc010ca1780: disconnect recorded output peers
 * (+0x198), resolving only input forwards. Ignore failures and retain links;
 * unlike bm_disconnect_block, no pre/post_disconnect callbacks are invoked. */
static int _bm_disconnect_blocks_without_update (void)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block = NULL;

    info ("disconnect blocks without updating bm info..\n");

    // connection list
    list_for_each_entry (block, &bm_info->list_block, list)
    {
        struct bm_list_block_port *port;

        debug ("block %s%d...\n", block->block_type, block->index);

        list_for_each_entry (port, &block->list_port_output, list)
        {
            struct bm_list_connected_port *c_port;
            debug ("  output port%d...\n", port->module_port);

            list_for_each_entry (c_port, &port->list_connected_port, list)
            {
                if (c_port->connected_port)
                {
                    struct bm_list_block_port *connected_port = c_port->connected_port;
                    struct aud_block_info *block_info_in = NULL;
                    struct bm_list_port_forward *port_forward = NULL;
                    int port_in, port_in_final;
                    struct apipeline *pipe_in;

                    block_info_in = connected_port->owner->block_info;
                    port_in = connected_port->module_port;

                    // find in port forward list
                    port_forward = bm_get_port_forward (block_info_in, PORT_IN, port_in);
                    if (port_forward)
                    {
                        pipe_in = port_forward->pipeline;
                        port_in_final = port_forward->port_pipeline;
                    }
                    else
                    {
                        pipe_in = block_info_in->pipeline;
                        port_in_final = port_in;
                    }

                    debug ("     disconnect -> (%s.%d)\n", block_info_in->block_type,
                        port_in_final);

                    apipeline_disconnect (pipe_in, port_in_final);
                }
            }
        }
    }

    info ("done.\n");

    return 0;
}

/*
 * Inlined into bm_resume (@0xffffffc010ca1b80): calls impl->pre_reconnect
 * (+0x60), apipeline_connect for every recorded link, impl->post_reconnect
 * (+0x68). pre_connect/post_connect are NOT called on resume, so the
 * drop_output_data pre/post pair is not replayed either. Both endpoints use
 * current port forwarding. apipeline_connect's result (+0x2d8) is ignored:
 * post_reconnect still runs for every block after a failed reconnect.
 */
static int _bm_reconnect_blocks (void)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block = NULL;

    info ("re-sconnect blocks by bm info..\n");

    // call functions which need to be called after reconnections
    list_for_each_entry (block, &bm_info->list_block, list)
    {
        struct audio_block_impl *block_impl = block->block_impl;
        debug ("pre-reconnect : block %s%d...\n", block->block_type, block->index);

        if (block_impl->pre_reconnect)
            block_impl->pre_reconnect (block->block_info);
    }

    // connection list
    list_for_each_entry (block, &bm_info->list_block, list)
    {
        struct bm_list_block_port *port;

        debug ("reconnect : block %s%d...\n", block->block_type, block->index);

        list_for_each_entry (port, &block->list_port_output, list)
        {
            struct bm_list_connected_port *c_port;
            debug ("  output port%d...\n", port->module_port);

            list_for_each_entry (c_port, &port->list_connected_port, list)
            {
                if (c_port->connected_port)
                {
                    struct bm_list_block_port *connected_port = c_port->connected_port;
                    struct aud_block_info *block_info_out, *block_info_in;
                    struct bm_list_port_forward *port_forward = NULL;
                    int port_out, port_in;
                    int port_out_final, port_in_final;
                    struct apipeline *pipe_in, *pipe_out;

                    block_info_out = block->block_info;
                    port_out = port->module_port;
                    block_info_in = connected_port->owner->block_info;
                    port_in = connected_port->module_port;

                    // find in port forward list
                    port_forward = bm_get_port_forward (block_info_out, PORT_OUT, port_out);
                    if (port_forward)
                    {
                        pipe_out = port_forward->pipeline;
                        port_out_final = port_forward->port_pipeline;
                    }
                    else
                    {
                        pipe_out = block_info_out->pipeline;
                        port_out_final = port_out;
                    }

                    port_forward = bm_get_port_forward (block_info_in, PORT_IN, port_in);
                    if (port_forward)
                    {
                        pipe_in = port_forward->pipeline;
                        port_in_final = port_forward->port_pipeline;
                    }
                    else
                    {
                        pipe_in = block_info_in->pipeline;
                        port_in_final = port_in;
                    }

                    debug ("     connect %s.%d -> (%s.%d)\n", block_info_out->block_type,
                        port_out_final, block_info_in->block_type, port_in_final);

                    // connect pipeline
                    apipeline_connect (pipe_out, port_out_final, pipe_in, port_in_final);
                }
            }
        }
    }

    // call functions which need to be called after reconnections
    list_for_each_entry (block, &bm_info->list_block, list)
    {
        struct audio_block_impl *block_impl = block->block_impl;
        debug ("post-reconnect : block %s%d...\n", block->block_type, block->index);

        if (block_impl->post_reconnect)
            block_impl->post_reconnect (block->block_info);
    }

    return 0;
}

/* @0xffffffc010ca1780 */
int bm_suspend (void)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block = NULL;

    noti ("bm suspend..\n");

    bm_info->suspended = true;

    mutex_lock (&bm_info->bm_lock);

    // disconnect blocks without information update
    _bm_disconnect_blocks_without_update ();

    list_for_each_entry (block, &bm_info->list_block, list)
    {
        struct audio_block_impl *block_impl = block->block_impl;

        trace ("type  : %s %d(%p)\n", block->block_type, block->index, block->block_info);

        if (block_impl->suspend)
            block_impl->suspend (block->block_info);
    }

    noti ("bm suspend done.\n");

    mutex_unlock (&bm_info->bm_lock);

    return 0;
}

/* @0xffffffc010ca1b80: under bm_lock, resume ALL blocks before reconnect's
 * three passes; clear suspended only after unlocking (+0x410/+0x418). */
int bm_resume (void)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block = NULL;

    noti ("bm resume..\n");

    mutex_lock (&bm_info->bm_lock);

    list_for_each_entry (block, &bm_info->list_block, list)
    {
        struct audio_block_impl *block_impl = block->block_impl;

        trace ("type  : %s %d(%p)\n", block->block_type, block->index, block->block_info);

        if (block_impl->resume)
            block_impl->resume (block->block_info);
    }

    // re-connect blocks
    _bm_reconnect_blocks ();

    mutex_unlock (&bm_info->bm_lock);

    bm_info->suspended = false;

    noti ("bm resume done.\n");

    return 0;
}

/* @0xffffffc010ca20d0 */
bool bm_get_suspended (void)
{
    struct aud_bm_info *bm_info = &_bm_info;

    return bm_info->suspended;
}

/* @0xffffffc010ca0480 */
struct aud_block_info *bm_create_block_peer (const char *block_type, int index,
    struct aud_block_info *peer, bool peer_is_input)
{
    struct audio_block_impl *block_impl = NULL;
    struct aud_block_info *block_info = NULL;
    int preffered_processor;

    // find block impl in registered block list @see bm_init()
    block_impl = _bm_find_block_impl_by_type (block_type);
    if (block_impl == NULL)
    {
        error ("error in _bm_find_block_impl_by_type(%s)\n", block_type);
        goto exit;
    }

    // back up
    preffered_processor = block_impl->preffered_processor;

    if (peer)
        block_impl->preffered_processor = bm_get_preffered_processor (peer);

    block_info = bm_create_block (block_type, index);

    // restore
    block_impl->preffered_processor = preffered_processor;
  exit:
    return block_info;
}

EXPORT_SYMBOL (bm_create_block_peer);

/* @0xffffffc010c9fe90 */
static struct aud_block_info *_bm_create_block_locked (const char *block_type, int index)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block = NULL;
    struct audio_block_impl *block_impl = NULL;
    struct aud_block_info *block_info = NULL;
    struct bm_list_reserved_conncection *rvd_con = NULL;

    debug ("create block : %s\n", block_type);

    if (index >= 0)
    {
        // first, find in block list
        block_info = bm_get_block_info_by_index (block_type, index);
        if (block_info)
        {
            error ("already created!\n");
            goto exit;
        }
    }
    else
    {
        index = ++bm_info->last_index;
        info ("temporary index : %u\n", index);
    }

    // find block impl in registered block list @see bm_init()
    block_impl = _bm_find_block_impl_by_type (block_type);
    if (block_impl == NULL)
    {
        error ("error in _bm_find_block_impl_by_type(%s)\n", block_type);
        goto exit;
    }

    // alloc and set block info
    block_info = kzalloc (block_impl->block_info_size, GFP_KERNEL);
    if (block_info == NULL)
    {
        error ("no mem!\n");
        goto exit;
    }

    block = kzalloc (sizeof (struct bm_list_block), GFP_KERNEL);
    if (block == NULL)
    {
        error ("no mem!\n");
        if (block_info)
            kfree (block_info);
        block_info = NULL;
        goto exit;
    }

    block_info->block_type = block_impl->block_type;
    block->block_info = block_info;
    block->block_type = block_impl->block_type;
    block->index = index;
    block->processor = (!block_impl->preffered_processor) ? -1 : block_impl->preffered_processor;
    block->block_impl = block_impl;
    INIT_LIST_HEAD (&block->list_port_input);
    INIT_LIST_HEAD (&block->list_port_output);
    INIT_LIST_HEAD (&block->list_port_forward);
    INIT_LIST_HEAD (&block->list);
    mutex_lock (&bm_info->list_block_lock);
    list_add (&block->list, &bm_info->list_block);
    mutex_unlock (&bm_info->list_block_lock);

    // call constructor of block
    block_impl->constructor (block_info);

    debug ("alloc and add in block list done.\n");

    // @0xffffffc010c9fe90 +0x2a8/+0x32c: replay every matching reservation
    // after construction, using its direction; retain entries and ignore errors.
    rvd_con = _bm_find_block_in_reserved_list (block_type, index, -1);
    if (rvd_con)
    {
        list_for_each_entry (rvd_con, &bm_info->list_reserved_connection, list)
        {
            trace ("type : %s (%d:%d)\n", rvd_con->block_type, rvd_con->index, rvd_con->port);
            if (!strcmp (block_type, rvd_con->block_type) && index == rvd_con->index)
            {
                struct aud_block_info *block_info_out, *block_info_in;
                int port_out, port_in;

                debug ("%s is in reserved list.. by %s\n", block_type,
                    rvd_con->reserver->block_type);

                if (rvd_con->in)
                {
                    // (B) - (A)
                    // (A) block which is just created is in module.
                    // (B) block which reserved is out module
                    block_info_out = rvd_con->reserver->block_info;
                    port_out = rvd_con->port_reserver;
                    block_info_in = block_info;
                    port_in = rvd_con->port;
                }
                else
                {
                    // (A) - (B): newly created output -> reserving input.
                    block_info_out = block_info;
                    port_out = rvd_con->port;
                    block_info_in = rvd_con->reserver->block_info;
                    port_in = rvd_con->port_reserver;
                }

                info ("connect %s.%d - %s.%d by reservation..\n", block_info->block_type, port_out,
                    block_info_in->block_type, port_in);

                _bm_connect_block_locked (block_info_out, port_out, block_info_in, port_in);
            }
        }
    }

    debug ("done : %p\n", block_info);

  exit:
    return block_info;
}

/* @0xffffffc010ca0420 */
struct aud_block_info *bm_create_block (const char *block_type, int index)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct aud_block_info *block_info;

    mutex_lock (&bm_info->bm_lock);
    block_info = _bm_create_block_locked (block_type, index);
    mutex_unlock (&bm_info->bm_lock);
    return block_info;
}

EXPORT_SYMBOL (bm_create_block);

/* @0xffffffc010c9ee80 */
static int _bm_destroy_block_locked (struct aud_block_info *block_info)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block;
    struct bm_list_reserved_conncection *rvd_con, *next_r;
    struct bm_list_port_forward *port_forward, *next_p;
    struct bm_list_work_farm *farm;
    int ret = -1;

    if (block_info == NULL)
    {
        error ("block_info is NULL!\n");
        return -1;
    }

    debug ("destroy block : %s (%p)\n", block_info->block_type, block_info);
    block = _bm_find_block_by_block_info (block_info);
    if (block == NULL)
    {
        error ("can't find block type : %s\n", block_info->block_type);
        goto exit;
    }

    list_for_each_entry_safe (rvd_con, next_r, &bm_info->list_reserved_connection, list)
    {
        if (rvd_con->reserver)
        {
            trace ("%s%d port%d  is connect-reserved by %s\n", rvd_con->block_type, rvd_con->index,
                rvd_con->port, rvd_con->reserver->block_type);
            if (block == rvd_con->reserver)
            {
                debug ("remove from reserved connection list..\n");
                list_del (&rvd_con->list);
                kfree (rvd_con);
            }
        }
    }

    // if block use workfarm, disconnect and destory farm block
    farm = _bm_find_farm_in_workfarm_list (block_info);
    if (farm)
    {
        noti ("workfarm is find, so disconnect and destroy farm..\n");

        _bm_disconnect_block_locked (farm->farm_block->block_info, 0);
        _bm_disconnect_block_locked (block_info, 0xfe);
        _bm_disconnect_block_locked (block_info, 0xff);

        _bm_destroy_block_locked (farm->farm_block->block_info);

        list_del (&farm->list);
        kfree (farm);

        noti ("destroying workfarm is done.\n");
    }

    list_for_each_entry_safe (port_forward, next_p, &block->list_port_forward, list)
    {
        trace ("remove port forwarding list...\n");
        list_del (&port_forward->list);
        kfree (port_forward);
    }

    // destroy ports
    _bm_del_ports (block);
    // call desctructor
    if (block->block_impl->destructor)
        block->block_impl->destructor (block->block_info);
    mutex_lock (&bm_info->list_block_lock);
    kfree (block->block_info);
    list_del (&block->list);
    kfree (block);
    mutex_unlock (&bm_info->list_block_lock);

    ret = 0;
  exit:

    debug ("done\n");
    return ret;
}

/* @0xffffffc010c9f4b0 */
int bm_destroy_block (struct aud_block_info *block_info)
{
    struct aud_bm_info *bm_info = &_bm_info;
    int ret = -1;

    if (block_info == NULL)
    {
        error ("block_info is NULL!\n");
        return -1;
    }

    mutex_lock (&bm_info->bm_lock);
    ret = _bm_destroy_block_locked (block_info);
    mutex_unlock (&bm_info->bm_lock);

    return ret;
}

EXPORT_SYMBOL (bm_destroy_block);

/* @0xffffffc010ca20e0 */
bool bm_check_query_farm (struct aud_block_info * block_info,
    bool (*check_func) (struct aud_block_info * me, struct aud_block_info * other))
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *b;
    struct bm_list_block *block;
    struct bm_list_work_farm *farm;
    bool ret = false;
    bool need_load_balance = false;

    info ("called\n");

    if (aud_hal_get_number_of_dsp () != 4)
        return false;

    mutex_lock (&bm_info->bm_lock);

    // check that already farm exist and this is not for me.
    if (!list_empty (&bm_info->list_work_farm) && !_bm_find_farm_in_workfarm_list (block_info))
    {
        noti ("%s : farm is already created. don't make!!\n", block_info->block_type);
        goto exit;
    }

    block = _bm_find_block_by_block_info (block_info);
    if (!block)
    {
        error ("can't find block by block info!!! %s\n", block_info->block_type);
        goto exit;
    }

    // count number of each processors, set least processor.
    list_for_each_entry (b, &bm_info->list_block, list)
    {
        trace ("type  : %s_%d\n", b->block_type, b->index);
        if ((!b->load_balanced) && !strcmp (block->block_type, b->block_type)
            && (block->index != b->index))
        {
            info ("%s_%d in DSP%d\n", block->block_type, block->index, block->processor - 2);
            if (check_func (block_info, b->block_info))
            {
                need_load_balance = true;
                break;
            }
        }
    }

    // check.. if there is already work farm is connected..
    farm = _bm_find_farm_in_workfarm_list (block_info);
    if (farm)
    {
        info ("find connected workfarm...\n");
        if (need_load_balance == false)
        {
            info ("new codec don't need workerfarm.. so remove...\n");

            _bm_disconnect_block_locked (farm->farm_block->block_info, 0);
            _bm_disconnect_block_locked (block_info, 0xfe);
            _bm_disconnect_block_locked (block_info, 0xff);

            _bm_destroy_block_locked (farm->farm_block->block_info);

            list_del (&farm->list);
            kfree (farm);

            noti ("destroying workfarm is done.\n");
            goto exit;
        }
        else
        {
            info ("same...\n");
            ret = true;
            goto exit;
        }
    }

    // create farm block and connect with master block
    if (need_load_balance)
    {
        struct bm_list_block *farm_block;
        struct aud_block_info *farm_block_info;

        noti ("request farm...\n");

        // make farm...
        farm = (struct bm_list_work_farm *) kzalloc (sizeof (*farm), GFP_KERNEL);
        if (!farm)
        {
            error ("no mem\n");
            goto exit;
        }

        farm_block_info = _bm_create_block_locked ("farm", -1);
        if (!farm_block_info)
        {
            error ("farm block is not created!!!\n");
            kfree (farm);
            goto exit;
        }

        farm_block = _bm_find_block_by_block_info (farm_block_info);
        if (!farm_block)
        {
            _bm_destroy_block_locked (farm_block_info);
            kfree (farm);
            goto exit;
        }

        farm->master_block = block;
        farm->farm_block = farm_block;

        _bm_connect_block_locked (block_info, 0xfe, farm_block_info, 0);
        _bm_connect_block_locked (farm_block_info, 0, block_info, 0xfe);
        _bm_connect_block_locked (farm_block_info, 1, block_info, 0xff);

        farm_set_master_info (farm_block_info, block_info);

        list_add (&farm->list, &bm_info->list_work_farm);

        noti ("farm created done.\n");

        block->load_balanced = true;
        ret = true;
        goto exit;
    }

  exit:
    noti ("done.\n");
    mutex_unlock (&bm_info->bm_lock);
    return ret;
}

/* @0xffffffc010ca27e0 */
void bm_block_registry (struct audio_block_impl *block_impl)
{
    struct aud_bm_info *bm_info = &_bm_info;
    info ("%s : registry\n", block_impl->block_type);
    INIT_LIST_HEAD (&block_impl->list);
    list_add (&block_impl->list, &bm_info->list_block_impl);
}

/* @0xffffffc010c9d590 */
static int bm_dot (struct seq_file *m, void *data)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block = NULL;

    mutex_lock (&bm_info->bm_lock);

    noti ("dot!!!!!!!!!\n");
    /* *INDENT-OFF* */
    seq_printf (m, "digraph {\n");
    seq_printf (m, " rankdir=\"LR\";\n");
    seq_printf (m, " node [color=black, fillcolor=white, style=filled, shape=box, fontname=\"sans\"]\n");

    seq_printf (m, " subgraph cluster_proc_0 {\n");
    seq_printf (m, "  label=\"proc_%x\";\n", 0);
    seq_printf (m, " }\n");

    // block list
    list_for_each_entry (block, &bm_info->list_block, list)
    {
        struct bm_list_block_port *port;

        seq_printf (m, "  subgraph cluster_%s_0x%x {\n", block->block_type, block->index);
        seq_printf (m, "   label=\"%s%x\";\n", block->block_type, block->index);

        // input ports
        list_for_each_entry (port, &block->list_port_input, list)
        {
            seq_printf (m, "  %s_%x_in%x [label=\"in%x\", fillcolor=lightgrey];\n", block->block_type, block->index, port->module_port, port->module_port);
        }

        // output ports
        list_for_each_entry (port, &block->list_port_output, list)
        {
            seq_printf (m, "  %s_%x_out%x [label=\"out%x\", fillcolor=lightgrey];\n", block->block_type, block->index, port->module_port, port->module_port);
        }

        seq_printf (m, "  }\n");
    }

    // connection list
    list_for_each_entry (block, &bm_info->list_block, list)
    {
        struct bm_list_block_port *port;
        list_for_each_entry (port, &block->list_port_output, list)
        {
            struct bm_list_connected_port *c_port;
            list_for_each_entry (c_port, &port->list_connected_port, list)
            {
                struct bm_list_block_port *connected_port = c_port->connected_port;

                seq_printf (m, " %s_%x_out%x -> %s_%x_in%x [label=\"\"]\n",
                    block->block_type, block->index, port->module_port,
                    connected_port->owner->block_type, connected_port->owner->index, connected_port->module_port);
            }
        }
    }

    seq_printf (m, "}\n");
    /* *INDENT-ON* */

    mutex_unlock (&bm_info->bm_lock);

    return 0;
}

/* @0xffffffc010ca2880 */
int bm_init (void)
{
    struct aud_bm_info *bm_info = &_bm_info;
    noti ("initalize block manager [%d]\n", bm_info->init);
    if (bm_info->init == false)
    {
        mutex_init (&bm_info->bm_lock);
        mutex_init (&bm_info->list_block_lock);

        INIT_LIST_HEAD (&bm_info->list_block);
        INIT_LIST_HEAD (&bm_info->list_block_impl);
        INIT_LIST_HEAD (&bm_info->list_reserved_connection);
        INIT_LIST_HEAD (&bm_info->list_work_farm);
        farm_initializer ();
        input_initializer ();
        decoder_initializer ();
        renderer_initializer ();
        mixer_initializer ();
        se_initializer ();
        output_initializer ();
        aud_encoder_initializer ();
        aud_capture_initializer ();
        bm_info->last_index = BLOCK_INDEX_TEMP_NUMBER;
        bm_info->main_audio_index = -1;
        bm_info->init = true;
        bm_info->suspended = false;

        proc_create_data ("dot_block_connection", 0640, aproc_root, &aaudio_seq_show_fops, bm_dot);
    }

    return 0;
}

/* @0xffffffc010ca29d0 */
int bm_close (void)
{
    struct aud_bm_info *bm_info = &_bm_info;
    struct bm_list_block *block, *next;
    noti ("close block manager [%d]\n", bm_info->init);
    if (bm_info->init == true)
    {
        list_for_each_entry_safe (block, next, &bm_info->list_block, list)
        {
            bm_destroy_block (block->block_info);
        }
    }

    bm_info->init = false;
    return 0;
}
