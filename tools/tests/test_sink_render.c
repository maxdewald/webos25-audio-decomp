/* Host mocks: execute extracted render_element.c, not a hand-copied model. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "libgstlgaudiosink/lgao.h"
typedef int gboolean, gint, GstFlowReturn, GstEventType, GstAudioFormat;
typedef unsigned guint;
typedef uint64_t guint64, GstClockTime;
typedef int64_t gint64;
typedef size_t gsize;
typedef char gchar;
#define TRUE 1
#define FALSE 0
#define GST_FLOW_OK 0
#define GST_STATE_READY 2
#define GST_STATE_PAUSED 3
#define GST_STATE_PLAYING 4
#define GST_CLOCK_TIME_NONE UINT64_MAX
#define GST_CLOCK_TIME_IS_VALID(t) ((t) != GST_CLOCK_TIME_NONE)
#define GST_BUFFER_FLAG_LAST 0x100000u
#define GST_BUFFER_FLAG_CORRUPTED 0x100u
#define GST_BUFFER_COPY_ALL 15
#define GST_MAP_READ 1
#define GST_AUDIO_FORMAT_UNKNOWN 0
#define GST_TYPE_BITMASK 1
#define G_TYPE_STRING 2
#define G_LITTLE_ENDIAN 1234
#define GST_MESSAGE_APPLICATION 1
#define G_TIME_SPAN_SECOND 1000000
#define CAPS_LOG_MAX_LEN 0x400
#define GST_SEGMENT_FORMAT "s"
#define SINK_LOG(...) ((void)0)
#define SINK_GST(...) ((void)0)
#define SINK_CHECK(call, line) ((void)(call))
#define GST_ELEMENT_WARNING(...) (++warnings)
typedef struct { double rate; guint64 start, stop, position, base; } GstSegment;
typedef struct { void *data; gsize size; } GstMapInfo;
typedef struct { guint flags; guint64 pts; gsize size, offset; int sub; } GstBuffer;
typedef struct { const char *key, *str; guint64 value; } Field;
typedef struct { const char *name; Field fields[32]; int n; } GstStructure;
typedef struct { GstStructure *s; } GstCaps;
typedef struct { int type, unrefs; GstStructure *s; GstCaps *caps; const char *id; GstSegment *segment; } GstEvent;
typedef struct { int width, endianness, integer, sign; } GstAudioFormatInfo;
typedef struct {
    void *ao, *clock, *sinkpad;
    GstClockTime base_time, currentPTS, segment_start, segment_position, decodedSize;
    GstSegment segment;
    int bClockCalibrated, object_ui_changed, enable_dump, isUnsupportedStream;
    int currentState, isFlushing, bSeamlessSeek, isSeamless, avail_irc_vdec_pts;
    int eos_mutex, eos_cond;
    double playrate;
    FILE *dump_file;
    GstCaps *pAudioCaps;
    char *pAppType, *pStreamId;
} GstLgAudioRender;
typedef GstLgAudioRender GstBaseSink, GstElement;
#define GST_LG_AUDIO_RENDER(s) (s)
#define GST_ELEMENT(s) (s)
#define GST_OBJECT(s) (s)
#define GST_BASE_SINK(s) (s)
#define GST_BASE_SINK_PAD(s) ((s)->sinkpad)
#define GST_BUFFER_FLAGS(b) ((b)->flags)
#define GST_BUFFER_PTS(b) ((b)->pts)
#define GST_BUFFER_PTS_IS_VALID(b) GST_CLOCK_TIME_IS_VALID((b)->pts)
#define GST_EVENT_TYPE(e) ((e)->type)
#define GST_EVENT_TYPE_NAME(e) "event"
#define GST_AUDIO_FORMAT_INFO_IS_INTEGER(f) ((f)->integer)
#define GST_AUDIO_FORMAT_INFO_IS_SIGNED(f) ((f)->sign)
enum { GST_EVENT_GAP, GST_EVENT_FLUSH_START, GST_EVENT_FLUSH_STOP, GST_EVENT_EOS,
       GST_EVENT_SEGMENT, GST_EVENT_CAPS, GST_EVENT_STREAM_START,
       GST_EVENT_CUSTOM_DOWNSTREAM, GST_EVENT_CUSTOM_DOWNSTREAM_OOB };
static int warnings, speed, mute, dropped, compare_result, port, feed_result;
static int feeds, copies, maps, unmaps, unrefs, copy_fail, map_fail, ui, dump_opens;
static int buffered, first, eos_result, waits, parents, parent_result, ring_pending;
static int clock_updates, info_sets, caps_frees, first_pushes, generation;
static gint64 clock_base, clock_start;
static char calls[256];
static int ncalls;
static gsize offsets[16], sizes[16];
static GstClockTime timestamps[16];
static LGAO_DATA_TYPE_T types[16];
static LGAO_AUDIO_START_INFO_T saved_info;
static unsigned char payload[0x30000];
static void record(char c) { assert(ncalls < 255); calls[ncalls++] = c; calls[ncalls] = 0; }
static char *g_strdup(const char *s) { size_t n = strlen(s) + 1; char *p = malloc(n); assert(p); return memcpy(p, s, n); }
#define g_free free
static Field *lookup(const GstStructure *s, const char *key) { for (int i = 0; i < s->n; ++i) if (!strcmp(s->fields[i].key, key)) return (Field *)&s->fields[i]; return NULL; }
static void number(GstStructure *s, const char *key, guint64 v) { s->fields[s->n++] = (Field){key, NULL, v}; }
static void string(GstStructure *s, const char *key, const char *v) { s->fields[s->n++] = (Field){key, v, 0}; }
static gboolean gst_structure_get_int(const GstStructure *s, const char *k, gint *p) { Field *f = lookup(s,k); if (!f) return 0; *p = (int)f->value; return 1; }
static gboolean gst_structure_get_uint(const GstStructure *s, const char *k, guint *p) { Field *f = lookup(s,k); if (!f) return 0; *p = (guint)f->value; return 1; }
#define gst_structure_get_boolean gst_structure_get_int
static gboolean gst_structure_get_uint64(const GstStructure *s, const char *k, guint64 *p) { Field *f = lookup(s,k); if (!f) return 0; *p = f->value; return 1; }
static const char *gst_structure_get_string(const GstStructure *s, const char *k) { Field *f = lookup(s,k); return f ? f->str : NULL; }
static const char *gst_structure_get_name(const GstStructure *s) { return s->name; }
static gboolean gst_structure_get(const GstStructure *s, const char *k, int type, guint64 *v, void *end) { return gst_structure_get_uint64(s,k,v); }
static int gst_structure_has_name(const GstStructure *s, const char *name) { return !strcmp(s->name,name); }
static int gst_structure_has_field(const GstStructure *s, const char *k) { return lookup(s,k) != NULL; }
static GstStructure *gst_structure_new_empty(const char *name) { GstStructure *s = calloc(1,sizeof(*s)); assert(s); s->name = name; return s; }
static void gst_structure_set(GstStructure *s, const char *k, ...) { }
static char *gst_structure_to_string(GstStructure *s) { return NULL; }
static char *gst_caps_to_string(GstCaps *c) { return NULL; }
static GstCaps *gst_caps_copy(GstCaps *c) { GstCaps *p = malloc(sizeof(*p)); assert(p); *p = *c; return p; }
static void gst_caps_unref(GstCaps *c) { ++caps_frees; free(c); }
static GstStructure *gst_caps_get_structure(GstCaps *c, int i) { return c->s; }
static char *gst_pad_get_stream_id(void *pad) { return g_strdup("stream"); }
static void *gst_message_new_custom(int type, void *obj, GstStructure *s) { return s; }
static int gst_element_post_message(void *e, void *m) { free(m); record('M'); return 1; }
static GstAudioFormat gst_audio_format_from_string(const char *s) { return !strcmp(s,"S16LE") ? 1 : !strcmp(s,"F32LE") ? 2 : 0; }
static const GstAudioFormatInfo *gst_audio_format_get_info(int f) { static GstAudioFormatInfo i = {16,1234,1,1}, fl = {32,1234,0,1}; return f == 1 ? &i : &fl; }
static gsize gst_buffer_get_size(GstBuffer *b) { return b->size; }
static GstBuffer *gst_buffer_copy_region(GstBuffer *b, int flags, gsize offset, gsize size) {
    assert(flags == 15); assert(copies < 16); offsets[copies] = offset; sizes[copies++] = size;
    if (copy_fail) { --copy_fail; return NULL; }
    GstBuffer *p = malloc(sizeof(*p)); assert(p); *p = *b; p->offset = offset; p->size = size; p->sub = 1; return p;
}
static int gst_buffer_map(GstBuffer *b, GstMapInfo *m, int flags) { ++maps; if (map_fail) { --map_fail; return 0; } m->size = b->size; m->data = payload + b->offset; return 1; }
static void gst_buffer_unmap(GstBuffer *b, GstMapInfo *m) { ++unmaps; }
static void gst_buffer_unref(GstBuffer *b) { ++unrefs; free(b); }
static GstEvent *gst_event_new_custom(int type, GstStructure *s) { GstEvent *e = calloc(1,sizeof(*e)); assert(e); e->type = type; e->s = s; return e; }
static int gst_pad_push_event(void *p, GstEvent *e) { ++ui; record('U'); free(e->s); free(e); return 1; }
static void gst_event_unref(GstEvent *e) { ++e->unrefs; }
static void gst_event_parse_caps(GstEvent *e, GstCaps **c) { *c = e->caps; }
static void gst_event_parse_stream_start(GstEvent *e, const char **id) { *id = e->id; }
static void gst_event_parse_segment(GstEvent *e, const GstSegment **s) { *s = e->segment; }
static const GstStructure *gst_event_get_structure(GstEvent *e) { return e->s; }
static int gst_event_has_name(GstEvent *e, const char *name) { return e->s && gst_structure_has_name(e->s,name); }
static char *gst_info_strdup_printf(const char *f, ...) { return NULL; }
static guint64 gst_clock_get_time(void *clock) { return 10000; }
static void gst_base_sink_set_render_delay(GstBaseSink *s, int d) { record('D'); }
static gint64 g_get_monotonic_time(void) { return 500; }
static void g_mutex_lock(int *m) { record('L'); }
static void g_mutex_unlock(int *m) { record('N'); }
static int g_cond_wait_until(int *c, int *m, gint64 end) { assert(end == 1000500); ++waits; record('W'); return 0; }
static int parent_event(GstBaseSink *s, GstEvent *e) { ++parents; record('P'); return parent_result; }
static struct { int (*event)(GstBaseSink *, GstEvent *); } parent = {parent_event}, *parent_class = &parent;
#define GST_BASE_SINK_CLASS(p) (p)
static void gst_lg_audio_render_update_clock(GstLgAudioRender *s, gint64 base, gint64 start) { ++clock_updates; clock_base = base; clock_start = start; record('T'); }
static void gst_lg_audio_render_update_ts_offset(GstLgAudioRender *s, int rate) { record('O'); }
static void gst_lg_audio_render_dump_open(GstLgAudioRender *s) { ++dump_opens; record('V'); }
static void gst_lg_audio_render_log_timestamp(GstLgAudioRender *s, GstBuffer *b, LGAO_DATA_TYPE_T t, guint len) { record('I'); }
static const char *out_type_name(LGAO_DATA_TYPE_T t) { return "mock"; }
int lgao_get_playrate(void *a, int *p) { *p = speed; record('r'); return 0; }
int lgao_get_nodelay(void *a, int *p) { *p = 1; record('n'); return 0; }
int lgao_get_mute(void *a, int *p) { *p = mute; record('m'); return 0; }
int lgao_get_mode_mixer(void *a, int *p) { *p = 0; return 0; }
int lgao_get_port(void *a, int *p) { *p = port; return 0; }
int lgao_get_drop_renderer(void *a) { return dropped; }
int lgao_set_drop_renderer(void *a, int d) { record(d ? 'X' : 'x'); if (d != dropped) ++generation; dropped = d; return 0; }
static int real_comparison;
static lgao live_ao;
#define LOGE(...) ((void)0)
#define lgao_compare_audio_info recovered_compare_audio_info
// @extract src/libgstlgaudiosink/lgao.c lgao_compare_audio_info
#undef lgao_compare_audio_info
int lgao_get_audio_info(void *a, LGAO_AUDIO_INFO_T prop, unsigned *p) { *p = 0; return 0; }
int lgao_set_audio_info(void *a, LGAO_AUDIO_START_INFO_T i) { saved_info = i; if (real_comparison) live_ao.audioStartInfo = i; ++info_sets; record('A'); return -1; }
int lgao_compare_audio_info(void *a, LGAO_AUDIO_START_INFO_T *i) { record('C'); return real_comparison ? recovered_compare_audio_info(a, i) : compare_result; }
#define ACTION(name, ch) int name(void *a) { record(ch); return -1; }
ACTION(lgao_stop_renderer,'s')
ACTION(lgao_disconnect_renderer,'d')
ACTION(lgao_close_renderer,'c')
ACTION(lgao_open_renderer,'o')
ACTION(lgao_config,'f')
ACTION(lgao_connect_renderer,'j')
ACTION(lgao_play,'p')
ACTION(lgao_start_renderer,'b')
ACTION(lgao_keep_fade,'k')
/* Cancellation sentinel, not an emulation of the ring implementation. */
int lgao_flush(void *a) { ring_pending=0; record('F'); return -1; }
int lgao_push_first_event(void *a) { ++first_pushes; record('1'); return -1; }
int lgao_set_playrate(void *a, int rate) { speed = rate; record('R'); return -1; }
int lgao_get_size_buffered(void *a, int t, int *p) { *p = buffered; record('B'); return 0; }
int lgao_get_first_render(void *a, int t, int *p) { *p = first; return 0; }
int lgao_eos(void *a) { record('E'); return eos_result; }
int lgao_audio_gap(void *a, unsigned long long b, unsigned long long e) { record('G'); return 0; }
int lgao_feed(void *a, void *data, unsigned len, unsigned long long pts, LGAO_DATA_TYPE_T t) { assert(feeds < 16); assert(data == payload + offsets[copies-1]); sizes[feeds] = len; timestamps[feeds] = pts; types[feeds++] = t; record('H'); return feed_result; }

// @match src/libgstlgaudiosink/render_element.c (?s)#define LG_BUFFER_FLAG_METADATA .*?#define CHAIN_MAX_FEED_SIZE[^\n]*
// @extract src/libgstlgaudiosink/render_element.c gst_lg_audio_render_set_caps gst_lg_audio_render_event gst_lg_audio_render_chain

static GstLgAudioRender reset(void) {
    real_comparison = 0;
    memset(&live_ao, 0, sizeof(live_ao));
    warnings = mute = dropped = compare_result = feed_result = 0;
    feeds = copies = maps = unmaps = unrefs = copy_fail = map_fail = ui = dump_opens = 0;
    buffered = waits = parents = clock_updates = info_sets = caps_frees = first_pushes = generation = 0;
    first = parent_result = 1; eos_result = ring_pending = 0; speed = 1000; port = 3;
    ncalls = 0; calls[0] = 0; memset(&saved_info,0,sizeof(saved_info));
    return (GstLgAudioRender){.ao = (void *)1, .currentState = GST_STATE_PLAYING,
                             .currentPTS = 99, .segment = {.rate = 1}};
}
static GstStructure pcm(void) { GstStructure s = {.name = "audio/x-raw"}; string(&s,"format","S16LE"); number(&s,"rate",48000); number(&s,"channels",2); return s; }
static void clear_caps(GstLgAudioRender *r) { if (r->pAudioCaps) { gst_caps_unref(r->pAudioCaps); r->pAudioCaps = NULL; } }
static void chain_tests(void) {
    GstBuffer b = {.pts = 1234, .size = 0x20001};
    GstLgAudioRender r = reset();
    assert(gst_lg_audio_render_chain(&r,&b) == GST_FLOW_OK);
    assert(feeds == 3 && sizes[0] == 65536 && sizes[1] == 65536 && sizes[2] == 1);
    assert(offsets[0] == 0 && offsets[1] == 65536 && offsets[2] == 131072);
    assert(timestamps[0] == 1234 && timestamps[1] == 1234 && timestamps[2] == 1234);
    assert(r.currentPTS == 1234 && r.decodedSize == 0 && unrefs == 3 && unmaps == 3);
    int rates[] = {-1,0,249,250,2000,2001};
    for (unsigned i=0;i<sizeof(rates)/sizeof(*rates);++i) {
        r=reset(); speed=rates[i]; b.size=1; b.flags=0;
        assert(gst_lg_audio_render_chain(&r,&b)==0);
        assert(feeds == (rates[i]>=250 && rates[i]<=2000));
    }
    r=reset(); mute=1; r.bClockCalibrated=1; r.segment.start=0; r.segment_position=7; r.segment_start=88; r.base_time=100; r.segment.base=200;
    b.flags=LG_BUFFER_FLAG_OBJECT_UI|LG_BUFFER_FLAG_DROP_RENDERER;
    assert(gst_lg_audio_render_chain(&r,&b)==0);
    assert(!feeds && !ui && !generation && clock_updates==1 && clock_start==88 && clock_base==9700 && !r.bClockCalibrated);
    r=reset(); mute=2; b.flags=0; gst_lg_audio_render_chain(&r,&b); assert(feeds==1);
    r=reset(); b.flags=GST_BUFFER_FLAG_CORRUPTED|LG_BUFFER_FLAG_OBJECT_UI|LG_BUFFER_FLAG_DROP_RENDERER;
    gst_lg_audio_render_chain(&r,&b); assert(!feeds && !ui && !generation && r.currentPTS==99);
    r=reset(); r.currentState=GST_STATE_READY; r.enable_dump=1; dropped=1; b.flags=LG_BUFFER_FLAG_OBJECT_UI;
    gst_lg_audio_render_chain(&r,&b); assert(!feeds && ui==1 && dump_opens==1 && generation==1 && r.currentPTS==99); assert(!strcmp(calls,"rnmxUV"));
    r=reset(); r.currentState=GST_STATE_PAUSED; b.flags=0; gst_lg_audio_render_chain(&r,&b); assert(feeds==1);
    r=reset(); r.currentState=GST_STATE_READY; r.decodedSize=1; gst_lg_audio_render_chain(&r,&b); assert(feeds==1);
    r=reset(); b.flags=LG_BUFFER_FLAG_DROP_RENDERER; b.size=25;
    gst_lg_audio_render_chain(&r,&b); assert(feeds==1 && dropped && generation==1 && !r.decodedSize);
    r=reset(); r.isUnsupportedStream=1; b.flags=LG_BUFFER_FLAG_DROP_RENDERER; b.size=25; gst_lg_audio_render_chain(&r,&b); assert(!feeds && generation==1 && r.decodedSize==25 && r.currentPTS==1234);
    guint flags[]={0,LG_BUFFER_FLAG_METADATA,LG_BUFFER_FLAG_METADATA2,LG_BUFFER_FLAG_ES|LG_BUFFER_FLAG_METADATA|LG_BUFFER_FLAG_METADATA2};
    int expected[]={LGAO_DATA_PCM,LGAO_DATA_METADATA,LGAO_DATA_METADATA2,LGAO_DATA_ES};
    for(int i=0;i<4;++i) { r=reset(); b.flags=flags[i]; b.pts=GST_CLOCK_TIME_NONE; gst_lg_audio_render_chain(&r,&b); assert((int)types[0]==expected[i] && timestamps[0]==GST_CLOCK_TIME_NONE && r.currentPTS==99); }
    r=reset(); b.flags=0; b.size=65537; copy_fail=1; gst_lg_audio_render_chain(&r,&b); assert(copies==2 && feeds==1 && offsets[0]==offsets[1] && sizes[0]==65536);
    r=reset(); b.size=65537; map_fail=1; gst_lg_audio_render_chain(&r,&b); assert(feeds==1 && copies==2 && offsets[1]==65536 && unrefs==2 && unmaps==1);
    r=reset(); b.size=65537; feed_result=-1; assert(gst_lg_audio_render_chain(&r,&b)==0 && feeds==2);
    r=reset(); b.size=0; gst_lg_audio_render_chain(&r,&b); assert(!feeds && r.currentPTS==99); /* invalid PTS leaves prior value */
}
static void caps_tests(void) {
    GstLgAudioRender r; GstStructure s; GstCaps c; gboolean changed;
    for(int result=-1;result<=3;++result) {
        r=reset(); s=pcm(); c=(GstCaps){&s}; changed=0; compare_result=result;
        assert(gst_lg_audio_render_set_caps(&r,&c,&changed));
        assert(changed==(result!=0 && result!=2));
        assert(info_sets == (changed ? 2 : 1));
        assert((strstr(calls,"sdc")!=NULL)==(result==1));
        assert((strstr(calls,"oAfjpb")!=NULL)==!!changed);
        assert(saved_info.freq==48000 && saved_info.bitsPerSample==16 && saved_info.numberOfChannel==2);
        clear_caps(&r);
    }
    r=reset(); s=pcm(); c=(GstCaps){&s}; changed=0; compare_result=-1; port=-1;
    assert(gst_lg_audio_render_set_caps(&r,&c,&changed) && !changed && info_sets==1 && !strchr(calls,'o')); clear_caps(&r);
    r=reset(); s=pcm(); c=(GstCaps){&s}; changed=0; compare_result=1; dropped=1;
    assert(gst_lg_audio_render_set_caps(&r,&c,&changed) && changed && !strchr(calls,'o') && info_sets==1); clear_caps(&r);
    r=reset(); s=pcm(); number(&s,"BYPASS",1); c=(GstCaps){&s}; changed=0;
    assert(gst_lg_audio_render_set_caps(&r,&c,&changed) && !changed && dropped && generation==1 && !info_sets); clear_caps(&r);
    r=reset(); s=pcm(); number(&s,"DUMMY",0); c=(GstCaps){&s}; changed=0;
    assert(gst_lg_audio_render_set_caps(&r,&c,&changed) && !info_sets); clear_caps(&r);
    r=reset(); s=(GstStructure){.name="audio/x-raw"}; c=(GstCaps){&s}; changed=0;
    assert(gst_lg_audio_render_set_caps(&r,&c,&changed) && !info_sets && r.pAudioCaps); clear_caps(&r);
    for(int i=0;i<2;++i) { r=reset(); s=pcm(); s.fields[0].str=i?"F32LE":"unknown"; c=(GstCaps){&s}; changed=0;
        assert(gst_lg_audio_render_set_caps(&r,&c,&changed) && info_sets==1 && saved_info.bitsPerSample==0); clear_caps(&r); }
    r=reset(); s=pcm(); s.fields[1].value=384001; c=(GstCaps){&s}; changed=0;
    assert(gst_lg_audio_render_set_caps(&r,&c,&changed) && warnings==1 && r.isUnsupportedStream && !info_sets); clear_caps(&r);
    r=reset(); s=pcm(); number(&s,"channel-mask",UINT64_C(0x100000002)); number(&s,"atmos",0); c=(GstCaps){&s}; changed=0;
    assert(gst_lg_audio_render_set_caps(&r,&c,&changed) && saved_info.acmod==1 && first_pushes==1); clear_caps(&r);
    r=reset(); s=pcm(); c=(GstCaps){&s}; changed=0;
    const char *optional_flags[]={"lfe-enable","dolby","clock-x4","atmos","restart","downmix"};
    for(unsigned index=0;index<sizeof(optional_flags)/sizeof(*optional_flags);++index)
        number(&s,optional_flags[index],1);
    assert(gst_lg_audio_render_set_caps(&r,&c,&changed));
    assert(saved_info.lfe_enable && saved_info.dolby && saved_info.clock_x4 &&
           saved_info.atmos && saved_info.restart && saved_info.downmix);
    s=pcm();
    assert(gst_lg_audio_render_set_caps(&r,&c,&changed));
    assert(!saved_info.lfe_enable && !saved_info.dolby && !saved_info.clock_x4 &&
           !saved_info.atmos && !saved_info.restart && !saved_info.downmix);
    clear_caps(&r);
    r=reset(); s=(GstStructure){.name="video/x-raw"}; c=(GstCaps){&s}; changed=0;
    assert(!gst_lg_audio_render_set_caps(&r,&c,&changed) && r.pAudioCaps); clear_caps(&r);
}
static void consecutive_caps_tests(void) {
    GstLgAudioRender r = reset();
    real_comparison = 1;
    r.ao = &live_ao;
    live_ao.fd = -1;
    GstStructure es = pcm();
    number(&es, "es", 1);
    number(&es, "codec", LGAO_CODEC_MAT);
    number(&es, "data-rate", 192000);
    string(&es, "codec-str", "mat\0padding-byte");
    GstCaps c = {&es};
    gboolean changed = 0;
    assert(gst_lg_audio_render_set_caps(&r, &c, &changed) && changed);
    assert(strstr(calls, "oAfjpb") && saved_info.es == 1);
    live_ao.fd = 7;
    GstStructure decoded = pcm();
    c.s = &decoded;
    changed = 0;
    ncalls = 0; calls[0] = 0;
    assert(gst_lg_audio_render_set_caps(&r, &c, &changed) && !changed);
    assert(!strpbrk(calls, "sdcofjpb") && saved_info.es == 0);
    assert(saved_info.codecType == LGAO_CODEC_PCM && saved_info.data_rate == 0);
    number(&decoded, "restart", 1);
    ncalls = 0; calls[0] = 0;
    assert(gst_lg_audio_render_set_caps(&r, &c, &changed) && !changed);
    assert(!strpbrk(calls, "sdcofjpb"));
    decoded.fields[1].value = 96000;
    ncalls = 0; calls[0] = 0;
    assert(gst_lg_audio_render_set_caps(&r, &c, &changed) && changed);
    assert(strstr(calls, "sdcoAfjpb"));
    assert(saved_info.freq == 96000 && !saved_info.es);
    clear_caps(&r);

    r = reset();
    real_comparison = 1;
    r.ao = &live_ao;
    live_ao.fd = 7;
    live_ao.audioStartInfo = (LGAO_AUDIO_START_INFO_T){
        .codecType = LGAO_CODEC_MAT, .freq = 192000,
        .numberOfChannel = 8, .bitsPerSample = 16, .es = 1
    };
    decoded = pcm(); c.s = &decoded; changed = 0;
    assert(gst_lg_audio_render_set_caps(&r, &c, &changed) && changed);
    assert(strstr(calls, "sdcoAfjpb") && saved_info.freq == 48000);
    assert(saved_info.numberOfChannel == 2 && saved_info.bitsPerSample == 16);
    clear_caps(&r);
}
static void event_tests(void) {
    GstLgAudioRender r=reset(); GstEvent e={.type=GST_EVENT_FLUSH_START}; r.bSeamlessSeek=1;
    ring_pending=1;
    assert(gst_lg_audio_render_event(&r,&e) && r.isFlushing && !r.bSeamlessSeek && !strcmp(calls,"P"));
    assert(ring_pending); /* Parent forwarding did not invoke renderer cancellation. */
    GstBuffer b={.size=1,.pts=10};
    gst_lg_audio_render_chain(&r,&b);
    assert(feeds==1 && ring_pending); /* isFlushing is not a chain gate. */
    r=reset(); r.isFlushing=1; r.decodedSize=123; ring_pending=1; e.type=GST_EVENT_FLUSH_STOP;
    assert(gst_lg_audio_render_event(&r,&e) && !r.isFlushing && !r.decodedSize && speed==0 && !strcmp(calls,"RFP") && !ring_pending);
    r=reset(); e.type=GST_EVENT_EOS; assert(gst_lg_audio_render_event(&r,&e) && !waits && !strchr(calls,'E'));
    for(int i=-1;i<=1;++i) { r=reset(); buffered=1; eos_result=i; e.type=GST_EVENT_EOS; assert(gst_lg_audio_render_event(&r,&e)); assert(waits==(i<=0) && parents==1); }
    r=reset(); first=0; e.type=GST_EVENT_EOS; gst_lg_audio_render_event(&r,&e); assert(waits==1);
    r=reset(); e.type=GST_EVENT_STREAM_START; e.id="new"; r.pStreamId=g_strdup("old"); r.currentState=GST_STATE_READY; dropped=1;
    assert(gst_lg_audio_render_event(&r,&e) && !strcmp(calls,"ksdcofjpbP") && !info_sets && !strcmp(r.pStreamId,"new")); free(r.pStreamId);
    r=reset(); e.id="same"; r.pStreamId=g_strdup("same"); gst_lg_audio_render_event(&r,&e); assert(!strcmp(calls,"P")); free(r.pStreamId);
    r=reset(); e.id="new"; r.isSeamless=1; r.pStreamId=g_strdup("old"); gst_lg_audio_render_event(&r,&e); assert(!strcmp(calls,"P")); free(r.pStreamId);
    r=reset(); e.id=NULL; gst_lg_audio_render_event(&r,&e); assert(!r.pStreamId);
    r=reset(); GstStructure s=pcm(); GstCaps c={&s}; e=(GstEvent){.type=GST_EVENT_CAPS,.caps=&c}; compare_result=1;
    assert(gst_lg_audio_render_event(&r,&e) && clock_updates==1 && calls[ncalls-2]=='T'); clear_caps(&r);
    r=reset(); s.name="video/x-raw"; e.unrefs=0;
    assert(!gst_lg_audio_render_event(&r,&e) && e.unrefs==1 && !parents); clear_caps(&r);
    r=reset(); s=(GstStructure){.name="mpegh-object-ui"}; e=(GstEvent){.type=GST_EVENT_CUSTOM_DOWNSTREAM_OOB,.s=&s};
    assert(gst_lg_audio_render_event(&r,&e) && e.unrefs==1 && !parents);
    r=reset(); e.type=GST_EVENT_GAP; parent_result=0; assert(!gst_lg_audio_render_event(&r,&e) && parents==1);
}
int main(void) { chain_tests(); caps_tests(); consecutive_caps_tests(); event_tests(); puts("sink render: source-extracted chain/caps/event lifecycle tests passed"); return 0; }
