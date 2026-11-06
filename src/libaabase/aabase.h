/*
 * libaabase public API (installed as <aa/module/appsrc.h>, <aa/module/appsink.h>
 * and the dlload helpers) plus the private instance layouts of the two modules.
 *
 * RECONSTRUCTION from orig/usr/lib/libaabase.so.1.0.0
 *   sha256 0d150201c8fb1dbda6c6769ed5d9254c21c98098fca31e2db33eb0b861f88806
 * Prototypes and the module_appsink_callback / appsink_event_return types come from
 * the DWARF of usr/lib/gstreamer-1.0/libgstlgaudiosink.so (decl_file
 * /usr/include/aa/module/appsink.h). Instance sizes: impl->instance_size == 0x110
 * in both aud_module_impl tables (raw ELF .data 0x12018 and 0x1208c;
 * Ghidra addresses 0x22018 and 0x2208c).
 */
#ifndef LIBAABASE_AABASE_H
#define LIBAABASE_AABASE_H

#include <stdbool.h>
#include "../libaa/types.h"

/* ---- appsrc: application -> module graph ---------------------------------- */

struct aud_appsrc {
    struct aud_module module;
    unsigned long long input_count;      /* 0x108: bytes pushed downstream */
};

/* Transfer one data reference to output 0 (no copy); sync=true forwards pending
 * objects on the caller's thread and CAN block before returning. false is not a
 * nonblocking guarantee (locks/global sync_at_push). Always 0, even on drops;
 * libaa enqueue OOM can leak the reference. See appsrc.c for ownership details. */
int module_appsrc_push_data(struct aud_module *module, struct aud_data *data, bool sync);
/* Transfer one event reference, then sync output 0; always 0. SYNC events share
 * the data FIFO, async events may overtake it; neither API awaits remote render. */
int module_appsrc_push_event(struct aud_module *module, struct aud_event *event);

/* ---- appsink: module graph -> application --------------------------------- */

typedef enum appsink_event_return {
    appsink_event_handled,
    appsink_event_not_handled,
} appsink_event_return;

typedef struct module_appsink_callback {
    int (*handle_data)(void *priv, int input_port_num, struct aud_data *data);
    appsink_event_return (*handle_event)(void *priv, int input_port_num,
                                         struct aud_event *event);
} module_appsink_callback;

struct aud_appsink {
    struct aud_module module;
    module_appsink_callback *callback;   /* 0x108 */
    void *priv;                          /* 0x10c */
};

int module_appsink_set_callback(struct aud_module *module, module_appsink_callback *cb,
                                void *priv);

/* ---- plugin loading -------------------------------------------------------- */

/* dlopen() every non-dot entry of dir; -1 if dir is NULL or unreadable. */
int dlload_dir(const char *dir);
/* dlload_dir() for each entry of a NULL-terminated list; always 0. */
int dlload_dirs(const char **dirs);

/* ---- logging modules exported by this library ------------------------------ */

extern unsigned int logmm_dummy_mask;
extern struct logmm_module logmm_module_aud_appsrc, logmm_module_aud_appsink;

#if defined(__arm__) && !defined(__aarch64__)
_Static_assert(sizeof(struct aud_module) == 0x108, "aud_module base size");
_Static_assert(sizeof(struct aud_appsrc) == 0x110, "appsrc instance_size");
_Static_assert(sizeof(struct aud_appsink) == 0x110, "appsink instance_size");
_Static_assert(sizeof(struct logmm_module) == 0x14, "logmm_module size");
#endif

#endif
