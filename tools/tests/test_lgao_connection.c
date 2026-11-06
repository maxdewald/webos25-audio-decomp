#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "libgstlgaudiosink/lgao.h"
#include "include/lg_audio_policy.h"

#ifndef O_CLOEXEC
#define O_CLOEXEC 0x100000
#endif
#define AREN_SET_BYPASS_FADER 36
#define AREN_SET_QUEUE_MAX_LENGTH 39
#define LOGE(...) ((void)0)
#define LOGI(...) ((void)0)
#define LOGN(...) ((void)0)
struct aud_module { const char *name; };
struct module_appsink_callback { int unused; };
static struct module_appsink_callback lgao_appsink_cb;
static struct aud_module modules[3] = {{"main"}, {"bypass"}, {"sink"}};
static int opens, closes, initialized, signaled, creates, connects, callbacks;
static int destroyed, disconnected, fail_create, fail_lookup, fail_connect, fail_open;
static unsigned generation, selected_index, proc_id, bypass_requests, kernel_bypass;
static unsigned source[3], destination[3], source_port[3], commands[16], ncommands;

static int mock_open(const char *file, int flags)
{
    assert(!strcmp(file, "/dev/lg/aren0"));
    assert(flags == (O_RDWR | O_CLOEXEC));
    ++opens;
    return fail_open ? -1 : 7;
}
static int mock_close(int fd) { assert(fd == 7); ++closes; return 0; }
static int mock_ioctl(int fd, unsigned request, ...)
{
    assert(fd == 7 && ncommands < 16);
    commands[ncommands++] = request;
    /* The only no-argument request in the recovered function. */
    if (request == AREN_ADD_QUEUE) return -1;
    va_list args;
    va_start(args, request);
    void *argument = va_arg(args, void *);
    va_end(args);
    if (request == AREN_SET_INDEX) selected_index = *(unsigned *)argument;
    else if (request == AREN_SET_PROC_ID) proc_id = *(unsigned *)argument;
    else if (request == AREN_GET_UID) *(unsigned *)argument = 100;
    else if (request == AREN_GET_TAIL_UID) *(unsigned *)argument = 101;
    else if (request == AREN_GET_BYPASS_UID) {
        ++bypass_requests;
        *(unsigned *)argument = kernel_bypass;
    }
    return -1; /* The shipped implementation ignores all these ioctl results. */
}
static void _lgao_aimc_init(lgao *ao)
{
    ++initialized;
    ao->imc = (struct aimc *)(uintptr_t)1;
}
static unsigned aimc_get_processor_id(struct aimc *imc) { return 3; }
static unsigned aimc_module_create(struct aimc *imc, const char *name, int processor)
{
    assert(!processor && creates < 3);
    ++creates;
    assert(!strcmp(name, creates == 3 ? "appsink" : "appsrc"));
    return creates == fail_create ? 0 : generation + creates;
}
static void aimc_handler_sync(struct aimc *imc) { assert(creates == 2); }
static struct aud_module *ModuleRegistry_SearchInstByUid(unsigned uid)
{
    unsigned slot = uid - generation;
    assert(slot >= 1 && slot <= 3);
    return slot == (unsigned)fail_lookup ? NULL : &modules[slot - 1];
}
static int aimc_module_connect(struct aimc *imc, unsigned src, int out,
                               unsigned dst, int in)
{
    assert(connects < 3 && !in);
    source[connects] = src; destination[connects] = dst; source_port[connects] = out;
    ++connects;
    return connects == fail_connect;
}
static void module_appsink_set_callback(struct aud_module *module,
                                       struct module_appsink_callback *cb, void *priv)
{
    assert(module == &modules[2] && cb == &lgao_appsink_cb && priv);
    ++callbacks;
}
static int aimc_module_destroy(struct aimc *imc, unsigned uid) { assert(uid); ++destroyed; return 0; }
static int aimc_module_disconnect_output(struct aimc *imc, unsigned uid, int port)
{ assert(uid && !port); ++disconnected; return 0; }
static int aimc_module_disconnect_input(struct aimc *imc, unsigned uid, int port)
{ return aimc_module_disconnect_output(imc, uid, port); }
static void _lgao_set_msgpid_to_signal(lgao *ao) { ++signaled; }
#define open mock_open
#define close mock_close
#define ioctl mock_ioctl

// @extract src/libgstlgaudiosink/lgao.c _lgao_open_renderer _lgao_connect_renderer lgao_disconnect_renderer lgao_close_renderer lgao_open_renderer lgao_compare_audio_info

static lgao reset(void)
{
    opens = closes = initialized = signaled = creates = connects = callbacks = 0;
    destroyed = disconnected = fail_create = fail_lookup = fail_connect = fail_open = 0;
    generation = 10; ncommands = bypass_requests = 0; kernel_bypass = 200;
    selected_index = proc_id = ~0u;
    return (lgao){.fd = -1, .index = 2, .rendererState = LGAO_STATE_STOP};
}
static void check_mapping(void)
{
    for (int mixer = 0; mixer < 2; ++mixer) {
        lgao ao = reset(); ao.isMixer = mixer;
        assert(lgao_open_renderer(&ao) == 0 && ao.fd == 7);
        assert(selected_index == (unsigned)(2 + 4 * mixer));
        assert(proc_id == 3 && initialized == 1 && ao.rendererState == LGAO_STATE_INIT);
        assert(ncommands == 2 && commands[0] == AREN_SET_INDEX && commands[1] == AREN_SET_PROC_ID);
    }
    lgao ao = reset(); ao.index = -1;
    assert(lgao_open_renderer(&ao) == -1 && !opens);
    ao = reset(); fail_open = 1;
    assert(lgao_open_renderer(&ao) == -1 && !initialized);
}
static void check_reconnect(void)
{
    lgao ao = reset();
    assert(lgao_open_renderer(&ao) == 0);
    assert(_lgao_connect_renderer(&ao) == 0);
    assert(creates == 3 && connects == 3 && callbacks == 1 && signaled == 1);
    assert(source[0] == 11 && destination[0] == 100 && source_port[0] == 0);
    assert(source[1] == 101 && destination[1] == 13 && source_port[1] == 1);
    assert(source[2] == 12 && destination[2] == 200 && source_port[2] == 0);
    assert(bypass_requests == 1);
    unsigned old = ao.uid_bypass_appsrc;
    assert(lgao_disconnect_renderer(&ao) == 0 && destroyed == 3 && disconnected == 3);
    assert(!ao.appsrc && !ao.bypass_appsrc && !ao.appsink);
    assert(lgao_close_renderer(&ao) == 0 && closes == 1 && ao.fd == -1);
    for (int type = 0; type <= LGAO_DATA_METADATA; ++type) assert(ao.isFirstRender[type]);
    generation = 20; creates = connects = ncommands = 0; kernel_bypass = 201;
    assert(lgao_open_renderer(&ao) == 0 && initialized == 1);
    assert(_lgao_connect_renderer(&ao) == 0 && bypass_requests == 2);
    assert(ao.uid_bypass_appsrc != old && source[2] == 22 && destination[2] == 201);
}
static void check_failure_cleanup(void)
{
    for (int phase = 0; phase < 3; ++phase) {
        for (int point = 1; point <= 3; ++point) {
            lgao ao = reset(); ao.fd = 7;
            if (phase == 0) fail_create = point;
            else if (phase == 1) fail_lookup = point;
            else fail_connect = point;
            assert(_lgao_connect_renderer(&ao) == -1);
            assert(!ao.uid_appsrc && !ao.uid_bypass_appsrc && !ao.uid_appsink);
            assert(!ao.appsrc && !ao.bypass_appsrc && !ao.appsink);
            assert(ao.fd == 7 && !closes); /* Failure does not close the AREN fd. */
            assert(destroyed == (phase == 0 ? point - 1 : creates));
        }
    }
}
int main(void)
{
    check_mapping(); check_reconnect(); check_failure_cleanup();
    puts("lgao connection: index mapping, second connection and failure cleanup passed");
    return 0;
}
