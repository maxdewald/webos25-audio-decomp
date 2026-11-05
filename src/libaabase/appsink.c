/*
 * libaabase: "appsink" module - exit point of a libaa module graph into the
 * application. Data/events reaching its input ports are handed to the callbacks set
 * with module_appsink_set_callback (lgaudiosink: lgao_appsink_cb).
 *
 * RECONSTRUCTION from orig/usr/lib/libaabase.so.1.0.0
 *   sha256 0d150201c8fb1dbda6c6769ed5d9254c21c98098fca31e2db33eb0b861f88806
 * Evidence:
 *   - aud_module_impl table at .data 0x1208c: name "appsink" (tail of "aud_appsink"
 *     @0x10f14), constructor 0x10d5c, destructor 0x10cf4, chain 0x10d34,
 *     event 0x10cfc, cmd 0x10cf8, instance_size 0x110, rest 0.
 *   - registered from init_array entry 0x10934: ModuleRegistry_Register(0x1208c).
 *   - logmm_module_aud_appsink at .data 0x12078.
 */
#include "aabase.h"

struct logmm_module logmm_module_aud_appsink = {
    .initial_level = 7,
    .mask = &logmm_dummy_mask,
    .name = "aud_appsink",
};

/* @0x10cf4 */
void appsink_destructor(struct aud_module *module)
{
}

/* @0x10cf8 */
int appsink_cmd(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    return aud_module_default_command(arg, cmd, param, len);
}

/* @0x10cfc: the application may swallow the event; otherwise default handling */
int appsink_event(struct aud_module *module, struct aud_port *port, struct aud_event *event)
{
    struct aud_appsink *sink = (struct aud_appsink *)module;
    module_appsink_callback *cb = sink->callback;

    if (cb && cb->handle_event &&
        cb->handle_event(sink->priv, port->number, event) == appsink_event_handled) {
        aud_object_put(&event->object);
        return 0;
    }
    return aud_module_default_event(module, port, event);
}

/* @0x10d34: the callback only peeks (no reference is taken for it); the data is then
 * pushed on to output 0 regardless of what handle_data returned. */
int appsink_chain(struct aud_port *port, struct aud_data *data)
{
    struct aud_appsink *sink = (struct aud_appsink *)port->module;

    if (sink->callback && sink->callback->handle_data)
        sink->callback->handle_data(sink->priv, port->number, data);
    return aud_module_push_data(&sink->module, 0, data);
}

/* @0x10d5c */
int appsink_constructor(struct aud_module *module, struct aimc *imc, const char *param)
{
    LOGI(aud_appsink, 0x5d, "created\n");
    return 0;
}

/* @0x10da4 */
int module_appsink_set_callback(struct aud_module *module, module_appsink_callback *cb,
                                void *priv)
{
    struct aud_appsink *sink = (struct aud_appsink *)module;

    sink->callback = cb;
    sink->priv = priv;
    return 0;
}

void appsink_register(void) __attribute__((constructor));

static struct aud_module_impl appsink_impl = {
    .name = "appsink",
    .constructor = appsink_constructor,
    .destructor = appsink_destructor,
    .chain = appsink_chain,
    .event = appsink_event,
    .cmd = appsink_cmd,
    .instance_size = sizeof(struct aud_appsink),
};

/* @0x10934: init_array constructor */
void appsink_register(void)
{
    ModuleRegistry_Register(&appsink_impl);
}
