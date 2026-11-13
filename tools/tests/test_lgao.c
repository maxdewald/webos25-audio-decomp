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
#define AREN_SET_MUTE 10
#define AREN_SET_SPEED 9
#define UNSET (~0u)
#define renderer_memset memset
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
static struct aud_module main_src = {0}, bypass_src = {1}, sink = {2};
static struct aud_event last_event[2];
static struct aud_data last_data[2];
static unsigned char payloads[2][32];
static const void *data_identity[2];
static int events[2], pushes[2], last_refs[2], live_objects, ioctls, closes, fail_data;
static struct aren_pcm_info last_pcm;
static unsigned disconnected[3], destroyed[3];
static int disconnects, destroys, messages, opens, connects, starts, stops, plays;
static int sequence, open_order, connect_order, start_order;
static int add_rts_order, rts_order, format_order, ease_order;
static LGAO_APPTYPE_T rts_mode;

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
    assert(m && block && d->size <= 32);
    assert(d->object.refs > 0);
    last_data[m->id] = *d;
    last_refs[m->id] = d->object.refs;
    data_identity[m->id] = d;
    memcpy(payloads[m->id], d->data, d->size);
    ++pushes[m->id];
    aud_object_put(&d->object);
    return -1; /* Recovered feed does not propagate push failure. */
}
static int mock_ioctl(int fd, unsigned command, void *arg)
{
    ++ioctls;
    if (command == AREN_SET_JP4K_STREAM) format_order = ++sequence;
    if (command == AREN_SET_PCM_INFO) last_pcm = *(struct aren_pcm_info *)arg;
    return 0;
}
static int mock_close(int fd) { ++closes; return 0; }
static int aimc_module_disconnect_output(struct aimc *imc, unsigned uid, int port)
{
    assert(port == 0 && disconnects < 3);
    disconnected[disconnects++] = uid;
    return 0;
}
static int aimc_module_disconnect_input(struct aimc *imc, unsigned uid, int port)
{
    return aimc_module_disconnect_output(imc, uid, port);
}
static int aimc_module_destroy(struct aimc *imc, unsigned uid)
{
    assert(destroys < 3);
    destroyed[destroys++] = uid;
    return 0;
}
static int mock_msgsnd(int mq, const void *msg, size_t size, int flags)
{
    const lgao_mq_item *item = msg;
    assert(size == sizeof(item->event) && item->event == LGAO_EVENT_BYPASS_DONE);
    ++messages;
    return 0;
}
static int _lgao_open_renderer(lgao *ao)
{
    assert(ao->fd == -1 && !ao->appsrc && !ao->bypass_appsrc);
    ao->fd = 8;
    ++opens; open_order = ++sequence;
    return 0;
}
static int _lgao_connect_renderer(lgao *ao)
{
    assert(ao->fd == 8 && ao->rendererState == LGAO_STATE_CONFIG);
    ao->appsrc = &main_src; ao->bypass_appsrc = &bypass_src; ao->appsink = &sink;
    ao->uid_appsrc = 11; ao->uid_bypass_appsrc = 12; ao->uid_appsink = 13;
    ++connects; connect_order = ++sequence;
    return 0;
}
int lgao_stop_renderer(void *a) { ++stops; return 0; }
int lgao_start_renderer(void *a) { ++starts; start_order = ++sequence; return 0; }
int lgao_play(void *a) { ((lgao *)a)->rendererState = LGAO_STATE_PLAY; ++plays; return 0; }
#define ioctl mock_ioctl
#define close mock_close
#define msgsnd mock_msgsnd
void lgao_add_rts(void *a) { add_rts_order = ++sequence; }
static void _lgao_config_set_rts_param(lgao *ao, LGAO_APPTYPE_T type)
{
    rts_mode = type;
    rts_order = ++sequence;
}
static void _lgao_config_set_easevol(lgao *ao) { ease_order = ++sequence; }
static void _lgao_write_dump(lgao *ao, void *buf, unsigned len) {}
static unsigned ns_to_pts(unsigned long long ts) { return ts * 9 / 100000; }

// @extract src/libgstlgaudiosink/lgao.c _lgao_config_set_renderer_param lgao_config lgao_set_first_render lgao_set_pcm_bypass lgao_set_pcm_multi_channel lgao_set_audio_info lgao_close_renderer lgao_flush lgao_eos _lgao_set_playspeed lgao_set_playrate lgao_compare_audio_info lgao_set_mute lgao_disconnect_renderer lgao_push_first_event lgao_feed lgao_set_drop_renderer

static lgao context(void)
{
    lgao ao = {0};
    ao.index = 0; ao.fd = 7; ao.playSpeed = 1000;
    ao.appsrc = &main_src; ao.bypass_appsrc = &bypass_src; ao.appsink = &sink;
    ao.uid_appsrc = 1; ao.uid_bypass_appsrc = 2; ao.uid_appsink = 3;
    ao.audioStartInfo.freq = 48000;
    ao.audioStartInfo.numberOfChannel = 6;
    ao.audioStartInfo.bitsPerSample = 32;
    strcpy(ao.audioStartInfo.codec_str, "ac3");
    for (int i = LGAO_DATA_PCM; i <= LGAO_DATA_METADATA; ++i) {
        ao.isFirstRender[i] = true;
        ao.prevTimestamp[i] = ~0ull;
    }
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
static const char *string_value(int dst, const char *key)
{
    struct aud_event *e = &last_event[dst];
    for (int i = 0; i < e->count; ++i)
        if (!strcmp(e->fields[i].key, key) && e->fields[i].type == aud_value_str)
            return e->fields[i].string;
    return NULL;
}
static void reset(void)
{
    assert(!live_objects);
    memset(events, 0, sizeof(events)); memset(pushes, 0, sizeof(pushes));
    ioctls = closes = fail_data = 0;
    disconnects = destroys = messages = opens = connects = starts = stops = plays = 0;
    sequence = open_order = connect_order = start_order = 0;
    add_rts_order = rts_order = format_order = ease_order = 0;
}
static void config_tests(void)
{
    const LGAO_APPTYPE_T modes[] = {
        LGAO_APPTYPE_RTC, LGAO_APPTYPE_MIRACAST, LGAO_APPTYPE_R2R,
        LGAO_APPTYPE_AIRPLAY, LGAO_APPTYPE_DIRECTMEDIA, LGAO_APPTYPE_UNKNOWN,
        LGAO_APPTYPE_JP4K,
    };
    for (unsigned index = 0; index < sizeof(modes) / sizeof(modes[0]); ++index) {
        reset();
        lgao ao = context();
        ao.isNoDelayMode = true;
        ao.appType = modes[index];
        assert(lgao_config(&ao) == 0);
        LGAO_APPTYPE_T expected = modes[index];
        if (expected == LGAO_APPTYPE_MIRACAST) expected = LGAO_APPTYPE_RTC;
        if (expected == LGAO_APPTYPE_JP4K) expected = LGAO_APPTYPE_UNKNOWN;
        assert(rts_mode == expected && ao.rendererState == LGAO_STATE_CONFIG);
        assert(add_rts_order < rts_order && rts_order < format_order);
        assert(format_order < ease_order && ioctls == 2);
    }
    reset();
    lgao ao = context();
    assert(lgao_config(&ao) == 0 && !add_rts_order && !rts_order);
    assert(format_order < ease_order && ioctls == 2);
    reset();
    ao.rendererState = LGAO_STATE_PLAY;
    ao.isNoDelayMode = true;
    assert(lgao_config(&ao) == 0 && !sequence && !ioctls);
    assert(ao.rendererState == LGAO_STATE_PLAY);
}
static void compare_caps_tests(void)
{
    lgao ao = context();
    LGAO_AUDIO_START_INFO_T caps = ao.audioStartInfo;
    caps.es = 1;
    caps.codecType = LGAO_CODEC_MAT;
    strcpy(caps.codec_str, "mat");
    caps.data_rate = 192000; caps.data_channels = 8; caps.data_bps = 16;
    caps.clock_x4 = 1; caps.downmix = 1; caps.acmod = 7;
    assert(lgao_compare_audio_info(&ao, &caps) == 0);
    caps.restart = 2;
    assert(lgao_compare_audio_info(&ao, &caps) == 0);
    caps.restart = 1;
    assert(lgao_compare_audio_info(&ao, &caps) == 2);
    for (int field = 0; field < 5; ++field) {
        caps = ao.audioStartInfo;
        if (field == 0) ++caps.freq;
        if (field == 1) ++caps.numberOfChannel;
        if (field == 2) ++caps.bitsPerSample;
        if (field == 3) ++caps.bigEndianOfPcm;
        if (field == 4) ++caps.unsignedOfPcm;
        assert(lgao_compare_audio_info(&ao, &caps) == 1);
    }
    ao.fd = -1; ao.audioStartInfo.codecType = LGAO_CODEC_UNKNOWN;
    assert(lgao_compare_audio_info(&ao, &caps) == -1);
    ao.audioStartInfo.codecType = LGAO_CODEC_PCM;
    assert(lgao_compare_audio_info(&ao, &caps) == 1);
    assert(lgao_compare_audio_info(NULL, &caps) == -1);
}
int main(void)
{
    config_tests();
    compare_caps_tests();
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
    assert(last_refs[0] == 2 && last_refs[1] == 2);
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
    ao.bufferedSize[0] = ~0u - 1;
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_PCM) == 4);
    assert(!events[0] && !pushes[0] && !ao.isFirstRender[0] && ao.bufferedSize[0] == 2);
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
    reset(); ao = context();
    strcpy(ao.audioStartInfo.codec_str, "mat");
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_ES) == 4);
    assert(value(1, "es", aud_value_bool, -1) == 1);
    assert(!strcmp(string_value(1, "codec"), "mat"));
    assert(!ao.pcm_multi_ch && !ao.pcm_bypass);
    bool saved_latches[LGAO_DATA_TYPE_NUM];
    memcpy(saved_latches, ao.isFirstRender, sizeof(saved_latches));
    unsigned saved_buffered[LGAO_DATA_TYPE_NUM];
    memcpy(saved_buffered, ao.bufferedSize, sizeof(saved_buffered));
    struct aud_data *direct = aud_data_new(4);
    direct->info.type = type_pcm;
    memcpy(direct->data, bytes, 4);
    assert(module_appsrc_push_data(ao.bypass_appsrc, direct, true) == -1);
    assert(events[0] == 1 && events[1] == 1 && !pushes[0] && pushes[1] == 2);
    assert(last_data[1].info.type == type_pcm && last_refs[1] == 1 && !live_objects);
    assert(!strcmp(string_value(1, "codec"), "mat") && !ao.isFirstRender[0]);
    assert(!memcmp(saved_latches, ao.isFirstRender, sizeof(saved_latches)));
    assert(!memcmp(saved_buffered, ao.bufferedSize, sizeof(saved_buffered)));
    assert(!ao.pcm_multi_ch && !ao.pcm_bypass);
    assert(pthread_mutex_init(&ao.flush_mutex, NULL) == 0);
    /* FLUSH_STOP then SEGMENT: rate zero/flush/rate restore, not FIRST replay. */
    assert(lgao_set_playrate(&ao, 0) == 0);
    assert(lgao_flush(&ao) == 0 && !ao.isFirstRender[0] && !ao.isFirstRender[1]);
    assert(lgao_set_playrate(&ao, 1000) == 0);
    assert(!memcmp(saved_latches, ao.isFirstRender, sizeof(saved_latches)));
    assert(pthread_mutex_destroy(&ao.flush_mutex) == 0);
    assert(lgao_set_mute(&ao, 1) == 0 && !ao.isFirstRender[0]);
    int before_eos = events[0];
    assert(lgao_eos(&ao) == 1 && events[0] == before_eos && !ao.isFirstRender[0]);
    assert(lgao_set_mute(&ao, 0) == 0 && !ao.isFirstRender[0]);
    ao.playSpeed = 249;
    assert(lgao_eos(&ao) == 1 && !ao.isFirstRender[0]);
    ao.playSpeed = 2001;
    assert(lgao_eos(&ao) == 1 && !ao.isFirstRender[0]);
    ao.playSpeed = 1000;
    ao.drop_renderer = 1;
    assert(lgao_eos(&ao) == 1 && !ao.isFirstRender[0]);
    ao.drop_renderer = 0;
    assert(lgao_set_pcm_multi_channel(&ao, 1) == 0 && !ao.isFirstRender[0]);
    assert(lgao_eos(&ao) == 0 && ao.isFirstRender[0] && !ao.isFirstRender[1]);
    assert(events[0] == before_eos);
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_PCM) == 4);
    assert(events[0] == before_eos + 1 && value(1, "es", aud_value_bool, -1) == -1);
    assert(string_value(1, "codec") == NULL);
    assert(lgao_set_pcm_multi_channel(&ao, 0) == 0 && !ao.isFirstRender[0]);
    assert(lgao_push_first_event(&ao) == 0 && value(1, "es", aud_value_bool, -1) == 1);
    assert(!strcmp(string_value(1, "codec"), "mat"));
    strcpy(ao.audioStartInfo.codec_str, "ac3");
    assert(lgao_set_audio_info(&ao, ao.audioStartInfo) == 0 && !ao.isFirstRender[0]);
    assert(lgao_push_first_event(&ao) == 0);
    assert(!strcmp(string_value(1, "codec"), "ac3"));

    reset(); ao = context();
    strcpy(ao.audioStartInfo.codec_str, "mat");
    ao.rendererState = LGAO_STATE_PLAY;
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_ES) == 4);
    assert(lgao_eos(&ao) == 0 && events[0] == 3 && events[1] == 3);
    assert(ao.isFirstRender[0] && !ao.isFirstRender[1]);
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_ES) == 4);
    assert(events[1] == 3 && ao.isFirstRender[0]);
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_PCM) == 4);
    assert(events[1] == 4 && !ao.isFirstRender[0]);
    assert(value(1, "es", aud_value_bool, -1) == 1);
    assert(!strcmp(string_value(1, "codec"), "mat"));

    reset(); ao = context();
    memset(ao.isFirstRender, 0, sizeof(ao.isFirstRender));
    assert(lgao_disconnect_renderer(&ao) == 0 && disconnects == 3 && destroys == 3);
    assert(disconnected[0] == 2 && disconnected[1] == 1 && disconnected[2] == 3);
    assert(destroyed[0] == 1 && destroyed[1] == 2 && destroyed[2] == 3);
    assert(!ao.appsrc && !ao.bypass_appsrc && !ao.appsink);
    assert(!ao.uid_appsrc && !ao.uid_bypass_appsrc && !ao.uid_appsink);
    assert(!ao.isFirstRender[0] && ao.fd == 7);
    assert(lgao_disconnect_renderer(&ao) == 0 && disconnects == 3 && destroys == 3);
    assert(lgao_close_renderer(&ao) == 0 && ao.fd == -1 && ao.isFirstRender[0]);

    reset(); ao = context();
    strcpy(ao.audioStartInfo.codec_str, "mat");
    ao.rendererState = LGAO_STATE_PLAY;
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_ES) == 4);
    assert(lgao_set_pcm_bypass(&ao, 1) == 0 && !ao.isFirstRender[0]);
    assert(lgao_set_drop_renderer(&ao, 1) == 0);
    assert(messages == 1 && closes == 1 && stops == 1 && ao.fd == -1);
    assert(!ao.appsrc && !ao.bypass_appsrc && ao.isFirstRender[0]);
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_PCM) == 4);
    assert(!ao.isFirstRender[0] && !pushes[0] && pushes[1] == 1);
    assert(lgao_set_drop_renderer(&ao, 0) == 0);
    assert(opens == 1 && connects == 1 && plays == 1 && starts == 1);
    assert(open_order < connect_order && connect_order < start_order);
    assert(ao.appsrc && ao.bypass_appsrc && ao.uid_bypass_appsrc == 12);
    assert(events[0] == 2 && events[1] == 2);
    assert(value(1, "es", aud_value_bool, -1) == -1 && string_value(1, "codec") == NULL);
    assert(!ao.isFirstRender[0] && !ao.isFirstRender[1]);
    assert(lgao_set_drop_renderer(&ao, 0) == 0 && events[1] == 2 && opens == 1);

    reset(); ao = context();
    strcpy(ao.audioStartInfo.codec_str, "mat");
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_ES) == 4);
    LGAO_AUDIO_START_INFO_T caps = ao.audioStartInfo;
    assert(lgao_compare_audio_info(&ao, &caps) == 0);
    caps.restart = 1;
    assert(lgao_compare_audio_info(&ao, &caps) == 2);
    assert(lgao_set_audio_info(&ao, caps) == 0 && !ao.isFirstRender[1]);
    assert(events[1] == 1 && !closes && !connects);
    caps.freq = 96000;
    assert(lgao_compare_audio_info(&ao, &caps) == 1);
    /* set_caps format-change sequence; FIRST waits for the next nonempty feed. */
    assert(lgao_stop_renderer(&ao) == 0);
    assert(lgao_disconnect_renderer(&ao) == 0);
    assert(lgao_close_renderer(&ao) == 0);
    assert(_lgao_open_renderer(&ao) == 0);
    assert(lgao_set_audio_info(&ao, caps) == 0);
    assert(lgao_config(&ao) == 0);
    assert(_lgao_connect_renderer(&ao) == 0);
    assert(ao.isFirstRender[0] && ao.isFirstRender[1] && events[1] == 1);
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_ES) == 4);
    assert(events[1] == 2 && value(1, "es", aud_value_bool, -1) == 1);
    assert(!strcmp(string_value(1, "codec"), "mat"));

    reset(); ao = context();
    strcpy(ao.audioStartInfo.codec_str, "mat");
    assert(lgao_feed(&ao, bytes, 4, 0, LGAO_DATA_ES) == 4);
    assert(lgao_set_drop_renderer(&ao, 1) == 0);
    assert(lgao_set_drop_renderer(&ao, 0) == 0);
    assert(!plays && !starts && events[1] == 2);
    assert(value(1, "es", aud_value_bool, -1) == 1);
    assert(!strcmp(string_value(1, "codec"), "mat") && !live_objects);
    puts("lgao transport: routing, FIRST replacement, lifetimes, teardown/rebuild and failure contracts passed");
    return 0;
}
