/*
 * Common instance struct + shared helpers of the 12 LG audio decoder elements
 * (aac_/ac3_/ac4_/adpcm_/amr_/flac_/mpegh_/pcm_/vorbis_/wma_/opus_/iamf_audiodec).
 *
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so
 *   sha256 84db367e1ea3eeb4b589bf63910140cfd43ea77f2db124ae6882fbee8ebb6bb7
 * Layout from DWARF (struct _GstLgAudioDec, size 0x368; _GstLgAudioDecClass, 0xfc).
 * Note: the instance embeds a GstAudioDecoder but every element type is registered as a
 * plain GstElement subclass (g_type_register_static_simple(gst_element_get_type(), ...,
 * 0xfc, ..., 0x368, ...) @0x21828); the GstAudioDecoder part is never initialised.
 */
#ifndef GST_LG_AUDIODEC_H
#define GST_LG_AUDIODEC_H

#include <gst/gst.h>
#include <gst/audio/audio.h>
#include <gst/audio/gstaudiodecoder.h>
#include "lgadec.h"

G_BEGIN_DECLS

typedef enum {
    DECODER_ELEMENT_STATUS_INIT,
    DECODER_ELEMENT_STATUS_OK,
    DECODER_ELEMENT_STATUS_ERR,
    DECODER_ELEMENT_STATUS_UNSUPPORTED,
} DECODER_ELEMENT_STATUS_T;

enum { INPUT_STREAM_UNKNOWN = -1, INPUT_STREAM_NON_ATMOS = 0, INPUT_STREAM_ATMOS = 1 };

enum {
    PROP_0,
    PROP_DECODED_SIZE,                 /* "decoded-size"                         */
    PROP_UNDECODED_SIZE,               /* "undecoded-size"                       */
    PROP_TRICK_MODE,                   /* "serverside-trickplay"                 */
    PROP_APP_TYPE,                     /* "app-type"                             */
    PROP_RESOURCE_INFO,                /* "resource-info"   (audio-port)         */
    PROP_INDEX,                        /* "index"                                */
    PROP_SEAMLESS,                     /* "seamless"                             */
    PROP_MPEG_DASH,                    /* "mpeg-dash"                            */
    PROP_PLAY_SPEED,                   /* "play-speed"                           */
    PROP_OUTPUT_MODE,                  /* "dual-mono"                            */
    PROP_VERSION,                      /* "version"                              */
    PROP_MPEGH_UI_XML,                 /* "mpegh-ui-xml" (mpegh_audiodec only)   */
    PROP_DEFAULT_PRL,                  /* "default-prl"                          */
    PROP_DIRECTMEDIA_TOTAL_BUF_SIZE,
    PROP_DIRECTMEDIA_USED_BUF_SIZE,
    PROP_AC4_DE_GAIN,                  /* "dialog-enhancement-gain"              */
    PROP_AC4_DE_LIMIT_MIN,
    PROP_AC4_DE_LIMIT_MAX,
    PROP_ES_BYPASS,                    /* "es-bypass"  -> lgadec_set_es_bypass   */
    PROP_DOLBY_DRC_MODE,               /* "drc-mode"                             */
    PROP_DOWNMIX,                      /* "downmix"                              */
    PROP_SOUND_SYSTEM,                 /* "sound-system" (IAMF)                  */
    PROP_LAST,
};

/* LG-private GstBuffer flags on the src pad (GST_BUFFER_FLAG_LAST == 1<<20).
 * Consumer: gst_lg_audio_render_chain in libgstlgaudiosink. */
#define GST_BUFFER_FLAG_LG_METADATA   (GST_BUFFER_FLAG_LAST << 0)  /* 0x00100000 LGADEC_DATA_METADATA  */
#define GST_BUFFER_FLAG_LG_ES         (GST_BUFFER_FLAG_LAST << 1)  /* 0x00200000 LGADEC_DATA_ES        */
#define GST_BUFFER_FLAG_MPEGH_UI      (GST_BUFFER_FLAG_LAST << 2)  /* 0x00400000 */
#define GST_BUFFER_FLAG_LG_METADATA2  (GST_BUFFER_FLAG_LAST << 3)  /* 0x00800000 LGADEC_DATA_METADATA2 */
#define GST_BUFFER_FLAG_LG_BYPASS     (GST_BUFFER_FLAG_LAST << 4)  /* 0x01000000 LGADEC_DATA_BYPASS    */
#define GST_BUFFER_FLAG_NULL_DATA     (GST_BUFFER_FLAG_LAST << 5)  /* 0x02000000 preroll filler        */

typedef struct _GstLgAudioDec {
    GstAudioDecoder parent;                  /* 0x000 */
    GstPad *pSrcPad;                         /* 0x1f8 */
    GstPad *pSinkPad;                        /* 0x1fc */
    DECODER_ELEMENT_STATUS_T elementStatus;  /* 0x200 */
    void *adec;                              /* 0x204 lgadec handle */
    guint64 feededFrameCount;                /* 0x208 */
    GstState currentState;                   /* 0x210 */
    GstState nextState;                      /* 0x214 */
    GstSegment segment;                      /* 0x218 */
    GstCaps *pAudioCaps;                     /* 0x280 last sink caps */
    gboolean isActive;
    gchar *mime;                             /* 0x288 */
    gboolean isMpeghUiUpdated;               /* 0x28c */
    gint isAtmos;
    guint32 index;                           /* 0x294 */
    pthread_mutex_t cb_mutex;                /* 0x298 serialises the lgadec callback */
    pthread_mutex_t adec_mutex;              /* 0x2b0 */
    guint64 decodedSize;                     /* 0x2c8 bytes received on the sink pad */
    gboolean isTrickPlay;                    /* 0x2d0 */
    gchar *appType;                          /* 0x2d4 */
    gboolean is_device_iamf_supported;
    guint channel_capa;
    gboolean isSeamless;                     /* 0x2e0 */
    gboolean isDTSSeamless;                  /* 0x2e4 never referenced by code */
    gboolean isMpegDash;                     /* 0x2e8 */
    gboolean isDtsCoreless;                  /* 0x2ec never referenced by code */
    gboolean sendPrerollBuffer;
    gboolean isVorbisCodec;                  /* 0x2f4 */
    gboolean need_gap;                       /* 0x2f8 unsupported codec: push GAPs only */
    gboolean set_push_gap;
    gboolean isEnableDumpInput;              /* 0x300 */
    FILE *pDumpAudioFd;
    gchar *pStreamId;                        /* 0x308 */
    gchar *pFormatString;
    gboolean isDecoderReset;
    gboolean isFlush;                        /* 0x314 */
    guint8 *pcm_buffer;
    guint32 pcm_rd_offset;
    guint32 pcm_wr_offset;
    gboolean bStreamStart;                   /* 0x324 */
    gboolean bIgnoreEOS;
    gboolean bWaitEOS;                       /* 0x32c */
    GCond stream_start_cond;                 /* 0x330 */
    GMutex stream_start_mutex;               /* 0x33c */
    GstClockTime in_timestamp;               /* 0x348 */
    GstClockTime last_in_timestamp;          /* 0x350 */
    GstEvent *pending_segment;               /* 0x358 */
    gboolean first_data_send;                /* 0x35c */
    gboolean restart;                        /* 0x360 */
} GstLgAudioDec;

typedef struct _GstLgAudioDecClass {
    GstElementClass parent_class;            /* 0x00 */
    void (*dialog_enhancement_limit_change)(GstLgAudioDec *pAudioDec); /* 0xf8 */
} GstLgAudioDecClass;

typedef struct _AUDIO_MIME_MAP_T {
    const gchar *mime;
    GstFlowReturn (*getCaps)(const GstStructure *s, LGADEC_CODEC_INFO_T *codec_info);
} AUDIO_MIME_MAP_T;

GST_DEBUG_CATEGORY_EXTERN(gst_lg_audiodec_debug);

/* gstlgaudiodec.c: shared element body, called by each per-codec element */
void gst_lg_adec_class_init(GObjectClass *gobject_class);
void gst_lg_adec_init(GstLgAudioDec *pAudioDec);
GstFlowReturn gst_lg_adec_chain(GstLgAudioDec *pAudioDec, GstBuffer *buffer);
gboolean gst_lg_adec_downstream_event(GstLgAudioDec *pAudioDec, GstEvent *event);
gboolean gst_lg_adec_upstream_event(GstLgAudioDec *pAudioDec, GstEvent *event);
gboolean gst_lg_adec_set_property(GstLgAudioDec *pAudioDec, guint prop_id, const GValue *value,
                                  GParamSpec *pspec);
gboolean gst_lg_adec_get_property(GstLgAudioDec *pAudioDec, guint prop_id, GValue *value,
                                  GParamSpec *pspec);
void gst_lg_adec_change_state_up(GstLgAudioDec *pAudioDec, GstStateChange transition);
void gst_lg_adec_change_state_down(GstLgAudioDec *pAudioDec, GstStateChange transition);

/* per-codec getinfo (mime map targets) */
GstFlowReturn gst_lg_audiodec_ac3_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_eac3_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_private_ac3_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_ac4_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_adpcm_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_amrwb_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_amrnb_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_flac_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_mpegh_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_aac_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_opus_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_vorbis_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_wma_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_pcm_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_dvdwifi_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_bdwifi_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_wifi_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_mulaw_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_alaw_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);
GstFlowReturn gst_lg_audiodec_iamf_getinfo(const GstStructure *s, LGADEC_CODEC_INFO_T *ci);

GType gst_lg_audiodec_ac3_get_type(void);
GType gst_lg_audiodec_mpegh_get_type(void);

G_END_DECLS
#endif /* GST_LG_AUDIODEC_H */
