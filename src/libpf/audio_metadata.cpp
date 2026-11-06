/* RapidJSON-assisted reconstruction of codec info readers. Raw ARM confirms
 * HasMember followed by operator[] (two member searches), with one more header
 * lookup only when decoding, no numeric type guard, and a strict float-epsilon
 * comparison against rates expressed in kHz.
 * Missing fields and unrecognized rates leave the corresponding output untouched.
 */
#include "audio_metadata.h"
#include <glib.h>
#include <cmath>
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
}
