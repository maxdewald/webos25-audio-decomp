#include <stdbool.h>
#include <stddef.h>
struct aud_event;
struct aud_data;
struct delay;
#include <stdlib.h>
struct aud_data *aud_data_new(unsigned);
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
