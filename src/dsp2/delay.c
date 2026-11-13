/*
 * dsp2 "delay" module (class uid 0x041c), first module of the renderer bypass/ES
 * output leg: ... -> delay -> gain -> bypass -> output.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Linked at 0x60000000. Property table @0x60102950: delay (int +0x100, setter),
 * packet-count (ro +0x140), queued-length (getter), delayed (bool +0x15c),
 * number-packet (+0x150), au-length (+0x144). Branch FLIX now decoded with
 * tools/xtobjdump.sh; queued PCM fade/flush paths remain incomplete.
 * queued-length is a STRING getter (type 0), no geti: kernel aimc_get_property
 * cannot read it. packet-count, au-length, number-packet and delay are integers.
 * Evidence: build/scratch/a13/delay.asm, build/decomp/audio_a0_dsp2.main_60000000.bin.c ~122932.
 *
 * ES handling (observed):
 *  - delay == 0: every buffer (PCM, ES, meta) goes straight to out0.
 *  - delay  > 0: ES buffers are cut into number-packet chunks, each re-typed ES,
 *    queued, and released once more than `delay` ms is queued.
 *  - Drops: a delay change flushes the queue (queued ES lost), may drop the
 *    current buffer ("return delay chain", lvl 4), then queues `delay` ms of zero
 *    packets (typed ES in ES mode). LAST flushes (pushing what is queued),
 *    synchronous DISCONNECT flushes and resets delay to 0.
 * For ES channels=8/data-rate=192000, bytes_per_ms_unit=16 and rate_khz=192;
 * unit=ceil(192*16/48)=64 bytes per 48-kHz tick. With au-length=256 and a
 * 32768-byte input, number-packet=max(1,ceil(floor(32768/256)/64))=2.
 * Release requires floor(192*256*packet_count/48)>256+192*delay-1;
 * at 10 ms the minimum is 3 chunks. Normal positive values do not hold an
 * infinite stream forever; a short tail can remain queued until LAST.
 * FIRST uses sampling-rate in kHz (default 48), not a raw GStreamer 48000;
 * ES data-rate=192000 is explicitly normalized to 192. No live FIRST was read.
 *  - No data-type based drop and no "drop until first" logic.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct aud_event { int pad[3]; int type; unsigned prop; };
struct aud_data {
    int pad[4];
    int type;                    /* +0x10 */
    int pad1[8];
    unsigned size;               /* +0x34 */
    void *buf;                   /* +0x38 */
};

struct delay {
    const char *name;            /* +0x000 */
    /* aud_module core up to 0xf0 */
    int  channels;               /* +0x0f0 */
    int  bits;                   /* +0x0f4 */
    int  rate_khz;               /* +0x0f8 sampling-rate/1000, or data-rate/1000 for ES */
    int  bytes_per_ms_unit;      /* +0x0fc bytes per frame (ch*bits/8, ch*16/8 for ES) */
    int  delay;                  /* +0x100 active delay, ms */
    int  new_delay;              /* +0x104 written by the "delay" prop setter */
    char mutex[0x1c];            /* +0x108 */
    char adapter[0x10];          /* +0x124 */
    unsigned q_in, q_out;        /* +0x134/+0x138 */
    int  packet_count;           /* +0x140 */
    int  au_length;              /* +0x144 */
    int  new_au_length;          /* +0x148 */
    bool fadein_pending;         /* +0x14c */
    bool dolby;                  /* +0x14d */
    bool es;                     /* +0x14e */
    int  number_packet;          /* +0x150 */
    int  new_number_packet;      /* +0x154 */
    unsigned last_size;          /* +0x158 */
    bool delayed;                /* +0x15c */
    bool recalc;                 /* +0x15d */
    struct aud_event *first;     /* +0x160 */
    void *pending_events;        /* +0x164 list */
};

extern void *aud_object_get(void *);
extern void aud_object_put(void *);
extern int  aud_module_push_data(void *, int, struct aud_data *);
extern int  aud_module_push_event(void *, int, struct aud_event *);
extern int  aud_module_sync(void *, int);
extern int  aud_module_default_event(void *, void *, struct aud_event *);
extern int  aud_module_default_command(void *, unsigned, void *, unsigned);
extern void aud_mutex_lock(void *), aud_mutex_unlock(void *);
extern struct aud_data *aud_data_new(unsigned size);                     /* @0x6006b198 */
extern struct aud_data *aud_data_new_zero(unsigned size);                /* @0x6006b3a0 */
extern int  aud_data_adapter_push(void *, struct aud_data *);
extern void aud_data_adapter_flush(void *, unsigned);
extern struct aud_data *aud_data_adapter_dequeue(void *);                /* @0x6006be14 */
extern void *memcpy(void *, const void *, size_t);
extern int  strcmp(const char *, const char *);
extern int  aud_event_value_get_int(struct aud_event *, const char *, int);
extern int  aud_event_value_get_bool(struct aud_event *, const char *, int);
extern const char *aud_event_value_get_string(struct aud_event *, const char *, const char *);
extern int  aud_event_has_name(struct aud_event *, const char *);        /* @0x6006a784 */
extern void aud_event_value_set_bool(struct aud_event *, const char *, int); /* @0x6006a3e8 */
extern struct aud_event *aud_event_newv_custom(const char *name, ...);   /* @0x6006a304 */
extern int  _pending_events_add(void *list, struct aud_event *, void *q, void *m,
                                int (*push)(void *, struct aud_event *)); /* @0x6008f950 */
extern void _pending_events_check(void *list);                           /* @0x6008fa08 */
extern void _pending_events_empty(void *m);                              /* @0x6008fabc */
extern void _pending_events_update(void *list, unsigned consumed);       /* inlined */
extern unsigned long strtoul(const char *, char **, int);

/* @0x6008fb68: signed divide before multiply; always channels * 4 bytes/frame,
 * even when +0xfc is channels * 2. Source is argument 3, frames argument 4.
 * With 8ch/16bit, chain's 8192 / 16 frames write 16384 bytes. The ascending
 * /512 ramp matches the captured damaged heap in native-hd-fault-20261113-140842;
 * entry arguments are not recorded, so execution attribution remains inferred. */
static void delay_fadein(struct delay *dl, int32_t *dst, const int32_t *src,
                         unsigned frames)
{
    for (unsigned frame = 0; frame < frames; frame++) {
        for (int channel = 0; channel < dl->channels; channel++) {
            int32_t sample = *src++ / (int32_t)frames;
            *dst++ = (int32_t)((uint32_t)sample * frame);
        }
    }
}

/* @0x6008fc40: starts at frames, ends at 1, with the same fixed 32-bit stride. */
static void delay_fadeout(struct delay *dl, int32_t *dst, const int32_t *src,
                          unsigned frames)
{
    for (unsigned frame = 0; frame < frames; frame++) {
        for (int channel = 0; channel < dl->channels; channel++) {
            int32_t sample = *src++ / (int32_t)frames;
            *dst++ = (int32_t)((uint32_t)sample * (frames - frame));
        }
    }
}

/* @0x6008fb20 delay_push_event (syms: "delay_set_delay"; log string "push event %s") */
int delay_push_event(void *m, struct aud_event *e)
{
    /* "%s: push event %s" (lvl 3) */
    aud_module_push_event(m, 0, e);
    aud_module_sync(m, 0);
    return 0;
}

/* @0x6008ffb4 delay_push_output */
int delay_push_output(struct delay *dl, struct aud_data *d)
{
    if (!d)
        return -1;                                    /* "%s: no data" */
    aud_module_push_data(dl, 0, d);
    aud_module_sync(dl, 0);
    return 0;
}

/* @0x6008fd1c delay_queue_push_locked */
int delay_queue_push_locked(struct delay *dl, struct aud_data *d, bool es)
{
    if (!d)
        return -1;
    if (es && dl->number_packet >= 1) {
        unsigned left = d->size, chunk = (d->size + dl->number_packet - 1) / dl->number_packet;
        const char *src = d->buf;

        for (int i = 0; i < dl->number_packet; i++) {
            struct aud_data *p = aud_data_new(chunk);
            memcpy(p->buf, src, chunk);
            p->type = 1;                              /* ES */
            aud_data_adapter_push(dl->adapter, p);
            dl->packet_count++;
            src += chunk;
            left -= chunk;
            chunk = chunk < left ? chunk : left;
            /* "%s: size %d, %d/%d" (lvl 4) */
        }
        aud_object_put(d);
        return 0;
    }
    aud_data_adapter_push(dl->adapter, d);
    if (d->type == 0 || (dl->es && d->type == 1))
        dl->packet_count++;
    /* "%s: packet_count=%d type=%d" (lvl 5); adapter owns d. */
    return 0;
}

/* @0x6008fe64 delay_add_zero: queue `ms` of silence. "%s: add zero" (lvl 3). */
int delay_add_zero(struct delay *dl, int ms)
{
    unsigned unit = (unsigned)((dl->rate_khz * dl->bytes_per_ms_unit + 0x2f) / 0x30);
    int total = ms * (int)unit * 48;
    int per = dl->au_length * (int)unit;

    aud_mutex_lock(dl->mutex);
    while (total > 0) {
        struct aud_data *z = aud_data_new_zero((unsigned)per);
        if (z && dl->es)
            z->type = 1;                              /* "%s: ES data size = %d" (lvl 5) */
        if (z) {
            aud_data_adapter_push(dl->adapter, z);
            if (z->type == 0 || (dl->es && z->type == 1))
                dl->packet_count++;
        }
        total -= per;
    }
    aud_mutex_unlock(dl->mutex);
    return 0;
}

/* 0x6008fffc: queued fadeout is not reconstructed. [FLIX] Declaration
 * only; set_delay gates it on !es, but flush does not. No +0x14c store.
 * Do not substitute a successful no-op. */
extern int delay_queue_fadeout(struct delay *dl);

/* Unresolved PCM flush hook. The original prefix @0x600900c1..600900d3
 * actually gates queue_fadeout on packet_count && push_all, including ES.
 * No +0x14c store occurs anywhere in flush @0x600900ac..60090180. */
extern void delay_queue_flush_pcm_unrecovered(struct delay *, int push_all);

/* @0x600900ac delay_queue_flush: recovered ES path; PCM remains unresolved. */
void delay_queue_flush(struct delay *dl, int push_all)
{
    struct aud_data *d;

    if (!dl->es && push_all) {
        delay_queue_flush_pcm_unrecovered(dl, push_all);
        return;
    }
    aud_mutex_lock(dl->mutex);
    while ((d = aud_data_adapter_dequeue(dl->adapter)) != NULL) {
        if (d->type != 2 && d->type != 3)
            dl->packet_count--;
        if (push_all)
            delay_push_output(dl, d);
        else
            aud_object_put(d);
    }
    _pending_events_empty(dl);
    dl->packet_count = 0;
    aud_mutex_unlock(dl->mutex);
}

/* @0x60090180 delay_set_delay: does not arm or clear +0x14c. */
int delay_set_delay(struct delay *dl, int ms)
{
    int pushed = 0;
    unsigned queued;

    aud_mutex_lock(dl->mutex);
    if (dl->delay == ms) {                            /* decoded beq @0x6009019a -> 0x60090295 */
        aud_mutex_unlock(dl->mutex);
        return 0;
    }
    /* "%s: set delay: %dms -> %dms" (lvl 2) */
    queued = dl->q_in - dl->q_out;
    if (dl->packet_count && !dl->es)
        pushed = delay_queue_fadeout(dl);
    if (queued)
        aud_data_adapter_flush(dl->adapter, queued);  /* queued data DROPPED */
    _pending_events_update(&dl->pending_events, queued);
    _pending_events_check(&dl->pending_events);
    dl->delay = ms;
    dl->packet_count = 0;
    /* "%s: send delay-changed" (lvl 3) */
    {
        struct aud_event *e = aud_event_newv_custom("delay-changed",
            "is-delay", ms != 0, "unit-per-packet", dl->number_packet,
            "au-length", dl->au_length, "es", dl->es, NULL);
        if (e)
            _pending_events_add(&dl->pending_events, e, dl->adapter, dl,
                                (int (*)(void *, struct aud_event *))delay_push_event);
    }
    aud_mutex_unlock(dl->mutex);
    return pushed;
}

/* @0x600902f0 delay_chain */
int delay_chain(void **port, struct aud_data *d)
{
    struct delay *dl = port[1];
    int old = dl->delay;

    /* 0x60090309/60090530/600905b8: re-evaluate on FIRST (recalc), the first
     * buffer, or a size change; the two divisions are NOT interchangeable with
     * ceil(size/(unit*au)). The firmware first truncates size/new_au_length. */
    if (dl->es && (dl->recalc || !dl->last_size || d->size != dl->last_size)) {
        unsigned unit = (unsigned)((dl->rate_khz * dl->bytes_per_ms_unit + 47) / 48);
        unsigned units = d->size / (unsigned)dl->new_au_length;
        unsigned packets = (units + unit - 1) / unit;
        dl->new_number_packet = packets ? (int)packets : 1;
    }
    dl->last_size = d->size;                           /* 0x60090362: for ALL data types */
    dl->recalc = false;
    if (dl->number_packet != dl->new_number_packet || dl->au_length != dl->new_au_length) {
        dl->number_packet = dl->new_number_packet;     /* 0x60090384 */
        dl->au_length = dl->new_au_length;
    }

    if (dl->delay != dl->new_delay) {                 /* decoded bne @0x60090393 -> 0x600904b8 */
        int pushed = delay_set_delay(dl, dl->new_delay);

        if (dl->delay)                                /* 0x600904c7..600904e8 */
            delay_add_zero(dl, (dl->rate_khz * dl->delay + dl->rate_khz - 1) /
                              dl->rate_khz);
        dl->fadein_pending = !dl->es;                 /* 0x600904f7: only chain's delay-change path */
        if (pushed) {
            aud_object_put(d);                        /* current buffer dropped */
            /* "%s: return delay chain" (lvl 4) */
            return 0;
        }
        if (old == 0 && dl->delay) {                  /* 0x60090608: 0 -> N */
            if (d->type == 0)
                delay_fadeout(dl, d->buf, d->buf,
                              d->size / (unsigned)dl->bytes_per_ms_unit);
            return delay_push_output(dl, d);
        }
    }

    if (d->type == 0 && dl->fadein_pending) {         /* 0x60090599 */
        delay_fadein(dl, d->buf, d->buf,
                     d->size / (unsigned)dl->bytes_per_ms_unit);
        dl->fadein_pending = false;
    }

    if (!dl->delay)
        return delay_push_output(dl, d);              /* 0x60090659: ES passes through */

    aud_mutex_lock(dl->mutex);
    delay_queue_push_locked(dl, d, dl->es);
    /* Exact 32-bit unsigned predicate @0x600903ca..600903e8 and
     * 0x6009045a..60090472 (decoded bgeu exits). The old reconstruction omitted
     * rate/48 and the one-AU rounding term. No PCM fade or bit-depth gate for ES. */
    while (dl->q_in != dl->q_out &&
           ((uint32_t)dl->rate_khz * ((uint32_t)dl->au_length *
                                    (uint32_t)dl->packet_count)) / 48u >
           (uint32_t)dl->au_length + (uint32_t)dl->rate_khz *
                                     (uint32_t)dl->delay - 1u) {
        struct aud_data *o = aud_data_adapter_dequeue(dl->adapter);
        if (!o)
            break;
        if (o->type != 2 && o->type != 3)
            dl->packet_count--;
        /* "%s: packet_count=%d type=%d" (lvl 5) */
        unsigned consumed = o->size;                 /* 0x6009049b */
        delay_push_output(dl, o);
        _pending_events_update(&dl->pending_events, consumed);
        _pending_events_check(&dl->pending_events);
    }
    aud_mutex_unlock(dl->mutex);
    return 0;
}

/* @0x600906bc delay_event: FIRST does not arm +0x14c, even when applying a
 * requested delay. LAST's flush does not clear it; consumption clears it
 * only after delay_fadein returns @0x600905b2. */
int delay_event(struct delay *dl, void *port, struct aud_event *e)
{
    /* "%s: event %s" (lvl 4) */
    if (e->type == 0xd && aud_event_has_name(e, "delay-changed")) {
        if (aud_event_value_get_bool(e, "is-delay", 0)) {
            dl->delayed = true;
            dl->new_number_packet = 1;
            dl->new_au_length = 0x100;
        } else {
            int u = aud_event_value_get_int(e, "unit-per-packet", 6);
            dl->delayed = false;
            dl->new_number_packet = u ? u : 1;
            dl->new_au_length = aud_event_value_get_int(e, "au-length", 0x100);
        }
        /* "%s: got delay-changed isDelay = %d, nPacket = %d" (lvl 2) */
        aud_object_put(e);
        return 0;
    }

    if (e->type == 1) {                               /* decoded beqi type,1 @0x600906d0 */
        const char *codec;

        /* "%s: got first event" (lvl 3) */
        if (dl->new_delay != dl->delay)               /* decoded bne @0x60090776 */
            delay_set_delay(dl, dl->new_delay);
        dl->channels = aud_event_value_get_int(e, "channels", 8);
        dl->bits = aud_event_value_get_int(e, "bits-per-sample", 32);
        dl->bytes_per_ms_unit = dl->channels * dl->bits / 8;
        dl->rate_khz = aud_event_value_get_int(e, "sampling-rate", 48);
        dl->es = aud_event_value_get_bool(e, "es", 0);
        dl->dolby = aud_event_value_get_bool(e, "dolby", 0);
        dl->delayed = aud_event_value_get_bool(e, "delayed", 0);
        codec = aud_event_value_get_string(e, "codec", "pcm");
        if (dl->es && strcmp(codec, "pcm")) {
            int sn = aud_event_value_get_int(e, "sample-number", 0x600);
            int dr;

            dl->new_au_length = aud_event_value_get_int(e, "au-length", 0x100);
            dl->new_number_packet = sn / dl->new_au_length ? sn / dl->new_au_length : 1;
            dr = aud_event_value_get_int(e, "data-rate", dl->rate_khz);
            dl->rate_khz = dr > 1000 ? dr / 1000 : dr;
            dl->bytes_per_ms_unit = dl->channels * 16 / 8;
            /* "%s: got es only mode codec %s, %d ms per packet, %d unit per packet,
             *  %dKHz" (lvl 3) */
        } else {
            dl->number_packet = 1;
            dl->au_length = 0x100;
        }
        if (dl->first) {                              /* drop an older pending first */
            /* "remove event %s. %d" (lvl 3) */
            aud_object_put(dl->first);
            dl->first = NULL;
        }
        dl->recalc = true;
        if (dl->delay) {
            dl->first = aud_object_get(e);
            if (dl->es && !dl->delayed)
                aud_event_value_set_bool(e, "delayed", 1);
            /* the FIRST is queued behind the data already in the delay line */
            _pending_events_add(&dl->pending_events, e, dl->adapter, dl,
                                (int (*)(void *, struct aud_event *))delay_push_event);
            if (dl->q_in == dl->q_out)               /* decoded beq @0x60090934 */
                delay_add_zero(dl, dl->delay);        /* @0x60090a05 */
            return 0;
        }
        _pending_events_empty(dl);                    /* 0x600909e9 */
        if (dl->q_in == dl->q_out)                   /* @0x600909f5 -> 60090a81 */
            delay_add_zero(dl, dl->delay);            /* zero delay: empty no-op */
        return aud_module_default_event(dl, port, e); /* forwarded at once */
    }

    if (e->type == 2) {                               /* decoded beqi type,2 @0x600906d8 */
        /* "%s: last event" (lvl 3) */
        if (dl->delay)
            delay_queue_flush(dl, 1);                 /* push the tail */
    } else if (e->type == 6 && (e->prop & 2)) {       /* 0x60090966: only SYNC DISCONNECT */
        /* "%s: disconnect event" (lvl 3) */
        if (dl->delay) {
            delay_queue_flush(dl, 0);                 /* drop the tail */
            delay_set_delay(dl, 0);
        }
    }
    return aud_module_default_event(dl, port, e);
}

/* @0x60090a9c delay_command */
int delay_command(struct delay *dl, unsigned cmd, void *param, unsigned len)
{
    return aud_module_default_command(dl, cmd, param, len);
}

/* @0x60090ad8 delay_constructor */
int delay_constructor(struct delay *dl)
{
    dl->channels = 2;
    dl->bits = 32;
    dl->rate_khz = 48;
    dl->bytes_per_ms_unit = 8;
    dl->au_length = dl->new_au_length = 0x100;
    dl->number_packet = dl->new_number_packet = 1;
    return 0;
}

/* @0x60090b58 delay_destructor */
void delay_destructor(struct delay *dl)
{
    delay_queue_flush(dl, 0);
    _pending_events_empty(dl);
    if (dl->first)
        aud_object_put(dl->first);
}

/* @0x60090bf8 delay_set_param_delay: only records the request; delay_chain/FIRST apply it */
int delay_set_param_delay(struct delay *dl, const void *prop, const char *val)
{
    (void)prop;
    dl->new_delay = (int)strtoul(val, NULL, 0);
    return 0;
}

/* @0x60090c10: allocated string, NOT a (buffer,size) getter. No geti is present
 * in descriptor @0x601029a0, so integer ADEC_CMD_GET_PROP returns -1. */
char *delay_prop_gets_queued_length(struct delay *dl, const void *prop)
{
    extern char *audutil_strdup_printf(const char *, ...);
    (void)prop;
    return audutil_strdup_printf("%d", (int)(dl->q_in - dl->q_out));
}
