// Copyright (c) 2008-2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

// Source-assisted behavioral reconstruction from the pinned Apache source:
// lg-opensource/public/umediaserver/src/resource_manager/ResourceManagerClient.cpp
// Evidence: symbols/libresource_mgr_client.so.1.0.0.functions.tsv and matching
// build/decomp export, corroborated with LLVM Thumb disassembly of the original.
// Tags identify only reviewed bodies. Diagnostic/performance logging is omitted;
// no instruction-match or full-class ABI equivalence is claimed.
#include "ResourceManagerClient.h"
#include <pbnjson.hpp>

namespace uMediaServer {
using namespace pbnjson;
using std::string;
namespace {
class ApiLock {
    Mutex* mutex;
public:
    // TV FUN_0002394c locks the pthread mutex at offset zero, ignoring errors
    // except for an EDEADLK diagnostic. Do not substitute std::mutex::lock().
    explicit ApiLock(Mutex* value) : mutex(value) {
        if (mutex) pthread_mutex_lock(reinterpret_cast<pthread_mutex_t*>(mutex));
    }
    ~ApiLock() {
        if (mutex) pthread_mutex_unlock(reinterpret_cast<pthread_mutex_t*>(mutex));
    }
};
}

/* @0x1a350: blocking acquire forwards block=true. */
bool ResourceManagerClient::acquire(const string& resources, string& response)
{
    return _acquire(resources, response);
}

/* @0x1a358: tryAcquire selects the remote nonblocking method, but still waits locally. */
bool ResourceManagerClient::tryAcquire(const string& resources, string& response)
{
    return _acquire(resources, response, false);
}

/* @0x19154: TV waits without the public source's eight-second timeout or predicate. */
bool ResourceManagerClient::_acquire(const string& resources, string& response, bool block)
{
    ApiLock lock(api_mutex());
    if (connection_state() == CONNECTION_CLOSED)
        return false;
    JValue args = Object();
    args.put("connectionId", connection_id());
    args.put("resources", resources);
    JGenerator serializer(nullptr);
    string payload;
    if (!serializer.toString(args, JSchema::AllSchema(), payload))
        return false;

    acquire_waiter_t::ptr_t waiter(new acquire_waiter_t);
    acquire_waiters()[connection_id()] = waiter;
    string cmd = resource_manager_connection_id() + connection_category() +
                 (block ? "/acquire" : "/tryAcquire");
    std::unique_lock<std::mutex> outer_lock(acquire_mutex());
    connector()->sendMessage(cmd, payload, commandResponseCallback, this);
    waiter->cv.wait(outer_lock);
    auto it = acquire_waiters().find(connection_id());
    if (it == acquire_waiters().end())
        return false;
    const auto& waiter_response = *it->second;
    response = waiter_response.acquire_response;
    acquire_waiters().erase(connection_id());
    return waiter_response.state;
}

/* @0x1a360: reports successful serialization/dispatch, not sendMessage's return value. */
bool ResourceManagerClient::release(string resources)
{
    ApiLock lock(api_mutex());
    if (connection_state() == CONNECTION_CLOSED)
        return false;
    JValue args = Object();
    args.put("connectionId", connection_id());
    args.put("resources", resources);
    JGenerator serializer(nullptr);
    string payload;
    if (!serializer.toString(args, JSchema::AllSchema(), payload))
        return false;
    string cmd = resource_manager_connection_id() + connection_category() + "/release";
    connector()->sendMessage(cmd, payload, commandResponseCallback, this);
    return true;
}

/* @0x16604: missing waiter is a successful no-op; completion does not erase it. */
bool ResourceManagerClient::informWaiter(string id, bool state, string response)
{
    std::lock_guard<std::mutex> lock(acquire_mutex());
    auto it = acquire_waiters().find(id);
    if (it != acquire_waiters().end()) {
        it->second->state = state;
        it->second->acquire_response = response;
        it->second->cv.notify_one();
    }
    return true;
}

/* @0x16790: TV-only cancellation wakes current acquisition with false/empty response. */
bool ResourceManagerClient::finalize()
{
    informWaiter(connection_id(), false, "");
    return true;
}

/* @0x1fe7c: policy reply uses mediaId, not connectionId; serializer result is ignored. */
string ResourceManagerClient::createRetObject(bool accepted, const string& mediaId)
{
    JValue result = Object();
    JGenerator serializer(nullptr);
    string response;
    result.put("returnValue", accepted);
    result.put("mediaId", mediaId);
    serializer.toString(result, JSchema::AllSchema(), response);
    return response;
}

/* @0x206c0: first object key must be policyAction; empty TV handler rejects safely. */
bool ResourceManagerClient::policyActionResponse(UMSConnectorHandle* handle,
                                                UMSConnectorMessage* message, void*)
{
    JDomParser parser;
    string cmd = connector()->getMessageText(message);
    if (!parser.parse(cmd, JSchema::AllSchema()))
        return false;
    JValue state = parser.getDom();
    if (!(*state.begin()).first.isString())
        return false;
    string name = (*state.begin()).first.asString();
    JValue value = state[name];
    if (name != "policyAction")
        return false;
    string action = value["action"].asString();
    string id = value["connectionId"].asString();
    string type = value["requestor_type"].asString();
    string requestor = value["requestor_name"].asString();
    string resources = value["resources"].asString();
    bool accepted = false;
    if (policyActionClientHandler())
        accepted = policyActionClientHandler()(action.c_str(), resources.c_str(),
                                              type.c_str(), requestor.c_str(), id.c_str());
    string response = createRetObject(accepted, id);
    connector()->sendResponseObject(handle, message, response);
    return accepted;
}

/* @0x2109c: state key is required; raw response is delivered to connectionId's waiter. */
bool ResourceManagerClient::acquireCompleteResponse(UMSConnectorHandle*,
                                                   UMSConnectorMessage* message, void*)
{
    JDomParser parser;
    string cmd = connector()->getMessageText(message);
    if (!parser.parse(cmd, JSchema::AllSchema()))
        return false;
    JValue response = parser.getDom();
    if (!response.hasKey("state"))
        return false;
    bool state = response["state"].asBool();
    string id = response["connectionId"].asString();
    informWaiter(id, state, cmd);
    return true;
}

/* @0x217a8: TV requires returnValue, but only an existing false state wakes a waiter. */
bool ResourceManagerClient::commandResponse(UMSConnectorHandle*, UMSConnectorMessage* message, void*)
{
    JDomParser parser;
    string cmd = connector()->getMessageText(message);
    if (!parser.parse(cmd, JSchema::AllSchema()))
        return false;
    JValue response = parser.getDom();
    if (!response.hasKey("returnValue"))
        return false;
    if (response.hasKey("state") && !response["state"].asBool()) {
        string id = response["connectionId"].asString();
        informWaiter(id, false, cmd);
    }
    return true;
}

/* @0x16400: event mutation and single wakeup are protected by the caller's mutex. */
bool ResourceManagerClient::signalEvent(uint32_t* event, uint32_t state,
                                       pthread_mutex_t* lock, pthread_cond_t* condition)
{
    pthread_mutex_lock(lock);
    *event = state;
    pthread_cond_signal(condition);
    pthread_mutex_unlock(lock);
    return true;
}
} // namespace uMediaServer
