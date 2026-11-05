/* Usage: python3 tools/tests/test_kernel_seek.py. Host contracts, not TV execution. */
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#define noti(...) ((void)0)
#define info(...) ((void)0)
#define debug(...) ((void)0)
#define error(...) ((void)0)
#define PLAY_SPEED_INDEX_MAX 2
static unsigned int play_speed[2] = {1000, 1000};
static int reconnects;
static void sndout_connect_output(void) { reconnects++; }
struct mutex { int held; };
static void mutex_lock(struct mutex *m) { assert(!m->held); m->held = 1; }
static void mutex_unlock(struct mutex *m) { assert(m->held); m->held = 0; }
enum aud_clock_output { CLOCK_OUTPUT_SPEAKER, CLOCK_OUTPUT_HP, CLOCK_OUTPUT_OPTIC,
                        CLOCK_OUTPUT_ARC, CLOCK_OUTPUT_MAX };
#define CLOCK_CENTER 71
#define DEFAULT_DTO_RATE CLOCK_CENTER
struct _output;
struct _output_info {
    int uid; unsigned input, channel, sample_rate; bool bypass;
    struct { unsigned dto_rate; } clock_info;
    struct _output *output;
};
struct _output {
    const char *name; bool initialized, enable; struct _output_info *info;
    unsigned sample_rate; int (*set_sample_rate)(unsigned);
    struct mutex lock; bool output_changed, input_index_changed;
};
static struct _output output_list[CLOCK_OUTPUT_MAX];
static struct _output_info clock_state;
static int hal_calls, applied_rate, order[8], order_count;
static int apply_rate(unsigned rate) {
    assert(!output_list[CLOCK_OUTPUT_ARC].lock.held);
    hal_calls++; applied_rate = rate; order[order_count++] = 1; return -7;
}
enum output_codec_t { OUTPUT_CODEC_PCM, OUTPUT_CODEC_AC3, OUTPUT_CODEC_EAC3,
                      OUTPUT_CODEC_AAC, OUTPUT_CODEC_MAT, OUTPUT_CODEC_MAX };
enum { OUTPUT_SPEAKER, OUTPUT_HP, OUTPUT_OPTIC, OUTPUT_ARC };
typedef unsigned FW_AUD_OUTPUT_CODEC_T;
#define FW_AUD_OUTPUT_CODEC_MAT 4
struct aud_block_info { int unused; };
struct aud_output_info {
    struct aud_block_info parent; int output_type;
    enum output_codec_t codec; unsigned channel, sample_rate, acmod, bypass;
    bool earc_on;
};
typedef struct { unsigned codec, channel, sample_rate, acmod; } OutputEvtFirstAu;
#define OUTPUT_TO_STR(x) "ARC"
#define AUDIO_HAL_OUTPUT_PCM 0
#define AUDIO_HAL_OUTPUT_ES 1
static int on_calls;
static void audio_hal_earc_set_mute(bool on) { order[order_count++] = on ? 2 : 5; }
static void audio_hal_earc_set_channel(unsigned ch, unsigned acmod) { order[order_count++] = 3; }
static void audio_hal_earc_set_output_format(int fmt, unsigned rate) { order[order_count++] = 4; }
static void audio_hal_earc_set_on(bool on) { on_calls++; order[order_count++] = 6; }
struct sndout_decoder_state {
    struct aud_block_info *dec_info; bool is_open; unsigned input;
    bool main_output, bypass_output, hdmi_multi_pcm_out;
};
struct sndout_input_state { struct aud_block_info *input_info; int index; };
static struct sndout_decoder_state alsa_decoder_info[2];
static struct sndout_input_state alsa_input_info[2];
static struct mutex alsa_decoder_lock;
enum decoder_output_mode { DECODER_OUTPUT_MODE_MAIN, DECODER_OUTPUT_MODE_BYPASS };
static int drop_calls, drop_port; static bool dropping;
static int input_set_hdmi_bypass(struct aud_block_info *b, bool drop, int port) {
    assert(alsa_decoder_lock.held); drop_calls++; dropping = drop; drop_port = port; return -1;
}
static int multi_calls;
static int input_set_hdmi_multi_pcm_out(struct aud_block_info *b, bool enable) { multi_calls++; return -1; }
#include "clock_under_test.inc"

static void reset_clock(void) {
    memset(output_list, 0, sizeof(output_list));
    memset(&clock_state, 0, sizeof(clock_state));
    struct _output *out = &output_list[CLOCK_OUTPUT_ARC];
    *out = (struct _output){.name="arc", .initialized=true, .enable=true,
                           .info=&clock_state, .set_sample_rate=apply_rate};
    clock_state.output = out; clock_state.channel = 8; clock_state.input = 9;
    order_count = hal_calls = on_calls = 0;
}
int main(void) {
    notifier_set_play_speed(-1, 0); notifier_set_play_speed(2, 0);
    notifier_set_play_speed(0, 1000); assert(reconnects == 0);
    notifier_set_play_speed(0, 0); assert(reconnects == 1 && play_speed[0] == 0);
    notifier_set_play_speed(0, 0); assert(reconnects == 1);
    notifier_set_play_speed(0, 1000); assert(reconnects == 2);
    assert(notifier_get_play_speed(-1) == 1000 && notifier_get_play_speed(2) == 1000);
    reset_clock();
    assert(aud_clock_update_output(CLOCK_OUTPUT_ARC, 192000, false) == 0);
    assert(clock_state.channel == 8 && clock_state.input == 9);
    assert(clock_state.sample_rate == 192000 && !clock_state.bypass);
    assert(clock_state.clock_info.dto_rate == 71 && output_list[3].output_changed);
    assert(hal_calls == 1 && applied_rate == 192000);
    assert(aud_clock_update_output(CLOCK_OUTPUT_ARC, 192000, true) == 0);
    assert(hal_calls == 2 && clock_state.bypass);
    order_count = 0;
    assert(aud_clock_set_input_index(CLOCK_OUTPUT_ARC, 1) == 0);
    assert(clock_state.input == 1 && output_list[3].input_index_changed && hal_calls == 2);
    output_list[3].enable = false;
    assert(aud_clock_update_output(CLOCK_OUTPUT_ARC, 48000, false) == -1);
    assert(!output_list[3].lock.held && hal_calls == 2);
    assert(aud_clock_update_output((enum aud_clock_output)-1, 48000, false) == -1);
    reset_clock(); output_list[3].info = NULL;
    assert(aud_clock_update_output(CLOCK_OUTPUT_ARC, 48000, false) == -1);
    assert(output_list[3].lock.held); /* preserve the shipped invariant-dependent leak */
    reset_clock();
    struct aud_output_info out = {.output_type=OUTPUT_ARC, .codec=OUTPUT_CODEC_AC3,
                                 .channel=2, .sample_rate=48000, .acmod=2,
                                 .bypass=0, .earc_on=true};
    assert(output_set_format(&out.parent, OUTPUT_CODEC_PCM, 8, 192000, true) == 0);
    assert(!out.bypass && out.codec == OUTPUT_CODEC_AC3 && hal_calls == 0);
    OutputEvtFirstAu event = {OUTPUT_CODEC_PCM, 8, 96000, 7};
    assert(_cb_output_status(&event, 0, &out) == 0); /* length is not checked */
    assert(hal_calls == 1 && !clock_state.bypass && out.acmod == 7);
    assert(order_count == 6);
    for (int i = 0; i < 6; i++) assert(order[i] == i + 1);
    order_count = 0;
    _cb_output_status(&event, sizeof(event), &out);
    assert(hal_calls == 1 && order_count == 1 && on_calls == 2);
    out.bypass = 17; event.sample_rate = 48000; order_count = 0;
    _cb_output_status(&event, sizeof(event), &out);
    assert(clock_state.bypass && out.bypass == 1);
    assert(_adec_codec_convert_output_codec(5) == OUTPUT_CODEC_PCM);
    assert(_adec_codec_convert_output_codec(UINT_MAX) == OUTPUT_CODEC_PCM);
    struct aud_block_info input = {0};
    alsa_input_info[0].input_info = &input;
    alsa_decoder_info[0] = (struct sndout_decoder_state){.is_open=true, .input=5};
    assert(decoder_set_output_mode(0, DECODER_OUTPUT_MODE_MAIN, true) == 0);
    assert(dropping && drop_port == 1);
    assert(decoder_set_output_mode(0, DECODER_OUTPUT_MODE_BYPASS, true) == 0);
    assert(!dropping && drop_port == 0);
    assert(decoder_set_output_mode(0, DECODER_OUTPUT_MODE_MAIN, false) == 0);
    assert(dropping && drop_port == 0);
    assert(decoder_set_output_mode(0, DECODER_OUTPUT_MODE_BYPASS, false) == 0);
    assert(dropping && drop_port == 1 && drop_calls == 4);
    assert(decoder_set_output_mode(0, DECODER_OUTPUT_MODE_BYPASS, false) == -1);
    assert(drop_calls == 4 && !alsa_decoder_lock.held);
    assert(decoder_set_output_mode(-1, 0, true) == -1);
    alsa_decoder_info[0].input = 256;
    assert(decoder_set_output_mode(0, DECODER_OUTPUT_MODE_MAIN, true) == 0 && drop_calls == 4);
    alsa_decoder_info[0].is_open = false;
    assert(decoder_set_multi_pcm_out(0, true) == -1 && alsa_decoder_info[0].hdmi_multi_pcm_out);
    alsa_decoder_info[0].is_open = true; alsa_decoder_info[0].input = 255;
    assert(decoder_set_multi_pcm_out(0, false) == 0 && multi_calls == 1);
    puts("PASS: speed guards, clock locking/stores/HAL order, log-only format, stored bypass, HDMI modes");
    return 0;
}
