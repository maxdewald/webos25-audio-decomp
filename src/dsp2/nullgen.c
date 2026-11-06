/* C2 DSP2 nullgen @60099528..600998c8. Scalar/branch slots recovered from
 * saved main image. Semantic host types, log-only calls omitted. No timer or
 * sample conversion: downstream sync/backpressure controls production. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
struct nullgen {
    bool running, started;             /* +0xf0/+0xf1 */
    void *thread;                      /* +0xf4 */
    int channels, frames, bits;         /* +0xf8/+0xfc/+0x100 */
    unsigned cond[4], mutex[7];         /* +0x104/+0x114 */
};
struct nullgen_event;
struct nullgen_data;
extern void aud_mutex_init(void *), aud_mutex_lock(void *), aud_mutex_unlock(void *);
extern void aud_cond_init(void *), aud_cond_signal(void *), aud_cond_wait(void *,void *);
extern void *aud_thread_create(const char *,unsigned,unsigned,int (*)(struct nullgen *),struct nullgen *);
extern void aud_thread_destroy(void *);
extern struct nullgen_event *aud_event_new_first(void), *aud_event_new_last(void);
extern int aud_event_value_set_int(struct nullgen_event *,const char *,int);
extern struct nullgen_data *aud_data_new_zero(unsigned);
extern int aud_module_push_data(struct nullgen *,int,struct nullgen_data *);
extern int aud_module_push_event(struct nullgen *,int,struct nullgen_event *);
extern int aud_module_sync(struct nullgen *,int);
extern int aud_module_default_event(struct nullgen *,void *,struct nullgen_event *);
extern int aud_module_default_command(struct nullgen *,uint32_t,void *,unsigned);
extern void ModuleRegistry_Register(const void *);
extern const unsigned char nullgen_implementation[]; /* 0x60103320 */

/* @0x60099528: width snapshotted once: 8/16/32, unsupported widths become32.
 * Initial wait is IF, not WHILE. FIRST sent once (even spurious stop wake),
 * with no codec/es/dolby; rate48000, acmod2, little_endian1,signed1 are INT keys.
 * Pause sends LAST, waits once, then resumes WITHOUT another FIRST. loop is
 * do/while: it may emit one buffer after stop wakes the initial/pause wait. */
int nullgen_work_thread(struct nullgen *m)
{
    unsigned bytes=4;
    if(m->bits==16) bytes=2;
    else if(m->bits==8) bytes=1;
    else if(m->bits!=32) m->bits=32;
    aud_mutex_lock(m->mutex);
    if(!m->started) aud_cond_wait(m->cond,m->mutex);
    aud_mutex_unlock(m->mutex);
    struct nullgen_event *first=aud_event_new_first();
    if(first!=NULL) {
        aud_event_value_set_int(first,"bits-per-sample",m->bits);
        aud_event_value_set_int(first,"rate",48000);
        aud_event_value_set_int(first,"acmod",2);
        aud_event_value_set_int(first,"channels",m->channels);
        aud_event_value_set_int(first,"little-endian",1);
        aud_event_value_set_int(first,"signed",1);
        aud_module_push_event(m,0,first); aud_module_sync(m,0);
    }
    do {
        uint32_t size=bytes*(uint32_t)m->channels*(uint32_t)m->frames;
        struct nullgen_data *data=aud_data_new_zero(size);
        if(data==NULL) break;
        aud_module_push_data(m,0,data); aud_module_sync(m,0);
        aud_mutex_lock(m->mutex);
        if(!m->started) {
            struct nullgen_event *last=aud_event_new_last();
            if(last!=NULL) { aud_module_push_event(m,0,last); aud_module_sync(m,0); }
            aud_cond_wait(m->cond,m->mutex);
        }
        aud_mutex_unlock(m->mutex);
    } while(m->running);
    return 0;
}
/* @0x6009970c: inert input callback, does not consume/release its input. */
int nullgen_chain(void)
{
    return 0;
}
/* @0x60099714 */
int nullgen_event(struct nullgen *m,void *port,struct nullgen_event *e)
{
    return aud_module_default_event(m,port,e);
}
/* @0x60099728 */
int nullgen_command(struct nullgen *m,uint32_t command,void *arg,unsigned len)
{
    if(command==0x10010000 || command==0x10020000 || command==0x10000000) return 0;
    return aud_module_default_command(m,command,arg,len);
}
/* @0x60099778: compare before lock; parse unsigned base0, nonzero starts. */
int ullgen_start(struct nullgen *m,const void *prop,const char *value)
{
    (void)prop;
    bool started=(uint32_t)strtoul(value,NULL,0)!=0;
    if(started==m->started) return 0;
    aud_mutex_lock(m->mutex); m->started=started;
    aud_cond_signal(m->cond); aud_mutex_unlock(m->mutex);
    return 0;
}
/* @0x600997dc: thread creation failure leaves running=true. */
int nullgen_constructor(struct nullgen *m)
{
    m->running=m->started=false;
    m->channels=2; m->bits=32; m->frames=256;
    aud_cond_init(m->cond); aud_mutex_init(m->mutex);
    if(m->thread==NULL) {
        m->running=true;
        m->thread=aud_thread_create("nullgen",0x8000,10,nullgen_work_thread,m);
    }
    return 0;
}
/* @0x6009987c */
void nullgen_destructor(struct nullgen *m)
{
    if(!m->running || m->thread==NULL) return;
    aud_mutex_lock(m->mutex); m->running=false;
    aud_cond_signal(m->cond); aud_mutex_unlock(m->mutex);
    aud_thread_destroy(m->thread); m->thread=NULL;
}
/* @0x600998b8 */
void nullgen_initializer(void)
{
    ModuleRegistry_Register(nullgen_implementation);
}
