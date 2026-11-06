/* Actual source format predicates. */
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define debug(...) ((void)0)
#define error(...) ((void)0)
#define SNDOUT_OUTPUT_SET_USE 2
#define AUDIO_DECODER_INPUT_NONE 0

enum output_codec_t { OUTPUT_CODEC_PCM, OUTPUT_CODEC_AC3, OUTPUT_CODEC_EAC3,
                      OUTPUT_CODEC_AAC, OUTPUT_CODEC_MAT };
enum sndout_input_codec {
    SNDOUT_INPUT_CODEC_AAC, SNDOUT_INPUT_CODEC_AC3, SNDOUT_INPUT_CODEC_EAC3,
    SNDOUT_INPUT_CODEC_HEAAC, SNDOUT_INPUT_CODEC_AC4, SNDOUT_INPUT_CODEC_MAT,
    SNDOUT_INPUT_CODEC_MPEG_H, SNDOUT_INPUT_CODEC_OPUS, SNDOUT_INPUT_CODEC_MPEG4_AAC,
    SNDOUT_INPUT_CODEC_OTHERS,
};
enum sndout_input { SNDOUT_INPUT_DTV, SNDOUT_INPUT_HDMI, SNDOUT_INPUT_SYSTEM, SNDOUT_INPUT_OTHERS };
enum audio_decoder_input { INPUT_NONE, INPUT_DTV, INPUT_HDMI, INPUT_SYSTEM };
enum audio_decoder_codec {
    AUDIO_DECODER_CODEC_PCM, AUDIO_DECODER_CODEC_AC3, AUDIO_DECODER_CODEC_EAC3,
    AUDIO_DECODER_CODEC_MAT, AUDIO_DECODER_CODEC_TRUE_HD, AUDIO_DECODER_CODEC_AAC,
    AUDIO_DECODER_CODEC_HEAAC,
};
typedef enum {
    SNDOUT_CODEC_PCM=1, SNDOUT_CODEC_AC3, SNDOUT_CODEC_AAC, SNDOUT_CODEC_DTS,
    SNDOUT_CODEC_EAC3, SNDOUT_CODEC_TRUEHD, SNDOUT_CODEC_MAT2PCM, SNDOUT_CODEC_DTSHD,
    SNDOUT_CODEC_MPEG4_AAC,
} sndout_output_codec_type_t;
struct aud_block_info { int unused; };
struct decoded_info { enum audio_decoder_codec codec; };
struct alsa_sndout_output_type { int set_type; sndout_output_codec_type_t codec_type;
                               int channel_num, sample_rate, mix_option; };
typedef struct {
    int main_audio_input, main_audio_port;
    bool earc_on, wowcast_on;
} ALSA_SNDOUT_INFO_T;
static ALSA_SNDOUT_INFO_T alsa_sndout_info[2];
static struct aud_block_info block;
static bool have_decoder, have_renderer, have_input, pcm_bypass, jp4k;
static enum sndout_input_codec input_codec;
static enum audio_decoder_codec decoded_codec;
static enum audio_decoder_input decoded_input;
static unsigned int speed;
static int common_input_convert_index(int input, int port) { return port; }
static struct aud_block_info *bm_get_block_info_by_index(const char *name, int index) {
    if (name[0]=='d') return have_decoder?&block:NULL;
    if (name[0]=='r') return have_renderer?&block:NULL;
    return have_input?&block:NULL;
}
static void decoder_get_decoded_info(struct aud_block_info *b, struct decoded_info *out) {
    out->codec=decoded_codec;
}
static enum audio_decoder_input decoder_get_input(struct aud_block_info *b) { return decoded_input; }
static enum sndout_input _decoder_input_convert_input(enum audio_decoder_input input) {
    if(input==INPUT_HDMI) return SNDOUT_INPUT_HDMI;
    if(input==INPUT_SYSTEM) return SNDOUT_INPUT_SYSTEM;
    if(input==INPUT_DTV) return SNDOUT_INPUT_DTV;
    return SNDOUT_INPUT_OTHERS;
}
static enum audio_decoder_codec input_get_hdmi_codec(struct aud_block_info *b) { return decoded_codec; }
static enum sndout_input_codec _hdmi_convert_input_codec(enum audio_decoder_codec codec) { return input_codec; }
static enum sndout_input_codec _decoded_info_convert_input_codec(struct decoded_info *info) { return input_codec; }
static bool renderer_get_jp4k(struct aud_block_info *b) { return jp4k; }
static bool renderer_get_pcm_bypass(struct aud_block_info *b) { return pcm_bypass; }
static unsigned int notifier_get_play_speed(int index) { return index<2 && index>=0?speed:1000; }

// @extract src/kernel/aud/alsa_sndout.c sndout_codec_convert_output_codec input_codec_convert_output_codec input_codec_convert_sndout_codec _use_output _org_output _get_output_format_set_type _is_decoder_bypass _get_output_format

static void check(struct alsa_sndout_output_type *out, enum output_codec_t expected, bool bypass) {
    enum output_codec_t codec=OUTPUT_CODEC_MAT; bool actual=true;
    _get_output_format(out,&codec,&actual);
    assert(codec==expected && actual==bypass);
}
int main(void) {
    struct alsa_sndout_output_type out={2,SNDOUT_CODEC_AC3,2,48000,0};
    speed=1000;
    /* No decoder + no mix always coerces to PCM, not requested encoded codec. */
    check(&out,OUTPUT_CODEC_PCM,false);
    out.mix_option=1; check(&out,OUTPUT_CODEC_AC3,false);
    out.mix_option=0; have_renderer=true; pcm_bypass=true;
    check(&out,OUTPUT_CODEC_PCM,false);
    alsa_sndout_info[0].earc_on=true; check(&out,OUTPUT_CODEC_PCM,true);
    speed=500; check(&out,OUTPUT_CODEC_PCM,false);
    alsa_sndout_info[0].main_audio_port=4;
    check(&out,OUTPUT_CODEC_PCM,true); /* speed array covers decoder slots 0/1 only */
    alsa_sndout_info[0].main_audio_port=0; speed=1000; pcm_bypass=false;
    have_decoder=true; have_input=true; input_codec=SNDOUT_INPUT_CODEC_AC3;
    decoded_codec=AUDIO_DECODER_CODEC_AC3; decoded_input=INPUT_SYSTEM;
    check(&out,OUTPUT_CODEC_AC3,true);
    out.codec_type=SNDOUT_CODEC_EAC3; check(&out,OUTPUT_CODEC_PCM,false);
    out.set_type=1; check(&out,OUTPUT_CODEC_AC3,true);
    out.mix_option=1; check(&out,OUTPUT_CODEC_AC3,false);
    out.set_type=2; check(&out,OUTPUT_CODEC_EAC3,false);
    out.mix_option=0; input_codec=SNDOUT_INPUT_CODEC_OTHERS;
    decoded_codec=AUDIO_DECODER_CODEC_PCM; decoded_input=INPUT_HDMI;
    check(&out,OUTPUT_CODEC_PCM,true); /* eARC HDMI PCM exception */
    alsa_sndout_info[0].earc_on=false; check(&out,OUTPUT_CODEC_PCM,false);
    alsa_sndout_info[0].wowcast_on=true; check(&out,OUTPUT_CODEC_PCM,true);
    alsa_sndout_info[0].wowcast_on=false; decoded_input=INPUT_SYSTEM;
    input_codec=SNDOUT_INPUT_CODEC_HEAAC; decoded_codec=AUDIO_DECODER_CODEC_HEAAC;
    check(&out,OUTPUT_CODEC_PCM,false);
    input_codec=SNDOUT_INPUT_CODEC_AAC; decoded_codec=AUDIO_DECODER_CODEC_AAC;
    out.codec_type=SNDOUT_CODEC_MPEG4_AAC;
    check(&out,OUTPUT_CODEC_AAC,true);
    out.mix_option=1; check(&out,OUTPUT_CODEC_PCM,false);
    jp4k=true; check(&out,OUTPUT_CODEC_PCM,false);
    out.mix_option=0; check(&out,OUTPUT_CODEC_AAC,true);
    out.codec_type=SNDOUT_CODEC_AAC; check(&out,OUTPUT_CODEC_AAC,false);
    input_codec=SNDOUT_INPUT_CODEC_MAT; decoded_codec=AUDIO_DECODER_CODEC_MAT; jp4k=false;
    out.codec_type=SNDOUT_CODEC_TRUEHD; check(&out,OUTPUT_CODEC_MAT,true);
    out.codec_type=SNDOUT_CODEC_MAT2PCM; check(&out,OUTPUT_CODEC_MAT,true);
    speed=1500; check(&out,OUTPUT_CODEC_PCM,false);
    puts("output format: no-decoder, mix, speed, HDMI PCM, AAC and MAT predicates passed");
}
