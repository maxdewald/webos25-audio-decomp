/* Resource delivery in libpf; Ghidra addresses = ELF VA + 0x10000.
 * All functions copy adecPort into audio-port unchanged. They do not allocate a
 * renderer index. lgao_set_port selects that index downstream (or mixerPort when
 * audio-port == -1). Full Ghidra export remains under build/decomp/.
 * Pipeline: +0x24 app ID string, +0xd8 GstElement*, +0x534 video port.
 * DvrPipeline: +0x3e8/+0x3ec dimensions, +0x3f0 fps, +0xc80 resource structure,
 *              +0xd5c proxy-error flag.
 * EsSWDecPlayer: +0x64 logger, +0x8c pipeline, +0x90 info, +0xa6 resource ready.
 * UriSWDecPlayer: +0x64 logger, +0x78 pipeline, +0x7c info, +0x94 resource ready.
 */
#include "libpf.h"

enum MEDIA_CUSTOM_SRC_TYPE_T : unsigned int {};

namespace mediapipeline {
class Pipeline {
public:
    static PfLog pflog;
    bool setResource(const std::string &, int, int, int, int, int, float, bool);
};
class DvrPipeline {
public:
    bool setResource(const std::string &, int, int, int, int, int, float, bool);
};
class CustomPipeline {
public:
    bool setResource(const std::string &, int, int, int, int, int, float, bool);
    void setResourcePropertyToSink();
    int addSrcElement(MEDIA_CUSTOM_SRC_TYPE_T);
    int addMiddleElement_ES();
    int addSinkElement_ES();
    bool internalPause();
};
class DirectMediaPlayer {
public:
    bool setResource(const std::string &, int, int, int, int, int, float, bool);
};
class EsSWDecPlayer {
public:
    bool setResource(const std::string &, int, int, int, int, int, float, bool);
    void delayedPauseSinkElements();
};
class UriSWDecPlayer {
public:
    bool setResource(const std::string &, int, int, int, int, int, float, bool);
    void delayedPauseSinkElements();
};

/* @0x75724: playbin receives the event; it later applies resource-info to its sinks. */
bool Pipeline::setResource(const std::string &core, int video, int audio, int mixer,
                           int width, int height, float fps, bool hfr)
{
    PF_LOG_DEBUG(pflog, "pipeline.cpp", "setResource", 0x41f,
        "vdecCoreType[%s] vdecPort[%d] adecPort[%d] mixerPort[%d] width[%d] height[%d] frameRate[%f] hfr[%d]",
        core.c_str(), video, audio, mixer, width, height, double(fps), int(hfr));
    PF_LOG_LEVEL(pflog, 6, "SVU_PF_INFO", "pipeline.cpp", "setResource", 0x421,
        "vdecCoreType[%s] vdecPort[%d] adecPort[%d] mixerPort[%d] width[%d] height[%d] frameRate[%f] hfr[%d]",
        core.c_str(), video, audio, mixer, width, height, double(fps), int(hfr));
    FIELD(int, this, 0x534) = video;
    if (!FIELD(GstElement *, this, 0xd8))
        return false;

    GstStructure *resource;
    if (FIELD(std::string, this, 0x24) == "com.webos.app.mediadiscovery") {
        resource = gst_structure_new("acquired-resource",
            "core-type", G_TYPE_STRING, core.c_str(),
            "video-port", G_TYPE_INT, video,
            "audio-port", G_TYPE_INT, audio,
            "mixer-port", G_TYPE_INT, mixer, NULL);
    } else {
        resource = gst_structure_new("acquired-resource",
            "core-type", G_TYPE_STRING, core.c_str(),
            "video-port", G_TYPE_INT, video,
            "audio-port", G_TYPE_INT, audio,
            "mixer-port", G_TYPE_INT, mixer,
            "max-width", G_TYPE_INT, width,
            "max-height", G_TYPE_INT, height,
            "max-frame-rate", G_TYPE_FLOAT, double(fps),
            "hfr", G_TYPE_BOOLEAN, int(hfr),
            "active", G_TYPE_BOOLEAN, TRUE, NULL);
    }
    gst_element_send_event(FIELD(GstElement *, this, 0xd8),
        gst_event_new_custom(PF_EVENT_ACQUIRED_RESOURCE, resource));
    return true;
}

/* @0x11b394: DVR retains the resource structure and sends a copy downstream. */
bool DvrPipeline::setResource(const std::string &core, int video, int audio, int mixer,
                              int width, int height, float fps, bool hfr)
{
    PF_LOG_DEBUG(Pipeline::pflog, "dvrpipeline.cpp", "setResource", 0x30c,
        "vdecCoreType[%s] vdecPort[%d] adecPort[%d] mixerPort[%d]",
        core.c_str(), video, audio, mixer);
    if (!FIELD(GstElement *, this, 0xd8))
        return false;
    if (FIELD(unsigned char, this, 0xd5c)) {
        PF_LOG_ERROR(Pipeline::pflog, "dvrpipeline.cpp", "setResource", 0x312,
                     "proxyError do not setResource");
        return false;
    }
    FIELD(int, this, 0x534) = video;
    GstStructure *resource;
    if (FIELD(int, this, 0x3e8) && FIELD(int, this, 0x3ec)) {
        resource = gst_structure_new("acquired-resource",
            "core-type", G_TYPE_STRING, core.c_str(),
            "video-port", G_TYPE_INT, video,
            "audio-port", G_TYPE_INT, audio,
            "mixer-port", G_TYPE_INT, mixer,
            "max-width", G_TYPE_INT, FIELD(int, this, 0x3e8),
            "max-height", G_TYPE_INT, FIELD(int, this, 0x3ec),
            "max-frame-rate", G_TYPE_FLOAT, double(FIELD(float, this, 0x3f0)),
            "active", G_TYPE_BOOLEAN, TRUE, NULL);
    } else {
        resource = gst_structure_new("acquired-resource",
            "core-type", G_TYPE_STRING, core.c_str(),
            "video-port", G_TYPE_INT, video,
            "audio-port", G_TYPE_INT, audio,
            "mixer-port", G_TYPE_INT, mixer,
            "active", G_TYPE_BOOLEAN, TRUE, NULL);
    }
    FIELD(GstStructure *, this, 0xc80) = resource;
    if (!resource)
        return false;
    if (FIELD(GstElement *, this, 0xd8))
        gst_element_send_event(FIELD(GstElement *, this, 0xd8),
            gst_event_new_custom(PF_EVENT_ACQUIRED_RESOURCE, gst_structure_copy(resource)));
    return true;
}

/* @0x12e094: software audio decoding still receives the booked ports at its sink. */
bool EsSWDecPlayer::setResource(const std::string &core, int video, int audio, int mixer,
                                int width, int height, float fps, bool hfr)
{
    if (!FIELD(GstElement *, this, 0x8c)) {
        PF_LOG_ERROR(FIELD(PfLog, this, 0x64), "esswdecplayer.cpp", "setResource", 0x469,
                     "pipeline was already destroyed");
        return false;
    }
    PF_LOG_DEBUG(FIELD(PfLog, this, 0x64), "esswdecplayer.cpp", "setResource", 0x46d,
        "setResource: vdecCoreType[%s] vdecPort[%d] adecPort[%d] mixerPort[%d]",
        core.c_str(), video, audio, mixer);
    if (!FIELD(unsigned char, FIELD(char *, this, 0x90), 0x88)) {
        GstElement *sink = gst_bin_get_by_name(GST_BIN(FIELD(GstElement *, this, 0x8c)), "audio-sink");
        GstStructure *resource = gst_structure_new("acquired-resource",
            "core-type", G_TYPE_STRING, core.c_str(),
            "video-port", G_TYPE_INT, video,
            "audio-port", G_TYPE_INT, audio,
            "mixer-port", G_TYPE_INT, mixer, NULL);
        g_object_set(G_OBJECT(sink), "resource-info", resource, NULL);
        gst_structure_free(resource);
        gst_object_unref(sink);
    }
    FIELD(unsigned char, this, 0xa6) = 1;
    delayedPauseSinkElements();
    return true;
}

/* @0x145bc0: no custom event; resource-info is set directly on audio-sink. */
bool UriSWDecPlayer::setResource(const std::string &core, int video, int audio, int mixer,
                                 int width, int height, float fps, bool hfr)
{
    PF_LOG_DEBUG(FIELD(PfLog, this, 0x64), "uriswdecplayer.cpp", "setResource", 0x1fd,
        "setResource: vdecCoreType[%s] vdecPort[%d] adecPort[%d] mixerPort[%d]",
        core.c_str(), video, audio, mixer);
    if (!FIELD(unsigned char, FIELD(char *, this, 0x7c), 0x8c)) {
        GstElement *sink = gst_bin_get_by_name(GST_BIN(FIELD(GstElement *, this, 0x78)), "audio-sink");
        GstStructure *resource = gst_structure_new("acquired-resource",
            "core-type", G_TYPE_STRING, core.c_str(),
            "video-port", G_TYPE_INT, video,
            "audio-port", G_TYPE_INT, audio,
            "mixer-port", G_TYPE_INT, mixer, NULL);
        g_object_set(G_OBJECT(sink), "resource-info", resource, NULL);
        gst_structure_free(resource);
        gst_object_unref(sink);
    }
    FIELD(unsigned char, this, 0x94) = 1;
    delayedPauseSinkElements();
    return true;
}

/* @0xc6724: source type 7 defers pipeline construction until resources arrive. */
bool CustomPipeline::setResource(const std::string &core, int video, int audio, int mixer,
                                 int width, int height, float fps, bool hfr)
{
    PF_LOG_DEBUG(Pipeline::pflog, "custompipeline.cpp", "setResource", 0x2cc,
        "vdecCoreType[%s] vdecPort[%d] adecPort[%d] mixerPort[%d]",
        core.c_str(), video, audio, mixer);
    if (!FIELD(GstElement *, this, 0xd8))
        return false;
    FIELD(int, this, 0x534) = video;
    GstStructure *resource;
    if (FIELD(int, this, 0x3e8) && FIELD(int, this, 0x3ec)) {
        resource = gst_structure_new("acquired-resource",
            "core-type", G_TYPE_STRING, core.c_str(),
            "video-port", G_TYPE_INT, video,
            "audio-port", G_TYPE_INT, audio,
            "mixer-port", G_TYPE_INT, mixer,
            "max-width", G_TYPE_INT, FIELD(int, this, 0x3e8),
            "max-height", G_TYPE_INT, FIELD(int, this, 0x3ec),
            "max-frame-rate", G_TYPE_FLOAT, double(FIELD(float, this, 0x3f0)),
            "active", G_TYPE_BOOLEAN, TRUE, NULL);
    } else {
        resource = gst_structure_new("acquired-resource",
            "core-type", G_TYPE_STRING, core.c_str(),
            "video-port", G_TYPE_INT, video,
            "audio-port", G_TYPE_INT, audio,
            "mixer-port", G_TYPE_INT, mixer,
            "active", G_TYPE_BOOLEAN, TRUE, NULL);
    }
    FIELD(GstStructure *, this, 0x868) = resource;
    if (!resource)
        return false;
    if (FIELD(int, this, 0x888) != 7) {
        if (FIELD(GstElement *, this, 0xd8))
            gst_element_send_event(FIELD(GstElement *, this, 0xd8),
                gst_event_new_custom(PF_EVENT_ACQUIRED_RESOURCE, gst_structure_copy(resource)));
        return true;
    }
    if (FIELD(int, this, 0x538) == 1)
        return true;
    PF_LOG_DEBUG(Pipeline::pflog, "custompipeline.cpp", "setResource", 0x2fd,
                 "Start to construct pipeline after receiving resources");
    if (addSrcElement(static_cast<MEDIA_CUSTOM_SRC_TYPE_T>(FIELD(int, this, 0x888))) != 0 ||
        addMiddleElement_ES() != 0)
        return false;
    gst_element_set_state(FIELD(GstElement *, this, 0xd8), GST_STATE_READY);
    if (!internalPause()) {
        PF_LOG_ERROR(Pipeline::pflog, "custompipeline.cpp", "setResource", 0x304,
                     "Custom::Load -> fail to pause");
        return false;
    }
    if (addSinkElement_ES() != 0) {
        PF_LOG_ERROR(Pipeline::pflog, "custompipeline.cpp", "setResource", 0x30a,
                     "Failed to create decoder/sink elements");
        return false;
    }
    PF_LOG_DEBUG(Pipeline::pflog, "custompipeline.cpp", "setResource", 0x30e,
                 "Pipeline construction completed!");
    return true;
}

/* @0xb37cc: retained audio-port/mixer-port reach the audio and video sinks here. */
void CustomPipeline::setResourcePropertyToSink()
{
    PF_LOG_DEBUG(Pipeline::pflog, "custompipeline.cpp", "setResourcePropertyToSink", 0x314, "%s", "");
    GstElement *audio = FIELD(GstElement *, this, 0x678);
    if (audio) {
        g_object_set(G_OBJECT(audio), "resource-info", FIELD(GstStructure *, this, 0x868), NULL);
        gst_element_set_locked_state(FIELD(GstElement *, this, 0x678), FALSE);
        gst_element_sync_state_with_parent(FIELD(GstElement *, this, 0x678));
    }
    GstElement *video = FIELD(GstElement *, this, 0x660);
    if (video) {
        g_object_set(G_OBJECT(video), "resource-info", FIELD(GstStructure *, this, 0x868), NULL);
        gst_element_set_locked_state(FIELD(GstElement *, this, 0x660), FALSE);
        gst_element_sync_state_with_parent(FIELD(GstElement *, this, 0x660));
    }
}

/* @0x14ab8c: entry stub branches back to the body at Ghidra 0x14a94c. */
bool DirectMediaPlayer::setResource(const std::string &core, int video, int audio, int mixer,
                                    int width, int height, float fps, bool hfr)
{
    if (!FIELD(GstElement *, this, 0x8c))
        return false;
    FIELD(int, FIELD(char *, this, 0x90), 0) = video;
    GstStructure *resource = gst_structure_new("acquired-resource",
        "core-type", G_TYPE_STRING, core.c_str(),
        "video-port", G_TYPE_INT, video,
        "audio-port", G_TYPE_INT, audio,
        "mixer-port", G_TYPE_INT, mixer,
        "max-width", G_TYPE_INT, width,
        "max-height", G_TYPE_INT, height,
        "max-frame-rate", G_TYPE_FLOAT, double(fps),
        "hfr", G_TYPE_BOOLEAN, int(hfr),
        "active", G_TYPE_BOOLEAN, TRUE, NULL);
    if (FIELD(GstElement *, this, 0xac)) {
        g_object_set(G_OBJECT(FIELD(GstElement *, this, 0xac)), "resource-info", resource, NULL);
        gst_element_set_locked_state(FIELD(GstElement *, this, 0xac), FALSE);
        gst_element_sync_state_with_parent(FIELD(GstElement *, this, 0xac));
    }
    if (FIELD(GstElement *, this, 0xa4))
        g_object_set(G_OBJECT(FIELD(GstElement *, this, 0xa4)), "resource-info", resource, NULL);
    if (FIELD(GstElement *, this, 0xb0)) {
        g_object_set(G_OBJECT(FIELD(GstElement *, this, 0xb0)), "resource-info", resource, NULL);
        gst_element_set_locked_state(FIELD(GstElement *, this, 0xb0), FALSE);
        gst_element_sync_state_with_parent(FIELD(GstElement *, this, 0xb0));
    }
    if (FIELD(GstElement *, this, 0xa8))
        g_object_set(G_OBJECT(FIELD(GstElement *, this, 0xa8)), "resource-info", resource, NULL);
    PF_LOG_DEBUG(FIELD(PfLog, this, 0x64), "directmediaplayer.cpp", "setResource", 0x405,
        "resource acquired, vdecCoreType[%s] vdecPort[%d] adecPort[%d] mixerPort[%d] max-width[%d] max-height[%d] max-framerate[%f] hfr[%d]",
        core.c_str(), video, audio, mixer, width, height, double(fps), int(hfr));
    return gst_element_set_state(FIELD(GstElement *, this, 0x8c), GST_STATE_PAUSED) != GST_STATE_CHANGE_FAILURE;
}
}
