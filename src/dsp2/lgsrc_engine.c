/* DSP2 resampler control, audio_a0_dsp2.main_60000000.bin.
 * State layout matches lgsrc.c; accessors preserve its target byte offsets.
 * [FLIX] Lowpass history initialization and filter kernels remain external,
 * not identity filters. Normal mode filters even at equal input/output rates.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct lgsrc_state;

static uint16_t src_u16(const struct lgsrc_state *s, size_t off)
{ uint16_t v; memcpy(&v, (const unsigned char *)s + off, sizeof(v)); return v; }
static uint32_t src_u32(const struct lgsrc_state *s, size_t off)
{ uint32_t v; memcpy(&v, (const unsigned char *)s + off, sizeof(v)); return v; }
static double src_factor(const struct lgsrc_state *s)
{ double v; memcpy(&v, (const unsigned char *)s + 0x38, sizeof(v)); return v; }
static void src_s16(struct lgsrc_state *s, size_t off, uint16_t v)
{ memcpy((unsigned char *)s + off, &v, sizeof(v)); }
static void src_s32(struct lgsrc_state *s, size_t off, uint32_t v)
{ memcpy((unsigned char *)s + off, &v, sizeof(v)); }
static unsigned char *src_plane(struct lgsrc_state *s, uint32_t ch)
{ return (unsigned char *)s + 0x40 + (size_t)ch * 0x2800; }

/* Target software conversion helpers: keep out-of-range/NaN semantics external
 * instead of introducing undefined host float-to-int conversions (zero rates
 * are accepted by the wrapper). These are not reconstructed functions. */
extern uint32_t lgsrc_engine_fixunsdfsi(double);
extern int32_t lgsrc_engine_fixdfsi(double);
/* Resolve a DSP ROM address without embedding the proprietary tables. */
extern const unsigned char *lgsrc_engine_rom(uint32_t address);
/* [FLIX] 0x6009e581 initializes the value stored to lowpass histories at
 * 0x1909c/0x19204 (eight words each, channel stride 36). Its non-branch
 * bundle is not decoded reliably; do not assume it means floating zero. */
extern void lgsrcFast_init_history_FLIX(struct lgsrc_state *);
extern int lgsrcFast_lowpass_filter(struct lgsrc_state *);
/* [FLIX] Per-output-sample FIR results, including rounding and saturation.
 * phase is Q15; the hook must evaluate the real kernel, not interpolate. */
extern int16_t lgsrcNormal_sample_FLIX(struct lgsrc_state *, uint32_t channel,
                                     uint32_t phase, int downsample);
extern int _lgsrcNormal_downFiltering_Hifi(void);
extern int _lgsrcNormal_upFiltering_Hifi(void);
/* Zero-output-channel firmware paths use a live, uninitialized incoming
 * register (a13 fast, a15 normal). Do not fabricate its value on a host. */
extern uint32_t lgsrc_engine_zero_channel_register(struct lgsrc_state *, int fast);

/* @0x6009e64c: changes only Q15 rate step and required input count. */
void lgsrcFast_changeFs(struct lgsrc_state *s)
{
    double inverse = 1.0 / src_factor(s);
    src_s32(s, 0x2c, lgsrc_engine_fixunsdfsi(inverse * 32768.0 + 0.5));
    src_s32(s, 0x0c, (uint32_t)lgsrc_engine_fixdfsi(
                (double)src_u32(s, 0x1c) * inverse + 20.0));
}

/* @0x6009e4a8: ten-sample startup history; coefficient choice is independent
 * of changeFs (which does not reinstall coefficients or clear histories). */
void lgsrcFast_init(struct lgsrc_state *s)
{
    lgsrcFast_changeFs(s);
    src_s16(s, 0x26, 10);
    src_s16(s, 0x22, 10);
    src_s16(s, 0x20, 10);
    src_s32(s, 0x28, 0x50000);
    for (uint32_t c = 0; c < src_u32(s, 4); ++c)
        memset(src_plane(s, c), 0, 20);
    double factor = src_factor(s);
    uint32_t table;
    if (factor < 0.27) table = 0x60103930;
    else if (factor < 0.49) table = 0x60103970;
    else if (factor < 0.54) table = 0x601039b0;
    else if (factor < 0.74) table = 0x601039f0;
    else if (factor < 0.99) table = 0x60103a30;
    else table = 0x601039b0;
    /* 0x6009e560..6009e56e: AE_L32.IP is postincrement; the parallel
     * store uses a9 before its +4 update. Keep the gap at 0x19074 intact. */
    const unsigned char *rom = lgsrc_engine_rom(table - 32);
    for (unsigned i = 0; i < 8; ++i) {
        memcpy((unsigned char *)s + 0x19078 + i * 4, rom + i * 4, 4);
        memcpy((unsigned char *)s + 0x19054 + i * 4, rom + 32 + i * 4, 4);
    }
    if (src_u32(s, 4) != 0)
        lgsrcFast_init_history_FLIX(s);
}

/* @0x6009ed34: history grows backwards and zero-fills its new prefix.
 * The shrinking branch really copies only old-new samples, not buffered-new;
 * this unusual loop bound is explicit at 0x6009ee26..6009ee8e. */
void _lgsrcNormal_copyXBuffer(struct lgsrc_state *s, uint32_t old,
                            uint32_t next)
{
    int32_t delta = (int32_t)(next - old);
    for (uint32_t c = 0; c < src_u32(s, 0x14); ++c) {
        unsigned char *p = src_plane(s, c);
        if (delta > 0) {
            memmove(p + (size_t)delta * 2, p, (size_t)src_u16(s, 0x20) * 2);
            memset(p, 0, (size_t)delta * 2);
        } else if (delta < 0) {
            size_t count = (size_t)(-(int64_t)delta);
            memmove(p, p + count * 2, count * 2);
        }
    }
    src_s16(s, 0x20, (uint16_t)(src_u16(s, 0x20) + (uint32_t)delta));
    src_s16(s, 0x22, (uint16_t)(src_u16(s, 0x22) + (uint32_t)delta));
    src_s32(s, 0x28, src_u32(s, 0x28) + ((uint32_t)delta << 15));
}

/* @0x6009ee94: Q15 step, anti-alias scale, variable history, required input,
 * and Q14 kernel gain. Literal doubles: 32768, .5, 21, and 15708. */
void _lgsrcNormal_initParam(struct lgsrc_state *s)
{
    double factor = src_factor(s);
    double inverse = 1.0 / factor;
    src_s32(s, 0x2c, lgsrc_engine_fixunsdfsi(inverse * 32768.0 + 0.5));
    src_s32(s, 0x30, lgsrc_engine_fixunsdfsi(
                (factor < 1.0 ? factor : 1.0) * 32768.0 + 0.5));
    uint32_t history = inverse < 1.0 ? 31 :
        lgsrc_engine_fixunsdfsi(inverse * 21.0) + 10;
    uint16_t old = src_u16(s, 0x26);
    if (old != (uint16_t)history) {
        _lgsrcNormal_copyXBuffer(s, old, (uint16_t)history);
        src_s16(s, 0x26, (uint16_t)history);
        inverse = 1.0 / src_factor(s);
    }
    src_s32(s, 0x0c, (uint32_t)lgsrc_engine_fixdfsi(
                2.0 * src_u16(s, 0x26) + (double)src_u32(s, 0x1c) * inverse));
    src_s16(s, 0x19050, 0x3d5c);
    if (factor < 1.0)
        src_s16(s, 0x19050, (uint16_t)lgsrc_engine_fixunsdfsi(factor * 15708.0 + 0.5));
}

/* @0x6009efd0: initial input history is zero, and the shared sinc-table
 * address is a 32-bit DSP address, not a native host pointer. */
void lgsrcNormal_init(struct lgsrc_state *s)
{
    src_s16(s, 0x26, 0);
    src_s16(s, 0x20, 0);
    src_s32(s, 0x28, 0);
    _lgsrcNormal_initParam(s);
    uint16_t history = src_u16(s, 0x26);
    src_s32(s, 0x19044, 0x60168490);
    src_s16(s, 0x22, history);
    src_s16(s, 0x20, history);
    src_s32(s, 0x28, (uint32_t)history << 15);
    for (uint32_t c = 0; c < src_u32(s, 4); ++c)
        memset(src_plane(s, c), 0, (size_t)history * 2);
}

/* @0x6009f030 */
void lgsrcNormal_changeFs(struct lgsrc_state *s)
{ _lgsrcNormal_initParam(s); }

/* @0x6009eb1c: both branches produce exactly the requested count. Equal rate
 * uses the up-filter kernel, not a bypass. Return is the advanced Q15 phase;
 * state phase itself is not updated until all channels finish in process. */
uint32_t _lgsrcNormal_resample(struct lgsrc_state *s, uint32_t channel)
{
    uint32_t start = src_u32(s, 0x28);
    uint32_t phase = start;
    uint32_t count = src_u32(s, 0x1c);
    uint32_t step = src_u32(s, 0x2c);
    int down = !(src_factor(s) >= 1.0);
    unsigned char *output = src_plane(s, channel) + 0x2400;
    for (uint32_t i = 0; i < count; ++i) {
        int16_t sample = lgsrcNormal_sample_FLIX(s, channel, phase, down);
        memcpy(output + (size_t)i * 2, &sample, sizeof(sample));
        phase += step;
    }
    src_s16(s, 0x24, (uint16_t)count);
    src_s32(s, 0x34, (phase >> 15) - (start >> 15));
    return phase;
}

static int src_finish(struct lgsrc_state *s, uint32_t phase)
{
    uint16_t history = src_u16(s, 0x26);
    uint16_t used = (uint16_t)(src_u16(s, 0x22) + src_u32(s, 0x34));
    uint32_t left = (uint32_t)history + src_u32(s, 0x0c) - used;
    src_s32(s, 0x28, phase - (src_u32(s, 0x34) << 15));
    for (uint32_t c = 0; c < src_u32(s, 0x14); ++c) {
        unsigned char *p = src_plane(s, c);
        if (left != 0)
            memmove(p, p + ((ptrdiff_t)used - history) * 2, (size_t)left * 2);
    }
    src_s16(s, 0x20, src_u32(s, 0x14) == 0 ? 0 : (uint16_t)left);
    src_s16(s, 0x22, history);
    return src_u32(s, 0x1c) < src_u16(s, 0x24) ? -1 : 0;
}

/* @0x6009f03c: output channels >10 fail without modifying state. */
int lgsrcNormal_process(struct lgsrc_state *s)
{
    uint32_t channels = src_u32(s, 0x14);
    if (channels > 10) return -1;
    uint32_t phase;
    if (channels == 0)
        phase = lgsrc_engine_zero_channel_register(s, 0);
    else {
        phase = 0;
        for (uint32_t c = 0; c < channels; ++c)
            phase = _lgsrcNormal_resample(s, c);
    }
    return src_finish(s, phase);
}

static int32_t src_signed32(uint32_t v)
{ return v <= INT32_MAX ? (int32_t)v : -1 - (int32_t)(UINT32_MAX - v); }
static int16_t src_fast_sample(int16_t a, int16_t b, uint32_t phase)
{
    uint32_t fraction = phase & 0x7fff;
    /* mul16s sign-extends the fraction, which is in 0..32767. Arithmetic
     * and +0x4000 rounding wrap at 32 bits before the target's signed shift. */
    uint32_t sum = (uint32_t)((int32_t)a * (int32_t)(32768 - fraction)) +
                   (uint32_t)((int32_t)b * (int32_t)fraction) + 0x4000;
    int32_t signed_sum = src_signed32(sum);
    int32_t rounded = signed_sum >= 0 ? signed_sum / 32768 :
        -1 - (int32_t)((-(int64_t)signed_sum - 1) / 32768);
    return (int16_t)rounded;
}

/* @0x6009e83c: linear Q15 interpolation is entirely scalar. The optional
 * lowpass runs only when input_fs > output_fs+1000 (unsigned). Fast changeFs
 * intentionally does not reset its ten-sample history. */
int lgsrcFast_process(struct lgsrc_state *s)
{
    uint32_t channels = src_u32(s, 0x14);
    if (channels > 10) return -1;
    if (src_u32(s, 0) > src_u32(s, 0x10) + 1000)
        (void)lgsrcFast_lowpass_filter(s);
    uint32_t phase;
    if (channels == 0)
        phase = lgsrc_engine_zero_channel_register(s, 1);
    else {
        uint32_t start = src_u32(s, 0x28);
        uint32_t step = src_u32(s, 0x2c);
        uint32_t count = src_u32(s, 0x1c);
        phase = start;
        for (uint32_t c = 0; c < channels; ++c) {
            unsigned char *p = src_plane(s, c);
            phase = start;
            for (uint32_t i = 0; i < count; ++i) {
                int16_t a, b;
                memcpy(&a, p + (size_t)(phase >> 15) * 2, sizeof(a));
                memcpy(&b, p + (size_t)(phase >> 15) * 2 + 2, sizeof(b));
                int16_t value = src_fast_sample(a, b, phase);
                memcpy(p + 0x2400 + (size_t)i * 2, &value, sizeof(value));
                phase += step;
            }
            src_s16(s, 0x24, (uint16_t)count);
            src_s32(s, 0x34, (phase >> 15) - (start >> 15));
        }
    }
    return src_finish(s, phase);
}
