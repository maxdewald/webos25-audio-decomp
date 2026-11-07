#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct decoderbin decoderbin;

typedef struct decoderbin_event_data {
    uint8_t unrecovered_000[0xc];
    uint32_t type;
    uint32_t flags;
} decoderbin_event_data;

typedef struct decoderbin_class {
    uint8_t unrecovered_000[0x60];
    void (*initialize)(decoderbin *module);
    void (*cleanup)(decoderbin *module);
    int (*event)(decoderbin *module, void *port, decoderbin_event_data *event);
} decoderbin_class;

struct decoderbin {
    uint8_t unrecovered_000[0xc];
    const decoderbin_class *klass;
    uint8_t unrecovered_010[0x108];
    char *codec_name;
    void *decoder;
    void *formatter;
    void *output_124;
    void *output_128;
    uint8_t connected;
    uint8_t dummy_codec;
    uint8_t previous_dummy_codec;
    uint8_t unrecovered_12f;
    uint32_t workerbin_uid;
    uint32_t unrecovered_134;
};

#if UINTPTR_MAX == UINT32_MAX
_Static_assert(offsetof(decoderbin, codec_name) == 0x118, "codec name offset");
_Static_assert(offsetof(decoderbin, connected) == 0x12c, "connected offset");
_Static_assert(offsetof(decoderbin_class, cleanup) == 0x64, "bin cleanup slot");
#endif

extern void aud_object_put(decoderbin_event_data *event);
extern const char *aud_event_value_get_str(decoderbin_event_data *event,
                                          const char *key, const char *fallback);
extern void add_decoder_module(decoderbin *module, const char *codec);
extern int aud_module_bin_set_input(decoderbin *module, unsigned port,
                                    void *child, unsigned child_port);
extern int aud_module_bin_set_output(decoderbin *module, unsigned port,
                                     void *child, unsigned child_port);
extern void aud_module_bin_add_cmd_propagate(decoderbin *module, void *child);
extern void aud_module_bin_propagate_pending(decoderbin *module);

/* @0x602568d4: Before connected, consume the event reference, even for a
 * NULL port. Once connected, START on a non-NULL port can select a codec,
 * then delegates to class+0x68 (bin_event). LAST and DISCONNECT only log here;
 * they neither clear connected nor destroy children. add_decoder_module's
 * codec replacement internals and outer dispatch lifetime remain external.
 * The START FLIX branch slot is checked, not codec arithmetic.
 */
int decoderbin_event(decoderbin *module, void *port, decoderbin_event_data *event)
{
    const decoderbin_class *klass = module->klass;
    if (!module->connected) {
        aud_object_put(event);
        return 0;
    }
    if (port != NULL && event->type == 1) {
        const char *codec = aud_event_value_get_str(event, "codec-desc", NULL);
        if (codec != NULL)
            add_decoder_module(module, codec);
    }
    return klass->event(module, port, event);
}

/* @0x602569e0: Delegate bin initialization, then reset only these cached
 * pointers/flags. output_124/output_128 and workerbin_uid are not set here;
 * their prior initialization is a caller prerequisite, not invented here.
 */
int decoderbin_constructor(decoderbin *module)
{
    if (module->klass->initialize != NULL)
        module->klass->initialize(module);
    module->codec_name = NULL;
    module->formatter = NULL;
    module->decoder = NULL;
    module->connected = 0;
    module->dummy_codec = 0;
    module->previous_dummy_codec = 0;
    return 0;
}

/* @0x60256a2c: Parent class+0x64 (bin_cleanup) runs while cached children
 * and codec_name are still present. Then free the name and clear child caches.
 * Children originate in aud_module_bin_add, not allocations owned here.
 * connected is NOT cleared. No quiescence, delay or synchronization is imposed
 * by this wrapper; post-destruction event safety cannot be inferred from it.
 */
void decoderbin_destructor(decoderbin *module)
{
    if (module->klass->cleanup != NULL)
        module->klass->cleanup(module);
    if (module->codec_name != NULL)
        free(module->codec_name);
    module->codec_name = NULL;
    module->decoder = NULL;
    module->formatter = NULL;
    module->output_124 = NULL;
    module->output_128 = NULL;
    module->dummy_codec = 0;
    module->previous_dummy_codec = 0;
    module->workerbin_uid = 0;
}

/* @0x60256cb8: A nonzero connect request requires decoder+0x11c; optional
 * formatter+0x120 becomes the input and a command target. Dummy mode assumes
 * both bypass outputs already exist (no local NULL checks). Wire routes, add
 * decoder command propagation, flush pending commands, THEN publish connected.
 * A zero request or an already connected bin is a no-op, not disconnection.
 * Child API results are ignored as in the scalar branches. Their implementation
 * and codec creation/error recovery are outside this focused reconstruction.
 */
int decoderbin_connect(decoderbin *module, const void *property, int connect)
{
    (void)property;
    if (module->connected || !connect)
        return 0;
    if (module->decoder == NULL)
        return -1;
    if (module->formatter == NULL) {
        aud_module_bin_set_input(module, 0, module->decoder, 0);
    } else {
        aud_module_bin_set_input(module, 0, module->formatter, 0);
        aud_module_bin_add_cmd_propagate(module, module->formatter);
    }
    if (!module->dummy_codec) {
        aud_module_bin_set_output(module, 0, module->decoder, 0);
        aud_module_bin_set_output(module, 1, module->decoder, 1);
    } else {
        aud_module_bin_set_output(module, 0xfe, module->decoder, 0);
        aud_module_bin_set_input(module, 0xfe, module->output_124, 0);
        aud_module_bin_set_input(module, 0xff, module->output_128, 0);
        aud_module_bin_set_output(module, 0, module->output_124, 0);
        aud_module_bin_set_output(module, 1, module->output_128, 0);
    }
    aud_module_bin_add_cmd_propagate(module, module->decoder);
    aud_module_bin_propagate_pending(module);
    module->connected = 1;
    return 0;
}