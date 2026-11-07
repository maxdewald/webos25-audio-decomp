#include <cassert>
#include <cstdio>
#include <string>

/* Host state is semantic, not the ARM ABI. Selector bodies come from the reconstruction. */
class umiTVAudio {
public:
    int getPassThroughArcMode();
    int getAutoArcMode();
    int getPassThroughOpticMode();
    bool isCountryCodeJP() { return japan; }
    bool getSupportBnoModel() { return bno; }

    bool japan = false;
    bool bno = false;
    bool mixer = false;
    bool forceDDPlus = false;
    bool mpeg4AacLc = false;
    std::string arcProfile;
};

// @extract src/libumi/arc_mode.cpp umiTVAudio::getPassThroughArcMode umiTVAudio::getAutoArcMode umiTVAudio::getPassThroughOpticMode

struct Modes {
    bool japan;
    bool bno;
    bool mixer;
    int passDolby;
    int passDolbyAac;
    int autoDolby;
    int passOther;
    int autoOther;
    int passOptic;
};

static const Modes modes[] = {
    {false, false, false, 14, 14, 4, 12, 2, 7},
    {false, false, true,   4,  4, 8,  2, 6, 2},
    {false, true,  false, 14, 14, 4, 12, 2, 7},
    {false, true,  true,   4,  4, 8,  2, 6, 2},
    {true,  false, false, 15, 16, 5, 13, 3, 8},
    {true,  false, true,   5,  5, 9,  3, 7, 3},
    {true,  true,  false, 14, 14, 4, 12, 2, 8},
    {true,  true,  true,   4,  4, 8,  2, 6, 3},
};

struct Profile {
    const char *name;
    int kind;
};

int main()
{
    const Profile profiles[] = {
        {"EAC3", 0}, {"ATMOS", 0}, {"PCM", 1},
        {"AC3", 2}, {"", 2}, {"pcm", 2}, {"EAC3-extra", 2},
    };
    unsigned cases = 0;
    for (const Modes &expected : modes) {
        umiTVAudio audio;
        audio.japan = expected.japan;
        audio.bno = expected.bno;
        audio.mixer = expected.mixer;
        for (const Profile &profile : profiles) {
            audio.arcProfile = profile.name;
            for (bool forced : {false, true}) {
                audio.forceDDPlus = forced;
                for (bool aac : {false, true}) {
                    audio.mpeg4AacLc = aac;
                    const int pass[] = {
                        aac ? expected.passDolbyAac : expected.passDolby,
                        1, expected.passOther,
                    };
                    const int automatic[] = {
                        forced ? 11 : expected.autoDolby,
                        1, forced ? 10 : expected.autoOther,
                    };
                    assert(audio.getPassThroughArcMode() == pass[profile.kind]);
                    assert(audio.getAutoArcMode() == automatic[profile.kind]);
                    assert(audio.getPassThroughOpticMode() == expected.passOptic);
                    ++cases;
                }
            }
        }
    }
    std::printf("ARC/optical selectors: %u combinations passed\n", cases);
}