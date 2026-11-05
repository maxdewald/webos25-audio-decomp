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

// Based on lg-opensource/public/umediaserver/include/public/ResourceManagerClient.h.
// Partial method ABI only: not an allocatable replacement for the TV class.
// ARM fields below are confirmed by the saved export and LLVM disassembly.
// Constructors, trailing TV callbacks, logging and lifecycle are not recovered here.
#pragma once
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <pthread.h>
#include <string>

struct UMSConnectorHandle;
struct UMSConnectorMessage;
using UMSConnectorEventFunction = bool (*)(UMSConnectorHandle*, UMSConnectorMessage*, void*);
class UMSConnector {
public:
    const char* getMessageText(UMSConnectorMessage*);
    bool sendMessage(const std::string&, const std::string&, UMSConnectorEventFunction, void*);
    bool sendResponseObject(UMSConnectorHandle*, UMSConnectorMessage*, const std::string&);
};

namespace uMediaServer {
class Mutex; // TV custom Lock.h type, not the public source's std::mutex subclass.
enum connection_state_t { CONNECTION_OPENED, CONNECTION_CLOSED };
class ResourceManagerClient {
public:
    bool acquire(const std::string&, std::string&);
    bool tryAcquire(const std::string&, std::string&);
    bool release(std::string);
    bool finalize();
    bool _acquire(const std::string&, std::string&, bool = true);
    bool informWaiter(std::string, bool, std::string);
    std::string createRetObject(bool, const std::string&);
    bool policyActionResponse(UMSConnectorHandle*, UMSConnectorMessage*, void*);
    bool acquireCompleteResponse(UMSConnectorHandle*, UMSConnectorMessage*, void*);
    bool commandResponse(UMSConnectorHandle*, UMSConnectorMessage*, void*);
    bool signalEvent(uint32_t*, uint32_t, pthread_mutex_t*, pthread_cond_t*);
    struct acquire_waiter_t {
        using ptr_t = std::shared_ptr<acquire_waiter_t>;
        bool state = false;
        std::string acquire_response;
        std::condition_variable cv;
    };
    using Waiters = std::map<std::string, acquire_waiter_t::ptr_t>;
    using PolicyHandler = std::function<bool(const char*, const char*, const char*, const char*, const char*)>;
#ifdef RMC_HOST_TEST
    struct HostState {
        std::string connection_id, connection_category;
        connection_state_t connection_state = CONNECTION_CLOSED;
        Mutex* api_mutex = nullptr;
        Waiters acquire_waiters;
        std::mutex acquire_mutex;
        UMSConnector* connector = nullptr;
        std::string resource_manager_connection_id = "com.webos.media";
        PolicyHandler policyActionClientHandler;
    } host;
#define RMC_FIELD(type, name, offset) type& name() { return host.name; }
#else
    ResourceManagerClient() = delete;
#define RMC_FIELD(type, name, offset) \
    type& name() { return *reinterpret_cast<type*>(reinterpret_cast<char*>(this) + offset); }
#endif
    RMC_FIELD(std::string, connection_id, 0x1c)
    RMC_FIELD(std::string, connection_category, 0x34)
    RMC_FIELD(connection_state_t, connection_state, 0x4c)
    RMC_FIELD(Mutex*, api_mutex, 0xb4)
    RMC_FIELD(Waiters, acquire_waiters, 0xb8)
    RMC_FIELD(std::mutex, acquire_mutex, 0xd0)
    RMC_FIELD(UMSConnector*, connector, 0xf0)
    RMC_FIELD(std::string, resource_manager_connection_id, 0xf4)
    RMC_FIELD(PolicyHandler, policyActionClientHandler, 0x10c)
#undef RMC_FIELD
    static bool commandResponseCallback(UMSConnectorHandle* h, UMSConnectorMessage* m, void* ctx) {
        return static_cast<ResourceManagerClient*>(ctx)->commandResponse(h, m, ctx);
    }
};
#ifndef RMC_HOST_TEST
static_assert(sizeof(void*) == 4 && sizeof(std::string) == 24,
              "This field view requires the shipped 32-bit libstdc++ C++11 string ABI");
static_assert(sizeof(ResourceManagerClient::Waiters) == 24 && sizeof(std::mutex) == 24,
              "Target map/mutex ABI differs from the confirmed TV fields");
static_assert(sizeof(ResourceManagerClient::PolicyHandler) == 16 &&
              sizeof(ResourceManagerClient::acquire_waiter_t) == 80,
              "Target callback/waiter ABI differs from the confirmed TV fields");
#endif
} // namespace uMediaServer
