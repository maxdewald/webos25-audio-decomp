/* Behavioral extraction tests; the kernel/DSP boundary is recorded, not modelled. */
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#define noti(...) ((void)0)
#define error(...) ((void)0)
struct aud_block_info { unsigned char opaque[0x10]; };
enum input_type { INPUT_HDMI=2 };
enum audio_decoder_codec { CODEC_PCM=1 };
struct list_head { void *next,*prev; };
// @match src/kernel/aud/input.c struct aud_input_info\n\{[\s\S]*?\n\};
enum { HDMI_DATA_DROP_MODE_NONE=0, HDMI_DATA_DROP_MODE_MAIN_PATH_DROP=1,
       HDMI_DATA_DROP_MODE_SUB_PATH_DROP=2 };
/* ABI structures for the HAL call are extracted with the implementation. */
// @match src/kernel/aud/input.c struct recovered_hdmi_(?:mute|audio_info)\n\{[\s\S]*?\n\};
struct event { int uid; char text[96]; } events[64];
static int n_events, hal_type=202, hal_freq=192000, hal_channels=8;
static int hal_error, property_error, hal_phase;
static bool hal_writes=true;
static struct aud_input_info *observed;
static int expected_mode, expected_kernel_codec, old_channel;
static bool old_forced;
static int aimc_set_property(int uid, const char *fmt, ...) {
    assert(n_events < 64);
    events[n_events].uid=uid;
    va_list ap; va_start(ap,fmt);
    vsnprintf(events[n_events].text,96,fmt,ap); va_end(ap);
    if (observed) {
        assert(hal_phase==2);
        /* Native stores follow the calls, not the success/failure results. */
        assert(observed->org_hdmi_freq==(unsigned)(hal_writes ? hal_freq : 0));
        assert(observed->org_hdmi_ch==(unsigned)(hal_writes ? hal_channels : 0));
        assert(observed->forced_pcm==old_forced);
        if (!strcmp(fmt,"hdmi-port=%d")) {
            assert(observed->sampling_freq==44100);
            assert(observed->channel_num==(unsigned)old_channel);
        } else if (!strcmp(fmt,"mode=%d")) {
            assert(observed->hdmi_codec==99); /* updated AFTER mode write */
            char text[32]; snprintf(text,sizeof(text),"mode=%d",expected_mode);
            assert(!strcmp(text,events[n_events].text));
        } else if (!strcmp(fmt,"codec=%d")) {
            assert((int)observed->hdmi_codec==expected_kernel_codec);
        }
    }
    n_events++;
    return property_error;
}
static int KDRV_HDMI21_HAL_Rx_Set_AudioMute(int port, void *arg, int size) {
    struct recovered_hdmi_mute *m=arg;
    assert(size==16 && port==3 && m->port==3 && m->mute==0 && m->mode==2);
    assert(m->unrecovered_08==0);
    assert(n_events==0 && hal_phase==0); /* HAL unmute precedes configuration */
    hal_phase=1;
    return hal_error;
}
static int KDRV_HDMI21_HAL_Rx_GetAudioInfo(int port, void *arg, int size) {
    struct recovered_hdmi_audio_info *i=arg;
    assert(size==40 && port==3 && i->port==3);
    assert(hal_phase==1 && n_events==0);
    assert(i->type==0 && i->sampling_freq==0 && i->channels==0);
    for (unsigned n=0;n<sizeof(i->unrecovered_10);n++) assert(i->unrecovered_10[n]==0);
    if (hal_writes) { i->type=hal_type; i->sampling_freq=hal_freq; i->channels=hal_channels; }
    hal_phase=2;
    return hal_error;
}
// @extract src/kernel/aud/input.c input_set_hdmi input_get_hdmi_codec input_set_hdmi_bypass
static void expect(int i,int uid,const char *text) {
    assert(i<n_events); assert(events[i].uid==uid);
    if(strcmp(events[i].text,text)) fprintf(stderr,"%d: %s != %s\n",i,events[i].text,text);
    assert(!strcmp(events[i].text,text));
}
static struct aud_input_info fresh_input(void) {
    return (struct aud_input_info){.index=3,.uid=71,.uid_queue=72,.uid_queue_bypass=73,
        .sampling_freq=44100,.channel_num=6,.hdmi_codec=99,.data_drop=true,.output_port=1};
}
static void reset_calls(void) { n_events=0; hal_phase=0; observed=NULL; }
static void native_hdmi_dts_hd_is_not_hbr(void) {
    struct aud_input_info in=fresh_input();
    struct aud_input_info untouched=fresh_input();
    hal_type=202; hal_freq=192000; hal_channels=8;
    hal_error=-5; property_error=-12; /* shipped ignores BOTH errors */
    reset_calls();
    assert(input_set_hdmi(&in.parent,false)==0);
    assert(n_events==4);
    expect(0,71,"hdmi-port=3"); expect(1,71,"sampling-freq=192000");
    expect(2,71,"mode=1"); expect(3,71,"codec=0");
    assert(input_get_hdmi_codec(&in.parent)==1); /* kernel PCM, not DSP PCM=0 */
    assert(in.org_hdmi_freq==192000 && in.org_hdmi_ch==8);
    assert(in.channel_num==6 && in.sampling_freq==192000 && !in.forced_pcm);
    assert(in.data_drop && in.output_port==1); /* codec setup does not select bypass */
    assert(untouched.sampling_freq==44100 && untouched.channel_num==6);
}
// @extract src/kernel/aud/alsa_decoder.c alsa_decoder_kernel_codec
static void usercodec_dts_does_not_become_a_dts_decoder(void) {
    for (int codec=48; codec<=53; codec++) assert(alsa_decoder_kernel_codec(codec)==2);
    assert(alsa_decoder_kernel_codec(21)==5); /* ALSA MAT -> kernel MAT */
    assert(alsa_decoder_kernel_codec(1)==1);
    assert(alsa_decoder_kernel_codec(0)==2 && alsa_decoder_kernel_codec(-1)==2);
    assert(alsa_decoder_kernel_codec(66)==1 && alsa_decoder_kernel_codec(67)==2);
}
static void usercodec_conversion_ranges(void) {
    /* Complete valid ALSA range: kernel enums, NOT the input DSP's codec field. */
    const int expected[66]={
        1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        3,3,4,4,5,5,5,5,7,7,2,2,2,2,2,8,9,10,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1
    };
    for (int codec=1;codec<=66;codec++) assert((int)alsa_decoder_kernel_codec(codec)==expected[codec-1]);
    assert(alsa_decoder_kernel_codec(INT_MIN)==2);
    assert(alsa_decoder_kernel_codec(INT_MAX)==2);
}
static void hal_type_mapping_and_store_order(void) {
    const struct { int type,mode,dsp,kernel; bool queues; } cases[]={
        {3,1,1,2,false},{5,1,3,7,false},{14,1,2,3,true},{16,4,4,5,true},
        {200,6,0,1,false},{201,6,0,1,false},{1003,6,1,2,false},
        {1005,6,3,7,false},{1014,6,2,3,true},{1016,7,4,5,true},
        {7,1,0,1,false},{202,1,0,1,false},{1004,1,0,1,false},
        {1202,1,0,1,false},{0,1,0,1,false},{INT_MIN,1,0,1,false},
        {INT_MAX,1,0,1,false},{199,1,0,1,false},{203,1,0,1,false}
    };
    hal_writes=true; hal_error=-5; property_error=-12;
    hal_freq=192000; hal_channels=8;
    for (unsigned n=0;n<sizeof(cases)/sizeof(cases[0]);n++) {
        struct aud_input_info in=fresh_input();
        in.forced_pcm=true;
        hal_type=cases[n].type;
        reset_calls(); observed=&in; old_forced=true; old_channel=6;
        expected_mode=cases[n].mode; expected_kernel_codec=cases[n].kernel;
        assert(input_set_hdmi(&in.parent,false)==0);
        observed=NULL;
        assert(n_events==(cases[n].queues ? 6 : 4));
        expect(0,71,"hdmi-port=3"); expect(1,71,"sampling-freq=192000");
        char text[32]; snprintf(text,sizeof(text),"mode=%d",cases[n].mode); expect(2,71,text);
        snprintf(text,sizeof(text),"codec=%d",cases[n].dsp); expect(3,71,text);
        if (cases[n].queues) {
            expect(4,72,"max-length=131072"); expect(5,73,"max-length=131072");
        }
        assert((int)in.hdmi_codec==cases[n].kernel && in.channel_num==6);
        assert(!in.forced_pcm && in.data_drop && in.output_port==1);
    }
}
static void pcm_channels_rates_and_forced_pcm(void) {
    const int channels[]={-1,0,1,2,3,4,5,6,7,8,9};
    const int rounded[]={2,2,2,2,4,4,6,6,8,8,2};
    const int rates[]={INT_MIN,-1,0,31999,32000,48000,48001,192000,192001,INT_MAX};
    hal_writes=true; hal_error=property_error=0; hal_type=2;
    for (unsigned c=0;c<sizeof(channels)/sizeof(channels[0]);c++) {
        for (unsigned r=0;r<sizeof(rates)/sizeof(rates[0]);r++) {
            struct aud_input_info in=fresh_input();
            reset_calls();
            hal_channels=channels[c]; hal_freq=rates[r];
            int freq=hal_freq>=32000 && hal_freq<=192000 ? hal_freq : 48000;
            bool queues=hal_freq>48000 || rounded[c]>2; /* original rate, not clamped */
            assert(input_set_hdmi(&in.parent,false)==0);
            assert(n_events==(queues ? 7 : 5));
            expect(0,71,"hdmi-port=3");
            char text[32]; snprintf(text,sizeof(text),"sampling-freq=%d",freq); expect(1,71,text);
            snprintf(text,sizeof(text),"channel-num=%d",rounded[c]); expect(2,71,text);
            expect(3,71,"mode=0"); expect(4,71,"codec=0");
            if (queues) { expect(5,72,"max-length=131072"); expect(6,73,"max-length=131072"); }
            assert(in.sampling_freq==(unsigned)freq && in.channel_num==(unsigned)rounded[c]);
            assert(in.org_hdmi_freq==(unsigned)hal_freq && in.org_hdmi_ch==(unsigned)hal_channels);
        }
    }
    /* 'forced_pcm' skips the PCM branch and leaves the channel count alone. */
    struct aud_input_info in=fresh_input();
    reset_calls(); hal_freq=192000; hal_channels=8;
    assert(input_set_hdmi(&in.parent,true)==0 && in.forced_pcm);
    assert(n_events==4 && in.channel_num==6 && in.hdmi_codec==1);
    expect(2,71,"mode=1"); expect(3,71,"codec=0");
    /* Nor does forcing PCM turn MAT into PCM. */
    reset_calls(); hal_type=16;
    assert(input_set_hdmi(&in.parent,true)==0 && in.hdmi_codec==5);
    assert(n_events==6); expect(2,71,"mode=4"); expect(3,71,"codec=4");
    /* A later stereo setup does not shrink previously enlarged queue limits. */
    reset_calls(); hal_type=2; hal_freq=48000; hal_channels=2;
    assert(input_set_hdmi(&in.parent,false)==0 && n_events==5);
    expect(3,71,"mode=0"); expect(4,71,"codec=0");
}
static void failed_hal_without_output_still_configures_defaults(void) {
    struct aud_input_info in=fresh_input();
    reset_calls(); hal_writes=false; hal_error=-5; property_error=-12;
    observed=&in; old_forced=false; old_channel=6; expected_mode=1; expected_kernel_codec=1;
    assert(input_set_hdmi(&in.parent,true)==0);
    observed=NULL;
    assert(in.org_hdmi_freq==0 && in.org_hdmi_ch==0 && in.sampling_freq==48000);
    assert(in.channel_num==6 && in.hdmi_codec==1 && in.forced_pcm && n_events==4);
    expect(1,71,"sampling-freq=48000"); expect(2,71,"mode=1"); expect(3,71,"codec=0");
    hal_writes=true;
}
int main(void) {
    native_hdmi_dts_hd_is_not_hbr();
    usercodec_dts_does_not_become_a_dts_decoder();
    usercodec_conversion_ranges();
    hal_type_mapping_and_store_order();
    pcm_channels_rates_and_forced_pcm();
    failed_hal_without_output_still_configures_defaults();
    puts("kernel HDMI setup: codec spaces, configuration/store order, PCM boundaries and ignored errors passed");
}
