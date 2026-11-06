/* Standalone: cc -std=c11 -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined tools/tests/dsp2/data.c -o build/scratch/dsp2-data-test && build/scratch/dsp2-data-test */
#include <stdlib.h>
static void *mock_calloc(size_t, size_t);
#define calloc mock_calloc
#include "../../../src/dsp2/data.c"
#undef calloc
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

static unsigned live_allocs, alloc_calls, frees, ref_gets, ref_puts, irq_depth, locks, messages;
static unsigned fail_at;
static int fail_zero;
uintptr_t aud_data_heap_boundary;

void *aud_data_alloc(size_t n, int32_t flag)
{
    unsigned char *cell;
    uint32_t magic = UINT32_C(0x12340102), span;
    assert(flag == -1);
    ++alloc_calls;
    if (fail_at == alloc_calls) return NULL;
    cell = malloc(n + 16);
    assert(cell && n + 16 <= UINT32_MAX);
    span = (uint32_t)(n + 16);
    memcpy(cell, &magic, 4);
    memcpy(cell + 8, &span, 4);
    ++live_allocs;
    return cell + 16;
}
void aud_data_free(void *p)
{
    if (!p) return;
    ++frees;
    assert(live_allocs);
    --live_allocs;
    free((unsigned char *)p - 16);
}
static void *mock_calloc(size_t n, size_t s)
{
    void *p;
    if (fail_zero) return NULL;
    p = aud_data_alloc(n * s, -1);
    if (p) memset(p, 0, n * s);
    return p;
}
void *aud_object_get(void *p)
{
    struct aud_data *d = p;
    ++ref_gets;
    if (d) { assert(d->refs); ++d->refs; }
    return p;
}
void aud_object_put(void *p)
{
    struct aud_data *d = p;
    assert(d && d->refs);
    ++ref_puts;
    if (!--d->refs) d->destroy(d);
}
uint32_t aud_data_irq_save(void) { return irq_depth++; }
void aud_data_irq_restore(uint32_t n) { assert(irq_depth == n + 1); irq_depth = n; }
void aud_mutex_lock(void *p) { assert(p == &zero_mutex && !locks); ++locks; }
void aud_mutex_unlock(void *p) { assert(p == &zero_mutex && locks == 1); --locks; }
void logm_message(const char *format, ...)
{
    va_list ap;
    assert(format);
    va_start(ap, format);
    va_end(ap);
    ++messages;
}
const char *symbol_get_name(uint32_t p, int n) { (void)p; assert(!n); return "mock"; }

static void drain(void)
{
    assert(!aud_data_gc(0));
    assert(!irq_depth && !locks);
}
static struct aud_data *buffer(void)
{
    struct aud_data *d = aud_data_new(8);
    unsigned i;
    assert(d && d->size == 8 && d->refs == 1 && d->type == UINT32_C(0x64617461));
    memcpy(d->ptr, "abcdefgh", 8);
    for (i = 0; i < 8; ++i) d->metadata[i] = 100 + i;
    return d;
}
static void ownership_regions(void)
{
    struct aud_data *d = buffer(), *r, *nested;
    unsigned i;
    r = aud_data_region(d, 2, 4);
    assert(r && r->ptr == d->ptr + 2 && r->size == 4 && r->parent == d && d->refs == 2);
    for (i = 0; i < 8; ++i) assert(r->metadata[i] == d->metadata[i]);
    nested = aud_data_region(r, 1, 2);
    assert(nested && nested->parent == r && r->refs == 2 && d->refs == 2);
    aud_object_put(d);
    aud_object_put(r);
    assert(!memcmp(nested->ptr, "de", 2));
    nested->ptr[0] = 'X';
    assert(nested->parent->ptr[1] == 'X');
    aud_object_put(nested);
    drain();
    assert(!live_allocs);
}
static void splits(void)
{
    struct aud_data *d = buffer(), *a, *b, *tail;
    assert(!aud_data_split2(d, 3, &a, &b));
    assert(d->size == 8 && d->refs == 3);
    assert(a->size == 3 && b->size == 5 && a->ptr == d->ptr && b->ptr == d->ptr + 3);
    assert(!memcmp(a->ptr, "abc", 3) && !memcmp(b->ptr, "defgh", 5));
    assert(!memcmp(a->metadata, d->metadata, sizeof(d->metadata)));
    assert(!memcmp(b->metadata, d->metadata, sizeof(d->metadata)));
    aud_object_put(a); aud_object_put(b);
    a = b = d;
    assert(aud_data_split2(d, 0, &a, &b) == -1 && !a && !b);
    assert(aud_data_split2(d, 8, &a, &b) == -1 && !a && !b);
    assert(aud_data_split2(d, UINT32_MAX, &a, &b) == -1 && !a && !b);
    tail = aud_data_split(d, 5);
    assert(tail && d->size == 5 && tail->size == 3 && d->refs == 2);
    aud_object_put(d);
    assert(!memcmp(tail->ptr, "fgh", 3));
    aud_object_put(tail);
    drain();
    assert(!live_allocs);
    d = buffer();
    a = aud_data_region(d, 8, 100); /* firmware does not validate region bounds */
    assert(a && a->size == 100 && a->ptr == d->ptr + 8);
    aud_object_put(a);
    tail = aud_data_split(d, 9); /* unchecked subtraction wraps */
    assert(tail && d->size == 9 && tail->size == UINT32_MAX);
    aud_object_put(tail); aud_object_put(d);
    drain();
    assert(!live_allocs);
}
static void failures(void)
{
    struct aud_data *d = buffer(), *a, *b;
    unsigned before;
    drain();
    fail_at = alloc_calls + 1;
    assert(!aud_data_region(d, 0, 2) && d->refs == 1);
    fail_at = alloc_calls + 1;
    assert(!aud_data_split(d, 3) && d->size == 8 && d->refs == 1);
    before = alloc_calls;
    fail_at = before + 1;
    assert(aud_data_split2(d, 3, &a, &b) == -1 && !a && !b);
    assert(alloc_calls == before + 2 && d->refs == 1);
    drain();
    fail_at = alloc_calls + 2;
    assert(aud_data_split2(d, 3, &a, &b) == -1 && !a && !b && d->refs == 1);
    fail_at = 0;
    aud_object_put(d);
    drain();
    fail_at = alloc_calls + 1;
    assert(!aud_data_new(8));
    fail_at = 0;
    assert(!live_allocs && !irq_depth);
}
static void caches(void)
{
    struct aud_data *a, *b;
    unsigned before;
    a = buffer();
    aud_object_put(a);
    before = alloc_calls;
    b = aud_data_new(10);
    assert(b == a && b->refs == 1 && b->size == 10 && !b->parent && !b->owns_buffer);
    assert(alloc_calls == before && !memcmp(b->ptr, "abcdefgh", 8));
    assert(b->metadata[0] == 0 && b->metadata[1] == UINT32_MAX && b->metadata[2] == UINT32_MAX);
    aud_object_put(b);
    assert(data_caches[1].free_count == 1);
    aud_data_gc(1);
    assert(data_caches[1].free_count == 1);
    a = aud_data_new(600000);
    assert(a && !a->cache && a->size == 600000);
    before = frees;
    aud_object_put(a);
    assert(frees == before + 1);
    drain();
    assert(!live_allocs);
    a = aud_data_new(0);
    assert(a && !a->size && !a->ptr);
    aud_object_put(a);
    b = aud_data_new(0);
    assert(a == b && !b->size && b->ptr == (unsigned char *)(b + 1));
    aud_object_put(b);
    drain();
    {
        struct aud_data *v[22];
        unsigned i;
        for (i = 0; i < 22; ++i) v[i] = aud_data_new(16);
        for (i = 0; i < 22; ++i) aud_object_put(v[i]);
        assert(data_caches[1].free_count == 22);
        aud_data_gc(-1);
        assert(data_caches[1].free_count == 21);
        aud_data_gc(1);
        assert(data_caches[1].free_count == 20);
    }
    drain();
    assert(!live_allocs);
}
static void zeros(void)
{
    struct aud_data *a = aud_data_new_zero(8), *b, *c;
    unsigned i;
    assert(a && a->parent == zero_data && a->size == 8 && (a->metadata[6] & 1));
    for (i = 0; i < 8; ++i) assert(!a->ptr[i]);
    b = aud_data_new_zero(4);
    assert(b && a->ptr == b->ptr && b->parent == a->parent);
    c = aud_data_new_zero(16);
    assert(c && c->ptr != a->ptr && c->parent != a->parent);
    assert(!memcmp(a->ptr, "\0\0\0\0\0\0\0\0", 8));
    aud_object_put(a); aud_object_put(b); aud_object_put(c);
    aud_object_put(zero_data);
    zero_data = NULL; zero_size = 0; zero_ptr = NULL;
    drain();
    assert(!live_allocs);
    fail_zero = 1;
    a = aud_data_new_zero(8);
    assert(a && !a->ptr && a->size == 8 && a->parent == zero_data);
    fail_zero = 0;
    aud_object_put(a); aud_object_put(zero_data);
    zero_data = NULL; zero_size = 0; zero_ptr = NULL;
    drain();
    assert(!live_allocs);
    fail_at = alloc_calls + 1;
    assert(!aud_data_new_zero(8) && !locks);
    fail_at = 0;
    assert(!live_allocs);
    fail_at = alloc_calls + 3; /* backing header, payload, then view */
    assert(!aud_data_new_zero(8) && zero_data && zero_size == 8 && !locks);
    fail_at = 0;
    aud_object_put(zero_data);
    zero_data = NULL; zero_size = 0; zero_ptr = NULL;
    drain();
    assert(!live_allocs);
}
static void corruption_and_dump(void)
{
    struct aud_data *a = buffer();
    uint32_t bad = 0, good = UINT32_C(0x12340102);
    const char *args[] = {"", "status", "alloc"};
    assert(!check_data_corrupt(a) && !aud_data_check_corruption(a));
    memcpy((unsigned char *)a - 16, &bad, 4);
    assert(check_data_corrupt(a) == -1 && aud_data_check_corruption(NULL) == -1);
    memcpy((unsigned char *)a - 16, &good, 4);
    assert(!aud_data_cache(3, args) && messages >= 19);
    assert(!dump_aud_data_cache());
    aud_object_put(a);
    drain();
    assert(!live_allocs);
    {
        unsigned char cells[64] = {0};
        uint32_t span = 32;
        memcpy(cells, &good, 4);
        memcpy(cells + 8, &span, 4);
        aud_data_heap_boundary = (uintptr_t)cells;
        assert(check_data_corrupt(cells + 16) == -1);
        memcpy(cells + 32, &good, 4);
        assert(!check_data_corrupt(cells + 16));
        aud_data_heap_boundary = (uintptr_t)(cells + 32);
        memcpy(cells + 32, &bad, 4);
        assert(!check_data_corrupt(cells + 16));
        aud_data_heap_boundary = UINTPTR_MAX;
    }
}
int main(void)
{
    aud_data_heap_boundary = UINTPTR_MAX;
    cache_initializer();
    ownership_regions(); splits(); failures(); caches(); zeros(); corruption_and_dump();
    assert(!live_allocs && !irq_depth && !locks && ref_gets && ref_puts);
    puts("dsp2 data: ownership/region/split/cache/zero/corruption/failure tests passed");
    return 0;
}
