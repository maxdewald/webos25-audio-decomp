#include <algorithm>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

struct UmiLogContext {
    int enabledLevel;
};

extern "C" {
extern UmiLogContext *umiLogContext;
int _PmLogMsgKV(UmiLogContext *, int, unsigned, const char *, ...);
}

namespace umi_arc_output {

struct Audio;
struct Delay;

struct AudioVtable {
    void *unknown00[0x4c / 4];
    bool (*setControl)(Audio *, const std::string &, const int *, int);
};

struct DelayVtable {
    void *unknown00[0x44 / 4];
    void (*setCodec)(Delay *, int);
};

struct Delay {
    const DelayVtable *vtable;
};

struct Audio {
    const AudioVtable *vtable;
    unsigned char unknownData04[0x1c - 4];
    int arcOn;
    int earcOn;
    unsigned char unknown24[0x48 - 0x24];
    int earcCodec;
    int earcChannels;
    int earcMix;
    unsigned char unknown54[0x60 - 0x54];
    int arcCodec;
    int requestedCodec;
    int requestedMix;
    unsigned char unknown6c[0xc4 - 0x6c];
    int output;
    unsigned char unknownc8[0x21c - 0xc8];
    Delay *delay;
};

#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(Audio, earcOn) == 0x20, "eARC state offset");
static_assert(offsetof(Audio, earcCodec) == 0x48, "eARC codec offset");
static_assert(offsetof(Audio, requestedCodec) == 0x64, "request offset");
static_assert(offsetof(Audio, output) == 0xc4, "output offset");
static_assert(offsetof(Audio, delay) == 0x21c, "delay offset");
static_assert(offsetof(AudioVtable, setControl) == 0x4c, "ALSA virtual slot");
static_assert(offsetof(DelayVtable, setCodec) == 0x44, "delay virtual slot");
#endif
extern void addInOutSettingNormalLog(Audio *) __asm__("_ZN10umiTVAudio24addInOutSettingNormalLogEv");
extern bool isArcSoundOutput(Audio *) __asm__("_ZN10umiTVAudio16isArcSoundOutputEv");
extern bool SupportSP50(Audio *) __asm__("_ZN10umiTVAudio11SupportSP50Ev");
extern bool SP50ValueBaseOnMatProfile(Audio *) __asm__("_ZN10umiTVAudio25SP50ValueBaseOnMatProfileEv");

#define ARC_SOURCE_FILE \
    "/usr/src/debug/lib32-umi/1.0.0-1.187.ptl4tv.13-r0/git/src/umitvaudioquality.cpp"
#define ARC_LOG(level, id, method, line, format, ...) \
    do { \
        if (!umiLogContext || umiLogContext->enabledLevel >= (level)) \
            _PmLogMsgKV(umiLogContext, level, 0, id, 0, nullptr, nullptr, \
                        "%s:%s(%d) " format, std::strrchr(ARC_SOURCE_FILE, '/') + 1, \
                        method, line, __VA_ARGS__); \
    } while (0)

bool setEArcOutputType(Audio *, int, int, int) __asm__("_ZN10umiTVAudio17setEArcOutputTypeE25UMI_SNDOUT_OUTPUT_CODEC_T26sndout_output_mix_option_t27sndout_output_channel_num_t");
/* @0x41b64: raw Thumb +0x20/+0x48 caches precede vtable +0x4c writes; delay is +0x21c/+0x44. */
bool setEArcOutputType(Audio *self, int codec, int mix, int channels)
{
    ARC_LOG(6, "INFO_LOG", "setEArcOutputType", 0x21a,
            "set codec type: %d for earc output type and mixer value is %d", codec, mix);
    int words[5];
    words[0] = 2;
    words[1] = codec;
    words[2] = channels;
    words[3] = 0;
    words[4] = mix;
    self->requestedCodec = codec;
    self->requestedMix = mix;
    if (self->earcOn && self->earcCodec != codec) {
        self->earcCodec = codec;
        addInOutSettingNormalLog(self);
    }
    self->earcCodec = codec;
    self->earcChannels = channels;
    self->earcMix = mix;
    if (!self->vtable->setControl(self, "Sndout EARC Outputtype", words, 5)) {
        ARC_LOG(3, "CONTROL_ELEMENT_SET_ERROR", "setEArcOutputType", 0x233,
                "Did not able to set the element: %s", "Sndout EARC Outputtype");
        return 0;
    }
    if (codec == 7 && SupportSP50(self)) {
        ARC_LOG(6, "INFO_LOG", "setEArcOutputType", 0x23b,
                "Support SP50, SP50Value:%d", SP50ValueBaseOnMatProfile(self));
        words[0] = SP50ValueBaseOnMatProfile(self);
        if (!self->vtable->setControl(self, "Sndout EARC MS12 SP50 OnOff", words, 1)) {
            ARC_LOG(3, "CONTROL_ELEMENT_SET_ERROR", "setEArcOutputType", 0x240,
                    "Did not able to set the element: %s", "Sndout EARC MS12 SP50 OnOff");
            return 0;
        }
    }
    if (self->output == 0x80000) {
        ARC_LOG(6, "INFO_LOG", "setEArcOutputType", 0x24a,
                "[AutoAvSync] SE_ARC Output codec value is (codec: %d)", 1);
        self->delay->vtable->setCodec(self->delay, 1);
    } else if (isArcSoundOutput(self) && self->earcOn) {
        ARC_LOG(6, "INFO_LOG", "setEArcOutputType", 0x24d,
                "[AutoAvSync] eARC Output codec value is (codec: %d)", codec);
        self->delay->vtable->setCodec(self->delay, codec);
    }
    return true;
}

bool setArcOutputType(Audio *, int, int, int) __asm__("_ZN10umiTVAudio16setArcOutputTypeE25UMI_SNDOUT_OUTPUT_CODEC_T26sndout_output_mix_option_t27sndout_output_channel_num_t");
/* @0x41f9c: vector validation accepts 1..5 and 9; cache +0x60 changes before ALSA, including failure. */
bool setArcOutputType(Audio *self, int codec, int mix, int channels)
{
    std::vector<int> supported = {1, 2, 3, 4, 5, 9};
    if (std::find(supported.begin(), supported.end(), codec) == supported.end()) {
        ARC_LOG(6, "INFO_LOG", "setArcOutputType", 0x2a3,
                "set codec type: %d is not supported", codec);
        return 1;
    }
    ARC_LOG(6, "INFO_LOG", "setArcOutputType", 0x2a7,
            "set codec type: %d for arc output type and mixer value is %d", codec, mix);
    if (self->arcOn && self->arcCodec != codec) {
        self->arcCodec = codec;
        addInOutSettingNormalLog(self);
    }
    int words[5];
    words[0] = 2;
    words[1] = codec;
    words[2] = channels;
    words[3] = 0;
    words[4] = mix;
    self->arcCodec = codec;
    if (!self->vtable->setControl(self, "Sndout ARC Outputtype", words, 5)) {
        ARC_LOG(3, "CONTROL_ELEMENT_SET_ERROR", "setArcOutputType", 0x2b9,
                "Did not able to set the element: %s", "Sndout ARC Outputtype");
        return 0;
    }
    if (self->output == 0x80000) {
        ARC_LOG(6, "INFO_LOG", "setArcOutputType", 0x2c1,
                "[AutoAvSync] SE_ARC Output codec value is (codec: %d)", 1);
        self->delay->vtable->setCodec(self->delay, 1);
    } else if (isArcSoundOutput(self) && self->arcOn) {
        ARC_LOG(6, "INFO_LOG", "setArcOutputType", 0x2c6,
                "[AutoAvSync] ARC Output codec value is (codec: %d)", codec);
        self->delay->vtable->setCodec(self->delay, codec);
    }
    return 1;
}

bool setOpticOutputType(Audio *, int, int) __asm__("_ZN10umiTVAudio18setOpticOutputTypeE25UMI_SNDOUT_OUTPUT_CODEC_T26sndout_output_mix_option_t");
/* @0x422e8: vector validation accepts 1..4; optical channel/rate words stay zero. */
bool setOpticOutputType(Audio *self, int codec, int mix)
{
    std::vector<int> supported = {1, 2, 3, 4};
    if (std::find(supported.begin(), supported.end(), codec) == supported.end()) {
        ARC_LOG(6, "INFO_LOG", "setOpticOutputType", 0x275,
                "set codec type: %d is not supported", codec);
        return 1;
    }
    ARC_LOG(6, "INFO_LOG", "setOpticOutputType", 0x279,
            "set codec type: %d for optic output type and mixer value is %d", codec, mix);
    int words[5];
    words[0] = 2;
    words[1] = codec;
    words[2] = 0;
    words[3] = 0;
    words[4] = mix;
    if (!self->vtable->setControl(self, "Sndout OPTIC Outputtype", words, 5)) {
        ARC_LOG(3, "CONTROL_ELEMENT_SET_ERROR", "setOpticOutputType", 0x283,
                "Did not able to set the element: %s", "Sndout OPTIC Outputtype");
        return 0;
    }
    if (self->output == 0x1000) {
        ARC_LOG(6, "INFO_LOG", "setOpticOutputType", 0x28a,
                "[AutoAvSync] LGSOUNDSYNC Output codec value is (codec: %d)", 1);
        self->delay->vtable->setCodec(self->delay, 1);
    } else if (self->output == 2) {
        ARC_LOG(6, "INFO_LOG", "setOpticOutputType", 0x28f,
                "[AutoAvSync] Optical Output codec value is (codec: %d)", codec);
        self->delay->vtable->setCodec(self->delay, codec);
    }
    return 1;
}

bool setArcOnOff(Audio *, int, int) __asm__("_ZN10umiTVAudio11setArcOnOffEii");
/* @0x3ffc8: transition log observes OLD state; both caches change before either write. */
bool setArcOnOff(Audio *self, int arc, int earc)
{
    ARC_LOG(6, "INFO_LOG", "setArcOnOff", 0x92,
            "Setting SoundOut on off for ARC status: %d & eARC status: %d", arc, earc);
    if ((!self->arcOn && arc == 1) || (!self->earcOn && earc == 1))
        addInOutSettingNormalLog(self);
    self->arcOn = arc;
    self->earcOn = earc;
    int words[5];
    words[0] = arc;
    if (!self->vtable->setControl(self, "Sndout ARC OnOff", words, 1)) {
        ARC_LOG(3, "CONTROL_ELEMENT_SET_ERROR", "setArcOnOff", 0xa0,
                "In setSoundOutArc : Did not able to set the element: %s with status %d",
                "Sndout ARC OnOff", arc);
        return 0;
    }
    words[0] = earc;
    if (!self->vtable->setControl(self, "Sndout EARC OnOff", words, 1)) {
        ARC_LOG(3, "CONTROL_ELEMENT_SET_ERROR", "setArcOnOff", 0xa8,
                "In setSoundOuteArc : Did not able to set the element: %s with status %d",
                "Sndout EARC OnOff", earc);
        return 0;
    }
    return true;
}

#undef ARC_LOG
#undef ARC_SOURCE_FILE

}