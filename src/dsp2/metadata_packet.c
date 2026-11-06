/* C2 DSP2 metadata list -> aud_data packet bridge, saved VA0x6008f298.
 * Scalar branch slots confirmed; no sample or metadata semantic conversion.
 * Host model preserves the command payload bytes but not target pointer ABI. */
#include <stddef.h>
#include <stdint.h>
#include <string.h>
struct metadata_command {
    struct metadata_command *prev,*next; /* target+0,+4 */
    uint32_t command;                    /* +8: 4=put metadata,10=write update */
    /* PUT's 28-byte fixed header: id,offset,type,format,payload_size,flags,... */
    uint32_t header[7];                  /* target+0x0c..0x27 */
    unsigned char payload[];             /* target+0x28 */
};
struct metadata_data { int type; unsigned size; unsigned char *ptr; };
extern struct metadata_data *aud_data_new(unsigned);
extern void aud_meta_data_free(void *);
static void metadata_word(unsigned char *p,uint32_t value)
{
    for(unsigned i=0;i<4;++i) p[i]=(unsigned char)(value>>(8*i));
}
/* @0x6008f298
 * Empty/NULL/nonpositive encoded size returns NULL without consuming commands.
 * Allocation failure retains the list. Success sets type2 and CONSUMES all
 * nodes, including unknown commands (skipped in encoding). PUT4 encodes
 * (4,payload_size+28,raw header/payload); WRITE10 encodes (10,4,samples).
 * The payload length is word at target node+0x1c (header[4]), not aud_data size.
 * No padding, bounds validation, sample processing or FIRST mutation here. */
struct metadata_data *aud_data_new_metadata(struct metadata_command *root)
{
    if(root==NULL || root->next==root) return NULL;
    uint32_t size=0;
    for(struct metadata_command *n=root->next;n!=root;n=n->next) {
        if(n->command==4) size+=n->header[4]+36u;
        else if(n->command==10) size+=12u;
    }
    if((int32_t)size<1) return NULL;
    struct metadata_data *data=aud_data_new(size);
    if(data==NULL) return NULL;
    data->type=2;
    uint32_t offset=0;
    while(root->next!=root) {
        struct metadata_command *n=root->next;
        if(n->command==4) {
            uint32_t bytes=n->header[4]+28u;
            metadata_word(data->ptr+offset,4);
            metadata_word(data->ptr+offset+4,bytes);
            /* Target header is 7 LE words; payload follows contiguous header. */
            for(unsigned word=0;word<7;++word)
                metadata_word(data->ptr+offset+8+word*4,n->header[word]);
            if(n->header[4]!=0) memcpy(data->ptr+offset+36,n->payload,n->header[4]);
            offset+=bytes+8u;
        } else if(n->command==10) {
            metadata_word(data->ptr+offset,10);
            metadata_word(data->ptr+offset+4,4);
            metadata_word(data->ptr+offset+8,n->header[0]);
            offset+=12;
        }
        if(n->prev!=NULL) {
            n->prev->next=n->next;
            n->next->prev=n->prev;
        }
        aud_meta_data_free(n);
    }
    return data;
}
