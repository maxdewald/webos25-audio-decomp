#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @match src/libgstlgaudiodec/lgadec.h struct logmm_module \{[^}]*\};
// @match src/libgstlgaudiodec/lgadec.h enum log_level \{[^}]*\};
// @match src/libgstlgaudiodec/lgadec.h #define LOGM\b(?:[^\n]*\\\n)*[^\n]*

static unsigned int log_mask;
#define logmm_dummy_mask log_mask
// @match src/libgstlgaudiodec/lgadec_codec.c (?m)^(?:static )?struct logmm_module logmm_module_lgadec_codec = [^;]*;
// @match src/libgstlgaudiodec/lgadec_codec.c (?m)^#define LOG[ENT]\([^\n]*

static struct {
    enum log_level level;
    const char *function;
    int line;
    char message[160];
} logs[32];
static unsigned int log_count;

static int logmm_printl(struct logmm_module *module, enum log_level level,
                        const char *function, int line, const char *format, ...)
{
    va_list arguments;

    assert(module == &logmm_module_lgadec_codec);
    assert(strcmp(module->name, "lgadec_codec") == 0);
    assert(log_count < sizeof(logs) / sizeof(logs[0]));
    logs[log_count].level = level;
    logs[log_count].function = function;
    logs[log_count].line = line;
    va_start(arguments, format);
    vsnprintf(logs[log_count].message, sizeof(logs[log_count].message), format, arguments);
    va_end(arguments);
    log_count++;
    return 0;
}

typedef enum {
    LGADEC_UNKNOWN_FORMAT, LGADEC_RAW_FORMAT, LGADEC_AAC_ADTS_FORMAT,
    LGADEC_AAC_LOAS_FORMAT
} LGADEC_CODEC_FORMAT_T;
enum { LGADEC_APP_TYPE_APPCAST = 4 };
typedef struct {
    uint8_t *dsi_buffer;
    uint32_t dsi_buffer_length;
    uint8_t channels;
    int object_type;
} LGADEC_STREAM_INFO_T;
typedef struct {
    void *codec_data;
    LGADEC_STREAM_INFO_T stream_info;
    int app_type;
    uint64_t fed_frame_cnt;
} lgadec;

static struct {
    uint8_t data[96];
    uint32_t len;
    uint64_t timestamp;
} packets[4];
static unsigned int packet_count, allocations, frees, allocation_attempts;
static unsigned int fail_allocation, fail_feed;
static void *owned_buffer;
static LGADEC_CODEC_FORMAT_T selected_format;

static void *feed_malloc(size_t size)
{
    allocation_attempts++;
    if (allocation_attempts == fail_allocation)
        return NULL;
    assert(owned_buffer == NULL);
    owned_buffer = malloc(size);
    assert(owned_buffer != NULL);
    allocations++;
    return owned_buffer;
}

static void feed_free(void *buffer)
{
    assert(buffer == owned_buffer && buffer != NULL);
    free(buffer);
    owned_buffer = NULL;
    frees++;
}

static int lgadec_decoder_feed(lgadec *adec, void *buffer, uint32_t len, uint64_t timestamp)
{
    assert(packet_count < 4 && len <= sizeof(packets[0].data));
    memcpy(packets[packet_count].data, buffer, len);
    packets[packet_count].len = len;
    packets[packet_count].timestamp = timestamp;
    packet_count++;
    return packet_count == fail_feed ? -1 : 0;
}

static int lgadec_decoder_set_aac_format_type(lgadec *adec, LGADEC_CODEC_FORMAT_T format)
{
    selected_format = format;
    return 0;
}

// @match src/libgstlgaudiodec/lgadec_codec.c typedef struct \{[^}]*\} AAC_DATA_T;
// @extract src/libgstlgaudiodec/lgadec_codec.c _get_freq_idx
#define malloc feed_malloc
#define free feed_free
// @extract src/libgstlgaudiodec/lgadec_codec.c _aac_set_adts_length _aac_detect _get_aac_info _aac_first_data _aac_feed
#undef malloc
#undef free

static void reset_capture(void)
{
    assert(owned_buffer == NULL && allocations == frees);
    packet_count = allocations = frees = allocation_attempts = 0;
    fail_allocation = fail_feed = 0;
    log_count = 0;
    memset(packets, 0, sizeof(packets));
}

static void raw_headers(void)
{
    uint8_t config[] = {0x12, 0x10};
    uint8_t payload[] = {0x20, 0x31, 0x42, 0x53, 0x64, 0x75, 0x86, 0x97};
    AAC_DATA_T state = {0};
    lgadec adec = {.codec_data = &state, .stream_info = {
        .dsi_buffer = config, .dsi_buffer_length = sizeof(config), .channels = 1}};
    const uint8_t first[] = {
        0xff, 0xf1, 0x50, 0x80, 2, 0x60, 0,
        0xef, 0xbe, 0xad, 0xde, 0xbe, 0xba, 0xfe, 0xca,
        2, LGADEC_RAW_FORMAT, 0x12, 0x10
    };
    const uint8_t header[] = {0xff, 0xf1, 0x50, 0x80, 1, 0xe0, 0};

    reset_capture();
    assert(_aac_feed(&adec, payload, sizeof(payload), 0x100000002ULL) == 0);
    assert(selected_format == LGADEC_RAW_FORMAT && adec.stream_info.channels == 2);
    assert(packet_count == 2 && packets[0].len == sizeof(first));
    assert(memcmp(packets[0].data, first, sizeof(first)) == 0 && packets[0].timestamp == 0);
    assert(packets[1].len == sizeof(header) + sizeof(payload));
    assert(memcmp(packets[1].data, header, sizeof(header)) == 0);
    assert(memcmp(packets[1].data + 7, payload, sizeof(payload)) == 0);
    assert(packets[1].timestamp == 0x100000002ULL);
    assert(state.feed_cnt == 2 && adec.fed_frame_cnt == 1 && allocations == frees);
    reset_capture();
    assert(_aac_feed(&adec, payload, sizeof(payload), 456) == 0);
    assert(packet_count == 1 && packets[0].timestamp == 456 && state.feed_cnt == 3);
    assert(memcmp(packets[0].data, header, sizeof(header)) == 0);
    assert(memcmp(packets[0].data + 7, payload, sizeof(payload)) == 0);
}

static void bypass_and_fallback(void)
{
    uint8_t adts[] = {0xff, 0xf1, 0x50, 0x80, 1, 0, 0, 0x19};
    uint8_t loas[] = {0x56, 0xe0, 0x21, 0x32, 0x43, 0x54, 0x65};
    uint8_t raw[] = {0x21, 0x32, 0x43};
    uint8_t config[] = {0x12, 0x10};
    AAC_DATA_T state = {0};
    lgadec adec = {.codec_data = &state};
    uint8_t *payloads[] = {adts, loas, raw};
    uint32_t lengths[] = {sizeof(adts), sizeof(loas), sizeof(raw)};
    LGADEC_CODEC_FORMAT_T formats[] = {
        LGADEC_AAC_ADTS_FORMAT, LGADEC_AAC_LOAS_FORMAT, LGADEC_UNKNOWN_FORMAT
    };

    for (unsigned int index = 0; index < 3; index++) {
        reset_capture();
        memset(&state, 0, sizeof(state));
        assert(_aac_feed(&adec, payloads[index], lengths[index], 700 + index) == 0);
        assert(state.aac_format == formats[index] && state.feed_cnt == 1);
        assert(packet_count == 1 && packets[0].len == lengths[index]);
        assert(memcmp(packets[0].data, payloads[index], lengths[index]) == 0);
        assert(packets[0].timestamp == 700 + index && allocations == 0);
    }
    reset_capture();
    state = (AAC_DATA_T){.aac_format_from_demux = LGADEC_AAC_LOAS_FORMAT};
    adec.app_type = LGADEC_APP_TYPE_APPCAST;
    assert(_aac_feed(&adec, adts, sizeof(adts), 999) == 0);
    assert(state.aac_format == LGADEC_AAC_LOAS_FORMAT && packets[0].timestamp == 999);
    reset_capture();
    memset(&state, 0, sizeof(state));
    adec.stream_info.dsi_buffer = config;
    adec.stream_info.dsi_buffer_length = 1;
    assert(_aac_feed(&adec, raw, sizeof(raw), 123) == 0);
    assert(state.aac_format == LGADEC_UNKNOWN_FORMAT && packet_count == 1);
    assert(packets[0].len == sizeof(raw) && memcmp(packets[0].data, raw, sizeof(raw)) == 0);
    assert(_aac_feed(NULL, raw, sizeof(raw), 0) == -1);
    adec.codec_data = NULL;
    assert(_aac_feed(&adec, raw, sizeof(raw), 0) == -1);
}

static void failure_ownership(void)
{
    uint8_t config[] = {0x12, 0x10}, payload[] = {0x21, 0x32, 0x43};

    for (unsigned int allocation_failure = 0; allocation_failure < 2; allocation_failure++) {
        for (unsigned int failure = 1; failure <= 2; failure++) {
            AAC_DATA_T state = {0};
            lgadec adec = {.codec_data = &state, .stream_info = {
                .dsi_buffer = config, .dsi_buffer_length = sizeof(config), .channels = 2}};

            reset_capture();
            if (allocation_failure)
                fail_allocation = failure;
            else
                fail_feed = failure;
            assert(_aac_feed(&adec, payload, sizeof(payload), 987) == (failure == 1 ? 0 : -1));
            assert(state.feed_cnt == 1 && adec.fed_frame_cnt == (failure == 2));
            assert(owned_buffer == NULL && allocations == frees);
            if (failure == 1) {
                unsigned int last = packet_count - 1;
                assert(packets[last].len == sizeof(payload) + 7 && packets[last].timestamp == 987);
                assert(memcmp(packets[last].data + 7, payload, sizeof(payload)) == 0);
            } else {
                assert(packets[0].len == sizeof(config) + 17 && packets[0].timestamp == 0);
                assert(memcmp(packets[0].data + 17, config, sizeof(config)) == 0);
            }
        }
    }
    reset_capture();
}

static void zero_object_type(void)
{
    uint8_t config[] = {0x02, 0x10}, payload[] = {0x21, 0x32, 0x43};
    AAC_DATA_T state = {0};
    lgadec adec = {.codec_data = &state, .stream_info = {
        .dsi_buffer = config, .dsi_buffer_length = sizeof(config), .channels = 2}};

    reset_capture();
    assert(_aac_feed(&adec, payload, sizeof(payload), 543) == 0);
    assert(packet_count == 2 && packets[0].data[15] == 0);
    assert(packets[0].data[2] == 0x10 && packets[1].data[2] == 0x10);
    assert(packets[1].timestamp == 543 && state.feed_cnt == 2);
    reset_capture();
}

static void log_metadata(void)
{
    uint8_t config[] = {0x12, 0x10}, payload[] = {0x21, 0x32, 0x43};
    AAC_DATA_T state = {0};
    lgadec adec = {.codec_data = &state, .stream_info = {
        .dsi_buffer = config, .dsi_buffer_length = sizeof(config), .channels = 2}};
    const char *functions[] = {
        "_aac_detect", "_get_aac_info", "_aac_first_data", "_aac_first_data", "_aac_feed"
    };
    const int lines[] = {0x107, 0xf5, 0x18f, 0x191, 0x1c3};

    reset_capture();
    logmm_module_lgadec_codec.mask = &log_mask;
    log_mask = (1u << log_level_trace) | (1u << log_level_noti);
    assert(_aac_feed(&adec, payload, sizeof(payload), 123) == 0);
    assert(log_count == 5 && packet_count == 2 && state.feed_cnt == 2);
    for (unsigned int index = 0; index < log_count; index++) {
        assert(strcmp(logs[index].function, functions[index]) == 0);
        assert(logs[index].line == lines[index]);
        assert(logs[index].level == (index < 2 ? log_level_trace : log_level_noti));
    }
    assert(strcmp(logs[1].message, "objectType = 0x02, freqIndex = 0x04, ch = 0x02") == 0);
    assert(strcmp(logs[3].message, "dsi / total : 2 / 19\n") == 0);
    reset_capture();
    log_mask = 0;
    assert(_aac_feed(NULL, payload, sizeof(payload), 0) == -1 && log_count == 0);
    log_mask = 1u << log_level_error;
    assert(_aac_feed(NULL, payload, sizeof(payload), 0) == -1);
    assert(log_count == 1 && logs[0].level == log_level_error && logs[0].line == 0x1b3);
    assert(strcmp(logs[0].function, "_aac_feed") == 0);
    assert(strcmp(logs[0].message, "adec is null") == 0);
    log_mask = 0;
    reset_capture();
}

int main(void)
{
    raw_headers();
    bypass_and_fallback();
    failure_ownership();
    zero_object_type();
    log_metadata();
    puts("AAC feed: PASS (headers, payloads, timestamps, fallback, ownership, log metadata)");
}