/* Host model tests, not DSP execution or an oracle for unresolved RFT math. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../src/dsp2/pcmcvt.c"

struct mock_src { int input_rate, output_rate, bits, channels, mode, filter, frames; };
static struct mock_src source;
static int applied, rft_calls, src_calls, dropped, flushed, pushed, synced;
static int rft_in_bits, rft_out_bits, rft_in_ch, rft_out_ch, rft_frames;
static int rft_sign, rft_endian, rft_gain, rft_gain_enable, rft_acmod;
static struct aud_data *last_data;
static struct aud_event *last_event;
static int event_rft_bits, event_rft_acmod;
static bool fail_dup;
struct property { const char *key; int value; };
struct mock_event { struct aud_event base; struct property values[16]; unsigned count; };
static struct mock_event copied;
static unsigned char input_bytes[16384], converted[32768], resampled[32768];
void *pcmcvt_impl_descriptor;
void *lgsrc_create(void) { return &source; }
int lgsrc_init(void *s) { (void)s; return 0; }
void lgsrc_Destroy(void *s) { (void)s; }
int lgsrc_setInputFs(void *s, int v) { ((struct mock_src *)s)->input_rate=v; return 0; }
int lgsrc_setOutputFs(void *s, int v) { ((struct mock_src *)s)->output_rate=v; return 0; }
int lgsrc_setInOutPCMFormat(void *s, int v) { ((struct mock_src *)s)->bits=v; return 0; }
int lgsrc_setInOutChannel(void *s, int v) { ((struct mock_src *)s)->channels=v; return 0; }
int lgsrc_setRunMode(void *s, int v) { ((struct mock_src *)s)->mode=v; ++applied; return 0; }
int lgsrc_setFilterType(void *s, int v) { ((struct mock_src *)s)->filter=v; return 0; }
int lgsrc_setOutSample(void *s, unsigned v)
{
    if (v>512) return 1;
    ((struct mock_src *)s)->frames=(int)v;
    return 0;
}
int lgsrc_getInChannel(void *s) { return ((struct mock_src *)s)->channels; }
int lgsrc_getOutChannel(void *s) { return ((struct mock_src *)s)->channels; }
int lgsrc_getInputFs(void *s) { return ((struct mock_src *)s)->input_rate; }
int lgsrc_getOutputFs(void *s) { return ((struct mock_src *)s)->output_rate; }
int lgsrc_getInPCMFormat(void *s) { return ((struct mock_src *)s)->bits; }
int lgsrc_getOutPCMFormat(void *s) { return ((struct mock_src *)s)->bits; }
int lgsrc_getOutSampleNum(void *s) { return ((struct mock_src *)s)->frames; }
int lgsrc_getNeedfulSamplePer1Ch(void *s) { return ((struct mock_src *)s)->frames; }
int lgsrc_process(void *s, const void *in, void *out)
{
    struct mock_src *v=s;
    ++src_calls;
    memcpy(out,in,(unsigned)(v->frames*v->channels*(v->bits/8)));
    return v->frames*v->channels;
}
#ifndef PCMCVT_REAL_RFT
#define RFT_SETTER(name, target) int name(void *r, int v) { (void)r; target=v; return 0; }
RFT_SETTER(RFT_SetInPCMFormat, rft_in_bits)
RFT_SETTER(RFT_SetOutPCMFormat, rft_out_bits)
RFT_SETTER(RFT_SetInChannel, rft_in_ch)
RFT_SETTER(RFT_SetOutChannel, rft_out_ch)
RFT_SETTER(RFT_SetInSigned, rft_sign)
RFT_SETTER(RFT_SetInEndian, rft_endian)
RFT_SETTER(RFT_SetAcmod, rft_acmod)
RFT_SETTER(RFT_SetOutSample, rft_frames)
int RFT_SetGenLFE(void *r, int v) { (void)r; (void)v; return 0; }
int RFT_SetGai(void *r, int enable, int gain)
{
    (void)r;
    rft_gain_enable=enable==1;
    if (rft_gain_enable) rft_gain=gain;
    return 0;
}
int RFT_GetNeedfulSample(const void *r) { (void)r; return rft_frames*rft_in_ch; }
int LGRFT(void *r, const void *in, void *out, void *tmp, int available)
{
    (void)r; (void)tmp; (void)available;
    ++rft_calls;
    /* Deliberately synthetic output: checks routing, not bit-conversion math. */
    memset(out, 0x5a, (unsigned)(rft_frames*rft_out_ch*(rft_out_bits/8)));
    assert(in == input_bytes);
    return rft_frames*rft_out_ch;
}
#endif
void aud_mutex_init(struct aud_mutex *l) { (void)l; }
void aud_mutex_lock(struct aud_mutex *l) { (void)l; }
void aud_mutex_unlock(struct aud_mutex *l) { (void)l; }
void aud_data_adapter_init(struct pcmcvt_adapter *a) { memset(a,0,sizeof(*a)); }
void aud_data_adapter_cleanup(struct pcmcvt_adapter *a) { (void)a; }
int aud_data_adapter_push(struct pcmcvt_adapter *a, struct aud_data *d) { a->pushed+=d->size; return 0; }
void *aud_data_adapter_map(struct pcmcvt_adapter *a, unsigned bytes) { assert(a->pushed-a->flushed>=bytes); return input_bytes; }
void aud_data_adapter_unmap(struct pcmcvt_adapter *a, void *p) { (void)a; (void)p; }
void aud_data_adapter_flush(struct pcmcvt_adapter *a, unsigned bytes) { a->flushed+=bytes; ++flushed; }
struct aud_data *aud_data_new(unsigned size)
{
    struct aud_data *d=calloc(1,sizeof(*d));
    assert(d);
    d->size=size; d->ptr=malloc(size); assert(d->ptr);
    return d;
}
void aud_object_put(void *p) { (void)p; ++dropped; }
int aud_module_push_data(struct pcmcvt *m, int port, struct aud_data *d) { (void)m; assert(port==0); last_data=d; ++pushed; return 0; }
int aud_module_push_event(struct pcmcvt *m, int port, struct aud_event *e)
{
    (void)m; assert(port==0); last_event=e;
    event_rft_bits=rft_in_bits; event_rft_acmod=rft_acmod;
    return 0;
}
int aud_module_sync(struct pcmcvt *m, int port) { (void)m; assert(port==0); ++synced; return 0; }
int aud_module_default_event(struct pcmcvt *m, struct aud_port *p, struct aud_event *e) { (void)m; (void)p; (void)e; return 19; }
int aud_module_default_command(struct pcmcvt *m, uint32_t c, void *p, unsigned n) { (void)m; (void)c; (void)p; (void)n; return 23; }
int aud_event_value_get_int(struct aud_event *e, const char *key, int fallback)
{
    struct mock_event *v=(struct mock_event *)e;
    for (unsigned i=0;i<v->count;++i) if (!strcmp(v->values[i].key,key)) return v->values[i].value;
    return fallback;
}
void aud_event_value_set_int(struct aud_event *e, const char *key, int value)
{
    struct mock_event *v=(struct mock_event *)e;
    for (unsigned i=0;i<v->count;++i) if (!strcmp(v->values[i].key,key)) { v->values[i].value=value; return; }
    assert(v->count<16); v->values[v->count++]=(struct property){key,value};
}
int aud_event_has_name(struct aud_event *e, const char *key) { return aud_event_value_get_int(e,key,-123)!=-123; }
struct aud_event *aud_event_dup(struct aud_event *e) { if(fail_dup) return NULL; copied=*(struct mock_event *)e; return &copied.base; }
void AuiLogger_AppendData(void *out, const void *in, unsigned size) { memcpy(out,in,size); }
void *_BufMonitor_RemoveBufferIdxLocal(unsigned size) { return malloc(size); }
void _BufMonitor_RemoveBufferIndex(void *p) { free(p); }
char *audutil_strdup_printf(const char *fmt, ...) { (void)fmt; return NULL; }
void ModuleRegistry_Register(void *d) { (void)d; }
void __assert_failed(const char *fn, unsigned line, const char *expr)
{
    (void)fn; (void)line; (void)expr; assert(!"unexpected firmware assertion");
}

static struct pcmcvt fresh(void)
{
    memset(&source,0,sizeof(source));
    source.frames=256;
    struct pcmcvt m={0};
    m.src=&source; m.rft_buffer=converted; m.src_buffer=resampled;
    m.running_mode=2; m.output_samples=256; m.previous_ts=UINT32_MAX;
    return m;
}
static void configure(struct pcmcvt *m, int output_bits)
{
    struct pcmcvt_allfmt f={192000,8,16,0,0,192000,8,(uint32_t)output_bits};
    assert(Pcmcvt_Cmd(m,0x50020020,&f,sizeof(f))==0);
}
static void command_tests(void)
{
    struct pcmcvt m=fresh();
    configure(&m,16);
    assert(m.input_bits==16 && m.output_bits==16 && source.input_rate==192000);
    assert(m.src_bypass==1 && m.sample_shift==4 && rft_in_ch==8 && rft_out_ch==8);
    assert(applied>0 && rft_in_bits==16 && rft_out_bits==16);
    uint32_t distinct[]={44100,6,24,1,1,48000,8,16};
    assert(Pcmcvt_Cmd(&m,0x50020020,distinct,sizeof(distinct))==0);
    assert(m.input_rate==44100 && m.input_channels==6 && m.input_bits==24);
    assert(m.input_endian==1 && m.input_sign==1);
    assert(m.output_rate==48000 && m.output_channels==8 && m.output_bits==16);
    assert(rft_in_ch==6 && rft_in_bits==24 && rft_endian==1 && rft_sign==1);
    assert(rft_out_ch==8 && rft_out_bits==16 && !m.src_bypass);
    configure(&m,32);
    assert(m.src_bypass==1 && m.output_bytes==4 && rft_in_bits==16 && rft_out_bits==32);
    struct pcmcvt_allfmt bad={192000,8,0,0,0,192000,8,16};
    assert(Pcmcvt_Cmd(&m,0x50020020,&bad,0)==-1 && bad.input_bits==32 && m.output_bits==32);
    bad=(struct pcmcvt_allfmt){192000,8,16,2,0,192000,8,16};
    assert(Pcmcvt_Cmd(&m,0x50020020,&bad,0)==-1);
    bad=(struct pcmcvt_allfmt){192000,8,16,0,2,192000,8,16};
    assert(Pcmcvt_Cmd(&m,0x50020020,&bad,0)==-1);
    bad=(struct pcmcvt_allfmt){44,10,7,1,1,192,10,31};
    assert(Pcmcvt_Cmd(&m,0x50020020,&bad,0)==0);
    assert(m.input_rate==44100 && m.output_rate==192000 && m.input_bits==7);
    assert(Pcmcvt_CheckSamplingRate(352)==352800 && Pcmcvt_CheckSamplingRate(22)==22050);
    assert(Pcmcvt_CheckSamplingRate(0)==48000 && Pcmcvt_CheckSamplingRate(11025)==11025);
    configure(&m,16);
    uint32_t rates[]={48000,48000};
    assert(Pcmcvt_Cmd(&m,0x50030008,rates,8)==0 && source.input_rate==48000);
    assert(m.input_rate==192000 && m.output_rate==192000 && !m.src_bypass);
    Pcmcvt_SetParamToLibs(&m);
    assert(source.input_rate==192000 && m.src_bypass);
    assert(Pcmcvt_SetOutFs(&m,48000)==0 && !m.src_bypass && m.output_rate==48000);
    assert(set_prop_input_rate(&m,NULL,"44")==0 && m.input_rate==44);
    assert(set_prop_input_bits(&m,NULL,"33")==0 && m.input_bits==16);
    set_prop_gain(&m,NULL,"8388608"); assert(!rft_gain_enable && m.gain==0x800000);
    set_prop_gain(&m,NULL,"0"); assert(rft_gain_enable && rft_gain==0);
    uint32_t bypass[]={1,8};
    Pcmcvt_Cmd(&m,0x50090008,bypass,8); assert(rft_acmod==8 && m.acmod==8);
    assert(Pcmcvt_SetMode(&m,99)==0 && m.running_mode==2);
    assert(Pcmcvt_Cmd(&m,0xdeadbeef,NULL,0)==23);
    m=fresh(); configure(&m,16);
    struct pcmcvt_fmt legacy={44,48,6,2,24,32};
    assert(Pcmcvt_Cmd(&m,0x50000018,&legacy,sizeof(legacy))==0);
    assert(source.input_rate==44100 && source.output_rate==48000);
    assert(rft_in_ch==6 && rft_in_bits==24 && rft_out_ch==2 && rft_out_bits==32);
    assert(m.input_rate==192000 && m.input_channels==8 && m.input_bits==16);
    assert(m.output_rate==48000 && m.output_channels==2 && m.output_bits==32);
    m.adapter.pushed=96; m.previous_ts=123; m.output_ts=456;
    m.total_input=7; m.total_output=8;
    assert(Pcmcvt_Cmd_Flush(&m)==0 && m.adapter.flushed==96);
    assert(source.input_rate==192000 && source.output_rate==48000);
    assert(rft_in_ch==8 && rft_in_bits==16 && rft_out_ch==2 && rft_out_bits==32);
    assert(m.previous_ts==123 && m.output_ts==456 && m.total_input==7 && m.total_output==8);
}
static void property_state_tests(void)
{
    struct pcmcvt m=fresh(); configure(&m,16);
    assert(set_prop_output_rate(&m,NULL,"48000")==0 && !m.src_bypass);
    assert(set_prop_output_rate(&m,NULL,"192000")==0 && !m.src_bypass);
    assert(source.input_rate==source.output_rate);
    m.adapter.pushed=64;
    assert(set_prop_input_bits(&m,NULL,"16")==0 && m.src_bypass);
    assert(m.adapter.flushed==0 && m.adapter.pushed==64);
    uint32_t rates[]={48000,48000};
    assert(Pcmcvt_Cmd(&m,0x50030008,rates,sizeof(rates))==0);
    assert(set_prop_output_rate(&m,NULL,"48000")==0);
    assert(m.output_rate==192000 && source.output_rate==48000);
    assert(set_prop_input_sign(&m,NULL,"0")==0 && source.output_rate==192000);
    assert(set_prop_output_rate(&m,NULL,"44")==0);
    assert(source.output_rate==44100 && m.output_rate==44);
    assert(set_prop_input_channels(&m,NULL,"8")==0);
    assert(source.output_rate==44 && m.output_rate==44 && m.adapter.flushed==0);
    m.gain=0x800000;
    uint32_t gain[]={1234,1};
    assert(Pcmcvt_Cmd(&m,0x50040008,gain,sizeof(gain))==0);
    assert(rft_gain_enable && rft_gain==1234 && m.gain==0x800000);
    assert(set_prop_gain_enable(&m,NULL,"false")==0);
    assert(!rft_gain_enable && rft_gain==1234 && !m.gain_enable_property);
    assert(set_prop_gain_enable(&m,NULL,"true")==0);
    assert(rft_gain_enable && rft_gain==0x800000 && !m.gain_enable_property);
    assert(set_prop_gain(&m,NULL,"8388608")==0 && !rft_gain_enable);
    gain[0]=99; gain[1]=1;
    assert(Pcmcvt_Cmd(&m,0x50040008,gain,sizeof(gain))==0);
    gain[0]=0; gain[1]=0;
    assert(Pcmcvt_Cmd(&m,0x50040008,gain,sizeof(gain))==0);
    assert(!rft_gain_enable && rft_gain==99 && m.gain==0x800000);
    configure(&m,16);
    assert(Pcmcvt_Cmd_Flush(&m)==0);
    assert(!rft_gain_enable && rft_gain==99 && m.gain==0x800000);
    assert(set_prop_output_samples(&m,NULL,"513")==0);
    assert(m.output_samples==513 && source.frames==256);
    assert(set_prop_output_samples(&m,NULL,"0")==-1 && m.output_samples==513);
    assert(Pcmcvt_Cmd_Flush(&m)==0 && m.output_samples==513 && source.frames==256);
    assert(set_prop_acmod(&m,NULL,"8")==0 && m.acmod==8 && rft_acmod==8);
    assert(set_prop_available(&m,NULL,"true")==0 && !m.available);
    assert(set_prop_available(&m,NULL,"2")==0 && m.available);
    assert(set_prop_over_protect(&m,NULL,"1")==0 && m.over_protect==1);
    assert(set_prop_over_protect(&m,NULL,"2")==-1 && m.over_protect==1);
    assert(set_prop_generate_lfe(&m,NULL,"yes")==0 && m.generate_lfe);
    assert(set_prop_generate_lfe(&m,NULL,"2")==0 && !m.generate_lfe);
    assert(set_prop_runing_mode(&m,NULL,"1")==0 && m.running_mode==1 && source.mode==1);
    assert(set_prop_runing_mode(&m,NULL,"0")==0 && m.running_mode==2 && source.mode==0);
}
static void event_tests(void)
{
    struct pcmcvt m=fresh(); configure(&m,32);
    struct mock_event e={.base={1,0}};
    aud_event_value_set_int(&e.base,"channels",8);
    aud_event_value_set_int(&e.base,"bits-per-sample",16);
    aud_event_value_set_int(&e.base,"rate",192000);
    aud_event_value_set_int(&e.base,"acmod",10);
    aud_event_value_set_int(&e.base,"es",1);
    aud_event_value_set_int(&e.base,"codec",1234);
    aud_event_value_set_int(&e.base,"data-rate",24576000);
    m.adapter.pushed=100;
    int before=flushed;
    assert(Pcmcvt_Event(&m,NULL,&e.base)==0 && flushed==before);
    assert(last_event!=&e.base && aud_event_value_get_int(last_event,"es",0)==1);
    assert(aud_event_value_get_int(last_event,"codec",0)==1234);
    assert(aud_event_value_get_int(last_event,"data-rate",0)==24576000);
    assert(aud_event_value_get_int(last_event,"bits-per-sample",0)==32);
    aud_event_value_set_int(&e.base,"bits-per-sample",24);
    Pcmcvt_Event(&m,NULL,&e.base); assert(flushed==before && m.input_bits==24);
    assert(event_rft_bits==16 && rft_in_bits==24 && m.adapter.pushed-m.adapter.flushed==100);
    aud_event_value_set_int(&e.base,"out-channels",6);
    Pcmcvt_Event(&m,NULL,&e.base);
    assert(flushed==before+1 && m.adapter.flushed==100);
    assert(aud_event_value_get_int(last_event,"acmod",0)==7 && m.acmod==10);
    aud_event_value_set_int(&e.base,"out-channels",2);
    Pcmcvt_Event(&m,NULL,&e.base); assert(aud_event_value_get_int(last_event,"acmod",0)==2);
    fail_dup=true; last_event=NULL;
    Pcmcvt_Event(&m,NULL,&e.base); assert(last_event==NULL); fail_dup=false;
    struct mock_event change={.base={13,0}};
    aud_event_value_set_int(&change.base,"change-out-rate",48000);
    assert(Pcmcvt_Event(&m,NULL,&change.base)==0 && m.output_rate==48000);
    struct mock_event flush={.base={7,0}};
    before=flushed; m.adapter.pushed+=8;
    assert(Pcmcvt_Event(&m,NULL,&flush.base)==19 && flushed==before);
    flush.base.prop=2;
    assert(Pcmcvt_Event(&m,NULL,&flush.base)==19 && flushed==before+1);
    m=fresh(); configure(&m,32);
    assert(set_prop_input_endian(&m,NULL,"1")==0);
    assert(set_prop_input_sign(&m,NULL,"1")==0);
    assert(set_prop_acmod(&m,NULL,"8")==0);
    uint32_t gain[]={55,1};
    assert(Pcmcvt_Cmd(&m,0x50040008,gain,sizeof(gain))==0);
    struct mock_event first={.base={1,0}};
    aud_event_value_set_int(&first.base,"channels",2);
    aud_event_value_set_int(&first.base,"bits-per-sample",24);
    aud_event_value_set_int(&first.base,"rate",44100);
    aud_event_value_set_int(&first.base,"endian",0);
    aud_event_value_set_int(&first.base,"sign",0);
    aud_event_value_set_int(&first.base,"es",1);
    m.adapter.pushed=100;
    assert(Pcmcvt_Event(&m,NULL,&first.base)==0 && m.adapter.flushed==100);
    assert(m.input_channels==2 && m.input_bits==24 && m.input_rate==44100 && m.acmod==2);
    assert(m.output_channels==8 && m.output_bits==32 && m.output_rate==192000);
    assert(m.input_endian==1 && m.input_sign==1 && rft_endian==1 && rft_sign==1);
    assert(event_rft_bits==16 && event_rft_acmod==8 && rft_in_bits==24 && rft_acmod==2);
    assert(rft_gain_enable && rft_gain==55);
    assert(aud_event_value_get_int(last_event,"bits-per-sample",0)==32);
    assert(aud_event_value_get_int(last_event,"rate",0)==192000);
    assert(aud_event_value_get_int(last_event,"es",0)==1);
}
static void chain_tests(void)
{
    for(int bits=16;bits<=32;bits+=16) {
        struct pcmcvt m=fresh(); configure(&m,bits);
        struct aud_port port={.module=&m};
        struct aud_data input={.type=1,.ts=9000,.size=4096,.ptr=input_bytes};
        int drop=dropped, push=pushed, rft=rft_calls, src=src_calls;
        Pcmcvt_Chai(&port,&input); assert(dropped==drop+1 && pushed==push && rft_calls==rft);
        input.type=3; Pcmcvt_Chai(&port,&input); assert(last_data==&input && pushed==push+1);
        input.type=0; Pcmcvt_Chai(&port,&input);
        assert(rft_calls==rft+1 && src_calls==src && last_data!=&input);
        assert(last_data->type==0 && last_data->ts==9000 && last_data->size==(unsigned)(256*8*(bits/8)));
        assert(((unsigned char *)last_data->ptr)[0]==0x5a && m.total_input==128 && synced>0);
        free(last_data->ptr); free(last_data);
        Pcmcvt_Chai(&port,&input);
        assert(last_data->ts==9120);
        free(last_data->ptr); free(last_data);
    }
    struct pcmcvt m=fresh(); configure(&m,16); Pcmcvt_SetOutFs(&m,48000);
    struct aud_port port={.module=&m};
    struct aud_data input={.type=0,.ts=0,.size=4096}; int count=src_calls;
    Pcmcvt_Chai(&port,&input); assert(src_calls==count+1 && last_data->type==0);
    free(last_data->ptr); free(last_data);
}
#ifndef PCMCVT_REAL_RFT
int main(void)
{
    command_tests(); property_state_tests(); event_tests(); chain_tests();
    puts("PASS pcmcvt command/cache, FIRST preservation, PCM typing and RFT/SRC dispatch");
    return 0;
}
#endif
