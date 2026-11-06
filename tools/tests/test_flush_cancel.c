/* Usage: python3 tools/test_flush_cancel.py. Actual recovered lock/wait bodies;
 * only peer/ring services and list traversal are host mocks. No target execution. */
#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "libaa/types.h"
#include "libaa/buffers.h"

unsigned int logmm_dummy_mask;
struct logmm_module logmm_module_commod = {.mask = &logmm_dummy_mask};
struct logmm_module logmm_module_aud_appsrc = {.mask = &logmm_dummy_mask};
bool aud_module_do_check_beep;
int aud_module_silence_noise = INT_MAX;
static struct aud_module producer, sink;
static struct aud_module_impl producer_impl, sink_impl;
static struct aud_port out, in;
static struct aud_event first;
static ADEC_BUF_T ring;
static pthread_mutex_t control = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t changed = PTHREAD_COND_INITIALIZER;
static pthread_cond_t impl_cond = PTHREAD_COND_INITIALIZER;
static _Thread_local int role;
static int ring_waits, block_waits, callback_entered, release_callback;
static int producer_done, cancel_done, sync_done, cancel_async_lock, cancel_node_lock;
static int sync_stream_lock, wrapper_sync_lock, free_bytes, writes;
static bool peer_enabled, block_callback;
static enum aud_event_type cancel_type;
static bool use_wrapper;

static struct timespec deadline(void)
{
    struct timespec ts;
    assert(clock_gettime(CLOCK_REALTIME, &ts) == 0);
    ts.tv_sec += 4;
    return ts;
}
static void wait_for(int *value, int minimum)
{
    struct timespec ts = deadline();
    assert(pthread_mutex_lock(&control) == 0);
    while (*value < minimum)
        assert(pthread_cond_timedwait(&changed, &control, &ts) == 0);
    assert(pthread_mutex_unlock(&control) == 0);
}
static void note(int *value)
{
    assert(pthread_mutex_lock(&control) == 0);
    ++*value;
    assert(pthread_cond_broadcast(&changed) == 0);
    assert(pthread_mutex_unlock(&control) == 0);
}
static int observed_lock(pthread_mutex_t *mutex)
{
    if (role == 2 && mutex == &out.async_down_lock) note(&cancel_async_lock);
    if (role == 2 && mutex == &out.module_node.lock) note(&cancel_node_lock);
    if (role == 3 && mutex == &out.stream_lock) note(&sync_stream_lock);
    if (role == 2 && mutex == &out.stream_lock) note(&wrapper_sync_lock);
    return pthread_mutex_lock(mutex);
}
static int observed_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    struct timespec ts = deadline();
    if (cond == &out.check_free_cond) {
        assert(mutex == &out.check_free_mutex);
        assert(pthread_mutex_trylock(&out.stream_lock) == EBUSY);
        note(&ring_waits);
    } else {
        assert(cond == &sink.block_cond && mutex == &sink.block_mutex);
        assert(pthread_mutex_trylock(&in.stream_lock) == EBUSY);
        note(&block_waits);
    }
    int rc = pthread_cond_timedwait(cond, mutex, &ts);
    assert(rc == 0);
    return rc;
}
int logmm_printl(struct logmm_module *m, int level, const char *fn, int line,
                 const char *format, ...) { return 0; }
int logm_message(const char *format, ...) { return 0; }
struct aud_object *aud_object_get(struct aud_object *o)
{ assert(__atomic_fetch_add(&o->user, 1, __ATOMIC_SEQ_CST) > 0); return o; }
void aud_object_put(struct aud_object *o)
{ assert(__atomic_fetch_sub(&o->user, 1, __ATOMIC_SEQ_CST) > 0); }
const char *aud_event_name(struct aud_event *e) { return "event"; }
char *aud_event_str(struct aud_event *e) { return NULL; }
struct aud_event *aud_event_new_first(void) { assert(!"unexpected first"); return NULL; }
void *aud_event_serialize(struct aud_event *e, unsigned *size)
{ *size = 1; return calloc(1, 1); }
unsigned long long aud_system_cycle(void) { return 1; }
long long aud_thread_cycle(void) { return 1; }
void adjust_thread_priority(struct aud_module *m) { m->thread_priority = 0; }
int aimc_raise_named_event(struct aimc *imc, unsigned uid, const char *name,
                           void *param, unsigned size) { return 0; }
int add_pending_obj_locked(struct aud_port *p, struct aud_object *o)
{
    p->pending_objs = realloc(p->pending_objs, (p->pending_obj_num + 1) * sizeof(o));
    assert(p->pending_objs);
    p->pending_objs[p->pending_obj_num++] = o;
    return 0;
}
int add_pending_obj(struct aud_port *p, struct aud_object *o)
{
    pthread_mutex_lock(&p->stream_lock);
    int rc = add_pending_obj_locked(p, o);
    pthread_mutex_unlock(&p->stream_lock);
    return rc;
}
static AUDIO_DATA_TYPE_T legacy_data_type(struct aud_port *p, struct aud_data *d,
                                         ADEC_AU_INFO_T *info) { return AUDIO_DATA_TYPE_PCM; }
struct aud_port *aud_module_get_output(struct aud_module *m, unsigned n)
{
    assert(m == &producer && n == 0);
    observed_lock(&out.module_node.lock);
    return &out;
}
struct aud_port *aud_module_new_output(struct aud_module *m, unsigned n)
{ return aud_module_get_output(m, n); }
int aud_link_foreach_node(struct aud_link_root *root, void *arg, int (*cb)(void *, void *))
{
    if (root == &producer.output_port_root) {
        observed_lock(&out.module_node.lock);
        return cb(arg, &out);
    }
    if (root == &sink.output_port_root) return 0;
    assert(root == &out.down_peer_root);
    if (!peer_enabled) return 0;
    observed_lock(&in.down_peer_node.lock);
    return cb(arg, &in);
}
#define DIR_STR(p) (AUD_PORT_IS_OUTPUT(p) ? "out" : "in")
#define LOG_DISC(line, ...) LOGF(commod, log_level_info, "disconnect", line, __VA_ARGS__)
#define LOG_LAST(line, ...) LOGF(commod, log_level_warn, "last", line, __VA_ARGS__)
#define LOG_FLUSH(line, ...) LOGF(commod, log_level_info, "flush", line, __VA_ARGS__)
int foreach_sync(void *, void *);
int foreach_down_peer_push_data(void *, void *);
int foreach_down_peer_push_event(void *, void *);
int aud_module_event_locked(struct aud_module *, struct aud_port *, struct aud_event *);
int aud_module_chain_locked(struct aud_port *, struct aud_data *);
void aud_module_unblock_locked(struct aud_module *);
#define pthread_mutex_lock observed_lock
#define pthread_cond_wait observed_wait
/* RECOVERED_FUNCTIONS */
#undef pthread_mutex_lock
#undef pthread_cond_wait

static int sink_event(struct aud_module *m, struct aud_port *p, struct aud_event *e)
{ aud_object_put(&e->object); return 0; }
static int sink_chain(struct aud_port *p, struct aud_data *d)
{
    assert(p == &in);
    pthread_mutex_lock(&control);
    ++callback_entered;
    pthread_cond_broadcast(&changed);
    struct timespec ts = deadline();
    while (block_callback && !release_callback)
        assert(pthread_cond_timedwait(&impl_cond, &control, &ts) == 0);
    pthread_mutex_unlock(&control);
    aud_object_put(&d->object);
    return 0;
}
static int get_free(ADEC_BUF_T *b) { return free_bytes; }
static int insert(unsigned char *p, int size, ADEC_AU_INFO_T *info, ADEC_BUF_T *b)
{ ++writes; return 0; }
static struct aud_event event(enum aud_event_type type, bool sync)
{
    struct aud_event e = {0};
    e.object.type = AUD_OBJECT_TYPE_EVENT;
    e.object.user = 1;
    e.type = type;
    e.prop = AUD_EVENT_PROP_DOWNWARD | (sync ? AUD_EVENT_PROP_SYNC : 0);
    return e;
}
static void init_port(struct aud_port *p, struct aud_module *m)
{
    memset(p, 0, sizeof(*p));
    p->module = m;
    pthread_mutex_init(&p->module_node.lock, NULL);
    pthread_mutex_init(&p->down_peer_node.lock, NULL);
    pthread_mutex_init(&p->stream_lock, NULL);
    pthread_mutex_init(&p->async_down_lock, NULL);
    pthread_mutex_init(&p->async_up_lock, NULL);
    pthread_mutex_init(&p->check_free_mutex, NULL);
    pthread_cond_init(&p->check_free_cond, NULL);
}
static void setup(bool with_peer)
{
    memset(&producer, 0, sizeof(producer));
    memset(&sink, 0, sizeof(sink));
    producer.impl = &producer_impl;
    sink.impl = &sink_impl;
    producer.name = "producer";
    sink.name = "sink";
    producer_impl.name = "producer";
    sink_impl.name = "sink";
    sink_impl.chain = sink_chain;
    sink_impl.event = sink_event;
    pthread_mutex_init(&producer.block_mutex, NULL);
    pthread_mutex_init(&sink.block_mutex, NULL);
    pthread_cond_init(&producer.block_cond, NULL);
    pthread_cond_init(&sink.block_cond, NULL);
    pthread_mutex_init(&sink.input_port_root.rlock, NULL);
    LINK_INIT(&sink.input_port_root.nodes);
    init_port(&out, &producer);
    init_port(&in, &sink);
    out.flag = AUD_PORT_OUTPUT;
    in.up_peer = &out;
    in.module_node.data = &in;
    link_add(&sink.input_port_root.nodes, &in.module_node.root_link);
    first = event(AUD_EVENT_TYPE_FIRST_AU, true);
    in.current_first = &first;
    ring.get_free_size = get_free;
    ring.insert_au = insert;
    ring_waits = block_waits = callback_entered = release_callback = 0;
    producer_done = cancel_done = sync_done = cancel_async_lock = cancel_node_lock = 0;
    sync_stream_lock = wrapper_sync_lock = writes = free_bytes = 0;
    peer_enabled = with_peer;
    block_callback = use_wrapper = false;
    cancel_type = AUD_EVENT_TYPE_FLUSH;
}
static void destroy_port(struct aud_port *p)
{
    assert(!p->pending_obj_num);
    free(p->pending_objs);
    pthread_mutex_destroy(&p->module_node.lock);
    pthread_mutex_destroy(&p->down_peer_node.lock);
    pthread_mutex_destroy(&p->stream_lock);
    pthread_mutex_destroy(&p->async_down_lock);
    pthread_mutex_destroy(&p->async_up_lock);
    pthread_mutex_destroy(&p->check_free_mutex);
    pthread_cond_destroy(&p->check_free_cond);
}
static void teardown(void)
{
    destroy_port(&out);
    destroy_port(&in);
    pthread_mutex_destroy(&producer.block_mutex);
    pthread_mutex_destroy(&sink.block_mutex);
    pthread_cond_destroy(&producer.block_cond);
    pthread_cond_destroy(&sink.block_cond);
    pthread_mutex_destroy(&sink.input_port_root.rlock);
}
static void queue(struct aud_data *d)
{
    memset(d, 0, sizeof(*d));
    d->object.type = AUD_OBJECT_TYPE_DATA;
    d->object.user = 1;
    d->size = 4;
    d->info.type = type_pcm;
    static unsigned char bytes[4];
    d->data = bytes;
    add_pending_obj_locked(&out, &d->object);
}
static void *run_producer(void *unused)
{ role = 1; assert(aud_module_sync(&producer, 0) == 0); note(&producer_done); return NULL; }
static void *run_cancel(void *unused)
{
    role = 2;
    struct aud_event e = event(cancel_type, false);
    if (use_wrapper) assert(module_appsrc_push_event(&producer, &e) == 0);
    else assert(aud_module_push_event(&producer, 0, &e) == 0);
    assert(!e.object.user);
    note(&cancel_done);
    return NULL;
}
static void *run_sync(void *unused)
{ role = 3; aud_module_sync(&producer, 0); note(&sync_done); return NULL; }
static void *run_sync_flush(void *unused)
{
    role = 3;
    struct aud_event e = event(AUD_EVENT_TYPE_FLUSH, true);
    assert(aud_module_push_event(&producer, 0, &e) == 0);
    assert(!e.object.user);
    note(&sync_done);
    return NULL;
}
static void release_impl(void)
{
    pthread_mutex_lock(&control);
    release_callback = 1;
    pthread_cond_broadcast(&impl_cond);
    pthread_mutex_unlock(&control);
}
static void join(pthread_t t, int *done)
{ wait_for(done, 1); assert(pthread_join(t, NULL) == 0); }
static void not_done(int *done)
{ pthread_mutex_lock(&control); assert(!*done); pthread_mutex_unlock(&control); }

static void test_ring(enum aud_event_type type)
{
    setup(false);
    struct aud_data d;
    queue(&d);
    out.legacy_buf = &ring;
    producer.block_module = true;
    pthread_t writer, cancellation;
    assert(pthread_create(&writer, NULL, run_producer, NULL) == 0);
    wait_for(&ring_waits, 1);
    assert(aud_port_check_legacy(&out) == 0);
    wait_for(&ring_waits, 2); /* a wake is not cancellation or new space */
    assert(aud_module_check_legacy_output(&producer, 0) == 0);
    wait_for(&ring_waits, 3);
    struct aud_event e = event(type, false);
    assert(aud_port_push_event(&out, &e) == 0); /* forwards only: no flushing */
    pthread_mutex_lock(&out.check_free_mutex);
    assert(!out.flushing);
    pthread_mutex_unlock(&out.check_free_mutex);
    not_done(&producer_done);
    pthread_mutex_lock(&out.async_down_lock);
    cancel_type = type;
    assert(pthread_create(&cancellation, NULL, run_cancel, NULL) == 0);
    wait_for(&cancel_async_lock, 1);
    join(writer, &producer_done); /* wake preceded cancellation's async lock */
    not_done(&cancel_done);
    assert(out.flushing && !producer.block_module && !writes && !d.object.user);
    assert(!!(out.flag & AUD_PORT_DISABLED) == (type == AUD_EVENT_TYPE_DISCONNECT));
    pthread_mutex_unlock(&out.async_down_lock);
    join(cancellation, &cancel_done);
    free_bytes = INT_MAX;
    e = event(type, true);
    assert(aud_module_push_event(&producer, 0, &e) == 0);
    assert(!out.flushing && !(out.flag & AUD_PORT_DISABLED) && !e.object.user);
    teardown();
    puts("PASS ring cancellation precedes async lock; recheck/raw port event do not cancel");
}
static void test_wrapper_and_arbitrary_wait(void)
{
    setup(true);
    block_callback = use_wrapper = true;
    struct aud_data d;
    queue(&d);
    pthread_t writer, cancellation;
    assert(pthread_create(&writer, NULL, run_producer, NULL) == 0);
    wait_for(&callback_entered, 1);
    assert(pthread_create(&cancellation, NULL, run_cancel, NULL) == 0);
    wait_for(&wrapper_sync_lock, 1);
    not_done(&producer_done);
    not_done(&cancel_done);
    assert(in.throwing); /* async phase completed, arbitrary chain still waits */
    release_impl();
    join(writer, &producer_done);
    join(cancellation, &cancel_done);
    assert(!d.object.user);
    teardown();
    puts("PASS async crosses peer stream lock, but arbitrary chain/wrapper sync still wait");
}
static void test_terminal_block_module(void)
{
    setup(true);
    sink.block_module = true;
    struct aud_data d;
    queue(&d);
    pthread_t writer;
    assert(pthread_create(&writer, NULL, run_producer, NULL) == 0);
    wait_for(&block_waits, 1);
    struct aud_event e = event(AUD_EVENT_TYPE_FLUSH, false);
    assert(aud_module_push_event(&producer, 0, &e) == 0);
    pthread_mutex_lock(&sink.block_mutex);
    assert(sink.block_module && in.throwing);
    pthread_mutex_unlock(&sink.block_mutex);
    not_done(&producer_done);
    aud_module_unblock_locked(&sink); /* explicit endpoint unblock, no port lock */
    join(writer, &producer_done);
    assert(!callback_entered && !d.object.user);
    teardown();
    puts("PASS terminal block-module is not woken by upstream output cancellation");
}
static void test_early_sync_flush(void)
{
    setup(false);
    out.flushing = 1;
    pthread_mutex_lock(&out.stream_lock);
    pthread_t sync_flush;
    assert(pthread_create(&sync_flush, NULL, run_sync_flush, NULL) == 0);
    wait_for(&sync_stream_lock, 1);
    pthread_mutex_lock(&out.check_free_mutex);
    assert(!out.flushing); /* cancellation state cleared before stream acquired */
    pthread_mutex_unlock(&out.check_free_mutex);
    not_done(&sync_done);
    pthread_mutex_unlock(&out.stream_lock);
    join(sync_flush, &sync_done);
    teardown();
    puts("PASS sync FLUSH clears cancellation flag before waiting for stream lock");
}
static void test_node_lock_obstruction(void)
{
    setup(true);
    block_callback = true;
    struct aud_data d;
    queue(&d);
    pthread_t writer, second_sync, cancellation;
    assert(pthread_create(&writer, NULL, run_producer, NULL) == 0);
    wait_for(&callback_entered, 1);
    assert(pthread_create(&second_sync, NULL, run_sync, NULL) == 0);
    wait_for(&sync_stream_lock, 1); /* owns module_node while stream is occupied */
    assert(pthread_create(&cancellation, NULL, run_cancel, NULL) == 0);
    wait_for(&cancel_node_lock, 1);
    pthread_mutex_lock(&out.check_free_mutex);
    assert(!out.flushing);
    pthread_mutex_unlock(&out.check_free_mutex);
    not_done(&cancel_done);
    release_impl();
    join(writer, &producer_done);
    join(second_sync, &sync_done);
    join(cancellation, &cancel_done);
    assert(out.flushing);
    teardown();
    puts("PASS queued sync holds node lock before stream: async cancellation can be obstructed");
}
int main(void)
{
    test_ring(AUD_EVENT_TYPE_FLUSH);
    test_ring(AUD_EVENT_TYPE_DISCONNECT);
    test_wrapper_and_arbitrary_wait();
    test_terminal_block_module();
    test_early_sync_flush();
    test_node_lock_obstruction();
    puts("flush cancellation tests passed (bounded pthread conditions, no sleep loops)");
    return 0;
}
