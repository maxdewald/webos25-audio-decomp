/* DSP2 lgsrc wrapper, audio_a0_dsp2.main_60000000.bin, linked at 0x60000000.
 * The handle contains one pointer; the work area is zeroed, not configured, by
 * create. Fields/padding retain target offsets on the host for mock testing.
 */
#include <stddef.h>
#include <stdint.h>

struct lgsrc_state {
    uint32_t input_fs;                 /* +0x00 */
    uint32_t input_channels;           /* +0x04 */
    uint32_t input_bits;               /* +0x08 */
    uint32_t input_samples;            /* +0x0c total required per channel */
    uint32_t output_fs;                /* +0x10 */
    uint32_t output_channels;          /* +0x14 */
    uint32_t output_bits;              /* +0x18 */
    uint32_t output_samples;           /* +0x1c requested per channel */
    uint16_t buffered_samples;         /* +0x20 */
    uint16_t unknown_22;
    uint16_t produced_samples;         /* +0x24 actual per channel */
    uint16_t unknown_26;
    uint32_t unknown_28[4];
    double factor;                    /* +0x38 output_fs / input_fs */
    unsigned char filter_work[0x19040 - 0x40];
    int32_t run_mode;                  /* +0x19040 zero fast, all others normal */
    uint32_t unknown_19044[2];
    uint32_t filter_type;              /* +0x1904c */
    unsigned char tail[0x19370 - 0x19050];
};
struct lgsrc { struct lgsrc_state *state; };

_Static_assert(offsetof(struct lgsrc_state, factor) == 0x38, "factor offset");
_Static_assert(offsetof(struct lgsrc_state, run_mode) == 0x19040, "mode offset");
_Static_assert(offsetof(struct lgsrc_state, filter_type) == 0x1904c, "filter offset");
_Static_assert(sizeof(struct lgsrc_state) == 0x19370, "work-area size");

extern void *aud_malloc(unsigned);      /* @0x600e97b4 allocator alias */
extern void aud_free(void *);           /* @0x600e9550 deallocator alias */
extern void AuiLogger_Init(void *, int, unsigned); /* @0x600fb998 memset alias */
extern void lgsrcSinctable_makeTable(void); /* @0x6009f650 shared table */
/* Filter controls are reconstructed in lgsrc_engine.c; remaining [FLIX]
 * kernels stay external, never identity/no-op substitutes. */
extern void lgsrcFast_init(struct lgsrc_state *);       /* @0x6009e4a8 */
extern void lgsrcFast_changeFs(struct lgsrc_state *);   /* @0x6009e64c */
extern int lgsrcFast_process(struct lgsrc_state *);     /* @0x6009e83c */
extern void lgsrcNormal_init(struct lgsrc_state *);     /* @0x6009efd0 */
extern void lgsrcNormal_changeFs(struct lgsrc_state *); /* @0x6009f030 */
extern int lgsrcNormal_process(struct lgsrc_state *);   /* @0x6009f03c */
extern int lgsrcIo_readData(const void *, struct lgsrc_state *); /* @0x6009f120 */
extern int lgsrcIo_writeData(void *, struct lgsrc_state *);      /* @0x6009f360 */

/* @0x600a0b18: only unsigned rate<=384000 and requested samples<=512 are
 * checked. Zero rates are accepted: division is the firmware's double division,
 * not a guarded or clamped ratio. Factor changes even if sample validation fails.
 */
int lgsrc_init(struct lgsrc *h)
{
    struct lgsrc_state *s = h->state;
    if (s->input_fs > 384000 || s->output_fs > 384000)
        return -1;
    s->factor = (double)s->output_fs / (double)s->input_fs;
    if (s->output_samples > 512)
        return -1;
    if (s->run_mode == 0)
        lgsrcFast_init(s);
    else
        lgsrcNormal_init(s);
    return 0;
}

int _lgsrc_ChangeState(struct lgsrc *h);

/* @0x600a0b78: new rates are committed before changing state; a state-change
 * failure is discarded. The final required-input bound is unsigned <=4608.
 */
int lgsrc_changeFs(struct lgsrc *h, uint32_t input_fs, uint32_t output_fs)
{
    struct lgsrc_state *s = h->state;
    if (input_fs > 384000 || output_fs > 384000)
        return -1;
    if (s->input_fs != input_fs || s->output_fs != output_fs) {
        s->input_fs = input_fs;
        s->output_fs = output_fs;
        (void)_lgsrc_ChangeState(h);
    }
    return s->input_samples > 4608 ? -1 : 0;
}

/* @0x600a0bac: same checks/order as init, but invokes changeFs, not init. */
int _lgsrc_ChangeState(struct lgsrc *h)
{
    struct lgsrc_state *s = h->state;
    if (s->output_fs > 384000 || s->input_fs > 384000)
        return -1;
    s->factor = (double)s->output_fs / (double)s->input_fs;
    if (s->output_samples > 512)
        return -1;
    if (s->run_mode == 0)
        lgsrcFast_changeFs(s);
    else
        lgsrcNormal_changeFs(s);
    return 0;
}

/* @0x600a0c08: NO equal-rate bypass. Even input_fs==output_fs runs read,
 * selected filter, write. Pcmcvt must bypass this call itself. The two upper
 * bounds here are SIGNED (unlike init/changeFs); return is actual produced
 * samples * output channels, not bytes or the requested output_samples.
 * All three callees' return values (including invalid I/O format) are ignored.
 */
uint32_t lgsrc_process(struct lgsrc *h, const void *input, void *output)
{
    struct lgsrc_state *s = h->state;
    if ((int32_t)s->output_samples > 512 || (int32_t)s->input_samples > 4608)
        return 0;
    lgsrcIo_readData(input, s);
    if (s->run_mode == 0)
        lgsrcFast_process(s);
    else
        lgsrcNormal_process(s);
    lgsrcIo_writeData(output, s);
    return (uint32_t)s->produced_samples * s->output_channels;
}

/* @0x600a0c60: no allocation-failure checks, defaults or init call. On the DSP
 * sizeof(*h)==4; use native pointer size on a host. The work area is 0x19370.
 */
struct lgsrc *lgsrc_create(void)
{
    struct lgsrc_state *s = aud_malloc(sizeof(*s));
    AuiLogger_Init(s, 0, sizeof(*s));
    struct lgsrc *h = aud_malloc(sizeof(*h));
    h->state = s;
    lgsrcSinctable_makeTable();
    return h;
}

/* @0x600a0c88 */
void lgsrc_Destroy(struct lgsrc *h)
{
    aud_free(h->state);
    aud_free(h);
}

/* @0x600a0ca0: any unsigned 0..384000 is accepted, not a sample-rate whitelist.
 * Changed rates trigger changeFs; unchanged ones do nothing. A counterpart
 * rate/sample validation failure is ignored after storing the new rate.
 */
int lgsrc_setInputFs(struct lgsrc *h, uint32_t fs)
{
    if (fs > 384000)
        return -1;
    if (h->state->input_fs != fs) {
        h->state->input_fs = fs;
        (void)_lgsrc_ChangeState(h);
    }
    return 0;
}

/* @0x600a0d04: same behavior as input setter, for output rate. */
int lgsrc_setOutputFs(struct lgsrc *h, uint32_t fs)
{
    if (fs > 384000)
        return -1;
    if (h->state->output_fs != fs) {
        h->state->output_fs = fs;
        (void)_lgsrc_ChangeState(h);
    }
    return 0;
}

/* @0x600a0d6c: 0..32 inclusive, not just 16/24/32. Stores both formats without
 * reinitialization or state change. I/O entry dispatches @0x6009f130..14f and
 * @0x6009f36f..391 implement only 8/16/24/32; other values return -1, ignored
 * by process. Scalar packing conversions are reconstructed in lgsrc_io.c.
 */
int lgsrc_setInOutPCMFormat(struct lgsrc *h, uint32_t bits)
{
    if (bits > 32)
        return -1;
    h->state->input_bits = bits;
    h->state->output_bits = bits;
    return 0;
}

/* @0x600a0d84 */
int lgsrc_setInPCMFormat(struct lgsrc *h, uint32_t bits)
{
    if (bits > 32)
        return -1;
    h->state->input_bits = bits;
    return 0;
}

/* @0x600a0d98 */
int lgsrc_setOutPCMFormat(struct lgsrc *h, uint32_t bits)
{
    if (bits > 32)
        return -1;
    h->state->output_bits = bits;
    return 0;
}

/* @0x600a0dac: 0..10 inclusive, stores both channels without reinitialization;
 * this API has no independent input/output channel setters.
 */
int lgsrc_setInOutChannel(struct lgsrc *h, uint32_t channels)
{
    if (channels > 10)
        return -1;
    h->state->input_channels = channels;
    h->state->output_channels = channels;
    return 0;
}

/* @0x600a0dc4: arbitrary mode, zero=fast, nonzero=normal. Changed mode invokes
 * full init; init failure is discarded, so the mode remains changed regardless.
 */
int lgsrc_setRunMode(struct lgsrc *h, int32_t mode)
{
    if (h->state->run_mode != mode) {
        h->state->run_mode = mode;
        (void)lgsrc_init(h);
    }
    return 0;
}

/* @0x600a0de4: arbitrary filter word; stores only, no init/changeFs. */
int lgsrc_setFilterType(struct lgsrc *h, uint32_t type)
{
    h->state->filter_type = type;
    return 0;
}

/* @0x600a0df8: 0..512, stores only; invalid returns +1, not -1. */
int lgsrc_setOutSample(struct lgsrc *h, uint32_t samples)
{
    if (samples > 512)
        return 1;
    h->state->output_samples = samples;
    return 0;
}

/* @0x600a0e10 */
uint32_t lgsrc_getInputFs(struct lgsrc *h) { return h->state->input_fs; }
/* @0x600a0e1c */
uint32_t lgsrc_getOutputFs(struct lgsrc *h) { return h->state->output_fs; }
/* @0x600a0e28 */
uint32_t lgsrc_getInPCMFormat(struct lgsrc *h) { return h->state->input_bits; }
/* @0x600a0e34 */
uint32_t lgsrc_getOutPCMFormat(struct lgsrc *h) { return h->state->output_bits; }
/* @0x600a0e40 */
uint32_t lgsrc_getInChannel(struct lgsrc *h) { return h->state->input_channels; }
/* @0x600a0e4c */
uint32_t lgsrc_getOutChannel(struct lgsrc *h) { return h->state->output_channels; }
/* @0x600a0e58 */
int32_t lgsrc_getRunMode(struct lgsrc *h) { return h->state->run_mode; }

/* @0x600a0e6c: required minus buffered, no clamp; unsigned arithmetic preserves
 * the target's wrapping 32-bit register result, including underflow.
 */
uint32_t lgsrc_getNeedfulSamplePer1Ch(struct lgsrc *h)
{
    return h->state->input_samples - h->state->buffered_samples;
}

/* @0x600a0e7c */
uint32_t lgsrc_getNeedfulSample(struct lgsrc *h)
{
    return h->state->input_channels * lgsrc_getNeedfulSamplePer1Ch(h);
}

/* @0x600a0e90: bits>>3 truncates non-byte-aligned formats; no rounding up. */
uint32_t lgsrc_getNeedfulByte(struct lgsrc *h)
{
    return h->state->input_channels * (h->state->input_bits >> 3) *
           lgsrc_getNeedfulSamplePer1Ch(h);
}

/* @0x600a0eac */
double lgsrc_getFactor(struct lgsrc *h) { return h->state->factor; }
/* @0x600a0eb8: returns REQUESTED count; process returns actual count*channels. */
uint32_t lgsrc_getOutSampleNum(struct lgsrc *h) { return h->state->output_samples; }
/* @0x600a0ec4 */
int lgsrc_getVersion(void) { return 1; }
/* @0x600a0ecc: exposes the original work area, not a copy. */
struct lgsrc_state *lgsrc_getDebugParam(struct lgsrc *h) { return h->state; }
