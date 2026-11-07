#include "audiooutput.hpp"
using namespace Contract;

static bool infoEnabled()
{
    return !getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5;
}

/* @0x1a67c = _ZN14QualityManagerC1ERN2LS6HandleEP9umiClient */
/* raw a67c..a74e registers exactly three /quality methods, flags=0.
 * This table has no getCapability, setMixer, external_arc or eARC-specific method.
 * LS2 permissions are external policy: registration alone does not authorize callers.
 * [GAP] TV role/permission files were not supplied; OSE's policy is not TV evidence. */
QualityManager::QualityManager(LS::Handle& h, umiClient* c):handle(&h),client(c)
{
    try {
        registerCategory(*handle, "/quality", qualityMethods, this);
    } catch (LS::Error& error) {
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 2)
            _PmLogMsgKV(getPmLogContext(), 3, 0, "LS2_SUBSCRIBE_FAILED", 0, 0, 0,
                        "%s - QualityManager API's registration Failed.", error.what());
    }
}

/* @0x1a7f8 = _ZN14QualityManager15isValidCategoryERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE */
/* raw a7f8 is an unconditional true, not a category whitelist. */
bool QualityManager::isValidCategory(std::string&)
{
    return true;
}

/* @0x1a7fc = _ZN14QualityManager11setSettingsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE */
/* pass category/value unchanged to UMI; null client returns 2.
 * setMixer is a category of quality/setSettings, not a Luna method:
 * {"category":"setMixer","value":{...}}. Value's internal policy belongs to UMI.
 * This service has no file/db/settings-service write or mixer-settings cache here;
 * it delegates directly. Persistence below UMI is NOT established by this body. */
int QualityManager::setSettings(const std::string& category, JValue value)
{
    JValue copy(value);
    if (infoEnabled()) {
        void* context = getPmLogContext();
        _PmLogMsgKV(context, 6, 0, "AUDIO_SET_SETTINGS_CALL", 0, 0, 0,
                    "setSettings request for category:%s with value:%s", category.c_str(), copy.stringify().c_str());
    }
    int status = client ? client->setSettings(category, copy) : 2;
    if (infoEnabled())
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_SET_SETTINGS_CALL", 0, 0, 0,
                    "setSettings umi returnStatus %d", status);
    return status;
}

/* @0x1a980 = _ZN14QualityManager11getSettingsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE */
/* raw a9de forwards category and settings object to UMI, not a TV getter.
 * Request envelope: /quality/getSettings {"category":"external_arc","settings":{}}.
 * The service does not recognize external_arc or earcCapability; this is generic
 * forwarding, not proof of a capability selector or a hardware capability query.
 * UMI getSettings @0x45bfc/getSettingsObject @0x45728 (separate worker) returns
 * an ARRAY of whole cached {category,value} entries for a generic category,
 * ignoring settings; miss/empty cache returns boolean true (raw 35a08/36676),
 * mapped below to settings STRING "null"; empty category returns all entries
 * when the cache is nonempty, not a live-state snapshot.
 * umiClient raw 1adf2 also returns true when its audio object is missing;
 * settings STRING "null" alone therefore does not establish a cache miss.
 * external_arc/setMixer/inputInfo take this generic cache path (raw 364cc).
 * It does not read the current input map (+0x214 -> +0x18) used by internal
 * codec/channel getters @0x4910c/0x49f24, or the mixer byte +0x7c.
 * Thus external_arc reads the last successful event, NOT a live capability
 * getter. Partial updates can replace cache value while internal capability
 * state persists. No earcCapability-specific getter is registered here.
 * Raw aa50 compares the WHOLE returned JSON to boolean true, not returnValue.
 * true -> settings STRING "null"; every other JSON (including false/null/error)
 * is wrapped as successful settings. No subscribe field is accepted, no
 * SubscriptionPoint is owned by QualityManager, no capability notification here. */
JValue QualityManager::getSettings(const std::string& category, JValue settings)
{
    JValue copy(settings);
    if (infoEnabled())
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_GET_SETTINGS_CALL", 0, 0, 0,
                    "Audio getSettings request for category:%s, settings:%s", category.c_str(), copy.stringify().c_str());
    JValue response = pbnjson::Object();
    JValue values = pbnjson::Object();
    if (!client) {
        response.put("returnValue", false);
        response.put("errorText", "Driver error while executing the command");
        response.put("errorCode", 20);
        return response;
    }
    values = client->getSettings(category, settings);
    if (infoEnabled())
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_GET_SETTINGS_CALL", 0, 0, 0,
                    "Audio getSettings response:%s", values.stringify().c_str());
    if (!(values == true)) {
        response.put("returnValue", true);
        response.put("settings", values);
    } else {
        response.put("returnValue", true);
        response.put("settings", "null");
    }
    return response;
}

/* @0x1ad10 = _ZN14QualityManager19setQualityAttributeEN7pbnjson6JValueE */
/* entire attribute object is forwarded, null client returns 2. */
int QualityManager::setQualityAttribute(JValue attributes)
{
    JValue copy(attributes);
    if (infoEnabled())
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_SET_QUALITY_ATTRIBUTE_CALL", 0, 0, 0,
                    "setQualityAttribute request for:%s", copy.stringify().c_str());
    int status = client ? client->setQualityAttribute(copy) : 2;
    if (infoEnabled())
        _PmLogMsgKV(getPmLogContext(), 6, 0, "AUDIO_SET_QUALITY_ATTRIBUTE_CALL", 0, 0, 0,
                    "setQualityAttribute umi returnStatus %d", status);
    return status;
}

/* @0x1ae00 = _ZN14QualityManager11setSettingsER9LSMessage */
/* required category string and value object; no additional properties.
 * Success echoes category only (not value); every UMI nonzero maps to code 20. */
bool QualityManager::setSettings(LSMessage& message)
{
    LS::Message requestMessage(message);
    JValue payload;
    int code = 0;
    std::string category;
    JValue value;
    const char* file = "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/qualitymanager.cpp";
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
        _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() Quality setSettings  call", file, "setSettings");
    std::string schema = R"({"type":"object","properties":{"category":{"type":"string"},"value":{"type":"object"}},"required":["category","value"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(requestMessage, schemaError, 3);
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
            _PmLogMsgKV(getPmLogContext(), 6, 0, "JSON_PARSE_ERROR", 0, 0, 0, "error for schema");
        return true;
    }
    category = text(payload["category"]);
    value = payload["value"];
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6) {
        void* context = getPmLogContext();
        _PmLogMsgKV(context, 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() Quality setSettings Request for category %s, value %s",
                    file, "setSettings", category.c_str(), value.stringify().c_str());
    }
    JValue response = pbnjson::Object();
    if (!setSettings(category, value)) {
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
            _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                        "%s:%s() umi setSettings for category %s is success",
                        file, "setSettings", category.c_str());
        response.put("returnValue", true);
        response.put("category", category);
    } else {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
    }
    respond(requestMessage, response);
    return true;
}

/* @0x1b398 = _ZN14QualityManager11getSettingsER9LSMessage */
/* category/settings are OPTIONAL; schema rejects subscribe and arrays.
 * This is a one-shot response. Missing category becomes an empty string. */
bool QualityManager::getSettings(LSMessage& message)
{
    LS::Message requestMessage(message);
    JValue payload;
    std::string category;
    JValue settings;
    int code = 0;
    std::string schema = R"({"type":"object","properties":{"category":{"type":"string"},"settings":{"type":"object"}},"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(requestMessage, schemaError, 3);
        return true;
    }
    category = text(payload["category"]);
    settings = payload["settings"];
    JValue response = pbnjson::Object();
    response = getSettings(category, settings);
    respond(requestMessage, response);
    return true;
}

/* @0x1b680 = _ZN14QualityManager19setQualityAttributeER9LSMessage */
/* required categoryType/requestType strings, four typed optional fields;
 * no authorization checks or category validation beyond the JSON schema here. */
bool QualityManager::setQualityAttribute(LSMessage& message)
{
    LS::Message requestMessage(message);
    JValue payload;
    int code = 0;
    if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 6)
        _PmLogMsgKV(getPmLogContext(), 7, 0, nullptr, 0, 0, 0,
                    "%s:%s() QualityManager::setQualityAttribute call",
                    "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/qualitymanager.cpp",
                    "setQualityAttribute");
    std::string schema = R"({"type":"object","properties":{"categoryType":{"type":"string"},"requestType":{"type":"string"},"categoryInfo":{"type":"object"},"isGameMode":{"type":"boolean"},"pipelineInfo":{"type":"object"},"activeState":{"type":"boolean"}},"required":["categoryType","requestType"],"additionalProperties":false})";
    if (!parse(LSMessageGetPayload(&message), payload, schema, code)) {
        respondError(requestMessage, schemaError, 3);
        if (!getPmLogContext() || *static_cast<int*>(getPmLogContext()) > 5)
            _PmLogMsgKV(getPmLogContext(), 6, 0, "JSON_PARSE_ERROR", 0, 0, 0, "error for schema");
        return true;
    }
    JValue response = pbnjson::Object();
    if (!setQualityAttribute(payload)) {
        response.put("returnValue", true);
    } else {
        response.put("returnValue", false);
        response.put("errorText", driverError);
        response.put("errorCode", 20);
    }
    respond(requestMessage, response);
    return true;
}
