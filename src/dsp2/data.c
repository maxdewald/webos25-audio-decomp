/* DSP2 data objects, 0x6006acd4..0x6006b638 (exclusive).
 * Saved image scalar instructions and validated FLIX branch slots are evidence.
 * Host semantic layout, NOT firmware ABI: public prefix matches data_adapter.c;
 * firmware's object header (+0..+0xc) is represented after that prefix here.
 * Allocator hooks represent 600e97b4/600e9550, not their misleading export names.
 * IRQ/mutex, object references and allocator state are external dependencies.
 * Debug logging and Xtensa return-address provenance are omitted, not guessed.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct data_link { struct data_link *next, *prev; };
struct data_cache;
struct aud_data {
    uint32_t metadata[8];             /* firmware +0x10..+0x2c */
    uint32_t provenance;              /* +0x30 */
    uint32_t size;                    /* +0x34 */
    unsigned char *ptr;               /* +0x38 */
    struct data_link link;           /* +0x3c, +0x40 */
    struct data_cache *cache;         /* +0x44 */
    uint32_t owns_buffer;            /* +0x48: separately allocated buffer */
    struct aud_data *parent;         /* +0x4c: retained immediate parent */
    uint32_t refs, type, reserved;    /* firmware +0, +4, +0xc */
    void (*destroy)(struct aud_data *); /* firmware +8 */
};
struct data_cache {
    uint32_t size;
    int32_t limit;
    uint32_t irq;
    struct data_link allocated;
    int32_t allocated_count;
    struct data_link idle;
    int32_t free_count;
};
extern void *aud_object_get(void *);
extern void aud_object_put(void *);
extern void *aud_data_alloc(size_t, int32_t);
extern void aud_data_free(void *);
extern uint32_t aud_data_irq_save(void);
extern void aud_data_irq_restore(uint32_t);
extern void aud_mutex_lock(void *);
extern void aud_mutex_unlock(void *);
extern uintptr_t aud_data_heap_boundary;
extern void logm_message(const char *, ...);
extern const char *symbol_get_name(uint32_t, int);

static struct data_cache data_caches[17] = {
    {.size=0, .limit=10}, {.size=16, .limit=20},
    {.size=32, .limit=20}, {.size=64, .limit=20},
    {.size=128, .limit=10}, {.size=256, .limit=15},
    {.size=512, .limit=15}, {.size=1024, .limit=10},
    {.size=2048, .limit=10}, {.size=4096, .limit=10},
    {.size=8192, .limit=10}, {.size=16384, .limit=10},
    {.size=33280, .limit=8}, {.size=66048, .limit=4},
    {.size=131584, .limit=2}, {.size=262656, .limit=1},
    {.size=524800, .limit=1}
};
static unsigned zero_mutex;
static struct aud_data *zero_data;
static int32_t zero_size;
static unsigned char *zero_ptr;

static struct aud_data *data_from_link(struct data_link *p)
{
    return (struct aud_data *)((unsigned char *)p - offsetof(struct aud_data, link));
}
static unsigned char *data_offset(unsigned char *p, uint32_t offset)
{
    return (unsigned char *)((uintptr_t)p + offset);
}
static void data_unlink(struct data_link *p)
{
    if (p->next) {
        p->next->prev = p->prev;
        p->prev->next = p->next;
    }
}
static void data_append(struct data_link *head, struct data_link *p)
{
    if (head->next != p) {
        p->next = head;
        p->prev = head->prev;
        head->prev->next = p;
        head->prev = p;
    }
}

/* @0x6006acd4: initializes list heads only; counters/configuration survive. */
void cache_initializer(void)
{
    unsigned i;
    for (i = 0; i < 17; ++i) {
        struct data_cache *c = &data_caches[i];
        c->allocated.next = c->allocated.prev = &c->allocated;
        c->idle.next = c->idle.prev = &c->idle;
    }
}

/* @0x6006ad18: destructor, NOT the duplicate exported aud_data_new name.
 * Cache retains storage, not a live object reference. Parent is always released.
 */
void aud_data_release(struct aud_data *d)
{
    struct data_cache *c;
    if (d->parent) {
        aud_object_put(d->parent);
        d->parent = NULL;
    }
    c = d->cache;
    if (c) {
        c->irq = aud_data_irq_save();
        data_unlink(&d->link);
        data_append(&c->idle, &d->link);
        ++c->free_count;
        --c->allocated_count;
        aud_data_irq_restore(c->irq);
        return;
    }
    if (d->owns_buffer)
        aud_data_free(d->ptr);
    d->type = UINT32_C(0x0dad0dad);
    d->destroy = NULL; /* firmware writes poison 0x0da00da0 */
    d->ptr = NULL;    /* firmware writes poison 0x0dadada0 */
    aud_data_free(d);
}

/* @0x6006adb4: 0 drains all idle objects; positive n removes ONE per cache
 * whose free count exceeds n; negative n uses each cache's configured limit.
 * Return is always zero, NOT bytes freed. Allocated objects are never collected.
 */
int aud_data_gc(int32_t n)
{
    unsigned i;
    for (i = 0; i < 17; ++i) {
        struct data_cache *c = &data_caches[i];
        int32_t threshold = n < 1 ? c->limit : n;
        c->irq = aud_data_irq_save();
        if (!n || c->free_count > threshold) {
            while (c->idle.prev != &c->idle) {
                struct aud_data *d = data_from_link(c->idle.prev);
                data_unlink(&d->link);
                --c->free_count;
                d->cache = NULL;
                aud_data_release(d);
                if (n) break;
            }
        }
        aud_data_irq_restore(c->irq);
    }
    return 0;
}

/* @0x6006aedc: checks the allocator cell, not data payload bounds.
 * Allocator contract: 16-byte prefix, magic at +0, 32-bit cell span at +8.
 * Boundary comparison is deliberately in the target's observed direction.
 */
int check_data_corrupt(const void *data)
{
    const unsigned char *cell = (const unsigned char *)data - 16;
    const unsigned char *next;
    uint32_t magic, span;
    memcpy(&magic, cell, sizeof(magic));
    if (magic != UINT32_C(0x12340102)) return -1;
    memcpy(&span, cell + 8, sizeof(span));
    next = cell + span;
    if ((uintptr_t)next > aud_data_heap_boundary) {
        memcpy(&magic, next, sizeof(magic));
        if (magic != UINT32_C(0x12340102)) return -1;
    }
    return 0;
}

/* @0x6006af44: optional object check, then all allocated cache lists only. */
int aud_data_check_corruption(const struct aud_data *d)
{
    unsigned i;
    if (d && check_data_corrupt(d) < 0) return -1;
    for (i = 0; i < 17; ++i) {
        struct data_cache *c = &data_caches[i];
        struct data_link *p;
        c->irq = aud_data_irq_save();
        for (p = c->allocated.prev; p != &c->allocated; p = p->prev) {
            if (check_data_corrupt(data_from_link(p)) < 0) {
                aud_data_irq_restore(c->irq);
                return -1;
            }
        }
        aud_data_irq_restore(c->irq);
    }
    return 0;
}

/* @0x6006b044: debug command; argc is unused in the firmware. */
int aud_data_cache(int argc, const char *const argv[])
{
    unsigned i;
    int allocated, idle;
    (void)argc;
    if (!argv[1]) return 0;
    if (!strcmp(argv[1], "gc"))
        return aud_data_gc(argv[2] ? (int32_t)strtoul(argv[2], NULL, 10) : 0);
    if (strcmp(argv[1], "status")) return 0;
    allocated = argv[2] && (!strcmp(argv[2], "all") || !strcmp(argv[2], "alloc"));
    idle = argv[2] && (!strcmp(argv[2], "all") || !strcmp(argv[2], "free"));
    for (i = 0; i < 17; ++i) {
        struct data_cache *c = &data_caches[i];
        unsigned list;
        logm_message("size %5x free count %d, allocated count %d\n",
                     c->size, c->free_count, c->allocated_count);
        for (list = 0; list < 2; ++list) {
            struct data_link *head = list ? &c->idle : &c->allocated, *p;
            if (!(list ? idle && c->free_count > 0 : allocated && c->allocated_count > 0))
                continue;
            logm_message(list ? "\tfree list:\n" : "\tallocated list:\n");
            for (p = head->prev; p != head; p = p->prev) {
                struct aud_data *d = data_from_link(p);
                logm_message("\t%p: sz : %05x caller=%p %s()\n", (void *)d,
                             d->size, (void *)(uintptr_t)d->provenance,
                             symbol_get_name(d->provenance, 0));
            }
        }
    }
    return 0;
}

/* @0x6006b178 */
int dump_aud_data_cache(void)
{
    const char *args[] = {"", "status", "all"};
    (void)aud_data_cache(3, args);
    return 0;
}

/* @0x6006b198: smallest fitting bucket; reuse its most recently freed object.
 * Call cache_initializer once before use. Payload is not zeroed on reuse.
 * Signed target size comparisons are retained; normal callers use <= INT32_MAX.
 */
struct aud_data *aud_data_new(uint32_t size)
{
    struct data_cache *c = NULL;
    struct aud_data *d;
    unsigned i;
    uint32_t capacity = size;
    (void)aud_data_gc(-1);
    for (i = 0; i < 17; ++i) {
        if ((int32_t)data_caches[i].size >= (int32_t)size) {
            c = &data_caches[i];
            break;
        }
    }
    if (c) {
        c->irq = aud_data_irq_save();
        capacity = c->size;
    }
    if (c && c->idle.prev != &c->idle) {
        d = data_from_link(c->idle.prev);
        data_unlink(&d->link);
        --c->free_count;
        memset(d, 0, sizeof(*d));
        d->ptr = (unsigned char *)(d + 1);
        d->size = size;
    } else {
        d = aud_data_alloc(sizeof(*d) + capacity, -1);
        if (!d) {
            if (c) aud_data_irq_restore(c->irq);
            return NULL;
        }
        memset(d, 0, sizeof(*d));
        if ((int32_t)size > 0) {
            d->size = size;
            d->ptr = (unsigned char *)(d + 1);
        }
    }
    d->refs = 1;
    d->type = UINT32_C(0x64617461);
    d->destroy = aud_data_release;
    d->metadata[1] = d->metadata[2] = UINT32_MAX;
    if (c) {
        data_append(&c->allocated, &d->link);
        d->cache = c;
        ++c->allocated_count;
        aud_data_irq_restore(c->irq);
    }
    return d;
}

/* @0x6006b3a0: shared zero backing grows, never shrinks. Each result retains
 * the backing object, not the previous view. Allocation of the separate zero
 * payload is NOT checked by firmware: failure still returns a NULL-pointer view.
 * A failed backing-object replacement leaves zero_size/zero_ptr unchanged.
 * Cached headers keep owns_buffer until GC; reusing one resets it without
 * freeing that separate allocation, as in firmware (no invented leak repair).
 */
struct aud_data *aud_data_new_zero(uint32_t size)
{
    struct aud_data *d;
    aud_mutex_lock(&zero_mutex);
    if (zero_size < (int32_t)size) {
        if (zero_data) aud_object_put(zero_data);
        zero_data = aud_data_new(0);
        if (!zero_data) {
            aud_mutex_unlock(&zero_mutex);
            return NULL;
        }
        zero_ptr = calloc(size, 1);
        zero_size = (int32_t)size;
        zero_data->ptr = zero_ptr;
        zero_data->size = size;
        zero_data->owns_buffer = 1;
    }
    d = aud_data_new(0);
    if (d) {
        d->size = size;
        d->parent = aud_object_get(zero_data);
        d->ptr = zero_ptr;
        d->metadata[6] |= 1;
    }
    aud_mutex_unlock(&zero_mutex);
    return d;
}

/* @0x6006b45c: unchecked, zero-copy region; retains immediate source even if
 * that source is itself a region. No size/offset validation is performed.
 */
struct aud_data *aud_data_region(struct aud_data *d, uint32_t offset, uint32_t size)
{
    struct aud_data *r = aud_data_new(0);
    if (!r) return NULL;
    memcpy(r->metadata, d->metadata, sizeof(r->metadata));
    r->ptr = data_offset(d->ptr, offset);
    r->size = size;
    r->parent = aud_object_get(d);
    return r;
}

/* @0x6006b4b4: returns retained suffix and MUTATES source size to offset.
 * Unlike split2, does not validate offset. Failure leaves source unchanged.
 */
struct aud_data *aud_data_split(struct aud_data *d, uint32_t offset)
{
    struct aud_data *r = aud_data_new(0);
    if (!r) return NULL;
    memcpy(r->metadata, d->metadata, sizeof(r->metadata));
    r->ptr = data_offset(d->ptr, offset);
    r->size = d->size - offset;
    r->parent = aud_object_get(d);
    d->size = offset;
    return r;
}

/* @0x6006b514: leaves source unchanged; outputs two independent references.
 * Both allocations are attempted even if the first fails; partial outputs are
 * released and cleared. Valid offsets are strictly inside the source buffer.
 */
int aud_data_split2(struct aud_data *d, uint32_t offset,
                    struct aud_data **first, struct aud_data **rest)
{
    *rest = NULL;
    *first = NULL;
    if (!offset || offset >= d->size) return -1;
    *first = aud_data_new(0);
    *rest = aud_data_new(0);
    if (!*first || !*rest) {
        if (*first) aud_object_put(*first);
        if (*rest) aud_object_put(*rest);
        *rest = NULL;
        *first = NULL;
        return -1;
    }
    memcpy((*first)->metadata, d->metadata, sizeof(d->metadata));
    (*first)->ptr = d->ptr;
    (*first)->size = offset;
    (*first)->parent = aud_object_get(d);
    memcpy((*rest)->metadata, d->metadata, sizeof(d->metadata));
    (*rest)->ptr = data_offset(d->ptr, offset);
    (*rest)->size = d->size - offset;
    (*rest)->parent = aud_object_get(d);
    return 0;
}
