/* Usage: python3 tools/tests/test_kernel_seek.py. Workqueue callback FIFO contract. */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define info(...) ((void)0)
#define error(...) ((void)0)
#define ADEC_ERR_NONE 0
#define GFP_KERNEL 0
#define container_of(p,t,m) ((t *)((char *)(p)-offsetof(t,m)))
struct mutex { int held; };
static void mutex_lock(struct mutex *m) { assert(!m->held); m->held=1; }
static void mutex_unlock(struct mutex *m) { assert(m->held); m->held=0; }
struct list_head { struct list_head *next,*prev; };
static void init_list(struct list_head *l) { l->next=l->prev=l; }
static void list_add_tail(struct list_head *l,struct list_head *head) {
    l->prev=head->prev; l->next=head; head->prev->next=l; head->prev=l;
}
static void list_del(struct list_head *l) { l->prev->next=l->next; l->next->prev=l->prev; }
#define list_first_entry_or_null(h,t,m) ((h)->next==(h)?NULL:container_of((h)->next,t,m))
struct work_struct { int unused; };
struct noti_info {
    struct list_head list;
    int (*callback_func)(void *,int,void *); void *callback_param; struct work_struct ws;
    unsigned target,event; bool repeat; unsigned action_id;
    struct mutex param_lock; struct list_head param_list;
};
struct noti_param { struct list_head list; int size; unsigned char param[0]; };
static int fail_alloc, allocations, frees, queued;
static void *kzalloc(size_t size,int flags) { if(fail_alloc)return NULL; allocations++;return calloc(1,size); }
static void kfree(void *p) { frees++;free(p); }
static void *system_wq;
static bool queue_work(void *wq,struct work_struct *w) { queued++;return queued==1; }
static int callbacks, values[3];
static struct noti_info *active;
static int callback(void *p,int n,void *arg) {
    assert(arg==active && !active->param_lock.held && n==sizeof(int));
    memcpy(&values[callbacks++],p,n); return -9;
}
struct named_event {
    struct list_head list; unsigned event_id,processor_from; int once;
    unsigned target_on; char event_name[32];
    int (*callback_func)(void *,void *,unsigned); void *callback_arg;
};
typedef struct { struct mutex lock; struct list_head event_list; } ImcInfo;
typedef struct { struct { unsigned sender; } senderInfo;
                 int extField; unsigned paramLength; unsigned char param[16]; } IpcHeader;
#define aud_mutex_lock mutex_lock
#define aud_mutex_unlock mutex_unlock
#define for_each_link_entry(e,h,m) \
    for ((e)=container_of((h)->next,struct named_event,m); &(e)->m!=(h); \
         (e)=container_of((e)->m.next,struct named_event,m))
#define link_del list_del
#define ADEC_FREE(ctx,p) kfree(p)
#include "noti_under_test.inc"
int main(void) {
    struct noti_info n={.callback_func=callback,.repeat=true};
    init_list(&n.list); init_list(&n.param_list); n.callback_param=&n; active=&n;
    assert(cb_func_local(NULL,NULL,0)==0 && !allocations);
    int value=31;
    assert(cb_func_local(&n,&value,sizeof(value))==0);
    value=72;
    assert(cb_func_local(&n,&value,sizeof(value))==0);
    value=99;
    assert(!callbacks && queued==2 && allocations==2);
    noti_handler_work(&n.ws);
    assert(callbacks==2 && values[0]==31 && values[1]==72 && frees==2);
    assert(n.param_list.next==&n.param_list && !n.param_lock.held);
    fail_alloc=1;
    assert(cb_func_local(&n,&value,sizeof(value))==-1 && queued==2);
    fail_alloc=0;
    ImcInfo imc={0}; init_list(&imc.event_list);
    struct named_event registration={.event_id=7,.target_on=0x03000042,
                                    .callback_func=cb_func_local,.callback_arg=&n};
    list_add_tail(&registration.list,&imc.event_list);
    IpcHeader packet={.senderInfo={0x03000042},.extField=8,.paramLength=sizeof(value)};
    memcpy(packet.param,&value,sizeof(value));
    assert(process_named_event_handle(&imc,&packet)==0 && queued==2);
    packet.extField=7; packet.senderInfo.sender++;
    assert(process_named_event_handle(&imc,&packet)==0 && queued==2);
    packet.senderInfo.sender--;
    assert(process_named_event_handle(&imc,&packet)==0 && queued==3 && callbacks==2);
    value=0; memset(packet.param,0,sizeof(packet.param));
    noti_handler_work(&n.ws);
    assert(callbacks==3 && values[2]==99 && !imc.lock.held);
    assert(imc.event_list.next==&registration.list);
    puts("PASS: sender/event-id selection, copied payload, deferred FIFO callbacks, allocation failure");
    return 0;
}
