/* Standalone: cc -std=c11 -Wall -Wextra -Werror -pedantic
 * -fsanitize=address,undefined tools/tests/dsp2/lgsrc_io.c
 * -o build/scratch/lgsrc-io-test && build/scratch/lgsrc-io-test
 */
#include "../../../src/dsp2/lgsrc_io.c"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct lgsrc_state { unsigned char raw[0x19370]; };
static struct lgsrc_state state, expected;
static unsigned char input[4608 * 10 * 4 + 2];
static unsigned char output[512 * 10 * 4 + 2];
static unsigned char wanted[sizeof(output)];

static void put16(unsigned char *p, uint16_t x)
{
    p[0] = (unsigned char)x;
    p[1] = (unsigned char)(x / 256);
}

static void put32(unsigned char *p, uint32_t x)
{
    for (unsigned i = 0; i < 4; ++i) {
        p[i] = (unsigned char)x;
        x /= 256;
    }
}

static uint32_t returned_bits(int value)
{
    uint32_t bits;
    memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static uint16_t pattern(uint32_t frame, uint32_t channel)
{
    return (uint16_t)(frame * 7919 + channel * 104729 + 0x80ff);
}

static void read_case(unsigned bits, unsigned channels, unsigned buffered,
                      unsigned frames, uint32_t first, int exhaustive)
{
    unsigned bytes = bits / 8;
    memset(&state, 0xa5, sizeof(state));
    put32(state.raw + 4, channels);
    put32(state.raw + 8, bits);
    put32(state.raw + 12, buffered + frames);
    put16(state.raw + 32, (uint16_t)buffered);
    expected = state;
    memset(input, 0x5a, sizeof(input));
    for (unsigned frame = 0; frame < frames; ++frame) {
        for (unsigned ch = 0; ch < channels; ++ch) {
            uint32_t index = frame * channels + ch;
            uint16_t value = exhaustive ? (uint16_t)(first + index) : pattern(frame, ch);
            unsigned char *p = input + 1 + index * bytes;
            if (bytes == 1) {
                p[0] = (unsigned char)value;
                value = (uint16_t)((value & 255u) * 256u);
            } else {
                for (unsigned k = 0; k < bytes - 2; ++k)
                    p[k] = (unsigned char)(index * 37 + k * 139 + first);
                put16(p + bytes - 2, value);
            }
            put16(expected.raw + 0x40 + ch * 0x2800 + (buffered + frame) * 2,
                  value);
        }
    }
    assert(returned_bits(lgsrcIo_readData(input + 1, &state)) == channels * frames);
    assert(memcmp(&state, &expected, sizeof(state)) == 0);
    assert(input[0] == 0x5a && input[sizeof(input) - 1] == 0x5a);
}

static void write_case(unsigned bits, unsigned channels, unsigned frames,
                       uint32_t first, int exhaustive)
{
    unsigned bytes = bits / 8;
    memset(&state, 0xa5, sizeof(state));
    put32(state.raw + 20, channels);
    put32(state.raw + 24, bits);
    put32(state.raw + 28, frames);
    put16(state.raw + 36, (uint16_t)(frames ^ 0x155));
    memset(output, 0x5a, sizeof(output));
    memset(wanted, 0x5a, sizeof(wanted));
    for (unsigned frame = 0; frame < frames; ++frame) {
        for (unsigned ch = 0; ch < channels; ++ch) {
            uint32_t index = frame * channels + ch;
            uint16_t value = exhaustive ? (uint16_t)(first + index) : pattern(frame, ch);
            put16(state.raw + 0x2440 + ch * 0x2800 + frame * 2, value);
            unsigned char *p = wanted + 1 + index * bytes;
            if (bits == 8) {
                p[0] = (unsigned char)(value / 256);
            } else {
                uint32_t scaled = (uint32_t)value << (bits - 16);
                for (unsigned k = 0; k < bytes; ++k) {
                    p[k] = (unsigned char)scaled;
                    scaled /= 256;
                }
            }
        }
    }
    expected = state;
    uint32_t end = (uint32_t)(uintptr_t)(output + 1) + channels * frames * bytes;
    assert(returned_bits(lgsrcIo_writeData(output + 1, &state)) == end);
    assert(memcmp(output, wanted, sizeof(output)) == 0);
    assert(memcmp(&state, &expected, sizeof(state)) == 0);
}

static void boundary_tests(void)
{
    static const unsigned formats[] = {8, 16, 24, 32};
    static const unsigned counts[] = {0, 1, 2, 3, 7, 511, 512};
    for (unsigned f = 0; f < 4; ++f) {
        for (unsigned ch = 0; ch <= 10; ++ch) {
            for (unsigned n = 0; n < sizeof(counts) / sizeof(counts[0]); ++n) {
                read_case(formats[f], ch, 0, counts[n], 0, 0);
                read_case(formats[f], ch, 31, counts[n], 0, 0);
                write_case(formats[f], ch, counts[n], 0, 0);
            }
            read_case(formats[f], ch, 0, 4608, 0, 0);
            read_case(formats[f], ch, 4096, 512, 0, 0);
            read_case(formats[f], ch, 4608, 0, 0, 0);
        }
    }
}

static void exhaustive_tests(void)
{
    static const unsigned formats[] = {8, 16, 24, 32};
    for (unsigned f = 0; f < 4; ++f) {
        /* Every 16-bit plane value, with independently varying discarded low
         * bits for 24/32 PCM; 8-bit covers every value 256 times. */
        for (uint32_t first = 0; first < 65536; first += 512) {
            read_case(formats[f], 1, 29, 512, first, 1);
            write_case(formats[f], 1, 512, first, 1);
        }
    }
}

static void guard_tests(void)
{
    memset(&state, 0xa5, sizeof(state));
    for (uint32_t bits = 0; bits <= 64; ++bits) {
        if (bits == 8 || bits == 16 || bits == 24 || bits == 32)
            continue;
        put32(state.raw + 8, bits);
        put32(state.raw + 24, bits);
        expected = state;
        assert(lgsrcIo_readData(NULL, &state) == -1);
        assert(lgsrcIo_writeData(NULL, &state) == -1);
        assert(memcmp(&state, &expected, sizeof(state)) == 0);
    }
    put32(state.raw + 8, UINT32_MAX);
    put32(state.raw + 24, UINT32_MAX);
    assert(lgsrcIo_readData(NULL, &state) == -1);
    assert(lgsrcIo_writeData(NULL, &state) == -1);

    static const uint32_t values[] = {0, 1, 10, 0x7fffffff, 0x80000000, UINT32_MAX};
    for (unsigned bits = 8; bits <= 32; bits += 8) {
        put32(state.raw + 8, bits);
        put32(state.raw + 24, bits);
        for (unsigned c = 0; c < sizeof(values) / sizeof(values[0]); ++c) {
            uint32_t channels = values[c];
            put32(state.raw + 4, channels);
            put32(state.raw + 20, channels);
            for (unsigned n = 0; n < sizeof(values) / sizeof(values[0]); ++n) {
                uint32_t frames = values[n];
                /* Either dimension must fail its signed-positive guard. */
                if (channels != 0 && channels < 0x80000000 &&
                    frames != 0 && frames < 0x80000000)
                    continue;
                put16(state.raw + 32, 0);
                put32(state.raw + 12, frames);
                put32(state.raw + 28, frames);
                expected = state;
                assert(returned_bits(lgsrcIo_readData(NULL, &state)) == channels * frames);
                assert(returned_bits(lgsrcIo_writeData(NULL, &state)) == 0);
                assert(memcmp(&state, &expected, sizeof(state)) == 0);
            }
        }
        /* Missing input <0, including u32 subtraction that crosses INT_MAX. */
        put32(state.raw + 4, 10);
        put16(state.raw + 32, UINT16_MAX);
        put32(state.raw + 12, 0);
        assert(returned_bits(lgsrcIo_readData(NULL, &state)) == 10u * (0u - UINT16_MAX));
        put16(state.raw + 32, 1);
        put32(state.raw + 12, 0x80000001);
        assert(returned_bits(lgsrcIo_readData(NULL, &state)) == 0);
    }
}

static void exact_buffer_tests(void)
{
    /* ASan redzones directly follow the last caller-owned PCM byte. */
    for (unsigned bits = 8; bits <= 32; bits += 8) {
        for (unsigned channels = 1; channels <= 10; ++channels) {
            for (unsigned frames = 1; frames <= 3; ++frames) {
                size_t size = (size_t)channels * frames * (bits / 8);
                unsigned char *pcm = malloc(size);
                assert(pcm != NULL);
                memset(pcm, 0x83, size);
                memset(&state, 0, sizeof(state));
                put32(state.raw + 4, channels);
                put32(state.raw + 8, bits);
                put32(state.raw + 12, frames + 19);
                put16(state.raw + 32, 19);
                assert(lgsrcIo_readData(pcm, &state) == (int)(channels * frames));
                put32(state.raw + 20, channels);
                put32(state.raw + 24, bits);
                put32(state.raw + 28, frames);
                assert(returned_bits(lgsrcIo_writeData(pcm, &state)) ==
                       (uint32_t)(uintptr_t)pcm + size);
                free(pcm);
            }
        }
    }
}

int main(void)
{
    boundary_tests();
    exhaustive_tests();
    guard_tests();
    exact_buffer_tests();
    puts("lgsrc_io: all formats, channels 0..10, boundaries, exhaustive 16-bit values OK");
    return 0;
}
