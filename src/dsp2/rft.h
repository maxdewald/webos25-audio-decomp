/* DSP2 RFT state and unresolved arithmetic boundary; VA base 0x60000000. */
#ifndef DSP2_RFT_H
#define DSP2_RFT_H
#include <stddef.h>
#include <stdint.h>

struct rft_config {
    int32_t in_bytes;             /* +00 bits-per-sample / 8 */
    int32_t in_ch;                /* +04 */
    int32_t in_unsigned;          /* +08: exactly 1 selects unsigned alignment */
    int32_t in_endian;            /* +0c: exactly 1 selects byte swapping */
    int32_t out_bytes;            /* +10 */
    int32_t out_ch;               /* +14 */
    int32_t gain_enable;          /* +18: exactly 1 selects gain */
    int32_t gain;                 /* +1c raw word, retained when gain is disabled */
    int32_t acmod;                /* +20 */
    uint8_t gen_lfe;              /* +24 */
    uint8_t reserved[3];
    int32_t processing_frames;    /* +28; firmware calls this ProcessingSample */
    int32_t lfe_history[4];       /* +2c,+30 input history; +34,+38 output history */
};
_Static_assert(sizeof(struct rft_config) == 0x3c, "RFT firmware state size");
_Static_assert(offsetof(struct rft_config, gain_enable) == 0x18, "RFT gain enable offset");
_Static_assert(offsetof(struct rft_config, gain) == 0x1c, "RFT gain word offset");
_Static_assert(offsetof(struct rft_config, processing_frames) == 0x28, "RFT frame offset");
_Static_assert(offsetof(struct rft_config, lfe_history) == 0x2c, "RFT LFE offset");

/* [FLIX] First gain result @6009dc0a..6009dc2e, repeated result
 * @6009dc38..6009dc50: AE_L32F24 -> MULF32S.LL -> SLAISQ56S 8 -> SAT48S
 * -> TRUNCA32Q48. The overlay validates scalar/branch slots only, not numeric
 * DSP semantics. Do not infer unity identity, precision, rounding or clipping.
 * Sample load/store/iteration @dc0d/dc2f/dc31/dc34/dc40/dc51 are recovered.
 * The scalar slot at dc40 advances OUTPUT, not gain or the input count.
 * Both arguments are raw 32-bit words: no Q-format or numeric zero/unity
 * result is established. Keeping this external hook unresolved is deliberate. */
extern int32_t rft_gain_sample_flix(int32_t sample, int32_t gain);
/* [FLIX] @6009d7b3..6009d830: coefficient table 0x60006f80, mulfp32x2rs,
 * mulsfp32x2rs, add32_hl_lh, slai32s, sel16i, add32s. Receives the averaged
 * mono plane and updates four state words. Exact filter math unresolved. */
extern void rft_lfe_filter_flix(struct rft_config *, int32_t *, size_t);
#endif
