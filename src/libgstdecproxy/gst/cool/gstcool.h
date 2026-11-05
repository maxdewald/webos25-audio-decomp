/* SPDX-License-Identifier: LGPL-2.0-or-later
 * Copied unchanged (except include paths) from LG gst-plugins-cool 1.24 for the
 * libgstdecproxy reconstruction; matches the shipped webOS 25 binary
 * (sha256 df504a8ac782e36ab8edf665cb7441ddeb5c75e98cb5db2b8375e385e1db34e5) per its DWARF layouts. */
/* GStreamer Plugins Cool
 * Copyright (C) 2014 LG Electronics, Inc.
 *	Author : Jeongseok Kim <jeongseok.kim@lge.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GST_COOL_H__
#define __GST_COOL_H__

#include <gst/gst.h>
#include "gstcoolutil.h"
#include "gstcoolplaybin.h"
#include "cool-prelude.h"

G_BEGIN_DECLS

GST_COOL_API void gst_cool_init (int *argc, char **argv[]);

GST_COOL_API
    gboolean gst_cool_init_check (int *argc, char **argv[], GError ** err);
GST_COOL_API GKeyFile *gst_cool_get_configuration (void);

GST_COOL_API void gst_cool_set_rank (const gchar * plugin, gint rank);

GST_COOL_API void gst_cool_give_priority_sw_decoder (gboolean use_sw_dec);

GST_COOL_API
    void gst_cool_set_decode_buffer_size (guint in_size, guint out_size);

GST_COOL_API gchar *gst_cool_get_external_video_decoder (void);

GST_COOL_API gchar *gst_cool_get_external_video_sink (void);

G_END_DECLS
#endif
