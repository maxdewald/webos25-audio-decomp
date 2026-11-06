/* Output routing topology on the recovered kernel bodies. */
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define COMMON_HP 0x10
#define COMMON_ARC 0x20
#define COMMON_SPK 1
#define COMMON_OPTIC 2
#define COMMON_OPTIC_LG 4
#define COMMON_SE_ARC 0x200
#define COMMON_WOWCAST 0x800
#define ALSA_SNDOUT_INPUT_NUM 12
#define ALSA_ADEC_MAX 2
#define SNDOUT_CODEC_PCM 1
#define SNDOUT_CODEC_MPEG4_AAC 9
#define SNDOUT_CHNUM_8 8
#define SNDOUT_SAMPLERATE_192 192000
#define SNDOUT_MIXOPTION_ALL 1
#define SNDOUT_OUTPUT_SET_NONE 0
#define SNDOUT_OUTPUT_SET_USE 2
#define SNDOUT_ARC_PCM 1
#define BYPASS_PORT 0x100
#define DECODER_OUTPUT_MODE_MAIN 0
#define DECODER_OUTPUT_MODE_BYPASS 1
#define EINVAL 22
#define PLAY_SPEED_INDEX_MAX 2
#define SNDOUT_LOCK() ((void)0)
#define SNDOUT_UNLOCK() ((void)0)
#define info(...) ((void)0)
#define noti(...) ((void)0)
#define error(...) ((void)0)
#define debug(...) ((void)0)
#define warn(...) ((void)0)
#define debug_call() ((void)0)

enum output_codec_t { OUTPUT_CODEC_PCM, OUTPUT_CODEC_AC3, OUTPUT_CODEC_EAC3,
                      OUTPUT_CODEC_AAC, OUTPUT_CODEC_MAT };
enum output_type_t { OUTPUT_SPEAKER, OUTPUT_HP, OUTPUT_OPTIC, OUTPUT_ARC };
typedef int sndout_optic_mode_ext_type_t;
typedef int sndout_arc_mode_ext_type_t;
typedef unsigned int sndout_output_set_type_t;
typedef unsigned int sndout_output_codec_type_t;
typedef unsigned int sndout_output_channel_num_t;
typedef unsigned int sndout_output_sample_rate_t;
typedef unsigned int sndout_output_mix_option_t;
struct aud_block_info { int id; };
struct alsa_sndout_output_type {
    unsigned int set_type, codec_type, channel_num, sample_rate, mix_option;
};
struct sndout_output_format {
    enum output_codec_t output_codec;
    bool bypass, forced_5_1;
    int main_audio, channel;
};
struct alsa_sndout_list_output {
    bool mixer_connected, bypass_connected;
    int output;
    struct aud_block_info *output_info;
    struct aud_block_info *alsa_se;
    struct aud_block_info *encoder_info;
    bool encoder_connected, output_set;
    struct sndout_output_format set_output;
};
struct param { int gain, mute, delay; };
typedef struct {
    bool isOpen;
    struct aud_block_info *mixer_info;
    int opened_output;
    bool earc_on, wowcast_on, arc_on;
    int optic_output_type, arc_output_type;
    int main_audio_input, main_audio_port, bypass_master_index, ott, atmos_locking;
    bool eac3_atmos_encode_on;
    unsigned int mixer_connected_output;
    struct alsa_sndout_output_type earc, arc, optic, wowcast;
    struct param param_input[12];
} ALSA_SNDOUT_INFO_T;
struct snd_kcontrol { int unused; };
struct snd_ctl_elem_value { struct { struct { long value[5]; } integer; } value; };

static ALSA_SNDOUT_INFO_T alsa_sndout_info[2];
static bool _gNewInterfaceOpticArc;
static unsigned int play_speed[2] = {1000,1000};
static struct aud_block_info mixer = {10}, outputs[4], encoder = {20}, se = {30};
static struct alsa_sndout_list_output routes[6];
struct edge { struct aud_block_info *from, *to; int port, input; };
static struct edge edges[100];
static int edge_count, disconnect_count, reserved_count, notifications, format_calls;
static int mode_calls, mode_main, mode_bypass, delay_calls, mute_calls, earc_calls;
static int multi_value, multi_calls, get_format_calls, old_format_calls, old_optic_calls;
static int old_arc_mode, init_calls;
static bool input_connected, chosen_bypass;
static enum output_codec_t chosen_codec;
static struct alsa_sndout_output_type *last_format;
static int last_format_channel, last_format_rate;

static void _connect_pcm(struct alsa_sndout_list_output *);
static void _set_output(int, struct alsa_sndout_list_output *, enum output_codec_t, bool, bool);
static void _old_arc_connect_output(sndout_arc_mode_ext_type_t);
static void _old_connect_output(struct alsa_sndout_output_type *, sndout_arc_mode_ext_type_t);
static void _connect_output(struct alsa_sndout_list_output *, struct alsa_sndout_output_type *);
static void _connect_output_all(void);
void sndout_connect_output(void);
void notifier_output_changed(void);
void notifier_input_changed(void);
unsigned int notifier_get_play_speed(int);

static struct aud_block_info *alsa_se_get_block(struct aud_block_info *block) { return block; }
static int common_input_convert_index(int input, int port) { (void)input; return port; }
static void decoder_set_multi_pcm_out(int index, bool value) { multi_value=value; multi_calls++; }
static void bm_connect_block(struct aud_block_info *from, int port,
                             struct aud_block_info *to, int input) {
    assert(edge_count < 100);
    edges[edge_count++] = (struct edge){from,to,port,input};
}
static void bm_disconnect_block(struct aud_block_info *block, int port) { disconnect_count++; }
static void bm_remove_reserved(struct aud_block_info *block, int port, bool out) { }
static int decoder_set_output_mode(int index, int mode, bool value) {
    mode_calls++;
    if (mode == DECODER_OUTPUT_MODE_MAIN) mode_main=value;
    else mode_bypass=value;
    return 0;
}
static int mixer_set_mute(struct aud_block_info *block, int port, bool value) { mute_calls++; return 0; }
static int mixer_set_delay(struct aud_block_info *block, int port, int value) { delay_calls++; return 0; }
static void _set_atmos_encode_on(int on) { }
static void output_set_format(struct aud_block_info *block, enum output_codec_t codec,
                              int channel, int rate, bool bypass) {
    format_calls++; last_format_channel=channel; last_format_rate=rate;
}
static bool _input_connected_any(int input, int port) { return input_connected; }
static void bm_connect_block_reserved_out(const char *name, int index, int port,
                                         struct aud_block_info *block, int input) {
    assert(port == BYPASS_PORT && input == BYPASS_PORT); reserved_count++;
}
static enum output_type_t common_output_convert_output(int output) { return OUTPUT_ARC; }
static struct aud_block_info *bm_create_block(const char *name, int index) { return &encoder; }
static void aud_encoder_set_codec(struct aud_block_info *block, enum output_codec_t codec, int channel) { }
static void aud_encoder_set_ch_lock_mode(struct aud_block_info *block, bool on) { }
static void aud_encoder_set_atmos_locking_mode(struct aud_block_info *block, bool on) { }
static struct alsa_sndout_list_output *output_get(int output) {
    for (int i=0; i<6; i++) if (routes[i].output == output) return &routes[i];
    return NULL;
}
static void _get_output_format(struct alsa_sndout_output_type *type,
                               enum output_codec_t *codec, bool *bypass) {
    last_format=type; get_format_calls++;
    *codec=chosen_codec; *bypass=chosen_bypass;
    if (notifier_get_play_speed(alsa_sndout_info[0].main_audio_port) != 1000) {
        *codec=OUTPUT_CODEC_PCM; *bypass=false;
    }
}
static void sndout_arc_convert_output_codec(int mode, enum output_codec_t *codec,
                                          bool *bypass, bool *forced) {
    old_format_calls++; old_arc_mode=mode;
    *codec=chosen_codec; *bypass=chosen_bypass; *forced=false;
}
static void _old_optic_connect_output(int mode) { old_optic_calls++; }
static void renderer_change_output(void) { notifications++; }
static int output_add(int output, bool mixed) { assert(output == COMMON_HP); return 0; }
static void sndout_init(void) { init_calls++; alsa_sndout_info[0].isOpen=true; }
static void output_set_earc_on(struct aud_block_info *block, bool on) { earc_calls++; }

// @extract src/kernel/aud/alsa_sndout.c _connect_pcm _set_output _connect_output _old_connect_output _old_arc_connect_output _connect_output_all sndout_connect_output default_hp_connection sndout_earc_onoff sndout_earc_output_type
// @extract src/kernel/aud/notifier.c notifier_output_changed notifier_input_changed notifier_set_play_speed notifier_get_play_speed

static void reset(void) {
    memset(alsa_sndout_info,0,sizeof(alsa_sndout_info));
    memset(routes,0,sizeof(routes));
    int identifiers[6]={COMMON_HP,COMMON_ARC,COMMON_SPK,COMMON_OPTIC,COMMON_OPTIC_LG,COMMON_SE_ARC};
    for(int i=0;i<6;i++) {
        routes[i].output=identifiers[i];
        routes[i].output_info=&outputs[i%4];
    }
    alsa_sndout_info[0].isOpen=true;
    alsa_sndout_info[0].mixer_info=&mixer;
    alsa_sndout_info[0].earc=(struct alsa_sndout_output_type){2,1,2,48000,0};
    edge_count=disconnect_count=reserved_count=notifications=format_calls=0;
    mode_calls=delay_calls=mute_calls=earc_calls=multi_calls=0;
    get_format_calls=old_format_calls=old_optic_calls=init_calls=0;
    mode_main=mode_bypass=multi_value=-1;
    _gNewInterfaceOpticArc=true;
    input_connected=true;
    chosen_codec=OUTPUT_CODEC_PCM; chosen_bypass=false;
    play_speed[0]=play_speed[1]=1000;
    last_format=NULL;
}

static void test_physical_pcm_ports(void) {
    for(int channels=0; channels<=8; channels++) for(int mix=0;mix<=1;mix++) {
        reset();
        ALSA_SNDOUT_INFO_T *s=&alsa_sndout_info[0];
        s->earc_on=true; s->earc.channel_num=channels; s->earc.mix_option=mix;
        _connect_pcm(&routes[1]);
        assert(edge_count==1 && edges[0].from==&mixer);
        assert(edges[0].port==((mix && (channels==6 || channels==8))?2:0));
        assert(multi_value==(mix && (channels==2 || channels==6 || channels==8)));
    }
    reset(); default_hp_connection();
    assert(alsa_sndout_info[0].opened_output==0);
    assert(edge_count==1 && edges[0].to==routes[0].output_info && edges[0].port==0);
    reset(); routes[2].alsa_se=&se; _connect_pcm(&routes[2]);
    assert(edge_count==3);
    assert(edges[0].port==0 && edges[0].to==&se);
    assert(edges[1].port==2 && edges[1].input==1);
    assert(edges[2].from==&se && edges[2].port==0);
}

static void test_bypass_and_simultaneous_pcm(void) {
    reset();
    ALSA_SNDOUT_INFO_T *s=&alsa_sndout_info[0];
    s->earc_on=true; s->mixer_connected_output=COMMON_SPK;
    _connect_pcm(&routes[2]);
    _set_output(0,&routes[1],OUTPUT_CODEC_PCM,true,false);
    assert(edge_count==2 && edges[1].port==BYPASS_PORT);
    assert(reserved_count==1 && mode_bypass==1);
    assert(s->mixer_connected_output==COMMON_SPK && mode_main==-1);
    int previous_edges=edge_count, previous_disconnects=disconnect_count;
    _set_output(0,&routes[1],OUTPUT_CODEC_PCM,true,false);
    assert(edge_count==previous_edges && disconnect_count==previous_disconnects);
    _set_output(1,&routes[1],OUTPUT_CODEC_PCM,true,false);
    assert(edge_count==previous_edges+1 && disconnect_count>previous_disconnects);
    reset(); input_connected=false;
    _set_output(0,&routes[1],OUTPUT_CODEC_AC3,true,false);
    assert(edges[0].port==BYPASS_PORT && reserved_count==0 && mode_bypass==-1);
}

static void test_mixed_reconnect_cache(void) {
    reset();
    ALSA_SNDOUT_INFO_T *s=&alsa_sndout_info[0]; s->earc_on=true;
    _set_output(0,&routes[1],OUTPUT_CODEC_PCM,false,false);
    assert(edge_count==1 && edges[0].port==0 && mode_main==1);
    int previous=edge_count;
    _set_output(1,&routes[1],OUTPUT_CODEC_PCM,false,false);
    assert(edge_count==previous); /* main index ignored for non-bypass cached route */
    s->earc.channel_num=8;
    _set_output(1,&routes[1],OUTPUT_CODEC_PCM,false,false);
    assert(edge_count==previous+1 && edges[previous].port==0); /* mix off */
    previous=edge_count;
    s->earc.mix_option=1; s->earc.sample_rate=192000;
    _set_output(1,&routes[1],OUTPUT_CODEC_PCM,false,false);
    assert(edge_count==previous); /* mix/rate absent from cache guard */
    routes[1].output_set=false;
    _set_output(1,&routes[1],OUTPUT_CODEC_PCM,false,false);
    assert(edge_count==previous+1 && edges[previous].port==2);
    reset(); _set_output(0,&routes[1],OUTPUT_CODEC_EAC3,false,false);
    assert(edge_count==2 && edges[0].port==1 && edges[1].from==&encoder);
    assert(last_format_channel==2 && last_format_rate==192000);
}

static void test_dispatch_and_speed(void) {
    reset(); ALSA_SNDOUT_INFO_T *s=&alsa_sndout_info[0];
    s->opened_output=COMMON_ARC|COMMON_OPTIC; s->earc_on=true;
    chosen_bypass=true; chosen_codec=OUTPUT_CODEC_AC3;
    sndout_connect_output();
    assert(get_format_calls==2 && last_format==&s->earc);
    assert(routes[1].set_output.bypass && routes[3].set_output.bypass);
    notifier_set_play_speed(0,500);
    assert(get_format_calls==4 && !routes[1].set_output.bypass);
    assert(routes[1].set_output.output_codec==OUTPUT_CODEC_PCM);
    int previous=get_format_calls;
    notifier_set_play_speed(0,500); notifier_set_play_speed(-1,1000); notifier_set_play_speed(2,1000);
    assert(get_format_calls==previous);
    notifier_input_changed(); assert(get_format_calls==previous+2);
    previous=get_format_calls;
    notifier_output_changed(); assert(notifications==1 && get_format_calls==previous);
    reset(); s=&alsa_sndout_info[0]; s->opened_output=COMMON_ARC; s->earc_on=true;
    _gNewInterfaceOpticArc=false; chosen_bypass=true; chosen_codec=OUTPUT_CODEC_AC3;
    s->earc.set_type=SNDOUT_OUTPUT_SET_NONE;
    notifier_set_play_speed(0,500);
    assert(old_format_calls==1 && get_format_calls==0 && routes[1].set_output.bypass);
    s->earc.set_type=SNDOUT_OUTPUT_SET_USE;
    sndout_connect_output(); assert(get_format_calls==1 && !routes[1].set_output.bypass);
    reset(); s=&alsa_sndout_info[0]; s->opened_output=COMMON_SE_ARC;
    chosen_bypass=true; chosen_codec=OUTPUT_CODEC_AC3;
    sndout_connect_output(); assert(!routes[5].set_output.bypass);
    assert(routes[5].set_output.output_codec==OUTPUT_CODEC_PCM);
}

static void test_earc_control_contract(void) {
    reset(); ALSA_SNDOUT_INFO_T *s=&alsa_sndout_info[0];
    struct snd_ctl_elem_value value={0}; value.value.integer.value[0]=1;
    assert(sndout_earc_onoff(NULL,&value)==0);
    assert(s->earc_on && notifications==1 && earc_calls==0 && get_format_calls==0);
    s->opened_output=COMMON_ARC;
    assert(sndout_earc_onoff(NULL,&value)==0);
    assert(notifications==2 && earc_calls==0); /* unchanged */
    value.value.integer.value[0]=0;
    assert(sndout_earc_onoff(NULL,&value)==0);
    assert(earc_calls==1 && get_format_calls==1 && last_format==&s->arc);
    value.value.integer.value[0]=2;
    assert(sndout_earc_onoff(NULL,&value)==-EINVAL && notifications==3);
    value=(struct snd_ctl_elem_value){.value.integer.value={2,1,8,192000,0}};
    assert(sndout_earc_output_type(NULL,&value)==0);
    assert(s->earc.channel_num==8 && notifications==4 && get_format_calls==1);
    s->earc_on=true;
    assert(sndout_earc_output_type(NULL,&value)==0);
    assert(get_format_calls==2 && notifications==5);
    value.value.integer.value[1]=10;
    assert(sndout_earc_output_type(NULL,&value)==-EINVAL && notifications==5);
    value.value.integer.value[1]=-1;
    assert(sndout_earc_output_type(NULL,&value)==-EINVAL && notifications==5);
}

int main(void) {
    test_physical_pcm_ports(); test_bypass_and_simultaneous_pcm();
    test_mixed_reconnect_cache(); test_dispatch_and_speed(); test_earc_control_contract();
    puts("output routing: physical ports, permanent HP, cache, legacy/speed and eARC controls passed");
    return 0;
}
