// Copyright (c) 2008-2019 LG Electronics, Inc.
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

/* Modified: source-assisted subset of webosose/umediaserver
 * af89f8ac3355027a90d7b299a78fb6fab9b6ffca,
 * src/resource_manager/ResourceManagerClient_c.cpp. Each tag was checked against
 * the shipped wrapper's ARM body and PLT relocations using LLVM, not REA.
 * RegisterPipeline adapts the public one-argument call to the shipped signature:
 * the ARM body constructs an empty app ID and passes false in r3.
 * Create, Acquire/TryAcquire, inline connection-ID access and firmware-only APIs
 * are not reconstructed here. No exact-GCC instruction match is claimed.
 */
#include <string>
#include "ResourceManagerClient_c.h"

namespace uMediaServer {
/* Imported method ABI only: no object layout, allocation or C++ client bodies.
 * Delete uses the observed virtual deleting-destructor slot, not sizeof.
 */
class ResourceManagerClient {
public:
    virtual ~ResourceManagerClient();
    bool registerPipeline(std::string type, const std::string &app_id, bool flag);
    bool unregisterPipeline();
    bool release(std::string resources);
    bool notifyActivity();
    bool notifyForeground();
    bool notifyBackground();
};
}
using uMediaServer::ResourceManagerClient;

/* @0x122d0: null-safe virtual deletion; always returns true. */
bool ResourceManagerClientDestroy(ResourceManagerClientHandle *handle)
{
    ResourceManagerClient *rc = reinterpret_cast<ResourceManagerClient *>(handle);
    if (rc)
        delete rc;
    return true;
}

/* @0x122e4: shipped overload adds an empty app ID and false. */
bool ResourceManagerClientRegisterPipeline(ResourceManagerClientHandle *handle, const char *type)
{
    ResourceManagerClient *rc = reinterpret_cast<ResourceManagerClient *>(handle);
    if (rc)
        return rc->registerPipeline(type, "", false);
    return false;
}

/* @0x124dc */
bool ResourceManagerClientUnregisterPipeline(ResourceManagerClientHandle *handle)
{
    ResourceManagerClient *rc = reinterpret_cast<ResourceManagerClient *>(handle);
    if (rc)
        return rc->unregisterPipeline();
    return false;
}

/* @0x1292c */
bool ResourceManagerClientRelease(ResourceManagerClientHandle *handle, const char *resources)
{
    ResourceManagerClient *rc = reinterpret_cast<ResourceManagerClient *>(handle);
    if (rc)
        return rc->release(resources);
    return false;
}

/* @0x129c0 */
bool ResourceManagerClientNotifyActivity(ResourceManagerClientHandle *handle)
{
    ResourceManagerClient *rc = reinterpret_cast<ResourceManagerClient *>(handle);
    if (rc)
        return rc->notifyActivity();
    return false;
}

/* @0x129c8 */
bool ResourceManagerClientNotifyForeground(ResourceManagerClientHandle *handle)
{
    ResourceManagerClient *rc = reinterpret_cast<ResourceManagerClient *>(handle);
    if (rc)
        return rc->notifyForeground();
    return false;
}

/* @0x129d8 */
bool ResourceManagerClientNotifyBackground(ResourceManagerClientHandle *handle)
{
    ResourceManagerClient *rc = reinterpret_cast<ResourceManagerClient *>(handle);
    if (rc)
        return rc->notifyBackground();
    return false;
}
