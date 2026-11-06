/* Source-extracted host mocks. */
#define _GNU_SOURCE
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libaa/buffers.h"

static unsigned int mask;
struct logmm_module logmm_module_commod = {.mask = &mask};
struct logmm_module logmm_module_buf_com = {.mask = &mask};
struct logmm_module logmm_module_aud = {.mask = &mask};
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
int main(void)
{
    properties();
    mask = ~0u;
    updates = setters = 0;
    properties();
    reads();
    puts("libaa match regressions: property types/callbacks/logging and read wrapping/cache passed");
    return 0;
}
