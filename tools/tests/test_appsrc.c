/* Recovered appsrc + libaa queue/sync bodies;
 * peer callbacks, allocator failures and legacy ring progress are host mocks. */
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libaabase/appsrc.c"
#include "libaa/buffers.h"

unsigned int logmm_dummy_mask;
bool aud_module_sync_at_push;
struct logmm_module logmm_module_commod = {.mask = &logmm_dummy_mask};
static struct aud_module_impl *registered;
static struct aud_appsrc src;
static struct aud_port *output;
static struct aud_event first;
static struct aud_data *retained;
static unsigned destroyed, waits, broadcasts, free_bytes, writes;
static unsigned seen, bypass_calls, expected_wait_seen;
static int order[32];
static bool no_output, fail_port, fail_alloc, mutate, retain, flush_on_wait;
static bool wake_without_progress, full_after_data;
static struct aud_data *expected_shared;

int foreach_sync(void *, void *);
int foreach_down_peer_push_data(void *, void *);
int foreach_down_peer_push_event(void *, void *);
int aud_port_sync_locked(struct aud_port *);
int add_pending_obj_locked(struct aud_port *, struct aud_object *);

void ModuleRegistry_Register(struct aud_module_impl *impl) { registered = impl; }
int logmm_printl(struct logmm_module *m, int level, const char *fn, int line,
                 const char *format, ...) { return 0; }
int aud_module_event(struct aud_module *m, struct aud_port *p, struct aud_event *e)
{ assert(!"unexpected upward event"); return -1; }
int aud_module_default_command(struct aud_module *m, unsigned cmd, void *p, unsigned n)
{ return -17; }
int aud_module_default_event(struct aud_module *m, struct aud_port *p, struct aud_event *e)
{ aud_object_put(&e->object); return -18; }
struct aud_object *aud_object_get(struct aud_object *o) { assert(o->user); ++o->user; return o; }
void aud_object_put(struct aud_object *o)
{ assert(o->user); if (!--o->user) ++destroyed; }
struct aud_event *aud_event_new_first(void) { assert(!first.object.user); first.object.user = 1; return &first; }
const char *aud_event_name(struct aud_event *e) { return "mock event"; }
void *aud_event_serialize(struct aud_event *e, unsigned *size)
{ *size = 1; return calloc(1, *size); }
unsigned long long aud_system_cycle(void) { return 123; }
void aud_starter_del_port(struct aud_starter *s, struct aud_port *p) { assert(!"unexpected starter"); }
void aud_module_unblock_locked(struct aud_module *m) { m->block_module = false; }
struct aud_port *aud_module_get_output(struct aud_module *m, unsigned n)
{
    assert(m == &src.module && n == 0);
    if (no_output) return NULL;
    pthread_mutex_lock(&output->module_node.lock);
    return output;
}
struct aud_port *aud_module_new_output(struct aud_module *m, unsigned n)
{ return fail_port ? NULL : aud_module_get_output(m, n); }
int aud_link_foreach_node(struct aud_link_root *root, void *arg,
                          int (*cb)(void *, void *))
{
    assert(root == &output->down_peer_root || root == &src.module.output_port_root);
    if (root == &src.module.output_port_root) {
        pthread_mutex_lock(&output->module_node.lock);
        return cb(arg, output);
    }
    return cb(arg, NULL);
}
int foreach_down_peer_push_data(void *arg, void *unused)
{
    struct aud_data *d = arg;
    assert(seen < 32);
    order[seen++] = 100 + d->info.index;
    if (expected_shared) assert(d == expected_shared);
    if (mutate) {
        d->info.type = type_es;
        d->info.timestamp = 77;
        d->info.timestamp64 = 88;
        d->data[0] = 0xa5;
    }
    if (retain) {
        assert(!retained);
        retained = (struct aud_data *)aud_object_get(&d->object);
    }
    return -23; /* ignored by recovered sync */
}
int foreach_down_peer_push_event(void *arg, void *unused)
{
    struct aud_event *e = arg;
    assert(seen < 32);
    order[seen++] = e->type;
    return -29;
}
static AUDIO_DATA_TYPE_T legacy_data_type(struct aud_port *p, struct aud_data *d,
                                         ADEC_AU_INFO_T *info)
{ return (AUDIO_DATA_TYPE_T)d->info.type; }
static void *mock_realloc(void *ptr, size_t size)
{ return fail_alloc ? NULL : realloc(ptr, size); }
static int mock_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    assert(cond == &output->check_free_cond && mutex == &output->check_free_mutex);
    assert(pthread_mutex_trylock(mutex) == EBUSY);
    assert(pthread_mutex_trylock(&output->stream_lock) == EBUSY);
    assert(bypass_calls == 0); /* later producer call cannot have happened */
    assert(seen == expected_wait_seen && order[0] == 101);
    for (unsigned index = 0; index < output->pending_obj_num; ++index)
        assert(output->pending_objs[index]->user > 0);
    ++waits;
    if (flush_on_wait) output->flushing = 1;
    else if (!wake_without_progress || waits > 1) free_bytes = INT_MAX;
    return 0;
}
static int mock_broadcast(pthread_cond_t *cond) { ++broadcasts; return 0; }
#define DIR_STR(port) (AUD_PORT_IS_OUTPUT(port) ? "out" : "in")
#define realloc mock_realloc
#define pthread_cond_wait mock_wait
#define pthread_cond_broadcast mock_broadcast
// @extract src/libaa/module.c add_pending_obj_locked aud_module_push_data aud_module_sync aud_port_sync_locked foreach_sync foreach_push_event aud_module_push_event del_output_port
#undef realloc
#undef pthread_cond_wait
#undef pthread_cond_broadcast

static struct aud_data data(unsigned size, int index, unsigned char *bytes)
{
    struct aud_data d = {0};
    d.object.type = AUD_OBJECT_TYPE_DATA;
    d.object.user = 1;
    d.info.type = type_pcm;
    d.info.index = index;
    d.info.timestamp = 11;
    d.info.timestamp64 = 22;
    d.size = size;
    d.data = bytes;
    return d;
}
static struct aud_event event(enum aud_event_type type, bool sync)
{
    struct aud_event e = {0};
    e.object.type = AUD_OBJECT_TYPE_EVENT;
    e.object.user = 1;
    e.type = type;
    e.prop = AUD_EVENT_PROP_DOWNWARD | (sync ? AUD_EVENT_PROP_SYNC : 0);
    return e;
}
static void setup(void)
{
    memset(&src, 0, sizeof(src));
    first = event(AUD_EVENT_TYPE_FIRST_AU, true);
    output = calloc(1, sizeof(*output));
    assert(output);
    output->module = &src.module;
    output->flag = AUD_PORT_OUTPUT;
    output->current_first = &first;
    pthread_mutex_init(&output->stream_lock, NULL);
    pthread_mutex_init(&output->module_node.lock, NULL);
    pthread_mutex_init(&output->async_down_lock, NULL);
    pthread_mutex_init(&output->async_up_lock, NULL);
    pthread_mutex_init(&output->check_free_mutex, NULL);
    pthread_cond_init(&output->check_free_cond, NULL);
    src.module.impl = registered;
    src.module.name = "appsrc_mock";
    destroyed = waits = broadcasts = writes = seen = bypass_calls = 0;
    expected_wait_seen = 1;
    no_output = fail_port = fail_alloc = mutate = retain = flush_on_wait = false;
    wake_without_progress = full_after_data = false;
    expected_shared = retained = NULL;
    aud_module_sync_at_push = false;
}
static void teardown(void)
{
    assert(!retained);
    del_output_port(output);
    output = NULL;
}
static int get_free(ADEC_BUF_T *b) { return (int)free_bytes; }
static int insert(unsigned char *p, int size, ADEC_AU_INFO_T *i, ADEC_BUF_T *b)
{
    ++writes;
    if (full_after_data && i->dataType != AUDIO_DATA_TYPE_EVENT) free_bytes = 0;
    return -1;
}

static void test_fifo_and_shared_reference(void)
{
    unsigned char bytes[] = {1, 2, 3, 4};
    setup();
    struct aud_data a = data(4, 1, bytes), b = data(4, 2, bytes);
    struct aud_event e = event(AUD_EVENT_TYPE_CUSTOM, true);
    assert(module_appsrc_push_data(&src.module, &a, false) == 0);
    assert(module_appsrc_push_data(&src.module, &b, false) == 0);
    assert(!seen && output->pending_obj_num == 2 && a.object.user == 1);
    assert(module_appsrc_push_event(&src.module, &e) == 0);
    assert(seen == 3 && order[0] == 101 && order[1] == 102 && order[2] == (int)e.type);
    assert(!a.object.user && !b.object.user && !e.object.user);
    assert(!output->pending_obj_num && src.input_count == 8);
    struct aud_data shared = data(4, 3, bytes);
    expected_shared = &shared;
    mutate = retain = true;
    assert(module_appsrc_push_data(&src.module, aud_data_get(&shared), true) == 0);
    assert(shared.object.user == 2 && retained == &shared);
    assert(shared.info.type == type_es && shared.info.timestamp == 77);
    assert(shared.info.timestamp64 == 88 && bytes[0] == 0xa5);
    ++bypass_calls; /* same pointer and mutations are visible to the next push */
    aud_object_put(&retained->object);
    retained = NULL;
    assert(shared.object.user == 1);
    aud_object_put(&shared.object);
    teardown();
}
static void test_debug_sync_and_teardown(void)
{
    setup();
    struct aud_data a = data(4, 1, NULL);
    aud_module_sync_at_push = true;
    assert(module_appsrc_push_data(&src.module, &a, false) == 0);
    assert(seen == 1 && !a.object.user && !output->pending_obj_num);
    aud_module_sync_at_push = false;
    struct aud_data b = data(4, 2, NULL);
    assert(module_appsrc_push_data(&src.module, &b, false) == 0);
    appsrc_destructor(&src.module); /* the appsrc callback alone owns nothing */
    assert(b.object.user == 1 && output->pending_obj_num == 1);
    teardown();
    assert(!b.object.user && !first.object.user);
}
static void test_drops_and_errors(void)
{
    setup();
    struct aud_data a = data(4, 1, NULL);
    src.module.drop_output_data = true;
    assert(module_appsrc_push_data(&src.module, &a, true) == 0 && !a.object.user);
    src.module.drop_output_data = false;
    struct aud_data b = data(5, 2, NULL);
    b.info.type = type_meta;
    src.module.drop_output_meta = true;
    assert(module_appsrc_push_data(&src.module, &b, false) == 0 && !b.object.user);
    no_output = true;
    struct aud_data c = data(6, 3, NULL);
    assert(module_appsrc_push_data(&src.module, &c, true) == 0 && !c.object.user);
    assert(!seen && src.input_count == 15);
    no_output = false;
    fail_port = true;
    struct aud_event e = event(AUD_EVENT_TYPE_CUSTOM, true);
    assert(module_appsrc_push_event(&src.module, &e) == 0 && !e.object.user);
    assert(!seen);
    fail_port = false;
    fail_alloc = true;
    struct aud_data oom = data(7, 4, NULL);
    assert(module_appsrc_push_data(&src.module, &oom, false) == 0);
    assert(oom.object.user == 1 && !output->pending_obj_num && !output->pending_objs);
    aud_object_put(&oom.object); /* fixture cleanup of demonstrated ignored-OOM leak */
    struct aud_event oom_event = event(AUD_EVENT_TYPE_CUSTOM, true);
    assert(module_appsrc_push_event(&src.module, &oom_event) == 0);
    assert(oom_event.object.user == 1 && !output->pending_obj_num);
    /* foreach_push_event's extra ref was neither queued nor released on OOM. */
    aud_object_put(&oom_event.object);
    teardown();
}
static void test_callbacks(void)
{
    setup();
    assert(registered->constructor(&src.module, NULL, NULL) == 0);
    assert(registered->cmd(&src.module, 0, NULL, 0) == -17);
    struct aud_event e = event(AUD_EVENT_TYPE_CUSTOM, true);
    assert(registered->event(&src.module, NULL, &e) == -18 && !e.object.user);
    struct aud_port input = {.module = &src.module};
    struct aud_data d = data(4, 7, NULL);
    assert(registered->chain(&input, &d) == 0);
    assert(!seen && output->pending_obj_num == 1 && src.input_count == 4);
    assert(aud_module_sync(&src.module, 0) == 0);
    assert(seen == 1 && order[0] == 107 && !d.object.user);
    teardown();
}
static void test_legacy_wait_and_flush(void)
{
    for (int flush = 0; flush < 2; ++flush) {
        setup();
        ADEC_BUF_T ring = {0};
        ring.get_free_size = get_free;
        ring.insert_au = insert;
        output->legacy_buf = &ring;
        free_bytes = 3;
        flush_on_wait = flush;
        unsigned char bytes[4] = {0};
        struct aud_data d = data(4, 1, bytes);
        assert(module_appsrc_push_data(&src.module, &d, true) == 0);
        assert(waits == 1 && writes == (flush ? 0u : 1u));
        assert(!d.object.user && !output->pending_obj_num);
        ++bypass_calls;
        teardown();
    }
    setup();
    struct aud_data d = data(4, 1, NULL);
    assert(module_appsrc_push_data(&src.module, &d, false) == 0);
    struct aud_event async = event(AUD_EVENT_TYPE_FLUSH, false);
    assert(module_appsrc_push_event(&src.module, &async) == 0);
    assert(broadcasts == 1 && output->flushing == 1);
    assert(seen == 2 && order[0] == AUD_EVENT_TYPE_FLUSH && order[1] == 101);
    struct aud_event sync = event(AUD_EVENT_TYPE_FLUSH, true);
    assert(module_appsrc_push_event(&src.module, &sync) == 0);
    assert(!output->flushing && order[2] == AUD_EVENT_TYPE_FLUSH);
    assert(!async.object.user && !sync.object.user);
    teardown();
}
static void test_eos_submission_wait(void)
{
    for (int mode = 0; mode < 3; ++mode) {
        setup();
        ADEC_BUF_T ring = {.get_free_size = get_free, .insert_au = insert};
        output->legacy_buf = &ring;
        free_bytes = 0;
        flush_on_wait = mode == 1;
        wake_without_progress = mode != 1;
        full_after_data = mode == 2;
        if (full_after_data) free_bytes = INT_MAX;
        unsigned char bytes[4] = {0};
        struct aud_data pending = data(sizeof(bytes), 1, bytes);
        struct aud_event eos = event(AUD_EVENT_TYPE_CUSTOM, true);
        struct aud_event last = event(AUD_EVENT_TYPE_LAST_AU, true);
        expected_wait_seen = 2;
        assert(module_appsrc_push_data(&src.module, &pending, false) == 0);
        assert(!seen && output->pending_obj_num == 1);
        assert(module_appsrc_push_event(&src.module, &eos) == 0);
        assert(waits == (mode == 1 ? 1u : 2u));
        assert(writes == (mode == 1 ? 0u : 2u));
        assert(seen == 2 && order[0] == 101 && order[1] == AUD_EVENT_TYPE_CUSTOM);
        assert(!pending.object.user && !eos.object.user);
        assert(first.object.user == 1 && output->current_first == &first);
        assert(module_appsrc_push_event(&src.module, &last) == 0);
        assert(seen == 3 && order[2] == AUD_EVENT_TYPE_LAST_AU);
        assert(!last.object.user && !first.object.user && !output->current_first);
        assert(!output->pending_obj_num);
        teardown();
    }
}
static void test_event_worker_boundary(void)
{
    setup();
    pthread_cond_t kick;
    assert(pthread_cond_init(&kick, NULL) == 0);
    src.module.othread = (struct aud_thread *)&src;
    src.module.othread_kick = &kick;
    struct aud_event queued = event(AUD_EVENT_TYPE_CUSTOM, true);
    assert(aud_module_push_event(&src.module, 0, &queued) == 0);
    assert(broadcasts == 1 && !seen && output->pending_obj_num == 1);
    assert(queued.object.user == 1);
    struct aud_event last = event(AUD_EVENT_TYPE_LAST_AU, true);
    assert(module_appsrc_push_event(&src.module, &last) == 0);
    assert(broadcasts == 2 && seen == 2);
    assert(order[0] == AUD_EVENT_TYPE_CUSTOM && order[1] == AUD_EVENT_TYPE_LAST_AU);
    assert(!queued.object.user && !last.object.user && !output->pending_obj_num);
    src.module.othread = NULL;
    src.module.othread_kick = NULL;
    pthread_cond_destroy(&kick);
    teardown();
}
int main(void)
{
    assert(registered && !registered->flag && !registered->prop);
    assert(registered->instance_size == sizeof(struct aud_appsrc));
    test_fifo_and_shared_reference();
    test_debug_sync_and_teardown();
    test_drops_and_errors();
    test_callbacks();
    test_legacy_wait_and_flush();
    test_eos_submission_wait();
    test_event_worker_boundary();
    puts("appsrc: FIFO, ownership, errors, legacy waits/flush, EOS before LAST, teardown passed");
    return 0;
}
