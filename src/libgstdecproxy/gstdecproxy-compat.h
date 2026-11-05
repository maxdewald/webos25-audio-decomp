/* SPDX-License-Identifier: LGPL-2.0-or-later
 *
 * Declarations the shipped plugin gets from LG's GStreamer 1.24 fork that are not in
 * the stock Ubuntu GStreamer 1.20 headers used to build this reconstruction.
 * Prototypes follow lg-opensource/gstreamer/subprojects/gstreamer/gst/gstsyslog.h and
 * gst/gstelement.h (LGPL-2.0-or-later).
 * RECONSTRUCTION support for orig/usr/lib/gstreamer-1.0/libgstdecproxy.so
 * (sha256 df504a8ac782e36ab8edf665cb7441ddeb5c75e98cb5db2b8375e385e1db34e5).
 */
#ifndef DECPROXY_COMPAT_H
#define DECPROXY_COMPAT_H

#include <gst/gst.h>

/* The binary registers itself as a GStreamer 1.24 plugin
 * (gst_plugin_register_static (1, 0x18, ...) @0x1a940). */
#undef GST_VERSION_MINOR
#define GST_VERSION_MINOR 24

G_BEGIN_DECLS

/* LG extension: gst/gstsyslog.h */
#ifndef __GSTSYSDEBUG_H_
#define __GSTSYSDEBUG_H_
#define GST_SYS_CAT_LEVEL_LOG(cat,level,object,...) G_STMT_START{   \
  gst_sys_debug_log ((cat), (level), __BASE_FILE__, GST_FUNCTION, __LINE__,  \
      (GObject *) (object), __VA_ARGS__);       \
} G_STMT_END

#define GST_SYS_DEBUG_OBJECT(obj,...) GST_SYS_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_DEBUG, obj, ##__VA_ARGS__)
#define GST_SYS_INFO_OBJECT(obj,...) GST_SYS_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_INFO, obj, ##__VA_ARGS__)
#define GST_SYS_WARNING_OBJECT(obj,...) GST_SYS_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_WARNING, obj, ##__VA_ARGS__)
#define GST_SYS_ERROR_OBJECT(obj,...) GST_SYS_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_ERROR, obj, ##__VA_ARGS__)

void gst_sys_debug_log (GstDebugCategory * category, GstDebugLevel level,
    const gchar * file, const gchar * function, gint line,
    GObject * object, const gchar * format, ...) G_GNUC_PRINTF (7, 8);
#endif

/* LG extension: gst/gstelement.h smart properties */
typedef enum
{
  GST_SMART_PROPERTIES_OK = 1,
  GST_SMART_PROPERTIES_QUERY_FAILED = -1,
  GST_SMART_PROPERTIES_NOT_LINKED = -2,
  GST_SMART_PROPERTIES_ERROR = -3
} GstSmartPropertiesReturn;

GstSmartPropertiesReturn gst_element_get_smart_properties (GstElement * element,
    const gchar * first_property_name, ...);

G_END_DECLS

#endif
