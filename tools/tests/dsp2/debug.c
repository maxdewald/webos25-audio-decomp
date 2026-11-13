#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

// @match src/dsp2/debug.c struct debug_\w+ \{[^}]*\};
// @match src/dsp2/debug.c typedef int \(\*debug_function\)[^;]*;
// @match src/dsp2/debug.c _Static_assert\([^;]*debug_adec[^;]*;
struct imc;

static char messages[16384];
static size_t used;
static unsigned control = 0x40, log_mask;
volatile uint32_t *aimc_debug_control = &control;
volatile uint32_t *aimc_debug_log_mask = &log_mask;
void *aimc_debug_log_ctx;
static unsigned mask, dispatched_uid, dispatched_command;
static int dispatches, function_calls, lookups;
static int function_argc;
static char **function_argv;
static void *locks[8];
static int depth, max_depth, lock_calls;

int logm_message(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int size = vsnprintf(messages + used, sizeof(messages) - used, format, args);
    va_end(args);
    assert(size >= 0 && (size_t)size < sizeof(messages) - used);
    used += (size_t)size;
    return size;
}
void *ModuleRegistry_SearchInstByUid(unsigned uid) { (void)uid; return NULL; }
char *aud_module_prop_str(void *module, const char *format)
{ (void)module; (void)format; abort(); }
void aud_free(void *memory) { free(memory); }
const char *ModuleList_GetModuleName(unsigned id)
{ assert(id == 42); lookups++; return "legacy"; }
void DebugPrint_SetMask(unsigned value) { mask = value; }
void logmm_printl(void *ctx, int level, const char *fn, int line, const char *fmt, ...)
{
    (void)ctx; (void)fmt;
    assert(level == 3 && !strcmp(fn, "debug_aimc_lstatus") && line == 0x4c2);
}
int aimc_dispatch_command(struct imc *imc, unsigned uid, unsigned command,
                          void *params, int size, unsigned source,
                          unsigned sequence, int response)
{
    assert(!imc && !params && !size && !source && !sequence && !response);
    assert(strstr(messages, "token: start\n") && !strstr(messages, "token: end\n"));
    dispatches++;
    dispatched_uid = uid;
    dispatched_command = command;
    return -99;
}
static int called(int argc, char **argv, int flags)
{
    assert(!flags);
    function_calls++;
    function_argc = argc;
    function_argv = argv;
    return -123;
}
debug_function symbol_get_add(const char *name)
{ return !strcmp(name, "known") ? called : NULL; }
void aud_mutex_lock(void *mutex)
{
    assert(depth < 8);
    locks[depth++] = mutex;
    if (depth > max_depth) max_depth = depth;
    lock_calls++;
}
void aud_mutex_unlock(void *mutex)
{ assert(depth && locks[--depth] == mutex); }

static void reset_messages(void) { used = 0; messages[0] = '\0'; }
static void root_init(struct debug_root *root)
{ root->nodes.next = root->nodes.prev = &root->nodes; }
static void append(struct debug_root *root, struct debug_link_node *node, void *data)
{
    node->data = data;
    node->link.prev = root->nodes.prev;
    node->link.next = &root->nodes;
    root->nodes.prev->next = &node->link;
    root->nodes.prev = &node->link;
}
static unsigned count(const char *needle)
{
    unsigned result = 0;
    for (const char *p = messages; (p = strstr(p, needle)); p += strlen(needle))
        result++;
    return result;
}

// @extract src/dsp2/debug.c audreg_print_property debug_aimc_lstatus debug_aimc_fcall debug_aimc2 debug_link_port debug_port_fields dump_inst dot_connection CommonBuffer_GetFreeSize CommonBuffer_GetAuCnt

static void buffer_counts(void)
{
    struct debug_adec_buf buf = {.max_size = 1024, .aui_count = 4};
    struct debug_adec_buf saved = buf;
    assert(CommonBuffer_GetFreeSize(&buf) == 1020);
    assert(CommonBuffer_GetAuCnt(&buf) == 0);
    assert(!memcmp(&buf, &saved, sizeof(buf)));

    buf.status.read_offset = 100;
    buf.status.write_offset = 300;
    buf.status.read_index = 1;
    buf.status.write_index = 2;
    saved = buf;
    assert(CommonBuffer_GetFreeSize(&buf) == 820);
    assert(CommonBuffer_GetAuCnt(&buf) == 1);
    assert(!memcmp(&buf, &saved, sizeof(buf)));

    buf.status.read_offset = 900;
    buf.status.write_offset = 100;
    buf.status.read_index = 3;
    buf.status.write_index = 1;
    assert(CommonBuffer_GetFreeSize(&buf) == 796);
    assert(CommonBuffer_GetAuCnt(&buf) == 2);
    buf.status.read_index = 2;
    assert(CommonBuffer_GetFreeSize(&buf) == 0);
    assert(CommonBuffer_GetAuCnt(&buf) == 3);

    buf.aui_count = 2;
    buf.status.read_index = 0;
    buf.status.write_index = 1;
    buf.status.read_offset = buf.status.write_offset = 0;
    assert(CommonBuffer_GetFreeSize(&buf) == 0);
    assert(CommonBuffer_GetAuCnt(&buf) == 1);
    buf.status.write_index = 0;
    buf.status.index_allocated = 1;
    assert(CommonBuffer_GetAuCnt(&buf) == 0);
    assert(CommonBuffer_GetFreeSize(&buf) == 1020);
    buf.aui_count = 1;
    assert(CommonBuffer_GetFreeSize(&buf) == 0);

    buf.aui_count = 4;
    buf.status.read_offset = 3;
    saved = buf;
    assert(CommonBuffer_GetFreeSize(&buf) == -1);
    assert(!memcmp(&buf, &saved, sizeof(buf)));
}

int main(void)
{
    buffer_counts();
    char *status[] = {"debug_aimc2", "token", "lstatus", "0x2a", NULL};
    assert(debug_aimc2(4, status) == 0);
    assert(dispatches == 1 && dispatched_uid == 42 && dispatched_command == 0x10070000);
    assert(mask == (control | 0x200000) && lookups == 0);
    assert(!strcmp(messages, "token: start\ntoken: end\n"));
    reset_messages();
    log_mask = 8;
    assert(debug_aimc2(4, status) == 0 && lookups == 1);
    reset_messages();
    char *fcall[] = {"debug_aimc2", "token", "fcall", "known", "extra", NULL};
    assert(debug_aimc2(5, fcall) == 0 && function_calls == 1);
    assert(function_argc == 2 && function_argv == fcall + 3);
    assert(!strcmp(messages, "token: start\ntoken: end\n"));
    reset_messages();
    fcall[3] = "missing";
    assert(debug_aimc2(5, fcall) == 0 && function_calls == 1);
    assert(strstr(messages, "no such a function. missing\n"));
    reset_messages();
    fcall[2] = "unknown";
    assert(debug_aimc2(5, fcall) == 0 && !used);
    fcall[2] = "fcall"; fcall[3] = NULL;
    assert(debug_aimc2(3, fcall) == 0 && !used);
    fcall[1] = NULL;
    assert(debug_aimc2(3, fcall) == 0 && !used);
    fcall[1] = "token"; fcall[2] = NULL;
    assert(debug_aimc2(2, fcall) == 0 && !used);

    struct debug_impl impl = {.name = "producer"}, sink_impl = {.name = "sink"};
    struct debug_module module = {.uid = 0x1234, .impl = &impl};
    struct debug_module sink = {.uid = 0x5678, .impl = &sink_impl};
    root_init(&module.inputs); root_init(&module.outputs);
    assert(dump_inst(&module) == 0 && depth == 0 && lock_calls == 2);
    assert(!strstr(messages, "  number=") && strstr(messages, "ouput ports..\n"));
    reset_messages();
    assert(dot_connection(&module) == 0 && !used && !depth);

    struct debug_port input = {.number = 7, .module = &module, .flag = 2};
    struct debug_port out = {.number = 3, .module = &module, .flag = 1,
                             .pending_obj_alloced = 5, .pending_obj_num = 2};
    struct debug_port idle = {.number = 4, .module = &module, .flag = 1};
    struct debug_port peer1 = {.number = 8, .module = &sink};
    struct debug_port peer2 = {.number = 9, .module = &sink};
    uint32_t data_object[] = {1, 0x64617461};
    uint32_t event_object[] = {1, 0x65766e74};
    void *pending[] = {data_object, event_object};
    out.pending_objs = pending;
    out.legacy_buf = event_object;
    input.up_peer = &out;
    root_init(&out.down_peers); root_init(&idle.down_peers);
    struct debug_link_node in_node, out_node, idle_node, peer_node1, peer_node2;
    append(&module.inputs, &in_node, &input);
    append(&module.outputs, &out_node, &out);
    append(&module.outputs, &idle_node, &idle);
    append(&out.down_peers, &peer_node1, &peer1);
    append(&out.down_peers, &peer_node2, &peer2);
    struct debug_port saved_out = out;
    assert(dump_inst(&module) == 0 && !depth && max_depth == 2);
    assert(!memcmp(&out, &saved_out, sizeof(out)));
    assert(pending[0] == data_object && pending[1] == event_object);
    assert(data_object[0] == 1 && event_object[0] == 1);
    assert(count("  number=") == 3 && count("    port=") == 2);
    assert(count("  up_peer=") == 1 && count("  down_peers..\n") == 2);
    assert(strstr(messages, "  pending_obj_alloced=5\n  pending_obj_num=2\n"));
    reset_messages();
    assert(dot_connection(&module) == 0 && !depth);
    assert(count(" -> ") == 2 && count("producer_00001234_") == 2);
    assert(count("sink_00005678_") == 2 && strstr(messages, "_in8 ") && strstr(messages, "_in9 "));
    puts("debug: cached occupancy, AU-slot exhaustion, wraparound, unchanged objects, discovery and locks: PASS");
    return 0;
}
