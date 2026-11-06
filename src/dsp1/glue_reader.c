// SPDX-License-Identifier: GPL-2.0-only
/*
 * DSP1 shared-memory receiver, source-assisted from LG's GPL-2.0 alternative
 * common/module/{starter,module_glue}.c (Copyright 2017 SIC Lab., LG Electronics).
 * Firmware: audio_a0_dsp1.bin SHA256
 * 8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4.
 * Saved main image base 0x60000000. Branch/literal evidence:
 * build/scratch/glue-reader/{process,control,flush}.asm and {create,new}.xt.
 * Ghidra's split process_buf blocks are ONE function, not separate functions.
 * Diagnostics are omitted. Structures are host-portable semantic views, NOT
 * ABI declarations; comments record shipped 32-bit offsets/vtable positions.
 * The real InterBufReader/CommonBuffer library remains external. In particular
 * InterBufReader_FlushData @600edfd4 calls CommonBuffer_FlushData under IRQ
 * protection, restores IRQ state, then ashbuf_notify_read EVEN on flush error.
 * No hardware/cache/ring implementation is substituted here.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

struct aud_mutex { uint32_t opaque[7]; };
struct aud_cond { uint32_t opaque[4]; };
struct aud_thread;
struct aud_event;
struct aud_starter;
struct mod_glue;
struct link { struct link *prev, *next; };
struct aud_data {
    uint32_t object[4];
    int type;                       /* +0x10: PCM=0, ES=1, META=2, EXT=3, MIXED=4 */
    int index;                      /* +0x14 */
    uint32_t timestamp;              /* +0x18 */
    uint32_t reserved;
    uint64_t timestamp64;            /* +0x20 */
    uint32_t reserved2[3];
    unsigned size;                  /* +0x34 */
    unsigned char *data;            /* +0x38 */
};
struct au_info {
    unsigned size;
    uint32_t timestamp;
    uint64_t timestamp64;
    uint32_t gstc;
    int16_t index;                  /* +0x14: signed, not the whole flags word */
    uint8_t flags, acmod;
    uint32_t first_module_gstc;
    int data_type;                  /* +0x1c: EVENT=4, MIXED=5 */
};
struct buffer_status {
    int32_t index_allocated;
    uint32_t read_offset, write_offset;
    int16_t read_index, write_index;
    int32_t written, passed, repeat_count;
};
struct adec_buf {
    unsigned offset;                /* original +0x38 */
    int (*get_au_cnt)(struct adec_buf *);                         /* +0x90 */
    int (*get_current_info)(int *, struct au_info *, struct adec_buf *); /* +0xbc */
    int (*read_data)(unsigned char **, int *, int *, struct adec_buf *); /* +0xc0 */
    int (*flush_data)(int, struct adec_buf *);                    /* +0xc4 */
    void (*unref)(struct adec_buf *);                            /* +0xdc */
};
struct aud_port {
    int number;
    struct mod_glue *module;         /* +0x04 */
    struct link starter_link;        /* +0x130 */
    struct aud_starter *starter;     /* +0x138 */
};
struct aud_starter {
    struct link ports;              /* +0x00 */
    struct aud_mutex ports_lock;    /* +0x08 */
    struct aud_thread *thread;      /* +0x24 */
    struct adec_buf *lbuf;          /* +0x28 */
    char *name;                     /* +0x2c */
    int stop;                       /* +0x30 */
    struct aud_mutex thread_mutex;  /* +0x34 */
    struct aud_cond thread_cond;    /* +0x50 */
    bool split_meta_pcm;            /* +0x60 */
};
struct mod_glue {
    uint32_t uid;                   /* module +0x04 */
    void *imc;                      /* +0xf0 */
    struct aud_starter *starter;    /* +0xf4 */
    struct adec_buf *shared_buf;    /* +0xf8 */
    unsigned cowork_uid;            /* +0xfc */
    unsigned shbuf_offset;          /* +0x108 */
    void *meta_adapter;             /* semantic adapter, original embedded +0x120 */
};
struct shared_buffer_cmd {
    uint32_t offset, size, cowork_uid, max_access_size, aui_count;
};
struct remote_status_cmd {
    uint32_t evt_type, reserved[2];
    struct buffer_status status;    /* +0x0c */
};
struct processor_adapter {
    void (*invalidate)(void *, unsigned, struct adec_buf *);
    void (*writeback)(void *, unsigned, struct adec_buf *);
    unsigned char *(*translate)(unsigned char *, struct adec_buf *);
};
extern void Cache_Invalidate(void *, unsigned, struct adec_buf *);
extern void Cache_Writeback(void *, unsigned, struct adec_buf *);
extern unsigned char *Mem_Translator(unsigned char *, struct adec_buf *);
extern unsigned CommonBuffer_GetStructSize(unsigned);
extern unsigned InterBufReader_GetStructSize(void), InterBufWriter_GetStructSize(void);
extern int InterBufReader_Create(const struct processor_adapter *, void *, unsigned,
    unsigned, unsigned, void *, unsigned, void *, struct adec_buf *);
extern int InterBufWriter_Create(const struct processor_adapter *, void *, unsigned,
    unsigned, unsigned, void *, unsigned, void *, struct adec_buf *);
extern void InterBufReader_UpdateStatus(struct adec_buf *, const struct buffer_status *);
extern void InterBufWriter_UpdateStatus(struct adec_buf *, const struct buffer_status *);
extern int aud_module_check_legacy_output(struct mod_glue *, int);
extern struct aud_data *aud_data_new(unsigned);
extern struct aud_data *aud_data_region(struct aud_data *, unsigned, unsigned);
extern struct aud_event *aud_event_deserialize(const void *, unsigned);
extern void *aud_object_get(void *);
extern void aud_object_put(void *);
extern int aud_module_event(struct mod_glue *, struct aud_port *, struct aud_event *);
extern int aud_port_push_data(struct aud_port *, struct aud_data *);
extern int aud_port_sync(struct aud_port *);
extern void aud_mutex_init(struct aud_mutex *), aud_mutex_lock(struct aud_mutex *);
extern void aud_mutex_unlock(struct aud_mutex *);
extern void aud_cond_init(struct aud_cond *), aud_cond_signal(struct aud_cond *);
extern void aud_cond_wait(struct aud_cond *, struct aud_mutex *);
extern struct aud_thread *aud_thread_create(const char *, unsigned, int,
    int (*)(void *), void *);
extern void aud_thread_destroy(struct aud_thread *);
extern char *audutil_strdup_vprintf(const char *, va_list);
extern struct aud_port *aud_module_get_output_port(struct mod_glue *, int);
extern struct aud_port *aud_module_new_output_port(struct mod_glue *, int);
extern void aud_module_del_output_port(struct mod_glue *, int);
extern void aud_data_adapter_cleanup(void *);

static struct aud_port *linked_port(struct link *l)
{
    return (struct aud_port *)((unsigned char *)l - offsetof(struct aud_port, starter_link));
}

/* Inlined process_event, 60100997..60100a4a, 60100c0e..60100cc7.
 * Contiguous serialized FIRST: deserialize BEFORE flushing borrowed ring bytes.
 * Wrapped event: copy/flush fragments, deserialize owned bytes, then free them.
 * Delivery is synchronous module_event(NULL port), in ring order before the
 * next AU. Each destination receives its own reference; the local one is put.
 * The shipped fragmented-event malloc is unchecked and a later read failure
 * leaks that allocation. Neither weakness is silently repaired here. */
static int process_event(struct aud_starter *s, unsigned size)
{
    unsigned char *data = NULL, *allocated = NULL;
    unsigned readed = 0;
    do {
        int cur_size = (int)(size - readed);
        if (s->lbuf->read_data(&data, &cur_size, NULL, s->lbuf) != 0)
            return -1;
        if ((unsigned)cur_size == size)
            break;
        if (!allocated)
            allocated = malloc(size);
        memcpy(allocated + readed, data, (size_t)cur_size);
        (void)s->lbuf->flush_data(cur_size, s->lbuf);
        readed += (unsigned)cur_size;
        data = allocated;
    } while (readed < size);

    struct aud_event *e = aud_event_deserialize(data, size);
    if (allocated)
        free(allocated);
    else
        (void)s->lbuf->flush_data((int)size, s->lbuf);
    if (!e)
        return -1;
    aud_mutex_lock(&s->ports_lock);
    for (struct link *l = s->ports.next; l != &s->ports; l = l->next)
        (void)aud_module_event(linked_port(l)->module, NULL, aud_object_get(e));
    aud_mutex_unlock(&s->ports_lock);
    aud_object_put(e);
    return 0;
}

/* Inlined split_push_data, 60100d66..60100e30. Subregions retain the copied
 * parent aud_data, NOT the shared ring. Malformed tail still reaches sync.
 * As shipped, a failed aud_data_region allocation is not checked. */
static int split_push_data(struct aud_starter *s, struct aud_port *p, struct aud_data *d)
{
    unsigned offs = 0;
    (void)s;
    while (offs + 8u <= d->size) {
        uint32_t type, size;
        memcpy(&type, d->data + offs, 4);
        memcpy(&size, d->data + offs + 4, 4);
        offs += 8;
        if (offs + size > d->size)
            return -1;
        struct aud_data *sub = aud_data_region(d, offs, size);
        sub->type = (int)type;
        offs += (sub->size + 7u) & ~7u;
        (void)aud_port_push_data(p, sub);
    }
    return 0;
}

/* Inlined process_data, 60100a58..60100dee. AU 4 is EVENT, not MIXED.
 * AU 0/1/2/3/5 -> aud_data 0/1/2/3/4; unknown -> PCM. No ES/channel/bits gate.
 * All bytes are copied and flush_data/notify_read occurs BEFORE ports_lock,
 * push, or potentially blocking downstream sync. Each port gets a reference;
 * after EVERY port's sync the starter drops its original reference.
 * Allocation/read failure returns without pushing; already flushed fragments
 * are not rolled back. Flush errors are logged by firmware but otherwise ignored. */
static int process_data(struct aud_starter *s, const struct au_info *aui)
{
    struct aud_data *d = aud_data_new(aui->size);
    if (!d)
        return -1;
    switch (aui->data_type) {
    case 1: d->type = 1; break;
    case 2: d->type = 2; break;
    case 3: d->type = 3; break;
    case 5: d->type = 4; break;
    default: d->type = 0; break;
    }
    d->index = aui->index;
    d->timestamp = aui->timestamp;
    d->timestamp64 = aui->timestamp64;
    unsigned readed = 0;
    while (readed < aui->size) {
        unsigned char *data = NULL;
        int cur_size = (int)(aui->size - readed);
        if (s->lbuf->read_data(&data, &cur_size, NULL, s->lbuf) != 0) {
            aud_object_put(d);
            return -1;
        }
        memcpy(d->data + readed, data, (size_t)cur_size);
        (void)s->lbuf->flush_data(cur_size, s->lbuf);
        readed += (unsigned)cur_size;
    }
    aud_mutex_lock(&s->ports_lock);
    for (struct link *l = s->ports.next; l != &s->ports; l = l->next) {
        struct aud_port *p = linked_port(l);
        if (d->type == 4 && s->split_meta_pcm)
            (void)split_push_data(s, p, d);
        else
            (void)aud_port_push_data(p, aud_object_get(d));
        (void)aud_port_sync(p);
    }
    aud_mutex_unlock(&s->ports_lock);
    aud_object_put(d);
    return 0;
}

/* @0x60100940: merged original including all split decompiler blocks.
 * Only get_current_info failure returns -1; processing failures are swallowed.
 * A nonzero AU offset is diagnostic only. Empty AU flushes zero to retire it.
 * No whole-ring flush, timeout, retry bound, or flush/stop check inside a read. */
int process_buf(struct aud_starter *s)
{
    struct au_info aui;
    int offset;
    aui.size = 0;
    if (s->lbuf->get_current_info(&offset, &aui, s->lbuf) != 0)
        return -1;
    if (aui.size > 0) {
        if (aui.data_type == 4)
            (void)process_event(s, aui.size);
        else
            (void)process_data(s, &aui);
    } else {
        (void)s->lbuf->flush_data(0, s->lbuf);
    }
    return 0;
}

/* @0x60100e58: waits for AU count, not used bytes; stop wakes this wait but
 * cannot interrupt downstream synchronous processing or a failing read loop. */
int starter_thread(void *param)
{
    struct aud_starter *s = param;
    for (;;) {
        aud_mutex_lock(&s->thread_mutex);
        while (!s->stop && s->lbuf->get_au_cnt(s->lbuf) <= 0)
            aud_cond_wait(&s->thread_cond, &s->thread_mutex);
        aud_mutex_unlock(&s->thread_mutex);
        if (s->stop)
            break;
        (void)process_buf(s);
    }
    return 0;
}

/* @0x60100ec8: firmware stack=0x20000, priority=10; notably thread-create
 * failure does NOT make aud_starter_new fail. The caller can get an inert starter. */
struct aud_starter *aud_starter_new(struct adec_buf *buf, const char *fmt, ...)
{
    struct aud_starter *s = calloc(1, sizeof(*s));
    if (!s)
        return NULL;
    s->ports.next = s->ports.prev = &s->ports;
    s->split_meta_pcm = true;
    s->lbuf = buf;
    va_list ap;
    va_start(ap, fmt);
    s->name = audutil_strdup_vprintf(fmt, ap);
    va_end(ap);
    if (!s->name) {
        free(s);
        return NULL;
    }
    aud_cond_init(&s->thread_cond);
    aud_mutex_init(&s->thread_mutex);
    aud_mutex_init(&s->ports_lock);
    s->thread = aud_thread_create(s->name, 0x20000, 10, starter_thread, s);
    return s;
}

/* @0x60100fb0 */
void aud_starter_trigger(struct aud_starter *s)
{
    aud_mutex_lock(&s->thread_mutex);
    aud_cond_signal(&s->thread_cond);
    aud_mutex_unlock(&s->thread_mutex);
}

/* @0x60100ff0 */
void aud_starter_stop(struct aud_starter *s)
{
    aud_mutex_lock(&s->thread_mutex);
    s->stop = 1;
    aud_cond_signal(&s->thread_cond);
    aud_mutex_unlock(&s->thread_mutex);
}

/* Matches shipped link_add: prepend; suppress only when root->prev == node,
 * not a general duplicate-membership test. Iteration visits newest port first. */
static void add_link(struct link *root, struct link *node)
{
    if (root->prev != node) {
        node->prev = root;
        node->next = root->next;
        root->next->prev = node;
        root->next = node;
    }
}

/* @0x60101040 */
void aud_starter_add(struct aud_starter *s, struct link *node)
{
    aud_mutex_lock(&s->ports_lock);
    add_link(&s->ports, node);
    aud_mutex_unlock(&s->ports_lock);
}

/* @0x60101068 */
void aud_starter_add_port(struct aud_starter *s, struct aud_port *p)
{
    aud_mutex_lock(&s->ports_lock);
    add_link(&s->ports, &p->starter_link);
    p->starter = s;
    aud_mutex_unlock(&s->ports_lock);
}

/* @0x601010c4 */
void aud_starter_del_port(struct aud_starter *s, struct aud_port *p)
{
    aud_mutex_lock(&s->ports_lock);
    if (p->starter_link.prev) {
        p->starter_link.prev->next = p->starter_link.next;
        p->starter_link.next->prev = p->starter_link.prev;
    }
    p->starter = NULL;
    aud_mutex_unlock(&s->ports_lock);
}

/* @0x60101118 */
struct adec_buf *aud_starter_get_lbuf(struct aud_starter *s) { return s->lbuf; }

/* @0x60101120: caller stops first. Clear port backpointers before destroying
 * the thread; this function neither unrefs lbuf nor removes output ports. */
void aud_starter_del(struct aud_starter *s)
{
    aud_mutex_lock(&s->ports_lock);
    for (struct link *l = s->ports.next; l != &s->ports; l = l->next)
        linked_port(l)->starter = NULL;
    aud_mutex_unlock(&s->ports_lock);
    aud_thread_destroy(s->thread);
    free(s->name);
    free(s);
}

/* @0x600ff250: DSP address translation is 32-bit offset+0x58000000. These
 * addresses are passed to the real library, never dereferenced in this model.
 * Firmware ignores Create's result. Writer arm retained to cover whole function. */
struct adec_buf *CreateBuffer(struct mod_glue *g, const struct shared_buffer_cmd *c, int reader)
{
    uint32_t common_addr = c->offset + UINT32_C(0x58000000);
    if (!common_addr)
        return NULL;
    unsigned header = CommonBuffer_GetStructSize(c->aui_count);
    unsigned bytes = reader ? InterBufReader_GetStructSize() : InterBufWriter_GetStructSize();
    struct adec_buf *b = malloc(bytes);
    if (!b)
        return NULL;
    memset(b, 0, bytes);
    static const struct processor_adapter adapter = {
        Cache_Invalidate, Cache_Writeback, Mem_Translator
    };
    void *common = (void *)(uintptr_t)common_addr;
    void *data = (void *)(uintptr_t)(uint32_t)(common_addr + header);
    if (reader)
        (void)InterBufReader_Create(&adapter, common, c->size, c->max_access_size,
            c->aui_count, data, c->cowork_uid, g->imc, b);
    else
        (void)InterBufWriter_Create(&adapter, common, c->size, c->max_access_size,
            c->aui_count, data, c->cowork_uid, g->imc, b);
    b->offset = c->offset;
    g->shbuf_offset = c->offset;
    g->cowork_uid = c->cowork_uid;
    return b;
}

/* @0x600ff84c: original truncated symbol set_shbuf_p is set_shbuf_pr.
 * Starter-name/allocation failure unrefs shared buffer and clears it.
 * Missing output port after failed new_output_port is nevertheless SUCCESS. */
int set_shbuf_pr(struct mod_glue *g, const struct shared_buffer_cmd *cmd)
{
    g->shared_buf = CreateBuffer(g, cmd, 1);
    if (!g->shared_buf)
        return -1;
    g->starter = aud_starter_new(g->shared_buf, "starter_%08x", g->uid);
    if (!g->starter) {
        g->shared_buf->unref(g->shared_buf);
        g->shared_buf = NULL;
        return -1;
    }
    struct aud_port *p = aud_module_get_output_port(g, 0);
    if (!p)
        p = aud_module_new_output_port(g, 0);
    if (p)
        aud_starter_add_port(g->starter, p);
    return 0;
}

/* Inlined SetSharedBufferStatus in Glue_Cmd @600ffa25..600ffaaa,
 * 600ffb58..600ffb75. No separate function tag for this inline body.
 * BM_EVT_WRITE updates reader status BEFORE signaling the starter. BM_EVT_READ
 * updates writer status before check_legacy_output wakes its free-space wait.
 * START/STOP/FLUSH commands in Glue_Cmd are no-ops, not ring flushes/stops. */
int SetSharedBufferStatus(struct mod_glue *g, const struct remote_status_cmd *cmd)
{
    switch (cmd->evt_type) {
    case 2:
        InterBufWriter_UpdateStatus(g->shared_buf, &cmd->status);
        (void)aud_module_check_legacy_output(g, 0);
        break;
    case 3:
        InterBufReader_UpdateStatus(g->shared_buf, &cmd->status);
        if (g->starter)
            aud_starter_trigger(g->starter);
        break;
    default: break;
    }
    return 0;
}

/* @0x600ffed8: deleting last peer stops the reader, but does not destroy it. */
int Glue_DeletingPort(struct mod_glue *g, struct aud_port *p)
{
    (void)p;
    if (g->starter)
        aud_starter_stop(g->starter);
    aud_data_adapter_cleanup(g->meta_adapter);
    return 0;
}

/* @0x600ff558: stop, destroy thread, unref buffer, delete out0, cleanup adapter.
 * Pointers are not cleared by the original destructor. */
void Glue_Destructor(struct mod_glue *g)
{
    if (g->starter) {
        aud_starter_stop(g->starter);
        aud_starter_del(g->starter);
    }
    if (g->shared_buf)
        g->shared_buf->unref(g->shared_buf);
    if (aud_module_get_output_port(g, 0))
        aud_module_del_output_port(g, 0);
    aud_data_adapter_cleanup(g->meta_adapter);
}
