// @sources src/libresource_mgr_client/ResourceManagerClient.cpp
// @cflags -std=c++14 -DRMC_HOST_TEST -Ilg-opensource/public/rapidjson/include
// Host-only tests: no target library loading or Luna/GLib execution.
#include "../../../src/libresource_mgr_client/ResourceManagerClient.h"
#include <pbnjson.hpp>
#include <cassert>
#include <chrono>
#include <future>
#include <iostream>

struct UMSConnectorHandle {};
struct UMSConnectorMessage { std::string text; };
namespace {
std::string uri, payload, reply;
UMSConnectorEventFunction callback;
void* context;
unsigned sends = 0, responses = 0;
std::promise<void>* sent = nullptr;
using Client = uMediaServer::ResourceManagerClient;
pbnjson::JValue parse(const std::string& text) {
    pbnjson::JDomParser parser;
    assert(parser.parse(text, pbnjson::JSchema::AllSchema()));
    return parser.getDom();
}
}
const char* UMSConnector::getMessageText(UMSConnectorMessage* message) { return message->text.c_str(); }
bool UMSConnector::sendMessage(const std::string& method, const std::string& args,
                               UMSConnectorEventFunction handler, void* ctx) {
    uri = method;
    payload = args;
    callback = handler;
    context = ctx;
    ++sends;
    if (sent) sent->set_value();
    return false;
}
bool UMSConnector::sendResponseObject(UMSConnectorHandle*, UMSConnectorMessage*, const std::string& text) {
    reply = text;
    ++responses;
    return false;
}
int main() {
    Client client;
    UMSConnector connector;
    pthread_mutex_t api = PTHREAD_MUTEX_INITIALIZER;
    client.host.connector = &connector;
    client.host.api_mutex = reinterpret_cast<uMediaServer::Mutex*>(&api);
    client.host.connection_id = "pipeline\"1";
    client.host.connection_category = "/rm";
    std::string output = "unchanged";
    assert(!client.acquire("[]", output) && output == "unchanged" && sends == 0);
    assert(!client.tryAcquire("[]", output));
    assert(!client.release("[]"));
    client.host.connection_state = uMediaServer::CONNECTION_OPENED;
    pbnjson::failSerialization() = true;
    assert(!client.acquire("[]", output) && client.host.acquire_waiters.empty());
    assert(!client.release("[]") && sends == 0);
    assert(client.createRetObject(true, "id").empty());
    pbnjson::failSerialization() = false;
    assert(client.release("[{\"resource\":\"ADEC\"}]"));
    assert(uri == "com.webos.media/rm/release");
    auto args = parse(payload);
    assert(args["connectionId"].asString() == client.host.connection_id);
    assert(args["resources"].asString() == "[{\"resource\":\"ADEC\"}]");
    assert(context == &client && callback);

    for (int mode = 0; mode != 3; ++mode) {
        std::promise<void> signal;
        auto dispatch = signal.get_future();
        sent = &signal;
        auto acquisition = std::async(std::launch::async, [&] {
            return mode == 1 ? client.tryAcquire("[]", output) : client.acquire("[]", output);
        });
        assert(dispatch.wait_for(std::chrono::seconds(2)) == std::future_status::ready);
        assert(uri == (mode == 1 ? "com.webos.media/rm/tryAcquire" : "com.webos.media/rm/acquire"));
        UMSConnectorMessage complete{"{\"state\":true,\"connectionId\":\"pipeline\\\"1\"}"};
        if (mode == 0) {
            assert(client.acquireCompleteResponse(nullptr, &complete, nullptr));
        } else if (mode == 1) {
            complete.text = "{\"returnValue\":true,\"state\":false,\"connectionId\":\"pipeline\\\"1\"}";
            assert(callback(nullptr, &complete, context));
        } else {
            assert(client.finalize());
        }
        assert(acquisition.wait_for(std::chrono::seconds(2)) == std::future_status::ready);
        assert(acquisition.get() == (mode == 0));
        assert(output == (mode == 2 ? "" : complete.text));
        assert(client.host.acquire_waiters.empty());
        sent = nullptr;
    }
    {
        std::promise<void> signal;
        auto dispatch = signal.get_future();
        sent = &signal;
        output = "missing-waiter";
        auto acquisition = std::async(std::launch::async, [&] { return client.acquire("[]", output); });
        assert(dispatch.wait_for(std::chrono::seconds(2)) == std::future_status::ready);
        {
            std::lock_guard<std::mutex> lock(client.host.acquire_mutex);
            auto held = client.host.acquire_waiters.at(client.host.connection_id);
            client.host.acquire_waiters.erase(client.host.connection_id);
            held->cv.notify_one();
        }
        assert(acquisition.wait_for(std::chrono::seconds(2)) == std::future_status::ready);
        assert(!acquisition.get() && output == "missing-waiter");
        sent = nullptr;
    }
    client.host.api_mutex = nullptr;
    assert(client.release("not parsed by the client"));
    client.host.api_mutex = reinterpret_cast<uMediaServer::Mutex*>(&api);
    auto waiter = std::make_shared<Client::acquire_waiter_t>();
    client.host.acquire_waiters[client.host.connection_id] = waiter;
    assert(client.informWaiter("missing", true, "ignored"));
    assert(waiter->acquire_response.empty());
    UMSConnectorMessage message{"{\"returnValue\":false,\"connectionId\":\"pipeline\\\"1\"}"};
    assert(client.commandResponse(nullptr, &message, nullptr));
    assert(waiter->acquire_response.empty()); // TV missing-state guard; do not fabricate failure.
    message.text = "{\"returnValue\":false,\"state\":true}";
    assert(client.commandResponse(nullptr, &message, nullptr));
    assert(waiter->acquire_response.empty()); // state, not returnValue, controls wakeup.
    message.text = "{\"state\":false}";
    assert(!client.commandResponse(nullptr, &message, nullptr));
    message.text = "{}";
    assert(!client.acquireCompleteResponse(nullptr, &message, nullptr));
    message.text = "invalid";
    assert(!client.acquireCompleteResponse(nullptr, &message, nullptr));
    assert(!client.commandResponse(nullptr, &message, nullptr));
    assert(!client.policyActionResponse(nullptr, &message, nullptr));
    message.text = "{\"other\":{}}";
    assert(!client.policyActionResponse(nullptr, &message, nullptr) && responses == 0);
    message.text = "{\"policyAction\":{\"action\":\"release\",\"resources\":\"[]\",\"connectionId\":\"victim\",\"requestor_type\":\"media\",\"requestor_name\":\"requestor\"}}";
    assert(!client.policyActionResponse(nullptr, &message, nullptr));
    auto result = parse(reply);
    assert(!result["returnValue"].asBool() && result["mediaId"].asString() == "victim");
    client.host.policyActionClientHandler = [](const char* action, const char* resources,
                                              const char* type, const char* name, const char* id) {
        assert(std::string(action) == "release" && std::string(resources) == "[]");
        assert(std::string(type) == "media" && std::string(name) == "requestor" && std::string(id) == "victim");
        return true;
    };
    assert(client.policyActionResponse(nullptr, &message, nullptr));
    result = parse(reply);
    assert(result["returnValue"].asBool() && !result.hasKey("connectionId"));
    message.text = "{\"other\":{},\"policyAction\":{}}";
    assert(!client.policyActionResponse(nullptr, &message, nullptr));
    assert(responses == 2);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
    uint32_t event = 99;
    assert(client.signalEvent(&event, 7, &mutex, &condition) && event == 7);
    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&api);
    std::cout << "RMC core host tests passed (11 reviewed routines; target not executed)\n";
}
