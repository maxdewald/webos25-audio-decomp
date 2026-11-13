/*
 * lgao: the audio-output layer under the lgaudiosink element
 * (gst-plugins-bsp/gstlgaudio/lgaudio/lgao.c). It owns /dev/lg/aren0, builds
 * the aimc chain "appsrc -> renderer head", "renderer tail -> appsink" and
 * "bypass appsrc -> bypass head", and feeds PCM/ES buffers into it.
 *
 * RECONSTRUCTION from usr/lib/gstreamer-1.0/libgstlgaudiosink.so
 *   sha256 90dba3ef32704ce4096bf1e6045d2879d0c2192d507bc04d1e8e98437f420a43
 * Names, prototypes and struct layouts come from the binary's DWARF (lgao.c CU).
 * Tags are raw ELF VAs (decomp.toml src_addr_bias = 0x10000).
 * Evidence: build/scratch/evidence/renderer-abi/, build/decomp/libgstlgaudiosink.so.c.
 */
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "../include/lg_audio_policy.h"
#include "lgao.h"

/* ---- /dev/lg/aren* requests not already in lg_audio_policy.h ('R') ------- */
typedef struct { unsigned int clock_base_time, stream_base_time; } aren_base_time_t;
typedef struct {
    unsigned int lower_bound, upper_bound, offset, freerun_lbound, freerun_ubound;
} aren_lipsync_param_t;
typedef enum {
    AUD_GAIN_DURATION_MODE_LINEAR = 0,
    AUD_GAIN_DURATION_MODE_INCUBIC,
    AUD_GAIN_DURATION_MODE_OUTCUBIC,
} aud_gain_duration_mode_t;
typedef struct {
    unsigned int volume;                 /* Q23: 0x800000 = 100 % */
    unsigned int duration;
    aud_gain_duration_mode_t type;
} aren_ease_volume_t;
typedef struct { unsigned int start_pts, end_pts; } aren_gap_info_t;
typedef struct {
    unsigned int max_mem_size, free_mem_size, max_aui_size, free_aui_size;
    unsigned int feeded_index, feeded_count, rendered_index;
    unsigned long long timestamp;
    bool present_end;
} aren_rendered_status_t;
typedef struct { unsigned int queue_max, queued_byte; } aren_rts_queue_info_t;
typedef struct { unsigned int port, pts; } aren_vdec_current_pts_t;

#define AREN_GET_GSTC              AREN_IOC(2, 4, 4)
#define AREN_SET_CLOCKTYPE         AREN_IOC(1, 5, 4)
#define AREN_SET_ENABLE_LIPSYNC    AREN_IOC(1, 6, 4)
#define AREN_SET_BASETIME          AREN_IOC(1, 7, sizeof(aren_base_time_t))
#define AREN_SET_LIPSYNC_PARAM     AREN_IOC(1, 8, sizeof(aren_lipsync_param_t))
#define AREN_SET_SPEED             AREN_IOC(1, 9, 4)
#define AREN_SET_MUTE              AREN_IOC(1, 10, 4)
#define AREN_SET_EASE_VOLUME       AREN_IOC(1, 11, sizeof(aren_ease_volume_t))
#define AREN_GET_EASE_VOLUME       AREN_IOC(2, 12, 4)
#define AREN_SET_GAP_INFO          AREN_IOC(1, 14, sizeof(aren_gap_info_t))
#define AREN_GET_RENDERED_STATUS   AREN_IOC(2, 16, sizeof(aren_rendered_status_t))
#define AREN_GET_RTS_QUEUE_INFO    AREN_IOC(2, 17, sizeof(aren_rts_queue_info_t))
#define AREN_SET_RTS_PARAM         AREN_IOC(1, 18, sizeof(aren_rts_param_t))
#define AREN_ADD_RTS               AREN_IOC(0, 19, 0)
#define AREN_SET_JP4K_STREAM       AREN_IOC(1, 20, sizeof(bool))
#define AREN_START                 AREN_IOC(0, 30, 0)
#define AREN_STOP                  AREN_IOC(0, 31, 0)
#define AREN_SET_BYPASS_FADER      AREN_IOC(1, 36, sizeof(bool))
#define AREN_GET_VDEC_CURRENT_PTS  AREN_IOC(3, 38, sizeof(aren_vdec_current_pts_t))
#define AREN_SET_QUEUE_MAX_LENGTH  AREN_IOC(1, 39, 4)
#define AREN_SET_VSP_MODE          AREN_IOC(0, 42, 0)

/* ---- libaudcommon / libaabase / libaa imports (declared locally) -------- */
struct aimc;
struct registry;
struct aud_connector;
struct aud_object { unsigned int user, type; void (*destroy)(struct aud_object *); };
struct aud_module { char *name; unsigned int uid; /* ... */ };

enum aud_event_type { AUD_EVENT_TYPE_CUSTOM = 13 };
struct aud_event {
    struct aud_object object;
    enum aud_event_type type;            /* +0x0c */
    /* ... */
};
enum aud_value_type { aud_value_int = 0, aud_value_bool = 1, aud_value_str = 2 };

enum aud_data_type { type_pcm, type_es, type_meta, type_meta_ext, type_mixed };
struct aud_data {
    struct aud_object object;
    unsigned int _pad;
    struct {
        enum aud_data_type type;         /* +0x10 */
        unsigned int index;
        unsigned int timestamp;          /* +0x18, 90 kHz */
        unsigned long long timestamp64;  /* +0x20, ns */
        unsigned int flag;
    } info;
    void *caller;
    unsigned int size;
    unsigned char *data;                 /* +0x38 */
};

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
extern struct aud_module *ModuleRegistry_SearchInstByUid(const unsigned int uid);
extern struct aud_connector *Connector_Create(int proc_id, struct aimc *imc);
extern void Connector_Destroy(struct aud_connector *con);
extern int module_appsrc_push_data(struct aud_module *m, struct aud_data *d, bool block);
extern int module_appsrc_push_event(struct aud_module *m, struct aud_event *e);
extern int module_appsink_set_callback(struct aud_module *m, struct module_appsink_callback *cb,
                                       void *priv);
extern struct aud_object *aud_object_get(struct aud_object *o);
extern void aud_object_put(struct aud_object *o);
extern struct aud_data *aud_data_new(int size);
extern struct aud_event *aud_event_newv_first(const char *field, ...);
extern struct aud_event *aud_event_newv_custom(const char *field, ...);
extern struct aud_event *aud_event_dup_and_set(struct aud_event *e, const char *field, ...);
extern struct aud_event *aud_event_new_flush(void);
extern struct aud_event *aud_event_new_flush_async(void);
extern struct aud_event *aud_event_new_last(void);
extern int aud_event_value_set_int(struct aud_event *e, const char *field, int v);
extern int aud_event_value_set_bool(struct aud_event *e, const char *field, bool v);
extern const char *aud_event_value_get_str(struct aud_event *e, const char *field,
                                           const char *def);
extern const char *aud_event_name(struct aud_event *e);
extern char *aud_event_str(struct aud_event *e);
extern void KADP_InitSystem(void);

/* ---- logging (liblogm) ------------------------------------------------- */
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
extern int logmm_printl(struct logmm_module *m, enum log_level lvl, const char *func,
                        int line, const char *fmt, ...);

struct logmm_module logmm_module_lgao = { 0, 0, 7, &logmm_dummy_mask, "lgao" };

/* line = __LINE__ of the original lgao.c, kept as evidence */
#define LOG(lvl, line, ...) do { \
        if (*logmm_module_lgao.mask & (1u << (lvl))) \
            logmm_printl(&logmm_module_lgao, (lvl), __func__, (line), __VA_ARGS__); \
    } while (0)
#define LOGE(line, ...) LOG(log_level_error, line, __VA_ARGS__)
#define LOGN(line, ...) LOG(log_level_noti, line, __VA_ARGS__)
#define LOGI(line, ...) LOG(log_level_info, line, __VA_ARGS__)
#define LOGD(line, ...) LOG(log_level_debug, line, __VA_ARGS__)

#define ADEC_KIND(ao)   ((ao)->isMixer ? "MIX" : "DEC")
#define UNSET           0xffffffffu
#define Q23_UNITY       8388608.0        /* gain 1.0 in the renderer's Q23 */
#define RTS_MS(ms)      ((ms) * 384u)    /* RTS thresholds: ms -> bytes (48k*2ch*4B) */

/* GStreamer ns -> 90 kHz PTS */
static inline unsigned int ns_to_pts(unsigned long long ns) { return ns / 10 * 9 / 10000; }
static inline unsigned int sns_to_pts(long long ns) { return ns / 10 * 9 / 10000; }

typedef struct { LGAO_LIPSYNC_OFFSET_MODE_T mode; const char *name; const int delay; }
    LIPSYNC_OFFSET_VALUE_T;
LIPSYNC_OFFSET_VALUE_T LIPSYNC_OFFSET_VALUES[8] = {
    { LGAO_LIPSYNC_OFFSET_MODE_PCM,         "PCM",           0 },
    { LGAO_LIPSYNC_OFFSET_MODE_WIRELESS,    "WIRELESS",    150 },
    { LGAO_LIPSYNC_OFFSET_MODE_DD_DDP,      "DD_DDP",      105 },
    { LGAO_LIPSYNC_OFFSET_MODE_ATMOS,       "ATMOS",       115 },
    { LGAO_LIPSYNC_OFFSET_MODE_WISA,        "WISA",        100 },
    { LGAO_LIPSYNC_OFFSET_MODE_ES,          "ES",           30 },
    { LGAO_LIPSYNC_OFFSET_MODE_AAC,         "AAC",         160 },
    { LGAO_LIPSYNC_OFFSET_MODE_BT_SURROUND, "BT_SURROUND", 200 },
};

static unsigned int dumpIndex;

/* @0x141b0 */
void _lgao_set_basetime(lgao *ao)
{
    aren_base_time_t base_time;

    LOGI(0x96c, "stc.base=%llu, pts.base=%llu", ao->clockBasetime, ao->streamBasetime);
    base_time.clock_base_time = ao->clockBasetime;
    base_time.stream_base_time = ao->streamBasetime;
    ioctl(ao->fd, AREN_SET_BASETIME, &base_time);
}

/* @0x1421c: current ease volume in percent (renderer value while fading) */
int _lgao_get_ease_volume(lgao *ao)
{
    unsigned int fade_volume = 0;
    int volume;

    if (ao->duration != UNSET && ao->remainDuration == 0) {
        volume = ao->targetVolume;
        LOGN(0x4ae, "Get fade_volume %d", volume);
        return volume;
    }
    ioctl(ao->fd, AREN_GET_EASE_VOLUME, &fade_volume);
    volume = (int)ceil(fade_volume / Q23_UNITY * 100.0);
    LOGN(0x4b5, "Get fade_volume %u, %d", fade_volume, volume);
    return volume;
}

/* @0x142e8: events coming back out of the renderer tail into our appsink */
enum appsink_event_return lgao_appsink_handle_event(void *priv, int port_num,
                                                    struct aud_event *event)
{
    lgao *ao = priv;
    char *str = aud_event_str(event);
    lgao_mq_item msg;

    if (str) {
        LOGI(0x6e3, "Event : %s", str);
        free(str);
    } else {
        LOGI(0x6e6, "Event : %s", aud_event_name(event));
    }

    /* the custom "eos"="last" marker pushed by lgao_eos made it through */
    if (event->type == AUD_EVENT_TYPE_CUSTOM &&
        strcmp(aud_event_value_get_str(event, "eos", "no"), "last") == 0) {
        msg.mtype = LGAO_MSG_TYPE_EVENT;
        msg.event = LGAO_EVENT_PRESENT_END;
        LOGN(0x6ef, "I GOT EOS\n");
        if (msgsnd(ao->callback_mq_id, &msg, sizeof(msg.event), 0) < 0)
            LOGE(0x6f1, "msgsnd failed, msg: %05X", msg.event);
    }
    return appsink_event_not_handled;
}

int lgao_appsink_handle_data(void *priv, int input_port_num, struct aud_data *data);
void _sig_handle(int sig, siginfo_t *info, void *context);

/* Exported mutable callback TABLE, not a registration/access API for producers.
 * Connect installs it on the main tail appsink with priv=ao. Replacing it affects
 * every instance sharing the table, can steal EOS/data ownership, and may run
 * while an upstream sync holds stream locks: reentrant bypass submission risks
 * a lock cycle. It supplies no lease against disconnect/close; not a safe hook.
 * pFnCallbackFn/pCallbackFnParam below are initialized NULL but never dispatched
 * by this shipped library. Event MQ getter returns an ID, not ao ownership. */
struct module_appsink_callback lgao_appsink_cb = {
    lgao_appsink_handle_data,
    lgao_appsink_handle_event,
};

/* inlined into _lgao_connect_renderer. The shipped kernel has no nr 43 (-ENOTTY),
 * so in practice no SIGUSR1/UNDERFLOW is ever raised (see build/scratch/evidence/renderer-abi). */
static inline void _lgao_set_msgpid_to_signal(lgao *ao)
{
    struct aren_msgpid_to_signal value;
    struct sigaction sa;

    value.msgq_id = ao->callback_mq_id;
    value.pid = getpid();
    ioctl(ao->fd, AREN_SET_MSGPID_TO_SIGNAL, &value);
    LOGI(0x768, "mq_id = %d, pid = %d\n", value.msgq_id, value.pid);

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = _sig_handle;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        LOGE(0x76f, "error sigaction\n");
}

/* @0x1441c: GET_BYPASS_UID follows SET_PCM_INFO on every rebuilt renderer.
 * The call creates new appsrcs; caps teardown invalidates the old borrowed ones.
 * No es/codec test here: PCM versus ES bypass selection belongs to the kernel.
 * Main and bypass have separate UIDs; only the main tail appsink gets cb/priv=ao.
 * Failure destroys owned modules and clears their borrowed registry pointers. */
int _lgao_connect_renderer(lgao *ao)
{
    unsigned int uid_src = 0, uid_gain = 0;

    ao->uid_appsrc = aimc_module_create(ao->imc, "appsrc", 0);
    if (!ao->uid_appsrc) {
        LOGE(0x77d, "can't create module appsrc");
        goto fail;
    }
    ao->uid_bypass_appsrc = aimc_module_create(ao->imc, "appsrc", 0);
    if (!ao->uid_bypass_appsrc) {
        LOGE(0x783, "can't create module bypass_appsrc");
        goto fail;
    }
    LOGI(0x787, "appsrc (0x%08X), bypass_appsrc (0x%08X)", ao->uid_appsrc,
         ao->uid_bypass_appsrc);
    aimc_handler_sync(ao->imc);

    ao->appsrc = ModuleRegistry_SearchInstByUid(ao->uid_appsrc);
    if (!ao->appsrc) {
        LOGE(0x78e, "create appsrc error!!");
        goto fail;
    }
    ao->bypass_appsrc = ModuleRegistry_SearchInstByUid(ao->uid_bypass_appsrc);
    if (!ao->bypass_appsrc) {
        LOGE(0x794, "create bypass_appsrc error!!");
        goto fail;
    }

    /* main path: appsrc -> renderer head ... renderer tail (port 1) -> appsink */
    ioctl(ao->fd, AREN_ADD_QUEUE);
    ioctl(ao->fd, AREN_GET_UID, &uid_src);
    ioctl(ao->fd, AREN_GET_TAIL_UID, &uid_gain);
    LOGI(0x79c, "appsrc (%s)", ao->appsrc->name);
    LOGI(0x79d, "appsrc head uid is 0x%08x", uid_src);
    if (aimc_module_connect(ao->imc, ao->uid_appsrc, 0, uid_src, 0)) {
        LOGE(0x7a0, "cannot connect module");
        goto fail;
    }
    LOGI(0x7a4, "renderer tail uid is 0x%08x", uid_gain);

    ao->uid_appsink = aimc_module_create(ao->imc, "appsink", 0);
    if (!ao->uid_appsink) {
        LOGE(0x7a9, "can't create module appsink\n");
        goto fail;
    }
    ao->appsink = ModuleRegistry_SearchInstByUid(ao->uid_appsink);
    if (!ao->appsink) {
        LOGE(0x7af, "cannot find appsink from registry\n");
        goto fail;
    }
    if (aimc_module_connect(ao->imc, uid_gain, 1, ao->uid_appsink, 0)) {
        LOGE(0x7b4, "cannot connect module");
        goto fail;
    }
    module_appsink_set_callback(ao->appsink, &lgao_appsink_cb, ao);

    /* bypass path: the kernel picks/creates PCM or ES bypass from SET_PCM_INFO */
    ioctl(ao->fd, AREN_GET_BYPASS_UID, &uid_src);
    LOGI(0x7bc, "bypass appsrc (%s)", ao->bypass_appsrc->name);
    LOGI(0x7bd, "bypass_head uid is 0x%08x", uid_src);
    if (aimc_module_connect(ao->imc, ao->uid_bypass_appsrc, 0, uid_src, 0)) {
        LOGE(0x7c0, "cannot connect module");
        goto fail;
    }

    ioctl(ao->fd, AREN_SET_BYPASS_FADER, &ao->game_mode);
    ioctl(ao->fd, AREN_SET_QUEUE_MAX_LENGTH, &ao->queue_max_length);
    _lgao_set_msgpid_to_signal(ao);
    return 0;

fail:
    if (ao->uid_appsrc) {
        aimc_module_destroy(ao->imc, ao->uid_appsrc);
        ao->uid_appsrc = 0;
        ao->appsrc = NULL;
    }
    if (ao->uid_appsink) {
        aimc_module_destroy(ao->imc, ao->uid_appsink);
        ao->uid_appsink = 0;
        ao->appsink = NULL;
    }
    if (ao->uid_bypass_appsrc) {
        aimc_module_destroy(ao->imc, ao->uid_bypass_appsrc);
        ao->uid_bypass_appsrc = 0;
        ao->bypass_appsrc = NULL;
    }
    return -1;
}

/* Inlined into _lgao_open_renderer: publish imc/reg/con only after all succeed.
 * Failure releases local allocations without publishing a partial owner. */
static inline void _lgao_aimc_init(lgao *ao)
{
    struct aimc *imc;
    struct registry *reg;
    struct aud_connector *con;

    LOGI(0x195, "aimc_init\n");
    imc = aimc_create();
    if (!imc) {
        LOGE(0x199, "can't create aimc");
        return;
    }
    aimc_create_handler(imc);
    reg = ModuleRegistry_Init(imc);
    if (!reg) {
        LOGE(0x1a0, "no registry");
        aimc_finalize(imc);
        return;
    }
    con = Connector_Create(aimc_get_processor_id(imc), imc);
    if (!con) {
        LOGE(0x1a7, "no connector");
        ModuleRegistry_Cleanup(reg);
        aimc_finalize(imc);
        return;
    }
    ao->imc = imc;
    ao->reg = reg;
    ao->con = con;
}

/* @0x14924: sink index maps to AREN index +4 only in mixer mode, without clamping.
 * All instances open aren0; SET_INDEX, not the device filename, selects the slot.
 * SET_PROC_ID registers this owner's AIMC processor, including on reopen. */
int _lgao_open_renderer(lgao *ao)
{
    unsigned int index = ao->index;
    unsigned int proc_id;

    if (ao->isMixer)
        index += 4;
    ao->fd = open("/dev/lg/aren0", O_RDWR | O_CLOEXEC);
    if (ao->fd < 0) {
        LOGE(0x73a, "open renderer error");
        LOGE(0x74b, "Open Err!!!");
        return -1;
    }
    ioctl(ao->fd, AREN_SET_INDEX, &index);

    if (!ao->imc)
        _lgao_aimc_init(ao);
    if (!ao->imc)
        return 0;               /* no aimc: renderer stays open without PROC_ID */

    proc_id = aimc_get_processor_id(ao->imc);
    ioctl(ao->fd, AREN_SET_PROC_ID, &proc_id);
    return 0;
}

/* @0x14ae0 */
int lgao_appsink_handle_data(void *priv, int input_port_num, struct aud_data *data)
{
    aud_object_put(&data->object);
    LOGD(0x6d7, "handle data done.");
    return 0;
}

/* @0x14b2c: SIGUSR1 from the renderer driver; si_int carries our msgq id */
void _sig_handle(int sig, siginfo_t *info, void *context)
{
    lgao_mq_item msg;

    if (sig != SIGUSR1)
        return;
    LOGI(0x753, "Received signal %s data : si_signo = %d, si_code = %d, si_int =%d \n",
         strsignal(SIGUSR1), info->si_signo, info->si_code, info->si_int);
    msg.mtype = LGAO_MSG_TYPE_EVENT;
    msg.event = LGAO_EVENT_UNDERFLOW;
    if (msgsnd(info->si_int, &msg, sizeof(msg.event), 0) < 0)
        LOGE(0x759, "msgsnd failed, msg: %05X", msg.event);
}

/* @0x14bd8 */
int lgao_get_event_mq_id(void *a)
{
    lgao *ao = a;
    return ao->callback_mq_id;
}

static inline void _lgao_init_rts(aren_rts_param_t *rts)
{
    rts->onoff = true;
    rts->opmode = rts->upper_threshold = rts->lower_threshold = UNSET;
    rts->recover_upper = rts->recover_lower = UNSET;
    rts->skipping_threshold = rts->recover_skipping = UNSET;
    rts->fast_freq = rts->normal_freq = rts->slow_freq = UNSET;
    rts->mix_fade_len = rts->mix_wait_len = rts->buffering_level = UNSET;
}

static inline void _lgao_init_dump(lgao *ao)
{
    char dumpFilename[64];

    ao->pDumpAudioFD = NULL;
    if (!ao->isEnableDump)
        return;
    sprintf(dumpFilename, "/build/scratch/audiosink-#%03u.dump", dumpIndex);
    ao->pDumpAudioFD = fopen(dumpFilename, "wb");
    if (!ao->pDumpAudioFD)
        LOGE(0x186, "FOpen error !! <%s>", dumpFilename);
    else
        dumpIndex++;
}

/* @0x14be0 */
void *lgao_open(void)
{
    lgao *ao = calloc(1, sizeof(*ao));

    KADP_InitSystem();
    LOGD(0x1bb, "Compile info : %s %s", "Oct 16 2025", "05:55:01");

    ao->fd = -1;
    ao->index = -1;
    ao->isMixer = false;
    ao->isMute = false;
    ao->targetVolume = UNSET;
    ao->duration = UNSET;
    ao->fadeType = LGAO_EASE_NONE;
    ao->currVolume = UNSET;
    ao->remainDuration = UNSET;
    ao->rendererState = LGAO_STATE_INIT;
    for (int i = LGAO_DATA_PCM; i <= LGAO_DATA_METADATA; i++) {
        ao->bufferedSize[i] = 0;
        ao->isFirstRender[i] = true;
        ao->prevTimestamp[i] = ~0ull;
    }
    ao->lipsyncMode = LGAO_LIPSYNC_AUTO;
    ao->delay = 0;
    ao->lipsyncOffset = 0xfffffff;
    ao->playSpeed = 1000;
    ao->isNoDelayMode = false;
    ao->appType = LGAO_APPTYPE_UNKNOWN;
    ao->isMainDecoder = false;

    ao->callback_mq_id = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    LOGI(0x1e2, "audio mq [%d] opended for instance %p", ao->callback_mq_id, ao);

    _lgao_init_rts(&ao->rts_params);

    ao->isEnableDump = access("/build/scratch/gst_dump_audio_pcm", F_OK) == 0;
    _lgao_init_dump(ao);

    ao->pFnCallbackFn = NULL;
    ao->pCallbackFnParam = NULL;
    memset(&ao->audioStartInfo, 0, sizeof(ao->audioStartInfo));
    ao->clock_type = AUD_CLK_TYPE_GSTC;
    ao->lipsyncOffsetMode = LGAO_LIPSYNC_OFFSET_MODE_UNKNOWN;
    ao->spdifStatus = LGAO_IEC_OUTPUT_NONE;
    ao->arcStatus = LGAO_IEC_OUTPUT_NONE;
    ao->render_task_id = -1;
    ao->render_task_running = 0;
    ao->render_task_join = 0;
    ao->uid_appsrc = 0;
    ao->appsrc = NULL;
    ao->uid_appsink = 0;
    ao->appsink = NULL;
    ao->pcm_bypass = false;
    ao->pcm_multi_ch = false;
    ao->uid_bypass_appsrc = 0;
    ao->bypass_appsrc = NULL;
    ao->game_mode = false;
    ao->vsp_mode = false;
    ao->drop_renderer = 0;
    ao->prev_state = LGAO_STATE_INIT;
    ao->queue_max_length = 0;
    pthread_cond_init(&ao->flush_cond, NULL);
    pthread_mutex_init(&ao->flush_mutex, NULL);
    return ao;
}

/* @0x14e00: final context release, not a stop/disconnect/close_renderer wrapper.
 * Caller must finish producers and renderer teardown before this frees ao. */
int lgao_close(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x225, "context is null");
        return -1;
    }
    LOGI(0x228, "callback thread joined");
    msgctl(ao->callback_mq_id, IPC_RMID, NULL);
    LOGI(0x22a, "msgq removed");
    ao->rendererState = LGAO_STATE_INIT;
    LOGI(0x22c, "Adec(%s.%d) Sink State = LGAO_STATE_INIT", ADEC_KIND(ao), ao->index);

    if (ao->isEnableDump) {         /* _lgao_deinit_dump */
        if (ao->pDumpAudioFD)
            fclose(ao->pDumpAudioFD);
        ao->pDumpAudioFD = NULL;
    }
    pthread_cond_destroy(&ao->flush_cond);
    pthread_mutex_destroy(&ao->flush_mutex);
    if (ao->con)
        Connector_Destroy(ao->con);
    if (ao->reg)
        ModuleRegistry_Cleanup(ao->reg);
    if (ao->imc)
        aimc_finalize(ao->imc);
    LOGN(0x23e, "aimc finalized.");
    free(ao);
    LOGN(0x242, "closed.");
    return 0;
}

/* @0x14fa4: returns 1 when the element must post EOS itself (nothing will come
 * back from the renderer), 0 when PRESENT_END will arrive via the msg queue. */
int lgao_eos(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x24e, "context is null");
        return -1;
    }
    if (ao->index == -1) {
        LOGE(0x24f, "index is invalid");
        return -1;
    }
    LOGN(0x251, "EOS: received event");

    if ((unsigned int)(ao->playSpeed - 250) > 1750 || ao->isMute) {
        LOGN(0x256, "trick play or mute. exit EOS handler");
        return 1;
    }
    if (ao->drop_renderer) {
        LOGN(0x25b, "dropping.. by es bypass..exit EOS handler\n");
        return 1;
    }
    if (ao->rendererState == LGAO_STATE_PLAY) {
        LOGN(0x260, "sennd last event to renderer!!");
        module_appsrc_push_event(ao->bypass_appsrc,
                                 aud_event_newv_custom("eos", aud_value_str, "last", NULL));
        module_appsrc_push_event(ao->bypass_appsrc, aud_event_new_last());
        module_appsrc_push_event(ao->appsrc,
                                 aud_event_newv_custom("eos", aud_value_str, "last", NULL));
        module_appsrc_push_event(ao->appsrc, aud_event_new_last());
    }
    ao->isFirstRender[LGAO_DATA_PCM] = true;
    LOGN(0x26f, "EOS end");
    return 0;
}

static inline void _lgao_set_playspeed(lgao *ao, int playSpeed)
{
    if (!ao->drop_renderer)
        ioctl(ao->fd, AREN_SET_SPEED, &playSpeed);
}

/* @0x151b0: rate in 1/1000 (1000 = 1.0x) */
int lgao_set_playrate(void *a, int rate)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x288, "context is null");
        return -1;
    }
    LOGI(0x28a, "playrate changing %d -> %d", ao->playSpeed, rate);
    ao->playSpeed = rate;
    _lgao_set_playspeed(ao, rate);
    return 0;
}

/* @0x15254 */
int lgao_get_playrate(void *a, int *rate)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x29f, "context is null");
        return -1;
    }
    *rate = ao->playSpeed;
    return 0;
}

/* @0x152b4 */
int lgao_set_lipsync_mode(void *a, int mode)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x2b3, "context is null");
        return -1;
    }
    LOGI(0x2b5, "lipsync mode changing %s -> %s",
         ao->lipsyncMode == LGAO_LIPSYNC_AUTO ? "AUTO" :
         ao->lipsyncMode == LGAO_LIPSYNC_ENABLE ? "ENABLE" : "DISABLE",
         mode == LGAO_LIPSYNC_ENABLE ? "ENABLE" : "DISABLE");
    ao->lipsyncMode = mode;
    return 0;
}

/* @0x15378 */
int lgao_get_lipsync_mode(void *a, int *mode)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x2cb, "context is null");
        return -1;
    }
    *mode = ao->lipsyncMode;
    return 0;
}

/* @0x153d8 */
int lgao_set_lipsync_offset(void *a, unsigned int offset)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x2df, "context is null");
        return -1;
    }
    LOGI(0x2e1, "lipsync offset changing %d -> %d", ao->lipsyncOffset, offset);
    ao->lipsyncOffset = offset;
    return 0;
}

/* @0x15464 */
int lgao_get_lipsync_offset(void *a, unsigned int *offset)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x2f4, "context is null");
        return -1;
    }
    *offset = ao->lipsyncOffset;
    return 0;
}

/* @0x154c4: mixer mode = renderer index + 4 (see _lgao_open_renderer) */
int lgao_set_mode_mixer(void *a, int mode)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x308, "context is null");
        return -1;
    }
    LOGI(0x30a, "mixer mode changing %s -> %s", ADEC_KIND(ao), mode == 1 ? "MIX" : "DEC");
    ao->isMixer = mode != 0;
    return 0;
}

/* @0x15580: scalar mode only; a missing owner leaves the caller's output alone. */
int lgao_get_mode_mixer(void *a, int *mode)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x31f, "context is null");
        return -1;
    }
    *mode = ao->isMixer;
    return 0;
}

/* @0x155e0 */
int lgao_set_nodelay(void *a, int mode)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x333, "context is null");
        return -1;
    }
    LOGI(0x335, "no-delay mode  changing %s -> %s", ao->isNoDelayMode ? "TRUE" : "FALSE",
         mode ? "TRUE" : "FALSE");
    ao->isNoDelayMode = mode != 0;
    return 0;
}

/* @0x1569c */
int lgao_get_nodelay(void *a, int *mode)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x34a, "context is null");
        return -1;
    }
    *mode = ao->isNoDelayMode;
    return 0;
}

/* @0x156fc: values are in ms; converted to bytes in _lgao_config_set_rts_param */
int lgao_set_rts(void *a, LGAO_RTS_PARAM param, unsigned int value)
{
    lgao *ao = a;
    aren_rts_param_t *rts;

    if (!ao) {
        LOGE(0x35f, "context is null");
        return -1;
    }
    rts = &ao->rts_params;
    switch (param) {
    case LGAO_RTS_OP_MODE:
        LOGI(0x363, "RTS OP Mode changing %u -> %u", rts->opmode, value);
        rts->opmode = value;
        break;
    case LGAO_RTS_THRESHOLD_SKIPPING:
        LOGI(0x368, "RTS Skipping threshold changing %u -> %u", rts->skipping_threshold, value);
        rts->skipping_threshold = value;
        break;
    case LGAO_RTS_THRESHOLD_FAST:
        LOGI(0x36e, "RTS Upper threshold changing %u -> %u", rts->upper_threshold, value);
        rts->upper_threshold = value;
        break;
    case LGAO_RTS_THRESHOLD_RECOVER:
        LOGI(0x374, "RTS Recover Skipping threshold changing %u -> %u",
             rts->recover_skipping, value);
        rts->recover_skipping = value;
        rts->recover_upper = value;
        break;
    case LGAO_RTS_FREQ_FAST:
        LOGI(0x37b, "RTS Fast Frequency of Fast changing %u -> %u", rts->fast_freq, value);
        rts->fast_freq = value;
        break;
    case LGAO_RTS_BUFFERING_LEVEL:
        LOGI(0x380, "RTS Buffering Level %u -> %u", rts->buffering_level, value);
        rts->buffering_level = value;
        break;
    default:
        LOGN(0x385, "undefined RTS param %d", param);
        break;
    }
    return 0;
}

/* @0x158f8 */
int lgao_get_rts(void *a, LGAO_RTS_PARAM param, unsigned int *value)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x399, "context is null");
        return -1;
    }
    switch (param) {
    case LGAO_RTS_OP_MODE:            *value = ao->rts_params.opmode; break;
    case LGAO_RTS_THRESHOLD_SKIPPING: *value = ao->rts_params.skipping_threshold; break;
    case LGAO_RTS_THRESHOLD_FAST:     *value = ao->rts_params.upper_threshold; break;
    case LGAO_RTS_THRESHOLD_RECOVER:  *value = ao->rts_params.recover_skipping; break;
    case LGAO_RTS_FREQ_FAST:          *value = ao->rts_params.fast_freq; break;
    case LGAO_RTS_BUFFERING_LEVEL:    *value = ao->rts_params.buffering_level; break;
    default:
        LOGN(0x3b2, "undefined RTS param %d", param);
        break;
    }
    return 0;
}

/* @0x159ec */
int lgao_set_apptype(void *a, LGAO_APPTYPE_T apptype)
{
    lgao *ao = a;
    const char *str;

    if (!ao) {
        LOGE(0x3c6, "context is null");
        return -1;
    }
    ao->appType = apptype;
    switch (apptype) {
    case LGAO_APPTYPE_DIRECTMEDIA: str = "DIRECTMEDIA"; break;
    case LGAO_APPTYPE_RTC:         str = "RTC"; break;
    case LGAO_APPTYPE_MIRACAST:    str = "MIRACAST"; break;
    case LGAO_APPTYPE_AIRPLAY:     str = "AIRPLAY"; break;
    case LGAO_APPTYPE_JP4K:        str = "JP4K"; break;
    case LGAO_APPTYPE_APPCAST:     str = "APPCAST"; break;
    case LGAO_APPTYPE_R2R:         str = "R2R"; break;
    case LGAO_APPTYPE_SYE:         str = "SYE"; break;
    default:                       str = "UNKNOWN"; break;
    }
    LOGN(0x3f0, "AppType received %s", str);
    return 0;
}

/* @0x15af8 */
int lgao_get_apptype(void *a, LGAO_APPTYPE_T *apptype)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x402, "context is null");
        return -1;
    }
    *apptype = ao->appType;
    return 0;
}

/* @0x15b58: audioPort 0..1 = decoder renderer; audioPort -1 + mixerPort 0..6 = mixer */
int lgao_set_port(void *a, int audioPort, int mixerPort)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x40f, "context is null");
        return -1;
    }
    if (audioPort == -1 && mixerPort == -1) {
        LOGE(0x412, "port no.s are both -1!");
        return -1;
    }
    if (mixerPort >= 7 || audioPort >= 7) {
        LOGE(0x417, "invalid port number %d %d", audioPort, mixerPort);
        return -1;
    }
    if (audioPort == -1) {
        if (mixerPort < 0) {
            LOGE(0x42b, "mixer-port(%d) is invalid value", mixerPort);
            return -1;
        }
        ao->index = mixerPort;
        LOGD(0x427, "Set mixer index(resource-info) to %u", mixerPort);
        lgao_set_mode_mixer(ao, 1);
        LOGD(0x429, "Set using Mixer");
        return 0;
    }
    if ((unsigned int)audioPort >= 2) {
        LOGE(0x421, "audio-port(%d) is invalid value", audioPort);
        return -1;
    }
    ao->index = audioPort;
    LOGD(0x41f, "Set audio index(resource-info) to %u", audioPort);
    return 0;
}

/* @0x15d3c: logical ao index, not the mixer-adjusted AREN slot or a module UID. */
int lgao_get_port(void *a, int *port)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x440, "context is null");
        return -1;
    }
    *port = ao->index;
    return 0;
}

/* @0x15d9c */
int lgao_set_mute(void *a, int mode)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x454, "context is null");
        return -1;
    }
    LOGN(0x456, "mute status of renderer [%d] changing %d -> %d", ao->fd, ao->isMute, mode);
    ao->isMute = mode != 0;
    if (!ao->drop_renderer)
        ioctl(ao->fd, AREN_SET_MUTE, &mode);
    return 0;
}

/* @0x15e50 */
int lgao_get_mute(void *a, int *mode)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x46d, "context is null");
        return -1;
    }
    *mode = ao->isMute;
    return 0;
}

/* @0x15eb0 */
int lgao_set_fade(void *a, LGAO_FADE_PROP_T prop, int value)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x482, "context is null");
        return -1;
    }
    switch (prop) {
    case LGAO_FADE_PROP_TARGET_VOL:
        LOGN(0x486, "ease volume setting, target vol changing %d -> %d", ao->targetVolume, value);
        ao->targetVolume = value;
        break;
    case LGAO_FADE_PROP_DURATION:
        LOGN(0x48c, "ease volume setting, duration changing %d -> %d", ao->duration, value);
        ao->duration = value;
        break;
    case LGAO_FADE_PROP_REMAIN_DURATION:
        LOGN(0x492, "ease volume setting, remaining duration changing %d -> %d",
             ao->remainDuration, value);
        ao->remainDuration = value;
        break;
    case LGAO_FADE_PROP_TYPE:
        LOGN(0x498, "ease volume setting, fade type changing %d -> %d", ao->fadeType, value);
        ao->fadeType = value;
        break;
    default:
        LOGN(0x49f, "undefined ease volume param %d", prop);
        break;
    }
    return 0;
}

/* @0x16018 */
int lgao_get_fade(void *a, LGAO_FADE_PROP_T prop, int *value)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x4c9, "context is null");
        return -1;
    }
    switch (prop) {
    case LGAO_FADE_PROP_TARGET_VOL:      *value = ao->targetVolume; break;
    case LGAO_FADE_PROP_DURATION:        *value = ao->duration; break;
    case LGAO_FADE_PROP_REMAIN_DURATION: *value = ao->remainDuration; break;
    case LGAO_FADE_PROP_TYPE:            *value = ao->fadeType; break;
    case LGAO_FADE_PROP_EASE_VOL:        *value = _lgao_get_ease_volume(ao); break;
    default:
        LOGN(0x4e1, "undefined ease volume param %d", prop);
        break;
    }
    return 0;
}

/* @0x160e8: snapshot a running fade (before the renderer is torn down) so
 * _lgao_config_set_easevol can resume it from currVolume over remainDuration */
int lgao_keep_fade(void *a)
{
    lgao *ao = a;
    int volume;
    unsigned int remain;
    unsigned int target;
    double ratio, left;

    if (!ao) {
        LOGE(0x4ed, "context is null");
        return -1;
    }
    if (ao->targetVolume != UNSET) {
        volume = _lgao_get_ease_volume(ao);
        if (volume < 0) {
            LOGE(0x4f2, "GetVolume Err");
            ao->currVolume = UNSET;
            ao->remainDuration = UNSET;
            return 0;
        }

        target = ao->targetVolume;
        if (target == 100) {
            left = (1.0 - volume / 100.0) * ao->duration;
        } else if ((unsigned int)volume < target) {
            if (volume == 100) {
                LOGE(0x504, "set remainDuration Err");
                volume = -1;
                remain = -1;
                goto out;
            }
            ratio = (double)(100 - target) / (double)(100 - volume);
            left = (1.0 - ratio) * ao->duration;
        } else {
            ratio = (double)(100 - volume) / (double)(100 - target);
            left = (1.0 - ratio) * ao->duration;
        }
        remain = (unsigned int)left;
out:
        ao->currVolume = volume;
        ao->remainDuration = remain;
        LOGN(0x50a, "current Vol %u, dur %u, target vol %u, dur %u", volume, remain,
             ao->targetVolume, ao->duration);
        }
    return 0;
}

/* @0x162a0 */
int lgao_set_clocktype(void *a, LGAO_CLOCK_TYPE_T type)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x51e, "context is null");
        return -1;
    }
    LOGN(0x520, "clocktype setting [%d] -> [%d]", ao->clock_type, type);
    if (type == LGAO_CLOCK_TYPE_GSTC) {
        ao->clock_type = AUD_CLK_TYPE_GSTC;
    } else if (type == LGAO_CLOCK_TYPE_ATSC3) {
        LOGI(0x527, "Set atsc-wall-clock Type");
        ao->clock_type = AUD_CLK_TYPE_GSTC_1;
    }
    return 0;
}

/* @0x16374: only caches currVolume, no ioctl */
int lgao_set_volume(void *a, int vol)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x53e, "context is null");
        return -1;
    }
    LOGN(0x540, "volume setting [%d] -> [%d]", ao->currVolume, vol);
    ao->currVolume = vol;
    return 0;
}

/* @0x163fc */
int lgao_get_volume(void *a, int *vol)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x554, "context is null");
        return -1;
    }
    *vol = ao->currVolume;
    return 0;
}

/* @0x1645c */
int lgao_set_state(void *a, LGAO_STATE_T state)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x568, "context is null");
        return -1;
    }
    LOGN(0x56a, "sink state setting [%d] -> [%d]", ao->rendererState, state);
    ao->rendererState = state;
    return 0;
}

/* @0x164e8 */
int lgao_get_state(void *a, LGAO_STATE_T *state)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x57e, "context is null");
        return -1;
    }
    *state = ao->rendererState;
    return 0;
}

/* @0x16548: queries the renderer but never fills *undecodedsize (always 0) */
int lgao_get_buffer_status(void *a, unsigned long long *undecodedsize)
{
    lgao *ao = a;
    aren_rendered_status_t status;

    *undecodedsize = 0;
    if (!ao) {
        LOGE(0x58d, "context is null");
        return -1;
    }
    ioctl(ao->fd, AREN_GET_RENDERED_STATUS, &status);
    LOGI(0x59c, "undeocded size : %llu", *undecodedsize);
    return 0;
}

/* @0x165f4 */
int lgao_get_buffer_size_for_directmedia(void *a, unsigned int *size, LGAO_DIRECTMEDIA_BUF_T buf)
{
    lgao *ao = a;
    aren_rts_queue_info_t rts_queue_info;

    *size = 0;
    if (!ao) {
        LOGE(0x5aa, "context is null");
        return -1;
    }
    if (ao->fd < 0) {
        if (buf == LGAO_DIRECTMEDIA_TOTAL_BUF) {
            *size = 96000;
            LOGD(0x5af, "total size  : %d (init value)", 96000);
        } else {
            LOGD(0x5b2, "used size  : %d", 0);
        }
        return 0;
    }
    ioctl(ao->fd, AREN_GET_RTS_QUEUE_INFO, &rts_queue_info);
    if (buf == LGAO_DIRECTMEDIA_TOTAL_BUF) {
        *size = rts_queue_info.queue_max;
        LOGD(0x5bb, "total size  : %d", rts_queue_info.queue_max);
    } else {
        *size = rts_queue_info.queued_byte;
        LOGD(0x5be, "used size  : %d", rts_queue_info.queued_byte);
    }
    return 0;
}

/* @0x1673c: NOTE check_downmix is not copied (stays 0 from lgao_open's memset) */
int lgao_set_audio_info(void *a, LGAO_AUDIO_START_INFO_T info)
{
    lgao *ao = a;
    LGAO_AUDIO_START_INFO_T *pAudioInfo;

    if (!ao) {
        LOGE(0x5ca, "context is null");
        return -1;
    }
    pAudioInfo = &ao->audioStartInfo;
    pAudioInfo->codecType = info.codecType;
    pAudioInfo->freq = info.freq;
    pAudioInfo->numberOfChannel = info.numberOfChannel;
    pAudioInfo->bitsPerSample = info.bitsPerSample;
    pAudioInfo->bigEndianOfPcm = info.bigEndianOfPcm;
    pAudioInfo->unsignedOfPcm = info.unsignedOfPcm;
    pAudioInfo->acmod = info.acmod;
    pAudioInfo->lfe_enable = info.lfe_enable;
    pAudioInfo->dolby = info.dolby;
    pAudioInfo->clock_x4 = info.clock_x4;
    pAudioInfo->atmos = info.atmos;
    pAudioInfo->out_channels = info.out_channels;
    pAudioInfo->restart = info.restart;
    pAudioInfo->gap_correction = info.gap_correction;
    pAudioInfo->es = info.es;
    memcpy(pAudioInfo->codec_str, info.codec_str, sizeof(info.codec_str));
    pAudioInfo->data_bps = info.data_bps;
    pAudioInfo->data_channels = info.data_channels;
    pAudioInfo->data_rate = info.data_rate;
    pAudioInfo->downmix = info.downmix;
    LOGI(0x5ed, "[audio info] codec %d, freq %d, no.ch. %d, bps %d, endian %d, sign %d, "
         "acmod %d, check-downmix %d, lfe_enable %d dolby %d, clock_x4 (eac3) %d, "
         "out_channels (pcmcvt out) %d, retart %d",
         info.codecType, info.freq, info.numberOfChannel, info.bitsPerSample,
         info.bigEndianOfPcm, info.unsignedOfPcm, info.acmod, info.check_downmix,
         info.lfe_enable, info.dolby, info.clock_x4, info.out_channels, info.restart);
    return 0;
}

/* @0x16884 */
int lgao_get_audio_info(void *a, LGAO_AUDIO_INFO_T prop, unsigned int *value)
{
    lgao *ao = a;
    LGAO_AUDIO_START_INFO_T *pAudioInfo;

    if (!ao) {
        LOGE(0x5f9, "context is null");
        return -1;
    }
    pAudioInfo = &ao->audioStartInfo;
    switch (prop) {
    case LGAO_AUDIO_INFO_CODEC:           *value = pAudioInfo->codecType; break;
    case LGAO_AUDIO_INFO_SAMPLING_FREQ:   *value = pAudioInfo->freq; break;
    case LGAO_AUDIO_INFO_NUM_CH:          *value = pAudioInfo->numberOfChannel; break;
    case LGAO_AUDIO_INFO_BITS_PER_SAMPLE: *value = pAudioInfo->bitsPerSample; break;
    case LGAO_AUDIO_INFO_ENDIAN:          *value = pAudioInfo->bigEndianOfPcm; break;
    case LGAO_AUDIO_INFO_SIGNED:          *value = pAudioInfo->unsignedOfPcm; break;
    case LGAO_AUDIO_INFO_ACMOD:           *value = pAudioInfo->acmod; break;
    case LGAO_AUDIO_INFO_LFE_ENABLE:      *value = pAudioInfo->lfe_enable; break;
    case LGAO_AUDIO_INFO_DOLBY:           *value = pAudioInfo->dolby; break;
    case LGAO_AUDIO_INFO_DATA_PER_SEC:
        *value = (pAudioInfo->bitsPerSample >> 3) * pAudioInfo->freq *
                 pAudioInfo->numberOfChannel;
        break;
    case LGAO_AUDIO_INFO_RESTART:         *value = pAudioInfo->restart; break;
    case LGAO_AUDIO_INFO_DOWNMIX:         *value = pAudioInfo->downmix; break;
    default:
        LOGN(0x62a, "[audio info] undefined param %d", prop);
        break;
    }
    return 0;
}

/* @0x1699c: -1 = no renderer/info yet, 0 = same PCM format, 1 = changed, 2 = restart.
 * Raw Thumb 0x169a8..0x169d6 compares rate, packed channels/bps, endian and sign
 * only: es/codec/codec-str/data-* do not force reopening. ES -> S16LE stereo
 * rebuilds only if one of those PCM fields changes. restart=1 alone returns 2;
 * the CAPS handler only logs that case, without config/connect or FIRST rearm. */
int lgao_compare_audio_info(void *a, LGAO_AUDIO_START_INFO_T *newinfo)
{
    lgao *ao = a;
    LGAO_AUDIO_START_INFO_T *cur;

    if (!ao) {
        LOGE(0x636, "context is null");
        return -1;
    }
    cur = &ao->audioStartInfo;
    if (ao->fd < 0 && cur->codecType == LGAO_CODEC_UNKNOWN)
        return -1;
    if (cur->freq != newinfo->freq ||
        cur->numberOfChannel != newinfo->numberOfChannel ||
        cur->bitsPerSample != newinfo->bitsPerSample ||
        cur->bigEndianOfPcm != newinfo->bigEndianOfPcm ||
        cur->unsignedOfPcm != newinfo->unsignedOfPcm)
        return 1;
    return newinfo->restart == 1 ? 2 : 0;
}

static inline aud_gain_duration_mode_t _lgao_convert_ease_type(LGAO_EASE_TYPE_T type)
{
    if (type == LGAO_EASE_INCUBIC)
        return AUD_GAIN_DURATION_MODE_INCUBIC;
    if (type == LGAO_EASE_OUTCUBIC)
        return AUD_GAIN_DURATION_MODE_OUTCUBIC;
    return AUD_GAIN_DURATION_MODE_LINEAR;
}

static inline unsigned int pct_to_q23(unsigned int pct)
{
    double v = pct * Q23_UNITY / 100.0;
    return v > 0.0 ? (unsigned int)v : 0;
}

/* @0x16a34 */
int lgao_set_ease_volume(void *a)
{
    lgao *ao = a;
    aren_ease_volume_t ease_volume;

    if (!ao) {
        LOGE(0x66e, "context is null");
        return -1;
    }
    if (ao->fadeType == LGAO_EASE_NONE) {
        LOGN(0x671, "EaseVolume Options are Unknown");
        return -1;
    }
    ease_volume.type = _lgao_convert_ease_type(ao->fadeType);
    ease_volume.duration = ao->duration;
    ease_volume.volume = pct_to_q23(ao->targetVolume);
    ioctl(ao->fd, AREN_SET_EASE_VOLUME, &ease_volume);
    return 0;
}

/* @0x16b28: borrowed registry pointers are invalid after module destruction.
 * No producer/lifetime lock or FIRST rearm here. The element stops its callback
 * task before final stop -> disconnect -> close_renderer -> close; caps changes,
 * non-seamless track changes and drop also destroy/recreate these appsrcs.
 * Cached module pointers/UIDs must not be reused across that transition. */
int lgao_disconnect_renderer(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x708, "context is null");
        return -1;
    }
    if (ao->bypass_appsrc) {
        LOGI(0x70b, "disconnect bypass_appsrc (%s) - ren 0x%08x", ao->bypass_appsrc->name,
             ao->uid_bypass_appsrc);
        aimc_module_disconnect_output(ao->imc, ao->uid_bypass_appsrc, 0);
    }
    if (ao->appsrc) {
        LOGI(0x711, "disconnect appsrc (%s) - ren 0x%08x", ao->appsrc->name, ao->uid_appsrc);
        aimc_module_disconnect_output(ao->imc, ao->uid_appsrc, 0);
        aimc_module_disconnect_input(ao->imc, ao->uid_appsink, 0);
    }
    if (ao->uid_appsrc) {
        aimc_module_destroy(ao->imc, ao->uid_appsrc);
        ao->uid_appsrc = 0;
        ao->appsrc = NULL;
    }
    if (ao->uid_bypass_appsrc) {
        aimc_module_destroy(ao->imc, ao->uid_bypass_appsrc);
        ao->uid_bypass_appsrc = 0;
        ao->bypass_appsrc = NULL;
    }
    if (ao->uid_appsink) {
        aimc_module_destroy(ao->imc, ao->uid_appsink);
        ao->uid_appsink = 0;
        ao->appsink = NULL;
    }
    return 0;
}

/* @0x16c6c */
int lgao_connect_renderer(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x7e8, "context is null");
        return -1;
    }
    if (_lgao_connect_renderer(ao) < 0) {
        LOGE(0x7eb, "connection error, pipeline\n");
        return -1;
    }
    return 0;
}

/* @0x16cf8: RTS (rate/latency control) defaults per app type; user values are ms */
void _lgao_config_set_rts_param(lgao *ao, LGAO_APPTYPE_T mode)
{
    unsigned int opmode = ao->rts_params.opmode;
    unsigned int upper = ao->rts_params.upper_threshold;
    unsigned int lower = ao->rts_params.lower_threshold;
    unsigned int rec_upper = ao->rts_params.recover_upper;
    unsigned int rec_lower = ao->rts_params.recover_lower;
    unsigned int skipping = ao->rts_params.skipping_threshold;
    unsigned int rec_skipping = ao->rts_params.recover_skipping;
    unsigned int fast_freq = ao->rts_params.fast_freq;
    unsigned int normal_freq = ao->rts_params.normal_freq;
    unsigned int slow_freq = ao->rts_params.slow_freq;
    unsigned int mix_fade_len = ao->rts_params.mix_fade_len;
    unsigned int mix_wait_len = ao->rts_params.mix_wait_len;
    unsigned int level = ao->rts_params.buffering_level;
    aren_rts_param_t dst;

#define MS_OR(v, def) ((v) == UNSET ? (def) : RTS_MS(v))
    memset(&dst, 0, sizeof(dst));
    switch (mode) {
    case LGAO_APPTYPE_DIRECTMEDIA:
        dst.onoff = true;
        dst.opmode = opmode == UNSET ? 2 : opmode;
        dst.upper_threshold    = MS_OR(upper,        RTS_MS(80));
        dst.recover_upper      = MS_OR(rec_upper,    RTS_MS(60));
        dst.lower_threshold    = MS_OR(lower,        0);
        dst.recover_lower      = MS_OR(rec_lower,    RTS_MS(70));
        dst.skipping_threshold = MS_OR(skipping,     RTS_MS(400));
        dst.recover_skipping   = MS_OR(rec_skipping, RTS_MS(60));
        break;
    case LGAO_APPTYPE_RTC:
    case LGAO_APPTYPE_MIRACAST:
        dst.onoff = true;
        dst.opmode = opmode == UNSET ? 2 : opmode;
        dst.upper_threshold    = MS_OR(upper,        RTS_MS(200));
        dst.recover_upper      = MS_OR(rec_upper,    RTS_MS(100));
        dst.lower_threshold    = MS_OR(lower,        0);
        dst.recover_lower      = MS_OR(rec_lower,    RTS_MS(150));
        dst.skipping_threshold = MS_OR(skipping,     RTS_MS(400));
        dst.recover_skipping   = MS_OR(rec_skipping, RTS_MS(100));
        break;
    case LGAO_APPTYPE_AIRPLAY:
        dst.onoff = true;
        dst.opmode = opmode == UNSET ? 2 : opmode;
        dst.upper_threshold    = MS_OR(upper,        RTS_MS(15));
        dst.recover_upper      = MS_OR(rec_upper,    RTS_MS(5));
        dst.lower_threshold    = MS_OR(lower,        0);
        dst.recover_lower      = MS_OR(rec_lower,    RTS_MS(150));
        dst.skipping_threshold = MS_OR(skipping,     RTS_MS(200));
        dst.recover_skipping   = MS_OR(rec_skipping, RTS_MS(5));
        break;
    case LGAO_APPTYPE_R2R:
        /* everything follows the buffering level (default 200 ms) */
        dst.onoff = true;
        dst.opmode = opmode == UNSET ? 2 : opmode;
        if (level == UNSET) {
            level = 200;
            dst.upper_threshold    = RTS_MS(400);
            dst.recover_upper      = RTS_MS(100);
            dst.recover_lower      = RTS_MS(300);
            dst.skipping_threshold = RTS_MS(700);
        } else {
            dst.upper_threshold    = RTS_MS(level) + RTS_MS(200);
            dst.recover_upper      = RTS_MS(level) - RTS_MS(100);
            dst.recover_lower      = RTS_MS(level) + RTS_MS(100);
            dst.skipping_threshold = RTS_MS(level) + RTS_MS(500);
        }
        dst.recover_skipping = dst.recover_upper;
        dst.buffering_level = level;
        break;
    default:            /* UNKNOWN, JP4K, IDGAPLESS, APPCAST, SYE: RTS off (all zero) */
        goto out;
    }
#undef MS_OR
    dst.normal_freq  = normal_freq  == UNSET ? 48000 : normal_freq;
    dst.slow_freq    = slow_freq    == UNSET ? 48000 : slow_freq;
    dst.fast_freq    = fast_freq    == UNSET ? 47250 : fast_freq;
    dst.mix_fade_len = mix_fade_len == UNSET ? 8 : mix_fade_len;
    dst.mix_wait_len = mix_wait_len == UNSET ? 0x800 : mix_wait_len;
out:
    ioctl(ao->fd, AREN_SET_RTS_PARAM, &dst);
}

/* Preserve the shipped out-of-line memset, rather than GCC's NEON expansion. */
extern void *renderer_memset(void *, int, size_t) __asm__("memset");

/* @0x16fc4: PCM format + bypass selection.
 * pcm_bypass/pcm_multi_ch decide whether GET_BYPASS_UID later builds a PCM
 * or an ES bypass in the kernel. */
void _lgao_config_set_renderer_param(lgao *ao)
{
    LGAO_AUDIO_START_INFO_T *pAudioInfo = &ao->audioStartInfo;
    struct aren_pcm_info pcm_info;
    bool is_jp4k;

    renderer_memset(&pcm_info, 0, sizeof(pcm_info));
    is_jp4k = ao->appType == LGAO_APPTYPE_JP4K;
    ioctl(ao->fd, AREN_SET_JP4K_STREAM, &is_jp4k);

    pcm_info.bits_per_sample = pAudioInfo->bitsPerSample;
    pcm_info.channels = pAudioInfo->numberOfChannel;
    pcm_info.sampling_rate = pAudioInfo->freq;
    pcm_info.byte_align = pAudioInfo->bigEndianOfPcm;
    pcm_info.sign = pAudioInfo->unsignedOfPcm;
    pcm_info.clock_x4 = pAudioInfo->clock_x4;
    pcm_info.pcm_bypass = ao->pcm_bypass;
    pcm_info.pcm_multi_ch = ao->pcm_multi_ch;
    ioctl(ao->fd, AREN_SET_PCM_INFO, &pcm_info);
}

/* @0x17038: restore volume/fade state on a freshly opened renderer */
void _lgao_config_set_easevol(lgao *ao)
{
    unsigned int target = ao->targetVolume, duration = ao->duration;
    LGAO_EASE_TYPE_T fade = ao->fadeType;
    aren_ease_volume_t ease_volume = { 0x800000, 0, AUD_GAIN_DURATION_MODE_LINEAR };

    if (target != UNSET && duration != UNSET) {
        if (ao->currVolume != UNSET) {
            /* resume a fade captured by lgao_keep_fade */
            double cur = ao->currVolume / 100.0 * Q23_UNITY;
            ease_volume.volume = (unsigned int)cur;
            if (fade != LGAO_EASE_NONE) {
                ioctl(ao->fd, AREN_SET_EASE_VOLUME, &ease_volume);   /* jump to current */
            }
            ease_volume.duration = ao->remainDuration;
            ease_volume.volume = (unsigned int)(ao->targetVolume * Q23_UNITY / 100.0);
            ease_volume.type = _lgao_convert_ease_type(ao->fadeType);
            ioctl(ao->fd, AREN_SET_EASE_VOLUME, &ease_volume);
            return;
        }
        double tv = target / 100.0 * Q23_UNITY;
        ease_volume.volume = (unsigned int)tv;
        ease_volume.duration = duration;
        if (fade == LGAO_EASE_INCUBIC || fade == LGAO_EASE_OUTCUBIC) {
            ease_volume.type = _lgao_convert_ease_type(fade);
            ioctl(ao->fd, AREN_SET_EASE_VOLUME, &ease_volume);
            return;
        }
    }
    if (fade == LGAO_EASE_NONE)
        return;
    ioctl(ao->fd, AREN_SET_EASE_VOLUME, &ease_volume);
}

/* @0x17168 */
void lgao_add_rts(void *a)
{
    lgao *ao = a;
    int ret;

    LOGI(0x8fa, "add rts\n");
    if (!ao->rts_params.onoff)
        return;
    ret = ioctl(ao->fd, AREN_ADD_RTS);
    LOGI(0x901, "add rts done ret = %d\n", ret);
}

/* @0x171f8 */
int lgao_config(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x90c, "context is null");
        return -1;
    }
    if (ao->rendererState == LGAO_STATE_PLAY) {
        LOGD(0x90f, "adec(%s.%d) is played already...", ADEC_KIND(ao), ao->index);
        return 0;
    }
    if (ao->isNoDelayMode) {
        lgao_add_rts(ao);
        switch (ao->appType) {
        case LGAO_APPTYPE_RTC:
        case LGAO_APPTYPE_MIRACAST:
            LOGI(0x917, "Set RTS mode ON - MIRACAST");
            _lgao_config_set_rts_param(ao, LGAO_APPTYPE_RTC);
            break;
        case LGAO_APPTYPE_R2R:
            LOGI(0x91a, "Set R2R mode ON - R2R");
            _lgao_config_set_rts_param(ao, LGAO_APPTYPE_R2R);
            break;
        case LGAO_APPTYPE_AIRPLAY:
            LOGI(0x91d, "Set RTS mode ON - AIRPLAY");
            _lgao_config_set_rts_param(ao, LGAO_APPTYPE_AIRPLAY);
            break;
        case LGAO_APPTYPE_DIRECTMEDIA:
            LOGI(0x920, "Set RTS mode ON - DIRECTMEDIA");
            _lgao_config_set_rts_param(ao, LGAO_APPTYPE_DIRECTMEDIA);
            break;
        default:
            LOGI(0x923, "Set RTS mode ON - UNKNOWN");
            _lgao_config_set_rts_param(ao, LGAO_APPTYPE_UNKNOWN);
            break;
        }
    }
    _lgao_config_set_renderer_param(ao);
    _lgao_config_set_easevol(ao);
    ao->rendererState = LGAO_STATE_CONFIG;
    LOGI(0x92d, "%s.%d, codecType=%d, freq=%d, numberOfChannel=%d, bitsPerSample=%u",
         ADEC_KIND(ao), ao->index, ao->audioStartInfo.codecType, ao->audioStartInfo.freq,
         ao->audioStartInfo.numberOfChannel, ao->audioStartInfo.bitsPerSample);
    return 0;
}

/* @0x17444 */
int lgao_set_first_render(void *a, int type, int value)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x93b, "context is null");
        return -1;
    }
    LOGN(0x93d, "first render of type %d is changing %d -> %d", type, ao->isFirstRender[type],
         value);
    ao->isFirstRender[type] = value != 0;
    return 0;
}

/* @0x174dc */
int lgao_get_first_render(void *a, int type, int *value)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x949, "context is null");
        return -1;
    }
    *value = ao->isFirstRender[type];
    return 0;
}

/* @0x17540 */
int lgao_get_size_buffered(void *a, int type, int *value)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x955, "context is null");
        return -1;
    }
    *value = ao->bufferedSize[type];
    return 0;
}

/* @0x175a4 */
int lgao_get_delay(void *a, int *value)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x961, "context is null");
        return -1;
    }
    *value = ao->delay;
    return 0;
}

/* @0x17604: base = running-time base (ns), start = segment start (ns) */
int lgao_set_basetime(void *a, long long base, long long start)
{
    lgao *ao = a;
    unsigned int gstc = 0;

    if (!ao) {
        LOGE(0x97b, "context is null");
        return -1;
    }
    if (start == -1) {
        LOGD(0x980, "AREN got INVALID starttime");
        return -1;
    }
    ioctl(ao->fd, AREN_GET_GSTC, &gstc);
    ao->clockBasetime = (gstc - sns_to_pts(base)) & 0xfffffff;
    ao->streamBasetime = sns_to_pts(start) & 0xfffffff;
    LOGI(0x98a, "stc.base=%llu, pts.base=%llu", ao->clockBasetime, ao->streamBasetime);
    _lgao_set_basetime(ao);
    return 0;
}

/* @0x1780c: stub, always "unchanged" */
int lgao_ischanged_output_connect(void *a)
{
    if (!a) {
        LOGE(0x998, "context is null");
        return -1;
    }
    return 0;
}

/* @0x17868: cached only; reaches the kernel in lgao_config (SET_PCM_INFO) */
int lgao_set_pcm_bypass(void *a, int bypass)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x9ce, "context is null");
        return -1;
    }
    LOGN(0x9d0, "pcm bypass status of renderer [%d] changing %d -> %d", ao->fd,
         ao->pcm_bypass, bypass);
    ao->pcm_bypass = bypass != 0;
    return 0;
}

/* @0x17900 */
int lgao_get_pcm_bypass(void *a, int *bypass)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x9e4, "context is null");
        return -1;
    }
    *bypass = ao->pcm_bypass;
    return 0;
}

/* @0x17960: only caches ao+0x1d1 (saved export @0x27960). Feed routing and
 * explicit FIRST construction see it immediately; no ioctl, reconnect, or
 * isFirstRender invalidation. SET_PCM_INFO waits for a non-PLAY lgao_config;
 * GET_BYPASS_UID selects the kernel head separately, before data is fed. */
int lgao_set_pcm_multi_channel(void *a, int multi_ch)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x9f0, "context is null");
        return -1;
    }
    LOGN(0x9f2, "pcm bypass status of renderer [%d] changing %d -> %d", ao->fd,
         ao->pcm_multi_ch, multi_ch);
    ao->pcm_multi_ch = multi_ch != 0;
    return 0;
}

/* @0x179f8 */
int lgao_get_pcm_multi_channel(void *a, int *multi_ch)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0x9fe, "context is null");
        return -1;
    }
    *multi_ch = ao->pcm_multi_ch;
    return 0;
}

/* @0x17a58 */
int lgao_start_renderer(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xa0a, "context is null");
        return -1;
    }
    LOGI(0xa0c, "start renderer");
    ioctl(ao->fd, AREN_START, &ao->index);
    return 0;
}

/* @0x17ae4 */
int lgao_stop_renderer(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xa17, "context is null");
        return -1;
    }
    LOGI(0xa19, "stop renderer");
    ioctl(ao->fd, AREN_STOP, &ao->index);
    return 0;
}

/* @0x17b70 */
int lgao_close_renderer(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xa24, "context is null");
        return -1;
    }
    LOGI(0xa26, "Close ADEC(%s.%d), state:STOP", ADEC_KIND(ao), ao->index);
    ao->rendererState = LGAO_STATE_STOP;
    for (int i = LGAO_DATA_PCM; i <= LGAO_DATA_METADATA; i++) {
        ao->isFirstRender[i] = true;
        ao->bufferedSize[i] = 0;
        ao->prevTimestamp[i] = ~0ull;
    }
    close(ao->fd);
    ao->fd = -1;
    return 0;
}

/* @0x17c40 */
int lgao_open_renderer(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xa40, "context is null");
        return -1;
    }
    LOGI(0xa42, "open renderer");
    if (ao->index == -1) {
        LOGE(0xa4a, "audio index(%d) is invalid value.", -1);
        return -1;
    }
    if (_lgao_open_renderer(ao) < 0) {
        LOGE(0xa46, "open audio modules ERROR!!!");
        return -1;
    }
    ao->rendererState = LGAO_STATE_INIT;
    LOGI(0xa4f, "Adec(%s.%d) Sink State = LGAO_STATE_INIT", ADEC_KIND(ao), ao->index);
    return 0;
}

/* @0x17d6c */
int lgao_pause(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xa5a, "context is null");
        return -1;
    }
    if (ao->rendererState != LGAO_STATE_PLAY) {
        LOGD(0xa63, "Adec(%s.%d) Sink State = %d", ADEC_KIND(ao), ao->index, ao->rendererState);
        return -1;
    }
    if (lgao_stop_renderer(ao) < 0)
        LOGE(0xa5f, "stop audio modules ERROR!!!");
    ao->rendererState = LGAO_STATE_PAUSE;
    return 0;
}

/* @0x17e54 */
int lgao_flush(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xa6f, "context is null");
        return -1;
    }
    if (ao->index == -1) {
        LOGE(0xa70, "index is invalid");
        return -1;
    }
    if (ao->bypass_appsrc) {
        module_appsrc_push_event(ao->bypass_appsrc, aud_event_new_flush_async());
        pthread_mutex_lock(&ao->flush_mutex);
        ao->flush_done[0] = ao->flush_done[1] = 0;
        module_appsrc_push_event(ao->bypass_appsrc, aud_event_new_flush());
        pthread_mutex_unlock(&ao->flush_mutex);
    }
    if (ao->appsrc) {
        module_appsrc_push_event(ao->appsrc, aud_event_new_flush_async());
        pthread_mutex_lock(&ao->flush_mutex);
        ao->flush_done[0] = ao->flush_done[1] = 0;
        module_appsrc_push_event(ao->appsrc, aud_event_new_flush());
        pthread_mutex_unlock(&ao->flush_mutex);
    }
    return 0;
}

/* inlined into lgao_play */
static inline void _lgao_set_lipsync_param(lgao *ao, LGAO_CODEC_T codec_type)
{
    aren_lipsync_param_t lipsync_param = { 150, 150, 400, 5000, 5000 };

    if (codec_type == LGAO_CODEC_WMA_PRO)
        lipsync_param.lower_bound = lipsync_param.upper_bound = 250;
    if (ao->isNoDelayMode)
        lipsync_param.offset = 0;
    else if (ao->lipsyncOffset != 0xfffffff)
        lipsync_param.offset = ao->lipsyncOffset;

    if ((unsigned int)(ao->playSpeed - 250) < 750) {     /* 0.25x .. <1.0x */
        lipsync_param.lower_bound = lipsync_param.lower_bound * 1000 / ao->playSpeed;
        lipsync_param.upper_bound = lipsync_param.upper_bound * 1000 / ao->playSpeed;
        LOGI(0x6c5, "Set Lipsync's Lbound to %u & Ubound to %u", lipsync_param.lower_bound,
             lipsync_param.upper_bound);
    }
    LOGN(0x6c9, "Set Renderer Lipsync Param (offset=%u, delay=%d)", lipsync_param.offset,
         ao->delay);
    if (!ao->drop_renderer)
        ioctl(ao->fd, AREN_SET_LIPSYNC_PARAM, &lipsync_param);
}

/* @0x17f4c: the same for ES and PCM; the ES/PCM choice was already made by
 * SET_PCM_INFO (lgao_config) and GET_BYPASS_UID (_lgao_connect_renderer). */
int lgao_play(void *a)
{
    lgao *ao = a;
    bool isLipsyncOn = false;

    if (!ao) {
        LOGE(0xa91, "context is null");
        return -1;
    }
    if (ao->audioStartInfo.codecType == LGAO_CODEC_UNKNOWN) {
        LOGE(0xa95, "audio info is not set");
        return -1;
    }
    _lgao_set_playspeed(ao, ao->playSpeed);
    ioctl(ao->fd, AREN_SET_CLOCKTYPE, &ao->clock_type);
    _lgao_set_lipsync_param(ao, ao->audioStartInfo.codecType);
    if (!ao->vsp_mode)
        _lgao_set_basetime(ao);

    if (ao->isNoDelayMode)
        isLipsyncOn = false;
    else if (ao->lipsyncMode == LGAO_LIPSYNC_ENABLE)
        isLipsyncOn = true;
    else if (ao->lipsyncMode == LGAO_LIPSYNC_AUTO)
        isLipsyncOn = !ao->isMixer;
    ioctl(ao->fd, AREN_SET_ENABLE_LIPSYNC, &isLipsyncOn);

    LOGI(0xaad, "Lip-sync %s", isLipsyncOn ? "ON" : "OFF");
    LOGI(0xaae, "%s.%d, codecType=%d, freq=%d, numberOfChannel=%d, bitsPerSample=%u",
         ADEC_KIND(ao), ao->index, ao->audioStartInfo.codecType, ao->audioStartInfo.freq,
         ao->audioStartInfo.numberOfChannel, ao->audioStartInfo.bitsPerSample);
    LOGI(0xab4, "Play (%s.%d) Success ", ADEC_KIND(ao), ao->index);
    ao->rendererState = LGAO_STATE_PLAY;
    LOGI(0xab8, "Adec(%s.%d) Sink State = LGAO_STATE_PLAY", ADEC_KIND(ao), ao->index);
    return 0;
}

/* @0x182d0: the "first AU" events that configure both chains. The main appsrc
 * gets the PCM description; the bypass appsrc gets either an ES description
 * (es=true, codec string, IEC data rate) or, with pcm_bypass/pcm_multi_ch, the
 * PCM description with an acmod override. This explicit call always sends both
 * FIRSTs (unless dropped); only feed consults isFirstRender[outType]. Both push
 * returns are ignored and ALL type latches are then cleared. Switching data type
 * or either cached PCM transport flag does not itself resend FIRST. lgao_open,
 * close_renderer (PCM through METADATA only), EOS (PCM only), or set_first_render
 * can rearm it (set_first_render is exported, with no internal callsite).
 * flush/set_audio_info/mute do not. EOS skips rearming for mute,
 * trick rates or drop. FLUSH_STOP/SEGMENT and seamless seeks do not rearm FIRST;
 * format-changing caps and non-seamless track changes close/reopen (rearming).
 * Same-PCM caps with restart=1 take case 2 without reopening; codec/data_* alone
 * are not compared. Caps acmod/downmix changes and undrop call here explicitly,
 * ignoring latches.
 * Therefore preserving a previous MAT FIRST while a PCM flag is set cannot be
 * guaranteed: any such call replaces it with a codec-less PCM FIRST. With both
 * flags clear a replay is ES again, but uses CURRENT codec_str/data_*; changed
 * caps can replace MAT too. No preservation guard exists in the shipped code.
 * Saved export @0x282d0; no kernel head is created by this call. */
int lgao_push_first_event(void *a)
{
    lgao *ao = a;
    LGAO_AUDIO_START_INFO_T *info;
    struct aud_event *event, *event_bypass;
    char *str;

    if (!ao) {
        LOGE(0xaea, "context is null");
        return -1;
    }
    if (ao->index == -1) {
        LOGE(0xaeb, "index is invalid");
        return -1;
    }
    info = &ao->audioStartInfo;
    LOGI(0xaed, "dolby %d", info->dolby);
    LOGI(0xaee, "decoder downmix %d", info->downmix);
    LOGI(0xaf0, "%s.%d - PTS.base = %llu, STC.base = %llu, speed = %6.3f", ADEC_KIND(ao),
         ao->index, ao->streamBasetime, ao->clockBasetime, (double)(ao->playSpeed / 1000.0f));

    event = aud_event_newv_first("channels", aud_value_int, info->numberOfChannel,
                                 "bits-per-sample", aud_value_int, info->bitsPerSample,
                                 "rate", aud_value_int, info->freq,
                                 "acmod", aud_value_int, info->acmod,
                                 "check-downmix", aud_value_int, info->check_downmix,
                                 "dolby", aud_value_bool, info->dolby,
                                 "from-appsrc", aud_value_int, 1,
                                 "out-channels", aud_value_int, info->out_channels,
                                 NULL);
    if (info->gap_correction)
        aud_event_value_set_int(event, "gap-correction", info->gap_correction);
    if (ao->vsp_mode)
        aud_event_value_set_bool(event, "seamless-mode", true);
    if (info->downmix)
        aud_event_value_set_bool(event, "downmix", true);
    if (!event) {
        LOGE(0xb17, "first_event is NULL\n");
        return -1;
    }
    str = aud_event_str(event);
    if (str) {
        LOGN(0xb1d, "Event : %s", str);
        free(str);
    }
    if (info->lfe_enable)
        aud_event_value_set_bool(event, "lfe-enable", true);

    /* raw 0x282d0+: one ldrh tests both adjacent flags; PCM branch is the fallthrough */
    if (ao->pcm_bypass || ao->pcm_multi_ch) {
        LOGN(0xb26, "pcm_bypass or pcm_multi_ch\n");
        int acmod = info->numberOfChannel < 3 ? 2 : 7;

        event_bypass = aud_event_dup_and_set(event, "acmod", aud_value_int, acmod, NULL);
        if (!event_bypass) {
            LOGE(0xb2d, "first_event is NULL\n");
            goto fail;
        }
        aud_event_value_set_int(event, "acmod", acmod);
        LOGN(0xb33, "pcm_bypass/pcm_multi_ch acmod = %d\n", acmod);
        if (ao->pcm_bypass) {
            aud_event_value_set_bool(event, "ch-mapping-lfe-c", true);
            aud_event_value_set_bool(event_bypass, "ch-mapping-lfe-c", true);
        }
    } else {
        /* ES bypass: data_* describe the IEC 61937 stream (defaults 2ch/16bit/48k) */
        if (info->data_channels < 1)
            info->data_channels = 2;
        if (info->data_bps < 1)
            info->data_bps = 16;
        if (info->data_rate < 1)
            info->data_rate = 48000;
        event_bypass = aud_event_dup_and_set(event,
                                             "acmod", aud_value_int, info->acmod,
                                             "atmos", aud_value_bool, info->atmos,
                                             "es", aud_value_bool, 1,
                                             "codec", aud_value_str, info->codec_str,
                                             "channels", aud_value_int, info->data_channels,
                                             "bits-per-sample", aud_value_int, info->data_bps,
                                             "data-rate", aud_value_int, info->data_rate,
                                             "clock-x4", aud_value_bool, info->clock_x4,
                                             NULL);
        if (!event_bypass) {
            LOGE(0xb4e, "first_event is NULL\n");
            goto fail;
        }
        str = aud_event_str(event_bypass);
        if (str) {
            LOGN(0xb54, "Event BYPASSS : %s", str);
            free(str);
        }
    }

    if (ao->drop_renderer) {
        LOGN(0xb5a, "dropping... skip send first event... \n");
        aud_object_put(&event->object);
        aud_object_put(&event_bypass->object);
        return 0;
    }
    module_appsrc_push_event(ao->appsrc, event);
    module_appsrc_push_event(ao->bypass_appsrc, event_bypass);
    for (int i = 0; i < LGAO_DATA_TYPE_NUM; i++)
        ao->isFirstRender[i] = false;
    return 0;

fail:
    aud_object_put(&event->object);
    return -1;
}

static inline void _lgao_write_dump(lgao *ao, void *buff, unsigned int buffLength)
{
    if (!buff) {
        LOGD(0xac2, "buff parameter is NULL");
        return;
    }
    if (ao->pDumpAudioFD)
        fwrite(buff, 1, buffLength, ao->pDumpAudioFD);
}

/* @0x18828: PCM -> appsrc (and bypass_appsrc too when pcm_bypass/pcm_multi_ch);
 * ES -> bypass_appsrc only; metadata -> appsrc (+bypass with either PCM flag).
 * Saved export @0x28828: both destinations receive references to the SAME copied
 * aud_data, still type_pcm; this does not frame IEC or convert samples. Configure
 * the intended head before connecting, with matching FIRST/format and valid
 * appsrcs; changing a cached flag midstream is not a safe pipeline transition.
 * PCM-typed carrier -> ES retyping is supported statically by lipsync's output
 * store @0x600a257e..2589 when dataType==1, NOT demonstrated playback: PCM-mode FIRST
 * (no codec) resets dataType=0. Enabling duplication after an ES FIRST also sends
 * those bytes into the main PCM chain; it is not a bypass-only carrier API.
 * The lower-level module_appsrc_push_data can submit a separately allocated
 * type_pcm object straight to an already connected bypass_appsrc without this
 * feed/FIRST logic or graph edits. That is conditional on a still-valid borrowed
 * module, a still-effective matching ES/MAT FIRST and external serialization
 * against feed, FIRST and teardown; the sink exports no getter/direct route.
 * lgao hands one extra aud_data reference to each push and releases its local
 * reference afterwards (raw 0x1893e..18958, 0x18a4c..18a56, 0x18ab8..18ac2).
 * A new direct object would hand off its initial reference, or get once first
 * if retaining a local reference; module pointers are NOT aud_objects.
 * Saved runtime/idle-state.txt:1533 shows only dataType=0, not this transport.
 * Feed's byte-count return is not delivery proof (drops/push errors are hidden). */
int lgao_feed(void *a, void *buff, unsigned int buffLength, unsigned long long timestamp,
              LGAO_DATA_TYPE_T outType)
{
    lgao *ao = a;
    struct aud_data *buffer;

    if (!ao) {
        LOGE(0xb74, "context is null");
        return -1;
    }
    if (ao->index == -1) {
        LOGE(0xb75, "index is invalid");
        return -1;
    }
    if (buffLength == 0)
        return 0;

    if (outType == LGAO_DATA_PCM && ao->isEnableDump)
        _lgao_write_dump(ao, buff, buffLength);

    if (ao->drop_renderer) {
        LOGD(0xb7e, "drop...\n");
        goto done;
    }
    if (ao->isFirstRender[outType])
        lgao_push_first_event(ao);

    buffer = aud_data_new(buffLength);
    if (!buffer) {
        LOGE(0xb88, "can't create aud_data");
        return 0;
    }
    if (timestamp == ~0ull) {
        buffer->info.timestamp = ~0u;
        buffer->info.timestamp64 = ao->prevTimestamp[outType];
    } else {
        buffer->info.timestamp = ns_to_pts(timestamp);
        buffer->info.timestamp64 = timestamp;
        ao->prevTimestamp[outType] = timestamp;
    }
    LOGD(0xb94, "ts : %llu -> %llu, outType = %d", timestamp,
            buffer->info.timestamp64, outType);
    memcpy(buffer->data, buff, buffLength);

    if (outType == LGAO_DATA_PCM) {
        buffer->info.type = type_pcm;
        module_appsrc_push_data(ao->appsrc,
                                (struct aud_data *)aud_object_get(&buffer->object), true);
        if (ao->pcm_bypass || ao->pcm_multi_ch)
            module_appsrc_push_data(ao->bypass_appsrc,
                                    (struct aud_data *)aud_object_get(&buffer->object), true);
    } else if (outType == LGAO_DATA_ES) {
        buffer->info.type = type_es;
        module_appsrc_push_data(ao->bypass_appsrc,
                                (struct aud_data *)aud_object_get(&buffer->object), true);
    } else {
        buffer->info.type = outType == LGAO_DATA_METADATA2 ? type_meta_ext : type_meta;
        module_appsrc_push_data(ao->appsrc,
                                (struct aud_data *)aud_object_get(&buffer->object), true);
        if (ao->pcm_bypass || ao->pcm_multi_ch)
            module_appsrc_push_data(ao->bypass_appsrc,
                                    (struct aud_data *)aud_object_get(&buffer->object), true);
    }
    aud_object_put(&buffer->object);

done:
    ao->isFirstRender[outType] = false;
    ao->bufferedSize[outType] += buffLength;
    LOGD(0xbb1, "done");
    return buffLength;
}

/* @0x18b84 */
int lgao_audio_gap(void *a, unsigned long long start, unsigned long long end)
{
    lgao *ao = a;
    aren_gap_info_t param = { 0, 0 };

    if (!ao) {
        LOGE(0xbbc, "context is null");
        return -1;
    }
    LOGN(0xbbe, "gap info received before %lld, after %lld", start, end);
    param.start_pts = ns_to_pts(start) & 0xfffffff;
    param.end_pts = ns_to_pts(end) & 0xfffffff;
    ioctl(ao->fd, AREN_SET_GAP_INFO, &param);
    return 0;
}

/* @0x18ce4: cached; reaches the kernel as SET_BYPASS_FADER on next connect */
int lgao_set_game_mode(void *a, int game_mode)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xbcc, "context is null");
        return -1;
    }
    LOGN(0xbce, "Set game mode (%d -> %d)", ao->game_mode, game_mode);
    ao->game_mode = game_mode != 0;
    return 0;
}

/* @0x18d78 */
int lgao_get_game_mode(void *a, int *game_mode)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xbd9, "context is null");
        return -1;
    }
    *game_mode = ao->game_mode;
    return 0;
}

/* @0x18dd8 */
int lgao_get_vdec_current_pts(void *a, unsigned int vdec_port, unsigned int *pts)
{
    lgao *ao = a;
    aren_vdec_current_pts_t info;

    if (!ao) {
        LOGE(0xbe5, "context is null");
        return -1;
    }
    info.port = vdec_port;
    info.pts = 0;
    ioctl(ao->fd, AREN_GET_VDEC_CURRENT_PTS, &info);
    *pts = info.pts;
    return 0;
}

/* @0x18e48 */
int lgao_get_drop_renderer(void *a)
{
    lgao *ao = a;
    return ao->drop_renderer;
}

/* @0x18e50: "ES bypass & drop" handover. Dropping tears the whole renderer
 * down (stop, disconnect, close fd) and posts BYPASS_DONE; while dropped,
 * feed still clears its type latch and counts dropped bytes; explicit FIRST
 * constructs/releases events without clearing latches; EOS returns 1. Mute and
 * speed still cache values but skip their ioctls. Un-dropping rebuilds appsrcs,
 * replays PLAY only for a saved PLAY state, and ALWAYS pushes both FIRSTs even
 * if no latch is armed. Rebuild return codes are ignored in the shipped code. */
int lgao_set_drop_renderer(void *a, int is_drop)
{
    lgao *ao = a;
    lgao_mq_item msg = { LGAO_MSG_TYPE_EVENT, LGAO_EVENT_BYPASS_DONE };

    if (ao->drop_renderer == is_drop) {
        LOGD(0xc00, "same.. ignore... (drop as %s)\n", ao->drop_renderer ? "On" : "Off");
        return 0;
    }
    LOGN(0xc04, "set drop as %s\n", is_drop ? "On" : "Off");
    ao->drop_renderer = is_drop;

    if (is_drop) {
        ao->prev_state = ao->rendererState;
        lgao_stop_renderer(ao);
        lgao_disconnect_renderer(ao);
        lgao_close_renderer(ao);
        if (msgsnd(ao->callback_mq_id, &msg, sizeof(msg.event), 0) < 0)
            LOGE(0xc11, "msgsnd failed, msg: %05X", msg.event);
        return 0;
    }

    _lgao_open_renderer(ao);
    lgao_config(ao);
    _lgao_connect_renderer(ao);
    LOGN(0xc18, "rendererState %d ao %d", ao->prev_state, ao->rendererState);
    if (ao->prev_state == LGAO_STATE_PLAY) {
        lgao_play(ao);
        lgao_start_renderer(ao);
        LOGN(0xc1f, "push first event.. \n");
    }
    lgao_push_first_event(ao);
    ao->prev_state = ao->rendererState;
    return 0;
}

/* @0x19018 */
int lgao_set_queue_max_length(void *a, int queue_max_length)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xc2c, "context is null");
        return -1;
    }
    LOGN(0xc2e, "Set queue_max_length (%d -> %d)", ao->queue_max_length, queue_max_length);
    ao->queue_max_length = queue_max_length;
    return 0;
}

/* @0x190a4 */
int lgao_get_queue_max_length(void *a, int *queue_max_length)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xc3a, "context is null");
        return -1;
    }
    *queue_max_length = ao->queue_max_length;
    return 0;
}

/* @0x19104 */
int lgao_set_vsp_mode(void *a)
{
    lgao *ao = a;

    if (!ao) {
        LOGE(0xc46, "context is null");
        return -1;
    }
    LOGN(0xc48, "Set vsp (variable speed playback) mode");
    ao->vsp_mode = true;
    ioctl(ao->fd, AREN_SET_VSP_MODE);
    return 0;
}
