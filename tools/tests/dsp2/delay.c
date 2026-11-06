// @sources tools/tests/dsp2/delay_stubs.c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#undef memcpy
#include "../../../src/dsp2/delay.c"
static int pushes, new_chunks, releases, frees, locks;
static unsigned bytes;
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
    assert(d->type == 1);
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
    releases++;
    return &original;
}
void _pending_events_check(void *a) { (void)a; }
void _pending_events_update(void *a, unsigned n)
{
    (void)a;
    (void)n;
}
/* The delay change path is not exercised here; isolate retained function sections. */
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
    puts("delay ES: MAT/192k/8ch/16bit recalculation=2 chunks, delay=0 forwards, delay=10 releases "
         "and leaves bounded tail: PASS");
    return 0;
}
