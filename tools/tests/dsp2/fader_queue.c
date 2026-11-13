/* DSP2 fader/queue: wait threshold, FIRST hold, back-pressure bound, queue-variant
 * LAST/FLUSH/yield paths. Framework doubles only; fade arithmetic is not checked.
 */
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../../src/dsp2/fader_queue.c"

struct object { void *p; int refs; bool data; };
static struct object objects[256];
static int object_count;
struct test_event { struct aud_event event; int size, count, channels, bits, es, delay, front, dolby, passing; };
static struct aud_data *fifo[128];
static int fifo_count, signals, waits, timeout_result, raised, creates, destroys, yields;
static int in_signals, out_signals, block_signals, out_waits, block_waits;
static unsigned char mapped[4096];
static struct { int kind, port, type; unsigned size; unsigned char bytes[256]; } output[128];
static int output_count;
static struct fader *active;
static void (*wait_action)(struct fader *);
static void (*sync_action)(struct fader *);
static const struct fader_desc normal_desc = { 2, 0, 4, 2, 4, 0 };
static const struct fader_desc queue_desc = { 2, 0, 4, 2, 4, 1 };
static const struct aud_module_class normal_class = { "fader", &normal_desc };
static const struct aud_module_class queue_class = { "queue", &queue_desc };
volatile uint32_t fader_log_mask;
const void *const fader_log = NULL;
const struct aud_module_class fader_class = { "fader", &normal_desc };
const struct fader_desc fader_desc_fader = {0}, fader_desc_queue = {0}, fader_desc_input_queue = {0};
const void *const fader_debug_opts = NULL;

static void *track(void *p, bool data)
{
    assert(object_count < 256 && p);
    objects[object_count++] = (struct object){p, 1, data};
    return p;
}
static struct object *object(void *p)
{
    for (int i = 0; i < object_count; ++i)
        if (objects[i].p == p) return &objects[i];
    assert(!"unknown object");
    return NULL;
}
void *aud_object_get(void *p) { assert(p); ++object(p)->refs; return p; }
void aud_object_put(void *p) { assert(p && object(p)->refs > 0); --object(p)->refs; }
static struct aud_event *event(int type)
{
    struct test_event *e = track(calloc(1, sizeof *e), false);
    e->event.type = type; e->count = 1; e->channels = 2; e->bits = 16; e->es = 1;
    return &e->event;
}
struct aud_event *aud_event_dup(struct aud_event *e)
{
    struct test_event *n = track(malloc(sizeof *n), false);
    *n = *(struct test_event *)e; return &n->event;
}
struct aud_event *aud_event_new_last(void) { return event(AUD_EVENT_LAST_AU); }
struct aud_event *aud_event_newv_custom(const char *n, int a, int b, int c)
{ (void)n; (void)a; (void)b; (void)c; return event(AUD_EVENT_CUSTOM); }
const char *aud_event_name(struct aud_event *e) { (void)e; return "event"; }
int aud_event_value_get_int(struct aud_event *e, const char *k, int def)
{
    struct test_event *t = (struct test_event *)e;
    if (!strcmp(k, "queue-size")) return t->size;
    if (!strcmp(k, "queue-count")) return t->count;
    if (!strcmp(k, "channels")) return t->channels;
    if (!strcmp(k, "bits-per-sample")) return t->bits;
    if (!strcmp(k, "low-delay")) return t->delay;
    if (!strcmp(k, "front-padding")) return t->front;
    if (!strcmp(k, "passing-length")) return t->passing;
    return def;
}
bool aud_event_value_get_bool(struct aud_event *e, const char *k, bool def)
{
    if (!strcmp(k, "es")) return ((struct test_event *)e)->es;
    if (!strcmp(k, "dolby")) return ((struct test_event *)e)->dolby;
    return def;
}
void aud_event_value_set_int(struct aud_event *e, const char *k, int v)
{ assert(!strcmp(k, "passing-length")); ((struct test_event *)e)->passing = v; }
void aud_event_value_del(struct aud_event *e, const char *k)
{ if (!strcmp(k, "queue-size")) ((struct test_event *)e)->size = 0; else ((struct test_event *)e)->count = 0; }
struct aud_data *aud_data_new(unsigned size)
{
    struct aud_data *d = track(calloc(1, sizeof *d), true);
    d->size = size; d->ptr = malloc(size ? size : 1); memset(d->ptr, 0xa5, size); return d;
}
struct aud_data *aud_data_new_zero(unsigned size)
{ struct aud_data *d = aud_data_new(size); memset(d->ptr, 0, size); return d; }
struct aud_data *aud_data_new_metadata(void *p) { (void)p; return NULL; }
void aud_data_adapter_push(struct aud_data_adapter *q, struct aud_data *d)
{ assert(fifo_count < 128); fifo[fifo_count++] = d; q->total += d->size; }
void aud_data_adapter_push_front(struct aud_data_adapter *q, struct aud_data *d)
{ memmove(fifo + 1, fifo, (size_t)fifo_count * sizeof *fifo); fifo[0] = d; ++fifo_count; q->total += d->size; }
struct aud_data *aud_data_adapter_dequeue(struct aud_data_adapter *q)
{
    if (!fifo_count) return NULL;
    struct aud_data *d = fifo[0]; --fifo_count;
    memmove(fifo, fifo + 1, (size_t)fifo_count * sizeof *fifo); q->offset += d->size; return d;
}
void aud_data_adapter_flush(struct aud_data_adapter *q, unsigned n)
{
    while (n) {
        assert(fifo_count && n >= fifo[0]->size);
        unsigned size = fifo[0]->size; aud_object_put(aud_data_adapter_dequeue(q)); n -= size;
    }
}
void *aud_data_adapter_map(struct aud_data_adapter *q, unsigned n)
{
    unsigned off = 0; (void)q;
    for (int i = 0; i < fifo_count && off < n; ++i) {
        unsigned k = fifo[i]->size; if (k > n - off) k = n - off;
        memcpy(mapped + off, fifo[i]->ptr, k); off += k;
    }
    assert(off == n); return mapped;
}
void aud_data_adapter_unmap(struct aud_data_adapter *q, void *p) { (void)q; assert(p == mapped); }
static int push(int kind, int port, int type, struct aud_data *d)
{
    assert(output_count < 128); int i = output_count++;
    output[i].kind = kind; output[i].port = port; output[i].type = type;
    if (d) { assert(d->size <= 256); output[i].size = d->size; memcpy(output[i].bytes, d->ptr, d->size); }
    return 0;
}
int aud_module_push_data(struct fader *f, int p, struct aud_data *d)
{ (void)f; push(1, p, d->type, d); aud_object_put(d); return 0; }
int aud_module_push_event(struct fader *f, int p, struct aud_event *e)
{ (void)f; push(2, p, e->type, NULL); aud_object_put(e); return 0; }
void aud_module_sync(struct fader *f, int p) { (void)p; if (sync_action) sync_action(f); }
int aud_module_default_event(struct fader *f, void *p, struct aud_event *e)
{ (void)p; return aud_module_push_event(f, 0, e); }
int aud_module_output_thread_create(struct fader *f, void *fn, void *arg, struct aud_mutex *m, struct aud_cond *c)
{ (void)fn; (void)arg; (void)m; (void)c; f->output_thread = f; ++creates; return 0; }
void aud_module_output_thread_destroy(struct fader *f) { f->output_thread = NULL; ++destroys; }
void xos_thread_yield(void) { ++yields; }
void aud_mutex_lock(struct aud_mutex *m) { ++m->opaque; }
void aud_mutex_unlock(struct aud_mutex *m) { --m->opaque; }
void aud_cond_signal(struct aud_cond *c)
{
    ++signals;
    if (c == &active->in_cond) ++in_signals;
    if (c == &active->out_cond) ++out_signals;
    if (c == &active->block_cond) ++block_signals;
}
void aud_cond_wait(struct aud_cond *c, struct aud_mutex *m)
{
    (void)m; ++waits;
    if (c == &active->out_cond) ++out_waits;
    if (c == &active->block_cond) ++block_waits;
    assert(wait_action); wait_action(active);
}
int aud_cond_wait_timeout(struct aud_cond *c, struct aud_mutex *m, unsigned us)
{ (void)c; (void)m; ++waits; if (us == 10) return 0; if (wait_action) wait_action(active); return timeout_result; }
void *aud_thread_create(const char *n, unsigned stack, int priority, void *fn, void *arg)
{ (void)n; (void)stack; (void)priority; (void)fn; return arg; }
void aud_thread_destroy(void *p) { (void)p; }
uint64_t aud_system_cycle(void) { return 42; }
void aimc_raise_named_event(void *a, uint32_t uid, const char *n, const void *p, int count, ...)
{ (void)a; (void)uid; (void)n; (void)p; (void)count; ++raised; }
void dot_module_connection(int a, int b) { (void)a; (void)b; }
int msf_get_curr_num_md(void *p) { (void)p; return 0; }
int msf_get_avail_samples_read(void *p) { (void)p; return 0; }
void *msf_get_metadata(void *p, int n) { (void)p; (void)n; return NULL; }
void msf_add_put_metadata(void *p, void *m) { (void)p; (void)m; }
void aud_meta_data_free(void *p) { (void)p; }
void msf_add_update_write_buf(void *p, int n) { (void)p; (void)n; }
void msf_update_read_buf(void *p, int n) { (void)p; (void)n; }
int msf_process_metadata(void *p, void *d, unsigned n) { (void)p; (void)d; (void)n; return 0; }
void msf_flush_metadata(void *p) { (void)p; }
bool fader_trace_enabled(const char *n, int dir) { (void)n; (void)dir; return false; }
void logmm_printl(const void *l, int v, const char *fn, int line, const char *fmt, ...)
{ (void)l; (void)v; (void)fn; (void)line; (void)fmt; }
void logmm_printlm(const char *l, int v, const char *fn, int line, const char *fmt, ...)
{ (void)l; (void)v; (void)fn; (void)line; (void)fmt; }
void logm_message(const char *fmt, ...) { (void)fmt; }
void ModuleRegistry_RegisterNamef(const void *c, const void *d, const char *n) { (void)c; (void)d; (void)n; }

static void setup(struct fader *f, bool queue)
{
    memset(f, 0, sizeof *f); active = f; f->cls = queue ? &queue_class : &normal_class;
    f->name = "test"; f->frame_bytes = 4; f->channels = 2; f->bits = 16;
    f->wait_len = 2; f->max_len = 4; f->fade_len = 2; f->chunk_len = 4; f->chunk_bytes = 16;
    f->pe.next = f->pe.prev = &f->pe; f->process = _fader_process_data_wait;
    f->padding_len = 4; f->fadein = fadein_bypass; f->fadeout = fadeout_bypass;
    output_count = signals = waits = raised = creates = destroys = yields = 0;
    in_signals = out_signals = block_signals = out_waits = block_waits = 0;
    wait_action = NULL; sync_action = NULL; timeout_result = 0;
}
static void cleanup(struct fader *f)
{
    pending_events_empty(f); aud_data_adapter_flush(&f->q, QUEUED_BYTES(f));
    if (f->first) aud_object_put(f->first);
    if (f->meta_ext) aud_object_put(f->meta_ext);
    for (int i = 0; i < object_count; ++i) {
        assert(objects[i].refs == 0);
        if (objects[i].data) free(((struct aud_data *)objects[i].p)->ptr);
        free(objects[i].p);
    }
    object_count = 0; assert(fifo_count == 0);
}
static void stop_wait(struct fader *f) { f->state = FS_STOP; }
static void drain_wait(struct fader *f) { aud_data_adapter_flush(&f->q, QUEUED_BYTES(f)); }
static void add_one(struct fader *f) { aud_data_adapter_push(&f->q, aud_data_new_zero(4)); }

static void test_pending_signed_readiness(void)
{
    const int positions[] = { 1, 0, -1, INT_MIN, INT_MAX };
    struct fader f; setup(&f, true);
    for (unsigned index = 0; index < sizeof positions / sizeof *positions; ++index) {
        assert(!pending_events_add(&f, event(100 + (int)index)));
        ((struct pending_event *)f.pe.prev)->at_bytes = positions[index];
    }
    f.pe_signal = true;
    aud_mutex_lock(&f.lock);
    assert(!pending_events_check(&f) && !f.pe_signal && f.lock.opaque == 1);
    aud_mutex_unlock(&f.lock);
    assert(output_count == 3 && output[0].type == 101 && output[1].type == 102 && output[2].type == 103);
    assert(((struct pending_event *)f.pe.next)->at_bytes == 1);
    assert(((struct pending_event *)f.pe.prev)->at_bytes == INT_MAX);
    cleanup(&f);
}

static void test_pending_coalescing_and_positions(void)
{
    struct fader f; setup(&f, true);
    aud_data_adapter_push(&f.q, aud_data_new_zero(8));
    struct aud_event *old = event(AUD_EVENT_CUSTOM);
    assert(!pending_events_add(&f, old));
    assert(!pending_events_add(&f, event(AUD_EVENT_FLUSH)));
    aud_data_adapter_push(&f.q, aud_data_new_zero(4));
    struct aud_event *replacement = event(AUD_EVENT_CUSTOM);
    assert(!pending_events_add(&f, replacement) && object(old)->refs == 0);
    struct pending_event *head = (struct pending_event *)f.pe.next;
    struct pending_event *tail = (struct pending_event *)f.pe.prev;
    assert(head->event->type == AUD_EVENT_FLUSH && head->at_bytes == 8);
    assert(tail->event == replacement && tail->at_bytes == 12 && head->link.next == &tail->link);
    pending_events_update(&f, 8);
    aud_mutex_lock(&f.lock);
    assert(!pending_events_check(&f) && output_count == 1 && output[0].type == AUD_EVENT_FLUSH);
    assert(tail->at_bytes == 4 && object(replacement)->refs == 1);
    pending_events_update(&f, 5);
    assert(!pending_events_check(&f) && output_count == 2 && output[1].type == AUD_EVENT_CUSTOM);
    assert(object(replacement)->refs == 0 && f.pe.next == &f.pe);
    aud_mutex_unlock(&f.lock);
    cleanup(&f);
}

static void held_marker_wake(struct fader *f)
{
    assert(QUEUED(f) == f->wait_len && output_count == 0 && !f->first_sent);
    assert(((struct pending_event *)f->pe.next)->at_bytes == 8);
    if (out_waits == 1) {
        assert(f->pe_signal);
    } else {
        assert(out_waits == 2 && !f->pe_signal);
        f->state = FS_STOP;
    }
}

static void test_custom_and_last_behind_wait_audio(void)
{
    for (int queue = 0; queue < 2; ++queue) {
        struct fader f; setup(&f, queue); struct aud_port port = {&f, 0};
        f.first = event(AUD_EVENT_FIRST_AU); f.enable_underflow = true;
        aud_data_adapter_push(&f.q, aud_data_new_zero(8));
        struct aud_event *last = event(AUD_EVENT_LAST_AU);
        assert(!fader_event(&f, &port, last) && object(last)->refs == 0);
        assert(!f.enable_underflow && signals == 0 && f.pe.next == &f.pe);
        assert(!fader_event(&f, &port, event(AUD_EVENT_CUSTOM)));
        assert(out_signals == 1 && in_signals == 0 && block_signals == 0);
        wait_action = held_marker_wake;
        aud_mutex_lock(&f.lock);
        assert(_fader_process_data_wait(&f) == -1 && out_waits == 2 && output_count == 0);
        aud_mutex_unlock(&f.lock);
        assert(QUEUED_BYTES(&f) == 8 && f.first && f.pe.next != &f.pe);
        cleanup(&f);
    }
}

static void empty_marker_wake(struct fader *f)
{
    if (out_waits == 1) {
        assert(!fader_event(f, NULL, event(AUD_EVENT_CUSTOM)));
    } else {
        assert(out_waits == 2 && output_count == 1 && output[0].type == AUD_EVENT_CUSTOM);
        assert(!f->first_sent && f->first && f->pe.next == &f->pe);
        f->state = FS_STOP;
    }
}

static void test_ready_custom_in_wait(void)
{
    struct fader f; setup(&f, true); f.first = event(AUD_EVENT_FIRST_AU);
    wait_action = empty_marker_wake;
    aud_mutex_lock(&f.lock);
    assert(_fader_process_data_wait(&f) == -1 && out_waits == 2 && out_signals == 1);
    aud_mutex_unlock(&f.lock);
    cleanup(&f);
}

static void pending_downstream_sync(struct fader *f)
{
    assert(f->lock.opaque == 0 && f->pe_lock.opaque == 0);
    if (output_count == 1) {
        assert(output[0].kind == 1 && !block_waits);
        assert(((struct pending_event *)f->pe.next)->at_bytes == 0);
    } else {
        assert(output_count == 2 && output[1].type == AUD_EVENT_CUSTOM);
        assert(block_waits == 1 && !f->block && f->pe.next == &f->pe);
    }
}

static void pending_block_release(struct fader *f)
{
    assert(block_waits == 1 && output_count == 1 && f->lock.opaque == 0);
    assert(f->block_lock.opaque == 1 && ((struct pending_event *)f->pe.next)->at_bytes == 0);
    queue_module_unblock(f);
}

static void test_pending_after_downstream_and_block(void)
{
    struct fader f; setup(&f, true); f.fade_len = 0;
    set_state(&f, FS_RUNNING, _fader_process_data_running);
    aud_data_adapter_push(&f.q, aud_data_new_zero(4));
    assert(!fader_event(&f, NULL, event(AUD_EVENT_CUSTOM)));
    f.block = true; sync_action = pending_downstream_sync; wait_action = pending_block_release;
    aud_mutex_lock(&f.lock);
    assert(!fader_output_work(NULL, &f) && f.lock.opaque == 1);
    aud_mutex_unlock(&f.lock);
    assert(output_count == 2 && in_signals == 1 && out_signals == 1 && block_signals == 1);
    assert(f.state == FS_WAIT && object_count && f.pe.next == &f.pe);
    cleanup(&f);
}

static void test_flush_retains_pending_distance(void)
{
    for (int producer = 0; producer < 2; ++producer) {
        struct fader f; setup(&f, true); struct aud_port port = {&f, 0};
        set_state(&f, FS_RUNNING, _fader_process_data_running); f.fade_len = 0;
        aud_data_adapter_push(&f.q, aud_data_new_zero(8));
        fader_event(&f, &port, event(AUD_EVENT_CUSTOM));
        f.block = true;
        struct aud_event *flush = event(AUD_EVENT_FLUSH); flush->prop = AUD_EVENT_PROP_SYNC;
        assert(!fader_event(&f, &port, flush));
        assert(f.set_flush && !f.flushing && !f.block);
        assert(in_signals == 1 && block_signals == 1 && out_signals == 2);
        aud_mutex_lock(&f.lock);
        if (producer) {
            aud_mutex_unlock(&f.lock);
            assert(!fader_chain(&port, aud_data_new_zero(4)));
            aud_mutex_lock(&f.lock);
        } else {
            assert(!fader_output_work(NULL, &f) && f.state == FS_WAIT);
        }
        assert(!f.set_flush && output_count == 0);
        assert(((struct pending_event *)f.pe.next)->at_bytes == 8);
        assert(((struct pending_event *)f.pe.prev)->at_bytes == 8);
        set_state(&f, FS_RUNNING, _fader_process_data_running);
        if (!producer) aud_data_adapter_push(&f.q, aud_data_new_zero(4));
        assert(!fader_output_work(NULL, &f) && output_count == 1);
        assert(((struct pending_event *)f.pe.next)->at_bytes == 4);
        aud_data_adapter_push(&f.q, aud_data_new_zero(4));
        set_state(&f, FS_RUNNING, _fader_process_data_running);
        assert(!fader_output_work(NULL, &f) && output_count == 4);
        assert(output[0].kind == 1 && output[1].kind == 1);
        assert(output[2].type == AUD_EVENT_CUSTOM && output[3].type == AUD_EVENT_FLUSH);
        aud_mutex_unlock(&f.lock);
        cleanup(&f);
    }
}

static void test_sync_flush_in_wait(void)
{
    struct fader f; setup(&f, true); f.first = event(AUD_EVENT_FIRST_AU);
    aud_data_adapter_push(&f.q, aud_data_new_zero(8));
    struct aud_event *flush = event(AUD_EVENT_FLUSH); flush->prop = AUD_EVENT_PROP_SYNC;
    assert(!fader_event(&f, NULL, flush));
    assert(in_signals == 1 && out_signals == 1 && f.set_flush);
    wait_action = held_marker_wake;
    aud_mutex_lock(&f.lock);
    assert(_fader_process_data_wait(&f) == -1 && out_waits == 2);
    aud_mutex_unlock(&f.lock);
    assert(f.set_flush && QUEUED_BYTES(&f) == 8 && object(flush)->refs == 1);
    cleanup(&f);
}

static void test_async_flush_overtakes_pending(void)
{
    for (int queue = 0; queue < 2; ++queue) {
        struct fader f; setup(&f, queue); struct aud_port port = {&f, 0}; f.fade_len = 0;
        set_state(&f, FS_RUNNING, _fader_process_data_running);
        aud_data_adapter_push(&f.q, aud_data_new_zero(4));
        assert(!fader_event(&f, &port, event(AUD_EVENT_CUSTOM)));
        f.block = true;
        assert(!fader_event(&f, &port, event(AUD_EVENT_FLUSH)));
        assert(f.flushing && !f.set_flush && !f.block);
        assert(in_signals == 1 && block_signals == 1 && out_signals == 1);
        assert(output_count == queue);
        if (queue) assert(output[0].type == AUD_EVENT_FLUSH);
        assert(((struct pending_event *)f.pe.next)->at_bytes == 4);
        assert(fader_chain(&port, aud_data_new_zero(4)) == -1 && f.dropped_frames == 1);
        aud_mutex_lock(&f.lock);
        assert(!fader_output_work(NULL, &f));
        aud_mutex_unlock(&f.lock);
        assert(output_count == queue + 2 && output[queue].kind == 1);
        assert(output[queue + 1].type == AUD_EVENT_CUSTOM && f.pe.next == &f.pe);
        cleanup(&f);
    }
}

static void test_last_short_tail_order(void)
{
    for (int queue = 0; queue < 2; ++queue) {
        for (int fadeout = 0; fadeout < 2; ++fadeout) {
            for (int dolby = 0; dolby <= fadeout; ++dolby) {
                struct fader f; setup(&f, queue); f.dolby = dolby;
                f.first = event(AUD_EVENT_FIRST_AU); f.first_sent = true;
                f.fade_pos = fadeout ? 1 : 0;
                set_state(&f, fadeout ? FS_FADEOUT : FS_FADEIN,
                          fadeout ? _fader_process_data_fadeout : _fader_process_data_fadein);
                struct aud_data *tail = aud_data_new(4); memset(tail->ptr, 0x91, 4);
                aud_data_adapter_push(&f.q, tail);
                assert(!fader_event(&f, NULL, event(AUD_EVENT_CUSTOM)));
                assert(!fader_event(&f, NULL, event(AUD_EVENT_LAST_AU)));
                assert(out_signals == 1 && !f.enable_underflow);
                aud_mutex_lock(&f.lock);
                assert(!fader_output_work(NULL, &f) && f.state == FS_WAIT && !QUEUED_BYTES(&f));
                int data_count = fadeout ? 2 + dolby : 1;
                assert(output_count == data_count + 1 && output[data_count].type == AUD_EVENT_CUSTOM);
                for (int index = 0; index < data_count; ++index) {
                    assert(output[index].kind == 1 && output[index].size == 16);
                    for (unsigned byte = 0; byte < 16; ++byte)
                        assert(output[index].bytes[byte] == (!index && byte < 4 ? 0x91 : 0));
                }
                assert(in_signals == 1 && f.pe.next == &f.pe);
                wait_action = stop_wait;
                assert(!fader_output_work(NULL, &f));
                aud_mutex_unlock(&f.lock);
                assert(output_count == data_count + 1 + !queue && raised == 0 && waits == 1);
                if (!queue) assert(output[output_count - 1].type == AUD_EVENT_LAST_AU && !f.first_sent);
                else assert(f.first_sent);
                cleanup(&f);
            }
        }
    }
}

static void test_last_passthrough_modes(void)
{
    for (int bypass = 0; bypass < 2; ++bypass) {
        struct fader f; setup(&f, false); f.enable_underflow = true;
        f.low_delay = !bypass; f.bypass = bypass;
        struct aud_event *last = event(AUD_EVENT_LAST_AU);
        assert(!fader_event(&f, NULL, last));
        assert(!f.enable_underflow && signals == 0 && output_count == 1);
        assert(output[0].type == AUD_EVENT_LAST_AU && object(last)->refs == 0);
        cleanup(&f);
    }
}

/* FIRST is held, and no data leaves, until MORE than wait-length frames are queued. */
static void test_first_held_until_threshold(void)
{
    struct fader f; setup(&f, false); struct aud_port p = {&f, 0};
    assert(!fader_event(&f, &p, event(AUD_EVENT_FIRST_AU)));
    assert(f.zero_left == 4 && fifo_count == 2 && creates == 1 && output_count == 0);
    struct aud_data *d = aud_data_new(4); memset(d->ptr, 0x6a, 4);
    assert(!fader_chain(&p, d) && waits == 0);                 /* queued == max: no back-pressure */
    assert(!fader_output_work(NULL, &f));
    assert(output_count == 3 && output[0].kind == 2 && output[0].type == AUD_EVENT_FIRST_AU);
    assert(output[1].kind == 1 && output[2].kind == 1 && output[1].size == 8);
    assert(f.zero_left == 0 && fifo_count == 1);
    assert(!fader_stop(&f) && f.state == FS_STOP && destroys == 1);
    cleanup(&f);

    setup(&f, false); f.first = event(AUD_EVENT_FIRST_AU); f.fade_len = 0;
    aud_data_adapter_push(&f.q, aud_data_new_zero(8)); wait_action = add_one;
    _fader_process_data_wait(&f);
    assert(waits == 1 && output_count == 1 && output[0].type == AUD_EVENT_FIRST_AU);
    assert(f.state == FS_RUNNING);
    cleanup(&f);
}

/* Only the fader variant closes a segment with LAST and raises underflow_input. */
static void test_wait_last_and_underflow_by_variant(void)
{
    struct fader f; setup(&f, false); f.first = event(1); f.first_sent = true; f.enable_underflow = true;
    timeout_result = 1; wait_action = stop_wait;
    assert(_fader_process_data_wait(&f) == -1 && raised == 1 && f.underflow_count == 1);
    assert(output_count == 2 && output[0].type == AUD_EVENT_LAST_AU && output[1].port == 1);
    cleanup(&f);

    setup(&f, true); f.first = event(1); f.first_sent = true; f.enable_underflow = true;
    timeout_result = 1; wait_action = stop_wait;
    assert(_fader_process_data_wait(&f) == -1 && raised == 0 && output_count == 0 && f.first_sent);
    cleanup(&f);
}

/* Producer blocks only above max-length; sync FLUSH discards queued PCM at the next chain;
 * async FLUSH on a queue variant is forwarded and turns on input dropping. */
static void test_queue_backpressure_and_flush(void)
{
    struct fader f; setup(&f, true); struct aud_port p = {&f, 0};
    aud_data_adapter_push(&f.q, aud_data_new_zero(12));
    assert(!fader_chain(&p, aud_data_new_zero(4)) && waits == 0 && yields == 1);
    wait_action = drain_wait;
    assert(!fader_chain(&p, aud_data_new_zero(4)) && waits == 0 && QUEUED(&f) == 5);
    assert(!fader_chain(&p, aud_data_new_zero(4)) && waits == 1 && QUEUED(&f) == 1);
    assert(yields == 3);

    struct aud_event *flush = event(AUD_EVENT_FLUSH); flush->prop = AUD_EVENT_PROP_SYNC;
    fader_event(&f, &p, flush);
    assert(f.set_flush && !f.flushing && f.pe.next != &f.pe && output_count == 0);
    assert(!fader_chain(&p, aud_data_new_zero(8)) && QUEUED_BYTES(&f) == 8 && !f.set_flush);

    fader_event(&f, &p, event(AUD_EVENT_FLUSH));
    assert(f.flushing && !f.set_flush && output_count == 1 && output[0].type == AUD_EVENT_FLUSH);
    assert(fader_chain(&p, aud_data_new_zero(8)) == -1 && f.dropped_frames == 2);
    cleanup(&f);
}

/* A normal fader swallows async FLUSH; its producer never yields. */
static void test_fader_flush_not_forwarded(void)
{
    struct fader f; setup(&f, false); struct aud_port p = {&f, 0};
    fader_event(&f, &p, event(AUD_EVENT_FLUSH));
    assert(f.flushing && output_count == 0 && !f.set_flush);
    assert(fader_chain(&p, aud_data_new_zero(4)) == -1 && yields == 0);
    cleanup(&f);
}

/* Running state: in_cond is signalled at <= max-length; block-queue parks only below max. */
static void block_release(struct fader *f) { f->block = false; }
static void test_running_signal_and_block_gate(void)
{
    struct fader f; setup(&f, true); f.state = FS_RUNNING; f.stop_len = 0; f.fade_len = 0;
    for (int i = 0; i < 5; ++i) aud_data_adapter_push(&f.q, aud_data_new_zero(4));
    f.block = true; wait_action = block_release;
    _fader_process_data_running(&f);
    assert(signals == 1 && waits == 0 && output_count == 1 && f.block);
    _fader_process_data_running(&f);
    assert(signals == 2 && waits == 1 && output_count == 2 && !f.block);
    cleanup(&f);
}

/* Fade-out tail: a short chunk is zero-filled, then one zero chunk (two for Dolby). */
static void test_fadeout_tail(void)
{
    for (int dolby = 0; dolby < 2; ++dolby) {
        struct fader f; setup(&f, false); f.state = FS_FADEOUT; f.fade_pos = 1; f.dolby = dolby;
        struct aud_data *d = aud_data_new(4); memset(d->ptr, 0x91, 4); aud_data_adapter_push(&f.q, d);
        _fader_process_data_fadeout(&f);
        assert(output_count == 2 + dolby && f.state == FS_WAIT && signals == 1);
        for (unsigned j = 0; j < 16; ++j) assert(output[0].bytes[j] == (j < 4 ? 0x91 : 0));
        cleanup(&f);
    }
}

int main(void)
{
    test_pending_signed_readiness();
    test_pending_coalescing_and_positions();
    test_custom_and_last_behind_wait_audio();
    test_ready_custom_in_wait();
    test_pending_after_downstream_and_block();
    test_flush_retains_pending_distance();
    test_sync_flush_in_wait();
    test_async_flush_overtakes_pending();
    test_last_short_tail_order();
    test_last_passthrough_modes();
    test_first_held_until_threshold();
    test_wait_last_and_underflow_by_variant();
    test_queue_backpressure_and_flush();
    test_fader_flush_not_forwarded();
    test_running_signal_and_block_gate();
    test_fadeout_tail();
    puts("DSP2 fader/queue: signed pending FIFO/ownership, WAIT markers, sync/block ordering, FLUSH distances, LAST tails passed");
    return 0;
}
