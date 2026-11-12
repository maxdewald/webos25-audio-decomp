/*
 * dsp0 "hdmi" input module (mod_hdmi_new, constructor @0x60229f64): HDMI audio capture,
 * IEC 61937 framing via HdmiParser_*, PCM/ES split into a main port (0, to the decoder)
 * and a bypass port (1, passthrough towards ARC/eARC). Only the FIRST-event producer and
 * the codec property check are reconstructed here.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct aud_event;

extern struct aud_event *aud_event_newv_first(const char *key, ...);       /* @0x60202100 */
extern void aud_event_value_set_bool(struct aud_event *, const char *, bool); /* @0x60202670 */
extern void aud_event_value_set_int(struct aud_event *, const char *, int);   /* @0x602025f4 */

enum { AUD_VALUE_INT = 0, AUD_VALUE_BOOL = 1, AUD_VALUE_STR = 2 };

/* Property table @0x60551e50: name, type, field offset. */
struct hdmi_module {
    uint8_t  unrecovered_000[0x158];
    int32_t  hdmi_port;        /* +0x158 "hdmi-port" */
    uint8_t  unrecovered_15c[0x0c];
    int32_t  sampling_freq;    /* +0x168 "sampling-freq" (also "clock-count") */
    uint8_t  unrecovered_16c[0x68];
    int32_t  data_drop_mode;   /* +0x1d4 "data-drop-mode" */
    int32_t  codec;            /* +0x1d8 "codec": 0..6, set by the host */
    uint8_t  unrecovered_1dc[0x18];
    uint32_t irq_count;        /* +0x1f4 */
};

struct aud_prop {
    const char *name;
    int32_t     type;
    int32_t     offset;
};

extern int aud_module_push_event(struct hdmi_module *, int port, struct aud_event *); /* @0x6020adc0 */

/* @0x60005820 / @0x60005860 (copied to the stack). Index = codec property. */
static const int32_t hdmi_es_channels[7] = { 2, 2, 2, 2, 8, 2, 8 };
static const uint8_t hdmi_es_reform[7]   = { 0, 1, 1, 1, 0, 1, 1 };

/* @0x60227e78: build and push the FIRST event for `port`. Answers DTS-over-eARC Q5:
 * this is the only producer of the HDMI "codec"/"es" values that dsp1 out_event reads.
 * The codec string is chosen by the host-set "codec" property (+0x1d8), NOT by the IEC
 * 61937 Pc data type (HdmiParser_ParseHeader @0x6023d3a0 uses Pc only for burst/frame
 * sizing; a DTS sub-type change only raises "hdmi_dts_format_changed" to the host).
 *   codec  string       ch  rate           data-rate      bps
 *   0      "pcm"        ch  sampling_freq  sampling_freq  32  (+acmod; port 0: out-channels 8)
 *   1      "ac3"        2   sampling_freq  sampling_freq  16
 *   2      "eac3"       2   48000          192000         16
 *   3      "aac"        2   sampling_freq  sampling_freq  16
 *   4      "mat"        8   48000          192000         16
 *   5      "dts"        2   sampling_freq  sampling_freq  16  -> dsp1 OUT codec 5
 *   6      "dts_hd_ma"  8   48000          192000         16  -> dsp1 OUT codec 7
 * "es"=true only for codec != 0 on the bypass port (port != 0); port 0 gets
 * "bypass-reform"=hdmi_es_reform[codec] instead. All DTS-HD (incl. HRA/MA/X) is "dts_hd_ma";
 * "dts_hd", "dts_ma", "dtsx_p1", "dtsx_p2" are never produced, and data-rate is never
 * 768000. dsp1 out_event therefore picks cpy_es8 (4 lanes, channels 8) but calls
 * HAL_SetArcEsMode(192000) -> eARC lane fs code 0xe, not the HBR code 9. */
int _hdmi_push_first_event(struct hdmi_module *m, int port, int pcm_channels, int acmod)
{
    const char *names[7] = { "pcm", "ac3", "eac3", "aac", "mat", "dts", "dts_hd_ma" };
    int codec = m->codec;
    int channels, rate, data_rate, bps;

    if (codec == 0) {
        channels = pcm_channels;
        rate = data_rate = m->sampling_freq;
        bps = 32;
    } else {
        channels = hdmi_es_channels[codec];
        if (codec == 1 || codec == 3 || codec == 5) {   /* FLIX beqi.w18 1/3/5 @0x60227ed4 */
            rate = data_rate = m->sampling_freq;
        } else {
            rate = 48000;
            data_rate = 192000;
        }
        bps = 16;
    }

    /* codec 7 passes hdmi_prop_updated but indexes past names[] (uninitialised stack). */
    struct aud_event *ev = aud_event_newv_first(
        "codec", AUD_VALUE_STR, names[codec],
        "rate", AUD_VALUE_INT, rate,
        "data-rate", AUD_VALUE_INT, data_rate,
        "channels", AUD_VALUE_INT, channels,
        "bits-per-sample", AUD_VALUE_INT, bps,
        NULL);
    if (ev == NULL) {
        /* "first_event is NULL." */
        return 0;
    }

    if (m->codec == 0) {
        aud_event_value_set_int(ev, "acmod", acmod);
        if (port == 0) {
            aud_event_value_set_int(ev, "out-channels", 8);
            /* "[HDMI_%d] PCM main:   codec=%d(%s), ch=%u, rate=%u, acmod=%u, drop=%d, irq_count=%u" */
        } else {
            /* "[HDMI_%d] PCM bypass: codec=%d(%s), ch=%u, rate=%u, acmod=%u, drop=%d, irq_count=%u" */
        }
    } else if (port == 0) {
        aud_event_value_set_bool(ev, "bypass-reform", hdmi_es_reform[m->codec]);
        /* "[HDMI_%d] ES main: codec=%d(%s), reform = %d, ch=%u, rate=%u, data-rate=%u, irq_count=%u" */
    } else {
        aud_event_value_set_bool(ev, "es", true);
        /* "[HDMI_%d] ES bypass: codec=%d(%s), ch=%u, rate=%u, data-rate=%u, irq_count=%u" */
    }

    aud_module_push_event(m, port, ev);
    return 1;
}

/* @0x6022a210: only the "codec" property is checked: >= 8 is reset to 0 (so 7 slips through). */
int hdmi_prop_updated(struct hdmi_module *m, const struct aud_prop *p)
{
    /* type 1/2: "%s: updated %s=%s" / "%s: updated %s=%d" when debug bit 3 is set */
    if (p->offset != offsetof(struct hdmi_module, codec))
        return 0;
    /* "codec %d" */
    if (m->codec >= 8) {
        /* "codec(%d) is incorrect!!!" */
        m->codec = 0;
    }
    return 0;
}
