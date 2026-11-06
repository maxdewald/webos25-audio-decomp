#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../../../src/dsp2/srcbin.c"

static int destroyed, constructed, events, pending, wired_in, wired_out, propagate;
static bool requested_dolby, fail_create;
static const char *selected;
static int converter;
static struct aud_event *received;
void *srcbin_class_descriptor;
bool aud_event_value_get_bool(struct aud_event *e, const char *key, bool fallback)
{
    (void)e; assert(!strcmp(key,"dolby") && !fallback); return requested_dolby;
}
void *aud_module_bin_add(struct srcbin *b, const char *name)
{
    (void)b; selected=name; return fail_create ? NULL : &converter;
}
int aud_module_bin_set_input(struct srcbin *b,int p,void *c,int cp)
{
    (void)b; assert(p==0 && cp==0 && c==&converter); ++wired_in; return 0;
}
int aud_module_bin_set_output(struct srcbin *b,int p,void *c,int cp)
{
    (void)b; assert(p==0 && cp==0 && c==&converter); ++wired_out; return 0;
}
int aud_module_bin_add_cmd_propagate(struct srcbin *b,void *c)
{
    (void)b; assert(c==&converter); ++propagate; return 0;
}
int aud_module_bin_propagate_pending(struct srcbin *b)
{
    assert(b->converter==NULL); ++pending; return 0;
}
void aud_object_class_register(void *c) { (void)c; }
void aud_mregistry_add_class(void *c) { (void)c; }
static int parent_constructor(struct srcbin *b) { (void)b; ++constructed; return -3; }
static void parent_destructor(struct srcbin *b) { b->converter=NULL; ++destroyed; }
static int parent_event(struct srcbin *b,struct aud_port *p,struct aud_event *e)
{
    (void)b; (void)p; received=e; ++events; return 37;
}
int main(void)
{
    struct srcbin_parent parent={parent_constructor,parent_destructor,parent_event};
    struct srcbin b={.parent=&parent};
    struct aud_event first={1}, last={2};
    struct aud_port *port=(struct aud_port *)&converter;
    assert(srcbin_constructor(&b)==0 && constructed==1 && !b.dolby && !b.converter);
    assert(srcbin_event(&b,port,&first)==37 && !strcmp(selected,"pcmcvt"));
    assert(received==&first && pending==1 && b.converter==&converter && !b.dolby);
    assert(wired_in==1 && wired_out==1 && propagate==1);
    srcbin_event(&b,port,&first); assert(pending==1);
    requested_dolby=true;
    srcbin_event(&b,port,&first);
    assert(!strcmp(selected,"mssrc") && b.dolby && destroyed==1 && pending==2);
    requested_dolby=false; fail_create=true;
    int count=events;
    srcbin_event(&b,port,&first);
    assert(events==count+1 && received==&first && b.dolby && destroyed==2);
    fail_create=false;
    srcbin_event(&b,NULL,&first); assert(pending==2);
    srcbin_event(&b,port,&last); assert(pending==2);
    struct srcbin_property prop={"out-bits-per-sample",2,0};
    assert(srcbin_prop_updated(&b,&prop)==0 && pending==2);
    srcbin_event(&b,port,&first); assert(pending==3 && !b.dolby);
    srcbin_destructor(&b); assert(!b.converter && destroyed==3);
    struct srcbin_class_descriptor descriptor={.parent_class=&parent};
    struct srcbin_class cls={.descriptor=&descriptor};
    assert(aud_object_class_init_srcbin(&cls)==0 && cls.instance_size==0x120);
    assert(cls.parent_class==&parent);
    assert(cls.event==srcbin_event && cls.prop_updated==srcbin_prop_updated);
    puts("PASS srcbin selection, pending-command ordering, unchanged FIRST and property no-op");
    return 0;
}
