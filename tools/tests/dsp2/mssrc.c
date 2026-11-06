/* Host mocks check recovered control flow/ownership, NOT MS12 DSP conversion. */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../src/dsp2/mssrc.c"

struct value { const char *key; int value; };
struct event { struct aud_event base; struct value values[16]; unsigned count; };
static struct event duplicates[8];
static struct aud_event *forwarded;
static struct aud_port *forwarded_port;
static void *released[16];
static unsigned releases, dup_calls, fail_dup_at, flushes, metadata_flushes;
static unsigned opens, connects, closes, cleanups, assertions, defaults, debug_calls;
static unsigned metadata_prints, initialized_memory, initialized_adapter;
static unsigned fail_open, fail_connect, fail_close;
static unsigned last_flush, default_size;
static uint32_t default_command;
static void *default_payload;
static char operations[128];
static unsigned operation_count;
static uint32_t mask, ipc_id;
volatile uint32_t *mssrc_ipc_log_mask = &mask;
uint32_t *mssrc_ipc_id = &ipc_id;
static struct aud_data packets[8];
static unsigned packet_count, pushes, metadata_allocs, pcm_allocs;
static bool fail_metadata, fail_pcm;
static unsigned char pcm_bytes[32], output_bytes[32];
static const struct mssrc_implementation *registered;
static struct mssrc *active;
static struct aud_data chunk, *adapter_head;
static unsigned char input_bytes[512], observed_bytes[512];
static unsigned adapter_pushes, reads, peeps, runs, library_inits, metadata_runs;
static unsigned needed_bytes, produced_bytes, observed_ratio, run_limit;
static int run_result, library_init_result, metadata_result;
static bool fail_read, initialize_format, no_head;
static unsigned run_sizes[8];

static void op(char c) { assert(operation_count + 1 < sizeof(operations)); operations[operation_count++] = c; operations[operation_count] = 0; }
static void reset(void)
{
    releases = dup_calls = fail_dup_at = flushes = metadata_flushes = 0;
    opens = connects = closes = cleanups = assertions = defaults = debug_calls = 0;
    metadata_prints = initialized_memory = initialized_adapter = 0;
    fail_open = fail_connect = fail_close = 0;
    packet_count = pushes = metadata_allocs = pcm_allocs = 0;
    fail_metadata = fail_pcm = false;
    forwarded = NULL; forwarded_port = NULL;
    operation_count = 0; operations[0] = 0;
    mask = ipc_id = 0;
    adapter_pushes = reads = peeps = runs = library_inits = metadata_runs = 0;
    needed_bytes = 32; produced_bytes = 32; observed_ratio = 0; run_limit = 8;
    run_result = metadata_result = 0; library_init_result = 4;
    fail_read = no_head = false; initialize_format = true; adapter_head = NULL;
    memset(run_sizes, 0, sizeof(run_sizes));
}
void aud_data_adapter_init(struct mssrc_adapter *a) { memset(a, 0, sizeof(*a)); ++initialized_adapter; }
void aud_data_adapter_cleanup(struct mssrc_adapter *a) { (void)a; ++cleanups; op('D'); }
void aud_data_adapter_flush(struct mssrc_adapter *a, unsigned n) { a->flushed += n; last_flush = n; ++flushes; op('F'); }
void AuiLogger_Init(void *p, int value, unsigned n) { assert(n == 0x100 && value == 0); memset(p, value, n); ++initialized_memory; }
void AuiLogger_AppendData(void *out, const void *in, unsigned n) { memcpy(out, in, n); }
int ms_src_main_process_open(void *p) { assert(p); ++opens; op('O'); return (int)fail_open; }
int ms_src_main_connect_msf(void *p) { assert(p); ++connects; op('C'); return (int)fail_connect; }
int ms_src_main_process_close(void *p) { assert(p); ++closes; op('X'); return (int)fail_close; }
void msf_flush_metadata(void *p) { assert(p); ++metadata_flushes; op('M'); }
void msf_print_metadata(void *p) { assert(p); ++metadata_prints; }
void __assert_failed(const char *fn, unsigned line, const char *what) { assert(fn && line && what); ++assertions; }
void aud_object_put(void *p) { assert(releases < 16); released[releases++] = p; }
int aud_module_default_command(struct mssrc *m, uint32_t cmd, void *p, unsigned n)
{ (void)m; ++defaults; default_command = cmd; default_payload = p; default_size = n; return 23; }
int aud_module_default_event(struct mssrc *m, struct aud_port *port, struct aud_event *e)
{ (void)m; forwarded = e; forwarded_port = port; ++defaults; op('E'); return 19; }
int aud_event_value_get_int(struct aud_event *base, const char *key, int fallback)
{
    struct event *e = (struct event *)base;
    for (unsigned i = 0; i < e->count; ++i)
        if (!strcmp(e->values[i].key, key)) return e->values[i].value;
    return fallback;
}
void aud_event_value_set_int(struct aud_event *base, const char *key, int value)
{
    struct event *e = (struct event *)base;
    for (unsigned i = 0; i < e->count; ++i)
        if (!strcmp(e->values[i].key, key)) { e->values[i].value = value; return; }
    assert(e->count < 16);
    e->values[e->count++] = (struct value){key, value};
}
struct aud_event *aud_event_dup(struct aud_event *e)
{
    ++dup_calls;
    if (dup_calls == fail_dup_at) return NULL;
    assert(dup_calls <= 8);
    duplicates[dup_calls - 1] = *(struct event *)e;
    return &duplicates[dup_calls - 1].base;
}
char *audutil_strdup_printf(const char *format, ...)
{
    static char text[40];
    va_list ap; va_start(ap, format); vsnprintf(text, sizeof(text), format, ap); va_end(ap);
    return text;
}
uint32_t ms_src_get_lib_version(void) { return 0x12345678; }
void IPC_WriteDebug(uint32_t id, int a, int b, int c, const char *fmt, ...)
{ (void)id; assert(a == 1 && b == 0x15 && c == 0 && fmt); ++debug_calls; }
struct aud_data *aud_data_new_metadata(void *p)
{
    assert(p); ++metadata_allocs;
    if (fail_metadata) return NULL;
    struct aud_data *d = &packets[packet_count++]; memset(d, 0, sizeof(*d)); d->type = 2;
    return d;
}
struct aud_data *aud_data_new(unsigned n)
{
    ++pcm_allocs;
    if (fail_pcm) return NULL;
    assert(n <= sizeof(output_bytes));
    struct aud_data *d = &packets[packet_count++]; memset(d, 0, sizeof(*d));
    d->type = 99; d->size = n; d->ptr = output_bytes;
    return d;
}
int aud_module_push_data(struct mssrc *m, int port, struct aud_data *d)
{ (void)m; assert(port == 0 && d == &packets[pushes]); ++pushes; return 31; }
void aud_data_adapter_push(struct mssrc_adapter *a, struct aud_data *d)
{ a->pushed += d->size; adapter_head = d; ++adapter_pushes; op('A'); }
struct aud_data *aud_data_adapter_get_data(struct mssrc_adapter *a, unsigned n)
{
    assert(n <= a->pushed - a->flushed && n <= sizeof(input_bytes)); ++reads; op('G');
    if (fail_read) return NULL;
    chunk = (struct aud_data){0}; chunk.ptr = input_bytes; chunk.size = n;
    return &chunk;
}
struct aud_data *aud_data_adapter_peep(struct mssrc_adapter *a)
{ (void)a; ++peeps; return no_head ? NULL : adapter_head; }
int ms_src_main_process_init(void *p, int a, int b)
{
    assert(active && p == active->library && a == 0 && b == 0); ++library_inits; op('I');
    if (initialize_format) {
        active->format_ready = 1;
        uint32_t format[7] = {48000, 8, 0, 32, 11, 22, 33};
        memcpy(active->library_output_format, format, sizeof(format));
    }
    return library_init_result;
}
unsigned ms_src_main_process_getsize(void *p, unsigned ratio)
{ assert(active && p == active->library); observed_ratio = ratio; return needed_bytes; }
int ms_src_main_process_run(void *p, void *pcm, unsigned n, unsigned ratio)
{
    assert(active && p == active->library && pcm == input_bytes && ratio == active->ratio);
    assert(runs < run_limit && n <= sizeof(observed_bytes));
    run_sizes[runs++] = n; memcpy(observed_bytes, pcm, n); op('R');
    active->output_size = produced_bytes; active->output_pcm = pcm_bytes;
    return run_result;
}
int msf_process_metadata(void *p, const void *data, unsigned n)
{ assert(active && p == metadata(active) && data == input_bytes && n == 12); ++metadata_runs; op('T'); return metadata_result; }
void ModuleRegistry_Register(const struct mssrc_implementation *impl) { registered = impl; }

static void set(struct event *e, const char *key, int value) { aud_event_value_set_int(&e->base, key, value); }
static int get(struct aud_event *e, const char *key) { return aud_event_value_get_int(e, key, -123); }
static struct event first(void) { struct event e = {0}; e.base.type = 1; return e; }

static void lifecycle(void)
{
    reset();
    struct mssrc m = {0};
    m.channel_mode = 7; m.unsupported = true;
    m.output_pcm = pcm_bytes; m.output_size = 999;
    assert(_mssrc_constructor(&m) == 0);
    assert(m.output_pcm == NULL && m.output_size == 0);
    assert(initialized_adapter == 1 && initialized_memory == 1);
    assert(m.sampling_rate == 48000 && m.channels == 8 && !m.initialized);
    assert(m.channel_mode == 7 && m.unsupported); /* ctor leaves these alone */
    for (unsigned i = 0; i < sizeof(m.library); ++i) assert(m.library[i] == 0);
    m.field_260 = 88; m.ratio = 9; m.output_size = 16;
    assert(_mssrc_init(&m) == 0 && !strcmp(operations, "OC"));
    assert(m.initialized == 1 && m.field_248 == 255 && m.field_260 == 88);
    assert(m.ratio == 9 && m.output_size == 16 && m.channel_mode == 7);
    m.total_input = 99; m.total_output = 88; m.field_24c = 77;
    m.adapter.pushed = 100; m.adapter.flushed = 20;
    reset();
    assert(_mssrc_flush(&m) == 0 && !strcmp(operations, "FM"));
    assert(last_flush == 80 && m.total_input == 99 && m.total_output == 88 && m.field_24c == 77);
    assert(_mssrc_init(&m) == 0 && !strcmp(operations, "FMMXOC"));
    assert(m.total_input == 0 && m.total_output == 0 && m.field_24c == 0);
    assert(m.field_260 == 88 && m.ratio == 9 && m.output_size == 16);
    reset();
    _mssrc_destructor(&m);
    assert(!strcmp(operations, "MXD") && m.initialized == 0 && flushes == 0);
    reset();
    m.initialized = 2;
    assert(_mssrc_init(&m) == 0 && opens == 0 && closes == 0 && m.initialized == 2);
    _mssrc_flush(&m); _mssrc_destructor(&m);
    assert(!strcmp(operations, "FD") && metadata_flushes == 0 && m.initialized == 2);
    reset();
    m.initialized = 0; fail_open = fail_connect = 1;
    assert(_mssrc_init(&m) == 0 && assertions == 2 && m.initialized == 1);
    fail_close = 1;
    assert(_mssrc_init(&m) == 0 && assertions == 5 && m.initialized == 1);
    _mssrc_destructor(&m); assert(assertions == 6 && m.initialized == 0);
    m.adapter.pushed = 0; m.adapter.flushed = 1;
    assert(!strcmp(mssrc_gets_internal_buf_size(&m), "(int)4294967295"));
}

static void commands(void)
{
    struct mssrc m = {0};
    const uint32_t noops[] = {0x10000000,0x10010000,0x10020000,0x1003002c,0x10150000,0x63010008,0x63020004};
    for (unsigned i = 0; i < sizeof(noops)/sizeof(noops[0]); ++i) {
        reset();
        assert(_mssrc_cmd(&m, noops[i], NULL, 999) == 0);
        assert(defaults == 0 && opens == 0 && flushes == 0);
        assert(_mssrc_cmd(&m, noops[i] - 1, NULL, 0) == 23);
        assert(_mssrc_cmd(&m, noops[i] + 1, NULL, 0) == 23);
    }
    for (uint32_t mode = 0; mode < 4; ++mode) {
        m.channel_mode = 99;
        assert(_mssrc_cmd(&m, 0x63030004, &mode, 0) == (mode < 3 ? 0 : 2));
        assert(m.channel_mode == (mode < 3 ? mode : 99));
    }
    uint32_t mode = UINT32_MAX;
    assert(_mssrc_cmd(&m, 0x63030004, &mode, 1) == 2 && m.channel_mode == 99);
    reset();
    assert(_mssrc_cmd(&m, 0x10070000, NULL, 0) == 0 && debug_calls == 0);
    mask = 0x200000;
    assert(_mssrc_cmd(&m, 0x10070000, NULL, 0) == 0 && debug_calls == 8);
    assert(_mssrc_cmd(&m, 0x10140000, NULL, 0) == 0 && metadata_prints == 1);
    const uint32_t unknowns[] = {0,UINT32_MAX,0x50020020,0x50030008,0x63020003,0x63030003,0x63030005,0x10070001,0x10140001};
    for (unsigned i = 0; i < sizeof(unknowns)/sizeof(unknowns[0]); ++i) {
        assert(_mssrc_cmd(&m, unknowns[i], &mode, 71) == 23);
        assert(default_command == unknowns[i] && default_payload == &mode && default_size == 71);
    }
}

static void events(void)
{
    struct mssrc m = {0};
    struct aud_port port = {3, &m};
    struct event e = first();
    reset();
    assert(_mssrc_event(&m, NULL, &e.base) == 19);
    assert(m.sampling_rate == 48000 && m.channels == 8 && !m.unsupported);
    assert(forwarded == &e.base && forwarded_port == NULL && dup_calls == 0 && releases == 0);
    assert(!strcmp(operations, "FOCE"));
    reset();
    e = first(); set(&e, "channels", 2); set(&e, "rate", 44100);
    assert(_mssrc_event(&m, &port, &e.base) == -1);
    assert(m.unsupported && m.sampling_rate == 44100 && m.channels == 2);
    assert(releases == 1 && released[0] == &e.base && defaults == 0 && flushes == 0 && opens == 0);
    e = first(); set(&e, "bits-per-sample", 16);
    assert(_mssrc_event(&m, &port, &e.base) == -1 && releases == 2);
    for (int rate = 0; rate < 2; ++rate) {
        reset(); e = first(); set(&e, "rate", rate ? -1 : 0);
        assert(_mssrc_event(&m, &port, &e.base) == 19 && !m.unsupported);
        assert(dup_calls == 1 && get(forwarded, "rate") == 48000); /* no rate whitelist */
    }
    reset(); e = first();
    set(&e, "rate", 44100); set(&e, "channels", 8); set(&e, "bits-per-sample", 32);
    set(&e, "es", 1); set(&e, "codec", 77); set(&e, "data-rate", 999);
    set(&e, "acmod", 9); set(&e, "check-downmix", 1); set(&e, "dolby", 0);
    m.channel_mode = 1;
    assert(_mssrc_event(&m, &port, &e.base) == 19);
    assert(dup_calls == 2 && releases == 2 && released[0] == &e.base && released[1] == &duplicates[0].base);
    assert(forwarded == &duplicates[1].base && forwarded_port == &port);
    assert(get(forwarded, "rate") == 48000 && get(forwarded, "channels") == 8 && get(forwarded, "acmod") == 2);
    assert(get(forwarded, "es") == 1 && get(forwarded, "codec") == 77 && get(forwarded, "data-rate") == 999);
    assert(get(forwarded, "bits-per-sample") == 32 && get(forwarded, "dolby") == 0);
    assert(get(&e.base, "rate") == 44100 && get(&e.base, "acmod") == 9);
    assert(m.sampling_rate == 44100 && m.channels == 8 && !strcmp(operations, "FMMXOCE"));
    reset(); fail_dup_at = 1;
    assert(_mssrc_event(&m, &port, &e.base) == 19);
    assert(dup_calls == 2 && releases == 1 && released[0] == &e.base);
    assert(get(forwarded, "rate") == 44100 && get(forwarded, "acmod") == 2);
    reset(); fail_dup_at = 2;
    assert(_mssrc_event(&m, &port, &e.base) == 19);
    assert(dup_calls == 2 && releases == 1 && forwarded == &duplicates[0].base);
    assert(get(forwarded, "rate") == 48000 && get(forwarded, "acmod") == 9);
    reset(); fail_dup_at = 1; m.channel_mode = 0;
    assert(_mssrc_event(&m, &port, &e.base) == 19);
    assert(forwarded == &e.base && releases == 0 && get(forwarded, "rate") == 44100 && flushes == 1);
    for (unsigned mode = 0; mode < 4; ++mode) {
        reset(); e = first(); set(&e, "check-downmix", 1); set(&e, "acmod", 9); m.channel_mode = mode;
        assert(_mssrc_event(&m, &port, &e.base) == 19);
        assert(dup_calls == (mode == 1 ? 1u : 0u));
        assert(get(forwarded, "acmod") == (mode == 1 ? 2 : 9));
    }
    reset(); e = first(); m.channel_mode = 1; set(&e, "check-downmix", 0);
    assert(_mssrc_event(&m, &port, &e.base) == 19 && dup_calls == 0);
    reset(); e = first(); set(&e, "check-downmix", 1); fail_dup_at = 1;
    assert(_mssrc_event(&m, &port, &e.base) == 19 && forwarded == &e.base && releases == 0);
    for (int type = 0; type <= 13; ++type) {
        if (type == 1) continue;
        for (unsigned flags = 0; flags < 4; ++flags) {
            reset(); e = first(); e.base.type = type; e.base.flags = flags;
            m.unsupported = true;
            assert(_mssrc_event(&m, &port, &e.base) == 19);
            assert(forwarded == &e.base && m.unsupported && opens == 0 && closes == 0 && releases == 0);
            assert(flushes == (type == 7 && (flags & 2) ? 1u : 0u));
        }
    }
}

static void push_and_properties(void)
{
    struct mssrc m = {0};
    reset();
    for (unsigned i = 0; i < sizeof(pcm_bytes); ++i) pcm_bytes[i] = (unsigned char)i;
    m.output_pcm = pcm_bytes; m.output_size = sizeof(pcm_bytes);
    m.timestamp = 123; m.field_23c = 0x1234fffe; m.field_230 = 456; m.field_234 = 789;
    assert(_mssrc_push_pcm(&m) == 0 && pushes == 2);
    assert(packets[0].type == 2 && packets[0].index == -2 && packets[0].ts == 123);
    assert(packets[0].field_20 == 456 && packets[0].field_24 == 789);
    assert(packets[1].type == 0 && packets[1].index == -2 && packets[1].ts == 123 && packets[1].size == 32);
    assert(!memcmp(output_bytes, pcm_bytes, 32) && m.output_size == 32 && m.total_output == 0);
    reset(); fail_metadata = true;
    assert(_mssrc_push_pcm(&m) == 0 && pushes == 1 && packets[0].type == 0);
    reset(); fail_pcm = true;
    assert(_mssrc_push_pcm(&m) == 0 && pushes == 1 && packets[0].type == 2);
    reset(); fail_metadata = fail_pcm = true;
    assert(_mssrc_push_pcm(&m) == 0 && pushes == 0 && metadata_allocs == 1 && pcm_allocs == 1);
    reset(); m.output_size = 1;
    assert(_mssrc_push_pcm(&m) == 0 && assertions == 1 && pushes == 2);
    assert(mssrc_properties[0].type == 2 && mssrc_properties[0].firmware_offset == 0x204);
    assert(!strcmp(mssrc_properties[0].name, "sampling-rate"));
    assert(!strcmp(mssrc_properties[1].name, "channels"));
    assert(!strcmp(mssrc_properties[2].name, "channel-mode"));
    assert(mssrc_properties[2].host_offset == offsetof(struct mssrc, channel_mode));
    assert(mssrc_properties[3].flags == 1 && mssrc_properties[3].gets == mssrc_gets_internal_buf_size);
    assert(mssrc_properties[4].name == NULL);
    mssrc_register(); assert(registered == &mssrc_implementation);
    assert(registered->firmware_instance_size == 0x280 && registered->chain == _mssrc_chain);
    assert(registered->event == _mssrc_event && registered->command == _mssrc_cmd);
}
static void prepare_chain(struct mssrc *m)
{
    reset(); memset(m, 0, sizeof(*m)); active = m;
    _mssrc_constructor(m); _mssrc_init(m);
    operation_count = 0; operations[0] = 0;
}
static void chain(void)
{
    struct mssrc m;
    struct aud_port port = {0, &m};
    struct aud_data input = {0}; input.ptr = input_bytes; input.size = 12;
    for (int type = 0; type <= 3; ++type) {
        prepare_chain(&m); m.unsupported = true; input.type = type;
        assert(_mssrc_chain(&port, &input) == -1 && releases == 1 && released[0] == &input);
        assert(adapter_pushes == 0 && library_inits == 0 && metadata_runs == 0);
    }
    prepare_chain(&m); input.type = 2; metadata_result = -7;
    assert(_mssrc_chain(&port, &input) == 0 && metadata_runs == 1 && releases == 1);
    assert(!strcmp(operations, "T") && library_inits == 0 && adapter_pushes == 0);
    assert(m.ratio == 0 && m.total_input == 0);
    input.type = 0; input.size = 16; input.ts = 9000;
    for (int result = -1; result <= 5; ++result) {
        prepare_chain(&m); library_init_result = result;
        int expected = result == 3 || result == 4 ? 0 : 1;
        assert(_mssrc_chain(&port, &input) == expected && library_inits == 1);
        assert(adapter_pushes == (expected == 0 ? 1u : 0u));
        assert(releases == (expected == 0 ? 0u : 1u));
        if (expected == 0) assert(m.output_format[0] == 48000 && m.output_format[1] == 8 && m.output_format[3] == 32);
    }
    prepare_chain(&m); initialize_format = false;
    assert(_mssrc_chain(&port, &input) == 1 && releases == 1 && adapter_pushes == 0);
    const int rates[] = {24000, 44100, 48000, 96000, -1};
    const unsigned ratios[] = {2, 1, 7, 7, 7};
    for (unsigned i = 0; i < 5; ++i) {
        prepare_chain(&m); m.sampling_rate = rates[i]; m.ratio = 7;
        assert(_mssrc_chain(&port, &input) == 0 && m.ratio == ratios[i] && observed_ratio == ratios[i]);
        assert(reads == 0 && m.total_input == 16 && m.adapter.pushed == 16);
    }
    prepare_chain(&m); m.sampling_rate = 0;
    assert(_mssrc_chain(&port, &input) == 1 && releases == 1 && adapter_pushes == 0);
    prepare_chain(&m); fail_read = true; input.size = 32; m.field_25c = 77;
    assert(_mssrc_chain(&port, &input) == 0 && reads == 1 && runs == 0 && m.field_25c == 0);
    assert(flushes == 0 && releases == 0 && m.adapter.pushed == 32);
    for (unsigned mode = 0; mode < 4; ++mode) {
        prepare_chain(&m); m.channel_mode = mode; input.size = needed_bytes = 291;
        for (unsigned i = 0; i < input.size; ++i) input_bytes[i] = (unsigned char)(i + 1);
        assert(_mssrc_chain(&port, &input) == 0 && runs == 1 && pushes == 2);
        for (unsigned frame = 0; frame < 9; ++frame) {
            for (unsigned byte = 0; byte < 32; ++byte) {
                unsigned char expected = (unsigned char)(frame * 32 + byte + 1);
                if (mode == 1) expected = byte < 8 ? (unsigned char)(frame * 32 + byte + 25) : 0;
                else if (mode != 0 && byte >= 24) expected = 0;
                assert(observed_bytes[frame * 32 + byte] == expected);
            }
        }
        for (unsigned i = 288; i < 291; ++i) assert(observed_bytes[i] == (unsigned char)(i + 1));
        assert(releases == 1 && released[0] == &chunk && last_flush == 291);
        assert(m.total_input == 291 && m.total_output == 32 && m.output_size == 0);
        assert(m.field_260 == 291 && m.field_258 == 1 && m.timestamp == 9000 && m.field_254 == 32);
    }
    prepare_chain(&m); input.size = 64;
    assert(_mssrc_chain(&port, &input) == 0 && runs == 2 && reads == 2 && pushes == 4);
    assert(m.timestamp == 9001 && m.field_250 == 9001 && m.field_248 == 9000);
    assert(m.field_24c == 9001 && m.total_output == 64 && flushes == 2);
    assert(run_sizes[0] == 32 && run_sizes[1] == 32 && m.adapter.flushed == 64);
    prepare_chain(&m); input.size = 32;
    m.format_ready = 1; m.output_format[1] = 3; m.output_format[3] = 32;
    m.field_248 = input.ts; m.field_250 = 1000;
    assert(_mssrc_chain(&port, &input) == 0 && m.timestamp == 1003 && library_inits == 0);
    assert(m.field_248 == input.ts && m.field_250 == 1003);
    prepare_chain(&m); m.format_ready = 1; m.output_format[1] = 8; m.output_format[3] = 16;
    m.field_248 = input.ts; m.field_250 = 1000;
    assert(_mssrc_chain(&port, &input) == 0 && m.timestamp == 1003);
    prepare_chain(&m); produced_bytes = 30; m.field_248 = input.ts; m.field_250 = 1000;
    assert(_mssrc_chain(&port, &input) == 0 && m.timestamp == 1000 && m.total_output == 30);
    prepare_chain(&m); input.size = 32; no_head = true;
    assert(_mssrc_chain(&port, &input) == 0 && m.timestamp == 1 && m.field_248 == 255);
    prepare_chain(&m); input.ts = 0x0fffffff;
    assert(_mssrc_chain(&port, &input) == 0 && m.timestamp == 1 && m.field_250 == 1);
    input.ts = 9000;
    prepare_chain(&m); m.format_ready = 1; m.output_size = 32; m.output_pcm = pcm_bytes;
    input.size = 16; m.timestamp = 123;
    assert(_mssrc_chain(&port, &input) == 0 && pushes == 2 && library_inits == 0);
    assert(m.total_output == 32 && m.output_size == 0 && packets[1].ts == 123);
    prepare_chain(&m); input.size = 32; run_result = 3;
    assert(_mssrc_chain(&port, &input) == 0 && flushes == 1 && pushes == 0 && m.total_output == 0);
    assert(m.output_size == 32 && releases == 1);
    run_result = 0; input.size = 16;
    assert(_mssrc_chain(&port, &input) == 0 && runs == 1 && pushes == 2);
    assert(m.output_size == 0 && m.total_output == 32 && m.total_input == 48);
    input.size = 32;
    prepare_chain(&m); produced_bytes = 0;
    assert(_mssrc_chain(&port, &input) == 0 && flushes == 1 && pushes == 0 && m.field_258 == 0);
    prepare_chain(&m); run_result = -7; input.size = 64;
    assert(_mssrc_chain(&port, &input) == 0 && flushes == 1 && metadata_flushes == 1);
    assert(m.total_input == 64 && m.total_output == 0 && opens == 1 && closes == 0);
    assert(last_flush == 64 && m.adapter.flushed == 64 && runs == 1);
    input.size = 32;
    prepare_chain(&m); run_result = -30656;
    assert(_mssrc_chain(&port, &input) == 0 && flushes == 1 && metadata_flushes == 2);
    assert(opens == 2 && closes == 1 && connects == 2 && m.total_input == 0);
    prepare_chain(&m); fail_metadata = fail_pcm = true;
    assert(_mssrc_chain(&port, &input) == 0 && pushes == 0 && m.total_output == 32 && m.output_size == 0);
    prepare_chain(&m); input.type = 1;
    m.total_input = m.total_output = 0xfffffff0u;
    m.adapter.pushed = m.adapter.flushed = 0xfffffff0u;
    assert(_mssrc_chain(&port, &input) == 0 && runs == 1 && pushes == 2);
    assert(m.total_input == 16 && m.total_output == 16 && m.adapter.flushed == 16);
    assert(m.adapter.pushed == m.adapter.flushed); /* unsigned occupancy survives wrap */
    prepare_chain(&m); input.type = 0; input.ts = 0xfffffffeu; input.size = 64;
    assert(_mssrc_chain(&port, &input) == 0 && m.timestamp == 0xffffffffu);
    assert(m.field_248 == 0xfffffffeu && m.field_250 == 0xffffffffu);
}
int main(void)
{
    lifecycle(); commands(); events(); push_and_properties(); chain();
    puts("mssrc: lifecycle, commands, FIRST, push/properties and conversion chain passed");
    return 0;
}
