#include "../../../src/dsp2/lgsrc_engine.c"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned char work[0x19370];
static unsigned coeff_calls, history_calls, lowpass_calls, sample_calls, zero_calls;
static uint32_t selected_table, expected_phase[10];
static int expected_down;

uint32_t lgsrc_engine_fixunsdfsi(double d)
{
    /* Mock only the finite, representable domain exercised by these tests. */
    assert(isfinite(d) && d >= 0.0 && d < 4294967296.0);
    return (uint32_t)d;
}
int32_t lgsrc_engine_fixdfsi(double d)
{
    assert(isfinite(d) && d >= INT32_MIN && d <= INT32_MAX);
    return (int32_t)d;
}
static unsigned char coefficient_rom[64];
const unsigned char *lgsrc_engine_rom(uint32_t address)
{
    ++coeff_calls;
    selected_table = address + 32;
    for (unsigned i = 0; i < sizeof(coefficient_rom); ++i)
        coefficient_rom[i] = (unsigned char)(i * 7 + (address >> 4));
    return coefficient_rom;
}
void lgsrcFast_init_history_FLIX(struct lgsrc_state *s)
{
    assert((void *)s == work);
    ++history_calls;
    /* Dispatch-only mock: no claim about the undecoded history value. */
}
int lgsrcFast_lowpass_filter(struct lgsrc_state *s)
{ assert((void *)s == work); ++lowpass_calls; return -77; }
int16_t lgsrcNormal_sample_FLIX(struct lgsrc_state *s, uint32_t channel,
                              uint32_t phase, int down)
{
    assert(down == expected_down);
    assert(phase == expected_phase[channel]);
    expected_phase[channel] += src_u32(s, 0x2c);
    ++sample_calls;
    /* Distinct output proves the equal-rate path used this mock kernel. */
    return (int16_t)(-2000 + (int32_t)channel * 100 + (int32_t)sample_calls);
}
uint32_t lgsrc_engine_zero_channel_register(struct lgsrc_state *s, int fast)
{ assert((void *)s == work); ++zero_calls; return fast ? 0x12345 : 0x67890; }

static struct lgsrc_state *setup(double factor, uint32_t channels, uint32_t n)
{
    memset(work, 0x5a, sizeof(work));
    struct lgsrc_state *s = (struct lgsrc_state *)(void *)work;
    src_s32(s, 0, 48000);
    src_s32(s, 4, channels);
    src_s32(s, 0x10, 48000);
    src_s32(s, 0x14, channels);
    src_s32(s, 0x1c, n);
    memcpy(work + 0x38, &factor, sizeof(factor));
    coeff_calls = history_calls = lowpass_calls = sample_calls = zero_calls = 0;
    return s;
}
static int16_t at(struct lgsrc_state *s, uint32_t c, unsigned i)
{ int16_t v; memcpy(&v, src_plane(s, c) + i * 2, 2); return v; }
static void put(struct lgsrc_state *s, uint32_t c, unsigned i, int16_t v)
{ memcpy(src_plane(s, c) + i * 2, &v, 2); }

static void test_fast_config(void)
{
    static const double factors[] = {0.26, 0.27, 0.48, 0.49, 0.53, 0.54,
                                    0.73, 0.74, 0.98, 0.99, 1.0, 2.0};
    static const uint32_t tables[] = {0x60103930, 0x60103970, 0x60103970,
        0x601039b0, 0x601039b0, 0x601039f0, 0x601039f0, 0x60103a30,
        0x60103a30, 0x601039b0, 0x601039b0, 0x601039b0};
    for (unsigned i = 0; i < sizeof(factors) / sizeof(factors[0]); ++i) {
        struct lgsrc_state *s = setup(factors[i], 2, 256);
        lgsrcFast_init(s);
        assert(coeff_calls == 1 && history_calls == 1 && selected_table == tables[i]);
        assert(memcmp(work + 0x19078, coefficient_rom, 32) == 0);
        assert(memcmp(work + 0x19054, coefficient_rom + 32, 32) == 0);
        assert(src_u32(s, 0x19050) == 0x5a5a5a5a);
        assert(src_u32(s, 0x19074) == 0x5a5a5a5a);
        assert(src_u32(s, 0x19098) == 0x5a5a5a5a);
        assert(src_u32(s, 0x2c) == (uint32_t)(32768.0 / factors[i] + 0.5));
        assert(src_u32(s, 0xc) == (uint32_t)(256.0 / factors[i] + 20.0));
        assert(src_u16(s, 0x20) == 10 && src_u16(s, 0x22) == 10);
        assert(src_u16(s, 0x26) == 10 && src_u32(s, 0x28) == 10 * 32768);
        for (unsigned c = 0; c < 2; ++c) {
            for (unsigned j = 0; j < 10; ++j) assert(at(s, c, j) == 0);
            assert(at(s, c, 10) == 0x5a5a);
        }
        put(s, 0, 0, 123);
        src_s16(s, 0x20, 7);
        src_s32(s, 0x28, 44);
        double f = 2.0;
        memcpy(work + 0x38, &f, sizeof(f));
        lgsrcFast_changeFs(s);
        assert(src_u32(s, 0x2c) == 16384 && src_u32(s, 0xc) == 148);
        assert(coeff_calls == 1 && at(s, 0, 0) == 123);
        assert(src_u16(s, 0x20) == 7 && src_u32(s, 0x28) == 44);
    }
}

static void test_finite_boundaries(void)
{
    static const double cuts[] = {0.27, 0.49, 0.54, 0.74, 0.99};
    static const uint32_t below[] = {0x60103930, 0x60103970, 0x601039b0,
                                   0x601039f0, 0x60103a30};
    static const uint32_t above[] = {0x60103970, 0x601039b0, 0x601039f0,
                                   0x60103a30, 0x601039b0};
    for (unsigned i = 0; i < 5; ++i) {
        for (int side = -1; side <= 1; ++side) {
            double factor = side == 0 ? cuts[i] :
                nextafter(cuts[i], side < 0 ? 0.0 : INFINITY);
            struct lgsrc_state *s = setup(factor, 0, 32);
            lgsrcFast_init(s);
            assert(selected_table == (side < 0 ? below[i] : above[i]));
            assert(coeff_calls == 1 && history_calls == 0);
        }
    }
    /* Exercise the actual double arithmetic either side of a Q15 half-step.
     * Conversion helpers remain finite-domain mocks, not recovered softfloat. */
    double center = 32768.0 / 16384.5;
    for (int side = -1; side <= 1; ++side) {
        double factor = side == 0 ? center :
            nextafter(center, side < 0 ? 0.0 : INFINITY);
        struct lgsrc_state *s = setup(factor, 1, 1);
        lgsrcFast_changeFs(s);
        double scaled = (1.0 / factor) * 32768.0;
        assert(src_u32(s, 0x2c) == (uint32_t)floor(scaled + 0.5));
        assert(src_u32(s, 0xc) == (uint32_t)floor(1.0 / factor + 20.0));
    }
}

static void test_copy_history(void)
{
    struct lgsrc_state *s = setup(1.0, 2, 5);
    src_s16(s, 0x20, 11); src_s16(s, 0x22, 10); src_s32(s, 0x28, 0x50001);
    for (unsigned c = 0; c < 2; ++c)
        for (unsigned j = 0; j < 25; ++j) put(s, c, j, (int16_t)(100 * c + j + 1));
    _lgsrcNormal_copyXBuffer(s, 10, 13);
    assert(src_u16(s, 0x20) == 14 && src_u16(s, 0x22) == 13);
    assert(src_u32(s, 0x28) == 0x68001);
    for (unsigned c = 0; c < 2; ++c) {
        for (unsigned j = 0; j < 3; ++j) assert(at(s, c, j) == 0);
        for (unsigned j = 0; j < 11; ++j) assert(at(s, c, j + 3) == (int)(100 * c + j + 1));
    }
    int16_t tail = at(s, 0, 3);
    _lgsrcNormal_copyXBuffer(s, 13, 10);
    assert(src_u16(s, 0x20) == 11 && src_u16(s, 0x22) == 10);
    assert(src_u32(s, 0x28) == 0x50001);
    for (unsigned j = 0; j < 3; ++j) assert(at(s, 0, j) == (int)(j + 1));
    assert(at(s, 0, 3) == tail); /* Shrink copies only three samples. */
    unsigned char before[sizeof(work)]; memcpy(before, work, sizeof(work));
    _lgsrcNormal_copyXBuffer(s, 10, 10);
    assert(memcmp(before, work, sizeof(work)) == 0);
}

static void test_normal_config(void)
{
    const double factors[] = {0.5, 1.0, 2.0, 48000.0 / 44100.0};
    const unsigned history[] = {52, 31, 31, 31};
    for (unsigned i = 0; i < 4; ++i) {
        struct lgsrc_state *s = setup(factors[i], 2, 32);
        lgsrcNormal_init(s);
        assert(src_u16(s, 0x26) == history[i]);
        assert(src_u16(s, 0x20) == history[i] && src_u16(s, 0x22) == history[i]);
        assert(src_u32(s, 0x28) == history[i] * 32768);
        assert(src_u32(s, 0xc) == (unsigned)(2 * history[i] + 32 / factors[i]));
        assert(src_u32(s, 0x30) == (factors[i] < 1 ? 16384u : 32768u));
        assert(src_u16(s, 0x19050) == (factors[i] < 1 ? 7854 : 15708));
        assert(src_u32(s, 0x19044) == 0x60168490);
        for (unsigned c = 0; c < 2; ++c)
            for (unsigned j = 0; j < history[i]; ++j) assert(at(s, c, j) == 0);
        double f = 0.5;
        memcpy(work + 0x38, &f, sizeof(f));
        lgsrcNormal_changeFs(s);
        assert(src_u16(s, 0x26) == 52 && src_u32(s, 0xc) == 168);
        assert(src_u16(s, 0x20) == 52 && src_u16(s, 0x22) == 52);
    }
}

static void test_fast_process(void)
{
    for (unsigned channels = 1; channels <= 10; ++channels) {
        struct lgsrc_state *s = setup(1.0, channels, 17);
        lgsrcFast_init(s);
        for (unsigned c = 0; c < channels; ++c)
            for (unsigned j = 0; j < 37; ++j)
                put(s, c, j, (int16_t)(-30000 + (int)c * 1000 + (int)j * 100));
        assert(lgsrcFast_process(s) == 0 && lowpass_calls == 0);
        assert(src_u16(s, 0x24) == 17 && src_u16(s, 0x20) == 20);
        assert(src_u16(s, 0x22) == 10 && src_u32(s, 0x34) == 17);
        assert(src_u32(s, 0x28) == 10 * 32768);
        for (unsigned c = 0; c < channels; ++c) {
            for (unsigned j = 0; j < 17; ++j)
                assert(at(s, c, 0x1200 + j) == -29000 + (int)c * 1000 + (int)j * 100);
            for (unsigned j = 0; j < 20; ++j)
                assert(at(s, c, j) == -28300 + (int)c * 1000 + (int)j * 100);
        }
    }
    struct lgsrc_state *s = setup(2.0, 1, 3);
    lgsrcFast_init(s);
    put(s, 0, 10, -32768); put(s, 0, 11, 32767); put(s, 0, 12, -123);
    assert(lgsrcFast_process(s) == 0);
    assert(at(s, 0, 0x1200) == -32768);
    assert(at(s, 0, 0x1201) == 0);
    assert(at(s, 0, 0x1202) == 32767);
    assert(src_u32(s, 0x28) == 10 * 32768 + 16384);
    assert(src_u32(s, 0x34) == 1);
    src_s32(s, 0, 49000); assert(lgsrcFast_process(s) == 0 && lowpass_calls == 0);
    src_s32(s, 0, 49001); assert(lgsrcFast_process(s) == 0 && lowpass_calls == 1);
}

static void test_normal_process(void)
{
    const double factors[] = {1.0, 0.5, 2.0};
    for (unsigned k = 0; k < 3; ++k) {
        struct lgsrc_state *s = setup(factors[k], 2, 5);
        lgsrcNormal_init(s);
        uint32_t history = src_u16(s, 0x26);
        uint32_t start = src_u32(s, 0x28);
        uint32_t consumed = (5 * src_u32(s, 0x2c)) >> 15;
        for (unsigned c = 0; c < 2; ++c) {
            expected_phase[c] = start;
            for (unsigned j = 0; j < src_u32(s, 0xc); ++j) put(s, c, j, (int16_t)j);
        }
        expected_down = factors[k] < 1.0;
        assert(lgsrcNormal_process(s) == 0);
        assert(sample_calls == 10 && src_u16(s, 0x24) == 5);
        assert(src_u32(s, 0x34) == consumed);
        assert(src_u32(s, 0x28) == start + 5 * src_u32(s, 0x2c) - consumed * 32768);
        assert(src_u16(s, 0x22) == history);
        for (unsigned c = 0; c < 2; ++c) {
            assert(at(s, c, 0) == (int)consumed);
            for (unsigned j = 0; j < 5; ++j)
                assert(at(s, c, 0x1200 + j) == -1999 + (int)c * 105 + (int)j);
        }
    }
}

static void test_guards_and_empty(void)
{
    struct lgsrc_state *s = setup(1.0, 11, 1);
    unsigned char before[sizeof(work)]; memcpy(before, work, sizeof(work));
    assert(lgsrcFast_process(s) == -1 && lgsrcNormal_process(s) == -1);
    assert(memcmp(before, work, sizeof(work)) == 0);
    s = setup(1.0, 1, 0); lgsrcFast_init(s);
    assert(lgsrcFast_process(s) == 0 && src_u16(s, 0x24) == 0);
    s = setup(1.0, 1, 0); lgsrcNormal_init(s);
    assert(lgsrcNormal_process(s) == 0 && sample_calls == 0 && src_u16(s, 0x24) == 0);
    s = setup(1.0, 0, 5);
    src_s32(s, 0x34, 2); src_s16(s, 0x24, 6); src_s16(s, 0x26, 10);
    assert(lgsrcFast_process(s) == -1);
    assert(src_u32(s, 0x28) == 0x12345 - 2 * 32768);
    assert(src_u16(s, 0x20) == 0 && src_u16(s, 0x22) == 10);
    assert(lgsrcNormal_process(s) == -1);
    assert(src_u32(s, 0x28) == 0x67890 - 2 * 32768 && zero_calls == 2);
}

int main(void)
{
    test_fast_config(); test_finite_boundaries(); test_copy_history(); test_normal_config();
    test_fast_process(); test_normal_process(); test_guards_and_empty();
    puts("lgsrc_engine: configuration, history, interpolation, FIR dispatch, guards passed");
    return 0;
}
