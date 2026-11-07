/* DSP2 RTS, C2 main image linked at 0x60000000, 0x600a5388..0x600a64d8.
 * Scalar control flow checked with xtobjdump.sh, including branch-slot FLIX.
 * Framework object layouts below are host-testable views; comments give DSP offsets.
 * Logging/stringification has no retained effect and is omitted.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct aud_event { uint32_t object[3]; int type; unsigned prop; };
struct aud_data {
    uint32_t object[4];
    int type, index;
    uint32_t reserved[7];
    unsigned size;                      /* +0x34 */
};
struct ts_module;
struct aud_port { int index; struct ts_module *module; };
struct aud_mutex { uint32_t opaque[7]; };
struct aud_cond { uint32_t opaque[4]; };
struct aud_data_adapter {
    uint32_t opaque[4];
    unsigned total, offset;             /* module +0x100/+0x104 */
};
struct rts_event_node {
    struct rts_event_node *prev, *next;
    struct aud_event *event;
};
struct ts_module {
    const char *name;
    struct aud_data_adapter queue;      /* +0xf0 */
    struct aud_mutex lock;              /* +0x10c */
    struct aud_cond ready, space;       /* +0x128/+0x138 */
    unsigned queue_max, packet_count;   /* +0x148/+0x14c */
    unsigned queued_frames, queued_bytes; /* +0x150/+0x154 */
    unsigned buffering_frames, buffering_ms; /* +0x158/+0x15c */
    bool buffering;                     /* +0x160 */
    unsigned operation_mode, state;     /* +0x164/+0x168 */
    unsigned unknown_16c;
    unsigned fast_threshold, slow_threshold; /* +0x170/+0x174, ms */
    unsigned skip_threshold, unknown_17c; /* +0x178/+0x17c */
    unsigned fast_recovery, slow_recovery, skip_recovery; /* +0x180..188 */
    unsigned output_rate[3];            /* +0x18c normal, +0x190 fast, +0x194 slow */
    unsigned unknown_198;
    struct aud_event *first;            /* +0x19c */
    bool first_sent;                    /* +0x1a0 */
    unsigned rate, frame_shift;         /* +0x1a4/+0x1a8 */
    unsigned input_count, output_count, dropped_count; /* +0x1ac..1b4 */
    struct rts_event_node events;       /* +0x1b8/+0x1bc */
    bool flushing, running;             /* +0x1c0/+0x1c1 */
    void *thread;                       /* +0x1c4 */
    unsigned priority;                  /* +0x1c8 */
};

extern void aud_mutex_init(struct aud_mutex *);
extern void aud_mutex_lock(struct aud_mutex *);
extern void aud_mutex_unlock(struct aud_mutex *);
extern void aud_cond_init(struct aud_cond *);
extern void aud_cond_wait(struct aud_cond *, struct aud_mutex *);
extern void aud_cond_signal(struct aud_cond *);
extern void aud_data_adapter_init(struct aud_data_adapter *);
extern void aud_data_adapter_cleanup(struct aud_data_adapter *);
extern void aud_data_adapter_flush(struct aud_data_adapter *, unsigned);
extern void aud_data_adapter_push(struct aud_data_adapter *, struct aud_data *);
extern struct aud_data *aud_data_adapter_dequeue(struct aud_data_adapter *);
extern void aud_object_put(void *);
extern struct aud_event *aud_event_dup(struct aud_event *);
extern int aud_event_value_get_int(struct aud_event *, const char *, int);
extern void aud_event_value_set_int(struct aud_event *, const char *, int);
extern void aud_event_value_set_bool(struct aud_event *, const char *, bool);
extern struct aud_event *aud_event_newv_custom(const char *, int, unsigned, int);
extern int aud_module_push_event(struct ts_module *, int, struct aud_event *);
extern int aud_module_push_data(struct ts_module *, int, struct aud_data *);
extern void aud_module_sync(struct ts_module *, int);
extern int aud_module_default_event(struct ts_module *, struct aud_port *, struct aud_event *);
extern int aud_module_default_command(struct ts_module *, unsigned, const unsigned *, unsigned);
extern void *aud_thread_create(const char *, unsigned, unsigned,
                              int (*)(struct ts_module *), struct ts_module *);
extern void aud_thread_destroy(void *);
extern void ModuleRegistry_Register(const void *);
extern const unsigned char rts_module_registration[]; /* descriptor @0x60106a60 */

/* @0x600a5388: Flush PCM bytes/counters only, not cached FIRST or pending events. */
void ts_queue_flush_locked(struct ts_module *m)
{
    aud_data_adapter_flush(&m->queue, m->queue.total - m->queue.offset);
    m->queued_bytes = m->queued_frames = m->packet_count = 0;
}

/* @0x600a53b0 */
void ts_queue_flush(struct ts_module *m)
{
    aud_mutex_lock(&m->lock);
    ts_queue_flush_locked(m);
    if (m->first) {
        aud_object_put(m->first);
        m->first = NULL;
    }
    aud_mutex_unlock(&m->lock);
}

/* @0x600a53e0: Uses 32-bit wrapped multiplication, and adapter emptiness. */
unsigned ts_get_queued_time(struct ts_module *m)
{
    if (m->queue.total == m->queue.offset)
        return 0;
    return (m->queued_frames * 1000u) / m->rate;
}

/* @0x600a5404: State 0/1/2 sends normal/fast/slow output-rate custom event;
 * state 3 is skipping and sends no rate event. No PCM conversion happens here. */
void ts_set_state(struct ts_module *m, int state)
{
    if ((unsigned)state > 3 || (unsigned)state == m->state)
        return;
    if (state != 3) {
        struct aud_event *e = aud_event_newv_custom("change-out-rate", 0,
                                                   m->output_rate[state], 0);
        if (e) {
            aud_module_push_event(m, 0, e);
            aud_module_sync(m, 0);
        }
    }
    m->state = (unsigned)state;
}

/* rts_push_event_first, inlined at 0x600a56e3..0x600a57cf in ts_thread_work.
 * Copies the cached event, not a PCM rebuild. All unmentioned fields (including
 * codec/es/rate/bits/channels) survive aud_event_dup unchanged. Original remains
 * owned by m. The duplicate changes ONLY front-padding, zero acmod, and dolby.
 * Padding is (ceil(average queued frames per packet / 256) + 2) * 256.
 * Failure leaves first_sent clear and the worker still processes PCM. */
static void rts_push_event_first(struct ts_module *m)
{
    struct aud_event *e = aud_event_dup(m->first);
    if (!e)
        return;
    aud_mutex_lock(&m->lock);
    unsigned frames = m->packet_count ?
        (m->queued_bytes / m->packet_count) >> (m->frame_shift & 31) : 0;
    aud_mutex_unlock(&m->lock);
    int32_t signed_frames = (int32_t)frames;
    int32_t blocks = signed_frames / 256 + ((frames & 255u) != 0);
    aud_event_value_set_int(e, "front-padding", (int)((uint32_t)blocks * 256u + 512u));
    if (aud_event_value_get_int(e, "acmod", 0) == 0)
        aud_event_value_set_int(e, "acmod", 2);
    aud_event_value_set_bool(e, "dolby", false);
    aud_module_push_event(m, 0, e);
    m->first_sent = true;
}

/* Scalar inlined processing at 0x600a5803..0x600a597e, after dequeue.
 * Thresholds/recoveries are milliseconds despite command values dividing by 384.
 * Hybrid runs continuous rate control only while not dropping. */
static bool rts_process(struct ts_module *m)
{
    unsigned queued;
    if (m->operation_mode == 1 || m->operation_mode == 2) {
        queued = ts_get_queued_time(m);
        if (m->state == 3) {
            if (queued >= m->skip_recovery)
                return true;
            ts_set_state(m, 0);
        } else if (queued > m->skip_threshold) {
            ts_set_state(m, 3);
            return true;
        }
        if (m->operation_mode == 1)
            return false;
    } else if (m->operation_mode != 0) {
        return false;
    }
    queued = ts_get_queued_time(m);
    if (m->state == 0) {
        if (queued > m->fast_threshold)
            ts_set_state(m, 1);
        else if (queued < m->slow_threshold)
            ts_set_state(m, 2);
    } else if (m->state == 1) {
        if (queued < m->fast_recovery)
            ts_set_state(m, 0);
    } else if (m->state == 2) {
        if (queued > m->slow_recovery)
            ts_set_state(m, 0);
    }
    return false;
}

/* @0x600a54f4: Events take priority over PCM. PCM is forwarded byte-for-byte;
 * skipping drops whole objects. Empty-queue wake sets the initial buffering gate.
 * Cached FIRST is not in the event list: @0x600a56e3 is reached only after the
 * PCM buffering gate. ES-only input cannot cause FIRST to be forwarded.
 * All worker FLIX slots were scalar branches; no hidden sample/SIMD operation. */
int ts_thread_work(struct ts_module *m)
{
    while (m->running) {
        aud_mutex_lock(&m->lock);
        if (m->queue.total == m->queue.offset && m->events.next == &m->events) {
            aud_cond_wait(&m->ready, &m->lock);
            m->buffering = true;
        }
        if (!m->running) {
            aud_mutex_unlock(&m->lock);
            return 0;
        }
        struct rts_event_node *node = m->events.next;
        if (node != &m->events) {
            if (node->prev) {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            }
            struct aud_event *e = node->event;
            free(node);
            aud_mutex_unlock(&m->lock);
            aud_module_push_event(m, 0, e);
            aud_module_sync(m, 0);
            continue;
        }
        if (m->buffering ? m->queued_frames <= m->buffering_frames :
                           m->queued_frames == 0) {
            aud_cond_wait(&m->ready, &m->lock);
            aud_mutex_unlock(&m->lock);
            continue;
        }
        aud_mutex_unlock(&m->lock);
        if (m->first && !m->first_sent)
            rts_push_event_first(m);
        aud_mutex_lock(&m->lock);
        struct aud_data *d = aud_data_adapter_dequeue(&m->queue);
        if (d) {
            m->queued_bytes -= d->size;
            m->queued_frames -= d->size >> (m->frame_shift & 31);
        }
        --m->packet_count;
        aud_cond_signal(&m->space);
        aud_mutex_unlock(&m->lock);
        if (d) {
            if (rts_process(m)) {
                ++m->dropped_count;
                aud_object_put(d);
            } else {
                ++m->output_count;
                aud_module_push_data(m, 0, d);
                aud_module_sync(m, 0);
            }
        }
        m->buffering = false;
    }
    return 0;
}

/* @0x600a5ae4: Only type 0 (PCM) admitted. Non-PCM is released immediately,
 * even if FIRST retained non-PCM metadata. Strict > max permits one extra packet. */
int ts_chai(struct aud_port *port, struct aud_data *d)
{
    struct ts_module *m = port->module;
    if (d->type != 0) {
        aud_object_put(d);
        return 0;
    }
    aud_mutex_lock(&m->lock);
    while (m->queued_frames > m->queue_max && !m->flushing)
        aud_cond_wait(&m->space, &m->lock);
    if (m->flushing) {
        aud_object_put(d);
    } else {
        ++m->input_count;
        aud_data_adapter_push(&m->queue, d);
        ++m->packet_count;
        m->queued_bytes += d->size;
        m->queued_frames += d->size >> (m->frame_shift & 31);
        aud_cond_signal(&m->ready);
    }
    aud_mutex_unlock(&m->lock);
    return 0;
}

/* @0x600a5c58: Includes split blocks 0x600a5d31 and 0x600a5d75. FIRST consumes
 * (takes ownership of) the original event without ref-get; does not forward it.
 * Byte/frame shift is ceil(log2(channels * bits / 8)), not exact division for
 * non-power-of-two frames. Format does not choose PCM/ES admission here.
 * Type 7, bit1 clear starts flushing and forwards; bit1 set ends flushing,
 * flushes PCM, queues that event to the worker and consumes it. */
int ts_event(struct ts_module *m, struct aud_port *port, struct aud_event *e)
{
    if (e->type == 1) {
        aud_mutex_lock(&m->lock);
        int channels = aud_event_value_get_int(e, "channels", 2);
        int bits = aud_event_value_get_int(e, "bits-per-sample", 32);
        int rate = aud_event_value_get_int(e, "rate", 48000);
        m->rate = (unsigned)rate;
        int32_t bytes = (int32_t)((uint32_t)channels * (uint32_t)bits) / 8;
        if (bytes > 0)
            --bytes;
        unsigned shift = 0;
        /* Negative malformed frame sizes never terminate in the binary either. */
        while (bytes != 0) {
            ++shift;
            bytes = bytes >= 0 ? bytes / 2 : -1 - (int32_t)((uint32_t)(-1 - bytes) / 2);
        }
        m->frame_shift = shift;
        m->queue_max = (unsigned)((int32_t)((uint32_t)rate * 500u) / 1000) +
                       m->buffering_frames;
        if (m->first)
            aud_object_put(m->first);
        m->first = e;
        m->first_sent = false;
        aud_mutex_unlock(&m->lock);
        if (!m->running && !m->thread) {
            m->running = true;
            m->thread = aud_thread_create(m->name, 0x20000, m->priority,
                                          ts_thread_work, m);
            if (!m->thread)
                m->running = false;
        }
        return 0;
    }
    if (e->type == 7) {
        aud_mutex_lock(&m->lock);
        if (!(e->prop & 2)) {
            m->flushing = true;
            aud_cond_signal(&m->space);
        } else {
            m->flushing = false;
            ts_queue_flush_locked(m);
            struct rts_event_node *node = calloc(1, sizeof(*node));
            if (node) {
                node->event = e;
                node->prev = m->events.prev;
                node->next = &m->events;
                m->events.prev->next = node;
                m->events.prev = node;
            }
            /* Binary allocation failure also consumes without releasing e. */
            aud_cond_signal(&m->ready);
            aud_mutex_unlock(&m->lock);
            return 0;
        }
        aud_mutex_unlock(&m->lock);
    }
    return aud_module_default_event(m, port, e);
}

/* @0x600a5ea8: Invalid mode leaves the old mode unchanged; always succeeds. */
int ts_set_operation_mode(struct ts_module *m, unsigned mode)
{
    aud_mutex_lock(&m->lock);
    if (mode <= 2)
        m->operation_mode = mode;
    aud_mutex_unlock(&m->lock);
    return 0;
}

/* @0x600a5f4c: Includes scalar split command blocks through 0x600a623f.
 * Configuration validation returns success even when refusing the values.
 * No payload-size validation was present in these recognized commands. */
int ts_command(struct ts_module *m, unsigned command, const unsigned *p, unsigned size)
{
    switch (command) {
    case 0x10010000: case 0x10020000: case 0x1003002c:
    case 0x5d010004: case 0x5d03000c:
        return 0;
    case 0x5d100004:
        return ts_set_operation_mode(m, p[0]);
    case 0x5d000010:
        if (p && p[0] >= p[1] && p[2] < p[0] && p[1] < p[3]) {
            aud_mutex_lock(&m->lock);
            m->fast_threshold = p[0] / 384;
            m->slow_threshold = p[1] / 384;
            m->fast_recovery = p[2] / 384;
            m->slow_recovery = p[3] / 384;
            aud_mutex_unlock(&m->lock);
        }
        return 0;
    case 0x5d02000c:
        /* Despite log names fast/normal/slow, actual branches accept ascending. */
        if (p[0] <= p[1] && p[1] <= p[2]) {
            aud_mutex_lock(&m->lock);
            m->output_rate[1] = p[0];
            m->output_rate[0] = p[1];
            m->output_rate[2] = p[2];
            aud_mutex_unlock(&m->lock);
        }
        return 0;
    case 0x5d110010:
        if (p[1] <= p[0]) {
            aud_mutex_lock(&m->lock);
            m->skip_recovery = p[1] / 384;
            m->skip_threshold = p[0] / 384;
            aud_mutex_unlock(&m->lock);
        }
        return 0;
    case 0x5d120004:
        if (p[0] > 8 && p[0] <= 10) {
            aud_mutex_lock(&m->lock);
            m->priority = p[0];
            aud_mutex_unlock(&m->lock);
        }
        return 0;
    default:
        return aud_module_default_command(m, command, p, size);
    }
}

/* @0x600a6244: Framework provides zeroed private storage; does not reset all fields. */
int ts_constructor(struct ts_module *m)
{
    m->rate = m->output_rate[0] = 48000;
    m->events.next = m->events.prev = &m->events;
    m->frame_shift = 3;
    m->unknown_16c = m->fast_threshold = m->skip_threshold =
        m->unknown_17c = m->unknown_198 = UINT32_MAX;
    m->priority = 10;
    m->output_rate[1] = 47750;
    m->output_rate[2] = 48250;
    m->queue_max = 24000;
    m->buffering_frames = m->buffering_ms = 0;
    m->slow_threshold = m->fast_recovery = m->slow_recovery = m->skip_recovery = 0;
    aud_data_adapter_init(&m->queue);
    aud_mutex_init(&m->lock);
    aud_cond_init(&m->ready);
    aud_cond_init(&m->space);
    return 0;
}

/* @0x600a6304: Stop, wake both waiters, join, release PCM and cached FIRST.
 * Pending event list is not drained by this destructor in the binary. */
void ts_destructor(struct ts_module *m)
{
    if (m->thread) {
        aud_mutex_lock(&m->lock);
        m->running = false;
        aud_cond_signal(&m->space);
        aud_cond_signal(&m->ready);
        aud_mutex_unlock(&m->lock);
        aud_thread_destroy(m->thread);
        m->thread = NULL;
    }
    ts_queue_flush(m);
    aud_data_adapter_cleanup(&m->queue);
}

/* @0x600a6394 */
int ts_set_prop_op_mode(struct ts_module *m, const void *property, const char *value)
{
    (void)property;
    return ts_set_operation_mode(m, (unsigned)strtoul(value, NULL, 0));
}

/* @0x600a6444: No locking in binary. FIRST does not recompute buffering_frames
 * from buffering_ms, so a rate change retains the existing buffering frame count. */
int ts_set_prop_buffering_level(struct ts_module *m, const void *property, const char *value)
{
    (void)property;
    m->buffering_ms = (unsigned)strtoul(value, NULL, 0);
    m->buffering_frames = (m->buffering_ms * m->rate) / 1000u;
    m->queue_max = m->buffering_frames +
                   (unsigned)((int32_t)(m->rate * 500u) / 1000);
    return 0;
}

/* @0x600a64c8 */
void ts_initializer(void)
{
    ModuleRegistry_Register(rts_module_registration);
}
