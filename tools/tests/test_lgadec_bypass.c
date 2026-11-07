#include <assert.h>
#include <stdlib.h>
#include <string.h>
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

static void _lgadec_callback_to_gst(lgadec *adec, LGADEC_EVENT_MSG_T event, void *data)
{
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
    puts("lgadec bypass: PASS (borrowed payload, synchronous ownership, timestamps, queued toggle, closed-fd gate)");
}