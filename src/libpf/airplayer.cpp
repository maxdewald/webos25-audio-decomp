/*
 * mediapipeline::AirPlayer (airplayer.cpp). RECONSTRUCTION from libpf-1.0.so.1.0.0.
 * Layout used here:
 *   +0x64  PfLog       per-instance logger (ctx, then name)
 *   +0x8c  GstElement* pipeline (a GstBin with children "sink" and "decoder")
 *   +0x90  info*       +0x18 video port, +0x9c max-width, +0xa0 max-height,
 *                      +0xa4 max-frame-rate (float)
 */
#include "libpf.h"

namespace mediapipeline {

struct AirPlayerInfo;

class AirPlayer {
public:
    bool setResource(const std::string &vdecCoreType, int vdecPort, int adecPort,
                     int mixerPort, int width, int height, float frameRate, bool hfr);
};

#define AP_LOG(self)      FIELD(PfLog, self, 0x64)
#define AP_PIPELINE(self) FIELD(GstElement *, self, 0x8c)
#define AP_INFO(self)     FIELD(char *, self, 0x90)

/* @0xa3ee8: sends "acquired-resource" {core-type, video-port, audio-port = adecPort,
 * mixer-port, max-*, active=TRUE}. The same structure is set as "resource-info" on the bin's "sink"
 * and (if present) "decoder" children, then sent to the pipeline as a custom event.
 * width/height/frameRate/hfr are ignored: the max-* values come from AP_INFO. */
bool AirPlayer::setResource(const std::string &vdecCoreType, int vdecPort, int adecPort,
                            int mixerPort, int width, int height, float frameRate, bool hfr)
{
    if (!AP_PIPELINE(this))
        return false;

    char *info = AP_INFO(this);
    if (FIELD(float, info, 0xa4) > 60.0f) {
        PF_LOG_DEBUG(AP_LOG(this), "airplayer.cpp", "setResource", 0x2e0,
                     "prevent to enable HFR mode");
        info = AP_INFO(this);
        FIELD(float, info, 0xa4) = 60.0f;
    }
    FIELD(int, info, 0x18) = vdecPort;

    GstStructure *res = gst_structure_new(
        "acquired-resource",
        "core-type", G_TYPE_STRING, vdecCoreType.c_str(),
        "video-port", G_TYPE_INT, vdecPort,
        "audio-port", G_TYPE_INT, adecPort,
        "mixer-port", G_TYPE_INT, mixerPort,
        "max-width", G_TYPE_INT, FIELD(int, info, 0x9c),
        "max-height", G_TYPE_INT, FIELD(int, info, 0xa0),
        "max-frame-rate", G_TYPE_FLOAT, (double)FIELD(float, info, 0xa4),
        "active", G_TYPE_BOOLEAN, TRUE,     /* dropped by Ghidra; from raw 0x93f40 */
        NULL);

    GstElement *sink = gst_bin_get_by_name(GST_BIN(AP_PIPELINE(this)), "sink");
    g_object_set(G_OBJECT(sink), "resource-info", res, NULL);
    gst_object_unref(sink);

    GstElement *dec = gst_bin_get_by_name(GST_BIN(AP_PIPELINE(this)), "decoder");
    if (dec) {
        g_object_set(G_OBJECT(dec), "resource-info", res, NULL);
        gst_object_unref(dec);
    }

    gst_element_send_event(AP_PIPELINE(this), gst_event_new_custom(PF_EVENT_ACQUIRED_RESOURCE, res));

    info = AP_INFO(this);
    PF_LOG_DEBUG(AP_LOG(this), "airplayer.cpp", "setResource", 0x2fc,
                 "Resource acquired, vdecCoreType[%s] vdecPort[%d] adecPort[%d] mixerPort[%d] "
                 "max-width[%d] max-height[%d] max-framerate[%f]",
                 vdecCoreType.c_str(), vdecPort, adecPort, mixerPort, FIELD(int, info, 0x9c),
                 FIELD(int, info, 0xa0), (double)FIELD(float, info, 0xa4));

    return gst_element_set_state(AP_PIPELINE(this), GST_STATE_PAUSED) != GST_STATE_CHANGE_FAILURE;
}

} // namespace mediapipeline
