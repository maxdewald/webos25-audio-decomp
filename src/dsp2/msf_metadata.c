/* Scalar MSF metadata ownership, reconstructed from DSP2 main, VA 0x60000000.
 * Evidence: aligned xtobjdump and xtlist literals in build/scratch/msf-metadata/.
 * This is the 0x1c-byte MSF list connector, NOT connector.c's slot-array ring.
 * Pointers are native for host tests; offsets below describe the 32-bit DSP ABI.
 * Diagnostic logging is omitted. No audio samples are stored or moved here.
 * Gaps: serialized process/check and verbose print owners are not reconstructed.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct msf_link { struct msf_link *prev, *next; };
struct msf_metadata {
    int32_t id, type, offset, duration, payload_size, flags;
    void *payload;
};
struct msf_md_item { struct msf_link link; struct msf_metadata md; };
struct msf_md_connector {
    struct msf_link head;             /* +0x00 circular, oldest at head.next */
    int32_t max_md, num_md;           /* +0x08, +0x0c */
    int32_t max_payload;              /* +0x10 */
    int32_t max_samples, samples;     /* +0x14, +0x18 */
};
struct msf_md_command {
    struct msf_link link;
    int32_t opcode;                   /* +0x08: 4 metadata, 10 samples */
    union { struct msf_metadata md; int32_t samples; } value;
};
extern void *aud_meta_data_new(unsigned);
extern void aud_meta_data_free(void *);
extern unsigned irq_raise_level(void);
extern void irq_restore(unsigned);

#if UINTPTR_MAX == UINT32_MAX
_Static_assert(sizeof(struct msf_metadata) == 0x1c, "metadata DSP ABI");
_Static_assert(sizeof(struct msf_md_item) == 0x24, "metadata item DSP ABI");
_Static_assert(sizeof(struct msf_md_connector) == 0x1c, "connector DSP ABI");
_Static_assert(offsetof(struct msf_md_command, value) == 0x0c, "command DSP ABI");
#endif

static struct msf_md_item *md_item(struct msf_link *p)
{
    return (struct msf_md_item *)p;
}
static void md_append(struct msf_link *head, struct msf_link *p)
{
    if (head->prev != p) {
        p->prev = head->prev;
        p->next = head;
        head->prev->next = p;
        head->prev = p;
    }
}
static void md_unlink(struct msf_link *p)
{
    if (p->prev) {
        p->prev->next = p->next;
        p->next->prev = p->prev;
    }
}
static void md_copy(const struct msf_metadata *src, struct msf_metadata *dst)
{
    void *payload = dst->payload;
    *dst = *src;
    dst->payload = payload;
    if (src->payload && src->payload_size > 0)
        memcpy(payload, src->payload, (size_t)src->payload_size);
}
static struct msf_md_item *md_new_item(const struct msf_metadata *md)
{
    size_t size = sizeof(struct msf_md_item) + (size_t)md->payload_size;
    struct msf_md_item *p = aud_meta_data_new((unsigned)size);
    if (p) {
        memset(p, 0, size);
        p->md.payload = p + 1;
        md_copy(md, &p->md);
    }
    return p;
}
/* Firmware compares the existing payload length, not the incoming length. */
static int md_matches(const struct msf_metadata *a, const struct msf_metadata *b)
{
    if (a->type == 1 || a->id != b->id)
        return 0;
    if (a->offset < b->offset) {
        if (a->offset + a->duration < b->offset)
            return 0;
    } else if (b->offset + b->duration < a->offset) {
        return 0;
    }
    return memcmp(a->payload, b->payload, (size_t)a->payload_size) == 0;
}
static void md_merge(struct msf_metadata *a, const struct msf_metadata *b)
{
    int32_t end_a = a->offset + a->duration;
    int32_t end_b = b->offset + b->duration;
    if (b->offset < a->offset)
        a->offset = b->offset;
    a->duration = (end_a > end_b ? end_a : end_b) - a->offset;
}

/* @0x6008d614: coalesce touching/overlapping identical payloads; free absorbed nodes. */
int _msf_combine_identical_metadata(struct msf_md_connector *c)
{
    int merged = 0;
    for (struct msf_link *a = c->head.next; a != &c->head; a = a->next) {
        struct msf_link *b = c->head.next;
        while (b != &c->head) {
            struct msf_link *next = b->next;
            if (a != b && md_matches(&md_item(a)->md, &md_item(b)->md)) {
                md_merge(&md_item(a)->md, &md_item(b)->md);
                md_unlink(b);
                aud_meta_data_free(b);
                if (c->num_md > 0)
                    c->num_md--;
                merged++;
            }
            b = next;
        }
    }
    return merged;
}

/* @0x6008d71c: copies input, but mutates type 0/1 offsets into buffered coordinates.
 * Type 2 replaces the first matching ID in place, retaining its allocation.
 * Caller holds the IRQ lock. The input metadata is never freed here. */
int _msf_cmd_put_metadata(struct msf_md_connector *c, struct msf_metadata *md)
{
    if (!c || c->max_payload < md->payload_size)
        return -1;
    if (md->type == 2) {
        for (struct msf_link *p = c->head.next; p != &c->head; p = p->next)
            if (md_item(p)->md.id == md->id) {
                md_copy(md, &md_item(p)->md);
                return 0;
            }
    }
    if (md->type == 0 || md->type == 1)
        md->offset += c->samples;
    if (c->num_md + 1 >= c->max_md)
        _msf_combine_identical_metadata(c);
    for (struct msf_link *p = c->head.next; p != &c->head; p = p->next)
        if (md_matches(&md_item(p)->md, md)) {
            md_merge(&md_item(p)->md, md);
            return 0;
        }
    if (c->num_md >= c->max_md)
        return -1;
    struct msf_md_item *item = md_new_item(md);
    if (!item)
        return -1;
    md_append(&c->head, &item->link);
    c->num_md++;
    return 0;
}

/* @0x6008d9ac: metadata sample accounting only; overflow leaves count unchanged. */
int msf_update_write_buf(struct msf_md_connector *c, int32_t samples)
{
    if (!c || samples > c->max_samples - c->samples)
        return -1;
    c->samples += samples;
    return 0;
}

/* @0x6008da24: enqueue an owned deep copy of a metadata command; no count update. */
int msf_add_put_metadata(struct msf_link *head, const struct msf_metadata *md)
{
    if (!head || !md)
        return 0x86;
    size_t size = sizeof(struct msf_md_command) + (size_t)md->payload_size;
    struct msf_md_command *cmd = aud_meta_data_new((unsigned)size);
    if (!cmd)
        return -32640;                /* literal 0xffff8080 */
    memset(cmd, 0, size);
    cmd->opcode = 4;
    cmd->value.md.payload = cmd + 1;
    md_copy(md, &cmd->value.md);
    unsigned ps = irq_raise_level();
    md_append(head, &cmd->link);
    irq_restore(ps);
    return 0;
}

/* @0x6008db30: enqueue a sample-count command, not an immediate buffer update. */
int msf_add_update_write_buf(struct msf_link *head, int32_t samples)
{
    if (!head)
        return 0x86;
    size_t size = offsetof(struct msf_md_command, value) + sizeof(int32_t);
    struct msf_md_command *cmd = aud_meta_data_new((unsigned)size);
    if (!cmd)
        return -32640;
    memset(cmd, 0, size);
    cmd->opcode = 10;
    cmd->value.samples = samples;
    unsigned ps = irq_raise_level();
    md_append(head, &cmd->link);
    irq_restore(ps);
    return 0;
}

/* @0x6008dc20: serialize immediate metadata insertion against IRQ consumers. */
int msf_cmd_put_metadata(struct msf_md_connector *c, struct msf_metadata *md)
{
    if (!c)
        return -1;
    unsigned ps = irq_raise_level();
    int rc = _msf_cmd_put_metadata(c, md);
    irq_restore(ps);
    return rc ? -1 : 0;
}

/* @0x6008e0b0: advance all metadata first, even if sample accounting underflows.
 * Offset exactly zero survives; only negative offsets shorten/drop an item. */
int msf_update_read_buf(struct msf_md_connector *c, int32_t samples)
{
    if (!c)
        return -1;
    unsigned ps = irq_raise_level();
    struct msf_link *p = c->head.next;
    while (p != &c->head) {
        struct msf_link *next = p->next;
        struct msf_metadata *md = &md_item(p)->md;
        md->offset -= samples;
        if (md->offset < 0) {
            md->duration += md->offset;
            md->offset = 0;
            if (md->duration <= 0) {
                md_unlink(p);
                aud_meta_data_free(p);
                if (c->num_md > 0)
                    c->num_md--;
            }
        }
        p = next;
    }
    int rc = 0;
    if (samples <= c->samples)
        c->samples -= samples;
    else {
        c->samples = 0;
        rc = -1;
    }
    irq_restore(ps);
    return rc;
}

/* @0x6008e1dc: release every owned node and reset both accounting counters. */
int msf_flush_metadata(struct msf_md_connector *c)
{
    if (!c)
        return -1;
    unsigned ps = irq_raise_level();
    while (c->head.next != &c->head) {
        struct msf_link *p = c->head.next;
        md_unlink(p);
        aud_meta_data_free(p);
    }
    c->samples = c->num_md = 0;
    irq_restore(ps);
    return 0;
}
/* @0x6008e250 */
int msf_get_avail_samples_read(const struct msf_md_connector *c)
{ return c ? c->samples : 0; }
/* @0x6008e280 */
int msf_get_avail_samples_write(const struct msf_md_connector *c)
{ return c ? c->max_samples - c->samples : 0; }
/* @0x6008e2b4 */
int msf_get_max_samples(const struct msf_md_connector *c)
{ return c ? c->max_samples : 0; }
/* @0x6008e2e4 */
int msf_get_curr_num_md(const struct msf_md_connector *c)
{ return c ? c->num_md : 0; }
/* @0x6008e314 */
int msf_get_avail_md_slot(const struct msf_md_connector *c)
{ return c ? c->max_md - c->num_md : 0; }

static struct msf_metadata *md_clone(const struct msf_metadata *md)
{
    size_t size = sizeof(struct msf_metadata) + (size_t)md->payload_size;
    struct msf_metadata *out = aud_meta_data_new((unsigned)size);
    if (out) {
        memset(out, 0, size);
        out->payload = out + 1;
        md_copy(md, out);
    }
    return out;
}
/* A failed output allocation does NOT undo an already inserted split tail. */
static struct msf_metadata *md_take(struct msf_md_connector *c,
                                   struct msf_md_item *item, int32_t limit)
{
    struct msf_metadata *md = &item->md;
    if (md->type == 0 && limit < md->offset + md->duration) {
        if (msf_get_avail_md_slot(c) < 1)
            return NULL;
        struct msf_md_item *tail = md_new_item(md);
        if (!tail)
            return NULL;
        tail->md.offset = limit;
        tail->md.duration = md->offset + md->duration - limit;
        md->duration = limit - md->offset;
        md_append(&c->head, &tail->link);
        c->num_md++;
    }
    struct msf_metadata *out = md_clone(md);
    if (out) {
        md_unlink(&item->link);
        aud_meta_data_free(item);
        c->num_md--;
    }
    return out;
}

/* @0x6008e348: consume first eligible node; split type 0 at exclusive limit. */
struct msf_metadata *msf_get_metadata(struct msf_md_connector *c, int32_t limit)
{
    if (!c)
        return NULL;
    unsigned ps = irq_raise_level();
    struct msf_metadata *out = NULL;
    for (struct msf_link *p = c->head.next; p != &c->head; p = p->next) {
        struct msf_md_item *item = md_item(p);
        if (item->md.type == 2 || item->md.offset < limit) {
            out = md_take(c, item, limit);
            break;
        }
    }
    irq_restore(ps);
    return out;
}

/* @0x6008e4c0: earliest offset for an ID, not first queue entry. */
struct msf_metadata *msf_get_metadata_by_id(struct msf_md_connector *c,
                                          int32_t id, int32_t limit)
{
    if (!c)
        return NULL;
    unsigned ps = irq_raise_level();
    struct msf_md_item *best = NULL;
    int32_t offset = 0x0fffffff;
    for (struct msf_link *p = c->head.next; p != &c->head; p = p->next) {
        struct msf_md_item *item = md_item(p);
        if (item->md.id == id && item->md.offset < offset) {
            best = item;
            offset = item->md.offset;
        }
    }
    struct msf_metadata *out = NULL;
    if (best && (best->md.type == 2 || best->md.offset < limit))
        out = md_take(c, best, limit);
    irq_restore(ps);
    return out;
}

/* @0x6008e684: non-consuming deep copy of first eligible matching ID. */
struct msf_metadata *msf_find_metadata(struct msf_md_connector *c,
                                     int32_t id, int32_t limit)
{
    if (!c)
        return NULL;
    unsigned ps = irq_raise_level();
    struct msf_metadata *out = NULL;
    for (struct msf_link *p = c->head.next; p != &c->head; p = p->next) {
        struct msf_metadata *md = &md_item(p)->md;
        if (md->id == id && (md->type == 2 || md->offset < limit)) {
            out = md_clone(md);
            break;
        }
    }
    irq_restore(ps);
    return out;
}

/* @0x6008e76c: the firmware ignores the third argument; no actual sorting. */
struct msf_metadata *msf_find_metadata_sorted(struct msf_md_connector *c,
                                            int32_t id, int32_t ignored, int32_t limit)
{
    (void)ignored;
    return msf_find_metadata(c, id, limit);
}
