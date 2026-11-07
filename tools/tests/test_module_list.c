// SPDX-License-Identifier: GPL-2.0-only
/* Kernel mocks, not a DSP runtime test. */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

typedef uint64_t phys_addr_t;
struct list_head { struct list_head *next, *prev; };
struct mutex { int held; };
typedef struct { int counter; } atomic_t;
typedef struct { int wakes; } wait_queue_head_t;
struct proc_dir_entry { int unused; };
struct aaudio_args_arg { void *callback, *arg, *read_fops; };
#define container_of(ptr, type, field) ((type *)((char *)(ptr) - offsetof(type, field)))
#define LIST_HEAD(name) struct list_head name = { &name, &name }
#define DEFINE_MUTEX(name) struct mutex name = {0}
#define INIT_LIST_HEAD(h) ((h)->next = (h)->prev = (h))
static void list_add(struct list_head *n, struct list_head *h)
{ n->next = h->next; n->prev = h; h->next->prev = n; h->next = n; }
static void list_del(struct list_head *n)
{ n->next->prev = n->prev; n->prev->next = n->next; n->next = n->prev = NULL; }
static bool list_empty(const struct list_head *h) { return h->next == h; }
#define list_entry(ptr,type,field) container_of(ptr,type,field)
#define list_first_entry_or_null(h,type,field) (list_empty(h) ? NULL : list_entry((h)->next,type,field))
#define list_for_each_entry(p,h,field) \
    for (p = list_entry((h)->next,__typeof__(*p),field); &p->field != (h); \
         p = list_entry(p->field.next,__typeof__(*p),field))
#define list_for_each_entry_safe(p,n,h,field) \
    for (p = list_entry((h)->next,__typeof__(*p),field), \
         n = list_entry(p->field.next,__typeof__(*p),field); &p->field != (h); \
         p = n, n = list_entry(n->field.next,__typeof__(*n),field))
static void mutex_lock(struct mutex *m) { assert(!m->held); m->held = 1; }
static void mutex_unlock(struct mutex *m) { assert(m->held); m->held = 0; }
static bool atomic_dec_and_test(atomic_t *v) { return --v->counter == 0; }
static void wake_up_all(wait_queue_head_t *w) { ++w->wakes; }
#define info(...) ((void)0)
#define error(...) ((void)0)
#define noti(...) ((void)0)
#define pnoti(...) ((void)0)
#define GFP_KERNEL 0
#define AUD_MODID_PROC(uid) (((uid) >> 24) & 0xff)
#define AUD_MODLIST_PROC(uid) AUD_MODID_PROC(uid)
#define AUD_MODLIST_MODIMPL(uid) (((uid) >> 16) & 0xff)
#define AUD_MODID_IS_INTERNAL(uid) (((uid) & 0xffff0000u) == ((uid) & 0xff000000u))
#define AUD_MODID_CONNECTOR(p) (((unsigned int)(p) << 24) | 2)
#define AUD_MODID_REGISTRY(p) (((unsigned int)(p) << 24) | 1)
#define CONNECTOR_CMD_CONNECT 0xf1000010u
#define CONNECTOR_CMD_DISCONNECT 0xf1010010u
#define GLUE_CMD_SET_SHARED_BUFFER 0xf1000014u
#define REGISTRY_CMD_DESTROY_MODULE 0x80f10004u
typedef struct { unsigned int output_module_id, output_module_port, input_module_id, input_module_port; } ConnectorCmdConnect;
typedef ConnectorCmdConnect ConnectorCmdDisconnect;
typedef struct { unsigned int uid; } RegistryCmdDestroyModule;
typedef struct { unsigned int shared_buf_offset, shared_buf_size, cowork_module_uid, max_access_size, aui_count; } GlueCmdSetSharedBuffer;
#define ADEC_CMD_SET_PROP 0x10180004u
typedef struct { int len; char str[]; } ImcCmdSetProp;

struct sent { unsigned int cmd, target, size, flags; bool sync; unsigned char payload[128]; };
static struct sent sent[128];
static int sends, allocations, fail_allocation, frees, glue_destroyed, shared_frees, roles_cleared;
static int helper_pre, helper_post, processor_removals;
static bool fail_shared, fail_glue;
static int allocation_balance;
static void *kzalloc(size_t size, int flags)
{
    (void)flags;
    if (++allocations == fail_allocation) return NULL;
    void *p = calloc(1, size);
    assert(p); ++allocation_balance; return p;
}
static void *kmalloc(size_t size, int flags) { return kzalloc(size ? size : 1, flags); }
static int kstrtouint(const char *text, unsigned int base, unsigned int *value)
{
    if (!*text || *text == '-' || *text == ' ' || *text == '\t') return -EINVAL;
    char *end;
    errno = 0;
    unsigned long long parsed = strtoull(text, &end, base);
    if (errno == ERANGE || parsed > UINT_MAX) return -ERANGE;
    if (end == text || (*end && strcmp(end, "\n"))) return -EINVAL;
    *value = (unsigned int)parsed;
    return 0;
}
static void kfree(void *p) { if (p) { ++frees; --allocation_balance; free(p); } }
static void vfree(void *p) { ++shared_frees; kfree(p); }
static void hma_free(const char *name, phys_addr_t p) { (void)name; (void)p; ++shared_frees; }
static void proc_remove(struct proc_dir_entry *p) { (void)p; }
static void remove_proc_subtree(const char *name, struct proc_dir_entry *p) { (void)name; (void)p; }
static void remove_proc_entry(const char *name, struct proc_dir_entry *p) { (void)name; (void)p; }
static int send(bool sync, void *imc, unsigned int cmd, unsigned int flags,
                unsigned int target, const void *payload, unsigned int size)
{
    assert(imc == NULL);
    assert(sends < 128 && size <= sizeof(sent[0].payload));
    sent[sends] = (struct sent){ .cmd=cmd, .target=target, .size=size, .flags=flags, .sync=sync };
    if (size) memcpy(sent[sends].payload, payload, size);
    ++sends;
    return -EIO; /* The shipped module-list layer deliberately ignores this. */
}
#define aimc_send_command(i,c,f,t,p,s) send(false,i,c,f,t,p,s)
#define aimc_send_command_sync(i,c,f,t,p,s) send(true,i,c,f,t,p,s)
static LIST_HEAD(processors);
static DEFINE_MUTEX(processors_lock);
static DEFINE_MUTEX(connection_lock);
static struct proc_dir_entry *proc_processors;
static bool allow_nonsecure_connect = true;
// @match src/kernel/aud/module_list.c (?ms)^struct (?:aud_mod_owner|jumper|port|aud_mod_impl_port_buf_def|aud_mod_inst|aud_mod_impl|aud_proc_info|aud_impl_helper)\n\{.*?^\};
// @declare src/kernel/aud/module_list.c unregister_impl unregister_processor aud_modlist_unregister_processor put_instance aud_modlist_del_owner get_input_port get_output_port disconnect_in_processor connect_in_processor jumper_inter_output_glue jumper_inter_input_glue connect_between_processor destroy_jumper inst_by_uid owner_inst_by_uid aud_modlist_disconnect_inst_output aud_modlist_disconnect_inst_input aud_inst_connect aud_inst_disconnect_output aud_inst_disconnect_input aud_modlist_connect_inst
#include "test_module_list_mocks.inc"
// @extract src/kernel/aud/module_list.c unregister_impl unregister_processor aud_modlist_unregister_processor put_instance aud_modlist_del_owner get_input_port get_output_port disconnect_in_processor connect_in_processor jumper_inter_output_glue jumper_inter_input_glue connect_between_processor destroy_jumper inst_by_uid owner_inst_by_uid aud_modlist_disconnect_inst_output aud_modlist_disconnect_inst_input aud_inst_connect aud_inst_disconnect_output aud_inst_disconnect_input aud_modlist_connect_inst
// @extract src/kernel/aud/module_list.c send_cmd proc_inst_cmd proc_inst_prop

static struct aud_mod_owner *owner(void)
{
    struct aud_mod_owner *o = kzalloc(sizeof(*o), 0);
    INIT_LIST_HEAD(&o->instances); strcpy(o->name,"owner"); return o;
}
static struct aud_proc_info *processor(int id)
{
    struct aud_proc_info *p = kzalloc(sizeof(*p), 0);
    p->proc_id = id; INIT_LIST_HEAD(&p->mods); list_add(&p->proc_list,&processors); return p;
}
static struct aud_mod_impl *implementation(struct aud_proc_info *p, int id)
{
    struct aud_mod_impl *m = kzalloc(sizeof(*m) + 16, 0);
    m->processor=p; m->impl_num=id; strcpy(m->name,"test");
    INIT_LIST_HEAD(&m->instances); list_add(&m->mods_list,&p->mods); return m;
}
static struct aud_mod_inst *instance(struct aud_mod_owner *o, struct aud_mod_impl *m, unsigned int uid)
{
    struct aud_mod_inst *i=kzalloc(sizeof(*i)+32,0);
    i->uid=uid; i->users.counter=1; i->owner=o; i->mod_impl=m; strcpy(i->name,"instance");
    INIT_LIST_HEAD(&i->input_ports); INIT_LIST_HEAD(&i->output_ports);
    if(o) list_add(&i->owner_list,&o->instances);
    if(m) { list_add(&i->impl_list,&m->instances); ++m->alloc_count; }
    return i;
}
static void reset_calls(void)
{ sends=0; allocations=0; fail_allocation=0; fail_shared=false; fail_glue=false; }
static void cleanup_processor(struct aud_proc_info *p)
{ assert(aud_modlist_unregister_processor(p)==0); }
static int pre_destroy(void *priv, const char *name, uint32_t uid)
{ (void)priv; (void)name; (void)uid; ++helper_pre; return 0; }
static int post_destroy(void *priv, const char *name, uint32_t uid)
{ (void)priv; (void)name; (void)uid; ++helper_post; return 0; }

static void test_proc_serialization(void)
{
    struct aud_mod_inst *inst = instance(NULL, NULL, 0x03020001);
    int balance = allocation_balance;
    char *property[] = {"out-bit-per-sample", "24", "ignored", NULL};
    reset_calls();
    assert(proc_inst_prop(inst, 3, property) == 0);
    const unsigned char expected[] = {
        21, 0, 0, 0, 'o', 'u', 't', '-', 'b', 'i', 't', '-', 'p', 'e', 'r', '-',
        's', 'a', 'm', 'p', 'l', 'e', '=', '2', '4', 0
    };
    assert(sends == 1 && !sent[0].sync && sent[0].flags == 0);
    assert(sent[0].cmd == ADEC_CMD_SET_PROP && sent[0].target == inst->uid);
    assert(sent[0].size == sizeof(expected));
    assert(!memcmp(sent[0].payload, expected, sizeof(expected)));
    assert(allocation_balance == balance);
    char *empty[] = {"", "", NULL};
    assert(proc_inst_prop(inst, 2, empty) == 0);
    assert(sent[1].size == 6 && sent[1].payload[0] == 1);
    assert(!memcmp(sent[1].payload + 4, "=", 2));
    char *missing[] = {"gain", NULL};
    assert(proc_inst_prop(inst, 1, missing) == -EINVAL && sends == 2);
    reset_calls(); fail_allocation = 1;
    assert(proc_inst_prop(inst, 2, property) == -ENOMEM && sends == 0);
    assert(allocation_balance == balance);

    char *command[] = {"0x50040008", "0x800000", "1", NULL};
    reset_calls();
    assert(proc_inst_cmd(inst, 3, command) == 0);
    const unsigned char gain[] = {0, 0, 0x80, 0, 1, 0, 0, 0};
    assert(sends == 1 && sent[0].cmd == 0x50040008 && sent[0].size == sizeof(gain));
    assert(sent[0].target == inst->uid && !sent[0].sync && sent[0].flags == 0);
    assert(!memcmp(sent[0].payload, gain, sizeof(gain)));
    char *words[] = {"0x50020020", "48000", "2", "32", "0", "0", "48000", "2", "24", NULL};
    assert(proc_inst_cmd(inst, 9, words) == 0 && sent[1].size == 32);
    assert(sent[1].cmd == 0x50020020);
    unsigned int formats[] = {48000, 2, 32, 0, 0, 48000, 2, 24};
    assert(!memcmp(sent[1].payload, formats, sizeof(formats)));
    char *no_payload[] = {"0x10010000", NULL};
    assert(proc_inst_cmd(inst, 1, no_payload) == 0 && sent[2].size == 0);
    char *mismatch[] = {"0x50020020", "7", NULL};
    assert(proc_inst_cmd(inst, 2, mismatch) == 0 && sent[3].size == 4);
    char *numeric[] = {"0x50050004", "+010", "4294967295", NULL};
    assert(proc_inst_cmd(inst, 3, numeric) == 0 && sent[4].size == 8);
    unsigned int values[] = {8, UINT_MAX};
    assert(!memcmp(sent[4].payload, values, sizeof(values)));
    assert(allocation_balance == balance);

    char *bad_command[] = {"oops", NULL};
    char *negative[] = {"0x50040008", "1", "-1", NULL};
    char *string[] = {"0x50040008", "pcm", NULL};
    char *buffer[] = {"0x50040008", "00:01:02:03", NULL};
    char *overflow[] = {"0x50040008", "4294967296", NULL};
    reset_calls();
    assert(proc_inst_cmd(inst, 0, NULL) == -EINVAL);
    assert(proc_inst_cmd(inst, 1, bad_command) == -EINVAL);
    assert(proc_inst_cmd(inst, 3, negative) == -EINVAL);
    assert(proc_inst_cmd(inst, 2, string) == -EINVAL);
    assert(proc_inst_cmd(inst, 2, buffer) == -EINVAL);
    assert(proc_inst_cmd(inst, 2, overflow) == -ERANGE);
    assert(sends == 0 && allocation_balance == balance);
    reset_calls(); fail_allocation = 1;
    assert(proc_inst_cmd(inst, 3, command) == -ENOMEM && sends == 0);
    assert(allocation_balance == balance);
    reset_calls();
    kfree(inst);
}

static void test_lookup_and_wrappers(void)
{
    struct aud_mod_owner *a=owner(), *b=owner();
    struct aud_proc_info *p=processor(2);
    struct aud_mod_impl *m=implementation(p,3);
    struct aud_mod_inst *i=instance(a,m,0x02030001), *j=instance(b,m,0x02030002);
    assert(owner_inst_by_uid(a,i->uid)==i && i->users.counter==1);
    allow_nonsecure_connect=false; assert(!owner_inst_by_uid(a,j->uid));
    allow_nonsecure_connect=true; assert(owner_inst_by_uid(a,j->uid)==j);
    assert(!owner_inst_by_uid(a,0x03030001));
    reset_calls();
    assert(aud_inst_connect(a,i->uid,4,j->uid,7,true)==0);
    assert(sends==1 && sent[0].sync && sent[0].cmd==CONNECTOR_CMD_CONNECT);
    ConnectorCmdConnect c;
    memcpy(&c,sent[0].payload,sizeof(c));
    assert(c.output_module_id==i->uid && c.output_module_port==4);
    assert(c.input_module_id==j->uid && c.input_module_port==7);
    assert(sent[0].target==0x02000002);
    assert(aud_inst_connect(a,i->uid,4,0x03000000,7,true)==-EINVAL);
    assert(aud_inst_connect(a,i->uid,4,i->uid,9,true)==-EINVAL);
    assert(aud_inst_disconnect_input(a,j->uid,7)==0);
    assert(list_empty(&i->output_ports) && list_empty(&j->input_ports));
    assert(aud_inst_disconnect_output(a,i->uid,4)==-EINVAL);
    assert(aud_inst_disconnect_input(a,0x03000000,0)==-EINVAL);
    assert(aud_inst_disconnect_output(a,0x03000000,0)==-EINVAL);
    aud_modlist_del_owner(a); aud_modlist_del_owner(b); cleanup_processor(p);
}
static void test_local_fanout_and_failures(void)
{
    struct aud_mod_owner *o=owner(); struct aud_proc_info *p=processor(2);
    struct aud_mod_impl *m=implementation(p,3);
    struct aud_mod_inst *out=instance(o,m,0x02030001), *a=instance(o,m,0x02030002), *b=instance(o,m,0x02030003);
    reset_calls(); fail_allocation=1;
    assert(connect_in_processor(out,1,a,2,false)==-ENOMEM && list_empty(&out->output_ports));
    reset_calls(); fail_allocation=2;
    assert(connect_in_processor(out,1,a,2,false)==-ENOMEM && list_empty(&a->input_ports));
    reset_calls();
    assert(connect_in_processor(out,1,a,2,false)==0);
    assert(connect_in_processor(out,1,b,3,true)==0);
    assert(connect_in_processor(out,9,a,2,false)==-EINVAL && sends==2);
    struct port *port=get_output_port(out,1);
    assert(get_input_port(a,2)->up_peer==port && get_input_port(b,3)->up_peer==port);
    assert(aud_inst_disconnect_input(o,a->uid,2)==0 && get_output_port(out,1)==port);
    assert(aud_inst_disconnect_output(o,out->uid,1)==0 && list_empty(&out->output_ports));
    assert(list_empty(&b->input_ports));
    assert(disconnect_in_processor(out,NULL,a,NULL)==-EINVAL);
    out->mod_impl=NULL;
    reset_calls(); assert(connect_in_processor(out,5,a,6,false)==-EINVAL);
    assert(get_output_port(out,5) && get_input_port(a,6) && sends==0);
    assert(aud_inst_disconnect_input(o,a->uid,6)==0 && sends==0);
    out->mod_impl=m; m->processor=NULL;
    assert(connect_in_processor(out,8,a,9,false)==-EINVAL);
    m->processor=p; assert(aud_inst_disconnect_output(o,out->uid,8)==0);
    aud_modlist_del_owner(o); cleanup_processor(p);
}
static void test_cross_processor(void)
{
    struct aud_mod_owner *o=owner(); struct aud_proc_info *p=processor(2), *q=processor(3);
    struct aud_mod_impl *m=implementation(p,3), *n=implementation(q,3);
    struct aud_mod_inst *out=instance(o,m,0x02030001), *in=instance(o,n,0x03030001);
    mock_output_impl=m; mock_input_impl=n;
    for(int sync=0;sync<2;++sync) {
        reset_calls();
        assert(aud_modlist_connect_inst(o,out,4,in,5,sync)==0);
        assert(sends==4 && sent[0].cmd==CONNECTOR_CMD_CONNECT && !sent[0].sync);
        assert(sent[1].cmd==GLUE_CMD_SET_SHARED_BUFFER && sent[1].sync);
        assert(sent[2].cmd==GLUE_CMD_SET_SHARED_BUFFER && sent[2].sync==(bool)sync);
        assert(sent[3].cmd==CONNECTOR_CMD_CONNECT && !sent[3].sync);
        struct jumper *j=get_input_port(in,5)->input_jumper;
        assert(j && !j->input_glue->owner && !j->output_glue->owner);
        assert(j->input_port==get_input_port(in,5) && j->output_port==get_output_port(out,4));
        int destroyed=glue_destroyed;
        if(sync) assert(aud_inst_disconnect_output(o,out->uid,4)==0);
        else assert(aud_inst_disconnect_input(o,in->uid,5)==0);
        assert(glue_destroyed==destroyed+2 && list_empty(&out->output_ports) && list_empty(&in->input_ports));
        assert(sent[4].cmd==CONNECTOR_CMD_DISCONNECT && sent[5].cmd==CONNECTOR_CMD_DISCONNECT);
    }
    reset_calls(); fail_allocation=1;
    assert(aud_modlist_connect_inst(o,out,4,in,5,false)==-1);
    reset_calls(); fail_glue=true;
    assert(aud_modlist_connect_inst(o,out,4,in,5,false)==-1);
    reset_calls(); fail_shared=true;
    assert(aud_modlist_connect_inst(o,out,4,in,5,false)==-1);
    assert(list_empty(&out->output_ports) && list_empty(&in->input_ports));
    aud_modlist_del_owner(o); cleanup_processor(p); cleanup_processor(q);
}
static void test_owner_and_processor_teardown(void)
{
    struct aud_mod_owner *o=owner(); struct aud_proc_info *p=processor(4);
    struct aud_mod_impl *m=implementation(p,2);
    struct aud_mod_inst *a=instance(o,m,0x04020001), *b=instance(o,m,0x04020002);
    struct aud_impl_helper helper={.destroy_pre=pre_destroy,.destroy_post=post_destroy};
    a->helper=&helper; a->role="role";
    assert(aud_modlist_connect_inst(o,a,0,b,0,false)==0);
    reset_calls();
    cleanup_processor(p);
    assert(a->owner==o && b->owner==o && !a->mod_impl && !b->mod_impl);
    assert(!a->impl_list.next && !b->impl_list.next && o->flags==FLAGS_HAVE_DEAD_INSTANCE);
    assert(o->flags_wq.wakes==2 && get_output_port(a,0) && sends==0);
    int before=frees; aud_modlist_del_owner(o);
    assert(frees==before+5 && sends==0 && helper_pre==1 && helper_post==1 && roles_cleared==1);
    o=owner(); p=processor(5); m=implementation(p,2); a=instance(o,m,0x05020001);
    a->users.counter=2;
    reset_calls(); aud_modlist_del_owner(o);
    assert(!a->owner && a->users.counter==1 && sends==0);
    put_instance(a); assert(sends==1 && sent[0].cmd==REGISTRY_CMD_DESTROY_MODULE);
    cleanup_processor(p);
}
int main(void)
{
    test_proc_serialization();
    test_lookup_and_wrappers(); test_local_fanout_and_failures();
    test_cross_processor(); test_owner_and_processor_teardown();
    assert(list_empty(&processors) && !allocation_balance);
    assert(!processors_lock.held && !connection_lock.held);
    puts("PASS module_list: proc property/command bytes, lengths, parse/allocation/send errors; owner/global lookup, UID wrappers, fan-out, jumper failures, processor death, reference-counted cleanup (ASan/UBSan)");
    return 0;
}
