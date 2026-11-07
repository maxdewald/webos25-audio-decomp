/* RapidJSON-assisted reconstruction of codec info readers. Raw ARM confirms
 * HasMember followed by operator[] (two member searches), with one more header
 * lookup only when decoding, no numeric type guard, and a strict float-epsilon
 * comparison against rates expressed in kHz.
 * Missing fields and unrecognized rates leave the corresponding output untouched.
 */
#include "audio_metadata.h"
#include <glib.h>
#include <cmath>
#include <cstring>
#include <limits>

#if UINTPTR_MAX == UINT32_MAX
static_assert(sizeof(rapidjson::Value) == 16, "shipped RapidJSON ARM layout");
static_assert(sizeof(MEDIA_AUDIO_OPUS_INFO_T) == 16, "codec metadata ARM layout");
#endif

namespace mediapipeline {
namespace {
bool sampleRate(double rate, MEDIA_AUDIO_SAMPLERATE &result)
{
    static const struct { double khz; MEDIA_AUDIO_SAMPLERATE value; } rates[] = {
        {0, SAMPLE_RATE_UNKNOWN}, {48, SAMPLE_RATE_48}, {32, SAMPLE_RATE_32},
        {24, SAMPLE_RATE_24}, {16, SAMPLE_RATE_16}, {12, SAMPLE_RATE_12},
        {8, SAMPLE_RATE_8}, {22.05, SAMPLE_RATE_22_05}, {44.1, SAMPLE_RATE_44_1}
    };
    for (const auto &entry : rates) {
        if (std::fabs(float(rate) - float(entry.khz)) < std::numeric_limits<float>::epsilon()) {
            result = entry.value;
            return true;
        }
    }
    return false;
}

template<class Info> void streamInfo(Info &out, const rapidjson::Value &value)
{
    if (value.HasMember("channels"))
        out.channels = value["channels"].GetInt();
    if (value.HasMember("sampleRate"))
        sampleRate(value["sampleRate"].GetDouble(), out.sampleRate);
    if (value.HasMember("streamHeader")) {
        const auto &header = value["streamHeader"];
        if (!header.IsString() || header.GetStringLength() == 0)
            return;
        gsize length;
        out.header = g_base64_decode(value["streamHeader"].GetString(), &length);
        out.headerSize = length;
    }
}
}

/* @0xd2658: AC3PLUS calls its rate field frequency, unlike the other three. */
void setAC3PLUSinfo(MEDIA_AUDIO_AC3PLUS_INFO_T &out, const rapidjson::Value &value)
{
    if (value.HasMember("channels"))
        out.channels = value["channels"].GetInt();
    if (value.HasMember("frequency"))
        sampleRate(value["frequency"].GetDouble(), out.sampleRate);
}

/* @0xd285c */
void setOPUSinfo(MEDIA_AUDIO_OPUS_INFO_T &out, const rapidjson::Value &value)
{
    streamInfo(out, value);
}

/* @0xd2ba8 */
void setIAMFinfo(MEDIA_AUDIO_IAMF_INFO_T &out, const rapidjson::Value &value)
{
    streamInfo(out, value);
}

/* @0xd2ef4 */
void setVORBISinfo(MEDIA_AUDIO_VORBIS_INFO_T &out, const rapidjson::Value &value)
{
    streamInfo(out, value);
}

/* @0xd3774 */
unsigned transSampleRate(MEDIA_AUDIO_SAMPLERATE rate)
{
    static const unsigned hz[] = {48000, 44100, 32000, 24000, 16000, 12000, 8000, 22050};
    unsigned index = unsigned(rate) - 1;
    return index < 8 ? hz[index] : 0;
}

/* Extracted predicate from GenericPipeline::handleBusApplication (0xe1f5c),
 * not a reconstruction of the full handler. decproxy may occur anywhere. */
bool dropGenericMediaInfo(const char *structureName, const char *sourceName)
{
    return structureName && std::strcmp(structureName, "media-info") == 0 &&
        sourceName && std::strstr(sourceName, "decproxy");
}

/* Partial projection of gstBusCallbackHandle 0x80694, handleBusApplication
 * 0x8f6e4/0xe1f5c and handleAudioInfo 0x8e410; excluded from coverage.
 * Caller supplies TrackInfo::getAudioTrackIndexByStreamId (0x986f4)'s result.
 * [GAP] Gst object ownership, locking, JSON
 * serialization and callbacks at +0x14/+0x53c are outside this projection.
 * Valid string GValues are required; original malformed string inputs may fail.
 * Field presence, not a media-info type field or source factory, selects audio. */
bool readAudioInfoEvent(const AudioInfoMessage &message, bool genericPipeline, int trackIndex,
    bool platformSupportDolbyAtmos, rapidjson::Document &event)
{
    if (!message.hasPlayerHandle || message.messageType != 0x4000 || !message.structureName ||
        std::strcmp(message.structureName, "media-info") != 0 ||
        (genericPipeline && dropGenericMediaInfo(message.structureName, message.sourceName)))
        return false;
    if (!message.streamId || !g_value_get_string(message.streamId) ||
        !(message.dualMono || message.immersive || message.codecName || message.outChannels))
        return false;
    event.SetObject();
    auto &allocator = event.GetAllocator();
    event.AddMember("track", trackIndex, allocator);
    if (message.dualMono)
        event.AddMember("dualMono", g_value_get_boolean(message.dualMono) != FALSE, allocator);
    if (message.immersive) {
        const char *immersive = g_value_get_string(message.immersive);
        if (!*immersive || !platformSupportDolbyAtmos) immersive = "none";
        event.AddMember("immersive", rapidjson::Value(immersive, allocator), allocator);
    }
    if (message.codecName) {
        const char *codec = g_value_get_string(message.codecName);
        if (std::strstr(codec, "dts"))
            event.AddMember("codec-name", rapidjson::Value(codec, allocator), allocator);
    }
    if (message.outChannels) {
        int channels = G_VALUE_HOLDS_INT(message.outChannels) ? g_value_get_int(message.outChannels) : -1;
        event.AddMember("out-channels", channels, allocator);
    }
    return true;
}

/* Partial projection of CustomPipeline::handleBusApplication 0xceae4 and
 * TrackInfo::getAudioTrackIndexByStreamId 0x986f4; excluded from coverage.
 * Only a single placeholder "0" is repaired, and out-channels cannot trigger it.
 * [GAP] Malformed string inputs and Gst ownership/locking are not reproduced. */
bool readCustomAudioInfoEvent(const AudioInfoMessage &message, std::vector<std::string> &streamIds,
    bool platformSupportDolbyAtmos, rapidjson::Document &event)
{
    if (!message.hasPlayerHandle || message.messageType != 0x4000 || !message.structureName ||
        std::strcmp(message.structureName, "media-info") != 0 || !message.streamId)
        return false;
    const char *streamId = g_value_get_string(message.streamId);
    if (!streamId) return false;
    if (streamIds.size() == 1 && streamIds[0] == "0" &&
        (message.dualMono || message.immersive || message.codecName))
        streamIds[0] = streamId;
    int trackIndex = -1;
    for (std::size_t index = 0; index < streamIds.size(); ++index) {
        if (streamIds[index] == streamId) { trackIndex = static_cast<int>(index); break; }
    }
    return readAudioInfoEvent(message, false, trackIndex, platformSupportDolbyAtmos, event);
}
}
