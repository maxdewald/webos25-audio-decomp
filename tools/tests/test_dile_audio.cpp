// @cflags -std=c++14 -Wpedantic -Wconversion -Wsign-conversion -Wshadow
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "../../src/libdile_audio_direct/direct.cpp"
#include "../../src/libdile_audio_pcm/pcm.cpp"

namespace {
unsigned int callback_calls;

void callback_mock()
{
    ++callback_calls;
}

struct OpaqueArgument {
    std::array<std::uint32_t, 12> words;
};
}

int main()
{
    using Stub = int (*)(...);
    const std::array<Stub, 15> stubs = {{
        DILE_AUDIO_DIRECT_Open,
        DILE_AUDIO_DIRECT_Start,
        DILE_AUDIO_DIRECT_Write,
        DILE_AUDIO_DIRECT_Stop,
        DILE_AUDIO_DIRECT_Close,
        DILE_AUDIO_DIRECT_Pause,
        DILE_AUDIO_DIRECT_Resume,
        DILE_AUDIO_DIRECT_Flush,
        DILE_AUDIO_DIRECT_GetBufferInfo,
        DILE_AUDIO_DIRECT_SetNoDelayParam,
        DILE_AUDIO_PCM_RegSendPCMCallback,
        DILE_AUDIO_PCM_StartUpload,
        DILE_AUDIO_PCM_StopUpload,
        DILE_AUDIO_PCM_SetVolume,
        DILE_AUDIO_PCM_SetChannel,
    }};
    std::array<unsigned char, 64> output;
    output.fill(0xa5);
    const auto before = output;
    OpaqueArgument argument = {{}};
    argument.words.fill(0xdeadbeefU);
    const auto argument_before = argument.words;
    auto *const unreadable = reinterpret_cast<void *>(std::uintptr_t{1});

    for (std::size_t round = 0; round < 128; ++round) {
        for (const auto stub : stubs) {
            assert(stub() == -1);
            assert(stub(nullptr, 0U, 0.0) == -1);
            assert(stub(unreadable, output.data(), output.size(), callback_mock,
                        argument, 0xffffffffU) == -1);
            assert(output == before);
            assert(argument.words == argument_before);
            assert(callback_calls == 0);
        }
        assert(DILE_AUDIO_DIRECT_Start(argument) == -1);
        assert(DILE_AUDIO_PCM_SetVolume(argument) == -1);
    }
    std::puts("DILE: 15 stubs return -1; pointers/outputs untouched; callback never invoked");
}
