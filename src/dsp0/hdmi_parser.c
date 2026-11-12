/* DSP0 HDMI parser reconstruction from the hash-verified C2 audio_a0_dsp0.bin.
 * Main segment: file +0x630 => VA 0x60000000.
 * Helpers below operate on receiver-normalized, big-endian byte sequences, not
 * HDMI wire samples. This is parsing evidence, not a transparency guarantee.
 */
#include <stddef.h>
#include <stdint.h>

/* @0x60211ad0: logs "assert failed at %s:%d" plus a backtrace and RETURNS. */
extern void __assert_failed(const char *fn, unsigned line, const char *expr);

/* @0x60553340 / @0x60553354: current and previously accepted sync classification.
 * These are parser-local values, NOT the hdmi module's codec property enum. */
static uint32_t hdmi_dtsx_sync;
static uint32_t hdmi_dtsx_sync_previous;

/* @0x6023d210: return a core/extension span; -1 for an unknown start marker.
 * 0x7ffe8001 and 0x1fffe800 share the shipped core-size extraction, including
 * the latter's surprising lack of 14->16-bit size scaling. 0x64582025 is the
 * extension marker. Classification is 0 core, 1 extension, 2 core+extension.
 * PRECONDITION: bytes 0..9 and (for core) a probe at core_size..core_size+9
 * must be readable. No length validation or payload copying occurs here.
 * Exact-entry listing and aligned branch targets 0x6023d278/0x6023d2ec were
 * checked; there are no unresolved FLIX slots in this recovered helper.
 */
int HdmiParser_Get_DTS_FrameSize(const void *hdr)
{
    const uint8_t *p = hdr;
    uint32_t marker = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16)
                    | ((uint32_t)p[2] << 8) | p[3];
    int core_size = 0;
    uint32_t classification = 1;
    if (marker == 0x7ffe8001 || marker == 0x1fffe800) {
        core_size = (((uint32_t)p[5] & 3) << 12)
                  | ((uint32_t)p[6] << 4) | (p[7] >> 4);
        core_size++;
        p += core_size;
        marker = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16)
               | ((uint32_t)p[2] << 8) | p[3];
        if (marker != 0x64582025) {
            hdmi_dtsx_sync = 0;
            return core_size;
        }
        classification = 2;
    } else if (marker != 0x64582025) {
        return -1; /* @0x6023d278: leave the previous classification untouched. */
    }
    hdmi_dtsx_sync = classification;
    uint32_t size;
    if (p[5] & 0x20) {
        /* @0x6023d2ec / @0x6023d313: 20-bit extension size-minus-one. */
        size = (((uint32_t)p[6] & 1) << 19) | ((uint32_t)p[7] << 11)
             | ((uint32_t)p[8] << 3) | (p[9] >> 5);
    } else {
        size = ((((uint32_t)p[6] << 16) | ((uint32_t)p[7] << 8) | p[8])
                >> 5) & 0xffff;
    }
    return core_size + (int)size + 1;
}

/* @0x6023d340: XP2 framing, not DTS-HD profile detection or payload conversion.
 * The two recognized markers at +12 return BE16(+10), including zero. All
 * branches, even invalid markers, set current classification to 3; previous
 * is not touched. PRECONDITION: at least 16 bytes readable; no length argument.
 * Branch target 0x6023d390 independently decoded after padding alignment loss.
 */
int HdmiParser_Get_DTSXP2_FrameSize(const void *hdr)
{
    const uint8_t *p = hdr;
    uint32_t marker = ((uint32_t)p[12] << 24) | ((uint32_t)p[13] << 16)
                    | ((uint32_t)p[14] << 8) | p[15];
    hdmi_dtsx_sync = 3;
    if (marker == 0x40411bf2 || marker == 0x71c442e8)
        return ((uint32_t)p[10] << 8) | p[11];
    return -1;
}

/* @0x6023dd4c: forget both classifications (4 = not yet established).
 * This does not flush a packet buffer or reset module lifecycle state. */
void HdmiParser_Init_Dtsx_SycnType(void)
{
    hdmi_dtsx_sync_previous = 4;
    hdmi_dtsx_sync = 4;
}

/* @0x6023dba8: scan raw DTS-CD markers and count subsequent 4096-byte syncs.
 * Return >=0 is the number of subsequent matching markers, NOT frame bytes.
 * -995 short input, -998 no marker, -993 a predicted marker mismatched.
 * The initial scanner stops eight bytes short of the end. A mismatch returns
 * initial_sync_offset+4, not the position of the later mismatch. No assembly
 * across calls or classification update occurs. Optional output pointers are
 * preserved when not written on that particular branch.
 * PRECONDITION: nonnull data, positive len; provide stride for a successful
 * scan. The shipped assertions are NOT fatal: __assert_failed @0x60211ad0 logs
 * and dumps a backtrace, then returns and the scan continues (len <= 0 still
 * reaches the -995 branch; NULL data with len >= 8 is dereferenced).
 * A null stride leaves the local stride zero in firmware, and a match then
 * loops forever (remaining never drops below 5); deliberately not hardened.
 * Omitted: a 160-byte local header (stride, BE16 after the sync) that is
 * filled and discarded, and a bltz count<0 rescan edge at 0x6023dca4 that is
 * unreachable for stride 0 or 4096.
 * Exact branch targets 0x6023dc02/0x6023dc0d were checked independently of the
 * padding which desynchronizes a broad listing. No unresolved FLIX slots here.
 */
int HdmiParser_FindDTSCDSync(const void *data, int len, int *offset, int *stride)
{
    const uint8_t *p = data;
    if (p == NULL)
        __assert_failed("HdmiParser_FindDTSCDSync", 0x298, "_buf != NULL");
    if (len < 1)
        __assert_failed("HdmiParser_FindDTSCDSync", 0x29a, "_length > 0");
    if (len < 8) {
        if (offset) *offset = 0;
        return -995;
    }
    int pos = 0;
    int remaining = len;
    uint32_t word = UINT32_MAX;
    while (pos < len - 8) {
        remaining--;
        word = (word << 8) | p[pos++];
        if (word != 0x7ffe8001 && word != 0x1fffe800)
            continue;
        int start = pos - 4;
        if (start < 0)
            __assert_failed("HdmiParser_FindDTSCDSync", 0x2b2, "packet_start >= _buf");
        int step = 0;
        if (stride) {
            step = 4096;
            *stride = step;
        }
        int count = 0;
        if (remaining >= 5) {
            do {
                remaining -= step;
                pos += step;
                if (pos <= len) {
                    word = ((uint32_t)p[pos - 4] << 24)
                         | ((uint32_t)p[pos - 3] << 16)
                         | ((uint32_t)p[pos - 2] << 8) | p[pos - 1];
                    if (word != 0x7ffe8001 && word != 0x1fffe800) {
                        if (offset) *offset = start + 4;
                        return -993;
                    }
                    count++;
                }
            } while (remaining >= 5);
        }
        if (offset) *offset = start;
        return count;
    }
    if (offset) *offset = len - 8;
    return -998;
}
