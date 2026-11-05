/*
 * GstLgAudioRender data flow: chain/preroll/event/query, start/stop, clock
 * handling and the lgao event task of the "audiosink" element
 * (gst-plugins-bsp/gstlgaudio/gstlgaudiosink.c).
 * RECONSTRUCTION from usr/lib/gstreamer-1.0/libgstlgaudiosink.so
 *   sha256 90dba3ef32704ce4096bf1e6045d2879d0c2192d507bc04d1e8e98437f420a43
 * Tags are raw ELF VAs (Ghidra address - 0x10000, see decomp.toml src_addr_bias).
 * Evidence: build/decomp/libgstlgaudiosink.so.c, .strings.tsv, DWARF (gstlgaudiosink.c CU).
 * Log line numbers in SINK_LOG(...) are the original __LINE__ values.
 */
#include <string.h>
#include <stdio.h>
#include <gst/gst.h>
#include <gst/base/gstbasesink.h>
#include <gst/audio/audio.h>
#include "lgao.h"
#include "render_element.h"

#include <sys/msg.h>
#include <unistd.h>
#include <math.h>

/* libgstlgclock (imported) */
extern gpointer gst_lgclock_new (GstElement *element);

/* @0x4488 gst_lg_audio_render_clock_calibrated: class handler of "clock-calibrated";
 * the next chain() call re-programs the renderer base time. */
void gst_lg_audio_render_clock_calibrated (GstLgAudioRender *pAudioRen)
{
    SINK_LOG (2, GST_LEVEL_INFO, 760, pAudioRen, "clock calibrated.");
    if (GST_STATE (pAudioRen) == GST_STATE_PLAYING)
        pAudioRen->bClockCalibrated = TRUE;
}

/* @0x4f60 gst_lg_audio_render_update_ts_offset: basesink ts-offset from playspeed
 * and lipsync offset. Normal speeds (0.25x..0.999x) get a fixed -475 s / rate. */
void gst_lg_audio_render_update_ts_offset (GstLgAudioRender *pAudioRen, gint playspeed)
{
    GstClockTimeDiff ts_offset;

    if ((guint) (playspeed - 250) < 750) {
        ts_offset = -475000000000LL / playspeed;
        SINK_LOG (3, GST_LEVEL_INFO, 600, pAudioRen, "Set TS Offset to %lld", ts_offset);
        gst_base_sink_set_ts_offset (GST_BASE_SINK (pAudioRen), ts_offset);
        return;
    }

    if (pAudioRen->lipsync_offset >= 400)
        ts_offset = 0;
    else if (pAudioRen->pAppType != NULL && strcmp (pAudioRen->pAppType, "ID-GAPLESS") == 0)
        ts_offset = -400 * GST_MSECOND;
    else
        ts_offset = (GstClockTimeDiff) (400 - pAudioRen->lipsync_offset) * -GST_MSECOND;

    SINK_LOG (3, GST_LEVEL_INFO, 615, pAudioRen, "Set TS Offset to %lld", ts_offset);
    gst_base_sink_set_ts_offset (GST_BASE_SINK (pAudioRen), ts_offset);
}

/* @0x5158 gst_lg_audio_render_update_clock: program renderer STC/PTS base.
 * lgao_set_basetime() was inlined here (GSTC ioctl, 90 kHz conversion, AREN_SET_BASETIME). */
void gst_lg_audio_render_update_clock (GstLgAudioRender *pAudioRen, gint64 base, gint64 start)
{
    GstClockTime latency = gst_base_sink_get_latency (GST_BASE_SINK (pAudioRen));
    GstClockTime render_delay = gst_base_sink_get_render_delay (GST_BASE_SINK (pAudioRen));
    gint64 basetime = base - latency + render_delay;

    SINK_LOGM (3, 584, "basetime %u:%02u:%02u.%09u = base %u:%02u:%02u.%09u - latency "
               "%u:%02u:%02u.%09u + render_delay %u:%02u:%02u.%09u, start %u:%02u:%02u.%09u",
               GST_TIME_ARGS (basetime), GST_TIME_ARGS (base), GST_TIME_ARGS (latency),
               GST_TIME_ARGS (render_delay), GST_TIME_ARGS (start));
    SINK_GST (GST_LEVEL_INFO, 584, pAudioRen, "basetime %u:%02u:%02u.%09u = base %u:%02u:%02u.%09u - latency "
              "%u:%02u:%02u.%09u + render_delay %u:%02u:%02u.%09u, start %u:%02u:%02u.%09u",
              GST_TIME_ARGS (basetime), GST_TIME_ARGS (base), GST_TIME_ARGS (latency),
              GST_TIME_ARGS (render_delay), GST_TIME_ARGS (start));

    lgao_set_basetime (pAudioRen->ao, basetime, start);
}

/* @0x5a08 gst_lg_audio_render_prepared_instant_rate_change: LG GstBaseSinkClass vfunc,
 * called after basesink applied an instant-rate-change segment.rate. With a video port
 * the renderer clock is re-based on the video decoder's current PTS (90 kHz). */
gboolean gst_lg_audio_render_prepared_instant_rate_change (GstBaseSink *sink, gdouble rate_multiplier)
{
    GstLgAudioRender *pAudioRen = (GstLgAudioRender *) sink;
    GstSegment *segment = &sink->segment;
    gdouble rate = segment->rate;
    GstClockTime now = gst_clock_get_time (GST_ELEMENT (pAudioRen)->clock);
    GstClockTime base;
    guint pts;

    (void) rate_multiplier;     /* segment.rate is already the new rate */
    pAudioRen->instant_rate_change = TRUE;
    pAudioRen->avail_irc_vdec_pts = TRUE;
    SINK_LOG (3, GST_LEVEL_INFO, 695, pAudioRen, "Instant rate change");

    if (pAudioRen->video_port != -1) {
        base = now - pAudioRen->lipsync_offset * 1000000;
        pts = 0;
        lgao_get_vdec_current_pts (pAudioRen->ao, pAudioRen->video_port, &pts);
        pAudioRen->irc_pts = (guint64) (pts / 9) * 100000;     /* 90 kHz -> ns */

        SINK_LOG (3, GST_LEVEL_INFO, 704, pAudioRen,
                  "instant rate : %f, now : %u:%02u:%02u.%09u, base :%u:%02u:%02u.%09u, "
                  "irc_pts : %u:%02u:%02u.%09u, current timestamp : %u:%02u:%02u.%09u", rate,
                  GST_TIME_ARGS (now), GST_TIME_ARGS (base), GST_TIME_ARGS (pAudioRen->irc_pts),
                  GST_TIME_ARGS (pAudioRen->currentPTS));
        SINK_LOG (3, GST_LEVEL_INFO, 711, pAudioRen, "irc_pts = %llu \n", pAudioRen->irc_pts);

        if (pAudioRen->irc_pts != 0 && pAudioRen->irc_pts != pAudioRen->currentPTS) {
            SINK_LOG (3, GST_LEVEL_INFO, 715, pAudioRen,
                      "basetime update.... irc_pts : %u:%02u:%02u.%09u, cur_pts : %u:%02u:%02u.%09u",
                      GST_TIME_ARGS (pAudioRen->irc_pts), GST_TIME_ARGS (pAudioRen->currentPTS));
            SINK_LOG (3, GST_LEVEL_INFO, 720, pAudioRen,
                      "update clock basetime : %u:%02u:%02u.%09u, lipsync_offset : "
                      "%u:%02u:%02u.%09u, cur_pts : %u:%02u:%02u.%09u", GST_TIME_ARGS (base),
                      GST_TIME_ARGS ((GstClockTime) (pAudioRen->lipsync_offset * 1000000)),
                      GST_TIME_ARGS (pAudioRen->irc_pts));
            gst_lg_audio_render_update_clock (pAudioRen, base, pAudioRen->irc_pts);
        } else {
            SINK_LOG (3, GST_LEVEL_INFO, 730, pAudioRen,
                      "basetime skip.... irc_pts : %u:%02u:%02u.%09u, cur_pts : %u:%02u:%02u.%09u",
                      GST_TIME_ARGS (pAudioRen->irc_pts), GST_TIME_ARGS (pAudioRen->currentPTS));
        }
        if (pAudioRen->vsp_mode)
            lgao_set_vsp_mode (pAudioRen->ao);
    }

    if (GST_STATE (pAudioRen) == GST_STATE_PLAYING)
        lgao_set_playrate (pAudioRen->ao, (gint) floor (rate * 1000.0 + 0.5));
    return TRUE;
}

/* @0x6f1c gst_lg_audio_render_dump_open: (re)open /build/scratch/gstlgaudiosink_<self>_<n>.dump */
void gst_lg_audio_render_dump_open (GstLgAudioRender *pAudioRen)
{
    GString *filename;

    if (pAudioRen->dump_file != NULL)
        fclose (pAudioRen->dump_file);

    filename = g_string_new ("/build/scratch/gstlgaudiosink_");
    if (filename == NULL) {
        SINK_GST (GST_LEVEL_ERROR, 284, pAudioRen, "can't alloc str");
        return;
    }
    pAudioRen->dump_index++;
    g_string_append_printf (filename, "%p_%03d.dump", pAudioRen, pAudioRen->dump_index);
    pAudioRen->dump_file = fopen (filename->str, "wb");
    if (pAudioRen->dump_file == NULL)
        SINK_GST (GST_LEVEL_ERROR, 292, pAudioRen, "can't open dump file. %s", filename->str);
    g_string_free (filename, TRUE);
}

/* @0x7004 gst_lg_audio_render_callback_task: GstTask body; one lgao event per
 * iteration from the SysV message queue that lgao's appsink callback fills. */
void gst_lg_audio_render_callback_task (void *data)
{
    GstLgAudioRender *pAudioRen = data;
    LGAO_STATE_T rendererState;
    lgao_mq_item msg;
    int eventMsg;

    if (pAudioRen == NULL) {
        SINK_LOG (0, GST_LEVEL_ERROR, 392, NULL, "pAudioRen is NULL");
        return;
    }
    if (!pAudioRen->event_mq_running) {
        usleep (0);
        return;
    }
    if (msgrcv (pAudioRen->event_mq_id, &msg, sizeof (msg.event), 0, 0) <= 0) {
        SINK_LOG (0, GST_LEVEL_ERROR, 403, pAudioRen, "mq error");
        return;
    }
    eventMsg = msg.event;
    SINK_LOG (4, GST_LEVEL_DEBUG, 408, pAudioRen, "Got Event (0x%x) from Renderer", eventMsg);

    if (eventMsg & LGAO_EVENT_CLOSE_IPC) {
        /* stop() kicks the blocked msgrcv() with this */
        SINK_LOG (3, GST_LEVEL_INFO, 411, pAudioRen, "Got LGAO_EVENT_CLOSE_IPC event. kicked!");
        pAudioRen->event_mq_running = FALSE;
        return;
    }
    if (eventMsg & LGAO_EVENT_PRESENT_END) {
        /* wakes the EOS wait in event() */
        SINK_LOG (3, GST_LEVEL_INFO, 417, pAudioRen, "Got LGAO_EVENT_PRESENT_END event");
        g_mutex_lock (&pAudioRen->eos_mutex);
        g_cond_signal (&pAudioRen->eos_cond);
        g_mutex_unlock (&pAudioRen->eos_mutex);
    }
    if (eventMsg & LGAO_EVENT_UNDERFLOW) {
        SINK_LOG (3, GST_LEVEL_INFO, 424, pAudioRen, "Got LGAO_EVENT_UNDERFLOW event");
        SINK_CHECK (lgao_get_state (pAudioRen->ao, &rendererState), 426);
        if (rendererState == LGAO_STATE_PLAY && !pAudioRen->isSeamless) {
            SINK_LOG (2, GST_LEVEL_INFO, 430, pAudioRen, "Send underflow signal to upper layer");
            g_signal_emit (pAudioRen, gst_lg_audio_render_signals[SIGNAL_UNDERRUN], 0);
        } else {
            SINK_LOG (4, GST_LEVEL_DEBUG, 433, pAudioRen,
                      "not send underflow signal(state=%d, seamless=%d)\n",
                      rendererState, pAudioRen->isSeamless);
        }
    }
    if (eventMsg & LGAO_EVENT_BYPASS_DONE) {
        SINK_LOG (2, GST_LEVEL_INFO, 440, pAudioRen, "Got LGAO_EVENT_BYPASS_DONE event, send signal..");
        g_signal_emit_by_name (GST_ELEMENT (pAudioRen), "es-bypass-done");
    }
}

/* @0x74d8 gst_lg_audio_render_provide_clock: LG hardware clock (libgstlgclock) */
GstClock *gst_lg_audio_render_provide_clock (GstElement *element)
{
    return GST_CLOCK (gst_lgclock_new (element));
}

/* @0x75d0 gst_lg_audio_render_prepare_seamless_seek: LG GstBaseSinkClass vfunc, called
 * for a seamless (flushless) seek with the new segment; restarts the renderer on the
 * new base time without a FLUSH. */
gboolean gst_lg_audio_render_prepare_seamless_seek (GstBaseSink *sink, GstSegment *segment, GstClockTime new_base_time)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (sink);
    gint newPlaySpeed = (gint) floor (segment->rate * 1000.0 + 0.5);
    gint playspeed = 0;
    GstClockTime now;
    GstClockTimeDiff base_time;
    GstClockTime base_pts;

    SINK_CHECK (lgao_get_playrate (pAudioRen->ao, &playspeed), 628);

    if (pAudioRen->currentState != GST_STATE_PLAYING) {
        if (playspeed != newPlaySpeed)
            pAudioRen->bSeamlessSeek = TRUE;
        return TRUE;
    }

    pAudioRen->bSeamlessSeek = TRUE;
    now = gst_clock_get_time (GST_ELEMENT (pAudioRen)->clock);

    {
        GstClockTimeDiff diff = (GstClockTimeDiff) (pAudioRen->currentPTS - segment->start);
        GstClockTime adiff = diff < 0 ? (GstClockTime) -diff : (GstClockTime) diff;
        SINK_LOG (4, GST_LEVEL_DEBUG, 638, pAudioRen,
                  "rate: %f, segment.start: %u:%02u:%02u.%09u, pts: %u:%02u:%02u.%09u, "
                  "diff: %c%u:%02u:%02u.%09u", segment->rate, GST_TIME_ARGS (segment->start),
                  GST_TIME_ARGS (pAudioRen->currentPTS), diff < 0 ? '-' : '+', GST_TIME_ARGS (adiff));
    }
    SINK_LOG (4, GST_LEVEL_DEBUG, 643, pAudioRen,
              "now: %u:%02u:%02u.%09u, new_base_time: %u:%02u:%02u.%09u",
              GST_TIME_ARGS (now), GST_TIME_ARGS (new_base_time));

    gst_lg_audio_render_update_ts_offset (pAudioRen, newPlaySpeed);

    if (new_base_time == GST_CLOCK_TIME_NONE)
        base_time = segment->base;
    else
        base_time = now + segment->base - new_base_time;
    base_pts = segment->start;

    if (pAudioRen->decodedSize != 0)
        lgao_stop_renderer (pAudioRen->ao);
    lgao_set_playrate (pAudioRen->ao, newPlaySpeed);
    gst_lg_audio_render_update_clock (pAudioRen, base_time, base_pts);
    lgao_play (pAudioRen->ao);
    lgao_start_renderer (pAudioRen->ao);
    pAudioRen->bSeamlessSeek = FALSE;
    return TRUE;
}

/* @0x8058 gst_lg_audio_render_preroll: data is only fed in render (chain); preroll
 * pauses the renderer clock on PLAYING->PAUSED and records the PTS. */
GstFlowReturn gst_lg_audio_render_preroll (GstBaseSink *sink, GstBuffer *buffer)
{
    SINK_LOG (2, GST_LEVEL_INFO, 3151, sink, "preroll done");

    /* Going PLAYING->PAUSED: freeze the renderer (rate 0) unless an instant rate
     * change is in progress (Ghidra shows this as "bsink[1].need_preroll" = +0x2e8). */
    if (GST_STATE (sink) == GST_STATE_PLAYING && GST_STATE_NEXT (sink) == GST_STATE_PAUSED &&
        !GST_LG_AUDIO_RENDER (sink)->instant_rate_change)
        lgao_set_playrate (GST_LG_AUDIO_RENDER (sink)->ao, 0);

    if (GST_BUFFER_PTS_IS_VALID (buffer)) {
        GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (sink);
        pAudioRen->currentPTS = GST_BUFFER_PTS (buffer);
        SINK_GST (GST_LEVEL_DEBUG, 3166, pAudioRen, "current_timestamp %u:%02u:%02u.%09u, %s:%d",
                  GST_TIME_ARGS (pAudioRen->currentPTS), __func__, 3168);
    }
    return GST_FLOW_OK;
}

#define CAPS_LOG_MAX_LEN 0x400

/* gst_lg_audio_render_set_caps (gstlgaudiosink.c:~1130): always inlined into event(),
 * no symbol. Parses the caps into LGAO_AUDIO_START_INFO_T and (re)opens the renderer.
 * Returns FALSE only for caps without structure/name or a non-audio mime. */
static inline gboolean
gst_lg_audio_render_set_caps (GstLgAudioRender *pAudioRen, GstCaps *caps, gboolean *caps_changed)
{
    const GstStructure *structure;
    LGAO_AUDIO_START_INFO_T AudioInfo;
    const gchar *mime, *format, *codec_str;
    gint getValue;
    guint acmod, check_downmix, codec, curr_acmod = 0, curr_downmix = 0;
    gboolean lfe_enable, dolby, clock_x4, atmos, restart, es, downmix;
    gint out_channels, gap_correction, data_channels, data_rate, data_bps;
    gint mode_mixer = 0, port_index = -1;
    guint64 channel_mask;
    GstStructure *media_info;
    gchar *stream_id, *str;
    int ret;

    if (pAudioRen->pAudioCaps) {
        gst_caps_unref (pAudioRen->pAudioCaps);
        pAudioRen->pAudioCaps = NULL;
    }
    pAudioRen->pAudioCaps = gst_caps_copy (caps);
    memset (&AudioInfo, 0, sizeof (AudioInfo));

    SINK_CHECK (lgao_get_mode_mixer (pAudioRen->ao, &mode_mixer), 1130);
    SINK_CHECK (lgao_get_port (pAudioRen->ao, &port_index), 1131);

    str = gst_caps_to_string (caps);
    if (str) {
        if (strlen (str) < CAPS_LOG_MAX_LEN)
            SINK_LOG (3, GST_LEVEL_INFO, 1135, pAudioRen, "%s.%d - caps \"%s\"",
                      mode_mixer == 1 ? "MIX" : "DEC", port_index, str);
        else
            SINK_LOG (3, GST_LEVEL_INFO, 1138, pAudioRen,
                      "caps string len is %d. So doesn't print audio caps string", (gint) strlen (str));
        g_free (str);
    }

    structure = gst_caps_get_structure (caps, 0);
    if (structure == NULL) {
        SINK_LOG (0, GST_LEVEL_ERROR, 1145, pAudioRen, "unable to get structure from caps");
        return FALSE;
    }

    /* ES bypass towards the "drop renderer": nothing is configured */
    if (gst_structure_get_int (structure, "BYPASS", &getValue)) {
        SINK_LOG (3, GST_LEVEL_INFO, 1150, pAudioRen, "BYPASS %d..\n", getValue);
        if (getValue != 0) {
            lgao_set_drop_renderer (pAudioRen->ao, 1);
            SINK_LOG (3, GST_LEVEL_INFO, 1153, pAudioRen, "renderer will be bypass...\n");
            return TRUE;
        }
    }
    if (gst_structure_get_int (structure, "DUMMY", &getValue)) {
        SINK_LOG (3, GST_LEVEL_INFO, 1159, pAudioRen, "This caps is dummy. So drop this");
        return TRUE;
    }

    mime = gst_structure_get_name (structure);
    if (mime == NULL) {
        SINK_LOG (0, GST_LEVEL_ERROR, 1164, pAudioRen, "unable to get MIME type");
        return FALSE;
    }
    SINK_LOG (4, GST_LEVEL_LOG, 1167, pAudioRen, "mime = %s", mime);
    if (strstr (mime, "audio") == NULL) {
        SINK_LOG (0, GST_LEVEL_ERROR, 1170, pAudioRen, "not a audio data(mime=%s)", mime);
        return FALSE;
    }

    if (gst_structure_get_int (structure, "rate", &getValue)) {
        AudioInfo.freq = getValue;
        SINK_LOG (4, GST_LEVEL_LOG, 1176, pAudioRen, "sampling-rate = %d", getValue);
    }
    if (gst_structure_get_int (structure, "channels", &getValue)) {
        AudioInfo.numberOfChannel = (unsigned char) getValue;
        SINK_LOG (4, GST_LEVEL_LOG, 1181, pAudioRen, "channels = %d", getValue);
    }

    /* every accepted caps needs a "format" field, also ES (IEC 61937) caps */
    format = gst_structure_get_string (structure, "format");
    if (format == NULL) {
        SINK_LOG (0, GST_LEVEL_ERROR, 1222, pAudioRen, "PCM format is NULL");
        return TRUE;
    }
    SINK_LOG (4, GST_LEVEL_LOG, 1189, pAudioRen, "PCM format = %s", format);
    {
        GstAudioFormat fmt = gst_audio_format_from_string (format);
        const GstAudioFormatInfo *finfo;

        if (fmt == GST_AUDIO_FORMAT_UNKNOWN) {
            SINK_LOG (0, GST_LEVEL_ERROR, 1218, pAudioRen,
                      "gst_audio_format_from_string () Err(GST_ADEC_FORMAT_UNKNOWN)");
        } else if ((finfo = gst_audio_format_get_info (fmt)) == NULL) {
            SINK_LOG (0, GST_LEVEL_ERROR, 1215, pAudioRen, "gst_audio_format_get_info() Err");
        } else if (GST_AUDIO_FORMAT_INFO_IS_INTEGER (finfo)) {
            AudioInfo.bitsPerSample = (unsigned char) finfo->width;
            SINK_LOG (4, GST_LEVEL_LOG, 1198, pAudioRen, "bitsPerSample = %d", finfo->width);
            AudioInfo.unsignedOfPcm = !GST_AUDIO_FORMAT_INFO_IS_SIGNED (finfo);
            SINK_LOG (4, GST_LEVEL_LOG, 1204, pAudioRen, "unsigned = %d", AudioInfo.unsignedOfPcm);
            AudioInfo.bigEndianOfPcm = finfo->endianness != G_LITTLE_ENDIAN;
            SINK_LOG (4, GST_LEVEL_LOG, 1210, pAudioRen, "endian = %d", AudioInfo.bigEndianOfPcm);
        } else {
            SINK_LOG (4, GST_LEVEL_LOG, 1212, pAudioRen, "Floating PCM");
        }
    }

    if (gst_structure_get_uint (structure, "acmod", &acmod)) {
        AudioInfo.acmod = acmod;
        SINK_LOG (4, GST_LEVEL_LOG, 1228, pAudioRen, "acmod = %d", acmod);
    } else if (AudioInfo.numberOfChannel == 1) {
        AudioInfo.acmod = 1;
        SINK_LOG (4, GST_LEVEL_LOG, 1232, pAudioRen, "channel = %d, acmod = %d",
                  AudioInfo.numberOfChannel, AudioInfo.acmod);
    } else if (gst_structure_get (structure, "channel-mask", GST_TYPE_BITMASK, &channel_mask, NULL)) {
        /* mono (front-center) -> 1/0, stereo-ish -> 2/0, anything else -> 3/2 */
        AudioInfo.acmod = channel_mask == 2 ? 1 : channel_mask == 1 ? 2 : 7;
        SINK_LOG (4, GST_LEVEL_LOG, 1245, pAudioRen, "channel-mask = %d, acmod = %d",
                  (gint) channel_mask, AudioInfo.acmod);
    } else {
        SINK_LOG (4, GST_LEVEL_LOG, 1248, pAudioRen, "acmod or channel-mask field is empty");
    }

    if (gst_structure_get_uint (structure, "check-downmix", &check_downmix)) {
        AudioInfo.check_downmix = check_downmix;
        SINK_LOG (4, GST_LEVEL_LOG, 1255, pAudioRen, "check-downmix = %d", check_downmix);
    } else {
        SINK_LOG (4, GST_LEVEL_LOG, 1257, pAudioRen, "check-downmix field is empty");
        AudioInfo.check_downmix = (unsigned int) -1;
    }
    if (gst_structure_get_boolean (structure, "lfe-enable", &lfe_enable)) {
        AudioInfo.lfe_enable = lfe_enable;
        SINK_LOG (4, GST_LEVEL_LOG, 1263, pAudioRen, "lfe_enable = %d", lfe_enable);
    } else {
        SINK_LOG (4, GST_LEVEL_LOG, 1265, pAudioRen, "lfe_enable field is empty");
    }
    if (gst_structure_get_boolean (structure, "dolby", &dolby)) {
        AudioInfo.dolby = dolby;
        SINK_LOG (4, GST_LEVEL_LOG, 1271, pAudioRen, "dolby = %d", dolby);
    } else {
        SINK_LOG (4, GST_LEVEL_LOG, 1273, pAudioRen, "dolby field is empty");
    }
    if (gst_structure_get_boolean (structure, "clock-x4", &clock_x4)) {
        AudioInfo.clock_x4 = clock_x4;
        SINK_LOG (4, GST_LEVEL_LOG, 1279, pAudioRen, "clock_x4 (eac3) = %d", clock_x4);
    } else {
        SINK_LOG (4, GST_LEVEL_LOG, 1281, pAudioRen, "clock_x4 (eac3) field is empty");
    }

    /* application message "media-info" {stream-id, immersive} for the media pipeline */
    media_info = gst_structure_new_empty ("media-info");
    stream_id = gst_pad_get_stream_id (GST_BASE_SINK_PAD (pAudioRen));
    if (stream_id) {
        gst_structure_set (media_info, "stream-id", G_TYPE_STRING, stream_id, NULL);
        SINK_LOG (3, GST_LEVEL_INFO, 1291, pAudioRen, "stream-id = %s", stream_id);
    }
    if (gst_structure_get_boolean (structure, "atmos", &atmos)) {
        AudioInfo.atmos = atmos;
        SINK_LOG (3, GST_LEVEL_INFO, 1296, pAudioRen, "atmos = %d", atmos);
        gst_structure_set (media_info, "immersive", G_TYPE_STRING, "ATMOS", NULL);
    } else {
        SINK_LOG (3, GST_LEVEL_INFO, 1300, pAudioRen, "atmos field is empty");
        gst_structure_set (media_info, "immersive", G_TYPE_STRING, "none", NULL);
    }
    if (media_info) {
        str = gst_structure_to_string (media_info);
        if (str) {
            SINK_LOG (3, GST_LEVEL_INFO, 1310, pAudioRen, "send media info: %s", str);
            g_free (str);
        }
        gst_element_post_message (GST_ELEMENT (pAudioRen),
                                  gst_message_new_custom (GST_MESSAGE_APPLICATION,
                                                          GST_OBJECT (pAudioRen), media_info));
    }

    if (gst_structure_get_int (structure, "out-channels", &out_channels)) {
        AudioInfo.out_channels = out_channels;
        SINK_LOG (4, GST_LEVEL_LOG, 1321, pAudioRen, "out_channels = %d", out_channels);
    } else {
        SINK_LOG (4, GST_LEVEL_LOG, 1323, pAudioRen, "out-channels field is empty");
        AudioInfo.out_channels = -1;
    }
    if (gst_structure_get_boolean (structure, "restart", &restart)) {
        AudioInfo.restart = restart;
        SINK_LOG (4, GST_LEVEL_LOG, 1329, pAudioRen, "restart = %d", restart);
    } else {
        SINK_LOG (4, GST_LEVEL_LOG, 1331, pAudioRen, "restart field is empty");
    }
    if (gst_structure_get_int (structure, "gap-correction", &gap_correction)) {
        AudioInfo.gap_correction = gap_correction;
        SINK_LOG (3, GST_LEVEL_INFO, 1352, pAudioRen, "gap_correction = %d", gap_correction);
    }
    if (gst_structure_get_uint (structure, "codec", &codec)) {
        AudioInfo.codecType = codec;
        SINK_LOG (4, GST_LEVEL_LOG, 1357, pAudioRen, "codec = %d", codec);
    } else {
        SINK_LOG (4, GST_LEVEL_LOG, 1359, pAudioRen, "codec field is empty");
        AudioInfo.codecType = LGAO_CODEC_PCM;
    }

    /* passthrough (IEC 61937) description, consumed by lgao's bypass/ES path */
    if (gst_structure_get_boolean (structure, "es", &es)) {
        AudioInfo.es = es;
        SINK_LOG (4, GST_LEVEL_LOG, 1365, pAudioRen, "es = %d", es);
    }
    if (gst_structure_get_int (structure, "data-channels", &data_channels)) {
        AudioInfo.data_channels = data_channels;
        SINK_LOG (4, GST_LEVEL_LOG, 1370, pAudioRen, "data_channels = %d", data_channels);
    }
    if (gst_structure_get_int (structure, "data-rate", &data_rate)) {
        AudioInfo.data_rate = data_rate;
        SINK_LOG (4, GST_LEVEL_LOG, 1375, pAudioRen, "data_rate = %d", data_rate);
    }
    if (gst_structure_get_int (structure, "data-bps", &data_bps)) {
        AudioInfo.data_bps = data_bps;
        SINK_LOG (4, GST_LEVEL_LOG, 1380, pAudioRen, "data_bps = %d", data_bps);
    }
    codec_str = gst_structure_get_string (structure, "codec-str");
    if (codec_str) {
        /* fixed 16-byte copy (no strlen/strncpy in the binary) */
        memcpy (AudioInfo.codec_str, codec_str, sizeof (AudioInfo.codec_str));
        SINK_LOG (4, GST_LEVEL_LOG, 1385, pAudioRen, "codec-str = %s -> %s", codec_str,
                  AudioInfo.codec_str);
    }
    if (gst_structure_get_boolean (structure, "downmix", &downmix)) {
        AudioInfo.downmix = downmix;
        SINK_LOG (4, GST_LEVEL_LOG, 1391, pAudioRen, "downmix = %d", downmix);
    } else {
        SINK_LOG (4, GST_LEVEL_LOG, 1393, pAudioRen, "downmix field is empty");
    }

    pAudioRen->isUnsupportedStream = FALSE;
    if (AudioInfo.freq > 384000 || AudioInfo.numberOfChannel > 10) {
        if (AudioInfo.freq > 384000) {
            SINK_LOG (0, GST_LEVEL_ERROR, 1400, pAudioRen, "freq(%u) is not supported", AudioInfo.freq);
            pAudioRen->isUnsupportedStream = TRUE;
        }
        if (AudioInfo.numberOfChannel > 10) {
            SINK_LOG (0, GST_LEVEL_ERROR, 1406, pAudioRen, "channels(%u) is not supported",
                      AudioInfo.numberOfChannel);
            pAudioRen->isUnsupportedStream = TRUE;
        }
        SINK_LOG (0, GST_LEVEL_ERROR, 1413, pAudioRen, "Send ADEC Warning msg to upper layer");
        GST_ELEMENT_WARNING (pAudioRen, STREAM, CODEC_NOT_FOUND, ("This audio is not supported"),
                             ("This audio is not supported"));
        g_free (stream_id);
        return TRUE;
    }

    SINK_CHECK (lgao_get_audio_info (pAudioRen->ao, LGAO_AUDIO_INFO_ACMOD, &curr_acmod), 1423);

    ret = lgao_compare_audio_info (pAudioRen->ao, &AudioInfo);
    switch (ret) {
    case 0:     /* same PCM format, no restart */
        SINK_LOG (3, GST_LEVEL_INFO, 1453, pAudioRen, "same caps for normal play received. ignore");
        break;
    case 2:     /* same PCM format, restart=TRUE */
        SINK_LOG (2, GST_LEVEL_INFO, 1458, pAudioRen, "Connect ES Renderer Only");
        break;
    case -1: {  /* nothing configured yet: only with a valid audio port */
        gint cur_port = -1;
        SINK_CHECK (lgao_get_port (pAudioRen->ao, &cur_port), 1439);
        if (cur_port < 0) {
            SINK_LOG (2, GST_LEVEL_INFO, 1445, pAudioRen, "audio port(%d) in invalid! ignore", cur_port);
            break;
        }
        goto open_renderer;
    }
    default:    /* not produced by lgao_compare_audio_info: open without teardown */
        goto open_renderer;
    case 1:     /* first caps or format changed */
        SINK_LOG (2, GST_LEVEL_INFO, 1427, pAudioRen, "Caps is first or updated");
        lgao_stop_renderer (pAudioRen->ao);
        lgao_disconnect_renderer (pAudioRen->ao);
        lgao_close_renderer (pAudioRen->ao);
    open_renderer:
        /* with the drop renderer active (caps BYPASS / buffer flag) nothing is opened */
        if (!lgao_get_drop_renderer (pAudioRen->ao)) {
            SINK_CHECK (lgao_open_renderer (pAudioRen->ao), 1467);
            SINK_CHECK (lgao_set_audio_info (pAudioRen->ao, AudioInfo), 1468);
            SINK_CHECK (lgao_config (pAudioRen->ao), 1469);
            SINK_CHECK (lgao_connect_renderer (pAudioRen->ao), 1470);
            if (pAudioRen->currentState == GST_STATE_PLAYING) {
                SINK_CHECK (lgao_play (pAudioRen->ao), 1474);
                SINK_CHECK (lgao_start_renderer (pAudioRen->ao), 1475);
            }
        }
        *caps_changed = TRUE;
        break;
    }

    /* always re-store the info; a changed acmod/downmix re-sends the first event */
    SINK_CHECK (lgao_get_audio_info (pAudioRen->ao, LGAO_AUDIO_INFO_DOWNMIX, &curr_downmix), 1491);
    SINK_CHECK (lgao_set_audio_info (pAudioRen->ao, AudioInfo), 1494);
    if (AudioInfo.acmod != curr_acmod || AudioInfo.downmix != curr_downmix) {
        if (AudioInfo.acmod != curr_acmod)
            SINK_LOG (3, GST_LEVEL_INFO, 1496, pAudioRen, "acmod changed %d -> %d", curr_acmod,
                      AudioInfo.acmod);
        if (AudioInfo.downmix != curr_downmix)
            SINK_LOG (3, GST_LEVEL_INFO, 1501, pAudioRen, "downmix mode changed %d -> %d",
                      curr_downmix, AudioInfo.downmix);
        lgao_push_first_event (pAudioRen->ao);
    }
    g_free (stream_id);
    return TRUE;
}

/* @0x8240 gst_lg_audio_render_event: GstBaseSink::event; everything except an upstream
 * mpegh-object-ui OOB event and a failed CAPS is chained to GstBaseSink. */
gboolean gst_lg_audio_render_event (GstBaseSink *sink, GstEvent *event)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (sink);
    GstElement *element = GST_ELEMENT (sink);

    SINK_LOG (4, GST_LEVEL_LOG, 2221, pAudioRen, "got event(%s)", GST_EVENT_TYPE_NAME (event));

    switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_GAP:
        SINK_LOG (3, GST_LEVEL_INFO, 2226, pAudioRen, "Received Event = GST_EVENT_GAP");
        break;

    case GST_EVENT_FLUSH_START:
        SINK_LOG (3, GST_LEVEL_INFO, 2231, pAudioRen, "Received GST_EVENT_FLUSH_START event");
        pAudioRen->isFlushing = TRUE;
        pAudioRen->bSeamlessSeek = FALSE;
        break;

    case GST_EVENT_FLUSH_STOP:
        SINK_LOG (3, GST_LEVEL_INFO, 2238, pAudioRen, "Received Event = GST_EVENT_FLUSH_STOP");
        lgao_set_playrate (pAudioRen->ao, 0);
        lgao_flush (pAudioRen->ao);
        pAudioRen->isFlushing = FALSE;
        pAudioRen->decodedSize = 0;
        break;

    case GST_EVENT_EOS: {
        gint buffered_size = 0, send_first_event = 0;

        SINK_LOG (3, GST_LEVEL_INFO, 2252, pAudioRen, "Received Event = GST_EVENT_EOS");
        lgao_get_size_buffered (pAudioRen->ao, LGAO_DATA_PCM, &buffered_size);
        lgao_get_first_render (pAudioRen->ao, LGAO_DATA_PCM, &send_first_event);
        SINK_LOG (3, GST_LEVEL_INFO, 2258, pAudioRen, "buffered_size: %d, send_first_event: %d",
                  buffered_size, send_first_event);
        if (buffered_size == 0 && send_first_event == 1) {
            /* nothing was ever rendered: don't wait for PRESENT_END */
            SINK_LOG (2, GST_LEVEL_INFO, 2262, pAudioRen, "do not send & wait eos event");
        } else if (lgao_eos (pAudioRen->ao) > 0) {
            SINK_LOG (2, GST_LEVEL_INFO, 2265, pAudioRen, "do not handle eos");
        } else {
            /* callback_task signals eos_cond on LGAO_EVENT_PRESENT_END */
            gint64 end_time = g_get_monotonic_time () + G_TIME_SPAN_SECOND;

            g_mutex_lock (&pAudioRen->eos_mutex);
            if (g_cond_wait_until (&pAudioRen->eos_cond, &pAudioRen->eos_mutex, end_time))
                SINK_LOG (3, GST_LEVEL_INFO, 2271, pAudioRen, "PRESENT_END for EOS received");
            else
                SINK_LOG (3, GST_LEVEL_INFO, 2273, pAudioRen, "EOS terminated due to timeout");
            g_mutex_unlock (&pAudioRen->eos_mutex);
        }
        SINK_LOG (2, GST_LEVEL_INFO, 2278, pAudioRen, "EOS end");
        break;
    }

    case GST_EVENT_SEGMENT: {
        const GstSegment *segment;
        gchar *str;
        gint playspeed;

        SINK_LOG (3, GST_LEVEL_INFO, 2287, pAudioRen, "Received Event = GST_EVENT_SEGMENT");
        gst_event_parse_segment (event, &segment);
        str = gst_info_strdup_printf ("%" GST_SEGMENT_FORMAT, segment);
        SINK_LOG (3, GST_LEVEL_INFO, 2291, pAudioRen, "%s", str);
        g_free (str);

        pAudioRen->playrate = segment->rate;
        pAudioRen->segment_start = segment->start;
        pAudioRen->segment_position = segment->position;
        playspeed = (gint) floor (segment->rate * 1000.0 + 0.5);
        if (lgao_set_playrate (pAudioRen->ao, playspeed) != 0)
            SINK_LOG (0, GST_LEVEL_ERROR, 2299, pAudioRen,
                      "lgao_set_playrate failed with value %d", playspeed);

        SINK_LOG (3, GST_LEVEL_INFO, 2303, pAudioRen,
                  "init decodedSize(current decodedSize is %llu)", pAudioRen->decodedSize);
        pAudioRen->decodedSize = 0;

        /* new segment while PLAYING: re-base the renderer clock right away */
        if (pAudioRen->currentState == GST_STATE_PLAYING)
            gst_lg_audio_render_update_clock (pAudioRen,
                                              gst_clock_get_time (element->clock) - segment->base -
                                              element->base_time, segment->start);

        if (pAudioRen->pAppType) {
            if (strcmp (pAudioRen->pAppType, "AIRPLAY") != 0)
                gst_lg_audio_render_update_ts_offset (pAudioRen, playspeed);
            if (pAudioRen->pAppType && strcmp (pAudioRen->pAppType, "ID-GAPLESS") == 0)
                gst_base_sink_set_render_delay (GST_BASE_SINK (pAudioRen), 0);
        }
        pAudioRen->avail_irc_vdec_pts = FALSE;
        break;
    }

    case GST_EVENT_CAPS: {
        GstCaps *caps;
        gboolean caps_changed = FALSE;

        SINK_LOG (3, GST_LEVEL_INFO, 2332, pAudioRen, "Received Event = GST_EVENT_CAPS");
        gst_event_parse_caps (event, &caps);
        if (!gst_lg_audio_render_set_caps (pAudioRen, caps, &caps_changed)) {
            SINK_LOG (0, GST_LEVEL_ERROR, 2344, pAudioRen, "gst_lg_audio_render_set_caps() Error");
            gst_event_unref (event);
            return FALSE;
        }
        /* renderer (re)opened after PLAYING started: program the base time now */
        if (pAudioRen->currentState == GST_STATE_PLAYING && caps_changed) {
            SINK_LOG (3, GST_LEVEL_INFO, 2350, pAudioRen, "Update clock when CAPS event is late");
            gst_lg_audio_render_update_clock (pAudioRen,
                                              gst_clock_get_time (element->clock) - element->base_time -
                                              sink->segment.base, sink->segment.start);
        }
        break;
    }

    case GST_EVENT_STREAM_START: {
        const gchar *stream_id = NULL;

        SINK_LOG (3, GST_LEVEL_INFO, 2367, pAudioRen, "Received Event = GST_EVENT_STREAM_START");
        gst_event_parse_stream_start (event, &stream_id);
        if (stream_id == NULL) {
            SINK_LOG (0, GST_LEVEL_ERROR, 2371, pAudioRen, "Stream ID is NULL");
            break;
        }
        SINK_LOG (3, GST_LEVEL_INFO, 2375, pAudioRen, "Stream ID: %s", stream_id);

        /* decodebin3 track switch on a non-seamless sink: full renderer restart */
        if (!pAudioRen->isSeamless && pAudioRen->pStreamId &&
            strcmp (pAudioRen->pStreamId, stream_id) != 0) {
            lgao_keep_fade (pAudioRen->ao);
            lgao_stop_renderer (pAudioRen->ao);
            lgao_disconnect_renderer (pAudioRen->ao);
            lgao_close_renderer (pAudioRen->ao);
            lgao_open_renderer (pAudioRen->ao);
            lgao_config (pAudioRen->ao);
            lgao_connect_renderer (pAudioRen->ao);
            lgao_play (pAudioRen->ao);
            lgao_start_renderer (pAudioRen->ao);
            SINK_LOG (3, GST_LEVEL_INFO, 2392, pAudioRen,
                      "Restart renderer - audio track changed(decodebin3)");
        }
        g_free (pAudioRen->pStreamId);
        pAudioRen->pStreamId = g_strdup (stream_id);
        break;
    }

    case GST_EVENT_CUSTOM_DOWNSTREAM: {
        const GstStructure *s;
        GstClockTime before = GST_CLOCK_TIME_NONE, after = GST_CLOCK_TIME_NONE;

        SINK_LOG (2, GST_LEVEL_INFO, 2404, pAudioRen, "got event(%s)", GST_EVENT_TYPE_NAME (event));
        s = gst_event_get_structure (event);
        if (s == NULL || !gst_structure_has_name (s, "audio-gap"))
            break;
        if (gst_structure_has_field (s, "pts-before-gap"))
            gst_structure_get_uint64 (s, "pts-before-gap", &before);
        else
            SINK_LOG (2, GST_LEVEL_INFO, 2418, pAudioRen, "no pts-before-gap field on structure");
        if (gst_structure_has_field (s, "pts-after-gap"))
            gst_structure_get_uint64 (s, "pts-after-gap", &after);
        else
            SINK_LOG (2, GST_LEVEL_INFO, 2424, pAudioRen, "no pts-after-gap field on structure");
        if (GST_CLOCK_TIME_IS_VALID (before) && GST_CLOCK_TIME_IS_VALID (after)) {
            SINK_LOG (2, GST_LEVEL_INFO, 2428, pAudioRen,
                      "audio-gap custom event received, before %lld, after %lld",
                      (gint64) before, (gint64) after);
            lgao_audio_gap (pAudioRen->ao, before, after);
        }
        break;
    }

    case GST_EVENT_CUSTOM_DOWNSTREAM_OOB:
        /* the sink's own upstream request comes back here: swallow it */
        if (gst_event_has_name (event, "mpegh-object-ui")) {
            SINK_LOG (3, GST_LEVEL_INFO, 2441, pAudioRen,
                      "received mpegh-object-ui event from upstream");
            gst_event_unref (event);
            return TRUE;
        }
        break;

    default:
        break;
    }

    return GST_BASE_SINK_CLASS (parent_class)->event (sink, event);
}

/* LG buffer flags (GST_BUFFER_FLAG_LAST = 0x100000 and up), set by LG demuxers/decoders */
#define LG_BUFFER_FLAG_METADATA      (GST_BUFFER_FLAG_LAST << 0)    /* 0x100000 */
#define LG_BUFFER_FLAG_ES            (GST_BUFFER_FLAG_LAST << 1)    /* 0x200000: IEC 61937 / ES */
#define LG_BUFFER_FLAG_OBJECT_UI     (GST_BUFFER_FLAG_LAST << 2)    /* 0x400000: MPEG-H UI changed */
#define LG_BUFFER_FLAG_METADATA2     (GST_BUFFER_FLAG_LAST << 3)    /* 0x800000 */
#define LG_BUFFER_FLAG_DROP_RENDERER (GST_BUFFER_FLAG_LAST << 4)    /* 0x1000000 */

/* LG custom upstream event type (num 270, GST_EVENT_TYPE_UPSTREAM) */
#define LG_EVENT_MPEGH_OBJECT_UI     ((GstEventType) 0x10e01)

#define CHAIN_MAX_FEED_SIZE 0x10000

static const char *
out_type_name (LGAO_DATA_TYPE_T outType)
{
    return outType == LGAO_DATA_PCM ? "PCM" : outType == LGAO_DATA_ES ? "ES" : "META";
}

/* gst_lg_audio_render_log_timestamp (gstlgaudiosink.c:~335): always inlined into
 * chain, no symbol. "Lip" log of every fed chunk; 90 kHz PTS rescaled by playspeed. */
static inline void
gst_lg_audio_render_log_timestamp (GstLgAudioRender *pAudioRen, GstBuffer *buffer,
                                   LGAO_DATA_TYPE_T outType, guint len)
{
    static guint index[LGAO_DATA_TYPE_NUM];
    GstClockTime ts = GST_BUFFER_PTS (buffer);
    gint mode_mixer = 0, port_index = -1, is_first = 1, playspeed;

    SINK_CHECK (lgao_get_mode_mixer (pAudioRen->ao, &mode_mixer), 341);
    SINK_CHECK (lgao_get_first_render (pAudioRen->ao, outType, &is_first), 343);
    if (is_first == 1)
        index[outType] = 0;
    SINK_CHECK (lgao_get_port (pAudioRen->ao, &port_index), 349);

    if (!GST_CLOCK_TIME_IS_VALID (ts)) {
        SINK_LOG (4, GST_LEVEL_LOG, 351, pAudioRen,
                  "Lip(%s.%02d-%04u): P=Invalid_TS, Len=%u, Out=%s",
                  mode_mixer == 1 ? "MIX" : "DEC", port_index, index[outType]++, len,
                  out_type_name (outType));
    } else {
        GstClockTime start_time = gst_element_get_start_time (GST_ELEMENT (pAudioRen));
        guint64 pts90k = ts * 9 / 100000;

        SINK_CHECK (lgao_get_playrate (pAudioRen->ao, &playspeed), 363);
        if (playspeed != 1000 && playspeed != 0)
            pts90k = (guint64) ((gfloat) (pts90k - start_time) * 1000.0f / playspeed) + start_time;
        SINK_LOG (4, GST_LEVEL_LOG, 375, pAudioRen,
                  "Lip(%s.%02d-%04u): P=%13llu ns( %12llu ), Len=%u, Out=%s",
                  mode_mixer == 1 ? "MIX" : "DEC", port_index, index[outType]++, ts, pts90k,
                  len, out_type_name (outType));
    }
}

/* @0xbbf4 gst_lg_audio_render_chain: GstBaseSink::render. Feeds the buffer to lgao in
 * <= 64 KiB sub-buffers; the data type (PCM / ES / metadata) comes from LG buffer flags. */
GstFlowReturn gst_lg_audio_render_chain (GstBaseSink *sink, GstBuffer *buffer)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (sink);
    gsize bufSize = gst_buffer_get_size (buffer);
    gint playspeed = 0, mode_nodelay = 0, mode_mute = 0, bufferedsize = 0;
    LGAO_DATA_TYPE_T outType;
    GstMapInfo bufMapInfo;
    guint flags;

    SINK_LOG (4, GST_LEVEL_LOG, 2490, pAudioRen, "called");
    SINK_CHECK (lgao_get_playrate (pAudioRen->ao, &playspeed), 2492);
    SINK_CHECK (lgao_get_nodelay (pAudioRen->ao, &mode_nodelay), 2493);
    SINK_CHECK (lgao_get_mute (pAudioRen->ao, &mode_mute), 2494);

    if (pAudioRen->bClockCalibrated) {
        GstElement *element = GST_ELEMENT (pAudioRen);
        GstClockTime now;
        gint64 start;

        SINK_LOG (2, GST_LEVEL_INFO, 2501, pAudioRen, "get calibrated clock. adjust basetime..");
        now = gst_clock_get_time (element->clock);
        if (sink->segment.rate > 0.0) {
            start = sink->segment.start;
            if (start == 0 && pAudioRen->segment_position != 0)
                start = pAudioRen->segment_start;
        } else {
            start = sink->segment.stop;
        }
        gst_lg_audio_render_update_clock (pAudioRen,
                                          now - element->base_time - GST_BASE_SINK (element)->segment.base,
                                          start);
        pAudioRen->bClockCalibrated = FALSE;
    }

    /* trick play (outside 0.25x..2x) and mute: drop silently */
    if ((guint) (playspeed - 250) > 1750 || mode_mute == 1)
        goto done;

    flags = GST_BUFFER_FLAGS (buffer);
    if (flags & GST_BUFFER_FLAG_CORRUPTED) {
        SINK_LOG (3, GST_LEVEL_INFO, 2528, pAudioRen, "This buffer is GST_BUFFER_FLAG_CORRUPTED");
        goto done;
    }
    if (flags & LG_BUFFER_FLAG_OBJECT_UI)
        pAudioRen->object_ui_changed = TRUE;

    if (flags & LG_BUFFER_FLAG_DROP_RENDERER) {
        /* still fed below; lgao discards it at the renderer (ao->drop_renderer) */
        SINK_LOG (4, GST_LEVEL_LOG, 2537, pAudioRen, "DROP!!!!! TS =%u:%02u:%02u.%09u\n",
                  GST_TIME_ARGS (GST_BUFFER_PTS (buffer)));
        lgao_set_drop_renderer (pAudioRen->ao, 1);
    } else {
        lgao_set_drop_renderer (pAudioRen->ao, 0);
    }

    if (pAudioRen->object_ui_changed) {
        GstEvent *ev = gst_event_new_custom (LG_EVENT_MPEGH_OBJECT_UI,
                                             gst_structure_new_empty ("mpegh-object-ui"));
        SINK_LOG (3, GST_LEVEL_INFO, 2549, pAudioRen, "push mpegh-object-ui event to upstream");
        gst_pad_push_event (sink->sinkpad, ev);
        pAudioRen->object_ui_changed = FALSE;
        if (pAudioRen->enable_dump)
            gst_lg_audio_render_dump_open (pAudioRen);
    }

    if (pAudioRen->enable_dump && pAudioRen->dump_file) {
        if (gst_buffer_map (buffer, &bufMapInfo, GST_MAP_READ)) {
            fwrite (bufMapInfo.data, 1, bufMapInfo.size, pAudioRen->dump_file);
            gst_buffer_unmap (buffer, &bufMapInfo);
        }
    }

    flags = GST_BUFFER_FLAGS (buffer);
    if (flags & LG_BUFFER_FLAG_ES)
        outType = LGAO_DATA_ES;
    else if (flags & LG_BUFFER_FLAG_METADATA)
        outType = LGAO_DATA_METADATA;
    else if (flags & LG_BUFFER_FLAG_METADATA2)
        outType = LGAO_DATA_METADATA2;
    else
        outType = LGAO_DATA_PCM;
    SINK_LOG (4, GST_LEVEL_LOG, 2574, pAudioRen, "outType = %d", outType);

    /* before the first byte is decoded, only render in PAUSED/PLAYING (no real wait) */
    if (pAudioRen->decodedSize == 0 && pAudioRen->currentState != GST_STATE_PLAYING) {
        SINK_LOG (5, GST_LEVEL_DEBUG, 2579, pAudioRen, "wait PLAYING state.(%s)",
                  gst_element_state_get_name (pAudioRen->currentState));
        if (pAudioRen->currentState <= GST_STATE_READY) {
            SINK_LOG (0, GST_LEVEL_ERROR, 2583, pAudioRen, "giveup wait...(%s)",
                      gst_element_state_get_name (pAudioRen->currentState));
            goto done;
        }
    }

    if (GST_BUFFER_PTS_IS_VALID (buffer)) {
        pAudioRen->currentPTS = GST_BUFFER_PTS (buffer);
        SINK_GST (GST_LEVEL_LOG, 2592, pAudioRen, "current_timestamp %u:%02u:%02u.%09u, %s:%d",
                  GST_TIME_ARGS (pAudioRen->currentPTS), __func__, 2594);
    }

    if (pAudioRen->isUnsupportedStream == TRUE) {
        SINK_LOG (5, GST_LEVEL_DEBUG, 2598, pAudioRen, "Unsupported Audio");
        pAudioRen->decodedSize += bufSize;
        goto done;
    }

    SINK_LOG (4, GST_LEVEL_LOG, 2605, pAudioRen, "map buffer");
    {
        gsize offset = 0, remain;

        while (bufSize != 0) {
            GstBuffer *sub;
            gsize len;

            /* [note] on copy failure the same chunk is retried and the remainder is lost */
            do {
                remain = 0;
                if (bufSize > CHAIN_MAX_FEED_SIZE) {
                    SINK_LOG (4, GST_LEVEL_LOG, 2609, pAudioRen, "bufSize = %d, limitSize = %d\n",
                              (gint) bufSize, CHAIN_MAX_FEED_SIZE);
                    remain = bufSize - CHAIN_MAX_FEED_SIZE;
                    bufSize = CHAIN_MAX_FEED_SIZE;
                }
                sub = gst_buffer_copy_region (buffer, GST_BUFFER_COPY_ALL, offset, bufSize);
                if (sub == NULL)
                    SINK_GST (GST_LEVEL_ERROR, 2619, pAudioRen, "gst_buffer_copy_region() Error");
            } while (sub == NULL);

            SINK_LOG (4, GST_LEVEL_LOG, 2621, pAudioRen,
                      "offset = %d, bufSize = %d, remainBufSize = %d\n",
                      (gint) offset, (gint) bufSize, (gint) remain);

            if (!gst_buffer_map (sub, &bufMapInfo, GST_MAP_READ)) {
                SINK_GST (GST_LEVEL_ERROR, 2625, pAudioRen, "gst_buffer_map() Error");
                len = bufSize;
            } else {
                len = bufMapInfo.size;
                gst_lg_audio_render_log_timestamp (pAudioRen, buffer, outType, len);
                lgao_get_size_buffered (pAudioRen->ao, outType, &bufferedsize);
                /* every chunk carries the PTS of the whole input buffer */
                if (lgao_feed (pAudioRen->ao, bufMapInfo.data, len, GST_BUFFER_PTS (buffer),
                               outType) < 0)
                    SINK_LOG (0, GST_LEVEL_ERROR, 2637, pAudioRen, "lgao_feed() is FAILED!!!");
                SINK_LOG (4, GST_LEVEL_LOG, 2640, pAudioRen, "RENDER %" GST_PTR_FORMAT " out=%s",
                          sub, out_type_name (outType));
                gst_buffer_unmap (sub, &bufMapInfo);
            }
            offset += len;
            gst_buffer_unref (sub);
            bufSize = remain;
        }
    }

done:
    SINK_LOG (4, GST_LEVEL_LOG, 2658, pAudioRen, "done");
    return GST_FLOW_OK;
}

/* @0xd040 gst_lg_audio_render_stop: GstBaseSink::stop - kick and join the event task,
 * tear the renderer down and close lgao. */
gboolean gst_lg_audio_render_stop (GstBaseSink *sink)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (sink);
    lgao_mq_item kick;

    pAudioRen->decodedSize = 0;
    pAudioRen->currentPTS = 0;
    SINK_LOG (2, GST_LEVEL_INFO, 2696, pAudioRen, "trace");

    if (pAudioRen->pAudioCaps) {
        gst_caps_unref (pAudioRen->pAudioCaps);
        pAudioRen->pAudioCaps = NULL;
    }
    if (pAudioRen->pAppType) {
        g_free (pAudioRen->pAppType);
        pAudioRen->pAppType = NULL;
    }
    if (pAudioRen->clocktype) {
        g_free (pAudioRen->clocktype);
        pAudioRen->clocktype = NULL;
    }

    SINK_LOG (3, GST_LEVEL_INFO, 2711, pAudioRen, "stopping renderer & callback task");
    kick.mtype = LGAO_MSG_TYPE_EVENT;
    kick.event = LGAO_EVENT_CLOSE_IPC;
    SINK_LOG (3, GST_LEVEL_INFO, 2715, pAudioRen, "kicking locked task");
    msgsnd (pAudioRen->event_mq_id, &kick, sizeof (kick.event), 0);
    if (pAudioRen->task) {
        gst_task_stop (pAudioRen->task);
        gst_task_join (pAudioRen->task);
        gst_object_unref (pAudioRen->task);
        pAudioRen->task = NULL;
    }

    lgao_stop_renderer (pAudioRen->ao);
    lgao_disconnect_renderer (pAudioRen->ao);
    lgao_close_renderer (pAudioRen->ao);
    lgao_close (pAudioRen->ao);
    pAudioRen->ao = NULL;

    if (pAudioRen->pStreamId) {
        g_free (pAudioRen->pStreamId);
        pAudioRen->pStreamId = NULL;
    }
    if (pAudioRen->enable_dump) {
        if (pAudioRen->dump_file)
            fclose (pAudioRen->dump_file);
        pAudioRen->dump_file = NULL;
    }
    SINK_LOG (2, GST_LEVEL_INFO, 2741, pAudioRen, "stop");
    return TRUE;
}

/* @0xd2e4 gst_lg_audio_render_start: GstBaseSink::start - start the lgao event task */
gboolean gst_lg_audio_render_start (GstBaseSink *sink)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (sink);

    SINK_LOG (2, GST_LEVEL_INFO, 2668, pAudioRen, "trace");
    pAudioRen->event_mq_id = lgao_get_event_mq_id (pAudioRen->ao);
    if (pAudioRen->enable_dump)
        gst_lg_audio_render_dump_open (pAudioRen);

    if (pAudioRen->task == NULL) {
        pAudioRen->task = gst_task_new (gst_lg_audio_render_callback_task, pAudioRen, NULL);
        gst_task_set_lock (pAudioRen->task, &pAudioRen->task_mutex);
        pAudioRen->event_mq_running = TRUE;
        gst_task_start (pAudioRen->task);
    }
    return TRUE;
}

/* @0xd3d4 gst_lg_audio_render_query: GstElement::query (installed at element_class+0xa4,
 * so `sink` is really the GstElement). POSITION answers are only kept for normal
 * playspeeds (0.25x..2x); in trick play the query fails. */
gboolean gst_lg_audio_render_query (GstBaseSink *sink, GstQuery *query)
{
    GstElement *element = GST_ELEMENT (sink);
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (element);
    gint playspeed = 0;
    gboolean res;

    res = GST_ELEMENT_CLASS (parent_class)->query (element, query);
    if (res == TRUE && GST_QUERY_TYPE (query) == GST_QUERY_POSITION) {
        GstFormat format;
        gint64 curTimeFromBase = 0;
        gint64 cur;

        gst_query_parse_position (query, &format, &curTimeFromBase);
        if (format != GST_FORMAT_TIME)
            return res;

        cur = curTimeFromBase;
        SINK_CHECK (lgao_get_playrate (pAudioRen->ao, &playspeed), 555);
        if ((guint) (playspeed - 250) <= 1750) {
            gst_query_set_position (query, format, cur);
        } else {
            cur = -1;
            res = FALSE;
        }
        SINK_LOG (5, GST_LEVEL_LOG, 563, pAudioRen,
                  "POSITION = %u:%02u:%02u.%09u(base) --> %u:%02u:%02u.%09u(audiosink)",
                  GST_TIME_ARGS (curTimeFromBase), GST_TIME_ARGS (cur));
    }
    return res;
}
