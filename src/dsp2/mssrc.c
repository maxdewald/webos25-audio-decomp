/* C2 DSP2 mssrc, saved main image SHA256
 * d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd.
 * Evidence: 0x60099c00..6009a930, literal pool 0x60060c84..60060dbc,
 * implementation/property tables 0x60103490/0x601034f0. Aligned Xtensa
 * entries and FLIX branch slots checked with xtlist/xtobjdump's objdump.
 * These are semantic host models, NOT binary-compatible ABI overlays.
 * Diagnostic-only log/string-formatting calls are omitted. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct aud_event { int type; unsigned flags; };
struct aud_data {
    int type, index;                      /* firmware +0x10, +0x14 */
    uint32_t ts, field_20, field_24;       /* +0x18, +0x20, +0x24 */
    unsigned size;                        /* +0x34 */
    void *ptr;                            /* +0x38 */
};
struct mssrc_adapter {
    unsigned opaque[4];
    uint32_t pushed, flushed;             /* module +0x100, +0x104 */
    unsigned tail;
};
struct mssrc {
    const char *name; int uid;             /* module +0x00, +0x04 */
    struct mssrc_adapter adapter;         /* +0xf0 */
    _Alignas(4) unsigned char library[0x100]; /* +0x10c..20b; opaque MS12 state */
    /* Named host views of library-owned output fields, not another ABI. */
    void *output_pcm;                     /* +0x160 */
    int format_ready;                     /* +0x178: library-owned */
    unsigned output_size;                 /* +0x180 */
    uint32_t library_output_format[7];    /* +0x194..1ac: library-owned */
    int sampling_rate, channels;          /* +0x204, +0x208: INPUT caches */
    uint32_t output_format[7];            /* +0x20c..224 */
    uint32_t field_228, timestamp;         /* +0x228, +0x22c */
    uint32_t field_230, field_234;         /* +0x230, +0x234 */
    uint32_t field_238, field_23c;         /* +0x238, +0x23c (signed short index) */
    uint32_t field_240, field_244;         /* +0x240, +0x244 */
    uint32_t field_248, field_24c, field_250, field_254;
    uint32_t field_258, field_25c, field_260, field_264;
    int initialized;                      /* +0x268: exactly 1 means open */
    unsigned ratio;                       /* +0x26c */
    uint32_t total_input, total_output;    /* +0x270, +0x274 */
    uint32_t channel_mode;                /* +0x278: property/command */
    bool unsupported;                     /* +0x27c: FIRST rejection latch */
};
struct aud_port { int index; struct mssrc *module; };
extern void aud_data_adapter_init(struct mssrc_adapter *);
extern void aud_data_adapter_cleanup(struct mssrc_adapter *);
extern void aud_data_adapter_flush(struct mssrc_adapter *, unsigned);
extern void aud_data_adapter_push(struct mssrc_adapter *, struct aud_data *);
extern struct aud_data *aud_data_adapter_get_data(struct mssrc_adapter *, unsigned);
extern struct aud_data *aud_data_adapter_peep(struct mssrc_adapter *);
extern void AuiLogger_Init(void *, int, unsigned);
extern void AuiLogger_AppendData(void *, const void *, unsigned);
extern int ms_src_main_process_open(void *), ms_src_main_connect_msf(void *);
extern int ms_src_main_process_close(void *);
extern int ms_src_main_process_init(void *, int, int);
extern unsigned ms_src_main_process_getsize(void *, unsigned);
extern int ms_src_main_process_run(void *, void *, unsigned, unsigned);
extern int msf_process_metadata(void *, const void *, unsigned);
extern void msf_flush_metadata(void *), msf_print_metadata(void *);
extern void __assert_failed(const char *, unsigned, const char *);
extern struct aud_data *aud_data_new_metadata(void *), *aud_data_new(unsigned);
extern int aud_module_push_data(struct mssrc *, int, struct aud_data *);
extern void aud_object_put(void *);
extern int aud_module_default_command(struct mssrc *, uint32_t, void *, unsigned);
extern int aud_module_default_event(struct mssrc *, struct aud_port *, struct aud_event *);
extern int aud_event_value_get_int(struct aud_event *, const char *, int);
extern void aud_event_value_set_int(struct aud_event *, const char *, int);
extern struct aud_event *aud_event_dup(struct aud_event *);
extern char *audutil_strdup_printf(const char *, ...);
extern uint32_t ms_src_get_lib_version(void);
extern volatile uint32_t *mssrc_ipc_log_mask; /* literal 0x60060ca0 */
extern uint32_t *mssrc_ipc_id;               /* literal 0x60060c9c */
extern void IPC_WriteDebug(uint32_t, int, int, int, const char *, ...);

static void *metadata(struct mssrc *m) { return m->library + 8; } /* +0x114 */
static void *output_metadata(struct mssrc *m) { return m->library + 0x40; } /* +0x14c */

/* @0x60099c00: prints metadata from the MSF connector, not the FIRST event. */
int _mssrc_print_metadata(struct mssrc *m)
{
    msf_print_metadata(metadata(m));
    return 0;
}

/* @0x60099c90: diagnostic status only; each line rechecks the volatile mask.
 * The fields labelled ES/output channel here are the input FIRST caches. */
int _mssrc_print_status(struct mssrc *m)
{
    if ((*mssrc_ipc_log_mask & 0x200000u) != 0)
        IPC_WriteDebug(*mssrc_ipc_id, 1, 0x15, 0, "================================\n");
    if ((*mssrc_ipc_log_mask & 0x200000u) != 0)
        IPC_WriteDebug(*mssrc_ipc_id, 1, 0x15, 0, "=== %s(%d) Info \n", m->name, m->uid);
    if ((*mssrc_ipc_log_mask & 0x200000u) != 0)
        IPC_WriteDebug(*mssrc_ipc_id, 1, 0x15, 0,
                       "= ES Info : Sampling Rate    : %d \n", m->sampling_rate);
    if ((*mssrc_ipc_log_mask & 0x200000u) != 0)
        IPC_WriteDebug(*mssrc_ipc_id, 1, 0x15, 0,
                       "= ES Info : Output Channel   : %d \n", m->channels);
    if ((*mssrc_ipc_log_mask & 0x200000u) != 0)
        IPC_WriteDebug(*mssrc_ipc_id, 1, 0x15, 0,
                       "= Total Input Byte(port1)    : %d \n", (int32_t)m->total_input);
    if ((*mssrc_ipc_log_mask & 0x200000u) != 0)
        IPC_WriteDebug(*mssrc_ipc_id, 1, 0x15, 0,
                       "= Total Output Byte          : %d \n", (int32_t)m->total_output);
    if ((*mssrc_ipc_log_mask & 0x200000u) != 0) {
        uint32_t version = ms_src_get_lib_version();
        IPC_WriteDebug(*mssrc_ipc_id, 1, 0x15, 0,
                       "= MSSRC Library Version      : %x \n", version);
    }
    if ((*mssrc_ipc_log_mask & 0x200000u) != 0)
        IPC_WriteDebug(*mssrc_ipc_id, 1, 0x15, 0, "================================\n");
    return 0;
}

/* @0x60099db0: FIRST always restarts an open library (branch @0x60099dc8
 * to 0x60099e64), not an idempotent open. Nonzero states OTHER than 1 skip
 * close/open/connect but still reset counters. Assertions do not map to an
 * error return; if the assertion hook returns, execution continues. Neither
 * +0x260, ratio, output_size, cached formats nor channel_mode is reset. */
int _mssrc_init(struct mssrc *m)
{
    if (m->initialized == 1) {
        msf_flush_metadata(metadata(m));
        if (ms_src_main_process_close(m->library) != 0)
            __assert_failed("_mssrc_init", 0xa5, "result == 0");
        m->initialized = 0;
    }
    if (m->initialized == 0) {
        if (ms_src_main_process_open(m->library) != 0)
            __assert_failed("_mssrc_init", 0xad, "result == 0");
        if (ms_src_main_connect_msf(m->library) != 0)
            __assert_failed("_mssrc_init", 0xaf, "result == 0");
        m->initialized = 1;
    }
    m->total_input = m->total_output = 0;
    m->field_24c = m->field_250 = m->field_254 = 0;
    m->field_258 = m->field_25c = m->field_264 = 0;
    m->field_248 = 0xff;
    return 0;
}

/* @0x60099e88: flushes even zero queued bytes; does NOT close/reopen, reset
 * totals/timestamps/output bytes or change the unsupported-stream latch. */
int _mssrc_flush(struct mssrc *m)
{
    aud_data_adapter_flush(&m->adapter, m->adapter.pushed - m->adapter.flushed);
    if (m->initialized == 1)
        msf_flush_metadata(metadata(m));
    return 0;
}

/* @0x60099edc: literal/branch tree @0x60099ef3..60099f78 and aligned
 * 0x60099f59. No init/flush command is dispatched to the local functions:
 * 0x10000000/10010000/10020000/1003002c/10150000/63010008 are no-ops.
 * 0x63020004 (SET_DEC_CORE) is explicitly unimplemented, also returns 0.
 * Only 0x63030004 changes settings: one unsigned word channel-mode 0..2,
 * invalid -> 2. Payload length is ignored. Everything else delegates.
 * There is NO output rate/bits/channels command here. */
int _mssrc_cmd(struct mssrc *m, uint32_t command, void *payload, unsigned size)
{
    switch (command) {
    case 0x10000000: case 0x10010000: case 0x10020000: case 0x1003002c:
    case 0x10150000: case 0x63010008: case 0x63020004:
        return 0;
    case 0x10070000:
        return _mssrc_print_status(m);
    case 0x10140000:
        return _mssrc_print_metadata(m);
    case 0x63030004: {
        uint32_t mode;
        memcpy(&mode, payload, sizeof(mode));
        if (mode >= 3) return 2;
        m->channel_mode = mode;
        return 0;
    }
    default:
        return aud_module_default_command(m, command, payload, size);
    }
}

/* @0x6009a014: metadata then PCM, using library-produced bytes, not a kernel.
 * Both packets inherit the signed low 16 bits at +0x23c and timestamp +0x22c.
 * Metadata also gets +0x230/+0x234. PCM type is explicitly 0. Output allocation
 * failures and downstream push errors still return 0; no totals/size changes
 * occur here (those belong to the chain). Odd output_size asserts. */
int _mssrc_push_pcm(struct mssrc *m)
{
    if ((m->output_size & 1u) != 0)
        __assert_failed("_mssrc_push_pcm", 0x113, "(dec->i_out_bytes & 0x1) == 0");
    struct aud_data *data = aud_data_new_metadata(output_metadata(m));
    int index = (int)(m->field_23c & 0xffffu);
    if (index >= 0x8000) index -= 0x10000;
    if (data != NULL) {
        data->index = index;
        data->ts = m->timestamp;
        data->field_20 = m->field_230;
        data->field_24 = m->field_234;
        aud_module_push_data(m, 0, data);
    }
    data = aud_data_new(m->output_size);
    if (data == NULL) return 0;
    data->index = index;
    data->ts = m->timestamp;
    data->type = 0;
    AuiLogger_AppendData(data->ptr, m->output_pcm, m->output_size);
    aud_module_push_data(m, 0, data);
    return 0;
}

/* @0x6009a17c: merged through 0x6009a5f4, including split branch targets.
 * META bypasses the adapter; every other type (including ES) is PCM input.
 * MS12 init/run/getsize remain genuine library boundaries, not fake kernels.
 * Aligned entries 0x6009a1dd/3c3 recover the ratio branch. Validated FLIX
 * branch slots suffice here; no SIMD arithmetic is synthesized. Library
 * formats must have positive channels and whole-byte sample widths; zero
 * divisors/malformed formats are outside this host model (guards below).
 * Neither metadata path copies index/field_20/field_24 into the PCM caches. */
int _mssrc_chain(struct aud_port *port, struct aud_data *input)
{
    struct mssrc *m = port->module;
    if (m->unsupported) {
        aud_object_put(input);
        return -1;
    }
    if (input->type == 2) {
        (void)msf_process_metadata(metadata(m), input->ptr, input->size);
        aud_object_put(input);
        return 0;
    }
    if ((uint32_t)m->sampling_rate < 48000u) {
        /* Host safety boundary: the DSP's unsigned divide-by-zero result is
         * not modeled. FIRST/property validation does not reject rate=0. */
        if (m->sampling_rate == 0) {
            aud_object_put(input);
            return 1;
        }
        m->ratio = 48000u / (uint32_t)m->sampling_rate;
    }
    if (m->ratio == 0) m->ratio = 1;
    if (m->format_ready == 0) {
        int result = ms_src_main_process_init(m->library, 0, 0);
        if ((result != 3 && result != 4) || m->format_ready == 0) {
            aud_object_put(input);
            return 1;
        }
        AuiLogger_AppendData(m->output_format, m->library_output_format, 28);
    }
    if ((int32_t)m->output_size > 0) {
        int result = _mssrc_push_pcm(m);
        if (result != 0) {
            aud_object_put(input);
            return result;
        }
        m->total_output += m->output_size;
        m->output_size = 0;
    }
    aud_data_adapter_push(&m->adapter, input);
    m->total_input += input->size;
    for (;;) {
        unsigned needed = ms_src_main_process_getsize(m->library, m->ratio);
        if (m->adapter.pushed - m->adapter.flushed < needed) return 0;
        m->field_25c = 0;
        struct aud_data *chunk = aud_data_adapter_get_data(&m->adapter, needed);
        if (chunk == NULL) return 0;
        struct aud_data *head = aud_data_adapter_peep(&m->adapter);
        m->timestamp = head == NULL ? 0x0fffffffu : head->ts;
        /* Scalar 8 x 32-bit interleaved channel routing. Mode 1 replaces
         * channels 0/1 with 6/7 and clears 2..7; any other nonzero mode
         * clears only 6/7. Trailing partial frames are untouched. */
        if (m->channel_mode != 0 && needed <= INT32_MAX) {
            unsigned char *pcm = chunk->ptr;
            for (unsigned frame = 0; frame < needed / 32; ++frame, pcm += 32) {
                if (m->channel_mode == 1) {
                    memcpy(pcm, pcm + 24, 8);
                    memset(pcm + 8, 0, 24);
                } else {
                    memset(pcm + 24, 0, 8);
                }
            }
        }
        int result = ms_src_main_process_run(m->library, chunk->ptr, needed, m->ratio);
        aud_object_put(chunk);
        if (result == -30656) { /* literal 0xffff8840: unsupported rate */
            _mssrc_flush(m);
            _mssrc_init(m);
            return 0;
        }
        if (result != 0 && result != 3) {
            _mssrc_flush(m);
            return 0;
        }
        aud_data_adapter_flush(&m->adapter, needed);
        if (result != 0 || (int32_t)m->output_size <= 0) continue;
        m->field_260 = needed;
        if (m->field_258 == 0) m->field_258 = 1;
        uint32_t timestamp = m->timestamp;
        if (timestamp == m->field_248 || timestamp == 0x0fffffffu) {
            /* Scalar quos/mull/quos @0x6009a59c: truncation occurs before
             * multiplication by 90000, then division by fixed 48000. */
            int32_t channels = (int32_t)m->output_format[1];
            int32_t bits = (int32_t)m->output_format[3];
            if (channels <= 0 || bits < 8 || (bits & 7) != 0 ||
                (uint32_t)channels > INT32_MAX / (uint32_t)(bits / 8))
                return 0; /* host safety boundary, not a firmware branch */
            int32_t frame_bytes = channels * (bits / 8);
            int32_t frames = (int32_t)m->output_size / frame_bytes;
            int32_t ticks = (int32_t)((uint32_t)frames * 90000u);
            m->field_250 += (uint32_t)(ticks / 48000);
            timestamp = m->field_250;
        } else {
            m->field_250 = timestamp;
            m->field_248 = timestamp;
        }
        m->timestamp = m->field_24c = timestamp;
        m->field_254 = m->output_size;
        result = _mssrc_push_pcm(m);
        if (result != 0) return 0;
        m->total_output += m->output_size;
        m->output_size = 0;
    }
}

/* @0x6009a5f4: FIRST accepts ONLY channels=8 and bits-per-sample=32 (defaults
 * 8/32), caches rate (default 48000) and channels BEFORE validating. No reads
 * of es, codec, data-rate, endian, sign or dolby. No supported-rate whitelist.
 * Invalid FIRST is released, sets +0x27c and returns -1 without forwarding,
 * flushing or initializing. A later valid FIRST clears the latch.
 *
 * At 48 kHz the original FIRST goes to default_event, unless acmod changes.
 * Otherwise dup -> rate=48000/channels=8; only the successful replacement
 * releases the old event. Independently, check-downmix==1 && channel-mode==1
 * causes ANOTHER dup -> acmod=2 (0x6009a79d/7c8). All other metadata, including
 * es/codec/data-rate/bits, stays unchanged. Failed dup forwards the previous
 * event unmodified, but still flushes/reinitializes. Mode 2 does NOT rewrite
 * acmod. FIRST is handled even when port=NULL. FLUSH(7) flushes iff flags&2,
 * then delegates; every other event delegates unchanged. */
int _mssrc_event(struct mssrc *m, struct aud_port *port, struct aud_event *event)
{
    if (event->type == 1) {
        m->sampling_rate = aud_event_value_get_int(event, "rate", 48000);
        m->channels = aud_event_value_get_int(event, "channels", 8);
        int downmix = aud_event_value_get_int(event, "check-downmix", -1);
        int bits = aud_event_value_get_int(event, "bits-per-sample", 32);
        if (m->channels != 8 || bits != 32) {
            aud_object_put(event);
            m->unsupported = true;
            return -1;
        }
        m->unsupported = false;
        if (m->sampling_rate != 48000) {
            struct aud_event *out = aud_event_dup(event);
            if (out != NULL) {
                aud_event_value_set_int(out, "rate", 48000);
                aud_event_value_set_int(out, "channels", 8);
                aud_object_put(event);
                event = out;
            }
        }
        if (downmix == 1 && m->channel_mode == 1) {
            struct aud_event *out = aud_event_dup(event);
            if (out != NULL) {
                aud_event_value_set_int(out, "acmod", 2);
                aud_object_put(event);
                event = out;
            }
        }
        _mssrc_flush(m);
        _mssrc_init(m);
    } else if (event->type == 7 && (event->flags & 2u) != 0) {
        _mssrc_flush(m);
    }
    return aud_module_default_event(m, port, event);
}

/* @0x6009a800: framework zero-allocation/property defaults supply mode/latch=0;
 * this constructor itself DOES NOT write +0x278/+0x27c. AuiLogger_Init is the
 * firmware's generic memory initializer; no SRC open/connect occurs here. */
int _mssrc_constructor(struct mssrc *m)
{
    aud_data_adapter_init(&m->adapter);
    AuiLogger_Init(m->library, 0, 0x100);
    m->output_pcm = NULL;
    m->format_ready = 0;
    m->output_size = 0;
    memset(m->library_output_format, 0, sizeof(m->library_output_format));
    m->sampling_rate = 48000;
    m->channels = 8;
    memset(m->output_format, 0, sizeof(m->output_format));
    m->field_228 = m->timestamp = m->field_230 = m->field_234 = 0;
    m->field_238 = m->field_23c = m->field_240 = m->field_244 = 0;
    m->field_248 = m->field_24c = m->field_250 = m->field_254 = 0;
    m->field_258 = m->field_25c = m->field_260 = m->field_264 = 0;
    m->initialized = 0;
    m->ratio = m->total_input = m->total_output = 0;
    return 0;
}

/* @0x6009a8a0: only initialized==1 closes, then always adapter cleanup.
 * It flushes MSF metadata but does not call the adapter-flushing function. */
void _mssrc_destructor(struct mssrc *m)
{
    if (m->initialized == 1) {
        msf_flush_metadata(metadata(m));
        if (ms_src_main_process_close(m->library) != 0)
            __assert_failed("_mssrc_destructor", 0x2de, "result == 0");
        m->initialized = 0;
    }
    aud_data_adapter_cleanup(&m->adapter);
}

/* @0x6009a908: unsigned wraparound adapter occupancy, formatted as a property. */
char *mssrc_gets_internal_buf_size(struct mssrc *m)
{
    return audutil_strdup_printf("(int)%u", m->adapter.pushed - m->adapter.flushed);
}

struct mssrc_property {
    const char *name;
    unsigned type, firmware_offset, flags;
    size_t host_offset;
    char *(*gets)(struct mssrc *);
};
/* Table @0x601034f0 (0x28-byte firmware entries): only these properties.
 * All default/set/geti/update slots are zero; generic integer writes apply
 * immediately to the caches, with NO validation or library reconfiguration.
 * sampling-rate/channels name INPUT caches, not output settings. channel-mode
 * defaults to zero via allocation, but its property write can exceed 2 (unlike
 * the command). There are no output-rate/bits/channels or acmod properties. */
const struct mssrc_property mssrc_properties[] = {
    { "sampling-rate", 2, 0x204, 0, offsetof(struct mssrc, sampling_rate), NULL },
    { "channels", 2, 0x208, 0, offsetof(struct mssrc, channels), NULL },
    { "channel-mode", 2, 0x278, 0, offsetof(struct mssrc, channel_mode), NULL },
    { "internal_buf_size", 0, 0, 1, 0, mssrc_gets_internal_buf_size },
    { NULL, 0, 0, 0, 0, NULL }
};
struct mssrc_implementation {
    const char *name;
    unsigned firmware_instance_size;
    int (*constructor)(struct mssrc *);
    void (*destructor)(struct mssrc *);
    int (*chain)(struct aud_port *, struct aud_data *);
    int (*event)(struct mssrc *, struct aud_port *, struct aud_event *);
    int (*command)(struct mssrc *, uint32_t, void *, unsigned);
    const struct mssrc_property *properties;
};
const struct mssrc_implementation mssrc_implementation = {
    "mssrc", 0x280, _mssrc_constructor, _mssrc_destructor, _mssrc_chain,
    _mssrc_event, _mssrc_cmd, mssrc_properties
};
extern void ModuleRegistry_Register(const struct mssrc_implementation *);

/* @0x6009a924: original initializer registers implementation @0x60103490. */
void mssrc_register(void)
{
    ModuleRegistry_Register(&mssrc_implementation);
}
