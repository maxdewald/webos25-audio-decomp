/*
 * dsp1 "output" module core (instances output_hp / output_speaker / output_arc /
 * output_spdif): FIFO (non-BC) and buffer-controller (BC) data paths, IRQ handlers,
 * start/stop, per-output start/stop/mute, soundbar channel status, out_cmd, ctor/dtor
 * and _init. out_event and the lane writers live in output_arc.c.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp1.bin
 *   sha256 8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4
 * Main segment linked at 0x60000000. BC fill/IRQ/start/stop/chain control flow was
 * rechecked with tools/xtobjdump.sh on the saved main image; validated .w18 branch
 * slots supersede flix_annot.py guesses. Unchecked branches elsewhere remain marked
 * [FLIX]; SIMD/float slots are not recovered. Log guards are omitted; short log
 * strings are quoted in comments.
 * Evidence: build/decomp/audio_a0_dsp1.main_60000000.bin.c (0x60121754..0x60125208),
 *           build/scratch/evidence/dsp1-out-event/, build/scratch/evidence/earc-dsp1/.
 * Names: firmware symbol table at 0x602bfb08. functions.tsv calls 0x60121a0c "out_stop"
 * (it is write_8ch), 0x601229ac "output_status" (cpy_es) and 0x60122a38 "output_spdif"
 * (cpy_es8). 0x60123634 is listed as "out_chai" (out_chain).
 */
#include <stdlib.h>
#include <string.h>
#include "out_module.h"

extern char aud_object_class_base_output[0x60];    /* class object at table - 0x60 */
extern struct out_cfg out_cfg_table[4];           /* @0x60280d60: hp, speaker, arc, spdif */

/* ceil(log2(x)) for x >= 1, 0 for x <= 1. Inlined into out_event and out_constructor. */
static inline int ceil_log2(int x)
{
    int n = 0;
    if (x > 0)
        x -= 1;
    while (x) {
        n++;
        x >>= 1;
    }
    return n;
}

/* ====================================================================================
 * FIFO (non-BC) writers. Called from the IRQ with pos = FIFO word index, n frames.
 * Source is 32-bit PCM (MSB-aligned) and the FIFO takes 24 bits, hence >> 8.
 * ================================================================================== */

static inline const int32_t *qsrc(struct out_qnode *q)
{
    return (const int32_t *)(q->data->ptr + q->offset);
}

static inline void fifo_put(struct out_module *m, int lane, int pos, uint32_t v)
{
    ((volatile uint32_t *)(uintptr_t)m->cfg->fifo[lane])[pos] = v;
}

/* @0x60121754 */
int write_2ch(struct out_module *m, int pos, int n, struct out_qnode *q)
{
    for (int i = 0; i < n; i++) {
        const int32_t *s = qsrc(q);
        fifo_put(m, 0, pos + i, (uint32_t)(s[0] >> 8));
        fifo_put(m, 1, pos + i, (uint32_t)(s[1] >> 8));
        q->offset += 1u << q->shift;
    }
    return pos + n;
}

/* @0x601217a8: 2 x 16-bit ES words per frame, byte-swapped, into FIFO lanes 0/1.
 * ((w & 0xff) << 16 | (w & 0xff00)) >> 8 == bswap16(w). */
int write_es(struct out_module *m, int pos, int n, struct out_qnode *q)
{
    for (int i = 0; i < n; i++) {
        const uint16_t *s = (const uint16_t *)(q->data->ptr + q->offset);
        fifo_put(m, 0, pos + i, ((uint32_t)(s[0] & 0xff) << 16 | (s[0] & 0xff00)) >> 8);
        fifo_put(m, 1, pos + i, ((uint32_t)(s[1] & 0xff) << 16 | (s[1] & 0xff00)) >> 8);
        q->offset += 1u << q->shift;
    }
    return pos + n;
}

/* @0x6012193c: 4 FIFO lanes. A 2-channel source is duplicated onto lanes 2/3. */
int write_4ch(struct out_module *m, int pos, int n, struct out_qnode *q)
{
    bool wide = q->channels == 4 || q->channels == 8;
    for (int i = 0; i < n; i++) {
        const int32_t *s = qsrc(q);
        fifo_put(m, 0, pos + i, (uint32_t)(s[0] >> 8));
        fifo_put(m, 1, pos + i, (uint32_t)(s[1] >> 8));
        fifo_put(m, 2, pos + i, (uint32_t)(s[wide ? 2 : 0] >> 8));
        fifo_put(m, 3, pos + i, (uint32_t)(s[wide ? 3 : 1] >> 8));
        q->offset += 1u << q->shift;
    }
    return pos + n;
}

/* @0x60121a0c write_8ch (functions.tsv: "out_stop"): 8 lanes; a 4- or 2-channel source
 * is repeated over the lanes. */
int write_8ch(struct out_module *m, int pos, int n, struct out_qnode *q)
{
    int src_ch = q->channels == 8 ? 8 : q->channels == 4 ? 4 : 2;  /* [FLIX] beqi 8 / 4 */
    for (int i = 0; i < n; i++) {
        const int32_t *s = qsrc(q);
        for (int lane = 0; lane < 8; lane++)
            fifo_put(m, lane, pos + i, (uint32_t)(s[lane % src_ch] >> 8));
        q->offset += 1u << q->shift;
    }
    return pos + n;
}

/* ====================================================================================
 * FIFO state machine: init -> wait (until wait_len frames queued) -> running.
 * ================================================================================== */

int process_data_wait(struct out_module *m);

/* @0x60121bcc */
int process_data_running(struct out_module *m)
{
    struct out_qnode *head = &m->queue;
    struct out_qnode *q = head->prev;                 /* oldest */
    int pos = 0;

    if (q == head) {
        /* "(ignored:%3d) %8s: underrun... queued %d" */
    } else {
        while (q && pos < m->cfg->fifo_words) {
            unsigned left = (q->data->size - q->offset) >> q->shift;
            int n = m->cfg->fifo_words - pos;
            if ((unsigned)n > left)
                n = (int)left;
            pos = m->fifo_writer(m, pos, n, q);
            if (q->offset < q->data->size)
                continue;
            if (q->next) {                            /* unlink */
                q->next->prev = q->prev;
                q->prev->next = q->next;
            }
            aud_object_put(q->data);
            free(q);
            q = head->prev;
            if (q == head) {
                /* "(ignored:%3d) %8s: underrun... queued %d" */
                break;
            }
        }
    }
    if (m->low_len != m->queued)                      /* [FLIX] 0x60121c0a bne */
        return pos;
    /* "%8s: to waiting.." */
    m->state = process_data_wait;
    return pos;
}

/* @0x60121d40 */
int process_data_wait(struct out_module *m)
{
    /* "%8s: wait.. %d" */
    if (m->queued < m->wait_len)
        return 0;
    /* "%8s: to running.." */
    m->state = process_data_running;
    return process_data_running(m);
}

/* @0x60121db0: zero lanes 0/1, unmute, then wait for data. */
int process_data_init(struct out_module *m)
{
    /* "%8s: init.." */
    for (int i = 0; i < m->cfg->fifo_words; i++) {
        fifo_put(m, 0, i, 0);
        fifo_put(m, 1, i, 0);
    }
    m->cfg->mute(m, 0);
    m->state = process_data_wait;
    return 0;
}

/* @0x60121e18: FIFO IRQ. Whatever the state function did not fill is zeroed, so an
 * underrun plays silence. */
void irq_handle(struct out_module *m)
{
    HAL_ClearInterrupt((unsigned)m->cfg->irq);
    m->irq_clock = REG(AOC_REGS, 0x84);
    m->irq_count++;
    int pos = m->state(m);
    for (int i = pos; i < m->cfg->fifo_words; i++) {
        fifo_put(m, 0, i, 0);
        fifo_put(m, 1, i, 0);
    }
    m->queued -= pos;
    if (m->queued <= m->max_len)
        aud_complete(&m->done);
}

/* ====================================================================================
 * Buffer-controller (BC) path. Each lane is a ring of cfg->bc_size bytes; lane k
 * starts at bc_virt + k * bc_size and has its own descriptor at bc_desc + 0x20 * k.
 * ================================================================================== */

/* @0x60121edc: write `bytes` PER LANE at bc_wr; return SOURCE bytes consumed.
 * xtobjdump 0x60121efd beqi.w18 8 selects four zero/writeback/descriptor lanes;
 * 0x60121f76/1fd3 commits silence even though its return is zero, but does not
 * commit a source writer returning zero. Wrapped source resumes at src + used.
 * Thus cpy_es8 consumes twice the lane-byte count (cpy_8 consumes four times).
 * cpy_es8 with channels != 8 still writes four lanes but commits only lane 0. */
int fill_bc(struct out_module *m, const uint8_t *src, unsigned bytes)
{
    struct out_cfg *cfg = m->cfg;
    unsigned room = cfg->bc_size - m->bc_wr;
    unsigned first = bytes, wrap = 0;
    int used = 0;
    int lanes = m->channels == 8 ? 4 : 1;

    if (room < bytes) {
        first = room;
        wrap = bytes - room;
    }

    if (src)
        used = m->bc_writer(m, cfg->bc_virt + m->bc_wr, src, first);
    else
        for (int k = 0; k < lanes; k++)
            memset(cfg->bc_virt + m->bc_wr + k * cfg->bc_size, 0, first);
    for (int k = 0; k < lanes; k++)
        HAL_CacheRegionWriteback(cfg->bc_virt + m->bc_wr + k * cfg->bc_size, first);

    if (wrap) {
        if (src)
            used += m->bc_writer(m, cfg->bc_virt, src + used, wrap);
        else
            for (int k = 0; k < lanes; k++)
                memset(cfg->bc_virt + k * cfg->bc_size, 0, wrap);
        for (int k = 0; k < lanes; k++)
            HAL_CacheRegionWriteback(cfg->bc_virt + k * cfg->bc_size, wrap);
    }

    if (used == 0 && src)
        return 0;
    m->bc_filled += (int)bytes;
    m->bc_wr = (bytes + m->bc_wr) & (cfg->bc_size - 1);
    for (int k = 0; k < lanes; k++)
        cfg->bc_desc[(0x20 * k + 0xc) / 4] = cfg->bc_phys + k * cfg->bc_size + m->bc_wr;
    return used;
}

/* @0x60122098: BC IRQ tracks lane-0 read delta modulo bc_size; larger deltas
 * count whole units, smaller ones do not increment irq_count (0x6012219f..22224).
 * 0x601221c1 blti.w18 1 includes exactly empty; only a NEGATIVE fill resets wr=rd
 * (0x60122403..11). Add one silent unit, flag re-prime, clear latency, wake writer.
 * 0x601221d1 beqi.w18 16 mutes before decrement; zero unmutes. eARC mute is a no-op.
 * Timing carry/borrow instructions in this region are not fully decoded by the
 * objdump profile; the 64-bit expressions retain the existing reconstruction. */
void irq_handle_bc(struct out_module *m)
{
    struct out_cfg *cfg = m->cfg;
    HAL_ClearInterrupt((unsigned)cfg->irq);

    uint32_t lo = REG(AOC_REGS, 0x84);
    uint32_t hi = HAL_GetCurrentStch();               /* inlined; chip quirk -> 0 */
    m->irq_time = (uint64_t)hi << 32 | lo;

    if (m->first_data && m->first_write_time) {
        m->first_data = false;
        m->first_out_time = m->first_write_time + (m->irq_time - m->write_time);
        /* "%8s: bc_write_time : %llu, %llu, %llu, %llu." */
    }

    switch (m->clock_source) {                        /* CLOCK_CMD_SET_CLOCK_SOURCE */
    case 1:  m->irq_clock = REG(AUD_REGS, 0x220); break;
    case 2:  m->irq_clock = REG(AUD_REGS, 0x224); break;
    case 7:  m->irq_clock = REG(AOC_REGS, 0x8c);  break;
    default: m->irq_clock = REG(AOC_REGS, 0x84);  break;   /* 6 after construct */
    }

    uint32_t rd = cfg->bc_desc[2] - cfg->bc_phys;
    int consumed = (int)(rd - m->bc_hw_rd);
    m->bc_hw_rd = rd;
    m->bc_rd = rd;
    if (consumed < 0)
        consumed += (int)cfg->bc_size;
    /* "bc %08x, rd %08x, wr %08x, size %08x, proceeded_size %d, bc_filled %d" */
    if (consumed > cfg->bc_unit)
        m->irq_count += (uint32_t)(consumed / cfg->bc_unit);  /* "...bigger than unit-size" */
    else if (consumed == cfg->bc_unit)
        m->irq_count++;                                       /* "...equals unit-size" */
    /* else "process size is less than unit-size" */

    m->bc_filled -= consumed;
    if (m->bc_filled < 1) {                           /* 0x601221c1 blti.w18 1 */
        /* "underflow. fill zero" */
        if (m->bc_filled < 0) {
            /* "overrun. rd %08x, wr %08x, bc_filled %d": hardware read past wr */
            m->bc_filled = 0;
            m->bc_wr = m->bc_rd;
        }
        fill_bc(m, NULL, (unsigned)cfg->bc_unit);
        m->bc_need_fill = true;                       /* out_chain_bc re-primes */
        m->underflow_count++;
        m->latency_us = 0;
    }

    if (m->mute_countdown) {
        if (m->mute_countdown == 16)                  /* 0x601221d1 beqi.w18 16 */
            cfg->mute(m, 1);                          /* "mute." */
        if (--m->mute_countdown == 0)
            cfg->mute(m, 0);                          /* "unmute." */
    }
    aud_complete(&m->done);
}

/* ====================================================================================
 * Per-output hardware start/stop/mute (cfg +0x2c/+0x30/+0x34).
 * ================================================================================== */

/* @0x60122438 */
void mute_output_hp(struct out_module *m, int mute)
{
    (void)m;
    REG(AUD_REGS, 0x398) = (mute == 0);
}

/* @0x60122450 */
void start_hp(struct out_module *m)
{
    (void)m;
    REG(AUD_REGS, 0x398) = 0;
    REG(AUD_REGS, 0x380) = 1;
}

/* @0x60122468 */
void stop_hp(struct out_module *m)
{
    (void)m;
    REG(AUD_REGS, 0x398) = 0;
    REG(AUD_REGS, 0x380) = 0;
}

/* @0x60122480 mute_output_speake (sic) */
void mute_output_speake(struct out_module *m, int mute)
{
    (void)m;
    REG(AUD_REGS, 0x318) = (mute == 0);
}

/* @0x60122498 */
void start_speaker(struct out_module *m)
{
    (void)m;
    REG(AUD_REGS, 0x318) = 0;
    REG(AUD_REGS, 0x300) = 1;
}

/* @0x601224b0 stop_speake (sic) */
void stop_speake(struct out_module *m)
{
    (void)m;
    REG(AUD_REGS, 0x318) = 0;
    REG(AUD_REGS, 0x300) = 0;
}

/* @0x601224c8: empty. dsp1 never mutes eARC in hardware; the post-start mute
 * countdown and process_data_init's unmute do nothing for output_arc. */
void mute_output_arc(struct out_module *m, int mute)
{
    (void)m;
    (void)mute;
}

/* @0x601224d0: eARC (port 5) output + ctrl enable, then ARC/eARC ctrl = 4. */
void start_arc(struct out_module *m)
{
    (void)m;
    HAL_EnableArcOut(1, 5);
    HAL_EnableArcCtrl(1, 5);
    HAL_SetArcCtrl(4);
    HAL_SetEarcCtrl(4);
}

/* @0x601224f4 */
void stop_arc(struct out_module *m)
{
    (void)m;
    HAL_EnableArcCtrl(0, 5);
    HAL_EnableArcOut(0, 5);
}

/* @0x60122510 */
void mute_output_spdif(struct out_module *m, int mute)
{
    (void)m;
    HAL_EnableSpdifCtrl(mute == 0);
}

/* @0x60122524 */
void start_spdif(struct out_module *m)
{
    (void)m;
    REG(AUD_REGS, 0x320) = 1;
}

/* @0x60122534 */
void stop_spdif(struct out_module *m)
{
    (void)m;
    REG(AUD_REGS, 0x320) = 0;
}

/* ====================================================================================
 * Soundbar (S/PDIF user channel-status) helpers.
 * Layout: id = 0x334[12..23] | 0x338[0..11] << 12, volume = 0x338[12..18],
 * mute = 0x338[19], misc nibbles in 0x33c, XOR checksum byte in 0x340[4..11].
 * ================================================================================== */

/* Inlined after every soundbar field write (_set_format_soundbar, out_cmd). */
static inline void soundbar_update_checksum(void)
{
    uint32_t a = REG(AUD_REGS, 0x334), b = REG(AUD_REGS, 0x338);
    uint32_t c = REG(AUD_REGS, 0x33c), d = REG(AUD_REGS, 0x340);
    uint32_t x = (a >> 12) ^ ((a >> 20) & 0xf)
               ^ (b >> 4) ^ (b >> 12) ^ ((b >> 20) & 0xf) ^ ((b & 0xf) << 4)
               ^ (c >> 4) ^ (c >> 12) ^ ((c >> 20) & 0xf) ^ ((c & 0xf) << 4)
               ^ ((d & 0xf) << 4);
    REG(AUD_REGS, 0x340) = (d & 0xfff00f) | ((x << 4) & 0xff0);
}

/* @0x60122544: raise "sound_bar_format" with the current 16-byte status. */
void _noti_info_soundbar(struct out_module *m)
{
    uint32_t st[4];
    st[0] = (REG(AUD_REGS, 0x334) >> 12 & 0xfff) | (REG(AUD_REGS, 0x338) << 12 & 0xfff000);
    st[1] = (REG(AUD_REGS, 0x338) >> 12 & 0xfff) | (REG(AUD_REGS, 0x33c) & 0xf) << 12;
    st[2] = (REG(AUD_REGS, 0x33c) >> 4 & 0xfffff) | (REG(AUD_REGS, 0x340) << 20 & 0xf00000);
    st[3] = REG(AUD_REGS, 0x340) >> 4 & 0xff;
    aimc_raise_named_event(m->aimc_ctx, m->aimc_id, "sound_bar_format", st, sizeof(st));
}

/* @0x601225d0: p = {id, volume, mute} (SpdifCmdSetFmtForSoundbar). */
int _set_format_soundbar(struct out_module *m, const uint32_t *p)
{
    REG(AUD_REGS, 0x334) = (p[0] & 0xfff) << 12 | (REG(AUD_REGS, 0x334) & 0xfff);
    REG(AUD_REGS, 0x338) = (p[0] >> 12 & 0xfff) | (REG(AUD_REGS, 0x338) & 0xfff000);
    REG(AUD_REGS, 0x338) = (p[1] & 0x7f) << 12 | (REG(AUD_REGS, 0x338) & 0xf80fff);
    REG(AUD_REGS, 0x338) = (p[2] & 1) << 19 | (REG(AUD_REGS, 0x338) & 0xf7ffff);
    soundbar_update_checksum();
    _noti_info_soundbar(m);
    /* "%8s: soundbar format id %d vol %d mute %d" */
    return 0;
}

/* ====================================================================================
 * start / stop
 * ================================================================================== */

void irq_handle_bc(struct out_module *m);

/* @0x601226f8: xtobjdump 0x60122721 bbci.w18 0 selects MAOC setup, not
 * a stride change. BC reads base/end/unit and sets hardware wr to raw hw rd;
 * the range check clamps only software rd (0x6012276b..7aa). Restart discards
 * previously committed BC data, but does NOT flush the software FIFO queue.
 * Priming is deferred to the next payload: unit * floor((0x800/unit)*rate/48000),
 * with stride_ch/2 scaling for speaker (0x601228c5..db). */
int out_start(struct out_module *m)
{
    struct out_cfg *cfg = m->cfg;
    /* "%8s: start.." */
    m->started = 1;
    if (cfg->flags & 1)                               /* hp, spdif: 2-slot FIFO */
        m->stride_ch = 2;
    else                                              /* speaker, arc: 0x60122721 bbci.w18 0 */
        HAL_SetMAOCChannelSet_BufferController(4);

    if (cfg->bc_desc[0] == 0) {
        /* FIFO path */
        m->state = process_data_init;
        if (m->stride_ch == 4)
            m->fifo_writer = write_4ch;
        else if (m->stride_ch == 8)
            m->fifo_writer = write_8ch;
        else
            m->fifo_writer = m->es ? write_es : write_2ch;
        irq_register(cfg->irq, cfg->name, irq_handle, m);
    } else {
        /* BC path (output_arc: descriptor 0xc1000420) */
        volatile uint32_t *d = cfg->bc_desc;
        cfg->bc_phys = d[0];
        cfg->bc_virt = AddrTrans_GetAddr(cfg->bc_phys);
        cfg->bc_size = d[1] - cfg->bc_phys;
        cfg->bc_unit = (int)d[5];
        if (cfg->bc_size > 0x100000) {
            /* "%8s: too big bc size. 0x%08x, 0x%08x" */
        }
        uint32_t rd = d[2];
        if (rd < cfg->bc_phys || rd >= cfg->bc_phys + cfg->bc_size) {
            /* "%8s: wrong bc read offset. 0x%08x" */
            rd = cfg->bc_phys;
        }
        m->bc_hw_rd = m->bc_rd = rd - cfg->bc_phys;
        d[3] = d[2];                                  /* hw wr = hw rd: empty ring */
        m->bc_filled = 0;
        m->bc_wr = d[3] - cfg->bc_phys;
        m->bc_need_fill = true;

        /* Zero pre-fill: 0x800 bytes scaled by data rate (x stride/2 for speaker). */
        int periods = 0x800 / cfg->bc_unit;
        if (strncmp(m->name, "output_speaker", 14) == 0)
            periods *= m->stride_ch / 2;
        periods = (int)((unsigned)(periods * m->data_rate) / 48000u);
        m->bc_init_fill = cfg->bc_unit * periods;
        /* "%8s: use BC. 0x%08x - 0x%08x(unit 0x%08x), rd %08x, wr %08x, filled %d" */
        irq_register(cfg->irq, cfg->name, irq_handle_bc, m);
    }

    m->irq_active = true;
    HAL_ClearInterrupt((unsigned)cfg->irq);
    xos_interrupt_enable(cfg->irq);
    m->irq_time = 0;
    m->write_time = 0;
    m->first_write_time = 0;
    m->first_data = true;
    m->mute_countdown = 0x10;
    cfg->start(m);
    return 0;
}

/* @0x60122940: hardware stop -> IRQ disable/clear/unregister -> started=0 ->
 * complete (0x60122951..87). No queue unlink, release, or ring clearing occurs;
 * FIRST format changes therefore preserve old FIFO nodes and their saved strides. */
int out_stop(struct out_module *m)
{
    /* "%8s: stop.." */
    m->cfg->stop(m);
    xos_interrupt_disable(m->cfg->irq);
    HAL_ClearInterrupt((unsigned)m->cfg->irq);
    m->irq_active = false;
    irq_register(m->cfg->irq, NULL, NULL, NULL);
    m->started = 0;
    aud_complete(&m->done);
    return 0;
}

/* ====================================================================================
 * data input
 * ================================================================================== */

/* @0x60122fa4: copy one buffer into the BC ring, waiting for room. Data is always
 * forwarded to output port 0 afterwards (nothing is connected there, so this is the
 * release). Partial writes happen on "port disabled", a new underflow, or a writer
 * returning 0. There is no rate/format or started check. Validated 0x6012310f
 * bge.w18 reserves one unit; 0x60123130 bgeu.w18 chooses all room on frame-count
 * equality too, including a partial source frame. Completion is reinitialized
 * BEFORE disable/underflow/room checks and waits occur with IRQs restored.
 * Priming resets wr=rd and filled=0; it is not appended to stale queued bytes.
 * The strided scalar copy and control flow are checked; timing carry/borrow
 * instructions still have undecoded slots, not covered as exact firmware timing. */
int out_chain_bc(struct out_module *m, struct aud_port *port, struct aud_data *d)
{
    struct out_cfg *cfg = m->cfg;
    /* "data size %08x, rd %08x, wr %08x" */

    if (m->bc_need_fill) {
        m->bc_wr = m->bc_rd;
        m->bc_filled = 0;
        fill_bc(m, NULL, (unsigned)m->bc_init_fill);
        /* "%8s: is_underflow. fill zero. rd %08x, wr %08x, size %d, bc_filled %d" */
        m->bc_need_fill = false;
    }

    if (d->ts != 0xffffffffu) {                       /* 0x60122fe8 beqi.w18 -1 */
        uint32_t now = REG(AOC_REGS, 0x84);           /* 90 kHz */
        uint32_t diff_us = now >= d->ts ? (now - d->ts) * 100 / 9
                                        : ((now - d->ts) * 100 - 100) / 9;
        uint32_t bc_us = m->es ? 0
            : (uint32_t)(m->bc_filled >> m->output_stride_shift) * 1000000u / (uint32_t)m->data_rate;
        uint32_t since_irq_us = 0;
        if (m->irq_time && now >= (uint32_t)m->irq_time)
            since_irq_us = (now - (uint32_t)m->irq_time) * 100 / 9;
        uint32_t aoc_us = (uint32_t)((cfg->bc_unit >> m->output_stride_shift) << 1)
                          * 1000000u / (uint32_t)m->data_rate;
        m->latency_us = diff_us + bc_us + aoc_us - since_irq_us;
        /* "TS: %u, total_latency(us): %u, diff(us): %u, bc(us): %u, aoc(us): %u, ..." */
    }

    unsigned written = 0;
    while (written < d->size) {                       /* 0x601233a5 bltu.w18 */
        aud_reinit_completion(&m->done);
        if (port->flags & PORT_DISABLED) {
            /* "port disabled." */
            break;
        }
        uint32_t ps = irq_save();
        if (m->bc_need_fill) {
            /* "is_underflow total_written %08x, rd %08x, wr %08x, size %08x" */
            irq_restore(ps);
            break;
        }
        int free_sz = (int)(m->bc_rd - m->bc_wr);
        if (free_sz < 1)
            free_sz += (int)cfg->bc_size;
        /* "%8s: free size %08x, rd %08x, wr %08x" */
        if (m->bc_filled >= 2 * cfg->bc_unit + m->bc_init_fill) {
            irq_restore(ps);
            /* "%8s: wait.. bc_filled %d bc_init_filled %d bc_unit_size %d." */
            aud_wait_for_completion(&m->done);
            continue;
        }
        if (cfg->bc_unit >= free_sz) {                /* 0x6012310f bge.w18 */
            irq_restore(ps);
            /* "%8s: wait.. free_size %d bc_unit_size %d." */
            aud_wait_for_completion(&m->done);
            continue;
        }

        /* Destination bytes: min(room - one unit, rest of the buffer rescaled from
         * input frames (stride_shift) to BC frames (output_stride_shift)). */
        unsigned room = (unsigned)(free_sz - cfg->bc_unit);
        unsigned rest = d->size - written;
        unsigned want;
        if ((rest >> m->stride_shift) >= (room >> m->output_stride_shift)) /* 0x60123130 bgeu.w18 */
            want = room;
        else if (m->output_stride_shift >= m->stride_shift)
            want = rest << (m->output_stride_shift - m->stride_shift);
        else
            want = rest >> (m->stride_shift - m->output_stride_shift);

        if (m->first_data) {
            uint64_t now = (uint64_t)HAL_GetCurrentStch() << 32 | REG(AOC_REGS, 0x84);
            uint64_t since = m->irq_time ? now - m->irq_time : 0;
            m->write_time = now;
            uint64_t bc_ticks = (uint64_t)((uint32_t)(m->bc_filled >> m->output_stride_shift)
                                           * 90000u / (uint32_t)m->data_rate);
            uint64_t unit_ticks = (uint64_t)(cfg->bc_unit >> m->output_stride_shift)
                                  * (m->irq_time ? 180000u : 90000u) / (uint32_t)m->data_rate;
            uint64_t t = now + bc_ticks + unit_ticks - since;
            if (m->irq_time) {
                m->first_data = false;   /* 0x60123274..7a; no-IRQ path bypasses this */
                m->first_out_time = t;   /* "%8s: bc_write_time : %llu, ..." */
            } else {
                /* 0x601233c6 -> 0x6012327d, 0x601233d9: one unit, not two.
                 * Keep first_data set so the first IRQ can resolve deferred time. */
                m->first_write_time = t; /* "%8s: bc_write_time_wo_diff : %llu, ..." */
            }
        }

        int used = fill_bc(m, d->ptr + written, want);
        if (used == 0) {
            /* "%8s: total_written %08x, written %08x, write_size %08x, rd, wr, size" */
            irq_restore(ps);
            break;
        }
        written += (unsigned)used;
        irq_restore(ps);
    }
    aud_module_push_data(m, 0, d);
    return 0;
}

/* @0x60123634 out_chai (functions.tsv) = out_chain: input-port chain function.
 * META / META_EXT are always dropped. BC outputs go to out_chain_bc. The FIFO path
 * queues frames; it blocks while queued > max_len and drops (-1) when not started. */
int out_chain(struct aud_port *port, struct aud_data *d)
{
    struct out_module *m = port->owner;

    if (d->type == AUD_DATA_META) {                   /* 0x60123644 beqi.w18 2 */
        /* "%8s: drop metadata." */
        aud_object_put(d);
        return 0;
    }
    if (d->type == AUD_DATA_META_EXT) {               /* 0x6012364c beqi.w18 3 */
        /* "%8s: drop meta ext." */
        aud_object_put(d);
        return 0;
    }
    if (m->cfg->bc_size)
        return out_chain_bc(m, port, d);

    int frames = (int)(d->size >> m->stride_shift);
    aud_reinit_completion(&m->done);
    if (!(port->flags & PORT_DISABLED)) {
        while (m->started && m->queued > m->max_len) {
            /* "%8s: queue full. wait.. (%d/%d)" */
            aud_wait_for_completion(&m->done);
            aud_reinit_completion(&m->done);
            if (port->flags & PORT_DISABLED)
                break;
        }
    }
    if (!m->started) {
        aud_object_put(d);
        return -1;
    }

    struct out_qnode *q = calloc(1, sizeof(*q));
    if (!q) {
        /* "%8s: no mem" */
        aud_object_put(d);
        return -1;
    }
    q->data = d;
    q->shift = (unsigned)m->stride_shift;
    q->channels = m->channels;

    uint32_t ps = irq_save();
    q->next = m->queue.next;                          /* newest at head.next */
    q->prev = &m->queue;
    m->queue.next->prev = q;
    m->queue.next = q;
    m->queued += frames;
    irq_restore(ps);
    /* "queued length %d(%d)" */
    return 0;
}

/* ====================================================================================
 * Commands
 * ================================================================================== */

#define ADEC_CMD_START                    0x10010000u
#define ADEC_CMD_STOP                     0x10020000u
#define SPDIF_CMD_SET_OUTFORMAT           0x5a000008u   /* {format, fs} */
#define SPDIF_CMD_SET_FMT_FOR_SOUNDBAR    0x5a01000cu   /* {id, volume, mute} */
#define SPDIF_CMD_SET_SCMS                0x5a020004u
#define SPDIF_CMD_CLEAR_FMT_FOR_SOUNDBAR  0x5a030000u
#define SPDIF_CMD_SET_LIGHT               0x5a040004u
#define SPDIF_CMD_ONOFF_SOUNDBAR          0x5a050004u
#define SPDIF_CMD_SET_CATEGORY_CODE       0x5a060004u
#define SPDIF_CMD_SET_EQBYPASS_SOUNDBAR   0x5a070004u
#define SPDIF_CMD_SET_WOOFERLEVEL         0x5a080004u
#define SPDIF_CMD_SET_WIRELESSRESET       0x5a090004u
#define SPDIF_CMD_SET_HDMIARC             0x5a0b000cu   /* {port, bypass, enable} */
#define SPDIF_CMD_SET_IEC958VALID         0x5a0c0004u
#define ARC_CMD_SET_OUTFORMAT             0x6000000cu   /* {out_format, Fs, out_port} */
#define ARC_CMD_SET_SCMS                  0x60010004u
#define ARC_CMD_SET_CATEGORY_CODE         0x60020004u
#define ARC_CMD_SET_HDMIARC               0x6004000cu
#define ARC_CMD_SET_IEC958VALID           0x60050004u
#define CLOCK_CMD_GET_CLOCK_INFO          0x6b00000cu
#define CLOCK_CMD_SET_CLOCK_SOURCE        0x6b010004u
#define CLOCK_CMD_GET_PLL_CONTROL_INFO    0x6b020020u   /* name from "_get_pll_control_info" */
#define CLOCK_CMD_GET_BC_WRITE_TIME       0x6b030008u   /* name from "_get_bc_write_time" */

/* SCMS -> channel-status copyright bit 2 / generation bit 15. Used on AUD+0x330 (spdif)
 * and on AUD+0x3c0 plus the 4 eARC lane ctl registers (arc). */
static inline uint32_t scms_bits(uint32_t v, uint32_t scms)
{
    switch (scms) {
    case 0:  return (v | 4) & ~0x8000u;               /* copy free */
    case 2:  return v & ~0x8004u;
    case 1:
    case 3:  return (v & ~4u) | 0x8000u;
    default: return v;
    }
}

/* ARC/S-PDIF HDMI ARC select. The "clear" mask is (1<<p) & (1<<(p+4)) and the value is
 * (bypass<<p) & (enable<<(p+4)), both always 0: the write leaves the register unchanged
 * (observed; looks like '&' was meant to be '|'). */
static inline void set_hdmiarc(const uint32_t *p)
{
    uint32_t port = p[0];
    if (REG(CHIP_ID, 0xe8) > 0x179f && port < 5) {
        uint32_t clr = (1u << port) & (1u << (port + 4));
        uint32_t set = ((p[1] & 1) << port) & ((p[2] & 1) << (port + 4));
        REG(AOC_REGS, 0x24) = (REG(AOC_REGS, 0x24) & ~clr) | set;
    }
    /* "%8s: HDMI ARC : hdmi port = %d, select bypass = %d, enable arc = %d" */
}

/* @0x60123ee4 */
int out_cmd(struct out_module *m, uint32_t cmd, uint32_t *p, int len)
{
    /* "%8s: start received: %p %d %p" */
    switch (cmd) {
    case ADEC_CMD_START:
        return out_start(m);
    case ADEC_CMD_STOP:
        return out_stop(m);

    case SPDIF_CMD_SET_OUTFORMAT:                     /* "_set_outformat_spdif" */
        if (p[0] == 0) {                              /* PCM */
            REG(AUD_REGS, 0x330) &= ~2u;
            REG(AUD_REGS, 0x334) = (REG(AUD_REGS, 0x334) & ~0xfu) | 2;
        } else {
            REG(AUD_REGS, 0x330) |= 2;
            REG(AUD_REGS, 0x334) = (REG(AUD_REGS, 0x334) & 0x0ffffff0u) | iec_rate_code((int)p[1]);
        }
        /* "%8s: outformat :%d fs:%d." */
        return 0;
    case SPDIF_CMD_SET_FMT_FOR_SOUNDBAR:
        return _set_format_soundbar(m, p);
    case SPDIF_CMD_CLEAR_FMT_FOR_SOUNDBAR: {
        static const uint32_t zero[3];
        return _set_format_soundbar(m, zero);
    }
    case SPDIF_CMD_SET_SCMS:                          /* "_set_scms_spdif" */
        REG(AUD_REGS, 0x330) = scms_bits(REG(AUD_REGS, 0x330), p[0]);
        return 0;
    case SPDIF_CMD_SET_LIGHT:                         /* "_set_lamp_spdif" "lamp %s" */
        if (p[0] != 0 && p[0] != 1)                   /* [FLIX] 0x601245ed */
            return 0;
        HAL_EnableSpdifCtrl((int)p[0]);
        return 0;
    case SPDIF_CMD_ONOFF_SOUNDBAR:                    /* "_set_onoff_soundbar" */
        if (p[0] != 0 && p[0] != 1)                   /* [FLIX] 0x6012429a */
            return 0;
        if (p[0] == 1) {
            REG(AUD_REGS, 0x338) &= 0xfffffu;
            REG(AUD_REGS, 0x33c) &= 0xfffff0u;
        } else {
            REG(AUD_REGS, 0x338) = (REG(AUD_REGS, 0x338) & 0xfffffu) | 0x100000;
            REG(AUD_REGS, 0x33c) = (REG(AUD_REGS, 0x33c) & 0xfffff0u) | 0xa;
        }
        soundbar_update_checksum();
        _noti_info_soundbar(m);
        return 0;
    case SPDIF_CMD_SET_CATEGORY_CODE:                 /* "_set_category_code_spdif" */
        REG(AUD_REGS, 0x330) = (REG(AUD_REGS, 0x330) & 0xffff80ffu) | (p[0] & 0x7f) << 8;
        return 0;
    case SPDIF_CMD_SET_EQBYPASS_SOUNDBAR:             /* "_set_eqbypass_soundbar" */
        REG(AUD_REGS, 0x33c) = (REG(AUD_REGS, 0x33c) & 0xfffeffu) | (p[0] == 1 ? 0x100u : 0u);
        soundbar_update_checksum();
        _noti_info_soundbar(m);
        return 0;
    case SPDIF_CMD_SET_WOOFERLEVEL:                   /* "_set_wooferlevel_soundbar" */
        REG(AUD_REGS, 0x33c) = (REG(AUD_REGS, 0x33c) & 0xffff0fu) | (p[0] & 0xf) << 4;
        soundbar_update_checksum();
        _noti_info_soundbar(m);
        return 0;
    case SPDIF_CMD_SET_WIRELESSRESET:                 /* "_set_wirelessreset_soundbar" */
        REG(AUD_REGS, 0x33c) = (REG(AUD_REGS, 0x33c) & 0xfffdffu) | (p[0] == 1 ? 0x200u : 0u);
        soundbar_update_checksum();
        _noti_info_soundbar(m);
        return 0;
    case SPDIF_CMD_SET_HDMIARC:                       /* "_set_hdmiarc_spdif" */
    case ARC_CMD_SET_HDMIARC:                         /* "_set_hdmiarc_arc" */
        set_hdmiarc(p);
        return 0;
    case SPDIF_CMD_SET_IEC958VALID:                   /* "_set_iec958valid_spdif" */
        REG(AUD_REGS, 0x324) = (REG(AUD_REGS, 0x324) & ~1u) | (p[0] == 1);
        return 0;

    case ARC_CMD_SET_OUTFORMAT:                       /* "_set_outformat_arc" */
        /* Same lane programming as HAL_SetArcPcmMode / HAL_SetArcEsMode, but the IEC
         * valid bit is left alone. p[2] (out_port) is not read. */
        if (p[0] == 0)                                /* [FLIX] 0x601241ee beqz */
            HAL_SetArcPcmMode();
        else
            HAL_SetArcEsMode((int)p[1]);
        return 0;
    case ARC_CMD_SET_SCMS:                            /* "_set_scms_arc" */
        REG(AUD_REGS, 0x3c0) = scms_bits(REG(AUD_REGS, 0x3c0), p[0]);
        for (int k = 0; k < 4; k++)
            REG(ARC_REGS, 0x130 + 0x20 * k) = scms_bits(REG(ARC_REGS, 0x130 + 0x20 * k), p[0]);
        return 0;
    case ARC_CMD_SET_CATEGORY_CODE:                   /* "_set_category_code_arc" */
        REG(AUD_REGS, 0x3c0) = (REG(AUD_REGS, 0x3c0) & 0xffff80ffu) | (p[0] & 0x7f) << 8;
        for (int k = 0; k < 4; k++)
            REG(ARC_REGS, 0x130 + 0x20 * k) =
                (REG(ARC_REGS, 0x130 + 0x20 * k) & 0xffff80ffu) | (p[0] & 0x7f) << 8;
        return 0;
    case ARC_CMD_SET_IEC958VALID:                     /* "_set_iec958valid_arc" */
        REG(ARC_REGS, 0x124) = (REG(ARC_REGS, 0x124) & ~1u) | (p[0] == 1);
        return 0;

    case CLOCK_CMD_GET_CLOCK_INFO: {
        uint32_t *r = malloc(12);
        if (!r)
            return -1;
        r[0] = (uint32_t)(m->cfg->bc_unit / m->stride_ch / 4);  /* frames per IRQ */
        if (m->irq_active)
            xos_interrupt_disable(m->cfg->irq);
        r[1] = m->irq_count;
        r[2] = m->irq_clock;
        if (m->irq_active)
            xos_interrupt_enable(m->cfg->irq);
        aimc_response_param(r, 12);                   /* [GAP] no free seen */
        return 0;
    }
    case CLOCK_CMD_SET_CLOCK_SOURCE:
        m->clock_source = (int)p[0];
        return 0;
    case CLOCK_CMD_GET_PLL_CONTROL_INFO: {
        uint32_t *r = malloc(0x20);
        if (!r)
            return -1;
        if (m->irq_active)
            xos_interrupt_disable(m->cfg->irq);
        memset(r, 0, 0x20);
        r[0] = m->irq_count;
        memcpy(&r[2], &m->irq_time, 8);
        /* "%8s: pll_control : %llu, %llu." */
        if (m->irq_active)
            xos_interrupt_enable(m->cfg->irq);
        aimc_response_param(r, 0x20);
        return 0;
    }
    case CLOCK_CMD_GET_BC_WRITE_TIME: {
        uint32_t *r = malloc(8);
        if (!r)
            return -1;
        memcpy(r, &m->first_out_time, 8);
        /* "%8s: bc_write_time : %llu." */
        aimc_response_param(r, 8);
        return 0;
    }
    default:
        return aud_module_default_command(m, cmd, p, len);
    }
}

/* ====================================================================================
 * Object lifecycle
 * ================================================================================== */

/* @0x60125054: wake a writer blocked in out_chain / out_chain_bc. */
int out_disconnect_port(struct out_module *m)
{
    aud_complete(&m->done);
    return 0;
}

/* @0x60125068 */
int out_constructor(struct out_module *m)
{
    struct out_cfg *cfg = m->cls->cfg;
    m->queue.next = m->queue.prev = &m->queue;
    m->cfg = cfg;
    m->stride_ch = cfg->stride_ch;
    m->output_stride_shift = ceil_log2(m->stride_ch * 32 / 8);
    /* "%8s: .." */
    if (cfg->instance) {
        /* "%8s: output already constructed. %p, %s" */
        return -1;
    }
    cfg->instance = m;
    m->max_len = 0x400;
    m->wait_len = 0x200;
    m->low_len = 0;
    aud_init_completion(&m->done);
    m->first_data = true;
    m->irq_count = 0;
    m->underflow_count = 0;
    m->irq_active = false;
    m->mute_countdown = 0;
    m->latency_us = 0;
    m->clock_source = 6;
    m->data_rate = 48000;
    return 0;
}

/* @0x6012514c */
void out_destructor(struct out_module *m)
{
    /* "%8s: .." */
    m->cfg->instance = NULL;
    if (m->started)
        out_stop(m);                                  /* inlined copy */
}

/* @0x601251e4: register the 4 outputs as "output_hp", "output_speaker", "output_arc",
 * "output_spdif" (class object sits 0x60 bytes before the table). */
int _init(void)
{
    for (int i = 0; i < 4; i++)
        ModuleRegistry_RegisterNamef(aud_object_class_base_output, &out_cfg_table[i],
                                     "output_%s", out_cfg_table[i].name);
    return 0;
}
