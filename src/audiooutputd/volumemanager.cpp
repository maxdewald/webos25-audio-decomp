#include "audiooutput.hpp"
using namespace Contract;

/* @0x1ba48 = _ZN13VolumeManagerC1ERN2LS6HandleEP9umiClient */
/* one in-memory output initially empty, unmuted, volume=100;
 * registers /audio/volume, with no subscription point or settings storage. */
VolumeManager::VolumeManager(LS::Handle& h, umiClient* c):handle(&h),client(c)
{
    registerCategory(*handle, "/audio/volume", volumeMethods, this);
}

/* @0x1bbe0 = _ZN13VolumeManager20setSoundOutputVolumeERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEh */
/* only update the currently tracked output. */
void VolumeManager::setSoundOutputVolume(const std::string& name, unsigned char value)
{
    if (name == output.soundOutput) output.volume = value;
}

/* @0x1bc08 = _ZN13VolumeManager20getSoundOutputVolumeERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
/* an untracked output reports 100, not a UMI hardware readback. */
unsigned char VolumeManager::getSoundOutputVolume(const std::string& name)
{
    return name == output.soundOutput ? output.volume : 100;
}

/* @0x1bc30 = _ZN13VolumeManager18setSoundOutputMuteERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEb */
/* the cached mute byte changes only for the tracked output. */
void VolumeManager::setSoundOutputMute(const std::string& name, bool value)
{
    if (name == output.soundOutput) output.muted = value;
}

/* @0x1bc58 = _ZN13VolumeManager17updateSoundOutputERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
/* changing route resets cached volume to UMI default and unmutes cache;
 * unchanged route preserves volume/mute. No driver setter or persistence here. */
void VolumeManager::updateSoundOutput(const std::string& name)
{
    if (name == output.soundOutput) return;
    output.soundOutput = name;
    output.volume = client->getDefaultVolume();
    output.muted = false;
}

/* @0x1bc94 = _ZN13VolumeManager3setERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKi */
/* internal forwarding narrows to unsigned byte without range checking. */
int VolumeManager::set(const std::string& name, const int& value)
{
    return client ? client->setOutputVolume(name, static_cast<unsigned char>(value)) : 2;
}

/* @0x1bd68 = _ZN13VolumeManager12muteSoundOutERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKi */
/* any nonzero internal int becomes true; absent client returns 2. */
int VolumeManager::muteSoundOut(const std::string& name, const int& value)
{
    return client ? client->setOutputMute(name, value != 0) : 2;
}

/* @0x1be44 = _ZN13VolumeManager16buildAudioStatusER11AudioOutput */
/* status consists of soundOutput, volume and muted (not mute). */
JValue VolumeManager::buildAudioStatus(AudioOutput& state)
{
    JValue response = pbnjson::Object();
    response.put("soundOutput", state.soundOutput);
    response.put("volume", static_cast<int>(state.volume));
    response.put("muted", state.muted);
    return response;
}

/* @0x1bf54 = _ZN13VolumeManager16buildAudioStatusEv */
/* volumeStatus is always a one-element array, even for an empty route. */
JValue VolumeManager::buildAudioStatus()
{
    JValue entries = array();
    entries.append(buildAudioStatus(output));
    JValue response = result(0);
    response.put("volumeStatus", entries);
    return response;
}

/* @0x1c088 = _ZN13VolumeManager9getStatusEv */
/* returns the service's cached status rather than a UMI query. */
JValue VolumeManager::getStatus()
{
    return buildAudioStatus();
}

/* @0x1c140 = _ZN13VolumeManager3setER9LSMessage */
/* raw c29a UXTB then <=100, not a full-int range check. Therefore 256
 * maps to zero, 356 to 100; cache updates only after successful forwarding. */
bool VolumeManager::set(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, volumeSchema, payload)) return true;
    std::string name = text(payload["soundOutput"]);
    unsigned char value = payload["volume"].asNumber<int>();
    if (value > 100) {
        respondError(m, "Volume out of range", 202);
        return true;
    }
    JValue response = result(set(name, static_cast<int>(value)));
    if (boolean(response["returnValue"])) {
        setSoundOutputVolume(name, value);
        response.put("soundOutput", name);
        response.put("volume", static_cast<int>(value));
    }
    respond(m, response);
    return true;
}

/* @0x1c620 = _ZN13VolumeManager2upER9LSMessage */
/* untracked outputs act as volume 100, so up returns code 202. */
bool VolumeManager::up(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, volumeOutputSchema, payload)) return true;
    std::string name = text(payload["soundOutput"]);
    int value = getSoundOutputVolume(name);
    if (value == 100) {
        respondError(m, "Volume already at max/min", 202);
        return true;
    }
    ++value;
    JValue response = result(set(name, value));
    if (boolean(response["returnValue"])) {
        setSoundOutputVolume(name, value);
        response.put("soundOutput", name);
        response.put("volume", value);
    }
    respond(m, response);
    return true;
}

/* @0x1cac8 = _ZN13VolumeManager4downER9LSMessage */
/* unlike up, down of an untracked output forwards 99 successfully. */
bool VolumeManager::down(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, volumeOutputSchema, payload)) return true;
    std::string name = text(payload["soundOutput"]);
    int value = getSoundOutputVolume(name);
    if (!value) {
        respondError(m, "Volume already at max/min", 202);
        return true;
    }
    --value;
    JValue response = result(set(name, value));
    if (boolean(response["returnValue"])) {
        setSoundOutputVolume(name, value);
        response.put("soundOutput", name);
        response.put("volume", value);
    }
    respond(m, response);
    return true;
}

/* @0x1cf88 = _ZN13VolumeManager12muteSoundOutER9LSMessage */
/* forwards even when cached mute already equals request. */
bool VolumeManager::muteSoundOut(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, volumeMuteSchema, payload)) return true;
    std::string name = text(payload["soundOutput"]);
    bool value = boolean(payload["mute"]);
    JValue response = result(muteSoundOut(name, static_cast<int>(value)));
    if (boolean(response["returnValue"])) {
        setSoundOutputMute(name, value);
        response.put("soundOutput", name);
        response.put("mute", value);
    }
    respond(m, response);
    return true;
}

/* @0x1d43c = _ZN13VolumeManager9getStatusER9LSMessage */
/* accepts only {}, so subscribe is not supported on volume status. */
bool VolumeManager::getStatus(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (request(m, emptySchema, payload)) respond(m, getStatus());
    return true;
}
