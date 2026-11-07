/* Policy predicates recovered from libumi.so.1.0.0; Ghidra VA = ELF VA + 0x10000.
 * JSON/string adapters in policy.h deliberately avoid fabricating class layouts.
 * Logging and C++ temporary-string allocation are not reproduced here.
 */
#include "policy.h"
#include <stdlib.h>
#include <string.h>

static const char *nonnull(const char *value) { return value ? value : ""; }
static bool eq(const char *a, const char *b) { return strcmp(nonnull(a), b) == 0; }

/* @0x4910c: parser input map (+0x214 -> map +0x18), missing codec returns empty. */
const char *getInputCodecType(umi_tv_audio *a)
{
    return nonnull(umi_ops.parameter(a, true, "codec"));
}

/* @0x47ac4: numeric output eligibility; harmony depends on the device flag +0xf1. */
bool isSoundOutputSupportSoundOutputDigital(umi_tv_audio *a, int output)
{
    if (output == 2 || output == 0x400)
        return true;
    if (output == 0x400000)
        return umi_byte(a, UMI_HARMONY);
    return (((unsigned)output - 0x800000U) & 0xff7fffffU) == 0;
}

/* @0x596dc: effective mode changes to pcm on ineligible outputs, NOT the stored
 * soundOutputDigital parameter. Returning to eligible ARC restores that parameter.
 * Raw 0x496fc..0x49766 checks numeric output then parser map, not the mix byte. */
const char *getSoundOutputDigitalMode(umi_tv_audio *a)
{
    if (!isSoundOutputSupportSoundOutputDigital(a, umi_word(a, UMI_OUTPUT)))
        return "pcm";
    return nonnull(umi_ops.parameter(a, false, "soundOutputDigital"));
}

/* @0x34bcc: parser's lgHarmony string must be exactly "true"; +0xf1 is distinct. */
bool isLgHarmonySoundOutput(umi_tv_audio *a)
{
    return eq(umi_ops.parameter(a, false, "lgHarmony"), "true");
}

/* @0x48858: raw 0x38858 cmp r1,#8 / movw r0,#0x144 / lsr / and. */
bool isValidChannel(umi_tv_audio *a, int channels)
{
    (void)a;
    return (unsigned)channels < 9 ? (0x144U >> channels) & 1U : 0;
}

/* @0x5905c: either the local IAMF flag (+0x70) OR sink flag (+0x71), not AND. */
bool isSupportIAMF(umi_tv_audio *a)
{
    bool support = ((const bool *)a)[UMI_IAMF];
    if (!support) support = ((const bool *)a)[UMI_IAMF_EARC];
    return support;
}

/* @0x58efc: signed thresholds; 7 channels rounds UP to 8, 3..5 DOWN to 2. */
int convertDeviceChannelCapability(umi_tv_audio *a, int channels)
{
    (void)a;
    if (!channels)
        return 0;
    if (channels > 5)
        return channels == 6 ? 6 : 8;
    return 2;
}

/* @0x4916c: only literal pcm/iamf with ct=1 and effective mode != pcm. */
bool supportOutputChannel(umi_tv_audio *a, int codec)
{
    const char *input = getInputCodecType(a);
    return (eq(input, "pcm") || eq(input, "iamf")) && codec == CT_PCM &&
           !eq(getSoundOutputDigitalMode(a), "pcm");
}

/* PARTIAL 0x49f24: channel is parser string->stoi base10; absence => zero.
 * [GAP] C++ invalid_argument/out_of_range exceptions are adapter errors here;
 * the original throws for present malformed/overflowing strings (it does not
 * quietly turn them into a valid channel). Callers only accept 2,6,8. */
int getInputAudioChannelNumber(umi_tv_audio *a)
{
    const char *text = umi_ops.parameter(a, true, "channel");
    char *end;
    long value;
    if (!text)
        return 0;
    value = strtol(text, &end, 10);
    if (end == text || value > INT32_MAX || value < INT32_MIN)
        return 0;
    return (int)value;
}

/* @0x498a0: seven exact recognized DTS names; no prefix/case normalization. */
bool is_input_dts(const char *input)
{
    static const char *const names[] = { "dts", "dts_hd", "dts_hd_ma", "dts_cd",
                                        "dts_express", "dts_x_p1", "dts_x_p2" };
    for (unsigned i = 0; i < sizeof(names) / sizeof(names[0]); ++i)
        if (eq(input, names[i]))
            return true;
    return false;
}

/* @0x4923c: despite its name this is the COMPLEMENT of 20 compressed codec
 * names. Unknown codecs (including reported DTSHD), empty, pcm and iamf return
 * true. This corrects the old model's pcm-only fallback assumption. */
bool isInputCodecPCM(umi_tv_audio *a)
{
    static const char *const compressed[] = {
        "truehd", "truehd_atmos", "mat", "mat_atmos", "eac3", "eac3_atmos",
        "ac3", "mpeg_h", "aac", "heaac", "ac4", "ac4_atmos", "dts", "dts_hd",
        "dts_hd_ma", "dts_cd", "dts_express", "dts_x_p1", "dts_x_p2", "opus"
    };
    const char *input = getInputCodecType(a);
    for (unsigned i = 0; i < sizeof(compressed) / sizeof(compressed[0]); ++i)
        if (eq(input, compressed[i]))
            return false;
    return true;
}

/* @0x591bc: configuration tv.hw.countryType compared case-sensitively to JP. */
bool isCountryCodeJP(umi_tv_audio *a)
{
    return eq(umi_ops.country(a), "JP");
}

/* @0x49c8c: source eligibility also REQUIRES codec aac. MEDIA content accepts
 * atsc30, dtv_8k or dtv; HDMI, TV, DP, USB have no content restriction. */
bool isInputSourceToSetAAC(umi_tv_audio *a)
{
    const char *source = umi_ops.parameter(a, true, "sourceType");
    const char *content = umi_ops.parameter(a, true, "contentType");
    return eq(getInputCodecType(a), "aac") &&
           (eq(source, "HDMI") || eq(source, "TV") || eq(source, "DP") ||
            eq(source, "USB") || (eq(source, "MEDIA") &&
            (eq(content, "atsc30") || eq(content, "dtv_8k") || eq(content, "dtv"))));
}

/* @0x59254: Japan AAC encoding suppressed for harmony and AI generation >=6. */
bool isAACOutputCodec(umi_tv_audio *a)
{
    return isCountryCodeJP(a) && isInputSourceToSetAAC(a) &&
           !(umi_word(a, UMI_AI_GENERATION) >= 6 && isLgHarmonySoundOutput(a));
}

/* @0x59854: forced Dolby flag +0x81 applies to auto or harmony passThrough. */
bool isForcedDolbyNeeded(umi_tv_audio *a)
{
    const char *mode = getSoundOutputDigitalMode(a);
    return (eq(mode, "auto") || (eq(mode, "passThrough") &&
            isLgHarmonySoundOutput(a))) && umi_byte(a, UMI_OTT_DOLBY);
}

static bool json_has_token(const struct umi_value *value, const char *token)
{
    for (size_t i = 0; i < umi_ops.size(value); ++i) {
        const char *entry;
        if (umi_ops.as_string(umi_ops.element(value, i), &entry) && eq(entry, token))
            return true;
    }
    return false;
}

/* @0x48648: stored JValue +4 is indexed as an array, then each element is
 * compared directly to std::string (raw 0x38674/0x3867c), not audioCodingType.
 * json_has_token models that comparison for string entries; no coercion or
 * case normalization. MAT2PCM is hidden under exact matprofile1 (raw
 * 0x386a8/0x386b6). Logging inside that loop is omitted here. */
bool isEarcCapabilityAvailable(umi_tv_audio *a, const char *token)
{
    if (eq(umi_ops.string_at(a, UMI_MAT_PROFILE), "matprofile1") && eq(token, "MAT2PCM"))
        return false;
    return json_has_token(umi_ops.json_at(a, UMI_EARC_CAP), token);
}

/* @0x48790: wowcast list +0x10, with no MAT profile exclusion. */
bool isWowCastCapabilityAvailable(umi_tv_audio *a, const char *token)
{
    return json_has_token(umi_ops.json_at(a, UMI_WOW_CAP), token);
}

/* @0x41b4c: ARC output identifiers, distinct from the broader related predicate. */
bool isArcSoundOutput(umi_tv_audio *a)
{
    int output = umi_word(a, UMI_OUTPUT);
    return output == 0x400 || output == 0x400000;
}

/* @0x41f28: identifiers 0x800000 and 0x1000000. */
bool isWowCastSoundOutput(umi_tv_audio *a)
{
    return (((unsigned)umi_word(a, UMI_OUTPUT) - 0x800000U) & 0xff7fffffU) == 0;
}

/* @0x47af4: includes SE ARC and combined outputs as well as plain ARC. */
bool isCurrentSoundOutputRelatedARC(umi_tv_audio *a)
{
    int output = umi_word(a, UMI_OUTPUT);
    return output == 0x400000 || output == 0x80000 || output == 0x8000 || output == 0x400;
}

/* @0x47b20: same numeric pair as isWowCastSoundOutput. */
bool isCurrentSoundOutputRelatedWowCast(umi_tv_audio *a)
{
    return (((unsigned)umi_word(a, UMI_OUTPUT) - 0x800000U) & 0xff7fffffU) == 0;
}

/* @0x5902c: enable SP50 for profiles 1 or 2; other/empty profiles disable it. */
bool SP50ValueBaseOnMatProfile(umi_tv_audio *a)
{
    const char *profile = umi_ops.string_at(a, UMI_MAT_PROFILE);
    return eq(profile, "matprofile1") || eq(profile, "matprofile2");
}

/* @0x58f14: ALSA get failure => false; major>=3 or major=2,minor>=6. */
bool SupportSP50(umi_tv_audio *a)
{
    int version[5] = {0};
    if (!umi_ops.get_alsa(a, "Dolby MS12 IDK Version", version, 5))
        return false;
    return version[0] >= 3 || (version[0] == 2 && version[1] >= 6);
}

/* @0x47924: init flags reset does NOT reset mixer +0x7c or capability JValues.
 * Constructor @0x36f14 zeros +0x7c once. No other policy-family write exists. */
void resetInitFlags(umi_tv_audio *a)
{
    umi_set_byte(a, 0x7b, 0);
    umi_set_byte(a, 0x89, 0);
    umi_set_byte(a, 0x8c, 0);
    umi_set_byte(a, 0x20d, 0);
    uint16_t zero = 0;
    memcpy((uint8_t *)a + 0x72, &zero, sizeof(zero));
    umi_set_word(a, 0x74, 0);
    memcpy((uint8_t *)a + 0x78, &zero, sizeof(zero));
}
