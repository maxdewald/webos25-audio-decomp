#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <initializer_list>
#include <set>
#include <string>
#include <vector>

// @match src/libumi/arc_policy.cpp (?s)\A.*\Z

struct Write {
    std::string endpoint;
    int codec;
    int mix;
    int channels;
};

static std::string inputCodec;
static std::set<std::string> earcCapabilities;
static std::set<std::string> wowCapabilities;
static std::vector<Write> writes;
static std::vector<std::string> events;
static bool countryJP;
static bool sourceAAC;
static bool aacOutput;
static bool forcedDolby;
static bool pcmInput;
static bool dtsInput;
static bool channelSupport;
static bool iamfSupport;
static bool writeResult;
static bool mutateInput;
static int inputChannels;
static int getterCalls;
static int disposals;
static int scenarios;
static UmiLogContext logContext;

extern "C" {
UmiLogContext *umiLogContext = &logContext;

int _PmLogMsgKV(UmiLogContext *, int level, unsigned keys, const char *id, ...)
{
    assert(keys == 0);
    assert(level == 3 || level == 7);
    assert(level == 7 ? id == nullptr : std::string(id) == "INFO_LOG");
    va_list arguments;
    va_start(arguments, id);
    assert(va_arg(arguments, int) == 0);
    assert(va_arg(arguments, void *) == nullptr);
    assert(va_arg(arguments, void *) == nullptr);
    const char *format = va_arg(arguments, const char *);
    char message[512];
    std::vsnprintf(message, sizeof(message), format, arguments);
    va_end(arguments);
    events.push_back(std::string("log:") + message);
    return 0;
}
}

namespace umi_arc_policy {

void disposeString(std::string *value)
{
    ++disposals;
    value->~basic_string();
}

std::string getInputCodecType(const Audio *)
{
    ++getterCalls;
    events.push_back("input");
    return inputCodec;
}

bool isEarcCapabilityAvailable(Audio *, const std::string &capability)
{
    events.push_back("E:" + capability);
    if (mutateInput)
        inputCodec.assign(256, 'x');
    return earcCapabilities.count(capability) != 0;
}

bool isWowCastCapabilityAvailable(Audio *, const std::string &capability)
{
    events.push_back("W:" + capability);
    return wowCapabilities.count(capability) != 0;
}

bool isCountryCodeJP(Audio *) { return countryJP; }
bool isInputSourceToSetAAC(const Audio *) { return sourceAAC; }
bool isAACOutputCodec(Audio *) { return aacOutput; }
bool isForcedDolbyNeeded(Audio *) { return forcedDolby; }
bool isInputCodecPCM(Audio *) { return pcmInput; }
bool isInputCodecDTS(Audio *) { events.push_back("DTS?"); return dtsInput; }
int getOpticOutputCodec(Audio *) { events.push_back("opticCodec"); return 2; }
int getOpticMixOption(Audio *) { events.push_back("opticMix"); return 1; }
bool supportOutputChannel(Audio *, int) { return channelSupport; }
int getInputAudioChannelNumber(const Audio *) { return inputChannels; }
bool isSupportIAMF(const Audio *) { return iamfSupport; }
bool isValidChannel(const Audio *, int channels) { return channels == 2 || channels == 6 || channels == 8; }

bool setOpticOutputType(Audio *, int codec, int mix)
{
    events.push_back("optic");
    writes.push_back({"optic", codec, mix, 0});
    return writeResult;
}

bool setArcOutputType(Audio *, int codec, int mix, int channels)
{
    events.push_back("arc");
    writes.push_back({"arc", codec, mix, channels});
    return writeResult;
}

bool setEArcOutputType(Audio *, int codec, int mix, int channels)
{
    events.push_back("earc");
    writes.push_back({"earc", codec, mix, channels});
    return writeResult;
}

void setWowCastOutputType(Audio *, int codec, int mix)
{
    events.push_back("wow");
    writes.push_back({"wow", codec, mix, 0});
}

}

using namespace umi_arc_policy;

static Audio reset(const std::string &codec = "unknown")
{
    ++scenarios;
    inputCodec = codec;
    earcCapabilities.clear();
    wowCapabilities.clear();
    writes.clear();
    events.clear();
    countryJP = sourceAAC = aacOutput = forcedDolby = pcmInput = dtsInput = false;
    channelSupport = iamfSupport = mutateInput = false;
    writeResult = true;
    inputChannels = getterCalls = disposals = 0;
    logContext.enabledLevel = 0;
    Audio audio{};
    audio.pcmChannels = 8;
    return audio;
}

static void expectWrites(std::initializer_list<Write> expected)
{
    assert(writes.size() == expected.size());
    auto actual = writes.begin();
    for (const Write &write : expected) {
        assert(actual->endpoint == write.endpoint);
        assert(actual->codec == write.codec);
        assert(actual->mix == write.mix);
        assert(actual->channels == write.channels);
        ++actual;
    }
}

static void testDirectCodecs()
{
    struct Direct { const char *input; const char *capability; int codec; };
    const Direct cases[] = {
        {"truehd", "TrueHD", 6}, {"truehd_atmos", "TrueHD", 6},
        {"mat", "MAT2PCM", 7}, {"mat_atmos", "MAT2PCM", 7},
        {"eac3", "DD+", 5}, {"eac3_atmos", "DD+", 5},
        {"ac3", "DD", 2}, {"pcm", "PCM", 1}, {"dts", "DTS", 4},
        {"dts_hd", "DTSHD", 8}, {"dts_hd_ma", "DTSHD_MA", 10},
        {"dts_x_p1", "DTSX_P1", 11}
    };
    for (const Direct &direct : cases) {
        Audio audio = reset(direct.input);
        earcCapabilities = wowCapabilities = {direct.capability};
        audio.mix = direct.codec & 1;
        assert(setOutputTypeForPassThrough(&audio));
        expectWrites({{"optic", 2, 1, 0}, {"arc", direct.codec, audio.mix, 0},
                      {"earc", direct.codec, audio.mix, 0}, {"wow", direct.codec, audio.mix, 0}});
        assert(getterCalls == 1);
        assert(events[0] == "input");
        assert(events[1] == std::string("E:") + direct.capability);
        assert(events[2] == std::string("W:") + direct.capability);
    }
}

static void testIndependentFallbacks()
{
    Audio audio = reset("truehd");
    earcCapabilities = {"TrueHD"};
    wowCapabilities = {"DD+"};
    assert(setOutputTypeForPassThrough(&audio));
    expectWrites({{"optic", 2, 1, 0}, {"arc", 1, 1, 0}, {"earc", 1, 1, 0},
                  {"wow", 5, 1, 0}, {"optic", 2, 1, 0}, {"arc", 6, 0, 0}, {"earc", 6, 0, 0}});
    assert(events[1] == "E:TrueHD" && events[2] == "W:TrueHD" && events[3] == "DTS?");

    audio = reset("truehd");
    earcCapabilities = {"DD"};
    wowCapabilities = {"TrueHD"};
    assert(setOutputTypeForPassThrough(&audio));
    expectWrites({{"optic", 2, 1, 0}, {"arc", 2, 1, 0}, {"earc", 2, 1, 0},
                  {"wow", 1, 1, 0}, {"wow", 6, 0, 0}});
    assert(events[1] == "E:TrueHD" && events[2] == "DTS?");

    audio = reset("truehd");
    earcCapabilities = {"DD"};
    wowCapabilities = {"DD+"};
    assert(setOutputTypeForPassThrough(&audio));
    expectWrites({{"optic", 2, 1, 0}, {"arc", 2, 1, 0}, {"earc", 2, 1, 0}, {"wow", 5, 1, 0},
                  {"optic", 2, 1, 0}, {"arc", 2, 1, 0}, {"earc", 2, 1, 0}, {"wow", 5, 1, 0}});
}

static void testAACAndFallbackPriorities()
{
    Audio audio = reset("aac");
    countryJP = sourceAAC = aacOutput = true;
    earcCapabilities = {"AAC", "MPEG4AACLC"};
    wowCapabilities = {"AAC"};
    assert(setOutputTypeForPassThrough(&audio));
    expectWrites({{"optic", 2, 1, 0}, {"arc", 9, 0, 0}, {"earc", 9, 0, 0}, {"wow", 3, 0, 0}});
    assert(events[1] == "E:MPEG4AACLC" && events[2] == "W:MPEG4AACLC" && events[3] == "W:AAC");
    events.clear();
    assert(getEarcAndARCOutputCodec(&audio) == 3);
    assert(events == std::vector<std::string>{"E:AAC"});
    earcCapabilities = {"MPEG4AACLC"};
    events.clear();
    assert(getEarcAndARCOutputCodec(&audio) == 3);
    assert((events == std::vector<std::string>{"E:AAC", "E:MPEG4AACLC"}));

    struct Fallback { const char *capability; int codec; };
    const Fallback cases[] = {{"MAT2PCM", 7}, {"DD+", 5}, {"DD", 2}, {"none", 1}};
    for (const Fallback &fallback : cases) {
        audio = reset();
        earcCapabilities = wowCapabilities = {fallback.capability};
        assert(getEarcAndARCOutputCodec(&audio) == fallback.codec);
        assert(getWowcastOutputCodec(&audio) == fallback.codec);
        pcmInput = true;
        assert(getEarcAndARCOutputCodec(&audio) == 1);
        assert(getWowcastOutputCodec(&audio) == 1);
        forcedDolby = true;
        assert(getEarcAndARCOutputCodec(&audio) == fallback.codec);
        assert(getWowcastOutputCodec(&audio) == fallback.codec);
    }
    audio = reset("ac3");
    countryJP = sourceAAC = true;
    earcCapabilities = wowCapabilities = {"DD"};
    assert(setOutputTypeForPassThrough(&audio));
    assert(writes.size() == 8 && writes[1].mix == 1 && writes[5].mix == 1);
}

static void testDTSPriorities()
{
    struct DTS { const char *capability; int codec; };
    const DTS cases[] = {{"DTSX_P2", 11}, {"DTSX_P1", 11}, {"DTSHD_MA", 10}, {"DTS", 4}, {"none", 1}};
    for (const DTS &dts : cases) {
        Audio audio = reset("dts_x_p2");
        dtsInput = true;
        earcCapabilities = {dts.capability};
        wowCapabilities = {"DTS"};
        assert(checkReEncodePriority(&audio));
        expectWrites({{"optic", 2, 1, 0}, {"arc", dts.codec, 1, 0},
                      {"earc", dts.codec, 1, 0}, {"wow", 4, 1, 0}});
        assert(events[0] == "DTS?" && events[1] == "E:DTSX_P2");
        audio = reset("dts_hd_ma");
        dtsInput = true;
        earcCapabilities = {"DTS"};
        wowCapabilities = {dts.capability};
        assert(checkReEncodePriority(&audio));
        expectWrites({{"optic", 2, 1, 0}, {"arc", 4, 1, 0},
                      {"earc", 4, 1, 0}, {"wow", dts.codec, 1, 0}});
    }
}

static void testExcludedAndJPEndpoints()
{
    const char *excluded[] = {"iamf", "dts_cd", "dts_express", "dts_x_p2", "TRUEHD", "AAC"};
    for (const char *codec : excluded) {
        Audio audio = reset(codec);
        earcCapabilities = wowCapabilities = {"TrueHD", "MAT2PCM", "DTSX_P2", "AAC"};
        assert(setOutputTypeForPassThrough(&audio));
        assert(writes.size() == 8);
        assert(writes[1].codec == 7 && writes[1].mix == 1);
    }
    Audio audio = reset("aac");
    countryJP = sourceAAC = aacOutput = true;
    earcCapabilities = {"MPEG4AACLC"};
    wowCapabilities = {"DD"};
    assert(setOutputTypeForPassThrough(&audio));
    expectWrites({{"optic", 2, 1, 0}, {"arc", 3, 1, 0}, {"earc", 3, 1, 0}, {"wow", 2, 1, 0},
                  {"optic", 2, 1, 0}, {"arc", 9, 0, 0}, {"earc", 9, 0, 0}});
    audio = reset("aac");
    countryJP = sourceAAC = aacOutput = true;
    earcCapabilities = {"DD"};
    wowCapabilities = {"MPEG4AACLC"};
    assert(setOutputTypeForPassThrough(&audio));
    expectWrites({{"optic", 2, 1, 0}, {"arc", 2, 1, 0}, {"earc", 2, 1, 0},
                  {"wow", 3, 1, 0}, {"wow", 9, 0, 0}});
}

static void testChannelsAndFailures()
{
    Audio audio = reset("pcm");
    channelSupport = true;
    inputChannels = 8;
    audio.pcmChannels = 6;
    setOutputType(&audio, 1, 0);
    expectWrites({{"optic", 2, 1, 0}, {"arc", 1, 1, 6}, {"earc", 1, 1, 6}});
    assert(audio.mix == 0);
    writes.clear();
    inputChannels = 2;
    setOutputType(&audio, 1, 0);
    assert(writes[1].mix == 0 && writes[1].channels == 2);
    writes.clear();
    inputChannels = 3;
    setOutputType(&audio, 1, 0);
    assert(writes[1].mix == 1 && writes[1].channels == 0);
    writes.clear();
    inputChannels = 6;
    audio.pcmChannels = 3;
    setOutputType(&audio, 1, 0);
    assert(writes[1].mix == 1 && writes[1].channels == 0);
    writes.clear();
    inputCodec = "iamf";
    iamfSupport = true;
    setOutputType(&audio, 1, 0);
    assert(writes[1].mix == 1 && writes[1].channels == 6);
    writes.clear();
    channelSupport = false;
    setOutputType(&audio, 1, 0);
    assert(writes[1].mix == 0 && writes[1].channels == 0);

    audio = reset("ac3");
    earcCapabilities = wowCapabilities = {"DD"};
    writeResult = false;
    assert(setOutputTypeForPassThrough(&audio));
    assert(writes.size() == 4);
}

static void testEmptyAndOwnedStrings()
{
    Audio audio = reset("");
    assert(setOutputTypeForPassThrough(&audio));
    assert(writes.size() == 4 && getterCalls == 1);
    assert(events[0] == "input" && events[1] == "DTS?");
    audio = reset("ac3");
    earcCapabilities = wowCapabilities = {"DD"};
    mutateInput = true;
    assert(setOutputTypeForPassThrough(&audio));
    assert(writes[1].codec == 2 && writes[3].codec == 2);
    assert(inputCodec.size() == 256 && getterCalls == 1);
    {
        std::string text(128, 'q');
        CapabilityString owned = capabilityString(text.c_str());
        text.clear();
        assert(static_cast<const std::string &>(owned).size() == 128);
    }
    assert(disposals >= 3);
    audio = reset(std::string(128, 'z'));
    assert(setOutputTypeForPassThrough(&audio));
    assert(writes.size() == 8 && getterCalls == 1);
}

static void testLogging()
{
    Audio audio = reset("ac3");
    earcCapabilities = wowCapabilities = {"DD"};
    umiLogContext = nullptr;
    assert(setOutputTypeForPassThrough(&audio));
    assert(events[1] == "log:umitvaudiosettings.cpp:setOutputTypeForPassThrough(1826) input codec value is ac3");
    umiLogContext = &logContext;
    events.clear();
    logContext.enabledLevel = 3;
    writeResult = false;
    setOutputType(&audio, 2, 0);
    assert(events[3] == "log:umitvaudiosettings.cpp:setOutputType(2135) Optic outputType Error(codec: 2)");
    assert(events[5] == "log:umitvaudiosettings.cpp:setOutputType(2171) ARC outputType Error(codec: 2, ch:0)");
    assert(events[7] == "log:umitvaudiosettings.cpp:setOutputType(2174) eARC outputType Error(codec: 2, ch: 0)");
    events.clear();
    writeResult = true;
    channelSupport = true;
    inputChannels = 3;
    setOutputType(&audio, 1, 0);
    assert(events[3] == "log:umitvaudiosettings.cpp:setOutputType(2163) channel is invalid (input ch: 3, dev ch:0)");
    events.clear();
    inputChannels = 8;
    audio.pcmChannels = 6;
    setOutputType(&audio, 1, 0);
    assert(events[3] == "log:umitvaudiosettings.cpp:setOutputType(2158) Setting channel (input ch:8, dev ch:6, output ch:6, mixOption:1)");
    events.clear();
    inputCodec = "iamf";
    iamfSupport = true;
    setOutputType(&audio, 1, 0);
    assert(events[4] == "log:umitvaudiosettings.cpp:setOutputType(2146) Setting channel for IAMF (input ch:8, output ch:8, mixOption:1)");
    events.clear();
    logContext.enabledLevel = 2;
    writeResult = false;
    setOutputType(&audio, 1, 0);
    for (const std::string &event : events)
        assert(event.compare(0, 4, "log:") != 0);
}

int main()
{
    testDirectCodecs();
    testIndependentFallbacks();
    testAACAndFallbackPriorities();
    testDTSPriorities();
    testExcludedAndJPEndpoints();
    testChannelsAndFailures();
    testEmptyAndOwnedStrings();
    testLogging();
    std::printf("%d ARC policy scenarios passed with native declarations and owned strings\n", scenarios);
}