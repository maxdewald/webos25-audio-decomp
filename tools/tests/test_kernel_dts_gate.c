/* "Adec Codec" put: DTS gate rejection/reset, store-before-check, SIF and HDMI routing. */
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
struct aud_block_info { int type; };
enum audio_decoder_codec { AUDIO_DECODER_CODEC_PCM = 1, AUDIO_DECODER_CODEC_AC3,
    AUDIO_DECODER_CODEC_EAC3, AUDIO_DECODER_CODEC_AC4, AUDIO_DECODER_CODEC_MAT,
    AUDIO_DECODER_CODEC_TRUE_HD, AUDIO_DECODER_CODEC_AAC, AUDIO_DECODER_CODEC_MP3,
    AUDIO_DECODER_CODEC_MPEG_H, AUDIO_DECODER_CODEC_DRA };
struct sndout_decoder_state { struct aud_block_info *dec_info; bool is_open, is_started; int codec; };
struct sndout_input_state { struct aud_block_info *input_info; int index; };
struct sndout_renderer_state { struct aud_block_info *ren_info; bool is_open; };
static struct sndout_decoder_state alsa_decoder_info[2];
static struct sndout_input_state alsa_input_info[2];
static struct sndout_renderer_state alsa_renderer_info[2];
static struct mutex alsa_decoder_lock;
static bool gate, created;
static int set_codec, port_out;
static bool aud_hal_dts_supported(void) { return gate; }
static void ablind_post(const char *n, int c, int a, int b) {}
static bool decoder_is_created(struct aud_block_info *b) { return created; }
static int input_get_type(struct aud_block_info *b) { return b->type; }
static int decoder_set_codec(struct aud_block_info *b, enum audio_decoder_codec c) { set_codec = c; return 0; }
static int bm_connect_block(struct aud_block_info *o, int po, struct aud_block_info *i, int pi) { port_out = po; return 0; }
// @match src/kernel/aud/alsa_decoder.c (?s)enum alsadec_codec\n\{.*?\};
// @match src/kernel/aud/alsa_decoder.c #define ALSADEC_INPUT_\w+ +\d+
// @extract src/kernel/aud/alsa_decoder.c alsadec_codec_str alsadec_convert_codec alsa_decoder_codec

static int put(int index, int codec) {
    struct snd_ctl_elem_value v = {.value.integer.value = {index, codec}};
    set_codec = port_out = 0;
    return alsa_decoder_codec(NULL, &v);
}
int main(void) {
    struct aud_block_info dec = {0}, ren = {0}, in = {ALSADEC_INPUT_HDMI};
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
    in.type = 1;
    assert(put(0, ALSADEC_CODEC_EAC3) == 0 && set_codec == AUDIO_DECODER_CODEC_EAC3 && port_out == 1);
    in.type = ALSADEC_INPUT_SIF;
    assert(put(0, ALSADEC_CODEC_AC3) == 0 && set_codec == 0 && alsa_decoder_info[0].codec == ALSADEC_CODEC_AC3);
    assert(put(0, ALSADEC_CODEC_SIF_A2) == 0 && set_codec == AUDIO_DECODER_CODEC_PCM);
    created = alsa_decoder_info[0].is_started = true;
    assert(put(0, ALSADEC_CODEC_PCM) == -1 && alsa_decoder_info[0].codec == ALSADEC_CODEC_PCM);
    alsa_renderer_info[0].is_open = false;
    assert(put(0, ALSADEC_CODEC_DTS) == -1 && alsa_decoder_info[0].codec == ALSADEC_CODEC_DTS);
    assert(!alsa_decoder_lock.held);
    puts("PASS: Adec Codec DTS gate reject/reset, store-before-check, SIF/HDMI routing");
    return 0;
}
