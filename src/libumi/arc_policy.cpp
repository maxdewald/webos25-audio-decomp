#include <cstddef>
#include <cstring>
#include <string>

struct UmiLogContext {
    int enabledLevel;
};

extern "C" {
extern UmiLogContext *umiLogContext;
int _PmLogMsgKV(UmiLogContext *, int, unsigned, const char *, ...);
}

namespace umi_arc_policy {

struct Audio {
    unsigned char unknown00[0x40];
    int pcmChannels;
    unsigned char unknown44[0x7c - 0x44];
    unsigned char mix;
};

static_assert(offsetof(Audio, pcmChannels) == 0x40, "PCM channel capability offset");
static_assert(offsetof(Audio, mix) == 0x7c, "mixer setting offset");

extern std::string getInputCodecType(const Audio *) __asm__("_ZNK10umiTVAudio17getInputCodecTypeB5cxx11Ev");
extern bool isInputCodecDTS(Audio *) __asm__("_ZN10umiTVAudio15isInputCodecDTSEv");
extern int getEarcAndARCOutputCodec(Audio *) __asm__("_ZN10umiTVAudio24getEarcAndARCOutputCodecEv");
extern int getWowcastOutputCodec(Audio *) __asm__("_ZN10umiTVAudio21getWowcastOutputCodecEv");
extern void setOutputType(Audio *, int, int) __asm__("_ZN10umiTVAudio13setOutputTypeE25UMI_SNDOUT_OUTPUT_CODEC_T26sndout_output_mix_option_t");
extern void setWowCastOutputType(Audio *, int, int) __asm__("_ZN10umiTVAudio20setWowCastOutputTypeE25UMI_SNDOUT_OUTPUT_CODEC_T26sndout_output_mix_option_t");
extern bool setOutputTypeForDTS(Audio *) __asm__("_ZN10umiTVAudio19setOutputTypeForDTSEv");
extern bool isEarcCapabilityAvailable(Audio *, const std::string &) __asm__("_ZN10umiTVAudio25isEarcCapabilityAvailableERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE");
extern bool isWowCastCapabilityAvailable(Audio *, const std::string &) __asm__("_ZN10umiTVAudio28isWowCastCapabilityAvailableERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE");
extern int getOpticOutputCodec(Audio *) __asm__("_ZN10umiTVAudio19getOpticOutputCodecEv");
extern int getOpticMixOption(Audio *) __asm__("_ZN10umiTVAudio17getOpticMixOptionEv");
extern bool supportOutputChannel(Audio *, int) __asm__("_ZN10umiTVAudio20supportOutputChannelE25UMI_SNDOUT_OUTPUT_CODEC_T");
extern int getInputAudioChannelNumber(const Audio *) __asm__("_ZNK10umiTVAudio26getInputAudioChannelNumberEv");
extern bool isSupportIAMF(const Audio *) __asm__("_ZNK10umiTVAudio13isSupportIAMFEv");
extern bool isValidChannel(const Audio *, int) __asm__("_ZNK10umiTVAudio14isValidChannelEi");
extern bool setOpticOutputType(Audio *, int, int) __asm__("_ZN10umiTVAudio18setOpticOutputTypeE25UMI_SNDOUT_OUTPUT_CODEC_T26sndout_output_mix_option_t");
extern bool setArcOutputType(Audio *, int, int, int) __asm__("_ZN10umiTVAudio16setArcOutputTypeE25UMI_SNDOUT_OUTPUT_CODEC_T26sndout_output_mix_option_t27sndout_output_channel_num_t");
extern bool setEArcOutputType(Audio *, int, int, int) __asm__("_ZN10umiTVAudio17setEArcOutputTypeE25UMI_SNDOUT_OUTPUT_CODEC_T26sndout_output_mix_option_t27sndout_output_channel_num_t");
extern bool isCountryCodeJP(Audio *) __asm__("_ZN10umiTVAudio15isCountryCodeJPEv");
extern bool isInputSourceToSetAAC(const Audio *) __asm__("_ZNK10umiTVAudio21isInputSourceToSetAACEv");
extern bool isAACOutputCodec(Audio *) __asm__("_ZN10umiTVAudio16isAACOutputCodecEv");
extern bool isForcedDolbyNeeded(Audio *) __asm__("_ZN10umiTVAudio19isForcedDolbyNeededEv");
extern bool isInputCodecPCM(Audio *) __asm__("_ZN10umiTVAudio15isInputCodecPCMEv");

extern void disposeString(std::string *) __asm__("_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv");

union CapabilityString {
    std::string value;

    explicit CapabilityString(const char *text) : value(text) {}
    CapabilityString(const CapabilityString &other) : value(other.value) {}
    ~CapabilityString() { disposeString(&value); }
    operator const std::string &() const { return value; }
};

__attribute__((noinline)) static CapabilityString capabilityString(const char *text)
{
    return CapabilityString(text);
}

#define POLICY_SOURCE_FILE \
    "/usr/src/debug/lib32-umi/1.0.0-1.187.ptl4tv.13-r0/git/src/umitvaudiosettings.cpp"
#define POLICY_LOG_FILE(file, level, id, method, line, format, ...) \
    do { \
        if (!umiLogContext || umiLogContext->enabledLevel >= (level)) \
            _PmLogMsgKV(umiLogContext, level, 0, id, 0, nullptr, nullptr, \
                        "%s:%s(%d) " format, file, \
                        method, line, ##__VA_ARGS__); \
    } while (0)
#define POLICY_LOG(level, id, method, line, format, ...) \
    POLICY_LOG_FILE(std::strrchr(POLICY_SOURCE_FILE, '/') + 1, level, id, method, line, format, ##__VA_ARGS__)

__attribute__((noinline)) static const char *settingsSourceFile()
{
    return std::strrchr(POLICY_SOURCE_FILE, '/') + 1;
}

int getEarcAndARCOutputCodec(Audio *) __asm__("_ZN10umiTVAudio24getEarcAndARCOutputCodecEv");
/* @0x49440: AAC preference precedes forced Dolby; forced Dolby bypasses PCM input. */
int getEarcAndARCOutputCodec(Audio *self)
{
    if (isAACOutputCodec(self) && (isEarcCapabilityAvailable(self, "AAC") ||
                                  isEarcCapabilityAvailable(self, "MPEG4AACLC")))
        return 3;
    if (isForcedDolbyNeeded(self)) {
        if (isEarcCapabilityAvailable(self, "MAT2PCM"))
            return 7;
        if (isEarcCapabilityAvailable(self, "DD+"))
            return 5;
        return isEarcCapabilityAvailable(self, capabilityString("DD")) + 1;
    }
    if (isInputCodecPCM(self))
        return 1;
    if (isEarcCapabilityAvailable(self, capabilityString("MAT2PCM")))
        return 7;
    if (isEarcCapabilityAvailable(self, capabilityString("DD+")))
        return 5;
    return isEarcCapabilityAvailable(self, capabilityString("DD")) + 1;
}

int getWowcastOutputCodec(Audio *) __asm__("_ZN10umiTVAudio21getWowcastOutputCodecEv");
/* @0x49670: the same priority ladder reads only the independent wowcast capabilities. */
int getWowcastOutputCodec(Audio *self)
{
    if (isAACOutputCodec(self) && (isWowCastCapabilityAvailable(self, "AAC") ||
                                  isWowCastCapabilityAvailable(self, "MPEG4AACLC")))
        return 3;
    if (isForcedDolbyNeeded(self)) {
        if (isWowCastCapabilityAvailable(self, "MAT2PCM"))
            return 7;
        if (isWowCastCapabilityAvailable(self, "DD+"))
            return 5;
        return isWowCastCapabilityAvailable(self, capabilityString("DD")) + 1;
    }
    if (isInputCodecPCM(self))
        return 1;
    if (isWowCastCapabilityAvailable(self, capabilityString("MAT2PCM")))
        return 7;
    if (isWowCastCapabilityAvailable(self, capabilityString("DD+")))
        return 5;
    return isWowCastCapabilityAvailable(self, capabilityString("DD")) + 1;
}

void setOutputType(Audio *, int, int) __asm__("_ZN10umiTVAudio13setOutputTypeE25UMI_SNDOUT_OUTPUT_CODEC_T26sndout_output_mix_option_t");
/* @0x4fc24: optic precedes channel policy; all three writes run even after failure. */
void setOutputType(Audio *self, int codec, int mix)
{
    int opticCodec = getOpticOutputCodec(self);
    int opticMix = getOpticMixOption(self);
    if (!setOpticOutputType(self, opticCodec, opticMix)) {
        POLICY_LOG(3, "INFO_LOG", "setOutputType", 0x857,
                   "Optic outputType Error(codec: %d)", codec);
    }
    int channels = supportOutputChannel(self, codec);
    if (channels) {
        int inputChannels = getInputAudioChannelNumber(self);
        channels = inputChannels;
        if (isSupportIAMF(self) && isValidChannel(self, inputChannels) &&
            getInputCodecType(self).compare("iamf") == 0) {
            channels = inputChannels;
            mix = 1;
            POLICY_LOG(3, "INFO_LOG", "setOutputType", 0x862,
                       "Setting channel for IAMF (input ch:%d, output ch:%d, mixOption:%d)",
                       inputChannels, channels, mix);
        } else if (isValidChannel(self, inputChannels) && isValidChannel(self, self->pcmChannels)) {
            channels = inputChannels;
            if (self->pcmChannels < inputChannels) {
                channels = self->pcmChannels;
                mix = 1;
            }
            POLICY_LOG_FILE(settingsSourceFile(), 3, "INFO_LOG", "setOutputType", 0x86e,
                       "Setting channel (input ch:%d, dev ch:%d, output ch:%d, mixOption:%d)",
                       inputChannels, self->pcmChannels, channels, mix);
        } else {
            channels = 0;
            mix = 1;
            POLICY_LOG_FILE(settingsSourceFile(), 3, "INFO_LOG", "setOutputType", 0x873,
                       "channel is invalid (input ch: %d, dev ch:%d)", inputChannels, channels);
        }
    }
    if (!setArcOutputType(self, codec, mix, channels)) {
        POLICY_LOG(3, "INFO_LOG", "setOutputType", 0x87b,
                   "ARC outputType Error(codec: %d, ch:%d)", codec, channels);
    }
    if (!setEArcOutputType(self, codec, mix, channels)) {
        POLICY_LOG(3, "INFO_LOG", "setOutputType", 0x87e,
                   "eARC outputType Error(codec: %d, ch: %d)", codec, channels);
    }
}

bool setOutputTypeForDTS(Audio *) __asm__("_ZN10umiTVAudio19setOutputTypeForDTSEv");
/* @0x4ff14: each endpoint independently prefers DTSX_P2/P1, MA, DTS, then PCM. */
bool setOutputTypeForDTS(Audio *self)
{
    POLICY_LOG(7, nullptr, "setOutputTypeForDTS", 0x945, "setOutputCodec for DTS");
    int codec = 1;
    if (isEarcCapabilityAvailable(self, "DTSX_P2"))
        codec = 11;
    else if (isEarcCapabilityAvailable(self, "DTSX_P1"))
        codec = 11;
    else if (isEarcCapabilityAvailable(self, "DTSHD_MA"))
        codec = 10;
    else if (isEarcCapabilityAvailable(self, capabilityString("DTS")))
        codec = 4;
    setOutputType(self, codec, 1);
    codec = 1;
    if (isWowCastCapabilityAvailable(self, "DTSX_P2"))
        codec = 11;
    else if (isWowCastCapabilityAvailable(self, "DTSX_P1"))
        codec = 11;
    else if (isWowCastCapabilityAvailable(self, "DTSHD_MA"))
        codec = 10;
    else if (isWowCastCapabilityAvailable(self, capabilityString("DTS")))
        codec = 4;
    setWowCastOutputType(self, codec, 1);
    return true;
}

bool setOutputTypeForDolby(Audio *) __asm__("_ZN10umiTVAudio21setOutputTypeForDolbyEv");
/* @0x4feec: independent codec selection; output write failures are not propagated. */
bool setOutputTypeForDolby(Audio *self)
{
    setOutputType(self, getEarcAndARCOutputCodec(self), 1);
    setWowCastOutputType(self, getWowcastOutputCodec(self), 1);
    return true;
}

bool checkReEncodePriority(Audio *) __asm__("_ZN10umiTVAudio21checkReEncodePriorityEv");
/* @0x50180: raw Thumb dispatches on the original DTS predicate, not a new string lookup. */
bool checkReEncodePriority(Audio *self)
{
    if (isInputCodecDTS(self))
        return setOutputTypeForDTS(self);
    return setOutputTypeForDolby(self);
}

bool setOutputTypeForPassThrough(Audio *) __asm__("_ZN10umiTVAudio27setOutputTypeForPassThroughEv");
/* @0x501a0: either endpoint miss rewrites both; successful direct writes follow both lookups. */
bool setOutputTypeForPassThrough(Audio *self)
{
    std::string input = getInputCodecType(self);
    POLICY_LOG(7, nullptr, "setOutputTypeForPassThrough", 0x722,
               "input codec value is %s", input.c_str());
    if (input.empty())
        return checkReEncodePriority(self);
    int earcCodec = 1;
    int wowCodec = 1;
    bool earcFallback = false;
    bool wowFallback = false;
    if (isCountryCodeJP(self) && isInputSourceToSetAAC(self)) {
        if (input.compare("aac") == 0 && isEarcCapabilityAvailable(self, capabilityString("MPEG4AACLC")))
            earcCodec = 9;
        else if (input.compare("aac") == 0 && isEarcCapabilityAvailable(self, capabilityString("AAC")))
            earcCodec = 3;
        else {
            checkReEncodePriority(self);
            earcFallback = true;
        }
        if (input.compare("aac") == 0 && isWowCastCapabilityAvailable(self, capabilityString("MPEG4AACLC")))
            wowCodec = 9;
        else if (input.compare("aac") == 0 && isWowCastCapabilityAvailable(self, capabilityString("AAC")))
            wowCodec = 3;
        else
            goto wowMiss;
    } else {
        if ((input.compare("truehd") == 0 || input.compare("truehd_atmos") == 0) &&
            isEarcCapabilityAvailable(self, "TrueHD"))
            earcCodec = 6;
        else if ((input.compare("mat") == 0 || input.compare("mat_atmos") == 0) &&
                 isEarcCapabilityAvailable(self, capabilityString("MAT2PCM")))
            earcCodec = 7;
        else if ((input.compare("eac3") == 0 || input.compare("eac3_atmos") == 0) &&
                 isEarcCapabilityAvailable(self, capabilityString("DD+")))
            earcCodec = 5;
        else if (input.compare("ac3") == 0 && isEarcCapabilityAvailable(self, capabilityString("DD")))
            earcCodec = 2;
        else if (input.compare("pcm") == 0 && isEarcCapabilityAvailable(self, capabilityString("PCM")))
            earcCodec = 1;
        else if (input.compare("dts") == 0 && isEarcCapabilityAvailable(self, capabilityString("DTS")))
            earcCodec = 4;
        else if (input.compare("dts_hd") == 0 && isEarcCapabilityAvailable(self, capabilityString("DTSHD")))
            earcCodec = 8;
        else if (input.compare("dts_hd_ma") == 0 && isEarcCapabilityAvailable(self, capabilityString("DTSHD_MA")))
            earcCodec = 10;
        else if (input.compare("dts_x_p1") == 0 && isEarcCapabilityAvailable(self, capabilityString("DTSX_P1")))
            earcCodec = 11;
        else {
            checkReEncodePriority(self);
            earcFallback = true;
        }
        if ((input.compare("truehd") == 0 || input.compare("truehd_atmos") == 0) &&
            isWowCastCapabilityAvailable(self, "TrueHD"))
            wowCodec = 6;
        else if ((input.compare("mat") == 0 || input.compare("mat_atmos") == 0) &&
                 isWowCastCapabilityAvailable(self, capabilityString("MAT2PCM")))
            wowCodec = 7;
        else if ((input.compare("eac3") == 0 || input.compare("eac3_atmos") == 0) &&
                 isWowCastCapabilityAvailable(self, capabilityString("DD+")))
            wowCodec = 5;
        else if (input.compare("ac3") == 0 && isWowCastCapabilityAvailable(self, capabilityString("DD")))
            wowCodec = 2;
        else if (input.compare("pcm") == 0 && isWowCastCapabilityAvailable(self, capabilityString("PCM")))
            wowCodec = 1;
        else if (input.compare("dts") == 0 && isWowCastCapabilityAvailable(self, capabilityString("DTS")))
            wowCodec = 4;
        else if (input.compare("dts_hd") == 0 && isWowCastCapabilityAvailable(self, capabilityString("DTSHD")))
            wowCodec = 8;
        else if (input.compare("dts_hd_ma") == 0 && isWowCastCapabilityAvailable(self, capabilityString("DTSHD_MA")))
            wowCodec = 10;
        else if (input.compare("dts_x_p1") == 0 && isWowCastCapabilityAvailable(self, capabilityString("DTSX_P1")))
            wowCodec = 11;
        else
            goto wowMiss;
    }
    if (!earcFallback)
        goto writeEarc;
writeWow:
    setWowCastOutputType(self, wowCodec, self->mix != 0);
    return true;
wowMiss:
    {
        bool result = checkReEncodePriority(self);
        if (earcFallback)
            return result;
    }
    wowFallback = true;
writeEarc:
    setOutputType(self, earcCodec, self->mix != 0);
    if (!wowFallback)
        goto writeWow;
    return true;
}

}