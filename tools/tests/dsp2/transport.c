#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#undef memcpy
#include "../../../src/dsp2/glue.c"
static int free_bytes, waits, writes, changed_to_flush;
static struct au_info written;
static void *written_bytes;
void aud_mutex_lock(void *p) { (void)p; }
void aud_mutex_unlock(void *p) { (void)p; }
static struct aud_port *waiting_port;
void aud_cond_wait(void *c, void *m)
{
    (void)c;
    (void)m;
    waits++;
    if (changed_to_flush)
        waiting_port->flushing = 1;
    else
        free_bytes = 65536;
}
int aud_event_value_get_int(struct aud_event *e, const char *k, int d)
{
    (void)e;
    (void)k;
    return d;
}
void *aud_event_serialize(struct aud_event *e, unsigned *n)
{
    (void)e;
    *n = 16;
    return malloc(16);
}
void aud_free(void *p) { free(p); }
static int get_free(struct adec_buf *b)
{
    (void)b;
    return free_bytes;
}
static int insert(void *p, unsigned n, struct au_info *i, struct adec_buf *b)
{
    (void)n;
    (void)b;
    writes++;
    written = *i;
    written_bytes = p;
    return 0;
}
int main(void)
{
    struct adec_buf b = {get_free, insert};
    struct aud_port p = {.legacy_buf = &b};
    struct aud_data d = {.type = 1, .size = 32768};
    char payload[32768] = {0};
    d.data = payload;
    d.object[1] = 0x64617461u;
    struct aud_object *data_pending[] = {(void *)&d};
    p.pending = data_pending;
    p.pending_num = 1;
    waiting_port = &p;
    free_bytes = 65536;
    reconstructed_port_write_shared(&p);
    assert(writes == 1 && written.data_type == 1 && written_bytes == payload);
    writes = 0;
    struct aud_object event = {.type = 0x65766e74u};
    struct aud_object *pending[] = {&event};
    p.pending = pending;
    p.pending_num = 1;
    waiting_port = &p;
    free_bytes = 65536;
    reconstructed_port_write_shared(&p);
    assert(writes == 1 && !waits && written.data_type == 4 && written_bytes);
    free_bytes = 0;
    reconstructed_port_write_shared(&p);
    assert(writes == 2 && waits == 1);
    free_bytes = 0;
    changed_to_flush = 1;
    reconstructed_port_write_shared(&p);
    assert(writes == 2 && waits == 2 && p.flushing);
    (void)d;
    puts(
        "shared transport: sufficient-space write, full-ring wait/wakeup, flush skips write: PASS");
    return 0;
}
