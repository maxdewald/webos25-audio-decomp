/*
 * libaudcommon: aud_meta_data, a size-class cached allocator for small metadata blobs.
 * Returns a pointer just past a hidden 0x18-byte struct aud_meta_data header.
 *
 * RECONSTRUCTION from orig/usr/lib/libaudcommon.so.1.0.0
 *   sha256 a4df15e082096e6c60c7091b81f90f4fe0a6c5ae07173867ca4a0b05bfd5087f
 * Evidence: decomp/libaudcommon.so.1.0.0.c (_INIT_2 @0x11a2c, 0x14440..0x14820);
 * cache table = 17 x 0x34-byte entries in .data at VA 0x1640c; "aud_meta_bufdata" logs.
 */
#include <pthread.h>
#include <stdlib.h>

#include "local.h"

struct aud_meta_data_cache {            /* 0x34 */
    int size;                           /* 0x00 */
    pthread_mutex_t lock;               /* 0x04 */
    struct link allocated;              /* 0x1c */
    int count_allocated;                /* 0x24 */
    struct link freed;                  /* 0x28 */
    int count_freed;                    /* 0x30 */
};

static struct aud_meta_data_cache meta_caches[] = {
    { 0x00000 }, { 0x00010 }, { 0x00020 }, { 0x00040 }, { 0x00080 }, { 0x00100 },
    { 0x00200 }, { 0x00400 }, { 0x00800 }, { 0x01000 }, { 0x02000 }, { 0x04000 },
    { 0x08000 }, { 0x10000 }, { 0x20000 }, { 0x40000 }, { 0x80000 },
};
#define N_META_CACHES   (int)(sizeof(meta_caches) / sizeof(meta_caches[0]))

void aud_meta_data_cache_init(void) __attribute__((constructor));

/* @0x11a2c: constructor */
void aud_meta_data_cache_init(void)
{
    struct aud_meta_data_cache *c;

    for (c = meta_caches; c != meta_caches + N_META_CACHES; c++) {
        pthread_mutex_init(&c->lock, NULL);
        link_init(&c->allocated);
        link_init(&c->freed);
    }
}

/* @0x14440: walk every free list and unlink all entries.
 * [GAP-like defect, observed] the entries are unlinked and their cache pointer cleared,
 * but free() is never called (no free@plt call in the function), so this leaks them.
 */
int aud_meta_data_gc(void)
{
    int all_size = 0, all_count = 0;
    struct aud_meta_data_cache *c;

    for (c = meta_caches; c != meta_caches + N_META_CACHES; c++) {
        int freed_size = 0, freed_count = 0;

        pthread_mutex_lock(&c->lock);
        while (!link_empty(&c->freed)) {
            struct aud_meta_data *m = link_first(struct aud_meta_data, cache_link, &c->freed);

            freed_count++;
            freed_size += m->cache->size;
            all_size += m->cache->size;
            link_del(&m->cache_link);
            m->cache->count_freed--;
            m->cache = NULL;
        }
        all_count += freed_count;
        pthread_mutex_unlock(&c->lock);
        LOGN(aud_meta_bufdata, 0x92, "size %5x freed size %d, count %d\n",
             c->size, freed_size, freed_count);
    }
    LOGN(aud_meta_bufdata, 0x94, "all freed size %d, count %d\n", all_size, all_count);
    return 0;
}

/* @0x14624: NULL if size > 0x80000 or malloc fails */
void *aud_meta_data_new(int size)
{
    struct aud_meta_data_cache *c = NULL;
    struct aud_meta_data *m;
    int i;

    for (i = 0; i < N_META_CACHES; i++) {
        if (size <= meta_caches[i].size) {
            c = &meta_caches[i];
            break;
        }
    }
    if (!c) {
        LOGE(aud_meta_bufdata, 300, "no cache for size %d\n", size);
        return NULL;
    }

    pthread_mutex_lock(&c->lock);
    if (!link_empty(&c->freed)) {
        m = link_first(struct aud_meta_data, cache_link, &c->freed);
        LOGD(aud_meta_bufdata, 0x11b, "new %p\n", m);
        link_del(&m->cache_link);
        c->count_freed--;
        link_add_tail(&c->allocated, &m->cache_link);
        c->count_allocated++;
        pthread_mutex_unlock(&c->lock);
        return m->data;
    }

    m = malloc(c->size + sizeof(*m));
    if (!m) {
        LOGE(aud_meta_bufdata, 0x135, "no mem\n");
        pthread_mutex_unlock(&c->lock);
        return NULL;
    }
    m->cache = NULL;
    m->cache_link.prev = NULL;
    m->cache_link.next = NULL;
    m->return_addr = NULL;
    m->return_addr2 = NULL;
    m->magic = AUD_META_DATA_MAGIC;
    link_add_tail(&c->allocated, &m->cache_link);
    m->cache = c;
    c->count_allocated++;
    pthread_mutex_unlock(&c->lock);
    LOGD(aud_meta_bufdata, 0x14f, "new cache. cache size %x for size %x. at %p\n",
         c->size, size, __builtin_return_address(0));
    return m->data;
}

/* @0x1453c: back to the class free list; foreign pointers are passed to free() */
void aud_meta_data_free(void *buf)
{
    struct aud_meta_data *m = link_container(struct aud_meta_data, data, buf);
    struct aud_meta_data_cache *c;

    LOGD(aud_meta_bufdata, 0xeb, "free %p\n", m);
    if (m->magic != AUD_META_DATA_MAGIC) {
        LOG(aud_meta_bufdata, log_level_warning, 0xef,
            "not allocated with meta alloc\n (%p)\n", buf);
        free(buf);
        return;
    }
    c = m->cache;
    if (!c)
        return;
    pthread_mutex_lock(&c->lock);
    link_del(&m->cache_link);
    link_add_tail(&c->freed, &m->cache_link);
    c->count_allocated--;
    c->count_freed++;
    pthread_mutex_unlock(&c->lock);
}
