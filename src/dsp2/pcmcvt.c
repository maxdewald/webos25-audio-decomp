/* C2 DSP2 pcmcvt, main image SHA256
 * d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd.
 * VA base 0x60000000; evidence 0x6009a930..6009ce28 and literal pool
 * 0x60060dc0..60061094. Branch slots checked with tools/xtobjdump.sh;
 * split Ghidra blocks are folded into their actual functions here.
 * Types below are semantic host models, NOT binary-compatible ABI overlays.
 * Diagnostic-only logging is omitted. Unrecovered RFT/SRC kernels are external,
 * not replaced by memcpy or guessed DSP arithmetic. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct aud_event { int type; unsigned prop; };
struct aud_data { int type; uint32_t ts; unsigned size; void *ptr; };
struct pcmcvt;
struct aud_port { int index; struct pcmcvt *module; unsigned flag; };
struct aud_mutex { unsigned opaque; };
struct pcmcvt_adapter {
    unsigned opaque[4];
    uint32_t pushed;                          /* instance +0x174 */
    uint32_t flushed;                         /* +0x178 */
    unsigned tail;
};
struct pcmcvt {
    int input_bytes, output_bytes;            /* +0xf0, +0xf4 */
    int over_protect;                         /* +0xf8 */
    int src_bypass;                           /* +0xfc: NOT an RFT bypass */
    int unused_100;
    int total_input, total_output;            /* +0x104, +0x108 */
    int last_input, last_output;              /* +0x10c, +0x110 */
    int sample_shift;                         /* +0x114, out_channels >> 1 */
    int running_mode;                         /* +0x118, 1=filter, 2=linear */
    void *src;                               /* +0x11c */
    _Alignas(4) unsigned char rft[0x3c];       /* +0x120 */
    uint32_t previous_ts, output_ts;          /* +0x15c, +0x160 */
    struct pcmcvt_adapter adapter;            /* +0x164 */
    int input_channels, input_bits, input_rate; /* +0x180..188 */
    int input_endian, input_sign;             /* +0x18c, +0x190 */
    int output_channels, output_bits, output_rate; /* +0x194..19c */
    unsigned output_samples;                 /* +0x1a0 */
    bool generate_lfe, gain_enable_property;  /* +0x1a4,+0x1a5; latter not active RFT state */
    int gain, acmod;                          /* +0x1a8, +0x1ac */
    bool available;                          /* +0x1b0 */
    void *rft_buffer, *src_buffer;            /* +0x1b4, +0x1b8 */
    struct aud_mutex lock;                   /* +0x1bc */
};
struct pcmcvt_allfmt {
    uint32_t input_rate, input_channels, input_bits, input_endian, input_sign;
    uint32_t output_rate, output_channels, output_bits;
};
struct pcmcvt_fmt {
    uint32_t input_rate, output_rate, input_channels, output_channels;
    uint32_t input_bits, output_bits;
};

extern void *lgsrc_create(void);
extern int lgsrc_init(void *);
extern void lgsrc_Destroy(void *);
extern int lgsrc_setInputFs(void *, int), lgsrc_setOutputFs(void *, int);
extern int lgsrc_setInOutPCMFormat(void *, int), lgsrc_setInOutChannel(void *, int);
extern int lgsrc_setRunMode(void *, int), lgsrc_setFilterType(void *, int);
extern int lgsrc_setOutSample(void *, unsigned);
extern int lgsrc_getInChannel(void *), lgsrc_getOutChannel(void *);
extern int lgsrc_getInputFs(void *), lgsrc_getOutputFs(void *);
extern int lgsrc_getInPCMFormat(void *), lgsrc_getOutPCMFormat(void *);
extern int lgsrc_getOutSampleNum(void *), lgsrc_getNeedfulSamplePer1Ch(void *);
extern int lgsrc_process(void *, const void *, void *);
extern int RFT_SetInPCMFormat(void *, int), RFT_SetInChannel(void *, int);
extern int RFT_SetInSigned(void *, int), RFT_SetInEndian(void *, int);
extern int RFT_SetOutPCMFormat(void *, int), RFT_SetOutChannel(void *, int);
extern int RFT_SetGenLFE(void *, int), RFT_SetAcmod(void *, int);
extern int RFT_SetGai(void *, int enable, int gain);
extern int RFT_SetOutSample(void *, int);
extern int RFT_GetNeedfulSample(const void *);
extern int LGRFT(void *, const void *, void *, void *, int available);
extern void aud_mutex_init(struct aud_mutex *);
extern void aud_mutex_lock(struct aud_mutex *), aud_mutex_unlock(struct aud_mutex *);
extern void aud_data_adapter_init(struct pcmcvt_adapter *);
extern void aud_data_adapter_cleanup(struct pcmcvt_adapter *);
extern int aud_data_adapter_push(struct pcmcvt_adapter *, struct aud_data *);
extern void *aud_data_adapter_map(struct pcmcvt_adapter *, unsigned);
extern void aud_data_adapter_unmap(struct pcmcvt_adapter *, void *);
extern void aud_data_adapter_flush(struct pcmcvt_adapter *, unsigned);
extern struct aud_data *aud_data_new(unsigned);
extern void aud_object_put(void *);
extern int aud_module_push_data(struct pcmcvt *, int, struct aud_data *);
extern int aud_module_push_event(struct pcmcvt *, int, struct aud_event *);
extern int aud_module_sync(struct pcmcvt *, int);
extern int aud_module_default_event(struct pcmcvt *, struct aud_port *, struct aud_event *);
extern int aud_module_default_command(struct pcmcvt *, uint32_t, void *, unsigned);
extern int aud_event_value_get_int(struct aud_event *, const char *, int);
extern int aud_event_has_name(struct aud_event *, const char *);
extern void aud_event_value_set_int(struct aud_event *, const char *, int);
extern struct aud_event *aud_event_dup(struct aud_event *);
extern void AuiLogger_AppendData(void *, const void *, unsigned);
extern void *_BufMonitor_RemoveBufferIdxLocal(unsigned);
extern void _BufMonitor_RemoveBufferIndex(void *);
extern char *audutil_strdup_printf(const char *, ...);
extern void ModuleRegistry_Register(void *);
extern void *pcmcvt_impl_descriptor;          /* literal @0x60061094 ->0x601035d0 */
extern void __assert_failed(const char *, unsigned, const char *);

/* @0x6009a930: legacy kHz shorthand; property setters do NOT call this. */
int Pcmcvt_CheckSamplingRate(int rate)
{
    if (rate == 0) return 48000;
    if (rate > 999) return rate;
    switch (rate) {
    case 44: return 44100;
    case 22: return 22050;
    case 11: return 11025;
    case 88: return 88200;
    case 176: return 176400;
    case 352: return 352800;
    default: return (int)((uint32_t)rate * 1000u);
    }
}

static unsigned queued_bytes(const struct pcmcvt *m)
{
    return m->adapter.pushed - m->adapter.flushed;
}

static void flush_adapter(struct pcmcvt *m, bool even_empty)
{
    aud_mutex_lock(&m->lock);
    unsigned bytes = queued_bytes(m);
    if (bytes != 0 || even_empty)
        aud_data_adapter_flush(&m->adapter, bytes);
    aud_mutex_unlock(&m->lock);
}

static int src_formats_equal(struct pcmcvt *m)
{
    if (lgsrc_getInChannel(m->src) != lgsrc_getOutChannel(m->src)) return 0;
    if (lgsrc_getInputFs(m->src) != lgsrc_getOutputFs(m->src)) return 0;
    return lgsrc_getInPCMFormat(m->src) == lgsrc_getOutPCMFormat(m->src);
}

/* @0x6009ac0c
 * Applies cached properties immediately, ignoring RFT setter errors. RFT always
 * precedes SRC. SRC sees OUTPUT bits/channels on BOTH sides, so even a 16->32
 * RFT conversion can set +0xfc=1 when rates match (tail @0x6009adec).
 * No endian/sign/gain/LFE/available check exists in the SRC bypass predicate.
 * Setting properties does not flush queued input; FIRST handles that separately.
 * Each format property applies the whole cache, even at the same value: a
 * multi-property update exposes intermediate formats, not an allfmt transaction.
 * RFT_SetGenLFE clears filter history on EVERY apply, even unchanged/disabled.
 * Does NOT apply m->gain or gain_enable_property: active RFT gain survives format
 * properties, FIRST, SET_ALLFMT and flush, even if the property cache disagrees. */
int Pcmcvt_SetParamToLibs(struct pcmcvt *m)
{
    RFT_SetInPCMFormat(m->rft, m->input_bits);
    RFT_SetInChannel(m->rft, m->input_channels);
    RFT_SetInSigned(m->rft, m->input_sign);
    RFT_SetInEndian(m->rft, m->input_endian);
    RFT_SetOutPCMFormat(m->rft, m->output_bits);
    RFT_SetOutChannel(m->rft, m->output_channels);
    RFT_SetGenLFE(m->rft, m->generate_lfe);
    RFT_SetAcmod(m->rft, m->acmod);
    lgsrc_setInputFs(m->src, m->input_rate);
    lgsrc_setInOutPCMFormat(m->src, m->output_bits);
    lgsrc_setInOutChannel(m->src, m->output_channels);
    m->sample_shift = m->output_channels >> 1;
    lgsrc_setOutputFs(m->src, m->output_rate);
    m->input_bytes = m->input_bits >> 3;
    m->output_bytes = m->output_bits >> 3;
    m->src_bypass = src_formats_equal(m);
    lgsrc_setRunMode(m->src, m->running_mode == 1 ? 1 : 0);
    if (m->running_mode == 1)
        lgsrc_setFilterType(m->src, 0);
    return 0;
}

/* @0x6009a990: drains input, resets SRC, restores cached formats/acmod/LFE
 * (clearing LFE history), but not active gain, timestamps or sample totals. */
int Pcmcvt_Cmd_Flush(struct pcmcvt *m)
{
    flush_adapter(m, false);
    lgsrc_init(m->src);
    return Pcmcvt_SetParamToLibs(m);
}

/* @0x6009ae5c: cached flag only; not read by Chai's admission/processing path. */
int Pcmcvt_Cmd_SetOverprotect(struct pcmcvt *m, unsigned value)
{
    if (value > 1) return -1;
    m->over_protect = (int)value;
    return 0;
}

/* @0x6009aeb4
 * Changing rate clears SRC bypass even if the normalized rate is equal to the
 * input. Cache keeps the raw command rate; a later SetParamToLibs reapplies it.
 * An already-active rate returns before updating either cache or bypass, so a
 * previous CHANGE_FS can leave the property cache stale even after success. */
int Pcmcvt_SetOutFs(struct pcmcvt *m, int rate)
{
    if (rate < 1 || rate > 384000) return -1;
    if (rate != lgsrc_getOutputFs(m->src)) {
        lgsrc_setOutputFs(m->src, Pcmcvt_CheckSamplingRate(rate));
        m->output_rate = rate;
        m->src_bypass = 0;
    }
    return 0;
}

/* @0x6009af4c: only zero is rejected; SRC rejects >512 but its error is ignored,
 * so the property cache can disagree with the active output frame count. */
int Pcmcvt_Cmd_SetOutSample(struct pcmcvt *m, unsigned samples)
{
    if (samples == 0) return -1;
    lgsrc_setOutSample(m->src, samples);
    m->output_samples = samples;
    return 0;
}

/* @0x6009afb8 */
int Pcmcvt_Cmd_GenerateLFEMode(struct pcmcvt *m, int enable)
{
    RFT_SetGenLFE(m->rft, enable);
    m->generate_lfe = enable != 0;
    return 0;
}

/* @0x6009b034 */
int Pcmcvt_GetTotalSample(struct pcmcvt *m, int *input, int *output)
{
    if (m == NULL) return -1;
    *input = m->total_input;
    *output = m->total_output;
    return 0;
}

/* @0x6009b04c */
int Pcmcvt_GetFreq(struct pcmcvt *m, int *input, int *output)
{
    if (m == NULL) return -1;
    *input = lgsrc_getInputFs(m->src);
    *output = lgsrc_getOutputFs(m->src);
    return 0;
}

/* @0x6009b070: any value other than 1 selects mode 2, not an error. */
int Pcmcvt_SetMode(struct pcmcvt *m, int mode)
{
    if (m == NULL) return -1;
    lgsrc_setRunMode(m->src, mode == 1 ? 1 : 0);
    if (mode == 1) lgsrc_setFilterType(m->src, 0);
    m->running_mode = mode == 1 ? 1 : 2;
    return 0;
}

static int set_allfmt(struct pcmcvt *m, struct pcmcvt_allfmt *f)
{
    if (f->input_channels == 0 || f->input_channels > 10) return -1;
    if (f->output_channels == 0 || f->output_channels > 10) return -1;
    if (f->input_rate == 0 || f->input_rate > 384000) return -1;
    if (f->output_rate == 0 || f->output_rate > 384000) return -1;
    if (f->input_bits == 0 || f->input_bits > 32) {
        f->input_bits = 32;
        return -1;
    }
    if (f->output_bits == 0 || f->output_bits > 32) {
        f->output_bits = 32;
        return -1;
    }
    if (f->input_endian > 1 || f->input_sign > 1) return -1;
    m->input_sign = (int)f->input_sign;
    m->input_endian = (int)f->input_endian;
    m->input_channels = (int)f->input_channels;
    m->input_bits = (int)f->input_bits;
    m->input_rate = Pcmcvt_CheckSamplingRate((int)f->input_rate);
    m->output_channels = (int)f->output_channels;
    m->output_bits = (int)f->output_bits;
    m->output_rate = Pcmcvt_CheckSamplingRate((int)f->output_rate);
    return Pcmcvt_SetParamToLibs(m);
}

static int set_fmt(struct pcmcvt *m, struct pcmcvt_fmt *f)
{
    if (f->input_channels == 0 || f->input_channels > 10) return -1;
    if (f->output_channels == 0 || f->output_channels > 10) return -1;
    if (f->input_rate == 0 || f->input_rate > 384000) return -1;
    if (f->output_rate == 0 || f->output_rate > 384000) return -1;
    if (f->input_bits == 0 || f->input_bits > 32) {
        f->input_bits = 32;
        return -1;
    }
    if (f->output_bits == 0 || f->output_bits > 32) {
        f->output_bits = 32;
        return -1;
    }
    /* Legacy six-word SET_FMT changes active input without changing its cache.
     * FIRST/property update/flush can therefore restore the previous input. */
    RFT_SetInPCMFormat(m->rft, (int)f->input_bits);
    RFT_SetInChannel(m->rft, (int)f->input_channels);
    RFT_SetInSigned(m->rft, 0);
    RFT_SetInEndian(m->rft, 0);
    RFT_SetOutPCMFormat(m->rft, (int)f->output_bits);
    RFT_SetOutChannel(m->rft, (int)f->output_channels);
    lgsrc_setInputFs(m->src, Pcmcvt_CheckSamplingRate((int)f->input_rate));
    lgsrc_setInOutPCMFormat(m->src, (int)f->output_bits);
    lgsrc_setInOutChannel(m->src, (int)f->output_channels);
    m->sample_shift = (int)(f->output_channels >> 1);
    lgsrc_setOutputFs(m->src, Pcmcvt_CheckSamplingRate((int)f->output_rate));
    m->input_bytes = (int)(f->input_bits >> 3);
    m->output_bytes = (int)(f->output_bits >> 3);
    m->src_bypass = src_formats_equal(m);
    m->output_channels = (int)f->output_channels;
    m->output_bits = (int)f->output_bits;
    m->output_rate = Pcmcvt_CheckSamplingRate((int)f->output_rate);
    return 0;
}

/* @0x6009b0d8
 * SET_ALLFMT=0x50020020 has eight 32-bit words in the order in pcmcvt_allfmt.
 * Accepted command ranges: ch 1..10, fs 1..384000 (legacy normalization),
 * bits 1..32, endian/sign 0..1. RFT stores bits>>3; helpers only implement
 * 1/2/3/4-byte samples. Values 1..7 can produce zero-byte formats, and widths
 * not divisible by 8 are truncated. Helper errors are ignored, not repaired.
 * Invalid bit width writes 32 to the caller's parameter and returns -1 without
 * applying the cache. Successful allfmt caches then applies NOW @0x6009bd71,
 * not deferred until FIRST. len is not checked for these commands in firmware.
 * No command here sets event es/codec/data-rate or output aud_data type.
 * ENABLE_BYPASS=0x50090008 misleadingly only sets acmod (second argument).
 * SET_GAIN=0x50040008 reads [gain,enable] @6009b474/77 and calls RFT_SetGai
 * @6009b486; does not update cached m->gain or +1a5. Enable must equal 1.
 * INIT(10000000) resets SRC/accounting only; START/STOP/PAUSE/10070000 return
 * without resetting RFT. Default-command SET_PROP(10180004), via module_core
 * @60076444 ->600759cc, can reach gain/gain-enable in pcmcvt_properties below.
 * This establishes setter availability, NOT that a command was sent or gain
 * is enabled on the current TV. No live gain state was observed here.
 */
int Pcmcvt_Cmd(struct pcmcvt *m, uint32_t cmd, void *arg, unsigned len)
{
    uint32_t *p = arg;
    switch (cmd) {
    case 0x10000000:
        lgsrc_init(m->src);
        m->src_bypass = src_formats_equal(m);
        m->previous_ts = UINT32_MAX;
        m->total_input = m->total_output = 0;
        return 0;
    case 0x10010000:
    case 0x10020000:
    case 0x1003002c:
    case 0x10070000:
        return 0;
    case 0x50000018:
        return set_fmt(m, arg);
    case 0x50010004:
        return Pcmcvt_Cmd_SetOverprotect(m, *p);
    case 0x50020020:
        return set_allfmt(m, arg);
    case 0x50030008:
        if (p[0] == 0 || p[1] == 0) return -1;
        if ((int)p[0] != lgsrc_getInputFs(m->src))
            lgsrc_setInputFs(m->src, Pcmcvt_CheckSamplingRate((int)p[0]));
        if ((int)p[1] != lgsrc_getOutputFs(m->src))
            lgsrc_setOutputFs(m->src, Pcmcvt_CheckSamplingRate((int)p[1]));
        m->src_bypass = 0;
        return 0;
    case 0x50040008:
        RFT_SetGai(m->rft, (int)p[1], (int)p[0]);
        return 0;
    case 0x50050004:
        if (p == NULL) __assert_failed("Pcmcvt_Cmd", 0x395, "param != NULL");
        return Pcmcvt_SetOutFs(m, (int)*p);
    case 0x50060004:
        if (p == NULL) __assert_failed("Pcmcvt_Cmd", 0x3a2, "param != NULL");
        return Pcmcvt_SetMode(m, (int)*p);
    case 0x50080004:
        if (p == NULL) __assert_failed("Pcmcvt_Cmd", 0x3bc, "param != NULL");
        return Pcmcvt_Cmd_SetOutSample(m, *p);
    case 0x50090008:
        if (p == NULL) __assert_failed("Pcmcvt_Cmd", 0x3ca, "param != NULL");
        RFT_SetAcmod(m->rft, (int)p[1]);
        m->acmod = (int)p[1];
        return 0;
    case 0x500a0004:
        if (p == NULL) __assert_failed("Pcmcvt_Cmd", 0x3da, "param != NULL");
        return Pcmcvt_Cmd_GenerateLFEMode(m, *p != 0);
    default:
        return aud_module_default_command(m, cmd, arg, len);
    }
}

/* @0x6009be40
 * PCM(0) only; META_EXT(3) passes unchanged, ES(1)/META(2)/other types drop.
 * RFT ALWAYS runs, including equal-rate/channels/bits. SRC runs iff +0xfc!=1.
 * Output is newly allocated/copied and explicitly PCM(0), never ES.
 * 8ch/192000/native-endian/sign0/gain-disabled 16->16 takes RFT's byte copy
 * (0x6009cf5d..66), then SRC bypass. 16->32 still bypasses SRC but RFT writes
 * each signed sample left-aligned by 16 bits. Enabled gain is [FLIX] and is
 * not proven transparent even at nominal unity. See rft.c LGRFT.
 * Sample accounting uses shift=(out_channels>>1), literally: for 8ch this
 * divides sample count by 16, not 8. Preserve the firmware's arithmetic.
 * Original code does not check output allocation before dereferencing it.
 * map failure leaves queued bytes in place (and retries while enough remain).
 * Fault @6009bfc2 is signed timestamp division by lgsrc_getOutputFs(src),
 * not cached output_rate or channel count. In native-hd-fault-20261113-140842,
 * the saved divisor is zero despite cached 8ch/16bit/192000 formats; src's
 * context pointer and upstream pending slots are already damaged. This does
 * not identify the writer or justify hiding the shipped fault with a guard.
 */
int Pcmcvt_Chai(struct aud_port *port, struct aud_data *data)
{
    struct pcmcvt *m = port->module;
    void *src = m->src;
    if (data->type != 0) {
        if (data->type == 3) aud_module_push_data(m, 0, data);
        else aud_object_put(data);
        return 0;
    }
    aud_mutex_lock(&m->lock);
    aud_data_adapter_push(&m->adapter, data);
    aud_mutex_unlock(&m->lock);
    uint32_t timestamp = data->ts;
    int available, needed;
    do {
        available = (int)queued_bytes(m);
        int frames = m->src_bypass == 1 ? lgsrc_getOutSampleNum(src)
                                       : lgsrc_getNeedfulSamplePer1Ch(src);
        RFT_SetOutSample(m->rft, frames);
        needed = RFT_GetNeedfulSample(m->rft) * m->input_bytes;
        if (available < needed) return 0;
        void *current_src = m->src;
        uint32_t out_ts;
        if (m->previous_ts == timestamp) {
            int rate = lgsrc_getOutputFs(current_src);
            int samples = lgsrc_getOutSampleNum(current_src);
            out_ts = m->output_ts + (uint32_t)(samples * 90000 / rate);
        } else {
            out_ts = timestamp;
        }
        m->output_ts = out_ts;
        m->previous_ts = timestamp;
        aud_mutex_lock(&m->lock);
        void *input = aud_data_adapter_map(&m->adapter, (unsigned)needed);
        if (input == NULL) {
            aud_mutex_unlock(&m->lock);
        } else {
            int samples = LGRFT(m->rft, input, m->rft_buffer, m->src_buffer, m->available);
            aud_data_adapter_unmap(&m->adapter, input);
            aud_data_adapter_flush(&m->adapter, (unsigned)needed);
            aud_mutex_unlock(&m->lock);
            void *output = m->rft_buffer;
            m->last_input = samples >> (m->sample_shift & 31);
            m->total_input += m->last_input;
            if (m->src_bypass != 1) {
                samples = lgsrc_process(current_src, output, m->src_buffer);
                output = m->src_buffer;
            }
            m->last_output = samples >> (m->sample_shift & 31);
            m->total_output += m->last_output;
            if (samples > 0 && m->output_bytes > 0) {
                int bytes = samples * m->output_bytes;
                if (bytes > 0) {
                    struct aud_data *out = aud_data_new((unsigned)bytes);
                    out->ts = out_ts;
                    out->type = 0;
                    AuiLogger_AppendData(out->ptr, output, out->size);
                    aud_module_push_data(m, 0, out);
                    aud_module_sync(m, 0);
                }
            }
        }
    } while (available >= needed);
    return 0;
}

/* @0x6009c134: no initial SetParamToLibs; FIRST/allfmt establishes formats.
 * @c182..c19f caches 0x800000 but disables RFT. RFT_SetGai(disable) writes only
 * +18, NOT +1c: an existing raw gain word is retained, never initialized here. */
int Pcmcvt_Constructor(struct pcmcvt *m)
{
    m->src = lgsrc_create();
    lgsrc_init(m->src);
    lgsrc_setOutSample(m->src, 256);
    m->output_samples = 256;
    m->previous_ts = UINT32_MAX;
    m->input_channels = m->output_channels = m->acmod = m->running_mode = 2;
    m->input_rate = m->output_rate = 48000;
    m->input_bits = m->output_bits = 32;
    m->over_protect = m->unused_100 = m->input_endian = m->input_sign = 0;
    m->generate_lfe = m->gain_enable_property = false;
    m->gain = 0x800000;
    RFT_SetGai(m->rft, 0, m->gain);
    RFT_SetAcmod(m->rft, 0);
    m->rft_buffer = _BufMonitor_RemoveBufferIdxLocal(0xc800);
    m->src_buffer = _BufMonitor_RemoveBufferIdxLocal(0xc800);
    aud_data_adapter_init(&m->adapter);
    aud_mutex_init(&m->lock);
    return 0;
}

/* @0x6009c220 */
void Pcmcvt_Destructor(struct pcmcvt *m)
{
    _BufMonitor_RemoveBufferIndex(m->rft_buffer);
    _BufMonitor_RemoveBufferIndex(m->src_buffer);
    aud_data_adapter_cleanup(&m->adapter);
    lgsrc_Destroy(m->src);
}

/* @0x6009c264
 * FIRST DUPLICATES the original, overwrites only channels, bits-per-sample,
 * rate, acmod, check-downmix, then pushes the duplicate before final settings
 * application. es, codec, data-rate and other fields are not touched here.
 * Thus metadata can survive even though Chai drops ES buffers / emits PCM.
 * FIRST replaces input ch/bits/rate and acmod (default 2), not cached output
 * bits/rate, sign/endian or active gain. Final apply clears LFE history.
 * Input channels/bits/rate default -1 (leave cache). out-channels 1..10 also
 * updates output and triggers an early SetParamToLibs @0x6009c604.
 * Flush predicate compares input channels, out-channels and input RATE only,
 * not sample width: queued bytes can be reinterpreted at the new width.
 * Outgoing bits/rate are snapshots of the output cache at
 * entry, not read back from SRC or inferred from input/codec.
 */
int Pcmcvt_Event(struct pcmcvt *m, struct aud_port *port, struct aud_event *event)
{
    if (event->type != 1) {
        if (event->type == 7 && (event->prop & 2) != 0)
            Pcmcvt_Cmd_Flush(m);
        else if (event->type == 13 && aud_event_has_name(event, "change-out-rate")) {
            int input, output;
            Pcmcvt_GetFreq(m, &input, &output);
            int rate = aud_event_value_get_int(event, "change-out-rate", output);
            if (rate != output) Pcmcvt_SetOutFs(m, rate);
            aud_object_put(event);
            return 0;
        }
        return aud_module_default_event(m, port, event);
    }
    int output_bits = m->output_bits, output_rate = m->output_rate;
    int channels = aud_event_value_get_int(event, "channels", -1);
    int bits = aud_event_value_get_int(event, "bits-per-sample", -1);
    int rate = aud_event_value_get_int(event, "rate", -1);
    int output_channels = aud_event_value_get_int(event, "out-channels", -1);
    bool changed = (channels != -1 && channels != m->input_channels)
        || (output_channels != -1 && output_channels != m->output_channels)
        || (rate != -1 && rate != m->input_rate);
    if (changed) flush_adapter(m, true);
    if (channels != -1) m->input_channels = channels;
    if (bits != -1) m->input_bits = bits;
    if (rate != -1) m->input_rate = rate;
    if (output_channels > 0 && output_channels <= 10) {
        m->output_channels = output_channels;
        Pcmcvt_SetParamToLibs(m);
    }
    output_channels = m->output_channels;
    int acmod = aud_event_value_get_int(event, "acmod", 2);
    int downmix = aud_event_value_get_int(event, "check-downmix", -1);
    if (acmod == 0 && channels > 0 && channels < 3) {
        m->acmod = 2;
        acmod = 2;
    } else {
        m->acmod = acmod;
        if (output_channels == 2) acmod = 2;
        else if (channels >= 7 && output_channels == 6 && acmod >= 8) acmod = 7;
    }
    struct aud_event *out = aud_event_dup(event);
    if (out != NULL) {
        aud_event_value_set_int(out, "channels", output_channels);
        aud_event_value_set_int(out, "bits-per-sample", output_bits);
        aud_event_value_set_int(out, "rate", output_rate);
        aud_event_value_set_int(out, "acmod", acmod);
        aud_event_value_set_int(out, "check-downmix", downmix);
        aud_module_push_event(m, 0, out);
    }
    aud_object_put(event);
    Pcmcvt_SetParamToLibs(m);
    return 0;
}

static int parse_int(const char *s)
{
    return (int)(uint32_t)strtoul(s, NULL, 0);
}

static bool parse_bool(const char *s)
{
    return strcmp(s, "true") == 0 || strcmp(s, "yes") == 0 || strcmp(s, "1") == 0;
}

/* @0x6009c6d0 */
int set_prop_input_rate(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    int v = parse_int(s);
    if (v >= 1 && v <= 384000) { m->input_rate = v; Pcmcvt_SetParamToLibs(m); }
    return 0;
}
/* @0x6009c744 */
int set_prop_input_channels(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    int v = parse_int(s);
    if (v >= 1 && v <= 10) { m->input_channels = v; Pcmcvt_SetParamToLibs(m); }
    return 0;
}
/* @0x6009c7b4 */
int set_prop_input_bits(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    int v = parse_int(s);
    if (v >= 1 && v <= 32) { m->input_bits = v; Pcmcvt_SetParamToLibs(m); }
    return 0;
}
/* @0x6009c824 */
int set_prop_input_endian(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    int v = parse_int(s);
    if (v >= 0 && v <= 1) { m->input_endian = v; Pcmcvt_SetParamToLibs(m); }
    return 0;
}
/* @0x6009c890 */
int set_prop_input_sign(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    int v = parse_int(s);
    if (v >= 0 && v <= 1) { m->input_sign = v; Pcmcvt_SetParamToLibs(m); }
    return 0;
}
/* @0x6009c900 */
int set_prop_generate_lfe(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    return Pcmcvt_Cmd_GenerateLFEMode(m, parse_bool(s));
}
/* @0x6009c9c0 */
int set_prop_output_bits(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    int v = parse_int(s);
    if (v >= 1 && v <= 32) { m->output_bits = v; Pcmcvt_SetParamToLibs(m); }
    return 0;
}
/* @0x6009ca30 */
int set_prop_output_rate(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    return Pcmcvt_SetOutFs(m, parse_int(s));
}
/* @0x6009ca98 */
int set_prop_output_channels(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    int v = parse_int(s);
    if (v >= 1 && v <= 10) { m->output_channels = v; Pcmcvt_SetParamToLibs(m); }
    return 0;
}
/* @0x6009cb08 */
int set_prop_output_samples(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    return Pcmcvt_Cmd_SetOutSample(m, (unsigned)parse_int(s));
}
/* @0x6009cba0 */
int set_prop_over_protect(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    return Pcmcvt_Cmd_SetOverprotect(m, (unsigned)parse_int(s));
}
/* @0x6009cc28 */
int set_prop_runing_mode(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    return Pcmcvt_SetMode(m, parse_int(s));
}
/* @0x6009cc6c: property gain==0x800000 disables arithmetic, every other raw
 * value enables it. Unlike SET_GAIN this updates m->gain. Setting unity does
 * not overwrite the previous active RFT gain word because the setter disables. */
int set_prop_gain(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    m->gain = parse_int(s);
    RFT_SetGai(m->rft, m->gain != 0x800000, m->gain);
    return 0;
}
/* @0x6009ccf8: true/yes/1 enables even cached UNITY; all other strings disable.
 * Does not store +1a5. The property descriptor's getter offset is not evidence
 * that this setter maintains it. A later enable restores cached PROPERTY gain,
 * not the last gain passed through explicit SET_GAIN. */
int set_prop_gain_enable(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    RFT_SetGai(m->rft, parse_bool(s), m->gain);
    return 0;
}
/* @0x6009cd64: numeric parser despite BOOL descriptor; FIRST replaces acmod. */
int set_prop_acmod(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    int acmod = parse_int(s);
    RFT_SetAcmod(m->rft, acmod);
    m->acmod = acmod;
    return 0;
}
/* @0x6009cdb4: numeric nonzero, not true/yes/1; Chai passes it to LGRFT. */
int set_prop_available(struct pcmcvt *m, const void *prop, const char *s)
{
    (void)prop;
    m->available = parse_int(s) != 0;
    return 0;
}
/* @0x6009cdfc */
char *pcmcvt_gets_internal_buf_size(struct pcmcvt *m)
{
    return audutil_strdup_printf("(int)%u", queued_bytes(m));
}
/* Literal table @60103630..601038d7, reached through impl+30 @60103600.
 * Firmware descriptors are 0x28 bytes; this is a semantic host view. Offsets
 * are getter metadata, NOT stores performed before a custom set callback.
 * module_core set_prop @60075109..60075140 calls the callback instead of store.
 * Types: 1=BOOL, 2=INT; type 0 internal_buf_size uses only its custom getter. */
struct pcmcvt_property {
    const char *name;
    unsigned type, firmware_offset, flags;
    int (*set)(struct pcmcvt *, const void *, const char *);
    char *(*gets)(struct pcmcvt *);
};
const struct pcmcvt_property pcmcvt_properties[] = {
    {"in-sampling-rate", 2, 0x188, 0, set_prop_input_rate, NULL},
    {"in-channels", 2, 0x180, 0, set_prop_input_channels, NULL},
    {"in-bit-per-sample", 2, 0x184, 0, set_prop_input_bits, NULL},
    {"out-sampling-rate", 2, 0x19c, 0, set_prop_output_rate, NULL},
    {"out-channels", 2, 0x194, 0, set_prop_output_channels, NULL},
    {"out-bit-per-sample", 2, 0x198, 0, set_prop_output_bits, NULL},
    {"generate-lfe", 1, 0x1a4, 0, set_prop_generate_lfe, NULL},
    {"out-samples", 2, 0x1a0, 0, set_prop_output_samples, NULL},
    {"over-protect", 2, 0xf8, 0, set_prop_over_protect, NULL},
    {"running-mode", 2, 0x118, 0, set_prop_runing_mode, NULL},
    {"gain", 2, 0x1a8, 0, set_prop_gain, NULL},
    {"gain-enable", 1, 0x1a5, 0, set_prop_gain_enable, NULL},
    {"acmod", 1, 0x1ac, 0, set_prop_acmod, NULL},
    {"endian", 2, 0x18c, 0, set_prop_input_endian, NULL},
    {"sign", 2, 0x190, 0, set_prop_input_sign, NULL},
    {"available", 1, 0x1b0, 0, set_prop_available, NULL},
    {"internal_buf_size", 0, 0, 1, NULL, pcmcvt_gets_internal_buf_size},
    {NULL, 0, 0, 0, NULL, NULL}
};

/* @0x6009ce18 */
void Pcmcvt_Init(void)
{
    ModuleRegistry_Register(pcmcvt_impl_descriptor);
}
