/* SPDX-License-Identifier: LGPL-2.0-or-later
 *
 * config.h for the libgstdecproxy reconstruction (gst-plugins-cool 1.24, webOS 25).
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstdecproxy.so
 * (sha256 df504a8ac782e36ab8edf665cb7441ddeb5c75e98cb5db2b8375e385e1db34e5).
 * Values are the strings gst_plugin_decproxy_register (@0x1a940) passes to
 * gst_plugin_register_static (0x1cd58..0x1cdb4), and the sink template caps string
 * at 0x1c55c (contains audio/x-dts... -> DTS_SUPPORT; contains x-wmv/x-wma).
 */
#ifndef DECPROXY_CONFIG_H
#define DECPROXY_CONFIG_H

#define PACKAGE "gst-plugins-cool"
#define VERSION "1.24.0"
#define GST_PACKAGE_NAME "GStreamer Cool Plug-ins source release"
#define GST_PACKAGE_ORIGIN "Unknown package origin"
#define GST_LICENSE "LGPL"

/* meson: cdata.set('DTS_SUPPORT', get_option('dca')) -> enabled on webOS 25 */
#define DTS_SUPPORT 1
/* DISABLE_WMA / DISABLE_WMV / DISABLE_VP9 / DISABLE_FLAC are not set. */

#include "gstdecproxy-compat.h"

#endif
