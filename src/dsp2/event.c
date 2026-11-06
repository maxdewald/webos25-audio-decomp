/* C2 DSP2 event objects, VA 0x60069988..0x6006a808 plus check_str_term.
 * Evidence: saved main image and aligned scalar listings. Semantic host models,
 * not ABI overlays. Diagnostic logging/caller-address stamps are omitted.
 * The export misnames the destructor @600699f8 as a second aud_event_new.
 * Typed properties are strictly typed: int es=1 is NOT bool es=true.
 */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct aud_object {
    int32_t references;                   /* +0x00 */
    uint32_t magic;                       /* +0x04 */
    void (*destroy)(void *);              /* +0x08 */
};
struct event_value {
    struct event_value *prev, *next;      /* +0x00/+0x04 */
    char *name;                          /* +0x08 */
    unsigned type;                       /* +0x0c: int/bool/str/mem/fraction */
    union { int32_t integer; uint8_t boolean; char *string; void *memory;
            struct { int32_t numerator, denominator; } fraction; } value; /* +0x10 */
    void *owned;                         /* +0x18 */
    uint32_t size;                       /* +0x1c */
};
struct aud_event {
    struct aud_object object;
    unsigned type, flags;                /* +0x0c/+0x10 */
    struct event_value values;           /* sentinel pointers +0x14/+0x18 */
    unsigned payload_size;               /* +0x20 */
    unsigned char payload[];             /* +0x24 */
};
extern uint32_t event_irq_save(void);     /* rsil(3), saved PS at 0x6005cdd4 */
extern void event_irq_restore(uint32_t); /* wsr PS / rsync */
extern void dump_backtrace(int);
/* Allocator aliases allow tests to inject firmware allocation failures. */
extern void *event_calloc(size_t, size_t);
extern void *event_malloc(size_t);
extern void event_free(void *);
extern char *event_strdup(const char *);
extern char *audutil_append_printf(char *,unsigned *,const char *,...);

static void append_value(struct aud_event *e, struct event_value *v)
{
    v->prev=e->values.prev;
    v->next=&e->values;
    e->values.prev->next=v;
    e->values.prev=v;
}
static void unlink_value(struct event_value *v)
{
    if (v->prev != NULL) {
        v->prev->next=v->next;
        v->next->prev=v->prev;
    }
}
static struct event_value *find_value(const struct aud_event *e, const char *key)
{
    if (e != NULL)
        for (struct event_value *v=e->values.next; v!=&e->values; v=v->next)
            if (strcmp(v->name,key)==0) return v;
    return NULL;
}
static struct event_value *get_value(struct aud_event *e, const char *key)
{
    if (e == NULL) return NULL;
    struct event_value *v=find_value(e,key);
    if (v != NULL) return v;
    v=event_calloc(1,sizeof(*v));
    if (v == NULL) return NULL;
    append_value(e,v);
    /* Firmware checks node, not strdup result: failed name allocation leaves
     * a linked nameless entry. It is not silently repaired here. */
    v->name=event_strdup(key);
    return v;
}

/* @0x60069988: no NULL guard; IRQ exclusion, not a host thread-safe atomic. */
void *aud_object_get(void *object)
{
    struct aud_object *o=object;
    uint32_t ps=event_irq_save();
    o->references=(int32_t)((uint32_t)o->references+1u);
    event_irq_restore(ps);
    return object;
}
/* @0x600699a4: zero-before-decrement logs/backtraces, does not call destroy. */
void aud_object_put(void *object)
{
    struct aud_object *o=object;
    uint32_t ps=event_irq_save();
    int32_t old=o->references;
    o->references=(int32_t)((uint32_t)old-1u);
    int32_t remaining=o->references;
    event_irq_restore(ps);
    if (old==0) dump_backtrace(0);
    if (remaining==0) o->destroy(object);
}
/* @0x600699f8: owned allocation is freed iff size!=0, unlike explicit deletion.
 * Magic/destroy poison values are written immediately before freeing event. */
void aud_event_destroy(void *object)
{
    struct aud_event *e=object;
    while (e->values.next!=&e->values) {
        struct event_value *v=e->values.next;
        if (v->name!=NULL) event_free(v->name);
        if (v->size!=0) event_free(v->owned);
        unlink_value(v);
        event_free(v);
    }
    e->object.magic=0x0eee0eee;
    /* Target additionally poisons +8 with 0x0ee00ee0; no invalid host fnptr. */
    event_free(e);
}
/* @0x60069a64: payload is copied, properties initially empty. */
struct aud_event *aud_event_new(const void *payload, unsigned bytes)
{
    struct aud_event *e=event_calloc(1,sizeof(*e)+bytes);
    if (e==NULL) return NULL;
    e->object.references=1;
    e->object.magic=0x65766e74;
    e->object.destroy=aud_event_destroy;
    e->payload_size=bytes;
    if (bytes!=0) memcpy(e->payload,payload,bytes);
    e->values.prev=e->values.next=&e->values;
    return e;
}

/* @0x6006a36c: replacement does NOT release prior owned storage. */
int aud_event_value_set_int(struct aud_event *e,const char *key,int32_t value)
{
    struct event_value *v=get_value(e,key);
    if (!v) return -1;
    v->value.integer=value; v->type=0; return 0;
}
/* @0x6006a3e8: raw byte, no boolean normalization in this direct setter. */
int aud_event_value_set_bool(struct aud_event *e,const char *key,uint8_t value)
{
    struct event_value *v=get_value(e,key);
    if (!v) return -1;
    v->value.boolean=value; v->type=1; return 0;
}
/* @0x6006a468: failed value strdup still returns success; size stays old/zero.
 * Replacing an allocated string/mem overwrites owned without freeing it, as in
 * firmware. Tests avoid leaking by deleting allocated values before replacement. */
int aud_event_value_set_str(struct aud_event *e,const char *key,const char *value)
{
    struct event_value *v=get_value(e,key);
    if (!v) return -1;
    v->type=2;
    v->value.string=event_strdup(value);
    v->owned=v->value.string;
    if (v->owned!=NULL) v->size=(unsigned)strlen(v->owned)+1;
    return 0;
}
/* @0x6006a4fc: failed payload allocation also returns 0. */
int aud_event_value_set_mem(struct aud_event *e,const char *key,const void *value,unsigned size)
{
    struct event_value *v=get_value(e,key);
    if (!v) return -1;
    v->type=3;
    v->value.memory=event_malloc(size);
    v->owned=v->value.memory;
    if (v->owned!=NULL) { memcpy(v->owned,value,size); v->size=size; }
    return 0;
}
/* @0x6006a594 */
int aud_event_value_set_fraction(struct aud_event *e,const char *key,int32_t num,int32_t den)
{
    struct event_value *v=get_value(e,key);
    if (!v) return -1;
    v->value.fraction.numerator=num;
    v->value.fraction.denominator=den;
    v->type=4;
    return 0;
}
/* @0x6006a638 */
int32_t aud_event_value_get_int(const struct aud_event *e,const char *key,int32_t def)
{
    struct event_value *v=find_value(e,key);
    return v!=NULL && v->type==0 ? v->value.integer : def;
}
/* @0x6006a668 */
unsigned aud_event_value_get_bool(const struct aud_event *e,const char *key,unsigned def)
{
    struct event_value *v=find_value(e,key);
    return v!=NULL && v->type==1 ? v->value.boolean : def;
}
/* @0x6006a69c */
const char *aud_event_value_get_str(const struct aud_event *e,const char *key,const char *def)
{
    struct event_value *v=find_value(e,key);
    return v!=NULL && v->type==2 ? v->value.string : def;
}
/* @0x6006a6cc: missing/wrong type leaves *size untouched. */
void *aud_event_value_get_mem(const struct aud_event *e,const char *key,unsigned *size)
{
    struct event_value *v=find_value(e,key);
    if (v==NULL || v->type!=3) return NULL;
    *size=v->size;
    return v->value.memory;
}
/* @0x6006a700: no division/denominator validation. */
int aud_event_value_get_fraction(const struct aud_event *e,const char *key,int32_t *num,int32_t *den)
{
    struct event_value *v=find_value(e,key);
    if (v==NULL || v->type!=4) return -1;
    *num=v->value.fraction.numerator; *den=v->value.fraction.denominator;
    return 0;
}
/* @0x6006a738 */
void aud_event_value_del(struct aud_event *e,const char *key)
{
    struct event_value *v=find_value(e,key);
    if (v==NULL) return;
    unlink_value(v);
    if (v->owned!=NULL) event_free(v->owned);
    event_free(v->name); event_free(v);
}
/* @0x6006a784: checks key existence, not event type or key's value type. */
int aud_event_has_name(const struct aud_event *e,const char *key)
{
    return find_value(e,key)!=NULL;
}

/* @0x60069c60: arbitrary fields copied, not a codec/PCM whitelist. int/bool/
 * fraction copied by value, strings/memory deep-copied, payload copied. A node
 * or key allocation failure returns a PARTIAL event, not NULL; string/memory
 * allocation failure leaves that copied field present but NULL/zero-sized.
 * This qualifies earlier FIRST-preservation claims: they assume allocation
 * success. Fields already copied survive when a later property fails. */
struct aud_event *aud_event_dup(const struct aud_event *source)
{
    struct aud_event *copy=aud_event_new(source->payload,source->payload_size);
    if (copy==NULL) return NULL;
    for (struct event_value *v=source->values.next; v!=&source->values; v=v->next) {
        struct event_value *c=event_calloc(1,sizeof(*c));
        if (c==NULL) break;
        c->name=event_strdup(v->name);
        if (c->name==NULL) { event_free(c); break; }
        c->type=v->type;
        switch(v->type) {
        case 1: c->value.boolean=v->value.boolean; break;
        case 2:
            c->value.string=event_strdup(v->value.string);
            c->owned=c->value.string;
            if (c->owned!=NULL) c->size=(unsigned)strlen(c->owned)+1;
            break;
        case 3:
            c->value.memory=event_malloc(v->size);
            c->owned=c->value.memory;
            if (c->owned!=NULL) { memcpy(c->owned,v->owned,v->size); c->size=v->size; }
            break;
        case 4: c->value.fraction=v->value.fraction; break;
        default: c->value.integer=v->value.integer; break;
        }
        append_value(copy,c);
    }
    copy->type=source->type; copy->flags=source->flags;
    return copy;
}

/* @0x60069adc: portable va_list replaces Xtensa register-window va_arg ABI.
 * Only int/bool/string/mem supported here; fraction type4 stops processing.
 * Ignores setter failures and returns the original event. */
struct aud_event *_aud_event_set_values(struct aud_event *e,const char *key,va_list args)
{
    if (e==NULL) return NULL;
    while(key!=NULL) {
        int type=va_arg(args,int);
        switch(type) {
        case 0: aud_event_value_set_int(e,key,va_arg(args,int)); break;
        case 1: aud_event_value_set_bool(e,key,va_arg(args,int)!=0); break;
        case 2: aud_event_value_set_str(e,key,va_arg(args,const char *)); break;
        case 3: {
            const void *data=va_arg(args,const void *);
            unsigned size=va_arg(args,unsigned);
            aud_event_value_set_mem(e,key,data,size); break;
        }
        default: return e;
        }
        key=va_arg(args,const char *);
    }
    return e;
}
/* @0x6006a614 */
struct aud_event *aud_event_set_values(struct aud_event *e,const char *key,...)
{
    va_list args; va_start(args,key);
    struct aud_event *result=_aud_event_set_values(e,key,args);
    va_end(args); return result;
}
/* @0x60069dd0 */
struct aud_event *aud_event_dup_and_set(const struct aud_event *e,const char *key,...)
{
    struct aud_event *copy=aud_event_dup(e);
    if (copy==NULL) return NULL;
    va_list args; va_start(args,key);
    _aud_event_set_values(copy,key,args);
    va_end(args); return copy;
}

static struct aud_event *new_typed(unsigned type,unsigned flags,const void *payload,unsigned size)
{
    struct aud_event *e=aud_event_new(payload,size);
    if (e!=NULL) { e->type=type; e->flags|=flags; }
    return e;
}
/* @0x60069e20 */
struct aud_event *aud_event_new_first(void)
{
    return new_typed(1,3,NULL,0);
}
/* @0x60069e78 */
struct aud_event *aud_event_newv_first(const char *key,...)
{
    struct aud_event *e=aud_event_new_first();
    va_list args; va_start(args,key); _aud_event_set_values(e,key,args); va_end(args);
    return e;
}
/* @0x60069ecc */
struct aud_event *aud_event_newv_restart(const char *key,...)
{
    struct aud_event *e=new_typed(8,3,NULL,0);
    va_list args; va_start(args,key); _aud_event_set_values(e,key,args); va_end(args);
    return e;
}
/* @0x60069f34 */
struct aud_event *aud_event_new_flush(void)
{
    return new_typed(7,3,NULL,0);
}
/* @0x60069f90 */
struct aud_event *aud_event_new_flush_async(void)
{
    return new_typed(7,1,NULL,0);
}
/* @0x60069fe8 */
struct aud_event *aud_event_new_last(void)
{
    return new_typed(2,3,NULL,0);
}
/* @0x6006a040 */
struct aud_event *aud_event_new_start_up(void)
{
    return new_typed(3,0,NULL,0);
}
/* @0x6006a08c */
struct aud_event *aud_event_new_start(void)
{
    struct aud_event *e=aud_event_new_start_up();
    if (e!=NULL) e->flags|=1;
    return e;
}
/* @0x6006a0bc */
struct aud_event *aud_event_new_stop(void)
{
    return new_typed(4,1,NULL,0);
}
/* @0x6006a114 */
struct aud_event *aud_event_new_mark(const void *payload,unsigned size)
{
    return new_typed(5,3,payload,size);
}
/* @0x6006a170 */
struct aud_event *aud_event_new_disconnect(void)
{
    return new_typed(6,1,NULL,0);
}
/* @0x6006a1c8 */
struct aud_event *aud_event_new_disconnect_sync(void)
{
    struct aud_event *e=aud_event_new_disconnect();
    if (e!=NULL) e->flags|=2;
    return e;
}
/* @0x6006a218 */
struct aud_event *aud_event_new_restart(void)
{
    return new_typed(8,3,NULL,0);
}
/* @0x6006a260 */
struct aud_event *aud_event_new_latency_check(void)
{
    return new_typed(9,3,NULL,0);
}
/* @0x6006a2a8 */
struct aud_event *aud_event_new_custom(void)
{
    return new_typed(13,3,NULL,0);
}
/* @0x6006a304 */
struct aud_event *aud_event_newv_custom(const char *key,...)
{
    struct aud_event *e=aud_event_new_custom();
    va_list args; va_start(args,key); _aud_event_set_values(e,key,args); va_end(args);
    return e;
}
/* @0x6006a7b0: restart(8) intentionally has the unknown name. */
const char *aud_event_name(const struct aud_event *e)
{
    switch(e->type) {
    case 0: return "enable/disable";
    case 1: return "first";
    case 2: return "last";
    case 3: return "start";
    case 4: return "stop";
    case 5: return "mark";
    case 6: return "disconnect";
    case 7: return e->flags&2 ? "flush (sync)" : "flush (async)";
    case 9: return "latency check";
    case 13: return "custom";
    default: return "unknown";
    }
}
/* @0x6006a808: insertion-order typed formatting; unknown value types omitted. */
char *aud_event_str2(const struct aud_event *e,int width,const char *separator)
{
    unsigned capacity=0;
    char *text=audutil_append_printf(NULL,&capacity,"%s(%d), prop %08x",
                                   aud_event_name(e),e->type,e->flags);
    for(struct event_value *v=e->values.next;v!=&e->values;v=v->next) {
        switch(v->type) {
        case 0: text=audutil_append_printf(text,&capacity,"%s%*s=(int)%d",
                    separator,width,v->name,v->value.integer); break;
        case 1: text=audutil_append_printf(text,&capacity,"%s%*s=(bool)%s",
                    separator,width,v->name,v->value.boolean?"true":"false"); break;
        case 2: text=audutil_append_printf(text,&capacity,"%s%*s=(str)%s",
                    separator,width,v->name,v->value.string); break;
        case 3: text=audutil_append_printf(text,&capacity,"%s%*s=(mem size)%d",
                    separator,width,v->name,v->size); break;
        case 4: text=audutil_append_printf(text,&capacity,"%s%*s=(fraction)%d/%d",
                    separator,width,v->name,v->value.fraction.numerator,
                    v->value.fraction.denominator); break;
        default: break;
        }
    }
    return text;
}
/* @0x6006a8dc */
char *aud_event_str(const struct aud_event *e)
{
    return aud_event_str2(e,0,", ");
}

static uint32_t wire_align(uint32_t n)
{
    return (n+3u)&~UINT32_C(3);
}
static void wire_word(unsigned char *p,uint32_t word)
{
    for (unsigned i=0;i<4;++i) p[i]=(unsigned char)(word>>(8*i));
}

/* @0x6006a8f0: native LE wire header (magic,type,flags,count,payload_size),
 * aligned payload, then (padded_name_bytes,name,type,value_bytes,value).
 * Names include NUL; bool uses a 4-byte raw byte value, fractions 8 bytes.
 * Padding is uninitialized, not zeroed. Overflow and unknown-property encoding
 * are not sanitized by the original; supported callers use types0..4.
 * Decoder below preserves the target's checks, not a hardened wire parser. */
void *aud_event_serialize(const struct aud_event *e,unsigned *size)
{
    if(e==NULL) return NULL;
    uint32_t bytes=20u+wire_align(e->payload_size), count=0;
    for(struct event_value *v=e->values.next;v!=&e->values;v=v->next) {
        bytes+=4u+wire_align((uint32_t)strlen(v->name)+1u)+8u;
        if(v->type==0 || v->type==1) bytes+=4;
        else if(v->type==2 || v->type==3) bytes+=wire_align(v->size);
        else if(v->type==4) bytes+=8;
        ++count;
    }
    unsigned char *out=event_malloc(bytes);
    if(out==NULL) return NULL;
    *size=bytes;
    wire_word(out,e->object.magic); wire_word(out+4,e->type);
    wire_word(out+8,e->flags); wire_word(out+12,count);
    wire_word(out+16,e->payload_size);
    if(e->payload_size!=0) memcpy(out+20,e->payload,e->payload_size);
    uint32_t offset=20u+wire_align(e->payload_size);
    for(struct event_value *v=e->values.next;v!=&e->values;v=v->next) {
        uint32_t name_bytes=(uint32_t)strlen(v->name)+1u;
        uint32_t name_space=wire_align(name_bytes);
        wire_word(out+offset,name_space);
        memcpy(out+offset+4,v->name,name_bytes);
        offset+=4u+name_space;
        wire_word(out+offset,v->type);
        offset+=4;
        switch(v->type) {
        case 0:
            wire_word(out+offset,4); wire_word(out+offset+4,(uint32_t)v->value.integer);
            offset+=8; break;
        case 1:
            wire_word(out+offset,4); wire_word(out+offset+4,v->value.boolean);
            offset+=8; break;
        case 2: case 3:
            wire_word(out+offset,v->size);
            if(v->size!=0) memcpy(out+offset+4,v->owned,v->size);
            offset+=4u+wire_align(v->size); break;
        case 4:
            wire_word(out+offset,8);
            wire_word(out+offset+4,(uint32_t)v->value.fraction.numerator);
            wire_word(out+offset+8,(uint32_t)v->value.fraction.denominator);
            offset+=12; break;
        default: break;
        }
    }
    return out;
}

/* @0x6006aa60: scans ALL supplied bytes, not only byte zero as Ghidra suggests. */
int check_str_term(const char *s,unsigned size)
{
    for (unsigned i=0;i<size;++i) if(s[i]=='\0') return 0;
    return -1;
}

static uint32_t event_wire_read(const unsigned char *p)
{
    return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;
}
/* @0x6006aa98: five-word header, payload then padded typed properties.
 * No magic validation. Bool normalizes nonzero to1. Setter failures ignored.
 * Preserves target bounds: name check is size >= name_len+word_index*4+4;
 * value check is size >= value_len+type_word_index*4+8. They do NOT establish
 * fixed int/fraction lengths, alignment or overflow safety. Some word reads
 * precede those checks. Call only on an adequately backed buffer; these are
 * evidence of original behavior, NOT a safe decoder for hostile/truncated data.
 * Deserialization failure releases the whole partially constructed event. */
struct aud_event *aud_event_deserialize(const void *buffer,unsigned size)
{
    const unsigned char *wire=buffer;
    if(size<20) return NULL;
    uint32_t payload=event_wire_read(wire+16);
    if(size<payload+20u) return NULL;
    struct aud_event *e=aud_event_new(wire+20,payload);
    if(e==NULL) return NULL;
    uint32_t index=((payload+3u)>>2)+5u;
    e->type=event_wire_read(wire+4); e->flags=event_wire_read(wire+8);
    uint32_t count=event_wire_read(wire+12);
    for(uint32_t n=0;n<count;++n) {
        uint32_t name_length=event_wire_read(wire+index*4u);
        if(size<name_length+index*4u+4u) goto fail;
        const char *key=(const char *)wire+index*4u+4u;
        if(check_str_term(key,name_length)<0) goto fail;
        uint32_t name_end=index+(name_length>>2);
        uint32_t type_index=name_end+1;
        const unsigned char *v=wire+type_index*4u;
        uint32_t value_length=event_wire_read(v+4),type=event_wire_read(v);
        if(size<value_length+type_index*4u+8u) goto fail;
        switch(type) {
        case 0:
            aud_event_value_set_int(e,key,(int32_t)event_wire_read(v+8));
            index=name_end+4; break;
        case 1:
            aud_event_value_set_bool(e,key,event_wire_read(v+8)!=0);
            index=name_end+4; break;
        case 2:
            if(check_str_term((const char *)v+8,value_length)<0) goto fail;
            aud_event_value_set_str(e,key,(const char *)v+8);
            index=type_index+2u+((value_length+3u)>>2); break;
        case 3:
            aud_event_value_set_mem(e,key,v+8,value_length);
            index=type_index+2u+((value_length+3u)>>2); break;
        case 4:
            aud_event_value_set_fraction(e,key,(int32_t)event_wire_read(v+8),
                                            (int32_t)event_wire_read(v+12));
            index=name_end+5; break;
        default: goto fail;
        }
    }
    return e;
fail:
    aud_object_put(e);
    return NULL;
}
