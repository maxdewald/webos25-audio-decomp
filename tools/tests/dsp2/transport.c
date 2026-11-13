#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#undef memcpy
#include "../../../src/dsp2/glue.c"
// @extract src/dsp2/module_core.c reconstructed_port_release_pending
struct aud_module { void *pending_root; };
static struct aud_port *selected_output;
static unsigned sync_callbacks;
static unsigned all_output_walks;
struct aud_port *aud_module_get_output(struct aud_module *module, int number)
{
    assert(module && number == 0);
    return selected_output;
}
int aud_port_sync_locked(void *context, struct aud_port *port)
{
    assert(context == NULL && port == selected_output);
    ++sync_callbacks;
    return -1;
}
void aud_link_foreach_cb(void *root, int argument,
                         int (*callback)(void *, struct aud_port *))
{
    assert(root && argument == 0);
    ++all_output_walks;
    callback(NULL, selected_output);
}
// @extract src/dsp2/module_core.c aud_module_sync
static int free_bytes, waits, writes, changed_to_flush;
static struct au_info written;
static void *written_bytes;
void aud_mutex_lock(void *p) { (void)p; }
void aud_mutex_unlock(void *p) { (void)p; }
static struct aud_port *waiting_port;
static struct aud_object *released[4];
static unsigned released_count;
static struct aud_object **replacement_pending;
void aud_object_put(void *object)
{
    struct aud_object *owned = object;
    assert(owned->ref > 0 && released_count < 4);
    released[released_count++] = owned;
    if (--owned->ref == 0 && replacement_pending) {
        waiting_port->pending = replacement_pending;
        waiting_port->pending_num = 2;
        replacement_pending = NULL;
    }
}
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
    struct aud_object first = {.ref = 1}, next = {.ref = 2};
    struct aud_object *initial[] = {&first};
    struct aud_object *replacement[] = {&first, &next};
    p.pending = initial;
    p.pending_num = 1;
    replacement_pending = replacement;
    reconstructed_port_release_pending(&p);
    assert(released_count == 2 && released[0] == &first && released[1] == &next);
    assert(first.ref == 0 && next.ref == 1 && p.pending_num == 0);
    reconstructed_port_release_pending(&p);
    assert(released_count == 2 && next.ref == 1);
    struct aud_module module = {0};
    selected_output = &p;
    assert(aud_module_sync(&module, 0) == 0 && sync_callbacks == 1);
    assert(aud_module_sync(&module, -1) == 0 && sync_callbacks == 2);
    assert(all_output_walks == 1);
    selected_output = NULL;
    assert(aud_module_sync(&module, 0) == 0 && sync_callbacks == 2);
    (void)d;
    puts(
        "shared transport: write/wakeup/flush, live pending release and retained peer ref: PASS");
    return 0;
}
