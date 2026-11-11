/* Focused reconstruction of the DSP0 HDMI FIRST producer.
 * LG OLED42C27LA, Rockhopper 10.3.1-3006 / firmware 33.31.68.01.
 * audio_a0_dsp0.bin SHA256:
 * 337a6330397f0891a4cf9b9bccc06b36d902b1dce18e110507bc98f2a5f25b58.
 * Original obtained from LG's matching USB package:
 * https://gscs-b2c.lge.com/downloadFile?fileId=tFYxTpYW4V60IWzmdBRjIg
 *
 * Evidence: Ghidra 12.2 and tools/xtobjdump.sh on the hash-matched image.
 * FLIX branch slots at 0x60227ed4/edc/ee4 were read with the validated
 * Intel overlay; Ghidra treats them as opaque userops and misses the
 * rate-selection edges. No unknown SIMD/float slots are guessed here.
 * Surrounding packet processing and hardware access are not reconstructed.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct dsp0_hdmi {
    uint8_t unrecovered_000[0x158];
    uint32_t hdmi_port;                      /* +0x158 */
    uint8_t unrecovered_15c[0x0c];
    uint32_t sampling_freq;                  /* +0x168 */
    uint8_t unrecovered_16c[0x68];
    int32_t data_drop_mode;                  /* +0x1d4 */
    int32_t codec;                           /* +0x1d8, FIRST label selector */
    uint8_t unrecovered_1dc[0x18];
    uint32_t irq_count;                      /* +0x1f4 */
} dsp0_hdmi;
_Static_assert(offsetof(dsp0_hdmi, sampling_freq) == 0x168, "HDMI rate");
_Static_assert(offsetof(dsp0_hdmi, codec) == 0x1d8, "HDMI codec");

typedef struct dsp0_hdmi_event dsp0_hdmi_event;
extern dsp0_hdmi_event *aud_event_newv_first(const char *key, ...);
extern void aud_event_value_set_int(dsp0_hdmi_event *event, const char *key, int value);
extern void aud_event_value_set_bool(dsp0_hdmi_event *event, const char *key, bool value);
extern void aud_module_push_event(dsp0_hdmi *module, int port, dsp0_hdmi_event *event);

/* The implementation's recovered property table at 0x60551e50 has 14
 * 40-byte records. codec is a generic numeric property (+0x1d8), parsed
 * with _Stoul(base 0); literal "MAT" is not an enum spelling, use numeric 4.
 * mode is independently at +0x1a0 with custom setter hdmi_set_mode.
 * No separate label property occurs in this table (inherited/dynamic
 * properties outside this descriptor have not been ruled out).
 *
 * hdmi_set_mode @0x60229600 sets the HBR byte +0x194 only for mode 4 or 7;
 * output-thread dispatch @0x60229d73 selects feed/parser processing for
 * modes 1/2/4/5/6/7/8, otherwise the PCM writer. codec 4 does NOT select
 * MAT conversion: the core @0x60228b18 special-cases codecs 1/2, and the
 * feed/start sizing paths special-case codec 2, not MAT 4 or DTS-HD 6.
 * Thus numeric codec=4 with HBR mode=4 is a plausible bypass-announcement
 * experiment, NOT a proven transparent transport. Changing codec with
 * mode=0 still uses PCM processing. The complete parser/IRQ/start lifecycle
 * is outside this focused reconstruction; no on-device test was performed.
 */
typedef struct dsp0_hdmi_property {
    uint32_t name_address;   /* Xtensa pointer, log-only here */
    int32_t type;            /* 1 = bool, 2 = int, affects logging only */
    uint32_t offset;         /* property value already written at module+offset */
} dsp0_hdmi_property;

/* @0x6022a210: Notification after the property writer has stored the value.
 * Apart from type-specific logging, only offset 0x1d8 (codec) acts: signed
 * codec >= 8 is reset to PCM. It neither restarts HDMI nor emits FIRST.
 * The signed test preserves 7 and negative values, although those are unsafe
 * for the seven-element FIRST tables. Do not broaden the valid stream range
 * merely because this callback fails to reject them.
 */
int hdmi_prop_updated(dsp0_hdmi *module, const dsp0_hdmi_property *property)
{
    if (property->offset == 0x1d8 && module->codec >= 8)
        module->codec = 0;
    return 0;
}

/* @0x60227e78: Builds a FIRST for an HDMI output port; does not reframe data.
 * Valid stream codec selectors are 0..6. The original has no bounds check:
 * its channel/reform tables at 0x60005820/0x60005860 have seven entries,
 * as does the label array copied from literals at 0x601f8530..0x601f8548.
 * hdmi_prop_updated's weaker signed <8 check does NOT make selector 7 or
 * a negative selector safe to use here. Callers must keep that invariant.
 *
 * Port 0 is the decoder-facing main path: ES formats set bypass-reform,
 * not es=true. Any nonzero port uses the compressed bypass announcement.
 * MAT(4) and DTS-HD(6) share rate=48000/data-rate=192000/ch=8/bps=16,
 * but main-path bypass-reform differs. There is no label-only argument.
 * The log-only branches and their strings are omitted from this semantic
 * reconstruction; the externally visible metadata/return behavior is kept.
 */
int _hdmi_push_first_event(dsp0_hdmi *module, int port, int channels, int acmod)
{
    static const char *const labels[7] = {
        "pcm", "ac3", "eac3", "aac", "mat", "dts", "dts_hd_ma"
    };
    static const int es_channels[7] = {2, 2, 2, 2, 8, 2, 8};
    static const bool reform[7] = {false, true, true, true, false, true, true};
    int codec = module->codec;
    int rate = (int)module->sampling_freq, data_rate = rate, bits = 32;
    if (codec != 0) {
        channels = es_channels[codec];
        bits = 16;
        if (codec != 1 && codec != 3 && codec != 5) {
            rate = 48000;
            data_rate = 192000;
        }
    }
    dsp0_hdmi_event *event = aud_event_newv_first(
        "codec", 2, labels[codec], "rate", 0, rate,
        "data-rate", 0, data_rate, "channels", 0, channels,
        "bits-per-sample", 0, bits, (const char *)0);
    if (!event) return 0;
    if (codec == 0) {
        aud_event_value_set_int(event, "acmod", acmod);
        if (port == 0) aud_event_value_set_int(event, "out-channels", 8);
    } else if (port != 0) {
        aud_event_value_set_bool(event, "es", true);
    } else {
        aud_event_value_set_bool(event, "bypass-reform", reform[codec]);
    }
    aud_module_push_event(module, port, event);
    return 1;
}
