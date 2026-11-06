#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef int GstState,GstStateChange,GstStateChangeReturn,LGAO_STATE_T,gint;
typedef unsigned guint;typedef uint64_t GstClockTime,guint64;typedef int64_t gint64;
enum { GST_STATE_NULL,GST_STATE_READY,GST_STATE_PAUSED,GST_STATE_PLAYING };
#define TRANSITION(a,b) ((a)*8+(b))
#define GST_STATE_TRANSITION_CURRENT(t) ((t)/8)
#define GST_STATE_TRANSITION_NEXT(t) ((t)%8)
#define GST_STATE_CHANGE_NULL_TO_READY TRANSITION(GST_STATE_NULL,GST_STATE_READY)
#define GST_STATE_CHANGE_READY_TO_PAUSED TRANSITION(GST_STATE_READY,GST_STATE_PAUSED)
#define GST_STATE_CHANGE_PAUSED_TO_PLAYING TRANSITION(GST_STATE_PAUSED,GST_STATE_PLAYING)
#define GST_STATE_CHANGE_PLAYING_TO_PLAYING TRANSITION(GST_STATE_PLAYING,GST_STATE_PLAYING)
#define GST_STATE_CHANGE_PAUSED_TO_READY TRANSITION(GST_STATE_PAUSED,GST_STATE_READY)
#define GST_STATE_CHANGE_PLAYING_TO_PAUSED TRANSITION(GST_STATE_PLAYING,GST_STATE_PAUSED)
#define GST_STATE_CHANGE_READY_TO_NULL TRANSITION(GST_STATE_READY,GST_STATE_NULL)
#define LGAO_STATE_PLAY 3
#define TRUE 1
#define GST_CLOCK_TIME_NONE UINT64_MAX
#define SINK_LOG(...) ((void)0)
#define SINK_CHECK(call,line) ((void)(call))
struct parent { int (*change_state)(void *,int); };
struct segment { double rate;uint64_t base;int64_t start; };
typedef struct sink {
    void *ao;GstState currentState,nextState;uint64_t preopen_conditions;
    struct segment segment;void *clock;uint64_t base_time;
    int bSeamlessSeek,instant_rate_change,video_port,avail_irc_vdec_pts;
    uint64_t irc_pts;int64_t segment_start;unsigned eos_cond,eos_mutex,task_mutex;
} GstElement,GstBaseSink,GstLgAudioRender,GObject;
#define GST_LG_AUDIO_RENDER(p) ((GstLgAudioRender *)(p))
#define GST_BASE_SINK(p) ((GstBaseSink *)(p))
#define GST_ELEMENT_CLASS(p) ((struct parent *)(p))
static struct parent parent_storage;static void *parent_class=&parent_storage;
static char order[128];static unsigned norder;
static int ao_token,open_fails,parent_result,parent_destroys,renderer_state,pause_calls;
static int mode,port,deferred,rate_value,start_calls,play_calls,flush_calls,deinit_calls;
static uint64_t now=100000;static unsigned pts_value=90009;
static int cleared[3],clear_count,clock_calls,seamless_calls;
static int64_t clock_base,clock_start;
static void record(char c) { assert(norder<sizeof(order)-1);order[norder++]=c;order[norder]=0; }
static int lgao_get_mode_mixer(void *a,int *v) { assert(a);*v=mode;return 0; }
static int lgao_get_port(void *a,int *v) { assert(a);*v=port;return 0; }
static int lgao_get_state(void *a,int *v) { assert(a);*v=renderer_state;return 0; }
static void *lgao_open(void) { record('O');return open_fails?NULL:&ao_token; }
static void _gst_lg_audio_render_set_preconditions(GstLgAudioRender *s)
{ ++deferred;record('D');assert(s->ao==(open_fails?NULL:&ao_token)); }
static int lgao_set_playrate(void *a,int rate) { (void)a;rate_value=rate;record('R');return -1; }
static uint64_t gst_clock_get_time(void *clock) { (void)clock;return now; }
static void gst_lg_audio_render_prepare_seamless_seek(GstBaseSink *s,struct segment *segment,uint64_t t)
{ assert(segment==&s->segment && t==GST_CLOCK_TIME_NONE);++seamless_calls;record('S'); }
static void gst_lg_audio_render_update_clock(GstLgAudioRender *s,int64_t base,int64_t start)
{ (void)s;clock_base=base;clock_start=start;++clock_calls;record('C'); }
static int lgao_get_vdec_current_pts(void *a,int port_index,unsigned *v)
{ (void)a;(void)port_index;*v=pts_value;record('V');return 0; }
static int lgao_play(void *a) { (void)a;++play_calls;record('P');return -1; }
static int lgao_start_renderer(void *a) { (void)a;++start_calls;record('T');return -1; }
static int lgao_flush(void *a) { (void)a;++flush_calls;record('F');return -1; }
static int lgao_pause(void *a) { (void)a;++pause_calls;record('Q');return -1; }
static void gst_lxlog_deinit(void) { ++deinit_calls;record('X'); }
static void g_cond_clear(unsigned *p) { (void)p;cleared[clear_count++]=1; }
static void g_mutex_clear(unsigned *p) { (void)p;cleared[clear_count++]=2; }
static void g_rec_mutex_clear(unsigned *p) { (void)p;cleared[clear_count++]=3; }
static int parent_change(void *ptr,int transition)
{
    GstLgAudioRender *s=ptr;record('B');
    if(transition==GST_STATE_CHANGE_NULL_TO_READY) assert(s->ao==(open_fails?NULL:&ao_token));
    if(parent_destroys) s->ao=NULL;
    return parent_result;
}
/* RECOVERED_FUNCTIONS */
static void reset(GstLgAudioRender *s)
{
    *s=(GstLgAudioRender){0};s->segment.rate=1.0;s->segment.base=200;s->segment.start=700;s->base_time=100;
    norder=0;order[0]=0;open_fails=parent_destroys=0;parent_result=1;renderer_state=LGAO_STATE_PLAY;
    pause_calls=deferred=rate_value=start_calls=play_calls=flush_calls=deinit_calls=0;
    clear_count=clock_calls=seamless_calls=0;parent_storage.change_state=parent_change;
}
int main(void)
{
    GstLgAudioRender s;
    reset(&s);s.preopen_conditions=1;
    assert(gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_NULL_TO_READY)==1);
    assert(!strcmp(order,"ODB") && s.ao==&ao_token && deferred==1 && s.currentState==GST_STATE_READY);
    reset(&s);open_fails=1;parent_result=-1;s.preopen_conditions=1;
    assert(gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_NULL_TO_READY)==-1);
    assert(!s.ao && !strcmp(order,"ODB"));
    reset(&s);s.ao=&ao_token;parent_result=-1;
    assert(gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_PAUSED_TO_PLAYING)==-1);
    assert(!strcmp(order,"RCPTB") && s.currentState==GST_STATE_PLAYING && rate_value==1000);
    assert(clock_base==99700 && clock_start==700 && play_calls==1 && start_calls==1);
    reset(&s);s.ao=&ao_token;s.segment.rate=0;
    gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_PAUSED_TO_PLAYING);assert(rate_value==1000);
    reset(&s);s.ao=&ao_token;s.instant_rate_change=1;s.segment_start=33;
    gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_PAUSED_TO_PLAYING);
    assert(!strcmp(order,"RVCPTB") && s.irc_pts==(uint64_t)(pts_value/9)*100000);
    assert(clock_base==0 && clock_start==33 && s.avail_irc_vdec_pts);
    reset(&s);s.ao=&ao_token;s.bSeamlessSeek=1;
    gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_PAUSED_TO_PLAYING);
    assert(!strcmp(order,"RSPTB") && seamless_calls==1 && !clock_calls);
    reset(&s);s.ao=&ao_token;
    gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_PLAYING_TO_PAUSED);
    assert(!strcmp(order,"BQ") && pause_calls==1 && s.currentState==GST_STATE_PAUSED);
    reset(&s);s.ao=&ao_token;renderer_state=0;
    gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_PLAYING_TO_PAUSED);assert(!pause_calls);
    reset(&s);s.ao=&ao_token;
    gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_PAUSED_TO_READY);assert(!strcmp(order,"BF"));
    reset(&s);s.ao=&ao_token;parent_destroys=1;
    gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_READY_TO_NULL);
    assert(!s.ao && !strcmp(order,"BX") && s.currentState==GST_STATE_NULL);
    reset(&s);s.ao=&ao_token;s.currentState=GST_STATE_PLAYING;
    gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_PLAYING_TO_PLAYING);assert(!strcmp(order,"B"));
    reset(&s);s.ao=&ao_token;s.currentState=GST_STATE_PAUSED;
    gst_lg_audio_render_change_state(&s,GST_STATE_CHANGE_PLAYING_TO_PLAYING);assert(!strcmp(order,"CPTB"));
    reset(&s);s.ao=&ao_token;gst_lg_audio_render_finalize(&s);
    assert(s.ao==&ao_token && clear_count==3 && cleared[0]==1 && cleared[1]==2 && cleared[2]==3);
    puts("PASS sink state: per-instance creation, parent/renderer ordering, failures, clock branches and finalize non-teardown");
    return 0;
}
