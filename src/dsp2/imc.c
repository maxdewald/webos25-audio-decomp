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

#define ADEC_CMD_SET_PROP      0x10180004u      /* DAT_6005d338 */
#define CMD_SHBUF_STATUS       0xf101002cu      /* DAT_6005c618 */
#define REG_CMD_ADD_PROCESSOR  0x80010020u      /* DAT_6005e0b4 */
#define REG_CMD_ADD_PORT_BUF   0x8004002cu      /* DAT_6005e0b8 */
#define REG_CMD_ADD_IMPLEMENT  0x80020024u      /* DAT_6005e0bc */
#define REG_CMD_ADD_DONE       0x80030000u      /* DAT_6005e0c0 */

typedef int (*imc_fn)(void *ctx, unsigned cmd, void *a, void *b, int c);
typedef void (*imc_response_fn)(int result, void *param, unsigned len, void *arg);

struct imc_response_link {
    struct imc_response_link *prev, *next;
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
    struct imc *self0, *self1;           /* +0x00 action list head */
    struct imc_response_link responses; /* +0x08 pending responses */
    int   lock[7];                       /* +0x10 */
    void *notif_head, *notif_tail;       /* +0x2c */
    unsigned _34;
    void *rbtree;                        /* +0x38 named events */
    unsigned seq;                        /* +0x3c */
    unsigned _40;
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
extern int   IPC_ReadHeaderAddr(void *h, void *out), IPC_ReadHeaderDone(void *h);
extern int   IPC_WriteCommand(void *h, void *msg, void *param, int irq);
extern int   ConnectionMng_ShBufCmd(int id, const int *p);
extern int   IMC_RaiseEvent(struct imc *, int, int, void *);
extern int   IMC_RegisterNamedEvent(void *, int, int, int, int, int);
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
    imc->self0 = imc->self1 = imc;
    imc->responses.prev = imc->responses.next = &imc->responses;
    imc->notif_head = imc->notif_tail = &imc->notif_head;
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

/* @0x6006da08 IMC_ProcessHeader: type 2 @0x6006dae8 consumes hdr[1].
 * Callback(result=hdr[4], borrowed hdr+8, bytes=hdr[7], borrowed argument)
 * runs after entry free and IRQ restore; its return value is ignored. No
 * command/source/target check or payload-length validation occurs here.
 * IPC storage remains owned by the reader until aimc_ipc_header_done. This
 * response alone does not establish asynchronous FIRST/data completion. */
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
    /* [FLIX] types 3..8 */
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
    int r = IMC_RegisterNamedEvent(imc, name, uid, once == 0, cb, arg);

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
