/* Host reconstruction checks, not execution of the firmware or SIMD validation.
 * usage: clang -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined
 *   -ffunction-sections -fdata-sections tools/tests/dsp1/output_bc.c
 *   -Wl,-dead_strip -o build/scratch/dsp1-output-bc && build/scratch/dsp1-output-bc
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
static uint32_t aud_regs[256], aoc_regs[256];
static volatile uint32_t *mock_reg(uint32_t base, unsigned off)
{
    assert(off % 4 == 0 && off < 1024);
    if (base == 0xc0fffe00u) return &aud_regs[off / 4];
    assert(base == 0xc1000600u);
    return &aoc_regs[off / 4];
}
#define REG(base, off) (*mock_reg((base), (off)))
#include "../../../src/dsp1/out_module.c"
#define ceil_log2 arc_ceil_log2
#include "../../../src/dsp1/output_arc.c"
#undef ceil_log2

static _Alignas(16) uint8_t ring[4 * 65536];
static uint32_t desc[32];
static struct out_module m;
static struct out_cfg cfg;
static struct aud_port port;
static unsigned puts_count, pushes, waits, completes, cache_calls, starts, stops;
static unsigned mute_on, mute_off, enables, disables, status_events, last_events;
static unsigned defaults, remaps, irq_depth, zero_allocs;
static unsigned cache_bytes;
static void (*wait_action)(void);
static struct {
    const char *codec;
    int channels, rate, bps, acmod;
    bool es, mapping;
} format;

uint32_t irq_save(void) { assert(!irq_depth); irq_depth = 1; return 0; }
void irq_restore(uint32_t ps) { (void)ps; assert(irq_depth); irq_depth = 0; }
uint32_t HAL_GetCurrentStch(void) { return 0; }
void HAL_ClearInterrupt(unsigned irq) { assert(irq == 16); }
void HAL_CacheRegionWriteback(void *p, unsigned n)
{
    assert((uint8_t *)p >= ring && (uint8_t *)p + n <= ring + sizeof(ring));
    ++cache_calls; cache_bytes += n;
}
uint8_t *AddrTrans_GetAddr(uint32_t phys) { assert(phys == 0x100000); return ring; }
void irq_register(int irq, const char *name, void (*fn)(struct out_module *),
                  struct out_module *arg)
{
    assert(irq == 16);
    if (fn) { assert(name && arg == &m); } else { assert(!name && !arg); }
}
void xos_interrupt_enable(int irq) { assert(irq == 16); ++enables; }
void xos_interrupt_disable(int irq) { assert(irq == 16); ++disables; }
void HAL_SetMAOCChannelSet_BufferController(int ch) { assert(ch == 4); }
void aud_reinit_completion(struct aud_completion *c) { assert(c == &m.done); }
void aud_complete(struct aud_completion *c) { assert(c == &m.done); ++completes; }
void aud_wait_for_completion(struct aud_completion *c)
{
    assert(c == &m.done && !irq_depth && wait_action && waits < 8);
    ++waits; wait_action();
}
void aud_object_put(void *obj) { assert(obj); ++puts_count; }
int aud_module_push_data(void *obj, int index, struct aud_data *d)
{ assert(obj == &m && index == 0 && d); ++pushes; return 0; }
struct aud_data *aud_data_new_zero(unsigned size)
{ (void)size; ++zero_allocs; return NULL; }
int aud_module_default_event(void *obj, struct aud_port *p, struct aud_event *e)
{ assert(obj == &m && p == &port && e); ++defaults; return 23; }
int aud_event_value_get_int(struct aud_event *e, const char *k, int def)
{
    (void)e;
    if (!strcmp(k, "channels")) return format.channels;
    if (!strcmp(k, "data-rate")) return format.rate;
    if (!strcmp(k, "bits-per-sample")) return format.bps;
    if (!strcmp(k, "acmod")) return format.acmod;
    return def;
}
bool aud_event_value_get_bool(struct aud_event *e, const char *k, bool def)
{
    (void)e;
    if (!strcmp(k, "es")) return format.es;
    if (!strcmp(k, "ch-mapping-lfe-c")) return format.mapping;
    return def;
}
const char *aud_event_value_get_str(struct aud_event *e, const char *k, const char *def)
{ (void)e; return !strcmp(k, "codec") ? format.codec : def; }
void aud_event_value_set_int(struct aud_event *e, const char *k, int v)
{ (void)e; assert(!strcmp(k, "cap-ch-remap") && v == 1); ++remaps; }
int aud_event_has_name(struct aud_event *e, const char *name)
{ (void)e; return !strcmp(name, "is_first_data"); }
void aimc_raise_named_event(void *ctx, int id, const char *name, const void *p, int len)
{
    (void)ctx; (void)id;
    if (!strcmp(name, "output_status")) { assert(p && len == 16); ++status_events; }
    else { assert(!strcmp(name, "last") && !p && !len); ++last_events; }
}
void HAL_SetIEC1958ValidBit(int v, int p) { assert((v == 0 || v == 1) && p == 5); }
void HAL_SetArcEsMode(int rate) { assert(rate == format.rate); }
void HAL_SetArcPcmMode(void) { }
void HAL_SetSpdifEsMode(int rate) { assert(rate == format.rate); }
static void start(struct out_module *obj) { assert(obj == &m); ++starts; }
static void stop(struct out_module *obj) { assert(obj == &m); ++stops; }
static void mute(struct out_module *obj, int on)
{ assert(obj == &m); if (on) ++mute_on; else ++mute_off; }

static void reset(void)
{
    memset(&m, 0, sizeof(m)); memset(&cfg, 0, sizeof(cfg));
    memset(desc, 0, sizeof(desc)); memset(ring, 0xa5, sizeof(ring));
    memset(aud_regs, 0, sizeof(aud_regs)); memset(aoc_regs, 0, sizeof(aoc_regs));
    puts_count = pushes = waits = completes = cache_calls = starts = stops = 0;
    mute_on = mute_off = enables = disables = status_events = last_events = 0;
    defaults = remaps = irq_depth = zero_allocs = cache_bytes = 0;
    wait_action = NULL;
    cfg.name = "arc"; cfg.irq = 16; cfg.start = start; cfg.stop = stop; cfg.mute = mute;
    cfg.bc_desc = desc; cfg.bc_virt = ring; cfg.bc_phys = 0x100000;
    cfg.bc_size = 65536; cfg.bc_unit = 256; cfg.stride_ch = 2;
    desc[0] = cfg.bc_phys; desc[1] = cfg.bc_phys + cfg.bc_size;
    desc[2] = cfg.bc_phys; desc[5] = (uint32_t)cfg.bc_unit;
    m.name = "output_arc"; m.cfg = &cfg; m.stride_ch = 2;
    m.channels = 2; m.bits_per_sample = 32; m.data_rate = 48000;
    m.stride_shift = m.output_stride_shift = 3; m.bc_writer = cpy_00;
    m.queue.next = m.queue.prev = &m.queue; m.max_len = 1024;
    port = (struct aud_port){ .owner = &m };
    format.codec = "pcm"; format.channels = 2; format.rate = 48000;
    format.bps = 32; format.acmod = 2; format.es = format.mapping = false;
}
static uint32_t word(unsigned lane_index, unsigned byte_offset)
{
    uint32_t v;
    memcpy(&v, ring + lane_index * cfg.bc_size + byte_offset, sizeof(v));
    return v;
}
static uint32_t slot(uint16_t v)
{ return ((uint32_t)(v & 255) << 16) | (v & 0xff00); }
static void writers(void)
{
    reset(); cfg.bc_size = 128;
    uint16_t es[16]; uint32_t pcm[16];
    for (unsigned i = 0; i < 16; ++i) { es[i] = (uint16_t)(0x1200 + i); pcm[i] = 100 + i; }
    assert(cpy_es(&m, ring, es, 9) == 4);
    assert(word(0, 0) == slot(es[0]) && word(0, 4) == slot(es[1]));
    assert(cpy_es8(&m, ring, es, 16) == 32);
    for (unsigned k = 0; k < 4; ++k)
        for (unsigned f = 0; f < 2; ++f)
            for (unsigned j = 0; j < 2; ++j)
                assert(word(k, 8 * f + 4 * j) == slot(es[8 * f + 2 * k + j]));
    assert(cpy_es8(&m, ring, es, 7) == 0);
    assert(cpy_8(&m, ring, pcm, 16) == 64);
    for (unsigned k = 0; k < 4; ++k) assert(word(k, 8) == pcm[8 + 2 * k]);
    assert(cpy_8_swap_c_lfe(&m, ring, pcm, 8) == 32);
    assert(word(1, 0) == pcm[3] && word(1, 4) == pcm[2]);
    const out_bc_writer fns[] = {cpy_24, cpy_28, cpy_42, cpy_48, cpy_82, cpy_84};
    const unsigned ins[] = {2,2,4,4,8,8}, outs[] = {4,8,2,8,2,4};
    for (unsigned i = 0; i < 6; ++i) {
        assert(fns[i](&m, ring, pcm, outs[i] * 4) == (int)(ins[i] * 4));
        for (unsigned j = 0; j < outs[i]; ++j)
            assert(word(0, j * 4) == (j < ins[i] ? pcm[j] : 0));
    }
}
static int no_progress(struct out_module *obj, void *dst, const void *src, unsigned n)
{ (void)obj; (void)dst; (void)src; (void)n; return 0; }
static void fill_and_wrap(void)
{
    reset(); cfg.bc_size = 128; m.channels = 8; m.bc_writer = cpy_es8; m.bc_wr = 120;
    uint16_t es[16]; for (unsigned i = 0; i < 16; ++i) es[i] = (uint16_t)(i + 1);
    assert(fill_bc(&m, (const uint8_t *)es, 16) == 32);
    assert(m.bc_wr == 8 && m.bc_filled == 16 && cache_calls == 8 && cache_bytes == 64);
    for (unsigned k = 0; k < 4; ++k) {
        assert(word(k, 120) == slot(es[2 * k]));
        assert(word(k, 0) == slot(es[8 + 2 * k]));
        assert(desc[8 * k + 3] == cfg.bc_phys + k * 128 + 8);
    }
    m.bc_wr = 120; assert(fill_bc(&m, NULL, 16) == 0);
    assert(m.bc_filled == 32 && m.bc_wr == 8);
    for (unsigned k = 0; k < 4; ++k) assert(word(k, 120) == 0 && word(k, 0) == 0);
    m.bc_writer = no_progress; unsigned wr = m.bc_wr; int filled = m.bc_filled;
    assert(fill_bc(&m, (const uint8_t *)es, 8) == 0);
    assert(m.bc_wr == wr && m.bc_filled == filled);
    m.channels = 2; memset(ring, 0xa5, sizeof(ring));
    assert(fill_bc(&m, NULL, 8) == 0 && word(0, wr) == 0 && word(1, wr) == 0xa5a5a5a5);
}
static void first_payload_restart(void)
{
    reset(); struct aud_event first = { AUD_EVENT_FIRST };
    format.codec = "mat"; format.channels = 8; format.bps = 16;
    format.rate = 768000; format.es = true;
    assert(out_event(&m, &port, &first) == 23);
    assert(starts == 1 && stops == 1 && status_events == 1 && defaults == 1);
    assert(m.bc_writer == cpy_es8 && m.stride_shift == 4 && m.output_stride_shift == 3);
    assert(m.bc_init_fill == 32768 && m.bc_need_fill && m.first_data && m.mute_countdown == 16);
    uint16_t es[16]; for (unsigned i = 0; i < 16; ++i) es[i] = (uint16_t)(0x5100 + i);
    struct aud_data d = { .type = AUD_DATA_ES, .ts = UINT32_MAX, .size = sizeof(es), .ptr = (uint8_t *)es };
    aoc_regs[0x84 / 4] = 1000;
    assert(out_chain(&port, &d) == 0 && pushes == 1 && !puts_count);
    assert(m.bc_wr == 32784 && m.bc_filled == 32784 && !m.bc_need_fill);
    assert(word(3, 32768) == slot(es[6]) && word(3, 32776) == slot(es[14]));
    assert(m.first_data && m.write_time == 1000);
    assert(m.first_write_time == 1000 + 4096 * 90000u / 768000u + 32 * 90000u / 768000u);
    desc[2] = cfg.bc_phys + 256; aoc_regs[0x84 / 4] = 1100;
    irq_handle_bc(&m);
    assert(!m.first_data && m.first_out_time == m.first_write_time + 100);
    assert(m.bc_filled == 32528 && m.irq_count == 1 && mute_on == 1 && m.mute_countdown == 15);
    unsigned old_starts = starts;
    assert(out_event(&m, &port, &first) == 23 && starts == old_starts && status_events == 1);
    format.codec = "pcm"; format.es = false; format.channels = 2;
    format.bps = 32; format.rate = 48000;
    assert(out_event(&m, &port, &first) == 23 && starts == old_starts + 1);
    assert(m.bc_writer == cpy_00 && m.bc_filled == 0 && m.bc_wr == 256 && m.bc_need_fill);
    uint32_t pcm[2] = { 0x12345678, 0x76543210 };
    d = (struct aud_data){ .ts = UINT32_MAX, .size = sizeof(pcm), .ptr = (uint8_t *)pcm };
    out_chain(&port, &d);
    assert(m.bc_wr == 2312 && m.bc_filled == 2056 && word(0, 2304) == pcm[0]);
    assert(word(0, 32768) == slot(es[0])); /* stale bytes remain, outside new committed range */
    struct aud_event last = { AUD_EVENT_LAST };
    assert(out_event(&m, &port, &last) == 23 && !m.started && !m.irq_active && last_events == 1);
}
static void advance_room(void) { m.bc_rd += 256; m.bc_filled -= 256; }
static void disable_port(void) { port.flags |= PORT_DISABLED; }
static void underflow_wait(void) { m.bc_need_fill = true; }
static void waits_and_irq(void)
{
    reset(); uint32_t pcm[4] = { 11,22,33,44 };
    struct aud_data d = { .ts = UINT32_MAX, .size = sizeof(pcm), .ptr = (uint8_t *)pcm };
    m.bc_init_fill = 0; m.bc_filled = 512; m.bc_rd = 512; wait_action = advance_room;
    out_chain(&port, &d); assert(waits == 1 && pushes == 1 && m.bc_filled == 272);
    reset(); m.bc_wr = 512; m.bc_rd = 768; m.bc_filled = 256; wait_action = advance_room;
    out_chain(&port, &d); assert(waits == 1 && m.bc_wr == 528);
    reset(); m.bc_filled = 512; wait_action = disable_port;
    out_chain(&port, &d); assert(waits == 1 && pushes == 1 && m.bc_wr == 0);
    reset(); m.bc_filled = 512; wait_action = underflow_wait;
    out_chain(&port, &d); assert(waits == 1 && pushes == 1 && m.bc_need_fill && !m.bc_wr);
    reset(); m.bc_rd = 264; wait_action = disable_port;
    d.size = 9;
    out_chain(&port, &d);
    assert(m.bc_wr == 8 && m.bc_filled == 8 && waits == 1 && pushes == 1);
    /* Equal frame counts use room=8, not all nine source bytes (60123130 bgeu). */
    d.size = sizeof(pcm);
    reset(); m.bc_writer = no_progress;
    out_chain(&port, &d); assert(pushes == 1 && !m.bc_wr && !irq_depth);
    reset(); m.channels = 8; m.bc_writer = cpy_es8; m.bc_hw_rd = 65528;
    m.bc_wr = 0; m.bc_filled = 0; m.latency_us = 123; m.mute_countdown = 16;
    desc[2] = cfg.bc_phys + 8;
    irq_handle_bc(&m);
    assert(m.bc_rd == 8 && m.bc_wr == 264 && m.bc_filled == 256);
    assert(m.bc_need_fill && m.underflow_count == 1 && !m.latency_us && completes == 1 && mute_on == 1);
    for (unsigned k = 0; k < 4; ++k) assert(word(k, 8) == 0);
    for (unsigned i = 0; i < 15; ++i) irq_handle_bc(&m);
    assert(m.mute_countdown == 0 && mute_off == 1 && completes == 16);
    reset(); m.bc_filled = 256; m.bc_wr = 256; desc[2] = cfg.bc_phys + 256;
    irq_handle_bc(&m); assert(m.bc_wr == 512 && m.bc_filled == 256 && m.irq_count == 1);
    m.bc_init_fill = 512;
    out_chain(&port, &d);
    assert(!m.bc_need_fill && m.bc_wr == 784 && m.bc_filled == 528);
    assert(word(0, 256) == 0 && word(0, 760) == 0 && word(0, 768) == pcm[0]);
    reset(); m.bc_filled = 1024; m.bc_wr = 1024; desc[2] = cfg.bc_phys + 768;
    irq_handle_bc(&m); assert(m.bc_filled == 256 && m.irq_count == 3 && !m.bc_need_fill);
}
static void first_variants(void)
{
    reset(); struct aud_event first = { AUD_EVENT_FIRST };
    format.codec = "ac3"; format.es = true; format.bps = 16;
    assert(out_event(&m, &port, &first) == 23 && m.bc_writer == cpy_es && m.stride_shift == 2);
    uint16_t es[4] = {0xf872, 0x4e1f, 0x0001, 0x0020};
    struct aud_data d = { .type = AUD_DATA_ES, .ts = UINT32_MAX, .size = sizeof(es), .ptr = (uint8_t *)es };
    m.irq_time = 100; aoc_regs[0x84 / 4] = 120;
    out_chain(&port, &d);
    assert(m.bc_filled == 2064 && m.bc_wr == 2064 && !m.first_data && !m.first_write_time);
    assert(m.first_out_time == 100 + 256 * 90000u / 48000u + 32 * 180000u / 48000u);
    for (unsigned i = 0; i < 4; ++i) assert(word(0, 2048 + i * 4) == slot(es[i]));
    unsigned saved_starts = starts;
    format.bps = 32;
    out_event(&m, &port, &first);
    assert(starts == saved_starts && m.stride_shift == 3); /* bps alone is not a restart */
    reset(); format.channels = 8; format.mapping = false;
    out_event(&m, &port, &first);
    uint32_t pcm[8] = {10,20,30,40,50,60,70,80};
    d = (struct aud_data){ .ts = UINT32_MAX, .size = sizeof(pcm), .ptr = (uint8_t *)pcm };
    out_chain(&port, &d);
    assert(m.bc_writer == cpy_8_swap_c_lfe && m.bc_filled == 2056);
    assert(word(1, 2048) == 40 && word(1, 2052) == 30 && word(3, 2052) == 80);
    saved_starts = starts; format.mapping = true;
    out_event(&m, &port, &first);
    assert(starts == saved_starts && m.bc_writer == cpy_8);
    struct aud_event custom = { AUD_EVENT_CUSTOM };
    m.irq_time = m.write_time = m.first_write_time = 99;
    out_event(&m, &port, &custom);
    assert(m.first_data && !m.irq_time && !m.write_time && !m.first_write_time && puts_count == 1);
    reset(); desc[2] = cfg.bc_phys - 4;
    out_start(&m);
    assert(m.bc_rd == 0 && m.bc_hw_rd == 0 && desc[3] == cfg.bc_phys - 4);
    assert(m.bc_wr == UINT32_MAX - 3); /* firmware clamps SW rd, not raw hw wr */
}
static void fifo_retention_and_mapping(void)
{
    reset(); cfg.bc_size = desc[0] = 0; m.started = 1;
    uint32_t samples[4] = {0};
    struct aud_data d = { .size = sizeof(samples), .ptr = (uint8_t *)samples };
    assert(out_chain(&port, &d) == 0 && m.queued == 2);
    struct out_qnode *q = m.queue.prev;
    struct aud_event first = { AUD_EVENT_FIRST };
    format.channels = 8;
    assert(out_event(&m, &port, &first) == 23);
    assert(m.queue.prev == q && m.queued == 2 && q->channels == 2 && q->shift == 3);
    assert(m.bc_writer == cpy_8_swap_c_lfe && remaps == 1 && zero_allocs == 1);
    out_stop(&m); assert(m.queue.prev == q && m.queued == 2);
    free(q); m.queue.next = m.queue.prev = &m.queue;
    reset(); m.name = "output_speaker"; m.stride_ch = 6;
    assert(out_event(&m, &port, &first) == 23 && m.bc_writer == cpy_00);
    /* No 6-slot entry: old bany inference incorrectly selected 2->2. */
    reset(); struct aud_data meta = { .type = AUD_DATA_META };
    assert(out_chain(&port, &meta) == 0 && puts_count == 1 && !pushes);
    meta.type = AUD_DATA_META_EXT; out_chain(&port, &meta); assert(puts_count == 2);
    cfg.bc_size = 0; assert(out_chain(&port, &d) == -1 && puts_count == 3);
}
int main(void)
{
    writers(); fill_and_wrap(); first_payload_restart(); first_variants(); waits_and_irq(); fifo_retention_and_mapping();
    puts("PASS DSP1 FIRST/payload, ES/HBR/PCM writers, BC wrap/priming/waits/IRQ/mute/restart and FIFO retention");
    return 0;
}
