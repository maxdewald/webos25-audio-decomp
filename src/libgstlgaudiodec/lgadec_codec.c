/*
 * lgadec codec layer (gst-plugins-bsp/gstlgaudio/lgaudio/lgadec_codec.c):
 *  - SELECTED_CODEC_T tables (init / cleanup / feed / flush) chosen by lgadec_select_codec.
 *    Every feeder ends in lgadec_decoder_feed(), which is where the ES-bypass fork is
 *    (lgadec.c @0x29dd0). Feeders may prepend a synthetic header first:
 *      AAC raw  -> ADTS header per frame, plus one "first data" block (ADTS hdr + 0xdeadbeef
 *                  0xcafebabe magic + object type + format + AudioSpecificConfig)
 *      AC-4 raw -> AC-4 sync frame header (0xAC40)
 *      FLAC     -> "fLaC" + STREAMINFO before the first frame
 *      Vorbis/Opus/IAMF -> caps codec_data (DSI) as the first access unit
 *      private  -> strips a 2-byte big-endian length prefix
 *    There is no DTS feeder and no DTS table.
 *  - host-side PCM helpers used by the element for no_dec codecs (PCM, LPCM, ADPCM, G.711):
 *    DVD/BD/Wi-Fi LPCM header parsing and big-endian -> little-endian repacking, mu-law,
 *    A-law, IMA (QT/WAV) and MS ADPCM decoding.
 *
 * RECONSTRUCTION from orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so
 *   sha256 84db367e1ea3eeb4b589bf63910140cfd43ea77f2db124ae6882fbee8ebb6bb7
 * DWARF CU lgadec_codec.c (struct layouts, inlined helper names). Tags are Ghidra addresses.
 * The ADPCM step/index/adapt tables, the AAC sampling-frequency table and the G.711 A-law
 * table are the public standard values (checked equal to the binary's, not copied from it).
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lgadec.h"

struct logmm_module logmm_module_lgadec_codec = { 0, 0, 7, &logmm_dummy_mask, "lgadec_codec" };
#define LOGE(line, ...) LOGM(logmm_module_lgadec_codec, log_level_error, line, __VA_ARGS__)
#define LOGW(line, ...) LOGM(logmm_module_lgadec_codec, log_level_warning, line, __VA_ARGS__)
#define LOGN(line, ...) LOGM(logmm_module_lgadec_codec, log_level_noti, line, __VA_ARGS__)
#define LOGI(line, ...) LOGM(logmm_module_lgadec_codec, log_level_info, line, __VA_ARGS__)
#define LOGD(line, ...) LOGM(logmm_module_lgadec_codec, log_level_debug, line, __VA_ARGS__)
#define LOGT(line, ...) LOGM(logmm_module_lgadec_codec, log_level_trace, line, __VA_ARGS__)

/* ---- per-codec state (adec->codec_data), layouts from DWARF ------------------- */
typedef struct {
    LGADEC_CODEC_FORMAT_T aac_format;            /* detected / used */
    LGADEC_CODEC_FORMAT_T aac_format_from_demux; /* caps "stream-format" */
    uint8_t aac_adts_header[7];
    uint32_t feed_cnt;
} AAC_DATA_T;

typedef struct {
    LGADEC_CODEC_FORMAT_T ac4_format;
    uint8_t ac4_sync_frame_header[7];
    uint32_t feed_cnt;
} AC4_DATA_T;

typedef struct {
    uint8_t *buffer;                             /* partial-block accumulator (16 KiB) */
    uint32_t wr_offset, rd_offset;
    LGADEC_ADPCM_T type;
    int adpcm_index[6];
    int adpcm_predictor_sample[6];
    int16_t adpcm_sample_1[2], adpcm_sample_2[2];
    int adpcm_coeff_1[2], adpcm_coeff_2[2];
    int adpcm_idelta[2];
} ADPCM_DATA_T;

typedef struct {
    uint32_t header;                             /* last LPCM header, 0 = none yet */
    uint32_t frequency, channels;
    uint32_t width, out_width;                   /* bits in the stream / bits pushed */
    uint32_t payload_size;
    uint64_t timestamp;
} PCM_DATA_T;

typedef struct { uint32_t feed_cnt; } FLAC_DATA_T, VORBIS_DATA_T, OPUS_DATA_T, IAMF_DATA_T;

_Static_assert(sizeof(AAC_DATA_T) == 0x14, "AAC_DATA_T");
_Static_assert(sizeof(AC4_DATA_T) == 0x10, "AC4_DATA_T");
_Static_assert(sizeof(ADPCM_DATA_T) == 0x60, "ADPCM_DATA_T");
_Static_assert(sizeof(PCM_DATA_T) == 0x20, "PCM_DATA_T");

/* ---- standard ADPCM tables (IMA / Microsoft) ---------------------------------- */
static const int _adpcm_index_table[16] = { -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8 };
static const int _adpcm_step_table[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60,
    66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371,
    408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707,
    1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132,
    7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623,
    27086, 29794, 32767,
};
static const int _adpcm_adapt_table[16] = {
    230, 230, 230, 230, 307, 409, 512, 614, 768, 614, 512, 409, 307, 230, 230, 230,
};
static const uint8_t _adpcm_adapt_coeff_1[7] = { 64, 128, 0, 48, 60, 115, 98 };
static const int8_t _adpcm_adapt_coeff_2[7] = { 0, -64, 0, 16, 0, -52, -58 };

static inline int16_t clip_int16(int v)
{
    return (unsigned int)(v + 0x8000) & 0xffff0000u ? (int16_t)((v >> 31) ^ 0x7fff) : (int16_t)v;
}

/* @0x31020: one IMA ADPCM nibble */
int16_t _adpcm_decoder(ADPCM_DATA_T *adpcm_data, uint8_t ch, int8_t nibble)
{
    int index = adpcm_data->adpcm_index[ch];
    int step = _adpcm_step_table[index];
    int predictor;
    int diff;

    index += _adpcm_index_table[nibble];
    if (index >= 88)
        index = 88;
    if (index < 0)
        index = 0;
    diff = (((nibble & 7) * 2 + 1) * step) >> 3;
    predictor = adpcm_data->adpcm_predictor_sample[ch];
    if (nibble & 8)
        predictor -= diff;
    else
        predictor += diff;
    if ((unsigned int)(predictor + 0x8000) & 0xffff0000u)
        predictor = (predictor >> 31) ^ 0x7fff;
    else
        predictor = (int16_t)predictor;
    adpcm_data->adpcm_index[ch] = index;
    adpcm_data->adpcm_predictor_sample[ch] = predictor;
    return predictor;
}

/* @0x3108c: one Microsoft ADPCM nibble */
int16_t _adpcm_ms_expand_nibble(ADPCM_DATA_T *adpcm_data, uint8_t ch, char nibble)
{
    int predictor;

    predictor = (adpcm_data->adpcm_sample_1[ch] * adpcm_data->adpcm_coeff_1[ch] +
                 adpcm_data->adpcm_sample_2[ch] * adpcm_data->adpcm_coeff_2[ch]) / 64;
    predictor += ((nibble & 8) ? nibble - 16 : nibble) * adpcm_data->adpcm_idelta[ch];
    adpcm_data->adpcm_sample_2[ch] = adpcm_data->adpcm_sample_1[ch];
    adpcm_data->adpcm_sample_1[ch] = clip_int16(predictor);
    adpcm_data->adpcm_idelta[ch] =
        (_adpcm_adapt_table[(unsigned char)nibble] * adpcm_data->adpcm_idelta[ch]) >> 8;
    if (adpcm_data->adpcm_idelta[ch] < 16)
        adpcm_data->adpcm_idelta[ch] = 16;
    return adpcm_data->adpcm_sample_1[ch];
}

/* @0x3110c: AAC sampling_frequency_index (0 if not in the table) */
uint8_t _get_freq_idx(uint32_t frequency)
{
    const uint32_t frequencyTable[13] = { 96000, 88200, 64000, 48000, 44100, 32000, 24000,
                                          22050, 16000, 12000, 11025, 8000, 7350 };
    uint8_t idx = 0;

    for (unsigned int i = 0; i < 13; i++) {
        if (frequencyTable[i] == frequency) {
            idx = i;
            break;
        }
    }
    LOGT(0xb8, "frequnecy=%u, freq_idx=%u", frequency, idx);
    return idx;
}

/* @0x311a0 */
void _adpcm_cleanup(void **h)
{
    ADPCM_DATA_T *adpcm = *h;

    if (adpcm) {
        if (adpcm->buffer) {
            free(adpcm->buffer);
            adpcm->buffer = NULL;
        }
        free(*h);
        *h = NULL;
        LOGT(0x319, "cleanup finish");
        return;
    }
    LOGT(0x30b, "cleanup finish");
}

/* @0x31230: "private" streams carry a 2-byte big-endian size in front of every chunk */
int _private_feed(lgadec *adec, uint8_t *data, uint32_t len, uint64_t timestamp)
{
    uint32_t length, size;

    if (len < 2) {
        LOGE(0x3f, "audio chunk size is less than 2bytes");
        return -1;
    }
    length = len - 2;
    size = data[0] << 8 | data[1];
    if (size > 1) {
        if (len < size + 1) {
            LOGE(0x4d, "private audio chunk size is small or len is zero!!!");
            return -1;
        }
        if ((int)(len - (size + 1)) < 1)
            length = size - 1;
    }
    if (lgadec_decoder_feed(adec, data + 2, length, timestamp) != 0) {
        LOGE(0x5b, "Feed Err");
        return -1;
    }
    return 0;
}

/* @0x312f8: the caps streamheader (DSI) goes first, with the first buffer's timestamp */
int _vorbis_feed(lgadec *adec, uint8_t *data, uint32_t len, uint64_t timestamp)
{
    VORBIS_DATA_T *vorbis;

    if (!adec) {
        LOGE(0x8b2, "adec is null");
        return -1;
    }
    vorbis = adec->codec_data;
    if (vorbis->feed_cnt == 0) {
        LGADEC_STREAM_INFO_T *si = &adec->stream_info;

        if (si->dsi_buffer_length == 0 || !si->dsi_buffer) {
            LOGE(0x8ca, "The header of Vorbis is not exist!!!");
            return -1;
        }
        LOGT(0x8ba, "use codec data(len=%u) for vorbis header", si->dsi_buffer_length);
        if (lgadec_decoder_feed(adec, si->dsi_buffer, si->dsi_buffer_length, timestamp) != 0) {
            LOGE(0x8c2, "Feed(header) Err");
            return -1;
        }
        adec->fed_frame_cnt++;
        vorbis->feed_cnt++;
    }
    if (lgadec_decoder_feed(adec, data, len, timestamp) != 0) {
        LOGE(0x8d1, "feed Err!!");
        return -1;
    }
    vorbis->feed_cnt++;
    return 0;
}

/* @0x31468: AC-4 RAW access units get a 7-byte sync frame header (0xAC40, 0xFFFF, 24-bit size) */
int _ac4_feed(lgadec *adec, uint8_t *data, uint32_t len, uint64_t timestamp)
{
    AC4_DATA_T *ac4;

    if (!adec) {
        LOGE(0x23f, "adec is null");
        return -1;
    }
    ac4 = adec->codec_data;
    if (!ac4) {
        LOGE(0x240, "adec->codec_data is null");
        return -1;
    }
    if (ac4->feed_cnt == 0 && ac4->ac4_format == LGADEC_UNKNOWN_FORMAT) {
        if (((data[0] << 8 | data[1]) & 0xac40) == 0xac40) {
            LOGT(0x24c, "This AC4 audio chunk is SYNC frame");
            ac4->ac4_format = LGADEC_AC4_SYNC_FRAME_FORMAT;
            LOGI(0x25a, "This stream is AC4 - SYNC frame format");
        } else {
            LOGT(0x24f, "This AC4 audio chunk is NOT SYNC frame format.");
            ac4->ac4_format = LGADEC_RAW_FORMAT;
            LOGI(0x255, "This stream is AC4 - RAW format. transform RAW to SYNC frame format");
        }
    }
    if (ac4->ac4_format == LGADEC_RAW_FORMAT) {
        uint8_t *frame = malloc(len + 7);
        int ret;

        if (!frame) {
            LOGE(0x26c, "malloc Err");
            return -1;
        }
        ac4->ac4_sync_frame_header[0] = 0xac;
        ac4->ac4_sync_frame_header[1] = 0x40;
        ac4->ac4_sync_frame_header[2] = 0xff;
        ac4->ac4_sync_frame_header[3] = 0xff;
        ac4->ac4_sync_frame_header[4] = len >> 16;
        ac4->ac4_sync_frame_header[5] = len >> 8;
        ac4->ac4_sync_frame_header[6] = len;
        memcpy(frame, ac4->ac4_sync_frame_header, 7);
        memcpy(frame + 7, data, len);
        ret = lgadec_decoder_feed(adec, frame, len + 7, timestamp);
        free(frame);
        if (ret != 0) {
            LOGE(0x285, "Feed Err");
            return -1;
        }
    } else if (lgadec_decoder_feed(adec, data, len, timestamp) != 0) {
        LOGE(0x290, "Feed Err");
        return -1;
    }
    ac4->feed_cnt++;
    return 0;
}

#define FLAC_HEADER_SIZE 42   /* "fLaC" + metadata block header + 34-byte STREAMINFO */

/* inlined into _flac_feed */
static inline void _flac_make_header(lgadec *adec, uint8_t *h)
{
    LGADEC_STREAM_INFO_T *si = &adec->stream_info;
    unsigned int bps = si->bits_per_sample - 1;

    memset(h + 4, 0, FLAC_HEADER_SIZE - 4);
    memcpy(h, "fLaC", 4);
    h[4] = 0x80;               /* last metadata block, type STREAMINFO */
    h[7] = 34;                 /* block length */
    h[18] = si->freq >> 12;    /* 20-bit sample rate, 3-bit channels-1, 5-bit bps-1 */
    h[19] = si->freq >> 4;
    h[20] = si->freq << 4 | ((si->channels - 1) << 1 & 0xe) | ((bps & 0x1f) >> 4);
    h[21] = bps << 4;
}

/* @0x316e4: synthesises a STREAMINFO from caps in front of the first frame */
int _flac_feed(lgadec *adec, uint8_t *data, uint32_t len, uint64_t timestamp)
{
    FLAC_DATA_T *flac;

    if (!adec) {
        LOGE(0x843, "adec is null");
        return -1;
    }
    flac = adec->codec_data;
    if (flac->feed_cnt == 0) {
        uint8_t flac_header[FLAC_HEADER_SIZE];
        uint8_t *frame;
        int ret;

        _flac_make_header(adec, flac_header);
        frame = malloc(len + FLAC_HEADER_SIZE);
        if (!frame) {
            LOGE(0x856, "malloc ERROR!!!");
            return -1;
        }
        memcpy(frame, flac_header, FLAC_HEADER_SIZE);
        memcpy(frame + FLAC_HEADER_SIZE, data, len);
        ret = lgadec_decoder_feed(adec, frame, len + FLAC_HEADER_SIZE, timestamp);
        if (ret != 0) {
            LOGE(0x861, "feed Err!!");
            free(frame);
            return -1;
        }
        free(frame);
    } else if (lgadec_decoder_feed(adec, data, len, timestamp) != 0) {
        LOGE(0x86b, "feed Err!!");
        return -1;
    }
    flac->feed_cnt++;
    return 0;
}

/* @0x318b8: OpusHead from caps (only if longer than the 19-byte minimum), timestamp 0 */
int _opus_feed(lgadec *adec, uint8_t *data, uint32_t len, uint64_t timestamp)
{
    OPUS_DATA_T *opus;

    if (!adec) {
        LOGE(0x91a, "adec is null");
        return -1;
    }
    opus = adec->codec_data;
    if (opus->feed_cnt == 0) {
        LGADEC_STREAM_INFO_T *si = &adec->stream_info;

        if (si->dsi_buffer_length == 0 || !si->dsi_buffer) {
            LOGN(0x944, "The header of opus is not exist!!! (no error)");
        } else if (si->dsi_buffer_length > 18) {
            void *header;
            int ret;

            LOGT(0x92a, "use codec data(len=%u) for opus header", si->dsi_buffer_length);
            header = malloc(si->dsi_buffer_length);
            if (!header) {
                LOGE(0x930, "malloc (dsi_buffer) failed");
                return -1;
            }
            memcpy(header, si->dsi_buffer, si->dsi_buffer_length);
            ret = lgadec_decoder_feed(adec, header, si->dsi_buffer_length, 0);
            free(header);
            if (ret != 0) {
                LOGE(0x93c, "Feed(header) Err");
                return -1;
            }
            adec->fed_frame_cnt++;
            opus->feed_cnt++;
        }
    }
    if (lgadec_decoder_feed(adec, data, len, timestamp) != 0) {
        LOGE(0x94a, "feed Err!!");
        return -1;
    }
    opus->feed_cnt++;
    return 0;
}

/* @0x31a90: IAMF descriptors (caps codec_data) first, timestamp 0 */
int _iamf_feed(lgadec *adec, uint8_t *data, uint32_t len, uint64_t timestamp)
{
    IAMF_DATA_T *iamf;

    if (!adec) {
        LOGE(0x990, "adec is null");
        return -1;
    }
    iamf = adec->codec_data;
    if (iamf->feed_cnt == 0) {
        LGADEC_STREAM_INFO_T *si = &adec->stream_info;

        if (si->dsi_buffer_length != 0 && si->dsi_buffer) {
            void *header;
            int ret;

            LOGT(0x99d, "use codec data(len=%u) for iamf header", si->dsi_buffer_length);
            header = malloc(si->dsi_buffer_length);
            if (!header) {
                LOGE(0x9a3, "malloc (dsi_buffer) failed");
                return -1;
            }
            memcpy(header, si->dsi_buffer, si->dsi_buffer_length);
            ret = lgadec_decoder_feed(adec, header, si->dsi_buffer_length, 0);
            free(header);
            if (ret != 0) {
                LOGE(0x9af, "Feed(header) Err");
                return -1;
            }
            adec->fed_frame_cnt++;
            iamf->feed_cnt++;
        } else {
            LOGN(0x9b6, "The header of iamf is not exist!!! (no error)");
        }
    }
    if (lgadec_decoder_feed(adec, data, len, timestamp) != 0) {
        LOGE(0x9bc, "feed Err!!");
        return -1;
    }
    iamf->feed_cnt++;
    return 0;
}

/* @0x31c5c */
static void _aac_init(void *h)
{
    AAC_DATA_T *aac = h;

    if (!aac) {
        LOGE(0x7b, "init fail. data is NULL!!");
        return;
    }
    aac->aac_format = LGADEC_UNKNOWN_FORMAT;
    aac->feed_cnt = 0;
    LOGT(0x83, "Init AAC format to LGADEC_UNKNOWN_FORMAT");
}

/* @0x31cd8 */
static void _ac4_init(void *h)
{
    AC4_DATA_T *ac4 = h;

    if (!ac4) {
        LOGE(0x219, "init fail. data is NULL!!");
        return;
    }
    ac4->ac4_format = LGADEC_UNKNOWN_FORMAT;
    ac4->feed_cnt = 0;
    LOGT(0x221, "Init AC4 format to LGADEC_UNKNOWN_FORMAT");
}

/* @0x31d58 */
void _pcm_init(void *h)
{
    PCM_DATA_T *pcm = h;

    if (!pcm) {
        LOGE(0x48f, "init fail. data is NULL!!");
        return;
    }
    pcm->header = 0;
    pcm->frequency = 0;
    pcm->channels = 0;
    pcm->width = 0;
    pcm->out_width = 0;
    pcm->payload_size = 0;
    pcm->timestamp = ~0ull;
}

/* @0x31dbc */
void _flac_init(void *h)
{
    uint32_t *feed_cnt = h;

    if (!feed_cnt) {
        LOGE(0x7f3, "init fail. data is NULL!!");
        return;
    }
    *feed_cnt = 0;
}
/* @0x31e0c */
void _vorbis_init(void *h)
{
    uint32_t *feed_cnt = h;

    if (!feed_cnt) {
        LOGE(0x886, "init fail. data is NULL!!");
        return;
    }
    *feed_cnt = 0;
}
/* @0x31e5c */
void _opus_init(void *h)
{
    uint32_t *feed_cnt = h;

    if (!feed_cnt) {
        LOGE(0x8ee, "init fail. data is NULL!!");
        return;
    }
    *feed_cnt = 0;
}
/* @0x31eac */
void _iamf_init(void *h)
{
    uint32_t *feed_cnt = h;

    if (!feed_cnt) {
        LOGE(0x964, "init fail. data is NULL!!");
        return;
    }
    *feed_cnt = 0;
}

/* @0x31efc: init of the codecs without state (none / private) */
void _init(void *h)
{
    LOGT(0x15, "init codec");
}

/* @0x31f44 */
void _cleanup(void **h)
{
    if (*h) {
        free(*h);
        *h = NULL;
    }
    LOGT(0x1c, "cleanup finish");
}

/* @0x31f94: keeps the ADPCM type across re-init */
void _adpcm_init(void *h)
{
    ADPCM_DATA_T *adpcm = h;
    LGADEC_ADPCM_T type;

    if (!adpcm) {
        LOGE(0x2da, "init fail. data is NULL!!");
        return;
    }
    type = adpcm->type;
    if (adpcm->buffer)
        free(adpcm->buffer);
    memset(adpcm, 0, sizeof(*adpcm));
    adpcm->type = type;
}

/* @0x31ff0: flush = resend the stream header with the next buffer */
int _vorbis_flush(lgadec *adec)
{
    if (!adec) {
        LOGE(0x8a3, "adec is null");
        return -1;
    }
    *(uint32_t *)adec->codec_data = 0;
    return 0;
}
/* @0x32054 */
int _opus_flush(lgadec *adec)
{
    if (!adec) {
        LOGE(0x90b, "adec is null");
        return -1;
    }
    *(uint32_t *)adec->codec_data = 0;
    return 0;
}
/* @0x320b8 */
int _iamf_flush(lgadec *adec)
{
    if (!adec) {
        LOGE(0x981, "adec is null");
        return -1;
    }
    *(uint32_t *)adec->codec_data = 0;
    return 0;
}

/* @0x3211c: everything else goes to the decoder unchanged */
int _raw_feed(lgadec *adec, uint8_t *data, uint32_t len, uint64_t timestamp)
{
    int ret = lgadec_decoder_feed(adec, data, len, timestamp);

    if (ret != 0)
        LOGE(0x28, "Feed Err");
    return ret;
}

/* ---- AAC ---------------------------------------------------------------------- */

/* ADTS frame_length (13 bit) + low channel bits into header bytes 3..5 */
static inline void _aac_set_adts_length(uint8_t *h, unsigned int channels, unsigned int length)
{
    h[3] = (length & 0x1fff) >> 11 | channels << 6;
    h[4] = length >> 3;
    h[5] = length << 5;
}

/* inlined: sync word check on the first access unit */
static int _aac_detect(lgadec *adec, uint8_t *data, uint32_t len)
{
    AAC_DATA_T *aac = adec->codec_data;

    if (len < 7) {
        LOGT(0x107, "buf size is less than %d bytes. This AAC audio chunk is NOT ADTS", 7);
        aac->aac_format = LGADEC_RAW_FORMAT;
    } else {
        uint16_t sync;

        memcpy(&sync, data, sizeof(sync));
        sync = (sync & 0xff) << 8 | sync >> 8;
        if ((sync & 0xfff0) == 0xfff0) {
            LOGT(0x112, "This AAC audio chunk is ADTS");
            aac->aac_format = LGADEC_AAC_ADTS_FORMAT;
        } else if (sync >> 5 == 0x2b7) {
            LOGT(0x116, "This AAC audio chunk is LOAS format.");
            aac->aac_format = LGADEC_AAC_LOAS_FORMAT;
        } else {
            LOGT(0x119, "This AAC audio chunk is NOT ADTS/LOAS format.");
            aac->aac_format = LGADEC_RAW_FORMAT;
        }
    }
    return 0;
}

/* inlined: AudioSpecificConfig -> object type / ADTS byte 2 / channel config */
static int _get_aac_info(void *extra_data, uint32_t len, uint8_t *object_type,
                         uint8_t *adts_byte2, uint8_t *channels)
{
    uint8_t *dsi = extra_data;
    uint8_t ot, freq_idx, ch = 0;

    if (!dsi || len < 2) {
        LOGE(0xc8, "extra_data is NULL or extra_data_len is less than 2!!!");
        return -1;
    }
    ot = dsi[0] >> 3;
    if (ot == 31) {                                /* escaped audioObjectType */
        ot = ((dsi[0] & 7) << 3 | dsi[1] >> 5) + 32;
        freq_idx = (dsi[1] & 0x1f) >> 1;
        if (freq_idx == 15) {
            if (len >= 5) {
                freq_idx = _get_freq_idx((dsi[1] & 1) << 23 | dsi[2] << 15 | dsi[3] << 7 | dsi[4] >> 1);
                if (len > 5)
                    ch = (dsi[4] & 1) << 3 | dsi[5] >> 5;
            }
        } else if (len > 2) {
            ch = (dsi[1] & 1) << 3 | dsi[2] >> 5;
        }
    } else {
        freq_idx = (dsi[0] & 7) << 1 | dsi[1] >> 7;
        if (freq_idx == 15) {
            if (len >= 5) {
                freq_idx = _get_freq_idx((dsi[1] & 0x7f) << 17 | dsi[2] << 9 | dsi[3] << 1 | dsi[4] >> 7);
                ch = (dsi[4] & 0x7f) >> 3;
            }
        } else {
            ch = (dsi[1] & 0x7f) >> 3;
        }
    }
    *object_type = ot;
    *adts_byte2 = freq_idx;
    *channels = ch;
        LOGT(0xf5, "objectType = 0x%02X, freqIndex = 0x%02X, ch = 0x%02X", *object_type,
            *adts_byte2, *channels);
    return 0;
}

/* inlined: tells the DSP the AAC config. Layout: ADTS header(7) 0xdeadbeef 0xcafebabe
 * object_type(1) format(1) AudioSpecificConfig(n), timestamp 0. */
static int _aac_first_data(lgadec *adec, uint8_t *data, uint32_t len)
{
    LGADEC_STREAM_INFO_T *si;
    AAC_DATA_T *aac;
    uint8_t object_type;
    uint8_t byte2, channels;
    uint8_t *buf, *ptr;
    int dsi_len;
    uint8_t *adts_header;
    uint32_t adts_len;
    int ret;

    if (!adec) {
        LOGE(0x12e, "adec is null");
        return -1;
    }
    aac = adec->codec_data;
    if (!aac) {
        LOGE(0x12f, "adec->codec_data is null");
        return -1;
    }
    si = &adec->stream_info;
    adts_header = aac->aac_adts_header;
    adts_len = si->dsi_buffer_length + 17;
    object_type = si->object_type;

    if (aac->aac_format == LGADEC_UNKNOWN_FORMAT || adec->app_type != LGADEC_APP_TYPE_APPCAST)
        _aac_detect(adec, data, len);
    lgadec_decoder_set_aac_format_type(adec, aac->aac_format);

    switch (aac->aac_format) {
    case LGADEC_AAC_ADTS_FORMAT:
        adts_header[0] = data[0];
        adts_header[1] = data[1];
        adts_header[2] = data[2];
        adts_header[6] = data[6];
        _aac_set_adts_length(adts_header, si->channels, adts_len);
        return 0;
    case LGADEC_AAC_LOAS_FORMAT:
        adts_header[0] = 0xff;
        adts_header[1] = 0xf1;
        adts_header[2] = (object_type - 1) * 64;
        adts_header[6] = 0;
        _aac_set_adts_length(adts_header, si->channels, adts_len);
        return 0;
    case LGADEC_RAW_FORMAT: {
        if (si->dsi_buffer && _get_aac_info(si->dsi_buffer, si->dsi_buffer_length,
                                           &object_type, &byte2, &channels) == 0) {
            uint8_t profile = object_type ? object_type - 1 : 0;

            if ((uint8_t)(channels - 1) > 6)
                channels = si->channels;
            si->channels = channels;
            adts_header[0] = 0xff;
            adts_header[1] = 0xf1;
            adts_header[2] = (int8_t)(profile << 6 | (byte2 & 0xf) << 2) | (channels & 7) >> 2;
            adts_header[6] = 0;
            _aac_set_adts_length(adts_header, si->channels, adts_len);

            dsi_len = adec->stream_info.dsi_buffer_length;
            adts_len = dsi_len + 17;
            buf = malloc(adts_len);
            uint32_t magic;

            if (!buf) {
                LOGE(0x178, "malloc Err");
                return -1;
            }
            memcpy(buf, adts_header, 7);
            ptr = buf + 7;
            magic = 0xdeadbeef;
            memcpy(ptr, &magic, sizeof(magic));
            ptr += sizeof(magic);
            magic = 0xcafebabe;
            memcpy(ptr, &magic, sizeof(magic));
            ptr += sizeof(magic);
            *ptr++ = object_type;
            *ptr++ = aac->aac_format;
            if (adec->stream_info.dsi_buffer)
                memcpy(ptr, adec->stream_info.dsi_buffer, dsi_len);
            LOGN(0x18f, "Tell F/W, object type %d, Data Format %d\n", object_type, aac->aac_format);
            LOGN(0x191, "dsi / total : %d / %d\n", adec->stream_info.dsi_buffer_length, adts_len);
            ret = lgadec_decoder_feed(adec, buf, adts_len, 0);

            free(buf);
            if (ret != 0) {
                LOGE(0x19a, "Feed Err (first data)");
                return -1;
            }
            aac->feed_cnt++;
            adec->fed_frame_cnt++;
        } else {
            LOGT(0x1a1, "DSI is NULL(%p) or _get_aac_info() ERR!!! do not create ADTS seq header",
                 si->dsi_buffer);
            aac->aac_format = LGADEC_UNKNOWN_FORMAT;
            return -1;
        }
        break;
    }
    default:
        return 0;
    }
    return 0;
}

/* @0x32174: on the first buffer: frame format from caps (trusted only for APPCAST) or from
 * the sync word; RAW gets an ADTS header built from the DSI and every later frame is wrapped
 * in ADTS; ADTS/LOAS are fed unchanged. */
int _aac_feed(lgadec *adec, uint8_t *data, uint32_t len, uint64_t timestamp)
{
    AAC_DATA_T *aac;
    LGADEC_STREAM_INFO_T *si;
    uint8_t *adts_header;

    if (!adec) {
        LOGE(0x1b3, "adec is null");
        return -1;
    }
    aac = adec->codec_data;
    if (!aac) {
        LOGE(0x1b4, "adec->codec_data is null");
        return -1;
    }
    si = &adec->stream_info;
    adts_header = aac->aac_adts_header;
    if (aac->aac_format == LGADEC_UNKNOWN_FORMAT)
        aac->aac_format = aac->aac_format_from_demux;

    if (aac->feed_cnt == 0) {
        _aac_first_data(adec, data, len);
        switch (aac->aac_format) {
        case LGADEC_RAW_FORMAT:
            LOGN(0x1c3, "This stream is AAC - RAW format. transform RAW to ADTS format");
            break;
        case LGADEC_AAC_ADTS_FORMAT:
            LOGN(0x1c7, "This stream is AAC - ADTS format");
            break;
        case LGADEC_AAC_LOAS_FORMAT:
            LOGN(0x1cb, "This stream is AAC - LOAS format");
            break;
        default:
            LOGE(0x1cf, "This stream is AAC - UNKNOWN format. bypass audio data");
            break;
        }
    }

    if (aac->aac_format == LGADEC_RAW_FORMAT) {
        uint8_t channels = si->channels;
        unsigned int total = len + 7;
        uint8_t *frame = malloc(total);
        int ret;

        if (!frame) {
            LOGE(0x1df, "malloc Err");
            return -1;
        }
        _aac_set_adts_length(adts_header, channels, total);
        memcpy(frame, adts_header, 7);
        memcpy(frame + 7, data, len);
        ret = lgadec_decoder_feed(adec, frame, total, timestamp);
        free(frame);
        if (ret != 0) {
            LOGE(0x1ef, "feed Err!!");
            return -1;
        }
    } else if (lgadec_decoder_feed(adec, data, len, timestamp) != 0) {
        LOGE(0x1f9, "feed Err!!");
        return -1;
    }
    aac->feed_cnt++;
    return 0;
}

/* ---- create ------------------------------------------------------------------- */

/* @0x329bc */
void *lgadec_aac_create(LGADEC_CODEC_FORMAT_T format)
{
    AAC_DATA_T *aac = calloc(1, sizeof(*aac));

    if (!aac) {
        LOGE(0x8e, "memory alloc fail!");
        return NULL;
    }
    _aac_init(aac);
    aac->aac_format_from_demux = format;
    return aac;
}

/* @0x32a38: unlike AAC, the caps format is used directly */
void *lgadec_ac4_create(LGADEC_CODEC_FORMAT_T format)
{
    AC4_DATA_T *ac4 = calloc(1, sizeof(*ac4));

    if (!ac4) {
        LOGE(0x22c, "memory alloc fail!");
        return NULL;
    }
    _ac4_init(ac4);
    ac4->ac4_format = format;
    return ac4;
}

/* @0x32ab8 */
void *lgadec_adpcm_create(LGADEC_ADPCM_T type)
{
    ADPCM_DATA_T *adpcm = calloc(1, sizeof(*adpcm));

    if (!adpcm) {
        LOGE(0x2f9, "memory alloc fail!");
        return NULL;
    }
    _adpcm_init(adpcm);
    adpcm->type = type;
    return adpcm;
}

/* ---- ADPCM (decoded on the host) ---------------------------------------------- */

/* @0x32b10: collects input until one block_align is available. Short input is copied into
 * a 16 KiB side buffer and *len set to 0 (nothing to decode yet). */
int lgadec_adpcm_data_length_check(void *a, unsigned char *data, unsigned int *len)
{
    lgadec *adec = a;
    ADPCM_DATA_T *adpcm;
    unsigned int block_align, n;

    if (!adec) {
        LOGE(0x324, "adec is null");
        return -1;
    }
    adpcm = adec->codec_data;
    block_align = adec->stream_info.block_align;
    n = *len;
    if (adpcm->wr_offset != 0) {
        memcpy(adpcm->buffer + adpcm->wr_offset, data, n);
        adpcm->wr_offset += *len;
        n = adpcm->wr_offset - adpcm->rd_offset;
        *len = n;
        data = adpcm->buffer + adpcm->rd_offset;
    }
    if (n >= block_align) {
        /* sic: the caller keeps decoding from its own input pointer, not the accumulated buffer */
        adpcm->wr_offset = 0;
        adpcm->rd_offset = 0;
        return 0;
    }
    LOGW(0x334, "ADPCM data size(%u) is less than block size(%u)", n, block_align);
    if (!adpcm->buffer) {
        adpcm->buffer = calloc(1, 0x4000);
        if (!adpcm->buffer) {
            LOGE(0x339, "memory alloc fail");
            return -1;
        }
        adpcm->wr_offset = 0;
        adpcm->rd_offset = 0;
    } else if (adpcm->wr_offset != 0) {
        goto out;
    }
    memcpy(adpcm->buffer, data, *len);
    adpcm->wr_offset += *len;
out:
    *len = 0;
    return 0;
}

static inline int16_t rd16(const uint8_t *p)
{
    int16_t value;

    memcpy(&value, p, sizeof(value));
    return value;
}

/* @0x32c40: ADPCM blocks -> interleaved S16; *sample is advanced past the output */
int lgadec_adpcm_resampling(void *a, unsigned char *data, unsigned int len, short **sample)
{
    lgadec *adec = a;
    ADPCM_DATA_T *adpcm;
    unsigned int block_align, ch, done;
    unsigned char stereo;
    int16_t *out, *start;

    if (!adec) {
        LOGE(0x3bb, "adec is null");
        return -1;
    }
    block_align = adec->stream_info.block_align;
    adpcm = adec->codec_data;
    ch = adec->stream_info.channels;
    start = out = *sample;
    stereo = ch == 2;

    switch (adpcm->type) {
    case LGADEC_ADPCM_IMA_WAV:
        /* per block: ch x {int16 predictor, u8 index, u8 0}, then 4-byte groups per channel */
        for (done = 0; done < len; done += block_align) {
            unsigned int pos = 0;

            for (int c = 0; c < ch; c++) {
                int16_t pred = rd16(data);

                *out++ = pred;
                adpcm->adpcm_predictor_sample[c] = pred;
                adpcm->adpcm_index[c] = data[2] >= 88 ? 88 : data[2];
                data += 4;
            }
            pos = ch * 4;
            for (; pos < block_align; pos += 4 * (stereo + 1)) {
                int16_t *low = out;
                int16_t *high = out + stereo;

                for (int i = 0; i < 4; i++) {
                    low[0] = _adpcm_decoder(adpcm, 0, data[i] & 0xf);
                    if (ch == 2) {
                        low[1] = _adpcm_decoder(adpcm, 1, data[4 + i] & 0xf);
                        high[1] = _adpcm_decoder(adpcm, 0, data[i] >> 4);
                        high[2] = _adpcm_decoder(adpcm, 1, data[4 + i] >> 4);
                    } else {
                        high[1] = _adpcm_decoder(adpcm, 0, data[i] >> 4);
                    }
                    low += 2 * (stereo + 1);
                    high += 2 * (stereo + 1);
                }
                out += 8 * (stereo + 1);
                data += 4 * (stereo + 1);
            }
        }
        break;

    case LGADEC_ADPCM_MS: {
        int remain = len - ch * 7;

        for (done = 0; done < len; done += block_align, remain -= block_align) {
            int nbytes = block_align - ch * 7;
            unsigned int pred = data[0] > 5 ? 6 : data[0];

            if (remain + ch * 7 < block_align)
                nbytes = remain;
            if (nbytes < 0) {
                LOGE(0x3fe, "data size is small");
                return -1;
            }
            if (ch == 2) {
                unsigned int pred1 = data[1] < 7 ? data[1] : 6;

                adpcm->adpcm_idelta[0] = (uint16_t)rd16(data + 2);
                adpcm->adpcm_idelta[1] = (uint16_t)rd16(data + 4);
                adpcm->adpcm_coeff_1[0] = _adpcm_adapt_coeff_1[pred];
                adpcm->adpcm_coeff_2[0] = _adpcm_adapt_coeff_2[pred];
                adpcm->adpcm_coeff_1[1] = _adpcm_adapt_coeff_1[pred1];
                adpcm->adpcm_coeff_2[1] = _adpcm_adapt_coeff_2[pred1];
                adpcm->adpcm_sample_1[0] = rd16(data + 6);
                adpcm->adpcm_sample_1[1] = rd16(data + 8);
                adpcm->adpcm_sample_2[0] = rd16(data + 10);
                adpcm->adpcm_sample_2[1] = rd16(data + 12);
                out[0] = adpcm->adpcm_sample_2[0];
                out[1] = adpcm->adpcm_sample_2[1];
                out[2] = adpcm->adpcm_sample_1[0];
                out[3] = adpcm->adpcm_sample_1[1];
                out += 4;
                data += 14;
            } else {
                adpcm->adpcm_idelta[0] = (uint16_t)rd16(data + 1);
                adpcm->adpcm_coeff_1[0] = _adpcm_adapt_coeff_1[pred];
                adpcm->adpcm_coeff_2[0] = _adpcm_adapt_coeff_2[pred];
                adpcm->adpcm_coeff_1[1] = 64;
                adpcm->adpcm_coeff_2[1] = 0;
                adpcm->adpcm_sample_1[0] = rd16(data + 3);
                adpcm->adpcm_sample_2[0] = rd16(data + 5);
                out[0] = adpcm->adpcm_sample_2[0];
                out[1] = adpcm->adpcm_sample_1[0];
                out += 2;
                data += 7;
            }
            for (int i = 0; i < nbytes; i++) {
                out[2 * i] = _adpcm_ms_expand_nibble(adpcm, 0, data[i] >> 4);
                out[2 * i + 1] = _adpcm_ms_expand_nibble(adpcm, ch == 2, data[i] & 0xf);
            }
            data += nbytes;
            out += 2 * nbytes;
        }
        break;
    }

    case LGADEC_ADPCM_IMA_QT:
        /* per channel 34-byte chunk: 9-bit predictor + 7-bit index, 32 bytes = 64 samples */
        for (done = 0; done < len; done += block_align) {
            for (int c = 0; c < ch; c++) {
                unsigned int index = data[1] & 0x7f;

                adpcm->adpcm_predictor_sample[c] = (int16_t)((data[0] << 8 | data[1]) & 0xff80);
                adpcm->adpcm_index[c] = index > 88 ? 88 : index;
                for (int i = 2; i < 34; i++) {
                    out[0] = _adpcm_decoder(adpcm, c, data[i] & 0xf);
                    out[ch] = _adpcm_decoder(adpcm, c, data[i] >> 4);
                    out += 2 * ch;
                }
                data += 34;
                if (c + 1 < ch)
                    out -= ch * 64 - 1;
            }
            /* sic: undoes the per-channel offset; for mono this drops one sample per block */
            if (out > start)
                out--;
        }
        break;

    default:
        LOGE(0x46b, "not support type (type:%d)", adpcm->type);
        return -1;
    }
    *sample = out;
    return 0;
}

/* ---- LPCM --------------------------------------------------------------------- */

/* @0x330cc */
void *lgadec_pcm_create(void)
{
    PCM_DATA_T *pcm = calloc(1, sizeof(*pcm));

    if (!pcm) {
        LOGE(0x4a5, "memory alloc fail!");
        return NULL;
    }
    pcm->channels = 0;
    pcm->width = 0;
    pcm->out_width = 0;
    pcm->payload_size = 0;
    pcm->timestamp = ~0ull;
    return pcm;
}

/* @0x33134 */
int lgadec_pcm_get_width(void *a, uint32_t *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x4b4, "adec is null");
        return -1;
    }
    if (!adec->codec_data) {
        LOGE(0x4b5, "adec->codec_data is null");
        return -1;
    }
    *value = ((PCM_DATA_T *)adec->codec_data)->width;
    return 0;
}

/* @0x331d4 */
int lgadec_pcm_get_payload_size(void *a, uint32_t *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x4c4, "adec is null");
        return -1;
    }
    if (!adec->codec_data) {
        LOGE(0x4c5, "adec->codec_data is null");
        return -1;
    }
    *value = ((PCM_DATA_T *)adec->codec_data)->payload_size;
    return 0;
}

/* @0x33274 */
int lgadec_pcm_get_timestamp(void *a, uint64_t *value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x4d4, "adec is null");
        return -1;
    }
    if (!adec->codec_data) {
        LOGE(0x4d5, "adec->codec_data is null");
        return -1;
    }
    *value = ((PCM_DATA_T *)adec->codec_data)->timestamp;
    return 0;
}

/* @0x33318 */
int lgadec_pcm_set_timestamp(void *a, uint64_t value)
{
    lgadec *adec = a;

    if (!adec) {
        LOGE(0x4e4, "adec is null");
        return -1;
    }
    if (!adec->codec_data) {
        LOGE(0x4e5, "adec->codec_data is null");
        return -1;
    }
    ((PCM_DATA_T *)adec->codec_data)->timestamp = value;
    return 0;
}

/* @0x333bc: DVD-Video LPCM private header (bytes 2..4 of the private stream header).
 * *new_header = 1 when it changed: the element then re-sets its src caps. */
int lgadec_dvdlpcm_header_check(void *a, unsigned char *data, int *new_header)
{
    lgadec *adec = a;
    PCM_DATA_T *pcm;
    uint32_t header;

    if (!adec) {
        LOGE(0x52f, "adec is null");
        return -1;
    }
    pcm = adec->codec_data;
    header = (data[2] & 0xc0) << 16 | data[3] << 8 | data[4];
    if (pcm->header == header) {
        *new_header = 0;
        return 0;
    }
    /* _dvdlpcm_parse_header (inlined) */
    switch (header & 0xc000) {                 /* quantization word length */
    case 0x4000: pcm->width = 20; pcm->out_width = 24; break;
    case 0x8000: pcm->width = 24; pcm->out_width = 24; break;
    default: pcm->width = 16; pcm->out_width = 16; break;
    }
    switch (header & 0x3000) {
    case 0:      pcm->frequency = 48000; break;
    case 0x1000: pcm->frequency = 96000; break;
    case 0x2000: pcm->frequency = 44100; break;
    case 0x3000: pcm->frequency = 32000; break;
    }
    pcm->header = header;
    pcm->channels = ((header >> 8) & 7) + 1;
    LOGT(0x54e, "frequency=%d, channels=%d, out_width=%d", pcm->frequency, pcm->channels,
         pcm->out_width);
    adec->stream_info.freq = pcm->frequency;
    adec->stream_info.channels = pcm->channels;
    adec->stream_info.bits_per_sample = pcm->out_width;
    LOGI(0x556, "SRC pad's audio caps is not set yet. Set audio caps of SRC pad (freq=%d, "
         "channels=%u, bps=%u)", pcm->frequency, pcm->channels & 0xff, pcm->out_width & 0xff);
    *new_header = 1;
    return 0;
}

/* @0x33540: DVD LPCM (big endian) -> little endian. 20-bit: 4 samples in 10 bytes -> 24-bit
 * into dest_buf; 24-bit (in place, 12-byte groups) and 16-bit (in place byte swap). */
int lgadec_dvdlpcm_resampling(void *a, unsigned char *data, unsigned int data_len,
                              unsigned long long timestamp, unsigned char *dest_buf)
{
    lgadec *adec = a;
    PCM_DATA_T *pcm;

    if (!adec) {
        LOGE(0x570, "adec is null");
        return -1;
    }
    pcm = adec->codec_data;
    if (!pcm || !data_len || !data) {
        LOGE(0x575, "parameters Err!! (data:0x%p, data len:%u, pcm_data:0x%p", data, data_len, pcm);
        return -1;
    }
    if (!pcm->frequency) {
        LOGE(0x57b, "NOT negotiated");
        return -1;
    }
    pcm->timestamp = timestamp;

    switch (pcm->width) {
    case 20: {
        unsigned int count = data_len / 10;

        if (!dest_buf) {
            LOGE(0x5a0, "parameters Err!! (dest_buf:0x%p)", NULL);
            return -1;
        }
        /* Preserve the source write order: overlapping input/output observes these stores. */
        for (unsigned int i = 0; count > i; i++, data += 10, dest_buf += 12) {
            dest_buf[2] = data[0];
            dest_buf[1] = data[1];
            dest_buf[0] = data[8] & 0xf0;
            dest_buf[5] = data[2];
            dest_buf[4] = data[3];
            dest_buf[3] = data[8] << 4;
            dest_buf[8] = data[4];
            dest_buf[7] = data[5];
            dest_buf[6] = data[9] & 0xf;           /* sic: not shifted like [0] */
            dest_buf[11] = data[6];
            dest_buf[10] = data[7];
            dest_buf[9] = data[9] << 4;
        }
        return 0;
    }

    case 24: {
        unsigned int count = data_len / 12;

        if (!pcm->channels) {
            LOGE(0x5c9, "Invalid channels(%u)", 0);
            return -1;
        }
        if (data_len / pcm->channels < 3) {
            LOGE(0x5cf, "data_len(%u) is too small. Dropping.", data_len);
            return -1;
        }
        /* 4 x 16-bit MSB words followed by 4 LSB bytes -> 4 x 24-bit LE */
        for (unsigned int i = 0; count > i; i++, data += 12) {
            uint8_t temp = data[8];
            uint8_t first, last;

            data[8] = data[4];
            first = data[0];
            last = data[11];
            data[0] = temp;
            data[4] = data[3];
            data[3] = data[9];
            data[11] = data[6];
            temp = data[2];
            data[9] = last;
            data[6] = data[10];
            data[10] = data[7];
            data[7] = data[5];
            data[2] = first;
            data[5] = temp;
        }
        return 0;
    }

    case 16:
        if (!pcm->channels) {
            LOGE(0x587, "Invalid channels(%u)", 0);
            return -1;
        }
        if (data_len / pcm->channels < 2) {
            LOGE(0x58d, "data_len(%u) is too small. Dropping.", data_len);
            return -1;
        }
        for (unsigned int i = 0; i + 1 < data_len; i += 2) {
            uint8_t t = data[i];

            data[i] = data[i + 1];
            data[i + 1] = t;
        }
        return 0;

    default:
        LOGE(0x5ec, "Invalid sample width");
        return -1;
    }
}

/* inlined: Blu-ray LPCM 4-byte header (payload size, channel assignment, rate, bits) */
static int _bdlpcm_parse_header(PCM_DATA_T *pcm, uint32_t header)
{
    unsigned int ch, freq, width, out_width;

    switch ((header >> 12) & 0xf) {
    case 1:  ch = 1; break;
    case 3:  ch = 2; break;
    case 4: case 5: ch = 3; break;
    case 6: case 7: ch = 4; break;
    case 8:  ch = 5; break;
    case 9:  ch = 6; break;
    case 10: ch = 7; break;
    case 11: ch = 8; break;
    default: goto err;
    }
    pcm->channels = ch;
    freq = (header >> 8) & 0xf;
    if (freq == 4)
        freq = 96000;
    else if (freq == 5)
        freq = 192000;
    else if (freq == 1)
        freq = 48000;
    else
        goto err;
    pcm->frequency = freq;
    width = (header >> 6) & 3;
    if (width == 2) {
        width = 20;
        out_width = 24;
    } else if (width == 3) {
        width = 24;
        out_width = 24;
    } else if (width == 1) {
        width = 16;
        out_width = 16;
    } else {
        goto err;
    }
    pcm->width = width;
    pcm->out_width = out_width;
    pcm->header = header;
    pcm->payload_size = header >> 16;
    LOGT(0x68a, "frequency=%u, channels=%u, width=%u(org=%u), channelPad=%u, startFlag=%u",
            freq, ch, out_width, width, ch & 1, (header >> 5) & 1);
    return 0;
err:
    LOGE(0x692, "BD LPCM header error");
    return -1;
}

/* @0x33900: header_flag 0 = same header, 1 = changed (caller re-negotiates), 2 = first */
int lgadec_bdlpcm_header_check(void *a, unsigned char *data, unsigned int rd_offset, int *header_flag)
{
    lgadec *adec = a;
    PCM_DATA_T *pcm;
    uint32_t header;

    if (!adec) {
        LOGE(0x6a0, "adec is null");
        return -1;
    }
    pcm = adec->codec_data;
    *header_flag = 0;
    header = (uint32_t)data[rd_offset] << 24 | data[rd_offset + 1] << 16 |
             data[rd_offset + 3] | data[rd_offset + 2] << 8;
    if (pcm->header) {
        if (((pcm->header ^ header) & 0xffc0) == 0)
            return 0;
        LOGW(0x6ca, "BD LPCM header is different");
        *header_flag = 1;
        return 0;
    }
    if (_bdlpcm_parse_header(pcm, header) != 0) {
        LOGE(0x6c0, "_bdlpcm_parse_header() Err");
        return -1;
    }
    adec->stream_info.channels = pcm->channels;
    adec->stream_info.bits_per_sample = pcm->out_width;
    adec->stream_info.freq = pcm->frequency;
    *header_flag = 2;
    return 0;
}

static int _wifilpcm_parse_header(PCM_DATA_T *pcm, uint32_t header)
{
    uint32_t freq, channels, width, frames;

    if (header >> 24 != 0xa0) {
        LOGE(0x6ee, "sub stream ID is not 0xA0");
        return -1;
    }
    frames = (header >> 16) & 0xff;
    LOGT(0x6f4, "number of audio frame is %u", frames);
    width = (header >> 6) & 3;
    if (width != 0) {
        LOGE(0x6f9, "width info is error");
        return -1;
    }
    width = 16;
    switch ((header >> 3) & 7) {
    case 1:  freq = 44100; break;
    case 2:  freq = 48000; break;
    default:
        LOGE(0x708, "frequency info is error");
        return -1;
    }
    channels = header & 7;
    if (channels > 1) {
        LOGE(0x714, "channel info is error");
        return -1;
    }
    channels = 2;
    pcm->width = width;
    pcm->out_width = width;
    pcm->header = header;
    pcm->frequency = freq;
    pcm->payload_size = frames * 320;
    pcm->channels = channels;
    return 0;
}

/* @0x33b18: Wi-Fi Display LPCM (sub stream 0xA0): always 2ch/16 bit, 44.1 or 48 kHz */
int lgadec_wifilpcm_header_check(void *a, unsigned char *data, unsigned int rd_offset, int *header_flag)
{
    lgadec *adec = a;
    PCM_DATA_T *pcm;
    const uint8_t *h;
    uint32_t header;

    if (!adec) {
        LOGE(0x72e, "adec is null");
        return -1;
    }
    pcm = adec->codec_data;
    *header_flag = 0;
    h = data + rd_offset;
    header = (uint32_t)h[0] << 24 | h[1] << 16 | h[3] | h[2] << 8;
    if (pcm->header) {
        if (((pcm->header ^ header) & 0xffff00ff) == 0)
            return 0;
        LOGW(0x75d, "wifi LPCM header is different");
        *header_flag = 1;
        return 0;
    }
    if (_wifilpcm_parse_header(pcm, header) != 0) {
        LOGE(0x750, "_wifilpcm_parse_header() Err");
        return -1;
    }
    adec->stream_info.freq = pcm->frequency;
    adec->stream_info.channels = pcm->channels;
    adec->stream_info.bits_per_sample = pcm->out_width;
    *header_flag = 2;
    return 0;
}

/* @0x33d40: BD / Wi-Fi LPCM big endian -> little endian into out_data */
int lgadec_bd_wifi_lpcm_resampling(void *a, unsigned char *data, unsigned int data_len,
                                   unsigned char *out_data)
{
    lgadec *adec = a;
    PCM_DATA_T *pcm;

    if (!adec) {
        LOGE(0x76c, "adec is null");
        return -1;
    }
    pcm = adec->codec_data;
    if (!pcm) {
        LOGE(0x76d, "adec->codec_data is null");
        return -1;
    }
    if (pcm->out_width == 16) {
        for (unsigned int i = 0; i + 1 < data_len; i += 2) {
            out_data[i] = data[i + 1];
            out_data[i + 1] = data[i];
        }
    } else if (pcm->out_width == 24) {
        for (unsigned int i = 0; i + 2 < data_len; i += 3) {
            out_data[i] = data[i + 2];
            out_data[i + 1] = data[i + 1];
            out_data[i + 2] = data[i];
        }
    } else {
        LOGE(0x786, "Invalid sample width");
        return -1;
    }
    return 0;
}

/* @0x33e64: G.711 mu-law -> S16 */
int lgadec_mulaw_resampling(void *a, unsigned char *data, unsigned int data_len, short *pcm_data)
{
    const int16_t expLut[8] = { 0, 132, 396, 924, 1980, 4092, 8316, 16764 };

    if (!a) {
        LOGE(0x79b, "adec is null");
        return -1;
    }
    for (unsigned int i = 0; i < data_len; i++) {
        unsigned int u = ~data[i] & 0xff;
        unsigned int exponent = (u & 0x7f) >> 4;
        int16_t s = ((u & 0xf) << (exponent + 3)) + expLut[exponent];

        pcm_data[i] = (u & 0x80) ? -s : s;
    }
    return 0;
}

/* @0x33f10: G.711 A-law -> S16 (table copied onto the stack in the original) */
int lgadec_alaw_resampling(void *a, unsigned char *data, unsigned int data_len, short *pcm_data)
{
    const int16_t trans_table_alaw_to_signed_16bits[256] = {
         -5504,  -5248,  -6016,  -5760,  -4480,  -4224,  -4992,  -4736,
         -7552,  -7296,  -8064,  -7808,  -6528,  -6272,  -7040,  -6784,
         -2752,  -2624,  -3008,  -2880,  -2240,  -2112,  -2496,  -2368,
         -3776,  -3648,  -4032,  -3904,  -3264,  -3136,  -3520,  -3392,
        -22016, -20992, -24064, -23040, -17920, -16896, -19968, -18944,
        -30208, -29184, -32256, -31232, -26112, -25088, -28160, -27136,
        -11008, -10496, -12032, -11520,  -8960,  -8448,  -9984,  -9472,
        -15104, -14592, -16128, -15616, -13056, -12544, -14080, -13568,
          -344,   -328,   -376,   -360,   -280,   -264,   -312,   -296,
          -472,   -456,   -504,   -488,   -408,   -392,   -440,   -424,
           -88,    -72,   -120,   -104,    -24,     -8,    -56,    -40,
          -216,   -200,   -248,   -232,   -152,   -136,   -184,   -168,
         -1376,  -1312,  -1504,  -1440,  -1120,  -1056,  -1248,  -1184,
         -1888,  -1824,  -2016,  -1952,  -1632,  -1568,  -1760,  -1696,
          -688,   -656,   -752,   -720,   -560,   -528,   -624,   -592,
          -944,   -912,  -1008,   -976,   -816,   -784,   -880,   -848,
          5504,   5248,   6016,   5760,   4480,   4224,   4992,   4736,
          7552,   7296,   8064,   7808,   6528,   6272,   7040,   6784,
          2752,   2624,   3008,   2880,   2240,   2112,   2496,   2368,
          3776,   3648,   4032,   3904,   3264,   3136,   3520,   3392,
         22016,  20992,  24064,  23040,  17920,  16896,  19968,  18944,
         30208,  29184,  32256,  31232,  26112,  25088,  28160,  27136,
         11008,  10496,  12032,  11520,   8960,   8448,   9984,   9472,
         15104,  14592,  16128,  15616,  13056,  12544,  14080,  13568,
           344,    328,    376,    360,    280,    264,    312,    296,
           472,    456,    504,    488,    408,    392,    440,    424,
            88,     72,    120,    104,     24,      8,     56,     40,
           216,    200,    248,    232,    152,    136,    184,    168,
          1376,   1312,   1504,   1440,   1120,   1056,   1248,   1184,
          1888,   1824,   2016,   1952,   1632,   1568,   1760,   1696,
           688,    656,    752,    720,    560,    528,    624,    592,
           944,    912,   1008,    976,    816,    784,    880,    848,
    };

    if (!a) {
        LOGE(0x7d7, "adec is null");
        return -1;
    }
    for (unsigned int i = 0; i < data_len; i++)
        pcm_data[i] = trans_table_alaw_to_signed_16bits[data[i]];
    return 0;
}

/* @0x33fa4 */
void *lgadec_flac_create(void)
{
    void *h;

    LOGI(0x800, "flac create");
    h = calloc(1, sizeof(FLAC_DATA_T));
    if (!h)
        LOGE(0x804, "memory alloc fail!");
    return h;
}

/* @0x34028 */
void *lgadec_vorbis_create(void)
{
    void *h = calloc(1, sizeof(VORBIS_DATA_T));

    if (!h)
        LOGE(0x895, "memory alloc fail!");
    return h;
}

/* @0x3407c */
void *lgadec_opus_create(void)
{
    void *h = calloc(1, sizeof(OPUS_DATA_T));

    if (!h)
        LOGE(0x8fd, "memory alloc fail!");
    return h;
}

/* @0x340d0 */
void *lgadec_iamf_create(void)
{
    void *h = calloc(1, sizeof(IAMF_DATA_T));

    if (!h)
        LOGE(0x973, "memory alloc fail!");
    return h;
}

/* ---- codec tables (selected by lgadec_select_codec) --------------------------- */
#define SELECTED_CODEC(name, ...) \
    SELECTED_CODEC_T name = { __VA_ARGS__ }; \
    static SELECTED_CODEC_T *__selected_codec_##name __attribute__((used)) = &name

SELECTED_CODEC(codec_type_iamf, _iamf_init, _cleanup, _iamf_feed, _iamf_flush);
SELECTED_CODEC(codec_type_opus, _opus_init, _cleanup, _opus_feed, _opus_flush);
SELECTED_CODEC(codec_type_vorbis, _vorbis_init, _cleanup, _vorbis_feed, _vorbis_flush);
SELECTED_CODEC(codec_type_flac, _flac_init, _cleanup, _flac_feed, NULL);
SELECTED_CODEC(codec_type_pcm, _pcm_init, _cleanup, NULL, NULL);      /* host-side only */
SELECTED_CODEC(codec_type_adpcm, _adpcm_init, _adpcm_cleanup, NULL, NULL);
SELECTED_CODEC(codec_type_ac4, _ac4_init, _cleanup, _ac4_feed, NULL);
SELECTED_CODEC(codec_type_aac, _aac_init, _cleanup, _aac_feed, NULL);
SELECTED_CODEC(codec_type_private, _init, _cleanup, _private_feed, NULL);
SELECTED_CODEC(codec_type_none, _init, _cleanup, _raw_feed, NULL);
