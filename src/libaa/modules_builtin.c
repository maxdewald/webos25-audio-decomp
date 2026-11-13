/*
 * Built-in libaa module implementations:
 *   - bypass2  : pass-through with byte accounting, "event-after" and real-time pacing
 *   - fakesink : swallows everything
 *   - gluer    : ARM<->ARM shm ring reader (pairs with gluew of another process)
 *   - gluew    : ARM<->ARM shm ring writer
 *   - glue     : legacy ADEC_BUF_T (InterBuf) bridge, also registered as glue_legacy,
 *                glue_proc, gluepw (inter-buffer writer) and gluepr (inter-buffer reader)
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 * Evidence: impl tables bypass2 @0x50954, fakesink @0x50a90, gluer @0x50b04,
 * gluew @0x50d08, glue @0x50ef0 (property tables @0x509b4/0x50b64/0x50d68/0x50f50),
 * gluer state table @0x4b7bc, glue adapter callbacks @0x50ee4; registration in
 * _INIT_1.._INIT_5. Callback bodies decompiled from the table entry points (Ghidra did
 * not discover most of them). Function names come from the __func__ strings in .rodata
 * and the kdriver tree (names only).
 */
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "objects.h"

#define ADEC_CMD_START          0x10010000u
#define ADEC_CMD_STOP           0x10020000u
#define ADEC_CMD_FLUSH          0x1003002cu
#define ADEC_CMD_ASYNC_EVENT    0x10190000u

#define ALIGN8(x)   (((x) + 7u) & ~7u)

LOGMM_MODULE_DEFINE(mod_bypass2);
LOGMM_MODULE_DEFINE(mod_fakesink);
LOGMM_MODULE_DEFINE(mod_gluer);
LOGMM_MODULE_DEFINE(mod_gluew);
LOGMM_MODULE_DEFINE(mod_glue);

void bypass2_initializer(void) __attribute__((constructor));
void fakesink_initializer(void) __attribute__((constructor));
void gluer_initializer(void) __attribute__((constructor));
void gluew_initializer(void) __attribute__((constructor));
void glue_initializer(void) __attribute__((constructor));

_Static_assert(sizeof(struct bypass2) == 0x128, "bypass2 size");
_Static_assert(offsetof(struct gluer, state) == 0x170, "gluer state");
_Static_assert(offsetof(struct gluer, header) == 0x180, "gluer header");
_Static_assert(sizeof(struct gluer) == 0x1c8, "gluer size");
_Static_assert(offsetof(struct gluew, shm_cond) == 0x138, "gluew cond");
_Static_assert(sizeof(struct gluew) == 0x180, "gluew size");
_Static_assert(offsetof(struct mod_glue, meta_adap) == 0x138, "glue adapter");
_Static_assert(sizeof(struct mod_glue) == 0x158, "glue size");
_Static_assert(sizeof(struct glue_header_data) == 0x30, "glue header");

/* ========================================================================= */
/* bypass2                                                                   */
/* ========================================================================= */

/* @0x28878 */
int bypass2_constructor(struct aud_module *m, struct aimc *imc, const char *param)
{
    return 0;
}

/* @0x2887c */
void bypass2_destructor(struct aud_module *m)
{
}

/* @0x28880 */
int bypass2_command(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    return aud_module_default_command(arg, cmd, param, len);
}

/* @0x28884: the first-AU event restarts the accounting and carries the PCM format */
int bypass2_event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    struct bypass2 *b = (struct bypass2 *)m;

    if (e->type == AUD_EVENT_TYPE_FIRST_AU) {
        b->data_passed = 0;
        b->chain_start = 0;
        b->rate = aud_event_value_get_int(e, "rate", 48000);
        b->channels = aud_event_value_get_int(e, "channels", 2);
        LOGI(mod_bypass2, 0x8a, "first. rate %d, channels %d\n", b->rate, b->channels);
    }
    return aud_module_default_event(m, port, e);
}

/* @0x2891c: everything (including meta) is forwarded to output 0. Non-meta bytes are
 * counted for "event-after"; with sleep-sync the chain is paced to real time assuming
 * 16-bit PCM of the announced rate/channels. */
int bypass2_chain(struct aud_port *port, struct aud_data *d)
{
    struct aud_module *m = port->module;
    struct bypass2 *b = (struct bypass2 *)m;
    enum aud_data_type type = d->info.type;

    if (type != type_meta) {
        LOGD(mod_bypass2, 0x4a, "type %d, size %d(%d frames)\n", type, d->size,
             (d->size >> 2) / b->channels);

        b->data_passed += d->size;
        pthread_mutex_lock(&m->prop_access_lock);
        if (b->event_after_set && b->data_passed >= b->event_after_set) {
            if (b->event_after) {
                b->event_after_set += b->event_after;
                b->event_after = 0;
            } else {
                b->event_after_set = 0;
            }
            pthread_mutex_unlock(&m->prop_access_lock);
            aimc_raise_named_event(m->aimc, m->uid, "event-after", NULL, 0);
        } else {
            pthread_mutex_unlock(&m->prop_access_lock);
        }

        if (b->sleep_sync) {
            unsigned long long now = aud_system_cycle();
            long long elapsed_us, data_us, sleep_us;

            if (b->chain_start == 0) {
                b->chain_start = now;
                elapsed_us = 0;
            } else {
                elapsed_us = (long long)(now - b->chain_start) * 1000000 / 1000000000;
            }
            data_us = (long long)((unsigned long long)((long long)b->data_passed * 1000000) >> 2)
                      / b->channels / b->rate;
            sleep_us = data_us - elapsed_us;
            if (sleep_us > 0) {
                LOGD(mod_bypass2, 0x75, "sleep %lldusec\n", sleep_us);
                usleep(sleep_us);
            }
        }
    }
    aud_module_push_data(port->module, 0, d);
    return 0;
}

/* @0x28aec: arm "event-after" relative to what already passed */
int bypass2_prop_updated(struct aud_module *m, const struct aud_module_property *p)
{
    struct bypass2 *b = (struct bypass2 *)m;

    if (p->offset == offsetof(struct bypass2, event_after) && b->event_after_set == 0) {
        int after = b->event_after;

        b->event_after = 0;
        b->event_after_set = b->data_passed + after;
    }
    return 0;
}

static struct aud_module_property bypass2_props[] = {
    { "data-passed", param_type_int, offsetof(struct bypass2, data_passed), AUD_PROP_RO },
    { "event-after-set", param_type_int, offsetof(struct bypass2, event_after_set), AUD_PROP_RO },
    { "event-after", param_type_int, offsetof(struct bypass2, event_after), 0 },
    { "sleep-sync", param_type_bool, offsetof(struct bypass2, sleep_sync), 0 },
    { NULL },
};

static struct aud_module_impl bypass2_impl = {           /* @0x50954 */
    .name = "bypass2",
    .constructor = bypass2_constructor,
    .destructor = bypass2_destructor,
    .chain = bypass2_chain,
    .event = bypass2_event,
    .cmd = bypass2_command,
    .prop_updated = bypass2_prop_updated,
    .instance_size = sizeof(struct bypass2),
    .prop = bypass2_props,
};

/* @0x181c8: init_array */
void bypass2_initializer(void)
{
    ModuleRegistry_Register(&bypass2_impl);
}

/* ========================================================================= */
/* fakesink                                                                  */
/* ========================================================================= */

/* @0x28b18 */
int fakesink_constructor(struct aud_module *m, struct aimc *imc, const char *param)
{
    return 0;
}

/* @0x28b1c */
void fakesink_destructor(struct aud_module *m)
{
}

/* @0x28b20 */
int fakesink_command(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    switch (cmd) {
    case ADEC_CMD_START:
    case ADEC_CMD_STOP:
        return 0;
    }
    return aud_module_default_command(arg, cmd, param, len);
}

/* @0x28b38 */
int fakesink_event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    return aud_module_default_event(m, port, e);
}

/* @0x28b3c */
int fakesink_chain(struct aud_port *port, struct aud_data *d)
{
    aud_data_put(d);
    return 0;
}

static struct aud_module_impl fakesink_impl = {          /* @0x50a90 */
    .name = "fakesink",
    .constructor = fakesink_constructor,
    .destructor = fakesink_destructor,
    .chain = fakesink_chain,
    .event = fakesink_event,
    .cmd = fakesink_command,
    .instance_size = sizeof(struct aud_module),
};

/* @0x181d4: init_array */
void fakesink_initializer(void)
{
    ModuleRegistry_Register(&fakesink_impl);
}

/* ========================================================================= */
/* gluer: reads {glue_header[_data], payload} records from the shm ring that  */
/* the peer gluew writes, rebuilds aud_data/aud_event and pushes them out.    */
/* ========================================================================= */

unsigned int process_header(struct gluer *g, void *mem, unsigned int size);
unsigned int process_data(struct gluer *g, void *mem, unsigned int size);
unsigned int process_data_header(struct gluer *g, void *mem, unsigned int size);
unsigned int process_event(struct gluer *g, void *mem, unsigned int size);
int changeto_header(struct gluer *g);
int changeto_data(struct gluer *g);
int changeto_data_header(struct gluer *g);
int changeto_event(struct gluer *g);
int gluer_read(struct aud_module *m, void *arg);

enum { GLUER_HEADER, GLUER_DATA, GLUER_DATA_HEADER, GLUER_EVENT };

static const struct process_state gluer_states[] = {     /* @0x4b7bc */
    [GLUER_HEADER] = { "header", process_header, changeto_header },
    [GLUER_DATA] = { "data", process_data, changeto_data },
    [GLUER_DATA_HEADER] = { "data_header", process_data_header, changeto_data_header },
    [GLUER_EVENT] = { "event", process_event, changeto_event },
};

static inline __attribute__((always_inline)) void _change_state_to(struct gluer *g, const struct process_state *s)
{
    LOGT(mod_gluer, 0xd2, "change state from %s to %s\n", g->state.name, s->name);
    s->changeto(g);
    g->state = *s;
}

/* @0x28b48: gluer has no input */
int gluer_chain(struct aud_port *port, struct aud_data *d)
{
    return 0;
}

/* @0x28b4c */
int changeto_header(struct gluer *g)
{
    g->header_gathered = 0;
    return 0;
}

/* @0x28b58: data_header continues filling the header gathered so far */
int changeto_data_header(struct gluer *g)
{
    return 0;
}

/* @0x28b5c */
int gluer_dot(struct aud_module *m)
{
    struct gluer *g = (struct gluer *)m;
    char *s, *c;

    aud_module_default_dot(m);
    logm_message(" subgraph cluster_proc_%d {\n", m->uid >> 24);
    logm_message(" subgraph cluster_%s_%p {\n", m->name, m);
    logm_message("  proc_%s[label=\"process\"]\n", m->name);
    logm_message(" }}\n");
    if (g->shm_name && (s = strdup(g->shm_name))) {
        for (c = s; *c; c++)
            if (*c == ':' || *c == '-')
                *c = '_';
        logm_message(" %s -> proc_%s[label=\"r/w %d/%d\"]\n", s, m->name, g->shm_rd, g->shm_wr);
        free(s);
    }
    return 0;
}

static void gluer_map_shbuf(struct gluer *g)
{
    int fd, err;
    void *mem;

    LOGF(mod_gluer, log_level_info, "map_shbuf", 0x90,
          "attach shared memory. name %s, size 0x%x\n", g->shm_name, g->shm_size);
    fd = shm_open(g->shm_name, O_RDONLY, 0600);
    if (fd < 0) {
        err = errno;
        LOGF(mod_gluer, log_level_error, "map_shbuf", 0x94, "shm_open() failed. %d(%s)\n",
              err, strerror(err));
        return;
    }
    mem = mmap(NULL, g->shm_size, PROT_READ, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        g->shm_mem = NULL;
        err = errno;
        LOGF(mod_gluer, log_level_error, "map_shbuf", 0x9b, "mmap() failed. %d(%s)\n",
              err, strerror(err));
    } else {
        g->shm_mem = mem;
    }
    close(fd);
}

/* @0x28bec: GLUE_CMD_UPDATE from the writer announces its write offset. The shm is
 * attached on the first update; a new age means the writer reset its ring. */
int gluer_command(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    struct gluer *g = arg;
    struct glue_cmd_update_param *u = param;

    if (cmd != GLUE_CMD_UPDATE)
        return aud_module_default_command(arg, cmd, param, len);

    pthread_mutex_lock(&g->shm_lock);
    LOGF(mod_gluer, log_level_trace, "cmd_update", 0xa9, "update... rd/wr %04x/%04x\n",
          g->shm_rd, u->offs_wr);
    if (g->shm_size != u->size)
        g->shm_size = u->size;
    if (!g->shm_mem)
        gluer_map_shbuf(g);
    if (g->shm_age != u->age) {
        LOGF(mod_gluer, log_level_info, "cmd_update", 0xb2, "got new age %d\n", u->age);
        g->shm_age = u->age;
        g->shm_rd = ~0u;
    }
    g->shm_wr = u->offs_wr;
    pthread_cond_broadcast(&g->shm_cond);
    pthread_mutex_unlock(&g->shm_lock);
    return 0;
}

/* @0x28db8 */
int gluer_event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    return aud_module_default_event(m, port, e);
}

/* @0x28dbc */
void gluer_destructor(struct aud_module *m)
{
    struct gluer *g = (struct gluer *)m;

    aud_module_output_thread_destroy(m);
    if (g->gathering_event)
        free(g->gathering_event);
    if (g->gathering_data)
        aud_data_put(g->gathering_data);
    g->state.name = NULL;
}

/* @0x28de0 */
int changeto_event(struct gluer *g)
{
    LOGT(mod_gluer, 0x151, "new event. size %d\n", g->header.header.size);
    g->event_gathered = 0;
    if (g->gathering_event_allocated < g->header.header.size) {
        g->gathering_event = realloc(g->gathering_event, g->header.header.size);
        g->gathering_event_allocated = g->header.header.size;
    }
    return 0;
}

/* @0x28e54 */
int changeto_data(struct gluer *g)
{
    g->data_gathered = 0;
    if (g->gathering_data)
        aud_data_put(g->gathering_data);
    LOGT(mod_gluer, 0x124, "new data. size %d\n", g->header.header.size);
    g->gathering_data = aud_data_new(g->header.header.size);
    if (!g->gathering_data) {
        LOGE(mod_gluer, 0x128, "no mem\n");
        return 0;
    }
    g->gathering_data->info = g->header.info;
    return 0;
}

/* @0x28f2c */
int gluer_constructor(struct aud_module *m, struct aimc *imc, const char *param)
{
    struct gluer *g = (struct gluer *)m;

    pthread_cond_init(&g->shm_cond, NULL);
    pthread_mutex_init(&g->shm_lock, NULL);
    _change_state_to(g, &gluer_states[GLUER_HEADER]);
    aud_module_output_thread_create(m, gluer_read, g, &g->shm_lock, &g->shm_cond);
    return 0;
}

/* @0x28fcc: rest of a data record header (bytes 0x0c..0x30) */
unsigned int process_data_header(struct gluer *g, void *mem, unsigned int size)
{
    if (g->header_gathered + size > sizeof(g->header))
        size = sizeof(g->header) - g->header_gathered;
    memcpy((char *)&g->header + g->header_gathered, mem, size);
    g->header_gathered += size;
    if (g->header_gathered >= (int)sizeof(g->header))
        _change_state_to(g, &gluer_states[GLUER_DATA]);
    return size;
}

/* @0x29070: once the payload is complete the aud_data goes out of port 0 (unlocked) */
unsigned int process_data(struct gluer *g, void *mem, unsigned int size)
{
    struct aud_data *d;

    if (g->data_gathered + size > g->header.header.size)
        size = g->header.header.size - g->data_gathered;
    memcpy(g->gathering_data->data + g->data_gathered, mem, size);
    g->data_gathered += size;
    if (g->data_gathered < g->header.header.size)
        return size;

    d = g->gathering_data;
    g->gathering_data = NULL;
    LOGD(mod_gluer, 0x142, "push data. type %d, size %d\n", d->info.type, d->size);
    pthread_mutex_unlock(&g->shm_lock);
    aud_module_push_data(&g->module, 0, d);
    aud_module_sync(&g->module, 0);
    pthread_mutex_lock(&g->shm_lock);
    _change_state_to(g, &gluer_states[GLUER_HEADER]);
    return size;
}

/* @0x29178: 12-byte common header, then either an event or the rest of a data header */
unsigned int process_header(struct gluer *g, void *mem, unsigned int size)
{
    if (g->header_gathered + size > sizeof(struct glue_header))
        size = sizeof(struct glue_header) - g->header_gathered;
    memcpy((char *)&g->header + g->header_gathered, mem, size);
    g->header_gathered += size;
    if (g->header_gathered < (int)sizeof(struct glue_header))
        return size;

    g->au_count = g->header.header.au_count;
    if (g->header.header.type != glue_data_type_data)
        _change_state_to(g, &gluer_states[GLUER_EVENT]);
    else
        _change_state_to(g, &gluer_states[GLUER_DATA_HEADER]);
    return size;
}

/* @0x29274 */
unsigned int process_event(struct gluer *g, void *mem, unsigned int size)
{
    struct aud_event *e;

    if (g->event_gathered + size > g->header.header.size)
        size = g->header.header.size - g->event_gathered;
    memcpy((char *)g->gathering_event + g->event_gathered, mem, size);
    g->event_gathered += size;
    if (g->event_gathered < g->header.header.size)
        return size;

    e = aud_event_deserialize(g->gathering_event, g->event_gathered);
    if (!e) {
        LOGE(mod_gluer, 0x16d, "no mem?\n");
    } else {
        LOGI(mod_gluer, 0x170, "push event. type %d(%s)\n", e->type, aud_event_name(e));
        pthread_mutex_unlock(&g->shm_lock);
        aud_module_push_event(&g->module, 0, e);
        aud_module_sync(&g->module, 0);
        pthread_mutex_lock(&g->shm_lock);
    }
    _change_state_to(g, &gluer_states[GLUER_HEADER]);
    return size;
}

/* @0x293b0: output-thread body, runs with shm_lock held. When the ring is empty the
 * read position is reported back to the writer (which may be blocked in write_data)
 * and the thread sleeps until the next GLUE_CMD_UPDATE. */
int gluer_read(struct aud_module *m, void *arg)
{
    struct gluer *g = arg;
    unsigned int rd, wr, avail, n;

    if (g->shm_rd == ~0u) {
        g->shm_rd = 0;
        LOGI(mod_gluer, 0x19b, "buffer reset\n");
        _change_state_to(g, &gluer_states[GLUER_HEADER]);
    }
    LOGT(mod_gluer, 0x19f, "rd/wr %04x/%04x\n", g->shm_rd, g->shm_wr);

    rd = g->shm_rd;
    wr = g->shm_wr;
    if (rd == wr) {
        if (g->cowork_uid) {
            struct glue_cmd_update_param u = { 0 };

            LOGF(mod_gluer, log_level_trace, "report_coworker", 0x185,
                  "report coworker. %08x, rd/wr %05x/%05x, au_count %d\n",
                  g->cowork_uid, rd, wr, g->au_count);
            u.offs_rd = g->shm_rd;
            u.au_count = g->au_count;
            u.age = g->shm_age;
            aimc_send_command(m->aimc, GLUE_CMD_UPDATE, m->uid, g->cowork_uid, &u, sizeof(u));
        }
        pthread_cond_wait(&g->shm_cond, &g->shm_lock);
        return 0;
    }

    avail = rd <= wr ? wr - rd : g->shm_size - rd;
    LOGT(mod_gluer, 0x1ac, "rd/wr %05x/%05x, size %5d\n", rd, wr, avail);
    n = g->state.process(g, (char *)g->shm_mem + g->shm_rd, avail);
    if (g->shm_rd != ~0u)
        g->shm_rd = (g->shm_rd + n) % g->shm_size;
    return 0;
}

static struct aud_module_property gluer_props[] = {
    { "cowork-uid", param_type_int, offsetof(struct gluer, cowork_uid), 0 },
    { "shm-size", param_type_int, offsetof(struct gluer, shm_size), 0 },
    { "shm-name", param_type_str, offsetof(struct gluer, shm_name), 0 },
    { "shm-rd", param_type_int, offsetof(struct gluer, shm_rd), AUD_PROP_RO },
    { "shm-wr", param_type_int, offsetof(struct gluer, shm_wr), 0 },
    { "state-name", param_type_str, offsetof(struct gluer, state.name), AUD_PROP_RO },
    { "header-gathered", param_type_int, offsetof(struct gluer, header_gathered), AUD_PROP_RO },
    { "data-gathered", param_type_int, offsetof(struct gluer, data_gathered), AUD_PROP_RO },
    { "event_gathered", param_type_int, offsetof(struct gluer, event_gathered), AUD_PROP_RO },
    { NULL },
};

static struct aud_module_impl gluer_impl = {             /* @0x50b04 */
    .name = "gluer",
    .constructor = gluer_constructor,
    .destructor = gluer_destructor,
    .chain = gluer_chain,
    .event = gluer_event,
    .cmd = gluer_command,
    .dot = gluer_dot,
    .instance_size = sizeof(struct gluer),
    .prop = gluer_props,
};

/* @0x181e0: init_array */
void gluer_initializer(void)
{
    ModuleRegistry_Register(&gluer_impl);
}

/* ========================================================================= */
/* gluew: writes records into a POSIX shm ring read by the peer gluer.        */
/* ========================================================================= */

/* @0x29594 */
int gluew_dot(struct aud_module *m)
{
    struct gluew *g = (struct gluew *)m;
    char *s, *c;

    aud_module_default_dot(m);
    logm_message(" subgraph cluster_proc_%d {\n", m->uid >> 24);
    logm_message(" subgraph cluster_%s_%p {\n", m->name, m);
    logm_message("  proc_%s[label=\"process\"]\n", m->name);
    logm_message(" }}\n");
    if (g->shm_name && (s = strdup(g->shm_name))) {
        for (c = s; *c; c++)
            if (*c == ':' || *c == '-')
                *c = '_';
        logm_message(" proc_%s -> %s[label=\"r/w %d/%d\"]\n", m->name, s, g->shm_rd, g->shm_wr);
        free(s);
    }
    return 0;
}

/* @0x29620: GLUE_CMD_UPDATE from the reader frees ring space (stale ages ignored) */
int gluew_command(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    struct gluew *g = arg;
    struct glue_cmd_update_param *u = param;

    if (cmd != GLUE_CMD_UPDATE)
        return aud_module_default_command(arg, cmd, param, len);

    pthread_mutex_lock(&g->shm_lock);
    LOGF(mod_gluew, log_level_trace, "cmd_update", 0x68,
          "update... rd/wr %05x/%05x, au_count %d\n", u->offs_rd, g->shm_wr, u->au_count);
    if (g->shm_age == u->age) {
        g->shm_rd = u->offs_rd;
        g->reader_au_count = u->au_count;
    } else {
        LOGF(mod_gluew, log_level_info, "cmd_update", 0x6d, "ignore old age %d. my %d\n",
              u->age, g->shm_age);
    }
    pthread_cond_broadcast(&g->shm_cond);
    pthread_mutex_unlock(&g->shm_lock);
    return 0;
}

/* @0x296e4 */
void gluew_destructor(struct aud_module *m)
{
    struct gluew *g = (struct gluew *)m;

    if (g->shm_mem)
        munmap(g->shm_mem, g->shm_size);
}

/* @0x296f8: at most one AU may be outstanding at the reader */
int gluew_constructor(struct aud_module *m, struct aimc *imc, const char *param)
{
    struct gluew *g = (struct gluew *)m;

    g->max_au_count = 1;
    pthread_cond_init(&g->shm_cond, NULL);
    pthread_mutex_init(&g->shm_lock, NULL);
    return 0;
}

/* @0x2971c: tell the reader where the write pointer is */
void gluew_report_coworker(struct gluew *g)
{
    struct glue_cmd_update_param u = { 0 };

    LOGF(mod_gluew, log_level_trace, "report_coworker", 0x8c,
          "report coworker, %08x. wr %05x\n", g->cowork_uid, g->shm_wr);
    u.offs_wr = g->shm_wr;
    u.size = g->shm_size;
    u.age = g->shm_age;
    aimc_send_command(g->module.aimc, GLUE_CMD_UPDATE, g->module.uid, g->cowork_uid, &u,
                      sizeof(u));
}

/* @0x297a8: copy into the ring with shm_lock held. 4 bytes always stay free so that
 * rd == wr means empty. Blocks (after reporting) while the ring is full or the reader
 * is more than max_au_count AUs behind; returns -1 if a reset was requested meanwhile. */
int gluew_write_data(struct gluew *g, void *data, unsigned int size)
{
    unsigned int written = 0;

    LOGF(mod_gluew, log_level_trace, "write_data", 0x9a, "write size 0x%x\n", size);
    while (written < size) {
        unsigned int rd = g->shm_rd, wr = g->shm_wr;
        unsigned int space, n, tail;

        space = rd > wr ? rd - wr : g->shm_size + rd - wr;
        space = space ? space - 4 : 0;
        LOGF(mod_gluew, log_level_trace, "write_data", 0xa9,
              "written/size %5d/%5d, rd/wr %05x/%05x, free %05x, rd/wr au_count %5d/%5d\n",
              written, size, rd, wr, space, g->reader_au_count, g->au_count);

        if (rd != wr && (g->au_count - g->reader_au_count > g->max_au_count || space == 0)) {
            gluew_report_coworker(g);
            pthread_cond_wait(&g->shm_cond, &g->shm_lock);
            if (g->reset_buf) {
                LOGF(mod_gluew, log_level_info, "write_data", 0xb2, "reset..\n");
                return -1;
            }
            continue;
        }

        n = size - written < space ? size - written : space;
        tail = g->shm_size - wr;
        if (tail < n) {
            LOGF(mod_gluew, log_level_trace, "write_data", 0xc6,
                  "write. r/w %04x/%04x, size1/2 %04x/%04x\n", rd, wr, tail, n - tail);
            memcpy((char *)g->shm_mem + wr, (char *)data + written, tail);
            if (n - tail)
                memcpy(g->shm_mem, (char *)data + written + tail, n - tail);
        } else {
            LOGF(mod_gluew, log_level_trace, "write_data", 0xc6,
                  "write. r/w %04x/%04x, size1/2 %04x/%04x\n", rd, wr, n, 0);
            memcpy((char *)g->shm_mem + wr, (char *)data + written, n);
        }
        written += n;
        g->shm_wr = (g->shm_wr + n) % g->shm_size;
    }
    return 0;
}

static void *gluew_map_shbuf(struct gluew *g)
{
    int fd, err;
    void *mem;

    if (!g->shm_name || !g->shm_size)
        return NULL;
    LOGF(mod_gluew, log_level_info, "map_shbuf", 0x118,
          "attach shared memory. name %s, size 0x%x\n", g->shm_name, g->shm_size);
    fd = shm_open(g->shm_name, O_CREAT | O_RDWR, 0600);
    if (fd < 0) {
        err = errno;
        LOGF(mod_gluew, log_level_error, "map_shbuf", 0x11c, "shm_open() failed. %d(%s)\n",
              err, strerror(err));
        return g->shm_mem;
    }
    if (ftruncate(fd, g->shm_size) < 0) {
        err = errno;
        LOGF(mod_gluew, log_level_error, "map_shbuf", 0x120, "ftruncate() failed. %d(%s)\n",
              err, strerror(err));
    }
    mem = mmap(NULL, g->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        g->shm_mem = NULL;
        err = errno;
        LOGF(mod_gluew, log_level_error, "map_shbuf", 0x126, "mmap() failed. %d(%s)\n",
              err, strerror(err));
    } else {
        g->shm_mem = mem;
    }
    close(fd);
    return g->shm_mem;
}

static void gluew_request_reset(struct gluew *g)
{
    pthread_mutex_lock(&g->shm_lock);
    g->reset_buf = true;
    pthread_cond_broadcast(&g->shm_cond);
    pthread_mutex_unlock(&g->shm_lock);
}

/* @0x299b8: async downward events go to the reader by IMC (DISCONNECT is dropped and,
 * like FLUSH, resets the ring); sync events travel in-band through the ring. The event
 * is then handled locally as usual. Nothing is sent before the shm can be mapped. */
int gluew_event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    struct gluew *g = (struct gluew *)m;
    unsigned int len;
    void *buf;

    LOGI(mod_gluew, 0x13b, "event. %s\n", aud_event_name(e));
    if (!g->shm_mem && !gluew_map_shbuf(g))
        return aud_module_default_event(m, port, e);

    if ((e->prop & (AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC)) == AUD_EVENT_PROP_DOWNWARD) {
        if (e->type == AUD_EVENT_TYPE_DISCONNECT) {
            LOGI(mod_gluew, 0x152, "async downward event.%s%s\n", " drop.", " reset.");
            gluew_request_reset(g);
        } else {
            LOGI(mod_gluew, 0x152, "async downward event.%s%s\n", "",
                 e->type == AUD_EVENT_TYPE_FLUSH ? " reset." : "");
            buf = aud_event_serialize(e, &len);
            if (!buf) {
                PLOG(m, log_level_error, 0x161, "glue%p, no mem\n", m);
            } else {
                aimc_send_command(m->aimc, ADEC_CMD_ASYNC_EVENT, m->uid, g->cowork_uid, buf, len);
                free(buf);
            }
            if (e->type == AUD_EVENT_TYPE_FLUSH)
                gluew_request_reset(g);
        }
    }

    if (AUD_EVENT_IS_SYNC(e)) {
        if (e->type == AUD_EVENT_TYPE_DISCONNECT) {
            LOGI(mod_gluew, 0x176, "sync downward event.%s\n", " drop.");
        } else {
            LOGI(mod_gluew, 0x176, "sync downward event.%s\n", "");
            buf = aud_event_serialize(e, &len);
            if (!buf) {
                PLOG(m, log_level_error, 0x18c, "glue%p, no mem\n", m);
            } else {
                struct glue_header h = { 0 };

                LOGI(mod_gluew, 0x17e, "sync downward event. size %d\n", len);
                h.type = glue_data_type_event;
                h.size = len;
                h.au_count = g->au_count++;
                pthread_mutex_lock(&g->shm_lock);
                gluew_write_data(g, &h, sizeof(h));
                gluew_write_data(g, buf, len);
                gluew_report_coworker(g);
                pthread_mutex_unlock(&g->shm_lock);
                free(buf);
            }
        }
    }
    LOGD(mod_gluew, 0x18f, "done.\n");
    return aud_module_default_event(m, port, e);
}

/* @0x29e1c: one record per aud_data. A canceled write (reset requested) restarts the
 * ring under a new age so the reader resynchronises; the data is lost. */
int gluew_chain(struct aud_port *port, struct aud_data *d)
{
    struct gluew *g = (struct gluew *)port->module;
    struct glue_header_data h = { 0 };

    h.header.type = glue_data_type_data;
    h.header.size = d->size;
    h.header.au_count = g->au_count++;
    h.info = d->info;

    pthread_mutex_lock(&g->shm_lock);
    LOGD(mod_gluew, 0xf9, "write data. size %d\n", d->size);
    if (!g->reset_buf && gluew_write_data(g, &h, sizeof(h)) >= 0 &&
        gluew_write_data(g, d->data, d->size) >= 0) {
        pthread_mutex_unlock(&g->shm_lock);
        gluew_report_coworker(g);
    } else {
        LOGI(mod_gluew, 0x103, "chain() canceled.\n");
        g->reset_buf = false;
        g->shm_rd = 0;
        g->shm_wr = 0;
        g->au_count = 0;
        g->shm_age++;
        g->reader_au_count = 0;
        pthread_mutex_unlock(&g->shm_lock);
    }
    aud_data_put(d);
    return 0;
}

static struct aud_module_property gluew_props[] = {
    { "cowork-uid", param_type_int, offsetof(struct gluew, cowork_uid), 0 },
    { "shm-size", param_type_int, offsetof(struct gluew, shm_size), 0 },
    { "shm-name", param_type_str, offsetof(struct gluew, shm_name), 0 },
    { "shm-rd", param_type_int, offsetof(struct gluew, shm_rd), 0 },
    { "shm-wr", param_type_int, offsetof(struct gluew, shm_wr), AUD_PROP_RO },
    { "au-count", param_type_int, offsetof(struct gluew, au_count), AUD_PROP_RO },
    { "reader-au-count", param_type_int, offsetof(struct gluew, reader_au_count), AUD_PROP_RO },
    { "max-au-count", param_type_int, offsetof(struct gluew, max_au_count), AUD_PROP_RO },
    { NULL },
};

static struct aud_module_impl gluew_impl = {             /* @0x50d08 */
    .name = "gluew",
    .constructor = gluew_constructor,
    .destructor = gluew_destructor,
    .chain = gluew_chain,
    .event = gluew_event,
    .cmd = gluew_command,
    .dot = gluew_dot,
    .instance_size = sizeof(struct gluew),
    .prop = gluew_props,
};

/* @0x181ec: init_array */
void gluew_initializer(void)
{
    ModuleRegistry_Register(&gluew_impl);
}

/* ========================================================================= */
/* glue: bridge to a legacy inter-processor ADEC_BUF_T ring. As writer        */
/* ("gluepw") the ring is the legacy_buf of output port 0, filled by           */
/* aud_port_sync; as reader ("gluepr") a starter thread drains the ring into   */
/* output port 0. The peer reports ring progress with BM_CMD_PROCESS_EVT_REMOTE.*/
/* ========================================================================= */

#define GLOG(g, lvl, line, fmt, ...) PLOG(&(g)->module, lvl, line, fmt, ##__VA_ARGS__)

/* @0x29f34: shared memory is mapped 1:1 */
void *Mem_Translator(void *addr, struct _ADEC_BUF_T *buf)
{
    return addr;
}

/* @0x29f38 */
int Glue_DeletingPort(struct aud_module *m, struct aud_port *port)
{
    struct mod_glue *g = (struct mod_glue *)m;

    GLOG(g, log_level_info, 0x2f2, "glue%p, deleting port\n", g);
    if (g->starter) {
        GLOG(g, log_level_info, 0x2f5, "glue%p, stop starter\n", g);
        aud_starter_stop(g->starter);
    }
    aud_data_adapter_cleanup(&g->meta_adap);
    return 0;
}

void Cache_Writeback(void *addr, int len, struct _ADEC_BUF_T *buf);
void Cache_Invalidate(void *addr, int len, struct _ADEC_BUF_T *buf);

static ADEC_PROCESSOR_ADAP_IF_T glue_adap = {             /* @0x50ee4 */
    Cache_Invalidate, Cache_Writeback, Mem_Translator,
};

/* @0x29fac: map the shared block and build an InterBuf reader/writer on it */
struct _ADEC_BUF_T *CreateBuffer(struct mod_glue *g, GlueCmdSetSharedBuffer *p, int reader)
{
    void *mem;
    int status_size;
    struct _ADEC_BUF_T *buf;

    mem = aimc_mmap(g->imc, p->shared_buf_offset, p->shared_buf_size);
    if (!mem) {
        GLOG(g, log_level_error, 0xa8, "glue%p, can not create common buffer\n", g);
        return NULL;
    }
    g->shmem_mapped = mem;
    g->shmem_size = p->shared_buf_size;
    status_size = CommonBuffer_GetStructSize(p->aui_count);
    GLOG(g, log_level_info, 0xad, "glue%p, offset %x, common_buf %p, coworker 0x%x\n", g,
         p->shared_buf_offset, mem, p->cowork_module_uid);

    if (!reader) {
        buf = malloc(InterBufWriter_GetStructSize());
        if (!buf)
            goto no_mem;
        memset(buf, 0, InterBufWriter_GetStructSize());
        InterBufWriter_Create(&glue_adap, mem, p->shared_buf_size, p->max_access_size,
                              p->aui_count, (char *)mem + status_size, p->cowork_module_uid,
                              g->imc, buf);
    } else {
        buf = malloc(InterBufReader_GetStructSize());
        if (!buf)
            goto no_mem;
        memset(buf, 0, InterBufReader_GetStructSize());
        InterBufReader_Create(&glue_adap, mem, p->shared_buf_size, p->max_access_size,
                              p->aui_count, (char *)mem + status_size, p->cowork_module_uid,
                              g->imc, buf);
    }
    buf->offset = p->shared_buf_offset;
    g->shbuf_offset = p->shared_buf_offset;
    g->cowork_uid = p->cowork_module_uid;
    return buf;

no_mem:
    GLOG(g, log_level_error, 0xb7, "glue%p, no mem\n", g);
    return NULL;
}

static void Glue_DestroyBuffer(struct mod_glue *g, struct _ADEC_BUF_T *buf)
{
    if (g->shmem_mapped) {
        aimc_munmap(g->imc, buf->offset, g->shmem_mapped, g->shmem_size);
        g->shmem_mapped = NULL;
        g->shmem_size = 0;
    }
    buf->unref(buf);
}

/* @0x2a114: writer side, the ring becomes output 0's legacy buffer */
int set_shbuf_pw(struct mod_glue *g, GlueCmdSetSharedBuffer *p)
{
    struct aud_port *port;

    GLOG(g, log_level_info, 0x1f0, "glue%p, Glue is inter_buffer writer\n", g);
    port = aud_module_new_output_port(&g->module, 0);
    if (!port) {
        GLOG(g, log_level_error, 0x1f5, "glue%p, no port\n", g);
        return -1;
    }
    g->shared_buf = CreateBuffer(g, p, 0);
    if (!g->shared_buf) {
        GLOG(g, log_level_error, 0x1fc, "glue%p, no glue\n", g);
        aud_module_del_output_port(&g->module, 0);
        return -1;
    }
    pthread_mutex_lock(&port->check_free_mutex);
    port->legacy_buf = g->shared_buf;
    pthread_mutex_unlock(&port->check_free_mutex);
    return 0;
}

/* @0x2a1ec */
void Cache_Writeback(void *addr, int len, struct _ADEC_BUF_T *buf)
{
    aimc_cache_sync(buf->aimc, 1, buf->offset, (char *)addr - (char *)buf->commonBuf, len);
}

/* @0x2a210 */
void Cache_Invalidate(void *addr, int len, struct _ADEC_BUF_T *buf)
{
    aimc_cache_sync(buf->aimc, 0, buf->offset, (char *)addr - (char *)buf->commonBuf, len);
}

static void push_zero_buffers(struct mod_glue *g, int count, int size)
{
    int i;

    for (i = 0; i < count; i++)
        aud_module_push_data(&g->module, 0, aud_data_new_zero(size));
}

/* @0x2a234: async events cross to the coworker by IMC (writer: downward ones, reader:
 * upward ones). Without output_buffer, non-FIRST events use default propagation:
 * synchronous custom eos=last and LAST enter output 0's FIFO as separate serialized
 * EVENT AUs, not IMC async commands. With output_buffer, FIRST/LAST pass on but custom
 * EOS is put/dropped (@0x2a26c); LAST tail-calls push_event (@0x2a38c). FIRST may
 * pre-fill with silent AUs. push_event can sync inline and wait for ring space;
 * its return is ignored in FIRST paths, propagated in the output_buffer LAST path.
 * No EOS-completion acknowledgement or deadline is implemented by this handler. */
int Glue_Event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    struct mod_glue *g = (struct mod_glue *)m;
    struct _ADEC_BUF_T *lbuf;
    int max_au, au_cnt, size, with_zero;

    GLOG(g, log_level_info, 0x162, "%s, event %s\n", m->name, aud_event_name(e));

    if (!AUD_EVENT_IS_SYNC(e) && g->cowork_uid &&
        (g->starter ? !AUD_EVENT_IS_DOWNWARD(e) : AUD_EVENT_IS_DOWNWARD(e))) {
        unsigned int len;
        void *buf;

        if (e->type == AUD_EVENT_TYPE_DISCONNECT)
            return aud_module_default_event(m, port, e);
        buf = aud_event_serialize(e, &len);
        if (!buf) {
            GLOG(g, log_level_error, 0x17c, "glue%p, no mem\n", g);
        } else {
            aimc_send_command(m->aimc, ADEC_CMD_ASYNC_EVENT, m->uid, g->cowork_uid, buf, len);
            free(buf);
        }
    }

    if (!g->output_buffer) {
        struct aud_port *out;

        if (e->type != AUD_EVENT_TYPE_FIRST_AU || !(out = aud_module_get_output_port(m, 0)))
            return aud_module_default_event(m, port, e);
        aud_module_push_event(m, 0, aud_event_get(e));
        lbuf = out->legacy_buf;
        if (lbuf) {
            max_au = lbuf->get_max_au(lbuf);
            au_cnt = lbuf->get_au_cnt(lbuf);
            size = aud_event_value_get_int(e, "processing-size", g->last_data_size);
            if (aud_event_value_get_int(e, "with-zero-buffer", 0)) {
                GLOG(g, log_level_info, 0x1bf, "glue%p, make zero data. %d\n", g, max_au - au_cnt);
                push_zero_buffers(g, max_au - au_cnt - 1, size);
            }
        }
        aud_event_put(e);
        return 0;
    }

    switch (e->type) {
    case AUD_EVENT_TYPE_FIRST_AU:
        lbuf = g->output_buffer;
        max_au = lbuf->get_max_au(lbuf);
        au_cnt = lbuf->get_au_cnt(lbuf);
        size = aud_event_value_get_int(e, "processing-size", g->last_data_size);
        with_zero = aud_event_value_get_int(e, "with-zero-buffer", 0);
        aud_module_push_event(m, 0, e);
        if (!with_zero)
            return 0;
        GLOG(g, log_level_info, 0x193, "glue%p, make zero data. au %d, size %d\n", g,
             max_au - au_cnt, size);
        push_zero_buffers(g, max_au - au_cnt - 1, size);
        return 0;
    case AUD_EVENT_TYPE_LAST_AU:
        return aud_module_push_event(m, 0, e);
    case AUD_EVENT_TYPE_DISCONNECT:
        return aud_module_default_event(m, port, e);
    default:
        GLOG(g, log_level_debug, 0x1a1, "drop event(%s)\n", aud_event_name(e));
        aud_event_put(e);
        return 0;
    }
}

/* queued meta records + this buffer -> one type_mixed buffer of
 * {u32 type, u32 size, data[size] padded to 8} records */
static struct aud_data *get_merged(struct mod_glue *g, struct aud_data *d)
{
    struct aud_data *merged, *meta;
    unsigned int off = 0;
    int i;

    merged = aud_data_new(ALIGN8(d->size) + g->aligned_meta_size + g->meta_count * 8 + 8);
    if (!merged) {
        GLOG(g, log_level_error, 0xf4, "Oops\n");
        aud_data_adapter_flush_all(&g->meta_adap);
        return d;
    }
    merged->info = d->info;
    merged->info.type = type_mixed;
    for (i = 0; i < g->meta_count; i++) {
        meta = aud_data_adapter_dequeue(&g->meta_adap);
        ((unsigned int *)(merged->data + off))[0] = meta->info.type;
        ((unsigned int *)(merged->data + off))[1] = meta->size;
        memcpy(merged->data + off + 8, meta->data, meta->size);
        off += ALIGN8(meta->size) + 8;
        aud_data_put(meta);
    }
    ((unsigned int *)(merged->data + off))[0] = d->info.type;
    ((unsigned int *)(merged->data + off))[1] = d->size;
    memcpy(merged->data + off + 8, d->data, d->size);
    off += ALIGN8(d->size) + 8;
    if (off != merged->size)
        LOGE(mod_glue, 0x113, "Oops. %d %d\n", off, merged->size);
    aud_data_put(d);
    return merged;
}

/* @0x2a4d4: meta buffers are held back and merged into the next PCM/ES buffer */
int Glue_Chain(struct aud_port *port, struct aud_data *d)
{
    struct mod_glue *g = (struct mod_glue *)port->module;

    GLOG(g, log_level_debug, 0x11c, "glue%p, type %d, chain %d bytes\n", g, d->info.type,
         d->size);
    if (g->merge_meta_pcm) {
        if (d->info.type == type_meta || d->info.type == type_meta_ext) {
            aud_data_adapter_push(&g->meta_adap, d);
            g->meta_count++;
            g->aligned_meta_size += ALIGN8(d->size);
            return 0;
        }
        if (g->meta_count > 0) {
            d = get_merged(g, d);
            g->meta_count = 0;
            g->aligned_meta_size = 0;
        }
    }
    g->last_data_size = d->size;
    aud_module_push_data(port->module, 0, d);
    return 0;
}

/* @0x2a6cc */
int Glue_Constructor(struct aud_module *m, struct aimc *imc, const char *param)
{
    struct mod_glue *g = (struct mod_glue *)m;

    GLOG(g, log_level_info, 0x301, "constructor (module=%p)\n", g);
    g->imc = imc;
    g->last_data_size = 0x800;
    g->merge_meta_pcm = true;
    aud_data_adapter_init(&g->meta_adap);
    return 0;
}

/* @0x2a720: reader side, a starter thread drains the ring into output 0 */
int set_shbuf_pr(struct mod_glue *g, GlueCmdSetSharedBuffer *p)
{
    struct aud_port *port;

    GLOG(g, log_level_info, 0x1d0, "glue%p, Glue is inter_buffer reader\n", g);
    g->shared_buf = CreateBuffer(g, p, 1);
    if (!g->shared_buf) {
        GLOG(g, log_level_error, 0x1d5, "glue%p, no glue\n", g);
        return -1;
    }
    g->starter = aud_starter_new(g->shared_buf, "starter_%08x", g->module.uid);
    if (!g->starter) {
        GLOG(g, log_level_error, 0x1dc, "glue%p, can not create starter\n", g);
        Glue_DestroyBuffer(g, g->shared_buf);
        g->shared_buf = NULL;
        return -1;
    }
    port = aud_module_get_output_port(&g->module, 0);
    if (!port && !(port = aud_module_new_output_port(&g->module, 0)))
        return 0;
    aud_starter_add_port(g->starter, port);
    return 0;
}

/* @0x2a834 */
void Glue_Destructor(struct aud_module *m)
{
    struct mod_glue *g = (struct mod_glue *)m;

    GLOG(g, log_level_info, 0x146, "destructor (module=%p)\n", g);
    if (g->starter) {
        GLOG(g, log_level_info, 0x14a, "glue%p, delete starter..\n", g);
        aud_starter_stop(g->starter);
        aud_starter_del(g->starter);
    }
    if (g->shared_buf)
        Glue_DestroyBuffer(g, g->shared_buf);
    if (aud_module_get_output_port(m, 0))
        aud_module_del_output_port(m, 0);
    aud_data_adapter_cleanup(&g->meta_adap);
}

static int SetSharedBuffer(struct mod_glue *g, GlueCmdSetSharedBuffer *p)
{
    struct aud_module *m = &g->module;
    struct aud_port *out = aud_module_get_output_port(m, 0);
    struct aud_port *in = aud_module_get_input_port(m, 0);
    const char *name;

    if (g->shared_buf) {
        GLOG(g, log_level_error, 0x215, "glue%p, already have shared_buf\n", g);
        return -1;
    }
    name = m->impl->name;
    if (!strcmp(name, "gluepw"))
        return set_shbuf_pw(g, p);
    if (!strcmp(name, "gluepr"))
        return set_shbuf_pr(g, p);

    /* "glue": the role follows from the ports/legacy buffers already present */
    if (!in && (out || g->output_buffer)) {
        if (set_shbuf_pr(g, p) < 0) {
            GLOG(g, log_level_error, 0x222, "glue%p, cannot set inter reader\n", g);
            goto fail;
        }
        if (aud_module_get_output_port(m, 0))
            return 0;
        out = aud_module_new_output_port(m, 0);
        if (out) {
            pthread_mutex_lock(&out->check_free_mutex);
            out->legacy_buf = g->output_buffer;
            pthread_mutex_unlock(&out->check_free_mutex);
            return 0;
        }
        GLOG(g, log_level_error, 0x22e, "glue%p, no port\n", g);
        aud_starter_del(g->starter);    /* g->starter is left dangling, as in the original */
        goto fail;
    }
    if ((!in && g->input_buffer) || (in && !out)) {
        if (set_shbuf_pw(g, p) < 0) {
            GLOG(g, log_level_error, 0x23c, "glue%p, cannot set inter writer\n", g);
            goto fail;
        }
        if (!g->input_buffer)
            return 0;
        out = aud_module_get_output_port(m, 0);
        if (!out) {
            GLOG(g, log_level_error, 0x246, "Oops?\n");
            goto fail;
        }
        if (!g->starter) {
            g->starter = aud_starter_new(g->input_buffer, "starter_%08x", m->uid);
            if (!g->starter) {
                GLOG(g, log_level_error, 0x252, "glue%p, can not create starter\n", g);
                aud_module_del_output_port(m, 0);
                goto fail;
            }
        }
        aud_starter_add_port(g->starter, out);
        return 0;
    }
    GLOG(g, log_level_error, 0x25d, "glue%p, unknown status..\n", g);
fail:
    if (!g->shared_buf)
        return -1;
    Glue_DestroyBuffer(g, g->shared_buf);
    g->shared_buf = NULL;
    return -1;
}

static int SetEnable(struct mod_glue *g)
{
    struct aud_module *m = &g->module;
    struct aud_port *out, *in;

    GLOG(g, log_level_debug, 0x27a, "glue%p, enable\n", g);
    out = aud_module_get_output_port(m, 0);
    in = aud_module_get_input_port(m, 0);
    if (g->input_buffer && !in && out) {
        if (!g->starter)
            return 0;
        aud_starter_add_port(g->starter, out);
        GLOG(g, log_level_debug, 0x285, "glue%p, new input starter %p\n", g, g->starter);
        return 0;
    }
    if (g->output_buffer && !out && in) {
        out = aud_module_new_output_port(m, 0);
        if (!out) {
            GLOG(g, log_level_error, 0x28f, "glue%p, no port\n", g);
            return -1;
        }
        pthread_mutex_lock(&out->check_free_mutex);
        out->legacy_buf = g->output_buffer;
        pthread_mutex_unlock(&out->check_free_mutex);
        GLOG(g, log_level_debug, 0x295, "glue%p, new output legacy_buf %p\n", g,
             g->output_buffer);
        return 0;
    }
    GLOG(g, log_level_error, 0x29a, "glue%p, can not create starter\n", g);
    return -1;
}

/* the coworker moved its side of the ring: reader progress frees space for the
 * writer's blocked aud_port_sync, writer progress wakes the reader's starter */
static int SetSharedBufferStatus(struct mod_glue *g, BmCmdProcessEvtRemote *p)
{
    switch (p->evt_type) {
    case BM_EVT_READ:
        GLOG(g, log_level_debug, 0x2ac, "glue%p, write status %d(%d)\n", g,
             g->shared_buf->status.writeOffset, g->shared_buf->status.writeIndex);
        GLOG(g, log_level_debug, 0x2af, "glue%p, update read status %d(%d) -> %d(%d)\n", g,
             g->shared_buf->status.readOffset, g->shared_buf->status.readIndex,
             p->status.readOffset, p->status.readIndex);
        InterBufWriter_UpdateStatus(g->shared_buf, &p->status);
        if (aud_module_check_legacy_output(&g->module, 0) < 0)
            GLOG(g, log_level_error, 0x2b6, "glue%p, Oops?\n", g);
        return 0;
    case BM_EVT_WRITE:
        GLOG(g, log_level_debug, 0x2ba, "glue%p, update write status %d, %d\n", g,
             p->status.writeOffset, p->status.writeIndex);
        InterBufReader_UpdateStatus(g->shared_buf, &p->status);
        if (g->starter)
            aud_starter_trigger(g->starter);
        else
            GLOG(g, log_level_error, 0x2c1, "glue%p, no starter??\n", g);
        return 0;
    default:
        GLOG(g, log_level_error, 0x2c5, "glue%p, unknown event type\n", g);
        return 0;
    }
}

/* @0x2a8ec */
int Glue_Cmd(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    struct mod_glue *g = arg;

    if (cmd == BM_CMD_PROCESS_EVT_REMOTE) {
        GLOG(g, log_level_debug, 0x2d4, "glue%p, received command: %08x\n", g, cmd);
        return SetSharedBufferStatus(g, param);
    }
    GLOG(g, log_level_info, 0x2d2, "glue%p, received command: %08x\n", g, cmd);
    switch (cmd) {
    case GLUE_CMD_SET_SHARED_BUFFER:
        return SetSharedBuffer(g, param);
    case GLUE_CMD_SET_ENABLE:
        return SetEnable(g);
    case ADEC_CMD_START:
    case ADEC_CMD_STOP:
    case ADEC_CMD_FLUSH:
        return 0;
    }
    return aud_module_default_command(arg, cmd, param, len);
}

static struct aud_module_property glue_props[] = {
    { "cowork-uid", param_type_int, offsetof(struct mod_glue, cowork_uid), 0 },
    { "shbuf-offset", param_type_int, offsetof(struct mod_glue, shbuf_offset), 0 },
    { "merge-meta-pcm", param_type_bool, offsetof(struct mod_glue, merge_meta_pcm), 0 },
    { NULL },
};

static struct aud_module_impl glue_impl = {              /* @0x50ef0 */
    .name = "glue",
    .constructor = Glue_Constructor,
    .destructor = Glue_Destructor,
    .chain = Glue_Chain,
    .event = Glue_Event,
    .cmd = Glue_Cmd,
    .deleting_port = Glue_DeletingPort,
    .instance_size = sizeof(struct mod_glue),
    .prop = glue_props,
};

/* @0x181f8: init_array; the same implementation under the legacy/pipe names */
void glue_initializer(void)
{
    ModuleRegistry_Register(&glue_impl);
    ModuleRegistry_RegisterNamef(&glue_impl, NULL, "glue_legacy");
    ModuleRegistry_RegisterNamef(&glue_impl, NULL, "glue_proc");
    ModuleRegistry_RegisterNamef(&glue_impl, NULL, "gluepw");
    ModuleRegistry_RegisterNamef(&glue_impl, NULL, "gluepr");
}
