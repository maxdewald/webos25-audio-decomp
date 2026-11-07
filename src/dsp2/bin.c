/* DSP2 bin commands and lifecycle. Semantic host types, not firmware ABI overlays.
 * The export/symbol alias aud_module_bin_add at 60070ee8 is bin_cmd,
 * confirmed by its logged __func__; child creation is at 6007144c. */
#include <stdint.h>
#include <string.h>

struct aud_bin_link {
    struct aud_bin_link *previous, *next;
};
struct aud_bin_module {
    const char *name;
    int (*command)(struct aud_bin_module *, uint32_t, void *, unsigned);
    const void *class_descriptor;             /* module +c */
    struct aud_bin *output_bin;               /* binout +f0 */
    int output_number;                       /* binout +f4 */
};
struct aud_bin_target {
    struct aud_bin_link link;
    struct aud_bin_module *module;             /* node +8 */
    const char *name;                         /* node +c */
    unsigned index;                           /* node +10, same-class child index */
};
struct aud_bin_pending {
    struct aud_bin_link link;
    uint32_t command;                         /* node +8 */
    unsigned length;                          /* node +c */
    void *payload;                            /* node +10 */
};
struct aud_bin_port {
    int number;
};
struct aud_bin_mapping {
    struct aud_bin_link link;
    struct aud_bin_module *module;            /* holder +8 */
    struct aud_bin_port *port;                /* holder +c */
    int number;                              /* holder +10, bin port */
    int active;                              /* holder +14 */
};
struct aud_bin {
    struct aud_bin_link targets;              /* firmware +f8 */
    struct aud_bin_link pending;              /* firmware +110 */
    struct aud_bin_link children;             /* firmware +f0 */
    void *registry;                           /* module +68 */
    struct aud_bin_link inputs;               /* firmware +100 */
    struct aud_bin_link outputs;              /* firmware +108 */
};
extern void *_BufMonitor_RemoveBufferIdxLocal(unsigned);
extern void _BufMonitor_RemoveBufferIndex(void *);
extern int aud_module_cmd(struct aud_bin_module *, uint32_t, void *, unsigned);
extern int aud_module_default_command(struct aud_bin *, uint32_t, void *, unsigned);
extern struct aud_bin_module *aud_registry_create_instance(void *, const char *, unsigned);
extern void aud_registry_destroy_instance(void *, struct aud_bin_module *);
extern void aud_module_del_input_port(struct aud_bin_module *, int);
extern struct aud_bin_port *aud_module_new_input_port(struct aud_bin_module *, int);
extern struct aud_bin_port *aud_module_new_output_port(struct aud_bin_module *, int);
extern int aud_connector_connect(struct aud_bin_module *, int, struct aud_bin_module *, int);

/* @0x600710b8: class +60; initializes all five lists, not registry or subclass state. */
int bin_initialize(struct aud_bin *bin)
{
    bin->inputs.previous = bin->inputs.next = &bin->inputs;
    bin->children.previous = bin->children.next = &bin->children;
    bin->targets.previous = bin->targets.next = &bin->targets;
    bin->outputs.previous = bin->outputs.next = &bin->outputs;
    bin->pending.previous = bin->pending.next = &bin->pending;
    return 0;
}

/* @0x6007164c: removes input ports, holders and borrowed command targets before
 * destroying owned children; drops pending copies without replay. */
int aud_module_bin_clear(struct aud_bin *bin)
{
    while (bin->inputs.next != &bin->inputs) {
        struct aud_bin_mapping *mapping = (struct aud_bin_mapping *)bin->inputs.next;
        if (mapping->module != NULL && mapping->port != NULL)
            aud_module_del_input_port(mapping->module, mapping->port->number);
        mapping->link.previous->next = mapping->link.next;
        mapping->link.next->previous = mapping->link.previous;
        _BufMonitor_RemoveBufferIndex(mapping);
    }
    while (bin->outputs.next != &bin->outputs) {
        struct aud_bin_mapping *mapping = (struct aud_bin_mapping *)bin->outputs.next;
        mapping->link.previous->next = mapping->link.next;
        mapping->link.next->previous = mapping->link.previous;
        _BufMonitor_RemoveBufferIndex(mapping);
    }
    while (bin->targets.next != &bin->targets) {
        struct aud_bin_link *link = bin->targets.next;
        link->previous->next = link->next;
        link->next->previous = link->previous;
        _BufMonitor_RemoveBufferIndex(link);
    }
    while (bin->children.next != &bin->children) {
        struct aud_bin_target *child = (struct aud_bin_target *)bin->children.next;
        aud_registry_destroy_instance(bin->registry, child->module);
        child->link.previous->next = child->link.next;
        child->link.next->previous = child->link.previous;
        _BufMonitor_RemoveBufferIndex(child);
    }
    while (bin->pending.next != &bin->pending) {
        struct aud_bin_pending *entry = (struct aud_bin_pending *)bin->pending.next;
        if (entry->payload != NULL) _BufMonitor_RemoveBufferIndex(entry->payload);
        entry->link.previous->next = entry->link.next;
        entry->link.next->previous = entry->link.previous;
        _BufMonitor_RemoveBufferIndex(entry);
    }
    return 0;
}

/* @0x6007110c: class +64; does not reset a subclass's converter pointer. */
void bin_cleanup(struct aud_bin *bin)
{
    aud_module_bin_clear(bin);
}

/* @0x6007144c: owns a registry child; index counts existing same-class children.
 * Name is borrowed. Does not register propagation or replay pending commands. */
struct aud_bin_module *aud_module_bin_add(struct aud_bin *bin, const char *name)
{
    struct aud_bin_target *child = _BufMonitor_RemoveBufferIdxLocal(sizeof(*child));
    if (child == NULL) return NULL;
    memset(child, 0, sizeof(*child));
    struct aud_bin_module *module = aud_registry_create_instance(bin->registry, name, 0);
    if (module == NULL) {
        _BufMonitor_RemoveBufferIndex(child);
        return NULL;
    }
    child->module = module;
    child->name = name;
    for (struct aud_bin_link *link = bin->children.next;
         link != &bin->children; link = link->next) {
        struct aud_bin_target *existing = (struct aud_bin_target *)link;
        if (existing->module->class_descriptor == module->class_descriptor)
            ++child->index;
    }
    child->link.previous = bin->children.previous;
    child->link.next = &bin->children;
    bin->children.previous->next = &child->link;
    bin->children.previous = &child->link;
    return module;
}

/* @0x60071570: pointer identity; unlink owned child before registry destruction.
 * Does not remove mappings/propagation targets or pending commands. A same-name
 * replacement therefore cannot repair a stale propagation target by registering. */
int aud_module_bin_del(struct aud_bin *bin, struct aud_bin_module *module)
{
    for (struct aud_bin_link *link = bin->children.next;
         link != &bin->children; link = link->next) {
        struct aud_bin_target *child = (struct aud_bin_target *)link;
        if (child->module != module) continue;
        link->previous->next = link->next;
        link->next->previous = link->previous;
        aud_registry_destroy_instance(bin->registry, module);
        _BufMonitor_RemoveBufferIndex(child);
        return 0;
    }
    return -1;
}

/* @0x600718e0: -1 selects first unused mapped child port 0..254, across modules.
 * Explicit numbers/duplicates pass through; even a NULL new port keeps a holder. */
int aud_module_bin_set_input(struct aud_bin *bin, int number,
                             struct aud_bin_module *module, int child_port)
{
    if (module == NULL) return -1;
    if (child_port == -1) {
        for (int candidate = 0; candidate < 255; ++candidate) {
            struct aud_bin_link *link = bin->inputs.next;
            while (link != &bin->inputs) {
                struct aud_bin_mapping *mapping = (struct aud_bin_mapping *)link;
                if (mapping->port != NULL && mapping->port->number == candidate) break;
                link = link->next;
            }
            if (link == &bin->inputs) {
                child_port = candidate;
                break;
            }
        }
        if (child_port < 0) return -1;
    }
    struct aud_bin_mapping *mapping = _BufMonitor_RemoveBufferIdxLocal(sizeof(*mapping));
    if (mapping == NULL) return -1;
    memset(mapping, 0, sizeof(*mapping));
    mapping->port = aud_module_new_input_port(module, child_port);
    mapping->module = module;
    mapping->number = number;
    mapping->active = -1;
    mapping->link.previous = &bin->inputs;
    mapping->link.next = bin->inputs.next;
    bin->inputs.next->previous = &mapping->link;
    bin->inputs.next = &mapping->link;
    return 0;
}

/* @0x60071a9c: prepends holder, owns a new binout for every mapping, no dedup.
 * binout failure leaves holder installed; ignores connector failure. */
int aud_module_bin_set_output(struct aud_bin *bin, int number,
                              struct aud_bin_module *module, int child_port)
{
    if (module == NULL) return -1;
    struct aud_bin_mapping *mapping = _BufMonitor_RemoveBufferIdxLocal(sizeof(*mapping));
    if (mapping == NULL) return -1;
    memset(mapping, 0, sizeof(*mapping));
    mapping->port = aud_module_new_output_port(module, child_port);
    mapping->module = module;
    mapping->number = number;
    mapping->active = -1;
    mapping->link.previous = &bin->outputs;
    mapping->link.next = bin->outputs.next;
    bin->outputs.next->previous = &mapping->link;
    bin->outputs.next = &mapping->link;
    struct aud_bin_module *output = aud_module_bin_add(bin, "binout");
    if (output == NULL) return -1;
    output->output_bin = bin;
    output->output_number = number;
    aud_connector_connect(module, child_port, output, 0);
    return 0;
}

/* @0x60070ee8: no targets => copy ALL commands FIFO, including SET_PROP.
 * Live targets get aud_module_cmd, without caching; child errors are ignored.
 * Original payload is shared, in target order, then reaches the default handler.
 * Queue copy precedes that handler; allocation failure skips it. Return is its
 * result, even after child success. Live sends can overtake unreplayed entries.
 * Parent SET_PROP is therefore separate from child command propagation. */
int bin_cmd(struct aud_bin *bin, uint32_t command, void *payload, unsigned length)
{
    if (bin->targets.next == &bin->targets) {
        struct aud_bin_pending *entry = _BufMonitor_RemoveBufferIdxLocal(sizeof(*entry));
        if (entry == NULL) return -1;
        memset(entry, 0, sizeof(*entry));
        if (length != 0) {
            entry->payload = _BufMonitor_RemoveBufferIdxLocal(length);
            if (entry->payload == NULL) {
                _BufMonitor_RemoveBufferIndex(entry);
                return -1;
            }
            memset(entry->payload, 0, length);
            memcpy(entry->payload, payload, length);
        }
        entry->command = command;
        entry->length = length;
        entry->link.previous = bin->pending.previous;
        entry->link.next = &bin->pending;
        bin->pending.previous->next = &entry->link;
        bin->pending.previous = &entry->link;
    } else {
        for (struct aud_bin_link *link = bin->targets.next;
             link != &bin->targets; link = link->next) {
            struct aud_bin_target *target = (struct aud_bin_target *)link;
            aud_module_cmd(target->module, command, payload, length);
        }
    }
    return aud_module_default_command(bin, command, payload, length);
}

/* @0x60071290 */
int bin_command(struct aud_bin *bin, uint32_t command, void *payload, unsigned length)
{
    return bin_cmd(bin, command, payload, length);
}

/* @0x60071d40: duplicate NAME is success without adding/replacing a target.
 * Appends only; registering a child does not itself replay queued commands. */
int aud_module_bin_add_cmd_propagate(struct aud_bin *bin, struct aud_bin_module *module)
{
    if (module == NULL) return -1;
    for (struct aud_bin_link *link = bin->targets.next;
         link != &bin->targets; link = link->next) {
        struct aud_bin_target *target = (struct aud_bin_target *)link;
        if (strcmp(target->name, module->name) == 0) return 0;
    }
    struct aud_bin_target *target = _BufMonitor_RemoveBufferIdxLocal(sizeof(*target));
    if (target == NULL) return -1;
    memset(target, 0, sizeof(*target));
    target->module = module;
    target->name = module->name;
    target->link.previous = bin->targets.previous;
    target->link.next = &bin->targets;
    bin->targets.previous->next = &target->link;
    bin->targets.previous = &target->link;
    return 0;
}

/* @0x60071e7c: command-major FIFO replay, target insertion order; invokes the
 * child class command directly, unlike live aud_module_cmd. Shared copied
 * payload is freed after all children; failures do not stop replay. Drains
 * even with zero targets. This is NOT persistent configuration for a new child. */
int aud_module_bin_propagate_pending(struct aud_bin *bin)
{
    struct aud_bin_link *link = bin->pending.next;
    while (link != &bin->pending) {
        struct aud_bin_pending *entry = (struct aud_bin_pending *)link;
        struct aud_bin_link *next = link->next;
        for (struct aud_bin_link *child = bin->targets.next;
             child != &bin->targets; child = child->next) {
            struct aud_bin_module *module = ((struct aud_bin_target *)child)->module;
            module->command(module, entry->command, entry->payload, entry->length);
        }
        if (entry->payload != NULL) _BufMonitor_RemoveBufferIndex(entry->payload);
        link->previous->next = link->next;
        link->next->previous = link->previous;
        _BufMonitor_RemoveBufferIndex(entry);
        link = next;
    }
    return 0;
}