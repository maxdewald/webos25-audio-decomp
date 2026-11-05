/*
 * libaabase: "appsrc" module - entry point of application data into a libaa module
 * graph. lgaudiosink creates it by name through libaa (registry instance "appsrc"),
 * connects its output 0 to the renderer/bypass head and feeds it with
 * module_appsrc_push_data / module_appsrc_push_event.
 *
 * RECONSTRUCTION from orig/usr/lib/libaabase.so.1.0.0
 *   sha256 0d150201c8fb1dbda6c6769ed5d9254c21c98098fca31e2db33eb0b861f88806
 * Evidence:
 *   - aud_module_impl table at .data 0x12018 (R_ARM_RELATIVE relocs): name "appsrc"
 *     (tail of "aud_appsrc" @0x10eb0), constructor 0x10b58, destructor 0x10af8,
 *     chain 0x10ba0, event 0x10b00, cmd 0x10afc, instance_size 0x110, rest 0.
 *   - registered from init_array entry 0x10928: ModuleRegistry_Register(0x12018).
 *   - logmm_module_aud_appsrc at .data 0x12004 = { {0,0}, 7, &logmm_dummy_mask, "aud_appsrc" }.
 *   Ghidra created no functions for most static callbacks (only FUN_00010ba0); they are
 *   tagged anyway and show up as "not in symbol table" in progress.py.
 */
#include "aabase.h"

struct logmm_module logmm_module_aud_appsrc = {
    .initial_level = 7,
    .mask = &logmm_dummy_mask,
    .name = "aud_appsrc",
};

/* @0x10af8: nothing to release */
void appsrc_destructor(struct aud_module *module)
{
}

/* @0x10afc */
int appsrc_cmd(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    return aud_module_default_command(arg, cmd, param, len);
}

/* @0x10b00: only logs; everything else is libaa's default handling */
int appsrc_event(struct aud_module *module, struct aud_port *port, struct aud_event *event)
{
    LOGI(aud_appsrc, 0x56, "event %d\n", event->type);
    return aud_module_default_event(module, port, event);
}

/* @0x10b58 */
int appsrc_constructor(struct aud_module *module, struct aimc *imc, const char *param)
{
    LOGI(aud_appsrc, 0x5d, "created\n");
    return 0;
}

/* Inlined into appsrc_chain and module_appsrc_push_data (keeps its own __func__). */
static inline void push_data_to_downstream(struct aud_appsrc *src, struct aud_data *data)
{
    src->input_count += data->size;
    LOGD(aud_appsrc, 0x31, "chain size %d, input_count:%lld\n", data->size, src->input_count);
    aud_module_push_data(&src->module, 0, data);
}

/* @0x10ba0: data arriving on an input port is passed straight through to output 0 */
int appsrc_chain(struct aud_port *port, struct aud_data *data)
{
    push_data_to_downstream((struct aud_appsrc *)port->module, data);
    return 0;
}

/* @0x10c0c: consumes the caller's reference on data; always returns 0 */
int module_appsrc_push_data(struct aud_module *module, struct aud_data *data, bool sync)
{
    push_data_to_downstream((struct aud_appsrc *)module, data);
    if (sync)
        aud_module_sync(module, 0);
    return 0;
}

/* @0x10c98: consumes the caller's reference on event; always returns 0 */
int module_appsrc_push_event(struct aud_module *module, struct aud_event *event)
{
    LOGI(aud_appsrc, 0x42, "event %d\n", event->type);
    aud_module_push_event(module, 0, event);
    aud_module_sync(module, 0);
    return 0;
}

void appsrc_register(void) __attribute__((constructor));

static struct aud_module_impl appsrc_impl = {
    .name = "appsrc",
    .constructor = appsrc_constructor,
    .destructor = appsrc_destructor,
    .chain = appsrc_chain,
    .event = appsrc_event,
    .cmd = appsrc_cmd,
    .instance_size = sizeof(struct aud_appsrc),
};

/* @0x10928: init_array constructor */
void appsrc_register(void)
{
    ModuleRegistry_Register(&appsrc_impl);
}
