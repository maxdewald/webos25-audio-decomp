/*
 * libaudcommon: logm message front-ends (hookable logm_message, logmm_printlm),
 * audutil printf/append helpers, audutil option parsing, and the exported
 * logmm_module_* objects.
 *
 * RECONSTRUCTION from orig/usr/lib/libaudcommon.so.1.0.0
 *   sha256 a4df15e082096e6c60c7091b81f90f4fe0a6c5ae07173867ca4a0b05bfd5087f
 * Evidence: build/decomp/libaudcommon.so.1.0.0.c (0x11b24..0x11d44, 0x14820..0x14a0c);
 * module objects in .data (VA 0x16004.., 20 bytes each, mask -> logmm_dummy_mask,
 * initial _mask 0x3f for "ainst", 7 for the rest).
 */
#define _GNU_SOURCE
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"

/* Defined here for all source files. Order/placement in the original .data:
 * ainst, audlog (logm_user.c); aud_bufadp, aud_bufdata (data.c);
 * aud_meta_bufdata (meta_data.c); comutil (utils.c). */
struct logmm_module logmm_module_ainst = { 0, 0, 0x3f, &logmm_dummy_mask, "ainst" };
struct logmm_module logmm_module_audlog = { 0, 0, 7, &logmm_dummy_mask, "audlog" };
struct logmm_module logmm_module_aud_bufadp = { 0, 0, 7, &logmm_dummy_mask, "aud_bufadp" };
struct logmm_module logmm_module_aud_bufdata = { 0, 0, 7, &logmm_dummy_mask, "aud_bufdata" };
struct logmm_module logmm_module_aud_meta_bufdata = { 0, 0, 7, &logmm_dummy_mask, "aud_meta_bufdata" };
struct logmm_module logmm_module_comutil = { 0, 0, 7, &logmm_dummy_mask, "comutil" };

static pthread_mutex_t hook_lock = PTHREAD_MUTEX_INITIALIZER;
static logm_message_hook_func_t message_hook;
static void *message_hook_arg;

/* @0x11b24: "audlog" module at noti level; func is the literal "audlog", line 0x12 */
int logm_message_default(const char *format, va_list ap)
{
    return logmm_vprintl(&logmm_module_audlog, log_level_noti, "audlog", 0x12, format, ap);
}

/* @0x11b58 */
int logm_message_defaultf(const char *format, ...)
{
    va_list ap;
    int ret;

    va_start(ap, format);
    ret = logmm_vprintl(&logmm_module_audlog, log_level_noti, "audlog", 0x12, format, ap);
    va_end(ap);
    return ret;
}

/* @0x11b98: install a hook, returns the previous one */
logm_message_hook_func_t logm_message_hook(logm_message_hook_func_t func, void *arg)
{
    logm_message_hook_func_t old;

    pthread_mutex_lock(&hook_lock);
    old = message_hook;
    message_hook = func;
    message_hook_arg = arg;
    pthread_mutex_unlock(&hook_lock);
    return old;
}

/* @0x11bbc: the formatted string is handed to the hook and not freed here */
int logm_message(const char *format, ...)
{
    va_list ap;
    char *str;
    int ret;

    va_start(ap, format);
    if (!message_hook) {
        ret = logm_message_default(format, ap);
    } else {
        pthread_mutex_lock(&hook_lock);
        if (!message_hook) {
            ret = logm_message_default(format, ap);
        } else {
            ret = vasprintf(&str, format, ap);
            if (ret < 0)
                fprintf(stderr, "%s.%d: error. %s\n", __func__, 0x50, strerror(errno));
            else
                ret = message_hook(message_hook_arg, str);
        }
        pthread_mutex_unlock(&hook_lock);
    }
    va_end(ap);
    return ret;
}

/* @0x11c64: log through "ainst" prefixed with a right-aligned instance name */
int logmm_printlm(const char *name, int level, const char *func, int line,
                  const char *format, ...)
{
    va_list ap;
    char *str;
    int ret;

    va_start(ap, format);
    ret = vasprintf(&str, format, ap);
    va_end(ap);
    if (ret < 0) {
        fprintf(stderr, "%s.%d: error. %s\n", __func__, 0x6f, strerror(errno));
        return ret;
    }
    ret = logmm_printl(&logmm_module_ainst, level, func, line, "%16s: %s", name, str);
    free(str);
    return ret;
}

/* @0x14820 */
char *audutil_strdup_vprintf(const char *fmt, va_list ap)
{
    va_list aq;
    char *s;
    int len;

    va_copy(aq, ap);
    len = vsnprintf(NULL, 0, fmt, aq) + 1;
    va_end(aq);
    s = malloc(len);
    if (!s) {
        LOGE(comutil, 0x37, "no mem. %d\n", len);
        return NULL;
    }
    vsnprintf(s, len, fmt, ap);
    return s;
}

/* @0x14888 */
char *audutil_strdup_printf(const char *fmt, ...)
{
    va_list ap;
    char *s;

    va_start(ap, fmt);
    s = audutil_strdup_vprintf(fmt, ap);
    va_end(ap);
    return s;
}

/* @0x148a4: append at str + *off, growing str; returns the (possibly moved) buffer,
 * or the old one unchanged if realloc fails */
char *audutil_append_vprintf(char *str, unsigned int *off, const char *fmt, va_list ap)
{
    va_list aq;
    unsigned int len;
    char *s;

    va_copy(aq, ap);
    len = vsnprintf(NULL, 0, fmt, aq) + 1;
    va_end(aq);
    s = realloc(str, *off + len);
    if (!s) {
        LOGE(comutil, 0x59, "no mem. %d\n", *off + len);
        return str;
    }
    *off += vsnprintf(s + *off, len, fmt, ap);
    return s;
}

/* @0x14928 */
char *audutil_append_printf(char *str, unsigned int *off, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    str = audutil_append_vprintf(str, off, fmt, ap);
    va_end(ap);
    return str;
}

/* @0x14944: argv = { name, value }; sets the matching option, returns it (or NULL).
 * int: strtoul(value, 0) (0 if value is NULL); bool: true iff value is "on" or "true". */
const struct audutil_opt *audutil_set_opts(const struct audutil_opt *opts, char **argv)
{
    const char *name = argv[0];
    const char *value = argv[1];

    if (!name)
        return NULL;
    for (; opts->name; opts++) {
        if (strcmp(name, opts->name) != 0)
            continue;
        if (opts->type == opt_int) {
            *(unsigned long *)opts->ptr = value ? strtoul(value, NULL, 0) : 0;
        } else if (opts->type == opt_bool) {
            *(bool *)opts->ptr = value && (strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
        }
        return opts;
    }
    return NULL;
}

/* @0x149c8 */
void audutil_print_opt(const struct audutil_opt *opt)
{
    if (opt->type == opt_bool)
        logm_message("%s %s\n", opt->name, *(bool *)opt->ptr ? "on" : "off");
    else if (opt->type == opt_int)
        logm_message("%s %d\n", opt->name, *(int *)opt->ptr);
}
