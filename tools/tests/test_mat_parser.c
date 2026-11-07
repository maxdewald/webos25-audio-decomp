#include <assert.h>
#include <stdlib.h>
#include <string.h>

// @extract src/dsp0/mat_parser.c mat_dec_dthd_sync_check

int main(void)
{
    const unsigned char headers[2][8] = {
        {0x00, 0x01, 0x02, 0x03, 0xf8, 0x72, 0x6f, 0xba},
        {0xff, 0xfe, 0xfd, 0xfc, 0xf8, 0x72, 0x6f, 0xbb},
    };
    unsigned char *data = malloc(8);
    assert(data);
    for (unsigned int kind = 0; kind < 2; kind++) {
        for (unsigned int offset = 0; offset < 8; offset++) {
            for (unsigned int value = 0; value < 256; value++) {
                memcpy(data, headers[kind], 8);
                data[offset] = (unsigned char)value;
                unsigned char original[8];
                memcpy(original, data, 8);
                int expected = (int)kind + 1;
                if (offset >= 4 && offset < 7 && value != headers[kind][offset])
                    expected = 0;
                if (offset == 7)
                    expected = value == 0xba ? 1 : value == 0xbb ? 2 : 0;
                int result = -1;
                mat_dec_dthd_sync_check(data, &result);
                assert(result == expected);
                assert(memcmp(data, original, 8) == 0);
            }
        }
    }
    const unsigned char sync_at_start[8] = {0xf8, 0x72, 0x6f, 0xba, 0, 0, 0, 0};
    int result = -1;
    mat_dec_dthd_sync_check(sync_at_start, &result);
    assert(result == 0);
    free(data);
    return 0;
}