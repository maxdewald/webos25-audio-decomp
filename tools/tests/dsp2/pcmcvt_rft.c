/* Execute the recovered pcmcvt control/data path with the REAL reconstructed
 * RFT, mocking only SRC/framework. This is not execution on Xtensa hardware. */
#define PCMCVT_REAL_RFT
#include "pcmcvt.c"
#include "../../../src/dsp2/rft.c"

#define aud_module_cmd converter_bin_module_cmd
#define aud_module_default_command converter_bin_default_command
// @match src/dsp2/bin.c (?s)struct aud_bin_link \{.*?extern int aud_module_default_command[^;]+;
// @extract src/dsp2/bin.c bin_cmd bin_command aud_module_bin_add_cmd_propagate aud_module_bin_propagate_pending
#undef aud_module_default_command
#undef aud_module_cmd

struct converter_target {
    struct aud_bin_module module;
    struct pcmcvt *converter;
};
static int converter_command(struct aud_bin_module *module, uint32_t command,
                              void *payload, unsigned length)
{
    return Pcmcvt_Cmd(((struct converter_target *)module)->converter, command, payload, length);
}
int converter_bin_module_cmd(struct aud_bin_module *module, uint32_t command,
                              void *payload, unsigned length)
{
    return module->command(module, command, payload, length);
}
int converter_bin_default_command(struct aud_bin *bin, uint32_t command,
                                  void *payload, unsigned length)
{
    (void)bin; (void)command; (void)payload; (void)length;
    return -1;
}
static void assert_history_cleared(const struct rft_config *state)
{
    for (unsigned index = 0; index < 4; ++index) assert(state->lfe_history[index] == 0);
}
static void pending_converter_tests(void)
{
    struct pcmcvt converter = {0};
    struct rft_config *state = (struct rft_config *)converter.rft;
    state->gain = 12345;
    assert(Pcmcvt_Constructor(&converter) == 0);
    assert(converter.input_rate == 48000 && converter.output_bits == 32);
    assert(converter.acmod == 2 && RFT_GetAcmod(state) == 0);
    assert(converter.gain == 0x800000 && !RFT_GetGainEnable(state));
    assert(RFT_GetGai(state) == 12345 && !converter.over_protect && !converter.generate_lfe);
    struct aud_bin bin;
    bin.targets.next = bin.targets.previous = &bin.targets;
    bin.pending.next = bin.pending.previous = &bin.pending;
    struct converter_target target = {{"pcmcvt", converter_command}, &converter};
    uint32_t format[] = {192000, 8, 16, 0, 0, 192000, 8, 16};
    uint32_t gain[] = {56789, 1};
    assert(bin_command(&bin, 0x50020020, format, sizeof(format)) == -1);
    assert(bin_command(&bin, 0x50040008, gain, sizeof(gain)) == -1);
    assert(converter.output_bits == 32 && !RFT_GetGainEnable(state));
    format[7] = 32;
    gain[0] = 999;
    assert(aud_module_bin_add_cmd_propagate(&bin, &target.module) == 0);
    assert(aud_module_bin_propagate_pending(&bin) == 0);
    assert(converter.input_rate == 192000 && converter.output_rate == 192000);
    assert(converter.input_channels == 8 && converter.output_channels == 8);
    assert(converter.input_bits == 16 && converter.output_bits == 16);
    assert(RFT_GetInPCMFormat(state) == 16 && RFT_GetOutPCMFormat(state) == 16);
    assert(!RFT_GetInSigned(state) && !RFT_GetInEndian(state) && converter.src_bypass);
    assert(RFT_GetGainEnable(state) && RFT_GetGai(state) == 56789);
    assert(converter.gain == 0x800000 && !converter.gain_enable_property);
    struct mock_event first = {.base = {1, 0}};
    aud_event_value_set_int(&first.base, "channels", 8);
    aud_event_value_set_int(&first.base, "bits-per-sample", 16);
    aud_event_value_set_int(&first.base, "rate", 192000);
    aud_event_value_set_int(&first.base, "es", 1);
    converter.previous_ts = 100;
    converter.output_ts = 200;
    converter.total_input = 300;
    converter.total_output = 400;
    state->lfe_history[0] = 88;
    assert(Pcmcvt_Event(&converter, NULL, &first.base) == 0);
    assert(RFT_GetGainEnable(state) && RFT_GetGai(state) == 56789);
    assert_history_cleared(state);
    assert(aud_event_value_get_int(last_event, "es", 0) == 1);
    state->lfe_history[0] = 99;
    converter.adapter.pushed = 100;
    assert(Pcmcvt_Cmd_Flush(&converter) == 0);
    assert(converter.adapter.pushed == converter.adapter.flushed);
    assert(converter.previous_ts == 100 && converter.output_ts == 200);
    assert(converter.total_input == 300 && converter.total_output == 400);
    assert(RFT_GetGainEnable(state) && RFT_GetGai(state) == 56789);
    assert_history_cleared(state);
    uint32_t disabled[] = {0, 0};
    assert(bin_command(&bin, 0x50040008, disabled, sizeof(disabled)) == -1);
    assert(!RFT_GetGainEnable(state) && RFT_GetGai(state) == 56789);
    set_prop_gain_enable(&converter, NULL, "true");
    assert(RFT_GetGainEnable(state) && RFT_GetGai(state) == 0x800000);
    assert(bin_command(&bin, 0x50040008, disabled, sizeof(disabled)) == -1);
    converter.adapter.pushed = converter.adapter.flushed = 0;
    struct aud_port port = {.module = &converter};
    struct aud_data input = {.type = 0, .ts = 12000, .size = 4096, .ptr = input_bytes};
    int before = src_calls;
    assert(Pcmcvt_Chai(&port, &input) == 0);
    assert(src_calls == before && last_data->type == 0 && last_data->size == 4096);
    assert(memcmp(last_data->ptr, input_bytes, 4096) == 0);
    free(last_data->ptr); free(last_data);
    int drop = dropped;
    input.type = 1;
    assert(Pcmcvt_Chai(&port, &input) == 0 && dropped == drop + 1);
    state->lfe_history[0] = 77;
    RFT_SetGai(state, 1, 111);
    assert(Pcmcvt_Cmd(&converter, 0x10000000, NULL, 0) == 0);
    assert(converter.previous_ts == UINT32_MAX && converter.output_ts == 12000);
    assert(converter.total_input == 0 && converter.total_output == 0);
    assert(state->lfe_history[0] == 77 && RFT_GetGai(state) == 111);
    assert(RFT_GetGainEnable(state));
    _BufMonitor_RemoveBufferIndex(bin.targets.next);
    Pcmcvt_Destructor(&converter);
}

static void gain_control_tests(void)
{
    struct pcmcvt converter = {0};
    struct rft_config *state = (struct rft_config *)converter.rft;
    state->gain = 12345;
    assert(Pcmcvt_Constructor(&converter) == 0);
    assert(converter.gain == 0x800000 && !converter.gain_enable_property);
    assert(RFT_GetGainEnable(state) == 0 && RFT_GetGai(state) == 12345);

    uint32_t command[] = {0x800000, 1};
    assert(Pcmcvt_Cmd(&converter, 0x50040008, command, sizeof(command)) == 0);
    assert(RFT_GetGainEnable(state) == 1 && RFT_GetGai(state) == 0x800000);
    assert(set_prop_gain(&converter, NULL, "4194304") == 0);
    assert(converter.gain == 0x400000 && !converter.gain_enable_property);
    assert(RFT_GetGainEnable(state) == 1 && RFT_GetGai(state) == 0x400000);

    command[0] = 56789;
    assert(Pcmcvt_Cmd(&converter, 0x50040008, command, sizeof(command)) == 0);
    assert(converter.gain == 0x400000 && RFT_GetGai(state) == 56789);
    assert(set_prop_gain(&converter, NULL, "8388608") == 0);
    assert(converter.gain == 0x800000 && !converter.gain_enable_property);
    assert(RFT_GetGainEnable(state) == 0 && RFT_GetGai(state) == 56789);
    assert(set_prop_gain_enable(&converter, NULL, "true") == 0);
    assert(RFT_GetGainEnable(state) == 1 && RFT_GetGai(state) == 0x800000);
    assert(!converter.gain_enable_property);

    uint32_t format[] = {192000, 8, 16, 0, 0, 192000, 8, 16};
    assert(Pcmcvt_Cmd(&converter, 0x50020020, format, sizeof(format)) == 0);
    assert(converter.src_bypass == 1);
    assert(RFT_GetGainEnable(state) == 1 && RFT_GetGai(state) == 0x800000);
    state->lfe_history[0] = 77;
    command[0] = 999;
    command[1] = 2;
    assert(Pcmcvt_Cmd(&converter, 0x50040008, command, sizeof(command)) == 0);
    assert(RFT_GetGainEnable(state) == 0 && RFT_GetGai(state) == 0x800000);
    assert(converter.gain == 0x800000 && state->lfe_history[0] == 77);

    assert(RFT_SetOutSample(state, 256) == 0);
    struct rft_config before = *state;
    uint8_t output[4096];
    int32_t scratch[2048];
    memset(scratch, 0xa5, sizeof(scratch));
    assert(LGRFT(state, input_bytes, output, scratch, 0) == 2048);
    assert(memcmp(output, input_bytes, sizeof(output)) == 0);
    assert(memcmp(state, &before, sizeof(before)) == 0);
    for (size_t index = 0; index < sizeof(scratch) / sizeof(scratch[0]); ++index)
        assert((uint32_t)scratch[index] == UINT32_C(0xa5a5a5a5));
    Pcmcvt_Destructor(&converter);
}

static void property_converter_tests(void)
{
    source = (struct mock_src){.frames = 256};
    struct pcmcvt raw = {0};
    assert(Pcmcvt_Constructor(&raw) == 0);
    RFT_SetGai(raw.rft, 1, 12345);
    configure(&raw, 16);
    uint32_t disabled[] = {0, 0};
    assert(Pcmcvt_Cmd(&raw, 0x50040008, disabled, sizeof(disabled)) == 0);
    struct mock_src expected_source = source;

    source = (struct mock_src){.frames = 256};
    struct pcmcvt properties = {0};
    assert(Pcmcvt_Constructor(&properties) == 0);
    RFT_SetGai(properties.rft, 1, 12345);
    assert(Pcmcvt_SetParamToLibs(&properties) == 0);
    uint32_t active_rates[] = {192000, 192000};
    assert(Pcmcvt_Cmd(&properties, 0x50030008, active_rates, sizeof(active_rates)) == 0);
    assert(source.output_rate == 192000 && properties.output_rate == 48000);
    const struct { const char *name, *value; } sequence[] = {
        {"in-sampling-rate", "192000"},
        {"out-sampling-rate", "192000"},
        {"in-channels", "8"},
        {"in-bit-per-sample", "16"},
        {"out-channels", "8"},
        {"out-bit-per-sample", "16"},
        {"endian", "0"},
        {"sign", "0"},
        {"gain-enable", "0"}
    };
    for (unsigned index = 0; index < sizeof(sequence) / sizeof(sequence[0]); ++index) {
        const struct pcmcvt_property *property = pcmcvt_properties;
        while (property->name != NULL && strcmp(property->name, sequence[index].name))
            ++property;
        assert(property->name != NULL && property->set != NULL);
        assert(property->set(&properties, property, sequence[index].value) == 0);
    }
    assert(properties.input_rate == raw.input_rate && properties.output_rate == raw.output_rate);
    assert(properties.input_channels == raw.input_channels && properties.output_channels == raw.output_channels);
    assert(properties.input_bits == raw.input_bits && properties.output_bits == raw.output_bits);
    assert(properties.input_endian == raw.input_endian && properties.input_sign == raw.input_sign);
    assert(properties.src_bypass == raw.src_bypass && properties.sample_shift == raw.sample_shift);
    assert(source.input_rate == expected_source.input_rate && source.output_rate == expected_source.output_rate);
    assert(source.bits == expected_source.bits && source.channels == expected_source.channels);
    assert(source.mode == expected_source.mode && source.filter == expected_source.filter);
    assert(source.frames == expected_source.frames);
    assert(!RFT_GetGainEnable(properties.rft) && !RFT_GetGainEnable(raw.rft));
    assert(RFT_GetGai(properties.rft) == RFT_GetGai(raw.rft));
    assert(properties.gain == raw.gain && !properties.gain_enable_property);
    Pcmcvt_Destructor(&raw);

    struct mock_event first = {.base = {1, 0}};
    aud_event_value_set_int(&first.base, "channels", 8);
    aud_event_value_set_int(&first.base, "bits-per-sample", 16);
    aud_event_value_set_int(&first.base, "rate", 192000);
    assert(Pcmcvt_Event(&properties, NULL, &first.base) == 0);
    assert(Pcmcvt_Cmd_Flush(&properties) == 0 && properties.src_bypass == 1);
    struct aud_port port = {.module = &properties};
    struct aud_data input = {.type = 0, .ts = 12000, .size = 4096, .ptr = input_bytes};
    int before = src_calls;
    assert(Pcmcvt_Chai(&port, &input) == 0);
    assert(src_calls == before && last_data->type == 0 && last_data->size == 4096);
    assert(memcmp(last_data->ptr, input_bytes, 4096) == 0);
    free(last_data->ptr); free(last_data);

    assert(set_prop_output_rate(&properties, NULL, "48000") == 0);
    assert(set_prop_output_rate(&properties, NULL, "192000") == 0);
    assert(source.input_rate == source.output_rate && properties.src_bypass == 0);
    assert(Pcmcvt_Chai(&port, &input) == 0 && src_calls == before + 1);
    free(last_data->ptr); free(last_data);
    assert(set_prop_input_sign(&properties, NULL, "0") == 0 && properties.src_bypass == 1);
    Pcmcvt_Destructor(&properties);
}

int32_t rft_gain_sample_flix(int32_t sample, int32_t gain)
{
    (void)sample; (void)gain;
    assert(!"unrecovered gain must not execute in transparency test");
    return 0;
}
void rft_lfe_filter_flix(struct rft_config *r, int32_t *mono, size_t frames)
{
    (void)r; (void)mono; (void)frames;
    assert(!"unrecovered LFE must not execute in transparency test");
}
int main(void)
{
    /* Suppress unused static test warnings from shared mocked services. */
    (void)command_tests; (void)event_tests; (void)chain_tests;
    for (unsigned i=0;i<4096;++i) input_bytes[i]=(unsigned char)(i*37u+11u);
    pending_converter_tests();
    gain_control_tests();
    property_converter_tests();
    for(int bits=16;bits<=32;bits+=16) {
        struct pcmcvt m=fresh();
        RFT_SetGai(m.rft,0,0x800000);
        m.acmod=2;
        configure(&m,bits);
        assert(m.src_bypass==1);
        struct aud_port port={.module=&m};
        struct aud_data input={.type=0,.ts=12000,.size=4096,.ptr=input_bytes};
        int before=src_calls;
        Pcmcvt_Chai(&port,&input);
        assert(src_calls==before && last_data->type==0 && last_data->ts==12000);
        const unsigned char *out=last_data->ptr;
        if(bits==16) {
            assert(last_data->size==4096 && !memcmp(out,input_bytes,4096));
        } else {
            assert(last_data->size==8192);
            for(unsigned sample=0;sample<2048;++sample) {
                assert(out[sample*4]==0 && out[sample*4+1]==0);
                assert(out[sample*4+2]==input_bytes[sample*2]);
                assert(out[sample*4+3]==input_bytes[sample*2+1]);
            }
        }
        free(last_data->ptr); free(last_data);
        if(bits==16) {
            for(int acmod=0;acmod<=10;++acmod) {
                m.acmod=acmod;
                m.generate_lfe=true;
                m.available=true;
                assert(Pcmcvt_Cmd_SetOverprotect(&m,1)==0);
                Pcmcvt_SetParamToLibs(&m);
                Pcmcvt_Chai(&port,&input);
                assert(src_calls==before && last_data->type==0);
                assert(last_data->size==4096 && !memcmp(last_data->ptr,input_bytes,4096));
                free(last_data->ptr); free(last_data);
            }
        }
        int drop=dropped;
        input.type=1;
        Pcmcvt_Chai(&port,&input);
        assert(dropped==drop+1);
    }
    puts("PASS bin+pcmcvt+RFT: gain cache/command ordering, raw/property equivalence, rate-order trap, FIRST/flush/INIT, 192k 8ch byte-copy, ES rejection");
    return 0;
}
