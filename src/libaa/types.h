/*
 * Shared types for the libaa reconstruction (webOS 25 TV audio framework).
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 *
 * Every offset below was read from the shipped binary (Ghidra addresses):
 *   aud_module       aud_module_constructor @0x2d03c, aud_module_destructor @0x2d14c,
 *                    chain_locked @0x2c9c8 (bool flags at 0x88..0x8b, othread at 0xb8)
 *   aud_port         aud_module_new_output @0x2d8b8 (calloc 0x160), add_pending_obj_locked
 *                    @0x2af48 (pending array at 0xd8, current_first at 0x148)
 *   aud_module_impl  ModuleRegistry_Register @0x274cc / RegisterNamef @0x274fc (memcpy 0x60)
 *   class desc       aud_object_class_get @0x30928, aud_mregistry_add_class @0x274a4
 *   apipeline        apipeline_new @0x1b6bc (calloc 0x10), _apipeline_add @0x1b7cc (0x1c)
 *   aud_event/data   libaudcommon aud_event_new @0x120e0, aud_data_new @0x133ec
 * Field names follow the (older, GPL-dual-licensed header) LG kdriver tree where they still
 * fit; fields that only exist in this build are named unknown_<offset>.
 *
 * This header is shared by all libaa source files (aimc_local.h, buffers.h and objects.h
 * build on it; all four can be included together).
 */
#ifndef LIBAA_TYPES_H
#define LIBAA_TYPES_H

#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* aud_object / aud_event / aud_data / struct link / audutil_* (libaudcommon, A1) */
#include "../libaudcommon/audcommon.h"

#ifndef LINK_INIT
#define LINK_INIT(l)            link_init(l)
#endif
#ifndef LINK_INITIALIZER
#define LINK_INITIALIZER(l)     { &(l), &(l) }
#endif


/* ------------------------------------------------------------------------- */
/* logging (liblogm "logmm" modules, imported logmm_printl)                  */
/* ------------------------------------------------------------------------- */
enum log_level {
    log_level_error = 0,
    log_level_warn  = 1,
    log_level_noti  = 2,
    log_level_info  = 3,
    log_level_debug = 4,
    log_level_trace = 5,
};

/* 20-byte object exported as logmm_module_<name>; .mask is set at registration */
struct logmm_module {
    struct link list;
    int initial_level;
    unsigned int *mask;
    const char *name;
};

extern int logmm_printl(struct logmm_module *m, int level, const char *func, int line,
                        const char *fmt, ...) __attribute__((format(printf, 5, 6)));

#define LOGMM_ON(mod, lvl)  (*logmm_module_##mod.mask & (1u << (lvl)))
/* line = the __LINE__ the original was built with (kept as evidence) */
#define LOGMM(mod, lvl, line, fmt, ...) do { \
        if (LOGMM_ON(mod, lvl)) \
            logmm_printl(&logmm_module_##mod, (lvl), __func__, (line), fmt, ##__VA_ARGS__); \
    } while (0)
#define LOGE(mod, line, fmt, ...)   LOGMM(mod, log_level_error, line, fmt, ##__VA_ARGS__)
#define LOGW(mod, line, fmt, ...)   LOGMM(mod, log_level_warn,  line, fmt, ##__VA_ARGS__)
#define LOGN(mod, line, fmt, ...)   LOGMM(mod, log_level_noti,  line, fmt, ##__VA_ARGS__)
#define LOGI(mod, line, fmt, ...)   LOGMM(mod, log_level_info,  line, fmt, ##__VA_ARGS__)
#define LOGD(mod, line, fmt, ...)   LOGMM(mod, log_level_debug, line, fmt, ##__VA_ARGS__)
#define LOGT(mod, line, fmt, ...)   LOGMM(mod, log_level_trace, line, fmt, ##__VA_ARGS__)

/* like LOGMM, for log calls whose __func__ belongs to an inlined helper */
#define LOGF(mod, lvl, func, line, fmt, ...) do { \
        if (LOGMM_ON(mod, lvl)) \
            logmm_printl(&logmm_module_##mod, (lvl), (func), (line), fmt, ##__VA_ARGS__); \
    } while (0)

/* every logmm_module_* in .data is { {0,0}, 7, &logmm_dummy_mask, "<name>" } */
extern unsigned int logmm_dummy_mask;
#define LOGMM_MODULE_DEFINE(n) \
    struct logmm_module logmm_module_##n = { { 0, 0 }, 7, &logmm_dummy_mask, #n }

extern struct logmm_module logmm_module_aabase, logmm_module_acont, logmm_module_adbg,
    logmm_module_aimc, logmm_module_aipc, logmm_module_aud, logmm_module_aud_modlist,
    logmm_module_aud_obj, logmm_module_aud_pipeline, logmm_module_bin, logmm_module_buf_com,
    logmm_module_buf_event, logmm_module_buf_reader, logmm_module_buf_writer,
    logmm_module_com_imc, logmm_module_com_link, logmm_module_commod,
    logmm_module_man_registry, logmm_module_mod_bypass2, logmm_module_mod_connector,
    logmm_module_mod_fakesink, logmm_module_mod_glue, logmm_module_mod_gluer,
    logmm_module_mod_gluew, logmm_module_modstart, logmm_module_oswrap;

/* ------------------------------------------------------------------------- */
/* result codes (ADEC_RESULT, kdriver common/adec_error_def.h order)         */
/* ------------------------------------------------------------------------- */
enum adec_result {
    ADEC_ERR_NONE = 0,
    ADEC_ERR_GENERAL,               /* 1 */
    ADEC_ERR_RANGE,                 /* 2 */
    ADEC_ERR_NULL,                  /* 3 */
    ADEC_ERR_DATA_CORRUPTED,        /* 4 */
    ADEC_ERR_PERMISSION,            /* 5 */
    ADEC_ERR_EMPTY_QUEUE,           /* 6 */
    ADEC_ERR_OVERFLOW,              /* 7 */
    ADEC_ERR_UNDERFLOW,             /* 8 */
    ADEC_ERR_TIMEOUT,
    ADEC_ERR_UNKNOWN,
    ADEC_ERR_WAITING,
    ADEC_ERR_ALREADY_EXIST,
    ADEC_ERR_ALIGNMENT,
    ADEC_ERR_NOT_READY,
    ADEC_ERR_NOT_SUPPORT,           /* 15 */
    ADEC_ERR_NOT_ENOUGH_MEM,        /* 16 */
    ADEC_ERR_NOT_IMPLEMENTED,
    ADEC_ERR_NOT_DEFINED,
    ADEC_ERR_NOT_FOUND,             /* 19 */
    ADEC_ERR_OUT_OF_RESOURCE,
    ADEC_ERR_INVALID_LOCATION,
    ADEC_ERR_DATA_SIZE,             /* 22 */
    ADEC_ERR_STATE,
    ADEC_ERR_CONNECTION,
    ADEC_ERR_WRAPAROUND,
    ADEC_ERR_DUPLICATED,            /* 26 */
    ADEC_ERR_SYNC_FAIL,
    ADEC_ERR_SYNTAX,
    ADEC_ERR_NEED_MORE_DATA,
    ADEC_ERR_NEED_MORE_PROCESS,     /* 30 */
    ADEC_ERR_CORRUPTED,             /* 31 */
};

/* ------------------------------------------------------------------------- */
/* aud_link: locked list (libaa, link2.c)                                    */
/* ------------------------------------------------------------------------- */
struct aud_link_root {                       /* 0x28 bytes */
    struct link nodes;
    pthread_mutex_t rlock;
    int last_id;
    unsigned int node_count;
};

struct aud_link_node {                       /* 0x2c bytes */
    struct link root_link;
    pthread_mutex_t lock;
    struct aud_link_root *root;
    void *data;
    int id;
};

void aud_link_root_init(struct aud_link_root *root);
void aud_link_node_init(struct aud_link_node *node, void *data);
int aud_link_add(struct aud_link_root *root, struct aud_link_node *node);
int aud_link_add_tail(struct aud_link_root *root, struct aud_link_node *node);
int aud_link_add_locked(struct aud_link_root *root, struct aud_link_node *node);
int aud_link_add_tail_locked(struct aud_link_root *root, struct aud_link_node *node);
int aud_link_del_locked(struct aud_link_node *node);
int aud_link_del(struct aud_link_node *node);
int aud_link_foreach_node(struct aud_link_root *root, void *arg,
                          int (*cb)(void *arg, void *node_data));

/* ------------------------------------------------------------------------- */
/* object classes (objects.c)                                                */
/* ------------------------------------------------------------------------- */
struct aud_object_class_desc;

struct aud_object_class {
    struct aud_object_class_desc *desc;
};

struct aud_object_class_desc {
    struct link list;                        /* 0x00 at class_desc_root */
    struct link mreg_list;                   /* 0x08 at registry class list */
    struct aud_object_class *object_class;   /* 0x10 lazily created */
    struct aud_object_class_desc *parent;    /* 0x14 */
    const char *name;                        /* 0x18 */
    int sizeof_class;                        /* 0x1c */
    int (*class_init)(void *klass);          /* 0x20 */
};

struct aud_object_class *aud_object_class_get(struct aud_object_class_desc *desc);
struct aud_object_class *aud_object_class_gets(const char *name);
int aud_object_class_register(struct aud_object_class_desc *desc);

/* class descriptor list (objects.c) and the exported descriptors */
extern struct link class_desc_root;
extern struct aud_object_class_desc __aud_object_class_base;     /* objects.c */
extern struct aud_object_class_desc __aud_object_class_module;   /* module.c */
extern struct aud_object_class_desc __aud_object_class_bin;      /* module.c */
extern struct aud_object_class_desc __aud_object_class_binout;   /* module.c */

/* ------------------------------------------------------------------------- */
/* modules                                                                   */
/* ------------------------------------------------------------------------- */
struct aimc;
struct aud_module;
struct aud_port;
struct aud_starter;
struct aud_thread;
struct _ADEC_BUF_T;

enum aud_module_prop_type {
    param_type_none,
    param_type_bool,
    param_type_int,
    param_type_longlong,
    param_type_str,
    param_type_data,
    param_type_struct,
};

#define AUD_PROP_RO 0x1

struct aud_module_property {                 /* 0x28 bytes */
    const char *name;
    enum aud_module_prop_type type;
    int offset;                              /* into the module instance */
    unsigned int flag;
    int (*set)(struct aud_module *m, const struct aud_module_property *p,
               const void *data, unsigned int size);
    char *(*gets)(struct aud_module *m, const struct aud_module_property *p);
    int (*geti)(struct aud_module *m, const struct aud_module_property *p);
    void *priv;
    struct link list;                        /* 0x20 at impl->prop_list (dynamic props) */
};

struct aud_module_mem_def {
    unsigned int max_num_au;
    unsigned int max_access_size;
};

#define AUD_IMPL_HAVE_OUTPUT_THREAD 0x1
#define AUD_IMPL_SKIP_SYNC          0x2

struct aud_module_impl {                     /* 0x60 bytes */
    struct aud_object_class parent;          /* 0x00 */
    const char *name;                        /* 0x04 */
    int (*constructor)(struct aud_module *m, struct aimc *imc, const char *param); /* 0x08 */
    void (*destructor)(struct aud_module *m);                                       /* 0x0c */
    int (*chain)(struct aud_port *port, struct aud_data *data);                     /* 0x10 */
    int (*event)(struct aud_module *m, struct aud_port *port, struct aud_event *e); /* 0x14 */
    int (*cmd)(void *arg, unsigned int cmd, void *param, unsigned int len);         /* 0x18 */
    int (*prop_updated)(struct aud_module *m, const struct aud_module_property *p); /* 0x1c */
    int (*disconnect_port)(struct aud_module *m, struct aud_port *in_port);         /* 0x20 */
    int (*deleting_port)(struct aud_module *m, struct aud_port *out_port);          /* 0x24 */
    int (*dot)(struct aud_module *m);                                               /* 0x28 */
    int instance_size;                       /* 0x2c */
    struct aud_module_property *prop;        /* 0x30 static table, name==NULL terminated */
    struct link prop_list;                   /* 0x34 dynamically added props */
    struct link implement_link;              /* 0x3c at registry implements */
    struct link instances;                   /* 0x44 of aud_module::instance_link */
    unsigned int flag;                       /* 0x4c AUD_IMPL_* */
    void *impl_priv;                         /* 0x50 */
    struct aud_module_mem_def *mem_def;      /* 0x54 */
    int mod_priority;                        /* 0x58 */
    unsigned int initial_log_mask;           /* 0x5c (Register sets 7) */
};

#define AUD_PORT_OUTPUT     0x1
#define AUD_PORT_DISABLED   0x2
#define AUD_PORT_HAVE_BEEP  0x4

#define AUD_PORT_WORK_CHAIN 0x1

struct aud_port {                            /* calloc(0x160) */
    unsigned int number;                     /* 0x000 */
    struct aud_module *module;               /* 0x004 */
    unsigned int flag;                       /* 0x008 AUD_PORT_* */
    pthread_mutex_t stream_lock;             /* 0x00c */
    pthread_mutex_t async_down_lock;         /* 0x024 */
    pthread_mutex_t async_up_lock;           /* 0x03c */
    struct aud_link_node module_node;        /* 0x054 at module->{in,out}put_port_root */
    struct aud_link_node down_peer_node;     /* 0x080 (input) at up_peer->down_peer_root */
    struct aud_link_root down_peer_root;     /* 0x0ac (output) */
    struct aud_port *up_peer;                /* 0x0d4 (input) */
    struct aud_object **pending_objs;        /* 0x0d8 (output) */
    unsigned int pending_obj_alloced;        /* 0x0dc */
    unsigned int pending_obj_num;            /* 0x0e0 */
    unsigned int unknown_e4;
    pthread_cond_t check_free_cond;          /* 0x0e8 */
    pthread_mutex_t check_free_mutex;        /* 0x118 */
    int flushing;                            /* 0x130 */
    int throwing;                            /* 0x134 */
    struct link starter_link;                /* 0x138 */
    struct aud_starter *starter;             /* 0x140 */
    struct _ADEC_BUF_T *legacy_buf;          /* 0x144 */
    struct aud_event *current_first;         /* 0x148 */
    unsigned int passed;                     /* 0x14c bytes chained */
    unsigned long long last_clock;           /* 0x150 */
    unsigned int flag_work;                  /* 0x158 AUD_PORT_WORK_* */
};

#define AUD_PORT_IS_OUTPUT(p)   (!!((p)->flag & AUD_PORT_OUTPUT))
#define AUD_PORT_IS_DISABLED(p) (!!((p)->flag & AUD_PORT_DISABLED))

struct aud_module {                          /* base 0x108 bytes; impl->instance_size total */
    char *name;                              /* 0x00 "<impl>_<uid %08x>" */
    unsigned int uid;                        /* 0x04 */
    unsigned int log_mask;                   /* 0x08 */
    struct aud_module_impl *impl;            /* 0x0c */
    struct aud_link_root input_port_root;    /* 0x10 */
    struct aud_link_root output_port_root;   /* 0x38 */
    struct aimc *aimc;                       /* 0x60 */
    struct link instance_link;               /* 0x64 at impl->instances */
    pthread_mutex_t prop_access_lock;        /* 0x6c */
    int thread_priority;                     /* 0x84 */
    bool drop_output_data;                   /* 0x88 */
    bool drop_output_meta;                   /* 0x89 */
    bool block_module;                       /* 0x8a */
    bool do_chain_profile;                   /* 0x8b */
    unsigned long long chain_profile;        /* 0x90 */
    unsigned long long chain_profile_last;   /* 0x98 */
    struct aud_event *first_event_out;       /* 0xa0 */
    bool othread_running;                    /* 0xa4 */
    int (*othread_callback)(struct aud_module *m, void *priv); /* 0xa8 */
    void *othread_callback_priv;             /* 0xac */
    pthread_mutex_t *othread_lock;           /* 0xb0 */
    pthread_cond_t *othread_kick;            /* 0xb4 */
    struct aud_thread *othread;              /* 0xb8 */
    unsigned int unknown_bc;
    pthread_cond_t block_cond;               /* 0xc0 */
    pthread_mutex_t block_mutex;             /* 0xf0 */
};

struct aud_module_bin {
    struct aud_module module;
    struct link inst_root;                   /* 0x108 struct aud_bin_inst */
    struct link cmd_propa_root;              /* 0x110 struct aud_bin_inst (propagate list) */
    struct link input_port_root;             /* 0x118 struct aud_bin_port */
    struct link output_port_root;            /* 0x120 struct aud_bin_port */
    struct link pending_cmd_root;            /* 0x128 struct aud_bin_pending_cmd */
};

struct aud_bin_inst {                        /* calloc(0x14) */
    struct link list;
    struct aud_module *inst;                 /* 0x08 */
    const char *name;                        /* 0x0c impl name */
    int index;                               /* 0x10 n-th instance of that impl in the bin */
};

struct aud_bin_port {                        /* calloc(0x18) */
    struct link list;
    struct aud_module *module;               /* 0x08 inner module */
    struct aud_port *port;                   /* 0x0c inner module's port */
    int bin_port;                            /* 0x10 port number seen from outside */
    int active;                              /* 0x14 last seen "have active output" (-1 = unknown; mixerbin) */
};

struct aud_bin_pending_cmd {
    struct link list;
    unsigned int cmd;                        /* 0x08 */
    unsigned int len;                        /* 0x0c */
    void *param;                             /* 0x10 malloc'd */
};

/* "bin" class: struct aud_module_impl followed by four bin hooks (sizeof_class 0x74) */
struct aud_module_bin_impl {
    struct aud_module_impl parent;
    int (*bin_initialize)(struct aud_module *m);                                    /* 0x60 */
    void (*bin_cleanup)(struct aud_module *m);                                      /* 0x64 */
    int (*bin_event)(struct aud_module *m, struct aud_port *p, struct aud_event *e); /* 0x68 */
    int (*bin_cmd)(void *arg, unsigned int cmd, void *param, unsigned int len);     /* 0x6c */
    unsigned int unknown_70;                 /* never touched; class size is 0x74 */
};

/* the "binout" helper module: forwards inner output to the bin's own output port */
struct aud_module_binout {
    struct aud_module module;
    struct aud_module_bin *bin;              /* 0x108 */
    int bin_port;                            /* 0x10c */
};

/* module core API (module.c) */
struct aud_module *aud_module_constructor(struct aimc *imc, struct aud_module_impl *impl,
                                          unsigned int uid, const char *param);
void aud_module_destructor(struct aud_module *m);
struct aud_port *aud_module_get_input_port(struct aud_module *m, unsigned int number);
struct aud_port *aud_module_get_output_port(struct aud_module *m, unsigned int number);
struct aud_port *aud_module_get_input(struct aud_module *m, unsigned int number);
struct aud_port *aud_module_get_output(struct aud_module *m, unsigned int number);
#define aud_module_put_port(port) pthread_mutex_unlock(&(port)->module_node.lock)
int aud_module_have_input(struct aud_module *m, unsigned int number);
int aud_module_have_output(struct aud_module *m, unsigned int number);
int aud_module_have_active_output(struct aud_module *m);
int aud_module_have_active_output_n(struct aud_module *m, unsigned int number);
struct aud_port *aud_module_new_input_port(struct aud_module *m, unsigned int number);
struct aud_port *aud_module_new_output_port(struct aud_module *m, unsigned int number);
struct aud_port *aud_module_new_input(struct aud_module *m, unsigned int number);
struct aud_port *aud_module_new_output(struct aud_module *m, unsigned int number);
void aud_module_del_input_port(struct aud_module *m, unsigned int number);
void aud_module_del_output_port(struct aud_module *m, unsigned int number);
int aud_module_connect(struct aud_port *out_port, struct aud_port *in_port);
void aud_module_disconnect(struct aud_port *out_port, struct aud_port *in_port);
int aud_module_chain(struct aud_port *port, struct aud_data *data);
int aud_module_event(struct aud_module *m, struct aud_port *port, struct aud_event *e);
int aud_module_default_event(struct aud_module *m, struct aud_port *port, struct aud_event *e);
int aud_module_propagate_event(struct aud_module *m, struct aud_event *e);
int aud_module_cmd(struct aud_module *m, unsigned int cmd, void *param, unsigned int len);
int aud_module_default_command(struct aud_module *m, unsigned int cmd, void *param,
                               unsigned int len);
int aud_module_dot(struct aud_module *m);
int aud_module_default_dot(struct aud_module *m);
int aud_module_default_dot_wo_proc(struct aud_module *m);
int aud_module_push_data(struct aud_module *m, int out_port_num, struct aud_data *data);
int aud_module_push_event(struct aud_module *m, int out_port_num, struct aud_event *e);
int aud_module_sync(struct aud_module *m, int out_port_num);
int aud_port_push_data(struct aud_port *port, struct aud_data *data);
int aud_port_push_event(struct aud_port *port, struct aud_event *e);
int aud_port_sync(struct aud_port *port);
int aud_port_check_legacy(struct aud_port *port);
int aud_module_check_legacy_output(struct aud_module *m, unsigned int number);
long long aud_module_get_prop(struct aud_module *m, const char *name);
int aud_module_set_prop(struct aud_module *m, char *str, int len);
char *aud_module_prop_str(struct aud_module *m, const char *fmt);
typedef int (*aud_prop_set_fn)(struct aud_module *m, const struct aud_module_property *p,
                               const void *data, unsigned int size);
typedef char *(*aud_prop_gets_fn)(struct aud_module *m, const struct aud_module_property *p);
typedef int (*aud_prop_geti_fn)(struct aud_module *m, const struct aud_module_property *p);
struct aud_module_property *aud_module_impl_add_prop2(struct aud_module_impl *impl,
    const char *name, enum aud_module_prop_type type, int offset, aud_prop_set_fn set,
    aud_prop_gets_fn gets, aud_prop_geti_fn geti, unsigned int flag, void *priv);
int aud_module_impl_add_prop(struct aud_module_impl *impl, const char *name,
                             enum aud_module_prop_type type, int offset, aud_prop_set_fn set,
                             aud_prop_gets_fn gets, unsigned int flag);
int aud_prop_get_value_int(const char *str, const char *name, int def);
bool aud_prop_get_value_bool(const char *str, const char *name, bool def);
char *aud_prop_get_value_str(const char *str, const char *name, const char *def);
/* cb runs on the module's own thread with *lock held; it must wait on *kick itself */
int aud_module_output_thread_create(struct aud_module *m,
                                    int (*cb)(struct aud_module *m, void *priv), void *priv,
                                    pthread_mutex_t *lock, pthread_cond_t *kick);
int aud_module_output_thread_destroy(struct aud_module *m);

/* bin (container module) */
struct aud_module *aud_module_bin_add(struct aud_module_bin *bin, const char *inst_name);
int aud_module_bin_del(struct aud_module_bin *bin, struct aud_module *inst);
struct aud_module *aud_module_bin_search(struct aud_module_bin *bin, const char *name, int index);
struct aud_module *aud_module_bin_search_index(struct aud_module_bin *bin, int index);
struct aud_module *aud_module_bin_search_name(struct aud_module_bin *bin, const char *name);
struct aud_module *aud_module_bin_clear(struct aud_module_bin *bin);
int aud_module_bin_connect(struct aud_module_bin *bin, struct aud_module *out, int port_out,
                           struct aud_module *in, int port_in);
int aud_module_bin_set_input(struct aud_module_bin *bin, int port_bin_in,
                             struct aud_module *in, int port_in);
int aud_module_bin_unset_input(struct aud_module_bin *bin, int port_bin_in);
int aud_module_bin_set_output(struct aud_module_bin *bin, int port_bin_out,
                              struct aud_module *out, int port_out);
int aud_module_bin_unset_output(struct aud_module_bin *bin, int port_bin_out);
int aud_module_bin_unset_output_no_del_port(struct aud_module_bin *bin, int port_bin_out);
int aud_module_bin_add_cmd_propagate(struct aud_module_bin *bin, struct aud_module *m);
int aud_module_bin_propagate_pending(struct aud_module_bin *bin);

/* ------------------------------------------------------------------------- */
/* registry / connector (registry.c)                                         */
/* ------------------------------------------------------------------------- */
struct registry {
    struct aimc *imc;
};

struct aud_connector {                       /* calloc(0x110) */
    unsigned char unknown_000[0x108];
    struct aimc *imc;                        /* 0x108 */
    unsigned int handler_uid;                /* 0x10c (proc_id << 24 | 2) */
};

/* internal-module index entry (registry "internal_list") */
struct aud_internal_inst {                   /* calloc(0x10) */
    struct link list;
    int iindex;                              /* uid = proc<<24 | 0xf000 + iindex */
    struct aud_module *module;
};

/* registry lists (static DAT_ in the binary: @0x4c3f8 / @0x4c400 / @0x4c408, @0x5119c) */
extern struct link module_implements;   /* aud_module_impl::implement_link */
extern struct link mreg_classes;        /* aud_object_class_desc::mreg_list */
extern struct link internal_instances;  /* aud_internal_inst::list */
extern int internal_index_last;
int ModuleRegistry_ProcessCmd(void *arg, unsigned int cmd, void *param, unsigned int len);
int add_implement(struct aimc *imc, struct aud_module_impl *impl);
void send_list_internal_module(struct aimc *imc, const char *name, unsigned int uid, int add);

struct registry *ModuleRegistry_Init(struct aimc *imc);
void ModuleRegistry_Register(struct aud_module_impl *impl);
void ModuleRegistry_RegisterNamef(struct aud_module_impl *templ, void *priv,
                                  const char *name_fmt, ...)
    __attribute__((format(printf, 3, 4)));
void ModuleRegistry_Cleanup(struct registry *r);
struct aud_module *ModuleRegistry_SearchInstByUid(unsigned int uid);
struct aud_module *ModuleRegistry_SearchInstByName(const char *name);
struct aud_module_impl *ModuleRegistry_SearchModuleImpl(const char *name);
struct aud_module *aud_registry_create_instance_param(struct aimc *imc, const char *impl_name,
                                                      unsigned int uid, const char *param);
struct aud_module *aud_registry_create_instance(struct aimc *imc, const char *impl_name,
                                                unsigned int uid);
void aud_registry_destroy_instance(struct aimc *imc, struct aud_module *m);
void aud_mregistry_add_class(struct aud_object_class_desc *desc);
struct aud_connector *Connector_Create(int processor_id, struct aimc *imc);
void Connector_Destroy(struct aud_connector *c);
int aud_connector_connect(struct aud_module *out, int port_out, struct aud_module *in,
                          int port_in);
int aud_connector_disconnect(struct aud_module *out, int port_out, struct aud_module *in,
                             int port_in);

/* ------------------------------------------------------------------------- */
/* user side pipeline (apipeline.c)                                          */
/* ------------------------------------------------------------------------- */
struct apipeline;
struct apipeline_element;

struct apipeline_event_desc {
    struct link list;
    const char *event_name;
    const char *func_name;
    const char *func_arg;
    struct apipeline *pipe;
    struct apipeline_element *inst;
};

struct apipeline_element {                   /* calloc(0x1c) */
    struct link list;                        /* 0x00 at apipeline::element_list */
    struct link events;                      /* 0x08 */
    unsigned int uid;                        /* 0x10 */
    char *name;                              /* 0x14 "<impl>_<uid %08x>" */
    char *alias;                             /* 0x18 */
};

struct apipeline {                           /* calloc(0x10) */
    char *name;                              /* 0x00 */
    struct aimc *imc;                        /* 0x04 */
    struct link element_list;                /* 0x08 head = upstream, tail = downstream */
};

typedef int (*arocket_assign_fn)(struct apipeline *pipe, void *priv, unsigned int uid,
                                 const char *name, const char *value);

struct apipeline *apipeline_new(struct aimc *imc, const char *name, ...);
void apipeline_del(struct apipeline *pipe);
struct apipeline_element *apipeline_search(struct apipeline *pipe, const char *name);
struct apipeline_element *_apipeline_add(struct apipeline *pipe, const char *impl_name,
                                         unsigned int uid, bool at_tail);
struct apipeline_element *apipeline_add_at(struct apipeline *pipe, const char *name, int proc);
struct apipeline_element *apipeline_add(struct apipeline *pipe, const char *name, int proc);
unsigned int apipeline_addf(struct apipeline *pipe, const char *fmt, ...);
unsigned int apipeline_head(struct apipeline *pipe);
unsigned int apipeline_tail(struct apipeline *pipe);
unsigned int apipeline_connect(struct apipeline *out, int port_out, struct apipeline *in,
                               int port_in);
unsigned int apipeline_disconnect(struct apipeline *pipe, int port);
unsigned int apipeline_prepend_uid_port(struct apipeline *pipe, int port_in,
                                        unsigned int uid_out, int port_out);
unsigned int apipeline_prepend_uid(struct apipeline *pipe, unsigned int uid_out);
unsigned int apipeline_append_uid_port(struct apipeline *pipe, int port_out,
                                       unsigned int uid_in, int port_in);
unsigned int apipeline_append_uid(struct apipeline *pipe, unsigned int uid_in);
unsigned int apipeline_prependfv_port(struct apipeline *pipe, int port_in, int port_out,
                                      const char *fmt, va_list ap);
unsigned int apipeline_prependf_port(struct apipeline *pipe, int port_in, int port_out,
                                     const char *fmt, ...);
unsigned int apipeline_prependf(struct apipeline *pipe, const char *fmt, ...);
unsigned int apipeline_appendfv_port(struct apipeline *pipe, int port_out, int port_in,
                                     const char *fmt, va_list ap);
unsigned int apipeline_appendf_port(struct apipeline *pipe, int port_out, int port_in,
                                    const char *fmt, ...);
unsigned int apipeline_appendf(struct apipeline *pipe, const char *fmt, ...);
const char *apipeline_get_name(struct apipeline *pipe);

int arocket_descv(struct apipeline *pipe, void *priv, arocket_assign_fn assign, char **argv);
int arocket_desc(struct apipeline *pipe, void *priv, arocket_assign_fn assign, const char *desc);
int arocket_vdescf(struct apipeline *pipe, void *priv, arocket_assign_fn assign,
                   const char *descf, va_list ap);
int arocket_descf(struct apipeline *pipe, void *priv, arocket_assign_fn assign,
                  const char *descf, ...) __attribute__((format(printf, 4, 5)));

/* ------------------------------------------------------------------------- */
/* aimc / module list / debug prototypes shared by all files                 */
/* ------------------------------------------------------------------------- */
unsigned int aimc_module_create(struct aimc *imc, const char *impl_name, int preferred_proc);
int aimc_module_destroy(struct aimc *imc, unsigned int uid);
int aimc_module_connect(struct aimc *imc, unsigned int uid_out, int port_out,
                        unsigned int uid_in, int port_in);
int aimc_module_disconnect_output(struct aimc *imc, unsigned int uid, int port);
int aimc_module_disconnect_input(struct aimc *imc, unsigned int uid, int port);
int aimc_get_processor_id(struct aimc *imc);
int aimc_register_command_handler(struct aimc *imc, unsigned int uid,
                                  int (*cb)(void *arg, unsigned int cmd, void *param,
                                            unsigned int len),
                                  void *arg);
int aimc_unregister_command_handler(struct aimc *imc, unsigned int uid);
void aimc_clear_named_event_on_uid(struct aimc *imc, unsigned int uid);
void aimc_wait_until_process_header(struct aimc *imc);
int aimc_add_processor(struct aimc *imc);
int aimc_add_processor_done(struct aimc *imc);
int aimc_send_command(struct aimc *imc, unsigned int cmd, unsigned int source,
                      unsigned int target, void *param, unsigned int param_size);
int aimc_send_command_sync(struct aimc *imc, unsigned int cmd, unsigned int source,
                           unsigned int target, void *param, unsigned int param_size);
int aimc_set_property(struct aimc *imc, unsigned int target, const char *prop_fmt, ...)
    __attribute__((format(printf, 3, 4)));
int aimc_handler_sync(struct aimc *imc);
int aimc_add_implement(struct aimc *imc, const char *name, unsigned int rank);
int aimc_add_port_buf_def(struct aimc *imc, const char *name, int def_num, int max_num_au,
                          int max_access_size);
unsigned int aimc_lmodule_create(struct aimc *imc, unsigned int module_id);
int aimc_lmodule_destroy(struct aimc *imc, unsigned int module_id);
/* cb(arg, data, size) runs on the aimc handler thread; repeat = 0: one-shot */
int aimc_register_named_event(struct aimc *imc, const char *name, unsigned int uid,
                              int repeat, int (*cb)(void *arg, void *data, unsigned int size),
                              void *arg);
void aimc_unregister_named_event(struct aimc *imc, int evt_id);
void *aimc_mmap(struct aimc *imc, unsigned int offset, unsigned int size);
void aimc_munmap(struct aimc *imc, unsigned int offset, void *addr, unsigned int size);
int aimc_cache_sync(struct aimc *imc, int to_device, unsigned int shbuf_offset,
                    unsigned int offset, unsigned int size);
int aimc_role_set(struct aimc *imc, unsigned int uid, const char *name);
unsigned int aimc_role_search(struct aimc *imc, const char *name);

/* static legacy module list (modlist.c) */
unsigned int ModuleList_GetPlatform(void);                /* ADEC_CORE_TYPE */
unsigned int ModuleList_FindModule(unsigned int muid);    /* uid -> processor id */
const char *ModuleList_GetModuleName(unsigned int module);

/* debug.c */
struct aipc;
struct adebug;
struct adebug *adebug_create(struct aipc *ipc);
void adebug_destroy(struct adebug *dbg);
int adebug_set_aimc(struct aimc *imc);

/* misc (util.c) */
char *autil_option_str_from_file(const char *basename);
char *autil_option_str_from_env(const char *basename);
char *autil_option_str(const char *basename);
int autil_option_int(const char *basename, int def);
char **aabase_string_vector(const char *str);
void aabase_free_vector(char **v);

/* ------------------------------------------------------------------------- */
/* implemented in other libaa files / libaudcommon; prototypes used by module.c */
/* ------------------------------------------------------------------------- */
struct aud_thread *aud_thread_create(const char *name, int stack_size, int priority,
                                     int (*fn)(void *arg), void *arg);
void aud_thread_destroy(struct aud_thread *t);
unsigned long long aud_system_cycle(void);   /* CLOCK_MONOTONIC in ns */
long long aud_thread_cycle(void);            /* thread CPU clock */
void aud_starter_del_port(struct aud_starter *s, struct aud_port *port);
struct _ADEC_BUF_T *aud_starter_get_lbuf(struct aud_starter *s);
int aimc_raise_named_event(struct aimc *imc, unsigned int uid, const char *name,
                           void *data, unsigned int size);
const char *IMC_CommandToString(unsigned int cmd);

/* module.c debug switches (table "module_debug_opts" @0x5102c, used by debug_module) */
extern struct audutil_opt aud_module_debug_opts[];
extern bool aud_module_sync_at_push;         /* @0x511a2 */
extern bool aud_module_skip_ext_dump;        /* @0x511a0 */
extern int aud_module_silence_noise;         /* @0x50ff0, default 0x10000 */
extern bool aud_module_do_check_beep;        /* @0x511a1 */

/* ------------------------------------------------------------------------- */
/* element list (acont side: which processor implements what, instance uids) */
/* ------------------------------------------------------------------------- */
struct aud_mod_impl_port_buf_def {
    int max_num_au;
    int max_access_size;
};

struct aud_impl_helper {                     /* calloc(0x1c) */
    struct link list;
    const char *impl_name;                   /* 0x08 */
    void *(*create_pre)(const char *impl_name, unsigned int uid);              /* 0x0c */
    int (*create_post)(void *priv, const char *impl_name, unsigned int uid);   /* 0x10 */
    int (*destroy_pre)(void *priv, const char *impl_name, unsigned int uid);   /* 0x14 */
    int (*destroy_post)(void *priv, const char *impl_name, unsigned int uid);  /* 0x18 */
};

struct aud_proc_info {                       /* calloc(0x50) */
    struct link proc_list;                   /* 0x00 */
    struct link mods;                        /* 0x08 struct aud_mod_impl */
    pthread_mutex_t mods_lock;               /* 0x10 */
    int cur_impl_num;                        /* 0x28 starts at 0xf, max 0xff */
    int proc_id;                             /* 0x2c */
    char name[32];                           /* 0x30 "processor_%d" */
};

struct aud_mod_impl {                        /* calloc(strlen(name) + 0x45) */
    struct link mods_list;                   /* 0x00 at aud_proc_info::mods */
    struct link instances;                   /* 0x08 aud_mod_inst::impl_list */
    struct aud_proc_info *processor;         /* 0x10 */
    int impl_num;                            /* 0x14 uid bits 16..23 */
    int alloc_count;                         /* 0x18 */
    int last_index;                          /* 0x1c uid bits 0..15 */
    int rank;                                /* 0x20 highest rank wins */
    pthread_mutex_t lock;                    /* 0x24 */
    int num_mem_defs;                        /* 0x3c */
    struct aud_mod_impl_port_buf_def *mem_defs; /* 0x40 */
    char name[];                             /* 0x44 */
};

struct aud_mod_owner {                       /* calloc(0x78) */
    struct link instances;                   /* 0x00 aud_mod_inst::owner_list */
    pthread_mutex_t instances_lock;          /* 0x08 */
    unsigned int flags;                      /* 0x20 bit0: has dead instance */
    unsigned int unknown_24;
    pthread_cond_t flags_cond;               /* 0x28 */
    char name[32];                           /* 0x58 */
};

struct aud_mod_inst {                        /* calloc(strlen(impl) + 0x6a) */
    struct link owner_list;                  /* 0x00 */
    struct link impl_list;                   /* 0x08 */
    pthread_mutex_t lock;                    /* 0x10 */
    int users;                               /* 0x28 */
    unsigned int uid;                        /* 0x2c proc<<24 | impl_num<<16 | index */
    struct link input_ports;                 /* 0x30 */
    struct link output_ports;                /* 0x38 */
    struct aud_mod_owner *owner;             /* 0x40 */
    struct aud_mod_impl *mod_impl;           /* 0x44 */
    unsigned int ext_buf_id;                 /* 0x48 */
    char *role;                              /* 0x4c */
    struct aud_impl_helper *helper;          /* 0x50 */
    void *helper_priv;                       /* 0x54 */
    int num_mem_defs;                        /* 0x58 */
    struct aud_mod_impl_port_buf_def *mem_defs; /* 0x5c */
    char name[];                             /* 0x60 "<impl>_<uid %08x>" */
};

struct aud_proc_info *aud_modlist_register_processor(int *index);
int aud_modlist_unregister_processor(struct aud_proc_info *p);
struct aud_mod_impl *aud_modlist_register_impl(struct aud_proc_info *p, const char *name, int rank);
struct aud_mod_owner *aud_modlist_new_ownerv(const char *name, va_list ap);
struct aud_mod_owner *aud_modlist_new_owner(const char *name, ...)
    __attribute__((format(printf, 1, 2)));
void aud_modlist_del_owner(struct aud_mod_owner *o);
int aud_modlist_register_impl_port_buf_def(struct aud_proc_info *p, const char *impl_name,
                                           int port, int max_num_au, int max_access_size);
struct aud_mod_inst *aud_modlist_create_inst(struct aud_mod_owner *o, const char *impl_name,
                                             int preferred_proc, bool sync);
void aud_modlist_destroy_inst(struct aud_mod_inst *inst);
int aud_modlist_role_set(struct aud_mod_inst *inst, const char *role);
struct aud_mod_inst *aud_modlist_search(struct aud_mod_owner *o, const char *impl_name, int index);
int aud_modlist_connect_inst(struct aud_mod_owner *o, struct aud_mod_inst *out, int port_out,
                             struct aud_mod_inst *in, int port_in, bool sync);
int aud_modlist_disconnect_inst_output(struct aud_mod_owner *o, struct aud_mod_inst *out, int port);
int aud_modlist_disconnect_inst_input(struct aud_mod_owner *o, struct aud_mod_inst *in, int port);
int aud_modlist_set_aimc(struct aimc *imc);
unsigned int aud_inst_create(struct aud_mod_owner *o, const char *impl_name, int proc, bool sync);
void aud_inst_destroy(struct aud_mod_owner *o, unsigned int uid);
int aud_inst_connect(struct aud_mod_owner *o, unsigned int uid_out, int port_out,
                     unsigned int uid_in, int port_in, bool sync);
int aud_inst_disconnect_output(struct aud_mod_owner *o, unsigned int uid, int port);
int aud_inst_disconnect_input(struct aud_mod_owner *o, unsigned int uid, int port);
void aud_mod_name(struct aud_mod_owner *o, unsigned int uid, char *name);
void *aud_impl_register_helper(const char *impl_name,
    void *(*create_pre)(const char *, unsigned int),
    int (*create_post)(void *, const char *, unsigned int),
    int (*destroy_pre)(void *, const char *, unsigned int),
    int (*destroy_post)(void *, const char *, unsigned int));
void aud_impl_unregister_helper(const char *impl_name);
int acont_init(void);
int acont_start(void);
int acont_set_aimc(struct aimc *imc);

/* element-list internals defined in apipeline.c, shared with objects.c */
extern struct link processors;
extern pthread_mutex_t processors_lock;
extern pthread_mutex_t connection_lock;
extern struct aimc *gimc;
struct aud_mod_impl *search_modimpl(const char *name, int proc);
struct aud_mod_inst *owner_inst_by_uid(struct aud_mod_owner *o, unsigned int uid);
struct aud_mod_impl *register_impl(struct aud_proc_info *p, const char *name, int rank,
                                   int impl_num);
struct aud_mod_inst *create_inst_on_sync(struct aud_mod_impl *impl, struct aud_mod_owner *o,
                                         bool sync);
void remove_from_processor(struct aud_mod_inst *inst);
void put_instance(struct aud_mod_inst *inst);
int aud_inst_role_set(struct aud_proc_info *p, unsigned int uid, const char *role);

#endif /* LIBAA_TYPES_H */
