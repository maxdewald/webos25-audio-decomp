/*
 * Plugin entry of the LG audio decoder elements
 * (gst-plugins-bsp/gstlgaudio/gstlgaudiodecplugin.c).
 *
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so
 *   sha256 84db367e1ea3eeb4b589bf63910140cfd43ea77f2db124ae6882fbee8ebb6bb7
 * DWARF CU gstlgaudiodecplugin.c (gstlgaudiodec_plugin_init is in the same CU, before
 * the GST_PLUGIN_DEFINE functions). gst_plugin_desc @0x3e6f0 (Ghidra 0x4e6f0).
 *
 * Registered elements: aac_ ac3_ ac4_ adpcm_ amr_ flac_ mpegh_(rank PRIMARY)
 * pcm_ vorbis_ wma_ opus_ iamf_audiodec (rank SECONDARY). There is no DTS element.
 */
#include "gstlgaudiodec.h"

#define PACKAGE "gst-plugins-bsp"
#ifndef GST_PACKAGE_RELEASE_DATETIME
#define GST_PACKAGE_RELEASE_DATETIME NULL
#endif

GType gst_lg_audiodec_aac_get_type(void);
GType gst_lg_audiodec_ac4_get_type(void);
GType gst_lg_audiodec_adpcm_get_type(void);
GType gst_lg_audiodec_amr_get_type(void);
GType gst_lg_audiodec_flac_get_type(void);
GType gst_lg_audiodec_pcm_get_type(void);
GType gst_lg_audiodec_vorbis_get_type(void);
GType gst_lg_audiodec_wma_get_type(void);
GType gst_lg_audiodec_opus_get_type(void);
GType gst_lg_audiodec_iamf_get_type(void);

/* @0x27dc4 */
static gboolean gstlgaudiodec_plugin_init(GstPlugin *plugin)
{
    GST_DEBUG_CATEGORY_INIT(gst_lg_audiodec_debug, "lgaudiodec", 6, "lgaudiodec element");

    if (!gst_element_register(plugin, "aac_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_aac_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "ac3_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_ac3_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "ac4_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_ac4_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "adpcm_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_adpcm_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "amr_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_amr_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "flac_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_flac_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "mpegh_audiodec", GST_RANK_PRIMARY,
                              gst_lg_audiodec_mpegh_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "pcm_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_pcm_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "vorbis_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_vorbis_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "wma_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_wma_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "opus_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_opus_get_type()))
        return FALSE;
    if (!gst_element_register(plugin, "iamf_audiodec", GST_RANK_SECONDARY, gst_lg_audiodec_iamf_get_type()))
        return FALSE;
    return TRUE;
}

/* GST_PLUGIN_DEFINE(GST_VERSION_MAJOR, 24, lgaudiodec, "LG's audio decoder element",
 *                   gstlgaudiodec_plugin_init, "1.0", "Proprietary", "LG Media Framework",
 *                   "www.lge.com") expanded so both generated functions carry their tags. */
const GstPluginDesc *gst_plugin_lgaudiodec_get_desc(void);
void gst_plugin_lgaudiodec_register(void);

static const GstPluginDesc gst_plugin_desc = {
    GST_VERSION_MAJOR, 24, "lgaudiodec", (gchar *)"LG's audio decoder element",
    gstlgaudiodec_plugin_init, "1.0", "Proprietary", PACKAGE, "LG Media Framework",
    "www.lge.com", GST_PACKAGE_RELEASE_DATETIME, GST_PADDING_INIT
};

/* @0x27f3c */
const GstPluginDesc *gst_plugin_lgaudiodec_get_desc(void)
{
    return &gst_plugin_desc;
}

/* @0x27f48 */
void gst_plugin_lgaudiodec_register(void)
{
    gst_plugin_register_static(GST_VERSION_MAJOR, 24, "lgaudiodec", "LG's audio decoder element",
                               gstlgaudiodec_plugin_init, "1.0", "Proprietary", PACKAGE,
                               "LG Media Framework", "www.lge.com");
}
