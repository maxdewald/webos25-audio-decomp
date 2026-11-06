/*
 * Common (ring) buffer shared between ARM and DSP, its remote-event notification and
 * the buffer debug helpers.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 *
 * Evidence: Ghidra build/decomp/libaa.so.2.0.0.c @0x25104..@0x26e40, __func__ strings in
 * build/decomp/libaa.so.2.0.0.strings.tsv (0x35564..0x3623c), disassembly of the
 * divide helper (@0x317c4 = __aeabi_uidivmod, so index arithmetic is unsigned).
 *
 * Model (see buffers.h for the layouts):
 *   - The shared block starts with ADEC_COMMON_BUF_T (maxSize/accessSize/maxIndex + AU
 *     index table); the data area follows: [mirror (accessSize)][ring (maxSize)].
 *   - status.readOffset/writeOffset are byte offsets into the ring, readIndex/writeIndex
 *     are AU slots. One AU slot and 4 bytes are always kept free so "empty" (r == w) and
 *     "full" can be told apart.
 *   - A writer opens an AU (CreateNewAu), appends data (WriteData, 'written' grows) and
 *     publishes it (FinishAu advances writeIndex/writeOffset). A full ring is an error
 *     (-ADEC_ERR_OVERFLOW): nothing blocks and nothing is overwritten.
 *   - A reader looks at the AU table (GetAuInfo/ReadAu), reads bytes (ReadData; a wrapped
 *     read is made contiguous through the mirror area or the reader's boundary_buf) and
 *     releases them (FlushData).
 * The ARM side never owns both ends: status of the remote end arrives through
 * InterBuf{Reader,Writer}_UpdateStatus and local progress is sent with ashbuf_notify_*.
 */
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "buffers.h"

LOGMM_MODULE_DEFINE(buf_com);
LOGMM_MODULE_DEFINE(buf_event);

static unsigned int buf_assert_count;   /* @0x51198 */
static unsigned int common_written;     /* @0x51194: bytes written since last reset */

#define AU_ENTRY(buf, i)    (&(buf)->commonBuf->indexTable[(i)])
#define CACHE_INV(buf, p, n) (buf)->adap->pfnInvalidate((p), (n), (buf))
#define CACHE_WB(buf, p, n)  (buf)->adap->pfnWriteback((p), (n), (buf))

/* @0x25104: shared header + AU index table (one extra, unused entry) */
int CommonBuffer_GetStructSize(int index_count)
{
    if (index_count < 1)
        index_count = 1;
    return index_count * (int)sizeof(ADEC_COM_BUF_AU_INFO_T) + 0x38;
}

/* @0x25114 */
int CommonBuffer_GetStatusStructSize(void)
{
    return sizeof(ADEC_COMMON_BUF_STATUS_T);
}

/* @0x25118: empty the ring; slot 0 is cleared and written back for the remote side */
int CommonBuffer_Init(ADEC_BUF_T *buf)
{
    ADEC_COMMON_BUF_T *cb;

    BUF_ASSERT(buf->commonBuf != NULL, "buf->commonBuf != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x62,
               "common_buf should not be null.\n");

    buf->status.indexAllocated = 0;
    buf->status.readOffset = 0;
    buf->status.writeOffset = 0;
    buf->status.readIndex = 0;
    buf->status.writeIndex = 0;
    buf->status.repeat_count = 0;
    memset(&buf->commonBuf->indexTable[0].base, 0, sizeof(ADEC_AU_INFO_T));
    cb = buf->commonBuf;
    cb->indexTable[0].bufOffset = 0;
    CACHE_WB(buf, &cb->indexTable[0], sizeof(ADEC_COM_BUF_AU_INFO_T));
    return 0;
}

/* @0x251b8: the first max_au_size bytes of the data area are the mirror area */
int CommonBuffer_Create(int buffer_size, int max_au_size, int index_count, ADEC_BUF_T *buf)
{
    ADEC_COMMON_BUF_T *cb;

    BUF_ASSERT(buffer_size >= max_au_size, "_bufferSize >= _maxAuSize", NULL, -ADEC_ERR_RANGE, 0x48,
               "_bufferSize should be greater than _maxAuSize.\n");

    if (index_count == 0)
        index_count = 1;
    cb = buf->commonBuf;
    cb->maxIndex = index_count;
    cb->maxSize = buffer_size - max_au_size;
    cb->accessSize = max_au_size;
    buf->max_access_size = max_au_size;
    buf->max_size = buffer_size - max_au_size;
    buf->aui_count = index_count;
    CommonBuffer_Init(buf);
    return 0;
}

/* @0x25258: free bytes, 0 when the AU table is full; 4 bytes are kept as a gap */
int CommonBuffer_GetFreeSize(ADEC_BUF_T *buf)
{
    ADEC_COMMON_BUF_STATUS_T *st = &buf->status;

    if ((unsigned int)(st->writeIndex + 1) % buf->aui_count == (unsigned int)st->readIndex)
        return 0;
    if (st->readOffset > st->writeOffset)
        return st->readOffset - st->writeOffset - 4;
    return st->readOffset + (buf->max_size - 4) - st->writeOffset;
}

/* @0x2528c: bytes flushed since the previous call */
int CommonBuffer_GetPassSize(ADEC_BUF_T *buf)
{
    int passed = buf->status.passed;

    buf->status.passed = 0;
    return passed;
}

/* @0x25298 */
int CommonBuffer_GetUsedSize(ADEC_BUF_T *buf)
{
    unsigned int used = buf->status.writeOffset - buf->status.readOffset;

    if ((int)used < 0)
        used += buf->max_size;
    BUF_ASSERT(used <= buf->max_size && (int)used >= 0,
               "usedSize <= buf->max_size && usedSize >= 0", buf, -ADEC_ERR_DATA_CORRUPTED, 0x9a,
               "used size is invalid\n");
    return used;
}

/* @0x25328: published AUs not yet consumed */
int CommonBuffer_GetAuCnt(ADEC_BUF_T *buf)
{
    int cnt = buf->status.writeIndex - buf->status.readIndex;

    if (cnt < 0)
        cnt += buf->aui_count;
    return cnt;
}

/* @0x2533c */
int CommonBuffer_GetMaxSize(ADEC_BUF_T *buf)
{
    return buf->max_size;
}

/* @0x25340 */
int CommonBuffer_GetMaxAu(ADEC_BUF_T *buf)
{
    return buf->aui_count;
}

/* @0x25344 */
int CommonBuffer_GetMaxAuLength(ADEC_BUF_T *buf)
{
    return buf->max_access_size;
}

/* @0x25348: debug counter of bytes written by all common buffers in this process */
void common_written_and_reset(void)
{
    LOGN(buf_com, 0x12e, "written %d\n", common_written);
    common_written = 0;
}

/* @0x253a4: publish the open AU (size = bytes written into it) */
int CommonBuffer_FinishAu(ADEC_BUF_T *buf)
{
    ADEC_COMMON_BUF_STATUS_T *st = &buf->status;
    ADEC_COMMON_BUF_T *cb = buf->commonBuf;
    int wi = st->writeIndex;
    unsigned int wo = st->writeOffset;
    unsigned int new_wo;

    if (!st->indexAllocated) {
        LOGE(buf_com, 0x28f, "no au created\n");
        return -ADEC_ERR_OVERFLOW;
    }

    new_wo = st->written + wo;
    if (new_wo >= buf->max_size)
        new_wo -= buf->max_size;
    LOGD(buf_com, 0x2a5, "base %p, finish AU. write offset %4d -> %4d, index %2d -> %2d\n",
         buf->data, wo, new_wo, wi, (unsigned int)(wi + 1) % buf->aui_count);

    cb->indexTable[wi].base.size = st->written;
    CACHE_WB(buf, AU_ENTRY(buf, wi), sizeof(ADEC_COM_BUF_AU_INFO_T));
    st->writeOffset = new_wo;
    st->writeIndex = (unsigned int)(wi + 1) % buf->aui_count;
    st->indexAllocated = 0;
    st->written = 0;
    return 0;
}

/* @0x25490: open a new AU at writeIndex. reserve > 0 also requires that many free bytes.
 * Fails silently with -ADEC_ERR_OVERFLOW when the AU table or the ring is full. */
int _CommonBuffer_CreateNewAu(ADEC_AU_INFO_T *au, ADEC_BUF_T *buf, unsigned int reserve)
{
    ADEC_COMMON_BUF_STATUS_T *st = &buf->status;
    ADEC_COMMON_BUF_T *cb = buf->commonBuf;
    int wi = st->writeIndex;
    int used_index = wi - st->readIndex;
    ADEC_COM_BUF_AU_INFO_T *e;

    if (used_index < 0)
        used_index += buf->aui_count;
    used_index++;
    BUF_ASSERT((unsigned int)used_index <= buf->aui_count && used_index >= 0,
               "usedIndex <= buf->aui_count && usedIndex >= 0", buf,
               -ADEC_ERR_DATA_CORRUPTED, 199, "Invalide used index cnt");

    if ((unsigned int)used_index == buf->aui_count)
        return -ADEC_ERR_OVERFLOW;
    if (reserve) {
        int pos = st->writeOffset + st->written;
        int room;

        if (pos >= (int)buf->max_size)
            pos -= buf->max_size;
        room = st->writeOffset < st->readOffset ? -pos : (int)buf->max_size - pos;
        if ((unsigned int)(st->readOffset + room) - 4 < reserve)
            return -ADEC_ERR_OVERFLOW;
    }

    if (st->indexAllocated) {
        CommonBuffer_FinishAu(buf);
        wi = st->writeIndex;
    }
    e = &cb->indexTable[wi];
    if (au)
        e->base = *au;
    else
        memset(&e->base, 0, sizeof(e->base));
    e->bufOffset = st->writeOffset;
    e->base.size = 0;
    st->written = reserve;
    st->indexAllocated = 1;
    LOGD(buf_com, 0xfe, "base %p, create au, index %2d, buf offset %4d\n", buf->data, wi,
         st->writeOffset);
    return 0;
}

/* @0x2561c */
int CommonBuffer_CreateNewAu(ADEC_AU_INFO_T *au, ADEC_BUF_T *buf)
{
    return _CommonBuffer_CreateNewAu(au, buf, 0);
}

/* @0x25620: append len bytes to the open AU (opening one if needed). src NULL writes
 * zeros. A user-space source (from_user) is a kernel-only path: in libaa it only logs.
 * Not enough room -> error log and -ADEC_ERR_OVERFLOW, nothing is written. */
int CommonBuffer_WriteDataInternal(unsigned char *src, unsigned int len, ADEC_BUF_T *buf,
                                   int from_user)
{
    ADEC_COMMON_BUF_STATUS_T *st = &buf->status;
    unsigned int max = buf->max_size;
    int ret, pos, room, free_size;
    unsigned char *ring, *dst;

    if (!st->indexAllocated && (ret = CommonBuffer_CreateNewAu(NULL, buf)) != 0) {
        LOGE(buf_com, 0x147, "Cannot create new AU\n");
        return ret;
    }

    pos = st->writeOffset + st->written;
    if (pos >= (int)max)
        pos -= max;
    room = st->writeOffset < st->readOffset ? -pos : (int)max - pos;
    free_size = st->readOffset + room - 4;
    if (free_size < (int)len) {
        LOGE(buf_com, 0x15e, "Buffer overflow. writing %d, free %d/%d\n", len, free_size, max);
        return -ADEC_ERR_OVERFLOW;
    }
    LOGD(buf_com, 0x163,
         "base %p, write data. write index %2d, offset %4d, written %d, size %d\n", buf->data, st->writeIndex, st->writeOffset, st->written, len);

    ring = buf->data + buf->max_access_size;
    dst = ring + pos;
    if ((int)(pos + len) < (int)max) {
        /* CommonBuffer_CopyToBuffer (inlined) */
        if (!src)
            memset(dst, 0, len);
        else if (!from_user)
            memcpy(dst, src, len);
        else
            LOGF(buf_com, log_level_error, "CommonBuffer_CopyToBuffer", 0x117,
                 "BUG: DSP shouldn't use this option!!!\n");
        CACHE_WB(buf, dst, len);
    } else {
        unsigned int first = max - pos, second = len - first;

        if (!src) {
            memset(dst, 0, first);
            CACHE_WB(buf, dst, first);
            memset(ring, 0, second);
        } else if (!from_user) {
            memcpy(dst, src, first);
            CACHE_WB(buf, dst, first);
            memcpy(ring, src + first, second);
        } else {
            LOGF(buf_com, log_level_error, "CommonBuffer_CopyToBuffer", 0x117,
                 "BUG: DSP shouldn't use this option!!!\n");
            CACHE_WB(buf, dst, first);
            LOGF(buf_com, log_level_error, "CommonBuffer_CopyToBuffer", 0x117,
                 "BUG: DSP shouldn't use this option!!!\n");
        }
        CACHE_WB(buf, ring, second);
    }
    st->written += len;
    common_written += len;
    return 0;
}

/* @0x2587c */
int CommonBuffer_WriteData(unsigned char *src, int len, ADEC_BUF_T *buf)
{
    return CommonBuffer_WriteDataInternal(src, len, buf, 0);
}

/* @0x25880 */
int CommonBuffer_WriteDataUser(unsigned char *src, int len, ADEC_BUF_T *buf)
{
    return CommonBuffer_WriteDataInternal(src, len, buf, 1);
}

/* @0x25884: open an AU with size bytes reserved and return where to put them */
void *CommonBuffer_RequestMem(ADEC_AU_INFO_T *au, ADEC_BUF_T *buf, unsigned int size)
{
    if (_CommonBuffer_CreateNewAu(au, buf, size) < 0)
        return NULL;
    return buf->data + buf->max_access_size + buf->status.writeOffset;
}

/* @0x258a0: CreateNewAu + WriteData + FinishAu. On a write failure the AU is dropped
 * (indexAllocated cleared, index not advanced). */
int CommonBuffer_InsertAuInternal(unsigned char *src, int len, ADEC_AU_INFO_T *au,
                                  ADEC_BUF_T *buf, int from_user)
{
    ADEC_COMMON_BUF_STATUS_T *st = &buf->status;
    int ret;

    LOGD(buf_com, 0x2bf, "base %p, au %d-%d, data %d-%d, indexalloc %d, written %d\n",
         buf->data, st->readIndex, st->writeIndex, st->readOffset, st->writeOffset,
         st->indexAllocated, st->written);

    ret = CommonBuffer_CreateNewAu(au, buf);
    if (ret) {
        LOGD(buf_com, 0x2c5, "Fail to create au\n");
        return ret;
    }
    ret = CommonBuffer_WriteDataInternal(src, len, buf, from_user);
    if (ret) {
        st->indexAllocated = 0;
        LOGE(buf_com, 0x2cd, "Fail to write data\n");
        return ret;
    }
    CommonBuffer_FinishAu(buf);
    LOGD(buf_com, 0x2d2, "base %p, au %d-%d, data %d-%d, indexalloc %d, written %d\n",
         buf->data, st->readIndex, st->writeIndex, st->readOffset, st->writeOffset,
         st->indexAllocated, st->written);
    return 0;
}

/* @0x259e8 */
int CommonBuffer_InsertAu(unsigned char *src, int len, ADEC_AU_INFO_T *au, ADEC_BUF_T *buf)
{
    return CommonBuffer_InsertAuInternal(src, len, au, buf, 0);
}

/* @0x259f8 */
int CommonBuffer_InsertAuUser(unsigned char *src, int len, ADEC_AU_INFO_T *au,
                              ADEC_BUF_T *buf)
{
    return CommonBuffer_InsertAuInternal(src, len, au, buf, 1);
}

/* @0x25a08: replace the info of the open AU */
int CommonBuffer_UpdateAu(ADEC_AU_INFO_T *au, ADEC_BUF_T *buf)
{
    ADEC_COMMON_BUF_T *cb = buf->commonBuf;

    if (!buf->status.indexAllocated)
        return -1;
    LOGD(buf_com, 0x2ec, "base %p, update au, index %d\n", buf->data, buf->status.writeIndex);
    CACHE_INV(buf, AU_ENTRY(buf, buf->status.writeIndex), sizeof(ADEC_COM_BUF_AU_INFO_T));
    cb->indexTable[buf->status.writeIndex].base = *au;
    CACHE_WB(buf, AU_ENTRY(buf, buf->status.writeIndex), sizeof(ADEC_COM_BUF_AU_INFO_T));
    return 0;
}

/* @0x25ad8: info of the index-th unread AU (negative = from the newest one). *offset is
 * the AU's byte distance from readOffset. */
int CommonBuffer_GetAuInfo(int index, int *offset, ADEC_AU_INFO_T *au, ADEC_BUF_T *buf)
{
    ADEC_COMMON_BUF_STATUS_T *st = &buf->status;
    ADEC_COMMON_BUF_T *cb = buf->commonBuf;
    int wi = st->writeIndex, ri = st->readIndex;
    int used, off;
    unsigned int size;

    BUF_ASSERT(au != NULL, "_au != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x2fd, "_au_info should not be null.\n");

    if (wi < ri)
        wi += buf->aui_count;
    used = wi - ri;
    if (index < 0) {
        if (-used != index && used <= -index)
            return -ADEC_ERR_RANGE;
        index += used;
    } else if (used <= index) {
        return -ADEC_ERR_RANGE;
    }

    CACHE_INV(buf, AU_ENTRY(buf, ri), sizeof(ADEC_COM_BUF_AU_INFO_T));
    off = cb->indexTable[ri].bufOffset - st->readOffset;
    size = cb->indexTable[ri].base.size;
    if (st->readOffset < cb->indexTable[ri].bufOffset)
        off -= buf->max_size;
    BUF_ASSERT(!(size > buf->max_size) || st->readIndex == st->writeIndex,
               "_status->readIndex == _status->writeIndex", buf,
               -ADEC_ERR_DATA_CORRUPTED, 0x317,
               "The case, au size is greater than max buf size, is allowed if and only if au is not used\n");

    while (index > 0) {
        off += size;
        index--;
        ri = (unsigned int)(ri + 1) % buf->aui_count;
        CACHE_INV(buf, AU_ENTRY(buf, ri), sizeof(ADEC_COM_BUF_AU_INFO_T));
        if (index == 0)
            break;
        size = cb->indexTable[ri].base.size;
    }
    LOGD(buf_com, 0x323, "base %p, get au, index %d\n", buf->data, ri);
    if (offset)
        *offset = off;
    *au = cb->indexTable[ri].base;
    return 0;
}

/* @0x25cf8 */
int CommonBuffer_GetCurrentInfo(int *offset, ADEC_AU_INFO_T *au, ADEC_BUF_T *buf)
{
    return CommonBuffer_GetAuInfo(0, offset, au, buf);
}

/* @0x25d08: map up to *size unread bytes at readOffset (does not consume them).
 * A read across the ring end is made contiguous: into the reader's boundary_buf when
 * present, otherwise by copying the tail into the mirror area just before the ring. */
int CommonBuffer_ReadData(unsigned char **dst, int *size, int *au_in_data, ADEC_BUF_T *buf)
{
    ADEC_COMMON_BUF_STATUS_T *st = &buf->status;
    int used;
    unsigned int ro, max, access, want, first, second;
    unsigned char *ring;

    (void)au_in_data;
    BUF_ASSERT(dst != NULL, "_dst != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x339, "_dst should not be null.\n");
    BUF_ASSERT(size != NULL, "_size != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x33a, "_size should not be null.\n");
    if (*size == 0)
        return 0;
    LOGD(buf_com, 0x33f, "base %p, au %d-%d, data %d-%d, indexalloc %d, written %d\n",
         buf->data, st->readIndex, st->writeIndex, st->readOffset, st->writeOffset,
         st->indexAllocated, st->written);
    if (st->writeIndex == st->readIndex)
        return -ADEC_ERR_UNDERFLOW;

    used = st->writeOffset - st->readOffset;
    if ((int)used < 0)
        used += buf->max_size;
    BUF_ASSERT((unsigned int)used <= buf->max_size, "usedSize <= buf->max_size", buf, -ADEC_ERR_DATA_CORRUPTED, 0x34b,
               "used size exceed max size of the buffer\n");
    BUF_ASSERT((int)used >= 0, "usedSize >= 0", buf, -ADEC_ERR_DATA_CORRUPTED, 0x34d,
               "usedSize is smaller than 0.\n");
    LOGD(buf_com, 0x350, "base %p, read data, index %2d, size %d(%d)\n", buf->data,
         st->readIndex, used, *size);
    if ((int)used < *size)
        *size = used;

    ro = st->readOffset;
    max = buf->max_size;
    BUF_ASSERT(ro <= max, "_status->readOffset <= buf->max_size", buf,
               -ADEC_ERR_DATA_CORRUPTED, 0x355, "Current readPtr is greater than bufferEnd.\n");
    want = *size;
    access = buf->max_access_size;
    if (ro + want > max) {
        /* wraps: limit to what the mirror area (or boundary_buf) can hold */
        if (!buf->boundary_buf) {
            first = max - ro;
            if (first > access) {
                LOGI(buf_com, 0x36b, "boundary. limit size %d to %d. max %d\n", want, first,
                     max);
                *size = want = first;
            }
        } else if (want > access) {
            LOGI(buf_com, 0x35f, "boundary. limit size %d to %d. max %d\n", want, access, max);
            *size = want = buf->max_access_size;
        }
        ro = st->readOffset;
        max = buf->max_size;
        access = buf->max_access_size;
    }
    ring = buf->data + access;
    if (ro + want <= max) {
        *dst = ring + ro;
        CACHE_INV(buf, ring + ro, want);
        return 0;
    }

    first = max - ro;
    second = want - first;
    BUF_ASSERT(first <= buf->max_access_size, "size1 <= buf->max_access_size", buf,
               -ADEC_ERR_GENERAL, 0x37d,
               "Mirroring Data size is too big, boundary protection is broken\n");
    CACHE_INV(buf, ring, second);
    CACHE_INV(buf, ring + st->readOffset, first);
    if (!buf->boundary_buf) {
        *dst = ring - first;
        memcpy(ring - first, ring + st->readOffset, first);
        return 0;
    }
    *dst = buf->boundary_buf;
    memcpy(buf->boundary_buf, ring + st->readOffset, first);
    memcpy((unsigned char *)buf->boundary_buf + first, ring, second);
    return 0;
}

/* @0x26190: consume *size bytes; readIndex follows over every AU that is fully consumed.
 * If the AU at readIndex does not contain readOffset the table is considered corrupted
 * and the reader jumps to the writer position: all unread data is dropped. */
int CommonBuffer_FlushData(int *size, ADEC_BUF_T *buf)
{
    ADEC_COMMON_BUF_STATUS_T *st = &buf->status;
    ADEC_COMMON_BUF_T *cb = buf->commonBuf;
    unsigned int flush, used, ro, start, end, max, n, index;
    int aus;
    short ri;

    flush = *size;
    if (st->readIndex == st->writeIndex)
        return flush ? -ADEC_ERR_UNDERFLOW : 0;

    used = st->writeOffset - st->readOffset;
    if ((int)used < 0)
        used += buf->max_size;
    LOGD(buf_com, 0x3ab, "base %p, flush data, used size %d, flushing %d\n", buf->data, used,
         flush);
    if ((int)*size <= (int)used)
        used = *size;
    if ((int)used < 0) {
        *size = 0;
        return 0;
    }

    ri = st->readIndex;
    index = ri;
    BUF_ASSERT((unsigned int)st->readOffset <= buf->max_size,
               "_status->readOffset <= buf->max_size", buf, -ADEC_ERR_DATA_CORRUPTED, 0x3b9,
               "Current readPtr is greater than bufferEnd.\n");
    CACHE_INV(buf, AU_ENTRY(buf, index), sizeof(ADEC_COM_BUF_AU_INFO_T));

    ro = st->readOffset;
    start = cb->indexTable[index].bufOffset;
    max = buf->max_size;
    if ((int)ro < (int)start || (end = cb->indexTable[index].base.size + start) < ro) {
        /* readOffset may sit past the ring end of an AU that wrapped */
        if (max + ro < start || (end = cb->indexTable[index].base.size + start) < max + ro) {
            if (LOGMM_ON(buf_com, log_level_error)) {
                logmm_printl(&logmm_module_buf_com, log_level_error, __func__, 0x3c9,
                             "index table corrupted.\n");
                LOGE(buf_com, 0x3ca,
                     "status r/w offset %d/%d, r/w index %d/%d, max size %d, max index %d\n",
                     st->readOffset, st->writeOffset, st->readIndex, st->writeIndex,
                     buf->max_size, buf->aui_count);
                if (LOGMM_ON(buf_com, log_level_error)) {
                    logmm_printl(&logmm_module_buf_com, log_level_error, __func__, 0x3cd,
                                 "indextable buf offset %d, size %d\n",
                                 cb->indexTable[index].bufOffset,
                                 cb->indexTable[index].base.size);
                    LOGE(buf_com, 0x3cf, "reset buffer status\n");
                }
            }
            st->readOffset = st->writeOffset;
            st->readIndex = st->writeIndex;
            return -ADEC_ERR_CORRUPTED;
        }
    }

    /* end = ring offset just past the AU at readIndex; n = bytes left in it */
    n = end - ro;
    if (n >= max)
        n -= max;
    aus = st->writeIndex < ri ? (int)buf->aui_count + st->writeIndex - ri
                              : st->writeIndex - ri;
    if ((int)used >= (int)n && aus >= 1) {
        int k = 0;

        /* step readIndex over every AU that ends inside the flushed range */
        while ((int)n >= 0) {
            unsigned int sz;
            int exact;

            k++;
            index = (index + 1) % buf->aui_count;
            CACHE_INV(buf, AU_ENTRY(buf, index), sizeof(ADEC_COM_BUF_AU_INFO_T));
            ri = index;
            if ((unsigned int)st->writeIndex == index) {
                BUF_ASSERT(used == n, "size == flushSize", buf, -ADEC_ERR_DATA_CORRUPTED,
                           0x3f4, "In this case the size(%d) should be equal to flushSize(%d)\n", n,
                           used);
                goto done;
            }
            BUF_ASSERT(index < buf->aui_count, "index < buf->aui_count", buf,
                       -ADEC_ERR_DATA_CORRUPTED, 0x3fa, "index err\n");
            sz = cb->indexTable[index].base.size;
            BUF_ASSERT(sz < buf->max_size, "_buf->indexTable[index].base.size < buf->max_size",
                       buf, -ADEC_ERR_DATA_CORRUPTED, 0x3fb,
                       "The case, au size is greater than max buf size, is allowed if and only if au is not used\n");
            exact = used == n;
            n += sz;
            if ((sz == 0 && exact) || k >= aus || (int)used < (int)n)
                goto done;
        }
        LOGE(buf_com, 0x3e6,
             "Indextable corrupted. WO : %d. RO : %d. WI : %d. RI : %d. IO : %d.\n",
             st->writeOffset, st->readOffset, st->writeIndex, st->readIndex,
             cb->indexTable[index].bufOffset);
    }
done:
    st->readIndex = ri;
    BUF_ASSERT((unsigned int)ri < buf->aui_count, "_status->readIndex < buf->aui_count", buf,
               -ADEC_ERR_DATA_CORRUPTED, 0x404, "index err\r");
    st->readOffset += used;
    if ((unsigned int)st->readOffset >= buf->max_size)
        st->readOffset -= buf->max_size;
    *size = used;
    st->passed += used;
    return 0;
}

/* @0x266b4: map *au_cnt whole AUs starting at readIndex (contiguous thanks to the mirror
 * area). Returns their byte count in *size. */
int CommonBuffer_ReadAu(unsigned char **dst, int *size, int *au_cnt, ADEC_BUF_T *buf)
{
    ADEC_COMMON_BUF_STATUS_T *st = &buf->status;
    ADEC_COMMON_BUF_T *cb = buf->commonBuf;
    int ri = st->readIndex, cnt, i;
    unsigned int ro = st->readOffset, max, bytes, access;
    unsigned char *p;

    BUF_ASSERT(dst != NULL && size != NULL, "_dst != ((void *)0) && _size != ((void *)0)", NULL, -ADEC_ERR_NULL, 0x41b,
               "Some arguments, should not be null, are null.\n");

    if (ri == st->writeIndex) {
        if ((unsigned int)st->writeOffset == ro)
            return -ADEC_ERR_UNDERFLOW;
        goto inconsistent;
    }
    if ((unsigned int)st->writeOffset == ro)
        goto inconsistent;

    cnt = st->writeIndex - ri;
    if (cnt < 0)
        cnt += buf->aui_count;
    if (*au_cnt > cnt) {
        *au_cnt = cnt;
        ro = st->readOffset;
    }
    max = buf->max_size;
    BUF_ASSERT(ro <= max, "_status->readOffset <= buf->max_size", buf,
               -ADEC_ERR_DATA_CORRUPTED, 0x42e, "Current readPtr is greater than bufferEnd.\n");
    BUF_ASSERT(cb->indexTable[ri].base.size < max, "_status->readIndex == _status->writeIndex", buf, -ADEC_ERR_GENERAL, 0x442,
               "Currently, maximum size of a AU is should be smller than buffer max size\n");

    bytes = cb->indexTable[ri].base.size + cb->indexTable[ri].bufOffset - ro;
    if (bytes > max)
        bytes -= max;
    /* [GAP?] the loop starts at readIndex again, so the first AU is counted twice;
     * reproduced as found in the binary */
    for (i = ri; i < ri + *au_cnt; i++)
        bytes += cb->indexTable[(unsigned int)i % buf->aui_count].base.size;

    access = buf->max_access_size;
    p = buf->data + access;
    if (ro > max - access) {
        BUF_ASSERT(*size + ro - max < access,
                   "_status->readOffset + *_size - buf->max_size < buf->max_access_size", buf, -ADEC_ERR_GENERAL, 0x44f,
                   "access size too big, boundary protection is broken\n");
        ro -= max;
    }
    p += ro;
    *dst = p;
    *size = bytes;
    CACHE_INV(buf, p, bytes);
    return 0;

inconsistent:
    /* exactly one of (index, offset) says "empty": always fails */
    BUF_ASSERT(0, "(_status->readIndex == _status->writeIndex) && "
               "_status->readOffset == _status->writeOffset", buf, -ADEC_ERR_DATA_CORRUPTED, 0x420,
               "if usedIndex or usedSize is 0, then both of them should be 0.\n");
}

/* @0x2697c */
int CommonBuffer_Ref(ADEC_BUF_T *buf)
{
    pthread_mutex_lock(&buf->ref_count_mutex);
    buf->ref_count++;
    pthread_mutex_unlock(&buf->ref_count_mutex);
    return 0;
}

/* @0x269a0: last reference frees the boundary buffer and the ADEC_BUF_T itself */
int CommonBuffer_Unref(ADEC_BUF_T *buf)
{
    int ref;

    pthread_mutex_lock(&buf->ref_count_mutex);
    ref = --buf->ref_count;
    if (ref) {
        pthread_mutex_unlock(&buf->ref_count_mutex);
        return ref;
    }
    if (buf->boundary_buf)
        free(buf->boundary_buf);
    pthread_mutex_unlock(&buf->ref_count_mutex);
    free(buf);
    return 0;
}

/* @0x269dc: tell the remote writer how far we have read (BM_EVT_READ) */
void ashbuf_notify_read(struct aimc *imc, ADEC_BUF_T *buf, unsigned int coworker)
{
    BmCmdProcessEvtRemote evt;

    memset(&evt, 0, sizeof(evt));
    LOGD(buf_event, 0x39, "notify read to %x, base 0x%08x, offset %d, index %d\n", coworker,
         buf->offset, buf->status.readOffset, buf->status.readIndex);
    evt.base_addr = buf->offset;
    evt.evt_type = BM_EVT_READ;
    evt.status = buf->status;
    aimc_send_command(imc, BM_CMD_PROCESS_EVT_REMOTE, 0, coworker, &evt, sizeof(evt));
}

/* @0x26a78: tell the remote reader that new AUs were published (BM_EVT_WRITE) */
void ashbuf_notify_write(struct aimc *imc, ADEC_BUF_T *buf, unsigned int coworker)
{
    BmCmdProcessEvtRemote evt;

    memset(&evt, 0, sizeof(evt));
    LOGD(buf_event, 0x48, "notify write to %x, base 0x%08x, offset %d, index %d\n", coworker,
         buf->offset, buf->status.writeOffset, buf->status.writeIndex);
    evt.base_addr = buf->offset;
    evt.evt_type = BM_EVT_WRITE;
    evt.status = buf->status;
    aimc_send_command(imc, BM_CMD_PROCESS_EVT_REMOTE, 0, coworker, &evt, sizeof(evt));
}

/* @0x26b14 */
void modlink(AdecModuleLink *link)
{
    if (!LOGMM_ON(aud, log_level_noti))
        return;
    logmm_printl(&logmm_module_aud, log_level_noti, __func__, 0x2a, "  link %p\n", link);
    if (!LOGMM_ON(aud, log_level_noti))
        return;
    logmm_printl(&logmm_module_aud, log_level_noti, __func__, 0x2d, "ext_addr %08x\n",
                 link->ext_addr);
    if (!LOGMM_ON(aud, log_level_noti))
        return;
    logmm_printl(&logmm_module_aud, log_level_noti, __func__, 0x2e, "  module %p\n",
                 link->module);
    if (link->module) {
        if (!LOGMM_ON(aud, log_level_noti))
            return;
        logmm_printl(&logmm_module_aud, log_level_noti, __func__, 0x31, "       id 0x%x\n",
                     link->module->module_id);
        if (!LOGMM_ON(aud, log_level_noti))
            return;
        logmm_printl(&logmm_module_aud, log_level_noti, __func__, 0x32, "     name %s\n",
                     link->module->module_name);
    }
    LOGN(aud, 0x34, "    port 0x%x\n", link->port);
}

/* @0x26c1c: breakpoint anchor, counts failed buffer asserts */
void BufDbg_AssertBreak(void)
{
    buf_assert_count++;
}

/* @0x26c2c: noti-level dump; stops at the first disabled check like the original */
void BufDbg_Dump(ADEC_BUF_T *buf)
{
#define DUMP(line, ...) do { \
        if (!LOGMM_ON(aud, log_level_noti)) \
            return; \
        logmm_printl(&logmm_module_aud, log_level_noti, __func__, (line), __VA_ARGS__); \
    } while (0)

    if (!buf) {
        LOGN(aud, 0x3c, "null buf\n");
        return;
    }
    if (LOGMM_ON(aud, log_level_noti)) {
        logmm_printl(&logmm_module_aud, log_level_noti, __func__, 0x40, "   buf: %p\n", buf);
        if (LOGMM_ON(aud, log_level_noti)) {
            logmm_printl(&logmm_module_aud, log_level_noti, __func__, 0x41, "offset: %x\n",
                         buf->offset);
            if (LOGMM_ON(aud, log_level_noti)) {
                logmm_printl(&logmm_module_aud, log_level_noti, __func__, 0x42,
                             "  data: %p\n", buf->data);
                if (LOGMM_ON(aud, log_level_noti)) {
                    logmm_printl(&logmm_module_aud, log_level_noti, __func__, 0x43,
                                 "combuf: %p\n", buf->commonBuf);
                    LOGN(aud, 0x44, "input link\n");
                }
            }
        }
    }
    modlink(&buf->input);
    LOGN(aud, 0x46, "output link\n");
    modlink(&buf->output);
    if (buf->commonBuf) {
        DUMP(0x4a, "Mindex: %d\n", buf->commonBuf->maxIndex);
        DUMP(0x4b, " Msize: %d\n", buf->commonBuf->maxSize);
        DUMP(0x4c, "access: %d\n", buf->commonBuf->accessSize);
    }
    DUMP(0x4e, "    wo: %d\n", buf->status.writeOffset);
    DUMP(0x4f, "    ro: %d\n", buf->status.readOffset);
    DUMP(0x50, "    wi: %d\n", buf->status.writeIndex);
    DUMP(0x51, "    ri: %d\n", buf->status.readIndex);
    DUMP(0x52, "    ia: %d\n", buf->status.indexAllocated);
#undef DUMP
}
