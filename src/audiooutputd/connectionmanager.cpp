#include "audiooutput.hpp"
#include <algorithm>
using namespace Contract;

/* @0x1692c = _ZN17ConnectionManagerC1ERN2LS6HandleER13VolumeManagerP9umiClient */
/* installs eight /audio methods, data=this. Subscription state starts
 * empty; headphone polling is started by getHeadphoneStatus(subscribe:true). */
ConnectionManager::ConnectionManager(LS::Handle& h, VolumeManager& v, umiClient* c)
    :volume(v),bus(&h),client(c),timer(0),headphone(false)
{
    registerCategory(h, "/audio", connectionMethods, this);
}

/* @0x16568 = _ZN17ConnectionManagerD1Ev */
/* disconnects every remembered pair even on driver failure; removes
 * polling source, then releases connections and the subscription base. */
ConnectionManager::~ConnectionManager()
{
    for (auto& connection : connections) disconnect(connection.source, connection.sink);
    connections.clear();
    if (timer) { g_source_remove(timer); timer = 0; }
}

/* @0x15604 = _ZN17ConnectionManager7connectERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_RN7pbnjson6JValueE */
/* source/sink by-value copies and pipelineAttribute forwarded to UMI. */
int ConnectionManager::connect(const std::string& source, const std::string& sink, JValue& attributes)
{
    return client ? client->connectInput(source, sink, attributes) : 2;
}

/* @0x15838 = _ZN17ConnectionManager11setSoundOutERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
/* no route validation or mixer mutation in this forwarding wrapper. */
int ConnectionManager::setSoundOut(const std::string& name)
{
    return client ? client->setSoundOutput(name) : 2;
}

/* @0x15958 = _ZN17ConnectionManager14setInputVolumeERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_RKi */
/* forwards unsigned-byte volume; the Luna handler performs validation. */
int ConnectionManager::setInputVolume(const std::string& source, const std::string& sink, const int& value)
{
    return client ? client->setInputVolume(source, sink, static_cast<unsigned char>(value)) : 2;
}

/* @0x15b38 = _ZN17ConnectionManager18getHeadphoneStatusEPbRNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_ */
/* UMI fills headphone flag and both microphone connection strings. */
int ConnectionManager::getHeadphoneStatus(bool* present, std::string& internal, std::string& external)
{
    return client ? client->getHeadphoneStatus(present, internal, external) : 2;
}

/* @0x15bd4 = _ZN17ConnectionManager18removePipelineInfoERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
/* forwards pipelineId only; this does not remove the local pair cache. */
int ConnectionManager::removePipelineInfo(const std::string& id)
{
    return client ? client->removePipelineInfo(id) : 2;
}

/* @0x15cf4 = _ZN17ConnectionManager16buildAudioStatusERK15AudioConnection */
/* outputMode is the literal string "null" when the cached string is empty. */
JValue ConnectionManager::buildAudioStatus(const AudioConnection& connection)
{
    JValue response = pbnjson::Object();
    response.put("sink", connection.sink);
    response.put("source", connection.source);
    response.put("outputMode", connection.outputMode.empty() ? std::string("null") : connection.outputMode);
    response.put("muted", connection.muted);
    return response;
}

/* @0x15e88 = _ZN17ConnectionManager11buildStatusEv */
/* builds audio array from local vector with stride 76 on ARM. */
JValue ConnectionManager::buildStatus()
{
    JValue entries = array();
    for (const auto& connection : connections) entries.append(buildAudioStatus(connection));
    JValue response = result(0);
    response.put("audio", entries);
    return response;
}

/* @0x15fd4 = _ZN17ConnectionManager9getStatusEv */
/* cached connections, not an observed hardware topology query. */
JValue ConnectionManager::getStatus()
{
    return buildStatus();
}

/* @0x1608c = _ZN17ConnectionManager19findAudioConnectionERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_ */
/* first exact source/sink pair; an empty string is not a wildcard. */
AudioConnection* ConnectionManager::findAudioConnection(const std::string& source, const std::string& sink)
{
    for (auto& connection : connections)
        if (connection.source == source && connection.sink == sink) return &connection;
    return nullptr;
}

/* @0x160e0 = _ZN17ConnectionManager21removeAudioConnectionERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_ */
/* removes only first matching pair and shifts subsequent entries. */
void ConnectionManager::removeAudioConnection(const std::string& source, const std::string& sink)
{
    auto it = std::find_if(connections.begin(), connections.end(), [&](const AudioConnection& item) {
        return item.source == source && item.sink == sink;
    });
    if (it != connections.end()) connections.erase(it);
}

/* @0x16394 = _ZN17ConnectionManager10disconnectERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_ */
/* null UMI returns 2; wrapper does not change service state. */
int ConnectionManager::disconnect(const std::string& source, const std::string& sink)
{
    return client ? client->disconnectInput(source, sink) : 2;
}

/* @0x16560 = _ZN17ConnectionManager17doDisconnectAudioER15AudioConnection */
/* cleanup callback deliberately discards driver status. */
void ConnectionManager::doDisconnectAudio(AudioConnection& connection)
{
    disconnect(connection.source, connection.sink);
}

/* @0x16714 = _ZN17ConnectionManager4muteERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_RKb */
/* forwards source/sink/mute with no cached-state change. */
int ConnectionManager::mute(const std::string& source, const std::string& sink, const bool& value)
{
    return client ? client->setMute(source, sink, value) : 2;
}

/* @0x168f4 = _ZN17ConnectionManager11doMuteAudioER15AudioConnectionb */
/* unchanged mute succeeds without UMI; failure returns false and
 * preserves cache. Raw 68f4..692c confirms boolean return, not void. */
bool ConnectionManager::doMuteAudio(AudioConnection& connection, bool value)
{
    if (connection.muted != value) {
        if (mute(connection.source, connection.sink, value)) return false;
        connection.muted = value;
    }
    return true;
}

/* @0x16b08 = _ZN17ConnectionManager7connectER9LSMessage */
/* duplicate connect returns code 205; successful connect inserts an
 * unmuted entry with empty outputMode, even if a route was previously selected. */
bool ConnectionManager::connect(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, connectionSchema, payload)) return true;
    std::string source = text(payload["source"]), sink = text(payload["sink"]);
    JValue response;
    if (findAudioConnection(source, sink)) {
        response = pbnjson::Object();
        response.put("returnValue", false);
        response.put("errorText", "Connection not possible");
        response.put("errorCode", 205);
        response.put("source", source);
        response.put("sink", sink);
    } else {
        JValue attributes = payload["pipelineAttribute"];
        response = result(connect(source, sink, attributes));
        if (boolean(response["returnValue"])) {
            connections.push_back(AudioConnection{source, sink, "", false});
            response.put("source", source);
            response.put("sink", sink);
        }
    }
    respond(m, response);
    return true;
}

/* @0x17520 = _ZN17ConnectionManager10disconnectER9LSMessage */
/* a remembered connection is erased EVEN when UMI disconnect fails. */
bool ConnectionManager::disconnect(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, pairSchema, payload)) return true;
    std::string source = text(payload["source"]), sink = text(payload["sink"]);
    auto* connection = findAudioConnection(source, sink);
    if (!connection) { respondError(m, "Audio not connected", 200); return true; }
    int status = disconnect(connection->source, connection->sink);
    removeAudioConnection(source, sink);
    JValue response = result(status);
    if (!status) { response.put("source", source); response.put("sink", sink); }
    respond(m, response);
    return true;
}

/* @0x17b40 = _ZN17ConnectionManager11setSoundOutER9LSMessage */
/* soundOut (not soundOutput) routes every cached connection on success
 * and resets VolumeManager's cache. It does not publish a status notification. */
bool ConnectionManager::setSoundOut(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, soundOutSchema, payload)) return true;
    std::string name = text(payload["soundOut"]);
    int status = setSoundOut(name);
    JValue response = result(status);
    if (!status) {
        for (auto& connection : connections) connection.outputMode = name;
        volume.updateSoundOutput(name);
        response.put("soundOut", name);
    }
    respond(m, response);
    return true;
}

/* @0x17fe8 = _ZN17ConnectionManager4muteER9LSMessage */
/* missing pair errors; already matching mute is a successful no-op. */
bool ConnectionManager::mute(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, inputMuteSchema, payload)) return true;
    std::string source = text(payload["source"]), sink = text(payload["sink"]);
    bool value = boolean(payload["mute"]);
    auto* connection = findAudioConnection(source, sink);
    if (!connection) { respondError(m, "Audio not connected", 200); return true; }
    int status = connection->muted == value ? 0 : mute(connection->source, connection->sink, value);
    JValue response = result(status);
    if (!status) {
        connection->muted = value;
        response.put("sink", sink);
        response.put("source", source);
        response.put("mute", value);
    }
    respond(m, response);
    return true;
}

/* @0x18654 = _ZN17ConnectionManager14setInputVolumeER9LSMessage */
/* low byte is range-checked <=100; no local connection lookup occurs. */
bool ConnectionManager::setInputVolume(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, inputVolumeSchema, payload)) return true;
    std::string source = text(payload["source"]), sink = text(payload["sink"]);
    unsigned char value = payload["volume"].asNumber<int>();
    if (value > 100) { respondError(m, "Volume out of range", 202); return true; }
    int status = setInputVolume(source, sink, static_cast<int>(value));
    JValue response = result(status);
    if (!status) {
        response.put("source", source);
        response.put("sink", sink);
        response.put("volume", static_cast<int>(value));
    }
    respond(m, response);
    return true;
}

/* @0x18ce0 = _ZN17ConnectionManager9getStatusER9LSMessage */
/* only empty-object request accepted; no audio status subscription. */
bool ConnectionManager::getStatus(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (request(m, emptySchema, payload)) respond(m, getStatus());
    return true;
}

JValue ConnectionManager::headphoneResponse()
{
    JValue response = result(0), internal = pbnjson::Object(), external = pbnjson::Object();
    response.put("headPhoneStatus", headphone);
    internal.put("connectStatus", internalStatus);
    external.put("connectStatus", externalStatus);
    response.put("internalMicrophone", internal);
    response.put("externalMicrophone", external);
    return response;
}

/* @0x18e78 = _ZN17ConnectionManager18getHeadphoneStatusER9LSMessage */
/* ONLY getHeadphoneStatus subscribes. Raw 9422 loads 500 into r0 for
 * g_timeout_add: actual interval is 500 ms, despite the misleading "5msec" log.
 * Subscription is installed before UMI query; a failed query returns true
 * WITHOUT an error response. No subscribed:true field in the initial response.
 * Every successful initial query calls headphoneDetectionMute with !headphone. */
bool ConnectionManager::getHeadphoneStatus(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, headphoneSchema, payload)) return true;
    if (boolean(payload["subscribe"])) {
        handle = bus;
        if (handle) {
            LS::Error error;
            LSCallCancelNotificationAdd(handle->handle, subscriberCancelCB, this, &error.error);
        }
        subscribe(m);
        if (!timer) timer = g_timeout_add(500, _headphoneStatusUpdateCallBack, this);
    }
    bool present = false;
    std::string internal, external;
    if (getHeadphoneStatus(&present, internal, external)) return true;
    headphone = present;
    internalStatus = internal;
    externalStatus = external;
    JValue settings = pbnjson::Object();
    settings.put("mute", !headphone);
    client->setSettings("headphoneDetectionMute", settings);
    respond(m, headphoneResponse());
    return true;
}

/* @0x150ec = _ZN17ConnectionManager30_headphoneStatusUpdateCallBackEPv */
/* repeating poll compares all three fields. Change -> cache ->
 * headphoneDetectionMute -> post; unchanged or failed UMI read posts nothing.
 * Notification carries headphone/microphone status, NOT eARC capability.
 * Cancellation does not stop this timer here; destructor removes it. */
int ConnectionManager::_headphoneStatusUpdateCallBack(void* data)
{
    auto* self = static_cast<ConnectionManager*>(data);
    bool present = false;
    std::string internal, external;
    if (!self || self->client->getHeadphoneStatus(&present, internal, external)) return 1;
    if (self->headphone == present && self->internalStatus == internal && self->externalStatus == external) return 1;
    self->headphone = present;
    self->internalStatus = internal;
    self->externalStatus = external;
    JValue settings = pbnjson::Object();
    settings.put("mute", !present);
    self->client->setSettings("headphoneDetectionMute", settings);
    JValue notification = self->headphoneResponse();
    std::string wire;
    pbnjson::JGenerator generator(nullptr);
    generator.toString(notification, pbnjson::JSchema::AllSchema(), wire);
    self->post(wire.c_str());
    return 1;
}

/* @0x195d0 = _ZN17ConnectionManager18removePipelineInfoER9LSMessage */
/* pipeline removal echoes pipelineId on success; no cache mutation. */
bool ConnectionManager::removePipelineInfo(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    if (!request(m, pipelineSchema, payload)) return true;
    std::string id = text(payload["pipelineId"]);
    int status = removePipelineInfo(id);
    JValue response = result(status);
    if (!status) response.put("pipelineId", id);
    respond(m, response);
    return true;
}
