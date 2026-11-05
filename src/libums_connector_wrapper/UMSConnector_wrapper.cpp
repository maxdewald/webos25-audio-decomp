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

/* Modified: source-assisted subset of webosose/umediaserver
 * af89f8ac3355027a90d7b299a78fb6fab9b6ffca,
 * src/ums_connector/UMSConnector_wrapper.cpp, checked against shipped ARM bodies
 * and PLT relocations with LLVM (REA unavailable). Imported methods are declared
 * below without inventing the private class layout. No objects are allocated.
 * Create/CreatePrivate are omitted: upstream's constructor has six arguments,
 * the shipped import has five. Subscribe is omitted: upstream returns size_t
 * converted to bool, whereas the shipped wrapper returns the method result
 * without normalization. No exact-GCC instruction match is claimed.
 */
#include <string>
typedef struct _GMainLoop GMainLoop;
#include "UMSConnector_wrapper.h"

class UMSConnector {
public:
    ~UMSConnector();
    bool wait();
    bool addEventHandler(std::string event, UMSConnectorEventFunction func,
                         const std::string &category = "");
    bool addEventHandlers(UMSConnectorEventHandler *methods);
    const char *getMessageText(UMSConnectorMessage *message);
    const char *getMessageSender(UMSConnectorMessage *message);
    const char *getSenderServiceName(UMSConnectorMessage *message);
    bool sendMessage(const std::string &uri, const std::string &payload,
                     UMSConnectorEventFunction func, void *ctx);
    bool sendSimpleResponse(UMSConnectorHandle *sender, UMSConnectorMessage *message, bool resp);
    bool sendResponse(UMSConnectorHandle *sender, UMSConnectorMessage *message,
                      const std::string &key, const std::string &value);
    bool addSubscriber(UMSConnectorHandle *sender, UMSConnectorMessage *message,
                       const std::string &key = "");
    bool sendChangeNotificationLongLong(const std::string &name, unsigned long long value);
    bool sendChangeNotificationString(const std::string &name, const std::string &value,
                                      const std::string &key = "");
    bool sendChangeNotificationJsonString(const std::string &json, const std::string &key = "");
    GMainLoop *getMainLoop();
};

/* @0x11694: null-safe destruction; uses unsized operator delete as shipped. */
bool UMSConnectorDestroy(UMSConnectorHandle *handle)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        delete connector;
    return true;
}

/* @0x116ac */
bool UMSConnectorWait(UMSConnectorHandle *handle)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->wait();
    return false;
}

/* @0x116b4: shipped export is the C++ string overload, not the public C prototype. */
bool UMSConnectorAddEventHandler(UMSConnectorHandle *handle, std::string event, UMSConnectorEventFunction func)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->addEventHandler(event, func);
    return false;
}

/* @0x1178c */
bool UMSConnectorAddEventHandlers(UMSConnectorHandle *handle, UMSConnectorEventHandler *methods)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->addEventHandlers(methods);
    return false;
}

/* @0x11794 */
const char *UMSConnectorGetMessageText(UMSConnectorHandle *handle, UMSConnectorMessage *message)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->getMessageText(message);
    return nullptr;
}

/* @0x1179c */
const char *UMSConnectorGetMessageSender(UMSConnectorHandle *handle, UMSConnectorMessage *message)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->getMessageSender(message);
    return nullptr;
}

/* @0x117a4 */
const char *UMSConnectorGetSenderServiceName(UMSConnectorHandle *handle, UMSConnectorMessage *message)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->getSenderServiceName(message);
    return nullptr;
}

/* @0x117ac */
bool UMSConnectorSendMessage(UMSConnectorHandle *handle, const char *uri, const char *payload,
                             UMSConnectorEventFunction func, void *ctx)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->sendMessage(uri, payload, func, ctx);
    return false;
}

/* @0x118e4 */
bool UMSConnectorSendSimpleResponse(UMSConnectorHandle *handle, UMSConnectorHandle *sender,
                                    UMSConnectorMessage *message, bool resp)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->sendSimpleResponse(sender, message, resp);
    return false;
}

/* @0x118ec */
bool UMSConnectorSendResponse(UMSConnectorHandle *handle, UMSConnectorHandle *sender,
                              UMSConnectorMessage *message, const char *key, const char *value)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->sendResponse(sender, message, key, value);
    return false;
}

/* @0x11b5c */
bool UMSConnectorAddSubscriber(UMSConnectorHandle *handle, UMSConnectorHandle *sender,
                               UMSConnectorMessage *message)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->addSubscriber(sender, message);
    return false;
}

/* @0x11ba0 */
bool UMSConnectorSendChangeNotificationLongLong(UMSConnectorHandle *handle, const char *name, unsigned long long value)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->sendChangeNotificationLongLong(name, value);
    return false;
}

/* @0x11c3c */
bool UMSConnectorSendChangeNotificationString(UMSConnectorHandle *handle, const char *name, char *value)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->sendChangeNotificationString(name, value);
    return false;
}

/* @0x11d84 */
bool UMSConnectorSendChangeNotificationJsonString(UMSConnectorHandle *handle, const char *json)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->sendChangeNotificationJsonString(json);
    return false;
}

/* @0x11e48 */
GMainLoop *UMSConnectorGetMainLoop(UMSConnectorHandle *handle)
{
    UMSConnector *connector = reinterpret_cast<UMSConnector *>(handle);
    if (connector)
        return connector->getMainLoop();
    return nullptr;
}
