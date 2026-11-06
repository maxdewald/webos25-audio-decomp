// SPDX-License-Identifier: GPL-2.0-only
/* Standalone: clang -std=c11 -Wall -Wextra -Werror -pedantic
 * -fsanitize=address,undefined tools/tests/dsp1/glue_reader.c
 * -o build/scratch/glue-reader/test && build/scratch/glue-reader/test
 * Deterministic framework/library mocks; no DSP hardware is simulated. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void *test_malloc(size_t);
static void *test_calloc(size_t, size_t);
static void test_free(void *);
#define malloc test_malloc
#define calloc test_calloc
#define free test_free
#include "../../../src/dsp1/glue_reader.c"
#undef malloc
#undef calloc
#undef free

static int fail_alloc, live_alloc;
static bool fail_name, fail_data, fail_region, fail_thread, fail_port;
static void *test_malloc(size_t n)
{
    if (fail_alloc && --fail_alloc == 0)
        return NULL;
    void *p = malloc(n);
    if (p) ++live_alloc;
    return p;
}
static void *test_calloc(size_t n, size_t size)
{
    void *p = test_malloc(n * size);
    if (p) memset(p, 0, n * size);
    return p;
}
static void test_free(void *p)
{
    if (p) { --live_alloc; free(p); }
}

struct mock_data {
    struct aud_data d;
    struct aud_data *parent;
};
struct aud_thread { int unused; };
static struct aud_thread thread;
static struct aud_port ports[2];
static struct aud_data *pending[2][8];
static unsigned pending_count[2];
static unsigned char ring[128], expected[128];
static struct au_info current;
static unsigned consumed, chunk, reads, flushes, zero_flushes, syncs, pushes, events;
static int fail_read, fail_info, flush_error, reported_offset;
static bool check_copy, stop_on_sync, stop_on_wait, release_on_wait;
static bool hold_downstream;
static int expected_type;
static bool published = true;
static struct aud_starter *active;
static unsigned waits, signals, reader_updates, writer_updates, checks;
static unsigned destroyed, unrefs, removed, cleanup, created;
static int locks;
static char trace[256];
static unsigned trace_len;
static void record(char c) { assert(trace_len + 1 < sizeof(trace)); trace[trace_len++] = c; trace[trace_len] = 0; }
static void reset(void)
{
    assert(live_alloc == 0);
    memset(ports, 0, sizeof(ports));
    memset(pending_count, 0, sizeof(pending_count));
    memset(&current, 0, sizeof(current));
    memset(ring, 0, sizeof(ring));
    for (unsigned i = 0; i < 2; ++i) ports[i].number = (int)i;
    fail_alloc = 0;
    fail_name = fail_data = fail_region = fail_thread = fail_port = false;
    fail_read = fail_info = flush_error = reported_offset = 0;
    check_copy = stop_on_sync = stop_on_wait = release_on_wait = false;
    hold_downstream = false;
    published = true;
    active = NULL;
    consumed = reads = flushes = zero_flushes = syncs = pushes = events = 0;
    waits = signals = reader_updates = writer_updates = checks = 0;
    destroyed = unrefs = removed = cleanup = created = 0;
    chunk = sizeof(ring);
    trace_len = 0;
    trace[0] = 0;
    locks = 0;
}
static int get_info(int *offset, struct au_info *aui, struct adec_buf *b)
{
    (void)b;
    record('I');
    *offset = reported_offset;
    *aui = current;
    return fail_info;
}
static int get_count(struct adec_buf *b) { (void)b; return published ? 1 : 0; }
static int read_data(unsigned char **data, int *size, int *count, struct adec_buf *b)
{
    (void)b;
    assert(count == NULL);
    record('R');
    if (fail_read && (int)++reads == fail_read) return -1;
    if (!fail_read) ++reads;
    assert(*size > 0 && consumed + (unsigned)*size <= current.size);
    if ((unsigned)*size > chunk) *size = (int)chunk;
    *data = ring + consumed;
    return 0;
}
static int flush_data(int size, struct adec_buf *b)
{
    (void)b;
    record('F');
    ++flushes;
    if (!size) ++zero_flushes;
    assert(size >= 0 && consumed + (unsigned)size <= current.size);
    memset(ring + consumed, 0xa5, (size_t)size);
    consumed += (unsigned)size;
    return flush_error;
}
static void unref(struct adec_buf *b)
{
    assert(!active || destroyed);
    record('U');
    ++unrefs;
    test_free(b);
}
static struct adec_buf mock_buffer = {
    .get_au_cnt = get_count, .get_current_info = get_info,
    .read_data = read_data, .flush_data = flush_data, .unref = unref
};
static void init_starter(struct aud_starter *s)
{
    memset(s, 0, sizeof(*s));
    s->ports.next = s->ports.prev = &s->ports;
    s->lbuf = &mock_buffer;
    s->split_meta_pcm = true;
    active = s;
}

struct aud_data *aud_data_new(unsigned size)
{
    if (fail_data) return NULL;
    struct mock_data *m = test_calloc(1, sizeof(*m));
    assert(m);
    m->d.object[0] = 1;
    m->d.object[1] = 1;
    m->d.size = size;
    m->d.data = test_malloc(size ? size : 1);
    assert(m->d.data);
    return &m->d;
}
struct aud_data *aud_data_region(struct aud_data *d, unsigned offset, unsigned size)
{
    if (fail_region) return NULL;
    struct mock_data *m = test_calloc(1, sizeof(*m));
    assert(m && offset + size <= d->size);
    m->d = *d;
    m->d.object[0] = 1;
    m->d.data = d->data + offset;
    m->d.size = size;
    m->parent = aud_object_get(d);
    return &m->d;
}
void *aud_object_get(void *p)
{
    uint32_t *obj = p;
    assert(obj[0] > 0);
    ++obj[0];
    return p;
}
void aud_object_put(void *p)
{
    uint32_t *obj = p;
    assert(obj[0] > 0);
    if (--obj[0]) return;
    if (obj[1] == 1) {
        struct mock_data *m = p;
        if (m->parent) aud_object_put(m->parent);
        else test_free(m->d.data);
    }
    test_free(p);
}
struct aud_event *aud_event_deserialize(const void *data, unsigned size)
{
    record('D');
    assert(size == current.size && memcmp(data, expected, size) == 0);
    if (*(const unsigned char *)data == 0xff) return NULL;
    struct aud_event *e = test_calloc(1, sizeof(*e));
    assert(e);
    e->object[0] = 1;
    e->object[1] = 2;
    e->type = *(const unsigned char *)data;
    return e;
}
int aud_module_event(struct mod_glue *g, struct aud_port *p, struct aud_event *e)
{
    assert(p == NULL && consumed == current.size && locks == 1);
    assert(g == ports[events % 2 == 0 ? 1 : 0].module);
    assert(e->type == expected[0]);
    record('E');
    ++events;
    aud_object_put(e);
    return -1; /* Caller ignores downstream returns. */
}
int aud_port_push_data(struct aud_port *p, struct aud_data *d)
{
    assert(consumed == current.size && locks == 1);
    assert(pending_count[p->number] < 8);
    if (check_copy) {
        assert(d->type == expected_type);
        assert(d->size == current.size);
        assert(memcmp(d->data, expected, d->size) == 0);
        assert(d->data != ring);
        assert(d->index == -7 && d->timestamp == 1234);
        assert(d->timestamp64 == UINT64_C(0x123456789abcdef0));
    }
    pending[p->number][pending_count[p->number]++] = d;
    record(p->number ? 'B' : 'A');
    ++pushes;
    return -1;
}
int aud_port_sync(struct aud_port *p)
{
    assert(locks == 1 && consumed == current.size);
    record('S');
    ++syncs;
    if (!hold_downstream) {
        for (unsigned i = 0; i < pending_count[p->number]; ++i)
            aud_object_put(pending[p->number][i]);
        pending_count[p->number] = 0;
    }
    if (stop_on_sync) active->stop = 1;
    return -1;
}
void aud_mutex_init(struct aud_mutex *m) { memset(m, 0, sizeof(*m)); }
void aud_mutex_lock(struct aud_mutex *m) { (void)m; ++locks; }
void aud_mutex_unlock(struct aud_mutex *m) { (void)m; assert(locks > 0); --locks; }
void aud_cond_init(struct aud_cond *c) { memset(c, 0, sizeof(*c)); }
void aud_cond_signal(struct aud_cond *c) { (void)c; record('N'); ++signals; }
void aud_cond_wait(struct aud_cond *c, struct aud_mutex *m)
{
    (void)c; (void)m;
    assert(locks == 1);
    ++waits;
    if (release_on_wait) published = true;
    if (stop_on_wait) active->stop = 1;
}
char *audutil_strdup_vprintf(const char *fmt, va_list ap)
{
    if (fail_name) return NULL;
    char *s = test_malloc(80);
    assert(s);
    (void)vsnprintf(s, 80, fmt, ap);
    return s;
}
struct aud_thread *aud_thread_create(const char *name, unsigned stack, int pri,
    int (*fn)(void *), void *arg)
{
    assert(name && stack == 0x20000 && pri == 10 && fn == starter_thread && arg);
    ++created;
    return fail_thread ? NULL : &thread;
}
void aud_thread_destroy(struct aud_thread *t)
{
    assert(t == &thread || t == NULL);
    if (active) {
        assert(active->stop);
        for (unsigned i = 0; i < 2; ++i) assert(!ports[i].starter);
    }
    record('T');
    ++destroyed;
}
void Cache_Invalidate(void *p, unsigned n, struct adec_buf *b) { (void)p; (void)n; (void)b; }
void Cache_Writeback(void *p, unsigned n, struct adec_buf *b) { (void)p; (void)n; (void)b; }
unsigned char *Mem_Translator(unsigned char *p, struct adec_buf *b) { (void)b; return p; }
unsigned CommonBuffer_GetStructSize(unsigned n) { assert(n == 2); return 96; }
unsigned InterBufReader_GetStructSize(void) { return sizeof(struct adec_buf); }
unsigned InterBufWriter_GetStructSize(void) { return sizeof(struct adec_buf); }
int InterBufReader_Create(const struct processor_adapter *a, void *common, unsigned size,
    unsigned max, unsigned cnt, void *data, unsigned uid, void *imc, struct adec_buf *b)
{
    assert(a->invalidate == Cache_Invalidate && a->writeback == Cache_Writeback);
    assert(a->translate == Mem_Translator);
    assert((uintptr_t)common == 0x68001000 && (uintptr_t)data == 0x68001060);
    assert(size == 262144 && max == 32768 && cnt == 2 && uid == 0x200);
    assert(imc == (void *)(uintptr_t)0x123);
    *b = mock_buffer;
    return -1; /* Shipped CreateBuffer still returns the constructed pointer. */
}
int InterBufWriter_Create(const struct processor_adapter *a, void *common, unsigned size,
    unsigned max, unsigned cnt, void *data, unsigned uid, void *imc, struct adec_buf *b)
{
    return InterBufReader_Create(a, common, size, max, cnt, data, uid, imc, b);
}
void InterBufReader_UpdateStatus(struct adec_buf *b, const struct buffer_status *s)
{
    assert(b && s->write_offset == 99); record('W'); ++reader_updates;
}
void InterBufWriter_UpdateStatus(struct adec_buf *b, const struct buffer_status *s)
{
    assert(b && s->read_offset == 55); record('V'); ++writer_updates;
}
int aud_module_check_legacy_output(struct mod_glue *g, int n)
{
    assert(g && n == 0); record('C'); ++checks; return -1;
}
struct aud_port *aud_module_get_output_port(struct mod_glue *g, int n)
{
    (void)g; assert(n == 0); return ports[0].module ? &ports[0] : NULL;
}
struct aud_port *aud_module_new_output_port(struct mod_glue *g, int n)
{
    assert(n == 0);
    if (fail_port) return NULL;
    ports[0].module = g;
    return &ports[0];
}
void aud_module_del_output_port(struct mod_glue *g, int n)
{
    (void)g; assert(n == 0 && unrefs); record('O'); ++removed;
    ports[0].module = NULL;
}
void aud_data_adapter_cleanup(void *p) { (void)p; record('K'); ++cleanup; }

static void payload(unsigned size, int type)
{
    current.size = size;
    current.data_type = type;
    current.index = -7;
    current.timestamp = 1234;
    current.timestamp64 = UINT64_C(0x123456789abcdef0);
    for (unsigned i = 0; i < size; ++i) ring[i] = (unsigned char)(i + 1);
    memcpy(expected, ring, size);
}
static void attach(struct aud_starter *s)
{
    static struct mod_glue modules[2];
    for (unsigned i = 0; i < 2; ++i) {
        ports[i].module = &modules[i];
        aud_starter_add_port(s, &ports[i]);
    }
}
static void test_mapping_copy_order(void)
{
    const int types[] = {0, 1, 2, 3, 5, 77, -1};
    const int mapped[] = {0, 1, 2, 3, 4, 0, 0};
    for (unsigned i = 0; i < sizeof(types) / sizeof(types[0]); ++i) {
        reset();
        struct aud_starter s;
        init_starter(&s);
        attach(&s);
        s.split_meta_pcm = false;
        payload(17, types[i]);
        check_copy = true;
        expected_type = mapped[i];
        chunk = 6;
        reported_offset = 3;
        assert(process_buf(&s) == 0);
        assert(pushes == 2 && syncs == 2 && reads == 3 && flushes == 3);
        assert(strcmp(trace, "IRFRFRFBSAS") == 0);
        assert(live_alloc == 0 && locks == 0);
    }
}
static void test_retained_ownership(void)
{
    reset();
    struct aud_starter s;
    init_starter(&s);
    attach(&s);
    payload(8, 1);
    hold_downstream = true;
    assert(process_buf(&s) == 0 && live_alloc == 2);
    assert(pending[0][0] == pending[1][0]);
    assert(pending[0][0]->object[0] == 2);
    assert(memcmp(pending[0][0]->data, expected, 8) == 0);
    aud_object_put(pending[0][0]);
    aud_object_put(pending[1][0]);
    pending_count[0] = pending_count[1] = 0;
    assert(!live_alloc);

    consumed = 0;
    payload(16, 5);
    const uint32_t words[] = {2, 3, 0x00332211, 0};
    memcpy(ring, words, sizeof(words));
    assert(process_buf(&s) == 0 && live_alloc == 4);
    struct mock_data *a = (void *)pending[0][0];
    struct mock_data *b = (void *)pending[1][0];
    assert(a != b && a->parent == b->parent);
    assert(a->parent->object[0] == 2 && a->d.type == 2 && a->d.size == 3);
    assert(memcmp(a->d.data, "\x11\x22\x33", 3) == 0);
    aud_object_put(a);
    assert(live_alloc == 3);
    aud_object_put(b);
    pending_count[0] = pending_count[1] = 0;
    assert(!live_alloc);
}
static void test_events(void)
{
    for (unsigned fragment = 0; fragment < 2; ++fragment) {
        reset();
        struct aud_starter s;
        init_starter(&s);
        attach(&s);
        payload(12, 4);
        chunk = fragment ? 5 : 128;
        assert(process_buf(&s) == 0 && events == 2 && syncs == 0);
        assert(strcmp(trace, fragment ? "IRFRFRFDEE" : "IRDFEE") == 0);
        assert(live_alloc == 0);
        /* The following data AU cannot overtake synchronous FIRST delivery. */
        consumed = 0;
        payload(4, 1);
        check_copy = true;
        expected_type = 1;
        assert(process_buf(&s) == 0 && pushes == 2);
    }
    reset();
    struct aud_starter s;
    init_starter(&s);
    payload(4, 4);
    ring[0] = expected[0] = 0xff;
    assert(process_buf(&s) == 0 && consumed == 4 && !events && !live_alloc);
}
static void test_errors(void)
{
    reset();
    struct aud_starter s;
    init_starter(&s);
    attach(&s);
    payload(8, 1);
    fail_info = -1;
    assert(process_buf(&s) == -1 && !reads && !flushes);
    fail_info = 0;
    fail_data = true;
    assert(process_buf(&s) == 0 && !reads && !flushes && !pushes);
    fail_data = false;
    fail_read = 2;
    chunk = 3;
    assert(process_buf(&s) == 0 && consumed == 3 && !pushes && !live_alloc);
    consumed = reads = 0;
    fail_read = 1;
    current.data_type = 4;
    assert(process_buf(&s) == 0 && !consumed && !events);
    fail_read = 0;
    current.size = 0;
    assert(process_buf(&s) == 0 && zero_flushes == 1);
    consumed = 0;
    payload(8, 1);
    flush_error = -9;
    assert(process_buf(&s) == 0 && consumed == 8 && pushes == 2);
    assert(live_alloc == 0);
}
static void test_mixed(void)
{
    reset();
    struct aud_starter s;
    init_starter(&s);
    attach(&s);
    payload(32, 5);
    const uint32_t words[] = {2, 3, 0x00332211, 0, 0, 5, 0x77665544, 0x88};
    memcpy(ring, words, sizeof(words));
    assert(process_buf(&s) == 0 && pushes == 4 && syncs == 2 && !live_alloc);
    consumed = 0;
    payload(9, 5);
    uint32_t oversized = 100;
    memcpy(ring + 4, &oversized, 4);
    assert(process_buf(&s) == 0 && pushes == 4 && syncs == 4 && !live_alloc);
    consumed = 0;
    payload(3, 5);
    assert(process_buf(&s) == 0 && pushes == 4 && syncs == 6 && !live_alloc);
}
static void test_thread_and_links(void)
{
    reset();
    struct aud_starter s;
    init_starter(&s);
    attach(&s);
    aud_starter_del_port(&s, &ports[1]);
    assert(!ports[1].starter && s.ports.prev == &ports[0].starter_link);
    aud_starter_del_port(&s, &ports[0]);
    aud_starter_add(&s, &ports[0].starter_link);
    assert(s.ports.prev == &ports[0].starter_link);
    aud_starter_del_port(&s, &ports[0]);
    aud_starter_add_port(&s, &ports[0]);
    assert(aud_starter_get_lbuf(&s) == &mock_buffer);
    published = false;
    stop_on_wait = true;
    assert(starter_thread(&s) == 0 && waits == 1 && !reads);
    s.stop = 0;
    waits = 0;
    stop_on_wait = false;
    release_on_wait = true;
    stop_on_sync = true;
    payload(4, 1);
    assert(starter_thread(&s) == 0 && waits == 1 && syncs == 1);
    assert(locks == 0 && !live_alloc);
}
static struct shared_buffer_cmd cmd = {0x10001000, 262144, 0x200, 32768, 2};
static void test_setup_status_teardown(void)
{
    reset();
    struct mod_glue g = {.uid = 0x123, .imc = (void *)(uintptr_t)0x123};
    assert(set_shbuf_pr(&g, &cmd) == 0);
    active = g.starter;
    assert(strcmp(active->name, "starter_00000123") == 0);
    assert(g.shared_buf->offset == cmd.offset && g.shbuf_offset == cmd.offset);
    assert(g.cowork_uid == cmd.cowork_uid && ports[0].starter == active);
    struct remote_status_cmd status = {.evt_type = 3, .status = {
        .read_offset = 55, .write_offset = 99, .read_index = 1, .write_index = 2}};
    trace_len = 0; trace[0] = 0;
    assert(SetSharedBufferStatus(&g, &status) == 0);
    assert(strcmp(trace, "WN") == 0 && reader_updates == 1 && signals == 1);
    status.evt_type = 2;
    assert(SetSharedBufferStatus(&g, &status) == 0 && checks == 1 && writer_updates == 1);
    assert(strcmp(trace, "WNVC") == 0);
    status.evt_type = 77;
    assert(SetSharedBufferStatus(&g, &status) == 0 && signals == 1);
    assert(Glue_DeletingPort(&g, &ports[0]) == 0 && active->stop);
    trace_len = 0; trace[0] = 0;
    Glue_Destructor(&g);
    active = NULL;
    assert(strcmp(trace, "NTUOK") == 0);
    assert(destroyed == 1 && unrefs == 1 && removed == 1 && cleanup == 2);
    assert(live_alloc == 0 && locks == 0);

    reset();
    g = (struct mod_glue){.imc = (void *)(uintptr_t)0x123};
    fail_name = true;
    assert(set_shbuf_pr(&g, &cmd) == -1 && !g.shared_buf && !g.starter);
    assert(unrefs == 1 && live_alloc == 0);
    reset();
    fail_alloc = 1;
    assert(set_shbuf_pr(&g, &cmd) == -1 && !g.shared_buf && !live_alloc);
    reset();
    fail_alloc = 2;
    assert(set_shbuf_pr(&g, &cmd) == -1 && !g.shared_buf && unrefs == 1 && !live_alloc);
    reset();
    fail_thread = fail_port = true;
    assert(set_shbuf_pr(&g, &cmd) == 0 && g.starter && !g.starter->thread);
    active = g.starter;
    assert(active->ports.next == &active->ports && !ports[0].module);
    Glue_Destructor(&g);
    active = NULL;
    assert(!live_alloc && !removed);
    reset();
    g.shared_buf = CreateBuffer(&g, &cmd, 0);
    assert(g.shared_buf);
    g.shared_buf->unref(g.shared_buf);
    assert(!live_alloc);
}
int main(void)
{
    test_mapping_copy_order();
    test_retained_ownership();
    test_events();
    test_errors();
    test_mixed();
    test_thread_and_links();
    test_setup_status_teardown();
    puts("dsp1 glue_reader: mapping, copy/ownership, FIRST ordering, consumption-before-sync, failures, mixed, waits, status, setup, teardown passed");
    return 0;
}
