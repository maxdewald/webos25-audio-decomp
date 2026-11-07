/* DVD-LPCM and ADPCM repacking from lgadec_codec.c with a minimal lgadec. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// @match src/libgstlgaudiodec/lgadec.h struct logmm_module \{[^}]*\};
// @match src/libgstlgaudiodec/lgadec.h enum log_level \{[^}]*\};
// @match src/libgstlgaudiodec/lgadec.h #define LOGM\b(?:[^\n]*\\\n)*[^\n]*
static unsigned int log_mask;
#define logmm_dummy_mask log_mask
// @match src/libgstlgaudiodec/lgadec_codec.c (?m)^(?:static )?struct logmm_module logmm_module_lgadec_codec = [^;]*;
// @match src/libgstlgaudiodec/lgadec_codec.c (?m)^#define LOG[EWIT]\([^\n]*
static struct {
    enum log_level level;
    const char *function;
    int line;
    const char *format;
} logs[4];
static unsigned int log_count;

static int logmm_printl(struct logmm_module *module, enum log_level level,
                        const char *function, int line, const char *format, ...)
{
    assert(module == &logmm_module_lgadec_codec);
    assert(strcmp(module->name, "lgadec_codec") == 0);
    assert(log_count < sizeof(logs) / sizeof(logs[0]));
    logs[log_count].level = level;
    logs[log_count].function = function;
    logs[log_count].line = line;
    logs[log_count].format = format;
    log_count++;
    return 0;
}

typedef enum {
    LGADEC_ADPCM_IMA_QT = 1, LGADEC_ADPCM_IMA_WAV = 2, LGADEC_ADPCM_MS = 7
} LGADEC_ADPCM_T;
typedef struct {
    void *codec_data;
    struct { unsigned int block_align, channels, freq, bits_per_sample; } stream_info;
} lgadec;
// @match src/libgstlgaudiodec/lgadec_codec.c typedef struct \{[^}]*\} ADPCM_DATA_T;
// @match src/libgstlgaudiodec/lgadec_codec.c typedef struct \{[^}]*\} PCM_DATA_T;
// @match src/libgstlgaudiodec/lgadec_codec.c static const [^;]*\b_adpcm_(?:index|step|adapt)_table\[[^;]*;
// @match src/libgstlgaudiodec/lgadec_codec.c static const [^;]*\b_adpcm_adapt_coeff_[12]\[[^;]*;
// @extract src/libgstlgaudiodec/lgadec_codec.c clip_int16 _adpcm_decoder _adpcm_ms_expand_nibble rd16 lgadec_adpcm_resampling lgadec_dvdlpcm_header_check lgadec_dvdlpcm_resampling
// @extract src/libgstlgaudiodec/lgadec_codec.c _bdlpcm_parse_header lgadec_bdlpcm_header_check
// @extract src/libgstlgaudiodec/lgadec_codec.c _wifilpcm_parse_header lgadec_wifilpcm_header_check
// @extract src/libgstlgaudiodec/lgadec_codec.c lgadec_alaw_resampling
static void alaw_samples(void)
{
    unsigned char input[256];
    short output[257];
    int decoder;

    for (unsigned int index = 0; index < 256; index++)
        input[index] = index;
    output[256] = 1234;
    assert(lgadec_alaw_resampling(&decoder, input, sizeof(input), output) == 0);
    for (unsigned int index = 0; index < 256; index++) {
        unsigned int value = index ^ 0x55;
        unsigned int segment = (value >> 4) & 7;
        int magnitude = ((value & 15) << 4) + (segment ? 264 : 8);

        if (segment > 1)
            magnitude <<= segment - 1;
        assert(output[index] == (value & 0x80 ? magnitude : -magnitude));
    }
    assert(output[256] == 1234);
    assert(lgadec_alaw_resampling(&decoder, NULL, 0, NULL) == 0);
    assert(lgadec_alaw_resampling(NULL, input, sizeof(input), output) == -1);
}

static void wifi_headers(void)
{
    for (unsigned int stream = 0; stream < 2; stream++) {
        for (unsigned int frames = 0; frames < 256; frames++) {
            for (unsigned int fields = 0; fields < 256; fields++) {
                PCM_DATA_T pcm = {0};
                lgadec adec = {.codec_data = &pcm};
                uint8_t data[] = {0xee, stream ? 0xa0 : 0xa1, frames, 0x39, fields};
                unsigned int rate = (fields >> 3) & 7;
                int valid = stream && (fields >> 6) == 0 &&
                    (rate == 1 || rate == 2) && (fields & 7) <= 1;
                int flag = -1;

                assert(lgadec_wifilpcm_header_check(&adec, data, 1, &flag) == (valid ? 0 : -1));
                if (!valid) {
                    assert(flag == 0 && pcm.header == 0 && adec.stream_info.freq == 0);
                    continue;
                }
                assert(flag == 2 && pcm.payload_size == frames * 320);
                assert(pcm.frequency == (rate == 1 ? 44100 : 48000));
                assert(pcm.width == 16 && pcm.out_width == 16 && pcm.channels == 2);
                assert(adec.stream_info.freq == pcm.frequency && adec.stream_info.channels == 2);
                assert(adec.stream_info.bits_per_sample == 16);
                uint32_t saved_header = pcm.header;

                data[3] ^= 0xff;
                assert(lgadec_wifilpcm_header_check(&adec, data, 1, &flag) == 0 && flag == 0);
                data[2] ^= 1;
                assert(lgadec_wifilpcm_header_check(&adec, data, 1, &flag) == 0 && flag == 1);
                assert(pcm.header == saved_header && pcm.payload_size == frames * 320);
            }
        }
    }
    assert(lgadec_wifilpcm_header_check(NULL, NULL, 0, NULL) == -1);
}

static void wifi_log_metadata(void)
{
    const uint8_t headers[][4] = {
        {0xa1, 1, 0, 0x10}, {0xa0, 1, 0, 0x50},
        {0xa0, 1, 0, 0}, {0xa0, 1, 0, 0x12}
    };
    const int lines[] = {0x6ee, 0x6f9, 0x708, 0x714};
    const char *messages[] = {
        "sub stream ID is not 0xA0", "width info is error",
        "frequency info is error", "channel info is error"
    };

    log_mask = 1u << log_level_error;
    for (unsigned int index = 0; index < 4; index++) {
        PCM_DATA_T pcm = {0};
        lgadec adec = {.codec_data = &pcm};
        uint8_t data[4];
        int flag = -1;

        memcpy(data, headers[index], sizeof(data));
        log_count = 0;
        assert(lgadec_wifilpcm_header_check(&adec, data, 0, &flag) == -1);
        assert(flag == 0 && pcm.header == 0 && log_count == 2);
        assert(logs[0].level == log_level_error && logs[0].line == lines[index]);
        assert(strcmp(logs[0].function, "_wifilpcm_parse_header") == 0);
        assert(strcmp(logs[0].format, messages[index]) == 0);
        assert(logs[1].level == log_level_error && logs[1].line == 0x750);
        assert(strcmp(logs[1].function, "lgadec_wifilpcm_header_check") == 0);
        assert(strcmp(logs[1].format, "_wifilpcm_parse_header() Err") == 0);
    }
    PCM_DATA_T pcm = {0};
    lgadec adec = {.codec_data = &pcm};
    uint8_t data[] = {0xa0, 3, 0, 0x10};
    int flag = -1;

    log_count = 0;
    log_mask = 1u << log_level_trace;
    assert(lgadec_wifilpcm_header_check(&adec, data, 0, &flag) == 0);
    assert(flag == 2 && pcm.payload_size == 960 && log_count == 1);
    assert(logs[0].level == log_level_trace && logs[0].line == 0x6f4);
    assert(strcmp(logs[0].function, "_wifilpcm_parse_header") == 0);
    assert(strcmp(logs[0].format, "number of audio frame is %u") == 0);
    log_count = 0;
    data[1]++;
    log_mask = 1u << log_level_warning;
    assert(lgadec_wifilpcm_header_check(&adec, data, 0, &flag) == 0);
    assert(flag == 1 && pcm.payload_size == 960 && log_count == 1);
    assert(logs[0].level == log_level_warning && logs[0].line == 0x75d);
    assert(strcmp(logs[0].function, "lgadec_wifilpcm_header_check") == 0);
    assert(strcmp(logs[0].format, "wifi LPCM header is different") == 0);
    log_mask = 0;
    log_count = 0;
}

static void bd_headers(void)
{
    const unsigned int channels[] = {0, 1, 0, 2, 3, 3, 4, 4, 5, 6, 7, 8, 0, 0, 0, 0};
    const unsigned int rates[] = {0, 48000, 0, 0, 96000, 192000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (unsigned int assignment = 0; assignment < 16; assignment++) {
        for (unsigned int rate = 0; rate < 16; rate++) {
            for (unsigned int width = 0; width < 4; width++) {
                PCM_DATA_T pcm = {0};
                lgadec adec = {.codec_data = &pcm};
                uint8_t data[] = {0xee, 0x12, 0x34, assignment << 4 | rate, width << 6 | 0x20};
                int flag = -1;
                int valid = channels[assignment] && rates[rate] && width;

                assert(lgadec_bdlpcm_header_check(&adec, data, 1, &flag) == (valid ? 0 : -1));
                if (!valid) {
                    assert(flag == 0 && pcm.header == 0);
                    assert(adec.stream_info.freq == 0 && adec.stream_info.channels == 0);
                    continue;
                }
                assert(flag == 2 && pcm.payload_size == 0x1234);
                assert(pcm.channels == channels[assignment] && pcm.frequency == rates[rate]);
                assert(pcm.width == 12 + 4 * width && pcm.out_width == (width == 1 ? 16 : 24));
                assert(adec.stream_info.channels == pcm.channels);
                assert(adec.stream_info.freq == pcm.frequency);
                assert(adec.stream_info.bits_per_sample == pcm.out_width);
                uint32_t saved_header = pcm.header;

                data[1] ^= 0x80;
                data[4] ^= 0x3f;
                assert(lgadec_bdlpcm_header_check(&adec, data, 1, &flag) == 0 && flag == 0);
                assert(pcm.header == saved_header && pcm.payload_size == 0x1234);
                data[3] ^= 1;
                assert(lgadec_bdlpcm_header_check(&adec, data, 1, &flag) == 0 && flag == 1);
                assert(pcm.header == saved_header && pcm.frequency == rates[rate]);
            }
        }
    }
    assert(lgadec_bdlpcm_header_check(NULL, NULL, 0, NULL) == -1);
}

static void dvd_headers(void)
{
    PCM_DATA_T pcm = {0};
    lgadec adec = {.codec_data = &pcm};
    const unsigned int rates[] = {48000, 96000, 44100, 32000};
    const unsigned int widths[] = {16, 20, 24, 16};
    for (unsigned int width = 0; width < 4; width++) {
        for (unsigned int rate = 0; rate < 4; rate++) {
            for (unsigned int channels = 1; channels <= 8; channels++) {
                uint8_t data[] = {0, 0, 0xc0, (width << 6) | (rate << 4) | (channels - 1), 0x35};
                int changed = -1;
                pcm.header = 0;
                assert(lgadec_dvdlpcm_header_check(&adec, data, &changed) == 0);
                assert(changed == 1);
                assert(pcm.frequency == rates[rate] && pcm.channels == channels);
                assert(pcm.width == widths[width]);
                assert(pcm.out_width == (width == 1 || width == 2 ? 24 : 16));
                assert(adec.stream_info.freq == pcm.frequency);
                assert(adec.stream_info.channels == channels);
                assert(adec.stream_info.bits_per_sample == pcm.out_width);
                data[2] |= 0x3f;
                assert(lgadec_dvdlpcm_header_check(&adec, data, &changed) == 0);
                assert(changed == 0);
            }
        }
    }
    assert(lgadec_dvdlpcm_header_check(NULL, NULL, NULL) == -1);
}

static void dvd_repacking(void)
{
    PCM_DATA_T pcm = {.frequency = 48000, .channels = 2, .width = 24};
    lgadec adec = {.codec_data = &pcm};
    uint8_t data[32], output[32], expected[32];
    const unsigned int order[] = {8, 1, 0, 9, 3, 2, 10, 5, 4, 11, 7, 6};
    for (unsigned int i = 0; i < sizeof(data); i++)
        data[i] = i + 0x20;
    memcpy(expected, data, sizeof(data));
    for (unsigned int block = 0; block < 2; block++)
        for (unsigned int i = 0; i < 12; i++)
            expected[12 * block + i] = data[12 * block + order[i]];
    assert(lgadec_dvdlpcm_resampling(&adec, data, 29, 123, NULL) == 0);
    assert(memcmp(data, expected, sizeof(data)) == 0 && pcm.timestamp == 123);
    pcm.width = 16;
    for (unsigned int i = 0; i < sizeof(data); i++)
        data[i] = i;
    assert(lgadec_dvdlpcm_resampling(&adec, data, 5, 456, NULL) == 0);
    assert(memcmp(data, (uint8_t[]){1, 0, 3, 2, 4, 5}, 6) == 0);
    pcm.width = 20;
    for (unsigned int i = 0; i < sizeof(data); i++)
        data[i] = i * 13 + 7;
    memset(output, 0xa5, sizeof(output));
    memset(expected, 0xa5, sizeof(expected));
    for (unsigned int block = 0; block < 2; block++) {
        uint8_t *s = data + 10 * block, *d = expected + 12 * block;
        d[0] = s[8] & 0xf0; d[1] = s[1]; d[2] = s[0];
        d[3] = s[8] << 4; d[4] = s[3]; d[5] = s[2];
        d[6] = s[9] & 0xf; d[7] = s[5]; d[8] = s[4];
        d[9] = s[9] << 4; d[10] = s[7]; d[11] = s[6];
    }
    assert(lgadec_dvdlpcm_resampling(&adec, data, 23, 789, output) == 0);
    assert(memcmp(output, expected, sizeof(output)) == 0);
    for (unsigned int i = 0; i < sizeof(data); i++)
        data[i] = i;
    assert(lgadec_dvdlpcm_resampling(&adec, data, 10, 789, data) == 0);
    assert(memcmp(data, (uint8_t[]){0, 1, 0, 128, 3, 0, 9, 0, 3, 144, 0, 9}, 12) == 0);
    assert(lgadec_dvdlpcm_resampling(&adec, data, 10, 111, NULL) == -1);
    assert(pcm.timestamp == 111);
    assert(lgadec_dvdlpcm_resampling(&adec, data, 0, 222, output) == -1);
    assert(pcm.timestamp == 111);
    pcm.frequency = 0;
    assert(lgadec_dvdlpcm_resampling(&adec, data, 10, 222, output) == -1);
    assert(pcm.timestamp == 111);
    pcm.frequency = 48000;
    pcm.width = 24;
    pcm.channels = 0;
    assert(lgadec_dvdlpcm_resampling(&adec, data, 12, 333, NULL) == -1);
    pcm.channels = 2;
    assert(lgadec_dvdlpcm_resampling(&adec, data, 5, 333, NULL) == -1);
    pcm.width = 16;
    assert(lgadec_dvdlpcm_resampling(&adec, data, 3, 333, NULL) == -1);
    pcm.width = 8;
    assert(lgadec_dvdlpcm_resampling(&adec, data, 12, 333, NULL) == -1);
    assert(lgadec_dvdlpcm_resampling(NULL, data, 12, 333, NULL) == -1);
    adec.codec_data = NULL;
    assert(lgadec_dvdlpcm_resampling(&adec, data, 12, 333, NULL) == -1);
}

static void adpcm_blocks(void)
{
    ADPCM_DATA_T state = {.type = LGADEC_ADPCM_IMA_WAV};
    lgadec adec = {.codec_data = &state, .stream_info = {.channels = 2, .block_align = 16}};
    uint8_t wav[] = {100, 0, 0, 0, 156, 255, 0, 0, 0x11, 0x11, 0x11, 0x11,
                     0x99, 0x99, 0x99, 0x99};
    int16_t output[300], *out = output;
    memset(output, 0x5a, sizeof(output));
    assert(lgadec_adpcm_resampling(&adec, wav, sizeof(wav), &out) == 0);
    assert(out == output + 18);
    for (int i = 0; i < 9; i++) {
        assert(output[2 * i] == 100 + i * 2);
        assert(output[2 * i + 1] == -100 - i * 2);
    }
    assert(output[18] == 0x5a5a);
    memset(&state, 0, sizeof(state));
    state.type = LGADEC_ADPCM_IMA_WAV;
    adec.stream_info.channels = 1;
    adec.stream_info.block_align = 8;
    uint8_t mono[] = {100, 0, 0, 0, 0x11, 0x11, 0x11, 0x11};
    out = output;
    assert(lgadec_adpcm_resampling(&adec, mono, sizeof(mono), &out) == 0);
    assert(out == output + 9);
    for (int i = 0; i < 9; i++)
        assert(output[i] == 100 + i * 2);

    uint8_t qt[136];
    for (unsigned int channels = 1; channels <= 2; channels++) {
        for (unsigned int block = 0; block < 2; block++) {
            for (unsigned int c = 0; c < channels; c++) {
                uint8_t *chunk = qt + (block * channels + c) * 34;
                chunk[0] = c ? 0xff : 1;
                chunk[1] = c ? 0x80 : 0;
                memset(chunk + 2, c ? 0x99 : 0x11, 32);
            }
        }
        memset(&state, 0, sizeof(state));
        state.type = LGADEC_ADPCM_IMA_QT;
        adec.stream_info.channels = channels;
        adec.stream_info.block_align = 34 * channels;
        out = output;
        memset(output, 0x5a, sizeof(output));
        assert(lgadec_adpcm_resampling(&adec, qt, 68 * channels, &out) == 0);
        assert(out == output + (channels == 1 ? 126 : 256));
        for (unsigned int block = 0; block < 2; block++) {
            unsigned int base = block * (channels == 1 ? 63 : 128);
            for (unsigned int i = 0; i < (channels == 1 ? 63 : 64); i++) {
                assert(output[base + channels * i] == 258 + 2 * (int)i);
                if (channels == 2)
                    assert(output[base + channels * i + 1] == -130 - 2 * (int)i);
            }
        }
        assert(output[channels == 1 ? 127 : 256] == 0x5a5a);
    }
    memset(&state, 0, sizeof(state));
    state.type = LGADEC_ADPCM_MS;
    adec.stream_info.channels = 1;
    adec.stream_info.block_align = 8;
    uint8_t ms[] = {0, 16, 0, 100, 0, 50, 0, 0};
    out = output;
    assert(lgadec_adpcm_resampling(&adec, ms, sizeof(ms), &out) == 0);
    assert(out == output + 4);
    assert(memcmp(output, (int16_t[]){50, 100, 100, 100}, 8) == 0);
    out = output;
    assert(lgadec_adpcm_resampling(&adec, ms, 6, &out) == -1 && out == output);
    state.type = 99;
    assert(lgadec_adpcm_resampling(&adec, ms, 8, &out) == -1 && out == output);
    assert(lgadec_adpcm_resampling(NULL, ms, 8, &out) == -1);
}

int main(void)
{
    alaw_samples();
    wifi_headers();
    wifi_log_metadata();
    bd_headers();
    dvd_headers();
    dvd_repacking();
    adpcm_blocks();
        puts("decoder codec regressions: PASS (131072 Wi-Fi / 1024 BD / 128 DVD headers, "
            "256 A-law samples, DVD 16/20/24, WAV/QT/MS ADPCM, Wi-Fi log metadata)");
}
