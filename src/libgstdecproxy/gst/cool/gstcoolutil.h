/* SPDX-License-Identifier: LGPL-2.0-or-later
 * Copied unchanged (except include paths) from LG gst-plugins-cool 1.24 for the
 * libgstdecproxy reconstruction; matches the shipped webOS 25 binary
 * (sha256 df504a8ac782e36ab8edf665cb7441ddeb5c75e98cb5db2b8375e385e1db34e5) per its DWARF layouts. */
/* GStreamer Plugins Cool
 * Copyright (C) 2014-2020 LG Electronics, Inc.
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

#ifndef __GST_COOLUTIL_H__
#define __GST_COOLUTIL_H__

#include <gst/gst.h>
#include "cool-prelude.h"

G_BEGIN_DECLS

typedef enum
{
  GST_COOL_STREAM_TYPE_UNKNOWN = -1,
  GST_COOL_STREAM_TYPE_AUDIO = 0,
  GST_COOL_STREAM_TYPE_VIDEO,
  GST_COOL_STREAM_TYPE_TEXT
} GstCoolStreamType;

GST_COOL_API
    GstStructure * gst_cool_caps_to_info (GstCaps * caps,
    char *stream_id, const char *mime_type);
GST_COOL_API
    GstStructure * gst_cool_taglist_to_info (GstTagList * taglist,
    char *stream_id, const char *mime_type);
GST_COOL_API GstCoolStreamType gst_cool_find_type (const gchar * mime_type);

GST_COOL_API
    GstCaps * negotiate_default_caps (GstCaps * input_caps, GstPad * srcpad,
    gint stream_type);

GST_COOL_API gboolean make_unsupported_codec (const GstCaps * input_caps);

GST_COOL_API
    GstStructure * get_smart_property_from_source (GstElement * parent);

G_END_DECLS
#endif
