#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @match src/dsp2/imc.c #define CMD_SHBUF_STATUS[^\n]*
// @match src/dsp2/imc.c typedef int \(\*imc_fn\)[^\n]*;
// @match src/dsp2/imc.c typedef void \(\*imc_response_fn\)[^\n]*;
// @match src/dsp2/imc.c (?s)struct imc_response_link \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc_pending_response \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc_handler \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc \{.*?\n};
// @match src/dsp2/imc.c (?s)struct aimc_state \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc_cmd \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc_response \{.*?\n};
// @match src/dsp2/imc.c (?s)#define ALOG\(.*?while \(0\)

static unsigned mask;
static volatile unsigned *imc_log_mask = &mask;
static void *imc_log_ctx;
static volatile unsigned *aimc_log_mask = &mask;
static void *aimc_log_ctx;
static unsigned imc_seq_mask = 0xffffff;
static struct aimc_state state;
static struct aimc_state *aimc = &state;
static int handler_lock;
static int *aimc_handler_lock = &handler_lock;
static unsigned irq_depth, saves, restores, active, callbacks;
static unsigned logs, command_calls, expected_active;
static struct imc *current;
static const unsigned *current_header;
static void *expected_payload;
static unsigned expected_length;
static int expected_result;
static int callback_arg;
static int fail_allocation, transport_error, no_handle, phase;
static int expected_free_phase = 2;
static unsigned sent_header[8], sent_count;
static void *sent_payload, *owned_payload;
static int sent_to;
static jmp_buf assertion;
static int assertion_expected, assertion_line;

static void __assert_failed(const char *fn, int line, const char *expr)
{
    assert(assertion_expected && !strcmp(fn, "IMC_Response"));
    assertion_line = line;
    longjmp(assertion, 1);
}

static unsigned irq_raise_level(void)
{
    ++saves;
    return irq_depth++;
}

static void irq_restore(unsigned ps)
{
    assert(irq_depth == ps + 1);
    irq_depth = ps;
    ++restores;
}

static void aud_free(void *ptr)
{
    if (ptr == owned_payload) {
        assert(!irq_depth && phase == expected_free_phase);
        phase = 3;
        owned_payload = NULL;
    } else {
        assert(irq_depth == 1 && active);
        --active;
    }
    free(ptr);
}

static void *aud_malloc(unsigned size)
{
    assert(irq_depth == 1 && size == sizeof(struct imc_pending_response));
    if (fail_allocation)
        return NULL;
    void *ptr = malloc(size);
    assert(ptr);
    ++active;
    return ptr;
}

static void AuiLogger_Init(void *ptr, int byte, unsigned size) { memset(ptr, byte, size); }
static void aud_mutex_lock(void *mutex) { assert(!handler_lock); handler_lock = 1; }
static void aud_mutex_unlock(void *mutex) { assert(handler_lock); handler_lock = 0; }
static int aud_who_am_i(void) { return 2; }
static int ConnectionMng_ShBufCmd(int uid, const int *param) { return -1; }

static void logmm_printl(void *ctx, int level, const char *fn, int line,
                         const char *format, ...)
{
    ++logs;
}

static int ModuleList_FindModule(unsigned uid) { return (int)(uid >> 24); }
static void *aimc_get_handle(int me, int from, int reply)
{
    assert(me == 2 && reply == 1);
    sent_to = from;
    return no_handle ? NULL : &sent_count;
}

static int IPC_WriteCommand(void *handle, void *header, void *payload, int irq)
{
    assert(handle == &sent_count && irq == 1 && !irq_depth);
    memcpy(sent_header, header, sizeof(sent_header));
    sent_payload = payload;
    ++sent_count;
    if (owned_payload) {
        assert(payload == owned_payload && phase == 1);
        assert(*(unsigned *)payload == 0xaabbccdd);
        phase = 2;
    }
    return transport_error;
}

// @declare src/dsp2/imc.c aimc_dispatch_command IMC_Response
// @extract src/dsp2/imc.c find_handler aimc_response_param aimc_ipc_send
// @extract src/dsp2/imc.c _IMC_FindAndClearResponse IMC_Response IMC_ProcessHeader IMC_SendCommand aimc_dispatch_command

static void append(struct imc *imc, unsigned seq, imc_response_fn fn)
{
    struct imc_pending_response *pending = calloc(1, sizeof(*pending));
    assert(pending);
    ++active;
    pending->fn = fn;
    pending->arg = &callback_arg;
    pending->seq = seq;
    pending->link.prev = &imc->responses;
    pending->link.next = imc->responses.next;
    imc->responses.next->prev = &pending->link;
    imc->responses.next = &pending->link;
}

static void complete(int result, void *payload, unsigned length, void *arg)
{
    assert(irq_depth == 0 && arg == &callback_arg);
    assert(result == expected_result && length == expected_length);
    assert(payload == expected_payload);
    assert(active == expected_active);
    ++callbacks;
    assert(IMC_ProcessHeader(current, current_header) == 0x13);
}

static void test_dispatch(void)
{
    struct imc imc = {0};
    imc.responses.prev = imc.responses.next = &imc.responses;
    unsigned header[9] = {0x10180004, 22, 0x04000002, 2, (unsigned)-37,
                          0x01000001, 0, 4, 0x12345678};
    current = &imc;
    current_header = header;
    expected_payload = header + 8;
    expected_length = 4;
    expected_result = -37;
    expected_active = 2;
    assert(IMC_ProcessHeader(&imc, header) == 0x13);
    append(&imc, 11, NULL);
    append(&imc, 22, complete);
    append(&imc, 33, NULL);
    assert(IMC_ProcessHeader(&imc, header) == 0);
    assert(callbacks == 1 && active == 2);
    assert(header[8] == 0x12345678);
    assert(IMC_ProcessHeader(&imc, header) == 0x13);
    header[1] = 11;
    assert(IMC_ProcessHeader(&imc, header) == 0 && active == 1);
    header[1] = 33;
    header[7] = UINT32_MAX;
    assert(IMC_ProcessHeader(&imc, header) == 0 && active == 0);
    append(&imc, 44, complete);
    expected_active = 0;
    expected_length = UINT32_MAX;
    header[1] = 44;
    header[0] = 0xdeadbeef;
    header[2] = 0;
    header[5] = UINT32_MAX;
    assert(IMC_ProcessHeader(&imc, header) == 0 && callbacks == 2);
    assert(imc.responses.next == &imc.responses && imc.responses.prev == &imc.responses);
    mask = 1;
    logs = 0;
    assert(IMC_ProcessHeader(&imc, header) == 0x13 && logs == 2);
    mask = 0;
    header[3] = 1;
    assert(IMC_ProcessHeader(&imc, header) == 1);
    header[3] = 99;
    assert(IMC_ProcessHeader(&imc, header) == 0xf);
}

static int command(void *ctx, unsigned cmd, void *param, void *length, int unused)
{
    assert(!irq_depth && !handler_lock && cmd == 0x10180004);
    assert((intptr_t)length == 4 && *(unsigned *)param == 42);
    ++command_calls;
    owned_payload = malloc(4);
    assert(owned_payload);
    *(unsigned *)owned_payload = 0xaabbccdd;
    aimc_response_param(owned_payload, 4);
    phase = 1;
    return -37;
}

static void local_complete(int result, void *payload, unsigned length, void *arg)
{
    assert(result == -37 && length == 4 && arg == &callback_arg);
    assert(!irq_depth && !active && phase == 1 && payload == owned_payload);
    assert(*(unsigned *)payload == 0xaabbccdd);
    phase = 2;
    ++callbacks;
}

static void test_command_response(void)
{
    struct imc imc = {.platform = 4, .seq = 0x1ffffff};
    imc.responses.prev = imc.responses.next = &imc.responses;
    struct imc_handler handler = {.uid = 0x04000010, .fn = command};
    state.handlers = &handler;
    unsigned value = 42, seq = 0;
    struct imc_cmd cmd = {.cmd = 0x10180004, .target = handler.uid,
        .source = 0x04000002, .want_resp = 1, .cb = local_complete,
        .cb_arg = &callback_arg, .len = 4, .param = &value};
    assert(IMC_SendCommand(&imc, &cmd, &seq) == 0);
    assert(seq == 0xffffff && imc.seq == 0x2000000);
    assert(phase == 3 && !owned_payload && !active);
    assert(!state.resp_param && !state.resp_len);

    cmd.source = 0x01000002;
    transport_error = 27;
    unsigned before = sent_count;
    assert(IMC_SendCommand(&imc, &cmd, &seq) == 0 && seq == 0);
    assert(phase == 3 && !owned_payload && sent_count == before + 1);
    assert(sent_header[4] == (unsigned)-37 && sent_header[7] == 4);
    assert(active == 1);
    unsigned reply[8] = {cmd.cmd, seq, cmd.target, 2, 0, cmd.source, 0, 0};
    struct imc_pending_response *pending = (void *)imc.responses.next;
    pending->fn = NULL;
    assert(IMC_ProcessHeader(&imc, reply) == 0 && !active);

    cmd.source = 0x04000002;
    fail_allocation = 1;
    unsigned before_callbacks = callbacks;
    expected_free_phase = 1;
    assert(IMC_SendCommand(&imc, &cmd, NULL) == 0);
    assert(callbacks == before_callbacks && !active && phase == 3);
    assert(!state.resp_param && !state.resp_len);
    fail_allocation = 0;
    cmd.want_resp = 0;
    before = sent_count;
    assert(IMC_SendCommand(&imc, &cmd, NULL) == 0);
    assert(phase == 3 && !active && sent_count == before && callbacks == before_callbacks);
    assert(!state.resp_param && !state.resp_len);
    expected_free_phase = 2;
    state.handlers = NULL;
    assert(command_calls == 4);
}

static void test_response_sender(void)
{
    struct imc imc = {.platform = 4};
    imc.responses.prev = imc.responses.next = &imc.responses;
    unsigned char payload[0xfd7] = {0};
    struct imc_response resp = {.source = 0x01000002, .seq = 71,
        .uid = 0x04000010, .cmd = 0x10180004, .result = -37,
        .param = payload, .len = sizeof(payload)};
    transport_error = 27;
    assert(IMC_Response(&imc, &resp) == -27);
    unsigned expected[8] = {resp.cmd, 71, resp.uid, 2, (unsigned)-37,
                            resp.source, 0, sizeof(payload)};
    assert(!memcmp(expected, sent_header, sizeof(expected)));
    assert(sent_payload == payload && sent_to == 1);
    no_handle = 1;
    unsigned before = sent_count;
    assert(IMC_Response(&imc, &resp) == -1 && sent_count == before);
    no_handle = 0;
    assertion_expected = 1;
    resp.len = 0xfd8;
    if (setjmp(assertion) == 0) {
        IMC_Response(&imc, &resp);
        assert(0);
    }
    assert(assertion_line == 0x392 && sent_count == before);
    resp.len = UINT32_MAX;
    if (setjmp(assertion) == 0) {
        IMC_Response(&imc, &resp);
        assert(0);
    }
    assert(assertion_line == 0x392 && sent_count == before);
    if (setjmp(assertion) == 0) {
        IMC_Response(NULL, &resp);
        assert(0);
    }
    assert(assertion_line == 0x36c);
    assertion_expected = 0;
    resp.source = 0x04000002;
    assert(IMC_Response(&imc, &resp) == 0x13);
    append(&imc, resp.seq, NULL);
    append(&imc, resp.seq, NULL);
    struct imc_pending_response *newest = (void *)imc.responses.next;
    struct imc_response_link *older = newest->link.next;
    assert(IMC_Response(&imc, &resp) == 0 && imc.responses.next == older);
    assert(IMC_Response(&imc, &resp) == 0 && !active);
    mask = 1;
    logs = 0;
    assert(IMC_Response(&imc, &resp) == 0x13 && logs == 2);
    mask = 0;
}

int main(void)
{
    test_dispatch();
    test_command_response();
    test_response_sender();
    assert(saves == restores && !irq_depth && !active && !owned_payload);
    puts("PASS IMC: dispatch, callback lifetime, handler results, ownership and transport/bounds errors");
    return 0;
}