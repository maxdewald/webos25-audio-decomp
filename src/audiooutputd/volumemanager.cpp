#include "audiooutput.hpp"
using namespace Contract;

/* @0x1ba48 = _ZN13VolumeManagerC1ERN2LS6HandleEP9umiClient */
/* one in-memory output initially empty, unmuted, volume=100;
 * registers /audio/volume, with no subscription point or settings storage. */
VolumeManager::VolumeManager(LS::Handle& h, umiClient* c):handle(&h),client(c)
{
    try {
        registerCategory(*handle, "/audio/volume", volumeMethods, this);
    } catch (LS::Error& error) {
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 2)
            _PmLogMsgKV(getPmLogContext(), 3, 0, "LS2_SUBSCRIBE_FAILED", 0, 0, 0,
                        "%s - VolumeManager API's registration Failed.", error.what());
    }
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
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_SET_VOLUME_CALL", 0, 0, 0,
                    "set volume request for soundOutput:%s volume:%d", name.c_str(), value);
    int status = client ? client->setOutputVolume(name, static_cast<unsigned char>(value)) : 2;
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_SET_VOLUME_CALL", 0, 0, 0,
                    "set volume umi returnStatus %d", status);
    return status;
}

/* @0x1bd68 = _ZN13VolumeManager12muteSoundOutERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKi */
/* any nonzero internal int becomes true; absent client returns 2. */
int VolumeManager::muteSoundOut(const std::string& name, const int& value)
{
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_MUTE_SOUNDOUTPUT_CALL", 0, 0, 0,
                    "muteSoundOut for soundOutput:%s muteFlag:%d", name.c_str(), value);
    int status = client ? client->setOutputMute(name, value != 0) : 2;
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_MUTE_SOUNDOUTPUT_CALL", 0, 0, 0,
                    "muteSoundOut request umi returnStatus %d", status);
    return status;
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
    pbnjson::JArray entries;
    JValue response = pbnjson::Object();
    entries.append(buildAudioStatus(output));
    response.put("returnValue", true);
    response.put("volumeStatus", entries);
    return response;
}

/* @0x1c088 = _ZN13VolumeManager9getStatusEv */
/* returns the service's cached status rather than a UMI query. */
JValue VolumeManager::getStatus()
{
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_GET_STATUS_CALL", 0, 0, 0,
                    "Volume getStatus request");
    JValue response = buildAudioStatus();
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5) {
        void* context = getPmLogContext();
        _PmLogMsgKV(context, 6, 0, "AUDIO_GET_STATUS_CALL", 0, 0, 0,
                    "Volume getStatus response:%s", response.stringify().c_str());
    }
    return response;
}

/* @0x1c140 = _ZN13VolumeManager3setER9LSMessage */
/* raw c29a UXTB then <=100, not a full-int range check. Therefore 256
 * maps to zero, 356 to 100; cache updates only after successful forwarding. */
bool VolumeManager::set(LSMessage& message)
{
    std::string name;
    int code = 0;
    LS::Message m(message);
    JValue payload;
    std::string schema = R"({"type":"object","properties":{"soundOutput":{"type":"string"},"volume":{"type":"integer"}},"required":["soundOutput","volume"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    name = text(payload["soundOutput"]);
    signed char value = payload["volume"].asNumber<int>();
    JValue response = pbnjson::Object();
    if (static_cast<unsigned char>(value) > 100) {
        respondError(m, volumeError, 202);
        return true;
    }
    if (set(name, static_cast<int>(value))) {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
    } else {
        setSoundOutputVolume(name, value);
        response.put("returnValue", true);
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
    std::string name;
    int code = 0;
    LS::Message m(message);
    JValue payload;
    std::string schema = R"({"type":"object","properties":{"soundOutput":{"type":"string"}},"required":["soundOutput"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    name = text(payload["soundOutput"]);
    JValue response = pbnjson::Object();
    unsigned char value = getSoundOutputVolume(name);
    if (value == 100) {
        respondError(m, limitError, 202);
        return true;
    }
    if (set(name, value + 1)) {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
    } else {
        setSoundOutputVolume(name, value + 1);
        response.put("returnValue", true);
        response.put("soundOutput", name);
        response.put("volume", value + 1);
    }
    respond(m, response);
    return true;
}

/* @0x1cac8 = _ZN13VolumeManager4downER9LSMessage */
/* unlike up, down of an untracked output forwards 99 successfully. */
bool VolumeManager::down(LSMessage& message)
{
    std::string name;
    int code = 0;
    LS::Message m(message);
    JValue payload;
    std::string schema = R"({"type":"object","properties":{"soundOutput":{"type":"string"}},"required":["soundOutput"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    name = text(payload["soundOutput"]);
    JValue response = pbnjson::Object();
    unsigned char value = getSoundOutputVolume(name);
    if (!value) {
        respondError(m, limitError, 202);
        return true;
    }
    if (set(name, value - 1)) {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
    } else {
        setSoundOutputVolume(name, value - 1);
        response.put("returnValue", true);
        response.put("soundOutput", name);
        response.put("volume", value - 1);
    }
    respond(m, response);
    return true;
}

/* @0x1cf88 = _ZN13VolumeManager12muteSoundOutER9LSMessage */
/* forwards even when cached mute already equals request. */
bool VolumeManager::muteSoundOut(LSMessage& message)
{
    std::string name;
    bool value = false;
    int code = 0;
    LS::Message m(message);
    JValue payload;
    std::string schema = R"({"type":"object","properties":{"soundOutput":{"type":"string"},"mute":{"type":"boolean"}},"required":["soundOutput","mute"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    name = text(payload["soundOutput"]);
    value = boolean(payload["mute"]);
    JValue response = pbnjson::Object();
    if (muteSoundOut(name, static_cast<int>(value))) {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
    } else {
        setSoundOutputMute(name, value);
        response.put("returnValue", true);
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
    int code = 0;
    std::string schema = R"({"type":"object","properties":{},"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    JValue response = getStatus();
    respond(m, response);
    return true;
}
