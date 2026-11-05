/*
 * Object/instance/thread layer and built-in module types for libaa.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 *
 * Shared types and the aimc/registry prototypes come from types.h. Everything declared
 * here is shared by objects.c, modules_builtin.c and debug.c.
 * Offsets are Ghidra-observed (see the @tags in the .c files).
 */
#ifndef LIBAA_OBJECTS_H
#define LIBAA_OBJECTS_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

#include "types.h"

/* ------------------------------------------------------------------------- */
/* logging helpers                                                           */
/* ------------------------------------------------------------------------- */

/* per-instance logger (logmm_printlm from audcommon.h): gated by aud_module::log_mask */
#define PLOG(m, lvl, line, fmt, ...) do { \
        if ((m)->log_mask & (1u << (lvl))) \
            logmm_printlm((m)->name, (lvl), __func__, (line), fmt, ##__VA_ARGS__); \
    } while (0)

/* command ids (ADEC_CMD_SIMP(id) = id << 16, ADEC_CMD(id, T) = id << 16 | sizeof T) */
#define GLUE_CMD_SET_SHARED_BUFFER  0xf1000014u   /* GlueCmdSetSharedBuffer, 0x14 bytes */
#define GLUE_CMD_SET_ENABLE         0xf1010000u
#define GLUE_CMD_UPDATE             0x67000114u   /* ('g'<<24)|(1<<8)|0x14 */

/* legacy shared buffer: ADEC_BUF_T, InterBufReader/Writer_* */
#include "buffers.h"

/* ------------------------------------------------------------------------- */
/* aud_thread / starter                                                      */
/* ------------------------------------------------------------------------- */
struct aud_thread {                          /* calloc(strlen(name) + 0xd) */
    pthread_t thread;
    int (*func)(void *arg);                  /* 0x04 */
    void *arg;                               /* 0x08 */
    char name[];                             /* 0x0c */
};
void aud_cond_wait_timeout(pthread_cond_t *cond, pthread_mutex_t *mutex, int usec);

struct aud_starter {                         /* calloc(0x80) */
    struct link ports;                       /* 0x00 of aud_port::starter_link */
    pthread_mutex_t ports_lock;              /* 0x08 */
    struct aud_thread *thread;               /* 0x20 */
    struct _ADEC_BUF_T *lbuf;                /* 0x24 */
    char *name;                              /* 0x28 */
    int stop;                                /* 0x2c */
    pthread_mutex_t thread_mutex;            /* 0x30 */
    pthread_cond_t thread_cond;              /* 0x48 */
    bool split_meta_pcm;                     /* 0x78 (always set true by aud_starter_new) */
};

/* object classes (objects.c); dump_class is debug_object's "class" callback */
int dump_class(void *priv, struct aud_object_class *klass);
int aud_object_class_foreach(int (*each)(void *priv, struct aud_object_class *klass),
                             void *priv);

struct aud_starter *aud_starter_new(struct _ADEC_BUF_T *lbuf, const char *name_fmt, ...)
    __attribute__((format(printf, 2, 3)));
void aud_starter_trigger(struct aud_starter *s);
void aud_starter_stop(struct aud_starter *s);
void aud_starter_add(struct aud_starter *s, struct link *port_link);
void aud_starter_add_port(struct aud_starter *s, struct aud_port *port);
void aud_starter_del(struct aud_starter *s);

/* ------------------------------------------------------------------------- */
/* element_list port/jumper records (connect_in_processor @0x19220,           */
/* aud_modlist_connect_inst @0x1acec). Lists/locks/helpers come from types.h.  */
/* ------------------------------------------------------------------------- */
struct jumper;

struct port {                                /* calloc(0x38) */
    struct link port_list;                   /* 0x00 at aud_mod_inst::{in,out}put_ports */
    bool input;                              /* 0x08 (never set on this build) */
    int index;                               /* 0x0c */
    struct link down_peers;                  /* 0x10 (output) of port::down_peer_list */
    struct link down_peer_list;              /* 0x18 (input) at up_peer->down_peers */
    struct port *up_peer;                    /* 0x20 (input) */
    struct jumper *input_jumper;             /* 0x24 (input) */
    struct link output_jumpers;              /* 0x28 (output) of jumper::jumper_list */
    struct aud_mod_inst *inst;               /* 0x30 */
    struct aud_mod_inst *glue;               /* 0x34 */
};

struct jumper {                              /* calloc(0x30) */
    unsigned long long dsp_shared;           /* 0x00 libhma physical address */
    unsigned int offset;                     /* 0x08 dsp_shared + 0x10000000 - hma base */
    char *shm_name;                          /* 0x0c "shm:<out>-<in>" (ARM<->ARM only) */
    struct aud_mod_inst *output_glue;        /* 0x10 gluew on the producer processor */
    struct link jumper_list;                 /* 0x14 at output port::output_jumpers */
    struct port *output_port;                /* 0x1c */
    struct aud_mod_inst *input_glue;         /* 0x20 gluer on the consumer processor */
    struct port *input_port;                 /* 0x24 */
    struct link all_jumpers_list;            /* 0x28 at all_jumpers @0x4c048 */
};

#define JUMPER_SHBUF_SIZE 0x40000

extern struct link all_jumpers;             /* struct jumper::all_jumpers_list */

int connect_in_processor(struct aud_mod_inst *out, int port_out, struct aud_mod_inst *in,
                         int port_in);                                      /* @0x19220 */
void destroy_glue(struct aud_mod_inst *glue);                               /* @0x19080 */
void disconnect_in_processor(struct aud_mod_inst *out, struct port *port_out,
                             struct aud_mod_inst *in, struct port *port_in); /* @0x198b4 */
void destroy_jumper(struct jumper *j);                                      /* @0x19a30 */

/* libhma (shared physical memory allocator) */
unsigned long long libhma_alloc(const char *pool, unsigned int size, unsigned int align, int flags);
void libhma_free(unsigned long paddr);   /* only the low word is passed (@0x19b70) */
int libhma_get_info(const char *pool, unsigned long long *base, unsigned int *size);

/* ------------------------------------------------------------------------- */
/* built-in module instances (modules_builtin.c)                             */
/* ------------------------------------------------------------------------- */
struct bypass2 {                             /* instance_size 0x128 */
    struct aud_module module;
    int data_passed;                         /* 0x108 prop "data-passed" (RO) */
    int event_after_set;                     /* 0x10c prop "event-after-set" (RO) */
    int event_after;                         /* 0x110 prop "event-after" */
    int rate;                                /* 0x114 from FIRST_AU "rate" (48000) */
    int channels;                            /* 0x118 from FIRST_AU "channels" (2) */
    bool sleep_sync;                         /* 0x11c prop "sleep-sync" */
    long long chain_start;                   /* 0x120 */
};

/* in-band record written into the gluew/gluer shm ring */
enum glue_data_type { glue_data_type_data, glue_data_type_event };

struct glue_header {                         /* 0x0c */
    enum glue_data_type type;
    unsigned int size;
    int au_count;
};

struct glue_header_data {                    /* 0x30 */
    struct glue_header header;
    unsigned int unknown_0c;
    struct aud_data_info info;               /* 0x10 copy of aud_data::info */
};

struct glue_cmd_update_param {               /* GLUE_CMD_UPDATE payload, 0x14 */
    unsigned int offs_rd;
    unsigned int offs_wr;
    int au_count;
    unsigned int size;
    unsigned int age;
};

struct gluer;
struct process_state {
    const char *name;
    unsigned int (*process)(struct gluer *g, void *mem, unsigned int size); /* bytes consumed */
    int (*changeto)(struct gluer *g);
};

struct gluer {                               /* instance_size 0x1c8 */
    struct aud_module module;
    char *shm_name;                          /* 0x108 prop "shm-name" */
    void *shm_mem;                           /* 0x10c */
    unsigned int shm_size;                   /* 0x110 prop "shm-size" */
    unsigned int shm_rd;                     /* 0x114 prop "shm-rd" (RO); ~0 = reset */
    unsigned int shm_wr;                     /* 0x118 prop "shm-wr" */
    unsigned int shm_age;                    /* 0x11c */
    int au_count;                            /* 0x120 */
    unsigned int cowork_uid;                 /* 0x124 prop "cowork-uid" (gluew uid) */
    pthread_cond_t shm_cond;                 /* 0x128 */
    pthread_mutex_t shm_lock;                /* 0x158 */
    struct process_state state;              /* 0x170 */
    unsigned int unknown_17c;
    struct glue_header_data header;          /* 0x180 */
    int header_gathered;                     /* 0x1b0 */
    struct aud_data *gathering_data;         /* 0x1b4 */
    unsigned int data_gathered;              /* 0x1b8 */
    void *gathering_event;                   /* 0x1bc */
    unsigned int gathering_event_allocated;  /* 0x1c0 */
    unsigned int event_gathered;             /* 0x1c4 */
};

struct gluew {                               /* instance_size 0x180 */
    struct aud_module module;
    char *shm_name;                          /* 0x108 */
    void *shm_mem;                           /* 0x10c */
    unsigned int shm_size;                   /* 0x110 */
    unsigned int shm_rd;                     /* 0x114 */
    unsigned int shm_wr;                     /* 0x118 (RO) */
    unsigned int shm_age;                    /* 0x11c */
    bool reset_buf;                          /* 0x120 */
    unsigned int cowork_uid;                 /* 0x124 (gluer uid) */
    int au_count;                            /* 0x128 prop "au-count" */
    int reader_au_count;                     /* 0x12c */
    int max_au_count;                        /* 0x130 (constructor: 1) */
    unsigned int unknown_134;
    pthread_cond_t shm_cond;                 /* 0x138 */
    pthread_mutex_t shm_lock;                /* 0x168 */
};

typedef struct {                             /* GLUE_CMD_SET_SHARED_BUFFER payload */
    unsigned int shared_buf_offset;
    unsigned int shared_buf_size;
    unsigned int cowork_module_uid;
    unsigned int max_access_size;
    unsigned int aui_count;
} GlueCmdSetSharedBuffer;

struct mod_glue {                            /* instance_size 0x158 ("glue", "gluepw", ...) */
    struct aud_module module;
    struct aimc *imc;                        /* 0x108 */
    struct aud_starter *starter;             /* 0x10c reader side only */
    struct _ADEC_BUF_T *shared_buf;          /* 0x110 InterBufReader or InterBufWriter */
    unsigned int cowork_uid;                 /* 0x114 prop "cowork-uid" */
    struct _ADEC_BUF_T *input_buffer;        /* 0x118 legacy I/F */
    struct _ADEC_BUF_T *output_buffer;       /* 0x11c legacy I/F */
    unsigned int shbuf_offset;               /* 0x120 prop "shbuf-offset" */
    void *shmem_mapped;                      /* 0x124 */
    unsigned int shmem_size;                 /* 0x128 */
    unsigned int last_data_size;             /* 0x12c (0x800 initially) */
    bool merge_meta_pcm;                     /* 0x130 prop "merge-meta-pcm" (true) */
    int meta_count;                          /* 0x134 */
    struct aud_data_adapter meta_adap;       /* 0x138 */
    unsigned int aligned_meta_size;          /* 0x154 */
};

#endif /* LIBAA_OBJECTS_H */
