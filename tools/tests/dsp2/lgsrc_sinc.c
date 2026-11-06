/* Synthetic ROM tests the actual copy/reindexing code, not a sinc/FIR mock.
 */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../../../src/dsp2/lgsrc_sinc.c"
static int16_t fixture_rom[5120];
static int16_t fixture_table[10240];
const int16_t *lgsrc_sinc_rom_60103a50 = fixture_rom;
int16_t *lgsrc_sinc_table_60168490 = fixture_table;

/* Scalar instruction-order oracle from 0x6009f659..6009f7c3. */
static void reference(int16_t *out)
{
    out[19] = fixture_rom[0];
    ptrdiff_t left = -1, right = 1;
    for (unsigned i = 1; i < 20; ++i) {
        --left;
        out[left + 20] = fixture_rom[i * 256];
        out[right + 19] = fixture_rom[i * 256];
        ++right;
    }
    for (unsigned i = 0; i < 20; ++i) {
        left = (0x4f2c / 2) + (ptrdiff_t)i - 40;
        right = 40 - (ptrdiff_t)i;
        unsigned source = i * 256 + 1;
        for (unsigned j = 0; j < 255; ++j) {
            left -= 40;
            out[left + 166] = fixture_rom[source];
            out[right + 19] = fixture_rom[source++];
            right += 40;
        }
    }
}

int main(void)
{
    int16_t expected[10240];
    for (unsigned trial = 0; trial < 3; ++trial) {
        for (unsigned i = 0; i < 5120; ++i) {
            if (trial == 0) fixture_rom[i] = (int16_t)((int)i - 2560);
            else if (trial == 1) fixture_rom[i] = i % 2 ? INT16_MIN : INT16_MAX;
            else fixture_rom[i] = (int16_t)((int)(i * 17 % 65536) - 32768);
        }
        for (unsigned i = 0; i < 10240; ++i)
            expected[i] = lgsrc_sinc_table_60168490[i] = -12345;
        reference(expected);
        lgsrcSinctable_makeTable();
        assert(memcmp(expected, lgsrc_sinc_table_60168490, sizeof(expected)) == 0);
        assert(lgsrc_sinc_table_60168490[39] == -12345);
        for (unsigned i = 0; i < 39; ++i)
            assert(lgsrc_sinc_table_60168490[i] == fixture_rom[(i < 19 ? 19 - i : i - 19) * 256]);
    }
    puts("lgsrc_sinc: all 10240 destinations, signed boundaries, untouched tap passed");
    return 0;
}
