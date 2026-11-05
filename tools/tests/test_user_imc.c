/* Scoped host harness. Run with python3 tools/tests/test_user_imc.py. */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define _IOC_WRITE 1U
#define _IOC_READ 2U
#define _IOC(dir, type, nr, size) (((dir) << 30) | ((size) << 16) | ((type) << 8) | (nr))
#define _IOR(type, nr, data) _IOC(_IOC_READ, type, nr, sizeof(data))
#define _IOW(type, nr, data) _IOC(_IOC_WRITE, type, nr, sizeof(data))
#define _IOWR(type, nr, data) _IOC(_IOC_READ | _IOC_WRITE, type, nr, sizeof(data))
#define _IOC_TYPE(cmd) (((cmd) >> 8) & 255U)
#define _IOC_SIZE(cmd) (((cmd) >> 16) & 16383U)
#define _IOC_DIR(cmd) ((cmd) >> 30)
#define _IOC_NR(cmd) ((cmd) & 255U)
#define ENOIOCTLCMD 515
#define GFP_KERNEL 0
#define CONFIG_COMPAT 1
#define info(...) ((void)0)
#define error(...) ((void)0)
#define trace(...) ((void)0)
#define compat_ptr(value) ((void *)(uintptr_t)(uint32_t)(value))
struct list_head { struct list_head *next, *prev; };
struct mutex { int held; };
typedef struct { int unused; } wait_queue_head_t;
typedef struct { unsigned int paramLength; } IpcHeader;
struct inode { int unused; };
struct file { void *private_data; };
struct aud_proc_info { int unused; };
struct aud_mod_owner { int unused; };
struct user_imc;
struct auimc_ipc_header;
struct auimc_module;
struct auimc_implement;
struct auimc_implement_port_buf_def;
struct auimc_cache;
#define INIT_LIST_HEAD(p) ((p)->next = (p)->prev = (p))
static void mutex_init(struct mutex *m) { m->held = 0; }
static void mutex_lock(struct mutex *m) { assert(!m->held); m->held = 1; }
static void mutex_unlock(struct mutex *m) { assert(m->held); m->held = 0; }
static void init_waitqueue_head(wait_queue_head_t *q) { q->unused = 0; }
static void list_add(struct list_head *p, struct list_head *h)
{
    p->next = h->next; p->prev = h; h->next->prev = p; h->next = p;
}
static void list_del(struct list_head *p)
{
    p->next->prev = p->prev; p->prev->next = p->next;
}
#define list_first_entry_or_null(h, type, field) \
    ((h)->next == (h) ? NULL : (type *)((char *)(h)->next - offsetof(type, field)))
static struct mutex processor_list_lock;
static struct list_head processor_list = { &processor_list, &processor_list };
static unsigned int open_count;
static int alloc_fail, processor_fail, owner_fail, outstanding, freed;
static int affinity_calls, unregister_calls, del_owner_calls, helper_result;
static int from_calls, to_calls, fail_from_at, fail_to_at;
static int routed, route_calls, last_sync, events[8], event_count;
static unsigned int last_out, last_in;
static int last_out_port, last_in_port;
static struct aud_mod_owner *last_owner;
static struct aud_mod_owner owner_stub;
static struct aud_proc_info processor_stub;
static void *kzalloc(size_t size, int flags)
{
    if (alloc_fail) return NULL;
    void *p = calloc(1, size); assert(p); outstanding++; return p;
}
static void kfree(void *p) { if (p) { outstanding--; freed++; free(p); } }
static struct aud_proc_info *aud_modlist_register_processor(void *p, unsigned int *n)
{
    *n = 42; return processor_fail ? NULL : &processor_stub;
}
static void aud_modlist_unregister_processor(struct aud_proc_info *p)
{
    assert(p == &processor_stub); unregister_calls++; events[event_count++] = 1;
}
static struct aud_mod_owner *aud_modlist_new_owner(const char *fmt, void *p)
{
    assert(!strcmp(fmt, "user_%p")); return owner_fail ? NULL : &owner_stub;
}
static void aud_modlist_del_owner(struct aud_mod_owner *p)
{
    assert(p == &owner_stub); del_owner_calls++; events[event_count++] = 2;
}
static int aimc_toggle_cpu0_affinity(bool enable)
{
    assert(!enable); affinity_calls++; return 0;
}
static unsigned long copy_from_user(void *dst, const void *src, size_t size)
{
    from_calls++; if (from_calls == fail_from_at) return 1;
    memcpy(dst, src, size); return 0;
}
static unsigned long copy_to_user(void *dst, const void *src, size_t size)
{
    to_calls++; if (to_calls == fail_to_at) return 1;
    memcpy(dst, src, size); return 0;
}
static int aud_inst_connect(struct aud_mod_owner *o, unsigned int out, int op,
                            unsigned int in, int ip, int sync)
{
    last_owner = o; last_out = out; last_in = in; last_out_port = op;
    last_in_port = ip; last_sync = sync; routed = 8; route_calls++;
    return helper_result;
}
static int aud_inst_disconnect_output(struct aud_mod_owner *o, unsigned int uid, int port)
{
    last_owner = o; last_out = uid; last_out_port = port; routed = 14; route_calls++;
    return helper_result;
}
static int aud_inst_disconnect_input(struct aud_mod_owner *o, unsigned int uid, int port)
{
    last_owner = o; last_in = uid; last_in_port = port; routed = 15; route_calls++;
    return helper_result;
}
static int ipc_write(struct user_imc *, struct auimc_ipc_header *);
static int ipc_read(struct user_imc *, struct auimc_ipc_header *);
static int create_module(struct user_imc *, struct auimc_module *);
static int destroy_module(struct user_imc *, unsigned int);
static int add_implement(struct user_imc *, struct auimc_implement *);
static int add_implement_port_buf_def(struct user_imc *, struct auimc_implement_port_buf_def *);
static int aud_inst_role_set(struct aud_proc_info *, unsigned int, char *);
static unsigned int aud_inst_role_search(char *);
static int auimc_cache_sync(struct user_imc *, struct auimc_cache *);
static int auimc_munmap(struct user_imc *, unsigned int);

#include "user_imc_scoped.inc"

static uintptr_t ipc_pointer;
static int ipc_write(struct user_imc *u, struct auimc_ipc_header *h)
{
    routed = 2; route_calls++; ipc_pointer = (uintptr_t)h->header; return helper_result;
}
static int ipc_read(struct user_imc *u, struct auimc_ipc_header *h)
{
    routed = 3; route_calls++; ipc_pointer = (uintptr_t)h->header; return helper_result;
}
static int create_module(struct user_imc *u, struct auimc_module *m)
{
    routed = 6; route_calls++; m->uid = 0x1234; return helper_result;
}
static int destroy_module(struct user_imc *u, unsigned int uid)
{
    routed = 7; route_calls++; last_out = uid; return helper_result;
}
static int add_implement(struct user_imc *u, struct auimc_implement *p)
{
    routed = 4; route_calls++; return helper_result;
}
static int add_implement_port_buf_def(struct user_imc *u, struct auimc_implement_port_buf_def *p)
{
    routed = 5; route_calls++; return helper_result;
}
static int aud_inst_role_set(struct aud_proc_info *p, unsigned int uid, char *name)
{
    assert(p == &processor_stub); routed = 11; route_calls++; return helper_result;
}
static unsigned int aud_inst_role_search(char *name)
{
    routed = 111; route_calls++; return (unsigned int)helper_result;
}
static int auimc_cache_sync(struct user_imc *u, struct auimc_cache *p)
{
    routed = 12; route_calls++; return helper_result;
}
static int auimc_munmap(struct user_imc *u, unsigned int offset)
{
    routed = 13; route_calls++; return helper_result;
}
static void reset_io(void)
{
    from_calls = to_calls = fail_from_at = fail_to_at = routed = route_calls = 0;
    helper_result = 0;
}
static void test_lifecycle(void)
{
    struct file a = {0}, b = {0}, c = {0};
    alloc_fail = 1; assert(open(NULL, &a) == -ENOMEM); assert(!outstanding);
    alloc_fail = 0; processor_fail = 1;
    assert(open(NULL, &a) == -EIO); assert(!outstanding && !unregister_calls);
    processor_fail = 0; owner_fail = 1;
    assert(open(NULL, &a) == -EIO); assert(!outstanding && unregister_calls == 1);
    owner_fail = 0; event_count = 0;
    assert(open(NULL, &a) == 0); assert(open_count == 1 && affinity_calls == 0);
    struct user_imc *u = a.private_data;
    assert(u->number == 42 && u->task_id == UINT32_MAX);
    assert(u->messages.next == &u->messages && u->hma_shmems.next == &u->hma_shmems);
    assert(open(NULL, &b) == 0); assert(open_count == 2 && affinity_calls == 1);
    assert(open(NULL, &c) == 0); assert(open_count == 3 && affinity_calls == 1);
    struct hma_shmem *mapping = kzalloc(sizeof(*mapping), 0);
    list_add(&mapping->list, &u->hma_shmems);
    assert(release(NULL, &a) == 0);
    assert(events[0] == 1 && events[1] == 2 && open_count == 2);
    assert(release(NULL, &b) == 0); assert(release(NULL, &c) == 0);
    assert(open_count == 0 && !outstanding && processor_list.next == &processor_list);
    assert(affinity_calls == 1 && del_owner_calls == 3);
    event_count = 0;
}
static void test_dispatch(void)
{
    struct user_imc u = { .number = 9, .processor = &processor_stub, .mod_owner = &owner_stub };
    struct file f = { .private_data = &u };
    unsigned char data[64] = {0};
    struct auimc_connect *c = (void *)data;
    struct auimc_disconnect *d = (void *)data;
    reset_io();
    assert(sizeof(struct auimc_ipc_header) == 16);
    assert(ioctl(&f, _IOW('X', 8, int), (unsigned long)data) == -ENOIOCTLCMD);
    assert(ioctl(&f, _IOC(_IOC_WRITE, 'M', 8, 49), (unsigned long)data) == -ENOIOCTLCMD);
    assert(!from_calls && !route_calls);
    c->out_uid = 0x11; c->out_port = -2; c->in_uid = 0x22; c->in_port = 3;
    helper_result = -7;
    assert(ioctl(&f, AUIMC_IO_CONNECT_MODULE, (unsigned long)c) == -7);
    assert(routed == 8 && last_owner == &owner_stub && last_out == 0x11 && last_in == 0x22);
    assert(last_out_port == -2 && last_in_port == 3 && last_sync == 1 && !to_calls);
    d->uid = 0x33; d->port = -4;
    assert(ioctl(&f, AUIMC_IO_DISCONNECT_MODULE_OUTPUT, (unsigned long)d) == -7);
    assert(routed == 14 && last_out == 0x33 && last_out_port == -4);
    assert(ioctl(&f, AUIMC_IO_DISCONNECT_MODULE_INPUT, (unsigned long)d) == -7);
    assert(routed == 15 && last_in == 0x33 && last_in_port == -4);
    reset_io(); fail_from_at = 1;
    assert(ioctl(&f, AUIMC_IO_CONNECT_MODULE, (unsigned long)c) == -EFAULT && !route_calls);
    reset_io(); struct auimc_info out = {0};
    assert(ioctl(&f, AUIMC_IO_GET_INFO, (unsigned long)&out) == 0 && out.proc_id == 9);
    assert(!from_calls && to_calls == 1);
    reset_io(); fail_to_at = 1;
    assert(ioctl(&f, AUIMC_IO_GET_INFO, (unsigned long)&out) == -EFAULT);
    reset_io(); struct auimc_module module = {0};
    assert(ioctl(&f, AUIMC_IO_CREATE_MODULE, (unsigned long)&module) == 0);
    assert(module.uid == 0x1234 && from_calls == 1 && to_calls == 1);
    reset_io(); helper_result = -EIO;
    assert(ioctl(&f, AUIMC_IO_CREATE_MODULE, (unsigned long)&module) == -EIO && !to_calls);
    reset_io(); struct auimc_role role = {0};
    assert(ioctl(&f, AUIMC_IO_SET_ROLE, (unsigned long)&role) == 0 && routed == 11 && !to_calls);
    assert(ioctl(&f, AUIMC_IO_SEARCH_ROLE, (unsigned long)&role) == -EINVAL && routed == 111 && !to_calls);
    helper_result = 17;
    assert(ioctl(&f, AUIMC_IO_SEARCH_ROLE, (unsigned long)&role) == 0 && role.uid == 17 && to_calls == 1);
    reset_io(); unsigned int pid = 88;
    assert(ioctl(&f, AUIMC_IO_SET_PID, (unsigned long)&pid) == 0 && u.task_id == pid && !route_calls);
    assert(ioctl(&f, AUIMC_IO_ADD_IMPL2, (unsigned long)data) == -ENOIOCTLCMD);
    assert(ioctl(&f, AUIMC_IO_CREATE_LEGACY_MODULE, (unsigned long)data) == -ENOIOCTLCMD);
    assert(ioctl(&f, AUIMC_IO_DESTROY_LEGACY_MODULE, (unsigned long)data) == -ENOIOCTLCMD);
    assert(ioctl(&f, AUIMC_IO_SET_PROC_IDLE, (unsigned long)data) == -ENOIOCTLCMD);
    assert(ioctl(&f, _IOW('M', 1, struct auimc_info), (unsigned long)data) == -ENOIOCTLCMD);
    struct { unsigned int cmd; int route; } cases[] = {
        { AUIMC_IO_DESTROY_MODULE, 7 }, { AUIMC_IO_ADD_IMPL, 4 },
        { AUIMC_IO_ADD_IMPL_PORT_BUF_DEF, 5 }, { AUIMC_IO_CACHE_SYNC, 12 },
        { AUIMC_IO_MUNMAP, 13 },
    };
    for (size_t i = 0; i < sizeof(cases)/sizeof(cases[0]); i++) {
        reset_io(); assert(ioctl(&f, cases[i].cmd, (unsigned long)data) == 0);
        assert(routed == cases[i].route && from_calls == 1 && !to_calls);
    }
}
static void test_compat(void)
{
    struct user_imc u = { .number = 5 };
    struct file f = { .private_data = &u };
    struct auimc_ipc_header h;
    unsigned int cmds[] = { AUIMC_IO_IPC_WRITE, AUIMC_IO_IPC_READ };
    for (size_t i = 0; i < 2; i++) {
        reset_io(); memset(&h, 0, sizeof(h)); h.compat_sizer = UINT64_C(0xdeadbeef87654321);
        helper_result = 37;
        assert(compat_ioctl(&f, cmds[i], (unsigned long)&h) == 37);
        assert(h.compat_sizer == UINT64_C(0x87654321) && ipc_pointer == UINT64_C(0x87654321));
        assert(from_calls == 2 && to_calls == 1 && route_calls == 1 && routed == (int)i + 2);
        reset_io(); fail_from_at = 1;
        assert(compat_ioctl(&f, cmds[i], (unsigned long)&h) == -EFAULT && !to_calls && !route_calls);
        reset_io(); fail_to_at = 1;
        assert(compat_ioctl(&f, cmds[i], (unsigned long)&h) == -EFAULT && from_calls == 1 && !route_calls);
        reset_io(); fail_from_at = 2;
        assert(compat_ioctl(&f, cmds[i], (unsigned long)&h) == -EFAULT && to_calls == 1 && !route_calls);
    }
    reset_io(); struct auimc_info out;
    assert(compat_ioctl(&f, AUIMC_IO_GET_INFO, (unsigned long)&out) == 0 && out.proc_id == 5);
    assert(!from_calls && to_calls == 1);
}
int main(void)
{
    test_lifecycle(); test_dispatch(); test_compat();
    puts("PASS user_imc lifecycle/unwind/affinity, exact-command dispatch/copy faults, connect/disconnect, compat translation");
    return 0;
}
