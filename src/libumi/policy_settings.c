/* libumi setting dispatch and lifetime. Raw branches were checked against the
 * Ghidra export; object storage is addressed only by verified ARM offsets.
 */
#include "policy.h"
#include <string.h>

static bool eq(const char *a, const char *b) { return a && b && strcmp(a, b) == 0; }

/* @0x52554: forceOutputDDPLUS == "true" toggles +0x7d and reapplies on change.
 * This flag is not the independent force-OTT-Dolby flag +0x81. */
bool setforceOutputSettings(umi_tv_audio *a, const struct umi_value *value)
{
    const char *text = "";
    if (!umi_ops.has(value, "forceOutputDDPLUS")) return false;
    umi_ops.as_string(umi_ops.member(value, "forceOutputDDPLUS"), &text);
    bool force = eq(text, "true");
    if (umi_byte(a, UMI_FORCE_DDPLUS) == force) return true;
    umi_set_byte(a, UMI_FORCE_DDPLUS, force);
    return setSoundOutputDigital(a, getSoundOutputDigitalMode(a));
}

/* @0x52a6c: successful category handlers cache the full value in the in-memory
 * settings vector via storeSettings @0x4a690. That function strips returnValue
 * and subscribe, removes the previous category entry and appends {category,value}
 * at +0xd8..+0xdc. Neither it nor setMixerSettings writes disk. getInstance
 * @0x37188 constructs a guarded static singleton with __aeabi_atexit cleanup;
 * deinitAudio @0x2fc50 only clears the resource vector end, NOT this cache or
 * mixer. Lifetime is normally the service process, not a request/pipeline.
 * Startup callers may replay settings; persistence across service restart/boot
 * outside this object is NOT established by these functions. */
bool setSoundOutputDigitalSettings(umi_tv_audio *a, const char *category, const struct umi_value *value)
{
    bool result;
    if (eq(category, "forceOutput"))
        result = setforceOutputSettings(a, value);
    else if (eq(category, "setMixer"))
        result = setMixerSettings(a, value);
    else if (eq(category, "soundOutputMode"))
        result = umi_ops.sound_output_mode(a, value);
    else
        return false;
    if (result) umi_ops.store_settings(a, category, value);
    return result;
}

/* @0x52b90: ordering matters. advancedEarcCapability and supportIamf reset on
 * EVERY request (omitted => 0/false), while earcCapability only changes when
 * present. This is a replace-style capability event, not a safe partial merge.
 * Raw 0x42cc4 rejects asNumber failures, signed-max at 0x42ccc, unconditional
 * +0x40 store at 0x42d24; +0x71 cleared on omission at 0x42d96.
 * Capability update/recompute PRECEDES earcMatProfile assignment: profile-only
 * updates do not immediately reapply. earcCapability+new profile first uses OLD
 * profile. Bool conversion failures originally read uninitialized locals for
 * supportIamf/earcATMOS; [GAP] rejected/defaulted here, not guessed.
 * mpeg4AACLC and lgHarmony may each trigger an earlier reapply. JSON ownership
 * is retained by assign_json, not borrowed from the incoming Luna request. */
bool setExternalArcSettings(umi_tv_audio *a, const struct umi_value *value)
{
    const struct umi_value *advanced = umi_ops.member(value, "advancedEarcCapability");
    int maximum = 0;
    if (umi_ops.is_array(advanced)) {
        for (size_t i = 0; i < umi_ops.size(advanced); ++i) {
            const struct umi_value *item = umi_ops.element(advanced, i);
            const char *coding;
            int channels = 0;
            if (umi_ops.as_string(umi_ops.member(item, "audioCodingType"), &coding) &&
                eq(coding, "PCM") &&
                umi_ops.as_int(umi_ops.member(item, "maxChannelNumber"), &channels) &&
                channels > maximum)
                maximum = channels;
        }
    }
    umi_set_word(a, UMI_PCM_CHANNELS, convertDeviceChannelCapability(a, maximum));

    const char *activation, *device;
    if (umi_ops.as_string(umi_ops.member(value, "arcActivation"), &activation) &&
        umi_ops.as_string(umi_ops.member(value, "audioDeviceType"), &device)) {
        umi_ops.assign_string(a, UMI_ACTIVATION, activation);
        umi_ops.assign_string(a, UMI_DEVICE_TYPE, device);
        umi_ops.arc_delay(a);
    }

    bool flag = false;
    if (umi_ops.has(value, "supportIamf"))
        umi_ops.as_bool(umi_ops.member(value, "supportIamf"), &flag);
    umi_set_byte(a, UMI_IAMF_EARC, flag);

    bool result = true;
    const char *profile = "none";
    if (umi_ops.as_string(umi_ops.member(value, "arcProfile"), &profile) &&
        umi_ops.as_bool(umi_ops.member(value, "mpeg4AACLC"), &flag)) {
        if (!eq(umi_ops.string_at(a, UMI_ARC_PROFILE), profile) ||
            umi_byte(a, UMI_MPEG4_AAC) != flag) {
            umi_ops.assign_string(a, UMI_ARC_PROFILE, profile);
            umi_set_byte(a, UMI_MPEG4_AAC, flag);
            result = setSoundOutputDigital(a, getSoundOutputDigitalMode(a));
        }
    }
    if (umi_ops.has(value, "lgHarmony") &&
        umi_ops.as_bool(umi_ops.member(value, "lgHarmony"), &flag)) {
        umi_set_byte(a, UMI_HARMONY, flag);
        setSoundOutputDigital(a, getSoundOutputDigitalMode(a));
    }

    bool stored = true;
    if (umi_ops.has(value, "earcCapability")) {
        updateEarcCapability(a, umi_ops.member(value, "earcCapability"));
        stored = umi_ops.store_parameters(a, value);
    }
    if (umi_ops.has(value, "earcATMOS") &&
        umi_ops.as_bool(umi_ops.member(value, "earcATMOS"), &flag)) {
        umi_set_byte(a, UMI_ARC_ATMOS, flag);
        umi_ops.atmos_capability(a);
    }
    if (umi_ops.has(value, "earcMatProfile")) {
        const char *mat = "";
        umi_ops.as_string(umi_ops.member(value, "earcMatProfile"), &mat);
        umi_ops.assign_string(a, UMI_MAT_PROFILE, mat);
    }
    return result && stored;
}
