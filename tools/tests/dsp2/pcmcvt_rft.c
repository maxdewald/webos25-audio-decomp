/* Execute the recovered pcmcvt control/data path with the REAL reconstructed
 * RFT, mocking only SRC/framework. This is not execution on Xtensa hardware. */
#define PCMCVT_REAL_RFT
#include "pcmcvt.c"
#include "../../../src/dsp2/rft.c"

int32_t rft_gain_sample_flix(int32_t sample, int32_t gain)
{
    (void)sample; (void)gain;
    assert(!"unrecovered gain must not execute in transparency test");
    return 0;
}
void rft_lfe_filter_flix(struct rft_config *r, int32_t *mono, size_t frames)
{
    (void)r; (void)mono; (void)frames;
    assert(!"unrecovered LFE must not execute in transparency test");
}
int main(void)
{
    /* Suppress unused static test warnings from shared mocked services. */
    (void)command_tests; (void)event_tests; (void)chain_tests;
    for (unsigned i=0;i<4096;++i) input_bytes[i]=(unsigned char)(i*37u+11u);
    for(int bits=16;bits<=32;bits+=16) {
        struct pcmcvt m=fresh();
        RFT_SetGai(m.rft,0,0x800000);
        configure(&m,bits);
        assert(m.src_bypass==1);
        struct aud_port port={.module=&m};
        struct aud_data input={.type=0,.ts=12000,.size=4096,.ptr=input_bytes};
        int before=src_calls;
        Pcmcvt_Chai(&port,&input);
        assert(src_calls==before && last_data->type==0 && last_data->ts==12000);
        const unsigned char *out=last_data->ptr;
        if(bits==16) {
            assert(last_data->size==4096 && !memcmp(out,input_bytes,4096));
        } else {
            assert(last_data->size==8192);
            for(unsigned sample=0;sample<2048;++sample) {
                assert(out[sample*4]==0 && out[sample*4+1]==0);
                assert(out[sample*4+2]==input_bytes[sample*2]);
                assert(out[sample*4+3]==input_bytes[sample*2+1]);
            }
        }
        free(last_data->ptr); free(last_data);
        int drop=dropped;
        input.type=1;
        Pcmcvt_Chai(&port,&input);
        assert(dropped==drop+1);
    }
    puts("PASS real pcmcvt+RFT: 8ch/192kHz 16->16 identical bytes, 16->32 left-aligned, PCM output");
    return 0;
}
