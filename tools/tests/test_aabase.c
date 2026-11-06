/* Host mocks, not ARM execution. */
#include <assert.h>
#include <dirent.h>
#include <dlfcn.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "../../src/libaabase/appsrc.c"

static DIR *mock_opendir(const char *name);
static struct dirent *mock_readdir(DIR *directory);
static int mock_closedir(DIR *directory);
static void *mock_dlopen(const char *name, int flags);
static char *mock_dlerror(void);
static int mock_fprintf(FILE *stream, const char *format, ...);
#define opendir mock_opendir
#define readdir mock_readdir
#define closedir mock_closedir
#define dlopen mock_dlopen
#define dlerror mock_dlerror
#define fprintf mock_fprintf
#include "../../src/libaabase/dlload.c"
#undef opendir
#undef readdir
#undef closedir
#undef dlopen
#undef dlerror
#undef fprintf

unsigned int logmm_dummy_mask;
static struct aud_module_impl *registered;
static struct aud_appsrc *source;
static struct aud_data *expected_data;
static unsigned long long expected_count;
static unsigned int expected_size;
static char calls[32];
static unsigned int call_count;
static int directory_token;
static DIR *const directory = (DIR *)&directory_token;
static struct dirent entries[6];
static unsigned int next_entry, entry_count, loads, errors, closes, opens;
static int fail_open;
static const char *expected_dir;
static char loaded_paths[4][1600];
static char error_text[] = "mock loader error";

static void record(char call)
{
    assert(call_count + 1 < sizeof(calls));
    calls[call_count++] = call;
    calls[call_count] = 0;
}

void ModuleRegistry_Register(struct aud_module_impl *impl)
{
    assert(!registered);
    registered = impl;
}

int aud_module_default_command(struct aud_module *arg, unsigned int cmd, void *param,
                               unsigned int len)
{
    return -17;
}

int aud_module_default_event(struct aud_module *module, struct aud_port *port,
                             struct aud_event *event)
{
    return -18;
}

int aud_module_push_event(struct aud_module *module, int port, struct aud_event *event)
{
    return -19;
}

int aud_module_push_data(struct aud_module *module, int port, struct aud_data *data)
{
    assert(module == &source->module && port == 0 && data == expected_data);
    assert(source->input_count == expected_count);
    record('P');
    return -23;
}

int aud_module_sync(struct aud_module *module, int port)
{
    assert(module == &source->module && port == 0);
    assert(call_count && calls[call_count - 1] == 'P');
    record('S');
    return -29;
}

int logmm_printl(struct logmm_module *module, int level, const char *func, int line,
                 const char *format, ...)
{
    va_list ap;
    assert(module == &logmm_module_aud_appsrc && level == log_level_debug);
    assert(strcmp(func, "push_data_to_downstream") == 0 && line == 0x31);
    assert(strcmp(format, "chain size %d, input_count:%lld\n") == 0);
    va_start(ap, format);
    assert(va_arg(ap, unsigned int) == expected_size);
    assert(va_arg(ap, unsigned long long) == expected_count);
    va_end(ap);
    assert(source->input_count == expected_count);
    expected_data->size = 99;
    record('L');
    return -31;
}

static DIR *mock_opendir(const char *name)
{
    assert(name == expected_dir);
    ++opens;
    return fail_open ? NULL : directory;
}

static struct dirent *mock_readdir(DIR *d)
{
    assert(d == directory);
    return next_entry < entry_count ? &entries[next_entry++] : NULL;
}

static int mock_closedir(DIR *d)
{
    assert(d == directory);
    ++closes;
    return -1;
}

static void *mock_dlopen(const char *name, int flags)
{
    assert(flags == RTLD_LAZY && loads < 4);
    assert(strlen(name) < sizeof(loaded_paths[0]));
    strcpy(loaded_paths[loads++], name);
    return strstr(name, "broken") ? NULL : &directory_token;
}

static char *mock_dlerror(void)
{
    assert(loads == 2 && errors == 0);
    ++errors;
    return error_text;
}

static int mock_fprintf(FILE *stream, const char *format, ...)
{
    va_list ap;
    assert(stream == stderr && errors == 1);
    assert(strcmp(format, "%s.%d: cannot load \"%s\". %s\n") == 0);
    va_start(ap, format);
    assert(strcmp(va_arg(ap, const char *), "load_dl") == 0);
    assert(va_arg(ap, int) == 25);
    assert(strcmp(va_arg(ap, const char *), loaded_paths[1]) == 0);
    assert(va_arg(ap, char *) == error_text);
    va_end(ap);
    return -1;
}

static void test_push_data(void)
{
    struct aud_appsrc src = {0};
    struct aud_data data = {0};
    const unsigned int sizes[] = {0, 1, UINT_MAX};
    const unsigned long long counts[] = {0, 0xffffffffULL, ULLONG_MAX};
    source = &src;
    expected_data = &data;
    for (unsigned int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        for (unsigned int j = 0; j < sizeof(counts) / sizeof(counts[0]); ++j) {
            for (int logging = 0; logging < 2; ++logging) {
                for (int sync = 0; sync < 2; ++sync) {
                    data.size = expected_size = sizes[i];
                    src.input_count = counts[j];
                    expected_count = counts[j] + sizes[i];
                    logmm_dummy_mask = logging ? (1u << log_level_debug) : 0;
                    call_count = 0;
                    calls[0] = 0;
                    assert(module_appsrc_push_data(&src.module, &data, sync) == 0);
                    assert(strcmp(calls, logging ? (sync ? "LPS" : "LP") :
                                                   (sync ? "PS" : "P")) == 0);
                    assert(src.input_count == expected_count);
                }
            }
        }
    }
    struct aud_port port = {.module = &src.module};
    data.size = expected_size = 7;
    expected_count = src.input_count + 7;
    call_count = 0;
    assert(registered->chain(&port, &data) == 0);
    assert(strcmp(calls, "LP") == 0);
}

static void reset_directory(const char *name)
{
    expected_dir = name;
    fail_open = 0;
    next_entry = entry_count = loads = errors = closes = opens = 0;
    memset(entries, 0, sizeof(entries));
}

static void test_loader(void)
{
    reset_directory("plugins");
    assert(dlload_dir(NULL) == -1 && opens == 0);
    fail_open = 1;
    assert(dlload_dir(expected_dir) == -1 && opens == 1 && closes == 0);
    reset_directory("empty");
    assert(dlload_dir(expected_dir) == 0 && closes == 1 && loads == 0);
    const char *names[] = {".", "..", ".hidden.so", "one.so", "broken.so", "last"};
    char long_dir[1024];
    memset(long_dir, 'x', sizeof(long_dir) - 1);
    long_dir[sizeof(long_dir) - 1] = 0;
    const char *directories[] = {"plugins", "plugins/", "", long_dir};
    for (unsigned int i = 0; i < sizeof(directories) / sizeof(directories[0]); ++i) {
        reset_directory(directories[i]);
        entry_count = sizeof(names) / sizeof(names[0]);
        for (unsigned int j = 0; j < entry_count; ++j)
            strcpy(entries[j].d_name, names[j]);
        assert(dlload_dir(expected_dir) == 0);
        assert(loads == 3 && errors == 1 && closes == 1 && opens == 1);
        for (unsigned int j = 0; j < loads; ++j) {
            char expected[1600];
            snprintf(expected, sizeof(expected), "%s/%s", expected_dir, names[j + 3]);
            assert(strcmp(loaded_paths[j], expected) == 0);
        }
    }
    reset_directory("unreadable");
    fail_open = 1;
    const char *dirs[] = {expected_dir, expected_dir, NULL};
    assert(dlload_dirs(dirs) == 0 && opens == 2 && closes == 0);
}

int main(void)
{
    assert(registered && strcmp(registered->name, "appsrc") == 0);
    test_push_data();
    test_loader();
    puts("aabase: push-data/chain and directory-loader regression tests passed");
    return 0;
}
