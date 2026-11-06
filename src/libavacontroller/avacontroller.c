#include "avacontroller.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Opaque libsoup/GLib ABI; only these imported entry points are used. */
typedef struct _SoupSession SoupSession;
typedef struct _SoupMessage SoupMessage;
extern SoupSession *soup_session_sync_new(void);
extern SoupMessage *soup_message_new(const char *method, const char *uri);
extern unsigned int soup_session_send_message(SoupSession *session, SoupMessage *message);
extern void soup_session_abort(SoupSession *session);
extern void g_object_unref(void *object);
extern const char *g_intern_static_string(const char *string);
extern const char *_SOUP_METHOD_GET;
/* Keep the observed libc call rather than GCC's inline NEON memset. */
#ifdef __arm__
extern void *ava_memset(void *buffer, int value, size_t size) __asm__("memset");
#else
#define ava_memset memset
#endif

/* The ARM literal at raw VA 0x840 addresses this base; prefix is base +24. */
static const char endpoint[] = "http://127.0.0.1:54321\0\0/proxy?url=";

/* ARM layout: pid +0, URL +4, header +8, total 40 bytes. */
static struct {
    long pid;
    char *url;
    char header[30];
} ava;

#ifdef __arm__
_Static_assert(sizeof(ava) == 40, "AVA state must retain the ARM layout");
#endif

/* @0x107c8: Gate on girafficava, then retain a PID-tagged proxy URL/header.
 * The original neither checks malloc nor frees a previously opened URL. */
char *openAVASession(const char *url)
{
    if (!url)
        return NULL;
    if (access("/usr/bin/girafficava", F_OK))
        return NULL;

    ava_memset(&ava, 0, sizeof(ava));
    ava.pid = getpid();
    size_t length = strlen(url) + 34;
    ava.url = malloc(length);
    snprintf(ava.url, length, "%s%s%s", endpoint, endpoint + 24, url);
    snprintf(ava.header, sizeof(ava.header), "X-GRF-EXT-ID: %ld", ava.pid);
    return ava.url;
}

/* @0x1084c: Synchronously request free_session, dispose soup objects, free URL.
 * HTTP status is ignored; PID and header survive close. No NULL soup checks. */
void closeAVASession(void)
{
    char url[100];
    if (!ava.url)
        return;

    snprintf(url, sizeof(url), "%s/%s?extid=%ld", endpoint,
             "free_session", ava.pid);
    SoupSession *session = soup_session_sync_new();
    if (!__atomic_load_n(&_SOUP_METHOD_GET, __ATOMIC_SEQ_CST))
        __atomic_store_n(&_SOUP_METHOD_GET, g_intern_static_string("GET"), __ATOMIC_SEQ_CST);
    SoupMessage *message = soup_message_new(*(const char *volatile *)&_SOUP_METHOD_GET, url);
    soup_session_send_message(session, message);
    soup_session_abort(session);
    g_object_unref(session);
    g_object_unref(message);
    if (ava.url) {
        free(ava.url);
        ava.url = NULL;
    }
}

/* @0x108fc: Constant server origin, not the allocated per-session URL. */
const char *getAVAProxyUrl(void)
{
    return endpoint;
}

/* @0x10908: Stable shared header buffer, including before open/after close. */
const char *getAVAExtendedHttpHeader(void)
{
    return ava.header;
}
