#include "audiooutput.hpp"
#include <cstring>

namespace Contract {
const char *const qualitySetSchema = R"({"type":"object","properties":{"category":{"type":"string"},"value":{"type":"object"}},"required":["category","value"],"additionalProperties":false})";
const char *const qualityGetSchema = R"({"type":"object","properties":{"category":{"type":"string"},"settings":{"type":"object"}},"additionalProperties":false})";
const char *const qualityAttributeSchema = R"({"type":"object","properties":{"categoryType":{"type":"string"},"requestType":{"type":"string"},"categoryInfo":{"type":"object"},"isGameMode":{"type":"boolean"},"pipelineInfo":{"type":"object"},"activeState":{"type":"boolean"}},"required":["categoryType","requestType"],"additionalProperties":false})";
const char *const connectionSchema = R"({"type":"object","properties":{"source":{"type":"string"},"sink":{"type":"string"},"pipelineAttribute":{"type":"object"}},"required":["source","sink"],"additionalProperties":false})";
const char *const pairSchema = R"({"type":"object","properties":{"sink":{"type":"string"},"source":{"type":"string"}},"required":["source","sink"],"additionalProperties":false})";
const char *const inputVolumeSchema = R"({"type":"object","properties":{"source":{"type":"string"},"sink":{"type":"string"},"volume":{"type":"integer"}},"required":["source","sink","volume"],"additionalProperties":false})";
const char *const inputMuteSchema = R"({"type":"object","properties":{"source":{"type":"string"},"sink":{"type":"string"},"mute":{"type":"boolean"}},"required":["source","sink","mute"],"additionalProperties":false})";
const char *const soundOutSchema = R"({"type":"object","properties":{"soundOut":{"type":"string"}},"required":["soundOut"],"additionalProperties":false})";
const char *const pipelineSchema = R"({"type":"object","properties":{"pipelineId":{"type":"string"}},"required":["pipelineId"],"additionalProperties":false})";
const char *const headphoneSchema = R"({"type":"object","properties":{"subscribe":{"type":"boolean"}},"additionalProperties":false})";
const char *const emptySchema = R"({"type":"object","properties":{},"additionalProperties":false})";
const char *const volumeSchema = R"({"type":"object","properties":{"soundOutput":{"type":"string"},"volume":{"type":"integer"}},"required":["soundOutput","volume"],"additionalProperties":false})";
const char *const volumeOutputSchema = R"({"type":"object","properties":{"soundOutput":{"type":"string"}},"required":["soundOutput"],"additionalProperties":false})";
const char *const volumeMuteSchema = R"({"type":"object","properties":{"soundOutput":{"type":"string"},"mute":{"type":"boolean"}},"required":["soundOutput","mute"],"additionalProperties":false})";

// Raw .data.rel.ro at Ghidra 30b78/30be4/30c14; each entry is name, Thumb thunk, flags=0.
template<class T, bool (T::*Method)(LSMessage&)>
static bool dispatch(LSHandle*, LSMessage* message, void* context)
{
    return (static_cast<T*>(context)->*Method)(*message);
}
const LSMethod qualityMethods[] = {
    {"setSettings", dispatch<QualityManager, &QualityManager::setSettings>, 0},
    {"setQualityAttribute", dispatch<QualityManager, &QualityManager::setQualityAttribute>, 0},
    {"getSettings", dispatch<QualityManager, &QualityManager::getSettings>, 0},
    {nullptr, nullptr, 0}
};
const LSMethod connectionMethods[] = {
    {"connect", dispatch<ConnectionManager, &ConnectionManager::connect>, 0},
    {"disconnect", dispatch<ConnectionManager, &ConnectionManager::disconnect>, 0},
    {"getStatus", dispatch<ConnectionManager, &ConnectionManager::getStatus>, 0},
    {"getHeadphoneStatus", dispatch<ConnectionManager, &ConnectionManager::getHeadphoneStatus>, 0},
    {"mute", dispatch<ConnectionManager, &ConnectionManager::mute>, 0},
    {"removePipelineInfo", dispatch<ConnectionManager, &ConnectionManager::removePipelineInfo>, 0},
    {"setInputVolume", dispatch<ConnectionManager, &ConnectionManager::setInputVolume>, 0},
    {"setSoundOut", dispatch<ConnectionManager, &ConnectionManager::setSoundOut>, 0},
    {nullptr, nullptr, 0}
};
const LSMethod volumeMethods[] = {
    {"up", dispatch<VolumeManager, &VolumeManager::up>, 0},
    {"down", dispatch<VolumeManager, &VolumeManager::down>, 0},
    {"set", dispatch<VolumeManager, &VolumeManager::set>, 0},
    {"getStatus", dispatch<VolumeManager, &VolumeManager::getStatus>, 0},
    {"muteSoundOut", dispatch<VolumeManager, &VolumeManager::muteSoundOut>, 0},
    {nullptr, nullptr, 0}
};
JValue array() { return pbnjson::JArray(); }
JValue result(int status)
{
    JValue out = pbnjson::Object();
    out.put("returnValue", status == 0);
    if (status) {
        out.put("errorText", "Driver error while executing the command");
        out.put("errorCode", 20);
    }
    return out;
}

/* @0x1a3d8 = audiooutput_parse */
/* Schema failure retries AllSchema to recover DOM/errorCode, but still
 * returns false; malformed JSON never gets forwarded to UMI. */
bool parse(const std::string& input, JValue& value, const std::string& schema, int& code)
{
    pbnjson::JSchema selected(pbnjson::JSchema::AllSchema());
    if (!schema.empty()) {
        pbnjson::JSchemaFragment fragment(schema);
        selected.swap(fragment);
    }
    pbnjson::JDomParser parser;
    bool valid = parser.parse(pbnjson::JInput(input), selected);
    if (valid) value = parser.getDom();
    else if (std::strstr(parser.getError(), "Schema error")) {
        if (parser.parse(pbnjson::JInput(input), pbnjson::JSchema::AllSchema())) {
            code = 3;
            value = parser.getDom();
        }
    }
    return valid;
}
bool request(LS::Message& message, const char* schema, JValue& payload)
{
    int code = 0;
    if (parse(LSMessageGetPayload(message.message), payload, schema, code)) return true;
    respondError(message, "Failed to validate against schema", 3);
    return false;
}

/* @0x19e90 = audiooutput_respond */
/* Serialization uses AllSchema; LS respond failure throws LS::Error. */
void respond(LS::Message& message, const JValue& value)
{
    std::string wire;
    {
        pbnjson::JGenerator generator(nullptr);
        generator.toString(value, pbnjson::JSchema::AllSchema(), wire);
    }
    LS::Error error;
    if (!LSMessageRespond(message.message, wire.c_str(), &error.error)) throw error;
}

/* @0x19c4c = audiooutput_error */
/* subscribed:false is emitted only when caller supplies that option. */
void respondError(LS::Message& message, const std::string& text, int code, bool subscription)
{
    JValue response = pbnjson::Object();
    if (subscription) response.put("subscribed", false);
    response.put("returnValue", false);
    response.put("errorText", text);
    response.put("errorCode", code);
    std::string wire;
    {
        pbnjson::JGenerator generator(nullptr);
        generator.toString(response, pbnjson::JSchema::AllSchema(), wire);
    }
    LS::Error error;
    if (!LSMessageRespond(message.message, wire.c_str(), &error.error)) throw error;
}
}

static void* logContext;
/* @0x1a530 = _Z15setPmLogContextPKc */
/* PmLogGetContext supplies the status and stores the shared context. */
int setPmLogContext(const char* name)
{
    return PmLogGetContext(name, &logContext);
}
/* @0x1a53c = _Z15getPmLogContextv */
/* returns the singleton logging context, no initialization side effect. */
void* getPmLogContext()
{
    return logContext;
}

LS::SubscriptionPoint::~SubscriptionPoint()
{
    if (handle) {
        LS::Error error;
        LSCallCancelNotificationRemove(handle->handle, subscriberCancelCB, this, &error.error);
    }
    for (auto* item : items) {
        if (!item) continue;
        if (item->active) cleanItem(item);
        if (item->message) LSMessageUnref(item->message);
        delete item;
    }
}

#ifndef AUDIOOUTPUT_HOST_TEST
static_assert(sizeof(JValue) == 12, "TV pbnjson JValue layout");
static_assert(sizeof(AudioConnection) == 76, "TV connection vector stride");
static_assert(sizeof(LS::SubscriptionPoint) == 44, "TV subscription base");
static_assert(sizeof(VolumeManager) == 36, "TV volume manager");
static_assert(sizeof(QualityManager) == 8, "TV quality manager");
static_assert(sizeof(ConnectionManager) == 124, "TV connection manager");
#endif
