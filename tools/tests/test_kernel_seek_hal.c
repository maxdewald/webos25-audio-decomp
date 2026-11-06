/* FRPLL write-order contract. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#define info(...) ((void)0)
#define RET_OK 0
static unsigned gArcSampleRate=48000;
static const char *fields[32]; static unsigned values[32]; static int writes,delay_count;
static unsigned delays[3];
static void write_field(const char *name,unsigned value) { fields[writes]=name;values[writes++]=value; }
#define CTOP_CTRL_O22Ax_RdFL(b,r) ((void)0)
#define CTOP_CTRL_O22Ax_WrFL(b,r) ((void)0)
#define CTOP_CTRL_O22Ax_Wr01(b,r,f,v) write_field(#f,v)
static void udelay(unsigned us) { assert(delay_count<3);delays[delay_count++]=us; }
// @match src/kernel/aud/hal_clock.c (?ms)^struct frpll_fout_settings\n.*?(?=^static struct frpll_fout_settings \*_aud_get_frpll_fout_settings)
// @extract src/kernel/aud/hal_clock.c _aud_get_frpll_fout_settings aud_set_earc_frpll aud_hal_set_arc_sample_rate
static void reset(void) { writes=delay_count=0; }
int main(void) {
    for(size_t i=0;i<9;i++)
        assert(_aud_get_frpll_fout_settings(frpll_fout_table[i].sample_rate)==&frpll_fout_table[i]);
    assert(_aud_get_frpll_fout_settings(12345)==frpll_fout_table);
    assert(aud_hal_set_arc_sample_rate(192000)==0 && gArcSampleRate==192000);
    assert(writes==9 && delay_count==3);
    assert(!strcmp(fields[0],"reg_earc_frpll_pdb") && values[0]==0);
    assert(values[1]==0x83 && values[3]==0x40 && values[5]==0x639d5e);
    assert(!strcmp(fields[6],"reg_earc_frpll_fout_en") && values[6]==1);
    assert(!strcmp(fields[7],"reg_earc_frpll_pdb") && values[7]==1);
    assert(!strcmp(fields[8],"reg_earc_frpll_syncon") && values[8]==1);
    assert(delays[0]==1 && delays[1]==1 && delays[2]==100);
    reset(); assert(aud_hal_set_arc_sample_rate(192000)==0 && writes>0);
    reset(); assert(aud_hal_set_arc_sample_rate(12345)==0 && gArcSampleRate==12345);
    assert(values[1]==0x1ff && values[3]==0x3e && values[5]==0xcaab8a);
    puts("PASS: all FRPLL rates, 48k fallback, unchanged-rate reprogramming, cached request and delay order");
    return 0;
}
