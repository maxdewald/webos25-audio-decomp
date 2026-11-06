/*
 * libaudcommon: aud_data_adapter, a FIFO of aud_data that can be consumed by byte count
 * (flush/copy/map) or re-chunked into new aud_data (get_data/dequeue).
 *
 * RECONSTRUCTION from orig/usr/lib/libaudcommon.so.1.0.0
 *   sha256 a4df15e082096e6c60c7091b81f90f4fe0a6c5ae07173867ca4a0b05bfd5087f
 * Evidence: build/decomp/libaudcommon.so.1.0.0.c (new_holder @0x11db4, push_at @0x11e40,
 * 0x138e4..0x14440). Logs go to "aud_bufadp" except the holder allocation failures and
 * the get_data split errors, which use "aud_bufdata" (as in the binary).
 */
#include <stdlib.h>
#include <string.h>

#include "local.h"

/* @0x138e4 */
void aud_data_adapter_init(struct aud_data_adapter *adap)
{
    adap->total_size = 0;
    adap->offset = 0;
    adap->alloced_map = NULL;
    link_init(&adap->holder);
    link_init(&adap->holder_free);
}

/* @0x11db4: take a recycled holder or calloc a new one */
struct aud_data_holder *new_holder(struct aud_data_adapter *adap)
{
    struct aud_data_holder *h;

    if (!link_empty(&adap->holder_free)) {
        h = link_first(struct aud_data_holder, link, &adap->holder_free);
        link_del(&h->link);
        return h;
    }
    h = calloc(sizeof(*h), 1);
    if (!h) {
        LOGE(aud_bufdata, 0x686, "no mem.\n");
        return NULL;
    }
    LOGI(aud_bufdata, 0x689, "adp%p, new holder %p\n", adap, h);
    return h;
}

/* inlined: return a holder to the free list */
static inline void del_holder(struct aud_data_adapter *adap, struct aud_data_holder *h)
{
    link_del(&h->link);
    link_add_tail(&adap->holder_free, &h->link);
}

/* @0x11e40: takes ownership of data. If no holder can be allocated the data is
 * silently leaked (neither queued nor put). size 0 data is queued normally. */
void aud_data_adapter_push_at(struct aud_data_adapter *adap, struct aud_data *data, int front)
{
    struct aud_data_holder *h = new_holder(adap);

    if (!h) {
        LOGE(aud_bufdata, 0x6a4, "no mem.\n");
        return;
    }
    h->data = data;
    if (front)
        link_add(&adap->holder, &h->link);
    else
        link_add_tail(&adap->holder, &h->link);
    adap->total_size += data->size;
    LOGD(aud_bufadp, 0x6af, "adp%p, push %p(%d), total size %d, offset %d at %s\n",
         adap, data, data->size, adap->total_size - adap->offset, adap->offset,
         front ? "front" : "tail");
}

/* @0x138fc: NULL data is ignored */
void aud_data_adapter_push(struct aud_data_adapter *adap, struct aud_data *data)
{
    if (data)
        aud_data_adapter_push_at(adap, data, 0);
}

/* @0x13908 */
void aud_data_adapter_push_front(struct aud_data_adapter *adap, struct aud_data *data)
{
    if (data)
        aud_data_adapter_push_at(adap, data, 1);
}

/* @0x13914: drop size bytes from the head. Oversized requests are clamped to what is
 * queued (one-time "Oops" via logm_message). Always returns 0. */
int aud_data_adapter_flush(struct aud_data_adapter *adap, unsigned int size)
{
    static int oops_printed;
    unsigned int offset;

    LOGD(aud_bufadp, 0x6c1, "adp%p, flush %d\n", adap, size);
    if (size == 0)
        return 0;

    offset = adap->offset + size;
    if (offset > adap->total_size) {
        LOGE(aud_bufadp, 0x6c7, "adp%p, wrong flush size. %d %d %d\n",
             adap, adap->offset, size, adap->total_size);
        if (!oops_printed) {
            logm_message("Oops\n");
            oops_printed = 1;
        }
        offset = adap->total_size;
    }
    adap->offset = offset;

    /* release every head buffer that is fully consumed */
    while (!link_empty(&adap->holder)) {
        struct aud_data_holder *h = link_first(struct aud_data_holder, link, &adap->holder);

        if (h->data->size > adap->offset)
            break;
        LOGD(aud_bufadp, 0x6d2, "adp%p, del %p(%d)\n", adap, h->data, h->data->size);
        adap->total_size -= h->data->size;
        adap->offset -= h->data->size;
        aud_data_put(h->data);
        del_holder(adap, h);
    }
    LOGD(aud_bufadp, 0x6da, "adp%p, remains %d. offset %d\n",
         adap, adap->total_size - adap->offset, adap->offset);
    if (adap->total_size == adap->offset && link_empty(&adap->holder)) {
        adap->total_size = 0;
        adap->offset = 0;
    }
    return 0;
}

/* @0x13aa4 */
void aud_data_adapter_cleanup(struct aud_data_adapter *adap)
{
    aud_data_adapter_flush_all(adap);
    while (!link_empty(&adap->holder_free)) {
        struct aud_data_holder *h = link_first(struct aud_data_holder, link, &adap->holder_free);

        LOGI(aud_bufadp, 0x677, "adp%p, del idle %p\n", adap, h);
        link_del(&h->link);
        free(h);
    }
}

/* @0x13b24: copy size bytes from the head without consuming them. -1 if fewer queued
 * (the partial copy stays in the destination). Even size 0 visits a queued head. */
int aud_data_adapter_copy(struct aud_data_adapter *adap, void *data, unsigned int size)
{
    unsigned int offset;
    unsigned char *dst = data;
    struct link *node;

    LOGD(aud_bufadp, 0x6eb, "adp%p, copy %d bytes\n", adap, size);
    offset = adap->offset;
    for (node = adap->holder.next; node != &adap->holder; node = node->next) {
        struct aud_data_holder *holder = link_container(struct aud_data_holder, link, node);
        unsigned int copy_size = holder->data->size - offset;

        if (copy_size > size)
            copy_size = size;
        LOGD(aud_bufadp, 0x6f7, "adp%p, copy %d\n", adap, copy_size);
        memcpy(dst, holder->data->data + offset, copy_size);
        offset = 0;
        size -= copy_size;
        dst += copy_size;
        if (!size)
            break;
    }
    if (size) {
        LOGE(aud_bufadp, 0x704, "adp%p, no data. remained size %d\n", adap, size);
        return -1;
    }
    LOGD(aud_bufadp, 0x708, "adp%p, copy done\n", adap);
    return 0;
}

/* @0x13c68: pointer to the first size bytes. Points into the head aud_data when it is
 * contiguous there, else into a malloc'ed bounce copy. NULL for size 0, an empty
 * adapter, malloc failure, or not enough data queued. */
void *aud_data_adapter_map(struct aud_data_adapter *adap, unsigned int size)
{
    struct aud_data_holder *h;

    LOGD(aud_bufadp, 0x710, "adp%p, map %d\n", adap, size);
    if (size == 0)
        return NULL;
    if (link_empty(&adap->holder)) {
        LOGE(aud_bufadp, 0x716, "adp%p, Oops. size %d\n", adap, size);
        return NULL;
    }
    if (adap->alloced_map) {
        LOGE(aud_bufadp, 0x71c, "adp%p, no unmap before map. %p\n", adap, adap->alloced_map);
        free(adap->alloced_map);
        adap->alloced_map = NULL;
    }

    h = link_first(struct aud_data_holder, link, &adap->holder);
    if (adap->offset + size <= h->data->size) {
        LOGD(aud_bufadp, 0x724, "adp%p, use aud_data memory\n", adap);
        return h->data->data + adap->offset;
    }

    adap->alloced_map = malloc(size);
    if (!adap->alloced_map) {
        LOGE(aud_bufadp, 0x72b, "adp%p, no mem for %d\n", adap, size);
        return NULL;
    }
    if (aud_data_adapter_copy(adap, adap->alloced_map, size) < 0) {
        LOGE(aud_bufadp, 0x732, "adp%p, cannot copy.. %d\n", adap, size);
        free(adap->alloced_map);
        adap->alloced_map = NULL;
        return NULL;
    }
    LOGD(aud_bufadp, 0x738, "adp%p, allocated %p\n", adap, adap->alloced_map);
    return adap->alloced_map;
}

/* @0x13e40: only frees the bounce buffer; direct pointers need no unmap */
void aud_data_adapter_unmap(struct aud_data_adapter *adap, void *ptr)
{
    if (!adap->alloced_map) {
        LOGD(aud_bufadp, 0x741, "adp%p, not mapped. %p\n", adap, ptr);
        return;
    }
    if (adap->alloced_map != ptr) {
        LOGE(aud_bufadp, 0x747, "adp%p, unknown ptr. %p != %p\n", adap, adap->alloced_map, ptr);
        return;
    }
    LOGD(aud_bufadp, 0x74b, "adp%p, free %p\n", adap, ptr);
    free(adap->alloced_map);
    adap->alloced_map = NULL;
}

/* @0x13efc: new reference to an aud_data holding the first size bytes (not consumed).
 *  - spans several buffers: fresh aud_data_new(size) + copy. NOTE: info (type, timestamps,
 *    flag) is NOT copied, so the result is type_pcm with ts -1.
 *  - head buffer with offset: head replaced by a region without the consumed prefix.
 *  - head larger than size: head split in two (split2); first half returned.
 *  - otherwise (head is exactly size bytes) a new reference to the head buffer itself.
 */
struct aud_data *aud_data_adapter_get_data(struct aud_data_adapter *adap, unsigned int size)
{
    struct aud_data_holder *h, *nh;
    struct aud_data *d, *d1, *d2;

    LOGD(aud_bufadp, 0x755, "adp%p, map %d\n", adap, size);
    if (size == 0)
        return NULL;
    if (link_empty(&adap->holder)) {
        LOGE(aud_bufadp, 0x75b, "adp%p, Oops. size %d\n", adap, size);
        return NULL;
    }

    h = link_first(struct aud_data_holder, link, &adap->holder);
    d = h->data;
    if (d->size < adap->offset + size) {
        d = aud_data_new(size);
        if (!d) {
            LOGE(aud_bufadp, 0x79a, "adp%p, no data for %d\n", adap, size);
            return NULL;
        }
        if (aud_data_adapter_copy(adap, d->data, size) < 0) {
            LOGE(aud_bufadp, 0x7a1, "adp%p, cannot copy.. %d\n", adap, size);
            aud_data_put(d);
            return NULL;
        }
        LOGD(aud_bufadp, 0x7a6, "adp%p, data allocated %d\n", adap, size);
        return d;
    }

    if (adap->offset) {
        LOGD(aud_bufadp, 0x766, "adp%p, remove first offset %d\n", adap, adap->offset);
        d = aud_data_region(h->data, adap->offset, h->data->size - adap->offset);
        aud_data_put(h->data);
        h->data = d;
        adap->total_size -= adap->offset;
        adap->offset = 0;
        if (!d) {
            /* holder left queued with a NULL data pointer */
            LOGE(aud_bufadp, 0x771, "h->data is NULL.\n");
            return NULL;
        }
    }

    if (d->size <= size)
        return aud_data_get(d);

    LOGD(aud_bufadp, 0x77a, "adp%p, split buffer %d into %d\n", adap, d->size, size);
    nh = new_holder(adap);
    if (!nh) {
        LOGE(aud_bufdata, 0x77e, "no mem.\n");
        return NULL;
    }
    if (aud_data_split2(h->data, size, &d1, &d2) < 0) {
        link_add_tail(&adap->holder_free, &nh->link);
        LOGE(aud_bufdata, 0x785, "cannot split. %d\n", size);
        return NULL;
    }
    aud_data_put(h->data);
    nh->data = d1;
    h->data = d2;
    link_add(&adap->holder, &nh->link);
    return aud_data_get(d1);
}

/* @0x14204: pop the head aud_data (ownership to caller), trimming a consumed prefix */
struct aud_data *aud_data_adapter_dequeue(struct aud_data_adapter *adap)
{
    struct aud_data_holder *h;
    struct aud_data *d;

    LOGD(aud_bufadp, 0x7af, "adp%p, dequeue\n", adap);
    if (link_empty(&adap->holder)) {
        LOGE(aud_bufadp, 0x7b3, "adp%p, Oops. no more data\n", adap);
        return NULL;
    }
    h = link_first(struct aud_data_holder, link, &adap->holder);
    if (adap->offset) {
        LOGD(aud_bufadp, 0x7bd, "adp%p, remove first offset %d\n", adap, adap->offset);
        d = aud_data_region(h->data, adap->offset, h->data->size - adap->offset);
        if (!d) {
            LOGE(aud_bufadp, 0x7c1, "adp%p, no mem.\n", adap);
            return NULL;
        }
        aud_data_put(h->data);
        h->data = d;
        adap->total_size -= adap->offset;
        adap->offset = 0;
    }
    d = h->data;
    LOGD(aud_bufadp, 0x7cb, "adp%p, del %p(%d)\n", adap, d, d->size);
    adap->total_size -= d->size;
    del_holder(adap, h);
    LOGD(aud_bufadp, 0x7d2, "adp%p, remains %d. offset %d\n",
         adap, adap->total_size - adap->offset, adap->offset);
    if (link_empty(&adap->holder)) {
        adap->total_size = 0;
        adap->offset = 0;
    }
    return d;
}

/* @0x143b8: head aud_data, no new reference, offset not applied */
struct aud_data *aud_data_adapter_peep(struct aud_data_adapter *adap)
{
    LOGD(aud_bufadp, 0x7e1, "adp%p, peep. offset %d\n", adap, adap->offset);
    if (link_empty(&adap->holder)) {
        LOGE(aud_bufadp, 0x7e5, "adp%p, Oops. no more data\n", adap);
        return NULL;
    }
    return link_first(struct aud_data_holder, link, &adap->holder)->data;
}
