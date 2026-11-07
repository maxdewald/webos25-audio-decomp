#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* dec_mat output investigation, dsp0 main image linked at 0x60000000.
 * Evidence: symbols/dsp0.syms.tsv and the corresponding build/decomp/ export;
 * scalar branch checks in build/scratch/dec-mat/. No chain reconstruction.
 *
 * _mat_chai @0x60255888 emits metadata (aud_data type 2) on port 0 at
 * 0x60255d39, and PCM (type 0) on port 0 at 0x60255f27. PCM allocation/copy
 * uses module+0x164 bytes from the pointer at module+0x148; downmix changes
 * this PCM copy, not the ES path. _mat_dec_process_oar @0x604e9230 publishes
 * a 0x3c00-byte PCM block (480 samples, 8 channels, 32 bits) on success.
 * No port-1 data push or raw TrueHD/IEC/MAT frame writer was found in this
 * wrapper. At 0x60255a6f, zero PCM bytes branch back without emitting data.
 *
 * FIRST on port 0 @0x60255c01 advertises 32-bit PCM, bypass_admix=true,
 * dolby=true, decoder-derived atmos, and the current downmix state. The copy
 * sent on port 1 @0x60255c56 sets es=true, codec="mat", rate=48000,
 * data_rate=192000, channels=8, bits_per_sample=16. It is an announcement,
 * not evidence of a MAT payload or retained compressed TrueHD/Atmos data.
 * bypass_admix is not a request to copy the input bitstream.
 *
 * _mat_cmd @0x60255218: 0x33010004 accepts IEC-header values 0/1, passed
 * through selector 0 of mat_dec_main_set_config @0x604e96cc to API (4,0x49).
 * 0x10050004 accepts 0..7, stored at module+0x568 @0x602555d3; no read of
 * that field was found in this chain. 0x33050004 accepts 0/1 and invokes
 * selector 4 / API (4,0x41). Neither command adds an ES producer here.
 *
 * Ghidra's FLIX branch gaps in this wrapper were checked with xtobjdump;
 * linear misdecodes at 0x60255de8 and 0x60256025 are scalar code, not SIMD.
 * [FLIX] Only branch slots are validated. Secondary slots in output-side
 * bundles at 0x60255c74, 0x60255c84, 0x60255cc2, 0x60255cdd, 0x60255d28,
 * 0x60255d60, 0x60255d87 and 0x60255e2e remain unvalidated; this is not a
 * full instruction-equivalence claim for the chain.
 * [UNRECOVERED] xa_mat_dec @0x604f9d04 and nested TrueHD/OAR processing are
 * not reconstructed here. IEC-header parsing, raw TrueHD input acceptance,
 * and lossless/Atmos retention must not be inferred from the MAT FIRST.
 */

typedef struct dec_mat {
    uint8_t unrecovered_000[0x574];
    uint8_t downmix;
    uint8_t first_pending;
} dec_mat;

_Static_assert(offsetof(dec_mat, downmix) == 0x574, "MAT downmix offset");
_Static_assert(offsetof(dec_mat, first_pending) == 0x575, "MAT FIRST offset");

/* @0x6025656c: Controls PCM downmix and requests a new FIRST, not ES bypass.
 * Complete scalar state/return behavior; diagnostic logging elided.
 */
static int _mat_downmix_out_channel(dec_mat *module, const void *property,
                                    const char *value)
{
    (void)property;
    if (strcmp(value, "true") == 0 || strcmp(value, "yes") == 0 ||
        strcmp(value, "1") == 0) {
        module->downmix = 1;
    } else if (strcmp(value, "false") == 0 || strcmp(value, "no") == 0 ||
               strcmp(value, "0") == 0) {
        module->downmix = 0;
    } else {
        return -1;
    }
    module->first_pending = 1;
    return 0;
}