/* Usage: python3 tools/test_registry_lifetime.py.
 * Exercises source-extracted functions with real list/type definitions and
 * controlled dependencies. No threads, ARM execution or invented registry lock.
 */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/libaa/types.h"

#undef LOGE
#undef LOGI
#undef LOGT
#define LOGE(...) ((void)0)
#define LOGI(...) ((void)0)
#define LOGT(...) ((void)0)

static int alloc_call, fail_at, registration_result, unregister_result;
static int handler_calls, unregister_calls, barrier_calls, class_calls;
static int ctor_calls, dtor_calls, connect_calls, disconnect_calls;
static int connect_result, fail_output, fail_input, input_present, output_present;
static int sequence, unregistered_at, hold_freed_at, destructor_at, instance_freed_at;
static int exhaust_cursor_one;
static unsigned int registered_uid, unregistered_uid;
static void *registered_arg, *tracked_hold, *tracked_instance;
static struct aud_module *expected_dtor;
static struct aud_object_class_desc *failed_class;
static struct aud_port out_port, in_port;
static struct aud_module_impl impl;
static struct aimc *const imc = (struct aimc *)(uintptr_t)1;

static void *test_malloc(size_t size)
{
    if (++alloc_call == fail_at)
        return NULL;
    return malloc(size);
}

static void *test_calloc(size_t count, size_t size)
{
    if (++alloc_call == fail_at)
        return NULL;
    return calloc(count, size);
}

static void test_free(void *ptr)
{
    if (ptr && ptr == tracked_hold) {
        hold_freed_at = ++sequence;
        tracked_hold = NULL;
    }
    if (ptr && ptr == tracked_instance) {
        instance_freed_at = ++sequence;
        tracked_instance = NULL;
    }
    free(ptr);
}

int aimc_get_processor_id(struct aimc *a) { return 3; }
int aimc_register_command_handler(struct aimc *a, unsigned int uid,
        int (*cb)(void *, unsigned int, void *, unsigned int), void *arg)
{
    handler_calls++;
    registered_uid = uid;
    registered_arg = arg;
    return registration_result;
}
int aimc_unregister_command_handler(struct aimc *a, unsigned int uid)
{
    unregister_calls++;
    unregistered_uid = uid;
    unregistered_at = ++sequence;
    return unregister_result;
}
void aimc_wait_until_process_header(struct aimc *a) { barrier_calls++; }
void aimc_clear_named_event_on_uid(struct aimc *a, unsigned int uid) {}
void aud_link_root_init(struct aud_link_root *root) { link_init(&root->nodes); }
int aud_link_foreach_node(struct aud_link_root *root, void *arg,
                         int (*cb)(void *, void *)) { return 0; }
int disconnect_input_port_cb(void *arg, void *data) { return 0; }
int disconnect_output_port_cb(void *arg, void *data) { return 0; }
static inline void aud_module_del_output_ports(struct aud_module *m) {}
void aud_module_free_prop_value(struct aud_module *m, const struct aud_module_property *p) {}
void aud_object_put(struct aud_object *obj) { assert(!"unexpected module object ref"); }
char *audutil_strdup_printf(const char *fmt, ...)
{
    char text[96];
    va_list args;
    va_start(args, fmt);
    vsnprintf(text, sizeof(text), fmt, args);
    va_end(args);
    return strdup(text);
}
struct aud_object_class *aud_object_class_get(struct aud_object_class_desc *desc)
{
    class_calls++;
    return desc == failed_class ? NULL : desc->object_class;
}
struct aud_object_class *aud_object_class_gets(const char *name) { return &impl.parent; }
int aud_module_default_command(struct aud_module *m, unsigned int cmd, void *p,
                               unsigned int len) { return -18; }
struct aud_port *aud_module_get_input_port(struct aud_module *m, unsigned int number)
{
    return input_present ? &in_port : NULL;
}
struct aud_port *aud_module_get_output_port(struct aud_module *m, unsigned int number)
{
    return output_present ? &out_port : NULL;
}
struct aud_port *aud_module_new_output_port(struct aud_module *m, unsigned int number)
{
    if (fail_output)
        return NULL;
    output_present = 1;
    out_port.module = m;
    out_port.number = number;
    return &out_port;
}
struct aud_port *aud_module_new_input_port(struct aud_module *m, unsigned int number)
{
    if (fail_input)
        return NULL;
    input_present = 1;
    in_port.module = m;
    in_port.number = number;
    return &in_port;
}
int aud_module_connect(struct aud_port *out, struct aud_port *in)
{
    connect_calls++;
    return connect_result;
}
void aud_module_disconnect(struct aud_port *out, struct aud_port *in) { disconnect_calls++; }

#define malloc test_malloc
#define calloc test_calloc
#define free test_free
#include "registry_under_test.inc"
#undef malloc
#undef calloc
#undef free

static int constructor(struct aud_module *m, struct aimc *a, const char *param)
{
    ctor_calls++;
    assert(ModuleRegistry_SearchInstByUid(m->uid) == m);
    return -20; /* Actual constructor ignores this result. */
}

static void destructor(struct aud_module *m)
{
    dtor_calls++;
    destructor_at = ++sequence;
    if (expected_dtor) {
        assert(m == expected_dtor);
        assert(ModuleRegistry_SearchInstByUid(m->uid) == m);
        assert(link_empty(&internal_instances));
        assert(unregistered_uid == m->uid);
        assert(unregistered_at < destructor_at);
        if (hold_freed_at)
            assert(hold_freed_at < destructor_at);
        assert(!instance_freed_at);
    }
}

static void reset(void)
{
    assert(link_empty(&internal_instances));
    link_init(&module_implements);
    link_init(&mreg_classes);
    memset(&impl, 0, sizeof(impl));
    impl.name = "test";
    impl.instance_size = sizeof(struct aud_module);
    impl.constructor = constructor;
    impl.destructor = destructor;
    ModuleRegistry_Register(&impl);
    internal_index_last = 0;
    alloc_call = fail_at = handler_calls = unregister_calls = barrier_calls = class_calls = 0;
    ctor_calls = dtor_calls = connect_calls = disconnect_calls = 0;
    registration_result = unregister_result = connect_result = 0;
    fail_output = fail_input = input_present = output_present = 0;
    sequence = unregistered_at = hold_freed_at = destructor_at = instance_freed_at = 0;
    tracked_hold = tracked_instance = registered_arg = NULL;
    expected_dtor = NULL;
    failed_class = NULL;
}

static void test_owned_teardown_and_uid_reuse(void)
{
    struct registry r = { imc };
    struct aud_module *m, *replacement;
    unsigned int uid;
    reset();
    m = aud_registry_create_instance(imc, "test", 0);
    assert(m && ctor_calls == 1 && handler_calls == 1);
    uid = m->uid;
    assert(uid == 0x0300f001 && registered_arg == m);
    assert(ModuleRegistry_SearchInstByUid(uid) == m);
    assert(ModuleRegistry_SearchInstByName(m->name) == m);
    tracked_instance = expected_dtor = m;
    tracked_hold = internal_instances.next;
    unregister_result = -1;
    assert(ModuleRegistry_ProcessCmd(&r, REGCMD_DESTROY_MODULE, &uid, 4) == 0);
    assert(instance_freed_at > destructor_at && dtor_calls == 1 && !barrier_calls);
    assert(ModuleRegistry_SearchInstByUid(uid) == NULL);
    assert(ModuleRegistry_ProcessCmd(&r, REGCMD_DESTROY_MODULE, &uid, 4) == -3);
    expected_dtor = NULL;
    /* Wrap the rotating cursor: same UID is sufficient to select the next
     * generation. Never dereference or compare the freed pointer as a lease. */
    internal_index_last = 0xfff;
    replacement = aud_registry_create_instance(imc, "test", 0);
    assert(replacement && replacement->uid == uid);
    assert(ModuleRegistry_SearchInstByUid(uid) == replacement);
    assert(ModuleRegistry_ProcessCmd(&r, REGCMD_DESTROY_MODULE, &uid, 4) == 0);
    assert(dtor_calls == 2);
}

static void test_allocation_and_registration_failures(void)
{
    struct registry r = { imc };
    struct aud_module *m;
    struct aud_internal_inst *hold;
    reset();
    assert(!aud_registry_create_instance(imc, "missing", 0) && !handler_calls);
    fail_at = 1;
    assert(!aud_registry_create_instance(imc, "test", 0x03000011));
    assert(handler_calls == 1 && !registered_arg && !ctor_calls);
    reset();
    fail_at = 1; /* Module NULL is still recorded in an internal hold. */
    assert(!aud_registry_create_instance(imc, "test", 0));
    hold = link_first(struct aud_internal_inst, list, &internal_instances);
    assert(hold->iindex == 1 && !hold->module && handler_calls == 1);
    assert(!ModuleRegistry_SearchInstByUid(registered_uid));
    link_del(&hold->list);
    free(hold);
    reset();
    fail_at = 2; /* Module allocation succeeds; hold allocation fails. */
    m = aud_registry_create_instance(imc, "test", 0);
    assert(m && link_empty(&internal_instances) && handler_calls == 1);
    aud_registry_destroy_instance(imc, m); /* Unknown hold does not veto free. */
    assert(dtor_calls == 1);
    reset();
    registration_result = -20;
    assert(regcmd_create_module(&r, "test?x=1", 0x03000012) == 0);
    m = ModuleRegistry_SearchInstByUid(0x03000012);
    assert(m && ctor_calls == 1 && handler_calls == 1);
    aud_registry_destroy_instance(imc, m);
    reset();
    fail_at = 1; /* Parameter-name allocation, before constructor. */
    assert(regcmd_create_module(&r, "test?x=1", 7) == -1);
    assert(!handler_calls && !ctor_calls);
}

static void test_borrowed_classes_duplicates_and_hold_order(void)
{
    struct aud_object_class_desc first = {0}, second = {0};
    struct aud_module *a, *b;
    struct aud_internal_inst *hold;
    reset();
    a = aud_registry_create_instance(imc, "test", 0);
    b = aud_registry_create_instance(imc, "test", 0);
    hold = link_first(struct aud_internal_inst, list, &internal_instances);
    assert(hold->module == a && hold->iindex == 1); /* Shipped append order. */
    aud_registry_destroy_instance(imc, a);
    aud_registry_destroy_instance(imc, b);
    reset();
    a = aud_registry_create_instance(imc, "test", 42);
    b = aud_registry_create_instance(imc, "test", 42);
    assert(ModuleRegistry_SearchInstByUid(42) == b && !class_calls);
    aud_registry_destroy_instance(imc, b);
    assert(ModuleRegistry_SearchInstByUid(42) == a);
    aud_registry_destroy_instance(imc, a);
    reset();
    a = aud_registry_create_instance(imc, "test", 81);
    link_del(&impl.implement_link);
    first.object_class = NULL;
    second.object_class = &impl.parent;
    aud_mregistry_add_class(&second);
    aud_mregistry_add_class(&first);
    failed_class = &first;
    assert(!ModuleRegistry_SearchInstByUid(81) && class_calls == 1);
    link_del(&first.mreg_list);
    assert(ModuleRegistry_SearchInstByUid(81) == a);
    assert(ModuleRegistry_SearchInstByUid(81) == a); /* No get/put on module. */
    aud_registry_destroy_instance(imc, a);
}

static void test_internal_hold_mismatch_and_exhaustion(void)
{
    struct aud_module *a, *b;
    struct aud_internal_inst *holds;
    reset();
    a = aud_registry_create_instance(imc, "test", 0);
    b = aud_registry_create_instance(imc, "test", a->uid);
    /* Explicit duplicate shares UID but not the hold's pointer: destroying it
     * still removes the first generation's hold by index. No safe rejection. */
    aud_registry_destroy_instance(imc, b);
    assert(link_empty(&internal_instances));
    assert(ModuleRegistry_SearchInstByUid(a->uid) == a);
    aud_registry_destroy_instance(imc, a);
    reset();
    holds = calloc(0xfff, sizeof(*holds));
    assert(holds);
    for (int n = 0; n < 0xfff; n++) {
        holds[n].iindex = n + 1;
        link_add_tail(&internal_instances, &holds[n].list);
    }
    internal_index_last = exhaust_cursor_one ? 1 : 2;
    assert(!aud_registry_create_instance(imc, "test", 0));
    assert(internal_index_last == 2 && !handler_calls && !ctor_calls);
    for (int n = 0; n < 0xfff; n++)
        link_del(&holds[n].list);
    free(holds);
    reset();
    assert(get_empty_internal_index() == 1);
    assert(get_empty_internal_index() == 2);
    /* Holdless indices are not permanently reserved by the cursor. */
    internal_index_last = 0xfff;
    assert(get_empty_internal_index() == 1);
}

static void test_connector_transitions(void)
{
    struct aud_module *a, *b;
    unsigned int p[4] = { 71, 2, 72, 3 };
    reset();
    a = aud_registry_create_instance(imc, "test", 71);
    b = aud_registry_create_instance(imc, "test", 72);
    assert(Connector_Cmd(NULL, 0, NULL, 0) == -18);
    p[0] = 70;
    assert(Connector_Cmd(NULL, CONNCMD_CONNECT, p, sizeof(p)) == -1);
    p[0] = 71; p[2] = 70;
    assert(Connector_Cmd(NULL, CONNCMD_CONNECT, p, sizeof(p)) == -1);
    p[2] = 72;
    fail_output = 1;
    assert(Connector_Cmd(NULL, CONNCMD_CONNECT, p, sizeof(p)) == -20);
    fail_output = 0; fail_input = 1;
    assert(Connector_Cmd(NULL, CONNCMD_CONNECT, p, sizeof(p)) == -20);
    assert(output_present && !input_present && !connect_calls);
    fail_input = 0; connect_result = -20;
    assert(Connector_Cmd(NULL, CONNCMD_CONNECT, p, sizeof(p)) == 0);
    assert(connect_calls == 1); /* Actual low-level failure is ignored. */
    assert(Connector_Cmd(NULL, CONNCMD_CONNECT, p, sizeof(p)) == -24);
    assert(Connector_Cmd(NULL, CONNCMD_DISCONNECT, p, sizeof(p)) == 0);
    assert(disconnect_calls == 1);
    input_present = 0;
    assert(Connector_Cmd(NULL, CONNCMD_DISCONNECT, p, sizeof(p)) == -1);
    aud_registry_destroy_instance(imc, a);
    assert(Connector_Cmd(NULL, CONNCMD_DISCONNECT, p, sizeof(p)) == -1);
    aud_registry_destroy_instance(imc, b);
    assert(!barrier_calls);
    reset();
    registration_result = -1;
    struct aud_connector *c = Connector_Create(3, imc);
    assert(c && handler_calls == 1);
    Connector_Destroy(c);
    assert(unregister_calls == 1 && !barrier_calls);
    fail_at = alloc_call + 1;
    assert(!Connector_Create(3, imc));
    struct registry *r = malloc(sizeof(*r));
    r->imc = imc;
    ModuleRegistry_Cleanup(r);
    assert(barrier_calls == 1); /* Registry cleanup has a distinct barrier. */
}

int main(int argc, char **argv)
{
    exhaust_cursor_one = argc == 2 && !strcmp(argv[1], "--full-cursor-one");
    test_owned_teardown_and_uid_reuse();
    test_allocation_and_registration_failures();
    test_borrowed_classes_duplicates_and_hold_order();
    test_internal_hold_mismatch_and_exhaustion();
    test_connector_transitions();
    puts("registry lifetime: 5 source-extracted suites passed (ASan/UBSan)");
    return 0;
}
