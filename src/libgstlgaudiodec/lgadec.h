/*
 * lgadec: the decoder-control layer under the lgaudiodec elements
 * (gst-plugins-bsp/gstlgaudio/lgaudio/lgadec.c + lgadec_codec.c, header lgadec.h).
 *
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so
 *   sha256 84db367e1ea3eeb4b589bf63910140cfd43ea77f2db124ae6882fbee8ebb6bb7
 * Every enum, struct layout and prototype below comes from the binary's DWARF
 * (CUs gstlgaudiodec.c / lgadec.c / lgadec_codec.c, producer GCC 11.4.0 -O2).
 * Offsets in the comments are the DWARF DW_AT_data_member_location values and are
 * enforced with _Static_assert in lgadec.c.
 */
#ifndef LGADEC_H
#define LGADEC_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <glib.h>

/* ---- public lgadec API types (gstlgaudiodec.h in the original tree) ------------ */
typedef enum {
    LGADEC_RET_NOT_SUPPORTED = -65535,
    LGADEC_RET_ERR = -1,
    LGADEC_RET_OK = 0,
    LGADEC_RET_NOK = 1,
} LGADEC_RET_T;

typedef enum {
    LGADEC_APP_TYPE_UNKNOWN, LGADEC_APP_TYPE_RTC, LGADEC_APP_TYPE_MIRACAST,
    LGADEC_APP_TYPE_JP4K, LGADEC_APP_TYPE_APPCAST, LGADEC_APP_TYPE_SYE, LGADEC_APP_TYPE_MAX,
} LGADEC_APP_TYPE_T;

/* No DTS member: lgaudiodec has no DTS decoder/bypass at all (see gstlgaudiodec.c). */
typedef enum {
    LGADEC_CODEC_UNKNOWN = 0, LGADEC_CODEC_PCM = 1, LGADEC_CODEC_AC3 = 2, LGADEC_CODEC_EAC3 = 3,
    LGADEC_CODEC_AAC = 4, LGADEC_CODEC_AAC_ELD = 5, LGADEC_CODEC_DRA = 6, LGADEC_CODEC_MP3 = 7,
    LGADEC_CODEC_MPEG_H = 8, LGADEC_CODEC_AC4 = 9, LGADEC_CODEC_WMA_PRO = 10,
    LGADEC_CODEC_VORBIS = 11, LGADEC_CODEC_AMR_WB = 12, LGADEC_CODEC_AMR_NB = 13,
    LGADEC_CODEC_ADPCM = 14, LGADEC_CODEC_RA8 = 15, LGADEC_CODEC_FLAC = 16,
    LGADEC_CODEC_OPUS = 17, LGADEC_CODEC_AAC_ENC = 18, LGADEC_CODEC_AC3_ENC = 19,
    LGADEC_CODEC_MP3_ENC = 20, LGADEC_CODEC_MAT = 21, LGADEC_CODEC_AAC_BYPASS = 22,
    LGADEC_CODEC_AC3_IEC60958 = 23, LGADEC_CODEC_EAC3_ATMOS = 24, LGADEC_CODEC_AC4_ATMOS = 25,
    LGADEC_CODEC_IAMF = 26, LGADEC_CODEC_TYPE_MAX = 27,
} LGADEC_CODEC_T;

/* Output type of one LGADEC_DATA_T handed to the element (callback PUSH_DATA). */
typedef enum {
    LGADEC_DATA_PCM = 0,        /* decoded PCM from the DSP appsink                  */
    LGADEC_DATA_ES = 1,         /* DSP pass-through ES (bypass_appsink, decoder port 1) */
    LGADEC_DATA_METADATA = 2,
    LGADEC_DATA_EVENT = 3,
    LGADEC_DATA_METADATA2 = 4,
    LGADEC_DATA_BYPASS = 5,     /* host-side "ES BYPASS & DROP": input ES, decoder closed */
    LGADEC_DATA_TYPE_NUM = 6,
} LGADEC_DATA_TYPE_T;

typedef enum {
    LGADEC_DECODER_STATE_INIT, LGADEC_DECODER_STATE_OPEN, LGADEC_DECODER_STATE_CONFIG,
    LGADEC_DECODER_STATE_CONNECT, LGADEC_DECODER_STATE_START, LGADEC_DECODER_STATE_STOP,
} LGADEC_DECODER_STATE_T;

typedef enum {
    LGADEC_EVENT_NONE = 0, LGADEC_EVENT_DSP_RESET = 1, LGADEC_EVENT_DEC_DECODED_INFO = 2,
    LGADEC_EVENT_UPDATE_DECODED_INFO = 3, LGADEC_EVENT_SEND_MEDIA_INFO = 4,
    LGADEC_EVENT_PUSH_DATA = 5, LGADEC_EVENT_EOS = 6, LGADEC_EVENT_DEC_MPEGH_UI = 7,
    LGADEC_EVENT_FLUSH_DONE = 8, LGADEC_EVENT_PREROLL = 9,
    LGADEC_EVENT_DIALOG_ENHANCEMENT_LIMIT_CHANGED = 10, LGADEC_EVENT_MAX = 11,
} LGADEC_EVENT_MSG_T;

typedef enum {
    LGADEC_GST_STATE_VOID_PENDING, LGADEC_GST_STATE_NULL, LGADEC_GST_STATE_READY,
    LGADEC_GST_STATE_PAUSED, LGADEC_GST_STATE_PLAYING,
} LGADEC_GST_STATE_T;

typedef enum {
    LGADEC_PCM_NONE, LGADEC_PCM_BASE, LGADEC_PCM_WIFI, LGADEC_PCM_DVD, LGADEC_PCM_BD,
    LGADEC_PCM_MULAW, LGADEC_PCM_ALAW,
} LGADEC_PCM_T;

typedef enum { LGADEC_DIRECTMEDIA_TOTAL_BUF, LGADEC_DIRECTMEDIA_USED_BUF } LGADEC_DIRECTMEDIA_BUF_T;
enum { LGADEC_DRC_LINE = 0, LGADEC_DRC_RF = 1 };

typedef enum {
    LGADEC_ADPCM_NONE, LGADEC_ADPCM_IMA_QT, LGADEC_ADPCM_IMA_WAV, LGADEC_ADPCM_IMA_DK3,
    LGADEC_ADPCM_IMA_DK4, LGADEC_ADPCM_IMA_WS, LGADEC_ADPCM_IMA_SMJPEG, LGADEC_ADPCM_MS,
    LGADEC_ADPCM_4XM, LGADEC_ADPCM_XA, LGADEC_ADPCM_ADX, LGADEC_ADPCM_EA, LGADEC_ADPCM_G726,
    LGADEC_ADPCM_CT, LGADEC_ADPCM_SWF, LGADEC_ADPCM_YAMAHA, LGADEC_ADPCM_SBPRO_4,
    LGADEC_ADPCM_SBPRO_3, LGADEC_ADPCM_SBPRO_2, LGADEC_ADPCM_THP, LGADEC_ADPCM_IMA_AMV,
    LGADEC_ADPCM_EA_R1, LGADEC_ADPCM_EA_R3, LGADEC_ADPCM_EA_R2, LGADEC_ADPCM_IMA_EA_SEAD,
    LGADEC_ADPCM_IMA_EA_EACS, LGADEC_ADPCM_EA_XAS, LGADEC_ADPCM_EA_MAXIS_XA,
    LGADEC_ADPCM_IMA_ISS, LGADEC_ADPCM_G722,
} LGADEC_ADPCM_T;

typedef enum {
    LGADEC_UNKNOWN_FORMAT, LGADEC_RAW_FORMAT, LGADEC_AAC_ADTS_FORMAT, LGADEC_AAC_LOAS_FORMAT,
    LGADEC_AAC_LATM_FORMAT, LGADEC_AC3_FORMAT, LGADEC_AC3_PRIVATE_FORMAT,
    LGADEC_AC4_SYNC_FRAME_FORMAT, LGADEC_WMA1_FORMAT, LGADEC_WMA2_FORMAT,
    LGADEC_WMA_PRO_FORMAT, LGADEC_WMA_LOSSLESS_FORMAT,
} LGADEC_CODEC_FORMAT_T;

typedef struct _LGADEC_AUDIO_CAPS_INFO {     /* 0x58 */
    int freq, channels, bits_per_sample, sign, endianness, acmod, check_downmix;
    bool dolby, atmos, lfe_enable;
    int channel_mode;
    bool clock_x4;
    int out_channels;
    bool restart;
    int gap_correction;
    bool es;                                 /* +0x34: first event came from the ES port */
    int data_channels;
    char codec_str[16];                      /* +0x3c */
    int data_rate, data_bps;
    bool downmix;
} LGADEC_AUDIO_CAPS_INFO;

typedef struct _LGADEC_CODEC_INFO_T {        /* 0x18 */
    LGADEC_CODEC_T codec_type;
    int codec_format;                        /* LGADEC_CODEC_FORMAT_T / LGADEC_PCM_T / ADPCM */
    int leaf_size;
    int disable_dsp;
    unsigned char *buffer;
    int buf_size;
} LGADEC_CODEC_INFO_T;

typedef int (*LGADEC_CALLBACK_FN)(void *data, LGADEC_EVENT_MSG_T msg, void *arg);

typedef struct _LGADEC_DATA_T {              /* 0x18, calloc'ed by lgadec, freed by the element */
    LGADEC_DATA_TYPE_T out_type;
    unsigned long long timestamp64;          /* +0x08 */
    unsigned int size;                       /* +0x10 */
    unsigned char *data;                     /* +0x14, NOT owned: points into aud_data / input */
} LGADEC_DATA_T;

/* ---- lgadec internals (lgadec.c) ------------------------------------------------ */
typedef enum {
    LGADEC_EOS_EVENT_NOT_RECEIVED, LGADEC_EOS_EVENT_RECEIVED,
    LGADEC_EOS_EVENT_RECEIVED_AND_PAUSED, LGADEC_EOS_EVENT_RECEIVED_AND_SENDED,
} LGADEC_EOS_EVENT_STATUS_T;
typedef enum { LGADEC_TASK_NONE, LGADEC_TASK_RUN, LGADEC_TASK_PAUSE, LGADEC_TASK_STOP } LGADEC_TASK_STATE_T;

typedef struct _LGADEC_ES_TIMESTAMP_NODE_T {
    struct _LGADEC_ES_TIMESTAMP_NODE_T *next, *prev;
    unsigned long long index, timestamp;
} LGADEC_ES_TIMESTAMP_NODE_T;
typedef struct _LGADEC_ES_TIMESTMP_T {
    LGADEC_ES_TIMESTAMP_NODE_T *head, *tail;
    unsigned int node_num;
    unsigned long long buffering_duration;
} LGADEC_ES_TIMESTMP_T;

typedef struct _LGADEC_STREAM_INFO_T {       /* 0x30 */
    int freq;
    unsigned char channels, bits_per_sample;
    unsigned int avg_bytes_per_sec;
    unsigned short block_align;
    unsigned int byte_align, sign;
    void *dsi_buffer;
    unsigned int dsi_buffer_length, wma_format_tag;
    unsigned short leaf_size;
    int adpcm_type, object_type;
} LGADEC_STREAM_INFO_T;

typedef struct _LGADEC_T lgadec;
typedef struct _SELECTED_CODEC_T {
    void (*init)(void *h);
    void (*cleanup)(void **h);
    int (*feed)(lgadec *adec, unsigned char *data, unsigned int len, unsigned long long ts);
    int (*flush)(lgadec *adec);
} SELECTED_CODEC_T;

struct aimc;
struct registry;
struct aud_connector;
struct aud_module;

struct _LGADEC_T {                           /* 0x3e0 */
    unsigned long magic_number;
    int fd;                                  /* +0x004 /dev/lg/adec0, -1 when closed      */
    int pipe[2];                             /* +0x008 task control: "stop", "SUB0", "SUB1" */
    int preroll_pipe[2];                     /* +0x010 "preroll"                          */
    LGADEC_CODEC_T codec_type;               /* +0x018 */
    LGADEC_AUDIO_CAPS_INFO audio_caps;       /* +0x01c last decoded info                  */
    int flushing;                            /* +0x074 */
    LGADEC_APP_TYPE_T app_type;
    pthread_mutex_t ts_mutex;                /* +0x07c */
    pthread_mutex_t task_mutex;              /* +0x094 */
    pthread_mutex_t pipe_mutex;              /* +0x0ac */
    pthread_cond_t pipe_cond;                /* +0x0c8 */
    pthread_mutex_t rc_mutex;                /* +0x0f8 */
    pthread_cond_t rc_cond;                  /* +0x110 */
    int rc_paused;                           /* +0x140 */
    pthread_mutex_t flush_event_mutex;       /* +0x144 */
    pthread_cond_t flush_event_cond;         /* +0x160 */
    int flush_done;                          /* +0x190 */
    pthread_mutex_t flush_mutex;             /* +0x194 */
    LGADEC_CALLBACK_FN callback_fn;          /* +0x1ac */
    void *callback_data;                     /* +0x1b0 GstLgAudioDec *                    */
    unsigned long long cur_decoded_index[LGADEC_DATA_TYPE_NUM];
    unsigned long long decoded_frame_cnt[LGADEC_DATA_TYPE_NUM];  /* +0x1e8 */
    unsigned long long fed_frame_cnt;        /* +0x218 */
    bool get_decoded_info;
    pthread_t task;                          /* +0x224 */
    LGADEC_TASK_STATE_T task_status;         /* +0x228 */
    unsigned long long segment_start_time;   /* +0x230 */
    unsigned long long prev_timestamp[LGADEC_DATA_TYPE_NUM];     /* +0x238, [BYPASS] = +0x260 */
    LGADEC_ES_TIMESTMP_T *timestamp[LGADEC_DATA_TYPE_NUM];
    unsigned long long buffering_duration[LGADEC_DATA_TYPE_NUM]; /* +0x280 */
    LGADEC_GST_STATE_T next_state;           /* +0x2b0 */
    LGADEC_EOS_EVENT_STATUS_T eos_event_status; /* +0x2b4 */
    int play_speed;                          /* +0x2b8 */
    int seamless;
    SELECTED_CODEC_T *selected_codec;        /* +0x2c0 */
    void *codec_data;                        /* +0x2c4 */
    int no_dec;                              /* +0x2c8 PCM/ADPCM: data never reaches the DSP */
    LGADEC_PCM_T pcm_type;
    LGADEC_STREAM_INFO_T stream_info;        /* +0x2d0 */
    LGADEC_DECODER_STATE_T decoder_state;    /* +0x300 */
    unsigned int feeded_size;                /* +0x304 */
    unsigned int audio_mode;                 /* +0x308 */
    bool audio_mode_changed;
    int dec_req_feed;
    int index;                               /* +0x314 ADEC port                          */
    unsigned int output_mode;                /* +0x318 */
    int is_disable_reform;
    unsigned int presentation_group_index;   /* +0x320 */
    unsigned int de_gain, de_limit;
    int default_prl;                         /* +0x32c */
    int dolby_drc_mode;                      /* +0x330 */
    bool downmix;
    unsigned int sound_system;               /* +0x338 */
    bool is_dump_enable;                     /* +0x33c */
    FILE *dump_fd;
    struct aimc *imc;                        /* +0x344 */
    struct registry *reg;
    struct aud_connector *con;
    int uid_appsrc, uid_appsink;             /* +0x350 */
    struct aud_module *appsrc;               /* +0x358 */
    struct aud_module *appsink;              /* +0x35c decoder port 0 (PCM/metadata)      */
    int uid_bypass_appsink;                  /* +0x360 */
    struct aud_module *bypass_appsink;       /* +0x364 decoder port 1 (pass-through ES)   */
    bool bypass_decoder;                     /* +0x368 == "es-bypass" ("ES BYPASS & DROP") */
    bool inited;                             /* +0x369 set by lgadec_start_init, never cleared */
    int use_decoder_feed;                    /* +0x36c in-flight lgadec_decoder_feed()    */
    pthread_mutex_t resource_mutex;          /* +0x370 */
    pthread_cond_t resource_cond;            /* +0x388 */
    pthread_mutex_t appsink_mutex;           /* +0x3b8 */
    bool wait_bypass_first_event;            /* +0x3d0 codec also produces DSP pass-through ES */
    bool first_event_done[LGADEC_DATA_TYPE_NUM]; /* +0x3d1 [PCM], +0x3d2 [ES] */
    bool caps_is_updated;                    /* +0x3d7 */
    bool caps_is_sent;                       /* +0x3d8 */
    GList *list_pending_data;                /* +0x3dc data that arrived before the first caps */
};

/* ---- lgadec API (all take the opaque handle returned by lgadec_open) ---------- */
void *lgadec_open(void);
int lgadec_close(void *a);
int lgadec_start_init(void *a);
int lgadec_start(void *a);
int lgadec_stop(void *a);
int lgadec_flush(void *a);
int lgadec_feed_eos(void *a);
int lgadec_feed_data(void *a, unsigned char *data, unsigned int len, unsigned long long timestamp);
int lgadec_select_codec(void *a, LGADEC_CODEC_INFO_T *codec_info);
int lgadec_register_callback_fn(void *a, LGADEC_CALLBACK_FN callback_fn, void *data);
int lgadec_log_ts(void *a, unsigned long long timestamp, unsigned int len);
const char *lgadec_get_codec_name(LGADEC_CODEC_T codec);
const char *lgadec_get_event_name(LGADEC_EVENT_MSG_T event);

int lgadec_decoder_open(void *a);
int lgadec_decoder_close(void *a);
int lgadec_decoder_start(void *a);
int lgadec_decoder_stop(void *a);
int lgadec_decoder_connect(void *a);
int lgadec_decoder_disconnect(void *a);
int lgadec_decoder_configure_decoder(void *a);
int lgadec_decoder_set_codec_param(void *a);
int lgadec_decoder_set_audio_output_mode(void *a, unsigned int mode);
int lgadec_decoder_feed(void *a, void *data, unsigned int length, unsigned long long timestamp);
int lgadec_decoder_set_ui_param(void *a, const char *ui_param, unsigned int length);
int lgadec_decoder_get_ui_param(void *a, char **data);
int lgadec_decoder_set_gap_info(void *a, unsigned long long start, unsigned long long end);
int lgadec_decoder_get_cpb_status(void *a, unsigned long long *pBufferingSize,
                                  unsigned int *pUsedAuiCount, unsigned int *pBufferSize,
                                  unsigned int *pBufferFreeSize);
int lgadec_decoder_set_dolby_drc_mode(void *a, unsigned int drc_mode);
int lgadec_decoder_set_basetime(void *a, unsigned long long start);
int lgadec_decoder_set_presentation_group_index(void *a, unsigned int value);
int lgadec_decoder_set_default_prl(void *a, unsigned int value);
int lgadec_decoder_set_aac_format_type(void *a, unsigned int format_type);
int lgadec_decoder_set_ac4_dialog_enhancement_gain(void *a, unsigned int gain);
int lgadec_decoder_get_ac4_dialog_enhancement_gain(void *a, unsigned int *gain);
int lgadec_decoder_get_ac4_dialog_enhancement_limit(void *a, unsigned int *limit);
int lgadec_decoder_set_downmix(void *a, bool enable);
int lgadec_decoder_send_serialized_event(void *a);
int lgadec_aimc_connect(lgadec *adec);
int lgadec_aimc_disconnect(lgadec *adec);

int lgadec_set_es_bypass(void *a, bool on);
int lgadec_get_es_bypass(void *a);
int lgadec_set_bypass_decoder(void *a, bool on);
int lgadec_set_preroll(void *a);
int lgadec_set_flushing(void *a, int value);
int lgadec_set_next_state(void *a, LGADEC_GST_STATE_T value);
int lgadec_set_app_type(void *a, LGADEC_APP_TYPE_T value);
int lgadec_set_play_speed(void *a, int value);
int lgadec_set_seamless(void *a, int value);
int lgadec_set_output_mode(void *a, unsigned int output_mode);
int lgadec_set_index(void *a, unsigned int value);
int lgadec_set_prev_timestamp(void *a, LGADEC_DATA_TYPE_T out_type, unsigned long long value);
int lgadec_init_stream_info(void *a);
int lgadec_set_freq(void *a, int value);
int lgadec_set_number_of_channel(void *a, int value);
int lgadec_set_bits_per_sample(void *a, int value);
int lgadec_set_avg_bytes_per_sec(void *a, int value);
int lgadec_set_block_align(void *a, int value);
int lgadec_set_endian_of_pcm(void *a, int value);
int lgadec_set_signed_of_pcm(void *a, int value);
int lgadec_set_DSI_buffer(void *a, void *value);
int lgadec_set_length_of_DSI(void *a, unsigned int value);
int lgadec_set_object_type(void *a, int value);
int lgadec_set_ac4_dialog_enhancement_gain(void *a, unsigned int value);
int lgadec_set_default_prl(void *a, int value);
int lgadec_set_presentation_group_index(void *a, int value);
int lgadec_set_dolby_drc_mode(void *a, int value);
int lgadec_set_basetime(void *a, unsigned long long value);
int lgadec_set_downmix(void *a, bool enable);
int lgadec_set_sound_system(void *a, unsigned int sound_system);

int lgadec_get_pcm_type(void *a, LGADEC_PCM_T *value);
int lgadec_get_no_decoder(void *a, int *value);
int lgadec_get_prev_timestamp(void *a, LGADEC_DATA_TYPE_T out_type, unsigned long long *value);
int lgadec_get_flushing(void *a, int *value);
int lgadec_get_codec_type(void *a, LGADEC_CODEC_T *value);
int lgadec_get_play_speed(void *a, int *value);
int lgadec_get_output_mode(void *a, unsigned int *value);
int lgadec_get_decoder_state(void *a, LGADEC_DECODER_STATE_T *value);
int lgadec_get_bits_per_sample(void *a, unsigned char *value);
int lgadec_get_number_of_channel(void *a, unsigned char *value);
int lgadec_get_freq(void *a, int *value);
int lgadec_get_audio_caps_channels(void *a, int *value);
int lgadec_get_audio_caps_bits_per_sample(void *a, int *value);
int lgadec_get_audio_caps_freq(void *a, int *value);
int lgadec_get_audio_caps_sign(void *a, int *value);
int lgadec_get_audio_caps_endianness(void *a, int *value);
int lgadec_get_audio_caps_acmod(void *a, int *value);
int lgadec_get_atmos(void *a, int *value);
int lgadec_get_block_align(void *a, unsigned int *value);
unsigned int lgadec_get_sound_system(void *a, unsigned int *value);
int lgadec_get_audio_mode(void *a, int *value, unsigned int *changed);
int lgadec_get_byte_per_sec(void *a, int *bytes_per_sec);
int lgadec_get_ac4_dialog_enhancement_gain(void *a, unsigned int *value);
int lgadec_get_ac4_dialog_enhancement_limit_min(void *a, unsigned int *value);
int lgadec_get_ac4_dialog_enhancement_limit_max(void *a, unsigned int *value);
int lgadec_get_buffer_size_for_directmedia(void *a, unsigned int *size, LGADEC_DIRECTMEDIA_BUF_T buf);
int lgadec_get_dolby_drc_mode(void *a, unsigned int *value);
unsigned int lgadec_get_out_channels(void *a, unsigned int *value);

/* lgadec_codec.c */
int lgadec_adpcm_data_length_check(void *a, unsigned char *data, unsigned int *len);
int lgadec_adpcm_resampling(void *a, unsigned char *data, unsigned int len, short **sample);
int lgadec_pcm_get_width(void *a, uint32_t *value);
int lgadec_pcm_get_payload_size(void *a, uint32_t *value);
int lgadec_pcm_get_timestamp(void *a, uint64_t *value);
int lgadec_pcm_set_timestamp(void *a, uint64_t value);
int lgadec_dvdlpcm_header_check(void *a, unsigned char *data, int *new_header);
int lgadec_dvdlpcm_resampling(void *a, unsigned char *data, unsigned int data_len,
                              unsigned long long timestamp, unsigned char *dest_buf);
int lgadec_bdlpcm_header_check(void *a, unsigned char *data, unsigned int rd_offset, int *header_flag);
int lgadec_wifilpcm_header_check(void *a, unsigned char *data, unsigned int rd_offset, int *header_flag);
int lgadec_bd_wifi_lpcm_resampling(void *a, unsigned char *data, unsigned int data_len,
                                   unsigned char *out_data);
int lgadec_mulaw_resampling(void *a, unsigned char *data, unsigned int data_len, short *pcm_data);
int lgadec_alaw_resampling(void *a, unsigned char *data, unsigned int data_len, short *pcm_data);

/* ---- liblogm ------------------------------------------------------------------ */
struct logmm_module {
    int index;
    unsigned int flags;
    unsigned int _mask;
    unsigned int *mask;
    const char *name;
};
enum log_level { log_level_error, log_level_warning, log_level_noti, log_level_info,
                 log_level_debug, log_level_trace };
extern unsigned int logmm_dummy_mask;
extern int logmm_printl(struct logmm_module *m, enum log_level lvl, const char *func, int line,
                        const char *fmt, ...);

/* line = __LINE__ of the original source, kept as evidence */
#define LOGM(mod, lvl, line, ...) do { \
        if (*(mod).mask & (1u << (lvl))) \
            logmm_printl(&(mod), (lvl), __func__, (line), __VA_ARGS__); \
    } while (0)

#endif /* LGADEC_H */
