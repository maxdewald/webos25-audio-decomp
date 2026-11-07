#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @match src/dsp0/eform.c (?s)typedef struct eform_formatter \{.*?\} eform_port;

static eform *active;
static uint8_t *queued;
static uint32_t queued_size;
static unsigned teardown_stage;
static unsigned flush_count;
static int cleanup_requires_order;
static int mapped;
static int fail_allocation;
static int fail_output;
static int fail_map;
static int fail_get;
static int fail_create;
static unsigned create_count;
static unsigned destroy_count;
static unsigned forwarded;
static unsigned pushed;
static unsigned process_count;
static int32_t output_sizes[3];
static int32_t consumed_sizes[3];
static int fail_on_process_count;
static uint8_t expected_output[16];
static uint32_t expected_size;
static int expected_bypass;
static int32_t passing_byte;

static void aud_data_adapter_init(eform_adapter *adapter)
{
    memset(adapter, 0, sizeof(*adapter));
}

static void aud_data_adapter_flush(eform_adapter *adapter, uint32_t bytes)
{
    assert(adapter == &active->adapter);
    assert(!mapped);
    assert(bytes <= queued_size);
    if (queued != NULL)
        memmove(queued, queued + bytes, queued_size - bytes);
    queued_size -= bytes;
    adapter->flushed += bytes;
    ++flush_count;
}

static void aud_data_adapter_cleanup(eform_adapter *adapter)
{
    assert(adapter == &active->adapter);
    if (cleanup_requires_order) {
        assert(teardown_stage == 1);
        assert(queued_size == 3);
        assert(memcmp(queued, "def", 3) == 0);
        assert(*(uint8_t *)active->scratch == 0x5a);
    }
    aud_data_adapter_flush(adapter, adapter->written - adapter->flushed);
    free(queued);
    queued = NULL;
    queued_size = 0;
    teardown_stage = 2;
}

static void destroy_formatter(eform_formatter *formatter)
{
    if (cleanup_requires_order) {
        assert(teardown_stage == 0);
        assert(queued != NULL);
    }
    assert(formatter == active->formatter);
    teardown_stage = 1;
    ++destroy_count;
    free(formatter);
}

static int32_t process_formatter(eform_formatter *formatter, const void *input,
                                  const void **output, int32_t *remaining, int find)
{
    assert(formatter == active->formatter && mapped && find == 1);
    assert(input == queued + queued_size - *remaining);
    assert(process_count < 3 && consumed_sizes[process_count] <= *remaining);
    *output = input;
    *remaining -= consumed_sizes[process_count];
    return output_sizes[process_count++];
}

static eform_formatter *create_formatter(void)
{
    ++create_count;
    if (fail_create)
        return NULL;
    eform_formatter *formatter = calloc(1, sizeof(*formatter));
    assert(formatter != NULL);
    formatter->destroy = destroy_formatter;
    formatter->process = process_formatter;
    return formatter;
}

static eform_data *aud_data_new(uint32_t bytes)
{
    if (fail_output || (fail_on_process_count != 0 &&
                       process_count == (unsigned)fail_on_process_count))
        return NULL;
    eform_data *data = calloc(1, sizeof(*data) + bytes);
    assert(data != NULL);
    data->size = bytes;
    return data;
}

static void aud_data_adapter_push(eform_adapter *adapter, eform_data *data)
{
    assert(adapter == &active->adapter && queued == NULL);
    queued = malloc(data->size);
    assert(queued != NULL);
    memcpy(queued, data->payload, data->size);
    queued_size = data->size;
    adapter->written += data->size;
}

static eform_data *aud_data_adapter_get_data(eform_adapter *adapter, uint32_t bytes)
{
    assert(adapter == &active->adapter && bytes <= queued_size);
    if (fail_get)
        return NULL;
    eform_data *data = aud_data_new(bytes);
    if (data != NULL && bytes != 0)
        memcpy(data->payload, queued, bytes);
    return data;
}

static void *aud_data_adapter_map(eform_adapter *adapter, uint32_t bytes)
{
    assert(adapter == &active->adapter && bytes == queued_size && !mapped);
    if (fail_map)
        return NULL;
    mapped = 1;
    return queued;
}

static void aud_data_adapter_unmap(eform_adapter *adapter, void *mapping)
{
    assert(adapter == &active->adapter && mapping == queued && mapped);
    mapped = 0;
}

static int aud_module_push_data(eform *module, unsigned port, eform_data *data)
{
    assert(module == active && port == 0);
    assert(data->size == expected_size);
    assert(memcmp(data->payload, expected_output, expected_size) == 0);
    if (!module->bypass) {
        assert(data->flags == 1);
        assert(data->meta_014 == module->meta_128);
        assert(data->meta_018 == module->meta_124);
        assert(data->meta_020 == module->meta_138);
        assert(data->meta_024 == module->meta_13c);
    }
    ++pushed;
    free(data);
    if (fail_on_process_count != 0) {
        expected_output[0] = 'c';
        expected_output[1] = 'd';
    }
    return -17;
}

static int aud_event_value_get_bool(eform_event_data *event, const char *key, int fallback)
{
    assert(strcmp(key, "bypass_reform") == 0 && fallback == 0);
    return expected_bypass;
}

static int32_t aud_event_value_get_int(eform_event_data *event, const char *key, int32_t fallback)
{
    assert(strcmp(key, "passing-byte") == 0 && fallback == 0);
    return passing_byte;
}

static void aud_event_value_set_int(eform_event_data *event, const char *key, int32_t value)
{
    assert(strcmp(key, "passing-byte") == 0);
    passing_byte = value;
}

static int aud_module_default_event(eform *module, eform_port *port, eform_event_data *event)
{
    assert(module == active);
    ++forwarded;
    return -23;
}

static int aud_module_default_command(eform *module, uint32_t command, void *argument, uint32_t size)
{
    assert(module == active && command == 99 && argument == module && size == 4);
    return -24;
}

static void *constructor_malloc(size_t size)
{
    assert(size == 0x10000);
    return fail_allocation ? NULL : malloc(size);
}

static void destructor_free(void *allocation)
{
    assert(allocation == active->scratch && teardown_stage == 2);
    free(allocation);
}

// @extract src/dsp0/eform.c eform_flush eform_process_new eform_chain eform_event eform_cmd
#define malloc constructor_malloc
// @extract src/dsp0/eform.c eform_constructor
#undef malloc
#define free destructor_free
// @extract src/dsp0/eform.c eform_destructor
#undef free

static void test_flush(void)
{
    eform module = {0};
    active = &module;
    queued = malloc(6);
    assert(queued != NULL);
    memcpy(queued, "abcdef", 6);
    queued_size = 6;
    module.adapter.written = 6;
    eform_flush(NULL, 0);
    eform_flush(NULL, UINT32_MAX);
    eform_flush(NULL, UINT32_C(0x80000000));
    eform_flush(&module, 2);
    assert(queued_size == 4 && module.adapter.flushed == 2);
    assert(memcmp(queued, "cdef", 4) == 0);
    eform_flush(&module, 100);
    assert(queued_size == 0 && module.adapter.flushed == 6);
    free(queued);
    queued = NULL;
}

static void test_destructor(int has_formatter)
{
    eform module = {0};
    active = &module;
    queued = malloc(6);
    module.scratch = malloc(1);
    assert(queued != NULL && module.scratch != NULL);
    memcpy(queued, "abcdef", 6);
    *(uint8_t *)module.scratch = 0x5a;
    queued_size = 6;
    module.adapter.written = 6;
    teardown_stage = has_formatter ? 0 : 1;
    if (has_formatter) {
        module.formatter = calloc(1, sizeof(*module.formatter));
        assert(module.formatter != NULL);
        module.formatter->destroy = destroy_formatter;
    }
    eform_flush(&module, 3);
    cleanup_requires_order = 1;
    eform_destructor(&module);
    assert(teardown_stage == 2 && queued == NULL);
    assert(module.adapter.written == 6 && module.adapter.flushed == 6);
    cleanup_requires_order = 0;
}

static void test_eform_events(void)
{
    eform_factory factory = {"test", 77, create_formatter};
    eform_class klass = { .factory = &factory };
    eform module;
    memset(&module, 0xa5, sizeof(module));
    module.klass = &klass;
    active = &module;
    fail_allocation = 1;
    assert(eform_constructor(&module) == 16);
    assert(module.formatter == NULL && module.scratch == NULL);
    assert(module.adapter.written == UINT32_C(0xa5a5a5a5));
    fail_allocation = 0;
    assert(eform_constructor(&module) == 0);
    assert(module.formatter == NULL && module.factory == &factory);
    assert(module.scratch_capacity == 0x10000 && module.factory_value == 77);
    assert(module.adapter.written == 0 && module.adapter.flushed == 0);
    queued = malloc(6);
    assert(queued != NULL);
    memcpy(queued, "abcdef", 6);
    queued_size = 6;
    module.adapter.written = 6;
    module.input_bytes = 5;
    eform_event_data event = { .type = 7 };
    unsigned before = flush_count;
    assert(eform_event(&module, NULL, &event) == -23);
    assert(flush_count == before && queued_size == 6);
    event.flags = 2;
    assert(eform_event(&module, NULL, &event) == -23);
    assert(queued_size == 1 && module.input_bytes == 5);
    event.type = 1;
    expected_bypass = 1;
    assert(eform_event(&module, NULL, &event) == -23);
    assert(module.formatter != NULL && module.bypass == 1);
    assert(module.input_bytes == 0 && queued_size == 1);
    assert(module.total_input_bytes == 0 && module.state_11c == 1);
    before = destroy_count;
    fail_create = 1;
    assert(eform_event(&module, NULL, &event) == -23);
    assert(destroy_count == before + 1 && module.formatter == NULL);
    fail_create = 0;
    event.type = 7;
    before = flush_count;
    assert(eform_event(&module, NULL, &event) == -23);
    assert(flush_count == before && queued_size == 1);
    event.type = 9;
    passing_byte = 40;
    assert(eform_event(&module, NULL, &event) == -23);
    assert(passing_byte == 41);
    assert(eform_cmd(&module, 0x10010000, NULL, 0) == 0);
    assert(eform_cmd(&module, 0x10020000, NULL, 0) == 0);
    assert(eform_cmd(&module, 0x1003002c, NULL, 0) == 0);
    assert(eform_cmd(&module, 99, &module, 4) == -24);
    eform_destructor(&module);
}

static void test_processing(void)
{
    eform module = {0};
    active = &module;
    queued = malloc(6);
    assert(queued != NULL);
    memcpy(queued, "abcdef", 6);
    queued_size = 6;
    module.adapter.written = 6;
    module.meta_124 = 12;
    module.meta_128 = 34;
    module.meta_138 = 56;
    module.meta_13c = 78;
    fail_get = 1;
    assert(eform_process_new(&module) == -1 && queued_size == 6);
    fail_get = 0;
    memcpy(expected_output, "abcdef", 6);
    expected_size = 6;
    assert(eform_process_new(&module) == 0 && queued_size == 0);
    memcpy(queued, "abcdef", 6);
    queued_size = 6;
    module.adapter.written = 12;
    module.formatter = create_formatter();
    fail_map = 1;
    assert(eform_process_new(&module) == -1 && !mapped && queued_size == 6);
    fail_map = 0;
    process_count = 0;
    output_sizes[0] = 2;
    consumed_sizes[0] = 2;
    fail_output = 1;
    unsigned before = flush_count;
    assert(eform_process_new(&module) == -1 && !mapped && queued_size == 6);
    assert(flush_count == before && process_count == 1);
    fail_output = 0;
    process_count = 0;
    output_sizes[1] = 2;
    consumed_sizes[1] = 2;
    fail_on_process_count = 2;
    memcpy(expected_output, "ab", 2);
    expected_size = 2;
    before = flush_count;
    assert(eform_process_new(&module) == -1 && process_count == 2);
    assert(queued_size == 6 && flush_count == before && !mapped);
    fail_on_process_count = 0;
    process_count = 0;
    output_sizes[1] = 0;
    consumed_sizes[1] = 1;
    memcpy(expected_output, "ab", 2);
    expected_size = 2;
    assert(eform_process_new(&module) == 0 && !mapped);
    assert(queued_size == 3 && memcmp(queued, "def", 3) == 0);
    process_count = 0;
    output_sizes[0] = -5;
    consumed_sizes[0] = 1;
    assert(eform_process_new(&module) == -5 && queued_size == 2 && !mapped);
    eform_destructor(&module);
}

static void test_chain(void)
{
    eform module = {0};
    eform_port port = { .module = &module };
    active = &module;
    expected_size = 3;
    memcpy(expected_output, "xyz", 3);
    eform_data *input = aud_data_new(3);
    memcpy(input->payload, "xyz", 3);
    input->meta_014 = 10;
    input->meta_018 = 20;
    input->meta_020 = 30;
    input->meta_024 = 40;
    assert(eform_chain(&port, input) == 0);
    assert(module.input_bytes == 3 && module.total_input_bytes == 3);
    assert(module.meta_128 == 10 && module.meta_124 == 20);
    assert(module.meta_138 == 30 && module.meta_13c == 40);
    free(input);
    eform_destructor(&module);
    module.bypass = 1;
    input = aud_data_new(3);
    memcpy(input->payload, "xyz", 3);
    unsigned before = pushed;
    assert(eform_chain(&port, input) == 0 && pushed == before + 1);
    assert(queued == NULL && module.input_bytes == 3);
}

static void test_retry_boundary(void)
{
    for (unsigned bytes = 256; bytes <= 257; ++bytes) {
        eform module = {0};
        eform_port port = { .module = &module };
        active = &module;
        module.formatter = create_formatter();
        process_count = 0;
        output_sizes[0] = -1;
        consumed_sizes[0] = 0;
        output_sizes[1] = 0;
        consumed_sizes[1] = 1;
        eform_data *input = aud_data_new(bytes);
        assert(eform_chain(&port, input) == 0);
        assert(process_count == (bytes == 256 ? 1u : 2u));
        assert(queued_size == 256);
        free(input);
        eform_destructor(&module);
    }
}

// @match src/dsp0/decoderbin.c (?s)typedef struct decoderbin decoderbin;.*?\n};

static unsigned routes;
static unsigned command_targets;
static unsigned pending_drains;
static unsigned dropped_events;
static unsigned parent_events;
static unsigned codec_updates;
static unsigned children_destroyed;
static void *inputs[256];
static void *outputs[256];
static unsigned output_ports[256];

static void aud_object_put(decoderbin_event_data *event)
{
    ++dropped_events;
    free(event);
}

static const char *aud_event_value_get_str(decoderbin_event_data *event,
                                          const char *key, const char *fallback)
{
    assert(strcmp(key, "codec-desc") == 0 && fallback == NULL);
    return "test-codec";
}

static void add_decoder_module(decoderbin *module, const char *codec)
{
    assert(strcmp(codec, "test-codec") == 0);
    ++codec_updates;
}

static int aud_module_bin_set_input(decoderbin *module, unsigned port,
                                    void *child, unsigned child_port)
{
    assert(!module->connected && port < 256 && child_port == 0);
    inputs[port] = child;
    ++routes;
    return -9;
}

static int aud_module_bin_set_output(decoderbin *module, unsigned port,
                                     void *child, unsigned child_port)
{
    assert(!module->connected && port < 256);
    outputs[port] = child;
    output_ports[port] = child_port;
    ++routes;
    return -9;
}

static void aud_module_bin_add_cmd_propagate(decoderbin *module, void *child)
{
    assert(!module->connected);
    assert(child == module->decoder || child == module->formatter);
    ++command_targets;
}

static void aud_module_bin_propagate_pending(decoderbin *module);

// @extract src/dsp0/decoderbin.c decoderbin_event decoderbin_constructor decoderbin_destructor decoderbin_connect

static void aud_module_bin_propagate_pending(decoderbin *module)
{
    assert(!module->connected && routes >= 3 && command_targets != 0);
    decoderbin_event_data *event = calloc(1, sizeof(*event));
    assert(event != NULL);
    event->type = 1;
    unsigned before = dropped_events;
    assert(decoderbin_event(module, module, event) == 0);
    assert(dropped_events == before + 1);
    ++pending_drains;
}

static int parent_event(decoderbin *module, void *port, decoderbin_event_data *event)
{
    ++parent_events;
    free(event);
    return -31;
}

static void parent_cleanup(decoderbin *module)
{
    assert(module->connected == 1);
    assert(strcmp(module->codec_name, "test-codec") == 0);
    assert(*(uint8_t *)module->decoder == 11);
    free(module->decoder);
    ++children_destroyed;
    if (module->formatter != NULL) {
        assert(*(uint8_t *)module->formatter == 22);
        free(module->formatter);
        ++children_destroyed;
    }
    free(module->output_124);
    free(module->output_128);
}

static void test_decoderbin(int dummy, int has_formatter)
{
    decoderbin_class klass = { .cleanup = parent_cleanup, .event = parent_event };
    decoderbin module = { .klass = &klass };
    assert(decoderbin_constructor(&module) == 0);
    routes = command_targets = pending_drains = 0;
    memset(inputs, 0, sizeof(inputs));
    memset(outputs, 0, sizeof(outputs));
    assert(decoderbin_connect(&module, NULL, 0) == 0);
    assert(decoderbin_connect(&module, NULL, 1) == -1);
    assert(routes == 0 && !module.connected);
    module.decoder = malloc(1);
    assert(module.decoder != NULL);
    *(uint8_t *)module.decoder = 11;
    if (has_formatter) {
        module.formatter = malloc(1);
        assert(module.formatter != NULL);
        *(uint8_t *)module.formatter = 22;
    }
    module.dummy_codec = dummy;
    if (dummy) {
        module.output_124 = malloc(1);
        module.output_128 = malloc(1);
        assert(module.output_124 != NULL && module.output_128 != NULL);
    }
    assert(decoderbin_connect(&module, NULL, 1) == 0);
    assert(module.connected == 1 && pending_drains == 1);
    assert(inputs[0] == (has_formatter ? module.formatter : module.decoder));
    assert(command_targets == 1u + has_formatter);
    if (dummy) {
        assert(routes == 6 && outputs[0xfe] == module.decoder);
        assert(inputs[0xfe] == module.output_124 && inputs[0xff] == module.output_128);
        assert(outputs[0] == module.output_124 && outputs[1] == module.output_128);
        assert(output_ports[1] == 0);
    } else {
        assert(routes == 3 && outputs[0] == module.decoder && outputs[1] == module.decoder);
        assert(output_ports[0] == 0 && output_ports[1] == 1);
    }
    unsigned before = routes;
    assert(decoderbin_connect(&module, NULL, 1) == 0);
    assert(decoderbin_connect(&module, NULL, 0) == 0);
    assert(routes == before && module.connected == 1);
    for (unsigned index = 0; index < 3; ++index) {
        decoderbin_event_data *event = calloc(1, sizeof(*event));
        assert(event != NULL);
        event->type = index == 2 ? 6 : 1;
        before = codec_updates;
        assert(decoderbin_event(&module, index == 0 ? NULL : &module, event) == -31);
        assert(codec_updates == before + (index == 1));
        assert(module.connected == 1 && children_destroyed == 0);
    }
    module.codec_name = strdup("test-codec");
    assert(module.codec_name != NULL);
    module.previous_dummy_codec = 1;
    module.workerbin_uid = 123;
    decoderbin_destructor(&module);
    assert(children_destroyed == 1u + has_formatter);
    assert(module.codec_name == NULL && module.decoder == NULL && module.formatter == NULL);
    assert(module.output_124 == NULL && module.output_128 == NULL);
    assert(module.dummy_codec == 0 && module.previous_dummy_codec == 0);
    assert(module.workerbin_uid == 0 && module.connected == 1);
    children_destroyed = 0;
}

int main(void)
{
    test_flush();
    test_destructor(1);
    test_destructor(0);
    test_eform_events();
    test_processing();
    test_chain();
    test_retry_boundary();
    test_decoderbin(0, 0);
    test_decoderbin(0, 1);
    test_decoderbin(1, 1);
    puts("DSP0 flush, START/failure ownership, processing cleanup, connect/event/teardown ordering");
    return 0;
}