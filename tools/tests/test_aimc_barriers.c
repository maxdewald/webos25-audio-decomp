/* Host mocks for source-extracted aimc.c; no LG code or target is executed. */
#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#define LOGD(...) ((void)0)
#define LOGI(...) ((void)0)
#define LOGN(...) ((void)0)
#define LOGE(...) ((void)0)
struct link { struct link *prev, *next; };
#define link_container(t, field, p) ((t *)((char *)(p) - offsetof(t, field)))
#define for_each_link(p, h) for ((p) = (h)->next; (p) != (h); (p) = (p)->next)
static void init_link(struct link *h) { h->next = h->prev = h; }
static void link_add_tail(struct link *h, struct link *p)
{ p->prev = h->prev; p->next = h; h->prev->next = p; h->prev = p; }
static void link_del(struct link *p) { p->prev->next = p->next; p->next->prev = p->prev; }

typedef struct { unsigned module, core, request_response, id; } ImcSenderInfo;
typedef struct { int unused; } ImcInfo;
typedef struct {
    ImcSenderInfo target;
    unsigned sender, request_interrupt, cmd;
    int result;
    void *param;
    unsigned param_len;
} ImcResponseParameter;
typedef struct {
    unsigned cmd, target, source, request_response, request_interrupt;
    void (*callback)(int, void *, int, void *);
    void *callback_param;
    unsigned param_len;
    void *param;
} ImcCommandParameter;
typedef int (*aimc_cmd_handler_fn)(void *, unsigned, void *, unsigned);
typedef int (*PFN_ImcCmdProc)(void *, unsigned, ImcSenderInfo *, void *, unsigned);
struct command_handler { struct link list; unsigned target; void *handler, *arg; int old; };
struct aud_thread { pthread_t thread; };
struct aimc {
    int fd, processor_id, handler_pipe[2];
    volatile int running_handler;
    struct aud_thread *handler_thread;
    pthread_mutex_t handler_sync_lock, imc_process_lock, command_lock;
    pthread_cond_t handler_sync_cond;
    struct link command_list;
    ImcInfo imc_info;
    bool command_need_response;
    ImcResponseParameter command_response;
    void *aipc, *adbg, *ipc_header;
};
#define aimc_of_info(p) link_container(struct aimc, imc_info, p)
#define AIMC_DISPATCH_FLAG_REQUIRE_RESPONSE 1u
struct auimc_module { char name[32]; int preferred_processor; unsigned uid; };
struct auimc_connect { unsigned out_uid; int out_port; unsigned in_uid; int in_port; };
struct auimc_disconnect { unsigned uid; int port; };
enum { AUIMC_IO_CREATE_MODULE = 6, AUIMC_IO_DESTROY_MODULE = 7,
       AUIMC_IO_CONNECT_MODULE = 8, AUIMC_IO_DISCONNECT_MODULE_OUTPUT = 14,
       AUIMC_IO_DISCONNECT_MODULE_INPUT = 15 };
enum { ipc_argtype_end, ipc_argtype_bool, ipc_argtype_int, ipc_argtype_longlong,
       ipc_argtype_double, ipc_argtype_str };

static char trace[128];
static size_t nt;
static bool real_io, self_sync;
static struct aimc *active;
static int write_result, wait_result, waits, broadcasts, processes;
static int poll_step, scenario, seen_timeout[8], ntimeouts;
static int transport_result, reply_value, calls, sends, ioctls;
static unsigned last_request, integers[4];
static int nintegers;
static const char *last_name, *string_arg;
static int responses, response_result, response_error, send_error;
static unsigned response_id, response_cmd;
static bool response_reenter;
static ImcCommandParameter sent_command;
static void *final_imc, *final_header;
static void mark(char c) { assert(nt + 1 < sizeof(trace)); trace[nt++] = c; trace[nt] = 0; }
static void reset_trace(void) { nt = 0; trace[0] = 0; }

int aimc_handler_sync(struct aimc *);
int aimc_command_response(struct aimc *, int, void *, unsigned);
int aimc_dispatch_command(ImcInfo *, unsigned, unsigned, void *, unsigned, unsigned, unsigned, unsigned);

static ssize_t mock_write(int fd, const void *p, size_t n)
{
    if (real_io) return write(fd, p, n);
    assert(n == 4);
    if (!memcmp(p, "sync", 4)) mark('w');
    else { assert(!memcmp(p, "stop", 4)); assert(!active->running_handler); mark('s'); }
    if (write_result < 0) errno = EIO;
    return write_result < 0 ? -1 : (ssize_t)n;
}
static int mock_wait(pthread_cond_t *c, pthread_mutex_t *m)
{
    if (real_io || self_sync) return pthread_cond_wait(c, m);
    assert(pthread_mutex_trylock(m) == EBUSY);
    waits++; mark('W');
    return wait_result;
}
static int mock_broadcast(pthread_cond_t *c)
{
    if (real_io) return pthread_cond_broadcast(c);
    assert(pthread_mutex_trylock(&active->handler_sync_lock) == EBUSY);
    broadcasts++; mark('B'); return 0;
}
static int mock_poll(struct pollfd *fds, nfds_t n, int timeout)
{
    if (real_io) return poll(fds, n, timeout);
    assert(ntimeouts < 8); seen_timeout[ntimeouts++] = timeout;
    for (nfds_t i = 0; i < n; i++) fds[i].revents = 0;
    int step = poll_step++;
    if (self_sync) { assert(n == 2); fds[1].revents = POLLIN; return 1; }
    if (step == 0) { fds[0].revents = POLLIN; if (n == 2) fds[1].revents = POLLIN; return 1; }
    if (scenario == 0 && step == 1) { fds[1].revents = POLLIN; return 1; }
    if (scenario == 0 && step == 2) return 0;
    errno = EIO; return -1;
}
static ssize_t mock_read(int fd, void *p, size_t n)
{
    if (real_io) return read(fd, p, n);
    const char *cmd = scenario == 1 ? "stop" : scenario == 2 ? "syncsync" : "sync";
    size_t len = strlen(cmd); assert(n >= len); memcpy(p, cmd, len); return (ssize_t)len;
}
static int mock_close(int fd) { mark('c'); return 0; }
static void mock_free(void *p)
{
    if (p == final_imc) mark('F');
    else if (p == final_header) mark('h');
    free(p);
}
static void aud_thread_destroy(struct aud_thread *t)
{
    assert(!active->running_handler); mark('j');
    if (real_io) { assert(!pthread_join(t->thread, NULL)); free(t); }
}
static int IMC_Finalize(ImcInfo *p) { mark('I'); return 0; }
static void aipc_destroy(void *p) { mark('a'); }
static void adebug_destroy(void *p) { mark('d'); }
static int aipc_get_pollfd(void *p, struct pollfd *fds) { return 0; }
static int aipc_handle_pollfd(void *p, struct pollfd *fds) { return 0; }
static int aimc_process(struct aimc *imc)
{
    processes++; mark('p');
    if (self_sync) aimc_handler_sync(imc);
    return 0;
}
static int IMC_Response(ImcInfo *info, ImcResponseParameter *rp)
{
    responses++; response_result = rp->result; response_id = rp->target.id;
    response_cmd = rp->cmd; mark('R');
    if (response_reenter) {
        response_reenter = false;
        assert(aimc_of_info(info)->command_need_response);
        aimc_command_response(aimc_of_info(info), 99, NULL, 0);
    }
    return response_error;
}
static int IMC_SendCommand(ImcInfo *info, ImcCommandParameter *cp, unsigned *id)
{ sent_command = *cp; *id = 4; return send_error; }
static void arguments(const char *name, va_list ap)
{
    last_name = name; nintegers = 0; string_arg = NULL;
    int type;
    while ((type = va_arg(ap, int)) != ipc_argtype_end) {
        if (type == ipc_argtype_str) string_arg = va_arg(ap, const char *);
        else { assert(type == ipc_argtype_int); assert(nintegers < 4); integers[nintegers++] = va_arg(ap, unsigned); }
    }
}
static int aipc_call(void *ipc, int target, int *result, const char *name, ...)
{
    assert(target == 0); calls++; va_list ap; va_start(ap, name);
    arguments(name, ap); va_end(ap); *result = reply_value; return transport_result;
}
static int aipc_send(void *ipc, int target, const char *name, ...)
{
    assert(target == 0); sends++; va_list ap; va_start(ap, name);
    arguments(name, ap); va_end(ap); return transport_result;
}
static int mock_ioctl(int fd, unsigned request, void *p)
{
    ioctls++; last_request = request;
    if (request == AUIMC_IO_CREATE_MODULE) {
        struct auimc_module *m = p;
        assert(m->preferred_processor == 7); assert(m->name[31] == 0);
        m->uid = 0x7000123;
    } else if (request == AUIMC_IO_CONNECT_MODULE) {
        struct auimc_connect *c = p;
        assert(c->out_uid == 11 && c->out_port == 2 && c->in_uid == 12 && c->in_port == 3);
    } else if (request == AUIMC_IO_DESTROY_MODULE) assert(*(unsigned *)p == 11);
    else { struct auimc_disconnect *d = p; assert(d->uid == 11 && d->port == 2); }
    errno = EIO; return transport_result;
}
#define write mock_write
#define read mock_read
#define poll mock_poll
#define close mock_close
#define free mock_free
#define ioctl mock_ioctl
#define pthread_cond_wait mock_wait
#define pthread_cond_broadcast mock_broadcast

/* RECOVERED_FUNCTIONS */

#undef write
#undef read
#undef poll
#undef close
#undef free
#undef ioctl
#undef pthread_cond_wait
#undef pthread_cond_broadcast

static void init(struct aimc *imc)
{
    memset(imc, 0, sizeof(*imc)); active = imc; imc->fd = -1; imc->processor_id = 7;
    assert(!pthread_mutex_init(&imc->handler_sync_lock, NULL));
    assert(!pthread_mutex_init(&imc->imc_process_lock, NULL));
    assert(!pthread_mutex_init(&imc->command_lock, NULL));
    assert(!pthread_cond_init(&imc->handler_sync_cond, NULL));
    init_link(&imc->command_list);
}
static void cleanup(struct aimc *imc)
{
    assert(!pthread_mutex_destroy(&imc->handler_sync_lock));
    assert(!pthread_mutex_destroy(&imc->imc_process_lock));
    assert(!pthread_mutex_destroy(&imc->command_lock));
    assert(!pthread_cond_destroy(&imc->handler_sync_cond));
}
static void *thread_main(void *p) { handler_thread(p); return NULL; }
static void test_sync(void)
{
    struct aimc imc; init(&imc);
    for (int fail = 0; fail < 2; fail++) {
        write_result = fail ? -1 : 0; wait_result = EINVAL; waits = 0; reset_trace();
        assert(aimc_handler_sync(&imc) == 0); assert(waits == 1); assert(!strcmp(trace, "wW"));
    }
    write_result = 0; wait_result = 0;
    for (scenario = 0; scenario < 3; scenario++) {
        imc.fd = 50; imc.running_handler = 1; poll_step = ntimeouts = processes = broadcasts = 0; reset_trace();
        assert(handler_thread(&imc) == 0);
        if (scenario == 0) {
            assert(processes == 2 && broadcasts == 1);
            assert(ntimeouts == 4 && seen_timeout[0] == -1 && seen_timeout[1] == 0 && seen_timeout[2] == 0 && seen_timeout[3] == -1);
            assert(!strcmp(trace, "ppB"));
        } else if (scenario == 1) { assert(processes == 0 && broadcasts == 0 && ntimeouts == 1); }
        else { assert(processes == 1 && broadcasts == 0 && seen_timeout[1] == -1); }
    }
    imc.fd = -1; imc.running_handler = 1; real_io = true;
    assert(!pipe(imc.handler_pipe));
    imc.handler_thread = calloc(1, sizeof(*imc.handler_thread)); assert(imc.handler_thread);
    assert(!pthread_create(&imc.handler_thread->thread, NULL, thread_main, &imc));
    assert(aimc_handler_sync(&imc) == 0);
    int rfd = imc.handler_pipe[0], wfd = imc.handler_pipe[1];
    aimc_handler_destory(&imc); assert(!imc.handler_thread);
    /* close is mocked to observe teardown ordering, so close the actual pipe. */
    close(rfd); close(wfd); real_io = false; cleanup(&imc);
    puts("PASS: sync idle boundary, failed writes/waits, coalescing, stop, real worker join");
}

static int handler(void *arg, unsigned cmd, void *param, unsigned len)
{
    struct aimc *imc = arg;
    assert(!pthread_mutex_trylock(&imc->command_lock)); pthread_mutex_unlock(&imc->command_lock);
    mark('H');
    if (cmd == 2) { aimc_command_response(imc, -23, param, len); mark('D'); }
    if (cmd == 3) assert(aimc_dispatch_command(&imc->imc_info, 11, 4, NULL, 0, 20, 8, 0) == -17);
    if (cmd == 5) assert(aimc_dispatch_command(&imc->imc_info, 11, 4, NULL, 0, 20, 8, 1) == -17);
    if (cmd == 6) assert(aimc_unregister_command_handler(imc, 11) == 0);
    return -17;
}
static void test_response(void)
{
    struct aimc imc; init(&imc); int payload = 3;
    assert(register_command_handler(&imc, 11, handler, &imc, 0) == 0);
    response_error = -8; responses = 0; reset_trace();
    assert(aimc_dispatch_command(&imc.imc_info, 11, 1, NULL, 0, 20, 7, 1) == -17);
    assert(responses == 1 && response_result == -17 && response_id == 7 && !strcmp(trace, "HR"));
    assert(imc.command_need_response); /* automatic path never clears the flag */
    responses = 0; reset_trace();
    assert(aimc_dispatch_command(&imc.imc_info, 11, 2, &payload, 4, 20, 7, 1) == -17);
    assert(responses == 1 && response_result == -23 && !strcmp(trace, "HRD"));
    assert(!imc.command_need_response);
    assert(aimc_command_response(&imc, 0, NULL, 0) == 0 && responses == 1);
    responses = 0;
    assert(aimc_dispatch_command(&imc.imc_info, 11, 3, NULL, 0, 20, 7, 1) == -17);
    assert(responses == 0); /* nested no-response dispatch clears outer slot */
    responses = 0;
    assert(aimc_dispatch_command(&imc.imc_info, 11, 5, NULL, 0, 20, 7, 1) == -17);
    assert(responses == 2 && response_id == 8 && response_cmd == 4);
    responses = 0; response_reenter = true;
    assert(aimc_dispatch_command(&imc.imc_info, 11, 2, NULL, 0, 20, 7, 1) == -17);
    assert(responses == 2 && response_result == 99);
    responses = 0;
    assert(aimc_dispatch_command(&imc.imc_info, 99, 1, NULL, 0, 20, 7, 1) == -1 && !responses);
    assert(aimc_dispatch_command(&imc.imc_info, 11, 6, NULL, 0, 20, 7, 0) == -17);
    assert(imc.command_list.next == &imc.command_list);
    cleanup(&imc);
    puts("PASS: inline automatic/early/error/reentrant responses, no handler pin, recursive slot loss");
}

static void test_wrappers(void)
{
    struct aimc imc; init(&imc); imc.aipc = (void *)1;
    transport_result = 0; reply_value = 0x7000123; calls = sends = ioctls = 0;
    assert(aimc_module_create(&imc, "sink", 0) == (unsigned)reply_value);
    assert(calls == 1 && sends == 0 && integers[0] == 7 && !strcmp(string_arg, "sink"));
    transport_result = -29; assert(aimc_module_create(&imc, "sink", 8) == 0);
    assert(aimc_module_destroy(&imc, 11) == -29 && !strcmp(last_name, "acont_module_destroy") && integers[0] == 11);
    assert(aimc_module_connect(&imc, 11, 2, 12, 3) == -29 && nintegers == 4 && integers[2] == 12);
    assert(aimc_module_disconnect_output(&imc, 11, 2) == -29 && !strcmp(last_name, "acont_module_disconnect_output"));
    assert(aimc_module_disconnect_input(&imc, 11, 2) == -29 && !strcmp(last_name, "acont_module_disconnect_input"));
    assert(calls == 2 && sends == 4 && ioctls == 0);
    transport_result = 9;
    assert(aimc_module_destroy(&imc, 11) == 9); /* send status is not normalized */
    assert(aimc_module_create(&imc, "sink", 8) == (unsigned)reply_value && integers[0] == 8);
    transport_result = -29;
    assert(aimc_add_processor_done(&imc) == -29);
    transport_result = 0; reply_value = -77; assert(aimc_add_processor_done(&imc) == 0);
    imc.aipc = NULL; imc.fd = 50;
    for (int fail = 0; fail < 2; fail++) {
        transport_result = fail ? -29 : 8;
        assert(aimc_module_create(&imc, "abcdefghijklmnopqrstuvwxyz0123456789", 0) == (fail ? 0u : 0x7000123u));
        assert(aimc_module_destroy(&imc, 11) == (fail ? -1 : 0));
        assert(last_request == AUIMC_IO_DESTROY_MODULE);
        assert(aimc_module_connect(&imc, 11, 2, 12, 3) == (fail ? -1 : 0));
        assert(aimc_module_disconnect_output(&imc, 11, 2) == (fail ? -1 : 0));
        assert(aimc_module_disconnect_input(&imc, 11, 2) == (fail ? -1 : 0));
    }
    send_error = 13;
    assert(aimc_send_command_sync(&imc, 77, 11, 12, &imc, 4) == -13);
    assert(sent_command.cmd == 77 && sent_command.param == &imc && sent_command.param_len == 4);
    assert(!sent_command.request_response && !sent_command.callback);
    aimc_wait_until_process_header(&imc); cleanup(&imc);
    puts("PASS: create call vs one-way teardown/connect, payloads, errors, send_sync is not a barrier");
}
static void test_finalize(void)
{
    struct aimc *imc = calloc(1, sizeof(*imc)); assert(imc); init(imc);
    imc->handler_thread = (struct aud_thread *)1; imc->running_handler = 1;
    imc->fd = 50; imc->aipc = (void *)1; imc->adbg = (void *)2;
    imc->ipc_header = malloc(8); assert(imc->ipc_header);
    final_imc = imc; final_header = imc->ipc_header;
    /* Mocks do not touch destroyed primitives; release them before the free. */
    cleanup(imc); reset_trace(); aimc_finalize(imc);
    assert(!strcmp(trace, "IsjccadhcF")); final_imc = final_header = NULL;
    puts("PASS: finalize IMC state before stop/join, no implicit pending-command drain");
}
int main(int argc, char **argv)
{
    if (argc > 1) {
        struct aimc imc; init(&imc); imc.fd = 50; imc.running_handler = 1;
        if (!strcmp(argv[1], "process-lock")) {
            assert(!pthread_mutex_lock(&imc.imc_process_lock));
            aimc_wait_until_process_header(&imc); return 1;
        }
        assert(!strcmp(argv[1], "self-sync"));
        self_sync = true; handler_thread(&imc); return 1;
    }
    test_sync(); test_response(); test_wrappers(); test_finalize(); return 0;
}
