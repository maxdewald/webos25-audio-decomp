/* Reconstructed from libumi.so.1.0.0, SHA256
 * f1ac7b61ac389af0329f36112e3b316c1f8575b1fecf79aed07986d1a873a764.
 * The former compact model omitted optic/wowcast writes and a cross-output
 * fallback which can overwrite an initially selected direct eARC codec. This
 * translation preserves those side effects; policy.h supplies opaque adapters.
 * Evidence: Ghidra export and raw Thumb in build/scratch/broad/policy/.
 * Logging/temporary std::strings are omitted, not claimed instruction-identical.
 */
#include "policy.h"
#include <string.h>

static bool eq(const char *a, const char *b) { return a && strcmp(a, b) == 0; }

static int encoded_codec(umi_tv_audio *a, bool wow)
{
    bool (*cap)(umi_tv_audio *, const char *) = wow ? isWowCastCapabilityAvailable : isEarcCapabilityAvailable;
    if (isAACOutputCodec(a) && (cap(a, "AAC") || cap(a, "MPEG4AACLC")))
        return CT_AAC;
    if (!isForcedDolbyNeeded(a) && isInputCodecPCM(a))
        return CT_PCM;
    if (cap(a, "MAT2PCM")) return CT_MAT2PCM;
    if (cap(a, "DD+")) return CT_DDP;
    return cap(a, "DD") ? CT_DD : CT_PCM;
}

/* @0x49440: AAC preference, optional forced Dolby, then MAT > DD+ > DD > PCM. */
int getEarcAndARCOutputCodec(umi_tv_audio *a)
{
    return encoded_codec(a, false);
}

/* @0x49670: same priorities as eARC, using independent wowcast capabilities. */
int getWowcastOutputCodec(umi_tv_audio *a)
{
    return encoded_codec(a, true);
}

/* @0x49ea8: optic encodes non-PCM-like/non-DTS input to DD (JP AAC -> AAC). */
int getOpticOutputCodec(umi_tv_audio *a)
{
    if (eq(getSoundOutputDigitalMode(a), "pcm") || umi_byte(a, UMI_DOWNMIX))
        return CT_PCM;
    if (isForcedDolbyNeeded(a)) return CT_DD;
    if (isInputCodecPCM(a)) return CT_PCM;
    if (is_input_dts(getInputCodecType(a))) return CT_DTS;
    return isCountryCodeJP(a) && isInputSourceToSetAAC(a) ? CT_AAC : CT_DD;
}

/* @0x4f920: optic mix-off only for exact ac3/aac/dts whose selected ct matches.
 * PCM is absent from this map: optic PCM always mixes, even when eARC does not. */
int getOpticMixOption(umi_tv_audio *a)
{
    const char *input = getInputCodecType(a);
    int expected = eq(input, "ac3") ? CT_DD : eq(input, "aac") ? CT_AAC :
                   eq(input, "dts") ? CT_DTS : 0;
    return !(eq(getSoundOutputDigitalMode(a), "passThrough") &&
             !umi_byte(a, UMI_MIX) && expected && expected == getOpticOutputCodec(a));
}

static int channel_and_mix(umi_tv_audio *a, int ct, int device_channels, int *mo)
{
    if (!supportOutputChannel(a, ct)) return 0;
    int channels = getInputAudioChannelNumber(a);
    if (isSupportIAMF(a) && isValidChannel(a, channels) && eq(getInputCodecType(a), "iamf")) {
        *mo = MO_ALL;
        return channels;
    }
    if (!isValidChannel(a, channels) || !isValidChannel(a, device_channels)) {
        *mo = MO_ALL;
        return 0;
    }
    if (device_channels < channels) {
        *mo = MO_ALL;
        return device_channels;
    }
    return channels;
}

/* @0x41b64: cache request and eARC state BEFORE ALSA write, even on failure.
 * [2,ct,cn,0,mo], then optional SP50; delay notification only after success.
 * Sole writer of the control; libumi never reads it back. The shipped kernel
 * rejects ct 10/11 (-EINVAL), so it keeps the previous [st,ct,cn,sr,mo]. */
bool setEArcOutputType(umi_tv_audio *a, int ct, int mo, int cn)
{
    int words[5] = {2, ct, cn, 0, mo};
    umi_set_word(a, UMI_REQUEST_CODEC, ct);
    umi_set_word(a, UMI_REQUEST_MIX, mo);
    umi_set_word(a, UMI_EARC_CODEC, ct);
    umi_set_word(a, UMI_EARC_CHANNEL, cn);
    umi_set_word(a, UMI_EARC_MIX, mo);
    if (!umi_ops.set_alsa(a, "Sndout EARC Outputtype", words, 5)) return false;
    if (ct == CT_MAT2PCM && SupportSP50(a)) {
        int sp50 = SP50ValueBaseOnMatProfile(a);
        if (!umi_ops.set_alsa(a, "Sndout EARC MS12 SP50 OnOff", &sp50, 1)) return false;
    }
    if (umi_word(a, UMI_OUTPUT) == 0x80000)
        umi_ops.delay_codec(a, CT_PCM);
    else if (isArcSoundOutput(a) && umi_word(a, UMI_EARC_ON))
        umi_ops.delay_codec(a, ct);
    return true;
}

/* @0x41f9c: unsupported ARC ct is silently ignored with SUCCESS (not failure). */
bool setArcOutputType(umi_tv_audio *a, int ct, int mo, int cn)
{
    int words[5] = {2, ct, cn, 0, mo};
    if (!(ct == 1 || ct == 2 || ct == 3 || ct == 4 || ct == 5 || ct == 9)) return true;
    umi_set_word(a, UMI_ARC_CODEC, ct);
    if (!umi_ops.set_alsa(a, "Sndout ARC Outputtype", words, 5)) return false;
    if (umi_word(a, UMI_OUTPUT) == 0x80000)
        umi_ops.delay_codec(a, CT_PCM);
    else if (isArcSoundOutput(a) && umi_word(a, UMI_ARC_ON))
        umi_ops.delay_codec(a, ct);
    return true;
}

/* @0x422e8: optic supports ct 1..4; both channel/rate words always zero. */
bool setOpticOutputType(umi_tv_audio *a, int ct, int mo)
{
    int words[5] = {2, ct, 0, 0, mo};
    if (ct < 1 || ct > 4) return true;
    if (!umi_ops.set_alsa(a, "Sndout OPTIC Outputtype", words, 5)) return false;
    if (umi_word(a, UMI_OUTPUT) == 0x1000)
        umi_ops.delay_codec(a, CT_PCM);
    else if (umi_word(a, UMI_OUTPUT) == 2)
        umi_ops.delay_codec(a, ct);
    return true;
}

/* @0x4a00c: wowcast independently clamps channels against +0x44, not +0x40. */
void setWowCastOutputType(umi_tv_audio *a, int ct, int mo)
{
    int cn = channel_and_mix(a, ct, umi_word(a, UMI_WOW_CHANNELS), &mo);
    int words[5] = {2, ct, cn, 0, mo};
    umi_set_word(a, UMI_WOW_CODEC, ct);
    umi_set_word(a, UMI_WOW_CHANNEL, cn);
    umi_set_word(a, UMI_WOW_MIX, mo);
    umi_ops.set_alsa(a, "Sndout WOWCAST Outputtype", words, 5);
    if (isWowCastSoundOutput(a)) umi_ops.delay_codec(a, ct);
}

/* @0x4fc24: optic first, then ARC/eARC. mix=false can survive only when PCM
 * channels 2/6/8 and device channels 2/6/8 with device>=input. Missing/invalid
 * channels, clamping, or supported IAMF FORCE mo=1, without changing +0x7c.
 * Raw 0x3fd7c/0x3fe20 sets r7=1; 0x3fe38..0x3fe44 only sets it on dev<input.
 * When supportOutputChannel=false, cn=0 and the incoming mo is untouched. */
void setOutputType(umi_tv_audio *a, int output_codec, int mix_option)
{
    setOpticOutputType(a, getOpticOutputCodec(a), getOpticMixOption(a));
    int channels = channel_and_mix(a, output_codec, umi_word(a, UMI_PCM_CHANNELS), &mix_option);
    setArcOutputType(a, output_codec, mix_option, channels);
    setEArcOutputType(a, output_codec, mix_option, channels);
}

static int dts_fallback(umi_tv_audio *a, bool wow)
{
    bool (*cap)(umi_tv_audio *, const char *) = wow ? isWowCastCapabilityAvailable : isEarcCapabilityAvailable;
    if (cap(a, "DTSX_P2") || cap(a, "DTSX_P1")) return CT_DTSX_P1;
    if (cap(a, "DTSHD_MA")) return CT_DTSHD_MA;
    return cap(a, "DTS") ? CT_DTS : CT_PCM;
}

/* @0x4ff14: both outputs always rewritten; fallback never uses mixer byte. */
bool setOutputTypeForDTS(umi_tv_audio *a)
{
    setOutputType(a, dts_fallback(a, false), MO_ALL);
    setWowCastOutputType(a, dts_fallback(a, true), MO_ALL);
    return true;
}

/* @0x4feec: Dolby fallback is separately selected for eARC and wowcast. */
bool setOutputTypeForDolby(umi_tv_audio *a)
{
    setOutputType(a, getEarcAndARCOutputCodec(a), MO_ALL);
    setWowCastOutputType(a, getWowcastOutputCodec(a), MO_ALL);
    return true;
}

/* @0x50180: unknown/case-mismatched DTS names take the non-DTS fallback. */
bool checkReEncodePriority(umi_tv_audio *a)
{
    return is_input_dts(getInputCodecType(a)) ? setOutputTypeForDTS(a) : setOutputTypeForDolby(a);
}

static int direct_codec(umi_tv_audio *a, bool wow)
{
    static const struct { const char *input, *alternate, *token; int codec; } direct[] = {
        {"truehd", "truehd_atmos", "TrueHD", CT_TRUEHD},
        {"mat", "mat_atmos", "MAT2PCM", CT_MAT2PCM},
        {"eac3", "eac3_atmos", "DD+", CT_DDP},
        {"ac3", 0, "DD", CT_DD}, {"pcm", 0, "PCM", CT_PCM},
        {"dts", 0, "DTS", CT_DTS}, {"dts_hd", 0, "DTSHD", CT_DTSHD},
        {"dts_hd_ma", 0, "DTSHD_MA", CT_DTSHD_MA},
        {"dts_x_p1", 0, "DTSX_P1", CT_DTSX_P1}
    };
    bool (*cap)(umi_tv_audio *, const char *) = wow ? isWowCastCapabilityAvailable : isEarcCapabilityAvailable;
    const char *input = getInputCodecType(a);
    if (isCountryCodeJP(a) && isInputSourceToSetAAC(a)) {
        if (eq(input, "aac") && cap(a, "MPEG4AACLC")) return CT_MPEG4AACLC;
        if (eq(input, "aac") && cap(a, "AAC")) return CT_AAC;
        return 0;
    }
    for (unsigned i = 0; i < sizeof(direct) / sizeof(direct[0]); ++i)
        if ((eq(input, direct[i].input) || (direct[i].alternate && eq(input, direct[i].alternate))) &&
            cap(a, direct[i].token)) return direct[i].codec;
    return 0;
}

/* @0x501a0: direct mo comes from +0x7c, but capability lookup is INDEPENDENT
 * for eARC and wowcast. A miss calls checkReEncodePriority, which rewrites BOTH
 * outputs; an eARC miss falls back first, and a wowcast miss falls back again
 * (two fallbacks when BOTH miss). Final direct eARC write follows fallback.
 * Thus eARC match+wowcast miss restores direct eARC after global fallback;
 * eARC miss+wowcast match retains fallback eARC and directly rewrites wowcast.
 * Raw 0x40304/0x40306, 0x40436, 0x40836 corroborate the two success flags.
 * No direct rule for iamf, dts_cd, dts_express or dts_x_p2. */
bool setOutputTypeForPassThrough(umi_tv_audio *a)
{
    if (!*getInputCodecType(a)) return checkReEncodePriority(a);
    int earc = direct_codec(a, false);
    if (!earc) checkReEncodePriority(a);
    int wow = direct_codec(a, true);
    if (!wow) checkReEncodePriority(a);
    if (earc) setOutputType(a, earc, umi_byte(a, UMI_MIX) ? MO_ALL : MO_NONE);
    if (wow) setWowCastOutputType(a, wow, umi_byte(a, UMI_MIX) ? MO_ALL : MO_NONE);
    return true;
}

/* @0x50978: pcm/downmix -> mixed PCM; passThrough+harmony -> fallback; auto
 * -> fallback. Unknown/empty modes succeed WITHOUT writing any output. */
bool setAllSoundOutputDigital(umi_tv_audio *a, const char *mode)
{
    if (eq(mode, "pcm") || umi_byte(a, UMI_DOWNMIX)) {
        setOutputType(a, CT_PCM, MO_ALL);
        setWowCastOutputType(a, CT_PCM, MO_ALL);
    } else if (eq(mode, "passThrough")) {
        if (!isLgHarmonySoundOutput(a)) return setOutputTypeForPassThrough(a);
        checkReEncodePriority(a);
    } else if (eq(mode, "auto")) {
        checkReEncodePriority(a);
    }
    return true;
}

/* @0x50f00: reapply even when mode unchanged. Changing output changes effective
 * mode, not mixer state. Previous mode +0x1f4 and delay are bookkeeping only;
 * +0x213 always requests delay update but suppresses previous-mode assignment.
 * External reapply sites (pseudocode): setSoundOutput @0x36904 after effective
 * output-id change (same output normally returns early); setInputDownMix
 * @0x3305c on each request; updateCodecImmersiveFormat @0x425d0 when codec key
 * exists and numeric output supports digital; setQualityAttributeSettings
 * @0x5110c when soundOutputDigital is the sole key. Capability callbacks and
 * successful mixer/forceOutput/arcProfile/mpeg4AACLC/lgHarmony/OTT-mode changes
 * also reapply. No notification subscription is created by this policy family.
 * Each reapply recomputes current predicates; +0x7c is NOT a hardware latch. */
bool setSoundOutputDigital(umi_tv_audio *a, const char *mode)
{
    bool encode = eq(mode, "auto") || (eq(mode, "passThrough") && isLgHarmonySoundOutput(a));
    umi_ops.ott_mode(a, encode && umi_byte(a, UMI_OTT_DOLBY), encode && umi_byte(a, UMI_OTT_DTS));
    bool result = setAllSoundOutputDigital(a, mode);
    if (umi_byte(a, UMI_ALWAYS_DELAY)) {
        umi_ops.delay_update(a);
    } else if (!eq(umi_ops.string_at(a, UMI_PREVIOUS_MODE), mode)) {
        umi_ops.assign_string(a, UMI_PREVIOUS_MODE, mode);
        umi_ops.delay_update(a);
    }
    return result;
}

/* @0x50a84: replace retained JSON +4, then recompute using effective mode;
 * does not store settings or reset mixer. JValue assignment retains ownership. */
bool updateEarcCapability(umi_tv_audio *a, const struct umi_value *value)
{
    umi_ops.assign_json(a, UMI_EARC_CAP, value);
    return setAllSoundOutputDigital(a, getSoundOutputDigitalMode(a));
}

/* @0x50ac8: independent retained wowcast JSON +0x10; recomputes both outputs. */
bool updateWowCastCapability(umi_tv_audio *a, const struct umi_value *value)
{
    umi_ops.assign_json(a, UMI_WOW_CAP, value);
    return setAllSoundOutputDigital(a, getSoundOutputDigitalMode(a));
}

/* @0x526f4: setMixer value.mix updates ONLY +0x7c, then reapplies effective
 * mode iff changed. Raw 0x427ae..0x427c6 proves compare/store/call order.
 * The original ignores asBool's return and reads an uninitialized temporary on
 * malformed mix; [GAP] invalid types rejected here rather than emulating UB.
 * No file/database/Luna persistence occurs in this method. */
bool setMixerSettings(umi_tv_audio *a, const struct umi_value *value)
{
    bool mix;
    if (!umi_ops.has(value, "mix")) return false;
    if (!umi_ops.as_bool(umi_ops.member(value, "mix"), &mix)) return false;
    if (umi_byte(a, UMI_MIX) == mix) return true;
    umi_set_byte(a, UMI_MIX, mix);
    return setSoundOutputDigital(a, getSoundOutputDigitalMode(a));
}
