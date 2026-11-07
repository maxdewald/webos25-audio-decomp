/*
 * opus_audiodec element (gst-plugins-bsp/gstlgaudio/gstlgaudiodec_opus.c): Opus.
 * Same boilerplate as gstlgaudiodec_ac3.c; the pads forward to the shared body in
 * gstlgaudiodec.c, and the getinfo functions below are the mime-map entries that
 * _gst_lg_adec_set_caps calls to fill LGADEC_CODEC_INFO_T.
 *
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so
 *   sha256 84db367e1ea3eeb4b589bf63910140cfd43ea77f2db124ae6882fbee8ebb6bb7
 * DWARF CU gstlgaudiodec_opus.c. Tags are Ghidra addresses.
 */
#include <stdlib.h>
#include <string.h>
#include "gstlgaudiodec.h"

#define GST_CAT_DEFAULT gst_lg_audiodec_debug

typedef GstLgAudioDec GstLgAudioDecOpus;
typedef GstLgAudioDecClass GstLgAudioDecOpusClass;

#define SRC_CAPS "audio/x-raw, format = (string) { S8, U8, S16LE, S16BE, U16LE, U16BE, S24LE, " \
    "S24BE, U24LE, U24BE, S32LE, S32BE, U32LE, U32BE }, layout = (string) interleaved, " \
    "rate = (int) [ 1, 384000 ], channels = (int) [ 1, 8 ]"

static GstStaticPadTemplate opus_sinkpadtemplate = GST_STATIC_PAD_TEMPLATE("sink", GST_PAD_SINK,
    GST_PAD_ALWAYS, GST_STATIC_CAPS("audio/x-opus; audio/x-eopus; audio/x-private1-opus; "));
static GstStaticPadTemplate srcpadtemplate = GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC,
    GST_PAD_ALWAYS, GST_STATIC_CAPS(SRC_CAPS));

static GstStateChangeReturn gst_lg_audiodec_opus_change_state(GstElement *element,
                                                                      GstStateChange transition);
static void gst_lg_audiodec_opus_get_property(GObject *object, guint prop_id, GValue *value,
                                                      GParamSpec *pspec);
static void gst_lg_audiodec_opus_set_property(GObject *object, guint prop_id,
                                                      const GValue *value, GParamSpec *pspec);
static gboolean gst_lg_audiodec_opus_src_event(GstPad *pad, GstObject *parent, GstEvent *event);
static gboolean gst_lg_audiodec_opus_sink_event(GstPad *pad, GstObject *parent, GstEvent *event);
static GstFlowReturn gst_lg_audiodec_opus_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer);
static void gst_lg_audiodec_opus_class_intern_init(gpointer klass);
static void gst_lg_audiodec_opus_init(GstLgAudioDecOpus *pAudioDec);

/* G_DEFINE_TYPE (GstLgAudioDecOpus, gst_lg_audiodec_opus, GST_TYPE_ELEMENT), expanded so that
 * each generated function carries its address. A GstElement subclass, not GstAudioDecoder. */
static gpointer parent_class = NULL;
static gint GstLgAudioDecOpus_private_offset;

/* @0x25834 */
static G_GNUC_NO_INLINE GType gst_lg_audiodec_opus_get_type_once(void)
{
    return g_type_register_static_simple(GST_TYPE_ELEMENT, g_intern_static_string("GstLgAudioDecOpus"),
                                         sizeof(GstLgAudioDecOpusClass),
                                         (GClassInitFunc)gst_lg_audiodec_opus_class_intern_init,
                                         sizeof(GstLgAudioDecOpus),
                                         (GInstanceInitFunc)gst_lg_audiodec_opus_init, 0);
}

/* @0x25a44 */
GType gst_lg_audiodec_opus_get_type(void)
{
    static gsize static_g_define_type_id = 0;

    if (g_once_init_enter(&static_g_define_type_id)) {
        GType g_define_type_id = gst_lg_audiodec_opus_get_type_once();
        g_once_init_leave(&static_g_define_type_id, g_define_type_id);
    }
    return static_g_define_type_id;
}

/* @0x25a84 */
static gboolean gst_lg_audiodec_opus_src_event(GstPad *pad, GstObject *parent, GstEvent *event)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(parent,
                                    gst_lg_audiodec_opus_get_type(), GstLgAudioDecOpus);
    gboolean ret;

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return FALSE;
    }
    gst_lg_adec_upstream_event(pAudioDec, event);
    ret = gst_pad_event_default(pad, parent, event);
    GST_INFO("ret = %s", ret ? "TRUE" : "FALSE");
    return ret;
}

/* @0x25b58: FALSE from the shared handler = event consumed/held (e.g. pending SEGMENT) */
static gboolean gst_lg_audiodec_opus_sink_event(GstPad *pad, GstObject *parent, GstEvent *event)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(parent,
                                    gst_lg_audiodec_opus_get_type(), GstLgAudioDecOpus);
    GstEventType type;
    gboolean ret;

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return FALSE;
    }
    ret = gst_lg_adec_downstream_event(pAudioDec, event);
    type = GST_EVENT_TYPE(event);
    if (ret == TRUE)
        ret = gst_pad_event_default(pad, parent, event);
    else
        gst_event_unref(event);
    if (type == GST_EVENT_SEGMENT)
        ret = TRUE;
    GST_INFO("ret = %s", ret ? "TRUE" : "FALSE");
    return ret;
}

/* @0x25c74 */
static GstFlowReturn gst_lg_audiodec_opus_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer)
{
    return gst_lg_adec_chain((GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(parent,
                                 gst_lg_audiodec_opus_get_type(), GstLgAudioDecOpus), buffer);
}

/* @0x25c90 */
static GstStateChangeReturn gst_lg_audiodec_opus_change_state(GstElement *element,
                                                                      GstStateChange transition)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(element,
                                    gst_lg_audiodec_opus_get_type(), GstLgAudioDecOpus);
    GstStateChangeReturn ret;

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return GST_STATE_CHANGE_FAILURE;
    }
    GST_INFO("DEC.%d - change state from %s to %s (pending=%s)", pAudioDec->index,
             gst_element_state_get_name(GST_STATE_TRANSITION_CURRENT(transition)),
             gst_element_state_get_name(GST_STATE_TRANSITION_NEXT(transition)),
             gst_element_state_get_name(GST_STATE_PENDING(element)));
    gst_lg_adec_change_state_up(pAudioDec, transition);
    ret = GST_ELEMENT_CLASS(parent_class)->change_state(element, transition);
    gst_lg_adec_change_state_down(pAudioDec, transition);
    GST_INFO("DEC.%d - change state from %s to %s. DONE", pAudioDec->index,
             gst_element_state_get_name(GST_STATE_TRANSITION_CURRENT(transition)),
             gst_element_state_get_name(GST_STATE_TRANSITION_NEXT(transition)));
    return ret;
}

/* @0x25dfc */
static void gst_lg_audiodec_opus_get_property(GObject *object, guint prop_id, GValue *value,
                                                      GParamSpec *pspec)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(object,
                                    gst_lg_audiodec_opus_get_type(), GstLgAudioDecOpus);

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    if (!gst_lg_adec_get_property(pAudioDec, prop_id, value, pspec))
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
}

/* @0x25ecc */
static void gst_lg_audiodec_opus_set_property(GObject *object, guint prop_id,
                                                      const GValue *value, GParamSpec *pspec)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(object,
                                    gst_lg_audiodec_opus_get_type(), GstLgAudioDecOpus);

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    if (!gst_lg_adec_set_property(pAudioDec, prop_id, value, pspec))
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
}

/* inlined into class_intern_init */
static inline void gst_lg_audiodec_opus_class_init(GstLgAudioDecOpusClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;
    GstElementClass *element_class = (GstElementClass *)klass;

    gobject_class->set_property = gst_lg_audiodec_opus_set_property;
    gobject_class->get_property = gst_lg_audiodec_opus_get_property;
    gst_lg_adec_class_init(gobject_class);
    gst_element_class_set_static_metadata(element_class, "OPUS audio decoder", "Codec/Decoder/Audio",
                                          "LG's OPUS audio decoder element",
                                          "Minho Hwang <minho.hwang@lge.com>");
    gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&opus_sinkpadtemplate));
    gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&srcpadtemplate));
    element_class->change_state = GST_DEBUG_FUNCPTR(gst_lg_audiodec_opus_change_state);
}

/* @0x25874 */
static void gst_lg_audiodec_opus_class_intern_init(gpointer klass)
{
    parent_class = g_type_class_peek_parent(klass);
    if (GstLgAudioDecOpus_private_offset != 0)
        g_type_class_adjust_private_offset(klass, &GstLgAudioDecOpus_private_offset);
    gst_lg_audiodec_opus_class_init((GstLgAudioDecOpusClass *)klass);
}

/* @0x2591c */
static void gst_lg_audiodec_opus_init(GstLgAudioDecOpus *pAudioDec)
{
    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    pAudioDec->pSinkPad = gst_pad_new_from_static_template(&opus_sinkpadtemplate, "sink");
    gst_pad_set_chain_function(pAudioDec->pSinkPad, GST_DEBUG_FUNCPTR(gst_lg_audiodec_opus_chain));
    gst_pad_set_event_function(pAudioDec->pSinkPad, GST_DEBUG_FUNCPTR(gst_lg_audiodec_opus_sink_event));
    gst_element_add_pad(GST_ELEMENT(pAudioDec), pAudioDec->pSinkPad);
    pAudioDec->pSrcPad = gst_pad_new_from_static_template(&srcpadtemplate, "src");
    gst_pad_set_event_function(pAudioDec->pSrcPad, GST_DEBUG_FUNCPTR(gst_lg_audiodec_opus_src_event));
    gst_element_add_pad(GST_ELEMENT(pAudioDec), pAudioDec->pSrcPad);
    gst_lg_adec_init(pAudioDec);
}

/* @0x25f9c: same as vorbis (calloc instead of malloc), plus a hex dump at LOG level */
GstFlowReturn gst_lg_audiodec_opus_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *codec_info)
{
    const GValue *streamheader;

    GST_INFO("Get OPUS info");
    streamheader = gst_structure_get_value(s, "streamheader");
    if (streamheader && GST_VALUE_HOLDS_ARRAY(streamheader)) {
        guint n = gst_value_array_get_size(streamheader);
        guint i;
        guint size = 0;

        for (i = 0; i < n; i++)
            size += gst_buffer_get_size(gst_value_get_buffer(gst_value_array_get_value(streamheader, i)));
        GST_INFO("opus header size = %u", size);

        if (codec_info->buffer)
            free(codec_info->buffer);
        codec_info->buffer = calloc(1, size);
        if (codec_info->buffer) {
            guint offset = 0;

            codec_info->buf_size = size;
            for (i = 0; i < n; i++) {
                GstBuffer *header = gst_value_get_buffer(gst_value_array_get_value(streamheader, i));
                GstMapInfo bufMapInfo;

                if (gst_buffer_map(header, &bufMapInfo, GST_MAP_READ) == TRUE) {
                    guint buffer_size = bufMapInfo.size;
                    guint8 *destination = codec_info->buffer + offset;

                    offset += buffer_size;
                    memcpy(destination, bufMapInfo.data, buffer_size);
                    gst_buffer_unmap(header, &bufMapInfo);
                } else {
                    GST_ERROR("gst_buffer_map() Err");
                }
            }
            if (codec_info->buffer) {
                GST_DEBUG("opus header =");
                for (i = 0; i < (guint)codec_info->buf_size; i++)
                    GST_DEBUG("%02X ", codec_info->buffer[i]);
            }
        }
    } else {
        GST_ERROR("OPUS: streamheader is not exist");
    }
    codec_info->codec_type = LGADEC_CODEC_OPUS;
    return GST_FLOW_OK;
}
