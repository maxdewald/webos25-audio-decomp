#include <cstddef>
#include <string>

enum UMI_AUDIO_SNDOUT_T : unsigned int;

namespace umi_arc_connection {

struct Audio;
struct VTable {
    void (*preceding[19])();
    int (*setElement)(Audio *, const std::string &, const int *, int);
};

struct SoundEngine;
struct SoundEngineVTable {
    void (*preceding[7])();
    int (*setVolume)(SoundEngine *, const unsigned int &);
};

struct SoundEngine {
    SoundEngineVTable *vtable;
};

struct Audio {
    VTable *vtable;
    unsigned char reserved[0xc4 - sizeof(VTable *)];
    UMI_AUDIO_SNDOUT_T selectedOutput;
    unsigned char reservedOutput[4];
    int currentOutput;
    int requestedOutput;
    unsigned char reservedEngine[0x224 - 0xd4];
    SoundEngine *engine;
};

#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(VTable, setElement) == 0x4c, "ALSA control slot");
static_assert(offsetof(Audio, currentOutput) == 0xcc, "current output offset");
static_assert(offsetof(Audio, engine) == 0x224, "sound-engine offset");
static_assert(offsetof(SoundEngineVTable, setVolume) == 0x1c, "volume slot");
#endif

static const char utilSource[] =
    "/usr/src/debug/lib32-umi/1.0.0-1.187.ptl4tv.13-r0/git/src/umitvaudioutil.cpp";
static const char audioSource[] =
    "/usr/src/debug/lib32-umi/1.0.0-1.187.ptl4tv.13-r0/git/src/umitvaudio.cpp";

extern int *umiLogContext __asm__("umiLogContext");
extern "C" int _PmLogMsgKV(int *, int, unsigned int, const char *, unsigned int,
                            const void *, const void *,
                            const char *, ...);
int getSoundOutputCommonType(Audio *, UMI_AUDIO_SNDOUT_T)
    __asm__("_ZN10umiTVAudio24getSoundOutputCommonTypeE18UMI_AUDIO_SNDOUT_T");
int connectDisconnectResources(Audio *, int, bool)
    __asm__("_ZN10umiTVAudio26connectDisconnectResourcesEib");
unsigned char connectOutput(Audio *, UMI_AUDIO_SNDOUT_T)
    __asm__("_ZN10umiTVAudio13connectOutputE18UMI_AUDIO_SNDOUT_T");
unsigned char disconnectOutput(Audio *, UMI_AUDIO_SNDOUT_T)
    __asm__("_ZN10umiTVAudio16disconnectOutputE18UMI_AUDIO_SNDOUT_T");

int connectExternalArcOutput(Audio *, UMI_AUDIO_SNDOUT_T) __asm__("_ZN10umiTVAudio24connectExternalArcOutputE18UMI_AUDIO_SNDOUT_T");
/* @0x5511c: opens external ARC, activates changed resources, then unmutes it. */
int connectExternalArcOutput(Audio *self, UMI_AUDIO_SNDOUT_T)
{
    if (!umiLogContext || *umiLogContext > 6)
        _PmLogMsgKV(umiLogContext, 7, 0, 0, 0, 0, 0,
                    "%s:%s(%d) Selecting external_arc soundOutput",
                    utilSource + 0x3a, "connectExternalArcOutput", 0x21a);
    int values[5];
    values[0] = getSoundOutputCommonType(self, UMI_AUDIO_SNDOUT_T(0x400));
    int result = self->vtable->setElement(self, std::string("Sndout Open", 11), values, 1);
    if (result) {
        if (self->currentOutput != self->requestedOutput) {
            int output = getSoundOutputCommonType(self, UMI_AUDIO_SNDOUT_T(0x400));
            if (!connectDisconnectResources(self, output, true))
                return 0;
        }
        values[0] = getSoundOutputCommonType(self, UMI_AUDIO_SNDOUT_T(0x400));
        values[1] = 0;
        result = self->vtable->setElement(self, std::string("Mute Output", 11), values, 2);
        if (result)
            return result;
        if (!umiLogContext || *umiLogContext > 2) {
            _PmLogMsgKV(umiLogContext, 3, 0, "CONTROL_ELEMENT_SET_ERROR", 0, 0, 0,
                        "%s:%s(%d) Did not able to set the element: %s for external arc",
                        utilSource + 0x3a, "connectExternalArcOutput", 0x22b,
                        "Mute Output");
            return 0;
        }
    } else if (!umiLogContext || *umiLogContext > 2) {
        _PmLogMsgKV(umiLogContext, 3, 0, "CONTROL_ELEMENT_SET_ERROR", 0, 0, 0,
                    "%s:%s(%d) Did not able to set the element: %s for external arc",
                    utilSource + 0x3a, "connectExternalArcOutput", 0x21d,
                    "Sndout Open");
        return 0;
    }
    return 0;
}

int connectSoundEngineArcOutput(Audio *, UMI_AUDIO_SNDOUT_T) __asm__("_ZN10umiTVAudio27connectSoundEngineArcOutputE18UMI_AUDIO_SNDOUT_T");
/* @0x56750: opens the selected sound-engine ARC route before unmuting. */
int connectSoundEngineArcOutput(Audio *self, UMI_AUDIO_SNDOUT_T output)
{
    if (!umiLogContext || *umiLogContext > 6)
        _PmLogMsgKV(umiLogContext, 7, 0, 0, 0, 0, 0,
                    "%s:%s(%d) Selecting external_arc soundOutput",
                    utilSource + 0x3a, "connectSoundEngineArcOutput", 0x351);
    int values[5];
    values[0] = getSoundOutputCommonType(self, output);
    int result = self->vtable->setElement(self, std::string("Sndout Open", 11), values, 1);
    if (result) {
        if (self->currentOutput != self->requestedOutput) {
            int common = getSoundOutputCommonType(self, output);
            if (!connectDisconnectResources(self, common, true))
                return 0;
        }
        values[0] = getSoundOutputCommonType(self, output);
        values[1] = 0;
        result = self->vtable->setElement(self, std::string("Mute Output", 11), values, 2);
        if (result)
            return result;
        if (!umiLogContext || *umiLogContext > 2) {
            _PmLogMsgKV(umiLogContext, 3, 0, "CONTROL_ELEMENT_SET_ERROR", 0, 0, 0,
                        "%s:%s(%d) Did not able to set the element: %s for external se arc",
                        utilSource + 0x3a, "connectSoundEngineArcOutput", 0x362,
                        "Mute Output");
            return 0;
        }
    } else if (!umiLogContext || *umiLogContext > 2) {
        _PmLogMsgKV(umiLogContext, 3, 0, "CONTROL_ELEMENT_SET_ERROR", 0, 0, 0,
                    "%s:%s(%d) Did not able to set the element: %s for external se arc",
                    utilSource + 0x3a, "connectSoundEngineArcOutput", 0x354,
                    "Sndout Open");
        return 0;
    }
    return 0;
}

int disconnectSoundEngineArcOutput(Audio *, UMI_AUDIO_SNDOUT_T) __asm__("_ZN10umiTVAudio30disconnectSoundEngineArcOutputE18UMI_AUDIO_SNDOUT_T");
/* @0x5796c: mutes sound-engine ARC, releases changed resources, then closes. */
int disconnectSoundEngineArcOutput(Audio *self, UMI_AUDIO_SNDOUT_T output)
{
    if (!umiLogContext || *umiLogContext > 6)
        _PmLogMsgKV(umiLogContext, 7, 0, 0, 0, 0, 0,
                    "%s:%s(%d) Disconnect soundoutput %d",
                    utilSource + 0x3a, "disconnectSoundEngineArcOutput", 0x4a0, output);
    int values[5];
    values[0] = getSoundOutputCommonType(self, output);
    values[1] = 1;
    int result = self->vtable->setElement(self, std::string("Mute Output", 11), values, 2);
    if (result) {
        if (self->currentOutput != self->requestedOutput) {
            int common = getSoundOutputCommonType(self, output);
            if (!connectDisconnectResources(self, common, false))
                return 0;
        }
        values[0] = getSoundOutputCommonType(self, output);
        result = self->vtable->setElement(self, std::string("Sndout Close", 12), values, 1);
        if (result)
            return result;
        if (!umiLogContext || *umiLogContext > 2) {
            _PmLogMsgKV(umiLogContext, 3, 0, "CONTROL_ELEMENT_SET_ERROR", 0, 0, 0,
                        "%s:%s(%d) Did not able to set the element: %s for external se arc",
                        utilSource + 0x3a, "disconnectSoundEngineArcOutput", 0x4b2,
                        "Sndout Close");
            return 0;
        }
    } else if (!umiLogContext || *umiLogContext > 2) {
        _PmLogMsgKV(umiLogContext, 3, 0, "CONTROL_ELEMENT_SET_ERROR", 0, 0, 0,
                    "%s:%s(%d) Did not able to set the element: %s for external se arc",
                    utilSource + 0x3a, "disconnectSoundEngineArcOutput", 0x4a5,
                    "Mute Output");
        return 0;
    }
    return 0;
}

int disconnectExternalArcOutput(Audio *, UMI_AUDIO_SNDOUT_T) __asm__("_ZN10umiTVAudio27disconnectExternalArcOutputE18UMI_AUDIO_SNDOUT_T");
/* @0x57b6c: external ARC always uses route 0x400, including resource release. */
int disconnectExternalArcOutput(Audio *self, UMI_AUDIO_SNDOUT_T output)
{
    if (!umiLogContext || *umiLogContext > 6)
        _PmLogMsgKV(umiLogContext, 7, 0, 0, 0, 0, 0,
                    "%s:%s(%d) Disconnect soundoutput %d",
                    utilSource + 0x3a, "disconnectExternalArcOutput", 0x4be, output);
    int values[5];
    values[0] = getSoundOutputCommonType(self, UMI_AUDIO_SNDOUT_T(0x400));
    values[1] = 1;
    int result = self->vtable->setElement(self, std::string("Mute Output", 11), values, 2);
    if (result) {
        if (self->currentOutput != self->requestedOutput) {
            int common = getSoundOutputCommonType(self, UMI_AUDIO_SNDOUT_T(0x400));
            if (!connectDisconnectResources(self, common, false))
                return 0;
        }
        values[0] = getSoundOutputCommonType(self, UMI_AUDIO_SNDOUT_T(0x400));
        result = self->vtable->setElement(self, std::string("Sndout Close", 12), values, 1);
        if (result)
            return result;
        if (!umiLogContext || *umiLogContext > 2) {
            _PmLogMsgKV(umiLogContext, 3, 0, "CONTROL_ELEMENT_SET_ERROR", 0, 0, 0,
                        "%s:%s(%d) Did not able to set the element: %s for external arc",
                        utilSource + 0x3a, "disconnectExternalArcOutput", 0x4d1,
                        "Sndout Close");
            return 0;
        }
    } else if (!umiLogContext || *umiLogContext > 2) {
        _PmLogMsgKV(umiLogContext, 3, 0, "CONTROL_ELEMENT_SET_ERROR", 0, 0, 0,
                    "%s:%s(%d) Did not able to set the element: %s for external arc",
                    utilSource + 0x3a, "disconnectExternalArcOutput", 0x4c3,
                    "Mute Output");
        return 0;
    }
    return 0;
}

int setSoundEngineARCOutputMute(Audio *, const bool &) __asm__("_ZN10umiTVAudio27setSoundEngineARCOutputMuteERKb");
/* @0x31fb0: UMI status 0 on success, 2 on an ALSA mute failure. */
int setSoundEngineARCOutputMute(Audio *self, const bool &mute)
{
    int values[5];
    values[0] = getSoundOutputCommonType(self, UMI_AUDIO_SNDOUT_T(0x80000));
    values[1] = mute;
    int result = self->vtable->setElement(self, std::string("Mute Output", 11), values, 2);
    if (!result) {
        if (!umiLogContext || *umiLogContext > 2) {
            _PmLogMsgKV(umiLogContext, 3, 0, "CONTROL_ELEMENT_SET_ERROR", 0, 0, 0,
                        "%s:%s(%d) Did not able to set the element: %s on tv speaker",
                        audioSource + 0x3a, "setSoundEngineARCOutputMute", 0x57c,
                        "Mute Output");
            return 2;
        }
        return 2;
    }
    return 0;
}

void setSoundEngineARCVolume(Audio *, unsigned char) __asm__("_ZN10umiTVAudio23setSoundEngineARCVolumeEh");
/* @0x3215c: forwards the complete unsigned-byte range without clamping. */
void setSoundEngineARCVolume(Audio *self, unsigned char volume)
{
    unsigned int value = volume;
    self->engine->vtable->setVolume(self->engine, value);
}

int getSoundOutArcCopyValue(Audio *, const std::string &) __asm__("_ZN10umiTVAudio23getSoundOutArcCopyValueERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE");
/* @0x581e0: ARC copyright mask, with unknown strings mapped to zero. */
int getSoundOutArcCopyValue(Audio *, const std::string &copyright)
{
    if (!umiLogContext || *umiLogContext > 6)
        _PmLogMsgKV(umiLogContext, 7, 0, 0, 0, 0, 0,
                    "%s:%s(%d) Copyright value received : %s", utilSource + 0x3a,
                    "getSoundOutArcCopyValue", 0x568, copyright.c_str());
    int value;
    if (copyright.compare("free") == 0)
        value = 1;
    else if (copyright.compare("no_more") == 0)
        value = 2;
    else if (copyright.compare("once") == 0)
        value = 4;
    else
        value = (copyright.compare("never") == 0) << 3;
    if (!umiLogContext || *umiLogContext > 6)
        _PmLogMsgKV(umiLogContext, 7, 0, 0, 0, 0, 0,
                    "%s:%s(%d) getSoundOutArcCopyValue - arc value is %d",
                    utilSource + 0x3a, "getSoundOutArcCopyValue", 0x575, value);
    return value;
}

int setSpeakerOpticalArcSoundOutput(Audio *) __asm__("_ZN10umiTVAudio31setSpeakerOpticalArcSoundOutputEv");
/* @0x31140: preserves compatible speaker routes and adds missing optical/ARC outputs. */
int setSpeakerOpticalArcSoundOutput(Audio *self)
{
    UMI_AUDIO_SNDOUT_T output = self->selectedOutput;
    unsigned char result;
    if (output == 1) {
        output = UMI_AUDIO_SNDOUT_T(2);
        goto addArc;
    }
    if (output == 2) {
        output = UMI_AUDIO_SNDOUT_T(1);
        goto addArc;
    }
    if (output == 0x400) {
        result = connectOutput(self, UMI_AUDIO_SNDOUT_T(1));
        result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(2));
        return result;
    }
    if (output == 0x200)
        return connectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
    if (output == 0x400000)
        return connectOutput(self, UMI_AUDIO_SNDOUT_T(2));
    if (output == 0x1000000) {
        output = UMI_AUDIO_SNDOUT_T(0x800000);
        goto replaceExternal;
    }
    if (output == 0x8000000) {
        output = UMI_AUDIO_SNDOUT_T(0x4000000);
        goto replaceExternal;
    }
    if (output == 0x10000) {
        output = UMI_AUDIO_SNDOUT_T(0x200000);
        goto replaceExternal;
    }
    if (output == 0x100000) {
        output = UMI_AUDIO_SNDOUT_T(0x20000);
        goto replaceExternal;
    }
    if (output == 0x800) {
        output = UMI_AUDIO_SNDOUT_T(0x20);
        goto replaceExternal;
    }
    result = disconnectOutput(self, output);
    result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(1));
    result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(2));
    result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
    return result;

replaceExternal:
    result = disconnectOutput(self, output);
    result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(2));
    result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
    return result;

addArc:
    result = connectOutput(self, output);
    result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
    return result;
}

int setSpeakerArcSoundOutput(Audio *) __asm__("_ZN10umiTVAudio24setSpeakerArcSoundOutputEv");
/* @0x31538: adds speaker/ARC while releasing only the incompatible external leg. */
int setSpeakerArcSoundOutput(Audio *self)
{
    UMI_AUDIO_SNDOUT_T output = self->selectedOutput;
    unsigned char result;
    if (output == 1)
        return connectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
    if (output == 0x400)
        return connectOutput(self, UMI_AUDIO_SNDOUT_T(1));
    if (output == 0x8000000) {
        output = UMI_AUDIO_SNDOUT_T(0x4000000);
        goto replaceExternal;
    }
    if (output == 0x200) {
        output = UMI_AUDIO_SNDOUT_T(2);
        goto replaceExternal;
    }
    if (output == 0x800) {
        output = UMI_AUDIO_SNDOUT_T(0x20);
        goto replaceExternal;
    }
    if (output == 0x10000) {
        output = UMI_AUDIO_SNDOUT_T(0x200000);
        goto replaceExternal;
    }
    if (output == 0x100000) {
        output = UMI_AUDIO_SNDOUT_T(0x20000);
        goto replaceExternal;
    }
    if (output == 0x1000000) {
        output = UMI_AUDIO_SNDOUT_T(0x800000);
        goto replaceExternal;
    }
    if (output == 0x8000)
        return disconnectOutput(self, UMI_AUDIO_SNDOUT_T(2));
    if (output == 0) {
        result = connectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
        result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(1));
        return result;
    }
    result = disconnectOutput(self, output);
    result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
    result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(1));
    return result;

replaceExternal:
    result = disconnectOutput(self, output);
    result &= connectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
    return result;
}

int setTvSpeakerArcSoundOutput(Audio *, UMI_AUDIO_SNDOUT_T) __asm__("_ZN10umiTVAudio26setTvSpeakerArcSoundOutputE18UMI_AUDIO_SNDOUT_T");
/* @0x319f0: keeps only the requested member of an existing speaker/ARC pair. */
int setTvSpeakerArcSoundOutput(Audio *self, UMI_AUDIO_SNDOUT_T output)
{
    if (output == 1)
        return disconnectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
    if (output == 2) {
        unsigned char result = disconnectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
        result &= disconnectOutput(self, UMI_AUDIO_SNDOUT_T(1));
        return result & connectOutput(self, output);
    }
    if (output == 0x400)
        return disconnectOutput(self, UMI_AUDIO_SNDOUT_T(1));
    if (output == 0) {
        unsigned char result = disconnectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
        return result & disconnectOutput(self, UMI_AUDIO_SNDOUT_T(1));
    }
    return 0;
}

int setTvSpeakerOpticalArcSoundOutput(Audio *, UMI_AUDIO_SNDOUT_T) __asm__("_ZN10umiTVAudio33setTvSpeakerOpticalArcSoundOutputE18UMI_AUDIO_SNDOUT_T");
/* @0x31ab0: removes the unrequested legs of a speaker/optical/ARC triple. */
int setTvSpeakerOpticalArcSoundOutput(Audio *self, UMI_AUDIO_SNDOUT_T output)
{
    unsigned char result;
    if (output == 1) {
        output = UMI_AUDIO_SNDOUT_T(2);
        goto keepSpeakerOrOptical;
    }
    if (output == 2) {
        output = UMI_AUDIO_SNDOUT_T(1);
        goto keepSpeakerOrOptical;
    }
    if (output == 0x400) {
        result = disconnectOutput(self, UMI_AUDIO_SNDOUT_T(1));
        return result & disconnectOutput(self, UMI_AUDIO_SNDOUT_T(2));
    }
    if (output == 0) {
        result = disconnectOutput(self, UMI_AUDIO_SNDOUT_T(1));
        result &= disconnectOutput(self, UMI_AUDIO_SNDOUT_T(2));
        return result & disconnectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
    }
    return 0;

keepSpeakerOrOptical:
    result = disconnectOutput(self, output);
    return result & disconnectOutput(self, UMI_AUDIO_SNDOUT_T(0x400));
}

}