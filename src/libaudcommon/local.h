/*
 * Private helpers for the libaudcommon reconstruction: liblogm module objects and
 * the logging macro every function uses.
 *
 * RECONSTRUCTION from orig/usr/lib/libaudcommon.so.1.0.0
 *   sha256 a4df15e082096e6c60c7091b81f90f4fe0a6c5ae07173867ca4a0b05bfd5087f
 *
 * The six exported logmm_module_* objects (20 bytes each, .data 0x16004..0x167a8) are
 * defined in util.c. The struct layout (index, flags, _mask, mask, name) is from the
 * DWARF of libgstlgaudiosink.so. The macros test (*mask >> level) & 1, as in the binary.
 */
#ifndef AUDCOMMON_LOCAL_H
#define AUDCOMMON_LOCAL_H

#include <stdarg.h>
#include "audcommon.h"

struct logmm_module {
    int index;
    unsigned int flags;
    unsigned int _mask;             /* initial level mask: 0x3f for ainst, 7 otherwise */
    unsigned int *mask;             /* -> logmm_dummy_mask until liblogm registers it */
    const char *name;
};

enum log_level {
    log_level_error = 0,
    log_level_warning = 1,
    log_level_noti = 2,
    log_level_info = 3,
    log_level_debug = 4,
    log_level_trace = 5,
};

/* imported from liblogm */
extern unsigned int logmm_dummy_mask;
extern int logmm_printl(struct logmm_module *m, int level, const char *func, int line,
                        const char *fmt, ...) __attribute__((format(printf, 5, 6)));
extern int logmm_vprintl(struct logmm_module *m, int level, const char *func, int line,
                         const char *fmt, va_list ap);

extern struct logmm_module logmm_module_ainst, logmm_module_audlog, logmm_module_aud_bufadp,
    logmm_module_aud_bufdata, logmm_module_aud_meta_bufdata, logmm_module_comutil;

#define LOG_ON(mod, lvl)    ((*logmm_module_##mod.mask >> (lvl)) & 1)

/* line = __LINE__ of the original build (kept as evidence, matches the binary) */
#define LOG(mod, lvl, line, fmt, ...) do { \
        if (LOG_ON(mod, lvl)) \
            logmm_printl(&logmm_module_##mod, (lvl), __func__, (line), fmt, ##__VA_ARGS__); \
    } while (0)

#define LOGE(mod, line, fmt, ...)   LOG(mod, log_level_error, line, fmt, ##__VA_ARGS__)
#define LOGN(mod, line, fmt, ...)   LOG(mod, log_level_noti,  line, fmt, ##__VA_ARGS__)
#define LOGI(mod, line, fmt, ...)   LOG(mod, log_level_info,  line, fmt, ##__VA_ARGS__)
#define LOGD(mod, line, fmt, ...)   LOG(mod, log_level_debug, line, fmt, ##__VA_ARGS__)
#define LOGT(mod, line, fmt, ...)   LOG(mod, log_level_trace, line, fmt, ##__VA_ARGS__)

/* unnamed helpers that exist as separate code in the binary */
int check_str_term(const void *data, unsigned int size);
struct aud_data_holder *new_holder(struct aud_data_adapter *adap);
struct aud_event_value *get_value(struct aud_event *e, const char *name);
void event_destroy(struct aud_object *obj);
void data_destroy(struct aud_object *obj);
void aud_data_adapter_push_at(struct aud_data_adapter *adap, struct aud_data *data, int front)
    __attribute__((noinline));

#endif /* AUDCOMMON_LOCAL_H */
