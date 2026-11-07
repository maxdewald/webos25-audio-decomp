#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @match src/dsp2/imc.c #define CMD_SHBUF_STATUS[^\n]*
// @match src/dsp2/imc.c typedef int \(\*imc_fn\)[^\n]*;
// @match src/dsp2/imc.c typedef void \(\*imc_response_fn\)[^\n]*;
// @match src/dsp2/imc.c typedef int \(\*imc_notification_fn\)[^\n]*;
// @match src/dsp2/imc.c typedef int \(\*imc_named_fn\)[^\n]*;
// @match src/dsp2/imc.c (?s)struct imc_response_link \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc_notification \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc_named_event \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc_event_action \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc_event_entry \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc_event_table \{.*?\n};
// @match src/dsp2/imc.c (?s)struct imc_event_payload \{.*?\n};
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
static unsigned named_active, named_callbacks;
static int imc_lock;
static struct imc_named_event *live_named;
static struct imc_event_table *tree_table;
static struct imc_event_table *allocated_table;
static unsigned tree_key, table_active;
static int tree_create_error;
static const char *assertion_function = "IMC_Response";
static unsigned lock_reentries;
static jmp_buf locked_callback;

static void __assert_failed(const char *fn, int line, const char *expr)
{
    assert(assertion_expected && !strcmp(fn, assertion_function));
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
    if (imc_lock) {
        assert(!irq_depth && named_active);
        --named_active;
    } else if (ptr == allocated_table) {
        assert(!irq_depth && table_active);
        --table_active;
        tree_table = NULL;
        allocated_table = NULL;
    } else if (ptr == owned_payload) {
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
        assert((irq_depth == 1 && (size == sizeof(struct imc_pending_response) ||
                       size == sizeof(struct imc_notification))) ||
            (imc_lock && !irq_depth && size == sizeof(struct imc_named_event)) ||
            (!irq_depth && size == sizeof(struct imc_event_table)));
    if (fail_allocation)
        return NULL;
    void *ptr = malloc(size);
    assert(ptr);
    if (imc_lock)
        ++named_active;
    else if (size == sizeof(struct imc_event_table)) {
        assert(!allocated_table);
        allocated_table = ptr;
        ++table_active;
    } else
        ++active;
    return ptr;
}

static void AuiLogger_Init(void *ptr, int byte, unsigned size) { memset(ptr, byte, size); }
static void aud_mutex_lock(void *mutex)
{
    int *locked = mutex == aimc_handler_lock ? &handler_lock : &imc_lock;
    if (locked == &imc_lock) {
        if (*locked)
            ++lock_reentries;
        ++*locked;
    } else {
        assert(!*locked);
        *locked = 1;
    }
}

static void aud_mutex_unlock(void *mutex)
{
    int *locked = mutex == aimc_handler_lock ? &handler_lock : &imc_lock;
    assert(*locked);
    --*locked;
}

static void BufMonitor_AddBufferRef(void *dst, const char *src, unsigned length)
{
    strncpy(dst, src, length);
}
static int aud_who_am_i(void) { return 2; }
static int ConnectionMng_ShBufCmd(int uid, const int *param) { return -1; }

static void logmm_printl(void *ctx, int level, const char *fn, int line,
                         const char *format, ...)
{
    ++logs;
}

static int RBTree_Search(void *tree, unsigned key, struct imc_event_table **out)
{
    assert(tree == &tree_table);
    *out = key == tree_key ? tree_table : NULL;
    return *out ? 0 : 0x13;
}

static int RBTree_CreateNode(void *tree, unsigned key, struct imc_event_table *table)
{
    assert(tree == &tree_table && !tree_table && table_active == 1);
    tree_key = key;
    if (tree_create_error)
        return tree_create_error;
    tree_table = table;
    return 0;
}

static int RBTree_Delete(void *tree, unsigned key)
{
    assert(tree == &tree_table && key == tree_key && tree_table && !tree_table->count);
    aud_free(tree_table);
    return 0;
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
// @extract src/dsp2/imc.c _IMC_FindAndClearNotification
// @extract src/dsp2/imc.c _IMC_InsertActionToList _IMC_DeleteActionFromList IMC_RegisterEvent IMC_CancelEvent IMC_RaiseEvent
// @extract src/dsp2/imc.c add_named_event remove_named_event IMC_RegisterNamedEvent IMC_UnregisterNamedEvent IMC_UnregisterNamedEvent_no_ipc IMC_UnregisterNamedEvent_onUID IMC_RaiseNamedEvent
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

static int notification_reentry;

static int notify(void *payload, unsigned length, void *arg)
{
    assert(!irq_depth && arg == &callback_arg);
    assert(payload == expected_payload && length == expected_length);
    assert(*(unsigned *)payload == 0x12345678);
    assert(active == expected_active);
    ++callbacks;
    if (!notification_reentry++)
        assert(IMC_ProcessHeader(current, current_header) == (expected_active ? 0 : 0x13));
    return -99;
}

static void test_notification(void)
{
    struct imc imc = {0};
    imc.actions.prev = imc.actions.next = &imc.actions;
    unsigned header[9] = {0xdeadbeef, 71, 0, 5, 0, UINT32_MAX, 0, 4, 0x12345678};
    current = &imc;
    current_header = header;
    expected_payload = header + 8;
    expected_length = 4;
    assert(IMC_ProcessHeader(&imc, header) == 0x13);
    for (int repeat = 0; repeat <= 1; ++repeat) {
        struct imc_notification *notification = calloc(1, sizeof(*notification));
        assert(notification);
        ++active;
        notification->repeat = repeat;
        notification->fn = notify;
        notification->arg = &callback_arg;
        notification->id = 71;
        notification->link.prev = notification->link.next = &imc.actions;
        imc.actions.prev = imc.actions.next = &notification->link;
        expected_active = (unsigned)repeat;
        notification_reentry = 0;
        unsigned before = callbacks;
        assert(IMC_ProcessHeader(&imc, header) == 0);
        assert(callbacks == before + 1 + (unsigned)repeat);
        if (repeat) {
            notification->fn = NULL;
            notification->repeat = 0;
            assert(IMC_ProcessHeader(&imc, header) == 0);
        }
        assert(!active && imc.actions.next == &imc.actions);
        assert(imc.actions.prev == &imc.actions && header[8] == 0x12345678);
    }
}

static int named_notify(void *arg, void *payload, unsigned length)
{
    assert(imc_lock && !irq_depth && arg == &callback_arg);
    assert(payload == expected_payload && length == expected_length);
    assert(live_named->arg == arg && live_named->fn == named_notify);
    assert(live_named->link.next->prev == &live_named->link);
    ++named_callbacks;
    return -99;
}

static void test_named_events(void)
{
    struct imc imc = {.platform = 4};
    imc.named.prev = imc.named.next = &imc.named;
    unsigned payload = 0x12345678;
    char name[40] = "last";
    expected_payload = &payload;
    expected_length = 4;
    unsigned before = sent_count;
    int id = IMC_RegisterNamedEvent(&imc, name, 0x04000002, 1, named_notify, &callback_arg);
    assert(id == 1 && named_active == 1 && sent_count == before);
    live_named = (void *)imc.named.next;
    memset(name, 'x', sizeof(name));
    assert(!strcmp(live_named->name, "last"));
    assert(IMC_RaiseNamedEvent(&imc, 0x04000003, "last", &payload, 4) == 0);
    assert(IMC_RaiseNamedEvent(&imc, 0x04000002, "LAST", &payload, 4) == 0);
    assert(!named_callbacks && named_active == 1);
    assert(IMC_RaiseNamedEvent(&imc, 0x04000002, "last", &payload, 4) == 0);
    assert(named_callbacks == 1 && !named_active && imc.named.next == &imc.named);
    assert(IMC_RaiseNamedEvent(&imc, 0x04000002, "last", NULL, 0) == 0);

    transport_error = 27;
    id = IMC_RegisterNamedEvent(&imc, "disconnect", 0x01000002, 0, named_notify, &callback_arg);
    assert(id == 2 && named_active == 1 && sent_count == before + 1);
    unsigned registration[8] = {0, 0, 4, 6, 0, 0x01000002, 2, 32};
    assert(!memcmp(sent_header, registration, sizeof(registration)));
    live_named = (void *)imc.named.next;
    assert(sent_payload == live_named->name && !strcmp(sent_payload, "disconnect"));
    unsigned header[9] = {0, 99, 0x01000003, 8, 0, 0, 2, 4, 0x12345678};
    expected_payload = header + 8;
    assert(IMC_ProcessHeader(&imc, header) == 0 && named_callbacks == 1);
    header[2] = 0x01000002;
    header[6] = 99;
    assert(IMC_ProcessHeader(&imc, header) == 0 && named_callbacks == 1);
    header[6] = 2;
    assert(IMC_ProcessHeader(&imc, header) == 0 && named_callbacks == 2 && named_active == 1);
    assert(IMC_ProcessHeader(&imc, header) == 0 && named_callbacks == 3 && named_active == 1);
    IMC_UnregisterNamedEvent(&imc, id);
    unsigned cancellation[8] = {0, 0, 4, 7, 0, 0x01000002, 2, 0};
    assert(!named_active && !memcmp(sent_header, cancellation, sizeof(cancellation)));
    before = sent_count;
    IMC_UnregisterNamedEvent(&imc, id);
    assert(sent_count == before);
    assert(IMC_ProcessHeader(&imc, header) == 0 && named_callbacks == 3);

    unsigned add[16] = {0, 0, 1, 6, 1, 0x04000002, 77, 32};
    memcpy(add + 8, "last", 5);
    assert(IMC_ProcessHeader(&imc, add) == 0 && named_active == 1);
    memset(add + 8, 'x', 32);
    assert(IMC_RaiseNamedEvent(&imc, 0x04000002, "last", &payload, 4) == 0);
    unsigned delivery[8] = {0, 0, 0x04000002, 8, 0, 0x01000000, 77, 4};
    assert(!named_active && sent_payload == &payload && sent_to == 1);
    assert(!memcmp(sent_header, delivery, sizeof(delivery)));
    assert(sent_count == before + 1);
    assert(IMC_ProcessHeader(&imc, add) == 0 && named_active == 1);
    assert(((struct imc_named_event *)imc.named.next)->name[31] == 0);
    unsigned remove[8] = {0, 0, 2, 7, 0, 0, 77, 0};
    assert(IMC_ProcessHeader(&imc, remove) == 1 && named_active == 1);
    remove[2] = 1;
    assert(IMC_ProcessHeader(&imc, remove) == 0 && !named_active);
    assert(IMC_ProcessHeader(&imc, remove) == 1);
    fail_allocation = 1;
    assert(IMC_ProcessHeader(&imc, add) == 0 && !named_active);
    assert(IMC_RegisterNamedEvent(&imc, "last", 0x04000002, 0, named_notify, NULL) == -1);
    assert(imc.named_id == 3);
    fail_allocation = 0;
    imc.named_id = 0x7fffffff;
    assert(IMC_RegisterNamedEvent(&imc, "last", 0x01000002, 0, named_notify, NULL) == 1);
    assert(IMC_RegisterNamedEvent(&imc, "last", 0x01000002, 0, named_notify, NULL) == 2);
    assert(IMC_RegisterNamedEvent(&imc, "last", 0x04000002, 0, named_notify, NULL) == 3);
    before = sent_count;
    IMC_UnregisterNamedEvent_onUID(&imc, 0x01000002);
    assert(named_active == 1 && sent_count == before);
    IMC_UnregisterNamedEvent_no_ipc(&imc, 3);
    assert(!named_active && sent_count == before);
    id = IMC_RegisterNamedEvent(&imc, "last", 0x01000002, 1, named_notify, &callback_arg);
    live_named = (void *)imc.named.next;
    header[6] = (unsigned)id;
    expected_payload = header + 8;
    assert(IMC_ProcessHeader(&imc, header) == 0 && !named_active && named_callbacks == 4);
    assert(IMC_ProcessHeader(&imc, header) == 0 && named_callbacks == 4);
    assert(imc.named.prev == &imc.named && imc.named.next == &imc.named);
}

static unsigned numeric_callbacks, numeric_id;
static int numeric_mode;
static const unsigned numeric_uid = 0x04000002, numeric_event = 0xca04;
static struct imc numeric_imc;

static int numeric_notify(void *payload, unsigned length, void *arg)
{
    assert(!irq_depth && !imc_lock && arg == &callback_arg);
    assert(payload == expected_payload && length == expected_length);
    assert(*(unsigned *)payload == 0x12345678);
    ++numeric_callbacks;
    if (numeric_mode == 1) {
        numeric_mode = 0;
        assert(IMC_ProcessHeader(&numeric_imc, current_header) == 0x13);
    } else if (numeric_mode == 2) {
        numeric_mode = 0;
        assert(IMC_CancelEvent(&numeric_imc, numeric_event, numeric_uid, numeric_id) == 0);
        assert(!active && !table_active);
        struct imc_event_action replacement = {.type = 1, .repeat = 0,
            .target = numeric_uid, .fn = numeric_notify, .arg = &callback_arg, .interval = 1};
        assert(IMC_RegisterEvent(&numeric_imc, numeric_event, numeric_uid,
                                 &numeric_id, &replacement) == 0);
    }
    return -99;
}

static void test_numeric_events(void)
{
    struct imc *imc = &numeric_imc;
    imc->platform = 4;
    imc->rbtree = &tree_table;
    imc->actions.prev = imc->actions.next = &imc->actions;
    unsigned payload = 0x12345678;
    struct imc_event_payload param = {.param = &payload, .len = 4};
    struct imc_event_action action = {.type = 1, .repeat = 1, .target = numeric_uid,
        .fn = numeric_notify, .arg = &callback_arg, .interval = 3};
    expected_payload = &payload;
    expected_length = 4;
    assert(IMC_RaiseEvent(imc, numeric_event, numeric_uid, &param) == 0x13);
    assert(IMC_RegisterEvent(imc, numeric_event, numeric_uid, &numeric_id, &action) == 0);
    assert(numeric_id == 0x400000 && active == 1 && table_active == 1);
    assert(tree_key == 0xca0402 && tree_table->count == 1);
    memset(&action, 0, sizeof(action));
    for (unsigned raised = 0; raised < 7; ++raised) {
        assert(IMC_RaiseEvent(imc, numeric_event, numeric_uid, &param) == 0);
        assert(numeric_callbacks == raised / 3 + 1);
    }
    unsigned second, third;
    action = (struct imc_event_action){.type = 1, .repeat = 1, .target = numeric_uid,
        .fn = numeric_notify, .arg = &callback_arg, .interval = 1001};
    imc->action_id = 0;
    assert(IMC_RegisterEvent(imc, numeric_event, numeric_uid, &second, &action) == 0);
    assert(second == 0x400001 && tree_table->entries[1].action.interval == 1);
    action.interval = -1;
    assert(IMC_RegisterEvent(imc, numeric_event, numeric_uid, &third, &action) == 0);
    assert(third == 0x400002 && active == 3 && tree_table->count == 3);
    assert(IMC_CancelEvent(imc, numeric_event, numeric_uid, second) == 0);
    assert(active == 2 && tree_table->count == 2 && tree_table->entries[1].id == third);
    assert(IMC_CancelEvent(imc, numeric_event, numeric_uid, third) == 0);
    assert(IMC_CancelEvent(imc, numeric_event, numeric_uid, numeric_id) == 0);
    assert(!active && !table_active && !tree_table);
    assert(IMC_CancelEvent(imc, numeric_event, numeric_uid, numeric_id) == 0x13);

    action.repeat = 0;
    assert(IMC_RegisterEvent(imc, numeric_event, numeric_uid, &numeric_id, &action) == 0);
    unsigned header[9] = {5, numeric_id, numeric_uid, 5, 5, numeric_uid, 0, 4, 0x12345678};
    current_header = header;
    expected_payload = header + 8;
    numeric_mode = 1;
    assert(IMC_ProcessHeader(imc, header) == 0 && !active && table_active == 1);
    assert(IMC_CancelEvent(imc, numeric_event, numeric_uid, numeric_id) == 0 && !table_active);
    action.repeat = 1;
    assert(IMC_RegisterEvent(imc, numeric_event, numeric_uid, &numeric_id, &action) == 0);
    header[1] = numeric_id;
    numeric_mode = 2;
    assert(IMC_ProcessHeader(imc, header) == 0 && active == 1 && table_active == 1);
    assert(IMC_ProcessHeader(imc, header) == 0x13);
    header[1] = numeric_id;
    assert(IMC_ProcessHeader(imc, header) == 0 && !active);
    assert(IMC_CancelEvent(imc, numeric_event, numeric_uid, numeric_id) == 0 && !table_active);

    action.repeat = 0;
    expected_payload = &payload;
    unsigned before = numeric_callbacks;
    assert(IMC_RegisterEvent(imc, numeric_event, numeric_uid, &numeric_id, &action) == 0);
    assert(IMC_RaiseEvent(imc, numeric_event, numeric_uid, &param) == 0);
    assert(numeric_callbacks == before + 1 && !active && !table_active);
    action.target = 0x01000001;
    transport_error = 27;
    assert(IMC_RegisterEvent(imc, numeric_event, numeric_uid, &numeric_id, &action) == 0);
    assert(IMC_RaiseEvent(imc, numeric_event, numeric_uid, &param) == 0 && !table_active && active == 1);
    unsigned notification[8] = {5, numeric_id, numeric_uid, 5, 5, action.target, 0, 4};
    assert(!memcmp(sent_header, notification, sizeof(notification)) && sent_payload == &payload);
    assert(IMC_CancelEvent(imc, numeric_event, numeric_uid, numeric_id) == 0x13 && !active);

    action.repeat = 1;
    action.target = numeric_uid;
    assert(IMC_RegisterEvent(imc, numeric_event, 0x01000002, &numeric_id, &action) == -27);
    assert(active == 1 && !table_active);
    unsigned registration[8] = {0, numeric_id, numeric_uid, 3, 6, (unsigned)-1, 0xca0402, 0};
    assert(!memcmp(sent_header, registration, sizeof(registration)) && !sent_payload);
    assert(IMC_CancelEvent(imc, numeric_event, 0x01000002, numeric_id) == -27 && !active);
    assert(sent_header[3] == 4 && sent_header[1] == numeric_id && sent_header[6] == 0xca0402);

    fail_allocation = 1;
    assertion_expected = 1;
    assertion_function = "IMC_RegisterEvent";
    if (!setjmp(assertion)) {
        IMC_RegisterEvent(imc, numeric_event, numeric_uid, &numeric_id, &action);
        assert(0);
    }
    assert(assertion_line == 0x3d7 && !active && !irq_depth);
    assertion_expected = 0;
    fail_allocation = 0;
    action.type = 0;
    assert(IMC_RegisterEvent(imc, numeric_event, numeric_uid, &numeric_id, &action) == 0xf);
    unsigned add[8] = {0, 77, 0x01000001, 3, 2, 1, 0xca0402, 0};
    fail_allocation = 1;
    assert(IMC_ProcessHeader(imc, add) == 0x10 && !table_active);
    fail_allocation = 0;
    tree_create_error = 0x10;
    assert(IMC_ProcessHeader(imc, add) == 0x10 && !table_active && !tree_table);
    tree_create_error = 0;
    assert(IMC_ProcessHeader(imc, add) == 0 && table_active == 1);
    before = sent_count;
    assert(IMC_RaiseEvent(imc, numeric_event, numeric_uid, &param) == 0);
    assert(sent_count == before + 1 && sent_header[1] == 77 && !table_active);
    add[4] = 0;
    assert(IMC_ProcessHeader(imc, add) == 0xf && !table_active);
    add[4] = 6;
    assert(IMC_ProcessHeader(imc, add) == 0 && table_active == 1);
    unsigned cancel[8] = {0, 88, 0, 4, 0, 0, 0xca0402, 0};
    assert(IMC_ProcessHeader(imc, cancel) == 0 && tree_table->count == 1);
    cancel[1] = 77;
    assert(IMC_ProcessHeader(imc, cancel) == 0 && !table_active);
    assert(IMC_ProcessHeader(imc, cancel) == 0x13);
}

static struct imc locked_imc;
static int locked_id;
static unsigned nested_deliveries;

static int named_nested(void *arg, void *payload, unsigned length)
{
    assert(imc_lock == (nested_deliveries ? 2 : 1) && named_active == 1);
    if (!nested_deliveries++)
        assert(IMC_RaiseNamedEvent(&locked_imc, 0x04000002, "last", NULL, 0) == 0);
    return 0;
}

static int named_self_unregister(void *arg, void *payload, unsigned length)
{
    assert(imc_lock && named_active == 1);
    IMC_UnregisterNamedEvent(&locked_imc, locked_id);
    assert(imc_lock == 1 && !named_active);
    assert(locked_imc.named.next == &locked_imc.named);
    longjmp(locked_callback, 1);
}

static void test_named_reentry_constraint(void)
{
    locked_imc.platform = 4;
    locked_imc.named.prev = locked_imc.named.next = &locked_imc.named;
    locked_id = IMC_RegisterNamedEvent(&locked_imc, "last", 0x04000002, 0, named_nested, NULL);
    assert(IMC_RaiseNamedEvent(&locked_imc, 0x04000002, "last", NULL, 0) == 0);
    assert(nested_deliveries == 2 && named_active == 1 && !imc_lock);
    IMC_UnregisterNamedEvent(&locked_imc, locked_id);
    locked_id = IMC_RegisterNamedEvent(&locked_imc, "last", 0x04000002, 1,
                                      named_self_unregister, NULL);
    if (!setjmp(locked_callback)) {
        IMC_RaiseNamedEvent(&locked_imc, 0x04000002, "last", NULL, 0);
        assert(0);
    }
    assert(lock_reentries == 2 && imc_lock == 1 && !named_active);
    aud_mutex_unlock(locked_imc.lock);
    assert(!named_active && !imc_lock);
}

int main(void)
{
    test_dispatch();
    test_command_response();
    test_response_sender();
    test_notification();
    test_named_events();
    test_numeric_events();
    test_named_reentry_constraint();
    assert(!named_active && !imc_lock);
    assert(saves == restores && !irq_depth && !active && !owned_payload);
    puts("PASS IMC: response ownership; numeric registration/gating/reentry/cancel; named lifecycle, recursive lock and self-removal boundary");
    return 0;
}