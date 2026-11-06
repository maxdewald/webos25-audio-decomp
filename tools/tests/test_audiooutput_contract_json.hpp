#pragma once
// Host semantic double only; target ABI declarations remain in audiooutput.hpp.
#include <memory>
#include <string>
#include <cstring>
#include <rapidjson/document.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
namespace pbnjson {
class JValue {
    std::shared_ptr<rapidjson::Document> owner;
    rapidjson::Value* node;
    JValue(std::shared_ptr<rapidjson::Document> o, rapidjson::Value* n):owner(o),node(n){}
public:
    JValue():owner(std::make_shared<rapidjson::Document>()),node(owner.get()){}
    virtual ~JValue() = default;
    JValue(bool v):JValue(){ node->SetBool(v); }
    JValue(int v):JValue(){ node->SetInt(v); }
    JValue(const char* v):JValue(std::string(v)){}
    JValue(const std::string& v):JValue(){ node->SetString(v.data(), v.size(), owner->GetAllocator()); }
    static JValue object(){ JValue v; v.node->SetObject(); return v; }
    static JValue array(){ JValue v; v.node->SetArray(); return v; }
    static JValue parsed(const std::string& s, bool& valid){ JValue v; v.owner->Parse(s.data(),s.size()); valid=!v.owner->HasParseError(); return v; }
    bool put(const std::string& key, const JValue& v){
        rapidjson::Value copy, name;
        copy.CopyFrom(*v.node, owner->GetAllocator());
        name.SetString(key.data(), key.size(), owner->GetAllocator());
        if(node->HasMember(key.c_str())) (*node)[key.c_str()]=std::move(copy);
        else node->AddMember(name,copy,owner->GetAllocator());
        return true;
    }
    bool append(const JValue& v){ rapidjson::Value copy; copy.CopyFrom(*v.node,owner->GetAllocator()); node->PushBack(copy,owner->GetAllocator()); return true; }
    JValue operator[](const char* key) const { if(!node->IsObject()||!node->HasMember(key))return {}; return JValue(owner,&(*node)[key]); }
    bool operator==(bool v) const { return node->IsBool() && node->GetBool()==v; }
    int asBool(bool& v) const { if(!node->IsBool())return 1; v=node->GetBool(); return 0; }
    int asString(std::string& v) const { if(!node->IsString())return 1; v.assign(node->GetString(),node->GetStringLength()); return 0; }
    template<class T> T asNumber() const { return node->IsInt()? static_cast<T>(node->GetInt()):0; }
    std::string stringify(const char* = nullptr) const { rapidjson::StringBuffer b; rapidjson::Writer<rapidjson::StringBuffer> w(b); node->Accept(w); return {b.GetString(),b.GetSize()}; }
    bool conforms(const std::string& schema) const { rapidjson::Document d; d.Parse(schema.c_str()); rapidjson::SchemaDocument s(d); rapidjson::SchemaValidator validator(s); return node->Accept(validator); }
};
inline JValue Object(){return JValue::object();}
struct JArray : JValue { JArray():JValue(JValue::array()){} };
struct JSchema { std::string schema; JSchema()=default; JSchema(const JSchema&)=default; static const JSchema& AllSchema(){static JSchema v;return v;} void swap(JSchema& other){schema.swap(other.schema);} };
struct JSchemaFragment : JSchema { explicit JSchemaFragment(const std::string& s){schema=s;} };
struct JInput { std::string input; explicit JInput(const std::string& s):input(s){} };
struct JDomParser {
    JValue value;
    std::string error;
    bool parse(const JInput& in,const JSchema& schema){ bool ok; value=JValue::parsed(in.input,ok); if(!ok){error="Syntax error";return false;} if(!schema.schema.empty()&&!value.conforms(schema.schema)){error="Schema error";return false;} return true; }
    JValue getDom(){return value;}
    const char* getError(){return error.c_str();}
};
struct JGenerator { explicit JGenerator(void*){} bool toString(const JValue& v,const JSchema&,std::string& out){out=v.stringify();return true;} };
}
