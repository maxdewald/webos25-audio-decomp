/* Source-extracted host mocks. */
#define _GNU_SOURCE
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libaa/objects.h"

static unsigned int mask;
struct logmm_module logmm_module_commod = {.mask = &mask};
struct logmm_module logmm_module_buf_com = {.mask = &mask};
struct logmm_module logmm_module_aud = {.mask = &mask};
struct logmm_module logmm_module_buf_writer = {.mask = &mask};
struct logmm_module logmm_module_buf_reader = {.mask = &mask};
static int assertions, dumps, updates, setters, invalidations;
static void *invalidated[2];
static int lengths[2];
int logmm_printl(struct logmm_module *m, int level, const char *fn, int line,
                 const char *format, ...)
{
    if (m == &logmm_module_commod)
        assert(strcmp(fn, "set_prop") == 0);
    return 0;
}
void BufDbg_AssertBreak(void) { assertions++; }
void BufDbg_Dump(ADEC_BUF_T *buf) { dumps++; }
static void invalidate(void *addr, int len, ADEC_BUF_T *buf)
{
    assert(invalidations < 2);
    invalidated[invalidations] = addr;
    lengths[invalidations++] = len;
}
#define CACHE_INV(buf, p, n) (buf)->adap->pfnInvalidate((p), (n), (buf))
// @extract src/libaa/module.c parse_bool aud_module_set_prop_locked
// @extract src/libaa/buffers.c CommonBuffer_ReadData
#define NULL_BUF_CHECK(line) BUF_ASSERT(buf != NULL, "buf", NULL, -ADEC_ERR_NULL, line, "null\n")
// @extract src/libaa/buffers.c CommonBuffer_GetFreeSize
// @extract src/libaa/interbuf.c InterBufWriter_UpdateStatus InterBufReader_UpdateStatus
#undef NULL_BUF_CHECK

static unsigned event_puts, event_defaults, event_pushes, event_commands;
struct aud_object *aud_object_get(struct aud_object *object)
{ assert(object->user); ++object->user; return object; }
void aud_object_put(struct aud_object *object)
{ assert(object->user); --object->user; ++event_puts; }
const char *aud_event_name(struct aud_event *event) { return "event"; }
void *aud_event_serialize(struct aud_event *event, unsigned *size)
{ *size = 4; return calloc(1, *size); }
int aud_module_default_event(struct aud_module *module, struct aud_port *port,
                             struct aud_event *event)
{ ++event_defaults; aud_event_put(event); return -37; }
int aud_module_push_event(struct aud_module *module, int number, struct aud_event *event)
{ assert(number == 0); ++event_pushes; aud_event_put(event); return -41; }
int aimc_send_command(struct aimc *imc, unsigned command, unsigned sender,
                       unsigned target, void *param, unsigned size)
{ assert(command == 0x10190000u && target == 7); ++event_commands; return -43; }
struct aud_port *aud_module_get_output_port(struct aud_module *module, unsigned number)
{ assert(!"unexpected FIRST path"); return NULL; }
int aud_event_value_get_int(struct aud_event *event, const char *name, int fallback)
{ assert(!"unexpected FIRST path"); return fallback; }
int logmm_printlm(const char *name, int level, const char *func, int line,
                  const char *format, ...) { return 0; }
static void push_zero_buffers(struct mod_glue *glue, int count, int size)
{ assert(!"unexpected FIRST path"); }
#define GLOG(g, lvl, line, fmt, ...) PLOG(&(g)->module, lvl, line, fmt, ##__VA_ARGS__)
#define ADEC_CMD_ASYNC_EVENT 0x10190000u
// @extract src/libaa/modules_builtin.c Glue_Event

static void glue_eos_paths(void)
{
    for (int writer = 0; writer < 2; ++writer) {
        for (int last = 0; last < 2; ++last) {
            struct mod_glue glue = {0};
            ADEC_BUF_T ring = {0};
            struct aud_event event = {0};
            glue.output_buffer = writer ? &ring : NULL;
            glue.cowork_uid = 7;
            event.object.user = 1;
            event.type = last ? AUD_EVENT_TYPE_LAST_AU : AUD_EVENT_TYPE_CUSTOM;
            event.prop = AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC;
            event_puts = event_defaults = event_pushes = event_commands = 0;
            int result = Glue_Event(&glue.module, NULL, &event);
            assert(!event.object.user && event_puts == 1 && !event_commands);
            assert(event_defaults == !writer && event_pushes == (writer && last));
            assert(result == (!writer ? -37 : last ? -41 : 0));
        }
    }
    struct mod_glue glue = {.cowork_uid = 7};
    struct aud_event event = {.object = {.user = 1}, .type = AUD_EVENT_TYPE_CUSTOM,
                             .prop = AUD_EVENT_PROP_DOWNWARD};
    event_puts = event_defaults = event_pushes = event_commands = 0;
    assert(Glue_Event(&glue.module, NULL, &event) == -37);
    assert(event_commands == 1 && event_defaults == 1 && !event.object.user);
}

struct values {
    struct aud_module module;
    unsigned long long wide;
    unsigned long integer;
    bool boolean;
    char *string;
    void *data;
};
static int updated(struct aud_module *m, const struct aud_module_property *p)
{
    assert(pthread_mutex_trylock(&m->prop_access_lock) != 0);
    updates++;
    return 0;
}
static int setter(struct aud_module *m, const struct aud_module_property *p,
                   const void *value, unsigned int size)
{
    assert(pthread_mutex_trylock(&m->prop_access_lock) != 0);
    setters++;
    return 23;
}
static void properties(void)
{
    struct values v = {0};
    struct aud_module_impl impl = {.prop_updated = updated};
    struct aud_module_property p = {.name = "test"};
    v.module.name = "mock";
    v.module.impl = &impl;
    assert(pthread_mutex_init(&v.module.prop_access_lock, NULL) == 0);
    p.type = param_type_longlong;
    p.offset = offsetof(struct values, wide);
    assert(aud_module_set_prop_locked(&v.module, &p, "0x123456789abcdef0", 0) == 0);
    assert(v.wide == 0x123456789abcdef0ULL && updates == 1);
    aud_module_set_prop_locked(&v.module, &p, "0x123456789abcdef0", 0);
    assert(updates == 1);
    p.type = param_type_int;
    p.offset = offsetof(struct values, integer);
    aud_module_set_prop_locked(&v.module, &p, "0x32", 0);
    assert(v.integer == 50 && updates == 2);
    aud_module_set_prop_locked(&v.module, &p, "50", 0);
    assert(updates == 2);
    p.type = param_type_bool;
    p.offset = offsetof(struct values, boolean);
    aud_module_set_prop_locked(&v.module, &p, "yes", 0);
    assert(v.boolean && updates == 3);
    aud_module_set_prop_locked(&v.module, &p, "true", 0);
    aud_module_set_prop_locked(&v.module, &p, "1", 0);
    assert(updates == 3);
    aud_module_set_prop_locked(&v.module, &p, "YES", 0);
    assert(!v.boolean && updates == 4);
    p.type = param_type_str;
    p.offset = offsetof(struct values, string);
    aud_module_set_prop_locked(&v.module, &p, "first", 0);
    aud_module_set_prop_locked(&v.module, &p, "second", 0);
    assert(strcmp(v.string, "second") == 0 && updates == 6);
    p.type = param_type_data;
    p.offset = offsetof(struct values, data);
    aud_module_set_prop_locked(&v.module, &p, "abc", 3);
    aud_module_set_prop_locked(&v.module, &p, "defg", 4);
    assert(memcmp(v.data, "defg", 4) == 0 && updates == 8);
    p.type = param_type_struct;
    aud_module_set_prop_locked(&v.module, &p, "ignored", 0);
    p.type = param_type_none;
    aud_module_set_prop_locked(&v.module, &p, "ignored", 0);
    assert(updates == 8);
    p.set = setter;
    assert(aud_module_set_prop_locked(&v.module, &p, "callback", 8) == 23);
    assert(setters == 1 && updates == 8);
    assert(pthread_mutex_trylock(&v.module.prop_access_lock) == 0);
    pthread_mutex_unlock(&v.module.prop_access_lock);
    pthread_mutex_destroy(&v.module.prop_access_lock);
    free(v.string);
    free(v.data);
}
static void reads(void)
{
    unsigned char data[20], boundary[4], *dst = NULL;
    ADEC_PROCESSOR_ADAP_IF_T adap = {.pfnInvalidate = invalidate};
    ADEC_BUF_T b = {.max_size = 16, .max_access_size = 4, .data = data, .adap = &adap};
    int n, au = 123;
    for (unsigned int i = 0; i < sizeof(data); i++) data[i] = i;
    b.status.writeIndex = 1;
    b.status.readOffset = 2;
    b.status.writeOffset = 10;
    invalidations = 0;
    n = 12;
    assert(CommonBuffer_ReadData(&dst, &n, &au, &b) == 0);
    assert(dst == data + 6 && n == 8 && au == 123);
    assert(invalidations == 1 && invalidated[0] == dst && lengths[0] == 8);
    b.status.readOffset = 14;
    b.status.writeOffset = 2;
    invalidations = 0;
    n = 4;
    assert(CommonBuffer_ReadData(&dst, &n, &au, &b) == 0);
    assert(dst == data + 2 && n == 4);
    assert(memcmp(dst, (unsigned char[]){18, 19, 4, 5}, 4) == 0);
    assert(invalidations == 2 && invalidated[0] == data + 4 && lengths[0] == 2);
    assert(invalidated[1] == data + 18 && lengths[1] == 2);
    b.boundary_buf = boundary;
    b.status.writeOffset = 5;
    invalidations = 0;
    n = 7;
    assert(CommonBuffer_ReadData(&dst, &n, &au, &b) == 0);
    assert(dst == boundary && n == 4);
    assert(memcmp(dst, (unsigned char[]){18, 19, 4, 5}, 4) == 0);
    b.boundary_buf = NULL;
    b.status.readOffset = 8;
    b.status.writeOffset = 4;
    invalidations = 0;
    n = 12;
    assert(CommonBuffer_ReadData(&dst, &n, &au, &b) == 0);
    assert(dst == data + 12 && n == 8 && invalidations == 1);
    b.status.readIndex = b.status.writeIndex;
    n = 1;
    assert(CommonBuffer_ReadData(&dst, &n, &au, &b) == -ADEC_ERR_UNDERFLOW);
    n = 0;
    assert(CommonBuffer_ReadData(&dst, &n, &au, &b) == 0);
    assert(CommonBuffer_ReadData(NULL, &n, &au, &b) == -ADEC_ERR_NULL);
    assert(CommonBuffer_ReadData(&dst, NULL, &au, &b) == -ADEC_ERR_NULL);
    assert(assertions == 2 && dumps == 2);
}
static void cached_ring_status(void)
{
    ADEC_COMMON_BUF_T shared = {.maxSize = 64, .maxIndex = 4};
    ADEC_BUF_T writer = {.commonBuf = &shared, .max_size = 64, .aui_count = 4};
    writer.status.writeIndex = 3;
    writer.status.writeOffset = 12;
    writer.status.indexAllocated = 1;
    writer.status.written = 7;
    writer.status.passed = 9;
    ADEC_COMMON_BUF_STATUS_T progress = {
        .readIndex = 1, .readOffset = 8, .writeIndex = 2, .writeOffset = 24,
        .indexAllocated = 0, .written = 99, .passed = 99
    };
    assert(CommonBuffer_GetFreeSize(&writer) == 0);
    assert(InterBufWriter_UpdateStatus(&writer, &progress) == 0);
    assert(CommonBuffer_GetFreeSize(&writer) == 56);
    assert(writer.status.writeIndex == 3 && writer.status.writeOffset == 12);
    assert(writer.status.indexAllocated == 1 && writer.status.written == 7);
    assert(writer.status.passed == 9);
    writer.status.readOffset = 16;
    assert(CommonBuffer_GetFreeSize(&writer) == 0);
    progress.readOffset = 20;
    assert(CommonBuffer_GetFreeSize(&writer) == 0);
    assert(InterBufWriter_UpdateStatus(&writer, &progress) == 0);
    assert(CommonBuffer_GetFreeSize(&writer) == 4);
    assert(shared.maxSize == 64 && shared.maxIndex == 4);
    ADEC_BUF_T reader = {.status = {.readIndex = 1, .readOffset = 8, .passed = 5}};
    progress.indexAllocated = 1;
    assert(InterBufReader_UpdateStatus(&reader, &progress) == 0);
    assert(reader.status.writeIndex == 2 && reader.status.writeOffset == 24);
    assert(reader.status.indexAllocated == 1);
    assert(reader.status.readIndex == 1 && reader.status.readOffset == 8);
    assert(reader.status.passed == 5 && reader.status.written == 0);
}
int main(void)
{
    properties();
    mask = ~0u;
    updates = setters = 0;
    properties();
    reads();
    cached_ring_status();
    glue_eos_paths();
    puts("libaa match regressions: properties, read wrapping/cache, cached status and ring FULL passed");
    return 0;
}
