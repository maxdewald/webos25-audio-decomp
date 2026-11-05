/*
 * libaa module core: aud_module instances, ports, the data/event push path,
 * properties, commands and the per-module output thread.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 * Evidence: decomp/libaa.so.2.0.0.c @0x2af18..0x2f138, logmm module "commod",
 * static names from the logm __func__ strings (decomp/libaa.so.2.0.0.funcstrings.tsv),
 * common property table @0x4b7ec, debug switch table @0x5102c.
 *
 * Data path summary (see aud_module_push_data):
 *   aud_module_push_data() only QUEUES the object on the output port (add_pending_obj_locked).
 *   The queue is flushed to the down peers by aud_port_sync_locked(), which runs
 *     - right after a module's chain() returns (aud_module_chain_locked), unless the module
 *       has an output thread or its impl sets AUD_IMPL_HAVE_OUTPUT_THREAD/AUD_IMPL_SKIP_SYNC,
 *     - on aud_module_sync()/aud_port_sync(),
 *     - immediately when the "sync_at_push" debug switch is on.
 *   Each down peer gets aud_module_chain_locked(), which DROPS the data when the input
 *   port is disconnecting (AUD_PORT_DISABLED) or throwing (flush in progress).
 * debug_module @0x2cf44 lives in debug.c.
 */
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "buffers.h"   /* ADEC_BUF_T: legacy buffer bound to a port */

/* ADEC_CMD_* used by the default command handler (kdriver adec_imc_cmd.h numbering) */
#define ADEC_CMD_START              0x10010000
#define ADEC_CMD_STOP               0x10020000
#define ADEC_CMD_CONNECT_METADATA   0x10150000
#define ADEC_CMD_GET_PROP           0x10170020
#define ADEC_CMD_SET_PROP           0x10180004
#define ADEC_CMD_ASYNC_EVENT        0x10190000
#define ADEC_CMD_SYNC               0x101a0000
#define ADEC_CMD_SET_PROP_SINGLE    0x101c0004
#define ADEC_CMD_SET_BASETIME       0x10200008

#define DIR_STR(port) (AUD_PORT_IS_OUTPUT(port) ? "out" : "in")

LOGMM_MODULE_DEFINE(commod);                                  /* @0x41018 */
LOGMM_MODULE_DEFINE(bin);                                     /* @0x4108c */

/* ------------------------------------------------------------------------- */
/* object classes "module", "bin", "binout"                                  */
/* ------------------------------------------------------------------------- */

/* @0x2af04: class_init of "module": empty dynamic property list, log mask 7 */
int aud_object_class_init_module(void *klass)
{
    struct aud_module_impl *impl = klass;

    LINK_INIT(&impl->prop_list);
    impl->initial_log_mask = 7;
    return 0;
}

int aud_object_class_init_bin(void *klass);
int aud_object_class_init_binout(void *klass);

struct aud_object_class_desc __aud_object_class_module = {   /* @0x40ff4 */
    .parent = &__aud_object_class_base,
    .name = "module",
    .sizeof_class = sizeof(struct aud_module_impl),
    .class_init = aud_object_class_init_module,
};

struct aud_object_class_desc __aud_object_class_binout = {   /* @0x41068 */
    .parent = &__aud_object_class_module,
    .name = "binout",
    .sizeof_class = sizeof(struct aud_module_impl),
    .class_init = aud_object_class_init_binout,
};

struct aud_object_class_desc __aud_object_class_bin = {      /* @0x410a0 */
    .parent = &__aud_object_class_module,
    .name = "bin",
    .sizeof_class = sizeof(struct aud_module_bin_impl),
    .class_init = aud_object_class_init_bin,
};

/* @0x18250 (_INIT_6) */
__attribute__((constructor)) void aud_module_class_register(void)
{
    aud_object_class_register(&__aud_object_class_module);
}

/* @0x18264 (_INIT_7) */
__attribute__((constructor)) void aud_bin_class_register(void)
{
    aud_object_class_register(&__aud_object_class_bin);
}

/* @0x18278 (_INIT_8) */
__attribute__((constructor)) void aud_binout_class_register(void)
{
    aud_object_class_register(&__aud_object_class_binout);
}

/* @0x1828c (_INIT_9): binout is also offered to the module registry as a class */
__attribute__((constructor)) void aud_binout_mregistry_add(void)
{
    aud_mregistry_add_class(&__aud_object_class_binout);
}

/* debug switches, toggled through debug_module @0x2cf44 */
bool aud_module_sync_at_push;
bool aud_module_skip_ext_dump;
bool aud_module_do_check_beep;
int aud_module_silence_noise = 0x10000;

struct audutil_opt aud_module_debug_opts[] = {
    { opt_bool, "sync_at_push",  &aud_module_sync_at_push },
    { opt_bool, "skip_ext_dump", &aud_module_skip_ext_dump },
    { opt_int,  "silence_noise", &aud_module_silence_noise },
    { opt_bool, "do_check_beep", &aud_module_do_check_beep },
    { opt_int, NULL, NULL },
};

int aud_module_set_log_level(struct aud_module *m, const struct aud_module_property *p,
                             const void *data, unsigned int size);
char *aud_module_get_log_level(struct aud_module *m, const struct aud_module_property *p);
int aud_module_set_block_module(struct aud_module *m, const struct aud_module_property *p,
                                const void *data, unsigned int size);
char *aud_module_get_chain_profile(struct aud_module *m, const struct aud_module_property *p);

/* properties every module has, searched after impl->prop (@0x4b7ec) */
struct aud_module_property aud_module_common_props[] = {
    { "thread-priority",  param_type_int,  offsetof(struct aud_module, thread_priority) },
    { "drop-output-data", param_type_bool, offsetof(struct aud_module, drop_output_data) },
    { "drop-output-meta", param_type_bool, offsetof(struct aud_module, drop_output_meta) },
    { "block-module",     param_type_bool, offsetof(struct aud_module, block_module), 0,
      aud_module_set_block_module },
    { "log-mask",         param_type_int,  offsetof(struct aud_module, log_mask) },
    { "log-level",        param_type_none, 0, 0, aud_module_set_log_level,
      aud_module_get_log_level },
    { "do-chain-profile", param_type_bool, offsetof(struct aud_module, do_chain_profile) },
    { "chain-profile",    param_type_none, 0, AUD_PROP_RO, NULL, aud_module_get_chain_profile },
    { NULL },
};

int aud_module_chain_locked(struct aud_port *in, struct aud_data *data);
int aud_module_event_locked(struct aud_module *m, struct aud_port *port, struct aud_event *e);
void aud_module_unblock_locked(struct aud_module *m);

/* ------------------------------------------------------------------------- */
/* output thread body                                                        */
/* ------------------------------------------------------------------------- */

/* @0x2af18: body of the thread started by aud_module_output_thread_create */
int aud_module_output_thread_main(void *arg)
{
    struct aud_module *m = arg;

    pthread_mutex_lock(m->othread_lock);
    while (m->othread_running)
        m->othread_callback(m, m->othread_callback_priv);
    pthread_mutex_unlock(m->othread_lock);
    return 0;
}

/* ------------------------------------------------------------------------- */
/* pending objects of an output port                                         */
/* ------------------------------------------------------------------------- */

/* @0x2af48: queue obj (data or event) on an output port, stream_lock held.
 * Also tracks the current FIRST_AU event: data queued while no first event is known gets
 * one synthesised (the module's last received first event, or a fresh empty one).
 */
int add_pending_obj_locked(struct aud_port *out, struct aud_object *obj)
{
    struct aud_module *m = out->module;
    struct aud_event *first;

    LOGT(commod, 0x3e, "pending object %08x to %s:out%d\n", obj->type, m->name, out->number);

    first = out->current_first;
    if (obj->type == AUD_OBJECT_TYPE_EVENT) {
        struct aud_event *e = (struct aud_event *)obj;

        if (e->type == AUD_EVENT_TYPE_FIRST_AU) {
            if (first)
                aud_object_put(&first->object);
            first = (struct aud_event *)aud_object_get(obj);
            out->current_first = first;
        }
        if (e->type == AUD_EVENT_TYPE_LAST_AU) {
            if (first) {
                aud_object_put(&first->object);
                out->current_first = NULL;
                first = NULL;
            } else {
                LOGW(commod, 0x52, "last without first.. at output%d of %s\n",
                     out->number, m->name);
                first = out->current_first;
            }
        }
    }

    if (!first && obj->type == AUD_OBJECT_TYPE_DATA) {
        if (m->first_event_out) {
            LOGI(commod, 0x59, "data without first at output%d of %s. "
                 "using previous first event\n", out->number, m->name);
            out->current_first =
                (struct aud_event *)aud_object_get(&m->first_event_out->object);
        } else {
            LOGI(commod, 0x5f, "data without first at output%d of %s. "
                 "make dummy first event\n", out->number, m->name);
            out->current_first = aud_event_new_first();
        }
    }

    if (out->pending_obj_num == out->pending_obj_alloced) {
        out->pending_obj_alloced = out->pending_obj_num + 1;
        LOGT(commod, 0x68, "increase memory %d\n", out->pending_obj_alloced);
        out->pending_objs = realloc(out->pending_objs,
                                    out->pending_obj_alloced * sizeof(*out->pending_objs));
    }
    if (!out->pending_objs) {
        LOGE(commod, 0x6f, "no mem\n");
        return -1;
    }
    out->pending_objs[out->pending_obj_num++] = obj;
    return 0;
}

/* inlined wrapper (own __func__ "add_pending_obj") */
static inline int add_pending_obj(struct aud_port *out, struct aud_object *obj)
{
    int ret;

    pthread_mutex_lock(&out->stream_lock);
    LOGT(commod, 0x7e, "pending object %08x to %s:out%d\n", obj->type, out->module->name,
         out->number);
    ret = add_pending_obj_locked(out, obj);
    pthread_mutex_unlock(&out->stream_lock);
    return ret;
}

/* @0x2cebc */
int aud_port_push_data(struct aud_port *port, struct aud_data *data)
{
    if (!port) {
        LOGD(commod, 0x8a, "no port.\n");
        aud_object_put(&data->object);
        return -1;
    }
    if (!data) {
        LOGD(commod, 0x91, "no data\n");
        return -1;
    }
    return add_pending_obj(port, &data->object);
}

/* ------------------------------------------------------------------------- */
/* delivering to down peers                                                  */
/* ------------------------------------------------------------------------- */

/* @0x2c8c4: aud_link_foreach_node callback over out->down_peer_root.
 * Entered with in->down_peer_node.lock held.
 */
int foreach_down_peer_push_event(void *arg, void *node_data)
{
    struct aud_event *e = arg;
    struct aud_port *in = node_data;
    struct aud_port *up = in->up_peer;
    struct aud_event *ev;

    if (e->prop & AUD_EVENT_PROP_SYNC)
        pthread_mutex_lock(&in->stream_lock);
    else if (e->prop & AUD_EVENT_PROP_DOWNWARD)
        pthread_mutex_lock(&in->async_down_lock);
    pthread_mutex_unlock(&in->down_peer_node.lock);

    LOGD(commod, 0x10b, "%s.%s%d : write event to %s.%s%d\n", up->module->name, DIR_STR(up),
         up->number, in->module->name, DIR_STR(in), in->number);

    ev = (struct aud_event *)aud_object_get(&e->object);
    aud_module_event_locked(in->module, in, ev);

    if (ev->prop & AUD_EVENT_PROP_SYNC)
        pthread_mutex_unlock(&in->stream_lock);
    else if (ev->prop & AUD_EVENT_PROP_DOWNWARD)
        pthread_mutex_unlock(&in->async_down_lock);
    aud_object_put(&ev->object);
    return 0;
}

/* @0x2ccb4: aud_link_foreach_node callback over out->down_peer_root: chain one data object
 * into a down peer. Optional "do_check_beep" signal detector on the first 64 PCM words.
 */
int foreach_down_peer_push_data(void *arg, void *node_data)
{
    struct aud_data *data = arg;
    struct aud_port *in = node_data;
    struct aud_port *up = in->up_peer;

    pthread_mutex_lock(&in->stream_lock);
    pthread_mutex_unlock(&in->down_peer_node.lock);

    LOGD(commod, 0x176, "%s.%s%d : write to %d bytes to %s.%s%d\n", up->module->name,
         DIR_STR(up), up->number, data->size, in->module->name, DIR_STR(in), in->number);

    if (aud_module_do_check_beep && data->info.type == type_pcm) {
        unsigned int off;
        bool signal = false;

        for (off = 0; off < data->size && off < 0x100; off += 4) {
            int s = *(int *)(data->data + off);

            if (s < -aud_module_silence_noise || s > aud_module_silence_noise) {
                signal = true;
                break;
            }
        }
        if (signal) {
            if (!(up->flag & AUD_PORT_HAVE_BEEP)) {
                logm_message("have signal. %20s.%d -> %20s.%d\n", up->module->name,
                             up->number, in->module->name, in->number);
                up->flag |= AUD_PORT_HAVE_BEEP;
            }
        } else if (up->flag & AUD_PORT_HAVE_BEEP) {
            logm_message("  no signal. %20s.%d -> %20s.%d\n", up->module->name, up->number,
                         in->module->name, in->number);
            up->flag &= ~AUD_PORT_HAVE_BEEP;
        }
    }

    aud_module_chain_locked(in, (struct aud_data *)aud_object_get(&data->object));

    if (in->flag & AUD_PORT_DISABLED) {
        pthread_mutex_unlock(&in->stream_lock);
        sched_yield();
        return 0;
    }
    pthread_mutex_unlock(&in->stream_lock);
    return 0;
}

static AUDIO_DATA_TYPE_T legacy_data_type(struct aud_port *out, struct aud_data *d,
                                          ADEC_AU_INFO_T *info)
{
    switch (d->info.type) {
    case type_pcm:
        if (out->current_first)
            info->acmod = aud_event_value_get_int(out->current_first, "acmod", 0);
        return AUDIO_DATA_TYPE_PCM;
    case type_es:
        return AUDIO_DATA_TYPE_ES;
    case type_meta:
        return AUDIO_DATA_TYPE_META;
    case type_meta_ext:
        return AUDIO_DATA_TYPE_META_EXT;
    case type_mixed:
        return AUDIO_DATA_TYPE_MIXED;
    default:
        LOGE(commod, 0x2c8, "unknown data type (%d). set default PCM type\n", d->info.type);
        return AUDIO_DATA_TYPE_PCM;
    }
}

/* @0x2b22c: flush the pending objects of an output port, stream_lock held.
 * 1) every pending object goes to every down peer (data -> chain, event -> event handler),
 * 2) if the port is bound to a legacy ADEC buffer, the same objects are written there,
 *    blocking on check_free_cond until enough space is free (or a flush starts),
 * 3) the pending references are dropped.
 */
int aud_port_sync_locked(struct aud_port *out)
{
    unsigned int i;

    LOGT(commod, 0x289, "sync port%d of %s\n", out->number, out->module->name);

    for (i = 0; i < out->pending_obj_num; i++) {
        struct aud_object *obj = out->pending_objs[i];

        LOGT(commod, 0x290, "obj %p, type %08x\n", obj, obj->type);
        if (obj->type == AUD_OBJECT_TYPE_DATA) {
            out->last_clock = aud_system_cycle();
            out->passed += ((struct aud_data *)obj)->size;
            aud_link_foreach_node(&out->down_peer_root, obj, foreach_down_peer_push_data);
        } else if (obj->type == AUD_OBJECT_TYPE_EVENT) {
            aud_link_foreach_node(&out->down_peer_root, obj, foreach_down_peer_push_event);
        }
    }

    pthread_mutex_lock(&out->check_free_mutex);
    if (out->legacy_buf && !out->flushing) {
        ADEC_BUF_T *lb = out->legacy_buf;

        for (i = 0; i < out->pending_obj_num; i++) {
            struct aud_object *obj = out->pending_objs[i];
            ADEC_AU_INFO_T info;
            unsigned int size = 0;
            void *buf;

            memset(&info, 0, sizeof(info));
            LOGT(commod, 0x2ac, "write to legacy. obj %p, type %08x\n", obj, obj->type);

            if (obj->type == AUD_OBJECT_TYPE_DATA) {
                struct aud_data *d = (struct aud_data *)obj;

                info.dataType = legacy_data_type(out, d, &info);
                size = d->size;
                info.index = d->info.index;
                info.timestamp = d->info.timestamp;
                info.timestamp64 = d->info.timestamp64;
                buf = d->data;
            } else if (obj->type == AUD_OBJECT_TYPE_EVENT) {
                info.dataType = AUDIO_DATA_TYPE_EVENT;
                buf = aud_event_serialize((struct aud_event *)obj, &size);
            } else {
                buf = NULL;
            }

            if (size == 0) {
                LOGE(commod, 0x2dd, "no data, type %08x, size %d from %s\n", obj->type, 0,
                     out->module->name);
                break;
            }

            if (!out->flushing) {
                unsigned int free_size = lb->get_free_size(lb);

                while (free_size < size) {
                    LOGD(commod, 0x2ea, "%s.%s%d : wait.. %d bytes. free %d\n",
                         out->module->name, DIR_STR(out), out->number, size, free_size);
                    pthread_cond_wait(&out->check_free_cond, &out->check_free_mutex);
                    if (out->flushing)
                        goto flushing;
                    free_size = lb->get_free_size(lb);
                }
                if (out->flushing)
                    goto flushing;
                LOGD(commod, 0x2f9, "%s.%s%d : write to legacy %d bytes\n", out->module->name,
                     DIR_STR(out), out->number, size);
                lb->insert_au(buf, size, &info, lb);
            } else {
flushing:
                LOGI(commod, 0x2ff, "%s.%s%d : flushing...\n", out->module->name,
                     DIR_STR(out), out->number);
            }
            if (obj->type == AUD_OBJECT_TYPE_EVENT)
                free(buf);
            if (out->flushing)
                break;
        }
    }
    pthread_mutex_unlock(&out->check_free_mutex);

    for (i = 0; i < out->pending_obj_num; i++)
        aud_object_put(out->pending_objs[i]);
    out->pending_obj_num = 0;
    LOGT(commod, 0x30e, "sync done.\n");
    return 0;
}

/* @0x2b6d4: aud_link_foreach_node callback over module->output_port_root */
int foreach_sync(void *arg, void *node_data)
{
    struct aud_port *out = node_data;

    (void)arg;
    pthread_mutex_lock(&out->stream_lock);
    LOGT(commod, 0x573, "sync %s.%d\n", out->module->impl->name, out->number);
    pthread_mutex_unlock(&out->module_node.lock);
    aud_port_sync_locked(out);
    pthread_mutex_unlock(&out->stream_lock);
    return 0;
}

/* @0x2cfb4 */
int aud_port_sync(struct aud_port *port)
{
    int ret;

    pthread_mutex_lock(&port->stream_lock);
    LOGT(commod, 0x319, "sync port%d of %s\n", port->number, port->module->name);
    ret = aud_port_sync_locked(port);
    pthread_mutex_unlock(&port->stream_lock);
    return ret;
}

/* @0x2d01c: wake a writer blocked on a full legacy buffer */
int aud_port_check_legacy(struct aud_port *port)
{
    pthread_mutex_lock(&port->check_free_mutex);
    pthread_cond_broadcast(&port->check_free_cond);
    pthread_mutex_unlock(&port->check_free_mutex);
    return 0;
}

/* ------------------------------------------------------------------------- */
/* properties                                                                */
/* ------------------------------------------------------------------------- */

/* @0x2b744: "chain-profile" getter: CPU time spent in chain() since the last read */
char *aud_module_get_chain_profile(struct aud_module *m, const struct aud_module_property *p)
{
    unsigned long long now = aud_system_cycle();
    char *s;

    (void)p;
    if (m->chain_profile_last == 0)
        s = audutil_strdup_printf("%llu", m->chain_profile);
    else
        s = audutil_strdup_printf("%llu/%llu", m->chain_profile, now - m->chain_profile_last);
    m->chain_profile_last = now;
    m->chain_profile = 0;
    return s;
}

/* @0x2b798: "log-level" getter */
char *aud_module_get_log_level(struct aud_module *m, const struct aud_module_property *p)
{
    (void)p;
    return audutil_strdup_printf("0x%08x", m->log_mask);
}

/* @0x2b908: "log-level" setter (__func__ "set_log_level"): level n -> mask (1<<n)-1 */
int aud_module_set_log_level(struct aud_module *m, const struct aud_module_property *p,
                             const void *data, unsigned int size)
{
    int level = strtoul(data, NULL, 0);

    (void)p;
    (void)size;
    m->log_mask = (1 << level) - 1;
    LOGN(commod, 0x77d, "%s: set log level %d(mask 0x%x)\n", m->name, level, m->log_mask);
    return 0;
}

static bool parse_bool(const char *v)
{
    if (!strcmp(v, "true") || !strcmp(v, "yes"))
        return true;
    return v[0] == '1' && v[1] == '\0';
}

/* @0x2b970: "block-module" setter (__func__ "set_block_module") */
int aud_module_set_block_module(struct aud_module *m, const struct aud_module_property *p,
                                const void *data, unsigned int size)
{
    bool block = parse_bool(data);
    bool old;

    (void)p;
    (void)size;
    pthread_mutex_lock(&m->block_mutex);
    old = m->block_module;
    if (old == block) {
        LOGI(commod, 0x78d, "setting same value. ignore. %s: %s\n", m->name,
             block ? "true" : "false");
    } else {
        m->block_module = block;
        LOGI(commod, 0x794, "%s: set block as %s\n", m->name, block ? "true" : "false");
        if (old && !block) {
            LOGI(commod, 0x798, "%s : unblock!\n", m->name);
            LOGI(commod, 0x9a, "trying to unblock module %s", m->name);
            m->block_module = false;
            pthread_cond_broadcast(&m->block_cond);
        }
    }
    pthread_mutex_unlock(&m->block_mutex);
    return 0;
}

/* @0x2baf8: look up name in a "{a:1, b:2}" style string; returns malloc'd value */
char *aud_prop_get_value(const char *str, const char *name)
{
    size_t len = strlen(str);
    char *copy = malloc(len + 1);
    char *save, *key, *val, *ret = NULL;

    if (!copy) {
        LOGE(commod, 0x801, "nomem.\n");
        return NULL;
    }
    strncpy(copy, str, len + 1);
    copy[len] = '\0';

    for (key = strtok_r(copy, ":{}, ", &save); key; key = strtok_r(NULL, ":{}, ", &save)) {
        val = strtok_r(NULL, ":{}, ", &save);
        LOGD(commod, 0x80d, "  name = %s, value = %s, prop_name = %s\n", key, val, name);
        if (!strcmp(key, name)) {
            LOGD(commod, 0x811, "    %s found.. value = %s\n", key, val);
            ret = strdup(val);
            break;
        }
    }
    free(copy);
    return ret;
}

/* @0x2e388 */
int aud_prop_get_value_int(const char *str, const char *name, int def)
{
    char *v = aud_prop_get_value(str, name);

    if (v) {
        def = strtoul(v, NULL, 0);
        free(v);
    }
    return def;
}

/* @0x2e3a8 */
bool aud_prop_get_value_bool(const char *str, const char *name, bool def)
{
    char *v = aud_prop_get_value(str, name);

    if (v) {
        def = parse_bool(v);
        free(v);
    }
    return def;
}

/* @0x2e3f0 */
char *aud_prop_get_value_str(const char *str, const char *name, const char *def)
{
    char *v = aud_prop_get_value(str, name);

    if (!v)
        return strdup(def);
    return v;
}

/* @0x2bc3c: set one property (no symbol-table entry; the body Ghidra inlined twice) */
int aud_module_set_prop_locked(struct aud_module *m, const struct aud_module_property *p,
                               const char *value, unsigned int size)
{
    char *field = (char *)m + p->offset;
    int ret;

    pthread_mutex_lock(&m->prop_access_lock);
    if (p->set) {
        LOGI(commod, 0x6f3, "%s: set %s..\n", m->name, p->name);
        ret = p->set(m, p, value, size);
        pthread_mutex_unlock(&m->prop_access_lock);
        return ret;
    }

    switch (p->type) {
    case param_type_longlong: {
        unsigned long long v = strtoull(value, NULL, 0);

        if (v == *(unsigned long long *)field) {
            LOGI(commod, 0x700, "%s: setting same value. ignore. %s: %lld\n", m->name, p->name,
                 v);
            goto out;
        }
        *(unsigned long long *)field = v;
        LOGI(commod, 0x705, "%s: set %s=%lld\n", m->name, p->name, v);
        break;
    }
    case param_type_bool: {
        bool v = parse_bool(value);

        if (*(bool *)field == v) {
            LOGI(commod, 0x722, "%s: setting same value. ignore. %s: %s\n", m->name, p->name,
                 v ? "true" : "false");
            goto out;
        }
        *(bool *)field = v;
        LOGI(commod, 0x728, "%s: set %s=%s\n", m->name, p->name, v ? "true" : "false");
        break;
    }
    case param_type_int: {
        unsigned long v = strtoul(value, NULL, 0);

        if (*(unsigned long *)field == v) {
            LOGI(commod, 0x70f, "%s: setting same value. ignore. %s: %d\n", m->name, p->name,
                 (int)v);
            goto out;
        }
        *(unsigned long *)field = v;
        LOGI(commod, 0x714, "%s: set %s=%d\n", m->name, p->name,
             (int)strtol(value, NULL, 10));
        break;
    }
    case param_type_str:
        free(*(char **)field);
        *(char **)field = strdup(value);
        LOGI(commod, 0x731, "%s: set %s=%s\n", m->name, p->name, *(char **)field);
        break;
    case param_type_data:
        free(*(void **)field);
        *(void **)field = malloc(size);
        if (*(void **)field)
            memcpy(*(void **)field, value, size);
        LOGI(commod, 0x73b, "%s: set %s=<data %p, size %d>\n", m->name, p->name,
             *(void **)field, size);
        break;
    case param_type_struct:
        LOGE(commod, 0x73f, "%s: %s, please use set function to struct type parameter\n",
             m->name, p->name);
        goto out;
    default:
        LOGE(commod, 0x745, "%s: unknown property type. %d\n", m->name, p->type);
        goto out;
    }
    if (m->impl->prop_updated)
        m->impl->prop_updated(m, p);
out:
    pthread_mutex_unlock(&m->prop_access_lock);
    return 0;
}

static inline bool prop_writable(const struct aud_module_property *p)
{
    return (p->offset || p->set) && !(p->flag & AUD_PROP_RO);
}

/* @0x2c038: set a property added at runtime (impl->prop_list) */
int aud_module_set_prop_list(struct aud_module *m, const char *name, const char *value,
                             unsigned int size)
{
    struct aud_module_impl *impl = m->impl;
    struct link *l;

    if (!impl->prop_list.next)
        return -1;
    for_each_link(l, &impl->prop_list) {
        struct aud_module_property *p = link_container(struct aud_module_property, list, l);

        if (strcmp(p->name, name))
            continue;
        if (!prop_writable(p))
            return -1;
        return aud_module_set_prop_locked(m, p, value, size);
    }
    return -1;
}

/* @0x2c34c: set a property from a NULL-name terminated table */
int aud_module_set_prop_table(struct aud_module *m, struct aud_module_property *table,
                              const char *name, const char *value, unsigned int size)
{
    struct aud_module_property *p;

    for (p = table; p->name; p++)
        if (!strcmp(p->name, name) && prop_writable(p))
            return aud_module_set_prop_locked(m, p, value, size);
    return -1;
}

/* @0x2c394: free a str/data property value owned by the instance */
void aud_module_free_prop_value(struct aud_module *m, const struct aud_module_property *p)
{
    void **field;

    if (p->type != param_type_str && p->type != param_type_data)
        return;
    field = (void **)((char *)m + p->offset);
    if (*field) {
        free(*field);
        *field = NULL;
    }
}

/* @0x2e408: "name=value\name=value..." */
int aud_module_set_prop(struct aud_module *m, char *str, int len)
{
    size_t n = strnlen(str, len);
    char *copy = malloc(n + 1);
    char *save, *name, *value;

    if (!copy) {
        LOGE(commod, 0x85e, "nomem.\n");
        return 0;
    }
    strncpy(copy, str, n + 1);
    copy[n] = '\0';

    for (name = strtok_r(copy, "=\\", &save); name; name = strtok_r(NULL, "=\\", &save)) {
        value = strtok_r(NULL, "=\\", &save);
        if (!value) {
            LOGE(commod, 0x86d, "no data for data. str %s\n", copy);
            break;
        }
        if (m->impl->prop && aud_module_set_prop_table(m, m->impl->prop, name, value, 0) >= 0)
            continue;
        if (aud_module_set_prop_table(m, aud_module_common_props, name, value, 0) >= 0)
            continue;
        aud_module_set_prop_list(m, name, value, 0);
    }
    free(copy);
    return 0;
}

/* @0x2e554 */
long long aud_module_get_prop(struct aud_module *m, const char *name)
{
    struct aud_module_impl *impl = m->impl;
    struct aud_module_property *p = NULL, *t;
    const char *field;
    long long v;

    for (t = impl->prop; t && t->name; t++)
        if (!strcmp(t->name, name)) {
            p = t;
            goto found;
        }
    for (t = aud_module_common_props; t->name; t++)
        if (!strcmp(t->name, name)) {
            p = t;
            goto found;
        }
    if (impl->prop_list.next) {
        struct link *l;

        for_each_link(l, &impl->prop_list) {
            t = link_container(struct aud_module_property, list, l);
            if (!strcmp(t->name, name)) {
                p = t;
                goto found;
            }
        }
        LOGE(commod, 0x8ab, "unknown prop. %s\n", name);
    }
    return -1;

found:
    if (p->geti) {
        v = p->geti(m, p);
        LOGI(commod, 0x8b4, "%s: %lld\n", p->name, v);
        return v;
    }
    if (p->gets) {
        LOGE(commod, 0x8b8, "gets() not supported. %s at %s\n", name, m->name);
        return -1;
    }
    field = (const char *)m + p->offset;
    switch (p->type) {
    case param_type_longlong:
        v = *(const long long *)field;
        LOGI(commod, 0x8c3, "%s(offset:%d): %lld\n", p->name, p->offset, v);
        return v;
    case param_type_int:
        v = *(const int *)field;
        LOGI(commod, 0x8c8, "%s(offset:%d): %lld\n", p->name, p->offset, v);
        return v;
    case param_type_bool:
        v = *(const unsigned char *)field;
        LOGI(commod, 0x8cd, "%s(offset:%d): %lld\n", p->name, p->offset, v);
        return v;
    case param_type_str:
        LOGI(commod, 0x8d1, "%s(offset:%d): %s\n", p->name, p->offset, *(char *const *)field);
        return 0;
    default:
        return 0;
    }
}

/* @0x2c090: append one "fmt(name, value)" line to s */
char *aud_module_prop_append(struct aud_module *m, const struct aud_module_property *p,
                             const char *fmt, char *s, unsigned int *len)
{
    const char *field = (const char *)m + p->offset;
    char *v;

    if (p->gets) {
        v = p->gets(m, p);
    } else {
        switch (p->type) {
        case param_type_longlong:
            v = audutil_strdup_printf("(long long)%lld", *(const long long *)field);
            break;
        case param_type_bool:
            v = audutil_strdup_printf("(bool)%s", *field ? "true" : "false");
            break;
        case param_type_int:
            v = audutil_strdup_printf("(int)%d", *(const int *)field);
            break;
        case param_type_str:
            v = audutil_strdup_printf("(str)%s",
                                      *(char *const *)field ? *(char *const *)field : "NULL");
            break;
        case param_type_struct:
            return s;
        default:
            v = NULL;
            break;
        }
    }
    if (!v)
        return audutil_append_printf(s, len, fmt, p->name, "<unknown_type>");
    s = audutil_append_printf(s, len, fmt, p->name, v);
    free(v);
    return s;
}

/* @0x2e7e4: all properties of an instance formatted with fmt ("%s=%s\n" style) */
char *aud_module_prop_str(struct aud_module *m, const char *fmt)
{
    struct aud_module_impl *impl;
    struct aud_module_property *p;
    char *s = NULL;
    unsigned int len = 0;

    for (p = aud_module_common_props; p->name; p++)
        s = aud_module_prop_append(m, p, fmt, s, &len);
    impl = m->impl;
    for (p = impl->prop; p && p->name; p++)
        s = aud_module_prop_append(m, p, fmt, s, &len);
    if (impl->prop_list.next) {
        struct link *l;

        for_each_link(l, &impl->prop_list)
            s = aud_module_prop_append(m, link_container(struct aud_module_property, list, l),
                                       fmt, s, &len);
    }
    return s;
}

/* @0x2e868: register a property at runtime */
struct aud_module_property *aud_module_impl_add_prop2(struct aud_module_impl *impl,
    const char *name, enum aud_module_prop_type type, int offset, aud_prop_set_fn set,
    aud_prop_gets_fn gets, aud_prop_geti_fn geti, unsigned int flag, void *priv)
{
    struct aud_module_property *p = calloc(1, sizeof(*p));

    if (!p)
        return NULL;
    p->name = name;
    p->set = set;
    p->type = type;
    p->offset = offset;
    p->gets = gets;
    p->geti = geti;
    p->flag = flag;
    p->priv = priv;
    if (!impl->prop_list.next)
        LINK_INIT(&impl->prop_list);
    link_add_tail(&impl->prop_list, &p->list);
    return p;
}

/* @0x2e8bc */
int aud_module_impl_add_prop(struct aud_module_impl *impl, const char *name,
                             enum aud_module_prop_type type, int offset, aud_prop_set_fn set,
                             aud_prop_gets_fn gets, unsigned int flag)
{
    return aud_module_impl_add_prop2(impl, name, type, offset, set, gets, NULL, flag, NULL)
        ? 0 : -1;
}

/* ------------------------------------------------------------------------- */
/* instances                                                                 */
/* ------------------------------------------------------------------------- */

/* @0x2d03c: allocate instance_size bytes, name "<impl>_<uid>", call impl->constructor */
struct aud_module *aud_module_constructor(struct aimc *imc, struct aud_module_impl *impl,
                                          unsigned int uid, const char *param)
{
    struct aud_module *m = calloc(impl->instance_size, 1);

    if (!m) {
        LOGE(commod, 0x332, "Oops\n");
        return NULL;
    }
    m->name = audutil_strdup_printf("%s_%08x", impl->name, uid);
    m->uid = uid;
    m->aimc = imc;
    m->log_mask = impl->initial_log_mask;
    m->impl = impl;
    pthread_mutex_init(&m->prop_access_lock, NULL);
    pthread_cond_init(&m->block_cond, NULL);
    pthread_mutex_init(&m->block_mutex, NULL);
    aud_link_root_init(&m->output_port_root);
    aud_link_root_init(&m->input_port_root);
    link_add(&m->impl->instances, &m->instance_link);

    /* the constructor's return value is ignored */
    if (impl->constructor)
        impl->constructor(m, imc, param);
    LOGI(commod, 0x344, "new module created. %s\n", m->name);
    return m;
}

/* @0x2b1b8: free an input port that was already unlinked (__func__ "del_input_port") */
void del_input_port(struct aud_port *in)
{
    LOGI(commod, 0x4a3, "delete input port%d, %p, on %p(%s)\n", in->number, in, in->module,
         in->module->name);
    pthread_mutex_lock(&in->stream_lock);
    pthread_mutex_lock(&in->async_down_lock);
    if (in->current_first)
        aud_object_put(&in->current_first->object);
    free(in);
}

/* @0x2b7a8: free an output port that was already unlinked */
void del_output_port(struct aud_port *out)
{
    unsigned int i;

    LOGI(commod, 0x4e3, "delete output port%d, %p, on %p(%s)\n", out->number, out,
         out->module, out->module->impl->name);
    if (out->starter)
        aud_starter_del_port(out->starter, out);

    /* wait for every user of the port to leave */
    pthread_mutex_lock(&out->module_node.lock);
    pthread_mutex_lock(&out->stream_lock);
    pthread_mutex_lock(&out->async_down_lock);
    pthread_mutex_lock(&out->async_up_lock);
    pthread_mutex_unlock(&out->async_up_lock);
    pthread_mutex_unlock(&out->async_down_lock);
    pthread_mutex_unlock(&out->stream_lock);

    if (out->pending_objs) {
        LOGI(commod, 0x4f7, "free %d pending objs\n", out->pending_obj_num);
        for (i = 0; i < out->pending_obj_num; i++) {
            LOGI(commod, 0x4fa, "%d type %08x, user %d\n", i, out->pending_objs[i]->type,
                 out->pending_objs[i]->user);
            aud_object_put(out->pending_objs[i]);
        }
        free(out->pending_objs);
    }
    if (out->current_first)
        aud_object_put(&out->current_first->object);
    free(out);
}

/* @0x2e368: destructor helper, input_port_root walk */
int disconnect_input_port_cb(void *arg, void *node_data)
{
    struct aud_port *in = node_data;
    struct aud_port *up = in->up_peer;

    (void)arg;
    pthread_mutex_unlock(&in->module_node.lock);
    if (up)
        aud_module_disconnect(up, in);
    return 0;
}

/* @0x2e348: destructor helper, out->down_peer_root walk */
int disconnect_down_peer_cb(void *arg, void *node_data)
{
    struct aud_port *in = node_data;
    struct aud_port *up = in->up_peer;

    (void)arg;
    pthread_mutex_unlock(&in->down_peer_node.lock);
    if (up)
        aud_module_disconnect(up, in);
    return 0;
}

/* @0x2b194: destructor helper, output_port_root walk */
int disconnect_output_port_cb(void *arg, void *node_data)
{
    struct aud_port *out = node_data;

    (void)arg;
    pthread_mutex_unlock(&out->module_node.lock);
    aud_link_foreach_node(&out->down_peer_root, NULL, disconnect_down_peer_cb);
    return 0;
}

/* inlined into the destructor (own __func__) */
static inline void aud_module_del_output_ports(struct aud_module *m)
{
    struct link *l, *next;

    LOGI(commod, 0x34d, "%s output ports will be deleted", m->name);
    pthread_mutex_lock(&m->output_port_root.rlock);
    for (l = m->output_port_root.nodes.next, next = l->next; l != &m->output_port_root.nodes;
         l = next, next = l->next) {
        struct aud_port *out = ((struct aud_link_node *)l)->data;

        if (!link_empty(&out->down_peer_root.nodes))
            LOGE(commod, 0x352, "output port%d of %s still has down peer\n", out->number,
                 m->name);
        aud_link_del_locked(&out->module_node);
        del_output_port(out);
    }
    pthread_mutex_unlock(&m->output_port_root.rlock);
    LOGI(commod, 0x359, "%s output port(s) deleted", m->name);
}

/* @0x2d14c */
void aud_module_destructor(struct aud_module *m)
{
    struct aud_module_impl *impl;
    struct aud_module_property *p;

    LOGI(commod, 0x39b, "destroy %s\n", m->name);
    aimc_clear_named_event_on_uid(m->aimc, m->uid);
    aud_link_foreach_node(&m->input_port_root, NULL, disconnect_input_port_cb);
    aud_link_foreach_node(&m->output_port_root, NULL, disconnect_output_port_cb);
    if (m->impl->destructor)
        m->impl->destructor(m);
    if (!link_empty(&m->input_port_root.nodes))
        LOGE(commod, 0x3a9, "input port(s) still available on %s\n", m->name);
    aud_module_del_output_ports(m);

    impl = m->impl;
    for (p = impl->prop; p && p->name; p++)
        aud_module_free_prop_value(m, p);
    impl = m->impl;
    if (impl->prop_list.next) {
        struct link *l;

        for_each_link(l, &m->impl->prop_list)
            aud_module_free_prop_value(m, link_container(struct aud_module_property, list, l));
    }
    link_del(&m->instance_link);
    free(m->name);
    if (m->first_event_out)
        aud_object_put(&m->first_event_out->object);
    free(m);
}

/* ------------------------------------------------------------------------- */
/* ports                                                                     */
/* ------------------------------------------------------------------------- */

static struct aud_port *find_port_locked(struct aud_link_root *root, unsigned int number)
{
    struct link *l;

    for_each_link(l, &root->nodes) {
        struct aud_link_node *n = (struct aud_link_node *)l;
        struct aud_port *port = n->data;

        if (port->number == number) {
            pthread_mutex_lock(&n->lock);
            return port;
        }
    }
    return NULL;
}

/* @0x2d34c: returns the port with module_node.lock held */
struct aud_port *aud_module_get_input(struct aud_module *m, unsigned int number)
{
    struct aud_port *port;

    pthread_mutex_lock(&m->input_port_root.rlock);
    port = find_port_locked(&m->input_port_root, number);
    pthread_mutex_unlock(&m->input_port_root.rlock);
    return port;
}

/* @0x2d394: returns the port with module_node.lock held */
struct aud_port *aud_module_get_output(struct aud_module *m, unsigned int number)
{
    struct aud_port *port;

    pthread_mutex_lock(&m->output_port_root.rlock);
    port = find_port_locked(&m->output_port_root, number);
    pthread_mutex_unlock(&m->output_port_root.rlock);
    return port;
}

/* @0x2d5b0 */
struct aud_port *aud_module_get_input_port(struct aud_module *m, unsigned int number)
{
    struct aud_port *port = aud_module_get_input(m, number);

    if (port)
        aud_module_put_port(port);
    return port;
}

/* @0x2d5c4 */
struct aud_port *aud_module_get_output_port(struct aud_module *m, unsigned int number)
{
    struct aud_port *port = aud_module_get_output(m, number);

    if (port)
        aud_module_put_port(port);
    return port;
}

/* @0x2d5d8 */
int aud_module_check_legacy_output(struct aud_module *m, unsigned int number)
{
    struct aud_port *port = aud_module_get_output(m, number);

    if (!port)
        return -1;
    pthread_mutex_lock(&port->check_free_mutex);
    pthread_cond_broadcast(&port->check_free_cond);
    pthread_mutex_unlock(&port->check_free_mutex);
    aud_module_put_port(port);
    return 0;
}

/* @0x2d60c */
int aud_module_have_input(struct aud_module *m, unsigned int number)
{
    struct aud_port *port = aud_module_get_input(m, number);

    if (!port)
        return 0;
    aud_module_put_port(port);
    return 1;
}

/* @0x2d65c */
int aud_module_have_output(struct aud_module *m, unsigned int number)
{
    struct aud_port *port = aud_module_get_output(m, number);

    if (!port)
        return 0;
    aud_module_put_port(port);
    return 1;
}

/* @0x2d6ac: any output port with at least one down peer */
int aud_module_have_active_output(struct aud_module *m)
{
    struct link *l;
    int ret = 0;

    pthread_mutex_lock(&m->output_port_root.rlock);
    for_each_link(l, &m->output_port_root.nodes) {
        struct aud_port *out = ((struct aud_link_node *)l)->data;

        if (!link_empty(&out->down_peer_root.nodes)) {
            ret = 1;
            break;
        }
    }
    pthread_mutex_unlock(&m->output_port_root.rlock);
    return ret;
}

/* @0x2d6f8 */
int aud_module_have_active_output_n(struct aud_module *m, unsigned int number)
{
    struct link *l;
    int ret = 0;

    pthread_mutex_lock(&m->output_port_root.rlock);
    for_each_link(l, &m->output_port_root.nodes) {
        struct aud_port *out = ((struct aud_link_node *)l)->data;

        if (out->number == number && !link_empty(&out->down_peer_root.nodes)) {
            ret = 1;
            break;
        }
    }
    pthread_mutex_unlock(&m->output_port_root.rlock);
    return ret;
}

/* @0x2d74c: get-or-create, returned with module_node.lock held */
struct aud_port *aud_module_new_input(struct aud_module *m, unsigned int number)
{
    struct aud_port *port;

    pthread_mutex_lock(&m->input_port_root.rlock);
    port = find_port_locked(&m->input_port_root, number);
    if (port) {
        LOGI(commod, 0x43f, "module(%s) have already input port %d\n", m->name, number);
        goto out;
    }
    port = calloc(sizeof(*port), 1);
    if (!port) {
        LOGE(commod, 0x447, "no mem\n");
        goto out;
    }
    port->number = number;
    port->module = m;
    aud_link_node_init(&port->module_node, port);
    aud_link_node_init(&port->down_peer_node, port);
    LINK_INIT(&port->starter_link);
    pthread_mutex_init(&port->stream_lock, NULL);
    pthread_mutex_init(&port->async_down_lock, NULL);
    pthread_mutex_init(&port->async_up_lock, NULL);
    aud_link_add_tail_locked(&m->input_port_root, &port->module_node);
    LOGI(commod, 0x455, "new input port%d, %p, on %p(%s)\n", number, port, m, m->name);
    pthread_mutex_lock(&port->module_node.lock);
out:
    pthread_mutex_unlock(&m->input_port_root.rlock);
    return port;
}

/* @0x2d8b8: get-or-create, returned with module_node.lock held */
struct aud_port *aud_module_new_output(struct aud_module *m, unsigned int number)
{
    struct aud_port *port;

    pthread_mutex_lock(&m->output_port_root.rlock);
    port = find_port_locked(&m->output_port_root, number);
    if (port) {
        LOGI(commod, 0x466, "module(%s) have already output port %d\n", m->name, number);
        goto out;
    }
    port = calloc(sizeof(*port), 1);
    if (!port) {
        LOGE(commod, 0x46e, "no mem\n");
        goto out;
    }
    port->flag = AUD_PORT_OUTPUT;
    port->number = number;
    port->module = m;
    aud_link_node_init(&port->module_node, port);
    aud_link_root_init(&port->down_peer_root);
    LINK_INIT(&port->starter_link);
    pthread_mutex_init(&port->stream_lock, NULL);
    pthread_mutex_init(&port->async_down_lock, NULL);
    pthread_mutex_init(&port->async_up_lock, NULL);
    pthread_mutex_init(&port->check_free_mutex, NULL);
    pthread_cond_init(&port->check_free_cond, NULL);
    aud_link_add_tail_locked(&m->output_port_root, &port->module_node);
    LOGI(commod, 0x47f, "new output port%d, %p, on %p(%s)\n", number, port, m, m->name);
    pthread_mutex_lock(&port->module_node.lock);
out:
    pthread_mutex_unlock(&m->output_port_root.rlock);
    return port;
}

/* @0x2da38 */
struct aud_port *aud_module_new_input_port(struct aud_module *m, unsigned int number)
{
    struct aud_port *port = aud_module_new_input(m, number);

    if (port)
        aud_module_put_port(port);
    return port;
}

/* @0x2da4c */
struct aud_port *aud_module_new_output_port(struct aud_module *m, unsigned int number)
{
    struct aud_port *port = aud_module_new_output(m, number);

    if (port)
        aud_module_put_port(port);
    return port;
}

/* @0x2da60 */
void aud_module_del_input_port(struct aud_module *m, unsigned int number)
{
    struct link *l;

    pthread_mutex_lock(&m->input_port_root.rlock);
    for_each_link(l, &m->input_port_root.nodes) {
        struct aud_port *in = ((struct aud_link_node *)l)->data;

        if (in->number == number) {
            aud_link_del_locked(&in->module_node);
            pthread_mutex_unlock(&m->input_port_root.rlock);
            del_input_port(in);
            return;
        }
    }
    pthread_mutex_unlock(&m->input_port_root.rlock);
    LOGE(commod, 0x4bd, "module(%s) have no input port %d\n", m->name, number);
}

/* @0x2db04 */
void aud_module_del_output_port(struct aud_module *m, unsigned int number)
{
    struct link *l;

    pthread_mutex_lock(&m->output_port_root.rlock);
    for_each_link(l, &m->output_port_root.nodes) {
        struct aud_port *out = ((struct aud_link_node *)l)->data;

        if (out->number == number) {
            aud_link_del_locked(&out->module_node);
            pthread_mutex_unlock(&m->output_port_root.rlock);
            del_output_port(out);
            return;
        }
    }
    pthread_mutex_unlock(&m->output_port_root.rlock);
    LOGE(commod, 0x517, "module(%s) have no output port %d\n", m->impl->name, number);
}

/* @0x2dba8 */
int aud_module_connect(struct aud_port *out, struct aud_port *in)
{
    LOGI(commod, 0x552, "module connect %s.%d -> %s.%d\n", out->module->name, out->number,
         in->module->name, in->number);
    pthread_mutex_lock(&in->async_up_lock);
    in->up_peer = out;
    pthread_mutex_unlock(&in->async_up_lock);
    aud_link_add_tail(&out->down_peer_root, &in->down_peer_node);
    return 0;
}

/* @0x2ce44 */
void aud_module_unblock_locked(struct aud_module *m)
{
    pthread_mutex_lock(&m->block_mutex);
    if (m->block_module) {
        LOGI(commod, 0x9a, "trying to unblock module %s", m->name);
        m->block_module = false;
        pthread_cond_broadcast(&m->block_cond);
    }
    pthread_mutex_unlock(&m->block_mutex);
}

/* @0x2e1e0: disconnect and FREE the input port. Sends an async DISCONNECT (input gets
 * AUD_PORT_DISABLED, so in-flight data is dropped), unlinks it from the up peer, then a
 * sync DISCONNECT.
 */
void aud_module_disconnect(struct aud_port *out, struct aud_port *in)
{
    struct aud_module *min = in->module;
    struct aud_module *mout = out->module;

    LOGI(commod, 0x522, "module disconnect %s.%d -> %s.%d\n", mout->name, out->number,
         min->name, in->number);
    if (out->down_peer_root.node_count == 1 && mout->impl->deleting_port) {
        LOGI(commod, 0x528, "deleting output %s.%d\n", mout->name, out->number);
        mout->impl->deleting_port(mout, out);
    }
    aud_module_unblock_locked(min);
    aud_module_unblock_locked(mout);

    LOGI(commod, 0x530, "send disconnect event..\n");
    aud_module_event(in->module, in, aud_event_new_disconnect());
    aud_link_del(&in->down_peer_node);

    pthread_mutex_lock(&in->down_peer_node.lock);
    pthread_mutex_lock(&in->stream_lock);
    pthread_mutex_lock(&in->async_down_lock);
    pthread_mutex_unlock(&in->down_peer_node.lock);
    pthread_mutex_lock(&in->async_up_lock);
    in->up_peer = NULL;
    pthread_mutex_unlock(&in->async_up_lock);
    pthread_mutex_unlock(&in->async_down_lock);
    pthread_mutex_unlock(&in->stream_lock);

    aud_module_event(in->module, in, aud_event_new_disconnect_sync());
    aud_link_del(&in->module_node);
    del_input_port(in);
}

/* ------------------------------------------------------------------------- */
/* chain                                                                     */
/* ------------------------------------------------------------------------- */

/* @0x2c3b8: negative (realtime) thread priorities are reset to 0 */
void adjust_thread_priority(struct aud_module *m)
{
    if (m->thread_priority >= 0)
        return;
    LOGI(commod, 0x589, "%s : change thread priority %d to %d\n", m->name, 0,
         m->thread_priority);
    m->thread_priority = 0;
}

/* @0x2c9c8: hand one data object to the module owning input port `in` (stream_lock held).
 * Consumes `data`. Data is DROPPED (return -1) when the input port is disconnecting
 * (AUD_PORT_DISABLED) or throwing (between async and sync FLUSH).
 */
int aud_module_chain_locked(struct aud_port *in, struct aud_data *data)
{
    struct aud_module *m = in->module;
    int ret;

    pthread_mutex_lock(&m->block_mutex);
    if (m->block_module) {
        LOGI(commod, 0x599, "%s : blocked.. goto sleep\n", m->name);
        pthread_cond_wait(&m->block_cond, &m->block_mutex);
        LOGI(commod, 0x59b, "%s : awaken from block!!\n", m->name);
    }
    pthread_mutex_unlock(&m->block_mutex);

    LOGD(commod, 0x59f, "data index %6d, size %4d, timestamp %6u, to %s\n", data->info.index,
         data->size, data->info.timestamp, m->impl->name);

    if (in->flag & AUD_PORT_DISABLED) {
        LOGD(commod, 0x5a3, "port disconnecting.\n");
        goto drop;
    }
    if (in->throwing) {
        LOGD(commod, 0x5aa, "data index %6d, Throwing...%s\n", data->info.index, m->impl->name);
        goto drop;
    }

    if (m->thread_priority)
        adjust_thread_priority(m);

    if (!in->current_first) {
        struct aud_event *first = NULL;

        LOGI(commod, 0x5b6, "chain without first at input%d of %s. "
             "get first event from up_peer..\n", in->number, m->impl->name);
        if (in->up_peer && in->up_peer->current_first)
            first = (struct aud_event *)aud_object_get(&in->up_peer->current_first->object);
        if (!first) {
            LOGW(commod, 0x5bc, "no first at up_peer. make new\n");
            first = aud_event_new_first();
            if (!first) {
                LOGE(commod, 0x5c0, "NULL e.\n");
                return -1;
            }
        }
        in->current_first = first;
        m->impl->event(m, in, (struct aud_event *)aud_object_get(&first->object));
    }

    in->last_clock = aud_system_cycle();
    in->passed += data->size;
    in->flag_work |= AUD_PORT_WORK_CHAIN;
    if (m->do_chain_profile) {
        long long t0 = aud_thread_cycle();

        ret = m->impl->chain(in, data);
        m->chain_profile += aud_thread_cycle() - t0;
    } else {
        ret = m->impl->chain(in, data);
    }
    in->flag_work &= ~AUD_PORT_WORK_CHAIN;

    /* forward whatever chain() pushed, unless an output thread/impl does it */
    if (!m->othread && !(m->impl->flag & (AUD_IMPL_HAVE_OUTPUT_THREAD | AUD_IMPL_SKIP_SYNC)))
        aud_link_foreach_node(&m->output_port_root, NULL, foreach_sync);
    return ret;

drop:
    aud_object_put(&data->object);
    return -1;
}

/* @0x2dc48 */
int aud_module_chain(struct aud_port *port, struct aud_data *data)
{
    int ret;

    pthread_mutex_lock(&port->stream_lock);
    ret = aud_module_chain_locked(port, data);
    pthread_mutex_unlock(&port->stream_lock);
    return ret;
}

/* @0x2d454: queue data on output port n. Never forwards by itself (see file header).
 * Dropped (and released) when: drop-output-data is set, drop-output-meta is set and the
 * data is type_meta, or output port n does not exist. Always returns 0.
 */
int aud_module_push_data(struct aud_module *m, int out_port_num, struct aud_data *data)
{
    struct aud_port *out;

    if (!data) {
        LOGD(commod, 0xd6, "%s: no data\n", m->name);
        return 0;
    }
    if (m->drop_output_data) {
        LOGD(commod, 0xdc, "%s: drop output data.\n", m->name);
        goto drop;
    }
    if (m->drop_output_meta && data->info.type == type_meta) {
        LOGD(commod, 0xe3, "%s: drop output data.\n", m->name);
        goto drop;
    }
    out = aud_module_get_output(m, out_port_num);
    if (!out) {
        LOGD(commod, 0xeb, "%s: no output port %d\n", m->name, out_port_num);
        goto drop;
    }
    pthread_mutex_lock(&out->stream_lock);
    aud_module_put_port(out);
    add_pending_obj_locked(out, &data->object);
    pthread_mutex_unlock(&out->stream_lock);
    if (aud_module_sync_at_push)
        aud_module_sync(m, out_port_num);
    return 0;

drop:
    aud_object_put(&data->object);
    return 0;
}

/* @0x2d3dc: flush output port n (all ports if n < 0) to the down peers */
int aud_module_sync(struct aud_module *m, int out_port_num)
{
    struct aud_port *out;

    if (out_port_num < 0) {
        aud_link_foreach_node(&m->output_port_root, NULL, foreach_sync);
        return 0;
    }
    out = aud_module_get_output(m, out_port_num);
    if (!out)
        LOGD(commod, 0x278, "no output port %d on %s\n", out_port_num, m->impl->name);
    else
        foreach_sync(NULL, out);
    return 0;
}

/* ------------------------------------------------------------------------- */
/* events                                                                    */
/* ------------------------------------------------------------------------- */

/* log lines of the helpers inlined into aud_module_event_locked keep their own __func__ */
#define LOG_DISC(line, ...) \
    LOGF(commod, log_level_info, "aud_module_event_disconnect_locked", line, __VA_ARGS__)
#define LOG_LAST(line, ...) \
    LOGF(commod, log_level_warn, "aud_module_event_last_au_locked", line, __VA_ARGS__)
#define LOG_FLUSH(line, ...) \
    LOGF(commod, log_level_info, "aud_module_event_flush_locked", line, __VA_ARGS__)

/* @0x2c41c: deliver an event to module m (input port `port`, or NULL = module itself).
 * Takes its own reference; the impl->event() handler consumes it.
 * Port state machine: async DISCONNECT sets AUD_PORT_DISABLED, sync DISCONNECT clears it;
 * async FLUSH sets `throwing` on all inputs, sync FLUSH clears it.
 */
int aud_module_event_locked(struct aud_module *m, struct aud_port *port, struct aud_event *event)
{
    struct aud_event *e;
    struct link *l;
    int ret;

    if (!m || !event) {
        LOGE(commod, 0x66c, "Oops. module %p, event %p.\n", m, event);
        return -1;
    }
    e = (struct aud_event *)aud_object_get(&event->object);

    if (port && !(e->prop & (AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC)))
        pthread_mutex_lock(&port->async_up_lock);

    if (LOGMM_ON(commod, log_level_info)) {
        char *s = aud_event_str(e);

        LOGI(commod, 0x67e, "event on %s %s\n", m->name, s ? s : "<null>");
        free(s);
    }
    if (!port)
        return m->impl->event(m, NULL, e);

    if (e->type == AUD_EVENT_TYPE_DISCONNECT) {
        if (e->prop & AUD_EVENT_PROP_SYNC) {
            LOG_DISC(0x639, "(sync) disconnecting.\n");
            if (port->flag & AUD_PORT_DISABLED) {
                LOG_DISC(0x63c, "input%d of %sx. disconnect done.\n", port->number,
                         m->name);
                port->flag &= ~AUD_PORT_DISABLED;
            } else {
                LOG_DISC(0x641, "input%d of %s. no more disconnecting.\n", port->number,
                         m->name);
            }
        } else {
            LOG_DISC(0x625, "(async) disconnecting.\n");
            port->flag |= AUD_PORT_DISABLED;
            if (port->throwing) {
                LOG_DISC(0x629, "%s : port is throwing.. so unset\n", port->module->name);
                port->throwing = 0;
            }
            if (m->impl->disconnect_port && m->impl->disconnect_port(m, port) >= 0) {
                LOG_DISC(0x632, "disconnect event handled at input%d of %s\n",
                         port->number, m->name);
                if (!(e->prop & (AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC)))
                    pthread_mutex_unlock(&port->async_up_lock);
                aud_object_put(&e->object);
                return 0;
            }
        }
        ret = m->impl->event(m, port, e);
    } else if (port->flag & AUD_PORT_DISABLED) {
        LOGI(commod, 0x687, "port disconnecting.\n");
        ret = -1;
        aud_object_put(&e->object);
    } else {
        switch (e->type) {
        case AUD_EVENT_TYPE_FIRST_AU:
            if (port->current_first)
                aud_object_put(&port->current_first->object);
            port->current_first = (struct aud_event *)aud_object_get(&e->object);
            adjust_thread_priority(m);
            break;
        case AUD_EVENT_TYPE_LAST_AU:
            if (port->current_first) {
                aud_object_put(&port->current_first->object);
                port->current_first = NULL;
            } else {
                LOG_LAST(0x65d, "last without first.. at input%d of %s\n", port->number,
                         m->impl->name);
            }
            break;
        case AUD_EVENT_TYPE_FLUSH: {
            int throwing = !(e->prop & AUD_EVENT_PROP_SYNC);

            if (throwing)
                LOG_FLUSH(0x605, "(async) flush event received:%s\n", m->impl->name);
            else
                LOG_FLUSH(0x611, "(sync) flush event received:%s\n", m->name);
            pthread_mutex_lock(&m->input_port_root.rlock);
            for_each_link(l, &m->input_port_root.nodes) {
                struct aud_port *in = ((struct aud_link_node *)l)->data;

                pthread_mutex_lock(&in->check_free_mutex);
                in->throwing = throwing;
                pthread_mutex_unlock(&in->check_free_mutex);
            }
            pthread_mutex_unlock(&m->input_port_root.rlock);
            aimc_raise_named_event(m->aimc, m->uid, throwing ? "flush_start" : "flush_done",
                                   NULL, 0);
            break;
        }
        default:
            break;
        }
        ret = m->impl->event(m, port, e);
    }

    if (!(e->prop & (AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC)))
        pthread_mutex_unlock(&port->async_up_lock);
    return ret;
}

/* @0x2dc6c: consumes event */
int aud_module_event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    int ret;

    if (!e) {
        LOGW(commod, 0x6c5, "no event.\n");
        return -1;
    }
    if (!port) {
        ret = aud_module_event_locked(m, NULL, e);
    } else {
        if (e->prop & AUD_EVENT_PROP_SYNC)
            pthread_mutex_lock(&port->stream_lock);
        else if (e->prop & AUD_EVENT_PROP_DOWNWARD)
            pthread_mutex_lock(&port->async_down_lock);
        ret = aud_module_event_locked(m, port, e);
        if (e->prop & AUD_EVENT_PROP_SYNC)
            pthread_mutex_unlock(&port->stream_lock);
        else if (e->prop & AUD_EVENT_PROP_DOWNWARD)
            pthread_mutex_unlock(&port->async_down_lock);
    }
    aud_object_put(&e->object);
    return ret;
}

/* @0x2dd1c: sync events are queued like data; async downward events go straight to the
 * down peers; upward events go to the up peer.
 */
int aud_port_push_event(struct aud_port *port, struct aud_event *e)
{
    if (!port) {
        LOGD(commod, 0x19a, "no port.\n");
        if (e)
            aud_object_put(&e->object);
        return -1;
    }
    if (!e) {
        LOGD(commod, 0x1a2, "no event\n");
        return -1;
    }
    if (e->prop & AUD_EVENT_PROP_SYNC)
        return add_pending_obj(port, &e->object);

    if (port->flag & AUD_PORT_OUTPUT) {
        pthread_mutex_lock(&port->async_down_lock);
        aud_link_foreach_node(&port->down_peer_root, e, foreach_down_peer_push_event);
        pthread_mutex_unlock(&port->async_down_lock);
    } else {
        struct aud_port *up;

        pthread_mutex_lock(&port->async_up_lock);
        up = port->up_peer;
        if (up)
            aud_module_event(up->module, up, (struct aud_event *)aud_object_get(&e->object));
        pthread_mutex_unlock(&port->async_up_lock);
    }
    aud_object_put(&e->object);
    return 0;
}

/* @0x2de18: aud_link_foreach_node callback over a module's port roots
 * (port->module_node.lock held on entry).
 */
int foreach_push_event(void *arg, void *node_data)
{
    struct aud_event *e = arg;
    struct aud_port *port = node_data;

    if (!e) {
        LOGD(commod, 0x212, "no event\n");
        pthread_mutex_unlock(&port->module_node.lock);
        return -1;
    }
    e = (struct aud_event *)aud_object_get(&e->object);
    LOGT(commod, 0x218, "push event %s.%d %s\n", port->module->name, port->number,
         aud_event_name(e));

    if (e->prop & AUD_EVENT_PROP_SYNC) {
        /* inlined foreach_push_sync_event */
        struct aud_module *m = port->module;

        if (e->type == AUD_EVENT_TYPE_DISCONNECT) {
            if (!(port->flag & AUD_PORT_DISABLED)) {
                LOGI(commod, 0x1c7, "last disconnect done event on %s.output%d\n", m->name,
                     port->number);
                pthread_mutex_unlock(&port->module_node.lock);
                aud_object_put(&e->object);
                return 0;
            }
        } else if (e->type == AUD_EVENT_TYPE_FLUSH) {
            LOGI(commod, 0x1d2, "flush event on %s. output%d\n", m->name, port->number);
            pthread_mutex_lock(&port->check_free_mutex);
            port->flushing = 0;
            pthread_mutex_unlock(&port->check_free_mutex);
        }
        pthread_mutex_lock(&port->stream_lock);
        pthread_mutex_unlock(&port->module_node.lock);
        add_pending_obj_locked(port, &e->object);
        if (m->othread)
            pthread_cond_broadcast(m->othread_kick);
        else if (!(m->impl->flag & AUD_IMPL_HAVE_OUTPUT_THREAD))
            aud_port_sync_locked(port);

        if (e->type == AUD_EVENT_TYPE_DISCONNECT && (port->flag & AUD_PORT_DISABLED)) {
            LOGI(commod, 0x1f8, "disconnect done event on %s.output%d\n",
                 port->module->impl->name, port->number);
            port->flag &= ~AUD_PORT_DISABLED;
            pthread_mutex_lock(&port->check_free_mutex);
            port->flushing = 0;
            pthread_mutex_unlock(&port->check_free_mutex);
        }
        pthread_mutex_unlock(&port->stream_lock);
        return 0;
    }

    if (!(port->flag & AUD_PORT_OUTPUT)) {
        struct aud_port *up;

        pthread_mutex_lock(&port->async_up_lock);
        pthread_mutex_unlock(&port->module_node.lock);
        up = port->up_peer;
        if (up)
            aud_module_event(up->module, up, (struct aud_event *)aud_object_get(&e->object));
        pthread_mutex_unlock(&port->async_up_lock);
        aud_object_put(&e->object);
        return 0;
    }

    if (e->type == AUD_EVENT_TYPE_DISCONNECT || e->type == AUD_EVENT_TYPE_FLUSH) {
        if (e->type == AUD_EVENT_TYPE_DISCONNECT) {
            LOGI(commod, 0x224, "disconnect event on %s.%d\n", port->module->name,
                 port->number);
            port->flag |= AUD_PORT_DISABLED;
        } else {
            LOGI(commod, 0x22f, "flush event on %s.%d", port->module->name, port->number);
        }
        /* wake a writer blocked on the legacy buffer and a blocked module */
        pthread_mutex_lock(&port->check_free_mutex);
        port->flushing = 1;
        pthread_cond_broadcast(&port->check_free_cond);
        pthread_mutex_unlock(&port->check_free_mutex);
        aud_module_unblock_locked(port->module);
    }
    pthread_mutex_lock(&port->async_down_lock);
    pthread_mutex_unlock(&port->module_node.lock);
    aud_link_foreach_node(&port->down_peer_root, e, foreach_down_peer_push_event);
    pthread_mutex_unlock(&port->async_down_lock);
    aud_object_put(&e->object);
    return 0;
}

/* @0x2e138: note it CREATES output port n if missing */
int aud_module_push_event(struct aud_module *m, int out_port_num, struct aud_event *e)
{
    struct aud_port *out;

    if (!e) {
        LOGD(commod, 0x259, "no event\n");
        return -1;
    }
    out = aud_module_new_output(m, out_port_num);
    if (!out) {
        LOGE(commod, 0x263, "no mem for port %d on %s\n", out_port_num, m->name);
        aud_object_put(&e->object);
        return -1;
    }
    foreach_push_event(e, out);
    aud_object_put(&e->object);
    return 0;
}

/* @0x2dc24: downward events go to all outputs, upward ones to all inputs; consumes e */
int aud_module_propagate_event(struct aud_module *m, struct aud_event *e)
{
    struct aud_link_root *root = (e->prop & AUD_EVENT_PROP_DOWNWARD) ? &m->output_port_root
                                                                      : &m->input_port_root;

    aud_link_foreach_node(root, e, foreach_push_event);
    aud_object_put(&e->object);
    return 0;
}

/* @0x2ed88: default impl->event: bookkeeping, then propagate; consumes e */
int aud_module_default_event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    struct aud_link_root *root;

    switch (e->type) {
    case AUD_EVENT_TYPE_FIRST_AU:
        LOGI(commod, 0xa19, "first event received at input%d of %s\n",
             port ? (int)port->number : -1, m->name);
        if (m->first_event_out)
            aud_object_put(&m->first_event_out->object);
        m->first_event_out = (struct aud_event *)aud_object_get(&e->object);
        break;
    case AUD_EVENT_TYPE_LAST_AU:
        LOGI(commod, 0xa23, "last event received at input%d of %s\n",
             port ? (int)port->number : -1, m->name);
        aimc_raise_named_event(m->aimc, m->uid, "last", NULL, 0);
        break;
    case AUD_EVENT_TYPE_START:
        aud_module_cmd(m, ADEC_CMD_START, NULL, 0);
        break;
    case AUD_EVENT_TYPE_STOP:
        aud_module_cmd(m, ADEC_CMD_STOP, NULL, 0);
        break;
    case AUD_EVENT_TYPE_MARK:
        LOGI(commod, 0xa12, "mark event with param size %d\n", e->param_size);
        aimc_raise_named_event(m->aimc, m->uid, "mark", e->param, e->param_size);
        break;
    default:
        break;
    }
    root = (e->prop & AUD_EVENT_PROP_DOWNWARD) ? &m->output_port_root : &m->input_port_root;
    aud_link_foreach_node(root, e, foreach_push_event);
    aud_object_put(&e->object);
    return 0;
}

/* ------------------------------------------------------------------------- */
/* commands                                                                  */
/* ------------------------------------------------------------------------- */

/*
 * ADEC_CMD_SET_PROP_SINGLE payload {u32 len; char "name=value"} (__func__ "set_prop_single").
 * [observed @0x2eb96] the name is copied to a terminated stack buffer, but the ORIGINAL
 * unterminated "name=value" pointer is what gets passed on as the name, so strcmp() can
 * never match; kept as in the binary.
 */
static inline int set_prop_single(struct aud_module *m, const unsigned int *param)
{
    unsigned int len = param[0], i;
    const char *str = (const char *)(param + 1);

    for (i = 0; (int)len > 0 && i < len; i++) {
        if (str[i] == '=') {
            char *name = __builtin_alloca(i + 1);
            const char *value = str + i + 1;
            unsigned int vlen = len - (i + 1);

            strncpy(name, str, i);
            name[i] = '\0';
            if (m->impl->prop &&
                aud_module_set_prop_table(m, m->impl->prop, str, value, vlen) >= 0)
                return 0;
            if (aud_module_set_prop_table(m, aud_module_common_props, str, value, vlen) < 0 &&
                aud_module_set_prop_list(m, str, value, vlen) < 0)
                LOGW(commod, 0x7f0, "no such property. name %s, on %s\n", str, m->name);
            return 0;
        }
    }
    LOGW(commod, 0x7de, "no =\n");
    return 0;
}

/* @0x2eaa0 */
int aud_module_default_command(struct aud_module *m, unsigned int cmd, void *param,
                               unsigned int len)
{
    struct aud_event *e;

    switch (cmd) {
    case ADEC_CMD_ASYNC_EVENT:
        LOGI(commod, 0x9e3, "ADEC_CMD_ASYNC_EVENT received at %s\n", m->name);
        e = aud_event_deserialize(param, len);
        if (e)
            aud_module_event(m, NULL, e);
        return 0;
    case ADEC_CMD_GET_PROP:
        return aud_module_get_prop(m, param);
    case ADEC_CMD_SET_PROP:
        aud_module_set_prop(m, (char *)param + 4, *(unsigned int *)param);
        return 0;
    case ADEC_CMD_SET_PROP_SINGLE:
        return set_prop_single(m, param);
    case ADEC_CMD_SYNC:
        return 0;
    case ADEC_CMD_CONNECT_METADATA:
    case ADEC_CMD_SET_BASETIME:
        LOGI(commod, 0x9f4, "command not handled. %08x at %s\n", cmd, m->name);
        return 0;
    default:
        LOGI(commod, 0x9f8, "command not handled. %08x(%s) at %s\n", cmd,
             IMC_CommandToString(cmd), m->name);
        return -1;
    }
}

/* @0x2ed0c */
int aud_module_cmd(struct aud_module *m, unsigned int cmd, void *param, unsigned int len)
{
    LOGI(commod, 0x6e2, "command %08X on %s\n", cmd, m->name);
    if (m->impl->cmd)
        return m->impl->cmd(m, cmd, param, len);
    return aud_module_default_command(m, cmd, param, len);
}

/* ------------------------------------------------------------------------- */
/* graphviz dump                                                             */
/* ------------------------------------------------------------------------- */

/* @0x2c184: per-port label; also resets the "passed" byte counter */
char *aud_port_dot_label(struct aud_port *port)
{
    unsigned int passed = port->passed;
    unsigned long long clk = port->last_clock;
    char *s, *ev;
    unsigned int len = 0;

    port->passed = 0;
    s = audutil_append_printf(NULL, &len,
            "\npassed:%u\nlast_ms:%lld.%03lld\nflag:%08x%s%s\nflag_work:%08x\n", passed,
            (long long)(clk / 1000000), (long long)((clk / 1000) % 1000), port->flag,
            port->flushing ? ",flushing" : "", port->throwing ? ",throwing" : "",
            port->flag_work);

    if (port->legacy_buf && !aud_module_skip_ext_dump) {
        ADEC_BUF_T *lb = port->legacy_buf;

        s = audutil_append_printf(s, &len, "\nlegacy_buf:%d/%d(%d-%d), %d/%d(%d-%d)",
                                  lb->get_used_size(lb), lb->get_max_size(lb), lb->status.readOffset,
                                  lb->status.writeOffset, lb->get_au_cnt(lb), lb->get_max_au(lb),
                                  lb->status.readIndex, lb->status.writeIndex);
    }
    if (port->current_first) {
        ev = aud_event_str2(port->current_first, 16, ",\\l");
        if (ev) {
            s = audutil_append_printf(s, &len, "\n%s\\l", ev);
            free(ev);
        }
    }
    if (port->starter && !aud_module_skip_ext_dump) {
        ADEC_BUF_T *lb = aud_starter_get_lbuf(port->starter);

        s = audutil_append_printf(s, &len, "\nstarter lbuf:%d/%d(%d-%d), %d/%d(%d-%d)",
                                  lb->get_used_size(lb), lb->get_max_size(lb), lb->status.readOffset,
                                  lb->status.writeOffset, lb->get_au_cnt(lb), lb->get_max_au(lb),
                                  lb->status.readIndex, lb->status.writeIndex);
    }
    return s;
}

static inline __attribute__((always_inline)) void dot_ports(struct aud_module *m,
                                                           struct aud_link_root *root,
                                                           const char *dir)
{
    struct link *l;

    pthread_mutex_lock(&root->rlock);
    for_each_link(l, &root->nodes) {
        struct aud_port *port = ((struct aud_link_node *)l)->data;
        char *label = aud_port_dot_label(port);

        logm_message("  %s_%p_%s%x [label=\"%s%x%s\", fillcolor=lightgrey];\n", m->name, port,
                     dir, port->number, dir, port->number, label ? label : "");
        free(label);
    }
    pthread_mutex_unlock(&root->rlock);
}

/* @0x2e8e4 */
int aud_module_default_dot_wo_proc(struct aud_module *m)
{
    char *props = aud_module_prop_str(m, "%16s=%s\\l");

    logm_message(" subgraph cluster_%s_%p {\n", m->name, m);
    logm_message("  label=\"%s:%08x\";\n", m->impl->name, m->uid);
    logm_message("  %s_props [label=\"%s\", style=dotted];\n", m->name, props ? props : "");
    free(props);
    dot_ports(m, &m->input_port_root, "in");
    dot_ports(m, &m->output_port_root, "out");
    logm_message(" }\n");
    return 0;
}

/* @0x2ea6c */
int aud_module_default_dot(struct aud_module *m)
{
    logm_message(" subgraph cluster_proc_%d {\n", m->uid >> 24);
    aud_module_default_dot_wo_proc(m);
    logm_message(" }\n");
    return 0;
}

/* @0x2ea94 */
int aud_module_dot(struct aud_module *m)
{
    if (m->impl->dot)
        return m->impl->dot(m);
    return aud_module_default_dot(m);
}

/* ------------------------------------------------------------------------- */
/* output thread                                                             */
/* ------------------------------------------------------------------------- */

/* @0x2eefc */
int aud_module_output_thread_create(struct aud_module *m,
                                    int (*cb)(struct aud_module *m, void *priv), void *priv,
                                    pthread_mutex_t *lock, pthread_cond_t *kick)
{
    if (m->othread) {
        LOGW(commod, 0xa4b, "%s: output thread already running.\n", m->name);
        return -1;
    }
    m->othread_callback = cb;
    m->othread_callback_priv = priv;
    m->othread_lock = lock;
    m->othread_running = true;
    m->othread_kick = kick;
    LOGI(commod, 0xa55, "%s: create output thread.\n", m->name);
    m->othread = aud_thread_create(m->name, 0x10000, 5, aud_module_output_thread_main, m);
    if (m->othread)
        return 0;
    LOGW(commod, 0xa5a, "%s: cannot make output thread.\n", m->name);
    return -1;
}

/* @0x2efd8 */
int aud_module_output_thread_destroy(struct aud_module *m)
{
    if (!m->othread) {
        LOGW(commod, 0xa65, "%s: output thread not running..\n", m->name);
        return -1;
    }
    pthread_mutex_lock(m->othread_lock);
    LOGI(commod, 0xa6a, "%s: stop output thread.\n", m->name);
    m->othread_running = false;
    pthread_cond_broadcast(m->othread_kick);
    pthread_mutex_unlock(m->othread_lock);
    aud_thread_destroy(m->othread);
    m->othread = NULL;
    return 0;
}

/* ------------------------------------------------------------------------- */
/* bin: a module containing other modules (logmm module "bin")               */
/* ------------------------------------------------------------------------- */

int bin_constructor(struct aud_module *m, struct aimc *imc, const char *param);
void bin_destructor(struct aud_module *m);
int bin_chain(struct aud_port *port, struct aud_data *data);
int bin_command(void *arg, unsigned int cmd, void *param, unsigned int len);
int bin_dot(struct aud_module *m);
int bin_initialize(struct aud_module *m);
void bin_cleanup(struct aud_module *m);
int bin_event(struct aud_module *m, struct aud_port *p, struct aud_event *e);
int bin_cmd(void *arg, unsigned int cmd, void *param, unsigned int len);
int binout_chain(struct aud_port *port, struct aud_data *data);
int binout_event(struct aud_module *m, struct aud_port *p, struct aud_event *e);

/* @0x2f090: class_init of "bin"; derived classes fill in name/instance_size */
int aud_object_class_init_bin(void *klass)
{
    struct aud_module_bin_impl *impl = klass;
    struct aud_module_impl *mi = &impl->parent;

    mi->name = NULL;
    mi->constructor = bin_constructor;
    mi->destructor = bin_destructor;
    mi->chain = bin_chain;
    mi->event = NULL;
    mi->instance_size = 0;
    mi->cmd = bin_command;
    mi->dot = bin_dot;
    mi->flag = AUD_IMPL_SKIP_SYNC;
    impl->bin_initialize = bin_initialize;
    impl->bin_cleanup = bin_cleanup;
    impl->bin_event = bin_event;
    impl->bin_cmd = bin_cmd;
    return 0;
}

/* @0x2f108: class_init of "binout" */
int aud_object_class_init_binout(void *klass)
{
    struct aud_module_impl *impl = klass;

    impl->instance_size = sizeof(struct aud_module_binout);
    impl->name = "binout";
    impl->chain = binout_chain;
    impl->event = binout_event;
    return 0;
}

/* @0x2f138: holder for bin port `number` in an input/output holder list */
struct aud_bin_port *bin_find_port(struct aud_module_bin *bin, struct link *root, int number)
{
    struct link *l;

    LOGD(bin, 0x43, "%s: find port%d\n", bin->module.name, number);
    for_each_link(l, root) {
        struct aud_bin_port *h = link_container(struct aud_bin_port, list, l);

        LOGT(bin, 0x48, "%s: port holder %d , port number %d\n", bin->module.name, h->bin_port,
             number);
        if (h->bin_port == number) {
            LOGD(bin, 0x4c, "%s: find input port%d\n", bin->module.name, number);
            return h;
        }
    }
    LOGD(bin, 0x51, "%s: no matched port\n", bin->module.name);
    return NULL;
}

/* inlined into bin_initialize and bin_constructor */
static inline int bin_init_lists(struct aud_module_bin *bin)
{
    LOGF(bin, log_level_info, "bin_initialize", 0xbd, "%s: initialize...\n", bin->module.name);
    LINK_INIT(&bin->inst_root);
    LINK_INIT(&bin->cmd_propa_root);
    LINK_INIT(&bin->input_port_root);
    LINK_INIT(&bin->output_port_root);
    LINK_INIT(&bin->pending_cmd_root);
    return 0;
}

/* @0x2f244: bin hook "initialize" (impl + 0x60) */
int bin_initialize(struct aud_module *m)
{
    return bin_init_lists((struct aud_module_bin *)m);
}

/*
 * @0x2f2b4: bin hook "cmd" (impl + 0x6c). Every command is forwarded to the instances on
 * the propagate list; while that list is empty it is queued (with a private copy of the
 * payload) for aud_module_bin_propagate_pending. The bin itself then runs the default
 * command handler either way.
 */
int bin_cmd(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    struct aud_module_bin *bin = arg;
    struct aud_bin_pending_cmd *pc;
    struct link *l;

    LOGI(bin, 0x90, "%s: command %08x (param = %p, len = %d)\n", bin->module.name, cmd, param,
         len);
    if (link_empty(&bin->cmd_propa_root)) {
        LOGI(bin, 0x94, "%s: no connected instance.. add pending list", bin->module.name);
        pc = calloc(sizeof(*pc), 1);
        if (!pc) {
            LOGE(bin, 0x98, "%s: no mem\n", bin->module.name);
            return -1;
        }
        if (len) {
            pc->param = calloc(len, 1);
            if (!pc->param) {
                LOGE(bin, 0xa1, "%s: no mem\n", bin->module.name);
                free(pc);
                return -1;
            }
            LOGD(bin, 0xa5, "%s: copy from  %p to %p len %d\n", bin->module.name, param,
                 pc->param, len);
            memcpy(pc->param, param, len);
        }
        pc->cmd = cmd;
        pc->len = len;
        link_add_tail(&bin->pending_cmd_root, &pc->list);
    } else {
        for_each_link(l, &bin->cmd_propa_root) {
            struct aud_bin_inst *h = link_container(struct aud_bin_inst, list, l);

            LOGD(bin, 0xb0, "%s: send command to %s", bin->module.name, h->inst->name);
            aud_module_cmd(h->inst, cmd, param, len);
        }
    }
    return aud_module_default_command(&bin->module, cmd, param, len);
}

/* @0x2f48c: impl->cmd of "bin" */
int bin_command(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    return bin_cmd(arg, cmd, param, len);
}

/* @0x2f490: bin hook "event": hand the event to the inner port bound to the bin port it
 * arrived on (downward events: input holders, upward: output holders) */
int bin_event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    struct aud_module_bin *bin = (struct aud_module_bin *)m;
    struct aud_bin_port *h;

    LOGI(bin, 0x7a, "%s: port%d, event %s\n", m->name, port ? (int)port->number : -1,
         aud_event_name(e));
    h = bin_find_port(bin, AUD_EVENT_IS_DOWNWARD(e) ? &bin->input_port_root
                                                    : &bin->output_port_root, port->number);
    if (!h)
        return aud_module_default_event(m, port, e);
    return aud_module_event(h->module, h->port, e);
}

/* @0x2f52c: impl->dot of "bin": one graphviz cluster with the inner modules, their
 * connections (except to "binout" helpers) and the bin port bindings as dashed edges */
int bin_dot(struct aud_module *m)
{
    struct aud_module_bin *bin = (struct aud_module_bin *)m;
    struct link *l, *pl, *pnext, *dl, *dnext;

    logm_message(" subgraph cluster_proc_%d {\n", m->uid >> 24);
    aud_module_default_dot_wo_proc(m);
    logm_message(" subgraph cluster_%s_%p {\n", m->name, m);
    for_each_link(l, &bin->inst_root) {
        struct aud_module *inst = link_container(struct aud_bin_inst, list, l)->inst;

        if (!strcmp("binout", inst->impl->name))
            continue;
        aud_module_default_dot_wo_proc(inst);
        pthread_mutex_lock(&inst->output_port_root.rlock);
        for (pl = inst->output_port_root.nodes.next, pnext = pl->next;
             pl != &inst->output_port_root.nodes; pl = pnext, pnext = pl->next) {
            struct aud_port *out = ((struct aud_link_node *)pl)->data;

            pthread_mutex_lock(&out->down_peer_root.rlock);
            for (dl = out->down_peer_root.nodes.next, dnext = dl->next;
                 dl != &out->down_peer_root.nodes; dl = dnext, dnext = dl->next) {
                struct aud_port *in = ((struct aud_link_node *)dl)->data;
                struct aud_module *peer = in->module;

                if (strcmp("binout", peer->impl->name))
                    logm_message(" %s_%08x_%p_out%x -> %s_%08x_%p_in%x [style=\"dashed\"]\n",
                                 out->module->impl->name, out->module->uid, out, out->number,
                                 peer->impl->name, peer->uid, in, in->number);
            }
            pthread_mutex_unlock(&out->down_peer_root.rlock);
        }
        pthread_mutex_unlock(&inst->output_port_root.rlock);
    }
    for_each_link(l, &bin->input_port_root) {
        struct aud_bin_port *h = link_container(struct aud_bin_port, list, l);
        struct aud_port *p = aud_module_get_input_port(m, h->bin_port);

        if (p && h->port && h->port->module)
            logm_message(" %s_%p_in%x -> %s_%p_in%x [style=\"dashed\"]\n", m->name, p,
                         p->number, h->port->module->name, h->port, h->port->number);
    }
    for_each_link(l, &bin->output_port_root) {
        struct aud_bin_port *h = link_container(struct aud_bin_port, list, l);
        struct aud_port *p = aud_module_get_output_port(m, h->bin_port);

        if (p && h->port && h->port->module)
            logm_message(" %s_%p_out%x -> %s_%p_out%x [style=\"dashed\"]\n",
                         h->port->module->name, h->port, h->port->number, m->name, p,
                         p->number);
    }
    logm_message(" }\n");
    logm_message(" }\n");
    return 0;
}

/* inlined into bin_chain: a "mixerbin" tells itself (property "connect-output=<n>" /
 * "disconnect-output=<n>") whenever one of its outputs gains or loses its last peer */
static inline void bin_check_active_output(struct aud_module_bin *bin)
{
    struct aud_module *m = &bin->module;
    struct link *l;

    if (!m || !m->impl || !m->impl->name || strcmp(m->impl->name, "mixerbin"))
        return;
    for_each_link(l, &bin->output_port_root) {
        struct aud_bin_port *h = link_container(struct aud_bin_port, list, l);
        int prev = h->active;
        int active = aud_module_have_active_output_n(m, h->bin_port);
        char *prop;

        LOGF(bin, log_level_debug, "bin_check_active_output", 0x132,
             "%s: port%d have active output? %d\n", m->name, h->bin_port, active);
        if (prev == active)
            continue;
        LOGF(bin, log_level_noti, "bin_check_active_output", 0x138,
             "%s: output port%d connnected %d -> %d\n", m->name, h->bin_port, prev, active);
        if (active)
            prop = audutil_strdup_printf("connect-output=%d", h->bin_port);
        else
            prop = audutil_strdup_printf("disconnect-output=%d", h->bin_port);
        if (prop) {
            aud_module_set_prop(m, prop, strlen(prop));
            free(prop);
        }
        h->active = active;
    }
}

/* @0x2f71c: impl->chain of "bin": data on bin input N goes to the inner input bound to N */
int bin_chain(struct aud_port *port, struct aud_data *data)
{
    struct aud_module_bin *bin = (struct aud_module_bin *)port->module;
    struct aud_bin_port *h;

    LOGD(bin, 0x150, "%s: port%d got chain \n", bin->module.name, port->number);
    bin_check_active_output(bin);
    h = bin_find_port(bin, &bin->input_port_root, port->number);
    if (h)
        return aud_module_chain(h->port, data);
    LOGD(bin, 0x15a, "%s: no one handles input data\n", bin->module.name);
    aud_data_put(data);
    return 0;
}

/* @0x2f8d8: impl->event of "binout": forward to the bin's own output port */
int binout_event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    struct aud_module_binout *bo = (struct aud_module_binout *)m;
    struct aud_module_bin *bin = bo->bin;

    if (!port) {
        LOGE(bin, 0x322, "%s: no port\n", bin->module.name);
    } else if (AUD_PORT_IS_OUTPUT(port)) {
        LOGE(bin, 0x329, "%s: expect input port\n", bin->module.name);
    } else {
        LOGI(bin, 0x32e, "%s: event %s, from %s to bin_output_port %d\n", bin->module.name,
             aud_event_name(e), m->name, bo->bin_port);
        return aud_module_push_event(&bin->module, bo->bin_port, e);
    }
    aud_event_put(e);
    return -1;
}

/* @0x2f9b8: impl->chain of "binout": push to the bin's output port and flush it */
int binout_chain(struct aud_port *port, struct aud_data *data)
{
    struct aud_module_binout *bo;
    struct aud_module_bin *bin;

    if (port) {
        bo = (struct aud_module_binout *)port->module;
        bin = bo->bin;
        if (!AUD_PORT_IS_OUTPUT(port)) {
            LOGD(bin, 0x313, "%s: data from %s to bin_output_port %d\n", bin->module.name,
                 bo->module.name, bo->bin_port);
            aud_module_push_data(&bin->module, bo->bin_port, data);
            aud_module_sync(&bin->module, bo->bin_port);
            return 0;
        }
        LOGE(bin, 0x30e, "%s: expect input port\n", bin->module.name);
    }
    aud_data_put(data);
    return -1;
}

/* @0x2fa64: impl->constructor of "bin" */
int bin_constructor(struct aud_module *m, struct aimc *imc, const char *param)
{
    (void)imc;
    (void)param;
    return bin_init_lists((struct aud_module_bin *)m);
}

/* @0x2fad4: match by name and/or index (NULL name / negative index = don't care) */
struct aud_module *bin_find_instance(struct aud_module_bin *bin, struct link *root,
                                     const char *name, int index)
{
    struct link *l;

    for_each_link(l, root) {
        struct aud_bin_inst *h = link_container(struct aud_bin_inst, list, l);
        bool name_ok, index_ok;

        LOGD(bin, 100, "%s: compare %s(%d) with %s(%d)\n", bin->module.name,
             name ? name : "NULL", index, h->name, h->index);
        name_ok = !name || !strcmp(name, h->name);
        index_ok = index < 0 || h->index == index;
        LOGD(bin, 0x6a, "%s: name_matched = %s, index_matched %s\n", bin->module.name,
             name_ok ? "TRUE" : "FALSE", index_ok ? "TRUE" : "FALSE");
        if (name_ok && index_ok)
            return h->inst;
    }
    LOGD(bin, 0x71, "%s: no instance. %s(%d)\n", bin->module.name, name ? name : "NULL", index);
    return NULL;
}

/* @0x2fc8c: create an internal instance of impl_name inside the bin */
struct aud_module *aud_module_bin_add(struct aud_module_bin *bin, const char *inst_name)
{
    struct aud_bin_inst *h = calloc(sizeof(*h), 1);
    struct aud_module *m;
    struct link *l;

    if (!h) {
        LOGE(bin, 0x18c, "%s: nomem\n", bin->module.name);
        return NULL;
    }
    m = aud_registry_create_instance(bin->module.aimc, inst_name, 0);
    if (!m) {
        LOGE(bin, 0x193, "%s: cannot create %s\n", bin->module.name, inst_name);
        free(h);
        return NULL;
    }
    h->inst = m;
    h->name = inst_name;
    for_each_link(l, &bin->inst_root)
        if (link_container(struct aud_bin_inst, list, l)->inst->impl == m->impl)
            h->index++;
    LOGI(bin, 0x1a1, "%s: add %s to %s, index %d\n", bin->module.name, m->name,
         bin->module.name, h->index);
    link_add(&bin->inst_root, &h->list);
    return m;
}

/* @0x2fdb4 */
int aud_module_bin_del(struct aud_module_bin *bin, struct aud_module *inst)
{
    struct link *l;

    LOGI(bin, 0x1ab, "%s: del %s from %s\n", bin->module.name, inst->name, bin->module.name);
    for_each_link(l, &bin->inst_root) {
        struct aud_bin_inst *h = link_container(struct aud_bin_inst, list, l);

        if (h->inst == inst) {
            link_del(&h->list);
            aud_registry_destroy_instance(bin->module.aimc, inst);
            free(h);
            return 0;
        }
    }
    LOGE(bin, 0x1b7, "%s: unknown instance, \"%s\", at bin %s\n", bin->module.name, inst->name,
         bin->module.name);
    return -1;
}

/* @0x2fe78 */
struct aud_module *aud_module_bin_search_index(struct aud_module_bin *bin, int index)
{
    return bin_find_instance(bin, &bin->inst_root, NULL, index);
}

/* @0x2fe84 */
struct aud_module *aud_module_bin_search_name(struct aud_module_bin *bin, const char *name)
{
    return bin_find_instance(bin, &bin->inst_root, name, -1);
}

/* @0x2fe94 */
struct aud_module *aud_module_bin_search(struct aud_module_bin *bin, const char *name, int index)
{
    return bin_find_instance(bin, &bin->inst_root, name, index);
}

/* @0x2fea0: free all holders, destroy all inner instances and pending commands */
struct aud_module *aud_module_bin_clear(struct aud_module_bin *bin)
{
    struct link *l;

    LOGI(bin, 0x1d3, "%s: clear bin..\n", bin->module.name);
    while ((l = bin->input_port_root.next) != &bin->input_port_root) {
        struct aud_bin_port *h = link_container(struct aud_bin_port, list, l);

        LOGI(bin, 0x1d8, "%s: free input port %d holder from %s\n", bin->module.name,
             h->bin_port, bin->module.name);
        if (h->module && h->port)
            aud_module_del_input_port(h->module, h->port->number);
        link_del(&h->list);
        free(h);
    }
    while ((l = bin->output_port_root.next) != &bin->output_port_root) {
        struct aud_bin_port *h = link_container(struct aud_bin_port, list, l);

        LOGD(bin, 0x1e4, "%s: free output port %d holder from %s\n", bin->module.name,
             h->bin_port, bin->module.name);
        link_del(&h->list);
        free(h);
    }
    while ((l = bin->cmd_propa_root.next) != &bin->cmd_propa_root) {
        struct aud_bin_inst *h = link_container(struct aud_bin_inst, list, l);

        LOGD(bin, 0x1ec, "%s: remove %s from %s\n", bin->module.name, h->inst->name,
             bin->module.name);
        link_del(&h->list);
        free(h);
    }
    while ((l = bin->inst_root.next) != &bin->inst_root) {
        struct aud_bin_inst *h = link_container(struct aud_bin_inst, list, l);

        LOGD(bin, 0x1f6, "%s: destroy %s(%d) from %s\n", bin->module.name, h->inst->name,
             h->index, bin->module.name);
        aud_registry_destroy_instance(bin->module.aimc, h->inst);
        link_del(&h->list);
        free(h);
    }
    while ((l = bin->pending_cmd_root.next) != &bin->pending_cmd_root) {
        struct aud_bin_pending_cmd *c = link_container(struct aud_bin_pending_cmd, list, l);

        LOGD(bin, 0x201, "%s: free pending cmd 0x%x from %s\n", bin->module.name, c->cmd,
             bin->module.name);
        if (c->param)
            free(c->param);
        link_del(&c->list);
        free(c);
    }
    return NULL;
}

/* @0x30120: bin hook "cleanup" (impl + 0x64) */
void bin_cleanup(struct aud_module *m)
{
    aud_module_bin_clear((struct aud_module_bin *)m);
}

/* @0x30124: impl->destructor of "bin" */
void bin_destructor(struct aud_module *m)
{
    aud_module_bin_clear((struct aud_module_bin *)m);
}

/* @0x30128 */
int aud_module_bin_connect(struct aud_module_bin *bin, struct aud_module *out, int port_out,
                           struct aud_module *in, int port_in)
{
    if (!out) {
        LOGE(bin, 0x210, "%s: no output\n", bin->module.name);
        return -1;
    }
    if (!in) {
        LOGE(bin, 0x216, "%s: no input\n", bin->module.name);
        return -1;
    }
    LOGI(bin, 0x21a, "%s: connecting.. %s.%d -> %s.%d\n", bin->module.name, out->name, port_out,
         in->name, port_in);
    return aud_connector_connect(out, port_out, in, port_in);
}

/* @0x301f4: expose in.port_in as the bin's input port_bin_in (port_in -1 = first free) */
int aud_module_bin_set_input(struct aud_module_bin *bin, int port_bin_in,
                             struct aud_module *in, int port_in)
{
    struct aud_bin_port *h;
    struct link *l;

    if (!in) {
        LOGE(bin, 0x226, "%s: no in module\n", bin->module.name);
        return -1;
    }
    if (port_in == -1) {
        for (port_in = 0; port_in != 0xff; port_in++) {
            for_each_link(l, &bin->input_port_root) {
                h = link_container(struct aud_bin_port, list, l);
                if (h->port && (int)h->port->number == port_in)
                    break;
            }
            if (l == &bin->input_port_root)
                break;
        }
        if (port_in == 0xff) {
            LOGE(bin, 0x243, "%s: can't find port number\n", bin->module.name);
            return -1;
        }
    }
    LOGI(bin, 0x248, "%s: set %s.in_%d as bin.in_%d\n", bin->module.name, in->name, port_in,
         port_bin_in);
    h = calloc(sizeof(*h), 1);
    if (!h) {
        LOGE(bin, 0x24d, "%s: no mem\n", bin->module.name);
        return -1;
    }
    h->active = -1;
    h->module = in;
    h->port = aud_module_new_input_port(in, port_in);
    h->bin_port = port_bin_in;
    link_add_tail(&bin->input_port_root, &h->list);
    return 0;
}

/* @0x30374 */
int aud_module_bin_unset_input(struct aud_module_bin *bin, int port_bin_in)
{
    struct aud_bin_port *h;

    LOGI(bin, 0x260, "%s: unset bin.in_%d\n", bin->module.name, port_bin_in);
    h = bin_find_port(bin, &bin->input_port_root, port_bin_in);
    if (h) {
        if (h->module && h->port)
            aud_module_del_input_port(h->module, h->port->number);
        link_del(&h->list);
        free(h);
    }
    return 0;
}

/* @0x303f0: expose out.port_out as the bin's output port_bin_out through a "binout" module */
int aud_module_bin_set_output(struct aud_module_bin *bin, int port_bin_out,
                              struct aud_module *out, int port_out)
{
    struct aud_module_binout *bo;
    struct aud_bin_port *h;

    if (!out) {
        LOGE(bin, 0x279, "%s: no out module\n", bin->module.name);
        return -1;
    }
    LOGI(bin, 0x27d, "%s: set %s.out_%d as bin.out_%d\n", bin->module.name, out->name, port_out,
         port_bin_out);
    h = calloc(sizeof(*h), 1);
    if (!h) {
        LOGE(bin, 0x282, "%s: no mem\n", bin->module.name);
        return -1;
    }
    h->active = -1;
    h->module = out;
    h->port = aud_module_new_output_port(out, port_out);
    h->bin_port = port_bin_out;
    link_add_tail(&bin->output_port_root, &h->list);

    bo = (struct aud_module_binout *)aud_module_bin_add(bin, "binout");
    if (!bo) {
        LOGE(bin, 0x291, "%s: no binout\n", bin->module.name);
        return -1;
    }
    bo->bin = bin;
    bo->bin_port = port_bin_out;
    aud_connector_connect(out, port_out, &bo->module, 0);
    return 0;
}

/* @0x30558 */
int aud_module_bin_unset_output(struct aud_module_bin *bin, int port_bin_out)
{
    struct aud_bin_port *h;

    LOGI(bin, 0x2a1, "%s: unset bin.out_%d\n", bin->module.name, port_bin_out);
    h = bin_find_port(bin, &bin->output_port_root, port_bin_out);
    if (h) {
        if (h->module && h->port)
            aud_module_del_output_port(h->module, h->port->number);
        link_del(&h->list);
        free(h);
    }
    return 0;
}

/* @0x305d4 */
int aud_module_bin_unset_output_no_del_port(struct aud_module_bin *bin, int port_bin_out)
{
    struct aud_bin_port *h;

    LOGI(bin, 0x2b5, "%s: unset bin.out_%d\n", bin->module.name, port_bin_out);
    h = bin_find_port(bin, &bin->output_port_root, port_bin_out);
    if (h) {
        link_del(&h->list);
        free(h);
    }
    return 0;
}

/* @0x30640: commands to the bin are forwarded to every module in this list */
int aud_module_bin_add_cmd_propagate(struct aud_module_bin *bin, struct aud_module *m)
{
    struct aud_bin_inst *h;

    if (!m) {
        LOGE(bin, 0x2c8, "%s: no propagate module\n", bin->module.name);
        return -1;
    }
    LOGI(bin, 0x2cc, "%s: add %s into propagate list\n", bin->module.name, m->name);
    if (bin_find_instance(bin, &bin->cmd_propa_root, m->name, -1)) {
        LOGI(bin, 0x2d0, "%s: %s already added.. ", bin->module.name, m->name);
        return 0;
    }
    h = calloc(sizeof(*h), 1);
    if (!h) {
        LOGE(bin, 0x2d7, "%s: no mem\n", bin->module.name);
        return -1;
    }
    h->inst = m;
    h->name = m->name;
    link_add(&bin->cmd_propa_root, &h->list);
    LOGI(bin, 0x2de, "%s: add %s done\n", bin->module.name, h->name);
    return 0;
}

/* @0x30798: replay queued commands to every propagate target, then drop them */
int aud_module_bin_propagate_pending(struct aud_module_bin *bin)
{
    struct link *l, *next;

    for (l = bin->pending_cmd_root.next, next = l->next; l != &bin->pending_cmd_root;
         l = next, next = l->next) {
        struct aud_bin_pending_cmd *c = link_container(struct aud_bin_pending_cmd, list, l);
        struct link *t;

        for_each_link(t, &bin->cmd_propa_root) {
            struct aud_module *m = link_container(struct aud_bin_inst, list, t)->inst;

            LOGI(bin, 0x2ef, "%s: send cmd %d to %s", bin->module.name, c->cmd, m->name);
            m->impl->cmd(m, c->cmd, c->param, c->len);
        }
        free(c->param);
        link_del(&c->list);
        free(c);
    }
    return 0;
}
