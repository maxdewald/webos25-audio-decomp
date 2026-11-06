/* DSP2 converter plumbing. Evidence: saved main image, entry-aligned Xtensa
 * listings and xtlist literal resolution for 0x6008e8f8..0x6008f298.
 * Host semantic structures, NOT firmware ABI layouts. Pointer tables and port
 * arrays use native pointer widths; query_mem still reports firmware bytes.
 * Diagnostic logging is omitted. Counts/strides must describe valid storage.
 * [FLIX] eval_node_activate, evaluation gain-step initialization, conversion
 * SIMD paths and msf_debug_metadata remain explicit external dependencies.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct msf_buffer {
    uint32_t channels, stride, type;
    void **planes;                 /* firmware descriptor +0xc */
};
struct databuffer {
    struct msf_buffer read;         /* firmware +0 */
    uint32_t max_samples, samples;  /* +0x10, +0x14 */
    struct msf_buffer write;        /* +0x18 */
    uint32_t interleaved;           /* +0x28 */
};
struct databuffer_config { uint32_t channels, max_samples, type, interleaved; };
struct node_port { void *connector; uint32_t type; };
struct base_node {
    int (*activate)(struct base_node *);
    int32_t num_inputs, num_outputs;
    struct node_port *inputs, *outputs;
    uint32_t verbosity, memory[4], delay;
};
struct eval_node {
    struct base_node base;
    uint32_t initial_samples, hold_samples, ramp_samples, reserved;
    uint16_t gain, step;
    uint32_t state, remaining;
};
struct msf_node_metadata { uint32_t type, length, offset, value[4]; };
extern struct msf_node_metadata *connector_get_metadata(void *, uint32_t);
extern int connector_put_metadata(void *, struct msf_node_metadata *);
extern int eval_node_activate(struct base_node *);
/* [FLIX] 0x6008ee0f/14 ae_movda16/ae_movad16.0 gain-step semantics. */
extern uint16_t eval_node_gain_step(uint32_t quotient);
extern int msf_convert_buffer_simd(const void *, uint32_t, int32_t, void *,
                                   uint32_t, int32_t, int32_t);
extern void msf_debug_metadata(const void *, uint32_t, uint32_t);
extern _Noreturn void _Assert(const char *, int);

/* @0x6008efb0: table at image VA 0x600056ec; negative types are invalid too. */
uint32_t msf_get_data_size(int32_t type)
{
    static const uint32_t sizes[9] = {0, 1, 1, 0, 2, 4, 4, 4, 8};
    if (type > 8)
        _Assert("../xt_dsp/src/msf/msf_v24/src/msf_pvt.c:60 data_type <= DLB_BUFFER_DOUBLE", 0);
    return sizes[type];
}
/* @0x6008efd0 */
uint32_t msf_get_dlb_buffer_type(int32_t type) { return type ? 6u : 4u; }

/* @0x6008e9e0 */
int32_t databuffer_get_avail_samples_write(const struct databuffer *b)
{
    return b ? (int32_t)(b->max_samples - b->samples) : 0;
}
/* @0x6008e9f4 */
int32_t databuffer_get_avail_samples_read(const struct databuffer *b)
{
    return b ? (int32_t)b->samples : 0;
}
/* @0x6008e8f8: neither getter advances or initializes the plane pointers. */
struct msf_buffer *databuffer_get_write_buf(struct databuffer *b, int32_t n)
{
    if (databuffer_get_avail_samples_write(b) < n) return NULL;
    return b ? &b->write : (struct msf_buffer *)(uintptr_t)0x18;
}
/* @0x6008e90c */
struct msf_buffer *databuffer_get_read_buf(struct databuffer *b, int32_t n)
{
    return databuffer_get_avail_samples_read(b) < n ? NULL : (struct msf_buffer *)b;
}
/* @0x6008e920 */
int databuffer_update_num_channels(struct databuffer *b, uint32_t n)
{
    b->read.channels = b->write.channels = n;
    b->read.stride = b->write.stride = b->interleaved ? n : 1u;
    return 0;
}
/* @0x6008e944: overfill leaves the previous sample count unchanged. */
int databuffer_update_write_buf(struct databuffer *b, int32_t n)
{
    if (n > databuffer_get_avail_samples_write(b)) return -32628;
    b->samples += (uint32_t)n;
    return 0;
}
/* @0x6008e988: over-read empties the count; NULL is a successful no-op. */
int databuffer_update_read_buf(struct databuffer *b, int32_t n)
{
    if (!b) return 0;
    if (n > databuffer_get_avail_samples_read(b)) {
        b->samples = 0;
        return -32629;
    }
    b->samples -= (uint32_t)n;
    return 0;
}
/* @0x6008e9d8 */
uint32_t databuffer_get_max_samples(const struct databuffer *b) { return b->max_samples; }
/* @0x6008ea04 */
int databuffer_query_mem(const struct databuffer_config *c, uint32_t *bytes)
{
    *bytes = c->channels * 8u + msf_get_data_size((int32_t)c->type) *
        c->max_samples * c->channels + 0x33u;
    return 0;
}
static void *buffer_align8(void *p)
{
    return (void *)(((uintptr_t)p + 7u) & ~(uintptr_t)7u);
}
/* @0x6008ea28: initializes descriptors only; plane entries are untouched.
 * Host storage: databuffer + channels native pointers + 7-byte alignment slack
 * + channels native pointers. Firmware uses +0x2c and align8(base+4*ch+0x33). */
struct databuffer *databuffer_open(const struct databuffer_config *c, struct databuffer *b)
{
    b->read.planes = (void **)(b + 1);
    b->write.planes = buffer_align8((unsigned char *)b->read.planes +
                                   c->channels * sizeof(void *));
    b->interleaved = c->interleaved;
    b->read.type = b->write.type = c->type;
    databuffer_update_num_channels(b, c->channels);
    b->max_samples = c->max_samples;
    b->samples = 0;
    return b;
}
/* @0x6008ee88 */
int base_node_query_mem(uint32_t inputs, uint32_t outputs, uint32_t *bytes)
{
    *bytes = (inputs + outputs) * 8u + 8u;
    return 0;
}
/* @0x6008ee98: zero counts preserve their old array pointers and callback. */
int base_node_open(struct base_node *n, int32_t inputs, int32_t outputs, void *storage)
{
    struct node_port *p = buffer_align8(storage);
    n->num_inputs = inputs;
    n->num_outputs = outputs;
    if (inputs > 0) {
        n->inputs = p;
        memset(p, 0, (size_t)inputs * sizeof(*p));
        p += inputs;
    }
    if (outputs > 0) {
        n->outputs = p;
        memset(p, 0, (size_t)outputs * sizeof(*p));
    }
    n->verbosity = n->delay = 0;
    memset(n->memory, 0, sizeof(n->memory));
    return 0;
}
/* @0x6008ed94 */
int eval_node_query_mem(uint32_t *bytes)
{
    base_node_query_mem(1, 1, bytes);
    *bytes += 0x48u;
    return 0;
}
/* @0x6008edb0: gain-step conversion delegated; zero ramp reaches the target's
 * unsigned divide-by-zero behavior, so callers must supply a nonzero ramp. */
struct eval_node *eval_node_open(uint32_t rate, uint32_t initial_ms,
                                uint32_t hold_ms, uint32_t ramp_ms,
                                int32_t type, struct eval_node *e)
{
    memset(e, 0, sizeof(*e));
    base_node_open(&e->base, 1, 1, e + 1);
    e->gain = 0x7fff;
    e->initial_samples = rate * initial_ms / 1000u;
    e->hold_samples = rate * hold_ms / 1000u;
    e->ramp_samples = rate * ramp_ms / 1000u;
    e->remaining = e->initial_samples;
    e->step = eval_node_gain_step(0x8000u / e->ramp_samples);
    e->base.inputs[0].type = msf_get_dlb_buffer_type(type);
    e->base.outputs[0].type = msf_get_dlb_buffer_type(type);
    e->base.activate = eval_node_activate;
    return e;
}
/* @0x6008ee38: symbol table truncates node_* names to ode_*. */
int node_activate(struct base_node *n) { return n->activate(n); }
static struct node_port *node_unconnected(struct node_port *p, int32_t count, uint32_t type)
{
    for (int32_t i = 0; i < count; ++i)
        if (!p[i].connector && p[i].type == type) return &p[i];
    return NULL;
}
/* @0x6008ee48: hardware-loop search, not just the first port. */
struct node_port *node_get_unconnected_input(struct base_node *n, uint32_t type)
{
    return node_unconnected(n->inputs, n->num_inputs, type);
}
/* @0x6008ee68 */
struct node_port *node_get_unconnected_output(struct base_node *n, uint32_t type)
{
    return node_unconnected(n->outputs, n->num_outputs, type);
}
/* @0x6008eee0: the same metadata pointer is passed to every connected output;
 * error 0x81 stops immediately. No ownership/release is added on empty fan-out. */
int base_node_pass_metadata(struct base_node *n, void *input, uint32_t samples, uint32_t delay)
{
    struct msf_node_metadata *md;
    while ((md = connector_get_metadata(input, samples)) != NULL) {
        md->offset += delay;
        for (int32_t i = 0; i < n->num_outputs; ++i)
            if (n->outputs[i].connector && connector_put_metadata(n->outputs[i].connector, md))
                return 0x81;
    }
    return 0;
}
/* @0x6008ef58 */
uint32_t node_get_delay(const struct base_node *n) { return n ? n->delay : 0; }
/* @0x6008ef68 */
void node_set_verbosity(struct base_node *n, uint32_t v) { n->verbosity = v; }
/* @0x6008ef70 */
int node_get_memory_usage(const struct base_node *n, uint32_t out[4])
{
    uint32_t any = 0;
    for (unsigned i = 0; i < 4; ++i) { out[i] = n->memory[i]; any |= n->memory[i]; }
    return any != 0;
}
/* @0x6008efa8 */
void node_set_delay(struct base_node *n, uint32_t delay) { n->delay = delay; }
/* @0x6008efe0: scalar paths only; type 4 is 16-bit when conversion is off,
 * types other than 4/5 copy words irrespective of the conversion flag. */
int msf_convert_buffer(const void *src, uint32_t src_stride, int32_t type, void *dst,
                       uint32_t dst_stride, int32_t count, int32_t convert)
{
    if (count <= 0) return 0;
    if (type == 5 || (type == 4 && convert))
        return msf_convert_buffer_simd(src, src_stride, type, dst, dst_stride, count, convert);
    size_t width = type == 4 ? 2u : 4u;
    const unsigned char *in = src;
    unsigned char *out = dst;
    for (int32_t i = 0; i < count; ++i) {
        unsigned char sample[4];
        memcpy(sample, in, width);
        memcpy(out, sample, width);
        if (i + 1 < count) {
            in += src_stride * width;
            out += dst_stride * width;
        }
    }
    return 0;
}
/* @0x6008f0b4: word five intentionally survives initialization. */
void msf_init_md(uint32_t *md, uint32_t a, uint32_t b, uint32_t c,
                 uint32_t d, uint32_t e, uint32_t f)
{
    md[0] = a; md[1] = b; md[2] = c; md[3] = d; md[4] = e; md[6] = f;
}
