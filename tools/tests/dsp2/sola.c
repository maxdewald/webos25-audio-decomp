/* Standalone host mocks test routing/control, not unrecovered DSP alpha math.
 * clang -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined
 *   tools/tests/dsp2/sola.c -o build/scratch/sola/test && build/scratch/sola/test */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../src/dsp2/sola.c"

static int creates, destroys, channels_set, pcm_set, bypass_set, init_count;
static int alpha_count, alpha_hooks, last_bypass, engine_result, run_result;
static int put_count, flushes, unmaps, default_events, default_commands, cleanups;
static unsigned last_alloc, last_copy, last_flush;
static bool fail_map, fail_new;
static struct aud_data *outputs[16];
static unsigned output_count, event_count;
static struct aud_event first_event = {1, 0};
static int event_channels = 2, speed_num = 3, speed_den = 2;
static bool event_seamless;
static uint32_t last_alpha;
static const void *registered;
const void *sola_impl_descriptor = &registered;
void *_BufMonitor_RemoveBufferIdxLocal(unsigned size)
{ last_alloc = size; return calloc(1, size); }
void _BufMonitor_RemoveBufferIndex(void *p) { free(p); }
void WSOLA2_create(void *p) { assert(p); ++creates; }
void WSOLA2_destroy(void *p) { assert(p); ++destroys; }
int WSOLA2_init(void *p) { assert(p); ++init_count; return engine_result; }
int WSOLA2_SetChannel(void *p, int n) { assert(p); channels_set = n; return engine_result; }
int WSOLA2_SetPcmByte(void *p, int n) { assert(p); pcm_set = n; return engine_result; }
int WSOLA2_SetBypass(void *p, int n) { assert(p); ++bypass_set; last_bypass = n; return engine_result; }
int WSOLA2_SetAlpha(void *p, uint32_t n) { assert(p); ++alpha_count; last_alpha = n; return engine_result; }
int WSOLA2_GetInputByte(void *p) { assert(p); return 16; }
int WSOLA2_GetOutputByte(void *p) { assert(p); return 8; }
int WSOLA2_GetStateFlag(void *p) { assert(p); return 7; }
int WSOLA2_RunMain(void *p, const void *in, void *out)
{ assert(p && in && out); memset(out, 0x5a, 8); return run_result; }
void AuiLogger_AppendData(void *out, const void *in, unsigned n)
{ last_copy = n; memcpy(out, in, n); }
void aud_data_adapter_init(struct sola_adapter *a) { a->pushed = a->flushed = 0; }
void aud_data_adapter_cleanup(struct sola_adapter *a) { ++cleanups; a->flushed = a->pushed; }
void aud_object_put(void *p)
{ struct aud_data *d = p; ++put_count; free(d->payload); free(d); }
struct aud_data *aud_data_new(unsigned size)
{
    if (fail_new) return NULL;
    struct aud_data *d = calloc(1, sizeof(*d)); assert(d);
    d->size = size; d->payload = calloc(1, size ? size : 1); assert(d->payload);
    return d;
}
int aud_data_adapter_push(struct sola_adapter *a, struct aud_data *d)
{ a->pushed += d->size; aud_object_put(d); return 0; }
void aud_data_adapter_flush(struct sola_adapter *a, unsigned n)
{ ++flushes; last_flush = n; assert(n <= queued(a)); a->flushed += n; }
void *aud_data_adapter_map(struct sola_adapter *a, unsigned n)
{ static unsigned char data[4096]; assert(n <= queued(a)); return fail_map ? NULL : data; }
void aud_data_adapter_unmap(struct sola_adapter *a, void *p) { (void)a; assert(p); ++unmaps; }
struct aud_data *aud_data_adapter_get_data(struct sola_adapter *a, unsigned n)
{ assert(n <= queued(a)); return n ? aud_data_new(n) : NULL; }
int aud_module_push_data(struct sola_module *m, int port, struct aud_data *d)
{ (void)m; assert(port == 0 && d && output_count < 16); outputs[output_count++] = d; return 0; }
int aud_module_push_event(struct sola_module *m, int port, struct aud_event *e)
{ (void)m; assert(port == 0 && e == &first_event); ++event_count; return 0; }
struct aud_event *aud_event_newv_first(const char *key, int z, int channels, int end)
{ assert(!strcmp(key, "channels") && z == 0 && end == 0 && channels == event_channels); return &first_event; }
int aud_module_default_event(struct sola_module *m, struct aud_port *p, struct aud_event *e)
{ (void)m; (void)p; (void)e; ++default_events; return 23; }
int aud_module_default_command(struct sola_module *m, uint32_t c, void *p, unsigned n)
{ (void)m; (void)c; (void)p; (void)n; ++default_commands; return 24; }
int aud_event_value_get_int(struct aud_event *e, const char *key, int def)
{ (void)e; assert(!strcmp(key, "channels") && def == 2); return event_channels; }
bool aud_event_value_get_bool(struct aud_event *e, const char *key, bool def)
{ (void)e; assert(!strcmp(key, "seamless-mode") && !def); return event_seamless; }
void aud_event_value_get_fraction(struct aud_event *e, const char *key, int *num, int *den)
{ (void)e; assert(!strcmp(key, "speed")); *num = speed_num; *den = speed_den; }
void sola_flix_alpha(struct sola_module *m, uint32_t site)
{
    assert(site == 0x600a4a71 || site == 0x600a4eed || site == 0x600a50ca);
    ++alpha_hooks;
    /* Synthetic sentinel. This is NOT an implementation of target arithmetic. */
    m->alpha_bits = 0x12345678;
    if (site != 0x600a50ca) m->alpha_pending = 1;
}
uint32_t sola_flix_initial_alpha(void) { return 0x87654321; }
char *audutil_strdup_printf(const char *fmt, ...)
{
    char *s = malloc(40); assert(s); va_list ap; va_start(ap, fmt);
    vsnprintf(s, 40, fmt, ap); va_end(ap); return s;
}
void ModuleRegistry_Register(const void *p) { registered = p; }
static void clear_outputs(void)
{ for (unsigned i = 0; i < output_count; ++i) aud_object_put(outputs[i]); output_count = 0; }
static struct aud_data *input(int type, unsigned size)
{ struct aud_data *d = aud_data_new(size); assert(d); d->type = type; d->index = 11; d->timestamp = 9876; return d; }
static void wrapper_test(void)
{
    struct sola_handle *s; struct sola_info info;
    assert(SOLA_Create(&s, 11, 2) == 0 && s->channels == 2 && channels_set == 2);
    assert(last_alloc == 0x78 && creates == 1 && s->enabled == 255);
    assert(SOLA_SetPcmByte(s, 4) == 0 && pcm_set == 4);
    assert(SOLA_SetPcmByte(s, 3) == -8 && s->pcm_bytes == 4);
    assert(SOLA_SetEnableChannel(s, 0xdead) == 0 && s->enabled == 0xdead);
    assert(SOLA_SetBypass(s, -2) == 0 && last_bypass == -2 && s->bypass);
    assert(SOLA_GetInputByte(s) == 2048 && SOLA_GetOutputByte(s) == 2048);
    unsigned char in[2048], out[2048]; memset(in, 0x81, sizeof(in));
    assert(SOLA_RunMain(s, in, out) == 0 && last_copy == 2048 && !memcmp(in, out, sizeof(in)));
    assert(SOLA_GetInfo(s, &info) == 0 && info.input_frames == 2 && info.output_frames == 1 && info.state == 7);
    assert(SOLA_GetInfo(NULL, &info) == -15 && SOLA_GetInfo(s, NULL) == -15);
    int n = bypass_set;
    assert(SOLA_SetSeamlessMode(s, 1) == 0 && !s->bypass && bypass_set == n);
    assert(SOLA_GetInputByte(s) == 16 && SOLA_GetOutputByte(s) == 8);
    engine_result = -9;
    assert(SOLA_SetChannel(s, 9) == -9 && s->channels == 9);
    assert(SOLA_SetPcmByte(s, 2) == -9 && s->pcm_bytes == 2);
    assert(SOLA_Init(s) == -9 && SOLA_SetAlpha(s, 0xaabbccdd) == -9 && last_alpha == 0xaabbccdd);
    engine_result = 0;
    assert(SOLA_Destroy(s) == 0 && destroys == 1);
    assert(SOLA_Create(&s, -1, 17) == -14 && s->channels == 2);
    assert(SOLA_SetPcmByte(s, 4) == -14 && s->pcm_bytes == 4);
    assert(SOLA_SetBypass(s, 0) == -14 && SOLA_Init(s) == -14 && SOLA_SetAlpha(s, 0) == -14);
    assert(SOLA_GetInputByte(s) == 0 && SOLA_GetOutputByte(s) == 0);
    assert(SOLA_RunMain(s, in, out) == -14);
    assert(SOLA_SetChannel(s, 3) == 0 && s->channels == 3);
    assert(SOLA_GetInfo(s, &info) == 0 && info.input_frames == 256 && info.output_frames == 256 && info.state == 0);
    assert(SOLA_Destroy(s) == -14 && destroys == 1);
}
static void module_test(void)
{
    struct sola_module m = {.rate_in = 10, .rate_out = 20, .first_sent = 9};
    struct aud_port p = {0, &m};
    sola_constructor(&m);
    assert(m.channels == 2 && !m.handle && !m.initialized && !m.seamless);
    assert(m.rate_in == 10 && m.rate_out == 20 && m.first_sent == 9 && m.alpha_bits == 0x87654321);
    struct aud_event e = {1, 0};
    event_seamless = true;
    assert(sola_event(&m, &p, &e) == 23);
    assert(m.rate_in == 2 && m.rate_out == 3 && m.seamless && m.first_sent == 1);
    assert(!m.initialized && m.handle->pcm_bytes == 4 && m.handle->enabled == 255);
    assert(sola_set_seamless_mode(&m, NULL, "0") == 0 && m.seamless);
    int hooks = alpha_hooks;
    assert(sola_chai(&p, input(2, 4)) == 0 && queued(&m.metadata) == 4);
    assert(sola_chai(&p, input(2, 8)) == 0 && queued(&m.metadata) == 8);
    assert(sola_chai(&p, input(1, 40)) == 0 && !queued(&m.pcm));
    assert(sola_chai(&p, input(0, 8)) == 0 && !output_count && m.initialized);
    assert(alpha_hooks == hooks + 1 && !m.alpha_pending && !last_bypass);
    assert(sola_chai(&p, input(0, 24)) == 0 && output_count == 4);
    assert(outputs[0]->type == 2 && outputs[1]->type == 0 && outputs[2]->type == 2);
    assert(outputs[1]->index == 11 && outputs[1]->timestamp == 9876 && outputs[1]->size == 8);
    assert(((unsigned char *)outputs[1]->payload)[0] == 0x5a && queued(&m.metadata) == 8);
    clear_outputs();
    e.type = 2; assert(sola_event(&m, &p, &e) == 23 && !m.first_sent);
    unsigned ev = event_count;
    assert(sola_chai(&p, input(0, 16)) == 0 && event_count == ev + 1 && m.first_sent);
    clear_outputs();
    fail_map = true;
    assert(sola_chai(&p, input(0, 16)) == -1 && queued(&m.pcm) == 16);
    fail_map = false; fail_new = true;
    struct aud_data *tiny; fail_new = false; tiny = input(0, 0); fail_new = true;
    int u = unmaps; assert(sola_chai(&p, tiny) == -1 && unmaps == u + 1 && queued(&m.pcm) == 16);
    fail_new = false; run_result = -3;
    assert(sola_chai(&p, input(0, 0)) == -1 && !queued(&m.pcm)); run_result = 0;
    int init = init_count; m.alpha_pending = 1;
    e.type = 7; e.flags = 0; assert(sola_event(&m, NULL, &e) == 23 && init_count == init);
    e.flags = 2; assert(sola_event(&m, NULL, &e) == 23 && init_count == init + 1 && !m.alpha_pending);
    assert(!queued(&m.metadata));
    m.seamless = false; m.rate_in = m.rate_out = 1;
    m.pcm.pushed += 4;
    assert(sola_chai(&p, input(1, 12)) == 0 && output_count == 2);
    assert(outputs[0]->size == 4 && outputs[1]->type == 1 && outputs[1]->size == 12);
    clear_outputs();
    int rate[2] = {4, 5};
    assert(sola_command(&m, 0x51000008, rate, 0) == 0 && m.rate_in == 4 && m.rate_out == 5);
    assert(sola_command(&m, 0x10000000, NULL, 0) == 0);
    assert(sola_command(&m, 0x10010000, NULL, 0) == 0);
    assert(sola_command(&m, 0x10020000, NULL, 0) == 0);
    assert(sola_command(&m, 0xffffffff, NULL, 0) == 24 && default_commands == 1);
    m.pcm.pushed += 7; char *s = sola_gets_internal_buf_size(&m); assert(!strcmp(s, "(int)7")); free(s);
    int f = flushes; m.old_in = m.old_out = 1; sola_flush(&m);
    assert(flushes == f + 1 && last_flush == 7); f = flushes; sola_flush(&m); assert(flushes == f);
    sola_destructor(&m); assert(cleanups == 2);
    SOLA_ModuleInit(); assert(registered == sola_impl_descriptor);
}
int main(void)
{ wrapper_test(); module_test(); printf("sola: wrapper, lifecycle, commands, events, PCM/metadata, errors OK (%d puts)\n", put_count); return 0; }
