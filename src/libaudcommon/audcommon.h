/*
 * libaudcommon public types: refcounted aud_object, aud_event (+ key/value list),
 * aud_data (PCM/ES/meta buffers with a size-class cache), aud_data_adapter,
 * aud_meta_data, audutil string helpers and logm message hooks.
 *
 * RECONSTRUCTION from orig/usr/lib/libaudcommon.so.1.0.0
 *   sha256 a4df15e082096e6c60c7091b81f90f4fe0a6c5ae07173867ca4a0b05bfd5087f
 *
 * Layout evidence:
 *   aud_object/aud_event/aud_data/aud_data_info/link: DWARF in
 *     orig/usr/lib/gstreamer-1.0/libgstlgaudiosink.so (byte sizes 0x0c/0x24/0x50/0x20/0x08)
 *   aud_event_value (0x20): get_value @0x11f24, aud_event_dup @0x12164
 *   aud_data_adapter (0x1c): aud_data_adapter_init @0x138e4, aud_data_adapter_map @0x13c68
 *   aud_meta_data header (0x18): aud_meta_data_new @0x14624, aud_meta_data_free @0x1453c
 *
 * Self-contained: include this header instead of re-declaring these types.
 */
#ifndef AUDCOMMON_H
#define AUDCOMMON_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------------- */
/* intrusive doubly linked list (always inlined in the binary)               */
/* ------------------------------------------------------------------------- */
#ifndef _AUDIO_LINK_H_
#define _AUDIO_LINK_H_
struct link {
    struct link *prev, *next;
};

#define link_init(l)            ((l)->prev = (l)->next = (l))
#define link_empty(l)           ((l) == (l)->next)
#define link_container(type, member, ptr) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
#define link_first(type, member, root)  link_container(type, member, (root)->next)
#define for_each_link(now, root) \
    for ((now) = (root)->next; (now) != (root); (now) = (now)->next)

/* append item at the tail (before root); no-op if it already is the tail */
static inline void link_add_tail(struct link *root, struct link *item)
{
    if (root->prev == item)
        return;
    item->next = root;
    item->prev = root->prev;
    root->prev->next = item;
    root->prev = item;
}

/* insert item at the head (right after root) */
static inline void link_add(struct link *root, struct link *item)
{
    if (root->prev == item)
        return;
    item->prev = root;
    item->next = root->next;
    root->next->prev = item;
    root->next = item;
}

static inline void link_del(struct link *l)
{
    if (!l->prev)
        return;
    l->prev->next = l->next;
    l->next->prev = l->prev;
}
#endif /* _AUDIO_LINK_H_ */

/* ------------------------------------------------------------------------- */
/* refcounted object header                                                  */
/* ------------------------------------------------------------------------- */
#define AUD_OBJECT_TYPE(a, b, c, d) \
    ((unsigned int)(a) << 24 | (unsigned int)(b) << 16 | (unsigned int)(c) << 8 | (unsigned int)(d))
#define AUD_OBJECT_TYPE_EVENT       AUD_OBJECT_TYPE('e', 'v', 'n', 't')   /* 0x65766e74 */
#define AUD_OBJECT_TYPE_DATA        AUD_OBJECT_TYPE('d', 'a', 't', 'a')   /* 0x64617461 */

struct aud_object {
    unsigned int user;                          /* 0x00 refcount, -1 after a double put */
    unsigned int type;                          /* 0x04 AUD_OBJECT_TYPE_* */
    void (*destroy)(struct aud_object *obj);    /* 0x08 called when user drops to 0 */
};

struct aud_object *aud_object_get(struct aud_object *o);
void aud_object_put(struct aud_object *o);

/* ------------------------------------------------------------------------- */
/* events                                                                    */
/* ------------------------------------------------------------------------- */
enum aud_event_type {
    AUD_EVENT_TYPE_ENABLE_DISABLE,              /* 0 */
    AUD_EVENT_TYPE_FIRST_AU,                    /* 1 "first" */
    AUD_EVENT_TYPE_LAST_AU,                     /* 2 */
    AUD_EVENT_TYPE_START,                       /* 3 */
    AUD_EVENT_TYPE_STOP,                        /* 4 */
    AUD_EVENT_TYPE_MARK,                        /* 5 */
    AUD_EVENT_TYPE_DISCONNECT,                  /* 6 */
    AUD_EVENT_TYPE_FLUSH,                       /* 7 */
    AUD_EVENT_TYPE_RESTART,                     /* 8 */
    AUD_EVENT_TYPE_LATENCY_CHECK,               /* 9 */
    AUD_EVENT_TYPE_DSP_RESET,                   /* 10 */
    AUD_EVENT_TYPE_DEC_ERROR,                   /* 11 */
    AUD_EVENT_TYPE_MPEGH_UI,                    /* 12 */
    AUD_EVENT_TYPE_CUSTOM,                      /* 13, must be last */
};

#define AUD_EVENT_PROP_DOWNWARD     0x1
#define AUD_EVENT_PROP_SYNC         0x2
#define AUD_EVENT_IS_DOWNWARD(e)    ((e)->prop & AUD_EVENT_PROP_DOWNWARD)
#define AUD_EVENT_IS_SYNC(e)        ((e)->prop & AUD_EVENT_PROP_SYNC)

enum aud_event_value_type {
    aud_value_int,                              /* 0 */
    aud_value_bool,                             /* 1 */
    aud_value_str,                              /* 2 */
    aud_value_data,                             /* 3 (raw memory) */
    aud_value_fraction,                         /* 4 */
};

struct aud_event_value {                        /* calloc(0x20) in get_value @0x11f24 */
    struct link value_link;                     /* 0x00 on aud_event::values */
    char *name;                                 /* 0x08 strdup'ed */
    enum aud_event_value_type type;             /* 0x0c */
    union {                                     /* 0x10 */
        int i;
        bool b;
        char *s;
        void *d;
        struct { int num, den; } fr;
    } value;
    void *mem;                                  /* 0x18 owned copy for str/data, else NULL */
    unsigned int mem_size;                      /* 0x1c strlen+1 for str */
};

struct aud_event {                              /* calloc(1, 0x24 + param_size) */
    struct aud_object object;                   /* 0x00 */
    enum aud_event_type type;                   /* 0x0c */
    unsigned int prop;                          /* 0x10 AUD_EVENT_PROP_* */
    struct link values;                         /* 0x14 list of aud_event_value */
    void *caller;                               /* 0x1c never set by libaudcommon */
    unsigned int param_size;                    /* 0x20 */
    unsigned char param[];                      /* 0x24 */
};

#define aud_event_get(e)    ((struct aud_event *)aud_object_get(&(e)->object))
#define aud_event_put(e)    aud_object_put(&(e)->object)

struct aud_event *aud_event_new(void *param, unsigned int param_size);
struct aud_event *aud_event_dup(struct aud_event *event);
struct aud_event *aud_event_dup_and_set(struct aud_event *event, const char *name, ...);
struct aud_event *aud_event_new_first(void);
struct aud_event *aud_event_newv_first(const char *name, ...);
struct aud_event *aud_event_new_flush(void);
struct aud_event *aud_event_new_flush_async(void);
struct aud_event *aud_event_new_last(void);
struct aud_event *aud_event_new_start_up(void);
struct aud_event *aud_event_new_start(void);
struct aud_event *aud_event_new_stop(void);
struct aud_event *aud_event_new_mark(void *data, unsigned int size);
struct aud_event *aud_event_new_disconnect(void);
struct aud_event *aud_event_new_disconnect_sync(void);
struct aud_event *aud_event_new_restart(void);
struct aud_event *aud_event_newv_restart(const char *name, ...);
struct aud_event *aud_event_new_latency_check(void);
struct aud_event *aud_event_new_custom(void);
struct aud_event *aud_event_newv_custom(const char *name, ...);

/* varargs: name, (int)type, value[, size for aud_value_data], ..., NULL.
 * aud_value_fraction is NOT accepted here (logs "unknown type"). */
struct aud_event *_aud_event_set_values(struct aud_event *e, const char *name, va_list ap);
struct aud_event *aud_event_set_values(struct aud_event *e, const char *name, ...);

int aud_event_value_set_int(struct aud_event *e, const char *name, int v);
int aud_event_value_set_bool(struct aud_event *e, const char *name, bool v);
int aud_event_value_set_str(struct aud_event *e, const char *name, const char *s);
int aud_event_value_set_mem(struct aud_event *e, const char *name, void *data, unsigned int size);
int aud_event_value_set_fraction(struct aud_event *e, const char *name, int num, int den);

int aud_event_value_get_int(struct aud_event *e, const char *name, int def);
bool aud_event_value_get_bool(struct aud_event *e, const char *name, bool def);
const char *aud_event_value_get_str(struct aud_event *e, const char *name, const char *def);
void *aud_event_value_get_mem(struct aud_event *e, const char *name, unsigned int *size);
int aud_event_value_get_fraction(struct aud_event *e, const char *name, int *num, int *den);
void aud_event_value_del(struct aud_event *e, const char *name);
bool aud_event_has_name(struct aud_event *e, const char *name);

const char *aud_event_name(struct aud_event *e);
char *aud_event_str(struct aud_event *e);       /* malloc'ed, caller frees */
char *aud_event_str2(struct aud_event *e, int name_width, const char *sep);

/* wire format: see aud_event_serialize @0x12dac in event.c */
void *aud_event_serialize(struct aud_event *e, unsigned int *mem_size);
struct aud_event *aud_event_deserialize(void *data, unsigned int size);

/* ------------------------------------------------------------------------- */
/* data buffers                                                              */
/* ------------------------------------------------------------------------- */
enum aud_data_type {
    type_pcm,                                   /* 0 default (memset by aud_data_new) */
    type_es,                                    /* 1 compressed/bitstream (sink lgao_feed) */
    type_meta,                                  /* 2 */
    type_meta_ext,                              /* 3 */
    type_mixed,                                 /* 4 repeated {u32 type, u32 size, data[size]} */
};

/* aud_data_info::flag bits. Only bit 0 is set inside libaudcommon
 * (aud_data_new_zero @0x13630); the name is ours. */
#define AUD_DATA_FLAG_ZERO          0x1

struct aud_data_cache;

struct aud_data {                               /* 0x50 header + payload */
    struct aud_object object;                   /* 0x00 */
    struct aud_data_info {                      /* 0x10 (8-aligned: u64 inside) */
        enum aud_data_type type;                /* 0x10 */
        unsigned int index;                     /* 0x14 -1 when new */
        unsigned int timestamp;                 /* 0x18 -1 when new */
        unsigned long long timestamp64;         /* 0x20 0 when new */
        unsigned int flag;                      /* 0x28 AUD_DATA_FLAG_* */
    } info;
    void *caller;                               /* 0x30 */
    unsigned int size;                          /* 0x34 */
    unsigned char *data;                        /* 0x38 */
    struct link cache_link;                     /* 0x3c */
    struct aud_data_cache *cache;               /* 0x44 NULL = plain malloc */
    int data_allocated;                         /* 0x48 free(data) on destroy (uncached only) */
    struct aud_data *parent;                    /* 0x4c region/split parent, ref held */
    unsigned char _data[];                      /* 0x50 */
};

#define AUD_DATA_TYPE(d)    ((d)->info.type)
#define AUD_DATA_INDEX(d)   ((d)->info.index)
#define AUD_DATA_TS(d)      ((d)->info.timestamp)
#define AUD_DATA_TS64(d)    ((d)->info.timestamp64)

#define aud_data_get(d)     ((struct aud_data *)aud_object_get(&(d)->object))
#define aud_data_put(d)     aud_object_put(&(d)->object)

int aud_data_gc(int min_cache_cnt);
struct aud_data *aud_data_new(int size);
struct aud_data *aud_data_new_zero(int size);
struct aud_data *aud_data_region(struct aud_data *data, unsigned int offset, int size);
struct aud_data *aud_data_split(struct aud_data *data, unsigned int offset);
int aud_data_split2(struct aud_data *data, unsigned int offset,
                    struct aud_data **d1, struct aud_data **d2);

/* ------------------------------------------------------------------------- */
/* data adapter: FIFO of aud_data, byte-addressable                          */
/* ------------------------------------------------------------------------- */
struct aud_data_holder {                        /* calloc(0xc) in new_holder @0x11db4 */
    struct link link;
    struct aud_data *data;
};

struct aud_data_adapter {                       /* 0x1c */
    struct link holder;                         /* 0x00 queued aud_data_holder */
    struct link holder_free;                    /* 0x08 recycled holders */
    unsigned int total_size;                    /* 0x10 sum of queued data->size */
    unsigned int offset;                        /* 0x14 bytes already consumed from head */
    void *alloced_map;                          /* 0x18 bounce buffer of _map() */
};

#define aud_data_adapter_offset(a)      ((a)->offset)
#define aud_data_adapter_size(a)        ((a)->total_size - (a)->offset)
#define aud_data_adapter_flush_all(a)   aud_data_adapter_flush((a), aud_data_adapter_size(a))

void aud_data_adapter_init(struct aud_data_adapter *adap);
void aud_data_adapter_cleanup(struct aud_data_adapter *adap);
void aud_data_adapter_push(struct aud_data_adapter *adap, struct aud_data *data);
void aud_data_adapter_push_front(struct aud_data_adapter *adap, struct aud_data *data);
int aud_data_adapter_flush(struct aud_data_adapter *adap, unsigned int size);
int aud_data_adapter_copy(struct aud_data_adapter *adap, void *data, unsigned int size);
void *aud_data_adapter_map(struct aud_data_adapter *adap, unsigned int size);
void aud_data_adapter_unmap(struct aud_data_adapter *adap, void *ptr);
struct aud_data *aud_data_adapter_get_data(struct aud_data_adapter *adap, unsigned int size);
struct aud_data *aud_data_adapter_peep(struct aud_data_adapter *adap);
struct aud_data *aud_data_adapter_dequeue(struct aud_data_adapter *adap);

/* ------------------------------------------------------------------------- */
/* meta data: size-class cached malloc with a 0x18-byte hidden header        */
/* ------------------------------------------------------------------------- */
#define AUD_META_DATA_MAGIC     0xadccadccu

struct aud_meta_data_cache;

struct aud_meta_data {
    unsigned int magic;                         /* 0x00 AUD_META_DATA_MAGIC */
    struct aud_meta_data_cache *cache;          /* 0x04 */
    struct link cache_link;                     /* 0x08 */
    void *return_addr;                          /* 0x10 zeroed, never filled in this build */
    void *return_addr2;                         /* 0x14 */
    unsigned char data[] __attribute__((aligned(8)));   /* 0x18 user pointer */
};

void *aud_meta_data_new(int size);
void aud_meta_data_free(void *buf);
int aud_meta_data_gc(void);

/* ------------------------------------------------------------------------- */
/* utilities / logging front-ends                                            */
/* ------------------------------------------------------------------------- */
char *audutil_strdup_vprintf(const char *fmt, va_list ap);
char *audutil_strdup_printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
char *audutil_append_vprintf(char *str, unsigned int *off, const char *fmt, va_list ap);
char *audutil_append_printf(char *str, unsigned int *off, const char *fmt, ...)
    __attribute__((format(printf, 3, 4)));

struct audutil_opt {
    enum { opt_int, opt_bool } type;
    const char *name;
    void *ptr;
};
const struct audutil_opt *audutil_set_opts(const struct audutil_opt *opts, char **argv);
void audutil_print_opt(const struct audutil_opt *opt);

typedef int (*logm_message_hook_func_t)(void *arg, char *str);
int logm_message(const char *format, ...) __attribute__((format(printf, 1, 2)));
int logm_message_default(const char *format, va_list ap);
int logm_message_defaultf(const char *format, ...) __attribute__((format(printf, 1, 2)));
logm_message_hook_func_t logm_message_hook(logm_message_hook_func_t func, void *arg);
int logmm_printlm(const char *name, int level, const char *func, int line,
                  const char *format, ...) __attribute__((format(printf, 5, 6)));

#endif /* AUDCOMMON_H */
