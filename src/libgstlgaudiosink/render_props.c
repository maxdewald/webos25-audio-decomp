/*
 * GstLgAudioRender ("audiosink" element of the lgaudiosink plugin): GType
 * boilerplate, instance/class init (property table + signals), finalize,
 * get/set_property, change_state and the plugin entry points.
 * Data flow (chain/event/preroll/query/clock/task) lives in render_element.c.
 *
 * RECONSTRUCTION from usr/lib/gstreamer-1.0/libgstlgaudiosink.so
 *   sha256 90dba3ef32704ce4096bf1e6045d2879d0c2192d507bc04d1e8e98437f420a43
 * Original source: gst-plugins-bsp/gstlgaudio/gstlgaudiosink.c (DWARF CU).
 * Evidence: decomp/libgstlgaudiosink.so.c, decomp/libgstlgaudiosink.so.strings.tsv.
 * The line argument of SINK_LOG*() is the __LINE__ of the original file.
 * Tags are raw ELF VAs (decomp.toml src_addr_bias = 0x10000).
 */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "render_element.h"
#include "lgao.h"

/* ---- shared element globals (declared extern in render_element.h) ------ */
struct logmm_module logmm_module_media_event = { 0, 0, 7, &logmm_dummy_mask, "media_event" };
GstDebugCategory   *gst_lg_audio_render_debug;
gpointer            parent_class;
guint               gst_lg_audio_render_signals[LAST_SIGNAL];
static gint         GstLgAudioRender_private_offset;

extern void gst_lxlog_init (void);

void         gst_lg_audio_render_finalize (GObject *obj);
void         gst_lg_audio_render_set_property (GObject *object, guint prop_id,
                                               const GValue *value, GParamSpec *pspec);
void         gst_lg_audio_render_get_property (GObject *object, guint prop_id,
                                               GValue *value, GParamSpec *pspec);
GstStateChangeReturn gst_lg_audio_render_change_state (GstElement *element,
                                                       GstStateChange transition);

GType gst_lg_audio_render_get_type_once (void);
void  gst_lg_audio_render_init (GstLgAudioRender *pAudioRen);
void  gst_lg_audio_render_class_intern_init (gpointer klass);

/* @0x4410 gst_lg_audio_render_get_type_once */
GType gst_lg_audio_render_get_type_once (void)
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

/* @0x451c gst_lg_audio_render_init */
void gst_lg_audio_render_init (GstLgAudioRender *pAudioRen)
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

    if (access ("/tmp/gstlgaudiosink_dump_enable", F_OK) == 0)
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
     * "es-bypass-done" has no id slot (never emitted from this library). */
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

/* @0x46b0 gst_lg_audio_render_class_intern_init (G_DEFINE_TYPE wrapper) */
void gst_lg_audio_render_class_intern_init (gpointer klass)
{
    parent_class = g_type_class_peek_parent (klass);
    if (GstLgAudioRender_private_offset != 0)
        g_type_class_adjust_private_offset (klass, &GstLgAudioRender_private_offset);
    gst_lg_audio_render_class_init ((GstLgAudioRenderClass *) klass);
}

/* @0x74f0 gst_lg_audio_render_finalize */
void gst_lg_audio_render_finalize (GObject *obj)
{
    GstLgAudioRender *pAudioRen = (GstLgAudioRender *) obj;

    g_cond_clear (&pAudioRen->eos_cond);
    g_mutex_clear (&pAudioRen->eos_mutex);
    g_rec_mutex_clear (&pAudioRen->task_mutex);
    SINK_LOG (3, GST_LEVEL_INFO, 0x2f2, obj, "finalized");
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

extern void gst_lxlog_deinit (void);

#define PREOPEN_BIT(prop_id)  (G_GUINT64_CONSTANT (1) << ((prop_id) - 1))

/*
 * Inlined into change_state (NULL_TO_READY), no symbol; name from the
 * logm __func__ string.  Replays every property whose lgao setter failed
 * (or was skipped) because no lgao handle existed yet.
 */
static void
_gst_lg_audio_render_set_preconditions (GstLgAudioRender *pAudioRen)
{
    guint prop_id;

    for (prop_id = PROP_PLAY_SPEED; prop_id < PROP_VERSION; prop_id++) {
        if (!(pAudioRen->preopen_conditions & PREOPEN_BIT (prop_id)))
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

/* @0xd7c0 gst_lg_audio_render_change_state */
GstStateChangeReturn gst_lg_audio_render_change_state (GstElement *element,
                                                       GstStateChange transition)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (element);
    GstBaseSink *basesink = GST_BASE_SINK (element);
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

        rate = (gint) floor (basesink->segment.rate * 1000.0 + 0.5);
        pAudioRen->currentState = GST_STATE_PLAYING;
        SINK_LOG (2, GST_LEVEL_INFO, 0xbba, pAudioRen, "state: playing");
        lgao_set_playrate (pAudioRen->ao, rate ? rate : 1000);

        now = gst_clock_get_time (element->clock);
        if (pAudioRen->bSeamlessSeek) {
            gst_lg_audio_render_prepare_seamless_seek (GST_BASE_SINK (pAudioRen),
                                                       &basesink->segment, GST_CLOCK_TIME_NONE);
        } else if (!pAudioRen->instant_rate_change) {
            GstClockTime base = now - element->base_time - basesink->segment.base;

            SINK_LOG (3, GST_LEVEL_INFO, 0xbc9, pAudioRen,
                      "base %" GST_TIME_FORMAT "= now %" GST_TIME_FORMAT " element base_time %"
                      GST_TIME_FORMAT " segment base %" GST_TIME_FORMAT,
                      GST_TIME_ARGS (base), GST_TIME_ARGS (now),
                      GST_TIME_ARGS (element->base_time), GST_TIME_ARGS (basesink->segment.base));
            gst_lg_audio_render_update_clock (pAudioRen, base, basesink->segment.start);
        } else {
            /* instant rate change: restart the clock at the video decoder's PTS */
            guint pts = 0;

            lgao_get_vdec_current_pts (pAudioRen->ao, pAudioRen->video_port, &pts);
            pAudioRen->irc_pts = (guint64) (pts / 9) * 100000;      /* 90 kHz -> ns */
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
                    now - element->base_time - basesink->segment.base, basesink->segment.start);
            SINK_CHECK (lgao_play (pAudioRen->ao), 0xc0e);
            SINK_CHECK (lgao_start_renderer (pAudioRen->ao), 0xc0f);
        }
        ret = GST_ELEMENT_CLASS (parent_class)->change_state (element,
                                                              GST_STATE_CHANGE_PLAYING_TO_PLAYING);
        goto done;

    default:
        /* downward transitions: chain up first, then act on the renderer */
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
        goto done;
    }

    ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

done:
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

/* @0x10318 gst_lg_audio_render_get_property */
void gst_lg_audio_render_get_property (GObject *object, guint prop_id,
                                       GValue *value, GParamSpec *pspec)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (object);
    gint fadeVolume = 100;
    guint64 undecodedSize = 0;
    gint ival = 0;           /* one stack slot shared by all lgao getters */
    gchar *contents;

    contents = g_strdup_value_contents (value);
    SINK_LOG (4, GST_LEVEL_DEBUG, 0x7a1, pAudioRen, "get_property \"%s\"(%d) = %s:'%s'",
              pspec->name, prop_id, g_type_name (G_VALUE_TYPE (value)), contents);
    g_free (contents);

    switch (prop_id) {
    case PROP_PLAY_SPEED:
        if (lgao_get_playrate (pAudioRen->ao, &ival) == -1)
            SINK_LOG (0, GST_LEVEL_ERROR, 0x7ab, pAudioRen, "lgao_get_playrate failed!");
        else
            SINK_LOG (3, GST_LEVEL_INFO, 0x7ad, pAudioRen, "current play speed %d", ival);
        g_value_set_int (value, ival);
        break;
    case PROP_DECODED_SIZE:
        /* original bug: queries the mixer mode and returns the (zero)
         * undecoded size; decodedSize is never reported */
        SINK_CHECK (lgao_get_mode_mixer (pAudioRen->ao, &ival), 0x7b4);
        g_value_set_uint64 (value, undecodedSize);
        break;
    case PROP_UNDECODED_SIZE:
        SINK_CHECK (lgao_get_buffer_status (pAudioRen->ao, &undecodedSize), 0x7ba);
        g_value_set_uint64 (value, undecodedSize);
        break;
    case PROP_CURRENT_PTS:
        g_value_set_uint64 (value, pAudioRen->currentPTS);
        break;
    case PROP_LIPSYNC_OFFSET:
        SINK_CHECK (lgao_get_lipsync_offset (pAudioRen->ao, (unsigned int *) &ival), 0x7c4);
        g_value_set_uint (value, ival);
        break;
    case PROP_DISABLE_GSTSYNC:
        g_value_set_boolean (value, pAudioRen->disableBasesinkSync);
        break;
    case PROP_LIPSYNC:
        SINK_CHECK (lgao_get_lipsync_mode (pAudioRen->ao, &ival), 0x7ce);
        g_value_set_boolean (value, ival != 2);     /* 2 = lipsync off */
        break;
    case PROP_NODELAY:
        SINK_CHECK (lgao_get_nodelay (pAudioRen->ao, &ival), 0x7d6);
        g_value_set_boolean (value, ival);
        break;
    case PROP_NODELAY_MODE:
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_OP_MODE, (unsigned int *) &ival),
                    0x7dd);
        g_value_set_uint (value, ival);
        break;
    case PROP_NODELAY_SKIP_UP:
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_SKIPPING,
                                  (unsigned int *) &ival), 0x7e5);
        g_value_set_uint (value, ival);
        break;
    case PROP_NODELAY_FAST_UP:
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_FAST,
                                  (unsigned int *) &ival), 0x7ed);
        g_value_set_uint (value, ival);
        break;
    case PROP_NODELAY_RECOVER:
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_RECOVER,
                                  (unsigned int *) &ival), 0x7f5);
        g_value_set_uint (value, ival);
        break;
    case PROP_NODELAY_FAST_FREQ:
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_FREQ_FAST, (unsigned int *) &ival),
                    0x7fd);
        g_value_set_uint (value, ival);
        break;
    case PROP_RENDER_BUFFER_THRESHOLD:
        SINK_CHECK (lgao_get_rts (pAudioRen->ao, LGAO_RTS_BUFFERING_LEVEL,
                                  (unsigned int *) &ival), 0x805);
        g_value_set_uint (value, ival);
        break;
    case PROP_APP_TYPE:
        SINK_CHECK (lgao_get_apptype (pAudioRen->ao, (LGAO_APPTYPE_T *) &ival), 0x80d);
        switch (ival) {
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
    case PROP_MIXER:
        SINK_CHECK (lgao_get_mode_mixer (pAudioRen->ao, &ival), 0x82d);
        g_value_set_boolean (value, ival);
        break;
    case PROP_AUDIO_INDEX:
        ival = -1;
        SINK_CHECK (lgao_get_port (pAudioRen->ao, &ival), 0x835);
        g_value_set_uint (value, ival);
        break;
    case PROP_AUDIO_ONLY:
        g_value_set_boolean (value, FALSE);
        SINK_LOG (0, GST_LEVEL_ERROR, 0x83d, pAudioRen, "meaningless property");
        break;
    case PROP_SEAMLESS:
        g_value_set_boolean (value, pAudioRen->isSeamless);
        break;
    case PROP_MUTE:
        SINK_CHECK (lgao_get_mute (pAudioRen->ao, &ival), 0x847);
        g_value_set_boolean (value, ival);
        break;
    case PROP_FADE_VOLUME:
        SINK_LOG (3, GST_LEVEL_INFO, 0x84c, pAudioRen, "get ease fadeVolume");
        SINK_CHECK (lgao_get_fade (pAudioRen->ao, LGAO_FADE_PROP_EASE_VOL, &fadeVolume), 0x84d);
        g_value_set_int (value, fadeVolume);
        if (fadeVolume < 0)
            SINK_LOG (3, GST_LEVEL_INFO, 0x851, pAudioRen, "GetVolume Err");
        else
            SINK_LOG (3, GST_LEVEL_INFO, 0x853, pAudioRen, "get fadeVolume = %d", fadeVolume);
        break;
    case PROP_CLOCK_TYPE:
        SINK_LOG (0, GST_LEVEL_ERROR, 0x85a, pAudioRen, "meaningless property get");
        g_value_set_string (value, NULL);
        break;
    case PROP_AUDIO_BE_DELAY:
        g_value_set_uint (value, 85);     /* fixed back-end delay, ms */
        break;
    case PROP_DIRECTMEDIA_TOTAL_BUF_SIZE:
        SINK_CHECK (lgao_get_buffer_size_for_directmedia (pAudioRen->ao, (unsigned int *) &ival,
                                                          LGAO_DIRECTMEDIA_TOTAL_BUF), 0x867);
        g_value_set_int (value, ival);
        break;
    case PROP_DIRECTMEDIA_USED_BUF_SIZE:
        SINK_CHECK (lgao_get_buffer_size_for_directmedia (pAudioRen->ao, (unsigned int *) &ival,
                                                          LGAO_DIRECTMEDIA_USED_BUF), 0x86f);
        g_value_set_int (value, ival);
        break;
    case PROP_PCM_BYPASS:
        SINK_CHECK (lgao_get_pcm_bypass (pAudioRen->ao, &ival), 0x878);
        g_value_set_boolean (value, ival);
        break;
    case PROP_PCM_MULTI_CH:
        SINK_CHECK (lgao_get_pcm_multi_channel (pAudioRen->ao, &ival), 0x880);
        g_value_set_boolean (value, ival);
        break;
    case PROP_GAME_MODE:
        SINK_CHECK (lgao_get_game_mode (pAudioRen->ao, &ival), 0x889);
        g_value_set_boolean (value, ival);
        break;
    case PROP_QUEUE_MAX_LENGTH:
        SINK_CHECK (lgao_get_queue_max_length (pAudioRen->ao, &ival), 0x890);
        g_value_set_uint (value, ival);
        break;
    case PROP_VARIABLE_SPEED_PLAYBACK:
        g_value_set_boolean (value, pAudioRen->vsp_mode);
        break;
    case PROP_VERSION:
        g_value_set_uint (value, AUDIOSINK_VERSION);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

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

static gboolean
is_id_gapless (GstLgAudioRender *pAudioRen)
{
    return pAudioRen->pAppType != NULL && strcmp (pAudioRen->pAppType, "ID-GAPLESS") == 0;
}

/* @0x11138 gst_lg_audio_render_set_property */
void gst_lg_audio_render_set_property (GObject *object, guint prop_id,
                                       const GValue *value, GParamSpec *pspec)
{
    GstLgAudioRender *pAudioRen = GST_LG_AUDIO_RENDER (object);
    gchar *contents;

    contents = g_strdup_value_contents (value);
    SINK_LOG (4, GST_LEVEL_DEBUG, 0x5fb, pAudioRen, "set_property \"%s\"(%d) = %s:'%s'",
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
        SINK_LOG (3, GST_LEVEL_INFO, 0x620, pAudioRen, "Set Audio's lipsync offset to %u",
                  pAudioRen->lipsync_offset);
        SET_OR_DEFER (pAudioRen,
                      lgao_set_lipsync_offset (pAudioRen->ao, pAudioRen->lipsync_offset),
                      PROP_LIPSYNC_OFFSET, 0x622);
        break;

    case PROP_DISABLE_GSTSYNC:
        pAudioRen->disableBasesinkSync = g_value_get_boolean (value);
        if (pAudioRen->disableBasesinkSync) {
            SINK_LOG (3, GST_LEVEL_INFO, 0x629, pAudioRen, "Disable BasepAudioRen's Sync");
            gst_base_sink_set_sync (GST_BASE_SINK (pAudioRen), FALSE);
        }
        break;

    case PROP_LIPSYNC:
        /* Only stored in lgao (ao->lipsyncMode); see report: it is consumed
         * when lgao builds/starts the renderer. */
        pAudioRen->lipsync_mode = g_value_get_boolean (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x62f, pAudioRen, "Set Lipsync Mode to %s",
                  pAudioRen->lipsync_mode ? "ENABLE" : "DISABLE");
        SET_OR_DEFER (pAudioRen,
                      lgao_set_lipsync_mode (pAudioRen->ao, pAudioRen->lipsync_mode
                                             ? LGAO_LIPSYNC_ENABLE : LGAO_LIPSYNC_DISABLE),
                      PROP_LIPSYNC, 0x632);
        break;

    case PROP_NODELAY:
        pAudioRen->nodelay_mode = g_value_get_boolean (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x639, pAudioRen, "Set No Delay Mode to %s",
                  TF (pAudioRen->nodelay_mode));
        SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, pAudioRen->nodelay_mode),
                      PROP_NODELAY, 0x63e);
        break;

    case PROP_NODELAY_MODE:
        pAudioRen->rts_mode = g_value_get_uint (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x643, pAudioRen, "Set RTS OP Mode = %u",
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
        SINK_LOG (3, GST_LEVEL_INFO, 0x64f, pAudioRen, "Set RTS Threshold of Skipping = %u",
                  pAudioRen->nodelay_skip_up);
        break;

    case PROP_NODELAY_FAST_UP:
        pAudioRen->nodelay_fast_up = g_value_get_uint (value);
        SET_OR_DEFER (pAudioRen, lgao_set_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_FAST,
                                               pAudioRen->nodelay_fast_up),
                      PROP_NODELAY_FAST_UP, 0x656);
        SINK_LOG (3, GST_LEVEL_INFO, 0x658, pAudioRen, "Set RTS Threshold of Fast = %u",
                  pAudioRen->nodelay_fast_up);
        break;

    case PROP_NODELAY_RECOVER:
        pAudioRen->nodelay_recover = g_value_get_uint (value);
        SET_OR_DEFER (pAudioRen, lgao_set_rts (pAudioRen->ao, LGAO_RTS_THRESHOLD_RECOVER,
                                               pAudioRen->nodelay_recover),
                      PROP_NODELAY_RECOVER, 0x65f);
        SINK_LOG (3, GST_LEVEL_INFO, 0x661, pAudioRen, "Set RTS Threshold of Recover = %u",
                  pAudioRen->nodelay_recover);
        break;

    case PROP_NODELAY_FAST_FREQ:
        pAudioRen->nodelay_fast_freq = g_value_get_uint (value);
        SET_OR_DEFER (pAudioRen, lgao_set_rts (pAudioRen->ao, LGAO_RTS_FREQ_FAST,
                                               pAudioRen->nodelay_fast_freq),
                      PROP_NODELAY_FAST_FREQ, 0x668);
        SINK_LOG (3, GST_LEVEL_INFO, 0x66a, pAudioRen, "Set RTS Fast Frequency of Fast = %u",
                  pAudioRen->nodelay_fast_freq);
        break;

    case PROP_RENDER_BUFFER_THRESHOLD:
        pAudioRen->render_buf_thres = g_value_get_uint (value);
        SET_OR_DEFER (pAudioRen, lgao_set_rts (pAudioRen->ao, LGAO_RTS_BUFFERING_LEVEL,
                                               pAudioRen->render_buf_thres),
                      PROP_RENDER_BUFFER_THRESHOLD, 0x671);
        SINK_LOG (3, GST_LEVEL_INFO, 0x673, pAudioRen, "Set PROP_RENDER_BUFFER_THRESHOLD = %u",
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
            SINK_LOG (3, GST_LEVEL_INFO, 0x680, pAudioRen, "This application is Airplay Mirroring");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_AIRPLAY),
                          PROP_APP_TYPE, 0x681);
            SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, TRUE), PROP_APP_TYPE, 0x683);
            goto app_done;
        } else if (strcmp (app, "RTC") == 0) {
            SINK_LOG (3, GST_LEVEL_INFO, 0x688, pAudioRen, "This application is WiDi");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_RTC),
                          PROP_APP_TYPE, 0x689);
            SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, TRUE), PROP_APP_TYPE, 0x68b);
        } else if (strcmp (app, "R2R") == 0) {
            SINK_LOG (3, GST_LEVEL_INFO, 0x691, pAudioRen, "This application is R2R");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_R2R),
                          PROP_APP_TYPE, 0x692);
            SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, TRUE), PROP_APP_TYPE, 0x694);
        } else if (strcmp (app, "MIRACAST") == 0) {
            SINK_LOG (3, GST_LEVEL_INFO, 0x698, pAudioRen, "This application is Miracast");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_MIRACAST),
                          PROP_APP_TYPE, 0x699);
            SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, TRUE), PROP_APP_TYPE, 0x69b);
        } else if (strcmp (app, "JP4K") == 0) {
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_JP4K),
                          PROP_APP_TYPE, 0x69f);
            goto app_done;
        } else if (strcmp (app, "DIRECTMEDIA") == 0) {
            SINK_LOG (3, GST_LEVEL_INFO, 0x6a2, pAudioRen,
                      "This application is Cloud Game(DIRECTMEDIA)");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_DIRECTMEDIA),
                          PROP_APP_TYPE, 0x6a3);
            SET_OR_DEFER (pAudioRen, lgao_set_nodelay (pAudioRen->ao, TRUE), PROP_APP_TYPE, 0x6a5);
        } else if (strcmp (app, "ID-GAPLESS") == 0) {
            SINK_LOG (3, GST_LEVEL_INFO, 0x6a9, pAudioRen, "This application is for ID-GAPLESS PLAY");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_IDGAPLESS),
                          PROP_APP_TYPE, 0x6aa);
            goto app_done;
        } else if (strcmp (app, "APPCAST") == 0) {
            SINK_LOG (3, GST_LEVEL_INFO, 0x6ad, pAudioRen, "This application is for APPCAST");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_APPCAST),
                          PROP_APP_TYPE, 0x6ae);
            goto app_done;
        } else if (strcmp (app, "SYE") == 0) {
            SINK_LOG (3, GST_LEVEL_INFO, 0x6b1, pAudioRen, "This application is for AMAZON SYE");
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_SYE),
                          PROP_APP_TYPE, 0x6b2);
        } else {
            SET_OR_DEFER (pAudioRen, lgao_set_apptype (pAudioRen->ao, LGAO_APPTYPE_UNKNOWN),
                          PROP_APP_TYPE, 0x6b6);
            goto app_done;
        }
        gst_base_sink_set_sync (GST_BASE_SINK (pAudioRen), FALSE);
app_done:
        SINK_LOG (3, GST_LEVEL_INFO, 0x6ba, pAudioRen, "Apptype %s received", pAudioRen->pAppType);
        break;
    }

    case PROP_RESOURCE_INFO: {
        const GstStructure *st = gst_value_get_structure (value);

        if (st == NULL) {
            SINK_LOG (0, GST_LEVEL_ERROR, 0x6d9, pAudioRen,
                      "resource-info has not mixer-port field");
        } else {
            if (gst_structure_has_field (st, "audio-port")) {
                gst_structure_get_int (st, "audio-port", &pAudioRen->audio_port);
                SINK_LOG (5, GST_LEVEL_LOG, 0x6c9, pAudioRen, "audio-port is %d",
                          pAudioRen->audio_port);
            }
            if (gst_structure_has_field (st, "video-port")) {
                gst_structure_get_int (st, "video-port", &pAudioRen->video_port);
                SINK_LOG (2, GST_LEVEL_INFO, 0x6d0, pAudioRen, "video-port is %d",
                          pAudioRen->video_port);
            }
            if (gst_structure_has_field (st, "mixer-port")) {
                gst_structure_get_int (st, "mixer-port", &pAudioRen->mixer_port);
                SINK_LOG (5, GST_LEVEL_LOG, 0x6d6, pAudioRen, "mixer-port is %d",
                          pAudioRen->mixer_port);
            }
        }
        SINK_LOG (3, GST_LEVEL_INFO, 0x6db, pAudioRen, "audio-port %d, video-port %d,mixer-port %d",
                  pAudioRen->audio_port, pAudioRen->video_port, pAudioRen->mixer_port);
        /* audio-port selects /dev/lg/aren<N> (renderer index); see lgao_set_port */
        SET_OR_DEFER (pAudioRen, lgao_set_port (pAudioRen->ao, pAudioRen->audio_port,
                                                pAudioRen->mixer_port),
                      PROP_RESOURCE_INFO, 0x6dd);
        break;
    }

    case PROP_MIXER:
        pAudioRen->mode_mixer = g_value_get_boolean (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x6e3, pAudioRen,
                  "Set using Mixer(TRUE=Mixer, FALSE=Decoder): %s", TF (pAudioRen->mode_mixer));
        SET_OR_DEFER (pAudioRen, lgao_set_mode_mixer (pAudioRen->ao, pAudioRen->mode_mixer),
                      PROP_MIXER, 0x6e5);
        break;

    case PROP_AUDIO_INDEX:
        if (pAudioRen->mode_mixer == 1)
            pAudioRen->mixer_port = g_value_get_uint (value);
        else
            pAudioRen->audio_port = g_value_get_uint (value);
        SINK_LOG (2, GST_LEVEL_INFO, 0x6ed, pAudioRen, "Set %s index to %u",
                  pAudioRen->mode_mixer ? "MIX" : "AUD",
                  pAudioRen->mode_mixer ? pAudioRen->mixer_port : pAudioRen->audio_port);
        SET_OR_DEFER (pAudioRen, lgao_set_port (pAudioRen->ao, pAudioRen->audio_port,
                                                pAudioRen->mixer_port),
                      PROP_AUDIO_INDEX, 0x6f1);
        break;

    case PROP_AUDIO_ONLY: {
        gboolean audio_only = g_value_get_boolean (value);

        SINK_LOG (0, GST_LEVEL_ERROR, 0x6f8, pAudioRen, "meaningless property");
        SINK_LOG (3, GST_LEVEL_INFO, 0x6f9, pAudioRen,
                  "Set Audio Only Stream(TRUE=Audio Only, FALSE=Audio and Video): %s",
                  TF (audio_only));
        break;
    }

    case PROP_SEAMLESS:
        pAudioRen->isSeamless = g_value_get_boolean (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x700, pAudioRen,
                  "Set Seamless audio change(TRUE=Enable, FALSE=Disable): %s",
                  TF (pAudioRen->isSeamless));
        break;

    case PROP_MUTE:
        pAudioRen->mode_mute = g_value_get_boolean (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x707, pAudioRen, "Set sound-mute to %s",
                  TF (pAudioRen->mode_mute));
        SET_OR_DEFER (pAudioRen, lgao_set_mute (pAudioRen->ao, pAudioRen->mode_mute),
                      PROP_MUTE, 0x709);
        break;

    case PROP_FADE: {
        const GstStructure *st;
        gint targetVolume = 0, duration = 0;
        const gchar *fadeType;

        SINK_LOG (3, GST_LEVEL_INFO, 0x714, pAudioRen, "ease volume setting start");
        st = gst_value_get_structure (value);
        if (st == NULL) {
            SINK_LOG (0, GST_LEVEL_ERROR, 0x747, pAudioRen, "ease volume fadeStructure is NULL");
            break;
        }
        if (!gst_structure_has_field (st, "target-volume") ||
            !gst_structure_has_field (st, "duration-in-ms") ||
            !gst_structure_has_field (st, "fade")) {
            SINK_LOG (0, GST_LEVEL_ERROR, 0x744, pAudioRen, "ease volume params are NULL");
            break;
        }
        gst_structure_get_int (st, "target-volume", &targetVolume);
        gst_structure_get_int (st, "duration-in-ms", &duration);
        fadeType = gst_structure_get_string (st, "fade");
        SINK_LOG (3, GST_LEVEL_INFO, 0x720, pAudioRen, "audio ease volume value = %d",
                  targetVolume);
        SINK_LOG (3, GST_LEVEL_INFO, 0x721, pAudioRen, "audio ease volume duration = %d",
                  duration);
        SINK_LOG (3, GST_LEVEL_INFO, 0x722, pAudioRen, "audio ease volume fadeType = %s",
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
            SINK_LOG (0, GST_LEVEL_ERROR, 0x73d, pAudioRen, "Unkown fadeType");
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
            SINK_LOG (3, GST_LEVEL_INFO, 0x755, pAudioRen, "Set %s Type", pAudioRen->clocktype);
            SET_OR_DEFER (pAudioRen, lgao_set_clocktype (pAudioRen->ao, LGAO_CLOCK_TYPE_ATSC3),
                          PROP_CLOCK_TYPE, 0x756);
        } else {
            SET_OR_DEFER (pAudioRen, lgao_set_clocktype (pAudioRen->ao, LGAO_CLOCK_TYPE_GSTC),
                          PROP_CLOCK_TYPE, 0x759);
        }
        break;

    case PROP_PCM_BYPASS:
        pAudioRen->pcm_bypass = g_value_get_boolean (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x762, pAudioRen, "Set pcm (multi channel) bypass %s",
                  TF (pAudioRen->pcm_bypass));
        SET_OR_DEFER (pAudioRen, lgao_set_pcm_bypass (pAudioRen->ao, pAudioRen->pcm_bypass),
                      PROP_PCM_BYPASS, 0x764);
        break;

    case PROP_PCM_MULTI_CH:
        pAudioRen->pcm_multi_ch = g_value_get_boolean (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x76b, pAudioRen, "Set pcm (multi channel) bypass %s",
                  TF (pAudioRen->pcm_multi_ch));
        SET_OR_DEFER (pAudioRen,
                      lgao_set_pcm_multi_channel (pAudioRen->ao, pAudioRen->pcm_multi_ch),
                      PROP_PCM_MULTI_CH, 0x76d);
        break;

    case PROP_GAME_MODE:
        pAudioRen->game_mode = g_value_get_boolean (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x774, pAudioRen, "Set game mode %s",
                  pAudioRen->game_mode ? "Enable" : "Disable");
        SET_OR_DEFER (pAudioRen, lgao_set_game_mode (pAudioRen->ao, pAudioRen->game_mode),
                      PROP_GAME_MODE, 0x776);
        break;

    case PROP_QUEUE_MAX_LENGTH:
        pAudioRen->queue_max_length = g_value_get_uint (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x77d, pAudioRen, "Set PROP_QUEUE_MAX_LENGTH = %u",
                  pAudioRen->queue_max_length);
        if (pAudioRen->queue_max_length <= 0x4000)
            SET_OR_DEFER (pAudioRen,
                          lgao_set_queue_max_length (pAudioRen->ao, pAudioRen->queue_max_length),
                          PROP_QUEUE_MAX_LENGTH, 0x781);
        break;

    case PROP_VARIABLE_SPEED_PLAYBACK:
        pAudioRen->vsp_mode = g_value_get_boolean (value);
        SINK_LOG (3, GST_LEVEL_INFO, 0x789, pAudioRen,
                  "Set Netflix VSP(Variable Speed Playback) mode %s", TF (pAudioRen->vsp_mode));
        if (pAudioRen->vsp_mode)
            SET_OR_DEFER (pAudioRen, lgao_set_vsp_mode (pAudioRen->ao),
                          PROP_VARIABLE_SPEED_PLAYBACK, 0x78d);
        break;

    default:
        /* includes the read-only ids (undecoded-size, current-pts, fade-volume, ...) */
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

/* @0x14120 gst_lg_audio_render_plugin_init */
gboolean gst_lg_audio_render_plugin_init (GstPlugin *plugin)
{
    return gst_element_register (plugin, "audiosink", GST_RANK_SECONDARY,
                                 GST_TYPE_LG_AUDIO_RENDER);
}

/* Expansion of GST_PLUGIN_DEFINE (static build) against LG's GStreamer 1.24. */
#define LG_GST_VERSION_MINOR 24
static const GstPluginDesc gst_plugin_desc = {
    GST_VERSION_MAJOR, LG_GST_VERSION_MINOR, "lgaudiosink", "LG's audio sink element",
    gst_lg_audio_render_plugin_init, "1.0", "Proprietary", "gst-plugins-bsp",
    "LG Media Framework", "www.lge.com", NULL, { NULL }
};

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
