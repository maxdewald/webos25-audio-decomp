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


#ifndef __GST_COOL_PLAYBIN_H__
#define __GST_COOL_PLAYBIN_H__

#include <gst/gst.h>
#include "gstcoolutil.h"
#include "cool-prelude.h"

G_BEGIN_DECLS

/* Copied from gst-plugins-base/gst/playback/gstplay-enum.h */
typedef enum
{
  GST_PLAY_FLAG_VIDEO = (1 << 0),
  GST_PLAY_FLAG_AUDIO = (1 << 1),
  GST_PLAY_FLAG_TEXT = (1 << 2),
  GST_PLAY_FLAG_VIS = (1 << 3),
  GST_PLAY_FLAG_SOFT_VOLUME = (1 << 4),
  GST_PLAY_FLAG_NATIVE_AUDIO = (1 << 5),
  GST_PLAY_FLAG_NATIVE_VIDEO = (1 << 6),
  GST_PLAY_FLAG_DOWNLOAD = (1 << 7),
  GST_PLAY_FLAG_BUFFERING = (1 << 8),
  GST_PLAY_FLAG_DEINTERLACE = (1 << 9),
  GST_PLAY_FLAG_SOFT_COLORBALANCE = (1 << 10),
  GST_PLAY_FLAG_FORCE_FILTERS = (1 << 11),
  GST_PLAY_FLAG_FORCE_SW_DECODERS = (1 << 12),
  GST_PLAY_FLAG_NATIVE_TEXT = (1 << 13),
  GST_PLAY_FLAG_FORCE_AUDIO_CONVERT = (1 << 14),
} GstPlayFlags;

GST_COOL_API gboolean gst_cool_playbin_init (GstElement * playbin);

GST_COOL_API
    gboolean gst_cool_playbin_no_resource_mode (GstElement * playbin,
    gboolean use_decoder_proxy, gboolean is_sync);

GST_COOL_API gboolean gst_cool_playbin_is_resource_mode (void);

GST_COOL_API GstElement *gst_cool_playbin_get_q2 (GstElement * playbin);

GST_COOL_API
    void gst_cool_playbin_set_q2_conf (GstElement * playbin,
    const gchar * firstfield, ...);

GST_COOL_API
    void gst_cool_playbin_set_default_audio_sink (GstElement * playbin,
    const gchar * audiosink);

G_END_DECLS
#endif
