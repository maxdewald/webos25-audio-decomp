/* SPDX-License-Identifier: LGPL-2.0-or-later
 *
 * Modified to match the shipped webOS 25 binary (sha256 df504a8ac782e36ab8edf665cb7441ddeb5c75e98cb5db2b8375e385e1db34e5).
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstdecproxy.so, DWARF CU
 * "../gst-plugins-cool/gst/decproxy/plugin.c". Base: LG's published gst-plugins-cool 1.24
 * (lg-opensource/gstreamer/subprojects/gst-plugins-cool/gst/decproxy/plugin.c), LGPL header kept below.
 *
 * webOS25: no behavioural difference from the published file. Every function compiles
 * (GCC 11.4, flags from DW_AT_producer) to the same instruction stream as the binary;
 * the only diffs are literal-pool/alignment padding. DWARF DW_AT_decl_line of every
 * function equals the published line, so the "#line 1" below keeps __LINE__ (used by the
 * GST_*_OBJECT logs) identical. Changes vs published text: address tags + joined
 * "return-type name (" definition lines (line count preserved), and the
 * HAVE_CONFIG_H guard replaced by an unconditional #include "config.h".
 */
#line 1
/* config.h is always included in this reconstruction */
#include "config.h"


#include <gst/gst.h>
#include "gstdecproxy2.h"
#include "gstmediainfo.h"
#include "gstdecbridge.h"

/* @0x1a8dc */
static gboolean plugin_init (GstPlugin * plugin)
{
  if (!gst_element_register (plugin, "decproxy", GST_RANK_NONE,
          GST_TYPE_DECPROXY))
    return FALSE;

  if (!gst_element_register (plugin, "mediainfo", GST_RANK_NONE,
          GST_TYPE_MEDIA_INFO))
    return FALSE;

  if (!gst_element_register (plugin, "decbridge", GST_RANK_NONE,
          GST_TYPE_DEC_BRIDGE))
    return FALSE;

  return TRUE;
}

/* @0x1a934 = gst_plugin_decproxy_get_desc */
/* @0x1a940 = gst_plugin_decproxy_register */
GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    decproxy,
    "Decoder Proxy",
    plugin_init, VERSION, "LGPL", GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN);
