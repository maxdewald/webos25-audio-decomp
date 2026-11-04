/*
 * dsp0 decoder inventory.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp0.bin
 *   sha256 337a6330397f0891a4cf9b9bccc06b36d902b1dce18e110507bc98f2a5f25b58
 * Main segment linked at 0x60000000 (file offset 0x630). Evidence:
 *   ../../symbols/dsp0.syms.tsv, ../../decomp/audio_a0_dsp0.main_60000000.bin.c,
 *   live /proc/aaudio/processors/dsp0 listing (2026-11-04).
 *
 * Answer to "does a DTS decoder survive in dsp0?": NO.
 *  - No dec_dts* module is registered live, and no dec_dts* / DCA / XLL / LBR symbol,
 *    constructor or string exists in the image (bounded search over 9.8k symbols).
 *  - What remains of DTS is parsing/detection only: HDMI input framing helpers and
 *    the format-change event. The enc_dts* strings are output-codec names; no
 *    matching encoder module is registered on dsp0 or dsp1 at runtime.
 * So restoring LG's native DTS decode is not possible without new firmware.
 */

/* Decoder modules registered live on dsp0 (same set the image constructs). */
const char *const dsp0_decoders[] = {
    "dec_ac4", "dec_amrnb", "dec_amrwb", "dec_ddp", "dec_dra", "dec_flac",
    "dec_heaac", "dec_iamf", "dec_mat", "dec_mp3", "dec_mpegh", "dec_ogg",
    "dec_opus", "dec_pcm", "dec_wma",
};

/* DTS remnants (dsp0 symbol table): HDMI-receiver bitstream parsing only. */
extern int HdmiParser_Get_DTS_FrameSize(const void *hdr);    /* @0x6023d210 */
extern int HdmiParser_Get_DTSXP2_FrameSize(const void *hdr); /* @0x6023d340 */
extern int HdmiParser_FindDTSCDSync(const void *buf);        /* @0x6023dba8 */
extern void HdmiParser_Init_Dtsx_SycnType(void);             /* @0x6023dd4c */

/* Debug enums printed by the HDMI path (strings in dsp0):
 *   codec:    0 PCM, 1 AC3, 2 EAC3, 3 AAC, 4 MAT, 5 DTS, 6 DTS-HD
 *   mode:     0 PCM, 1 ES, 2 60958ES, 3 DTSCD, 4 HBR, 5 HRA, 6 BYTE0,
 *             7 BYTE0_HBR, 8 BYTE0_HBR_HRA
 *   burst:    0 NULL, 1 DD, 11..13 DTS, 21 DD+, 22 MAT
 */
