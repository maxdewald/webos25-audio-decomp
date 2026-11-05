/*
 * libaa "arocket": gst-launch-like textual pipeline description on top of apipeline.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 * Evidence: decomp/libaa.so.2.0.0.c @0x1bd34..0x1c7fa, logmm module "aud_pipeline",
 * __func__ strings "get_instance", "assign_name", "assign_event", "event_handler",
 * "_exception"; assignment table @0x4c068 = { "name", assign_name, "event", assign_event }.
 *
 * Grammar (tokens are whitespace separated, see aabase_string_vector for quoting):
 *   element    impl[@proc][.port]   create impl (preferred processor proc, default: the
 *                                   processor of the previous element, else any) and make it
 *                                   the current element; ".port" selects the port used by
 *                                   the next/previous link (default 0)
 *              alias[.port]         re-use an element named earlier with name=alias
 *              %legacy              legacy (ADEC module list) name, existing instance
 *              %%legacy             legacy name, instance created via aimc_lmodule_create
 *              ^uid                 existing instance by numeric uid
 *   link       !                    connect current.port -> next element.port   (downstream)
 *              ^                    connect next element.port -> current.port   (upstream)
 *   property   key=value            sent to the current element as ADEC_CMD_SET_PROP
 *   assignment name:alias           give the current element an alias
 *              event:func(arg)@ev   on named event "ev" from the element, call the global
 *                                   symbol arocket_handle_event_<func>(desc, data, size)
 *                                   (built-ins: print -> puts(arg), exit -> exit(atoi(arg)))
 *   end        -                    stop parsing
 * Any error longjmp()s out ("rocket failed.") and returns -1; instances created so far stay.
 * Every created element is followed by aimc_handler_sync().
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#define ADEC_CMD_SET_PROP 0x10180004

struct arocket_ctx {
    jmp_buf env;
};

typedef int (*arocket_assign_cb)(struct apipeline *pipe, struct arocket_ctx *ctx,
                                 struct apipeline_element *el, const char *key, char *value);

void _exception(struct arocket_ctx *ctx, const char *func, int line, const char *fmt, ...)
    __attribute__((noreturn, format(printf, 4, 5)));

/* @0x1be9c */
void _exception(struct arocket_ctx *ctx, const char *func, int line, const char *fmt, ...)
{
    va_list ap;
    char *msg;

    va_start(ap, fmt);
    vasprintf(&msg, fmt, ap);
    va_end(ap);
    LOGE(aud_pipeline, 0x37, "exception at %s.%d\n", func, line);
    LOGE(aud_pipeline, 0x38, "message : %s", msg);
    free(msg);
    longjmp(ctx->env, -1);
}

#define THROW(ctx, line, ...) _exception(ctx, __func__, line, __VA_ARGS__)

/* @0x1bd34: named event callback registered by assign_event */
int event_handler(void *priv, void *data, unsigned int size)
{
    struct apipeline_event_desc *d = priv;
    int (*fn)(struct apipeline_event_desc *d, void *data, unsigned int size);
    char *sym;

    LOGD(aud_pipeline, 0xb5, "got event, \"%s\", from %s(%x) param %p(%d). call %s(\"%s\")\n",
         d->event_name, d->inst->name, d->inst->uid, data, size, d->func_name, d->func_arg);
    if (asprintf(&sym, "arocket_handle_event_%s", d->func_name) < 0) {
        LOGE(aud_pipeline, 0xba, "nomem. %s\n", d->func_name);
        return -1;
    }
    fn = (int (*)(struct apipeline_event_desc *, void *, unsigned int))dlsym(NULL, sym);
    if (!fn) {
        LOGE(aud_pipeline, 0xc1, "\"arocket_handle_event_%s\" not found\n", d->func_name);
        free(sym);
        return -1;
    }
    free(sym);
    fn(d, data, size);
    return 0;
}

/* @0x1be3c: "name:alias" */
int assign_name(struct apipeline *pipe, struct arocket_ctx *ctx, struct apipeline_element *el,
                const char *key, char *value)
{
    (void)pipe;
    (void)ctx;
    (void)key;
    free(el->alias);
    el->alias = strdup(value);
    LOGD(aud_pipeline, 0x102, "nick '%s' at '%s'\n", el->alias, el->name);
    return 0;
}

/* @0x1bf2c: "event:func(arg)@event_name" */
int assign_event(struct apipeline *pipe, struct arocket_ctx *ctx, struct apipeline_element *el,
                 const char *key, char *value)
{
    struct apipeline_event_desc *d;
    char *at, *paren, *arg, *close;

    (void)key;
    LOGD(aud_pipeline, 0xd5, "assign event. %s\n", value);
    at = strrchr(value, '@');
    if (!at)
        THROW(ctx, 0xda, "no '@' for event name\n");
    *at = '\0';
    paren = strchr(value, '(');
    if (!paren)
        THROW(ctx, 0xe1, "no '(' for function argument\n");
    *paren = '\0';
    arg = paren + 1;
    close = strchr(arg, ')');
    if (close)
        *close = '\0';
    LOGD(aud_pipeline, 0xeb, "register event, \"%s\", to %s. call %s(\"%s\")\n", at + 1,
         el->name, value, arg);

    /* [observed] the strings point into the caller's token vector, which arocket_desc
     * frees after parsing, so they dangle once the event fires (unless argv outlives it). */
    d = calloc(1, sizeof(*d));
    d->func_arg = arg;
    d->event_name = at + 1;
    d->func_name = value;
    link_add(&el->events, &d->list);
    d->pipe = pipe;
    d->inst = el;
    aimc_register_named_event(pipe->imc, d->event_name, el->uid, 0, event_handler, d);
    return 0;
}

/* @0x1c060 */
void arocket_handle_event_print(struct apipeline_event_desc *d, void *data, unsigned int size)
{
    LOGD(aud_pipeline, 0xa1, "print(%s). param %s(%d)\n", d->func_arg, (char *)data, size);
    puts(d->func_arg);
}

/* @0x1c0c0 */
void arocket_handle_event_exit(struct apipeline_event_desc *d, void *data, unsigned int size)
{
    LOGN(aud_pipeline, 0xa9, "exit(%s). param %s(%d)\n", d->func_arg, (char *)data, size);
    exit(strtol(d->func_arg, NULL, 10));
}

static const struct {
    const char *key;
    arocket_assign_cb fn;
} assignments[] = {
    { "name",  assign_name },
    { "event", assign_event },
};

/* inlined into arocket_descv (own __func__ "get_instance") */
static inline struct apipeline_element *get_instance(struct apipeline *pipe,
                                                     struct arocket_ctx *ctx, const char *tok,
                                                     struct apipeline_element *prev,
                                                     long *port)
{
    struct apipeline_element *el;
    const char *impl;
    char *name, *p;
    int proc = -1;

    if (prev) {
        proc = prev->uid >> 24;
        if (!proc)      /* legacy module id: look up its processor */
            proc = ModuleList_FindModule(prev->uid);
    }
    name = strdup(tok);
    *port = 0;
    p = strchr(name, '.');
    if (p) {
        *p = '\0';
        *port = strtol(p + 1, NULL, 10);
    }

    el = apipeline_search(pipe, name);
    if (el) {
        free(name);
        return el;
    }

    if (name[0] == '%') {
        bool create = name[1] == '%';
        unsigned int mod;

        impl = name + (create ? 2 : 1);
        for (mod = 1; strcmp(ModuleList_GetModuleName(mod), impl); mod++)
            if (mod + 1 == 0xc9)
                THROW(ctx, 0x6f, "unknown legacy module name. %s\n", impl);
        if (create) {
            mod = aimc_lmodule_create(pipe->imc, mod);
            if (!mod)
                THROW(ctx, 0x77, "cannot create legacy module. %s\n", impl);
        }
        el = _apipeline_add(pipe, impl, mod, true);
    } else if (name[0] == '^') {
        impl = name + 1;
        el = _apipeline_add(pipe, impl, strtoul(impl, NULL, 0), true);
    } else {
        p = strchr(name, '@');
        if (p) {
            *p = '\0';
            proc = strtol(p + 1, NULL, 10);
        }
        impl = name;
        el = apipeline_add(pipe, name, proc);
    }
    if (!el)
        THROW(ctx, 0x95, "cannot make instance, \"%s\"\n", tok);
    LOGD(aud_pipeline, 0x98, "create instance '%s', %x\n", impl, el->uid);
    free(name);
    return el;
}

enum link_dir { LINK_NONE, LINK_DOWN, LINK_UP };

/* @0x1c118: argv is a NULL terminated token vector */
int arocket_descv(struct apipeline *pipe, void *priv, arocket_assign_fn assign, char **argv)
{
    struct apipeline_element *cur = NULL, *prev = NULL;
    long cur_port = 0, prev_port = 0;
    enum link_dir dir = LINK_NONE;
    struct arocket_ctx ctx;
    int ret;

    (void)priv;
    (void)assign;
    memset(&ctx, 0, sizeof(ctx));
    if (!pipe) {
        LOGE(aud_pipeline, 0x121, "no pipe\n");
        return -1;
    }
    ret = setjmp(ctx.env);
    if (ret) {
        LOGE(aud_pipeline, 0x129, "rocket failed.\n");
        return ret;
    }

    for (; *argv; argv++) {
        char *tok = *argv, *sep;

        if (!strcmp(tok, "-"))
            return 0;
        if (!strcmp(tok, "!") || !strcmp(tok, "^")) {
            if (!cur)
                THROW(&ctx, tok[0] == '!' ? 0x138 : 0x144, "no instance for pipe\n");
            dir = tok[0] == '!' ? LINK_DOWN : LINK_UP;
            prev = cur;
            prev_port = cur_port;
            cur = NULL;
            cur_port = 0;
            continue;
        }

        if (cur && strchr(tok, '=')) {
            size_t n = strlen(tok);
            unsigned int *cmd;

            LOGD(aud_pipeline, 0x152, "'%s' param : '%s'\n", cur->name, tok);
            cmd = calloc(1, n + 5);
            memcpy(cmd + 1, tok, n);
            cmd[0] = n;
            ((char *)(cmd + 1))[n] = '\0';
            aimc_send_command(pipe->imc, ADEC_CMD_SET_PROP, aimc_get_processor_id(pipe->imc),
                              cur->uid, cmd, n + 5);
            free(cmd);
            continue;
        }

        sep = strchr(tok, ':');
        if (sep) {
            unsigned int i;

            *sep = '\0';
            LOGD(aud_pipeline, 0x170, "assign.. %s:%s\n", tok, sep + 1);
            if (!cur)
                THROW(&ctx, 0x173, "no instance for assign\n");
            if (!strcmp(tok, assignments[0].key))
                i = 0;
            else if (!strcmp(tok, assignments[1].key))
                i = 1;
            else
                THROW(&ctx, 0x17e, "unknown assignment. %s\n", tok);
            assignments[i].fn(pipe, &ctx, cur, tok, sep + 1);
            continue;
        }

        cur = get_instance(pipe, &ctx, tok, prev, &cur_port);
        if (dir == LINK_DOWN) {
            LOGD(aud_pipeline, 399, "connect instance '%s.%d'->'%s.%d'\n", prev->name,
                 (int)prev_port, cur->name, (int)cur_port);
            if (aimc_module_connect(pipe->imc, prev->uid, prev_port, cur->uid, cur_port))
                THROW(&ctx, 0x195, "can't connect instance %s.%d -> %s.%d\n", prev->name,
                      (int)prev_port, cur->name, (int)cur_port);
            cur_port = 0;
            dir = LINK_NONE;
        } else if (dir == LINK_UP) {
            LOGD(aud_pipeline, 0x19d, "connect instance '%s.%d'->'%s.%d'\n", cur->name,
                 (int)cur_port, prev->name, (int)prev_port);
            if (aimc_module_connect(pipe->imc, cur->uid, cur_port, prev->uid, prev_port))
                THROW(&ctx, 0x1a3, "can't connect instance %s.%d -> %s.%d\n", cur->name,
                      (int)cur_port, prev->name, (int)prev_port);
            cur_port = 0;
            dir = LINK_NONE;
        } else {
            aimc_handler_sync(pipe->imc);
        }
    }
    return 0;
}

/* @0x1c6d4 */
int arocket_desc(struct apipeline *pipe, void *priv, arocket_assign_fn assign, const char *desc)
{
    char **v = aabase_string_vector(desc);
    int ret;

    if (!v) {
        LOGE(aud_pipeline, 0x1c0, "Oops\n");
        return -1;
    }
    ret = arocket_descv(pipe, priv, assign, v);
    aabase_free_vector(v);
    return ret;
}

/* @0x1c74c */
int arocket_vdescf(struct apipeline *pipe, void *priv, arocket_assign_fn assign,
                   const char *descf, va_list ap)
{
    char *desc = NULL;
    int ret;

    if (vasprintf(&desc, descf, ap) < 0) {
        LOGE(aud_pipeline, 0x1d4, "Oops?\n");
        return -1;
    }
    ret = arocket_desc(pipe, priv, assign, desc);
    free(desc);
    return ret;
}

/* @0x1c7dc */
int arocket_descf(struct apipeline *pipe, void *priv, arocket_assign_fn assign,
                  const char *descf, ...)
{
    va_list ap;
    int ret;

    va_start(ap, descf);
    ret = arocket_vdescf(pipe, priv, assign, descf, ap);
    va_end(ap);
    return ret;
}
