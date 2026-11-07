#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct eform_formatter {
    uintptr_t unrecovered_000;
    int32_t (*process)(struct eform_formatter *formatter, const void *input,
                       const void **output, int32_t *remaining, int find);
    uintptr_t unrecovered_008;
    void (*destroy)(struct eform_formatter *formatter);
} eform_formatter;

typedef struct eform_factory {
    const char *name;
    uint32_t value_004;
    eform_formatter *(*create)(void);
} eform_factory;

typedef struct eform_class {
    uint8_t unrecovered_000[0x50];
    const eform_factory *factory;
} eform_class;

typedef struct eform_adapter {
    uint8_t unrecovered_000[0x10];
    uint32_t written;
    uint32_t flushed;
    uint32_t unrecovered_018;
} eform_adapter;

typedef struct eform {
    uint8_t unrecovered_000[0xc];
    const eform_class *klass;
    uint8_t unrecovered_010[0xe8];
    eform_formatter *formatter;
    void *scratch;
    uint32_t state_100;
    uint32_t factory_value;
    uint32_t state_108;
    uint32_t state_10c;
    uint32_t state_110;
    uint32_t scratch_capacity;
    uint32_t input_bytes;
    uint32_t state_11c;
    uint32_t unrecovered_120;
    uint32_t meta_124;
    uint32_t meta_128;
    uint32_t state_12c;
    uint32_t unrecovered_130[2];
    uint32_t meta_138;
    uint32_t meta_13c;
    uint32_t total_input_bytes;
    uint32_t state_144;
    uint8_t bypass;
    uint8_t unrecovered_149[3];
    const eform_factory *factory;
    eform_adapter adapter;
} eform;

typedef struct eform_data {
    uint8_t unrecovered_000[0x10];
    uint32_t flags;
    uint32_t meta_014;
    uint32_t meta_018;
    uint32_t unrecovered_01c;
    uint32_t meta_020;
    uint32_t meta_024;
    uint8_t unrecovered_028[0xc];
    uint32_t size;
    uint8_t unrecovered_038[0x18];
    uint8_t payload[];
} eform_data;

typedef struct eform_event_data {
    uint8_t unrecovered_000[0xc];
    uint32_t type;
    uint32_t flags;
} eform_event_data;

typedef struct eform_port {
    uint32_t number;
    eform *module;
} eform_port;

#if UINTPTR_MAX == UINT32_MAX
_Static_assert(offsetof(eform, formatter) == 0xf8, "formatter offset");
_Static_assert(offsetof(eform, factory) == 0x14c, "factory offset");
_Static_assert(offsetof(eform, adapter) == 0x150, "adapter offset");
#endif
_Static_assert(offsetof(eform_data, payload) == 0x50, "data payload offset");

extern void aud_data_adapter_init(eform_adapter *adapter);
extern void aud_data_adapter_flush(eform_adapter *adapter, uint32_t bytes);
extern void aud_data_adapter_cleanup(eform_adapter *adapter);
extern void aud_data_adapter_push(eform_adapter *adapter, eform_data *data);
extern eform_data *aud_data_adapter_get_data(eform_adapter *adapter, uint32_t bytes);
extern void *aud_data_adapter_map(eform_adapter *adapter, uint32_t bytes);
extern void aud_data_adapter_unmap(eform_adapter *adapter, void *mapping);
extern eform_data *aud_data_new(uint32_t bytes);
extern int aud_module_push_data(eform *module, unsigned port, eform_data *data);
extern int aud_event_value_get_bool(eform_event_data *event, const char *key, int fallback);
extern int32_t aud_event_value_get_int(eform_event_data *event, const char *key, int32_t fallback);
extern void aud_event_value_set_int(eform_event_data *event, const char *key, int32_t value);
extern int aud_module_default_event(eform *module, eform_port *port, eform_event_data *event);
extern int aud_module_default_command(eform *module, uint32_t command, void *argument, uint32_t size);

/* @0x60241514: Positive signed requests drain at most adapter+0x10 minus
 * adapter+0x14 bytes. No formatter access, reference acquisition or lifetime
 * guard occurs here: the module and its embedded adapter must still be live.
 * Scalar branches independently checked; diagnostic logging is absent.
 */
void eform_flush(eform *module, uint32_t bytes)
{
    if ((int32_t)bytes > 0) {
        uint32_t available = module->adapter.written - module->adapter.flushed;
        aud_data_adapter_flush(&module->adapter,
                               available < bytes ? available : bytes);
    }
}

/* @0x60241540: A missing formatter selects adapter pass-through, not an
 * error. Otherwise map, invoke formatter+4, copy/push positive output, unmap,
 * then flush consumed input. Allocation failure unmaps WITHOUT flushing,
 * including input consumed by earlier successful iterations. Map/get-data
 * failures return -1. Validated FLIX branch slots only; codec implementation,
 * callback progress guarantees and diagnostic logging are not reconstructed.
 */
int32_t eform_process_new(eform *module)
{
    eform_adapter *adapter = &module->adapter;
    uint32_t available = adapter->written - adapter->flushed;
    eform_formatter *formatter = module->formatter;
    if (formatter == NULL) {
        eform_data *output = aud_data_adapter_get_data(adapter, available);
        if (output == NULL)
            return -1;
        output->flags = 1;
        output->meta_014 = module->meta_128;
        output->meta_018 = module->meta_124;
        output->meta_020 = module->meta_138;
        output->meta_024 = module->meta_13c;
        aud_module_push_data(module, 0, output);
        aud_data_adapter_flush(adapter, available);
        return 0;
    }
    uint8_t *mapping = aud_data_adapter_map(adapter, available);
    if (mapping == NULL)
        return -1;
    int32_t remaining = (int32_t)available;
    int32_t result = 0;
    while (remaining > 0) {
        const void *payload;
        result = formatter->process(formatter, mapping + available - remaining,
                                    &payload, &remaining, 1);
        if (result <= 0)
            break;
        eform_data *output = aud_data_new((uint32_t)result);
        if (output == NULL) {
            aud_data_adapter_unmap(adapter, mapping);
            return -1;
        }
        output->flags = 1;
        output->meta_014 = module->meta_128;
        output->meta_018 = module->meta_124;
        output->meta_020 = module->meta_138;
        output->meta_024 = module->meta_13c;
        memcpy(output->payload, payload, output->size);
        aud_module_push_data(module, 0, output);
    }
    aud_data_adapter_unmap(adapter, mapping);
    aud_data_adapter_flush(adapter, available - (uint32_t)remaining);
    return result;
}

/* @0x60241730: Bypass transfers the input directly. Otherwise retain it in
 * the adapter, snapshot metadata/counts, then process at least once; retry
 * negative results only while more than 256 bytes remain. No lifetime guard
 * or retry progress bound exists here. Codec internals remain external.
 */
int eform_chain(eform_port *port, eform_data *input)
{
    eform *module = port->module;
    if (module->bypass) {
        aud_module_push_data(module, 0, input);
        return 0;
    }
    aud_data_adapter_push(&module->adapter, input);
    module->meta_128 = input->meta_014;
    module->meta_124 = input->meta_018;
    module->meta_13c = input->meta_024;
    module->meta_138 = input->meta_020;
    module->input_bytes += input->size;
    module->total_input_bytes += input->size;
    int32_t result;
    do {
        result = eform_process_new(port->module);
    } while (result < 0 && module->adapter.written - module->adapter.flushed > 256);
    return 0;
}

/* @0x602417f8: Destroy formatter (module+0xf8), clean adapter (+0x150),
 * then free scratch (+0xfc). Does not free the enclosing module or clear
 * pointers, and is not repeatable. Formatter/adapter internals and outer
 * dispatch serialization are outside this reconstruction; no race is proven.
 */
void eform_destructor(eform *module)
{
    if (module->formatter != NULL)
        module->formatter->destroy(module->formatter);
    aud_data_adapter_cleanup(&module->adapter);
    free(module->scratch);
}

/* @0x6024181c: Type 7 flushes input_bytes only for flag bit 1. START (1)
 * resets counters and destroys/recreates formatter, but leaves adapter and
 * scratch intact. Failed create leaves NULL and still forwards the event.
 * Type 9 adds adapter availability to "passing-byte". All paths delegate ownership
 * to default_event; there is no local stop/destruction state or locking.
 */
int eform_event(eform *module, eform_port *port, eform_event_data *event)
{
    if (event->type == 7) {
        if ((event->flags & 2) != 0)
            eform_flush(module, module->input_bytes);
    } else if (event->type == 1) {
        module->bypass = aud_event_value_get_bool(event, "bypass_reform", 0);
        module->state_110 = 0;
        module->input_bytes = 0;
        module->meta_124 = 0;
        module->meta_13c = 0;
        module->meta_138 = 0;
        module->meta_128 = 0;
        module->state_108 = 0;
        module->state_10c = 0;
        module->state_100 = 0;
        module->state_12c = 0;
        module->total_input_bytes = 0;
        module->state_144 = 0;
        module->state_11c = 1;
        if (module->formatter != NULL)
            module->formatter->destroy(module->formatter);
        module->formatter = module->factory->create();
    } else if (event->type == 9) {
        int32_t size = aud_event_value_get_int(event, "passing-byte", 0);
        aud_event_value_set_int(event, "passing-byte", (int32_t)((uint32_t)size +
            module->adapter.written - module->adapter.flushed));
    }
    return aud_module_default_event(module, port, event);
}

/* @0x60241994: These three command IDs are acknowledged without state
 * changes. Other commands delegate; diagnostic logging elided.
 */
int eform_cmd(eform *module, uint32_t command, void *argument, uint32_t size)
{
    if (command == 0x10010000 || command == 0x10020000 || command == 0x1003002c)
        return 0;
    return aud_module_default_command(module, command, argument, size);
}

/* @0x602419e8: Factory is borrowed from class+0x50; formatter is created
 * only by START. Allocate 64 KiB scratch before initializing adapter. Failure
 * returns 16 with scratch/formatter NULL but adapter not initialized here;
 * outer allocation/failed-construction cleanup policy remains unrecovered.
 * Other untouched bytes rely on the enclosing object's initialization.
 */
int eform_constructor(eform *module)
{
    module->state_11c = 1;
    module->formatter = NULL;
    module->meta_124 = 0;
    module->meta_13c = 0;
    module->meta_138 = 0;
    module->meta_128 = 0;
    module->state_110 = 0;
    module->factory = module->klass->factory;
    module->factory_value = module->factory->value_004;
    module->scratch = malloc(0x10000);
    if (module->scratch == NULL)
        return 16;
    module->input_bytes = 0;
    module->state_108 = 0;
    module->state_10c = 0;
    module->state_100 = 0;
    module->scratch_capacity = 0x10000;
    aud_data_adapter_init(&module->adapter);
    return 0;
}