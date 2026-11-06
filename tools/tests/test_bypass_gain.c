/* Host-only control-flow tests: the gain hook is deliberately NOT DSP math. */
#define PCMCVT_REAL_RFT
#include "dsp2/pcmcvt.c"
#include "../../src/dsp2/rft.c"

static unsigned gain_samples;
static int32_t observed_gain;
int32_t rft_gain_sample_flix(int32_t sample, int32_t gain)
{
    ++gain_samples;
    observed_gain = gain;
    return rft_bits((uint32_t)sample ^ UINT32_C(0x00010000));
}
void rft_lfe_filter_flix(struct rft_config *r, int32_t *mono, size_t frames)
{
    (void)r; (void)mono; (void)frames;
    assert(!"LFE must not run on equal-channel gain test");
}

static struct rft_config state(const struct pcmcvt *m)
{
    struct rft_config r;
    memcpy(&r, m->rft, sizeof(r));
    return r;
}
static void active(const struct pcmcvt *m, int enabled, int gain)
{
    struct rft_config r = state(m);
    assert(r.gain_enable == enabled && r.gain == gain);
}
static void property(struct pcmcvt *m, const char *key, const char *value)
{
    /* Emulate ONLY module_core search_prop_set/custom-callback dispatch, not
     * its parser, transport or locks. The real recovered descriptor is used. */
    for (const struct pcmcvt_property *p = pcmcvt_properties; p->name; ++p)
        if (!strcmp(p->name, key)) {
            assert(p->set && !(p->flags & 1));
            assert(p->set(m, p, value) == 0);
            return;
        }
    assert(!"unknown test property");
}
static void send_pcm(struct pcmcvt *m, int gain_enabled)
{
    unsigned before = gain_samples;
    struct aud_port port = {.module=m};
    struct aud_data input = {.type=0, .ts=12000, .size=4096, .ptr=input_bytes};
    Pcmcvt_Chai(&port, &input);
    assert(last_data->type == 0 && last_data->size == sizeof(uint16_t)*2048);
    assert(gain_samples - before == (gain_enabled ? 2048u : 0));
    if (!gain_enabled)
        assert(!memcmp(input_bytes, last_data->ptr, 4096));
    else {
        const uint8_t *out = last_data->ptr;
        for (unsigned i=0; i<4096; ++i)
            assert(out[i] == (uint8_t)(input_bytes[i] ^ (i%2 == 0 ? 1 : 0)));
    }
    free(last_data->ptr); free(last_data);
}
static void persistence(struct pcmcvt *m, int gain)
{
    static const uint32_t lifecycle[] = {
        0x10000000, 0x10010000, 0x10020000, 0x1003002c, 0x10070000
    };
    for (unsigned i=0; i<sizeof(lifecycle)/sizeof(lifecycle[0]); ++i) {
        assert(Pcmcvt_Cmd(m, lifecycle[i], NULL, 0) == 0);
        active(m, 1, gain);
    }
    assert(Pcmcvt_Cmd_Flush(m) == 0);
    active(m, 1, gain);
    configure(m, 16);
    active(m, 1, gain);
    property(m, "out-bit-per-sample", "16");
    active(m, 1, gain);
    struct mock_event first = {.base={.type=1}};
    struct aud_port port = {.module=m};
    assert(Pcmcvt_Event(m, &port, &first.base) == 0);
    active(m, 1, gain);
}
static void controls(void)
{
    (void)command_tests; (void)event_tests; (void)chain_tests;
    struct pcmcvt m = {0};
    struct rft_config sentinel = {.gain=0x123456};
    memcpy(m.rft, &sentinel, sizeof(sentinel));
    assert(Pcmcvt_Constructor(&m) == 0);
    active(&m, 0, 0x123456);
    assert(m.gain == 0x800000 && !m.gain_enable_property);
    configure(&m, 16);
    assert(m.src_bypass == 1);
    send_pcm(&m, 0);

    uint32_t command[] = {0x800000, 1};
    assert(Pcmcvt_Cmd(&m, 0x50040008, command, 0) == 0);
    active(&m, 1, 0x800000);
    send_pcm(&m, 1);
    assert(observed_gain == 0x800000);
    persistence(&m, 0x800000);

    command[0] = 0x234567;
    assert(Pcmcvt_Cmd(&m, 0x50040008, command, sizeof(command)) == 0);
    active(&m, 1, 0x234567);
    assert(m.gain == 0x800000 && !m.gain_enable_property);
    persistence(&m, 0x234567);
    command[0] = 0x555555;
    command[1] = 2;
    assert(Pcmcvt_Cmd(&m, 0x50040008, command, sizeof(command)) == 0);
    active(&m, 0, 0x234567);
    send_pcm(&m, 0);

    property(&m, "gain-enable", "true");
    active(&m, 1, 0x800000);
    assert(!m.gain_enable_property);
    property(&m, "gain", "0");
    active(&m, 1, 0);
    assert(m.gain == 0);
    property(&m, "gain", "8388608");
    active(&m, 0, 0);
    send_pcm(&m, 0);
    static const char *enabled[] = {"true", "yes", "1"};
    for (unsigned i=0; i<3; ++i) {
        property(&m, "gain-enable", enabled[i]);
        active(&m, 1, 0x800000);
        send_pcm(&m, 1);
        property(&m, "gain-enable", "TRUE");
        active(&m, 0, 0x800000);
    }
    /* Full generic transport is external in this harness; prove delegation,
     * rather than falsely claiming the host stub implements that transport. */
    assert(Pcmcvt_Cmd(&m, 0x10180004, command, sizeof(command)) == 23);
    Pcmcvt_Destructor(&m);
}
static void exhaustive_copy(void)
{
    uint16_t in[2048], out[2048];
    int32_t scratch[2048];
    struct rft_config r = {.in_bytes=2, .out_bytes=2, .in_ch=8,
                           .out_ch=8, .processing_frames=256};
    unsigned before = gain_samples;
    for (unsigned base=0; base<65536; base+=2048) {
        for (unsigned i=0; i<2048; ++i) in[i]=(uint16_t)(base+i);
        for (int available=0; available<=1; ++available) {
            memset(scratch, 0xa5, sizeof(scratch));
            assert(LGRFT(&r, in, out, scratch, available) == 2048);
            assert(!memcmp(in, out, sizeof(in)));
            for (unsigned i=0; i<2048; ++i) assert((uint32_t)scratch[i] == 0xa5a5a5a5u);
        }
        assert(_LGRFT_AlignSignedto32(&r, in, scratch) == 0);
        assert(_LGRFT_WriteOut(&r, scratch, out) == 0);
        assert(!memcmp(in, out, sizeof(in)));
    }
    assert(gain_samples == before);
    /* Firmware caches gain before stores: an aliased destination must not
     * change the coefficient observed by the second iteration. */
    union { struct rft_config config; int32_t words[15]; } alias = {.config={.gain=0x123456}};
    int32_t samples[2] = {0x11110000, 0x22220000};
    assert(_LGRFT_Gai(&alias.config, samples, alias.words + 7, 2) == 0);
    assert(observed_gain == 0x123456);
    r.gain_enable=2;
    assert(LGRFT(&r, in, out, scratch, 0) == 2048 && !memcmp(in,out,sizeof(in)));
    r.gain_enable=0; r.in_endian=1;
    assert(LGRFT(&r, in, out, scratch, 0) == 2048);
    for (unsigned i=0; i<2048; ++i) assert(out[i] == (uint16_t)((in[i]<<8)|(in[i]>>8)));
    r.in_endian=0; r.in_unsigned=1;
    assert(LGRFT(&r, in, out, scratch, 0) == 2048);
    for (unsigned i=0; i<2048; ++i) assert(out[i] == (uint16_t)(in[i]^0x8000));
}
int main(void)
{
    for (unsigned i=0; i<sizeof(input_bytes); ++i) input_bytes[i]=(uint8_t)(i*37u+11u);
    controls();
    exhaustive_copy();
    puts("PASS gain: constructor-off, commands/property cache, lifecycle persistence, exhaustive signed16 copy; enabled math mocked");
    return 0;
}
