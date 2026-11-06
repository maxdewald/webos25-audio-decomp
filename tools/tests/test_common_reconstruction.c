// @sources src/libaudcommon/adapter.c src/libaudcommon/data.c src/libaudcommon/event.c
/* Host regressions for libaudcommon adapter and metadata reconstruction. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/libaudcommon/meta_data.c"

unsigned int logmm_dummy_mask;
#define MODULE(name) struct logmm_module logmm_module_##name = { .mask = &logmm_dummy_mask }
MODULE(ainst);
MODULE(audlog);
MODULE(aud_bufadp);
MODULE(aud_bufdata);
MODULE(aud_meta_bufdata);
MODULE(comutil);

static unsigned int copy_logs;
static unsigned int oops_logs;
static struct aud_data_adapter *change_offset_on_copy;

int logmm_printl(struct logmm_module *m, int level, const char *func, int line,
                 const char *fmt, ...)
{
    (void)m;
    (void)level;
    (void)func;
    (void)fmt;
    if (line == 0x6f7)
        copy_logs++;
    if (line == 0x6eb && change_offset_on_copy) {
        change_offset_on_copy->offset = 1;
        change_offset_on_copy = NULL;
    }
    return 0;
}

int logm_message(const char *fmt, ...)
{
    (void)fmt;
    oops_logs++;
    return 0;
}

static struct aud_data *buffer(const char *s)
{
    struct aud_data *d = aud_data_new((int)strlen(s));
    assert(d);
    memcpy(d->data, s, d->size);
    return d;
}

static void test_copy(unsigned int mask)
{
    struct aud_data_adapter a;
    char out[16] = {0};
    unsigned int logs;

    logmm_dummy_mask = mask;
    aud_data_adapter_init(&a);
    assert(aud_data_adapter_copy(&a, out, 0) == 0);
    assert(aud_data_adapter_copy(&a, out, 1) == -1);
    aud_data_adapter_push(&a, buffer("abc"));
    aud_data_adapter_push(&a, buffer("defg"));
    logs = copy_logs;
    assert(aud_data_adapter_copy(&a, out, 0) == 0);
    assert(copy_logs - logs == ((mask & 0x10) != 0));
    assert(aud_data_adapter_copy(&a, out, 5) == 0);
    assert(memcmp(out, "abcde", 5) == 0);
    assert(aud_data_adapter_size(&a) == 7);
    memset(out, 0, sizeof(out));
    assert(aud_data_adapter_copy(&a, out, 8) == -1);
    assert(memcmp(out, "abcdefg", 7) == 0);
    if (mask & 0x10) {
        change_offset_on_copy = &a;
        assert(aud_data_adapter_copy(&a, out, 2) == 0);
        assert(memcmp(out, "bc", 2) == 0);
        assert(a.offset == 1);
    }
    aud_data_adapter_cleanup(&a);
    assert(link_empty(&a.holder));
    assert(link_empty(&a.holder_free));
}

static void test_get_data(unsigned int mask)
{
    struct aud_data_adapter a;
    struct aud_data *first, *d;
    void *mapped;

    logmm_dummy_mask = mask;
    aud_data_adapter_init(&a);
    assert(!aud_data_adapter_get_data(&a, 0));
    assert(!aud_data_adapter_get_data(&a, 1));
    first = buffer("abcdef");
    first->info.timestamp = 123;
    aud_data_adapter_push(&a, first);
    d = aud_data_adapter_get_data(&a, 6);
    assert(d == first && d->object.user == 2);
    aud_data_put(d);
    aud_data_adapter_flush(&a, 1);
    d = aud_data_adapter_get_data(&a, 2);
    assert(d && d->size == 2 && memcmp(d->data, "bc", 2) == 0);
    assert(d->info.timestamp == 123);
    assert(a.offset == 0 && a.total_size == 5);
    assert(aud_data_adapter_peep(&a) == d);
    aud_data_put(d);
    mapped = aud_data_adapter_map(&a, 5);
    assert(mapped && memcmp(mapped, "bcdef", 5) == 0);
    aud_data_adapter_unmap(&a, mapped);
    d = aud_data_adapter_get_data(&a, 5);
    assert(d && d->size == 5 && memcmp(d->data, "bcdef", 5) == 0);
    assert(d->info.timestamp == (unsigned int)-1);
    aud_data_put(d);
    assert(!aud_data_adapter_get_data(&a, 6));
    assert(aud_data_adapter_size(&a) == 5);
    d = aud_data_adapter_dequeue(&a);
    assert(d && d->size == 2);
    aud_data_put(d);
    aud_data_adapter_flush(&a, 100);
    assert(a.total_size == 0 && a.offset == 0 && link_empty(&a.holder));
    aud_data_adapter_cleanup(&a);
}

static struct aud_meta_data *header(void *p)
{
    return link_container(struct aud_meta_data, data, p);
}

static void test_metadata(void)
{
    void *a = aud_meta_data_new(5);
    void *b = aud_meta_data_new(16);
    struct aud_meta_data_cache *c = header(a)->cache;
    void *reused;
    int i;

    assert(a && b && c == header(b)->cache && c->size == 16);
    assert(header(a)->magic == AUD_META_DATA_MAGIC);
    assert(!header(a)->return_addr && !header(a)->return_addr2);
    assert(c->count_allocated == 2 && c->count_freed == 0);
    assert(link_first(struct aud_meta_data, cache_link, &c->allocated) == header(b));
    aud_meta_data_free(a);
    assert(c->count_allocated == 1 && c->count_freed == 1);
    reused = aud_meta_data_new(1);
    assert(reused == a);
    assert(link_first(struct aud_meta_data, cache_link, &c->allocated) == header(a));
    assert(c->count_allocated == 2 && c->count_freed == 0);
    aud_meta_data_free(reused);
    aud_meta_data_free(b);
    a = aud_meta_data_new(-1);
    assert(a && header(a)->cache->size == 0);
    aud_meta_data_free(a);
    a = aud_meta_data_new(0x80000);
    assert(a && header(a)->cache->size == 0x80000);
    aud_meta_data_free(a);
    assert(!aud_meta_data_new(0x80001));
    for (i = 0; i < N_META_CACHES; i++) {
        c = &meta_caches[i];
        assert(link_empty(&c->allocated) && c->count_allocated == 0);
        while (!link_empty(&c->freed)) {
            struct aud_meta_data *m = link_first(struct aud_meta_data, cache_link, &c->freed);
            link_del(&m->cache_link);
            free(m);
            c->count_freed--;
        }
        assert(c->count_freed == 0);
    }
}

int main(void)
{
    test_copy(0);
    test_copy(0x1f);
    test_get_data(0);
    test_get_data(0x1f);
    assert(oops_logs == 1);
    test_metadata();
    aud_data_gc(0);
    puts("libaudcommon: adapter copy/get_data/map/flush and metadata cache regressions passed");
    return 0;
}
