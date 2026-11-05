/*
 * libaa object / instance / thread layer:
 *   - aud_inst_role_set, aud_inst_connect and the cross-processor jumper builder
 *     (aud_modlist_connect_inst with its inlined connect_between_processor /
 *     initialize_shbuf, connect_in_processor, destroy_glue) and its teardown
 *     (disconnect_in_processor, destroy_jumper, aud_modlist_disconnect_inst_*)
 *   - aud_thread_* / aud_system_cycle / aud_cond_wait_timeout (os wrapper)
 *   - aud_link_* (locked list, link2.c)
 *   - aud_object_class_* (object.c)
 *   - aud_starter_* and its worker thread (starter.c)
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 * Evidence: decomp/libaa.so.2.0.0.c plus headless-Ghidra decompilation of entry points
 * Ghidra did not discover (module callbacks, starter thread), litpool string resolution.
 * Names come from __func__ strings in the binary and the kdriver tree (names only).
 */
#define _GNU_SOURCE
#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <time.h>

#include "types.h"
#include "objects.h"

LOGMM_MODULE_DEFINE(oswrap);
LOGMM_MODULE_DEFINE(com_link);
LOGMM_MODULE_DEFINE(modstart);

/* ========================================================================= */
/* element list: roles and connections                                      */
/* ========================================================================= */

/* @0x1aae8: attach a role name to instance `uid`; creates a dummy "_predefined" instance
 * if the uid is not known on the processor */
int aud_inst_role_set(struct aud_proc_info *p, unsigned int uid, const char *role)
{
    struct aud_mod_impl *impl = NULL;
    struct aud_mod_inst *inst;
    struct link *l;
    unsigned int impl_num;

    if (!p) {
        pthread_mutex_lock(&processors_lock);
        for_each_link(l, &processors) {
            struct aud_proc_info *q = link_container(struct aud_proc_info, proc_list, l);

            if ((unsigned int)q->proc_id == uid >> 24) {
                p = q;
                break;
            }
        }
        if (!p) {
            pthread_mutex_unlock(&processors_lock);
            LOGE(aud_modlist, 0x5e6, "cannot search processor for uid %08x\n", uid);
            return -22;
        }
        pthread_mutex_lock(&p->mods_lock);
        pthread_mutex_unlock(&processors_lock);
    } else {
        pthread_mutex_lock(&p->mods_lock);
    }

    LOGI(aud_modlist, 0x5ed, "set role \"%s\" to instance %08x @ proc \"%s\"\n", role, uid,
         p->name);

    impl_num = (uid & 0xffffff) >> 16;
    for_each_link(l, &p->mods) {
        struct aud_mod_impl *m = link_container(struct aud_mod_impl, mods_list, l);

        if ((unsigned int)m->impl_num == impl_num) {
            impl = m;
            break;
        }
    }
    if (!impl && impl_num == 0) {
        LOGI(aud_modlist, 0x5fa, "register internal implement for \"_predefined\"..\n");
        impl = register_impl(p, "_predefined", 0, 0);
    }
    if (impl) {
        inst = NULL;
        for_each_link(l, &impl->instances) {
            struct aud_mod_inst *i = link_container(struct aud_mod_inst, impl_list, l);

            if (i->uid == uid) {
                inst = i;
                break;
            }
        }
        if (!inst) {
            LOGI(aud_modlist, 0x60f, "create dummy instance for role \"%s\"\n", role);
            inst = create_inst_on_sync(impl, NULL, false);
        }
        if (inst) {
            aud_modlist_role_set(inst, role);
            pthread_mutex_unlock(&p->mods_lock);
            return 0;
        }
    }
    pthread_mutex_unlock(&p->mods_lock);
    LOGE(aud_modlist, 0x61c, "unknown uid %08x, role %s\n", uid, role);
    return -22;
}

/* @0x1b1dc */
int aud_inst_connect(struct aud_mod_owner *o, unsigned int uid_out, int port_out,
                     unsigned int uid_in, int port_in, bool sync)
{
    struct aud_mod_inst *out, *in;

    LOGI(aud_modlist, 0x5a7, "connect %08x.%d -> %d.%d\n", uid_out, port_out, uid_in, port_in);
    out = owner_inst_by_uid(o, uid_out);
    in = owner_inst_by_uid(o, uid_in);
    if (!out || !in) {
        LOGE(aud_modlist, 0x5ac, "no inst\n");
        return -22;
    }
    return aud_modlist_connect_inst(o, out, port_out, in, port_in, sync);
}

static struct port *find_port(struct link *root, int index)
{
    struct link *l;

    for_each_link(l, root) {
        struct port *p = link_container(struct port, port_list, l);

        if (p->index == index)
            return p;
    }
    return NULL;
}

/* @0x19080: destroy a gluer/gluew instance created for a jumper */
void destroy_glue(struct aud_mod_inst *glue)
{
    struct aud_mod_impl *impl = glue->mod_impl;
    struct link *l;

    LOGD(aud_modlist, 0x450, "destroy glue %08x\n", glue->uid);
    if (!link_empty(&glue->input_ports)) {
        LOGE(aud_modlist, 0x457, "%s: input port still active. disconnect.\n", glue->name);
        for_each_link(l, &glue->input_ports)
            LOGE(aud_modlist, 0x459, "%s: input%d\n", glue->name,
                 link_container(struct port, port_list, l)->index);
    }
    if (!link_empty(&glue->output_ports)) {
        LOGE(aud_modlist, 0x45f, "%s: output port still active. disconnect.\n", glue->name);
        for_each_link(l, &glue->output_ports)
            LOGE(aud_modlist, 0x461, "%s: output%d\n", glue->name,
                 link_container(struct port, port_list, l)->index);
    }
    if (impl) {
        unsigned int uid = glue->uid;

        /* registry command "destroy module" to the glue's processor handler */
        aimc_send_command(gimc, 0x80f10004, 0, impl->processor->proc_id << 24 | 1, &uid, 4);
        impl->alloc_count--;
        remove_from_processor(glue);
    }
    free(glue);
}

/* @0x19220: wire out.port_out -> in.port_in on the same processor; the actual connect is
 * done by that processor's connector (cmd 0xf1000010) */
int connect_in_processor(struct aud_mod_inst *out, int port_out, struct aud_mod_inst *in,
                         int port_in)
{
    struct port *pin, *pout;
    struct {
        unsigned int uid_out;
        int port_out;
        unsigned int uid_in;
        int port_in;
    } param;

    LOGI(aud_modlist, 0x3c3, "connect %s:%d -> %s:%d\n", out->name, port_out, in->name, port_in);
    if (find_port(&in->input_ports, port_in)) {
        LOGE(aud_modlist, 0x3c8, "%s: already have input port. %d\n", in->name, port_in);
        return -22;
    }
    pin = calloc(1, sizeof(*pin));
    if (!pin) {
        LOGE(aud_modlist, 0x3cf, "no mem\n");
        return -12;
    }
    pin->inst = in;
    pin->index = port_in;

    pout = find_port(&out->output_ports, port_out);
    if (!pout) {
        pout = calloc(1, sizeof(*pout));
        if (!pout) {
            LOGE(aud_modlist, 0x3dc, "no mem\n");
            free(pin);
            return -12;
        }
        LINK_INIT(&pout->output_jumpers);
        pout->inst = out;
        pout->index = port_out;
        LINK_INIT(&pout->down_peers);
        link_add_tail(&out->output_ports, &pout->port_list);
    }
    link_add_tail(&in->input_ports, &pin->port_list);
    pin->up_peer = pout;
    link_add_tail(&pout->down_peers, &pin->down_peer_list);

    if (!out->mod_impl) {
        LOGE(aud_modlist, 0x3f4, "mod_impl is null\n");
        return -22;
    }
    if (!out->mod_impl->processor) {
        LOGE(aud_modlist, 0x3fa, "processor is null\n");
        return -22;
    }
    param.uid_out = out->uid;
    param.port_out = port_out;
    param.uid_in = in->uid;
    param.port_in = port_in;
    LOGI(aud_modlist, 0x403, "connect by connector..\n");
    aimc_send_command(gimc, 0xf1000010, 0, out->mod_impl->processor->proc_id << 24 | 2, &param,
                      sizeof(param));
    return 0;
}

struct link all_jumpers = LINK_INITIALIZER(all_jumpers);           /* @0x4c048 */
static unsigned long long hma_base;                                 /* @0x51168 */

/* processors 2 and 3 are the DSPs (kernel uimc ids, ADEC_CORE_DSP0/DSP1 numbering) */
static int proc_is_dsp(struct aud_mod_inst *inst)
{
    return (unsigned int)(inst->mod_impl->processor->proc_id - 2) < 2;
}

/* @0x1acec: connect two instances. Same processor -> connect_in_processor. Different
 * processors -> build a "jumper": gluew on the producer side, gluer on the consumer side,
 * a 256 KiB shared buffer between them (libhma physical memory if either side is a DSP,
 * POSIX shm "shm:<out>-<in>" otherwise). */
int aud_modlist_connect_inst(struct aud_mod_owner *o, struct aud_mod_inst *out, int port_out,
                             struct aud_mod_inst *in, int port_in, bool sync)
{
    struct jumper *j;
    struct port *pin, *pout;
    int ret;

    (void)o;
    if (out == in) {
        LOGE(aud_modlist, 0x64e, "connect same instance. %s\n", out->name);
        return -22;
    }
    pthread_mutex_lock(&out->lock);
    pthread_mutex_lock(&in->lock);
    LOGI(aud_modlist, 0x655, "connect %s.%d -> %s.%d, sync%d\n", out->name, port_out, in->name,
         port_in, sync);

    if (((unsigned char *)&out->uid)[3] == ((unsigned char *)&in->uid)[3]) {
        ret = connect_in_processor(out, port_out, in, port_in);
        goto unlock;
    }

    /* connect_between_processor */
    LOGI(aud_modlist, 0x47b, "connect between %s.%d -> %s.%d\n", out->name, port_out, in->name,
         port_in);
    j = calloc(1, sizeof(*j));
    if (!j) {
        LOGE(aud_modlist, 0x480, "no mem\n");
        ret = -1;
        goto unlock;
    }
    j->output_glue = aud_modlist_create_inst(NULL, "gluew", ((unsigned char *)&out->uid)[3], false);
    j->input_glue = aud_modlist_create_inst(NULL, "gluer", ((unsigned char *)&in->uid)[3], false);
    if (!j->output_glue || !j->input_glue) {
        LOGE(aud_modlist, 0x488, "no glue. %p, %p\n", j->output_glue, j->input_glue);
        if (j->output_glue)
            destroy_glue(j->output_glue);
        if (j->input_glue)
            destroy_glue(j->input_glue);
        goto fail;
    }

    /* initialize_shbuf */
    if (proc_is_dsp(out) || proc_is_dsp(in)) {
        j->dsp_shared = libhma_alloc("audio_buffer", JUMPER_SHBUF_SIZE, 0x1000, 0);
        if (!j->dsp_shared) {
            LOGE(aud_modlist, 0x42d, "no mem %d\n", JUMPER_SHBUF_SIZE);
            LOGE(aud_modlist, 0x492, "failed..\n");
            goto fail_glue;
        }
        if (!hma_base) {
            unsigned long long base;
            unsigned int size;

            if (libhma_get_info("audio_buffer", &base, &size) >= 0)
                hma_base = base;
        }
        j->offset = (unsigned int)j->dsp_shared + 0x10000000 - (unsigned int)hma_base;
        LOGI(aud_modlist, 0x432, "allocate physical shared buffer. dsp_shared %pap, offset %x\n",
             (void *)j, j->offset);
        /* [observed] no properties are set for the DSP case (0x1adb4..0x1ae10); the DSP
         * glues get their buffer from GLUE_CMD_SET_SHARED_BUFFER instead */
    } else {
        char *name = NULL;

        if (asprintf(&name, "shm:%s-%s", out->name, in->name) < 0)
            LOGE(aud_modlist, 0x43b, "Oops\n");
        if (name) {
            aimc_set_property(gimc, j->output_glue->uid,
                              "cowork-uid=0x%08x\\shm-name=%s\\shm-size=%d",
                              j->input_glue->uid, name, JUMPER_SHBUF_SIZE);
            aimc_set_property(gimc, j->input_glue->uid, "cowork-uid=0x%08x\\shm-name=%s",
                              j->output_glue->uid, name);
            j->shm_name = name;
        }
    }

    if (connect_in_processor(j->input_glue, 0, in, port_in) < 0) {
        LOGE(aud_modlist, 0x498, "failed..\n");
        goto fail_glue;
    }
    pin = find_port(&in->input_ports, port_in);
    if (connect_in_processor(out, port_out, j->output_glue, 0) < 0) {
        LOGE(aud_modlist, 0x49f, "failed..\n");
        disconnect_in_processor(j->input_glue, pin->up_peer, in, pin);
        goto fail_glue;
    }
    pout = find_port(&out->output_ports, port_out);
    pin->input_jumper = j;
    /* the binary has no NULL check for pout here (it traps) */
    link_add_tail(&pout->output_jumpers, &j->jumper_list);
    j->output_port = pout;
    j->input_port = pin;
    link_add_tail(&all_jumpers, &j->all_jumpers_list);
    ret = 0;
    goto unlock;

fail_glue:
    destroy_glue(j->input_glue);
    destroy_glue(j->output_glue);
fail:
    free(j);
    ret = -1;
unlock:
    pthread_mutex_unlock(&in->lock);
    pthread_mutex_unlock(&out->lock);
    return ret;
}

/* @0x198b4: undo connect_in_processor: tell the processor's connector (cmd 0xf1010010),
 * free the input port record and, once it has no peers left, the output port record */
void disconnect_in_processor(struct aud_mod_inst *out, struct port *port_out,
                             struct aud_mod_inst *in, struct port *port_in)
{
    struct {
        unsigned int uid_out;
        int port_out;
        unsigned int uid_in;
        int port_in;
    } param;

    if (!port_out || !port_in) {
        LOGE(aud_modlist, 0x393, "Oops\n");
        if (!port_out)
            LOGE(aud_modlist, 0x395, "port_out is null\n");
        if (!port_in)
            LOGE(aud_modlist, 0x397, "port_in is null\n");
        return;
    }
    LOGI(aud_modlist, 0x39b, "disconnect %s:%d -> %s:%d\n", out->name, port_out->index,
         in->name, port_in->index);
    pthread_mutex_lock(&out->lock);
    if (out->mod_impl) {
        param.uid_out = out->uid;
        param.port_out = port_out->index;
        param.uid_in = in->uid;
        param.port_in = port_in->index;
        aimc_send_command(gimc, 0xf1010010, 0, out->mod_impl->processor->proc_id << 24 | 2,
                          &param, sizeof(param));
    }
    pthread_mutex_unlock(&out->lock);
    link_del(&port_in->down_peer_list);
    link_del(&port_in->port_list);
    free(port_in);
    if (link_empty(&port_out->down_peers)) {
        link_del(&port_out->port_list);
        free(port_out);
    }
}

/* @0x19a30: tear down a cross-processor jumper: both in-processor links, both glues, a
 * sync round trip to both registries, then the shared buffer */
void destroy_jumper(struct jumper *j)
{
    struct port *pin = j->input_port, *pout = j->output_port;
    struct aud_mod_inst *in = pin->inst, *out = pout->inst;

    LOGI(aud_modlist, 0x4bf, "destroy jumper between %s:%d -> %s:%d\n", out->name,
         pout->index, in->name, pin->index);
    link_del(&j->jumper_list);
    if (j->input_glue)
        disconnect_in_processor(j->input_glue, pin->up_peer, in, pin);
    else
        LOGE(aud_modlist, 0x4cb, "Oops??\n");
    if (j->output_glue)
        disconnect_in_processor(out, pout, j->output_glue,
                                find_port(&j->output_glue->input_ports, 0));
    else
        LOGE(aud_modlist, 0x4d4, "Oops??\n");
    if (j->input_glue)
        destroy_glue(j->input_glue);
    if (j->output_glue)
        destroy_glue(j->output_glue);
    /* ADEC_CMD_SYNC to both registry handlers (proc << 24 | 1) */
    aimc_send_command_sync(gimc, 0x101a0000, 0, (in->uid & 0xff000000) | 1, NULL, 0);
    aimc_send_command_sync(gimc, 0x101a0000, 0, (out->uid & 0xff000000) | 1, NULL, 0);
    if (j->dsp_shared)
        libhma_free((unsigned long)j->dsp_shared);
    if (j->shm_name) {
        shm_unlink(j->shm_name);
        free(j->shm_name);
    }
    link_del(&j->all_jumpers_list);
    free(j);
}

/* @0x1a528: drop every connection of output `port`: first its jumpers, then the
 * in-processor peers. Each step can free the port record, so it is looked up again. */
int aud_modlist_disconnect_inst_output(struct aud_mod_owner *o, struct aud_mod_inst *inst,
                                       int port)
{
    struct port *p;
    int ret = 0;

    (void)o;
    pthread_mutex_lock(&connection_lock);
    LOGI(aud_modlist, 0x53f, "disconnect output%d of %s\n", port, inst->name);
    p = find_port(&inst->output_ports, port);
    if (!p) {
        LOGE(aud_modlist, 0x543, "no such output port. %s:%d\n", inst->name, port);
        ret = -22;
        goto out;
    }
    while (p && !link_empty(&p->output_jumpers)) {
        destroy_jumper(link_first(struct jumper, jumper_list, &p->output_jumpers));
        p = find_port(&inst->output_ports, port);
    }
    while (p && !link_empty(&p->down_peers)) {
        struct port *pin = link_first(struct port, down_peer_list, &p->down_peers);

        disconnect_in_processor(inst, p, pin->inst, pin);
        p = find_port(&inst->output_ports, port);
    }
out:
    pthread_mutex_unlock(&connection_lock);
    return ret;
}

/* @0x1a66c */
int aud_modlist_disconnect_inst_input(struct aud_mod_owner *o, struct aud_mod_inst *inst,
                                      int port)
{
    struct port *p;
    int ret = 0;

    (void)o;
    pthread_mutex_lock(&connection_lock);
    LOGI(aud_modlist, 0x571, "disconnect input%d of %s\n", port, inst->name);
    p = find_port(&inst->input_ports, port);
    if (!p) {
        LOGE(aud_modlist, 0x575, "no such input port. %s:%d\n", inst->name, port);
        ret = -22;
    } else if (p->input_jumper) {
        destroy_jumper(p->input_jumper);
    } else if (p->up_peer) {
        disconnect_in_processor(p->up_peer->inst, p->up_peer, inst, p);
    }
    pthread_mutex_unlock(&connection_lock);
    return ret;
}

/* ========================================================================= */
/* os wrapper: threads and clocks                                            */
/* ========================================================================= */

/* @0x20b28: thread trampoline (sets the thread name) */
void *aud_thread_main(void *arg)
{
    struct aud_thread *t = arg;

    prctl(PR_SET_NAME, t->name, 0, 0, 0);
    t->func(t->arg);
    return NULL;
}

/* @0x20b4c: CPU time of the calling thread, ns */
long long aud_thread_cycle(void)
{
    clockid_t cid;
    struct timespec ts = { 0, 0 };
    int err;

    err = pthread_getcpuclockid(pthread_self(), &cid);
    if (err < 0) {
        LOGE(oswrap, 0x29, "cannot get cpuclockid. %d(%s)\n", err, strerror(err));
        return 0;
    }
    if (clock_gettime(cid, &ts) < 0) {
        err = errno;
        LOGE(oswrap, 0x2f, "cannot get time. %d(%s)\n", err, strerror(err));
        return 0;
    }
    return (long long)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

/* @0x20c08: CLOCK_MONOTONIC in ns */
unsigned long long aud_system_cycle(void)
{
    struct timespec ts = { 0, 0 };

    if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0) {
        int err = errno;

        LOGE(oswrap, 0x3c, "cannot get time. %d(%s)\n", err, strerror(err));
        return 0;
    }
    return (long long)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

/* @0x20c88: stack_size and priority are accepted but ignored (default attrs) */
struct aud_thread *aud_thread_create(const char *name, int stack_size, int priority,
                                     int (*fn)(void *arg), void *arg)
{
    size_t len = strlen(name);
    struct aud_thread *t = calloc(1, sizeof(*t) + len + 1);

    (void)stack_size;
    (void)priority;
    if (!t) {
        LOGE(oswrap, 0x55, "no mem\n");
        return NULL;
    }
    memcpy(t->name, name, len + 1);
    t->func = fn;
    t->arg = arg;
    pthread_create(&t->thread, NULL, aud_thread_main, t);
    return t;
}

/* @0x20d04 */
void aud_thread_destroy(struct aud_thread *t)
{
    pthread_join(t->thread, NULL);
    free(t);
}

/* @0x20d1c: timed wait on CLOCK_REALTIME */
void aud_cond_wait_timeout(pthread_cond_t *cond, pthread_mutex_t *mutex, int usec)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec += usec * 1000;
    if (ts.tv_nsec > 1000000000) {
        ts.tv_nsec -= 1000000000;
        ts.tv_sec++;
        if (ts.tv_nsec > 1000000000) {
            ts.tv_nsec -= 1000000000;
            ts.tv_sec++;
        }
    }
    pthread_cond_timedwait(cond, mutex, &ts);
}

/* ========================================================================= */
/* aud_link: locked lists with per-node locks                                */
/* ========================================================================= */

/* @0x233ec: snapshot the node ids, then visit each still-present node with its lock held.
 * Entered with root->rlock held; returns with it released. */
int _foreach_node(struct aud_link_root *root, void *arg, int (*cb)(void *arg, void *data),
                  int *ids)
{
    struct link *l;
    unsigned int n = 0, i;

    for_each_link(l, &root->nodes)
        ids[n++] = link_container(struct aud_link_node, root_link, l)->id;
    if (n != root->node_count) {
        LOGE(com_link, 0x93, "?? %d != %d\n", n, root->node_count);
        pthread_mutex_unlock(&root->rlock);
        return -1;
    }
    if (n == 0) {
        pthread_mutex_unlock(&root->rlock);
        return 0;
    }
    pthread_mutex_unlock(&root->rlock);
    for (i = 0; i < n; i++) {
        struct aud_link_node *found = NULL;

        pthread_mutex_lock(&root->rlock);
        for_each_link(l, &root->nodes) {
            struct aud_link_node *node = link_container(struct aud_link_node, root_link, l);

            if (node->id == ids[i]) {
                found = node;
                break;
            }
        }
        if (!found) {
            pthread_mutex_unlock(&root->rlock);
            continue;
        }
        pthread_mutex_lock(&found->lock);
        pthread_mutex_unlock(&root->rlock);
        /* cb must unlock found->lock */
        if (cb(arg, found->data))
            return 0;
    }
    return 0;
}

/* @0x234e4 */
void aud_link_root_init(struct aud_link_root *root)
{
    LINK_INIT(&root->nodes);
    pthread_mutex_init(&root->rlock, NULL);
}

/* @0x234f4 */
void aud_link_node_init(struct aud_link_node *node, void *data)
{
    pthread_mutex_init(&node->lock, NULL);
    node->root = NULL;
    node->data = data;
}

/* @0x2350c: insert at head; ids are allocated per root */
int aud_link_add_locked(struct aud_link_root *root, struct aud_link_node *node)
{
    link_add(&root->nodes, &node->root_link);
    node->root = root;
    node->id = ++root->last_id;
    root->node_count++;
    return 0;
}

/* @0x2353c */
int aud_link_add(struct aud_link_root *root, struct aud_link_node *node)
{
    pthread_mutex_lock(&root->rlock);
    aud_link_add_locked(root, node);
    pthread_mutex_unlock(&root->rlock);
    return 0;
}

/* @0x23560 */
int aud_link_add_tail_locked(struct aud_link_root *root, struct aud_link_node *node)
{
    link_add_tail(&root->nodes, &node->root_link);
    node->root = root;
    node->id = ++root->last_id;
    root->node_count++;
    return 0;
}

/* @0x2358c */
int aud_link_add_tail(struct aud_link_root *root, struct aud_link_node *node)
{
    pthread_mutex_lock(&root->rlock);
    aud_link_add_tail_locked(root, node);
    pthread_mutex_unlock(&root->rlock);
    return 0;
}

/* @0x235b0 */
int aud_link_del_locked(struct aud_link_node *node)
{
    if (node->root_link.prev) {
        node->root_link.prev->next = node->root_link.next;
        node->root_link.next->prev = node->root_link.prev;
    }
    node->root->node_count--;
    node->root = NULL;
    return 0;
}

/* @0x235cc */
int aud_link_del(struct aud_link_node *node)
{
    pthread_mutex_t *m = &node->root->rlock;

    pthread_mutex_lock(m);
    aud_link_del_locked(node);
    pthread_mutex_unlock(m);
    return 0;
}

/* @0x235ec: id snapshot on the stack for <= 256 nodes, heap otherwise */
int aud_link_foreach_node(struct aud_link_root *root, void *arg,
                          int (*cb)(void *arg, void *node_data))
{
    unsigned int n;
    int *ids, ret;

    pthread_mutex_lock(&root->rlock);
    n = root->node_count;
    if (n <= 0x100) {
        int stack_ids[n ? n : 1];

        return _foreach_node(root, arg, cb, stack_ids);
    }
    ids = malloc(n * sizeof(*ids));
    if (!ids) {
        LOGE(com_link, 0xb6, "no mem\n");
        pthread_mutex_unlock(&root->rlock);
        return -1;
    }
    ret = _foreach_node(root, arg, cb, ids);
    free(ids);
    return ret;
}

/* ========================================================================= */
/* object classes                                                            */
/* ========================================================================= */

struct link class_desc_root = LINK_INITIALIZER(class_desc_root);     /* @0x510c4 */
LOGMM_MODULE_DEFINE(aud_obj);                                         /* @0x510cc */

/* root of the class tree; never registered itself (@0x510e0) */
struct aud_object_class_desc __aud_object_class_base = {
    .name = "base",
};

/* @0x30864: debug_object "class" dump callback */
int dump_class(void *priv, struct aud_object_class *klass)
{
    struct aud_object_class_desc *d = klass->desc, *p;
    int depth = 0;

    (void)priv;
    logm_message("class %s\n", d->name);
    for (p = d->parent; p; p = p->parent) {
        depth += 2;
        logm_message("%*s<- inherit %s\n", depth, "", p->name);
    }
    return 0;
}

/* @0x308a4: run class_init of every ancestor first, then our own */
void init_class(struct aud_object_class *klass, struct aud_object_class_desc *desc)
{
    if (desc->parent)
        init_class(klass, desc->parent);
    LOGI(aud_obj, 0x25, "init class %s\n", desc->name);
    if (desc->class_init)
        desc->class_init(klass);
}

/* @0x30904: append to class_desc_root */
int aud_object_class_register(struct aud_object_class_desc *desc)
{
    link_add_tail(&class_desc_root, &desc->list);
    return 0;
}

/* @0x30928: lazily instantiate the class object */
struct aud_object_class *aud_object_class_get(struct aud_object_class_desc *desc)
{
    struct aud_object_class *klass;

    if (desc->object_class)
        return desc->object_class;
    LOGI(aud_obj, 0x31, "initialize class %s\n", desc->name);
    klass = calloc(1, desc->sizeof_class);
    desc->object_class = klass;
    if (!klass) {
        LOGE(aud_obj, 0x35, "no mem. %d for %s\n", desc->sizeof_class, desc->name);
        return NULL;
    }
    klass->desc = desc;
    init_class(klass, desc);
    return desc->object_class;
}

/* @0x309c4 */
struct aud_object_class *aud_object_class_gets(const char *name)
{
    struct link *l;

    for_each_link(l, &class_desc_root) {
        struct aud_object_class_desc *d = link_container(struct aud_object_class_desc, list, l);

        if (!strcmp(d->name, name))
            return aud_object_class_get(d);
    }
    LOGE(aud_obj, 0x4f, "no such class desc. %s\n", name);
    return NULL;
}

/* @0x30a38: stops at the first negative return */
int aud_object_class_foreach(int (*each)(void *priv, struct aud_object_class *klass), void *priv)
{
    struct link *l;
    int ret = 0;

    for_each_link(l, &class_desc_root) {
        struct aud_object_class_desc *d = link_container(struct aud_object_class_desc, list, l);

        ret = each(priv, aud_object_class_get(d));
        if (ret < 0)
            return ret;
    }
    return ret;
}

/* ========================================================================= */
/* starter: pulls AUs out of a legacy/inter buffer and pushes them to ports  */
/* ========================================================================= */

/* starter.c process_event (inlined): an AU of type 4 holds a serialized event, possibly
 * split across the ring wrap */
static void starter_process_event(struct aud_starter *s, int size)
{
    unsigned char *mem = NULL, *chunk;
    struct aud_event *e;
    int done = 0, n;
    struct link *l;

    for (;;) {
        chunk = NULL;
        n = size - done;
        if (s->lbuf->read_data(&chunk, &n, NULL, s->lbuf)) {
            LOGE(modstart, 0x53, "starter%p, %s: cannot read data. %d\n", s, s->name, size);
            return;
        }
        if (n == size) {
            e = aud_event_deserialize(chunk, size);
            break;
        }
        if (!mem)
            mem = malloc(size);
        memcpy(mem + done, chunk, n);
        if (s->lbuf->flush_data(n, s->lbuf))
            LOGE(modstart, 0x60, "starter%p, %s: cannot flush data. %d\n", s, s->name, n);
        done += n;
        if (done >= size) {
            e = aud_event_deserialize(mem, size);
            break;
        }
    }
    if (mem)
        free(mem);
    else if (s->lbuf->flush_data(size, s->lbuf))
        LOGE(modstart, 0x6e, "starter%p, %s: cannot flush data. %d\n", s, s->name, size);
    if (!e) {
        LOGE(modstart, 0x73, "starter%p, %s: no event\n", s, s->name);
        return;
    }
    /* events go to the modules that own the ports, not down the ports */
    pthread_mutex_lock(&s->ports_lock);
    for_each_link(l, &s->ports) {
        struct aud_port *port = link_container(struct aud_port, starter_link, l);

        aud_module_event(port->module, NULL,
                         (struct aud_event *)aud_object_get(&e->object));
    }
    pthread_mutex_unlock(&s->ports_lock);
    aud_object_put(&e->object);
}

/* starter.c split_push_data (inlined): unpack a type_mixed AU into its parts */
static void starter_split_push(struct aud_starter *s, struct aud_port *port, struct aud_data *d)
{
    unsigned int offs = 0, next = 8;

    while (next <= d->size) {
        unsigned int type = *(unsigned int *)(d->data + offs);
        unsigned int size = *(unsigned int *)(d->data + offs + 4);
        struct aud_data *part;

        if (next + size > d->size) {
            LOGE(modstart, 0x93, "starter%p, %s: wrong mixed data size. type %d, size %d\n", s,
                 s->name, type, size);
            return;
        }
        part = aud_data_region(d, next, size);
        part->info.type = type;
        aud_port_push_data(port, part);
        offs = next + ((part->size + 7) & ~7u);
        next = offs + 8;
    }
    if (d->size != offs)
        LOGE(modstart, 0xa0, "starter%p, %s: wrong mixed data size. offs %d, size %d\n", s,
             s->name, offs, d->size);
}

/* starter.c process_data (inlined) */
static void starter_process_data(struct aud_starter *s, ADEC_AU_INFO_T *aui)
{
    struct aud_data *d = aud_data_new(aui->size);
    unsigned int done;
    struct link *l;

    if (!d) {
        LOGE(modstart, 0xae, "starter%p, %s: cannot allocate data.\n", s, s->name);
        return;
    }
    switch (aui->dataType) {
    case 0: case 1: case 2: case 3:
        d->info.type = aui->dataType;
        break;
    case 5:
        d->info.type = type_mixed;
        break;
    default:
        LOGE(modstart, 0xc3, "starter%p, %s: unknown data type(%d). set default PCM type\n", s,
             s->name, aui->dataType);
        d->info.type = type_pcm;
        break;
    }
    d->info.index = aui->index;
    d->info.timestamp = aui->timestamp;
    d->info.timestamp64 = aui->timestamp64;

    for (done = 0; done < aui->size;) {
        unsigned char *chunk = NULL;
        int n = aui->size - done;

        if (s->lbuf->read_data(&chunk, &n, NULL, s->lbuf)) {
            LOGE(modstart, 0xd5, "starter%p, %s: cannot read data. %d\n", s, s->name, aui->size);
            aud_object_put(&d->object);
            return;
        }
        LOGD(modstart, 0xda, "starter%p, %s: got %d\n", s, s->name, n);
        memcpy(d->data + done, chunk, n);
        if (s->lbuf->flush_data(n, s->lbuf))
            LOGE(modstart, 0xde, "starter%p, %s: cannot flush data. %d\n", s, s->name, n);
        done += n;
    }

    /* every registered port gets the same AU; sync after each push */
    pthread_mutex_lock(&s->ports_lock);
    for_each_link(l, &s->ports) {
        struct aud_port *port = link_container(struct aud_port, starter_link, l);

        if (d->info.type == type_mixed && s->split_meta_pcm)
            starter_split_push(s, port, d);
        else
            aud_port_push_data(port, (struct aud_data *)aud_object_get(&d->object));
        aud_port_sync(port);
    }
    pthread_mutex_unlock(&s->ports_lock);
    aud_object_put(&d->object);
}

/* @0x30abc: starter_thread. Sleeps until the buffer holds at least one AU (or stop),
 * then handles exactly one AU per wake-up loop iteration. */
int starter_thread(void *param)
{
    struct aud_starter *s = param;

    for (;;) {
        ADEC_AU_INFO_T aui;
        int offset;

        pthread_mutex_lock(&s->thread_mutex);
        while (!s->stop && s->lbuf->get_au_cnt(s->lbuf) <= 0)
            pthread_cond_wait(&s->thread_cond, &s->thread_mutex);
        pthread_mutex_unlock(&s->thread_mutex);
        if (s->stop) {
            LOGI(modstart, 0x121, "starter%p, %s: stop\n", s, s->name);
            return 0;
        }

        aui.size = 0;
        if (s->lbuf->get_current_info(&offset, &aui, s->lbuf)) {
            LOGE(modstart, 0xfb, "starter%p, %s: cannot get AU info\n", s, s->name);
            continue;
        }
        if (offset)
            LOGE(modstart, 0x100, "starter%p, au splited. offset %d\n", s, offset);
        LOGD(modstart, 0x102, "starter%p, %s: buffer type %d, size %d\n", s, s->name,
             aui.dataType, aui.size);

        if (aui.size == 0) {
            LOGI(modstart, 0x10c, "starter%p, %s: zero sized au.. type %d, flags 0x%08x\n", s,
                 s->name, aui.dataType, aui.flags);
            s->lbuf->flush_data(0, s->lbuf);
        } else if (aui.dataType == 4) {
            starter_process_event(s, aui.size);
        } else {
            starter_process_data(s, &aui);
        }
        LOGD(modstart, 0x10f, "starter%p, %s: process done.\n", s, s->name);
    }
}

/* @0x311a8 */
struct aud_starter *aud_starter_new(struct _ADEC_BUF_T *lbuf, const char *name_fmt, ...)
{
    struct aud_starter *s = calloc(1, sizeof(*s));
    va_list ap;

    if (!s) {
        LOGE(modstart, 0x133, "starter%p, no mem\n", NULL);
        return NULL;
    }
    s->split_meta_pcm = true;
    LINK_INIT(&s->ports);
    s->lbuf = lbuf;
    va_start(ap, name_fmt);
    s->name = audutil_strdup_vprintf(name_fmt, ap);
    va_end(ap);
    if (!s->name) {
        LOGE(modstart, 0x141, "starter%p, no name\n", s);
        free(s);
        return NULL;
    }
    pthread_cond_init(&s->thread_cond, NULL);
    pthread_mutex_init(&s->thread_mutex, NULL);
    pthread_mutex_init(&s->ports_lock, NULL);
    s->thread = aud_thread_create(s->name, 0x20000, 5, starter_thread, s);
    LOGI(modstart, 0x14b, "starter%p, %s: new starter.\n", s, s->name);
    return s;
}

/* @0x312c4: wake the thread (new data was written by the peer) */
void aud_starter_trigger(struct aud_starter *s)
{
    pthread_mutex_lock(&s->thread_mutex);
    LOGD(modstart, 0x153, "starter%p, %s: trigger..\n", s, s->name);
    pthread_cond_broadcast(&s->thread_cond);
    pthread_mutex_unlock(&s->thread_mutex);
}

/* @0x31328 */
void aud_starter_stop(struct aud_starter *s)
{
    pthread_mutex_lock(&s->thread_mutex);
    LOGI(modstart, 0x15c, "starter%p, %s: stop starter.\n", s, s->name);
    s->stop = 1;
    pthread_cond_broadcast(&s->thread_cond);
    pthread_mutex_unlock(&s->thread_mutex);
}

/* @0x31390 (deprecated) */
void aud_starter_add(struct aud_starter *s, struct link *port_link)
{
    pthread_mutex_lock(&s->ports_lock);
    link_add_tail(&s->ports, port_link);
    pthread_mutex_unlock(&s->ports_lock);
}

/* @0x313bc */
void aud_starter_add_port(struct aud_starter *s, struct aud_port *port)
{
    LOGI(modstart, 0x16b, "starter%p, %s: add port. %p\n", s, s->name, port);
    pthread_mutex_lock(&s->ports_lock);
    link_add_tail(&s->ports, &port->starter_link);
    port->starter = s;
    pthread_mutex_unlock(&s->ports_lock);
}

/* @0x31434 */
void aud_starter_del_port(struct aud_starter *s, struct aud_port *port)
{
    pthread_mutex_lock(&s->ports_lock);
    LOGI(modstart, 0x176, "starter%p, %s: del port. %p\n", s, s->name, port);
    link_del(&port->starter_link);
    port->starter = NULL;
    pthread_mutex_unlock(&s->ports_lock);
}

/* @0x314a8 */
struct _ADEC_BUF_T *aud_starter_get_lbuf(struct aud_starter *s)
{
    return s->lbuf;
}

/* @0x314ac: caller must have stopped the thread (aud_starter_stop) first */
void aud_starter_del(struct aud_starter *s)
{
    struct link *l;

    LOGD(modstart, 0x185, "starter%p, %s: delete starter.\n", s, s->name);
    pthread_mutex_lock(&s->ports_lock);
    for_each_link(l, &s->ports)
        link_container(struct aud_port, starter_link, l)->starter = NULL;
    pthread_mutex_unlock(&s->ports_lock);
    aud_thread_destroy(s->thread);
    LOGI(modstart, 399, "starter%p, %s: thread deleted.\n", s, s->name);
    free(s->name);
    free(s);
}
