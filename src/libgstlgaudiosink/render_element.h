/*
 * GstLgAudioRender: the "audiosink" element of libgstlgaudiosink
 * (gst-plugins-bsp/gstlgaudio/gstlgaudiosink.c).
 * RECONSTRUCTION from usr/lib/gstreamer-1.0/libgstlgaudiosink.so
 *   sha256 90dba3ef32704ce4096bf1e6045d2879d0c2192d507bc04d1e8e98437f420a43
 * Field, enum and type names come from the binary's DWARF (gstlgaudiosink.c CU);
 * offsets are checked with _Static_assert below.
 *
 * LG ships a patched GStreamer 1.24 whose GstBaseSink/GstBaseSinkClass are larger
 * than the stock 1.20 headers used to build this (sizeof 0x200 / 0x19c). Only the
 * fields up to max_lateness (segment, sinkpad, need_preroll, ...) are used and
 * those share the upstream layout, so the parent is padded instead of redefined.
 */
#ifndef RENDER_ELEMENT_H
#define RENDER_ELEMENT_H

#include <stdio.h>
#include <gst/gst.h>
#include <gst/base/gstbasesink.h>

G_BEGIN_DECLS

#define GST_TYPE_LG_AUDIO_RENDER   (gst_lg_audio_render_get_type ())
#define GST_LG_AUDIO_RENDER(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_LG_AUDIO_RENDER, GstLgAudioRender))

#define LG_BASE_SINK_SIZE        0x200
#define LG_BASE_SINK_CLASS_SIZE  0x19c

typedef struct _GstLgAudioRender      GstLgAudioRender;
typedef struct _GstLgAudioRenderClass GstLgAudioRenderClass;

/* Vfuncs LG appended to GstBaseSinkClass (they replace _gst_reserved[0..2]). */
typedef struct {
    guint8 upstream[0x140];
    gboolean (*prepare_seamless_seek) (GstBaseSink *sink, GstSegment *segment,
                                       GstClockTime new_base_time);
    gboolean (*prepare_instant_rate_change) (GstBaseSink *sink, gdouble rate_multiplier);
    gboolean (*prepared_instant_rate_change) (GstBaseSink *sink, gdouble rate_multiplier);
} GstLgBaseSinkClassExt;

#define GST_LG_BASE_SINK_CLASS_EXT(k) ((GstLgBaseSinkClassExt *) GST_BASE_SINK_CLASS (k))

struct _GstLgAudioRender {
    union {
        GstBaseSink parent;
        guint8      _lg_parent[LG_BASE_SINK_SIZE];
    };

    void       *ao;                    /* lgao handle (lgao_open) */
    gboolean    isSeamless;            /* "seamless": suppress underrun signal */
    guint64     decodedSize;
    guint64     currentPTS;            /* last buffer PTS (ns) */
    GstState    currentState;
    GstState    nextState;
    gboolean    disableBasesinkSync;
    char       *pAppType;
    gboolean    isUnsupportedStream;   /* caps freq > 384k or > 10 ch */
    GstCaps    *pAudioCaps;
    GstTask    *task;                  /* gst_lg_audio_render_callback_task */
    GRecMutex   task_mutex;
    gchar      *pStreamId;
    gboolean    isFlushing;
    gboolean    bSeamlessSeek;
    gdouble     playrate;              /* segment.rate of the last SEGMENT */
    GMutex      eos_mutex;
    GCond       eos_cond;              /* signalled on LGAO_EVENT_PRESENT_END */
    int         event_mq_id;
    gboolean    event_mq_running;
    int         audio_port;
    int         video_port;
    int         mixer_port;
    gboolean    object_ui_changed;
    gint        lipsync_offset;        /* ms */
    gint        rts_mode;
    gboolean    nodelay_mode;
    gboolean    lipsync_mode;
    gboolean    enable_dump;
    gint        dump_index;
    FILE       *dump_file;
    guint64     preopen_conditions;    /* bit (prop_id - 1): setter failed before open */
    gint        play_speed;
    guint       nodelay_skip_up;
    guint       nodelay_fast_up;
    guint       nodelay_recover;
    guint       nodelay_fast_freq;
    gint        mode_mixer;
    gint        mode_mute;
    gchar      *clocktype;
    gboolean    bClockCalibrated;
    gint64      segment_start;
    gint64      segment_position;
    gint32      base_delay;            /* ms */
    gint32      render_delay;          /* ms */
    gboolean    instant_rate_change;
    guint64     irc_pts;
    gboolean    avail_irc_vdec_pts;
    gboolean    pcm_bypass;
    gboolean    pcm_multi_ch;
    gboolean    game_mode;
    gint        render_buf_thres;
    guint       queue_max_length;
    gboolean    vsp_mode;
};

struct _GstLgAudioRenderClass {
    union {
        GstBaseSinkClass      parent_class;
        GstLgBaseSinkClassExt lg;
        guint8                _lg_parent[LG_BASE_SINK_CLASS_SIZE];
    };
    void (*underrun) (GstLgAudioRender *self);
    void (*clock_calibrated) (GstLgAudioRender *self);
    void (*es_bypass_done) (GstLgAudioRender *self);
};

enum {
    PROP_0,
    PROP_PLAY_SPEED,
    PROP_DECODED_SIZE,
    PROP_UNDECODED_SIZE,
    PROP_CURRENT_PTS,
    PROP_LIPSYNC_OFFSET,
    PROP_DISABLE_GSTSYNC,
    PROP_LIPSYNC,
    PROP_NODELAY,
    PROP_NODELAY_MODE,
    PROP_NODELAY_SKIP_UP,
    PROP_NODELAY_FAST_UP,
    PROP_NODELAY_RECOVER,
    PROP_NODELAY_FAST_FREQ,
    PROP_APP_TYPE,
    PROP_RESOURCE_INFO,
    PROP_MIXER,
    PROP_AUDIO_INDEX,
    PROP_AUDIO_ONLY,
    PROP_SEAMLESS,
    PROP_MUTE,
    PROP_FADE,
    PROP_FADE_VOLUME,
    PROP_CLOCK_TYPE,
    PROP_AUDIO_BE_DELAY,
    PROP_DIRECTMEDIA_TOTAL_BUF_SIZE,
    PROP_DIRECTMEDIA_USED_BUF_SIZE,
    PROP_PCM_BYPASS,
    PROP_PCM_MULTI_CH,
    PROP_GAME_MODE,
    PROP_RENDER_BUFFER_THRESHOLD,
    PROP_QUEUE_MAX_LENGTH,
    PROP_VARIABLE_SPEED_PLAYBACK,
    PROP_VERSION,
    PROP_LAST
};

enum {
    SIGNAL_UNDERRUN,
    SIGNAL_CLOCK_CALIBRATED,
    LAST_SIGNAL
};

#if defined(__arm__) && !defined(__aarch64__)
_Static_assert(sizeof(GstLgAudioRender) == 0x318, "GstLgAudioRender size");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioRender, ao) == 0x200, "ao");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioRender, playrate) == 0x250, "playrate");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioRender, preopen_conditions) == 0x2a0, "preopen");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioRender, segment_start) == 0x2d0, "segment_start");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioRender, irc_pts) == 0x2f0, "irc_pts");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioRender, vsp_mode) == 0x310, "vsp_mode");
_Static_assert(G_STRUCT_OFFSET(GstBaseSink, sinkpad) == 0xb8, "sinkpad");
_Static_assert(G_STRUCT_OFFSET(GstBaseSink, need_preroll) == 0xe8, "need_preroll");
_Static_assert(G_STRUCT_OFFSET(GstBaseSink, segment) == 0xf8, "segment");
_Static_assert(G_STRUCT_OFFSET(GstElement, current_state) == 0x60, "current_state");
_Static_assert(G_STRUCT_OFFSET(GstElement, clock) == 0x74, "clock");
_Static_assert(G_STRUCT_OFFSET(GstElement, base_time) == 0x78, "base_time");
_Static_assert(G_STRUCT_OFFSET(GstBaseSinkClass, render) == 0x138, "render vfunc");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioRenderClass, underrun) == 0x19c, "underrun");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioRenderClass, es_bypass_done) == 0x1a4, "bypass");
_Static_assert(sizeof(GstLgAudioRenderClass) == 0x1a8, "class size");
#endif

GType gst_lg_audio_render_get_type (void);

/* ---- shared element globals (defined in render_props.c) ----------------- */
#ifndef LOGMM_MODULE_DEFINED
#define LOGMM_MODULE_DEFINED
struct logmm_module {
    int index;
    unsigned int flags;
    unsigned int _mask;
    unsigned int *mask;
    const char *name;
};
#endif
extern unsigned int logmm_dummy_mask;
extern int logmm_printl (struct logmm_module *m, int lvl, const char *func, int line,
                         const char *fmt, ...);

extern struct logmm_module logmm_module_media_event;
extern GstDebugCategory   *gst_lg_audio_render_debug;
extern gpointer            parent_class;
extern guint               gst_lg_audio_render_signals[LAST_SIGNAL];

#define LG_SINK_FILE "../../git/gst-plugins-bsp/../gstlgaudio/gstlgaudiosink.c"

/* Every element log goes to both liblogm ("media_event", <audio_sink> prefix)
 * and the GStreamer category; the two levels are chosen independently.
 * line = __LINE__ of the original gstlgaudiosink.c, kept as evidence. */
#define SINK_LOGM(mlvl, line, fmt, ...) do { \
        if (G_UNLIKELY (*logmm_module_media_event.mask & (1u << (mlvl)))) \
            logmm_printl (&logmm_module_media_event, (mlvl), __func__, (line), \
                          "<audio_sink> " fmt, ##__VA_ARGS__); \
    } while (0)
#define SINK_GST(glvl, line, obj, fmt, ...) do { \
        if (G_UNLIKELY ((glvl) <= _gst_debug_min)) \
            gst_debug_log (gst_lg_audio_render_debug, (glvl), LG_SINK_FILE, __func__, (line), \
                           (GObject *) (obj), fmt, ##__VA_ARGS__); \
    } while (0)
#define SINK_LOG(mlvl, glvl, line, obj, fmt, ...) do { \
        SINK_LOGM (mlvl, line, fmt, ##__VA_ARGS__); \
        SINK_GST (glvl, line, obj, fmt, ##__VA_ARGS__); \
    } while (0)
/* lgao call whose failure is only reported to the GStreamer log */
#define SINK_CHECK(call, line) do { \
        if ((call) != 0) \
            SINK_GST (GST_LEVEL_WARNING, line, NULL, #call " failed!"); \
    } while (0)

/* ---- render_element.c (data flow); installed by class_init ------------- */
void          gst_lg_audio_render_clock_calibrated (GstLgAudioRender *pAudioRen);
void          gst_lg_audio_render_update_ts_offset (GstLgAudioRender *pAudioRen, gint playspeed);
void          gst_lg_audio_render_update_clock (GstLgAudioRender *pAudioRen, gint64 base,
                                                gint64 start);
gboolean      gst_lg_audio_render_prepared_instant_rate_change (GstBaseSink *sink,
                                                                gdouble rate_multiplier);
void          gst_lg_audio_render_dump_open (GstLgAudioRender *pAudioRen);
void          gst_lg_audio_render_callback_task (void *data);
GstClock     *gst_lg_audio_render_provide_clock (GstElement *element);
gboolean      gst_lg_audio_render_prepare_seamless_seek (GstBaseSink *sink, GstSegment *segment,
                                                         GstClockTime new_base_time);
GstFlowReturn gst_lg_audio_render_preroll (GstBaseSink *sink, GstBuffer *buffer);
gboolean      gst_lg_audio_render_event (GstBaseSink *sink, GstEvent *event);
GstFlowReturn gst_lg_audio_render_chain (GstBaseSink *sink, GstBuffer *buffer);
gboolean      gst_lg_audio_render_stop (GstBaseSink *sink);
gboolean      gst_lg_audio_render_start (GstBaseSink *sink);
gboolean      gst_lg_audio_render_query (GstBaseSink *sink, GstQuery *query);

G_END_DECLS

#endif
