/* Framework doubles exercise recovered control flow, not external PCM SIMD kernels.
 */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../../src/dsp1/fader_queue.c"

struct object { void *p; int refs; bool data; };
static struct object objects[256];
static int object_count;
struct test_event { struct aud_event event; int size, count, channels, bits, es, delay, front, dolby, passing; };
static struct aud_data *fifo[128];
static int fifo_count, signals, waits, timeout_result, raised, creates, destroys;
static unsigned char mapped[4096];
static struct { int kind, port, type; unsigned size; unsigned char bytes[256]; } output[128];
static int output_count;
static struct fader *active;
static void (*wait_action)(struct fader *);
static const struct fader_desc normal_desc = { 2, 0, 4, 2, 4, 0 };
static const struct fader_desc queue_desc = { 2, 0, 4, 2, 4, 1 };
static const struct aud_module_class normal_class = { "fader", &normal_desc };
static const struct aud_module_class queue_class = { "queue", &queue_desc };
volatile uint32_t fader_log_mask;
const void *const fader_log = NULL;
const struct aud_module_class fader_class = { "fader", &normal_desc };
const struct fader_desc fader_desc_fader = {0}, fader_desc_queue = {0}, fader_desc_input_queue = {0};
const void *const fader_debug_opts = NULL;

static void *track(void *p, bool data)
{
    assert(object_count < 256 && p);
    objects[object_count++] = (struct object){p, 1, data};
    return p;
}
static struct object *object(void *p)
{
    for (int i = 0; i < object_count; ++i)
        if (objects[i].p == p) return &objects[i];
    assert(!"unknown object");
    return NULL;
}
void *aud_object_get(void *p) { assert(p); ++object(p)->refs; return p; }
void aud_object_put(void *p) { assert(p && object(p)->refs > 0); --object(p)->refs; }
static struct aud_event *event(int type)
{
    struct test_event *e = track(calloc(1, sizeof *e), false);
    e->event.type = type; e->count = 1; e->channels = 2; e->bits = 16; e->es = 1;
    return &e->event;
}
struct aud_event *aud_event_dup(struct aud_event *e)
{
    struct test_event *n = track(malloc(sizeof *n), false);
    *n = *(struct test_event *)e; return &n->event;
}
struct aud_event *aud_event_new_last(void) { return event(AUD_EVENT_LAST_AU); }
struct aud_event *aud_event_newv_custom(const char *n, int a, int b, int c)
{ (void)n; (void)a; (void)b; (void)c; return event(AUD_EVENT_CUSTOM); }
const char *aud_event_name(struct aud_event *e) { (void)e; return "event"; }
int aud_event_value_get_int(struct aud_event *e, const char *k, int def)
{
    struct test_event *t = (struct test_event *)e;
    if (!strcmp(k,"queue-size")) return t->size;
    if (!strcmp(k,"queue-count")) return t->count;
    if (!strcmp(k,"channels")) return t->channels;
    if (!strcmp(k,"bits-per-sample")) return t->bits;
    if (!strcmp(k,"low-delay")) return t->delay;
    if (!strcmp(k,"front-padding")) return t->front;
    if (!strcmp(k,"passing-length")) return t->passing;
    return def;
}
bool aud_event_value_get_bool(struct aud_event *e, const char *k, bool def)
{
    if (!strcmp(k,"es")) return ((struct test_event *)e)->es;
    if (!strcmp(k,"dolby")) return ((struct test_event *)e)->dolby;
    return def;
}
void aud_event_value_set_int(struct aud_event *e, const char *k, int v)
{ assert(!strcmp(k,"passing-length")); ((struct test_event *)e)->passing=v; }
void aud_event_value_del(struct aud_event *e, const char *k)
{ if (!strcmp(k,"queue-size")) ((struct test_event *)e)->size=0; else ((struct test_event *)e)->count=0; }
struct aud_data *aud_data_new(unsigned size)
{
    struct aud_data *d=track(calloc(1,sizeof *d),true);
    d->size=size; d->ptr=malloc(size ? size : 1); memset(d->ptr,0xa5,size); return d;
}
struct aud_data *aud_data_new_zero(unsigned size)
{ struct aud_data *d=aud_data_new(size); memset(d->ptr,0,size); return d; }
struct aud_data *aud_data_new_metadata(void *p) { (void)p; return NULL; }
void aud_data_adapter_push(struct aud_data_adapter *q,struct aud_data *d)
{ assert(fifo_count<128); fifo[fifo_count++]=d; q->total+=d->size; }
void aud_data_adapter_push_front(struct aud_data_adapter *q,struct aud_data *d)
{ memmove(fifo+1,fifo,(size_t)fifo_count*sizeof *fifo); fifo[0]=d; ++fifo_count; q->total+=d->size; }
struct aud_data *aud_data_adapter_dequeue(struct aud_data_adapter *q)
{
    if (!fifo_count) return NULL;
    struct aud_data *d=fifo[0]; --fifo_count;
    memmove(fifo,fifo+1,(size_t)fifo_count*sizeof *fifo); q->offset+=d->size; return d;
}
void aud_data_adapter_flush(struct aud_data_adapter *q,unsigned n)
{
    while(n) {
        assert(fifo_count && n>=fifo[0]->size);
        unsigned size=fifo[0]->size; aud_object_put(aud_data_adapter_dequeue(q)); n-=size;
    }
}
void *aud_data_adapter_map(struct aud_data_adapter *q,unsigned n)
{
    unsigned off=0; (void)q;
    for(int i=0;i<fifo_count && off<n;++i) { unsigned k=fifo[i]->size; if(k>n-off) k=n-off; memcpy(mapped+off,fifo[i]->ptr,k); off+=k; }
    assert(off==n); return mapped;
}
void aud_data_adapter_unmap(struct aud_data_adapter *q,void *p) { (void)q; assert(p==mapped); }
static int push(int kind,int port,int type,struct aud_data *d)
{
    assert(output_count<128); int i=output_count++;
    output[i].kind=kind; output[i].port=port; output[i].type=type;
    if(d) { assert(d->size<=256); output[i].size=d->size; memcpy(output[i].bytes,d->ptr,d->size); }
    return 0;
}
int aud_module_push_data(struct fader *f,int p,struct aud_data *d)
{ (void)f; push(1,p,d->type,d); aud_object_put(d); return 0; }
int aud_module_push_event(struct fader *f,int p,struct aud_event *e)
{ (void)f; push(2,p,e->type,NULL); aud_object_put(e); return 0; }
void aud_module_sync(struct fader *f,int p) { (void)f; (void)p; }
int aud_module_default_event(struct fader *f,void *p,struct aud_event *e)
{ (void)p; return aud_module_push_event(f,0,e); }
int aud_module_output_thread_create(struct fader *f,void *fn,void *arg,struct aud_mutex *m,struct aud_cond *c)
{ (void)fn; (void)arg; (void)m; (void)c; f->output_thread=f; ++creates; return 0; }
void aud_module_output_thread_destroy(struct fader *f) { f->output_thread=NULL; ++destroys; }
void aud_mutex_lock(struct aud_mutex *m) { ++m->opaque; }
void aud_mutex_unlock(struct aud_mutex *m) { --m->opaque; }
void aud_cond_signal(struct aud_cond *c) { (void)c; ++signals; }
void aud_cond_wait(struct aud_cond *c,struct aud_mutex *m)
{ (void)c; (void)m; ++waits; assert(wait_action); wait_action(active); }
int aud_cond_wait_timeout(struct aud_cond *c,struct aud_mutex *m,unsigned us)
{ (void)c; (void)m; ++waits; if (us==10) return 0; if(wait_action) wait_action(active); return timeout_result; }
void *aud_thread_create(const char *n,unsigned stack,int priority,void *fn,void *arg)
{ (void)n; (void)stack; (void)priority; (void)fn; return arg; }
void aud_thread_destroy(void *p) { (void)p; }
uint64_t aud_system_cycle(void) { return 42; }
int fader_bypass_work(struct fader *f) { (void)f; assert(!"opaque watchdog"); return -1; }
void aimc_raise_named_event(void *a,uint32_t uid,const char *n,const void *p,int count,...)
{ (void)a; (void)uid; (void)n; (void)p; (void)count; ++raised; }
void dot_module_connection(int a,int b) { (void)a; (void)b; }
int msf_get_curr_num_md(void *p) { (void)p; return 0; }
int msf_get_avail_samples_read(void *p) { (void)p; return 0; }
void *msf_get_metadata(void *p,int n) { (void)p; (void)n; return NULL; }
void msf_add_put_metadata(void *p,void *m) { (void)p; (void)m; }
void aud_meta_data_free(void *p) { (void)p; }
void msf_add_update_write_buf(void *p,int n) { (void)p; (void)n; }
void msf_update_read_buf(void *p,int n) { (void)p; (void)n; }
int msf_process_metadata(void *p,void *d,unsigned n) { (void)p; (void)d; (void)n; return 0; }
void msf_flush_metadata(void *p) { (void)p; }
bool fader_trace_enabled(const char *n,int dir) { (void)n; (void)dir; return false; }
void logmm_printl(const void *l,int v,const char *fn,int line,const char *fmt,...)
{ (void)l; (void)v; (void)fn; (void)line; (void)fmt; }
void logmm_printlm(const char *l,int v,const char *fn,int line,const char *fmt,...)
{ (void)l; (void)v; (void)fn; (void)line; (void)fmt; }
void logm_message(const char *fmt,...) { (void)fmt; }
void ModuleRegistry_RegisterNamef(const void *c,const void *d,const char *n) { (void)c; (void)d; (void)n; }
#define KERNEL(name) void name(struct fader *f,int32_t *d,const int32_t *s,int n) { (void)f; (void)d; (void)s; (void)n; assert(!"opaque PCM kernel"); }
KERNEL(fadein_32bit_2ch) KERNEL(fadein_32bit_4ch) KERNEL(fadein_32bit_8ch) KERNEL(fadein_32bit_10ch)
KERNEL(fadeout_32bit_2ch) KERNEL(fadeout_32bit_4ch) KERNEL(fadeout_32bit_8ch) KERNEL(fadeout_32bit_10ch)

static void setup(struct fader *f,bool queue)
{
    memset(f,0,sizeof *f); active=f; f->cls=queue?&queue_class:&normal_class;
    f->name="test"; f->frame_bytes=4; f->channels=2; f->bits=16;
    f->wait_len=2; f->max_len=4; f->fade_len=2; f->chunk_len=4; f->chunk_bytes=16;
    f->pe.next=f->pe.prev=&f->pe; f->process=_fader_process_data_wait;
    f->padding_len=4; f->fadein=fadein_bypass; f->fadeout=fadeout_bypass;
    output_count=signals=waits=raised=creates=destroys=0; wait_action=NULL; timeout_result=0;
}
static void cleanup(struct fader *f)
{
    pending_events_empty(f); aud_data_adapter_flush(&f->q,QUEUED_BYTES(f));
    if(f->first) aud_object_put(f->first);
    if(f->meta_ext) aud_object_put(f->meta_ext);
    for(int i=0;i<object_count;++i) { assert(objects[i].refs==0); if(objects[i].data) free(((struct aud_data *)objects[i].p)->ptr); free(objects[i].p); }
    object_count=0; assert(fifo_count==0);
}
static void stop_wait(struct fader *f) { f->state=FS_STOP; }
static void drain_wait(struct fader *f) { aud_data_adapter_flush(&f->q,QUEUED_BYTES(f)); }
static void add_one(struct fader *f) { aud_data_adapter_push(&f->q,aud_data_new_zero(4)); }
static void test_first_padding(void)
{
    struct fader f; setup(&f,false); struct aud_port p={&f,0};
    struct aud_event *first=event(1); assert(!fader_event(&f,&p,first));
    assert(f.zero_left==4 && fifo_count==2 && creates==1 && output_count==0);
    struct aud_data *d=aud_data_new(4); memset(d->ptr,0x6a,4); assert(!fader_chain(&p,d));
    assert(!fader_output_work(NULL,&f));
    assert(output_count==3 && output[0].kind==2 && output[0].type==1);
    assert(output[1].kind==1 && output[2].kind==1 && output[1].size==8);
    for(int i=1;i<3;++i) for(unsigned j=0;j<output[i].size;++j) assert(output[i].bytes[j]==0);
    assert(f.zero_left==0 && fifo_count==1);
    assert(!fader_stop(&f) && f.state==FS_STOP && destroys==1); cleanup(&f);
}
static void test_pending(void)
{
    struct fader f; setup(&f,false); aud_data_adapter_push(&f.q,aud_data_new_zero(8));
    struct aud_event *old=event(13), *last=event(2), *replacement=event(13);
    pending_events_add(&f,old); pending_events_add(&f,last); pending_events_add(&f,replacement);
    assert(object(old)->refs==0); assert(!pending_events_check(&f) && output_count==0);
    pending_events_update(&f,8); pending_events_check(&f);
    assert(output_count==2 && output[0].type==2 && output[1].type==13);
    cleanup(&f);
}
static void test_flush_backpressure(void)
{
    struct fader f; setup(&f,true); struct aud_port p={&f,0};
    aud_data_adapter_push(&f.q,aud_data_new_zero(16));
    assert(!fader_chain(&p,aud_data_new_zero(4)) && waits==0);
    wait_action=drain_wait; assert(!fader_chain(&p,aud_data_new_zero(4)) && waits==1);
    struct aud_event *flush=event(7); flush->prop=AUD_EVENT_PROP_SYNC;
    fader_event(&f,&p,flush); assert(f.set_flush && !f.flushing && f.pe.next!=&f.pe);
    assert(!fader_chain(&p,aud_data_new_zero(4)) && QUEUED_BYTES(&f)==4 && !f.set_flush);
    flush=event(7); fader_event(&f,&p,flush);
    assert(f.flushing && output_count==1 && !f.set_flush);
    assert(fader_chain(&p,aud_data_new_zero(8))==-1 && f.dropped_frames==2);
    cleanup(&f);
}
static void test_es_short_tail(void)
{
    for(int dolby=0;dolby<2;++dolby) {
        struct fader f; setup(&f,false); f.state=FS_FADEOUT; f.fade_pos=1; f.dolby=dolby;
        struct aud_data *d=aud_data_new(4); memset(d->ptr,0x91,4); aud_data_adapter_push(&f.q,d);
        _fader_process_data_fadeout(&f);
        assert(output_count==2+dolby && f.state==FS_WAIT);
        for(unsigned j=0;j<16;++j) assert(output[0].bytes[j]==(j<4?0x91:0));
        for(int i=1;i<output_count;++i) for(unsigned j=0;j<16;++j) assert(output[i].bytes[j]==0);
        cleanup(&f);
    }
    struct fader f; setup(&f,false); f.state=FS_FADEIN;
    struct aud_data *d=aud_data_new(4); memset(d->ptr,0x7c,4); aud_data_adapter_push(&f.q,d);
    _fader_process_data_fadein(&f); assert(output_count==1 && output[0].bytes[0]==0x7c);
    for(int j=4;j<16;++j) assert(output[0].bytes[j]==0); cleanup(&f);
}
static void test_wait_boundary_underflow(void)
{
    struct fader f; setup(&f,false); f.first=event(1); f.fade_len=0;
    aud_data_adapter_push(&f.q,aud_data_new_zero(8)); wait_action=add_one;
    _fader_process_data_wait(&f); assert(waits==1 && output_count==1 && f.state==FS_RUNNING); cleanup(&f);
    setup(&f,false); f.first=event(1); f.first_sent=true; f.enable_underflow=true;
    timeout_result=1; wait_action=stop_wait;
    assert(_fader_process_data_wait(&f)==-1 && raised==1 && f.underflow_count==1);
    assert(output_count==2 && output[0].type==2 && output[1].port==1); cleanup(&f);
    setup(&f,true); f.first=event(1); f.first_sent=true; f.enable_underflow=true;
    timeout_result=1; wait_action=stop_wait;
    assert(_fader_process_data_wait(&f)==-1 && raised==0 && output_count==0); cleanup(&f);
}
static int process_calls;
static int process_probe(struct fader *f) { (void)f; ++process_calls; return -1; }
static void test_output_dispatch_and_flush(void)
{
    struct fader f; setup(&f,false); f.state=FS_RUNNING; f.process=process_probe;
    f.first=event(1); f.zero_left=3; wait_action=stop_wait;
    fader_output_work(NULL,&f);
    assert(process_calls==0 && f.state==FS_STOP && f.zero_left==0);
    f.process=process_probe; f.zero_left=3;
    fader_output_work(NULL,&f); assert(process_calls==1 && f.zero_left==0);
    aud_data_adapter_push(&f.q,aud_data_new_zero(4)); f.state=FS_RUNNING;
    fader_output_work(NULL,&f); assert(process_calls==2);
    f.set_flush=true; _fader_process_data_running(&f);
    assert(f.state==FS_WAIT && !f.set_flush && QUEUED_BYTES(&f)==0);
    f.block=true; queue_module_unblock(&f); assert(!f.block);
    cleanup(&f);
}
static void test_midstream_first_and_bypass(void)
{
    struct fader f; setup(&f,false); struct aud_port p={&f,0};
    f.state=FS_RUNNING; f.process=_fader_process_data_running; f.output_thread=&f;
    aud_data_adapter_push(&f.q,aud_data_new_zero(4));
    struct aud_event *e=event(1); fader_event(&f,&p,e);
    assert(f.first==e && object(e)->refs==2 && QUEUED(&f)==4);
    struct pending_event *pe=(struct pending_event *)f.pe.next;
    assert(pe->event==e && pe->at_bytes==16);
    assert(fifo_count==3 && fifo[0]->size==4 && fifo[1]->size==8);
    pending_events_check(&f); assert(output_count==0);
    pending_events_update(&f,16); pending_events_check(&f);
    assert(output_count==1 && output[0].type==1 && object(e)->refs==1);
    cleanup(&f);
    setup(&f,false); e=event(1); ((struct test_event *)e)->delay=1;
    fader_event(&f,&p,e);
    assert(f.low_delay && output_count==1 && fifo_count==0 && f.bypass_thread==&f);
    struct aud_data *d=aud_data_new(4); memset(d->ptr,0xed,4);
    assert(!fader_chain(&p,d));
    assert(output_count==2 && output[1].bytes[0]==0xed && fifo_count==0 && f.last_input==42);
    cleanup(&f);
}
static void test_event_ownership(void)
{
    struct fader f; setup(&f,false); struct aud_port p={&f,PORT_DISABLED};
    assert(fader_chain(&p,aud_data_new_zero(4))==-1 && f.dropped_frames==1);
    struct aud_event *e=event(2); f.enable_underflow=true; fader_event(&f,&p,e);
    assert(!f.enable_underflow && object(e)->refs==0);
    struct aud_data *md=aud_data_new_zero(4); md->type=AUD_DATA_META_EXT;
    fader_chain(&p,md); assert(f.meta_ext==md && object(md)->refs==1);
    struct aud_data *md2=aud_data_new_zero(4); md2->type=AUD_DATA_META_EXT; fader_chain(&p,md2);
    assert(object(md)->refs==0); _fader_write_metadata(&f,1,0,0); assert(!f.meta_ext);
    aud_data_adapter_push(&f.q,aud_data_new_zero(8)); e=event(9);
    ((struct test_event *)e)->passing=3; fader_event(&f,&p,e); assert(((struct test_event *)e)->passing==5);
    cleanup(&f);
}
int main(void)
{
    test_first_padding(); test_pending(); test_flush_backpressure(); test_es_short_tail();
    test_wait_boundary_underflow(); test_output_dispatch_and_flush();
    test_midstream_first_and_bypass(); test_event_ownership();
    puts("DSP1 fader: FIRST ordering, burst padding, pending ownership, flush/backpressure, ES tails, underflow passed");
    return 0;
}
