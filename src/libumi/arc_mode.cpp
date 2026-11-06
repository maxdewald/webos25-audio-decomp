/* libumi umitvaudioutil.cpp: ARC digital-mode selectors. Only the ARM offsets
 * these functions read are modelled; the rest of umiTVAudio stays opaque.
 */
#include <string>

class umiTVAudio {
public:
    int getPassThroughArcMode();
    int getAutoArcMode();
    bool isCountryCodeJP();
    bool getSupportBnoModel();

private:
    void *vptr;
    unsigned char pad0[0x7c - 4];
    bool mixer;                 /* +0x7c, written only by setMixerSettings */
    bool forceDDPlus;           /* +0x7d */
    unsigned char pad1[0xa8 - 0x7e];
    std::string arcProfile;     /* +0xa8 */
    unsigned char pad2[0xc0 - 0xa8 - sizeof(std::string)];
    bool mpeg4AacLc;            /* +0xc0 */
};

/* @0x59344: mode id for ARC "passThrough". The mixer byte alone splits mixed
 * (1..5) from unmixed (12..16); no call site in shipped binaries was found. */
int umiTVAudio::getPassThroughArcMode()
{
    if (arcProfile.compare("EAC3") == 0 || arcProfile.compare("ATMOS") == 0) {
        if (!isCountryCodeJP() || getSupportBnoModel())
            return mixer ? 4 : 14;
        if (mpeg4AacLc)
            return mixer ? 5 : 16;
        return mixer ? 5 : 15;
    }
    if (arcProfile.compare("PCM") == 0)
        return 1;
    if (!isCountryCodeJP() || getSupportBnoModel())
        return mixer ? 2 : 12;
    return mixer ? 3 : 13;
}

/* @0x593ec: mode id for ARC "auto"; forceOutputDDPLUS (+0x7d) short-circuits. */
int umiTVAudio::getAutoArcMode()
{
    int mode;
    if (arcProfile.compare("EAC3") == 0 || arcProfile.compare("ATMOS") == 0) {
        mode = 11;
        if (!forceDDPlus) {
            if (isCountryCodeJP() && !getSupportBnoModel())
                return mixer ? 9 : 5;
            return mixer ? 8 : 4;
        }
    } else {
        if (arcProfile.compare("PCM") == 0)
            return 1;
        mode = 10;
        if (!forceDDPlus) {
            if (isCountryCodeJP() && !getSupportBnoModel())
                return mixer ? 7 : 3;
            return mixer ? 6 : 2;
        }
    }
    return mode;
}
