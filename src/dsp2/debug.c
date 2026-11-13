/*
 * dsp2 property diagnostics and the integer GET_PROP distinction.
 * Reconstruction: audio_a0_dsp2.bin, sha256
 * c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d;
 * main image sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd.
 * Evidence: build/scratch/kprop/{audreg_print_property,get_prop,prop_str}.lst,
 * ipcm_writedebug.od, call_function.od; tools/xtobjdump.sh for branch slots.
 *
 * audreg_print_property emits magic start/end via logm_message @6007af14 ->
 * IPC_WriteDebugV @6006f014 -> AUD_IPCM_WriteDebug @6006ee18. It formats ALL
 * properties (default, class, runtime descriptors), invoking string getters.
 * No generic module mutex is taken by aud_module_prop_str @60075d24.
 * A particular custom getter can still lock or block. Formatting one property
 * via integer GET_PROP avoids this all-property traversal and the log capture.
 *
 * The debug writer does not guarantee delivery: ring full @6006ef30/6006ef53
 * drops the message, sets *(u8 *)0x601282b5=1, returns 7; the next successful
 * message gets flag bit1 (DROPPED). Neither marker send here checks its result.
 * A missing marker leaves the kernel's proc read waiting indefinitely.
 *
 * Command scheduling: aimc_start @60079efc registers the IRQ callback at
 * 60079c2c (misnamed aimc_ipc_header_get in the recovered symbol table).
 * It copies incoming headers to a list and @60079c98..60079c9e queues scheduler
 * job 0x60101298 on queue 1. That job's function is ipc_header_process @60079974,
 * which calls IMC_ProcessHeader @6006da08 -> aimc_dispatch_command @6007a450.
 * sched_init @6007c46c creates queue 1's thread at priority 10; sched_process
 * @6007c3f8 invokes one job at a time. Command getters and the proc dump both
 * need this queue to run; an asynchronous ARM request cannot bypass a stuck DSP.
 * Queue 0 is run by sched_mai in the main thread. These are static facts, not
 * evidence that scheduler queue 1 is blocked during the user's playback.
 *
 * GET_PROP @60076444 returns aud_module_get_prop @60075b74 as the IMC result:
 * gets-only descriptors return -1 (60075c3e..60075c64); geti @60075c18 works;
 * int offset load @60075cab and bool byte load @60075cd6 work. No response text
 * parsing is involved. Delay queued-length is gets-only, unlike fader's
 * queued-length which supplies both gets and geti. Gain es is not a descriptor.
 */
#include <stddef.h>
#include <stdint.h>

extern int strcmp(const char *, const char *);
extern unsigned long strtoul(const char *, char **, int);
extern void *ModuleRegistry_SearchInstByUid(unsigned uid);
extern int logm_message(const char *fmt, ...);
extern char *aud_module_prop_str(void *module, const char *format);
extern void aud_free(void *);

/* @0x600708d4: argv[1] is the kernel's magic; argv[2] is a numeric uid. */
int audreg_print_property(int argc, char **argv)
{
    void *module;
    char *text;
    if (argc < 3)
        return -1;
    module = ModuleRegistry_SearchInstByUid((unsigned)strtoul(argv[2], NULL, 0));
    logm_message("%s: start\n", argv[1]);
    if (module) {
        text = aud_module_prop_str(module, "%s=%s\n");
        logm_message("%s", text);
        aud_free(text); /* actual free target 600e9550; duplicate symbol alias */
    } else {
        logm_message("no instance for uid %s\n", argv[2]);
    }
    logm_message("%s: end\n", argv[1]);
    return 0;
}

/* Offline verification: build/scratch/dsp-discovery-verify/ contains listings
 * from the hashed main image above, not runtime observations. Unlike
 * audreg_print_property, lstatus addresses the legacy module-id namespace.
 * Port/peer discovery below walks the modern aud_module graph instead.
 */
extern volatile uint32_t *aimc_debug_control; /* MMIO 0xc10102ec */
extern volatile uint32_t *aimc_debug_log_mask; /* 0x4ff90060 */
extern void *aimc_debug_log_ctx;              /* 0x60101280 */
extern const char *ModuleList_GetModuleName(unsigned id);
extern void DebugPrint_SetMask(unsigned mask);
extern void logmm_printl(void *, int, const char *, int, const char *, ...);
struct imc;
extern int aimc_dispatch_command(struct imc *, unsigned, unsigned, void *, int,
                                 unsigned, unsigned, int);
typedef int (*debug_function)(int argc, char **argv, int flags);
extern debug_function symbol_get_add(const char *name);

/* @0x6007a714: magic-delimited legacy status; argc is ignored. The mask change
 * is not restored, and the dispatch result does not affect the return value.
 * The jump at 6007a789 rejoins this function; it is not a second handler.
 */
int debug_aimc_lstatus(const char *magic, int argc, char **argv)
{
    unsigned id = (unsigned)strtoul(argv[0], NULL, 0);

    (void)argc;
    if (*aimc_debug_log_mask & 8)
        logmm_printl(aimc_debug_log_ctx, 3, "debug_aimc_lstatus", 0x4c2,
                     "legacy module status. id %d(0x%x, %s), magic: %s\n",
                     id, id, ModuleList_GetModuleName(id), magic);
    DebugPrint_SetMask(*aimc_debug_control | 0x200000);
    logm_message("%s: start\n", magic);
    aimc_dispatch_command(NULL, id, 0x10070000, NULL, 0, 0, 0, 0);
    logm_message("%s: end\n", magic);
    return 0;
}

/* @0x6007a790: argv[0] remains the function name; no argument shifting. */
int debug_aimc_fcall(const char *magic, int argc, char **argv)
{
    debug_function function = symbol_get_add(argv[0]);

    logm_message("%s: start\n", magic);
    if (function)
        function(argc, argv, 0);
    else
        logm_message("no such a function. %s\n", argv[0]);
    logm_message("%s: end\n", magic);
    return 0;
}

/* @0x6007a8d0: argv is null-terminated by the caller. Unknown subcommands and
 * missing magic/command/argument are silent success; there is no argc guard.
 */
int debug_aimc2(int argc, char **argv)
{
    const char *magic = argv[1];
    const char *command = argv[2];

    if (magic && command) {
        if (!strcmp(command, "lstatus") && argv[3]) {
            debug_aimc_lstatus(magic, argc - 3, argv + 3);
            return 0;
        }
        if (!strcmp(command, "fcall") && argv[3]) {
            debug_aimc_fcall(magic, argc - 3, argv + 3);
            return 0;
        }
    }
    return 0;
}

/* These are the discovery fields only. Padding preserves the shipped ILP32
 * offsets; native-width pointers also let host tests build synthetic lists.
 */
struct debug_link {
    struct debug_link *prev, *next;
};
struct debug_link_node {
    struct debug_link link;
    unsigned char unused[0x20];
    void *data;                              /* +0x28 */
};
struct debug_root {
    struct debug_link nodes;
    unsigned char mutex[0x24];               /* root +0x08 */
};
struct debug_impl {
    void *unused;
    const char *name;                        /* +0x04 */
};
struct debug_module {
    const char *name;
    unsigned uid;
    unsigned log_mask;
    struct debug_impl *impl;                 /* +0x0c */
    struct debug_root inputs;                /* +0x10, mutex +0x18 */
    struct debug_root outputs;               /* +0x3c, mutex +0x44 */
};
struct debug_port {
    int number;
    struct debug_module *module;
    unsigned flag;
    unsigned char unused_0c[0xc0 - 0x0c];
    struct debug_root down_peers;            /* +0xc0, mutex +0xc8 */
    struct debug_port *up_peer;              /* +0xec */
    void **pending_objs;                    /* +0xf0, local object pointers */
    int pending_obj_alloced;                 /* +0xf4 */
    int pending_obj_num;                     /* +0xf8 */
    unsigned char unused_fc[0x13c - 0xfc];
    void *legacy_buf;                        /* +0x13c */
};
#if UINTPTR_MAX == UINT32_MAX
_Static_assert(offsetof(struct debug_link_node, data) == 0x28, "link data");
_Static_assert(offsetof(struct debug_root, mutex) == 8, "root mutex");
_Static_assert(offsetof(struct debug_module, inputs) == 0x10, "module inputs");
_Static_assert(offsetof(struct debug_module, outputs) == 0x3c, "module outputs");
_Static_assert(offsetof(struct debug_port, down_peers) == 0xc0, "downstream peers");
_Static_assert(offsetof(struct debug_port, up_peer) == 0xec, "upstream peer");
_Static_assert(offsetof(struct debug_port, pending_objs) == 0xf0, "pending array");
_Static_assert(offsetof(struct debug_port, pending_obj_num) == 0xf8, "pending count");
_Static_assert(offsetof(struct debug_port, legacy_buf) == 0x13c, "legacy buffer");
#endif
extern void aud_mutex_lock(void *);
extern void aud_mutex_unlock(void *);

static struct debug_port *debug_link_port(struct debug_link *link)
{
    return ((struct debug_link_node *)link)->data;
}

static void debug_port_fields(struct debug_port *port)
{
    logm_message("  port=%p\n", (void *)port);
    logm_message("  number=%d\n", port->number);
    logm_message("  pending_obj_alloced=%d\n", port->pending_obj_alloced);
    logm_message("  pending_obj_num=%d\n", port->pending_obj_num);
    logm_message("  legacy_buf=%p\n", port->legacy_buf);
}

/* @0x6007037c: pending_obj_alloced/num are pointer slots, not byte sizes.
 * add_pending_obj_locked @60072078 indexes +0xf0 with a four-byte stride;
 * object +4 distinguishes 'data' (0x64617461) from 'evnt' (0x65766e74).
 * aud_data_new @6006b198 confirms data bytes at +0x34, payload pointer +0x38;
 * subtype is +0x10. aud_event_new @60069a64 confirms event payload bytes at
 * +0x20, inline payload +0x24; event type is +0x0c. Event payload length is
 * not its serialized size (properties are separate). Never apply data offsets
 * to events or equate aud_data subtype with legacy AU dataType.
 * This dump does not traverse those objects or dereference legacy_buf.
 * It takes list mutexes, so it can block, and its logging changes debug state;
 * it is not a lock-free, side-effect-free or coherent occupancy snapshot.
 * Inputs expose one upstream pointer; outputs enumerate all
 * downstream peers. Output-root lock stays held while each peer-root is locked.
 * FLIX branch slots at 60070424/600704a7 confirm empty-list and loop-back tests.
 */
int dump_inst(struct debug_module *module)
{
    struct debug_link *link, *next;

    logm_message("instance %p\n", (void *)module);
    logm_message("uid=%08x\n", module->uid);
    logm_message("implement=%p(%s)\n", (void *)module->impl, module->impl->name);
    logm_message("input ports..\n");
    aud_mutex_lock(module->inputs.mutex);
    for (link = module->inputs.nodes.next, next = link->next;
         link != &module->inputs.nodes; link = next, next = next->next) {
        struct debug_port *port = debug_link_port(link);
        debug_port_fields(port);
        logm_message("  up_peer=%p\n", (void *)port->up_peer);
        logm_message("  flag=%x\n", port->flag);
    }
    aud_mutex_unlock(module->inputs.mutex);
    logm_message("ouput ports..\n");
    aud_mutex_lock(module->outputs.mutex);
    for (link = module->outputs.nodes.next, next = link->next;
         link != &module->outputs.nodes; link = next, next = next->next) {
        struct debug_port *port = debug_link_port(link);
        struct debug_link *peer, *peer_next;
        debug_port_fields(port);
        logm_message("  down_peers..\n");
        aud_mutex_lock(port->down_peers.mutex);
        for (peer = port->down_peers.nodes.next, peer_next = peer->next;
             peer != &port->down_peers.nodes;
             peer = peer_next, peer_next = peer_next->next)
            logm_message("    port=%p\n", (void *)debug_link_port(peer));
        aud_mutex_unlock(port->down_peers.mutex);
        logm_message("  flag=%x\n", port->flag);
    }
    aud_mutex_unlock(module->outputs.mutex);
    logm_message("\n");
    return 0;
}

/* Ordinary offline instruction evidence: listings in build/scratch/diag-buf/;
 * CommonBuffer name/function pairs checked in the firmware symbol table.
 * This is the local ADEC_BUF_T prefix, NOT the shared commonBuf allocation.
 * Its status may be stale: neither getter refreshes shared registers, takes
 * a lock, nor provides an atomic snapshot. Caller must ensure valid lifetime,
 * aui_count > 0, valid indices/offsets and externally stable cached status.
 * DSP writer wrappers @60065e38/60065ed0/60065f60 mask IRQs and store saved PS
 * at ADEC_BUF_T +0xf4 before these queries; they are not strictly read-only.
 * Those normal paths do not refresh shared status. No live call is prescribed.
 * These native signatures are NOT the argc/argv debug fcall interface.
 */
struct debug_adec_status {
    int32_t index_allocated, read_offset, write_offset;
    int16_t read_index, write_index;
    int32_t written, passed, repeat_count;
};
struct debug_adec_buf {
    struct debug_adec_status status;
    uint32_t common_buf_address;             /* DSP address, not host pointer */
    uint32_t max_access_size, max_size, aui_count;
};
_Static_assert(sizeof(struct debug_adec_status) == 0x1c, "cached status size");
_Static_assert(offsetof(struct debug_adec_buf, max_size) == 0x24, "buffer size");
_Static_assert(offsetof(struct debug_adec_buf, aui_count) == 0x28, "AU slots");

/* @0x600635cc: AU-slot exhaustion overrides free bytes; four-byte guard.
 * No buffer writes, callbacks, logging or locks in this function itself.
 */
int32_t CommonBuffer_GetFreeSize(const struct debug_adec_buf *buf)
{
    uint32_t next = (uint32_t)((int32_t)buf->status.write_index + 1) % buf->aui_count;
    if (next == (uint32_t)(int32_t)buf->status.read_index)
        return 0;
    uint32_t free_size = (uint32_t)buf->status.read_offset -
                         (uint32_t)buf->status.write_offset;
    if (buf->status.read_offset <= buf->status.write_offset)
        free_size += buf->max_size;
    return (int32_t)(free_size - 4u);
}

/* @0x60063680: committed AU count, not bytes or the indexAllocated flag.
 * Same local-status and lifetime restrictions as GetFreeSize.
 */
int32_t CommonBuffer_GetAuCnt(const struct debug_adec_buf *buf)
{
    int32_t count = (int32_t)buf->status.write_index - buf->status.read_index;
    return count < 0 ? (int32_t)((uint32_t)count + buf->aui_count) : count;
}

/* Not reconstructed as safe diagnostic calls:
 * BufDbg_Dump @60061850 takes ADEC_BUF_T *, not commonBuf/uid/argv. It logs
 * local status and dereferences commonBuf plus module links without locking;
 * logging has IPC/debug side effects and provides no coherent snapshot.
 * GetUsedSize @60063610 takes the same pointer; invalid occupancy logs, calls
 * BufDbg_Dump and dump_backtrace, then spins forever at 6006367d.
 * GetAuInfo @600640f0 takes (int index, int *offset_out, AU *out, ADEC_BUF_T *).
 * out must hold 32 bytes; offset_out may be NULL. Index 0 is oldest, -1 newest;
 * out-of-range returns -2 without writing outputs. It invokes adap->invalidate
 * (buf+0x34, slot 0) as (entry, 40, buf) on each visited AU entry, then writes
 * outputs. Entries start at commonBuf+0x10 with a 40-byte stride.
 * NULL out/corrupt AU size reach logging/backtrace/infinite loops. Callback
 * effects and shared-state freshness must be established separately.
 */

/* @0x600705c8: one directed edge per output peer, including fan-out. Both
 * endpoint names/uids come from each port's module, not the module being walked.
 */
int dot_connection(struct debug_module *module)
{
    struct debug_link *link, *next;

    aud_mutex_lock(module->outputs.mutex);
    for (link = module->outputs.nodes.next, next = link->next;
         link != &module->outputs.nodes; link = next, next = next->next) {
        struct debug_port *out = debug_link_port(link);
        struct debug_link *peer, *peer_next;
        aud_mutex_lock(out->down_peers.mutex);
        for (peer = out->down_peers.nodes.next, peer_next = peer->next;
             peer != &out->down_peers.nodes;
             peer = peer_next, peer_next = peer_next->next) {
            struct debug_port *in = debug_link_port(peer);
            logm_message(" %s_%08x_%p_out%x -> %s_%08x_%p_in%x [style=\"dashed\"]\n",
                         out->module->impl->name, out->module->uid, (void *)out,
                         (unsigned)out->number, in->module->impl->name,
                         in->module->uid, (void *)in, (unsigned)in->number);
        }
        aud_mutex_unlock(out->down_peers.mutex);
    }
    aud_mutex_unlock(module->outputs.mutex);
    return 0;
}
