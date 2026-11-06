/*
 * mediapipeline::DirectMediaPlayer (directmediaplayer.cpp). RECONSTRUCTION from
 * libpf-1.0.so.1.0.0. setResource lives in resource_delivery.cpp.
 * Layout used here:
 *   +0x64  PfLog        logger (ctx, then name)
 *   +0x8c  GstElement*  pipeline bin
 *   +0x90  info*        +0x04 bool needs decoder, +0x08 std::string codec ("PCM"/"OPUS"/"AAC"),
 *                       +0x60 std::string PCM format, +0x90 std::string PCM layout,
 *                       +0xac PCM rate index (1..8), +0xb0 PCM channels,
 *                       +0xb4 std::string AAC stream-format, +0xd0 AAC channels,
 *                       +0xd4 float AAC rate (kHz), +0xf4 OPUS channels,
 *                       +0xf8 float OPUS rate (kHz), +0xfc/+0x100 OPUS codec data/size
 *   +0xa0  GstElement*  "audio-src" appsrc
 *   +0xa8  GstElement*  audio decoder (only when info+0x04)
 *   +0xb0  GstElement*  audio sink (lgaudiosink)
 *   +0xb4  std::string  application id
 */
#include "libpf.h"

#include <gst/audio/audio-channels.h>

/* free function in libpf (@0xa2464); first argument is a GstElementFactoryListType mask */
extern GstElementFactory *findElementFactory(guint64 type, GstCaps *caps);

namespace mediapipeline {

class DirectMediaPlayer {
public:
    bool createAudioPath();
};

#define DM_LOG(self)      FIELD(PfLog, self, 0x64)
#define DM_PIPELINE(self) FIELD(GstElement *, self, 0x8c)
#define DM_INFO(self)     FIELD(char *, self, 0x90)
#define DM_APPSRC(self)   FIELD(GstElement *, self, 0xa0)
#define DM_DECODER(self)  FIELD(GstElement *, self, 0xa8)
#define DM_SINK(self)     FIELD(GstElement *, self, 0xb0)
#define DM_APPID(self)    FIELD(std::string, self, 0xb4)

static const int dm_pcm_rates[8] = { 48000, 44100, 32000, 24000, 16000, 12000, 8000, 22050 };

/* @0x14bf68: the only stock producer of lgaudiosink pcm-bypass / pcm-multi-ch for
 * DirectMedia. Both are set BEFORE the pipeline leaves NULL (ao does not exist yet, so
 * the sink defers them into preopen_conditions) and are never toggled afterwards.
 * Multichannel (>5ch) audio: "com.google.stadia*" -> pcm-bypass, everyone else ->
 * pcm-multi-ch. The source caps are plain PCM (no es/codec-str/clock-x4): LG never
 * pairs either flag with an ES/MAT description. game-mode=TRUE sets the bypass fader. */
bool DirectMediaPlayer::createAudioPath()
{
    PF_LOG_DEBUG(DM_LOG(this), "directmediaplayer.cpp", "createAudioPath", 0x1e6,
                 "createAudioPath");

    std::string name("audio-src");
    DM_APPSRC(this) = gst_element_factory_make("appsrc", name.c_str());

    char *info = DM_INFO(this);
    std::string &codec = FIELD(std::string, info, 0x08);
    GstCaps *srcCaps;

    if (codec.compare("PCM") == 0) {
        unsigned idx = FIELD(int, info, 0xac) - 1;
        int rate = idx < 8 ? dm_pcm_rates[idx] : 44100;
        int channels = FIELD(int, info, 0xb0);

        srcCaps = gst_caps_new_simple("audio/x-raw",
            "format", G_TYPE_STRING, FIELD(std::string, info, 0x60).c_str(),
            "layout", G_TYPE_STRING, FIELD(std::string, info, 0x90).c_str(),
            "channels", G_TYPE_INT, channels,
            "rate", G_TYPE_INT, rate,
            "channel-mask", GST_TYPE_BITMASK, gst_audio_channel_get_fallback_mask(channels),
            NULL);
    } else if (codec.compare("OPUS") == 0) {
        srcCaps = gst_caps_new_simple("audio/x-opus",
            "channels", G_TYPE_INT, FIELD(int, info, 0xf4),
            "rate", G_TYPE_INT, (int)(FIELD(float, info, 0xf8) * 1000.0f), NULL);
        size_t size = FIELD(size_t, info, 0x100);
        if (size) {
            const void *data = FIELD(const void *, info, 0xfc);
            GstBuffer *header = gst_buffer_new_allocate(NULL, size, NULL);
            GstMapInfo map;
            gst_buffer_map(header, &map, GST_MAP_READ);
            memcpy(map.data, data, size);
            gst_buffer_unmap(header, &map);

            srcCaps = gst_caps_make_writable(srcCaps);
            GstStructure *s = gst_caps_get_structure(srcCaps, 0);
            GValue array = G_VALUE_INIT, value = G_VALUE_INIT;
            g_value_init(&array, GST_TYPE_ARRAY);
            g_value_init(&value, GST_TYPE_BUFFER);
            GST_BUFFER_FLAG_SET(header, GST_BUFFER_FLAG_HEADER);
            g_value_set_boxed(&value, header);
            gst_buffer_unref(header);
            gst_value_array_append_value(&array, &value);
            g_value_unset(&value);
            gst_structure_set_value(s, "streamheader", &array);
            g_value_unset(&array);
        }
    } else if (codec.compare("AAC") == 0) {
        srcCaps = gst_caps_new_simple("audio/mpeg",
            "mpegversion", G_TYPE_INT, 4,
            "stream-format", G_TYPE_STRING, FIELD(std::string, info, 0xb4).c_str(),
            "rate", G_TYPE_INT, (int)(FIELD(float, info, 0xd4) * 1000.0f),
            "channels", G_TYPE_INT, FIELD(int, info, 0xd0), NULL);
    } else {
        srcCaps = NULL;
    }

    gchar *str = gst_caps_to_string(srcCaps);
    PF_LOG_DEBUG(DM_LOG(this), "directmediaplayer.cpp", "createAudioPath", 0x1ec,
                 "check src caps: %s", str);
    g_free(str);

    GstCaps *sinkCaps = gst_caps_new_empty_simple("audio/x-raw");
    if (FIELD(bool, DM_INFO(this), 0x04)) {
        PF_LOG_DEBUG(DM_LOG(this), "directmediaplayer.cpp", "createAudioPath", 0x1fa,
                     "create and add audio decoder");
        GstElementFactory *factory = findElementFactory(GST_ELEMENT_FACTORY_TYPE_DECODER, srcCaps);
        DM_DECODER(this) = gst_element_factory_create(factory, NULL);
        gst_object_unref(factory);
    }
    if (DM_DECODER(this)) {
        GstPad *pad = gst_element_get_static_pad(DM_DECODER(this), "src");
        if (pad) {
            if (sinkCaps)
                gst_caps_unref(sinkCaps);
            sinkCaps = gst_pad_query_caps(pad, NULL);
            gst_object_unref(pad);
        }
    }

    str = gst_caps_to_string(sinkCaps);
    PF_LOG_DEBUG(DM_LOG(this), "directmediaplayer.cpp", "createAudioPath", 0x20f,
                 "audio sink caps: %s", str);
    g_free(str);

    /* SINK | MEDIA_AUDIO | 0xc000000000000 (LG-private factory list bits) */
    GstElementFactory *factory = findElementFactory(0xe000000000004ULL, sinkCaps);
    DM_SINK(this) = gst_element_factory_create(factory, NULL);
    gst_object_unref(factory);
    gst_caps_unref(sinkCaps);
    if (!DM_SINK(this)) {
        PF_LOG_ERROR(DM_LOG(this), "directmediaplayer.cpp", "createAudioPath", 0x21a,
                     "cannot create audiosink");
        return false;
    }

    bool hasDecoder = FIELD(bool, DM_INFO(this), 0x04);
    if (!hasDecoder)
        gst_bin_add_many(GST_BIN(DM_PIPELINE(this)), DM_APPSRC(this), DM_SINK(this), NULL);
    else
        gst_bin_add_many(GST_BIN(DM_PIPELINE(this)), DM_APPSRC(this), DM_DECODER(this),
                         DM_SINK(this), NULL);
    if (!FIELD(bool, DM_INFO(this), 0x04)) {
        if (!gst_element_link_many(DM_APPSRC(this), DM_SINK(this), NULL))
            PF_LOG_ERROR(DM_LOG(this), "directmediaplayer.cpp", "createAudioPath", 0x24c,
                         "error in linking audio elements");
    } else if (!gst_element_link_many(DM_APPSRC(this), DM_DECODER(this), DM_SINK(this), NULL)) {
        PF_LOG_ERROR(DM_LOG(this), "directmediaplayer.cpp", "createAudioPath", 0x249,
                     "error in linking audio elements");
    }

    g_object_set(G_OBJECT(DM_APPSRC(this)), "caps", srcCaps, "format", GST_FORMAT_TIME,
                 "emit-signals", FALSE, NULL);
    /* set even when no decoder was created (NULL instance, GLib critical) */
    g_object_set(G_OBJECT(DM_DECODER(this)), "app-type", "DIRECTMEDIA", NULL);
    g_object_set(G_OBJECT(DM_SINK(this)), "app-type", "DIRECTMEDIA", "sync", FALSE,
                 "nodelay", TRUE, "disable-gstsync", TRUE, "game-mode", TRUE, NULL);

    if (FIELD(int, DM_INFO(this), 0xb0) > 5) {
        if (DM_APPID(this).compare(0, 17, "com.google.stadia") == 0) {
            PF_LOG_DEBUG(DM_LOG(this), "directmediaplayer.cpp", "createAudioPath", 0x267,
                         "set pcm-bypass to asink");
            g_object_set(G_OBJECT(DM_SINK(this)), "pcm-bypass", TRUE, NULL);
        } else {
            PF_LOG_DEBUG(DM_LOG(this), "directmediaplayer.cpp", "createAudioPath", 0x26b,
                         "set pcm-multi-ch to asink");
            g_object_set(G_OBJECT(DM_SINK(this)), "pcm-multi-ch", TRUE, NULL);
        }
    }
    gst_caps_unref(srcCaps);
    return true;
}

} // namespace mediapipeline
