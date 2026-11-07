#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
    uint8_t unrecovered_010[0x58];
    void *registry;
    uint8_t unrecovered_06c[0xac];
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
_Static_assert(offsetof(decoderbin, registry) == 0x68, "registry offset");
_Static_assert(offsetof(decoderbin, connected) == 0x12c, "connected offset");
_Static_assert(offsetof(decoderbin_class, cleanup) == 0x64, "bin cleanup slot");
#endif

extern void aud_object_put(decoderbin_event_data *event);
extern const char *aud_event_value_get_str(decoderbin_event_data *event,
                                          const char *key, const char *fallback);
extern void *aud_module_bin_add(decoderbin *module, const char *name);
extern int aud_module_bin_connect(decoderbin *module, void *source, unsigned source_port,
                                   void *destination, unsigned destination_port);
extern int aimc_set_property(void *registry, uint32_t uid, const char *format, ...);
extern int aud_module_bin_set_input(decoderbin *module, unsigned port,
                                    void *child, unsigned child_port);
extern int aud_module_bin_set_output(decoderbin *module, unsigned port,
                                     void *child, unsigned child_port);
extern void aud_module_bin_add_cmd_propagate(decoderbin *module, void *child);
extern void aud_module_bin_propagate_pending(decoderbin *module);

typedef struct decoderbin_codec {
    const char *prefix;
    const char *decoder;
    const char *formatter;
} decoderbin_codec;

static const decoderbin_codec decoderbin_codecs[] = {
    {"AAC", "dec_heaac", "reform_ddt"},
    {"AC3", "dec_ddp", "reform_ddc"},
    {"EAC3", "dec_ddp", "reform_ddc"},
    {"AC4", "dec_ac4", "reform_ac4"},
    {"MP3", "dec_mp3", "reform_mp3"},
    {"DRA", "dec_dra", "reform_dra"},
    {"OPUS", "dec_opus", "reform_opus"},
    {"MPEG_H", "dec_mpegh", NULL},
    {"WMA", "dec_wma", NULL},
    {"VORBIS", "dec_ogg", NULL},
    {"AMR_NB", "dec_amrnb", NULL},
    {"AMR_WB", "dec_amrwb", NULL},
    {"FLAC", "dec_flac", NULL},
    {"MAT", "dec_mat", NULL},
    {"PCM", "dec_pcm", NULL},
    {"RA8", "dec_ra8", NULL},
    {"IAMF", "dec_iamf", "reform_iamf"},
    {NULL, NULL, NULL},
};

/* @0x60256610: Prefix-select children from DSP0's table at 0x60012b10.
 * Changed codec or same-codec dummy-mode change invokes parent cleanup BEFORE
 * connected=0. Caches/name are not cleared on cleanup or creation failure.
 * Formatter failure cleans the newly owned decoder through the parent; decoder
 * failure does not. Bypass creation/connect/property errors and strdup failure
 * are not propagated. Success publishes caches but neither reconnects nor sets
 * previous_dummy_codec. No local lock, callback barrier or grace delay appears.
 * A missing cached name with changed dummy mode reaches strncmp(NULL,...), as
 * in the image; caller validity and outer dispatch lifetime remain unrecovered.
 * Only FLIX branch slots are used; diagnostic logging is omitted.
 */
int add_decoder_module(decoderbin *module, const char *codec)
{
    const decoderbin_class *klass = module->klass;
    if (module->decoder != NULL) {
        if ((module->codec_name != NULL &&
             strncmp(module->codec_name, codec, strlen(codec)) != 0) ||
            (module->dummy_codec != module->previous_dummy_codec &&
             strncmp(module->codec_name, codec, strlen(codec)) == 0)) {
            if (klass->cleanup != NULL)
                klass->cleanup(module);
            module->connected = 0;
        } else {
            goto remember_codec;
        }
    }
    const decoderbin_codec *entry = decoderbin_codecs;
    while (entry->prefix != NULL &&
           strncmp(entry->prefix, codec, strlen(entry->prefix)) != 0)
        ++entry;
    if (entry->prefix == NULL || entry->decoder == NULL)
        return -1;
    void *decoder = aud_module_bin_add(module,
                                      module->dummy_codec ? "dummy" : entry->decoder);
    if (decoder == NULL)
        return -1;
    void *formatter = NULL;
    if (entry->formatter != NULL) {
        formatter = aud_module_bin_add(module, entry->formatter);
        if (formatter == NULL) {
            if (klass->cleanup != NULL)
                klass->cleanup(module);
            return -1;
        }
        aud_module_bin_connect(module, formatter, 0, decoder, 0);
    }
    if (module->dummy_codec) {
        if (module->workerbin_uid != 0) {
            uint32_t uid;
            memcpy(&uid, (const uint8_t *)decoder + 4, sizeof(uid));
            aimc_set_property(module->registry, uid, "workerbin-uid=%d",
                              module->workerbin_uid);
        }
        module->output_124 = aud_module_bin_add(module, "bypass");
        module->output_128 = aud_module_bin_add(module, "bypass");
    }
    module->formatter = formatter;
    module->decoder = decoder;
remember_codec:
    if (module->codec_name != NULL)
        free(module->codec_name);
    module->codec_name = strdup(codec);
    return 0;
}

/* @0x602568d4: Before connected, consume the event reference, even for a
 * NULL port. Once connected, START on a non-NULL port can select a codec,
 * then delegates to class+0x68 (bin_event). LAST and DISCONNECT only log here;
 * they neither clear connected nor destroy children. Replacement failure is
 * ignored and the parent event still runs, even after connected becomes zero.
 * Outer dispatch lifetime remains external.
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
 * Child API results are ignored as in the scalar branches. Route/command APIs
 * and their delivery synchronization remain external.
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