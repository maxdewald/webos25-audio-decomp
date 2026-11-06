/* Standalone wrapper-only test: cc -std=c11 -Wall -Wextra -Werror
 * -fsanitize=address,undefined tools/tests/dsp2/lgsrc.c -o build/scratch/lgsrc-test
 * && build/scratch/lgsrc-test
 */
#include "../../../src/dsp2/lgsrc.c"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned allocations, releases, tables, fast_init, normal_init;
static unsigned fast_change, normal_change, trace, engine;
static struct lgsrc_state *expected;
static const void *expected_input;
static void *expected_output;
static void *allocated[2];

void *aud_malloc(unsigned bytes)
{
    assert(allocations < 2);
    assert(bytes == (allocations == 0 ? 0x19370 : sizeof(struct lgsrc)));
    void *p = malloc(bytes);
    assert(p);
    memset(p, 0xa5, bytes);
    allocated[allocations++] = p;
    return p;
}
void aud_free(void *p)
{
    assert(releases < 2 && p == allocated[releases]);
    ++releases;
    free(p);
}
void AuiLogger_Init(void *p, int fill, unsigned bytes)
{
    assert(p == allocated[0] && fill == 0 && bytes == 0x19370);
    memset(p, fill, bytes);
}
void lgsrcSinctable_makeTable(void) { ++tables; }
void lgsrcFast_init(struct lgsrc_state *s) { assert(s == expected); ++fast_init; }
void lgsrcNormal_init(struct lgsrc_state *s) { assert(s == expected); ++normal_init; }
void lgsrcFast_changeFs(struct lgsrc_state *s) { assert(s == expected); ++fast_change; }
void lgsrcNormal_changeFs(struct lgsrc_state *s) { assert(s == expected); ++normal_change; }
int lgsrcIo_readData(const void *p, struct lgsrc_state *s)
{
    assert(s == expected && p == expected_input && trace == 0);
    trace = 1;
    return -1;
}
int lgsrcFast_process(struct lgsrc_state *s)
{
    assert(s == expected && trace == 1 && s->run_mode == 0);
    trace = 2;
    engine = 1;
    s->produced_samples = 123;
    return -1;
}
int lgsrcNormal_process(struct lgsrc_state *s)
{
    assert(s == expected && trace == 1 && s->run_mode != 0);
    trace = 2;
    engine = 2;
    s->produced_samples = 124;
    return -1;
}
int lgsrcIo_writeData(void *p, struct lgsrc_state *s)
{
    assert(s == expected && p == expected_output && trace == 2);
    trace = 3;
    return -1;
}

static void test_formats(struct lgsrc *h)
{
    for (uint32_t bits = 0; bits <= 32; ++bits) {
        assert(lgsrc_setInOutPCMFormat(h, bits) == 0);
        assert(lgsrc_getInPCMFormat(h) == bits && lgsrc_getOutPCMFormat(h) == bits);
    }
    assert(lgsrc_setInOutPCMFormat(h, 33) == -1);
    assert(lgsrc_setInOutPCMFormat(h, UINT32_MAX) == -1);
    assert(lgsrc_setInPCMFormat(h, 7) == 0);
    assert(lgsrc_getInPCMFormat(h) == 7 && lgsrc_getOutPCMFormat(h) == 32);
    assert(lgsrc_setOutPCMFormat(h, 24) == 0);
    assert(lgsrc_setInPCMFormat(h, 33) == -1);
    assert(lgsrc_setOutPCMFormat(h, UINT32_MAX) == -1);
    assert(lgsrc_getInPCMFormat(h) == 7 && lgsrc_getOutPCMFormat(h) == 24);
    for (uint32_t channels = 0; channels <= 10; ++channels) {
        assert(lgsrc_setInOutChannel(h, channels) == 0);
        assert(lgsrc_getInChannel(h) == channels && lgsrc_getOutChannel(h) == channels);
    }
    assert(lgsrc_setInOutChannel(h, 11) == -1);
    assert(lgsrc_setInOutChannel(h, UINT32_MAX) == -1);
    assert(lgsrc_getInChannel(h) == 10 && lgsrc_getOutChannel(h) == 10);
    assert(lgsrc_setOutSample(h, 512) == 0);
    assert(lgsrc_setOutSample(h, 513) == 1);
    assert(lgsrc_setOutSample(h, UINT32_MAX) == 1);
    assert(lgsrc_getOutSampleNum(h) == 512);
    assert(lgsrc_setOutSample(h, 0) == 0);
    assert(lgsrc_setFilterType(h, UINT32_MAX) == 0 && expected->filter_type == UINT32_MAX);
    assert(fast_init == 0 && normal_init == 0 && fast_change == 0 && normal_change == 0);
}

static void test_rates(struct lgsrc *h)
{
    struct lgsrc_state *s = h->state;
    s->input_fs = s->output_fs = 48000;
    assert(lgsrc_init(h) == 0 && fast_init == 1 && lgsrc_getFactor(h) == 1.0);
    assert(lgsrc_changeFs(h, 48000, 48000) == 0 && fast_change == 0);
    assert(lgsrc_setInputFs(h, 48000) == 0 && fast_change == 0);
    assert(lgsrc_setOutputFs(h, 48000) == 0 && fast_change == 0);
    assert(lgsrc_changeFs(h, 12345, 384000) == 0 && fast_change == 1);
    assert(lgsrc_getFactor(h) == 384000.0 / 12345.0);
    assert(lgsrc_getInputFs(h) == 12345 && lgsrc_getOutputFs(h) == 384000);
    assert(lgsrc_changeFs(h, 384001, 1) == -1);
    assert(lgsrc_changeFs(h, 1, UINT32_MAX) == -1);
    assert(lgsrc_setInputFs(h, UINT32_MAX) == -1);
    assert(lgsrc_setOutputFs(h, 384001) == -1);
    assert(lgsrc_getInputFs(h) == 12345 && lgsrc_getOutputFs(h) == 384000);
    assert(lgsrc_setInputFs(h, 384000) == 0 && fast_change == 2);
    assert(lgsrc_setOutputFs(h, 123) == 0 && fast_change == 3);
    assert(lgsrc_getFactor(h) == 123.0 / 384000.0);
    s->output_samples = 513;
    assert(lgsrc_setInputFs(h, 123) == 0 && fast_change == 3);
    assert(s->input_fs == 123 && s->factor == 1.0);
    assert(lgsrc_init(h) == -1 && fast_init == 1);
    assert(lgsrc_changeFs(h, 246, 123) == 0 && fast_change == 3 && s->factor == 0.5);
    s->input_samples = 4609;
    s->output_samples = 512;
    assert(lgsrc_changeFs(h, 123, 123) == -1 && fast_change == 4);
    assert(s->input_fs == 123 && s->factor == 1.0);
    s->input_samples = UINT32_MAX;
    assert(lgsrc_changeFs(h, 123, 123) == -1 && fast_change == 4);
    s->input_samples = 4608;
    assert(lgsrc_changeFs(h, 123, 123) == 0);
    s->output_fs = 384001;
    s->factor = 17.0;
    assert(lgsrc_setInputFs(h, 456) == 0 && s->input_fs == 456);
    assert(s->factor == 17.0 && fast_change == 4);
    assert(_lgsrc_ChangeState(h) == -1 && lgsrc_init(h) == -1);
    s->input_fs = 384001;
    assert(lgsrc_setOutputFs(h, 456) == 0 && s->output_fs == 456);
    assert(s->factor == 17.0 && fast_change == 4);
    s->input_fs = s->output_fs = 48000;
    s->input_samples = 256;
    assert(lgsrc_setRunMode(h, -7) == 0 && normal_init == 1);
    assert(lgsrc_getRunMode(h) == -7);
    assert(lgsrc_setRunMode(h, -7) == 0 && normal_init == 1);
    assert(lgsrc_changeFs(h, 48000, 96000) == 0 && normal_change == 1);
    s->output_samples = 513;
    assert(lgsrc_setRunMode(h, 0) == 0 && fast_init == 1 && lgsrc_getRunMode(h) == 0);
    assert(lgsrc_setRunMode(h, 99) == 0 && normal_init == 1 && lgsrc_getRunMode(h) == 99);
    s->output_samples = 256;
    assert(lgsrc_init(h) == 0 && normal_init == 2);
    assert(lgsrc_setRunMode(h, 0) == 0 && fast_init == 2);
    assert(lgsrc_changeFs(h, 0, 1) == 0 && isinf(s->factor));
    assert(lgsrc_changeFs(h, 0, 0) == 0 && isnan(s->factor));
    assert(lgsrc_changeFs(h, 1, 0) == 0 && s->factor == 0.0);
}

static void test_process(struct lgsrc *h)
{
    struct lgsrc_state *s = h->state;
    char input, output;
    expected_input = &input;
    expected_output = &output;
    s->input_fs = s->output_fs = 48000;
    s->input_samples = 4608;
    s->output_samples = 512;
    s->output_channels = 10;
    s->run_mode = 0;
    trace = 0;
    assert(lgsrc_process(h, &input, &output) == 1230 && trace == 3 && engine == 1);
    assert(lgsrc_getOutSampleNum(h) == 512);
    s->run_mode = -1;
    trace = 0;
    assert(lgsrc_process(h, &input, &output) == 1240 && trace == 3 && engine == 2);
    s->output_samples = 513;
    trace = 0;
    assert(lgsrc_process(h, &input, &output) == 0 && trace == 0);
    s->output_samples = 512;
    s->input_samples = 4609;
    assert(lgsrc_process(h, &input, &output) == 0 && trace == 0);
    s->output_samples = s->input_samples = UINT32_MAX;
    assert(lgsrc_process(h, &input, &output) == 1240 && trace == 3);
    s->output_samples = s->input_samples = 0;
    s->output_channels = 0;
    trace = 0;
    assert(lgsrc_process(h, &input, &output) == 0 && trace == 3);
}

static void test_getters(struct lgsrc *h)
{
    struct lgsrc_state *s = h->state;
    s->input_channels = 8;
    s->input_bits = 24;
    s->input_samples = 256;
    s->buffered_samples = 17;
    assert(lgsrc_getNeedfulSamplePer1Ch(h) == 239);
    assert(lgsrc_getNeedfulSample(h) == 1912);
    assert(lgsrc_getNeedfulByte(h) == 5736);
    s->input_bits = 7;
    assert(lgsrc_getNeedfulByte(h) == 0);
    s->input_bits = 15;
    assert(lgsrc_getNeedfulByte(h) == 1912);
    s->input_samples = 16;
    assert(lgsrc_getNeedfulSamplePer1Ch(h) == UINT32_MAX);
    assert(lgsrc_getNeedfulSample(h) == UINT32_MAX - 7);
    assert(lgsrc_getNeedfulByte(h) == UINT32_MAX - 7);
    assert(lgsrc_getVersion() == 1 && lgsrc_getDebugParam(h) == s);
}

int main(void)
{
    struct lgsrc *h = lgsrc_create();
    assert(allocations == 2 && tables == 1);
    expected = h->state;
    const unsigned char *bytes = (const unsigned char *)expected;
    for (size_t i = 0; i < sizeof(*expected); ++i)
        assert(bytes[i] == 0);
    test_formats(h);
    test_rates(h);
    test_process(h);
    test_getters(h);
    lgsrc_Destroy(h);
    assert(releases == 2);
    puts("lgsrc wrapper tests passed");
    return 0;
}
