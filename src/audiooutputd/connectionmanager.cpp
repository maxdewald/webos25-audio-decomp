#include "audiooutput.hpp"
#include <algorithm>
using namespace Contract;

#ifdef __GLIBCXX__
/* @0x1a048 = _ZNSt6vectorI15AudioConnectionSaIS0_EE17_M_realloc_insertIJS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_ */
template void std::vector<AudioConnection>::_M_realloc_insert<AudioConnection>(
    std::vector<AudioConnection>::iterator, AudioConnection&&);
#endif

/* @0x1692c = _ZN17ConnectionManagerC1ERN2LS6HandleER13VolumeManagerP9umiClient */
/* installs eight /audio methods, data=this. Subscription state starts
 * empty; headphone polling is started by getHeadphoneStatus(subscribe:true). */
ConnectionManager::ConnectionManager(LS::Handle& h, VolumeManager& v, umiClient* c)
    :volume(v),bus(&h),client(c)
{
    try {
        registerCategory(h, "/audio", connectionMethods, this);
    } catch (LS::Error& error) {
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 2)
            _PmLogMsgKV(getPmLogContext(), 3, 0, "LS2_SUBSCRIBE_FAILED", 0, 0, 0,
                        "%s - ConnectionManager API's registration Failed.", error.what());
    }
    timer = 0;
    headphone = false;
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
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5) {
        void* context = getPmLogContext();
        _PmLogMsgKV(context, 6, 0, "AUDIO_CONNECT_CALL", 0, 0, 0,
                    "Audio connect request for source:%s, sink:%s, pipelineAttribute:%s",
                    source.c_str(), sink.c_str(), attributes.stringify().c_str());
    }
    int status = client ? client->connectInput(source, sink, attributes) : 2;
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5) {
        void* context = getPmLogContext();
        _PmLogMsgKV(context, 6, 0, "AUDIO_CONNECT_CALL", 0, 0, 0,
                    "Audio connect umi returnStatus %d", status);
    }
    return status;
}

/* @0x15838 = _ZN17ConnectionManager11setSoundOutERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
/* no route validation or mixer mutation in this forwarding wrapper. */
int ConnectionManager::setSoundOut(const std::string& name)
{
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_SET_SOUNDOUTPUT_CALL", 0, 0, 0,
                    "Audio setSoundOut request for soundOutput:%s", name.c_str());
    int status = client ? client->setSoundOutput(name) : 2;
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_SET_SOUNDOUTPUT_CALL", 0, 0, 0,
                    "Audio setSoundOutput umi returnStatus %d", status);
    return status;
}

/* @0x15958 = _ZN17ConnectionManager14setInputVolumeERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_RKi */
/* forwards unsigned-byte volume; the Luna handler performs validation. */
int ConnectionManager::setInputVolume(const std::string& source, const std::string& sink, const int& value)
{
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_SET_INPUT_VOLUME", 0, 0, 0,
                    "Audio setinputvolume request for source:%s, sink:%s, volume:%d",
                    source.c_str(), sink.c_str(), value);
    int status = client ? client->setInputVolume(source, sink, static_cast<unsigned char>(value)) : 2;
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_SET_INPUT_VOLUME", 0, 0, 0,
                    "Audio setinputvolume umi returnStatus %d", status);
    return status;
}

/* @0x15b38 = _ZN17ConnectionManager18getHeadphoneStatusEPbRNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_ */
/* UMI fills headphone flag and both microphone connection strings. */
int ConnectionManager::getHeadphoneStatus(bool* present, std::string& internal, std::string& external)
{
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_GET_HEADPHONE_STATUS_CALL", 0, 0, 0,
                    "Audio getHeadphoneStatus request");
    int status = client ? client->getHeadphoneStatus(present, internal, external) : 2;
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_GET_HEADPHONE_STATUS_CALL", 0, 0, 0,
                    "Audio umi getHeadphoneStatus:%d internalConnectStatus:%s externalConnectStatus:%s returnStatus %d",
                    *present, internal.c_str(), external.c_str(), status);
    return status;
}

/* @0x15bd4 = _ZN17ConnectionManager18removePipelineInfoERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
/* forwards pipelineId only; this does not remove the local pair cache. */
int ConnectionManager::removePipelineInfo(const std::string& id)
{
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_REMOVE_PIPELINEINFO_CALL", 0, 0, 0,
                    "Audio removePipelineInfo request for pipelineId:%s", id.c_str());
    int status = client ? client->removePipelineInfo(id) : 2;
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_REMOVE_PIPELINEINFO_CALL", 0, 0, 0,
                    "Audio removePipelineInfo umi returnStatus %d", status);
    return status;
}

/* @0x15cf4 = _ZN17ConnectionManager16buildAudioStatusERK15AudioConnection */
/* outputMode is the literal string "null" when the cached string is empty. */
JValue ConnectionManager::buildAudioStatus(const AudioConnection& connection)
{
    JValue response = pbnjson::Object();
    response.put("sink", connection.sink);
    response.put("source", connection.source);
    if (connection.outputMode.empty()) response.put("outputMode", "null");
    else response.put("outputMode", connection.outputMode);
    response.put("muted", connection.muted);
    return response;
}

/* @0x15e88 = _ZN17ConnectionManager11buildStatusEv */
/* builds audio array from local vector with stride 76 on ARM. */
JValue ConnectionManager::buildStatus()
{
    pbnjson::JArray entries;
    JValue response = pbnjson::Object();
    for (const auto& connection : connections) entries.append(buildAudioStatus(connection));
    response.put("returnValue", true);
    response.put("audio", entries);
    return response;
}

/* @0x15fd4 = _ZN17ConnectionManager9getStatusEv */
/* cached connections, not an observed hardware topology query. */
JValue ConnectionManager::getStatus()
{
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_GET_STATUS_CALL", 0, 0, 0,
                    "Audio getStatus request");
    JValue response = buildStatus();
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5) {
        void* context = getPmLogContext();
        _PmLogMsgKV(context, 6, 0, "AUDIO_GET_STATUS_CALL", 0, 0, 0,
                    "Audio getStatus response:%s", response.stringify().c_str());
    }
    return response;
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
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->source == source && it->sink == sink) {
            connections.erase(it);
            return;
        }
    }
}

/* @0x16394 = _ZN17ConnectionManager10disconnectERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_ */
/* null UMI returns 2; wrapper does not change service state. */
int ConnectionManager::disconnect(const std::string& source, const std::string& sink)
{
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_DISCONNECT_CALL", 0, 0, 0,
                    "Audio disconnect request for source:%s, sink:%s", source.c_str(), sink.c_str());
    int status = client ? client->disconnectInput(source, sink) : 2;
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_DISCONNECT_CALL", 0, 0, 0,
                    "Audio disconnect umi returnStatus %d", status);
    return status;
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
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_MUTE_CALL", 0, 0, 0,
                    "Audio mute request for source:%s, sink:%s, mute:%d",
                    source.c_str(), sink.c_str(), value);
    int status = client ? client->setMute(source, sink, value) : 2;
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_MUTE_CALL", 0, 0, 0,
                    "Audio mute umi returnStatus %d", status);
    return status;
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
    int code = 0;
    std::string sink, source;
    JValue attributes;
    std::string schema = R"({"type":"object","properties":{"source":{"type":"string"},"sink":{"type":"string"},"pipelineAttribute":{"type":"object"}},"required":["source","sink"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    sink = text(payload["sink"]);
    source = text(payload["source"]);
    attributes = payload["pipelineAttribute"];
    const char* file = "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp";
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6) {
        void* context = getPmLogContext();
        _PmLogMsgKV(context, 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() Audio connect request for source %s, sink %s, pipelineAttribute %s",
                    file, "connect", source.c_str(), sink.c_str(), attributes.stringify().c_str());
    }
    auto* connection = findAudioConnection(source, sink);
    JValue response = pbnjson::Object();
    if (connection) {
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
            _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_CONNECT_CALL", 0, 0, 0,
                        "Audio is already connected (source:%s, sink:%s)", source.c_str(), sink.c_str());
        response.put("returnValue", false);
        response.put("errorText", duplicateError);
        response.put("errorCode", 205);
        response.put("source", source);
        response.put("sink", sink);
    } else {
        if (!connect(source, sink, attributes)) {
            if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
                _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                            "%s:%s() Audio connect success", file, "connect");
            auto inserted = connections.insert(connections.end(), AudioConnection());
            inserted->sink = sink;
            inserted->source = source;
            response.put("returnValue", true);
            response.put("source", source);
            response.put("sink", sink);
        } else {
            if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 2)
                _PmLogMsgKV(getPmLogContext(), 3, 0, "AUDIO_DRIVER_ERROR", 0, 0, 0,
                            "Audio is not connected");
            response.put("returnValue", false);
            response.put("errorText", driverError);
            response.put("errorCode", 20);
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
    int code = 0;
    std::string sink, source;
    std::string schema = R"({"type":"object","properties":{"sink":{"type":"string"},"source":{"type":"string"}},"required":["source","sink"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    sink = text(payload["sink"]);
    source = text(payload["source"]);
    const char* file = "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp";
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
        _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() Audio disconnect request for source %s, sink %s",
                    file, "disconnect", source.c_str(), sink.c_str());
    auto* connection = findAudioConnection(source, sink);
    if (!connection) { respondError(m, connectionError, 200); return true; }
    int status = disconnect(connection->source, connection->sink);
    removeAudioConnection(source, sink);
    JValue response = pbnjson::Object();
    if (!status) {
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
            _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                        "%s:%s() Audio disconnect with source %s and sink %s",
                        file, "disconnect", source.c_str(), sink.c_str());
        response.put("returnValue", true);
        response.put("source", source);
        response.put("sink", sink);
    } else {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
    }
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
    int code = 0;
    std::string name;
    std::string schema = R"({"type":"object","properties":{"soundOut":{"type":"string"}},"required":["soundOut"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    name = text(payload["soundOut"]);
    const char* file = "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp";
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
        _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() Audio setSoundOut request for soundOut %s", file, "setSoundOut", name.c_str());
    JValue response = pbnjson::Object();
    if (!setSoundOut(name)) {
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
            _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                        "%s:%s() Audio routing to soundOut %s  is success", file, "setSoundOut", name.c_str());
        for (auto& connection : connections) connection.outputMode = name;
        volume.updateSoundOutput(name);
        response.put("returnValue", true);
        response.put("soundOut", name);
    } else {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
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
    int code = 0;
    std::string sink, source;
    std::string schema = R"({"type":"object","properties":{"source":{"type":"string"},"sink":{"type":"string"},"mute":{"type":"boolean"}},"required":["source","sink","mute"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    sink = text(payload["sink"]);
    source = text(payload["source"]);
    bool value = boolean(payload["mute"]);
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
        _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() Audio mute called for source %s, sink %s, mute %d",
                    "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp",
                    "mute", source.c_str(), sink.c_str(), value);
    auto* connection = findAudioConnection(source, sink);
    if (!connection) { respondError(m, connectionError, 200); return true; }
    JValue response = pbnjson::Object();
    if (!doMuteAudio(*connection, value)) {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
    } else {
        response.put("returnValue", true);
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
    int code = 0;
    std::string source, sink;
    std::string schema = R"({"type":"object","properties":{"source":{"type":"string"},"sink":{"type":"string"},"volume":{"type":"integer"}},"required":["source","sink","volume"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    source = text(payload["source"]);
    sink = text(payload["sink"]);
    signed char value = payload["volume"].asNumber<int>();
    const char* file = "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp";
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
        _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() Audio setInputVolume called for source %s and sink %s with volume %d",
                    file, "setInputVolume", source.c_str(), sink.c_str(), value);
    if (static_cast<unsigned char>(value) > 100) { respondError(m, volumeError, 202); return true; }
    JValue response = pbnjson::Object();
    if (!setInputVolume(source, sink, static_cast<int>(value))) {
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
            _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                        "%s:%s() Audio setInputVolume is success for source %s and sink %s with volume %d",
                        file, "setInputVolume", source.c_str(), sink.c_str(), value);
        response.put("returnValue", true);
        response.put("source", source);
        response.put("sink", sink);
        response.put("volume", static_cast<int>(value));
    } else {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
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

/* @0x18e78 = _ZN17ConnectionManager18getHeadphoneStatusER9LSMessage */
/* ONLY getHeadphoneStatus subscribes. Raw 9422 loads 500 into r0 for
 * g_timeout_add: actual interval is 500 ms, despite the misleading "5msec" log.
 * Subscription is installed before UMI query; a failed query returns true
 * WITHOUT an error response. No subscribed:true field in the initial response.
 * Every successful initial query calls headphoneDetectionMute with !headphone. */
bool ConnectionManager::getHeadphoneStatus(LSMessage& message)
{
    int code = 0;
    bool present = false;
    std::string internal, external;
    LS::Message m(message);
    JValue payload;
    std::string schema = R"({"type":"object","properties":{"subscribe":{"type":"boolean"}},"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(m.message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    const char* file = "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp";
    if (boolean(payload["subscribe"])) {
        handle = bus;
        if (handle) {
            LS::Error error;
            LSCallCancelNotificationAdd(handle->handle, subscriberCancelCB, this, &error.error);
        }
        subscribe(m);
        if (!timer) {
            timer = g_timeout_add(500, _headphoneStatusUpdateCallBack, this);
            if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
                _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                            "%s:%s() Timer for 5msec is created to monitor headphone status- id = %d",
                            file, "getHeadphoneStatus", timer);
        }
    }
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
        _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() umi->getHeadphoneStatus", file, "getHeadphoneStatus");
    if (getHeadphoneStatus(&present, internal, external)) return true;
    headphone = present;
    internalStatus = internal;
    externalStatus = external;
    JValue settings = pbnjson::Object();
    settings.put("mute", !headphone);
    client->setSettings("headphoneDetectionMute", settings);
    JValue response = pbnjson::Object();
    JValue internalResponse = pbnjson::Object();
    JValue externalResponse = pbnjson::Object();
    response.put("headPhoneStatus", headphone);
    internalResponse.put("connectStatus", internalStatus);
    response.put("internalMicrophone", internalResponse);
    externalResponse.put("connectStatus", externalStatus);
    response.put("externalMicrophone", externalResponse);
    response.put("returnValue", true);
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
        _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() Send headphone connection status: %d mInternalConnectStatus:%s mExternalConnectStatus:%s",
                    file, "getHeadphoneStatus", headphone, internalStatus.c_str(), externalStatus.c_str());
    respond(m, response);
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
    if (!self || self->client->getHeadphoneStatus(&present, internal, external)) {
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 2)
            _PmLogMsgKV(getPmLogContext(), 3, 0, "AUDIO_DRIVER_ERROR", 0, 0, 0,
                        "getHeadphoneStatus return error");
    } else if (self->headphone != present || internal != self->internalStatus || external != self->externalStatus) {
        self->headphone = present;
        self->internalStatus = internal;
        self->externalStatus = external;
        JValue settings = pbnjson::Object();
        settings.put("mute", !present);
        self->client->setSettings("headphoneDetectionMute", settings);
        JValue notification = pbnjson::Object();
        JValue internalResponse = pbnjson::Object();
        JValue externalResponse = pbnjson::Object();
        notification.put("headPhoneStatus", self->headphone);
        internalResponse.put("connectStatus", self->internalStatus);
        notification.put("internalMicrophone", internalResponse);
        externalResponse.put("connectStatus", self->externalStatus);
        notification.put("externalMicrophone", externalResponse);
        notification.put("returnValue", true);
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
            _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                        "%s:%s() Send notification for headphone connection status: %d mInternalConnectStatus:%s mExternalConnectStatus:%s",
                        "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp",
                        "_headphoneStatusUpdateCallBack", self->headphone,
                        self->internalStatus.c_str(), self->externalStatus.c_str());
        std::string wire;
        {
            pbnjson::JGenerator generator(nullptr);
            generator.toString(notification, pbnjson::JSchema::AllSchema(), wire);
        }
        self->post(wire.c_str());
    }
    return 1;
}

/* @0x195d0 = _ZN17ConnectionManager18removePipelineInfoER9LSMessage */
/* pipeline removal echoes pipelineId on success; no cache mutation. */
bool ConnectionManager::removePipelineInfo(LSMessage& message)
{
    LS::Message m(message);
    JValue payload;
    int code = 0;
    std::string id;
    std::string schema = R"({"type":"object","properties":{"pipelineId":{"type":"string"}},"required":["pipelineId"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(m, schemaError, 3);
        return true;
    }
    id = text(payload["pipelineId"]);
    const char* file = "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp";
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
        _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() Audio removePipelineInfo request for pipelineId %s", file, "removePipelineInfo", id.c_str());
    JValue response = pbnjson::Object();
    if (!removePipelineInfo(id)) {
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
            _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                        "%s:%s() Audio removePipelineInfo removed pipelineId %s is success",
                        file, "removePipelineInfo", id.c_str());
        response.put("returnValue", true);
        response.put("pipelineId", id);
    } else {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
    }
    respond(m, response);
    return true;
}
