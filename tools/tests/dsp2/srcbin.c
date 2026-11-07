#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../src/dsp2/srcbin.c"

#define aud_module_bin_add_cmd_propagate recovered_bin_add_cmd_propagate
#define aud_module_bin_propagate_pending recovered_bin_propagate_pending
// @match src/dsp2/bin.c (?s)struct aud_bin_link \{.*?extern int aud_module_default_command[^;]+;
// @extract src/dsp2/bin.c bin_cmd bin_command aud_module_bin_add_cmd_propagate aud_module_bin_propagate_pending
#undef aud_module_bin_propagate_pending
#undef aud_module_bin_add_cmd_propagate

static int destroyed, constructed, events, pending, wired_in, wired_out, propagate;
static bool requested_dolby, fail_create;
static bool fail_wiring, fail_registration, fail_allocation;
static int child_result;
static int parent_result=37;
static struct srcbin *observed_bin;
static void *command_converter, *first_converter, *cleanup_converter;
static bool command_dolby, first_dolby;
static const char *selected;
static struct aud_bin commands;
static unsigned child_setting;
static unsigned first_setting;
static int child_command(struct aud_bin_module *module, uint32_t command,
                          void *payload, unsigned length)
{
    (void)module;
    assert(command == 0x50040008 && length == 8);
    child_setting = ((unsigned *)payload)[0];
    command_converter = observed_bin->converter;
    command_dolby = observed_bin->dolby;
    return child_result;
}
static struct aud_bin_module converter = {"converter", child_command};
static struct aud_event *received;
void *srcbin_class_descriptor;
void *_BufMonitor_RemoveBufferIdxLocal(unsigned bytes)
{
    if (fail_allocation) {
        fail_allocation = false;
        return NULL;
    }
    return malloc(bytes);
}
void _BufMonitor_RemoveBufferIndex(void *memory) { free(memory); }
int aud_module_cmd(struct aud_bin_module *module, uint32_t command,
                   void *payload, unsigned length)
{
    return module->command(module, command, payload, length);
}
int aud_module_default_command(struct aud_bin *bin, uint32_t command,
                               void *payload, unsigned length)
{
    (void)bin; (void)command; (void)payload; (void)length;
    return -1;
}
bool aud_event_value_get_bool(struct aud_event *e, const char *key, bool fallback)
{
    (void)e; assert(!strcmp(key,"dolby") && !fallback); return requested_dolby;
}
void *aud_module_bin_add(struct srcbin *b, const char *name)
{
    (void)b; selected=name;
    if (fail_create) return NULL;
    child_setting = 0;
    return &converter;
}
int aud_module_bin_set_input(struct srcbin *b,int p,void *c,int cp)
{
    (void)b; assert(p==0 && cp==0 && c==&converter);
    if (fail_wiring) return -7;
    ++wired_in; return 0;
}
int aud_module_bin_set_output(struct srcbin *b,int p,void *c,int cp)
{
    (void)b; assert(p==0 && cp==0 && c==&converter);
    if (fail_wiring) return -8;
    ++wired_out; return 0;
}
int aud_module_bin_add_cmd_propagate(struct srcbin *b,void *c)
{
    (void)b; assert(c==&converter); ++propagate;
    fail_allocation = fail_registration;
    return recovered_bin_add_cmd_propagate(&commands, c);
}
int aud_module_bin_propagate_pending(struct srcbin *b)
{
    (void)b; ++pending;
    return recovered_bin_propagate_pending(&commands);
}
void aud_object_class_register(void *c) { (void)c; }
void aud_mregistry_add_class(void *c) { (void)c; }
static int parent_constructor(struct srcbin *b)
{
    (void)b; ++constructed;
    commands.targets.previous = commands.targets.next = &commands.targets;
    commands.pending.previous = commands.pending.next = &commands.pending;
    return -3;
}
static void parent_destructor(struct srcbin *b)
{
    cleanup_converter=b->converter;
    struct aud_bin_link *link = commands.targets.next;
    while (link != &commands.targets) {
        struct aud_bin_link *next = link->next;
        free(link);
        link = next;
    }
    commands.targets.previous = commands.targets.next = &commands.targets;
    link = commands.pending.next;
    while (link != &commands.pending) {
        struct aud_bin_link *next = link->next;
        free(((struct aud_bin_pending *)link)->payload);
        free(link);
        link = next;
    }
    commands.pending.previous = commands.pending.next = &commands.pending;
    ++destroyed;
}
static int parent_event(struct srcbin *b,struct aud_port *p,struct aud_event *e)
{
    (void)p; received=e; first_setting=child_setting;
    first_converter=b->converter; first_dolby=b->dolby;
    ++events; return parent_result;
}
int main(void)
{
    struct srcbin_parent parent={parent_constructor,parent_destructor,parent_event};
    struct srcbin b={.parent=&parent};
    struct aud_event first={1}, last={2};
    struct aud_port *port=(struct aud_port *)&converter;
    observed_bin=&b;
    assert(srcbin_constructor(&b)==0 && constructed==1 && !b.dolby && !b.converter);
    unsigned gain[] = {123, 0};
    assert(bin_command(&commands, 0x50040008, gain, sizeof(gain)) == -1);
    gain[0] = 456;
    assert(srcbin_event(&b,port,&first)==37 && !strcmp(selected,"pcmcvt"));
    assert(received==&first && pending==1 && b.converter==&converter && !b.dolby);
    assert(first_setting==123);
    assert(wired_in==1 && wired_out==1 && propagate==1);
    assert(bin_command(&commands, 0x50040008, gain, sizeof(gain)) == -1);
    srcbin_event(&b,port,&first); assert(pending==1);
    assert(first_setting==456);
    requested_dolby=true;
    srcbin_event(&b,port,&first);
    assert(!strcmp(selected,"mssrc") && b.dolby && destroyed==1 && pending==2);
    assert(cleanup_converter==&converter);
    assert(first_setting==0);
    requested_dolby=false; fail_create=true;
    int count=events;
    srcbin_event(&b,port,&first);
    assert(events==count+1 && received==&first && b.dolby && destroyed==2);
    assert(b.converter==&converter);
    fail_create=false;
    requested_dolby=true;
    srcbin_event(&b,port,&first);
    assert(pending==2 && commands.targets.next==&commands.targets);
    requested_dolby=false;
    assert(bin_command(&commands, 0x50040008, gain, sizeof(gain)) == -1);
    srcbin_event(&b,NULL,&first); assert(pending==2);
    srcbin_event(&b,port,&last); assert(pending==2);
    struct srcbin_property prop={"out-bits-per-sample",2,0};
    assert(srcbin_prop_updated(&b,&prop)==0 && pending==2);
    srcbin_event(&b,port,&first); assert(pending==3 && !b.dolby);
    assert(first_setting==0 && commands.pending.next==&commands.pending);
    srcbin_destructor(&b); assert(!b.converter && destroyed==4);

    assert(srcbin_constructor(&b)==0);
    requested_dolby=true;
    gain[0]=123;
    assert(bin_command(&commands, 0x50040008, gain, sizeof(gain))==-1);
    gain[0]=456;
    assert(bin_command(&commands, 0x50040008, gain, sizeof(gain))==-1);
    fail_wiring=true; child_result=-9;
    int inputs=wired_in, outputs=wired_out;
    assert(srcbin_event(&b,port,&first)==37);
    assert(wired_in==inputs && wired_out==outputs);
    assert(first_setting==456 && first_converter==&converter && first_dolby);
    assert(command_converter==NULL && !command_dolby);
    assert(commands.pending.next==&commands.pending);
    srcbin_destructor(&b);
    assert(!b.converter && b.dolby);
    assert(cleanup_converter==&converter);
    fail_wiring=false; child_result=0;

    assert(srcbin_constructor(&b)==0);
    assert(bin_command(&commands, 0x50040008, gain, sizeof(gain))==-1);
    fail_registration=true;
    assert(srcbin_event(&b,port,&first)==37);
    assert(b.converter==&converter && b.dolby && first_setting==0);
    assert(commands.targets.next==&commands.targets);
    assert(commands.pending.next==&commands.pending);
    fail_registration=false;
    assert(bin_command(&commands, 0x50040008, gain, sizeof(gain))==-1);
    assert(srcbin_event(&b,port,&first)==37);
    assert(first_setting==0 && commands.targets.next==&commands.targets);
    assert(commands.pending.next!=&commands.pending);
    srcbin_destructor(&b);

    assert(srcbin_constructor(&b)==0 && !b.dolby && !b.converter);
    assert(bin_command(&commands, 0x50040008, gain, sizeof(gain))==-1);
    fail_create=true; parent_result=-11;
    assert(srcbin_event(&b,port,&first)==-11);
    assert(!b.converter && !b.dolby);
    assert(commands.pending.next!=&commands.pending);
    fail_create=false;
    struct aud_event disconnect={6};
    assert(srcbin_event(&b,NULL,&first)==-11 && received==&first);
    assert(srcbin_event(&b,port,&last)==-11 && received==&last);
    assert(srcbin_event(&b,port,&disconnect)==-11 && received==&disconnect);
    assert(!b.converter && !b.dolby && commands.pending.next!=&commands.pending);
    assert(srcbin_event(&b,port,&first)==-11);
    assert(first_setting==456 && b.converter==&converter && b.dolby);
    assert(commands.pending.next==&commands.pending);
    srcbin_destructor(&b);

    struct srcbin_class_descriptor descriptor={.parent_class=&parent};
    struct srcbin_class cls={.descriptor=&descriptor};
    assert(aud_object_class_init_srcbin(&cls)==0 && cls.instance_size==0x120);
    assert(cls.parent_class==&parent);
    assert(cls.event==srcbin_event && cls.prop_updated==srcbin_prop_updated);
    puts("PASS srcbin+bin: FIFO before FIRST, ignored errors, target failure loss, replacement stale state");
    return 0;
}
