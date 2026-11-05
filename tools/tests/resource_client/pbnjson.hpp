// Host semantic test double, NOT a declaration of pbnjson's target ABI.
// Only the JSON operations exercised by the reconstructed subset are modeled.
#pragma once
#include <memory>
#include <string>
#include <utility>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
namespace pbnjson {
inline bool& failSerialization() { static bool fail = false; return fail; }
struct JSchema { static JSchema AllSchema() { return {}; } };
class JValue {
    std::shared_ptr<rapidjson::Document> owner;
    rapidjson::Value* value;
    JValue(std::shared_ptr<rapidjson::Document> doc, rapidjson::Value* node)
        : owner(std::move(doc)), value(node) {}
public:
    JValue() : owner(std::make_shared<rapidjson::Document>()), value(owner.get()) {}
    explicit JValue(const std::string& text) : JValue() {
        value->SetString(text.data(), static_cast<rapidjson::SizeType>(text.size()), owner->GetAllocator());
    }
    explicit JValue(bool b) : JValue() { value->SetBool(b); }
    static JValue object() { JValue result; result.value->SetObject(); return result; }
    static JValue parsed(const std::string& text, bool& valid) {
        JValue result;
        result.owner->Parse(text.data(), text.size());
        valid = !result.owner->HasParseError();
        return result;
    }
    bool hasKey(const std::string& name) const {
        return value->IsObject() && value->HasMember(name.c_str());
    }
    JValue operator[](const std::string& name) const {
        if (!hasKey(name)) return {};
        return JValue(owner, &(*value)[name.c_str()]);
    }
    bool isString() const { return value->IsString(); }
    std::string asString() const {
        return isString() ? std::string(value->GetString(), value->GetStringLength()) : "";
    }
    bool asBool() const { return value->IsBool() && value->GetBool(); }
    void put(const std::string& name, const JValue& item) {
        rapidjson::Value key, copy;
        key.SetString(name.data(), static_cast<rapidjson::SizeType>(name.size()), owner->GetAllocator());
        copy.CopyFrom(*item.value, owner->GetAllocator());
        value->AddMember(key, copy, owner->GetAllocator());
    }
    void put(const std::string& name, const std::string& text) { put(name, JValue(text)); }
    void put(const std::string& name, bool b) { put(name, JValue(b)); }
    std::string dump() const {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        value->Accept(writer);
        return {buffer.GetString(), buffer.GetSize()};
    }
    struct Iterator {
        const JValue* object;
        std::pair<JValue, JValue> operator*() const;
    };
    Iterator begin() const { return {this}; }
};
inline std::pair<JValue, JValue> JValue::Iterator::operator*() const {
    if (!object->value->IsObject() || object->value->ObjectEmpty()) return {};
    auto member = object->value->MemberBegin();
    return {JValue(std::string(member->name.GetString(), member->name.GetStringLength())),
            JValue(object->owner, &member->value)};
}
inline JValue Object() { return JValue::object(); }
struct JGenerator {
    explicit JGenerator(void*) {}
    bool toString(const JValue& value, JSchema, std::string& out) {
        if (failSerialization()) return false;
        out = value.dump();
        return true;
    }
};
struct JDomParser {
    JValue value;
    bool parse(const std::string& text, JSchema) {
        bool valid;
        value = JValue::parsed(text, valid);
        return valid;
    }
    JValue getDom() { return value; }
};
} // namespace pbnjson
