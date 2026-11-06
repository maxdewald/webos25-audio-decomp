/* Saved C2 DSP2 main image, VA base 0x60000000.
 * Evidence: srcbin 0x600998c8..60099bfc, class @0x60103458.
 * Semantic host types below name firmware offsets, not a host ABI overlay.
 * Diagnostic-only log guards/calls are omitted. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct srcbin;
struct aud_port;
struct aud_event { int type; };
struct srcbin_parent {
    int (*constructor)(struct srcbin *);                 /* class +0x60 */
    void (*destructor)(struct srcbin *);                 /* class +0x64 */
    int (*event)(struct srcbin *, struct aud_port *, struct aud_event *); /* +0x68 */
};
struct srcbin {
    struct srcbin_parent *parent;                        /* instance +0x0c */
    bool dolby;                                         /* +0x118 */
    void *converter;                                    /* +0x11c */
};
struct srcbin_property {
    const char *name;
    unsigned type, offset;
};
struct srcbin_class_descriptor {
    void *parent_class;                                  /* descriptor +0x18 */
};
struct srcbin_class {
    struct srcbin_class_descriptor *descriptor;
    void *parent_class;
    int (*constructor)(struct srcbin *);
    void (*destructor)(struct srcbin *);
    void *chain;
    int (*event)(struct srcbin *, struct aud_port *, struct aud_event *);
    void *command;
    int (*prop_updated)(struct srcbin *, const struct srcbin_property *);
    unsigned instance_size;
};
extern bool aud_event_value_get_bool(struct aud_event *, const char *, bool);
extern void *aud_module_bin_add(struct srcbin *, const char *);
extern int aud_module_bin_set_input(struct srcbin *, int, void *, int);
extern int aud_module_bin_set_output(struct srcbin *, int, void *, int);
extern int aud_module_bin_add_cmd_propagate(struct srcbin *, void *);
extern int aud_module_bin_propagate_pending(struct srcbin *);
extern void aud_object_class_register(void *);
extern void aud_mregistry_add_class(void *);
extern void *srcbin_class_descriptor;                     /* 0x60103458 */

/* @0x600998c8
 * FIRST chooses ONLY on boolean "dolby" (default false): pcmcvt vs mssrc,
 * not es/codec/rate. Changed selection destroys the bin contents through the
 * parent's destructor, creates the selected child, wires input/output port 0,
 * registers command propagation and replays pending commands BEFORE the same
 * FIRST reaches the parent event handler. No FIRST fields are rewritten here.
 * Creation failure still delegates the event; dolby cache is not updated.
 * See calls 0x60099a60/6b/72/77 and delegation 0x60099916. */
int srcbin_event(struct srcbin *bin, struct aud_port *port, struct aud_event *event)
{
    struct srcbin_parent *parent = bin->parent;
    if (port != NULL && event->type == 1) {
        bool dolby = aud_event_value_get_bool(event, "dolby", false);
        if (bin->dolby != dolby || bin->converter == NULL) {
            if (bin->converter != NULL && bin->parent->destructor != NULL)
                bin->parent->destructor(bin);
            void *converter = aud_module_bin_add(bin, dolby ? "mssrc" : "pcmcvt");
            if (converter == NULL)
                return parent->event(bin, port, event);
            aud_module_bin_set_input(bin, 0, converter, 0);
            aud_module_bin_set_output(bin, 0, converter, 0);
            aud_module_bin_add_cmd_propagate(bin, converter);
            aud_module_bin_propagate_pending(bin);
            bin->converter = converter;
        }
        bin->dolby = dolby;
    }
    return parent->event(bin, port, event);
}

/* @0x60099aa4 */
int srcbin_constructor(struct srcbin *bin)
{
    if (bin->parent->constructor != NULL)
        bin->parent->constructor(bin);
    bin->dolby = false;
    bin->converter = NULL;
    return 0;
}

/* @0x60099ae4 */
void srcbin_destructor(struct srcbin *bin)
{
    if (bin->parent->destructor != NULL)
        bin->parent->destructor(bin);
    bin->converter = NULL;
}

/* @0x60099b1c
 * This entire callback only logs integer/bool/string changes and returns 0.
 * It does NOT configure the child or forward properties. Do not confuse it
 * with pending COMMAND replay above. Class @0x60103458 has no own property
 * table; parent bin_command @0x60071290 calls bin_cmd @0x60070ee8 (misnamed
 * aud_module_bin_add in the export), which queues command/length/payload;
 * aud_module_bin_propagate_pending @0x60071e7c invokes child command handlers.
 * This does not establish arbitrary property-name forwarding to the child. */
int srcbin_prop_updated(struct srcbin *bin, const struct srcbin_property *property)
{
    (void)bin;
    (void)property;
    return 0;
}

/* @0x60099bb8 */
void __aud_object_register_srcbin(void)
{
    aud_object_class_register(srcbin_class_descriptor);
}

/* @0x60099bc8 */
int aud_object_class_init_srcbin(struct srcbin_class *cls)
{
    cls->parent_class = cls->descriptor->parent_class;
    cls->constructor = srcbin_constructor;
    cls->destructor = srcbin_destructor;
    cls->event = srcbin_event;
    cls->prop_updated = srcbin_prop_updated;
    cls->instance_size = 0x120;
    return 0;
}

/* @0x60099bf0 */
void __aud_mreg_class_srcbin(void)
{
    aud_mregistry_add_class(srcbin_class_descriptor);
}
