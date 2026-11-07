/* Recovered metadata types. ARM layout assertions describe the shipped C++11 ABI;
 * reporting contexts below are explicit projections, not MediaApiPrivate layouts. */
#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <boost/property_tree/ptree.hpp>

namespace smp {
using Ptree = boost::property_tree::ptree;
struct AudioTrack {
    bool audioDescription = false;
    int audioType = 0, bitRate = 0, channels = 0, ctag = 0, pid = 0;
    int trackId = 0;
    float sampleRate = 0;
    std::string codec, reserved38, language, level, profile, role, immersive;
    int adaptationSetId = 0;
    bool dualMono = false, selected = false;
    int periodStart = 0;
    Ptree makeAudioTrackPtree() const;
};
struct VideoTrack {
    int angleNumber = 0, bitRate = 0, ctag = 0, pid = 0, trackId = 0, width = 0, height = 0;
    float frameRate = 0;
    std::string aspectRatio, codec, mimeType, level, reserved80, profile, role;
    int adaptationSetId = 0;
    bool progressive = false;
    std::string orgCodec;
    bool selected = false;
    int periodStart = 0;
    bool isHFR() const;
    Ptree makeVideoTrackPtree() const;
};
struct HdrInfo {
    int reserved = 0, type = 0, transferCharacteristics = -1, colorPrimaries = -1, matrixCoeffs = -1;
    int displayPrimariesX[3] = {-1, -1, -1}, displayPrimariesY[3] = {-1, -1, -1};
    int whitePointX = -1, whitePointY = -1, minLuminance = -1, maxLuminance = -1;
    int maxContentLightLevel = -1, maxPicAverageLightLevel = -1;
    bool reserved44 = false, videoFullRangeFlag = false;
};
namespace util {
void putString(Ptree *, const char *, std::string) __asm__("putString");
void putInt(Ptree *, const char *, int) __asm__("putInt");
void putFloat(Ptree *, const char *, float) __asm__("putFloat");
void putBool(Ptree *, const char *, bool) __asm__("putBool");
const char *getRgbRangeString(int);
Ptree generateVuiPtree(const HdrInfo *);
Ptree generateSeiPtree(const HdrInfo *);
bool parseJson(const char *, Ptree *);
std::string generateJsonString(const Ptree &);
}
/* Projection of generateAudioDecodeInfo's verified reads: private +0x16c/170
 * resources, +0x18c codec-name, +0x3cc reset; source +0x38 immersive, +0x50
 * output channels; selected public AudioTrack +0x20 codec and +0xc channels. */
struct AudioReportingContext {
    std::string pipelineId, streamType, contentType, sourceAttribute;
    Ptree pipelinePurpose;
    Ptree audioInfoEvent;
    std::string audioInfoDescription;
    int sourceType = 0;
    int adecPort = -1, mixerPort = -1, audioTrackCount = 0, videoTrackCount = 0;
    int videoSinkType = 0, outChannels = 0;
    const AudioTrack *selectedTrack = nullptr;
    std::string codecName, immersive, internalSourceImmersive, params;
    bool reset = false, isGameMode = false, hasAudioDropControl = false, disableAudioDrop = false;
};
struct AudioInfoEventCallbacks {
    std::function<void(const std::string &, const std::string &)> setDecodeInfo;
    std::function<void(int, std::int64_t, const std::string &)> sendCallbackEvent;
};
struct VideoReportingContext {
    std::string pipelineId, contentType, hdrType, appStatus, aviContentType;
    int playerType = 0, path = 0, width = 0, height = 0, pixelWidth = 0, pixelHeight = 0;
    int afd = 0, rgbRange = 0, rotation = 0, scanType = 0;
    float frameRate = 0;
    bool adaptive = false, autoAspectRatio = false, subScalerOccupied = false;
    bool lowDelayMode = false, allmEnabled = false, isGameMode = false;
    bool hasAdaptiveTracks = false, adaptiveHfr = false;
    HdrInfo hdr;
};
bool isAudioOnly(int) __asm__("isAudioOnly");
std::string generateStreamTypeForAudioD(int, bool) __asm__("generateStreamTypeForAudioD");
std::string getContentType(const std::string &, const std::string &, bool, int) __asm__("getContentType");
Ptree generateAudioDecodeInfo(const AudioReportingContext &) __asm__("generateAudioDecodeInfo");
Ptree generateVideoDecodeInfo(const VideoReportingContext &) __asm__("generateVideoDecodeInfo");
/* Event projection: the caller's AVProxy submission and logging are external.
 * Returns whether the recovered change test requests a fresh audio report. */
bool handleAudioInfoEvent(AudioReportingContext &, AudioTrack *, int, const Ptree &) __asm__("handleAudioInfoEvent");
void callbackFunctionHook(AudioReportingContext &, AudioTrack *, int, int, std::int64_t,
    const char *, const AudioInfoEventCallbacks &);
}
namespace smp_internal {
using smp::Ptree;
struct AudioTrack {
    std::string mimeType, profile, level;
    float sampleRate = 0;
    int channels = 0;
    std::string language;
    int pid = 0, ctag = 0;
    bool audioDescription = false;
    int audioType = 0;
    std::string codec;
    int trackId = 0, bitRate = 0;
    std::string role;
    int adaptationSetId = 0;
    bool reservedb4 = false;
    std::string immersive;
    bool selected = false;
    int periodStart = 0;
    std::string representationId, preselectionId, ac4Text, ratings;
    int selectionPriority = 0;
    bool audioEmergency = false;
    void fromString(const std::string &);
    bool fromJsonString(const std::string &);
    Ptree makeAudioTrackPtree() const;
};
struct VideoTrack {
    int angleNumber = 0, bitRate = 0, ctag = 0, pid = 0, trackId = 0, width = 0, height = 0;
    float frameRate = 0;
    std::string aspectRatio, codec, mimeType, level, reserved80, profile, role;
    int adaptationSetId = 0;
    bool progressive = false;
    std::string orgCodec;
    bool selected = false;
    int periodStart = 0;
    std::string representationId, ratings;
    int selectionPriority = 0;
    void fromString(const std::string &);
    bool fromJsonString(const std::string &);
    Ptree makeVideoTrackPtree() const;
};
}
#if defined(__arm__)
static_assert(sizeof(std::string) == 0x18, "shipped C++11 string ABI");
static_assert(sizeof(smp::AudioTrack) == 0xd4, "public audio track stride");
static_assert(offsetof(smp::AudioTrack, codec) == 0x20, "reported codec");
static_assert(offsetof(smp::AudioTrack, channels) == 0xc, "reported channels");
static_assert(offsetof(smp::AudioTrack, immersive) == 0xb0, "track immersive");
static_assert(offsetof(smp::AudioTrack, sampleRate) == 0x1c, "sample rate");
static_assert(offsetof(smp::AudioTrack, profile) == 0x80, "public audio profile");
static_assert(offsetof(smp::AudioTrack, role) == 0x98, "public audio role");
static_assert(offsetof(smp::AudioTrack, selected) == 0xcd, "public audio selected");
static_assert(sizeof(smp::VideoTrack) == 0xf0, "public video track stride");
static_assert(offsetof(smp::VideoTrack, orgCodec) == 0xd0, "original video codec");
static_assert(sizeof(smp_internal::AudioTrack) == 0x140, "internal audio track stride");
static_assert(offsetof(smp_internal::AudioTrack, codec) == 0x78, "internal codec");
static_assert(offsetof(smp_internal::AudioTrack, ratings) == 0x120, "serialized ratings");
static_assert(offsetof(smp_internal::AudioTrack, audioEmergency) == 0x13c, "emergency flag");
static_assert(sizeof(smp_internal::VideoTrack) == 0x124, "internal video track stride");
static_assert(offsetof(smp_internal::VideoTrack, profile) == 0x98, "video profile");
static_assert(offsetof(smp::HdrInfo, videoFullRangeFlag) == 0x45, "HDR full range");
#endif
