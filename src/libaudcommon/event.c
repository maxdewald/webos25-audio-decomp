/*
 * libaudcommon: aud_object refcounting and aud_event (typed event + named value list,
 * serialised for the IPC/IMC hop to the DSP side).
 *
 * RECONSTRUCTION from orig/usr/lib/libaudcommon.so.1.0.0
 *   sha256 a4df15e082096e6c60c7091b81f90f4fe0a6c5ae07173867ca4a0b05bfd5087f
 * Evidence: decomp/libaudcommon.so.1.0.0.c (functions 0x11cfc..0x13244), log strings and
 * __LINE__ constants from decomp/libaudcommon.so.1.0.0.strings.tsv. All logs use the
 * "aud_bufdata" logmm module.
 */
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"

static pthread_mutex_t object_lock = PTHREAD_MUTEX_INITIALIZER;

/* @0x11fc8 */
struct aud_object *aud_object_get(struct aud_object *o)
{
    pthread_mutex_lock(&object_lock);
    o->user++;
    pthread_mutex_unlock(&object_lock);
    return o;
}

/* @0x11fec: drops a reference; destroy() runs outside the lock when it hits 0 */
void aud_object_put(struct aud_object *o)
{
    unsigned int user;

    pthread_mutex_lock(&object_lock);
    if (o->user == 0) {
        o->user = (unsigned int)-1;
        pthread_mutex_unlock(&object_lock);
        LOGE(aud_bufdata, 0x47, "double free!! %p\n", o);
        return;
    }
    user = --o->user;
    pthread_mutex_unlock(&object_lock);
    if (user == 0)
        o->destroy(o);
}

/* @0x11cfc: aud_event destroy callback (no symbol; sits after logmm_printlm) */
void event_destroy(struct aud_object *obj)
{
    struct aud_event *e = (struct aud_event *)obj;

    while (!link_empty(&e->values)) {
        struct aud_event_value *v = link_first(struct aud_event_value, value_link, &e->values);

        if (v->name)
            free(v->name);
        if (v->mem_size)
            free(v->mem);
        link_del(&v->value_link);
        free(v);
    }
    free(e);
}

/* @0x120e0 */
struct aud_event *aud_event_new(void *param, unsigned int param_size)
{
    struct aud_event *e = calloc(1, sizeof(*e) + param_size);

    if (!e) {
        LOGE(aud_bufdata, 0x78, "no mem. size %d\n", param_size);
        return NULL;
    }
    e->param_size = param_size;
    e->object.type = AUD_OBJECT_TYPE_EVENT;
    e->object.user = 1;
    e->object.destroy = event_destroy;
    if (param_size)
        memcpy(e->param, param, param_size);
    link_init(&e->values);
    return e;
}

/* @0x12164: deep copy of type, prop, param and every value */
struct aud_event *aud_event_dup(struct aud_event *event)
{
    struct aud_event *e = aud_event_new(event->param, event->param_size);
    struct link *l;

    if (!e)
        return NULL;

    for_each_link(l, &event->values) {
        struct aud_event_value *src = link_container(struct aud_event_value, value_link, l);
        struct aud_event_value *v = calloc(sizeof(*v), 1);

        if (!v) {
            LOGE(aud_bufdata, 0xea, "no mem\n");
            break;
        }
        v->name = strdup(src->name);
        if (!v->name) {
            free(v);
            LOGE(aud_bufdata, 0xf2, "no mem\n");
            break;
        }
        v->type = src->type;
        switch (src->type) {
        case aud_value_bool:
            v->value.b = src->value.b;
            break;
        case aud_value_str:
            v->value.s = strdup(src->value.s);
            v->mem = v->value.s;
            if (v->value.s)
                v->mem_size = strlen(v->value.s) + 1;
            break;
        case aud_value_data:
            v->value.d = malloc(src->mem_size);
            v->mem = v->value.d;
            if (v->value.d) {
                memcpy(v->value.d, src->mem, src->mem_size);
                v->mem_size = src->mem_size;
            }
            break;
        case aud_value_fraction:
            v->value.fr = src->value.fr;
            break;
        default:
            LOGE(aud_bufdata, 0xfa, "unknown type. %d\n", src->type);
            /* fall through */
        case aud_value_int:
            v->value.i = src->value.i;
            break;
        }
        link_add_tail(&e->values, &v->value_link);
    }
    e->type = event->type;
    e->prop = event->prop;
    return e;
}

/* @0x122d8 */
struct aud_event *aud_event_new_first(void)
{
    struct aud_event *e = aud_event_new(NULL, 0);

    if (!e) {
        LOGE(aud_bufdata, 0x136, "no first event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_FIRST_AU;
    e->prop |= AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC;
    return e;
}

/* @0x12334 */
struct aud_event *aud_event_new_flush(void)
{
    struct aud_event *e = aud_event_new(NULL, 0);

    if (!e) {
        LOGE(aud_bufdata, 0x170, "no last event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_FLUSH;
    e->prop |= AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC;
    return e;
}

/* @0x12390 */
struct aud_event *aud_event_new_flush_async(void)
{
    struct aud_event *e = aud_event_new(NULL, 0);

    if (!e) {
        LOGE(aud_bufdata, 0x183, "no last event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_FLUSH;
    e->prop |= AUD_EVENT_PROP_DOWNWARD;
    return e;
}

/* @0x123ec */
struct aud_event *aud_event_new_last(void)
{
    struct aud_event *e = aud_event_new(NULL, 0);

    if (!e) {
        LOGE(aud_bufdata, 0x195, "no last event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_LAST_AU;
    e->prop |= AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC;
    return e;
}

/* @0x12448: START travelling upward (prop 0) */
struct aud_event *aud_event_new_start_up(void)
{
    struct aud_event *e = aud_event_new(NULL, 0);

    if (!e) {
        LOGE(aud_bufdata, 0x1a8, "no start event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_START;
    return e;
}

/* @0x1249c */
struct aud_event *aud_event_new_start(void)
{
    struct aud_event *e = aud_event_new_start_up();

    if (e)
        e->prop |= AUD_EVENT_PROP_DOWNWARD;
    return e;
}

/* @0x124b0 */
struct aud_event *aud_event_new_stop(void)
{
    struct aud_event *e = aud_event_new(NULL, 0);

    if (!e) {
        LOGE(aud_bufdata, 0x1c7, "no event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_STOP;
    e->prop |= AUD_EVENT_PROP_DOWNWARD;
    return e;
}

/* @0x1250c */
struct aud_event *aud_event_new_mark(void *data, unsigned int size)
{
    struct aud_event *e = aud_event_new(data, size);

    if (!e) {
        LOGE(aud_bufdata, 0x1d9, "no event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_MARK;
    e->prop |= AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC;
    return e;
}

/* @0x12564 */
struct aud_event *aud_event_new_disconnect(void)
{
    struct aud_event *e = aud_event_new(NULL, 0);

    if (!e) {
        LOGE(aud_bufdata, 0x1ec, "no event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_DISCONNECT;
    e->prop |= AUD_EVENT_PROP_DOWNWARD;
    return e;
}

/* @0x125c0 */
struct aud_event *aud_event_new_disconnect_sync(void)
{
    struct aud_event *e = aud_event_new_disconnect();

    if (!e) {
        LOGE(aud_bufdata, 0x1fe, "no event\n");
        return NULL;
    }
    e->prop |= AUD_EVENT_PROP_SYNC;
    return e;
}

/* @0x12614 */
struct aud_event *aud_event_new_restart(void)
{
    struct aud_event *e = aud_event_new(NULL, 0);

    if (!e) {
        LOGE(aud_bufdata, 0x20f, "no event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_RESTART;
    e->prop |= AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC;
    return e;
}

/* @0x12670 */
struct aud_event *aud_event_new_latency_check(void)
{
    struct aud_event *e = aud_event_new(NULL, 0);

    if (!e) {
        LOGE(aud_bufdata, 0x221, "no event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_LATENCY_CHECK;
    e->prop |= AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC;
    return e;
}

/* @0x126cc */
struct aud_event *aud_event_new_custom(void)
{
    struct aud_event *e = aud_event_new(NULL, 0);

    if (!e) {
        LOGE(aud_bufdata, 0x233, "no event\n");
        return NULL;
    }
    e->type = AUD_EVENT_TYPE_CUSTOM;
    e->prop |= AUD_EVENT_PROP_DOWNWARD | AUD_EVENT_PROP_SYNC;
    return e;
}

/* inlined everywhere: linear lookup by name */
static inline struct aud_event_value *search_value(struct aud_event *e, const char *name)
{
    struct link *l;

    if (!e)
        return NULL;
    for_each_link(l, &e->values) {
        struct aud_event_value *v = link_container(struct aud_event_value, value_link, l);

        if (strcmp(v->name, name) == 0)
            return v;
    }
    return NULL;
}

/* @0x11f24: find or append a value slot. An existing slot is overwritten in place;
 * its old str/mem copy is NOT freed (leaks on re-set of str/mem values). */
struct aud_event_value *get_value(struct aud_event *e, const char *name)
{
    struct aud_event_value *v;

    if (!e)
        return NULL;
    v = search_value(e, name);
    if (v)
        return v;
    v = calloc(sizeof(*v), 1);
    if (!v) {
        LOGE(aud_bufdata, 0x271, "no mem\n");
        return NULL;
    }
    link_add_tail(&e->values, &v->value_link);
    v->name = strdup(name);
    return v;
}

/* @0x12728 */
int aud_event_value_set_int(struct aud_event *e, const char *name, int i)
{
    struct aud_event_value *v = get_value(e, name);

    if (!v)
        return -1;
    v->value.i = i;
    v->type = aud_value_int;
    return 0;
}

/* @0x12744 */
int aud_event_value_set_bool(struct aud_event *e, const char *name, bool b)
{
    struct aud_event_value *v = get_value(e, name);

    if (!v)
        return -1;
    v->type = aud_value_bool;
    v->value.b = b;
    return 0;
}

/* @0x12760 */
int aud_event_value_set_str(struct aud_event *e, const char *name, const char *s)
{
    struct aud_event_value *v = get_value(e, name);

    if (!v)
        return -1;
    v->type = aud_value_str;
    v->value.s = strdup(s);
    v->mem = v->value.s;
    if (v->value.s)
        v->mem_size = strlen(v->value.s) + 1;
    return 0;
}

/* @0x12790 */
int aud_event_value_set_mem(struct aud_event *e, const char *name, void *data, unsigned int size)
{
    struct aud_event_value *v = get_value(e, name);

    if (!v)
        return -1;
    v->type = aud_value_data;
    v->value.d = malloc(size);
    v->mem = v->value.d;
    if (v->value.d) {
        memcpy(v->value.d, data, size);
        v->mem_size = size;
    }
    return 0;
}

/* @0x12a1c */
int aud_event_value_set_fraction(struct aud_event *e, const char *name, int num, int den)
{
    struct aud_event_value *v = get_value(e, name);

    if (!v)
        return -1;
    v->type = aud_value_fraction;
    v->value.fr.num = num;
    v->value.fr.den = den;
    return 0;
}

/* @0x127c4: NULL-terminated (name, type, value...) list; fraction is not supported */
struct aud_event *_aud_event_set_values(struct aud_event *e, const char *name, va_list ap)
{
    if (!e) {
        LOGE(aud_bufdata, 0xae, "no event\n");
        return NULL;
    }
    while (name) {
        int type = va_arg(ap, int);

        switch (type) {
        case aud_value_int:
            aud_event_value_set_int(e, name, va_arg(ap, int));
            break;
        case aud_value_bool:
            aud_event_value_set_bool(e, name, va_arg(ap, int) != 0);
            break;
        case aud_value_str:
            aud_event_value_set_str(e, name, va_arg(ap, const char *));
            break;
        case aud_value_data: {
            void *data = va_arg(ap, void *);
            unsigned int size = va_arg(ap, unsigned int);

            aud_event_value_set_mem(e, name, data, size);
            break;
        }
        default:
            LOGE(aud_bufdata, 0xba, "unknown type. %d\n", type);
            return e;
        }
        name = va_arg(ap, const char *);
    }
    return e;
}

/* @0x1289c */
struct aud_event *aud_event_dup_and_set(struct aud_event *event, const char *name, ...)
{
    struct aud_event *e = aud_event_dup(event);
    va_list ap;

    if (!e) {
        LOGE(aud_bufdata, 0x124, "no dup event\n");
        return NULL;
    }
    va_start(ap, name);
    e = _aud_event_set_values(e, name, ap);
    va_end(ap);
    return e;
}

/* @0x128fc */
struct aud_event *aud_event_newv_first(const char *name, ...)
{
    struct aud_event *e = aud_event_new_first();
    va_list ap;

    if (!e) {
        LOGE(aud_bufdata, 0x14a, "no first event\n");
        return NULL;
    }
    va_start(ap, name);
    e = _aud_event_set_values(e, name, ap);
    va_end(ap);
    return e;
}

/* @0x1295c */
struct aud_event *aud_event_newv_restart(const char *name, ...)
{
    struct aud_event *e = aud_event_new_restart();
    va_list ap;

    if (!e) {
        LOGE(aud_bufdata, 0x15d, "no restart event\n");
        return NULL;
    }
    va_start(ap, name);
    e = _aud_event_set_values(e, name, ap);
    va_end(ap);
    return e;
}

/* @0x129bc */
struct aud_event *aud_event_newv_custom(const char *name, ...)
{
    struct aud_event *e = aud_event_new_custom();
    va_list ap;

    if (!e) {
        LOGE(aud_bufdata, 0x247, "no custom event\n");
        return NULL;
    }
    va_start(ap, name);
    e = _aud_event_set_values(e, name, ap);
    va_end(ap);
    return e;
}

/* @0x12a3c */
struct aud_event *aud_event_set_values(struct aud_event *e, const char *name, ...)
{
    va_list ap;

    va_start(ap, name);
    e = _aud_event_set_values(e, name, ap);
    va_end(ap);
    return e;
}

/* @0x12a58: def if missing or not an int */
int aud_event_value_get_int(struct aud_event *e, const char *name, int def)
{
    struct aud_event_value *v = search_value(e, name);

    if (v && v->type == aud_value_int)
        return v->value.i;
    return def;
}

/* @0x12a88 */
bool aud_event_value_get_bool(struct aud_event *e, const char *name, bool def)
{
    struct aud_event_value *v = search_value(e, name);

    if (v && v->type == aud_value_bool)
        return v->value.b;
    return def;
}

/* @0x12ac0 */
const char *aud_event_value_get_str(struct aud_event *e, const char *name, const char *def)
{
    struct aud_event_value *v = search_value(e, name);

    if (v && v->type == aud_value_str)
        return v->value.s;
    return def;
}

/* @0x12af8: *size is only written on success */
void *aud_event_value_get_mem(struct aud_event *e, const char *name, unsigned int *size)
{
    struct aud_event_value *v = search_value(e, name);

    if (!v || v->type != aud_value_data)
        return NULL;
    *size = v->mem_size;
    return v->value.d;
}

/* @0x12b34 */
int aud_event_value_get_fraction(struct aud_event *e, const char *name, int *num, int *den)
{
    struct aud_event_value *v = search_value(e, name);

    if (!v || v->type != aud_value_fraction)
        return -1;
    *num = v->value.fr.num;
    *den = v->value.fr.den;
    return 0;
}

/* @0x12b80 */
void aud_event_value_del(struct aud_event *e, const char *name)
{
    struct aud_event_value *v = search_value(e, name);

    if (!v)
        return;
    link_del(&v->value_link);
    if (v->mem)
        free(v->mem);
    free(v->name);
    free(v);
}

/* @0x12bd0 */
bool aud_event_has_name(struct aud_event *e, const char *name)
{
    return search_value(e, name) != NULL;
}

/* @0x12c00: RESTART/DSP_RESET/DEC_ERROR/MPEGH_UI all print "unknown" */
const char *aud_event_name(struct aud_event *e)
{
    switch (e->type) {
    case AUD_EVENT_TYPE_ENABLE_DISABLE:
        return "enable/disable";
    case AUD_EVENT_TYPE_FIRST_AU:
        return "first";
    case AUD_EVENT_TYPE_LAST_AU:
        return "last";
    case AUD_EVENT_TYPE_START:
        return "start";
    case AUD_EVENT_TYPE_STOP:
        return "stop";
    case AUD_EVENT_TYPE_MARK:
        return "mark";
    case AUD_EVENT_TYPE_DISCONNECT:
        return "disconnect";
    case AUD_EVENT_TYPE_FLUSH:
        return AUD_EVENT_IS_SYNC(e) ? "flush (sync)" : "flush (async)";
    case AUD_EVENT_TYPE_LATENCY_CHECK:
        return "latency check";
    case AUD_EVENT_TYPE_CUSTOM:
        return "custom";
    default:
        return "unknown";
    }
}

/* @0x12c98: "<name>(<type>), prop %08x" then sep + "name=(type)value" per value */
char *aud_event_str2(struct aud_event *e, int name_width, const char *sep)
{
    unsigned int off = 0;
    struct link *l;
    char *s;

    s = audutil_append_printf(NULL, &off, "%s(%d), prop %08x", aud_event_name(e), e->type, e->prop);
    for_each_link(l, &e->values) {
        struct aud_event_value *v = link_container(struct aud_event_value, value_link, l);

        switch (v->type) {
        case aud_value_int:
            s = audutil_append_printf(s, &off, "%s%*s=(int)%d", sep, name_width, v->name,
                                      v->value.i);
            break;
        case aud_value_bool:
            s = audutil_append_printf(s, &off, "%s%*s=(bool)%s", sep, name_width, v->name,
                                      v->value.b ? "true" : "false");
            break;
        case aud_value_str:
            s = audutil_append_printf(s, &off, "%s%*s=(str)%s", sep, name_width, v->name,
                                      v->value.s);
            break;
        case aud_value_data:
            s = audutil_append_printf(s, &off, "%s%*s=(mem size)%d", sep, name_width, v->name,
                                      v->mem_size);
            break;
        case aud_value_fraction:
            s = audutil_append_printf(s, &off, "%s%*s=(fraction)%d/%d", sep, name_width,
                                      v->name, v->value.fr.num, v->value.fr.den);
            break;
        }
    }
    return s;
}

/* @0x12d9c */
char *aud_event_str(struct aud_event *e)
{
    return aud_event_str2(e, 0, ", ");
}

#define ALIGN4(x)   (((x) + 3u) & ~3u)

/* @0x12dac: flatten an event into one malloc'ed block of 32-bit words (host endian):
 *
 *   u32 object.type ('evnt')   u32 type   u32 prop   u32 value_count   u32 param_size
 *   u8  param[ALIGN4(param_size)]
 *   per value, in list order:
 *     u32 name_len             = ALIGN4(strlen(name) + 1), NUL included
 *     char name[name_len]
 *     u32 value_type           (0 int, 1 bool, 2 str, 3 mem, 4 fraction)
 *     u32 value_size           int/bool: 4, str: strlen+1, mem: size, fraction: 8
 *     u8  value[ALIGN4(value_size)]   bool is widened to a u32 0/1, str keeps its NUL
 */
void *aud_event_serialize(struct aud_event *e, unsigned int *mem_size)
{
    unsigned int size, count = 0, w;
    struct link *l;
    uint32_t *p;

    if (!e)
        return NULL;

    size = 5 * 4 + ALIGN4(e->param_size);
    for_each_link(l, &e->values) {
        struct aud_event_value *v = link_container(struct aud_event_value, value_link, l);

        size += 4 + ALIGN4(strlen(v->name) + 1) + 8;
        if (v->type == aud_value_int || v->type == aud_value_bool)
            size += 4;
        else if (v->type == aud_value_str || v->type == aud_value_data)
            size += ALIGN4(v->mem_size);
        else if (v->type == aud_value_fraction)
            size += 8;
        count++;
    }

    p = malloc(size);
    if (!p) {
        LOGE(aud_bufdata, 0x3b2, "no mem??\n");
        return NULL;
    }
    *mem_size = size;

    p[0] = e->object.type;
    p[1] = e->type;
    p[2] = e->prop;
    p[3] = count;
    p[4] = e->param_size;
    w = 5;
    if (e->param_size) {
        memcpy(&p[w], e->param, e->param_size);
        w += (e->param_size + 3) / 4;
    }

    for_each_link(l, &e->values) {
        struct aud_event_value *v = link_container(struct aud_event_value, value_link, l);
        unsigned int len = strlen(v->name);

        p[w++] = (len + 4) & ~3u;
        strcpy((char *)&p[w], v->name);
        w += (len + 4) / 4;
        p[w++] = v->type;
        switch (v->type) {
        case aud_value_int:
            p[w++] = 4;
            p[w++] = v->value.i;
            break;
        case aud_value_bool:
            p[w++] = 4;
            p[w++] = v->value.b;
            break;
        case aud_value_str:
        case aud_value_data:
            p[w++] = v->mem_size;
            memcpy(&p[w], v->mem, v->mem_size);
            w += (v->mem_size + 3) / 4;
            break;
        case aud_value_fraction:
            p[w++] = 8;
            p[w++] = v->value.fr.num;
            p[w++] = v->value.fr.den;
            break;
        default:
            /* unknown type: value_size slot left unwritten and reused by the next value */
            break;
        }
    }
    return p;
}

/* @0x11d44: 0 if a NUL occurs within size bytes */
int check_str_term(const void *data, unsigned int size)
{
    const char *s = data;
    unsigned int i;

    for (i = 0; i < size; i++)
        if (s[i] == '\0')
            return 0;
    LOGE(aud_bufdata, 0x3f6, "no string terminate. %p, %d\n", data, size);
    return -1;
}

/* @0x12f6c: inverse of aud_event_serialize. The leading 'evnt' magic is not checked. */
struct aud_event *aud_event_deserialize(void *data, unsigned int size)
{
    uint32_t *p = data;
    struct aud_event *e;
    unsigned int param_size, count, i, w;

    if (size < 5 * 4) {
        LOGE(aud_bufdata, 0x404, "too small size. %d\n", size);
        return NULL;
    }
    param_size = p[4];
    if (size < param_size + 5 * 4) {
        LOGE(aud_bufdata, 0x40b, "wrong param_size. %d, %d\n", param_size, size);
        return NULL;
    }
    e = aud_event_new(&p[5], param_size);
    if (!e)
        return NULL;
    e->type = p[1];
    e->prop = p[2];
    count = p[3];
    w = 5 + (param_size + 3) / 4;

    for (i = 0; i < count; i++) {
        unsigned int name_len = p[w++], vtype, vsize, vw;
        const char *name;

        if (w * 4 + name_len > size) {
            LOGE(aud_bufdata, 0x421, "wrong name_len. %d %d %d\n", name_len, w, size);
            goto fail;
        }
        name = (const char *)&p[w];
        if (check_str_term(name, name_len) < 0) {
            LOGE(aud_bufdata, 0x429, "wrong name\n");
            goto fail;
        }
        w += name_len / 4;
        vtype = p[w];
        vsize = p[w + 1];
        vw = w + 2;
        if (vw * 4 + vsize > size) {
            LOGE(aud_bufdata, 0x433, "wrong mem_size. %d %d %d\n", vsize, vw, size);
            goto fail;
        }
        switch (vtype) {
        case aud_value_int:
            aud_event_value_set_int(e, name, p[vw]);
            w = vw + 1;
            break;
        case aud_value_bool:
            aud_event_value_set_bool(e, name, p[vw] != 0);
            w = vw + 1;
            break;
        case aud_value_str:
            if (check_str_term(&p[vw], vsize) < 0) {
                LOGE(aud_bufdata, 0x448, "wrong str.\n");
                goto fail;
            }
            aud_event_value_set_str(e, name, (const char *)&p[vw]);
            w = vw + (vsize + 3) / 4;
            break;
        case aud_value_data:
            aud_event_value_set_mem(e, name, &p[vw], vsize);
            w = vw + (vsize + 3) / 4;
            break;
        case aud_value_fraction:
            aud_event_value_set_fraction(e, name, p[vw], p[vw + 1]);
            w = vw + 2;
            break;
        default:
            LOGE(aud_bufdata, 0x465, "wrong value type. %d\n", vtype);
            goto fail;
        }
    }
    return e;

fail:
    aud_event_put(e);
    return NULL;
}
