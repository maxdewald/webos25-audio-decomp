#include <assert.h>
#include <stdio.h>
#include "../../../src/dsp2/event.c"

static int fail_after=-1;
static unsigned active, saves, restores, backtraces;
static int failing(void)
{
    if(fail_after<0) return 0;
    if(fail_after==0) return 1;
    --fail_after; return 0;
}
void *event_calloc(size_t a,size_t b)
{
    if(failing()) return NULL;
    void *p=calloc(a,b); if(p) ++active; return p;
}
void *event_malloc(size_t size)
{
    if(failing()) return NULL;
    void *p=malloc(size?size:1); if(p) ++active; return p;
}
char *event_strdup(const char *s)
{
    if(failing()) return NULL;
    size_t n=strlen(s)+1;
    char *p=malloc(n); assert(p); memcpy(p,s,n); ++active; return p;
}
void event_free(void *p) { if(p) { assert(active); --active; free(p); } }
uint32_t event_irq_save(void) { ++saves; return 0x123; }
void event_irq_restore(uint32_t ps) { assert(ps==0x123); ++restores; }
void dump_backtrace(int v) { assert(v==0); ++backtraces; }
char *audutil_append_printf(char *text,unsigned *capacity,const char *format,...)
{
    va_list args,copy; va_start(args,format); va_copy(copy,args);
    int extra=vsnprintf(NULL,0,format,copy); va_end(copy); assert(extra>=0);
    size_t old=text?strlen(text):0;
    text=realloc(text,old+(unsigned)extra+1); assert(text);
    vsnprintf(text+old,(unsigned)extra+1,format,args); va_end(args);
    *capacity=(unsigned)(old+(unsigned)extra+1); return text;
}

static void test_values(void)
{
    unsigned char payload[]={9,8,7};
    struct aud_event *e=aud_event_new(payload,sizeof(payload)); assert(e);
    assert(e->object.references==1 && e->object.magic==0x65766e74);
    assert(e->payload_size==3 && !memcmp(e->payload,payload,3));
    e->type=1; e->flags=3;
    assert(aud_event_value_set_int(e,"es",1)==0);
    assert(aud_event_value_get_bool(e,"es",7)==7);
    assert(aud_event_value_set_bool(e,"es",0x80)==0);
    assert(aud_event_value_get_bool(e,"es",0)==0x80);
    assert(aud_event_value_get_int(e,"es",-1)==-1);
    assert(aud_event_value_set_str(e,"codec","mat")==0);
    assert(aud_event_value_set_mem(e,"private",payload,3)==0);
    assert(aud_event_value_set_fraction(e,"ratio",5,0)==0);
    int32_t num=-1,den=-1;
    assert(aud_event_value_get_fraction(e,"ratio",&num,&den)==0 && num==5 && den==0);
    unsigned size=99;
    assert(!memcmp(aud_event_value_get_mem(e,"private",&size),payload,3) && size==3);
    size=99; assert(!aud_event_value_get_mem(e,"codec",&size) && size==99);
    assert(aud_event_has_name(e,"codec") && !aud_event_has_name(e,"absent"));
    struct aud_event *copy=aud_event_dup(e); assert(copy && copy!=e);
    assert(copy->type==1 && copy->flags==3 && !memcmp(copy->payload,payload,3));
    const char *orig=aud_event_value_get_str(e,"codec",NULL);
    const char *dup=aud_event_value_get_str(copy,"codec",NULL);
    assert(orig!=dup && !strcmp(dup,"mat"));
    assert(aud_event_value_get_bool(copy,"es",0)==0x80);
    void *origmem=aud_event_value_get_mem(e,"private",&size);
    void *dupmem=aud_event_value_get_mem(copy,"private",&size);
    assert(origmem!=dupmem && !memcmp(origmem,dupmem,3));
    aud_event_value_del(copy,"codec"); assert(!aud_event_has_name(copy,"codec"));
    assert(!strcmp(aud_event_value_get_str(e,"codec",NULL),"mat"));
    assert(aud_object_get(e)==e && e->object.references==2);
    aud_object_put(e); assert(e->object.references==1);
    aud_object_put(copy); aud_object_put(e);
    assert(active==0 && saves==restores);
}
static void test_constructors(void)
{
    struct aud_event *(*constructors[])(void)={aud_event_new_first,aud_event_new_last,
        aud_event_new_flush,aud_event_new_flush_async,aud_event_new_start_up,
        aud_event_new_start,aud_event_new_stop,aud_event_new_disconnect,
        aud_event_new_disconnect_sync,aud_event_new_restart,aud_event_new_latency_check,
        aud_event_new_custom};
    unsigned type[]={1,2,7,7,3,3,4,6,6,8,9,13};
    unsigned flags[]={3,3,3,1,0,1,1,1,3,3,3,3};
    for(unsigned i=0;i<sizeof(type)/sizeof(*type);++i) {
        struct aud_event *e=constructors[i](); assert(e && e->type==type[i] && e->flags==flags[i]);
        if(type[i]==8) assert(!strcmp(aud_event_name(e),"unknown"));
        if(type[i]==7) assert(!strcmp(aud_event_name(e),flags[i]==3?"flush (sync)":"flush (async)"));
        aud_object_put(e);
    }
    unsigned char p=3; struct aud_event *mark=aud_event_new_mark(&p,1);
    assert(mark->type==5 && mark->payload[0]==3); aud_object_put(mark);
    struct aud_event *e=aud_event_newv_first("channels",0,8,"es",1,42,"codec",2,"mat",(const char *)NULL);
    assert(e && aud_event_value_get_int(e,"channels",0)==8);
    assert(aud_event_value_get_bool(e,"es",0)==1);
    assert(!strcmp(aud_event_value_get_str(e,"codec",NULL),"mat"));
    char *text=aud_event_str(e);
    assert(!strcmp(text,"first(1), prop 00000003, channels=(int)8, es=(bool)true, codec=(str)mat"));
    free(text);
    struct aud_event *copy=aud_event_dup_and_set(e,"channels",0,2,(const char *)NULL);
    assert(aud_event_value_get_int(copy,"channels",0)==2 && aud_event_value_get_int(e,"channels",0)==8);
    aud_object_put(copy);
    aud_event_set_values(e,"bad",4,"ignored",0,1,(const char *)NULL);
    assert(!aud_event_has_name(e,"bad") && !aud_event_has_name(e,"ignored"));
    aud_object_put(e); assert(!active);
}
static uint32_t read_word(const unsigned char *p)
{
    return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;
}
static void test_wire(void)
{
    const unsigned char payload[]={1,2,3};
    struct aud_event *e=aud_event_new_mark(payload,3);
    aud_event_value_set_int(e,"rate",192000);
    aud_event_value_set_bool(e,"es",0x80);
    aud_event_value_set_str(e,"codec","mat");
    aud_event_value_set_mem(e,"blob",payload,3);
    aud_event_value_set_fraction(e,"fraction",7,3);
    unsigned size=0;
    unsigned char *wire=aud_event_serialize(e,&size); assert(wire && size>24);
    assert(read_word(wire)==0x65766e74 && read_word(wire+4)==5);
    assert(read_word(wire+8)==3 && read_word(wire+12)==5 && read_word(wire+16)==3);
    assert(!memcmp(wire+20,payload,3));
    unsigned off=24;
    const char *keys[]={"rate","es","codec","blob","fraction"};
    unsigned expected_size[]={4,4,4,3,8};
    for(unsigned i=0;i<5;++i) {
        unsigned keybytes=read_word(wire+off);
        assert(keybytes%4==0 && !strcmp((char *)wire+off+4,keys[i]));
        off+=4+keybytes;
        assert(read_word(wire+off)==i && read_word(wire+off+4)==expected_size[i]);
        off+=8;
        if(i==0) assert(read_word(wire+off)==192000);
        if(i==1) assert(read_word(wire+off)==0x80);
        if(i==2) assert(!strcmp((char *)wire+off,"mat"));
        if(i==3) assert(!memcmp(wire+off,payload,3));
        if(i==4) assert(read_word(wire+off)==7 && read_word(wire+off+4)==3);
        off+=wire_align(expected_size[i]);
    }
    assert(off==size);
    struct aud_event *decoded=aud_event_deserialize(wire,size); assert(decoded);
    assert(decoded->type==5 && decoded->flags==3 && decoded->payload_size==3);
    assert(!memcmp(decoded->payload,payload,3));
    assert(aud_event_value_get_int(decoded,"rate",0)==192000);
    assert(aud_event_value_get_bool(decoded,"es",0)==1);
    assert(!strcmp(aud_event_value_get_str(decoded,"codec",NULL),"mat"));
    int32_t num=0,den=0;
    assert(aud_event_value_get_fraction(decoded,"fraction",&num,&den)==0 && num==7 && den==3);
    aud_object_put(decoded);
    wire_word(wire,0); decoded=aud_event_deserialize(wire,size); assert(decoded); aud_object_put(decoded);
    assert(aud_event_deserialize(wire,19)==NULL);
    assert(aud_event_deserialize(wire,22)==NULL);
    /* Backing storage remains full-sized: verify observed failure branches,
     * never exercise the original decoder's unsafe pre-check word reads. */
    uint32_t keyspace=read_word(wire+24);
    memset(wire+28,'x',keyspace);
    assert(aud_event_deserialize(wire,size)==NULL);
    memcpy(wire+28,"rate",5);
    wire_word(wire+28+keyspace,99);
    assert(aud_event_deserialize(wire,size)==NULL);
    wire_word(wire+28+keyspace,0);
    fail_after=0; assert(aud_event_deserialize(wire,size)==NULL); fail_after=-1;
    event_free(wire);
    size=123; fail_after=0; assert(!aud_event_serialize(e,&size) && size==123); fail_after=-1;
    aud_object_put(e); assert(active==0);
}
static void test_failures(void)
{
    fail_after=0; assert(!aud_event_new_first()); fail_after=-1;
    struct aud_event *e=aud_event_newv_first("channels",0,8,"rate",0,192000,(const char *)NULL);
    for(int n=0;n<5;++n) {
        fail_after=n; struct aud_event *copy=aud_event_dup(e); fail_after=-1;
        if(n==0) assert(!copy);
        else {
            assert(copy && copy->type==1 && copy->flags==3);
            if(n==1 || n==2) assert(!aud_event_has_name(copy,"channels"));
            if(n==3 || n==4) assert(aud_event_value_get_int(copy,"channels",0)==8);
            assert(!aud_event_has_name(copy,"rate"));
            aud_object_put(copy);
        }
    }
    aud_object_put(e);
    e=aud_event_new(NULL,0);
    aud_event_value_set_int(e,"codec",99);
    fail_after=0; assert(aud_event_value_set_str(e,"codec","mat")==0); fail_after=-1;
    assert(aud_event_has_name(e,"codec") && aud_event_value_get_str(e,"codec","fallback")==NULL);
    aud_object_put(e); assert(!active);
    struct aud_object bad={.references=0};
    aud_object_put(&bad); assert(backtraces==1 && bad.references==-1);
    assert(aud_event_value_get_int(NULL,"a",42)==42);
    assert(aud_event_value_set_int(NULL,"a",42)==-1);
    assert(check_str_term("abc",3)==-1 && check_str_term("abc",4)==0 && check_str_term("",0)==-1);
}
int main(void)
{
    test_values(); test_constructors(); test_wire(); test_failures();
    puts("PASS events: strict typed values, deep-copy/partial failure, FIRST flags and ownership");
    return 0;
}
