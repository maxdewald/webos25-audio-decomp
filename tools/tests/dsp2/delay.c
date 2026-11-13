// @sources tools/tests/dsp2/delay_stubs.c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#undef memcpy
// @match src/dsp2/delay.c (?s)struct aud_event \{.*?\};
// @match src/dsp2/delay.c (?s)struct aud_data \{.*?\};
// @match src/dsp2/delay.c (?s)struct delay \{.*?\};
// @match src/dsp2/delay.c (?m)^extern[^;]+;
// @extract src/dsp2/delay.c delay_fadein delay_fadeout delay_push_event delay_push_output delay_queue_push_locked delay_add_zero delay_queue_flush delay_set_delay delay_chain delay_event
static int pushes, new_chunks, releases, frees, locks;
static unsigned bytes;
static bool pcm_queue, dequeue_empty;
static int event_es, default_events;
void *aud_object_get(void *p) { return p; }
void aud_object_put(void *p)
{
    (void)p;
    frees++;
}
int aud_module_push_data(void *m, int p, struct aud_data *d)
{
    (void)m;
    (void)p;
    pushes++;
    bytes += d->size;
    return 0;
}
int aud_module_sync(void *m, int p)
{
    (void)m;
    (void)p;
    return 0;
}
void aud_mutex_lock(void *m)
{
    (void)m;
    locks++;
}
void aud_mutex_unlock(void *m)
{
    (void)m;
    locks--;
}
int aud_data_adapter_push(void *a, struct aud_data *d)
{
    (void)a;
    assert(d->type == (pcm_queue ? 0 : 1));
    new_chunks++;
    return 0;
}
struct aud_data *aud_data_new(unsigned n)
{
    static struct aud_data chunk;
    static char buf[32768];
    memset(&chunk, 0, sizeof(chunk));
    chunk.buf = buf;
    chunk.size = n;
    return &chunk;
}
static struct aud_data original = {.type = 1, .size = 32768};
struct aud_data *aud_data_adapter_dequeue(void *a)
{
    (void)a;
    if (dequeue_empty)
        return NULL;
    releases++;
    return &original;
}
void _pending_events_empty(void *a) { (void)a; }
int aud_event_has_name(struct aud_event *event, const char *name) { return 0; }
int aud_event_value_get_int(struct aud_event *event, const char *name, int fallback)
{
    if (!strcmp(name, "channels"))
        return 8;
    if (!strcmp(name, "bits-per-sample"))
        return 16;
    return fallback;
}
int aud_event_value_get_bool(struct aud_event *event, const char *name, int fallback)
{
    return !strcmp(name, "es") ? event_es : fallback;
}
const char *aud_event_value_get_string(struct aud_event *event, const char *name,
                                      const char *fallback)
{
    return fallback;
}
void aud_event_value_set_bool(struct aud_event *event, const char *name, int value) {}
int aud_module_default_event(void *module, void *port, struct aud_event *event)
{
    default_events++;
    return 0;
}
void _pending_events_check(void *a) { (void)a; }
void _pending_events_update(void *a, unsigned n)
{
    (void)a;
    (void)n;
}
static void test_fades(void)
{
    struct delay dl = {.channels = 8, .bits = 16, .bytes_per_ms_unit = 16};
    enum { payload_words = 8192 / 4, extent_words = 16384 / 4 };
    int32_t source[extent_words], destination[extent_words + 2];
    const int32_t samples[] = {0x12340102, -0x12340102, INT32_MIN, INT32_MAX,
                               511, -511, 513, -513};
    for (unsigned index = 0; index < extent_words; index++)
        source[index] = samples[index % 8];
    for (unsigned stride = 16; stride <= 32; stride += 16) {
        unsigned frames = 8192 / stride, written = frames * 8;
        for (unsigned direction = 0; direction < 2; direction++) {
            for (unsigned index = 0; index < extent_words + 2; index++)
                destination[index] = 0x13579bdf;
            if (direction)
                delay_fadeout(&dl, destination + 1, source, frames);
            else
                delay_fadein(&dl, destination + 1, source, frames);
            assert(destination[0] == 0x13579bdf);
            for (unsigned index = 0; index < written; index++) {
                unsigned factor = direction ? frames - index / 8 : index / 8;
                int32_t expected = (int32_t)((int64_t)(source[index] / (int)frames) * factor);
                assert(destination[index + 1] == expected);
            }
            for (unsigned index = written + 1; index < extent_words + 2; index++)
                assert(destination[index] == 0x13579bdf);
            assert((destination[payload_words + 1] != 0x13579bdf) == (stride == 16));
        }
    }
    memcpy(destination + 1, source, sizeof(source));
    delay_fadein(&dl, destination + 1, destination + 1, 512);
    for (unsigned index = 0; index < extent_words; index++)
        assert(destination[index + 1] == (int32_t)((int64_t)(source[index] / 512) * (index / 8)));
    destination[0] = 0x13579bdf;
    delay_fadein(&dl, NULL, NULL, 0);
    delay_fadeout(&dl, NULL, NULL, 0);
    dl.channels = 0;
    delay_fadein(&dl, destination, source, 512);
    delay_fadeout(&dl, destination, source, 512);
    assert(destination[0] == 0x13579bdf);
}

static void test_chain_fade_units(void)
{
    struct delay dl = {.channels = 8, .bits = 16, .bytes_per_ms_unit = 16, .rate_khz = 48,
                       .au_length = 256, .new_au_length = 256,
                       .number_packet = 1, .new_number_packet = 1,
                       .fadein_pending = true};
    int32_t storage[4097];
    struct aud_data data = {.type = 0, .size = 8192, .buf = storage};
    void *port[2] = {NULL, &dl};
    for (unsigned index = 0; index < 4097; index++)
        storage[index] = 0x12340102;
    int previous_pushes = pushes;
    assert(delay_chain(port, &data) == 0);
    assert(!dl.fadein_pending && pushes == previous_pushes + 1);
    for (unsigned index = 0; index < 4096; index++)
        assert(storage[index] == (0x12340102 / 512) * (int)(index / 8));
    assert(storage[4096] == 0x12340102);
    assert(delay_chain(port, &data) == 0);
    assert(storage[2048] == (0x12340102 / 512) * 256);
    for (unsigned index = 0; index < 4097; index++)
        storage[index] = 0x12340102;
    dl.new_delay = 1;
    pcm_queue = true;
    assert(delay_chain(port, &data) == 0);
    pcm_queue = false;
    assert(dl.delay == 1 && dl.fadein_pending && !locks);
    for (unsigned index = 0; index < 4096; index++)
        assert(storage[index] == (0x12340102 / 512) * (int)(512 - index / 8));
    assert(storage[4096] == 0x12340102);
}

static void test_event_arming_boundary(void)
{
    struct aud_event first = {.type = 1}, last = {.type = 2};
    for (unsigned pending = 0; pending < 2; pending++) {
        struct delay dl = {.delay = 40, .new_delay = 40,
                           .au_length = 256, .new_au_length = 256,
                           .number_packet = 1, .new_number_packet = 1,
                           .q_in = 1, .fadein_pending = pending, .es = true};
        assert(delay_event(&dl, NULL, &first) == 0);
        assert(dl.channels == 8 && dl.bits == 16 && dl.bytes_per_ms_unit == 16);
        assert(!dl.es && dl.recalc && dl.fadein_pending == pending);
        assert(dl.delay == 40 && dl.new_delay == 40);
        dl.new_delay = 20;
        assert(delay_event(&dl, NULL, &first) == 0);
        assert(dl.delay == 20 && dl.fadein_pending == pending);
        assert(delay_set_delay(&dl, 10) == 0);
        assert(dl.delay == 10 && dl.fadein_pending == pending);
        assert(delay_set_delay(&dl, 10) == 0);
        assert(dl.fadein_pending == pending);
        dl.es = true;
        dl.q_in = dl.q_out;
        dequeue_empty = true;
        int forwarded = default_events;
        assert(delay_event(&dl, NULL, &last) == 0);
        assert(default_events == forwarded + 1 && dl.fadein_pending == pending);
        dl.es = false;
        dl.delay = 0;
        assert(delay_event(&dl, NULL, &last) == 0);
        assert(default_events == forwarded + 2 && dl.fadein_pending == pending);
        dequeue_empty = false;
    }
    struct delay dl = {.delay = 40, .new_delay = 40,
                       .au_length = 256, .new_au_length = 256,
                       .number_packet = 1, .new_number_packet = 1, .q_in = 1};
    int32_t storage[4097];
    struct aud_data data = {.type = 0, .size = 8192, .buf = storage};
    void *port[2] = {NULL, &dl};
    for (unsigned index = 0; index < 4097; index++)
        storage[index] = 0x12340102;
    assert(delay_event(&dl, NULL, &first) == 0);
    pcm_queue = true;
    int previous_pushes = pushes;
    assert(delay_chain(port, &data) == 0);
    pcm_queue = false;
    assert(!dl.fadein_pending && pushes == previous_pushes && dl.packet_count == 1);
    for (unsigned index = 0; index < 4097; index++)
        assert(storage[index] == 0x12340102);
    assert(!locks);
}

int main(void)
{
    struct delay dl = {.channels = 8,
                       .bits = 16,
                       .rate_khz = 192,
                       .bytes_per_ms_unit = 16,
                       .au_length = 256,
                       .new_au_length = 256,
                       .number_packet = 6,
                       .new_number_packet = 6,
                       .es = true,
                       .recalc = true};
    void *p[2] = {NULL, &dl};
    char data[32768] = {0};
    original.buf = data;
    assert(delay_chain(p, &original) == 0);
    assert(pushes == 1 && bytes == 32768);
    assert(dl.number_packet == 2 && dl.new_number_packet == 2 && !dl.recalc &&
           dl.last_size == 32768);
    assert(!new_chunks && !locks);
    dl.delay = dl.new_delay = 10;
    dl.q_in = 1;
    dl.q_out = 0;
    pushes = 0;
    bytes = 0;
    delay_chain(p, &original);
    assert(new_chunks == 2 && dl.packet_count == 2 && !pushes);
    delay_chain(p, &original);
    assert(new_chunks == 4 && dl.packet_count == 2 && pushes == 2 && releases == 2);
    assert(!locks);
    test_fades();
    test_chain_fade_units();
        test_event_arming_boundary();
    puts("delay ES: MAT/192k/8ch/16bit recalculation=2 chunks, delay=0 forwards, delay=10 releases "
            "and leaves bounded tail; scalar fades, +0xfc call units, FIRST/LAST arming boundary: PASS");
    return 0;
}
