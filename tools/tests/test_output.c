#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define info(...) ((void)0)
#define noti(...) ((void)0)
#define error(...) ((void)0)
#define debug_call() ((void)0)
#define OUTPUT_TO_STR(output) "mock"
#define EINVAL 22
#define ALSA_SNDOUT_INPUT_NUM 12
#define BYPASS_PORT 256
#define ADEC_CMD_START 0x10010000
#define ADEC_CMD_STOP 0x10020000
#define AUDIO_HAL_OUTPUT_PCM 0
#define AUDIO_HAL_OUTPUT_ES 1
#define DECODER_OUTPUT_MODE_MAIN 0
#define DECODER_OUTPUT_MODE_BYPASS 1
#define COMMON_HP 0x10
#define COMMON_ARC 0x20
#define COMMON_WOWCAST 0x800
#define SNDOUT_OUTPUT_SET_USE 2
#define SNDOUT_CODEC_MPEG4_AAC 9
#define SNDOUT_CHNUM_8 8
#define SNDOUT_SAMPLERATE_192 192000
#define SNDOUT_MIXOPTION_ALL 1
#define FW_AUD_OUTPUT_CODEC_MAT 4

enum output_type_t { OUTPUT_SPEAKER, OUTPUT_HP, OUTPUT_OPTIC, OUTPUT_ARC };
enum output_codec_t { OUTPUT_CODEC_PCM, OUTPUT_CODEC_AC3, OUTPUT_CODEC_EAC3,
                      OUTPUT_CODEC_AAC, OUTPUT_CODEC_MAT, OUTPUT_CODEC_MAX };
typedef unsigned int FW_AUD_OUTPUT_CODEC_T;
typedef unsigned int sndout_output_set_type_t;
typedef unsigned int sndout_output_codec_type_t;
typedef unsigned int sndout_output_channel_num_t;
typedef unsigned int sndout_output_sample_rate_t;
typedef unsigned int sndout_output_mix_option_t;
struct aud_block_info { int unused; };
struct aud_output_info {
    struct aud_block_info parent;
    enum output_type_t output_type;
    int uid_outctrl, uid_out;
    bool mute;
    unsigned int delay;
    enum output_codec_t codec;
    unsigned int channel, sample_rate, acmod, bypass;
    bool earc_on;
};
typedef struct { unsigned int codec, channel, sample_rate, acmod; } OutputEvtFirstAu;
struct sndout_output_format {
    enum output_codec_t output_codec;
    bool bypass, forced_5_1;
    int main_audio, channel;
};
struct alsa_sndout_list_output {
    bool mixer_connected, bypass_connected;
    unsigned int output;
    struct aud_block_info *output_info, *encoder_info;
    bool encoder_connected, output_set;
    struct sndout_output_format set_output;
};
struct alsa_sndout_output_type {
    unsigned int set_type, codec_type, channel_num, sample_rate, mix_option;
};
typedef struct {
    bool isOpen;
    struct aud_block_info *mixer_info;
    unsigned int mixer_connected_output, main_audio_input, main_audio_port, opened_output;
    int bypass_master_index, arc_output_type;
    bool earc_on, wowcast_on, eac3_atmos_encode_on, atmos_locking;
    struct alsa_sndout_output_type earc, wowcast;
    struct { int mute, delay; } param_input[12];
} ALSA_SNDOUT_INFO_T;
struct snd_kcontrol { int unused; };
struct snd_ctl_elem_value { struct { struct { long value[5]; } integer; } value; };
static ALSA_SNDOUT_INFO_T alsa_sndout_info[1];
static bool _gNewInterfaceOpticArc;
static struct aud_block_info mixer, outblock, encoder;
static struct alsa_sndout_list_output route;
static bool connected, create_encoder;
enum { PROPERTY, CLOCK, HAL_MUTE, HAL_CHANNEL, HAL_FORMAT, HAL_ON, COMMAND,
       FORMAT, CONNECT, DISCONNECT, REMOVE, MIX_MUTE, MIX_DELAY, MODE, LOCK,
       UNLOCK, PCM_CONNECT, CODEC, ATMOS, CH_LOCK, ATMOS_LOCK, CREATE,
       INIT, NEW_CONNECT, OLD_CONNECT, NOTIFY };
static struct { int id, a, b, c, d; } events[128];
static unsigned int count;
static void record(int id, int a, int b, int c, int d)
{
    assert(count < 128);
    events[count].id = id;
    events[count].a = a;
    events[count].b = b;
    events[count].c = c;
    events[count].d = d;
    count++;
}
#define R(id,a,b,c,d) record(id,a,b,c,d)
#define SNDOUT_LOCK() R(LOCK,0,0,0,0)
#define SNDOUT_UNLOCK() R(UNLOCK,0,0,0,0)
static int aimc_set_property(int uid, const char *text, ...)
{
    va_list ap;
    va_start(ap, text);
    int first = va_arg(ap, int), second = 0;
    bool mute = strstr(text, "mute-output") != NULL;
    if (mute) second = va_arg(ap, int);
    va_end(ap);
    R(PROPERTY, uid, mute, first, second);
    return -99;
}
static void aud_clock_update_output(int type, unsigned int rate, bool bypass)
{ R(CLOCK,type,rate,bypass,0); }
static void audio_hal_earc_set_mute(bool on) { R(HAL_MUTE,on,0,0,0); }
static void audio_hal_earc_set_channel(unsigned int channel, unsigned int acmod)
{ R(HAL_CHANNEL,channel,acmod,0,0); }
static void audio_hal_earc_set_output_format(int format, unsigned int rate)
{ R(HAL_FORMAT,format,rate,0,0); }
static void audio_hal_earc_set_on(bool on) { R(HAL_ON,on,0,0,0); }
static int aimc_send_command(void *owner, int cmd, int response, int uid, void *data, int size)
{ R(COMMAND,cmd,uid,0,0); return -99; }
static int output_set_format(struct aud_block_info *block, enum output_codec_t codec,
                            unsigned int channels, unsigned int rate, bool bypass)
{ R(FORMAT,codec,channels,rate,bypass); return -99; }
static int bm_connect_block(struct aud_block_info *from, int port,
                            struct aud_block_info *to, int to_port)
{ R(CONNECT,port,to == &encoder,to_port,0); return -99; }
static int bm_disconnect_block(struct aud_block_info *block, int port)
{ R(DISCONNECT,port,0,0,0); return -99; }
static int bm_remove_reserved(struct aud_block_info *block, int port, bool in)
{ R(REMOVE,port,in,0,0); return -99; }
static int bm_connect_block_reserved_out(const char *name, int index, int port,
                                        struct aud_block_info *to, int to_port)
{ R(CONNECT,port,index,to_port,1); return -99; }
static int mixer_set_mute(struct aud_block_info *block, int port, bool mute)
{ R(MIX_MUTE,port,mute,0,0); return -99; }
static int mixer_set_delay(struct aud_block_info *block, int port, int delay)
{ R(MIX_DELAY,port,delay,0,0); return -99; }
static int decoder_set_output_mode(int index, int mode, bool enable)
{ R(MODE,index,mode,enable,0); return -99; }
static void _set_atmos_encode_on(bool on) { R(ATMOS,on,0,0,0); }
static bool _input_connected_any(unsigned int input, unsigned int port) { return connected; }
static void _connect_pcm(struct alsa_sndout_list_output *output) { R(PCM_CONNECT,0,0,0,0); }
static enum output_type_t common_output_convert_output(unsigned int output) { return OUTPUT_ARC; }
static struct aud_block_info *bm_create_block(const char *name, int type)
{ R(CREATE,type,0,0,0); return create_encoder ? &encoder : NULL; }
static void aud_encoder_set_codec(struct aud_block_info *block, int codec, int channels)
{ R(CODEC,codec,channels,0,0); }
static void aud_encoder_set_ch_lock_mode(struct aud_block_info *block, bool on)
{ R(CH_LOCK,on,0,0,0); }
static void aud_encoder_set_atmos_locking_mode(struct aud_block_info *block, bool on)
{ R(ATMOS_LOCK,on,0,0,0); }
static struct alsa_sndout_list_output *output_get(unsigned int output) { return &route; }
static void sndout_init(void) { alsa_sndout_info[0].isOpen = true; R(INIT,0,0,0,0); }
static void _connect_output(struct alsa_sndout_list_output *output,
                            struct alsa_sndout_output_type *type) { R(NEW_CONNECT,0,0,0,0); }
static void _old_connect_output(struct alsa_sndout_output_type *type, int old)
{ R(OLD_CONNECT,0,0,0,0); }
static void notifier_output_changed(void) { R(NOTIFY,0,0,0,0); }

/* RECOVERED_FUNCTIONS */

static unsigned int occurrences(int id)
{
    unsigned int n = 0;
    for (unsigned int i = 0; i < count; i++) n += events[i].id == id;
    return n;
}
static int find(int id)
{
    for (unsigned int i = 0; i < count; i++) if (events[i].id == id) return i;
    return -1;
}
static void reset(void)
{
    memset(alsa_sndout_info, 0, sizeof(alsa_sndout_info));
    memset(&route, 0, sizeof(route));
    route.output = COMMON_ARC;
    route.output_info = &outblock;
    alsa_sndout_info[0].mixer_info = &mixer;
    alsa_sndout_info[0].earc.channel_num = 8;
    connected = false;
    create_encoder = false;
    _gNewInterfaceOpticArc = true;
    count = 0;
}
static void test_callbacks(void)
{
    struct aud_output_info out = { .output_type=OUTPUT_ARC, .uid_outctrl=7,
        .uid_out=8, .mute=true, .delay=40, .earc_on=true, .acmod=3 };
    reset();
    assert(output_set_mute(&out.parent, false) == 0);
    assert(!out.mute && count == 1 && events[0].a == 7 && events[0].b == 1);
    assert(events[0].c == 0 && events[0].d == 0);
    count = 0;
    output_pre_connect(&out.parent, 99, false);
    output_post_connect(&out.parent, 99, false);
    output_pre_disconnect(&out.parent, 99, false);
    output_post_disconnect(&out.parent, 99, false);
    assert(count == 0);
    output_pre_connect(&out.parent, 99, true);
    assert(count == 1 && events[0].id == COMMAND && events[0].a == ADEC_CMD_START);
    count = 0;
    output_pre_disconnect(&out.parent, 99, true);
    assert(count == 3 && events[0].id == PROPERTY && events[0].c == 1);
    assert(events[1].id == HAL_MUTE && events[1].a == 1);
    assert(events[2].id == PROPERTY && events[2].b == 0 && events[2].c == 0);
    assert(out.delay == 40 && !out.mute);
    count = 0;
    output_post_disconnect(&out.parent, 99, true);
    assert(count == 1 && events[0].a == ADEC_CMD_STOP);
    count = 0;
    output_post_connect(&out.parent, 99, true);
    assert(count == 3 && events[0].id == PROPERTY && events[0].c == 40);
    assert(events[1].id == HAL_MUTE && !events[1].a);
    assert(events[2].id == PROPERTY && !events[2].c);
    count = 0;
    _set_format(&out, OUTPUT_CODEC_MAT, 8, 7, 192000, true);
    assert(count == 6 && events[0].id == CLOCK && events[1].id == HAL_MUTE);
    assert(events[2].id == HAL_CHANNEL && events[2].a == 8 && events[2].b == 7);
    assert(events[3].id == HAL_FORMAT && events[3].a == AUDIO_HAL_OUTPUT_ES);
    assert(events[4].id == HAL_MUTE && events[4].a == 0 && events[5].id == HAL_ON);
    assert(out.codec == OUTPUT_CODEC_MAT && out.channel == 8 && out.bypass == 1);
    assert(out.acmod == 3);
    OutputEvtFirstAu evt = { OUTPUT_CODEC_AC3, 2, 48000, 7 };
    count = 0;
    assert(_cb_output_status(&evt, 0, &out) == 0);
    assert(out.acmod == 3 && occurrences(CLOCK) == 1);
    count = 0;
    assert(_cb_output_status(&evt, 0, &out) == 0);
    assert(count == 1 && events[0].id == HAL_ON);
    evt.codec = 5;
    count = 0;
    assert(_cb_output_status(&evt, 0, &out) == 0);
    assert(out.codec == OUTPUT_CODEC_PCM && out.acmod == 7);
    assert(events[find(HAL_FORMAT)].a == AUDIO_HAL_OUTPUT_PCM);
    out.output_type = OUTPUT_OPTIC;
    count = 0;
    assert(_cb_output_status(&evt, 0, &out) == 0 && count == 0);
    out.output_type = OUTPUT_HP;
    count = 0;
    assert(_cb_output_status(&evt, 0, &out) == -1 && count == 0);
}
static void test_routing(void)
{
    reset();
    _set_output(3, &route, OUTPUT_CODEC_EAC3, true, false);
    assert(route.bypass_connected && !route.encoder_connected);
    assert(events[find(FORMAT)].c == 192000 && events[find(FORMAT)].d == 1);
    assert(occurrences(CONNECT) == 1 && occurrences(MODE) == 1);
    assert(events[find(MODE)].b == DECODER_OUTPUT_MODE_MAIN && !events[find(MODE)].c);
    reset();
    connected = true;
    alsa_sndout_info[0].param_input[3].delay = 17;
    alsa_sndout_info[0].param_input[3].mute = 1;
    _set_output(3, &route, OUTPUT_CODEC_MAT, true, false);
    assert(events[find(FORMAT)].b == 8 && events[find(FORMAT)].c == 192000);
    assert(occurrences(CONNECT) == 2 && occurrences(MODE) == 2);
    assert(events[find(MIX_DELAY)].b == 17 && events[find(MIX_MUTE)].b == 1);
    assert(events[count-2].id == MODE && events[count-2].b == 1 && events[count-2].c == 1);
    assert(events[count-1].id == LOCK && alsa_sndout_info[0].bypass_master_index == 3);
    count = 0;
    _set_output(3, &route, OUTPUT_CODEC_MAT, true, false);
    assert(count == 0);
    _set_output(3, &route, OUTPUT_CODEC_PCM, false, false);
    assert(!route.bypass_connected && route.set_output.output_codec == OUTPUT_CODEC_PCM);
    assert(occurrences(REMOVE) == 1 && occurrences(PCM_CONNECT) == 1);
    assert(alsa_sndout_info[0].mixer_connected_output == COMMON_ARC);
    reset();
    _set_output(0, &route, OUTPUT_CODEC_EAC3, false, true);
    assert(!route.encoder_connected && occurrences(CREATE) == 1 && occurrences(CONNECT) == 0);
    assert(alsa_sndout_info[0].mixer_connected_output == COMMON_ARC);
    assert(events[count-1].id == MODE && events[count-1].c == 1);
    reset();
    alsa_sndout_info[0].earc_on = true;
    _set_output(0, &route, OUTPUT_CODEC_PCM, false, false);
    assert(route.set_output.channel == 8);
    count = 0;
    _set_output(1, &route, OUTPUT_CODEC_PCM, false, false);
    assert(count == 0);
    alsa_sndout_info[0].earc.channel_num = 2;
    _set_output(0, &route, OUTPUT_CODEC_PCM, false, false);
    assert(route.set_output.channel == 2 && occurrences(PCM_CONNECT) == 1);
    reset();
    route.output = COMMON_WOWCAST;
    alsa_sndout_info[0].wowcast_on = true;
    alsa_sndout_info[0].wowcast.channel_num = 6;
    _set_output(0, &route, OUTPUT_CODEC_PCM, false, false);
    assert(route.set_output.channel == 6);
    reset();
    connected = true;
    alsa_sndout_info[0].param_input[0].delay = 29;
    alsa_sndout_info[0].mixer_connected_output = COMMON_HP | COMMON_ARC;
    _set_output(-1, &route, OUTPUT_CODEC_AC3, true, false);
    assert(events[find(MIX_DELAY)].b == 29);
    assert(occurrences(MODE) == 1 && events[find(MODE)].b == DECODER_OUTPUT_MODE_BYPASS);
    assert(alsa_sndout_info[0].mixer_connected_output == COMMON_HP);
    reset();
    create_encoder = true;
    _set_output(0, &route, OUTPUT_CODEC_MAT, false, true);
    assert(route.encoder_connected && occurrences(CONNECT) == 2);
    assert(events[find(CODEC)].b == 8 && events[find(CH_LOCK)].a == 1);
    reset();
    route.output = COMMON_HP;
    _set_output(0, &route, OUTPUT_CODEC_PCM, false, false);
    assert(occurrences(DISCONNECT) == 0 && occurrences(PCM_CONNECT) == 0);
}
static void test_earc_type(void)
{
    static const long limits[5] = { 2, 9, 8, 192000, 1 };
    struct snd_ctl_elem_value control = {0};
    for (int field = 0; field < 5; field++) {
        reset();
        control = (struct snd_ctl_elem_value){0};
        control.value.integer.value[field] = limits[field] + 1;
        assert(sndout_earc_output_type(NULL, &control) == -EINVAL && count == 0);
        control.value.integer.value[field] = -1;
        assert(sndout_earc_output_type(NULL, &control) == -EINVAL && count == 0);
    }
    reset();
    for (int i = 0; i < 5; i++) control.value.integer.value[i] = limits[i];
    assert(sndout_earc_output_type(NULL, &control) == 0);
    assert(count == 4 && events[0].id == LOCK && events[1].id == INIT);
    assert(events[2].id == UNLOCK && events[3].id == NOTIFY);
    assert(alsa_sndout_info[0].earc.codec_type == 9);
    assert(alsa_sndout_info[0].earc.sample_rate == 192000);
    count = 0;
    alsa_sndout_info[0].opened_output = COMMON_ARC;
    alsa_sndout_info[0].earc_on = true;
    assert(sndout_earc_output_type(NULL, &control) == 0);
    assert(count == 4 && events[1].id == NEW_CONNECT);
    count = 0;
    _gNewInterfaceOpticArc = false;
    assert(sndout_earc_output_type(NULL, &control) == 0);
    assert(count == 4 && events[1].id == OLD_CONNECT);
    count = 0;
    control.value.integer.value[1] = 0x100000009L;
    assert(sndout_earc_output_type(NULL, &control) == 0);
    assert(alsa_sndout_info[0].earc.codec_type == 9);
}
int main(void)
{
    test_callbacks();
    test_routing();
    test_earc_type();
    puts("PASS: output callbacks/format/mute, routing gates/failures, eARC validation/state/notify");
    return 0;
}
