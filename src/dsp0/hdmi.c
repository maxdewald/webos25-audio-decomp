/* Focused reconstruction of the DSP0 HDMI FIRST producer.
 * LG OLED42C27LA, Rockhopper 10.3.1-3006 / firmware 33.31.68.01.
 * audio_a0_dsp0.bin SHA256:
 * 337a6330397f0891a4cf9b9bccc06b36d902b1dce18e110507bc98f2a5f25b58.
 * Original obtained from LG's matching USB package:
 * https://gscs-b2c.lge.com/downloadFile?fileId=tFYxTpYW4V60IWzmdBRjIg
 *
 * Evidence: Ghidra 12.2 and tools/xtobjdump.sh on the hash-matched image.
 * FLIX branch slots at 0x60227ed4/edc/ee4 were read with the validated
 * Intel overlay; Ghidra treats them as opaque userops and misses the
 * rate-selection edges. No unknown SIMD/float slots are guessed here.
 * Packet processing and IRQ/helper bodies remain unrecovered; the bounded
 * BC/start/stop MMIO sequence below is reconstructed, not hardware-verified.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct dsp0_hdmi {
    uint32_t module_id;                      /* +0x000, IRQ owner on start */
    uint8_t unrecovered_004[0xec];
    uint32_t hardware_descriptor;            /* +0x0f0, firmware pointer */
    uint32_t bc_virtual;                     /* +0x0f4, translated address */
    uint32_t bc_physical;                    /* +0x0f8 */
    uint32_t bc_size;                        /* +0x0fc */
    uint32_t bc_write_offset;                /* +0x100 */
    uint32_t bc_read_offset;                 /* +0x104 */
    uint32_t bc_snapshot_write;              /* +0x108 */
    uint8_t unrecovered_10c[0x49];
    uint8_t first_155;                       /* +0x155, shared feed/main latch */
    uint8_t first_156;                       /* +0x156, PCM port 1 latch */
    uint8_t unrecovered_157;
    uint32_t hdmi_port;                      /* +0x158 */
    uint8_t unrecovered_15c[4];
    uint32_t input_spacing;                  /* +0x160, stale in modes 6/7/8 */
    uint32_t channel_num;                    /* +0x164 */
    uint32_t sampling_freq;                  /* +0x168 */
    uint8_t unrecovered_16c[8];
    uint32_t start_zero_174;                 /* +0x174, zeroed by start */
    uint8_t unrecovered_178[8];
    uint32_t start_zero_180[4];              /* +0x180..+0x18c, zeroed by start */
    uint32_t start_one_190;                  /* +0x190, set to 1 by start */
    uint8_t hbr;                             /* +0x194, ONLY modes 4/7 */
    uint8_t unrecovered_195[3];
    uint32_t start_zero_198[2];              /* +0x198/+0x19c, zeroed by start */
    uint32_t mode;                           /* +0x1a0 */
    uint32_t start_zero_1a4[2];              /* +0x1a4/+0x1a8, zeroed by start */
    uint32_t byte0_limit;                    /* +0x1ac, stale outside 6/7/8 */
    uint32_t start_zero_1b0;                 /* +0x1b0, zeroed by start */
    uint32_t block_size;                     /* +0x1b4, 0x400 or 0x1000 */
    uint32_t start_zero_1b8[7];              /* +0x1b8..+0x1d0, zeroed by start */
    int32_t data_drop_mode;                  /* +0x1d4 */
    int32_t codec;                           /* +0x1d8, FIRST label selector */
    uint8_t sizing_pending;                  /* +0x1dc, drop setter only */
    uint8_t unrecovered_1dd[3];
    uint32_t start;                          /* +0x1e0, raw parsed property */
    uint8_t start_one_1e4;                   /* +0x1e4, set to 1 by start */
    uint8_t unrecovered_1e5[3];
    uint32_t multi_pcm_out;                  /* +0x1e8 */
    uint8_t unrecovered_1ec[4];
    uint8_t start_zero_1f0;                  /* +0x1f0, zeroed by start */
    uint8_t unrecovered_1f1[3];
    uint32_t irq_count;                      /* +0x1f4 */
    uint32_t start_zero_1f8;                 /* +0x1f8, zeroed by start */
    uint8_t unrecovered_1fc[8];
    uint32_t start_zero_204;                 /* +0x204, zeroed by start */
    uint8_t start_zero_208;                  /* +0x208, zeroed by start */
    uint8_t unrecovered_209[3];              /* real object size not recovered */
} dsp0_hdmi;
_Static_assert(offsetof(dsp0_hdmi, sampling_freq) == 0x168, "HDMI rate");
_Static_assert(offsetof(dsp0_hdmi, codec) == 0x1d8, "HDMI codec");
_Static_assert(offsetof(dsp0_hdmi, block_size) == 0x1b4, "HDMI block size");
_Static_assert(offsetof(dsp0_hdmi, start) == 0x1e0, "HDMI start");
_Static_assert(offsetof(dsp0_hdmi, irq_count) == 0x1f4, "HDMI IRQ count");
_Static_assert(offsetof(dsp0_hdmi, start_zero_208) == 0x208, "HDMI tail");

/* @0x605386f8: firmware's 32-bit unsigned parser, base 0 in these setters.
 * Host tests supply this external dependency, not a setter model. */
extern uint32_t _Stoul(const char *value, char **end, int base);

/* Actual firmware dependencies: bodies/lifetime guarantees remain external.
 * AddrTrans_GetAddr @0x602112f8; IRQ helpers @0x6053ce80/0x6053ceb8;
 * irq_register @0x60212cf4; handler @0x60229760; parser reset @0x6023dd4c.
 * Host tests substitute MMIO storage and these dependencies only. */
extern uint32_t AddrTrans_GetAddr(uint32_t address);
extern void HdmiParser_Init_Dtsx_SycnType(void);
extern int xos_interrupt_enable(uint32_t irq);
extern int xos_interrupt_disable(uint32_t irq);
extern int irq_register(uint32_t irq, uint32_t owner, void (*handler)(void *), void *arg);
extern void hdmi_handle_irq(void *arg);
#ifndef DSP0_HDMI_REGISTERS
#define DSP0_HDMI_REGISTERS ((volatile uint32_t *)(uintptr_t)0xc1000600)
#endif
#if defined(__XTENSA__)
#define DSP0_HDMI_MEMW() __asm__ volatile("memw" ::: "memory")
#else
/* Host equivalent ordering for tests, not an Xtensa instruction-match claim. */
#define DSP0_HDMI_MEMW() __atomic_thread_fence(__ATOMIC_SEQ_CST)
#endif

/* @0x60229600: Changes live software mode/lane fields without restarting HDMI
 * or clearing either FIRST latch. 6/7/8 intentionally preserve input_spacing;
 * all other modes preserve byte0_limit. Mode 8 is NOT marked HBR. There is no
 * range validation or strict end-of-string check. Logging only is omitted.
 * Dispatch at 0x60229d73 reads mode each iteration: 1/2/4/5/6/7/8 use feed/core,
 * all others (including documented DTSCD=3) use the PCM writer. Thus a live
 * mode write can change processing while old FIRST remains latched; it is
 * not an atomic transport reconfiguration or evidence of safe hardware use.
 */
int hdmi_set_mode(dsp0_hdmi *module, const void *property, const char *value)
{
    uint32_t mode = _Stoul(value, NULL, 0);
    module->mode = mode;
    if (mode == 0)
        module->input_spacing = 1;
    else if (mode == 6)
        module->byte0_limit = 77200;
    else if (mode == 7 || mode == 8)
        module->byte0_limit = 308800;
    else
        module->input_spacing = 4;
    module->hbr = mode == 4 || mode == 7;
    return 0;
}

/* @0x6022a100: Clears BOTH FIRST latches and marks block sizing pending (along
 * with _hdmi_start which also clears both latches). Same-value writes are no-ops,
 * even with stale FIRST. No event is emitted here. Output callback 0x60229c5e..0x60229ca1
 * consumes sizing_pending after data arrives, not in this setter. No range validation.
 */
int hdmi_set_data_drop_mode(dsp0_hdmi *module, const void *property, const char *value)
{
    int32_t mode = (int32_t)_Stoul(value, NULL, 0);
    if (mode == module->data_drop_mode)
        return 0;
    module->data_drop_mode = mode;
    module->sizing_pending = 1;
    module->first_155 = 0;
    module->first_156 = 0;
    return 0;
}

#define HDMI_REG(byte_offset) (DSP0_HDMI_REGISTERS[(byte_offset) / 4])

/* Word indexes of the hardware descriptor at module+0xf0 (32-bit pointers). */
enum {
    HDMI_DESC_SAI = 0,        /* selects SAI control word 0x200+4*n and mux field */
    HDMI_DESC_IRQ = 1,
    HDMI_DESC_BC = 2,         /* -> {start, end, write pointer, read pointer} */
    HDMI_DESC_BC_ENABLE = 3,  /* -> word: 1 on start, 0 on stop */
    HDMI_DESC_BC_RESET = 4,   /* -> word: 3 on start, untouched on stop */
};
static inline volatile uint32_t *hdmi_mmio(uint32_t address)
{
    return (volatile uint32_t *)(uintptr_t)address;
}

/* @0x60229154: Initializes BC pointers, discarding unread data by copying the
 * hardware write pointer into its read pointer. Unsigned size must be 1..65536;
 * failure leaves translated/base/size fields written but old offsets untouched.
 * An out-of-range write offset clears +0x100/+0x108 only: +0x104 and hardware
 * read pointer intentionally stay stale. Snapshot +0x108 rereads live hardware.
 * No pointer/descriptor validation in LG; callers must provide mapped storage.
 * FLIX at 0x60229165 is a log-level branch + nop only. Logging is omitted.
 * AddrTrans_GetAddr remains external, not guessed here. Not hardware-verified.
 */
int _hdmi_InitBC(dsp0_hdmi *module)
{
    const uint32_t *desc = (const uint32_t *)(uintptr_t)module->hardware_descriptor;
    volatile uint32_t *bc = hdmi_mmio(desc[HDMI_DESC_BC]);
    DSP0_HDMI_MEMW();
    module->bc_physical = bc[0];
    module->bc_virtual = AddrTrans_GetAddr(module->bc_physical);
    bc = hdmi_mmio(desc[HDMI_DESC_BC]);
    DSP0_HDMI_MEMW();
    module->bc_size = bc[1] - module->bc_physical;
    if (module->bc_size == 0 || module->bc_size > 0x10000)
        return -1;
    DSP0_HDMI_MEMW();
    module->bc_write_offset = bc[2];
    DSP0_HDMI_MEMW();
    bc[3] = module->bc_write_offset;
    module->bc_write_offset -= module->bc_physical;
    bc = hdmi_mmio(desc[HDMI_DESC_BC]);
    DSP0_HDMI_MEMW();
    module->bc_read_offset = bc[3] - module->bc_physical;
    DSP0_HDMI_MEMW();
    module->bc_snapshot_write = bc[2] - module->bc_physical;
    if (module->bc_write_offset >= module->bc_size) {
        module->bc_write_offset = 0;
        module->bc_snapshot_write = 0;
    }
    return 0;
}

/* @0x60229258: start != 0 clears both FIRST latches (+0x155/+0x156)
 * and the start-owned counters, resets DTS parser sync, recomputes
 * block_size, then calls _hdmi_InitBC. An InitBC failure returns -1 with
 * those software resets kept and no rollback; IRQ/SAI/BC stay untouched.
 * It does not set sizing_pending (+0x1dc); block_size is recomputed here.
 * On success: IRQ register + enable, SAI bit 3, port mux from {0,1,6,7}
 * indexed by hdmi_port WITHOUT a bounds check, BC reset=3 then enable=1,
 * and per-port clock words for ports 0..3 (other ports: none).
 * start == 0: SAI bit 3 off, IRQ disable + unregister, BC enable=0, then an
 * UNBOUNDED wait for SAI busy bits 0x70000. Stop leaves FIRST latches, BC
 * reset and clock words alone. FLIX bundles here are validated branch slots
 * with nop partners; no unrecovered slot. Logging is omitted. Register
 * meanings are inferred from access patterns, not hardware-verified.
 */
int _hdmi_start(dsp0_hdmi *module, int start)
{
    const uint32_t *desc = (const uint32_t *)(uintptr_t)module->hardware_descriptor;
    if (!start) {
        DSP0_HDMI_MEMW();
        uint32_t sai = HDMI_REG(0x200 + 4 * desc[HDMI_DESC_SAI]) & ~8u;
        DSP0_HDMI_MEMW();
        HDMI_REG(0x200 + 4 * desc[HDMI_DESC_SAI]) = sai;
        xos_interrupt_disable(desc[HDMI_DESC_IRQ]);
        irq_register(desc[HDMI_DESC_IRQ], 0, NULL, NULL);
        DSP0_HDMI_MEMW();
        *hdmi_mmio(desc[HDMI_DESC_BC_ENABLE]) = 0;
        DSP0_HDMI_MEMW();
        if (HDMI_REG(0x200 + 4 * desc[HDMI_DESC_SAI]) & 0x70000) {
            do DSP0_HDMI_MEMW();      /* no timeout in LG either */
            while (HDMI_REG(0x200 + 4 * desc[HDMI_DESC_SAI]) & 0x70000);
        }
        return 0;
    }
    module->first_156 = 0;
    module->first_155 = 0;
    for (int i = 6; i >= 0; i--)
        module->start_zero_1b8[i] = 0;
    module->start_zero_198[1] = 0;
    module->start_zero_198[0] = 0;
    module->start_zero_1a4[1] = 0;
    module->start_zero_1a4[0] = 0;
    module->start_zero_1b0 = 0;
    module->start_zero_174 = 0;
    module->start_zero_180[2] = 0;
    module->start_zero_180[1] = 0;
    module->start_zero_180[0] = 0;
    module->start_zero_180[3] = 0;
    module->start_one_190 = 1;
    module->start_one_1e4 = 1;
    module->start_zero_204 = 0;
    module->start_zero_1f8 = 0;
    module->irq_count = 0;
    module->start_zero_208 = 0;
    module->start_zero_1f0 = 0;
    HdmiParser_Init_Dtsx_SycnType();

    uint32_t size = 0x400;
    if ((module->hbr && module->channel_num == 2) || module->codec == 2)
        size = 0x1000;
    else if (module->mode == 0 && module->data_drop_mode != 2 &&
             !(module->channel_num == 2 && module->sampling_freq <= 48000))
        size = 0x1000;
    module->block_size = size;
    if (_hdmi_InitBC(module) != 0)
        return -1;

    irq_register(desc[HDMI_DESC_IRQ], module->module_id, hdmi_handle_irq, module);
    xos_interrupt_enable(desc[HDMI_DESC_IRQ]);
    DSP0_HDMI_MEMW();
    uint32_t sai = HDMI_REG(0x200 + 4 * desc[HDMI_DESC_SAI]) | 8;
    DSP0_HDMI_MEMW();
    HDMI_REG(0x200 + 4 * desc[HDMI_DESC_SAI]) = sai;
    static const uint32_t port_mux[4] = {0, 1, 6, 7};  /* copied from 0x60005920 */
    DSP0_HDMI_MEMW();
    uint32_t mux = HDMI_REG(0x140);
    uint32_t selected = port_mux[module->hdmi_port];    /* no bounds check */
    if (desc[HDMI_DESC_SAI] == 0)
        mux = (mux & ~7u) | selected;
    else
        mux = (mux & ~0x70u) | (selected << 4);
    DSP0_HDMI_MEMW();
    HDMI_REG(0x140) = mux;
    DSP0_HDMI_MEMW();
    *hdmi_mmio(desc[HDMI_DESC_BC_RESET]) = 3;
    DSP0_HDMI_MEMW();
    *hdmi_mmio(desc[HDMI_DESC_BC_ENABLE]) = 1;
    uint32_t port = module->hdmi_port;
    if (port <= 3) {                     /* 0x60229450..0x6022956c */
        DSP0_HDMI_MEMW();
        HDMI_REG(0x304 + 8 * port) = 4;
        DSP0_HDMI_MEMW();
        HDMI_REG(0x300 + 8 * port) = 1;
    }
    return 0;
}

/* @0x60229570: Only an off->on or on->off transition (by zero/nonzero) is
 * accepted; repeated start or stop returns -1 without side effects. The
 * helper receives a boolean, but the RAW parsed value is stored, and the
 * helper's return is ignored: a failed InitBC still reports 0 and records
 * "started", so a retry needs an explicit stop first. Thus property success
 * is not hardware success. Logging only is omitted.
 */
int hdmi_set_start(dsp0_hdmi *module, const void *property, const char *value)
{
    uint32_t start = _Stoul(value, NULL, 0);
    if ((module->start != 0) == (start != 0))
        return -1;
    _hdmi_start(module, start != 0);
    module->start = start;
    return 0;
}

typedef struct dsp0_hdmi_event dsp0_hdmi_event;
extern dsp0_hdmi_event *aud_event_newv_first(const char *key, ...);
extern void aud_event_value_set_int(dsp0_hdmi_event *event, const char *key, int value);
extern void aud_event_value_set_bool(dsp0_hdmi_event *event, const char *key, bool value);
extern void aud_module_push_event(dsp0_hdmi *module, int port, dsp0_hdmi_event *event);

/* The implementation's recovered property table at 0x60551e50 has 14
 * 40-byte records. codec is a generic numeric property (+0x1d8), parsed
 * with _Stoul(base 0); literal "MAT" is not an enum spelling, use numeric 4.
 * mode is independently at +0x1a0 with custom setter hdmi_set_mode.
 * No separate label property occurs in this table (inherited/dynamic
 * properties outside this descriptor have not been ruled out).
 *
 * hdmi_set_mode @0x60229600 sets the HBR byte +0x194 only for mode 4 or 7;
 * output-thread dispatch @0x60229d73 selects feed/parser processing for
 * modes 1/2/4/5/6/7/8, otherwise the PCM writer. Codec 4 selects MAT label
 * in FIRST events without modifying the feed/start sizing paths (which
 * special-case codec 2). Changing codec with mode=0 still uses PCM processing.
 * FIRST latches are cleared by start (not stop), a changed drop-mode, and a
 * changed multi-pcm-out @0x6022a174 (+0x155 only). Parser bodies, the IRQ
 * handler and the output thread are not reconstructed here.
 */
typedef struct dsp0_hdmi_property {
    uint32_t name_address;   /* Xtensa pointer, log-only here */
    int32_t type;            /* 1 = bool, 2 = int, affects logging only */
    uint32_t offset;         /* property value already written at module+offset */
} dsp0_hdmi_property;

/* @0x6022a210: Notification after the property writer has stored the value.
 * Apart from type-specific logging, only offset 0x1d8 (codec) acts: signed
 * codec >= 8 is reset to PCM. It neither restarts HDMI nor emits FIRST.
 * The signed test preserves 7 and negative values, although those are unsafe
 * for the seven-element FIRST tables. Do not broaden the valid stream range
 * merely because this callback fails to reject them.
 */
int hdmi_prop_updated(dsp0_hdmi *module, const dsp0_hdmi_property *property)
{
    if (property->offset == 0x1d8 && module->codec >= 8)
        module->codec = 0;
    return 0;
}

/* @0x60227e78: Builds a FIRST for an HDMI output port; does not reframe data.
 * Valid stream codec selectors are 0..6. The original has no bounds check:
 * its channel/reform tables at 0x60005820/0x60005860 have seven entries,
 * as does the label array copied from literals at 0x601f8530..0x601f8548.
 * hdmi_prop_updated's weaker signed <8 check does NOT make selector 7 or
 * a negative selector safe to use here. Callers must keep that invariant.
 *
 * Port 0 is the decoder-facing main path: ES formats set bypass-reform,
 * not es=true. Any nonzero port uses the compressed bypass announcement.
 * MAT(4) and DTS-HD(6) share rate=48000/data-rate=192000/ch=8/bps=16,
 * but main-path bypass-reform differs. There is no label-only argument.
 * The log-only branches and their strings are omitted from this semantic
 * reconstruction; the externally visible metadata/return behavior is kept.
 */
int _hdmi_push_first_event(dsp0_hdmi *module, int port, int channels, int acmod)
{
    static const char *const labels[7] = {
        "pcm", "ac3", "eac3", "aac", "mat", "dts", "dts_hd_ma"
    };
    static const int es_channels[7] = {2, 2, 2, 2, 8, 2, 8};
    static const bool reform[7] = {false, true, true, true, false, true, true};
    int codec = module->codec;
    int rate = (int)module->sampling_freq, data_rate = rate, bits = 32;
    if (codec != 0) {
        channels = es_channels[codec];
        bits = 16;
        if (codec != 1 && codec != 3 && codec != 5) {
            rate = 48000;
            data_rate = 192000;
        }
    }
    dsp0_hdmi_event *event = aud_event_newv_first(
        "codec", 2, labels[codec], "rate", 0, rate,
        "data-rate", 0, data_rate, "channels", 0, channels,
        "bits-per-sample", 0, bits, (const char *)0);
    if (!event) return 0;
    if (codec == 0) {
        aud_event_value_set_int(event, "acmod", acmod);
        if (port == 0) aud_event_value_set_int(event, "out-channels", 8);
    } else if (port != 0) {
        aud_event_value_set_bool(event, "es", true);
    } else {
        aud_event_value_set_bool(event, "bypass-reform", reform[codec]);
    }
    aud_module_push_event(module, port, event);
    return 1;
}
