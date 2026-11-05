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

volatile uint32_t gain_log_mask;
const void *const gain_log = NULL;
static int pushed, freed, first_kind, fail_alloc;
static struct aud_data *last;
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
    (void)p;
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
    memset(&z, 0, sizeof(z));
    z.size = n;
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
    return !strcmp(k, "es") ? true : d;
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
    puts("gain ES: pass/zero/drop, 3-unit unmute boundary, allocation failure, FIRST/chain mute "
         "threshold and pending gain: PASS");
    return 0;
}
