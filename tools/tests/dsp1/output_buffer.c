#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @match src/dsp1/output_buffer.c (?s)struct output_buffer_status \{.*?\n\};
// @match src/dsp1/output_buffer.c (?s)struct output_au_info \{.*?\n\};
// @match src/dsp1/output_buffer.c (?s)struct output_common_buffer \{.*?\n\};
struct processor_adapter { unsigned identity; };
// @match src/dsp1/output_buffer.c (?s)struct interbuf_writer \{.*?\n\};
void BufDbg_AssertBreak(void) { abort(); }
static unsigned ps, initial_ps, notices, committed, payload_bytes;
static int create_error, copy_error, finish_error;
static unsigned char storage[64];
static struct output_au_info current_info;
static int init_error;
static unsigned init_capacity;

unsigned CommonBuffer_GetStructSize(unsigned count) { return 12 + 40 * count; }
int CommonBuffer_Init(struct interbuf_writer *buf)
{
    assert((ps & 15) >= 3 && buf->ref_count == 1 && buf->buf_type == 1);
    init_capacity = buf->max_size;
    if (init_error) return init_error;
    buf->status = (struct output_buffer_status){ .read_index = -1, .write_index = -1 };
    return 0;
}
int CommonBuffer_Ref(struct interbuf_writer *buf) { return ++buf->ref_count; }
int CommonBuffer_Unref(struct interbuf_writer *buf) { return --buf->ref_count; }
int CommonBuffer_GetUsedSize(struct interbuf_writer *buf)
{ assert((ps & 15) >= 3); return buf->status.write_offset - buf->status.read_offset; }
int CommonBuffer_GetFreeSize(struct interbuf_writer *buf)
{ return (int)buf->max_size - CommonBuffer_GetUsedSize(buf); }
int CommonBuffer_GetAuCnt(struct interbuf_writer *buf)
{ assert((ps & 15) >= 3); return buf->status.write_index - buf->status.read_index; }
int CommonBuffer_GetMaxSize(struct interbuf_writer *buf)
{ assert((ps & 15) >= 3); return (int)buf->max_size; }
int CommonBuffer_GetMaxAu(struct interbuf_writer *buf)
{ assert((ps & 15) >= 3); return (int)buf->aui_count; }
int CommonBuffer_GetMaxAuLength(struct interbuf_writer *buf)
{ assert((ps & 15) >= 3); return (int)buf->max_access_size; }

uint32_t irq_save(void)
{
    uint32_t saved = ps;
    if ((ps & 15) < 3) ps = (ps & ~15u) | 3;
    return saved;
}
void irq_restore(uint32_t saved) { ps = saved; }

int CommonBuffer_CreateNewAu(struct output_au_info *info, struct interbuf_writer *buf)
{
    assert((ps & 15) >= 3);
    if (create_error) return create_error;
    assert(!buf->status.index_allocated);
    current_info = *info;
    buf->status.index_allocated = 1;
    return 0;
}
static int copy_data(unsigned char *src, int length, struct interbuf_writer *buf)
{
    assert(ps == initial_ps && buf->status.index_allocated);
    if (copy_error) return copy_error;
    assert(length >= 0 && (unsigned)length <= sizeof(storage) - payload_bytes);
    memcpy(storage + payload_bytes, src, (size_t)length);
    payload_bytes += (unsigned)length;
    buf->status.write_offset += length;
    return 0;
}
int CommonBuffer_WriteData(unsigned char *src, int length, struct interbuf_writer *buf)
{ return copy_data(src, length, buf); }
int CommonBuffer_WriteDataUser(unsigned char *src, int length, struct interbuf_writer *buf)
{
    if (length && src[0] == 0xff) return -31;
    return copy_data(src, length, buf);
}
int CommonBuffer_FinishAu(struct interbuf_writer *buf)
{
    assert((ps & 15) >= 3 && buf->status.index_allocated);
    if (finish_error) return finish_error;
    buf->status.index_allocated = 0;
    ++buf->status.write_index;
    committed = payload_bytes;
    return 0;
}
int CommonBuffer_UpdateAu(struct output_au_info *info, struct interbuf_writer *buf)
{
    assert((ps & 15) >= 3 && buf->status.index_allocated);
    current_info = *info;
    return -41;
}
void ashbuf_notify_write(void *aimc, struct interbuf_writer *buf, unsigned cowork)
{
    assert(ps == initial_ps && aimc == storage && cowork == 0x1234);
    assert(finish_error || (committed == payload_bytes && !buf->status.index_allocated));
    ++notices;
}
// @extract src/dsp1/output_buffer.c writer_require
// @extract src/dsp1/output_buffer.c InterBufWriter_UpdateStatus
// @extract src/dsp1/output_buffer.c InterBufWriter_CreateNewAu InterBufWriter_WriteData InterBufWriter_WriteDataUser InterBufWriter_FinishAu writer_insert InterBufWriter_InsertAu InterBufWriter_InsertAuUser InterBufWriter_UpdateAu
// @extract src/dsp1/output_buffer.c InterBufWriter_Init InterBufWriter_Ref InterBufWriter_Unref InterBufWriter_GetFreeSize InterBufWriter_GetPassSize InterBufWriter_GetUsedSize InterBufWriter_GetAuCnt InterBufWriter_GetMaxSize InterBufWriter_GetMaxAu InterBufWriter_GetMaxAuLength
// @extract src/dsp1/output_buffer.c InterBufWriter_GetAuInfo InterBufWriter_GetCurrentInfo InterBufWriter_ReadData InterBufWriter_FlushData InterBufWriter_ReadAu InterBufWriter_SetRepeat InterBufWriter_RequestData InterBufWriter_GetStructSize _InterBufWriter_Create InterBufWriter_Create InterBufWriter_CreateFromCommonBuffer InterBufWriter_CreateFromCommonBufferUser

static struct interbuf_writer reset(unsigned level)
{
    initial_ps = ps = 0x120 | level;
    notices = committed = payload_bytes = 0;
    create_error = copy_error = finish_error = 0;
    init_error = 0;
    memset(storage, 0xa5, sizeof(storage));
    return (struct interbuf_writer){ .aimc = storage, .cowork_module = 0x1234 };
}

static void insert_paths(void)
{
    unsigned char payload[] = { 0x72, 0xf8, 0x1f, 0x4e, 5, 6, 7, 8 };
    struct output_au_info info = { .size = sizeof(payload), .timestamp = UINT32_MAX,
        .timestamp64 = UINT64_C(0x123456789abc), .index = -1, .flags = 0x18,
        .acmod = 7, .data_type = 1 };
    for (unsigned level = 0; level <= 5; ++level) {
        struct interbuf_writer buf = reset(level);
        assert(InterBufWriter_InsertAu(payload, sizeof(payload), &info, &buf) == 0);
        assert(ps == initial_ps && committed == sizeof(payload) && notices == 1);
        assert(!memcmp(storage, payload, sizeof(payload)) && buf.status.write_index == 1);
        assert(current_info.timestamp64 == info.timestamp64 && current_info.index == -1);
        assert(current_info.flags == 0x18 && current_info.data_type == 1);
    }
    struct interbuf_writer buf = reset(0);
    create_error = -11;
    assert(InterBufWriter_InsertAu(payload, 8, &info, &buf) == -11);
    assert(!payload_bytes && !notices && !buf.status.index_allocated && ps == initial_ps);
    buf = reset(0); copy_error = -21;
    assert(InterBufWriter_InsertAu(payload, 8, &info, &buf) == -21);
    assert(!committed && !notices && !buf.status.index_allocated && ps == initial_ps);
    buf = reset(0); finish_error = -22;
    assert(InterBufWriter_InsertAu(payload, 8, &info, &buf) == -22);
    assert(payload_bytes == 8 && !committed && notices == 1 && buf.status.index_allocated);
    assert(ps == initial_ps && !buf.status.write_index);
    buf = reset(1); info.data_type = 4;
    assert(InterBufWriter_InsertAuUser(payload, 8, &info, &buf) == 0);
    assert(committed == 8 && current_info.data_type == 4 && ps == initial_ps);
    buf = reset(1); payload[0] = 0xff;
    assert(InterBufWriter_InsertAuUser(payload, 8, &info, &buf) == -31);
    assert(!payload_bytes && !notices && !buf.status.index_allocated);
}

static void incremental_write(void)
{
    struct interbuf_writer buf = reset(1);
    struct output_au_info info = { .data_type = 5, .timestamp = 42 };
    unsigned char payload[] = { 1, 2, 3, 4 };
    assert(InterBufWriter_CreateNewAu(&info, &buf) == 0 && !notices);
    assert(InterBufWriter_WriteData(payload, 2, &buf) == 0);
    assert(InterBufWriter_WriteDataUser(payload + 2, 2, &buf) == 0 && !committed);
    info.timestamp = 99;
    assert(InterBufWriter_UpdateAu(&info, &buf) == -41 && ps == initial_ps);
    assert(current_info.timestamp == 99 && current_info.data_type == 5 && !notices);
    assert(InterBufWriter_FinishAu(&buf) == 0);
    assert(committed == 4 && notices == 1 && !memcmp(storage, payload, 4));
}

static void construction_and_permissions(void)
{
    struct processor_adapter adapter = { 17 };
    struct output_common_buffer common = { 4096, 256, 8 };
    struct interbuf_writer buf = reset(0);
    memset(&buf, 0xa5, sizeof(buf));
    assert(InterBufWriter_Create(&adapter, &common, 8192, 256, 8, storage,
                                0x1234, storage, &buf) == 0);
    assert(buf.max_size == 8192 - 332 - 256 && init_capacity == buf.max_size);
    assert(buf.common == &common && buf.adapter == &adapter && buf.data == storage);
    assert(buf.aimc == storage && buf.cowork_module == 0x1234 && !buf.request_mem);
    assert(!buf.offset && buf.ref_count == 1 && buf.ref_lock_ps == UINT32_MAX);
    assert(buf.status.read_index == -1 && buf.status.write_index == -1);
    assert(ps == initial_ps && buf.lock_ps == initial_ps && !notices);
    unsigned char payload[] = { 0xff, 2, 3, 4 };
    struct output_au_info info = { .data_type = 1 };
    assert(buf.insert_au(payload, 4, &info, &buf) == 0 && committed == 4);
    assert(buf.get_used_size(&buf) == 4 && buf.get_free_size(&buf) == (int)buf.max_size - 4);
    assert(buf.get_au_cnt(&buf) == 1 && buf.get_pass_size(&buf) == 0);
    assert(buf.get_max_size(&buf) == (int)buf.max_size);
    assert(buf.get_max_au(&buf) == 8 && buf.get_max_au_length(&buf) == 256);
    assert(ps == initial_ps && buf.ref(&buf) == 2 && buf.unref(&buf) == 1);
    struct output_buffer_status before = buf.status;
    int offset = 17, size = 23, count = 29;
    unsigned char *dest = payload;
    assert(buf.get_au_info(0, &offset, &info, &buf) == -5);
    assert(buf.get_current_info(&offset, &info, &buf) == -5);
    assert(buf.read_data(&dest, &size, &count, &buf) == -5);
    assert(buf.read_au(&dest, &size, &count, &buf) == -5);
    assert(buf.flush_data(4, &buf) == -5 && buf.request_data(&buf) == -5);
    assert(buf.set_repeat(2, &buf) == -1 && !memcmp(&before, &buf.status, sizeof(before)));
    assert(dest == payload && offset == 17 && size == 23 && count == 29);
    reset(2);
    assert(InterBufWriter_CreateFromCommonBufferUser(&adapter, &common, storage,
                                                    0x1234, storage, &buf) == 0);
    assert(init_capacity == 0u - 332 - 256 && buf.max_size == 4096);
    assert(buf.insert_au(payload, 4, &info, &buf) == -31);
    assert(!buf.status.index_allocated && !notices && ps == initial_ps);
    assert(buf.create_new_au(&info, &buf) == 0);
    assert(buf.write_data(payload, 4, &buf) == -31);
    payload[0] = 1;
    assert(buf.write_data(payload, 4, &buf) == 0 && buf.finish_au(&buf) == 0);
    reset(5);
    assert(InterBufWriter_CreateFromCommonBuffer(&adapter, &common, storage,
                                                0x1234, storage, &buf) == 0);
    assert(buf.max_size == 4096 && ps == initial_ps);
    assert(buf.insert_au(payload, 4, &info, &buf) == 0 && committed == 4);
    reset(0); init_error = -51;
    assert(InterBufWriter_Create(&adapter, &common, 8192, 256, 8, storage,
                                0x1234, storage, &buf) == 0);
    assert(buf.init(&buf) == -51 && ps == initial_ps);
}

int main(void)
{
    struct interbuf_writer buf = {
        .status = { 1, 10, 20, 3, 4, 50, 60, 70 }
    };
    struct output_buffer_status remote = { 99, 100, 200, -1, 40, 500, 600, 700 };
    assert(InterBufWriter_UpdateStatus(&buf, &remote) == 0);
    assert(buf.status.read_index == -1 && buf.status.read_offset == 100);
    assert(buf.status.index_allocated == 1 && buf.status.write_offset == 20);
    assert(buf.status.write_index == 4 && buf.status.written == 50);
    assert(buf.status.passed == 60 && buf.status.repeat_count == 70);
    insert_paths();
    incremental_write();
    construction_and_permissions();
    puts("DSP1 writer construction, ownership, AU failures, IRQ scope and permissions pass");
    return 0;
}