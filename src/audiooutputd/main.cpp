#include "audiooutput.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <sys/signalfd.h>
#include <unistd.h>

struct GError { unsigned domain; int code; char* message; };
struct GOptionEntry { const char* name; char shortName; int flags; int arg; void* data; const char* description; const char* argument; };
extern "C" {
void* g_option_context_new(const char*);
void g_option_context_add_main_entries(void*, const GOptionEntry*, const char*);
int g_option_context_parse(void*, int*, char***, GError**);
void g_option_context_free(void*);
void g_error_free(GError*);
void* g_main_loop_new(void*, int);
void g_main_loop_run(void*);
void g_main_loop_unref(void*);
void g_main_loop_quit(void*);
void* g_io_channel_unix_new(int);
int g_io_channel_unix_get_fd(void*);
void g_io_channel_set_close_on_unref(void*, int);
int g_io_channel_set_encoding(void*, const char*, GError**);
void g_io_channel_set_buffered(void*, int);
unsigned g_io_add_watch(void*, unsigned, int (*)(void*, unsigned, void*), void*);
void g_io_channel_unref(void*);
bool LSGmainAttach(LSHandle*, void*, LSError*);
bool LSSetDisconnectHandler(LSHandle*, void (*)(LSHandle*, void*), void*, LSError*);
void LSErrorLogDefault(const char*, LSError*);
}
static void* loop;
static bool terminated;
static int version;
static const GOptionEntry options[] = {
    {"version", 'v', 0, 0, &version, "Show version information and exit", ""},
    {nullptr, ' ', 0, 0, nullptr, nullptr, nullptr}
};

// Raw a548 and a55c callbacks are omitted from Ghidra's function inventory.
static void disconnected(LSHandle*, void*)
{
    terminated = true;
    g_main_loop_quit(loop);
}
static int signalHandler(void* channel, unsigned condition, void*)
{
    if (condition & 0x38) return 0;
    signalfd_siginfo info;
    if (read(g_io_channel_unix_get_fd(channel), &info, sizeof(info)) != sizeof(info)) return 0;
    if (info.ssi_signo == SIGUSR1) std::exit(1);
    if (info.ssi_signo == SIGINT || info.ssi_signo == SIGTERM) {
        if (!terminated) g_main_loop_quit(loop);
        terminated = true;
    }
    return 1;
}
static unsigned setupSignals()
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &mask, nullptr) < 0) {
        std::cerr << "Failed to set signal mask";
        return 0;
    }
    int fd = signalfd(-1, &mask, 0);
    if (fd < 0) { std::cerr << "Failed to create signal descriptor"; return 0; }
    void* channel = g_io_channel_unix_new(fd);
    g_io_channel_set_close_on_unref(channel, 1);
    g_io_channel_set_encoding(channel, nullptr, nullptr);
    g_io_channel_set_buffered(channel, 0);
    unsigned source = g_io_add_watch(channel, 0x39, signalHandler, nullptr);
    g_io_channel_unref(channel);
    return source;
}

extern "C" LS::Handle* audiooutput_unregister(LS::Handle*);
/* @0x1a618 = audiooutput_unregister */
/* Teardown logs unregister failure, does not throw and does not zero the handle. */
extern "C" LS::Handle* audiooutput_unregister(LS::Handle* handle)
{
    if (handle->handle) {
        LS::Error error;
        if (!LSUnregister(handle->handle, &error.error))
            LSErrorLogDefault("LS_FAILED_TO_UNREG", &error.error);
    }
    return handle;
}
struct BusOwner : LS::Handle { ~BusOwner() { audiooutput_unregister(this); } };

/* @0x144dc = main */
/* raw 463a registers com.webos.service.audiooutput (NO trailing d).
 * Creates volume, connection, quality categories in that order after UMI init.
 * No settings database, file restore, or mixer persistence load in startup.
 * --version is parsed but not acted on in this shipped main. Diagnostic logging
 * is abbreviated; signal handling/LS2 and UMI failure/lifetime ordering retained.
 * [GAP] Exact external policy/TV permissions and successful live calls unobserved. */
int main(int argc, char** argv)
{
    GError* error = nullptr;
    void* optionsContext = g_option_context_new(nullptr);
    g_option_context_add_main_entries(optionsContext, options, nullptr);
    if (!g_option_context_parse(optionsContext, &argc, &argv, &error)) {
        std::cerr << "[audiooutputd] " << (error ? error->message : "An unknown error occurred") << std::endl;
        if (error) g_error_free(error);
        std::exit(1);
    }
    g_option_context_free(optionsContext);
    if (setPmLogContext("audiooutputd")) {
        std::cerr << "[audiooutputd] Failed to setup up log context audiooutputd" << std::endl;
        std::exit(1);
    }
    loop = g_main_loop_new(nullptr, 0);
    unsigned signals = setupSignals();
    umiClient* client = umiClient::getInstance();
    try {
        if (!client || !client->initialize()) throw "stop AudiooutputD Service";
        BusOwner bus{};
        { LS::Error e; if (!LSRegister("com.webos.service.audiooutput", &bus.handle, &e.error)) throw e; }
        VolumeManager volume(bus, client);
        ConnectionManager connections(bus, volume, client);
        QualityManager quality(bus, client);
        { LS::Error e; if (!LSGmainAttach(bus.handle, loop, &e.error)) throw e; }
        { LS::Error e; if (!LSSetDisconnectHandler(bus.handle, disconnected, nullptr, &e.error)) throw e; }
        g_main_loop_run(loop);
    } catch (const std::exception& e) {
        std::cerr << "[audiooutputd] Caught exception: '" << e.what() << "' exiting" << std::endl;
        std::exit(1);
    } catch (...) {
        std::cerr << "[audiooutputd] Caught exception, exiting" << std::endl;
        std::exit(1);
    }
    g_source_remove(signals);
    g_main_loop_unref(loop);
    if (!client->deinitialize())
        _PmLogMsgKV(getPmLogContext(), 3, 0, "HAL_DEINIT_ERROR", 0, 0, 0,
                    "UMI deinitialization error. See logs for details.");
    return 0;
}
