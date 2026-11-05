/*
 * aac_audiodec element (gst-plugins-bsp/gstlgaudio/gstlgaudiodec_aac.c): MPEG-1/2 audio (MP3) and AAC (ADTS/LOAS/raw).
 * Same boilerplate as gstlgaudiodec_ac3.c; the pads forward to the shared body in
 * gstlgaudiodec.c, and the getinfo functions below are the mime-map entries that
 * _gst_lg_adec_set_caps calls to fill LGADEC_CODEC_INFO_T.
 *
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so
 *   sha256 84db367e1ea3eeb4b589bf63910140cfd43ea77f2db124ae6882fbee8ebb6bb7
 * DWARF CU gstlgaudiodec_aac.c. Tags are Ghidra addresses.
 */
#include <string.h>
#include <gst/base/gstbitreader.h>
#include "gstlgaudiodec.h"

#define GST_CAT_DEFAULT gst_lg_audiodec_debug

typedef GstLgAudioDec GstLgAudioDecAac;
typedef GstLgAudioDecClass GstLgAudioDecAacClass;

#define SRC_CAPS "audio/x-raw, format = (string) { S8, U8, S16LE, S16BE, U16LE, U16BE, S24LE, " \
    "S24BE, U24LE, U24BE, S32LE, S32BE, U32LE, U32BE }, layout = (string) interleaved, " \
    "rate = (int) [ 1, 384000 ], channels = (int) [ 1, 8 ]"

static GstStaticPadTemplate aac_sinkpadtemplate = GST_STATIC_PAD_TEMPLATE("sink", GST_PAD_SINK,
    GST_PAD_ALWAYS, GST_STATIC_CAPS("audio/x-aac; audio/x-eaac; audio/x-private1-aac; "));
static GstStaticPadTemplate srcpadtemplate = GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC,
    GST_PAD_ALWAYS, GST_STATIC_CAPS(SRC_CAPS));

static GstStateChangeReturn gst_lg_audiodec_aac_change_state(GstElement *element,
                                                                      GstStateChange transition);
static void gst_lg_audiodec_aac_get_property(GObject *object, guint prop_id, GValue *value,
                                                      GParamSpec *pspec);
static void gst_lg_audiodec_aac_set_property(GObject *object, guint prop_id,
                                                      const GValue *value, GParamSpec *pspec);
static gboolean gst_lg_audiodec_aac_src_event(GstPad *pad, GstObject *parent, GstEvent *event);
static gboolean gst_lg_audiodec_aac_sink_event(GstPad *pad, GstObject *parent, GstEvent *event);
static GstFlowReturn gst_lg_audiodec_aac_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer);
static void gst_lg_audiodec_aac_class_intern_init(gpointer klass);
static void gst_lg_audiodec_aac_init(GstLgAudioDecAac *pAudioDec);

/* G_DEFINE_TYPE (GstLgAudioDecAac, gst_lg_audiodec_aac, GST_TYPE_ELEMENT), expanded so that
 * each generated function carries its address. A GstElement subclass, not GstAudioDecoder. */
static gpointer parent_class = NULL;
static gint GstLgAudioDecAac_private_offset;

/* @0x21828 */
static G_GNUC_NO_INLINE GType gst_lg_audiodec_aac_get_type_once(void)
{
    return g_type_register_static_simple(GST_TYPE_ELEMENT, g_intern_static_string("GstLgAudioDecAac"),
                                         sizeof(GstLgAudioDecAacClass),
                                         (GClassInitFunc)gst_lg_audiodec_aac_class_intern_init,
                                         sizeof(GstLgAudioDecAac),
                                         (GInstanceInitFunc)gst_lg_audiodec_aac_init, 0);
}

/* @0x21a38 */
GType gst_lg_audiodec_aac_get_type(void)
{
    static gsize static_g_define_type_id = 0;

    if (g_once_init_enter(&static_g_define_type_id)) {
        GType g_define_type_id = gst_lg_audiodec_aac_get_type_once();
        g_once_init_leave(&static_g_define_type_id, g_define_type_id);
    }
    return static_g_define_type_id;
}

/* @0x21a78 */
static gboolean gst_lg_audiodec_aac_src_event(GstPad *pad, GstObject *parent, GstEvent *event)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(parent,
                                    gst_lg_audiodec_aac_get_type(), GstLgAudioDecAac);
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

/* @0x21b50: FALSE from the shared handler = event consumed/held (e.g. pending SEGMENT) */
static gboolean gst_lg_audiodec_aac_sink_event(GstPad *pad, GstObject *parent, GstEvent *event)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(parent,
                                    gst_lg_audiodec_aac_get_type(), GstLgAudioDecAac);
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

/* @0x21c70 */
static GstFlowReturn gst_lg_audiodec_aac_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer)
{
    return gst_lg_adec_chain((GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(parent,
                                 gst_lg_audiodec_aac_get_type(), GstLgAudioDecAac), buffer);
}

/* @0x21c8c */
static GstStateChangeReturn gst_lg_audiodec_aac_change_state(GstElement *element,
                                                                      GstStateChange transition)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(element,
                                    gst_lg_audiodec_aac_get_type(), GstLgAudioDecAac);
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

/* @0x21df8 */
static void gst_lg_audiodec_aac_get_property(GObject *object, guint prop_id, GValue *value,
                                                      GParamSpec *pspec)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(object,
                                    gst_lg_audiodec_aac_get_type(), GstLgAudioDecAac);

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    if (!gst_lg_adec_get_property(pAudioDec, prop_id, value, pspec))
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
}

/* @0x21ec8 */
static void gst_lg_audiodec_aac_set_property(GObject *object, guint prop_id,
                                                      const GValue *value, GParamSpec *pspec)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(object,
                                    gst_lg_audiodec_aac_get_type(), GstLgAudioDecAac);

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    if (!gst_lg_adec_set_property(pAudioDec, prop_id, value, pspec))
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
}

/* inlined into class_intern_init */
static inline void gst_lg_audiodec_aac_class_init(GstLgAudioDecAacClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;
    GstElementClass *element_class = (GstElementClass *)klass;

    gobject_class->set_property = gst_lg_audiodec_aac_set_property;
    gobject_class->get_property = gst_lg_audiodec_aac_get_property;
    gst_lg_adec_class_init(gobject_class);
    gst_element_class_set_static_metadata(element_class, "AAC audio decoder", "Codec/Decoder/Audio",
                                          "LG's AAC audio decoder element",
                                          "Jihoon Lee <gaius.lee@lge.com>");
    gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&aac_sinkpadtemplate));
    gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&srcpadtemplate));
    element_class->change_state = GST_DEBUG_FUNCPTR(gst_lg_audiodec_aac_change_state);
}

/* @0x21868 */
static void gst_lg_audiodec_aac_class_intern_init(gpointer klass)
{
    parent_class = g_type_class_peek_parent(klass);
    if (GstLgAudioDecAac_private_offset != 0)
        g_type_class_adjust_private_offset(klass, &GstLgAudioDecAac_private_offset);
    gst_lg_audiodec_aac_class_init((GstLgAudioDecAacClass *)klass);
}

/* @0x21910 */
static void gst_lg_audiodec_aac_init(GstLgAudioDecAac *pAudioDec)
{
    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    pAudioDec->pSinkPad = gst_pad_new_from_static_template(&aac_sinkpadtemplate, "sink");
    gst_pad_set_chain_function(pAudioDec->pSinkPad, GST_DEBUG_FUNCPTR(gst_lg_audiodec_aac_chain));
    gst_pad_set_event_function(pAudioDec->pSinkPad, GST_DEBUG_FUNCPTR(gst_lg_audiodec_aac_sink_event));
    gst_element_add_pad(GST_ELEMENT(pAudioDec), pAudioDec->pSinkPad);
    pAudioDec->pSrcPad = gst_pad_new_from_static_template(&srcpadtemplate, "src");
    gst_pad_set_event_function(pAudioDec->pSrcPad, GST_DEBUG_FUNCPTR(gst_lg_audiodec_aac_src_event));
    gst_element_add_pad(GST_ELEMENT(pAudioDec), pAudioDec->pSrcPad);
    gst_lg_adec_init(pAudioDec);
}

/* frame format from caps "stream-format"; returns FALSE if unknown */
static inline gboolean aac_stream_format(const gchar *format, LGADEC_CODEC_INFO_T *codec_info,
                                         gboolean raw_disables_dsp)
{
    if (!strcmp(format, "raw")) {
        codec_info->codec_format = LGADEC_RAW_FORMAT;
        if (raw_disables_dsp)
            codec_info->disable_dsp = 1;
        GST_INFO("AAC frame format(from DEMUX) is RAW");
    } else if (!strcmp(format, "adts")) {
        codec_info->codec_format = LGADEC_AAC_ADTS_FORMAT;
        GST_INFO("AAC frame format(from DEMUX) is ADTS");
    } else if (!strcmp(format, "loas")) {
        codec_info->codec_format = LGADEC_AAC_LOAS_FORMAT;
        GST_INFO("AAC frame format(from DEMUX) is LOAS");
    } else {
        return FALSE;
    }
    return TRUE;
}

/* @0x21f98: audio/mpeg. mpegversion 1 -> MP3; 4 -> AAC; 2 -> AAC if stream-format is
 * raw/adts/loas, else MP3. For AAC, the AudioSpecificConfig object type in codec_data
 * (5 bits, 31 = escape + 6 bits) detects AAC-ELD (39) and AAC-LD (23, log only). */
GstFlowReturn gst_lg_audiodec_aac_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *codec_info)
{
    gint mpegversion;
    const gchar *format;
    GstBuffer *codec_data;
    GstMapInfo map;
    GstBitReader *br;
    guint32 object_type;

    GST_INFO("This content is MPEG type");
    codec_info->codec_format = LGADEC_UNKNOWN_FORMAT;
    codec_info->disable_dsp = 0;
    if (!gst_structure_get_int(s, "mpegversion", &mpegversion)) {
        GST_ERROR("Unknown audio codec");
        return GST_FLOW_ERROR;
    }
    if (mpegversion == 1) {
        GST_INFO("This content is MPEG/MP3 audio codec");
        codec_info->codec_type = LGADEC_CODEC_MP3;
        return GST_FLOW_OK;
    } else if (mpegversion == 4) {
        GST_INFO("This content is AAC audio codec");
        codec_info->codec_type = LGADEC_CODEC_AAC;
        format = gst_structure_get_string(s, "stream-format");
        if (format)
            aac_stream_format(format, codec_info, TRUE);
    } else if (mpegversion == 2) {
        format = gst_structure_get_string(s, "stream-format");
        if (!format) {
            GST_INFO("This content is MPEG/MP3 audio codec");
            codec_info->codec_type = LGADEC_CODEC_MP3;
            return GST_FLOW_OK;
        }
        codec_info->codec_type = LGADEC_CODEC_AAC;
        if (!aac_stream_format(format, codec_info, FALSE)) {
            GST_INFO("This content is MPEG/MP3 audio codec");
            codec_info->codec_type = LGADEC_CODEC_MP3;
            return GST_FLOW_OK;
        }
    } else {
        GST_ERROR("Unknown audio codec");
        return GST_FLOW_ERROR;
    }

    if (codec_info->codec_type != LGADEC_CODEC_AAC)
        return GST_FLOW_OK;
    if (!gst_structure_has_field(s, "codec_data")) {
        GST_INFO("do not have codec_data");
        return GST_FLOW_OK;
    }
    codec_data = gst_value_get_buffer(gst_structure_get_value(s, "codec_data"));
    if (!gst_buffer_map(codec_data, &map, GST_MAP_READ))
        return GST_FLOW_OK;
    GST_INFO("codec data size: %lu", (gulong)map.size);
    br = gst_bit_reader_new(map.data, map.size);
    if (gst_bit_reader_get_bits_uint32(br, &object_type, 5)) {
        if (object_type == 31) {
            if (!gst_bit_reader_get_bits_uint32(br, &object_type, 6))
                goto out;
            object_type += 32;
        }
        if (object_type == 39) {
            GST_INFO("detect AAC-ELD stream");
            codec_info->codec_type = LGADEC_CODEC_AAC_ELD;
        } else if (object_type == 23) {
            GST_INFO("detect AAC_LD stream");
        }
    }
out:
    gst_bit_reader_free(br);
    gst_buffer_unmap(codec_data, &map);
    return GST_FLOW_OK;
}
