/* Semantic reconstruction boundary, not a declaration of LG's C++ class layout.
 * The audio object is opaque. Only shipped ARM offsets are read/written; string,
 * JSON, parser and virtual calls remain explicit adapter operations below.
 */
#ifndef UMI_POLICY_H
#define UMI_POLICY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../include/lg_audio_policy.h"

typedef void umi_tv_audio;
struct umi_value;
struct umi_policy_ops {
    const char *(*parameter)(umi_tv_audio *, bool input, const char *key);
    const char *(*string_at)(umi_tv_audio *, unsigned offset);
    void (*assign_string)(umi_tv_audio *, unsigned offset, const char *);
    const struct umi_value *(*json_at)(umi_tv_audio *, unsigned offset);
    void (*assign_json)(umi_tv_audio *, unsigned offset, const struct umi_value *);
    bool (*has)(const struct umi_value *, const char *);
    const struct umi_value *(*member)(const struct umi_value *, const char *);
    bool (*is_array)(const struct umi_value *);
    size_t (*size)(const struct umi_value *);
    const struct umi_value *(*element)(const struct umi_value *, size_t);
    bool (*as_string)(const struct umi_value *, const char **);
    bool (*as_bool)(const struct umi_value *, bool *);
    bool (*as_int)(const struct umi_value *, int *);
    bool (*set_alsa)(umi_tv_audio *, const char *, const int *, size_t);
    bool (*get_alsa)(umi_tv_audio *, const char *, int *, size_t);
    void (*delay_codec)(umi_tv_audio *, int);
    void (*delay_update)(umi_tv_audio *);
    void (*ott_mode)(umi_tv_audio *, bool, bool);
    void (*arc_delay)(umi_tv_audio *);
    void (*atmos_capability)(umi_tv_audio *);
    bool (*store_parameters)(umi_tv_audio *, const struct umi_value *);
    void (*store_settings)(umi_tv_audio *, const char *, const struct umi_value *);
    bool (*sound_output_mode)(umi_tv_audio *, const struct umi_value *);
    const char *(*country)(umi_tv_audio *);
};
extern const struct umi_policy_ops umi_ops;

/* ARM object offsets confirmed by stores/loads, not host struct member positions. */
enum {
    UMI_EARC_CAP = 4, UMI_WOW_CAP = 0x10,
    UMI_ARC_ON = 0x1c, UMI_EARC_ON = 0x20,
    UMI_PCM_CHANNELS = 0x40, UMI_WOW_CHANNELS = 0x44,
    UMI_EARC_CODEC = 0x48, UMI_EARC_CHANNEL = 0x4c, UMI_EARC_MIX = 0x50,
    UMI_WOW_CODEC = 0x54, UMI_WOW_CHANNEL = 0x58, UMI_WOW_MIX = 0x5c,
    UMI_ARC_CODEC = 0x60, UMI_REQUEST_CODEC = 0x64, UMI_REQUEST_MIX = 0x68,
    UMI_IAMF = 0x70, UMI_IAMF_EARC = 0x71, UMI_MIX = 0x7c,
    UMI_FORCE_DDPLUS = 0x7d, UMI_OTT_DOLBY = 0x81, UMI_OTT_DTS = 0x82,
    UMI_ARC_PROFILE = 0xa8, UMI_MPEG4_AAC = 0xc0, UMI_OUTPUT = 0xc4,
    UMI_HARMONY = 0xf1, UMI_ARC_ATMOS = 0x12b, UMI_ACTIVATION = 0x12c,
    UMI_DEVICE_TYPE = 0x144, UMI_WOW_ON = 0x15c, UMI_AI_GENERATION = 0x190,
    UMI_MAT_PROFILE = 0x1c4, UMI_PREVIOUS_MODE = 0x1f4,
    UMI_DOWNMIX = 0x211, UMI_ALWAYS_DELAY = 0x213
};
static inline uint8_t umi_byte(const umi_tv_audio *a, unsigned offset)
{
    return ((const uint8_t *)a)[offset];
}
static inline void umi_set_byte(umi_tv_audio *a, unsigned offset, uint8_t value)
{
    ((uint8_t *)a)[offset] = value;
}
static inline int umi_word(const umi_tv_audio *a, unsigned offset)
{
    int value;
    memcpy(&value, (const uint8_t *)a + offset, sizeof(value));
    return value;
}
static inline void umi_set_word(umi_tv_audio *a, unsigned offset, int value)
{
    memcpy((uint8_t *)a + offset, &value, sizeof(value));
}
const char *getInputCodecType(umi_tv_audio *);
const char *getSoundOutputDigitalMode(umi_tv_audio *);
bool isValidChannel(umi_tv_audio *, int);
bool isInputCodecPCM(umi_tv_audio *);
bool is_input_dts(const char *);
bool isInputSourceToSetAAC(umi_tv_audio *);
bool isCountryCodeJP(umi_tv_audio *);
bool isLgHarmonySoundOutput(umi_tv_audio *);
bool isAACOutputCodec(umi_tv_audio *);
bool isForcedDolbyNeeded(umi_tv_audio *);
bool isEarcCapabilityAvailable(umi_tv_audio *, const char *);
bool isWowCastCapabilityAvailable(umi_tv_audio *, const char *);
bool isSoundOutputSupportSoundOutputDigital(umi_tv_audio *, int);
bool isArcSoundOutput(umi_tv_audio *);
bool isWowCastSoundOutput(umi_tv_audio *);
bool isCurrentSoundOutputRelatedARC(umi_tv_audio *);
bool isCurrentSoundOutputRelatedWowCast(umi_tv_audio *);
bool supportOutputChannel(umi_tv_audio *, int);
bool isSupportIAMF(umi_tv_audio *);
int getInputAudioChannelNumber(umi_tv_audio *);
int convertDeviceChannelCapability(umi_tv_audio *, int);
int getEarcAndARCOutputCodec(umi_tv_audio *);
int getWowcastOutputCodec(umi_tv_audio *);
int getOpticOutputCodec(umi_tv_audio *);
int getOpticMixOption(umi_tv_audio *);
bool SupportSP50(umi_tv_audio *);
bool SP50ValueBaseOnMatProfile(umi_tv_audio *);
bool setEArcOutputType(umi_tv_audio *, int, int, int);
bool setArcOutputType(umi_tv_audio *, int, int, int);
bool setOpticOutputType(umi_tv_audio *, int, int);
void setWowCastOutputType(umi_tv_audio *, int, int);
void setOutputType(umi_tv_audio *, int, int);
bool setOutputTypeForDTS(umi_tv_audio *);
bool setOutputTypeForDolby(umi_tv_audio *);
bool checkReEncodePriority(umi_tv_audio *);
bool setOutputTypeForPassThrough(umi_tv_audio *);
bool setAllSoundOutputDigital(umi_tv_audio *, const char *);
bool setSoundOutputDigital(umi_tv_audio *, const char *);
bool updateEarcCapability(umi_tv_audio *, const struct umi_value *);
bool updateWowCastCapability(umi_tv_audio *, const struct umi_value *);
bool setMixerSettings(umi_tv_audio *, const struct umi_value *);
bool setforceOutputSettings(umi_tv_audio *, const struct umi_value *);
bool setSoundOutputDigitalSettings(umi_tv_audio *, const char *, const struct umi_value *);
bool setExternalArcSettings(umi_tv_audio *, const struct umi_value *);
void resetInitFlags(umi_tv_audio *);
#endif
