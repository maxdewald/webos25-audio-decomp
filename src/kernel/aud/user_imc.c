// SPDX-License-Identifier: GPL-2.0-only
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

/*
 * Base: published GPL-2.0 kdriver/core/aud/user_imc.c. The tagged entrypoints
 * and inlined connect/disconnect wrappers were checked against the shipped
 * orig/boot/Image-5.4.268-329.decompressed, SHA256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Offline Capstone evidence: build/scratch/user-imc-verify/ (REA MCP unavailable).
 * Shipped difference: successful-open count and CPU0-affinity toggle below.
 * Other published routines are retained as dependencies, not separately verified.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/poll.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/dma-mapping.h>
#include <asm/cacheflush.h>

#include "ipc/adec_ipc.h"
#include "imc/aimc_internal.h"

#include "kaimc.h"
#include "audio_uimc.h"
#include "module_list.h"
#include "regs.h"

#include "os_util.h"
#include "log.h"

logm_define (auimc, log_level_noti);
#define trace(fmt,args...)  logm_trace(auimc,fmt,##args)
#define debug(fmt,args...)  logm_debug(auimc,fmt,##args)
#define info(fmt,args...)   logm_info(auimc,fmt,##args)
#define noti(fmt,args...)   logm_noti(auimc,fmt,##args)
#define error(fmt,args...)  logm_error(auimc,fmt,##args)

struct hma_shmem
{
    struct list_head list;

    unsigned long vaddr;
    unsigned int paddr;
};

/* Shipped allocation 0xc8 @0xffffffc010c99b68. lock +0x10, name[32]
 * +0x30 (zeroed and only used by IPC error/trace logs), number +0x50,
 * messages +0x58, wq +0x68, processor +0x80, mod_owner +0x88,
 * hma lock +0x90/list +0xb0, task_id +0xc0. No stored opener pid/tgid/comm.
 * Bounded raw proof: open/release, ioctl (incl. inlined add-implement/write),
 * ipc_read, post_imc, task-status, processor/owner constructors and command
 * handler registration. This does NOT exclude unrelated kernel subsystems.
 * aud_proc_info.pid +0x48 is the opaque uimc pointer passed to registration,
 * NOT a Linux PID; its name is processor_%d. Owner has a user_%p name, instance
 * list/lock/flags/wq (0x70 bytes), not task identity. Handler nodes are 0x20
 * bytes: next/target/callback/arg; no automatic caller identity is captured.
 * Module ownership is not absent: create/connect/destroy pass this mod_owner.
 * owner_inst_by_uid @0xffffffc010c90810 searches its instances under owner lock,
 * then may fall back to global UID lookup when allow_nonsecure_connect is set
 * (shipped data default true). This is object ownership, not pid/tgid identity;
 * no claim about unrelated credential or procfs access controls is made. */
struct user_imc
{
    struct list_head proc_list;

    struct mutex lock;

    char name[32];
    unsigned int number;
    struct list_head messages;
    wait_queue_head_t wq;

    struct aud_proc_info *processor;
    struct aud_mod_owner *mod_owner;

    struct mutex hma_shmem_lock;
    struct list_head hma_shmems;

    /* Explicit AUIMC_IO_SET_PID (0x40044d14) payload, initially UINT_MAX;
     * advisory liveness only, not opener identity or an authorization check. */
    unsigned int task_id;
};

struct imc_message
{
    struct list_head msg_list;

    IpcHeader header;
};

static DEFINE_MUTEX (processor_list_lock);
static LIST_HEAD (processor_list);
static bool uimc_noncached;
module_param (uimc_noncached, bool, 0644);

static int major;
module_param_named (uimc_major, major, int, 0644);

/* Shipped global @0xffffffc012605028; name inferred. It is not atomic. */
static unsigned int open_count;
/* Shipped kaimc entrypoint @0xffffffc010c8e910; absent from published headers. */
extern int aimc_toggle_cpu0_affinity (bool enable);

/* @0xffffffc010c99b60: allocate processor/owner; unwind either registration failure. */
static int open (struct inode *inode, struct file *filp)
{
    struct user_imc *uimc;

    uimc = kzalloc (sizeof (*uimc), GFP_KERNEL);
    if (!uimc)
        return -ENOMEM;
    mutex_init (&uimc->lock);
    init_waitqueue_head (&uimc->wq);

    uimc->processor = aud_modlist_register_processor (uimc, &uimc->number);
    if (!uimc->processor)
    {
        kfree (uimc);
        return -EIO;
    }

    uimc->mod_owner = aud_modlist_new_owner ("user_%p", uimc);
    if (!uimc->mod_owner)
    {
        aud_modlist_unregister_processor (uimc->processor);
        kfree (uimc);
        return -EIO;
    }

    uimc->task_id = -1;

    mutex_lock (&processor_list_lock);
    list_add (&uimc->proc_list, &processor_list);
    mutex_unlock (&processor_list_lock);

    INIT_LIST_HEAD (&uimc->messages);

    mutex_init (&uimc->hma_shmem_lock);
    INIT_LIST_HEAD (&uimc->hma_shmems);

    filp->private_data = uimc;
    info ("open.. %d\n", uimc->number);

    /* Shipped-only: the second successful concurrent open removes CPU0 affinity. */
    if (open_count++ == 1)
        aimc_toggle_cpu0_affinity (false);

    return 0;
}

/* @0xffffffc010c99a40: unregister processor, delete owner, unlink, free mappings.
 * del_owner @0xffffffc010c93c80 clears instance->owner, unlinks under owner lock
 * and put_instance()s each entry, then frees owner. Unlike aren slots, module
 * instances DO have users refcounts: put_instance @0xffffffc010c939a0 decrements
 * users +0x40; only zero disconnects ports/sends DESTROY_MODULE and frees it.
 * This is distinct from the non-atomic successful-uimc-open count below. */
static int release (struct inode *inode, struct file *filp)
{
    struct user_imc *uimc = filp->private_data;
    struct hma_shmem *shmem;

    info ("release.. %d\n", uimc->number);
    aud_modlist_unregister_processor (uimc->processor);
    aud_modlist_del_owner (uimc->mod_owner);

    mutex_lock (&processor_list_lock);
    list_del (&uimc->proc_list);
    mutex_unlock (&processor_list_lock);

    while ((shmem = list_first_entry_or_null (&uimc->hma_shmems, struct hma_shmem, list)))
    {
        list_del (&shmem->list);
        kfree (shmem);
    }

    kfree (uimc);

    /* No affinity re-enable or queued-message drain exists on this shipped path. */
    open_count--;
    return 0;
}

static unsigned int poll (struct file *filp, struct poll_table_struct *pt)
{
    struct user_imc *uimc = filp->private_data;

    poll_wait (filp, &uimc->wq, pt);

    if (!list_empty (&uimc->messages))
        return POLLIN | POLLRDNORM;

    return 0;
}

static int mmap (struct file *filp, struct vm_area_struct *vma)
{
    struct user_imc *uimc = filp->private_data;
    unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
    unsigned long size = vma->vm_end - vma->vm_start;

    info ("uimc:%p - pgoff 0x%lx(0x%lx), flags 0x%lx, vm_start 0x%lx, size %ld\n",
        uimc, vma->vm_pgoff, offset, vma->vm_flags, vma->vm_start, size);

    if (UIMC_VMALLOC_MMAP_OFFSET <= offset)
    {
        /* vmalloc memory */
        offset = offset - UIMC_VMALLOC_MMAP_OFFSET + VMALLOC_START;
        if (offset >= VMALLOC_END)
        {
            error ("uimc:%p, unknown page offset. %lx\n", uimc, vma->vm_pgoff);
            return -ENXIO;
        }

        return remap_vmalloc_range (vma, (void *) offset, 0);
    }

    if (UIMC_HMA_MMAP_OFFSET <= offset)
    {
        int ret;

        offset -= UIMC_HMA_MMAP_OFFSET;
        offset += aud_base_address ();
        vma->vm_flags |= VM_IO | VM_DONTEXPAND | VM_DONTDUMP;
        if (unlikely (uimc_noncached))
            vma->vm_page_prot = pgprot_noncached (vma->vm_page_prot);

        ret = remap_pfn_range (vma, vma->vm_start, offset >> PAGE_SHIFT, size, vma->vm_page_prot);
        if (ret == 0)
        {
            struct hma_shmem *hma_shmem;

            /* This memory will be freed at release() or ioctl(_MUNMAP). */
            hma_shmem = kzalloc (sizeof (*hma_shmem), GFP_KERNEL);
            if (!hma_shmem)
            {
                error ("nomem for shmem %08lx(%08lx)\n", vma->vm_start, offset);
                return ret;
            }

            hma_shmem->vaddr = vma->vm_start;
            hma_shmem->paddr = offset;

            mutex_lock (&uimc->hma_shmem_lock);
            list_add_tail (&hma_shmem->list, &uimc->hma_shmems);
            mutex_unlock (&uimc->hma_shmem_lock);
        }

        return ret;
    }

    error ("unknown offset. %lx\n", offset);
    return -ENXIO;
}

static int auimc_munmap (struct user_imc *uimc, unsigned int offset)
{
    struct hma_shmem *shmem;
    unsigned int paddr;

    if (offset < UIMC_HMA_MMAP_OFFSET || UIMC_VMALLOC_MMAP_OFFSET <= offset)
    {
        error ("unknown offset for hma shmem. %08x\n", offset);
        return -EINVAL;
    }

    paddr = offset - UIMC_HMA_MMAP_OFFSET + aud_base_address ();

    mutex_lock (&uimc->hma_shmem_lock);
    list_for_each_entry (shmem, &uimc->hma_shmems, list)
    {
        if (shmem->paddr == paddr)
        {
            info ("remove mapping. paddr %08x, vaddr %lx\n", shmem->paddr, shmem->vaddr);
            list_del (&shmem->list);
            kfree (shmem);

            mutex_unlock (&uimc->hma_shmem_lock);
            return 0;
        }
    }

    error ("unknown ptr to unmap. %08x, %08x\n", offset, paddr);
    mutex_unlock (&uimc->hma_shmem_lock);

    return -EINVAL;
}

static int auimc_cache_sync (struct user_imc *uimc, struct auimc_cache *cache)
{
    unsigned int size = cache->size;
    enum dma_data_direction dir;
    struct hma_shmem *selected_shmem, *shmem;
    unsigned int paddr;

    if (cache->shbuf_offset < UIMC_HMA_MMAP_OFFSET
        || UIMC_VMALLOC_MMAP_OFFSET <= cache->shbuf_offset)
    {
        error ("unknown offset for hma shmem. %08x\n", cache->shbuf_offset);
        return -EINVAL;
    }

    /* search hma_shmem */
    paddr = cache->shbuf_offset - UIMC_HMA_MMAP_OFFSET + aud_base_address ();
    selected_shmem = NULL;
    mutex_lock (&uimc->hma_shmem_lock);
    list_for_each_entry (shmem, &uimc->hma_shmems, list)
    {
        if (shmem->paddr == paddr)
        {
            selected_shmem = shmem;
            break;
        }
    }
    mutex_unlock (&uimc->hma_shmem_lock);
    if (!selected_shmem)
    {
        error ("unknown offset. %08x\n", cache->shbuf_offset);
        return -EINVAL;
    }

    if (cache->direction == AUIMC_CACHE_FROMDEV)
        dir = DMA_FROM_DEVICE;
    else
        dir = DMA_TO_DEVICE;

    trace ("uimc:%p shbuf_offset=%08x, offset=%08x, size=%u, %p, direction=%d\n",
        uimc, cache->shbuf_offset, cache->offset, cache->size,
        (void *) (selected_shmem->vaddr + cache->offset), cache->direction);

    if (unlikely (uimc_noncached))
    {
        trace ("uimc:%p, noncached.\n", uimc);
        return 0;
    }

    __dma_map_area ((void *) (selected_shmem->vaddr + cache->offset), size, dir);
    return 0;
}

static int ipc_write (struct user_imc *uimc, struct auimc_ipc_header *aipc)
{
    int rc;
    IpcHeader *h;

    h = kmalloc (sizeof (*h) + aipc->param_length, GFP_KERNEL);
    if (!h)
        return -ENOMEM;

    rc = copy_from_user (h, aipc->header, sizeof (*h) + aipc->param_length);
    if (rc)
    {
        error ("%s - copy_from_user failed. aipc->header:%p, paramLength:%d\n",
            uimc->name, aipc->header, h->paramLength);
        rc = -EFAULT;
        goto exit;
    }

    if (aipc->param_length != h->paramLength)
    {
        error ("param len is different(%u, %u)\n", aipc->param_length, h->paramLength);
        rc = -EINVAL;
        goto exit;
    }

    trace ("%s - cmd %x, sender %x, target %x, target action %x\n",
        uimc->name, h->cmd, h->senderInfo.sender, h->targetInfo.target, h->targetInfo.actionFlag);

    rc = aimc_process_header (h);

  exit:
    kfree (h);

    return rc;

}

/* Raw @0xffffffc010c99f90..0xffffffc010c9a254: queue is per uimc, not
 * per pid or reader thread. Oversize head returns 0 without dequeue; user-copy
 * failure returns -EFAULT and retains it; only successful copy dequeues/frees.
 * Handler/implementation registration binds the supplied processor, not current.
 * ioctl's SP_EL0 reads are stack-canary accesses (+0x458), not identity saves. */
static int ipc_read (struct user_imc *uimc, struct auimc_ipc_header *aipc)
{
    int rc;
    struct imc_message *msg;

    if (aipc->timeout == -1)
    {
        rc = wait_event_interruptible (uimc->wq, !list_empty (&uimc->messages));
        if (rc < 0)
            return rc;
    }
    else if (aipc->timeout > 0)
    {
        rc = wait_event_interruptible_timeout (uimc->wq,
            !list_empty (&uimc->messages), msecs_to_jiffies (aipc->timeout));
        if (rc == 0)
            return -ETIME;
        else if (rc < 0)
            return rc;
    }

    mutex_lock (&uimc->lock);
    msg = list_first_entry_or_null (&uimc->messages, struct imc_message, msg_list);
    if (msg)
    {
        IpcHeader *h = &msg->header;

        if (h->paramLength > aipc->param_length)
            rc = 0;
        else
        {
            rc = copy_to_user (aipc->header, h, sizeof (*h) + h->paramLength);
            if (rc)
            {
                error ("%s - copy_to_user failed. aipc->header:%p, paramLength:%d\n",
                    uimc->name, aipc->header, h->paramLength);
                rc = -EFAULT;
            }
            else
            {
                list_del (&msg->msg_list);
                rc = sizeof (*h) + h->paramLength;
                kfree (msg);
            }
        }
    }
    else
        rc = -ENODATA;
    mutex_unlock (&uimc->lock);

    return rc;
}

static int create_module (struct user_imc *uimc, struct auimc_module *m)
{
    unsigned int uid;

    uid = aud_inst_create (uimc->mod_owner, m->name, m->preferred_processor, 1);
    if (!uid)
    {
        error ("can't create module '%s'\n", m->name);
        return -EFAULT;
    }

    m->uid = uid;

    return 0;
}

static int destroy_module (struct user_imc *uimc, unsigned int uid)
{
    aud_inst_destroy (uimc->mod_owner, uid);

    return 0;
}

/* Inlined in ioctl @0xffffffc010c9aa94; call @0xffffffc010c9aab4, sync=1. */
static int connect_module (struct user_imc *uimc, struct auimc_connect *c)
{
    return aud_inst_connect (uimc->mod_owner, c->out_uid, c->out_port, c->in_uid, c->in_port, 1);
}

/* Inlined in ioctl @0xffffffc010c9ab4c; call @0xffffffc010c9ab64. */
static int disconnect_module_output (struct user_imc *uimc, struct auimc_disconnect *d)
{
    return aud_inst_disconnect_output (uimc->mod_owner, d->uid, d->port);
}

/* Inlined in ioctl @0xffffffc010c9a704; call @0xffffffc010c9a71c. */
static int disconnect_module_input (struct user_imc *uimc, struct auimc_disconnect *d)
{
    return aud_inst_disconnect_input (uimc->mod_owner, d->uid, d->port);
}

static int add_implement (struct user_imc *uimc, struct auimc_implement *imp)
{
    imp->name[sizeof (imp->name) - 1] = 0;
    aud_modlist_register_impl (uimc->processor, imp->name, imp->rank);

    return 0;
}

static int add_implement_port_buf_def (struct user_imc *uimc,
    struct auimc_implement_port_buf_def *def)
{
    def->name[sizeof (def->name) - 1] = 0;
    aud_modlist_register_impl_port_buf_def (uimc->processor, def->name, def->def_num,
        def->max_num_au, def->max_access_size);

    return 0;
}

/* Raw @0xffffffc010c9b1d0: global-list lock then uimc lock; explicit task_id
 * -> find_vpid -> pid_task(PIDTYPE_PID), flags &0x404 only. Missing task, zero
 * id or unmatched processor returns true; UINT_MAX is nonzero and looked up.
 * No tgid/comm comparison and no permission gate on IPC/module operations. */
bool ausrimc_get_task_status_normal (int processor_number)
{
    struct user_imc *uimc;
    bool is_normal = true;

    mutex_lock (&processor_list_lock);
    list_for_each_entry (uimc, &processor_list, proc_list)
    {
        mutex_lock (&uimc->lock);
        if (uimc->number == processor_number)
        {
            unsigned int task_id = uimc->task_id;
            if (task_id > 0)
            {
                struct task_struct *task = pid_task (find_vpid (task_id), PIDTYPE_PID);
                if (task)
                {
                    debug ("flags 0x%08X state 0x%08lX\n", task->flags, task->state);

                    /* inlude/linux/sched.h
                     * #define PF_EXITING               0x00000004       Getting shut down
                     * #define PF_SIGNALED              0x00000400       Killed by a signal  */
                    if (task->flags & (PF_EXITING | PF_SIGNALED))
                    {
                        info ("task flags 0x%08X PF_EXITING %d PF_SIGNALED %d\n", task->flags,
                            ! !(task->flags & PF_EXITING), ! !(task->flags & PF_SIGNALED));
                        is_normal = false;
                    }
                }
            }
            mutex_unlock (&uimc->lock);
            break;
        }
        mutex_unlock (&uimc->lock);
    }

    mutex_unlock (&processor_list_lock);
    return is_normal;
}


/* @0xffffffc010c9a550: validate magic/size, copy by direction, dispatch full cmd.
 * Shipped union bound is 48 bytes. IPC_READ is _IOW: ipc_read copies its payload.
 * ADD_IMPL2, legacy-module and SET_PROC_IDLE requests remain unsupported.
 */
static long ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct user_imc *uimc = filp->private_data;

    union
    {
        struct auimc_info info;
        struct auimc_ipc_header header;
        struct auimc_module module;
        unsigned int uid;
        struct auimc_connect connect;
        struct auimc_disconnect disconnect;
        struct auimc_implement implement;
        struct auimc_implement_port_buf_def implement_port_buf_def;
        struct auimc_role role;
        struct auimc_cache cache;
        unsigned int offset;
    } a;
    long ret = 0;

    if (_IOC_TYPE (cmd) != AUIMC_IOC_MAGIC)
    {
        error ("unknown cmd. %x\n", cmd);
        return -ENOIOCTLCMD;
    }

    if (_IOC_SIZE (cmd) > sizeof (a))
    {
        error ("invalid ioctl size. size %d max_size %ld\n", _IOC_SIZE (cmd), sizeof (a));
        return -ENOIOCTLCMD;
    }

    /* copy input data from user land */
    if (_IOC_DIR (cmd) & _IOC_WRITE)
    {
        int r;

        r = copy_from_user (&a, (void *) arg, _IOC_SIZE (cmd));
        if (r)
        {
            error ("copy_from_user failed. " "cmd %08x, arg %08lx\n", cmd, arg);
            return -EFAULT;
        }
    }

    switch (cmd)
    {
        case AUIMC_IO_GET_INFO:
            a.info.proc_id = uimc->number;
            break;

        case AUIMC_IO_IPC_WRITE:
            ret = ipc_write (uimc, &a.header);
            break;

        case AUIMC_IO_IPC_READ:
            ret = ipc_read (uimc, &a.header);
            break;

        case AUIMC_IO_CREATE_MODULE:
            ret = create_module (uimc, &a.module);
            break;

        case AUIMC_IO_DESTROY_MODULE:
            ret = destroy_module (uimc, a.uid);
            break;

        case AUIMC_IO_CONNECT_MODULE:
            ret = connect_module (uimc, &a.connect);
            break;

        case AUIMC_IO_DISCONNECT_MODULE_OUTPUT:
            ret = disconnect_module_output (uimc, &a.disconnect);
            break;

        case AUIMC_IO_DISCONNECT_MODULE_INPUT:
            ret = disconnect_module_input (uimc, &a.disconnect);
            break;

        case AUIMC_IO_ADD_IMPL:
            ret = add_implement (uimc, &a.implement);
            break;

        case AUIMC_IO_ADD_IMPL_PORT_BUF_DEF:
            ret = add_implement_port_buf_def (uimc, &a.implement_port_buf_def);
            break;

        case AUIMC_IO_SET_ROLE:
            ret = aud_inst_role_set (uimc->processor, a.role.uid, a.role.name);
            break;

        case AUIMC_IO_SEARCH_ROLE:
            a.role.uid = aud_inst_role_search (a.role.name);
            if (a.role.uid == 0)
                ret = -EINVAL;
            break;

        case AUIMC_IO_CACHE_SYNC:
            ret = auimc_cache_sync (uimc, &a.cache);
            break;

        case AUIMC_IO_MUNMAP:
            ret = auimc_munmap (uimc, a.offset);
            break;

        case AUIMC_IO_SET_PID:
            uimc->task_id = a.uid;
            break;

        default:
            error ("unknown cmd. %x\n", cmd);
            ret = -ENOIOCTLCMD;
            break;
    }

    /* copy output data to user land */
    if (ret >= 0 && _IOC_DIR (cmd) & _IOC_READ)
    {
        int r;

        r = copy_to_user ((void *) arg, &a, _IOC_SIZE (cmd));
        if (r)
        {
            error ("copy_to_user failed. " "cmd %08x, arg %08lx\n", cmd, arg);
            return -EFAULT;
        }
    }

    trace ("cmd = %08x (cmd_idx=%d) ret %lx\n", cmd, _IOC_NR (cmd), ret);
    return ret;
}

#ifdef CONFIG_COMPAT
/* @0xffffffc010c9b0a0: only IPC requests zero-extend the embedded compat pointer.
 * Writes the converted 16-byte header back before native ioctl copies it again.
 */
static long compat_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
        case AUIMC_IO_IPC_READ:
        case AUIMC_IO_IPC_WRITE:
        {
            struct auimc_ipc_header header;
            int r;

            r = copy_from_user (&header, (void *) arg, sizeof (header));
            if (r)
            {
                error ("cmd 0x%x, %d\n", cmd, r);
                return -EFAULT;
            }
            header.header = compat_ptr (header.compat_data);
            r = copy_to_user ((void *) arg, &header, sizeof (header));
            if (r)
            {
                error ("cmd 0x%x, %d\n", cmd, r);
                return -EFAULT;
            }

            break;
        }

        default:
            break;
    }
    return ioctl (filp, cmd, arg);
}
#endif

static struct file_operations aud_uimc_fops = {
    .owner = THIS_MODULE,
    .open = open,
    .release = release,
    .poll = poll,
    .mmap = mmap,
    .unlocked_ioctl = ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = compat_ioctl,
#endif
};

int ausrimc_post_imc (int processor_number, IpcHeader * header, void *param)
{
    struct user_imc *uimc;
    struct imc_message *message = NULL;

    mutex_lock (&processor_list_lock);
    list_for_each_entry (uimc, &processor_list, proc_list)
    {
        mutex_lock (&uimc->lock);
        if (uimc->number == processor_number)
        {
            message = kzalloc (sizeof (*message) + header->paramLength, GFP_KERNEL);
            if (!message)
            {
                error ("no mem. %d\n", header->paramLength);

                mutex_unlock (&uimc->lock);
                mutex_unlock (&processor_list_lock);
                return -ENOMEM;
            }

            memcpy (&message->header, header, sizeof (*header));
            if (header->paramLength)
                memcpy (message->header.param, param, header->paramLength);

            list_add_tail (&message->msg_list, &uimc->messages);
            wake_up_all (&uimc->wq);

            debug ("message posted on %d\n", processor_number);

            mutex_unlock (&uimc->lock);
            mutex_unlock (&processor_list_lock);

            return 0;
        }
        mutex_unlock (&uimc->lock);
    }

    error ("no such processor number. %d, cmd %x, sender %x, target %x, length %x\n",
        processor_number, header->cmd, header->senderInfo.sender,
        header->targetInfo.target, header->paramLength);
    mutex_unlock (&processor_list_lock);
    return -EIO;
}

EXPORT_SYMBOL (ausrimc_post_imc);

int ausrimc_init (void)
{
    int err;

    err = register_chrdev (major, "uimc", &aud_uimc_fops);
    if (err < 0)
    {
        error ("err %d\n", err);
        return err;
    }
    if (!major && err > 0)
        major = err;

    OS_CreateDeviceClass (MKDEV (major, 0), "audio_uimc");

    return 0;
}

void __exit ausrimc_exit (void)
{
    noti ("..\n");

    OS_DestroyDeviceClass (MKDEV (major, 0));
    unregister_chrdev (major, "uimc");
}
