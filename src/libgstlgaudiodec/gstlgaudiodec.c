/*
 * Shared element body of the LG audio decoders (gst-plugins-bsp/gstlgaudio/gstlgaudiodec.c):
 * property table (incl. "es-bypass"), sink CAPS/EOS/SEGMENT handling, chain, the lgadec
 * callback that pushes buffers/caps on the src pad, and state changes. Each per-codec
 * element (gstlgaudiodec_<codec>.c) forwards its pad/GObject vfuncs to these.
 *
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so
 *   sha256 84db367e1ea3eeb4b589bf63910140cfd43ea77f2db124ae6882fbee8ebb6bb7
 * Names/prototypes/layouts: DWARF (CU gstlgaudiodec.c). Line numbers in log calls are the
 * originals (gst_debug_log/logmm_printl arguments). Tags are Ghidra addresses.
 * Evidence: decomp/libgstlgaudiodec.so.c (Ghidra), decomp/libgstlgaudiodec.so.strings.tsv.
 *
 * Also here: the host-side PCM path for codecs without a DSP decoder (lgadec no_dec:
 * LPCM/DVD/BD/Wi-Fi LPCM, G.711, ADPCM, helpers in lgadec_codec.c), the
 * GstAudioClippingMeta front/back trimming, and the optional input dump
 * (/tmp/gst_dump_audio_input -> /tmp/audiodec_ES_ALL_Original-*.dump).
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gstlgaudiodec.h"

_Static_assert(sizeof(GstLgAudioDec) == 0x368, "GstLgAudioDec size (DWARF)");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioDec, adec) == 0x204, "adec");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioDec, isFlush) == 0x314, "isFlush");
_Static_assert(G_STRUCT_OFFSET(GstLgAudioDec, restart) == 0x360, "restart");
_Static_assert(sizeof(GstLgAudioDecClass) == 0xfc, "class size");

GST_DEBUG_CATEGORY(gst_lg_audiodec_debug);
#define GST_CAT_DEFAULT gst_lg_audiodec_debug

extern void gst_lxlog_init(void);
extern void gst_lxlog_deinit(void);
struct logmm_module logmm_module_media_event = { 0, 0, 15, &logmm_dummy_mask, "media_event" };

/* GST_* log + the "<lgaudiodec> " copy to logm media_event (both carry the same line) */
#define MEDIA_LOG(line, fmt, ...) do { \
        LOGM(logmm_module_media_event, log_level_noti, line, "<lgaudiodec> " fmt, ##__VA_ARGS__); \
        GST_INFO(fmt, ##__VA_ARGS__); \
    } while (0)

/* Ordered by strstr() precedence; table @0x3e84c (22 entries). There is no DTS entry:
 * audio/x-dts never matches and the element reports "Unknown MIME". */
static const AUDIO_MIME_MAP_T _g_audio_mime_map[] = {
    { "x-ac3", gst_lg_audiodec_ac3_getinfo },
    { "x-eac3", gst_lg_audiodec_eac3_getinfo },
    { "x-private1-ac3", gst_lg_audiodec_private_ac3_getinfo },
    { "x-ac4", gst_lg_audiodec_ac4_getinfo },
    { "x-adpcm", gst_lg_audiodec_adpcm_getinfo },
    { "AMR-WB", gst_lg_audiodec_amrwb_getinfo },
    { "AMR", gst_lg_audiodec_amrnb_getinfo },
    { "x-flac", gst_lg_audiodec_flac_getinfo },
    { "mpeg-h", gst_lg_audiodec_mpegh_getinfo },
    { "mpeg", gst_lg_audiodec_aac_getinfo },
    { "x-opus", gst_lg_audiodec_opus_getinfo },
    { "x-vorbis", gst_lg_audiodec_vorbis_getinfo },
    { "x-wma", gst_lg_audiodec_wma_getinfo },
    { "x-private-lg-lpcm", gst_lg_audiodec_pcm_getinfo },
    { "x-lpcm-1", gst_lg_audiodec_dvdwifi_getinfo },
    { "x-lpcm", gst_lg_audiodec_pcm_getinfo },
    { "x-private1-lpcm", gst_lg_audiodec_dvdwifi_getinfo },
    { "x-private-ts-lpcm", gst_lg_audiodec_bdwifi_getinfo },
    { "x-private2-lpcm", gst_lg_audiodec_wifi_getinfo },
    { "x-mulaw", gst_lg_audiodec_mulaw_getinfo },
    { "x-alaw", gst_lg_audiodec_alaw_getinfo },
    { "x-iamf", gst_lg_audiodec_iamf_getinfo },
};

static const char *const sound_system_name[] = {
    "2", "5.1", "5.1.2", "5.1.4", "5.1.4.\u25cb", "7.2.3", "9.1.4", "9.2.10.\u25cb",
    "7.1", "7.1.4", "7.1.2", "3.1.2", "1", "9.1.6", "invalid",
};

GstFlowReturn _gst_lg_adec_set_caps_srcpad_dummy(GstLgAudioDec *pAudioDec);
GstFlowReturn _gst_lg_adec_set_caps_use_decoded_info(GstLgAudioDec *pAudioDec,
                                                     LGADEC_AUDIO_CAPS_INFO *caps_info);
GstFlowReturn _gst_lg_adec_set_caps_srcpad_no_dec(GstLgAudioDec *pAudioDec);
GstFlowReturn _gst_lg_adec_set_caps(GstLgAudioDec *pAudioDec, GstCaps *pCaps);
GstFlowReturn _gst_lg_adec_send_media_info(GstLgAudioDec *pAudioDec);
void _gst_lg_adec_signal_eos(GstLgAudioDec *pAudioDec);
static inline void _gst_lg_adec_init_dump_file(GstLgAudioDec *pAudioDec);
static inline GstFlowReturn _gst_lg_adec_no_decoder_feed_data(GstLgAudioDec *pAudioDec,
                                                              GstBuffer *buffer);

static inline gint play_speed_from_rate(gdouble rate)
{
    return (gint)floor(rate * 1000.0 + 0.5);
}

/* play_speed outside [250, 2000] (0.25x..2x) counts as trick play */
static inline gboolean _gst_lg_adec_no_audio(GstLgAudioDec *pAudioDec)
{
    gint play_speed = 0;

    lgadec_get_play_speed(pAudioDec->adec, &play_speed);
    return (pAudioDec->segment.flags & GST_SEGMENT_FLAG_TRICKMODE_NO_AUDIO) ||
           pAudioDec->isTrickPlay == TRUE || (guint)(play_speed - 250) > 1750;
}

/* ---- GstAudioClippingMeta bookkeeping ------------------------------------------
 * The chain records (input PTS -> front/back µs to cut) for every buffer carrying a
 * GstAudioClippingMeta; _gst_lg_adec_push_data looks the PTS up when the decoded PCM comes
 * back and trims it. One list for the whole process (file-scope, not per element). */
typedef struct audio_clipping_meta {
    struct audio_clipping_meta *prev, *next;
    unsigned long long timestamp;            /* ns */
    unsigned long long front, back;          /* µs (GstAudioClippingMeta in TIME format) */
    unsigned int repeated_count;             /* same PTS seen again (e.g. after a seek) */
} audio_clipping_meta;

typedef struct audio_clipping_meta_list {
    audio_clipping_meta *head, *tail;
    unsigned int count;
} audio_clipping_meta_list;

static audio_clipping_meta_list *clipping_meta_list;

/* @0x169b0 */
bool audio_clipping_meta_list_find(unsigned long long timestamp, unsigned long long *front,
                                   unsigned long long *back)
{
    audio_clipping_meta *meta;

    if (!clipping_meta_list) {
        GST_ERROR("clipping_meta_list is NULL");
        return false;
    }
    for (meta = clipping_meta_list->head; meta; meta = meta->next) {
        if (meta->timestamp == timestamp) {
            if (front)
                *front = meta->front;
            if (back)
                *back = meta->back;
            return true;
        }
    }
    return false;
}

/* @0x17820: creates the list, or empties it */
void audio_clipping_meta_list_init(void)
{
    if (!clipping_meta_list) {
        clipping_meta_list = malloc(sizeof(*clipping_meta_list));
        if (!clipping_meta_list) {
            GST_ERROR("malloc failed");
            return;
        }
    } else {
        audio_clipping_meta *meta;

        while ((meta = clipping_meta_list->head)) {
            clipping_meta_list->head = meta->next;
            free(meta);
        }
    }
    clipping_meta_list->head = NULL;
    clipping_meta_list->tail = NULL;
    clipping_meta_list->count = 0;
    GST_TRACE("init done");
}

/* audio_clipping_meta_list_cleanup, inlined into gst_lg_adec_change_state_down */
static inline void audio_clipping_meta_list_cleanup(void)
{
    if (!clipping_meta_list) {
        GST_TRACE("clipping_meta_list is NULL");
        return;
    }
    audio_clipping_meta_list_init();
    free(clipping_meta_list);
    clipping_meta_list = NULL;
    GST_TRACE("cleanup done");
}

/* audio_clipping_meta_list_delete, inlined into _gst_lg_adec_push_data. Note the count is
 * decremented even when the PTS was not found. */
static inline void audio_clipping_meta_list_delete(unsigned long long timestamp)
{
    audio_clipping_meta *meta;

    if (!clipping_meta_list) {
        GST_ERROR("clipping_meta_list is NULL");
        return;
    }
    for (meta = clipping_meta_list->head; meta; meta = meta->next) {
        if (meta->timestamp != timestamp)
            continue;
        if (meta->prev)
            meta->prev->next = meta->next;
        else
            clipping_meta_list->head = meta->next;
        if (meta->next)
            meta->next->prev = meta->prev;
        else
            clipping_meta_list->tail = meta->prev;
        free(meta);
        break;
    }
    clipping_meta_list->count--;
    GST_TRACE("clipping_meta_list_deleted, timestamp: %lld\n", timestamp);
}

/* audio_clipping_meta_list_find_repeated_count, inlined into the chain */
static inline unsigned int audio_clipping_meta_list_find_repeated_count(unsigned long long timestamp)
{
    audio_clipping_meta *meta;

    if (!clipping_meta_list) {
        GST_ERROR("clipping_meta_list is NULL");
        return 0;
    }
    for (meta = clipping_meta_list->tail; meta; meta = meta->prev)
        if (meta->timestamp == timestamp)
            return meta->repeated_count + 1;
    return 1;
}

/* @0x16a4c: "dummy/trick" src caps pushed after every sink CAPS (and on DSP_RESET):
 * audio/x-raw S8 8000 Hz mono + DUMMY=1 + BYPASS=<es-bypass>. This is the only place
 * the BYPASS caps field is produced. */
GstFlowReturn _gst_lg_adec_set_caps_srcpad_dummy(GstLgAudioDec *pAudioDec)
{
    GstAudioInfo audioInfo;
    GstAudioFormat audioFormat;
    GstCaps *new_caps;
    GstFlowReturn ret;

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return GST_FLOW_ERROR;
    }
    GST_INFO("Set dummy/trick audio caps");
    audioFormat = gst_audio_format_build_integer(TRUE, G_LITTLE_ENDIAN, 8, 8);
    gst_audio_info_init(&audioInfo);
    gst_audio_info_set_format(&audioInfo, audioFormat, 8000, 1, NULL);
    new_caps = gst_audio_info_to_caps(&audioInfo);
    if (!new_caps) {
        GST_ERROR("convert audio caps err");
        return GST_FLOW_ERROR;
    }
    gst_caps_set_simple(new_caps, "DUMMY", G_TYPE_INT, 1, NULL);
    gst_caps_set_simple(new_caps, "BYPASS", G_TYPE_INT, lgadec_get_es_bypass(pAudioDec->adec), NULL);
    /* Inverted as shipped (raw 0x6ae4 bne -> -5): a successful push reports the
     * "dummy caps err" line and GST_FLOW_ERROR, a failed one reports success. */
    if (!gst_pad_push_event(pAudioDec->pSrcPad, gst_event_new_caps(new_caps))) {
        GST_INFO("set src pad's caps success");
        GST_DEBUG("caps are: %" GST_PTR_FORMAT, new_caps);
        ret = GST_FLOW_OK;
    } else {
        GST_INFO("set src pad's dummy caps err");
        ret = GST_FLOW_ERROR;
    }
    gst_caps_unref(new_caps);
    return ret;
}

/* @0x16c60: real src caps from the DSP's FIRST_AU info (UPDATE_DECODED_INFO). No BYPASS
 * field; "es"=TRUE when the DSP announced a pass-through ES output (data-* fields). */
GstFlowReturn _gst_lg_adec_set_caps_use_decoded_info(GstLgAudioDec *pAudioDec,
                                                     LGADEC_AUDIO_CAPS_INFO *caps_info)
{
    gchar codec_str[16] = { 0 };
    LGADEC_CODEC_T codec_type = LGADEC_CODEC_UNKNOWN;
    int freq, channels, bits, sign, acmod, check_downmix, out_channels, gap_correction;
    int data_channels, data_rate, data_bps;
    bool lfe_enable, dolby, clock_x4, atmos, es, downmix;
    GstAudioInfo audioInfo;
    GstAudioFormat audioFormat;
    GstCaps *new_caps, *cur_caps;
    GstStructure *pStructure;
    gchar *caps_string;
    gboolean restart;
    GstFlowReturn ret = GST_FLOW_OK;

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return GST_FLOW_ERROR;
    }
    memcpy(codec_str, caps_info->codec_str, sizeof(codec_str) - 1);
    freq = caps_info->freq;
    channels = caps_info->channels;
    bits = caps_info->bits_per_sample;
    sign = caps_info->sign;
    acmod = caps_info->acmod;
    check_downmix = caps_info->check_downmix;
    lfe_enable = caps_info->lfe_enable;
    dolby = caps_info->dolby;
    clock_x4 = caps_info->clock_x4;
    atmos = caps_info->atmos;
    out_channels = caps_info->out_channels;
    gap_correction = caps_info->gap_correction;
    restart = pAudioDec->restart;
    data_channels = caps_info->data_channels;
    data_rate = caps_info->data_rate;
    data_bps = caps_info->data_bps;
    es = caps_info->es;
    downmix = caps_info->downmix;
    if (lgadec_get_codec_type(pAudioDec->adec, &codec_type) != 0) {
        GST_ERROR("get codec_type error");
        return GST_FLOW_ERROR;
    }
    GST_INFO("SRC pad's audio caps is not set yet. Set audio caps of SRC pad (freq=%d, channels=%d, bps=%d)",
             freq, channels, bits);
    GST_INFO("restart = %d", restart);

    audioFormat = gst_audio_format_build_integer(sign, caps_info->endianness ? G_BIG_ENDIAN : G_LITTLE_ENDIAN,
                                                 bits, bits);
    gst_audio_info_init(&audioInfo);
    gst_audio_info_set_format(&audioInfo, audioFormat, freq, channels, NULL);
    new_caps = gst_audio_info_to_caps(&audioInfo);
    if (!new_caps) {
        GST_ERROR("convert audio caps err");
        return GST_FLOW_ERROR;
    }
    gst_caps_set_simple(new_caps, "codec", G_TYPE_UINT, codec_type, NULL);
    gst_caps_set_simple(new_caps, "acmod", G_TYPE_UINT, acmod, NULL);
    if (check_downmix > 0)
        gst_caps_set_simple(new_caps, "check-downmix", G_TYPE_UINT, check_downmix, NULL);
    if (lfe_enable)
        gst_caps_set_simple(new_caps, "lfe-enable", G_TYPE_BOOLEAN, TRUE, NULL);
    if (dolby)
        gst_caps_set_simple(new_caps, "dolby", G_TYPE_BOOLEAN, TRUE, NULL);
    if (clock_x4)
        gst_caps_set_simple(new_caps, "clock-x4", G_TYPE_BOOLEAN, TRUE, NULL);
    if (atmos)
        gst_caps_set_simple(new_caps, "atmos", G_TYPE_BOOLEAN, TRUE, NULL);
    if (out_channels)
        gst_caps_set_simple(new_caps, "out-channels", G_TYPE_INT, out_channels, NULL);
    if (restart) {
        gst_caps_set_simple(new_caps, "restart", G_TYPE_BOOLEAN, restart, NULL);
        pAudioDec->restart = FALSE;
    }
    if (gap_correction)
        gst_caps_set_simple(new_caps, "gap-correction", G_TYPE_INT, gap_correction, NULL);
    if (es)
        gst_caps_set_simple(new_caps, "es", G_TYPE_BOOLEAN, TRUE, NULL);
    if (codec_str[0])
        gst_caps_set_simple(new_caps, "codec-str", G_TYPE_STRING, codec_str, NULL);
    if (data_rate)
        gst_caps_set_simple(new_caps, "data-rate", G_TYPE_INT, data_rate, NULL);
    if (data_channels)
        gst_caps_set_simple(new_caps, "data-channels", G_TYPE_INT, data_channels, NULL);
    if (data_bps)
        gst_caps_set_simple(new_caps, "data-bps", G_TYPE_INT, data_bps, NULL);
    if (downmix)
        gst_caps_set_simple(new_caps, "downmix", G_TYPE_BOOLEAN, TRUE, NULL);

    caps_string = gst_caps_to_string(new_caps);
    if (caps_string) {
        if (strlen(caps_string) < 0x400)
            MEDIA_LOG(0x919, "DEC.%d - decoded caps \"%s\"", pAudioDec->index, caps_string);
        else
            GST_INFO("caps string len is %d. So doesn't print audio caps string", (int)strlen(caps_string));
        g_free(caps_string);
    }

    pStructure = gst_caps_get_structure(new_caps, 0);
    if (!pStructure) {
        GST_ERROR("unable to get structure from caps");
    } else {
        const gchar *format_string = gst_structure_get_string(pStructure, "format");
        if (format_string) {
            g_free(pAudioDec->pFormatString);
            pAudioDec->pFormatString = g_strdup(format_string);
            if (!pAudioDec->pFormatString)
                GST_ERROR("memory alloc fail!!");
            else
                GST_TRACE("save pAudioDec->pFormatString:%s", pAudioDec->pFormatString);
        }
    }

    /* _gst_lg_adec_set_caps_srcpad_normal, inlined */
    cur_caps = gst_pad_get_current_caps(pAudioDec->pSrcPad);
    if (cur_caps && gst_caps_is_equal(cur_caps, new_caps)) {
        GST_INFO("caps is same");
    } else if (gst_pad_push_event(pAudioDec->pSrcPad, gst_event_new_caps(new_caps))) {
        GST_INFO("set src pad's caps success");
        GST_DEBUG("caps are: %" GST_PTR_FORMAT, new_caps);
    } else {
        GST_ERROR("set src pad's real caps err");
        GST_ERROR("set caps Err");
        ret = GST_FLOW_ERROR;
    }
    if (cur_caps)
        gst_caps_unref(cur_caps);
    gst_caps_unref(new_caps);
    return ret;
}

/* @0x1749c: "media-info" element message (dual-mono, mpegh-ui-updated, IAMF out-channels) */
GstFlowReturn _gst_lg_adec_send_media_info(GstLgAudioDec *pAudioDec)
{
    gint audio_mode;
    guint audio_mode_changed = 0, out_channels, isUpdated = 0;
    LGADEC_CODEC_T codec_type = LGADEC_CODEC_UNKNOWN;
    GstStructure *pMediaInfoStructure;
    gchar *stream_id;

    if (lgadec_get_audio_mode(pAudioDec->adec, &audio_mode, &audio_mode_changed) != 0) {
        GST_ERROR("Get audio_mode Err");
        return GST_FLOW_ERROR;
    }
    GST_INFO("Get DECODED_INFO Event, Send Media Info to App");
    pMediaInfoStructure = gst_structure_new_empty("media-info");
    stream_id = gst_pad_get_stream_id(pAudioDec->pSinkPad);
    gst_structure_set(pMediaInfoStructure, "stream-id", G_TYPE_STRING, stream_id, NULL);
    gst_structure_set(pMediaInfoStructure, "dual-mono", G_TYPE_BOOLEAN, audio_mode == 3, NULL);
    if (audio_mode_changed) {
        isUpdated = 1;
        GST_DEBUG("Audio Mode Changed: %d", audio_mode);
    }
    if (pAudioDec->isMpeghUiUpdated)
        isUpdated = 1;
    gst_structure_set(pMediaInfoStructure, "mpegh-ui-updated", G_TYPE_BOOLEAN,
                      pAudioDec->isMpeghUiUpdated ? TRUE : FALSE, NULL);
    if (lgadec_get_codec_type(pAudioDec->adec, &codec_type) != 0) {
        GST_ERROR("get codec_type error");
        return GST_FLOW_ERROR;
    }
    if (codec_type == LGADEC_CODEC_IAMF) {
        if (lgadec_get_out_channels(pAudioDec->adec, &out_channels) != 0) {
            GST_ERROR("get channels error");
            return GST_FLOW_ERROR;
        }
        gst_structure_set(pMediaInfoStructure, "out-channels", G_TYPE_INT, out_channels, NULL);
    }
    if (!isUpdated) {
        gst_structure_free(pMediaInfoStructure);
    } else {
        gchar *pStr = gst_structure_to_string(pMediaInfoStructure);
        if (pStr) {
            MEDIA_LOG(0x6f2, "send media info: %s", pStr);
            g_free(pStr);
        }
        gst_element_post_message(GST_ELEMENT(pAudioDec),
                                 gst_message_new_custom(GST_MESSAGE_APPLICATION, GST_OBJECT(pAudioDec),
                                                        pMediaInfoStructure));
    }
    g_free(stream_id);
    return GST_FLOW_OK;
}

/* @0x17420 */
void _gst_lg_adec_signal_eos(GstLgAudioDec *pAudioDec)
{
    g_mutex_lock(&pAudioDec->stream_start_mutex);
    if (pAudioDec->bWaitEOS) {
        pAudioDec->bIgnoreEOS = FALSE;
        pAudioDec->bWaitEOS = FALSE;
        GST_INFO("signal EOS");
        g_cond_signal(&pAudioDec->stream_start_cond);
    }
    g_mutex_unlock(&pAudioDec->stream_start_mutex);
}

/* @0x178e4: PCM/ADPCM (no_dec) src caps from the sink stream info */
GstFlowReturn _gst_lg_adec_set_caps_srcpad_no_dec(GstLgAudioDec *pAudioDec)
{
    guint8 bits_per_sample = 0, channels = 0;
    gint freq = 0;
    GstAudioInfo audioInfo;
    GstCaps *new_caps, *cur_caps;
    GstFlowReturn ret = GST_FLOW_OK;

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return GST_FLOW_ERROR;
    }
    if (lgadec_get_bits_per_sample(pAudioDec->adec, &bits_per_sample) != 0) {
        GST_ERROR("lgadec_get_bits_per_sample Err");
        return GST_FLOW_ERROR;
    }
    lgadec_get_number_of_channel(pAudioDec->adec, &channels);
    lgadec_get_freq(pAudioDec->adec, &freq);
    gst_audio_info_init(&audioInfo);
    gst_audio_info_set_format(&audioInfo,
                              gst_audio_format_build_integer(TRUE, G_LITTLE_ENDIAN, bits_per_sample,
                                                             bits_per_sample),
                              freq, channels, NULL);
    new_caps = gst_audio_info_to_caps(&audioInfo);
    if (!new_caps) {
        GST_ERROR("convert audio caps err");
        return GST_FLOW_ERROR;
    }
    cur_caps = gst_pad_get_current_caps(pAudioDec->pSrcPad);
    if (cur_caps && gst_caps_is_equal(cur_caps, new_caps)) {
        GST_INFO("caps is same");
    } else if (gst_pad_push_event(pAudioDec->pSrcPad, gst_event_new_caps(new_caps))) {
        GST_INFO("SRC pad's audio caps is not set yet. Set audio caps of SRC pad (freq=%d, channels=%u, bps=%u)",
                 freq, channels, bits_per_sample);
        GST_DEBUG("caps are: %" GST_PTR_FORMAT, new_caps);
    } else {
        GST_ERROR("set src pad's caps err");
        ret = GST_FLOW_ERROR;
    }
    if (cur_caps)
        gst_caps_unref(cur_caps);
    gst_caps_unref(new_caps);
    return ret;
}

/* @0x17ba4: sink caps -> stream info -> lgadec_select_codec */
GstFlowReturn _gst_lg_adec_set_caps(GstLgAudioDec *pAudioDec, GstCaps *pCaps)
{
    LGADEC_CODEC_INFO_T codec_info = { 0 };
    const GstStructure *pStructure;
    const gchar *pMime, *pPcmFormatString;
    const GValue *pCodecDataValue;
    gint get_value, prev_freq, preselection_id;
    guint i;
    int lg_ret;

    GST_INFO("Handle audio's CAPS");
    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return GST_FLOW_ERROR;
    }
    pStructure = gst_caps_get_structure(pCaps, 0);
    if (!pStructure) {
        GST_ERROR("unable to get structure from caps");
        return GST_FLOW_ERROR;
    }
    pMime = gst_structure_get_name(pStructure);
    if (!pMime) {
        GST_ERROR("unable to get MIME type");
        return GST_FLOW_ERROR;
    }
    GST_INFO("mime = %s", pMime);
    if (!pAudioDec->mime || strcmp(pAudioDec->mime, pMime) != 0) {
        if (pAudioDec->mime)
            GST_INFO("mime is chagend (%s->%s)", pAudioDec->mime, pMime);
        g_free(pAudioDec->mime);
        pAudioDec->mime = g_strdup(pMime);
        if (!pAudioDec->mime)
            GST_ERROR("memory alloc fail!!");
    }
    if (lgadec_get_freq(pAudioDec->adec, &prev_freq) != 0) {
        GST_ERROR("fail get freq");
        return GST_FLOW_ERROR;
    }
    lgadec_init_stream_info(pAudioDec->adec);
    if (gst_structure_get_int(pStructure, "depth", &get_value)) {
        lgadec_set_bits_per_sample(pAudioDec->adec, get_value);
        GST_INFO("bit per sample = %d", get_value);
    }
    if (gst_structure_get_int(pStructure, "rate", &get_value)) {
        lgadec_set_freq(pAudioDec->adec, get_value);
        GST_INFO("sampling frequency = %d", get_value);
        if (prev_freq && prev_freq != get_value)
            GST_INFO("freq is chagend (%d->%d)", prev_freq, get_value);
    }
    if (gst_structure_get_int(pStructure, "channels", &get_value)) {
        lgadec_set_number_of_channel(pAudioDec->adec, get_value);
        GST_INFO("channels = %d", get_value);
    }
    if (gst_structure_get_int(pStructure, "block_align", &get_value)) {
        lgadec_set_block_align(pAudioDec->adec, get_value);
        GST_INFO("block_align = %d", get_value);
    }
    pCodecDataValue = gst_structure_get_value(pStructure, "codec_data");
    if (pCodecDataValue) {
        GstBuffer *pGstBuf = gst_value_get_buffer(pCodecDataValue);
        GstMapInfo bufMapInfo;

        if (gst_buffer_map(pGstBuf, &bufMapInfo, GST_MAP_READ) == TRUE) {
            lgadec_set_length_of_DSI(pAudioDec->adec, bufMapInfo.size);
            if (bufMapInfo.size) {
                void *DSI_buffer_addr = malloc(bufMapInfo.size);
                if (!DSI_buffer_addr) {
                    GST_ERROR("malloc fail");
                } else {
                    memcpy(DSI_buffer_addr, bufMapInfo.data, bufMapInfo.size);
                    lgadec_set_DSI_buffer(pAudioDec->adec, DSI_buffer_addr);
                }
            }
            gst_buffer_unmap(pGstBuf, &bufMapInfo);
        } else {
            GST_ERROR("gst_buffer_map() Err");
        }
    }
    if (gst_structure_get_int(pStructure, "byterate", &get_value)) {
        lgadec_set_avg_bytes_per_sec(pAudioDec->adec, get_value);
        GST_INFO("AvgBytesPerSec = %d", get_value);
    } else if (gst_structure_get_int(pStructure, "bitrate", &get_value)) {
        GST_INFO("bitrate = %d", get_value);
        get_value >>= 3;
        lgadec_set_avg_bytes_per_sec(pAudioDec->adec, get_value);
        GST_INFO("AvgBytesPerSec(using bitrate) = %d", get_value);
    }
    pPcmFormatString = gst_structure_get_string(pStructure, "format");
    if (!pPcmFormatString) {
        GST_INFO("PCM format is NULL");
    } else {
        GstAudioFormat pcmFormat;
        const GstAudioFormatInfo *pAudioFormatInfo;

        GST_INFO("PCM format = %s", pPcmFormatString);
        g_free(pAudioDec->pFormatString);
        pAudioDec->pFormatString = g_strdup(pPcmFormatString);
        pcmFormat = gst_audio_format_from_string(pPcmFormatString);
        if (!pAudioDec->pFormatString)
            GST_ERROR("memory alloc fail!!");
        else
            GST_TRACE("save pAudioDec->pFormatString:%s", pAudioDec->pFormatString);
        if (pcmFormat == GST_AUDIO_FORMAT_UNKNOWN) {
            GST_ERROR("gst_audio_format_from_string() Err(GST_AUDIO_FORMAT_UNKNOWN)");
        } else if (!(pAudioFormatInfo = gst_audio_format_get_info(pcmFormat))) {
            GST_ERROR("gst_audio_format_get_info() Err");
        } else if (!GST_AUDIO_FORMAT_INFO_IS_INTEGER(pAudioFormatInfo)) {
            GST_INFO("Floating PCM");
        } else {
            get_value = pAudioFormatInfo->width;
            lgadec_set_bits_per_sample(pAudioDec->adec, get_value);
            GST_INFO("bitsPerSample = %d", get_value);
            get_value = GST_AUDIO_FORMAT_INFO_IS_SIGNED(pAudioFormatInfo) ? 0 : 1;
            lgadec_set_signed_of_pcm(pAudioDec->adec, get_value);
            GST_INFO("signed = %d", get_value);
            get_value = pAudioFormatInfo->endianness != G_LITTLE_ENDIAN;
            lgadec_set_endian_of_pcm(pAudioDec->adec, get_value);
            GST_INFO("endian = %d", get_value);
        }
    }
    if (gst_structure_get_int(pStructure, "object-type", &get_value)) {
        lgadec_set_object_type(pAudioDec->adec, get_value);
        GST_INFO("object_type = %d", get_value);
    }

    pAudioDec->isVorbisCodec = FALSE;
    pAudioDec->need_gap = FALSE;
    pAudioDec->elementStatus = DECODER_ELEMENT_STATUS_OK;
    pAudioDec->feededFrameCount = 0;
    free(pAudioDec->pcm_buffer);
    pAudioDec->pcm_buffer = NULL;
    pAudioDec->pcm_rd_offset = 0;
    pAudioDec->pcm_wr_offset = 0;

    for (i = 0; i < G_N_ELEMENTS(_g_audio_mime_map); i++)
        if (strstr(pMime, _g_audio_mime_map[i].mime))
            break;
    if (i == G_N_ELEMENTS(_g_audio_mime_map)) {
        GST_ERROR("Unknown MIME(%s)", pMime);
        pAudioDec->elementStatus = DECODER_ELEMENT_STATUS_UNSUPPORTED;
    } else if (_g_audio_mime_map[i].getCaps) {
        if (_g_audio_mime_map[i].getCaps(pStructure, &codec_info) != GST_FLOW_OK) {
            GST_ERROR("getCaps() Err");
            pAudioDec->elementStatus = DECODER_ELEMENT_STATUS_ERR;
            goto unsupported;
        }
        lg_ret = lgadec_select_codec(pAudioDec->adec, &codec_info);
        if (lg_ret == LGADEC_RET_NOT_SUPPORTED) {
            GST_INFO("need GAP event!! This audio is not supported (codec_type:%d)",
                     codec_info.codec_type);
            pAudioDec->need_gap = TRUE;
            GST_ELEMENT_WARNING(pAudioDec, STREAM, CODEC_NOT_FOUND,
                                ("This audio is not supported"), ("This audio is not supported"));
            return GST_FLOW_OK;
        }
        if (lg_ret != 0) {
            GST_ERROR("select codec Err");
            pAudioDec->elementStatus = DECODER_ELEMENT_STATUS_ERR;
            goto unsupported;
        }
        if (codec_info.codec_type == LGADEC_CODEC_VORBIS)
            pAudioDec->isVorbisCodec = TRUE;
    }
    if (gst_structure_get_int(pStructure, "preselection-id", &preselection_id)) {
        lgadec_set_presentation_group_index(pAudioDec->adec, preselection_id);
        GST_INFO("presentation_group_index = %d", preselection_id);
    }
    if (pAudioDec->elementStatus == DECODER_ELEMENT_STATUS_OK) {
        if (pAudioDec->isEnableDumpInput)
            _gst_lg_adec_init_dump_file(pAudioDec);
        return GST_FLOW_OK;
    }

unsupported:
    GST_ERROR("Send ADEC Warning msg(Unsupported Stream) to upper layer");
    GST_ELEMENT_WARNING(pAudioDec, STREAM, CODEC_NOT_FOUND,
                        ("This audio is not supported"), ("This audio is not supported"));
    return GST_FLOW_ERROR;
}

/* @0x18d28 */
void _gst_lg_adec_init_variables(GstLgAudioDec *pAudioDec)
{
    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    pAudioDec->elementStatus = DECODER_ELEMENT_STATUS_INIT;
    pAudioDec->feededFrameCount = 0;
    pAudioDec->isVorbisCodec = FALSE;
    pAudioDec->pcm_rd_offset = 0;
    pAudioDec->pcm_wr_offset = 0;
    memset(&pAudioDec->segment, 0, sizeof(pAudioDec->segment));
    if (pAudioDec->pAudioCaps) {
        gst_caps_unref(pAudioDec->pAudioCaps);
        pAudioDec->pAudioCaps = NULL;
    }
    g_clear_pointer(&pAudioDec->mime, g_free);
    g_clear_pointer(&pAudioDec->pStreamId, g_free);
    g_clear_pointer(&pAudioDec->pFormatString, g_free);
    if (pAudioDec->pcm_buffer) {
        free(pAudioDec->pcm_buffer);
        pAudioDec->pcm_buffer = NULL;
    }
    g_clear_pointer(&pAudioDec->appType, g_free);
    if (pAudioDec->pending_segment) {
        gst_event_unref(pAudioDec->pending_segment);
        pAudioDec->pending_segment = NULL;
    }
}

/* @0x18e18: codec/stream switch: push LAST into the DSP and block until its EOS came back */
void _gst_lg_adec_wait_eos(GstLgAudioDec *pAudioDec)
{
    g_mutex_lock(&pAudioDec->stream_start_mutex);
    if (lgadec_feed_eos(pAudioDec->adec) == 0) {
        GST_INFO("waiting for EOS");
        pAudioDec->bIgnoreEOS = TRUE;
        pAudioDec->bWaitEOS = TRUE;
        g_cond_wait(&pAudioDec->stream_start_cond, &pAudioDec->stream_start_mutex);
        if (!pAudioDec->isSeamless)
            lgadec_flush(pAudioDec->adec);
        GST_INFO("driver EOS done");
    } else {
        pAudioDec->bIgnoreEOS = FALSE;
    }
    g_mutex_unlock(&pAudioDec->stream_start_mutex);
}

/* @0x18f00 (audio_clipping_meta_list_add.isra.0) */
int audio_clipping_meta_list_add(unsigned long long timestamp, unsigned long long front,
                                 unsigned long long back, unsigned int repeated_count)
{
    audio_clipping_meta_list *list = clipping_meta_list;
    audio_clipping_meta *meta;

    if (!list) {
        GST_ERROR("clipping_meta_list is NULL");
        return 0;
    }
    meta = malloc(sizeof(*meta));
    if (!meta) {
        GST_ERROR("malloc failed");
        return 0;
    }
    meta->timestamp = timestamp;
    meta->front = front;
    meta->back = back;
    meta->prev = NULL;
    meta->next = NULL;
    if (repeated_count)
        meta->repeated_count = repeated_count;   /* sic: left uninitialised otherwise */
    if (list->count == 0) {
        list->head = meta;
    } else {
        meta->prev = list->tail;
        list->tail->next = meta;
    }
    list->tail = meta;
    list->count++;
    GST_TRACE("clipping_meta_list_added, timestamp: %lld (front: %llu, back: %llu)\n",
              timestamp, front, back);
    return 0;
}

/* _gst_lg_adec_push_data, inlined into the PUSH_DATA case of the callback.
 * One LGADEC_DATA_T -> one GstBuffer on the src pad. Data is copied (PCM possibly trimmed
 * by a GstAudioClippingMeta, see above); timestamps:
 * PTS = lgadec prev_timestamp[out_type] (ns), DURATION = size * 1e9 / bytes_per_sec
 * (bypass: fixed 352800 B/s). Flags by out_type:
 *   ES 0x200000 | METADATA 0x100000 | METADATA2 0x800000 | BYPASS 0x1000000.
 * Takes ownership of (frees) lgadec_data. */
static GstFlowReturn _gst_lg_adec_push_data(GstLgAudioDec *pAudioDec, LGADEC_DATA_T *lgadec_data)
{
    GstFlowReturn ret = GST_FLOW_OK, retPushBuffer;
    GstClockTime duration = GST_CLOCK_TIME_NONE;
    unsigned long long timestamp;
    gint bytesPerSec;
    GstBuffer *pOutGstBuf;
    GstMapInfo outMap;
    LGADEC_CODEC_T codecType;
    const guint8 *src;
    guint8 *clipped = NULL;
    guint size;
    unsigned long long front = 0, back = 0;

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return GST_FLOW_ERROR;
    }
    if (lgadec_get_byte_per_sec(pAudioDec->adec, &bytesPerSec) != 0) {
        GST_ERROR("lgadec_get_byte_per_sec Err");
        return GST_FLOW_ERROR;
    }
    if (!lgadec_data->data) {
        GST_ERROR("Capture Buffer is NULL!!");
        return GST_FLOW_ERROR;
    }
    lgadec_get_prev_timestamp(pAudioDec->adec, lgadec_data->out_type, &timestamp);
    size = lgadec_data->size;
    src = lgadec_data->data;
    if (bytesPerSec > 0) {
        duration = (gint64)size * GST_SECOND / bytesPerSec;
        if (duration == 0)
            goto alloc;
    }

    /* GstAudioClippingMeta registered for this PTS by the chain: cut front/back (µs) out of
     * the PCM, rounded up to whole frames; drop METADATA belonging to a clipped buffer. */
    if (clipping_meta_list && clipping_meta_list->count &&
        audio_clipping_meta_list_find(timestamp, &front, &back)) {
        if (lgadec_data->out_type == LGADEC_DATA_PCM) {
            unsigned long long duration_us = duration / 1000, cut_us = front + back;
            gint bits = 0, channels = 0, bytes;
            guint offset, frame;
            gint64 remain;

            lgadec_get_audio_caps_bits_per_sample(pAudioDec->adec, &bits);
            bytes = bits / 8;
            if (bytes == 0 || duration_us == 0) {
                GST_ERROR("bps: %d, duration: %llu(usec)\n", bytes, duration_us);
                return GST_FLOW_ERROR;   /* sic: lgadec_data leaks */
            }
            GST_TRACE("cut off audio data, timestamp: %lld(nsec), front: %llu(usec), back %llu(usec)\n",
                      timestamp, front, back);
            audio_clipping_meta_list_delete(timestamp);
            if (cut_us >= duration_us) {
                GST_TRACE("remain_duration is 0, duration: %lld(nsec), discard_duration: %lld(nsec)",
                          duration, cut_us * 1000);
                free(lgadec_data);
                return GST_FLOW_OK;
            }
            remain = duration - cut_us * 1000;
            GST_TRACE("remain_duration: %lld(nsec), remain_size: %d", remain,
                      (gint)((unsigned long long)size * remain / duration));
            GST_TRACE("size: %d, front: %llu(usec), duration: %lld(nsec)\n", size, front, duration);
            offset = (unsigned long long)size * front / duration_us;
            size = (unsigned long long)size * remain / duration;
            lgadec_get_audio_caps_channels(pAudioDec->adec, &channels);
            frame = channels * bytes;
            if (size % frame)
                size = (size / frame) * frame + frame;
            clipped = calloc(size, 1);
            if (!clipped) {
                GST_ERROR("calloc failed");
                free(lgadec_data);
                return GST_FLOW_ERROR;
            }
            memcpy(clipped, lgadec_data->data + offset, size);
            src = clipped;
        } else if (lgadec_data->out_type == LGADEC_DATA_METADATA) {
            GST_TRACE("drop metadata\n");
            free(lgadec_data);
            return GST_FLOW_OK;
        }
    }

alloc:
    pOutGstBuf = gst_buffer_new_allocate(NULL, size, NULL);
    if (!pOutGstBuf) {
        GST_ERROR("GstBuffer alloc Err");
        ret = GST_FLOW_ERROR;
        goto out;
    }
    if (!gst_buffer_map(pOutGstBuf, &outMap, GST_MAP_WRITE)) {
        GST_ERROR("Map buffer Err");
        gst_buffer_unref(pOutGstBuf);
        ret = GST_FLOW_ERROR;
        goto out;
    }
    memcpy(outMap.data, src, size);
    gst_buffer_unmap(pOutGstBuf, &outMap);
    GST_BUFFER_PTS(pOutGstBuf) = timestamp;
    if (pAudioDec->isMpeghUiUpdated) {
        GST_INFO("set GST_BUFFER_FLAG_MPEGH_UI");
        GST_BUFFER_FLAG_SET(pOutGstBuf, GST_BUFFER_FLAG_MPEGH_UI);
        pAudioDec->isMpeghUiUpdated = FALSE;
    }

    switch (lgadec_data->out_type) {
    case LGADEC_DATA_ES:
        GST_BUFFER_FLAG_SET(pOutGstBuf, GST_BUFFER_FLAG_LG_ES);
        break;
    case LGADEC_DATA_METADATA:
        GST_BUFFER_FLAG_SET(pOutGstBuf, GST_BUFFER_FLAG_LG_METADATA);
        goto log;           /* metadata: no duration */
    case LGADEC_DATA_METADATA2:
        GST_BUFFER_FLAG_SET(pOutGstBuf, GST_BUFFER_FLAG_LG_METADATA2);
        goto log;
    case LGADEC_DATA_BYPASS:
        GST_BUFFER_FLAG_SET(pOutGstBuf, GST_BUFFER_FLAG_LG_BYPASS);
        break;
    case LGADEC_DATA_PCM:
        if (duration != GST_CLOCK_TIME_NONE)  /* PCM: extrapolate next PTS */
            lgadec_set_prev_timestamp(pAudioDec->adec, LGADEC_DATA_PCM, timestamp + duration);
        break;
    default:
        break;
    }
    if (lgadec_get_codec_type(pAudioDec->adec, &codecType) != 0)
        GST_ERROR("Get codecType Err");
    GST_BUFFER_DURATION(pOutGstBuf) = duration;

log:
    GST_LOG_OBJECT(pAudioDec, "buffer: type=%s, size=%u, timestamp=%" GST_TIME_FORMAT ", func=%s:%d",
                   "OUT", (guint)gst_buffer_get_size(pOutGstBuf),
                   GST_TIME_ARGS(GST_BUFFER_PTS(pOutGstBuf)), "_gst_lg_adec_push_data", 0x324);
    if (pAudioDec->isFlush) {
        GST_DEBUG("skipped while flushing data..");
        gst_buffer_unref(pOutGstBuf);
    } else {
        LGADEC_DATA_TYPE_T out_type = lgadec_data->out_type;

        retPushBuffer = gst_pad_push(pAudioDec->pSrcPad, pOutGstBuf);
        if (retPushBuffer < GST_FLOW_FLUSHING) {
            GST_ERROR("gst_pad_push() Err (%s)", gst_flow_get_name(retPushBuffer));
            ret = GST_FLOW_ERROR;
        } else if (retPushBuffer < GST_FLOW_OK) {
            GST_DEBUG("gst_pad_push() fail (not error) (%s)", gst_flow_get_name(retPushBuffer));
        }
        /* a SEGMENT that arrived while PLAYING is forwarded once PCM caught up with it */
        if (pAudioDec->pending_segment && timestamp > pAudioDec->last_in_timestamp &&
            retPushBuffer == GST_FLOW_OK && out_type == LGADEC_DATA_PCM) {
            gst_pad_push_event(pAudioDec->pSrcPad, pAudioDec->pending_segment);
            gst_event_unref(pAudioDec->pending_segment);   /* sic: ref was consumed by push */
            pAudioDec->pending_segment = NULL;
        }
        if (!pAudioDec->first_data_send && out_type == LGADEC_DATA_PCM)
            pAudioDec->first_data_send = TRUE;
    }
out:
    if (clipped)
        free(clipped);
    free(lgadec_data);
    GST_DEBUG("exit with ret %s", gst_flow_get_name(ret));
    return ret;
}

/* @0x19048: lgadec -> element callback (runs on aimc appsink threads, the lgadec task
 * thread, or -- for LGADEC_DATA_BYPASS -- synchronously inside the chain call). */
int _gst_lg_adec_callback_task(void *a, LGADEC_EVENT_MSG_T event_msg, void *data)
{
    GstLgAudioDec *pAudioDec = a;
    gint ret = 0;

    pthread_mutex_lock(&pAudioDec->cb_mutex);
    GST_DEBUG("received msg: %s", lgadec_get_event_name(event_msg));
    switch (event_msg) {
    case LGADEC_EVENT_DSP_RESET:
        pAudioDec->decodedSize = 0;
        if (_gst_lg_adec_set_caps_srcpad_dummy(pAudioDec) != GST_FLOW_OK) {
            GST_ERROR("Set dummy caps of SRC pad is failed");
            ret = -1;
        }
        break;
    case LGADEC_EVENT_UPDATE_DECODED_INFO:
        if (_gst_lg_adec_set_caps_use_decoded_info(pAudioDec, data) != GST_FLOW_OK) {
            GST_ERROR("Set normal caps of SRC pad is failed");
            ret = -1;
        }
        break;
    case LGADEC_EVENT_SEND_MEDIA_INFO:
        if (_gst_lg_adec_send_media_info(pAudioDec) != GST_FLOW_OK) {
            GST_ERROR("send_media_info Err");
            ret = -1;
        }
        break;
    case LGADEC_EVENT_PUSH_DATA:
        if (!data) {
            GST_ERROR("data is NULL");
            ret = -1;
        } else if (_gst_lg_adec_push_data(pAudioDec, data) != GST_FLOW_OK) {
            GST_ERROR("push data Err!!");
            ret = -1;
        }
        break;
    case LGADEC_EVENT_EOS:
        g_mutex_lock(&pAudioDec->stream_start_mutex);
        if (!pAudioDec->bIgnoreEOS) {
            g_mutex_unlock(&pAudioDec->stream_start_mutex);
            gst_pad_push_event(pAudioDec->pSrcPad, gst_event_new_eos());
            GST_INFO("send EOS to sink");
        } else {
            g_mutex_unlock(&pAudioDec->stream_start_mutex);
            _gst_lg_adec_signal_eos(pAudioDec);
            GST_INFO("ignore EOS for DSC");
        }
        break;
    case LGADEC_EVENT_DEC_MPEGH_UI:
        pAudioDec->isMpeghUiUpdated = TRUE;
        if (_gst_lg_adec_send_media_info(pAudioDec) != GST_FLOW_OK) {
            GST_ERROR("send_media_info Err");
            ret = -1;
        }
        break;
    case LGADEC_EVENT_PREROLL: {           /* _gst_lg_adec_push_null_data, inlined */
        GstBuffer *out_gst_buf = gst_buffer_new_allocate(NULL, 0, NULL);
        GstFlowReturn fret;

        if (!out_gst_buf) {
            GST_ERROR("GstBuffer alloc Err");
            ret = -1;
            break;
        }
        LOGM(logmm_module_media_event, log_level_noti, 0x103e,
             "<lgaudiodec> Set GST_BUFFER_FLAG_NULL_DATA\n");
        GST_INFO("Set GST_BUFFER_FLAG_NULL_DATA\n");
        GST_BUFFER_FLAG_SET(out_gst_buf, GST_BUFFER_FLAG_NULL_DATA);
        fret = gst_pad_push(pAudioDec->pSrcPad, out_gst_buf);
        if (fret != GST_FLOW_OK) {
            GST_ERROR("gst_pad_push() Err (ret=%d)", fret);
            GST_ERROR("push null data Err");
            ret = -1;
        } else {
            GST_LOG("push null data");
        }
        break;
    }
    case LGADEC_EVENT_DIALOG_ENHANCEMENT_LIMIT_CHANGED:
        g_signal_emit_by_name(GST_ELEMENT(pAudioDec), "dialog-enhancement-limit-change");
        GST_INFO("signal dialog-enhancement-limit-change");
        break;
    default:
        break;
    }
    pthread_mutex_unlock(&pAudioDec->cb_mutex);
    return ret;
}

/* _gst_lg_adec_log_buffer (inlined everywhere, line 0x217) */
#define LOG_BUFFER(dir, buf, func, line) \
    GST_LOG_OBJECT(pAudioDec, "buffer: type=%s, size=%u, timestamp=%" GST_TIME_FORMAT \
                   ", func=%s:%d", dir, (guint)gst_buffer_get_size(buf), \
                   GST_TIME_ARGS(GST_BUFFER_PTS(buf)), func, line)

/* gst_pad_push result handling shared by every host-side feeder */
static inline GstFlowReturn _gst_lg_adec_push_result(GstFlowReturn ret)
{
    if (ret < GST_FLOW_FLUSHING) {
        GST_ERROR("gst_pad_push() Err (%s)", gst_flow_get_name(ret));
        return GST_FLOW_ERROR;
    }
    if (ret < GST_FLOW_OK) {
        GST_DEBUG("gst_pad_push() fail (not error) (%s)", gst_flow_get_name(ret));
        return GST_FLOW_OK;
    }
    return ret;
}

/* _gst_lg_adec_init_dump_file (inlined into _gst_lg_adec_set_caps): only with
 * /tmp/gst_dump_audio_input present at READY->PAUSED */
static inline void _gst_lg_adec_init_dump_file(GstLgAudioDec *pAudioDec)
{
    char filename[64];

    memset(filename, 0, sizeof(filename));
    snprintf(filename, sizeof(filename), "/tmp/audiodec_ES_ALL_Original-%u:%02u:%02u.%09u.dump",
             GST_TIME_ARGS(gst_util_get_timestamp()));
    GST_DEBUG("Audio ES filename: %s", filename);
    if (pAudioDec->pDumpAudioFd)
        fclose(pAudioDec->pDumpAudioFd);
    pAudioDec->pDumpAudioFd = fopen(filename, "wb");
    if (!pAudioDec->pDumpAudioFd)
        GST_ERROR("FOpen error !! <%s>", filename);
}

/* _gst_lg_adec_dump_file (inlined into the chain): raw input ES */
static inline void _gst_lg_adec_dump_file(GstLgAudioDec *pAudioDec, GstBuffer *buffer)
{
    GstMapInfo map;

    if (!gst_buffer_map(buffer, &map, GST_MAP_READ)) {
        GST_ERROR("gst_buffer_map() Err");
        return;
    }
    if (pAudioDec->pDumpAudioFd)
        fwrite(map.data, 1, map.size, pAudioDec->pDumpAudioFd);
    gst_buffer_unmap(buffer, &map);
}

/* @0x1a318: one BD / Wi-Fi LPCM payload -> little-endian S16/S24 buffer on the src pad */
GstFlowReturn _gst_lg_adec_bd_wifi_lpcm_write_buffer(GstLgAudioDec *pAudioDec, guint8 *data,
                                                     guint data_len)
{
    GstBuffer *out;
    GstMapInfo out_map;
    GstClockTime timestamp;

    out = gst_buffer_new_allocate(NULL, data_len, NULL);
    if (!out) {
        GST_ERROR("GstBuffer alloc Err");
        return GST_FLOW_ERROR;
    }
    if (!gst_buffer_map(out, &out_map, GST_MAP_WRITE)) {
        GST_ERROR("Map buffer Err");
        gst_buffer_unref(out);
        return GST_FLOW_ERROR;
    }
    if (lgadec_bd_wifi_lpcm_resampling(pAudioDec->adec, data, data_len, out_map.data) != 0) {
        GST_ERROR("get timestamp fail!!");             /* sic */
        goto err;
    }
    if (lgadec_pcm_get_timestamp(pAudioDec->adec, &timestamp) != 0) {
        GST_ERROR("get timestamp fail!!");
        goto err;
    }
    gst_buffer_unmap(out, &out_map);
    GST_BUFFER_PTS(out) = timestamp;
    LOG_BUFFER("OUT", out, "_gst_lg_adec_bd_wifi_lpcm_write_buffer", 0x4c4);
    return _gst_lg_adec_push_result(gst_pad_push(pAudioDec->pSrcPad, out));

err:
    gst_buffer_unmap(out, &out_map);
    gst_buffer_unref(out);
    return GST_FLOW_ERROR;
}

#define LPCM_BUFFER_SIZE 0x4000
#define BD_LPCM_HEADER_SIZE 4

/* _gst_lg_adec_bd_wifi_lpcm_feed_data (inlined): accumulate input in pcm_buffer, then
 * walk [4-byte header][payload] units. A changed header flushes what is left. */
static inline GstFlowReturn _gst_lg_adec_bd_wifi_lpcm_feed_data(GstLgAudioDec *pAudioDec,
                                                                GstBuffer *buffer,
                                                                LGADEC_PCM_T pcm_type)
{
    GstFlowReturn ret = GST_FLOW_OK;
    GstMapInfo map;
    guint payload_size;
    int header_flag;

    if (!pAudioDec->pcm_buffer) {
        pAudioDec->pcm_buffer = malloc(LPCM_BUFFER_SIZE);
        if (!pAudioDec->pcm_buffer) {
            GST_ERROR("malloc Err");
            return GST_FLOW_ERROR;
        }
        pAudioDec->pcm_rd_offset = 0;
        pAudioDec->pcm_wr_offset = 0;
    }
    if (!gst_buffer_map(buffer, &map, GST_MAP_READ)) {
        GST_ERROR("gst_buffer_map() Err");
        goto err;
    }
    if (map.size + pAudioDec->pcm_wr_offset > LPCM_BUFFER_SIZE) {
        GST_ERROR("overflow lpcm buffer");
        gst_buffer_unmap(buffer, &map);
        goto err;
    }
    if (pAudioDec->pcm_wr_offset == 0 &&
        lgadec_pcm_set_timestamp(pAudioDec->adec, GST_BUFFER_PTS(buffer)) != 0) {
        GST_ERROR("set timestamp fail!!");
        gst_buffer_unmap(buffer, &map);
        goto err;
    }
    memcpy(pAudioDec->pcm_buffer + pAudioDec->pcm_wr_offset, map.data, map.size);
    pAudioDec->pcm_wr_offset += map.size;
    gst_buffer_unmap(buffer, &map);

    do {
        guint avail = pAudioDec->pcm_wr_offset - pAudioDec->pcm_rd_offset;

        if (avail <= BD_LPCM_HEADER_SIZE) {
            GST_WARNING("BD LPCM audio chunk's size is less than header (%d bytes).",
                        BD_LPCM_HEADER_SIZE);
            goto keep_rest;
        }
        if (pcm_type == LGADEC_PCM_BD) {
            if (lgadec_bdlpcm_header_check(pAudioDec->adec, pAudioDec->pcm_buffer,
                                           pAudioDec->pcm_rd_offset, &header_flag) != 0)
                goto hdr_err;
        } else if (pcm_type == LGADEC_PCM_WIFI) {
            if (lgadec_wifilpcm_header_check(pAudioDec->adec, pAudioDec->pcm_buffer,
                                             pAudioDec->pcm_rd_offset, &header_flag) != 0)
                goto hdr_err;
        } else {
            GST_ERROR("unknown PCM type (type:%d)", LGADEC_PCM_DVD);
            goto err;
        }
        if (lgadec_pcm_get_payload_size(pAudioDec->adec, &payload_size) != 0) {
            GST_ERROR("dblpcm resampling Err!!");
            goto err;
        }
        if (header_flag == 2) {                          /* first header: src caps */
            if (_gst_lg_adec_set_caps_srcpad_no_dec(pAudioDec) != GST_FLOW_OK) {
                GST_ERROR("Set no decoder caps of SRC pad is failed");
                goto err;
            }
            ret = GST_FLOW_OK;
        }
        if (header_flag == 1) {                          /* changed: flush the rest */
            GST_WARNING("BD LPCM header is different");
            ret = _gst_lg_adec_bd_wifi_lpcm_write_buffer(pAudioDec,
                      pAudioDec->pcm_buffer + pAudioDec->pcm_rd_offset,
                      pAudioDec->pcm_wr_offset - pAudioDec->pcm_rd_offset);
            pAudioDec->pcm_rd_offset = 0;
            pAudioDec->pcm_wr_offset = 0;
            break;
        }
        avail = pAudioDec->pcm_wr_offset - BD_LPCM_HEADER_SIZE - pAudioDec->pcm_rd_offset;
        if (avail < payload_size) {
            GST_ERROR("audio chunk size(%u) is less than BD LPCM payload size(%u).", avail,
                      payload_size);
            goto keep_rest;
        }
        ret = _gst_lg_adec_bd_wifi_lpcm_write_buffer(pAudioDec,
                  pAudioDec->pcm_buffer + pAudioDec->pcm_rd_offset + BD_LPCM_HEADER_SIZE,
                  payload_size);
        pAudioDec->pcm_rd_offset += payload_size + BD_LPCM_HEADER_SIZE;
        if (pAudioDec->pcm_rd_offset == pAudioDec->pcm_wr_offset) {
            pAudioDec->pcm_rd_offset = 0;
            pAudioDec->pcm_wr_offset = 0;
            break;
        }
        GST_WARNING("audio chunk size(%u) is great than BD LPCM payload size(%u).",
                    pAudioDec->pcm_wr_offset - BD_LPCM_HEADER_SIZE - pAudioDec->pcm_rd_offset,
                    payload_size);
        continue;

keep_rest:
        memcpy(pAudioDec->pcm_buffer, pAudioDec->pcm_buffer + pAudioDec->pcm_rd_offset,
               pAudioDec->pcm_wr_offset - pAudioDec->pcm_rd_offset);
        pAudioDec->pcm_wr_offset -= pAudioDec->pcm_rd_offset;
        pAudioDec->pcm_rd_offset = 0;
        break;
    } while (pAudioDec->pcm_rd_offset < pAudioDec->pcm_wr_offset);
    return ret;

hdr_err:
    GST_ERROR("bdlpcm header check Err!!");
err:
    pAudioDec->pcm_rd_offset = 0;
    pAudioDec->pcm_wr_offset = 0;
    return GST_FLOW_ERROR;
}

#define DVD_LPCM_HEADER_SIZE 5

/* _gst_lg_adec_dvdlpcm_feed_data (inlined): DVD private stream 1 LPCM. Header =
 * first_access(2, BE) + 3 audio bytes; 16/24-bit are repacked in place, 20-bit into a new
 * buffer of 3/2.5 the size. */
static inline GstFlowReturn _gst_lg_adec_dvdlpcm_feed_data(GstLgAudioDec *pAudioDec,
                                                           GstBuffer *buffer)
{
    GstMapInfo map, in_map, out_map;
    GstBuffer *payload, *out;
    guint first_access;
    guint32 width;
    int new_header = 0;
    GstClockTime timestamp;

    if (!gst_buffer_map(buffer, &map, GST_MAP_READ)) {
        GST_ERROR("gst_buffer_map() Err");
        return GST_FLOW_ERROR;
    }
    if (map.size < DVD_LPCM_HEADER_SIZE) {
        GST_ERROR("DVD LPCM audio chunk's size is less than header size(5 bytes). Drop.");
        goto unmap_err;
    }
    first_access = map.data[0] << 8 | map.data[1];
    if (map.size < first_access) {
        GST_ERROR("Invalide first access of LPCM packet. Drop.");
        return GST_FLOW_ERROR;   /* sic: still mapped */
    }
    if (lgadec_dvdlpcm_header_check(pAudioDec->adec, map.data, &new_header) != 0) {
        GST_ERROR("dvdlpcm header check fail");
        goto unmap_err;
    }
    if (new_header && _gst_lg_adec_set_caps_srcpad_no_dec(pAudioDec) != GST_FLOW_OK) {
        GST_ERROR("Set no decoder caps of SRC pad is failed");
        return GST_FLOW_ERROR;
    }
    if (first_access > 4 && map.size < first_access + 1) {
        GST_ERROR("firstAccess parameter out of range");
        goto unmap_err;
    }
    gst_buffer_unmap(buffer, &map);

    if (lgadec_pcm_get_width(pAudioDec->adec, &width) != 0) {
        GST_ERROR("get width fail!!");
        return GST_FLOW_ERROR;
    }
    payload = gst_buffer_copy_region(buffer, GST_BUFFER_COPY_MEMORY, DVD_LPCM_HEADER_SIZE,
                                     map.size - DVD_LPCM_HEADER_SIZE);
    if (!payload) {
        GST_ERROR("fail to copy buffer!!");
        return GST_FLOW_ERROR;
    }
    GST_BUFFER_PTS(payload) = GST_BUFFER_PTS(buffer);
    GST_BUFFER_DURATION(payload) = GST_BUFFER_DURATION(buffer);
    if (!gst_buffer_map(payload, &in_map, GST_MAP_WRITE)) {
        GST_ERROR("gst_buffer_map() Err");
        gst_buffer_unref(payload);
        return GST_FLOW_ERROR;
    }
    timestamp = GST_BUFFER_PTS(payload);

    if (width != 20) {
        if (lgadec_dvdlpcm_resampling(pAudioDec->adec, in_map.data, in_map.size, timestamp, NULL) != 0) {
            GST_ERROR("lgadec_dvdlpcm_resampling fail");
            gst_buffer_unmap(payload, &in_map);
            gst_buffer_unref(payload);
            return GST_FLOW_ERROR;
        }
        gst_buffer_unmap(payload, &in_map);
        LOG_BUFFER("OUT", payload, "_gst_lg_adec_dvdlpcm_feed_data", 0x480);
        return _gst_lg_adec_push_result(gst_pad_push(pAudioDec->pSrcPad, payload));
    }

    if (in_map.size * 8 < 20) {
        GST_ERROR("dataLen(%u) is too small. Dropping.", (guint)in_map.size);
        gst_buffer_unmap(payload, &in_map);
        gst_buffer_unref(payload);
        return GST_FLOW_ERROR;
    }
    out = gst_buffer_new_allocate(NULL, (in_map.size * 8 / 20) * 3, NULL);
    if (!out) {
        GST_ERROR("GstBuffer alloc Err");
        gst_buffer_unmap(payload, &in_map);
        gst_buffer_unref(payload);
        return GST_FLOW_ERROR;
    }
    if (!gst_buffer_map(out, &out_map, GST_MAP_WRITE)) {
        GST_ERROR("Map buffer Err");
        gst_buffer_unmap(payload, &in_map);
        gst_buffer_unref(payload);
        gst_buffer_unref(out);
        return GST_FLOW_ERROR;
    }
    if (lgadec_dvdlpcm_resampling(pAudioDec->adec, in_map.data, in_map.size, timestamp,
                                  out_map.data) != 0) {
        GST_ERROR("lgadec_dvdlpcm_resampling fail");
        gst_buffer_unmap(payload, &in_map);
        gst_buffer_unref(payload);
        gst_buffer_unmap(out, &out_map);
        gst_buffer_unref(out);
        return GST_FLOW_ERROR;
    }
    gst_buffer_unmap(payload, &in_map);
    gst_buffer_unref(payload);
    gst_buffer_unmap(out, &out_map);
    GST_BUFFER_PTS(out) = GST_BUFFER_PTS(payload);          /* sic: read after unref */
    GST_BUFFER_DURATION(out) = GST_BUFFER_DURATION(payload);
    LOG_BUFFER("OUT", out, "_gst_lg_adec_dvdlpcm_feed_data", 0x464);
    return _gst_lg_adec_push_result(gst_pad_push(pAudioDec->pSrcPad, out));

unmap_err:
    gst_buffer_unmap(buffer, &map);
    return GST_FLOW_ERROR;
}

/* _gst_lg_adec_mulaw_alaw_feed_data (inlined): G.711 -> S16 (2x size) */
static inline GstFlowReturn _gst_lg_adec_mulaw_alaw_feed_data(GstLgAudioDec *pAudioDec,
                                                              GstBuffer *buffer,
                                                              LGADEC_PCM_T pcm_type)
{
    GstMapInfo in_map, out_map;
    GstBuffer *out;
    int err;

    if (pAudioDec->decodedSize == 0 && _gst_lg_adec_set_caps_srcpad_no_dec(pAudioDec) != GST_FLOW_OK) {
        GST_ERROR("Set no decoder caps of SRC pad is failed");
        return GST_FLOW_ERROR;
    }
    if (!gst_buffer_map(buffer, &in_map, GST_MAP_READ)) {
        GST_ERROR("gst_buffer_map() Err");
        return GST_FLOW_ERROR;
    }
    out = gst_buffer_new_allocate(NULL, in_map.size * 2, NULL);
    if (!out) {
        GST_ERROR("GstBuffer alloc Err");
        gst_buffer_unmap(buffer, &in_map);
        return GST_FLOW_ERROR;
    }
    if (!gst_buffer_map(out, &out_map, GST_MAP_WRITE)) {
        GST_ERROR("Map buffer Err");
        gst_buffer_unmap(buffer, &in_map);
        gst_buffer_unref(out);
        return GST_FLOW_ERROR;
    }
    if (pcm_type == LGADEC_PCM_MULAW)
        err = lgadec_mulaw_resampling(pAudioDec->adec, in_map.data, in_map.size, (short *)out_map.data);
    else
        err = lgadec_alaw_resampling(pAudioDec->adec, in_map.data, in_map.size, (short *)out_map.data);
    if (err != 0) {
        GST_ERROR("mulaw_resampling Err!!");
        gst_buffer_unmap(buffer, &in_map);
        gst_buffer_unmap(out, &out_map);
        gst_buffer_unref(out);
        return GST_FLOW_ERROR;
    }
    gst_buffer_unmap(buffer, &in_map);
    gst_buffer_unmap(out, &out_map);
    GST_BUFFER_PTS(out) = GST_BUFFER_PTS(buffer);
    GST_BUFFER_DURATION(out) = GST_BUFFER_DURATION(buffer);
    LOG_BUFFER("OUT", out, "_gst_lg_adec_mulaw_alaw_feed_data", 0x5d4);
    return _gst_lg_adec_push_result(gst_pad_push(pAudioDec->pSrcPad, out));
}

/* _gst_lg_adec_adpcm_feed_data (inlined): blocks -> S16 (output allocated at 4x input) */
static inline GstFlowReturn _gst_lg_adec_adpcm_feed_data(GstLgAudioDec *pAudioDec, GstBuffer *buffer)
{
    GstMapInfo in_map, out_map;
    GstBuffer *out;
    unsigned int data_len, out_size;
    short *pcm_sample = NULL;

    if (pAudioDec->decodedSize == 0 && _gst_lg_adec_set_caps_srcpad_no_dec(pAudioDec) != GST_FLOW_OK) {
        GST_ERROR("Set no decoder caps of SRC pad is failed");
        return GST_FLOW_ERROR;
    }
    if (!gst_buffer_map(buffer, &in_map, GST_MAP_READ)) {
        GST_ERROR("gst_buffer_map() Err");
        return GST_FLOW_ERROR;
    }
    data_len = in_map.size;
    if (lgadec_adpcm_data_length_check(pAudioDec->adec, in_map.data, &data_len) != 0) {
        GST_ERROR("adpcm data len check Err");
        gst_buffer_unmap(buffer, &in_map);
        return GST_FLOW_ERROR;
    }
    if (data_len == 0) {                  /* buffered, wait for a full block */
        GST_WARNING("input data length is too short");
        return GST_FLOW_OK;               /* sic: input stays mapped */
    }
    out_size = data_len * 4;
    out = gst_buffer_new_allocate(NULL, out_size, NULL);
    if (!out) {
        GST_ERROR("GstBuffer alloc Err");
        gst_buffer_unmap(buffer, &in_map);
        return GST_FLOW_ERROR;
    }
    if (!gst_buffer_map(out, &out_map, GST_MAP_WRITE)) {
        GST_ERROR("Map buffer Err");
        gst_buffer_unmap(buffer, &in_map);
        gst_buffer_unref(out);
        return GST_FLOW_ERROR;
    }
    pcm_sample = (short *)out_map.data;
    if (lgadec_adpcm_resampling(pAudioDec->adec, in_map.data, data_len, &pcm_sample) != 0) {
        GST_ERROR("adpcm feed data Err");
        gst_buffer_unmap(buffer, &in_map);
        gst_buffer_unmap(out, &out_map);
        gst_buffer_unref(out);
        return GST_FLOW_ERROR;
    }
    gst_buffer_unmap(buffer, &in_map);
    gst_buffer_unmap(out, &out_map);
    if ((guint)((guint8 *)pcm_sample - out_map.data) < out_size) {
        GST_TRACE("resize buffer size from %u to %u", out_size,
                  (guint)((guint8 *)pcm_sample - out_map.data));
        gst_buffer_set_size(out, (guint8 *)pcm_sample - out_map.data);
    }
    GST_BUFFER_PTS(out) = GST_BUFFER_PTS(buffer);
    GST_BUFFER_DURATION(out) = GST_BUFFER_DURATION(buffer);
    LOG_BUFFER("OUT", out, "_gst_lg_adec_adpcm_feed_data", 0x3a2);
    if (gst_pad_push(pAudioDec->pSrcPad, out) < GST_FLOW_FLUSHING) {
        GST_ERROR("gst_pad_push() Err (%s)", gst_flow_get_name(GST_FLOW_ERROR));
        return GST_FLOW_ERROR;
    }
    return GST_FLOW_OK;
}

/* _gst_lg_adec_no_decoder_feed_data (inlined into the chain): codecs that never reach the
 * DSP (lgadec no_dec: PCM family and ADPCM) are converted here and pushed directly.
 * Plain LPCM (LGADEC_PCM_BASE) and anything unknown is forwarded as is. */
static inline GstFlowReturn _gst_lg_adec_no_decoder_feed_data(GstLgAudioDec *pAudioDec,
                                                              GstBuffer *buffer)
{
    LGADEC_CODEC_T codec_type;
    LGADEC_PCM_T pcm_type;
    GstFlowReturn ret;

    if (lgadec_get_codec_type(pAudioDec->adec, &codec_type) != 0) {
        GST_ERROR("get codec type Err");
        goto forward;
    }
    if (codec_type == LGADEC_CODEC_ADPCM)
        return _gst_lg_adec_adpcm_feed_data(pAudioDec, buffer);
    if (codec_type != LGADEC_CODEC_PCM)
        goto forward;
    if (lgadec_get_pcm_type(pAudioDec->adec, &pcm_type) != 0) {
        GST_ERROR("get pcm type Err");
        goto forward;
    }
    switch (pcm_type) {
    case LGADEC_PCM_WIFI:
    case LGADEC_PCM_BD:
        return _gst_lg_adec_bd_wifi_lpcm_feed_data(pAudioDec, buffer, pcm_type);
    case LGADEC_PCM_DVD:
        return _gst_lg_adec_dvdlpcm_feed_data(pAudioDec, buffer);
    case LGADEC_PCM_MULAW:
    case LGADEC_PCM_ALAW:
        return _gst_lg_adec_mulaw_alaw_feed_data(pAudioDec, buffer, pcm_type);
    default:
        break;
    }

forward:
    if (pAudioDec->decodedSize == 0 && _gst_lg_adec_set_caps_srcpad_no_dec(pAudioDec) != GST_FLOW_OK) {
        GST_ERROR("Set no decoder caps of SRC pad is failed");
        return GST_FLOW_ERROR;
    }
    gst_buffer_ref(buffer);
    LOG_BUFFER("OUT", buffer, "_gst_lg_adec_no_decoder_feed_data", 0x626);
    ret = _gst_lg_adec_push_result(gst_pad_push(pAudioDec->pSrcPad, buffer));
    return ret;
}

/* @0x1a68c: property table shared by all 12 elements */
void gst_lg_adec_class_init(GObjectClass *gobject_class)
{
    const GParamFlags RW = G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS;
    const GParamFlags RO = G_PARAM_READABLE | G_PARAM_STATIC_STRINGS;
    const GParamFlags WO = G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS;

    g_object_class_install_property(gobject_class, PROP_DECODED_SIZE,
        g_param_spec_uint64("decoded-size", "Decoded Data Length", "Decoded Data Length",
                            0, G_MAXUINT64, 0, RO));
    g_object_class_install_property(gobject_class, PROP_UNDECODED_SIZE,
        g_param_spec_uint64("undecoded-size", "UnDecoded Data Length", "UnDecoded Data Length",
                            0, G_MAXUINT64, 0, RO));
    g_object_class_install_property(gobject_class, PROP_TRICK_MODE,
        g_param_spec_boolean("serverside-trickplay", "Set trick play mode",
                             "Set server's trick play mode", FALSE, RW));
    g_object_class_install_property(gobject_class, PROP_APP_TYPE,
        g_param_spec_string("app-type", "Application Type", "Set application type", NULL, RW));
    g_object_class_install_property(gobject_class, PROP_RESOURCE_INFO,
        g_param_spec_boxed("resource-info", "Resource Info", "Set the port of audio resource",
                           GST_TYPE_STRUCTURE, WO));
    g_object_class_install_property(gobject_class, PROP_INDEX,
        g_param_spec_uint("index", "Index of audio resource", "Index of audio resource", 0, 7, 0, RW));
    g_object_class_install_property(gobject_class, PROP_SEAMLESS,
        g_param_spec_boolean("seamless", "Seamless audio change", "Seamless audio change", FALSE, RW));
    g_object_class_install_property(gobject_class, PROP_MPEG_DASH,
        g_param_spec_boolean("mpeg-dash", "MPEG-DASH player", "MPEG-DASH player", FALSE, RW));
    g_object_class_install_property(gobject_class, PROP_PLAY_SPEED,
        g_param_spec_int("play-speed", "Play Speed", "Play Speed", G_MININT, G_MAXINT, 1000, RW));
    g_object_class_install_property(gobject_class, PROP_VERSION,
        g_param_spec_string("version", "Version of audio_dec", "Version of audio decoder element",
                            "v1.00.00-2017.10.13", RO));
    g_object_class_install_property(gobject_class, PROP_OUTPUT_MODE,
        g_param_spec_uint("dual-mono", "Audio output mode of audio_dec (Dual mono)",
                          "Audio output mode of decoder element (Dual mono)", 0, 2, 1, RW));
    if (G_TYPE_CHECK_CLASS_TYPE(gobject_class, gst_lg_audiodec_mpegh_get_type()))
        g_object_class_install_property(gobject_class, PROP_MPEGH_UI_XML,
            g_param_spec_string("mpegh-ui-xml", "MPEG-H UI XML", "XML description of MPEG-H audio UI",
                                NULL, RW));
    g_object_class_install_property(gobject_class, PROP_DEFAULT_PRL,
        g_param_spec_int("default-prl", "AAC/HE-AAC Default PRL",
                         "PRL (Programme Reference Level) Parameter for AAC/HE-AAC ", -31, 0, 0, WO));
    g_object_class_install_property(gobject_class, PROP_DOLBY_DRC_MODE,
        g_param_spec_uint("drc-mode", "Dolby DRC Mode", "Dolby DRC Mode ", 0, 1, 0, RW));
    g_object_class_install_property(gobject_class, PROP_AC4_DE_GAIN,
        g_param_spec_uint("dialog-enhancement-gain", "Ac4 Dialogue Enhancement Gain",
                          "Ac4 Dialogue Enhancement Gain", 0, G_MAXINT, 0, RW));
    g_object_class_install_property(gobject_class, PROP_AC4_DE_LIMIT_MIN,
        g_param_spec_uint("dialog-enhancement-min", "Ac4 Dialogue Enhancement Limit (Min)",
                          "Ac4 Dialogue Enhancement Limit (Min)", 0, G_MAXINT, 0, RW));
    g_object_class_install_property(gobject_class, PROP_AC4_DE_LIMIT_MAX,
        g_param_spec_uint("dialog-enhancement-max", "Ac4 Dialogue Enhancement Limit (Max)",
                          "Ac4 Dialogue Enhancement Limit (Max)", 0, G_MAXINT, 0, RW));
    g_object_class_install_property(gobject_class, PROP_DIRECTMEDIA_TOTAL_BUF_SIZE,
        g_param_spec_int("directmedia-total-audio-buffer-size", "Total Audio Buffer Size for Directmedia",
                         "Total Audio Buffer Size for Directmedia", 0, G_MAXINT, 0, RO));
    g_object_class_install_property(gobject_class, PROP_DIRECTMEDIA_USED_BUF_SIZE,
        g_param_spec_int("directmedia-used-audio-buffer-size", "Used Audio Buffer Size for Directmedia",
                         "Used Audio Buffer Size for Directmedia", 0, G_MAXINT, 0, RO));
    /* 0xe3 = READWRITE|STATIC_STRINGS, default FALSE */
    g_object_class_install_property(gobject_class, PROP_ES_BYPASS,
        g_param_spec_boolean("es-bypass", "ES Bypass Decoder & Renderer",
                             "ES Bypass Decoder & Renderer", FALSE, RW));
    g_object_class_install_property(gobject_class, PROP_DOWNMIX,
        g_param_spec_boolean("downmix", "Downmix decoder output to 2ch",
                             "Decoder output 2ch downmix for Multiview", FALSE, WO));
    g_object_class_install_property(gobject_class, PROP_SOUND_SYSTEM,
        g_param_spec_boxed("sound-system", "Sound System for IAMF", "Set Sound System for IAMF",
                           GST_TYPE_STRUCTURE, WO));
    g_signal_new("dialog-enhancement-limit-change", G_TYPE_FROM_CLASS(gobject_class),
                 G_SIGNAL_RUN_FIRST, G_STRUCT_OFFSET(GstLgAudioDecClass, dialog_enhancement_limit_change),
                 NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
}

/* @0x1aad4 (also inlined into every gst_lg_audiodec_<codec>_init) */
void gst_lg_adec_init(GstLgAudioDec *pAudioDec)
{
    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    gst_lxlog_init();
    GST_INFO("Compile info : %s %s", "Oct 16 2025", "05:55:01");
    pAudioDec->isMpeghUiUpdated = FALSE;
    pAudioDec->isAtmos = INPUT_STREAM_UNKNOWN;
    pAudioDec->index = (guint32)-1;
    pAudioDec->isTrickPlay = FALSE;
    pAudioDec->appType = NULL;
    pAudioDec->isFlush = FALSE;
    pAudioDec->decodedSize = 0;
    pAudioDec->isSeamless = FALSE;
    pAudioDec->isMpegDash = FALSE;
    pAudioDec->sendPrerollBuffer = FALSE;
    pAudioDec->currentState = GST_STATE_NULL;
    pAudioDec->nextState = GST_STATE_NULL;
    pAudioDec->bStreamStart = FALSE;
    pAudioDec->bIgnoreEOS = FALSE;
    pAudioDec->bWaitEOS = FALSE;
    pAudioDec->need_gap = FALSE;
    pAudioDec->set_push_gap = FALSE;
    pAudioDec->restart = FALSE;
    g_mutex_init(&pAudioDec->stream_start_mutex);
    g_cond_init(&pAudioDec->stream_start_cond);
    pAudioDec->in_timestamp = 0;
    pAudioDec->last_in_timestamp = 0;
    pAudioDec->pending_segment = NULL;
    pAudioDec->first_data_send = FALSE;
    pAudioDec->adec = lgadec_open();
    if (!pAudioDec->adec) {
        GST_ERROR("fail lgadec open!!");
        return;
    }
    if (lgadec_register_callback_fn(pAudioDec->adec, _gst_lg_adec_callback_task, pAudioDec) == -1)
        GST_ERROR("lgadec_register_callback_fn (pAudioDec->adec, _gst_lg_adec_callback_task, "
                  "(void *) pAudioDec) Err!");
}

/* @0x1ac98: one input buffer. DSP codecs go through lgadec_feed_data (-> decoder or, in
 * es-bypass mode, straight back out as LGADEC_DATA_BYPASS); no_dec codecs (PCM/ADPCM)
 * are converted on the host and pushed here. */
GstFlowReturn gst_lg_adec_chain(GstLgAudioDec *pAudioDec, GstBuffer *buffer)
{
    GstFlowReturn ret = GST_FLOW_OK;
    LGADEC_CODEC_T codec_type;
    guint buf_size;
    GstClockTime timestamp;

    if (!buffer) {
        GST_ERROR("buffer is NULL");
        return GST_FLOW_ERROR;
    }
    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        gst_buffer_unref(buffer);
        return GST_FLOW_ERROR;
    }
    buf_size = gst_buffer_get_size(buffer);
    GST_LOG_OBJECT(pAudioDec, "buffer: type=%s, size=%u, timestamp=%" GST_TIME_FORMAT ", func=%s:%d",
                   "IN", buf_size, GST_TIME_ARGS(GST_BUFFER_PTS(buffer)), "gst_lg_adec_chain", 0xb4b);
    GST_TRACE("input data: count=%" G_GUINT64_FORMAT ", size=%u, ts=%" G_GUINT64_FORMAT,
              pAudioDec->feededFrameCount, buf_size, GST_BUFFER_PTS(buffer));

    if (buf_size == 0) {
        GST_ERROR("buffer size is zero");
        ret = GST_FLOW_ERROR;
        gst_buffer_unref(buffer);
        goto out;
    }
    if (pAudioDec->elementStatus != DECODER_ELEMENT_STATUS_OK) {
        GST_ERROR("Decoding Error or Unsupported Audio or Resource Error");
        pAudioDec->decodedSize += buf_size;
        gst_buffer_unref(buffer);
        ret = GST_FLOW_ERROR;
        goto out;
    }
    if (pAudioDec->need_gap) {
        if (pAudioDec->set_push_gap) {
            GstEvent *gap = gst_event_new_gap(pAudioDec->segment.start, pAudioDec->segment.stop);
            GST_LOG_OBJECT(pAudioDec, "push gap event with time :%" GST_TIME_FORMAT " duration: %"
                           GST_TIME_FORMAT, GST_TIME_ARGS(pAudioDec->segment.start),
                           GST_TIME_ARGS(pAudioDec->segment.stop));
            if (gst_pad_push_event(pAudioDec->pSrcPad, gap))
                pAudioDec->set_push_gap = FALSE;
        }
        gst_buffer_unref(buffer);
        return GST_FLOW_OK;
    }
    if (_gst_lg_adec_no_audio(pAudioDec)) {
        GST_LOG_OBJECT(pAudioDec, "Drop buffer %p", buffer);
        gst_buffer_unref(buffer);
        return GST_FLOW_OK;
    }
    if (pAudioDec->isEnableDumpInput)
        _gst_lg_adec_dump_file(pAudioDec, buffer);
    if (lgadec_get_codec_type(pAudioDec->adec, &codec_type) == -1) {
        GST_ERROR("get codec type Err");
        ret = GST_FLOW_ERROR;
        gst_buffer_unref(buffer);
        goto out;
    }

    if (pAudioDec->isFlush) {
        if (pAudioDec->isFlush == TRUE) {
            GST_INFO("FLUSHING");
            ret = GST_FLOW_FLUSHING;
        } else if (pAudioDec->nextState < GST_STATE_PAUSED) {
            GST_INFO("next state is GST_STATE_READY");
        } else {
            GST_ERROR("feed function is NULL");
            ret = GST_FLOW_ERROR;
        }
        goto count;
    }
    if (pAudioDec->nextState < GST_STATE_PAUSED) {
        GST_INFO("next state is GST_STATE_READY");
        goto count;
    }

    lgadec_log_ts(pAudioDec->adec, GST_BUFFER_PTS(buffer), gst_buffer_get_size(buffer));
    timestamp = GST_BUFFER_PTS(buffer);
    {
        GstAudioClippingMeta *cmeta = gst_buffer_get_audio_clipping_meta(buffer);

        if (cmeta) {
            if (!clipping_meta_list)
                audio_clipping_meta_list_init();
            if (audio_clipping_meta_list_find(timestamp, NULL, NULL))
                audio_clipping_meta_list_add(timestamp, cmeta->start, cmeta->end,
                                             audio_clipping_meta_list_find_repeated_count(timestamp));
            else
                audio_clipping_meta_list_add(timestamp, cmeta->start, cmeta->end, 0);
            GST_TRACE("received audio clipping meta (timestamp: %lld, front: %lld, back %lld (usec))\n",
                      timestamp, cmeta->start, cmeta->end);
        }
    }

    {   /* _gst_lg_adec_feed_data, inlined */
        gint no_decoder;
        GstMapInfo buf_map_info;

        if (lgadec_get_no_decoder(pAudioDec->adec, &no_decoder) != 0) {
            GST_ERROR("no decoder check fail!!");
            ret = GST_FLOW_ERROR;
            goto feed_err;
        }
        if (no_decoder) {
            GST_LOG("no decoder feed");
            ret = _gst_lg_adec_no_decoder_feed_data(pAudioDec, buffer);
            if (ret == GST_FLOW_OK)
                goto fed;
            if (ret == GST_FLOW_CUSTOM_SUCCESS)
                goto count;
            goto feed_err;
        }
        if (pAudioDec->isVorbisCodec && GST_BUFFER_FLAG_IS_SET(buffer, GST_BUFFER_FLAG_HEADER)) {
            GST_LOG("VORBIS - GST_BUFFER_FLAG_HEADER is set. drop vorbis header.");
            ret = GST_FLOW_OK;
            goto count;
        }
        if (!gst_buffer_map(buffer, &buf_map_info, GST_MAP_READ)) {
            GST_ERROR("gst_buffer_map() Err");
            ret = GST_FLOW_ERROR;
            goto feed_err;
        }
        /* In es-bypass mode this returns only after the BYPASS buffer was pushed downstream
         * (lgadec_decoder_feed -> callback -> _gst_lg_adec_push_data, same thread). */
        if (lgadec_feed_data(pAudioDec->adec, buf_map_info.data, buf_map_info.size,
                             GST_BUFFER_PTS(buffer)) != 0) {
            GST_ERROR("data feed Fail!!");
            gst_buffer_unmap(buffer, &buf_map_info);
            ret = GST_FLOW_ERROR;
            goto feed_err;
        }
        gst_buffer_unmap(buffer, &buf_map_info);
    }
fed:
    pAudioDec->in_timestamp = timestamp;
    pAudioDec->feededFrameCount++;
    ret = GST_FLOW_OK;

count:
    if (pAudioDec->decodedSize == 0)
        MEDIA_LOG(0xbb1, "received buffer, %" GST_TIME_FORMAT, GST_TIME_ARGS(GST_BUFFER_PTS(buffer)));
    pAudioDec->decodedSize += buf_size;
    gst_buffer_unref(buffer);
    if (ret == GST_FLOW_OK)
        return GST_FLOW_OK;
    goto out;

feed_err:
    GST_ERROR("feed err (%s)", gst_flow_get_name(ret));
    gst_buffer_unref(buffer);
out:
    GST_DEBUG("Feed (index=%u, count=%" G_GUINT64_FORMAT ") Err (%s)", pAudioDec->index,
              pAudioDec->feededFrameCount, gst_flow_get_name(ret));
    return ret;
}

/* _gst_lg_adec_caps_is_equal, inlined into the CAPS case: TRUE = same stream, keep decoder */
static gboolean _gst_lg_adec_caps_is_equal(GstLgAudioDec *pAudioDec, const GstCaps *caps)
{
    const GstStructure *structure = gst_caps_get_structure(caps, 0), *prev_structure;
    const gchar *mime, *stream_format, *prev_stream_format;
    gint freq = 0, prev_freq = 0, ch = 0, prev_ch = 0;
    LGADEC_CODEC_T codec_type;

    if (!structure) {
        MEDIA_LOG(0xbd2, "unable to get structure from caps");
        return FALSE;
    }
    mime = gst_structure_get_name(structure);
    if (!mime) {
        MEDIA_LOG(0xbd7, "unable to get MIME type");
        return FALSE;
    }
    if (!pAudioDec->mime)
        return FALSE;
    if (strcmp(pAudioDec->mime, mime) != 0) {
        GST_INFO("mime is chagend (%s->%s)", pAudioDec->mime, mime);
        return FALSE;
    }
    if (!gst_structure_get_int(structure, "rate", &freq)) {
        MEDIA_LOG(0xbe6, "unable to get frequency");
        return FALSE;
    }
    if (lgadec_get_freq(pAudioDec->adec, &prev_freq) != 0) {
        MEDIA_LOG(0xbeb, "fail get freq");
        return FALSE;
    }
    if (prev_freq != freq) {
        GST_INFO("freq is chagend (%d->%d)", prev_freq, freq);
        return FALSE;
    }
    stream_format = gst_structure_get_string(structure, "stream-format");
    prev_structure = gst_caps_get_structure(pAudioDec->pAudioCaps, 0);
    prev_stream_format = gst_structure_get_string(prev_structure, "stream-format");
    if ((stream_format == NULL) != (prev_stream_format == NULL) ||
        (stream_format && strcmp(prev_stream_format, stream_format) != 0)) {
        GST_INFO("stream_format is changed");
        return FALSE;
    }
    if (lgadec_get_codec_type(pAudioDec->adec, &codec_type) != 0) {
        GST_ERROR("get codec type Err");
    } else if (codec_type != LGADEC_CODEC_AC4) {
        if (!gst_structure_get_int(structure, "channels", &ch)) {
            MEDIA_LOG(0xc0b, "unable to get channels");
            return FALSE;
        }
        if (!gst_structure_get_int(prev_structure, "channels", &prev_ch)) {
            MEDIA_LOG(0xc0f, "unable to get prev channels");
            return FALSE;
        }
        if (prev_ch != ch) {
            GST_INFO("channel is chagend (%d->%d)", prev_ch, ch);
            return FALSE;
        }
    }
    if (lgadec_get_codec_type(pAudioDec->adec, &codec_type) != 0) {
        GST_ERROR("get codec type Err");
        return TRUE;
    }
    if (codec_type == LGADEC_CODEC_AAC) {
        const GValue *codec_data = gst_structure_get_value(structure, "codec_data");
        const GValue *prev_codec_data = gst_structure_get_value(prev_structure, "codec_data");
        GstMapInfo m1 = GST_MAP_INFO_INIT, m2 = GST_MAP_INFO_INIT;
        GstBuffer *buf, *prev_buf;
        gboolean same;

        if (!codec_data) {
            MEDIA_LOG(0xc22, "unable to get codec_data");
            if (gst_structure_get_value(prev_structure, "codec_data"))
                goto changed;
            goto no_prev;               /* sic: two caps without codec_data never match */
        }
        if (!prev_codec_data) {
no_prev:
            MEDIA_LOG(0xc26, "unable to get prev codec_data");
changed:
            GST_INFO("codec_data is changed");
            return FALSE;
        }
        buf = gst_value_get_buffer(codec_data);
        prev_buf = gst_value_get_buffer(prev_codec_data);
        if (!gst_buffer_map(buf, &m1, GST_MAP_READ))
            MEDIA_LOG(0xc2f, "gst_buffer_map () Err");
        if (!gst_buffer_map(prev_buf, &m2, GST_MAP_READ))
            MEDIA_LOG(0xc34, "gst_buffer_map () Err");
        if (m1.size != m2.size) {
            GST_INFO("codec_data is changed");
            same = FALSE;
        } else {
            same = memcmp(m1.data, m2.data, m1.size) == 0;
            if (!same)
                GST_INFO("codec_data is changed");
        }
        gst_buffer_unmap(buf, &m1);
        gst_buffer_unmap(prev_buf, &m2);
        return same;
    }
    return TRUE;
}

/* CAPS: (re)configure lgadec. es-bypass decides whether the DSP decoder is opened at all;
 * the src pad always gets the DUMMY caps (with BYPASS) right after. */
static gboolean _gst_lg_adec_handle_caps(GstLgAudioDec *pAudioDec, GstCaps *caps)
{
    gboolean ret = FALSE;
    gint lg_ret;
    gchar *caps_string = gst_caps_to_string(caps);

    if (caps_string) {
        if (strlen(caps_string) < 0x400)
            MEDIA_LOG(0xd0c, "DEC.%d - caps \"%s\"", pAudioDec->index, caps_string);
        else
            GST_INFO("caps string len is %d. So doesn't print audio caps string",
                     (int)strlen(caps_string));
        g_free(caps_string);
    }

    if (!pAudioDec->pAudioCaps) {
        /* first caps of this element */
        pAudioDec->first_data_send = FALSE;
        pAudioDec->elementStatus = DECODER_ELEMENT_STATUS_INIT;
        pAudioDec->restart = TRUE;
        pAudioDec->decodedSize = 0;
        if (lgadec_start_init(pAudioDec->adec) == -1)
            GST_ERROR("lgadec_start_init (pAudioDec->adec) Err!");
        if (!lgadec_get_es_bypass(pAudioDec->adec) && lgadec_decoder_open(pAudioDec->adec) == -1)
            GST_ERROR("lgadec_decoder_open (pAudioDec->adec) Err!");
        else
            GST_INFO("Call Set Caps");
        if (_gst_lg_adec_set_caps(pAudioDec, caps) != GST_FLOW_OK)
            GST_ERROR("set_caps() Err");
        _gst_lg_adec_set_caps_srcpad_dummy(pAudioDec);
        if (pAudioDec->need_gap) {
            pAudioDec->pAudioCaps = gst_caps_copy(caps);
            ret = TRUE;
            goto done;
        }
        goto start;
    }

    /* caps renegotiation */
    {
        gboolean vorbis_restart = FALSE;
        const GstStructure *pStructure;
        gint preselection_id;

        pAudioDec->restart = FALSE;
        if (pAudioDec->bStreamStart) {
            GST_INFO("Codec changed. DSC start");
            if (pAudioDec->decodedSize == 0) {
                GST_LOG("do not wait EOS when no input data before playing");
            } else {
                pAudioDec->first_data_send = FALSE;
                pAudioDec->decodedSize = 0;
                _gst_lg_adec_wait_eos(pAudioDec);
            }
            vorbis_restart = pAudioDec->isVorbisCodec != 0;
        }
        pStructure = gst_caps_get_structure(caps, 0);
        if (!pStructure) {
            GST_ERROR("unable to get structure from caps");
        } else if (gst_structure_get_int(pStructure, "preselection-id", &preselection_id)) {
            lgadec_set_presentation_group_index(pAudioDec->adec, preselection_id);
            GST_INFO("presentation_group_index = %d", preselection_id);
        }
        if (gst_caps_is_equal(pAudioDec->pAudioCaps, caps) ||
            _gst_lg_adec_caps_is_equal(pAudioDec, caps)) {
            if (!vorbis_restart) {
                ret = TRUE;
                goto done;
            }
        } else if (pAudioDec->need_gap) {
            goto set_caps;
        }
    }

    MEDIA_LOG(0xd63, "decoder restart");
    pAudioDec->restart = TRUE;
    if (lgadec_stop(pAudioDec->adec) == -1)
        GST_ERROR("lgadec_stop (pAudioDec->adec) Err!");
    if (lgadec_decoder_close(pAudioDec->adec) == -1)
        GST_ERROR("lgadec_decoder_close (pAudioDec->adec) Err!");
    pAudioDec->elementStatus = DECODER_ELEMENT_STATUS_INIT;
    if (lgadec_start_init(pAudioDec->adec) == -1)
        GST_ERROR("lgadec_start_init (pAudioDec->adec) Err!");
    if (!lgadec_get_es_bypass(pAudioDec->adec) && lgadec_decoder_open(pAudioDec->adec) == -1)
        GST_ERROR("lgadec_decoder_open (pAudioDec->adec) Err!");
    else
        GST_INFO("Call Set Caps");
set_caps:
    if (_gst_lg_adec_set_caps(pAudioDec, caps) != GST_FLOW_OK)
        GST_ERROR("set_caps() Err");
    _gst_lg_adec_set_caps_srcpad_dummy(pAudioDec);
    if (pAudioDec->need_gap) {
        ret = TRUE;
        goto replace;
    }

start:
    lg_ret = lgadec_start(pAudioDec->adec);
    if (lg_ret == LGADEC_RET_NOT_SUPPORTED) {
        GST_ERROR_OBJECT(pAudioDec, "Not supported codec");
        GST_ELEMENT_ERROR(pAudioDec, STREAM, DECODE, ("Failed to decode stream"), (NULL));
    } else if (lg_ret == -1) {
        GST_ERROR_OBJECT(pAudioDec, "lgadec_start error!");
    } else {
        ret = TRUE;
    }
replace:
    if (pAudioDec->pAudioCaps)
        gst_caps_unref(pAudioDec->pAudioCaps);
    pAudioDec->pAudioCaps = gst_caps_copy(caps);
done:
    pAudioDec->bStreamStart = FALSE;
    return ret;
}

/* @0x1d404: sink pad events. Return TRUE = forward with gst_pad_event_default. */
gboolean gst_lg_adec_downstream_event(GstLgAudioDec *pAudioDec, GstEvent *event)
{
    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return FALSE;
    }

    switch (GST_EVENT_TYPE(event)) {
    case GST_EVENT_FLUSH_START:
        MEDIA_LOG(0xc88, "got event(%s)", GST_EVENT_TYPE_NAME(event));
        _gst_lg_adec_signal_eos(pAudioDec);
        pAudioDec->isFlush = TRUE;
        lgadec_set_flushing(pAudioDec->adec, 1);
        return TRUE;

    case GST_EVENT_FLUSH_STOP:
        MEDIA_LOG(0xc94, "got event(%s)", GST_EVENT_TYPE_NAME(event));
        if (!pAudioDec->need_gap)
            lgadec_flush(pAudioDec->adec);
        lgadec_set_flushing(pAudioDec->adec, 0);
        pAudioDec->bStreamStart = FALSE;
        pAudioDec->first_data_send = FALSE;
        pAudioDec->decodedSize = 0;
        pAudioDec->isFlush = FALSE;
        return TRUE;

    case GST_EVENT_EOS:
        MEDIA_LOG(0xca5, "got event(%s)", GST_EVENT_TYPE_NAME(event));
        if (pAudioDec->need_gap) {
            GST_INFO("Send EOS righ now when need_gap is TRUE");
            return TRUE;
        }
        if (_gst_lg_adec_no_audio(pAudioDec)) {
            GST_INFO("Send EOS right now when trick play");
            return TRUE;
        }
        if (pAudioDec->decodedSize == 0) {
            GST_INFO("Send EOS right now when no input data before playing");
            return TRUE;
        }
        /* es-bypass: nothing is in the DSP, forward EOS immediately */
        if (lgadec_get_es_bypass(pAudioDec->adec)) {
            GST_INFO("Send EOS right now when es bypass mode\n");
            return TRUE;
        }
        {
            gint lg_ret = lgadec_feed_eos(pAudioDec->adec);
            gboolean ret = FALSE;      /* 0: EOS comes back later via LGADEC_EVENT_EOS */

            if (lg_ret != 0) {
                if (lg_ret == -1)
                    GST_ERROR("lgadec_event_eos Err!!");
                ret = TRUE;
            }
            pAudioDec->first_data_send = FALSE;
            pAudioDec->decodedSize = 0;
            return ret;
        }

    case GST_EVENT_CAPS: {
        GstCaps *caps;

        MEDIA_LOG(0xd03, "got event(%s)", GST_EVENT_TYPE_NAME(event));
        gst_event_parse_caps(event, &caps);
        return _gst_lg_adec_handle_caps(pAudioDec, caps);
    }

    case GST_EVENT_SEGMENT: {
        const GstSegment *pSegment;

        MEDIA_LOG(0xccb, "got event(%s)", GST_EVENT_TYPE_NAME(event));
        gst_event_parse_segment(event, &pSegment);
        GST_INFO("rate %6.2f, format %d, start %" G_GINT64_FORMAT ", stop %" G_GINT64_FORMAT
                 ", pos %" G_GINT64_FORMAT, pSegment->rate, pSegment->format, pSegment->start,
                 pSegment->stop, pSegment->position);
        GST_INFO("Segment --> %" GST_SEGMENT_FORMAT, pSegment);
        if (pAudioDec->need_gap) {
            pAudioDec->set_push_gap = TRUE;
            gst_segment_copy_into(pSegment, &pAudioDec->segment);
            return TRUE;
        }
        lgadec_set_play_speed(pAudioDec->adec, play_speed_from_rate(pSegment->rate));
        lgadec_set_basetime(pAudioDec->adec, pSegment->start);
        gst_segment_copy_into(pSegment, &pAudioDec->segment);
        if (!pAudioDec->pAudioCaps)
            GST_ERROR("audio caps is NULL. Set caps will be called on GST_EVENT_CAPS event");
        if (_gst_lg_adec_no_audio(pAudioDec)) {
            GST_INFO("Pushing GAP event if NO AUDIO !!");
            gst_pad_push_event(pAudioDec->pSrcPad, gst_event_new_gap(0, GST_CLOCK_TIME_NONE));
        }
        if (!pAudioDec->first_data_send || pAudioDec->currentState != GST_STATE_PLAYING)
            return TRUE;
        /* while playing: hold the segment until decoded PCM passed in_timestamp */
        if (pAudioDec->pending_segment)
            gst_event_unref(pAudioDec->pending_segment);
        pAudioDec->pending_segment = gst_event_ref(event);
        pAudioDec->last_in_timestamp = pAudioDec->in_timestamp;
        return FALSE;
    }

    case GST_EVENT_STREAM_START: {
        const gchar *stream_id = NULL;

        MEDIA_LOG(0xd98, "got event(%s)", GST_EVENT_TYPE_NAME(event));
        gst_event_parse_stream_start(event, &stream_id);
        if (!stream_id) {
            GST_ERROR("Stream ID is NULL");
            return TRUE;
        }
        if (!pAudioDec->pStreamId) {
            pAudioDec->pStreamId = g_strdup(stream_id);
            pAudioDec->bStreamStart = TRUE;
            MEDIA_LOG(0xda3, "new stream-start ID: %s", stream_id);
            return TRUE;
        }
        if (strcmp(pAudioDec->pStreamId, stream_id) == 0) {
            MEDIA_LOG(0xdb2, "same stream-start ID: %s", stream_id);
            return TRUE;
        }
        g_free(pAudioDec->pStreamId);
        pAudioDec->pStreamId = g_strdup(stream_id);
        pAudioDec->bStreamStart = TRUE;
        MEDIA_LOG(0xda9, "changed stream-start ID: %s", stream_id);
        if (pAudioDec->decodedSize) {
            pAudioDec->first_data_send = FALSE;
            pAudioDec->decodedSize = 0;
            _gst_lg_adec_wait_eos(pAudioDec);
        } else {
            GST_LOG("do not wait EOS when no input data before playing");
        }
        return TRUE;
    }

    case GST_EVENT_CUSTOM_DOWNSTREAM: {
        const GstStructure *structure;
        guint64 before = GST_CLOCK_TIME_NONE, after = GST_CLOCK_TIME_NONE;

        MEDIA_LOG(0xdba, "got event(%s)", GST_EVENT_TYPE_NAME(event));
        structure = gst_event_get_structure(event);
        if (!structure || !gst_structure_has_name(structure, "audio-gap"))
            return TRUE;
        if (!gst_structure_has_field(structure, "pts-before-gap"))
            MEDIA_LOG(0xdc8, "no pts-before-gap field on structure");
        else
            gst_structure_get_uint64(structure, "pts-before-gap", &before);
        if (!gst_structure_has_field(structure, "pts-after-gap"))
            MEDIA_LOG(0xdce, "no pts-after-gap field on structure");
        else
            gst_structure_get_uint64(structure, "pts-after-gap", &after);
        if (before == GST_CLOCK_TIME_NONE || after == GST_CLOCK_TIME_NONE)
            return TRUE;
        MEDIA_LOG(0xdd2, "audio-gap custom event received, before %lld, after %lld",
                  (long long)before, (long long)after);
        lgadec_decoder_set_gap_info(pAudioDec->adec, before, after);
        return TRUE;
    }

    case GST_EVENT_INSTANT_RATE_CHANGE: {
        gdouble rate = 0.0;
        LGADEC_CODEC_T codec_type;
        guint sound_system = 0;

        gst_event_parse_instant_rate_change(event, &rate, NULL);
        rate *= pAudioDec->segment.rate;
        MEDIA_LOG(0xde9, "got event(%s), rate = %f", GST_EVENT_TYPE_NAME(event), rate);
        if (lgadec_get_codec_type(pAudioDec->adec, &codec_type) != 0) {
            GST_ERROR("get codec type Err");
            return TRUE;
        }
        if (codec_type != LGADEC_CODEC_IAMF)
            return TRUE;
        /* _gst_lg_adec_set_sound_system, inlined: >1x playback falls back to stereo */
        if (rate <= 1.0) {
            if (pAudioDec->is_device_iamf_supported == TRUE)
                sound_system = (pAudioDec->channel_capa == 8) << 1;
            else if (pAudioDec->is_device_iamf_supported == FALSE)
                sound_system = (pAudioDec->channel_capa == 8) << 3;
        }
        lgadec_set_sound_system(pAudioDec->adec, sound_system);
        return TRUE;
    }

    default:
        GST_INFO("got event(%s)", GST_EVENT_TYPE_NAME(event));
        return TRUE;
    }
}

/* @0x1f808 */
gboolean gst_lg_adec_upstream_event(GstLgAudioDec *pAudioDec, GstEvent *event)
{
    GST_INFO("got event(%s)", GST_EVENT_TYPE_NAME(event));
    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return FALSE;
    }
    if (GST_EVENT_TYPE(event) == GST_EVENT_CUSTOM_UPSTREAM) {
        gboolean active = TRUE;

        if (gst_event_has_name(event, "acquired-resource"))
            gst_structure_get_boolean(gst_event_get_structure(event), "active", &active);
        if (gst_event_has_name(event, "mpegh-object-ui"))
            GST_INFO("received mpegh-object-ui event from downstream");
    }
    return TRUE;
}

/* @0x1f93c */
gboolean gst_lg_adec_set_property(GstLgAudioDec *pAudioDec, guint prop_id, const GValue *value,
                                  GParamSpec *pspec)
{
    gchar *string;

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return FALSE;
    }
    string = g_strdup_value_contents(value);
    GST_INFO("set_property \"%s\"(id:%d) = %s:'%s'", pspec->name, prop_id,
             G_VALUE_TYPE_NAME(value), string);
    g_free(string);

    switch (prop_id) {
    case PROP_TRICK_MODE:
        pAudioDec->isTrickPlay = g_value_get_boolean(value);
        return TRUE;

    case PROP_APP_TYPE: {
        LGADEC_APP_TYPE_T app_type = LGADEC_APP_TYPE_UNKNOWN;

        g_clear_pointer(&pAudioDec->appType, g_free);
        pAudioDec->appType = g_value_dup_string(value);
        if (!pAudioDec->appType)
            return TRUE;
        if (!strcmp(pAudioDec->appType, "RTC")) {
            app_type = LGADEC_APP_TYPE_RTC;
            GST_INFO("This application is WiDi");
        } else if (!strcmp(pAudioDec->appType, "MIRACAST")) {
            app_type = LGADEC_APP_TYPE_MIRACAST;
            GST_INFO("This application is Miracast");
        } else if (!strcmp(pAudioDec->appType, "JP4K")) {
            app_type = LGADEC_APP_TYPE_JP4K;
        } else if (!strcmp(pAudioDec->appType, "APPCAST")) {
            app_type = LGADEC_APP_TYPE_APPCAST;
        } else if (!strcmp(pAudioDec->appType, "SYE")) {
            app_type = LGADEC_APP_TYPE_SYE;
        }
        if (lgadec_set_app_type(pAudioDec->adec, app_type) == -1)
            GST_ERROR("lgadec_set_app_type (pAudioDec->adec, app_type) Err!");
        return TRUE;
    }

    case PROP_RESOURCE_INFO: {
        const GstStructure *pMediaInfoStruct = gst_value_get_structure(value);
        gint audioPort = -1;

        if (!pMediaInfoStruct)
            return TRUE;
        if (!gst_structure_has_field(pMediaInfoStruct, "audio-port")) {
            GST_ERROR("resource-info has not audio-port field");
            return TRUE;
        }
        gst_structure_get_int(pMediaInfoStruct, "audio-port", &audioPort);
        pAudioDec->index = audioPort;
        if (lgadec_set_index(pAudioDec->adec, audioPort) != 0) {
            GST_ERROR("set index Err");
            return FALSE;
        }
        MEDIA_LOG(0xe75, "Set audio index(resource-info) to %d", pAudioDec->index);
        return TRUE;
    }

    case PROP_INDEX: {
        guint32 index = g_value_get_uint(value);

        GST_INFO("Set audio index to %u", index);
        pAudioDec->index = index;
        if (lgadec_set_index(pAudioDec->adec, index) != 0) {
            GST_ERROR("set index Err");
            return FALSE;
        }
        return TRUE;
    }

    case PROP_SEAMLESS:
        pthread_mutex_lock(&pAudioDec->adec_mutex);
        pAudioDec->isSeamless = g_value_get_boolean(value);
        if (lgadec_set_seamless(pAudioDec->adec, pAudioDec->isSeamless) == -1)
            GST_ERROR("lgadec_set_seamless (pAudioDec->adec, pAudioDec->isSeamless) Err!");
        pthread_mutex_unlock(&pAudioDec->adec_mutex);
        GST_INFO("Set Seamless audio change(TRUE=Enable, FALSE=Disable): %s",
                 pAudioDec->isSeamless ? "TRUE" : "FALSE");
        return TRUE;

    case PROP_MPEG_DASH:
        pAudioDec->isMpegDash = g_value_get_boolean(value);
        GST_INFO("Set MPEG-DASH Player(TRUE=Enable, FALSE=Disable): %s",
                 pAudioDec->isMpegDash ? "TRUE" : "FALSE");
        return TRUE;

    case PROP_PLAY_SPEED:
        if (lgadec_set_play_speed(pAudioDec->adec, g_value_get_int(value)) == -1)
            GST_ERROR("lgadec_set_play_speed (pAudioDec->adec, play_speed) Err!");
        return TRUE;

    case PROP_OUTPUT_MODE:
        if (lgadec_set_output_mode(pAudioDec->adec, g_value_get_uint(value)) == -1)
            GST_ERROR("lgadec_set_output_mode (pAudioDec->adec, output_mode) Err!");
        return TRUE;

    case PROP_MPEGH_UI_XML: {
        gchar *val = g_value_dup_string(value);

        if (!val) {
            GST_WARNING("value is NULL");
            return FALSE;
        }
        GST_LOG("%s, %d", val, (int)strlen(val));
        if (lgadec_decoder_set_ui_param(pAudioDec->adec, val, strlen(val)) != 0) {
            GST_ERROR("lgadec_decoder_set_ui_param Err");
            return FALSE;   /* sic: val leaks, as in the original */
        }
        return TRUE;
    }

    case PROP_DEFAULT_PRL: {
        gint default_prl = g_value_get_int(value);

        if (default_prl < -31 || default_prl > 0) {
            GST_ERROR("invalid default_prl, default_prl: %d\n", default_prl);
            return FALSE;
        }
        if (lgadec_set_default_prl(pAudioDec->adec, default_prl) == -1)
            GST_ERROR("lgadec_set_default_prl (pAudioDec->adec, default_prl) Err!");
        GST_INFO("Set default PRL: %d", default_prl);
        return TRUE;
    }

    case PROP_AC4_DE_GAIN: {
        guint gain = g_value_get_uint(value);

        GST_INFO("dialog enhancement gain: %d\n", gain);
        if (lgadec_set_ac4_dialog_enhancement_gain(pAudioDec->adec, gain) == -1)
            GST_ERROR("lgadec_set_ac4_dialog_enhancement_gain (pAudioDec->adec, gain) Err!");
        return TRUE;
    }

    case PROP_ES_BYPASS: {
        /* set by the application/pipeline owner (no in-binary caller); see lgadec_set_es_bypass */
        gboolean es_bypass;

        pthread_mutex_lock(&pAudioDec->adec_mutex);
        es_bypass = g_value_get_boolean(value);
        if (lgadec_set_es_bypass(pAudioDec->adec, es_bypass != FALSE) == -1)
            GST_ERROR("lgadec_set_es_bypass (pAudioDec->adec, es_bypass) Err!");
        pthread_mutex_unlock(&pAudioDec->adec_mutex);
        MEDIA_LOG(0xef7, "Set ES Bypass (TRUE=bypass, FALSE=decoding): %s", es_bypass ? "TRUE" : "FALSE");
        GST_INFO("Set ES Bypass (TRUE=bypass, FALSE=decoding): %s", es_bypass ? "TRUE" : "FALSE");
        return TRUE;
    }

    case PROP_DOLBY_DRC_MODE: {
        guint drc_mode = g_value_get_uint(value);

        lgadec_set_dolby_drc_mode(pAudioDec->adec, drc_mode);
        GST_INFO("Set dolby drc mode: %d (%s)", drc_mode, drc_mode ? "LGADEC_DRC_RF" : "LGADEC_DRC_LINE");
        return TRUE;
    }

    case PROP_DOWNMIX: {
        gboolean downmix = g_value_get_boolean(value);

        lgadec_set_downmix(pAudioDec->adec, downmix != FALSE);
        GST_INFO("Set downmix: %d (%s)", downmix, downmix ? "enable" : "disable");
        return TRUE;
    }

    case PROP_SOUND_SYSTEM: {
        const GstStructure *structure;
        gboolean is_device_iamf_supported = FALSE;
        guint channel_capa = 0, sound_system;

        MEDIA_LOG(0xf20, "Set PROP_SOUND_SYSTEM");
        structure = gst_value_get_structure(value);
        if (!structure)
            return TRUE;
        if (!gst_structure_has_field(structure, "is_device_iamf_supported") ||
            !gst_structure_has_field(structure, "channel_capa")) {
            GST_ERROR("sound-mode has not is_device_iamf_supported, channel_capa");
            return TRUE;
        }
        gst_structure_get_boolean(structure, "is_device_iamf_supported", &is_device_iamf_supported);
        gst_structure_get_uint(structure, "channel_capa", &channel_capa);
        pAudioDec->is_device_iamf_supported = is_device_iamf_supported;
        pAudioDec->channel_capa = channel_capa;
        /* _gst_lg_adec_set_sound_system, inlined: 8ch device -> 5.1.2 (IAMF) / 7.1 (else) */
        if (is_device_iamf_supported == TRUE)
            sound_system = (channel_capa == 8) << 1;
        else
            sound_system = (channel_capa == 8 && !(is_device_iamf_supported || channel_capa == 2)) << 3;
        MEDIA_LOG(0xf2e, "is_device_iamf_supported = %s", is_device_iamf_supported ? "TRUE" : "FALSE");
        MEDIA_LOG(0xf2f, "channel_capa = %d", channel_capa);
        MEDIA_LOG(0xf30, "sound_system = %d, layout = %s\n", sound_system, sound_system_name[sound_system]);
        lgadec_set_sound_system(pAudioDec->adec, sound_system);
        return TRUE;
    }

    default:
        return FALSE;
    }
}

/* @0x2082c */
gboolean gst_lg_adec_get_property(GstLgAudioDec *pAudioDec, guint prop_id, GValue *value,
                                  GParamSpec *pspec)
{
    guint64 cpbBufferingSize = 0;
    gboolean ret = TRUE;
    gchar *string;

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return FALSE;
    }
    switch (prop_id) {
    case PROP_DECODED_SIZE:
        if (lgadec_decoder_get_cpb_status(pAudioDec->adec, &cpbBufferingSize, NULL, NULL, NULL) != 0) {
            GST_INFO("Get CPB buffer status Err");
            cpbBufferingSize = 0;
        } else if (pAudioDec->decodedSize <= cpbBufferingSize) {
            cpbBufferingSize = 0;
        } else {
            /* sic: the original returns cpbBufferingSize here, not decodedSize - cpb;
             * kept as read */
        }
        g_value_set_uint64(value, cpbBufferingSize);
        break;
    case PROP_UNDECODED_SIZE:
        if (lgadec_decoder_get_cpb_status(pAudioDec->adec, &cpbBufferingSize, NULL, NULL, NULL) != 0) {
            GST_INFO("Get CPB buffer status Err");
            cpbBufferingSize = 0;
        }
        g_value_set_uint64(value, cpbBufferingSize);
        break;
    case PROP_TRICK_MODE:
        g_value_set_boolean(value, pAudioDec->isTrickPlay);
        break;
    case PROP_APP_TYPE:
        g_value_set_string(value, pAudioDec->appType);
        break;
    case PROP_INDEX:
        g_value_set_uint(value, pAudioDec->index);
        break;
    case PROP_SEAMLESS:
        g_value_set_boolean(value, pAudioDec->isSeamless);
        break;
    case PROP_MPEG_DASH:
        g_value_set_boolean(value, pAudioDec->isMpegDash);
        break;
    case PROP_PLAY_SPEED: {
        gint play_speed;

        if (lgadec_get_play_speed(pAudioDec->adec, &play_speed) == -1)
            GST_ERROR("lgadec_get_play_speed (pAudioDec->adec, &play_speed) Err!");
        else
            g_value_set_int(value, play_speed);
        break;
    }
    case PROP_OUTPUT_MODE: {
        guint output_mode;

        if (lgadec_get_output_mode(pAudioDec->adec, &output_mode) != 0) {
            GST_ERROR("get output mode Err");
            return FALSE;
        }
        g_value_set_uint(value, output_mode);
        break;
    }
    case PROP_VERSION:
        g_value_set_string(value, "v1.00.00-2017.10.13");
        break;
    case PROP_MPEGH_UI_XML: {
        char *data = NULL;

        if (lgadec_decoder_get_ui_param(pAudioDec->adec, &data) != 0) {
            GST_LOG("lgadec_decoder_get_ui_param Err");
            ret = FALSE;
            data = NULL;
        }
        g_value_set_string(value, data);
        g_free(data);
        break;
    }
    case PROP_DIRECTMEDIA_TOTAL_BUF_SIZE:
    case PROP_DIRECTMEDIA_USED_BUF_SIZE: {
        guint size = 0;
        gboolean total = prop_id == PROP_DIRECTMEDIA_TOTAL_BUF_SIZE;

        if (lgadec_get_buffer_size_for_directmedia(pAudioDec->adec, &size,
                total ? LGADEC_DIRECTMEDIA_TOTAL_BUF : LGADEC_DIRECTMEDIA_USED_BUF) == -1)
            GST_ERROR(total ? "lgadec_get_buffer_size_for_directmedia (pAudioDec->adec, &size, "
                              "LGADEC_DIRECTMEDIA_TOTAL_BUF) Err!"
                            : "lgadec_get_buffer_size_for_directmedia (pAudioDec->adec, &size, "
                              "LGADEC_DIRECTMEDIA_USED_BUF) Err!");
        g_value_set_int(value, size);
        break;
    }
    case PROP_AC4_DE_GAIN:
    case PROP_AC4_DE_LIMIT_MIN:
    case PROP_AC4_DE_LIMIT_MAX:
    case PROP_DOLBY_DRC_MODE: {
        guint v = 0;
        int lg_ret;

        if (prop_id == PROP_AC4_DE_GAIN) {
            lg_ret = lgadec_get_ac4_dialog_enhancement_gain(pAudioDec->adec, &v);
            GST_INFO("dialog enhacement gain: %d\n", v);
        } else if (prop_id == PROP_AC4_DE_LIMIT_MIN) {
            lg_ret = lgadec_get_ac4_dialog_enhancement_limit_min(pAudioDec->adec, &v);
        } else if (prop_id == PROP_AC4_DE_LIMIT_MAX) {
            lg_ret = lgadec_get_ac4_dialog_enhancement_limit_max(pAudioDec->adec, &v);
            GST_INFO("dialog enhancement limit (max): %d (dB)\n", v);
        } else {
            lg_ret = lgadec_get_dolby_drc_mode(pAudioDec->adec, &v);
            GST_INFO("dolby drc mode: %d\n", v);
        }
        if (lg_ret != 0) {
            GST_ERROR("get property %u Err", prop_id);   /* per-property texts in the original */
            return FALSE;
        }
        g_value_set_uint(value, v);
        break;
    }
    case PROP_ES_BYPASS:
        g_value_set_boolean(value, lgadec_get_es_bypass(pAudioDec->adec));
        break;
    default:
        ret = FALSE;
        break;
    }
    string = g_strdup_value_contents(value);
    GST_LOG("get_property \"%s\"(id:%d) = %s:'%s'", pspec->name, prop_id, G_VALUE_TYPE_NAME(value), string);
    g_free(string);
    return ret;
}

/* _gst_lg_adec_convert_gst_state, inlined */
static LGADEC_GST_STATE_T _gst_lg_adec_convert_gst_state(GstState state)
{
    switch (state) {
    case GST_STATE_VOID_PENDING: return LGADEC_GST_STATE_VOID_PENDING;
    case GST_STATE_NULL: return LGADEC_GST_STATE_NULL;
    case GST_STATE_READY: return LGADEC_GST_STATE_READY;
    case GST_STATE_PAUSED: return LGADEC_GST_STATE_PAUSED;
    case GST_STATE_PLAYING: return LGADEC_GST_STATE_PLAYING;
    default:
        GST_ERROR("Unkonwn GstState (%d)", state);
        return LGADEC_GST_STATE_NULL;
    }
}

/* @0x20f14: before chaining up to GstElement */
void gst_lg_adec_change_state_up(GstLgAudioDec *pAudioDec, GstStateChange transition)
{
    GstState next = GST_STATE_TRANSITION_NEXT(transition);

    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    MEDIA_LOG(0x105b, "change state (up) from %s to %s",
              gst_element_state_get_name(GST_STATE_TRANSITION_CURRENT(transition)),
              gst_element_state_get_name(next));
    pthread_mutex_lock(&pAudioDec->adec_mutex);
    pAudioDec->nextState = next;
    {
        LGADEC_GST_STATE_T lgadec_next_state = _gst_lg_adec_convert_gst_state(next);
        if (pAudioDec->nextState != GST_STATE_NULL &&
            lgadec_set_next_state(pAudioDec->adec, lgadec_next_state) == -1)
            GST_ERROR("lgadec_set_next_state (pAudioDec->adec, lgadec_next_state) Err!");
    }
    pthread_mutex_unlock(&pAudioDec->adec_mutex);

    switch (transition) {
    case GST_STATE_CHANGE_READY_TO_PAUSED:     /* _gst_lg_adec_start, inlined */
        pAudioDec->currentState = GST_STATE_PAUSED;
        pthread_mutex_init(&pAudioDec->cb_mutex, NULL);
        pthread_mutex_init(&pAudioDec->adec_mutex, NULL);
        _gst_lg_adec_init_variables(pAudioDec);
        pAudioDec->isEnableDumpInput = access("/tmp/gst_dump_audio_input", F_OK) == 0;
        MEDIA_LOG(0xb07, "start");
        goto done_log;          /* skips the DEBUG "DONE" */
    case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
        if (!pAudioDec->need_gap) {
            pAudioDec->currentState = GST_STATE_PLAYING;
            lgadec_set_play_speed(pAudioDec->adec, play_speed_from_rate(pAudioDec->segment.rate));
        }
        break;
    case GST_STATE_CHANGE_NULL_TO_READY:
        pAudioDec->currentState = GST_STATE_READY;
        break;
    default:
        break;
    }
    GST_DEBUG("change state from %s to %s. DONE",
              gst_element_state_get_name(GST_STATE_TRANSITION_CURRENT(transition)),
              gst_element_state_get_name(next));
done_log:
    MEDIA_LOG(0x1084, "change state from %s to %s. DONE",
              gst_element_state_get_name(GST_STATE_TRANSITION_CURRENT(transition)),
              gst_element_state_get_name(next));
}

/* @0x21334: after chaining up */
void gst_lg_adec_change_state_down(GstLgAudioDec *pAudioDec, GstStateChange transition)
{
    if (!pAudioDec) {
        GST_ERROR("pAudioDec is NULL");
        return;
    }
    MEDIA_LOG(0x1094, "change state (down) from %s to %s",
              gst_element_state_get_name(GST_STATE_TRANSITION_CURRENT(transition)),
              gst_element_state_get_name(GST_STATE_TRANSITION_NEXT(transition)));
    switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:     /* _gst_lg_adec_stop, inlined */
        pAudioDec->currentState = GST_STATE_READY;
        _gst_lg_adec_signal_eos(pAudioDec);
        _gst_lg_adec_init_variables(pAudioDec);
        if (pAudioDec->isEnableDumpInput) {
            if (pAudioDec->pDumpAudioFd)
                fclose(pAudioDec->pDumpAudioFd);
            pAudioDec->pDumpAudioFd = NULL;
        }
        audio_clipping_meta_list_cleanup();
        if (lgadec_stop(pAudioDec->adec) == -1)
            GST_ERROR("lgadec_stop (pAudioDec->adec) Err!");
        if (lgadec_decoder_close(pAudioDec->adec) == -1)
            GST_ERROR("lgadec_decoder_close (pAudioDec->adec) Err!");
        if (lgadec_close(pAudioDec->adec) == -1)
            GST_ERROR("lgadec_close (pAudioDec->adec) Err!");
        pthread_mutex_destroy(&pAudioDec->cb_mutex);
        pthread_mutex_destroy(&pAudioDec->adec_mutex);
        g_mutex_clear(&pAudioDec->stream_start_mutex);
        g_cond_clear(&pAudioDec->stream_start_cond);
        MEDIA_LOG(0xb30, "stop");
        goto done_log;
    case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
        pAudioDec->currentState = GST_STATE_PAUSED;
        if (!pAudioDec->need_gap) {
            lgadec_set_play_speed(pAudioDec->adec, 0);
            lgadec_set_preroll(pAudioDec->adec);   /* -> LGADEC_EVENT_PREROLL -> NULL_DATA buffer */
        } else {
            GST_LOG_OBJECT(pAudioDec, "Push gap event for preroll when changing state PLAYING to PAUSED");
            gst_pad_push_event(pAudioDec->pSrcPad,
                               gst_event_new_gap(pAudioDec->segment.start, pAudioDec->segment.stop));
        }
        break;
    case GST_STATE_CHANGE_READY_TO_NULL:
        pAudioDec->currentState = GST_STATE_NULL;
        gst_lxlog_deinit();
        break;
    default:
        break;
    }
    GST_DEBUG("change state from %s to %s. DONE",
              gst_element_state_get_name(GST_STATE_TRANSITION_CURRENT(transition)),
              gst_element_state_get_name(GST_STATE_TRANSITION_NEXT(transition)));
done_log:
    MEDIA_LOG(0x10c4, "change state from %s to %s. DONE",
              gst_element_state_get_name(GST_STATE_TRANSITION_CURRENT(transition)),
              gst_element_state_get_name(GST_STATE_TRANSITION_NEXT(transition)));
}
