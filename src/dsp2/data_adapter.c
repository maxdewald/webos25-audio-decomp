/* DSP2 byte FIFO, 0x6006b638..0x6006bfd8 (end exclusive).
 * Evidence: saved DSP2 main image, aligned Xtensa scalar/branch-slot listing,
 * and build/decomp/audio_a0_dsp2.main_60000000.bin.c. Host semantic types,
 * NOT a binary ABI: offsets below describe the 32-bit firmware structures.
 * Logging/backtrace guards are omitted. +0x30 allocation call-site provenance
 * cannot be represented portably (Xtensa windowed return address); omitted.
 * No locking is performed. Callers serialize access and manage mapped lifetimes.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct aud_data {
    uint32_t metadata[8];        /* +0x10..+0x2c: copied by region/split callees */
    uint32_t provenance;         /* +0x30 */
    uint32_t size;               /* +0x34 */
    unsigned char *ptr;          /* +0x38 */
};
struct adapter_link {
    struct adapter_link *prev, *next; /* +0, +4 */
};
struct adapter_holder {
    struct adapter_link link;
    struct aud_data *data;       /* +8; holder allocation is 12 firmware bytes */
};
struct aud_data_adapter {
    struct adapter_link queued; /* +0 */
    struct adapter_link idle;   /* +8 */
    uint32_t total;              /* +0x10: sum of queued object sizes */
    uint32_t offset;             /* +0x14: consumed prefix of first object */
    void *mapped;               /* +0x18: allocated map only, not direct maps */
};
extern void *aud_object_get(void *);
extern void aud_object_put(void *);
extern struct aud_data *aud_data_new(uint32_t);
extern struct aud_data *aud_data_region(struct aud_data *, uint32_t, uint32_t);
extern int aud_data_split2(struct aud_data *, uint32_t,
                           struct aud_data **, struct aud_data **);

static void adapter_unlink(struct adapter_link *n)
{
    n->prev->next = n->next;
    n->next->prev = n->prev;
}
static void adapter_before(struct adapter_link *at, struct adapter_link *n)
{
    n->prev = at->prev;
    n->next = at;
    at->prev->next = n;
    at->prev = n;
}
static struct adapter_holder *adapter_first(struct aud_data_adapter *q)
{
    return q->queued.next == &q->queued ? NULL :
        (struct adapter_holder *)q->queued.next;
}
static void adapter_recycle(struct aud_data_adapter *q, struct adapter_holder *h)
{
    adapter_unlink(&h->link);
    adapter_before(&q->idle, &h->link);
}

/* @0x6006b638 */
void aud_data_adapter_init(struct aud_data_adapter *q)
{
    q->queued.prev = q->queued.next = &q->queued;
    q->idle.prev = q->idle.next = &q->idle;
    q->total = q->offset = 0;
    q->mapped = NULL;
}

/* @0x6006b6bc: new_holder (export's truncated name: ew_holde).
 * Reuse the oldest idle holder; stale data pointers there own no references. */
static struct adapter_holder *new_holder(struct aud_data_adapter *q)
{
    struct adapter_holder *h;
    if (q->idle.next == &q->idle)
        return calloc(1, sizeof(*h));
    h = (struct adapter_holder *)q->idle.next;
    adapter_unlink(&h->link);
    return h;
}

/* @0x6006b72c: transfers the existing reference, without aud_object_get.
 * NULL and holder allocation failure are no-ops; failure leaves caller ownership.
 * Firmware has no defined status return, despite some reconstructed callers. */
void aud_data_adapter_push(struct aud_data_adapter *q, struct aud_data *d)
{
    struct adapter_holder *h;
    if (!d || !(h = new_holder(q)))
        return;
    h->data = d;
    adapter_before(&q->queued, &h->link);
    q->total += d->size;
}

/* @0x6006b7a0: same ownership as push. Existing offset is NOT normalized;
 * after prepend it applies to the new first object, exactly as in firmware. */
void aud_data_adapter_push_front(struct aud_data_adapter *q, struct aud_data *d)
{
    struct adapter_holder *h;
    if (!d || !(h = new_holder(q)))
        return;
    h->data = d;
    adapter_before(q->queued.next, &h->link);
    q->total += d->size;
}

/* @0x6006b814: zero is a true no-op, including queued zero-sized objects.
 * Over-flush clamps after unsigned offset+size, as the target does. */
int aud_data_adapter_flush(struct aud_data_adapter *q, uint32_t size)
{
    struct adapter_holder *h;
    uint32_t end;
    if (!size)
        return 0;
    end = q->offset + size;
    q->offset = end > q->total ? q->total : end;
    while ((h = adapter_first(q)) && q->offset >= h->data->size) {
        uint32_t n = h->data->size;
        q->offset -= n;
        q->total -= n;
        aud_object_put(h->data);
        adapter_recycle(q, h);
    }
    if (q->total == q->offset && !adapter_first(q))
        q->total = q->offset = 0;
    return 0;
}

/* @0x6006b650: requires outstanding allocated maps to be unmapped first.
 * Firmware does not free +0x18 here. Zero-sized entries survive flush(0), too. */
void aud_data_adapter_cleanup(struct aud_data_adapter *q)
{
    aud_data_adapter_flush(q, q->total - q->offset);
    while (q->idle.next != &q->idle) {
        struct adapter_link *n = q->idle.next;
        adapter_unlink(n);
        free(n);
    }
}

/* @0x6006b944: non-consuming copy; insufficient input returns -1 AFTER copying
 * its available prefix. Imported +0x6005c584 is the memcpy implementation. */
int aud_data_adapter_copy(struct aud_data_adapter *q, void *dst, uint32_t size)
{
    struct adapter_link *p;
    uint32_t offset = q->offset;
    unsigned char *out = dst;
    for (p = q->queued.next; p != &q->queued; p = p->next) {
        struct aud_data *d = ((struct adapter_holder *)p)->data;
        uint32_t available = d->size - offset;
        uint32_t n = size < available ? size : available;
        if (n)
            memcpy(out, d->ptr + offset, n);
        size -= n;
        if (!size)
            break;
        offset = 0;
        out += n;
    }
    return size ? -1 : 0;
}

/* @0x6006ba0c: direct alias for a contiguous first-object range, otherwise a
 * private copy. A subsequent nonzero map on a nonempty queue invalidates the
 * previous allocated map even if the new allocation/copy subsequently fails. */
void *aud_data_adapter_map(struct aud_data_adapter *q, uint32_t size)
{
    struct adapter_holder *h;
    if (!size || !(h = adapter_first(q)))
        return NULL;
    if (q->mapped) {
        free(q->mapped);
        q->mapped = NULL;
    }
    if (q->offset + size <= h->data->size)
        return h->data->ptr + q->offset;
    q->mapped = malloc(size);
    if (q->mapped && aud_data_adapter_copy(q, q->mapped, size) < 0) {
        free(q->mapped);
        q->mapped = NULL;
    }
    return q->mapped;
}

/* @0x6006bb5c: direct aliases and mismatched pointers are no-ops. */
void aud_data_adapter_unmap(struct aud_data_adapter *q, void *ptr)
{
    if (q->mapped && q->mapped == ptr) {
        free(q->mapped);
        q->mapped = NULL;
    }
}

/* @0x6006bbd0: NON-consuming retrieval. A first-object prefix is split into two
 * queued regions; caller receives an additional reference to the prefix. An
 * exact match retains the existing object; spanning requests return a new copy.
 * Offset normalization is destructive even if region allocation fails: firmware
 * releases the old object, stores NULL, and clears the offset. That failed queue
 * must not be reused without caller recovery; do not invent rollback behavior. */
struct aud_data *aud_data_adapter_get_data(struct aud_data_adapter *q, uint32_t size)
{
    struct adapter_holder *h, *prefix;
    struct aud_data *d, *first, *rest;
    if (!size || !(h = adapter_first(q)))
        return NULL;
    d = h->data;
    if (q->offset + size > d->size) {
        d = aud_data_new(size);
        if (d && aud_data_adapter_copy(q, d->ptr, size) < 0) {
            aud_object_put(d);
            d = NULL;
        }
        return d;
    }
    if (q->offset) {
        d = aud_data_region(h->data, q->offset, h->data->size - q->offset);
        aud_object_put(h->data);
        h->data = d;
        q->total -= q->offset;
        q->offset = 0;
    }
    if (!d)
        return NULL;
    if (size >= d->size)
        return aud_object_get(d);
    prefix = new_holder(q);
    if (!prefix)
        return NULL;
    if (aud_data_split2(d, size, &first, &rest) < 0) {
        adapter_before(&q->idle, &prefix->link);
        return NULL;
    }
    aud_object_put(d);
    prefix->data = first;
    h->data = rest;
    adapter_before(q->queued.next, &prefix->link);
    return aud_object_get(first);
}

/* @0x6006be14: removes first remaining object and transfers its reference.
 * Region allocation failure leaves both queue and ownership unchanged. */
struct aud_data *aud_data_adapter_dequeue(struct aud_data_adapter *q)
{
    struct adapter_holder *h = adapter_first(q);
    struct aud_data *d;
    if (!h)
        return NULL;
    d = h->data;
    if (q->offset) {
        d = aud_data_region(d, q->offset, d->size - q->offset);
        if (!d)
            return NULL;
        aud_object_put(h->data);
        h->data = d;
        q->total -= q->offset;
        q->offset = 0;
    }
    q->total -= d->size;
    adapter_recycle(q, h);
    if (!adapter_first(q))
        q->total = q->offset = 0;
    return d;
}

/* @0x6006bf7c: borrowed first OBJECT, not an offset-adjusted region, timestamp,
 * status, or retained reference. There is no second argument in the target. */
struct aud_data *aud_data_adapter_peep(struct aud_data_adapter *q)
{
    struct adapter_holder *h = adapter_first(q);
    return h ? h->data : NULL;
}
