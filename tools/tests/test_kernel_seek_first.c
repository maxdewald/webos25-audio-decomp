/* Mock DSP FIRST ownership and ordering. */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define info(...) ((void)0)
#define debug(...) ((void)0)
#define warn(...) ((void)0)
#define error(...) ((void)0)
enum { AUD_EVENT_TYPE_FIRST_AU=1, AUD_EVENT_TYPE_LAST_AU=2,
       AUD_EVENT_TYPE_DISCONNECT=6, AUD_EVENT_TYPE_FLUSH=7 };
#define AUD_PORT_DISABLED 2
#define AUD_PORT_WORK_CHAIN 1
#define AUD_IMPL_HAVE_OUTPUT_THREAD 1
#define AUD_IMPL_SKIP_SYNC 2
#define AUD_PORT_IS_DISABLED(p) ((p)->flag & AUD_PORT_DISABLED)
#define AUD_EVENT_IS_SYNC(e) ((e)->flags & 2)
#define AUD_EVENT_IS_DOWNWARD(e) ((e)->flags & 1)
struct aud_module; struct aud_port;
struct aud_event { int type, flags, refs; };
struct aud_data { unsigned size; };
struct mutex { int held; };
struct link { int unused; };
struct aud_module_impl {
    const char *name; unsigned flag;
    int (*event)(struct aud_module *,struct aud_port *,struct aud_event *);
    int (*chain)(struct aud_port *,struct aud_data *);
    int (*disconnect_port)(struct aud_module *,struct aud_port *);
    void (*deleting_port)(struct aud_module *,struct aud_port *);
};
struct aud_module {
    const char *name; int uid, thread_priority; struct aud_module_impl *impl;
    struct mutex block_mutex; int block_cond; bool block_module, do_chain_profile;
    unsigned long long chain_profile; void *othread, *aimc;
    struct link input_port_root, output_port_root;
};
struct aud_port {
    int number; struct aud_module *module; unsigned flag, flag_work;
    struct aud_port *up_peer; struct aud_event *current_first;
    struct mutex stream_lock, async_up_lock, async_down_lock, check_free_mutex;
    struct link down_peer_root, down_peer_node, module_node;
    int throwing; unsigned last_clock, passed;
};
int aud_module_event(struct aud_module *,struct aud_port *,struct aud_event *);
static void aud_module_unblock(struct aud_module *m) { m->block_module=false; }
static void aud_link_del(struct link *l) {}
static void aud_link_node_lock(struct link *l) {}
static int aud_link_node_count(struct link *l) { return 1; }
static struct aud_event disconnect_event, disconnect_sync;
static struct aud_event *aud_event_new_disconnect(void) {
    disconnect_event=(struct aud_event){6,1,1}; return &disconnect_event;
}
static struct aud_event *aud_event_new_disconnect_sync(void) {
    disconnect_sync=(struct aud_event){6,3,1}; return &disconnect_sync;
}
#define ADEC_FREE(ctx,p) free(p)
static struct aud_port *all_input;
#define aud_link_foreach(root,p) for ((p)=all_input; (p); (p)=NULL)
#define aud_link_foreach_end(root) ((void)0)
static void aud_mutex_lock(struct mutex *m) { assert(!m->held); m->held=1; }
static void aud_mutex_unlock(struct mutex *m) { assert(m->held); m->held=0; }
static void aud_cond_wait(int *c, struct mutex *m) { assert(m->held); }
static void aud_link_node_unlock(struct link *l) {}
static void aud_link_add_tail(struct link *r, struct link *n) {}
static int syncs;
static void aud_port_sync_locked(struct aud_port *p) { syncs++; }
static void aud_link_foreach_node(struct link *r, void *a, int (*cb)(void *,void *)) {}
static int aud_thread_get_preempt_priority(void) { return 10; }
static int aud_thread_get_priority(void) { return 10; }
static void aud_thread_set_priority(int p) {}
static unsigned aud_system_cycle(void) { return 123; }
static unsigned long long aud_thread_cycle(void) { static unsigned n; return ++n; }
static struct aud_event *aud_event_get(struct aud_event *e) { assert(e->refs>0); e->refs++; return e; }
static void aud_event_put(struct aud_event *e) { assert(e->refs>0); e->refs--; }
static int creates, allocation_fails;
static struct aud_event fallback;
static struct aud_event *aud_event_new_first(void) {
    creates++; if (allocation_fails) return NULL;
    fallback=(struct aud_event){AUD_EVENT_TYPE_FIRST_AU,1,1}; return &fallback;
}
static int data_puts;
static void aud_data_put(struct aud_data *d) { data_puts++; }
static int events, chains, sequence, event_sequence, chain_sequence;
static int event_handler(struct aud_module *m,struct aud_port *p,struct aud_event *e) {
    events++; event_sequence=++sequence; aud_event_put(e); return 0;
}
static int chain_handler(struct aud_port *p,struct aud_data *d) {
    chains++; chain_sequence=++sequence; assert(p->current_first); return 17;
}
static const char *named;
static void aimc_raise_named_event(void *a,int uid,const char *name,void *p,int n) { named=name; }
// @extract src/dsp1/module.c foreach_sync adjust_thread_priority del_input_port aud_module_disconnect aud_module_connect aud_module_chain_locked aud_module_event_flush_locked aud_module_event_disconnect_locked aud_module_event_first_au_locked aud_module_event_last_au_locked aud_module_event_locked aud_module_event
int main(void) {
    struct aud_module_impl impl={.name="output_arc", .flag=AUD_IMPL_SKIP_SYNC,
                                .event=event_handler,.chain=chain_handler};
    struct aud_module module={.name="output_arc",.impl=&impl};
    struct aud_event first={1,1,1};
    struct aud_port upstream={.module=&module,.current_first=&first};
    struct aud_port port={.module=&module};
    struct aud_data data={.size=32}; all_input=&port;
    assert(aud_module_connect(&upstream,&port)==0);
    assert(port.up_peer==&upstream && events==0 && !port.current_first);
    assert(aud_module_chain_locked(&port,&data)==17);
    assert(port.current_first==&first && first.refs==2 && !creates);
    assert(events==1 && chains==1 && event_sequence<chain_sequence);
    assert(port.passed==32 && port.last_clock==123 && !port.flag_work);
    assert(aud_module_chain_locked(&port,&data)==17 && events==1 && chains==2);
    struct aud_event last={2,1,1};
    assert(aud_module_event(&module,&port,&last)==0);
    assert(!port.current_first && first.refs==1 && last.refs==0);
    upstream.current_first=NULL;
    assert(aud_module_chain_locked(&port,&data)==17 && creates==1);
    assert(port.current_first==&fallback && fallback.refs==1);
    struct aud_event explicit_first={1,1,1};
    assert(aud_module_event(&module,&port,&explicit_first)==0);
    assert(port.current_first==&explicit_first && fallback.refs==0 && explicit_first.refs==1);
    struct aud_event flush_async={7,1,1};
    assert(aud_module_event(&module,&port,&flush_async)==0);
    assert(port.throwing && port.current_first==&explicit_first && named);
    assert(aud_module_chain_locked(&port,&data)==-1 && data_puts==1);
    struct aud_event flush_sync={7,3,1};
    assert(aud_module_event(&module,&port,&flush_sync)==0);
    assert(!port.throwing && port.current_first==&explicit_first);
    int before=events;
    assert(aud_module_chain_locked(&port,&data)==17 && events==before);
    port.flag=AUD_PORT_DISABLED;
    assert(aud_module_chain_locked(&port,&data)==-1 && data_puts==2);
    port.flag=0; aud_event_put(port.current_first); port.current_first=NULL;
    allocation_fails=1;
    assert(aud_module_chain_locked(&port,&data)==-1 && !port.current_first);
    assert(aud_module_event_locked(NULL,&port,&first)==-1);
    assert(aud_module_event(&module,&port,NULL)==-1);
    struct aud_event retained={1,1,1};
    struct aud_port *removed=calloc(1,sizeof(*removed)); assert(removed);
    upstream.current_first=&retained;
    *removed=(struct aud_port){.module=&module,.up_peer=&upstream,
                             .current_first=aud_event_get(&retained)};
    aud_module_disconnect(&upstream,removed);
    assert(upstream.current_first==&retained && retained.refs==1);
    assert(disconnect_event.refs==0 && disconnect_sync.refs==0);
    puts("PASS: graph-only connect, FIRST replay before data, fallback, LAST clearing, FLUSH retention, ownership");
    return 0;
}
