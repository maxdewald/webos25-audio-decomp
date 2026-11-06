/* Recovered wrapper + fast engine + real scalar I/O, without identity mocks.
 * FIR/lowpass/coefficient math remains external and is excluded by these cases. */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../src/dsp2/lgsrc.c"
#include "../../../src/dsp2/lgsrc_engine.c"
#include "../../../src/dsp2/lgsrc_io.c"

void *aud_malloc(unsigned size) { void *p=malloc(size); assert(p); return p; }
void aud_free(void *p) { free(p); }
void AuiLogger_Init(void *p,int byte,unsigned size) { memset(p,byte,size); }
void lgsrcSinctable_makeTable(void) { }
uint32_t lgsrc_engine_fixunsdfsi(double n)
{
    assert(isfinite(n) && n>=0 && n<4294967296.0); return (uint32_t)n;
}
int32_t lgsrc_engine_fixdfsi(double n)
{
    assert(isfinite(n) && n>=INT32_MIN && n<=INT32_MAX); return (int32_t)n;
}
void lgsrcFast_init_coefficients_FLIX(struct lgsrc_state *s,uint32_t table)
{
    (void)s; assert(table==0x601039b0);
}
int lgsrcFast_lowpass_filter(struct lgsrc_state *s)
{
    (void)s; assert(!"lowpass must not run at equal input/output rates"); return -1;
}
int16_t lgsrcNormal_sample_FLIX(struct lgsrc_state *s,uint32_t ch,uint32_t phase,int down)
{
    (void)s;(void)ch;(void)phase;(void)down;
    assert(!"normal filter is not part of this fast equal-rate test"); return 0;
}
uint32_t lgsrc_engine_zero_channel_register(struct lgsrc_state *s,int fast)
{
    (void)s;(void)fast; assert(!"nonzero channels required"); return 0;
}
int main(void)
{
    unsigned char input[266*8*4],output[256*8*4];
    for(unsigned bits=16;bits<=32;bits+=16) {
        struct lgsrc *h=lgsrc_create();
        struct lgsrc_state *s=h->state;
        s->input_fs=s->output_fs=192000;
        s->input_channels=s->output_channels=8;
        s->input_bits=s->output_bits=bits;
        s->output_samples=256; s->run_mode=0;
        assert(lgsrc_init(h)==0 && s->input_samples==276 && s->buffered_samples==10);
        unsigned bytes=bits/8;
        for(unsigned sample=0;sample<266*8;++sample) {
            unsigned char *p=input+sample*bytes;
            for(unsigned j=0;j<bytes;++j) p[j]=(unsigned char)(sample*37u+j*19u+11u);
        }
        memset(output,0xa5,sizeof(output));
        assert(lgsrc_process(h,input,output)==256*8);
        assert(s->produced_samples==256 && s->buffered_samples==20);
        if(bits==16) assert(!memcmp(input,output,256*8*2));
        else {
            for(unsigned sample=0;sample<256*8;++sample) {
                assert(output[sample*4]==0 && output[sample*4+1]==0);
                assert(output[sample*4+2]==input[sample*4+2]);
                assert(output[sample*4+3]==input[sample*4+3]);
            }
            assert(memcmp(input,output,256*8*4)!=0);
        }
        lgsrc_Destroy(h);
    }
    puts("PASS real lgsrc fast pipeline: 8ch/192k equal-rate 16-bit identity, 32-bit low16 lost");
    return 0;
}
