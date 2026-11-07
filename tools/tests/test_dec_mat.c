#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// @match src/dsp0/dec_mat.c (?s)typedef struct dec_mat \{.*?\} dec_mat;
// @extract src/dsp0/dec_mat.c _mat_downmix_out_channel

int main(void)
{
    static const char *const enabled[] = {"true", "yes", "1"};
    static const char *const disabled[] = {"false", "no", "0"};
    static const char *const invalid[] = {"", "TRUE", "false ", "2", "-1"};
    dec_mat module;
    dec_mat before;

    memset(&module, 0xa5, sizeof(module));
    for (unsigned index = 0; index < sizeof(enabled) / sizeof(enabled[0]); ++index) {
        module.first_pending = 0;
        before = module;
        assert(_mat_downmix_out_channel(&module, NULL, enabled[index]) == 0);
        assert(module.downmix == 1 && module.first_pending == 1);
        assert(memcmp(module.unrecovered_000, before.unrecovered_000,
                      sizeof(module.unrecovered_000)) == 0);
    }
    for (unsigned index = 0; index < sizeof(disabled) / sizeof(disabled[0]); ++index) {
        module.first_pending = 0;
        before = module;
        assert(_mat_downmix_out_channel(&module, NULL, disabled[index]) == 0);
        assert(module.downmix == 0 && module.first_pending == 1);
        assert(memcmp(module.unrecovered_000, before.unrecovered_000,
                      sizeof(module.unrecovered_000)) == 0);
    }
    for (unsigned index = 0; index < sizeof(invalid) / sizeof(invalid[0]); ++index) {
        module.downmix = 1;
        module.first_pending = index & 1;
        before = module;
        assert(_mat_downmix_out_channel(&module, NULL, invalid[index]) == -1);
        assert(memcmp(&module, &before, sizeof(module)) == 0);
    }
    puts("MAT downmix values, FIRST refresh, and invalid-input atomicity");
    return 0;
}