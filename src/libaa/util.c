/*
 * libaa small utilities: debug option lookup (autil_*) and the shell-like string
 * splitter used by arocket (aabase_string_vector).
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 * Evidence: build/decomp/libaa.so.2.0.0.c @0x20634..0x20b4c, logmm module "aabase",
 * __func__ strings "search_quotationmark", "append_string".
 */
#define _GNU_SOURCE
#include <ctype.h>
#include <fcntl.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "types.h"

LOGMM_MODULE_DEFINE(aabase);

/* @0x20634: first line of /build/scratch/<name>, else /var/<name> (malloc'd) */
char *autil_option_str_from_file(const char *basename)
{
    char path[strlen(basename) + 6];
    char *buf, *p;
    ssize_t n, i;
    off_t size;
    int fd;

    sprintf(path, "/build/scratch/%s", basename);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        sprintf(path, "/var/%s", basename);
        fd = open(path, O_RDONLY);
        if (fd < 0)
            return NULL;
    }
    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    if (size < 0 || !(buf = malloc(size + 1))) {
        buf = NULL;
    } else {
        n = read(fd, buf, size);
        if (n < 0) {
            close(fd);
            free(buf);
            return NULL;
        }
        p = buf;
        for (i = 0; i < n; i++, p++)
            if (*p == '\n' || *p == '\0')
                break;
        *p = '\0';
    }
    close(fd);
    return buf;
}

/* @0x2070c: getenv("DEBUG_<name>") (malloc'd) */
char *autil_option_str_from_env(const char *basename)
{
    char name[strlen(basename) + 7];
    char *v;

    sprintf(name, "DEBUG_%s", basename);
    v = getenv(name);
    return v ? strdup(v) : NULL;
}

/* @0x20748: file overrides environment */
char *autil_option_str(const char *basename)
{
    char *v = autil_option_str_from_file(basename);

    if (v)
        return v;
    return autil_option_str_from_env(basename);
}

/* @0x20760 */
int autil_option_int(const char *basename, int def)
{
    char *v = autil_option_str(basename);

    if (v) {
        def = strtol(v, NULL, 10);
        free(v);
    }
    return def;
}

struct strvec_ctx {
    jmp_buf env;
    char **vec;
    int count;
    const char *error_at;
};

/* @0x20780: find the closing quote `q`, skipping nested quotes; longjmp if missing */
const char *search_quotationmark(struct strvec_ctx *ctx, const char *s, int q)
{
    LOGD(aabase, 0x1f, "search '%c' at \"%s\".\n", q, s);
    for (; *s; s++) {
        if (*s == q)
            return s;
        if (*s == '\'' || *s == '"')
            s = search_quotationmark(ctx, s + 1, *s);
    }
    LOGE(aabase, 0x2c, "cannot find quotation mark. '%c' at \"%s\"\n", q, s);
    ctx->error_at = s;
    longjmp(ctx->env, -1);
}

/* @0x2082c: append s[0..len) to the vector, stripping one level of matching quotes */
void append_string(struct strvec_ctx *ctx, const char *s, size_t len)
{
    char **v = realloc(ctx->vec, (ctx->count + 2) * sizeof(char *));

    if (!v) {
        LOGE(aabase, 0x3d, "no mem. \"%s\"\n", s);
        longjmp(ctx->env, -1);
    }
    if ((int)len > 1 && (s[0] == '\'' || s[0] == '"') && s[len - 1] == s[0]) {
        s++;
        len -= 2;
    }
    v[ctx->count] = strndup(s, len);
    v[ctx->count + 1] = NULL;
    LOGD(aabase, 0x49, "count %d, append \"%s\".\n", ctx->count + 1, v[ctx->count]);
    ctx->vec = v;
    ctx->count++;
}

/* @0x20920 */
void aabase_free_vector(char **vector)
{
    char **entry;

    if (!vector)
        return;
    for (entry = vector; *entry; entry++)
        free(*entry);
    free(vector);
}

/* @0x20944: split at whitespace; '...' and "..." group (and may nest the other kind),
 * the outer quotes of a whole token are removed. NULL on unbalanced quotes.
 */
char **aabase_string_vector(const char *str)
{
    struct strvec_ctx ctx;
    const char *p, *start = NULL;
    int i;

    memset(&ctx, 0, sizeof(ctx));
    LOGD(aabase, 0x70, "convert string to vector. \"%s\"\n", str);
    if (setjmp(ctx.env)) {
        LOGE(aabase, 0x77, "parse failed. string \"%s\"\n", str);
        if (ctx.error_at)
            LOGE(aabase, 0x79, "error offset %d. \"%s\"\n", (int)(ctx.error_at - str),
                 ctx.error_at);
        aabase_free_vector(ctx.vec);
        return NULL;
    }

    for (p = str; *p; p++) {
        if (isspace((unsigned char)*p)) {
            if (start) {
                append_string(&ctx, start, p - start);
                start = NULL;
            }
            continue;
        }
        if (*p == '\'' || *p == '"') {
            const char *end = search_quotationmark(&ctx, p + 1, *p);

            if (!start)
                start = p;
            p = end;
            continue;
        }
        if (!start)
            start = p;
    }
    if (start)
        append_string(&ctx, start, p - start);

    LOGD(aabase, 0x9c, "got %d strings.\n", ctx.count);
    for (i = 0; i < ctx.count; i++)
        LOGD(aabase, 0x9e, "%d. \"%s\"\n", i, ctx.vec[i]);
    return ctx.vec;
}
