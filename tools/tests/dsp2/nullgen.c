#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../../../src/dsp2/nullgen.c"
struct nullgen_event { unsigned type; };
struct nullgen_data { unsigned size; };
const unsigned char nullgen_implementation[]={0};
static struct nullgen_event first={1},last={2};
static struct nullgen_data data;
static struct nullgen *module;
static unsigned first_count,last_count,waits,signals,packets,bytes,fields,destroyed;
static bool allocation_fails,thread_fails,pause_after_first;
static int locked;
void aud_mutex_init(void *m) { (void)m; }
void aud_mutex_lock(void *m) { (void)m; assert(locked++==0); }
void aud_mutex_unlock(void *m) { (void)m; assert(--locked==0); }
void aud_cond_init(void *c) { (void)c; }
void aud_cond_signal(void *c) { (void)c; ++signals; }
void aud_cond_wait(void *c,void *m)
{
    (void)c; (void)m; assert(locked==1); ++waits;
    if(pause_after_first && packets!=0) { module->running=false; module->started=true; }
    else module->started=true;
}
void *aud_thread_create(const char *name,unsigned stack,unsigned priority,int (*fn)(struct nullgen *),struct nullgen *m)
{
    assert(!strcmp(name,"nullgen") && stack==0x8000 && priority==10 && fn==nullgen_work_thread);
    module=m; return thread_fails?NULL:&data;
}
void aud_thread_destroy(void *p) { assert(p==&data); ++destroyed; }
struct nullgen_event *aud_event_new_first(void) { return &first; }
struct nullgen_event *aud_event_new_last(void) { return &last; }
int aud_event_value_set_int(struct nullgen_event *e,const char *name,int v)
{
    assert(e==&first);
    if(!strcmp(name,"bits-per-sample")) assert(v==module->bits);
    else if(!strcmp(name,"rate")) assert(v==48000);
    else if(!strcmp(name,"channels")) assert(v==module->channels);
    else if(!strcmp(name,"acmod")) assert(v==2);
    else assert(v==1 && (!strcmp(name,"little-endian") || !strcmp(name,"signed")));
    ++fields; return 0;
}
struct nullgen_data *aud_data_new_zero(unsigned size)
{
    if(allocation_fails) return NULL;
    data.size=size; return &data;
}
int aud_module_push_data(struct nullgen *m,int p,struct nullgen_data *d)
{
    assert(m==module && p==0 && d==&data); bytes=d->size; ++packets;
    if(pause_after_first) m->started=false;
    else m->running=false;
    return 0;
}
int aud_module_push_event(struct nullgen *m,int p,struct nullgen_event *e)
{
    assert(m==module && p==0);
    if(e->type==1) ++first_count; else { assert(e->type==2); ++last_count; }
    return 0;
}
int aud_module_sync(struct nullgen *m,int p) { assert(m==module && p==0); return 0; }
int aud_module_default_event(struct nullgen *m,void *p,struct nullgen_event *e) { (void)m;(void)p;(void)e;return 19; }
int aud_module_default_command(struct nullgen *m,uint32_t c,void *a,unsigned n) { (void)m;(void)c;(void)a;(void)n;return 23; }
void ModuleRegistry_Register(const void *p) { assert(p==nullgen_implementation); }
static void reset(struct nullgen *m)
{
    *m=(struct nullgen){0}; module=m;
    first_count=last_count=waits=signals=packets=bytes=fields=destroyed=0;
    allocation_fails=thread_fails=pause_after_first=false;
    assert(nullgen_constructor(m)==0);
}
int main(void)
{
    struct nullgen m;
    for(int bits=8;bits<=32;bits*=2) {
        reset(&m); m.bits=bits; nullgen_work_thread(&m);
        assert(first_count==1 && fields==6 && waits==1 && packets==1);
        assert(bytes==(unsigned)(256*2*(bits/8)));
    }
    reset(&m); m.bits=24; nullgen_work_thread(&m); assert(m.bits==32 && bytes==2048);
    reset(&m); pause_after_first=true; nullgen_work_thread(&m);
    assert(first_count==1 && last_count==1 && waits==2 && packets==1);
    reset(&m); allocation_fails=true; nullgen_work_thread(&m); assert(!packets && first_count==1);
    reset(&m); assert(!m.started); ullgen_start(&m,NULL,"0x2"); assert(m.started && signals==1);
    ullgen_start(&m,NULL,"1"); assert(signals==1);
    ullgen_start(&m,NULL,"0"); assert(!m.started && signals==2);
    nullgen_destructor(&m); assert(!m.running && !m.thread && destroyed==1);
    assert(nullgen_command(&m,0x10000000,NULL,0)==0);
    assert(nullgen_command(&m,42,NULL,0)==23 && nullgen_event(&m,NULL,&first)==19);
    m=(struct nullgen){0}; thread_fails=true; nullgen_constructor(&m);
    assert(m.running && !m.thread); nullgen_destructor(&m); assert(m.running);
    nullgen_initializer();
    puts("PASS nullgen: format, zero sizes, FIRST/LAST pause, command and thread lifecycle");
    return 0;
}
