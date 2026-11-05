#ifndef LIBPF_AUDIO_METADATA_H
#define LIBPF_AUDIO_METADATA_H
#include <rapidjson/document.h>
#include <cstdint>

/* Recovered prefixes; sampleRate is the LG enum, not a rate in hertz. */
enum MEDIA_AUDIO_SAMPLERATE : int {
    SAMPLE_RATE_UNKNOWN, SAMPLE_RATE_48, SAMPLE_RATE_44_1, SAMPLE_RATE_32,
    SAMPLE_RATE_24, SAMPLE_RATE_16, SAMPLE_RATE_12, SAMPLE_RATE_8, SAMPLE_RATE_22_05
};
struct MEDIA_AUDIO_AC3PLUS_INFO_T { int channels; MEDIA_AUDIO_SAMPLERATE sampleRate; };
struct MEDIA_AUDIO_OPUS_INFO_T {
    int channels; MEDIA_AUDIO_SAMPLERATE sampleRate; unsigned char *header; uint32_t headerSize;
};
struct MEDIA_AUDIO_IAMF_INFO_T {
    int channels; MEDIA_AUDIO_SAMPLERATE sampleRate; unsigned char *header; uint32_t headerSize;
};
struct MEDIA_AUDIO_VORBIS_INFO_T {
    int channels; MEDIA_AUDIO_SAMPLERATE sampleRate; unsigned char *header; uint32_t headerSize;
};
namespace mediapipeline {
void setAC3PLUSinfo(MEDIA_AUDIO_AC3PLUS_INFO_T &, const rapidjson::Value &);
void setOPUSinfo(MEDIA_AUDIO_OPUS_INFO_T &, const rapidjson::Value &);
void setIAMFinfo(MEDIA_AUDIO_IAMF_INFO_T &, const rapidjson::Value &);
void setVORBISinfo(MEDIA_AUDIO_VORBIS_INFO_T &, const rapidjson::Value &);
unsigned transSampleRate(MEDIA_AUDIO_SAMPLERATE);
}
#endif
