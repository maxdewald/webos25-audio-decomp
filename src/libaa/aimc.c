/*
 * libaa aimc: the per-process "audio IMC" endpoint. Wraps the kernel driver
 * /dev/lg/audio_uimc (ioctl magic 'M') which forwards module create/connect requests
 * and IpcHeader messages to the audio DSPs, or - when the "aimc_use_aipc" option is
 * set - the aipc socket protocol to the "acont" controller process.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 * Evidence: build/decomp/libaa.so.2.0.0.c @0x1cc08..@0x1e654, logmm "aimc" strings,
 * handler thread strings resolved from its literal pool (VA 0xd71c..0xd74c).
 */
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "aimc_local.h"

LOGMM_MODULE_DEFINE(aimc);
LOGMM_MODULE_DEFINE(aud);

/* command handler registered per module uid */
struct command_handler {          /* 0x18 */
    struct link list;
    unsigned int target;
    void *handler;                /* aimc_cmd_handler_fn (old == 0) or PFN_ImcCmdProc */
    void *arg;
    int old;                      /* 1: DSP-style ImcCmdProc with ImcSenderInfo */
};

/* aimc_register_event() result */
struct aimc_event {               /* 0x28 */
    struct aimc *imc;
    struct link list;
    unsigned int action_id;
    unsigned int unknown_10;
    void (*callback)(void *param, int size, void *arg);
    void *arg;
    unsigned int event;
    unsigned int target;
    int repeat;
};

static int ignore_uimc = -1;      /* option "aimc_ignore_uimc" */
static int use_aipc = -1;         /* option "aimc_use_aipc" */

/* @0x1cc08: aipc command "aimc__ipc_send" - IpcHeader from another Linux process */
int aimc__ipc_send(void *arg, int cfd, const char *name, int nargs, struct aipc_ipc_arg *args)
{
    struct aimc *imc = arg;
    int param_length = args[0].i;     /* header->param_len, as sent by aimc_ipc_send */
    IpcHeader *h = args[1].m;

    (void)cfd; (void)name; (void)nargs;
    pthread_mutex_lock(&imc->imc_process_lock);
    /* the original really subtracts the header size from param_len here */
    LOGD(aimc, 0x78, "process imc header.. param_length %d\n", param_length - 0x20);
    IMC_ProcessHeader(&imc->imc_info, h);
    LOGD(aimc, 0x7a, "process imc header done.\n");
    pthread_mutex_unlock(&imc->imc_process_lock);
    return 0;
}

/* @0x1cd4c: aipc command "__sync" - round trip used by aimc_add_processor_done */
int aimc___sync(void *arg, int cfd, const char *name, int nargs, struct aipc_ipc_arg *args)
{
    (void)arg; (void)cfd; (void)name; (void)nargs; (void)args;
    LOGI(aimc, 0x69, "sync..\n");
    return 0;
}

/* @0x1cc9c: register_command_handler (add or update the handler for target) */
int register_command_handler(struct aimc *imc, unsigned int target, void *handler, void *arg,
                             int old)
{
    struct command_handler *ch;
    struct link *l;

    pthread_mutex_lock(&imc->command_lock);
    for_each_link(l, &imc->command_list) {
        ch = link_container(struct command_handler, list, l);
        if (ch->target == target)
            goto found;
    }
    ch = calloc(1, sizeof(*ch));
    if (!ch) {
        LOGE(aimc, 0x329, "Oops\n");
        pthread_mutex_unlock(&imc->command_lock);
        return -1;
    }
    ch->target = target;
    ch->old = old;
    link_add_tail(&imc->command_list, &ch->list);
found:
    ch->handler = handler;
    ch->arg = arg;
    pthread_mutex_unlock(&imc->command_lock);
    return 0;
}

/* @0x1cd94 */
struct aimc *aimc_create(void)
{
    struct aimc *imc;
    struct auimc_info info;
    pid_t pid;

    imc = calloc(1, sizeof(*imc));
    if (!imc) {
        LOGE(aimc, 0x8b, "no mem\n");
        return NULL;
    }

    if (ignore_uimc < 0)
        ignore_uimc = autil_option_int("aimc_ignore_uimc", 0);
    if (ignore_uimc) {
        imc->fd = -1;
    } else {
        imc->fd = open("/dev/lg/audio_uimc", O_RDWR);
        if (imc->fd < 0)
            imc->fd = open("/dev/audio_uimc", O_RDWR);
        if (imc->fd < 0) {
            LOGE(aimc, 0x9a, "cannot open audio_uimc. %s\n", strerror(errno));
            goto fail;
        }
    }

    pthread_mutex_init(&imc->lock, NULL);
    pthread_mutex_init(&imc->command_lock, NULL);
    LINK_INIT(&imc->command_list);
    LINK_INIT(&imc->event_list);
    imc->ipc_param_length = 0x100;
    imc->ipc_header = calloc(0x120, 1);
    if (!imc->ipc_header) {
        LOGE(aimc, 0xa9, "no mem\n");
        goto fail;
    }

    if (use_aipc < 0)
        use_aipc = autil_option_int("aimc_use_aipc", 0);
    if (use_aipc) {
        imc->aipc = aipc_create();
        if (!imc->aipc) {
            LOGE(aimc, 0xb4, "no aipc\n");
            goto fail;
        }
        imc->processor_id = aipc_get_proc_id(imc->aipc);
        pthread_mutex_init(&imc->imc_process_lock, NULL);
        aipc_register_command(imc->aipc, "__sync", aimc___sync, imc);
        aipc_register_command(imc->aipc, "aimc__ipc_send", aimc__ipc_send, imc);
        imc->adbg = adebug_create(imc->aipc);
        if (!imc->adbg) {
            LOGE(aimc, 0xc1, "no adebug\n");
            goto fail;
        }
    }

    if (imc->fd >= 0) {
        pid = getpid();
        if (ioctl(imc->fd, AUIMC_IO_GET_INFO, &info) < 0) {
            LOGE(aimc, 0xce, "AUIMC_IO_GET_INFO - %s\n", strerror(errno));
            goto fail;
        }
        imc->processor_id = info.proc_id;
        LOGN(aimc, 0xd3, "set pid = %d\n", pid);
        ioctl(imc->fd, AUIMC_IO_SET_PID, &pid);
    }

    snprintf(imc->name, sizeof(imc->name), "aimc_%d", imc->processor_id);
    IMC_Init(&imc->imc_info, imc->processor_id);
    IMC_SetLocalImc(&imc->imc_info);
    return imc;

fail:
    if (imc->aipc)
        aipc_destroy(imc->aipc);
    if (imc->adbg)
        adebug_destroy(imc->adbg);
    if (imc->ipc_header)
        free(imc->ipc_header);
    if (imc->fd >= 0)
        close(imc->fd);
    free(imc);
    return NULL;
}

/* @0x1d34c */
void aimc_finalize(struct aimc *imc)
{
    IMC_Finalize(&imc->imc_info);
    aimc_handler_destory(imc);
    if (imc->aipc)
        aipc_destroy(imc->aipc);
    if (imc->adbg)
        adebug_destroy(imc->adbg);
    free(imc->ipc_header);
    if (imc->fd >= 0)
        close(imc->fd);
    free(imc);
}

/* @0x1d50c: handler_thread - polls the control pipe, the uimc fd and (optionally)
 * the aipc sockets; runs aimc_process() for every message from the driver.
 */
int handler_thread(void *arg)
{
    struct aimc *imc = arg;
    struct pollfd *fds = NULL;
    int nfds_alloc = 0;
    int timeout = -1;

    while (imc->running_handler) {
        int nfds = imc->fd < 0 ? 1 : 2;
        int base;
        int r;

        if (imc->aipc)
            nfds += aipc_get_pollfd(imc->aipc, NULL);
        if (nfds > nfds_alloc) {
            fds = realloc(fds, nfds * sizeof(*fds));
            nfds_alloc = nfds;
        }
        fds[0].fd = imc->handler_pipe[0];
        fds[0].events = POLLIN;
        base = 1;
        if (imc->fd >= 0) {
            fds[1].fd = imc->fd;
            fds[1].events = POLLIN;
            base = 2;
        }
        if (imc->aipc)
            aipc_get_pollfd(imc->aipc, &fds[base]);

        r = poll(fds, nfds, timeout);
        if (r < 0) {
            LOGE(aimc, 0x128, "poll error\n");
            break;
        }
        if (r == 0) {
            /* timed out after a sync request: the pipe drained, wake the waiter */
            LOGD(aimc, 0x12e, "sync done..\n");
            timeout = -1;
            pthread_mutex_lock(&imc->handler_sync_lock);
            pthread_cond_broadcast(&imc->handler_sync_cond);
            pthread_mutex_unlock(&imc->handler_sync_lock);
            continue;
        }

        if (fds[0].revents & POLLIN) {
            char cmd[32];

            memset(cmd, 0, sizeof(cmd));
            if (read(imc->handler_pipe[0], cmd, sizeof(cmd)) >= 0) {
                cmd[sizeof(cmd) - 1] = '\0';
                LOGD(aimc, 0x13d, "got command \"%s\"\n", cmd);
                if (strcmp(cmd, "sync") == 0) {
                    timeout = 0;
                    LOGD(aimc, 0x141, "sync start..\n");
                }
                if (strcmp(cmd, "stop") == 0) {
                    LOGN(aimc, 0x14d, "I'm going!!!!!!\n");
                    break;
                }
            }
        }
        if (imc->fd >= 0 && (fds[1].revents & POLLIN)) {
            if (aimc_process(imc) != 0)
                break;
        }
        if (imc->aipc)
            aipc_handle_pollfd(imc->aipc, &fds[base]);
    }
    free(fds);
    return 0;
}

/* @0x1d0d4 */
int aimc_handler_create(struct aimc *imc)
{
    if (imc->handler_thread) {
        LOGE(aimc, 0x169, "already created\n");
        return -1;
    }
    imc->running_handler = 1;
    if (pipe(imc->handler_pipe) < 0) {
        LOGE(aimc, 0x170, "no pipe. %s\n", strerror(errno));
        return -1;
    }
    pthread_cond_init(&imc->handler_sync_cond, NULL);
    pthread_mutex_init(&imc->handler_sync_lock, NULL);
    imc->handler_thread = aud_thread_create(imc->name, 0x8000, 0, handler_thread, imc);
    if (!imc->handler_thread) {
        LOGE(aimc, 0x179, "can't create handler thread\n");
        return -1;
    }
    return 0;
}

/* @0x1d1d8: alias of aimc_handler_create */
int aimc_create_handler(struct aimc *imc)
{
    return aimc_handler_create(imc);
}

/* @0x1d1dc: wait until the handler thread has drained everything queued so far */
int aimc_handler_sync(struct aimc *imc)
{
    pthread_mutex_lock(&imc->handler_sync_lock);
    LOGD(aimc, 0x189, "sync handler.\n");
    if (write(imc->handler_pipe[1], "sync", 4) < 0)
        LOGE(aimc, 0x18b, "cannot write to pipe. %s\n", strerror(errno));
    pthread_cond_wait(&imc->handler_sync_cond, &imc->handler_sync_lock);
    LOGD(aimc, 0x18d, "sync done.\n");
    pthread_mutex_unlock(&imc->handler_sync_lock);
    return 0;
}

/* @0x1d2bc */
void aimc_handler_destory(struct aimc *imc)
{
    if (!imc->handler_thread)
        return;
    imc->running_handler = 0;
    if (write(imc->handler_pipe[1], "stop", 4) < 0)
        LOGE(aimc, 0x199, "cannot write to pipe. %s\n", strerror(errno));
    aud_thread_destroy(imc->handler_thread);
    close(imc->handler_pipe[0]);
    close(imc->handler_pipe[1]);
    imc->handler_thread = NULL;
}

/* @0x1d38c: join the handler thread (it exits on "stop" or error) */
void aimc_handler_wait(struct aimc *imc)
{
    if (!imc->handler_thread || !imc->running_handler)
        return;
    aud_thread_destroy(imc->handler_thread);
}

/* @0x1d3a0: alias of aimc_handler_destory */
void aimc_destroy_handler(struct aimc *imc)
{
    aimc_handler_destory(imc);
}

/* @0x1d3a4 */
int aimc_get_processor_id(struct aimc *imc)
{
    return imc->processor_id;
}

/* @0x1d3a8: unused on Linux */
int aimc_ipc_header_get(ImcInfo *info, unsigned int from, IpcHeader **h)
{
    (void)info; (void)from; (void)h;
    return 0;
}

/* @0x1d3ac: unused on Linux */
int aimc_ipc_header_done(ImcInfo *info, unsigned int from)
{
    (void)info; (void)from;
    return 0;
}

/* @0x1d3b0: read one IpcHeader from the driver (growing the buffer) and process it */
int aimc_process(struct aimc *imc)
{
    struct auimc_ipc_header req;
    int r;

    memset(&req, 0, sizeof(req));
    for (;;) {
        req.param_length = imc->ipc_param_length;
        req.header = imc->ipc_header;
        r = ioctl(imc->fd, AUIMC_IO_IPC_READ, &req);
        if (r < 0) {
            LOGE(aimc, 0x1d3, "AUIMC_IO_IPC_READ - %s\n", strerror(errno));
            return r;
        }
        if (r != 0)
            break;

        /* 0: the message did not fit; retry with a larger buffer */
        if (imc->ipc_param_length >= 0xa00) {
            LOGE(aimc, 0x1e1, "ipc_param_length over max(%d)\n", imc->ipc_param_length);
            return -1;
        }
        imc->ipc_param_length += 0x100;
        free(imc->ipc_header);
        imc->ipc_header = calloc(imc->ipc_param_length + sizeof(IpcHeader), 1);
        if (!imc->ipc_header) {
            LOGE(aimc, 0x1ea, "no mem\n");
            return -1;
        }
    }

    pthread_mutex_lock(&imc->imc_process_lock);
    IMC_ProcessHeader(&imc->imc_info, req.header);
    pthread_mutex_unlock(&imc->imc_process_lock);
    return 0;
}

/* @0x1d750: fire-and-forget IMC command (no response requested) */
int aimc_send_command(struct aimc *imc, unsigned int cmd, unsigned int source,
                      unsigned int target, void *param, unsigned int param_size)
{
    ImcCommandParameter cp;
    unsigned int response_id = 0;

    memset(&cp, 0, sizeof(cp));
    cp.cmd = cmd;
    cp.target = target;
    cp.source = source;
    cp.param = param;
    cp.param_len = param_size;
    return -IMC_SendCommand(&imc->imc_info, &cp, &response_id);
}

/* @0x1d780: alias of aimc_send_command (same signature, no sync semantics anymore) */
int aimc_send_command_sync(struct aimc *imc, unsigned int cmd, unsigned int source,
                           unsigned int target, void *param, unsigned int param_size)
{
    return aimc_send_command(imc, cmd, source, target, param, param_size);
}

/* @0x1d784 */
int aimc_add_processor(struct aimc *imc)
{
    (void)imc;
    return 0;
}

/* @0x1d788 */
int aimc_add_port_buf_def(struct aimc *imc, const char *name, int def_num, int max_num_au,
                          int max_access_size)
{
    struct auimc_implement_port_buf_def def;
    int r;

    if (imc->aipc)
        return aipc_send(imc->aipc, 0, "acont_add_port_buf_def",
                         ipc_argtype_str, name, ipc_argtype_int, def_num,
                         ipc_argtype_int, max_num_au, ipc_argtype_int, max_access_size,
                         ipc_argtype_end);

    strncpy(def.name, name, sizeof(def.name) - 1);
    def.name[sizeof(def.name) - 1] = '\0';
    def.def_num = def_num;
    def.max_num_au = max_num_au;
    def.max_access_size = max_access_size;
    r = ioctl(imc->fd, AUIMC_IO_ADD_IMPL_PORT_BUF_DEF, &def);
    if (r < 0)
        LOGE(aimc, 0x233, "AUIMC_IO_ADD_IMPL_PORT_BUF_DEF - %s\n", strerror(errno));
    return r;
}

/* @0x1d848: announce that this process implements module type `name` */
int aimc_add_implement(struct aimc *imc, const char *name, unsigned int rank)
{
    struct auimc_implement impl;
    int r;

    if (imc->aipc)
        return aipc_send(imc->aipc, 0, "acont_add_implement",
                         ipc_argtype_str, name, ipc_argtype_int, rank, ipc_argtype_end);

    strncpy(impl.name, name, sizeof(impl.name) - 1);
    impl.name[sizeof(impl.name) - 1] = '\0';
    impl.rank = rank;
    r = ioctl(imc->fd, AUIMC_IO_ADD_IMPL, &impl);
    if (r < 0)
        LOGE(aimc, 0x24b, "AUIMC_IO_ADD_IMPL - %s\n", strerror(errno));
    return r;
}

/* @0x1d8f4 */
int aimc_add_processor_done(struct aimc *imc)
{
    int ret = 0, r;

    if (!imc->aipc)
        return 0;
    r = aipc_call(imc->aipc, 0, &ret, "__sync", ipc_argtype_end);
    return r < 0 ? r : 0;
}

/* @0x1d920: deliver an IpcHeader to target_processor. Processors 0..5 (DSPs and the
 * kernel-side endpoints) always go through the driver; higher ids are other Linux
 * processes, reached through the aipc controller when it is enabled.
 */
int aimc_ipc_send(ImcInfo *info, unsigned int target_processor, IpcHeader *header,
                  const void *param)
{
    struct aimc *imc = aimc_of_info(info);
    unsigned int len = header->param_len;
    IpcHeader *msg;
    int r;

    msg = malloc(len + sizeof(*msg));
    if (!msg) {
        LOGE(aimc, 0x26b, "no mem\n");
        return -1;
    }
    *msg = *header;
    if (len)
        memcpy(msg->param, param, len);

    if (imc->aipc && target_processor > 5) {
        r = aipc_send(imc->aipc, target_processor, "aimc__ipc_send",
                      ipc_argtype_int, len, ipc_argtype_mem, msg, len + sizeof(*msg),
                      ipc_argtype_end);
        if (r < 0)
            LOGE(aimc, 0x279, "aipc_send() failed. %d\n", r);
    } else {
        struct auimc_ipc_header req;

        req.timeout = 0;    /* computed as (aipc && target > 5), always 0 on this path */
        req.param_length = len;
        req.compat_sizer = 0;
        req.header = msg;
        r = ioctl(imc->fd, AUIMC_IO_IPC_WRITE, &req);
        if (r < 0)
            LOGE(aimc, 0x285, "AUIMC_IO_IPC_WRITE - %s\n", strerror(errno));
    }
    free(msg);
    return r;
}

/* @0x1da8c */
int aimc_raise_event(struct aimc *imc, unsigned int event, unsigned int owner, void *param,
                     unsigned int param_len)
{
    ImcEventParameter ep = { .request_interrupt = 1, .param = param, .param_len = param_len };

    return -IMC_RaiseEvent(&imc->imc_info, event, owner, &ep);
}

/* @0x1cbec: IMC notification -> user callback, counts deliveries */
void aimc_event_handler(void *param, int size, void *arg)
{
    struct aimc_event *ev = arg;

    ev->unknown_10++;
    ev->callback(param, size, ev->arg);
}

/* @0x1daa8 */
struct aimc_event *aimc_register_event(struct aimc *imc, unsigned int event, unsigned int target,
                                       int repeat, void (*callback)(void *, int, void *),
                                       void *arg)
{
    struct aimc_event *ev;
    ImcActionParameter ap;
    unsigned int action_id;
    int rc;

    ev = calloc(1, sizeof(*ev));
    if (!ev) {
        LOGE(aimc, 0x2b1, "no memory\n");
        return NULL;
    }
    memset(&ap, 0, sizeof(ap));
    ap.action_type = IMC_ACTION_GET_CALLBACK;
    ap.repeat_type = repeat != 0;
    ap.u.noti.noti = aimc_event_handler;
    ap.u.noti.param = ev;
    ap.u.noti.level = 1;
    rc = IMC_RegisterEvent(&imc->imc_info, event, target, &action_id, &ap);
    if (rc != ADEC_ERR_NONE) {
        LOGE(aimc, 0x2be, "IMC_RegisterEvent : [%d]\n", rc);
        free(ev);
        return NULL;
    }
    ev->event = event;
    ev->imc = imc;
    ev->callback = callback;
    ev->target = target;
    ev->repeat = repeat;
    ev->arg = arg;
    ev->action_id = action_id;

    pthread_mutex_lock(&imc->lock);
    link_add_tail(&imc->event_list, &ev->list);
    pthread_mutex_unlock(&imc->lock);
    return ev;
}

/* @0x1dbc0 */
void aimc_cancel_event(struct aimc *imc, struct aimc_event *ev)
{
    int rc;

    rc = IMC_CancelEvent(&imc->imc_info, ev->event, ev->target, ev->action_id);
    if (rc != ADEC_ERR_NONE)
        LOGE(aimc, 0x2d8, "IMC_CancelEvent : [%d]\n", rc);
    pthread_mutex_lock(&imc->lock);
    link_del(&ev->list);
    pthread_mutex_unlock(&imc->lock);
    /* observed: the original frees `imc`, not `ev` (tail call free(r5=imc) @0x1dc0a) */
    free(imc);
}

/* @0x1dc38 */
int aimc_register_named_event(struct aimc *imc, const char *name, unsigned int uid,
                              int repeat, int (*cb)(void *, void *, unsigned int), void *arg)
{
    int id = IMC_RegisterNamedEvent(&imc->imc_info, name, uid, repeat == 0, cb, arg);

    if (id == 0) {
        LOGE(aimc, 0x2ec, "cannot register event. %s\n", name);
        return 0;
    }
    return id;
}

/* @0x1dc9c */
void aimc_unregister_named_event(struct aimc *imc, int evt_id)
{
    IMC_UnregisterNamedEvent(&imc->imc_info, evt_id);
}

/* @0x1dca4 */
void aimc_clear_named_event_on_uid(struct aimc *imc, unsigned int uid)
{
    IMC_UnregisterNamedEvent_onUID(&imc->imc_info, uid);
}

/* @0x1dcac */
int aimc_raise_named_event(struct aimc *imc, unsigned int uid, const char *name,
                           void *data, unsigned int size)
{
    return IMC_RaiseNamedEvent(&imc->imc_info, uid, name, data, size);
}

/* @0x1dcb4 */
int aimc_register_command_handler(struct aimc *imc, unsigned int uid,
                                  int (*cb)(void *arg, unsigned int cmd, void *param,
                                            unsigned int len),
                                  void *arg)
{
    return register_command_handler(imc, uid, cb, arg, 0);
}

/* @0x1dcc4 */
int aimc_unregister_command_handler(struct aimc *imc, unsigned int uid)
{
    struct link *l;

    pthread_mutex_lock(&imc->command_lock);
    for_each_link(l, &imc->command_list) {
        struct command_handler *ch = link_container(struct command_handler, list, l);
        if (ch->target == uid) {
            link_del(&ch->list);
            free(ch);
            pthread_mutex_unlock(&imc->command_lock);
            return 0;
        }
    }
    LOGE(aimc, 0x34b, "no such uid. %x\n", uid);
    pthread_mutex_unlock(&imc->command_lock);
    return 0;
}

/* @0x1dd50: barrier against a concurrently running IMC_ProcessHeader */
void aimc_wait_until_process_header(struct aimc *imc)
{
    pthread_mutex_lock(&imc->imc_process_lock);
    pthread_mutex_unlock(&imc->imc_process_lock);
}

/* @0x1dd68 */
int aimc__register_command_handler(ImcInfo *info, unsigned int target, PFN_ImcCmdProc proc,
                                   void *module)
{
    return register_command_handler(aimc_of_info(info), target, proc, module, 1);
}

/* @0x1dd7c */
void aimc__unregister_command_handler(ImcInfo *info, unsigned int target)
{
    aimc_unregister_command_handler(aimc_of_info(info), target);
}

/* @0x1dd84: call the handler registered for target */
int aimc_dispatch_command(ImcInfo *info, unsigned int target, unsigned int cmd, void *param,
                          unsigned int param_len, unsigned int sender, unsigned int message_id,
                          unsigned int flags)
{
    struct aimc *imc = aimc_of_info(info);
    struct command_handler *ch = NULL;
    struct link *l;
    void *handler, *arg;
    int old, r;

    pthread_mutex_lock(&imc->command_lock);
    for_each_link(l, &imc->command_list) {
        struct command_handler *c = link_container(struct command_handler, list, l);
        if (c->target == target) {
            ch = c;
            break;
        }
    }
    if (!ch) {
        pthread_mutex_unlock(&imc->command_lock);
        LOGE(aimc, 0x3af, "no handler for %x\n", target);
        return -1;
    }
    old = ch->old;
    handler = ch->handler;
    arg = ch->arg;
    pthread_mutex_unlock(&imc->command_lock);

    if (!old) {
        /* aimc-style handler; answers via aimc_command_response() or automatically */
        imc->command_need_response = false;
        if (flags & AIMC_DISPATCH_FLAG_REQUIRE_RESPONSE) {
            ImcResponseParameter *rp = &imc->command_response;

            memset(&rp->target.request_response, 0, 0x20);
            rp->target.module = sender;
            rp->target.core = imc->processor_id;
            rp->target.id = message_id;
            rp->sender = target;
            rp->request_interrupt = 1;
            rp->cmd = cmd;
            imc->command_need_response = true;
        }
        r = ((aimc_cmd_handler_fn)handler)(arg, cmd, param, param_len);
        if (imc->command_need_response) {
            imc->command_response.result = r;
            IMC_Response(info, &imc->command_response);
        }
        return r;
    }

    /* DSP-style ImcCmdProc */
    {
        ImcSenderInfo si = { 0, 0, 0, 0 };

        LOGD(aimc, 0x381, "from %x to %x, cmd %x\n", sender, target, cmd);
        si.module = sender;
        si.id = message_id;
        if (flags & AIMC_DISPATCH_FLAG_REQUIRE_RESPONSE) {
            si.request_response = 1;
            LOGD(aimc, 0x387, "request response.\n");
        }
        if (!arg)
            return -((PFN_ImcCmdProc)handler)(NULL, cmd, &si, param, param_len);
        /* module mutex at +0x84 of the legacy module object */
        pthread_mutex_lock((pthread_mutex_t *)((char *)arg + 0x84));
        r = ((PFN_ImcCmdProc)handler)(arg, cmd, &si, param, param_len);
        pthread_mutex_unlock((pthread_mutex_t *)((char *)arg + 0x84));
        return -r;
    }
}

/* @0x1df68: explicit answer from inside an aimc-style command handler */
int aimc_command_response(struct aimc *imc, int result, void *param, unsigned int param_size)
{
    if (!imc->command_need_response) {
        LOGI(aimc, 0x3ba, "command does not need response.\n");
        return 0;
    }
    imc->command_response.result = result;
    imc->command_response.param = param;
    imc->command_response.param_len = param_size;
    IMC_Response(&imc->imc_info, &imc->command_response);
    imc->command_need_response = false;
    return 0;
}

/* @0x1dfe0: map DSP shared memory through the uimc fd */
void *aimc_mmap(struct aimc *imc, unsigned int offset, unsigned int size)
{
    void *p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, imc->fd, offset);

    if (p == MAP_FAILED) {
        LOGE(aimc, 0x3d1, "mmap failed - offset:0x%x size:%d\n", offset, size);
        return NULL;
    }
    return p;
}

/* @0x1e048: offsets 0xf0000000.. are driver-managed shared buffers */
void aimc_munmap(struct aimc *imc, unsigned int offset, void *addr, unsigned int size)
{
    if (offset - 0xf0000000u < 0x10000000u)
        ioctl(imc->fd, AUIMC_IO_MUNMAP, &offset);
    munmap(addr, size);
}

/* @0x1e084: instantiate module type `module_name` on a processor; returns its uid
 * (top byte = processor id) or 0. preferred_processor 0 means "this process".
 */
unsigned int aimc_module_create(struct aimc *imc, const char *module_name,
                                int preferred_processor)
{
    struct auimc_module req;
    int uid;

    if (preferred_processor == 0)
        preferred_processor = imc->processor_id;

    if (imc->aipc) {
        uid = 0;
        if (aipc_call(imc->aipc, 0, &uid, "acont_module_create",
                      ipc_argtype_str, module_name, ipc_argtype_int, preferred_processor,
                      ipc_argtype_end) < 0)
            uid = 0;
        return uid;
    }

    memset(&req, 0, sizeof(req));
    snprintf(req.name, sizeof(req.name), "%s", module_name);
    req.preferred_processor = preferred_processor;
    if (ioctl(imc->fd, AUIMC_IO_CREATE_MODULE, &req) >= 0)
        return req.uid;
    LOGE(aimc, 0x3fc, "AUIMC_IO_CREATE_MODULE - %s\n", strerror(errno));
    return 0;
}

/* @0x1e15c */
int aimc_module_destroy(struct aimc *imc, unsigned int module_uid)
{
    if (imc->aipc)
        return aipc_send(imc->aipc, 0, "acont_module_destroy",
                         ipc_argtype_int, module_uid, ipc_argtype_end);
    if (ioctl(imc->fd, AUIMC_IO_DESTROY_MODULE, &module_uid) < 0) {
        LOGE(aimc, 0x411, "AUIMC_IO_DESTROY_MODULE - %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/* @0x1e1f4: legacy (ADEC_MODULE_ID based) module */
unsigned int aimc_lmodule_create(struct aimc *imc, unsigned int module_id)
{
    if (imc->aipc)
        return aipc_send(imc->aipc, 0, "acont_lmodule_create",
                         ipc_argtype_int, module_id, ipc_argtype_end);
    if (ioctl(imc->fd, AUIMC_IO_CREATE_LEGACY_MODULE, &module_id) >= 0)
        return module_id;
    LOGE(aimc, 0x426, "AUIMC_IO_CREATE_LEGACY_MODULE - %s\n", strerror(errno));
    return 0;
}

/* @0x1e288 */
int aimc_lmodule_destroy(struct aimc *imc, unsigned int module_id)
{
    if (imc->aipc)
        return aipc_send(imc->aipc, 0, "acont_lmodule_destroy",
                         ipc_argtype_int, module_id, ipc_argtype_end);
    if (ioctl(imc->fd, AUIMC_IO_DESTROY_LEGACY_MODULE, &module_id) < 0) {
        LOGE(aimc, 0x43b, "AUIMC_IO_DESTROY_LEGACY_MODULE - %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/* @0x1e320: out_uid.out_port -> in_uid.in_port */
int aimc_module_connect(struct aimc *imc, unsigned int out_uid, int out_port,
                        unsigned int in_uid, int in_port)
{
    struct auimc_connect c;

    if (imc->aipc)
        return aipc_send(imc->aipc, 0, "acont_module_connect",
                         ipc_argtype_int, out_uid, ipc_argtype_int, out_port,
                         ipc_argtype_int, in_uid, ipc_argtype_int, in_port, ipc_argtype_end);
    c.out_uid = out_uid;
    c.out_port = out_port;
    c.in_uid = in_uid;
    c.in_port = in_port;
    if (ioctl(imc->fd, AUIMC_IO_CONNECT_MODULE, &c) < 0) {
        LOGE(aimc, 0x45a, "AUIMC_IO_CONNECT_MODULE - %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/* @0x1e3c8 */
int aimc_module_disconnect_output(struct aimc *imc, unsigned int uid, int port)
{
    struct auimc_disconnect d = { uid, port };

    if (imc->aipc)
        return aipc_send(imc->aipc, 0, "acont_module_disconnect_output",
                         ipc_argtype_int, uid, ipc_argtype_int, port, ipc_argtype_end);
    if (ioctl(imc->fd, AUIMC_IO_DISCONNECT_MODULE_OUTPUT, &d) < 0) {
        LOGE(aimc, 0x475, "AUIMC_IO_DISCONNECT_MODULE_OUTPUT - %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/* @0x1e460 */
int aimc_module_disconnect_input(struct aimc *imc, unsigned int uid, int port)
{
    struct auimc_disconnect d = { uid, port };

    if (imc->aipc)
        return aipc_send(imc->aipc, 0, "acont_module_disconnect_input",
                         ipc_argtype_int, uid, ipc_argtype_int, port, ipc_argtype_end);
    if (ioctl(imc->fd, AUIMC_IO_DISCONNECT_MODULE_INPUT, &d) < 0) {
        LOGE(aimc, 0x490, "AUIMC_IO_DISCONNECT_MODULE_INPUT - %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/* @0x1e4f8: give module uid a global role name (searchable by other processes) */
int aimc_role_set(struct aimc *imc, unsigned int uid, const char *name)
{
    struct auimc_role role;

    if (imc->aipc)
        return aipc_send(imc->aipc, 0, "acont_role_set",
                         ipc_argtype_int, uid, ipc_argtype_str, name, ipc_argtype_end);
    strncpy(role.name, name, sizeof(role.name) - 1);
    role.name[sizeof(role.name) - 1] = '\0';
    role.uid = uid;
    if (ioctl(imc->fd, AUIMC_IO_SET_ROLE, &role) < 0) {
        LOGE(aimc, 0x4ac, "AUIMC_IO_SET_ROLE - %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/* @0x1e5a4: role name -> uid, 0 if unknown */
unsigned int aimc_role_search(struct aimc *imc, const char *name)
{
    struct auimc_role role;
    int uid;

    int r;

    if (imc->aipc) {
        uid = 0;
        r = aipc_call(imc->aipc, 0, &uid, "acont_role_search",
                      ipc_argtype_str, name, ipc_argtype_end);
        return r < 0 ? r : uid;          /* a negative error leaks out as "uid" */
    }
    role.uid = 0;
    strncpy(role.name, name, sizeof(role.name) - 1);
    role.name[sizeof(role.name) - 1] = '\0';
    if (ioctl(imc->fd, AUIMC_IO_SEARCH_ROLE, &role) >= 0)
        return role.uid;
    LOGE(aimc, 0x4cb, "AUIMC_IO_SET_ROLE - %s\n", strerror(errno));
    return 0;
}

/* @0x1e654: cache maintenance for driver-managed shared buffers only */
int aimc_cache_sync(struct aimc *imc, int to_device, unsigned int shbuf_offset,
                    unsigned int offset, unsigned int size)
{
    struct auimc_cache c;

    if (shbuf_offset - 0xf0000000u >= 0x10000000u)
        return 0;
    c.direction = to_device != 0;
    c.shbuf_offset = shbuf_offset;
    c.offset = offset;
    c.size = size;
    if (ioctl(imc->fd, AUIMC_IO_CACHE_SYNC, &c) < 0) {
        LOGE(aimc, 0x4e8, "AUIMC_IO_CACHE_SYNC - %s\n", strerror(errno));
        return -1;
    }
    return 0;
}
