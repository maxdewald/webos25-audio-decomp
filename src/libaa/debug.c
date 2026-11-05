/*
 * Debug commands of libaa: the "adebug" aipc commands (callv / set_prop) and the
 * functions reachable through "callv <symbol> args..." (debug_registry, debug_mreg,
 * debug_reg, dot_registry, debug_bin, audreg_print_property, debug_module, debug_object).
 * callv runs any exported function as fn(argc, argv) with its logm_message output
 * redirected back to the requesting aipc client.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 *
 * Evidence: Ghidra build/decomp/libaa.so.2.0.0.c @0x1c7fc..@0x1cbec, @0x26ea0..@0x2813a,
 * @0x2cf44, @0x30a74; thumb disassembly of the unnamed statics @0x1c7fc (cmd_set_prop),
 * @0x1cb00 (log hook), @0x272c0/@0x272e4 (debug_bin callbacks); strings 0x32fc8..0x330a8
 * and 0x36248..0x366cc. The debug_registry option table (.rodata 0x3685c) is empty.
 */
#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aimc_local.h"
#include "objects.h"     /* dump_class, aud_object_class_foreach */

LOGMM_MODULE_DEFINE(adbg);

struct adebug {                              /* calloc(0xc) */
    void *dl;                                /* dlopen(NULL): symbols for callv */
    struct aipc *ipc;
    pthread_t caller;                        /* thread running a callv, else 0 */
};

static struct aimc *adebug_aimc;             /* @0x51170, target for set_prop */

static const struct audutil_opt registry_debug_opts[] = {
    { 0 },
};

/* @0x1c7fc: "set_prop <uid> <prop string>": forward as ADEC_CMD_SET_PROP to uid.
 * Always answers -1; the payload is never freed. */
int cmd_set_prop(void *arg, int cfd, const char *name, int nargs, struct aipc_ipc_arg *args)
{
    const char *prop;
    unsigned int uid, len, size;
    char *p;

    (void)arg;
    (void)name;
    if (nargs < 1) {
        LOGE(adbg, 0x80, "no function name. from cfd %d\n", cfd);
        return -1;
    }
    if (args[0].type != ipc_argtype_int || args[1].type != ipc_argtype_str) {
        LOGE(adbg, 0x86, "type error %d %d\n", args[0].type, args[1].type);
        return -1;
    }
    /* the uid is printed before it is read: always shows 0xffffffff */
    LOGN(adbg, 0x8a, "set prop %s to 0x%x\n", args[1].s, -1);

    prop = args[1].s;
    len = strlen(prop);
    size = len + 5;
    p = malloc(size);
    uid = args[0].i;
    snprintf(p + 4, len + 1, "%s", prop);
    *(unsigned int *)p = len;
    LOGN(adbg, 0x93, "'0x%08x' len %d, prop '%s'\n", uid, len, p + 4);
    aimc_send_command(adebug_aimc, ADEC_CMD_SET_PROP, 0, uid, p, size);
    return -1;
}

/* @0x1cb00: logm_message hook while a callv runs: the calling thread's output goes to the
 * aipc reply, other threads keep logging normally */
int adebug_log_hook(void *arg, char *str)
{
    struct adebug *dbg = arg;

    if (dbg->caller != pthread_self()) {
        logm_message_defaultf("%s", str);
        free(str);
        return 0;
    }
    aipc_append_additional(dbg->ipc, str, strlen(str));
    return 0;
}

/* @0x1c938: "callv <symbol> [string args...]": call symbol(argc, argv) in this process */
int cmd_callv(void *arg, int cfd, const char *name, int nargs, struct aipc_ipc_arg *args)
{
    struct adebug *dbg = arg;
    logm_message_hook_func_t old;
    int (*fn)(int argc, char **argv);
    char **argv;
    int i, ret;

    if (nargs < 1) {
        LOGE(adbg, 0x42, "no function name. from cfd %d\n", cfd);
        return -1;
    }
    for (i = 0; i < nargs; i++) {
        if (args[i].type != ipc_argtype_str) {
            LOGE(adbg, 0x4a, "%s arg%d is not string. %d\n", name, i, args[i].type);
            return -1;
        }
    }
    argv = calloc(nargs + 1, sizeof(*argv));
    if (!argv) {
        LOGE(adbg, 0x54, "nomem\n");
        return -1;
    }
    for (i = 0; i < nargs; i++) {
        argv[i] = args[i].s;
        LOGD(adbg, 0x5b, "callv args%d \"%s\"\n", i, argv[i]);
    }
    argv[nargs] = NULL;

    fn = (int (*)(int, char **))dlsym(dbg->dl, argv[0]);
    if (!fn) {
        LOGE(adbg, 0x62, "unknown symbol. %s\n", argv[0]);
        free(argv);
        return -1;
    }
    dbg->caller = pthread_self();
    old = logm_message_hook(adebug_log_hook, dbg);
    ret = fn(nargs, argv);
    logm_message_hook(old, NULL);
    dbg->caller = 0;
    free(argv);
    return ret;
}

/* @0x1cb3c */
int adebug_set_aimc(struct aimc *imc)
{
    adebug_aimc = imc;
    return 0;
}

/* @0x1cb4c */
struct adebug *adebug_create(struct aipc *ipc)
{
    struct adebug *dbg = calloc(1, sizeof(*dbg));

    if (!dbg) {
        LOGE(adbg, 0xa8, "no mem..\n");
        return NULL;
    }
    dbg->dl = dlopen(NULL, 0);
    dbg->ipc = ipc;
    aipc_register_command(ipc, "callv", cmd_callv, dbg);
    aipc_register_command(ipc, "set_prop", cmd_set_prop, dbg);
    return dbg;
}

/* @0x1cbd8 */
void adebug_destroy(struct adebug *dbg)
{
    dlclose(dbg->dl);
    free(dbg);
}

/* ------------------------------------------------------------------------- */
/* registry walkers                                                          */
/* ------------------------------------------------------------------------- */
static inline int uid_is_internal(unsigned int uid)
{
    return !(uid & 0xff0000) && (uid & 0xffff) >= 0xf000;
}

/* @0x26ea0: fn(m) for every instance of impl whose internal-ness matches */
int foreach_impl_instances(int (*fn)(struct aud_module *m), struct aud_module_impl *impl,
                           int internal)
{
    struct link *l;

    for_each_link(l, &impl->instances) {
        struct aud_module *m = link_container(struct aud_module, instance_link, l);

        if (uid_is_internal(m->uid) == internal)
            fn(m);
    }
    return 0;
}

/* @0x26f4c: debug_registry_dump callback */
int dump_instance(struct aud_module *m)
{
    struct link *l, *next;

    logm_message("instance %p\n", m);
    logm_message("uid=%08x\n", m->uid);
    logm_message("implement=%p(%s)\n", m->impl, m->impl->name);
    logm_message("input ports..\n");
    pthread_mutex_lock(&m->input_port_root.rlock);
    for (l = m->input_port_root.nodes.next, next = l->next; l != &m->input_port_root.nodes;
         l = next, next = next->next) {
        struct aud_port *p = link_container(struct aud_link_node, root_link, l)->data;

        logm_message("  port=%p\n", p);
        logm_message("  number=%d\n", p->number);
        logm_message("  pending_obj_alloced=%d\n", p->pending_obj_alloced);
        logm_message("  pending_obj_num=%d\n", p->pending_obj_num);
        logm_message("  legacy_buf=%p\n", p->legacy_buf);
        logm_message("  up_peer=%p\n", p->up_peer);
        logm_message("  flag=%x\n", p->flag);
    }
    pthread_mutex_unlock(&m->input_port_root.rlock);

    logm_message("ouput ports..\n");
    pthread_mutex_lock(&m->output_port_root.rlock);
    for (l = m->output_port_root.nodes.next, next = l->next; l != &m->output_port_root.nodes;
         l = next, next = next->next) {
        struct aud_port *p = link_container(struct aud_link_node, root_link, l)->data;
        struct link *d, *dnext;

        logm_message("  port=%p\n", p);
        logm_message("  number=%d\n", p->number);
        logm_message("  pending_obj_alloced=%d\n", p->pending_obj_alloced);
        logm_message("  pending_obj_num=%d\n", p->pending_obj_num);
        logm_message("  legacy_buf=%p\n", p->legacy_buf);
        logm_message("  down_peers..\n");
        pthread_mutex_lock(&p->down_peer_root.rlock);
        for (d = p->down_peer_root.nodes.next, dnext = d->next; d != &p->down_peer_root.nodes;
             d = dnext, dnext = dnext->next)
            logm_message("    port=%p\n", link_container(struct aud_link_node, root_link, d)->data);
        pthread_mutex_unlock(&p->down_peer_root.rlock);
        logm_message("  flag=%x\n", p->flag);
    }
    pthread_mutex_unlock(&m->output_port_root.rlock);
    logm_message("\n");
    return 0;
}

/* @0x27130: dot edges for every output -> input connection of m */
int dot_connections(struct aud_module *m)
{
    struct link *l, *next;

    pthread_mutex_lock(&m->output_port_root.rlock);
    for (l = m->output_port_root.nodes.next, next = l->next; l != &m->output_port_root.nodes;
         l = next, next = next->next) {
        struct aud_port *out = link_container(struct aud_link_node, root_link, l)->data;
        struct link *d, *dnext;

        pthread_mutex_lock(&out->down_peer_root.rlock);
        for (d = out->down_peer_root.nodes.next, dnext = d->next;
             d != &out->down_peer_root.nodes; d = dnext, dnext = dnext->next) {
            struct aud_port *in = link_container(struct aud_link_node, root_link, d)->data;

            logm_message(" %s_%08x_%p_out%x -> %s_%08x_%p_in%x [style=\"dashed\"]\n",
                         out->module->impl->name, out->module->uid, out, out->number,
                         in->module->impl->name, in->module->uid, in, in->number);
        }
        pthread_mutex_unlock(&out->down_peer_root.rlock);
    }
    pthread_mutex_unlock(&m->output_port_root.rlock);
    return 0;
}

/* @0x272c0: debug_bin "1" */
int bin_remove_internal(struct aud_module *m)
{
    if (uid_is_internal(m->uid))
        send_list_internal_module(m->aimc, m->impl->name, m->uid, 0);
    return 0;
}

/* @0x272e4: debug_bin "0" */
int bin_add_internal(struct aud_module *m)
{
    if (uid_is_internal(m->uid))
        send_list_internal_module(m->aimc, m->impl->name, m->uid, 1);
    return 0;
}

/* @0x27310: static implementations, then registered classes (stops on a negative result) */
void foreach_all_instances(int (*fn)(struct aud_module *m), int internal)
{
    struct link *l;

    for_each_link(l, &module_implements)
        foreach_impl_instances(fn, link_container(struct aud_module_impl, implement_link, l),
                               internal);
    for_each_link(l, &mreg_classes) {
        struct aud_object_class_desc *desc =
            link_container(struct aud_object_class_desc, mreg_list, l);

        if (foreach_impl_instances(fn, (struct aud_module_impl *)aud_object_class_get(desc),
                                   internal) < 0)
            return;
    }
}

/* ------------------------------------------------------------------------- */
/* callv entry points                                                        */
/* ------------------------------------------------------------------------- */

/* @0x2737c: "class" = class hierarchy, "impls" = implementation names */
int debug_mreg(int argc, char **argv)
{
    struct link *l;

    (void)argc;
    if (!argv[1])
        return 0;
    if (!strcmp(argv[1], "class")) {
        logm_message("dump all implement class..\n");
        for_each_link(l, &mreg_classes) {
            struct aud_object_class *k =
                aud_object_class_get(link_container(struct aud_object_class_desc, mreg_list, l));
            struct aud_object_class_desc *d = k->desc, *p;
            int indent = 0;

            logm_message("class %s\n", d->name);
            for (p = d->parent; p; p = p->parent) {
                indent += 2;
                logm_message("%*s<- inherit %s\n", indent, "", p->name);
            }
        }
    } else if (!strcmp(argv[1], "impls")) {
        logm_message("dump all implements..\n");
        for_each_link(l, &module_implements)
            logm_message("name:%s\n",
                         link_container(struct aud_module_impl, implement_link, l)->name);
        for_each_link(l, &mreg_classes) {
            struct aud_module_impl *impl = (struct aud_module_impl *)aud_object_class_get(
                link_container(struct aud_object_class_desc, mreg_list, l));

            logm_message("name:%s\n", impl->name);
        }
    } else {
        return 0;
    }
    logm_message("done.\n");
    return 0;
}

/* @0x27790: "internal_list" | "initial_log_level <impl> <level>" */
int debug_reg(int argc, char **argv)
{
    struct aud_module_impl *impl;
    struct link *l;

    (void)argc;
    if (!argv[1])
        return 0;
    if (!strcmp(argv[1], "internal_list")) {
        logm_message("list internal implements..\n");
        for_each_link(l, &internal_instances) {
            struct aud_internal_inst *e = link_container(struct aud_internal_inst, list, l);

            logm_message("  %05d, 0x%08x, %s\n", e->iindex, e->module->uid, e->module->name);
        }
        logm_message("done.\n");
        return 0;
    }
    if (strcmp(argv[1], "initial_log_level") || !argv[2] || !argv[3])
        return 0;
    logm_message("initial log level for \"%s\". %s\n", argv[2], argv[3]);
    impl = ModuleRegistry_SearchModuleImpl(argv[2]);
    if (!impl) {
        logm_message("no seach implement. %s\n", argv[2]);
        return 0;
    }
    impl->initial_log_mask = (1u << strtol(argv[3], NULL, 10)) - 1;
    return 0;
}

/* @0x27fe8: there are no registry options in this build */
int debug_registry(int argc, char **argv)
{
    const struct audutil_opt *o;

    (void)argc;
    o = audutil_set_opts(registry_debug_opts, argv + 1);
    if (o)
        audutil_print_opt(o);
    return 0;
}

/* @0x28004 */
int debug_registry_dump(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    foreach_all_instances(dump_instance, 0);
    return 0;
}

/* @0x28018 */
int _dot_registry_connection(void)
{
    foreach_all_instances(aud_module_dot, 0);
    foreach_all_instances(dot_connections, 0);
    return 0;
}

/* @0x28044: graphviz of all (non-internal) instances and their connections */
int dot_registry(void)
{
    logm_message("digraph {\n");
    logm_message(" rankdir=\"LR\";\n");
    _dot_registry_connection();
    logm_message("}\n");
    return 0;
}

/* @0x28074: re-announce (0) or withdraw (1) all internal instances to the bin module */
int debug_bin(int argc, char **argv)
{
    unsigned long op;

    if (argc < 2) {
        logm_message("not enough arguments.. 0 : add internal module, 1 : remove internal module\n");
        return -1;
    }
    op = strtoul(argv[1], NULL, 0);
    if (op == 0)
        foreach_all_instances(bin_add_internal, 1);
    else if (op == 1)
        foreach_all_instances(bin_remove_internal, 1);
    else
        logm_message("%d is not valid\n", (int)op);
    logm_message("done\n");
    return 0;
}

/* @0x280d8: "<tag> <uid>": print all properties of an instance */
int audreg_print_property(int argc, char **argv)
{
    struct aud_module *m;
    char *s;

    if (argc < 3)
        return -1;
    m = ModuleRegistry_SearchInstByUid(strtoul(argv[2], NULL, 0));
    logm_message("%s: start\n", argv[1]);
    if (!m) {
        logm_message("no instance for uid %s\n", argv[2]);
    } else {
        s = aud_module_prop_str(m, "%s=%s\n");
        logm_message("%s", s);
        free(s);
    }
    logm_message("%s: end\n", argv[1]);
    return 0;
}

/* @0x2cf44: "<switch> [value]": set/show a module.c debug switch */
int debug_module(int argc, char **argv)
{
    const struct audutil_opt *o;

    (void)argc;
    o = audutil_set_opts(aud_module_debug_opts, argv + 1);
    if (!o) {
        logm_message("unknown.\n");
        return 0;
    }
    if (o->type == opt_bool)
        logm_message("%s %s\n", o->name, *(bool *)o->ptr ? "on" : "off");
    else if (o->type == opt_int)
        logm_message("%s %d\n", o->name, *(int *)o->ptr);
    return 0;
}

/* @0x30a74: "class": dump all object classes */
int debug_object(int argc, char **argv)
{
    (void)argc;
    if (!argv[1])
        return 0;
    if (!strcmp(argv[1], "class")) {
        logm_message("dump all class..\n");
        aud_object_class_foreach(dump_class, NULL);
        logm_message("done.\n");
    }
    return 0;
}
