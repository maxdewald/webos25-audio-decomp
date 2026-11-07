#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include "libgstlgaudiodec/lgadec.h"

// @pkg glib-2.0
#define LOGE(...) ((void)0)
#define LOGN(...) ((void)0)
#define LOGI(...) ((void)0)
#define LOGD(...) ((void)0)
#define LOGT(...) ((void)0)
// @match src/libgstlgaudiodec/lgadec.c #define ADEC_USABLE[^\n]*

static struct {
    void *data;
    unsigned int length;
    unsigned long long timestamp;
    int result;
} delivered;

int lgadec_decoder_feed(void *adec, void *data, unsigned int length,
                        unsigned long long timestamp)
{
    delivered.data = data;
    delivered.length = length;
    delivered.timestamp = timestamp;
    return delivered.result;
}

// @match src/libgstlgaudiodec/lgadec.c enum audio_decoder_codec \{[^}]*\};
// @match src/libgstlgaudiodec/lgadec.c static const struct \{[^}]*\}\s*_codec_type\[\] = \{[^;]*\};
// @match src/libgstlgaudiodec/lgadec.c #define ADEC_IOW_SET_CODEC[ \t]+[^\n]*
// @match src/libgstlgaudiodec/lgadec.c typedef struct adec_codec_param \{[\s\S]*?\} adec_codec_param_t;
// @match src/libgstlgaudiodec/lgadec.c #define ADEC_IOW_SET_CODEC_PARAM[^\n]*
// @match src/libgstlgaudiodec/lgadec.c typedef struct \{[^}]*\} adec_(?:pcm|ac3|aac|mp3|mpegh|ac4|wma|ch)_param_t;
// @match src/libgstlgaudiodec/lgadec.c typedef adec_mpegh_param_t adec_opus_param_t;
// @match src/libgstlgaudiodec/lgadec.c #define UNSET_PARAM[^\n]*
// @match src/libgstlgaudiodec/lgadec.c #define PLAY_SPEED_VALID[^\n]*
// @match src/libgstlgaudiodec/lgadec.c #define ALLOC_PARAM[\s\S]*?while \(0\)

static enum audio_decoder_codec configured;
static unsigned int parameter_size, parameter_count;
static unsigned char parameters[64];

static int capture_ioctl(int fd, unsigned long request, const void *argument)
{
    if (request == ADEC_IOW_SET_CODEC) {
        configured = *(const enum audio_decoder_codec *)argument;
    } else {
        const adec_codec_param_t *parameter = argument;
        assert(request == ADEC_IOW_SET_CODEC_PARAM);
        assert(parameter->size <= sizeof(parameters));
        assert(parameter->size == 0 || parameter->param);
        parameter_size = parameter->size;
        parameter_count++;
        memset(parameters, 0xa5, sizeof(parameters));
        if (parameter->size)
            memcpy(parameters, parameter->param, parameter->size);
    }
    return -1;
}

int lgadec_decoder_set_audio_output_mode(void *adec, unsigned int mode) { return 0; }
#define ioctl capture_ioctl
// @extract src/libgstlgaudiodec/lgadec.c lgadec_decoder_set_codec_param lgadec_decoder_configure_decoder
#undef ioctl

// @extract src/libgstlgaudiodec/lgadec_codec.c _raw_feed

static void cleanup(void **data)
{
    free(*data);
    *data = NULL;
}

static SELECTED_CODEC_T codec_type_none = {.cleanup = cleanup, .feed = _raw_feed};
static SELECTED_CODEC_T codec_type_private = {.cleanup = cleanup};
static SELECTED_CODEC_T codec_type_pcm, codec_type_aac, codec_type_ac4, codec_type_adpcm,
    codec_type_flac, codec_type_vorbis, codec_type_opus, codec_type_iamf;
#define lgadec_pcm_create() NULL
#define lgadec_aac_create(format) NULL
#define lgadec_ac4_create(format) NULL
#define lgadec_adpcm_create(type) NULL
#define lgadec_flac_create() NULL
#define lgadec_vorbis_create() NULL
#define lgadec_opus_create() NULL
#define lgadec_iamf_create() NULL
// @extract src/libgstlgaudiodec/lgadec.c freq_in lgadec_is_available_bypass_output lgadec_select_codec lgadec_feed_data

static void rejected_selection(void)
{
    const LGADEC_CODEC_T rejected[] = {
        LGADEC_CODEC_UNKNOWN, LGADEC_CODEC_DRA, LGADEC_CODEC_RA8,
        LGADEC_CODEC_AAC_ENC, LGADEC_CODEC_AC3_ENC, LGADEC_CODEC_MP3_ENC,
        LGADEC_CODEC_MAT, LGADEC_CODEC_AAC_BYPASS, LGADEC_CODEC_AC3_IEC60958,
        LGADEC_CODEC_EAC3_ATMOS, LGADEC_CODEC_AC4_ATMOS, LGADEC_CODEC_TYPE_MAX,
        (LGADEC_CODEC_T)-1, (LGADEC_CODEC_T)INT_MAX,
    };
    for (unsigned int bypass = 0; bypass < 2; bypass++) {
        for (unsigned int codec = 0; codec < G_N_ELEMENTS(rejected); codec++) {
            lgadec adec = {.fd = bypass ? -1 : 3, .bypass_decoder = bypass,
                           .stream_info = {.freq = 48000, .channels = 2}};
            LGADEC_CODEC_INFO_T info = {.codec_type = LGADEC_CODEC_AC3,
                                      .codec_format = LGADEC_AC3_FORMAT};
            assert(lgadec_select_codec(&adec, &info) == 0);
            adec.codec_data = malloc(1);
            assert(adec.codec_data);
            info.codec_type = rejected[codec];
            assert(lgadec_select_codec(&adec, &info) == -1);
            assert(adec.selected_codec == NULL && adec.codec_data == NULL);
            assert(adec.codec_type == LGADEC_CODEC_AC3);
            assert(lgadec_feed_data(&adec, NULL, 0, 0) == -1);
        }
    }
}

static void supported_raw_selection(void)
{
    const LGADEC_CODEC_T codecs[] = {LGADEC_CODEC_AC3, LGADEC_CODEC_EAC3};
    const int rates[] = {0, 32000, 44100, 48000};
    unsigned char bytes[] = {0xf8, 0x72, 0x6f, 0xba, 0x7f, 0xfe, 0x80, 0x01};
    unsigned char original[sizeof(bytes)];
    memcpy(original, bytes, sizeof(bytes));
    for (unsigned int codec = 0; codec < G_N_ELEMENTS(codecs); codec++) {
        for (unsigned int rate = 0; rate < G_N_ELEMENTS(rates); rate++) {
            lgadec adec = {.fd = 3, .stream_info = {.freq = rates[rate], .channels = 6}};
            LGADEC_CODEC_INFO_T info = {.codec_type = codecs[codec],
                                      .codec_format = LGADEC_AC3_FORMAT};
            assert(lgadec_select_codec(&adec, &info) == 0);
            assert(adec.selected_codec == &codec_type_none && adec.no_dec == 0);
            assert(adec.codec_type == codecs[codec] && adec.wait_bypass_first_event);
            delivered.result = 0;
            assert(lgadec_feed_data(&adec, bytes, sizeof(bytes), 1234567890123ULL) == 0);
            assert(delivered.data == bytes && delivered.length == sizeof(bytes));
            assert(delivered.timestamp == 1234567890123ULL);
            assert(memcmp(bytes, original, sizeof(bytes)) == 0);
            assert(adec.fed_frame_cnt == 1);
            delivered.result = -1;
            assert(lgadec_feed_data(&adec, bytes, sizeof(bytes), 987654321ULL) == -1);
            assert(adec.fed_frame_cnt == 1);
            info.codec_format = LGADEC_AC3_PRIVATE_FORMAT;
            assert(lgadec_select_codec(&adec, &info) == 0);
            assert(adec.selected_codec == &codec_type_private);
            info.codec_format = LGADEC_RAW_FORMAT;
            assert(lgadec_select_codec(&adec, &info) == -1);
            assert(adec.selected_codec == NULL);
        }
        lgadec adec = {.fd = 3, .stream_info = {.freq = 96000, .channels = 2}};
        LGADEC_CODEC_INFO_T info = {.codec_type = codecs[codec],
                                  .codec_format = LGADEC_AC3_FORMAT};
        assert(lgadec_select_codec(&adec, &info) == -1);
        adec.stream_info.freq = 48000;
        adec.stream_info.channels = 7;
        assert(lgadec_select_codec(&adec, &info) == -1);
        adec.stream_info.channels = 2;
        adec.fd = -1;
        assert(lgadec_select_codec(&adec, &info) == -1);
        adec.bypass_decoder = true;
        assert(lgadec_select_codec(&adec, &info) == 0);
        assert(adec.selected_codec == &codec_type_none && adec.no_dec == 0);
    }
}

static void codec_mapping(void)
{
    const struct {
        LGADEC_CODEC_T input;
        enum audio_decoder_codec output;
    } cases[] = {
        {LGADEC_CODEC_AC3, AUDIO_DECODER_CODEC_AC3},
        {LGADEC_CODEC_EAC3, AUDIO_DECODER_CODEC_EAC3},
        {LGADEC_CODEC_MAT, AUDIO_DECODER_CODEC_MAT},
        {LGADEC_CODEC_EAC3_ATMOS, AUDIO_DECODER_CODEC_EAC3},
        {LGADEC_CODEC_AAC_ELD, AUDIO_DECODER_CODEC_UNKNOWN},
        {LGADEC_CODEC_DRA, AUDIO_DECODER_CODEC_DRA},
        {LGADEC_CODEC_OPUS, AUDIO_DECODER_CODEC_OPUS},
        {LGADEC_CODEC_AAC_ENC, AUDIO_DECODER_CODEC_UNKNOWN},
        {(LGADEC_CODEC_T)INT_MAX, AUDIO_DECODER_CODEC_UNKNOWN},
    };
    for (unsigned int index = 0; index < G_N_ELEMENTS(cases); index++) {
        lgadec adec = {.fd = 3, .codec_type = cases[index].input};
        assert(lgadec_decoder_configure_decoder(&adec) == 0);
        assert(configured == cases[index].output);
        assert(adec.decoder_state == LGADEC_DECODER_STATE_CONFIG);
    }
}

static void wma_parameters(void)
{
    unsigned char dsi[18] = {0};
    lgadec adec = {.fd = 3, .codec_type = LGADEC_CODEC_WMA_PRO,
                  .stream_info = {.freq = 48000, .channels = 6, .bits_per_sample = 24,
                                  .block_align = 2048, .avg_bytes_per_sec = 32000,
                                  .dsi_buffer = dsi}};
    const unsigned int tags[] = {0x160, 0x161, 0x162, 0x163};
    const unsigned int lengths[] = {4, 10, 18, 18};
    const unsigned int mask = 0x3f, advanced = 0x12345678;
    memcpy(dsi + 2, &mask, sizeof(mask));
    memcpy(dsi + 6, &advanced, sizeof(advanced));
    dsi[14] = 0x34;
    dsi[15] = 0x12;
    dsi[16] = 0x78;
    dsi[17] = 0x56;
    for (unsigned int index = 0; index < G_N_ELEMENTS(tags); index++) {
        adec_wma_param_t actual;
        adec_wma_param_t expected = {
            .sampling_rate = 48000, .channels = 6, .bits_per_sample = 24,
            .wma_format_tag = tags[index], .block_align = 2048, .avg_bps = 32000,
        };
        adec.stream_info.wma_format_tag = tags[index];
        adec.stream_info.dsi_buffer_length = lengths[index];
        if (index >= 2) {
            expected.channel_mask = mask;
            expected.encode_opt = 0x1234;
            expected.adv_encode_opt = 0x5678;
            expected.adv_encode_opt2 = advanced;
        } else {
            expected.encode_opt = index == 0 ? mask : dsi[4] & 0x3f;
        }
        assert(lgadec_decoder_set_codec_param(&adec) == 0);
        assert(parameter_size == sizeof(actual));
        memcpy(&actual, parameters, sizeof(actual));
        assert(memcmp(&actual, &expected, sizeof(actual)) == 0);
        unsigned int before = parameter_count;
        adec.stream_info.dsi_buffer_length--;
        assert(lgadec_decoder_set_codec_param(&adec) == -1);
        assert(parameter_count == before);
    }
}

int main(void)
{
    LGADEC_CODEC_INFO_T info = {.codec_type = LGADEC_CODEC_AC3,
                              .codec_format = LGADEC_AC3_FORMAT};
    assert(lgadec_select_codec(NULL, &info) == -1);
    rejected_selection();
    supported_raw_selection();
    codec_mapping();
    wma_parameters();
    puts("lgadec selection: PASS (codec gates, raw forwarding, mapping, WMA parameters and malformed DSI)");
}