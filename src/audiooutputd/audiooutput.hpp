#pragma once
#include <cstdint>
#include <exception>
#include <string>
#include <vector>
#include <utility>

// Behavioral reconstruction of shipped ARM code; diagnostic logging is abbreviated.
// Minimal shipped ABI declarations, not replacement implementations of LS2/UMI.
// Host doubles test control flow/contracts, not target execution or LS2 permissions.
#ifdef AUDIOOUTPUT_HOST_TEST
#include "test_audiooutput_contract_json.hpp"
#else
extern "C" void* jarray_create(void*);
namespace pbnjson {
class JValue {
public:
    virtual ~JValue();
    JValue();
    JValue(const JValue&);
    JValue(bool);
    JValue(int);
    JValue(const char*);
    JValue(const std::string&);
    bool put(const std::string&, const JValue&);
    bool append(const JValue&);
    JValue operator[](const char*) const;
    bool operator==(bool) const;
    int asBool(bool&) const;
    int asString(std::string&) const;
    template<class T> T asNumber() const;
    std::string stringify(const char* = nullptr);
    JValue& operator=(JValue other) { std::swap(schema, other.schema); std::swap(value, other.value); return *this; }
    static JValue JNULL;
protected:
    friend class JArray;
    JValue(void* s, void* v):schema(s),value(v){}
    void* schema;
    void* value;
};
JValue Object();
class JArray : public JValue {
public:
    JArray():JValue(nullptr, jarray_create(nullptr)) { if (!value) value = JNULL.value; }
};
class JSchema {
    void* state[3];
public:
    JSchema(const JSchema&);
    ~JSchema();
    static const JSchema& AllSchema();
    void swap(JSchema&);
};
class JSchemaFragment : public JSchema {
public:
    explicit JSchemaFragment(const std::string&);
    ~JSchemaFragment();
};
struct JInput { const char* data; unsigned length; explicit JInput(const std::string& s):data(s.data()),length(s.size()){} };
class JDomParser {
    unsigned char state[112];
public:
    JDomParser();
    ~JDomParser();
    bool parse(const JInput&, const JSchema&);
    JValue getDom();
    const char* getError();
};
class JResolver;
class JGenerator {
    unsigned char state[12];
public:
    explicit JGenerator(JResolver*);
    ~JGenerator();
    bool toString(const JValue&, const JSchema&, std::string&);
};
}
extern "C" void* jarray_create(void*);
#endif

struct LSHandle;
struct LSMessage;
struct LSError { int error_code; const char* message; const char* file; int line; const char* func; void* reserved[2]; };
using LSFilterFunc = bool (*)(LSHandle*, LSMessage*, void*);
struct LSMethod { const char* name; LSFilterFunc function; unsigned flags; };
extern "C" {
void LSErrorInit(LSError*);
void LSErrorFree(LSError*);
bool LSRegister(const char*, LSHandle**, LSError*);
bool LSUnregister(LSHandle*, LSError*);
bool LSRegisterCategory(LSHandle*, const char*, const LSMethod*, void*, void*, LSError*);
bool LSCategorySetData(LSHandle*, const char*, void*, LSError*);
const char* LSMessageGetPayload(LSMessage*);
void LSMessageRef(LSMessage*);
void LSMessageUnref(LSMessage*);
bool LSMessageRespond(LSMessage*, const char*, LSError*);
bool LSCallCancelNotificationAdd(LSHandle*, bool (*)(LSHandle*, const char*, void*), void*, LSError*);
bool LSCallCancelNotificationRemove(LSHandle*, bool (*)(LSHandle*, const char*, void*), void*, LSError*);
unsigned g_timeout_add(unsigned, int (*)(void*), void*);
int g_source_remove(unsigned);
int PmLogGetContext(const char*, void**);
void _PmLogMsgKV(void*, int, unsigned, const char*, unsigned, unsigned, unsigned, const char*, ...);
}
namespace LS {
struct Handle { LSHandle* handle; };
class Error : public std::exception {
public:
    LSError error;
    Error() { LSErrorInit(&error); }
    Error(const Error& other):std::exception(other),error(other.error) { LSErrorInit(const_cast<LSError*>(&other.error)); }
    ~Error() noexcept override { LSErrorFree(&error); }
};
struct Message {
    LSMessage* message;
    explicit Message(LSMessage& m):message(&m) { LSMessageRef(message); }
    ~Message() { if (message) LSMessageUnref(message); }
};
struct SubscriptionPoint {
    struct SubscriptionItem { LSMessage* message; void* item; bool active; };
    Handle* handle = nullptr;
    std::vector<SubscriptionItem*> items;
    std::string service;
    bool flag = false;
    static bool subscriberCancelCB(LSHandle*, const char*, void*);
    static void cleanItem(SubscriptionItem*);
    bool subscribe(Message&);
    bool post(const char*);
    ~SubscriptionPoint();
};
}
class umiClient {
public:
    static umiClient* getInstance();
    bool initialize();
    bool deinitialize();
    int setSettings(std::string, pbnjson::JValue);
    pbnjson::JValue getSettings(const std::string&, pbnjson::JValue);
    int setQualityAttribute(pbnjson::JValue);
    int connectInput(std::string, std::string, pbnjson::JValue);
    int disconnectInput(std::string, std::string);
    int setSoundOutput(std::string);
    int setMute(std::string, std::string, bool);
    int setInputVolume(std::string, std::string, unsigned char);
    int removePipelineInfo(std::string);
    int getHeadphoneStatus(bool*, std::string&, std::string&);
    int setOutputVolume(std::string, unsigned char);
    int setOutputMute(std::string, bool);
    int getDefaultVolume();
};
using pbnjson::JValue;
struct AudioOutput { std::string soundOutput; bool muted = false; unsigned char volume = 100; };
struct AudioConnection { std::string source, sink, outputMode; bool muted = false; };
class QualityManager {
    LS::Handle* handle;
    umiClient* client;
public:
    QualityManager(LS::Handle&, umiClient*);
    static bool isValidCategory(std::string&);
    int setSettings(const std::string&, JValue);
    JValue getSettings(const std::string&, JValue);
    int setQualityAttribute(JValue);
    bool setSettings(LSMessage&);
    bool getSettings(LSMessage&);
    bool setQualityAttribute(LSMessage&);
};
class VolumeManager {
    LS::Handle* handle;
    umiClient* client;
    AudioOutput output;
public:
    VolumeManager(LS::Handle&, umiClient*);
    void setSoundOutputVolume(const std::string&, unsigned char);
    unsigned char getSoundOutputVolume(const std::string&);
    void setSoundOutputMute(const std::string&, bool);
    void updateSoundOutput(const std::string&);
    int set(const std::string&, const int&);
    int muteSoundOut(const std::string&, const int&);
    JValue buildAudioStatus(AudioOutput&);
    JValue buildAudioStatus();
    JValue getStatus();
    bool set(LSMessage&);
    bool up(LSMessage&);
    bool down(LSMessage&);
    bool muteSoundOut(LSMessage&);
    bool getStatus(LSMessage&);
};
class ConnectionManager : public LS::SubscriptionPoint {
    VolumeManager& volume;
    std::vector<AudioConnection> connections;
    LS::Handle* bus;
    umiClient* client;
    unsigned timer;
    bool headphone;
    std::string internalStatus, externalStatus;
    JValue headphoneResponse();
public:
    ConnectionManager(LS::Handle&, VolumeManager&, umiClient*);
    ~ConnectionManager();
    static int _headphoneStatusUpdateCallBack(void*);
    int connect(const std::string&, const std::string&, JValue&);
    int disconnect(const std::string&, const std::string&);
    int setSoundOut(const std::string&);
    int mute(const std::string&, const std::string&, const bool&);
    int setInputVolume(const std::string&, const std::string&, const int&);
    int getHeadphoneStatus(bool*, std::string&, std::string&);
    int removePipelineInfo(const std::string&);
    AudioConnection* findAudioConnection(const std::string&, const std::string&);
    void removeAudioConnection(const std::string&, const std::string&);
    void doDisconnectAudio(AudioConnection&);
    bool doMuteAudio(AudioConnection&, bool);
    JValue buildAudioStatus(const AudioConnection&);
    JValue buildStatus();
    JValue getStatus();
    bool connect(LSMessage&);
    bool disconnect(LSMessage&);
    bool setSoundOut(LSMessage&);
    bool mute(LSMessage&);
    bool setInputVolume(LSMessage&);
    bool getStatus(LSMessage&);
    bool getHeadphoneStatus(LSMessage&);
    bool removePipelineInfo(LSMessage&);
};
void* getPmLogContext();
int setPmLogContext(const char*);
namespace Contract {
bool parse(const std::string&, JValue&, const std::string&, int&) __asm__("audiooutput_parse");
void respond(LS::Message&, const JValue&) __asm__("audiooutput_respond");
void respondError(LS::Message&, const std::string&, int, bool) __asm__("audiooutput_error");
extern const LSMethod qualityMethods[], connectionMethods[], volumeMethods[];
extern const char *const qualitySetSchema, *const qualityGetSchema, *const qualityAttributeSchema;
extern const char *const connectionSchema, *const pairSchema, *const inputVolumeSchema, *const inputMuteSchema;
extern const char *const soundOutSchema, *const pipelineSchema, *const headphoneSchema, *const emptySchema;
extern const char *const volumeSchema, *const volumeOutputSchema, *const volumeMuteSchema;
std::string text(const JValue&);
bool boolean(const JValue&);
JValue array();
bool parse(const std::string&, JValue&, const std::string&, int&);
bool request(LS::Message&, const char*, JValue&);
void respond(LS::Message&, const JValue&);
void respondError(LS::Message&, const std::string&, int, bool = false);
JValue result(int);
void registerCategory(LS::Handle&, const char*, const LSMethod*, void*);
}
