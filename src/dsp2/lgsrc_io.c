/* DSP2 scalar SRC PCM I/O, audio_a0_dsp2.main_60000000.bin.
 * Opaque state is the exact 0x19370-byte work area in lgsrc.c; no host pointers
 * or private layout replace its fields. PCM and work-area samples are LE.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct lgsrc_state;

_Static_assert(sizeof(int) == sizeof(int32_t), "DSP int return width");

static uint16_t lgsrc_io_load16(const unsigned char *p)
{
    return (uint16_t)((uint16_t)p[0] | (uint16_t)p[1] << 8);
}

static uint32_t lgsrc_io_load32(const unsigned char *p)
{
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 |
           (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

static int32_t lgsrc_io_signed(uint32_t bits)
{
    int32_t value;
    memcpy(&value, &bits, sizeof(value));
    return value;
}

static void lgsrc_io_store16(unsigned char *p, uint16_t value)
{
    p[0] = (unsigned char)value;
    p[1] = (unsigned char)(value >> 8);
}

static unsigned lgsrc_io_sample_bytes(uint32_t bits)
{
    switch (bits) {
    case 8: return 1;
    case 16: return 2;
    case 24: return 3;
    case 32: return 4;
    default: return 0;
    }
}

/* @0x6009f120: interleaved PCM -> signed-16 planes +0x40+c*0x2800,
 * appending after u16 buffered_samples (+0x20), without changing that field.
 * Read exactly signed32(input_samples-buffered_samples) frames, not the total.
 * 8-bit is bit-preserving signed PCM (NO unsigned-PCM bias); 24/32 drop their
 * low 8/16 bits. Branches @130/138/142 dispatch 8/16/24; scalar bodies @1f0,
 * @249, @2d2, @151 respectively. Hardware loopgtz instructions iterate ALL
 * remainder/group channels, despite Ghidra's single-iteration pseudocode.
 * @358 returns channels*missing modulo 2^32 even for nonpositive counts or
 * channels. The only -1 error is an unsupported bit depth, before any access.
 * FLIX bundles in this function contain decoded branches + nop, no gaps.
 */
int lgsrcIo_readData(const void *input, struct lgsrc_state *state)
{
    unsigned char *s = (unsigned char *)state;
    uint32_t channels = lgsrc_io_load32(s + 0x04);
    unsigned bytes = lgsrc_io_sample_bytes(lgsrc_io_load32(s + 0x08));
    if (bytes == 0)
        return -1;
    uint16_t buffered = lgsrc_io_load16(s + 0x20);
    uint32_t missing = lgsrc_io_load32(s + 0x0c) - buffered;
    if (lgsrc_io_signed(missing) > 0 && lgsrc_io_signed(channels) > 0) {
        const unsigned char *pcm = input;
        for (uint32_t frame = 0; frame < missing; ++frame) {
            for (uint32_t channel = 0; channel < channels; ++channel) {
                uint16_t sample;
                if (bytes == 1)
                    sample = (uint16_t)((uint16_t)pcm[0] << 8);
                else
                    sample = lgsrc_io_load16(pcm + bytes - 2);
                lgsrc_io_store16(s + 0x40 + (size_t)channel * 0x2800 +
                                ((size_t)buffered + frame) * 2, sample);
                pcm += bytes;
            }
        }
    }
    return lgsrc_io_signed(channels * missing);
}

/* @0x6009f360: signed-16 planes +0x2440+c*0x2800 -> interleaved PCM.
 * Uses signed requested output_samples (+0x1c), NOT u16 produced_samples
 * (+0x24). +0x2440 is literal [0x600611b0]=0x23c4 plus l16si +0x7c;
 * 8-bit uses [0x600611b4]=0x2344 plus +0xfc, the SAME plane address.
 * Scalar 8/16/24/32 bodies @435/@49d/@528/@398: truncate high byte, copy
 * 16 bits, prepend one zero low byte, prepend two zero low bytes. No rounding,
 * saturation or sign rebias. Output preserves requested count and all state.
 * @421/@48a/@512/@636 return the advanced DSP pointer as a 32-bit int, NOT
 * bytes/samples written; negative counts/channels are independently max(x,0).
 * On a host, preserve that ABI by returning the low 32 bits of the address.
 * All FLIX here are decoded branches + nop; sinc generation @650 is separate.
 */
int lgsrcIo_writeData(void *output, struct lgsrc_state *state)
{
    const unsigned char *s = (const unsigned char *)state;
    uint32_t channels = lgsrc_io_load32(s + 0x14);
    uint32_t frames = lgsrc_io_load32(s + 0x1c);
    unsigned bytes = lgsrc_io_sample_bytes(lgsrc_io_load32(s + 0x18));
    if (bytes == 0)
        return -1;
    if (lgsrc_io_signed(channels) < 0)
        channels = 0;
    if (lgsrc_io_signed(frames) < 0)
        frames = 0;
    unsigned char *pcm = output;
    for (uint32_t frame = 0; frame < frames && channels != 0; ++frame) {
        for (uint32_t channel = 0; channel < channels; ++channel) {
            uint16_t sample = lgsrc_io_load16(s + 0x2440 +
                                (size_t)channel * 0x2800 + (size_t)frame * 2);
            if (bytes == 1) {
                pcm[0] = (unsigned char)(sample >> 8);
            } else {
                for (unsigned low = 0; low < bytes - 2; ++low)
                    pcm[low] = 0;
                lgsrc_io_store16(pcm + bytes - 2, sample);
            }
            pcm += bytes;
        }
    }
    uint32_t end = (uint32_t)(uintptr_t)output + frames * channels * bytes;
    return lgsrc_io_signed(end);
}
