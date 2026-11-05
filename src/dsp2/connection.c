/*
 * dsp2 ConnectionMng_*: the legacy ADEC "connection manager" (module table of
 * 200 entries x 0x2e0, IMC command processor "CON_MAN"). It wires *buffers*
 * (MultiSinkBuf / inter-processor shared buffers) between legacy ADEC modules
 * and schedules them. It is NOT the aud_module graph used by the renderer /
 * mixer / bypass pipelines: those are connected by Connector_Cmd
 * (connector.c, aud_connector_connect). ConnectionMng_ShBufCmd is however on
 * every IMC dispatch (aimc_dispatch_command checks it first for the
 * 0xf101002c shared-buffer status command), so it services the gluepw/gluepr
 * shared-buffer handshakes.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Evidence: build/decomp/audio_a0_dsp2.main_60000000.bin.c (0x6007f9d8..0x600840d4),
 * build/scratch/a18/asm/cm_connection.asm, cm_disconnect.asm, process_cmd_noirq.asm.
 *
 * ConnectionMng_Connection refuses (observed return codes, ADEC_ERR_*):
 *   2    output module id out of 1..200
 *   (2)  input module id out of 1..200 ("Invalid input module_id(id=%d, name=%s")
 *   0x15 module not on this processor ("Invalid module %s location [cur : %d, exp : %d]")
 *   0x12 input (or output, [FLIX]) module not registered ("Module(%s) is not defined")
 *   0x18 input port uses an externally allocated buffer          [FLIX] return
 *   -    input already connected: "Input module(%s) is already connected to %d" (level 0)
 *        + assert(input_buf_info->buffer == NULL)
 *   -    output port is externally allocated: log level 0 (then continues/aborts [FLIX])
 *   -    output client already connected: "Output module %s port %d client %d is already
 *        connected to %d" + assert(multi_client[client] == NULL)
 *   10   output buffer type != 5 (multi-sink): "Unsupported buffer type (%d)"
 *   0x10 out of memory; -(MultiSinkBuf_RegsterClient error) if the client can't register
 *   Asserts: out port must have bit 0x100, in port must not; out index < 4,
 *   in index < 10, client < 8.
 * Nothing looks at the data type: there is no ES/PCM check.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CM_MAX_MODULES   200
#define CM_MAX_OUT       4
#define CM_MAX_IN        10
#define CM_MAX_CLIENT    8
#define PORT_OUT_FLAG    0x100
#define BUF_TYPE_MULTISINK 5

enum adec_err {
    ADEC_ERR_NONE = 0, ADEC_ERR_GENERAL = 1, ADEC_ERR_RANGE = 2, ADEC_ERR_NOT_REGISTERED = 3,
    ADEC_ERR_BUFTYPE = 10, ADEC_ERR_NOMEM = 0x10, ADEC_ERR_NOT_DEFINED = 0x12,
    ADEC_ERR_LOCATION = 0x15, ADEC_ERR_EXTERN_BUF = 0x18,
};

struct cm_module;                        /* legacy module object (+0x120 last result, +0xe4 set_buffer) */
typedef int (*cm_set_buffer_fn)(struct cm_module *, unsigned port, void *buf);

struct cm_out_port {                     /* 0x74 bytes, at entry +0x18 + idx*0x74 ... */
    int   _pad0[7];
    void *ext_buf;                       /* +0x34 extern buffer used */
    int   ext_size;                      /* +0x38 */
    void *default_buf;                   /* +0x3c MultiSinkBuf */
    int   buf_type;                      /* +0x40 */
    int   _44;
    void *client[CM_MAX_CLIENT];         /* +0x48 */
    int   peer_module[CM_MAX_CLIENT];    /* +0x68 */
    int   peer_port[CM_MAX_CLIENT];      /* +0x88 */
};

struct cm_in_port {                      /* 0x14 bytes at entry +0x204 + idx*0x14 */
    int   ext_used;                      /* +0x204 */
    void *buffer;                        /* +0x208 */
    int   type;                          /* +0x20c (6 = multisink client) */
    int   peer_module;                   /* +0x210 */
    unsigned peer_port;                  /* +0x214 */
};

struct cm_entry {                        /* gRegModuleInfo[id], 0x2e0 bytes */
    int   _0, _4;
    int   type;                          /* +0x08 */
    int   location;                      /* +0x0c processor */
    char  _10[0x30 - 0x10];
    struct cm_module *module;            /* +0x30 */
    char  _34[0x204 - 0x34];             /* outputs live at +0x34 + i*0x74 (overlaid) */
    struct cm_in_port in[CM_MAX_IN];     /* +0x204 */
    char  _2cc_pad[0x2cc - (0x204 + CM_MAX_IN * 0x14)];
    void *initializer;                   /* +0x2cc */
    void *init_arg;                      /* +0x2d0 */
    void *allocator;                     /* +0x2d4 */
    void *alloc_arg;                     /* +0x2d8 */
    void *shmeta;                        /* +0x2dc */
};

extern struct cm_entry *cm_table;                       /* DAT_6005ea70 (entry 0 unused) */
extern struct { void *self; int imc; int platform; int _c; int sched_count; } *cm_state; /* 0x60143d50 */
extern int  *cm_initialized;                            /* DAT_6005ea50 */
extern volatile unsigned *cm_log_mask;                  /* 0x4ff90080 */
extern void *cm_log_ctx;                                /* 0x60101c30 */
extern void logmm_printl(void *ctx, int lvl, const char *fn, int line, const char *fmt, ...);
#define MLOG(lvl, ...) \
    do { if (*cm_log_mask & (1u << (lvl))) \
        logmm_printl(cm_log_ctx, lvl, __func__, __LINE__, __VA_ARGS__); } while (0)

extern void __assert_failed(const char *fn, int line, const char *expr, ...);
extern int  ModuleList_GetPlatform(void);
extern int  ModuleList_GetModuleInfo(int id, void *out);
extern int  ModuleList_SetModuleInfo(int id, void *in);
extern int *ModuleList_GetModuleMemoryInfo(int id);
extern const char *ModuleList_GetModuleName(int id);
extern void *IMC_GetLocalImc(int create);
extern int  IMC_RegisterCmdProcessor(void *imc, int id, void *fn, void *ctx);
extern void ScheduleModule_EnQueue(void), ScheduleModule_DeQueue(void);
extern void BufMonitor_AddBuffer(int id, unsigned port, void *buf, const char *name);
extern void BufMonitor_RemoveBuffer(int id, unsigned port);
extern void BufMonitor_AddBufferRef(void *dst, const char *src, unsigned n);
extern void *aud_malloc(unsigned);
extern void  aud_free(void *);
extern void  AuiLogger_Init(void *, int, unsigned);
extern void InterBufReader_UpdateStatus(void *, const int *), InterBufWriter_UpdateStatus(void *, const int *);
extern int  MultiSinkBuf_GetStructSize(int clients), MultiSinkClient_GetStructSize(void);
extern void MultiSinkBuf_Create(void *, int size, int au, int clients);
extern int  MultiSinkClient_Create(void *);
extern int  MultiSinkBuf_RegsterClient(void *client, void *buf);
extern void *MultiSinkBuf_GetClientLink(void *);
extern void msched_lock(void *), msched_unlock(void);
extern int  snprintf(char *, size_t, const char *, ...);

static inline struct cm_out_port *cm_out(struct cm_entry *e, unsigned idx)
{
    return (struct cm_out_port *)((char *)e + idx * 0x74);
}

static inline cm_set_buffer_fn cm_setbuf(struct cm_module *m)
{
    return *(cm_set_buffer_fn *)((char *)m + 0xe4);
}

/* @0x6007f9d8 ConnectionMng_UpdateList: refresh entries 1..200 from ModuleList */
int ConnectionMng_UpdateList(void)
{
    for (int id = 1; id <= CM_MAX_MODULES; id++) {
        int *e = (int *)&cm_table[id];

        if (ModuleList_GetModuleInfo(id, e) != 0)
            __assert_failed("ConnectionMng_UpdateList", 0xaf, "result == ADEC_ERR_NONE");
        e[0x40 / 4] = e[0x18 / 4];                     /* per-output buffer types */
        e[0xb4 / 4] = e[0x1c / 4];
        e[0x128 / 4] = e[0x20 / 4];
        e[0x19c / 4] = e[0x24 / 4];
    }
    return 0;
}

int ConnectionMng_DestroyModule(int id, ...);
int ConnectionMng_ReleaseBuffer(int id, unsigned port);
int ConnectionMng_Connection(int out_id, unsigned out_port, int in_id, unsigned in_port, int bufsize, int aucount);
int ConnectionMng_Disconnect(int id, unsigned port);
int ConnectionMng_CreateModule(int id, ...);

/* @0x6007fa24 process_cmd_noirq: the CON_MAN IMC command processor. The command
 * compares are FLIX bundles; the id -> handler mapping below comes from the
 * literal ids loaded right before each handler call and the "\t..." dump logs
 * (inferred). Unknown: "%s.%d: CONNECTION_MAN(%d) - Unknown Command %08X". */
int process_cmd_noirq(void *ctx, unsigned cmd, void *resp, const int *p)
{
    int ret = -1;

    (void)ctx;
    (void)resp;
    msched_lock(cm_state);
    switch (cmd >> 16) {
    case 0xf021:                                       /* destroy module (0xf0210003/4) */
        MLOG(5, "\tmodule        : %s\n", ModuleList_GetModuleName(p[0]));
        ret = ConnectionMng_DestroyModule(p[0]);
        break;
    case 0xf011:                                       /* release buffer (0xf0110007/8) */
        MLOG(5, "\tmodule        : %s\n", ModuleList_GetModuleName(p[0]));
        ret = ConnectionMng_ReleaseBuffer(p[0], (unsigned)p[1]);
        break;
    case 0xf002:                                       /* connect (0xf0020017/18) */
        MLOG(5, "\toutput_module : %s\n", ModuleList_GetModuleName(p[0]));
        MLOG(5, "\tinput_module  : %s\n", ModuleList_GetModuleName(p[2]));
        ret = ConnectionMng_Connection(p[0], (unsigned)p[1], p[2], (unsigned)p[3], p[4], p[5]);
        break;
    case 0xf001:                                       /* disconnect (0xf0010007/8) */
        MLOG(5, "\tmodule        : %s\n", ModuleList_GetModuleName(p[0]));
        ret = ConnectionMng_Disconnect(p[0], (unsigned)p[1]);
        break;
    case 0xf012: case 0xf013: case 0xf020: case 0xf081:  /* create/initialize */
        ret = ConnectionMng_CreateModule(p[0]);
        break;
    case 0xf080:
        MLOG(4, "Not implemented command\n");
        break;
    default:
        /* 0xf091 (refs), 0xf0d0 (num_of_modules), 0xf010 (cowork/offset/size): [FLIX] */
        break;
    }
    msched_unlock();
    return ret;
}

/* @0x600806a4 CON_MAN: dummy process function of the CON_MAN pseudo module */
int CON_MAN(void)
{
    return 0;
}

int ConnectionMng_RegisterModule(int id, struct cm_module *m);

/* @0x600806ac ConnectionMng_Init */
int ConnectionMng_Init(int platform, int id)
{
    int *self;

    if (!IMC_GetLocalImc(0)) {
        MLOG(0, "IMC should be initialized before ConnectionMng_Init\n");
        return ADEC_ERR_NOT_DEFINED;
    }
    if (*cm_initialized) {
        cm_state->platform = platform;
        cm_state->imc = id;
        return 0;
    }
    ConnectionMng_UpdateList();
    self = aud_malloc(0x238);
    if (!self) {
        MLOG(0, "no mem\n");
        return ADEC_ERR_NOMEM;
    }
    AuiLogger_Init(self, 0, 0x238);
    self[1] = id;
    self[0x3c] = (int)(intptr_t)process_cmd_noirq;     /* +0xf0 command processor */
    self[0x3b] = (int)(intptr_t)CON_MAN;
    for (int i = 0x28; i <= 0x2c; i += 2)
        self[i] = self[i + 1] = (int)(intptr_t)&self[i];
    BufMonitor_AddBufferRef(&self[0x8a], "CON_MAN", 0xf);
    ConnectionMng_RegisterModule(id, (struct cm_module *)self);
    cm_state->self = self;
    cm_state->imc = id;
    cm_state->platform = platform;
    *cm_initialized = 1;
    return 0;
}

/* @0x6008078c ConnectionMng_RegisterModule: attach object, preset buffers,
 * register its IMC command processor */
int ConnectionMng_RegisterModule(int id, struct cm_module *m)
{
    struct cm_entry *e;

    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return ADEC_ERR_RANGE;
    }
    e = &cm_table[id];
    if (e->location != ModuleList_GetPlatform())
        return ADEC_ERR_LOCATION;
    if (!m)
        __assert_failed("ConnectionMng_RegisterModule", 0x131, "_module != NULL");
    e->module = m;
    *(int *)((char *)m + 0x120) = 0;
    for (unsigned i = 0; i < CM_MAX_OUT; i++) {
        struct cm_out_port *o = cm_out(e, i);
        unsigned port = (i & 0x1f) << 3 | PORT_OUT_FLAG;

        if ((intptr_t)o->ext_buf == 1) {
            MLOG(0, "external buffer allocated before module (%s)\n", (char *)m + 0x228);
            if (cm_setbuf(m)(m, port, (void *)(intptr_t)o->ext_size) != 0)
                __assert_failed("ConnectionMng_RegisterModule", 0x142, "result == ADEC_ERR_NONE");
            BufMonitor_AddBuffer(id, port, (void *)(intptr_t)o->ext_size, "inter");
        } else if (o->default_buf) {
            MLOG(0, "default buffer allocated before module (%s)\n", (char *)m + 0x228);
            if (cm_setbuf(m)(m, port, o->default_buf) != 0)
                __assert_failed("ConnectionMng_RegisterModule", 0x14c, "result == ADEC_ERR_NONE");
            BufMonitor_AddBuffer(id, port, o->default_buf, ModuleList_GetModuleName(id));
        }
    }
    if (IMC_RegisterCmdProcessor(IMC_GetLocalImc(0), id, *(void **)((char *)m + 0xf0), m) != 0)
        __assert_failed("ConnectionMng_RegisterModule", 0x156, "result == ADEC_ERR_NONE");
    return 0;
}

/* @0x60081008 ConnectionMng_AddDefaultBuffer */
int ConnectionMng_AddDefaultBuffer(int id, unsigned port, void *buf, int type)
{
    struct cm_entry *e;
    unsigned idx = port >> 3 & 0x1f;

    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return ADEC_ERR_RANGE;
    }
    e = &cm_table[id];
    if (e->location != ModuleList_GetPlatform()) {
        MLOG(0, "Invalid module %s location [cur : %d, exp : %d]\n", ModuleList_GetModuleName(id),
             ModuleList_GetPlatform(), e->location);
        return ADEC_ERR_LOCATION;
    }
    if (idx >= CM_MAX_OUT) {
        MLOG(0, "Invalid port num. Port index is %d\n", idx);
        return ADEC_ERR_RANGE;
    }
    if (cm_out(e, idx)->buf_type != type)
        __assert_failed("ConnectionMng_AddDefaultBuffer", 0x30e, "gRegModuleInfo[module_id].output...");
    if (!(port & PORT_OUT_FLAG))
        __assert_failed("ConnectionMng_AddDefaultBuffer", 0x30f, "VALIDATE_OUT_PORT (port_num)");
    cm_out(e, idx)->buf_type = type;
    cm_out(e, idx)->default_buf = buf;
    if (!e->module)
        return 0;
    port = idx << 3 | PORT_OUT_FLAG;
    cm_setbuf(e->module)(e->module, port, buf);
    BufMonitor_AddBuffer(id, port, buf, ModuleList_GetModuleName(id));
    return 0;
}

int ConnectionMng_ScheduleModule(int id, int enqueue);

/* @0x60081124 ConnectionMng_UnregisterModule */
int ConnectionMng_UnregisterModule(int id)
{
    struct cm_entry *e;

    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return ADEC_ERR_RANGE;
    }
    e = &cm_table[id];
    if (e->location != ModuleList_GetPlatform()) {
        MLOG(0, "Invalid module %s location [cur : %d, exp : %d]\n", ModuleList_GetModuleName(id),
             ModuleList_GetPlatform(), e->location);
        return ADEC_ERR_LOCATION;
    }
    ConnectionMng_ScheduleModule(id, 0);
    e->module = NULL;
    if (IMC_RegisterCmdProcessor(IMC_GetLocalImc(0), id, NULL, NULL) != 0)
        __assert_failed("ConnectionMng_UnregisterModule", 0x340, "result == ADEC_ERR_NONE");
    return 0;
}

/* @0x600811c8 ConnectionMng_ScheduleModule */
int ConnectionMng_ScheduleModule(int id, int enqueue)
{
    struct cm_entry *e;

    if ((unsigned)(id - 1) >= CM_MAX_MODULES)
        return ADEC_ERR_RANGE;
    e = &cm_table[id];
    if (e->location != ModuleList_GetPlatform())
        return ADEC_ERR_LOCATION;
    if (e->module) {
        if (enqueue)
            ScheduleModule_EnQueue();
        else
            ScheduleModule_DeQueue();
    }
    return 0;
}

/* @0x6008120c ConnectionMng_GetScheduleModuleCount */
int ConnectionMng_GetScheduleModuleCount(void)
{
    return cm_state->sched_count;
}

/* @0x60081218 ConnectionMng_SetModuleLastResult */
int ConnectionMng_SetModuleLastResult(int id, int result)
{
    struct cm_entry *e;

    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return ADEC_ERR_RANGE;
    }
    e = &cm_table[id];
    if (e->location != ModuleList_GetPlatform())
        return ADEC_ERR_LOCATION;
    if (e->module) {
        *(int *)((char *)e->module + 0x120) = result;
        return 0;
    }
    MLOG(0, "module(%s) is not registered\n", ModuleList_GetModuleName(id));
    return ADEC_ERR_NOT_REGISTERED;
}

/* @0x6008129c ConnectionMng_GetModuleLastResult */
int ConnectionMng_GetModuleLastResult(int id)
{
    struct cm_entry *e;

    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return ADEC_ERR_RANGE;
    }
    e = &cm_table[id];
    if (e->location != ModuleList_GetPlatform())
        return ADEC_ERR_LOCATION;
    if (e->module)
        return *(int *)((char *)e->module + 0x120);
    MLOG(0, "module(%s) is not registered\n", ModuleList_GetModuleName(id));
    return ADEC_ERR_NOT_REGISTERED;
}

/* @0x6008131c ConnectionMng_GetModule */
struct cm_module *ConnectionMng_GetModule(int id)
{
    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return NULL;
    }
    if (cm_table[id].location != ModuleList_GetPlatform())
        return NULL;
    return cm_table[id].module;
}

/* @0x60081368 ConnectionMng_GetInputModuleID */
int ConnectionMng_GetInputModuleID(int id, int in_idx)
{
    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return 0;
    }
    if (cm_table[id].location != ModuleList_GetPlatform())
        return 0;
    return cm_table[id].in[in_idx].peer_module;
}

/* @0x600813b8 ConnectionMng_GetOutputModuleID: first client of output <idx> */
int ConnectionMng_GetOutputModuleID(int id, int out_idx)
{
    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return 0;
    }
    if (cm_table[id].location != ModuleList_GetPlatform())
        return 0;
    return cm_out(&cm_table[id], (unsigned)out_idx)->peer_module[0];
}

/* @0x6008140c ConnectionMng_GetOutputModuleType */
int ConnectionMng_GetOutputModuleType(int id)
{
    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return 0;
    }
    if (cm_table[id].location != ModuleList_GetPlatform())
        return 0;
    return cm_table[id].type;
}

/* @0x60081458 ConnectionMng_SetOutputBuf: set mem info and force every output
 * to buffer type 5 (multi-sink) */
int ConnectionMng_SetOutputBuf(int id, int size, int au_count, int au_size)
{
    int *mi, info[0x14];

    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id(%d)\n", id);
        return ADEC_ERR_RANGE;
    }
    mi = ModuleList_GetModuleMemoryInfo(id);
    if (!mi) {
        MLOG(0, "no meminfo for %s\n", ModuleList_GetModuleName(id));
        return ADEC_ERR_GENERAL;
    }
    mi[4] = au_count;
    mi[3] = au_size;
    mi[0] = id;
    mi[2] = size + au_size + 4;
    ModuleList_GetModuleInfo(id, info);
    info[6] = info[7] = info[8] = info[9] = BUF_TYPE_MULTISINK;
    ModuleList_SetModuleInfo(id, info);
    return 0;
}

/* @0x600814f0 ConnectionMng_Connection (refusal list in the file header) */
int ConnectionMng_Connection(int out_id, unsigned out_port, int in_id, unsigned in_port,
                             int bufsize, int aucount)
{
    struct cm_entry *oe, *ie;
    struct cm_out_port *o;
    struct cm_in_port *ip;
    unsigned idx, client;
    void *c;
    char name[20];
    int r;

    MLOG(4, "connect %s(%x) -> %s(%x)\n", ModuleList_GetModuleName(out_id), out_port,
         ModuleList_GetModuleName(in_id), in_port);
    if ((unsigned)(out_id - 1) >= CM_MAX_MODULES)
        return ADEC_ERR_RANGE;                         /* [FLIX] log */
    if ((unsigned)(in_id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid input module_id(id=%d, name=%s\n", in_id, ModuleList_GetModuleName(in_id));
        return ADEC_ERR_RANGE;
    }
    oe = &cm_table[out_id];
    if (oe->location != ModuleList_GetPlatform()) {
        MLOG(0, "Invalid module %s location [cur : %d, exp : %d]\n", ModuleList_GetModuleName(out_id),
             ModuleList_GetPlatform(), oe->location);
        return ADEC_ERR_LOCATION;
    }
    ie = &cm_table[in_id];
    if (ie->location != ModuleList_GetPlatform()) {
        MLOG(0, "Invalid module %s location [cur : %d, exp : %d]\n", ModuleList_GetModuleName(in_id),
             ModuleList_GetPlatform(), ie->location);
        return ADEC_ERR_LOCATION;
    }
    if (!oe->module)
        return ADEC_ERR_NOT_DEFINED;                   /* [FLIX] @0x6008153c */
    if (!ie->module) {
        MLOG(0, "Module(%s) is not defined\n", ModuleList_GetModuleName(in_id));
        return ADEC_ERR_NOT_DEFINED;
    }
    if (!(out_port & PORT_OUT_FLAG))
        __assert_failed("ConnectionMng_Connection", 0x474, "VALIDATE_OUT_PORT (_output_port)");
    if (in_port & PORT_OUT_FLAG)
        __assert_failed("ConnectionMng_Connection", 0x475, "VALIDATE_IN_PORT (_input_port)");
    client = out_port & 7;
    idx = out_port >> 3 & 0x1f;
    if (idx >= CM_MAX_OUT)
        __assert_failed("ConnectionMng_Connection", 0x47b, "output_index < MAX_NUM_OF_OUT_PORT");
    if ((int)in_port >= CM_MAX_IN)
        __assert_failed("ConnectionMng_Connection", 0x47c, "input_index < MAX_NUM_OF_INPUT_PORT");

    ip = &ie->in[in_port];
    if (ip->ext_used)
        return ADEC_ERR_EXTERN_BUF;                    /* [FLIX] @0x6008159c */
    if (ip->peer_module)
        MLOG(0, "Input module(%s) is already connected to %d\n", ModuleList_GetModuleName(in_id),
             ip->peer_module);
    if (ip->buffer)
        __assert_failed("ConnectionMng_Connection", 0x495, "input_buf_info->buffer == NULL");

    o = cm_out(oe, idx);
    if (o->ext_buf)
        MLOG(0, "Output port %d of module %s is extern allocated buffer\n", idx,
             ModuleList_GetModuleName(out_id));
    if (o->buf_type == BUF_TYPE_MULTISINK && (out_port & 0x1000)) {
        client = o->peer_module[1] ? 6 : 7;            /* auto client */
        MLOG(3, "select auto output client %d at %s\n", client, ModuleList_GetModuleName(out_id));
    }
    if (o->peer_module[client])
        MLOG(0, "Output module %s port %d client %d is already connected to %d\n",
             ModuleList_GetModuleName(out_id), idx, client, o->peer_module[client]);
    if (o->buf_type != BUF_TYPE_MULTISINK) {
        MLOG(0, "Unsupported buffer type (%d)\n", o->buf_type);
        return ADEC_ERR_BUFTYPE;
    }
    if (o->client[client])
        __assert_failed("ConnectionMng_Connection", 0x4c8, "output_buf_info->multi_client[client_index] == NULL");

    if (!o->default_buf) {
        void *mem, *msb;

        if (!bufsize || !aucount) {
            int *mi = ModuleList_GetModuleMemoryInfo(out_id);
            if (!mi)
                MLOG(0, "Module Memory Information %s is not defined\n", ModuleList_GetModuleName(out_id));
            MLOG(0, "Check Module Memory Information\n");
            /* [FLIX] sizes taken from mi */
        }
        mem = aud_malloc((unsigned)(bufsize * (aucount + 1)));
        if (!mem)
            return ADEC_ERR_NOMEM;
        msb = aud_malloc((unsigned)MultiSinkBuf_GetStructSize(aucount));
        if (!msb) {
            aud_free(mem);
            return ADEC_ERR_NOMEM;
        }
        AuiLogger_Init(msb, 0, (unsigned)MultiSinkBuf_GetStructSize(aucount));
        MultiSinkBuf_Create(mem, bufsize * (aucount + 1), bufsize, aucount);
        o->default_buf = msb;
        ConnectionMng_AddDefaultBuffer(out_id, out_port, msb, o->buf_type);
    }

    c = aud_malloc((unsigned)MultiSinkClient_GetStructSize());
    if (!c) {
        MLOG(0, "Fail to allocated memory\n");
        return ADEC_ERR_NOMEM;
    }
    AuiLogger_Init(c, 0, (unsigned)MultiSinkClient_GetStructSize());
    if (MultiSinkClient_Create(c) != 0)
        __assert_failed("ConnectionMng_Connection", 0x52d, "result == ADEC_ERR_NONE");
    r = MultiSinkBuf_RegsterClient(c, o->default_buf);
    if (r < 0) {
        aud_free(c);
        return -r;
    }
    o->peer_module[client] = in_id;
    o->peer_port[client] = (int)in_port;
    o->client[client] = c;
    ip->type = 6;
    ip->peer_module = out_id;
    ip->buffer = c;
    ip->peer_port = out_port;
    ip->ext_used = 0;
    cm_setbuf(ie->module)(ie->module, in_port, c);
    snprintf(name, 0x10, "%s", ModuleList_GetModuleName(out_id));
    BufMonitor_AddBuffer(in_id, in_port, c, name);
    MLOG(4, "buf %p for multisink client\n", c);
    return 0;
}

/* @0x60081ef8 ConnectionMng_Disconnect: <port> without 0x100 = input side,
 * with 0x100 = output client side. Several branch bodies are FLIX-only. */
int ConnectionMng_Disconnect(int id, unsigned port)
{
    struct cm_entry *e;

    MLOG(3, "Try to disconnect %s:%x\n", ModuleList_GetModuleName(id), port);
    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return ADEC_ERR_RANGE;
    }
    e = &cm_table[id];
    if (e->location != ModuleList_GetPlatform())
        return ADEC_ERR_LOCATION;
    if (!e->module)
        return ADEC_ERR_NOT_DEFINED;

    if (!(port & PORT_OUT_FLAG)) {
        struct cm_in_port *ip = &e->in[port];
        struct cm_entry *oe;

        if (ip->ext_used)
            return ADEC_ERR_EXTERN_BUF;                /* [FLIX] */
        if (!ip->peer_module)
            MLOG(0, "%s:%x is not connected to any other module\n", ModuleList_GetModuleName(id), port);
        oe = &cm_table[ip->peer_module];
        if (!oe->module)
            __assert_failed("ConnectionMng_Disconnect", 0x60b, "output_module->module != NULL");
        if (cm_out(oe, ip->peer_port >> 3 & 0x1f)->ext_buf)
            __assert_failed("ConnectionMng_Disconnect", 0x617, "output_buf_info->extern_buffer_used == 0");
        if (cm_setbuf(e->module)(e->module, port, NULL) != 0)
            __assert_failed("ConnectionMng_Disconnect", 0x667, "result == ADEC_ERR_NONE");
        BufMonitor_RemoveBuffer(id, port);
        /* [FLIX] client unregister + bookkeeping */
        return 0;
    }
    {
        struct cm_out_port *o = cm_out(e, port >> 3 & 0x1f);
        unsigned client = port & 7;

        if (o->ext_buf) {
            MLOG(0, "%s:%x is using extern allocated buffer\n", ModuleList_GetModuleName(id), port);
            return ADEC_ERR_EXTERN_BUF;
        }
        if (!o->peer_module[client])
            MLOG(0, "%s:%x is not connected to any other module\n", ModuleList_GetModuleName(id), port);
        if (!cm_table[o->peer_module[client]].module)
            __assert_failed("ConnectionMng_Disconnect", 0x64b, "input_module->module != NULL");
        if (cm_table[o->peer_module[client]].in[o->peer_port[client]].ext_used)
            __assert_failed("ConnectionMng_Disconnect", 0x654, "input_buf_info->extern_buffer_used == 0");
        MLOG(3, "Disconnect %s:%x -> %s:%x\n", ModuleList_GetModuleName(id), port,
             ModuleList_GetModuleName(o->peer_module[client]), o->peer_port[client]);
        /* [FLIX] the actual unlink is FLIX-scheduled */
        return 0;
    }
}

/* @0x60082b4c CM_GetOutputModule: call <cb> for every consumer of <m>'s outputs */
int CM_GetOutputModule(struct cm_module *m, void *arg, void (*cb)(void *, int, int, int))
{
    struct link { struct link *next, *prev; } *head = (struct link *)((char *)m + 0xa8), *l;

    for (l = head->prev; l != head; l = l->prev) {
        int *f = (int *)l;

        if (f[-3])
            cb(arg, f[-3], 0, f[-2]);
        else if (f[-4])
            cb(arg, 0, f[-4], f[-2]);
        else {
            struct link *ch = MultiSinkBuf_GetClientLink((void *)(intptr_t)f[-1]), *x;
            for (x = ch->prev; x != ch; x = x->prev)
                cb(arg, ((int *)x)[-3], 0, f[-2]);
        }
    }
    return 0;
}

/* @0x60082bb4 ConnectionMng_SetShMetaBuf */
int ConnectionMng_SetShMetaBuf(int id, void *buf)
{
    cm_table[id].shmeta = buf;
    return 0;
}

/* @0x60082bcc ConnectionMng_ShBufCmd: shared (inter-DSP) buffer status update.
 * p[0] == 3: reader status, else writer status; p[2] = buffer key. Matching
 * buffer -> update + schedule the module. Unknown target is only logged. */
int ConnectionMng_ShBufCmd(int id, const int *p)
{
    struct cm_entry *e = &cm_table[id];
    void *b;

    if (!e->module)
        return 0;
    MLOG(5, "target %d, type %d\n", id, p[0]);
    b = e->shmeta;
    if (b && p[2] == *(int *)((char *)b + 0x38)) {
        if (p[0] == 3)
            InterBufReader_UpdateStatus(b, p + 3);
        else
            InterBufWriter_UpdateStatus(b, p + 3);
        ConnectionMng_ScheduleModule(id, 1);
        return 0;
    }
    b = p[0] == 3 ? e->in[0].buffer : cm_out(e, 0)->ext_buf;
    if (b && p[2] == *(int *)((char *)b + 0x38)) {
        if (p[0] == 3)
            InterBufReader_UpdateStatus(b, p + 3);
        else
            InterBufWriter_UpdateStatus(b, p + 3);
        ConnectionMng_ScheduleModule(id, 1);
        return 0;
    }
    MLOG(4, "wrong target (%s %x) %s event %08x\n", ModuleList_GetModuleName(id), id,
         p[0] == 3 ? "write" : "read", p[2]);
    return 0;
}

extern void *ConnectionMng_NewPBuf(int, int, int, int);  /* @0x600838d0 */

/* @0x60083a00 ConnectionMng_NewDBuf: NewPBuf with the "physical" flag cleared */
void *ConnectionMng_NewDBuf(int a, int b, int c, int d)
{
    void *p = ConnectionMng_NewPBuf(a, b, c, d);

    if (!p)
        return NULL;
    *(int *)((char *)p + 0x6c) = 0;
    return p;
}

/* @0x60083a20 ConnectionMng_RegisterAllocator */
int ConnectionMng_RegisterAllocator(int id, void *fn, void *arg)
{
    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return ADEC_ERR_RANGE;
    }
    cm_table[id].allocator = fn;
    cm_table[id].alloc_arg = arg;
    return 0;
}

/* @0x60083a64 ConnectionMng_RegisterInitializer */
int ConnectionMng_RegisterInitializer(int id, void *fn, void *arg, void *arg2)
{
    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id\n");
        return ADEC_ERR_RANGE;
    }
    cm_table[id].initializer = fn;
    cm_table[id].alloc_arg = arg;
    cm_table[id].init_arg = arg2;
    return 0;
}

/* @0x60083ec4 ConnectionMng_ClearAllReference: set_buffer(0x200|i, NULL) for
 * every reference slot (count at module +0x10) */
int ConnectionMng_ClearAllReference(int id)
{
    struct cm_entry *e;
    unsigned n;

    MLOG(4, "Try to clear all reference of %s\n", ModuleList_GetModuleName(id));
    if ((unsigned)(id - 1) >= CM_MAX_MODULES) {
        MLOG(0, "Invalid module_id(%08x)\n", id);
        return ADEC_ERR_RANGE;
    }
    e = &cm_table[id];
    if (e->location != ModuleList_GetPlatform())
        return ADEC_ERR_LOCATION;
    if (!e->module) {
        MLOG(0, "Module(%08x) is not defined\n", id);
        return ADEC_ERR_NOT_DEFINED;
    }
    n = *(unsigned *)((char *)e->module + 0x10);
    for (unsigned i = 0; i < n; i++) {
        unsigned port = (i & 0xff) | 0x200;
        int r = cm_setbuf(e->module)(e->module, port, NULL);

        if (r) {
            MLOG(0, "Fail(%d) to set buffer to %s.%d\n", r, ModuleList_GetModuleName(id), port);
            return r;
        }
    }
    return 0;
}
