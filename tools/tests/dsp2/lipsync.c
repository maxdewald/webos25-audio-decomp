/* Standalone mocked semantics, not execution of the DSP firmware.
 * macOS: cc -std=c11 -Wall -Wextra -Werror -ffunction-sections -fdata-sections \
 *   -Wl,-dead_strip tools/tests/dsp2/lipsync.c -o build/scratch/test-lipsync
 * Linux: replace -Wl,-dead_strip with -Wl,--gc-sections. Run that executable.
 */
#include "../../../src/dsp2/lipsync.c"
#include <assert.h>
#include <stdio.h>

struct first_event {
    struct aud_event event;
    const char *codec;
    int channels, bits, rate, samples, gap, seamless;
    bool explicit_values;
};
static unsigned get_count, put_count, events, data_pushes, syncs, defaults, signals;
static unsigned flushes, metadata_flushes, flushed_bytes;
static int locks, named_last;
static void *last_get, *last_put;
static struct aud_event *last_event;
static struct aud_data *last_data;
static char order[16];
static unsigned order_size;

static void record(char c)
{
    assert(order_size < sizeof(order));
    order[order_size++] = c;
}
static void reset_observations(void)
{
    get_count = put_count = events = data_pushes = syncs = defaults = signals = 0;
    flushes = metadata_flushes = flushed_bytes = 0;
    locks = named_last = 0;
    last_get = last_put = NULL;
    last_event = NULL;
    last_data = NULL;
    order_size = 0;
}
void *aud_object_get(void *p)
{
    get_count++;
    last_get = p;
    return p;
}
void aud_object_put(void *p)
{
    put_count++;
    last_put = p;
}
int aud_module_push_event(void *m, int port, struct aud_event *e)
{
    (void)m;
    assert(port == 0);
    events++;
    last_event = e;
    record('E');
    return -5;
}
int aud_module_push_data(void *m, int port, struct aud_data *d)
{
    (void)m;
    assert(port == 0);
    data_pushes++;
    last_data = d;
    record('D');
    return -5;
}
int aud_module_sync(void *m, int port)
{
    (void)m;
    assert(port == 0);
    syncs++;
    record('S');
    return -5;
}
int aud_module_default_event(void *m, struct aud_port *p, struct aud_event *e)
{
    (void)m;
    (void)p;
    (void)e;
    defaults++;
    return 23;
}
void aud_mutex_lock(void *m) { (void)m; assert(locks++ == 0); }
void aud_mutex_unlock(void *m) { (void)m; assert(--locks == 0); }
void aud_cond_signal(void *c) { (void)c; assert(locks == 1); signals++; }
void aud_data_adapter_flush(void *a, unsigned n)
{
    (void)a;
    flushes++;
    flushed_bytes = n;
}
void msf_flush_metadata(void *m) { (void)m; metadata_flushes++; }
void aimc_raise_named_event(void *imc, int uid, const char *name, void *p, int size)
{
    (void)imc;
    (void)uid;
    assert(!strcmp(name, "last") && p == NULL && size == 0);
    named_last++;
}
int aud_event_value_get_int(struct aud_event *e, const char *key, int def)
{
    struct first_event *f = (struct first_event *)e;
    assert(e->type == 1);
    if (!strcmp(key, "channels")) { assert(def == 8); return f->explicit_values ? f->channels : def; }
    if (!strcmp(key, "bits-per-sample")) { assert(def == 32); return f->explicit_values ? f->bits : def; }
    if (!strcmp(key, "rate")) { assert(def == 48000); return f->explicit_values ? f->rate : def; }
    if (!strcmp(key, "sample-number")) { assert(def == 1536); return f->explicit_values ? f->samples : def; }
    assert(!strcmp(key, "gap-correction") && def == 0);
    return f->explicit_values ? f->gap : def;
}
const char *aud_event_value_get_str(struct aud_event *e, const char *key, const char *def)
{
    struct first_event *f = (struct first_event *)e;
    assert(!strcmp(key, "codec") && !strcmp(def, "pcm"));
    return f->codec ? f->codec : def;
}
int aud_event_value_get_bool(struct aud_event *e, const char *key, int def)
{
    struct first_event *f = (struct first_event *)e;
    assert(!strcmp(key, "seamless-mode") && def == 0);
    return f->explicit_values ? f->seamless : def;
}

static void test_first(void)
{
    struct lipsync ls = {0};
    struct first_event missing = {.event = {.type = 1}};
    struct first_event pcm = {.event = {.type = 1}, .codec = "pcm", .channels = 2,
                             .bits = 16, .rate = 96000, .samples = 777,
                             .explicit_values = true};
    struct first_event es = {.event = {.type = 1}, .codec = "mat", .channels = 8,
                            .bits = 32, .rate = 44100, .samples = 1024,
                            .gap = 19, .seamless = 1, .explicit_values = true};
    struct first_event saved = es;
    reset_observations();
    assert(_lipsync_set_prop_datatype(&ls, 1) == 0);
    assert(_lipsync_event(&ls, NULL, &missing.event) == 0);
    assert(ls.channels == 8 && ls.fs == 48 && ls.bytePerSample == 4);
    assert(ls.dataType == 0 && ls.is_pcm == 1 && ls.nSamplePerFrame == 256);
    assert(ls.first == &missing.event && ls.first_received && !ls.first_sent);
    assert(!get_count && !put_count && !events && !defaults && !flushes && signals == 1);

    assert(_lipsync_event(&ls, NULL, &pcm.event) == 0);
    assert(put_count == 1 && last_put == &missing.event && !get_count && !events);
    assert(ls.channels == 2 && ls.fs == 96 && ls.bytePerSample == 4);
    assert(ls.nSamplePerFrame == 256 && ls.is_pcm == 1 && ls.dataType == 0);

    reset_observations();
    ls.first_sent = 1;
    assert(_lipsync_event(&ls, NULL, &es.event) == 0);
    assert(ls.first == &es.event && ls.first_received && ls.first_sent == 1);
    assert(ls.channels == 8 && ls.fs == 44 && ls.bytePerSample == 2);
    assert(ls.nSamplePerFrame == 1024 && !ls.is_pcm && ls.dataType == 1);
    assert(ls.print_interval == (44u * 2000u) / 1024u);
    assert(ls.gap_correction == 19 && ls.seamless);
    assert(put_count == 1 && last_put == &pcm.event && get_count == 1 && last_get == &es.event);
    assert(events == 1 && last_event == &es.event && !flushes);
    assert(!memcmp(&es, &saved, sizeof(es)));

    es.codec = "PCM";
    es.explicit_values = false;
    ls.first = NULL;
    ls.first_sent = 0;
    assert(_lipsync_event(&ls, NULL, &es.event) == 0);
    assert(ls.dataType == 1 && ls.bytePerSample == 2 && ls.nSamplePerFrame == 1536);
    assert(ls.print_interval == (48u * 2000u) / 1536u);
}

static void test_output(void)
{
    unsigned char payload[67], saved[sizeof(payload)];
    struct first_event first = {.event = {.type = 1}, .codec = "pcm"};
    struct lipsync ls = {.first = &first.event, .dataType = 1, .out_ts = 0x12345678};
    struct aud_data d = {.type = 3, .timestamp = 19, .size = sizeof(payload), .buf = payload};
    struct aud_data original;
    for (unsigned i = 0; i < sizeof(payload); ++i)
        payload[i] = (unsigned char)(i * 37u);
    memcpy(saved, payload, sizeof(saved));
    reset_observations();
    assert(_lipsync_write_output(&ls, &d) == 0);
    assert(d.type == 1 && d.timestamp == ls.out_ts && d.buf == payload && d.size == sizeof(payload));
    assert(!memcmp(payload, saved, sizeof(payload)));
    assert(ls.first_sent && get_count == 1 && last_get == &first.event);
    assert(events == 1 && last_event == &first.event && last_data == &d);
    assert(data_pushes == 1 && syncs == 1 && !put_count);
    assert(order_size == 3 && !memcmp(order, "EDS", 3));

    reset_observations();
    ls.dataType = 2;
    assert(_lipsync_write_output(&ls, &d) == 0 && d.type == 0);
    assert(!events && !get_count && data_pushes == 1 && syncs == 1);
    assert(!memcmp(payload, saved, sizeof(payload)));

    reset_observations();
    ls.autotest_mode = 1;
    assert(_lipsync_write_output(&ls, &d) == 0);
    for (unsigned i = 0; i < sizeof(payload); ++i)
        assert(payload[i] == 0x1f);
    assert(d.buf == payload && d.size == sizeof(payload));

    for (int protection = 0; protection < 2; ++protection) {
        memcpy(payload, saved, sizeof(payload));
        ls.over_protection = protection;
        ls.drop_count = protection ? 0 : 1;
        ls.first_sent = 0;
        original = d;
        reset_observations();
        assert(_lipsync_write_output(&ls, &d) == 0);
        assert(put_count == 1 && last_put == &d && !events && !data_pushes && !syncs && !get_count);
        assert(!ls.first_sent && !memcmp(&d, &original, sizeof(d)));
        assert(!memcmp(payload, saved, sizeof(payload)));
    }
    ls.over_protection = 0;
    ls.drop_count = -1;
    ls.autotest_mode = 0;
    ls.first = NULL;
    reset_observations();
    assert(_lipsync_write_output(&ls, &d) == 0);
    assert(data_pushes == 1 && syncs == 1 && !events && !ls.first_sent);
}

static void test_metadata_first(void)
{
    struct aud_event first = {.type = 1};
    struct lipsync ls = {.first = &first, .dataType = 1};
    reset_observations();
    _lipsync_write_metadata(&ls, 1536, 1);
    assert(!events && !ls.first_sent);
    ls.drop_count = 1;
    _lipsync_write_metadata(&ls, 1536, 0);
    assert(!events);
    ls.drop_count = 0;
    ls.over_protection = 1;
    _lipsync_write_metadata(&ls, 1536, 0);
    assert(!events);
    ls.over_protection = 0;
    _lipsync_write_metadata(&ls, 1536, 0);
    assert(events == 1 && get_count == 1 && ls.first_sent && last_event == &first);
    _lipsync_write_metadata(&ls, 1536, 0);
    assert(events == 1 && !data_pushes && !syncs);
}

static void test_datatype(void)
{
    struct first_event first = {.event = {.type = 1}, .codec = "pcm", .bits = 32};
    struct first_event saved = first;
    struct lipsync ls = {.first = &first.event, .nSamplePerFrame = 777,
                         .print_interval = 33, .channels = 8, .fs = 192};
    const int values[] = {1, 0, 2, -1};
    reset_observations();
    for (unsigned i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        int v = values[i];
        assert(_lipsync_set_prop_datatype(&ls, v) == 0);
        assert(ls.dataType == v && ls.bytePerSample == (v == 1 ? 2 : 4));
        assert(ls.is_pcm == (v != 1));
        assert(ls.nSamplePerFrame == 777 && ls.print_interval == 33);
        assert(ls.channels == 8 && ls.fs == 192 && ls.first == &first.event);
        assert(!ls.first_received && !ls.first_sent && !events && !get_count && !put_count);
        assert(!memcmp(&first, &saved, sizeof(first)));
    }
}

static void test_last_and_flush(void)
{
    struct aud_event first = {.type = 1}, last = {.type = 2};
    struct aud_event flush = {.type = 7}, other = {.type = 6, .flags = 2};
    struct lipsync ls = {.first = &first, .start_pts = 11, .end_pts = 22,
                         .gap_handling = 1, .gap_correction = 99,
                         .adapter_in = 50, .adapter_out = 7};
    reset_observations();
    assert(_lipsync_event(&ls, NULL, &last) == 0);
    assert(named_last == 1 && put_count == 1 && last_put == &last && !defaults);
    assert(ls.first == &first && ls.start_pts == 11 && ls.end_pts == 22 && ls.gap_correction == 99);
    reset_observations();
    ls.first_sent = 1;
    assert(_lipsync_event(&ls, NULL, &last) == 23);
    assert(named_last == 1 && put_count == 1 && last_put == &first && defaults == 1);
    assert(!ls.first && !ls.first_sent && !ls.start_pts && !ls.end_pts);
    assert(!ls.gap_handling && !ls.gap_correction);

    reset_observations();
    assert(_lipsync_event(&ls, NULL, &flush) == 23);
    assert(ls.flush_async && !flushes && !metadata_flushes && defaults == 1);
    reset_observations();
    ls.flush_async = false;
    ls.first = &first;
    ls.first_sent = 1;
    ls.start_pts = ls.end_pts = ls.gap_handling = 1;
    flush.flags = 2;
    assert(_lipsync_event(&ls, NULL, &flush) == 23);
    assert(!ls.flush_async && flushes == 1 && flushed_bytes == 43 && metadata_flushes == 1);
    assert(!ls.start_pts && !ls.end_pts && !ls.gap_handling && !ls.first_sent);
    assert(ls.first == &first && !put_count && defaults == 1);
    ls.flush_async = true;
    assert(_lipsync_event(&ls, NULL, &flush) == 23 && ls.flush_async);
    reset_observations();
    assert(_lipsync_event(&ls, NULL, &other) == 23);
    assert(!flushes && !metadata_flushes && !put_count && defaults == 1 && signals == 1);
}

int main(void)
{
    _Static_assert(offsetof(struct aud_event, flags) == 0x10, "event flags offset");
    test_first();
    test_output();
    test_metadata_first();
    test_datatype();
    test_last_and_flush();
    puts("lipsync: FIRST ownership/selection, output bytes/type, internal datatype and FLUSH/LAST: PASS");
    return 0;
}
