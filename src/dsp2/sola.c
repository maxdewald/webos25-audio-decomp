/* DSP2 SOLA wrapper/module, image d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd.
 * Evidence: 0x600a46b0..0x600a5388; literals 0x6009f7d0..0x6009f894;
 * descriptor/properties 0x60106900..0x60106a28. Aligned xtobjdump branch
 * slots and xtlist literals checked. Split Ghidra blocks belong to owners.
 * Semantic host model, not a 32-bit ABI overlay. Logging omitted.
 * Float/FLIX alpha computation remains an explicit dependency, not guessed. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct sola_handle {
    int algorithm;                            /* target +0 */
    void *engine;                             /* +4 */
    int channels, pcm_bytes;                  /* +8,+0xc */
    bool bypass;                              /* +0x10 */
    unsigned enabled;                         /* +0x14 */
};
struct sola_info { int channels; unsigned enabled; int algorithm, input_frames, output_frames, state; };
struct sola_adapter { uint32_t pushed, flushed; };
struct aud_data { int type; uint32_t index, timestamp; unsigned size; void *payload; };
struct aud_event { int type; unsigned flags; };
struct sola_module {
    struct sola_handle *handle;               /* +0xf0 */
    struct sola_adapter pcm, metadata;        /* +0xf4, +0x110 */
    int first_sent;                           /* +0x12c */
    int rate_in, rate_out, channels;           /* +0x130..138 */
    int old_in, old_out, old_channels;         /* +0x13c..144 */
    bool initialized;                         /* +0x148 */
    uint32_t alpha_bits;                      /* +0x14c, opaque float */
    int alpha_pending;                        /* +0x150 */
    bool seamless;                            /* +0x154 */
};
struct aud_port { int index; struct sola_module *module; };

extern void *_BufMonitor_RemoveBufferIdxLocal(unsigned);
extern void _BufMonitor_RemoveBufferIndex(void *);
extern void WSOLA2_create(void *), WSOLA2_destroy(void *);
extern int WSOLA2_init(void *), WSOLA2_RunMain(void *, const void *, void *);
extern int WSOLA2_SetChannel(void *, int), WSOLA2_SetPcmByte(void *, int);
extern int WSOLA2_SetBypass(void *, int), WSOLA2_SetAlpha(void *, uint32_t);
extern int WSOLA2_GetInputByte(void *), WSOLA2_GetOutputByte(void *), WSOLA2_GetStateFlag(void *);
extern void AuiLogger_AppendData(void *, const void *, unsigned);
extern void aud_data_adapter_init(struct sola_adapter *), aud_data_adapter_cleanup(struct sola_adapter *);
extern int aud_data_adapter_push(struct sola_adapter *, struct aud_data *);
extern void aud_data_adapter_flush(struct sola_adapter *, unsigned);
extern void *aud_data_adapter_map(struct sola_adapter *, unsigned);
extern void aud_data_adapter_unmap(struct sola_adapter *, void *);
extern struct aud_data *aud_data_adapter_get_data(struct sola_adapter *, unsigned);
extern struct aud_data *aud_data_new(unsigned);
extern void aud_object_put(void *);
extern int aud_module_push_data(struct sola_module *, int, struct aud_data *);
extern int aud_module_push_event(struct sola_module *, int, struct aud_event *);
extern int aud_module_default_event(struct sola_module *, struct aud_port *, struct aud_event *);
extern int aud_module_default_command(struct sola_module *, uint32_t, void *, unsigned);
extern int aud_event_value_get_int(struct aud_event *, const char *, int);
extern bool aud_event_value_get_bool(struct aud_event *, const char *, bool);
extern void aud_event_value_get_fraction(struct aud_event *, const char *, int *numerator, int *denominator);
extern struct aud_event *aud_event_newv_first(const char *, int, int, int);
extern char *audutil_strdup_printf(const char *, ...);
extern void ModuleRegistry_Register(const void *);
/* [FLIX] 0x600a4a71..4ac3, 4eed..4f48, 50ca..5103: computes/clamps
 * float alpha and, in chain/flush, compares it to the previous alpha to set
 * alpha_pending. Site distinguishes these three non-identical sequences. */
extern void sola_flix_alpha(struct sola_module *, uint32_t site);
/* [FLIX] 0x600a52ca: constructor float constant, stored at +0x14c. */
extern uint32_t sola_flix_initial_alpha(void);

/* @0x600a46b0: allocates even for unsupported algorithms; caller must destroy.
 * Firmware allocates 24 bytes; sizeof accommodates a host pointer in this model.
 * Like firmware, no allocation check or initialization of pcm_bytes/bypass. */
int SOLA_Create(struct sola_handle **out, int channels, int algorithm)
{
    struct sola_handle *s = _BufMonitor_RemoveBufferIdxLocal(sizeof(*s));
    *out = s;
    s->algorithm = algorithm;
    s->channels = channels < 1 || channels > 10 ? 2 : channels;
    if (algorithm == 2) {
        s->engine = _BufMonitor_RemoveBufferIdxLocal(0x78);
        WSOLA2_create(s->engine);
        WSOLA2_SetChannel(s->engine, s->channels);
    }
    s->enabled = 255;
    return algorithm == 2 ? 0 : -14;
}
/* @0x600a46f8 */
int SOLA_Destroy(struct sola_handle *s)
{
    int result = -14;
    if (s->algorithm == 2) {
        WSOLA2_destroy(s->engine);
        _BufMonitor_RemoveBufferIndex(s->engine);
        result = 0;
    }
    _BufMonitor_RemoveBufferIndex(s);
    return result;
}
static unsigned bypass_bytes(const struct sola_handle *s)
{
    return (uint32_t)s->channels * (uint32_t)s->pcm_bytes * 256u;
}
/* @0x600a4724: bypass copies 256 interleaved frames even for unknown algorithm. */
int SOLA_RunMain(struct sola_handle *s, const void *in, void *out)
{
    if (s->bypass) { AuiLogger_AppendData(out, in, bypass_bytes(s)); return 0; }
    return s->algorithm == 2 ? WSOLA2_RunMain(s->engine, in, out) : -14;
}
/* @0x600a4760 */
int SOLA_Init(struct sola_handle *s) { return s->algorithm == 2 ? WSOLA2_init(s->engine) : -14; }
/* @0x600a4778: target alpha argument travels in an AE/float register. */
int SOLA_SetAlpha(struct sola_handle *s, uint32_t bits)
{ return s->algorithm == 2 ? WSOLA2_SetAlpha(s->engine, bits) : -14; }
/* @0x600a4790: invalid width does not alter the cached width. */
int SOLA_SetPcmByte(struct sola_handle *s, int bytes)
{
    if (bytes != 2 && bytes != 4) return -8;
    int result = s->algorithm == 2 ? WSOLA2_SetPcmByte(s->engine, bytes) : -14;
    s->pcm_bytes = bytes;
    return result;
}
/* @0x600a47b8: normalizes local flag, forwards original integer to engine. */
int SOLA_SetBypass(struct sola_handle *s, int bypass)
{
    s->bypass = bypass != 0;
    return s->algorithm == 2 ? WSOLA2_SetBypass(s->engine, bypass) : -14;
}
/* @0x600a47dc */
int SOLA_GetInputByte(struct sola_handle *s)
{
    if (s->bypass) return (int)bypass_bytes(s);
    return s->algorithm == 2 ? WSOLA2_GetInputByte(s->engine) : 0;
}
/* @0x600a4808 */
int SOLA_GetOutputByte(struct sola_handle *s)
{
    if (s->bypass) return (int)bypass_bytes(s);
    return s->algorithm == 2 ? WSOLA2_GetOutputByte(s->engine) : 0;
}
/* @0x600a4834: stores a mask only; no call into WSOLA2. */
int SOLA_SetEnableChannel(struct sola_handle *s, unsigned mask) { s->enabled = mask; return 0; }
/* @0x600a4840: reports ENGINE sizes even if wrapper bypass is enabled.
 * No zero-channel/width guard exists in the original division. */
int SOLA_GetInfo(struct sola_handle *s, struct sola_info *info)
{
    if (!s || !info) return -15;
    info->channels = s->channels;
    info->enabled = s->enabled;
    info->algorithm = s->algorithm;
    if (s->algorithm == 2) {
        info->input_frames = WSOLA2_GetInputByte(s->engine) / (s->channels * s->pcm_bytes);
        info->output_frames = WSOLA2_GetOutputByte(s->engine) / (s->channels * s->pcm_bytes);
        info->state = WSOLA2_GetStateFlag(s->engine);
    } else { info->input_frames = 256; info->output_frames = 256; info->state = 0; }
    return 0;
}
/* @0x600a489c: inverted local bypass only, unlike SOLA_SetBypass. */
int SOLA_SetSeamlessMode(struct sola_handle *s, int mode) { s->bypass = mode == 0; return 0; }
/* @0x600a48b0: unlike Create, no channel range check. */
int SOLA_SetChannel(struct sola_handle *s, int channels)
{
    int result = s->algorithm == 2 ? WSOLA2_SetChannel(s->engine, channels) : 0;
    s->channels = channels;
    return result;
}
static unsigned queued(const struct sola_adapter *a) { return a->pushed - a->flushed; }
static bool configuration_matches(const struct sola_module *m)
{
    return m->old_in == m->rate_in && m->old_out == m->rate_out && m->old_channels == m->channels;
}

/* @0x600a48d0: equal rates without seamless mode are a direct pass-through,
 * including non-PCM payloads. On transition, drains queued PCM before input.
 * Otherwise accepts PCM(0); seamless metadata(2) replaces the previous metadata
 * adapter contents; other types are dropped. Every generated PCM block inherits
 * index/timestamp of the input call that triggers processing (no interpolation).
 * Metadata is pushed before PCM and is not flushed after get_data here.
 * WSOLA2 and the explicit alpha hook remain external DSP dependencies. */
int sola_chai(struct aud_port *port, struct aud_data *data)
{
    struct sola_module *m = port->module;
    if (!m->seamless && m->rate_in == m->rate_out) {
        m->old_in = m->rate_in; m->old_out = m->rate_out;
        m->old_channels = m->channels; m->alpha_bits = UINT32_C(0x3f800000);
        unsigned count = queued(&m->pcm);
        if ((int32_t)count > 0) {
            struct aud_data *tail = aud_data_adapter_get_data(&m->pcm, count);
            aud_data_adapter_flush(&m->pcm, count);
            aud_module_push_data(m, 0, tail);
        }
        aud_module_push_data(m, 0, data);
        return 0;
    }
    if (m->seamless && data->type == 2) {
        aud_data_adapter_flush(&m->metadata, queued(&m->metadata));
        aud_data_adapter_push(&m->metadata, data);
        return 0;
    }
    if (data->type != 0) { aud_object_put(data); return 0; }
    uint32_t index = data->index, timestamp = data->timestamp;
    aud_data_adapter_push(&m->pcm, data);
    if (!configuration_matches(m) || !m->initialized) {
        m->old_in = m->rate_in; m->old_out = m->rate_out; m->old_channels = m->channels;
        sola_flix_alpha(m, UINT32_C(0x600a4a71));
        if (!m->initialized) {
            SOLA_Init(m->handle);
            m->initialized = true;
            SOLA_SetBypass(m->handle, !m->seamless && m->rate_in == m->rate_out);
        }
        if (m->alpha_pending) {
            SOLA_SetAlpha(m->handle, m->alpha_bits);
            m->alpha_pending = 0;
        }
    }
    for (;;) {
        int needed = SOLA_GetInputByte(m->handle);
        if ((int32_t)queued(&m->pcm) < needed) return 0;
        void *input = aud_data_adapter_map(&m->pcm, (unsigned)needed);
        if (!input) return -1;
        struct aud_data *out = aud_data_new((unsigned)SOLA_GetOutputByte(m->handle));
        if (!out) { aud_data_adapter_unmap(&m->pcm, input); return -1; }
        int result = SOLA_RunMain(m->handle, input, out->payload);
        aud_data_adapter_unmap(&m->pcm, input);
        aud_data_adapter_flush(&m->pcm, (unsigned)needed);
        struct sola_info info;
        if (result != 0 || SOLA_GetInfo(m->handle, &info) != 0) {
            aud_object_put(out); return -1;
        }
        if (!m->first_sent) {
            aud_module_push_event(m, 0, aud_event_newv_first("channels", 0, m->channels, 0));
            m->first_sent = 1;
        }
        if (!out->size) { aud_object_put(out); continue; }
        struct aud_data *meta = m->seamless ? aud_data_adapter_get_data(&m->metadata, queued(&m->metadata)) : NULL;
        out->index = index; out->timestamp = timestamp;
        if (meta) { meta->type = 2; aud_module_push_data(m, 0, meta); }
        aud_module_push_data(m, 0, out);
    }
}
/* @0x600a4ea8: PCM-only mode skips an empty flush; seamless flushes BOTH
 * adapters even empty. Rate comparison is old_in != old_out, not current rates.
 * Does not reset initialized/first_sent or configuration caches. */
int sola_flush(struct sola_module *m)
{
    unsigned bytes = queued(&m->pcm);
    if (m->seamless || bytes) aud_data_adapter_flush(&m->pcm, bytes);
    if (m->seamless) aud_data_adapter_flush(&m->metadata, queued(&m->metadata));
    if (m->old_in != m->old_out) sola_flix_alpha(m, UINT32_C(0x600a4eed));
    if (m->seamless) {
        SOLA_Init(m->handle);
        if (m->alpha_pending) {
            SOLA_SetAlpha(m->handle, m->alpha_bits); m->alpha_pending = 0;
        }
    }
    return 0;
}
/* @0x600a4fd8: FIRST recreates algorithm 2, fixes width to FOUR bytes,
 * irrespective of input bit-depth properties. speed is read into +134/+130
 * (numerator/denominator); getter receives pointers in opposite order.
 * LAST permits a fresh generated FIRST; reset event 7 flushes only for flag 2.
 * FIRST/LAST special handling requires a non-NULL port; reset does not. */
int sola_event(struct sola_module *m, struct aud_port *port, struct aud_event *event)
{
    if (port && event->type == 1) {
        m->channels = aud_event_value_get_int(event, "channels", 2);
        m->seamless = aud_event_value_get_bool(event, "seamless-mode", false);
        aud_event_value_get_fraction(event, "speed", &m->rate_out, &m->rate_in);
        m->first_sent = 1;
        if (m->handle) SOLA_Destroy(m->handle);
        SOLA_Create(&m->handle, m->channels, 2);
        m->initialized = false;
        SOLA_SetEnableChannel(m->handle, 255);
        SOLA_SetPcmByte(m->handle, 4);
        sola_flix_alpha(m, UINT32_C(0x600a50ca));
    }
    if (port && event->type == 2) m->first_sent = 0;
    if (event->type == 7 && (event->flags & 2u)) sola_flush(m);
    return aud_module_default_event(m, port, event);
}
/* @0x600a5158: three lifecycle commands are acknowledged without side effects.
 * SET_RATE reads two words without validating length; applies lazily in chain. */
int sola_command(struct sola_module *m, uint32_t command, void *arg, unsigned size)
{
    if (command == UINT32_C(0x10010000) || command == UINT32_C(0x10020000) || command == UINT32_C(0x10000000)) return 0;
    if (command == UINT32_C(0x51000008)) {
        const int32_t *rate = arg; m->rate_in = rate[0]; m->rate_out = rate[1]; return 0;
    }
    return aud_module_default_command(m, command, arg, size);
}
/* @0x600a5288: rate_in/rate_out/first_sent are left to framework defaults. */
void sola_constructor(struct sola_module *m)
{
    aud_data_adapter_init(&m->pcm); aud_data_adapter_init(&m->metadata);
    m->old_channels = 0; m->old_out = 0; m->old_in = 0; m->channels = 2;
    m->initialized = false; m->handle = NULL; m->seamless = false;
    m->alpha_pending = 0; m->alpha_bits = sola_flix_initial_alpha();
}
/* @0x600a52f4 */
void sola_destructor(struct sola_module *m)
{
    if (m->handle) SOLA_Destroy(m->handle);
    aud_data_adapter_cleanup(&m->pcm); aud_data_adapter_cleanup(&m->metadata);
}
/* @0x600a5338: counts PCM bytes, never the metadata adapter. */
char *sola_gets_internal_buf_size(struct sola_module *m)
{ return audutil_strdup_printf("(int)%u", queued(&m->pcm)); }
/* @0x600a5354: property setter intentionally does nothing; FIRST owns mode. */
int sola_set_seamless_mode(struct sola_module *m, const void *property, const char *value)
{ (void)m; (void)property; (void)value; return 0; }
/* Property table @0x60106960, 40-byte target entries: rate-in (int,+130),
 * rate-out (int,+134), channels (int,+138), internal_buf_size (read-only,
 * custom getter), seamless-mode (int,+154, ignored custom setter). The latter
 * is a byte in code despite its table integer type. Descriptor size 0x158.
 * Framework owns generic property parsing, not additional SOLA functions. */
extern const void *sola_impl_descriptor;       /* target 0x60106900 */
/* @0x600a537c */
void SOLA_ModuleInit(void) { ModuleRegistry_Register(sola_impl_descriptor); }
