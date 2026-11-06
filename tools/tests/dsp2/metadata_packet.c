#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../src/dsp2/metadata_packet.c"
static int fail_alloc;
static unsigned released;
struct metadata_data *aud_data_new(unsigned size)
{
    if(fail_alloc) return NULL;
    struct metadata_data *d=calloc(1,sizeof(*d)); assert(d);
    d->size=size; d->ptr=malloc(size); assert(d->ptr); return d;
}
void aud_meta_data_free(void *p) { ++released; free(p); }
static void append(struct metadata_command *root,unsigned command,unsigned payload)
{
    struct metadata_command *n=calloc(1,sizeof(*n)+payload); assert(n);
    n->command=command; n->header[0]=123; n->header[4]=payload;
    for(unsigned i=0;i<payload;++i) n->payload[i]=(unsigned char)(i+1);
    n->prev=root->prev;n->next=root;root->prev->next=n;root->prev=n;
}
static uint32_t word(const unsigned char *p)
{
    return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;
}
int main(void)
{
    struct metadata_command root={0}; root.prev=root.next=&root;
    assert(!aud_data_new_metadata(NULL) && !aud_data_new_metadata(&root));
    append(&root,4,3); append(&root,99,0); append(&root,10,0);
    fail_alloc=1; assert(!aud_data_new_metadata(&root) && !released && root.next!=&root);
    fail_alloc=0;
    struct metadata_data *d=aud_data_new_metadata(&root);
    assert(d && d->type==2 && d->size==51 && released==3);
    assert(root.next==&root && root.prev==&root);
    assert(word(d->ptr)==4 && word(d->ptr+4)==31 && word(d->ptr+8)==123);
    assert(word(d->ptr+24)==3 && d->ptr[36]==1 && d->ptr[38]==3);
    assert(word(d->ptr+39)==10 && word(d->ptr+43)==4 && word(d->ptr+47)==123);
    free(d->ptr);free(d);
    append(&root,99,0); assert(!aud_data_new_metadata(&root) && released==3);
    struct metadata_command *unknown=root.next;root.next=root.prev=&root;free(unknown);
    puts("PASS metadata packet: exact command wire bytes, ownership, empty/unknown/failure paths");
    return 0;
}
