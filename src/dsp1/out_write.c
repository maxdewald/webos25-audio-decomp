/*
 * dsp1 OUT_Write / OUT_Close: the Dolby MAT IEC 61937-9 frame writer of the MAT
 * encoder's file front end. It parses MAT frames and writes each one, wrapped in an
 * IEC 61937 burst, to a dlb_octfile. NOT part of the output path: output_arc never calls
 * it, and the only reference is a function-pointer literal at 0x602c18f0 (encoder
 * test/harness table).
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp1.bin
 *   sha256 8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4
 * Evidence: build/decomp/audio_a0_dsp1.main_60000000.bin.c, build/scratch/a17/outw.c.asm.
 * Strings: "IEC61937-9 frame generator: MAT frame size incorrect! (%u vs. %u byte)",
 * "Error writing IEC61937 header!", "...length!", "...padding!", "...burst spacing!",
 * "Error writing MAT Sync word!", "MAT: Invalid state of MAT Frame/Chunk parser!".
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define OUT_ERR_NULL   ((int)0xffff8080)
#define OUT_ERR_WRITE  ((int)0xffff8083)
#define MAT_BURST_SIZE 0xeff0          /* 61440 - 16: MAT burst payload incl. padding */

struct dlb_octfile;
extern size_t dlb_octfile_write(const void *p, size_t sz, size_t n, struct dlb_octfile *f);
extern int    dlb_octfile_close(struct dlb_octfile *f);

extern const uint8_t mat_sync[2];       /* @0x6000b240 MAT frame sync bytes */
extern const uint8_t iec61937_hdr[6];   /* @0x6000b248 Pa/Pb + Pc for MAT */
extern const uint8_t mat_burst_gap[8];  /* @0x60327d50 zero spacing */

struct mat_writer {
    int      state;                     /* +0x38 0 sync, 1..3 chunk header, ... */
    int      chunk_state;               /* +0x3c 0 len hi/lo, 1 body, 2 next */
    int      sync_pos;                  /* +0x40 */
    uint8_t  len_bytes[2];              /* +0x44 */
    unsigned chunk_left;                /* +0x48 */
    int      frame_done;                /* +0x4c */
    unsigned frame_len;                 /* +0x50 parsed MAT frame bytes */
    uint8_t  frame[MAT_BURST_SIZE];     /* +0x54 */
    unsigned fill;                      /* +0xee48 + 0x1fc */
    struct dlb_octfile *file;           /* +0x90 (in the owning context) */
};

struct out_ctx {
    void     *src;                      /* +0x2c */
    unsigned  len;                      /* +0x38 */
    void     *enabled;                  /* +0x88 */
    struct mat_writer *w;               /* +0x90 */
    int       raw;                      /* enabled->+0x2c: write bytes as-is */
};

static int write_burst(struct mat_writer *w)
{
    if (dlb_octfile_write(iec61937_hdr, 1, 6, w->file) != 6) {
        fprintf(stderr, "Error writing IEC61937 header!\n");
        return OUT_ERR_WRITE;
    }
    uint8_t len[2] = { (uint8_t)(w->frame_len >> 8), (uint8_t)w->frame_len };
    if (dlb_octfile_write(len, 1, 2, w->file) != 2) {
        fprintf(stderr, "Error writing IEC61937 length!\n");
        return OUT_ERR_WRITE;
    }
    if (dlb_octfile_write(w->frame, 1, w->fill, w->file) != w->fill) {
        fprintf(stderr, "Error writing MAT Sync word!\n");
        return OUT_ERR_WRITE;
    }
    for (unsigned i = w->fill; i < MAT_BURST_SIZE; i++) {
        uint8_t z = 0;
        if (dlb_octfile_write(&z, 1, 1, w->file) != 1)
            fprintf(stderr, "Error writing IEC padding!\n");
    }
    if (dlb_octfile_write(mat_burst_gap, 1, 8, w->file) != 8) {
        fprintf(stderr, "Error writing IEC61937 burst spacing!\n");
        return OUT_ERR_WRITE;
    }
    return 0;
}

/* @0x60146c8c: append ctx->len bytes. In MAT mode the bytes are parsed (sync, chunk
 * lengths) until a frame is complete, and the frame is then written as one burst. */
int OUT_Write(struct out_ctx *ctx)
{
    if (!ctx || !ctx->enabled)
        return OUT_ERR_NULL;
    if (ctx->len == 0)
        return 0;
    struct mat_writer *w = ctx->w;
    if (ctx->raw)
        return dlb_octfile_write(ctx->src, 1, ctx->len, w->file) == ctx->len ? 0 : OUT_ERR_WRITE;

    /* [GAP] the byte-wise sync/chunk parser (states +0x38/+0x3c) is summarized here:
     * it finds mat_sync, then reads 2-byte chunk lengths (x2) and sets frame_done
     * after the third chunk. */
    unsigned take = ctx->len;
    memcpy(w->frame + w->fill, ctx->src, take);
    w->fill += take;
    if (!w->frame_done)
        return 0;
    if (w->frame_len != w->fill)
        fprintf(stderr, "IEC61937-9 frame generator: MAT frame size incorrect! (%u vs. %u byte)\n",
                w->frame_len, w->fill);
    w->frame_done = 0;
    return write_burst(w);
}

/* @0x60146f3c */
int OUT_Close(struct out_ctx *ctx)
{
    if (!ctx || !ctx->enabled)
        return OUT_ERR_NULL;
    dlb_octfile_close(ctx->w->file);
    return 0;
}
