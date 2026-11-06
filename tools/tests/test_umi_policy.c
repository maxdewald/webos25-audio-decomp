/* Usage: python3 tools/tests/test_umi_policy.py [--sanitize].
 * Host adapters are not an LG class layout: byte storage represents the shipped
 * ARM object and separately owned maps/JSON stand in for C++ library objects.
 */
#include "../../src/libumi/policy.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct umi_value {
    enum { NIL, BOOL, INT, STRING, ARRAY, OBJECT } kind;
    const char *key;
    const char *text;
    int number;
    const struct umi_value *items;
    size_t count;
};
#define STR(k,s) {.kind=STRING,.key=k,.text=s}
#define NUM(k,n) {.kind=INT,.key=k,.number=n}
#define BIT(k,b) {.kind=BOOL,.key=k,.number=b}
#define LIST(k,a) {.kind=ARRAY,.key=k,.items=a,.count=sizeof(a)/sizeof((a)[0])}
#define OBJ(k,a) {.kind=OBJECT,.key=k,.items=a,.count=sizeof(a)/sizeof((a)[0])}
static const struct umi_value nil;
static unsigned char object[0x28c];
static const char *codec, *channel, *source, *content, *mode, *harmony, *country;
static const char *strings[0x28c];
static const struct umi_value *earc, *wow;
static struct { const char *control; int words[5]; size_t count; } writes[512];
static size_t write_count;
static unsigned delay_updates, ott_updates, stores, parser_stores, arc_delays, atmos_updates;
static bool ott_dd, ott_dts, alsa_ok, get_alsa_ok;
static int version[5];
static const char *parameter(umi_tv_audio *a, bool input, const char *key)
{
    assert(a == object);
    if (!input) {
        if (!strcmp(key, "soundOutputDigital")) return mode;
        if (!strcmp(key, "lgHarmony")) return harmony;
    } else {
        if (!strcmp(key, "codec")) return codec;
        if (!strcmp(key, "channel")) return channel;
        if (!strcmp(key, "sourceType")) return source;
        if (!strcmp(key, "contentType")) return content;
    }
    return NULL;
}
static const char *string_at(umi_tv_audio *a, unsigned offset)
{
    assert(a == object && offset < sizeof(object));
    return strings[offset] ? strings[offset] : "";
}
static void assign_string(umi_tv_audio *a, unsigned offset, const char *text)
{
    assert(a == object && offset < sizeof(object));
    strings[offset] = text;
}
static const struct umi_value *json_at(umi_tv_audio *a, unsigned offset)
{
    assert(a == object);
    assert(offset == UMI_EARC_CAP || offset == UMI_WOW_CAP);
    return offset == UMI_EARC_CAP ? earc : wow;
}
static void assign_json(umi_tv_audio *a, unsigned offset, const struct umi_value *v)
{
    assert(a == object);
    if (offset == UMI_EARC_CAP) earc = v;
    else { assert(offset == UMI_WOW_CAP); wow = v; }
}
static const struct umi_value *member(const struct umi_value *v, const char *key)
{
    if (v && v->kind == OBJECT)
        for (size_t i=0; i<v->count; ++i)
            if (!strcmp(v->items[i].key,key)) return &v->items[i];
    return &nil;
}
static bool has(const struct umi_value *v, const char *key) { return member(v,key) != &nil; }
static bool is_array(const struct umi_value *v) { return v && v->kind == ARRAY; }
static size_t size(const struct umi_value *v) { return is_array(v) ? v->count : 0; }
static const struct umi_value *element(const struct umi_value *v, size_t i)
{
    return i < size(v) ? &v->items[i] : &nil;
}
static bool as_string(const struct umi_value *v, const char **text)
{
    if (!v || v->kind != STRING) return false;
    *text = v->text;
    return true;
}
static bool as_bool(const struct umi_value *v, bool *b)
{
    if (!v || v->kind != BOOL) return false;
    *b = v->number;
    return true;
}
static bool as_int(const struct umi_value *v, int *n)
{
    if (!v || v->kind != INT) return false;
    *n = v->number;
    return true;
}
static bool set_alsa(umi_tv_audio *a, const char *control, const int *words, size_t count)
{
    assert(a == object && count <= 5 && write_count < 512);
    writes[write_count].control = control;
    writes[write_count].count = count;
    memcpy(writes[write_count++].words, words, count*sizeof(int));
    return alsa_ok;
}
static bool get_alsa(umi_tv_audio *a, const char *control, int *words, size_t count)
{
    assert(a == object && !strcmp(control,"Dolby MS12 IDK Version") && count == 5);
    memcpy(words,version,sizeof(version));
    return get_alsa_ok;
}
static void delay_codec(umi_tv_audio *a, int ct) { assert(a == object && ct > 0); }
static void delay_update(umi_tv_audio *a) { assert(a == object); ++delay_updates; }
static void ott_mode(umi_tv_audio *a, bool dd, bool dts)
{
    assert(a == object); ++ott_updates; ott_dd=dd; ott_dts=dts;
}
static void arc_delay(umi_tv_audio *a) { assert(a == object); ++arc_delays; }
static void atmos_capability(umi_tv_audio *a) { assert(a == object); ++atmos_updates; }
static bool store_parameters(umi_tv_audio *a, const struct umi_value *v)
{
    assert(a == object && v->kind == OBJECT); ++parser_stores; return true;
}
static void store_settings(umi_tv_audio *a, const char *category, const struct umi_value *v)
{
    assert(a == object && category && v->kind == OBJECT); ++stores;
}
static bool sound_output_mode(umi_tv_audio *a, const struct umi_value *v)
{
    assert(a == object && v); return true;
}
static const char *get_country(umi_tv_audio *a) { assert(a == object); return country; }
const struct umi_policy_ops umi_ops = {
    parameter,string_at,assign_string,json_at,assign_json,has,member,is_array,size,element,
    as_string,as_bool,as_int,set_alsa,get_alsa,delay_codec,delay_update,ott_mode,arc_delay,
    atmos_capability,store_parameters,store_settings,sound_output_mode,get_country
};
static const struct umi_value pcm_entries[] = {STR(NULL,"PCM")};
static const struct umi_value pcm_caps = LIST(NULL,pcm_entries);
static const struct umi_value all_entries[] = {
    STR(NULL,"PCM"),STR(NULL,"MAT2PCM"),STR(NULL,"DD+"),STR(NULL,"DD"),
    STR(NULL,"DTS"),STR(NULL,"DTSHD"),STR(NULL,"DTSHD_MA"),STR(NULL,"DTSX_P1"),
    STR(NULL,"DTSX_P2"),STR(NULL,"AAC"),STR(NULL,"MPEG4AACLC"),STR(NULL,"TrueHD")
};
static const struct umi_value all_caps = LIST(NULL,all_entries);
static const struct umi_value mix_off_items[] = {BIT("mix",false)};
static const struct umi_value mix_on_items[] = {BIT("mix",true)};
static const struct umi_value mix_off = OBJ(NULL,mix_off_items), mix_on = OBJ(NULL,mix_on_items);
static const struct umi_value empty = {.kind=OBJECT};

static void reset(void)
{
    memset(object,0,sizeof(object)); memset(strings,0,sizeof(strings));
    memset(version,0,sizeof(version));
    codec="pcm"; channel="8"; source="HDMI"; content="";
    mode="passThrough"; harmony="false"; country="US";
    earc=&pcm_caps; wow=&pcm_caps; write_count=0;
    delay_updates=ott_updates=stores=parser_stores=arc_delays=atmos_updates=0;
    alsa_ok=true; get_alsa_ok=false;
    umi_set_word(object,UMI_OUTPUT,0x400);
    umi_set_word(object,UMI_PCM_CHANNELS,8);
    umi_set_word(object,UMI_WOW_CHANNELS,8);
}
static size_t last(const char *name)
{
    for (size_t n=write_count; n; --n)
        if (!strcmp(writes[n-1].control,name)) return n-1;
    assert(!"missing ALSA write"); return 0;
}
static void expect(const char *name, int ct, int cn, int mo)
{
    size_t i=last(name);
    assert(writes[i].count==5 && writes[i].words[0]==2 && writes[i].words[1]==ct &&
           writes[i].words[2]==cn && writes[i].words[3]==0 && writes[i].words[4]==mo);
}
static void earc_expect(int ct,int cn,int mo) { expect("Sndout EARC Outputtype",ct,cn,mo); }

static void test_pcm_mix_channels(void)
{
    reset();
    assert(setMixerSettings(object,&mix_off));
    assert(!write_count); /* constructor already false: no reapply at all */
    assert(setMixerSettings(object,&mix_on));
    earc_expect(CT_PCM,8,MO_ALL);
    write_count=0;
    assert(setSoundOutputDigitalSettings(object,"setMixer",&mix_off));
    assert(stores==1 && !umi_byte(object,UMI_MIX));
    earc_expect(CT_PCM,8,MO_NONE);
    expect("Sndout OPTIC Outputtype",CT_PCM,0,MO_ALL);
    expect("Sndout ARC Outputtype",CT_PCM,8,MO_NONE);
    size_t count=write_count;
    assert(setMixerSettings(object,&mix_off)); assert(write_count==count);
    umi_set_word(object,UMI_PCM_CHANNELS,6);
    setSoundOutputDigital(object,mode); earc_expect(CT_PCM,6,MO_ALL);
    umi_set_word(object,UMI_PCM_CHANNELS,0);
    setSoundOutputDigital(object,mode); earc_expect(CT_PCM,0,MO_ALL);
    umi_set_word(object,UMI_PCM_CHANNELS,8); channel="3";
    setSoundOutputDigital(object,mode); earc_expect(CT_PCM,0,MO_ALL);
    channel="2"; setSoundOutputDigital(object,mode); earc_expect(CT_PCM,2,MO_NONE);
    mode="pcm"; setSoundOutputDigital(object,mode); earc_expect(CT_PCM,0,MO_ALL);
    assert(!umi_byte(object,UMI_MIX));
    mode="auto"; setSoundOutputDigital(object,mode); earc_expect(CT_PCM,2,MO_ALL);
    mode="passThrough"; umi_set_byte(object,UMI_DOWNMIX,1);
    setSoundOutputDigital(object,mode); earc_expect(CT_PCM,2,MO_ALL);
    umi_set_byte(object,UMI_DOWNMIX,0); harmony="true";
    setSoundOutputDigital(object,mode); earc_expect(CT_PCM,2,MO_ALL);
}

static void test_dts_and_unknown(void)
{
    static const char *const names[]={"dts","dts_hd","dts_hd_ma","dts_cd","dts_express","dts_x_p1","dts_x_p2"};
    static const int direct[]={CT_DTS,CT_DTSHD,CT_DTSHD_MA,CT_DTSX_P1,CT_DTSX_P1,CT_DTSX_P1,CT_DTSX_P1};
    for (size_t i=0;i<7;++i) {
        reset(); codec=names[i]; earc=wow=&all_caps;
        assert(is_input_dts(codec)); assert(!isInputCodecPCM(object));
        setSoundOutputDigital(object,mode);
        earc_expect(direct[i],0,(i==3||i==4||i==6)?MO_ALL:MO_NONE);
    }
    reset(); codec="DTSHD"; earc=wow=&all_caps;
    assert(!is_input_dts(codec) && isInputCodecPCM(object));
    setSoundOutputDigital(object,mode); earc_expect(CT_PCM,0,MO_ALL);
    reset(); codec="dts_hd_ma"; earc=wow=&pcm_caps;
    setSoundOutputDigital(object,mode); earc_expect(CT_PCM,0,MO_ALL);
    codec="ac3"; earc=wow=&all_caps; setSoundOutputDigital(object,mode);
    earc_expect(CT_DD,0,MO_NONE); expect("Sndout OPTIC Outputtype",CT_DD,0,MO_NONE);
}

static void test_independent_capability_fallback(void)
{
    reset(); wow=&nil;
    setSoundOutputDigital(object,mode);
    earc_expect(CT_PCM,8,MO_NONE);
    assert(write_count==7); /* fallback OPTIC/ARC/eARC/wow then direct OPTIC/ARC/eARC */
    assert(!strcmp(writes[2].control,"Sndout EARC Outputtype") && writes[2].words[4]==MO_ALL);
    reset(); earc=&nil;
    setSoundOutputDigital(object,mode);
    earc_expect(CT_PCM,8,MO_ALL);
    expect("Sndout WOWCAST Outputtype",CT_PCM,8,MO_NONE);
    assert(write_count==5);
    reset(); earc=wow=&nil;
    setSoundOutputDigital(object,mode); assert(write_count==8); /* two global fallbacks */
    reset(); codec="";
    setSoundOutputDigital(object,mode); assert(write_count==4);
}

static void test_external_capability_lifetime(void)
{
    static const struct umi_value pcm6[]={STR("audioCodingType","PCM"),NUM("maxChannelNumber",6)};
    static const struct umi_value pcm7[]={STR("audioCodingType","PCM"),NUM("maxChannelNumber",7)};
    static const struct umi_value other[]={STR("audioCodingType","DTS"),NUM("maxChannelNumber",64)};
    static const struct umi_value invalid[]={STR("audioCodingType","PCM"),STR("maxChannelNumber","20")};
    static const struct umi_value advanced[]={OBJ(NULL,pcm6),OBJ(NULL,other),OBJ(NULL,invalid),OBJ(NULL,pcm7)};
    static const struct umi_value fields[]={LIST("earcCapability",pcm_entries),LIST("advancedEarcCapability",advanced),BIT("supportIamf",true)};
    static const struct umi_value event=OBJ(NULL,fields);
    reset();
    assert(setExternalArcSettings(object,&event));
    assert(umi_word(object,UMI_PCM_CHANNELS)==8 && umi_byte(object,UMI_IAMF_EARC));
    assert(parser_stores==1 && isEarcCapabilityAvailable(object,"PCM"));
    earc_expect(CT_PCM,8,MO_NONE);
    const struct umi_value *retained=earc;
    write_count=0;
    assert(setExternalArcSettings(object,&empty));
    assert(earc==retained && !write_count && !umi_byte(object,UMI_IAMF_EARC));
    assert(umi_word(object,UMI_PCM_CHANNELS)==0);
    setSoundOutputDigital(object,mode); earc_expect(CT_PCM,0,MO_ALL);
    reset(); codec="iamf"; umi_set_byte(object,UMI_IAMF_EARC,1);
    setOutputType(object,CT_PCM,MO_NONE); earc_expect(CT_PCM,8,MO_ALL);
    umi_set_byte(object,UMI_IAMF_EARC,0); umi_set_byte(object,UMI_IAMF,1);
    assert(isSupportIAMF(object));
}

static void test_profile_update_order(void)
{
    static const struct umi_value fields[]={LIST("earcCapability",all_entries),STR("earcMatProfile","matprofile1")};
    static const struct umi_value event=OBJ(NULL,fields);
    reset(); codec="mat"; wow=&all_caps;
    assert(setExternalArcSettings(object,&event));
    earc_expect(CT_MAT2PCM,0,MO_NONE); /* recompute used old profile */
    assert(!isEarcCapabilityAvailable(object,"MAT2PCM"));
    assert(isWowCastCapabilityAvailable(object,"MAT2PCM"));
    setSoundOutputDigital(object,mode); earc_expect(CT_DDP,0,MO_ALL);
    strings[UMI_MAT_PROFILE]="matprofile2";
    assert(isEarcCapabilityAvailable(object,"MAT2PCM") && SP50ValueBaseOnMatProfile(object));
}

static void test_output_change_and_ott(void)
{
    reset(); umi_set_byte(object,UMI_MIX,1);
    resetInitFlags(object); assert(umi_byte(object,UMI_MIX));
    assert(setMixerSettings(object,&mix_off));
    umi_set_word(object,UMI_OUTPUT,1);
    assert(!strcmp(getSoundOutputDigitalMode(object),"pcm"));
    setSoundOutputDigital(object,getSoundOutputDigitalMode(object)); earc_expect(CT_PCM,0,MO_ALL);
    assert(!umi_byte(object,UMI_MIX) && !strcmp(mode,"passThrough"));
    umi_set_word(object,UMI_OUTPUT,0x400);
    setSoundOutputDigital(object,getSoundOutputDigitalMode(object)); earc_expect(CT_PCM,8,MO_NONE);
    umi_set_word(object,UMI_OUTPUT,0x400000);
    assert(!strcmp(getSoundOutputDigitalMode(object),"pcm"));
    umi_set_byte(object,UMI_HARMONY,1);
    assert(!strcmp(getSoundOutputDigitalMode(object),"passThrough"));
    assert(!isLgHarmonySoundOutput(object)); /* numeric flag != parser harmony */
    umi_set_byte(object,UMI_OTT_DOLBY,1); umi_set_byte(object,UMI_OTT_DTS,1);
    mode="auto"; setSoundOutputDigital(object,mode); assert(ott_dd && ott_dts);
    mode="passThrough"; setSoundOutputDigital(object,mode); assert(!ott_dd && !ott_dts);
    harmony="true"; setSoundOutputDigital(object,mode); assert(ott_dd && ott_dts);
    mode="unexpected"; write_count=0;
    assert(setSoundOutputDigital(object,mode) && !write_count);
    umi_set_byte(object,UMI_ALWAYS_DELAY,1);
    unsigned old=delay_updates; setSoundOutputDigital(object,mode); assert(delay_updates==old+1);
}

static void test_aac_and_writer_failure(void)
{
    reset(); codec="aac"; country="JP"; source="MEDIA"; content="dtv_8k"; earc=wow=&all_caps;
    assert(isInputSourceToSetAAC(object) && isAACOutputCodec(object));
    setSoundOutputDigital(object,mode); earc_expect(CT_MPEG4AACLC,0,MO_NONE);
    mode="auto"; assert(getEarcAndARCOutputCodec(object)==CT_AAC);
    harmony="true"; umi_set_word(object,UMI_AI_GENERATION,6);
    assert(!isAACOutputCodec(object));
    source="WEB"; assert(!isInputSourceToSetAAC(object));
    reset(); alsa_ok=false;
    assert(!setEArcOutputType(object,CT_DTSHD_MA,MO_NONE,0));
    assert(umi_word(object,UMI_REQUEST_CODEC)==CT_DTSHD_MA && umi_word(object,UMI_EARC_MIX)==MO_NONE);
    write_count=0;
    assert(setArcOutputType(object,CT_DTSHD_MA,MO_NONE,0) && !write_count);
    assert(setOpticOutputType(object,CT_DDP,MO_ALL) && !write_count);
    assert(setAllSoundOutputDigital(object,"pcm")); /* low-level errors not propagated */
    reset(); get_alsa_ok=true; version[0]=2;version[1]=5; assert(!SupportSP50(object));
    version[1]=6;assert(SupportSP50(object));version[0]=3;version[1]=0;assert(SupportSP50(object));
    strings[UMI_MAT_PROFILE]="matprofile2";
    assert(setEArcOutputType(object,CT_MAT2PCM,MO_NONE,0));
    assert(!strcmp(writes[write_count-1].control,"Sndout EARC MS12 SP50 OnOff"));
}

int main(void)
{
    test_pcm_mix_channels();
    test_dts_and_unknown();
    test_independent_capability_fallback();
    test_external_capability_lifetime();
    test_profile_update_order();
    test_output_change_and_ott();
    test_aac_and_writer_failure();
    puts("UMI policy: 7 scenario groups passed");
    return 0;
}
