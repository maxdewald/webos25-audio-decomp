// SPDX-License-Identifier: GPL-2.0-only
/*
 * Restored from LG's GPL-2.0 module_list.c, copyright/license below preserved.
 * Binary confirmation: orig/boot/Image-5.4.268-329.decompressed, SHA256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Requested connection/ownership/teardown functions and their tagged helpers
 * match the published source's branches, field offsets, IMC payloads and logs.
 * Offline Capstone evidence: build/scratch/module-list-verify/ (not REA output).
 * Other published routines are retained as dependencies, not claimed verified.
 * No functional binary/source differences found within the verified scope.
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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <asm/pgtable.h>

#include "hma_alloc.h"

#include "kaimc.h"
#include "imc/aimc_internal.h"
#include "imc/cmd/module_cmd_connection.h"

#include "buffer/adec_buf.h"
#include "buffer/adec_buf_monitor.h"
#include "buffer/adec_inter_buf_writer.h"
#include "module/adec_module_list.h"

#include "imc/cmd/module_cmd_registry.h"
#include "imc/cmd/module_cmd_connector.h"
#include "imc/cmd/module_cmd_glue.h"

#include "module_list.h"
#include "proc.h"
#include "regs.h"
#include "audio_uimc.h"

#include "log.h"

logm_define (amodlist, log_level_noti);
#define trace(fmt,args...)	logm_trace(amodlist,fmt,##args)
#define debug(fmt,args...)	logm_debug(amodlist,fmt,##args)
#define info(fmt,args...)	logm_info(amodlist,fmt,##args)
#define noti(fmt,args...)	logm_noti(amodlist,fmt,##args)
#define error(fmt,args...)	logm_error(amodlist,fmt,##args)

logm_define (amlp, log_level_noti);
#define ptrace(fmt,args...)	logm_trace(amlp,fmt,##args)
#define pdebug(fmt,args...)	logm_debug(amlp,fmt,##args)
#define pinfo(fmt,args...)	logm_info(amlp,fmt,##args)
#define pnoti(fmt,args...)	logm_noti(amlp,fmt,##args)
#define perror(fmt,args...)	logm_error(amlp,fmt,##args)

static LIST_HEAD (processors);
static DEFINE_MUTEX (processors_lock);
static DEFINE_MUTEX (connection_lock);

struct aud_mod_owner
{
    struct list_head instances;
    struct mutex instances_lock;

#define FLAGS_HAVE_DEAD_INSTANCE	1
    unsigned int flags;
    wait_queue_head_t flags_wq;

    char name[32];
};

struct aud_mod_inst;
struct port;
struct jumper
{
    void *shared;
    phys_addr_t dsp_shared;
    unsigned int offset;
    unsigned int shared_size;
    unsigned int max_access_size;
    unsigned int aui_count;

    /* output */
    struct aud_mod_inst *output_glue;
    struct list_head jumper_list;
    struct port *output_port;

    /* input */
    struct aud_mod_inst *input_glue;
    struct port *input_port;
};

struct port
{
    /* list added at struct aud_mod_inst::{in,out}put_ports */
    struct list_head port_list;

    bool input;
    int index;

    struct list_head down_peers;
    struct list_head down_peer_list;
    struct port *up_peer;

    /* input port can have jumper to connect between different processors */
    struct jumper *input_jumper;
    struct list_head output_jumpers;

    struct aud_mod_inst *inst;

    struct aud_mod_inst *glue;
};

struct aud_mod_impl_port_buf_def
{
    int max_num_au;
    int max_access_size;
};
struct aud_impl_helper;
struct aud_mod_inst
{
    struct list_head owner_list;
    struct list_head impl_list;

    struct mutex lock;

    atomic_t users;

    unsigned int uid;

    struct list_head input_ports;
    struct list_head output_ports;

    struct aud_mod_owner *owner;
    struct aud_mod_impl *mod_impl;

    unsigned int ext_buf_id;

    const char *role;
    struct proc_dir_entry *proc_role;
    struct proc_dir_entry *proc;
    struct proc_dir_entry *proc_status_sym;
    struct proc_dir_entry *proc_dump;
    struct proc_dir_entry *proc_dump_port;
    struct aaudio_args_arg proc_dump_port_arg;
    struct aaudio_args_arg proc_prop_arg;
    struct aaudio_args_arg proc_cmd_arg;

    struct aud_impl_helper *helper;
    void *helper_priv;

    int num_mem_defs;
    struct aud_mod_impl_port_buf_def *mem_defs;
    char name[0];
};

struct aud_proc_info;
struct aud_mod_impl
{
    struct list_head mods_list;

    struct list_head instances;

    struct aud_proc_info *processor;
    struct proc_dir_entry *proc_entry;
    int impl_num;

    int alloc_count;
    int last_index;
    int rank;

    struct aaudio_args_arg proc_status_arg;

    /* send command to legacy manager modules */
    struct aaudio_args_arg proc_cmd_arg;

    struct mutex lock;

    int num_mem_defs;
    struct aud_mod_impl_port_buf_def *mem_defs;
    char name[];
};

struct aud_proc_info
{
    struct list_head proc_list;

    struct list_head mods;
    struct mutex mods_lock;

    int cur_impl_num;

    int proc_id;
    void *pid;
    struct proc_dir_entry *proc_entry;
    char name[32];

    bool send_async;
};

static int next_proc_id = ADEC_CORE_START_PROC;
module_param (next_proc_id, int, 0444);

static bool aproc_debug;
module_param (aproc_debug, bool, 0644);

static bool allow_nonsecure_connect = true;
module_param (allow_nonsecure_connect, bool, 0644);

static bool no_glue_on_inter_legacy = true;
module_param (no_glue_on_inter_legacy, bool, 0644);

static struct proc_dir_entry *proc_processors;
static struct proc_dir_entry *proc_instances;
static struct proc_dir_entry *proc_roles;

static struct
{
    int proc_id;
    const char *name;

    struct aud_proc_info *proc_info;
} predefined_procs[] =
{
    /* *INDENT-OFF* */
    { ADEC_CORE_DSP0, "dsp0",},
    { ADEC_CORE_DSP1, "dsp1",},
    { ADEC_CORE_DSP2, "dsp2",},
    { ADEC_CORE_DSP3, "dsp3",},
    /* *INDENT-ON* */
};

struct aud_impl_helper
{
    struct list_head list;

    const char *impl_name;

    void *(*create_pre) (const char *impl_name, uint32_t uid);
    int (*create_post) (void *priv, const char *impl_name, uint32_t uid);

    int (*destroy_pre) (void *priv, const char *impl_name, uint32_t uid);
    int (*destroy_post) (void *priv, const char *impl_name, uint32_t uid);
};

static LIST_HEAD (helper_list);

static struct aud_impl_helper *search_helper (const char *impl_name)
{
    struct aud_impl_helper *now;

    list_for_each_entry (now, &helper_list, list)
    {
        if (!strcmp (now->impl_name, impl_name))
            return now;
    }

    return NULL;
}

void *aud_impl_register_helper (const char *impl_name /* must not be freed after call */ ,
    void *(*create_pre) (const char *impl_name,
        uint32_t uid) /* returns helper priv for the instance */ ,
    int (*create_post) (void *priv, const char *impl_name, uint32_t uid),
    int (*destroy_pre) (void *priv, const char *impl_name, uint32_t uid),
    int (*destroy_post) (void *priv, const char *impl_name, uint32_t uid))
{
    struct aud_impl_helper *helper;

    if (search_helper (impl_name))
    {
        error ("impl, \"%s\", already registered.\n", impl_name);
        return NULL;
    }

    helper = kzalloc (sizeof (*helper), GFP_KERNEL);
    if (!helper)
    {
        error ("nomem for %s\n", impl_name);
        return NULL;
    }

    helper->impl_name = impl_name;
    helper->create_pre = create_pre;
    helper->create_post = create_post;
    helper->destroy_pre = destroy_pre;
    helper->destroy_post = destroy_post;

    list_add (&helper->list, &helper_list);

    return helper;
}

void aud_impl_unregister_helper (const char *impl_name)
{
    struct aud_impl_helper *helper;

    helper = search_helper (impl_name);
    if (!helper)
        error ("no such helper for %s\n", impl_name);
    else
    {
        list_del (&helper->list);
        kfree (helper);
    }
}

static int impl_status_write (void *arg, int argc, char **argv)
{
    struct aud_mod_impl *impl = arg;

    if (!strcmp (argv[0], "rank") && argv[1])
    {
        int rank = 0;

        if (kstrtoint (argv[1], 0, &rank) >= 0)
        {
            noti ("change rank of \"%s\" from %d to %d\n", impl->name, impl->rank, rank);
            impl->rank = rank;
        }
    }
    else
    {
        error ("unknown command string.\n");
        return -EINVAL;
    }

    return 0;
}

static int impl_status_read (void *arg, struct seq_file *m)
{
    struct aud_mod_impl *impl = arg;

#define seq_print_impl(v,f)	seq_printf (m, #v"="f"\n", impl->v)
    seq_print_impl (name, "%s");
    seq_print_impl (impl_num, "%d");
    seq_print_impl (alloc_count, "%d");
    seq_print_impl (rank, "%d");

    return 0;
}

static struct aud_mod_impl *register_impl (struct aud_proc_info *p, const char *name, int rank,
    int impl_num)
{
    struct aud_mod_impl *impl;
    int name_len;

    list_for_each_entry (impl, &p->mods, mods_list)
    {
        if (!strcmp (impl->name, name))
        {
            error ("have same implement(\"%s\") on proc(\"%s\")\n", name, p->name);
            return NULL;
        }
    }

    if (p->cur_impl_num == AUD_MODLIST_MODIMPL_MAX)
    {
        error ("too many implementations on processor %s\n", p->name);
        return NULL;
    }

    name_len = strlen (name);
    impl = kzalloc (sizeof (*impl) + name_len + 1, GFP_KERNEL);
    if (!impl)
    {
        error ("no mem for %s\n", name);
        return NULL;
    }

    strncpy (impl->name, name, name_len);
    impl->name[name_len] = 0;
    impl->processor = p;
    impl->rank = rank;
    impl->proc_entry = proc_mkdir (impl->name, p->proc_entry);
    impl->proc_status_arg.arg = impl;
    impl->proc_status_arg.callback = impl_status_write;
    impl->proc_status_arg.show = impl_status_read;
    proc_create_data ("status", 0640, impl->proc_entry, &aaudio_args_arg_fops,
        &impl->proc_status_arg);

    INIT_LIST_HEAD (&impl->instances);
    mutex_init (&impl->lock);

    if (impl_num < 0)
        impl->impl_num = ++p->cur_impl_num;
    else
        impl->impl_num = impl_num;
    list_add (&impl->mods_list, &p->mods);

    info ("processor %s, add %s, impl_num %x\n", p->name, impl->name, impl->impl_num);

    return impl;
}

struct aud_mod_impl *aud_modlist_register_impl (struct aud_proc_info *p, const char *name, int rank)
{
    struct aud_mod_impl *impl;

    mutex_lock (&p->mods_lock);
    impl = register_impl (p, name, rank, -1);
    mutex_unlock (&p->mods_lock);

    return impl;
}

/* @0xffffffc010c90a04: Publish a named processor directory; pid is an opaque
 * caller cookie (user_imc passes itself), not Linux task identity. */
static struct aud_proc_info *__printf (3, 4) register_processor (int proc_id, void *pid,
    const char *name, ...)
{
    struct aud_proc_info *p;
    va_list ap;

    p = kzalloc (sizeof (*p), GFP_KERNEL);
    if (!p)
    {
        error ("no mem for %p\n", pid);
        return NULL;
    }

    va_start (ap, name);
    vsnprintf (p->name, sizeof (p->name), name, ap);
    va_end (ap);

    p->pid = pid;
    p->proc_id = proc_id;
    p->proc_entry = proc_mkdir (p->name, proc_processors);
    /* reserve first 16 module implementation for CM, BM, ... */
    p->cur_impl_num = 0xf;
    p->send_async = false;
    INIT_LIST_HEAD (&p->mods);
    mutex_init (&p->mods_lock);

    noti ("new processor. %s, proc_id %x\n", p->name, p->proc_id);

    list_add_tail (&p->proc_list, &processors);

    return p;
}

/* @0xffffffc010c92c20: Allocate a free processor ID under processors_lock;
 * processor_%d identifies this registration, not an opener PID or renderer. */
struct aud_proc_info *aud_modlist_register_processor (void *pid, int *index)
{
    struct aud_proc_info *p;
    int i;

    mutex_lock (&processors_lock);
    /* search empty proc_id */
    for (i = 0; i < AUD_MODLIST_PROC_MAX; i++)
    {
        bool found_same = false;

        list_for_each_entry (p, &processors, proc_list)
        {
            if (next_proc_id == p->proc_id)
            {
                found_same = true;
                break;
            }
        }

        if (!found_same)
            break;

        next_proc_id++;
        if (next_proc_id > AUD_MODLIST_PROC_MAX)
            next_proc_id = ADEC_CORE_START_PROC;
    }
    if (i == AUD_MODLIST_PROC_MAX)
    {
        error ("too many processor registered. cannot select proc_id for %p\n", pid);
        mutex_unlock (&processors_lock);
        return NULL;
    }

    p = register_processor (next_proc_id, pid, "processor_%d", next_proc_id);
    if (p && index)
        *index = next_proc_id;
    mutex_unlock (&processors_lock);

    return p;
}

static void unregister_impl (struct aud_mod_impl *impl)
{
    struct aud_mod_inst *inst, *n;

    info ("processor %s(%d), remove implement %s\n", impl->processor->name,
        impl->processor->proc_id, impl->name);

    mutex_lock (&impl->lock);
    list_for_each_entry_safe (inst, n, &impl->instances, impl_list)
    {
        info ("remove instance %08x from implement %s\n", inst->uid, impl->name);
        list_del (&inst->impl_list);
        inst->impl_list.next = NULL;
        inst->impl_list.prev = NULL;

        mutex_lock (&inst->lock);
        if (inst->owner)
        {
            /* wake up owner to handle this dead instance */
            inst->owner->flags |= FLAGS_HAVE_DEAD_INSTANCE;
            wake_up_all (&inst->owner->flags_wq);
        }

        inst->mod_impl = NULL;
        proc_remove (inst->proc);
        proc_remove (inst->proc_status_sym);
        inst->proc = NULL;
        inst->proc_status_sym = NULL;
        mutex_unlock (&inst->lock);
    }
    mutex_unlock (&impl->lock);

    list_del (&impl->mods_list);
    remove_proc_subtree (impl->name, impl->processor->proc_entry);

    kfree (impl->mem_defs);
    kfree (impl);
}

/* @0xffffffc010c901e0: unregister_impl is inlined: detach implementations/instances, mark owning
 * owners dead and wake them, clear mod_impl/proc pointers, then free processor. */
static void unregister_processor (struct aud_proc_info *p)
{
    struct aud_mod_impl *m, *n;

    noti ("unregister processor %s\n", p->name);

    list_del (&p->proc_list);

    mutex_lock (&p->mods_lock);
    list_for_each_entry_safe (m, n, &p->mods, mods_list)
    {
        unregister_impl (m);
    }
    mutex_unlock (&p->mods_lock);

    remove_proc_entry (p->name, proc_processors);

    kfree (p);
}

/* @0xffffffc010c92d60: Serialize processor removal; instances survive detached for owner cleanup. */
int aud_modlist_unregister_processor (struct aud_proc_info *p)
{
    mutex_lock (&processors_lock);
    unregister_processor (p);
    mutex_unlock (&processors_lock);

    return 0;
}

struct aud_mod_owner *aud_modlist_new_ownerv (const char *name, va_list ap)
{
    struct aud_mod_owner *o;

    o = kzalloc (sizeof (*o), GFP_KERNEL);
    if (!o)
    {
        error ("no mem\n");
        return NULL;
    }

    vsnprintf (o->name, sizeof (o->name), name, ap);

    INIT_LIST_HEAD (&o->instances);
    mutex_init (&o->instances_lock);

    init_waitqueue_head (&o->flags_wq);

    info ("new owner %s\n", o->name);

    return o;
}

struct aud_mod_owner *__printf (1, 2) aud_modlist_new_owner (const char *name, ...)
{
    struct aud_mod_owner *o;
    va_list ap;

    va_start (ap, name);
    o = aud_modlist_new_ownerv (name, ap);
    va_end (ap);

    return o;
}

/* @0xffffffc010c90bd0: Remove the implementation link under processor/implementation locks. */
static void remove_from_processor (struct aud_mod_inst *inst)
{
    struct aud_proc_info *p;

    mutex_lock (&processors_lock);
    debug ("remove instance, %p, uid %08x\n", inst, inst->uid);
    list_for_each_entry (p, &processors, proc_list)
    {
        struct aud_mod_impl *impl;

        if (AUD_MODLIST_PROC (inst->uid) != p->proc_id)
            continue;

        list_for_each_entry (impl, &p->mods, mods_list)
        {
            if (!AUD_MODID_IS_INTERNAL (inst->uid)
                && AUD_MODLIST_MODIMPL (inst->uid) != impl->impl_num)
                continue;

            mutex_lock (&impl->lock);
            if (inst->impl_list.next)
            {
                list_del (&inst->impl_list);
                inst->impl_list.next = NULL;
                inst->impl_list.prev = NULL;
            }
            mutex_unlock (&impl->lock);
            info ("remove instance, %p, from processor %s\n", inst, p->name);
            mutex_unlock (&processors_lock);

            return;
        }
    }

    info ("inst, %x, is not linked at anywhere.\n", inst->uid);
    mutex_unlock (&processors_lock);
}

static void get_instance (struct aud_mod_inst *inst)
{
    atomic_inc (&inst->users);
}

/* @0xffffffc010c939a0: Last reference disconnects input then output before helper/registry destruction.
 * A dead implementation suppresses registry traffic, not graph cleanup. */
static void put_instance (struct aud_mod_inst *inst)
{
    if (!atomic_dec_and_test (&inst->users))
        return;

    info ("destroy instance. %p(%s)\n", inst, inst->name);

    /* check input/output port connection */
    if (!list_empty (&inst->input_ports))
    {
        struct port *port, *next_port;

        info ("%s: input port still active. disconnect.\n", inst->name);
        list_for_each_entry_safe (port, next_port, &inst->input_ports, port_list)
            aud_modlist_disconnect_inst_input (NULL, inst, port->index);
    }
    if (!list_empty (&inst->output_ports))
    {
        struct port *port, *next_port;

        info ("%s: output port still active. disconnect.\n", inst->name);
        list_for_each_entry_safe (port, next_port, &inst->output_ports, port_list)
            aud_modlist_disconnect_inst_output (NULL, inst, port->index);
    }

    if (inst->helper && inst->helper->destroy_pre)
        inst->helper->destroy_pre (inst->helper_priv, inst->helper->impl_name, inst->uid);

    /* send IMC to destroy the instance */
    if (inst->mod_impl)
    {
        RegistryCmdDestroyModule param = { };
        struct aud_mod_impl *impl = inst->mod_impl;

        param.uid = inst->uid;

        aimc_send_command (NULL, REGISTRY_CMD_DESTROY_MODULE, 0,
            AUD_MODID_REGISTRY (impl->processor->proc_id), &param, sizeof (param));

        inst->mod_impl->alloc_count--;
    }

    if (inst->helper && inst->helper->destroy_post)
        inst->helper->destroy_post (inst->helper_priv, inst->helper->impl_name, inst->uid);

    if (inst->role)
        aud_modlist_role_set (inst, NULL);

    /* remove from processor list to prevent deadlock */
    remove_from_processor (inst);

    if (inst->owner)
        list_del (&inst->owner_list);
    proc_remove (inst->proc_status_sym);
    proc_remove (inst->proc);

    kfree (inst);
}

/* @0xffffffc010c93c80: Detach/null each owner link under the owner lock, drop that lock around
 * put_instance, and free the owner only after its instance list is empty. */
void aud_modlist_del_owner (struct aud_mod_owner *o)
{
    struct aud_mod_inst *inst;

    info ("delete owner %s\n", o->name);

    mutex_lock (&o->instances_lock);
    while ((inst = list_first_entry_or_null (&o->instances, struct aud_mod_inst, owner_list)))
    {
        inst->owner = NULL;
        list_del (&inst->owner_list);
        mutex_unlock (&o->instances_lock);

        put_instance (inst);

        mutex_lock (&o->instances_lock);
    }
    mutex_unlock (&o->instances_lock);

    kfree (o);
}

/* lock (struct aud_mod_impl::lock) when success */
static struct aud_mod_impl *search_modimpl (const char *name, int pref_proc)
{
    int selected_rank = INT_MIN;
    struct aud_mod_impl *selected_impl;
    struct aud_proc_info *p;

    mutex_lock (&processors_lock);
    selected_impl = NULL;
    list_for_each_entry_reverse (p, &processors, proc_list)
    {
        struct aud_mod_impl *impl;

        if (pref_proc > 0 && p->proc_id != pref_proc)
            continue;

        list_for_each_entry (impl, &p->mods, mods_list)
        {
            if (!strcmp (impl->name, name) && selected_rank < impl->rank)
            {
                selected_impl = impl;
                selected_rank = impl->rank;
                break;
            }
        }
    }

    if (selected_impl)
        mutex_lock (&selected_impl->lock);
    mutex_unlock (&processors_lock);

    return selected_impl;
}

int aud_modlist_register_impl_port_buf_def (struct aud_proc_info *p,
    const char *name, int def_num, int max_num_au, int max_access_size)
{
    struct aud_mod_impl *impl;

    mutex_lock (&p->mods_lock);
    impl = search_modimpl (name, p->proc_id);
    if (!impl)
    {
        error ("no such implementation on %d. %s\n", p->proc_id, name);
        mutex_unlock (&p->mods_lock);
        return -EINVAL;
    }

    if (impl->mem_defs == NULL)
    {
        impl->mem_defs =
            kzalloc (sizeof (struct aud_mod_impl_port_buf_def) * (def_num + 1), GFP_KERNEL);
        if (impl->mem_defs == NULL)
        {
            error ("no mem\n");
            mutex_unlock (&p->mods_lock);
            return -ENOMEM;
        }
        impl->num_mem_defs = def_num + 1;
    }
    impl->mem_defs[def_num].max_num_au = max_num_au;
    impl->mem_defs[def_num].max_access_size = max_access_size;

    info ("%dth mem def for %s on %d, n_au %d, max_size %d(0x%x)", def_num, name, p->proc_id,
        max_num_au, max_access_size, max_access_size);

    mutex_unlock (&impl->lock);
    mutex_unlock (&p->mods_lock);

    return 0;
}

/* @0xffffffc010c8f934: Passive status contains only name and UID, not owner,
 * role, opener identity or DSP state. Unlike lstatus/prop, it sends no IMC. */
static int status_show (struct seq_file *m, void *data)
{
    struct aud_mod_inst *inst = m->private;

#define seq_print_aud(v,f)	seq_printf (m, #v"="f"\n", inst->v)
    seq_print_aud (name, "%s");
    seq_print_aud (uid, "0x%08x");

    return 0;
}

/* Inlined in proc_inst_cmd @0xffffffc010c92630: command then u32 base-0 words,
 * not strings/buffer syntax. Storage is 16 bytes/word; wire length is 4/word.
 * The command's low-16 size bits are not checked against that wire length. */
static int send_cmd (const char *name, unsigned int uid, int argc, char **argv)
{
    unsigned int cmd;
    unsigned int *param;
    unsigned int param_len;
    int ret;
    int i;

    if (argc < 1)
    {
        error ("no command for module %s.\n", name);
        return -EINVAL;
    }

    ret = kstrtouint (argv[0], 0, &cmd);
    if (ret < 0)
    {
        error ("wrong cmd. %s. %d\n", argv[0], ret);
        return ret;
    }
    argv++;
    argc--;

    param_len = argc * sizeof (unsigned int);
    param = kmalloc (sizeof (param[0]) * param_len, GFP_KERNEL);
    if (!param)
        return -ENOMEM;

    info ("send 0x%x to %s\n", cmd, name);
    for (i = 0; i < argc; i++)
    {
        ret = kstrtouint (argv[i], 0, &param[i]);
        if (ret < 0)
        {
            error ("wrong arg[%d]. %s, %d\n", i, argv[i], ret);
            kfree (param);
            return ret;
        }
        info ("arg[%d] = 0x%08x\n", i, param[i]);
    }

    aimc_send_command (NULL, cmd, 0, uid, param, param_len);
    kfree (param);

    return 0;
}

/* Shipped @0xffffffc010c92630: source=0, async send, then free; ignores send result.
 * IPC copies before return and successful sends are FIFO on this DSP route.
 * Parse/allocation failures return before sending; no FIRST/data completion barrier. */
static int proc_inst_cmd (void *arg, int argc, char **argv)
{
    struct aud_mod_inst *inst = arg;

    return send_cmd (inst->name, inst->uid, argc, argv);
}

/* Shipped @0xffffffc010c91dc0: 0x10180004, source=0, instance UID. Payload is
 * LE32 strlen("name=value"), then those bytes plus NUL: total len+5.
 * Only argv[0..1] are used verbatim; missing/nomem fail before the async send.
 * Transport errors are ignored, so success does not establish DSP application.
 * IPC_WriteCommand @ffffffc010d0c9f0 copies under its ring lock before return;
 * DSP IRQ @60079c2c / consumer @60079974 preserve successfully queued order.
 * Neither callback locks the instance or waits for FIRST/data processing. */
static int proc_inst_prop (void *arg, int argc, char **argv)
{
    struct aud_mod_inst *inst = arg;
    ImcCmdSetProp *param;
    int len;

    if (!argv[0] || !argv[1])
    {
        error ("no param.\n");
        return -EINVAL;
    }

    len = snprintf (NULL, 0, "%s=%s", argv[0], argv[1]);
    param = kmalloc (sizeof (*param) + len + 1, GFP_KERNEL);
    if (!param)
    {
        error ("no mem. %d, %s, %s\n", len, argv[0], argv[1]);
        return -ENOMEM;
    }

    param->len = len;
    snprintf (param->str, len + 1, "%s=%s", argv[0], argv[1]);
    pnoti ("set property \"%s\" to %s\n", param->str, inst->name);

    aimc_send_command (NULL, ADEC_CMD_SET_PROP, 0, inst->uid, param, sizeof (*param) + len + 1);

    kfree (param);

    return 0;
}

static int status_open (struct inode *inode, struct file *file)
{
    struct aud_mod_inst *inst = PDE_DATA (inode);
    int ret;

    ret = single_open (file, status_show, inst);
    if (ret < 0)
        return ret;

    ret = mutex_lock_interruptible (&inst->lock);
    if (ret < 0)
    {
        error ("cannot get lock. %d\n", ret);
        single_release (inode, file);
        return ret;
    }

    return ret;
}

static int status_release (struct inode *inode, struct file *file)
{
    struct aud_mod_inst *inst = PDE_DATA (inode);
    int ret;

    mutex_unlock (&inst->lock);

    ret = single_release (inode, file);

    return ret;
}

static const struct file_operations inst_status_fops = {
    .open = status_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = status_release,
};

static int dump_open (struct inode *inode, struct file *file)
{
    struct aud_mod_inst *inst = PDE_DATA (inode);
    const char *filename = file->f_path.dentry->d_iname;
    long port;
    int ret;

    get_instance (inst);
    ret = kstrtoul (filename, 10, &port);
    if (ret < 0)
    {
        perror ("wrong filename. %s\n", filename);
        return ret;
    }
    pinfo ("dump %s output port %s(%ld)\n", inst->name, filename, port);

    /* TODO: create dump module and connect with "inst" */

    return 0;
}

static int dump_release (struct inode *inode, struct file *file)
{
    struct aud_mod_inst *inst = PDE_DATA (inode);

    /* TODO: disconnect with dump and "inst" module and destroy dump module */

    put_instance (inst);

    return 0;
}

static ssize_t dump_read (struct file *file, char __user * data, size_t size, loff_t * off)
{
    /* TODO: read data from dump module */

    return 0;
}

static const struct file_operations inst_dump_fops = {
    .open = dump_open,
    .read = dump_read,
    .release = dump_release,
};

static int proc_inst_dump_port (void *arg, int argc, char **argv)
{
    struct aud_mod_inst *inst = arg;
    long port;
    char *filename;
    int ret;

    if (!argv[0])
        return -EINVAL;

    port = 0;
    ret = kstrtoul (argv[0], 0, &port);
    if (ret < 0)
    {
        perror ("wrong port. %s\n", argv[0]);
        return ret;
    }
    filename = kasprintf (GFP_KERNEL, "%ld", port);
    if (!filename)
        return -ENOMEM;

    info ("make port %s\n", filename);
    proc_create_data (filename, 0440, inst->proc_dump, &inst_dump_fops, inst);

    kfree (filename);

    return 0;
}

static int inst_prop_read_open (struct inode *inode, struct file *file)
{
    struct aaudio_args_arg *arg = PDE_DATA (inode);
    struct aud_mod_inst *inst = arg->arg;
    char moduleid[16];

    snprintf (moduleid, sizeof (moduleid), "0x%08x", inst->uid);

    info ("read module property from %s\n", inst->name);
    return aproc_lc_trigger (inst->mod_impl->processor->proc_id,
        "audreg_print_property", moduleid, NULL);
}

static const struct file_operations inst_prop_read_fops = {
    .open = inst_prop_read_open,
    .read = aproc_lc_read,
    .release = aproc_lc_release,
};

static int inst_lstate_open (struct inode *inode, struct file *file)
{
    struct aud_mod_inst *inst = (void *) PDE_DATA (inode);
    char moduleid[16] /* 0x12345678 */ ;

    snprintf (moduleid, sizeof (moduleid), "0x%08x", inst->uid);
    info ("send status imc to %s(0x%08x)\n", inst->name, inst->uid);
    return aproc_lc_trigger (inst->mod_impl->processor->proc_id, "debug_aimc2", "lstatus", moduleid,
        NULL);
}

struct file_operations inst_lstate_fops = {
    .open = inst_lstate_open,
    .read = aproc_lc_read,
    .release = aproc_lc_release,
};

static struct aud_mod_inst *create_inst_on_sync (struct aud_mod_impl *impl,
    struct aud_mod_owner *o, bool sync)
{
    struct aud_mod_inst *inst;
    int index;
    int name_len;
    char proc_name[9];          /* string length for 32bit hexa uid */
    char *sym_dest;

    info ("create instance for %s\n", impl->name);

    /* search empty index */
    index = impl->last_index + 1;
    while (index != impl->last_index)
    {
        bool found_same;

        if (index == AUD_MODLIST_INDEX_MAX + 1)
            index = 1;

        found_same = false;
        list_for_each_entry (inst, &impl->instances, impl_list)
        {
            if (AUD_MODLIST_MODINDEX (inst->uid) == index)
            {
                found_same = true;
                break;
            }
        }

        if (!found_same)
            break;

        index++;
    }
    if (index == impl->last_index)
    {
        error ("too many instances for %s at %s\n", impl->name, impl->processor->name);
        return NULL;
    }

    impl->last_index = index;

    /* bypass_02030001 */
    name_len = strlen (impl->name) + 10;

    inst = kzalloc (sizeof (*inst) + name_len, GFP_KERNEL);
    if (!inst)
    {
        error ("no mem for %s\n", impl->name);
        return NULL;
    }

    inst->uid = AUD_MODLIST_UID (impl->processor->proc_id, impl->impl_num, index);
    inst->helper = search_helper (impl->name);
    if (inst->helper && inst->helper->create_pre)
    {
        inst->helper_priv = inst->helper->create_pre (inst->helper->impl_name, inst->uid);
        if (!inst->helper_priv)
        {
            error ("helper failed for \"%s\"\n", impl->name);
            kfree (inst);
            return NULL;
        }
    }

    impl->alloc_count++;

    atomic_set (&inst->users, 1);
    inst->mod_impl = impl;
    inst->owner = o;
    mutex_init (&inst->lock);
    INIT_LIST_HEAD (&inst->input_ports);
    INIT_LIST_HEAD (&inst->output_ports);
    snprintf (inst->name, name_len, "%s_%08x", impl->name, inst->uid);
    snprintf (proc_name, sizeof (proc_name), "%08x", inst->uid);

    list_add (&inst->impl_list, &impl->instances);
    inst->proc = proc_mkdir (proc_name, impl->proc_entry);
    inst->proc_cmd_arg.callback = proc_inst_cmd;
    inst->proc_cmd_arg.arg = inst;
    proc_create_data ("cmd", 0220, inst->proc, &aaudio_args_arg_fops, &inst->proc_cmd_arg);
    inst->proc_prop_arg.callback = proc_inst_prop;
    inst->proc_prop_arg.arg = inst;
    inst->proc_prop_arg.read_fops = &inst_prop_read_fops;
    proc_create_data ("prop", 0640, inst->proc, &aaudio_args_arg_fops, &inst->proc_prop_arg);
    proc_create_data ("status", 0440, inst->proc, &inst_status_fops, inst);
    proc_create_data ("lstatus", 0440, inst->proc, &inst_lstate_fops, inst);
    sym_dest = kasprintf (GFP_KERNEL, "../processors/%s/%s/%08x/",
        impl->processor->name, impl->name, inst->uid);
    if (sym_dest)
    {
        inst->proc_status_sym = proc_symlink (proc_name, proc_instances, sym_dest);
        info ("symlink %s -> %s\n", proc_name, sym_dest);
        kfree (sym_dest);
    }
    if (aproc_debug)
    {
        inst->proc_dump = proc_mkdir ("dump", inst->proc);
        inst->proc_dump_port_arg.callback = proc_inst_dump_port;
        inst->proc_dump_port_arg.arg = inst;
        inst->proc_dump_port =
            proc_create_data ("port", 0644, inst->proc_dump, &aaudio_args_arg_fops,
            &inst->proc_dump_port_arg);
    }

    if (impl->impl_num != 0)
    {
        /* send IMC to connection manager or registry */
        RegistryCmdCreateModule create_param = { };

        strncpy (create_param.name, impl->name, sizeof (create_param.name));
        create_param.name[sizeof (create_param.name) - 1] = 0;
        create_param.uid = inst->uid;

        info ("create by registry\n");
        if (sync)
            aimc_send_command_sync (NULL, REGISTRY_CMD_CREATE_MODULE, 0,
                AUD_MODID_REGISTRY (impl->processor->proc_id),
                &create_param, sizeof (create_param));
        else
            aimc_send_command (NULL, REGISTRY_CMD_CREATE_MODULE, 0,
                AUD_MODID_REGISTRY (impl->processor->proc_id),
                &create_param, sizeof (create_param));
    }

    if (inst->helper && inst->helper->create_post)
        inst->helper->create_post (inst->helper_priv, inst->helper->impl_name, inst->uid);

    if (o)
    {
        mutex_lock (&o->instances_lock);
        list_add (&inst->owner_list, &o->instances);
        mutex_unlock (&o->instances_lock);
    }

    info ("instance created. %08x\n", inst->uid);

    return inst;
}

static struct aud_mod_inst *create_inst_on (struct aud_mod_impl *impl, struct aud_mod_owner *o)
{
    return create_inst_on_sync (impl, o, false);
}

struct aud_mod_inst *aud_modlist_create_inst (struct aud_mod_owner *o,
    const char *impl_name, int preferred_processor, bool sync)
{
    struct aud_mod_inst *inst;
    struct aud_mod_impl *impl;

    info ("create instance for %s on %d\n", impl_name, preferred_processor);

    /* search_modimpl() will mutex_lock(&impl->lock) when success */
    impl = search_modimpl (impl_name, preferred_processor);
    if (!impl)
    {
        error ("no such implementation on %d. %s\n", preferred_processor, impl_name);
        return NULL;
    }

    inst = create_inst_on_sync (impl, o, sync);
    mutex_unlock (&impl->lock);
    if (!inst)
    {
        error ("cannot create instance.\n");
        return NULL;
    }

    if (impl->mem_defs != NULL)
    {
        inst->mem_defs =
            kzalloc (sizeof (struct aud_mod_impl_port_buf_def) * impl->num_mem_defs, GFP_KERNEL);
        if (!inst->mem_defs)
        {
            error ("no mem\n");
            return NULL;
        }
        inst->num_mem_defs = impl->num_mem_defs;
        memcpy (inst->mem_defs, impl->mem_defs,
            sizeof (struct aud_mod_impl_port_buf_def) * impl->num_mem_defs);
    }
    info ("new instance from %s:%s, uid %x\n", impl->processor->name, impl->name, inst->uid);

    return inst;
}

void aud_modlist_destroy_inst (struct aud_mod_inst *inst)
{
    struct mutex *owner_lock;

    info ("destroy instance %x\n", inst->uid);

    if (inst->mem_defs != NULL)
    {
        kfree (inst->mem_defs);
        inst->mem_defs = NULL;
    }

    owner_lock = &inst->owner->instances_lock;
    mutex_lock (owner_lock);
    inst->owner = NULL;
    list_del (&inst->owner_list);
    mutex_unlock (owner_lock);

    put_instance (inst);
}

/* @0xffffffc010c933a0: Publish roles/<label> -> processors/<proc>/<impl>/<uid>.
 * This caller-supplied alias is not ownership; allocation/symlink failure is
 * not propagated. Clearing frees the old role but does not clear its pointer. */
int aud_modlist_role_set (struct aud_mod_inst *inst, const char *role)
{
    info ("set role \"%s\" to instance \"%s\"\n", role, inst->name);

    if (inst->role)
    {
        proc_remove (inst->proc_role);
        inst->proc_role = NULL;
        kfree_const (inst->role);
    }

    if (role)
    {
        inst->role = kstrdup_const (role, GFP_KERNEL);
        if (!inst->role)
            error ("no mem for role %s, inst %s\n", role, inst->name);
        else
        {
            char *sym_dest;
            struct aud_mod_impl *impl = inst->mod_impl;

            sym_dest = kasprintf (GFP_KERNEL, "../processors/%s/%s/%08x",
                impl->processor->name, impl->name, inst->uid);
            if (sym_dest)
            {
                inst->proc_role = proc_symlink (inst->role, proc_roles, sym_dest);
                kfree (sym_dest);
            }
        }
    }

    return 0;
}

struct aud_mod_inst *aud_modlist_search (struct aud_mod_owner *o, const char *impl_name, int index)
{
    struct aud_mod_inst *inst;

    mutex_lock (&o->instances_lock);
    list_for_each_entry (inst, &o->instances, owner_list)
    {
        mutex_lock (&inst->lock);
        if (inst->mod_impl && !strcmp (inst->mod_impl->name, impl_name))
        {
            if (index == 0)
            {
                mutex_unlock (&inst->lock);
                mutex_unlock (&o->instances_lock);
                return inst;
            }
            index--;
        }
        mutex_unlock (&inst->lock);
    }
    mutex_unlock (&o->instances_lock);

    error ("unknown instance. %s, index %d\n", impl_name, index);
    return NULL;
}

static struct port *get_input_port (struct aud_mod_inst *inst, int index)
{
    struct port *p;

    list_for_each_entry (p, &inst->input_ports, port_list)
    {
        if (p->index == index)
            return p;
    }

    return NULL;
}

static struct port *get_output_port (struct aud_mod_inst *inst, int index)
{
    struct port *p;

    list_for_each_entry (p, &inst->output_ports, port_list)
    {
        if (p->index == index)
            return p;
    }

    return NULL;
}

/* @0xffffffc010c904a0: Shipped as .isra.0: async disconnect if output implementation lives, then
 * unlink/free input and free output only after its last downstream disappears. */
static int disconnect_in_processor (struct aud_mod_inst *inst_out, struct port *port_out,
    struct aud_mod_inst *inst_in, struct port *port_in)
{
    if (!port_out || !port_in)
    {
        if (!port_out)
            error ("port_out is null\n");
        if (!port_in)
            error ("port_in is null\n");
        return -EINVAL;
    }

    info ("disconnect %s:%d -> %s:%d\n",
        inst_out->name, port_out->index, inst_in->name, port_in->index);

    /* send IMC */
    /* new connection */
    mutex_lock (&inst_out->lock);
    if (inst_out->mod_impl)
    {
        ConnectorCmdDisconnect param = { };

        param.output_module_id = inst_out->uid;
        param.output_module_port = port_out->index;
        param.input_module_id = inst_in->uid;
        param.input_module_port = port_in->index;

        aimc_send_command (NULL, CONNECTOR_CMD_DISCONNECT, 0,
            AUD_MODID_CONNECTOR (inst_out->mod_impl->processor->proc_id), &param, sizeof (param));
    }
    mutex_unlock (&inst_out->lock);

    /* input port */
    list_del (&port_in->down_peer_list);
    list_del (&port_in->port_list);
    kfree (port_in);

    /* output port */
    if (list_empty (&port_out->down_peers))
    {
        list_del (&port_out->port_list);
        kfree (port_out);
    }

    return 0;
}

/* @0xffffffc010c916c0: One upstream per input, output fan-out; link before implementation checks, no rollback.
 * Connector send result is ignored; sync only selects the send function. */
static int connect_in_processor (struct aud_mod_inst *inst_out, int port_out_index,
    struct aud_mod_inst *inst_in, int port_in_index, bool sync)
{
    struct port *port_out, *port_in;

    info ("connect %s:%d -> %s:%d\n", inst_out->name, port_out_index, inst_in->name, port_in_index);

    /* input port */
    if (get_input_port (inst_in, port_in_index))
    {
        error ("%s: already have input port. %d\n", inst_in->name, port_in_index);
        return -EINVAL;
    }

    port_in = kzalloc (sizeof (*port_in), GFP_KERNEL);
    if (!port_in)
    {
        error ("no mem\n");
        return -ENOMEM;
    }
    port_in->inst = inst_in;
    port_in->index = port_in_index;

    /* output port */
    port_out = get_output_port (inst_out, port_out_index);
    if (!port_out)
    {
        port_out = kzalloc (sizeof (*port_out), GFP_KERNEL);
        if (!port_out)
        {
            error ("no mem\n");
            kfree (port_in);
            return -ENOMEM;
        }

        INIT_LIST_HEAD (&port_out->down_peers);
        INIT_LIST_HEAD (&port_out->output_jumpers);
        port_out->inst = inst_out;
        port_out->index = port_out_index;

        list_add (&port_out->port_list, &inst_out->output_ports);
    }

    /* */
    list_add (&port_in->port_list, &inst_in->input_ports);

    port_in->up_peer = port_out;
    list_add (&port_in->down_peer_list, &port_out->down_peers);

    {
        ConnectorCmdConnect connect_param = { };

        if (inst_out->mod_impl == NULL)
        {
            error ("mod_impl is null\n");
            return -EINVAL;
        }

        if (inst_out->mod_impl->processor == NULL)
        {
            error ("processor is null\n");
            return -EINVAL;
        }

        connect_param.output_module_id = inst_out->uid;
        connect_param.output_module_port = port_out_index;
        connect_param.input_module_id = inst_in->uid;
        connect_param.input_module_port = port_in_index;

        info ("connect by connector..\n");
        if (sync)
            aimc_send_command_sync (NULL, CONNECTOR_CMD_CONNECT, 0,
                AUD_MODID_CONNECTOR (inst_out->mod_impl->processor->proc_id), &connect_param,
                sizeof (connect_param));
        else
            aimc_send_command (NULL, CONNECTOR_CMD_CONNECT, 0,
                AUD_MODID_CONNECTOR (inst_out->mod_impl->processor->proc_id), &connect_param,
                sizeof (connect_param));
    }

    return 0;
}

static unsigned int shared_size = 0x40000;
module_param (shared_size, uint, 0644);

static unsigned int max_access_size = 0x8000;
module_param (max_access_size, uint, 0644);

static unsigned int aui_count = 0x2;
module_param (aui_count, uint, 0644);

static void cache_invalidate (void *addr, unsigned int length, struct _ADEC_BUF_T *buf)
{
    return;
}

static void cache_writeback (void *addr, unsigned int length, struct _ADEC_BUF_T *buf)
{
    wmb ();
    return;
}

static unsigned char *mem_translator (unsigned char *addr, struct _ADEC_BUF_T *buf)
{
    return addr;
}

/* @0xffffffc010c91a90: Allocate DSP-backed or vmalloc shared storage and initialize the common buffer. */
static int initialize_shbuf (struct aud_mod_inst *inst_out, struct aud_mod_inst *inst_in,
    struct jumper *j)
{
    unsigned int imax_access_size = max_access_size;
    unsigned int iaui_count = aui_count;
    unsigned int ishared_size = shared_size;

    /* allocate shared buffer */
    if (inst_out->mod_impl->processor->proc_id == ADEC_CORE_DSP0 ||
        inst_out->mod_impl->processor->proc_id == ADEC_CORE_DSP1 ||
        inst_out->mod_impl->processor->proc_id == ADEC_CORE_DSP2 ||
        inst_out->mod_impl->processor->proc_id == ADEC_CORE_DSP3 ||
        inst_in->mod_impl->processor->proc_id == ADEC_CORE_DSP0 ||
        inst_in->mod_impl->processor->proc_id == ADEC_CORE_DSP1 ||
        inst_in->mod_impl->processor->proc_id == ADEC_CORE_DSP2 ||
        inst_in->mod_impl->processor->proc_id == ADEC_CORE_DSP3)
    {
        j->dsp_shared = hma_alloc_user ("audio_buffer", ishared_size, 0x1000, inst_out->name);
        if (!j->dsp_shared)
        {
            error ("no mem %d\n", ishared_size);
            return -ENOMEM;
        }
        j->shared = hma_map (j->dsp_shared, ishared_size);
        if (!j->shared)
        {
            error ("no vmap %pap, %d\n", &j->dsp_shared, ishared_size);
            hma_free ("audio_buffer", j->dsp_shared);
            return -ENOMEM;
        }

        j->offset = (unsigned long) j->dsp_shared - aud_base_address () + 0x10000000;
        info ("allocate physical shared buffer. dsp_shared %pap, offset %x\n", &j->dsp_shared,
            j->offset);
    }
    else
    {
        j->shared = vmalloc_user (ishared_size);
        if (!j->shared)
        {
            error ("no vmalloc %d\n", ishared_size);
            return -ENOMEM;
        }

        j->offset = (unsigned long) j->shared - VMALLOC_START + UIMC_VMALLOC_MMAP_OFFSET;
        info ("allocate shared buffer. shared %p, offset %x\n", j->shared, j->offset);
    }
    j->shared_size = ishared_size;
    j->max_access_size = imax_access_size;
    j->aui_count = iaui_count;

    /* create common buffer */
    {
        unsigned int common_size;
        ADEC_BUF_T buf = { };
        struct _ADEC_PROCESSOR_ADAP_IF_T adap;

        adap.pfnInvalidate = cache_invalidate;
        adap.pfnWriteback = cache_writeback;
        adap.pfnAddrTrans = mem_translator;

        common_size = CommonBuffer_GetStructSize (iaui_count);
        buf.commonBuf = j->shared;
        buf.data = j->shared + common_size;
        buf.adap = &adap;

        CommonBuffer_Create (ishared_size - common_size, imax_access_size, iaui_count, &buf);
    }

    if (j->dsp_shared)
    {
        /* dont need shared memory mapping anymore */
        hma_unmap (j->shared);
        j->shared = NULL;
    }

    return 0;
}

/* @0xffffffc010c90dc0: Destroy the ownerless glue, optionally waiting for its registry command. */
static void destroy_glue_sync (struct aud_mod_inst *glue, bool sync)
{
    struct aud_mod_impl *impl = glue->mod_impl;

    debug ("destroy glue %08x\n", glue->uid);

    /* check input/output port connection */
    if (!list_empty (&glue->input_ports))
    {
        struct port *port;

        error ("%s: input port still active. disconnect.\n", glue->name);
        list_for_each_entry (port, &glue->input_ports, port_list)
            error ("%s: input%d\n", glue->name, port->index);
    }
    if (!list_empty (&glue->output_ports))
    {
        struct port *port;

        error ("%s: output port still active. disconnect.\n", glue->name);
        list_for_each_entry (port, &glue->output_ports, port_list)
            error ("%s: output%d\n", glue->name, port->index);
    }

    if (impl)
    {
        RegistryCmdDestroyModule param = { };
        int proc_id = AUD_MODLIST_PROC (glue->uid);

        param.uid = glue->uid;

        if (sync)
            aimc_send_command_sync (NULL, REGISTRY_CMD_DESTROY_MODULE, 0,
                AUD_MODID_REGISTRY (proc_id), &param, sizeof (param));
        else
            aimc_send_command (NULL, REGISTRY_CMD_DESTROY_MODULE, 0,
                AUD_MODID_REGISTRY (proc_id), &param, sizeof (param));

        impl->alloc_count--;
        remove_from_processor (glue);
    }

    proc_remove (glue->proc);
    proc_remove (glue->proc_status_sym);

    kfree (glue);
}

static inline void destroy_glue (struct aud_mod_inst *glue)
{
    if (glue->mem_defs != NULL)
    {
        kfree (glue->mem_defs);
        glue->mem_defs = NULL;
    }
    destroy_glue_sync (glue, false);
}

static int jumper_inter_output_glue (struct jumper *j, struct aud_mod_inst *inst_out,
    int port_out_index)
{
    struct aud_mod_impl *impl;

    impl = search_modimpl ("gluepw", AUD_MODID_PROC (inst_out->uid));
    if (!impl)
    {
        error ("no glue on processor %d\n", AUD_MODID_PROC (inst_out->uid));
        return -1;
    }
    j->output_glue = create_inst_on (impl, NULL);
    mutex_unlock (&impl->lock);
    if (!j->output_glue)
    {
        return -1;
    }

    return 0;
}

static int jumper_inter_input_glue (struct jumper *j, struct aud_mod_inst *inst_out,
    int port_out_index, struct aud_mod_inst *inst_in, int port_in_index)
{
    int ret;
    struct aud_mod_impl *impl;

    info ("input glue..\n");
    impl = search_modimpl ("gluepr", AUD_MODID_PROC (inst_in->uid));
    if (!impl)
    {
        error ("no glue on processor %d\n", AUD_MODID_PROC (inst_in->uid));
        return -1;
    }
    j->input_glue = create_inst_on (impl, NULL);
    mutex_unlock (&impl->lock);
    if (!j->input_glue)
    {
        error ("failed..\n");
        return -1;
    }

    ret = connect_in_processor (j->input_glue, 0, inst_in, port_in_index, false);
    if (ret < 0)
    {
        error ("failed..\n");
        destroy_glue (j->input_glue);
        j->input_glue = NULL;
        return -1;
    }

    return 0;
}

/* Inlined at aud_modlist_connect_inst @0xffffffc010c94d10: ownerless gluepw/gluepr
 * bridge the processors. Input shared-buffer setup always waits; output setup
 * follows sync. Both local connector links remain asynchronous. */
static int connect_between_processor (struct aud_mod_inst *inst_out,
    int port_out_index, struct aud_mod_inst *inst_in, int port_in_index, bool sync)
{
    struct jumper *j;
    int ret;
    struct port *port_out;
    struct port *port_in;

    j = kzalloc (sizeof (*j), GFP_KERNEL);
    if (!j)
    {
        error ("no mem\n");
        return -1;
    }

    /* make glue on output processor */
    ret = jumper_inter_output_glue (j, inst_out, port_out_index);
    if (ret < 0)
    {
        error ("cannot make output glue..\n");
        goto failed;
    }

    /* make glue on input processor */
    ret = jumper_inter_input_glue (j, inst_out, port_out_index, inst_in, port_in_index);
    if (ret < 0)
    {
        error ("cannot make input glue..\n");
        goto failed;
    }

    if (initialize_shbuf (inst_out, inst_in, j) < 0)
    {
        error ("failed..\n");
        ret = -1;
        goto failed;
    }

    info ("jumper between %s:%d -> %s:%d, offset %x\n",
        inst_out->name, port_out_index, inst_in->name, port_in_index, j->offset);

    {
        GlueCmdSetSharedBuffer shared_param = { };

        shared_param.shared_buf_offset = j->offset;
        shared_param.shared_buf_size = j->shared_size;
        shared_param.cowork_module_uid = j->output_glue->uid;
        shared_param.max_access_size = j->max_access_size;
        shared_param.aui_count = j->aui_count;

        aimc_send_command_sync (NULL, GLUE_CMD_SET_SHARED_BUFFER, 0, j->input_glue->uid,
            &shared_param, sizeof (shared_param));

        shared_param.cowork_module_uid = j->input_glue->uid;

        if (sync)
            aimc_send_command_sync (NULL, GLUE_CMD_SET_SHARED_BUFFER, 0, j->output_glue->uid,
                &shared_param, sizeof (shared_param));
        else
            aimc_send_command (NULL, GLUE_CMD_SET_SHARED_BUFFER, 0, j->output_glue->uid,
                &shared_param, sizeof (shared_param));
    }

    // finally, connect output and output glue.
    ret = connect_in_processor (inst_out, port_out_index, j->output_glue, 0, false);
    if (ret < 0)
    {
        error ("cannot connect output/glue\n");
        goto failed;
    }

    port_out = get_output_port (inst_out, port_out_index);
    port_in = get_input_port (inst_in, port_in_index);
    if (!port_out || !port_in)
    {
        error ("failed..\n");
        ret = -1;
        goto failed;
    }

    port_in->input_jumper = j;
    list_add (&j->jumper_list, &port_out->output_jumpers);
    j->output_port = port_out;
    j->input_port = port_in;

    return 0;

  failed:
    if (j->input_glue)
    {
        disconnect_in_processor (j->input_glue, get_output_port (j->input_glue, 0),
            inst_in, get_input_port (inst_in, port_in_index));
        destroy_glue (j->input_glue);
    }
    if (j->output_glue)
    {
        disconnect_in_processor (inst_out, get_output_port (inst_out, port_out_index),
            j->output_glue, get_input_port (j->output_glue, 0));
        destroy_glue (j->output_glue);
    }

    kfree (j);

    return ret;
}

/* @0xffffffc010c91070: Shipped as .isra.0: detach input then output, destroy both glues
 * synchronously iff shared_size != 0, then free shared storage and jumper. */
static int destroy_jumper (struct jumper *j)
{
    struct port *port_in = j->input_port;
    struct port *port_out = j->output_port;
    struct aud_mod_inst *inst_in = port_in->inst;
    struct aud_mod_inst *inst_out = port_out->inst;

    info ("destroy jumper between %s:%d -> %s:%d\n",
        inst_out->name, port_out->index, inst_in->name, port_in->index);
    list_del (&j->jumper_list);

    /* inst_out  -->  j->output_glue  -->  shared  -->  j->input_glue  -->  inst_in */

    /* disconnect input of "inst_in" */
    if (j->input_glue)
    {
        disconnect_in_processor (j->input_glue, port_in->up_peer, inst_in, port_in);
    }
    else
        error ("Oops??\n");

    /* disconnect output of "inst_out" */
    if (j->output_glue)
    {
        disconnect_in_processor (inst_out, port_out,
            j->output_glue, get_input_port (j->output_glue, 0));
    }
    else
        error ("Oops??\n");

    if (j->input_glue)
        destroy_glue_sync (j->input_glue, j->shared_size != 0);
    if (j->output_glue)
        destroy_glue_sync (j->output_glue, j->shared_size != 0);

    if (j->dsp_shared)
    {
        hma_free ("audio_buffer", j->dsp_shared);
    }
    else if (j->shared)
    {
        vfree (j->shared);
    }

    kfree (j);

    return 0;
}

/* @0xffffffc010c8f764: Global lookup filters processor/implementation, with no reference increment. */
static struct aud_mod_inst *inst_by_uid (unsigned int uid)
{
    struct aud_proc_info *p;
    struct aud_mod_inst *ret = NULL;

    mutex_lock (&processors_lock);
    list_for_each_entry (p, &processors, proc_list)
    {
        struct aud_mod_impl *impl;

        if (AUD_MODLIST_PROC (uid) != p->proc_id)
            continue;

        mutex_lock (&p->mods_lock);
        list_for_each_entry (impl, &p->mods, mods_list)
        {
            struct aud_mod_inst *inst;

            if (!AUD_MODID_IS_INTERNAL (uid) && AUD_MODLIST_MODIMPL (uid) != impl->impl_num)
                continue;

            mutex_lock (&impl->lock);
            list_for_each_entry (inst, &impl->instances, impl_list)
            {
                if (inst->uid == uid)
                {
                    ret = inst;
                    break;
                }
            }
            mutex_unlock (&impl->lock);

            if (ret)
                break;
        }
        mutex_unlock (&p->mods_lock);

        if (ret)
            break;
    }
    mutex_unlock (&processors_lock);

    return ret;
}

/* @0xffffffc010c90810: Returns a borrowed owner instance; optional global fallback is not owner-isolated. */
static struct aud_mod_inst *owner_inst_by_uid (struct aud_mod_owner *o, unsigned int uid)
{
    struct aud_mod_inst *inst;

    mutex_lock (&o->instances_lock);
    list_for_each_entry (inst, &o->instances, owner_list)
    {
        if (inst->uid == uid)
        {
            mutex_unlock (&o->instances_lock);
            return inst;
        }
    }
    mutex_unlock (&o->instances_lock);

    if (allow_nonsecure_connect)
    {
        inst = inst_by_uid (uid);
        if (inst)
            return inst;
    }

    error ("no such uid(%x) on %s\n", uid, o->name);
    return NULL;
}

/* @0xffffffc010c93640: Remove jumpers first, then local peers; reacquire the port after each removal. */
int aud_modlist_disconnect_inst_output (struct aud_mod_owner *o /* not used */ ,
    struct aud_mod_inst *inst_out, int port_out_index)
{
    struct port *port_out;
    int ret = 0;

    mutex_lock (&connection_lock);

    info ("disconnect output%d of %s\n", port_out_index, inst_out->name);
    port_out = get_output_port (inst_out, port_out_index);
    if (!port_out)
    {
        error ("no such output port. %s:%d\n", inst_out->name, port_out_index);
        ret = -EINVAL;
        goto exit;
    }

    /* destroy jumper */
    do
    {
        struct jumper *j;

        j = list_first_entry_or_null (&port_out->output_jumpers, struct jumper, jumper_list);
        if (!j)
            break;
        destroy_jumper (j);

        port_out = get_output_port (inst_out, port_out_index);
    }
    while (port_out);

    /* disconnect port */
    port_out = get_output_port (inst_out, port_out_index);
    while (port_out)
    {
        struct port *port_in;

        port_in = list_first_entry_or_null (&port_out->down_peers, struct port, down_peer_list);
        if (!port_in)
            break;
        disconnect_in_processor (inst_out, port_out, port_in->inst, port_in);

        port_out = get_output_port (inst_out, port_out_index);
    }

  exit:
    mutex_unlock (&connection_lock);
    return ret;
}

/* @0xffffffc010c93840: Connection lock protects jumper teardown or a single local edge removal. */
int aud_modlist_disconnect_inst_input (struct aud_mod_owner *o /* not used */ ,
    struct aud_mod_inst *inst_in, int port_in_index)
{
    struct port *port_out, *port_in;
    int ret = 0;

    mutex_lock (&connection_lock);

    info ("disconnect input%d of %s\n", port_in_index, inst_in->name);
    port_in = get_input_port (inst_in, port_in_index);
    if (!port_in)
    {
        error ("no such input port. %s:%d\n", inst_in->name, port_in_index);
        ret = -EINVAL;
        goto exit;
    }

    if (port_in->input_jumper)
        destroy_jumper (port_in->input_jumper);
    else
    {
        port_out = port_in->up_peer;
        if (port_out)
            disconnect_in_processor (port_out->inst, port_out, inst_in, port_in);
    }

  exit:
    mutex_unlock (&connection_lock);
    return ret;
}

unsigned int aud_inst_create (struct aud_mod_owner *o,
    const char *impl_name, int preferred_processor, bool sync)
{
    struct aud_mod_inst *inst;

    inst = aud_modlist_create_inst (o, impl_name, preferred_processor, sync);
    if (inst)
        return inst->uid;

    return 0;
}

// for add internal module
static struct aud_mod_inst *add_inst_on_sync (struct aud_mod_impl *impl,
    struct aud_mod_owner *owner, unsigned int uid, bool sync)
{
    struct aud_mod_inst *inst;
    int name_len;
    char proc_name[9];          /* string length for 32bit hexa uid */
    char *sym_dest;

    info ("create instance for %s\n", impl->name);

    /* ex : bypass_02030001 */
    name_len = strlen (impl->name) + 10;

    inst = kzalloc (sizeof (*inst) + name_len, GFP_KERNEL);
    if (!inst)
    {
        error ("no mem for %s\n", impl->name);
        return NULL;
    }

    inst->uid = uid;
    inst->helper = search_helper (impl->name);
    if (inst->helper && inst->helper->create_pre)
    {
        inst->helper_priv = inst->helper->create_pre (inst->helper->impl_name, inst->uid);
        if (!inst->helper_priv)
        {
            error ("helper failed for \"%s\"\n", impl->name);
            kfree (inst);
            return NULL;
        }
    }

    impl->alloc_count++;

    atomic_set (&inst->users, 1);
    inst->mod_impl = impl;
    inst->owner = owner;
    mutex_init (&inst->lock);
    INIT_LIST_HEAD (&inst->input_ports);
    INIT_LIST_HEAD (&inst->output_ports);
    snprintf (inst->name, name_len, "%s_%08x__", impl->name, inst->uid);
    snprintf (proc_name, sizeof (proc_name), "%08x", inst->uid);

    list_add (&inst->impl_list, &impl->instances);
    inst->proc = proc_mkdir (proc_name, impl->proc_entry);
    inst->proc_cmd_arg.callback = proc_inst_cmd;
    inst->proc_cmd_arg.arg = inst;
    proc_create_data ("cmd", 0220, inst->proc, &aaudio_args_arg_fops, &inst->proc_cmd_arg);
    inst->proc_prop_arg.callback = proc_inst_prop;
    inst->proc_prop_arg.arg = inst;
    inst->proc_prop_arg.read_fops = &inst_prop_read_fops;
    proc_create_data ("prop", 0640, inst->proc, &aaudio_args_arg_fops, &inst->proc_prop_arg);
    proc_create_data ("status", 0440, inst->proc, &inst_status_fops, inst);
    proc_create_data ("lstatus", 0440, inst->proc, &inst_lstate_fops, inst);
    sym_dest = kasprintf (GFP_KERNEL, "../processors/%s/%s/%08x/",
        impl->processor->name, impl->name, inst->uid);
    if (sym_dest)
    {
        inst->proc_status_sym = proc_symlink (proc_name, proc_instances, sym_dest);
        info ("symlink %s -> %s\n", proc_name, sym_dest);
        kfree (sym_dest);
    }

    if (inst->helper && inst->helper->create_post)
        inst->helper->create_post (inst->helper_priv, inst->helper->impl_name, inst->uid);

    if (owner)
    {
        mutex_lock (&owner->instances_lock);
        list_add (&inst->owner_list, &owner->instances);
        mutex_unlock (&owner->instances_lock);
    }

    info ("instance created. %08x\n", inst->uid);

    return inst;
}

// for add internal module
int aud_modlist_add_inst (struct aud_mod_owner *owner, const char *impl_name, unsigned int uid,
    int proc, bool sync)
{
    struct aud_mod_inst *inst;
    struct aud_mod_impl *impl;

    info ("create instance for %s on %d\n", impl_name, proc);

    // check redundan
    inst = inst_by_uid (uid);
    if (inst)
    {
        error ("%s %x is already added\n", impl_name, uid);
        return -1;
    }

    impl = search_modimpl (impl_name, proc);
    if (!impl)
    {
        error ("no such implementation on %d. %s\n", proc, impl_name);
        return -1;
    }

    inst = add_inst_on_sync (impl, owner, uid, sync);
    mutex_unlock (&impl->lock);
    if (!inst)
    {
        error ("cannot create instance.\n");
        return -1;
    }

    info ("new instance from %s:%s, uid %x\n", impl->processor->name, impl->name, inst->uid);

    return 0;
}

void aud_inst_destroy (struct aud_mod_owner *o, unsigned int uid)
{
    struct aud_mod_inst *inst;

    inst = owner_inst_by_uid (o, uid);
    if (!inst)
    {
        error ("no owner\n");
        return;
    }

    aud_modlist_destroy_inst (inst);
}

// for add internal module
int aud_modlist_remove_inst (struct aud_mod_owner *owner, const char *impl_name, unsigned int uid)
{
    struct aud_mod_inst *inst;

    inst = owner_inst_by_uid (owner, uid);
    if (!inst)
    {
        error ("no owner\n");
        return -1;
    }

    info ("destroy instance. %p(%s)\n", inst, inst->name);

    if (inst->helper && inst->helper->destroy_pre)
        inst->helper->destroy_pre (inst->helper_priv, inst->helper->impl_name, inst->uid);

    if (inst->mod_impl)
    {
        inst->mod_impl->alloc_count--;
    }

    if (inst->helper && inst->helper->destroy_post)
        inst->helper->destroy_post (inst->helper_priv, inst->helper->impl_name, inst->uid);

    if (inst->role)
        aud_modlist_role_set (inst, NULL);

    /* remove from processor list to prevent deadlock */
    remove_from_processor (inst);

    if (inst->owner)
        list_del (&inst->owner_list);
    proc_remove (inst->proc_status_sym);
    proc_remove (inst->proc);

    kfree (inst);
    return 0;
}

/* @0xffffffc010c95540: Resolves both UIDs before rejecting a missing endpoint; forwards sync unchanged. */
int aud_inst_connect (struct aud_mod_owner *o, unsigned int uid_out, int port_out_index,
    unsigned int uid_in, int port_in_index, bool sync)
{
    struct aud_mod_inst *inst_out, *inst_in;

    info ("connect %08x.%d -> %d.%d\n", uid_out, port_out_index, uid_in, port_in_index);
    inst_out = owner_inst_by_uid (o, uid_out);
    inst_in = owner_inst_by_uid (o, uid_in);
    if (!inst_out || !inst_in)
    {
        error ("no inst\n");
        return -EINVAL;
    }

    return aud_modlist_connect_inst (o, inst_out, port_out_index, inst_in, port_in_index, sync);
}

/* @0xffffffc010c946b0: Missing UID returns -EINVAL; otherwise disconnect every selected-output peer. */
int aud_inst_disconnect_output (struct aud_mod_owner *o, unsigned int uid_out, int port_out_index)
{
    struct aud_mod_inst *inst;

    inst = owner_inst_by_uid (o, uid_out);
    if (!inst)
    {
        error ("no inst\n");
        return -EINVAL;
    }

    return aud_modlist_disconnect_inst_output (o, inst, port_out_index);
}

/* @0xffffffc010c94730: Missing UID returns -EINVAL; otherwise disconnect the selected input. */
int aud_inst_disconnect_input (struct aud_mod_owner *o, unsigned int uid_in, int port_in_index)
{
    struct aud_mod_inst *inst;

    inst = owner_inst_by_uid (o, uid_in);
    if (!inst)
    {
        error ("no inst\n");
        return -EINVAL;
    }

    return aud_modlist_disconnect_inst_input (o, inst, port_in_index);
}

/* @0xffffffc010c947b0: Select p, or UID[31:24] only when p is NULL; no owner
 * lookup. Missing instances use create_inst_on_sync(impl, NULL, false), hence
 * a newly allocated UID, not necessarily uid. Only implementation zero may be
 * auto-registered. The shipped direct caller is user_imc ioctl SET_ROLE;
 * this API's presence does not establish a renderer assigning role labels. */
int aud_inst_role_set (struct aud_proc_info *p, unsigned int uid, const char *role)
{
    struct aud_mod_impl *_impl, *impl;

    /* search processor */
    if (!p)
    {
        struct aud_proc_info *pt;

        mutex_lock (&processors_lock);
        list_for_each_entry (pt, &processors, proc_list)
        {
            if (pt->proc_id == AUD_MODLIST_PROC (uid))
            {
                p = pt;
                mutex_lock (&p->mods_lock);
                break;
            }
        }
        mutex_unlock (&processors_lock);

        if (!p)
        {
            error ("cannot search processor for uid %08x\n", uid);
            return -EINVAL;
        }
    }
    else
        mutex_lock (&p->mods_lock);

    info ("set role \"%s\" to instance %08x @ proc \"%s\"\n", role, uid, p->name);
    /* search implement */
    impl = NULL;
    list_for_each_entry (_impl, &p->mods, mods_list)
    {
        if (AUD_MODLIST_MODIMPL (uid) == _impl->impl_num)
        {
            impl = _impl;
            break;
        }
    }
    if (!impl && AUD_MODLIST_MODIMPL (uid) == 0)
    {
        info ("register internal implement for \"_predefined\"..\n");
        impl = register_impl (p, "_predefined", 0, AUD_MODLIST_MODIMPL (uid));
    }

    if (impl)
    {
        struct aud_mod_inst *_inst, *inst;

        /* search instance */
        inst = NULL;
        list_for_each_entry (_inst, &impl->instances, impl_list)
        {
            if (_inst->uid == uid)
            {
                inst = _inst;
                break;
            }
        }

        if (!inst)
        {
            info ("create dummy instance for role \"%s\"\n", role);
            inst = create_inst_on_sync (impl, NULL, false);
        }

        if (inst)
        {
            aud_modlist_role_set (inst, role);
            mutex_unlock (&p->mods_lock);
            return 0;
        }
    }
    mutex_unlock (&p->mods_lock);

    error ("unknown uid %08x, role %s\n", uid, role);
    return -EINVAL;
}

unsigned int aud_inst_role_search (const char *role)
{
    struct aud_proc_info *p;

    mutex_lock (&processors_lock);
    /* search processor */
    list_for_each_entry (p, &processors, proc_list)
    {
        struct aud_mod_impl *impl;

        /* search implement */
        list_for_each_entry (impl, &p->mods, mods_list)
        {
            struct aud_mod_inst *inst;

            list_for_each_entry (inst, &impl->instances, impl_list)
            {
                /* search instance */
                if (inst->role && !strcmp (inst->role, role))
                {
                    unsigned int uid = inst->uid;
                    info ("uid %08x has role \"%s\"\n", uid, role);

                    mutex_lock (&p->mods_lock);
                    return uid;
                }
            }
        }
    }
    error ("unknown role. %s\n", role);

    mutex_unlock (&processors_lock);

    return 0;
}

struct aud_proc_info *aud_modlist_reset_dsp (unsigned int dsp_num)
{
    struct aud_proc_info *p;

    if (predefined_procs[dsp_num].proc_info)
        aud_modlist_unregister_processor (predefined_procs[dsp_num].proc_info);

    p = register_processor (predefined_procs[dsp_num].proc_id, 0, predefined_procs[dsp_num].name);
    if (!p)
    {
        error ("register_processor failed\n");
        return NULL;
    }
    predefined_procs[dsp_num].proc_info = p;

    return p;
}

/* @0xffffffc010c94d10: Locks output then input; rejects self-connect and routes by UID processor byte. */
int aud_modlist_connect_inst (struct aud_mod_owner *o /*not used */ ,
    struct aud_mod_inst *inst_out,
    int port_out_index, struct aud_mod_inst *inst_in, int port_in_index, bool sync)
{
    int ret = 0;

    if (inst_out == inst_in)
    {
        error ("connect same instance. %s\n", inst_out->name);
        return -EINVAL;
    }

    mutex_lock (&inst_out->lock);
    mutex_lock (&inst_in->lock);

    info ("connect %s.%d -> %s.%d, sync%d\n", inst_out->name, port_out_index, inst_in->name,
        port_in_index, sync);

    if (AUD_MODID_PROC (inst_out->uid) == AUD_MODID_PROC (inst_in->uid))
        ret = connect_in_processor (inst_out, port_out_index, inst_in, port_in_index, sync);
    else
        ret = connect_between_processor (inst_out, port_out_index, inst_in, port_in_index, sync);

    mutex_unlock (&inst_in->lock);
    mutex_unlock (&inst_out->lock);

    return ret;
}

/* returns UID of glue */
unsigned int aud_prepare_ext_buf (struct aud_mod_owner *o, unsigned int uid, unsigned int port,
    unsigned int in_out)
{
    struct aud_mod_inst *inst;
    struct aud_mod_inst *glue;
    struct aud_mod_impl *impl;
    struct port *inst_port;

    int ret;

    inst = owner_inst_by_uid (o, uid);
    if (!inst)
    {
        error ("uid(%x) is not created\n", uid);
        return -1;
    }

    impl = search_modimpl ("glue", AUD_MODID_PROC (uid));
    if (!impl)
    {
        error ("no glue on processor %d\n", AUD_MODID_PROC (uid));
        return 0;
    }
    glue = create_inst_on (impl, NULL);
    if (!glue)
    {
        error ("can not create instatnce\n");
        return -1;
    }

    mutex_unlock (&impl->lock);

    if (in_out == AUD_PORT_INPUT)
    {
        ret = connect_in_processor (glue, 0, inst, port, true);
        if (ret < 0)
        {
            destroy_glue (glue);
            return 0;
        }

        inst_port = get_input_port (inst, port);
        if (!inst_port)
        {
            error ("no such input port. %s:%d\n", inst->name, port);
            destroy_glue (glue);
            return -EINVAL;
        }
    }
    else
    {
        ret = connect_in_processor (inst, port, glue, 0, true);
        if (ret < 0)

        {
            destroy_glue (glue);
            return 0;
        }

        inst_port = get_output_port (inst, port);
        if (!inst_port)
        {
            error ("no such output port. %s:%d\n", inst->name, port);
            destroy_glue (glue);
            return -EINVAL;
        }
    }
    inst_port->glue = glue;

    return glue->uid;
}

void aud_cleanup_ext_buf (struct aud_mod_owner *o, unsigned int uid, unsigned int port,
    unsigned int in_out)
{
    struct aud_mod_inst *inst;
    struct aud_mod_inst *glue;
    struct port *inst_port;

    inst = owner_inst_by_uid (o, uid);
    if (!inst)
    {
        error ("uid(%x) is not created\n", uid);
        return;
    }

    if (in_out == AUD_PORT_INPUT)
    {
        inst_port = get_input_port (inst, port);
        if (!inst_port)
        {
            error ("no such input port. %s:%d\n", inst->name, port);
            return;
        }

        if (!inst_port->glue)
        {
            error ("no glue. %s:%d\n", inst->name, port);
            return;
        }
        glue = inst_port->glue;
        disconnect_in_processor (glue, get_output_port (glue, 0), inst, inst_port);
    }
    else
    {
        inst_port = get_output_port (inst, port);
        if (!inst_port)
        {
            error ("no such output port. %s:%d\n", inst->name, port);
            return;
        }

        if (!inst_port->glue)
        {
            error ("no glue. %s:%d\n", inst->name, port);
            return;
        }

        glue = inst_port->glue;
        disconnect_in_processor (inst, inst_port, glue, get_input_port (glue, 0));
    }

    destroy_glue (glue);
}

/* @0xffffffc010c8f980: Passive DOT projection of processors, named instances
 * (names contain UIDs), ports and local peers, not owners/roles/task identity.
 * Shared jumpers add phys/virt buffer, size, offset and dotted glue edges;
 * their synthetic <glue>_out/in IDs are distinct from ordinary port node IDs.
 * Bufferless jumpers add no bridge. Locks nest processors/mods/impl/instance,
 * but connection_lock is NOT held, so this is not an atomic topology snapshot. */
static int ml_show (struct seq_file *m, void *data)
{
    struct aud_proc_info *p;

    mutex_lock (&processors_lock);
    seq_printf (m, "digraph {\n");
    seq_printf (m, " rankdir=\"LR\";\n");
    seq_printf (m,
        " node [color=black, fillcolor=white, style=filled, shape=box, fontname=\"sans\"]\n");
    list_for_each_entry (p, &processors, proc_list)
    {
        struct aud_mod_impl *impl;

        seq_printf (m, " subgraph cluster_proc%d_%p_%s {\n", p->proc_id, p, p->name);
        seq_printf (m, "  label=\"%s\nproc_id:%d\";\n", p->name, p->proc_id);
        mutex_lock (&p->mods_lock);
        list_for_each_entry (impl, &p->mods, mods_list)
        {
            struct aud_mod_inst *inst;

            mutex_lock (&impl->lock);
            list_for_each_entry (inst, &impl->instances, impl_list)
            {
                struct port *port;

                seq_printf (m, "   subgraph cluster_inst_%p_%s {\n", inst, inst->name);
                seq_printf (m, "    label=\"%s\";\n", inst->name);
                mutex_lock (&inst->lock);
                list_for_each_entry (port, &inst->input_ports, port_list)
                {
                    seq_printf (m, "    %s_%p_in%d [label=\"in%d\"];\n", inst->name, port,
                        port->index, port->index);
                }
                list_for_each_entry (port, &inst->output_ports, port_list)
                {
                    seq_printf (m, "    %s_%p_out%d [label=\"out%d\"];\n", inst->name, port,
                        port->index, port->index);
                }
                mutex_unlock (&inst->lock);
                seq_printf (m, "   }\n");
            }
            mutex_unlock (&impl->lock);
        }
        mutex_unlock (&p->mods_lock);
        seq_printf (m, " }\n");
    }

    list_for_each_entry (p, &processors, proc_list)
    {
        struct aud_mod_impl *impl;

        mutex_lock (&p->mods_lock);
        list_for_each_entry (impl, &p->mods, mods_list)
        {
            struct aud_mod_inst *inst;

            mutex_lock (&impl->lock);
            list_for_each_entry (inst, &impl->instances, impl_list)
            {
                struct port *port;

                mutex_lock (&inst->lock);
                list_for_each_entry (port, &inst->output_ports, port_list)
                {
                    struct port *peer;
                    struct jumper *j;

                    list_for_each_entry (peer, &port->down_peers, down_peer_list)
                    {
                        seq_printf (m, " %s_%p_out%d -> %s_%p_in%d;\n",
                            inst->name, port, port->index, peer->inst->name, peer, peer->index);
                    }

                    list_for_each_entry (j, &port->output_jumpers, jumper_list)
                    {
                        if (j->shared || j->dsp_shared)
                        {
                            struct aud_proc_info *p;
                            struct aud_mod_inst *instglue;

                            if (j->dsp_shared)
                                seq_printf (m,
                                    " shared_buf_%p [label=\"phys:%pap\nsize:%d\noffset:0x%08x\"];\n",
                                    j, &j->dsp_shared, j->shared_size, j->offset);
                            else
                                seq_printf (m,
                                    " shared_buf_%p [label=\"virt:%p\nsize:%d\noffset:0x%08x\"];\n",
                                    j, j->shared, j->shared_size, j->offset);

                            instglue = j->output_glue;
                            p = instglue->mod_impl->processor;
                            seq_printf (m,
                                " subgraph cluster_proc%d_%p_%s {\n"
                                "   subgraph cluster_inst_%p_%s {\n"
                                "     %s_out [label=\"jumper\"];\n"
                                "   }\n"
                                " }\n",
                                p->proc_id, p, p->name, instglue, instglue->name, instglue->name);
                            seq_printf (m, " %s_out -> shared_buf_%p [style=dotted];\n",
                                instglue->name, j);

                            instglue = j->input_glue;
                            p = instglue->mod_impl->processor;
                            seq_printf (m,
                                " subgraph cluster_proc%d_%p_%s {\n"
                                "   subgraph cluster_inst_%p_%s {\n"
                                "     %s_in [label=\"jumper\"];\n"
                                "   }\n"
                                " }\n",
                                p->proc_id, p, p->name, instglue, instglue->name, instglue->name);
                            seq_printf (m, " shared_buf_%p -> %s_in [style=dotted];\n", j,
                                instglue->name);

                        }
                    }
                }
                mutex_unlock (&inst->lock);
            }
            mutex_unlock (&impl->lock);
        }
        mutex_unlock (&p->mods_lock);
    }
    seq_printf (m, "}\n");

    mutex_unlock (&processors_lock);

    return 0;
}

/* @0xffffffc011a7a700: dot_module_connection is 0440. Enumerating directories
 * and reading instance status (name, uid) are passive; cmd/prop/lstatus/dump
 * siblings are not interchangeable with this read-only metadata surface. */
int __init aud_modlist_init (void)
{
    proc_processors = proc_mkdir ("processors", aproc_root);
    proc_instances = proc_mkdir ("instances", aproc_root);
    proc_roles = proc_mkdir ("roles", aproc_root);
    proc_create_data ("dot_module_connection", 0440, aproc_root, &aaudio_seq_show_fops, ml_show);

    return 0;
}

void __exit aud_modlist_exit (void)
{
    struct aud_proc_info *p, *n;

    mutex_lock (&processors_lock);
    list_for_each_entry_safe (p, n, &processors, proc_list)
    {
        unregister_processor (p);
    }
    mutex_unlock (&processors_lock);

    remove_proc_entry ("instances", aproc_root);
    remove_proc_entry ("processors", aproc_root);
}

void aud_mod_name (struct aud_mod_owner *o, unsigned int uid, unsigned char *name)
{
    struct aud_mod_inst *inst;

    inst = owner_inst_by_uid (o, uid);
    if (!inst)
    {
        error ("uid(%x) is not created\n", uid);
        return;
    }

    if (!name)
    {
        error ("param is null\n");
        return;
    }

    snprintf (name, strlen (inst->name), "%s", inst->name);
}
