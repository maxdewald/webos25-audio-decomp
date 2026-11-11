/* HDMI FIRST metadata: the real producer body, not an invented relabel path. */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../src/dsp0/hdmi.c"

struct dsp0_hdmi_event {
    const char *codec;
    int rate, data_rate, channels, bits, acmod, out_channels;
    bool es, reform, have_es, have_reform, have_acmod;
};
static struct dsp0_hdmi_event event;
static bool allocation_fails;
static unsigned delivered;
static unsigned allocations;
static int delivered_port;
static dsp0_hdmi *delivered_module;

dsp0_hdmi_event *aud_event_newv_first(const char *key, ...)
{
    allocations++;
    if (allocation_fails) return NULL;
    memset(&event, 0, sizeof(event));
    va_list args;
    va_start(args, key);
    while (key) {
        int type = va_arg(args, int);
        if (type == 2) {
            assert(strcmp(key, "codec") == 0);
            event.codec = va_arg(args, const char *);
        } else {
            assert(type == 0);
            int v = va_arg(args, int);
            if (!strcmp(key, "rate")) event.rate = v;
            else if (!strcmp(key, "data-rate")) event.data_rate = v;
            else if (!strcmp(key, "channels")) event.channels = v;
            else if (!strcmp(key, "bits-per-sample")) event.bits = v;
            else assert(!"unexpected FIRST field");
        }
        key = va_arg(args, const char *);
    }
    va_end(args);
    return &event;
}
void aud_event_value_set_int(dsp0_hdmi_event *e, const char *key, int v)
{
    if (!strcmp(key, "acmod")) { e->acmod = v; e->have_acmod = true; }
    else { assert(!strcmp(key, "out-channels")); e->out_channels = v; }
}
void aud_event_value_set_bool(dsp0_hdmi_event *e, const char *key, bool v)
{
    if (!strcmp(key, "es")) { e->es = v; e->have_es = true; }
    else { assert(!strcmp(key, "bypass-reform")); e->reform = v; e->have_reform = true; }
}
void aud_module_push_event(dsp0_hdmi *m, int port, dsp0_hdmi_event *e)
{
    assert(e == &event);
    delivered++; delivered_port = port; delivered_module = m;
}

int main(void)
{
    dsp0_hdmi m = {0};
    m.sampling_freq = 44100;
    assert(_hdmi_push_first_event(&m, 0, 6, 7) == 1);
    assert(delivered == 1 && delivered_port == 0 && delivered_module == &m);
    assert(!strcmp(event.codec, "pcm") && event.rate == 44100 && event.data_rate == 44100);
    assert(event.channels == 6 && event.bits == 32 && event.acmod == 7 && event.out_channels == 8);
    assert(!event.have_es && !event.have_reform);
    assert(_hdmi_push_first_event(&m, 1, 2, 3) == 1);
    assert(event.have_acmod && event.acmod == 3 && event.out_channels == 0 && !event.have_es);

    const char *labels[] = {"pcm", "ac3", "eac3", "aac", "mat", "dts", "dts_hd_ma"};
    for (int codec = 1; codec <= 6; codec++) {
        m.codec = codec;
        for (int port = 0; port <= 2; port++) {
            assert(_hdmi_push_first_event(&m, port, 99, 42) == 1);
            assert(delivered_port == port && delivered_module == &m);
            assert(!strcmp(event.codec, labels[codec]));
            assert(event.channels == ((codec == 4 || codec == 6) ? 8 : 2));
            assert(event.bits == 16 && !event.have_acmod);
            assert(event.rate == ((codec == 1 || codec == 3 || codec == 5) ? 44100 : 48000));
            assert(event.data_rate == ((codec == 1 || codec == 3 || codec == 5) ? 44100 : 192000));
            assert(event.have_es == (port != 0) && event.es == (port != 0));
            assert(event.have_reform == (port == 0));
            assert(event.reform == (port == 0 && codec != 4));
        }
    }
    unsigned before = delivered;
    allocation_fails = true;
    assert(_hdmi_push_first_event(&m, 1, 2, 0) == 0 && delivered == before);
    allocation_fails = false;
    unsigned allocations_before = allocations;
    m.sampling_freq = 96000; m.data_drop_mode = 2; m.irq_count = 19;
    /* Only a codec property's notification validates the already-stored value.
     * It must not emit a FIRST or switch the processing mode. */
    dsp0_hdmi_property codec_prop = {0, 2, 0x1d8};
    dsp0_hdmi_property rate_prop = {0, 2, 0x168};
    m.codec = 8;
    assert(hdmi_prop_updated(&m, &rate_prop) == 0 && m.codec == 8);
    assert(hdmi_prop_updated(&m, &codec_prop) == 0 && m.codec == 0);
    m.codec = 7;
    assert(hdmi_prop_updated(&m, &codec_prop) == 0 && m.codec == 7);
    m.codec = -1;
    assert(hdmi_prop_updated(&m, &codec_prop) == 0 && m.codec == -1);
    m.codec = 4;
    assert(hdmi_prop_updated(&m, &codec_prop) == 0 && m.codec == 4);
    assert(delivered == before);
    assert(allocations == allocations_before);
    assert(m.sampling_freq == 96000 && m.data_drop_mode == 2 && m.irq_count == 19);
    puts("DSP0 HDMI: FIRST formats, MAT vs DTS-HD, allocation failure and property-validation bounds");
    return 0;
}
