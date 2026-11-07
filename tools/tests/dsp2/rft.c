/* Standalone: cc -std=c11 -Wall -Wextra -Werror -pedantic
 * -fsanitize=address,undefined tools/tests/dsp2/rft.c -o build/scratch/test-rft
 * && build/scratch/test-rft
 * Mocks test dispatch ONLY: neither mock claims recovered SIMD arithmetic. */
#include "../../../src/dsp2/rft.c"
#include <stdio.h>

static int gain_calls, lfe_calls;
static int32_t last_gain;
int32_t rft_gain_sample_flix(int32_t sample, int32_t gain)
{
    ++gain_calls;
    last_gain = gain;
    return rft_bits((uint32_t)sample ^ UINT32_C(0x00010001));
}
void rft_lfe_filter_flix(struct rft_config *r, int32_t *mono, size_t frames)
{
    ++lfe_calls;
    for (size_t i = 0; i < frames; ++i)
        mono[i] = rft_bits((uint32_t)mono[i] ^ UINT32_C(0x01000000));
    r->lfe_history[0] = 123;
}

static struct rft_config config(int in_bytes, int in_ch, int out_bytes, int out_ch, int frames)
{
    struct rft_config r = {0};
    r.in_bytes = in_bytes; r.in_ch = in_ch;
    r.out_bytes = out_bytes; r.out_ch = out_ch;
    r.processing_frames = frames;
    r.acmod = -1;
    return r;
}

static void formats(void)
{
    uint8_t input[37 * 4], aligned[37 * 4], result[37 * 4];
    for (size_t i = 0; i < sizeof(input); ++i) input[i] = (uint8_t)(i * 197u + 19u);
    for (int bytes = 1; bytes <= 4; ++bytes) {
        struct rft_config r = config(bytes, 1, bytes, 1, 37);
        assert(_LGRFT_AlignSignedto32(&r, input, aligned) == 0);
        for (size_t i = 0; i < 37; ++i) {
            for (int j = 0; j < 4 - bytes; ++j) assert(aligned[i * 4 + (size_t)j] == 0);
            assert(memcmp(aligned + i * 4 + (size_t)(4 - bytes), input + i * (size_t)bytes, (size_t)bytes) == 0);
        }
        assert(_LGRFT_WriteOut(&r, aligned, result) == 0);
        assert(memcmp(input, result, 37u * (unsigned)bytes) == 0);
        assert(_LGRFT_AlignUnsignedto32(&r, input, aligned) == 0);
        assert(_LGRFT_WriteOut(&r, aligned, result) == 0);
        for (size_t i = 0; i < 37; ++i)
            for (int j = 0; j < bytes; ++j)
                assert(result[i * (size_t)bytes + (size_t)j] ==
                       (uint8_t)(input[i * (size_t)bytes + (size_t)j] ^ (j == bytes - 1 ? 0x80 : 0)));
        memset(result, 0, sizeof(result));
        assert(_LGRFT_SwapEndian(&r, input, result) == 0);
        for (size_t i = 0; i < 37; ++i)
            for (int j = 0; j < bytes; ++j)
                assert(result[i * (size_t)bytes + (size_t)j] == input[i * (size_t)bytes + (size_t)(bytes - 1 - j)]);
        assert(_LGRFT_SwapEndian(&r, result, result) == 0);
        assert(memcmp(input, result, 37u * (unsigned)bytes) == 0);
    }
    struct rft_config bad = config(5, 1, 5, 1, 1);
    assert(_LGRFT_AlignSignedto32(&bad, input, result) == -1);
    assert(_LGRFT_AlignUnsignedto32(&bad, input, result) == -1);
    assert(_LGRFT_WriteOut(&bad, input, result) == -1);
    assert(_LGRFT_SwapEndian(&bad, input, result) == 0);
}

static void signed16_exhaustive(void)
{
    uint8_t input[256 * 8 * 2], output[256 * 8 * 4], scratch[256 * 8 * 4];
    struct rft_config r = config(2, 8, 2, 8, 256);
    for (unsigned base = 0; base < 65536; base += 2048) {
        for (unsigned i = 0; i < 2048; ++i) {
            unsigned v = base + i;
            input[i * 2] = (uint8_t)v; input[i * 2 + 1] = (uint8_t)(v >> 8);
        }
        memset(scratch, 0xa5, sizeof(scratch));
        assert(LGRFT(&r, input, output, scratch, 0) == 2048);
        assert(memcmp(input, output, sizeof(input)) == 0);
        for (size_t i = 0; i < sizeof(scratch); ++i) assert(scratch[i] == 0xa5);
        assert(_LGRFT_AlignSignedto32(&r, input, scratch) == 0);
        assert(_LGRFT_WriteOut(&r, scratch, output) == 0);
        assert(memcmp(input, output, sizeof(input)) == 0);
        r.out_bytes = 4;
        assert(LGRFT(&r, input, output, scratch, 0) == 2048);
        for (unsigned i = 0; i < 2048; ++i) {
            assert(output[i * 4] == 0 && output[i * 4 + 1] == 0);
            assert(output[i * 4 + 2] == input[i * 2] && output[i * 4 + 3] == input[i * 2 + 1]);
        }
        r.out_bytes = 2;
    }
    assert(gain_calls == 0);
    assert(LGRFT(NULL, input, output, scratch, 0) == -1);
    assert(LGRFT(&r, NULL, output, scratch, 0) == -1);
    assert(LGRFT(&r, input, NULL, scratch, 0) == -1);
    assert(LGRFT(&r, input, output, NULL, 0) == -1);
}

static void copy_bounds_and_failures(void)
{
    uint8_t input[122], output[124];
    int32_t scratch[30];
    const int enables[] = {-1, 0, 2};
    for (size_t index = 0; index < sizeof(input); ++index)
        input[index] = (uint8_t)(index * 197u + 19u);
    for (int bytes = 1; bytes <= 5; ++bytes) {
        for (size_t mode = 0; mode < sizeof(enables) / sizeof(enables[0]); ++mode) {
            for (int available = 0; available <= 1; ++available) {
                struct rft_config state = config(bytes, 8, bytes, 8, 3);
                state.gain_enable = enables[mode];
                state.gain = 0x800000;
                state.acmod = 1;
                state.gen_lfe = 1;
                struct rft_config before = state;
                memset(output, 0xa5, sizeof(output));
                memset(scratch, 0x5a, sizeof(scratch));
                assert(LGRFT(&state, input + 1, output + 2, scratch, available) == 24);
                size_t length = 24u * (unsigned)bytes;
                assert(memcmp(output + 2, input + 1, length) == 0);
                assert(output[0] == 0xa5 && output[1] == 0xa5);
                for (size_t index = length + 2; index < sizeof(output); ++index)
                    assert(output[index] == 0xa5);
                for (size_t index = 0; index < sizeof(scratch) / sizeof(scratch[0]); ++index)
                    assert(scratch[index] == INT32_C(0x5a5a5a5a));
                assert(memcmp(&state, &before, sizeof(state)) == 0);
            }
        }
    }
    for (int failure = 0; failure < 6; ++failure) {
        struct rft_config state = config(2, failure == 4 ? 11 : 8,
                                         2, failure == 5 ? 11 : 8, 3);
        memset(output, 0xa5, sizeof(output));
        memset(scratch, 0x5a, sizeof(scratch));
        assert(LGRFT(failure == 0 ? NULL : &state,
                     failure == 1 ? NULL : input + 1,
                     failure == 2 ? NULL : output + 2,
                     failure == 3 ? NULL : scratch, 0) == -1);
        for (size_t index = 0; index < sizeof(output); ++index)
            assert(output[index] == 0xa5);
        for (size_t index = 0; index < sizeof(scratch) / sizeof(scratch[0]); ++index)
            assert(scratch[index] == INT32_C(0x5a5a5a5a));
    }
    struct rft_config empty = config(2, 8, 2, 8, 0);
    assert(LGRFT(&empty, input + 1, output + 2, scratch, 0) == 0);
    for (size_t index = 0; index < sizeof(output); ++index)
        assert(output[index] == 0xa5);
    for (size_t index = 0; index < sizeof(input); ++index)
        assert(input[index] == (uint8_t)(index * 197u + 19u));
}

static void ignored_format_failures(void)
{
    const uint8_t input[] = {0x34, 0x92, 0x56, 0x78, 0x9a};
    uint8_t output[8];
    int32_t scratch = INT32_C(0x12345678);
    struct rft_config state = config(5, 1, 2, 1, 1);
    memset(output, 0xa5, sizeof(output));
    assert(LGRFT(&state, input, output + 1, &scratch, 0) == 1);
    assert(scratch == INT32_C(0x12345678));
    assert(output[0] == 0xa5 && output[1] == 0x34 && output[2] == 0x12);
    for (size_t index = 3; index < sizeof(output); ++index)
        assert(output[index] == 0xa5);
    state.in_bytes = 2;
    state.out_bytes = 5;
    memset(output, 0xa5, sizeof(output));
    assert(LGRFT(&state, input, output + 1, &scratch, 0) == 1);
    assert((uint32_t)scratch == UINT32_C(0x92340000));
    for (size_t index = 0; index < sizeof(output); ++index)
        assert(output[index] == 0xa5);
}

static void dispatch(void)
{
    uint16_t input[16] = {0x1234, 0x8000, 0xffff};
    uint16_t output[16] = {0};
    int32_t scratch[16];
    struct rft_config r = config(2, 8, 2, 8, 2);
    r.gain_enable = 2;
    assert(LGRFT(&r, input, output, scratch, 0) == 16);
    assert(memcmp(input, output, sizeof(input)) == 0 && gain_calls == 0);
    r.gain_enable = 1; r.gain = 0x800000;
    assert(LGRFT(&r, input, output, scratch, 0) == 16);
    assert(gain_calls == 16 && last_gain == 0x800000);
    for (size_t i = 0; i < 16; ++i) assert(output[i] == (uint16_t)(input[i] ^ 1));
    r.gain_enable = 0; r.in_unsigned = 2;
    memset(scratch, 0xa5, sizeof(scratch));
    assert(LGRFT(&r, input, output, scratch, 0) == 16);
    assert(memcmp(input, output, sizeof(input)) == 0);
    assert((uint32_t)scratch[0] == UINT32_C(0x12340000));
    r.in_unsigned = 0; r.in_endian = 1;
    assert(LGRFT(&r, input, output, scratch, 0) == 16);
    for (size_t i = 0; i < 16; ++i)
        assert(output[i] == (uint16_t)((input[i] << 8) | (input[i] >> 8)));
    r.in_endian = 0; r.in_unsigned = 1;
    assert(LGRFT(&r, input, output, scratch, 0) == 16);
    for (size_t i = 0; i < 16; ++i) assert(output[i] == (uint16_t)(input[i] ^ 0x8000));

    r = config(4, 2, 4, 3, 2); r.gen_lfe = 1; r.in_endian = 1;
    int32_t stereo[4] = {-3, 3, INT32_MIN, INT32_MAX}, three[6];
    assert(LGRFT(&r, stereo, three, scratch, 0) == 6);
    assert(lfe_calls == 1 && r.lfe_history[0] == 123);
    assert(three[0] == -3 && three[1] == 3);
    assert(three[2] == rft_bits(UINT32_C(0xffffffff) ^ UINT32_C(0x01000000)));
    assert(three[3] == INT32_MIN && three[4] == INT32_MAX);
    assert(three[5] == three[2]);
    int before_gain = gain_calls;
    r.gain_enable = 1;
    assert(LGRFT(&r, stereo, three, scratch, 0) == 6);
    assert(gain_calls - before_gain == 4 && lfe_calls == 2);
    assert(three[0] == rft_bits((uint32_t)stereo[0] ^ UINT32_C(0x00010001)));

    r = config(4, 3, 4, 2, 1); r.acmod = 0;
    int32_t multichannel[3] = {10,20,30};
    assert(LGRFT(&r, multichannel, three, scratch, 0) == 2);
    assert(three[0] == 15 && three[1] == 15);
    assert(LGRFT(&r, multichannel, three, scratch, 1) == 2);
    assert(three[0] == 10 && three[1] == 20);
}

static void channels(void)
{
    int32_t src[20] = {256,512,768,1024,1280,1536,1792,2048,2304,2560};
    int32_t dst[32] = {0};
    struct rft_config r = config(4, 8, 4, 2, 1);
    _LGRFT_DownMix(&r, src, dst);
    assert(dst[0] == 3*58 + 82 + 5*58 + 7*57);
    assert(dst[1] == 3*58 + 2*82 + 6*58 + 8*57);
    r.in_ch = 10; _LGRFT_DownMix(&r, src, dst);
    assert(dst[0] == 3*58 + 82 + 5*58 + 7*57);
    r.in_ch = 6; _LGRFT_DownMix(&r, src, dst);
    assert(dst[0] == 3*75 + 106 + 5*75 && dst[1] == 3*75 + 2*106 + 6*75);
    r.in_ch = 4; _LGRFT_DownMix(&r, src, dst);
    assert(dst[0] == 3*75 + 4*75 + 106 && dst[1] == 3*75 + 4*75 + 2*106);
    r.in_ch = 3; _LGRFT_DownMix(&r, src, dst);
    assert(dst[0] == 256 && dst[1] == 512);
    for (int acmod = 0; acmod <= 2; ++acmod) {
        r.in_ch = 6; r.acmod = acmod;
        assert(_LGRFT_ConvertCh(&r, src, dst) == 0);
        assert(dst[0] == (acmod == 0 ? 384 : acmod == 1 ? 768 : 256));
        assert(dst[1] == (acmod == 0 ? 384 : acmod == 1 ? 768 : 512));
    }
    r = config(4, 1, 4, 2, 3);
    int32_t mono[12] = {11,22,33};
    assert(_LGRFT_ConvertCh(&r, mono, mono) == 0);
    for (size_t i = 0; i < 3; ++i) assert(mono[i*2] == (int32_t)((i+1)*11) && mono[i*2+1] == mono[i*2]);
    r = config(4, 3, 4, 1, 1); src[0] = -3; src[1] = 3;
    assert(_LGRFT_ConvertCh(&r, src, dst) == 0 && dst[0] == -1);
    r = config(4, 3, 4, 5, 2);
    int32_t expand[12] = {1,2,3,4,5,6};
    assert(_LGRFT_ConvertCh(&r, expand, expand) == -1);
    int32_t expected[10] = {1,2,3,0,0,4,5,6,0,0};
    assert(memcmp(expand, expected, sizeof(expected)) == 0);
    assert(_LGRFT_ConvertAvailableCh(&r, src, dst) == 0);
    assert(dst[0] == -3 && dst[1] == 3 && dst[2] == 768 && dst[3] == 0 && dst[4] == 0);
    int32_t forward[12] = {1,2,3,4,5,6};
    assert(_LGRFT_ConvertAvailableCh(&r, forward, forward) == 0);
    int32_t overwritten[10] = {1,2,3,0,0,0,0,0,0,0};
    assert(memcmp(forward, overwritten, sizeof(overwritten)) == 0);
    memset(dst, 0x7f, sizeof(dst));
    _LGRFT_UpMix(&r, dst, src);
    assert(dst[0] == -3 && dst[1] == 3 && dst[2] == 768 && dst[3] == 0 && dst[4] == 0);
    r = config(4, 5, 4, 3, 2);
    assert(_LGRFT_ConvertCh(&r, expected, dst) == 0);
    int32_t reduced[6] = {1,2,3,4,5,6};
    assert(memcmp(dst, reduced, sizeof(reduced)) == 0);
    assert(_LGRFT_ConvertCh(&r, NULL, dst) == -1);
    assert(_LGRFT_ConvertAvailableCh(&r, src, NULL) == -1);
}

static void gain_empty_ranges(void)
{
    struct rft_config state = config(4, 8, 4, 8, 256);
    assert(RFT_SetGai(&state, 1, 0x800000) == 0);
    state.lfe_history[0] = 12345;
    struct rft_config before = state;
    const int32_t counts[] = {0, -1, INT32_MIN};
    for (size_t index = 0; index < sizeof(counts) / sizeof(counts[0]); ++index) {
        int32_t output[] = {INT32_MIN, 123, INT32_MAX};
        const int32_t expected[] = {INT32_MIN, 123, INT32_MAX};
        assert(_LGRFT_Gai(&state, NULL, output, counts[index]) == 0);
        assert(memcmp(output, expected, sizeof(output)) == 0);
        assert(_LGRFT_Gai(&state, NULL, NULL, counts[index]) == 0);
        assert(memcmp(&state, &before, sizeof(state)) == 0);
    }
}

static void properties(void)
{
    struct rft_config r = {0};
    assert(RFT_SetInPCMFormat(NULL, 16) == -1);
    assert(RFT_SetInChannel(NULL, 2) == -1);
    assert(RFT_SetInSigned(NULL, 0) == -1);
    assert(RFT_SetInEndian(NULL, 0) == -1);
    assert(RFT_SetAcmod(NULL, 0) == -1);
    assert(RFT_SetGenLFE(NULL, 0) == -1);
    assert(RFT_SetOutPCMFormat(NULL, 16) == -1);
    assert(RFT_SetOutChannel(NULL, 2) == -1);
    assert(RFT_SetGai(NULL, 1, 123) == -1);
    assert(RFT_SetOutSample(NULL, 32) == -1);
    assert(RFT_SetInPCMFormat(&r, 23) == 0 && RFT_GetInPCMFormat(&r) == 16);
    assert(RFT_SetOutPCMFormat(&r, -1) == 0 && RFT_GetOutPCMFormat(&r) == -8);
    assert(RFT_SetInChannel(&r, 11) == 0 && RFT_GetInChannel(&r) == 11);
    assert(LGRFT_CheckConfig(&r) == -1);
    assert(RFT_SetInChannel(&r, -1) == 0 && LGRFT_CheckConfig(&r) == 0);
    assert(RFT_SetOutChannel(&r, 11) == 0 && LGRFT_CheckConfig(&r) == -1);
    assert(RFT_SetOutChannel(&r, 8) == 0 && RFT_GetOutChannel(&r) == 8);
    assert(RFT_SetInChannel(&r, 8) == 0);
    assert(RFT_SetInSigned(&r, 7) == 0 && RFT_GetInSigned(&r) == 7);
    assert(RFT_SetInEndian(&r, 1) == 0 && RFT_GetInEndian(&r) == 1);
    assert(RFT_SetInEndian(&r, 2) == -1 && RFT_GetInEndian(&r) == 1);
    assert(RFT_SetAcmod(&r, 9) == 0 && RFT_GetAcmod(&r) == 9);
    memset(r.lfe_history, 0xff, sizeof(r.lfe_history)); r.reserved[0] = 42;
    assert(RFT_SetGenLFE(&r, -1) == 0 && RFT_GetGenLFE(&r) == 1 && r.reserved[0] == 42);
    for (size_t i = 0; i < 4; ++i) assert(r.lfe_history[i] == 0);
    assert(RFT_SetGai(&r, 1, 456) == 0 && RFT_GetGainEnable(&r) == 1 && RFT_GetGai(&r) == 456);
    assert(RFT_SetGai(&r, 2, 999) == 0 && RFT_GetGainEnable(&r) == 0 && RFT_GetGai(&r) == 456);
    assert(RFT_SetOutSample(&r, 32) == 0 && RFT_GetNeedfulSample(&r) == 256);
    assert(RFT_GetOutSigned() == 0 && RFT_GetOutEndian() == 0);
    assert(LGRFT_CheckConfig(NULL) == -1);
}

int main(void)
{
    formats(); signed16_exhaustive(); copy_bounds_and_failures(); ignored_format_failures();
    dispatch(); channels(); gain_empty_ranges(); properties();
    puts("RFT: scalar formats, exhaustive signed16, dispatch, channel policies and properties passed; SIMD mocked");
    return 0;
}
