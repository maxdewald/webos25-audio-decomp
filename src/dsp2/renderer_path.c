/*
 * dsp2: generic module/port plumbing (first-event replay, drop flags) and the
 * lipsync clock selector used by the renderer's bypass (ES) head.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 * Main segment linked at 0x60000000. Plumbing matches the paluma-5 GPL module.c /
 * module_connector.c logic. Evidence: build/scratch/evidence/dsp2-connect/,
 * build/scratch/evidence/dsp2-lipsync/, build/decomp/audio_a0_dsp2.main_60000000.bin.c
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ------------------------------------------------------------------------
 * Ports and modules (offsets read from the firmware)
 * ---------------------------------------------------------------------- */
struct aud_event;
struct aud_port {
    struct aud_module *module;       /* +0x04 */
    uint32_t           flag;         /* +0x08: bit0 OUTPUT, bit1 DISABLED */
    struct aud_port   *up_peer;      /* +0xec */
    int                throwing;     /* +0x12c */
    struct aud_event  *current_first;/* +0x140 */
};

struct aud_module_impl {
    int (*chain)(struct aud_port *, void *data);                      /* +0x10 */
    int (*event)(struct aud_module *, struct aud_port *, struct aud_event *); /* +0x14 */
};

struct aud_module {
    const char *name;
    const struct aud_module_impl *impl;  /* +0x0c */
    bool  drop_output_data;              /* +0x94 property "drop-output-data" (bool); Ghidra
                                          * shows it as word index param_1+0x25 */
    bool  drop_output_meta;              /* +0x95 property "drop-output-meta" (bool) */
    struct aud_event *first_event_out;   /* +0xa8 */
};

extern struct aud_event *aud_event_new_first(void);  /* @0x60069e20: no key-values */
extern struct aud_event *aud_object_get(void *o);
extern void aud_object_put(void *o);
extern int  data_meta_type(void *data);               /* data +0x10 */
extern struct aud_port *aud_module_get_output(struct aud_module *, int);
extern void add_pending_obj_locked(struct aud_port *, void *obj);

/* @0x600724d8 aud_module_push_data: where "drop-output-data" is applied. */
int aud_module_push_data(struct aud_module *m, int port, void *data)
{
    if (m->drop_output_data) {
        aud_object_put(data);                        /* "%s: drop output data" */
        return 0;
    }
    if (m->drop_output_meta && data_meta_type(data) == 2) {
        aud_object_put(data);
        return 0;
    }
    struct aud_port *out = aud_module_get_output(m, port);
    if (!out) {
        aud_object_put(data);
        return 0;
    }
    add_pending_obj_locked(out, data);
    return 0;
}

/* @0x6006f600 / @0x60074154: connecting a port sends NOTHING. An existing output port
 * of the same number is reused, so its current_first survives a re-route. */

/* @0x600746d4 aud_module_chain_locked: the only first-event replay. On the first
 * DATA reaching an input port without a first event, the upstream output port's
 * stored first event (same es/codec/data-rate values) is handed to impl->event. */
int aud_module_chain_locked(struct aud_port *port, void *data)
{
    struct aud_module *m = port->module;

    if (port->flag & 2 || port->throwing) {
        aud_object_put(data);
        return -1;
    }
    if (!port->current_first) {
        struct aud_event *e = NULL;
        if (port->up_peer && port->up_peer->current_first)
            e = aud_object_get(port->up_peer->current_first);
        if (!e && !(e = aud_event_new_first()))     /* dummy first: no key-values */
            return -1;
        port->current_first = e;
        m->impl->event(m, port, aud_object_get(e));
    }
    return m->impl->chain(port, data);
}

/* ------------------------------------------------------------------------
 * lipsync clockType (+0x200), set by _lipsync_cmd (@0x600a159c) from the command
 * payload. It selects the STC source and PTS wrap mask.
 * ---------------------------------------------------------------------- */
/* Values are exact; the names are descriptive guesses (no enum strings in firmware). */
enum lipsync_clock_type {
    CLK_GSTC0       = 0x001,
    CLK_GSTC1       = 0x002,
    CLK_MEDIA_33BIT = 0x011,   /* STC +0x84, 31-bit mask, rebased via get_pts_for_media */
    CLK_MEDIA       = 0x100,
    CLK_MEDIA_RAW   = 0x101,   /* STC +0x8c */
    CLK_MEDIA_31BIT = 0x200,   /* STC +0x84, 31-bit; also selects seamless-delay mode */
};

#define PTS_MASK_28 0x0fffffffu
#define PTS_MASK_31 0x7fffffffu

extern volatile uint32_t *gstc_regs;    /* 0xc0fffe00 */
extern volatile uint32_t *stc_regs;     /* 0xc1000600 */
extern uint32_t _lipsync_get_pts_for_media(void *ls, uint32_t pts); /* @0x600a2078 */

/* @0x600a2f08 */
uint32_t _lipsync_get_stc(int clock_type)
{
    switch (clock_type) {
    case CLK_GSTC0:       return gstc_regs[0x220 / 4] & PTS_MASK_28;
    case CLK_GSTC1:       return gstc_regs[0x224 / 4] & PTS_MASK_28;
    case CLK_MEDIA_RAW:   return stc_regs[0x8c / 4] & PTS_MASK_28;
    case CLK_MEDIA_33BIT:
    case CLK_MEDIA_31BIT: return stc_regs[0x84 / 4] & PTS_MASK_31;
    default:              return stc_regs[0x84 / 4] & PTS_MASK_28;
    }
}

/* @0x600a2eac */
uint32_t _lipsync_get_pts(void *ls, int clock_type, uint32_t pts)
{
    switch (clock_type) {
    case CLK_GSTC0:
    case CLK_GSTC1:
    case CLK_MEDIA_RAW:   return pts & PTS_MASK_28;
    case CLK_MEDIA_33BIT: return _lipsync_get_pts_for_media(ls, pts) & PTS_MASK_31;
    case CLK_MEDIA_31BIT: return pts & PTS_MASK_31;
    default:              return _lipsync_get_pts_for_media(ls, pts) & PTS_MASK_28;
    }
}

/* @0x600a2f84 */
uint32_t _lipsync_get_maxpts(int clock_type)
{
    return (clock_type == CLK_MEDIA_33BIT || clock_type == CLK_MEDIA_31BIT)
           ? PTS_MASK_31 : PTS_MASK_28;
}
