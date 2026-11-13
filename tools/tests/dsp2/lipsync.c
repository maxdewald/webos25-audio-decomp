/* Extracted host semantics with adapter/register boundaries, not firmware execution. */
#include "../../../src/dsp2/lipsync.c"
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static uint32_t gstc_values[0x228 / 4], stc_values[0x90 / 4];
volatile uint32_t *gstc_regs = gstc_values, *stc_regs = stc_values;
// @match src/dsp2/renderer_path.c enum lipsync_clock_type \{[\s\S]*?\};
// @match src/dsp2/renderer_path.c #define PTS_MASK_\w+ .*
// @extract src/dsp2/renderer_path.c _lipsync_get_stc _lipsync_get_pts _lipsync_get_maxpts

struct first_event {
    struct aud_event event;
    const char *codec;
    int channels, bits, rate, samples, gap, seamless;
    int data_rate, data_channels, data_bps, acmod;
    bool es, clock_x4;
    bool explicit_values;
};
static unsigned get_count, put_count, events, data_pushes, syncs, defaults, signals;
static unsigned flushes, metadata_flushes, flushed_bytes;
static int locks, named_last, named_master;
static int32_t master_base[2];
static void *last_get, *last_put;
static struct aud_event *last_event;
static struct aud_data *last_data;
static char order[16];
static unsigned order_size;
static int selected_action;
static struct lipsync *adapter_owner;
static struct aud_data frame_data;
static bool fail_frame;
static unsigned fadeins, fadeouts;
static bool active_output;
static uint32_t adapter_timestamp;
static bool have_timestamp;
static unsigned waits;
static int wait_timeout;
static bool repeat_wait, advance_wait;

uint32_t _lipsync_media_trick_unrecovered(struct lipsync *ls, uint32_t pts)
{
    assert(!"unrecovered trick-speed clock");
    return 0;
}
int _lipsync_core_clock_unrecovered(struct lipsync *ls, uint32_t pts)
{
    assert(!"unrecovered clock predicate");
    return -1;
}

static int selected_state(struct lipsync *ls) { (void)ls; return selected_action; }
#define _lipsync_state clock_lipsync_state
// @extract src/dsp2/lipsync.c _lipsync_state
#undef _lipsync_state
#define _lipsync_state clock_lipsync_state
#define _lipsync_core clock_lipsync_core
// @extract src/dsp2/lipsync.c _lipsync_core
#undef _lipsync_core
#undef _lipsync_state
#define _lipsync_state selected_state
#define _lipsync_core test_lipsync_core
// @extract src/dsp2/lipsync.c _lipsync_core
#undef _lipsync_core
#undef _lipsync_state
#define _lipsync_chain test_lipsync_chain
#define _lipsync_core test_lipsync_core
// @extract src/dsp2/lipsync.c _lipsync_chain
#undef _lipsync_core
#undef _lipsync_chain

static void record(char c)
{
    assert(order_size < sizeof(order));
    order[order_size++] = c;
}
static void reset_observations(void)
{
    get_count = put_count = events = data_pushes = syncs = defaults = signals = 0;
    flushes = metadata_flushes = flushed_bytes = 0;
    locks = named_last = named_master = 0;
    last_get = last_put = NULL;
    last_event = NULL;
    last_data = NULL;
    order_size = 0;
    adapter_owner = NULL;
    fail_frame = false;
    fadeins = fadeouts = 0;
    active_output = true;
    adapter_timestamp = 42;
    have_timestamp = true;
    waits = 0;
    wait_timeout = 0;
    repeat_wait = advance_wait = false;
    memset(gstc_values, 0, sizeof(gstc_values));
    memset(stc_values, 0, sizeof(stc_values));
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
int aud_module_default_command(void *m, unsigned command, void *param, unsigned size)
{
    (void)m; (void)command; (void)param; (void)size;
    defaults++;
    return 29;
}
int aud_module_have_active_output(void *module) { (void)module; return active_output; }
int aud_data_adapter_push(void *adapter, struct aud_data *data)
{
    assert(adapter_owner && adapter == adapter_owner->adapter);
    adapter_owner->adapter_in += data->size;
    return 0;
}
int msf_process_metadata(void *metadata, void *buf, unsigned size)
{
    (void)metadata; (void)buf; (void)size;
    return 0;
}
void aud_mutex_lock(void *m) { (void)m; assert(locks++ == 0); }
void aud_mutex_unlock(void *m) { (void)m; assert(--locks == 0); }
void aud_cond_signal(void *c) { (void)c; assert(locks == 1); signals++; }
void aud_cond_wait_timeout(void *cond, void *mutex, int us)
{
    struct lipsync *ls = (void *)((char *)mutex - offsetof(struct lipsync, mutex));
    assert(locks == 1 && cond == ls->cond);
    waits++;
    wait_timeout = us;
    if (advance_wait)
        stc_values[0x84 / 4] += 900;
    else if (!repeat_wait)
        ls->flush_async = true;
}
void aud_data_adapter_flush(void *a, unsigned n)
{
    (void)a;
    flushes++;
    flushed_bytes = n;
    if (adapter_owner) {
        assert(a == adapter_owner->adapter);
        assert(n <= adapter_owner->adapter_in - adapter_owner->adapter_out);
        adapter_owner->adapter_out += n;
    }
}
struct aud_data *aud_data_adapter_get_data(void *adapter, unsigned size)
{
    assert(adapter_owner && adapter == adapter_owner->adapter);
    assert(size <= adapter_owner->adapter_in - adapter_owner->adapter_out);
    frame_data.size = size;
    return fail_frame ? NULL : &frame_data;
}
struct aud_data *aud_data_adapter_peep(void *adapter)
{
    static struct aud_data queued;
    (void)adapter;
    queued.timestamp = adapter_timestamp;
    return have_timestamp ? &queued : NULL;
}
void lipsync_fadein(int channels, void *buf, const void *src, unsigned size)
{
    assert(channels == 8 && size && buf && src == buf);
    ++fadeins;
    memset(buf, 0x11, size);
}
void lipsync_fadeout(int channels, void *buf, const void *src, unsigned size)
{
    assert(channels == 8 && size && buf && src == buf);
    ++fadeouts;
    memset(buf, 0x22, size);
}
void _lipsync_write_lipsync_state(struct lipsync *ls, int action)
{
    (void)ls;
    (void)action;
}
void *IMC_GetLocalImc(int index) { assert(!index); return NULL; }
int IMC_RaiseEvent(void *imc, unsigned event, void *dst, void *param)
{
    (void)imc; (void)dst;
    assert(event == LIPSYNC_EVT_FIRST_OUTPUT && param);
    return 0;
}
void msf_flush_metadata(void *m) { (void)m; metadata_flushes++; }
void aimc_raise_named_event(void *imc, int uid, const char *name, void *p, int size)
{
    (void)imc;
    (void)uid;
    if (!strcmp(name, "adec_master_info")) {
        assert(p && size == sizeof(master_base));
        memcpy(master_base, p, sizeof(master_base));
        named_master++;
        return;
    }
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

static void test_frame_dispatch(void)
{
    unsigned char payload[64], saved[sizeof(payload)];
    memset(saved, 0xa5, sizeof(saved));
    const int actions[] = {LS_OFF, LS_IDLE, LS_SKIP, LS_TONORM, LS_E_NORM};
    for (unsigned index = 0; index < sizeof(actions) / sizeof(actions[0]); ++index) {
        for (int datatype = 0; datatype < 2; ++datatype) {
            struct aud_event first = {.type = 1};
            struct lipsync ls = {.channels = 8, .nSamplePerFrame = 2,
                .bytePerSample = datatype == 1 ? 2 : 4, .dataType = datatype,
                .first = &first, .out_ts = 42};
            unsigned frame = 2u * 8u * (unsigned)ls.bytePerSample;
            selected_action = actions[index];
            reset_observations();
            adapter_owner = &ls;
            ls.adapter_in = frame - 1;
            frame_data = (struct aud_data){.type = datatype == 1 ? 0 : 1,
                                          .buf = payload};
            memcpy(payload, saved, sizeof(payload));
            assert(test_lipsync_core(&ls) == 1);
            assert(!ls.adapter_out && !ls.first_sent && !events && !data_pushes);
            assert(!flushes && !put_count && !fadeins && !fadeouts && ls.print_cnt == 1);
            assert(!memcmp(payload, saved, sizeof(payload)));
            ls.adapter_in++;
            assert(test_lipsync_core(&ls) == 0);
            assert(ls.adapter_out == frame && ls.bytes_in == frame);
            assert(ls.action == selected_action);
            if (selected_action == LS_IDLE || selected_action == LS_SKIP) {
                assert(!events && !data_pushes && !ls.first_sent);
            } else {
                assert(events == 1 && data_pushes == 1 && ls.first_sent);
                assert(frame_data.type == datatype && frame_data.size == frame);
                assert(frame_data.timestamp == 42 && last_data == &frame_data);
            }
            assert(fadeins == (unsigned)(datatype == 0 && selected_action == LS_TONORM));
            assert(fadeouts == (unsigned)(datatype == 0 && selected_action == LS_E_NORM));
            if (datatype == 1 || (!fadeins && !fadeouts))
                assert(!memcmp(payload, saved, sizeof(payload)));
        }
    }
    const int held[] = {LS_RUN, LS_REPEAT, 14, -1};
    for (unsigned index = 0; index < sizeof(held) / sizeof(held[0]); ++index) {
        struct lipsync ls = {.channels = 8, .nSamplePerFrame = 2, .bytePerSample = 2};
        reset_observations();
        selected_action = held[index];
        assert(test_lipsync_core(&ls) == 0);
        assert(!flushes && !data_pushes && !events && !put_count && !ls.adapter_out);
    }
    struct lipsync ls = {.channels = 8, .nSamplePerFrame = 2, .bytePerSample = 2,
                         .adapter_in = 32};
    reset_observations();
    adapter_owner = &ls;
    selected_action = LS_OFF;
    fail_frame = true;
    assert(test_lipsync_core(&ls) == 1 && !flushes && !put_count && !data_pushes);
    assert(ls.adapter_out == 0 && ls.adapter_in == 32);
}

static void test_core_timestamp(void)
{
    struct lipsync ls = {.channels = 8, .fs = 48, .bytePerSample = 2,
        .nSamplePerFrame = 2, .action = LS_REPEAT, .adapter_out = 1536};
    reset_observations();
    selected_action = LS_REPEAT;
    adapter_timestamp = UINT32_MAX - 3u;
    assert(test_lipsync_core(&ls) == 0);
    assert(ls.out_ts == UINT32_MAX - 3u && ls.adapter_out == 1536);
    have_timestamp = false;
    assert(test_lipsync_core(&ls) == 0 && ls.out_ts == 0x0fffffffu);
    assert(!flushes && !data_pushes && !events);
}

static void test_first_master_baseline(void)
{
    const int clocks[] = {0x11, 0x200};
    const struct { int sync; bool seamless; uint32_t pts; int32_t base; } cases[] = {
        {0, false, 0xfffffff0u, 164},
        {1, true, 0xfffffff0u, 164},
        {0, true, 0x80000100u, (int32_t)0x800001b4u},
        {1, false, 0x80000100u, (int32_t)0x800001b4u}
    };
    for (unsigned index = 0; index < sizeof(clocks) / sizeof(clocks[0]); ++index) {
        for (unsigned sample = 0; sample < sizeof(cases) / sizeof(cases[0]); ++sample) {
            struct lipsync ls = {.channels = 8, .fs = 48, .bytePerSample = 2,
                .nSamplePerFrame = 2, .adapter_in = 1600, .adapter_out = 1536,
                .action = LS_SKIP, .clockType = clocks[index], .unknown_190 = 1,
                .unknown_19c = 7, .LipsyncOnOff = cases[sample].sync,
                .seamless = cases[sample].seamless,
                .BaseST = -1, .BaseCT = -1, .diff = 123, .last_stc = 456,
                .drop_count = 3, .bytes_in = 17, .bytes_out = 19, .print_cnt = 23};
            reset_observations();
            adapter_timestamp = cases[sample].pts;
            stc_values[0x84 / 4] = 0x80000123u;
            assert(clock_lipsync_core(&ls) == 1);
            assert(!ls.unknown_190 && named_master == 1);
            assert(master_base[0] == cases[sample].base &&
                   (uint32_t)master_base[1] == 0x80000123u);
            assert(ls.BaseST_copy == cases[sample].base &&
                   (uint32_t)ls.BaseCT_copy == 0x80000123u);
            assert(ls.BaseST == (clocks[index] == 0x11 ? -1 :
                   (int32_t)((uint32_t)cases[sample].base & 0x7fffffffu)));
            assert(ls.BaseCT == (clocks[index] == 0x11 ? -1 : 0x123));
            assert(ls.unknown_19c == (clocks[index] == 0x11 ? 7 : 1));
            assert(ls.out_ts == adapter_timestamp && ls.adapter_out == 1536);
            assert(ls.action == LS_SKIP && ls.diff == 123 && ls.last_stc == 456);
            assert(ls.drop_count == 3 && ls.bytes_in == 17 && ls.bytes_out == 19);
            assert(ls.print_cnt == 23 && !waits && !flushes && !data_pushes && !events);
            assert(!ls.first_output_sent && !get_count && !put_count && !locks);
            if (clocks[index] == 0x11) {
                ls.LipsyncOnOff = 1;
                assert(clock_lipsync_core(&ls) == 12 && named_master == 1);
            }
        }
    }
}

static void test_clock_difference(void)
{
    const struct { int clock; uint32_t pts, stc; int offset, base_ct, base_st;
                   unsigned consumed; int32_t expected; } cases[] = {
        {0x100, 10000, 5000, 0, 1000, 60, 0, 0},
        {0x101, 20000, 10000, 2, 0, 0, 1536, 4420},
        {1, 0x08001733, 0, 0, 0, 0, 0, 0x07ffffff},
        {1, 0x08001734, 0, 0, 0, 0, 0, -0x07ffffff},
        {2, 5940, 0x0fffffff, 0, 0, 0, 0, -0x0fffffff},
        {2, 5939, 0x0fffffff, 0, 0, 0, 0, -1},
        {0x11, 0x40001734, 0, 0, 0, 0, 0, -0x3fffffff},
        {0x100, UINT32_MAX, 0, 0, 100, 99, 0, -5940}
    };
    for (unsigned index = 0; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        struct lipsync ls = {.channels = 8, .fs = 48, .bytePerSample = 2,
            .nSamplePerFrame = 2, .action = LS_REPEAT, .clockType = cases[index].clock,
            .offset = cases[index].offset, .BaseCT = cases[index].base_ct,
            .BaseST = cases[index].base_st, .adapter_out = cases[index].consumed};
        reset_observations();
        selected_action = LS_REPEAT;
        adapter_timestamp = cases[index].pts;
        gstc_values[0x220 / 4] = gstc_values[0x224 / 4] = cases[index].stc;
        stc_values[0x84 / 4] = stc_values[0x8c / 4] = cases[index].stc;
        assert(test_lipsync_core(&ls) == 0);
        assert(ls.diff == cases[index].expected && ls.last_stc == cases[index].stc);
        assert(ls.out_ts == cases[index].pts && !flushes && !events && !data_pushes);
    }
}

static void test_clock_to_action(void)
{
    const struct { int previous; int32_t diff; int count, expected;
                   int expected_repeat, expected_skip; } cases[] = {
        {LS_NORMAL, 900, 0, LS_NORMAL, 0, 0},
        {LS_NORMAL, 901, 4, LS_NORMAL, 0, 5},
        {LS_NORMAL, 901, 5, LS_REPEAT, 0, 0},
        {LS_NORMAL, 1801, 5, LS_FREE_RUN, 0, 0},
        {LS_NORMAL, -450, 0, LS_NORMAL, 0, 0},
        {LS_NORMAL, -451, 199, LS_NORMAL, 200, 0},
        {LS_NORMAL, -451, 200, LS_SKIP, 0, 0},
        {LS_SKIP, 1, 0, LS_TONORM, 0, 0},
        {LS_SKIP, -1799, 0, LS_SKIP, 1, 0},
        {LS_SKIP, -1801, 0, LS_FREE_RUN, 0, 0},
        {LS_SKIP, 0, 200, LS_EXCEPT, 201, 0},
        {LS_REPEAT, 0, 0, LS_NORMAL, 0, 0},
        {LS_REPEAT, 1801, 0, LS_FREE_RUN, 0, 0},
        {LS_FREE_RUN, 899, 0, LS_NORMAL, 0, 0},
        {LS_FREE_RUN, 900, 0, LS_REPEAT, 0, 0},
        {LS_FREE_RUN, -449, 0, LS_NORMAL, 0, 0},
        {LS_FREE_RUN, -450, 0, LS_SKIP, 0, 0},
        {LS_FREE_RUN, -1800, 0, LS_FREE_RUN, 0, 0},
        {LS_TONORM, -451, 0, LS_E_NORM, 0, 0},
        {LS_E_NORM, -450, 0, LS_TONORM, 0, 0},
        {LS_E_NORM, -451, 0, LS_SKIP, 0, 0},
        {LS_IDLE, 0, 0, LS_TONORM, 0, 0},
        {LS_IDLE, -1800, 0, LS_SKIP, 0, 0},
        {LS_IDLE, -1801, 0, LS_FREE_RUN, 0, 0}
    };
    unsigned char payload[32];
    struct aud_event first = {.type = 1};
    for (unsigned index = 0; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        struct lipsync ls = {.channels = 8, .fs = 48, .bytePerSample = 2,
            .nSamplePerFrame = 2, .frame_bytes = 32, .adapter_in = 32,
            .first = &first, .dataType = 1, .LipsyncOnOff = 1, .clockType = 0x100,
            .upperbound = 10, .lowerbound = 5, .freerunupperbound = 20,
            .freerunlowerbound = 20, .idle_time = 1, .action = cases[index].previous};
        reset_observations();
        adapter_owner = &ls;
        frame_data = (struct aud_data){.buf = payload};
        ls.repeat_cnt = cases[index].previous == LS_NORMAL && cases[index].diff > 0 ?
            0 : cases[index].count;
        ls.skip_cnt = cases[index].previous == LS_NORMAL && cases[index].diff > 0 ?
            cases[index].count : 0;
        adapter_timestamp = (uint32_t)(10000 + 5940 + cases[index].diff);
        stc_values[0x84 / 4] = 10000;
        assert(clock_lipsync_core(&ls) == 0);
        assert(ls.diff == cases[index].diff && ls.action == cases[index].expected);
        assert(ls.repeat_cnt == cases[index].expected_repeat);
        assert(ls.skip_cnt == cases[index].expected_skip);
        if (ls.action == LS_REPEAT) {
            assert(!ls.adapter_out && !events && !data_pushes && !ls.first_sent);
            assert(ls.repeat_stc == 10000);
        } else if (ls.action == LS_SKIP || ls.action == LS_IDLE) {
            assert(ls.adapter_out == 32 && !events && !data_pushes && !ls.first_sent);
        } else {
            assert(ls.adapter_out == 32 && events == 1 && data_pushes == 1);
            assert(ls.first_sent && frame_data.timestamp == adapter_timestamp);
            assert(frame_data.type == 1 && !fadeins && !fadeouts);
        }
    }
}

static void test_clock_reset_boundaries(void)
{
    unsigned char payload[32];
    struct aud_event first = {.type = 1};
    const struct { uint32_t previous_stc, stc, previous_pts, pts;
                   int pcr_count, expected_count, expected_disc, expected; } cases[] = {
        {10000, 55000, 0, 55000 + 5940 + 901, 0, 0, 0, LS_NORMAL},
        {10000, 55001, 0, 55001 + 5940 + 901, 0, 0, 1, LS_IDLE},
        {10000, 55001, 0, 55001 + 5940 + 900, 0, 0, 0, LS_NORMAL},
        {20000, 19000, 26000, 25000, 0, 0, 1, LS_IDLE},
        {20000, 19000, 25000, 25000, 0, 99, 0, LS_NORMAL},
        {20000, 19000, 26000, 25000, 1, 0, 1, LS_IDLE},
        {10000, 10000, 16000, 15999, 0, 0, 0, LS_NORMAL}
    };
    for (unsigned index = 0; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        struct lipsync ls = {.channels = 8, .fs = 48, .bytePerSample = 2,
            .nSamplePerFrame = 2, .adapter_in = 32, .dataType = 1, .first = &first,
            .LipsyncOnOff = 1, .clockType = 0x100, .upperbound = 10, .lowerbound = 5,
            .action = LS_NORMAL, .last_stc = cases[index].previous_stc,
            .last_pts = cases[index].previous_pts, .pcr_change_count = cases[index].pcr_count};
        reset_observations();
        adapter_owner = &ls;
        frame_data = (struct aud_data){.buf = payload};
        adapter_timestamp = cases[index].pts;
        stc_values[0x84 / 4] = cases[index].stc;
        assert(clock_lipsync_core(&ls) == 0);
        assert(ls.action == cases[index].expected && ls.discontinuity == cases[index].expected_disc);
        assert(ls.pcr_change_count == cases[index].expected_count && ls.last_pts == cases[index].pts);
        assert(ls.last_stc == cases[index].stc && ls.adapter_out == 32);
        assert(data_pushes == (unsigned)(ls.action != LS_IDLE));
        if (ls.action == LS_IDLE)
            assert(!ls.idle_cnt && !events && !ls.first_sent);
    }
    struct lipsync ls = {.channels = 8, .fs = 48, .bytePerSample = 2,
        .nSamplePerFrame = 2, .frame_bytes = 32, .adapter_in = 31, .dataType = 1,
        .first = &first, .LipsyncOnOff = 1, .clockType = 0x100, .upperbound = 10,
        .lowerbound = 5, .freerunupperbound = 20, .freerunlowerbound = 20, .idle_time = 2,
        .action = LS_REPEAT, .idle_cnt = 9, .discard_param = 2};
    reset_observations();
    adapter_owner = &ls;
    frame_data = (struct aud_data){.buf = payload};
    adapter_timestamp = 15000;
    stc_values[0x84 / 4] = 10000;
    int32_t base[] = {0, 0};
    assert(_lipsync_cmd(&ls, 0x52030008, base, sizeof(base)) == 0 && !ls.state_changed);
    base[0] = 1;
    assert(_lipsync_cmd(&ls, 0x52030008, base, sizeof(base)) == 0 && ls.state_changed);
    assert(clock_lipsync_core(&ls) == 1);
    assert(ls.action == LS_IDLE && ls.idle_cnt == 1 && ls.drop_count == 2 && !ls.state_changed);
    assert(!ls.adapter_out && !events && !data_pushes && !ls.first_sent);
    ls.adapter_in = 32;
    assert(clock_lipsync_core(&ls) == 0 && ls.action == LS_SKIP && ls.adapter_out == 32);
    assert(ls.drop_count == 1 && !events && !data_pushes);
    ls.adapter_in += 32;
    adapter_timestamp = 15937;
    assert(clock_lipsync_core(&ls) == 0 && ls.action == LS_TONORM);
    assert(!ls.drop_count && !events && !data_pushes && put_count == 1);
    ls.adapter_in += 32;
    assert(clock_lipsync_core(&ls) == 0 && ls.action == LS_NORMAL);
    assert(ls.first_sent && events == 1 && data_pushes == 1);
    assert(frame_data.timestamp == adapter_timestamp && !fadeins && !fadeouts);

    reset_observations();
    adapter_owner = &ls;
    ls.BaseST = -1;
    unsigned consumed = ls.adapter_out;
    assert(clock_lipsync_core(&ls) == 12 && ls.adapter_out == consumed);
    assert(!events && !data_pushes && !flushes);
    ls.dataType = 0;
    assert(clock_lipsync_core(&ls) == 12 && ls.adapter_out == consumed);
    ls.LipsyncOnOff = 0;
    ls.dataType = 1;
    ls.adapter_in += 32;
    assert(clock_lipsync_core(&ls) == 0 && ls.action == LS_OFF);
    assert(ls.adapter_out == consumed + 32 && data_pushes == 1);

    const struct { uint32_t pts, stc; int32_t expected; } wraps[] = {
        {0x08001733, 0, 0x07ffffff}, {0x08001734, 0, -0x07ffffff},
        {5940, 0x0fffffff, -0x0fffffff}, {5939, 0x0fffffff, -1}
    };
    for (unsigned index = 0; index < sizeof(wraps) / sizeof(wraps[0]); ++index) {
        ls = (struct lipsync){.channels = 8, .fs = 48, .bytePerSample = 2,
            .nSamplePerFrame = 2, .adapter_in = 32, .dataType = 1, .first = &first,
            .LipsyncOnOff = 1, .clockType = 1, .upperbound = 10, .lowerbound = 5,
            .action = LS_NORMAL, .last_stc = wraps[index].stc};
        reset_observations();
        adapter_owner = &ls;
        frame_data = (struct aud_data){.buf = payload};
        adapter_timestamp = wraps[index].pts;
        gstc_values[0x220 / 4] = wraps[index].stc;
        assert(clock_lipsync_core(&ls) == 0 && ls.action == LS_NORMAL);
        assert(ls.diff == wraps[index].expected && data_pushes == 1);
    }
}

static void test_clock_counters(void)
{
    const struct { int clock, transition, total, repeat, skip;
                   int32_t diff; int expected, final_repeat, final_skip; } cases[] = {
        {0x100, 2, 0, 0, 1, 901, LS_NORMAL, 0, 2},
        {0x100, 2, 0, 0, 2, 901, LS_REPEAT, 0, 0},
        {1, 0, 0, 0, 199, 901, LS_NORMAL, 0, 200},
        {1, 0, 0, 0, 200, 901, LS_REPEAT, 0, 0},
        {0x101, 0, -2, 0, 9, 901, LS_NORMAL, 0, 10},
        {0x101, 0, -2, 0, 10, 901, LS_REPEAT, 0, 0},
        {0x100, 0, 0, 0, INT32_MAX, 901, LS_NORMAL, 0, INT32_MIN},
        {0x100, 0, 0, INT32_MAX, 0, -451, LS_NORMAL, INT32_MIN, 0},
        {0x100, 1, 0, 1, 0, -451, LS_SKIP, 0, 0}
    };
    unsigned char payload[32];
    for (unsigned index = 0; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        struct lipsync ls = {.channels = 8, .fs = 48, .bytePerSample = 2,
            .nSamplePerFrame = 2, .adapter_in = 32, .frame_bytes = 32, .dataType = 1,
            .LipsyncOnOff = 1, .clockType = cases[index].clock, .upperbound = 10,
            .lowerbound = 5, .freerunupperbound = 20, .action = LS_NORMAL,
            .count_transition = cases[index].transition, .idle_total = cases[index].total,
            .repeat_cnt = cases[index].repeat, .skip_cnt = cases[index].skip};
        reset_observations();
        adapter_owner = &ls;
        frame_data = (struct aud_data){.buf = payload};
        adapter_timestamp = (uint32_t)(15940 + cases[index].diff);
        gstc_values[0x220 / 4] = stc_values[0x84 / 4] = stc_values[0x8c / 4] = 10000;
        assert(clock_lipsync_core(&ls) == 0 && ls.action == cases[index].expected);
        assert(ls.repeat_cnt == cases[index].final_repeat && ls.skip_cnt == cases[index].final_skip);
    }
    struct lipsync ls = {.channels = 8, .fs = 48, .bytePerSample = 2,
        .nSamplePerFrame = 2, .adapter_in = 31, .frame_bytes = 32, .dataType = 1,
        .LipsyncOnOff = 1, .clockType = 0x100, .upperbound = 10, .lowerbound = 5,
        .action = LS_NORMAL, .repeat_cnt = 200};
    reset_observations();
    adapter_owner = &ls;
    adapter_timestamp = 15489;
    stc_values[0x84 / 4] = 10000;
    assert(clock_lipsync_core(&ls) == 1 && !ls.repeat_cnt && !ls.adapter_out);
    assert(!events && !data_pushes && !flushes);
    ls.adapter_in = 32;
    frame_data = (struct aud_data){.buf = payload};
    assert(clock_lipsync_core(&ls) == 0 && ls.action == LS_NORMAL);
    assert(ls.repeat_cnt == 1 && data_pushes == 1);

    ls = (struct lipsync){.channels = 8, .fs = 48, .bytePerSample = 2,
        .nSamplePerFrame = 2, .adapter_in = 32, .LipsyncOnOff = 1,
        .clockType = 0x100, .action = LS_IDLE};
    reset_observations();
    adapter_owner = &ls;
    have_timestamp = false;
    assert(clock_lipsync_core(&ls) == 0 && ls.action == LS_IDLE && ls.idle_time == 11);
    assert(ls.out_ts == 0x0fffffff && ls.adapter_out == 32 && !events && !data_pushes);

    const int interrupted[] = {LS_IDLE, LS_REPEAT, LS_E_NORM, LS_EXCEPT};
    for (unsigned index = 0; index < sizeof(interrupted) / sizeof(interrupted[0]); ++index) {
        ls = (struct lipsync){.channels = 8, .fs = 48, .bytePerSample = 2,
            .nSamplePerFrame = 2, .adapter_in = 32, .dataType = 1,
            .LipsyncOnOff = 1, .clockType = 0x100, .upperbound = 10,
            .lowerbound = 5, .freerunupperbound = 20, .idle_time = 1,
            .action = interrupted[index], .skip_cnt = 5, .idle_cnt = 7};
        reset_observations();
        adapter_owner = &ls;
        adapter_timestamp = 55001 + 5940 + 901;
        stc_values[0x84 / 4] = 55001;
        assert(clock_lipsync_core(&ls) == 0 && ls.action == LS_IDLE);
        assert(ls.discontinuity == 1 && !ls.idle_cnt && ls.adapter_out == 32);
        assert(!events && !data_pushes);
    }
}

static void test_repeat_wait(void)
{
    unsigned char payload[32];
    struct aud_event first = {.type = 1};
    struct lipsync ls = {.channels = 8, .fs = 48, .bytePerSample = 2,
        .nSamplePerFrame = 2, .adapter_in = 32, .dataType = 1, .first = &first,
        .LipsyncOnOff = 1, .clockType = 0x100, .upperbound = 10,
        .lowerbound = 5, .freerunupperbound = 5000, .action = LS_REPEAT,
        .repeat_stc = 10000};
    reset_observations();
    adapter_owner = &ls;
    adapter_timestamp = 16840;
    stc_values[0x84 / 4] = 10000;
    repeat_wait = true;
    assert(clock_lipsync_core(&ls) == 1 && waits == 11 && wait_timeout == 10000);
    assert(!ls.unchanged_stc && ls.action == LS_REPEAT && ls.diff == 900 && ls.state_changed);
    assert(!ls.adapter_out && !events && !data_pushes && !ls.first_sent);

    reset_observations();
    ls.state_changed = false;
    adapter_owner = &ls;
    adapter_timestamp = 16840;
    stc_values[0x84 / 4] = 10000;
    frame_data = (struct aud_data){.buf = payload};
    advance_wait = true;
    assert(clock_lipsync_core(&ls) == 0 && waits == 1 && wait_timeout == 10000);
    assert(ls.action == LS_NORMAL && !ls.diff && ls.adapter_out == 32);
    assert(events == 1 && data_pushes == 1 && ls.first_sent);

    reset_observations();
    ls.adapter_out = 0;
    ls.action = LS_REPEAT;
    ls.first_sent = 0;
    adapter_timestamp = 16840;
    stc_values[0x84 / 4] = 10000;
    assert(clock_lipsync_core(&ls) == 0 && waits == 1 && ls.flush_async);
    assert(!ls.adapter_out && !events && !data_pushes && !locks);

    reset_observations();
    ls.flush_async = false;
    ls.disconnecting = true;
    adapter_timestamp = 16840;
    stc_values[0x84 / 4] = 10000;
    repeat_wait = true;
    assert(clock_lipsync_core(&ls) == 1 && waits == 1);
    assert(!ls.disconnecting && ls.state_changed && !ls.adapter_out && !events && !locks);

    reset_observations();
    ls.state_changed = false;
    ls.gap_handling = 1;
    ls.start_pts = 1000;
    ls.end_pts = 10000;
    adapter_timestamp = 16840;
    stc_values[0x84 / 4] = 10000;
    assert(clock_lipsync_core(&ls) == 0 && waits == 1 && wait_timeout == 100000);
    assert(!ls.gap_handling && !ls.start_pts && !ls.end_pts);
    assert(!ls.adapter_out && !events && !data_pushes && !locks);
}

static void test_carrier_first_fields(void)
{
    const char *codecs[] = {"mat", "pcm"};
    for (unsigned codec = 0; codec < 2; ++codec) {
        for (int clock_x4 = 0; clock_x4 < 2; ++clock_x4) {
            struct first_event first = {.event = {.type = 1}, .codec = codecs[codec],
                .channels = 8, .bits = 16, .rate = clock_x4 ? 48000 : 192000,
                .samples = 1536, .data_rate = 192000, .data_channels = 2,
                .data_bps = 32, .acmod = 7, .es = codec == 1,
                .clock_x4 = clock_x4, .explicit_values = true};
            struct first_event saved = first;
            struct lipsync ls = {0};
            reset_observations();
            _lipsync_set_prop_datatype(&ls, codec);
            assert(_lipsync_event(&ls, NULL, &first.event) == 0);
            assert(ls.dataType == (codec == 0) && ls.is_pcm == (codec == 1));
            assert(ls.channels == 8 && ls.fs == (clock_x4 ? 48 : 192));
            assert(ls.bytePerSample == (codec == 0 ? 2 : 4));
            assert(ls.nSamplePerFrame == (codec == 0 ? 1536 : 256));
            assert(!memcmp(&first, &saved, sizeof(first)));
            unsigned char payload[16];
            memset(payload, 0xa5, sizeof(payload));
            struct aud_data data = {.type = codec, .size = sizeof(payload), .buf = payload};
            assert(_lipsync_write_output(&ls, &data) == 0);
            assert(data.type == (codec == 0) && last_event == &first.event);
            for (unsigned index = 0; index < sizeof(payload); ++index)
                assert(payload[index] == 0xa5);
        }
    }
}

static void test_set_base(void)
{
    struct aud_event first = {.type = 1};
    struct lipsync ls = {.first = &first, .first_sent = 1, .dataType = 1,
        .bytePerSample = 2, .nSamplePerFrame = 1536, .channels = 8, .fs = 192,
        .adapter_in = 99, .adapter_out = 11, .action = LS_REPEAT, .idle_cnt = 7,
        .drop_count = 8, .discard_param = 3, .first_output_sent = 1,
        .BaseCT = 31, .BaseST = 47, .BaseCT_copy = 9, .BaseST_copy = 10};
    int32_t base[] = {31, 47};
    reset_observations();
    assert(_lipsync_cmd(&ls, 0x52030008, base, 0) == 0);
    assert(!ls.state_changed && ls.BaseCT_copy == 31 && ls.BaseST_copy == 47);
    assert(ls.action == LS_REPEAT && ls.idle_cnt == 7 && ls.drop_count == 8);
    for (unsigned index = 0; index < 2; ++index) {
        base[index] = index ? INT32_MIN : INT32_MAX;
        ls.state_changed = false;
        assert(_lipsync_cmd(&ls, 0x52030008, base, sizeof(base)) == 0);
        assert(ls.state_changed && ls.BaseCT == base[0] && ls.BaseST == base[1]);
        assert(ls.BaseCT_copy == base[0] && ls.BaseST_copy == base[1]);
        assert(ls.action == LS_REPEAT && ls.idle_cnt == 7 && ls.drop_count == 8);
        assert(_lipsync_cmd(&ls, 0x52030008, base, sizeof(base)) == 0);
        assert(ls.state_changed);
    }
    assert(!flushes && !metadata_flushes && !defaults && !events && !put_count && !get_count);
    assert(ls.first == &first && ls.first_sent && ls.first_output_sent);
    assert(ls.adapter_in == 99 && ls.adapter_out == 11);
    assert(ls.dataType == 1 && ls.bytePerSample == 2 && ls.nSamplePerFrame == 1536);
    assert(ls.channels == 8 && ls.fs == 192);
    assert(_lipsync_state(&ls) == LS_OFF);
    assert(!ls.state_changed && ls.action == LS_IDLE && !ls.idle_cnt && ls.drop_count == 3);
    assert(ls.first == &first && ls.first_sent && ls.first_output_sent);
    assert(ls.adapter_in == 99 && ls.adapter_out == 11 && !flushes && !put_count);
}

static void test_set_base_null(void)
{
    pid_t child = fork();
    assert(child >= 0);
    if (!child) {
        struct lipsync ls = {0};
        _lipsync_cmd(&ls, 0x52030008, NULL, 0);
        _exit(0);
    }
    int child_status = 0;
    assert(waitpid(child, &child_status, 0) == child);
    assert(WIFSIGNALED(child_status) && WTERMSIG(child_status) == SIGABRT);
}

static void test_gap_timestamps(void)
{
    const struct { uint64_t timestamp; uint32_t expected; } cases[] = {
        {0, 0}, {11111, 0}, {11112, 1}, {1000000000, 90000},
        {UINT64_C(0x100000000), 386547},
        {UINT64_C(47721858844445), 0},
        {UINT64_C(0x8000000000000000), (uint32_t)UINT64_C(92233720368547)},
        {UINT64_MAX, (uint32_t)UINT64_C(184467440737095)}
    };
    struct aud_event first = {.type = 1};
    struct lipsync ls = {.first = &first, .first_sent = 1, .dataType = 1,
        .bytePerSample = 2, .nSamplePerFrame = 1536, .channels = 8, .fs = 192,
        .adapter_in = 99, .adapter_out = 11, .action = LS_REPEAT,
        .drop_count = 8, .first_output_sent = 1, .BaseCT = 17, .BaseST = 23};
    reset_observations();
    for (unsigned index = 0; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        unsigned other = (index + 3u) % (sizeof(cases) / sizeof(cases[0]));
        uint32_t timestamps[] = {(uint32_t)cases[index].timestamp,
            (uint32_t)(cases[index].timestamp >> 32), (uint32_t)cases[other].timestamp,
            (uint32_t)(cases[other].timestamp >> 32)};
        assert(_lipsync_cmd(&ls, 0x10210010, timestamps, 0) == 0);
        assert((uint32_t)ls.start_pts == cases[index].expected);
        assert((uint32_t)ls.end_pts == cases[other].expected && ls.gap_handling == 1);
        assert(ls.first == &first && ls.first_sent && ls.first_output_sent);
        assert(ls.adapter_in == 99 && ls.adapter_out == 11);
        assert(ls.action == LS_REPEAT && ls.drop_count == 8 && !ls.state_changed);
        assert(ls.BaseCT == 17 && ls.BaseST == 23 && ls.dataType == 1);
    }
    assert(!flushes && !metadata_flushes && !defaults && !events && !put_count && !get_count);
    assert(_lipsync_cmd(&ls, 0x1021000f, NULL, 0) == 29 && defaults == 1);
    assert(ls.gap_handling == 1 && ls.first == &first && ls.adapter_out == 11);
}

static void test_idle_boundary(void)
{
    struct aud_event first = {.type = 1};
    struct lipsync ls = {.first = &first, .first_sent = 1, .dataType = 1,
        .adapter_in = 99, .adapter_out = 11, .action = LS_IDLE,
        .LipsyncOnOff = 1, .clockType = 0x100, .idle_cnt = 1, .idle_time = 3};
    reset_observations();
    assert(_lipsync_state(&ls) == LS_IDLE && ls.idle_cnt == 2);
    int escaped = _lipsync_state(&ls);
    assert(escaped == LS_TONORM && ls.idle_cnt == 3);
    ls.idle_cnt = 1;
    ls.discontinuity = 1;
    assert(_lipsync_state(&ls) == LS_IDLE && ls.idle_cnt == 0);
    ls.idle_cnt = 1;
    ls.discontinuity = 2;
    assert(_lipsync_state(&ls) == LS_IDLE && ls.idle_cnt == 2);
    const struct { int clock; uint32_t timestamp; int count, limit, expected_limit; } gates[] = {
        {0x100, 0x0fffffff, 0, 0, 11}, {0x100, 0xffffffff, 0, 0, 11},
        {0x100, 0x0ffffffe, 0, 0, 0}, {0x101, 0x0fffffff, 0, 0, 0},
        {0x100, 0x0fffffff, 1, 0, 0}, {0x100, 0x0fffffff, 0, 7, 7},
        {0x100, 0, INT32_MAX, 3, 3}
    };
    for (unsigned index = 0; index < sizeof(gates) / sizeof(gates[0]); ++index) {
        ls.clockType = gates[index].clock;
        ls.out_ts = gates[index].timestamp;
        ls.idle_cnt = gates[index].count;
        ls.idle_time = gates[index].limit;
        int action = _lipsync_state(&ls);
        assert(ls.idle_time == gates[index].expected_limit);
        if (index == 6)
            assert(action == LS_IDLE && ls.idle_cnt == INT32_MIN);
        else if (ls.idle_time > 1)
            assert(action == LS_IDLE);
        else
            assert(action == LS_TONORM);
    }
    assert(ls.first == &first && ls.first_sent && ls.dataType == 1);
    assert(ls.adapter_in == 99 && ls.adapter_out == 11);
    assert(!flushes && !metadata_flushes && !events && !put_count && !get_count);
}

static int state_for(int previous, int32_t diff, uint32_t stc_now)
{
    struct lipsync ls = {.LipsyncOnOff = 1, .clockType = 0x100, .upperbound = 10,
        .lowerbound = 5, .freerunupperbound = 20, .freerunlowerbound = 20,
        .idle_time = 1, .action = previous, .diff = diff, .repeat_stc = 1000};
    stc_values[0x84 / 4] = stc_now;
    return _lipsync_state(&ls);
}

static void test_positive_gap_transitions(void)
{
    reset_observations();
    /* IDLE escape: hold (REPEAT) up to freerun bound, otherwise FREE_RUN. */
    assert(state_for(LS_IDLE, 1, 1000) == LS_REPEAT);
    assert(state_for(LS_IDLE, 1800, 1000) == LS_REPEAT);
    assert(state_for(LS_IDLE, 1801, 1000) == LS_FREE_RUN);
    assert(state_for(LS_E_NORM, 901, 1000) == LS_REPEAT);
    assert(state_for(LS_E_NORM, 1801, 1000) == LS_FREE_RUN);
    /* REPEAT gives up only once the STC passes repeat_stc + bound. */
    assert(state_for(LS_REPEAT, 1, 2800) == LS_REPEAT);
    assert(state_for(LS_REPEAT, 1, 2801) == LS_WRONG);
    assert(state_for(LS_WRONG, 1, 1000) == LS_WRONG);
    assert(state_for(LS_WRONG, -5000, 1000) == LS_WRONG);

    struct lipsync ls = {.LipsyncOnOff = 1, .clockType = 0x100, .upperbound = 10,
        .lowerbound = 5, .freerunupperbound = 20, .action = LS_EXCEPT,
        .diff = 901, .skip_cnt = 5};
    assert(_lipsync_state(&ls) == LS_REPEAT && !ls.skip_cnt);
    ls.action = LS_EXCEPT;
    ls.diff = 1801;
    ls.skip_cnt = 5;
    assert(_lipsync_state(&ls) == LS_FREE_RUN);

    /* Frozen STC with a held frame: the repeat window never expires. */
    ls = (struct lipsync){.LipsyncOnOff = 1, .clockType = 0x100, .upperbound = 10,
        .lowerbound = 5, .freerunupperbound = 20, .action = LS_IDLE, .idle_time = 1,
        .diff = 900};
    stc_values[0x84 / 4] = 5000;
    for (int i = 0; i < 1000; i++) {
        ls.action = _lipsync_state(&ls);
        assert(ls.action == LS_REPEAT && ls.repeat_stc == 5000);
    }
    stc_values[0x84 / 4] = 6801;
    assert(_lipsync_state(&ls) == LS_WRONG);
}

static void test_master_and_nodelay(void)
{
    reset_observations();
    struct lipsync ls = {.LipsyncOnOff = 1, .clockType = 0x200, .upperbound = 10,
        .lowerbound = 5, .action = LS_NORMAL, .count_transition = 1,
        .BaseCT_copy = 7, .BaseST_copy = 9, .diff = 901};
    assert(_lipsync_state(&ls) == LS_FREERUN_WITHNOTI && ls.skip_cnt == 1 && !named_master);
    assert(_lipsync_state(&ls) == LS_FREERUN_WITHNOTI && !ls.skip_cnt && named_master == 1);
    assert(ls.last_diff == 901 && ls.unknown_19c == 1);
    assert(master_base[0] == 9 && master_base[1] == 7);
    ls.diff = 901 - 450;
    assert(_lipsync_state(&ls) == LS_FREERUN_WITHNOTI && named_master == 1);
    ls.LipsyncOnOff = 0;
    assert(_lipsync_state(&ls) == LS_FREERUN_WITHNOTI);

    ls = (struct lipsync){.LipsyncOnOff = 1, .NodelayOnOff = 1, .action = LS_NORMAL,
        .adapter_in = 100, .unknown_16c = 199, .unknown_170 = 99, .unknown_174 = 2};
    assert(_lipsync_state(&ls) == LS_NODELAY);
    ls.action = LS_NODELAY;
    assert(_lipsync_state(&ls) == LS_SKIP);
    ls.unknown_16c = 200;
    assert(_lipsync_state(&ls) == LS_NODELAY);
    ls.action = LS_SKIP;
    assert(_lipsync_state(&ls) == LS_SKIP);
    ls.unknown_170 = 200;
    assert(_lipsync_state(&ls) == LS_NODELAY);
    ls.NodelayOnOff = 2;
    assert(_lipsync_state(&ls) == LS_OFF);
}

static void test_commands_and_replay(void)
{
    struct first_event mat = {.event = {.type = 1}, .codec = "mat", .channels = 8,
        .bits = 16, .rate = 192000, .samples = 2, .explicit_values = true};
    struct first_event pcm = mat;
    pcm.codec = "pcm";
    unsigned char payload[64];
    memset(payload, 0xa5, sizeof(payload));
    struct lipsync ls = {0};
    struct aud_port port = {.module = &ls};
    struct aud_data carrier = {.type = 0, .size = 32, .buf = payload};
    reset_observations();
    adapter_owner = &ls;
    selected_action = LS_OFF;
    frame_data = carrier;
    assert(_lipsync_event(&ls, &port, &mat.event) == 0);
    assert(test_lipsync_chain(&port, &carrier) == 1);
    assert(events == 1 && data_pushes == 1 && ls.first_sent && !ls.first_received);
    assert(ls.dataType == 1 && ls.fs == 192 && ls.channels == 8);
    assert(frame_data.type == 1 && frame_data.size == 32 && last_event == &mat.event);

    reset_observations();
    adapter_owner = &ls;
    ls.first_output_sent = 1;
    ls.drop_count = 7;
    ls.discard_param = 0;
    assert(_lipsync_cmd(&ls, 0x10010000, NULL, 0) == 0);
    assert(ls.action == LS_IDLE && ls.state_changed && ls.drop_count == 7);
    assert(ls.first == &mat.event && ls.first_sent && ls.dataType == 1);
    assert(!ls.first_output_sent && !events && !put_count && !flushes && !defaults);
    assert(_lipsync_state(&ls) == LS_OFF && !ls.state_changed && !ls.drop_count);
    ls.discard_param = 3;
    assert(_lipsync_cmd(&ls, 0x10010000, NULL, 0) == 0 && ls.drop_count == 3);
    assert(_lipsync_state(&ls) == LS_OFF && !ls.state_changed && ls.drop_count == 3);
    struct lipsync saved = ls;
    assert(_lipsync_cmd(&ls, 0x10020000, NULL, 0) == 0);
    assert(_lipsync_cmd(&ls, 0x10150000, NULL, 0) == 0);
    assert(!memcmp(&ls, &saved, sizeof(ls)));
    ls.start_pts = 11;
    ls.end_pts = 22;
    ls.gap_handling = ls.gap_correction = ls.additional_bound = 1;
    ls.debug_count[0] = 99;
    assert(_lipsync_cmd(&ls, 0x10000000, NULL, 0) == 0);
    assert(!ls.start_pts && !ls.end_pts && !ls.gap_handling && !ls.gap_correction);
    assert(!ls.debug_count[0] && !ls.additional_bound && ls.first == &mat.event);
    assert(ls.dataType == 1 && ls.bytePerSample == 2 && ls.nSamplePerFrame == 2);
    assert(_lipsync_cmd(&ls, 0xdeadbeef, NULL, 0) == 29);

    _lipsync_set_prop_offset(&ls, 123);
    _lipsync_set_baseCT(&ls, 11);
    _lipsync_set_baseST(&ls, 22);
    _lipsync_set_state(&ls, 0);
    assert(ls.offset_changed && ls.state_changed && ls.state == 0);
    ls.drop_count = 0;
    assert(_lipsync_state(&ls) == LS_OFF && ls.action == LS_IDLE && ls.drop_count == 3);
    assert(!ls.offset_changed && !ls.state_changed && ls.first == &mat.event);
    assert(ls.first_sent && ls.dataType == 1 && ls.channels == 8 && ls.fs == 192);
    assert(ls.bytePerSample == 2 && ls.nSamplePerFrame == 2);

    reset_observations();
    adapter_owner = &ls;
    ls.discard_param = ls.drop_count = 0;
    ls.adapter_in = 40;
    ls.adapter_out = 32;
    assert(_lipsync_cmd(&ls, 0x1003002c, NULL, 0) == 0);
    assert(ls.first_received && !flushes && ls.first_sent && ls.first == &mat.event);
    assert(test_lipsync_chain(&port, &carrier) == 1);
    assert(ls.adapter_in == ls.adapter_out && !ls.first_received);
    assert(events == 1 && last_event == &mat.event && data_pushes == 1);
    struct aud_event flush = {.type = 7, .flags = 2};
    assert(_lipsync_event(&ls, &port, &flush) == 23);
    assert(!ls.first_sent && ls.first == &mat.event && ls.dataType == 1);
    assert(test_lipsync_chain(&port, &carrier) == 1);
    assert(events == 2 && last_event == &mat.event && data_pushes == 2);

    reset_observations();
    adapter_owner = &ls;
    assert(_lipsync_event(&ls, &port, &pcm.event) == 0);
    assert(events == 1 && last_event == &pcm.event && ls.first_received);
    assert(ls.first_sent && ls.dataType == 0 && ls.bytePerSample == 4);
    carrier.size = 0;
    assert(test_lipsync_chain(&port, &carrier) == 1);
    assert(!ls.first_sent && !ls.first_received && events == 1 && !data_pushes);
    struct aud_event last = {.type = 2};
    assert(_lipsync_event(&ls, &port, &last) == 0 && ls.first == &pcm.event);
    ls.first_sent = 1;
    assert(_lipsync_event(&ls, &port, &last) == 23 && !ls.first);
    assert(ls.dataType == 0 && ls.channels == 8 && ls.fs == 192 && ls.bytePerSample == 4);
}

int main(void)
{
    _Static_assert(offsetof(struct aud_event, flags) == 0x10, "event flags offset");
    test_first();
    test_output();
    test_metadata_first();
    test_datatype();
    test_last_and_flush();
    test_frame_dispatch();
    test_core_timestamp();
    test_first_master_baseline();
    test_clock_difference();
    test_clock_to_action();
    test_clock_reset_boundaries();
    test_clock_counters();
    test_repeat_wait();
    test_carrier_first_fields();
    test_set_base();
    test_set_base_null();
    test_gap_timestamps();
    test_idle_boundary();
    test_positive_gap_transitions();
    test_master_and_nodelay();
    test_commands_and_replay();
    puts("lipsync: clock-to-action, wrap/discontinuity, short-frame/base reset and FIRST/data ownership: PASS");
    return 0;
}
