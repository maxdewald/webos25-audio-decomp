#include <stdbool.h>
#include <stddef.h>
struct aud_event;
struct aud_data;
struct delay;
#include <stdlib.h>
struct aud_data *aud_data_new(unsigned);
int delay_queue_fadeout(struct delay *d)
{
    (void)d;
    abort();
}
void delay_queue_flush_pcm_unrecovered(struct delay *d, int push)
{
    (void)d;
    (void)push;
    abort();
}
struct aud_data *aud_data_new_zero(unsigned n) { return aud_data_new(n); }
void aud_data_adapter_flush(void *p, unsigned n)
{
    (void)p;
    (void)n;
}
struct aud_event *aud_event_newv_custom(const char *n, ...)
{
    (void)n;
    return NULL;
}
int _pending_events_add(void *a, struct aud_event *e, void *q, void *m,
                        int (*p)(void *, struct aud_event *))
{
    (void)a;
    (void)e;
    (void)q;
    (void)m;
    (void)p;
    return 0;
}
int aud_module_push_event(void *m, int p, struct aud_event *e)
{
    (void)m;
    (void)p;
    (void)e;
    return 0;
}
void delay_fadein(struct delay *d, void *p, unsigned n)
{
    (void)d;
    (void)p;
    (void)n;
}
void delay_fadeout(struct delay *d, void *p, unsigned n)
{
    (void)d;
    (void)p;
    (void)n;
}
