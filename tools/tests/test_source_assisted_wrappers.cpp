// Host semantic tests only: mock imported methods, not the original ARM runtime.
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include "../../src/libresource_mgr_client_c/ResourceManagerClient_c.cpp"
#include "../../src/libums_connector_wrapper/UMSConnector_wrapper.cpp"

namespace {
struct Calls {
    int count = 0;
    int destroyed = 0;
    bool result = false;
    bool flag = true;
    bool throw_method = false;
    const void *self = nullptr;
    UMSConnectorHandle *sender = nullptr;
    UMSConnectorMessage *message = nullptr;
    UMSConnectorEventHandler *methods = nullptr;
    UMSConnectorEventFunction callback = nullptr;
    void *context = nullptr;
    unsigned long long number = 0;
    std::string first, second, key;
    bool call(const void *object) {
        ++count;
        self = object;
        if (throw_method)
            throw std::runtime_error("mock method");
        return result;
    }
} calls;
char text[] = "message";
int loop_storage;
GMainLoop *loop = reinterpret_cast<GMainLoop *>(&loop_storage);
bool callback(UMSConnectorHandle *, UMSConnectorMessage *, void *) { return true; }
}

namespace uMediaServer {
ResourceManagerClient::~ResourceManagerClient() { ++calls.destroyed; }
bool ResourceManagerClient::registerPipeline(std::string type, const std::string &app, bool flag)
{
    calls.first = type;
    calls.second = app;
    calls.flag = flag;
    return calls.call(this);
}
bool ResourceManagerClient::unregisterPipeline() { return calls.call(this); }
bool ResourceManagerClient::release(std::string resources)
{
    calls.first = resources;
    return calls.call(this);
}
bool ResourceManagerClient::notifyActivity() { return calls.call(this); }
bool ResourceManagerClient::notifyForeground() { return calls.call(this); }
bool ResourceManagerClient::notifyBackground() { return calls.call(this); }
}

UMSConnector::~UMSConnector() { ++calls.destroyed; }
bool UMSConnector::wait() { return calls.call(this); }
bool UMSConnector::addEventHandler(std::string event, UMSConnectorEventFunction func, const std::string &category)
{
    calls.first = event;
    calls.callback = func;
    calls.key = category;
    return calls.call(this);
}
bool UMSConnector::addEventHandlers(UMSConnectorEventHandler *methods)
{
    calls.methods = methods;
    return calls.call(this);
}
const char *UMSConnector::getMessageText(UMSConnectorMessage *message)
{
    calls.message = message;
    calls.call(this);
    return text;
}
const char *UMSConnector::getMessageSender(UMSConnectorMessage *message)
{
    calls.message = message;
    calls.call(this);
    return text;
}
const char *UMSConnector::getSenderServiceName(UMSConnectorMessage *message)
{
    calls.message = message;
    calls.call(this);
    return text;
}
bool UMSConnector::sendMessage(const std::string &uri, const std::string &payload,
                               UMSConnectorEventFunction func, void *ctx)
{
    calls.first = uri;
    calls.second = payload;
    calls.callback = func;
    calls.context = ctx;
    return calls.call(this);
}
bool UMSConnector::sendSimpleResponse(UMSConnectorHandle *sender, UMSConnectorMessage *message, bool response)
{
    calls.sender = sender;
    calls.message = message;
    calls.flag = response;
    return calls.call(this);
}
bool UMSConnector::sendResponse(UMSConnectorHandle *sender, UMSConnectorMessage *message,
                                const std::string &key, const std::string &value)
{
    calls.sender = sender;
    calls.message = message;
    calls.first = key;
    calls.second = value;
    return calls.call(this);
}
bool UMSConnector::addSubscriber(UMSConnectorHandle *sender, UMSConnectorMessage *message, const std::string &key)
{
    calls.sender = sender;
    calls.message = message;
    calls.key = key;
    return calls.call(this);
}
bool UMSConnector::sendChangeNotificationLongLong(const std::string &name, unsigned long long value)
{
    calls.first = name;
    calls.number = value;
    return calls.call(this);
}
bool UMSConnector::sendChangeNotificationString(const std::string &name, const std::string &value,
                                               const std::string &key)
{
    calls.first = name;
    calls.second = value;
    calls.key = key;
    return calls.call(this);
}
bool UMSConnector::sendChangeNotificationJsonString(const std::string &json, const std::string &key)
{
    calls.first = json;
    calls.key = key;
    return calls.call(this);
}
GMainLoop *UMSConnector::getMainLoop() { calls.call(this); return loop; }

int main()
{
    assert(ResourceManagerClientDestroy(nullptr));
    assert(!ResourceManagerClientRegisterPipeline(nullptr, nullptr));
    assert(!ResourceManagerClientUnregisterPipeline(nullptr));
    assert(!ResourceManagerClientRelease(nullptr, nullptr));
    assert(!ResourceManagerClientNotifyActivity(nullptr));
    assert(!ResourceManagerClientNotifyForeground(nullptr));
    assert(!ResourceManagerClientNotifyBackground(nullptr));
    assert(UMSConnectorDestroy(nullptr));
    assert(!UMSConnectorWait(nullptr));
    assert(!UMSConnectorAddEventHandler(nullptr, std::string(), nullptr));
    assert(!UMSConnectorAddEventHandlers(nullptr, nullptr));
    assert(!UMSConnectorGetMessageText(nullptr, nullptr));
    assert(!UMSConnectorGetMessageSender(nullptr, nullptr));
    assert(!UMSConnectorGetSenderServiceName(nullptr, nullptr));
    assert(!UMSConnectorSendMessage(nullptr, nullptr, nullptr, nullptr, nullptr));
    assert(!UMSConnectorSendSimpleResponse(nullptr, nullptr, nullptr, false));
    assert(!UMSConnectorSendResponse(nullptr, nullptr, nullptr, nullptr, nullptr));
    assert(!UMSConnectorAddSubscriber(nullptr, nullptr, nullptr));
    assert(!UMSConnectorSendChangeNotificationLongLong(nullptr, nullptr, 0));
    assert(!UMSConnectorSendChangeNotificationString(nullptr, nullptr, nullptr));
    assert(!UMSConnectorSendChangeNotificationJsonString(nullptr, nullptr));
    assert(!UMSConnectorGetMainLoop(nullptr));
    assert(calls.count == 0 && calls.destroyed == 0);

    ResourceManagerClient resource;
    auto r = reinterpret_cast<ResourceManagerClientHandle *>(&resource);
    UMSConnector connector;
    auto c = reinterpret_cast<UMSConnectorHandle *>(&connector);
    int message_storage;
    auto message = reinterpret_cast<UMSConnectorMessage *>(&message_storage);
    UMSConnectorEventHandler handlers[] = {{"event", callback}, {nullptr, nullptr}};
    const std::string long_text(128, 'x');
    char value[] = "value";
    for (bool expected : {false, true}) {
        calls.result = expected;
        assert(ResourceManagerClientRegisterPipeline(r, long_text.c_str()) == expected);
        assert(calls.self == &resource && calls.first == long_text && calls.second.empty() && !calls.flag);
        assert(ResourceManagerClientRelease(r, long_text.c_str()) == expected);
        assert(calls.first == long_text);
        assert(ResourceManagerClientUnregisterPipeline(r) == expected);
        assert(ResourceManagerClientNotifyActivity(r) == expected);
        assert(ResourceManagerClientNotifyForeground(r) == expected);
        assert(ResourceManagerClientNotifyBackground(r) == expected);
        assert(UMSConnectorWait(c) == expected);
        assert(calls.self == &connector);
        assert(UMSConnectorAddEventHandler(c, long_text, callback) == expected);
        assert(calls.first == long_text && calls.callback == callback && calls.key.empty());
        assert(UMSConnectorAddEventHandlers(c, handlers) == expected);
        assert(calls.methods == handlers);
        assert(UMSConnectorGetMessageText(c, message) == text);
        assert(UMSConnectorGetMessageSender(c, message) == text);
        assert(UMSConnectorGetSenderServiceName(c, message) == text);
        assert(calls.message == message);
        assert(UMSConnectorSendMessage(c, long_text.c_str(), "payload", callback, &resource) == expected);
        assert(calls.first == long_text && calls.second == "payload" && calls.callback == callback && calls.context == &resource);
        assert(UMSConnectorSendSimpleResponse(c, c, message, expected) == expected);
        assert(calls.sender == c && calls.message == message && calls.flag == expected);
        assert(UMSConnectorSendResponse(c, c, message, "key", value) == expected);
        assert(calls.sender == c && calls.message == message && calls.first == "key" && calls.second == value);
        assert(UMSConnectorAddSubscriber(c, c, message) == expected);
        assert(calls.sender == c && calls.message == message && calls.key.empty());
        assert(UMSConnectorSendChangeNotificationLongLong(c, "number", UINT64_C(0xfedcba9876543210)) == expected);
        assert(calls.first == "number" && calls.number == UINT64_C(0xfedcba9876543210));
        assert(UMSConnectorSendChangeNotificationString(c, "name", value) == expected);
        assert(calls.first == "name" && calls.second == value && calls.key.empty());
        assert(UMSConnectorSendChangeNotificationJsonString(c, long_text.c_str()) == expected);
        assert(calls.first == long_text && calls.key.empty());
        assert(UMSConnectorGetMainLoop(c) == loop);
    }
    calls.throw_method = true;
    bool caught = false;
    try { UMSConnectorSendMessage(c, long_text.c_str(), "payload", callback, nullptr); }
    catch (const std::runtime_error &) { caught = true; }
    assert(caught);
    caught = false;
    try { ResourceManagerClientRelease(r, long_text.c_str()); }
    catch (const std::runtime_error &) { caught = true; }
    assert(caught);
    calls.throw_method = false;

    struct Derived : ResourceManagerClient {
        bool &deleted;
        explicit Derived(bool &value) : deleted(value) {}
        ~Derived() override { deleted = true; }
    };
    bool deleted = false;
    assert(ResourceManagerClientDestroy(reinterpret_cast<ResourceManagerClientHandle *>(new Derived(deleted))));
    assert(deleted && calls.destroyed == 1);
    assert(UMSConnectorDestroy(reinterpret_cast<UMSConnectorHandle *>(new UMSConnector)));
    assert(calls.destroyed == 2);
    std::cout << "22 wrapper bodies: null guards, forwarding, defaults, exceptions, deletion passed\n";
}
