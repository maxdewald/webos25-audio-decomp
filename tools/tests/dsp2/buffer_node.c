/* Standalone: cc -std=c11 -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined tools/tests/dsp2/buffer_node.c -o build/scratch/buffer-node-test && build/scratch/buffer-node-test */
#include "../../../src/dsp2/buffer_node.c"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static struct msf_node_metadata queued[3];
static unsigned get_count, put_count, fail_put, simd_calls, step_calls;
static void *put_connectors[8];
static struct msf_node_metadata *put_metadata[8];
static uint32_t expected_samples;
struct msf_node_metadata *connector_get_metadata(void *input, uint32_t samples)
{
    assert(input == queued && samples == expected_samples);
    return get_count < 3 ? &queued[get_count++] : NULL;
}
int connector_put_metadata(void *out, struct msf_node_metadata *md)
{
    assert(put_count < 8);
    put_connectors[put_count] = out;
    put_metadata[put_count++] = md;
    return put_count == fail_put;
}
int eval_node_activate(struct base_node *n) { assert(n); return 73; }
uint16_t eval_node_gain_step(uint32_t q)
{
    ++step_calls;
    assert(q == 68);
    return 0x1234;
}
int msf_convert_buffer_simd(const void *src, uint32_t ss, int32_t type,
                           void *dst, uint32_t ds, int32_t count, int32_t convert)
{
    assert(src && dst && ss == 2 && ds == 3 && count == 4);
    assert(type == 5 || (type == 4 && convert));
    ++simd_calls;
    return 91;
}
_Noreturn void _Assert(const char *message, int value)
{
    (void)message; (void)value;
    abort();
}
static void buffers(void)
{
    union { max_align_t align; unsigned char bytes[512]; } storage;
    memset(&storage, 0xa5, sizeof(storage));
    struct databuffer_config c = {3, 16, 4, 0};
    struct databuffer *b = (struct databuffer *)storage.bytes;
    uint32_t bytes;
    assert(databuffer_query_mem(&c, &bytes) == 0 && bytes == 171);
    assert(databuffer_open(&c, b) == b);
    assert(b->read.channels == 3 && b->write.channels == 3);
    assert(b->read.stride == 1 && b->write.stride == 1);
    assert(b->read.type == 4 && b->write.type == 4);
    assert(b->read.planes == (void **)(b + 1));
    assert((uintptr_t)b->write.planes % 8 == 0);
    assert((unsigned char *)b->write.planes >= (unsigned char *)(b->read.planes + 3));
    assert(((unsigned char *)b->read.planes)[0] == 0xa5);
    assert(databuffer_get_max_samples(b) == 16);
    assert(databuffer_get_write_buf(b, 16) == &b->write);
    assert(databuffer_get_write_buf(b, 17) == NULL);
    assert(databuffer_get_read_buf(b, 0) == &b->read);
    assert(databuffer_get_read_buf(b, 1) == NULL);
    assert(databuffer_update_write_buf(b, 7) == 0);
    assert(databuffer_get_avail_samples_write(b) == 9);
    assert(databuffer_get_avail_samples_read(b) == 7);
    assert(databuffer_update_write_buf(b, 10) == -32628 && b->samples == 7);
    assert(databuffer_update_read_buf(b, 4) == 0 && b->samples == 3);
    assert(databuffer_update_read_buf(b, 4) == -32629 && b->samples == 0);
    assert(databuffer_update_read_buf(NULL, 9) == 0);
    assert(databuffer_get_avail_samples_write(NULL) == 0);
    assert(databuffer_get_avail_samples_read(NULL) == 0);
    assert(databuffer_get_write_buf(NULL, 1) == NULL);
    assert(databuffer_get_write_buf(NULL, 0) == (struct msf_buffer *)(uintptr_t)0x18);
    assert(databuffer_get_read_buf(NULL, 0) == NULL);
    b->interleaved = 1;
    assert(databuffer_update_num_channels(b, 6) == 0);
    assert(b->read.channels == 6 && b->write.channels == 6);
    assert(b->read.stride == 6 && b->write.stride == 6);
    b->samples = UINT32_MAX;
    assert(databuffer_update_read_buf(b, -1) == 0 && b->samples == 0);
    assert(databuffer_update_write_buf(b, -1) == 0 && b->samples == UINT32_MAX);
}
static void nodes(void)
{
    struct base_node n = {0};
    union { max_align_t align; unsigned char bytes[256]; } storage;
    uint32_t bytes, out[4];
    n.activate = eval_node_activate;
    assert(base_node_query_mem(2, 3, &bytes) == 0 && bytes == 48);
    assert(base_node_open(&n, 2, 3, storage.bytes + 1) == 0);
    assert((uintptr_t)n.inputs % 8 == 0 && n.outputs == n.inputs + 2);
    assert(n.inputs[0].connector == NULL && n.outputs[2].type == 0);
    assert(node_activate(&n) == 73);
    assert(node_get_unconnected_input(&n, 6) == NULL);
    n.inputs[0].connector = &n;
    n.inputs[0].type = n.inputs[1].type = 6;
    assert(node_get_unconnected_input(&n, 6) == &n.inputs[1]);
    n.outputs[0].type = 4;
    n.outputs[1].type = n.outputs[2].type = 6;
    n.outputs[1].connector = &n;
    assert(node_get_unconnected_output(&n, 6) == &n.outputs[2]);
    node_set_delay(&n, 42);
    node_set_verbosity(&n, 9);
    assert(node_get_delay(&n) == 42 && node_get_delay(NULL) == 0 && n.verbosity == 9);
    assert(node_get_memory_usage(&n, out) == 0);
    for (unsigned i = 0; i < 4; ++i) {
        n.memory[i] = 1u << i;
        assert(node_get_memory_usage(&n, out) == 1);
        assert(memcmp(out, n.memory, sizeof(out)) == 0);
    }
    struct node_port *old_in = n.inputs, *old_out = n.outputs;
    base_node_open(&n, 0, 0, storage.bytes);
    assert(n.inputs == old_in && n.outputs == old_out && n.activate == eval_node_activate);
    assert(n.delay == 0 && n.verbosity == 0 && node_get_memory_usage(&n, out) == 0);
    assert(node_get_unconnected_input(&n, 6) == NULL);
    union { max_align_t align; unsigned char bytes[512]; } eval_storage;
    struct eval_node *e = (struct eval_node *)eval_storage.bytes;
    assert(eval_node_query_mem(&bytes) == 0 && bytes == 96);
    assert(eval_node_open(48000, 20, 30, 10, 0, e) == e);
    assert(e->initial_samples == 960 && e->hold_samples == 1440 && e->ramp_samples == 480);
    assert(e->remaining == 960 && e->gain == 0x7fff && e->step == 0x1234 && step_calls == 1);
    assert(e->state == 0 && e->reserved == 0);
    assert(e->base.inputs[0].type == 4 && e->base.outputs[0].type == 4);
    assert(node_activate(&e->base) == 73);
}
static void metadata(void)
{
    struct node_port outputs[3] = {{queued, 0}, {NULL, 0}, {&get_count, 0}};
    struct base_node n = {0};
    n.outputs = outputs;
    n.num_outputs = 3;
    expected_samples = 12;
    get_count = put_count = fail_put = 0;
    memset(queued, 0, sizeof(queued));
    queued[0].offset = UINT32_MAX;
    assert(base_node_pass_metadata(&n, queued, 12, 2) == 0);
    assert(get_count == 3 && put_count == 6 && queued[0].offset == 1 && queued[1].offset == 2);
    assert(put_connectors[0] == queued && put_connectors[1] == &get_count);
    assert(put_metadata[0] == &queued[0] && put_metadata[1] == &queued[0]);
    get_count = put_count = 0; fail_put = 2;
    assert(base_node_pass_metadata(&n, queued, 12, 0) == 0x81 && get_count == 1 && put_count == 2);
    get_count = put_count = fail_put = 0;
    n.num_outputs = 0;
    assert(base_node_pass_metadata(&n, queued, 12, 1) == 0 && get_count == 3 && put_count == 0);
    uint32_t md[7] = {0, 0, 0, 0, 0, 55, 0};
    msf_init_md(md, 1, 2, 3, 4, 5, 6);
    assert(md[0] == 1 && md[4] == 5 && md[5] == 55 && md[6] == 6);
}
static void conversions(void)
{
    static const uint32_t sizes[9] = {0, 1, 1, 0, 2, 4, 4, 4, 8};
    for (int32_t i = 0; i < 9; ++i) assert(msf_get_data_size(i) == sizes[i]);
    assert(msf_get_dlb_buffer_type(0) == 4 && msf_get_dlb_buffer_type(-1) == 6);
    uint32_t in[12] = {1, 2, 3, 4, 5, 6, 7};
    uint32_t out[12] = {0};
    assert(msf_convert_buffer(in, 2, 6, out, 3, 4, 1) == 0);
    assert(out[0] == 1 && out[3] == 3 && out[6] == 5 && out[9] == 7 && out[1] == 0);
    uint16_t in16[7] = {0xffff, 2, 0x8000, 4, 5, 6, 7}, out16[10] = {0};
    assert(msf_convert_buffer(in16, 2, 4, out16, 3, 4, 0) == 0);
    assert(out16[0] == 0xffff && out16[3] == 0x8000 && out16[6] == 5 && out16[9] == 7);
    assert(msf_convert_buffer(in, 2, 5, out, 3, 4, 0) == 91);
    assert(msf_convert_buffer(in, 2, 4, out, 3, 4, 1) == 91 && simd_calls == 2);
    assert(msf_convert_buffer(NULL, 2, 6, NULL, 3, 0, 0) == 0);
    assert(msf_convert_buffer(NULL, 2, 4, NULL, 3, -1, 0) == 0);
    assert(msf_convert_buffer(NULL, 2, 5, NULL, 3, 0, 1) == 0 && simd_calls == 2);
    assert(msf_convert_buffer(in, 0, 6, in, 0, 4, 0) == 0 && in[0] == 1);
}
int main(void)
{
    buffers(); nodes(); metadata(); conversions();
    puts("buffer_node: all tests passed");
    return 0;
}
