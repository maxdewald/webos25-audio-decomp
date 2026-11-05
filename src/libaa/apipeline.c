/*
 * libaa user-side pipeline builder ("apipeline"): an ordered list of module instances
 * created through aimc (on whichever processor/DSP owns the implementation) and
 * connected output0 -> input0 as elements are appended/prepended.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 * Evidence: build/decomp/libaa.so.2.0.0.c @0x1b6bc..0x1bd2c, logmm module "aud_pipeline",
 * strings "addfv", "_apipeline_add", "add \"%s\" at %s, pipe \"%s\"\n".
 *
 * Processor placement (apipeline_appendfv_port / prependfv_port / addfv):
 *   - first element of an empty pipe: aimc_module_create(imc, name, -1) = any processor
 *     (the registry/acont picks the highest-rank implementation),
 *   - every further element: preferred processor = uid >> 24 of the current tail (append)
 *     or head (prepend), i.e. "stay on the DSP of the neighbour"; if that fails, retry
 *     with -1 (any processor). An implementation only on another processor therefore
 *     still gets created, and aimc_module_connect() then inserts the cross-processor
 *     glue (jumper) itself.
 */
#define _GNU_SOURCE
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aimc_local.h"   /* aipc: struct aipc_ipc_arg, aipc_* */
#include "objects.h"      /* struct port */

LOGMM_MODULE_DEFINE(acont);
LOGMM_MODULE_DEFINE(aud_modlist);
LOGMM_MODULE_DEFINE(aud_pipeline);

/* @0x1b6bc */
struct apipeline *apipeline_new(struct aimc *imc, const char *name, ...)
{
    struct apipeline *pipe = calloc(1, sizeof(*pipe));
    va_list ap;

    if (!pipe) {
        LOGE(aud_pipeline, 0x26, "no mem\n");
        return NULL;
    }
    va_start(ap, name);
    if (vasprintf(&pipe->name, name, ap) < 0) {
        va_end(ap);
        LOGE(aud_pipeline, 0x2f, "no mem\n");
        free(pipe);
        return NULL;
    }
    va_end(ap);
    pipe->imc = imc;
    LINK_INIT(&pipe->element_list);
    return pipe;
}

/* @0x1b75c: destroys every module instance of the pipe (head to tail) */
void apipeline_del(struct apipeline *pipe)
{
    struct link *l, *next;

    for (l = pipe->element_list.next, next = l->next; l != &pipe->element_list;
         l = next, next = l->next) {
        struct apipeline_element *el = link_container(struct apipeline_element, list, l);

        aimc_module_destroy(pipe->imc, el->uid);
        link_del(&el->list);
        free(el->name);
        free(el);
    }
    free(pipe->name);
    free(pipe);
}

/* @0x1b7a4: search by alias (set with "name=" in an arocket description) */
struct apipeline_element *apipeline_search(struct apipeline *pipe, const char *name)
{
    struct link *l;

    for_each_link(l, &pipe->element_list) {
        struct apipeline_element *el = link_container(struct apipeline_element, list, l);

        if (el->alias && !strcmp(el->alias, name))
            return el;
    }
    return NULL;
}

/* @0x1b7cc: record an already created instance; no-op if the uid is already in the pipe */
struct apipeline_element *_apipeline_add(struct apipeline *pipe, const char *impl_name,
                                         unsigned int uid, bool at_tail)
{
    struct apipeline_element *el;
    struct link *l;

    for_each_link(l, &pipe->element_list) {
        el = link_container(struct apipeline_element, list, l);
        if (el->uid == uid)
            return el;
    }

    el = calloc(1, sizeof(*el));
    if (!el) {
        LOGE(aud_pipeline, 0x66, "%s: no mem for 0x%08x.\n", pipe->name, uid);
        return NULL;
    }
    if (asprintf(&el->name, "%s_%08x", impl_name, uid) < 0) {
        LOGE(aud_pipeline, 0x6c, "no mem\n");
        free(el);
        return NULL;
    }
    el->uid = uid;
    LINK_INIT(&el->events);
    if (at_tail)
        link_add_tail(&pipe->element_list, &el->list);
    else
        link_add(&pipe->element_list, &el->list);
    LOGD(aud_pipeline, 0x78, "add \"%s\" at %s, pipe \"%s\"\n", el->name,
         at_tail ? "tail" : "head", pipe->name);
    return el;
}

/* @0x1b920: create "<fmt>" on preferred_proc (falling back to any), add it, return uid */
unsigned int addfv(struct apipeline *pipe, int preferred_proc, bool at_tail,
                   const char *fmt, va_list ap)
{
    char *name;
    unsigned int uid;

    if (vasprintf(&name, fmt, ap) < 0) {
        LOGE(aud_pipeline, 0x9c, "%s: no mem\n", pipe->name);
        return 0;
    }
    uid = aimc_module_create(pipe->imc, name, preferred_proc);
    if (!uid) {
        if (preferred_proc == -1) {
            LOGE(aud_pipeline, 0xa5, "can't find module %s", name);
            goto out;
        }
        uid = aimc_module_create(pipe->imc, name, -1);
        if (!uid)
            goto out;
    }
    _apipeline_add(pipe, name, uid, at_tail);
out:
    free(name);
    return uid;
}

/* @0x1b9f0: create on proc (-1 = any) and append without connecting */
struct apipeline_element *apipeline_add_at(struct apipeline *pipe, const char *name, int proc)
{
    unsigned int uid = aimc_module_create(pipe->imc, name, proc);

    if (!uid)
        return NULL;
    return _apipeline_add(pipe, name, uid, true);
}

/* @0x1ba10: try proc, then any processor */
struct apipeline_element *apipeline_add(struct apipeline *pipe, const char *name, int proc)
{
    struct apipeline_element *el = apipeline_add_at(pipe, name, proc);

    if (proc < 0 || el)
        return el;
    return apipeline_add_at(pipe, name, -1);
}

/* @0x1ba38: add without connecting */
unsigned int apipeline_addf(struct apipeline *pipe, const char *fmt, ...)
{
    va_list ap;
    unsigned int uid;

    va_start(ap, fmt);
    uid = addfv(pipe, -1, false, fmt, ap);
    va_end(ap);
    return uid;
}

/* @0x1ba5c: uid of the first (most upstream) element, 0 if empty */
unsigned int apipeline_head(struct apipeline *pipe)
{
    if (link_empty(&pipe->element_list))
        return 0;
    return link_container(struct apipeline_element, list, pipe->element_list.next)->uid;
}

/* @0x1ba6c: uid of the last (most downstream) element, 0 if empty */
unsigned int apipeline_tail(struct apipeline *pipe)
{
    if (link_empty(&pipe->element_list))
        return 0;
    return link_container(struct apipeline_element, list, pipe->element_list.prev)->uid;
}

/* @0x1ba80: tail(out).port_out -> head(in).port_in */
unsigned int apipeline_connect(struct apipeline *pipe_out, int port_out,
                               struct apipeline *pipe_in, int port_in)
{
    unsigned int out = apipeline_tail(pipe_out);
    unsigned int in = apipeline_head(pipe_in);

    if (!out || !in) {
        LOGE(aud_pipeline, 0xeb, "error in get uid of  out(%d) or in(%d)", out, in);
        return -1;
    }
    return aimc_module_connect(pipe_out->imc, out, port_out, in, port_in);
}

/* @0x1bb00: disconnect input `port` of the head (aimc_module_disconnect_input inlined) */
unsigned int apipeline_disconnect(struct apipeline *pipe, int port)
{
    unsigned int head = apipeline_head(pipe);

    if (!head) {
        LOGE(aud_pipeline, 0xfd, "error in get uid (%d)", head);
        return -1;
    }
    return aimc_module_disconnect_input(pipe->imc, head, port);
}

/* @0x1bb64: put an existing instance in front of the pipe: uid_out.port_out -> head.port_in */
unsigned int apipeline_prepend_uid_port(struct apipeline *pipe, int port_in,
                                        unsigned int uid_out, int port_out)
{
    unsigned int head = apipeline_head(pipe);

    if (head)
        aimc_module_connect(pipe->imc, uid_out, port_out, head, port_in);
    _apipeline_add(pipe, "<unknown>", uid_out, false);
    return uid_out;
}

/* @0x1bba0 */
unsigned int apipeline_prepend_uid(struct apipeline *pipe, unsigned int uid_out)
{
    return apipeline_prepend_uid_port(pipe, 0, uid_out, 0);
}

/* @0x1bbac: append an existing instance: tail.port_out -> uid_in.port_in */
unsigned int apipeline_append_uid_port(struct apipeline *pipe, int port_out,
                                       unsigned int uid_in, int port_in)
{
    unsigned int tail = apipeline_tail(pipe);

    if (tail)
        aimc_module_connect(pipe->imc, tail, port_out, uid_in, port_in);
    _apipeline_add(pipe, "<unknown>", uid_in, true);
    return uid_in;
}

/* @0x1bbe8 */
unsigned int apipeline_append_uid(struct apipeline *pipe, unsigned int uid_in)
{
    return apipeline_append_uid_port(pipe, 0, uid_in, 0);
}

/* @0x1bbf4: create new.port_out -> head.port_in, new instance on the head's processor */
unsigned int apipeline_prependfv_port(struct apipeline *pipe, int port_in, int port_out,
                                      const char *fmt, va_list ap)
{
    unsigned int head = apipeline_head(pipe);
    unsigned int uid;

    if (!head)
        return addfv(pipe, -1, false, fmt, ap);
    uid = addfv(pipe, head >> 24, false, fmt, ap);
    aimc_module_connect(pipe->imc, uid, port_out, head, port_in);
    return uid;
}

/* @0x1bc4c */
unsigned int apipeline_prependf_port(struct apipeline *pipe, int port_in, int port_out,
                                     const char *fmt, ...)
{
    va_list ap;
    unsigned int uid;

    va_start(ap, fmt);
    uid = apipeline_prependfv_port(pipe, port_in, port_out, fmt, ap);
    va_end(ap);
    return uid;
}

/* @0x1bc6c */
unsigned int apipeline_prependf(struct apipeline *pipe, const char *fmt, ...)
{
    va_list ap;
    unsigned int uid;

    va_start(ap, fmt);
    uid = apipeline_prependfv_port(pipe, 0, 0, fmt, ap);
    va_end(ap);
    return uid;
}

/* @0x1bc90: create new instance and connect tail.port_out -> new.port_in.
 * The new instance prefers the tail's processor (uid >> 24). The return value of
 * aimc_module_connect() is IGNORED: a failed connect still returns the new uid.
 */
unsigned int apipeline_appendfv_port(struct apipeline *pipe, int port_out, int port_in,
                                     const char *fmt, va_list ap)
{
    unsigned int tail = apipeline_tail(pipe);
    unsigned int uid;

    if (!tail)
        return addfv(pipe, -1, true, fmt, ap);
    uid = addfv(pipe, tail >> 24, true, fmt, ap);
    aimc_module_connect(pipe->imc, tail, port_out, uid, port_in);
    return uid;
}

/* @0x1bce8 */
unsigned int apipeline_appendf_port(struct apipeline *pipe, int port_out, int port_in,
                                    const char *fmt, ...)
{
    va_list ap;
    unsigned int uid;

    va_start(ap, fmt);
    uid = apipeline_appendfv_port(pipe, port_out, port_in, fmt, ap);
    va_end(ap);
    return uid;
}

/* @0x1bd08: append "<impl name printf>" connected out0 -> in0 */
unsigned int apipeline_appendf(struct apipeline *pipe, const char *fmt, ...)
{
    va_list ap;
    unsigned int uid;

    va_start(ap, fmt);
    uid = apipeline_appendfv_port(pipe, 0, 0, fmt, ap);
    va_end(ap);
    return uid;
}

/* @0x1bd2c */
const char *apipeline_get_name(struct apipeline *pipe)
{
    return pipe ? pipe->name : NULL;
}

/* ========================================================================= */
/* element list ("aud_modlist"): the acont-side book-keeping of processors,   */
/* implementations and instances. Evidence @0x18c38..0x1b344.                */
/* ========================================================================= */

#define REGCMD_CREATE_MODULE    0x80f00024
#define REGCMD_DESTROY_MODULE   0x80f10004

struct link processors = LINK_INITIALIZER(processors);              /* @0x4c038 */
struct link helper_list = LINK_INITIALIZER(helper_list);            /* @0x4c040 */
int next_proc_id = 16;                                              /* @0x4c020 */
pthread_mutex_t processors_lock = PTHREAD_MUTEX_INITIALIZER;        /* @0x51130 */
pthread_mutex_t connection_lock = PTHREAD_MUTEX_INITIALIZER;        /* @0x5114c */
struct aimc *gimc;                                                  /* @0x51148 */

#define PROC_OF(l)  link_container(struct aud_proc_info, proc_list, l)
#define IMPL_OF(l)  link_container(struct aud_mod_impl, mods_list, l)
#define INST_OF(l)  link_container(struct aud_mod_inst, impl_list, l)

/* @0x18c38: unlink an instance from its implementation's list */
void remove_from_processor(struct aud_mod_inst *inst)
{
    struct link *pl, *il;

    pthread_mutex_lock(&processors_lock);
    LOGD(aud_modlist, 0x1df, "remove instance, %p, uid %08x\n", inst, inst->uid);
    for_each_link(pl, &processors) {
        struct aud_proc_info *p = PROC_OF(pl);

        if (inst->uid >> 24 != (unsigned int)p->proc_id)
            continue;
        for_each_link(il, &p->mods) {
            struct aud_mod_impl *impl = IMPL_OF(il);

            if (((inst->uid & 0xffffff) >> 16) != (unsigned int)impl->impl_num)
                continue;
            pthread_mutex_lock(&impl->lock);
            if (inst->impl_list.next) {
                link_del(&inst->impl_list);
                inst->impl_list.prev = inst->impl_list.next = NULL;
            }
            pthread_mutex_unlock(&impl->lock);
            LOGI(aud_modlist, 500, "remove instance, %p, from processor %s\n", inst, p->name);
            goto out;
        }
    }
    LOGI(aud_modlist, 0x1fb, "inst, %x, is not linked at anywhere.\n", inst->uid);
out:
    pthread_mutex_unlock(&processors_lock);
}

/* @0x18d60: highest-rank implementation named `name`, on processor `proc` (proc <= 0 = any
 * processor). Returned with impl->lock held. This is where "which DSP" is decided when
 * apipeline asks for preferred processor -1. */
struct aud_mod_impl *search_modimpl(const char *name, int proc)
{
    struct aud_mod_impl *best = NULL;
    int best_rank = (int)0x80000000;
    struct link *pl, *il;

    pthread_mutex_lock(&processors_lock);
    for_each_link(pl, &processors) {
        struct aud_proc_info *p = PROC_OF(pl);

        if (proc > 0 && p->proc_id != proc)
            continue;
        for_each_link(il, &p->mods) {
            struct aud_mod_impl *impl = IMPL_OF(il);

            /* first match per processor only; ties keep the earlier processor */
            if (!strcmp(impl->name, name) && impl->rank > best_rank) {
                best_rank = impl->rank;
                best = impl;
                break;
            }
        }
    }
    if (best)
        pthread_mutex_lock(&best->lock);
    pthread_mutex_unlock(&processors_lock);
    return best;
}

/* @0x18df4: instance by uid, owned by o first, then globally */
struct aud_mod_inst *owner_inst_by_uid(struct aud_mod_owner *o, unsigned int uid)
{
    struct link *l, *pl, *il, *nl;

    pthread_mutex_lock(&o->instances_lock);
    for_each_link(l, &o->instances) {
        struct aud_mod_inst *inst = link_container(struct aud_mod_inst, owner_list, l);

        if (inst->uid == uid) {
            pthread_mutex_unlock(&o->instances_lock);
            return inst;
        }
    }
    pthread_mutex_unlock(&o->instances_lock);

    pthread_mutex_lock(&processors_lock);
    for_each_link(pl, &processors) {
        struct aud_proc_info *p = PROC_OF(pl);

        if (uid >> 24 != (unsigned int)p->proc_id)
            continue;
        pthread_mutex_lock(&p->mods_lock);
        for_each_link(il, &p->mods) {
            struct aud_mod_impl *impl = IMPL_OF(il);

            if (((uid & 0xffffff) >> 16) != (unsigned int)impl->impl_num)
                continue;
            pthread_mutex_lock(&impl->lock);
            for_each_link(nl, &impl->instances) {
                if (INST_OF(nl)->uid == uid) {
                    pthread_mutex_unlock(&impl->lock);
                    pthread_mutex_unlock(&p->mods_lock);
                    pthread_mutex_unlock(&processors_lock);
                    return INST_OF(nl);
                }
            }
            pthread_mutex_unlock(&impl->lock);
            break;
        }
        pthread_mutex_unlock(&p->mods_lock);
    }
    pthread_mutex_unlock(&processors_lock);
    LOGE(aud_modlist, 0x533, "no such uid(%x) on %s\n", uid, o->name);
    return NULL;
}

/* @0x18f44: p->mods_lock held */
struct aud_mod_impl *register_impl(struct aud_proc_info *p, const char *name, int rank,
                                   int impl_num)
{
    struct aud_mod_impl *impl;
    size_t n;

    if (p->cur_impl_num == 0xff) {
        LOGE(aud_modlist, 0x105, "too many implementations on processor %s\n", p->name);
        return NULL;
    }
    n = strlen(name);
    impl = calloc(1, sizeof(*impl) + n + 1);
    if (!impl) {
        LOGE(aud_modlist, 0x10d, "no mem for %s\n", name);
        return NULL;
    }
    memcpy(impl->name, name, n);
    impl->name[n] = '\0';
    LINK_INIT(&impl->instances);
    impl->rank = rank;
    impl->processor = p;
    pthread_mutex_init(&impl->lock, NULL);
    if (impl_num == -1)
        impl_num = ++p->cur_impl_num;
    impl->impl_num = impl_num;
    link_add_tail(&p->mods, &impl->mods_list);
    LOGI(aud_modlist, 0x11f, "processor %s, add %s, impl_num %x\n", p->name, impl->name,
         impl_num);
    return impl;
}

/* @0x194c4: allocate uid, create the instance record, ask the processor's registry */
struct aud_mod_inst *create_inst_on_sync(struct aud_mod_impl *impl, struct aud_mod_owner *o,
                                         bool sync)
{
    unsigned int last = impl->last_index, idx = last + 1;
    struct aud_mod_inst *inst;
    struct link *l;
    size_t n;

    LOGI(aud_modlist, 0x2a4, "create instance for %s\n", impl->name);
    for (;; idx++) {
        if (idx == 0xf000)
            idx = 1;
        for_each_link(l, &impl->instances)
            if ((INST_OF(l)->uid & 0xffff) == idx)
                break;
        if (l == &impl->instances)
            break;
        if (idx + 1 == last) {
            LOGE(aud_modlist, 0x2c0, "too many instances for %s at %s\n", impl->name,
                 impl->processor->name);
            return NULL;
        }
    }
    if (idx == last)
        return NULL;
    impl->last_index = idx;

    n = strlen(impl->name);
    inst = calloc(1, sizeof(*inst) + n + 10);
    if (!inst) {
        LOGE(aud_modlist, 0x2cc, "no mem for %s\n", impl->name);
        return NULL;
    }
    inst->uid = impl->processor->proc_id << 24 | impl->impl_num << 16 | idx;

    for_each_link(l, &helper_list) {
        struct aud_impl_helper *h = link_container(struct aud_impl_helper, list, l);

        if (strcmp(h->impl_name, impl->name))
            continue;
        inst->helper = h;
        if (h->create_pre) {
            inst->helper_priv = h->create_pre(h->impl_name, inst->uid);
            if (!inst->helper_priv) {
                LOGE(aud_modlist, 0x2d7, "helper failed for \"%s\"\n", impl->name);
                free(inst);
                return NULL;
            }
        }
        break;
    }

    inst->owner = o;
    inst->mod_impl = impl;
    impl->alloc_count++;
    inst->users = 1;
    pthread_mutex_init(&inst->lock, NULL);
    LINK_INIT(&inst->input_ports);
    LINK_INIT(&inst->output_ports);
    snprintf(inst->name, n + 10, "%s_%08x", impl->name, inst->uid);
    link_add_tail(&impl->instances, &inst->impl_list);

    if (impl->impl_num) {
        struct {
            char name[0x20];
            unsigned int uid;
        } req;
        unsigned int reg_uid = impl->processor->proc_id << 24 | 1;

        strncpy(req.name, impl->name, sizeof(req.name) - 1);
        req.name[sizeof(req.name) - 1] = '\0';
        req.uid = inst->uid;
        LOGI(aud_modlist, 0x2f2, "create by registry\n");
        if (sync)
            aimc_send_command_sync(gimc, REGCMD_CREATE_MODULE, 0, reg_uid, &req, sizeof(req));
        else
            aimc_send_command(gimc, REGCMD_CREATE_MODULE, 0, reg_uid, &req, sizeof(req));
    }
    if (inst->helper && inst->helper->create_post)
        inst->helper->create_post(inst->helper_priv, inst->helper->impl_name, inst->uid);
    if (o) {
        pthread_mutex_lock(&o->instances_lock);
        link_add_tail(&o->instances, &inst->owner_list);
        pthread_mutex_unlock(&o->instances_lock);
    }
    LOGI(aud_modlist, 0x308, "instance created. %08x\n", inst->uid);
    return inst;
}

/* @0x197e0: processors_lock held */
struct aud_proc_info *register_processor(int proc_id, const char *fmt, ...)
{
    struct aud_proc_info *p = calloc(1, sizeof(*p));
    va_list ap;

    if (!p) {
        LOGE(aud_modlist, 0x138, "no mem\n");
        return NULL;
    }
    va_start(ap, fmt);
    vsnprintf(p->name, sizeof(p->name), fmt, ap);
    va_end(ap);
    LINK_INIT(&p->mods);
    p->cur_impl_num = 0xf;
    p->proc_id = proc_id;
    pthread_mutex_init(&p->mods_lock, NULL);
    LOGN(aud_modlist, 0x146, "new processor. %s, proc_id %x\n", p->name, p->proc_id);
    link_add(&processors, &p->proc_list);
    return p;
}

/* destroy_glue @0x19080, connect_in_processor @0x19220, disconnect_in_processor @0x198b4,
 * destroy_jumper @0x19a30, aud_modlist_disconnect_inst_* @0x1a528/@0x1a66c and
 * aud_modlist_connect_inst @0x1acec live in objects.c with the port/jumper records. */

/* @0x19bd0 */
void *aud_impl_register_helper(const char *impl_name,
    void *(*create_pre)(const char *, unsigned int),
    int (*create_post)(void *, const char *, unsigned int),
    int (*destroy_pre)(void *, const char *, unsigned int),
    int (*destroy_post)(void *, const char *, unsigned int))
{
    struct aud_impl_helper *h;
    struct link *l;

    for_each_link(l, &helper_list)
        if (!strcmp(link_container(struct aud_impl_helper, list, l)->impl_name, impl_name)) {
            LOGE(aud_modlist, 0xd9, "impl, \"%s\", already registered.\n", impl_name);
            return NULL;
        }
    h = calloc(1, sizeof(*h));
    if (!h) {
        LOGE(aud_modlist, 0xe0, "nomem for %s\n", impl_name);
        return NULL;
    }
    h->impl_name = impl_name;
    h->create_pre = create_pre;
    h->create_post = create_post;
    h->destroy_pre = destroy_pre;
    h->destroy_post = destroy_post;
    link_add_tail(&helper_list, &h->list);
    return h;
}

/* @0x19cc4 */
void aud_impl_unregister_helper(const char *impl_name)
{
    struct link *l;

    for_each_link(l, &helper_list) {
        struct aud_impl_helper *h = link_container(struct aud_impl_helper, list, l);

        if (!strcmp(h->impl_name, impl_name)) {
            link_del(&h->list);
            free(h);
            return;
        }
    }
    LOGE(aud_modlist, 0xf5, "no such helper for %s\n", impl_name);
}

/* @0x19d4c */
struct aud_mod_impl *aud_modlist_register_impl(struct aud_proc_info *p, const char *name,
                                               int rank)
{
    struct aud_mod_impl *impl;

    pthread_mutex_lock(&p->mods_lock);
    impl = register_impl(p, name, rank, -1);
    pthread_mutex_unlock(&p->mods_lock);
    return impl;
}

/* @0x19d78: next free processor id, 16..255 */
struct aud_proc_info *aud_modlist_register_processor(int *index)
{
    struct aud_proc_info *p;
    int id = next_proc_id, tries = 0xff;
    struct link *l;

    pthread_mutex_lock(&processors_lock);
    for (;;) {
        for_each_link(l, &processors)
            if (PROC_OF(l)->proc_id == id)
                break;
        if (l == &processors)
            break;
        if (++id > 0xff)
            id = 0x10;
        if (--tries == 0) {
            next_proc_id = id;
            LOGE(aud_modlist, 0x16a, "too many processor registered. cannot select proc_id\n");
            pthread_mutex_unlock(&processors_lock);
            return NULL;
        }
    }
    next_proc_id = id;
    p = register_processor(next_proc_id, "processor_%d", next_proc_id);
    if (p && index)
        *index = next_proc_id;
    pthread_mutex_unlock(&processors_lock);
    return p;
}

/* @0x19e8c: drops every implementation; instances are orphaned and their owners woken */
int aud_modlist_unregister_processor(struct aud_proc_info *p)
{
    struct link *il, *inext, *nl, *nnext;

    pthread_mutex_lock(&processors_lock);
    LOGN(aud_modlist, 0x19f, "unregister processor %s\n", p->name);
    link_del(&p->proc_list);
    pthread_mutex_lock(&p->mods_lock);
    for (il = p->mods.next, inext = il->next; il != &p->mods; il = inext, inext = il->next) {
        struct aud_mod_impl *impl = IMPL_OF(il);

        /* inlined unregister_impl */
        LOGI(aud_modlist, 0x17c, "processor %s(%d), remove implement %s\n",
             impl->processor->name, impl->processor->proc_id, impl->name);
        pthread_mutex_lock(&impl->lock);
        for (nl = impl->instances.next, nnext = nl->next; nl != &impl->instances;
             nl = nnext, nnext = nl->next) {
            struct aud_mod_inst *inst = INST_OF(nl);

            LOGI(aud_modlist, 0x182, "remove instance %08x from implement %s\n", inst->uid,
                 impl->name);
            link_del(&inst->impl_list);
            inst->impl_list.prev = inst->impl_list.next = NULL;
            pthread_mutex_lock(&inst->lock);
            if (inst->owner) {
                inst->owner->flags |= 1;
                pthread_cond_broadcast(&inst->owner->flags_cond);
            }
            inst->mod_impl = NULL;
            pthread_mutex_unlock(&inst->lock);
        }
        pthread_mutex_unlock(&impl->lock);
        link_del(&impl->mods_list);
        free(impl->mem_defs);
        free(impl);
    }
    pthread_mutex_unlock(&p->mods_lock);
    free(p);
    pthread_mutex_unlock(&processors_lock);
    return 0;
}

/* @0x1a048 */
struct aud_mod_owner *aud_modlist_new_ownerv(const char *name, va_list ap)
{
    struct aud_mod_owner *o = calloc(1, sizeof(*o));

    if (!o) {
        LOGE(aud_modlist, 0x1bd, "no mem\n");
        return NULL;
    }
    vsnprintf(o->name, sizeof(o->name), name, ap);
    LINK_INIT(&o->instances);
    pthread_mutex_init(&o->instances_lock, NULL);
    pthread_cond_init(&o->flags_cond, NULL);
    LOGI(aud_modlist, 0x1c8, "new owner %s\n", o->name);
    return o;
}

/* @0x1a100 */
struct aud_mod_owner *aud_modlist_new_owner(const char *name, ...)
{
    struct aud_mod_owner *o;
    va_list ap;

    va_start(ap, name);
    o = aud_modlist_new_ownerv(name, ap);
    va_end(ap);
    return o;
}

/* @0x1a11c */
int aud_modlist_register_impl_port_buf_def(struct aud_proc_info *p, const char *impl_name,
                                           int port, int max_num_au, int max_access_size)
{
    struct aud_mod_impl *impl;

    pthread_mutex_lock(&p->mods_lock);
    impl = search_modimpl(impl_name, p->proc_id);
    if (!impl) {
        LOGE(aud_modlist, 0x281, "no such implementation on %d. %s\n", p->proc_id, impl_name);
        pthread_mutex_unlock(&p->mods_lock);
        return -22;
    }
    /* [observed] sized by the first call only: later, larger port numbers overflow */
    if (!impl->mem_defs) {
        impl->mem_defs = calloc(1, (port + 1) * sizeof(*impl->mem_defs));
        if (!impl->mem_defs) {
            LOGE(aud_modlist, 0x28b, "no mem\n");
            pthread_mutex_unlock(&p->mods_lock);
            return -12;
        }
        impl->num_mem_defs = port + 1;
    }
    impl->mem_defs[port].max_num_au = max_num_au;
    impl->mem_defs[port].max_access_size = max_access_size;
    LOGI(aud_modlist, 0x294, "%dth mem def for %s on %d, n_au %d, max_size %d(0x%x)", port,
         impl_name, p->proc_id, max_num_au, max_access_size, max_access_size);
    pthread_mutex_unlock(&impl->lock);
    pthread_mutex_unlock(&p->mods_lock);
    return 0;
}

/* @0x1a248: preferred_processor <= 0 means "highest rank on any processor" */
struct aud_mod_inst *aud_modlist_create_inst(struct aud_mod_owner *o, const char *impl_name,
                                             int preferred_processor, bool sync)
{
    struct aud_mod_impl *impl;
    struct aud_mod_inst *inst;

    LOGI(aud_modlist, 0x313, "create instance for %s on %d\n", impl_name, preferred_processor);
    impl = search_modimpl(impl_name, preferred_processor);
    if (!impl) {
        LOGE(aud_modlist, 0x319, "no such implementation on %d. %s\n", preferred_processor,
             impl_name);
        return NULL;
    }
    inst = create_inst_on_sync(impl, o, sync);
    pthread_mutex_unlock(&impl->lock);
    if (!inst) {
        LOGE(aud_modlist, 0x321, "cannot create instance.\n");
        return NULL;
    }
    if (impl->mem_defs) {
        size_t sz = impl->num_mem_defs * sizeof(*impl->mem_defs);

        inst->mem_defs = calloc(1, sz);
        if (!inst->mem_defs) {
            LOGE(aud_modlist, 0x32a, "no mem\n");
            return NULL;
        }
        inst->num_mem_defs = impl->num_mem_defs;
        memcpy(inst->mem_defs, impl->mem_defs, sz);
    }
    LOGI(aud_modlist, 0x331, "new instance from %s:%s, uid %x\n", impl->processor->name,
         impl->name, inst->uid);
    return inst;
}

/* @0x1a3dc */
int aud_modlist_role_set(struct aud_mod_inst *inst, const char *role)
{
    LOGI(aud_modlist, 0x349, "set role \"%s\" to instance \"%s\"\n", role, inst->name);
    free(inst->role);
    if (role) {
        inst->role = strdup(role);
        if (!inst->role)
            LOGE(aud_modlist, 0x354, "no mem for role %s, inst %s\n", role, inst->name);
    }
    return 0;
}

/* @0x1a474: index-th instance of impl_name owned by o */
struct aud_mod_inst *aud_modlist_search(struct aud_mod_owner *o, const char *impl_name, int index)
{
    struct link *l;

    pthread_mutex_lock(&o->instances_lock);
    for_each_link(l, &o->instances) {
        struct aud_mod_inst *inst = link_container(struct aud_mod_inst, owner_list, l);

        pthread_mutex_lock(&inst->lock);
        if (inst->mod_impl && !strcmp(inst->mod_impl->name, impl_name)) {
            if (index == 0) {
                pthread_mutex_unlock(&inst->lock);
                pthread_mutex_unlock(&o->instances_lock);
                return inst;
            }
            index--;
        }
        pthread_mutex_unlock(&inst->lock);
    }
    pthread_mutex_unlock(&o->instances_lock);
    LOGE(aud_modlist, 0x370, "unknown instance. %s, index %d\n", impl_name, index);
    return NULL;
}

/* @0x1a750: drop a reference; on the last one disconnect everything and destroy remotely */
void put_instance(struct aud_mod_inst *inst)
{
    struct link *l, *next;
    unsigned int uid;

    if (--inst->users > 0)
        return;
    LOGI(aud_modlist, 0x20b, "destroy instance. %p(%s)\n", inst, inst->name);
    if (!link_empty(&inst->input_ports)) {
        LOGN(aud_modlist, 0x213, "%s: input port still active. disconnect.\n", inst->name);
        for (l = inst->input_ports.next, next = l->next; l != &inst->input_ports;
             l = next, next = l->next)
            aud_modlist_disconnect_inst_input(inst->owner, inst,
                                              link_container(struct port, port_list, l)->index);
    }
    if (!link_empty(&inst->output_ports)) {
        LOGN(aud_modlist, 0x21c, "%s: output port still active. disconnect.\n", inst->name);
        for (l = inst->output_ports.next, next = l->next; l != &inst->output_ports;
             l = next, next = l->next)
            aud_modlist_disconnect_inst_output(inst->owner, inst,
                                               link_container(struct port, port_list, l)->index);
    }
    if (inst->helper && inst->helper->destroy_pre)
        inst->helper->destroy_pre(inst->helper_priv, inst->helper->impl_name, inst->uid);
    if (inst->mod_impl) {
        uid = inst->uid;
        aimc_send_command(gimc, REGCMD_DESTROY_MODULE, 0,
                          inst->mod_impl->processor->proc_id << 24 | 1, &uid, sizeof(uid));
        inst->mod_impl->alloc_count--;
    }
    if (inst->helper && inst->helper->destroy_post)
        inst->helper->destroy_post(inst->helper_priv, inst->helper->impl_name, inst->uid);
    if (inst->role)
        aud_modlist_role_set(inst, NULL);
    remove_from_processor(inst);
    link_del(&inst->owner_list);
    free(inst);
}

/* @0x1a8dc */
void aud_modlist_del_owner(struct aud_mod_owner *o)
{
    struct link *l, *next;

    LOGI(aud_modlist, 0x24a, "delete owner %s\n", o->name);
    pthread_mutex_lock(&o->instances_lock);
    for (l = o->instances.next, next = l->next; l != &o->instances; l = next, next = l->next)
        put_instance(link_container(struct aud_mod_inst, owner_list, l));
    pthread_mutex_unlock(&o->instances_lock);
    free(o);
}

/* @0x1a954 */
void aud_modlist_destroy_inst(struct aud_mod_inst *inst)
{
    pthread_mutex_t *lock = &inst->owner->instances_lock;

    LOGI(aud_modlist, 0x33a, "destroy instance %x\n", inst->uid);
    pthread_mutex_lock(lock);
    if (inst->mem_defs) {
        free(inst->mem_defs);
        inst->mem_defs = NULL;
    }
    put_instance(inst);
    pthread_mutex_unlock(lock);
}

/* @0x1a9c0 */
unsigned int aud_inst_create(struct aud_mod_owner *o, const char *impl_name, int proc, bool sync)
{
    struct aud_mod_inst *inst = aud_modlist_create_inst(o, impl_name, proc, sync);

    return inst ? inst->uid : 0;
}

/* @0x1a9cc */
void aud_inst_destroy(struct aud_mod_owner *o, unsigned int uid)
{
    struct aud_mod_inst *inst = owner_inst_by_uid(o, uid);

    if (!inst) {
        LOGE(aud_modlist, 0x59b, "no owner\n");
        return;
    }
    aud_modlist_destroy_inst(inst);
}

/* @0x1aa28 */
int aud_inst_disconnect_output(struct aud_mod_owner *o, unsigned int uid, int port)
{
    struct aud_mod_inst *inst = owner_inst_by_uid(o, uid);

    if (!inst) {
        LOGE(aud_modlist, 0x5ba, "no inst\n");
        return -22;
    }
    return aud_modlist_disconnect_inst_output(o, inst, port);
}

/* @0x1aa88 */
int aud_inst_disconnect_input(struct aud_mod_owner *o, unsigned int uid, int port)
{
    struct aud_mod_inst *inst = owner_inst_by_uid(o, uid);

    if (!inst) {
        LOGE(aud_modlist, 0x5c8, "no inst\n");
        return -22;
    }
    return aud_modlist_disconnect_inst_input(o, inst, port);
}

/* @0x1b2a0: [observed] snprintf limit is strlen(name), so the last char is cut off */
void aud_mod_name(struct aud_mod_owner *o, unsigned int uid, char *name)
{
    struct aud_mod_inst *inst = owner_inst_by_uid(o, uid);

    if (!inst) {
        LOGE(aud_modlist, 0x66c, "uid(%x) is not created\n", uid);
        return;
    }
    if (!name) {
        LOGE(aud_modlist, 0x672, "param is null\n");
        return;
    }
    snprintf(name, strlen(inst->name), "%s", inst->name);
}

/* @0x1b344 */
int aud_modlist_set_aimc(struct aimc *imc)
{
    gimc = imc;
    return 0;
}

/* @0x1b354: debug dump, called through adebug "callv aud_elist <what>":
 *   pids     comma separated processor ids
 *   procs    processors with their implementations and instances
 *   jumpers  graphviz nodes for the shared buffers of all jumpers
 *   uids     "<impl> : <uid>" for every instance */
int aud_elist(int argc, char **argv)
{
    const char *what = argv[1];
    struct link *pl, *il, *nl;

    (void)argc;
    if (!what)
        return 0;
    if (!strcmp(what, "pids")) {
        unsigned int len = 0;
        char *s = NULL;

        pthread_mutex_lock(&processors_lock);
        for_each_link(pl, &processors)
            s = audutil_append_printf(s, &len, len ? ",%d" : "%d", PROC_OF(pl)->proc_id);
        pthread_mutex_unlock(&processors_lock);
        logm_message("%s\n", s);
        free(s);
        return 0;
    }
    if (!strcmp(what, "procs")) {
        pthread_mutex_lock(&processors_lock);
        for_each_link(pl, &processors) {
            struct aud_proc_info *p = PROC_OF(pl);

            logm_message("pid:%d\n", p->proc_id);
            logm_message("impls:\n");
            pthread_mutex_lock(&p->mods_lock);
            for_each_link(il, &p->mods) {
                struct aud_mod_impl *impl = IMPL_OF(il);

                logm_message("  name:%s\n", impl->name);
                logm_message("    impl_num:%d\n", impl->impl_num);
                logm_message("    rank:%d\n", impl->rank);
                logm_message("    alloc_count:%d\n", impl->alloc_count);
                logm_message("    last_index:%d\n", impl->last_index);
                logm_message("    insts:\n");
                pthread_mutex_lock(&impl->lock);
                for_each_link(nl, &impl->instances) {
                    logm_message("      name:%s\n", INST_OF(nl)->name);
                    logm_message("        uid:0x%08x\n", INST_OF(nl)->uid);
                }
                pthread_mutex_unlock(&impl->lock);
            }
            pthread_mutex_unlock(&p->mods_lock);
        }
        pthread_mutex_unlock(&processors_lock);
        return 0;
    }
    if (!strcmp(what, "jumpers")) {
        for_each_link(nl, &all_jumpers) {
            struct jumper *j = link_container(struct jumper, all_jumpers_list, nl);
            char *id, *c;

            if (j->offset) {
                logm_message("hma_%08x[label=\"hma:0x%08x\", shape=cylinder]\n", j->offset,
                             j->offset);
                continue;
            }
            if (!j->shm_name) {
                LOGE(aud_modlist, 0x6d1, "Oops??\n");
                continue;
            }
            id = strdup(j->shm_name);       /* graphviz id: ':' and '-' become '_' */
            for (c = id; *c; c++)
                if (*c == ':' || *c == '-')
                    *c = '_';
            logm_message("%s[label=\"%s\", shape=cylinder]\n", id, j->shm_name);
            free(id);
        }
        return 0;
    }
    if (!strcmp(what, "uids")) {
        pthread_mutex_lock(&processors_lock);
        for_each_link(pl, &processors) {
            struct aud_proc_info *p = PROC_OF(pl);

            pthread_mutex_lock(&p->mods_lock);
            for_each_link(il, &p->mods) {
                struct aud_mod_impl *impl = IMPL_OF(il);

                pthread_mutex_lock(&impl->lock);
                for_each_link(nl, &impl->instances)
                    logm_message("%12s : 0x%08x\n", impl->name, INST_OF(nl)->uid);
                pthread_mutex_unlock(&impl->lock);
            }
            pthread_mutex_unlock(&p->mods_lock);
        }
        pthread_mutex_unlock(&processors_lock);
    }
    return 0;
}

/* ========================================================================= */
/* acontroller: aipc server that lets other processes (DSP glue daemons,     */
/* other aimc clients) register as processors. Evidence @0x1846c..0x18c24,   */
/* command table @0x4b6e4, logmm module "acont".                             */
/* ========================================================================= */

/* one connected client process (= one processor) */
struct acont_client {                        /* calloc(0x18) */
    struct link list;
    int cfd;                                 /* 0x08 */
    int pid;                                 /* 0x0c */
    struct aud_proc_info *proc;              /* 0x10 */
    struct aud_mod_owner *owner;             /* 0x14 */
};

struct link acont_clients = LINK_INITIALIZER(acont_clients);   /* @0x4c018 */
struct aipc *acont_aipc;                                       /* @0x5111c */
int acont_nfds_alloc;                                          /* @0x51120 */
struct pollfd *acont_fds;                                      /* @0x51124 */
pthread_t acont_thread;                                        /* @0x51128 */

/* @0x1846c */
struct acont_client *get_cinfo(int cfd)
{
    struct link *l;

    for_each_link(l, &acont_clients)
        if (link_container(struct acont_client, list, l)->cfd == cfd)
            return link_container(struct acont_client, list, l);
    LOGE(acont, 0x37, "unknown cfd %d\n", cfd);
    return NULL;
}

/* @0x184e4: a client process went away: forget its processor and destroy its instances */
int cb_client_disconnected(void *arg, int cfd)
{
    struct acont_client *c = get_cinfo(cfd);

    (void)arg;
    if (!c) {
        LOGE(acont, 0xf0, "unknown cfd %d disconnected\n", cfd);
        return 0;
    }
    LOGI(acont, 0xe7, "cfd %d, pid %d, disconnected.\n", c->cfd, c->pid);
    aud_modlist_unregister_processor(c->proc);
    aud_modlist_del_owner(c->owner);
    link_del(&c->list);
    free(c);
    return 0;
}

/* @0x18584 */
int acont_module_disconnect_input(void *arg, int cfd, const char *name, int nargs,
                                  struct aipc_ipc_arg *args)
{
    struct acont_client *c = get_cinfo(cfd);

    (void)arg, (void)name, (void)nargs;
    if (!c) {
        LOGE(acont, 199, "no client\n");
        return -1;
    }
    LOGI(acont, 0xce, "proc%d. disconnect -> %08x.%d\n", c->pid, args[0].i, args[1].i);
    return aud_inst_disconnect_input(c->owner, args[0].i, args[1].i);
}

/* @0x18618 */
int acont_module_disconnect_output(void *arg, int cfd, const char *name, int nargs,
                                   struct aipc_ipc_arg *args)
{
    struct acont_client *c = get_cinfo(cfd);

    (void)arg, (void)name, (void)nargs;
    if (!c) {
        LOGE(acont, 0xb3, "no client\n");
        return -1;
    }
    LOGI(acont, 0xba, "proc%d. disconnect %08x.%d ->\n", c->pid, args[0].i, args[1].i);
    return aud_inst_disconnect_output(c->owner, args[0].i, args[1].i);
}

/* @0x186ac */
int acont_module_connect(void *arg, int cfd, const char *name, int nargs,
                         struct aipc_ipc_arg *args)
{
    struct acont_client *c = get_cinfo(cfd);

    (void)arg, (void)name, (void)nargs;
    if (!c) {
        LOGE(acont, 0x9d, "no client\n");
        return -1;
    }
    LOGI(acont, 0xa6, "proc%d. connect %08x.%d -> %08x.%d\n", c->pid, args[0].i, args[1].i,
         args[2].i, args[3].i);
    return aud_inst_connect(c->owner, args[0].i, args[1].i, args[2].i, args[3].i, true);
}

/* @0x18758 */
int acont_module_destroy(void *arg, int cfd, const char *name, int nargs,
                         struct aipc_ipc_arg *args)
{
    struct acont_client *c = get_cinfo(cfd);

    (void)arg, (void)name, (void)nargs;
    if (!c) {
        LOGE(acont, 0x89, "no client\n");
        return -1;
    }
    LOGI(acont, 0x8f, "proc%d. destroy. uid %08x\n", c->pid, args[0].i);
    aud_inst_destroy(c->owner, args[0].i);
    return 0;
}

/* @0x187e8: returns the new uid (0 on failure) */
int acont_module_create(void *arg, int cfd, const char *name, int nargs,
                        struct aipc_ipc_arg *args)
{
    struct acont_client *c = get_cinfo(cfd);
    unsigned int uid;

    (void)arg, (void)name, (void)nargs;
    if (!c) {
        LOGE(acont, 0x72, "no client\n");
        return -1;
    }
    LOGI(acont, 0x78, "proc%d. instantiation. %s(at %d)\n", c->pid, args[0].s, args[1].i);
    uid = aud_inst_create(c->owner, args[0].s, args[1].i, true);
    LOGI(acont, 0x7b, "proc%d. instantiation. %s(at %d) uid %08x\n", c->pid, args[0].s,
         args[1].i, uid);
    return uid;
}

/* @0x188c4 */
int acont_add_implement(void *arg, int cfd, const char *name, int nargs,
                        struct aipc_ipc_arg *args)
{
    struct acont_client *c = get_cinfo(cfd);

    (void)arg, (void)name, (void)nargs;
    if (!c) {
        LOGE(acont, 0x5c, "no client\n");
        return -1;
    }
    LOGI(acont, 0x62, "proc%d. add implement. %s(rank %d)\n", c->pid, args[0].s, args[1].i);
    aud_modlist_register_impl(c->proc, args[0].s, args[1].i);
    return 0;
}

/* @0x18368: accepted but ignored */
int acont_add_port_buf_def(void *arg, int cfd, const char *name, int nargs,
                           struct aipc_ipc_arg *args)
{
    (void)arg, (void)cfd, (void)name, (void)nargs, (void)args;
    return 0;
}

/* @0x1836c: a new client process registers itself as a processor; returns its proc id */
int acont_register_proc(void *arg, int cfd, const char *name, int nargs,
                        struct aipc_ipc_arg *args)
{
    struct acont_client *c;

    (void)arg, (void)nargs, (void)args;
    LOGI(acont, 0x40, "new processor. cfd %d, cmd name %s..\n", cfd, name);
    c = calloc(1, sizeof(*c));      /* [observed] not checked */
    c->cfd = cfd;
    c->proc = aud_modlist_register_processor(&c->pid);
    c->owner = aud_modlist_new_owner("owner_cfd%d", cfd);
    link_add_tail(&acont_clients, &c->list);
    LOGI(acont, 0x48, "register processor.. pid %d for cfd %d\n", c->pid, cfd);
    return c->pid;
}

/* @0x18420: "__sync" round trip */
int acont_sync(void *arg, int cfd, const char *name, int nargs, struct aipc_ipc_arg *args)
{
    (void)arg, (void)cfd, (void)name, (void)nargs, (void)args;
    LOGI(acont, 0x11e, "sync..\n");
    return 0;
}

/* @0x1895c */
int acont_role_search(void *arg, int cfd, const char *name, int nargs,
                      struct aipc_ipc_arg *args)
{
    (void)arg, (void)cfd, (void)name, (void)nargs, (void)args;
    LOGW(acont, 0xdc, "not implemented\n");
    return 0;
}

/* @0x189a4 */
int acont_role_set(void *arg, int cfd, const char *name, int nargs, struct aipc_ipc_arg *args)
{
    (void)arg, (void)cfd, (void)name, (void)nargs, (void)args;
    LOGW(acont, 0xd5, "not implemented\n");
    return 0;
}

/* command table @0x4b6e4 */
struct acont_command {
    const char *name;
    aipc_command_fn func;
};

const struct acont_command acont_commands[] = {
    { "acont_register_proc",            acont_register_proc },
    { "acont_add_port_buf_def",         acont_add_port_buf_def },
    { "acont_add_implement",            acont_add_implement },
    { "acont_module_create",            acont_module_create },
    { "acont_module_destroy",           acont_module_destroy },
    { "acont_module_connect",           acont_module_connect },
    { "acont_module_disconnect_output", acont_module_disconnect_output },
    { "acont_module_disconnect_input",  acont_module_disconnect_input },
    { "acont_role_set",                 acont_role_set },
    { "acont_role_search",              acont_role_search },
};

/* @0x18b00 */
int acont_init(void)
{
    unsigned int i;

    acont_aipc = aipc_create0();
    if (!acont_aipc) {
        LOGE(acont, 0x13f, "no aimc0\n");
        return -1;
    }
    /* [observed] the aipc itself is passed as the callback's priv */
    aipc_register_cb(acont_aipc, cb_client_disconnected, acont_aipc);
    for (i = 0; i < sizeof(acont_commands) / sizeof(acont_commands[0]); i++)
        aipc_register_command(acont_aipc, acont_commands[i].name, acont_commands[i].func, NULL);
    aipc_register_command(acont_aipc, "__sync", acont_sync, NULL);
    adebug_create(acont_aipc);
    return 0;
}

/* @0x189ec: acont thread: serve the aipc sockets forever */
void *do_ipc_handle(void *arg)
{
    int nfds, ret;

    (void)arg;
    for (;;) {
        nfds = aipc_get_pollfd(acont_aipc, NULL);
        if (nfds > acont_nfds_alloc) {
            acont_fds = realloc(acont_fds, nfds * sizeof(*acont_fds));
            acont_nfds_alloc = nfds;
        }
        aipc_get_pollfd(acont_aipc, acont_fds);
        LOGD(acont, 0x105, "poll()... nfds %d\n", nfds);
        ret = poll(acont_fds, nfds, -1);
        if (ret <= 0) {
            LOGE(acont, 0x108, "poll() error. ret %d, %s\n", ret, strerror(errno));
            continue;
        }
        LOGD(acont, 0x10b, "got poll %d\n", ret);
        aipc_handle_pollfd(acont_aipc, acont_fds);
    }
    return NULL;
}

/* @0x18bbc */
int acont_start(void)
{
    int ret = pthread_create(&acont_thread, NULL, do_ipc_handle, NULL);

    if (ret)
        LOGE(acont, 0x155, "pthread_create() failed. %d\n", ret);
    return 0;
}

/* @0x18c24 */
int acont_set_aimc(struct aimc *imc)
{
    aud_modlist_set_aimc(imc);
    adebug_set_aimc(imc);
    return 0;
}
