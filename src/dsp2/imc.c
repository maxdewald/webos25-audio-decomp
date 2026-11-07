/*
 * dsp2 IMC / aimc: inter-processor command transport and dispatch. Every
 * kernel aimc_* request (module create, connect, set-property, ...) arrives
 * here as an IPC header, is matched to a handler by target uid and executed.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Evidence: build/decomp/audio_a0_dsp2.main_60000000.bin.c (0x6006c7a0..0x6006e5a8,
 * 0x60079878..0x6007a7c8), build/scratch/a18/asm/aimc_dispatch.asm, aimc_set_property.asm.
 * Response recovery: aligned scalar listings and validated FLIX branch slots in
 * build/scratch/pr23-imc/. REA rejected the raw image; no REA Evidence was issued.
 * Structures use semantic host pointers; offset comments describe the DSP ABI.
 *
 * Command path (observed):
 *   IPC irq -> aimc_process -> IMC_ProcessHeader(hdr)
 *     hdr type 1 (command) -> aimc_dispatch_command(imc, target_uid, cmd, param, ...)
 *       1. cmd 0xf101002c (shared-buffer status) for a legacy module id < 201:
 *          ConnectionMng_ShBufCmd (connection.c)
 *       2. linear search of the handler list (imc state +0x5c) for target_uid.
 *          Missing uid -> -1, command silently dropped (no log on this path).
 *       3. handler(ctx, cmd, param, len); a response is sent if the sender asked.
 *   Handlers are installed by aimc_register_command_handler:
 *     - Connector_Create            uid proc<<24|2   -> Connector_Cmd (connector.c)
 *     - aud_registry_create_instance_param           -> every aud_module instance
 *       (its cmd -> impl->cmd, e.g. bypass_cmd -> aud_module_default_command)
 *     - ModuleRegistry_Init         registry uid
 *     - ConnectionMng_RegisterModule (via IMC_RegisterCmdProcessor, "type 1" entries)
 *   The kernel's aimc_set_property(uid, fmt, ...) becomes ADEC_CMD_SET_PROP
 *   0x10180004 with {len, "name=val[\name=val]"} and reaches
 *   aud_module_default_command -> aud_module_set_prop (module_core.c @0x600759cc):
 *   split on '=' and '\', exact strcmp per name in impl->props, then the default
 *   table (thread-priority, drop-output-data, drop-output-meta, block-module,
 *   log-mask, log-level, do-chain-profile, chain-profile), then the runtime
 *   prop list. No '-'/'_' folding, no case folding. Unknown names are ignored
 *   with no log on this (multi) path; only ADEC_CMD_SET_PROP_SINGLE 0x101c0004
 *   warns "no such property. name %s, on %s".
 *   dsp2's own aimc_set_property (below) builds the same command for its children.
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define ADEC_CMD_SET_PROP      0x10180004u      /* DAT_6005d338 */
#define CMD_SHBUF_STATUS       0xf101002cu      /* DAT_6005c618 */
#define REG_CMD_ADD_PROCESSOR  0x80010020u      /* DAT_6005e0b4 */
#define REG_CMD_ADD_PORT_BUF   0x8004002cu      /* DAT_6005e0b8 */
#define REG_CMD_ADD_IMPLEMENT  0x80020024u      /* DAT_6005e0bc */
#define REG_CMD_ADD_DONE       0x80030000u      /* DAT_6005e0c0 */

typedef int (*imc_fn)(void *ctx, unsigned cmd, void *a, void *b, int c);
typedef void (*imc_response_fn)(int result, void *param, unsigned len, void *arg);
typedef int (*imc_notification_fn)(void *param, unsigned len, void *arg);
typedef int (*imc_named_fn)(void *arg, void *param, unsigned len);

struct imc_response_link {
    struct imc_response_link *prev, *next;
};

struct imc_notification {                 /* 0x18 bytes on DSP */
    struct imc_response_link link;        /* +0x00/+0x04 */
    int repeat;                          /* +0x08, zero removes before callback */
    imc_notification_fn fn;               /* +0x0c */
    void *arg;                           /* +0x10, borrowed */
    unsigned id;                         /* +0x14 */
};

struct imc_named_event {                  /* 0x40 bytes on DSP */
    struct imc_response_link link;        /* +0x00/+0x04 */
    unsigned id, processor;               /* +0x08/+0x0c; processor 0 = local */
    int once;                            /* +0x10, nonzero removes AFTER callback */
    unsigned uid;                        /* +0x14 */
    char name[32];                       /* +0x18, owned truncated copy */
    imc_named_fn fn;                      /* +0x38 */
    void *arg;                           /* +0x3c, borrowed */
};

struct imc_event_action {                 /* 0x1c-byte registration on DSP */
    unsigned type;                       /* +0x00, only notification 1 recovered */
    int repeat;                          /* +0x04, zero = one shot */
    unsigned target;                     /* +0x08, notification recipient */
    imc_notification_fn fn;               /* +0x0c, borrowed */
    void *arg;                           /* +0x10, borrowed */
    int interval;                        /* +0x14, valid 1..1000, else 1 */
    unsigned remaining;                  /* +0x18, initialized by insertion */
};

struct imc_event_entry {                  /* 0x20 bytes on DSP */
    struct imc_event_action action;
    unsigned id;                         /* +0x1c */
};

struct imc_event_table {                  /* 0x64c bytes on DSP */
    unsigned _0, _4;
    struct imc_event_entry entries[50];   /* +0x08 */
    int count;                           /* +0x648 */
};

struct imc_event_payload {                /* 0x0c bytes on DSP */
    unsigned _0;                         /* not consumed by notification path */
    void *param;                         /* +0x04, borrowed for synchronous send */
    unsigned len;                        /* +0x08 */
};

struct imc_pending_response {             /* 0x20 bytes on DSP */
    struct imc_response_link link;        /* +0x00/+0x04 */
    unsigned _8, _c;
    imc_response_fn fn;                   /* +0x10 */
    void *arg;                           /* +0x14, borrowed */
    unsigned seq;                        /* +0x18 */
    unsigned _1c;
};

struct imc_handler {                     /* 0x14 bytes */
    struct imc_handler *next;
    unsigned uid;                        /* +0x04 */
    imc_fn fn;                           /* +0x08 */
    void *ctx;                           /* +0x0c */
    int  legacy;                         /* +0x10 1 = IMC_RegisterCmdProcessor style */
};

struct imc {                             /* 0x50 bytes */
    struct imc_response_link actions;   /* +0x00 notification callbacks */
    struct imc_response_link responses; /* +0x08 pending responses */
    int   lock[7];                       /* +0x10 */
    struct imc_response_link named;     /* +0x2c named-event registrations */
    unsigned named_id;                  /* +0x34 */
    void *rbtree;                        /* +0x38 named events */
    unsigned seq;                        /* +0x3c */
    unsigned action_id;                 /* +0x40 */
    int   unread;                        /* +0x44 */
    int   platform;                      /* +0x48 */
    void *tracer;                        /* +0x4c */
};

struct aimc_state {                      /* DAT_6005e040 */
    struct imc imc;
    void *resp_param;                    /* +0x50 */
    int   resp_len;                      /* +0x54 */
    int   _58;
    struct imc_handler *handlers;        /* +0x5c */
};

struct imc_cmd {                         /* IMC_SendCommand parameter */
    unsigned cmd;                        /* +0x00 */
    unsigned target;                     /* +0x04 */
    unsigned source;                     /* +0x08 */
    int   want_resp;                     /* +0x0c */
    int   _10;
    imc_response_fn cb;                  /* +0x14 */
    void *cb_arg;                        /* +0x18 */
    int   len;                           /* +0x1c */
    void *param;                         /* +0x20 */
};

struct imc_response {                    /* IMC_Response parameter, 0x28 on DSP */
    unsigned source;                    /* +0x00 response recipient */
    int me, _8;                         /* +0x04/+0x08, unused by IMC_Response */
    unsigned seq;                       /* +0x0c pending response ID */
    unsigned uid;                       /* +0x10 responding command target */
    int one;                            /* +0x14, unused by IMC_Response */
    unsigned cmd;                       /* +0x18 */
    int result;                         /* +0x1c handler result */
    void *param;                        /* +0x20 borrowed payload */
    unsigned len;                       /* +0x24 payload bytes */
};

extern struct aimc_state *aimc;                         /* DAT_6005e040 */
extern int *aimc_handler_lock;                          /* PTR_DAT_6005e0cc */
extern struct { void *local; } *imc_local;              /* DAT_6005d258 (+4) */
extern volatile unsigned *aimc_log_mask;                /* DAT_6005d454 */
extern void *aimc_log_ctx;                              /* PTR_DAT_6005d458 */
extern volatile unsigned *imc_log_mask;                 /* DAT_6005d170 */
extern void *imc_log_ctx;                               /* PTR_DAT_6005d16c */
extern void logmm_printl(void *ctx, int lvl, const char *fn, int line, const char *fmt, ...);
#define ALOG(lvl, ...) \
    do { if (*aimc_log_mask & (1u << (lvl))) \
        logmm_printl(aimc_log_ctx, lvl, __func__, __LINE__, __VA_ARGS__); } while (0)

extern void __assert_failed(const char *fn, int line, const char *expr);
extern void *aud_malloc(unsigned);                      /* _BufMonitor_RemoveBufferIdxLocal */
extern void  aud_free(void *);                          /* _BufMonitor_RemoveBufferIndex */
extern void *calloc(size_t, size_t);
extern void  AuiLogger_Init(void *, int, unsigned);     /* memset */
extern void  BufMonitor_AddBufferRef(void *dst, const char *src, unsigned n); /* strncpy */
extern int   vsnprintf_(char *, unsigned, const char *, va_list); /* @0x600ea9b8 ("IPC_Init" in syms) */
extern int   snprintf(char *, size_t, const char *, ...);
extern void  aud_mutex_init(void *), aud_mutex_lock(void *), aud_mutex_unlock(void *);
extern unsigned irq_raise_level(void);
extern void irq_restore(unsigned);
extern int   aud_who_am_i(void);
extern int   ModuleList_GetPlatform(void);
extern void  ModuleList_SetPlatform(int);
extern int   ModuleList_FindModule(unsigned uid);
extern void *RBTree_CreateTree(void *root, void *cmp);
extern int RBTree_Search(void *, unsigned, struct imc_event_table **);
extern int RBTree_CreateNode(void *, unsigned, struct imc_event_table *);
extern int RBTree_Delete(void *, unsigned);
extern int   IPC_ReadHeaderAddr(void *h, void *out), IPC_ReadHeaderDone(void *h);
extern int   IPC_WriteCommand(void *h, void *msg, void *param, int irq);
extern int   ConnectionMng_ShBufCmd(int id, const int *p);
extern int   IMC_RaiseEvent(struct imc *, int, int, void *);
extern int   IMC_RegisterNamedEvent(void *, const char *, unsigned, int, imc_named_fn, void *);
extern void  IMC_UnregisterNamedEvent(void *, int), IMC_UnregisterNamedEvent_onUID(void *, int);
extern int   IMC_RaiseNamedEvent(void *, int, const char *, void *, int);
extern int   IMC_Response(struct imc *, void *);
extern void  irq_register(int, int, void *, void *);
extern void  HAL_ClearInterrupt(int), HAL_EnableInterrupt(int);
extern struct { char _h[0x40]; int irq_arg; int irq; } *aimc_irq;   /* PTR_DAT_6005e098 */
extern struct { int h[8]; } *aimc_ipc_tab;                           /* PTR_DAT_6005e03c */

/* @0x6006caa4 IMC_Create */
int IMC_Create(struct imc **out, int platform)
{
    int IMC_Init(struct imc *, int);

    *out = aud_malloc(sizeof(struct imc));
    if (!*out)
        return 0x10;
    return IMC_Init(*out, platform);
}

/* @0x6006cac4 IMC_Terminate */
void IMC_Terminate(struct imc *imc)
{
    if (imc)
        aud_free(imc);
}

/* @0x6006cad4 IMC_GetStructSize */
int IMC_GetStructSize(void)
{
    return 0x50;
}

/* @0x6006cadc IMC_Destroy: second symbol named "IMC_Init" in functions.tsv;
 * the body only frees, like IMC_Terminate */
void IMC_Destroy(struct imc *imc)
{
    if (imc)
        aud_free(imc);
}

/* @0x6006caec IMC_Init */
int IMC_Init(struct imc *imc, int platform)
{
    extern void *imc_rbtree_cmp;                       /* PTR_IMC_Init_6005d188 */
    void *r;

    if (!imc)
        __assert_failed("IMC_Init", 0x30a, "_imc != NULL");
    AuiLogger_Init(imc, 0, sizeof(*imc));
    imc->platform = platform ? platform : ModuleList_GetPlatform();
    r = RBTree_CreateTree(&imc->rbtree, imc_rbtree_cmp);
    imc->actions.prev = imc->actions.next = &imc->actions;
    imc->responses.prev = imc->responses.next = &imc->responses;
    imc->named.prev = imc->named.next = &imc->named;
    imc->unread = 0;
    aud_mutex_init(imc->lock);
    return (int)(intptr_t)r;
}

int aimc_dispatch_command(struct imc *imc, unsigned uid, unsigned cmd, void *param, int len,
                          unsigned source, unsigned seq, unsigned flags);

/* @0x6006cbd4 IMC_SendCommand: local-send reconstruction; the remote-send
 * branch remains [FLIX]. Pending entries are prepended before dispatch.
 * Allocation failure still dispatches with want_resp and returns 0, as does
 * handler failure. The response callback/argument are borrowed pointers. */
int IMC_SendCommand(struct imc *imc, const struct imc_cmd *c, unsigned *seq_out)
{
    extern unsigned imc_seq_mask;                      /* DAT_6005d19c */
    unsigned seq;

    if (!imc)
        __assert_failed("IMC_SendCommand", 0x34e, "_imc != NULL");
    if (!c)
        __assert_failed("IMC_SendCommand", 0x34f, "_imcCommandParam != NULL");
    ModuleList_FindModule(c->target);
    seq = imc->seq++ & imc_seq_mask;
    if (seq_out)
        *seq_out = seq;
    if (c->want_resp) {
        unsigned ps = irq_raise_level();
        struct imc_pending_response *pending = aud_malloc(sizeof(*pending));

        if (pending) {
            AuiLogger_Init(pending, 0, sizeof(*pending));
            pending->fn = c->cb;
            pending->arg = c->cb_arg;
            pending->seq = seq;
            pending->link.next = imc->responses.next;
            pending->link.prev = &imc->responses;
            imc->responses.next->prev = &pending->link;
            imc->responses.next = &pending->link;
        } else if (*imc_log_mask & 1) {
            logmm_printl(imc_log_ctx, 0, "_IMC_InsertResponseToList", 0x1aa, "no mem\n");
        }
        irq_restore(ps);
    }
    aimc_dispatch_command(imc, c->target, c->cmd, c->param, c->len, c->source, seq, c->want_resp != 0);
    return 0;
}

int aimc__register_command_handler(void *imc, unsigned uid, imc_fn fn, void *ctx);

/* @0x6006cedc IMC_RegisterCmdProcessor */
int IMC_RegisterCmdProcessor(struct imc *imc, unsigned uid, imc_fn fn, void *ctx)
{
    return -aimc__register_command_handler(imc, uid, fn, ctx);
}

/* @0x6006c94c _IMC_FindAndClearResponse: search newest first by sequence only.
 * Unlink/copy/free the pending entry under IRQ exclusion, then restore PS.
 * The callback argument is borrowed, not freed with the entry. */
static int _IMC_FindAndClearResponse(struct imc *imc, unsigned seq,
                                     struct imc_pending_response *out)
{
    unsigned ps = irq_raise_level();
    struct imc_response_link *link;

    for (link = imc->responses.next; link != &imc->responses; link = link->next) {
        struct imc_pending_response *pending = (struct imc_pending_response *)link;

        if (pending->seq != seq)
            continue;
        if (link->prev) {
            link->prev->next = link->next;
            link->next->prev = link->prev;
        }
        out->link = pending->link;
        out->_8 = pending->_8;
        out->_c = pending->_c;
        out->fn = pending->fn;
        out->arg = pending->arg;
        out->seq = pending->seq;
        aud_free(pending);
        irq_restore(ps);
        return 0;
    }
    if (*imc_log_mask & 1)
        logmm_printl(imc_log_ctx, 0, "_IMC_FindAndClearResponse", 0x270, "not found\n");
    irq_restore(ps);
    return 0x13;
}

int aimc_ipc_send(void *imc, int to, void *msg, void *param);

/* @0x6006cde4 IMC_Response: local callbacks consume a pending entry; remote
 * responses carry {cmd, seq, uid, 2, result, source, 0, len}. Only the remote
 * path asserts len <= 0xfd7 (unsigned); it returns the transport result without
 * taking ownership of param. Local callbacks run after pending-entry free. */
int IMC_Response(struct imc *imc, void *response)
{
    const struct imc_response *resp = response;
    struct imc_pending_response pending;
    int platform, result;

    if (!imc)
        __assert_failed("IMC_Response", 0x36c, "_imc != NULL");
    platform = ModuleList_FindModule(resp->source);
    if (imc->platform != platform) {
        unsigned header[8] = { resp->cmd, resp->seq, resp->uid, 2,
                               (unsigned)resp->result, resp->source, 0, resp->len };

        if (resp->len > 0xfd7u)
            __assert_failed("IMC_Response", 0x392,
                            "_ImcResponseParameter->paramLen < ADEC_CNST_IPC_MAX_PARAM_SIZE - 4");
        return aimc_ipc_send(imc, platform, header, resp->param);
    }
    result = _IMC_FindAndClearResponse(imc, resp->seq, &pending);
    if (result) {
        if (*imc_log_mask & 1)
            logmm_printl(imc_log_ctx, 0, "IMC_Response", 0x376,
                         "Fail to find response (%d)\n", result);
        return result;
    }
    if (pending.fn)
        pending.fn(resp->result, resp->param, resp->len, pending.arg);
    return 0;
}

/* @0x6006c9f4 _IMC_FindAndClearNotification: scalar copy/unlink/free in
 * build/scratch/pr23-next-imc/find-notification.asm. A nonzero repeat keeps
 * the registration. One-shot storage is freed under IRQ exclusion before
 * callback delivery; the copied callback/argument are borrowed. */
static int _IMC_FindAndClearNotification(struct imc *imc, unsigned id,
                                        struct imc_notification *out)
{
    unsigned ps = irq_raise_level();
    struct imc_response_link *link;

    for (link = imc->actions.next; link != &imc->actions; link = link->next) {
        struct imc_notification *notification = (struct imc_notification *)link;

        if (notification->id != id)
            continue;
        *out = *notification;
        if (!notification->repeat) {
            if (link->prev) {
                link->prev->next = link->next;
                link->next->prev = link->prev;
            }
            aud_free(notification);
        }
        irq_restore(ps);
        return 0;
    }
    if (*imc_log_mask & 1)
        logmm_printl(imc_log_ctx, 0, "_IMC_FindAndClearNotification", 0x291, "not found\n");
    irq_restore(ps);
    return 0x13;
}

/* @0x6006c7a0 _IMC_InsertActionToList: numeric-actions.asm. Selective
 * notification action (type 1) only; command action (0) remains [GAP].
 * Clamp interval to 1..1000; initialize countdown to interval. At capacity
 * the image resets count to zero, overwriting registrations, not failing. */
static int _IMC_InsertActionToList(struct imc_event_table *table,
                                   const struct imc_event_action *action, unsigned id)
{
    unsigned ps;
    struct imc_event_entry *entry;

    if (action->type != 1)
        return 0xf;
    ps = irq_raise_level();
    if (table->count > 49)
        table->count = 0;
    entry = &table->entries[table->count];
    entry->action = *action;
    entry->id = id;
    if (action->interval < 1 || action->interval > 1000)
        entry->action.interval = 1;
    entry->action.remaining = (unsigned)entry->action.interval;
    ++table->count;
    irq_restore(ps);
    return 0;
}

/* @0x6006c88c _IMC_DeleteActionFromList: numeric-actions.asm, including
 * scalar loopgtz omitted by Ghidra. Search ALL entries by ID, shift the
 * tail on deletion. Only notification entries are recovered here. */
static int _IMC_DeleteActionFromList(struct imc_event_table *table, unsigned id)
{
    unsigned ps = irq_raise_level();

    if (table->count > 49) {
        irq_restore(ps);
        return 0x1f;
    }
    for (int index = 0; index < table->count; ++index) {
        if (table->entries[index].id != id)
            continue;
        if (table->entries[index].action.type != 1) {
            irq_restore(ps);
            return 0xf;
        }
        --table->count;
        memmove(table->entries + index, table->entries + index + 1,
                 (size_t)(table->count - index) * sizeof(*table->entries));
        irq_restore(ps);
        return 0;
    }
    irq_restore(ps);
    return 0x13;
}

/* @0x6006cef4 IMC_RegisterEvent: numeric-register.asm. Notification-only
 * recovery: ID = (counter & 0xfffff) | ((platform & 15) << 20), skipping
 * live callback IDs. Callback storage precedes publisher registration and
 * survives publisher/IPC failure. No rollback, and fn/arg remain borrowed.
 * [GAP] command-action registration is explicitly rejected with 0xf. */
int IMC_RegisterEvent(struct imc *imc, unsigned event, unsigned uid, unsigned *id,
                       const struct imc_event_action *action)
{
    struct imc_event_table *table = NULL;
    struct imc_response_link *link;
    struct imc_notification *notification;
    unsigned ps, key = (event << 8) | (uid & 0xff);
    int platform, result;

    if (!imc)
        __assert_failed("IMC_RegisterEvent", 0x3b1, "_imc != NULL");
    if (!action)
        __assert_failed("IMC_RegisterEvent", 0x3b2, "_imcActionParam != NULL");
    if (action->type != 1)
        return 0xf;
    platform = ModuleList_FindModule(uid);
    ps = irq_raise_level();
    do {
        *id = (imc->action_id++ & 0xfffffu) | (((unsigned)imc->platform & 15u) << 20);
        for (link = imc->actions.next; link != &imc->actions; link = link->next)
            if (((struct imc_notification *)link)->id == *id)
                break;
    } while (link != &imc->actions);
    irq_restore(ps);
    ps = irq_raise_level();
    notification = aud_malloc(sizeof(*notification));
    if (!notification) {
        irq_restore(ps);
        __assert_failed("IMC_RegisterEvent", 0x3d7, "result == ADEC_ERR_NONE");
        return 0x10;
    }
    AuiLogger_Init(notification, 0, sizeof(*notification));
    notification->repeat = action->repeat;
    notification->fn = action->fn;
    notification->arg = action->arg;
    notification->id = *id;
    notification->link.next = imc->actions.next;
    notification->link.prev = &imc->actions;
    imc->actions.next->prev = &notification->link;
    imc->actions.next = &notification->link;
    irq_restore(ps);
    if (platform != imc->platform) {
        unsigned header[8] = {0, *id, action->target, 3,
            2u | (((unsigned)action->repeat & 1u) << 2), (unsigned)action->interval, key, 0};

        if ((unsigned)action->repeat & ~1u)
            __assert_failed("_IMC_RegisterRemoteAction", 0x15a,
                             "(_imcActionParam->repeatType & ~1) == 0");
        return aimc_ipc_send(imc, platform, header, NULL);
    }
    result = RBTree_Search(imc->rbtree, key, &table);
    if (result == 0x13) {
        table = aud_malloc(sizeof(*table));
        if (!table)
            return 0x10;
        AuiLogger_Init(table, 0, sizeof(*table));
        RBTree_CreateNode(imc->rbtree, key, table);
    }
    return _IMC_InsertActionToList(table, action, *id);
}

/* @0x6006d1c0 IMC_CancelEvent: numeric-cancel.asm. Remove/free callback
 * first under IRQ exclusion even if publisher cancellation later fails.
 * Missing callback still attempts publisher cancellation. Tree deletion
 * owns table destruction; no callback argument or payload is freed here. */
int IMC_CancelEvent(struct imc *imc, unsigned event, unsigned uid, unsigned id)
{
    struct imc_event_table *table = NULL;
    struct imc_response_link *link;
    unsigned ps, key = (event << 8) | (uid & 0xff);
    int platform, result;

    if (!imc)
        __assert_failed("IMC_CancelEvent", 0x3f6, "_imc != NULL");
    ps = irq_raise_level();
    for (link = imc->actions.next; link != &imc->actions; link = link->next) {
        struct imc_notification *notification = (struct imc_notification *)link;

        if (notification->id != id)
            continue;
        if (link->prev) {
            link->prev->next = link->next;
            link->next->prev = link->prev;
        }
        aud_free(notification);
        break;
    }
    irq_restore(ps);
    platform = ModuleList_FindModule(uid);
    if (platform != imc->platform) {
        /* [GAP] unused remote header words are uninitialized in the image. */
        unsigned header[8] = {4, id, 0, 4, 0, 0, key, 0};

        return aimc_ipc_send(imc, platform, header, NULL);
    }
    result = RBTree_Search(imc->rbtree, key, &table);
    if (result) {
        if (result != 0x13)
            __assert_failed("_IMC_CancelLocalAction", 0x17d,
                             "result == ADEC_ERR_NOT_FOUND");
        return result;
    }
    result = _IMC_DeleteActionFromList(table, id);
    if (result == 2 || !table->count)
        result = RBTree_Delete(imc->rbtree, key);
    return result;
}

/* @0x6006d318 IMC_RaiseEvent: numeric-raise.asm plus aligned
 * numeric-remote-notification.asm. Notification type 1 only; [GAP] command
 * actions return 0xf rather than fake success. Key is event<<8 | uid&255.
 * Deliver when remaining==interval, then decrement/reset; one-shot actions
 * are removed after this operation. Payload is a borrowed {unused,ptr,len},
 * NOT an inline PTS word. Local callbacks execute after IRQ restore; raising
 * itself holds no mutex/IRQ exclusion. Callback-driven publisher mutation
 * is not made safe by this reconstruction. Remote send errors are ignored. */
int IMC_RaiseEvent(struct imc *imc, int event, int uid, void *payload)
{
    const struct imc_event_payload *param = payload;
    struct imc_event_table *table = NULL;
    unsigned key = ((unsigned)event << 8) | ((unsigned)uid & 0xff);
    int result;

    if (!imc)
        __assert_failed("IMC_RaiseEvent", 0x42e, "_imc != NULL");
    result = RBTree_Search(imc->rbtree, key, &table);
    if (!table || result)
        return result;
    if (table->count > 50) {
        for (;;) {
        }
    }
    for (int index = 0; index < table->count; ++index) {
        struct imc_event_entry *entry = table->entries + index;
        struct imc_event_action *action = &entry->action;

        if (action->type != 1)
            return 0xf;
        if (action->remaining == (unsigned)action->interval) {
            int platform = ModuleList_FindModule(action->target);

            if (platform == imc->platform) {
                struct imc_notification notification;

                if (!_IMC_FindAndClearNotification(imc, entry->id, &notification) && notification.fn)
                    notification.fn(param->param, param->len, notification.arg);
            } else {
                unsigned header[8] = {5, entry->id, (unsigned)uid, 5, 5,
                                       action->target, 0, param->len};

                aimc_ipc_send(imc, platform, header, param->param);
            }
        }
        if (!--action->remaining)
            action->remaining = (unsigned)action->interval;
        if (!action->repeat) {
            --table->count;
            memmove(entry, entry + 1, (size_t)(table->count - index) * sizeof(*entry));
            --index;
        }
    }
    if (!table->count)
        return RBTree_Delete(imc->rbtree, key);
    return result;
}

/* @0x6006d598 add_named_event: named-registration.asm, scalar stores and
 * strncpy(32), then name[31]=0. The IMC owns the entry/name, not fn or arg.
 * Callers hold imc->lock; newest registrations are searched first. */
static struct imc_named_event *add_named_event(struct imc *imc, unsigned id, int once,
                                               const char *name, unsigned uid,
                                               unsigned processor, imc_named_fn fn, void *arg)
{
    struct imc_named_event *event = aud_malloc(sizeof(*event));

    if (!event) {
        if (*imc_log_mask & 1)
            logmm_printl(imc_log_ctx, 0, "add_named_event", 0x49f, "no mem\n");
        return NULL;
    }
    AuiLogger_Init(event, 0, sizeof(*event));
    event->id = id;
    event->processor = processor;
    event->uid = uid;
    BufMonitor_AddBufferRef(event->name, name, sizeof(event->name));
    event->name[31] = 0;
    event->once = once;
    event->fn = fn;
    event->arg = arg;
    event->link.next = imc->named.next;
    event->link.prev = &imc->named;
    imc->named.next->prev = &event->link;
    imc->named.next = &event->link;
    return event;
}

static void remove_named_event(struct imc_named_event *event)
{
    if (event->link.prev) {
        event->link.prev->next = event->link.next;
        event->link.next->prev = event->link.prev;
    }
    aud_free(event);
}

/* @0x6006d64c IMC_RegisterNamedEvent: named-registration.asm. Increment the
 * signed 31-bit ID (negative wraps to 1), retain entry even on IPC failure.
 * Remote type 6: {0,0,platform,6,once!=0,uid,id,32}, owned name as payload. */
int IMC_RegisterNamedEvent(void *info, const char *name, unsigned uid, int once,
                            imc_named_fn fn, void *arg)
{
    struct imc *imc = info;
    struct imc_named_event *event;
    unsigned id;

    aud_mutex_lock(imc->lock);
    id = imc->named_id + 1;
    if ((int32_t)id < 0)
        id = 1;
    imc->named_id = id;
    event = add_named_event(imc, id, once, name, uid, 0, fn, arg);
    if (!event) {
        aud_mutex_unlock(imc->lock);
        return -1;
    }
    if ((unsigned)imc->platform != uid >> 24) {
        unsigned header[8] = {0, 0, (unsigned)imc->platform, 6, once != 0, uid, id, 32};

        aimc_ipc_send(imc, uid >> 24, header, event->name);
    }
    aud_mutex_unlock(imc->lock);
    return (int)id;
}

/* @0x6006d6e4 IMC_UnregisterNamedEvent: named-registration.asm. First ID
 * match, optional type-7 IPC, then free under the mutex regardless of send
 * result. A missing ID leaves the list untouched. */
void IMC_UnregisterNamedEvent(void *info, int id)
{
    struct imc *imc = info;
    struct imc_response_link *link;

    aud_mutex_lock(imc->lock);
    for (link = imc->named.next; link != &imc->named; link = link->next) {
        struct imc_named_event *event = (struct imc_named_event *)link;

        if (event->id != (unsigned)id)
            continue;
        if ((unsigned)imc->platform != event->uid >> 24) {
            unsigned header[8] = {0, 0, (unsigned)imc->platform, 7, 0, event->uid,
                                   (unsigned)id, 0};

            aimc_ipc_send(imc, event->uid >> 24, header, NULL);
        }
        remove_named_event(event);
        aud_mutex_unlock(imc->lock);
        return;
    }
    if (*imc_log_mask & 1)
        logmm_printl(imc_log_ctx, 0, "IMC_UnregisterNamedEvent", 0x4fc,
                     "unknown event id. %x\n", id);
    aud_mutex_unlock(imc->lock);
}

/* @0x6006d7b4 IMC_UnregisterNamedEvent_no_ipc: named-registration.asm.
 * Remove only the newest ID match; never notify the remote publisher. */
void IMC_UnregisterNamedEvent_no_ipc(void *info, int id)
{
    struct imc *imc = info;
    struct imc_response_link *link;

    aud_mutex_lock(imc->lock);
    for (link = imc->named.next; link != &imc->named; link = link->next) {
        struct imc_named_event *event = (struct imc_named_event *)link;

        if (event->id == (unsigned)id) {
            remove_named_event(event);
            aud_mutex_unlock(imc->lock);
            return;
        }
    }
    if (*imc_log_mask & 1)
        logmm_printl(imc_log_ctx, 0, "IMC_UnregisterNamedEvent_no_ipc", 0x529,
                     "unknown event id. %x\n", id);
    aud_mutex_unlock(imc->lock);
}

/* @0x6006d844 IMC_UnregisterNamedEvent_onUID: named-clear-uid.asm.
 * Remove ALL matching UIDs under the mutex, including remote registrations,
 * without IPC. This local cleanup is not a remote-unregister acknowledgement. */
void IMC_UnregisterNamedEvent_onUID(void *info, int uid)
{
    struct imc *imc = info;
    struct imc_response_link *link;

    aud_mutex_lock(imc->lock);
    link = imc->named.next;
    while (link != &imc->named) {
        struct imc_named_event *event = (struct imc_named_event *)link;

        link = link->next;
        if (event->uid == (unsigned)uid)
            remove_named_event(event);
    }
    aud_mutex_unlock(imc->lock);
}

/* @0x6006d8d0 IMC_RaiseNamedEvent: named-raise.asm. Exact uid AND name;
 * newest-first, callbacks and sends under the mutex with borrowed payload.
 * Local callback(arg,param,len), or type 8 {0,0,uid,8,0,peer<<24,id,len}.
 * Remove once AFTER delivery, even on transport error; always return 0.
 * XOS same-thread locks recurse (recursive-mutex.asm), so nested delivery
 * can see this entry before once removal. Self-unregister frees an entry
 * this function still uses after callback: not made safe here. This reports
 * the raise operation, not downstream drain. */
int IMC_RaiseNamedEvent(void *info, int uid, const char *name, void *param, int len)
{
    struct imc *imc = info;
    struct imc_response_link *link;

    aud_mutex_lock(imc->lock);
    link = imc->named.next;
    while (link != &imc->named) {
        struct imc_named_event *event = (struct imc_named_event *)link;

        link = link->next;
        if (event->uid != (unsigned)uid || strcmp(event->name, name))
            continue;
        if (!event->processor)
            event->fn(event->arg, param, (unsigned)len);
        else {
            unsigned header[8] = {0, 0, (unsigned)uid, 8, 0, event->processor << 24,
                                   event->id, (unsigned)len};

            aimc_ipc_send(imc, event->processor, header, param);
        }
        if (event->once)
            remove_named_event(event);
    }
    aud_mutex_unlock(imc->lock);
    return 0;
}

/* @0x6006da08 IMC_ProcessHeader: type 2 @0x6006dae8 consumes hdr[1].
 * Callback(result=hdr[4], borrowed hdr+8, bytes=hdr[7], borrowed argument)
 * runs after entry free and IRQ restore; its return value is ignored. No
 * command/source/target check or payload-length validation occurs here.
 * IPC storage remains owned by the reader until aimc_ipc_header_done. This
 * response alone does not establish asynchronous FIRST/data completion.
 * Type 5 @0x6006db20 selects notification id hdr[1], ignoring other routing
 * fields, and calls fn(borrowed hdr+8, hdr[7], borrowed arg) outside IRQ
 * exclusion. Null callbacks succeed; missing ids return 0x13. Evidence:
 * build/scratch/pr23-next-imc/{header-notification,notification-errors}.asm.
 * Types 6/7/8: named-{add-header,remove,delivery,delivery-tail}.asm. Add
 * owns a name copy; remove matches (processor,id), delivery (uid,id).
 * Named callbacks run with the recursive lock held and remove once AFTER
 * callback; self-unregister leaves a post-callback dangling entry. */
int IMC_ProcessHeader(struct imc *imc, const unsigned *hdr)
{
    if (*imc_log_mask & 0x20)
        logmm_printl(imc_log_ctx, 5, "IMC_ProcessHeader", 0x698,
                     "cmd %x, target %x, source %x\n", hdr[0], hdr[5], hdr[2]);
    if (hdr[3] == 1) {
        ModuleList_FindModule(hdr[2]);
        return -aimc_dispatch_command(imc, hdr[5], hdr[0], (void *)(hdr + 8), (int)hdr[7], hdr[2],
                                      hdr[1], hdr[4] & 1);
    }
    if (hdr[3] == 2) {
        struct imc_pending_response pending;
        int result = _IMC_FindAndClearResponse(imc, hdr[1], &pending);

        if (result) {
            if (*imc_log_mask & 1)
                logmm_printl(imc_log_ctx, 0, "process_response", 0x607,
                             "Fail to find response (%d)\n", result);
            return result;
        }
        if (pending.fn)
            pending.fn((int32_t)hdr[4], (void *)(hdr + 8), hdr[7], pending.arg);
        return 0;
    }
    if (hdr[3] == 5) {
        struct imc_notification notification;
        int result = _IMC_FindAndClearNotification(imc, hdr[1], &notification);

        if (result) {
            if (*imc_log_mask & 1)
                logmm_printl(imc_log_ctx, 0, "process_notification", 0x621,
                             "Fail to find notification (%d)\n", result);
            return result;
        }
        if (notification.fn)
            notification.fn((void *)(hdr + 8), hdr[7], notification.arg);
        else if (*imc_log_mask & 1)
            logmm_printl(imc_log_ctx, 0, "process_notification", 0x62a,
                         "null noti function\n");
        return 0;
    }
    if (hdr[3] == 6) {
        aud_mutex_lock(imc->lock);
        add_named_event(imc, hdr[6], hdr[4] & 1, (const char *)(hdr + 8),
                         hdr[5], hdr[2], NULL, NULL);
        aud_mutex_unlock(imc->lock);
        return 0;
    }
    if (hdr[3] == 7 || hdr[3] == 8) {
        struct imc_response_link *link;

        aud_mutex_lock(imc->lock);
        for (link = imc->named.next; link != &imc->named; link = link->next) {
            struct imc_named_event *event = (struct imc_named_event *)link;

            if (event->id != hdr[6] ||
                (hdr[3] == 7 ? event->processor : event->uid) != hdr[2])
                continue;
            if (hdr[3] == 8)
                event->fn(event->arg, (void *)(hdr + 8), hdr[7]);
            if (hdr[3] == 7 || event->once)
                remove_named_event(event);
            aud_mutex_unlock(imc->lock);
            return 0;
        }
        aud_mutex_unlock(imc->lock);
        return hdr[3] == 7 ? 1 : 0;
    }
    if (hdr[3] == 3) {
        struct imc_event_table *table = NULL;
        struct imc_event_action action = {.type = (hdr[4] >> 1) & 1,
            .repeat = (hdr[4] >> 2) & 1, .target = hdr[2], .interval = (int)hdr[5]};
        int result;

        if (action.type != 1)
            return 0xf;                /* [GAP] command actions */
        result = RBTree_Search(imc->rbtree, hdr[6], &table);
        if (result == 0x13) {
            table = aud_malloc(sizeof(*table));
            if (!table)
                return 0x10;
            AuiLogger_Init(table, 0, sizeof(*table));
            result = RBTree_CreateNode(imc->rbtree, hdr[6], table);
            if (result) {
                aud_free(table);
                return result;
            }
        }
        result = _IMC_InsertActionToList(table, &action, hdr[1]);
        return result == 0xf ? result : 0;
    }
    if (hdr[3] == 4) {
        struct imc_event_table *table = NULL;
        int result = RBTree_Search(imc->rbtree, hdr[6], &table);

        if (result == 0x13)
            return result;
        if (result)
            __assert_failed("process_cancel", 0x684, "result == ADEC_ERR_NONE");
        result = _IMC_DeleteActionFromList(table, hdr[1]);
        if (result == 2 || !table->count)
            RBTree_Delete(imc->rbtree, hdr[6]);
        return result == 0xf ? result : 0;
    }
    return 0xf;
}

/* @0x6006e0d8 IMC_SetLocalImc */
int IMC_SetLocalImc(struct imc *imc)
{
    imc_local->local = imc;
    return 0;
}

/* @0x6006e0e4 IMC_GetLocalImc: lazily create when asked */
struct imc *IMC_GetLocalImc(int create)
{
    extern struct imc **imc_local_slot;                /* DAT_6005d2b8 */

    if (!imc_local->local && create) {
        IMC_Create(imc_local_slot, ModuleList_GetPlatform());
        return imc_local->local;
    }
    return imc_local->local;
}

/* @0x6006e108 IMC_SetImcTracer */
void IMC_SetImcTracer(struct imc *imc, void *tracer)
{
    if (imc)
        imc->tracer = tracer;
}

/* @0x6006e114 IMC_GetIPCUnreadCounter */
int IMC_GetIPCUnreadCounter(struct imc *imc)
{
    return imc ? imc->unread : 0;
}

int aimc_send_command(void *imc, unsigned cmd, unsigned source, unsigned target, void *param, int len);

/* @0x6006e4d0 aimc_set_property: printf into {u32 len; char str[len+1]} and
 * send ADEC_CMD_SET_PROP to <uid>. Same wire format as the kernel side. */
int aimc_set_property(void *imc, unsigned uid, const char *fmt, ...)
{
    va_list ap;
    int n, *msg;

    va_start(ap, fmt);
    n = vsnprintf_(NULL, 0, fmt, ap);
    va_end(ap);
    msg = aud_malloc((unsigned)n + 5);
    if (!msg) {
        ALOG(0, "no mem. %s\n", fmt);
        return -1;
    }
    msg[0] = n;
    va_start(ap, fmt);
    vsnprintf_((char *)(msg + 1), (unsigned)n + 1, fmt, ap);
    va_end(ap);
    ALOG(3, "'%08x' len %d, prop '%s'\n", uid, n, (char *)(msg + 1));
    aimc_send_command(imc, ADEC_CMD_SET_PROP, 0, uid, msg, n + 5);
    aud_free(msg);
    return 0;
}

/* @0x60079878 aimc_create: one IPC channel per peer processor, then IMC init */
void aimc_create(void)
{
    extern void AddrTrans_Init(void), HAL_EnableInterruptToArm(void);
    extern int IPC_GetStructSize(void);
    extern void IPC_Init(void *, ...);
    extern void IPC_SetInterrupt(void *, int, unsigned);
    int me = aud_who_am_i();

    AddrTrans_Init();
    (void)IPC_GetStructSize;
    HAL_EnableInterruptToArm();
    /* 11 channel descriptors (0x20 each) of processor <me>: allocate an IPC
     * object for every configured slot, set the doorbell interrupt for the
     * 0/1/2/3 reply slots; details are register arithmetic, see decomp. */
    for (int i = 0; i < 11; i++) {
        int *d = aimc_ipc_tab[me * 11 + i].h;
        if (d[0]) {
            void *ipc = calloc(1, (size_t)IPC_GetStructSize());
            IPC_Init(ipc);
            if (d[6])
                IPC_SetInterrupt(ipc, d[6], (unsigned)me << 16);
            d[7] = (int)(intptr_t)ipc;
        }
    }
    ModuleList_SetPlatform(me + 2);
    IMC_Init(&aimc->imc, me + 2);
    IMC_SetLocalImc(&aimc->imc);
}

void *aimc_get_handle(int me, int from, int reply);
int aimc_ipc_header_get(void *imc, int from, void *hdr);

/* @0x60079efc aimc_start */
int aimc_start(void)
{
    irq_register(aimc_irq->irq, aimc_irq->irq_arg, (void *)aimc_ipc_header_get, &aimc_irq->irq_arg);
    HAL_ClearInterrupt(aimc_irq->irq);
    HAL_EnableInterrupt(aimc_irq->irq);
    return 0;
}

/* @0x60079f4c aimc_processor_id */
int aimc_processor_id(void)
{
    return aud_who_am_i() + 2;
}

/* @0x60079f5c aimc_get_handle: channel slot of (me, peer processor id) */
void *aimc_get_handle(int me, int from, int reply)
{
    int slot;

    switch (from) {
    case 2: slot = reply ? 3 : 2; break;
    case 3: slot = reply ? 5 : 4; break;
    case 4: slot = reply ? 7 : 6; break;
    default: slot = (from == 5 ? 8 : 0) + (reply ? 1 : 0); break;
    }
    return (void *)(intptr_t)aimc_ipc_tab[me * 11 + slot].h[7];
}

/* @0x60079fe0 aimc_ipc_header_get (the second symbol of that name; 0x60079c2c
 * is the irq-time variant and is not reconstructed) */
int aimc_ipc_header_get(void *imc, int from, void *hdr)
{
    int me = aud_who_am_i();
    void *h = aimc_get_handle(me, from, 0);

    (void)imc;
    if (h)
        return -IPC_ReadHeaderAddr(h, hdr);
    ALOG(0, "no handle dsp num %d from %d\n", me, from);
    return -1;
}

/* @0x6007a02c aimc_ipc_header_done */
int aimc_ipc_header_done(void *imc, int from)
{
    void *h = aimc_get_handle(aud_who_am_i(), from, 0);

    (void)imc;
    if (h)
        return -IPC_ReadHeaderDone(h);
    ALOG(0, "no handle\n");
    return -1;
}

/* @0x6007a070 aimc_process: drain the ARM channel, then the DSP peers */
int aimc_process(void)
{
    int me = aud_who_am_i(), r;
    void *hdr;

    while ((r = aimc_ipc_header_get(aimc, 1, &hdr)) >= 0) {
        IMC_ProcessHeader(&aimc->imc, hdr);
        aimc_ipc_header_done(aimc, 1);
    }
    if (me == 2) {
        for (int from = 2; from < 5; from++) {
            if (from == 4)
                continue;
            while ((r = aimc_ipc_header_get(aimc, from, &hdr)) >= 0) {
                IMC_ProcessHeader(&aimc->imc, hdr);
                aimc_ipc_header_done(aimc, from);
            }
        }
    } else if (me == 0 || me == 1 || me == 3) {
        while ((r = aimc_ipc_header_get(aimc, 4, &hdr)) >= 0) {
            IMC_ProcessHeader(&aimc->imc, hdr);
            aimc_ipc_header_done(aimc, 4);
        }
    }
    return r;
}

/* @0x6007a100 aimc_send_command */
int aimc_send_command(void *imc, unsigned cmd, unsigned source, unsigned target, void *param, int len)
{
    struct imc_cmd c = { .cmd = cmd, .target = target, .source = source, .len = len, .param = param };
    unsigned seq;

    (void)imc;
    return -IMC_SendCommand(&aimc->imc, &c, &seq);
}

int aimc_get_processor_id(void *imc);

/* @0x6007a12c aimc_add_processor */
int aimc_add_processor(void *imc)
{
    char name[0x20] = {0};

    snprintf(name, sizeof(name), "dsp%d", aimc_get_processor_id(imc) - 2);
    return aimc_send_command(imc, REG_CMD_ADD_PROCESSOR, (unsigned)aimc_get_processor_id(imc) << 24 | 1,
                             1, name, sizeof(name));
}

/* @0x6007a178 aimc_add_port_buf_def */
int aimc_add_port_buf_def(void *imc, const char *name, int a, int b, int c)
{
    struct { char name[0x20]; int a, b, c; } m = {{0}, 0, 0, 0};

    BufMonitor_AddBufferRef(m.name, name, 0x1f);
    m.name[0x1f] = 0;
    m.a = a;
    m.b = b;
    m.c = c;
    return aimc_send_command(imc, REG_CMD_ADD_PORT_BUF, (unsigned)aimc_get_processor_id(imc) << 24 | 1,
                             1, &m, 0x2c);
}

/* @0x6007a1cc aimc_add_implement */
int aimc_add_implement(void *imc, const char *name, int flags)
{
    struct { char name[0x20]; int flags; } m;

    BufMonitor_AddBufferRef(m.name, name, 0x1f);
    m.name[0x1f] = 0;
    m.flags = flags;
    return aimc_send_command(imc, REG_CMD_ADD_IMPLEMENT, (unsigned)aimc_get_processor_id(imc) << 24 | 1,
                             1, &m, 0x24);
}

/* @0x6007a204 aimc_add_processor_done */
int aimc_add_processor_done(void *imc)
{
    return aimc_send_command(imc, REG_CMD_ADD_DONE, (unsigned)aimc_get_processor_id(imc) << 24 | 1,
                             1, NULL, 0);
}

/* @0x6007a228 aimc_raise_event */
int aimc_raise_event(void *imc, int a, int b, int c, int d)
{
    int ev[3] = { 1, c, d };

    (void)imc;
    return -IMC_RaiseEvent(&aimc->imc, a, b, ev);
}

/* @0x6007a248 aimc_register_named_event */
int aimc_register_named_event(void *imc, int name, int uid, int once, int cb, int arg)
{
    int r = IMC_RegisterNamedEvent(imc, (const char *)(intptr_t)name, uid, once == 0,
                                   (imc_named_fn)(intptr_t)cb, (void *)(intptr_t)arg);

    if (r == 0)
        ALOG(0, "cannot register event. %s\n", (const char *)(intptr_t)name);
    return r;
}

/* @0x6007a288 aimc_unregister_named_event */
void aimc_unregister_named_event(void *imc, int id)
{
    IMC_UnregisterNamedEvent(imc, id);
}

/* @0x6007a298 aimc_unregister_named_event2 (same body) */
void aimc_unregister_named_event2(void *imc, int id)
{
    IMC_UnregisterNamedEvent(imc, id);
}

/* @0x6007a2a8 aimc_clear_named_event_on_uid */
void aimc_clear_named_event_on_uid(void *imc, int uid)
{
    (void)imc;
    IMC_UnregisterNamedEvent_onUID(aimc, uid);
}

/* @0x6007a2b8 aimc_raise_named_event */
int aimc_raise_named_event(void *imc, int uid, const char *name, void *p, int n)
{
    return IMC_RaiseNamedEvent(imc, uid, name, p, n);
}

static struct imc_handler *find_handler(unsigned uid)
{
    struct imc_handler *h;

    for (h = aimc->handlers; h && h->uid != uid; h = h->next)
        ;
    return h;
}

/* @0x6007a2d0 aimc_register_command_handler: add or replace (uid -> fn, ctx) */
int aimc_register_command_handler(void *imc, unsigned uid, imc_fn fn, void *ctx)
{
    struct imc_handler *h;
    int ret = 0;

    (void)imc;
    aud_mutex_lock(aimc_handler_lock);
    h = find_handler(uid);
    if (!h) {
        h = calloc(1, sizeof(*h));
        if (h) {
            h->uid = uid;
            h->next = aimc->handlers;
            aimc->handlers = h;
            h->legacy = 0;
        }
    }
    if (h) {
        h->fn = fn;
        h->ctx = ctx;
    } else {
        ALOG(0, "Oops\n");
        ret = -1;
    }
    aud_mutex_unlock(aimc_handler_lock);
    return ret;
}

/* @0x6007a34c aimc_unregister_command_handler */
void aimc_unregister_command_handler(void *imc, unsigned uid)
{
    struct imc_handler *h, *prev = NULL;

    (void)imc;
    aud_mutex_lock(aimc_handler_lock);
    for (h = aimc->handlers; h; prev = h, h = h->next) {
        if (h->uid != uid)
            continue;
        if (prev)
            prev->next = h->next;
        else
            aimc->handlers = h->next;
        aud_free(h);
        aud_mutex_unlock(aimc_handler_lock);
        return;
    }
    ALOG(0, "no such uid. %x\n", uid);
    aud_mutex_unlock(aimc_handler_lock);
}

/* @0x6007a3b0 aimc_wait_until_process_header */
void aimc_wait_until_process_header(void)
{
}

/* @0x6007a3b8 aimc__register_command_handler: legacy (IMC_RegisterCmdProcessor)
 * flavour, marks the entry legacy=1 when created */
int aimc__register_command_handler(void *imc, unsigned uid, imc_fn fn, void *ctx)
{
    struct imc_handler *h;
    int ret = 0;

    (void)imc;
    aud_mutex_lock(aimc_handler_lock);
    h = find_handler(uid);
    if (!h) {
        h = calloc(1, sizeof(*h));
        if (!h) {
            ALOG(0, "Oops\n");
            ret = -1;
            goto out;
        }
        h->uid = uid;
        h->legacy = 1;
        h->next = aimc->handlers;
        aimc->handlers = h;
    }
    h->fn = fn;
    h->ctx = ctx;
out:
    aud_mutex_unlock(aimc_handler_lock);
    return ret;
}

/* @0x6007a434 aimc__unregister_command_handler */
void aimc__unregister_command_handler(void *imc, unsigned uid)
{
    aimc_unregister_command_handler(imc, uid);
}

/* @0x6007a440 aimc_response_param: payload returned with the next response */
void aimc_response_param(void *param, int len)
{
    aimc->resp_len = len;
    aimc->resp_param = param;
}

/* @0x6007a450 aimc_dispatch_command: normal handlers respond only after fn
 * returns, with its result and optional payload. IMC_Response's error is
 * ignored; the payload is freed and cleared even when sending fails. This
 * acknowledges handler return, not asynchronous FIRST handling or data flow.
 * Legacy handlers receive the request and control their own responses. */
int aimc_dispatch_command(struct imc *imc, unsigned uid, unsigned cmd, void *param, int len,
                          unsigned source, unsigned seq, unsigned flags)
{
    struct imc_handler *h;
    imc_fn fn;
    void *ctx;
    int legacy, r;

    if (cmd == CMD_SHBUF_STATUS && uid < 201 && ConnectionMng_ShBufCmd((int)uid, param) >= 0)
        return 0;

    aud_mutex_lock(aimc_handler_lock);
    h = find_handler(uid);
    if (!h) {
        aud_mutex_unlock(aimc_handler_lock);           /* unknown uid: silently -1 [FLIX] log guard */
        return -1;
    }
    fn = h->fn;
    ctx = h->ctx;
    legacy = h->legacy;
    aud_mutex_unlock(aimc_handler_lock);

    if (!legacy) {
        aimc_response_param(NULL, 0);
        r = fn(ctx, cmd, param, (void *)(intptr_t)len, 0);
        if (flags & 1) {
            struct imc_response resp = { source, aud_who_am_i() + 2, 0, seq, uid, 1, cmd, r,
                                         aimc->resp_param, (unsigned)aimc->resp_len };
            IMC_Response(imc, &resp);
        }
        if (aimc->resp_param)
            aud_free(aimc->resp_param);
        aimc_response_param(NULL, 0);
        return r;
    }
    /* legacy CmdProcessor: fn(ctx, cmd, &{source, seq, want_resp}, param, len)
     * under the module mutex at ctx+0x84 */
    if (!fn)
        return -1;
    {
        struct { unsigned source; unsigned want; unsigned seq; } rq = { source, flags & 1, seq };

        if (ctx)
            aud_mutex_lock((char *)ctx + 0x84);
        r = fn(ctx, cmd, &rq, param, len);
        if (ctx)
            aud_mutex_unlock((char *)ctx + 0x84);
    }
    return -r;
}

/* @0x6007a6b4 aimc_ipc_send: param is a payload pointer, length is msg[7]. */
int aimc_ipc_send(void *imc, int to, void *msg, void *param)
{
    int me = aud_who_am_i();
    void *h = aimc_get_handle(me, to, 1);

    (void)imc;
    if (h)
        return -IPC_WriteCommand(h, msg, param, 1);
    ALOG(0, "no handle dsp num %d from %d\n", me, to);
    return -1;
}

/* @0x6007a704 aimc_get_processor_id */
int aimc_get_processor_id(void *imc)
{
    (void)imc;
    return aud_who_am_i() + 2;
}
