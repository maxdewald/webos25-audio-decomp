/* DSP2 RFT reconstruction, main image VA base 0x60000000.
 * Evidence: saved decompilation and exact-entry xtobjdump listings of
 * 0x6009ce28..0x6009e4a8. FLIX branch slots are validated, SIMD math is not.
 * No sample-rate field or rate-dependent branch exists in this family.
 * Log-only branches are omitted; invalid-format helper failures are deliberately
 * ignored by LGRFT, just as in the image. Buffers require caller-sized storage.
 */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "rft.h"

static uint32_t rft_load_le(const uint8_t *p, unsigned bytes)
{
    uint32_t v = 0;
    for (unsigned j = 0; j < bytes; ++j)
        v |= (uint32_t)p[j] << (8u * j);
    return v;
}
static void rft_store_le(uint8_t *p, uint32_t v, unsigned bytes)
{
    for (unsigned j = 0; j < bytes; ++j)
        p[j] = (uint8_t)(v >> (8u * j));
}
static int32_t rft_sar(int32_t v, unsigned bits)
{
    int64_t divisor = (int64_t)1 << bits;
    return (int32_t)(v >= 0 ? (int64_t)v / divisor :
                     -((-(int64_t)v + divisor - 1) / divisor));
}
static int32_t rft_bits(uint32_t v)
{
    int32_t result;
    memcpy(&result, &v, sizeof(result));
    return result;
}
static int32_t rft_count(int32_t channels, int32_t frames)
{
    return rft_bits((uint32_t)channels * (uint32_t)frames);
}
static int32_t rft_mean(int32_t a, int32_t b)
{
    return rft_sar(a, 1) + rft_sar(b, 1);
}

/* @0x6009ce28: signed channel upper bounds ONLY; zero/negative counts pass. */
int LGRFT_CheckConfig(const void *config)
{
    const struct rft_config *r = config;
    return r != NULL && r->in_ch <= 10 && r->out_ch <= 10 ? 0 : -1;
}

/* @0x6009cfa8: width 1 copies; widths 2/3/4 reverse each sample's bytes.
 * Assertions @cfb5/cfc1 bound frames <=1280 and in_ch <=10, not positivity.
 * Unknown widths silently do nothing and return zero. */
int _LGRFT_SwapEndian(void *config, const void *input, void *output)
{
    const struct rft_config *r = config;
    const uint8_t *src = input;
    uint8_t *dst = output;
    assert(r->processing_frames <= 0x500);
    assert(r->in_ch <= 10);
    if (r->in_bytes < 1 || r->in_bytes > 4)
        return 0;
    for (int32_t i = 0, n = rft_count(r->in_ch, r->processing_frames); i < n; ++i) {
        uint8_t sample[4];
        memcpy(sample, src, (size_t)r->in_bytes);
        for (int32_t j = 0; j < r->in_bytes; ++j)
            dst[j] = sample[r->in_bytes - 1 - j];
        src += r->in_bytes;
        dst += r->in_bytes;
    }
    return 0;
}

/* @0x6009d21c: ALL widths are recovered scalar, including the 3/4-byte branches
 * hidden from Ghidra by FLIX branch bundles. @d270..d2c7 signed16 is precisely
 * sample bits <<16. Packed24 is little-endian sample bits <<8; 32 is memcpy.
 * Therefore, absent gain/channel changes, 16->32 adds 16 zero LOW bits, not
 * sign extension into a low-justified 32-bit container. */
int _LGRFT_AlignSignedto32(void *config, const void *input, void *output)
{
    const struct rft_config *r = config;
    const uint8_t *src = input;
    uint8_t *dst = output;
    if (r->in_bytes < 1 || r->in_bytes > 4)
        return -1;
    for (int32_t i = 0, n = rft_count(r->in_ch, r->processing_frames); i < n; ++i) {
        uint32_t v = rft_load_le(src, (unsigned)r->in_bytes);
        rft_store_le(dst, v << (32u - 8u * (unsigned)r->in_bytes), 4);
        src += r->in_bytes;
        dst += 4;
    }
    return 0;
}

/* @0x6009d36c: subtract unsigned midpoint, then left-align; literal pools
 * @600610b4=80000000, @600610b8=ff800000 confirm 32/24-bit cases. */
int _LGRFT_AlignUnsignedto32(void *config, const void *input, void *output)
{
    const struct rft_config *r = config;
    const uint8_t *src = input;
    uint8_t *dst = output;
    if (r->in_bytes < 1 || r->in_bytes > 4)
        return -1;
    unsigned bits = 8u * (unsigned)r->in_bytes;
    for (int32_t i = 0, n = rft_count(r->in_ch, r->processing_frames); i < n; ++i) {
        uint32_t v = rft_load_le(src, (unsigned)r->in_bytes) - (UINT32_C(1) << (bits - 1));
        rft_store_le(dst, v << (32u - bits), 4);
        src += r->in_bytes;
        dst += 4;
    }
    return 0;
}

/* @0x6009d4bc: scalar truncation, no saturation/dither/rounding. @d550..d5a4
 * arithmetic >>16 followed by s16i; @d4ed..d51b 32-bit output is word copy.
 * Equal-channel signed16->16 forced through these scalar helpers is exactly
 * byte-identical IF gain is not run. Enabled gain is not proven transparent. */
int _LGRFT_WriteOut(void *config, const void *input, void *output)
{
    const struct rft_config *r = config;
    const uint8_t *src = input;
    uint8_t *dst = output;
    if (r->out_bytes < 1 || r->out_bytes > 4)
        return -1;
    for (int32_t i = 0, n = rft_count(r->out_ch, r->processing_frames); i < n; ++i) {
        uint32_t v = rft_load_le(src, 4);
        rft_store_le(dst, v >> (32u - 8u * (unsigned)r->out_bytes), (unsigned)r->out_bytes);
        src += 4;
        dst += r->out_bytes;
    }
    return 0;
}

/* @0x6009d62c: stereo -> L,R,filtered((L>>1)+(R>>1)); preserves L/R planes.
 * @d6e9..d7ad proves separate shifts, so odd negative inputs round separately.
 * Scalar de/interleave recovered; filter/state semantics remain [FLIX] above.
 * Caller selects this ONLY for in_ch2,out_ch3,gen_lfe!=0, before endian swap. */
void _LGRFT_GenerateLFE2ch(void *config, const int32_t *src, int32_t *dst)
{
    struct rft_config *r = config;
    if (r->processing_frames == 0)
        return;
    assert(r->processing_frames > 0);
    size_t n = (size_t)r->processing_frames;
    int32_t planes[n * 3];
    for (size_t i = 0; i < n; ++i) {
        planes[i] = src[i * 2];
        planes[n + i] = src[i * 2 + 1];
        planes[n * 2 + i] = rft_mean(src[i * 2], src[i * 2 + 1]);
    }
    rft_lfe_filter_flix(r, planes + n * 2, n);
    for (size_t i = 0; i < n; ++i)
        for (int32_t ch = 0; ch < r->out_ch; ++ch)
            dst[i * (size_t)r->out_ch + (size_t)ch] = planes[(size_t)ch * n + i];
}

/* @0x6009dc00: count is total interleaved samples, not frames. [FLIX] multiply
 * runs even at a nominal unity value; no equality/zero/unity shortcut exists. */
int _LGRFT_Gai(void *config, const int32_t *src, int32_t *dst, int32_t count)
{
    const struct rft_config *r = config;
    int32_t gain = r->gain; /* @dc03 cached before the count check, not per sample. */
    for (int32_t i = 0; i < count; ++i)
        dst[i] = rft_gain_sample_flix(src[i], gain);
    return 0;
}

/* @0x6009dee4: default downmix into two channels. Scalar @df90..e22c.
 * 4ch sums both extra channels into BOTH outputs; 6ch ignores index3 (LFE).
 * 8/10ch use 82/58/57 weights after >>8, ignoring index3 and (10ch) 8/9.
 * Integer wrap, NOT saturation. Other channel counts retain first two words. */
void _LGRFT_DownMix(void *config, const int32_t *src, int32_t *dst)
{
    const struct rft_config *r = config;
    for (int32_t i = 0; i < r->processing_frames; ++i) {
        uint32_t left = (uint32_t)src[0], right = (uint32_t)src[1];
        if (r->in_ch == 4) {
            uint32_t extra = (uint32_t)rft_sar(src[2], 8) * 75u + (uint32_t)rft_sar(src[3], 8) * 75u;
            left = extra + (uint32_t)rft_sar(src[0], 8) * 106u;
            right = extra + (uint32_t)rft_sar(src[1], 8) * 106u;
        } else if (r->in_ch == 6) {
            uint32_t center = (uint32_t)rft_sar(src[2], 8) * 75u;
            left = center + (uint32_t)rft_sar(src[0], 8) * 106u + (uint32_t)rft_sar(src[4], 8) * 75u;
            right = center + (uint32_t)rft_sar(src[1], 8) * 106u + (uint32_t)rft_sar(src[5], 8) * 75u;
        } else if (r->in_ch == 8 || r->in_ch == 10) {
            uint32_t center = (uint32_t)rft_sar(src[2], 8) * 58u;
            left = center + (uint32_t)rft_sar(src[0], 8) * 82u + (uint32_t)rft_sar(src[4], 8) * 58u + (uint32_t)rft_sar(src[6], 8) * 57u;
            right = center + (uint32_t)rft_sar(src[1], 8) * 82u + (uint32_t)rft_sar(src[5], 8) * 58u + (uint32_t)rft_sar(src[7], 8) * 57u;
        }
        dst[0] = rft_bits(left);
        dst[1] = rft_bits(right);
        src += r->in_ch;
        dst += 2;
    }
}

/* @0x6009e230: acmod2 selects the first pair; channels <2 leave output alone. */
void _LGRFT_DownMix_Multo2Ch_Acmod2(void *config, const int32_t *src, int32_t *dst)
{
    const struct rft_config *r = config;
    if (r->in_ch < 2)
        return;
    for (int32_t i = 0; i < r->processing_frames; ++i) {
        dst[0] = src[0]; dst[1] = src[1];
        src += r->in_ch; dst += 2;
    }
}
/* @0x6009e2e0: acmod1 duplicates input word2, even for in_ch==2. This odd
 * bound/read mismatch is present @e2ea/e30a; do not substitute input word0. */
void _LGRFT_DownMix_Multo2Ch_Acmod1(void *config, const int32_t *src, int32_t *dst)
{
    const struct rft_config *r = config;
    if (r->in_ch < 2)
        return;
    for (int32_t i = 0; i < r->processing_frames; ++i) {
        int32_t v = src[2];
        dst[0] = v; dst[1] = v;
        src += r->in_ch; dst += 2;
    }
}
/* @0x6009e390: acmod0 duplicates separately-shifted first-pair average. */
void _LGRFT_DownMix_Multo2Ch_Acmod0(void *config, const int32_t *src, int32_t *dst)
{
    const struct rft_config *r = config;
    if (r->in_ch < 2)
        return;
    for (int32_t i = 0; i < r->processing_frames; ++i) {
        int32_t v = rft_mean(src[0], src[1]);
        dst[0] = v; dst[1] = v;
        src += r->in_ch; dst += 2;
    }
}

/* @0x6009d894: normal channel policy. Mono->stereo duplicates backwards;
 * any->mono averages ONLY first two. Any->stereo dispatches acmod 2/1/0 or
 * default downmix; other reductions discard trailing channels. Other expansion
 * zero-pads BACKWARDS (alias-safe) but returns -1 even after writing @d998.
 * LGRFT ignores that status. These are not ConvertAvailableCh semantics. */
int _LGRFT_ConvertCh(void *config, const int32_t *src, int32_t *dst)
{
    const struct rft_config *r = config;
    if (src == NULL || dst == NULL)
        return -1;
    if (r->in_ch == r->out_ch) {
        if (src != dst)
            memcpy(dst, src, (size_t)rft_count(r->in_ch, r->processing_frames) * 4u);
        return 0;
    }
    if (r->in_ch == 1 && r->out_ch == 2) {
        for (int32_t i = r->processing_frames; i > 0; --i) {
            int32_t v = src[i - 1];
            dst[2 * (i - 1)] = v; dst[2 * (i - 1) + 1] = v;
        }
        return 0;
    }
    if (r->out_ch == 2) {
        if (r->acmod == 2) _LGRFT_DownMix_Multo2Ch_Acmod2(config, src, dst);
        else if (r->acmod == 1) _LGRFT_DownMix_Multo2Ch_Acmod1(config, src, dst);
        else if (r->acmod == 0) _LGRFT_DownMix_Multo2Ch_Acmod0(config, src, dst);
        else _LGRFT_DownMix(config, src, dst);
        return 0;
    }
    if (r->out_ch == 1) {
        for (int32_t i = 0; i < r->processing_frames; ++i)
            dst[i] = rft_mean(src[i * r->in_ch], src[i * r->in_ch + 1]);
        return 0;
    }
    if (r->out_ch < r->in_ch) {
        for (int32_t i = 0; i < r->processing_frames; ++i)
            for (int32_t ch = 0; ch < r->out_ch; ++ch)
                dst[i * r->out_ch + ch] = src[i * r->in_ch + ch];
        return 0;
    }
    for (int32_t i = r->processing_frames; i > 0; --i)
        for (int32_t ch = r->out_ch; ch > 0; --ch)
            dst[(i - 1) * r->out_ch + ch - 1] = ch > r->in_ch ? 0 : src[(i - 1) * r->in_ch + ch - 1];
    return -1;
}

/* @0x6009daf4: retain first min(in,out) channels, append zeros. Expansion is
 * FORWARD @dbd6..dbf3 even when buffers alias, unlike ConvertCh. Preserve this
 * overwrite behavior, not an invented alias-safe channel mapping. */
int _LGRFT_ConvertAvailableCh(void *config, const int32_t *src, int32_t *dst)
{
    const struct rft_config *r = config;
    if (src == NULL || dst == NULL)
        return -1;
    if (r->in_ch == r->out_ch) {
        if (src != dst)
            memcpy(dst, src, (size_t)rft_count(r->in_ch, r->processing_frames) * 4u);
        return 0;
    }
    for (int32_t i = 0; i < r->processing_frames; ++i) {
        int32_t ch = 0;
        while (ch < r->in_ch && ch < r->out_ch) {
            dst[ch] = src[ch];
            ++ch;
        }
        while (ch < r->out_ch)
            dst[ch++] = 0;
        src += r->in_ch;
        dst += r->out_ch;
    }
    return 0;
}

/* @0x6009e410: actual ABI is (config, OUTPUT, INPUT), reversed relative to
 * other helpers. @e47a loads third argument, e47c stores second. Forward-only
 * expansion, zeros after copied input channels; not called by LGRFT. */
void _LGRFT_UpMix(void *config, int32_t *dst, const int32_t *src)
{
    const struct rft_config *r = config;
    assert(r->out_ch - r->in_ch >= 0);
    for (int32_t i = 0; i < r->processing_frames; ++i) {
        for (int32_t ch = 0; ch < r->in_ch; ++ch) dst[ch] = src[ch];
        for (int32_t ch = r->in_ch; ch < r->out_ch; ++ch) dst[ch] = 0;
        src += r->in_ch; dst += r->out_ch;
    }
}

/* @0x6009ce6c: returns total output samples, not bytes/frames. Scratch is
 * mandatory even on copy path. @cf5d..cf66 proves Ghidra omitted gain==1 test:
 * equal widths/channels, native endian, signed input copies ONLY gain!=1.
 * Thus 8ch signed16->16 is proven byte-copy when gain!=1 (at ANY rate).
 * gain==1 forces alignment + unresolved gain + writeout even with equal formats;
 * its byte identity cannot be asserted. 16->32 without gain/channel/endian
 * changes is exact signed sample bits <<16, all scalar. LFE special path bypasses
 * endian swap entirely. available_mode!=0 selects truncation/zero-fill policy. */
int LGRFT(void *config, const void *input, void *output, void *scratch, int available_mode)
{
    struct rft_config *r = config;
    if (r == NULL || input == NULL || output == NULL || scratch == NULL)
        return -1;
    int checked = LGRFT_CheckConfig(config);
    if (checked != 0)
        return checked;
    int32_t out_count = rft_count(r->out_ch, r->processing_frames);
    if (r->out_bytes == r->in_bytes && r->out_ch == r->in_ch &&
        r->in_endian == 0 && r->in_unsigned == 0 && r->gain_enable != 1) {
        memcpy(output, input, (size_t)rft_count(r->in_bytes, out_count));
        return out_count;
    }
    if (r->in_ch == 2 && r->out_ch == 3 && r->gen_lfe != 0) {
        if (r->in_unsigned == 1) (void)_LGRFT_AlignUnsignedto32(config, input, scratch);
        else (void)_LGRFT_AlignSignedto32(config, input, scratch);
        if (r->gain_enable == 1)
            (void)_LGRFT_Gai(config, scratch, scratch, rft_count(r->in_ch, r->processing_frames));
        _LGRFT_GenerateLFE2ch(config, scratch, output);
        (void)_LGRFT_WriteOut(config, output, output);
        return out_count;
    }
    if (r->in_endian == 1) {
        (void)_LGRFT_SwapEndian(config, input, output);
        input = output;
    }
    if (r->in_unsigned == 1) (void)_LGRFT_AlignUnsignedto32(config, input, scratch);
    else (void)_LGRFT_AlignSignedto32(config, input, scratch);
    if (r->in_ch != r->out_ch) {
        if (available_mode != 0) (void)_LGRFT_ConvertAvailableCh(config, scratch, scratch);
        else (void)_LGRFT_ConvertCh(config, scratch, scratch);
    }
    if (r->gain_enable == 1) (void)_LGRFT_Gai(config, scratch, scratch, out_count);
    (void)_LGRFT_WriteOut(config, scratch, output);
    return out_count;
}

/* @0x6009dc5c: setters store bits >>3, without validating format. */
int RFT_SetInPCMFormat(void *p, int bits)
{
    if (p == NULL) return -1;
    ((struct rft_config *)p)->in_bytes = rft_sar(bits, 3); return 0;
}
/* @0x6009dc9c: >10 logs only, still stores; negative channels accepted. */
int RFT_SetInChannel(void *p, int ch)
{
    if (p == NULL) return -1;
    ((struct rft_config *)p)->in_ch = ch; return 0;
}
/* @0x6009dcf8 */
int RFT_SetInSigned(void *p, int v)
{
    if (p == NULL) return -1;
    ((struct rft_config *)p)->in_unsigned = v; return 0;
}
/* @0x6009dd08: endian alone validates its enum. */
int RFT_SetInEndian(void *p, int v)
{
    if (p == NULL || (v != 0 && v != 1)) return -1;
    ((struct rft_config *)p)->in_endian = v; return 0;
}
/* @0x6009dd1c */
int RFT_SetAcmod(void *p, int v)
{
    if (p == NULL) return -1;
    ((struct rft_config *)p)->acmod = v; return 0;
}
/* @0x6009dd50: every call resets four histories, even setting same enable. */
int RFT_SetGenLFE(void *p, int v)
{
    if (p == NULL) return -1;
    struct rft_config *r = p;
    memset(r->lfe_history, 0, sizeof(r->lfe_history));
    r->gen_lfe = (uint8_t)(v != 0); return 0;
}
/* @0x6009dda4 */
int RFT_SetOutPCMFormat(void *p, int bits)
{
    if (p == NULL) return -1;
    ((struct rft_config *)p)->out_bytes = rft_sar(bits, 3); return 0;
}
/* @0x6009dde4: same logging-only upper bound as input. */
int RFT_SetOutChannel(void *p, int ch)
{
    if (p == NULL) return -1;
    ((struct rft_config *)p)->out_ch = ch; return 0;
}
/* @0x6009de40: only enable==1 stores gain; disabling leaves prior gain. */
int RFT_SetGai(void *p, int enable, int gain)
{
    if (p == NULL) return -1;
    struct rft_config *r = p;
    if (enable == 1) { r->gain = gain; r->gain_enable = 1; }
    else r->gain_enable = 0;
    return 0;
}
/* @0x6009de60 */
int RFT_SetOutSample(void *p, int frames)
{
    if (p == NULL) return -1;
    ((struct rft_config *)p)->processing_frames = frames; return 0;
}
/* Getters deliberately have no null guard, matching the image. */
/* @0x6009de70 */
int RFT_GetInPCMFormat(const void *p) { return rft_bits((uint32_t)((const struct rft_config *)p)->in_bytes << 3); }
/* @0x6009de7c */
int RFT_GetInChannel(const void *p) { return ((const struct rft_config *)p)->in_ch; }
/* @0x6009de84 */
int RFT_GetInSigned(const void *p) { return ((const struct rft_config *)p)->in_unsigned; }
/* @0x6009de8c */
int RFT_GetInEndian(const void *p) { return ((const struct rft_config *)p)->in_endian; }
/* @0x6009de94 */
int RFT_GetAcmod(const void *p) { return ((const struct rft_config *)p)->acmod; }
/* @0x6009de9c */
int RFT_GetGenLFE(const void *p) { return ((const struct rft_config *)p)->gen_lfe; }
/* @0x6009dea4 */
int RFT_GetOutPCMFormat(const void *p) { return rft_bits((uint32_t)((const struct rft_config *)p)->out_bytes << 3); }
/* @0x6009deb0 */
int RFT_GetOutChannel(const void *p) { return ((const struct rft_config *)p)->out_ch; }
/* @0x6009deb8: output is always signed. */
int RFT_GetOutSigned(void) { return 0; }
/* @0x6009dec0: output is always native little endian. */
int RFT_GetOutEndian(void) { return 0; }
/* @0x6009dec8 */
int RFT_GetGainEnable(const void *p) { return ((const struct rft_config *)p)->gain_enable; }
/* @0x6009ded0 */
int RFT_GetGai(const void *p) { return ((const struct rft_config *)p)->gain; }
/* @0x6009ded8: input interleaved samples required for processing_frames. */
int RFT_GetNeedfulSample(const void *p)
{
    const struct rft_config *r = p;
    return rft_count(r->in_ch, r->processing_frames);
}
