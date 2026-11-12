/* Behavioral tests run recovered parser bodies via the extraction harness. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// @match src/dsp0/hdmi_parser.c (?m)^static uint32_t hdmi_dtsx_sync(?:_previous)?;
/* Firmware __assert_failed logs and dumps a backtrace, then RETURNS. */
static int assert_failures;
static const char *last_assert;
static void __assert_failed(const char *fn, unsigned line, const char *expr)
{
    (void)fn; (void)line; assert_failures++; last_assert = expr;
}
// @extract src/dsp0/hdmi_parser.c HdmiParser_Get_DTS_FrameSize HdmiParser_Get_DTSXP2_FrameSize HdmiParser_Init_Dtsx_SycnType HdmiParser_FindDTSCDSync

static void put32(uint8_t *p, uint32_t v)
{
    p[0] = v >> 24; p[1] = v >> 16; p[2] = v >> 8; p[3] = v;
}

static void test_core_and_extension_sizes(void)
{
    /* Caller-owned padding is intentional: the shipped helper has no length
     * parameter and unconditionally probes the end of a claimed core frame. */
    uint8_t b[16400] = {0};
    put32(b, 0x7ffe8001);
    b[6] = 1; b[7] = 0xf0; /* 32-byte core */
    hdmi_dtsx_sync = 99; hdmi_dtsx_sync_previous = 88;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 32);
    assert(hdmi_dtsx_sync == 0 && hdmi_dtsx_sync_previous == 88);
    put32(b, 0x1fffe800); /* shipped 14-bit marker uses the same size formula */
    assert(HdmiParser_Get_DTS_FrameSize(b) == 32);
    b[5] = 3; b[6] = 255; b[7] = 255;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 16384);
    b[5] = b[6] = b[7] = 0;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 1);

    memset(b, 0, sizeof(b));
    put32(b, 0x64582025); /* extension alone, short size field */
    b[6] = 0x12; b[7] = 0x34; b[8] = 0xe0;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 37288);
    assert(hdmi_dtsx_sync == 1);
    b[5] = 0x20; b[6] = 1; b[7] = 0x23; b[8] = 0x45; b[9] = 0xe0;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 596528);
    b[6] = 1; b[7] = 255; b[8] = 255; b[9] = 255;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 1048576);
    b[5] = b[6] = b[7] = b[8] = b[9] = 0;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 1);

    memset(b, 0, sizeof(b));
    put32(b, 0x7ffe8001); b[6] = 1; b[7] = 0xf0;
    put32(b + 32, 0x64582025);
    b[32 + 7] = 1; b[32 + 8] = 0xe0; /* 16-byte extension */
    assert(HdmiParser_Get_DTS_FrameSize(b) == 48 && hdmi_dtsx_sync == 2);
    b[32 + 5] = 0x20; b[32 + 6] = 1;
    b[32 + 7] = 0x23; b[32 + 8] = 0x45; b[32 + 9] = 0xe0;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 596560 && hdmi_dtsx_sync == 2);
    /* Invalid, byte-swapped and X markers are not accepted by this helper;
     * failure preserves the old classification instead of resetting it. */
    const uint32_t rejected[] = {0, 0xfe7f0180, 0xff1f00e8, 0x40411bf2, 0x71c442e8};
    for (unsigned i = 0; i < sizeof(rejected)/sizeof(rejected[0]); ++i) {
        put32(b, rejected[i]); hdmi_dtsx_sync = 77;
        assert(HdmiParser_Get_DTS_FrameSize(b) == -1 && hdmi_dtsx_sync == 77);
    }
}

static void test_xp2_length_and_failed_marker_state(void)
{
    uint8_t b[16] = {0};
    /* Unlike extension size, this is an unadjusted big-endian 16-bit value. */
    put32(b + 12, 0x40411bf2);
    assert(HdmiParser_Get_DTSXP2_FrameSize(b) == 0 && hdmi_dtsx_sync == 3);
    b[10] = 0x12; b[11] = 0x34;
    assert(HdmiParser_Get_DTSXP2_FrameSize(b) == 4660);
    put32(b + 12, 0x71c442e8);
    assert(HdmiParser_Get_DTSXP2_FrameSize(b) == 4660);
    b[10] = 255; b[11] = 255;
    assert(HdmiParser_Get_DTSXP2_FrameSize(b) == 65535);
    put32(b + 12, 0); hdmi_dtsx_sync = 66; hdmi_dtsx_sync_previous = 55;
    assert(HdmiParser_Get_DTSXP2_FrameSize(b) == -1);
    /* Failure sets current to 3, unlike Get_DTS_FrameSize failure. */
    assert(hdmi_dtsx_sync == 3 && hdmi_dtsx_sync_previous == 55);
    put32(b + 12, 0xf21b4140);
    assert(HdmiParser_Get_DTSXP2_FrameSize(b) == -1);
}

static void test_extension_size_bit_boundaries(void)
{
    uint8_t b[16] = {0};
    put32(b, 0x64582025);
    hdmi_dtsx_sync_previous = 88;
    /* @0x6023d270: extract bits 5..20, excluding both p[6] high bits
     * and p[8] low bits; all unused bits must not alter the returned size. */
    b[5] = 0xdf; b[6] = 0xe0; b[8] = 0x1f; b[9] = 0xff;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 1);
    b[6] = b[7] = b[8] = 0xff;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 65536);
    assert(hdmi_dtsx_sync == 1 && hdmi_dtsx_sync_previous == 88);
    /* @0x6023d2f1..0x6023d30c: only p[6] bit 0 and p[9] bits 5..7
     * survive in the long field. The flag itself is p[5] bit 5 only. */
    b[5] = 0xff; b[6] = 0xfe; b[7] = b[8] = 0; b[9] = 0x1f;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 1);
    b[6] = 0xff; b[9] = 0x3f;
    assert(HdmiParser_Get_DTS_FrameSize(b) == 524290);
    assert(hdmi_dtsx_sync == 1 && hdmi_dtsx_sync_previous == 88);
}

static void test_sync_reset(void)
{
    hdmi_dtsx_sync = 2; hdmi_dtsx_sync_previous = 3;
    HdmiParser_Init_Dtsx_SycnType();
    assert(hdmi_dtsx_sync == 4 && hdmi_dtsx_sync_previous == 4);
    HdmiParser_Init_Dtsx_SycnType();
    assert(hdmi_dtsx_sync == 4 && hdmi_dtsx_sync_previous == 4);
    uint8_t b[16] = {0};
    put32(b, 0x64582025);
    assert(HdmiParser_Get_DTS_FrameSize(b) == 1);
    assert(hdmi_dtsx_sync == 1 && hdmi_dtsx_sync_previous == 4);
}

static void test_dtscd_scan_and_truncation(void)
{
    uint8_t b[8210] = {0};
    int offset = 99, stride = 77;
    for (int len = 1; len < 8; ++len) {
        offset = 99;
        assert(HdmiParser_FindDTSCDSync(b, len, &offset, &stride) == -995);
        assert(offset == 0 && stride == 77);
    }
    put32(b, 0x7ffe8001);
    assert(HdmiParser_FindDTSCDSync(b, 8, &offset, &stride) == -998 && offset == 0);
    assert(HdmiParser_FindDTSCDSync(b, 11, &offset, &stride) == -998 && offset == 3);
    assert(HdmiParser_FindDTSCDSync(b, 12, &offset, &stride) == 0);
    assert(offset == 0 && stride == 4096);
    /* Initial scan leaves eight bytes, rather than admitting every complete
     * four-byte marker near the end. It does NOT assemble split calls. */
    memset(b, 0, sizeof(b)); put32(b + 5, 0x1fffe800);
    assert(HdmiParser_FindDTSCDSync(b, 16, &offset, &stride) == -998 && offset == 8);
    assert(HdmiParser_FindDTSCDSync(b, 17, &offset, &stride) == 0 && offset == 5);
    put32(b + 5, 0x64582025);
    assert(HdmiParser_FindDTSCDSync(b, 17, &offset, &stride) == -998);
    put32(b + 5, 0xfe7f0180);
    assert(HdmiParser_FindDTSCDSync(b, 17, &offset, &stride) == -998);

    memset(b, 0, sizeof(b)); put32(b, 0x7ffe8001);
    put32(b + 4096, 0x1fffe800); put32(b + 8192, 0x7ffe8001);
    assert(HdmiParser_FindDTSCDSync(b, 4099, &offset, &stride) == 0 && offset == 0);
    assert(HdmiParser_FindDTSCDSync(b, 4100, &offset, &stride) == 1 && offset == 0);
    assert(HdmiParser_FindDTSCDSync(b, 8196, &offset, &stride) == 2 && offset == 0);
    put32(b + 8192, 0);
    assert(HdmiParser_FindDTSCDSync(b, 8196, &offset, &stride) == -993 && offset == 4);
    put32(b + 4096, 0);
    assert(HdmiParser_FindDTSCDSync(b, 4100, &offset, &stride) == -993 && offset == 4);
    assert(HdmiParser_FindDTSCDSync(b, 4100, NULL, &stride) == -993);
    /* A null stride output leaves the local stride zero, rather than using
     * 4096. Select a short-tail case that returns without the zero-stride loop. */
    memset(b, 0, sizeof(b)); put32(b + 5, 0x7ffe8001);
    assert(HdmiParser_FindDTSCDSync(b, 13, &offset, NULL) == -998 && offset == 5);
    assert(HdmiParser_FindDTSCDSync(b, 8, NULL, NULL) == -998);
}

static void test_dtscd_shipped_assertions_do_not_stop(void)
{
    /* Violated preconditions are reported, then the short-input branch still
     * runs. Only lengths < 8 are safe here: a NULL buffer is not dereferenced. */
    int offset = 99, stride = 77;
    assert_failures = 0;
    assert(HdmiParser_FindDTSCDSync(NULL, 0, &offset, &stride) == -995);
    assert(assert_failures == 2 && !strcmp(last_assert, "_length > 0"));
    assert(offset == 0 && stride == 77);
    offset = 99;
    assert(HdmiParser_FindDTSCDSync("x", -5, &offset, NULL) == -995);
    assert(assert_failures == 3 && offset == 0);
    assert(HdmiParser_FindDTSCDSync(NULL, 7, NULL, NULL) == -995);
    assert(assert_failures == 4 && !strcmp(last_assert, "_buf != NULL"));
}

static void test_dtscd_shifted_mismatch_and_classification_preservation(void)
{
    uint8_t b[4105] = {0};
    int offset = -1, stride = -1;
    put32(b + 5, 0x7ffe8001);
    put32(b + 4101, 0x1fffe800);
    hdmi_dtsx_sync = 3; hdmi_dtsx_sync_previous = 2;
    assert(HdmiParser_FindDTSCDSync(b, sizeof(b), &offset, &stride) == 1);
    assert(offset == 5 && stride == 4096);
    assert(hdmi_dtsx_sync == 3 && hdmi_dtsx_sync_previous == 2);
    put32(b + 4101, 0x64582025);
    assert(HdmiParser_FindDTSCDSync(b, sizeof(b), &offset, &stride) == -993);
    /* @0x6023dcf7..0x6023dcff: initial offset + 4, not the later sync. */
    assert(offset == 9 && stride == 4096);
    assert(hdmi_dtsx_sync == 3 && hdmi_dtsx_sync_previous == 2);
    assert(HdmiParser_FindDTSCDSync(b, 17, NULL, &stride) == 0);
    /* Outputs remain untouched on no-marker and short-input branches except
     * for the documented offset writes; neither branch clears classification. */
    put32(b + 5, 0); stride = 77;
    assert(HdmiParser_FindDTSCDSync(b, 17, &offset, &stride) == -998);
    assert(offset == 9 && stride == 77);
    assert(HdmiParser_FindDTSCDSync(b, 7, NULL, &stride) == -995);
    assert(stride == 77 && hdmi_dtsx_sync == 3 && hdmi_dtsx_sync_previous == 2);
}

int main(void)
{
    test_core_and_extension_sizes();
    test_xp2_length_and_failed_marker_state();
    test_extension_size_bit_boundaries();
    test_sync_reset();
    test_dtscd_scan_and_truncation();
    test_dtscd_shifted_mismatch_and_classification_preservation();
    test_dtscd_shipped_assertions_do_not_stop();
    puts("DTS core/extension size and classification behavior passed");
    return 0;
}
