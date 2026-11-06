/*
 * dsp1 "output" module: shared types for output_arc.c, out_module.c, hal_arc.c.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp1.bin
 *   sha256 8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4
 * Offsets were read from the code (out_constructor @0x60125068, out_start @0x601226f8,
 * out_event @0x60123814, irq_handle_bc @0x60122098). The C layout does not reproduce the
 * offsets; each field carries its firmware offset as a comment.
 * Instance table (4 x 0x58 bytes @0x60280d60: hp, speaker, arc, spdif) registered by
 * _init @0x601251e4 as "output_%s".
 */
#ifndef DSP1_OUT_MODULE_H
#define DSP1_OUT_MODULE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ---- MMIO -------------------------------------------------------------------------- */
#define AUD_REGS   0xc0fffe00u   /* spdif/hp/speaker/arc(port 3) block (DAT_600e924c) */
#define ARC_REGS   0xc1000200u   /* eARC lane block; +0x130 + 0x20*k = lane k ctl/cs  */
#define AOC_REGS   0xc1000600u   /* +0x24 ARC ctrl, +0x28 eARC ctrl, +0x80/+0x84 clocks */
#define CHIP_REGS  0xc100fe00u
#define CHIP_ID    0xc1010200u   /* +0xe8 chip revision (0x26a0, > 0x179f checks)        */
#ifndef REG
#define REG(base, off) (*(volatile uint32_t *)(uintptr_t)((base) + (off)))
#endif

/* ---- framework types (only the fields this code touches) ---------------------------- */
enum { AUD_DATA_PCM = 0, AUD_DATA_ES = 1, AUD_DATA_META = 2, AUD_DATA_META_EXT = 3 };
enum { AUD_EVENT_FIRST = 1, AUD_EVENT_LAST = 2, AUD_EVENT_DISCONNECT = 6,
       AUD_EVENT_CUSTOM = 0xd };
#define PORT_DISABLED 0x2u       /* port->flags bit1 */

struct aud_data {
    int       type;              /* +0x10 */
    uint32_t  ts;                /* +0x18 90 kHz, 0xffffffff = none */
    unsigned  size;              /* +0x34 */
    uint8_t  *ptr;               /* +0x38 */
};

struct aud_event {
    int type;                    /* +0x0c */
};

struct aud_port {
    int       index;             /* +0x00 */
    void     *owner;             /* +0x04 module that owns this input port */
    uint32_t  flags;             /* +0x08 */
};

struct aud_completion { uint32_t opaque[4]; };

struct out_module;
struct out_qnode;

/* BC writer: dst_bytes of BC are produced; returns the SOURCE bytes consumed. */
typedef int (*out_bc_writer)(struct out_module *m, void *dst, const void *src,
                             unsigned dst_bytes);
/* FIFO writer: write n frames at FIFO word index pos; returns the new pos. */
typedef int (*out_fifo_writer)(struct out_module *m, int pos, int n, struct out_qnode *q);
typedef int (*out_state_fn)(struct out_module *m);

/* Per-output static configuration, 0x58 bytes. */
struct out_cfg {
    const char *name;                    /* +0x00 "hp", "speaker", "arc", "spdif" */
    uint32_t    fifo[8];                 /* +0x04 lane FIFO registers (arc 0xc0005000..) */
    int         fifo_words;              /* +0x24 FIFO depth per IRQ (arc 0x80) */
    int         irq;                     /* +0x28 (arc 0x10) */
    void      (*start)(struct out_module *);           /* +0x2c */
    void      (*stop)(struct out_module *);            /* +0x30 */
    void      (*mute)(struct out_module *, int mute);  /* +0x34 */
    uint32_t    flags;                   /* +0x38 bit0: buffer controller 4-ch (hp, spdif) */
    struct out_module *instance;         /* +0x3c */
    volatile uint32_t *bc_desc;          /* +0x40 [0]=phys base [1]=end [2]=hw rd [3]=wr;
                                            lane k descriptor at +0x20*k (arc 0xc1000420) */
    uint32_t    bc_size;                 /* +0x44 per-lane ring size (power of 2); 0 = no BC */
    uint8_t    *bc_virt;                 /* +0x48 */
    uint32_t    bc_phys;                 /* +0x4c */
    int         bc_unit;                 /* +0x50 bytes per IRQ period */
    int         stride_ch;               /* +0x54 output slots per lane frame (arc 2) */
};

struct out_qnode {                       /* calloc(0x18) in out_chain */
    struct out_qnode *next;              /* +0x00 */
    struct out_qnode *prev;              /* +0x04 */
    struct aud_data  *data;              /* +0x08 */
    unsigned          offset;            /* +0x0c bytes already written */
    unsigned          shift;             /* +0x10 log2(frame bytes) */
    int               channels;          /* +0x14 */
};

struct out_module {
    const char     *name;                /* +0x000 "output_arc", ... */
    int             aimc_id;             /* +0x004 */
    uint32_t        debug;               /* +0x008 log mask */
    struct out_class *cls;               /* +0x00c */
    void           *aimc_ctx;            /* +0x068 */
    struct out_cfg *cfg;                 /* +0x0f0 */
    out_fifo_writer fifo_writer;         /* +0x0f4 non-BC writer */
    int             stride_ch;           /* +0x0f8 */
    int             output_stride_shift; /* +0x0fc log2(stride_ch * 4) */
    int             channels;            /* +0x100 */
    int             bits_per_sample;     /* +0x104 */
    int             stride_shift;        /* +0x108 log2(channels * bps / 8) */
    int             codec;               /* +0x10c enum dsp1_out_codec */
    int             data_rate;           /* +0x110 */
    int             acmod;               /* +0x114 */
    bool            ch_mapping_lfe_c;    /* +0x118 */
    uint32_t        bc_hw_rd;            /* +0x11c last hardware read offset */
    uint32_t        bc_rd;               /* +0x120 */
    uint32_t        bc_wr;               /* +0x124 */
    int             bc_filled;           /* +0x128 bytes written but not played */
    int             bc_init_fill;        /* +0x12c zero pre-fill after start/underflow */
    out_bc_writer   bc_writer;           /* +0x130 */
    struct out_qnode queue;              /* +0x134 head: next=newest, prev=oldest */
    int             queued;              /* +0x13c frames */
    out_state_fn    state;               /* +0x140 process_data_init/wait/running */
    int             wait_len;            /* +0x144 0x200 */
    int             low_len;             /* +0x148 0 */
    int             max_len;             /* +0x14c 0x400 */
    int             started;             /* +0x150 */
    struct aud_completion done;          /* +0x154 */
    bool            es;                  /* +0x164 */
    int             mute_countdown;      /* +0x168 */
    int             clock_source;        /* +0x16c (6 after construct) */
    uint32_t        irq_count;           /* +0x170 */
    uint32_t        irq_clock;           /* +0x174 */
    uint32_t        underflow_count;     /* +0x178 */
    bool            irq_active;          /* +0x17c */
    bool            bc_need_fill;        /* +0x17d */
    uint64_t        first_out_time;      /* +0x180 */
    bool            first_data;          /* +0x188 */
    uint64_t        irq_time;            /* +0x190 {lo=AOC+0x84, hi=stch} at last IRQ */
    uint64_t        write_time;          /* +0x198 */
    uint64_t        first_write_time;    /* +0x1a0 */
    uint32_t        latency_us;          /* +0x1a8 */
};

struct out_class {
    struct out_cfg *cfg;                 /* +0x50 */
};

/* ---- externs (framework, OS, libc) --------------------------------------------------- */
extern int         aud_event_value_get_int(struct aud_event *e, const char *k, int def);
extern bool        aud_event_value_get_bool(struct aud_event *e, const char *k, bool def);
extern const char *aud_event_value_get_str(struct aud_event *e, const char *k, const char *def);
extern void        aud_event_value_set_int(struct aud_event *e, const char *k, int v);
extern int         aud_event_has_name(struct aud_event *e, const char *name);
extern void        aud_object_put(void *obj);
extern int         aud_module_default_event(void *m, struct aud_port *p, struct aud_event *e);
extern int         aud_module_default_command(void *m, uint32_t cmd, void *param, int len);
extern int         aud_module_push_data(void *m, int port, struct aud_data *d);
extern struct aud_data *aud_data_new_zero(unsigned size);
extern void        aimc_raise_named_event(void *ctx, int id, const char *name,
                                          const void *payload, int len);
extern void        aimc_response_param(const void *p, int len);
extern void        irq_register(int irq, const char *name, void (*fn)(struct out_module *),
                                struct out_module *arg);
extern void        xos_interrupt_enable(int irq);
extern void        xos_interrupt_disable(int irq);
extern void        aud_init_completion(struct aud_completion *c);
extern void        aud_reinit_completion(struct aud_completion *c);
extern void        aud_wait_for_completion(struct aud_completion *c);
extern void        aud_complete(struct aud_completion *c);
extern uint8_t    *AddrTrans_GetAddr(uint32_t phys);
extern void        ModuleRegistry_RegisterNamef(void *cls, void *desc, const char *fmt, ...);

/* ---- this module ------------------------------------------------------------------- */
uint32_t irq_save(void);
void     irq_restore(uint32_t ps);
uint32_t HAL_GetCurrentStch(void);
void     HAL_SetIEC1958ValidBit(int valid, int port);
void     HAL_SetArcPcmMode(void);
void     HAL_SetSpdifEsMode(int data_rate);
void     HAL_SetArcEsMode(int data_rate);
void     HAL_ClearInterrupt(unsigned irq);
void     HAL_SetMAOCChannelSet_BufferController(int ch);
void     HAL_EnableSpdifCtrl(int en);
void     HAL_EnableArcCtrl(int en, int port);
void     HAL_SetArcCtrl(uint32_t v);
void     HAL_SetEarcCtrl(uint32_t v);
void     HAL_EnableArcOut(uint32_t v, int port);
void     HAL_CacheRegionWriteback(void *p, unsigned len);

int  out_start(struct out_module *m);
int  out_stop(struct out_module *m);
int  out_chain(struct aud_port *port, struct aud_data *d);
int  out_chain_bc(struct out_module *m, struct aud_port *port, struct aud_data *d);
int  out_event(struct out_module *m, struct aud_port *port, struct aud_event *ev);
void irq_handle_bc(struct out_module *m);
int  fill_bc(struct out_module *m, const uint8_t *src, unsigned bytes);

int  cpy_es(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);
int  cpy_es8(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);
int  cpy_8(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);
int  cpy_8_swap_c_lfe(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);
int  cpy_24(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);
int  cpy_28(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);
int  cpy_42(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);
int  cpy_48(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);
int  cpy_82(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);
int  cpy_84(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);
int  cpy_00(struct out_module *m, void *dst, const void *src, unsigned dst_bytes);

/* IEC 60958 channel-status sampling-frequency code (bits 24..27) for a data rate in Hz,
 * or in kHz when the caller passes a small number. Inlined into HAL_SetSpdifEsMode,
 * HAL_SetArcEsMode and both out_cmd SET_OUTFORMAT cases. */
static inline uint32_t iec_rate_code(int rate)
{
    switch (rate) {
    case 44100: case 44:            return 0x0;
    case 32000: case 32:            return 0x3;
    case 22050: case 22:            return 0x4;
    case 24000: case 24:            return 0x6;
    case 88200: case 88:            return 0x8;
    case 768000: case 768:          return 0x9;   /* HBR (4 x 192 kHz) */
    case 96000: case 96:            return 0xa;
    case 176400: case 176:          return 0xc;
    case 192000: case 192:          return 0xe;
    default:                        return 0x2;   /* 48 kHz */
    }
}

#endif
