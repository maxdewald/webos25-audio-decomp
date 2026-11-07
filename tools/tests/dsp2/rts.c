/* Standalone: cc -std=c11 -Wall -Wextra -Werror -pedantic tools/tests/dsp2/rts.c -o build/scratch/rts-test && build/scratch/rts-test */
#include <stdlib.h>
static void *test_calloc(size_t, size_t);
#define calloc test_calloc
#include "../../../src/dsp2/rts.c"
#undef calloc
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct event_values {
    struct aud_event event;
    const char *codec;
    int channels, bits, rate, acmod, front_padding;
    bool dolby, es;
    unsigned marker;
};
static struct event_values duplicate;
static struct aud_event rate_event;
static struct aud_data *fifo[16];
static unsigned head, tail, waits, signals, locks, releases, duplicates, event_pushes;
static unsigned data_pushes, syncs, creates, destroys, defaults, requested_rate;
static unsigned events_before_data;
static unsigned created_stack, created_priority;
static bool fail_alloc, fail_dup, fail_thread, fail_rate, stop_after_push;
static struct aud_event *last_event;
static struct aud_data *last_data;
static void *last_put;
static struct ts_module *active;
static void (*on_wait)(struct aud_cond *);
const unsigned char rts_module_registration[] = {0};

static void *test_calloc(size_t n, size_t size)
{
    return fail_alloc ? NULL : calloc(n, size);
}
void aud_mutex_init(struct aud_mutex *p) { memset(p, 0, sizeof(*p)); }
void aud_mutex_lock(struct aud_mutex *p) { (void)p; ++locks; }
void aud_mutex_unlock(struct aud_mutex *p) { (void)p; assert(locks); --locks; }
void aud_cond_init(struct aud_cond *p) { memset(p, 0, sizeof(*p)); }
void aud_cond_wait(struct aud_cond *p, struct aud_mutex *m)
{
    (void)m;
    assert(locks == 1 && on_wait);
    ++waits;
    assert(waits < 8);
    on_wait(p);
}
void aud_cond_signal(struct aud_cond *p) { (void)p; ++signals; }
void aud_data_adapter_init(struct aud_data_adapter *q)
{
    memset(q, 0, sizeof(*q));
    head = tail = 0;
}
void aud_data_adapter_cleanup(struct aud_data_adapter *q) { (void)q; }
void aud_data_adapter_flush(struct aud_data_adapter *q, unsigned n)
{
    assert(n == q->total - q->offset);
    while (head != tail)
        aud_object_put(fifo[head++]);
    q->offset += n;
}
void aud_data_adapter_push(struct aud_data_adapter *q, struct aud_data *d)
{
    assert(tail < 16);
    fifo[tail++] = d;
    q->total += d->size;
}
struct aud_data *aud_data_adapter_dequeue(struct aud_data_adapter *q)
{
    if (head == tail)
        return NULL;
    struct aud_data *d = fifo[head++];
    q->offset += d->size;
    return d;
}
void aud_object_put(void *p)
{
    ++releases;
    last_put = p;
    if (stop_after_push)
        active->running = false;
}
struct aud_event *aud_event_dup(struct aud_event *p)
{
    ++duplicates;
    if (fail_dup)
        return NULL;
    duplicate = *(struct event_values *)p;
    return &duplicate.event;
}
int aud_event_value_get_int(struct aud_event *p, const char *key, int fallback)
{
    struct event_values *e = (void *)p;
    int v = !strcmp(key, "channels") ? e->channels :
            !strcmp(key, "bits-per-sample") ? e->bits :
            !strcmp(key, "rate") ? e->rate :
            !strcmp(key, "acmod") ? e->acmod : -1;
    return v < 0 ? fallback : v;
}
void aud_event_value_set_int(struct aud_event *p, const char *key, int value)
{
    struct event_values *e = (void *)p;
    if (!strcmp(key, "front-padding"))
        e->front_padding = value;
    else {
        assert(!strcmp(key, "acmod"));
        e->acmod = value;
    }
}
void aud_event_value_set_bool(struct aud_event *p, const char *key, bool value)
{
    assert(!strcmp(key, "dolby"));
    ((struct event_values *)p)->dolby = value;
}
struct aud_event *aud_event_newv_custom(const char *key, int a, unsigned rate, int b)
{
    assert(!strcmp(key, "change-out-rate") && a == 0 && b == 0);
    requested_rate = rate;
    return fail_rate ? NULL : &rate_event;
}
int aud_module_push_event(struct ts_module *m, int port, struct aud_event *e)
{
    assert(!port && !locks);
    ++event_pushes;
    last_event = e;
    if (stop_after_push)
        m->running = false;
    return 0;
}
int aud_module_push_data(struct ts_module *m, int port, struct aud_data *d)
{
    assert(!port && !locks);
    events_before_data = event_pushes;
    ++data_pushes;
    last_data = d;
    if (stop_after_push)
        m->running = false;
    return 0;
}
void aud_module_sync(struct ts_module *m, int port) { (void)m; assert(!port); ++syncs; }
int aud_module_default_event(struct ts_module *m, struct aud_port *p, struct aud_event *e)
{
    (void)m; (void)p; (void)e;
    ++defaults;
    return 23;
}
int aud_module_default_command(struct ts_module *m, unsigned c, const unsigned *p, unsigned n)
{
    (void)m; (void)c; (void)p; (void)n;
    ++defaults;
    return 29;
}
void *aud_thread_create(const char *n, unsigned stack, unsigned priority,
                        int (*entry)(struct ts_module *), struct ts_module *m)
{
    assert(n == m->name && entry == ts_thread_work && m->running);
    ++creates;
    created_stack = stack;
    created_priority = priority;
    return fail_thread ? NULL : m;
}
void aud_thread_destroy(void *p) { assert(p == active && !active->running); ++destroys; }
void ModuleRegistry_Register(const void *p) { assert(p == rts_module_registration); }

static void reset(struct ts_module *m)
{
    memset(m, 0, sizeof(*m));
    waits = signals = locks = releases = duplicates = event_pushes = data_pushes = 0;
    syncs = creates = destroys = defaults = requested_rate = 0;
    events_before_data = 0;
    fail_alloc = fail_dup = fail_thread = fail_rate = stop_after_push = false;
    last_put = NULL;
    last_event = NULL;
    last_data = NULL;
    on_wait = NULL;
    active = m;
    m->name = "rts-test";
    assert(ts_constructor(m) == 0);
}
static struct event_values first(int channels, int bits, int rate)
{
    struct event_values e = {.event = {.type = 1}, .channels = channels,
        .codec = "pcm", .bits = bits, .rate = rate, .dolby = true,
        .es = true, .marker = 0xdecafbad};
    return e;
}
static void release_full_queue(struct aud_cond *p)
{
    assert(p == &active->space);
    active->queued_frames = active->queue_max;
}
static void start_flushing(struct aud_cond *p)
{
    assert(p == &active->space);
    active->flushing = true;
}
static void stop_worker(struct aud_cond *p)
{
    assert(p == &active->ready);
    active->running = false;
}
static void test_first(void)
{
    struct ts_module m;
    reset(&m);
    assert(m.rate == 48000 && m.frame_shift == 3 && m.queue_max == 24000);
    assert(m.output_rate[1] == 47750 && m.output_rate[2] == 48250);
    ts_set_prop_buffering_level(&m, NULL, "10");
    struct event_values e = first(6, 24, 96000);
    struct event_values old = first(2, 32, 48000);
    m.first = &old.event;
    m.first_sent = true;
    assert(ts_event(&m, NULL, &e.event) == 0);
    assert(m.first == &e.event && !m.first_sent && releases == 1 && last_put == &old.event);
    assert(m.rate == 96000 && m.frame_shift == 5 && m.queue_max == 48480);
    assert(m.buffering_frames == 480 && !event_pushes && !defaults && !duplicates);
    assert(creates == 1 && created_stack == 0x20000 && created_priority == 10);
    assert(m.running && m.thread == &m);
    assert(e.dolby && e.es && e.marker == 0xdecafbad);
    ts_event(&m, NULL, &e.event);
    assert(creates == 1 && releases == 2);
    ts_destructor(&m);
    assert(destroys == 1 && !m.thread && !m.first && !locks);

    reset(&m);
    fail_thread = true;
    e = first(-1, -1, -1);
    ts_event(&m, NULL, &e.event);
    assert(!m.running && !m.thread && m.rate == 48000 && m.frame_shift == 3);
    e = first(1, 8, 44100);
    ts_event(&m, NULL, &e.event);
    assert(m.frame_shift == 0 && m.queue_max == 22050);
    m.thread = &m;
    fail_thread = false;
    ts_event(&m, NULL, &e.event);
    assert(!m.running && creates == 2);
}
static void test_copy(void)
{
    struct ts_module m;
    reset(&m);
    struct event_values e = first(8, 16, 192000);
    m.first = &e.event;
    const unsigned frames[] = {0, 1, 255, 256, 257, 512, 513};
    const int padding[] = {512, 768, 768, 768, 1024, 1024, 1280};
    for (unsigned i = 0; i < sizeof(frames) / sizeof(frames[0]); ++i) {
        m.packet_count = i ? 3 : 0;
        m.queued_bytes = frames[i] * 3 * 8;
        m.first_sent = false;
        rts_push_event_first(&m);
        assert(last_event == &duplicate.event && m.first_sent);
        assert(duplicate.front_padding == padding[i] && duplicate.acmod == 2);
        assert(!duplicate.dolby && duplicate.es && duplicate.marker == e.marker);
        assert(duplicate.rate == e.rate && duplicate.channels == e.channels &&
               duplicate.bits == e.bits && e.dolby && e.acmod == 0 && !e.front_padding);
    }
    e.acmod = 7;
    rts_push_event_first(&m);
    assert(duplicate.acmod == 7);
    fail_dup = true;
    m.first_sent = false;
    unsigned pushed = event_pushes;
    rts_push_event_first(&m);
    assert(!m.first_sent && event_pushes == pushed && !locks);
}
static void test_chain_and_flush(void)
{
    struct ts_module m;
    reset(&m);
    struct aud_port port = {.module = &m};
    struct aud_data d = {.type = 1, .size = 8192};
    ts_chai(&port, &d);
    assert(releases == 1 && !m.packet_count && !signals);
    d.type = 0;
    m.queued_frames = m.queue_max;
    ts_chai(&port, &d);
    assert(!waits && m.packet_count == 1 && m.queued_bytes == 8192);
    assert(m.queued_frames == 25024 && m.input_count == 1);
    on_wait = release_full_queue;
    ts_chai(&port, &d);
    assert(waits == 1 && m.packet_count == 2 && m.input_count == 2);
    on_wait = start_flushing;
    ts_chai(&port, &d);
    assert(waits == 2 && releases == 2 && m.input_count == 2 && !locks);
    struct aud_event begin = {.type = 7}, end = {.type = 7, .prop = 2};
    assert(ts_event(&m, &port, &begin) == 23 && m.flushing);
    assert(ts_event(&m, &port, &end) == 0 && !m.flushing);
    assert(!m.packet_count && !m.queued_bytes && !m.queued_frames);
    assert(releases == 4 && m.events.next->event == &end && m.events.prev->event == &end);
    stop_after_push = true;
    m.running = true;
    ts_thread_work(&m);
    assert(last_event == &end && event_pushes == 1 && syncs == 1);
    assert(m.events.next == &m.events && m.events.prev == &m.events && !locks);
    fail_alloc = true;
    ts_event(&m, &port, &end);
    assert(m.events.next == &m.events && releases == 4);
    struct aud_event other = {.type = 2};
    assert(ts_event(&m, &port, &other) == 23);

    reset(&m);
    struct event_values e = first(2, 32, 48000);
    m.first = &e.event;
    m.first_sent = true;
    ts_chai(&port, &d);
    struct aud_event end2 = {.type = 7, .prop = 2};
    ts_event(&m, &port, &end);
    ts_event(&m, &port, &end2);
    assert(m.first == &e.event && m.first_sent && releases == 1);
    assert(m.events.next->event == &end && m.events.prev->event == &end2);
    ts_chai(&port, &d);
    m.running = stop_after_push = true;
    ts_thread_work(&m);
    assert(last_event == &end && !data_pushes && m.packet_count == 1);
    m.running = true;
    ts_thread_work(&m);
    assert(last_event == &end2 && event_pushes == 2 && !data_pushes);
    assert(m.events.next == &m.events && m.events.prev == &m.events);
}
static void test_worker(void)
{
    struct ts_module m;
    reset(&m);
    struct aud_port port = {.module = &m};
    struct aud_data d = {.size = 2048};
    struct event_values e = first(2, 32, 48000);
    m.first = &e.event;
    ts_chai(&port, &d);
    m.running = stop_after_push = true;
    ts_thread_work(&m);
    assert(duplicates == 1 && event_pushes == 1 && data_pushes == 1);
    assert(last_data == &d && syncs == 1 && !m.packet_count && !m.queued_frames);
    assert(m.output_count == 1 && !m.dropped_count && !locks);
    reset(&m);
    m.first = &e.event;
    ts_chai(&port, &d);
    m.running = stop_after_push = fail_dup = true;
    ts_thread_work(&m);
    assert(duplicates == 1 && !event_pushes && data_pushes == 1 && !m.first_sent);
    assert(last_data == &d && m.first == &e.event && !locks);
    reset(&m);
    m.running = true;
    on_wait = stop_worker;
    ts_thread_work(&m);
    assert(waits == 1 && m.buffering && !locks);
    reset(&m);
    ts_chai(&port, &d);
    m.running = m.buffering = true;
    m.buffering_frames = m.queued_frames;
    on_wait = stop_worker;
    ts_thread_work(&m);
    assert(waits == 1 && !data_pushes && !locks);
    reset(&m);
    ts_chai(&port, &d);
    m.running = stop_after_push = true;
    m.operation_mode = 1;
    m.skip_threshold = 0;
    m.queue.total += 8;
    m.queued_frames += 1;
    m.rate = 1;
    on_wait = stop_worker;
    ts_thread_work(&m);
    assert(m.state == 3 && m.dropped_count == 1 && releases == 1 && !data_pushes);
}
static void test_first_requires_pcm(void)
{
    const char *codecs[] = {"mat", "pcm"};
    for (unsigned codec = 0; codec < 2; ++codec) {
        struct ts_module m;
        reset(&m);
        struct aud_port port = {.module = &m};
        struct event_values e = first(8, 16, 192000);
        e.codec = codecs[codec];
        assert(ts_event(&m, &port, &e.event) == 0);
        struct aud_data es = {.type = 1, .size = 8192};
        ts_chai(&port, &es);
        on_wait = stop_worker;
        ts_thread_work(&m);
        assert(releases == 1 && last_put == &es && !m.input_count);
        assert(m.first == &e.event && !m.first_sent && !duplicates && !event_pushes);

        struct aud_data carrier = {.type = 0, .index = 7, .size = 8192};
        carrier.reserved[0] = 0x12345678;
        struct aud_data original = carrier;
        ts_chai(&port, &carrier);
        m.running = stop_after_push = true;
        ts_thread_work(&m);
        assert(events_before_data == 1 && data_pushes == 1 && last_data == &carrier);
        assert(!memcmp(&carrier, &original, sizeof(carrier)) && releases == 1);
        assert(duplicate.codec == e.codec && duplicate.es && !duplicate.dolby);
        assert(duplicate.channels == 8 && duplicate.bits == 16 && duplicate.rate == 192000);
        assert(duplicate.acmod == 2 && duplicate.front_padding == 1024);
        assert(m.first == &e.event && m.first_sent && m.input_count == 1);
        ts_destructor(&m);
        assert(releases == 2 && last_put == &e.event && !m.first);
    }
}
static void test_state_and_commands(void)
{
    struct ts_module m;
    reset(&m);
    unsigned p[] = {38400, 7680, 30720, 11520};
    ts_command(&m, 0x5d000010, p, sizeof(p));
    assert(m.fast_threshold == 100 && m.slow_threshold == 20 &&
           m.fast_recovery == 80 && m.slow_recovery == 30);
    unsigned bad[] = {1, 2, 3, 4};
    ts_command(&m, 0x5d000010, bad, sizeof(bad));
    assert(m.fast_threshold == 100);
    unsigned freq[] = {47000, 48000, 49000};
    ts_command(&m, 0x5d02000c, freq, 0);
    assert(m.output_rate[0] == 48000 && m.output_rate[1] == 47000 && m.output_rate[2] == 49000);
    freq[0] = 50000;
    ts_command(&m, 0x5d02000c, freq, 0);
    assert(m.output_rate[1] == 47000);
    unsigned skip[] = {384000, 192000};
    ts_command(&m, 0x5d110010, skip, 0);
    assert(m.skip_threshold == 1000 && m.skip_recovery == 500);
    unsigned pri = 8;
    ts_command(&m, 0x5d120004, &pri, 0);
    assert(m.priority == 10);
    pri = 9;
    ts_command(&m, 0x5d120004, &pri, 0);
    assert(m.priority == 9);
    unsigned mode = 2;
    ts_command(&m, 0x5d100004, &mode, 0);
    ts_set_prop_op_mode(&m, NULL, "0xff");
    assert(m.operation_mode == 2);
    ts_set_prop_op_mode(&m, NULL, "0x1");
    assert(m.operation_mode == 1);
    assert(ts_command(&m, 0xdeadbeef, NULL, 0) == 29);
    assert(ts_command(&m, 0x10020000, NULL, 0) == 0);
    ts_set_state(&m, -1);
    ts_set_state(&m, 4);
    ts_set_state(&m, 0);
    assert(!event_pushes);
    ts_set_state(&m, 1);
    assert(requested_rate == 47000 && event_pushes == 1 && syncs == 1);
    ts_set_state(&m, 3);
    assert(event_pushes == 1 && m.state == 3);
    fail_rate = true;
    ts_set_state(&m, 2);
    assert(event_pushes == 1 && m.state == 2);
    fail_rate = false;
    m.queue.total = 1;
    m.rate = 1000;
    m.operation_mode = 0;
    m.state = 0;
    m.queued_frames = 101;
    assert(!rts_process(&m) && m.state == 1);
    m.queued_frames = 80;
    assert(!rts_process(&m) && m.state == 1);
    m.queued_frames = 79;
    assert(!rts_process(&m) && m.state == 0);
    m.queued_frames = 19;
    assert(!rts_process(&m) && m.state == 2);
    m.queued_frames = 30;
    assert(!rts_process(&m) && m.state == 2);
    m.queued_frames = 31;
    assert(!rts_process(&m) && m.state == 0);
    m.operation_mode = 1;
    m.queued_frames = 1001;
    assert(rts_process(&m) && m.state == 3);
    m.queued_frames = 500;
    assert(rts_process(&m));
    m.queued_frames = 499;
    assert(!rts_process(&m) && m.state == 0);
    m.operation_mode = 2;
    assert(!rts_process(&m) && m.state == 1);
    m.operation_mode = 3;
    assert(!rts_process(&m) && m.state == 1);
    ts_initializer();
    assert(!locks);
}
int main(void)
{
    test_first();
    test_copy();
    test_chain_and_flush();
    test_worker();
    test_first_requires_pcm();
    test_state_and_commands();
    puts("dsp2 rts tests passed");
    return 0;
}
