// @sources src/audiooutputd/connectionmanager.cpp src/audiooutputd/contract.cpp src/audiooutputd/qualitymanager.cpp src/audiooutputd/volumemanager.cpp
// @cflags -std=c++14 -DAUDIOOUTPUT_HOST_TEST -Isrc/audiooutputd -isystem lg-opensource/public/rapidjson/include
#include "audiooutput.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
struct LSHandle {};
struct LSMessage { std::string payload, response; int refs = 0; };
static std::map<std::string,const LSMethod*> registrations;
static std::map<std::string,void*> contexts;
static std::string category, first, second;
static JValue forwarded, getter = pbnjson::Object();
static int calls, driverStatus, posts, subscriptions, timerCalls, cancelled;
static bool rejectRegistration, rejectCategoryData;
static unsigned interval;
static bool hp;
static std::string internal = "disconnected", external = "disconnected";
static JValue notification;
static JValue json(const std::string& s){ bool ok; auto v=JValue::parsed(s,ok); assert(ok); return v; }
static bool truth(const JValue& v){return Contract::boolean(v);}
static std::string str(const JValue& v){return Contract::text(v);}
extern "C" {
void LSErrorInit(LSError* e){std::memset(e,0,sizeof(*e));}
void LSErrorFree(LSError*){}
bool LSRegisterCategory(LSHandle*, const char* c,const LSMethod* m,void*,void*,LSError* error){ if(rejectRegistration){error->message="registration failed";return false;} registrations[c]=m; return true; }
bool LSCategorySetData(LSHandle*,const char* c,void* data,LSError* error){if(rejectCategoryData){error->message="category data failed";return false;} contexts[c]=data;return true;}
const char* LSMessageGetPayload(LSMessage* m){return m->payload.c_str();}
void LSMessageRef(LSMessage* m){++m->refs;}
void LSMessageUnref(LSMessage* m){--m->refs;}
bool LSMessageRespond(LSMessage* m,const char* wire,LSError*){m->response=wire;return true;}
bool LSCallCancelNotificationAdd(LSHandle*,bool (*)(LSHandle*,const char*,void*),void*,LSError*){return true;}
bool LSCallCancelNotificationRemove(LSHandle*,bool (*)(LSHandle*,const char*,void*),void*,LSError*){++cancelled;return true;}
unsigned g_timeout_add(unsigned ms,int (*)(void*),void*){interval=ms;++timerCalls;return 42;}
int g_source_remove(unsigned id){assert(id==42);return 1;}
int PmLogGetContext(const char*,void** p){*p=nullptr;return 0;}
void _PmLogMsgKV(void*,int,unsigned,const char*,unsigned,unsigned,unsigned,const char*,...){}
}
bool LS::SubscriptionPoint::subscriberCancelCB(LSHandle*,const char*,void*){return true;}
void LS::SubscriptionPoint::cleanItem(SubscriptionItem*){}
bool LS::SubscriptionPoint::subscribe(Message&){++subscriptions;return true;}
bool LS::SubscriptionPoint::post(const char* wire){++posts;notification=json(wire);return true;}
int umiClient::setSettings(std::string c,JValue v){category=c;forwarded=v;++calls;return driverStatus;}
JValue umiClient::getSettings(const std::string& c,JValue v){category=c;forwarded=v;++calls;return getter;}
int umiClient::setQualityAttribute(JValue v){forwarded=v;++calls;return driverStatus;}
int umiClient::connectInput(std::string a,std::string b,JValue v){first=a;second=b;forwarded=v;++calls;return driverStatus;}
int umiClient::disconnectInput(std::string a,std::string b){first=a;second=b;++calls;return driverStatus;}
int umiClient::setSoundOutput(std::string a){first=a;++calls;return driverStatus;}
int umiClient::setMute(std::string a,std::string b,bool v){first=a;second=b;forwarded=JValue(v);++calls;return driverStatus;}
int umiClient::setInputVolume(std::string a,std::string b,unsigned char v){first=a;second=b;forwarded=JValue(static_cast<int>(v));++calls;return driverStatus;}
int umiClient::removePipelineInfo(std::string a){first=a;++calls;return driverStatus;}
int umiClient::getHeadphoneStatus(bool* b,std::string& i,std::string& e){*b=hp;i=internal;e=external;return driverStatus;}
int umiClient::setOutputVolume(std::string a,unsigned char v){first=a;forwarded=JValue(static_cast<int>(v));++calls;return driverStatus;}
int umiClient::setOutputMute(std::string a,bool v){first=a;forwarded=JValue(v);++calls;return driverStatus;}
int umiClient::getDefaultVolume(){return 37;}
static JValue call(const char* c,const char* method,const std::string& payload){
    for(auto* m=registrations.at(c);m->name;++m){
        assert(m->flags==0);
        if(std::string(m->name)==method){LSMessage request{payload,"",0};assert(m->function(nullptr,&request,contexts.at(c)));assert(request.refs==0);return request.response.empty()?JValue():json(request.response);}
    }
    assert(false);return {};
}
int main(){
    LSHandle raw; LS::Handle h{&raw}; umiClient client;
    for(int failure=0;failure<2;++failure){
        rejectRegistration=failure==0;
        rejectCategoryData=failure==1;
        VolumeManager failedVolume(h,&client);
        QualityManager failedQuality(h,&client);
        ConnectionManager failedConnection(h,failedVolume,&client);
        assert(failedVolume.getSoundOutputVolume("untracked")==100);
        assert(failedQuality.setSettings("setMixer",pbnjson::Object())==0);
        assert(!failedConnection.findAudioConnection("source","sink"));
    }
    rejectRegistration=false;
    rejectCategoryData=false;
    VolumeManager volume(h,&client);
    {
        ConnectionManager connection(h,volume,&client); QualityManager quality(h,&client);
        assert(registrations.size()==3);
        int count=0;for(auto* m=registrations.at("/quality");m->name;++m)++count;assert(count==3);
        auto r=call("/quality","setSettings",R"({"category":"setMixer","value":{"mix":false}})");
        assert(truth(r["returnValue"])&&str(r["category"])=="setMixer");
        assert(category=="setMixer"&&forwarded.stringify()==R"({"mix":false})");
        assert(!r["value"].stringify().compare("null"));
        int before=calls;
        for(auto payload:{R"({"category":"setMixer","value":{ },"subscribe":true})",R"({"category":"setMixer"})",R"({"category":"setMixer","value":[]})","[1]","{"}){
            r=call("/quality","setSettings",payload);assert(!truth(r["returnValue"])&&r["errorCode"].asNumber<int>()==3);
        }
        assert(calls==before);
        getter=json(R"([{"category":"external_arc","value":{"earcCapability":"opaque"}}])");
        r=call("/quality","getSettings",R"({"category":"external_arc","settings":{}})");
        assert(category=="external_arc"&&forwarded.stringify()=="{}");
        assert(truth(r["returnValue"])&&r["settings"].stringify()==getter.stringify());
        before=calls;
        r=call("/quality","getSettings",R"({"category":"external_arc","subscribe":true})");
        assert(!truth(r["returnValue"])&&calls==before&&subscriptions==0);
        r=call("/quality","getSettings",R"({"category":"external_arc","subscribe":false})");
        assert(!truth(r["returnValue"])&&calls==before&&subscriptions==0);
        r=call("/quality","getSettings",R"({"settings":[]})");assert(!truth(r["returnValue"]));
        getter=JValue(true);r=call("/quality","getSettings",R"({"category":"external_arc","settings":{}})");
        assert(truth(r["returnValue"])&&r["settings"].stringify()==R"("null")"&&category=="external_arc");
        r=call("/quality","getSettings","{}");assert(truth(r["returnValue"])&&r["settings"].stringify()==R"("null")"&&category.empty());
        getter=JValue(false);r=call("/quality","getSettings","{}");assert(r["settings"]==false&&truth(r["returnValue"]));
        getter=JValue();r=call("/quality","getSettings","{}");assert(truth(r["returnValue"])&&r["settings"].stringify()=="null");
        driverStatus=7;r=call("/quality","setSettings",R"({"category":"anything","value":{}})");assert(r["errorCode"].asNumber<int>()==20);
        driverStatus=0;r=call("/quality","setQualityAttribute",R"({"categoryType":"x","requestType":"y","isGameMode":true})");assert(truth(r["returnValue"])&&truth(forwarded["isGameMode"]));
        r=call("/quality","setQualityAttribute",R"({"categoryType":"x","requestType":"y","activeState":0})");assert(!truth(r["returnValue"]));
        QualityManager missing(h,nullptr);assert(missing.setSettings("setMixer",pbnjson::Object())==2);assert(missing.getSettings("external_arc",pbnjson::Object())["errorCode"].asNumber<int>()==20);
        // Restore the original registered object after testing null-client behavior.
        contexts["/quality"]=&quality;
        r=call("/audio","connect",R"({"source":"media","sink":"main","pipelineAttribute":{"id":3}})");assert(truth(r["returnValue"])&&first=="media"&&second=="main");
        r=call("/audio","connect",R"({"source":"media","sink":"main"})");assert(r["errorCode"].asNumber<int>()==205);
        auto* pair=connection.findAudioConnection("media","main");assert(pair&&!pair->muted&&pair->outputMode.empty());
        assert(str(connection.buildAudioStatus(*pair)["outputMode"])=="null");
        before=calls;assert(connection.doMuteAudio(*pair,false)&&calls==before);
        driverStatus=5;assert(!connection.doMuteAudio(*pair,true)&&!pair->muted);driverStatus=0;
        assert(connection.doMuteAudio(*pair,true)&&pair->muted);
        assert(connection.doMuteAudio(*pair,false)&&!pair->muted);
        before=calls;r=call("/audio","mute",R"({"source":"media","sink":"main","mute":false})");assert(truth(r["returnValue"])&&calls==before);
        r=call("/audio","setSoundOut",R"({"soundOut":"external_arc"})");assert(truth(r["returnValue"])&&pair->outputMode=="external_arc"&&volume.getSoundOutputVolume("external_arc")==37);
        r=call("/audio/volume","set",R"({"soundOutput":"external_arc","volume":356})");assert(truth(r["returnValue"])&&volume.getSoundOutputVolume("external_arc")==100);
        r=call("/audio/volume","up",R"({"soundOutput":"external_arc"})");assert(r["errorCode"].asNumber<int>()==202);
        r=call("/audio/volume","down",R"({"soundOutput":"untracked"})");assert(truth(r["returnValue"])&&forwarded.asNumber<int>()==99&&volume.getSoundOutputVolume("untracked")==100);
        before=calls;r=call("/audio/volume","getStatus","{}");
        assert(truth(r["returnValue"])&&calls==before&&r["volumeStatus"].stringify()==R"([{"soundOutput":"external_arc","volume":100,"muted":false}])");
        r=call("/audio/volume","set",R"({"soundOutput":"external_arc","volume":-1})");assert(!truth(r["returnValue"]));
        r=call("/audio","setInputVolume",R"({"source":"not-connected","sink":"main","volume":256})");assert(truth(r["returnValue"])&&forwarded.asNumber<int>()==0);
        r=call("/audio","getStatus",R"({"subscribe":true})");assert(!truth(r["returnValue"]));
        driverStatus=3;r=call("/audio","disconnect",R"({"source":"media","sink":"main"})");assert(!truth(r["returnValue"])&&!connection.findAudioConnection("media","main"));driverStatus=0;
        std::vector<std::string> sources;
        for(int index=0;index<20;++index){
            sources.push_back(std::string(40,'s')+std::to_string(index));
            r=call("/audio","connect",std::string("{\"source\":\"")+sources.back()+"\",\"sink\":\"main\"}");
            assert(truth(r["returnValue"]));
        }
        for(int removed:{0,10,19}){
            connection.removeAudioConnection(sources[removed],"main");
            assert(!connection.findAudioConnection(sources[removed],"main"));
        }
        for(int index=0;index<20;++index){
            if(index==0||index==10||index==19)continue;
            auto* remaining=connection.findAudioConnection(sources[index],"main");
            assert(remaining&&remaining->source==sources[index]&&remaining->sink=="main");
        }
        before=calls;r=call("/audio","getStatus","{}");
        auto expected=pbnjson::JArray();
        for(int index=0;index<20;++index){
            if(index==0||index==10||index==19)continue;
            expected.append(connection.buildAudioStatus(*connection.findAudioConnection(sources[index],"main")));
        }
        assert(truth(r["returnValue"])&&calls==before&&r["audio"].stringify()==expected.stringify());
        r=call("/audio","getHeadphoneStatus",R"({"subscribe":true})");assert(subscriptions==1&&timerCalls==1&&interval==500&&str(r["headPhoneStatus"]).empty());
        assert(category=="headphoneDetectionMute"&&truth(forwarded["mute"]));
        before=calls;ConnectionManager::_headphoneStatusUpdateCallBack(&connection);assert(posts==0&&calls==before);
        hp=true;internal="connected";ConnectionManager::_headphoneStatusUpdateCallBack(&connection);assert(posts==1&&truth(notification["headPhoneStatus"])&&!truth(forwarded["mute"]));
        ConnectionManager::_headphoneStatusUpdateCallBack(&connection);assert(posts==1);
        driverStatus=9;hp=false;ConnectionManager::_headphoneStatusUpdateCallBack(&connection);assert(posts==1);
        r=call("/audio","getHeadphoneStatus",R"({})");assert(r.stringify()=="null");driverStatus=0;
        ConnectionManager::_headphoneStatusUpdateCallBack(nullptr);
    }
    assert(cancelled==1);
    std::cout<<"audiooutput contract: registration, schemas, forwarding, cache, error paths and notification tests passed\n";
}
