/* Host semantics of DSP1 output control bin; not DSP execution. */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../../src/dsp1/outctrlbin.c"

char outctrlbin_class_object[1];
const struct aud_prop_desc outctrlbin_props[]={{NULL,0,0}};
static int channel=8,es=1,creates,destroys,parent_events,pending,fail_create;
static int child[4],next_child;
static char factories[4][16];
struct prop_record { void *child; char text[80]; };
static struct prop_record records[32];static unsigned nrecords;
static int ctor(struct outctrlbin *b) { (void)b;return 0; }
static void dtor(struct outctrlbin *b)
{
    ++destroys;b->gain=b->delay=b->pcmcvt=NULL;
}
static int event(struct outctrlbin *b,struct aud_port *p,struct aud_event *e)
{
    (void)b;(void)p;(void)e;++parent_events;return 19;
}
char *audutil_strdup_printf(const char *fmt,...)
{
    char tmp[80];va_list args;va_start(args,fmt);int n=vsnprintf(tmp,sizeof(tmp),fmt,args);va_end(args);
    assert(n>=0 && (unsigned)n<sizeof(tmp));char *s=malloc((unsigned)n+1);assert(s);memcpy(s,tmp,(unsigned)n+1);return s;
}
int aud_module_set_prop(void *c,const char *kv,size_t length)
{
    assert(strlen(kv)==length && nrecords<32);
    records[nrecords].child=c;snprintf(records[nrecords++].text,80,"%s",kv);return 0;
}
void *aud_module_bin_add(void *b,const char *factory)
{
    (void)b;++creates;if(fail_create) return NULL;
    assert(next_child<4);snprintf(factories[next_child],16,"%s",factory);return &child[next_child++];
}
int aud_module_bin_connect(void *b,void *s,int sp,void *d,int dp)
{
    (void)b;assert(s && d && !sp && !dp);return 0;
}
int aud_module_bin_set_input(void *b,int p,void *m,int mp) { (void)b;assert(!p && m && !mp);return 0; }
int aud_module_bin_set_output(void *b,int p,void *m,int mp) { (void)b;assert(!p && m && !mp);return 0; }
int aud_module_bin_add_cmd_propagate(void *b,void *m) { (void)b;assert(m);return 0; }
int aud_module_bin_propagate_pending(void *b) { (void)b;++pending;return 0; }
int aud_event_value_get_int(struct aud_event *e,const char *k,int def) { (void)e;assert(!strcmp(k,"channels") && def==2);return channel; }
bool aud_event_value_get_bool(struct aud_event *e,const char *k,bool def) { (void)e;assert(!strcmp(k,"es") && !def);return es!=0; }
char *aud_event_str(struct aud_event *e) { (void)e;return audutil_strdup_printf("first"); }
int aud_object_class_register(void *c) { assert(c==outctrlbin_class_object);return 0; }
int aud_mregistry_add_class(void *c) { assert(c==outctrlbin_class_object);return 0; }
unsigned long _Stoul(const char *s,char **end,int base) { return strtoul(s,end,base); }
static void reset_counters(void) { next_child=0;nrecords=0;memset(factories,0,sizeof(factories)); }
int main(void)
{
    struct outctrlbin_class cls={ctor,dtor,event};
    struct outctrlbin b={.cls=&cls};
    outctrlbin_constructor(&b);
    assert(b.mute_output && b.gain_value==-1 && b.fade_gain==-1 && b.delay_ms==-1);
    struct aud_port port={0};struct aud_event first={1};
    assert(outctrlbin_event(&b,&port,&first)==19 && creates==2 && pending==1);
    assert(!b.pcmcvt && !strcmp(factories[0],"gain") && !strcmp(factories[1],"delay"));
    assert(nrecords==0); /* unset gain does not apply either mute property */
    outctrlbin_event(&b,&port,&first);assert(creates==2 && pending==2);
    outctrlbin_set_param_mute_output(&b,NULL,"0");
    assert(!b.mute_output && nrecords==1 && !strcmp(records[0].text,"mute-output=0"));
    reset_counters();channel=10;es=0;
    outctrlbin_event(&b,&port,&first);
    assert(destroys==1 && creates==5 && b.pcmcvt && !strcmp(factories[0],"pcmcvt"));
    assert(nrecords==1 && !strcmp(records[0].text,"out-channels=8"));
    reset_counters();b.gain_value=0x800000;b.fade_gain=0x400000;b.delay_ms=10;es=1;
    outctrlbin_event(&b,&port,&first);
    assert(!b.pcmcvt && nrecords==5);
    assert(!strcmp(records[0].text,"gain=8388608"));
    assert(!strcmp(records[1].text,"fade-gain=4194304"));
    assert(!strcmp(records[2].text,"mute-input=0"));
    assert(!strcmp(records[3].text,"mute-output=0"));
    assert(!strcmp(records[4].text,"delay=10"));
    int before=creates;
    outctrlbin_event(&b,NULL,&first);assert(creates==before);
    fail_create=1;channel=8;
    assert(outctrlbin_event(&b,&port,&first)==19 && b.channels==8 && parent_events>0);
    fail_create=0;outctrlbin_destructor(&b);assert(!b.gain && !b.delay && !b.pcmcvt);
    puts("PASS outctrlbin: ES excludes converter, FIRST rebuild/order, cached mute and property propagation");
    return 0;
}
