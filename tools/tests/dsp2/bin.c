#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// @match src/dsp2/bin.c (?s)struct aud_bin_link \{.*?extern int aud_module_default_command[^;]+;
// @match src/dsp2/bin.c (?m)^extern .*aud_registry_.*;
// @match src/dsp2/bin.c (?m)^extern .*aud_module_del_input_port.*;
// @match src/dsp2/bin.c (?m)^extern .*aud_module_new_.*;
// @match src/dsp2/bin.c (?m)^extern .*aud_connector_connect.*;
// @extract src/dsp2/bin.c bin_initialize aud_module_bin_clear bin_cleanup aud_module_bin_add aud_module_bin_del aud_module_bin_set_input aud_module_bin_set_output bin_cmd bin_command aud_module_bin_add_cmd_propagate aud_module_bin_propagate_pending

struct aud_module;
struct aud_port;
struct aud_data;
struct aud_event;
// @match src/dsp2/module_core.c (?s)struct aud_module_property \{.*?\n};
// @match src/dsp2/module_core.c (?s)struct aud_prop_list_node \{.*?\n};
// @match src/dsp2/module_core.c (?s)struct aud_module_impl \{.*?\n};
// @match src/dsp2/module_core.c (?s)struct aud_module \{.*?\n};
// @match src/dsp2/module_core.c #define ADEC_CMD_.*
static const struct aud_module_property aud_module_default_prop[] = {{0}};
int set_prop(struct aud_module *, const struct aud_module_property *, const char *, unsigned);
void *aud_malloc(unsigned);
void aud_free(void *);
int aud_module_get_prop(struct aud_module *, const char *);
struct aud_event *aud_event_deserialize(void *, unsigned);
int aud_module_event(struct aud_module *, struct aud_port *, struct aud_event *);
#define aud_module_default_command core_default_command
// @extract src/dsp2/module_core.c search_prop_set set_prop_list aud_module_set_prop aud_module_default_command
#undef aud_module_default_command

struct child_state {
    struct aud_bin_module module;
    unsigned value, order;
};
static unsigned sequence, live_dispatches, default_value, allocations;
static int fail_after = -1;
static void *original;
static struct aud_module *parent_properties;
static bool fail_factory;
static unsigned factory_calls, registry_children;
static int registry_token, converter_class, other_class, binout_class;
static struct aud_bin *clearing_bin;
static struct aud_bin *deleting_bin;
static unsigned destroyed_children;
struct test_port {
    struct aud_bin_port port;
    struct test_port *next;
};
struct registry_child {
    struct aud_bin_module module;
    unsigned setting;
    struct test_port *inputs, *outputs;
};
static bool fail_port;
static unsigned input_ports, output_ports, deleted_inputs, connected;
static int registry_command(struct aud_bin_module *module, uint32_t command,
                             void *payload, unsigned length)
{
    (void)command;
    if (length != 0) ((struct registry_child *)module)->setting = *(unsigned *)payload;
    return -19;
}

struct aud_bin_module *aud_registry_create_instance(void *registry, const char *name,
                                                    unsigned flags)
{
    assert(registry == &registry_token && flags == 0);
    ++factory_calls;
    if (fail_factory) return NULL;
    struct aud_bin_module *module = calloc(1, sizeof(struct registry_child));
    assert(module != NULL);
    module->name = name;
    module->command = registry_command;
    module->class_descriptor = strcmp(name, "other") == 0 ? &other_class : &converter_class;
    if (strcmp(name, "binout") == 0) module->class_descriptor = &binout_class;
    ++registry_children;
    return module;
}
void aud_registry_destroy_instance(void *registry, struct aud_bin_module *module)
{
    assert(registry == &registry_token && registry_children > 0);
    if (deleting_bin != NULL)
        assert(deleting_bin->children.next == &deleting_bin->children);
    if (clearing_bin != NULL) {
        assert(clearing_bin->inputs.next == &clearing_bin->inputs);
        assert(clearing_bin->outputs.next == &clearing_bin->outputs);
        assert(clearing_bin->targets.next == &clearing_bin->targets);
        assert(clearing_bin->pending.next != &clearing_bin->pending);
    }
    ++destroyed_children;
    --registry_children;
    struct registry_child *child = (struct registry_child *)module;
    assert(child->inputs == NULL);
    while (child->outputs != NULL) {
        struct test_port *port = child->outputs;
        child->outputs = port->next;
        free(port);
        --output_ports;
    }
    free(module);
}
void aud_module_del_input_port(struct aud_bin_module *module, int number)
{
    struct registry_child *child = (struct registry_child *)module;
    struct test_port **slot = &child->inputs;
    while (*slot != NULL && (*slot)->port.number != number) slot = &(*slot)->next;
    assert(*slot != NULL);
    struct test_port *port = *slot;
    *slot = port->next;
    free(port);
    --input_ports;
    ++deleted_inputs;
}
struct aud_bin_port *aud_module_new_input_port(struct aud_bin_module *module, int number)
{
    if (fail_port) return NULL;
    struct registry_child *child = (struct registry_child *)module;
    struct test_port *port = calloc(1, sizeof(*port));
    assert(port != NULL);
    port->port.number = number;
    port->next = child->inputs;
    child->inputs = port;
    ++input_ports;
    return &port->port;
}
struct aud_bin_port *aud_module_new_output_port(struct aud_bin_module *module, int number)
{
    if (fail_port) return NULL;
    struct registry_child *child = (struct registry_child *)module;
    struct test_port *port = calloc(1, sizeof(*port));
    assert(port != NULL);
    port->port.number = number;
    port->next = child->outputs;
    child->outputs = port;
    ++output_ports;
    return &port->port;
}
int aud_connector_connect(struct aud_bin_module *module, int number,
                           struct aud_bin_module *output, int output_number)
{
    assert(module != output && output_number == 0 && !strcmp(output->name, "binout"));
    assert(output->output_bin != NULL);
    struct aud_bin_mapping *mapping = (struct aud_bin_mapping *)output->output_bin->outputs.next;
    assert(mapping->module == module && mapping->number == output->output_number);
    if (mapping->port != NULL) assert(mapping->port->number == number);
    ++connected;
    return -29;
}

void *_BufMonitor_RemoveBufferIdxLocal(unsigned bytes)
{
    if (fail_after == 0) return NULL;
    if (fail_after > 0) --fail_after;
    ++allocations;
    return malloc(bytes);
}
void _BufMonitor_RemoveBufferIndex(void *memory)
{
    assert(allocations > 0);
    --allocations;
    free(memory);
}
static int child_command(struct aud_bin_module *module, uint32_t command,
                         void *payload, unsigned length)
{
    struct child_state *child = (struct child_state *)module;
    child->order = ++sequence;
    if (length != 0) {
        child->value = child->value * 10 + *(unsigned *)payload;
        if (command == 2) ++*(unsigned *)payload;
    }
    return -19;
}
int aud_module_cmd(struct aud_bin_module *module, uint32_t command,
                   void *payload, unsigned length)
{
    ++live_dispatches;
    return module->command(module, command, payload, length);
}
int aud_module_default_command(struct aud_bin *bin, uint32_t command,
                               void *payload, unsigned length)
{
    (void)bin; (void)command;
    if (parent_properties != NULL)
        return core_default_command(parent_properties, command, payload, length);
    assert(payload == original);
    if (length != 0) default_value = *(unsigned *)payload;
    return -7;
}
static void initialize(struct aud_bin *bin)
{
    bin->registry = &registry_token;
    assert(bin_initialize(bin) == 0 && bin->registry == &registry_token);
}
static void free_targets(struct aud_bin *bin)
{
    struct aud_bin_link *link = bin->targets.next;
    while (link != &bin->targets) {
        struct aud_bin_link *next = link->next;
        _BufMonitor_RemoveBufferIndex(link);
        link = next;
    }
    bin->targets.previous = bin->targets.next = &bin->targets;
}
void *aud_malloc(unsigned bytes) { return malloc(bytes); }
void aud_free(void *memory) { free(memory); }
int aud_module_get_prop(struct aud_module *module, const char *name)
{
    (void)module; (void)name; assert(!"unexpected getter"); return -1;
}
struct aud_event *aud_event_deserialize(void *payload, unsigned length)
{
    (void)payload; (void)length; assert(!"unexpected event"); return NULL;
}
int aud_module_event(struct aud_module *module, struct aud_port *port, struct aud_event *event)
{
    (void)module; (void)port; (void)event; assert(!"unexpected event"); return -1;
}
int set_prop(struct aud_module *module, const struct aud_module_property *property,
              const char *value, unsigned size)
{
    assert(property->set != NULL);
    return property->set(module, property, value, size);
}
static unsigned property_value;
static int child_property(struct aud_module *module, const struct aud_module_property *property,
                           const char *value, unsigned size)
{
    (void)module; (void)property; (void)size;
    property_value = (unsigned)strtoul(value, NULL, 0);
    return 0;
}
static const struct aud_module_property child_properties[] = {
    {"out-bit-per-sample", 2, 0, 0, child_property}, {0}
};
static const struct aud_module_impl child_impl = {.props = child_properties};
static const struct aud_module_impl parent_impl = {0};
static struct aud_module child_properties_module = {.impl = &child_impl};
static int property_command(struct aud_bin_module *module, uint32_t command,
                             void *payload, unsigned length)
{
    (void)module;
    return core_default_command(&child_properties_module, command, payload, length);
}
static void property_tests(void)
{
    struct aud_bin bin;
    initialize(&bin);
    struct aud_module parent = {.impl = &parent_impl};
    struct aud_bin_module child = {"pcmcvt", property_command};
    parent_properties = &parent;
    struct { unsigned length; char text[32]; } payload = {0};
    strcpy(payload.text, "out-bit-per-sample=16");
    payload.length = (unsigned)strlen(payload.text);
    assert(aud_module_set_prop(&parent, payload.text, payload.length) == 0);
    assert(property_value == 0);
    assert(bin_command(&bin, ADEC_CMD_SET_PROP, &payload, sizeof(payload)) == 0);
    assert(property_value == 0);
    assert(aud_module_bin_add_cmd_propagate(&bin, &child) == 0);
    assert(property_value == 0);
    assert(aud_module_bin_propagate_pending(&bin) == 0 && property_value == 16);
    strcpy(payload.text, "out-bit-per-sample=24");
    assert(bin_command(&bin, ADEC_CMD_SET_PROP, &payload, sizeof(payload)) == 0);
    assert(property_value == 24);
    strcpy(payload.text, "out-bit-per-sample=32");
    assert(bin_command(&bin, ADEC_CMD_SET_PROP_SINGLE, &payload, sizeof(payload)) == 0);
    assert(property_value == 32);
    assert(aud_module_set_prop(&parent, "out-bit-per-sample=16", 21) == 0);
    assert(property_value == 32);
    assert(bin_command(&bin, 0x50020020, &payload, sizeof(payload)) == -1);
    free_targets(&bin);
    parent_properties = NULL;
    assert(allocations == 0);
}
static void child_creation_tests(void)
{
    struct aud_bin bin;
    initialize(&bin);
    fail_after = 0;
    unsigned before = factory_calls;
    assert(aud_module_bin_add(&bin, "pcmcvt") == NULL && factory_calls == before);
    fail_after = -1;
    fail_factory = true;
    assert(aud_module_bin_add(&bin, "pcmcvt") == NULL);
    assert(allocations == 0 && bin.children.next == &bin.children);
    fail_factory = false;
    unsigned payload = 7;
    original = &payload;
    assert(bin_command(&bin, 1, &payload, sizeof(payload)) == -7);
    char name[] = "pcmcvt";
    struct aud_bin_module *first = aud_module_bin_add(&bin, name);
    struct aud_bin_module *other = aud_module_bin_add(&bin, "other");
    struct aud_bin_module *second = aud_module_bin_add(&bin, "alias");
    assert(first != NULL && other != NULL && second != NULL);
    struct aud_bin_target *entry = (struct aud_bin_target *)bin.children.next;
    assert(entry->module == first && entry->name == name && entry->index == 0);
    entry = (struct aud_bin_target *)entry->link.next;
    assert(entry->module == other && entry->index == 0);
    entry = (struct aud_bin_target *)entry->link.next;
    assert(entry->module == second && entry->index == 1);
    assert(bin.targets.next == &bin.targets && bin.pending.next != &bin.pending);
    assert(registry_children == 3 && allocations == 5);
    struct child_state borrowed = {{"borrowed", child_command}};
    assert(aud_module_bin_add_cmd_propagate(&bin, &borrowed.module) == 0);
    assert(aud_module_bin_add_cmd_propagate(&bin, first) == 0);
    clearing_bin = &bin;
    before = destroyed_children;
    bin_cleanup(&bin);
    clearing_bin = NULL;
    assert(destroyed_children == before + 3 && borrowed.value == 0);
    assert(bin.children.next == &bin.children && bin.children.previous == &bin.children);
    assert(bin.targets.next == &bin.targets && bin.targets.previous == &bin.targets);
    assert(bin.pending.next == &bin.pending && bin.pending.previous == &bin.pending);
    assert(aud_module_bin_clear(&bin) == 0 && destroyed_children == before + 3);
    assert(allocations == 0 && registry_children == 0);
}
static void mapping_tests(void)
{
    struct aud_bin bin;
    initialize(&bin);
    struct aud_bin_module *first = aud_module_bin_add(&bin, "pcmcvt");
    struct aud_bin_module *second = aud_module_bin_add(&bin, "other");
    assert(first != NULL && second != NULL);
    assert(aud_module_bin_set_input(&bin, 0, NULL, 0) == -1);
    assert(aud_module_bin_set_output(&bin, 0, NULL, 0) == -1);
    fail_after = 0;
    assert(aud_module_bin_set_input(&bin, 0, first, 0) == -1);
    assert(aud_module_bin_set_output(&bin, 0, first, 0) == -1);
    fail_after = -1;
    assert(input_ports == 0 && output_ports == 0);
    assert(aud_module_bin_set_input(&bin, 4, first, 0) == 0);
    assert(aud_module_bin_set_input(&bin, 4, first, 0) == 0);
    struct aud_bin_mapping *mapping = (struct aud_bin_mapping *)bin.inputs.next;
    assert(mapping->number == 4 && mapping->active == -1);
    assert(((struct aud_bin_mapping *)mapping->link.next)->number == 4);
    assert(aud_module_bin_set_input(&bin, -7, second, -1) == 0);
    mapping = (struct aud_bin_mapping *)bin.inputs.next;
    assert(mapping->number == -7 && mapping->port->number == 1);
    assert(aud_module_bin_set_input(&bin, 8, second, -2) == 0);
    assert(((struct aud_bin_mapping *)bin.inputs.next)->port->number == -2);
    fail_port = true;
    assert(aud_module_bin_set_input(&bin, 9, first, 99) == 0);
    assert(((struct aud_bin_mapping *)bin.inputs.next)->port == NULL);
    assert(aud_module_bin_set_output(&bin, 9, first, 99) == 0);
    assert(((struct aud_bin_mapping *)bin.outputs.next)->port == NULL);
    fail_port = false;
    unsigned connections = connected;
    assert(aud_module_bin_set_output(&bin, 4, first, 0) == 0);
    assert(aud_module_bin_set_output(&bin, 4, first, 0) == 0);
    assert(connected == connections + 2 && registry_children == 5);
    mapping = (struct aud_bin_mapping *)bin.outputs.next;
    assert(mapping->number == 4 && mapping->active == -1);
    assert(((struct aud_bin_mapping *)mapping->link.next)->number == 4);
    fail_after = 1;
    assert(aud_module_bin_set_output(&bin, 10, first, 2) == -1);
    fail_after = -1;
    assert(((struct aud_bin_mapping *)bin.outputs.next)->number == 10);
    fail_factory = true;
    assert(aud_module_bin_set_output(&bin, 11, first, 3) == -1);
    fail_factory = false;
    assert(((struct aud_bin_mapping *)bin.outputs.next)->number == 11);
    assert(connected == connections + 2 && registry_children == 5);
    unsigned deletions = deleted_inputs;
    bin_cleanup(&bin);
    assert(deleted_inputs == deletions + 4);
    assert(input_ports == 0 && output_ports == 0 && allocations == 0 && registry_children == 0);

    initialize(&bin);
    first = aud_module_bin_add(&bin, "pcmcvt");
    assert(first != NULL);
    for (int number = 0; number < 255; ++number)
        assert(aud_module_bin_set_input(&bin, number, first, number) == 0);
    unsigned before = allocations;
    assert(aud_module_bin_set_input(&bin, 255, first, -1) == -1 && allocations == before);
    bin_cleanup(&bin);
    assert(input_ports == 0 && allocations == 0 && registry_children == 0);
}
static void replacement_tests(void)
{
    struct aud_bin bin;
    initialize(&bin);
    unsigned setting = 123;
    original = &setting;
    assert(bin_command(&bin, 1, &setting, sizeof(setting)) == -7);
    struct aud_bin_module *old = aud_module_bin_add(&bin, "pcmcvt");
    assert(old != NULL && aud_module_bin_set_input(&bin, 0, old, 0) == 0);
    assert(aud_module_bin_set_output(&bin, 0, old, 0) == 0);
    assert(aud_module_bin_add_cmd_propagate(&bin, old) == 0);
    assert(aud_module_bin_propagate_pending(&bin) == 0);
    assert(((struct registry_child *)old)->setting == 123);
    setting = 456;
    assert(bin_command(&bin, 1, &setting, sizeof(setting)) == -7);
    assert(((struct registry_child *)old)->setting == 456);
    unsigned before = destroyed_children;
    bin_cleanup(&bin);
    assert(destroyed_children == before + 2 && allocations == 0);
    struct aud_bin_module *replacement = aud_module_bin_add(&bin, "mssrc");
    assert(replacement != NULL);
    assert(aud_module_bin_set_input(&bin, 0, replacement, 0) == 0);
    assert(aud_module_bin_set_output(&bin, 0, replacement, 0) == 0);
    assert(aud_module_bin_add_cmd_propagate(&bin, replacement) == 0);
    assert(aud_module_bin_propagate_pending(&bin) == 0);
    assert(((struct registry_child *)replacement)->setting == 0);
    bin_cleanup(&bin);
    assert(allocations == 0 && registry_children == 0 && input_ports == 0 && output_ports == 0);
}
static void interleaved_dispatch_tests(void)
{
    struct aud_bin bin;
    initialize(&bin);
    struct child_state first = {{"first", child_command}}, second = {{"second", child_command}};
    unsigned payload = 1;
    original = &payload;
    unsigned dispatches = live_dispatches;
    unsigned before = sequence;
    assert(bin_command(&bin, 1, &payload, sizeof(payload)) == -7);
    assert(aud_module_bin_add_cmd_propagate(&bin, &first.module) == 0);
    assert(aud_module_bin_add_cmd_propagate(&bin, &second.module) == 0);
    payload = 2;
    assert(bin_command(&bin, 2, &payload, sizeof(payload)) == -7);
    assert(first.value == 2 && second.value == 3 && payload == 4);
    assert(default_value == 4 && live_dispatches == dispatches + 2);
    assert(first.order == before + 1 && second.order == before + 2);
    assert(bin.pending.next != &bin.pending);
    assert(aud_module_bin_propagate_pending(&bin) == 0);
    assert(first.value == 21 && second.value == 31 && payload == 4);
    assert(first.order == before + 3 && second.order == before + 4);
    assert(bin.pending.next == &bin.pending && allocations == 2);
    assert(live_dispatches == dispatches + 2);
    bin_cleanup(&bin);
    assert(allocations == 0);
}
static void child_deletion_tests(void)
{
    struct aud_bin bin;
    initialize(&bin);
    unsigned payload = 7;
    original = &payload;
    assert(bin_command(&bin, 1, &payload, sizeof(payload)) == -7);
    struct aud_bin_module *child = aud_module_bin_add(&bin, "pcmcvt");
    assert(child != NULL);
    assert(aud_module_bin_add_cmd_propagate(&bin, child) == 0);
    struct aud_bin_link *target = bin.targets.next;
    struct aud_bin_link *pending = bin.pending.next;
    struct aud_bin_module unknown = {"pcmcvt", registry_command};
    unsigned before = destroyed_children;
    assert(aud_module_bin_del(&bin, &unknown) == -1);
    assert(destroyed_children == before && registry_children == 1);
    deleting_bin = &bin;
    assert(aud_module_bin_del(&bin, child) == 0);
    deleting_bin = NULL;
    assert(destroyed_children == before + 1 && registry_children == 0);
    assert(bin.children.previous == &bin.children);
    assert(bin.targets.next == target && bin.pending.next == pending);
    assert(allocations == 3);
    fail_factory = true;
    assert(aud_module_bin_add(&bin, "pcmcvt") == NULL);
    fail_factory = false;
    assert(bin.children.next == &bin.children && registry_children == 0);
    assert(bin.targets.next == target && bin.pending.next == pending && allocations == 3);
    struct aud_bin_module *replacement = aud_module_bin_add(&bin, "pcmcvt");
    assert(replacement != NULL);
    assert(aud_module_bin_add_cmd_propagate(&bin, replacement) == 0);
    assert(bin.targets.next == target && target->next == &bin.targets);
    assert(bin.pending.next == pending);
    bin_cleanup(&bin);
    assert(destroyed_children == before + 2 && allocations == 0 && registry_children == 0);
}
int main(void)
{
    struct aud_bin bin;
    initialize(&bin);
    unsigned payload = 3;
    original = &payload;
    assert(bin_command(&bin, 1, &payload, sizeof(payload)) == -7);
    payload = 4;
    assert(bin_command(&bin, 2, &payload, sizeof(payload)) == -7);
    payload = 9;
    struct child_state first = {{"first", child_command}}, second = {{"second", child_command}};
    assert(aud_module_bin_add_cmd_propagate(&bin, NULL) == -1);
    assert(aud_module_bin_add_cmd_propagate(&bin, &first.module) == 0);
    assert(aud_module_bin_add_cmd_propagate(&bin, &second.module) == 0);
    struct aud_bin_module duplicate = {"first", child_command};
    assert(aud_module_bin_add_cmd_propagate(&bin, &duplicate) == 0);
    assert(aud_module_bin_propagate_pending(&bin) == 0);
    assert(first.value == 34 && second.value == 35 && payload == 9);
    assert(first.order == 3 && second.order == 4 && live_dispatches == 0);
    assert(allocations == 2 && bin.pending.next == &bin.pending);
    assert(aud_module_bin_propagate_pending(&bin) == 0 && sequence == 4);
    assert(bin_command(&bin, 1, &payload, sizeof(payload)) == -7);
    assert(first.value == 349 && second.value == 359 && live_dispatches == 2);
    assert(default_value == 9 && allocations == 2);
    free_targets(&bin);
    initialize(&bin);
    assert(aud_module_bin_propagate_pending(&bin) == 0);
    assert(first.value == 349 && second.value == 359);
    assert(bin_command(&bin, 1, &payload, sizeof(payload)) == -7);
    assert(aud_module_bin_propagate_pending(&bin) == 0 && allocations == 0);
    fail_after = 0;
    assert(aud_module_bin_add_cmd_propagate(&bin, &first.module) == -1);
    assert(bin_command(&bin, 1, &payload, sizeof(payload)) == -1);
    fail_after = 1;
    assert(bin_command(&bin, 1, &payload, sizeof(payload)) == -1);
    assert(allocations == 0 && bin.pending.next == &bin.pending);
    fail_after = -1;
    original = NULL;
    assert(bin_command(&bin, 0x10010000, NULL, 0) == -7);
    assert(aud_module_bin_propagate_pending(&bin) == 0 && allocations == 0);
    property_tests();
    child_creation_tests();
    mapping_tests();
    replacement_tests();
    interleaved_dispatch_tests();
    child_deletion_tests();
    puts("PASS bin: FIFO replay, live overtaking/shared mutation, ignored child errors, child ownership/failure/deletion, stale same-name target, pending discard, duplicate/auto ports, binout failure, replacement settings loss");
    return 0;
}