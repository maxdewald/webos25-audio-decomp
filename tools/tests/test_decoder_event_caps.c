#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef int gboolean;
typedef int gint;
typedef unsigned guint;
typedef char gchar;
typedef enum { LGADEC_CODEC_AAC, LGADEC_CODEC_AC4, LGADEC_CODEC_OTHER } LGADEC_CODEC_T;
enum { FALSE, TRUE, GST_MAP_READ = 1 };
typedef struct { const unsigned char *data; size_t size; int mapped; } GstBuffer;
typedef struct { GstBuffer *buffer; } GValue;
typedef struct { const unsigned char *data; size_t size; } GstMapInfo;
typedef struct {
    const char *mime, *format;
    int rate, channels;
    GValue *codec_data;
} GstStructure;
typedef struct { GstStructure structure; } GstCaps;
typedef struct { int rate, codec_error; LGADEC_CODEC_T codec; } Decoder;
typedef struct { Decoder *adec; const char *mime; GstCaps *pAudioCaps; } GstLgAudioDec;
#define EVENT_MEDIA(...) ((void)0)
#define EVENT_INFO(...) ((void)0)
#define EVENT_ERROR(...) ((void)0)
static const GstStructure *gst_caps_get_structure(const GstCaps *caps, unsigned index)
{
    assert(index == 0);
    return caps ? &caps->structure : NULL;
}
static const char *gst_structure_get_name(const GstStructure *structure) { return structure->mime; }
static int gst_structure_get_int(const GstStructure *structure, const char *field, int *value)
{
    *value = strcmp(field, "rate") == 0 ? structure->rate : structure->channels;
    return *value > 0;
}
static const char *gst_structure_get_string(const GstStructure *structure, const char *field)
{
    assert(strcmp(field, "stream-format") == 0);
    return structure->format;
}
static const GValue *gst_structure_get_value(const GstStructure *structure, const char *field)
{
    assert(strcmp(field, "codec_data") == 0);
    return structure->codec_data;
}
static int lgadec_get_freq(Decoder *decoder, int *rate) { *rate = decoder->rate; return 0; }
static int lgadec_get_codec_type(Decoder *decoder, LGADEC_CODEC_T *codec)
{
    *codec = decoder->codec;
    return decoder->codec_error;
}
static GstBuffer *gst_value_get_buffer(const GValue *value) { return value->buffer; }
static int gst_buffer_map(GstBuffer *buffer, GstMapInfo *map, int flags)
{
    assert(flags == GST_MAP_READ && buffer && !buffer->mapped);
    *map = (GstMapInfo){buffer->data, buffer->size};
    buffer->mapped = 1;
    return TRUE;
}
static void gst_buffer_unmap(GstBuffer *buffer, GstMapInfo *map)
{
    if (buffer) {
        assert(buffer->mapped && map->data == buffer->data && map->size == buffer->size);
        buffer->mapped = 0;
    }
}

// @extract src/libgstlgaudiodec/gstlgaudiodec.c _gst_lg_adec_caps_is_equal

int main(void)
{
    Decoder decoder = {48000, 0, LGADEC_CODEC_OTHER};
    GstCaps previous = {{"audio/mpeg", "raw", 48000, 2, NULL}};
    GstCaps incoming = previous;
    GstLgAudioDec element = {&decoder, "audio/mpeg", &previous};
    assert(_gst_lg_adec_caps_is_equal(&element, &incoming));
    assert(!_gst_lg_adec_caps_is_equal(&element, NULL));
    incoming.structure.mime = "audio/x-ac3";
    assert(!_gst_lg_adec_caps_is_equal(&element, &incoming));
    incoming = previous;
    incoming.structure.rate = 44100;
    assert(!_gst_lg_adec_caps_is_equal(&element, &incoming));
    incoming = previous;
    incoming.structure.format = NULL;
    assert(!_gst_lg_adec_caps_is_equal(&element, &incoming));
    incoming.structure.format = "adts";
    assert(!_gst_lg_adec_caps_is_equal(&element, &incoming));
    incoming = previous;
    incoming.structure.channels = 6;
    assert(!_gst_lg_adec_caps_is_equal(&element, &incoming));
    decoder.codec = LGADEC_CODEC_AC4;
    assert(_gst_lg_adec_caps_is_equal(&element, &incoming));
    decoder.codec = LGADEC_CODEC_AAC;
    incoming = previous;
    assert(!_gst_lg_adec_caps_is_equal(&element, &incoming));
    const unsigned char first[] = {0x12, 0x10}, changed[] = {0x12, 0x11};
    GstBuffer previous_buffer = {first, sizeof(first), 0};
    GstBuffer incoming_buffer = {first, sizeof(first), 0};
    GValue previous_data = {&previous_buffer}, incoming_data = {&incoming_buffer};
    previous.structure.codec_data = &previous_data;
    incoming.structure.codec_data = &incoming_data;
    assert(_gst_lg_adec_caps_is_equal(&element, &incoming));
    assert(!previous_buffer.mapped && !incoming_buffer.mapped);
    incoming_buffer.data = changed;
    assert(!_gst_lg_adec_caps_is_equal(&element, &incoming));
    assert(!previous_buffer.mapped && !incoming_buffer.mapped);
    incoming_buffer.size = 1;
    assert(!_gst_lg_adec_caps_is_equal(&element, &incoming));
    assert(!previous_buffer.mapped && !incoming_buffer.mapped);
    incoming.structure.codec_data = NULL;
    assert(!_gst_lg_adec_caps_is_equal(&element, &incoming));
    previous.structure.codec_data = NULL;
    incoming.structure.codec_data = &incoming_data;
    assert(!_gst_lg_adec_caps_is_equal(&element, &incoming));
    decoder.codec_error = -1;
    assert(_gst_lg_adec_caps_is_equal(&element, &incoming));
    puts("decoder event caps decisions and mapped-buffer cleanup passed");
    return 0;
}