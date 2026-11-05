/*
 * libaudcommon: aud_data buffers, their per-size free-list cache, regions/splits and the
 * shared all-zero buffer.
 *
 * RECONSTRUCTION from orig/usr/lib/libaudcommon.so.1.0.0
 *   sha256 a4df15e082096e6c60c7091b81f90f4fe0a6c5ae07173867ca4a0b05bfd5087f
 * Evidence: decomp/libaudcommon.so.1.0.0.c (_INIT_1 @0x119f8, data_destroy @0x12068,
 * 0x13244..0x138e4); cache table = 17 x 0x38-byte entries in .data at VA 0x1602c,
 * guarded by the "aud_bufdata" logmm module.
 */
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"

struct aud_data_cache {                 /* 0x38 */
    int size;                           /* 0x00 payload capacity of this class */
    int max_count;                      /* 0x04 freed entries kept by aud_data_gc(-1) */
    pthread_mutex_t lock;               /* 0x08 */
    struct link allocated;              /* 0x20 */
    int count_allocated;                /* 0x28 */
    struct link freed;                  /* 0x2c */
    int count_freed;                    /* 0x34 */
};

/* size classes (payload bytes, kept-free count) as shipped */
static struct aud_data_cache caches[] = {
    { 0x00000, 10 }, { 0x00010, 20 }, { 0x00020, 20 }, { 0x00040, 20 },
    { 0x00080, 10 }, { 0x00100, 15 }, { 0x00200, 15 }, { 0x00400, 10 },
    { 0x00800, 10 }, { 0x01000, 10 }, { 0x02000, 10 }, { 0x04000, 10 },
    { 0x08200, 8 },  { 0x10200, 4 },  { 0x20200, 2 },  { 0x40200, 1 },
    { 0x80200, 1 },
};
#define N_CACHES    (int)(sizeof(caches) / sizeof(caches[0]))

void aud_data_cache_init(void) __attribute__((constructor));

/* @0x119f8: constructor, initialises every cache lock and list */
void aud_data_cache_init(void)
{
    struct aud_data_cache *c;

    for (c = caches; c != caches + N_CACHES; c++) {
        pthread_mutex_init(&c->lock, NULL);
        link_init(&c->allocated);
        link_init(&c->freed);
    }
}

/* @0x12068: destroy callback. Cached buffers go back to their free list (the payload
 * is never freed there, even with data_allocated set); uncached ones are freed. */
void data_destroy(struct aud_object *obj)
{
    struct aud_data *d = (struct aud_data *)obj;
    struct aud_data_cache *c;

    if (d->parent) {
        aud_data_put(d->parent);
        d->parent = NULL;
    }
    c = d->cache;
    if (!c) {
        if (d->data_allocated)
            free(d->data);
        free(d);
        return;
    }
    pthread_mutex_lock(&c->lock);
    link_del(&d->cache_link);
    link_add_tail(&c->freed, &d->cache_link);
    c->count_allocated--;
    c->count_freed++;
    pthread_mutex_unlock(&c->lock);
}

/* @0x13244: trim the free lists.
 *   min_cache_cnt == 0: free every cached-free buffer of every class.
 *   min_cache_cnt  > 0: per class, free ONE buffer if more than min_cache_cnt are free.
 *   min_cache_cnt  < 0: same, against the class' own max_count (called by aud_data_new).
 */
int aud_data_gc(int min_cache_cnt)
{
    int all_size = 0, all_count = 0;
    struct aud_data_cache *c;

    for (c = caches; c != caches + N_CACHES; c++) {
        int limit = min_cache_cnt > 0 ? min_cache_cnt : c->max_count;
        int freed_size = 0, freed_count = 0;

        pthread_mutex_lock(&c->lock);
        LOGT(aud_bufdata, 0x4d3,
             "cache status --> size %5x free count %d, allocated count %d max %d\n",
             c->size, c->count_freed, c->count_allocated, c->max_count);
        if (min_cache_cnt != 0 && c->count_freed <= limit) {
            pthread_mutex_unlock(&c->lock);
            continue;
        }
        while (!link_empty(&c->freed)) {
            struct aud_data *d = link_first(struct aud_data, cache_link, &c->freed);

            freed_count++;
            all_count++;
            freed_size += d->cache->size;
            all_size += d->cache->size;
            link_del(&d->cache_link);
            d->cache->count_freed--;
            d->cache = NULL;
            data_destroy(&d->object);
            if (min_cache_cnt != 0)
                break;
        }
        pthread_mutex_unlock(&c->lock);
        LOGD(aud_bufdata, 0x4f3, "cache delete --> size %5x freed size %d, count %d\n",
             c->size, freed_size, freed_count);
    }
    LOGT(aud_bufdata, 0x4f7, "all freed size %d, count %d\n", all_size, all_count);
    return 0;
}

/* clears 0x0c..0x50: padding before info, info, caller, size, data, cache_link, cache,
 * data_allocated, parent */
static inline void clear_header(struct aud_data *d)
{
    memset((char *)d + sizeof(struct aud_object), 0, sizeof(*d) - sizeof(struct aud_object));
}

static inline void data_init_header(struct aud_data *d)
{
    d->object.user = 1;
    d->object.type = AUD_OBJECT_TYPE_DATA;
    d->object.destroy = data_destroy;
    d->info.index = (unsigned int)-1;
    d->info.timestamp = (unsigned int)-1;
}

/* @0x133ec: allocate a buffer with room for size payload bytes.
 * Everything from 0x0c to 0x50 is zeroed: info.type = type_pcm, info.flag = 0,
 * timestamp64 = 0, parent = NULL, data_allocated = 0; index/timestamp = -1, user = 1.
 * data points at the inline _data[] (or NULL for size <= 0 on the malloc path).
 * The smallest class with capacity >= size is used; a buffer on its free list is reused,
 * otherwise a new one of the class capacity is malloc'ed under the class lock. Sizes above
 * 0x80200 are malloc'ed uncached. Returns NULL only if malloc fails.
 */
struct aud_data *aud_data_new(int size)
{
    struct aud_data_cache *c = NULL;
    struct aud_data *d;
    int alloc_size;
    int i;

    aud_data_gc(-1);

    for (i = 0; i < N_CACHES; i++) {
        if (size <= caches[i].size) {
            c = &caches[i];
            break;
        }
    }

    if (c) {
        pthread_mutex_lock(&c->lock);
        if (!link_empty(&c->freed)) {
            d = link_first(struct aud_data, cache_link, &c->freed);
            link_del(&d->cache_link);
            c->count_freed--;
            clear_header(d);
            data_init_header(d);
            d->data = d->_data;
            d->size = size;
            link_add_tail(&c->allocated, &d->cache_link);
            c->count_allocated++;
            d->cache = c;
            pthread_mutex_unlock(&c->lock);
            return d;
        }
        alloc_size = c->size;
    } else {
        LOGD(aud_bufdata, 0x5b2, "no cache for size %d\n", size);
        alloc_size = size;
    }

    d = malloc(alloc_size + sizeof(*d));
    if (!d) {
        LOGE(aud_bufdata, 0x5bc, "no mem\n");
        if (c)
            pthread_mutex_unlock(&c->lock);
        return NULL;
    }
    clear_header(d);
    if (size > 0)
        d->size = size;
    data_init_header(d);
    if (size > 0)
        d->data = d->_data;
    if (c) {
        link_add_tail(&c->allocated, &d->cache_link);
        d->cache = c;
        c->count_allocated++;
        pthread_mutex_unlock(&c->lock);
        LOGI(aud_bufdata, 0x5d8, "new cache. cache size %x for size %x. at %p\n",
             c->size, size, __builtin_return_address(0));
    }
    return d;
}

static pthread_mutex_t zero_lock = PTHREAD_MUTEX_INITIALIZER;
static int zero_size;
static struct aud_data *zero_master;
static void *zero_mem;

/* @0x13630: a header-only aud_data whose payload is a shared calloc'ed block of zeros
 * (grown on demand), with info.flag bit 0 set and parent = the shared master buffer.
 */
struct aud_data *aud_data_new_zero(int size)
{
    struct aud_data *d;

    pthread_mutex_lock(&zero_lock);
    if (zero_size < size) {
        if (zero_master)
            aud_data_put(zero_master);
        zero_master = aud_data_new(0);
        if (!zero_master) {
            LOGE(aud_bufdata, 0x5f2, "no mem\n");
            goto fail;
        }
        zero_mem = calloc(size, 1);
        zero_size = size;
        zero_master->size = size;
        zero_master->data = zero_mem;
        zero_master->data_allocated = 1;
    }
    d = aud_data_new(0);
    if (!d)
        goto fail;
    d->parent = aud_data_get(zero_master);
    d->size = size;
    d->data = zero_mem;
    d->info.flag |= AUD_DATA_FLAG_ZERO;
    pthread_mutex_unlock(&zero_lock);
    return d;

fail:
    pthread_mutex_unlock(&zero_lock);
    return NULL;
}

/* @0x13704: view of [offset, offset+size) of data; copies info, holds a ref on data.
 * No bounds check. */
struct aud_data *aud_data_region(struct aud_data *data, unsigned int offset, int size)
{
    struct aud_data *d = aud_data_new(0);

    if (d) {
        d->info = data->info;
        d->size = size;
        d->data = data->data + offset;
        d->parent = aud_data_get(data);
    }
    return d;
}

/* @0x13744: deprecated. Returns the tail [offset, size) and truncates data to offset. */
struct aud_data *aud_data_split(struct aud_data *data, unsigned int offset)
{
    struct aud_data *d = aud_data_new(0);

    if (d) {
        d->info = data->info;
        d->size = data->size - offset;
        d->data = data->data + offset;
        d->parent = aud_data_get(data);
        data->size = offset;
    }
    return d;
}

/* @0x1378c: two views [0, offset) and [offset, size); both inherit info. */
int aud_data_split2(struct aud_data *data, unsigned int offset,
                    struct aud_data **d1, struct aud_data **d2)
{
    *d2 = NULL;
    *d1 = NULL;
    if (offset == 0 || offset >= data->size) {
        LOGE(aud_bufdata, 0x639, "wrong offset. size %d, offset %d\n", data->size, offset);
        return -1;
    }
    *d1 = aud_data_new(0);
    *d2 = aud_data_new(0);
    if (!*d1 || !*d2) {
        LOGE(aud_bufdata, 0x641, "no mem\n");
        if (*d1)
            aud_data_put(*d1);
        if (*d2)
            aud_data_put(*d2);
        *d2 = NULL;
        *d1 = NULL;
        return -1;
    }
    (*d1)->info = data->info;
    (*d1)->size = offset;
    (*d1)->data = data->data;
    (*d1)->parent = aud_data_get(data);

    (*d2)->info = data->info;
    (*d2)->data = data->data + offset;
    (*d2)->size = data->size - offset;
    (*d2)->parent = aud_data_get(data);
    return 0;
}
