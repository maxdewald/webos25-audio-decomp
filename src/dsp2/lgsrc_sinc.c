/* DSP2 shared sinc layout. ROM contents remain external, not reconstructed data. */
#include <stdint.h>

extern const int16_t *lgsrc_sinc_rom_60103a50; /* 5120 halfwords */
extern int16_t *lgsrc_sinc_table_60168490;     /* 10240 halfwords */

/* @0x6009f650: rearranges 20 * 256 ROM samples into 256 rows of 40 taps.
 * There is no sinc evaluation or rounding: only signed halfword copies.
 * The actual return is at 0x6009f7cb; 0x6009f7d0..600a0b18 is data.
 * Row zero's final tap is deliberately untouched by the firmware. */
void lgsrcSinctable_makeTable(void)
{
    int16_t *table = lgsrc_sinc_table_60168490;
    const int16_t *rom = lgsrc_sinc_rom_60103a50;
    table[19] = rom[0];
    for (unsigned tap = 1; tap < 20; ++tap) {
        table[19 - tap] = rom[256 * tap];
        table[19 + tap] = rom[256 * tap];
    }
    for (unsigned tap = 0; tap < 20; ++tap) {
        for (unsigned phase = 1; phase < 256; ++phase) {
            int16_t value = rom[256 * tap + phase];
            table[40 * phase + 19 - tap] = value;
            table[40 * (256 - phase) + 20 + tap] = value;
        }
    }
}
