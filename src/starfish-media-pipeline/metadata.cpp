/* Offline reconstruction from shipped starfish-media-pipeline (Thumb/Ghidra).
 * Context arguments project verified object fields; they are NOT replacement ABI
 * layouts for MediaApiPrivate. No custom hdbitstream artifact was available.
 * Traces do not establish which caps or codec-name a custom player emits. */
#include "metadata.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <regex>
#include <sstream>
#include <cstdlib>
#include <vector>

namespace smp { namespace util {
void putString(Ptree *, const char *, std::string) __asm__("putString");
/* @0x8a3c8: empty and literal (null) strings are absent, not JSON null values. */
void putString(Ptree *tree, const char *key, std::string value)
{
    if (!value.empty() && value != "(null)") tree->put(key, value);
}
void putInt(Ptree *, const char *, int) __asm__("putInt");
/* @0x8a454: integer zero is omitted by putIfPresent<int>, negatives survive. */
void putInt(Ptree *tree, const char *key, int value)
{
    if (value != 0) tree->put(key, value);
}
void putFloat(Ptree *, const char *, float) __asm__("putFloat");
/* @0x8a290: zero sample/frame rates are omitted, not serialized as defaults. */
void putFloat(Ptree *tree, const char *key, float value)
{
    if (value != 0.0f) tree->put(key, value);
}
void putBool(Ptree *, const char *, bool) __asm__("putBool");
/* @0x8a4b4: false flags are omitted by putIfPresent<bool>. */
void putBool(Ptree *tree, const char *key, bool value)
{
    if (value) tree->put(key, value);
}
/* @0x7fdd8: only range enum 1 reports FULL. */
const char *getRgbRangeString(int range)
{
    return range == 1 ? "FULL" : "LIMITED";
}
/* @0x80f7c: negative VUI values are absent; HDR type 4 adds full-range flag. */
Ptree generateVuiPtree(const HdrInfo *hdr)
{
    Ptree tree;
    if (hdr->transferCharacteristics >= 0) tree.put("transferCharacteristics", hdr->transferCharacteristics);
    if (hdr->colorPrimaries >= 0) tree.put("colorPrimaries", hdr->colorPrimaries);
    if (hdr->matrixCoeffs >= 0) tree.put("matrixCoeffs", hdr->matrixCoeffs);
    if (hdr->type == 4) tree.put("videoFullRangeFlag", hdr->videoFullRangeFlag);
    return tree;
}
/* @0x811bc: zero is a valid SEI value; each negative mastering field is absent. */
Ptree generateSeiPtree(const HdrInfo *hdr)
{
    Ptree tree;
    const char *keys[] = {"displayPrimariesX0", "displayPrimariesX1", "displayPrimariesX2",
        "displayPrimariesY0", "displayPrimariesY1", "displayPrimariesY2", "whitePointX",
        "whitePointY", "minDisplayMasteringLuminance", "maxDisplayMasteringLuminance",
        "maxContentLightLevel", "maxPicAverageLightLevel"};
    const int values[] = {hdr->displayPrimariesX[0], hdr->displayPrimariesX[1], hdr->displayPrimariesX[2],
        hdr->displayPrimariesY[0], hdr->displayPrimariesY[1], hdr->displayPrimariesY[2],
        hdr->whitePointX, hdr->whitePointY, hdr->minLuminance, hdr->maxLuminance,
        hdr->maxContentLightLevel, hdr->maxPicAverageLightLevel};
    for (unsigned i = 0; i < 12; ++i) if (values[i] >= 0) tree.put(keys[i], values[i]);
    return tree;
}
/* @0x81584: parse errors return false. File logging/exception diagnostics omitted. */
bool parseJson(const char *text, Ptree *tree)
{
    if (!text || !*text) return false;
    try {
        std::istringstream input(text);
        boost::property_tree::read_json(input, *tree);
        return true;
    } catch (const boost::property_tree::json_parser_error &) {
        return false;
    }
}
/* @0x83b84: property_tree scalar strings matching null/bool/decimal are unquoted.
 * Semantic reconstruction uses std::regex instead of the shipped Boost regex;
 * exception-file logging is omitted. Numeric-looking channel strings become numbers. */
std::string generateJsonString(const Ptree &tree)
{
    try {
        std::ostringstream output;
        boost::property_tree::write_json(output, tree, false);
        return std::regex_replace(output.str(), std::regex("\"(null|true|false|-?[0-9]+(\\.[0-9]+)?)\""), "$1");
    } catch (const boost::property_tree::json_parser_error &) {
        return "{}";
    }
}
} // util

bool isAudioOnly(int) __asm__("isAudioOnly");
/* @0x43f38: audio-only application types are 2,3,5,6,12 (not codec-dependent). */
bool isAudioOnly(int appType)
{
    unsigned type = static_cast<unsigned>(appType);
    return appType == 12 || type - 5 < 2 || type - 2 < 2;
}
std::string generateStreamTypeForAudioD(int, bool) __asm__("generateStreamTypeForAudioD");
/* @0x43f54: Bluetooth overrides app-type stream naming. */
std::string generateStreamTypeForAudioD(int appType, bool bluetooth)
{
    std::string value;
    if (appType == 10) value = "umioverlay";
    else if (appType == 12) value = "umimediamusic";
    else value = "umimedia";
    if (bluetooth) value = "umibluetooth";
    return value;
}
std::string getContentType(const std::string &, const std::string &, bool, int) __asm__("getContentType");
/* @0x43fe0: app/platform/player type select content class, not DTS/PCM caps. */
std::string getContentType(const std::string &appId, const std::string &soc, bool is8k, int playerType)
{
    if (appId == "com.webos.app.livedmost") return "ipch";
    if (is8k && soc == "o208k") return playerType == 0x116 ? "dtv_8k" : "movie_8k";
    if (playerType == 0x116 || static_cast<unsigned>(playerType) - 0x119 < 2 ||
        static_cast<unsigned>(playerType) - 0x113 < 2 || appId == "com.webos.app.recordings") return "atsc30";
    return "movie";
}
Ptree generateAudioDecodeInfo(const AudioReportingContext &) __asm__("generateAudioDecodeInfo");
/* Partial model of 0x58630, deliberately excluded from reconstruction coverage:
 * category codec is selected public track codec unless codec-name is nonempty,
 * in which case codec-name wins. Exact x-raw -> pcm; mpeg-h -> mpeg_h.
 * [GAP] Context projects private-object reads; this returns the report tree,
 * not the original serialized string, and consumes precomputed stream/content types.
 * Raw 0x48e88..48e9e: find("dts",0,3) returns into r0, then r0 is overwritten;
 * no branch, suffix test, profile check, or dts_hd_ma assignment follows it.
 * Thus dts/DTSHD/dts_hd_ma survive verbatim. This does not identify custom-player caps. */
Ptree generateAudioDecodeInfo(const AudioReportingContext &context)
{
    Ptree pipeline, category, message, game;
    pipeline.put("pipelineId", context.pipelineId);
    pipeline.put("streamType", context.streamType);
    pipeline.put("contentType", context.contentType);
    pipeline.put("sourceType", "MEDIA");
    if (context.adecPort != -1) pipeline.put("resource", "ADEC" + std::to_string(context.adecPort));
    if (context.mixerPort != -1) pipeline.put("resource", "AMIXER" + std::to_string(context.mixerPort));
    if (!context.pipelinePurpose.empty()) pipeline.put_child("pipelinePurpose", context.pipelinePurpose);
    pipeline.put("sourceAttribute", context.sourceAttribute);
    pipeline.put("reset", context.reset);
    game.put("isGameMode", context.isGameMode);
    pipeline.put_child("gameInfo", game);
    if (context.audioTrackCount < 1 || context.reset) category.put("codec", "none");
    else {
        const AudioTrack &track = *context.selectedTrack;
        std::string codec = track.codec;
        if (!context.codecName.empty()) codec = context.codecName;
        if (codec == "x-raw") codec = "pcm";
        else if (codec == "mpeg-h") codec = "mpeg_h";
        else (void)codec.find("dts", 0, 3);
        category.put("codec", codec);
        category.put("channel", std::to_string(context.outChannels > 0 ? context.outChannels : track.channels));
    }
    category.put("immersive", context.immersive == "ATMOS" && !context.reset ? "atmos" : "none");
    if (!context.params.empty()) {
        Ptree values, value;
        value.put_value(context.params);
        values.push_back({"", value});
        category.put_child("params", values);
    }
    pipeline.put("pipelinetype", context.audioTrackCount > 0 && context.videoTrackCount == 0 ? "audio" :
        context.videoSinkType == 0x10 ? "graphic" : "audiovideo");
    pipeline.put("videoType", context.videoSinkType == 2 ? "main_video" :
        context.videoSinkType == 4 ? "sub_video" : context.videoSinkType == 0x10 ? "graphic" : "unknown");
    if (context.hasAudioDropControl) pipeline.put("audioDropEnable", !context.disableAudioDrop);
    message.put("categoryType", "audioInputInfo");
    message.put("requestType", "setAudioInputInfo");
    message.put_child("pipelineInfo", pipeline);
    message.put_child("categoryInfo", category);
    return message;
}
Ptree generateVideoDecodeInfo(const VideoReportingContext &) __asm__("generateVideoDecodeInfo");
/* Partial model of 0x529c4, deliberately excluded from reconstruction coverage:
 * video decode metadata includes dimensions, HDR/VUI/SEI and game flags.
 * [GAP] Context is a snapshot: caller supplies HDR name/adaptive HFR; original
 * locks private +0x39c while reading aviContentType. This returns a report tree
 * rather than the original serialized string and does not reproduce locking. */
Ptree generateVideoDecodeInfo(const VideoReportingContext &context)
{
    Ptree info, pixel, message;
    info.put("adaptive", context.playerType != 0x116 && context.adaptive);
    info.put("path", context.path == 1 ? "network" : "file");
    info.put("autoAspectRatio", context.autoAspectRatio);
    info.put("subScalerOccupied", context.subScalerOccupied);
    info.put("lowDelayMode", context.lowDelayMode);
    pixel.put("width", context.pixelWidth);
    pixel.put("height", context.pixelHeight);
    info.put_child("pixelAspectRatio", pixel);
    info.put("afd", context.afd);
    info.put("rgbRange", util::getRgbRangeString(context.rgbRange));
    info.put("rotation", context.rotation == 90 ? "Deg90" : context.rotation == 180 ? "Deg180" :
        context.rotation == 270 ? "Deg270" : "Deg0");
    info.put("hfrEnabled", context.hasAdaptiveTracks ? context.adaptiveHfr :
        context.frameRate > 60.0f && context.width * context.height > 0x1fe000);
    info.put("hdrType", context.hdrType);
    Ptree sei = util::generateSeiPtree(&context.hdr), vui = util::generateVuiPtree(&context.hdr);
    if (!sei.empty()) info.put_child("sei", sei);
    if (!vui.empty()) info.put_child("vui", vui);
    if (context.allmEnabled) info.put("allmEnabled", context.allmEnabled);
    if (context.isGameMode) info.put("isGameMode", context.isGameMode);
    if (!context.appStatus.empty()) info.put("appStatus", context.appStatus);
    if (!context.aviContentType.empty()) info.put("aviContentType", context.aviContentType);
    message.put("context", context.pipelineId);
    message.put("contentType", context.contentType);
    message.put("frameRate", context.frameRate);
    message.put("scanType", context.scanType == 0 ? "progressive" : "interlaced");
    message.put("width", context.width);
    message.put("height", context.height);
    message.put_child("videoInfo", info);
    return message;
}
bool handleAudioInfoEvent(AudioReportingContext &, AudioTrack *, int, const Ptree &) __asm__("handleAudioInfoEvent");
/* Partial model of 0x595bc, deliberately excluded from reconstruction coverage:
 * only immersive/codec-name/out-channels changes cause decode resubmission.
 * dualMono updates the selected public track but does not request a report.
 * Cache +0x420 is refreshed even for inactive sources or rejected tracks.
 * Changed immersive also updates internal source +0x74.
 * [GAP] callback/logging and object ownership are external to this projection. */
bool handleAudioInfoEvent(AudioReportingContext &context, AudioTrack *tracks, int count, const Ptree &event)
{
    context.audioInfoEvent = event;
    int index = event.get<int>("track");
    /* Host safety: original 0x5965e checks only count > index (signed). */
    if (context.sourceType == -1 || index < 0 || index >= count) return false;
    if (auto value = event.get_optional<bool>("dualMono")) tracks[index].dualMono = *value;
    bool changed = false;
    if (auto value = event.get_optional<std::string>("immersive")) {
        if (context.immersive != *value) {
            context.immersive = *value;
            context.internalSourceImmersive = *value;
            changed = true;
        }
        if (context.audioInfoDescription != "Dolby Atmos") context.audioInfoEvent.put("immersive", "none");
    }
    if (auto value = event.get_optional<std::string>("codec-name")) {
        if (context.codecName != *value) { context.codecName = *value; changed = true; }
    }
    if (auto value = event.get_optional<int>("out-channels")) {
        if (context.outChannels != *value) { context.outChannels = *value; changed = true; }
    }
    return changed;
}
/* Partial recovery of 0x5b984, deliberately excluded from reconstruction coverage:
 * case 7 accepts nonempty JSON, not a GstMessage; numeric payload is forwarded.
 * Handler tail 0x59a06 submits audio decode info before the cached event callback.
 * +0x408 == "Dolby Atmos" preserves cached immersive; report uses source +0x38.
 * [GAP] Only audio-info dispatch is recovered; function objects project AVProxy
 * and sendCallbackEvent boundaries, not the shipped callback ABI or logging. */
void callbackFunctionHook(AudioReportingContext &context, AudioTrack *tracks, int count,
    int eventType, std::int64_t number, const char *text, const AudioInfoEventCallbacks &callbacks)
{
    if (eventType != 7 || !text || !*text) return;
    Ptree event;
    std::istringstream input(text);
    boost::property_tree::read_json(input, event);
    if (handleAudioInfoEvent(context, tracks, count, event)) {
        callbacks.setDecodeInfo("audio", util::generateJsonString(generateAudioDecodeInfo(context)));
    }
    callbacks.sendCallbackEvent(eventType, number, util::generateJsonString(context.audioInfoEvent));
}
/* @0x6b4e0: HFR requires >60 fps AND more than 1920*1088 pixels. */
bool VideoTrack::isHFR() const
{
    return frameRate > 60.0f && width * height > 0x1fe000;
}
/* @0x6e284: public track report uses codec verbatim, never infers a DTS profile. */
Ptree AudioTrack::makeAudioTrackPtree() const
{
    Ptree tree;
    tree.put("bitRate", bitRate);
    util::putString(&tree, "language", language);
    util::putString(&tree, "codec", codec);
    util::putString(&tree, "profile", profile);
    util::putString(&tree, "level", level);
    util::putFloat(&tree, "sampleRate", sampleRate);
    util::putInt(&tree, "channels", channels);
    util::putInt(&tree, "pid", pid);
    util::putInt(&tree, "ctag", ctag);
    util::putBool(&tree, "audioDescription", audioDescription);
    util::putString(&tree, "role", role);
    util::putInt(&tree, "adaptationSetId", adaptationSetId);
    tree.put("audioType", audioType);
    util::putInt(&tree, "trackId", trackId);
    util::putString(&tree, "immersive", immersive);
    util::putBool(&tree, "selected", selected);
    tree.put("periodStart", periodStart);
    return tree;
}
/* @0x6dd20: public video emits angleNumber/periodStart even when zero. */
Ptree VideoTrack::makeVideoTrackPtree() const
{
    Ptree tree;
    tree.put("angleNumber", angleNumber);
    util::putString(&tree, "codec", codec);
    util::putString(&tree, "profile", profile);
    util::putString(&tree, "level", level);
    util::putString(&tree, "aspectRatio", aspectRatio);
    util::putFloat(&tree, "frameRate", frameRate);
    util::putInt(&tree, "bitRate", bitRate);
    util::putInt(&tree, "pid", pid);
    util::putInt(&tree, "ctag", ctag);
    util::putBool(&tree, "progressive", progressive);
    util::putInt(&tree, "trackId", trackId);
    util::putString(&tree, "role", role);
    util::putInt(&tree, "adaptationSetId", adaptationSetId);
    util::putString(&tree, "orgCodec", orgCodec);
    util::putInt(&tree, "width", width);
    util::putInt(&tree, "height", height);
    util::putBool(&tree, "selected", selected);
    tree.put("periodStart", periodStart);
    return tree;
}
} // smp
