#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../src/dsp2/link.c"
static void *held[520];static unsigned held_count,calls;static int fail_alloc;
void aud_mutex_init(void *m) { (void)m; }
void aud_mutex_lock(void *m)
{
    for(unsigned i=0;i<held_count;++i) assert(held[i]!=m);
    assert(held_count<520);held[held_count++]=m;
}
void aud_mutex_unlock(void *m)
{
    unsigned i=0;while(i<held_count && held[i]!=m) ++i;assert(i<held_count);
    held[i]=held[--held_count];
}
void *aud_link_malloc(unsigned size) { return fail_alloc?NULL:malloc(size); }
void aud_link_free(void *p) { free(p); }
static int visit(void *ctx,void *data)
{
    struct aud_link *node=data;
    assert(node->payload.data==node);
    ++calls;aud_mutex_unlock(node->mutex);
    return ctx!=NULL;
}
int main(void)
{
    struct aud_link root={0},a={0},b={0},c={0};
    aud_link_root_init(&root);
    aud_link_node_init(&a,&a);aud_link_node_init(&b,&b);aud_link_node_init(&c,&c);
    aud_link_add(&root,&a);aud_link_add(&root,&b);aud_link_add_tail(&root,&c);
    assert(root.next==&b && b.next==&a && a.next==&c && c.next==&root);
    assert(root.prev==&c && root.payload.count==3 && root.owner.sequence==3);
    assert(a.serial==1 && b.serial==2 && c.serial==3);
    assert(aud_link_foreach_node(&root,NULL,visit)==0 && calls==3 && held_count==0);
    calls=0;assert(aud_link_foreach_node(&root,&root,visit)==0 && calls==1 && held_count==0);
    aud_link_del(&b);assert(root.next==&a && !b.owner.root && root.payload.count==2);
    assert(b.next==&a);aud_link_del(&a);aud_link_del(&c);
    assert(root.next==&root && root.prev==&root && root.payload.count==0);
    struct aud_link *many=calloc(257,sizeof(*many));assert(many);
    for(unsigned i=0;i<257;++i) { aud_link_node_init(&many[i],&many[i]);aud_link_add_tail(&root,&many[i]); }
    fail_alloc=1;assert(aud_link_foreach_node(&root,NULL,visit)==-1 && !held_count);
    fail_alloc=0;calls=0;assert(aud_link_foreach_node(&root,NULL,visit)==0 && calls==257 && !held_count);
    for(unsigned i=0;i<257;++i) aud_link_del(&many[i]);free(many);
    root.payload.count=1;assert(aud_link_foreach_node(&root,NULL,visit)==-1 && !held_count);
    puts("PASS aud_link: head/tail/order, callback mutex handoff, serial snapshot and heap failure");
    return 0;
}
