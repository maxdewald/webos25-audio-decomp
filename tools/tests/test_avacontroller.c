#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int fake_access(const char *, int);
static pid_t fake_getpid(void);
static void *fake_malloc(size_t);
static void fake_free(void *);

#define access fake_access
#define getpid fake_getpid
#define malloc fake_malloc
#define free fake_free
#include "../../src/libavacontroller/avacontroller.c"
#undef access
#undef getpid
#undef malloc
#undef free

const char *_SOUP_METHOD_GET;
static int available, access_calls, pid_calls, malloc_calls, free_calls;
static pid_t process_id;
static size_t allocation_size;
static char *last_allocation;
static char request[100];
static int session_token, message_token;
static const char *events[16];
static size_t event_count;
static int clear_url_on_unref;

static void event(const char *name)
{
    assert(event_count < sizeof(events) / sizeof(events[0]));
    events[event_count++] = name;
}

static int fake_access(const char *name, int mode)
{
    assert(strcmp(name, "/usr/bin/girafficava") == 0);
    assert(mode == F_OK);
    ++access_calls;
    return available ? 0 : -1;
}

static pid_t fake_getpid(void)
{
    ++pid_calls;
    return process_id;
}

static void *fake_malloc(size_t size)
{
    ++malloc_calls;
    allocation_size = size;
    last_allocation = malloc(size);
    assert(last_allocation);
    return last_allocation;
}

static void fake_free(void *pointer)
{
    assert(pointer == last_allocation);
    ++free_calls;
    event("free");
    free(pointer);
    last_allocation = NULL;
}

SoupSession *soup_session_sync_new(void)
{
    event("new-session");
    return (SoupSession *)&session_token;
}

const char *g_intern_static_string(const char *string)
{
    assert(strcmp(string, "GET") == 0);
    event("intern-GET");
    return "interned-GET";
}

SoupMessage *soup_message_new(const char *method, const char *uri)
{
    assert(method == _SOUP_METHOD_GET);
    assert(method);
    assert(strlen(uri) < sizeof(request));
    strcpy(request, uri);
    event("new-message");
    return (SoupMessage *)&message_token;
}

unsigned int soup_session_send_message(SoupSession *session, SoupMessage *message)
{
    assert(session == (SoupSession *)&session_token);
    assert(message == (SoupMessage *)&message_token);
    event("send");
    return 503;
}

void soup_session_abort(SoupSession *session)
{
    assert(session == (SoupSession *)&session_token);
    event("abort");
}

void g_object_unref(void *object)
{
    if (object == &session_token)
        event("unref-session");
    else {
        assert(object == &message_token);
        event("unref-message");
        if (clear_url_on_unref) {
            free(ava.url);
            ava.url = NULL;
            last_allocation = NULL;
        }
    }
}

static void expect_events(int intern, int freed)
{
    const char *expected[] = {"new-session", "intern-GET", "new-message", "send",
                              "abort", "unref-session", "unref-message", "free"};
    size_t actual = 0;
    for (size_t i = 0; i < sizeof(expected) / sizeof(expected[0]); ++i) {
        if ((!intern && i == 1) || (!freed && i == 7))
            continue;
        assert(actual < event_count);
        assert(strcmp(events[actual++], expected[i]) == 0);
    }
    assert(actual == event_count);
    event_count = 0;
}

int main(void)
{
    const char *header = getAVAExtendedHttpHeader();
    assert(header && !*header);
    assert(strcmp(getAVAProxyUrl(), "http://127.0.0.1:54321") == 0);
    closeAVASession();
    assert(event_count == 0);
    assert(openAVASession(NULL) == NULL);
    assert(access_calls == 0 && pid_calls == 0 && malloc_calls == 0);
    assert(openAVASession("not-running") == NULL);
    assert(access_calls == 1 && pid_calls == 0 && malloc_calls == 0);

    available = 1;
    process_id = 12345;
    char *url = openAVASession("https://example/path?a=1&b=%20");
    assert(url == last_allocation);
    assert(allocation_size == strlen("https://example/path?a=1&b=%20") + 34);
    assert(strcmp(url, "http://127.0.0.1:54321/proxy?url=https://example/path?a=1&b=%20") == 0);
    assert(header == getAVAExtendedHttpHeader());
    assert(strcmp(header, "X-GRF-EXT-ID: 12345") == 0);
    available = 0;
    assert(openAVASession("unavailable") == NULL);
    assert(openAVASession(NULL) == NULL);
    assert(ava.url == url && ava.pid == 12345);
    assert(strcmp(header, "X-GRF-EXT-ID: 12345") == 0);
    closeAVASession();
    expect_events(1, 1);
    assert(strcmp(request, "http://127.0.0.1:54321/free_session?extid=12345") == 0);
    assert(ava.url == NULL && free_calls == 1);
    assert(strcmp(header, "X-GRF-EXT-ID: 12345") == 0);
    closeAVASession();
    assert(event_count == 0);

    available = 1;
    process_id = INT_MAX;
    url = openAVASession("");
    assert(allocation_size == 34);
    assert(strcmp(url, "http://127.0.0.1:54321/proxy?url=") == 0);
    assert(strcmp(header, "X-GRF-EXT-ID: 2147483647") == 0);
    closeAVASession();
    expect_events(0, 1);
    assert(strcmp(request, "http://127.0.0.1:54321/free_session?extid=2147483647") == 0);

    process_id = 77;
    char *orphan = openAVASession("first");
    char long_input[4097];
    memset(long_input, 'x', sizeof(long_input) - 1);
    long_input[sizeof(long_input) - 1] = '\0';
    url = openAVASession(long_input);
    assert(url != orphan && allocation_size == strlen(long_input) + 34);
    assert(strlen(url) == strlen(getAVAProxyUrl()) + strlen("/proxy?url=") + strlen(long_input));
    assert(strcmp(orphan, "http://127.0.0.1:54321/proxy?url=first") == 0);
    free(orphan); /* Original reopen leaks this allocation; test owns its cleanup. */
    clear_url_on_unref = 1;
    closeAVASession();
    expect_events(0, 0);
    assert(ava.url == NULL && last_allocation == NULL);
    assert(strcmp(header, "X-GRF-EXT-ID: 77") == 0);
    puts("avacontroller: all four exports and session lifecycle checks passed");
    return 0;
}
