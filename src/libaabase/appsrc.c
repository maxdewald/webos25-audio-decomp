/*
 * libaabase: "appsrc" module - entry point of application data into a libaa module
 * graph. lgaudiosink creates it by name through libaa (registry instance "appsrc"),
 * connects its output 0 to the renderer/bypass head and feeds it with
 * module_appsrc_push_data / module_appsrc_push_event.
 *
 * RECONSTRUCTION from orig/usr/lib/libaabase.so.1.0.0
 *   sha256 0d150201c8fb1dbda6c6769ed5d9254c21c98098fca31e2db33eb0b861f88806
 * Evidence:
 *   - aud_module_impl table at raw ELF .data 0x12018 (Ghidra 0x22018,
 *     R_ARM_RELATIVE relocs): name "appsrc" (tail of "aud_appsrc" @0x10eb0),
 *     constructor 0x10b58, destructor 0x10af8,
 *     chain 0x10ba0, event 0x10b00, cmd 0x10afc, instance_size 0x110, rest 0.
 *   - registered from init_array callback 0x10928: ModuleRegistry_Register(table).
 *   - logmm_module_aud_appsrc at raw .data 0x12004 (Ghidra 0x22004) =
 *     { {0,0}, 7, &logmm_dummy_mask, "aud_appsrc" }.
 *   Code addresses below use Ghidra's +0x10000 bias; exports retain Thumb bit 0.
 *   Static shell analysis only; no REA provider or target runtime was available.
 *   Ghidra created no functions for most static callbacks (only FUN_00010ba0); they are
 *   tagged anyway and show up as "not in symbol table" in progress.py.
 */
#include "aabase.h"

struct logmm_module logmm_module_aud_appsrc = {
    .initial_level = 7,
    .mask = &logmm_dummy_mask,
    .name = "aud_appsrc",
};

/* @0x10af8 (raw 0xaf8): no private queue or worker to release. libaa's
 * aud_module_destructor @0x2d14c disconnects peers, calls this, then deletes
 * outputs; del_output_port @0x2b7a8 puts their pending objects/current FIRST.
 * Destruction is not a delivery guarantee; callers must stop using the module. */
void appsrc_destructor(struct aud_module *module)
{
}

/* @0x10afc */
int appsrc_cmd(void *arg, unsigned int cmd, void *param, unsigned int len)
{
    return aud_module_default_command(arg, cmd, param, len);
}

/* @0x10b00: only logs; everything else is libaa's default handling */
int appsrc_event(struct aud_module *module, struct aud_port *port, struct aud_event *event)
{
    LOGI(aud_appsrc, 0x56, "event %d\n", event->type);
    return aud_module_default_event(module, port, event);
}

/* @0x10b58 */
int appsrc_constructor(struct aud_module *module, struct aimc *imc, const char *param)
{
    LOGI(aud_appsrc, 0x5d, "created\n");
    return 0;
}

/* Inlined into appsrc_chain and module_appsrc_push_data (keeps its own __func__). */
static inline void push_data_to_downstream(struct aud_appsrc *src, struct aud_data *data)
{
    src->input_count += data->size;
    LOGD(aud_appsrc, 0x31, "chain size %d, input_count:%lld\n", data->size, src->input_count);
    aud_module_push_data(&src->module, 0, data);
}

/* @0x10ba0: data arriving on an input port is passed straight through to output 0 */
int appsrc_chain(struct aud_port *port, struct aud_data *data)
{
    push_data_to_downstream((struct aud_appsrc *)port->module, data);
    return 0;
}

/* @0x10c0c (ELF Thumb symbol 0xc0d): always 0, NOT delivery/admission status.
 * libaa @0x2d454 transfers the supplied reference to output 0's pending array
 * under stream_lock (no copy, bounded admission test, or appsrc worker). Missing
 * output/drop-output-data/drop-output-meta release it instead. input_count counts
 * attempted bytes even on drops. NULL data is invalid here (size read first).
 * Exception: add_pending_obj_locked @0x2af48 realloc failure is ignored by libaa;
 * that path leaks the supplied ref, and can lose the old array. Do not interpret
 * the unconditional 0 as an allocation-success guarantee or add a retry put.
 *
 * sync=true calls aud_module_sync @0x2d3dc ON THIS THREAD: foreach_sync ->
 * aud_port_sync_locked @0x2b22c walks pending objects in insertion order, calls
 * each peer's chain under its input stream_lock, then writes any legacy buffer,
 * then puts pending refs. It is not a wait-for-worker-completion API. It can
 * block on port/list mutexes, a peer's block-module condition (@0x2c9c8), any
 * downstream chain, or a legacy ring while get_free_size < object size (no
 * timeout; check_free_cond wakes on reader progress or async FLUSH/DISCONNECT).
 * CommonBuffer_GetFreeSize @0x25258 reports zero on a full AU table as well as
 * byte exhaustion (4-byte gap); insert_au errors are ignored, not retried.
 * sync=false only skips THIS explicit sync: libaa's global sync_at_push
 * (@0x511a2, raw .bss 0x411a2, initially false, debug-toggleable) can still sync
 * inside push_data. Even without it, admission can wait for stream_lock held
 * by a concurrent sync; false is therefore not a nonblocking guarantee.
 *
 * lgao_feed uses true for main BEFORE bypass: main can block before bypass is
 * even called. Default aimc uses driver connections (aimc_use_aipc=0): the
 * kernel cross-processor path inserts gluepw/gluepr; Glue_Chain @0x2a4d4 and
 * its post-chain sync can wait on the legacy writer above. Remote DSP queue
 * admission/worker is NOT executed on this ARM call stack: a blocked DSP
 * worker can fill its queue and then the ring, eventually backpressuring ARM.
 * Alternate aipc gluew @0x29e1c waits in write_data @0x297a8 when rd!=wr and
 * (outstanding AUs > max_au_count OR ring space == 0), until progress/reset.
 *
 * appsrc neither clones nor changes data->info/type/timestamps/payload. Each
 * local peer gets aud_object_get of the SAME object; synchronous peer mutation
 * is visible before the later bypass push, or a retaining worker can race it.
 * A reference protects lifetime, not contents. Driver glue copies AU info and
 * payload to the inter-buffer; DSP queue/worker mutations after that hop affect
 * remote objects, not lgao's original. No live playback/thread timing inferred. */
int module_appsrc_push_data(struct aud_module *module, struct aud_data *data, bool sync)
{
    push_data_to_downstream((struct aud_appsrc *)module, data);
    if (sync)
        aud_module_sync(module, 0);
    return 0;
}

/* @0x10c98 (ELF Thumb symbol 0xc99): push then sync even on push failure;
 * always 0. libaa @0x2e138 creates output 0 if missing, consumes the event on
 * port allocation failure, and otherwise foreach_push_event @0x2de18 owns it.
 * SYNC events enter the SAME pending FIFO as data; no appsrc event queue exists.
 * Async events bypass that FIFO via async_down_lock, so can overtake data.
 * Async FLUSH/DISCONNECT marks output flushing and wakes legacy writers before
 * this wrapper's trailing sync takes stream_lock; input throwing/disabled makes
 * downstream drop data. Sync FLUSH clears flushing and travels in-band, not a
 * generic discard-pending call. Worker completion is not implied by return.
 * Enqueue allocation failure has the same ignored/leaked-ref caveat as data;
 * sync-event FIRST can additionally be retained as current_first until LAST or
 * output destruction. No extra put or validation here: the shipped wrapper
 * dereferences event only if logging is enabled and hides all lower errors. */
int module_appsrc_push_event(struct aud_module *module, struct aud_event *event)
{
    LOGI(aud_appsrc, 0x42, "event %d\n", event->type);
    aud_module_push_event(module, 0, event);
    aud_module_sync(module, 0);
    return 0;
}

void appsrc_register(void) __attribute__((constructor));

static struct aud_module_impl appsrc_impl = {
    .name = "appsrc",
    .constructor = appsrc_constructor,
    .destructor = appsrc_destructor,
    .chain = appsrc_chain,
    .event = appsrc_event,
    .cmd = appsrc_cmd,
    .instance_size = sizeof(struct aud_appsrc),
};

/* @0x10928: init_array constructor */
void appsrc_register(void)
{
    ModuleRegistry_Register(&appsrc_impl);
}
