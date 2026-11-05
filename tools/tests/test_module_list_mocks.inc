// SPDX-License-Identifier: GPL-2.0-only
/* Support for test_module_list.c; production functions are extracted unchanged. */
static struct aud_mod_impl *mock_output_impl, *mock_input_impl;
static unsigned int next_glue = 0x100;
static void remove_from_processor(struct aud_mod_inst *inst)
{
    ++processor_removals;
    if (inst->impl_list.next) list_del(&inst->impl_list);
}
static int aud_modlist_role_set(struct aud_mod_inst *inst, const char *role)
{ (void)inst; assert(!role); ++roles_cleared; return 0; }
static struct aud_mod_impl *search_modimpl(const char *name, int proc)
{
    struct aud_mod_impl *impl = !strcmp(name,"gluepw") ? mock_output_impl : mock_input_impl;
    assert(impl && impl->processor->proc_id==proc);
    mutex_lock(&impl->lock); return impl;
}
static struct aud_mod_inst *create_inst_on(struct aud_mod_impl *impl, struct aud_mod_owner *owner)
{
    assert(!owner);
    if(fail_glue) return NULL;
    struct aud_mod_inst *i=kzalloc(sizeof(*i)+16,0);
    assert(i); i->mod_impl=impl; i->users.counter=1;
    i->uid=((unsigned int)impl->processor->proc_id<<24) | (0x40<<16) | next_glue++;
    INIT_LIST_HEAD(&i->input_ports); INIT_LIST_HEAD(&i->output_ports);
    strcpy(i->name,"glue"); return i;
}
static void destroy_glue_sync(struct aud_mod_inst *glue, bool sync)
{
    assert(list_empty(&glue->input_ports) && list_empty(&glue->output_ports));
    assert(!glue->owner);
    assert(sync == !fail_shared);
    ++glue_destroyed; kfree(glue);
}
static void destroy_glue(struct aud_mod_inst *glue)
{ kfree(glue->mem_defs); destroy_glue_sync(glue,false); }
static int initialize_shbuf(struct aud_mod_inst *out, struct aud_mod_inst *in, struct jumper *j)
{
    (void)out; (void)in;
    if(fail_shared) return -1;
    j->shared=kzalloc(8,0); j->shared_size=8; j->offset=0x1234;
    j->max_access_size=4; j->aui_count=2; return 0;
}
