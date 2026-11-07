#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct dsp0_bin_link {
    struct dsp0_bin_link *next;
    struct dsp0_bin_link *previous;
} dsp0_bin_link;

typedef struct dsp0_bin_child {
    dsp0_bin_link link;
    void *module;
    const char *name;
    uint32_t index;
} dsp0_bin_child;

typedef struct dsp0_bin_route {
    dsp0_bin_link link;
    void *module;
    const uint32_t *port;
    uint32_t number;
    uint32_t active;
} dsp0_bin_route;

typedef struct dsp0_bin_command {
    dsp0_bin_link link;
    uint32_t command;
    uint32_t size;
    void *payload;
} dsp0_bin_command;

typedef struct dsp0_bin {
    uint8_t unrecovered_000[0x68];
    void *registry;
    uint8_t unrecovered_06c[0x84];
    dsp0_bin_link children;
    dsp0_bin_link command_targets;
    dsp0_bin_link inputs;
    dsp0_bin_link outputs;
    dsp0_bin_link pending;
} dsp0_bin;

typedef struct dsp0_bin_instance {
    uint8_t unrecovered_000[0xc];
    const void *klass;
} dsp0_bin_instance;

#if UINTPTR_MAX == UINT32_MAX
_Static_assert(offsetof(dsp0_bin, children) == 0xf0, "bin children offset");
_Static_assert(offsetof(dsp0_bin, pending) == 0x110, "bin pending offset");
_Static_assert(sizeof(dsp0_bin_child) == 0x14, "bin child size");
_Static_assert(offsetof(dsp0_bin_instance, klass) == 0xc, "child class offset");
#endif

extern void *aud_registry_create_instance(void *registry, const char *name, uint32_t uid);
extern void aud_registry_destroy_instance(void *registry, void *module);
extern int aud_module_del_input_port(void *module, uint32_t number);

/* @0x60209340: Initialize the five intrusive list heads, not module storage,
 * decoder caches or synchronization. Reinitializing a nonempty bin leaks it.
 */
int bin_initialize(dsp0_bin *module)
{
    module->inputs.next = module->inputs.previous = &module->inputs;
    module->children.next = module->children.previous = &module->children;
    module->command_targets.next = module->command_targets.previous = &module->command_targets;
    module->outputs.next = module->outputs.previous = &module->outputs;
    module->pending.next = module->pending.previous = &module->pending;
    return 0;
}

/* Intrusive unlink shared by the recovered clear loops. A NULL next link is
 * deliberately left alone, matching the scalar guard rather than repairing it.
 */
static void dsp0_bin_unlink(dsp0_bin_link *link)
{
    if (link->next != NULL) {
        link->next->previous = link->previous;
        link->previous->next = link->next;
    }
}

/* @0x602096d4: Allocate ownership before creating a registry child. Failure
 * frees only that ownership record. Index counts already owned same-class
 * children; insert at the head, so clear destroys oldest first. No bin lock or
 * callback exclusion is imposed here; registry creation remains external.
 */
void *aud_module_bin_add(dsp0_bin *module, const char *name)
{
    dsp0_bin_child *child = malloc(sizeof(*child));
    if (child == NULL)
        return NULL;
    *child = (dsp0_bin_child){0};
    void *instance = aud_registry_create_instance(module->registry, name, 0);
    if (instance == NULL) {
        free(child);
        return NULL;
    }
    child->module = instance;
    child->name = name;
    const void *instance_class = ((const dsp0_bin_instance *)instance)->klass;
    for (dsp0_bin_link *link = module->children.previous;
         link != &module->children; link = link->previous) {
        dsp0_bin_child *other = (dsp0_bin_child *)link;
        if (((const dsp0_bin_instance *)other->module)->klass == instance_class)
            ++child->index;
    }
    if (module->children.next != &child->link) {
        child->link.next = module->children.next;
        child->link.previous = &module->children;
        module->children.next->previous = &child->link;
        module->children.next = &child->link;
    }
    return instance;
}

/* @0x602098d4: Remove input ports/holders, output holders and command targets,
 * then registry-destroy each child BEFORE unlinking its ownership record, then
 * free pending payloads/records. Decoderbin caches are not touched. No local
 * lock, wait or callback barrier appears at this boundary; registry destruction
 * and outer delivery synchronization must not be inferred from this function.
 * Only validated FLIX branch slots are used; diagnostic logging is omitted.
 */
int aud_module_bin_clear(dsp0_bin *module)
{
    while (module->inputs.previous != &module->inputs) {
        dsp0_bin_route *route = (dsp0_bin_route *)module->inputs.previous;
        if (route->module != NULL && route->port != NULL)
            aud_module_del_input_port(route->module, *route->port);
        dsp0_bin_unlink(&route->link);
        free(route);
    }
    while (module->outputs.previous != &module->outputs) {
        dsp0_bin_link *link = module->outputs.previous;
        dsp0_bin_unlink(link);
        free(link);
    }
    while (module->command_targets.previous != &module->command_targets) {
        dsp0_bin_link *link = module->command_targets.previous;
        dsp0_bin_unlink(link);
        free(link);
    }
    while (module->children.previous != &module->children) {
        dsp0_bin_child *child = (dsp0_bin_child *)module->children.previous;
        aud_registry_destroy_instance(module->registry, child->module);
        dsp0_bin_unlink(&child->link);
        free(child);
    }
    while (module->pending.previous != &module->pending) {
        dsp0_bin_command *command = (dsp0_bin_command *)module->pending.previous;
        if (command->payload != NULL)
            free(command->payload);
        dsp0_bin_unlink(&command->link);
        free(command);
    }
    return 0;
}

/* @0x60209394: Parent decoderbin cleanup delegates directly to bin clear.
 * It does not clear decoder-specific caches/connected or add synchronization.
 */
void bin_cleanup(dsp0_bin *module)
{
    aud_module_bin_clear(module);
}