#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define info(...) ((void)0)
#define debug(...) ((void)0)
#define error(...) ((void)0)
#define noti(...) ((void)0)
#define trace(...) ((void)0)
#define GFP_KERNEL 0
#define STR_INOUT(in) ((in) ? "IN" : "OUT")
#define PORT_IN true
#define PORT_OUT false
#define BLOCK_PORT_CONNECTED 2
#define SET_BLOCK_PORT_CONNECTED(port) ((port)->flag |= BLOCK_PORT_CONNECTED)
#define AUD_MODLIST_PROC(uid) ((uid) >> 24)
#define AUD_MODLIST_MODIMPL(uid) (((uid) >> 16) & 255)

struct list_head { struct list_head *next, *prev; };
#define INIT_LIST_HEAD(head) (*(head) = (struct list_head){ (head), (head) })
#define list_entry(node, type, member) \
    ((type *)((char *)(node) - offsetof(type, member)))
#define list_for_each_entry(entry, head, member) \
    for (struct list_head *cursor = (head)->next; \
         cursor != (head) && ((entry) = list_entry(cursor, __typeof__(*(entry)), member), 1); \
         cursor = cursor->next)
static void list_add(struct list_head *node, struct list_head *head)
{
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
}

struct apipeline { int identity; };
struct aud_block_info { const char *block_type; };
struct mutex { bool held; };
static void mutex_lock(struct mutex *lock)
{
    assert(!lock->held);
    lock->held = true;
}
static void mutex_unlock(struct mutex *lock)
{
    assert(lock->held);
    lock->held = false;
}
struct bm_list_block {
    struct list_head list, list_port_forward, list_port_input, list_port_output;
    const char *block_type;
    int index;
};
// @match src/kernel/aud/block_manager.c (?s)struct bm_list_block_port\s*\{.*?\n\};
// @match src/kernel/aud/block_manager.c (?s)struct bm_list_connected_port\s*\{.*?\n\};
// @match src/kernel/aud/block_manager.c (?s)struct bm_list_port_forward\s*\{.*?\n\};
struct aud_bm_info { struct list_head list_block; struct mutex bm_lock; };
static struct aud_bm_info _bm_info;

struct seq_file { char text[8192]; size_t length; void *private; };
static void seq_printf(struct seq_file *seq, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int length = vsnprintf(seq->text + seq->length, sizeof(seq->text) - seq->length,
                           format, args);
    va_end(args);
    assert(length >= 0 && (size_t)length < sizeof(seq->text) - seq->length);
    seq->length += length;
}

static struct bm_list_block block;
static struct aud_block_info known = { "renderer" };
static bool fail_allocation;
static int allocations_until_failure = -1;
static struct bm_list_block *_bm_find_block_by_block_info(struct aud_block_info *info)
{
    return info == &known ? &block : NULL;
}
static void *kzalloc(size_t size, int flags)
{
    if (fail_allocation || allocations_until_failure == 0)
        return NULL;
    if (allocations_until_failure > 0)
        allocations_until_failure--;
    return calloc(1, size);
}

// @extract src/kernel/aud/block_manager.c bm_get_port_forward bm_register_port_forward _bm_get_block_port _bm_add_port_connection _bm_connect_port bm_dot

struct aud_proc_info {
    struct list_head proc_list, mods;
    struct mutex mods_lock;
    unsigned int proc_id;
    char name[32];
};
struct aud_mod_impl {
    struct list_head mods_list, instances;
    struct mutex lock;
    struct aud_proc_info *processor;
    unsigned int impl_num;
    char name[32];
};
struct proc_dir_entry { const char *name; char target[128]; bool removed; };
struct aud_mod_inst {
    struct list_head impl_list, input_ports, output_ports;
    struct mutex lock;
    struct aud_mod_impl *mod_impl;
    void *owner;
    unsigned int uid;
    const char *role;
    struct proc_dir_entry *proc_role;
    char name[64];
};
typedef unsigned long long phys_addr_t;
// @match src/kernel/aud/module_list.c (?s)struct jumper\s*\{.*?\n\};
// @match src/kernel/aud/module_list.c (?s)struct port\s*\{.*?\n\};
static struct list_head processors;
static struct mutex processors_lock;
static struct proc_dir_entry role_root, role_link;
static struct proc_dir_entry *proc_roles = &role_root;
static bool fail_role, fail_impl, fail_instance;
static struct aud_mod_impl predefined;
static struct aud_mod_inst dummy;

static void init_instance(struct aud_mod_inst *inst, struct aud_mod_impl *impl,
                          unsigned int uid, const char *name)
{
    *inst = (struct aud_mod_inst){ .mod_impl = impl, .uid = uid };
    snprintf(inst->name, sizeof(inst->name), "%s", name);
    INIT_LIST_HEAD(&inst->input_ports);
    INIT_LIST_HEAD(&inst->output_ports);
    list_add(&inst->impl_list, &impl->instances);
}
static struct aud_mod_impl *register_impl(struct aud_proc_info *processor,
                                         const char *name, int rank, int number)
{
    assert(processor->mods_lock.held);
    if (fail_impl)
        return NULL;
    predefined = (struct aud_mod_impl){ .processor = processor, .impl_num = number };
    snprintf(predefined.name, sizeof(predefined.name), "%s", name);
    INIT_LIST_HEAD(&predefined.instances);
    list_add(&predefined.mods_list, &processor->mods);
    return &predefined;
}
static struct aud_mod_inst *create_inst_on_sync(struct aud_mod_impl *impl,
                                                void *owner, bool sync)
{
    assert(impl->processor->mods_lock.held);
    if (fail_instance)
        return NULL;
    init_instance(&dummy, impl, (impl->processor->proc_id << 24) |
                  (impl->impl_num << 16) | 1, "dummy");
    dummy.owner = owner;
    return &dummy;
}
static const char *kstrdup_const(const char *role, int flags)
{
    return fail_role ? NULL : strdup(role);
}
static void kfree_const(const char *value) { free((void *)value); }
#define kfree free
static char *kasprintf(int flags, const char *format, ...)
{
    char *result = malloc(128);
    assert(result);
    va_list args;
    va_start(args, format);
    assert(vsnprintf(result, 128, format, args) < 128);
    va_end(args);
    return result;
}
static struct proc_dir_entry *proc_symlink(const char *name,
                                          struct proc_dir_entry *parent,
                                          const char *target)
{
    role_link = (struct proc_dir_entry){ .name = name };
    snprintf(role_link.target, sizeof(role_link.target), "%s", target);
    return &role_link;
}
static void proc_remove(struct proc_dir_entry *entry)
{
    if (entry)
        entry->removed = true;
}
// @extract src/kernel/aud/module_list.c aud_modlist_role_set aud_inst_role_set ml_show status_show

static void test_forwarding(void)
{
    struct apipeline original = { 1 }, replacement = { 2 };
    struct aud_block_info missing = { "missing" };
    struct bm_list_port_forward *input, *output, *newest;
    INIT_LIST_HEAD(&block.list_port_forward);
    assert(bm_get_port_forward(&known, true, 7) == NULL);
    assert(bm_register_port_forward(&missing, true, 7, &original, 3) == -1);
    fail_allocation = true;
    assert(bm_register_port_forward(&known, true, 7, &original, 3) == -1);
    assert(block.list_port_forward.next == &block.list_port_forward);
    fail_allocation = false;
    assert(bm_register_port_forward(&known, true, 7, &original, 3) == 0);
    input = bm_get_port_forward(&known, true, 7);
    assert(input && input->pipeline == &original && input->port_pipeline == 3);
    assert(bm_get_port_forward(&known, false, 7) == NULL);
    assert(bm_register_port_forward(&known, true, 7, &original, 3) == -1);
    fail_allocation = true;
    assert(bm_register_port_forward(&known, true, 7, &replacement, 3) == 0);
    assert(bm_get_port_forward(&known, true, 7) == input);
    assert(input->pipeline == &replacement);
    fail_allocation = false;
    assert(bm_register_port_forward(&known, false, 7, &original, 3) == 0);
    output = bm_get_port_forward(&known, false, 7);
    assert(output && output != input && output->pipeline == &original);
    assert(bm_register_port_forward(&known, true, 7, &original, 4) == 0);
    newest = bm_get_port_forward(&known, true, 7);
    assert(newest != input && newest->port_pipeline == 4);
    assert(input->pipeline == &replacement && input->port_pipeline == 3);
    assert(bm_get_port_forward(&missing, true, 7) == NULL);
    free(newest);
    free(output);
    free(input);
    INIT_LIST_HEAD(&block.list_port_forward);
}

static void init_processor(struct aud_proc_info *processor, unsigned int number)
{
    *processor = (struct aud_proc_info){ .proc_id = number };
    snprintf(processor->name, sizeof(processor->name), "processor_%u", number);
    INIT_LIST_HEAD(&processor->mods);
    list_add(&processor->proc_list, &processors);
}

static void test_roles(void)
{
    struct aud_proc_info processor;
    struct aud_mod_impl impl = { .impl_num = 2 };
    struct aud_mod_inst instance;
    int owner_cookie;
    INIT_LIST_HEAD(&processors);
    init_processor(&processor, 7);
    impl.processor = &processor;
    snprintf(impl.name, sizeof(impl.name), "renderer");
    INIT_LIST_HEAD(&impl.instances);
    list_add(&impl.mods_list, &processor.mods);
    init_instance(&instance, &impl, 0x07020005, "renderer_07020005");
    instance.owner = &owner_cookie;

    assert(aud_inst_role_set(NULL, 0x08020005, "absent") == -EINVAL);
    assert(!processors_lock.held && !instance.role);
    assert(aud_inst_role_set(NULL, instance.uid, "main") == 0);
    assert(!strcmp(instance.role, "main") && instance.owner == &owner_cookie);
    assert(!strcmp(instance.proc_role->target,
                   "../processors/processor_7/renderer/07020005"));
    assert(!processors_lock.held && !processor.mods_lock.held);

    assert(aud_inst_role_set(&processor, instance.uid, "secondary") == 0);
    assert(!strcmp(instance.role, "secondary"));
    fail_role = true;
    assert(aud_inst_role_set(&processor, instance.uid, "unallocated") == 0);
    assert(!instance.role && !instance.proc_role && role_link.removed);
    fail_role = false;

    assert(aud_inst_role_set(&processor, 0x07030005, "missing-impl") == -EINVAL);
    assert(!processor.mods_lock.held);
    fail_instance = true;
    assert(aud_inst_role_set(&processor, 0x07020099, "missing-inst") == -EINVAL);
    assert(!processor.mods_lock.held);
    fail_instance = false;
    assert(aud_inst_role_set(&processor, 0x08020099, "foreign-prefix") == 0);
    assert(dummy.uid == 0x07020001 && dummy.owner == NULL);
    assert(!strcmp(dummy.role, "foreign-prefix"));
    assert(!strcmp(dummy.proc_role->target,
                   "../processors/processor_7/renderer/07020001"));
    free((void *)dummy.role);
    INIT_LIST_HEAD(&impl.instances);

    fail_impl = true;
    assert(aud_inst_role_set(&processor, 0x07000099, "predefined") == -EINVAL);
    assert(!processor.mods_lock.held);
    fail_impl = false;
    assert(aud_inst_role_set(&processor, 0x07000099, "predefined") == 0);
    assert(predefined.impl_num == 0 && !strcmp(predefined.name, "_predefined"));
    assert(dummy.uid == 0x07000001 && dummy.owner == NULL);
    free((void *)dummy.role);
    INIT_LIST_HEAD(&processors);
}

static void init_port(struct port *port, struct aud_mod_inst *instance,
                      int index, bool input)
{
    *port = (struct port){ .inst = instance, .index = index, .input = input };
    INIT_LIST_HEAD(&port->down_peers);
    INIT_LIST_HEAD(&port->output_jumpers);
    list_add(&port->port_list, input ? &instance->input_ports : &instance->output_ports);
}

static void test_module_graph(void)
{
    struct aud_proc_info processor;
    struct aud_mod_impl impl = { .impl_num = 2 };
    struct aud_mod_inst source, sink;
    struct port output, input;
    struct jumper jumper = { 0 };
    struct seq_file graph = { 0 };
    char shared[16];
    INIT_LIST_HEAD(&processors);
    init_processor(&processor, 7);
    impl.processor = &processor;
    INIT_LIST_HEAD(&impl.instances);
    list_add(&impl.mods_list, &processor.mods);
    init_instance(&source, &impl, 0x07020001, "source_07020001");
    init_instance(&sink, &impl, 0x07020002, "sink_07020002");
    source.role = "role-not-in-dot";
    graph.private = &source;
    assert(status_show(&graph, NULL) == 0);
    assert(!strcmp(graph.text, "name=source_07020001\nuid=0x07020001\n"));
    graph = (struct seq_file){ 0 };
    init_port(&output, &source, 3, false);
    init_port(&input, &sink, 4, true);
    list_add(&input.down_peer_list, &output.down_peers);
    input.up_peer = &output;
    jumper.output_glue = &source;
    jumper.input_glue = &sink;
    jumper.shared = shared;
    jumper.shared_size = sizeof(shared);
    jumper.offset = 0x123;
    list_add(&jumper.jumper_list, &output.output_jumpers);

    assert(ml_show(&graph, NULL) == 0);
    assert(strstr(graph.text, "proc_id:7") && strstr(graph.text, "source_07020001"));
    assert(strstr(graph.text, "_out3 -> sink_07020002_") && strstr(graph.text, "_in4;"));
    assert(strstr(graph.text, "virt:") && strstr(graph.text, "size:16"));
    assert(strstr(graph.text, "offset:0x00000123") && strstr(graph.text, "style=dotted"));
    assert(strstr(graph.text, "source_07020001_out [label=\"jumper\"]"));
    assert(strstr(graph.text, "source_07020001_out -> shared_buf_"));
    assert(strstr(graph.text, "-> sink_07020002_in [style=dotted]"));
    assert(!strstr(graph.text, "role-not-in-dot") && !strstr(graph.text, "owner"));
    assert(input.up_peer == &output && output.down_peers.next == &input.down_peer_list);
    assert(!processors_lock.held && !processor.mods_lock.held && !impl.lock.held);
    assert(!source.lock.held && !sink.lock.held);

    graph = (struct seq_file){ 0 };
    jumper.dsp_shared = 0x12340000;
    assert(ml_show(&graph, NULL) == 0);
    assert(strstr(graph.text, "phys:") && !strstr(graph.text, "virt:"));
    graph = (struct seq_file){ 0 };
    jumper.shared = NULL;
    jumper.dsp_shared = 0;
    assert(ml_show(&graph, NULL) == 0);
    assert(!strstr(graph.text, "shared_buf_") && !strstr(graph.text, "label=\"jumper\""));
    assert(strstr(graph.text, "_out3 -> sink_07020002_"));
    INIT_LIST_HEAD(&processors);
}

static void test_block_connections(void)
{
    struct bm_list_block source = { .block_type = "renderer", .index = 0x1000 };
    struct bm_list_block sink = { .block_type = "mixer", .index = 0x1001 };
    struct bm_list_block_port output = { .owner = &source, .module_port = 3 };
    struct bm_list_block_port input = { .owner = &sink, .module_port = 4 };
    struct bm_list_connected_port *edge;
    struct seq_file graph = { 0 };
    INIT_LIST_HEAD(&_bm_info.list_block);
    INIT_LIST_HEAD(&source.list_port_input);
    INIT_LIST_HEAD(&source.list_port_output);
    INIT_LIST_HEAD(&sink.list_port_input);
    INIT_LIST_HEAD(&sink.list_port_output);
    INIT_LIST_HEAD(&output.list_connected_port);
    INIT_LIST_HEAD(&input.list_connected_port);
    list_add(&output.list, &source.list_port_output);
    list_add(&input.list, &sink.list_port_input);
    list_add(&sink.list, &_bm_info.list_block);
    list_add(&source.list, &_bm_info.list_block);
    assert(_bm_connect_port(&source, 99, &sink, 4) == -1);
    assert(_bm_connect_port(&source, 3, &sink, 99) == -1);
    fail_allocation = true;
    assert(_bm_connect_port(&source, 3, &sink, 4) == 0);
    assert(!output.flag && !input.flag);
    assert(output.list_connected_port.next == &output.list_connected_port);
    assert(input.list_connected_port.next == &input.list_connected_port);
    fail_allocation = false;
    allocations_until_failure = 1;
    assert(_bm_connect_port(&source, 3, &sink, 4) == 0);
    assert(output.flag == BLOCK_PORT_CONNECTED && !input.flag);
    assert(input.list_connected_port.next == &input.list_connected_port);
    edge = list_entry(output.list_connected_port.next, struct bm_list_connected_port, list);
    assert(edge->connected_port == &input);
    free(edge);
    INIT_LIST_HEAD(&output.list_connected_port);
    output.flag = 0;
    allocations_until_failure = -1;
    assert(_bm_connect_port(&source, 3, &sink, 4) == 0);
    assert(output.flag == BLOCK_PORT_CONNECTED && input.flag == BLOCK_PORT_CONNECTED);
    edge = list_entry(output.list_connected_port.next, struct bm_list_connected_port, list);
    assert(edge->connected_port == &input);
    assert(bm_dot(&graph, NULL) == 0);
    assert(strstr(graph.text, "renderer_1000_out3 -> mixer_1001_in4"));
    assert(strstr(graph.text, "label=\"proc_0\"") && !strstr(graph.text, "uid"));
    assert(!strstr(graph.text, "pipeline") && !strstr(graph.text, "owner"));
    assert(!_bm_info.bm_lock.held && output.flag == BLOCK_PORT_CONNECTED);
    free(edge);
    edge = list_entry(input.list_connected_port.next, struct bm_list_connected_port, list);
    assert(edge->connected_port == &output);
    free(edge);
    INIT_LIST_HEAD(&_bm_info.list_block);
}

int main(void)
{
    test_forwarding();
    test_roles();
    test_module_graph();
    test_block_connections();
    puts("kernel ownership: forwarding, role, graph and connection paths passed");
    return 0;
}