#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include "libgstlgaudiodec/lgadec.h"

// @pkg glib-2.0
#define LOGE(...) ((void)0)
#define LOGN(...) ((void)0)
#define LOGI(...) ((void)0)
#define LOGD(...) ((void)0)
// @match src/libgstlgaudiodec/lgadec.c #define ADEC_USABLE[^\n]*

struct aud_object { int unused; };
struct aud_data {
    struct aud_object object;
    struct { int type; unsigned int timestamp; unsigned long long timestamp64; } info;
    void *data;
};
enum { type_es = 1 };
static struct aud_event *aud_event_new_first(void) { abort(); }
static void module_appsrc_push_event(struct aud_module *module, struct aud_event *event) { abort(); }
static struct aud_data *aud_data_new(unsigned int size) { abort(); }
static struct aud_object *aud_object_get(struct aud_object *object) { abort(); }
static void aud_object_put(struct aud_object *object) { abort(); }
static void module_appsrc_push_data(struct aud_module *module, struct aud_data *data, bool block) { abort(); }

static unsigned char *borrowed;
static unsigned char copied[16];
static unsigned int expected_size, expected_before;
static unsigned long long expected_timestamp;
static bool delivered;
static unsigned int preroll_events;
static unsigned int caps_updates, media_updates;
static LGADEC_AUDIO_CAPS_INFO published_caps;

static void _lgadec_callback_to_gst(lgadec *adec, LGADEC_EVENT_MSG_T event, void *data)
{
    if (event == LGADEC_EVENT_UPDATE_DECODED_INFO) {
        assert(data == &adec->audio_caps);
        published_caps = *(LGADEC_AUDIO_CAPS_INFO *)data;
        caps_updates++;
        return;
    }
    if (event == LGADEC_EVENT_SEND_MEDIA_INFO) {
        assert(data == NULL && caps_updates == media_updates + 1);
        media_updates++;
        return;
    }
    if (event == LGADEC_EVENT_PREROLL) {
        assert(data == NULL && adec->task_status == LGADEC_TASK_RUN);
        preroll_events++;
        return;
    }
    LGADEC_DATA_T *output = data;
    assert(event == LGADEC_EVENT_PUSH_DATA);
    assert(output->out_type == LGADEC_DATA_BYPASS && output->out_type != LGADEC_DATA_ES);
    assert(output->data == borrowed && output->size == expected_size);
    assert(output->timestamp64 == expected_timestamp);
    assert(adec->feeded_size == expected_before && adec->use_decoder_feed == 0);
    assert(pthread_mutex_trylock(&adec->resource_mutex) == 0);
    assert(pthread_mutex_unlock(&adec->resource_mutex) == 0);
    assert(output->size <= sizeof(copied));
    memcpy(copied, output->data, output->size);
    delivered = true;
    free(output);
}

// @extract src/libgstlgaudiodec/lgadec.c lgadec_set_es_bypass lgadec_decoder_feed
// @extract src/libgstlgaudiodec/lgadec.c _lgadec_set_audio_mode _lgadec_process_decoded_info

struct aimc { unsigned int processor; };
struct registry { struct aimc *imc; };
struct aud_connector { struct aimc *imc; };
static unsigned int init_failure, live_resources;

static struct aimc *aimc_create(void)
{
    if (init_failure == 0)
        return NULL;
    struct aimc *imc = malloc(sizeof(*imc));
    assert(imc);
    imc->processor = 17;
    live_resources |= 1;
    return imc;
}

static int aimc_create_handler(struct aimc *imc)
{
    assert(imc && (live_resources & 1));
    return 0;
}

static struct registry *ModuleRegistry_Init(struct aimc *imc)
{
    if (init_failure == 1)
        return NULL;
    struct registry *reg = malloc(sizeof(*reg));
    assert(reg);
    reg->imc = imc;
    live_resources |= 2;
    return reg;
}

static unsigned int aimc_get_processor_id(struct aimc *imc) { return imc->processor; }

static struct aud_connector *Connector_Create(unsigned int processor, struct aimc *imc)
{
    assert(processor == imc->processor && (live_resources & 3) == 3);
    if (init_failure == 2)
        return NULL;
    struct aud_connector *con = malloc(sizeof(*con));
    assert(con);
    con->imc = imc;
    live_resources |= 4;
    return con;
}

static void ModuleRegistry_Cleanup(struct registry *reg)
{
    assert(reg->imc && (live_resources & 3) == 3 && !(live_resources & 4));
    live_resources &= ~2u;
    free(reg);
}

static void aimc_finalize(struct aimc *imc)
{
    assert(imc && live_resources == 1);
    live_resources = 0;
    free(imc);
}

// @extract src/libgstlgaudiodec/lgadec.c _lgadec_aimc_init

static void aimc_ownership(void)
{
    for (init_failure = 0; init_failure < 4; init_failure++) {
        lgadec adec = {0};
        assert(live_resources == 0);
        _lgadec_aimc_init(&adec);
        if (init_failure < 3) {
            assert(live_resources == 0 && !adec.imc && !adec.reg && !adec.con);
        } else {
            assert(live_resources == 7 && adec.imc && adec.reg && adec.con);
            assert(adec.reg->imc == adec.imc && adec.con->imc == adec.imc);
            free(adec.con);
            live_resources &= ~4u;
            ModuleRegistry_Cleanup(adec.reg);
            aimc_finalize(adec.imc);
        }
    }
}

static void caps_publication(void)
{
    const LGADEC_AUDIO_CAPS_INFO pcm = {
        .freq = 48000, .channels = 2, .bits_per_sample = 16, .acmod = 2,
        .dolby = true, .channel_mode = 2, .out_channels = 2,
    };
    const LGADEC_AUDIO_CAPS_INFO es = {
        .es = true, .data_channels = 2, .data_rate = 192000, .data_bps = 16,
        .codec_str = "EAC3",
    };
    for (unsigned int es_first = 0; es_first < 2; es_first++) {
        lgadec adec = {.fd = 3, .codec_type = LGADEC_CODEC_AC3, .audio_mode = 5,
                       .wait_bypass_first_event = true};
        caps_updates = media_updates = 0;
        for (unsigned int index = 0; index < 2; index++) {
            bool is_es = index == 0 ? es_first : !es_first;
            LGADEC_AUDIO_CAPS_INFO caps = is_es ? es : pcm;
            adec.first_event_done[is_es ? LGADEC_DATA_ES : LGADEC_DATA_PCM] = true;
            assert(_lgadec_process_decoded_info(&adec, &caps, is_es) == 0);
            assert(caps_updates == index && media_updates == index);
            assert(adec.caps_is_sent == (index != 0));
        }
        assert(published_caps.freq == 48000 && published_caps.channels == 2);
        assert(published_caps.bits_per_sample == 16 && published_caps.sign == 1);
        assert(published_caps.data_rate == 192000 && published_caps.data_bps == 16);
        assert(published_caps.es && strcmp(published_caps.codec_str, "EAC3") == 0);
        assert(adec.audio_mode == 2 && adec.audio_mode_changed && !adec.caps_is_updated);
        for (unsigned int index = 0; index < LGADEC_DATA_TYPE_NUM; index++)
            assert(!adec.first_event_done[index]);

        LGADEC_AUDIO_CAPS_INFO caps = pcm;
        adec.first_event_done[LGADEC_DATA_PCM] = true;
        assert(_lgadec_process_decoded_info(&adec, &caps, false) == 0);
        caps = es;
        adec.first_event_done[LGADEC_DATA_ES] = true;
        assert(_lgadec_process_decoded_info(&adec, &caps, true) == 0);
        assert(caps_updates == 1 && media_updates == 1);
        caps = pcm;
        caps.freq = 44100;
        adec.wait_bypass_first_event = false;
        adec.first_event_done[LGADEC_DATA_PCM] = true;
        assert(_lgadec_process_decoded_info(&adec, &caps, false) == 0);
        assert(caps_updates == 2 && media_updates == 2 && published_caps.freq == 44100);
    }
}

static int failed_read_fd = -1;
static ssize_t failed_read_result;
static bool fail_poll;

static int task_poll(struct pollfd *descriptors, nfds_t count, int timeout)
{
    if (fail_poll) {
        errno = EINTR;
        return -1;
    }
    return poll(descriptors, count, timeout);
}

static ssize_t task_read(int fd, void *buffer, size_t size)
{
    if (fd == failed_read_fd) {
        failed_read_fd = -1;
        errno = EIO;
        return failed_read_result;
    }
    return read(fd, buffer, size);
}

int lgadec_set_bypass_decoder(void *adec, bool on) { abort(); }
#define poll task_poll
#define read task_read
// @extract src/libgstlgaudiodec/lgadec.c lgadec_task_run
#undef read
#undef poll

static void task_ordering(void)
{
    const char *messages[] = {"preroll", "wrong", "pre"};
    for (unsigned int index = 0; index < G_N_ELEMENTS(messages); index++) {
        for (int failure = -1; failure <= 2; failure++) {
            lgadec adec = {.task_status = LGADEC_TASK_RUN};
            assert(pipe(adec.pipe) == 0 && pipe(adec.preroll_pipe) == 0);
            assert(write(adec.preroll_pipe[1], messages[index], strlen(messages[index])) ==
                   (ssize_t)strlen(messages[index]));
            assert(write(adec.pipe[1], "stop", 4) == 4);
            preroll_events = 0;
            failed_read_fd = failure < 0 ? -1 :
                (failure == 2 ? adec.pipe[0] : adec.preroll_pipe[0]);
            failed_read_result = failure == 0 ? 0 : -1;
            assert(lgadec_task_run(&adec) == NULL);
            assert(adec.task_status == LGADEC_TASK_STOP);
            assert(preroll_events == (unsigned int)(index == 0 && (failure < 0 || failure == 2)));
            assert(close(adec.pipe[0]) == 0 && close(adec.pipe[1]) == 0);
            assert(close(adec.preroll_pipe[0]) == 0 && close(adec.preroll_pipe[1]) == 0);
        }
    }
    lgadec adec = {.task_status = LGADEC_TASK_RUN};
    fail_poll = true;
    assert(lgadec_task_run(&adec) == NULL);
    assert(adec.task_status == LGADEC_TASK_RUN);
    fail_poll = false;
}

static int join_result;

static int task_join(pthread_t task, void **result)
{
    memcpy(result, &(int){23}, sizeof(int));
    return join_result;
}

#define pthread_join task_join
// @extract src/libgstlgaudiodec/lgadec.c lgadec_task_stop
#undef pthread_join

static void task_stop_ownership(void)
{
    lgadec adec = {0};
    char command[4];
    assert(pipe(adec.pipe) == 0);
    assert(pthread_mutex_init(&adec.pipe_mutex, NULL) == 0);
    assert(lgadec_task_stop(&adec) == 0 && adec.task_status == LGADEC_TASK_NONE);
    for (unsigned int failure = 0; failure < 2; failure++) {
        adec.task_status = LGADEC_TASK_RUN;
        join_result = failure ? EINVAL : 0;
        assert(lgadec_task_stop(&adec) == (failure ? -1 : 0));
        assert(read(adec.pipe[0], command, sizeof(command)) == sizeof(command));
        assert(memcmp(command, "stop", sizeof(command)) == 0);
        assert(adec.task_status == (failure ? LGADEC_TASK_RUN : LGADEC_TASK_NONE));
        assert(pthread_mutex_trylock(&adec.pipe_mutex) == 0);
        assert(pthread_mutex_unlock(&adec.pipe_mutex) == 0);
    }
    assert(close(adec.pipe[0]) == 0 && close(adec.pipe[1]) == 0);
    assert(pthread_mutex_destroy(&adec.pipe_mutex) == 0);
}

// @match src/libgstlgaudiodec/lgadec.c #define ADEC_IO_STOP[ \t]+[^\n]*
static unsigned int decoder_stops;

static int stop_ioctl(int fd, unsigned long request, const int *index)
{
    assert(fd == 3 && request == ADEC_IO_STOP && *index == 7);
    decoder_stops++;
    return -1;
}

#define ioctl stop_ioctl
// @extract src/libgstlgaudiodec/lgadec.c lgadec_decoder_stop
#undef ioctl

static void decoder_stop_gates(void)
{
    lgadec adec = {.fd = -1, .index = 7, .decoder_state = LGADEC_DECODER_STATE_START};
    assert(lgadec_decoder_stop(NULL) == -1);
    assert(lgadec_decoder_stop(&adec) == -1);
    assert(adec.decoder_state == LGADEC_DECODER_STATE_START && decoder_stops == 0);
    adec.bypass_decoder = true;
    assert(lgadec_decoder_stop(&adec) == 0);
    assert(adec.decoder_state == LGADEC_DECODER_STATE_STOP && decoder_stops == 0);
    adec.fd = 3;
    adec.bypass_decoder = false;
    assert(lgadec_decoder_stop(&adec) == 0);
    assert(adec.decoder_state == LGADEC_DECODER_STATE_STOP && decoder_stops == 1);
}

static void feed_borrowed(lgadec *adec, unsigned char *bytes, unsigned int size,
                          unsigned long long timestamp)
{
    borrowed = bytes;
    expected_size = size;
    expected_timestamp = timestamp;
    expected_before = adec->feeded_size;
    delivered = false;
    assert(lgadec_decoder_feed(adec, bytes, size, timestamp) == 0);
    assert(delivered && memcmp(bytes, copied, size) == 0);
    assert(adec->feeded_size == expected_before + size);
    assert(adec->use_decoder_feed == 0 && adec->fd == -1);
}

int main(void)
{
    lgadec adec = {.fd = -1, .prev_timestamp = {[LGADEC_DATA_BYPASS] = ~0ull}};
    unsigned char bytes[] = {0xf8, 0x72, 0x6f, 0xba, 0x7f, 0xfe, 0x80, 0x01};
    char command[4];
    assert(pthread_mutex_init(&adec.resource_mutex, NULL) == 0);
    assert(pthread_mutex_init(&adec.pipe_mutex, NULL) == 0);
    assert(lgadec_set_es_bypass(NULL, true) == -1);
    assert(lgadec_decoder_feed(NULL, bytes, sizeof(bytes), 1) == -1);
    assert(lgadec_decoder_feed(&adec, bytes, sizeof(bytes), 1) == -1);
    assert(!delivered && adec.feeded_size == 0);

    assert(lgadec_set_es_bypass(&adec, true) == 0);
    feed_borrowed(&adec, bytes, sizeof(bytes), ~0ull);
    assert(adec.prev_timestamp[LGADEC_DATA_BYPASS] == ~0ull);
    feed_borrowed(&adec, bytes, sizeof(bytes), 1234567890123ull);
    assert(adec.prev_timestamp[LGADEC_DATA_BYPASS] == 1234567890123ull);
    bytes[0] ^= 0xff;
    assert(copied[0] != bytes[0]);
    feed_borrowed(&adec, bytes, sizeof(bytes), ~0ull);
    assert(adec.prev_timestamp[LGADEC_DATA_BYPASS] == 1234567890123ull);

    assert(pipe(adec.pipe) == 0);
    adec.inited = true;
    assert(lgadec_set_es_bypass(&adec, false) == 0);
    assert(read(adec.pipe[0], command, sizeof(command)) == sizeof(command));
    assert(memcmp(command, "SUB0", sizeof(command)) == 0);
    assert(adec.bypass_decoder);
    feed_borrowed(&adec, bytes, sizeof(bytes), 99);
    assert(adec.prev_timestamp[LGADEC_DATA_BYPASS] == 99);
    adec.inited = false;
    assert(lgadec_set_es_bypass(&adec, false) == 0);
    delivered = false;
    expected_before = adec.feeded_size;
    assert(lgadec_decoder_feed(&adec, bytes, sizeof(bytes), 100) == -1);
    assert(!delivered && adec.feeded_size == expected_before);
    assert(adec.prev_timestamp[LGADEC_DATA_BYPASS] == 99);

    assert(close(adec.pipe[0]) == 0 && close(adec.pipe[1]) == 0);
    assert(pthread_mutex_destroy(&adec.resource_mutex) == 0);
    assert(pthread_mutex_destroy(&adec.pipe_mutex) == 0);
    task_ordering();
    task_stop_ownership();
    decoder_stop_gates();
    caps_publication();
    aimc_ownership();
    puts("lgadec bypass: PASS (borrowed payload, task ordering, I/O failures, PCM/ES caps, aimc ownership)");
}