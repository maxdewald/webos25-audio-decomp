/*
 * Inter-processor buffer front-ends: InterBufReader (consumer side of a shared common
 * buffer) and InterBufWriter (producer side). Both wrap the CommonBuffer_* ring
 * (buffers.c) with a per-buffer lock and fill the ADEC_BUF_T method table; the writer
 * sends BM_EVT_WRITE after every published AU, the reader BM_EVT_READ after a flush.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 *
 * Evidence: Ghidra decomp/libaa.so.2.0.0.c @0x236a0..@0x250e0; method table stores in
 * InterBufReader_Create @0x2405c / _InterBufWriter_Create @0x24ccc; __func__ strings at
 * 0x35128..0x35548. The tiny "not supported" stubs (mvn r0,#4; bx lr) have no symbol of
 * their own; they are tagged with their addresses anyway.
 */
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "buffers.h"

LOGMM_MODULE_DEFINE(buf_reader);
LOGMM_MODULE_DEFINE(buf_writer);

#define NULL_BUF_CHECK(line) \
    BUF_ASSERT(buf != NULL, "buf != ((void *)0)", NULL, -ADEC_ERR_NULL, line, \
               "buf should not be null.\n")

#define LOCKED(expr) do { \
        pthread_mutex_lock(&buf->lock); \
        ret = (expr); \
        pthread_mutex_unlock(&buf->lock); \
    } while (0)

/* ========================================================================= */
/* reader                                                                    */
/* ========================================================================= */

/* @0x236a0: writer-only methods on a reader */
int InterBufReader_CreateNewAu(ADEC_AU_INFO_T *info, ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x236a8 */
int InterBufReader_WriteData(unsigned char *src, int len, ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x236b0 */
int InterBufReader_FinishAu(ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x236b8 */
int InterBufReader_InsertAu(unsigned char *src, int len, ADEC_AU_INFO_T *info, ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x236c0 */
int InterBufReader_SetRepeat(int cnt, ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x236c8 */
int InterBufReader_RequestData(ADEC_BUF_T *buf)
{
    NULL_BUF_CHECK(0x218);
    return 0;
}

/* @0x23744 */
int InterBufReader_Unref(ADEC_BUF_T *buf)
{
    NULL_BUF_CHECK(0x107);
    return CommonBuffer_Unref(buf);
}

/* @0x237cc */
int InterBufReader_Ref(ADEC_BUF_T *buf)
{
    NULL_BUF_CHECK(0x100);
    return CommonBuffer_Ref(buf);
}

/* @0x23854 */
int InterBufReader_ReadData(unsigned char **dst, int *size, int *au_in_data, ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x1cd);
    LOCKED(CommonBuffer_ReadData(dst, size, au_in_data, buf));
    return ret;
}

/* @0x238fc: drop everything that is in the buffer (read position := write position) */
int InterBufReader_Init(ADEC_BUF_T *buf)
{
    NULL_BUF_CHECK(0xeb);
    pthread_mutex_lock(&buf->lock);
    buf->status.readIndex = buf->status.writeIndex;
    buf->status.readOffset = buf->status.writeOffset;
    pthread_mutex_unlock(&buf->lock);
    return 0;
}

/* @0x23990 */
int InterBufReader_ReadAu(unsigned char **dst, int *size, int *au_cnt, ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x200);
    LOCKED(CommonBuffer_ReadAu(dst, size, au_cnt, buf));
    return ret;
}

/* @0x23a38 */
int InterBufReader_GetUsedSize(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x131);
    LOCKED(CommonBuffer_GetUsedSize(buf));
    return ret;
}

/* @0x23ad0 */
int InterBufReader_GetMaxSize(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x158);
    LOCKED(CommonBuffer_GetMaxSize(buf));
    return ret;
}

/* @0x23b68 */
int InterBufReader_GetMaxAuLength(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x178);
    LOCKED(CommonBuffer_GetMaxAuLength(buf));
    return ret;
}

/* @0x23c00 */
int InterBufReader_GetMaxAu(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x168);
    LOCKED(CommonBuffer_GetMaxAu(buf));
    return ret;
}

/* @0x23c98 */
int InterBufReader_GetPassSize(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x123);
    LOCKED(CommonBuffer_GetPassSize(buf));
    return ret;
}

/* @0x23d30 */
int InterBufReader_GetFreeSize(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x111);
    LOCKED(CommonBuffer_GetFreeSize(buf));
    return ret;
}

/* @0x23dc8 */
int InterBufReader_GetCurrentInfo(int *offset, ADEC_AU_INFO_T *info, ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x1b8);
    LOCKED(CommonBuffer_GetCurrentInfo(offset, info, buf));
    return ret;
}

/* @0x23e64 */
int InterBufReader_GetAuInfo(int index, int *offset, ADEC_AU_INFO_T *info, ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x1a3);
    LOCKED(CommonBuffer_GetAuInfo(index, offset, info, buf));
    return ret;
}

/* @0x23f0c */
int InterBufReader_GetAuCnt(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x144);
    LOCKED(CommonBuffer_GetAuCnt(buf));
    return ret;
}

/* @0x23fa4: consume and report the new read position to the remote writer. The notify
 * is sent even when the flush failed (e.g. after a corrupted-table reset). */
int InterBufReader_FlushData(int size, ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x1e3);
    LOCKED(CommonBuffer_FlushData(&size, buf));
    ashbuf_notify_read(buf->aimc, buf, buf->coworker);
    return ret;
}

/* @0x2404c */
int InterBufReader_UpdateAu(ADEC_AU_INFO_T *info, ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x24054 */
int InterBufReader_GetStructSize(void)
{
    return sizeof(ADEC_BUF_T);
}

/* @0x2405c: attach to a common buffer that the remote writer already initialised.
 * Starts at read position 0 (no CommonBuffer_Init: the writer owns the indices). */
int InterBufReader_Create(ADEC_PROCESSOR_ADAP_IF_T *adap, ADEC_COMMON_BUF_T *common_buf,
                          int buffer_size, int max_access_size, int index_count, void *data,
                          unsigned int coworker, struct aimc *imc, ADEC_BUF_T *buf)
{
    BUF_ASSERT(adap != NULL, "_adapIf != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x8c,
               "adap should not be null.\n");
    BUF_ASSERT(buf != NULL, "_struct != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x8d,
               "struct should not be null.\n");

    memset(buf, 0, sizeof(*buf));
    buf->create_new_au = InterBufReader_CreateNewAu;
    buf->flush_data = InterBufReader_FlushData;
    buf->get_au_cnt = InterBufReader_GetAuCnt;
    buf->get_au_info = InterBufReader_GetAuInfo;
    buf->get_current_info = InterBufReader_GetCurrentInfo;
    buf->get_free_size = InterBufReader_GetFreeSize;
    buf->get_pass_size = InterBufReader_GetPassSize;
    buf->get_max_au = InterBufReader_GetMaxAu;
    buf->get_max_au_length = InterBufReader_GetMaxAuLength;
    buf->get_max_size = InterBufReader_GetMaxSize;
    buf->get_used_size = InterBufReader_GetUsedSize;
    buf->init = InterBufReader_Init;
    buf->insert_au = InterBufReader_InsertAu;
    buf->read_au = InterBufReader_ReadAu;
    buf->read_data = InterBufReader_ReadData;
    buf->update_au = InterBufReader_UpdateAu;
    buf->write_data = InterBufReader_WriteData;
    buf->finish_au = InterBufReader_FinishAu;
    buf->set_repeat = InterBufReader_SetRepeat;
    buf->request_data = InterBufReader_RequestData;
    buf->ref = InterBufReader_Ref;
    buf->unref = InterBufReader_Unref;
    pthread_mutex_init(&buf->ref_count_mutex, NULL);
    pthread_mutex_init(&buf->lock, NULL);
    buf->ref_count = 1;

    buf->aui_count = index_count;
    buf->commonBuf = common_buf;
    buf->max_access_size = max_access_size;
    buf->max_size = buffer_size - max_access_size - CommonBuffer_GetStructSize(index_count);
    buf->boundary_buf = malloc(max_access_size);
    buf->status.readOffset = 0;
    buf->status.readIndex = 0;
    buf->aimc = imc;
    buf->data = data;
    buf->adap = adap;
    buf->coworker = coworker;
    return 0;
}

/* @0x242a8: geometry taken from the shared header (maxSize, accessSize, maxIndex) */
int InterBufReader_CreateFromCommonBuffer(ADEC_PROCESSOR_ADAP_IF_T *adap,
                                          ADEC_COMMON_BUF_T *common_buf, void *data,
                                          unsigned int coworker, struct aimc *imc,
                                          ADEC_BUF_T *buf)
{
    int ret = InterBufReader_Create(adap, common_buf, 0, common_buf->accessSize,
                                    common_buf->maxIndex, data, coworker, imc, buf);

    buf->max_size = common_buf->maxSize;
    return ret;
}

/* @0x242d0: remote writer published AUs (BM_EVT_WRITE) */
int InterBufReader_UpdateStatus(ADEC_BUF_T *buf, ADEC_COMMON_BUF_STATUS_T *status)
{
    NULL_BUF_CHECK(0xd8);
    LOGD(buf_reader, 0xda, "buf %x, update write index %d -> %d, offset %d -> %d\n",
         buf->offset, buf->status.writeIndex, status->writeIndex, buf->status.writeOffset,
         status->writeOffset);
    buf->status.writeIndex = status->writeIndex;
    buf->status.writeOffset = status->writeOffset;
    buf->status.indexAllocated = status->indexAllocated;
    return 0;
}

/* ========================================================================= */
/* writer                                                                    */
/* ========================================================================= */

/* @0x243a4: nothing is consumed on the writer side */
int InterBufWriter_GetPassSize(ADEC_BUF_T *buf)
{
    return 0;
}

/* @0x243a8: reader-only methods on a writer */
int InterBufWriter_GetAuInfo(int index, int *offset, ADEC_AU_INFO_T *info, ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x243b0 */
int InterBufWriter_GetCurrentInfo(int *offset, ADEC_AU_INFO_T *info, ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x243b8 */
int InterBufWriter_ReadData(unsigned char **dst, int *size, int *au_in_data, ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x243c0 */
int InterBufWriter_FlushData(int size, ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x243c8 */
int InterBufWriter_SetRepeat(int cnt, ADEC_BUF_T *buf)
{
    return -1;
}

/* @0x243d0 */
int InterBufWriter_RequestData(ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x243d8: open, fill and publish one AU, then notify the reader. The data copy runs
 * outside the lock. A failed write throws the AU away (no notify); the caller only
 * learns about it from the return value. */
int InterBufWriter_InsertAuInternal(unsigned char *src, int len, ADEC_AU_INFO_T *info,
                                    ADEC_BUF_T *buf, int from_user)
{
    int ret;

    NULL_BUF_CHECK(0x205);
    LOCKED(CommonBuffer_CreateNewAu(info, buf));
    if (ret) {
        LOGD(buf_writer, 0x20e, "Fail to create au\n");
        return ret;
    }
    if (!from_user)
        ret = CommonBuffer_WriteData(src, len, buf);
    else
        ret = CommonBuffer_WriteDataUser(src, len, buf);
    if (ret) {
        buf->status.indexAllocated = 0;
        LOGE(buf_writer, 0x21d, "Fail to write data\n");
        return ret;
    }
    LOCKED(CommonBuffer_FinishAu(buf));
    ashbuf_notify_write(buf->aimc, buf, buf->coworker);
    return ret;
}

/* @0x24524 */
int InterBufWriter_InsertAu(unsigned char *src, int len, ADEC_AU_INFO_T *info, ADEC_BUF_T *buf)
{
    return InterBufWriter_InsertAuInternal(src, len, info, buf, 0);
}

/* @0x24534 */
int InterBufWriter_InsertAuUser(unsigned char *src, int len, ADEC_AU_INFO_T *info,
                                ADEC_BUF_T *buf)
{
    return InterBufWriter_InsertAuInternal(src, len, info, buf, 1);
}

/* @0x24544 */
int InterBufWriter_Unref(ADEC_BUF_T *buf)
{
    NULL_BUF_CHECK(0x12d);
    return CommonBuffer_Unref(buf);
}

/* @0x245cc */
int InterBufWriter_Ref(ADEC_BUF_T *buf)
{
    NULL_BUF_CHECK(0x126);
    return CommonBuffer_Ref(buf);
}

/* @0x24654 */
int InterBufWriter_FinishAu(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x1ea);
    LOCKED(CommonBuffer_FinishAu(buf));
    ashbuf_notify_write(buf->aimc, buf, buf->coworker);
    return ret;
}

/* @0x2476c */
int InterBufWriter_CreateNewAu(ADEC_AU_INFO_T *info, ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x1a6);
    LOCKED(CommonBuffer_CreateNewAu(info, buf));
    return ret;
}

/* @0x24808 */
int InterBufWriter_UpdateAu(ADEC_AU_INFO_T *info, ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x23b);
    LOCKED(CommonBuffer_UpdateAu(info, buf));
    return ret;
}

/* @0x248a4 */
int InterBufWriter_GetUsedSize(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x14f);
    LOCKED(CommonBuffer_GetUsedSize(buf));
    return ret;
}

/* @0x2493c */
int InterBufWriter_GetMaxSize(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x176);
    LOCKED(CommonBuffer_GetMaxSize(buf));
    return ret;
}

/* @0x249d4 */
int InterBufWriter_GetMaxAuLength(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x196);
    LOCKED(CommonBuffer_GetMaxAuLength(buf));
    return ret;
}

/* @0x24a6c */
int InterBufWriter_GetMaxAu(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x186);
    LOCKED(CommonBuffer_GetMaxAu(buf));
    return ret;
}

/* @0x24b04 */
int InterBufWriter_GetFreeSize(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x137);
    LOCKED(CommonBuffer_GetFreeSize(buf));
    return ret;
}

/* @0x24b9c */
int InterBufWriter_GetAuCnt(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x162);
    LOCKED(CommonBuffer_GetAuCnt(buf));
    return ret;
}

/* @0x24c34 */
int InterBufWriter_Init(ADEC_BUF_T *buf)
{
    int ret;

    NULL_BUF_CHECK(0x113);
    LOCKED(CommonBuffer_Init(buf));
    return ret;
}

/* @0x24f90 */
int InterBufWriter_ReadAu(unsigned char **dst, int *size, int *au_cnt, ADEC_BUF_T *buf)
{
    return -ADEC_ERR_PERMISSION;
}

/* @0x24f98: InterBufWriter_WriteDataInternal(user = 0), inlined null check */
int InterBufWriter_WriteData(unsigned char *src, int len, ADEC_BUF_T *buf)
{
    BUF_ASSERT(buf != NULL, "buf != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x1bf,
               "buf should not be null.\n");
    return CommonBuffer_WriteData(src, len, buf);
}

/* @0x24fa4: InterBufWriter_WriteDataInternal(user = 1) */
int InterBufWriter_WriteDataUser(unsigned char *src, int len, ADEC_BUF_T *buf)
{
    BUF_ASSERT(buf != NULL, "buf != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x1bf,
               "buf should not be null.\n");
    return CommonBuffer_WriteDataUser(src, len, buf);
}

/* @0x24ccc: the writer owns the indices, so it (re)initialises the shared header */
int _InterBufWriter_Create(ADEC_PROCESSOR_ADAP_IF_T *adap, ADEC_COMMON_BUF_T *common_buf,
                           int buffer_size, int max_access_size, int index_count, void *data,
                           unsigned int coworker, struct aimc *imc, ADEC_BUF_T *buf,
                           int from_user)
{
    BUF_ASSERT(adap != NULL, "_adapIf != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x92,
               "adap should not be null.\n");
    BUF_ASSERT(common_buf != NULL, "_commonBufferStruct != ((void *)0)", NULL, -ADEC_ERR_NULL,
               0x93, "common buffer should not be null.\n");
    BUF_ASSERT(buf != NULL, "_struct != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x95,
               "struct should not be null.\n");

    memset(buf, 0, sizeof(*buf));
    if (!from_user) {
        buf->write_data = InterBufWriter_WriteData;
        buf->insert_au = InterBufWriter_InsertAu;
    } else {
        buf->write_data = InterBufWriter_WriteDataUser;
        buf->insert_au = InterBufWriter_InsertAuUser;
    }
    buf->buf_type = INTER_BUF_WRITER_TYPE;
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
    buf->finish_au = InterBufWriter_FinishAu;
    buf->set_repeat = InterBufWriter_SetRepeat;
    buf->request_data = InterBufWriter_RequestData;
    buf->ref = InterBufWriter_Ref;
    buf->unref = InterBufWriter_Unref;
    pthread_mutex_init(&buf->ref_count_mutex, NULL);
    pthread_mutex_init(&buf->lock, NULL);
    buf->ref_count = 1;

    buf->aui_count = index_count;
    buf->commonBuf = common_buf;
    buf->data = data;
    buf->adap = adap;
    buf->max_access_size = max_access_size;
    buf->max_size = buffer_size - max_access_size - CommonBuffer_GetStructSize(index_count);
    buf->aimc = imc;
    buf->coworker = coworker;
    InterBufWriter_Init(buf);
    return 0;
}

/* @0x24fb0 */
int InterBufWriter_GetStructSize(void)
{
    return sizeof(ADEC_BUF_T);
}

/* @0x24fb8 */
int InterBufWriter_Create(ADEC_PROCESSOR_ADAP_IF_T *adap, ADEC_COMMON_BUF_T *common_buf,
                          int buffer_size, int max_access_size, int index_count, void *data,
                          unsigned int coworker, struct aimc *imc, ADEC_BUF_T *buf)
{
    return _InterBufWriter_Create(adap, common_buf, buffer_size, max_access_size, index_count,
                                  data, coworker, imc, buf, 0);
}

/* @0x24fdc */
int InterBufWriter_CreateFromCommonBuffer(ADEC_PROCESSOR_ADAP_IF_T *adap,
                                          ADEC_COMMON_BUF_T *common_buf, void *data,
                                          unsigned int coworker, struct aimc *imc,
                                          ADEC_BUF_T *buf)
{
    int ret = _InterBufWriter_Create(adap, common_buf, 0, common_buf->accessSize,
                                     common_buf->maxIndex, data, coworker, imc, buf, 0);

    buf->max_size = common_buf->maxSize;
    return ret;
}

/* @0x25004 */
int InterBufWriter_CreateFromCommonBufferUser(ADEC_PROCESSOR_ADAP_IF_T *adap,
                                              ADEC_COMMON_BUF_T *common_buf, void *data,
                                              unsigned int coworker, struct aimc *imc,
                                              ADEC_BUF_T *buf)
{
    int ret = _InterBufWriter_Create(adap, common_buf, 0, common_buf->accessSize,
                                     common_buf->maxIndex, data, coworker, imc, buf, 1);

    buf->max_size = common_buf->maxSize;
    return ret;
}

/* @0x25030: remote reader consumed data (BM_EVT_READ) */
int InterBufWriter_UpdateStatus(ADEC_BUF_T *buf, ADEC_COMMON_BUF_STATUS_T *status)
{
    NULL_BUF_CHECK(0xfe);
    LOGD(buf_writer, 0x101, "buf %x, update  read index %d -> %d, offset %d -> %d\n",
         buf->offset, buf->status.readIndex, status->readIndex, buf->status.readOffset,
         status->readOffset);
    buf->status.readIndex = status->readIndex;
    buf->status.readOffset = status->readOffset;
    return 0;
}
