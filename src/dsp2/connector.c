/*
 * dsp2 connectors, two unrelated families that share the word:
 *  1. aud_connector_* / Connector_*: module-graph port connect/disconnect,
 *     driven by the kernel's aimc_module_connect (command 0xf1000010) and
 *     aimc_module_disconnect (0xf1010010) addressed to the "connector" uid
 *     (proc<<24 | 2).
 *  2. connector_*: the Dolby MS12 "MSF" sample/metadata ring used between
 *     ms_mixer and pcm_ren (src/msf). Only the small accessors are rebuilt.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Evidence: build/decomp/audio_a0_dsp2.main_60000000.bin.c,
 * build/scratch/evidence/dsp2-connect/6006f600_aud_connector_connect.asm.
 *
 * When is a connection refused (observed, aud_connector_connect):
 *   - the input port <in_port> of <in_mod> already exists (it is connected):
 *       "already used input module port (%s.%d)" level 0, returns -24.
 *     An input port is single-peer; a second producer for the same input is
 *     refused, the existing link stays.
 *   - no memory for the output or input port: "no mem", returns -20.
 *   Output ports are NOT exclusive: aud_module_new_output_port returns the
 *   existing port and the new peer is added (fan-out), see module_core.c.
 *   Nothing else is checked: no data type, no ES/PCM compatibility, no
 *   processor check (cross-DSP links are made by the kernel with gluepw/gluepr).
 * Disconnect fails (-1) only when either port does not exist.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct aud_module { const char *name; int uid; };
struct aud_port;

extern volatile unsigned *conn_log_mask;                /* 0x6005d570 -> 0x4ff9.... */
extern void *conn_log_ctx;                              /* 0x6005d568 */
extern void logmm_printl(void *ctx, int lvl, const char *fn, int line, const char *fmt, ...);
#define CLOG(lvl, ...) \
    do { if (*conn_log_mask & (1u << (lvl))) \
        logmm_printl(conn_log_ctx, lvl, __func__, __LINE__, __VA_ARGS__); } while (0)

extern struct aud_port *aud_module_get_input_port(struct aud_module *, int);
extern struct aud_port *aud_module_get_output_port(struct aud_module *, int);
extern struct aud_port *aud_module_new_output_port(struct aud_module *, int);
extern struct aud_port *aud_module_new_input_port(struct aud_module *, int);
extern int  aud_module_connect(struct aud_port *out, struct aud_port *in);     /* @0x60074154 */
extern int  aud_module_disconnect(struct aud_port *out, struct aud_port *in);  /* @0x60074048 */
extern struct aud_module *ModuleRegistry_SearchInstByUid(int uid);
extern int  aimc_register_command_handler(void *imc, unsigned uid, void *fn, void *ctx);
extern void aimc_unregister_command_handler(void *imc, unsigned uid);
extern void *aud_malloc(unsigned);
extern void  aud_free(void *);
extern void  AuiLogger_Init(void *dst, int c, unsigned n);   /* memset */

#define CONNECTOR_CMD_CONNECT     0xf1000010u   /* DAT_6005d598 */
#define CONNECTOR_CMD_DISCONNECT  0xf1010010u   /* DAT_6005d59c */

struct connector_param { int out_uid, out_port, in_uid, in_port; };

/* aud_connector_connect, 0x6006f600 (address tag already used by a comment in
 * renderer_path.c, so not tagged again here) */
int aud_connector_connect(struct aud_module *out, int out_port, struct aud_module *in, int in_port)
{
    struct aud_port *op, *ip = aud_module_get_input_port(in, in_port);

    if (ip) {
        CLOG(0, "already used input module port (%s.%d)\n", in->name, in_port);
        return -24;
    }
    op = aud_module_new_output_port(out, out_port);
    if (!op) {
        CLOG(0, "no mem\n");
        return -20;
    }
    ip = aud_module_new_input_port(in, in_port);
    if (!ip) {
        CLOG(0, "no mem\n");
        return -20;
    }
    aud_module_connect(op, ip);
    CLOG(3, "module connected (%s.%d -> %s.%d)\n", out->name, out_port, in->name, in_port);
    return 0;
}

/* @0x6006f6c8 aud_connector_disconnect */
int aud_connector_disconnect(struct aud_module *out, int out_port, struct aud_module *in, int in_port)
{
    struct aud_port *ip = aud_module_get_input_port(in, in_port);
    struct aud_port *op = aud_module_get_output_port(out, out_port);

    if (ip && op) {
        CLOG(3, "disconnect module %s.%d -> %s.%d\n", out->name, out_port, in->name, in_port);
        aud_module_disconnect(op, ip);
        return 0;
    }
    if (!ip)
        CLOG(0, "no input port%d on %s??\n", in_port, in->name);
    if (!op)
        CLOG(0, "no output port%d on %s??\n", out_port, out->name);
    CLOG(0, "module disconnecting (%s.%d, %s.%d)\n", out->name, out_port, in->name, in_port);
    return -1;
}

/* @0x6006f7a8 Connector_Cmd: IMC handler of the connector uid */
int Connector_Cmd(void *ctx, unsigned cmd, const struct connector_param *p)
{
    struct aud_module *out, *in;

    (void)ctx;
    CLOG(3, "received command %08x\n", cmd);
    if (cmd == CONNECTOR_CMD_CONNECT) {
        out = ModuleRegistry_SearchInstByUid(p->out_uid);
        if (!out) {
            CLOG(0, "unable to find output module (uid: %08x)\n", p->out_uid);   /* [FLIX] guard */
            return -1;
        }
        in = ModuleRegistry_SearchInstByUid(p->in_uid);
        if (!in) {
            CLOG(0, "unable to find module (uid: %08x)\n", p->in_uid);
            return -1;
        }
        return aud_connector_connect(out, p->out_port, in, p->in_port);
    }
    if (cmd != CONNECTOR_CMD_DISCONNECT) {
        CLOG(0, "Unknown command\n");
        return -18;
    }
    CLOG(3, "disconnect out (%08x) in (%08x)\n", p->out_uid, p->in_uid);
    out = ModuleRegistry_SearchInstByUid(p->out_uid);
    if (!out) {
        CLOG(0, "unable to find output module (uid: %08x)\n", p->out_uid);
        return -1;
    }
    in = ModuleRegistry_SearchInstByUid(p->in_uid);
    if (!in) {
        CLOG(0, "unable to find input module (uid: %08x)\n", p->in_uid);
        return -1;
    }
    return aud_connector_disconnect(out, p->out_port, in, p->in_port);
}

struct connector_obj { char _pad[0xf0]; void *imc; unsigned uid; };

/* @0x6006f8f4 Connector_Create: uid = proc << 24 | 2 */
struct connector_obj *Connector_Create(int proc, void *imc)
{
    struct connector_obj *c = aud_malloc(sizeof(*c));

    if (!c) {
        CLOG(0, "no mem\n");
        return NULL;
    }
    AuiLogger_Init(c, 0, sizeof(*c));
    c->imc = imc;
    c->uid = ((unsigned)proc << 24) | 2;
    aimc_register_command_handler(imc, c->uid, (void *)Connector_Cmd, NULL);
    return c;
}

/* @0x6006f954 Connector_Destroy */
void Connector_Destroy(struct connector_obj *c)
{
    aimc_unregister_command_handler(c->imc, c->uid);
    aud_free(c);
}

/* ------------------------------------------------------------------------
 * MS12 MSF connector (struct connector: md slot array +0x00, max md +0x04,
 * current md +0x08, last find index +0x0c, databuffer +0x14, ...)
 * ---------------------------------------------------------------------- */
struct md_node { int id; int type; int offset; int remain; int _10; const char *name; };

struct connector {
    struct md_node **md;                 /* +0x00 */
    int      max_md;                     /* +0x04 */
    int      num_md;                     /* +0x08 */
    int      last_idx;                   /* +0x0c */
    int      _10;
    void    *databuf;                    /* +0x14 */
    void    *md_info;                    /* +0x18 */
    void    *md_store;                   /* +0x1c */
    int      md_size;                    /* +0x20 */
    char     name[0x80];                 /* +0x24 */
    const char *pname;                   /* +0xa4 */
};

struct connector_cfg { int a, b, c, d, max_md, md_size; const char *name; };

#define MD_ID_EVO      0x65e
#define MD_ID_KEEP     0x3ea
#define MD_ID_CHECK    0x579

extern unsigned irq_raise_level(void);                  /* rsr ps; wsr ps.intlevel=3 */
extern void irq_restore(unsigned);
extern void *databuffer_get_write_buf(void *, int), *databuffer_get_read_buf(void *, int);
extern int  databuffer_get_avail_samples_write(void *), databuffer_get_avail_samples_read(void *);
extern int  databuffer_get_max_samples(void *);
extern int  databuffer_update_num_channels(void *, int), databuffer_update_write_buf(void *, int);
extern void databuffer_query_mem(void *cfg, void *out);
extern void *databuffer_open(void *cfg, void *mem);
extern void BufMonitor_AddBufferRef(void *dst, const char *src, unsigned n); /* strncpy */
extern int  connector_put_inter_metadata(void *c, ...);
extern int  evo_trp_writer_get_au_count(void);
extern int  msf_get_dlb_buffer_type(int);
extern void IPC_WriteDebug(int, int, int, int, const char *, ...);
extern void *_assert_ptr(const char *, int);
extern struct { int evo_writer[8]; } *msf_global;       /* DAT_6005fa50 */
extern struct { char _h[0x10]; int level; void *pcm_conn; } *msf_log;  /* 0x6005f944 */
extern volatile unsigned *msf_log_mask;                 /* DAT_6005f93c */

/* @0x6008c054 connector_init_md_info: (re)create the 12-byte md-info list head */
void connector_init_md_info(void **info)
{
    extern void *calloc(size_t, size_t);
    struct node { struct node *next; } *h = *info, *n;

    if (h) {
        while ((n = h->next)) {
            h->next = n->next;
            aud_free(n);
        }
        aud_free(h);
    }
    *info = aud_malloc(12);
    if (*info) {
        ((int *)*info)[0] = ((int *)*info)[1] = ((int *)*info)[2] = 0;
        return;
    }
    if (*msf_log_mask & 1)
        logmm_printl(msf_log, 0, "connector_init_md_info", 0x21d, "malloc metadata info, Err\n");
}

/* @0x6008c0b4 connector_deinit_md_info */
void connector_deinit_md_info(void **info)
{
    struct node { struct node *next; } *h = *info, *n;

    if (!h)
        return;
    while ((n = h->next)) {
        h->next = n->next;
        aud_free(n);
    }
    aud_free(h);
}

/* @0x6008c3ec connector_get_md_node */
void *connector_get_md_node(void **list)
{
    if (!list) {
        if (*msf_log_mask & 1)
            logmm_printl(msf_log, 0, "connector_get_md_node", 0x304, "metadata is null\n");
        return NULL;
    }
    return ((int *)list)[2] ? list[0] : NULL;
}

/* @0x6008c7f4 connector_get_count_evo_metadata: 1 if slot 0 holds an EVO (0x65e) md */
int connector_get_count_evo_metadata(struct connector *c)
{
    unsigned ps;
    int n = 0;

    if (!c)
        return 0;
    ps = irq_raise_level();
    if (c->max_md > 0 && c->md[0] && c->md[0]->id == MD_ID_EVO)
        n = 1;
    irq_restore(ps);
    return n;
}

/* @0x6008c84c connector_init_metadata: drop every md except ids 0x3ea / 0x65e */
int connector_init_metadata(struct connector *c)
{
    unsigned ps;

    if (!c)
        return 0;
    ps = irq_raise_level();
    for (int i = 0; i < c->max_md; i++)
        if (c->md[i] && c->md[i]->id != MD_ID_KEEP && c->md[i]->id != MD_ID_EVO) {
            c->md[i] = NULL;
            c->num_md--;
        }
    irq_restore(ps);
    return 0;
}

/* @0x6008c8ac connector_find_metadata: look at last_idx+1, then slot 0 */
struct md_node *connector_find_metadata(struct connector *c, int id, int offset)
{
    struct md_node *n;
    int i;

    if (!c)
        return NULL;
    if (c->md) {
        i = c->last_idx + 1;
        if (i < c->max_md && c->max_md - c->last_idx - 1 > 0 && (n = c->md[i]) &&
            n->id == id && (n->type == 2 || n->offset < offset)) {
            c->last_idx = i;
            return n;
        }
        if (c->last_idx >= 0 && i > 0 && (n = c->md[0]) &&
            n->id == id && (n->type == 2 || n->offset < offset)) {
            c->last_idx = 0;
            return n;
        }
    }
    c->last_idx = -1;
    return NULL;
}

int connector_erase_metadata_by_id(struct connector *c, int id);

/* @0x6008ce18 connector_check_metadata */
int connector_check_metadata(struct connector *c)
{
    unsigned ps;
    int n = 0;

    if (!c)
        return 0;
    ps = irq_raise_level();
    if (c->max_md > 0 && c->md[0] && c->md[0]->id == MD_ID_CHECK)
        n = 1;
    /* "keep at most 3": dead code with n <= 1 (observed as compiled) */
    if (n > 3)
        for (int i = 0; i < n - 3; i++)
            connector_erase_metadata_by_id(c, MD_ID_CHECK);
    irq_restore(ps);
    return n;
}

/* @0x6008ce88 connector_erase_metadata_by_id: only slot 0 is examined */
int connector_erase_metadata_by_id(struct connector *c, int id)
{
    unsigned ps;

    if (!c)
        return 0;
    ps = irq_raise_level();
    if (c->max_md > 0 && c->md[0] && c->md[0]->id == id && c->md[0]->offset > 0) {
        struct md_node *n = c->md[0];

        c->md[0] = NULL;
        c->num_md--;
        irq_restore(ps);
        return (int)(intptr_t)n;
    }
    irq_restore(ps);
    return 0;
}

/* @0x6008cefc connector_get_write_buf */
void *connector_get_write_buf(struct connector *c, int n)
{
    return c ? databuffer_get_write_buf(c->databuf, n) : NULL;
}

/* @0x6008cf14 connector_get_read_buf */
void *connector_get_read_buf(struct connector *c, int n)
{
    return c ? databuffer_get_read_buf(c->databuf, n) : NULL;
}

/* @0x6008cf2c connector_update_inter_num_channels: queue an inter-processor
 * "num channels" md (id DAT_6005fad8) through the EVO writer */
int connector_update_inter_num_channels(void *c, int nch)
{
    if (msf_global->evo_writer[2]) {
        int md[8] = { 0 };
        md[0] = md[1] = 0;                             /* id from DAT_6005fad8 [GAP] */
        md[5] = 4;
        md[4] = nch;
        connector_put_inter_metadata(c, md);
        return 0;
    }
    if (*msf_log_mask & 1)
        logmm_printl(msf_log, 0, "connector_update_inter_num_channels", 0x5a5,
                     "ERROR: inter evo writer is null\n");
    return -1;
}

/* @0x6008cf84 connector_update_num_channels */
int connector_update_num_channels(struct connector *c, int nch)
{
    if (c)
        return databuffer_update_num_channels(c->databuf, nch);
    if (msf_log->level == 3 && (*msf_log_mask & 2))
        logmm_printl(msf_log, 1, "connector_update_num_channels", 0x5bc,
                     "ERROR: Update number of channels 0x%x\n", 0x81);
    return 0x81;
}

/* @0x6008cfc8 connector_update_inter_write_buf */
int connector_update_inter_write_buf(void *c, int n)
{
    if (msf_global->evo_writer[2]) {
        int md[8] = { 0 };
        md[5] = 4;
        md[4] = n;                                     /* id from DAT_6005fadc [GAP] */
        connector_put_inter_metadata(c, md);
        return 0;
    }
    if (*msf_log_mask & 1)
        logmm_printl(msf_log, 0, "connector_update_inter_write_buf", 0x5ca,
                     "ERROR: inter evo writer is null\n");
    return -1;
}

/* @0x6008d020 connector_update_write_buf */
int connector_update_write_buf(struct connector *c, int n)
{
    unsigned ps;
    int r;

    if (!c)
        return 0x81;                                   /* [FLIX] error log when level == 3 */
    if (c->pname && (*msf_log_mask & 0x10))
        logmm_printl(msf_log, 4, "connector_update_write_buf", 0x5f2,
                     "connector %s current_num_md %d\n", c->pname, c->num_md);
    ps = irq_raise_level();
    r = databuffer_update_write_buf(c->databuf, n);
    irq_restore(ps);
    return r;
}

/* @0x6008d11c connector_update_read_buf: advance every md offset by <n>,
 * releasing the ones fully consumed. Loop bound is FLIX [FLIX]; written for
 * max_md slots. */
int connector_update_read_buf(struct connector *c, int n)
{
    unsigned ps;

    if (!c)
        return 0x81;
    ps = irq_raise_level();
    for (int i = 0; i < c->max_md; i++) {
        struct md_node *m = c->md[i];
        int off, rem;

        if (!m)
            continue;
        off = m->offset;
        rem = m->remain;
        m->offset = off - n;
        if (m->offset >= 0)
            continue;
        m->remain = rem + m->offset;
        m->offset = 0;
        if (m->remain < 1) {
            if (m->id == MD_ID_EVO && (*msf_log_mask & 8))
                logmm_printl(msf_log, 3, "connector_update_read_buf", 0x624,
                             "EV %2d (%8s) %2d removed offset %u\n", (int)(intptr_t)m->name,
                             c->pname, i, off);
            c->md[i] = NULL;
            c->num_md--;
        }
    }
    irq_restore(ps);
    return 0;
}

/* @0x6008d224 connector_get_avail_samples_write */
int connector_get_avail_samples_write(struct connector *c)
{
    return c ? databuffer_get_avail_samples_write(c->databuf) : 0;
}

/* @0x6008d23c connector_get_avail_samples_read */
int connector_get_avail_samples_read(struct connector *c)
{
    return c ? databuffer_get_avail_samples_read(c->databuf) : 0;
}

/* @0x6008d254 connector_get_max_samples */
int connector_get_max_samples(struct connector *c)
{
    return c ? databuffer_get_max_samples(c->databuf) : 0;
}

/* @0x6008d26c connector_get_available_md_slot */
int connector_get_available_md_slot(struct connector *c)
{
    extern volatile unsigned *ipc_debug_mask;           /* DAT_6005c61c */
    extern int *ipc_debug_handle;                       /* DAT_6005c624 */

    if (c)
        return c->max_md - c->num_md;
    if (*ipc_debug_mask & 0x800000)
        IPC_WriteDebug(*ipc_debug_handle, 1, 0x17, 0, "ERROR: connector is null\n");
    return 0;
}

/* @0x6008d2a4 connector_get_inter_available_md_slot: 1000 - EVO AU count */
int connector_get_inter_available_md_slot(void)
{
    extern volatile unsigned *ipc_debug_mask;
    extern int *ipc_debug_handle;
    int n;

    if (!msf_global->evo_writer[2]) {
        if (*ipc_debug_mask & 0x200000)
            IPC_WriteDebug(*ipc_debug_handle, 1, 0x15, 0, "ERROR: inter evo writer is null\n");
        return 0;
    }
    n = evo_trp_writer_get_au_count();
    if (n > 0)
        return 1000 - n;
    if (*ipc_debug_mask & 0x200000)
        IPC_WriteDebug(*ipc_debug_handle, 1, 0x15, 0, "ERROR: evo get au count failed (%d)\n", n);
    return 0;
}

/* @0x6008d308 connector_get_current_num_md */
int connector_get_current_num_md(struct connector *c)
{
    return c ? c->num_md : 0;
}

/* @0x6008d318 connector_query_mem */
int connector_query_mem(const struct connector_cfg *cfg, int *size)
{
    int db[4] = { cfg->a, cfg->b, cfg->c, cfg->d };

    databuffer_query_mem(db, size);
    size[1] = cfg->max_md * 0x20 + cfg->md_size * cfg->max_md + 0xaa;
    return 0;
}

/* @0x6008d350 connector_open: lay out md slots + md store after the header */
struct connector *connector_open(const struct connector_cfg *cfg, void *const mem[2])
{
    struct connector *c = mem[1];
    struct { int a, b, c, d; } db = { cfg->a, cfg->b, cfg->c, cfg->d };
    int n = cfg->max_md;

    if (cfg->md_size & 3)
        _assert_ptr("../xt_dsp/src/msf/msf_v24/src/msf_connector.c", 0);
    c->md = (struct md_node **)((int *)c + 0x2a);
    c->num_md = 0;
    c->last_idx = -1;
    c->max_md = n;
    c->md_info = (int *)c + n + 0x2a;
    c->md_store = (void *)(((uintptr_t)c->md_info + (size_t)n * 0x1c + 3) & ~(uintptr_t)3);
    c->databuf = databuffer_open(&db, mem[0]);
    c->md_size = cfg->md_size;
    for (int i = 0; i < n; i++) {
        c->md[i] = NULL;
        ((void **)((char *)c->md_info + i * 0x1c))[6] = (char *)c->md_store + cfg->md_size * i;
    }
    c->pname = c->name;
    BufMonitor_AddBufferRef(c->name, cfg->name, 0x80);
    return c;
}

/* @0x6008d3fc init_connector_cfg */
void init_connector_cfg(struct connector_cfg *cfg, int a, int b, int c, int d, int max_md,
                        int md_size, const char *name)
{
    if (*msf_log_mask & 8)
        logmm_printl(msf_log, 3, "init_connector_cfg", 0x6d3,
                     "[%s] connector sample size for %s: %d\n", "1213_2325_develop_49c38adea6", name, c);
    cfg->a = a;
    cfg->b = b;
    cfg->c = c;
    cfg->d = d;
    cfg->max_md = max_md;
    cfg->name = name;
    cfg->md_size = md_size;
}

/* @0x6008d454 connector_setaddress: place the shared connectors relative to
 * the shared-memory window (registers at 0x200/0x24c of the peri block) */
void connector_setaddress(int base)
{
    extern volatile int *peri_regs;                     /* SUB_6005df90 */
    extern int shm_base, off_sys, off_ren0, off_ren1;   /* 6005dfa8, 6005e2e8, 6005fb84, 6005fb88 */
    int *g = msf_global->evo_writer;
    int a = (peri_regs[0x24c / 4] - peri_regs[0x200 / 4]) + shm_base;

    g[5] = base;
    g[0] = a + off_sys;
    g[3] = a + off_ren0;
    g[4] = a + off_ren1;
}

/* @0x6008d48c connector_create: id 0x2f = system mixer (+renderer DAP for
 * variant 2), 0x65 = shared system connector, 0x27/0x28 = PCM connector */
struct connector *connector_create(int id, int variant)
{
    int *g = msf_global->evo_writer;
    struct connector_cfg cfg;
    int size[2];
    void *mem[2] = { 0, 0 };
    extern int sys_mixer_md_size, renderer_dap_samples, conn_mem_off; /* 6005f29c, 6005fb98, 6005fb90 */

    if (id == 0x2f) {
        init_connector_cfg(&cfg, 8, 1, 0x600, msf_get_dlb_buffer_type(1), 0x1e, sys_mixer_md_size,
                           "SYS_MIXER");
        connector_query_mem(&cfg, size);
        mem[1] = (void *)(intptr_t)g[0];
        mem[0] = (char *)mem[1] + conn_mem_off;
        connector_open(&cfg, mem);
        if (variant == 2) {                            /* [FLIX] */
            init_connector_cfg(&cfg, 8, 1, renderer_dap_samples, msf_get_dlb_buffer_type(1), 0x1e,
                               sys_mixer_md_size, "RENDERER_DAP");
            connector_query_mem(&cfg, size);
            mem[1] = (void *)(intptr_t)g[4];
            mem[0] = (char *)mem[1] + conn_mem_off;
        }
        if (mem[1])
            return connector_open(&cfg, mem);
        return NULL;
    }
    if (id == 0x65)
        return (struct connector *)(intptr_t)g[0];
    if (id == 0x27 || id == 0x28)
        return msf_log->pcm_conn;
    return NULL;                                       /* [FLIX] log */
}

/* @0x6008d5d0 connector_input_handle */
struct connector *connector_input_handle(int id, int variant)
{
    int *g = msf_global->evo_writer;

    if (id == 0x2f)
        return (struct connector *)(intptr_t)(variant == 2 ? g[4] : g[3]);
    if (id == 0x65)
        return (struct connector *)(intptr_t)g[0];
    if (id == 0x27 || id == 0x28)
        return msf_log->pcm_conn;
    return NULL;
}
