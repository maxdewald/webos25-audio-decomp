/*
 * libaa module registry (implementations + instances on THIS processor) and the
 * connector command handler (processor-local module connect/disconnect).
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 * Evidence: decomp/libaa.so.2.0.0.c @0x26efc, @0x271cc, @0x274a4..0x27fb8, @0x28150..0x2860c,
 * logmm modules "man_registry" / "mod_connector"; handlers @0x27d5c (ModuleRegistry_ProcessCmd)
 * and @0x283bc (Connector_Cmd) read from disassembly.
 * Debug commands (debug_mreg, debug_reg, dot_registry, ...) live in debug.c.
 *
 * Instance uids: (processor_id << 24) | local. Instances created without a uid by this
 * processor ("internal" modules) get local = 0xf000 + iindex, iindex 1..0xfff.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

LOGMM_MODULE_DEFINE(man_registry);
LOGMM_MODULE_DEFINE(mod_connector);

/* registry command uids (low byte of the handler uid) */
#define REGISTRY_HANDLER    1
#define CONNECTOR_HANDLER   2
#define REGISTRY_EVENT_UID  3

/* commands to the registry handler (proc<<24 | 1) */
#define REGCMD_CREATE_MODULE    0x80f00024
#define REGCMD_DESTROY_MODULE   0x80f10004
#define REGCMD_CREATE_NAMED     0x80f20008

/* commands to the connector handler (proc<<24 | 2), param = {uid_out, port_out, uid_in, port_in} */
#define CONNCMD_CONNECT         0xf1000010
#define CONNCMD_DISCONNECT      0xf1010010

/* registered implementations (struct aud_module_impl::implement_link) */
struct link module_implements = LINK_INITIALIZER(module_implements);
/* class descriptors added with aud_mregistry_add_class (aud_object_class_desc::mreg_list) */
struct link mreg_classes = LINK_INITIALIZER(mreg_classes);
/* struct aud_internal_inst */
struct link internal_instances = LINK_INITIALIZER(internal_instances);
int internal_index_last;

#define IMPL_OF(l)   link_container(struct aud_module_impl, implement_link, l)
#define CDESC_OF(l)  link_container(struct aud_object_class_desc, mreg_list, l)
#define INST_OF(l)   link_container(struct aud_module, instance_link, l)

/* @0x274a4 */
void aud_mregistry_add_class(struct aud_object_class_desc *desc)
{
    link_add(&mreg_classes, &desc->mreg_list);
}

/* @0x274cc: static implementation; instances get log mask 7 (error..noti) */
void ModuleRegistry_Register(struct aud_module_impl *impl)
{
    link_init(&impl->instances);
    impl->initial_log_mask = 7;
    link_add_tail(&module_implements, &impl->implement_link);
}

/* @0x274fc: register a renamed copy of a template implementation */
void ModuleRegistry_RegisterNamef(struct aud_module_impl *templ, void *priv,
                                  const char *name_fmt, ...)
{
    struct aud_module_impl *impl;
    va_list ap;
    char *name;

    va_start(ap, name_fmt);
    name = audutil_strdup_vprintf(name_fmt, ap);
    va_end(ap);
    if (!name) {
        LOGE(man_registry, 0xb0, "no name??\n");
        return;
    }
    impl = malloc(sizeof(*impl));
    if (!impl) {
        LOGE(man_registry, 0xb8, "no mem???\n");
        free(name);
        return;
    }
    memcpy(impl, templ, sizeof(*impl));
    impl->name = name;
    impl->impl_priv = priv;
    ModuleRegistry_Register(impl);
}

/* @0x275b4 */
struct aud_module *ModuleRegistry_SearchInstByUid(unsigned int uid)
{
    struct link *i, *l;

    for_each_link(i, &module_implements)
        for_each_link(l, &IMPL_OF(i)->instances)
            if (INST_OF(l)->uid == uid)
                return INST_OF(l);
    for_each_link(i, &mreg_classes) {
        struct aud_module_impl *impl =
            (struct aud_module_impl *)aud_object_class_get(CDESC_OF(i));

        if (!impl)
            return NULL;
        for_each_link(l, &impl->instances)
            if (INST_OF(l)->uid == uid)
                return INST_OF(l);
    }
    return NULL;
}

/* @0x27658 */
struct aud_module *ModuleRegistry_SearchInstByName(const char *name)
{
    struct link *i, *l;

    for_each_link(i, &module_implements)
        for_each_link(l, &IMPL_OF(i)->instances)
            if (!strcmp(INST_OF(l)->name, name))
                return INST_OF(l);
    for_each_link(i, &mreg_classes) {
        struct aud_module_impl *impl =
            (struct aud_module_impl *)aud_object_class_get(CDESC_OF(i));

        if (!impl)
            return NULL;
        for_each_link(l, &impl->instances)
            if (!strcmp(INST_OF(l)->name, name))
                return INST_OF(l);
    }
    return NULL;
}

/* @0x27710: static implementations first, then classes (aud_object_class_gets inlined) */
struct aud_module_impl *ModuleRegistry_SearchModuleImpl(const char *name)
{
    struct link *i;

    for_each_link(i, &module_implements)
        if (!strcmp(name, IMPL_OF(i)->name))
            return IMPL_OF(i);
    for_each_link(i, &mreg_classes)
        if (!strcmp(name, CDESC_OF(i)->name))
            return (struct aud_module_impl *)aud_object_class_gets(name);
    return NULL;
}

/* inlined: next free internal index, 1..0xfff, 0 when exhausted */
static inline int get_empty_internal_index(void)
{
    int idx = internal_index_last + 1;
    struct link *l;

    LOGI(man_registry, 0x14d, "internal_index_last = 0x%04x", internal_index_last);
    do {
        if (idx == 0x1000)
            idx = 1;
        for_each_link(l, &internal_instances)
            if (link_container(struct aud_internal_inst, list, l)->iindex == idx)
                break;
        if (l == &internal_instances) {
            if (idx == internal_index_last)
                break;
            LOGI(man_registry, 0x16d, "done iindex = 0x%04x", idx);
            internal_index_last = idx;
            return idx;
        }
        idx++;
    } while (idx != internal_index_last);
    LOGE(man_registry, 0x169, "too many instances for internal module!!\n");
    return 0;
}

/* @0x27848: instantiate impl_name on this processor. uid 0 = allocate an internal uid.
 * The new instance also becomes the aimc command handler for its uid.
 */
struct aud_module *aud_registry_create_instance_param(struct aimc *imc, const char *impl_name,
                                                      unsigned int uid, const char *param)
{
    struct aud_module_impl *impl = ModuleRegistry_SearchModuleImpl(impl_name);
    struct aud_internal_inst *ii;
    struct aud_module *m;
    int idx;

    if (!impl) {
        LOGE(man_registry, 0x17f, "unable to find module implementation\n");
        return NULL;
    }
    if (uid) {
        LOGI(man_registry, 0x192, "create %s(%08x), %s\n", impl_name, uid,
             param ? param : "<null>");
        m = aud_module_constructor(imc, impl, uid, param);
    } else {
        idx = get_empty_internal_index();
        if (!idx) {
            LOGE(man_registry, 0x18b, "no more internal instance index for %s. %d\n", impl_name,
                 internal_index_last);
            return NULL;
        }
        uid = (aimc_get_processor_id(imc) << 24) | (0xf000 + idx);
        LOGI(man_registry, 0x192, "create %s(%08x), %s\n", impl_name, uid,
             param ? param : "<null>");
        m = aud_module_constructor(imc, impl, uid, param);
        ii = calloc(1, sizeof(*ii));
        if (!ii) {
            LOGE(man_registry, 0x19c, "no mem\n");
        } else {
            ii->iindex = idx;
            ii->module = m;
            link_add(&internal_instances, &ii->list);
        }
    }
    aimc_register_command_handler(imc, uid, impl->cmd, m);
    return m;
}

/* @0x27b2c */
struct aud_module *aud_registry_create_instance(struct aimc *imc, const char *impl_name,
                                                unsigned int uid)
{
    return aud_registry_create_instance_param(imc, impl_name, uid, NULL);
}

/* @0x27b34: "impl?param" */
int regcmd_create_module(struct registry *r, const char *name, unsigned int uid)
{
    const char *q = strchr(name, '?');
    struct aud_module *m;

    if (!q) {
        m = aud_registry_create_instance(r->imc, name, uid);
    } else {
        size_t n = q - name;
        char *impl = malloc(n + 1);

        if (!impl) {
            LOGE(man_registry, 0x1f2, "no mem\n");
            goto fail;
        }
        strncpy(impl, name, n);
        impl[n] = '\0';
        m = aud_registry_create_instance_param(r->imc, impl, uid, q + 1);
        free(impl);
    }
    if (m)
        return 0;
fail:
    LOGE(man_registry, 0x1f9, "cannot create \"%s\"\n", name);
    return -1;
}

/* @0x27c08: aud_module_destructor is inlined in the binary */
void aud_registry_destroy_instance(struct aimc *imc, struct aud_module *m)
{
    unsigned int uid;

    LOGI(man_registry, 0x1b4, "destroy %s(%08x)\n", m->impl->name, m->uid);
    aimc_unregister_command_handler(imc, m->uid);
    uid = m->uid;
    if (!(uid & 0xff0000) && (uid & 0xffff) >= 0xf000 &&
        (unsigned int)aimc_get_processor_id(imc) == uid >> 24) {
        int idx = (m->uid & 0xffff) - 0xf000;
        struct link *l;

        for_each_link(l, &internal_instances) {
            struct aud_internal_inst *ii = link_container(struct aud_internal_inst, list, l);

            if (ii->iindex != idx)
                continue;
            if (ii->module != m) {
                LOGE(man_registry, 0x1c7, "wrong inst, iindex. (%p, %p), (%d, %d), 0x%08x\n",
                     m, ii->module, idx, idx, uid);
                LOGE(man_registry, 0x1c9, "%s, %s\n", ii->module->name, m->name);
            }
            link_del(&ii->list);
            free(ii);
            goto destroy;
        }
        LOGE(man_registry, 0x1d4, "unknown internal module. %s(0x%08x)\n", m->name, uid);
    }
destroy:
    aud_module_destructor(m);
}

/* @0x26efc: announce one implementation (and its per-port buffer sizes) to aimc */
int add_implement(struct aimc *imc, struct aud_module_impl *impl)
{
    struct aud_module_mem_def *def = impl->mem_def;
    int n;

    aimc_add_implement(imc, impl->name, 1);
    if (!def || !def[0].max_num_au)
        return 0;
    for (n = 0; def[n + 1].max_num_au; n++)
        ;
    for (;; n--) {
        aimc_add_port_buf_def(imc, impl->name, n, impl->mem_def[n].max_num_au,
                              impl->mem_def[n].max_access_size);
        if (!n)
            break;
    }
    return 0;
}

/* @0x271cc: raise "bin" named event announcing an internal module add/remove */
void send_list_internal_module(struct aimc *imc, const char *name, unsigned int uid, int add)
{
    struct {
        char name[16];
        unsigned int uid;
        int proc;
        int add;
    } msg;
    int proc;

    memset(&msg, 0, sizeof(msg));
    proc = aimc_get_processor_id(imc);
    if (!strcmp(name, "binout")) {
        LOGI(man_registry, 0x30f, "skip binout...\n");
        return;
    }
    LOGI(man_registry, 0x313, "%s %s (0x%x)\n", add ? "Add" : "Remove", name, uid);
    msg.uid = uid;
    strncpy(msg.name, name, sizeof(msg.name) - 1);
    msg.proc = aimc_get_processor_id(imc);
    msg.add = add;
    aimc_raise_named_event(imc, proc << 24 | REGISTRY_EVENT_UID, "bin", &msg, sizeof(msg));
}

/* @0x27d5c: aimc command handler for uid (proc << 24 | 1) */
int ModuleRegistry_ProcessCmd(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    struct registry *r = arg;
    unsigned int *p = param;
    struct aud_module *m;

    LOGT(man_registry, 0x205, "received: %08x\n", cmd);
    switch (cmd) {
    case REGCMD_DESTROY_MODULE:
        m = ModuleRegistry_SearchInstByUid(p[0]);
        if (!m) {
            LOGE(man_registry, 0x227, "unable to find module. 0x%08x\n", p[0]);
            return -3;
        }
        LOGI(man_registry, 0x22a, "destroy %s(%08x)\n", m->impl->name, m->uid);
        aud_registry_destroy_instance(r->imc, m);
        return 0;
    case REGCMD_CREATE_NAMED:
        /* {uid, name_len, name[name_len]} */
        if (p[1] + 8 > len) {
            LOGE(man_registry, 0x215,
                 "too small param_len. uid 0x%08x, name_len %d, param_len %d\n", p[0], p[1],
                 len);
            return -1;
        }
        return regcmd_create_module(r, (const char *)(p + 2), p[0]);
    case REGCMD_CREATE_MODULE:
        /* {char name[0x20], uid} */
        return regcmd_create_module(r, (const char *)p, p[8]);
    default:
        /* [observed] tail call with the registry posing as a module; only reaches the
         * logging/default branches of aud_module_default_command */
        return aud_module_default_command((struct aud_module *)r, cmd, param, len);
    }
}

/* @0x27ecc */
struct registry *ModuleRegistry_Init(struct aimc *imc)
{
    struct registry *r = malloc(sizeof(*r));
    struct link *l;

    r->imc = imc;
    aimc_add_processor(imc);
    for_each_link(l, &module_implements)
        add_implement(imc, IMPL_OF(l));
    for_each_link(l, &mreg_classes)
        if (add_implement(imc, (struct aud_module_impl *)aud_object_class_get(CDESC_OF(l))) < 0)
            break;
    aimc_add_processor_done(imc);
    if (aimc_register_command_handler(imc, aimc_get_processor_id(imc) << 24 | REGISTRY_HANDLER,
                                      ModuleRegistry_ProcessCmd, r) < 0)
        LOGE(man_registry, 0x265, "Registry Command Handler Error\n");
    return r;
}

/* @0x27fb8 */
void ModuleRegistry_Cleanup(struct registry *r)
{
    struct aimc *imc;

    if (!r)
        return;
    imc = r->imc;
    if (imc) {
        aimc_unregister_command_handler(imc, aimc_get_processor_id(imc) << 24 | REGISTRY_HANDLER);
        aimc_wait_until_process_header(imc);
    }
    free(r);
}

/* ------------------------------------------------------------------------- */
/* connector                                                                 */
/* ------------------------------------------------------------------------- */

/* @0x28150: same-processor connect; the input port must not exist yet */
int aud_connector_connect(struct aud_module *out, int port_out, struct aud_module *in,
                          int port_in)
{
    struct aud_port *po, *pi;

    pi = aud_module_get_input_port(in, port_in);
    if (pi) {
        LOGE(mod_connector, 0x4b, "already used input module port (%s.%d)\n", in->name,
             pi->number);
        return -24;
    }
    po = aud_module_new_output_port(out, port_out);
    if (!po) {
        LOGE(mod_connector, 0x53, "no mem\n");
        return -20;
    }
    pi = aud_module_new_input_port(in, port_in);
    if (!pi) {
        LOGE(mod_connector, 0x5a, "no mem\n");
        return -20;
    }
    aud_module_connect(po, pi);
    LOGI(mod_connector, 0x5f, "module connected (%s.%d -> %s.%d)\n", out->name, port_out,
         in->name, port_in);
    return 0;
}

/* @0x28280 */
int aud_connector_disconnect(struct aud_module *out, int port_out, struct aud_module *in,
                             int port_in)
{
    struct aud_port *pi = aud_module_get_input_port(in, port_in);
    struct aud_port *po = aud_module_get_output_port(out, port_out);

    if (pi && po) {
        LOGI(mod_connector, 0x7a, "disconnect module %s.%d -> %s.%d\n", out->name, port_out,
             in->name, port_in);
        aud_module_disconnect(po, pi);
        return 0;
    }
    if (!pi)
        LOGE(mod_connector, 0x6f, "no input port%d on %s??\n", port_in, in->name);
    if (!po)
        LOGE(mod_connector, 0x72, "no output port%d on %s??\n", port_out, out->name);
    LOGE(mod_connector, 0x74, "module disconnecting (%s.%d, %s.%d)\n", out->name, port_out,
         in->name, port_in);
    return -1;
}

/* @0x283bc: aimc command handler for uid (proc << 24 | 2) */
int Connector_Cmd(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    unsigned int *p = param;
    struct aud_module *out, *in;

    (void)arg;
    (void)len;
    LOGI(mod_connector, 0xb4, "received command: %08x\n", cmd);
    if (cmd == CONNCMD_CONNECT) {
        out = ModuleRegistry_SearchInstByUid(p[0]);
        if (!out) {
            LOGE(mod_connector, 0x89, "unable to find output module (uid: %08x)\n", p[0]);
            return -1;
        }
        in = ModuleRegistry_SearchInstByUid(p[2]);
        if (!in) {
            LOGE(mod_connector, 0x90, "unable to find input module (uid: %08x)\n", p[2]);
            return -1;
        }
        return aud_connector_connect(out, p[1], in, p[3]);
    }
    if (cmd == CONNCMD_DISCONNECT) {
        /* inlined Connector_Cmd_Disconnect */
        LOGI(mod_connector, 0x9d, "disconnect out: %08x, in: %08x\n", p[0], p[2]);
        out = ModuleRegistry_SearchInstByUid(p[0]);
        if (!out) {
            LOGE(mod_connector, 0xa2, "unable to find output module (uid: %08x)\n", p[0]);
            return -1;
        }
        in = ModuleRegistry_SearchInstByUid(p[2]);
        if (!in) {
            LOGE(mod_connector, 0xa9, "unable to find input module (uid: %08x)\n", p[2]);
            return -1;
        }
        return aud_connector_disconnect(out, p[1], in, p[3]);
    }
    LOGE(mod_connector, 0xc5, "Unknown command\n");
    return -18;
}

/* @0x2859c */
struct aud_connector *Connector_Create(int processor_id, struct aimc *imc)
{
    struct aud_connector *c = calloc(sizeof(*c), 1);

    if (!c) {
        LOGE(mod_connector, 0xd5, "no mem\n");
        return NULL;
    }
    c->imc = imc;
    c->handler_uid = processor_id << 24 | CONNECTOR_HANDLER;
    aimc_register_command_handler(imc, c->handler_uid, Connector_Cmd, NULL);
    return c;
}

/* @0x2860c */
void Connector_Destroy(struct aud_connector *c)
{
    aimc_unregister_command_handler(c->imc, c->handler_uid);
    free(c);
}
