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
#include <linux/kernel.h>
#include <linux/slab.h>

#include "kaimc.h"
#include "module_list.h"

#include "log.h"

logm_define (aud_pipeline, log_level_noti);
#define trace(fmt,args...)	logm_trace(aud_pipeline,fmt,##args)
#define debug(fmt,args...)	logm_debug(aud_pipeline,fmt,##args)
#define info(fmt,args...)	logm_info(aud_pipeline,fmt,##args)
#define noti(fmt,args...)	logm_noti(aud_pipeline,fmt,##args)
#define error(fmt,args...)	logm_error(aud_pipeline,fmt,##args)

struct element
{
    struct list_head list;

    unsigned int uid;
};

struct apipeline
{
    char *name;

    int preferred_proc;

    struct list_head element_list;
};

#define PIPE_NULL_CHECK(pipe, action) \
    if (pipe == NULL) \
    {\
        error ("pipe is null!!\n");\
        action;\
    }\

/* Inlined head/tail lookups in the shipped apipeline_connect. */
unsigned int apipeline_head (struct apipeline *pipe)
{
    struct element *e;

    PIPE_NULL_CHECK (pipe, return 0);

    if (list_empty (&pipe->element_list))
        return 0;

    e = list_first_entry (&pipe->element_list, struct element, list);
    return e->uid;
}

unsigned int apipeline_tail (struct apipeline *pipe)
{
    struct element *e;

    PIPE_NULL_CHECK (pipe, return 0);

    if (list_empty (&pipe->element_list))
        return 0;

    e = list_last_entry (&pipe->element_list, struct element, list);
    return e->uid;
}

/* bm_connect_block -> _bm_connect_block_locked -> this function ->
 * aimc_module_connect -> aud_inst_connect -> aud_modlist_connect_inst.
 * The DSP connector changes graph links, not clock state; FIRST is replayed
 * by DSP aud_module_chain_locked when subsequent data reaches a new input. */
/* @0xffffffc010c96770 */
unsigned int apipeline_connect (struct apipeline *pipe_out, int port_out, struct apipeline *pipe_in,
    int port_in)
{
    int uid_in, uid_out;

    uid_out = apipeline_tail (pipe_out);
    uid_in = apipeline_head (pipe_in);

    if (!uid_in || !uid_out)
    {
        error ("error in get uid of  out(%d) or in(%d)", uid_out, uid_in);
        return -1;
    }

    return aimc_module_connect (uid_out, port_out, uid_in, port_in);
}
