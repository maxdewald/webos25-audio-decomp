/* Recovered named-event dispatch/removal and output lifetime characterization.
 * Kernel services below are test doubles, not replacement IMC implementations.
 * The scheduling seam proves a specific detached-work interleaving, not real
 * kernel scheduling, private ABI compatibility, safe replacement, or unload. */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define info(...) ((void)0)
#define error(...) ((void)0)
#define container_of(p,t,m) ((t *)((char *)(p)-offsetof(t,m)))
struct mutex { int held; };
static void mutex_lock(struct mutex *m) { assert(!m->held); m->held=1; }
static void (*unlocked_hook)(struct mutex *);
static void mutex_unlock(struct mutex *m) {
    assert(m->held); m->held=0;
    if (unlocked_hook) unlocked_hook(m);
}
#define aud_mutex_lock mutex_lock
#define aud_mutex_unlock mutex_unlock
struct link { struct link *prev,*next; };
static void link_init(struct link *h) { h->next=h->prev=h; }
static void link_add(struct link *h,struct link *e) {
    e->prev=h->prev; e->next=h; h->prev->next=e; h->prev=e;
}
static void link_del(struct link *e) { e->prev->next=e->next; e->next->prev=e->prev; }
#define for_each_link_entry(e,h,m) \
    for ((e)=container_of((h)->next,__typeof__(*(e)),m); &(e)->m!=(h); \
         (e)=container_of((e)->m.next,__typeof__(*(e)),m))
// @match src/kernel/aud/adec_imc.c struct named_event\n\{[\s\S]*?\n\};
typedef struct { struct mutex lock; struct link event_list; unsigned core; } ImcInfo;
typedef struct { struct { unsigned sender; } senderInfo;
    struct { unsigned action,target,optionFlag; } targetInfo;
    unsigned extField,paramLength; void *param; } IpcHeader;
#define AUD_MODID_PROC(uid) ((uid)>>24)
enum { IMC_ACTION_NEVENT_DEL=7 };
static int sends,freed,send_result;
static IpcHeader sent;
static ImcInfo *active_imc;
static int aimc_ipc_send(ImcInfo *imc,unsigned core,IpcHeader *header,void *param) {
    assert(imc==active_imc && imc->lock.held && !param);
    assert(core==AUD_MODID_PROC(header->targetInfo.target));
    sent=*header; sends++; return send_result;
}
static void adec_free(void *p) { assert(active_imc->lock.held); freed++; free(p); }
#define ADEC_FREE(ctx,p) adec_free(p)
// @extract src/kernel/aud/adec_imc.c IMC_UnregisterNamedEvent process_named_event_handle
static struct named_event *event_new(ImcInfo *imc,unsigned id,unsigned target) {
    struct named_event *e=calloc(1,sizeof(*e)); assert(e);
    e->event_id=id; e->target_on=target; strcpy(e->event_name,"output_status");
    link_add(&imc->event_list,&e->list); return e;
}
static void test_imc_remove(void) {
    ImcInfo imc={.core=1}; active_imc=&imc; link_init(&imc.event_list);
    struct named_event *other=event_new(&imc,17,0x03000051);
    event_new(&imc,18,0x03000042);
    IMC_UnregisterNamedEvent(&imc,99);
    assert(!sends && !freed && !imc.lock.held);
    send_result=-5; /* A failed transport send does not preserve local ownership. */
    IMC_UnregisterNamedEvent(&imc,18);
    assert(sends==1 && freed==1 && !imc.lock.held);
    assert(sent.senderInfo.sender==1 && sent.targetInfo.action==7);
    assert(sent.targetInfo.target==0x03000042 && sent.extField==18);
    assert(!sent.paramLength && !sent.targetInfo.optionFlag);
    assert(imc.event_list.next==&other->list && imc.event_list.prev==&other->list);
    other->target_on=0x01000042;
    IMC_UnregisterNamedEvent(&imc,17);
    assert(sends==1 && freed==2 && imc.event_list.next==&imc.event_list);
    IMC_UnregisterNamedEvent(&imc,18);
    assert(sends==1 && freed==2 && !imc.lock.held);
}
/* Linux list/work services; a deterministic scheduling seam executes a
 * competing unhook in the real unregister's unlock-before-flush window. */
struct list_head { struct list_head *next,*prev; };
static void list_init(struct list_head *h) { h->next=h->prev=h; }
static void list_add_tail(struct list_head *e,struct list_head *h) {
    e->prev=h->prev; e->next=h; h->prev->next=e; h->prev=e;
}
static void list_del(struct list_head *e) { e->prev->next=e->next; e->next->prev=e->prev; }
#define list_first_entry_or_null(h,t,m) ((h)->next==(h)?NULL:container_of((h)->next,t,m))
#define list_for_each_entry(e,h,m) \
    for ((e)=container_of((h)->next,__typeof__(*(e)),m); &(e)->m!=(h); \
         (e)=container_of((e)->m.next,__typeof__(*(e)),m))
struct work_struct { bool pending; };
// @match src/kernel/aud/kaimc.c struct noti_info\n\{[\s\S]*?\n\};
// @match src/kernel/aud/kaimc.c struct noti_param\n\{[\s\S]*?\n\};
static struct { ImcInfo imc_info; struct list_head noti_list; } imc_context;
static __typeof__(imc_context) *gaimc=&imc_context;
static struct mutex wq_lock,api_lock;
#define AUDIO_IMC_WQ_LOCK() mutex_lock(&wq_lock)
#define AUDIO_IMC_WQ_UNLOCK() mutex_unlock(&wq_lock)
#define AUDIO_IMC_LOCK() mutex_lock(&api_lock)
#define AUDIO_IMC_UNLOCK() mutex_unlock(&api_lock)
#define GFP_KERNEL 0
#define ADEC_ERR_NONE 0
static void *system_wq;
static struct noti_info *active_noti;
static int noti_frees,param_frees,flushes,callbacks,returned_without_drain;
static int allocation_fails,queued,coalesced;
static int copied_values[4];
static void *kzalloc(size_t n,int flags) {
    if (allocation_fails) { allocation_fails--; return NULL; }
    void *p=calloc(1,n); assert(p); return p;
}
static void kfree(void *p) {
    if (p==active_noti) {
        assert(!wq_lock.held && !api_lock.held && !active_imc->lock.held);
        assert(!active_noti->param_lock.held && !active_noti->ws.pending);
        assert(callbacks==2); noti_frees++;
    } else param_frees++;
    free(p);
}
static bool queue_work(void *wq,struct work_struct *w) {
    assert(wq==system_wq && active_imc->lock.held && wq_lock.held && !api_lock.held);
    assert(!active_noti->param_lock.held);
    queued++; coalesced+=w->pending;
    bool already=w->pending; w->pending=true; return !already;
}
/* aimc_process_header holds the outer WQ lock; the extracted named-event
 * branch supplies the inner imc->lock and real callback dispatch. */
static void dispatch(unsigned id,unsigned source,void *param,unsigned size) {
    IpcHeader header={.senderInfo={source},.extField=id,.paramLength=size,.param=param};
    AUDIO_IMC_WQ_LOCK();
    assert(process_named_event_handle(active_imc,&header)==0);
    AUDIO_IMC_WQ_UNLOCK();
    assert(!active_imc->lock.held);
}
// @extract src/kernel/aud/kaimc.c noti_handler_work cb_func_local
static bool flush_work(struct work_struct *w) {
    assert(!wq_lock.held && !api_lock.held && !active_imc->lock.held);
    assert(gaimc->noti_list.next==&gaimc->noti_list);
    bool pending=w->pending; w->pending=false; flushes++;
    if (pending) noti_handler_work(w);
    return pending;
}
// @extract src/kernel/aud/kaimc.c aimc_unregister_named_event
static int notification(void *p,int size,void *arg) {
    assert(arg==active_noti && size==sizeof(int));
    assert(!active_noti->param_lock.held && !api_lock.held && !wq_lock.held);
    assert(returned_without_drain); /* Competing unhook returned, callback did not. */
    memcpy(&copied_values[callbacks++],p,size); return -9;
}
static void competing_unhook(struct mutex *m) {
    if (m!=&wq_lock) return;
    unlocked_hook=NULL;
    assert(gaimc->noti_list.next==&gaimc->noti_list && !flushes && !callbacks);
    /* The first remover exclusively owns the detached noti now. A second
     * API unregister (or list-scan+swap+flush) cannot see or drain that work. */
    aimc_unregister_named_event(71);
    assert(!flushes && !callbacks && !noti_frees);
    /* Once the lower registration is gone, late packets cannot requeue this
     * detached work; nevertheless its previously copied payloads still run. */
    int late=99;
    dispatch(71,0x03000042,&late,sizeof(late));
    assert(queued==2 && !callbacks);
    returned_without_drain=1;
}
static void test_detached_worker(void) {
    active_imc=&gaimc->imc_info; active_imc->core=1;
    link_init(&active_imc->event_list); list_init(&gaimc->noti_list);
    active_noti=calloc(1,sizeof(*active_noti)); assert(active_noti);
    active_noti->event=71; active_noti->repeat=true;
    active_noti->callback_func=notification; active_noti->callback_param=active_noti;
    list_init(&active_noti->param_list);
    list_add_tail(&active_noti->list,&gaimc->noti_list);
    struct named_event *e=event_new(active_imc,71,0x03000042);
    e->callback_func=cb_func_local; e->callback_arg=active_noti;
    assert(cb_func_local(NULL,NULL,0)==0);
    int value=21;
    dispatch(70,0x03000042,&value,sizeof(value));
    dispatch(71,0x03000043,&value,sizeof(value));
    assert(!queued && !active_noti->ws.pending);
    allocation_fails=1;
    dispatch(71,0x03000042,&value,sizeof(value));
    assert(!allocation_fails && !queued && active_noti->param_list.next==&active_noti->param_list);
    dispatch(71,0x03000042,&value,sizeof(value));
    value=34; dispatch(71,0x03000042,&value,sizeof(value));
    assert(queued==2 && coalesced==1 && !callbacks);
    value=0; /* The worker must consume the copied FIFO, not the input buffer. */
    unlocked_hook=competing_unhook;
    aimc_unregister_named_event(71);
    assert(callbacks==2 && copied_values[0]==21 && copied_values[1]==34);
    assert(flushes==1 && param_frees==2 && noti_frees==1);
    assert(active_imc->event_list.next==&active_imc->event_list);
    active_noti=NULL;
}
/* Output lifecycle uses its existing recovered bodies. Host structures below
 * carry semantic fields only; these tests do not validate the private ABI. */
enum output_type_t { OUTPUT_SPEAKER,OUTPUT_HP,OUTPUT_OPTIC,OUTPUT_ARC,OUTPUT_MAX };
enum output_codec_t { OUTPUT_CODEC_PCM,OUTPUT_CODEC_MAT=4 };
struct aud_block_info { bool isOpened; void *pipeline; };
struct aud_output_info {
    struct aud_block_info parent;
    bool suspended; enum output_type_t output_type;
    int uid_outctrl,uid_out,event_id;
    bool mute,earc_on,on;
    unsigned volume,delay,category_code,scms,codec,channel,acmod,sample_rate,bypass;
};
static struct mutex output_lock;
#define AUD_OUTPUT_LOCK() mutex_lock(&output_lock)
#define AUD_OUTPUT_UNLOCK() mutex_unlock(&output_lock)
#define CLOCK_TYPE(t) (t)
#define OUTPUT_TO_STR(o) "test-output"
#define noti(...) ((void)0)
#define INCLUDE_KDRV_HDMI20 1
static bool gOutputConnectSpdif;
static int pipeline_cookie,uid_counter,next_event=100,register_result=1;
static int sequence[64],sequence_count,output_unregistered;
enum { PIPE_NEW=1,PIPE_DEL,UNREGISTER,REGISTER,CLOCK_OFF,CLOCK_ON,ARC_OFF,IRQ_ON,IRQ_OFF };
static void record(int n) { assert(sequence_count<64); sequence[sequence_count++]=n; }
static void output_earc_dmac_irq(unsigned status) { }
static void HDMI21_ISRUNIT_Rx_Set_RegisterEarcDmacIsrCallBack(void (*cb)(unsigned)) {
    assert(!cb || cb==output_earc_dmac_irq); record(cb?IRQ_ON:IRQ_OFF);
}
static void *apipeline_new(const char *fmt,...) { record(PIPE_NEW); return &pipeline_cookie; }
static int apipeline_appendf(void *p,const char *fmt,...) { assert(p); return ++uid_counter; }
static int apipeline_prependf(void *p,const char *fmt,...) { assert(p); return ++uid_counter; }
static void apipeline_del(void *p) { assert(p==&pipeline_cookie); record(PIPE_DEL); }
static void aud_clock_enable_output(int type,int uid) { assert(uid>0); record(CLOCK_ON); }
static void aud_clock_disable_output(int type) { record(CLOCK_OFF); }
static void aud_hal_switch_spdif_clock(bool on) { }
static void audio_hal_speaker_start(bool on) { }
static void audio_hal_hp_start(bool on) { }
static void audio_hal_spdif_start(bool on) { }
static void audio_hal_arc_start(bool on) { assert(!on); record(ARC_OFF); }
static int _cb_output_status(void *p,int n,void *arg) { return 0; }
static int output_register(void *arg,int (*cb)(void *,int,void *),unsigned uid,const char *name,bool repeat) {
    struct aud_output_info *o=arg;
    assert(cb==_cb_output_status && uid==(unsigned)o->uid_out && repeat);
    assert(!strcmp(name,"output_status")); record(REGISTER);
    return register_result>0?++next_event:register_result;
}
static void output_unregister(int id) { assert(id>0); output_unregistered=id; record(UNREGISTER); }
#define aimc_register_named_event output_register
#define aimc_unregister_named_event output_unregister
static int output_set_mute(struct aud_block_info *b,bool v) { return 0; }
static int output_set_earc_on(struct aud_block_info *b,bool v) { return 0; }
static int output_set_on(struct aud_block_info *b,bool v) { return 0; }
static int output_set_category_code(struct aud_block_info *b,unsigned v) { return 0; }
static int output_set_scms(struct aud_block_info *b,unsigned v) { return 0; }
static int output_set_delay(struct aud_block_info *b,unsigned v) { return 0; }
static int output_set_volume(struct aud_block_info *b,unsigned v) { return 0; }
static void _set_format(struct aud_output_info *o,enum output_codec_t c,unsigned ch,
                        unsigned acmod,unsigned rate,bool bypass) { }
// @extract src/kernel/aud/output.c output_set_type output_destroy output_suspend output_resume
#undef aimc_register_named_event
#undef aimc_unregister_named_event
static void expect_sequence(const int *wanted,size_t count) {
    assert(sequence_count==(int)count);
    for (size_t i=0;i<count;i++) assert(sequence[i]==wanted[i]);
    sequence_count=0;
}
#define EXPECT(...) do { const int wanted[]={__VA_ARGS__}; \
    expect_sequence(wanted,sizeof(wanted)/sizeof(wanted[0])); } while (0)
static void test_output_lifetime(void) {
    struct aud_output_info o={.parent={true,&pipeline_cookie},.output_type=OUTPUT_ARC,
                              .event_id=45,.earc_on=true,.channel=8,.sample_rate=192000};
    assert(output_suspend(&o.parent)==0);
    assert(o.suspended && o.parent.isOpened && !o.parent.pipeline && o.event_id==45);
    EXPECT(ARC_OFF,CLOCK_OFF,PIPE_DEL); /* Suspend is not notification teardown. */
    assert(output_resume(&o.parent)==0);
    assert(!o.suspended && o.parent.isOpened && o.parent.pipeline);
    assert(output_unregistered==45 && o.event_id==101 && o.channel==2 && o.sample_rate==48000);
    EXPECT(PIPE_NEW,IRQ_ON,UNREGISTER,REGISTER,CLOCK_ON);
    int previous_uid=o.uid_out;
    assert(output_resume(&o.parent)==0 && o.uid_out==previous_uid && o.event_id==101);
    assert(!sequence_count); /* Not suspended: no new registration or pipeline. */
    output_destroy(&o.parent);
    assert(!o.parent.isOpened && !output_lock.held && output_unregistered==101);
    assert(o.event_id==101 && o.parent.pipeline==&pipeline_cookie); /* stale fields shipped */
    EXPECT(UNREGISTER,CLOCK_OFF,PIPE_DEL,IRQ_OFF);
    output_destroy(&o.parent); output_destroy(NULL);
    assert(!sequence_count && !output_lock.held);
    /* Not a general type-switch API: switching to HP does not unregister. */
    o.event_id=151;
    assert(output_set_type(&o.parent,OUTPUT_HP)==0 && o.event_id==151);
    EXPECT(PIPE_NEW,CLOCK_ON);
    /* Registration failure is cached but is not propagated by set_type. */
    register_result=-1;
    assert(output_set_type(&o.parent,OUTPUT_ARC)==0 && o.event_id==-1);
    EXPECT(PIPE_NEW,IRQ_ON,UNREGISTER,REGISTER,CLOCK_ON);
    output_destroy(&o.parent);
    EXPECT(CLOCK_OFF,PIPE_DEL,IRQ_OFF);
    o=(struct aud_output_info){.parent={true,&pipeline_cookie},.output_type=OUTPUT_ARC,.event_id=201};
    assert(output_suspend(&o.parent)==0 && o.event_id==201);
    EXPECT(ARC_OFF,CLOCK_OFF,PIPE_DEL);
    output_destroy(&o.parent); /* Teardown while suspended still unregisters. */
    assert(!o.parent.isOpened && !o.parent.pipeline && o.event_id==201);
    EXPECT(UNREGISTER,CLOCK_OFF,IRQ_OFF);
}
int main(void) {
    test_imc_remove();
    test_detached_worker();
    test_output_lifetime();
    puts("PASS: named removal, FIFO drain-before-free, detached-unhook race, output suspend/recreate/destroy");
    return 0;
}
