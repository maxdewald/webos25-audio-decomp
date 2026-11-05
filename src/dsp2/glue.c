// SPDX-License-Identifier: GPL-2.0-only
/*
 * dsp2 -> dsp1 shared-memory transport after mixer bypass.
 * Reconstructed against audio_a0_dsp2.bin sha256
 * c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d;
 * main sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd.
 * GPL references: lg-opensource/kdriver/core/aud/common/module/{module,module_glue}.c,
 * common/buffer/adec_inter_buf_writer.c and module_list.c. Evidence listings:
 * build/scratch/delay2/{glue,portsync2,procbuf,starter}.xo. Branch conditions
 * checked with tools/xtobjdump.sh; only the relevant data-path functions below.
 * Structures here are semantic views; comments give shipped 32-bit offsets.
 *
 * Wiring: kernel aud_inst_connect @ffffffc010c95540 -> aud_modlist_connect_inst
 * @ffffffc010c94d10 (processor-byte comparison +0xb8..+0xc4); a different-processor
 * connection creates source gluepw + destination gluepr, allocates a shared
 * ring (initialize_shbuf @ffffffc010c91a90), sends GLUE_CMD_SET_SHARED_BUFFER,
 * connects gluepr.out0 to the destination and source.out to gluepw.in0.
 * Same-processor links need no shared-memory hop. This is not selected by
 * es/channels/bits. Kernel source connect_between_processor is inlined in the
 * shipped connect path; don't treat its source name as a callable kernel symbol.
 *
 * Writer: set_shbuf_pw @60076f4c installs an InterBufWriter in gluepw.out0
 * legacy_buf (+0x13c), under check_free_mutex (+0x10c). Glue_Chain itself
 * just forwards data (or packs preceding metadata); aud_port_sync_locked
 * @600741d8 actually calls insert_au via the writer vtable +0xb0.
 * No FIRST/es/channels/bits gate for ES. Data type 1 maps to AU dataType=1.
 * Metadata packing is type4 (MIXED), not a conversion of raw ES into PCM.
 * FIRST and other events serialize as AU dataType=4 (EVENT), not aud_data
 * type4. Do not confuse the two enums. FIRST zero padding is ONLY when the
 * event's with-zero-buffer is nonzero (default 0); processing-size defaults to
 * last_data_size. Neither is activated merely by es=1 or channels=8.
 *
 * Full ring: 60074580 checks free bytes; 600745ee aud_cond_wait until free >=
 * this object's WHOLE byte size or flushing!=0 (+0x128). No timeout/chunking.
 * Thus an object bigger than total usable ring capacity can wait forever;
 * max_access_size is not a chunk size applied by this loop. CommonBuffer_GetFreeSize
 * @600635cc also returns ZERO when (writeIndex+1)%aui_count==readIndex,
 * regardless of unused byte space: with aui_count=2 only one committed AU can
 * be outstanding. FIRST events occupy AU slots too. Missing downstream
 * consumption/status updates also stalls it. Kernel defaults in the saved idle
 * snapshot: shared_size=262144, max_access_size=32768, aui_count=2; inspect the
 * actual live allocation/object size before concluding it is oversized.
 * The writer's CreateNewAu/WriteData/FinishAu can return errors; this caller
 * ignores insert_au's return, then releases pending objects (possible loss).
 * Flushing skips remaining writes and also releases pending objects.
 *
 * Wake path: Glue_Cmd @60077065 evt_type=2 (BM_EVT_READ) ->
 * InterBufWriter_UpdateStatus @60065c44, then aud_module_check_legacy_output
 * @600739fc -> signal out0.check_free_cond. evt_type=3 (BM_EVT_WRITE) ->
 * InterBufReader_UpdateStatus @60064e80, aud_starter_trigger @600785e8.
 * A writer waits in its CALLER'S thread; whether this blocks DSP command queue 1
 * depends on that live caller. Do not infer scheduler deadlock just from a full
 * ring. The reader's starter has its own thread (aud_starter_new @60078500,
 * priority 10), not queue 1. Its synchronous downstream chain can block on
 * a full writer while the command queue remains able to deliver read updates.
 *
 * dsp1 matching reader: set_shbuf_pr @600ff84c creates its reader and starter;
 * process_buf @60100940 maps AU type1 to aud_data ES, copies bytes, pushes to
 * attached ports then syncs them. type4 is deserialized as an event. No ES,
 * 16-bit or 8-channel rejection was found in this reader. Reader can wait for
 * data, fail allocation/read, or block in downstream sync (queue/output).
 * dsp2's identical reader process_buf @60077f78, starter_thread @60078490.
 *
 * Useful observations: gain mute-input, mute-output, current-gain via integer GET_PROP; delay
 * packet-count/number-packet/au-length/delay; dsp1 queue queued-length;
 * output_arc queued-length + underflow-count. Glue's registered props are
 * cowork-uid, shbuf-offset, merge-meta-pcm, plus generic drop/block flags;
 * there is no exported glue queued-length/free-size property. To prove a stuck
 * writer requires its port legacy buffer free/used status and waiting thread,
 * not only output_arc's queued-length. Gain es remains private +0x13c.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct aud_data {
    uint32_t object[4];
    int type;                       /* aud_data +0x10 */
    int index;                      /* +0x14 */
    uint32_t ts;                    /* +0x18 */
    uint32_t reserved;
    uint64_t ts64;                  /* +0x20 */
    uint32_t reserved2[3];
    unsigned size;                  /* +0x34 */
    void *data;                     /* +0x38 */
};
struct aud_object { unsigned ref, type; }; /* +0x04 'data' or 'evnt' */
struct aud_event;
struct au_info {
    unsigned size;
    uint32_t timestamp;
    uint64_t timestamp64;
    int index;
    unsigned char reserved[8];
    int data_type;                  /* AU +0x1c, differs from aud_data enum */
    int acmod;
};
struct adec_buf {
    int (*get_free_size)(struct adec_buf *);             /* vtable +0x84 */
    int (*insert_au)(void *, unsigned, struct au_info *, struct adec_buf *); /* +0xb0 */
};
struct aud_port {
    int number;                     /* +0x00 */
    void *module;                   /* +0x04 */
    unsigned flag;                  /* +0x08 */
    void *pending_root;             /* abstract down_peer_root */
    struct aud_object **pending;    /* +0xf0 */
    unsigned pending_num;           /* +0xf8 */
    int check_free_cond;            /* +0xfc */
    int check_free_mutex;           /* +0x10c */
    int flushing;                   /* +0x128 */
    struct adec_buf *legacy_buf;    /* +0x13c */
    struct aud_event *first;        /* +0x140 */
};
struct glue {
    const char *name;
    void *shared_buf;               /* +0xf8 */
    unsigned cowork_uid;            /* +0xfc */
    unsigned last_data_size;        /* +0x114 */
    bool merge_meta_pcm;            /* +0x118 */
    int meta_count;                 /* +0x11c */
    unsigned char meta_adapter[0x20]; /* starts +0x120, semantic view */
    unsigned aligned_meta_size;     /* +0x13c */
};

extern void aud_object_put(void *);
extern void *aud_data_new(unsigned);
extern int aud_module_push_data(void *, int, void *);
extern int aud_data_adapter_push(void *, void *);
extern struct aud_data *aud_data_adapter_dequeue(void *);
extern void aud_data_adapter_flush(void *, unsigned);
static unsigned adapter_bytes(const void *adapter)
{
    const unsigned char *base = adapter;
    return *(const uint32_t *)(base + 0x10) - *(const uint32_t *)(base + 0x14);
}
extern void *memcpy(void *, const void *, size_t);
extern void aud_mutex_lock(void *), aud_mutex_unlock(void *);
extern void aud_cond_wait(void *, void *);
extern int aud_event_value_get_int(struct aud_event *, const char *, int);
extern void *aud_event_serialize(struct aud_event *, unsigned *);
extern void aud_free(void *);

/* Inlined get_merged @60076a03..60076b03. Each record is {type,size,payload},
 * payload padded to 8 bytes. Allocation failure flushes queued metadata but
 * the original current data still reaches output. */
static struct aud_data *glue_merge(struct glue *g, struct aud_data *data)
{
    unsigned current = (data->size + 7u) & ~7u;
    unsigned total = g->aligned_meta_size + 8u * (unsigned)g->meta_count + current + 8;
    struct aud_data *out = aud_data_new(total);
    if (out) {
        unsigned char *dst = out->data;
        out->type = 4;
        out->index = data->index;
        out->ts = data->ts;
        out->ts64 = data->ts64;
        for (int i = 0; i <= g->meta_count; ++i) {
            struct aud_data *part = i == g->meta_count ? data :
                                   aud_data_adapter_dequeue(g->meta_adapter);
            uint32_t header[2] = { (unsigned)part->type, part->size };
            memcpy(dst, header, sizeof(header));
            memcpy(dst + 8, part->data, part->size);
            dst += 8 + ((part->size + 7u) & ~7u);
            aud_object_put(part);
        }
        data = out;
    } else {
        aud_data_adapter_flush(g->meta_adapter, adapter_bytes(g->meta_adapter));
    }
    g->meta_count = 0;
    g->aligned_meta_size = 0;
    return data;
}

/* @0x600769d0: symbol table spells it Glue_Chai. */
int Glue_Chain(struct aud_port *port, struct aud_data *data)
{
    struct glue *g = port->module;
    if (g->merge_meta_pcm) {
        if (data->type == 2 || data->type == 3) {
            aud_data_adapter_push(g->meta_adapter, data);
            ++g->meta_count;
            g->aligned_meta_size += (data->size + 7u) & ~7u;
            return 0;
        }
        if (g->meta_count > 0)
            data = glue_merge(g, data);
    }
    g->last_data_size = data->size;
    aud_module_push_data(g, 0, data);
    return 0;
}

/* Semantic view of the inlined legacy-writer half of aud_port_sync_locked
 * @600743be..60074622. Uncalled reference helper, not a standalone firmware
 * function. Down-peer forwarding happens before this; object
 * release happens after it. Kept untagged because it is not a separate function. */
void reconstructed_port_write_shared(struct aud_port *port)
{
    aud_mutex_lock(&port->check_free_mutex);
    if (port->legacy_buf) {
        for (unsigned i = 0; !port->flushing && i < port->pending_num; ++i) {
            struct aud_object *obj = port->pending[i];
            struct au_info info = {0};
            unsigned size = 0;
            void *bytes = NULL;
            bool event = obj->type == 0x65766e74u;
            if (obj->type == 0x64617461u) {
                struct aud_data *data = (void *)obj;
                /* 60074485..6007466a: PCM=0, ES=1, META=2, EXT=3, MIXED=5. */
                info.data_type = data->type == 4 ? 5 : data->type;
                if (data->type < 0 || data->type > 4)
                    info.data_type = 0;
                if (!data->type && port->first)
                    info.acmod = aud_event_value_get_int(port->first, "acmod", 0);
                info.index = data->index;
                info.timestamp = data->ts;
                info.timestamp64 = data->ts64;
                bytes = data->data;
                size = data->size;
            } else if (event) {
                info.data_type = 4;
                bytes = aud_event_serialize((void *)obj, &size);
            }
            if (!size)
                break;
            if (!port->flushing) {
                int free_size = port->legacy_buf->get_free_size(port->legacy_buf);
                while (free_size < (int)size) {
                    aud_cond_wait(&port->check_free_cond, &port->check_free_mutex);
                    if (port->flushing)
                        break;
                    free_size = port->legacy_buf->get_free_size(port->legacy_buf);
                }
            }
            if (!port->flushing)
                port->legacy_buf->insert_au(bytes, size, &info, port->legacy_buf);
            if (event)
                aud_free(bytes);
        }
    }
    aud_mutex_unlock(&port->check_free_mutex);
}

/* @0x60066398: critical sections inlined from the DSP buffer lock macros;
 * irq save/restore details omitted in this semantic reconstruction. */
int InterBufWriter_InsertAu(void *src, int length, struct au_info *info, void *buffer)
{
    extern int CommonBuffer_CreateNewAu(struct au_info *, void *);
    extern int CommonBuffer_WriteData(void *, int, void *);
    extern int CommonBuffer_FinishAu(void *);
    extern void ashbuf_notify_write(void *imc, void *buffer, unsigned cowork_uid);
    unsigned char *base = buffer;
    int ret = CommonBuffer_CreateNewAu(info, buffer);
    if (ret)
        return ret;
    ret = CommonBuffer_WriteData(src, length, buffer);
    if (ret) {
        *(uint32_t *)base = 0;
        return ret;
    }
    ret = CommonBuffer_FinishAu(buffer);
    void *imc = (void *)(uintptr_t)*(uint32_t *)(base + 0xf8);
    unsigned cowork = *(uint32_t *)(base + 0xfc);
    ashbuf_notify_write(imc, buffer, cowork);
    return ret;
}
