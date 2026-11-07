/*
 * DSP1 shared-buffer writer used by glue's legacy output port.
 * Semantic host views, not firmware ABI declarations. Diagnostics are omitted.
 * Evidence: build/decomp/audio_a0_dsp1.main_60000000.bin.c and
 * build/scratch/arc-native/dsp1/ writer listings from tools/xtobjdump.sh.
 * Public buffer sources under lg-opensource/kdriver are reference only.
 */
#include <stdint.h>
#include <string.h>

struct output_buffer_status {
    int32_t index_allocated;
    int32_t read_offset, write_offset;
    int16_t read_index, write_index;
    int32_t written, passed, repeat_count;
};

struct output_au_info {
    uint32_t size, timestamp;
    uint64_t timestamp64;
    uint32_t gstc;
    int16_t index;
    uint8_t flags, acmod;
    uint32_t first_module_gstc;
    int data_type;
};

struct output_common_buffer {
    int32_t max_size, access_size, max_index;
};

struct processor_adapter;

struct interbuf_writer {
    struct output_buffer_status status;
    struct output_common_buffer *common;
    unsigned max_access_size, max_size, aui_count;
    void *data;
    struct processor_adapter *adapter;
    unsigned offset;
    int (*init)(struct interbuf_writer *);
    int (*get_free_size)(struct interbuf_writer *);
    int (*get_pass_size)(struct interbuf_writer *);
    int (*get_used_size)(struct interbuf_writer *);
    int (*get_au_cnt)(struct interbuf_writer *);
    int (*get_max_size)(struct interbuf_writer *);
    int (*get_max_au)(struct interbuf_writer *);
    int (*get_max_au_length)(struct interbuf_writer *);
    int (*create_new_au)(struct output_au_info *, struct interbuf_writer *);
    void *(*request_mem)(struct output_au_info *, unsigned, struct interbuf_writer *);
    int (*write_data)(unsigned char *, int, struct interbuf_writer *);
    int (*finish_au)(struct interbuf_writer *);
    int (*insert_au)(unsigned char *, int, struct output_au_info *, struct interbuf_writer *);
    int (*update_au)(struct output_au_info *, struct interbuf_writer *);
    int (*get_au_info)(int, int *, struct output_au_info *, struct interbuf_writer *);
    int (*get_current_info)(int *, struct output_au_info *, struct interbuf_writer *);
    int (*read_data)(unsigned char **, int *, int *, struct interbuf_writer *);
    int (*flush_data)(int, struct interbuf_writer *);
    int (*read_au)(unsigned char **, int *, int *, struct interbuf_writer *);
    int (*set_repeat)(int, struct interbuf_writer *);
    int (*request_data)(struct interbuf_writer *);
    int (*ref)(struct interbuf_writer *);
    int (*unref)(struct interbuf_writer *);
    int buf_type, ref_count;
    uint32_t ref_lock_ps;
    uint32_t lock_ps;
    void *aimc;
    unsigned cowork_module;
};

extern void BufDbg_AssertBreak(void);
extern uint32_t irq_save(void);
extern void irq_restore(uint32_t);
extern unsigned CommonBuffer_GetStructSize(unsigned);
extern int CommonBuffer_Init(struct interbuf_writer *);
extern int CommonBuffer_Ref(struct interbuf_writer *);
extern int CommonBuffer_Unref(struct interbuf_writer *);
extern int CommonBuffer_GetFreeSize(struct interbuf_writer *);
extern int CommonBuffer_GetUsedSize(struct interbuf_writer *);
extern int CommonBuffer_GetAuCnt(struct interbuf_writer *);
extern int CommonBuffer_GetMaxSize(struct interbuf_writer *);
extern int CommonBuffer_GetMaxAu(struct interbuf_writer *);
extern int CommonBuffer_GetMaxAuLength(struct interbuf_writer *);
extern int CommonBuffer_CreateNewAu(struct output_au_info *, struct interbuf_writer *);
extern int CommonBuffer_WriteData(unsigned char *, int, struct interbuf_writer *);
extern int CommonBuffer_WriteDataUser(unsigned char *, int, struct interbuf_writer *);
extern int CommonBuffer_FinishAu(struct interbuf_writer *);
extern int CommonBuffer_UpdateAu(struct output_au_info *, struct interbuf_writer *);
extern void ashbuf_notify_write(void *, struct interbuf_writer *, unsigned);

static void writer_require(const void *ptr)
{
    if (!ptr) {
        BufDbg_AssertBreak();
        for (;;) {}
    }
}

/* @0x600ee5b4: accept only reader-owned fields from a remote status snapshot. */
int InterBufWriter_UpdateStatus(struct interbuf_writer *buf,
                               const struct output_buffer_status *status)
{
    writer_require(buf);
    buf->status.read_index = status->read_index;
    buf->status.read_offset = status->read_offset;
    return 0;
}

/* @0x600eeb10: allocation is protected; it does not publish a write notification. */
int InterBufWriter_CreateNewAu(struct output_au_info *info, struct interbuf_writer *buf)
{
    writer_require(buf);
    buf->lock_ps = irq_save();
    int result = CommonBuffer_CreateNewAu(info, buf);
    irq_restore(buf->lock_ps);
    return result;
}

/* @0x600eeba0: functions.tsv aliases this as UpdateStatus; logged __func__ is
 * InterBufWriter_WriteDataInternal. Payload copying does not disable interrupts. */
int InterBufWriter_WriteData(unsigned char *src, int length, struct interbuf_writer *buf)
{
    writer_require(buf);
    return CommonBuffer_WriteData(src, length, buf);
}

/* @0x600eec04 */
int InterBufWriter_WriteDataUser(unsigned char *src, int length, struct interbuf_writer *buf)
{
    writer_require(buf);
    return CommonBuffer_WriteDataUser(src, length, buf);
}

/* @0x600eec68: notify after restoring PS, including when finish returns an error. */
int InterBufWriter_FinishAu(struct interbuf_writer *buf)
{
    writer_require(buf);
    buf->lock_ps = irq_save();
    int result = CommonBuffer_FinishAu(buf);
    irq_restore(buf->lock_ps);
    ashbuf_notify_write(buf->aimc, buf, buf->cowork_module);
    return result;
}

static int writer_insert(unsigned char *src, int length, struct output_au_info *info,
                         struct interbuf_writer *buf, int from_user)
{
    int result = InterBufWriter_CreateNewAu(info, buf);
    if (result)
        return result;
    result = from_user ? CommonBuffer_WriteDataUser(src, length, buf)
                       : CommonBuffer_WriteData(src, length, buf);
    if (result) {
        buf->status.index_allocated = 0;
        return result;
    }
    return InterBufWriter_FinishAu(buf);
}

/* @0x600eed08: copy failure abandons the AU without finish or notification. */
int InterBufWriter_InsertAu(unsigned char *src, int length, struct output_au_info *info,
                           struct interbuf_writer *buf)
{
    return writer_insert(src, length, info, buf, 0);
}

/* @0x600eee2c */
int InterBufWriter_InsertAuUser(unsigned char *src, int length, struct output_au_info *info,
                               struct interbuf_writer *buf)
{
    return writer_insert(src, length, info, buf, 1);
}

/* @0x600eef4c: metadata update is protected, without notifying the peer. */
int InterBufWriter_UpdateAu(struct output_au_info *info, struct interbuf_writer *buf)
{
    writer_require(buf);
    buf->lock_ps = irq_save();
    int result = CommonBuffer_UpdateAu(info, buf);
    irq_restore(buf->lock_ps);
    return result;
}

/* @0x600ee658 */
int InterBufWriter_Init(struct interbuf_writer *buf)
{
    writer_require(buf);
    buf->lock_ps = irq_save();
    int result = CommonBuffer_Init(buf);
    irq_restore(buf->lock_ps);
    return result;
}

/* @0x600ee6e8 */
int InterBufWriter_Ref(struct interbuf_writer *buf)
{
    writer_require(buf);
    return CommonBuffer_Ref(buf);
}

/* @0x600ee748 */
int InterBufWriter_Unref(struct interbuf_writer *buf)
{
    writer_require(buf);
    return CommonBuffer_Unref(buf);
}

/* @0x600ee7a8 */
int InterBufWriter_GetFreeSize(struct interbuf_writer *buf)
{
    writer_require(buf);
    buf->lock_ps = irq_save();
    int result = CommonBuffer_GetFreeSize(buf);
    irq_restore(buf->lock_ps);
    return result;
}

/* @0x600ee838: a writer reports no passed bytes. */
int InterBufWriter_GetPassSize(struct interbuf_writer *buf)
{
    return 0;
}

/* @0x600ee840 */
int InterBufWriter_GetUsedSize(struct interbuf_writer *buf)
{
    writer_require(buf);
    buf->lock_ps = irq_save();
    int result = CommonBuffer_GetUsedSize(buf);
    irq_restore(buf->lock_ps);
    return result;
}

/* @0x600ee8d0 */
int InterBufWriter_GetAuCnt(struct interbuf_writer *buf)
{
    writer_require(buf);
    buf->lock_ps = irq_save();
    int result = CommonBuffer_GetAuCnt(buf);
    irq_restore(buf->lock_ps);
    return result;
}

/* @0x600ee960 */
int InterBufWriter_GetMaxSize(struct interbuf_writer *buf)
{
    writer_require(buf);
    buf->lock_ps = irq_save();
    int result = CommonBuffer_GetMaxSize(buf);
    irq_restore(buf->lock_ps);
    return result;
}

/* @0x600ee9f0 */
int InterBufWriter_GetMaxAu(struct interbuf_writer *buf)
{
    writer_require(buf);
    buf->lock_ps = irq_save();
    int result = CommonBuffer_GetMaxAu(buf);
    irq_restore(buf->lock_ps);
    return result;
}

/* @0x600eea80 */
int InterBufWriter_GetMaxAuLength(struct interbuf_writer *buf)
{
    writer_require(buf);
    buf->lock_ps = irq_save();
    int result = CommonBuffer_GetMaxAuLength(buf);
    irq_restore(buf->lock_ps);
    return result;
}

/* @0x600eefdc: this producer cannot read AU metadata. */
int InterBufWriter_GetAuInfo(int index, int *offset, struct output_au_info *info,
                            struct interbuf_writer *buf)
{
    return -5;
}

/* @0x600eefe4 */
int InterBufWriter_GetCurrentInfo(int *offset, struct output_au_info *info,
                                 struct interbuf_writer *buf)
{
    return -5;
}

/* @0x600eefec */
int InterBufWriter_ReadData(unsigned char **dst, int *size, int *au_count,
                           struct interbuf_writer *buf)
{
    return -5;
}

/* @0x600eeff4: flushing belongs to the reader, not the producer. */
int InterBufWriter_FlushData(int size, struct interbuf_writer *buf)
{
    return -5;
}

/* @0x600eeffc */
int InterBufWriter_ReadAu(unsigned char **dst, int *size, int *au_count,
                         struct interbuf_writer *buf)
{
    return -5;
}

/* @0x600ef004 */
int InterBufWriter_SetRepeat(int count, struct interbuf_writer *buf)
{
    return -1;
}

/* @0x600ef00c */
int InterBufWriter_RequestData(struct interbuf_writer *buf)
{
    return -5;
}

/* @0x600ee170: shipped 32-bit allocation size, not sizeof the semantic view. */
int InterBufWriter_GetStructSize(void)
{
    return 0x100;
}

/* @0x600ee178: initialize the producer table, selecting ordinary or user copying.
 * Init's return is deliberately ignored. No capacity/underflow guard is shipped. */
int _InterBufWriter_Create(struct processor_adapter *adapter,
                          struct output_common_buffer *common, unsigned total_size,
                          unsigned access_size, unsigned aui_count, void *data,
                          unsigned cowork, void *aimc, struct interbuf_writer *buf,
                          int from_user)
{
    writer_require(adapter);
    writer_require(common);
    writer_require(buf);
    memset(buf, 0, sizeof(*buf));
    buf->create_new_au = InterBufWriter_CreateNewAu;
    buf->flush_data = InterBufWriter_FlushData;
    buf->get_au_cnt = InterBufWriter_GetAuCnt;
    buf->get_au_info = InterBufWriter_GetAuInfo;
    buf->get_current_info = InterBufWriter_GetCurrentInfo;
    buf->get_free_size = InterBufWriter_GetFreeSize;
    buf->get_pass_size = InterBufWriter_GetPassSize;
    buf->get_max_au = InterBufWriter_GetMaxAu;
    buf->get_max_au_length = InterBufWriter_GetMaxAuLength;
    buf->get_max_size = InterBufWriter_GetMaxSize;
    buf->get_used_size = InterBufWriter_GetUsedSize;
    buf->init = InterBufWriter_Init;
    buf->read_au = InterBufWriter_ReadAu;
    buf->read_data = InterBufWriter_ReadData;
    buf->update_au = InterBufWriter_UpdateAu;
    buf->write_data = from_user ? InterBufWriter_WriteDataUser : InterBufWriter_WriteData;
    buf->insert_au = from_user ? InterBufWriter_InsertAuUser : InterBufWriter_InsertAu;
    buf->finish_au = InterBufWriter_FinishAu;
    buf->set_repeat = InterBufWriter_SetRepeat;
    buf->request_data = InterBufWriter_RequestData;
    buf->ref = InterBufWriter_Ref;
    buf->unref = InterBufWriter_Unref;
    buf->buf_type = 1;
    buf->ref_lock_ps = buf->lock_ps = UINT32_MAX;
    buf->ref_count = 1;
    buf->max_size = total_size - CommonBuffer_GetStructSize(aui_count) - access_size;
    buf->max_access_size = access_size;
    buf->aui_count = aui_count;
    buf->common = common;
    buf->data = data;
    buf->adapter = adapter;
    buf->aimc = aimc;
    buf->cowork_module = cowork;
    InterBufWriter_Init(buf);
    return 0;
}

/* @0x600ee380: shipped inline ordinary-mode specialization of the constructor. */
int InterBufWriter_Create(struct processor_adapter *adapter,
                         struct output_common_buffer *common, unsigned total_size,
                         unsigned access_size, unsigned aui_count, void *data,
                         unsigned cowork, void *aimc, struct interbuf_writer *buf)
{
    return _InterBufWriter_Create(adapter, common, total_size, access_size, aui_count,
                                  data, cowork, aimc, buf, 0);
}

/* @0x600ee56c: attach to the existing ring; repair capacity after Init, not before. */
int InterBufWriter_CreateFromCommonBuffer(struct processor_adapter *adapter,
                                        struct output_common_buffer *common, void *data,
                                        unsigned cowork, void *aimc,
                                        struct interbuf_writer *buf)
{
    int result = _InterBufWriter_Create(adapter, common, 0, (unsigned)common->access_size,
                                       (unsigned)common->max_index, data, cowork, aimc, buf, 0);
    buf->max_size = (unsigned)common->max_size;
    return result;
}

/* @0x600ee590 */
int InterBufWriter_CreateFromCommonBufferUser(struct processor_adapter *adapter,
                                            struct output_common_buffer *common, void *data,
                                            unsigned cowork, void *aimc,
                                            struct interbuf_writer *buf)
{
    int result = _InterBufWriter_Create(adapter, common, 0, (unsigned)common->access_size,
                                       (unsigned)common->max_index, data, cowork, aimc, buf, 1);
    buf->max_size = (unsigned)common->max_size;
    return result;
}