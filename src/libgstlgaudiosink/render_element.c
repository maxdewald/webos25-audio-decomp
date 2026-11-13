#define LG_SINK_LOCAL static
/*
 * GstLgAudioRender ("audiosink" element of the lgaudiosink plugin): GType
 * boilerplate, instance/class init (property table + signals), finalize,
 * get/set_property, change_state and the plugin entry points.
 * Data flow (chain/event/preroll/query/clock/task) lives in render_element.c.
 *
 * RECONSTRUCTION from usr/lib/gstreamer-1.0/libgstlgaudiosink.so
 *   sha256 90dba3ef32704ce4096bf1e6045d2879d0c2192d507bc04d1e8e98437f420a43
 * Original source: gst-plugins-bsp/gstlgaudio/gstlgaudiosink.c (DWARF CU).
 * Evidence: build/decomp/libgstlgaudiosink.so.c, build/decomp/libgstlgaudiosink.so.strings.tsv.
 * The line argument of SINK_LOG*() is the __LINE__ of the original file.
 * Tags are raw ELF VAs (decomp.toml src_addr_bias = 0x10000).
 */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "render_element.h"
#include "lgao.h"

static GType gst_lg_audio_render_get_type_once (void) __attribute__ ((noinline));
static void gst_lg_audio_render_class_intern_init (gpointer klass);
GType gst_lg_audio_render_get_type (void);
void gst_lg_audio_render_dump_open (GstLgAudioRender *pAudioRen);
static const char *
out_type_name (LGAO_DATA_TYPE_T outType);
static inline void
gst_lg_audio_render_log_timestamp (GstLgAudioRender *pAudioRen, GstBuffer *buffer,
                                   LGAO_DATA_TYPE_T outType, guint len);
void gst_lg_audio_render_callback_task (void *data);
GstClock *gst_lg_audio_render_provide_clock (GstElement *element);
gboolean gst_lg_audio_render_query (GstBaseSink *sink, GstQuery *query);
void gst_lg_audio_render_update_clock (GstLgAudioRender *pAudioRen, gint64 base, gint64 start);
void gst_lg_audio_render_update_ts_offset (GstLgAudioRender *pAudioRen, gint playspeed);
gboolean gst_lg_audio_render_prepare_seamless_seek (GstBaseSink *sink, GstSegment *segment, GstClockTime new_base_time);
gboolean gst_lg_audio_render_prepared_instant_rate_change (GstBaseSink *sink, gdouble rate_multiplier);
static void gst_lg_audio_render_finalize (GObject *obj);
void gst_lg_audio_render_clock_calibrated (GstLgAudioRender *pAudioRen);
static void
gst_lg_audio_render_class_init (GstLgAudioRenderClass *klass);
static void gst_lg_audio_render_init (GstLgAudioRender *pAudioRen);
static inline gboolean
gst_lg_audio_render_set_caps (GstLgAudioRender *pAudioRen, GstCaps *caps, gboolean *caps_changed);
static gboolean
is_id_gapless (GstLgAudioRender *pAudioRen);
static void gst_lg_audio_render_set_property (GObject *object, guint prop_id,
                                       const GValue *value, GParamSpec *pspec);
static void gst_lg_audio_render_get_property (GObject *object, guint prop_id,
                                       GValue *value, GParamSpec *pspec);
gboolean gst_lg_audio_render_event (GstBaseSink *sink, GstEvent *event);
GstFlowReturn gst_lg_audio_render_chain (GstBaseSink *sink, GstBuffer *buffer);
gboolean gst_lg_audio_render_start (GstBaseSink *sink);
gboolean gst_lg_audio_render_stop (GstBaseSink *sink);
static void
_gst_lg_audio_render_set_preconditions (GstLgAudioRender *pAudioRen);
static GstStateChangeReturn gst_lg_audio_render_change_state (GstElement *element,
                                                       GstStateChange transition);
GstFlowReturn gst_lg_audio_render_preroll (GstBaseSink *sink, GstBuffer *buffer);
gboolean gst_lg_audio_render_plugin_init (GstPlugin *plugin);
const GstPluginDesc *gst_plugin_lgaudiosink_get_desc (void);
void gst_plugin_lgaudiosink_register (void);

/* ---- shared element globals (declared extern in render_element.h) ------ */
struct logmm_module logmm_module_media_event = { 0, 0, 7, &logmm_dummy_mask, "media_event" };
GstDebugCategory   *gst_lg_audio_render_debug;
gpointer            parent_class;
guint               gst_lg_audio_render_signals[LAST_SIGNAL];
static gint         GstLgAudioRender_private_offset;

extern void gst_lxlog_init (void);

static void         gst_lg_audio_render_finalize (GObject *obj);
static void         gst_lg_audio_render_set_property (GObject *object, guint prop_id,
                                               const GValue *value, GParamSpec *pspec);
static void         gst_lg_audio_render_get_property (GObject *object, guint prop_id,
                                               GValue *value, GParamSpec *pspec);
static GstStateChangeReturn gst_lg_audio_render_change_state (GstElement *element,
                                                       GstStateChange transition);

static GType gst_lg_audio_render_get_type_once (void);
static void  gst_lg_audio_render_init (GstLgAudioRender *pAudioRen);
static void  gst_lg_audio_render_class_intern_init (gpointer klass);

static GstStaticPadTemplate sinktemplate = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK, GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw, "
        "format = (string) { S8, U8, S16LE, S16BE, U16LE, U16BE, S24LE, S24BE, "
        "U24LE, U24BE, S32LE, S32BE, U32LE, U32BE }, "
        "layout = (string) interleaved, "
        "rate = (int) [ 1, 384000 ], channels = (int) [ 1, 10 ]; "));

#define RW_STATIC  (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)   /* 0xe3 */
#define RO_STATIC  (G_PARAM_READABLE  | G_PARAM_STATIC_STRINGS)   /* 0xe1 */
#define WO_STATIC  (G_PARAM_WRITABLE  | G_PARAM_STATIC_STRINGS)   /* 0xe2 */
#define AUDIOSINK_VERSION 20180329u                                /* 0x133ed69 */

/* inlined into class_intern_init in the binary (no symbol) */

extern void gst_lxlog_deinit (void);

#define PREOPEN_BIT(prop_id)  (G_GUINT64_CONSTANT (1) << ((prop_id) - 1))

/*
 * Inlined into change_state (NULL_TO_READY), no symbol; name from the
 * logm __func__ string. Replays every marked property in ascending property ID;
 * a bit means a setter failed (or was skipped), not that replay succeeded.
 * Setter results are ignored and the mask is NOT cleared here. Later opens can
 * therefore replay old marked values, including both PCM transport flags; a
 * failed parent transition does not roll this replay/context creation back.
 */

/* The logmm mask branch is unhinted; GStreamer diagnostics are unlikely. */
#define PROP_LOGM(mlvl, line, fmt, ...) do { \
        if (*logmm_module_media_event.mask & (1u << (mlvl))) \
            logmm_printl (&logmm_module_media_event, (mlvl), __func__, (line), \
                          "<audio_sink> " fmt, ##__VA_ARGS__); \
    } while (0)
#define PROP_LOG(mlvl, glvl, line, obj, fmt, ...) do { \
        PROP_LOGM (mlvl, line, fmt, ##__VA_ARGS__); \
        SINK_GST (glvl, line, obj, fmt, ##__VA_ARGS__); \
    } while (0)

/*
 * Remember that an lgao setter failed (normally because ao == NULL before
 * NULL_TO_READY) so change_state can replay it.  The original shifts a plain
 * int, so bit 31 (variable-speed-playback) sign-extends into the high word.
 */
#define PREOPEN_FAILED(self, prop_id) \
    ((self)->preopen_conditions |= (guint64) (gint64) (1 << ((prop_id) - 1)))

/* lgao call whose failure marks the property for replay after lgao_open() */
#define SET_OR_DEFER(self, call, prop_id, line) do { \
        if ((call) != 0) { \
            SINK_GST (GST_LEVEL_WARNING, line, NULL, #call " failed!"); \
            PREOPEN_FAILED (self, prop_id); \
        } \
    } while (0)

#define TF(b)  ((b) ? "TRUE" : "FALSE")

/* Expansion of GST_PLUGIN_DEFINE (static build) against LG's GStreamer 1.24. */
#define LG_GST_VERSION_MINOR 24
static const GstPluginDesc gst_plugin_desc = {
    GST_VERSION_MAJOR, LG_GST_VERSION_MINOR, "lgaudiosink", "LG's audio sink element",
    gst_lg_audio_render_plugin_init, "1.0", "Proprietary", "gst-plugins-bsp",
    "LG Media Framework", "www.lge.com", NULL, { NULL }
};

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

/* The logmm mask branch is unhinted; GStreamer diagnostics are unlikely. */
#define RENDER_LOGM(mlvl, line, fmt, ...) do { \
        if (*logmm_module_media_event.mask & (1u << (mlvl))) \
            logmm_printl (&logmm_module_media_event, (mlvl), __func__, (line), \
                          "<audio_sink> " fmt, ##__VA_ARGS__); \
    } while (0)
#define RENDER_LOG(mlvl, glvl, line, obj, fmt, ...) do { \
        RENDER_LOGM (mlvl, line, fmt, ##__VA_ARGS__); \
        SINK_GST (glvl, line, obj, fmt, ##__VA_ARGS__); \
    } while (0)

#define CAPS_LOG_MAX_LEN 0x400

/* gst_lg_audio_render_set_caps (gstlgaudiosink.c:~1130): always inlined into event(),
 * no symbol. Parses the caps into LGAO_AUDIO_START_INFO_T and (re)opens the renderer.
 * Returns FALSE only for caps without structure/name or a non-audio mime.
 * Shipped Thumb: 0x88f6/0x8952 BYPASS closes via drop before returning;
 * 0x915c..0x91e6 requires format but unknown/float formats continue with zero
 * PCM fields. 0xa60a..0xa620 dispatches compare {-1,0,1,2}; only 1 tears down
 * (0xa8ca..0xa8da), -1 checks port (0xa956), all other values open without
 * teardown. 0xa8e2 suppresses open while dropped, not the final info/FIRST.
 * Caps are copied before validation; acceptance is not proof of configuration. */

/* LG buffer flags (GST_BUFFER_FLAG_LAST = 0x100000 and up), set by LG demuxers/decoders */
#define LG_BUFFER_FLAG_METADATA      (GST_BUFFER_FLAG_LAST << 0)    /* 0x100000 */
#define LG_BUFFER_FLAG_ES            (GST_BUFFER_FLAG_LAST << 1)    /* 0x200000: IEC 61937 / ES */
#define LG_BUFFER_FLAG_OBJECT_UI     (GST_BUFFER_FLAG_LAST << 2)    /* 0x400000: MPEG-H UI changed */
#define LG_BUFFER_FLAG_METADATA2     (GST_BUFFER_FLAG_LAST << 3)    /* 0x800000 */
#define LG_BUFFER_FLAG_DROP_RENDERER (GST_BUFFER_FLAG_LAST << 4)    /* 0x1000000 */

/* LG custom upstream event type (num 270, GST_EVENT_TYPE_UPSTREAM) */
#define LG_EVENT_MPEGH_OBJECT_UI     ((GstEventType) 0x10e01)

#define CHAIN_MAX_FEED_SIZE 0x10000

/* gst_lg_audio_render_log_timestamp (gstlgaudiosink.c:~335): always inlined into
 * chain, no symbol. "Lip" log of every fed chunk; 90 kHz PTS rescaled by playspeed. */



/* @0x4410 gst_lg_audio_render_get_type_once */
static GType gst_lg_audio_render_get_type_once (void)
{
    GType type = g_type_register_static_simple (GST_TYPE_BASE_SINK,
            g_intern_static_string ("GstLgAudioRender"),
            sizeof (GstLgAudioRenderClass),
            (GClassInitFunc) gst_lg_audio_render_class_intern_init,
            sizeof (GstLgAudioRender),
            (GInstanceInitFunc) gst_lg_audio_render_init, 0);

    if (gst_lg_audio_render_debug == NULL)
        gst_lg_audio_render_debug = _gst_debug_category_new ("lgaudiosink", 5,
                                                             "lgaudio_sink element");
    return type;
}

/* @0x46b0 gst_lg_audio_render_class_intern_init (G_DEFINE_TYPE wrapper) */
static void gst_lg_audio_render_class_intern_init (gpointer klass)
{
    parent_class = g_type_class_peek_parent (klass);
    if (GstLgAudioRender_private_offset != 0)
        g_type_class_adjust_private_offset (klass, &GstLgAudioRender_private_offset);
    gst_lg_audio_render_class_init ((GstLgAudioRenderClass *) klass);
}

/* @0x7590 gst_lg_audio_render_get_type */
GType gst_lg_audio_render_get_type (void)
{
    static gsize static_g_define_type_id = 0;

    if (g_once_init_enter (&static_g_define_type_id)) {
        GType id = gst_lg_audio_render_get_type_once ();
        g_once_init_leave (&static_g_define_type_id, id);
    }
    return static_g_define_type_id;
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

static const char *
out_type_name (LGAO_DATA_TYPE_T outType)
{
    return outType == LGAO_DATA_PCM ? "PCM" : outType == LGAO_DATA_ES ? "ES" : "META";
}

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
        SINK_LOG (4, GST_LEVEL_DEBUG, 351, pAudioRen,
                  "Lip(%s.%02d-%04u): P=Invalid_TS, Len=%u, Out=%s",
                  mode_mixer == 1 ? "MIX" : "DEC", port_index, index[outType]++, len,
                  out_type_name (outType));
    } else {
        GstClockTime start_time = gst_element_get_start_time (GST_ELEMENT (pAudioRen));
        guint64 pts90k = ts * 9 / 100000;

        SINK_CHECK (lgao_get_playrate (pAudioRen->ao, &playspeed), 363);
        if (playspeed != 1000 && playspeed != 0)
            pts90k = (guint64) ((gfloat) (pts90k - start_time) * 1000.0f / playspeed) + start_time;
        SINK_LOG (4, GST_LEVEL_DEBUG, 375, pAudioRen,
                  "Lip(%s.%02d-%04u): P=%13llu ns( %12llu ), Len=%u, Out=%s",
                  mode_mixer == 1 ? "MIX" : "DEC", port_index, index[outType]++, ts, pts90k,
                  len, out_type_name (outType));
    }
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
        RENDER_LOG (0, GST_LEVEL_ERROR, 392, NULL, "pAudioRen is NULL");
        return;
    }
    if (!pAudioRen->event_mq_running) {
        usleep (0);
        return;
    }
    if (msgrcv (pAudioRen->event_mq_id, &msg, sizeof (msg.event), 0, 0) <= 0) {
        RENDER_LOG (0, GST_LEVEL_ERROR, 403, pAudioRen, "mq error");
        return;
    }
    eventMsg = msg.event;
    RENDER_LOG (4, GST_LEVEL_DEBUG, 408, pAudioRen, "Got Event (0x%x) from Renderer", eventMsg);

    if (eventMsg & LGAO_EVENT_CLOSE_IPC) {
        /* stop() kicks the blocked msgrcv() with this */
        RENDER_LOG (3, GST_LEVEL_INFO, 411, pAudioRen, "Got LGAO_EVENT_CLOSE_IPC event. kicked!");
        pAudioRen->event_mq_running = FALSE;
        return;
    }
    if (eventMsg & LGAO_EVENT_PRESENT_END) {
        /* wakes the EOS wait in event() */
        RENDER_LOG (3, GST_LEVEL_INFO, 417, pAudioRen, "Got LGAO_EVENT_PRESENT_END event");
        g_mutex_lock (&pAudioRen->eos_mutex);
        g_cond_signal (&pAudioRen->eos_cond);
        g_mutex_unlock (&pAudioRen->eos_mutex);
    }
    if (eventMsg & LGAO_EVENT_UNDERFLOW) {
        RENDER_LOG (3, GST_LEVEL_INFO, 424, pAudioRen, "Got LGAO_EVENT_UNDERFLOW event");
        SINK_CHECK (lgao_get_state (pAudioRen->ao, &rendererState), 426);
        if (rendererState == LGAO_STATE_PLAY && !pAudioRen->isSeamless) {
            RENDER_LOG (2, GST_LEVEL_INFO, 430, pAudioRen, "Send underflow signal to upper layer");
            g_signal_emit (pAudioRen, gst_lg_audio_render_signals[SIGNAL_UNDERRUN], 0);
        } else {
            RENDER_LOG (4, GST_LEVEL_DEBUG, 433, pAudioRen,
                      "not send underflow signal(state=%d, seamless=%d)\n",
                      rendererState, pAudioRen->isSeamless);
        }
    }
    if (eventMsg & LGAO_EVENT_BYPASS_DONE) {
        RENDER_LOG (2, GST_LEVEL_INFO, 440, pAudioRen, "Got LGAO_EVENT_BYPASS_DONE event, send signal..");
        g_signal_emit_by_name (GST_ELEMENT (pAudioRen), "es-bypass-done");
    }
}

/* @0x74d8 gst_lg_audio_render_provide_clock: LG hardware clock (libgstlgclock) */
GstClock *gst_lg_audio_render_provide_clock (GstElement *element)
{
    return GST_CLOCK (gst_lgclock_new (element));
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

/* @0x5158 gst_lg_audio_render_update_clock: program renderer STC/PTS base.
 * Tail-calls lgao_set_basetime@plt via the Thumb veneer at 0x3c14 (raw 0x5902). */
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

/* @0x4f60 gst_lg_audio_render_update_ts_offset: basesink ts-offset from playspeed
 * and lipsync offset. Normal speeds (0.25x..0.999x) get a fixed -475 s / rate. */
void gst_lg_audio_render_update_ts_offset (GstLgAudioRender *pAudioRen, gint playspeed)
{
    GstClockTimeDiff ts_offset;

    if (G_LIKELY ((guint) (playspeed - 250) < 750)) {
        RENDER_LOG (3, GST_LEVEL_INFO, 600, pAudioRen, "Set TS Offset to %lld",
                    -475000000000LL / playspeed);
        gst_base_sink_set_ts_offset (GST_BASE_SINK (pAudioRen),
                                     -475000000000LL / playspeed);
        return;
    }

    if (pAudioRen->lipsync_offset >= 400)
        ts_offset = 0;
    else if (pAudioRen->pAppType != NULL && strcmp (pAudioRen->pAppType, "ID-GAPLESS") == 0)
        ts_offset = -400 * GST_MSECOND;
    else
        ts_offset = (GstClockTimeDiff) (400 - pAudioRen->lipsync_offset) * -GST_MSECOND;

    RENDER_LOG (3, GST_LEVEL_INFO, 615, pAudioRen, "Set TS Offset to %lld", ts_offset);
    gst_base_sink_set_ts_offset (GST_BASE_SINK (pAudioRen), ts_offset);
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

    SINK_LOG (4, GST_LEVEL_DEBUG, 638, pAudioRen,
              "rate: %f, segment.start: %u:%02u:%02u.%09u, pts: %u:%02u:%02u.%09u, "
              "diff: %c%u:%02u:%02u.%09u", segment->rate, GST_TIME_ARGS (segment->start),
              GST_TIME_ARGS (pAudioRen->currentPTS),
              GST_STIME_ARGS (pAudioRen->currentPTS - segment->start));
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

/* @0x5a08 gst_lg_audio_render_prepared_instant_rate_change: LG GstBaseSinkClass vfunc,
 * called after basesink applied an instant-rate-change segment.rate. With a video port
 * the renderer clock is re-based on the video decoder's current PTS (90 kHz). */
gboolean gst_lg_audio_render_prepared_instant_rate_change (GstBaseSink *sink, gdouble rate_multiplier)
{
    GstSegment *segment = &sink->segment;
    GstLgAudioRender *pAudioRen = (GstLgAudioRender *) sink;
    gdouble rate = segment->rate;
    GstClockTime now = gst_clock_get_time (GST_ELEMENT (pAudioRen)->clock);
    GstClockTime base = now - pAudioRen->lipsync_offset * 1000000;
    guint pts;

    (void) rate_multiplier;     /* segment.rate is already the new rate */
    pAudioRen->instant_rate_change = TRUE;
    pAudioRen->avail_irc_vdec_pts = TRUE;
    SINK_LOG (3, GST_LEVEL_INFO, 695, pAudioRen, "Instant rate change");

    if (pAudioRen->video_port != -1) {
        pts = 0;
        lgao_get_vdec_current_pts (pAudioRen->ao, pAudioRen->video_port, &pts);
        pAudioRen->irc_pts = pts;
        pAudioRen->irc_pts = pAudioRen->irc_pts / 9 * 100000;

        SINK_LOG (3, GST_LEVEL_INFO, 704, pAudioRen,
                  "instant rate : %f, now : %u:%02u:%02u.%09u, base :%u:%02u:%02u.%09u, "
                  "irc_pts : %u:%02u:%02u.%09u, current timestamp : %u:%02u:%02u.%09u", rate,
                  GST_TIME_ARGS (now), GST_TIME_ARGS (base), GST_TIME_ARGS (pAudioRen->irc_pts),
                  GST_TIME_ARGS (pAudioRen->currentPTS));
        {
            guint64 rate_bits;
            memcpy (&rate_bits, &rate, sizeof (rate_bits));
            SINK_LOG (3, GST_LEVEL_INFO, 711, pAudioRen, "irc_pts = %llu \n", rate_bits);
        }

        if (pAudioRen->irc_pts != 0 && pAudioRen->irc_pts != pAudioRen->currentPTS) {
            SINK_LOG (3, GST_LEVEL_INFO, 715, pAudioRen,
                      "basetime update.... irc_pts : %u:%02u:%02u.%09u, cur_pts : %u:%02u:%02u.%09u",
                      GST_TIME_ARGS (pAudioRen->irc_pts), GST_TIME_ARGS (pAudioRen->currentPTS));
            SINK_LOG (3, GST_LEVEL_INFO, 720, pAudioRen,
                      "update clock basetime : %u:%02u:%02u.%09u, lipsync_offset : "
                      "%u:%02u:%02u.%09u, cur_pts : %u:%02u:%02u.%09u", GST_TIME_ARGS (base),
                      GST_TIME_ARGS (pAudioRen->lipsync_offset * 1000000),
                      GST_TIME_ARGS (pAudioRen->currentPTS));
            gst_lg_audio_render_update_clock (pAudioRen, now - base, pAudioRen->irc_pts);
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

/* @0x74f0 gst_lg_audio_render_finalize
 * Exact ARM body only clears eos_cond/eos_mutex/task_mutex and logs. It does
 * NOT close ao, join its event task, or chain to parent finalize. Teardown
 * must already have happened through BaseSink::stop; finalize is not a fallback
 * cancellation/lifetime barrier for an injected producer. */
static void gst_lg_audio_render_finalize (GObject *obj)
{
    GstLgAudioRender *pAudioRen = (GstLgAudioRender *) obj;

    g_cond_clear (&pAudioRen->eos_cond);
    g_mutex_clear (&pAudioRen->eos_mutex);
    g_rec_mutex_clear (&pAudioRen->task_mutex);
    SINK_LOG (3, GST_LEVEL_INFO, 0x2f2, obj, "finalized");
}

/* @0x4488 gst_lg_audio_render_clock_calibrated: class handler of "clock-calibrated";
 * the next chain() call re-programs the renderer base time. */
void gst_lg_audio_render_clock_calibrated (GstLgAudioRender *pAudioRen)
{
    RENDER_LOG (2, GST_LEVEL_INFO, 760, pAudioRen, "clock calibrated.");
    if (GST_STATE (pAudioRen) == GST_STATE_PLAYING)
        pAudioRen->bClockCalibrated = TRUE;
}

static void
gst_lg_audio_render_class_init (GstLgAudioRenderClass *klass)
{
    GObjectClass     *gobject_class  = G_OBJECT_CLASS (klass);
    GstElementClass  *element_class  = GST_ELEMENT_CLASS (klass);
    GstBaseSinkClass *basesink_class = GST_BASE_SINK_CLASS (klass);

    gobject_class->set_property = (GObjectSetPropertyFunc) gst_lg_audio_render_set_property;
    gobject_class->get_property = gst_lg_audio_render_get_property;
    gobject_class->finalize = GST_DEBUG_FUNCPTR (gst_lg_audio_render_finalize);

    /* "audio-underrun": emitted from the event task on LGAO_EVENT_UNDERRUN
     * unless the sink is "seamless"; "clock-calibrated" is an action signal
     * whose class handler is gst_lg_audio_render_clock_calibrated;
     * "es-bypass-done" has no id slot (emitted by name from the callback task).
     * All three signals return void and have zero payload arguments: handlers
     * identify the emitting sink, not a retained ao/appsrc submission capability.
     * Properties below are scalar/string/resource configuration, no ao accessor. */
    gst_lg_audio_render_signals[SIGNAL_UNDERRUN] =
        g_signal_new ("audio-underrun", G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (GstLgAudioRenderClass, underrun), NULL, NULL,
                      g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
    gst_lg_audio_render_signals[SIGNAL_CLOCK_CALIBRATED] =
        g_signal_new ("clock-calibrated", G_TYPE_FROM_CLASS (klass),
                      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                      G_STRUCT_OFFSET (GstLgAudioRenderClass, clock_calibrated), NULL, NULL,
                      g_cclosure_marshal_generic, G_TYPE_NONE, 0);
    g_signal_new ("es-bypass-done", G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_FIRST,
                  G_STRUCT_OFFSET (GstLgAudioRenderClass, es_bypass_done), NULL, NULL,
                  g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

    g_object_class_install_property (gobject_class, PROP_PLAY_SPEED,
        g_param_spec_int ("play-speed", "Play Speed", "Play Speed",
                          G_MININT, G_MAXINT, 1000, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_DECODED_SIZE,
        g_param_spec_uint64 ("decoded-size", "Decoded Data Length", "Decoded Data Length",
                             0, G_MAXUINT64, 0, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_UNDECODED_SIZE,
        g_param_spec_uint64 ("undecoded-size", "UnDecoded Data Length", "UnDecoded Data Length",
                             0, G_MAXUINT64, 0, RO_STATIC));
    g_object_class_install_property (gobject_class, PROP_CURRENT_PTS,
        g_param_spec_uint64 ("current-pts", "PTS", "Get current PTS",
                             0, G_MAXUINT64, 0, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_LIPSYNC_OFFSET,
        g_param_spec_uint ("lipsync-offset", "Lipsync offset", "Audio's lipsync offset (msec)",
                           0, G_MAXUINT, G_MAXUINT, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_DISABLE_GSTSYNC,
        g_param_spec_boolean ("disable-gstsync", "Disable Basesink's Sync",
                              "Disable Basesink's Sync", FALSE, G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_LIPSYNC,
        g_param_spec_boolean ("lipsync", "Set Lipsync Mode", "Set Lipsync Mode",
                              TRUE, G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_NODELAY,
        g_param_spec_boolean ("nodelay", "Set No Delay Mode", "Set No Delay Mode",
                              FALSE, G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_NODELAY_MODE,
        g_param_spec_uint ("nodelay-mode", "Nodelay Mode",
                           "Nodelay Mode (0: CONT, 1: SKIP, 2: HYBRID)",
                           0, G_MAXUINT, G_MAXUINT, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_NODELAY_SKIP_UP,
        g_param_spec_uint ("nodelay-skip", "Threshold of Skipping",
                           "Buffering Threshold of Skipping", 0, G_MAXUINT, G_MAXUINT, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_NODELAY_FAST_UP,
        g_param_spec_uint ("nodelay-fast", "Threshold of Fast", "Buffering Threshold of Fast",
                           0, G_MAXUINT, G_MAXUINT, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_NODELAY_RECOVER,
        g_param_spec_uint ("nodelay-recover", "Threshold of Normal",
                           "Buffering Threshold of Normal State",
                           0, G_MAXUINT, G_MAXUINT, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_NODELAY_FAST_FREQ,
        g_param_spec_uint ("nodelay-fast-freq", "Frequency of Fast", "Frequency of Fast State",
                           0, G_MAXUINT, G_MAXUINT, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_APP_TYPE,
        g_param_spec_string ("app-type", "Application Type", "Set application type",
                             NULL, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_RESOURCE_INFO,
        g_param_spec_boxed ("resource-info", "Resource Info", "Set the port of audio resource",
                            GST_TYPE_STRUCTURE, WO_STATIC));
    g_object_class_install_property (gobject_class, PROP_MIXER,
        g_param_spec_boolean ("mixer", "Set using mixer", "Set using mixer",
                              FALSE, G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_AUDIO_INDEX,
        g_param_spec_uint ("index", "Index of audio resource", "Index of audio resource",
                           0, 7, 0, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_AUDIO_ONLY,
        g_param_spec_boolean ("audio-only", "Audio Only Stream", "This stream has audio only",
                              FALSE, G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_SEAMLESS,
        g_param_spec_boolean ("seamless", "Seamless audio change", "Seamless audio change",
                              FALSE, G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_MUTE,
        g_param_spec_boolean ("mute", "Control sound mute", "Control sound mute",
                              FALSE, G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_VERSION,
        g_param_spec_uint ("version", "Version of audio sink", "Version of audio sink",
                           0, G_MAXUINT, AUDIOSINK_VERSION, RO_STATIC));
    g_object_class_install_property (gobject_class, PROP_FADE,
        g_param_spec_boxed ("fade", "Fade Structure", "Set the fade volume",
                            GST_TYPE_STRUCTURE, WO_STATIC));
    g_object_class_install_property (gobject_class, PROP_FADE_VOLUME,
        g_param_spec_int ("fade-volume", "Fade Volume", "get the fade volume",
                          0, 100, 100, RO_STATIC));
    g_object_class_install_property (gobject_class, PROP_CLOCK_TYPE,
        g_param_spec_string ("clock-source", "atsc3-wall-clock", "acquire the source of clock",
                             NULL, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_AUDIO_BE_DELAY,
        g_param_spec_uint ("audio-be-delay", "Audio Back-end Delay",
                           "Total delay from render to spk", 0, G_MAXUINT, 85,
                           G_PARAM_READABLE));
    g_object_class_install_property (gobject_class, PROP_DIRECTMEDIA_TOTAL_BUF_SIZE,
        g_param_spec_int ("directmedia-total-audio-buffer-size",
                          "Total Audio Buffer Size for Directmedia",
                          "Total Audio Buffer Size for Directmedia",
                          0, G_MAXINT, 0, RO_STATIC));
    g_object_class_install_property (gobject_class, PROP_DIRECTMEDIA_USED_BUF_SIZE,
        g_param_spec_int ("directmedia-used-audio-buffer-size",
                          "Used Audio Buffer Size for Directmedia",
                          "Used Audio Buffer Size for Directmedia",
                          0, G_MAXINT, 0, RO_STATIC));
    g_object_class_install_property (gobject_class, PROP_PCM_BYPASS,
        g_param_spec_boolean ("pcm-bypass", "pcm multi channel for google stadia (directmedia)",
                              "pcm multi channel for google stadia (directmedia)",
                              FALSE, G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_PCM_MULTI_CH,
        g_param_spec_boolean ("pcm-multi-ch", "pcm multi channel for netflix (directmedia)",
                              "pcm multi channel for netflix (directmedia)",
                              FALSE, G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_GAME_MODE,
        g_param_spec_boolean ("game-mode", "Game Mode", "Game Mode.(bypass fader)",
                              FALSE, G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_RENDER_BUFFER_THRESHOLD,
        g_param_spec_uint ("base-playback-threshold", "base-playback-threshold",
                           "render buffer threshold value in milliseconds for R2R",
                           0, 200, 0, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_QUEUE_MAX_LENGTH,
        g_param_spec_uint ("queue-max-length", "Queue Max Length", "Queue Max Length",
                           0, G_MAXUINT, 0x4000, RW_STATIC));
    g_object_class_install_property (gobject_class, PROP_VARIABLE_SPEED_PLAYBACK,
        g_param_spec_boolean ("variable-speed-playback", "Netflix Variable Speed Playback",
                              "Netflix Variable Speed Playback", FALSE, G_PARAM_READWRITE));

    gst_element_class_set_metadata (element_class, "audio sink", "Sink/Audio",
                                    "LG's audio sink element",
                                    "Jaeyoun Jung <jaey.jung@lge.com>");
    gst_element_class_add_pad_template (element_class,
                                        gst_static_pad_template_get (&sinktemplate));

    element_class->change_state = GST_DEBUG_FUNCPTR (gst_lg_audio_render_change_state);
    element_class->provide_clock = GST_DEBUG_FUNCPTR (gst_lg_audio_render_provide_clock);
    /* element-level query (GstElementClass::query), not GstBaseSinkClass::query */
    element_class->query =
        (gboolean (*) (GstElement *, GstQuery *)) GST_DEBUG_FUNCPTR (gst_lg_audio_render_query);
    basesink_class->start = GST_DEBUG_FUNCPTR (gst_lg_audio_render_start);
    basesink_class->stop = GST_DEBUG_FUNCPTR (gst_lg_audio_render_stop);
    basesink_class->render = GST_DEBUG_FUNCPTR (gst_lg_audio_render_chain);
    basesink_class->event = GST_DEBUG_FUNCPTR (gst_lg_audio_render_event);
    basesink_class->preroll = GST_DEBUG_FUNCPTR (gst_lg_audio_render_preroll);
    klass->clock_calibrated = GST_DEBUG_FUNCPTR (gst_lg_audio_render_clock_calibrated);
    klass->lg.prepared_instant_rate_change =
        GST_DEBUG_FUNCPTR (gst_lg_audio_render_prepared_instant_rate_change);
    klass->lg.prepare_seamless_seek = GST_DEBUG_FUNCPTR (gst_lg_audio_render_prepare_seamless_seek);
}

/* @0x451c gst_lg_audio_render_init */
static void gst_lg_audio_render_init (GstLgAudioRender *pAudioRen)
{
    GstBaseSink *basesink = GST_BASE_SINK (pAudioRen);

    gst_lxlog_init ();
    SINK_LOG (2, GST_LEVEL_INFO, 0x408, pAudioRen, "Compile info : %s %s",
              "Oct 16 2025", "05:55:01");

    pAudioRen->playrate = 0.0;
    pAudioRen->event_mq_id = -1;
    pAudioRen->event_mq_running = FALSE;
    pAudioRen->audio_port = -1;
    pAudioRen->video_port = -1;
    pAudioRen->mixer_port = -1;
    pAudioRen->ao = NULL;
    pAudioRen->isSeamless = FALSE;
    pAudioRen->decodedSize = 0;
    pAudioRen->currentPTS = 0;
    pAudioRen->disableBasesinkSync = FALSE;
    pAudioRen->isUnsupportedStream = FALSE;
    pAudioRen->pAudioCaps = NULL;
    pAudioRen->task = NULL;
    pAudioRen->pStreamId = NULL;
    pAudioRen->isFlushing = FALSE;
    pAudioRen->lipsync_offset = 400;
    pAudioRen->pAppType = NULL;
    pAudioRen->clocktype = NULL;
    pAudioRen->rts_mode = 6;
    pAudioRen->nodelay_mode = FALSE;
    pAudioRen->bClockCalibrated = FALSE;
    pAudioRen->segment_start = 0;
    pAudioRen->segment_position = 0;
    pAudioRen->base_delay = 0;
    pAudioRen->render_delay = 0;
    pAudioRen->enable_dump = FALSE;
    pAudioRen->dump_index = 0;
    pAudioRen->dump_file = NULL;
    pAudioRen->currentState = GST_STATE_NULL;
    pAudioRen->nextState = GST_STATE_NULL;
    pAudioRen->lipsync_mode = TRUE;      /* lipsync on by default */
    pAudioRen->preopen_conditions = 0;
    pAudioRen->instant_rate_change = FALSE;
    pAudioRen->irc_pts = 0;
    pAudioRen->avail_irc_vdec_pts = TRUE;
    pAudioRen->pcm_bypass = FALSE;
    pAudioRen->render_buf_thres = 0;

    if (access ("/build/scratch/gstlgaudiosink_dump_enable", F_OK) == 0)
        pAudioRen->enable_dump = TRUE;

    g_cond_init (&pAudioRen->eos_cond);
    g_mutex_init (&pAudioRen->eos_mutex);
    g_rec_mutex_init (&pAudioRen->task_mutex);

    /* 475 ms default render delay (renderer path latency) */
    gst_base_sink_set_render_delay (basesink, 475 * GST_MSECOND);
    pAudioRen->base_delay = 475;
    pAudioRen->render_delay = 475;
    GST_OBJECT_FLAG_SET (pAudioRen, GST_ELEMENT_FLAG_SINK);
}

static inline gboolean
gst_lg_audio_render_set_caps (GstLgAudioRender *pAudioRen, GstCaps *caps, gboolean *caps_changed)
{
    const GstStructure *structure;
    LGAO_AUDIO_START_INFO_T AudioInfo;
    const gchar *mime;
    gint getValue;
    guint acmod, check_downmix;
    gboolean lfe_enable, dolby, clock_x4, atmos, downmix;
    gint out_channels;
    gboolean restart;
    gint gap_correction;
    gboolean es = FALSE;
    gint data_channels = 0, data_rate = 0, data_bps = 0;
    const gchar *codec_str = NULL;
    gchar *str = NULL;
    const gchar *format = NULL;
    gint mode_mixer = 0, port_index = -1;
    guint curr_acmod = 0, curr_downmix = 0;
    guint codec;
    gboolean send_first_event = FALSE;
    gchar *stream_id = NULL;
    GstStructure *media_info = NULL;
    guint64 channel_mask;
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
    SINK_LOG (4, GST_LEVEL_DEBUG, 1167, pAudioRen, "mime = %s", mime);
    if (strstr (mime, "audio") == NULL) {
        SINK_LOG (0, GST_LEVEL_ERROR, 1170, pAudioRen, "not a audio data(mime=%s)", mime);
        return FALSE;
    }

    if (gst_structure_get_int (structure, "rate", &getValue)) {
        AudioInfo.freq = getValue;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1176, pAudioRen, "sampling-rate = %d", getValue);
    }
    if (gst_structure_get_int (structure, "channels", &getValue)) {
        AudioInfo.numberOfChannel = (unsigned char) getValue;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1181, pAudioRen, "channels = %d", getValue);
    }

    /* every accepted caps needs a "format" field, also ES (IEC 61937) caps */
    format = gst_structure_get_string (structure, "format");
    if (format != NULL) {
    SINK_LOG (4, GST_LEVEL_DEBUG, 1189, pAudioRen, "PCM format = %s", format);
    {
        GstAudioFormat fmt = gst_audio_format_from_string (format);
        const GstAudioFormatInfo *finfo;

        if (fmt != GST_AUDIO_FORMAT_UNKNOWN) {
            finfo = gst_audio_format_get_info (fmt);
            if (finfo != NULL) {
                if (GST_AUDIO_FORMAT_INFO_IS_INTEGER (finfo)) {
                    AudioInfo.bitsPerSample = (unsigned char) finfo->width;
                    SINK_LOG (4, GST_LEVEL_DEBUG, 1198, pAudioRen, "bitsPerSample = %d", AudioInfo.bitsPerSample);
                    AudioInfo.unsignedOfPcm = !GST_AUDIO_FORMAT_INFO_IS_SIGNED (finfo);
                    SINK_LOG (4, GST_LEVEL_DEBUG, 1204, pAudioRen, "unsigned = %d", AudioInfo.unsignedOfPcm);
                    AudioInfo.bigEndianOfPcm = finfo->endianness != G_LITTLE_ENDIAN;
                    SINK_LOG (4, GST_LEVEL_DEBUG, 1210, pAudioRen, "endian = %d", AudioInfo.bigEndianOfPcm);
                } else {
                    SINK_LOG (4, GST_LEVEL_DEBUG, 1212, pAudioRen, "Floating PCM");
                }
            } else {
                SINK_LOG (0, GST_LEVEL_ERROR, 1215, pAudioRen, "gst_audio_format_get_info() Err");
            }
        } else {
            SINK_LOG (0, GST_LEVEL_ERROR, 1218, pAudioRen,
                      "gst_audio_format_from_string () Err(GST_ADEC_FORMAT_UNKNOWN)");
        }
    }

    if (gst_structure_get_uint (structure, "acmod", &acmod)) {
        AudioInfo.acmod = acmod;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1228, pAudioRen, "acmod = %d", acmod);
    } else if (AudioInfo.numberOfChannel == 1) {
        AudioInfo.acmod = 1;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1232, pAudioRen, "channel = %d, acmod = %d",
                  AudioInfo.numberOfChannel, AudioInfo.acmod);
    } else if (gst_structure_get (structure, "channel-mask", GST_TYPE_BITMASK, &channel_mask, NULL)) {
        /* 0xaf4e..0xaf5e compares ONLY the low 32 bits of the 64-bit mask.
         * Preserve that shipped truncation (not a normal GStreamer mask map). */
        AudioInfo.acmod = (guint) channel_mask == 2 ? 1 : (guint) channel_mask == 1 ? 2 : 7;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1245, pAudioRen, "channel-mask = %d, acmod = %d",
                  (gint) channel_mask, AudioInfo.acmod);
    } else {
        SINK_LOG (4, GST_LEVEL_DEBUG, 1248, pAudioRen, "acmod or channel-mask field is empty");
    }

    if (gst_structure_get_uint (structure, "check-downmix", &check_downmix)) {
        AudioInfo.check_downmix = check_downmix;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1255, pAudioRen, "check-downmix = %d", check_downmix);
    } else {
        SINK_LOG (4, GST_LEVEL_DEBUG, 1257, pAudioRen, "check-downmix field is empty");
        AudioInfo.check_downmix = (unsigned int) -1;
    }
    if (gst_structure_get_boolean (structure, "lfe-enable", &lfe_enable)) {
        AudioInfo.lfe_enable = lfe_enable;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1263, pAudioRen, "lfe_enable = %d", lfe_enable);
    } else {
        SINK_LOG (4, GST_LEVEL_DEBUG, 1265, pAudioRen, "lfe_enable field is empty");
    }
    if (gst_structure_get_boolean (structure, "dolby", &dolby)) {
        AudioInfo.dolby = dolby;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1271, pAudioRen, "dolby = %d", dolby);
    } else {
        SINK_LOG (4, GST_LEVEL_DEBUG, 1273, pAudioRen, "dolby field is empty");
    }
    if (gst_structure_get_boolean (structure, "clock-x4", &clock_x4)) {
        AudioInfo.clock_x4 = clock_x4;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1279, pAudioRen, "clock_x4 (eac3) = %d", clock_x4);
    } else {
        SINK_LOG (4, GST_LEVEL_DEBUG, 1281, pAudioRen, "clock_x4 (eac3) field is empty");
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
        SINK_LOG (4, GST_LEVEL_DEBUG, 1321, pAudioRen, "out_channels = %d", out_channels);
    } else {
        SINK_LOG (4, GST_LEVEL_DEBUG, 1323, pAudioRen, "out-channels field is empty");
        AudioInfo.out_channels = -1;
    }
    if (gst_structure_get_boolean (structure, "restart", &restart)) {
        AudioInfo.restart = restart;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1329, pAudioRen, "restart = %d", restart);
    } else {
        SINK_LOG (4, GST_LEVEL_DEBUG, 1331, pAudioRen, "restart field is empty");
    }
    if (gst_structure_get_int (structure, "gap-correction", &gap_correction)) {
        AudioInfo.gap_correction = gap_correction;
        SINK_LOG (3, GST_LEVEL_INFO, 1352, pAudioRen, "gap_correction = %d", gap_correction);
    }
    if (gst_structure_get_uint (structure, "codec", &codec)) {
        AudioInfo.codecType = codec;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1357, pAudioRen, "codec = %d", codec);
    } else {
        SINK_LOG (4, GST_LEVEL_DEBUG, 1359, pAudioRen, "codec field is empty");
        AudioInfo.codecType = LGAO_CODEC_PCM;
    }

    /* passthrough (IEC 61937) description, consumed by lgao's bypass/ES path */
    if (gst_structure_get_boolean (structure, "es", &es)) {
        AudioInfo.es = es;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1365, pAudioRen, "es = %d", es);
    }
    if (gst_structure_get_int (structure, "data-channels", &data_channels)) {
        AudioInfo.data_channels = data_channels;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1370, pAudioRen, "data_channels = %d", data_channels);
    }
    if (gst_structure_get_int (structure, "data-rate", &data_rate)) {
        AudioInfo.data_rate = data_rate;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1375, pAudioRen, "data_rate = %d", data_rate);
    }
    if (gst_structure_get_int (structure, "data-bps", &data_bps)) {
        AudioInfo.data_bps = data_bps;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1380, pAudioRen, "data_bps = %d", data_bps);
    }
    codec_str = gst_structure_get_string (structure, "codec-str");
    if (codec_str) {
        /* fixed 16-byte copy (no strlen/strncpy in the binary) */
        memcpy (AudioInfo.codec_str, codec_str, sizeof (AudioInfo.codec_str));
        SINK_LOG (4, GST_LEVEL_DEBUG, 1385, pAudioRen, "codec-str = %s -> %s", codec_str,
                  AudioInfo.codec_str);
    }
    if (gst_structure_get_boolean (structure, "downmix", &downmix)) {
        AudioInfo.downmix = downmix;
        SINK_LOG (4, GST_LEVEL_DEBUG, 1391, pAudioRen, "downmix = %d", downmix);
    } else {
        SINK_LOG (4, GST_LEVEL_DEBUG, 1393, pAudioRen, "downmix field is empty");
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
    case 1:     /* first caps or format changed */
        SINK_LOG (2, GST_LEVEL_INFO, 1427, pAudioRen, "Caps is first or updated");
        lgao_stop_renderer (pAudioRen->ao);
        lgao_disconnect_renderer (pAudioRen->ao);
        lgao_close_renderer (pAudioRen->ao);
        break;
    case -1: {  /* nothing configured yet: only with a valid audio port */
        gint cur_port = -1;
        SINK_CHECK (lgao_get_port (pAudioRen->ao, &cur_port), 1439);
        if (cur_port < 0) {
            SINK_LOG (2, GST_LEVEL_INFO, 1445, pAudioRen, "audio port(%d) in invalid! ignore", cur_port);
            goto same_caps;
        }
        break;
    }
    case 0:     /* same PCM format, no restart */
        SINK_LOG (3, GST_LEVEL_INFO, 1453, pAudioRen, "same caps for normal play received. ignore");
        goto same_caps;
    case 2:     /* same PCM format, restart=TRUE: log only, NO connect call (0xa624) */
        SINK_LOG (2, GST_LEVEL_INFO, 1458, pAudioRen, "Connect ES Renderer Only");
        goto same_caps;
    default:    /* not produced by lgao_compare_audio_info: open without teardown */
        break;
    }

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

same_caps:
    /* always re-store the info; a changed acmod/downmix re-sends the first event */
    SINK_CHECK (lgao_get_audio_info (pAudioRen->ao, LGAO_AUDIO_INFO_DOWNMIX, &curr_downmix), 1491);
    SINK_CHECK (lgao_set_audio_info (pAudioRen->ao, AudioInfo), 1494);
    if (AudioInfo.acmod != curr_acmod) {
        SINK_LOG (3, GST_LEVEL_INFO, 1496, pAudioRen, "acmod changed %d -> %d", curr_acmod,
                  AudioInfo.acmod);
        send_first_event = TRUE;
    }
    if (AudioInfo.downmix != curr_downmix) {
        SINK_LOG (3, GST_LEVEL_INFO, 1501, pAudioRen, "downmix mode changed %d -> %d",
                  curr_downmix, AudioInfo.downmix);
        send_first_event = TRUE;
    }
    if (send_first_event)
        lgao_push_first_event (pAudioRen->ao);
    if (stream_id)
        g_free (stream_id);
    } else {
        SINK_LOG (0, GST_LEVEL_ERROR, 1222, pAudioRen, "PCM format is NULL");
    }
    return TRUE;
}

static gboolean
is_id_gapless (GstLgAudioRender *pAudioRen)
{
    return pAudioRen->pAppType != NULL && strcmp (pAudioRen->pAppType, "ID-GAPLESS") == 0;
}

/* @0x11138 gst_lg_audio_render_set_property */
static void gst_lg_audio_render_set_property (GObject *object, guint prop_id,
                                       const GValue *value, GParamSpec *pspec)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (object);
    gchar *contents;

    contents = g_strdup_value_contents (value);
    PROP_LOG (4, GST_LEVEL_DEBUG, 0x5fb, pAudioRen, "set_property \"%s\"(%d) = %s:'%s'",
              pspec->name, prop_id, g_type_name (G_VALUE_TYPE (value)), contents);
    g_free (contents);

    switch (prop_id) {
    case PROP_PLAY_SPEED:
        pAudioRen->play_speed = g_value_get_int (value);
        SET_OR_DEFER (pAudioRen, lgao_set_playrate (pAudioRen->ao, pAudioRen->play_speed),
                      PROP_PLAY_SPEED, 0x603);
        break;

    case PROP_DECODED_SIZE:
        pAudioRen->decodedSize = g_value_get_uint64 (value);
        break;

    case PROP_LIPSYNC_OFFSET:
        /* offset (ms) is applied twice: as GstBaseSink render delay
         * (offset + 75 ms) and, below 400 ms, as a negative ts-offset;
         * the renderer gets the raw value via lgao. */
        pAudioRen->lipsync_offset = g_value_get_uint (value);
        pAudioRen->base_delay = pAudioRen->lipsync_offset + 75;
        pAudioRen->render_delay = pAudioRen->lipsync_offset + 75;
        if (!is_id_gapless (pAudioRen))
            gst_base_sink_set_render_delay (GST_BASE_SINK (object),
                                            (gint64) pAudioRen->render_delay * GST_MSECOND);
        else
            gst_base_sink_set_render_delay (GST_BASE_SINK (pAudioRen), 0);
        if (pAudioRen->lipsync_offset < 400) {
            GstClockTimeDiff ts_offset = is_id_gapless (pAudioRen)
                ? -400 * GST_MSECOND
                : -(gint64) (400 - pAudioRen->lipsync_offset) * GST_MSECOND;
            gst_base_sink_set_ts_offset (GST_BASE_SINK (object), ts_offset);
        }
        PROP_LOG (3, GST_LEVEL_INFO, 0x620, pAudioRen, "Set Audio's lipsync offset to %u",
                  pAudioRen->lipsync_offset);
        SET_OR_DEFER (pAudioRen,
                      lgao_set_lipsync_offset (pAudioRen->ao, pAudioRen->lipsync_offset),
                      PROP_LIPSYNC_OFFSET, 0x622);
        break;

    case PROP_DISABLE_GSTSYNC:
        pAudioRen->disableBasesinkSync = g_value_get_boolean (value);
        if (pAudioRen->disableBasesinkSync) {
            PROP_LOG (3, GST_LEVEL_INFO, 0x629, pAudioRen, "Disable BasepAudioRen's Sync");
            gst_base_sink_set_sync (GST_BASE_SINK (pAudioRen), FALSE);
        }
        break;

    case PROP_LIPSYNC:
        /* Only stored in lgao (ao->lipsyncMode); see report: it is consumed
         * when lgao builds/starts the renderer. */
        pAudioRen->lipsync_mode = g_value_get_boolean (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x62f, pAudioRen, "Set Lipsync Mode to %s",
                  pAudioRen->lipsync_mode ? "ENABLE" : "DISABLE");
        SET_OR_DEFER (pAudioRen,
                      lgao_set_lipsync_mode (pAudioRen->ao, pAudioRen->lipsync_mode
                                             ? LGAO_LIPSYNC_ENABLE : LGAO_LIPSYNC_DISABLE),
                      PROP_LIPSYNC, 0x632);
        break;

    case PROP_NODELAY:
        pAudioRen->nodelay_mode = g_value_get_boolean (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x639, pAudioRen, "Set No Delay Mode to %s",
                  TF (pAudioRen->nodelay_mode));
        SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, pAudioRen->nodelay_mode),
                      PROP_NODELAY, 0x63e);
        break;

    case PROP_NODELAY_MODE:
        pAudioRen->rts_mode = g_value_get_uint (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x643, pAudioRen, "Set RTS OP Mode = %u",
                  pAudioRen->rts_mode);
        if (pAudioRen->rts_mode != 6)
            SET_OR_DEFER (pAudioRen,
                          lgao_set_rts (pAudioRen->ao, LGAO_RTS_OP_MODE, pAudioRen->rts_mode),
                          PROP_NODELAY_MODE, 0x646);
        break;

    case PROP_NODELAY_SKIP_UP:
        pAudioRen->nodelay_skip_up = g_value_get_uint (value);
        SET_OR_DEFER (pAudioRen, lgao_set_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_SKIPPING,
                                               pAudioRen->nodelay_skip_up),
                      PROP_NODELAY_SKIP_UP, 0x64d);
        PROP_LOG (3, GST_LEVEL_INFO, 0x64f, pAudioRen, "Set RTS Threshold of Skipping = %u",
                  pAudioRen->nodelay_skip_up);
        break;

    case PROP_NODELAY_FAST_UP:
        pAudioRen->nodelay_fast_up = g_value_get_uint (value);
        SET_OR_DEFER (pAudioRen, lgao_set_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_FAST,
                                               pAudioRen->nodelay_fast_up),
                      PROP_NODELAY_FAST_UP, 0x656);
        PROP_LOG (3, GST_LEVEL_INFO, 0x658, pAudioRen, "Set RTS Threshold of Fast = %u",
                  pAudioRen->nodelay_fast_up);
        break;

    case PROP_NODELAY_RECOVER:
        pAudioRen->nodelay_recover = g_value_get_uint (value);
        SET_OR_DEFER (pAudioRen, lgao_set_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_RECOVER,
                                               pAudioRen->nodelay_recover),
                      PROP_NODELAY_RECOVER, 0x65f);
        PROP_LOG (3, GST_LEVEL_INFO, 0x661, pAudioRen, "Set RTS Threshold of Recover = %u",
                  pAudioRen->nodelay_recover);
        break;

    case PROP_NODELAY_FAST_FREQ:
        pAudioRen->nodelay_fast_freq = g_value_get_uint (value);
        SET_OR_DEFER (pAudioRen, lgao_set_rts (pAudioRen->ao, LGAO_RTS_FREQ_FAST,
                                               pAudioRen->nodelay_fast_freq),
                      PROP_NODELAY_FAST_FREQ, 0x668);
        PROP_LOG (3, GST_LEVEL_INFO, 0x66a, pAudioRen, "Set RTS Fast Frequency of Fast = %u",
                  pAudioRen->nodelay_fast_freq);
        break;

    case PROP_RENDER_BUFFER_THRESHOLD:
        pAudioRen->render_buf_thres = g_value_get_uint (value);
        SET_OR_DEFER (pAudioRen, lgao_set_rts (pAudioRen->ao, LGAO_RTS_BUFFERING_LEVEL,
                                               pAudioRen->render_buf_thres),
                      PROP_RENDER_BUFFER_THRESHOLD, 0x671);
        PROP_LOG (3, GST_LEVEL_INFO, 0x673, pAudioRen, "Set PROP_RENDER_BUFFER_THRESHOLD = %u",
                  pAudioRen->render_buf_thres);
        break;

    case PROP_APP_TYPE: {
        const char *app;

        if (pAudioRen->pAppType) {
            g_free (pAudioRen->pAppType);
            pAudioRen->pAppType = NULL;
        }
        pAudioRen->pAppType = g_value_dup_string (value);
        app = pAudioRen->pAppType;
        if (app == NULL)
            goto app_done;

        /* low-latency sources (all except AIRPLAY) also drop basesink sync */
        if (strcmp (app, "AIRPLAY") == 0) {
            PROP_LOG (3, GST_LEVEL_INFO, 0x680, pAudioRen, "This application is Airplay Mirroring");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_AIRPLAY),
                          PROP_APP_TYPE, 0x681);
            SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, TRUE), PROP_APP_TYPE, 0x683);
            goto app_done;
        } else if (strcmp (app, "RTC") == 0) {
            PROP_LOG (3, GST_LEVEL_INFO, 0x688, pAudioRen, "This application is WiDi");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_RTC),
                          PROP_APP_TYPE, 0x689);
            SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, TRUE), PROP_APP_TYPE, 0x68b);
        } else if (strcmp (app, "R2R") == 0) {
            PROP_LOG (3, GST_LEVEL_INFO, 0x691, pAudioRen, "This application is R2R");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_R2R),
                          PROP_APP_TYPE, 0x692);
            SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, TRUE), PROP_APP_TYPE, 0x694);
        } else if (strcmp (app, "MIRACAST") == 0) {
            PROP_LOG (3, GST_LEVEL_INFO, 0x698, pAudioRen, "This application is Miracast");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_MIRACAST),
                          PROP_APP_TYPE, 0x699);
            SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, TRUE), PROP_APP_TYPE, 0x69b);
        } else if (strcmp (app, "JP4K") == 0) {
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_JP4K),
                          PROP_APP_TYPE, 0x69f);
            goto app_done;
        } else if (strcmp (app, "DIRECTMEDIA") == 0) {
            PROP_LOG (3, GST_LEVEL_INFO, 0x6a2, pAudioRen,
                      "This application is Cloud Game(DIRECTMEDIA)");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_DIRECTMEDIA),
                          PROP_APP_TYPE, 0x6a3);
            SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, TRUE), PROP_APP_TYPE, 0x6a5);
        } else if (strcmp (app, "ID-GAPLESS") == 0) {
            PROP_LOG (3, GST_LEVEL_INFO, 0x6a9, pAudioRen, "This application is for ID-GAPLESS PLAY");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_IDGAPLESS),
                          PROP_APP_TYPE, 0x6aa);
            goto app_done;
        } else if (strcmp (app, "APPCAST") == 0) {
            PROP_LOG (3, GST_LEVEL_INFO, 0x6ad, pAudioRen, "This application is for APPCAST");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_APPCAST),
                          PROP_APP_TYPE, 0x6ae);
            goto app_done;
        } else if (strcmp (app, "SYE") == 0) {
            PROP_LOG (3, GST_LEVEL_INFO, 0x6b1, pAudioRen, "This application is for AMAZON SYE");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_SYE),
                          PROP_APP_TYPE, 0x6b2);
        } else {
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_UNKNOWN),
                          PROP_APP_TYPE, 0x6b6);
            goto app_done;
        }
        gst_base_sink_set_sync (GST_BASE_SINK (pAudioRen), FALSE);
app_done:
        PROP_LOG (3, GST_LEVEL_INFO, 0x6ba, pAudioRen, "Apptype %s received", pAudioRen->pAppType);
        break;
    }

    case PROP_RESOURCE_INFO: {
        const GstStructure *st = gst_value_get_structure (value);

        if (st == NULL) {
            PROP_LOG (0, GST_LEVEL_ERROR, 0x6d9, pAudioRen,
                      "resource-info has not mixer-port field");
        } else {
            if (gst_structure_has_field (st, "audio-port")) {
                gst_structure_get_int (st, "audio-port", &pAudioRen->audio_port);
                PROP_LOG (5, GST_LEVEL_LOG, 0x6c9, pAudioRen, "audio-port is %d",
                          pAudioRen->audio_port);
            }
            if (gst_structure_has_field (st, "video-port")) {
                gst_structure_get_int (st, "video-port", &pAudioRen->video_port);
                PROP_LOG (2, GST_LEVEL_INFO, 0x6d0, pAudioRen, "video-port is %d",
                          pAudioRen->video_port);
            }
            if (gst_structure_has_field (st, "mixer-port")) {
                gst_structure_get_int (st, "mixer-port", &pAudioRen->mixer_port);
                PROP_LOG (5, GST_LEVEL_LOG, 0x6d6, pAudioRen, "mixer-port is %d",
                          pAudioRen->mixer_port);
            }
        }
        PROP_LOG (3, GST_LEVEL_INFO, 0x6db, pAudioRen, "audio-port %d, video-port %d,mixer-port %d",
                  pAudioRen->audio_port, pAudioRen->video_port, pAudioRen->mixer_port);
        /* Ports configure the logical index; aren0 + SET_INDEX selects the slot. */
        SET_OR_DEFER (pAudioRen, lgao_set_port (pAudioRen->ao, pAudioRen->audio_port,
                                                pAudioRen->mixer_port),
                      PROP_RESOURCE_INFO, 0x6dd);
        break;
    }

    case PROP_MIXER:
        pAudioRen->mode_mixer = g_value_get_boolean (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x6e3, pAudioRen,
                  "Set using Mixer(TRUE=Mixer, FALSE=Decoder): %s", TF (pAudioRen->mode_mixer));
        SET_OR_DEFER (pAudioRen, lgao_set_mode_mixer (pAudioRen->ao, pAudioRen->mode_mixer),
                      PROP_MIXER, 0x6e5);
        break;

    case PROP_AUDIO_INDEX:
        if (pAudioRen->mode_mixer == 1)
            pAudioRen->mixer_port = g_value_get_uint (value);
        else
            pAudioRen->audio_port = g_value_get_uint (value);
        PROP_LOG (2, GST_LEVEL_INFO, 0x6ed, pAudioRen, "Set %s index to %u",
                  pAudioRen->mode_mixer ? "MIX" : "AUD",
                  pAudioRen->mode_mixer ? pAudioRen->mixer_port : pAudioRen->audio_port);
        SET_OR_DEFER (pAudioRen, lgao_set_port (pAudioRen->ao, pAudioRen->audio_port,
                                                pAudioRen->mixer_port),
                      PROP_AUDIO_INDEX, 0x6f1);
        break;

    case PROP_AUDIO_ONLY: {
        gboolean audio_only = g_value_get_boolean (value);

        PROP_LOG (0, GST_LEVEL_ERROR, 0x6f8, pAudioRen, "meaningless property");
        PROP_LOG (3, GST_LEVEL_INFO, 0x6f9, pAudioRen,
                  "Set Audio Only Stream(TRUE=Audio Only, FALSE=Audio and Video): %s",
                  TF (audio_only));
        break;
    }

    case PROP_SEAMLESS:
        pAudioRen->isSeamless = g_value_get_boolean (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x700, pAudioRen,
                  "Set Seamless audio change(TRUE=Enable, FALSE=Disable): %s",
                  TF (pAudioRen->isSeamless));
        break;

    case PROP_MUTE:
        pAudioRen->mode_mute = g_value_get_boolean (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x707, pAudioRen, "Set sound-mute to %s",
                  TF (pAudioRen->mode_mute));
        SET_OR_DEFER (pAudioRen, lgao_set_mute (pAudioRen->ao, pAudioRen->mode_mute),
                      PROP_MUTE, 0x709);
        break;

    case PROP_FADE: {
        const GstStructure *st;
        gint targetVolume = 0, duration = 0;
        const gchar *fadeType;

        PROP_LOG (3, GST_LEVEL_INFO, 0x714, pAudioRen, "ease volume setting start");
        st = gst_value_get_structure (value);
        if (st == NULL) {
            PROP_LOG (0, GST_LEVEL_ERROR, 0x747, pAudioRen, "ease volume fadeStructure is NULL");
            break;
        }
        if (!gst_structure_has_field (st, "target-volume") ||
            !gst_structure_has_field (st, "duration-in-ms") ||
            !gst_structure_has_field (st, "fade")) {
            PROP_LOG (0, GST_LEVEL_ERROR, 0x744, pAudioRen, "ease volume params are NULL");
            break;
        }
        gst_structure_get_int (st, "target-volume", &targetVolume);
        gst_structure_get_int (st, "duration-in-ms", &duration);
        fadeType = gst_structure_get_string (st, "fade");
        PROP_LOG (3, GST_LEVEL_INFO, 0x720, pAudioRen, "audio ease volume value = %d",
                  targetVolume);
        PROP_LOG (3, GST_LEVEL_INFO, 0x721, pAudioRen, "audio ease volume duration = %d",
                  duration);
        PROP_LOG (3, GST_LEVEL_INFO, 0x722, pAudioRen, "audio ease volume fadeType = %s",
                  fadeType);

        SET_OR_DEFER (pAudioRen,
                      lgao_set_fade (pAudioRen->ao, LGAO_FADE_PROP_TARGET_VOL, targetVolume),
                      PROP_FADE, 0x725);
        SET_OR_DEFER (pAudioRen, lgao_set_fade (pAudioRen->ao, LGAO_FADE_PROP_DURATION, duration),
                      PROP_FADE, 0x727);
        SET_OR_DEFER (pAudioRen,
                      lgao_set_fade (pAudioRen->ao, LGAO_FADE_PROP_REMAIN_DURATION, duration),
                      PROP_FADE, 0x729);
        if (fadeType == NULL)
            break;
        if (strcmp (fadeType, "Linear") == 0) {
            SET_OR_DEFER (pAudioRen,
                          lgao_set_fade (pAudioRen->ao, LGAO_FADE_PROP_TYPE, LGAO_EASE_LINEAR),
                          PROP_FADE, 0x72e);
        } else if (strcmp (fadeType, "InCubic") == 0) {
            SET_OR_DEFER (pAudioRen,
                          lgao_set_fade (pAudioRen->ao, LGAO_FADE_PROP_TYPE, LGAO_EASE_INCUBIC),
                          PROP_FADE, 0x732);
        } else if (strcmp (fadeType, "OutCubic") == 0) {
            SET_OR_DEFER (pAudioRen,
                          lgao_set_fade (pAudioRen->ao, LGAO_FADE_PROP_TYPE, LGAO_EASE_OUTCUBIC),
                          PROP_FADE, 0x736);
        } else {
            SET_OR_DEFER (pAudioRen,
                          lgao_set_fade (pAudioRen->ao, LGAO_FADE_PROP_TYPE, LGAO_EASE_NONE),
                          PROP_FADE, 0x73a);
            PROP_LOG (0, GST_LEVEL_ERROR, 0x73d, pAudioRen, "Unkown fadeType");
        }
        SET_OR_DEFER (pAudioRen, lgao_set_ease_volume (pAudioRen->ao), PROP_FADE, 0x740);
        break;
    }

    case PROP_CLOCK_TYPE:
        if (pAudioRen->clocktype) {
            g_free (pAudioRen->clocktype);
            pAudioRen->clocktype = NULL;
        }
        pAudioRen->clocktype = g_value_dup_string (value);
        if (pAudioRen->clocktype == NULL)
            break;
        if (strcmp (pAudioRen->clocktype, "atsc3-wall-clock") == 0 ||
            strcmp (pAudioRen->clocktype, "jp4k_ntp") == 0) {
            PROP_LOG (3, GST_LEVEL_INFO, 0x755, pAudioRen, "Set %s Type", pAudioRen->clocktype);
            SET_OR_DEFER (pAudioRen, lgao_set_clocktype (pAudioRen->ao, LGAO_CLOCK_TYPE_ATSC3),
                          PROP_CLOCK_TYPE, 0x756);
        } else {
            SET_OR_DEFER (pAudioRen, lgao_set_clocktype (pAudioRen->ao, LGAO_CLOCK_TYPE_GSTC),
                          PROP_CLOCK_TYPE, 0x759);
        }
        break;

    case PROP_PCM_BYPASS:
        pAudioRen->pcm_bypass = g_value_get_boolean (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x762, pAudioRen, "Set pcm (multi channel) bypass %s",
                  TF (pAudioRen->pcm_bypass));
        SET_OR_DEFER (pAudioRen, lgao_set_pcm_bypass (pAudioRen->ao, pAudioRen->pcm_bypass),
                      PROP_PCM_BYPASS, 0x764);
        break;

    case PROP_PCM_MULTI_CH:
        pAudioRen->pcm_multi_ch = g_value_get_boolean (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x76b, pAudioRen, "Set pcm (multi channel) bypass %s",
                  TF (pAudioRen->pcm_multi_ch));
        SET_OR_DEFER (pAudioRen,
                      lgao_set_pcm_multi_channel (pAudioRen->ao, pAudioRen->pcm_multi_ch),
                      PROP_PCM_MULTI_CH, 0x76d);
        break;

    case PROP_GAME_MODE:
        pAudioRen->game_mode = g_value_get_boolean (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x774, pAudioRen, "Set game mode %s",
                  pAudioRen->game_mode ? "Enable" : "Disable");
        SET_OR_DEFER (pAudioRen, lgao_set_game_mode (pAudioRen->ao, pAudioRen->game_mode),
                      PROP_GAME_MODE, 0x776);
        break;

    case PROP_QUEUE_MAX_LENGTH:
        pAudioRen->queue_max_length = g_value_get_uint (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x77d, pAudioRen, "Set PROP_QUEUE_MAX_LENGTH = %u",
                  pAudioRen->queue_max_length);
        if (pAudioRen->queue_max_length <= 0x4000)
            SET_OR_DEFER (pAudioRen,
                          lgao_set_queue_max_length (pAudioRen->ao, pAudioRen->queue_max_length),
                          PROP_QUEUE_MAX_LENGTH, 0x781);
        break;

    case PROP_VARIABLE_SPEED_PLAYBACK:
        pAudioRen->vsp_mode = g_value_get_boolean (value);
        PROP_LOG (3, GST_LEVEL_INFO, 0x789, pAudioRen,
                  "Set Netflix VSP(Variable Speed Playback) mode %s", TF (pAudioRen->vsp_mode));
        if (pAudioRen->vsp_mode)
            SET_OR_DEFER (pAudioRen, lgao_set_vsp_mode (pAudioRen->ao),
                          PROP_VARIABLE_SPEED_PLAYBACK, 0x78d);
        break;

    default:
        /* includes the read-only ids (undecoded-size, current-pts, fade-volume, ...) */
        g_log (NULL, G_LOG_LEVEL_WARNING,
               "%s:%d: invalid %s id %u for \"%s\" of type '%s' in '%s'",
               LG_SINK_FILE, 0x792, "property", prop_id, pspec->name,
               g_type_name (G_PARAM_SPEC_TYPE (pspec)), G_OBJECT_TYPE_NAME (object));
        break;
    }
}

/* @0x10318: index/mixer publish scalar ao state, never an ao/appsrc handle.
 * Index is logical (no mixer +4); missing ao returns UINT_MAX/FALSE. */
static void gst_lg_audio_render_get_property (GObject *object, guint prop_id,
                                       GValue *value, GParamSpec *pspec)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (object);
    gint fadeVolume = 100;
    guint64 undecodedSize = 0;
    gchar *contents;

    contents = g_strdup_value_contents (value);
    PROP_LOG (4, GST_LEVEL_DEBUG, 0x7a1, pAudioRen, "get_property \"%s\"(%d) = %s:'%s'",
              pspec->name, prop_id, g_type_name (G_VALUE_TYPE (value)), contents);
    g_free (contents);

    switch (prop_id) {
    case PROP_PLAY_SPEED: {
        gint curr_speed = 0;
        if (lgao_get_playrate (pAudioRen->ao, &curr_speed) == -1)
            PROP_LOG (0, GST_LEVEL_ERROR, 0x7ab, pAudioRen, "lgao_get_playrate failed!");
        else
            PROP_LOG (3, GST_LEVEL_INFO, 0x7ad, pAudioRen, "current play speed %d", curr_speed);
        g_value_set_int (value, curr_speed);
        break;
    }
    case PROP_DECODED_SIZE: {
        gint mode_mixer = 0;
        /* original bug: queries the mixer mode and returns the (zero)
         * undecoded size; decodedSize is never reported */
        SINK_CHECK (lgao_get_mode_mixer (pAudioRen->ao, &mode_mixer), 0x7b4);
        g_value_set_uint64 (value, undecodedSize);
        break;
    }
    case PROP_UNDECODED_SIZE:
        SINK_CHECK (lgao_get_buffer_status (pAudioRen->ao, &undecodedSize), 0x7ba);
        g_value_set_uint64 (value, undecodedSize);
        break;
    case PROP_CURRENT_PTS:
        g_value_set_uint64 (value, pAudioRen->currentPTS);
        break;
    case PROP_LIPSYNC_OFFSET: {
        guint offset = 0;
        SINK_CHECK (lgao_get_lipsync_offset (pAudioRen->ao, &offset), 0x7c4);
        g_value_set_uint (value, offset);
        break;
    }
    case PROP_DISABLE_GSTSYNC:
        g_value_set_boolean (value, pAudioRen->disableBasesinkSync);
        break;
    case PROP_LIPSYNC: {
        gint mode = 0;
        SINK_CHECK (lgao_get_lipsync_mode (pAudioRen->ao, &mode), 0x7ce);
        g_value_set_boolean (value, mode != 2);     /* 2 = lipsync off */
        break;
    }
    case PROP_NODELAY: {
        gint mode_nodelay = 0;
        SINK_CHECK (lgao_get_nodelay (pAudioRen->ao, &mode_nodelay), 0x7d6);
        g_value_set_boolean (value, mode_nodelay);
        break;
    }
    case PROP_NODELAY_MODE: {
        guint val = 0;
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_OP_MODE, &val),
                    0x7dd);
        g_value_set_uint (value, val);
        break;
    }
    case PROP_NODELAY_SKIP_UP: {
        guint val = 0;
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_SKIPPING,
                                  &val), 0x7e5);
        g_value_set_uint (value, val);
        break;
    }
    case PROP_NODELAY_FAST_UP: {
        guint val = 0;
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_FAST,
                                  &val), 0x7ed);
        g_value_set_uint (value, val);
        break;
    }
    case PROP_NODELAY_RECOVER: {
        guint val = 0;
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_RECOVER,
                                  &val), 0x7f5);
        g_value_set_uint (value, val);
        break;
    }
    case PROP_NODELAY_FAST_FREQ: {
        guint val = 0;
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_FREQ_FAST, &val),
                    0x7fd);
        g_value_set_uint (value, val);
        break;
    }
    case PROP_APP_TYPE: {
        LGAO_APPTYPE_T apptype = 0;
        SINK_CHECK (lgao_get_apptype (pAudioRen->ao, &apptype), 0x80d);
        switch (apptype) {
        case LGAO_APPTYPE_DIRECTMEDIA: g_value_set_string (value, "DIRECTMEDIA"); break;
        case LGAO_APPTYPE_RTC:         g_value_set_string (value, "RTC");         break;
        case LGAO_APPTYPE_MIRACAST:    g_value_set_string (value, "MIRACAST");    break;
        case LGAO_APPTYPE_JP4K:        g_value_set_string (value, "JP4K");        break;
        case LGAO_APPTYPE_IDGAPLESS:   g_value_set_string (value, "ID-GAPLESS");  break;
        case LGAO_APPTYPE_R2R:         g_value_set_string (value, "R2R");         break;
        case LGAO_APPTYPE_SYE:         g_value_set_string (value, "SYE");         break;
        default:                       g_value_set_string (value, "UNKNOWN");     break;
        }
        break;
    }
    case PROP_MIXER: {
        gint mode_mixer = 0;
        SINK_CHECK (lgao_get_mode_mixer (pAudioRen->ao, &mode_mixer), 0x82d);
        g_value_set_boolean (value, mode_mixer);
        break;
    }
    case PROP_AUDIO_INDEX: {
        gint port_index = -1;
        SINK_CHECK (lgao_get_port (pAudioRen->ao, &port_index), 0x835);
        g_value_set_uint (value, port_index);
        break;
    }
    case PROP_AUDIO_ONLY:
        g_value_set_boolean (value, FALSE);
        PROP_LOG (0, GST_LEVEL_ERROR, 0x83d, pAudioRen, "meaningless property");
        break;
    case PROP_SEAMLESS:
        g_value_set_boolean (value, pAudioRen->isSeamless);
        break;
    case PROP_MUTE: {
        gint mode = 0;
        SINK_CHECK (lgao_get_mute (pAudioRen->ao, &mode), 0x847);
        g_value_set_boolean (value, mode);
        break;
    }
    case PROP_FADE_VOLUME:
        PROP_LOG (3, GST_LEVEL_INFO, 0x84c, pAudioRen, "get ease fadeVolume");
        SINK_CHECK (lgao_get_fade (pAudioRen->ao, LGAO_FADE_PROP_EASE_VOL, &fadeVolume), 0x84d);
        g_value_set_int (value, fadeVolume);
        if (fadeVolume < 0)
            PROP_LOG (3, GST_LEVEL_INFO, 0x851, pAudioRen, "GetVolume Err");
        else
            PROP_LOG (3, GST_LEVEL_INFO, 0x853, pAudioRen, "get fadeVolume = %d", fadeVolume);
        break;
    case PROP_CLOCK_TYPE:
        PROP_LOG (0, GST_LEVEL_ERROR, 0x85a, pAudioRen, "meaningless property get");
        g_value_set_string (value, NULL);
        break;
    case PROP_AUDIO_BE_DELAY:
        g_value_set_uint (value, 85);     /* fixed back-end delay, ms */
        break;
    case PROP_DIRECTMEDIA_TOTAL_BUF_SIZE: {
        guint size = 0;
        SINK_CHECK (lgao_get_buffer_size_for_directmedia (pAudioRen->ao, &size,
                                                          LGAO_DIRECTMEDIA_TOTAL_BUF), 0x867);
        g_value_set_int (value, size);
        break;
    }
    case PROP_DIRECTMEDIA_USED_BUF_SIZE: {
        guint size = 0;
        SINK_CHECK (lgao_get_buffer_size_for_directmedia (pAudioRen->ao, &size,
                                                          LGAO_DIRECTMEDIA_USED_BUF), 0x86f);
        g_value_set_int (value, size);
        break;
    }
    case PROP_PCM_BYPASS: {
        gint bypass = 0;
        SINK_CHECK (lgao_get_pcm_bypass (pAudioRen->ao, &bypass), 0x878);
        g_value_set_boolean (value, bypass);
        break;
    }
    case PROP_PCM_MULTI_CH: {
        gint multi_ch = 0;
        SINK_CHECK (lgao_get_pcm_multi_channel (pAudioRen->ao, &multi_ch), 0x880);
        g_value_set_boolean (value, multi_ch);
        break;
    }
    case PROP_GAME_MODE: {
        gint game_mode = 0;
        SINK_CHECK (lgao_get_game_mode (pAudioRen->ao, &game_mode), 0x889);
        g_value_set_boolean (value, game_mode);
        break;
    }
    case PROP_RENDER_BUFFER_THRESHOLD: {
        guint val = 0;
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_BUFFERING_LEVEL,
                                  &val), 0x805);
        g_value_set_uint (value, val);
        break;
    }
    case PROP_QUEUE_MAX_LENGTH: {
        gint queue_max_length = 0;
        SINK_CHECK (lgao_get_queue_max_length (pAudioRen->ao, &queue_max_length), 0x890);
        g_value_set_uint (value, queue_max_length);
        break;
    }
    case PROP_VARIABLE_SPEED_PLAYBACK:
        g_value_set_boolean (value, pAudioRen->vsp_mode);
        break;
    case PROP_VERSION:
        g_value_set_uint (value, AUDIOSINK_VERSION);
        break;
    default:
        g_log (NULL, G_LOG_LEVEL_WARNING,
               "%s:%d: invalid %s id %u for \"%s\" of type '%s' in '%s'",
               LG_SINK_FILE, 0x89e, "property", prop_id, pspec->name,
               g_type_name (G_PARAM_SPEC_TYPE (pspec)), G_OBJECT_TYPE_NAME (object));
        break;
    }
}

/* @0x8240 gst_lg_audio_render_event: GstBaseSink::event; everything except an upstream
 * mpegh-object-ui OOB event and a failed CAPS is chained to GstBaseSink.
 * Shipped Thumb 0x87fa..0x8802: FLUSH_STOP sets speed 0 then flushes;
 * FLUSH_START 0x8884..0x888c only stores {isFlushing=1,bSeamlessSeek=0}
 * then chains to the parent: it makes NO lgao cancellation/flush call. A public
 * flush-start alone cannot guarantee cancellation of a synchronous ring push;
 * chain() does not test isFlushing either. EOS 0x8dae..0x8dc4 skips an untouched PCM FIRST,
 * otherwise waits once (including lgao_eos errors <=0), never a predicate loop.
 * STREAM_START 0x8ac8..0x8b08 unconditionally rebuilds and plays on a changed
 * nonseamless ID: no currentState/drop check, no set_audio_info in this path.
 * These events can invalidate borrowed appsrcs even without a render call. */
gboolean gst_lg_audio_render_event (GstBaseSink *sink, GstEvent *event)
{
    gboolean ret = TRUE;
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (sink);
    GstEventType type = GST_EVENT_TYPE (event);

    SINK_LOG (4, GST_LEVEL_DEBUG, 2221, pAudioRen, "got event(%s)", GST_EVENT_TYPE_NAME (event));

    switch (type) {
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
                                              gst_clock_get_time (GST_ELEMENT (sink)->clock) - segment->base -
                                              GST_ELEMENT (sink)->base_time, segment->start);

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
            ret = FALSE;
            goto done;
        }
        /* renderer (re)opened after PLAYING started: program the base time now */
        if (pAudioRen->currentState == GST_STATE_PLAYING && caps_changed) {
            GstElement *element = GST_ELEMENT (sink);
            SINK_LOG (3, GST_LEVEL_INFO, 2350, pAudioRen, "Update clock when CAPS event is late");
            gst_lg_audio_render_update_clock (pAudioRen,
                                              gst_clock_get_time (element->clock) - GST_BASE_SINK (element)->segment.base -
                                              element->base_time, GST_BASE_SINK (element)->segment.start);
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
            goto done;
        }
        break;

    default:
        break;
    }

    ret = GST_BASE_SINK_CLASS (parent_class)->event (sink, event);
done:
    return ret;
}

/* @0xbbf4 gst_lg_audio_render_chain: GstBaseSink::render. Feeds the buffer to lgao in
 * <= 64 KiB sub-buffers; the data type (PCM / ES / metadata) comes from LG buffer flags.
 * Shipped Thumb 0xbd5a..0xbd86: speed 250..2000 inclusive, mute exactly 1,
 * CORRUPTED before UI/drop flags. Clock calibration happens before all gates.
 * 0xbe78 / 0xcaee calls drop/UNDROP before UI, dump, state, PTS or unsupported
 * checks: even an ultimately discarded buffer can destroy/rebuild appsrcs.
 * A render-scoped producer could suppress stock feed, but render is not an
 * exclusive lifecycle boundary (CAPS/STREAM_START/stop/properties also act).
 * Captured bypass pointers must not survive these calls or a generation change.
 * 0xbfb6..0xc01c caps chunks at 65536; 0xc152 retries the same offset forever
 * on copy failure, resetting the remainder to zero after the first failure.
 * 0xc06a map failure skips a chunk; 0xc696 feed failure is log-only. Every exit
 * returns GST_FLOW_OK, including muted/trick/corrupt/READY/unsupported/drop,
 * empty buffers and failed feeds; normal feeding does NOT add decodedSize.
 * PTS is updated only past the state gate; each chunk receives the ORIGINAL
 * PTS (even NONE). Host tests exercise recovered source, not shipped execution. */
GstFlowReturn gst_lg_audio_render_chain (GstBaseSink *sink, GstBuffer *buffer)
{
    gint playspeed = 0, mode_nodelay = 0, mode_mute = 0, bufferedsize = 0;
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (sink);
    guint bufSize = gst_buffer_get_size (buffer);
    LGAO_DATA_TYPE_T outType;
    GstMapInfo bufMapInfo;
    guint flags;

    SINK_LOG (4, GST_LEVEL_DEBUG, 2490, pAudioRen, "called");
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
        SINK_LOG (4, GST_LEVEL_DEBUG, 2537, pAudioRen, "DROP!!!!! TS =%u:%02u:%02u.%09u\n",
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
    SINK_LOG (4, GST_LEVEL_DEBUG, 2574, pAudioRen, "outType = %d", outType);

    /* before the first byte is decoded, only render in PAUSED/PLAYING (no real wait) */
    if (pAudioRen->decodedSize == 0 && pAudioRen->currentState != GST_STATE_PLAYING) {
        SINK_LOG (5, GST_LEVEL_LOG, 2579, pAudioRen, "wait PLAYING state.(%s)",
                  gst_element_state_get_name (pAudioRen->currentState));
        if (pAudioRen->currentState <= GST_STATE_READY) {
            SINK_LOG (0, GST_LEVEL_ERROR, 2583, pAudioRen, "giveup wait...(%s)",
                      gst_element_state_get_name (pAudioRen->currentState));
            goto done;
        }
    }

    if (GST_BUFFER_PTS_IS_VALID (buffer)) {
        pAudioRen->currentPTS = GST_BUFFER_PTS (buffer);
        SINK_GST (GST_LEVEL_DEBUG, 2592, pAudioRen, "current_timestamp %u:%02u:%02u.%09u, %s:%d",
                  GST_TIME_ARGS (pAudioRen->currentPTS), __func__, 2594);
    }

    if (pAudioRen->isUnsupportedStream == TRUE) {
        SINK_LOG (5, GST_LEVEL_LOG, 2598, pAudioRen, "Unsupported Audio");
        pAudioRen->decodedSize += bufSize;
        goto done;
    }

    SINK_LOG (4, GST_LEVEL_DEBUG, 2605, pAudioRen, "map buffer");
    {
        gsize offset = 0, remaining_size;

        while (bufSize != 0) {
            GstBuffer *chunk;

            /* On copy failure the same chunk is retried and the remainder is lost. */
            remaining_size = 0;
            if (bufSize > CHAIN_MAX_FEED_SIZE) {
                SINK_LOG (4, GST_LEVEL_DEBUG, 2609, pAudioRen, "bufSize = %d, limitSize = %d\n",
                          (gint) bufSize, CHAIN_MAX_FEED_SIZE);
                remaining_size = bufSize - CHAIN_MAX_FEED_SIZE;
                bufSize = CHAIN_MAX_FEED_SIZE;
            }
            chunk = gst_buffer_copy_region (buffer, GST_BUFFER_COPY_ALL, offset, bufSize);
            if (chunk == NULL) {
                SINK_LOG (0, GST_LEVEL_ERROR, 2619, pAudioRen, "gst_buffer_copy_region() Error");
                continue;
            }

            SINK_LOG (4, GST_LEVEL_DEBUG, 2621, pAudioRen,
                      "offset = %d, bufSize = %d, remainBufSize = %d\n",
                      (gint) offset, (gint) bufSize, (gint) remaining_size);

            if (!gst_buffer_map (chunk, &bufMapInfo, GST_MAP_READ)) {
                SINK_LOG (0, GST_LEVEL_ERROR, 2625, pAudioRen, "gst_buffer_map() Error");
            } else {
                bufSize = bufMapInfo.size;
                gst_lg_audio_render_log_timestamp (pAudioRen, buffer, outType, bufSize);
                lgao_get_size_buffered (pAudioRen->ao, outType, &bufferedsize);
                /* every chunk carries the PTS of the whole input buffer */
                if (lgao_feed (pAudioRen->ao, bufMapInfo.data, bufSize, GST_BUFFER_PTS (buffer),
                               outType) < 0)
                    SINK_LOG (0, GST_LEVEL_ERROR, 2637, pAudioRen, "lgao_feed() is FAILED!!!");
                SINK_LOG (4, GST_LEVEL_DEBUG, 2640, pAudioRen, "RENDER %" GST_PTR_FORMAT " out=%s",
                          chunk, out_type_name (outType));
                gst_buffer_unmap (chunk, &bufMapInfo);
            }
            offset += bufSize;
            gst_buffer_unref (chunk);
            bufSize = remaining_size;
        }
    }

done:
    SINK_LOG (4, GST_LEVEL_DEBUG, 2658, pAudioRen, "done");
    return GST_FLOW_OK;
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

/* @0xd040 gst_lg_audio_render_stop: GstBaseSink::stop - kick and join the event task,
 * tear the renderer down and close lgao. task_mutex belongs to GstTask execution;
 * joining excludes that callback task only, NOT external submission threads.
 * eos_mutex/flush_mutex protect notification waits, not ao/appsrc lifetime.
 * A pad probe/stream lock alone cannot pin ao here. Any private producer hook
 * must cover stop/close as well as caps/STREAM_START/drop/undrop rebuilds, and
 * reject new work BEFORE module destruction. A blocked synchronous push must
 * be cancelled before waiting for in-flight drain; public FLUSH_START alone
 * is NOT that cancellation (lgao_flush is only in FLUSH_STOP above). Holding a
 * private lifecycle mutex across both push and teardown can deadlock. */
gboolean gst_lg_audio_render_stop (GstBaseSink *sink)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (sink);
    lgao_mq_item kick;

    pAudioRen->decodedSize = 0;
    pAudioRen->currentPTS = 0;
    RENDER_LOG (2, GST_LEVEL_INFO, 2696, pAudioRen, "trace");

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

    RENDER_LOG (3, GST_LEVEL_INFO, 2711, pAudioRen, "stopping renderer & callback task");
    kick.mtype = LGAO_MSG_TYPE_EVENT;
    kick.event = LGAO_EVENT_CLOSE_IPC;
    RENDER_LOG (3, GST_LEVEL_INFO, 2715, pAudioRen, "kicking locked task");
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
    RENDER_LOG (2, GST_LEVEL_INFO, 2741, pAudioRen, "stop");
    return TRUE;
}

static void
_gst_lg_audio_render_set_preconditions (GstLgAudioRender *pAudioRen)
{
    int prop_id;

    for (prop_id = PROP_PLAY_SPEED; prop_id < PROP_VERSION; prop_id++) {
        if (!((pAudioRen->preopen_conditions >> (prop_id - 1)) & 1))
            continue;

        switch (prop_id) {
        case PROP_PLAY_SPEED:
            SINK_LOG (2, GST_LEVEL_INFO, 0xac3, pAudioRen, "applying PROP_PLAY_SPEED...%d",
                      pAudioRen->play_speed);
            lgao_set_playrate (pAudioRen->ao, pAudioRen->play_speed);
            break;
        case PROP_LIPSYNC_OFFSET:
            SINK_LOG (2, GST_LEVEL_INFO, 0xac8, pAudioRen, "applying PROP_LIPSYNC_OFFSET...%d",
                      pAudioRen->lipsync_offset);
            if (pAudioRen->lipsync_offset != 0xfffffff)
                lgao_set_lipsync_offset (pAudioRen->ao, pAudioRen->lipsync_offset);
            break;
        case PROP_LIPSYNC:
            SINK_LOG (2, GST_LEVEL_INFO, 0xace, pAudioRen, "applying PROP_LIPSYNC...%d",
                      pAudioRen->lipsync_mode);
            /* lgao lipsync mode: 1 = on, 2 = off ("free run") */
            lgao_set_lipsync_mode (pAudioRen->ao, pAudioRen->lipsync_mode ? 1 : 2);
            break;
        case PROP_NODELAY:
            SINK_LOG (2, GST_LEVEL_INFO, 0xad5, pAudioRen, "applying PROP_NODELAY...%d",
                      pAudioRen->nodelay_mode);
            lgao_set_nodelay (pAudioRen->ao, pAudioRen->nodelay_mode);
            break;
        case PROP_NODELAY_MODE:
            SINK_LOG (2, GST_LEVEL_INFO, 0xada, pAudioRen, "applying PROP_NODELAY_MODE...%d",
                      pAudioRen->rts_mode);
            if (pAudioRen->rts_mode != 6)
                lgao_set_rts (pAudioRen->ao, LGAO_RTS_OP_MODE, pAudioRen->rts_mode);
            break;
        case PROP_NODELAY_SKIP_UP:
            SINK_LOG (2, GST_LEVEL_INFO, 0xae0, pAudioRen, "applying PROP_NODELAY_SKIP_UP...%d",
                      pAudioRen->nodelay_skip_up);
            lgao_set_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_SKIPPING, pAudioRen->nodelay_skip_up);
            break;
        case PROP_NODELAY_FAST_UP:
            SINK_LOG (2, GST_LEVEL_INFO, 0xae6, pAudioRen, "applying PROP_NODELAY_FAST_UP...%d",
                      pAudioRen->nodelay_fast_up);
            lgao_set_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_FAST, pAudioRen->nodelay_fast_up);
            break;
        case PROP_NODELAY_RECOVER:
            SINK_LOG (2, GST_LEVEL_INFO, 0xaec, pAudioRen, "applying PROP_NODELAY_RECOVER...%d",
                      pAudioRen->nodelay_recover);
            lgao_set_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_RECOVER, pAudioRen->nodelay_recover);
            break;
        case PROP_NODELAY_FAST_FREQ:
            SINK_LOG (2, GST_LEVEL_INFO, 0xaf2, pAudioRen, "applying PROP_NODELAY_FAST_FREQ...%d",
                      pAudioRen->nodelay_fast_freq);
            lgao_set_rts (pAudioRen->ao, LGAO_RTS_FREQ_FAST, pAudioRen->nodelay_fast_freq);
            break;
        case PROP_APP_TYPE: {
            const char *app = pAudioRen->pAppType;
            LGAO_APPTYPE_T type;

            SINK_LOG (2, GST_LEVEL_INFO, 0xafe, pAudioRen, "applying PROP_APP_TYPE...%s", app);
            if (app == NULL)
                break;
            if (strcmp (app, "AIRPLAY") == 0) {
                SINK_LOG (3, GST_LEVEL_INFO, 0xb02, pAudioRen,
                          "This application is Airplay Mirroring");
                lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_AIRPLAY);
                lgao_set_nodelay (pAudioRen->ao, TRUE);
                break;
            } else if (strcmp (app, "RTC") == 0) {
                SINK_LOG (3, GST_LEVEL_INFO, 0xb07, pAudioRen, "This application is WiDi");
                type = LGAO_APPTYPE_RTC;
            } else if (strcmp (app, "R2R") == 0) {
                SINK_LOG (3, GST_LEVEL_INFO, 0xb0e, pAudioRen, "This application is R2R");
                type = LGAO_APPTYPE_R2R;
            } else if (strcmp (app, "MIRACAST") == 0) {
                SINK_LOG (3, GST_LEVEL_INFO, 0xb13, pAudioRen, "This application is Miracast");
                type = LGAO_APPTYPE_MIRACAST;
            } else if (strcmp (app, "JP4K") == 0) {
                lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_JP4K);
                break;
            } else if (strcmp (app, "DIRECTMEDIA") == 0) {
                SINK_LOG (3, GST_LEVEL_INFO, 0xb1a, pAudioRen,
                          "This application is Cloud Game(DIRECTMEDIA)");
                type = LGAO_APPTYPE_DIRECTMEDIA;
            } else {
                if (strcmp (app, "ID-GAPLESS") == 0) {
                    SINK_LOG (3, GST_LEVEL_INFO, 0xb20, pAudioRen,
                              "This application is for ID GAPLESS PLAY");
                    type = LGAO_APPTYPE_IDGAPLESS;
                } else if (strcmp (app, "APPCAST") == 0) {
                    SINK_LOG (3, GST_LEVEL_INFO, 0xb23, pAudioRen,
                              "This application is for APPCAST");
                    type = LGAO_APPTYPE_APPCAST;
                } else if (strcmp (app, "SYE") == 0) {
                    SINK_LOG (3, GST_LEVEL_INFO, 0xb26, pAudioRen,
                              "This application is for AMAZON SYE");
                    type = LGAO_APPTYPE_SYE;
                } else {
                    type = LGAO_APPTYPE_UNKNOWN;
                }
                lgao_set_apptype (pAudioRen->ao, type);
                break;
            }
            /* low-latency apps: renderer no-delay + no basesink clock sync */
            lgao_set_apptype (pAudioRen->ao, type);
            lgao_set_nodelay (pAudioRen->ao, TRUE);
            gst_base_sink_set_sync (GST_BASE_SINK (pAudioRen), FALSE);
            break;
        }
        case PROP_RESOURCE_INFO:
            SINK_LOG (2, GST_LEVEL_INFO, 0xb2e, pAudioRen,
                      "applying PROP_RESOURCE_INFO...port %d,%d",
                      pAudioRen->audio_port, pAudioRen->mixer_port);
            lgao_set_port (pAudioRen->ao, pAudioRen->audio_port, pAudioRen->mixer_port);
            break;
        case PROP_MIXER:
            SINK_LOG (2, GST_LEVEL_INFO, 0xb34, pAudioRen, "applying PROP_MIXER...%d",
                      pAudioRen->mode_mixer);
            lgao_set_mode_mixer (pAudioRen->ao, pAudioRen->mode_mixer);
            break;
        case PROP_AUDIO_INDEX:
            SINK_LOG (2, GST_LEVEL_INFO, 0xb39, pAudioRen, "applying PROP_AUDIO_INDEX...%d,%d",
                      pAudioRen->audio_port, pAudioRen->mixer_port);
            lgao_set_port (pAudioRen->ao, pAudioRen->audio_port, pAudioRen->mixer_port);
            break;
        case PROP_MUTE:
            SINK_LOG (2, GST_LEVEL_INFO, 0xb3f, pAudioRen, "applying PROP_MUTE...%d",
                      pAudioRen->mode_mute);
            lgao_set_mute (pAudioRen->ao, pAudioRen->mode_mute);
            break;
        case PROP_CLOCK_TYPE: {
            const gchar *clk = pAudioRen->clocktype;

            SINK_LOG (2, GST_LEVEL_INFO, 0xb44, pAudioRen, "applying PROP_CLOCK_TYPE...%s", clk);
            if (clk == NULL)
                break;
            if (strcmp (clk, "atsc3-wall-clock") == 0 || strcmp (clk, "jp4k_ntp") == 0) {
                SINK_LOG (3, GST_LEVEL_INFO, 0xb49, pAudioRen, "Set %s Type", clk);
                lgao_set_clocktype (pAudioRen->ao, LGAO_CLOCK_TYPE_ATSC3);
            } else {
                lgao_set_clocktype (pAudioRen->ao, LGAO_CLOCK_TYPE_GSTC);
            }
            break;
        }
        case PROP_PCM_BYPASS:
            SINK_LOG (3, GST_LEVEL_INFO, 0xb52, pAudioRen,
                      "This application is for Cloud Game (Google Stadia) PCM Multi Channel");
            lgao_set_pcm_bypass (pAudioRen->ao, pAudioRen->pcm_bypass);
            break;
        case PROP_PCM_MULTI_CH:
            SINK_LOG (3, GST_LEVEL_INFO, 0xb59, pAudioRen,
                      "This application is for Netflix LPCM Multi Channel");
            lgao_set_pcm_multi_channel (pAudioRen->ao, pAudioRen->pcm_multi_ch);
            break;
        case PROP_GAME_MODE:
            SINK_LOG (3, GST_LEVEL_INFO, 0xb60, pAudioRen, "applying PROP_GAME_MODE...%d",
                      pAudioRen->game_mode);
            lgao_set_game_mode (pAudioRen->ao, pAudioRen->game_mode);
            break;
        case PROP_RENDER_BUFFER_THRESHOLD:
            SINK_LOG (2, GST_LEVEL_INFO, 0xaf8, pAudioRen,
                      "applying PROP_RENDER_BUFFER_THRESHOLD...%d", pAudioRen->render_buf_thres);
            lgao_set_rts (pAudioRen->ao, LGAO_RTS_BUFFERING_LEVEL, pAudioRen->render_buf_thres);
            break;
        case PROP_QUEUE_MAX_LENGTH:
            SINK_LOG (3, GST_LEVEL_INFO, 0xb67, pAudioRen, "applying PROP_QUEUE_MAX_LENGTH...%d",
                      pAudioRen->queue_max_length);
            lgao_set_queue_max_length (pAudioRen->ao, pAudioRen->queue_max_length);
            break;
        case PROP_VARIABLE_SPEED_PLAYBACK:
            SINK_LOG (3, GST_LEVEL_INFO, 0xb6f, pAudioRen,
                      "Set Netflix VSP(Variable Speed Playback) saeamless mode. %s",
                      pAudioRen->vsp_mode ? "TRUE" : "FALSE");
            if (pAudioRen->vsp_mode)
                lgao_set_vsp_mode (pAudioRen->ao);
            break;
        default:
            SINK_LOG (3, GST_LEVEL_INFO, 0xb80, pAudioRen,
                      "property not for preopen condition %d", prop_id);
            break;
        }
    }
}

/* @0xd7c0 gst_lg_audio_render_change_state
 * NULL_TO_READY obtains this instance's ao before chaining to BaseSink::start;
 * downward transitions chain first, allowing BaseSink::stop to free ao.
 * Neither this override nor the matching READY_TO_NULL BaseSink stop call uses
 * this sink pad's stream mutex as a universal producer lease. State transition
 * exclusion is not exclusion against arbitrary external appsrc submissions.
 * Renderer/state setters are applied BEFORE upward parent transition; lower
 * return codes only log. A failed parent PLAYING transition can therefore
 * leave currentState=PLAYING and the renderer already started. NULL_TO_READY
 * similarly keeps its freshly opened ao if parent start fails. Downward parent
 * failure does not skip this override's flush/pause/deinit. Readiness for a
 * private producer must follow actual context/configuration, not state enum or
 * transition return alone. Source-extracted tests preserve these failure paths. */
static GstStateChangeReturn gst_lg_audio_render_change_state (GstElement *element,
                                                       GstStateChange transition)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (element);
    GstState from = GST_STATE_TRANSITION_CURRENT (transition);
    GstState to = GST_STATE_TRANSITION_NEXT (transition);
    void *ao = pAudioRen->ao;       /* NULL before the first NULL_TO_READY */
    int mode_mixer = 0;
    int port_index = -1;
    LGAO_STATE_T rendererState;
    GstStateChangeReturn ret;

    if (ao == NULL) {
        SINK_LOG (2, GST_LEVEL_INFO, 0xb9f, pAudioRen, "change state from %s to %s",
                  gst_element_state_get_name (from), gst_element_state_get_name (to));
    } else {
        SINK_CHECK (lgao_get_mode_mixer (pAudioRen->ao, &mode_mixer), 0xb95);
        SINK_CHECK (lgao_get_port (pAudioRen->ao, &port_index), 0xb96);
        SINK_CHECK (lgao_get_state (pAudioRen->ao, &rendererState), 0xb97);
        SINK_LOG (2, GST_LEVEL_INFO, 0xb99, pAudioRen, "%s.%d - change state from %s to %s",
                  mode_mixer == 1 ? "MIX" : "DEC", port_index,
                  gst_element_state_get_name (from), gst_element_state_get_name (to));
    }
    pAudioRen->nextState = to;

    switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
        pAudioRen->currentState = GST_STATE_READY;
        pAudioRen->ao = lgao_open ();
        if (pAudioRen->preopen_conditions != 0) {
            SINK_LOG (2, GST_LEVEL_INFO, 0xbab, pAudioRen, "send preopen properties to lgao");
            _gst_lg_audio_render_set_preconditions (pAudioRen);
        }
        break;

    case GST_STATE_CHANGE_READY_TO_PAUSED:
        pAudioRen->currentState = GST_STATE_PAUSED;
        break;

    case GST_STATE_CHANGE_PAUSED_TO_PLAYING: {
        GstClockTime now;
        gint rate;

        rate = (gint) floor (GST_BASE_SINK (element)->segment.rate * 1000.0 + 0.5);
        pAudioRen->currentState = GST_STATE_PLAYING;
        SINK_LOG (2, GST_LEVEL_INFO, 0xbba, pAudioRen, "state: playing");
        lgao_set_playrate (pAudioRen->ao, rate ? rate : 1000);

        now = gst_clock_get_time (element->clock);
        if (pAudioRen->bSeamlessSeek) {
            gst_lg_audio_render_prepare_seamless_seek (GST_BASE_SINK (pAudioRen),
                                                       &GST_BASE_SINK (element)->segment, GST_CLOCK_TIME_NONE);
        } else if (!pAudioRen->instant_rate_change) {
            GstClockTime base = now - element->base_time - GST_BASE_SINK (element)->segment.base;

            SINK_LOG (3, GST_LEVEL_INFO, 0xbc9, pAudioRen,
                      "base %" GST_TIME_FORMAT "= now %" GST_TIME_FORMAT " element base_time %"
                      GST_TIME_FORMAT " segment base %" GST_TIME_FORMAT,
                      GST_TIME_ARGS (base), GST_TIME_ARGS (now),
                      GST_TIME_ARGS (element->base_time), GST_TIME_ARGS (GST_BASE_SINK (element)->segment.base));
            gst_lg_audio_render_update_clock (pAudioRen, base, GST_BASE_SINK (element)->segment.start);
        } else {
            /* instant rate change: restart the clock at the video decoder's PTS */
            guint pts = 0;

            lgao_get_vdec_current_pts (pAudioRen->ao, pAudioRen->video_port, &pts);
            pAudioRen->irc_pts = (guint64) pts / 9 * 100000;      /* 90 kHz -> ns */
            SINK_LOG (3, GST_LEVEL_INFO, 0xbd7, pAudioRen,
                      "basetime update for instant rate change... now : %" GST_TIME_FORMAT
                      ", irc_pts : %" GST_TIME_FORMAT ", segment_start : %" GST_TIME_FORMAT,
                      GST_TIME_ARGS (now), GST_TIME_ARGS (pAudioRen->irc_pts),
                      GST_TIME_ARGS (pAudioRen->segment_start));
            gst_lg_audio_render_update_clock (pAudioRen, 0,
                    pAudioRen->avail_irc_vdec_pts ? (gint64) pAudioRen->irc_pts
                                                  : pAudioRen->segment_start);
            pAudioRen->avail_irc_vdec_pts = TRUE;
        }
        SINK_CHECK (lgao_play (pAudioRen->ao), 0xbe5);
        SINK_CHECK (lgao_start_renderer (pAudioRen->ao), 0xbe6);
        pAudioRen->currentState = GST_STATE_PLAYING;
        break;
    }

    case GST_STATE_CHANGE_PLAYING_TO_PLAYING:
        SINK_LOG (3, GST_LEVEL_INFO, 0xbfd, pAudioRen, "Requested change PLAYING to PLAYING???");
        if (pAudioRen->currentState != GST_STATE_PLAYING) {
            GstClockTime now;

            pAudioRen->currentState = GST_STATE_PLAYING;
            now = gst_clock_get_time (element->clock);
            gst_lg_audio_render_update_clock (pAudioRen,
                    now - element->base_time - GST_BASE_SINK (element)->segment.base, GST_BASE_SINK (element)->segment.start);
            SINK_CHECK (lgao_play (pAudioRen->ao), 0xc0e);
            SINK_CHECK (lgao_start_renderer (pAudioRen->ao), 0xc0f);
        }
        break;

    default:
        break;
    }

    ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

    switch (transition) {
        case GST_STATE_CHANGE_PAUSED_TO_READY:
            pAudioRen->currentState = GST_STATE_READY;
            lgao_flush (pAudioRen->ao);
            break;
        case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
            pAudioRen->currentState = GST_STATE_PAUSED;
            SINK_LOG (2, GST_LEVEL_INFO, 0xc24, pAudioRen, "state: paused");
            if (rendererState == LGAO_STATE_PLAY && lgao_pause (pAudioRen->ao) < 0)
                SINK_LOG (0, GST_LEVEL_ERROR, 0xc29, pAudioRen, "Pause ERR!!!");
            break;
        case GST_STATE_CHANGE_READY_TO_NULL:
            /* lgao_close() already happened in basesink stop() */
            pAudioRen->currentState = GST_STATE_NULL;
            gst_lxlog_deinit ();
            break;
        default:
            break;
    }

    if (ao == NULL) {
        SINK_LOG (2, GST_LEVEL_INFO, 0xc45, pAudioRen, "change state from %s to %s. DONE",
                  gst_element_state_get_name (from), gst_element_state_get_name (to));
    } else {
        SINK_LOG (2, GST_LEVEL_INFO, 0xc3f, pAudioRen, "%s.%d - change state from %s to %s. DONE",
                  mode_mixer == 1 ? "MIX" : "DEC", port_index,
                  gst_element_state_get_name (from), gst_element_state_get_name (to));
    }
    return ret;
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

/* @0x14120 gst_lg_audio_render_plugin_init */
gboolean gst_lg_audio_render_plugin_init (GstPlugin *plugin)
{
    return gst_element_register (plugin, "audiosink", GST_RANK_SECONDARY,
                                 GST_TYPE_LG_AUDIO_RENDER);
}

/* @0x14144 gst_plugin_lgaudiosink_get_desc */
const GstPluginDesc *gst_plugin_lgaudiosink_get_desc (void)
{
    return &gst_plugin_desc;
}

/* @0x14150 gst_plugin_lgaudiosink_register */
void gst_plugin_lgaudiosink_register (void)
{
    gst_plugin_register_static (GST_VERSION_MAJOR, LG_GST_VERSION_MINOR, "lgaudiosink",
                                "LG's audio sink element",
                                gst_lg_audio_render_plugin_init, "1.0",
                                "Proprietary", "gst-plugins-bsp", "LG Media Framework",
                                "www.lge.com");
}
