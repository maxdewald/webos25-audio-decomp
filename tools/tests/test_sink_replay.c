/* Deferred-property replay switch with mocked lgao setters; no GStreamer. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef unsigned guint; typedef int gint; typedef const char gchar; typedef int LGAO_APPTYPE_T;
#define FALSE 0
#define GST_BASE_SINK(p) (p)
#define gst_base_sink_set_sync(p,v) ((void)(p),(void)(v))
#define SINK_LOG(...) ((void)0)
#define PREOPEN_BIT(p) (UINT64_C(1)<<((p)-1))
#define strcmp(a,b) replay_compare(a,b)
static int replay_compare(const char *a,const char *b) { assert(a && b); return __builtin_strcmp(a,b); }
// @match src/libgstlgaudiosink/render_element.h (?s)enum \{\s*PROP_0,.*?\n\};
#define LGAO_APPTYPE_AIRPLAY 1
#define LGAO_APPTYPE_APPCAST 2
#define LGAO_APPTYPE_DIRECTMEDIA 3
#define LGAO_APPTYPE_IDGAPLESS 4
#define LGAO_APPTYPE_JP4K 5
#define LGAO_APPTYPE_MIRACAST 6
#define LGAO_APPTYPE_R2R 7
#define LGAO_APPTYPE_RTC 8
#define LGAO_APPTYPE_SYE 9
#define LGAO_APPTYPE_UNKNOWN 10
#define LGAO_CLOCK_TYPE_ATSC3 11
#define LGAO_CLOCK_TYPE_GSTC 12
#define LGAO_RTS_BUFFERING_LEVEL 13
#define LGAO_RTS_FREQ_FAST 14
#define LGAO_RTS_OP_MODE 15
#define LGAO_RTS_THRESHOLD_FAST 16
#define LGAO_RTS_THRESHOLD_RECOVER 17
#define LGAO_RTS_THRESHOLD_SKIPPING 18
typedef struct { void *ao; uint64_t preopen_conditions;
int audio_port;
const char *clocktype;
int game_mode;
int lipsync_mode;
int lipsync_offset;
int mixer_port;
int mode_mixer;
int mode_mute;
int nodelay_fast_freq;
int nodelay_fast_up;
int nodelay_mode;
int nodelay_recover;
int nodelay_skip_up;
const char *pAppType;
int pcm_bypass;
int pcm_multi_ch;
int play_speed;
int queue_max_length;
int render_buf_thres;
int rts_mode;
int vsp_mode;
} GstLgAudioRender;
static const char *trace[64]; static unsigned count, pcm_calls;
static void *last_ao; static int last_pcm;
static int record(const char *name) { assert(count<64); trace[count++]=name; return -1; }
static int record_pcm(const char *name,void *ao,int value) { ++pcm_calls;last_ao=ao;last_pcm=value;return record(name); }
#define lgao_set_apptype(...) record("lgao_set_apptype")
#define lgao_set_clocktype(...) record("lgao_set_clocktype")
#define lgao_set_game_mode(...) record("lgao_set_game_mode")
#define lgao_set_lipsync_mode(...) record("lgao_set_lipsync_mode")
#define lgao_set_lipsync_offset(...) record("lgao_set_lipsync_offset")
#define lgao_set_mode_mixer(...) record("lgao_set_mode_mixer")
#define lgao_set_mute(...) record("lgao_set_mute")
#define lgao_set_nodelay(...) record("lgao_set_nodelay")
#define lgao_set_pcm_bypass(ao,value) record_pcm("lgao_set_pcm_bypass",(ao),(value))
#define lgao_set_pcm_multi_channel(ao,value) record_pcm("lgao_set_pcm_multi_channel",(ao),(value))
#define lgao_set_playrate(...) record("lgao_set_playrate")
#define lgao_set_port(...) record("lgao_set_port")
#define lgao_set_queue_max_length(...) record("lgao_set_queue_max_length")
#define lgao_set_rts(...) record("lgao_set_rts")
#define lgao_set_vsp_mode(...) record("lgao_set_vsp_mode")
// @extract src/libgstlgaudiosink/render_element.c _gst_lg_audio_render_set_preconditions
#define replay _gst_lg_audio_render_set_preconditions
int main(void) {
    int token;
    GstLgAudioRender s={.ao=&token,.pcm_bypass=1,.pcm_multi_ch=1};
    s.preopen_conditions=PREOPEN_BIT(PROP_PCM_BYPASS)|PREOPEN_BIT(PROP_PCM_MULTI_CH)|PREOPEN_BIT(PROP_QUEUE_MAX_LENGTH);
    uint64_t original=s.preopen_conditions;
    replay(&s);
    assert(count==3 && !strcmp(trace[0],"lgao_set_pcm_bypass"));
    assert(!strcmp(trace[1],"lgao_set_pcm_multi_channel") && !strcmp(trace[2],"lgao_set_queue_max_length"));
    assert(s.preopen_conditions==original && pcm_calls==2 && last_ao==&token && last_pcm==1);
    replay(&s);assert(count==6 && pcm_calls==4 && s.preopen_conditions==original);
    s.preopen_conditions=0;count=0;replay(&s);assert(!count);
    s.preopen_conditions=PREOPEN_BIT(PROP_PCM_MULTI_CH);s.ao=NULL;replay(&s);
    assert(count==1 && !strcmp(trace[0],"lgao_set_pcm_multi_channel") && last_ao==NULL && last_pcm==1);
    puts("PASS sink replay: property-ID order, cached PCM flags, failed replay retains mask, no ao readiness gate");
    return 0;
}
