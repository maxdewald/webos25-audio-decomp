#include "../../../src/dsp2/gain.c"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gain_chain_pcm_unrecovered(struct gain_module *m, struct aud_data *d, int muted)
{
    (void)m;
    (void)d;
    (void)muted;
    abort();
}

void gain_control(const int32_t *src, int word_offset, int32_t *dst,
                  int channels, int32_t effective_gain)
{
    abort();
}

void gain_set_fade_gain(struct gain_module *m)
{
    abort();
}

volatile uint32_t gain_log_mask;
const void *const gain_log = NULL;
static int pushed, freed, first_kind, fail_alloc;
static struct aud_data *last;
static unsigned char zero_payload[32768];
static void *last_released;
static bool first_es = true;
static struct aud_event original, muted_first;
void logmm_printl(const void *p, int l, const char *f, int n, const char *s, ...)
{
    (void)p;
    (void)l;
    (void)f;
    (void)n;
    (void)s;
}
void *aud_object_get(void *p) { return p; }
void aud_object_put(void *p)
{
    last_released = p;
    ++freed;
}
int aud_module_push_data(struct gain_module *m, int p, struct aud_data *d)
{
    (void)m;
    (void)p;
    ++pushed;
    last = d;
    return 0;
}
int aud_module_push_event(struct gain_module *m, int p, struct aud_event *e)
{
    (void)m;
    (void)p;
    first_kind = e == &muted_first ? 1 : 2;
    return 0;
}
void aud_module_sync(struct gain_module *m, int p)
{
    (void)m;
    (void)p;
}
struct aud_data *aud_data_new_zero(unsigned n)
{
    static struct aud_data z;
    if (fail_alloc)
        return NULL;
    assert(n <= sizeof(zero_payload));
    memset(zero_payload, 0, n);
    memset(&z, 0, sizeof(z));
    z.size = n;
    z.ptr = zero_payload;
    return &z;
}
void aud_mutex_lock(struct aud_mutex *m) { (void)m; }
void aud_mutex_unlock(struct aud_mutex *m) { (void)m; }
int aud_event_value_get_int(struct aud_event *e, const char *k, int d)
{
    (void)e;
    return !strcmp(k, "channels") ? 8 : d;
}
bool aud_event_value_get_bool(struct aud_event *e, const char *k, bool d)
{
    (void)e;
    return !strcmp(k, "es") ? first_es : d;
}
const char *aud_event_value_get_str(struct aud_event *e, const char *k, const char *d)
{
    (void)e;
    (void)k;
    return d;
}
void aud_event_value_set_int(struct aud_event *e, const char *k, int v)
{
    (void)e;
    (void)k;
    (void)v;
}
struct aud_event *aud_event_dup(struct aud_event *e)
{
    (void)e;
    return &muted_first;
}
int aud_module_default_event(struct gain_module *m, struct aud_port *p, struct aud_event *e)
{
    (void)m;
    (void)p;
    first_kind = e == &muted_first ? 1 : 2;
    return 0;
}

static void reset(void)
{
    pushed = freed = first_kind = fail_alloc = 0;
    last = NULL;
    last_released = NULL;
}
static void test_steady_muted_pcm(void)
{
    unsigned char payload[67], saved[sizeof(payload)];
    memset(payload, 0xa5, sizeof(payload));
    memcpy(saved, payload, sizeof(saved));
    for (int es_mode = 0; es_mode < 2; ++es_mode) {
        struct gain_module m = {.channel = 8, .current_gain = GAIN_UNITY,
            .gain = -1, .fadetime_ms = -1, .fade_gain = GAIN_UNITY,
            .fade_target = GAIN_UNITY, .target_gain = 123,
            .prev_muted = 1, .mute_input = true,
            .es_mode = es_mode, .es_unit_per_packet = 3, .es_pkt_pos = 2,
            .es_zero_state = 2, .delay_changed = true};
        struct aud_port port = {.module = (void *)&m};
        struct aud_data data = {.type = AUD_DATA_PCM, .index = 17, .ts = 123,
                               .ptr = payload, .size = sizeof(payload)};
        reset();
        assert(gain_chain(&port, &data) == 0);
        assert(pushed == 1 && freed == 1 && last_released == &data && last != &data);
        assert(last->type == AUD_DATA_PCM && last->size == data.size);
        assert(last->index == data.index && last->ts == data.ts);
        for (unsigned index = 0; index < data.size; ++index)
            assert(((unsigned char *)last->ptr)[index] == 0);
        assert(!memcmp(payload, saved, sizeof(payload)) && !first_kind);
        assert(!m.final_gain && m.prev_muted == 1 && !m.delay_changed);
        assert(m.es_pkt_pos == 2 && m.es_zero_state == 2);
        reset();
        fail_alloc = 1;
        m.final_gain = 99;
        m.delay_changed = true;
        assert(gain_chain(&port, &data) == -1);
        assert(!pushed && freed == 1 && last_released == &data);
        assert(m.final_gain == 99 && m.delay_changed && m.es_pkt_pos == 2);
    }
}
static void test_unity_pcm_canary(void)
{
    struct {
        uint32_t before[8];
        unsigned char payload[32768];
        uint32_t after[8];
    } bounded, saved;
    const unsigned sizes[] = {32, 48, 4096, 8192, 32768};
    memset(&bounded, 0xa5, sizeof(bounded));
    memcpy(&saved, &bounded, sizeof(saved));
    for (unsigned index = 0; index < sizeof(sizes) / sizeof(sizes[0]); ++index) {
        struct gain_module m = {.channel = 8, .current_gain = GAIN_UNITY,
            .fade_gain = GAIN_UNITY, .gain = -1, .fadetime_ms = -1,
            .fade_target = GAIN_UNITY, .target_gain = GAIN_UNITY,
            .delay_changed = true};
        struct aud_port port = {.module = (void *)&m};
        struct aud_data data = {.type = AUD_DATA_PCM, .index = 7, .ts = 368902,
            .size = sizes[index], .ptr = bounded.payload};
        first_es = false;
        reset();
        assert(gain_event(&m, &port, &original) == 0);
        assert(!m.es_mode && m.channel == 8);
        reset();
        fail_alloc = 1;
        assert(gain_chain(&port, &data) == 0);
        assert(pushed == 1 && last == &data && !freed && !first_kind);
        assert(data.size == sizes[index] && data.ptr == bounded.payload);
        assert(data.index == 7 && data.ts == 368902);
        assert(!memcmp(&bounded, &saved, sizeof(bounded)));
        assert(m.final_gain == GAIN_UNITY && m.got_first_data);
        assert(!m.prev_muted && !m.gain_changed && !m.delay_changed);
    }
    first_es = true;
}
static void test_first_mute_policy(void)
{
    unsigned char payload[32], saved[sizeof(payload)];
    memset(payload, 0xa5, sizeof(payload));
    memcpy(saved, payload, sizeof(saved));
    for (int es_mode = 0; es_mode < 2; ++es_mode) {
        struct gain_module m = {.current_gain = GAIN_UNITY, .gain = -1,
            .fadetime_ms = -1, .fade_gain = 0x400000, .fade_target = 0x600000,
            .target_gain = 0x700000, .fade_pos = 17, .delta_g = 3, .delta_s = 2,
            .prev_muted = 1, .mute_input = true, .es_unit_per_packet = 1};
        struct aud_port port = {.module = (void *)&m};
        struct aud_data data = {.type = AUD_DATA_ES, .size = sizeof(payload), .ptr = payload};
        first_es = es_mode;
        reset();
        gain_event(&m, &port, &original);
        assert(m.es_mode == (bool)es_mode && first_kind == 1);
        assert(m.mute_input && !m.mute_output && m.current_gain == GAIN_UNITY);
        assert(m.fade_gain == 0x400000 && m.fade_target == 0x600000);
        assert(m.target_gain == 0x700000 && m.fade_pos == 17 && m.delta_g == 3 && m.delta_s == 2);
        reset();
        assert(gain_chain(&port, &data) == 0);
        assert(freed == 1 && last_released == &data);
        assert(pushed == es_mode && (!es_mode || (last != &data && last->size == data.size)));
        if (es_mode) {
            assert(last->type == AUD_DATA_ES);
            for (unsigned index = 0; index < data.size; ++index)
                assert(((unsigned char *)last->ptr)[index] == 0);
        }
        assert(!memcmp(payload, saved, sizeof(payload)));
        reset();
        m.mute_input = false;
        assert(gain_chain(&port, &data) == 0 && pushed == 1 && last == &data && !freed);
        assert(m.fade_gain == 0x400000 && m.delta_g == 3 && m.fade_pos == 17);
        assert(first_kind == 2 && !m.es_zero_state);
        m.es_pkt_pos = 2;
        m.es_zero_state = 2;
        reset();
        gain_event(&m, &port, &original);
        assert(m.es_pkt_pos == 2 && m.es_zero_state == 2);
    }
    first_es = true;
}
int main(void)
{
    struct gain_module m = {0};
    struct aud_data d = {.type = AUD_DATA_ES, .size = 32768};
    m.es_mode = true;
    m.es_unit_per_packet = 3;
    m.first = &original;
    m.first_muted = &muted_first;
    reset();
    gain_handle_nonpcm(&m, &d, 1);
    assert(pushed == 1 && last != &d && last->type == 1 && last->size == d.size &&
           m.es_zero_state == 2 && m.es_pkt_pos == 1);
    reset();
    gain_handle_nonpcm(&m, &d, 0);
    assert(pushed == 1 && last != &d && m.es_pkt_pos == 2 && m.es_zero_state == 2 &&
           first_kind == 2);
    reset();
    gain_handle_nonpcm(&m, &d, 0);
    assert(pushed == 1 && last != &d && m.es_pkt_pos == 0);
    reset();
    gain_handle_nonpcm(&m, &d, 0);
    assert(pushed == 1 && last == &d && m.es_zero_state == 0);
    m.es_mode = false;
    reset();
    gain_handle_nonpcm(&m, &d, 1);
    assert(!pushed && freed == 1);
    reset();
    gain_handle_nonpcm(&m, &d, 0);
    assert(pushed == 1 && last == &d);
    m.es_mode = true;
    m.es_zero_state = 2;
    fail_alloc = 1;
    pushed = 0;
    assert(gain_handle_nonpcm(&m, &d, 1) == -1 && !pushed);
    struct aud_port p = {.module = (void *)&m};
    original.type = AUD_EVENT_FIRST_AU;
    const int gains[] = {-1, 0, 1, 2, 3, 4, 0x800000};
    for (unsigned i = 0; i < sizeof(gains) / sizeof(gains[0]); i++) {
        memset(&m, 0, sizeof(m));
        m.current_gain = gains[i];
        m.gain = -1;
        reset();
        gain_event(&m, &p, &original);
        assert(first_kind == ((uint32_t)gains[i] < 4u ? 1 : 2));
        assert(m.es_mode && m.channel == 8);
    }
    memset(&m, 0, sizeof(m));
    m.current_gain = 0x800000;
    m.gain = 0;
    reset();
    gain_event(&m, &p, &original);
    assert(first_kind == 1);
    for (unsigned i = 0; i < sizeof(gains) / sizeof(gains[0]); i++) {
        memset(&m, 0, sizeof(m));
        m.got_first_data = true;
        m.current_gain = gains[i];
        m.gain = -1;
        m.fadetime_ms = -1;
        m.es_mode = true;
        m.es_unit_per_packet = 1;
        reset();
        assert(gain_chain(&p, &d) == 0 && pushed == 1);
        assert((last != &d) == ((uint32_t)gains[i] < 4u));
    }
    m.current_gain = 0;
    m.gain = 0x800000;
    reset();
    assert(gain_chain(&p, &d) == 0 && last == &d);
    assert(m.current_gain == 0x800000 && m.gain == -1);
    test_steady_muted_pcm();
    test_unity_pcm_canary();
    test_first_mute_policy();
    puts("gain ES: pass/zero/drop, 3-unit unmute boundary, allocation failure, FIRST/chain mute "
            "threshold, fade persistence, settled PCM zeroing and unity PCM canaries: PASS");
    return 0;
}
