#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <set>
#include <string>
#include <vector>

enum UMI_AUDIO_SNDOUT_T : unsigned int {};

// @match src/libumi/arc_connection.cpp struct Audio;[\s\S]*?\n};\n\n#if[\s\S]*?#endif

namespace umi_arc_connection {

int logThreshold = 0;
int *umiLogContext = &logThreshold;

struct Backend {
    bool opened = false;
    bool muted = true;
    bool active = false;
    unsigned int volume = 0;
    std::string failure;
    bool failResources = false;
    int finalResult = 1;
    std::vector<std::string> transitions;
    std::vector<unsigned int> requested;
    std::vector<std::string> errors;
    std::vector<std::string> logSources;
    std::set<unsigned int> routes;
    std::vector<int> routeTransitions;
    int failRoute = -1;
};

static Backend backend;

extern "C" int _PmLogMsgKV(int *, int level, unsigned int, const char *, unsigned int,
                            const void *, const void *,
                            const char *, ...);

extern "C" int _PmLogMsgKV(int *, int level, unsigned int, const char *, unsigned int,
                            const void *, const void *,
                            const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    const char *source = va_arg(arguments, const char *);
    const char *function = va_arg(arguments, const char *);
    int line = va_arg(arguments, int);
    assert(source && function && line > 0);
    backend.logSources.emplace_back(source);
    if (level == 3)
        backend.errors.emplace_back(va_arg(arguments, const char *));
    va_end(arguments);
    return 0;
}

static int setElement(Audio *, const std::string &element, const int *values, int count)
{
    backend.transitions.push_back(element);
    assert(values[0] == 73);
    if (backend.failure == element)
        return 0;
    if (element == "Sndout Open") {
        assert(count == 1 && !backend.opened);
        backend.opened = true;
        return 1;
    }
    if (element == "Mute Output") {
        assert(count == 2);
        if (!values[1])
            assert(backend.opened && backend.active);
        backend.muted = values[1] != 0;
        return values[1] ? 1 : backend.finalResult;
    }
    assert(element == "Sndout Close" && count == 1);
    assert(backend.opened && backend.muted && !backend.active);
    backend.opened = false;
    return backend.finalResult;
}

static int setVolume(SoundEngine *, const unsigned int &volume)
{
    backend.volume = volume;
    return 0;
}

int getSoundOutputCommonType(Audio *, UMI_AUDIO_SNDOUT_T output)
{
    backend.requested.push_back(output);
    return 73;
}

int connectDisconnectResources(Audio *, int common, bool active)
{
    assert(common == 73);
    assert(backend.opened && backend.muted);
    backend.transitions.push_back(active ? "activate" : "release");
    if (backend.failResources)
        return 0;
    backend.active = active;
    return 1;
}

unsigned char connectOutput(Audio *, UMI_AUDIO_SNDOUT_T output)
{
    int position = static_cast<int>(backend.routeTransitions.size());
    backend.routeTransitions.push_back(output);
    if (position == backend.failRoute)
        return 0;
    backend.routes.insert(output);
    return 1;
}

unsigned char disconnectOutput(Audio *, UMI_AUDIO_SNDOUT_T output)
{
    int position = static_cast<int>(backend.routeTransitions.size());
    backend.routeTransitions.push_back(-static_cast<int>(output));
    if (position == backend.failRoute)
        return 0;
    backend.routes.erase(output);
    return 1;
}

// @match src/libumi/arc_connection.cpp static const char utilSource\[\][\s\S]*?audioSource\[\][\s\S]*?;
// @extract src/libumi/arc_connection.cpp connectExternalArcOutput connectSoundEngineArcOutput disconnectExternalArcOutput disconnectSoundEngineArcOutput setSoundEngineARCOutputMute setSoundEngineARCVolume getSoundOutArcCopyValue
// @extract src/libumi/arc_connection.cpp setSpeakerOpticalArcSoundOutput setSpeakerArcSoundOutput setTvSpeakerArcSoundOutput setTvSpeakerOpticalArcSoundOutput

}

using namespace umi_arc_connection;

static VTable controls = {{}, setElement};
static SoundEngineVTable volumeControls = {{}, setVolume};
static SoundEngine engine = {&volumeControls};

static Audio reset(bool changed, bool disconnecting)
{
    backend = Backend{};
    backend.opened = disconnecting;
    backend.muted = !disconnecting;
    backend.active = disconnecting || !changed;
    Audio audio{};
    audio.vtable = &controls;
    audio.engine = &engine;
    audio.currentOutput = 1;
    audio.requestedOutput = changed ? 2 : 1;
    return audio;
}

static void lifecycle()
{
    using Operation = int (*)(Audio *, UMI_AUDIO_SNDOUT_T);
    const Operation operations[] = {
        connectExternalArcOutput, connectSoundEngineArcOutput,
        disconnectExternalArcOutput, disconnectSoundEngineArcOutput,
    };
    for (unsigned int index = 0; index < 4; ++index) {
        const bool disconnecting = index >= 2;
        const unsigned int route = (index & 1) ? 0x80000 : 0x400;
        const std::string first = disconnecting ? "Mute Output" : "Sndout Open";
        const std::string last = disconnecting ? "Sndout Close" : "Mute Output";
        for (bool changed : {false, true}) {
            Audio audio = reset(changed, disconnecting);
            if (!changed && disconnecting)
                backend.active = false;
            backend.finalResult = 7;
            assert(operations[index](&audio, UMI_AUDIO_SNDOUT_T(0x80000)) == 7);
            assert(backend.opened == !disconnecting);
            assert(backend.muted == disconnecting);
            assert(backend.active == !disconnecting);
            const std::vector<std::string> expected = changed
                ? std::vector<std::string>{first, disconnecting ? "release" : "activate", last}
                : std::vector<std::string>{first, last};
            assert(backend.transitions == expected);
            assert(backend.requested == std::vector<unsigned int>(changed ? 3 : 2, route));
            assert(audio.currentOutput == 1 && audio.requestedOutput == (changed ? 2 : 1));
        }
        Audio audio = reset(true, disconnecting);
        backend.failure = first;
        assert(operations[index](&audio, UMI_AUDIO_SNDOUT_T(0x80000)) == 0);
        assert(backend.opened == disconnecting);
        assert(backend.muted == !disconnecting);
        assert(backend.active == disconnecting);
        assert(backend.transitions == std::vector<std::string>{first});
        assert(backend.errors.empty());

        audio = reset(true, disconnecting);
        backend.failResources = true;
        assert(operations[index](&audio, UMI_AUDIO_SNDOUT_T(0x80000)) == 0);
        assert(backend.opened && backend.muted);
        assert(backend.active == disconnecting);
        assert(backend.transitions.size() == 2);

        audio = reset(true, disconnecting);
        backend.failure = last;
        assert(operations[index](&audio, UMI_AUDIO_SNDOUT_T(0x80000)) == 0);
        assert(backend.opened && backend.muted);
        assert(backend.active == !disconnecting);
        assert(backend.transitions.size() == 3);

        for (int threshold : {3, 7}) {
            audio = reset(true, disconnecting);
            logThreshold = threshold;
            backend.failure = first;
            assert(operations[index](&audio, UMI_AUDIO_SNDOUT_T(0x80000)) == 0);
            assert(backend.errors == std::vector<std::string>{first});
            assert(backend.logSources.size() == (threshold == 7 ? 2u : 1u));
            for (const std::string &source : backend.logSources)
                assert(source == "umitvaudioutil.cpp");
        }
        logThreshold = 0;
    }
}

static void selectors()
{
    struct Case {
        unsigned int output;
        std::vector<int> opticalArc;
        std::vector<int> arc;
    };
    const Case cases[] = {
        {1, {2, 0x400}, {0x400}},
        {2, {1, 0x400}, {-2, 0x400, 1}},
        {0x400, {1, 2}, {1}},
        {0x200, {0x400}, {-2, 0x400}},
        {0x400000, {2}, {-0x400000, 0x400, 1}},
        {0x1000000, {-0x800000, 2, 0x400}, {-0x800000, 0x400}},
        {0x8000000, {-0x4000000, 2, 0x400}, {-0x4000000, 0x400}},
        {0x10000, {-0x200000, 2, 0x400}, {-0x200000, 0x400}},
        {0x100000, {-0x20000, 2, 0x400}, {-0x20000, 0x400}},
        {0x800, {-0x20, 2, 0x400}, {-0x20, 0x400}},
        {0x8000, {-0x8000, 1, 2, 0x400}, {-2}},
        {0, {0, 1, 2, 0x400}, {0x400, 1}},
        {0x40, {-0x40, 1, 2, 0x400}, {-0x40, 0x400, 1}},
    };
    unsigned int count = 0;
    for (const Case &test : cases) {
        for (bool triple : {false, true}) {
            const auto &expected = triple ? test.opticalArc : test.arc;
            for (int failure = -1; failure < static_cast<int>(expected.size()); ++failure) {
                Audio audio = reset(false, false);
                audio.selectedOutput = UMI_AUDIO_SNDOUT_T(test.output);
                backend.routes = {0, 1, 2, 0x20, 0x40, 0x400, 0x20000, 0x200000,
                                  0x400000, 0x800000, 0x4000000, 0x8000};
                auto finalRoutes = backend.routes;
                for (unsigned int index = 0; index < expected.size(); ++index) {
                    if (static_cast<int>(index) == failure)
                        continue;
                    if (expected[index] > 0 || (expected[index] == 0 && !triple))
                        finalRoutes.insert(expected[index]);
                    else
                        finalRoutes.erase(-expected[index]);
                }
                backend.failRoute = failure;
                int result = triple ? setSpeakerOpticalArcSoundOutput(&audio)
                                    : setSpeakerArcSoundOutput(&audio);
                assert(result == (failure == -1));
                assert(backend.routeTransitions == expected);
                assert(backend.routes == finalRoutes);
                ++count;
            }
        }
    }
    const Case transitions[] = {
        {1, {-2, -0x400}, {-0x400}},
        {2, {-1, -0x400}, {-0x400, -1, 2}},
        {0x400, {-1, -2}, {-1}},
        {0, {-1, -2, -0x400}, {-0x400, -1}},
        {0x40, {}, {}},
    };
    for (const Case &test : transitions) {
        for (bool triple : {false, true}) {
            const auto &expected = triple ? test.opticalArc : test.arc;
            for (int failure = -1; failure < static_cast<int>(expected.size()); ++failure) {
                Audio audio = reset(false, false);
                backend.routes = {1, 2, 0x400};
                auto finalRoutes = backend.routes;
                for (unsigned int index = 0; index < expected.size(); ++index) {
                    if (static_cast<int>(index) == failure)
                        continue;
                    if (expected[index] > 0)
                        finalRoutes.insert(expected[index]);
                    else
                        finalRoutes.erase(-expected[index]);
                }
                backend.failRoute = failure;
                int result = triple
                    ? setTvSpeakerOpticalArcSoundOutput(&audio, UMI_AUDIO_SNDOUT_T(test.output))
                    : setTvSpeakerArcSoundOutput(&audio, UMI_AUDIO_SNDOUT_T(test.output));
                assert(result == (failure == -1 && !expected.empty()));
                assert(backend.routeTransitions == expected);
                assert(backend.routes == finalRoutes);
                ++count;
            }
        }
    }
    std::printf("ARC route selectors: %u success/failure transitions passed\n", count);
}

int main()
{
    lifecycle();
    selectors();
    Audio cached = reset(true, false);
    assert(connectExternalArcOutput(&cached, UMI_AUDIO_SNDOUT_T(0x80000)) == 1);
    cached.currentOutput = cached.requestedOutput;
    backend.opened = false;
    backend.muted = true;
    backend.transitions.clear();
    assert(connectExternalArcOutput(&cached, UMI_AUDIO_SNDOUT_T(1)) == 1);
    assert(backend.transitions == std::vector<std::string>({"Sndout Open", "Mute Output"}));
    assert(backend.opened && backend.active && !backend.muted);
    for (bool muted : {false, true}) {
        Audio audio = reset(false, true);
        assert(setSoundEngineARCOutputMute(&audio, muted) == 0);
        assert(backend.muted == muted);
        assert(backend.requested == std::vector<unsigned int>{0x80000});
        backend.failure = "Mute Output";
        assert(setSoundEngineARCOutputMute(&audio, muted) == 2);
        assert(backend.muted == muted);
    }
    Audio muteFailure = reset(false, true);
    backend.failure = "Mute Output";
    logThreshold = 3;
    assert(setSoundEngineARCOutputMute(&muteFailure, true) == 2);
    assert(backend.errors == std::vector<std::string>{"Mute Output"});
    assert(backend.logSources == std::vector<std::string>{"umitvaudio.cpp"});
    logThreshold = 0;
    Audio audio = reset(false, false);
    for (unsigned int volume : {0u, 1u, 100u, 127u, 128u, 254u, 255u}) {
        setSoundEngineARCVolume(&audio, static_cast<unsigned char>(volume));
        assert(backend.volume == volume);
    }
    const std::pair<std::string, int> policies[] = {
        {"free", 1}, {"no_more", 2}, {"once", 4}, {"never", 8},
        {"", 0}, {"Free", 0}, {"never ", 0}, {"unknown", 0},
        {std::string("free\0extra", 10), 0},
    };
    for (bool defaultContext : {false, true}) {
        umiLogContext = defaultContext ? nullptr : &logThreshold;
        for (const auto &policy : policies)
            assert(getSoundOutArcCopyValue(&audio, policy.first) == policy.second);
    }
    umiLogContext = &logThreshold;
    std::puts("ARC lifecycle, selector failure states, log policy, mute, volume and copyright passed");
}