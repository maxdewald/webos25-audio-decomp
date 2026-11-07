#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @match src/dsp0/bin.c (?s)typedef struct dsp0_bin_link \{.*?\} dsp0_bin;
// @match src/dsp0/bin.c (?s)typedef struct dsp0_bin_instance \{.*?\} dsp0_bin_instance;

typedef struct test_child {
    uint8_t unrecovered_000[0xc];
    const void *klass;
    void *queued;
    unsigned ordinal;
} test_child;

static dsp0_bin *active_bin;
static unsigned created;
static unsigned destroyed;
static unsigned ports_deleted;
static unsigned allocations;
static unsigned releases;
static int fail_record;
static int fail_child;
static int clearing;
static int child_class;

static void *record_malloc(size_t bytes)
{
    if (fail_record)
        return NULL;
    void *allocation = malloc(bytes);
    assert(allocation != NULL);
    ++allocations;
    return allocation;
}

static void record_free(void *allocation)
{
    assert(allocation != NULL);
    if (clearing && destroyed < created)
        assert(active_bin->pending.previous != &active_bin->pending);
    ++releases;
    free(allocation);
}

static void *aud_registry_create_instance(void *registry, const char *name, uint32_t uid)
{
    assert(registry == active_bin && uid == 0);
    if (fail_child)
        return NULL;
    test_child *child = calloc(1, sizeof(*child));
    assert(child != NULL);
    child->klass = &child_class;
    child->queued = malloc(4);
    assert(child->queued != NULL);
    memcpy(child->queued, "live", 4);
    child->ordinal = created++;
    return child;
}

static int aud_module_del_input_port(void *module, uint32_t number)
{
    test_child *child = module;
    assert(number == 7 && destroyed == 0);
    assert(memcmp(child->queued, "live", 4) == 0);
    ++ports_deleted;
    return -1;
}

static void aud_registry_destroy_instance(void *registry, void *module)
{
    assert(registry == active_bin && ports_deleted == 1);
    assert(active_bin->inputs.next == &active_bin->inputs);
    assert(active_bin->outputs.next == &active_bin->outputs);
    assert(active_bin->command_targets.next == &active_bin->command_targets);
    dsp0_bin_child *entry = (dsp0_bin_child *)active_bin->children.previous;
    assert(entry->module == module);
    test_child *child = module;
    assert(child->ordinal == destroyed++);
    assert(entry->index == child->ordinal);
    assert(memcmp(child->queued, "live", 4) == 0);
    dsp0_bin_command *pending = (dsp0_bin_command *)active_bin->pending.previous;
    assert(pending->command == 99 && memcmp(pending->payload, "keep", 4) == 0);
    free(child->queued);
    free(child);
}

// @extract src/dsp0/bin.c bin_initialize dsp0_bin_unlink
#define malloc record_malloc
#define free record_free
// @extract src/dsp0/bin.c aud_module_bin_add aud_module_bin_clear bin_cleanup
#undef malloc
#undef free

static void insert(dsp0_bin_link *head, dsp0_bin_link *link)
{
    link->next = head->next;
    link->previous = head;
    head->next->previous = link;
    head->next = link;
}

int main(void)
{
    dsp0_bin module = {0};
    active_bin = &module;
    module.registry = &module;
    assert(bin_initialize(&module) == 0);
    fail_record = 1;
    assert(aud_module_bin_add(&module, "decoder") == NULL);
    assert(created == 0 && allocations == 0);
    fail_record = 0;
    fail_child = 1;
    assert(aud_module_bin_add(&module, "decoder") == NULL);
    assert(created == 0 && allocations == releases);
    assert(module.children.next == &module.children);
    fail_child = 0;
    test_child *first = aud_module_bin_add(&module, "decoder");
    test_child *second = aud_module_bin_add(&module, "formatter");
    assert(first != NULL && second != NULL);
    assert(((dsp0_bin_child *)module.children.previous)->module == first);
    assert(((dsp0_bin_child *)module.children.next)->module == second);
    assert(((dsp0_bin_child *)module.children.next)->index == 1);
    uint32_t port_number = 7;
    dsp0_bin_route *input = record_malloc(sizeof(*input));
    *input = (dsp0_bin_route){ .module = first, .port = &port_number };
    insert(&module.inputs, &input->link);
    input = record_malloc(sizeof(*input));
    *input = (dsp0_bin_route){ .module = first };
    insert(&module.inputs, &input->link);
    dsp0_bin_route *output = record_malloc(sizeof(*output));
    *output = (dsp0_bin_route){ .module = second, .port = &port_number };
    insert(&module.outputs, &output->link);
    dsp0_bin_child *target = record_malloc(sizeof(*target));
    *target = (dsp0_bin_child){ .module = second };
    insert(&module.command_targets, &target->link);
    dsp0_bin_command *command = record_malloc(sizeof(*command));
    *command = (dsp0_bin_command){ .command = 99, .size = 4, .payload = record_malloc(4) };
    memcpy(command->payload, "keep", 4);
    insert(&module.pending, &command->link);
    clearing = 1;
    bin_cleanup(&module);
    assert(destroyed == 2 && allocations == releases);
    assert(module.children.next == &module.children);
    assert(module.pending.next == &module.pending);
    assert(aud_module_bin_clear(&module) == 0);
    assert(destroyed == 2 && allocations == releases);
    puts("DSP0 bin allocation/creation failure, ownership and reentrant cleanup visibility");
    return 0;
}