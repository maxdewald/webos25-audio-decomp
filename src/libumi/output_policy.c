/*
 * libumi (loaded by audiooutputd): sound output / eARC policy.
 * RECONSTRUCTION from usr/lib/libumi.so.1.0.0
 *   sha256 f1ac7b61ac389af0329f36112e3b316c1f8575b1fecf79aed07986d1a873a764
 * audiooutputd sha256 026331391098d078a1941a95ec78fe8a23c05c5d944555c7bab431d8410d15f4
 * Addresses: Ghidra (= raw ELF VA + 0x10000). Evidence: ../../evidence/luna-contract/
 *
 * Luna entry: luna://com.webos.service.audiooutput/quality/setSettings
 *   {"category":"setMixer","value":{"mix":<bool>}}              -> mix_enabled
 *   {"category":"sound","value":{"soundOutputDigital":"passThrough"}}
 *   {"category":"external_arc","value":{"earcCapability":[...],
 *        "advancedEarcCapability":[{"audioCodingType":"PCM","maxChannelNumber":8}], ...}}
 * Luna entry: .../quality/setQualityAttribute
 *   {"categoryType":..,"requestType":"setAudioInputInfo",
 *    "pipelineInfo":{"pipelineId":"..","resource":".."},
 *    "categoryInfo":{"codec":"dts_hd_ma","channel":"8"}}       -> input_codec/channels
 */
#include <stdbool.h>
#include <string.h>
#include "../include/lg_audio_policy.h"

struct umi_tv_audio {
    bool  mix_enabled;          /* +0x7c, set by setMixerSettings("mix") @0x526f4 */
    bool  input_downmix;        /* +0x211, forces ct=PCM, mo=1 */
    int   device_pcm_channels;  /* +0x40, from advancedEarcCapability (0 if omitted!) */
    const char *input_codec;    /* categoryInfo "codec"  (map @+0x214 key "codec") */
    int   input_channels;       /* categoryInfo "channel" (stoi) */
    const char *digital_mode;   /* "pcm" | "auto" | "passThrough" */
    bool  harmony_output;       /* LG Sound Sync harmony: no direct pass-through */
    bool  japan;
    bool  iamf_supported;
};

/* Does the eARC sink advertise this capability token? (@0x48648) Case-sensitive. */
extern bool earc_cap(struct umi_tv_audio *a, const char *token);
extern bool aac_source_predicate(struct umi_tv_audio *a); /* HDMI/TV/DP/USB, or MEDIA atsc30/dtv */
extern bool set_alsa(const char *ctl, const int *vals, int n);

static bool is_valid_channel(int ch) { return ch == 2 || ch == 6 || ch == 8; } /* 0x144 mask */

static bool codec_is(const char *c, const char *a, const char *b)
{
    return c && (strcmp(c, a) == 0 || (b && strcmp(c, b) == 0));
}

/* @0x41b64 */
static bool setEArcOutputType(struct umi_tv_audio *a, int ct, int mo, int cn)
{
    (void)a;
    int w[5] = { 2, ct, cn, 0, mo };
    return set_alsa("Sndout EARC Outputtype", w, 5);
}

/* @0x4fc24: also writes optic + ARC; ARC writer only accepts ct {1,2,3,4,5,9}. */
static void setOutputType(struct umi_tv_audio *a, int ct, int mo)
{
    int cn = 0;
    bool pcm_like = codec_is(a->input_codec, "pcm", "iamf");

    /* supportOutputChannel @0x4916c: explicit cn only for pcm/iamf, ct=PCM, mode != "pcm" */
    if (ct == CT_PCM && pcm_like && strcmp(a->digital_mode, "pcm") != 0) {
        int in = a->input_channels;
        if (a->iamf_supported && strcmp(a->input_codec, "iamf") == 0 && is_valid_channel(in)) {
            cn = in;
            mo = MO_ALL;
        } else if (is_valid_channel(in) && is_valid_channel(a->device_pcm_channels)) {
            cn = in;
            if (a->device_pcm_channels < in) {   /* clamp to device forces mixing */
                cn = a->device_pcm_channels;
                mo = MO_ALL;
            }
        } else {
            cn = 0;
            mo = MO_ALL;
        }
    }
    setEArcOutputType(a, ct, mo, cn);
}

/* @0x4ff14: DTS fallback when the stream is DTS but direct pass-through is not chosen. */
static void setOutputTypeForDTS(struct umi_tv_audio *a)
{
    int ct = CT_PCM;
    if (earc_cap(a, "DTSX_P2") || earc_cap(a, "DTSX_P1")) ct = CT_DTSX_P1;
    else if (earc_cap(a, "DTSHD_MA"))                     ct = CT_DTSHD_MA;
    else if (earc_cap(a, "DTS"))                          ct = CT_DTS;
    setOutputType(a, ct, MO_ALL);
}

/* @0x49440: non-direct output codec when the input is not DTS. */
static int getEarcAndARCOutputCodec(struct umi_tv_audio *a)
{
    if (codec_is(a->input_codec, "pcm", NULL)) return CT_PCM;
    if (earc_cap(a, "MAT2PCM")) return CT_MAT2PCM;   /* suppressed under matprofile1 */
    if (earc_cap(a, "DD+"))     return CT_DDP;
    return earc_cap(a, "DD") ? CT_DD : CT_PCM;
}

/* @0x498a0 */
static bool is_input_dts(const char *c)
{
    static const char *const dts[] = { "dts", "dts_hd", "dts_hd_ma", "dts_cd",
                                       "dts_express", "dts_x_p1", "dts_x_p2" };
    for (unsigned i = 0; c && i < sizeof(dts) / sizeof(dts[0]); i++)
        if (strcmp(c, dts[i]) == 0)
            return true;
    return false;
}

/* @0x50180 */
static void checkReEncodePriority(struct umi_tv_audio *a)
{
    if (is_input_dts(a->input_codec))
        setOutputTypeForDTS(a);
    else
        setOutputType(a, getEarcAndARCOutputCodec(a), MO_ALL);
}

/* @0x501a0: the only path that emits mo=0. */
static void setOutputTypeForPassThrough(struct umi_tv_audio *a)
{
    static const struct { const char *in1, *in2, *cap; int ct; } direct[] = {
        { "truehd",    "truehd_atmos", "TrueHD",   CT_TRUEHD   },
        { "mat",       "mat_atmos",    "MAT2PCM",  CT_MAT2PCM  },
        { "eac3",      "eac3_atmos",   "DD+",      CT_DDP      },
        { "ac3",       NULL,           "DD",       CT_DD       },
        { "pcm",       NULL,           "PCM",      CT_PCM      },
        { "dts",       NULL,           "DTS",      CT_DTS      },
        { "dts_hd",    NULL,           "DTSHD",    CT_DTSHD    },
        { "dts_hd_ma", NULL,           "DTSHD_MA", CT_DTSHD_MA },
        { "dts_x_p1",  NULL,           "DTSX_P1",  CT_DTSX_P1  },
    };
    const char *c = a->input_codec;

    if (!c || !*c) {
        checkReEncodePriority(a);
        return;
    }
    if (a->japan && aac_source_predicate(a) && strcmp(c, "aac") == 0) {
        if (earc_cap(a, "MPEG4AACLC")) { setOutputType(a, CT_MPEG4AACLC, a->mix_enabled); return; }
        if (earc_cap(a, "AAC"))        { setOutputType(a, CT_AAC,        a->mix_enabled); return; }
        checkReEncodePriority(a);
        return;
    }
    for (unsigned i = 0; i < sizeof(direct) / sizeof(direct[0]); i++) {
        if (codec_is(c, direct[i].in1, direct[i].in2) && earc_cap(a, direct[i].cap)) {
            setOutputType(a, direct[i].ct, a->mix_enabled ? MO_ALL : MO_NONE);
            return;
        }
    }
    /* No capability match: fallback, never a hard reject. */
    checkReEncodePriority(a);
}

/* @0x50978: entry after any setting or input-info change (also via setSoundOutputDigital @0x50f00). */
void setAllSoundOutputDigital(struct umi_tv_audio *a, bool current_output_supports_digital)
{
    if (!current_output_supports_digital)
        a->digital_mode = "pcm";
    if (strcmp(a->digital_mode, "pcm") == 0 || a->input_downmix) {
        setOutputType(a, CT_PCM, MO_ALL);
        return;
    }
    if (strcmp(a->digital_mode, "passThrough") == 0 && !a->harmony_output) {
        setOutputTypeForPassThrough(a);
        return;
    }
    checkReEncodePriority(a);   /* "auto" */
}

/* @0x526f4: {"category":"setMixer","value":{"mix":b}} -- recompute only on change. */
void setMixerSettings(struct umi_tv_audio *a, bool mix)
{
    if (a->mix_enabled == mix)
        return;
    a->mix_enabled = mix;
    setAllSoundOutputDigital(a, true);
}
