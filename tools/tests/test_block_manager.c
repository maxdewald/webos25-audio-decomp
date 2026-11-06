/* Host-only block-manager contract harness. */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list_head { struct list_head *next, *prev; };
static void INIT_LIST_HEAD(struct list_head *h) { h->next = h->prev = h; }
static void list_add(struct list_head *n, struct list_head *h)
{ n->next = h->next; n->prev = h; h->next->prev = n; h->next = n; }
static void list_del(struct list_head *n) { n->prev->next = n->next; n->next->prev = n->prev; }
static int list_empty(const struct list_head *h) { return h->next == h; }
#define list_entry(p,t,m) ((t *)((char *)(p) - offsetof(t,m)))
#define list_for_each_entry(p,h,m) \
    for (p=list_entry((h)->next,__typeof__(*p),m); &p->m!=(h); p=list_entry(p->m.next,__typeof__(*p),m))
#define list_for_each_entry_safe(p,n,h,m) \
    for (p=list_entry((h)->next,__typeof__(*p),m),n=list_entry(p->m.next,__typeof__(*p),m); \
         &p->m!=(h); p=n,n=list_entry(n->m.next,__typeof__(*n),m))
struct mutex { int depth; };
static void mutex_lock(struct mutex *m) { assert(m->depth == 0); m->depth++; }
static void mutex_unlock(struct mutex *m) { assert(m->depth == 1); m->depth--; }
#define GFP_KERNEL 0
static bool fail_alloc;
static void *kzalloc(size_t n, int flags) { return fail_alloc ? NULL : calloc(1, n); }
#define kfree free
#define EXPORT_SYMBOL(x)
#define trace(...) ((void)0)
#define debug(...) ((void)0)
#define info(...) ((void)0)
#define noti(...) ((void)0)
#define error(...) ((void)0)
#define PORT_OUT false
#define PORT_IN true
#define BLOCK_PORT_IN 1
#define BLOCK_PORT_CONNECTED 2
#define SET_BLOCK_PORT_INPUT(x) ((x)->flag |= BLOCK_PORT_IN)
#define SET_BLOCK_PORT_CONNECTED(x) ((x)->flag |= BLOCK_PORT_CONNECTED)
#define UNSET_BLOCK_PORT_CONNECTED(x) ((x)->flag &= ~BLOCK_PORT_CONNECTED)
#define IS_BLOCK_PORT_CONNECTED(x) ((x)->flag & BLOCK_PORT_CONNECTED)
struct apipeline { const char *name; };
struct aud_block_info { const char *block_type; struct apipeline *pipeline; };
struct audio_block_impl {
    const char *block_type;
    struct list_head list;
    int (*pre_connect)(struct aud_block_info *, int, bool);
    int (*post_connect)(struct aud_block_info *, int, bool);
    int (*pre_disconnect)(struct aud_block_info *, int, bool);
    int (*post_disconnect)(struct aud_block_info *, int, bool);
    int (*suspend)(struct aud_block_info *);
    int (*resume)(struct aud_block_info *);
    int (*pre_reconnect)(struct aud_block_info *);
    int (*post_reconnect)(struct aud_block_info *);
};
static int connect_result, disconnect_result, connects, disconnects;
static struct apipeline *last_out, *last_in;
static int last_out_port, last_in_port;
static char events[4096];
static size_t event_length;
static void event(const char *name, const char *block)
{ event_length += (size_t)snprintf(events + event_length, sizeof(events) - event_length, "%s:%s;", name, block); }
static int apipeline_connect(struct apipeline *out, int op, struct apipeline *in, int ip)
{ last_out=out; last_out_port=op; last_in=in; last_in_port=ip; connects++; event("connect", "pipe"); return connect_result; }
static int apipeline_disconnect(struct apipeline *in, int ip)
{ last_in=in; last_in_port=ip; disconnects++; event("disconnect", "pipe"); return disconnect_result; }
static const char *apipeline_get_name(struct apipeline *p) { return p->name; }
// @match src/kernel/aud/block_manager.c (?ms)^struct (?:bm_list_block|bm_list_block_port|bm_list_connected_port|bm_list_port_forward|bm_list_reserved_conncection|bm_list_work_farm|aud_bm_info)\n\{.*?^\};
struct aud_bm_info _bm_info = { };
static int _bm_connect_block_locked(struct aud_block_info *, int, struct aud_block_info *, int);
// @extract src/kernel/aud/block_manager.c _bm_find_block_impl_by_type _bm_find_block_in_reserved_list _bm_find_block_by_index _bm_find_block_by_block_info _bm_get_block_port bm_get_port_forward _bm_add_port_connection _bm_connect_port _bm_del_port _bm_add_port _bm_disconnect_block_locked bm_disconnect_block _bm_connect_block_locked _bm_connect_block_reserved bm_connect_block_reserved_out _bm_disconnect_blocks_without_update _bm_reconnect_blocks bm_suspend bm_resume

static struct apipeline po={"out"}, pi={"in"}, pfo={"forward-out"}, pfi={"forward-in"};
static struct aud_block_info oi={"out",&po}, ii={"in",&pi};
static struct bm_list_block ob, ib;
static struct audio_block_impl impl;
static bool callback_locked;
#define CALLBACK(name) \
    static int name(struct aud_block_info *b, int port, bool in) \
    { assert(_bm_info.bm_lock.depth == callback_locked); assert(in == (b == &ii)); \
      assert(port == (b == &ii ? 7 : 3)); event(#name,b->block_type); return -73; }
CALLBACK(pre_connect)
CALLBACK(post_connect)
CALLBACK(pre_disconnect)
CALLBACK(post_disconnect)
#define LIFECYCLE(name) \
    static int name(struct aud_block_info *b) \
    { assert(_bm_info.bm_lock.depth == callback_locked); event(#name,b->block_type); return -73; }
LIFECYCLE(suspend)
LIFECYCLE(resume)
LIFECYCLE(pre_reconnect)
LIFECYCLE(post_reconnect)
static void clear_events(void) { events[0]=0; event_length=0; }
static void init_block(struct bm_list_block *b, struct aud_block_info *bi, int index)
{
    memset(b,0,sizeof(*b)); b->block_type=bi->block_type; b->block_info=bi; b->block_impl=&impl; b->index=index;
    INIT_LIST_HEAD(&b->list_port_input); INIT_LIST_HEAD(&b->list_port_output); INIT_LIST_HEAD(&b->list_port_forward);
    list_add(&b->list,&_bm_info.list_block);
}
static void reset(void)
{
    memset(&_bm_info,0,sizeof(_bm_info));
    INIT_LIST_HEAD(&_bm_info.list_block); INIT_LIST_HEAD(&_bm_info.list_block_impl);
    INIT_LIST_HEAD(&_bm_info.list_reserved_connection); INIT_LIST_HEAD(&_bm_info.list_work_farm);
    impl=(struct audio_block_impl){.block_type="out",.pre_connect=pre_connect,.post_connect=post_connect,
      .pre_disconnect=pre_disconnect,.post_disconnect=post_disconnect,.suspend=suspend,.resume=resume,
      .pre_reconnect=pre_reconnect,.post_reconnect=post_reconnect};
    list_add(&impl.list,&_bm_info.list_block_impl);
    init_block(&ib,&ii,2); init_block(&ob,&oi,1);
    connect_result=disconnect_result=connects=disconnects=0; fail_alloc=false; callback_locked=false; clear_events();
}
static void forwards(struct bm_list_port_forward *fo, struct bm_list_port_forward *fi)
{
    *fo=(struct bm_list_port_forward){.in=false,.pipeline=&pfo,.port_pipeline=31,.port_block=3};
    *fi=(struct bm_list_port_forward){.in=true,.pipeline=&pfi,.port_pipeline=71,.port_block=7};
    list_add(&fo->list,&ob.list_port_forward); list_add(&fi->list,&ib.list_port_forward);
}
static void cleanup(void)
{
    struct bm_list_block_port *p,*n;
    struct bm_list_reserved_conncection *r,*rn;
    list_for_each_entry_safe(p,n,&ib.list_port_input,list) _bm_del_port(p);
    list_for_each_entry_safe(p,n,&ob.list_port_output,list) _bm_del_port(p);
    list_for_each_entry_safe(r,rn,&_bm_info.list_reserved_connection,list) { list_del(&r->list); kfree(r); }
}
static void connect_checks(void)
{
    struct bm_list_port_forward fo,fi;
    reset(); forwards(&fo,&fi); connect_result=-19;
    assert(_bm_connect_block_locked(&oi,3,&ii,7)==-19);
    assert(!strcmp(events,"pre_connect:out;pre_connect:in;connect:pipe;"));
    assert(list_empty(&ob.list_port_output) && list_empty(&ib.list_port_input));
    assert(last_out==&pfo && last_out_port==31 && last_in==&pfi && last_in_port==71);
    connect_result=0; clear_events();
    assert(_bm_connect_block_locked(&oi,3,&ii,7)==0);
    assert(!strcmp(events,"pre_connect:out;pre_connect:in;connect:pipe;post_connect:out;post_connect:in;"));
    assert(IS_BLOCK_PORT_CONNECTED(_bm_get_block_port(&ob,3,PORT_OUT)));
    cleanup();
    reset(); fail_alloc=true;
    assert(_bm_connect_block_locked(&oi,3,&ii,7)==0);
    assert(strstr(events,"post_connect:out;post_connect:in;"));
    assert(list_empty(&ob.list_port_output) && list_empty(&ib.list_port_input));
    fail_alloc=false;
    reset(); list_del(&ob.list);
    assert(_bm_connect_block_locked(&oi,3,&ii,7)==-1 && connects==0 && !events[0]);
}
static void disconnect_checks(void)
{
    struct bm_list_port_forward fo,fi;
    reset(); forwards(&fo,&fi); assert(_bm_connect_block_locked(&oi,3,&ii,7)==0);
    clear_events(); callback_locked=true; disconnect_result=-29;
    assert(bm_disconnect_block(&ii,7)==-29);
    assert(!strcmp(events,"pre_disconnect:out;pre_disconnect:in;disconnect:pipe;"));
    assert(last_in==&pfi && last_in_port==71 && !list_empty(&ib.list_port_input));
    clear_events(); disconnect_result=0;
    assert(bm_disconnect_block(&ii,7)==0);
    assert(!strcmp(events,"pre_disconnect:out;pre_disconnect:in;disconnect:pipe;post_disconnect:out;post_disconnect:in;"));
    assert(list_empty(&ib.list_port_input));
    assert(list_empty(&_bm_get_block_port(&ob,3,PORT_OUT)->list_connected_port));
    assert(!IS_BLOCK_PORT_CONNECTED(_bm_get_block_port(&ob,3,PORT_OUT)));
    clear_events(); assert(bm_disconnect_block(&ii,7)==-1 && !events[0]);
    assert(bm_disconnect_block(NULL,7)==0 && _bm_info.bm_lock.depth==0);
    cleanup();
}
static void fan_in_checks(void)
{
    struct apipeline p2={"out2"};
    struct aud_block_info i2={"out2",&p2};
    struct bm_list_block b2;
    reset(); init_block(&b2,&i2,3);
    assert(_bm_connect_block_locked(&oi,3,&ii,7)==0);
    assert(_bm_connect_block_locked(&i2,3,&ii,7)==0);
    clear_events(); callback_locked=true;
    assert(bm_disconnect_block(&ii,7)==0);
    assert(!strcmp(events,"pre_disconnect:out2;pre_disconnect:out;pre_disconnect:in;disconnect:pipe;post_disconnect:out2;post_disconnect:out;post_disconnect:in;"));
    assert(list_empty(&_bm_get_block_port(&b2,3,PORT_OUT)->list_connected_port));
    _bm_del_port(_bm_get_block_port(&b2,3,PORT_OUT)); cleanup();
}
static void reserved_checks(void)
{
    struct bm_list_reserved_conncection *r;
    reset(); callback_locked=true;
    assert(bm_connect_block_reserved_out("out",1,3,&ii,7)==-1 && connects==1);
    r=list_entry(_bm_info.list_reserved_connection.next,struct bm_list_reserved_conncection,list);
    assert(!r->in && r->reserver==&ib && r->port_reserver==7 && r->port==3 && r->index==1);
    assert(r->list.next==&_bm_info.list_reserved_connection);
    assert(bm_connect_block_reserved_out("out",1,3,&ii,7)==-1 && connects==2);
    assert(r->list.next==&_bm_info.list_reserved_connection);
    assert(_bm_find_block_in_reserved_list("out",1,-1)==r);
    r->in=true; r->port_reserver=91;
    assert(bm_connect_block_reserved_out("out",1,3,&ii,7)==-1 && connects==3);
    assert(r->in && r->port_reserver==91);
    cleanup();
    reset(); callback_locked=true; connect_result=-5;
    assert(bm_connect_block_reserved_out("out",1,3,&ii,7)==-1 && connects==1);
    assert(!list_empty(&_bm_info.list_reserved_connection) && list_empty(&ib.list_port_input)); cleanup();
    reset(); callback_locked=true; list_del(&ob.list);
    assert(bm_connect_block_reserved_out("out",1,3,&ii,7)==-1 && connects==0);
    assert(!list_empty(&_bm_info.list_reserved_connection)); cleanup();
    reset(); callback_locked=true; fail_alloc=true;
    assert(bm_connect_block_reserved_out("out",1,3,&ii,7)==-1 && connects==0);
    assert(list_empty(&_bm_info.list_reserved_connection)); fail_alloc=false;
}
static void reconnect_checks(void)
{
    struct bm_list_port_forward fo,fi;
    reset(); forwards(&fo,&fi); assert(_bm_connect_block_locked(&oi,3,&ii,7)==0);
    clear_events(); callback_locked=true; disconnect_result=-11;
    assert(bm_suspend()==0 && _bm_info.suspended);
    assert(!strcmp(events,"disconnect:pipe;suspend:out;suspend:in;"));
    assert(!list_empty(&ib.list_port_input));
    clear_events(); connect_result=-12;
    assert(bm_resume()==0 && !_bm_info.suspended && _bm_info.bm_lock.depth==0);
    assert(!strcmp(events,"resume:out;resume:in;pre_reconnect:out;pre_reconnect:in;connect:pipe;post_reconnect:out;post_reconnect:in;"));
    assert(last_out==&pfo && last_out_port==31 && last_in==&pfi && last_in_port==71);
    cleanup();
    reset(); callback_locked=true; assert(bm_suspend()==0 && bm_resume()==0);
    assert(connects==0 && disconnects==0);
}
int main(void)
{
    connect_checks(); disconnect_checks(); fan_in_checks(); reserved_checks(); reconnect_checks();
    puts("PASS block-manager connect/disconnect/reservation/suspend/resume contracts (source-extracted host harness)");
    return 0;
}
