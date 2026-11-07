/*
 * adpcm_audiodec element (gst-plugins-bsp/gstlgaudio/gstlgaudiodec_adpcm.c): ADPCM (decoded on the host, no DSP decoder).
 * Same boilerplate as gstlgaudiodec_ac3.c; the pads forward to the shared body in
 * gstlgaudiodec.c, and the getinfo functions below are the mime-map entries that
 * _gst_lg_adec_set_caps calls to fill LGADEC_CODEC_INFO_T.
 *
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so
 *   sha256 84db367e1ea3eeb4b589bf63910140cfd43ea77f2db124ae6882fbee8ebb6bb7
 * DWARF CU gstlgaudiodec_adpcm.c. Tags are Ghidra addresses.
 */
#include <string.h>
#include "gstlgaudiodec.h"

#define GST_CAT_DEFAULT gst_lg_audiodec_debug

typedef GstLgAudioDec GstLgAudioDecAdpcm;
typedef GstLgAudioDecClass GstLgAudioDecAdpcmClass;

#define SRC_CAPS "audio/x-raw, format = (string) { S8, U8, S16LE, S16BE, U16LE, U16BE, S24LE, " \
    "S24BE, U24LE, U24BE, S32LE, S32BE, U32LE, U32BE }, layout = (string) interleaved, " \
    "rate = (int) [ 1, 384000 ], channels = (int) [ 1, 8 ]"

static GstStaticPadTemplate adpcm_sinkpadtemplate = GST_STATIC_PAD_TEMPLATE("sink", GST_PAD_SINK,
    GST_PAD_ALWAYS, GST_STATIC_CAPS("audio/x-adpcm; audio/x-eadpcm; audio/x-private1-adpcm; "));
static GstStaticPadTemplate srcpadtemplate = GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC,
    GST_PAD_ALWAYS, GST_STATIC_CAPS(SRC_CAPS));

static GstStateChangeReturn gst_lg_audiodec_adpcm_change_state(GstElement *element,
                                                                      GstStateChange transition);
static void gst_lg_audiodec_adpcm_get_property(GObject *object, guint prop_id, GValue *value,
                                                      GParamSpec *pspec);
static void gst_lg_audiodec_adpcm_set_property(GObject *object, guint prop_id,
                                                      const GValue *value, GParamSpec *pspec);
static gboolean gst_lg_audiodec_adpcm_src_event(GstPad *pad, GstObject *parent, GstEvent *event);
static gboolean gst_lg_audiodec_adpcm_sink_event(GstPad *pad, GstObject *parent, GstEvent *event);
static GstFlowReturn gst_lg_audiodec_adpcm_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer);
static void gst_lg_audiodec_adpcm_class_intern_init(gpointer klass);
static void gst_lg_audiodec_adpcm_init(GstLgAudioDecAdpcm *pAudioDec);

/* G_DEFINE_TYPE (GstLgAudioDecAdpcm, gst_lg_audiodec_adpcm, GST_TYPE_ELEMENT), expanded so that
 * each generated function carries its address. A GstElement subclass, not GstAudioDecoder. */
static gpointer parent_class = NULL;
static gint GstLgAudioDecAdpcm_private_offset;

/* @0x237b8 */
static G_GNUC_NO_INLINE GType gst_lg_audiodec_adpcm_get_type_once(void)
{
    return g_type_register_static_simple(GST_TYPE_ELEMENT, g_intern_static_string("GstLgAudioDecAdpcm"),
                                         sizeof(GstLgAudioDecAdpcmClass),
                                         (GClassInitFunc)gst_lg_audiodec_adpcm_class_intern_init,
                                         sizeof(GstLgAudioDecAdpcm),
                                         (GInstanceInitFunc)gst_lg_audiodec_adpcm_init, 0);
}

/* @0x239c8 */
GType gst_lg_audiodec_adpcm_get_type(void)
{
    static gsize static_g_define_type_id = 0;

    if (g_once_init_enter(&static_g_define_type_id)) {
        GType g_define_type_id = gst_lg_audiodec_adpcm_get_type_once();
        g_once_init_leave(&static_g_define_type_id, g_define_type_id);
    }
    return static_g_define_type_id;
}

/* @0x23a08 */
static gboolean gst_lg_audiodec_adpcm_src_event(GstPad *pad, GstObject *parent, GstEvent *event)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(parent,
                                    gst_lg_audiodec_adpcm_get_type(), GstLgAudioDecAdpcm);
    gboolean ret;

    if (!pAudioDec) {
#line 265 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
        GST_ERROR("pAudioDec is NULL");
        return FALSE;
    }
    gst_lg_adec_upstream_event(pAudioDec, event);
    ret = gst_pad_event_default(pad, parent, event);
#line 273 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
    GST_INFO("ret = %s", ret ? "TRUE" : "FALSE");
    return ret;
}

/* @0x23ae0: FALSE from the shared handler = event consumed/held (e.g. pending SEGMENT) */
static gboolean gst_lg_audiodec_adpcm_sink_event(GstPad *pad, GstObject *parent, GstEvent *event)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(parent,
                                    gst_lg_audiodec_adpcm_get_type(), GstLgAudioDecAdpcm);
    GstEventType type;
    gboolean ret;

    if (!pAudioDec) {
#line 235 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
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
#line 250 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
    GST_INFO("ret = %s", ret ? "TRUE" : "FALSE");
    return ret;
}

/* @0x23bfc */
static GstFlowReturn gst_lg_audiodec_adpcm_chain(GstPad *pad, GstObject *parent, GstBuffer *buffer)
{
    return gst_lg_adec_chain((GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(parent,
                                 gst_lg_audiodec_adpcm_get_type(), GstLgAudioDecAdpcm), buffer);
}

/* @0x23c18 */
static GstStateChangeReturn gst_lg_audiodec_adpcm_change_state(GstElement *element,
                                                                      GstStateChange transition)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(element,
                                    gst_lg_audiodec_adpcm_get_type(), GstLgAudioDecAdpcm);
    GstStateChangeReturn ret;

    if (!pAudioDec) {
#line 321 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
        GST_ERROR("pAudioDec is NULL");
        return GST_STATE_CHANGE_FAILURE;
    }
#line 325 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
    GST_INFO("DEC.%d - change state from %s to %s (pending=%s)", pAudioDec->index,
             gst_element_state_get_name(GST_STATE_TRANSITION_CURRENT(transition)),
             gst_element_state_get_name(GST_STATE_TRANSITION_NEXT(transition)),
             gst_element_state_get_name(GST_STATE_PENDING(element)));
    gst_lg_adec_change_state_up(pAudioDec, transition);
    ret = GST_ELEMENT_CLASS(parent_class)->change_state(element, transition);
    gst_lg_adec_change_state_down(pAudioDec, transition);
#line 338 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
    GST_INFO("DEC.%d - change state from %s to %s. DONE", pAudioDec->index,
             gst_element_state_get_name(GST_STATE_TRANSITION_CURRENT(transition)),
             gst_element_state_get_name(GST_STATE_TRANSITION_NEXT(transition)));
    return ret;
}

/* @0x23d84 */
static void gst_lg_audiodec_adpcm_get_property(GObject *object, guint prop_id, GValue *value,
                                                      GParamSpec *pspec)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(object,
                                    gst_lg_audiodec_adpcm_get_type(), GstLgAudioDecAdpcm);

    if (!pAudioDec) {
#line 304 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    if (!gst_lg_adec_get_property(pAudioDec, prop_id, value, pspec))
#line 310 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
}

/* @0x23e54 */
static void gst_lg_audiodec_adpcm_set_property(GObject *object, guint prop_id,
                                                      const GValue *value, GParamSpec *pspec)
{
    GstLgAudioDec *pAudioDec = (GstLgAudioDec *)G_TYPE_CHECK_INSTANCE_CAST(object,
                                    gst_lg_audiodec_adpcm_get_type(), GstLgAudioDecAdpcm);

    if (!pAudioDec) {
#line 286 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    if (!gst_lg_adec_set_property(pAudioDec, prop_id, value, pspec))
#line 293 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
}

/* inlined into class_intern_init */
static inline void gst_lg_audiodec_adpcm_class_init(GstLgAudioDecAdpcmClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;
    GstElementClass *element_class = (GstElementClass *)klass;

    gobject_class->set_property = gst_lg_audiodec_adpcm_set_property;
    gobject_class->get_property = gst_lg_audiodec_adpcm_get_property;
    gst_lg_adec_class_init(gobject_class);
    gst_element_class_set_static_metadata(element_class, "ADPCM audio decoder", "Codec/Decoder/Audio",
                                          "LG's ADPCM audio decoder element",
                                          "Minho Hwang <minho.hwang@lge.com>");
    gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&adpcm_sinkpadtemplate));
    gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&srcpadtemplate));
    element_class->change_state = GST_DEBUG_FUNCPTR(gst_lg_audiodec_adpcm_change_state);
}

/* @0x237f8 */
static void gst_lg_audiodec_adpcm_class_intern_init(gpointer klass)
{
    parent_class = g_type_class_peek_parent(klass);
    if (GstLgAudioDecAdpcm_private_offset != 0)
        g_type_class_adjust_private_offset(klass, &GstLgAudioDecAdpcm_private_offset);
    gst_lg_audiodec_adpcm_class_init((GstLgAudioDecAdpcmClass *)klass);
}

/* @0x238a0 */
static void gst_lg_audiodec_adpcm_init(GstLgAudioDecAdpcm *pAudioDec)
{
    if (!pAudioDec) {
#line 191 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    pAudioDec->pSinkPad = gst_pad_new_from_static_template(&adpcm_sinkpadtemplate, "sink");
    gst_pad_set_chain_function(pAudioDec->pSinkPad, GST_DEBUG_FUNCPTR(gst_lg_audiodec_adpcm_chain));
    gst_pad_set_event_function(pAudioDec->pSinkPad, GST_DEBUG_FUNCPTR(gst_lg_audiodec_adpcm_sink_event));
    gst_element_add_pad(GST_ELEMENT(pAudioDec), pAudioDec->pSinkPad);
    pAudioDec->pSrcPad = gst_pad_new_from_static_template(&srcpadtemplate, "src");
    gst_pad_set_event_function(pAudioDec->pSrcPad, GST_DEBUG_FUNCPTR(gst_lg_audiodec_adpcm_src_event));
    gst_element_add_pad(GST_ELEMENT(pAudioDec), pAudioDec->pSrcPad);
    gst_lg_adec_init(pAudioDec);
}

/* caps "layout" -> LGADEC_ADPCM_T (names as in gst-libav's adpcm layouts) */
typedef struct {
    gchar *pAdpcmLayout;
    LGADEC_ADPCM_T adpcmLayout;
} ADPCM_LAYOUT;

static const ADPCM_LAYOUT _gAdpcmLyaoutMap[30] = {
    { "quicktime", LGADEC_ADPCM_IMA_QT },   { "dvi", LGADEC_ADPCM_IMA_WAV },
    { "dk3", LGADEC_ADPCM_IMA_DK3 },        { "dk4", LGADEC_ADPCM_IMA_DK4 },
    { "westwood", LGADEC_ADPCM_IMA_WS },    { "smjpeg", LGADEC_ADPCM_IMA_SMJPEG },
    { "microsoft", LGADEC_ADPCM_MS },       { "4xm", LGADEC_ADPCM_4XM },
    { "xa", LGADEC_ADPCM_XA },              { "adx", LGADEC_ADPCM_ADX },
    { "ea", LGADEC_ADPCM_EA },              { "g726", LGADEC_ADPCM_G726 },
    { "ct", LGADEC_ADPCM_CT },              { "swf", LGADEC_ADPCM_SWF },
    { "yamaha", LGADEC_ADPCM_YAMAHA },      { "sbpro4", LGADEC_ADPCM_SBPRO_4 },
    { "sbpro3", LGADEC_ADPCM_SBPRO_3 },     { "sbpro2", LGADEC_ADPCM_SBPRO_2 },
    { "thp", LGADEC_ADPCM_THP },            { "amv", LGADEC_ADPCM_IMA_AMV },
    { "ea-r1", LGADEC_ADPCM_EA_R1 },        { "ea-r3", LGADEC_ADPCM_EA_R3 },
    { "ea-r2", LGADEC_ADPCM_EA_R2 },        { "ea-sead", LGADEC_ADPCM_IMA_EA_SEAD },
    { "ea-eacs", LGADEC_ADPCM_IMA_EA_EACS },{ "ea-xas", LGADEC_ADPCM_EA_XAS },
    { "ea-maxis-xa", LGADEC_ADPCM_EA_MAXIS_XA }, { "iss", LGADEC_ADPCM_IMA_ISS },
    { "g722", LGADEC_ADPCM_G722 },          { NULL, LGADEC_ADPCM_NONE },
};

/* @0x23f24: note the original matches with strstr(table_name, layout) */
GstFlowReturn gst_lg_audiodec_adpcm_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *codec_info)
{
    const gchar *layout;
    int i;

#line 354 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
    GST_INFO("Get ADPCM info");
    codec_info->codec_type = LGADEC_CODEC_ADPCM;
    layout = gst_structure_get_string(s, "layout");
    if (layout) {
#line 360 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
        GST_INFO("layout of ADPCM = %s", layout);
        for (i = 0; _gAdpcmLyaoutMap[i].pAdpcmLayout; i++) {
            if (strstr(_gAdpcmLyaoutMap[i].pAdpcmLayout, layout)) {
                codec_info->codec_format = _gAdpcmLyaoutMap[i].adpcmLayout;
                break;
            }
        }
    }
#line 370 "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiodec_adpcm.c"
    GST_INFO("ADPCM Layout is 0x%X", codec_info->codec_format);
    return GST_FLOW_OK;
}
