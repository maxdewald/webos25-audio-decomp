#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libgstlgaudiosink/lgao.h"
#include "include/lg_audio_policy.h"

#define LOGE(...) ((void)0)
#define LOGN(...) ((void)0)
#define LOGI(...) ((void)0)
#define LOGD(...) ((void)0)
#define AREN_SET_JP4K_STREAM 20
#define UNSET (~0u)
struct aud_module { int id; };
struct aud_object { int refs, kind; };
enum aud_value_type { aud_value_int, aud_value_bool, aud_value_str };
enum aud_data_type { type_pcm, type_es, type_meta, type_meta_ext };
struct field { const char *key; int type, number; const char *string; };
struct aud_event { struct aud_object object; int count; struct field fields[24]; };
struct aud_data {
    struct aud_object object;
    struct { int type; unsigned timestamp; unsigned long long timestamp64; } info;
    unsigned char *data;
    unsigned size;
};
static struct aud_module main_src = {0}, bypass_src = {1};
static struct aud_event last_event[2];
static struct aud_data last_data[2];
static unsigned char payloads[2][32];
static const void *data_identity[2];
static int events[2], pushes[2], live_objects, ioctls, closes, fail_data;
static struct aren_pcm_info last_pcm;

static struct aud_object *aud_object_get(struct aud_object *o)
{
    assert(o->refs > 0);
    ++o->refs;
    return o;
}
static void aud_object_put(struct aud_object *o)
{
    assert(o && o->refs > 0);
    if (--o->refs) return;
    if (o->kind == 1) free(((struct aud_data *)o)->data);
    --live_objects;
    free(o);
}
static struct aud_event *new_event(void)
{
    struct aud_event *e = calloc(1, sizeof(*e));
    assert(e);
    e->object.refs = 1;
    ++live_objects;
    return e;
}
static struct field *field(struct aud_event *e, const char *key)
{
    for (int i = 0; i < e->count; ++i)
        if (!strcmp(e->fields[i].key, key)) return &e->fields[i];
    assert(e->count < 24);
    struct field *f = &e->fields[e->count++];
    f->key = key;
    return f;
}
static void fields(struct aud_event *e, const char *key, va_list ap)
{
    while (key) {
        struct field *f = field(e, key);
        f->type = va_arg(ap, int);
        if (f->type == aud_value_str) f->string = va_arg(ap, const char *);
        else f->number = va_arg(ap, int);
        key = va_arg(ap, const char *);
    }
}
static struct aud_event *aud_event_newv_first(const char *key, ...)
{
    struct aud_event *e = new_event();
    va_list ap;
    va_start(ap, key); fields(e, key, ap); va_end(ap);
    return e;
}
static struct aud_event *aud_event_dup_and_set(struct aud_event *e, const char *key, ...)
{
    struct aud_event *copy = new_event();
    *copy = *e;
    copy->object.refs = 1;
    va_list ap;
    va_start(ap, key); fields(copy, key, ap); va_end(ap);
    return copy;
}
static void aud_event_value_set_int(struct aud_event *e, const char *key, int v)
{
    struct field *f = field(e, key);
    f->type = aud_value_int; f->number = v;
}
static void aud_event_value_set_bool(struct aud_event *e, const char *key, int v)
{
    struct field *f = field(e, key);
    f->type = aud_value_bool; f->number = v;
}
static char *aud_event_str(struct aud_event *e) { return NULL; }
static struct aud_event *aud_event_newv_custom(const char *key, ...) { return new_event(); }
static struct aud_event *aud_event_new_last(void) { return new_event(); }
static struct aud_event *aud_event_new_flush(void) { return new_event(); }
static struct aud_event *aud_event_new_flush_async(void) { return new_event(); }
static int module_appsrc_push_event(struct aud_module *m, struct aud_event *e)
{
    last_event[m->id] = *e;
    ++events[m->id];
    aud_object_put(&e->object);
    return -1; /* FIRST latches clear even when submission reports failure. */
}
static struct aud_data *aud_data_new(unsigned size)
{
    if (fail_data) return NULL;
    struct aud_data *d = calloc(1, sizeof(*d));
    assert(d);
    d->object.refs = 1; d->object.kind = 1;
    d->data = malloc(size); assert(d->data); d->size = size;
    ++live_objects;
    return d;
}
static int module_appsrc_push_data(struct aud_module *m, struct aud_data *d, bool block)
{
    assert(block && d->size <= 32);
    last_data[m->id] = *d;
    data_identity[m->id] = d;
    memcpy(payloads[m->id], d->data, d->size);
    ++pushes[m->id];
    aud_object_put(&d->object);
    return -1; /* Recovered feed does not propagate push failure. */
}
static int mock_ioctl(int fd, unsigned command, void *arg)
{
    ++ioctls;
    if (command == AREN_SET_PCM_INFO) last_pcm = *(struct aren_pcm_info *)arg;
    return 0;
}
static int mock_close(int fd) { ++closes; return 0; }
#define ioctl mock_ioctl
#define close mock_close
void lgao_add_rts(void *a) {}
static void _lgao_config_set_rts_param(lgao *ao, LGAO_APPTYPE_T type) {}
static void _lgao_config_set_easevol(lgao *ao) {}
static void _lgao_write_dump(lgao *ao, void *buf, unsigned len) {}
static unsigned ns_to_pts(unsigned long long ts) { return ts * 9 / 100000; }

/* RECOVERED_FUNCTIONS */

static lgao context(void)
{
    lgao ao = {0};
    ao.index = 0; ao.fd = 7; ao.playSpeed = 1000;
    ao.appsrc = &main_src; ao.bypass_appsrc = &bypass_src;
    ao.audioStartInfo.freq = 48000;
    ao.audioStartInfo.numberOfChannel = 6;
    ao.audioStartInfo.bitsPerSample = 32;
    strcpy(ao.audioStartInfo.codec_str, "ac3");
    for (int i = 0; i < LGAO_DATA_TYPE_NUM; ++i) ao.isFirstRender[i] = true;
    return ao;
}
static int value(int dst, const char *key, int type, int fallback)
{
    struct aud_event *e = &last_event[dst];
    for (int i = 0; i < e->count; ++i)
        if (!strcmp(e->fields[i].key, key) && e->fields[i].type == type)
            return e->fields[i].number;
    return fallback;
}
static void reset(void)
{
    assert(!live_objects);
    memset(events, 0, sizeof(events)); memset(pushes, 0, sizeof(pushes));
    ioctls = closes = fail_data = 0;
}
int main(void)
{
    unsigned char bytes[] = {1, 3, 5, 7};
    lgao ao = context();
    assert(lgao_feed(&ao, bytes, 4, 1000000000, LGAO_DATA_ES) == 4);
    assert(events[0] == 1 && events[1] == 1 && !pushes[0] && pushes[1] == 1);
    assert(value(1, "es", aud_value_bool, -1) == 1);
    assert(value(1, "es", aud_value_int, -1) == -1);
    assert(value(0, "es", aud_value_bool, -1) == -1);
    assert(last_data[1].info.type == type_es && last_data[1].info.timestamp == 90000);
    assert(ao.audioStartInfo.data_channels == 2 && ao.audioStartInfo.data_bps == 16);
    assert(ao.audioStartInfo.data_rate == 48000);
    for (int i = 0; i < LGAO_DATA_TYPE_NUM; ++i) assert(!ao.isFirstRender[i]);
    assert(lgao_feed(&ao, bytes, 4, ~0ull, LGAO_DATA_PCM) == 4);
    assert(events[0] == 1 && pushes[0] == 1 && pushes[1] == 1);
    assert(last_data[0].info.type == type_pcm);
    assert(lgao_set_pcm_multi_channel(&ao, 9) == 0 && ao.pcm_multi_ch && !ioctls);
    assert(lgao_feed(&ao, bytes, 4, 2000000000, LGAO_DATA_PCM) == 4);
    assert(events[0] == 1 && events[1] == 1);
    assert(data_identity[0] == data_identity[1]);
    assert(last_data[1].info.type == type_pcm && !memcmp(payloads[1], bytes, 4));
    assert(!memcmp(payloads[0], payloads[1], 4));
    assert(lgao_set_first_render(&ao, LGAO_DATA_PCM, 1) == 0);
    assert(lgao_feed(&ao, bytes, 4, 3000000000, LGAO_DATA_PCM) == 4);
    assert(events[0] == 2 && events[1] == 2);
    assert(value(1, "es", aud_value_bool, -1) == -1);
    assert(value(1, "acmod", aud_value_int, -1) == 7);
    assert(value(1, "ch-mapping-lfe-c", aud_value_bool, -1) == -1);
    assert(lgao_set_pcm_bypass(&ao, 1) == 0 && !ioctls);
    ao.audioStartInfo.numberOfChannel = 2;
    assert(lgao_push_first_event(&ao) == 0 && events[0] == 3);
    assert(value(0, "acmod", aud_value_int, -1) == 2);
    assert(value(1, "ch-mapping-lfe-c", aud_value_bool, -1) == 1);
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_METADATA2) == 4);
    assert(last_data[0].info.type == type_meta_ext && last_data[1].info.type == type_meta_ext);
    assert(lgao_set_audio_info(&ao, ao.audioStartInfo) == 0 && !ao.isFirstRender[0]);
    assert(lgao_config(&ao) == 0 && ioctls == 2);
    assert(last_pcm.pcm_multi_ch == 1 && last_pcm.pcm_bypass == 1);
    ao.rendererState = LGAO_STATE_PLAY;
    assert(lgao_set_pcm_multi_channel(&ao, 0) == 0);
    assert(lgao_config(&ao) == 0 && ioctls == 2 && last_pcm.pcm_multi_ch == 1);
    assert(pthread_mutex_init(&ao.flush_mutex, NULL) == 0);
    assert(lgao_flush(&ao) == 0 && !ao.isFirstRender[0]);
    assert(pthread_mutex_destroy(&ao.flush_mutex) == 0);
    assert(lgao_eos(&ao) == 0 && ao.isFirstRender[LGAO_DATA_PCM]);
    assert(!ao.isFirstRender[LGAO_DATA_ES]);
    assert(lgao_close_renderer(&ao) == 0 && closes == 1);
    for (int i = LGAO_DATA_PCM; i <= LGAO_DATA_METADATA; ++i) assert(ao.isFirstRender[i]);
    assert(!ao.isFirstRender[LGAO_DATA_METADATA2]);
    reset(); ao = context(); ao.drop_renderer = 1;
    assert(lgao_push_first_event(&ao) == 0 && !events[0] && ao.isFirstRender[0]);
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_PCM) == 4);
    assert(!events[0] && !pushes[0] && !ao.isFirstRender[0] && ao.bufferedSize[0] == 4);
    reset(); ao = context();
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_PCM) == 4);
    assert(pushes[0] == 1 && !pushes[1] && events[0] == 1 && events[1] == 1);
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_METADATA) == 4);
    assert(last_data[0].info.type == type_meta && !pushes[1] && events[0] == 1);
    reset(); ao = context();
    assert(lgao_feed(&ao, bytes, 0, 0, LGAO_DATA_PCM) == 0 && ao.isFirstRender[0]);
    fail_data = 1;
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_PCM) == 0);
    assert(events[0] == 1 && !pushes[0] && !ao.isFirstRender[0]);
    assert(!ao.bufferedSize[0] && !live_objects);
    assert(lgao_set_pcm_multi_channel(NULL, 1) == -1);
    assert(lgao_feed(NULL, bytes, 4, 0, LGAO_DATA_PCM) == -1);
    puts("lgao transport: routing, typed FIRSTs, latches, cache/ioctl order and drop/failure contracts passed");
    return 0;
}
