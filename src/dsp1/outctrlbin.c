/*
 * dsp1 "outctrlbin": the bin module between "queue" and "output_*" in every kernel
 * output chain (src/kernel/aud/output.c output_set_type: queue -> outctrlbin -> output_X).
 * It builds [pcmcvt ->] delay -> gain internally and forwards gain / fade-gain /
 * mute-output / delay to its children.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp1.bin
 *   sha256 8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4
 * Main segment linked at 0x60000000. Class @0x602810b0 ("outctrlbin", instance size
 * 0x140), property table @0x602810e0 (gain int +0x12c, fade-gain int +0x130,
 * mute-output bool +0x134, delay int +0x138; each 0x14-byte entry has a setter).
 * Evidence: ../../decomp/audio_a0_dsp1.main_60000000.bin.c (0x60125208..0x60125948).
 * The child modules are the same code as dsp2's: gain 0x60110c54.. (gain_control,
 * gcd, gain_set_fade_gain, ... LEN-EQ/BYTES-EQ with dsp2 0x6008810c..; ../dsp2/gain.c)
 * and delay 0x601186f8..0x60119810 (first 10 functions LEN-EQ with dsp2 0x6008fb20..;
 * ../dsp2/delay.c). The ES strings ("ES drop? %s", "MUTE : ES ONLY.. so push zero
 * data", "%s: add zero %d/%d") are in both images.
 *
 * ES through this bin (FIRST with es=1): outctrlbin_init(bin, false), so there is NO
 * pcmcvt and the chain is delay -> gain.
 *  - delay: delay 0 passes ES unchanged. delay > 0 queues ES in packet chunks, and a
 *    delay change flushes the queue and inserts zero ES packets.
 *  - gain: es_mode (from FIRST "es") with mute-output=1 replaces whole ES packets by
 *    same-size zero buffers. Not muted -> ES passes unchanged (no scaling). If ES data
 *    arrives without es_mode, it is dropped while muted.
 * Note: the constructor sets mute-output = 1, and the kernel's output_pre_disconnect
 * also sets mute-output/mute-input = 1. ES therefore stays zeroed until the kernel
 * writes mute-output=0.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct aud_event { int type; };                 /* +0x0c */
struct aud_port  { int index; };

struct outctrlbin;
struct outctrlbin_class {
    int  (*super_constructor)(struct outctrlbin *b);    /* +0x60 */
    void (*super_destructor)(struct outctrlbin *b);     /* +0x64: also tears down children */
    int  (*super_event)(struct outctrlbin *b, struct aud_port *p, struct aud_event *e); /* +0x68 */
};

struct outctrlbin {
    const char *name;                           /* +0x000 */
    const struct outctrlbin_class *cls;         /* +0x00c */
    void       *gain;                           /* +0x118 child "gain" */
    void       *delay;                          /* +0x11c child "delay" */
    void       *pcmcvt;                         /* +0x120 child "pcmcvt" (10-ch PCM only) */
    int         channels;                       /* +0x124 last FIRST "channels" */
    bool        es;                             /* +0x128 last FIRST "es" */
    int         gain_value;                     /* +0x12c property "gain" (-1 = unset) */
    int         fade_gain;                      /* +0x130 property "fade-gain" */
    bool        mute_output;                    /* +0x134 property "mute-output" */
    int         delay_ms;                       /* +0x138 property "delay" (-1 = unset) */
};

struct aud_prop_desc { const char *name; int type; int offset; };   /* type 1 bool 2 int 4 str */

extern char *audutil_strdup_printf(const char *fmt, ...);
extern int   aud_module_set_prop(void *m, const char *kv, size_t len);
extern void *aud_module_bin_add(void *bin, const char *factory);
extern int   aud_module_bin_connect(void *bin, void *src, int sport, void *dst, int dport);
extern int   aud_module_bin_set_input(void *bin, int port, void *m, int mport);
extern int   aud_module_bin_set_output(void *bin, int port, void *m, int mport);
extern int   aud_module_bin_add_cmd_propagate(void *bin, void *m);
extern int   aud_module_bin_propagate_pending(void *bin);
extern int   aud_event_value_get_int(struct aud_event *e, const char *k, int def);
extern bool  aud_event_value_get_bool(struct aud_event *e, const char *k, bool def);
extern char *aud_event_str(struct aud_event *e);
extern int   aud_object_class_register(void *cls);
extern int   aud_mregistry_add_class(void *cls);
extern unsigned long _Stoul(const char *s, char **end, int base);

extern char outctrlbin_class_object[];          /* @0x602810b0 */
extern const struct aud_prop_desc outctrlbin_props[]; /* @0x602810e0 */

/* @0x60125208: child->set("<key>=<value>"). */
int outctrlbin_set_property(void *child, const char *key, int value)
{
    if (!child)
        return 0;
    char *kv = audutil_strdup_printf("%s=%d", key, value);
    /* "set [%s] : [%s] = [%d]" */
    if (kv) {
        aud_module_set_prop(child, kv, strlen(kv));
        free(kv);
    }
    return 0;
}

/* @0x60125268: (re)build the children. with_pcmcvt is true only for 10-channel PCM. */
int outctrlbin_init(struct outctrlbin *b, bool with_pcmcvt)
{
    if (b->gain || b->delay) {
        /* "%s: remove old modules" */
        if (b->cls->super_destructor)
            b->cls->super_destructor(b);
    }
    if (with_pcmcvt) {
        b->pcmcvt = aud_module_bin_add(b, "pcmcvt");
        if (!b->pcmcvt)
            return -1;
    }
    b->gain = aud_module_bin_add(b, "gain");
    if (!b->gain)
        return -1;                              /* "%s: failed to add %s" */
    b->delay = aud_module_bin_add(b, "delay");
    if (!b->delay)
        return -1;

    if (with_pcmcvt) {
        aud_module_bin_connect(b, b->pcmcvt, 0, b->delay, 0);
        aud_module_bin_add_cmd_propagate(b, b->pcmcvt);
        aud_module_bin_set_input(b, 0, b->pcmcvt, 0);
    } else {
        aud_module_bin_set_input(b, 0, b->delay, 0);
    }
    aud_module_bin_connect(b, b->delay, 0, b->gain, 0);
    aud_module_bin_add_cmd_propagate(b, b->gain);
    aud_module_bin_add_cmd_propagate(b, b->delay);
    aud_module_bin_set_output(b, 0, b->gain, 0);

    if (with_pcmcvt)
        outctrlbin_set_property(b->pcmcvt, "out-channels", 8);
    if (b->gain_value >= 0) {
        outctrlbin_set_property(b->gain, "gain", b->gain_value);
        outctrlbin_set_property(b->gain, "fade-gain", b->fade_gain);
        outctrlbin_set_property(b->gain, "mute-input", b->mute_output);
        outctrlbin_set_property(b->gain, "mute-output", b->mute_output);
    }
    if (b->delay_ms >= 0)
        outctrlbin_set_property(b->delay, "delay", b->delay_ms);
    return 0;
}

/* @0x60125488: rebuild on FIRST when channels or es changed, then flush pending props
 * and continue with the bin's default event handling. */
int outctrlbin_event(struct outctrlbin *b, struct aud_port *port, struct aud_event *e)
{
    /* "%s: [port %d] event %s" */
    if (port) {
        int type = e->type;
        if (type == 1) {
            char *s = aud_event_str(e);
            /* "%s: [port %d] first event: %s" / "...got first event" */
            free(s);
            int ch = aud_event_value_get_int(e, "channels", 2);
            bool es = aud_event_value_get_bool(e, "es", false);
            if (b->channels != ch || b->es != es) {
                b->channels = ch;
                b->es = es;
                outctrlbin_init(b, ch == 10 && !es);
            }
            aud_module_bin_propagate_pending(b);
        }
        /* type 2: "%s: [port %d] last event"; type 6: "%s: [port %d] disconnect" */
    }
    return b->cls->super_event(b, port, e);
}

/* @0x601255f0: unset gain/fade/delay (-1), mute-output = 1. */
int outctrlbin_constructor(struct outctrlbin *b)
{
    /* "%s: create.." */
    if (b->cls->super_constructor)
        b->cls->super_constructor(b);
    b->fade_gain = -1;
    b->gain_value = -1;
    b->mute_output = true;
    b->delay_ms = -1;
    return 0;
}

/* @0x6012563c */
void outctrlbin_destructor(struct outctrlbin *b)
{
    /* "%s: destroy.." */
    if (b->cls->super_destructor)
        b->cls->super_destructor(b);
    b->pcmcvt = NULL;
    b->gain = NULL;
    b->delay = NULL;
}

/* @0x6012567c: logging only ("%s: updated %s %d" / bool / string). */
int outctrlbin_prop_updated(struct outctrlbin *b, const struct aud_prop_desc *p)
{
    (void)b;
    (void)p;
    return 0;
}

/* @0x60125718 */
int outctrlbin_set_param_gain(struct outctrlbin *b, const char *key, const char *val)
{
    (void)key;
    b->gain_value = (int)_Stoul(val, NULL, 0);
    return outctrlbin_set_property(b->gain, "gain", b->gain_value);
}

/* @0x60125790 */
int outctrlbin_set_param_fade_gain(struct outctrlbin *b, const char *key, const char *val)
{
    (void)key;
    b->fade_gain = (int)_Stoul(val, NULL, 0);
    return outctrlbin_set_property(b->gain, "fade-gain", b->fade_gain);
}

/* @0x60125808: only "mute-output" is forwarded here. outctrlbin_init sets mute-input
 * from the same flag. */
int outctrlbin_set_param_mute_output(struct outctrlbin *b, const char *key, const char *val)
{
    (void)key;
    b->mute_output = _Stoul(val, NULL, 0) != 0;
    return outctrlbin_set_property(b->gain, "mute-output", b->mute_output);
}

/* @0x60125888 */
int outctrlbin_set_param_delay(struct outctrlbin *b, const char *key, const char *val)
{
    (void)key;
    b->delay_ms = (int)_Stoul(val, NULL, 0);
    return outctrlbin_set_property(b->delay, "delay", b->delay_ms);
}

/* @0x60125900 */
void __aud_object_register_outctrlbin(void)
{
    aud_object_class_register(outctrlbin_class_object);
}

struct aud_object_class_hdr {
    const struct aud_object_class_hdr *parent;  /* +0x00 */
    int    parent_size;                         /* +0x04 */
    int  (*constructor)(struct outctrlbin *);   /* +0x08 */
    void (*destructor)(struct outctrlbin *);    /* +0x0c */
    int    pad10;
    int  (*event)(struct outctrlbin *, struct aud_port *, struct aud_event *); /* +0x14 */
    int    pad18;
    int  (*prop_updated)(struct outctrlbin *, const struct aud_prop_desc *);   /* +0x1c */
    int    pad20[3];
    int    size;                                /* +0x2c */
    const struct aud_prop_desc *props;          /* +0x30 */
};

/* @0x60125910 */
int aud_object_class_init_outctrlbin(struct aud_object_class_hdr *c)
{
    c->destructor = outctrlbin_destructor;
    c->constructor = outctrlbin_constructor;
    c->props = outctrlbin_props;
    c->event = outctrlbin_event;
    c->prop_updated = outctrlbin_prop_updated;
    c->size = 0x140;
    c->parent_size = *(const int *)((const char *)c->parent + 0x18);
    return 0;
}

/* @0x6012593c */
void __aud_mreg_class_outctrlbin(void)
{
    aud_mregistry_add_class(outctrlbin_class_object);
}
