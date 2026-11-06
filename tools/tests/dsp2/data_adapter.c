/* Standalone: cc -std=c11 -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined tools/tests/dsp2/data_adapter.c -o build/scratch/data-adapter-test && build/scratch/data-adapter-test */
#include <stdlib.h>
static void *test_malloc(size_t);
static void *test_calloc(size_t, size_t);
static void test_free(void *);
#define malloc test_malloc
#define calloc test_calloc
#define free test_free
#include "../../../src/dsp2/data_adapter.c"
#undef malloc
#undef calloc
#undef free
#include <assert.h>
#include <stdio.h>

static unsigned allocations, frees, live_objects, ref_gets, ref_puts;
static int fail_malloc, fail_calloc, fail_new, fail_region, fail_split;
struct mock_data {
    struct aud_data data;
    unsigned refs;
    struct mock_data *parent;
    int owns;
};
static struct mock_data *mock(struct aud_data *d) { return (struct mock_data *)d; }
static void *test_malloc(size_t n)
{
    void *p = fail_malloc ? NULL : malloc(n);
    if (p) ++allocations;
    return p;
}
static void *test_calloc(size_t n, size_t s)
{
    void *p = fail_calloc ? NULL : calloc(n, s);
    if (p) ++allocations;
    return p;
}
static void test_free(void *p)
{
    assert(p);
    ++frees;
    free(p);
}
void *aud_object_get(void *p)
{
    assert(p && mock(p)->refs);
    ++mock(p)->refs;
    ++ref_gets;
    return p;
}
void aud_object_put(void *p)
{
    struct mock_data *m = p;
    assert(m && m->refs);
    ++ref_puts;
    if (--m->refs) return;
    if (m->parent) aud_object_put(m->parent);
    if (m->owns) free(m->data.ptr);
    --live_objects;
    free(m);
}
struct aud_data *aud_data_new(uint32_t n)
{
    struct mock_data *m;
    if (fail_new) return NULL;
    m = calloc(1, sizeof(*m));
    assert(m);
    m->refs = 1;
    if (n) {
        m->data.ptr = malloc(n);
        assert(m->data.ptr);
        m->owns = 1;
    }
    m->data.size = n;
    ++live_objects;
    return &m->data;
}
struct aud_data *aud_data_region(struct aud_data *d, uint32_t off, uint32_t n)
{
    struct aud_data *r;
    if (fail_region) return NULL;
    assert(off <= d->size && n <= d->size - off);
    r = aud_data_new(0);
    if (!r) return NULL;
    memcpy(r->metadata, d->metadata, sizeof(r->metadata));
    r->ptr = d->ptr + off;
    r->size = n;
    mock(r)->parent = aud_object_get(d);
    return r;
}
int aud_data_split2(struct aud_data *d, uint32_t off,
                    struct aud_data **first, struct aud_data **rest)
{
    *first = *rest = NULL;
    if (fail_split) return -1;
    assert(off && off < d->size);
    *first = aud_data_region(d, 0, off);
    *rest = aud_data_region(d, off, d->size - off);
    if (*first && *rest) return 0;
    if (*first) aud_object_put(*first);
    if (*rest) aud_object_put(*rest);
    *first = *rest = NULL;
    return -1;
}
static struct aud_data *data(const char *s)
{
    struct aud_data *d = aud_data_new((uint32_t)strlen(s));
    assert(d);
    if (d->size) memcpy(d->ptr, s, d->size);
    d->metadata[0] = 73;
    return d;
}
static void expect(struct aud_data_adapter *q, const char *s)
{
    unsigned char out[128] = {0};
    assert(q->total - q->offset == strlen(s));
    assert(aud_data_adapter_copy(q, out, (uint32_t)strlen(s)) == 0);
    assert(memcmp(out, s, strlen(s)) == 0);
}
static void finish(struct aud_data_adapter *q)
{
    aud_data_adapter_cleanup(q);
    assert(!live_objects);
    assert(allocations == frees);
    assert(!adapter_first(q));
    assert(q->idle.next == &q->idle && q->idle.prev == &q->idle);
}
static void test_fifo(void)
{
    struct aud_data_adapter q;
    struct aud_data *a = data("abc"), *b = data("defg"), *d;
    unsigned before_gets = ref_gets;
    aud_data_adapter_init(&q);
    assert(!q.total && !q.offset && !q.mapped);
    assert(!aud_data_adapter_peep(&q));
    assert(!aud_data_adapter_dequeue(&q));
    assert(!aud_data_adapter_get_data(&q, 1));
    assert(!aud_data_adapter_map(&q, 1));
    aud_data_adapter_push(&q, NULL);
    aud_data_adapter_push_front(&q, NULL);
    aud_data_adapter_push(&q, a);
    aud_data_adapter_push(&q, b);
    assert(ref_gets == before_gets && mock(a)->refs == 1);
    assert(aud_data_adapter_peep(&q) == a);
    expect(&q, "abcdefg");
    aud_data_adapter_flush(&q, 2);
    assert(q.offset == 2 && q.total == 7);
    assert(aud_data_adapter_peep(&q) == a);
    expect(&q, "cdefg");
    d = aud_data_adapter_dequeue(&q);
    assert(d != a && d->size == 1 && d->ptr[0] == 'c');
    assert(d->metadata[0] == 73 && q.offset == 0 && q.total == 4);
    aud_object_put(d);
    assert(aud_data_adapter_dequeue(&q) == b);
    aud_object_put(b);
    expect(&q, "");
    finish(&q);
}
static void test_front_reuse_and_offset(void)
{
    struct aud_data_adapter q;
    struct adapter_holder *old;
    unsigned count;
    aud_data_adapter_init(&q);
    aud_data_adapter_push(&q, data("abcd"));
    old = adapter_first(&q);
    aud_data_adapter_flush(&q, 4);
    count = allocations;
    fail_calloc = 1;
    aud_data_adapter_push(&q, data("efgh"));
    fail_calloc = 0;
    assert(allocations == count && adapter_first(&q) == old);
    aud_data_adapter_flush(&q, 1);
    aud_data_adapter_push_front(&q, data("WXYZ"));
    assert(q.offset == 1 && q.total == 8);
    expect(&q, "XYZefgh");
    aud_data_adapter_flush(&q, 100);
    expect(&q, "");
    finish(&q);
}
static void test_copy_map(void)
{
    struct aud_data_adapter q;
    struct aud_data *a = data("abcd");
    unsigned char out[12];
    void *p, *old;
    unsigned count;
    aud_data_adapter_init(&q);
    aud_data_adapter_push(&q, a);
    aud_data_adapter_push(&q, data("efgh"));
    assert(!aud_data_adapter_map(&q, 0));
    assert(aud_data_adapter_map(&q, 4) == a->ptr);
    assert(!q.mapped);
    aud_data_adapter_unmap(&q, a->ptr);
    aud_data_adapter_flush(&q, 1);
    assert(aud_data_adapter_map(&q, 2) == a->ptr + 1);
    memset(out, '?', sizeof(out));
    assert(aud_data_adapter_copy(&q, out, 9) == -1);
    assert(memcmp(out, "bcdefgh??", 9) == 0);
    p = aud_data_adapter_map(&q, 7);
    assert(p == q.mapped && memcmp(p, "bcdefgh", 7) == 0);
    old = p;
    aud_data_adapter_unmap(&q, a->ptr);
    assert(q.mapped == old);
    assert(!aud_data_adapter_map(&q, 0) && q.mapped == old);
    count = frees;
    assert(aud_data_adapter_map(&q, 2) == a->ptr + 1);
    assert(!q.mapped && frees == count + 1);
    fail_malloc = 1;
    assert(!aud_data_adapter_map(&q, 7));
    fail_malloc = 0;
    assert(!aud_data_adapter_map(&q, 8) && !q.mapped);
    p = aud_data_adapter_map(&q, 6);
    assert(p);
    aud_data_adapter_flush(&q, 100);
    assert(!aud_data_adapter_map(&q, 1) && q.mapped == p);
    aud_data_adapter_cleanup(&q);
    assert(q.mapped == p); /* cleanup is not unmap */
    aud_data_adapter_unmap(&q, p);
    finish(&q);
}
static void test_get(void)
{
    struct aud_data_adapter q;
    struct aud_data *a = data("abcdefgh"), *d, *first;
    unsigned count;
    aud_data_adapter_init(&q);
    aud_data_adapter_push(&q, a);
    assert(!aud_data_adapter_get_data(&q, 0));
    d = aud_data_adapter_get_data(&q, 8);
    assert(d == a && mock(a)->refs == 2);
    aud_object_put(d);
    d = aud_data_adapter_get_data(&q, 3);
    assert(d != a && d->size == 3 && mock(d)->refs == 2);
    assert(aud_data_adapter_peep(&q) == d);
    expect(&q, "abcdefgh");
    first = aud_data_adapter_dequeue(&q);
    assert(first == d && mock(d)->refs == 2);
    aud_object_put(first);
    aud_object_put(d);
    aud_data_adapter_flush(&q, 1);
    d = aud_data_adapter_get_data(&q, 2);
    assert(d && d->size == 2 && memcmp(d->ptr, "ef", 2) == 0);
    assert(q.offset == 0 && q.total == 4 && d->metadata[0] == 73);
    aud_object_put(d);
    expect(&q, "efgh");
    d = aud_data_adapter_get_data(&q, 4);
    assert(d && d->size == 4 && memcmp(d->ptr, "efgh", 4) == 0);
    assert(d != aud_data_adapter_peep(&q)); /* spans prefix/rest */
    aud_object_put(d);
    count = live_objects;
    assert(!aud_data_adapter_get_data(&q, 5));
    assert(live_objects == count);
    fail_new = 1;
    assert(!aud_data_adapter_get_data(&q, 4));
    fail_new = 0;
    finish(&q);
}
static void test_failures(void)
{
    struct aud_data_adapter q;
    struct aud_data *a = data("abcd"), *lost;
    unsigned count;
    aud_data_adapter_init(&q);
    fail_calloc = 1;
    aud_data_adapter_push(&q, a);
    aud_data_adapter_push_front(&q, a);
    assert(!q.total && !adapter_first(&q) && mock(a)->refs == 1);
    fail_calloc = 0;
    aud_data_adapter_push(&q, a);
    fail_calloc = 1;
    assert(!aud_data_adapter_get_data(&q, 2));
    fail_calloc = 0;
    assert(aud_data_adapter_peep(&q) == a);
    count = allocations;
    fail_split = 1;
    assert(!aud_data_adapter_get_data(&q, 2));
    assert(allocations == count + 1 && q.idle.next != &q.idle);
    fail_split = 0;
    aud_data_adapter_flush(&q, 1);
    fail_region = 1;
    assert(!aud_data_adapter_dequeue(&q));
    assert(q.offset == 1 && q.total == 4 && aud_data_adapter_peep(&q) == a);
    lost = aud_object_get(a); /* preserve old object to inspect destructive failure */
    assert(!aud_data_adapter_get_data(&q, 2));
    assert(!adapter_first(&q)->data && !q.offset && q.total == 3);
    fail_region = 0;
    adapter_first(&q)->data = aud_data_region(lost, 1, 3); /* caller recovery */
    aud_object_put(lost);
    expect(&q, "bcd");
    finish(&q);
}
static void test_zero_and_overflow(void)
{
    struct aud_data_adapter q;
    struct aud_data *z = data("");
    aud_data_adapter_init(&q);
    aud_data_adapter_push(&q, z);
    aud_data_adapter_flush(&q, 0);
    aud_data_adapter_cleanup(&q);
    assert(aud_data_adapter_peep(&q) == z); /* exact firmware zero-flush */
    assert(aud_data_adapter_dequeue(&q) == z);
    aud_object_put(z);
    finish(&q);
    aud_data_adapter_init(&q);
    aud_data_adapter_push(&q, data("abc"));
    aud_data_adapter_flush(&q, 2);
    aud_data_adapter_flush(&q, UINT32_MAX); /* wraps 2+UINT32_MAX to 1 */
    assert(q.offset == 1 && q.total == 3);
    expect(&q, "bc");
    finish(&q);
}
static uint32_t random_state = 1;
static uint32_t next_random(void)
{
    random_state = random_state * UINT32_C(1664525) + UINT32_C(1013904223);
    return random_state;
}
static void test_model(void)
{
    struct aud_data_adapter q;
    char model[128] = "";
    unsigned n = 0, i;
    aud_data_adapter_init(&q);
    for (i = 0; i < 2000; ++i) {
        unsigned op = next_random() % 5;
        if (!n || (op < 2 && n < 100)) {
            char s[5];
            unsigned len = 1 + next_random() % 4, j;
            for (j = 0; j < len; ++j) s[j] = (char)('A' + next_random() % 26);
            s[len] = 0;
            aud_data_adapter_push(&q, data(s));
            memcpy(model + n, s, len + 1);
            n += len;
        } else if (op == 2) {
            unsigned len = 1 + next_random() % n;
            aud_data_adapter_flush(&q, len);
            memmove(model, model + len, n - len + 1);
            n -= len;
        } else if (op == 3) {
            unsigned len = 1 + next_random() % n;
            struct aud_data *d = aud_data_adapter_get_data(&q, len);
            assert(d && d->size == len && memcmp(d->ptr, model, len) == 0);
            aud_object_put(d);
        } else {
            struct aud_data *d = aud_data_adapter_dequeue(&q);
            unsigned len = d->size;
            assert(len <= n && memcmp(d->ptr, model, len) == 0);
            aud_object_put(d);
            memmove(model, model + len, n - len + 1);
            n -= len;
        }
        expect(&q, model);
    }
    finish(&q);
}
int main(void)
{
    test_fifo();
    test_front_reuse_and_offset();
    test_copy_map();
    test_get();
    test_failures();
    test_zero_and_overflow();
    test_model();
    printf("data_adapter: 7 groups + 2000 model steps passed (%u gets, %u puts)\n", ref_gets, ref_puts);
    return 0;
}
