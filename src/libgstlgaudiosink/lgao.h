/*
 * lgao: LG audio-output helper used by the lgaudiosink element
 * (gst-plugins-bsp/gstlgaudio/lgaudio/lgao.{c,h}).
 * RECONSTRUCTION from usr/lib/gstreamer-1.0/libgstlgaudiosink.so
 *   sha256 90dba3ef32704ce4096bf1e6045d2879d0c2192d507bc04d1e8e98437f420a43
 * Type, enum and field names come from the binary's DWARF (lgao.c CU);
 * offsets are checked with _Static_assert below. Plain C types only, so the
 * header compiles with or without glib.
 */
#ifndef LGAO_H
#define LGAO_H

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    LGAO_DATA_PCM = 0,
    LGAO_DATA_ES,
    LGAO_DATA_METADATA,
    LGAO_DATA_EVENT,
    LGAO_DATA_METADATA2,
    LGAO_DATA_TYPE_NUM,
} LGAO_DATA_TYPE_T;

/* Posted to the event message queue (lgao_get_event_mq_id). */
typedef enum {
    LGAO_EVENT_MASK_NONE              = 0,
    LGAO_EVENT_DSP_RESET              = 1 << 0,
    LGAO_EVENT_PRESENT_END            = 1 << 1,
    LGAO_EVENT_UNDERFLOW              = 1 << 2,
    LGAO_EVENT_DEC_ERROR              = 1 << 3,
    LGAO_EVENT_DEC_AC4_DUAL           = 1 << 4,
    LGAO_EVENT_OUTPUT_CONNECT_CHANGED = 1 << 6,
    LGAO_EVENT_SPDIF_CHANGED          = 1 << 7,
    LGAO_EVENT_1KHZ_TONE_ON           = 1 << 8,
    LGAO_EVENT_1KHZ_TONE_OFF          = 1 << 9,
    LGAO_EVENT_DEC_DECODED_INFO       = 1 << 10,
    LGAO_EVENT_DECODED_END            = 1 << 11,
    LGAO_EVENT_PRESENT_START          = 1 << 12,
    LGAO_EVENT_DSP_HANG_UP            = 1 << 13,
    LGAO_EVENT_DEC_AAC51_DUAL         = 1 << 14,
    LGAO_EVENT_CLOSE_IPC              = 1 << 15,
    LGAO_EVENT_DELAY_CHANGED          = 1 << 16,
    LGAO_EVENT_FLUSH_DONE             = 1 << 17,
    LGAO_EVENT_BYPASS_DONE            = 1 << 18,
    LGAO_EVENT_MASK_ALL               = 0xfffff,
} LGAO_EVENT_MSG_T;

typedef enum { LGAO_MSG_TYPE_EVENT = 1, LGAO_MSG_TYPE_MAX } LGAO_MSG_TYPE_T;

/* SysV message: msgsnd(mq, &item, sizeof(item.event), 0). */
typedef struct lgao_mq_item {
    long             mtype;   /* always LGAO_MSG_TYPE_EVENT */
    LGAO_EVENT_MSG_T event;
} lgao_mq_item;

typedef enum {
    LGAO_PLAYRATE_0_00X, LGAO_PLAYRATE_0_25X, LGAO_PLAYRATE_0_50X, LGAO_PLAYRATE_0_80X,
    LGAO_PLAYRATE_1_00X, LGAO_PLAYRATE_1_20X, LGAO_PLAYRATE_1_25X, LGAO_PLAYRATE_1_50X,
    LGAO_PLAYRATE_2_00X, LGAO_PLAYRATE_4_00X, LGAO_PLAYRATE_8_00X, LGAO_PLAYRATE_16_00X,
    LGAO_PLAYRATE_32_00X,
} LGAO_PLAYRATE_T;

typedef enum {
    LGAO_LIPSYNC_AUTO = 0,
    LGAO_LIPSYNC_ENABLE,
    LGAO_LIPSYNC_DISABLE,
} LGAO_LIPSYNC_MODE_T;

typedef enum {
    LGAO_RTS_OP_MODE = 0,
    LGAO_RTS_THRESHOLD_SKIPPING,
    LGAO_RTS_THRESHOLD_FAST,
    LGAO_RTS_THRESHOLD_RECOVER,
    LGAO_RTS_FREQ_FAST,
    LGAO_RTS_BUFFERING_LEVEL,
    LGAO_RTS_PARAM_MAX,
} LGAO_RTS_PARAM;

typedef enum {
    LGAO_APPTYPE_UNKNOWN = 0,
    LGAO_APPTYPE_DIRECTMEDIA,
    LGAO_APPTYPE_RTC,
    LGAO_APPTYPE_MIRACAST,
    LGAO_APPTYPE_AIRPLAY,
    LGAO_APPTYPE_JP4K,
    LGAO_APPTYPE_IDGAPLESS,
    LGAO_APPTYPE_APPCAST,
    LGAO_APPTYPE_R2R,
    LGAO_APPTYPE_SYE,
    LGAO_APPTYPE_MAX,
} LGAO_APPTYPE_T;

typedef enum {
    LGAO_EASE_NONE = 0,
    LGAO_EASE_LINEAR,
    LGAO_EASE_INCUBIC,
    LGAO_EASE_OUTCUBIC,
    LGAO_EASE_MAX,
} LGAO_EASE_TYPE_T;

typedef enum {
    LGAO_FADE_PROP_TARGET_VOL = 0,
    LGAO_FADE_PROP_DURATION,
    LGAO_FADE_PROP_REMAIN_DURATION,
    LGAO_FADE_PROP_TYPE,
    LGAO_FADE_PROP_EASE_VOL,
    LGAO_FADE_PROP_MAX,
} LGAO_FADE_PROP_T;

typedef enum {
    LGAO_CLOCK_TYPE_GSTC = 0,
    LGAO_CLOCK_TYPE_ATSC3,
} LGAO_CLOCK_TYPE_T;

typedef enum {
    LGAO_STATE_INIT = 0,
    LGAO_STATE_CONFIG,
    LGAO_STATE_PLAY,
    LGAO_STATE_PAUSE,
    LGAO_STATE_STOP,
} LGAO_STATE_T;

typedef enum {
    LGAO_DIRECTMEDIA_TOTAL_BUF = 0,
    LGAO_DIRECTMEDIA_USED_BUF,
} LGAO_DIRECTMEDIA_BUF_T;

typedef enum {
    LGAO_AUDIO_INFO_CODEC = 0,
    LGAO_AUDIO_INFO_SAMPLING_FREQ,
    LGAO_AUDIO_INFO_NUM_CH,
    LGAO_AUDIO_INFO_BITS_PER_SAMPLE,
    LGAO_AUDIO_INFO_ENDIAN,
    LGAO_AUDIO_INFO_SIGNED,
    LGAO_AUDIO_INFO_ACMOD,
    LGAO_AUDIO_INFO_LFE_ENABLE,
    LGAO_AUDIO_INFO_DOLBY,
    LGAO_AUDIO_INFO_FULL,
    LGAO_AUDIO_INFO_DATA_PER_SEC,
    LGAO_AUDIO_INFO_CLOCK_X4,
    LGAO_AUDIO_INFO_OUT_CHANNELS,
    LGAO_AUDIO_INFO_RESTART,
    LGAO_AUDIO_INFO_DOWNMIX,
    LGAO_AUDIO_INFO_MAX,
} LGAO_AUDIO_INFO_T;

typedef enum {
    LGAO_CODEC_UNKNOWN = 0,
    LGAO_CODEC_PCM,
    LGAO_CODEC_AC3,
    LGAO_CODEC_EAC3,
    LGAO_CODEC_AAC,
    LGAO_CODEC_AAC_ELD,
    LGAO_CODEC_DRA,
    LGAO_CODEC_MP3,
    LGAO_CODEC_MPEG_H,
    LGAO_CODEC_AC4,
    LGAO_CODEC_WMA_PRO,
    LGAO_CODEC_VORBIS,
    LGAO_CODEC_AMR_WB,
    LGAO_CODEC_AMR_NB,
    LGAO_CODEC_ADPCM,
    LGAO_CODEC_RA8,
    LGAO_CODEC_FLAC,
    LGAO_CODEC_OPUS,
    LGAO_CODEC_AAC_ENC,
    LGAO_CODEC_AC3_ENC,
    LGAO_CODEC_MP3_ENC,
    LGAO_CODEC_MAT,
    LGAO_CODEC_AAC_BYPASS,
    LGAO_CODEC_AC3_IEC60958,
    LGAO_CODEC_EAC3_ATMOS,
    LGAO_CODEC_AC4_ATMOS,
    LGAO_CODEC_MAX,
} LGAO_CODEC_T;

typedef enum {
    LGAO_SAMPLING_FREQ_NONE      = 0,
    LGAO_SAMPLING_FREQ_4_KHZ     = 4000,
    LGAO_SAMPLING_FREQ_8_KHZ     = 8000,
    LGAO_SAMPLING_FREQ_11_025KHZ = 11025,
    LGAO_SAMPLING_FREQ_12_KHZ    = 12000,
    LGAO_SAMPLING_FREQ_16_KHZ    = 16000,
    LGAO_SAMPLING_FREQ_22_05KHZ  = 22050,
    LGAO_SAMPLING_FREQ_24_KHZ    = 24000,
    LGAO_SAMPLING_FREQ_32_KHZ    = 32000,
    LGAO_SAMPLING_FREQ_44_1KHZ   = 44100,
    LGAO_SAMPLING_FREQ_48_KHZ    = 48000,
    LGAO_SAMPLING_FREQ_64_KHZ    = 64000,
    LGAO_SAMPLING_FREQ_88_2KHZ   = 88200,
    LGAO_SAMPLING_FREQ_96_KHZ    = 96000,
    LGAO_SAMPLING_FREQ_128_KHZ   = 128000,
    LGAO_SAMPLING_FREQ_176_4KHZ  = 176400,
    LGAO_SAMPLING_FREQ_192_KHZ   = 192000,
    LGAO_SAMPLING_FREQ_768_KHZ   = 768000,
    LGAO_SAMPLING_FREQ_DEFAULT   = 999000,
} LGAO_SAMPLING_FREQ_T;

typedef struct _LGAO_AUDIO_START_INFO_T {
    LGAO_CODEC_T         codecType;
    LGAO_SAMPLING_FREQ_T freq;
    unsigned char        numberOfChannel;
    unsigned char        bitsPerSample;
    unsigned int         bigEndianOfPcm;
    unsigned int         unsignedOfPcm;
    unsigned int         acmod;
    unsigned int         check_downmix;
    unsigned int         lfe_enable;
    unsigned int         dolby;
    unsigned int         clock_x4;
    unsigned int         atmos;
    int                  out_channels;
    unsigned int         restart;
    unsigned int         gap_correction;
    unsigned int         es;            /* caps es=true: IEC 61937 passthrough */
    char                 codec_str[16];
    int                  data_bps;
    int                  data_channels;
    int                  data_rate;
    unsigned int         downmix;
} LGAO_AUDIO_START_INFO_T;

typedef enum {
    LGAO_LIPSYNC_OFFSET_MODE_UNKNOWN     = 0,
    LGAO_LIPSYNC_OFFSET_MODE_PCM         = 1,
    LGAO_LIPSYNC_OFFSET_MODE_WIRELESS    = 2,
    LGAO_LIPSYNC_OFFSET_MODE_DD_DDP      = 3,
    LGAO_LIPSYNC_OFFSET_MODE_ATMOS       = 4,
    LGAO_LIPSYNC_OFFSET_MODE_WISA        = 6,
    LGAO_LIPSYNC_OFFSET_MODE_ES          = 7,
    LGAO_LIPSYNC_OFFSET_MODE_AAC         = 8,
    LGAO_LIPSYNC_OFFSET_MODE_BT_SURROUND = 9,
} LGAO_LIPSYNC_OFFSET_MODE_T;

typedef enum {
    LGAO_IEC_OUTPUT_PCM  = 0,
    LGAO_IEC_OUTPUT_ES   = 1,
    LGAO_IEC_OUTPUT_DD   = 2,
    LGAO_IEC_OUTPUT_DDP  = 3,
    LGAO_IEC_OUTPUT_AAC  = 5,
    LGAO_IEC_OUTPUT_MAT  = 6,
    LGAO_IEC_OUTPUT_NONE = 7,
} LGAO_IEC_OUTPUT_TYPE_T;

/* /dev/lg/aren* AREN_SET_RTS_PARAM payload (56 bytes). */
typedef struct {
    bool         onoff;
    unsigned int opmode;
    unsigned int upper_threshold;
    unsigned int lower_threshold;
    unsigned int recover_upper;
    unsigned int recover_lower;
    unsigned int skipping_threshold;
    unsigned int recover_skipping;
    unsigned int fast_freq;
    unsigned int normal_freq;
    unsigned int slow_freq;
    unsigned int mix_fade_len;
    unsigned int mix_wait_len;
    unsigned int buffering_level;
} aren_rts_param_t;

enum aud_clock_type {
    AUD_CLK_TYPE_PCR_M  = 0x001,
    AUD_CLK_TYPE_PCR_A  = 0x002,
    AUD_CLK_TYPE_NO_PCR = 0x011,
    AUD_CLK_TYPE_GSTC   = 0x100,
    AUD_CLK_TYPE_GSTC_1 = 0x101,
    AUD_CLK_TYPE_MASTER = 0x200,
};

struct aimc;
struct registry;
struct aud_connector;
struct aud_module;

/* Private handle behind every "void *a" below (lgao_open: 0x1f0 bytes on ARM).
 * Shipped sink dynsym exports no handle/appsrc/UID accessor. GstLgAudioRender's
 * private ao is at +0x200 in that build, not a portable GStreamer API. These
 * offsets describe the identified ARM binary, not a supported injection ABI.
 * Registry module pointers below are borrowed, not aud_object references;
 * disconnect destroys their UIDs and clears them. No context lifetime lock is
 * held by feed/FIRST/connect/disconnect: producers must be quiesced externally.
 * flush_mutex coordinates flush notifications only; it does not pin modules.
 * PRIVATE-HOOK DECISION: no existing supported acquisition+submit+release API.
 * A sink ref, pad stream lock, registry lookup or callback with priv=ao is not a
 * module lease. Offset-only/late-attach injection is unacceptable. A private
 * hook is only a candidate if explicitly authorized, exact loaded binaries and
 * ARM ABI/entry bindings are gated, and association/lifecycle interception is
 * installed BEFORE use with generation/in-flight accounting and cancellation.
 * A validated render scope can capture lgao_feed's real ao argument per sink;
 * it must not guess the context from global UIDs or keep it across a rebuild.
 * Dynamic JUMP_SLOT entries for lgao_feed/open/close/connect/disconnect/drop
 * exist in this build (BIND_NOW); that is a hook surface, not proof LD_PRELOAD
 * or late symbol replacement intercepts the already-resolved internal calls.
 * No hook, lifetime gate or public capability is implemented by this model. */
typedef struct {
    int                     fd;              /* /dev/lg/aren0, -1 when closed */
    bool                    isMixer;
    int                     index;           /* audio port; renderer = index (+4 if mixer) */
    bool                    isMute;
    unsigned int            targetVolume;
    unsigned int            duration;
    LGAO_EASE_TYPE_T        fadeType;
    unsigned int            currVolume;
    unsigned int            remainDuration;
    LGAO_AUDIO_START_INFO_T audioStartInfo;  /* +0x24 */
    LGAO_STATE_T            rendererState;   /* +0x80 */
    unsigned int            bufferedSize[LGAO_DATA_TYPE_NUM];
    bool                    isFirstRender[LGAO_DATA_TYPE_NUM];
    LGAO_LIPSYNC_MODE_T     lipsyncMode;
    int                     delay;
    unsigned int            lipsyncOffset;
    unsigned long long      prevTimestamp[LGAO_DATA_TYPE_NUM];
    int                     playSpeed;       /* +0xd8, 1000 = 1.0x */
    unsigned long long      streamBasetime;
    unsigned long long      clockBasetime;
    bool                    isNoDelayMode;
    aren_rts_param_t        rts_params;      /* +0xf4 */
    LGAO_APPTYPE_T          appType;
    bool                    isMainDecoder;
    bool                    isEnableDump;
    FILE                   *pDumpAudioFD;
    void                   *pFnCallbackFn;
    void                   *pCallbackFnParam;
    enum aud_clock_type     clock_type;      /* +0x140 */
    LGAO_LIPSYNC_OFFSET_MODE_T lipsyncOffsetMode;
    LGAO_IEC_OUTPUT_TYPE_T  spdifStatus;
    LGAO_IEC_OUTPUT_TYPE_T  arcStatus;
    pthread_t               render_task;
    int                     render_task_id;
    int                     render_task_running;
    int                     render_task_join;
    pthread_mutex_t         flush_mutex;     /* +0x160 */
    pthread_cond_t          flush_cond;
    int                     flush_done[2];
    int                     callback_mq_id;  /* +0x1b0 */
    struct aimc            *imc;
    struct registry        *reg;
    struct aud_connector   *con;
    unsigned int            uid_appsrc;      /* +0x1c0: "appsrc" -> renderer head */
    struct aud_module      *appsrc;
    unsigned int            uid_appsink;     /* appsink <- renderer tail port 1 */
    struct aud_module      *appsink;
    bool                    pcm_bypass;      /* +0x1d0 */
    bool                    pcm_multi_ch;
    unsigned int            uid_bypass_appsrc; /* +0x1d4: 2nd "appsrc" -> bypass head */
    struct aud_module      *bypass_appsrc;
    bool                    game_mode;       /* +0x1dc */
    bool                    vsp_mode;
    int                     drop_renderer;   /* +0x1e0 */
    LGAO_STATE_T            prev_state;
    unsigned int            queue_max_length;
} lgao;

#if defined(__arm__) && !defined(__aarch64__)
_Static_assert(sizeof(lgao) == 0x1f0, "lgao size");
_Static_assert(__builtin_offsetof(lgao, rendererState) == 0x80, "rendererState");
_Static_assert(__builtin_offsetof(lgao, playSpeed) == 0xd8, "playSpeed");
_Static_assert(__builtin_offsetof(lgao, clock_type) == 0x140, "clock_type");
_Static_assert(__builtin_offsetof(lgao, callback_mq_id) == 0x1b0, "callback_mq_id");
_Static_assert(__builtin_offsetof(lgao, isFirstRender) == 0x98, "isFirstRender");
_Static_assert(__builtin_offsetof(lgao, uid_appsrc) == 0x1c0, "uid_appsrc");
_Static_assert(__builtin_offsetof(lgao, appsrc) == 0x1c4, "appsrc");
_Static_assert(__builtin_offsetof(lgao, pcm_bypass) == 0x1d0, "pcm_bypass");
_Static_assert(__builtin_offsetof(lgao, pcm_multi_ch) == 0x1d1, "pcm_multi_ch");
_Static_assert(__builtin_offsetof(lgao, uid_bypass_appsrc) == 0x1d4, "uid_bypass_appsrc");
_Static_assert(__builtin_offsetof(lgao, bypass_appsrc) == 0x1d8, "bypass_appsrc");
_Static_assert(__builtin_offsetof(lgao, drop_renderer) == 0x1e0, "drop_renderer");
#endif

void *lgao_open(void);
int   lgao_close(void *a);
int   lgao_get_event_mq_id(void *a);
int   lgao_eos(void *a);

int lgao_set_playrate(void *a, int rate);
int lgao_get_playrate(void *a, int *rate);
int lgao_set_lipsync_mode(void *a, int mode);
int lgao_get_lipsync_mode(void *a, int *mode);
int lgao_set_lipsync_offset(void *a, unsigned int offset);
int lgao_get_lipsync_offset(void *a, unsigned int *offset);
int lgao_set_mode_mixer(void *a, int mode);
int lgao_get_mode_mixer(void *a, int *mode);
int lgao_set_nodelay(void *a, int mode);
int lgao_get_nodelay(void *a, int *mode);
int lgao_set_rts(void *a, LGAO_RTS_PARAM param, unsigned int value);
int lgao_get_rts(void *a, LGAO_RTS_PARAM param, unsigned int *value);
int lgao_set_apptype(void *a, LGAO_APPTYPE_T apptype);
int lgao_get_apptype(void *a, LGAO_APPTYPE_T *apptype);
int lgao_set_port(void *a, int audioPort, int mixerPort);
int lgao_get_port(void *a, int *port);
int lgao_set_mute(void *a, int mode);
int lgao_get_mute(void *a, int *mode);
int lgao_set_fade(void *a, LGAO_FADE_PROP_T prop, int value);
int lgao_get_fade(void *a, LGAO_FADE_PROP_T prop, int *value);
int lgao_keep_fade(void *a);
int lgao_set_clocktype(void *a, LGAO_CLOCK_TYPE_T type);
int lgao_set_volume(void *a, int vol);
int lgao_get_volume(void *a, int *vol);
int lgao_set_state(void *a, LGAO_STATE_T state);
int lgao_get_state(void *a, LGAO_STATE_T *state);
int lgao_get_buffer_status(void *a, unsigned long long *undecodedsize);
int lgao_get_buffer_size_for_directmedia(void *a, unsigned int *size, LGAO_DIRECTMEDIA_BUF_T buf);
int lgao_set_audio_info(void *a, LGAO_AUDIO_START_INFO_T info);
int lgao_get_audio_info(void *a, LGAO_AUDIO_INFO_T prop, unsigned int *value);
int lgao_compare_audio_info(void *a, LGAO_AUDIO_START_INFO_T *newinfo);
int lgao_set_ease_volume(void *a);

int  lgao_connect_renderer(void *a);
int  lgao_disconnect_renderer(void *a);
void lgao_add_rts(void *a);
int  lgao_config(void *a);
int  lgao_start_renderer(void *a);
int  lgao_stop_renderer(void *a);
int  lgao_open_renderer(void *a);
int  lgao_close_renderer(void *a);

int lgao_set_first_render(void *a, int type, int value);
int lgao_get_first_render(void *a, int type, int *value);
int lgao_get_size_buffered(void *a, int type, int *value);
int lgao_get_delay(void *a, int *value);
int lgao_set_basetime(void *a, long long base, long long start);
int lgao_ischanged_output_connect(void *a);
int lgao_set_pcm_bypass(void *a, int bypass);
int lgao_get_pcm_bypass(void *a, int *bypass);
int lgao_set_pcm_multi_channel(void *a, int multi_ch);
int lgao_get_pcm_multi_channel(void *a, int *multi_ch);

int lgao_pause(void *a);
int lgao_flush(void *a);
int lgao_play(void *a);
int lgao_push_first_event(void *a);
int lgao_feed(void *a, void *buff, unsigned int buffLength, unsigned long long timestamp,
              LGAO_DATA_TYPE_T outType);
int lgao_audio_gap(void *a, unsigned long long start, unsigned long long end);
int lgao_set_game_mode(void *a, int game_mode);
int lgao_get_game_mode(void *a, int *game_mode);
int lgao_get_vdec_current_pts(void *a, unsigned int vdec_port, unsigned int *pts);
int lgao_get_drop_renderer(void *a);
int lgao_set_drop_renderer(void *a, int is_drop);
int lgao_set_queue_max_length(void *a, int queue_max_length);
int lgao_get_queue_max_length(void *a, int *queue_max_length);
int lgao_set_vsp_mode(void *a);

#endif
