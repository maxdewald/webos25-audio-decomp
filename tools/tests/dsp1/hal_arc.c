#include <assert.h>
#include <stdio.h>
#include "../../../src/dsp1/out_module.h"
static uint32_t aud_regs[0x500/4],arc_regs[0x500/4],aoc_regs[0x500/4];
static uint32_t chip_regs[0x500/4],chip_id[0x500/4],processor_status,irq_clear;
static uint32_t *reg(uint32_t base,unsigned offset)
{
    assert(offset<0x500 && !(offset&3));
    if(base==AUD_REGS) return &aud_regs[offset/4];
    if(base==ARC_REGS) return &arc_regs[offset/4];
    if(base==AOC_REGS) return &aoc_regs[offset/4];
    if(base==CHIP_REGS) return &chip_regs[offset/4];
    assert(base==CHIP_ID);return &chip_id[offset/4];
}
#undef REG
#define REG(base,offset) (*reg(base,offset))
#include "../../../src/dsp1/hal_arc.c"
void xthal_set_intclear(uint32_t mask) { irq_clear=mask; }
void xthal_dcache_region_writeback(void *p,unsigned len) { (void)p;(void)len; }
uint32_t xt_rsr_ps(void) { return processor_status; }
void xt_wsr_ps(uint32_t ps) { processor_status=ps; }
int main(void)
{
    static const struct { int rate; unsigned code; } cases[]={
        {44100,0},{44,0},{32000,3},{22050,4},{24000,6},{88200,8},
        {768000,9},{768,9},{96000,10},{176400,12},{192000,14},{192,14},{48000,2},{12345,2}};
    for(unsigned i=0;i<sizeof(cases)/sizeof(*cases);++i) {
        for(unsigned lane=0;lane<4;++lane) {
            REG(ARC_REGS,0x130+lane*32)=0xa5a50000;
            REG(ARC_REGS,0x134+lane*32)=0xf123456f;
        }
        HAL_SetArcEsMode(cases[i].rate);
        for(unsigned lane=0;lane<4;++lane) {
            assert(REG(ARC_REGS,0x130+lane*32)==0xa5a50002);
            assert(REG(ARC_REGS,0x134+lane*32)==(0x01234560|cases[i].code));
        }
        HAL_SetArcPcmMode();
        for(unsigned lane=0;lane<4;++lane) {
            assert(!(REG(ARC_REGS,0x130+lane*32)&2));
            assert((REG(ARC_REGS,0x134+lane*32)&15)==2);
        }
    }
    REG(ARC_REGS,0x124)=0x100;
    HAL_SetIEC1958ValidBit(1,5);assert(REG(ARC_REGS,0x124)==0x101);
    HAL_SetIEC1958ValidBit(2,5);assert(REG(ARC_REGS,0x124)==0x100);
    HAL_SetMAOCChannelSet_BufferController(8);
    assert(REG(ARC_REGS,0x48)==7 && REG(ARC_REGS,0x4c)==0x54763210);
    HAL_SetMAOCChannelSet_BufferController(2);
    assert(REG(ARC_REGS,0x48)==1 && REG(ARC_REGS,0x4c)==0x10);
    processor_status=0x20;assert(irq_save()==0x20 && processor_status==0x23);
    irq_restore(0x27);assert(irq_save()==0x27 && processor_status==0x27);
    HAL_ClearInterrupt(31);assert(irq_clear==0x80000000u);
    puts("PASS eARC HAL: HBR rate/status lane codes, validity, slot mapping and IRQ state");
    return 0;
}
