#include <array>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "../../src/libumi/arc_output.cpp"

using namespace umi_arc_output;

struct Write {
    std::string control;
    std::vector<int> words;
    std::array<int, 7> cache;
};

struct Observation {
    std::vector<Write> writes;
    std::vector<std::array<int, 4>> transitions;
    std::vector<int> delays;
    std::vector<std::string> logs;
    int failWrite = 0;
    int result = 1;
    int sp50 = 0;
    bool support = false;
    unsigned profileReads = 0;
};

static Observation observed;
static UmiLogContext logContext{};
static int &logLevel = logContext.enabledLevel;

extern "C" {
UmiLogContext *umiLogContext = &logContext;

int _PmLogMsgKV(UmiLogContext *, int, unsigned, const char *identifier, ...)
{
    char buffer[512];
    va_list arguments;
    va_start(arguments, identifier);
    assert(va_arg(arguments, int) == 0);
    assert(va_arg(arguments, const void *) == nullptr);
    assert(va_arg(arguments, const void *) == nullptr);
    const char *format = va_arg(arguments, const char *);
    std::vsnprintf(buffer, sizeof(buffer), format, arguments);
    va_end(arguments);
    observed.logs.emplace_back(buffer);
    return 0;
}
}

namespace umi_arc_output {

void addInOutSettingNormalLog(Audio *self)
{
    observed.transitions.push_back({self->arcOn, self->earcOn, self->arcCodec, self->earcCodec});
}

bool isArcSoundOutput(Audio *self)
{
    return self->output == 0x400 || self->output == 0x400000;
}

bool SupportSP50(Audio *)
{
    return observed.support;
}

bool SP50ValueBaseOnMatProfile(Audio *)
{
    ++observed.profileReads;
    return observed.sp50;
}

}

static bool writeControl(Audio *self, const std::string &control, const int *words, int count)
{
    observed.writes.push_back({control, {words, words + count},
                              {self->requestedCodec, self->requestedMix, self->earcCodec,
                               self->earcChannels, self->earcMix, self->arcOn, self->earcOn}});
    return observed.failWrite == static_cast<int>(observed.writes.size()) ? 0 : observed.result;
}

static void delayCodec(Delay *, int codec)
{
    observed.delays.push_back(codec);
}

struct Fixture {
    AudioVtable methods{};
    DelayVtable delayMethods{};
    Delay delay{};
    Audio audio{};

    Fixture()
    {
        observed = {};
        logLevel = 0;
        umiLogContext = &logContext;
        methods.setControl = writeControl;
        delayMethods.setCodec = delayCodec;
        delay.vtable = &delayMethods;
        audio.vtable = &methods;
        audio.delay = &delay;
    }
};

static unsigned testEarc()
{
    unsigned cases = 0;
    for (int output : {0, 0x80000, 0x400, 0x400000}) {
        for (int enabled : {0, 1}) {
            for (int failure : {0, 1, 2}) {
                for (bool support : {false, true}) {
                    Fixture fixture;
                    fixture.audio.output = output;
                    fixture.audio.earcOn = enabled;
                    observed.support = support;
                    observed.sp50 = 1;
                    observed.failWrite = failure;
                    bool success = failure != 1 && !(support && failure == 2);
                    assert(setEArcOutputType(&fixture.audio, 7, 0, 8) == success);
                    assert(observed.writes.size() == (support && failure != 1 ? 2u : 1u));
                    const Write &first = observed.writes[0];
                    assert(first.control == "Sndout EARC Outputtype");
                    assert((first.words == std::vector<int>{2, 7, 8, 0, 0}));
                    assert((first.cache == std::array<int, 7>{7, 0, 7, 8, 0, 0, enabled}));
                    assert(observed.transitions.size() == (enabled ? 1u : 0u));
                    if (enabled)
                        assert((observed.transitions[0] == std::array<int, 4>{0, 1, 0, 7}));
                    if (observed.writes.size() == 2) {
                        assert(observed.writes[1].control == "Sndout EARC MS12 SP50 OnOff");
                        assert((observed.writes[1].words == std::vector<int>{1}));
                    }
                    bool notify = success && (output == 0x80000 || (output && enabled));
                    assert(observed.delays.size() == (notify ? 1u : 0u));
                    if (notify)
                        assert(observed.delays[0] == (output == 0x80000 ? 1 : 7));
                    ++cases;
                }
            }
        }
    }
    for (int codec : {-5, 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 100}) {
        Fixture fixture;
        observed.support = true;
        fixture.audio.earcCodec = codec;
        fixture.audio.earcOn = 1;
        assert(setEArcOutputType(&fixture.audio, codec, 1, 6) == 1);
        assert(observed.writes.size() == 1);
        assert((observed.writes[0].words == std::vector<int>{2, codec, 6, 0, 1}));
        assert(observed.transitions.empty());
        ++cases;
    }
    for (int level : {0, 3, 6}) {
        Fixture fixture;
        logLevel = level;
        observed.support = true;
        observed.sp50 = 0;
        assert(setEArcOutputType(&fixture.audio, 7, 1, 2) == 1);
        assert(observed.profileReads == (level >= 6 ? 2u : 1u));
        assert((observed.writes[1].words == std::vector<int>{0}));
        ++cases;
    }
    Fixture fixture;
    observed.result = 9;
    assert(setEArcOutputType(&fixture.audio, 2, 1, 6) == 1);
    fixture.audio.output = 0x400;
    assert(setEArcOutputType(&fixture.audio, 2, 1, 6) == 1);
    return cases + 2;
}

static unsigned testArcAndOptic()
{
    unsigned cases = 0;
    for (int codec : {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 100}) {
        for (int output : {0, 2, 0x1000, 0x400, 0x400000, 0x80000}) {
            for (int enabled : {0, 1}) {
                for (int failure : {0, 1}) {
                    Fixture fixture;
                    fixture.audio.output = output;
                    fixture.audio.arcOn = enabled;
                    observed.failWrite = failure;
                    bool validArc = (codec >= 1 && codec <= 5) || codec == 9;
                    assert(setArcOutputType(&fixture.audio, codec, 1, 6) == (!validArc || !failure));
                    assert(observed.writes.size() == (validArc ? 1u : 0u));
                    assert(fixture.audio.arcCodec == (validArc ? codec : 0));
                    assert(observed.transitions.size() == (validArc && enabled ? 1u : 0u));
                    if (validArc) {
                        assert(observed.writes[0].control == "Sndout ARC Outputtype");
                        assert((observed.writes[0].words == std::vector<int>{2, codec, 6, 0, 1}));
                    }
                    bool arcNotify = validArc && !failure &&
                        (output == 0x80000 || (enabled && (output == 0x400 || output == 0x400000)));
                    assert(observed.delays.size() == (arcNotify ? 1u : 0u));
                    if (arcNotify)
                        assert(observed.delays[0] == (output == 0x80000 ? 1 : codec));
                    observed = {};
                    observed.failWrite = failure;
                    bool validOptic = codec >= 1 && codec <= 4;
                    assert(setOpticOutputType(&fixture.audio, codec, 0) == (!validOptic || !failure));
                    assert(observed.writes.size() == (validOptic ? 1u : 0u));
                    assert(observed.transitions.empty());
                    if (validOptic) {
                        assert(observed.writes[0].control == "Sndout OPTIC Outputtype");
                        assert((observed.writes[0].words == std::vector<int>{2, codec, 0, 0, 0}));
                    }
                    bool opticNotify = validOptic && !failure && (output == 0x1000 || output == 2);
                    assert(observed.delays.size() == (opticNotify ? 1u : 0u));
                    if (opticNotify)
                        assert(observed.delays[0] == (output == 0x1000 ? 1 : codec));
                    cases += 2;
                }
            }
        }
    }
    return cases;
}

static unsigned testOnOff()
{
    unsigned cases = 0;
    for (int previousArc : {0, 1}) {
        for (int previousEarc : {0, 1}) {
            for (int arc : {-1, 0, 1, 2}) {
                for (int earc : {-1, 0, 1, 2}) {
                    for (int failure : {0, 1, 2}) {
                        Fixture fixture;
                        fixture.audio.arcOn = previousArc;
                        fixture.audio.earcOn = previousEarc;
                        observed.failWrite = failure;
                        observed.result = 5;
                        assert(setArcOnOff(&fixture.audio, arc, earc) == (failure ? 0 : 1));
                        bool transition = (!previousArc && arc == 1) || (!previousEarc && earc == 1);
                        assert(observed.transitions.size() == (transition ? 1u : 0u));
                        if (transition)
                            assert((observed.transitions[0] ==
                                    std::array<int, 4>{previousArc, previousEarc, 0, 0}));
                        assert(fixture.audio.arcOn == arc && fixture.audio.earcOn == earc);
                        assert(observed.writes.size() == (failure == 1 ? 1u : 2u));
                        assert(observed.writes[0].control == "Sndout ARC OnOff");
                        assert((observed.writes[0].words == std::vector<int>{arc}));
                        assert(observed.writes[0].cache[5] == arc && observed.writes[0].cache[6] == earc);
                        if (failure != 1) {
                            assert(observed.writes[1].control == "Sndout EARC OnOff");
                            assert((observed.writes[1].words == std::vector<int>{earc}));
                        }
                        assert(observed.delays.empty());
                        ++cases;
                    }
                }
            }
        }
    }
    return cases;
}

int main()
{
    unsigned cases = testEarc() + testArcAndOptic() + testOnOff();
    Fixture fixture;
    const int channelMarker = 0x01020304;
    assert(setEArcOutputType(&fixture.audio, 1, 1, channelMarker) == 1);
    unsigned char bytes[4];
    std::memcpy(bytes, &observed.writes[0].words[2], sizeof(bytes));
    assert(bytes[0] == 4 && bytes[1] == 3 && bytes[2] == 2 && bytes[3] == 1);
    umiLogContext = nullptr;
    assert(setArcOnOff(&fixture.audio, 0, 1) == 1);
    assert(!observed.logs.empty());
    std::printf("Native ARC/eARC/optical: %u behavior cases passed\n", cases + 2);
}