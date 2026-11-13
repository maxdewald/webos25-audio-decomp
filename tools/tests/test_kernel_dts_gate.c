/* C5 codec gate and input lifecycle; no reference-only DTS decoder/events. */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#define debug_call() ((void)0)
#define error(...) ((void)0)
#define noti(...) ((void)0)
#define info(...) ((void)0)
struct mutex { int held; };
static void mutex_lock(struct mutex *m) { assert(!m->held); m->held = 1; }
static void mutex_unlock(struct mutex *m) { assert(m->held); m->held = 0; }
struct snd_kcontrol { int unused; };
struct snd_ctl_elem_value { union { struct { long value[4]; } integer; } value; };
enum input_type { INPUT_SIF, INPUT_TP, INPUT_HDMI, INPUT_ADC, INPUT_MAX };
enum input_evt { INPUT_EVT_TP_ES_EXIST_INFO, INPUT_EVT_HDMI_PCM_FORMAT,
    INPUT_EVT_TP_PDEC_ERROR_INFO, INPUT_EVT_HDMI_DTS_FORMAT_CHANGED, INPUT_EVT_MAX };
enum audio_decoder_event { AUDIO_DECODER_EVENT_ERROR, AUDIO_DECODER_EVENT_INFO,
    AUDIO_DECODER_EVENT_MPEGH_UI, AUDIO_DECODER_EVENT_MAX };
enum audio_renderer_event { AUDIO_RENDERER_EVENT_MASTER_INFO, AUDIO_RENDERER_EVENT_MAX };
enum RENDERER_CLK_TYPE { RENDERER_CLK_TYPE_PCR_M = 1, RENDERER_CLK_TYPE_GSTC = 0x100,
    RENDERER_CLK_TYPE_MASTER = 0x200 };
struct aud_block_info {
    enum input_type type;
    bool running, dropping, forced_pcm, clock_x4;
    unsigned int input_events, decoder_events, idle_time;
    int codec;
};
enum audio_decoder_codec { AUDIO_DECODER_CODEC_PCM = 1, AUDIO_DECODER_CODEC_AC3,
    AUDIO_DECODER_CODEC_EAC3, AUDIO_DECODER_CODEC_AC4, AUDIO_DECODER_CODEC_MAT,
    AUDIO_DECODER_CODEC_TRUE_HD, AUDIO_DECODER_CODEC_AAC, AUDIO_DECODER_CODEC_MP3,
    AUDIO_DECODER_CODEC_MPEG_H, AUDIO_DECODER_CODEC_DRA };
// @match src/kernel/aud/alsa_decoder.c (?s)struct sndout_(?:decoder|input|renderer)_state\n\{.*?\};
static struct sndout_decoder_state alsa_decoder_info[2];
static struct sndout_input_state alsa_input_info[2];
static struct sndout_renderer_state alsa_renderer_info[2];
static struct mutex alsa_decoder_lock;
static bool gate, created;
static bool audio_drop_enable, atmos_decoding_enable, ad_on_off, encoder_connected;
static int main_audio_index = -1, adc_port_index;
static int set_codec, port_out;
static bool aud_hal_dts_supported(void) { return gate; }
static void ablind_post(const char *n, int c, int a, int b) {}
static bool decoder_is_created(struct aud_block_info *b) { return created; }
static enum input_type input_get_type(struct aud_block_info *b) { return b->type; }
static int decoder_set_codec(struct aud_block_info *b, enum audio_decoder_codec c) { set_codec = c; return 0; }
static int bm_connect_block(struct aud_block_info *o, int po, struct aud_block_info *i, int pi) { port_out = po; return 0; }
static int bm_disconnect_block(struct aud_block_info *b, int port) { return 0; }
static struct aud_block_info *bm_get_block_info_by_index(const char *name, int index) { return NULL; }
static int decoder_set_main_decoder_property(int index) { return 0; }
static int decoder_get_downmix_mode(int index) { return 0; }
static int decoder_get_drc_mode(int index) { return 0; }
static int decoder_set_param(struct aud_block_info *b, char *fmt, ...) { return 0; }
static int mixer_set_pcmren_drc_mode(struct aud_block_info *b, int mode) { return 0; }
static int mixer_set_pcmren_downmix_mode(struct aud_block_info *b, int mode) { return 0; }
static int mixer_set_pcmren_metadata_type(struct aud_block_info *b, int type) { return 0; }
static bool sndout_get_atmos_encode_on(void) { return false; }
static bool acapture_is_encoder_connected(void) { return encoder_connected; }
static int input_set_hdmi(struct aud_block_info *b, bool forced_pcm) { b->forced_pcm = forced_pcm; return 0; }
static int input_set_tp(struct aud_block_info *b, enum audio_decoder_codec codec, bool clock_x4) {
    b->codec = codec; b->clock_x4 = clock_x4; return 0;
}
static enum audio_decoder_codec input_get_hdmi_codec(struct aud_block_info *b) { return b->codec; }
static int input_register_event(struct aud_block_info *b, enum input_evt event,
    int (*callback)(void *, int, void *), struct aud_block_info *target) {
    b->input_events |= 1u << event; return 0;
}
static int input_unregister_event(struct aud_block_info *b, enum input_evt event) {
    b->input_events &= ~(1u << event); return 0;
}
static int decoder_register_event(struct aud_block_info *b, enum audio_decoder_event event,
    int (*callback)(void *, int, void *), int repeat) {
    b->decoder_events |= 1u << event; return 0;
}
static int decoder_unregister_event(struct aud_block_info *b, enum audio_decoder_event event) {
    b->decoder_events &= ~(1u << event); return 0;
}
static int decoder_connect(struct aud_block_info *b) { return 0; }
static int input_start(struct aud_block_info *b) { b->running = true; return 0; }
static int input_stop(struct aud_block_info *b) { b->running = false; return 0; }
static int input_drop_data(struct aud_block_info *b, bool drop) { b->dropping = drop; return 0; }
static bool input_is_drop_data(struct aud_block_info *b) { return b->dropping; }
static int input_get_uid(struct aud_block_info *b) { return 0; }
static int aimc_set_property(int uid, const char *fmt, ...) { return 0; }
static int aud_hal_set_adc_port_number(int port) { return 0; }
static int input_tp_pdec_error_info_cb(void *param, int len, void *target) { return 0; }
static int decoder_decoded_info_cb(void *param, int len, void *target) { return 0; }
static int decoder_hdmi_pcm_format_info_cb(void *param, int len, void *target) { return 0; }
static int decoder_tp_es_exist_info_cb(void *param, int len, void *target) { return 0; }
static int renderer_master_info_cb(void *param, int len, void *target) { return 0; }
static int renderer_set_param(struct aud_block_info *b, char *fmt, ...) { return 0; }
static int renderer_lipsync_set_idle(struct aud_block_info *b) { return 0; }
static int renderer_lipsync_clear_idle(struct aud_block_info *b) { return 0; }
static int renderer_lipsync_set_idle_time(struct aud_block_info *b, unsigned int msec) {
    b->idle_time = msec; return 0;
}
static int renderer_register_event(struct aud_block_info *b, enum audio_renderer_event event,
    int (*callback)(void *, int, void *), int repeat) { return 0; }
static int renderer_unregister_event(struct aud_block_info *b, enum audio_renderer_event event) { return 0; }
static void renderer_set_basetime_ipc(unsigned int index, unsigned int clock_base_time,
    unsigned int stream_base_time, bool is_set, bool is_all_set) {}
// @match src/kernel/aud/alsa_decoder.c (?s)enum alsadec_codec\n\{.*?\};
// @match src/kernel/aud/alsa_decoder.c (?s)enum alsadec_input\n\{.*?\};
// @match src/kernel/aud/alsa_decoder.c (?s)static const int alsadec_metadata_type\[.*?\};
// @extract src/kernel/aud/alsa_decoder.c alsadec_codec_str alsadec_convert_codec alsa_decoder_codec
// @extract src/kernel/aud/alsa_decoder.c sndout_decoder_is_hdmi decoder_set_codec_param decoder_config alsa_decoder_start alsa_decoder_stop decoder_set_input_drop

static int put(int index, int codec) {
    struct snd_ctl_elem_value v = {.value.integer.value = {index, codec}};
    set_codec = port_out = 0;
    return alsa_decoder_codec(NULL, &v);
}
static int start(int index) {
    struct snd_ctl_elem_value value = {.value.integer.value = {index}};
    return alsa_decoder_start(NULL, &value);
}
static int stop(int index) {
    struct snd_ctl_elem_value value = {.value.integer.value = {index}};
    return alsa_decoder_stop(NULL, &value);
}
int main(void) {
    struct aud_block_info dec = {0}, ren = {0}, in = {INPUT_HDMI};
    alsa_decoder_info[0] = (struct sndout_decoder_state){.dec_info = &dec, .is_open = true};
    alsa_renderer_info[0] = (struct sndout_renderer_state){.ren_info = &ren, .is_open = true};
    alsa_input_info[0].input_info = &in;
    assert(put(2, ALSADEC_CODEC_AC3) == -1 && !alsa_decoder_lock.held);
    for (int c = ALSADEC_CODEC_DTS; c <= ALSADEC_CODEC_DTS_X_P2; c++) {
        gate = false;
        assert(put(0, c) == -1 && alsa_decoder_info[0].codec == 0 && set_codec == 0);
        gate = true;
        assert(put(0, c) == 0 && alsa_decoder_info[0].codec == c);
        assert(set_codec == AUDIO_DECODER_CODEC_AC3 && port_out == 0x100);
    }
    gate = false;
    assert(put(0, 0x36) == 0 && set_codec == AUDIO_DECODER_CODEC_AC3);
    assert(put(0, ALSADEC_CODEC_UNKNOWN) == -1);
    assert(put(0, ALSADEC_CODEC_TRUEHD) == 0 && set_codec == AUDIO_DECODER_CODEC_MAT);
    in.type = INPUT_TP;
    assert(put(0, ALSADEC_CODEC_EAC3) == 0 && set_codec == AUDIO_DECODER_CODEC_EAC3 && port_out == 1);
    in.type = INPUT_SIF;
    assert(put(0, ALSADEC_CODEC_AC3) == 0 && set_codec == 0 && alsa_decoder_info[0].codec == ALSADEC_CODEC_AC3);
    assert(put(0, ALSADEC_CODEC_SIF_A2) == 0 && set_codec == AUDIO_DECODER_CODEC_PCM);
    created = alsa_decoder_info[0].is_started = true;
    assert(put(0, ALSADEC_CODEC_PCM) == -1 && alsa_decoder_info[0].codec == ALSADEC_CODEC_PCM);
    alsa_renderer_info[0].is_open = false;
    assert(put(0, ALSADEC_CODEC_DTS) == -1 && alsa_decoder_info[0].codec == ALSADEC_CODEC_DTS);
    assert(!alsa_decoder_lock.held);

    alsa_renderer_info[0].is_open = true;
    alsa_decoder_info[0].is_started = created = false;
    alsa_decoder_info[0].input = ALSADEC_IN_HDMI_PORT0;
    in.type = INPUT_HDMI;
    in.codec = AUDIO_DECODER_CODEC_PCM;
    in.dropping = true;
    gate = false;
    assert(put(0, ALSADEC_CODEC_DTS) == -1 && start(0) == -1);
    assert(!alsa_decoder_info[0].is_started && !in.running && in.dropping);
    assert(in.input_events == 0 && dec.decoder_events == 0);
    assert(start(-1) == -1 && stop(2) == -1 && !alsa_decoder_lock.held);
    gate = true;
    for (int codec = ALSADEC_CODEC_DTS; codec <= ALSADEC_CODEC_DTS_X_P2; codec++) {
        assert(put(0, codec) == 0 && set_codec == AUDIO_DECODER_CODEC_AC3);
        assert(start(0) == 0 && alsa_decoder_info[0].is_started);
        assert(!in.running && in.dropping && !in.forced_pcm);
        assert(in.input_events == (1u << INPUT_EVT_HDMI_PCM_FORMAT));
        assert(dec.decoder_events == (1u << AUDIO_DECODER_EVENT_INFO));
        assert(start(0) == -1 && !alsa_decoder_lock.held);
        assert(stop(0) == 0 && !alsa_decoder_info[0].is_started);
        assert(!alsa_decoder_info[0].is_connected && in.input_events == 0 && dec.decoder_events == 0);
        assert(stop(0) == -1 && !alsa_decoder_lock.held);
    }
    assert(put(0, ALSADEC_CODEC_PCM) == 0 && start(0) == 0);
    assert(in.running && !in.dropping && in.forced_pcm);
    assert(stop(0) == 0 && !in.running && in.dropping);
    in.codec = AUDIO_DECODER_CODEC_EAC3;
    assert(put(0, ALSADEC_CODEC_EAC3) == 0 && start(0) == 0);
    assert(in.running && !in.dropping && !in.forced_pcm);
    assert(stop(0) == 0);

    in.type = INPUT_TP;
    alsa_decoder_info[0].input = ALSADEC_IN_ATP0;
    alsa_renderer_info[0].is_play_paused = true;
    assert(put(0, ALSADEC_CODEC_EAC3) == 0 && start(0) == 0);
    assert(in.running && !in.dropping && in.clock_x4 && in.codec == AUDIO_DECODER_CODEC_EAC3);
    assert(ren.idle_time == 300 && !alsa_renderer_info[0].is_play_paused);
    assert(in.input_events == ((1u << INPUT_EVT_TP_ES_EXIST_INFO) | (1u << INPUT_EVT_TP_PDEC_ERROR_INFO)));
    assert(stop(0) == 0 && in.input_events == 0 && !in.running && in.dropping);
    assert(put(0, ALSADEC_CODEC_DTS) == 0 && start(0) == 0);
    assert(in.running && !in.dropping && !in.clock_x4 && in.codec == AUDIO_DECODER_CODEC_AC3);
    assert(ren.idle_time == 60 && stop(0) == 0);

    main_audio_index = 0;
    alsa_decoder_info[0].is_main_dec = true;
    mutex_lock(&alsa_decoder_lock);
    decoder_set_input_drop(0, true);
    assert(in.running && !in.dropping);
    alsa_decoder_info[0].is_main_dec = false;
    alsa_decoder_info[0].unknown_54 = true;
    decoder_set_input_drop(0, true);
    assert(!in.dropping);
    alsa_decoder_info[0].unknown_54 = false;
    ad_on_off = true;
    decoder_set_input_drop(0, true);
    assert(!in.dropping);
    ad_on_off = false;
    decoder_set_input_drop(0, true);
    assert(in.dropping);
    decoder_set_input_drop(0, false);
    mutex_unlock(&alsa_decoder_lock);
    assert(!in.dropping && !alsa_decoder_lock.held);
    puts("PASS: C5 DTS gate, HDMI/TP start-stop lifecycle, pause idle and input-drop policy");
    return 0;
}
