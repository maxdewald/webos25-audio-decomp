/*
 * lgadec: decoder-control layer of the LG audio decoder elements
 * (gst-plugins-bsp/gstlgaudio/lgaudio/lgadec.c). Owns /dev/lg/adec0 and the aimc chain
 *     appsrc --> [ADEC] --port0--> appsink         (PCM / metadata)
 *                       \--port1--> bypass_appsink  (DSP pass-through ES)
 * and the "ES BYPASS & DROP" mode in which the DSP decoder is closed and the input ES
 * is handed straight back to the element as LGADEC_DATA_BYPASS.
 *
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so
 *   sha256 84db367e1ea3eeb4b589bf63910140cfd43ea77f2db124ae6882fbee8ebb6bb7
 * Names, prototypes, struct layouts: DWARF (CU lgadec.c). Tags are Ghidra addresses
 * (raw VA + 0x10000). Evidence: build/decomp/libgstlgaudiodec.so.c, .strings.tsv, .funcstrings.tsv,
 * build/scratch/evidence/renderer-abi/decoder-set_es_bypass.asm.
 * lgadec_decoder_set_codec_param (@0x2a414) was recovered from the disassembly (Ghidra
 * failed). The codec feeders are in lgadec_codec.c.
 *
 * The ioctl ABI recorded in this library's DWARF has no DTS: audio_decoder_codec
 * (DWARF) runs ...FLAC=16, OPUS=17, AAC_BYPASS=18, AAC_ELD=19, RA8=20, IAMF=21, whereas the
 * published lg-opensource/kdriver aud_kapi_decoder.h has DTS=17, DTS_HD=18, OPUS=19...
 * ADEC_IOR_GET_DTS_SUPPORTED / _GET_DTS_BITSTREAM_PASSTHROUGH (24/25) are never issued.
 */
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "lgadec.h"

_Static_assert(sizeof(lgadec) == 0x3e0, "lgadec size (DWARF)");
_Static_assert(offsetof(lgadec, prev_timestamp) == 0x238, "prev_timestamp");
_Static_assert(offsetof(lgadec, no_dec) == 0x2c8, "no_dec");
_Static_assert(offsetof(lgadec, bypass_decoder) == 0x368, "bypass_decoder");
_Static_assert(offsetof(lgadec, resource_mutex) == 0x370, "resource_mutex");
_Static_assert(offsetof(lgadec, list_pending_data) == 0x3dc, "list_pending_data");
_Static_assert(sizeof(LGADEC_AUDIO_CAPS_INFO) == 0x58, "caps info");
_Static_assert(sizeof(LGADEC_DATA_T) == 0x18, "LGADEC_DATA_T");

/* ---- /dev/lg/adec* (aud_kapi_decoder.h, magic 'D') ---------------------------- */
#define ADEC_IOW_SET_INDEX   _IOW('D', 1, int)
#define ADEC_IOW_SET_CODEC   _IOW('D', 2, unsigned int)
#define ADEC_IOR_GET_UID     _IOR('D', 4, unsigned int)
#define ADEC_IO_START        _IO('D', 5)
#define ADEC_IO_STOP         _IO('D', 6)
#define ADEC_IOW_SET_INPUT   _IOW('D', 13, unsigned int)
#define ADEC_IOW_SET_PROC_ID _IOW('D', 28, unsigned int)
#define ADEC_IOW_SET_OUTPUT_MODE              _IOW('D', 11, unsigned int)
#define ADEC_IOW_SET_UI_PARAM                 _IOW('D', 15, struct adec_mpegh_ui_param)
#define ADEC_IOR_GET_UI_PARAM                 _IOR('D', 16, struct adec_mpegh_ui_param)
#define ADEC_IOW_SET_BASETIME                 _IOW('D', 17, unsigned long long)
#define ADEC_IOW_SET_PRESENTATION_GROUP_INDEX _IOW('D', 18, unsigned int)
#define ADEC_IOW_SET_DEFAULT_PRL              _IOW('D', 19, unsigned int)
#define ADEC_IOW_SET_GAP_INFO                 _IOW('D', 20, struct gap_info)
#define ADEC_IOW_SET_DIALOG_ENHANCEMENT_GAIN  _IOW('D', 21, unsigned int)
#define ADEC_IOR_GET_DIALOG_ENHANCEMENT_GAIN  _IOR('D', 22, unsigned int)
#define ADEC_IOR_GET_DIALOG_ENHANCEMENT_LIMIT _IOR('D', 23, unsigned int)
#define ADEC_IOW_SET_DOLBY_DRC_MODE           _IOW('D', 26, unsigned int)
#define ADEC_IOW_SET_AAC_FORMAT_TYPE          _IOW('D', 27, unsigned int)
#define ADEC_IOW_SET_DOWNMIX                  _IOW('D', 30, bool)
#define ADEC_IOW_SET_SOUND_SYSTEM             _IOW('D', 31, unsigned int)
/* ADEC_IOW_SET_CODEC_PARAM (3), defined below with adec_codec_param_t. 24/25
 * (GET_DTS_SUPPORTED / GET_DTS_BITSTREAM_PASSTHROUGH) exist in the GPL kernel header but
 * are never issued by this library. */

/* ioctl parameter blocks: names from the GPL kernel header aud_kapi_decoder.h, layouts
 * (incl. the trailing "downmix" members, absent from the published header) from DWARF. */
struct adec_mpegh_ui_param {
    unsigned int size;
    union { void *param; unsigned int compat_param; unsigned long long sizer; };
};
struct gap_info { unsigned int start_pts, end_pts; };   /* 90 kHz, 28 bit */
typedef struct adec_codec_param {
    unsigned int size;
    union { void *param; unsigned int compat_param; unsigned long long sizer; };
} adec_codec_param_t;
#define ADEC_IOW_SET_CODEC_PARAM _IOW('D', 3, adec_codec_param_t)

typedef struct { unsigned int sampling_rate, channels, bits_per_sample, byte_align, sign; } adec_pcm_param_t;
typedef struct {
    unsigned int sampling_rate, channels, drc_mode, downmix_mode;
    bool audio_description, atmos_decoding, legacy_ddp_out;
    unsigned int play_speed;
    bool downmix;
} adec_ac3_param_t;
typedef struct {
    unsigned int sampling_rate, channels, drc_mode, downmix_mode;
    bool audio_description;
    unsigned int iec_format;
    bool transcoding;
    unsigned int play_speed;
    bool downmix;
} adec_aac_param_t;
typedef struct { unsigned int sampling_rate, channels, drc_mode, audio_description; } adec_mp3_param_t;
typedef struct { unsigned int sampling_rate, channels; bool downmix; } adec_mpegh_param_t;
typedef struct {
    unsigned int sampling_rate, channels, drc_mode, downmix_mode, audio_description;
    bool atmos_decoding;
    unsigned int de_gain;
    bool legacy_ddp_out;
    unsigned int play_speed;
    bool downmix;
} adec_ac4_param_t;
typedef struct {
    unsigned int sampling_rate, channels, bits_per_sample, wma_format_tag, block_align, avg_bps;
    unsigned int channel_mask, encode_opt, adv_encode_opt, adv_encode_opt2;
} adec_wma_param_t;
typedef struct { unsigned int sampling_rate, channels; } adec_ch_param_t; /* aac_eld/dra/vorbis/amr/ra8/flac */
typedef adec_mpegh_param_t adec_opus_param_t;

enum audio_decoder_input { AUDIO_DECODER_INPUT_NONE, AUDIO_DECODER_INPUT_SYSTEM };
enum audio_decoder_codec {
    AUDIO_DECODER_CODEC_UNKNOWN, AUDIO_DECODER_CODEC_PCM, AUDIO_DECODER_CODEC_AC3,
    AUDIO_DECODER_CODEC_EAC3, AUDIO_DECODER_CODEC_AC4, AUDIO_DECODER_CODEC_MAT,
    AUDIO_DECODER_CODEC_TRUE_HD, AUDIO_DECODER_CODEC_AAC, AUDIO_DECODER_CODEC_MP3,
    AUDIO_DECODER_CODEC_MPEG_H, AUDIO_DECODER_CODEC_DRA, AUDIO_DECODER_CODEC_WMA_PRO,
    AUDIO_DECODER_CODEC_VORBIS, AUDIO_DECODER_CODEC_AMR_WB, AUDIO_DECODER_CODEC_AMR_NB,
    AUDIO_DECODER_CODEC_ADPCM, AUDIO_DECODER_CODEC_FLAC, AUDIO_DECODER_CODEC_OPUS,
    AUDIO_DECODER_CODEC_AAC_BYPASS, AUDIO_DECODER_CODEC_AAC_ELD, AUDIO_DECODER_CODEC_RA8,
    AUDIO_DECODER_CODEC_IAMF, AUDIO_DECODER_CODEC_MAX,  /* this library's DWARF, not the GPL enum */
};

/* ---- libaudcommon / libaabase / libaa imports (declared locally) -------------- */
struct aud_object { unsigned int user, type; void (*destroy)(struct aud_object *); };
enum aud_event_type {
    AUD_EVENT_TYPE_ENABLE_DISABLE, AUD_EVENT_TYPE_FIRST_AU, AUD_EVENT_TYPE_LAST_AU,
    AUD_EVENT_TYPE_START, AUD_EVENT_TYPE_STOP, AUD_EVENT_TYPE_MARK, AUD_EVENT_TYPE_DISCONNECT,
    AUD_EVENT_TYPE_FLUSH, AUD_EVENT_TYPE_RESTART, AUD_EVENT_TYPE_LATENCY_CHECK,
    AUD_EVENT_TYPE_DSP_RESET, AUD_EVENT_TYPE_DEC_ERROR, AUD_EVENT_TYPE_MPEGH_UI,
    AUD_EVENT_TYPE_CUSTOM,
};
struct aud_event {
    struct aud_object object;
    enum aud_event_type type;                /* +0x0c */
    unsigned int prop;                       /* +0x10 FLUSH: bit1 = flush done */
};
enum aud_data_type { type_pcm, type_es, type_meta, type_meta_ext, type_mixed };
struct aud_data {
    struct aud_object object;
    struct {
        enum aud_data_type type;             /* +0x10 */
        unsigned int index;
        unsigned int timestamp;              /* +0x18, 90 kHz */
        unsigned long long timestamp64;      /* +0x20, ns */
        unsigned int flag;
    } info;
    void *caller;
    unsigned int size;                       /* +0x34 */
    unsigned char *data;                     /* +0x38 */
};
struct aud_module { char *name; unsigned int uid; };
enum appsink_event_return { appsink_event_handled = 0, appsink_event_not_handled = 1 };
struct module_appsink_callback {
    int (*handle_data)(void *priv, int input_port_num, struct aud_data *data);
    enum appsink_event_return (*handle_event)(void *priv, int port_num, struct aud_event *event);
};

extern struct aimc *aimc_create(void);
extern int aimc_create_handler(struct aimc *imc);
extern void aimc_finalize(struct aimc *imc);
extern unsigned int aimc_get_processor_id(struct aimc *imc);
extern int aimc_handler_sync(struct aimc *imc);
extern unsigned int aimc_module_create(struct aimc *imc, const char *name, int flag);
extern int aimc_module_destroy(struct aimc *imc, unsigned int uid);
extern int aimc_module_connect(struct aimc *imc, unsigned int src, int src_port,
                               unsigned int dst, int dst_port);
extern int aimc_module_disconnect_input(struct aimc *imc, unsigned int uid, int port);
extern int aimc_module_disconnect_output(struct aimc *imc, unsigned int uid, int port);
extern struct registry *ModuleRegistry_Init(struct aimc *imc);
extern void ModuleRegistry_Cleanup(struct registry *reg);
extern struct aud_module *ModuleRegistry_SearchInstByUid(unsigned int uid);
extern struct aud_connector *Connector_Create(unsigned int proc_id, struct aimc *imc);
extern void Connector_Destroy(struct aud_connector *con);
extern int module_appsrc_push_data(struct aud_module *m, struct aud_data *d, bool block);
extern int module_appsrc_push_event(struct aud_module *m, struct aud_event *e);
extern int module_appsink_set_callback(struct aud_module *m, struct module_appsink_callback *cb,
                                       void *priv);
extern struct aud_object *aud_object_get(struct aud_object *o);
extern void aud_object_put(struct aud_object *o);
extern struct aud_data *aud_data_new(unsigned int size);
extern struct aud_event *aud_event_new_first(void);
extern struct aud_event *aud_event_new_last(void);
extern struct aud_event *aud_event_new_flush(void);
extern struct aud_event *aud_event_new_flush_async(void);
extern const char *aud_event_name(struct aud_event *e);
extern char *aud_event_str(struct aud_event *e);
extern int aud_event_value_get_int(struct aud_event *e, const char *field, int def);
extern bool aud_event_value_get_bool(struct aud_event *e, const char *field, bool def);
extern const char *aud_event_value_get_str(struct aud_event *e, const char *field, const char *def);


struct logmm_module logmm_module_lgadec = { 0, 0, 7, &logmm_dummy_mask, "lgadec" };
#define LOGE(line, ...) LOGM(logmm_module_lgadec, log_level_error, line, __VA_ARGS__)
#define LOGN(line, ...) LOGM(logmm_module_lgadec, log_level_noti, line, __VA_ARGS__)
#define LOGI(line, ...) LOGM(logmm_module_lgadec, log_level_info, line, __VA_ARGS__)
#define LOGD(line, ...) LOGM(logmm_module_lgadec, log_level_debug, line, __VA_ARGS__)
#define LOGT(line, ...) LOGM(logmm_module_lgadec, log_level_trace, line, __VA_ARGS__)

#define UNSET_PARAM 0x7fffffff
#define BYPASS_FAKE_BYTES_PER_SEC 352800   /* 0x56220: duration base while bypassing */

/* lgadec codec -> kernel decoder codec (_codec_type, 19 entries). AAC_ELD has no entry. */
static const struct { enum audio_decoder_codec lg_audio_codec; LGADEC_CODEC_T lgadec_codec; }
_codec_type[] = {
    { AUDIO_DECODER_CODEC_UNKNOWN, LGADEC_CODEC_UNKNOWN }, { AUDIO_DECODER_CODEC_PCM, LGADEC_CODEC_PCM },
    { AUDIO_DECODER_CODEC_AC3, LGADEC_CODEC_AC3 },         { AUDIO_DECODER_CODEC_EAC3, LGADEC_CODEC_EAC3 },
    { AUDIO_DECODER_CODEC_AAC, LGADEC_CODEC_AAC },         { AUDIO_DECODER_CODEC_DRA, LGADEC_CODEC_DRA },
    { AUDIO_DECODER_CODEC_MP3, LGADEC_CODEC_MP3 },         { AUDIO_DECODER_CODEC_MPEG_H, LGADEC_CODEC_MPEG_H },
    { AUDIO_DECODER_CODEC_AC4, LGADEC_CODEC_AC4 },         { AUDIO_DECODER_CODEC_WMA_PRO, LGADEC_CODEC_WMA_PRO },
    { AUDIO_DECODER_CODEC_VORBIS, LGADEC_CODEC_VORBIS },   { AUDIO_DECODER_CODEC_AMR_WB, LGADEC_CODEC_AMR_WB },
    { AUDIO_DECODER_CODEC_AMR_NB, LGADEC_CODEC_AMR_NB },   { AUDIO_DECODER_CODEC_ADPCM, LGADEC_CODEC_ADPCM },
    { AUDIO_DECODER_CODEC_FLAC, LGADEC_CODEC_FLAC },       { AUDIO_DECODER_CODEC_OPUS, LGADEC_CODEC_OPUS },
    { AUDIO_DECODER_CODEC_MAT, LGADEC_CODEC_MAT },         { AUDIO_DECODER_CODEC_EAC3, LGADEC_CODEC_EAC3_ATMOS },
    { AUDIO_DECODER_CODEC_IAMF, LGADEC_CODEC_IAMF },
};

const struct { LGADEC_EVENT_MSG_T id; const char *name; } _event_name[] = {
    { LGADEC_EVENT_DSP_RESET, "LGADEC_EVENT_DSP_RESET" },
    { LGADEC_EVENT_DEC_DECODED_INFO, "LGADEC_EVENT_DEC_DECODED_INFO" },
    { LGADEC_EVENT_UPDATE_DECODED_INFO, "LGADEC_EVENT_UPDATE_DECODED_INFO" },
    { LGADEC_EVENT_SEND_MEDIA_INFO, "LGADEC_EVENT_SEND_MEDIA_INFO" },
    { LGADEC_EVENT_PUSH_DATA, "LGADEC_EVENT_PUSH_DATA" },
    { LGADEC_EVENT_EOS, "LGADEC_EVENT_EOS" },
    { LGADEC_EVENT_DEC_MPEGH_UI, "LGADEC_EVENT_DEC_MPEGH_UI" },
    { LGADEC_EVENT_FLUSH_DONE, "LGADEC_EVENT_FLUSH_DONE" },
    { LGADEC_EVENT_PREROLL, "LGADEC_EVENT_PREROLL" },
    { LGADEC_EVENT_DIALOG_ENHANCEMENT_LIMIT_CHANGED, "LGADEC_EVENT_DIALOG_ENHANCEMENT_LIMIT_CHANGED" },
};

const struct { LGADEC_CODEC_T id; const char *name; } _codec_name[] = {
    { LGADEC_CODEC_PCM, "PCM" }, { LGADEC_CODEC_AC3, "AC3" },
    { LGADEC_CODEC_AAC, "AAC" }, { LGADEC_CODEC_AAC_ELD, "AAC_ELD" },
    { LGADEC_CODEC_DRA, "DRA" }, { LGADEC_CODEC_MP3, "MP3" },
    { LGADEC_CODEC_MPEG_H, "MPEG_H" }, { LGADEC_CODEC_AC4, "AC4" },
    { LGADEC_CODEC_WMA_PRO, "WMA_PRO" }, { LGADEC_CODEC_VORBIS, "VORBIS" },
    { LGADEC_CODEC_AMR_WB, "AMR_WB" }, { LGADEC_CODEC_AMR_NB, "AMR_NB" },
    { LGADEC_CODEC_ADPCM, "ADPCM" }, { LGADEC_CODEC_RA8, "RA8" },
    { LGADEC_CODEC_FLAC, "FLAC" }, { LGADEC_CODEC_OPUS, "OPUS" },
    { LGADEC_CODEC_AAC_ENC, "AAC_ENC" }, { LGADEC_CODEC_AC3_ENC, "AC3_ENC" },
    { LGADEC_CODEC_MP3_ENC, "MP3_ENC" }, { LGADEC_CODEC_MAT, "MAT" },
    { LGADEC_CODEC_AAC_BYPASS, "AAC_BYPASS" }, { LGADEC_CODEC_AC3_IEC60958, "AC3_IEC60958" },
    { LGADEC_CODEC_EAC3_ATMOS, "EAC3_ATMOS" }, { LGADEC_CODEC_AC4_ATMOS, "AC4_ATMOS" },
    { LGADEC_CODEC_IAMF, "IAMF" }, { LGADEC_CODEC_UNKNOWN, "UNKNOWN" },
};

static int lgadec_flush_event_sync(void *a);
static int lgadec_flush_event_async(void *a);
static int lgadec_appsink_handle_data(void *priv, int input_port_num, struct aud_data *data);
static enum appsink_event_return lgadec_appsink_handle_event(void *priv, int port_num,
                                                             struct aud_event *event);
struct module_appsink_callback lgadec_appsink_cb = {
    lgadec_appsink_handle_data, lgadec_appsink_handle_event,
};

/* The DSP chain is usable either with an open fd or while the decoder is bypassed. */
#define ADEC_USABLE(adec) ((adec)->bypass_decoder || (adec)->fd >= 0)

/* @0x2891c */
const char *lgadec_get_event_name(LGADEC_EVENT_MSG_T event)
{
    for (unsigned int i = 0; i < G_N_ELEMENTS(_event_name); i++)
        if (_event_name[i].id == event)
            return _event_name[i].name;
    return "LGADEC_EVENT_UNKNOWN";
}

/* inlined everywhere in the original (line 0xd8..0xdb) */
static inline void _lgadec_callback_to_gst(lgadec *adec, LGADEC_EVENT_MSG_T msg, void *data)
{
    if (!adec->callback_fn) {
        LOGE(0xd8, "adec->callback_fn is null");
        return;
    }
    if (!adec->callback_data) {
        LOGE(0xd9, "adec->callback_data is null");
        return;
    }
    LOGD(0xdb, "send event [%s] to gst audio decoder\n", lgadec_get_event_name(msg));
    adec->callback_fn(adec->callback_data, msg, data);
}

/* @0x2895c: g_list_foreach() helper, flushes data queued before the first caps */
static void lgadec_push_pending_data(gpointer data, gpointer user_data)
{
    LGADEC_DATA_T *lgadec_data = data;
    lgadec *adec = user_data;

    if (!lgadec_data || !adec)
        return;
    LOGN(0x10b, "send pending data.. (type %d)\n", lgadec_data->out_type);
    _lgadec_callback_to_gst(adec, LGADEC_EVENT_PUSH_DATA, lgadec_data);
}

/* @0x28a4c: DSP output (appsink port 0 = PCM/meta, bypass_appsink = pass-through ES).
 * Every aud_data becomes an LGADEC_DATA_T. Data is held back until the caps were sent
 * (first event) or 10 buffers piled up. */
static int lgadec_appsink_handle_data(void *priv, int input_port_num, struct aud_data *data)
{
    lgadec *adec = priv;
    LGADEC_DATA_T *lgadec_data;
    LGADEC_DATA_TYPE_T type;

    LOGD(0x117, "port %d : data type %d, size %d TS %llu\n", input_port_num, data->info.type,
         data->size, data->info.timestamp64);
    pthread_mutex_lock(&adec->appsink_mutex);

    lgadec_data = calloc(1, sizeof(*lgadec_data));
    lgadec_data->data = data->data;
    lgadec_data->size = data->size;
    lgadec_data->timestamp64 = data->info.timestamp64;
    if (data->info.type == type_pcm)
        type = LGADEC_DATA_PCM;
    else if (data->info.type == type_es)
        type = LGADEC_DATA_ES;
    else if (data->info.type == type_meta_ext)
        type = LGADEC_DATA_METADATA2;
    else
        type = LGADEC_DATA_METADATA;
    lgadec_data->out_type = type;

    if (data->info.timestamp64 == ~0ull) {
        if (adec->prev_timestamp[type] == ~0ull && adec->segment_start_time != ~0ull)
            adec->prev_timestamp[type] = adec->segment_start_time;
    } else {
        adec->prev_timestamp[type] = data->info.timestamp64;
    }

    if (adec->caps_is_sent) {
        if (g_list_length(adec->list_pending_data)) {
            g_list_foreach(adec->list_pending_data, lgadec_push_pending_data, adec);
            g_list_free(adec->list_pending_data);
            adec->list_pending_data = NULL;
        }
        _lgadec_callback_to_gst(adec, LGADEC_EVENT_PUSH_DATA, lgadec_data);
    } else {
        adec->list_pending_data = g_list_append(adec->list_pending_data, lgadec_data);
        LOGN(0x13f, "data (type %d) before caps.. add to pending list... %d\n",
             lgadec_data->out_type, g_list_length(adec->list_pending_data));
        if (g_list_length(adec->list_pending_data) >= 10) {
            LOGE(0x144, "first event is not coming! send caps.. PCM %d ES %d\n",
                 adec->first_event_done[LGADEC_DATA_PCM], adec->first_event_done[LGADEC_DATA_ES]);
            _lgadec_callback_to_gst(adec, LGADEC_EVENT_UPDATE_DECODED_INFO, &adec->audio_caps);
            _lgadec_callback_to_gst(adec, LGADEC_EVENT_SEND_MEDIA_INFO, NULL);
            adec->caps_is_sent = true;
        }
    }
    LOGD(0x14e, "handle data done.");
    pthread_mutex_unlock(&adec->appsink_mutex);
    return 0;
}

/* @0x28e28 */
const char *lgadec_get_codec_name(LGADEC_CODEC_T codec)
{
    for (unsigned int index = 0; index < G_N_ELEMENTS(_codec_name); index++)
        if (_codec_name[index].id == codec)
            return _codec_name[index].name;
    return "UNKNOWN";
}

/* @0x28e68: appsrc -> ADEC(uid from the driver) -> appsink (port 0) + bypass_appsink (port 1) */
int lgadec_aimc_connect(lgadec *adec)
{
    unsigned int uid_appsrc, uid_appsink, uid_bypass_appsink, uid_dec = 0;

    LOGI(0x21f, "open appsrc");
    if (!adec) {
        LOGE(0x221, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x221, "audio device is not opened");
        return -1;
    }
    uid_appsrc = aimc_module_create(adec->imc, "appsrc", 0);
    if (!uid_appsrc)
        LOGE(0x225, "can't create module '%s'", "appsrc");
    uid_appsink = aimc_module_create(adec->imc, "appsink", 0);
    if (!uid_appsink)
        LOGE(0x22a, "can't create module '%s'", "appsink");
    uid_bypass_appsink = aimc_module_create(adec->imc, "appsink", 0);
    if (!uid_bypass_appsink)
        LOGE(0x22f, "can't create module '%s'", "appsink");
    aimc_handler_sync(adec->imc);

    adec->appsrc = ModuleRegistry_SearchInstByUid(uid_appsrc);
    adec->appsink = ModuleRegistry_SearchInstByUid(uid_appsink);
    adec->bypass_appsink = ModuleRegistry_SearchInstByUid(uid_bypass_appsink);
    if (!adec->appsrc || !adec->appsink) {
        LOGE(0x239, "appsrc %p appksink %p", adec->appsrc, adec->appsink);
        return -1;
    }
    if (!adec->bypass_appsink) {
        LOGE(0x23e, "bypass appksink %p", NULL);
        return -1;
    }
    LOGI(0x242, "appsrc (%s)", adec->appsrc->name);
    LOGI(0x243, "appsink (%s)", adec->appsink->name);
    LOGI(0x244, "bypass_appsink (%s)", adec->bypass_appsink->name);

    ioctl(adec->fd, ADEC_IOR_GET_UID, &uid_dec);
    if (aimc_module_connect(adec->imc, uid_appsrc, 0, uid_dec, 0))
        LOGE(0x24a, "cannot connect module");
    if (aimc_module_connect(adec->imc, uid_dec, 0, uid_appsink, 0))
        LOGE(0x24d, "cannot connect module");
    if (aimc_module_connect(adec->imc, uid_dec, 1, uid_bypass_appsink, 0))
        LOGE(0x250, "cannot connect module");
    module_appsink_set_callback(adec->appsink, &lgadec_appsink_cb, adec);
    module_appsink_set_callback(adec->bypass_appsink, &lgadec_appsink_cb, adec);
    adec->uid_appsrc = uid_appsrc;
    adec->uid_appsink = uid_appsink;
    adec->uid_bypass_appsink = uid_bypass_appsink;
    return 0;
}

/* @0x29230 */
int lgadec_aimc_disconnect(lgadec *adec)
{
    if (!adec) {
        LOGE(0x25e, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x25e, "audio device is not opened");
        return -1;
    }
    if (adec->appsrc) {
        LOGI(0x261, "disconnect appsrc (%s) - dec 0x%08x", adec->appsrc->name, adec->uid_appsrc);
        aimc_module_disconnect_output(adec->imc, adec->uid_appsrc, 0);
        aimc_module_destroy(adec->imc, adec->uid_appsrc);
        adec->appsrc = NULL;
        adec->uid_appsrc = 0;
    }
    if (adec->appsink) {
        LOGI(0x26b, "disconnect appsink (%s) - dec 0x%08x", adec->appsink->name, adec->uid_appsink);
        aimc_module_disconnect_input(adec->imc, adec->uid_appsink, 0);
        aimc_module_destroy(adec->imc, adec->uid_appsink);
        adec->appsink = NULL;
        adec->uid_appsink = 0;
    }
    if (adec->bypass_appsink) {
        LOGI(0x275, "disconnect bypass_appsink (%s) - dec 0x%08x", adec->bypass_appsink->name,
             adec->uid_bypass_appsink);
        aimc_module_disconnect_input(adec->imc, adec->uid_bypass_appsink, 0);
        aimc_module_destroy(adec->imc, adec->uid_bypass_appsink);
        adec->uid_bypass_appsink = 0;
        adec->bypass_appsink = NULL;
    }
    return 0;
}

/* @0x293e0 */
void *lgadec_open(void)
{
    lgadec *adec = calloc(1, sizeof(*adec));

    if (!adec) {
        LOGE(0x382, "memory alloc fail!!");
        return NULL;
    }
    adec->fd = -1;
    adec->index = -1;
    adec->output_mode = 1;
    adec->is_disable_reform = 0;
    adec->app_type = LGADEC_APP_TYPE_UNKNOWN;
    adec->next_state = LGADEC_GST_STATE_NULL;
    adec->selected_codec = NULL;
    adec->codec_data = NULL;
    adec->no_dec = 0;
    adec->pcm_type = LGADEC_PCM_NONE;
    adec->rc_paused = 0;
    adec->callback_fn = NULL;
    adec->callback_data = NULL;
    adec->codec_type = LGADEC_CODEC_UNKNOWN;
    adec->seamless = 0;
    adec->is_dump_enable = false;
    adec->play_speed = 1000;

    if (access("/build/scratch/lgadec_dump_enable", F_OK) == 0) {   /* _lgadec_dump_init, inlined */
        time_t timer;
        struct tm *t;
        char dumpFilename[64];

        adec->is_dump_enable = true;
        timer = time(NULL);
        t = localtime(&timer);
        if (!t) {
            LOGE(0x2c7, "no local time..");
        } else {
            memset(dumpFilename, 0, sizeof(dumpFilename));
            snprintf(dumpFilename, sizeof(dumpFilename),
                     "/build/scratch/lgadec_ES_%04d%02d%02d_%02d%02d%02d.dump", t->tm_year + 1900,
                     t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
            if (adec->dump_fd)
                fclose(adec->dump_fd);
            adec->dump_fd = fopen(dumpFilename, "wb");
            if (!adec->dump_fd)
                LOGE(0x2d7, "FOpen error !! <%s>", dumpFilename);
            LOGN(0x2da, "dump file : %s", dumpFilename);
        }
    }

    adec->presentation_group_index = 0;
    adec->de_gain = UNSET_PARAM;
    adec->default_prl = UNSET_PARAM;
    adec->dolby_drc_mode = UNSET_PARAM;
    adec->sound_system = UNSET_PARAM;
    adec->audio_mode = 5;                           /* LGADEC_MODE_UNKNOWN */
    adec->downmix = false;
    adec->wait_bypass_first_event = false;
    memset(adec->first_event_done, 0, sizeof(adec->first_event_done));
    adec->uid_appsrc = adec->uid_appsink = 0;
    adec->appsrc = adec->appsink = NULL;
    adec->uid_bypass_appsink = 0;
    adec->bypass_appsink = NULL;
    adec->bypass_decoder = false;                   /* es-bypass defaults to FALSE */
    adec->inited = false;
    pthread_mutex_init(&adec->ts_mutex, NULL);
    pthread_mutex_init(&adec->task_mutex, NULL);
    pthread_mutex_init(&adec->rc_mutex, NULL);
    pthread_cond_init(&adec->rc_cond, NULL);
    pthread_mutex_init(&adec->flush_event_mutex, NULL);
    pthread_cond_init(&adec->flush_event_cond, NULL);
    pthread_mutex_init(&adec->flush_mutex, NULL);
    pthread_mutex_init(&adec->appsink_mutex, NULL);
    pthread_mutex_init(&adec->resource_mutex, NULL);
    pthread_cond_init(&adec->resource_cond, NULL);
    LOGI(0x3c3, "open.");
    return adec;
}

/* @0x29668 */
int lgadec_close(void *a)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x3ce, "adec is null");
        return -1;
    }
    if (adec->selected_codec) {
        adec->selected_codec->cleanup(&adec->codec_data);
        adec->selected_codec = NULL;
    }
    if (adec->stream_info.dsi_buffer) {
        free(adec->stream_info.dsi_buffer);
        adec->stream_info.dsi_buffer = NULL;
    }
    if (adec->is_dump_enable) {
        if (adec->dump_fd)
            fclose(adec->dump_fd);
        adec->dump_fd = NULL;
    }
    if (adec->con)
        Connector_Destroy(adec->con);
    if (adec->reg)
        ModuleRegistry_Cleanup(adec->reg);
    if (adec->imc)
        aimc_finalize(adec->imc);
    LOGN(0x3e5, "aimc finalized.");

    pthread_mutex_lock(&adec->appsink_mutex);
    g_list_free_full(adec->list_pending_data, free);
    pthread_mutex_unlock(&adec->appsink_mutex);

    pthread_mutex_destroy(&adec->ts_mutex);
    pthread_mutex_destroy(&adec->task_mutex);
    pthread_mutex_destroy(&adec->rc_mutex);
    pthread_cond_destroy(&adec->rc_cond);
    pthread_mutex_destroy(&adec->flush_event_mutex);
    pthread_cond_destroy(&adec->flush_event_cond);
    pthread_mutex_destroy(&adec->flush_mutex);
    pthread_mutex_destroy(&adec->appsink_mutex);
    pthread_mutex_destroy(&adec->resource_mutex);
    free(adec);
    LOGN(0x3f7, "closed.");
    return 0;
}

/* @0x297dc: per-stream reset, called on every (re)negotiated CAPS. Marks the handle
 * "inited" -- from now on es-bypass changes go through the task pipe. */
int lgadec_start_init(void *a)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x403, "adec is null");
        return -1;
    }
    adec->decoder_state = LGADEC_DECODER_STATE_INIT;
    adec->fed_frame_cnt = 0;
    adec->eos_event_status = LGADEC_EOS_EVENT_NOT_RECEIVED;
    adec->flushing = 0;
    adec->task_status = LGADEC_TASK_NONE;
    adec->get_decoded_info = false;
    adec->audio_caps.freq = 0;
    adec->audio_caps.channels = 0;
    adec->audio_caps.bits_per_sample = 0;
    adec->audio_caps.sign = 0;
    adec->audio_caps.endianness = 0;
    adec->audio_caps.acmod = 0;
    memset(adec->cur_decoded_index, 0, sizeof(adec->cur_decoded_index) + sizeof(adec->decoded_frame_cnt));
    adec->dec_req_feed = 0;
    memset(adec->prev_timestamp, 0xff, sizeof(adec->prev_timestamp));
    memset(adec->buffering_duration, 0, sizeof(adec->buffering_duration));
    adec->presentation_group_index = 0;
    memset(&adec->stream_info, 0, sizeof(adec->stream_info));  /* field-wise in the original */
    adec->feeded_size = 0;
    adec->use_decoder_feed = 0;
    adec->segment_start_time = 0xff;   /* original stores the 64-bit constant 0xff */
    adec->inited = true;
    LOGI(0x43b, "init.");
    return 0;
}

static inline void _lgadec_aimc_init(lgadec *adec)
{
    struct aimc *imc;
    struct registry *reg;
    struct aud_connector *con;

    imc = aimc_create();
    if (!imc) {
        LOGE(0xeb, "can't create aimc");
        return;
    }
    aimc_create_handler(imc);
    reg = ModuleRegistry_Init(imc);
    if (!reg) {
        LOGE(0xf2, "no registry");
        goto fail;
    }
    con = Connector_Create(aimc_get_processor_id(imc), imc);
    if (!con) {
        LOGE(0xf9, "no connector");
        ModuleRegistry_Cleanup(reg);
        goto fail;
    }
    adec->con = con;
    adec->imc = imc;
    adec->reg = reg;
    return;

fail:
    aimc_finalize(imc);
}

/* @0x298ec: open /dev/lg/adec0, bind it to the ADEC index, create aimc on first use */
int lgadec_decoder_open(void *a)
{
    lgadec *adec = a;
    unsigned int proc_id;
    int ret;

    if (!adec) {
        LOGE(0x447, "adec is null");
        return -1;
    }
    adec->fd = open("/dev/lg/adec0", O_RDWR | O_CLOEXEC);
    if (adec->fd < 0) {
        LOGE(0x44b, "can't open  device (%s)\n", "/dev/lg/adec0");
        goto fail;
    }
    ret = ioctl(adec->fd, ADEC_IOW_SET_INDEX, &adec->index);
    if (ret < 0) {
        LOGE(0x453, "audio decoder [%d] is not available", adec->index);
        goto fail;
    }
    adec->decoder_state = LGADEC_DECODER_STATE_OPEN;
    adec->feeded_size = 0;

    if (!adec->imc)
        _lgadec_aimc_init(adec);
    if (adec->imc) {
        proc_id = aimc_get_processor_id(adec->imc);
        ioctl(adec->fd, ADEC_IOW_SET_PROC_ID, &proc_id);
    }
    LOGI(0x463, "Decoder state is LGADEC_DECODER_STATE_OPEN");
    return ret;

fail:
    if (adec->fd >= 0) {
        close(adec->fd);
        adec->fd = -1;
    }
    return -1;
}

/* @0x29b20: no-op while the decoder is bypassed */
int lgadec_decoder_close(void *a)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x475, "adec is null");
        return -1;
    }
    if (adec->bypass_decoder) {
        LOGI(0x478, "bypass decoder is on\n");
        return 0;
    }
    if (adec->fd < 0) {
        LOGE(0x475, "audio device is not opened");
        return -1;
    }
    close(adec->fd);
    adec->decoder_state = LGADEC_DECODER_STATE_INIT;
    adec->fd = -1;
    LOGI(0x483, "Decoder state is LGADEC_DECODER_STATE_INIT");
    return 0;
}

/* @0x29c20 */
int lgadec_decoder_start(void *a)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x48d, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x48d, "audio device is not opened");
        return -1;
    }
    ioctl(adec->fd, ADEC_IO_START, &adec->index);
    adec->decoder_state = LGADEC_DECODER_STATE_START;
    LOGI(0x493, "Decoder state is LGADEC_DECODER_STATE_START");
    return 0;
}

/* @0x29cf8 */
int lgadec_decoder_stop(void *a)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x49e, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x49e, "audio device is not opened");
        return -1;
    }
    if (!adec->bypass_decoder)
        ioctl(adec->fd, ADEC_IO_STOP, &adec->index);
    adec->decoder_state = LGADEC_DECODER_STATE_STOP;
    LOGI(0x4a6, "Decoder state is LGADEC_DECODER_STATE_STOP");
    return 0;
}

/* @0x29dd0: the fork between DSP decoding and ES bypass.
 *  - normal: copy into an aud_data (90 kHz + ns timestamp), push into the aimc appsrc;
 *    output comes back asynchronously through lgadec_appsink_handle_data.
 *  - bypass_decoder ("ES BYPASS & DROP"): nothing reaches the DSP; the input itself is
 *    returned synchronously to the element as LGADEC_DATA_BYPASS (pointer not copied). */
int lgadec_decoder_feed(void *a, void *data, unsigned int length, unsigned long long timestamp)
{
    lgadec *adec = a;
    struct aud_data *buffer;

    if (!adec) {
        LOGE(0x4b6, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x4b6, "audio device is not opened");
        return -1;
    }

    pthread_mutex_lock(&adec->resource_mutex);
    if (adec->bypass_decoder) {
        LGADEC_DATA_T *lgadec_data;

        LOGD(0x4bc, "bypass decoder... %d\n", adec->use_decoder_feed);
        pthread_mutex_unlock(&adec->resource_mutex);
        lgadec_data = calloc(1, sizeof(*lgadec_data));
        lgadec_data->out_type = LGADEC_DATA_BYPASS;
        lgadec_data->size = length;
        lgadec_data->data = data;
        lgadec_data->timestamp64 = timestamp;
        if (timestamp != ~0ull)
            adec->prev_timestamp[LGADEC_DATA_BYPASS] = timestamp;
        _lgadec_callback_to_gst(adec, LGADEC_EVENT_PUSH_DATA, lgadec_data);
        adec->feeded_size += length;
        return 0;
    }

    adec->use_decoder_feed++;
    LOGD(0x4cf, "use_decoder_feed... %d\n", adec->use_decoder_feed);
    pthread_mutex_unlock(&adec->resource_mutex);
    LOGD(0x4d2, "PTS=%13llu ns, length %u", timestamp, length);
    if (adec->feeded_size == 0) {
        LOGI(0x4d5, "feeded_size is Zero. First data feed");
        module_appsrc_push_event(adec->appsrc, aud_event_new_first());
    }
    buffer = aud_data_new(length);
    if (!buffer) {
        LOGE(0x4db, "can't create aud_data");
        adec->use_decoder_feed--;
        return 0;
    }
    buffer->info.timestamp = timestamp == ~0ull ? ~0u : (unsigned int)(timestamp / 10 * 9 / 10000);
    buffer->info.timestamp64 = timestamp;
    LOGD(0x4e7, "ts : %llu -> %d", timestamp, buffer->info.timestamp);
    memcpy(buffer->data, data, length);
    buffer->info.type = type_es;
    module_appsrc_push_data(adec->appsrc, (struct aud_data *)aud_object_get(&buffer->object), true);
    aud_object_put(&buffer->object);

    LOGD(0x4f0, "before resource_mutex %d\n", adec->use_decoder_feed);
    pthread_mutex_lock(&adec->resource_mutex);
    adec->use_decoder_feed--;
    LOGD(0x4f3, "after resource_mutex %d\n", adec->use_decoder_feed);
    pthread_cond_signal(&adec->resource_cond);
    pthread_mutex_unlock(&adec->resource_mutex);
    if (adec->is_dump_enable && adec->dump_fd)
        fwrite(data, 1, length, adec->dump_fd);
    adec->feeded_size += length;
    return 0;
}

/* @0x2a214 */
int lgadec_decoder_connect(void *a)
{
    lgadec *adec = a;
    enum audio_decoder_input input = AUDIO_DECODER_INPUT_SYSTEM;

    if (!adec) {
        LOGE(0x505, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x505, "audio device is not opened");
        return -1;
    }
    if (lgadec_aimc_connect(adec) != 0) {
        LOGE(0x508, "error in aimc_connect");
        return 1;
    }
    ioctl(adec->fd, ADEC_IOW_SET_INPUT, &input);
    module_appsrc_push_event(adec->appsrc, aud_event_new_first());
    adec->decoder_state = LGADEC_DECODER_STATE_CONNECT;
    LOGI(0x512, "Decoder state is LGADEC_DECODER_STATE_CONNECT");
    return 0;
}

/* @0x2a344 */
int lgadec_decoder_disconnect(void *a)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x51c, "adec is null");
        return -1;
    }
    if (!adec->bypass_decoder) {
        if (adec->fd < 0) {
            LOGE(0x51c, "audio device is not opened");
            return -1;
        }
        lgadec_aimc_disconnect(adec);
    }
    adec->decoder_state = LGADEC_DECODER_STATE_CONFIG;
    LOGI(0x525, "Decoder state is LGADEC_DECODER_STATE_CONFIG - DisConnect");
    return 0;
}

#define PLAY_SPEED_VALID(s) ((unsigned int)((s) - 250) <= 1750)  /* 0.25x .. 2x */
#define ALLOC_PARAM(p, line) do { \
        (p) = malloc(sizeof(*(p))); \
        if (!(p)) { LOGE(line, "malloc error\n"); return -1; } \
    } while (0)

/* @0x2a414: ADEC_IOW_SET_CODEC_PARAM. Ghidra could not decompile this one; recovered from
 * the disassembly (tbh jump table over audio_decoder_codec - 1 at 0x1a488). Parameters come
 * from adec->stream_info (filled by the element from caps) plus the stored DRC / DE gain /
 * play speed / downmix settings. MAT, TRUE_HD, AAC_BYPASS and IAMF get no parameter block.
 * No DTS members in this library's ioctl enum; parameter defaults do not prove support. */
int lgadec_decoder_set_codec_param(void *a)
{
    lgadec *adec = a;
    LGADEC_STREAM_INFO_T *si;
    adec_codec_param_t param = { 0 };
    enum audio_decoder_codec codec = AUDIO_DECODER_CODEC_UNKNOWN;

    if (!adec) {
        LOGE(0x531, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x531, "audio device is not opened");
        return -1;
    }
    si = &adec->stream_info;
    for (unsigned int i = 0; i < G_N_ELEMENTS(_codec_type); i++) {
        if (adec->codec_type == _codec_type[i].lgadec_codec) {
            codec = _codec_type[i].lg_audio_codec;
            break;
        }
    }
    LOGI(0x534, "set_codec_param. codec %d\n", codec);
    param.param = NULL;

    switch (codec) {
    case AUDIO_DECODER_CODEC_PCM:
    case AUDIO_DECODER_CODEC_ADPCM: {
        adec_pcm_param_t *p;

        ALLOC_PARAM(p, 0x540);
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        p->bits_per_sample = si->bits_per_sample;
        p->byte_align = si->byte_align;
        p->sign = si->sign;
        LOGI(0x54c, "sampling rate: %d, channels: %d, bits per sample: %d, byte align: %d "
             "(little endian:0, big endian:1), sign: %d (signed:0, unsigned:1)\n",
             p->sampling_rate, p->channels, p->bits_per_sample, p->byte_align, p->sign);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_AC3:
    case AUDIO_DECODER_CODEC_EAC3: {
        adec_ac3_param_t *p;

        ALLOC_PARAM(p, 0x55b);
        memset(&p->drc_mode, 0, sizeof(*p) - offsetof(adec_ac3_param_t, drc_mode));
        if (adec->dolby_drc_mode != UNSET_PARAM)
            p->drc_mode = adec->dolby_drc_mode;
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        p->downmix_mode = 1;
        if (PLAY_SPEED_VALID(adec->play_speed))
            p->play_speed = adec->play_speed;
        p->downmix = adec->downmix;
        p->atmos_decoding = true;
        LOGI(0x570, "sampling rate: %d, channels: %d, drc mode: %d, downmix mode: %d, "
             "play speed: %d, atmos decoding: %d\n", p->sampling_rate, p->channels,
             p->drc_mode, p->downmix_mode, p->play_speed, p->atmos_decoding);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_AAC: {
        adec_aac_param_t *p;

        ALLOC_PARAM(p, 0x57e);
        memset(&p->drc_mode, 0, sizeof(*p) - offsetof(adec_aac_param_t, drc_mode));
        if (adec->dolby_drc_mode != UNSET_PARAM)
            p->drc_mode = adec->dolby_drc_mode;
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        if (PLAY_SPEED_VALID(adec->play_speed))
            p->play_speed = adec->play_speed;
        /* transcoding off only at normal speed (1000) */
        if (adec->play_speed != 1000)
            p->transcoding = true;
        p->iec_format = 1;
        p->downmix = adec->downmix;
        LOGI(0x596, "sampling rate: %d, channels: %d, drc mode: %d, downmix mode: %d, "
             "play speed: %d, transcoding: %d, iec_format: %d\n", p->sampling_rate,
             p->channels, p->drc_mode, 0, p->play_speed, p->transcoding, 1);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_AAC_ELD: {
        adec_ch_param_t *p;

        ALLOC_PARAM(p, 0x5a6);
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        LOGI(0x5ae, "sampling rate: %d, channels: %d\n", p->sampling_rate, p->channels);
        param.size = sizeof(adec_aac_param_t);   /* sic: 0x24 for an 8-byte block */
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_DRA: {
        adec_ch_param_t *p;

        ALLOC_PARAM(p, 0x5bd);
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        LOGI(0x5c5, "sampling rate: %d, channels: %d\n", p->sampling_rate, p->channels);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_VORBIS: {
        adec_ch_param_t *p;

        ALLOC_PARAM(p, 0x674);
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        LOGI(0x67c, "sampling rate: %d, channels: %d\n", p->sampling_rate, p->channels);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_AMR_WB:
    case AUDIO_DECODER_CODEC_AMR_NB: {
        adec_ch_param_t *p;

        ALLOC_PARAM(p, 0x68c);
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        LOGI(0x694, "sampling rate: %d, channels: %d\n", p->sampling_rate, p->channels);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_RA8: {
        adec_ch_param_t *p;

        ALLOC_PARAM(p, 0x6a3);
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        LOGI(0x6ab, "sampling rate: %d, channels: %d\n", p->sampling_rate, p->channels);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_FLAC: {
        adec_ch_param_t *p;

        ALLOC_PARAM(p, 0x6bb);
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        LOGI(0x6c4, "sampling rate: %d, channels: %d\n", p->sampling_rate, p->channels);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_MP3: {
        adec_mp3_param_t *p;

        ALLOC_PARAM(p, 0x5d4);
        p->drc_mode = 0;
        p->audio_description = 0;
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        LOGI(0x5dc, "sampling rate: %d, channels: %d\n", p->sampling_rate, p->channels);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_MPEG_H: {
        adec_mpegh_param_t *p;

        ALLOC_PARAM(p, 0x5ec);
        memset(p, 0, sizeof(*p));
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        p->downmix = adec->downmix;
        LOGI(0x5f5, "sampling rate: %d, channels: %d\n", p->sampling_rate, p->channels);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_OPUS: {
        adec_mpegh_param_t *p;

        ALLOC_PARAM(p, 0x6d4);
        memset(p, 0, sizeof(*p));
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        p->downmix = adec->downmix;
        LOGI(0x6de, "sampling rate: %d, channels: %d\n", p->sampling_rate, p->channels);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_AC4: {
        adec_ac4_param_t *p;

        ALLOC_PARAM(p, 0x604);
        memset(&p->drc_mode, 0, sizeof(*p) - offsetof(adec_ac4_param_t, drc_mode));
        p->downmix_mode = 1;
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        if (PLAY_SPEED_VALID(adec->play_speed))
            p->play_speed = adec->play_speed;
        p->atmos_decoding = true;
        if (adec->de_gain != UNSET_PARAM)
            p->de_gain = adec->de_gain;
        p->downmix = adec->downmix;
        LOGI(0x61a, "sampling rate: %d, channels: %d, drc mode: %d, downmix mode: %d, "
             "play speed: %d, atmos decoding: %d, dialog enhancement gain: %d\n",
             p->sampling_rate, p->channels, 0, 1, p->play_speed, 1, p->de_gain);
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    case AUDIO_DECODER_CODEC_WMA_PRO: {
        adec_wma_param_t *p;
        const unsigned char *dsi;

        ALLOC_PARAM(p, 0x628);
        memset(p, 0, sizeof(*p));
        p->sampling_rate = si->freq;
        p->channels = si->channels;
        p->bits_per_sample = si->bits_per_sample;
        p->wma_format_tag = si->wma_format_tag;
        p->block_align = si->block_align;
        p->avg_bps = si->avg_bytes_per_sec;
        LOGI(0x635, "sampling rate: %d, channels: %d, bits per sample: %d, wma format tag: %d, "
             "block align: %d, avg bytes per sec: %d\n", p->sampling_rate, p->channels,
             p->bits_per_sample, p->wma_format_tag, p->block_align, p->avg_bps);
        /* WAVEFORMATEX extra data (DSI) */
        dsi = si->dsi_buffer;
        if (si->wma_format_tag == 0x162 || si->wma_format_tag == 0x163) {   /* Pro / Lossless */
            if (si->dsi_buffer_length != 18) {
                LOGE(0x63e, "DSI length(%d) is less than WMA3 info\n", si->dsi_buffer_length);
                free(p);
                return -1;
            }
            memcpy(&p->channel_mask, dsi + 2, 4);
            memcpy(&p->adv_encode_opt2, dsi + 6, 4);
            p->encode_opt = *(const unsigned short *)(dsi + 14);
            p->adv_encode_opt = *(const unsigned short *)(dsi + 16);
            LOGI(0x648, "channel mask: %d, encode opt: %d, adv encode opt: %d, adv encode opt2: %d\n",
                 p->channel_mask, p->encode_opt, p->adv_encode_opt, p->adv_encode_opt2);
        } else if (si->wma_format_tag == 0x161) {                           /* WMA2 */
            if (si->dsi_buffer_length != 10) {
                LOGE(0x64c, "DSI length(%d) is less than WMA2 info\n", si->dsi_buffer_length);
                free(p);
                return -1;
            }
            p->encode_opt = *(const unsigned short *)(dsi + 4) & 0x3f;
            LOGI(0x656, "encode opt: %d\n", p->encode_opt);
        } else {                                                           /* WMA1 */
            if (si->dsi_buffer_length != 4) {
                LOGE(0x65a, "DSI length(%d) is less than WMA1 info\n", si->dsi_buffer_length);
                free(p);
                return -1;
            }
            p->encode_opt = *(const unsigned short *)(dsi + 2);
            LOGI(0x661, "encode opt: %d\n", p->encode_opt);
        }
        param.size = sizeof(*p);
        param.param = p;
        break;
    }
    default:   /* UNKNOWN, MAT, TRUE_HD, AAC_BYPASS, IAMF */
        break;
    }

    ioctl(adec->fd, ADEC_IOW_SET_CODEC_PARAM, &param);
    if (param.param)
        free(param.param);
    return 0;
}

/* @0x2af94 */
int lgadec_decoder_configure_decoder(void *a)
{
    lgadec *adec = a;
    enum audio_decoder_codec codec = AUDIO_DECODER_CODEC_UNKNOWN;
    unsigned int mode;

    if (!adec) {
        LOGE(0x718, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x718, "audio device is not opened");
        return -1;
    }
    for (unsigned int i = 0; i < G_N_ELEMENTS(_codec_type); i++) {
        if (adec->codec_type == _codec_type[i].lgadec_codec) {
            codec = _codec_type[i].lg_audio_codec;
            break;
        }
    }
    ioctl(adec->fd, ADEC_IOW_SET_CODEC, &codec);
    lgadec_decoder_set_codec_param(adec);
    mode = adec->output_mode + 4;
    if (lgadec_decoder_set_audio_output_mode(adec, mode) == 0)
        LOGD(0x725, "Set Audio(Dual-mono) Output Mode (4: LR_FORCED, 5: LL_FORCED, 6: RR_FORCED): %u",
             mode);
    else
        LOGE(0x723, "Set Audio(Dual-mono) Mode Err");
    adec->decoder_state = LGADEC_DECODER_STATE_CONFIG;
    LOGI(0x72c, "Decoder state is LGADEC_DECODER_STATE_CONFIG (Decoder)");
    return 0;
}

/* ---- /dev/lg/adec ioctl wrappers (0x2aeb8..0x2ca8c) ------------------------- */

/* @0x2aeb8 */
int lgadec_decoder_set_audio_output_mode(void *a, unsigned int mode)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x737, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x737, "audio device is not opened");
        return -1;
    }
    ioctl(adec->fd, ADEC_IOW_SET_OUTPUT_MODE, &mode);
    LOGI(0x73b, "output mode: %d", mode);
    return 0;
}

/* @0x2b2ec */
int lgadec_decoder_set_basetime(void *a, unsigned long long start)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x774, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x774, "audio device is not opened");
        return -1;
    }
    ioctl(adec->fd, ADEC_IOW_SET_BASETIME, &start);
    LOGI(0x778, "basetime: %llu", start);
    return 0;
}

/* @0x2b638 */
int lgadec_decoder_set_ac4_dialog_enhancement_gain(void *a, unsigned int gain)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x7a4, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x7a4, "audio device is not opened");
        return -1;
    }
    ioctl(adec->fd, ADEC_IOW_SET_DIALOG_ENHANCEMENT_GAIN, &gain);
    LOGI(0x7a8, "dialog enhancement gain : %u", gain);
    return 0;
}

/* @0x2c5a8 */
int lgadec_decoder_set_aac_format_type(void *a, unsigned int format_type)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x7d5, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x7d5, "audio device is not opened");
        return -1;
    }
    ioctl(adec->fd, ADEC_IOW_SET_AAC_FORMAT_TYPE, &format_type);
    LOGI(0x7d9, "format type : %u", format_type);
    return 0;
}

/* @0x2c684 */
int lgadec_decoder_get_ac4_dialog_enhancement_gain(void *a, unsigned int *gain)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x7e3, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x7e3, "audio device is not opened");
        return -1;
    }
    ioctl(adec->fd, ADEC_IOR_GET_DIALOG_ENHANCEMENT_GAIN, gain);
    LOGI(0x7e7, "ac4 dialog enhancement gain : %d", *gain);
    return 0;
}

/* @0x2c760 */
int lgadec_decoder_get_ac4_dialog_enhancement_limit(void *a, unsigned int *limit)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x7f1, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x7f1, "audio device is not opened");
        return -1;
    }
    ioctl(adec->fd, ADEC_IOR_GET_DIALOG_ENHANCEMENT_LIMIT, limit);
    LOGI(0x7f5, "ac4 dialog enhancement limit (min: 0, max: %d (dB)", *limit);
    return 0;
}

/* @0x2c83c */
int lgadec_decoder_set_dolby_drc_mode(void *a, unsigned int drc_mode)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x7ff, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x7ff, "audio device is not opened");
        return -1;
    }
    ioctl(adec->fd, ADEC_IOW_SET_DOLBY_DRC_MODE, &drc_mode);
    LOGI(0x803, "dolby drc mode %d (LINE: 0, RF: 1)", drc_mode);
    return 0;
}

/* @0x2c3f0: no ioctl while the decoder is bypassed (replayed by lgadec_set_bypass_decoder) */
int lgadec_decoder_set_presentation_group_index(void *a, unsigned int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x7b3, "adec is null");
        return -1;
    }
    if (!adec->bypass_decoder) {
        if (adec->fd < 0) {
            LOGE(0x7b3, "audio device is not opened");
            return -1;
        }
        ioctl(adec->fd, ADEC_IOW_SET_PRESENTATION_GROUP_INDEX, &value);
    }
    LOGI(0x7b9, "presentation group index : %u", value);
    return 0;
}

/* @0x2c4cc: no ioctl while the decoder is bypassed (replayed by lgadec_set_bypass_decoder) */
int lgadec_decoder_set_default_prl(void *a, unsigned int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x7c4, "adec is null");
        return -1;
    }
    if (!adec->bypass_decoder) {
        if (adec->fd < 0) {
            LOGE(0x7c4, "audio device is not opened");
            return -1;
        }
        ioctl(adec->fd, ADEC_IOW_SET_DEFAULT_PRL, &value);
    }
    LOGI(0x7ca, "default prl : %u", value);
    return 0;
}

/* @0x2b11c: MPEG-H UI command (XML) */
int lgadec_decoder_set_ui_param(void *a, const char *ui_param, unsigned int length)
{
    lgadec *adec = a;
    struct adec_mpegh_ui_param param;

    if (!adec) {
        LOGE(0x746, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x746, "audio device is not opened");
        return -1;
    }
    param.size = length;
    param.param = (void *)ui_param;
    ioctl(adec->fd, ADEC_IOW_SET_UI_PARAM, &param);
    return 0;
}

#define MPEGH_UI_MAX 0x3000

/* @0x2b1b4: MPEG-H UI state (XML); *data is malloc'ed, NUL-terminated */
int lgadec_decoder_get_ui_param(void *a, char **data)
{
    lgadec *adec = a;
    struct adec_mpegh_ui_param param;

    if (!adec) {
        LOGE(0x758, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x758, "audio device is not opened");
        return -1;
    }
    param.param = malloc(MPEGH_UI_MAX);
    param.size = MPEGH_UI_MAX;
    if (!param.param) {
        LOGE(0x75d, "malloc fail. (%d)", MPEGH_UI_MAX);
        return -1;
    }
    ioctl(adec->fd, ADEC_IOR_GET_UI_PARAM, &param);
    if (param.size > MPEGH_UI_MAX) {
        free(param.param);
        LOGE(0x765, "size (%d) is exceeding the %d limit", param.size, MPEGH_UI_MAX);
        return -1;
    }
    ((char *)param.param)[param.size] = '\0';
    *data = param.param;
    return 0;
}

/* @0x2b3d0: stub in this build */
int lgadec_decoder_get_cpb_status(void *a, unsigned long long *pBufferingSize,
                                  unsigned int *pUsedAuiCount, unsigned int *pBufferSize,
                                  unsigned int *pBufferFreeSize)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x784, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x784, "audio device is not opened");
        return -1;
    }
    LOGI(0x786, "cpb status: do nothing, return ok");
    return 0;
}

/* @0x2b498: ns -> 90 kHz, 28-bit (PES PTS wrap is not modelled) */
int lgadec_decoder_set_gap_info(void *a, unsigned long long start, unsigned long long end)
{
    lgadec *adec = a;
    struct gap_info param = { 0, 0 };

    if (!adec) {
        LOGE(0x792, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x792, "audio device is not opened");
        return -1;
    }
    param.start_pts = (start / 10 * 9 / 10000) & 0xfffffff;
    param.end_pts = (end / 10 * 9 / 10000) & 0xfffffff;
    ioctl(adec->fd, ADEC_IOW_SET_GAP_INFO, &param);
    LOGI(0x799, "audio gap. start %d, end %d", param.start_pts, param.end_pts);
    return 0;
}

/* @0x2ca8c: no fd check (like lgadec_set_sound_system) */
int lgadec_decoder_set_downmix(void *a, bool enable)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x82c, "adec is null");
        return -1;
    }
    ioctl(adec->fd, ADEC_IOW_SET_DOWNMIX, &enable);
    LOGN(0x830, "downmix %s", enable ? "enable" : "disable");
    return 0;
}

extern struct aud_event *aud_event_newv_custom(const char *name, int n, ...);

/* @0x2c918: custom "serialize" aimc event (field "sync") down the DSP chain; only for DSP
 * codecs, and refused while the decoder is bypassed */
int lgadec_decoder_send_serialized_event(void *a)
{
    lgadec *adec = a;
    int ret;

    if (!adec) {
        LOGE(0x80e, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x80e, "audio device is not opened");
        return -1;
    }
    if (!adec->selected_codec || adec->no_dec == 1)
        return -1;
    pthread_mutex_lock(&adec->resource_mutex);
    if (adec->bypass_decoder) {
        LOGN(0x816, "not send serialized event\n");
        ret = -1;
    } else {
        LOGN(0x81a, "send lgadec_decoder_send_serialized_event\n");
        module_appsrc_push_event(adec->appsrc, aud_event_newv_custom("serialize", 2, "sync", 0));
        LOGN(0x81f, "send lgadec_decoder_send_serialized_event done\n");
        ret = 0;
    }
    pthread_mutex_unlock(&adec->resource_mutex);
    return ret;
}

/* @0x2cf28: lip-sync trace of one input PTS. feeded_size == 0 resets the counter. P(C) is
 * the PTS scaled by 1000/play_speed. Returns 1 for an invalid timestamp. */
int lgadec_log_ts(void *a, unsigned long long timestamp, unsigned int len)
{
    static unsigned int index;
    lgadec *adec = a;
    unsigned long long pts90k, scaled;

    if (!adec) {
        LOGE(0x8ca, "adec is null");
        return -1;
    }
    if (adec->feeded_size == 0) {
        index = 0;
        LOGD(0x8cf, "(TEMP)PTS.base = %llu, speed = %6.3f", 0ull,
             (double)((float)adec->play_speed / 1000.0f));
    }
    if (timestamp == ~0ull) {
        LOGD(0x8d6, "Lip(DEC.%02d-%04u): P=Invalid_TS", adec->index, index++);
        return 1;
    }
    pts90k = timestamp * 9 / 100000;
    scaled = pts90k;
    if (adec->play_speed != 1000 && adec->play_speed != 0)
        scaled = (unsigned long long)((float)pts90k * 1000.0f / (float)adec->play_speed);
    LOGD(0x8e6, "Lip(DEC.%02d-%04u): P=%13llu ns( %12llu ), P(C)=%12lld, len=%5d", adec->index,
         index++, timestamp, pts90k, scaled, len);
    return 0;
}

static inline int _lgadec_set_audio_mode(lgadec *adec, int channel_mode)
{
    int audio_mode = 5;
    int channel_mode_dolby[8] = { 3, 0, 2, 4, 4, 4, 4, 4 };
    int channel_mode_mp3[4] = { 2, 1, 3, 0 };

    if (channel_mode >= 0) {
        if (adec->codec_type == LGADEC_CODEC_MP3)
            audio_mode = channel_mode_mp3[channel_mode];
        else if ((adec->codec_type >= LGADEC_CODEC_AC3 && adec->codec_type <= LGADEC_CODEC_AAC) ||
                 adec->codec_type == LGADEC_CODEC_AC4)
            audio_mode = channel_mode_dolby[channel_mode];
    }
    return audio_mode;
}

static inline int _lgadec_process_decoded_info(lgadec *adec, LGADEC_AUDIO_CAPS_INFO *caps_info,
                                              bool es)
{
    if (!ADEC_USABLE(adec)) {
        LOGE(0x934, "audio device is not opened");
        return -1;
    }
    LOGN(0x938, "got info --> fr: %d ch:%d bps:%d acmod:%d atmos:%d channel_mode:%d "
         "downmix:%d out_ch:%d es:%d codec_str:%s\n", caps_info->freq, caps_info->channels,
         caps_info->bits_per_sample, caps_info->acmod, caps_info->atmos, caps_info->channel_mode,
         caps_info->downmix, caps_info->out_channels, caps_info->es, caps_info->codec_str);
    if (!es) {
        LGADEC_AUDIO_CAPS_INFO *cur = &adec->audio_caps;

        if (cur->freq == 0 || cur->freq != caps_info->freq || cur->channels != caps_info->channels ||
            cur->bits_per_sample != caps_info->bits_per_sample || cur->acmod != caps_info->acmod ||
            cur->dolby != caps_info->dolby || cur->atmos != caps_info->atmos ||
            cur->channel_mode != caps_info->channel_mode || cur->lfe_enable != caps_info->lfe_enable ||
            cur->downmix != caps_info->downmix || cur->out_channels != caps_info->out_channels) {
            LOGN(0x94e, "caps is updated\n");
            adec->caps_is_updated = true;
        }
        cur->freq = caps_info->freq;
        cur->channels = caps_info->channels;
        cur->acmod = caps_info->acmod;
        cur->lfe_enable = caps_info->lfe_enable;
        if (adec->codec_type == LGADEC_CODEC_PCM) {
            caps_info->endianness = adec->stream_info.byte_align;
            caps_info->sign = adec->stream_info.sign;
        } else {
            caps_info->sign = 1;
            caps_info->endianness = 0;
        }
        cur->bits_per_sample = caps_info->bits_per_sample;
        cur->sign = caps_info->sign;
        cur->out_channels = caps_info->out_channels;
        cur->check_downmix = caps_info->check_downmix;
        cur->endianness = caps_info->endianness;
        cur->dolby = caps_info->dolby;
        cur->atmos = caps_info->atmos;
        cur->gap_correction = caps_info->gap_correction;
        if (adec->audio_mode == 5 || cur->channel_mode != caps_info->channel_mode) {
            adec->audio_mode = _lgadec_set_audio_mode(adec, caps_info->channel_mode);
            cur->channel_mode = caps_info->channel_mode;
            adec->audio_mode_changed = true;
        }
        cur->downmix = caps_info->downmix;
    } else {
        adec->audio_caps.data_channels = caps_info->data_channels;
        adec->audio_caps.data_rate = caps_info->data_rate;
        adec->audio_caps.data_bps = caps_info->data_bps;
        adec->audio_caps.es = caps_info->es;
        memcpy(adec->audio_caps.codec_str, caps_info->codec_str, sizeof(caps_info->codec_str));
    }
    if (adec->wait_bypass_first_event) {
        if (!adec->first_event_done[LGADEC_DATA_PCM] || !adec->first_event_done[LGADEC_DATA_ES])
            return 0;
    } else if (!adec->first_event_done[LGADEC_DATA_PCM]) {
        return 0;
    }
    if (adec->caps_is_updated) {
        LOGN(0x982, "update decoded info");
        _lgadec_callback_to_gst(adec, LGADEC_EVENT_UPDATE_DECODED_INFO, &adec->audio_caps);
        _lgadec_callback_to_gst(adec, LGADEC_EVENT_SEND_MEDIA_INFO, NULL);
    } else {
        LOGN(0x987, "do not update decoded info");
    }
    memset(adec->first_event_done, 0, sizeof(adec->first_event_done));
    adec->caps_is_updated = false;
    adec->caps_is_sent = true;
    return 0;
}

/* @0x2b714: aimc events from both appsinks. FIRST_AU carries the decoded format; on the
 * ES (bypass_appsink) port it has "es"=TRUE and describes the IEC stream (data-*). */
static enum appsink_event_return lgadec_appsink_handle_event(void *priv, int port_num,
                                                             struct aud_event *event)
{
    lgadec *adec = priv;
    char *str;

    pthread_mutex_lock(&adec->appsink_mutex);
    str = aud_event_str(event);
    if (str) {
        LOGN(0x15e, "Event : %s", str);
        free(str);
    } else {
        LOGN(0x161, "Event : %s", aud_event_name(event));
    }

    switch (event->type) {
    case AUD_EVENT_TYPE_FIRST_AU: {
        LGADEC_AUDIO_CAPS_INFO caps_info;
        int channels, bits_per_sample, rate, acmod, check_downmix, channel_mode, out_channels;
        int data_rate, gap_correction;
        bool atmos, lfe_enable, dolby, clock_x4, downmix, es;
        const char *codec_str;

        memset(&caps_info, 0, sizeof(caps_info));
        channels = aud_event_value_get_int(event, "channels", -1);
        bits_per_sample = aud_event_value_get_int(event, "bits-per-sample", -1);
        rate = aud_event_value_get_int(event, "rate", -1);
        acmod = aud_event_value_get_int(event, "acmod", -1);
        check_downmix = aud_event_value_get_int(event, "check-downmix", -1);
        atmos = aud_event_value_get_bool(event, "atmos", 0);
        lfe_enable = aud_event_value_get_bool(event, "lfe-enable", 0);
        dolby = aud_event_value_get_bool(event, "dolby", 0);
        channel_mode = aud_event_value_get_int(event, "channel-mode", -1);
        clock_x4 = aud_event_value_get_bool(event, "clock-x4", 0);
        out_channels = aud_event_value_get_int(event, "out-channels", -1);
        es = aud_event_value_get_bool(event, "es", 0);
        codec_str = aud_event_value_get_str(event, "codec", NULL);
        data_rate = aud_event_value_get_int(event, "data-rate", rate);
        gap_correction = aud_event_value_get_int(event, "gap-correction", 0);
        downmix = aud_event_value_get_bool(event, "downmix", 0);
        if ((channels | bits_per_sample | rate | acmod) < 0) {
            LOGE(0x18e, "error first event");
            pthread_mutex_unlock(&adec->appsink_mutex);
            return (enum appsink_event_return)-1;
        }
        caps_info.gap_correction = gap_correction;
        caps_info.acmod = acmod;
        caps_info.check_downmix = check_downmix;
        caps_info.lfe_enable = lfe_enable;
        caps_info.atmos = atmos;
        caps_info.dolby = dolby;
        caps_info.channel_mode = channel_mode;
        caps_info.clock_x4 = clock_x4;
        caps_info.out_channels = out_channels;
        if (es) {
            caps_info.data_bps = bits_per_sample;
            caps_info.data_channels = channels;
            caps_info.es = true;
            caps_info.data_rate = data_rate;
            if (codec_str) {
                memcpy(caps_info.codec_str, codec_str, sizeof(caps_info.codec_str));
                LOGN(0x1ae, "caps_info.codec_str = %s\n", caps_info.codec_str);
            }
            adec->first_event_done[LGADEC_DATA_ES] = true;
        } else {
            caps_info.freq = rate;
            caps_info.channels = channels;
            caps_info.bits_per_sample = bits_per_sample;
            if (codec_str) {
                memcpy(caps_info.codec_str, codec_str, sizeof(caps_info.codec_str));
                LOGN(0x1ae, "caps_info.codec_str = %s\n", caps_info.codec_str);
            }
            caps_info.data_rate = data_rate;
            adec->first_event_done[LGADEC_DATA_PCM] = true;
        }
        caps_info.downmix = downmix;
        caps_info.data_rate = data_rate;

        _lgadec_process_decoded_info(adec, &caps_info, es);
        break;
    }
    case AUD_EVENT_TYPE_LAST_AU:
        if (adec->eos_event_status == LGADEC_EOS_EVENT_RECEIVED) {
            _lgadec_callback_to_gst(adec, LGADEC_EVENT_EOS, NULL);
            adec->eos_event_status = LGADEC_EOS_EVENT_RECEIVED_AND_SENDED;
        }
        break;
    case AUD_EVENT_TYPE_FLUSH:
        LOGN(0x1c8, "Got AUD_EVENT_TYPE_FLUSH event");
        if (event->prop & 2) {
            _lgadec_callback_to_gst(adec, LGADEC_EVENT_FLUSH_DONE, NULL);
            if (--adec->flush_done <= 0) {
                LOGN(0x1cf, "cond_signal\n");
                pthread_mutex_lock(&adec->flush_event_mutex);
                pthread_cond_signal(&adec->flush_event_cond);
                pthread_mutex_unlock(&adec->flush_event_mutex);
            }
        } else {
            adec->flush_done++;
        }
        break;
    case AUD_EVENT_TYPE_MPEGH_UI:
        _lgadec_callback_to_gst(adec, LGADEC_EVENT_DEC_MPEGH_UI, NULL);
        break;
    case AUD_EVENT_TYPE_CUSTOM: {
        unsigned int limit;

        if (aud_event_value_get_bool(event, "ui-config-changed", 0))
            _lgadec_callback_to_gst(adec, LGADEC_EVENT_DEC_MPEGH_UI, NULL);
        limit = aud_event_value_get_int(event, "ac4_dialog_enhancement_limit", 0) * 3;
        if ((int)limit > 0) {
            adec->de_limit = limit;
            _lgadec_callback_to_gst(adec, LGADEC_EVENT_DIALOG_ENHANCEMENT_LIMIT_CHANGED, NULL);
            if (adec->de_gain != UNSET_PARAM) {
                if (adec->de_gain < limit) {
                    LOGI(0x1ff, "user-preference dialog enhancement gain < limit. set de_gain : %d\n",
                         adec->de_gain);
                    if (lgadec_decoder_set_ac4_dialog_enhancement_gain(adec, adec->de_gain))
                        LOGE(0x202, "Set Ac4 Dialog Enhancement Gain Err");
                } else {
                    LOGI(0x1f9, "user-preference dialog enhancement gain >= limit. set de_gain : %d\n",
                         limit);
                    if (lgadec_decoder_set_ac4_dialog_enhancement_gain(adec, limit))
                        LOGE(0x1fc, "Set Ac4 Dialog Enhancement Gain Err");
                }
            }
        }
        break;
    }
    default:
        LOGD(0x20b, "unkown event");
        break;
    }
    pthread_mutex_unlock(&adec->appsink_mutex);
    return appsink_event_not_handled;
}

static inline int lgadec_task_stop(lgadec *adec)
{
    int status;
    int thr_ret;

    if (adec->task_status == LGADEC_TASK_NONE) {
        LOGE(0x364, "stop task alreay\n");
        return 0;
    }
    pthread_mutex_lock(&adec->pipe_mutex);
    write(adec->pipe[1], "stop", 4);
    LOGN(0x36a, "pipe write: stop");
    pthread_mutex_unlock(&adec->pipe_mutex);
    thr_ret = pthread_join(adec->task, (void **)&status);
    if (thr_ret != 0) {
        LOGE(0x36f, "task join ERR (thr_ret:%d, status:%d)\n", thr_ret, status);
        return -1;
    }
    adec->task_status = LGADEC_TASK_NONE;
    LOGI(0x375, "task joined %d\n", status);
    return 0;
}

/* @0x2cb44 */
int lgadec_stop(void *a)
{
    lgadec *adec = a;
    int ret = 0;

    if (!adec) {
        LOGE(0x87f, "adec is null");
        return -1;
    }
    if (!adec->bypass_decoder) {
        if (adec->fd < 0) {
            LOGE(0x87f, "audio device is not opened");
            return -1;
        }
        if (adec->decoder_state == LGADEC_DECODER_STATE_INIT) {
            LOGI(0x883, "decoder state is initialization");
            return 0;
        }
    }
    if (adec->no_dec) {
        LOGI(0x888, "no decoder (codec:%d). Decoder did not open and start", adec->codec_type);
        return 0;
    }

    if (lgadec_task_stop(adec) != 0) {
        LOGE(0x88e, "task stop Err!!\n");
        ret = -1;
    }
    close(adec->pipe[0]);
    close(adec->pipe[1]);
    close(adec->preroll_pipe[0]);
    close(adec->preroll_pipe[1]);
    if (lgadec_decoder_stop(adec) != 0) {
        LOGE(0x899, "Stop Err");
        ret = -1;
    }
    if (adec->appsrc) {
        LOGI(0x89e, "flush event");
        lgadec_flush_event_async(adec);
        lgadec_flush_event_sync(adec);
    }
    if (lgadec_decoder_disconnect(adec) != 0) {
        LOGE(0x8a4, "DisConnect Err");
        ret = -1;
    }
    LOGI(0x8a8, "stop");
    return ret;
}

/* @0x2ce90 */
int lgadec_register_callback_fn(void *a, LGADEC_CALLBACK_FN callback_fn, void *data)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x8b3, "adec is null");
        return -1;
    }
    if (!callback_fn) {
        LOGE(0x8b6, "callback function pointer is NULL!!");
        return -1;
    }
    adec->callback_fn = callback_fn;
    adec->callback_data = data;
    return 0;
}

static int lgadec_flush_event_async(void *a);

/* @0x28764: push a flush through the DSP chain and wait (1 s) for its FLUSH_DONE */
static int lgadec_flush_event_sync(void *a)
{
    lgadec *adec = a;
    struct timespec ts;

    if (!adec) {
        LOGE(0x9b0, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x9b0, "audio device is not opened");
        return -1;
    }
    if (adec->no_dec)
        return 0;
    pthread_mutex_lock(&adec->resource_mutex);
    if (!adec->bypass_decoder) {
        pthread_mutex_lock(&adec->flush_event_mutex);
        module_appsrc_push_event(adec->appsrc, aud_event_new_flush());
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;
        if (pthread_cond_timedwait(&adec->flush_event_cond, &adec->flush_event_mutex, &ts) == ETIMEDOUT)
            LOGN(0x9c5, "wait timed out\n");
        pthread_mutex_unlock(&adec->flush_event_mutex);
    }
    return pthread_mutex_unlock(&adec->resource_mutex);
}

/* @0x2886c */
static int lgadec_flush_event_async(void *a)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x995, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x995, "audio device is not opened");
        return -1;
    }
    if (adec->no_dec)
        return 0;
    adec->flush_done = 0;
    pthread_mutex_lock(&adec->resource_mutex);
    if (!adec->bypass_decoder)
        module_appsrc_push_event(adec->appsrc, aud_event_new_flush_async());
    return pthread_mutex_unlock(&adec->resource_mutex);
}

/* @0x2d12c */
int lgadec_flush(void *a)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x9d5, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x9d5, "audio device is not opened");
        return -1;
    }
    LOGN(0x9d7, "flush Start");
    pthread_mutex_lock(&adec->rc_mutex);
    if (adec->rc_paused) {
        LOGD(0x9db, "rc_cond signal");
        adec->rc_paused = 0;
        pthread_cond_signal(&adec->rc_cond);
    }
    pthread_mutex_unlock(&adec->rc_mutex);
    if (adec->appsrc) {
        lgadec_flush_event_async(adec);
        lgadec_flush_event_sync(adec);
    }
    pthread_mutex_lock(&adec->task_mutex);
    adec->eos_event_status = LGADEC_EOS_EVENT_NOT_RECEIVED;
    pthread_mutex_unlock(&adec->task_mutex);
    if (adec->selected_codec && adec->selected_codec->flush)
        adec->selected_codec->flush(adec);
    memset(adec->decoded_frame_cnt, 0, sizeof(adec->decoded_frame_cnt));
    memset(adec->prev_timestamp, 0xff, sizeof(adec->prev_timestamp));
    memset(adec->buffering_duration, 0, sizeof(adec->buffering_duration));
    LOGI(0x9f4, "flush done");
    return 0;
}

/* @0x2d2c4: returns 1 = "element may forward EOS right now" (PCM/ADPCM, or bypass for
 * every other codec returns 0 without pushing LAST to the DSP). */
int lgadec_feed_eos(void *a)
{
    lgadec *adec = a;
    bool host_codec;

    if (!adec) {
        LOGE(0x9fe, "adec is null");
        return -1;
    }
    host_codec = adec->codec_type == LGADEC_CODEC_ADPCM || adec->codec_type == LGADEC_CODEC_PCM;
    if (adec->bypass_decoder) {
        if (!host_codec)
            return 0;
    } else {
        if (adec->fd < 0) {
            LOGE(0x9fe, "audio device is not opened");
            return -1;
        }
        if (!host_codec) {
            LOGT(0xa0a, "Feed LG_AUD_BUFFER_END data");
            if (adec->flushing == 1) {
                LOGN(0xa0d, "Flushing...");
                return 0;
            }
            adec->eos_event_status = LGADEC_EOS_EVENT_RECEIVED;
            LOGN(0xa13, "set eosEventStatus as LGADEC_EOS_EVENT_RECEIVED");
            module_appsrc_push_event(adec->appsrc, aud_event_new_last());
            return 0;
        }
    }
    LOGN(0xa02, "Send EOS right now when PCM codec");
    return 1;
}

extern SELECTED_CODEC_T codec_type_none, codec_type_private, codec_type_pcm, codec_type_aac,
    codec_type_ac4, codec_type_adpcm, codec_type_flac, codec_type_vorbis, codec_type_opus,
    codec_type_iamf;
extern void *lgadec_pcm_create(void);
extern void *lgadec_aac_create(LGADEC_CODEC_FORMAT_T format);
extern void *lgadec_ac4_create(LGADEC_CODEC_FORMAT_T format);
extern void *lgadec_adpcm_create(LGADEC_ADPCM_T type);
extern void *lgadec_flac_create(void);
extern void *lgadec_vorbis_create(void);
extern void *lgadec_opus_create(void);
extern void *lgadec_iamf_create(void);

/* inlined into lgadec_select_codec: codecs whose DSP decoder also emits pass-through ES
 * on output port 1 (constant 0x480007 >> (codec - AC3)). No DTS. */
static inline bool lgadec_is_available_bypass_output(LGADEC_CODEC_T codec)
{
    switch (codec) {
    case LGADEC_CODEC_AC3: case LGADEC_CODEC_EAC3: case LGADEC_CODEC_AAC:
    case LGADEC_CODEC_MAT: case LGADEC_CODEC_EAC3_ATMOS:
        return true;
    default:
        return false;
    }
}

static inline bool freq_in(int freq, int lo, int hi) { return freq == 0 || (freq >= lo && freq <= hi); }

/* @0x2d444: validate stream info per codec, pick the host-side feeder
 * (selected_codec) and decide whether a DSP decoder is used at all (no_dec).
 * MAT has an enum/map entry but no case: it is rejected even with es-bypass enabled. */
int lgadec_select_codec(void *a, LGADEC_CODEC_INFO_T *codec_info)
{
    lgadec *adec = a;
    LGADEC_STREAM_INFO_T *si;
    void *data;

    if (!adec) {
        LOGE(0xa3f, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0xa3f, "audio device is not opened");
        return -1;
    }
    si = &adec->stream_info;
    adec->no_dec = 0;
    adec->pcm_type = LGADEC_PCM_NONE;
    if (adec->selected_codec) {
        adec->selected_codec->cleanup(&adec->codec_data);
        adec->selected_codec = NULL;
    }
    switch (codec_info->codec_type) {
    case LGADEC_CODEC_MP3:
        if (si->freq != 0 && (si->freq < 8000 || si->freq > 48000)) {
            LOGE(0xa51, "freq(%u) is not supported", si->freq);
            return -1;
        }
        if (si->channels > 2 && si->channels != 6) {
            LOGE(0xa59, "channels(%u) is not supported", si->channels);
            return -1;
        }
        break;
    case LGADEC_CODEC_AAC:
        adec->is_disable_reform = codec_info->disable_dsp;
        if (si->freq != 0 && (si->freq < 8000 || si->freq > 96000)) {
            LOGE(0xa67, "freq(%u) is not supported", si->freq);
            return -1;
        }
        if (si->channels > 8) {
            LOGE(0xa6c, "channels(%u) is not supported", si->channels);
            return -1;
        }
        data = lgadec_aac_create(codec_info->codec_format);
        if (!data) {
            LOGE(0xa73, "aac data create fail!!");
            return -1;
        }
        adec->codec_data = data;
        adec->selected_codec = &codec_type_aac;
        goto selected;
    case LGADEC_CODEC_AAC_ELD:
        if (si->channels > 2) {
            LOGE(0xa80, "AAC-ELD do not support multi-channel (channel:%d)", si->channels);
            return LGADEC_RET_NOT_SUPPORTED;
        }
        break;
    case LGADEC_CODEC_AC3:
    case LGADEC_CODEC_EAC3:
        if (codec_info->codec_format == LGADEC_AC3_FORMAT) {
            if (si->freq != 0 && si->freq != 32000 && si->freq != 44100 && si->freq != 48000) {
                LOGE(0xa92, "freq(%u) is not supported", si->freq);
                return -1;
            }
            if (si->channels > 6) {
                LOGE(0xa97, "channels(%u) is not supported", si->channels);
                return -1;
            }
            adec->selected_codec = &codec_type_none;
        } else if (codec_info->codec_format == LGADEC_AC3_PRIVATE_FORMAT) {
            adec->selected_codec = &codec_type_private;
        } else {
            LOGE(0xaa4, "invalid AC3 format!!");
            return -1;
        }
        goto selected;
    case LGADEC_CODEC_AC4:
        if (si->freq != 0 && si->freq != 44100 && si->freq != 48000) {
            LOGE(0xaaf, "freq(%u) is not supported", si->freq);
            return -1;
        }
        if (si->channels > 12) {
            LOGE(0xab4, "channels(%u) is not supported", si->channels);
            return -1;
        }
        data = lgadec_ac4_create(codec_info->codec_format);
        if (!data) {
            LOGE(0xabb, "ac4 data create fail!!");
            return -1;
        }
        adec->codec_data = data;
        adec->selected_codec = &codec_type_ac4;
        goto selected;
    case LGADEC_CODEC_AMR_WB:
        if (si->freq != 0 && si->freq != 16000) {
            LOGE(0xac9, "freq(%u) is not supported", si->freq);
            return -1;
        }
        if (si->channels > 1) {
            LOGE(0xacf, "channels(%u) is not supported", si->channels);
            return -1;
        }
        break;
    case LGADEC_CODEC_AMR_NB:
        if (si->freq != 0 && si->freq != 8000) {
            LOGE(0xadc, "freq(%u) is not supported", si->freq);
            return -1;
        }
        if (si->channels > 1) {
            LOGE(0xae2, "channels(%u) is not supported", si->channels);
            return -1;
        }
        break;
    case LGADEC_CODEC_FLAC:
        LOGI(0xb1a, "flac enter");
        if (si->freq > 192000) {
            LOGE(0xb1c, "freq(%u) is not supported", si->freq);
            return -1;
        }
        if (si->channels > 8) {
            LOGE(0xb21, "channels(%u) is not supported", si->channels);
            return -1;
        }
        data = lgadec_flac_create();
        if (!data) {
            LOGE(0xb27, "flac data create fail!!");
            return -1;
        }
        adec->codec_data = data;
        adec->selected_codec = &codec_type_flac;
        goto selected;
    case LGADEC_CODEC_MPEG_H:
        if (si->freq != 0 && (si->freq < 8000 || si->freq > 48000)) {
            LOGE(0xb36, "freq(%u) is not supported", si->freq);
            return -1;
        }
        if (si->channels > 16) {
            LOGE(0xb3b, "channels(%u) is not supported", si->channels);
            return -1;
        }
        break;
    case LGADEC_CODEC_OPUS:
        free(si->dsi_buffer);
        si->dsi_buffer = codec_info->buffer;
        si->dsi_buffer_length = codec_info->buf_size;
        if (si->freq != 0 && si->freq != 8000 && si->freq != 12000 && si->freq != 16000 &&
            si->freq != 24000 && si->freq != 48000) {
            LOGE(0xb54, "freq(%u) is not supported", si->freq);
            goto free_dsi;
        }
        data = lgadec_opus_create();
        if (!data) {
            LOGE(0xb62, "opus data create fail!!");
            goto free_dsi;
        }
        adec->codec_data = data;
        adec->selected_codec = &codec_type_opus;
        goto selected;
    case LGADEC_CODEC_VORBIS:
        if (!codec_info->buffer) {
            LOGE(0xb76, "codec_data is NULL");
            return -1;
        }
        free(si->dsi_buffer);
        si->dsi_buffer = codec_info->buffer;
        si->dsi_buffer_length = codec_info->buf_size;
        if (si->freq != 0 && (si->freq < 8000 || si->freq > 48000)) {
            LOGE(0xb86, "freq(%u) is not supported", si->freq);
            goto free_dsi;
        }
        if (si->channels > 2) {
            LOGE(0xb8d, "channels(%u) is not supported", si->channels);
            goto free_dsi;
        }
        data = lgadec_vorbis_create();
        if (!data) {
            LOGE(0xb93, "flac data create fail!!");
            goto free_dsi;
        }
        adec->codec_data = data;
        adec->selected_codec = &codec_type_vorbis;
        goto selected;
    case LGADEC_CODEC_WMA_PRO: {
        switch (codec_info->codec_format) {
        case LGADEC_WMA2_FORMAT:
            if (si->freq != 0 && (si->freq < 8000 || si->freq > 96000)) {
                LOGE(3000, "freq(%u) is not supported", si->freq);
                return -1;
            }
            si->wma_format_tag = 0x161;
            break;
        case LGADEC_WMA_PRO_FORMAT:
            if (si->freq != 0 && (si->freq < 8000 || si->freq > 96000)) {
                LOGE(0xbc3, "freq(%u) is not supported", si->freq);
                return -1;
            }
            si->wma_format_tag = 0x162;
            break;
        case LGADEC_WMA_LOSSLESS_FORMAT:
            if (si->freq != 0 && (si->freq < 44100 || si->freq > 192000)) {
                LOGE(0xbce, "freq(%u) is not supported", si->freq);
                return -1;
            }
            si->wma_format_tag = 0x163;
            break;
        default:
            LOGE(0xbd6, "WMA format tag(%d) is not supported", codec_info->codec_format);
            return -1;
        }
        if (si->channels > 8) {
            LOGE(0xbde, "channels(%u) is not supported", si->channels);
            return -1;
        }
        break;
    }
    case LGADEC_CODEC_ADPCM: {
        int adpcm_type = codec_info->codec_format;

        si->bits_per_sample = 16;
        si->adpcm_type = adpcm_type;
        if (si->block_align == 0 && adpcm_type == 1 /* LGADEC_ADPCM_IMA_QT */) {
            si->block_align = si->channels * 34;
            LOGI(0xbf5, "block_align is Zero(ADPCM layout is LGADEC_ADPCM_IMA_QT). Cal block align -> %u",
                 si->block_align);
        }
        if (si->freq > 384000) {
            LOGE(0xbfa, "freq(%u) is not supported", si->freq);
            return -1;
        }
        if (si->channels > 2) {
            LOGE(0xc01, "channels(%u) is not supported", si->channels);
            return -1;
        }
        if (adpcm_type != 1 && adpcm_type != 2 && adpcm_type != 7) {  /* IMA_QT, IMA_WAV, MS */
            LOGE(0xc09, "adpcm_type(%d) is not supported", adpcm_type);
            return -1;
        }
        data = lgadec_adpcm_create(adpcm_type);
        if (!data) {
            LOGE(0xc10, "adpcm data create fail!!");
            return -1;
        }
        adec->codec_data = data;
        adec->no_dec = 1;                           /* decoded on the host (chain) */
        adec->selected_codec = &codec_type_adpcm;
        goto selected;
    }
    case LGADEC_CODEC_PCM: {
        LGADEC_PCM_T type = codec_info->codec_format;

        switch (type) {
        case LGADEC_PCM_BASE:
            if (si->freq == 0 || si->freq > 384000) {
                LOGE(0xc25, "freq(%u) is not supported", si->freq);
                return -1;
            }
            if (si->channels < 1 || si->channels > 8) {
                LOGE(0xc2b, "channels(%u) is not supported", si->channels);
                return -1;
            }
            break;
        case LGADEC_PCM_MULAW:
        case LGADEC_PCM_ALAW:
            si->bits_per_sample = 16;
            LOGD(0xc35, "bits_per_sample = %d", 16);
            if (si->freq != 0 && (si->freq < 8000 || si->freq > 192000)) {
                LOGE(0xc3a, "freq(%u) is not supported", si->freq);
                return -1;
            }
            if (si->channels > 2) {
                LOGE(0xc41, "channels(%u) is not supported", si->channels);
                return -1;
            }
            break;
        case LGADEC_PCM_WIFI:
        case LGADEC_PCM_DVD:
        case LGADEC_PCM_BD:
            break;
        default:
            LOGE(0xc4d, "unknown pcm type:%d", type);
            return -1;
        }
        data = lgadec_pcm_create();
        if (!data) {
            LOGE(0xc53, "adpcm data create fail!!");
            return -1;
        }
        adec->codec_data = data;
        adec->pcm_type = type;
        adec->selected_codec = &codec_type_pcm;
        adec->no_dec = 1;
        goto selected;
    }
    case LGADEC_CODEC_IAMF:
        data = lgadec_iamf_create();
        if (!data) {
            LOGE(0xc63, "iamf data create fail!!");
            return -1;
        }
        adec->codec_data = data;
        adec->selected_codec = &codec_type_iamf;
        goto selected;
    default:
        LOGE(0xc6f, "not support codec type (%d)", codec_info->codec_type);
        return -1;
    }
    adec->selected_codec = &codec_type_none;

selected:
    adec->codec_type = codec_info->codec_type;
    LOGN(0xc75, "select codec done (codec_type:%s, no_dec:%s)", lgadec_get_codec_name(adec->codec_type),
         adec->no_dec ? "TRUE" : "FALSE");
    adec->wait_bypass_first_event = lgadec_is_available_bypass_output(adec->codec_type);
    return 0;

free_dsi:
    free(si->dsi_buffer);
    si->dsi_buffer = NULL;
    si->dsi_buffer_length = 0;
    return -1;
}

/* @0x2e2b4: hand one input access unit to the codec-specific feeder
 * (which ends in lgadec_decoder_feed) */
int lgadec_feed_data(void *a, unsigned char *data, unsigned int len, unsigned long long timestamp)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xc86, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0xc86, "audio device is not opened");
        return -1;
    }
    if (!adec->selected_codec) {
        LOGE(0xc87, "adec->selected_codec is null");
        return -1;
    }
    if (adec->selected_codec->feed(adec, data, len, timestamp) != 0)
        return -1;
    adec->fed_frame_cnt++;
    LOGT(0xc8e, "fed frame cnt:%llu", adec->fed_frame_cnt);
    return 0;
}

/* @0x2e42c */
int lgadec_get_no_decoder(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xca8, "adec is null");
        return -1;
    }
    *value = adec->no_dec;
    return 0;
}

/* @0x2e48c */
int lgadec_get_prev_timestamp(void *a, LGADEC_DATA_TYPE_T out_type, unsigned long long *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xcb5, "adec is null");
        return -1;
    }
    *value = adec->prev_timestamp[out_type];
    return 0;
}

/* @0x2e554 */
int lgadec_get_codec_type(void *a, LGADEC_CODEC_T *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xccd, "adec is null");
        return -1;
    }
    *value = adec->codec_type;
    return 0;
}

/* @0x2eb64: duration base for _gst_lg_adec_push_data. While bypassing, a fixed
 * 352800 B/s is reported (log: "so make random value"). */
int lgadec_get_byte_per_sec(void *a, int *bytes_per_sec)
{
    lgadec *adec = a;
    int channels, bits;

    if (!adec) {
        LOGE(0xda0, "adec is null");
        return -1;
    }
    if (adec->bypass_decoder) {
        *bytes_per_sec = BYPASS_FAKE_BYTES_PER_SEC;
        LOGD(0xda4, "bypass decoder.. so make random value.. %d\n", BYPASS_FAKE_BYTES_PER_SEC);
        return 0;
    }
    channels = adec->audio_caps.channels;
    if (channels < 1 || channels > 10) {
        LOGE(0xdaa, "Invalid channels : %d", channels);
        return -1;
    }
    bits = adec->audio_caps.bits_per_sample;
    if (bits < 8 || bits > 32) {
        LOGE(0xdaf, "Invalid bits per sample : %d", bits);
        return -1;
    }
    if (bytes_per_sec)
        *bytes_per_sec = (bits >> 3) * adec->audio_caps.freq * channels;
    return 0;
}

/* @0x2eee4: the "es-bypass" property value (no NULL log) */
int lgadec_get_es_bypass(void *a)
{
    lgadec *adec = a;

    return adec ? adec->bypass_decoder : 0;
}

/* @0x2f00c */
int lgadec_set_flushing(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xe3a, "adec is null");
        return -1;
    }
    adec->flushing = value;
    pthread_mutex_lock(&adec->rc_mutex);
    if (adec->rc_paused) {
        LOGD(0xe40, "rc_cond signal");
        adec->rc_paused = 0;
        pthread_cond_signal(&adec->rc_cond);
    }
    pthread_mutex_unlock(&adec->rc_mutex);
    return 0;
}

/* @0x2f0b4 */
int lgadec_set_next_state(void *a, LGADEC_GST_STATE_T value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xe4e, "adec is null");
        return -1;
    }
    adec->next_state = value;
    pthread_mutex_lock(&adec->rc_mutex);
    if (adec->rc_paused && adec->next_state < LGADEC_GST_STATE_PAUSED) {
        LOGD(0xe54, "rc_cond signal");
        adec->rc_paused = 0;
        pthread_cond_signal(&adec->rc_cond);
    }
    pthread_mutex_unlock(&adec->rc_mutex);
    return 0;
}

/* @0x2f424 */
int lgadec_set_seamless(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xec3, "adec is null");
        return -1;
    }
    adec->seamless = value;
    return 0;
}

/* @0x2f480 */
int lgadec_set_output_mode(void *a, unsigned int output_mode)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xecf, "adec is null");
        return -1;
    }
    if (output_mode >= 3) {
        LOGE(0xed3, "Audio output type is invalid value: %u", output_mode);
        return 0;
    }
    adec->output_mode = output_mode;
    if (lgadec_decoder_set_audio_output_mode(adec, output_mode + 4) == 0)
        LOGD(0xedc, "Set Audio(Dual-mono) Output Mode (4: LR_FORCED, 5: LL_FORCED, 6: RR_FORCED): %u",
             output_mode + 4);
    else
        LOGE(0xeda, "Set Audio(Dual-mono) Mode Err");
    return 0;
}

/* @0x2f578 */
int lgadec_set_index(void *a, unsigned int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xeea, "adec is null");
        return -1;
    }
    adec->index = value;
    return 0;
}

/* @0x2f5d4 */
int lgadec_set_prev_timestamp(void *a, LGADEC_DATA_TYPE_T out_type, unsigned long long value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xf05, "adec is null");
        return -1;
    }
    adec->prev_timestamp[out_type] = value;
    return 0;
}

/* @0x3007c: the actual switch, run on the lgadec task thread (pipe "SUB0"/"SUB1").
 * Waits up to 1 s for an in-flight lgadec_decoder_feed, then under resource_mutex:
 *   on : stop + disconnect + close the DSP decoder (ADEC freed, aimc chain destroyed)
 *   off: reopen, configure, reconnect and start it; replay PG index / default PRL. */
int lgadec_set_bypass_decoder(void *a, bool on)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x287, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x287, "audio device is not opened");
        return -1;
    }
    LOGN(0x289, "set bypass as %s\n", on ? "On" : "Off");
    if (adec->bypass_decoder == on) {
        LOGN(0x28c, "same.. ignore....\n");
        return 0;
    }
    if (adec->use_decoder_feed) {
        struct timespec ts;

        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;
        LOGN(0x295, "wait.. %d\n", adec->use_decoder_feed);
        pthread_mutex_lock(&adec->resource_mutex);
        if (pthread_cond_timedwait(&adec->resource_cond, &adec->resource_mutex, &ts) == 0)
            LOGN(0x299, "got condition.... %d\n", adec->use_decoder_feed);
        LOGN(0x29c, "wait done... %d\n", adec->use_decoder_feed);
        pthread_mutex_unlock(&adec->resource_mutex);
    }

    pthread_mutex_lock(&adec->resource_mutex);
    if (on) {
        LOGI(0x2a2, "stop & disconnect decoder\n");
        lgadec_decoder_stop(adec);
        lgadec_decoder_disconnect(adec);
        lgadec_decoder_close(adec);
    } else {
        LOGI(0x2a7, "open & connect & start decoder\n");
        lgadec_decoder_open(adec);
        lgadec_decoder_configure_decoder(adec);
        lgadec_decoder_connect(adec);
        lgadec_decoder_start(adec);
        if (adec->presentation_group_index)
            lgadec_set_presentation_group_index(adec, adec->presentation_group_index);
        if (adec->default_prl != UNSET_PARAM)
            lgadec_set_default_prl(adec, adec->default_prl);
    }
    LOGN(0x2b6, "done.\n");
    adec->bypass_decoder = on;
    pthread_mutex_unlock(&adec->resource_mutex);
    return 0;
}

/* @0x30344: control thread. preroll_pipe "preroll" -> LGADEC_EVENT_PREROLL;
 * pipe "stop" ends the task, "SUB0"/"SUB1" switch the ES bypass. */
static void *lgadec_task_run(void *data)
{
    lgadec *adec = data;
    int ret = 0;
    struct pollfd event_fd[2];

    for (;;) {
        int nfds = 2;

        memset(event_fd, 0, sizeof(event_fd));
        event_fd[1].fd = adec->pipe[0];
        event_fd[0].fd = adec->preroll_pipe[0];
        event_fd[1].events = POLLIN;
        event_fd[0].events = POLLIN;
        ret = poll(event_fd, nfds, -1);
        if (ret < 0) {
            LOGE(0x30c, "poll Err!! %s\n", strerror(errno));
            return NULL;
        }
        if (event_fd[0].revents & POLLIN) {
            char buf[8] = {0};
            ssize_t readed;

            LOGI(0x314, "callback event from PREROLL_PIPE\n");
            readed = read(adec->preroll_pipe[0], buf, 7);
            if (readed > 0) {
                if (strncmp(buf, "preroll", 7) == 0)
                    _lgadec_callback_to_gst(adec, LGADEC_EVENT_PREROLL, NULL);
                else
                    LOGE(0x31a, "wrong message!! %s\n", buf);
            } else {
                LOGE(0x31d, "read Err %d\n", readed);
            }
        }
        if (event_fd[1].revents & POLLIN) {
            char buf[8] = {0};
            ssize_t readed;

            LOGI(0x325, "callback event from PIPE\n");
            readed = read(adec->pipe[0], buf, 7);
            if (readed > 0) {
                if (strncmp(buf, "stop", 4) == 0) {
                    adec->task_status = LGADEC_TASK_STOP;
                    LOGI(0x32a, "set task status: stop\n");
                    return NULL;
                } else if (buf[0] == 'S' && buf[1] == 'U' && buf[2] == 'B') {
                    bool onoff = strncmp(buf, "SUB0", 4) != 0;

                    LOGN(0x333, "send to SUB%d\n", onoff);
                    lgadec_set_bypass_decoder(adec, onoff);
                } else {
                    LOGE(0x336, "wrong message!! %s\n", buf);
                }
            } else {
                LOGE(0x339, "read Err %d\n", readed);
            }
        }
    }
}

/* @0x307ac */
int lgadec_set_preroll(void *a)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x103f, "adec is null");
        return -1;
    }
    write(adec->preroll_pipe[1], "preroll", 7);
    LOGI(0x1043, "set preroll\n");
    return 0;
}

/* @0x30848: "es-bypass" property setter ("ES BYPASS & DROP" / "NORMAL DECODING").
 * Before the first lgadec_start_init (no stream yet) only the flag is stored; afterwards
 * the request is posted to the task thread, which runs lgadec_set_bypass_decoder. */
int lgadec_set_es_bypass(void *a, bool on)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x104d, "adec is null");
        return -1;
    }
    LOGN(0x104f, "set as %s\n", on ? "ES BYPASS & DROP" : "NORMAL DECODING");
    if (adec->inited) {
        pthread_mutex_lock(&adec->pipe_mutex);
        write(adec->pipe[1], on ? "SUB1" : "SUB0", 4);
        LOGN(0x105c, "pipe write: SUB%d", on);
        pthread_mutex_unlock(&adec->pipe_mutex);
        return 0;
    }
    LOGN(0x1052, "set bypass decoder now\n");
    adec->bypass_decoder = on;
    return 0;
}

static inline int lgadec_task_start(lgadec *adec)
{
    if (adec->task_status == LGADEC_TASK_NONE) {
        pthread_mutex_lock(&adec->task_mutex);
        if (pthread_create(&adec->task, NULL, lgadec_task_run, adec) < 0) {
            LOGE(0x34f, "task create Err\n");
        } else {
            adec->task_status = LGADEC_TASK_RUN;
            LOGI(0x356, "task create success\n");
        }
        pthread_mutex_unlock(&adec->task_mutex);
    } else {
        LOGE(0x347, "alreay task is running\n");
        return -1;
    }
    return 0;
}

/* @0x30cb0: bypass_decoder skips configure/connect/start (no DSP), but the control task
 * and pipes are still created so SUB0 can bring the decoder back later. */
int lgadec_start(void *a)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x83c, "adec is null");
        return -1;
    }
    if (!ADEC_USABLE(adec)) {
        LOGE(0x83c, "audio device is not opened");
        return -1;
    }
    if (adec->no_dec)
        goto no_decoder;
    if (!adec->bypass_decoder) {
        /* sic: this message is printed on the NON-bypass path (0x20ce0 -> 0x20df6 -> 0x20ce6) */
        LOGN(0x846, "bypass decoder is on... so not config & connect & start\n");
        if (lgadec_decoder_configure_decoder(adec) == -1) {
            LOGE(0x849, "lgadec_decoder_configure_decoder (adec) Err!");
            goto fail;
        }
        if (lgadec_decoder_connect(adec) == -1) {
            LOGE(0x84c, "lgadec_decoder_connect (adec) Err!");
            goto fail;
        }
        if (lgadec_decoder_start(adec) == -1) {
            LOGE(0x84f, "lgadec_decoder_start (adec) Err!");
            goto fail;
        }
    }

    if (pipe(adec->pipe) < 0) {
        LOGE(0x854, "cannot make pipe. %s\n", strerror(errno));
        goto fail;
    }
    if (pipe(adec->preroll_pipe) < 0) {
        LOGE(0x859, "cannot make preroll_pipe. %s\n", strerror(errno));
        goto fail;
    }
    if (adec->presentation_group_index)
        lgadec_set_presentation_group_index(adec, adec->presentation_group_index);
    if (adec->default_prl != UNSET_PARAM)
        lgadec_set_default_prl(adec, adec->default_prl);
    if (adec->dolby_drc_mode != UNSET_PARAM)
        lgadec_set_dolby_drc_mode(adec, adec->dolby_drc_mode);
    if (adec->sound_system != UNSET_PARAM)
        lgadec_set_sound_system(adec, adec->sound_system);

    lgadec_task_start(adec);
    LOGI(0x86f, "start");
    return 0;

no_decoder:
    LOGI(0x83f, "no decoder (codec:%d). config and start(DEC.%d) will be called by handle_frame().",
         adec->codec_type, adec->index);
    return 0;
fail:
    lgadec_decoder_close(adec);
    return -1;
}

/* ---- plain accessors (lgadec.c, 0x2e3cc..0x2efac) ---------------------------- */

/* @0x2e3cc */
int lgadec_get_pcm_type(void *a, LGADEC_PCM_T *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xc9c, "adec is null");
        return -1;
    }
    *value = adec->pcm_type;
    return 0;
}

/* @0x2e4f4 */
int lgadec_get_flushing(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xcc1, "adec is null");
        return -1;
    }
    *value = adec->flushing;
    return 0;
}

/* @0x2e5b4 */
int lgadec_get_play_speed(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xcd9, "adec is null");
        return -1;
    }
    *value = adec->play_speed;
    return 0;
}

/* @0x2e614 */
int lgadec_get_output_mode(void *a, unsigned int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xce5, "adec is null");
        return -1;
    }
    *value = adec->output_mode;
    return 0;
}

/* @0x2e674 */
int lgadec_get_decoder_state(void *a, LGADEC_DECODER_STATE_T *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xcf1, "adec is null");
        return -1;
    }
    *value = adec->decoder_state;
    return 0;
}

/* @0x2e6d4 */
int lgadec_get_bits_per_sample(void *a, unsigned char *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xcfe, "adec is null");
        return -1;
    }
    *value = adec->stream_info.bits_per_sample;
    return 0;
}

/* @0x2e734 */
int lgadec_get_number_of_channel(void *a, unsigned char *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd0c, "adec is null");
        return -1;
    }
    *value = adec->stream_info.channels;
    return 0;
}

/* @0x2e794 */
int lgadec_get_freq(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd1a, "adec is null");
        return -1;
    }
    *value = adec->stream_info.freq;
    return 0;
}

/* @0x2e7f4 */
int lgadec_get_block_align(void *a, unsigned int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd28, "adec is null");
        return -1;
    }
    *value = adec->stream_info.block_align;
    return 0;
}

/* @0x2e854 */
int lgadec_get_audio_caps_freq(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd35, "adec is null");
        return -1;
    }
    *value = adec->audio_caps.freq;
    return 0;
}

/* @0x2e8b4 */
int lgadec_get_audio_caps_channels(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd41, "adec is null");
        return -1;
    }
    *value = adec->audio_caps.channels;
    return 0;
}

/* @0x2e914 */
int lgadec_get_audio_caps_bits_per_sample(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd4d, "adec is null");
        return -1;
    }
    *value = adec->audio_caps.bits_per_sample;
    return 0;
}

/* @0x2e974 */
int lgadec_get_audio_caps_sign(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd59, "adec is null");
        return -1;
    }
    *value = adec->audio_caps.sign;
    return 0;
}

/* @0x2e9d4 */
int lgadec_get_audio_caps_endianness(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd65, "adec is null");
        return -1;
    }
    *value = adec->audio_caps.endianness;
    return 0;
}

/* @0x2ea34 */
int lgadec_get_audio_caps_acmod(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd71, "adec is null");
        return -1;
    }
    *value = adec->audio_caps.acmod;
    return 0;
}

/* @0x2ea94 */
int lgadec_get_atmos(void *a, int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd7e, "adec is null");
        return -1;
    }
    *value = adec->audio_caps.atmos;
    return 0;
}

/* @0x2ed60 */
int lgadec_get_ac4_dialog_enhancement_limit_min(void *a, unsigned int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xdd2, "adec is null");
        return -1;
    }
    *value = 0;
    return 0;
}

/* @0x2eeec */
int lgadec_get_dolby_drc_mode(void *a, unsigned int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xe15, "adec is null");
        return -1;
    }
    *value = adec->dolby_drc_mode;
    return 0;
}

/* @0x2eaf4: audio_mode_changed is a one-shot flag */
int lgadec_get_audio_mode(void *a, int *value, unsigned int *changed)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xd8d, "adec is null");
        return -1;
    }
    *value = adec->audio_mode;
    if (adec->audio_mode_changed) {
        *changed = 1;
        adec->audio_mode_changed = false;
    }
    return 0;
}

/* @0x2ec9c: returns 0 even when the ioctl wrapper fails */
int lgadec_get_ac4_dialog_enhancement_gain(void *a, unsigned int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xdc1, "adec is null");
        return -1;
    }
    if (lgadec_decoder_get_ac4_dialog_enhancement_gain(adec, value) != 0)
        LOGE(0xdc5, "Get Ac4 Dialog Enhancement Gain Err");
    else
        LOGD(0xdc7, "Get Ac4 Dialog Enhancement Gain (0 ~ 12 dB): %u", *value);
    return 0;
}

/* @0x2edc0 */
int lgadec_get_ac4_dialog_enhancement_limit_max(void *a, unsigned int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xdde, "adec is null");
        return -1;
    }
    if (lgadec_decoder_get_ac4_dialog_enhancement_limit(adec, value) != 0)
        LOGE(0xde2, "Get Ac4 Dialog Enhancement Limit Err");
    else
        LOGD(0xde4, "Get Ac4 Dialog Enhancement Limit : %u", *value);
    return 0;
}

/* @0x2ee84: DirectMedia buffers are not implemented: always 0 */
int lgadec_get_buffer_size_for_directmedia(void *a, unsigned int *size, LGADEC_DIRECTMEDIA_BUF_T buf)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xdf0, "adec is null");
        return -1;
    }
    *size = 0;
    return 0;
}

/* @0x2ef4c */
unsigned int lgadec_get_sound_system(void *a, unsigned int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xe21, "adec is null");
        return -1;
    }
    *value = adec->sound_system;
    return 0;
}

/* @0x2efac */
unsigned int lgadec_get_out_channels(void *a, unsigned int *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xe2d, "adec is null");
        return -1;
    }
    *value = adec->audio_caps.out_channels;
    return 0;
}

/* @0x2f164 */
int lgadec_set_app_type(void *a, LGADEC_APP_TYPE_T value)
{
    lgadec *adec = a;
    const char *name;

    if (!adec) {
        LOGE(0xe63, "adec is null");
        return -1;
    }
    switch (value) {
    case LGADEC_APP_TYPE_RTC:      name = "RTC"; break;
    case LGADEC_APP_TYPE_MIRACAST: name = "MIRACAST"; break;
    case LGADEC_APP_TYPE_JP4K:     name = "JP4K"; break;
    case LGADEC_APP_TYPE_APPCAST:  name = "APPCAST"; break;
    case LGADEC_APP_TYPE_SYE:      name = "SYE"; break;
    default:                       name = "UNKNOWN"; break;
    }
    LOGI(0xe7f, "AppType received %s", name);
    adec->app_type = value;
    return 0;
}

/* @0x2f234: 0 pauses the render-control wait (rc_paused, returns 1); any other rate
 * resumes it, and a changed rate reconfigures AC3/EAC3 decoders (play_speed is part of
 * their codec param). no_dec codecs are configured later from the chain. */
int lgadec_set_play_speed(void *a, int value)
{
    lgadec *adec = a;
    int ret;

    if (!adec) {
        LOGE(0xe8b, "adec is null");
        return -1;
    }
    LOGI(0xe8d, "set playrate:%d (prev playrate:%d)", value, adec->play_speed);
    if (value == 0) {
        pthread_mutex_lock(&adec->rc_mutex);
        LOGD(0xe91, "paused");
        adec->rc_paused = 1;
        pthread_mutex_unlock(&adec->rc_mutex);
        return 1;
    }
    pthread_mutex_lock(&adec->rc_mutex);
    LOGD(0xe97, "playing");
    pthread_cond_signal(&adec->rc_cond);
    adec->rc_paused = 0;
    pthread_mutex_unlock(&adec->rc_mutex);

    if (adec->play_speed == value)
        return 0;
    adec->play_speed = value;
    if (adec->no_dec) {
        LOGI(0xea1, "no decoder (codec:%d). config and start(DEC.%d) will be called by handle_frame().",
             adec->codec_type, adec->index);
        return 0;
    }
    if (adec->codec_type != LGADEC_CODEC_AC3 && adec->codec_type != LGADEC_CODEC_EAC3)
        return 0;
    ret = lgadec_decoder_configure_decoder(adec);
    if (ret == LGADEC_RET_NOT_SUPPORTED) {
        LOGE(0xeac, "not supported codec");
        return LGADEC_RET_NOT_SUPPORTED;
    }
    if (ret == -1) {
        LOGE(0xeb0, "lgadec_decoder_configure_decoder error");
        return -1;
    }
    return 0;
}

/* @0x2f63c */
int lgadec_init_stream_info(void *a)
{
    lgadec *adec = a;
    LGADEC_STREAM_INFO_T *si;

    if (!adec) {
        LOGE(0xf12, "adec is null");
        return -1;
    }
    si = &adec->stream_info;
    adec->codec_type = LGADEC_CODEC_UNKNOWN;
    si->freq = 0;
    si->channels = 0;
    si->bits_per_sample = 0;
    si->avg_bytes_per_sec = 0;
    si->block_align = 0;
    si->byte_align = 0;
    si->sign = 0;
    if (si->dsi_buffer) {
        free(si->dsi_buffer);
        si->dsi_buffer = NULL;
    }
    si->dsi_buffer_length = 0;
    si->wma_format_tag = 0;
    si->object_type = 0;
    si->leaf_size = 0;
    LOGD(0xf28, "stream info init");
    return 0;
}

/* @0x2f6f8 */
int lgadec_set_freq(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xf33, "adec is null");
        return -1;
    }
    adec->stream_info.freq = value;
    LOGD(0xf39, "set stream_info->freq: %d", value);
    return 0;
}

/* @0x2f78c */
int lgadec_set_number_of_channel(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xf44, "adec is null");
        return -1;
    }
    adec->stream_info.channels = value;
    LOGD(0xf4a, "set stream_info->number_of_channel: %d", value);
    return 0;
}

/* @0x2f820 */
int lgadec_set_bits_per_sample(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xf55, "adec is null");
        return -1;
    }
    adec->stream_info.bits_per_sample = value;
    LOGD(0xf5b, "set stream_info->bits_per_sample: %d", value);
    return 0;
}

/* @0x2f8b4 */
int lgadec_set_avg_bytes_per_sec(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xf66, "adec is null");
        return -1;
    }
    adec->stream_info.avg_bytes_per_sec = value;
    LOGD(0xf6c, "set stream_info->avg_bytes_per_sec: %d", value);
    return 0;
}

/* @0x2f948 */
int lgadec_set_block_align(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xf77, "adec is null");
        return -1;
    }
    adec->stream_info.block_align = value;
    LOGD(0xf7d, "set stream_info->block_align: %d", value);
    return 0;
}

/* @0x2f9d8 */
int lgadec_set_endian_of_pcm(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xf88, "adec is null");
        return -1;
    }
    adec->stream_info.byte_align = value;
    LOGD(0xf8d, "set endian of pcm: %d", value);
    return 0;
}

/* @0x2fa6c */
int lgadec_set_signed_of_pcm(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xf98, "adec is null");
        return -1;
    }
    adec->stream_info.sign = value;
    LOGD(0xf9d, "set signed of pcm: %d", value);
    return 0;
}

/* @0x2fb00 */
int lgadec_set_DSI_buffer(void *a, void * value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xfa8, "adec is null");
        return -1;
    }
    adec->stream_info.dsi_buffer = value;
    LOGD(0xfae, "set stream_info->DSI_buffer: %p", value);
    return 0;
}

/* @0x2fb90 */
int lgadec_set_length_of_DSI(void *a, unsigned int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xfb9, "adec is null");
        return -1;
    }
    adec->stream_info.dsi_buffer_length = value;
    LOGD(0xfbf, "set stream_info->length_of_DSI: %u", value);
    return 0;
}

/* @0x2fc24 */
int lgadec_set_object_type(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0xfca, "adec is null");
        return -1;
    }
    adec->stream_info.object_type = (unsigned short)value;
    LOGD(0xfd0, "set stream_info->object_type: %d", value);
    return 0;
}

/* @0x2fcb8: clamps to 0..12 dB and to the DSP-reported limit */
int lgadec_set_ac4_dialog_enhancement_gain(void *a, unsigned int value)
{
    lgadec *adec = a;
    unsigned int limit = 0, gain;

    if (!adec) {
        LOGE(0xfdc, "adec is null");
        return -1;
    }
    adec->de_gain = value;
    if (value > 12) {
        LOGI(0xfe2, "Ac4 dialogue enhancement gain is too big: %u", value);
        lgadec_decoder_get_ac4_dialog_enhancement_limit(adec, &limit);
        gain = limit > 11 ? 12 : limit;
        LOGI(0xfed, "de_gain: %d -> %d\n", value, gain);
    } else {
        lgadec_decoder_get_ac4_dialog_enhancement_limit(adec, &limit);
        if (value > limit) {
            gain = limit;
            LOGI(0xfed, "de_gain: %d -> %d\n", value, gain);
        } else {
            gain = value;
            LOGI(0xfef, "de_gain: %d\n", value);
        }
    }
    if (lgadec_decoder_set_ac4_dialog_enhancement_gain(adec, gain) != 0)
        LOGE(0xff4, "Set Ac4 Dialog Enhancement Gain Err");
    else
        LOGD(0xff6, "Set Ac4 Dialog Enhancement Gain Failed");   /* sic: success path */
    return 0;
}

/* @0x2fe5c: MPEG-H default program reference level, -31..0; DSP gets -4*value */
int lgadec_set_default_prl(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x1001, "adec is null");
        return -1;
    }
    if ((unsigned int)(value + 31) >= 32) {
        LOGE(0x1004, "default prl is invalid value: %u", value);
        return 0;
    }
    adec->default_prl = value;
    LOGI(0x1007, "AAC dec. set default PRL : %d", value);
    if (lgadec_decoder_set_default_prl(adec, -4 * value) != 0)
        LOGE(0x100a, "Set Default PRL Err");
    else
        LOGD(0x100c, "Set Default PRL: %u", value);
    return 0;
}

/* @0x2ff84 */
int lgadec_set_presentation_group_index(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x1018, "adec is null");
        return -1;
    }
    adec->presentation_group_index = value;
    LOGI(0x101b, "presentation group index : %d", value);
    if (lgadec_decoder_set_presentation_group_index(adec, value) != 0)
        LOGE(0x101f, "Set Presentation Group Index Err");
    else
        LOGD(0x1021, "Set Presentation Group Index: %u", value);
    return 0;
}

/* @0x306b4 */
int lgadec_set_dolby_drc_mode(void *a, int value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x102c, "adec is null");
        return -1;
    }
    adec->dolby_drc_mode = value;
    LOGI(0x102f, "dolby drc mode : %d", value);
    if (lgadec_decoder_set_dolby_drc_mode(adec, value) != 0)
        LOGE(0x1032, "Set Dolby DRC Mode Err");
    else
        LOGD(0x1034, "Set Dolby DRC Mode: %u", value);
    return 0;
}

/* @0x309c8: segment start -> DSP basetime */
int lgadec_set_basetime(void *a, unsigned long long value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x1067, "adec is null");
        return -1;
    }
    adec->segment_start_time = value;
    LOGI(0x106a, "segment start time : %llu\n", value);
    if (lgadec_decoder_set_basetime(adec, value) != 0)
        LOGE(0x106d, "set basetime Err");
    else
        LOGD(0x106f, "set basetime : %llu", value);
    LOGI(0x1072, "set basetime\n");
    return 0;
}

/* @0x30af4 */
int lgadec_set_downmix(void *a, bool enable)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x107c, "adec is null");
        return -1;
    }
    adec->downmix = enable;
    LOGI(0x107f, "downmix : %s", enable ? "enable" : "disable");
    if (lgadec_decoder_set_downmix(adec, enable) != 0)
        LOGE(0x1082, "Set Downmix Err");
    else
        LOGD(0x1084, "Set Downmix: %s", enable ? "enable" : "disable");
    return 0;
}

/* @0x30c14: IAMF loudspeaker layout; no fd check */
int lgadec_set_sound_system(void *a, unsigned int sound_system)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x1090, "adec is null");
        return -1;
    }
    ioctl(adec->fd, ADEC_IOW_SET_SOUND_SYSTEM, &sound_system);
    LOGI(0x1094, "sound_system = %d", sound_system);
    adec->sound_system = sound_system;
    return 0;
}
