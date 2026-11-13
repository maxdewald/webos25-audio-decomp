// SPDX-License-Identifier: GPL-2.0-only
/*
 * /proc/aaudio/instances/<uid>/prop: the read side is DSP log capture, NOT
 * aimc_get_property(). Reconstructed against Image-5.4.268-329.decompressed
 * sha256 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * GPL reference: lg-opensource/kdriver/core/aud/{module_list,proc,kaimc}.c.
 * Shipped disassembly: build/scratch/kprop/{lc,getprop_k,release}.lst.
 * DSP side: src/dsp2/debug.c. No runtime diagnosis is implied by this listing.
 *
 * open: debug_open_arg @ffffffc010c97380 selects inst_prop_read_fops when
 * FMODE_READ is set. O_RDWR therefore selects READ, not the write handler.
 * inst_prop_read_open @ffffffc010c90010 formats inst->uid (+0x44), obtains
 * inst->mod_impl (+0x70)->processor (+0x20)->proc_id (+0x44), then calls
 * aproc_lc_trigger(proc_id, "audreg_print_property", uid_string, NULL).
 * The /instances entry links to the processor's module instance directory;
 * reading it does not use a separate implementation.
 *
 * Blocking sites:
 *  1. open holds the GLOBAL aud_log_lock until close/release. A concurrent
 *     capture waits in mutex_lock_interruptible, before issuing any command.
 *  2. read waits without timeout until logs arrive or the exact end marker
 *     is seen. Even after copying bytes, it waits again to fill the requested
 *     count. Missing start/end => can wait forever. O_NONBLOCK is not tested;
 *     there is no poll method. A signal breaks the loop, returning bytes copied
 *     (possibly zero), not the wait's negative error.
 *  3. send_call_param ignores aimc_send_command's result and returns 0.
 *     Failed sends therefore look like a successful open with no markers.
 * Capture accepts only the selected processor and exact magic strings.
 * send_call_param @ffffffc010c97780 routes dspnum 1/2/3 to DBG ids 7/8/9;
 * all other values to DBG id 6 (dsp0). This cannot dump userspace appsrc/appsink
 * processors (proc_id >= 8): commands go to dsp0 but capture expects another
 * processor, so no matching marker can arrive.
 *
 * DSP debug logs are lossy: AUD_IPCM_WriteDebug @6006ee18 skips a message on
 * overflow (@6006ef30/6006ef53), sets the dropped flag, and does not retry.
 * handle_message (kaimc.c) concatenates CONTINUE fragments but does not recover
 * missing markers or treat DROPPED as capture failure. This is a mechanism
 * explaining hangs, not proof that overflow caused a particular playback hang.
 * A busy/blocked DSP command scheduler or a custom property getter is another
 * possible cause. No playback stack trace/log transport snapshot was supplied.
 *
 * Module-side alternatives, verified in the shipped kernel:
 *  aimc_get_property @ffffffc010c8ee24 (uid, char *name, int *value)
 *    sends 0x10170020; sends min(strlen(name)+1,32) bytes (last byte NUL).
 *    send_command_sync @ffffffc010c8e430 waits interruptibly for the RESPONSE,
 *    not log text: audio_imc_sync_timeout seconds (default 5), suspend 100 ms.
 *    Return is transport status; *value is DSP handler result/property value.
 *    Initialize *value to a sentinel: stop_imc/dead-DSP paths can return 0
 *    without writing it. Do not call from atomic context or the IMC response
 *    worker (which would wait for itself). It can still sleep on send/locks.
 *  aimc_send_command_rsp @ffffffc010c8e310:
 *    int (struct aimc *, unsigned cmd, unsigned source, unsigned uid,
 *         void *payload, unsigned bytes,
 *         void (*cb)(int result, void *payload, int bytes, void *cookie),
 *         void *cookie);
 *    Same GET_PROP name payload: callback's result is the property value.
 *    No synchronous DSP-response wait; NOT an atomic/non-sleeping API.
 *    It still needs a functioning DSP command scheduler/ARM response worker.
 *    Keep callback code and cookie alive until the request retires; this wrapper
 *    exposes no response-id/cancellation handle. Never use a stack cookie for
 *    a request that outlives the call, nor unload a module with callbacks pending.
 *    These non-exported helpers require verified symbol resolution in a module;
 *    aimc_send_command_sync is exported but discards the result value.
 *
 * GET_PROP limitations (aud_module_get_prop @60075b74 on dsp2):
 *  - direct bool/int fields and geti callbacks work; result -1 also means
 *    unknown property or unsupported string getter (no distinct status).
 *  - gain mute-input/mute-output/current-gain: direct fields; fader/queue
 *    queued-length: geti; dsp1 output_arc queued-length/underflow-count: direct.
 *  - delay queued-length: ONLY gets callback, type 0, no geti => -1.
 *    Read packet-count, au-length and delay instead; byte count requires a
 *    different diagnostic/firmware helper, not this integer GET_PROP.
 *  - gain es (+0x13c) and delay es (+0x14e) are FIRST-event state, not props.
 *    GET_PROP("es") cannot read them. lipsync exposes dataType, not gain's es.
 * Raw DSP RAM from ARM is not established as a safe substitute here: fwload
 * exposes a physical RAM allocation, but cache visibility, live object lifetime
 * and pointer validation must also be established before dereferencing it.
 * Shipped acquisition boundary (build/scratch/diag-acquire/fwload.c):
 *  fwload_get_dsp_memory_info @ffffffc01107b0bc delegates to aud_get_img_info
 *  @ffffffc010c8b350: "aud1"/"aud2", "ram" return allocation PA/size only.
 *  /proc/mcu/aud{1,2}/area/ram, when registered, uses area_open
 *  @ffffffc010c87bc0 (hma_map), area_read @ffffffc010c89cc0 (bounded
 *  copy_to_user), and release (unmap); runtime presence was not checked.
 *  Mode 0440, seek/read, no mmap method; image/rom reads the backup, not RAM.
 *  Open's mapped-pointer busy check is unlocked and does not check map failure;
 *  read has no DSP lock, halt, cache maintenance, or snapshot protocol.
 *  hma_map @ffffffc010d3c094 selects vmap_phys/__ioremap with AttrIndx 3;
 *  ARM mapping attributes alone do not establish DSP dirty-cache visibility.
 *  aud_reset @ffffffc010c8b510 writes ROM PA to OFFSET5 and RAM allocation PA
 *  to OFFSET6. DSP2 boot @4ff800c9 loads 0x2222442f, installs region attributes
 *  with WITLB @4ff800f9 / WDTLB @4ff8010f, then enters main @600611e0.
 *  DSP1 has the same attribute setup (0x60000000..0x7fffffff nibble 4).
 *  Main sets SP=0x60d80000 and clears 0x60123c00..0x60170b68 @6006120c;
 *  0x601... includes writable storage. Startup does not establish the external
 *  OFFSET5/6 window selection/addition rule, so no VA-to-proc offset formula
 *  or 0x601... heap/TCB backing is proven. Runtime allocation PA is dynamic.
 *  Reset cache initialization/writeback is not maintenance during area_read;
 *  neither dirty-data visibility nor an atomic queue snapshot is established.
 *  initialize_shbuf @ffffffc010c91a90 separately allocates audio_buffer RAM;
 *  its 32-bit offset = PA - aud_base_address() + 0x10000000, not a DSP RAM
 *  offset. No ring lifetime pinning or coherent multi-field read is supplied.
 *  _dump_dsp @ffffffc010cc1540 sends IRQ cmd 8; secure dump also programs IPC,
 *  requests a DSP copy and temporarily exposes dump RAM. Not passive playback
 *  acquisition. Whether halting plus cache handling suffices remains unknown;
 *  no halt is authorized. Non-mutating reads are not established safe snapshots.
 */
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/wait.h>

#include "imc/adec_imc_cmd.h"
#include "kaimc.h"
#include "proc.h"

static DEFINE_MUTEX(aud_log_lock);
static DEFINE_MUTEX(lc_received_lock);
static DECLARE_WAIT_QUEUE_HEAD(lc_wq);
static LIST_HEAD(lc_received);
static int lc_proc_num, lc_state, lc_magic_num;
static char lc_magic[16];
enum { lc_state_wait, lc_state_working, lc_state_done };
struct log_capture_line {
    struct list_head list;
    int processed, size;
    char string[];
};
extern void aud_log_default(int, unsigned int, const char *);
extern void (*aud_log_print)(int, unsigned int, const char *);
enum call_param_type
{
    param_arg,
    param_funcv,
};
extern int send_call_param(int dspnum, const char *str, enum call_param_type type);

/* @0xffffffc010c90010: only reads confirmed fields, not a guessed full struct. */
static int inst_prop_read_open(struct inode *inode, struct file *file)
{
    void **arg = PDE_DATA(inode);
    unsigned char *inst = arg[0];
    unsigned char *impl = *(void **)(inst + 0x70);
    unsigned char *processor = *(void **)(impl + 0x20);
    char uid[16];
    (void)file;
    snprintf(uid, sizeof(uid), "0x%08x", *(unsigned *)(inst + 0x44));
    return aproc_lc_trigger(*(int *)(processor + 0x44),
                            "audreg_print_property", uid, NULL);
}

/* @0xffffffc010c985d0 */
static void log_capture_log(int proc_num, unsigned int ts, const char *string)
{
    struct log_capture_line *line = NULL;
    bool drop_log = false;
    char marker[32];

    mutex_lock(&lc_received_lock);
    if (lc_proc_num == proc_num) {
        if (lc_state == lc_state_working) {
            snprintf(marker, sizeof(marker), "%s: end\n", lc_magic);
            if (!strcmp(marker, string)) {
                lc_state = lc_state_done;
                drop_log = true;
                wake_up(&lc_wq);
            }
        }
        if (lc_state == lc_state_working) {
            int size = strlen(string);
            line = kmalloc(sizeof(*line) + size, GFP_KERNEL);
            if (line) {
                line->processed = 0;
                line->size = size;
                memcpy(line->string, string, size);
                list_add_tail(&line->list, &lc_received);
                wake_up(&lc_wq);
            }
        }
        if (lc_state == lc_state_wait) {
            snprintf(marker, sizeof(marker), "%s: start\n", lc_magic);
            if (!strcmp(marker, string)) {
                lc_state = lc_state_working;
                drop_log = true;
            }
        }
    }
    if (!line && !drop_log)
        aud_log_default(proc_num, ts, string);
    mutex_unlock(&lc_received_lock);
}

/* @0xffffffc010c98df0 */
int aproc_lc_trigger(int proc_num, const char *func_name, ...)
{
    va_list args;
    char *arg;
    int ret = mutex_lock_interruptible(&aud_log_lock);
    if (ret < 0)
        return ret;
    lc_proc_num = proc_num;
    lc_state = lc_state_wait;
    snprintf(lc_magic, sizeof(lc_magic), "_MAGIC_%d_", ++lc_magic_num);
    aud_log_print = log_capture_log;
    send_call_param(proc_num - 2, lc_magic, param_arg);
    va_start(args, func_name);
    while ((arg = va_arg(args, char *)) != NULL)
        send_call_param(proc_num - 2, arg, param_arg);
    va_end(args);
    return send_call_param(proc_num - 2, func_name, param_funcv);
}

/* @0xffffffc010c97f10 */
ssize_t aproc_lc_read(struct file *file, char __user *data, size_t size, loff_t *pos)
{
    ssize_t processed = 0;
    (void)file;
    (void)pos;
    while (processed < size) {
        struct log_capture_line *line;
        int copy_size;
        int ret = wait_event_interruptible(lc_wq,
            (list_empty(&lc_received) && lc_state == lc_state_done) ||
            !list_empty(&lc_received));
        if (ret < 0)
            break;
        if (list_empty(&lc_received) && lc_state == lc_state_done)
            break;
        line = list_first_entry(&lc_received, struct log_capture_line, list);
        copy_size = min_t(size_t, size - processed, line->size - line->processed);
        if (copy_to_user(data + processed, line->string + line->processed, copy_size))
            return -EFAULT;
        processed += copy_size;
        line->processed += copy_size;
        if (line->processed >= line->size) {
            mutex_lock(&lc_received_lock);
            list_del(&line->list);
            mutex_unlock(&lc_received_lock);
            /* Shipped code does not kfree this consumed line. Preserved, not fixed. */
        }
    }
    return processed;
}

/* @0xffffffc010c97680 */
int aproc_lc_release(struct inode *inode, struct file *file)
{
    struct log_capture_line *line, *next;
    (void)inode;
    (void)file;
    aud_log_print = aud_log_default;
    mutex_lock(&lc_received_lock);
    lc_proc_num = 0;
    list_for_each_entry_safe(line, next, &lc_received, list) {
        list_del(&line->list);
        kfree(line);
    }
    mutex_unlock(&lc_received_lock);
    mutex_unlock(&aud_log_lock);
    return 0;
}

extern int send_command_sync(struct aimc *, unsigned cmd, unsigned source,
                             unsigned target, void *param, unsigned bytes,
                             int *result, bool copy_param);

/* @0xffffffc010c8ee24: integer result returned separately from transport status. */
int aimc_get_property(unsigned target, char *name, int *value)
{
    unsigned size = min_t(size_t, strlen(name) + 1, sizeof(ImcCmdGetProp));
    ImcCmdGetProp *param = kzalloc(sizeof(*param), GFP_KERNEL);
    int ret;
    if (!param)
        return -ENOMEM;
    strncpy(param->name, name, size);
    param->name[size - 1] = 0;
    ret = send_command_sync(NULL, ADEC_CMD_GET_PROP, 0, target, param, size, value, false);
    kfree(param);
    return ret;
}
