/*
 * ADEC cost calculation.
 * RECONSTRUCTION from usr/lib/libmedia-resource-calculator.so.1
 *   sha256 9cf65c00a778d9b408b1dd701349642be55338394f4720f6008b5187aeed2d06
 * Addresses: Ghidra (= raw ELF VA + 0x10000). Evidence: ../../evidence/resource-booking/
 */
#include <stdbool.h>

/* Embedded default table @0x17ebc (JSON, 426 bytes). */
struct adec_cost { const char *key; int adec; int bandwidth; bool notsupported; };
const struct adec_cost mrc_table[] = {
    { "aac6",        1, 0, true  },
    { "dtse",        1, 4, false },
    { "mpeg-h",      1, 4, false },
    { "ac4",         1, 4, false },
    { "atmos",       1, 3, false },
    { "description", 1, 2, false },
    { "max",         1, 3, false },
    { "default",     1, 2, false },
    /* "pcm", "dts", "mat", "iamf" are looked up too but absent -> nothing added */
};

/* Codec flag bits tested by calcAdecResources (bit numbers of the flags word). */
enum mrc_flag {
    MRC_FORCE_DEFAULT = 0, MRC_AAC = 1, MRC_PCM = 2, MRC_DTS = 3, MRC_DTSE = 4,
    MRC_MPEGH = 5, MRC_AC4 = 6, MRC_ATMOS = 7, MRC_DESCRIPTION = 8,
    MRC_MAT = 9, MRC_MAX = 10, MRC_IAMF = 11,
};

extern void add_cost(void *list, const char *key);   /* ResourceTable::lookup + concat */
extern bool list_empty(void *list);

/* @0x158d8 mrc::ResourceCalculator::calcAdecResources(list, table, flags, channels, ...) */
void calcAdecResources(void *out, unsigned flags, int aac_channels, int aac_profile)
{
    static const struct { int bit; const char *key; } map[] = {
        { MRC_PCM, "pcm" }, { MRC_DTS, "dts" }, { MRC_DTSE, "dtse" }, { MRC_MPEGH, "mpeg-h" },
        { MRC_AC4, "ac4" }, { MRC_ATMOS, "atmos" }, { MRC_DESCRIPTION, "description" },
        { MRC_MAT, "mat" }, { MRC_MAX, "max" }, { MRC_IAMF, "iamf" },
    };
    if (aac_channels == 6 && (flags & (1u << MRC_AAC)) && (aac_profile == 2 || aac_profile == 4))
        add_cost(out, "aac6");
    for (unsigned i = 0; i < sizeof(map) / sizeof(map[0]); i++)
        if (flags & (1u << map[i].bit))
            add_cost(out, map[i].key);
    if (list_empty(out) || (flags & (1u << MRC_FORCE_DEFAULT)))
        add_cost(out, "default");
}
