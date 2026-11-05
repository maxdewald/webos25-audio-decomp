/* SPDX-License-Identifier: LGPL-2.0-or-later
 * Copied unchanged (except include paths) from LG gst-plugins-cool 1.24 for the
 * libgstdecproxy reconstruction; matches the shipped webOS 25 binary
 * (sha256 df504a8ac782e36ab8edf665cb7441ddeb5c75e98cb5db2b8375e385e1db34e5) per its DWARF layouts. */
/* GStreamer Cool Library
 * Copyright (C) 2020 LG Electronics, Inc.
 *	Author : ekwange <ekwang.lee@lge.com>
 *
 * cool-prelude.h: prelude include header for gst-cool library
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
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GST_COOL_PRELUDE_H__
#define __GST_COOL_PRELUDE_H__

#include <gst/gst.h>

#ifndef GST_COOL_API
#ifdef BUILDING_GST_COOL
#define GST_COOL_API GST_API_EXPORT        /* from config.h */
#else
#define GST_COOL_API GST_API_IMPORT
#endif
#endif

#endif /* __GST_COOL_PRELUDE_H__ */
