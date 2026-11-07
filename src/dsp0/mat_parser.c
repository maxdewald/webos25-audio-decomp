/* mat_dec_add_bytes @0x604f2404 enters the accumulator @0x604f2238, whose
 * initial state requires 07 9E MAT sync. IEC mode first filters the burst at
 * 0x604f8f74 (type 0x16); it does not bypass the MAT accumulator. The TrueHD
 * sync probe below is downstream, not an alternate raw-input entry point.
 * [FLIX] Full MAT header/CRC validation and chunk dispatch remain unrecovered. */

/* @0x604eda80: Classifies bytes 4..7 as F8 72 6F BA (1), BB (2), or neither (0).
 * Requires eight readable bytes and a writable result; does not validate framing
 * or establish that a raw TrueHD access unit reaches the decoder. */
void mat_dec_dthd_sync_check(const unsigned char *data, int *sync_type)
{
    if (data[4] != 0xf8 || data[5] != 0x72 || data[6] != 0x6f) {
        *sync_type = 0;
        return;
    }
    if (data[7] == 0xba) {
        *sync_type = 1;
        return;
    }
    *sync_type = data[7] == 0xbb ? 2 : 0;
}