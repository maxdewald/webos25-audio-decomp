// @sources src/libpf/audio_metadata.cpp
// @cflags -std=c++14 -Isrc/libpf -isystem lg-opensource/public/rapidjson/include
// @pkg gobject-2.0
#include "audio_metadata.h"
#include <cassert>
#include <cstring>
#include <iostream>

static void audioInfoEvents()
{
    GValue stream = G_VALUE_INIT, codec = G_VALUE_INIT, immersive = G_VALUE_INIT;
    GValue channels = G_VALUE_INIT, dualMono = G_VALUE_INIT, wrongChannels = G_VALUE_INIT;
    g_value_init(&stream, G_TYPE_STRING); g_value_set_string(&stream, "audio-stream");
    g_value_init(&codec, G_TYPE_STRING); g_value_set_string(&codec, "dts_hd_ma");
    g_value_init(&immersive, G_TYPE_STRING); g_value_set_string(&immersive, "ATMOS");
    g_value_init(&channels, G_TYPE_INT); g_value_set_int(&channels, 8);
    g_value_init(&dualMono, G_TYPE_BOOLEAN); g_value_set_boolean(&dualMono, TRUE);
    g_value_init(&wrongChannels, G_TYPE_UINT); g_value_set_uint(&wrongChannels, 6);
    mediapipeline::AudioInfoMessage message;
    message.structureName = "media-info";
    message.sourceName = "dtsdec0";
    message.streamId = &stream;
    message.codecName = &codec;
    message.immersive = &immersive;
    message.outChannels = &channels;
    message.dualMono = &dualMono;
    rapidjson::Document event;
    auto read = [&](bool generic = true, bool atmos = true) {
        return mediapipeline::readAudioInfoEvent(message, generic, 1, atmos, event);
    };
    assert(read());
    assert(event["track"].GetInt() == 1 && event["out-channels"].GetInt() == 8);
    message.hasPlayerHandle = false;
    assert(!read());
    message.hasPlayerHandle = true;
    assert(event["dualMono"].GetBool());
    assert(std::strcmp(event["codec-name"].GetString(), "dts_hd_ma") == 0);
    assert(std::strcmp(event["immersive"].GetString(), "ATMOS") == 0);
    assert(read(true, false) && std::strcmp(event["immersive"].GetString(), "none") == 0);
    g_value_set_string(&immersive, "");
    assert(read() && std::strcmp(event["immersive"].GetString(), "none") == 0);
    for (const char *name : {"DTSHD", "x-raw", "DTS"}) {
        g_value_set_string(&codec, name);
        assert(read() && !event.HasMember("codec-name"));
    }
    g_value_set_string(&codec, "vendor-dts-extra");
    assert(read() && event.HasMember("codec-name"));
    message.outChannels = &wrongChannels;
    assert(read() && event["out-channels"].GetInt() == -1);
    message.sourceName = "audio-decproxy0";
    assert(!read() && read(false));
    message.sourceName = "Decproxy0";
    assert(read());
    message.sourceName = nullptr;
    assert(read());
    message.messageType = 0x8000;
    assert(!read());
    message.messageType = 0x4000;
    message.structureName = "audio-info";
    assert(!read());
    message.structureName = "media-info";
    message.streamId = nullptr;
    assert(!read());
    message.streamId = &stream;
    g_value_set_string(&stream, nullptr);
    assert(!read());
    g_value_set_string(&stream, "audio-stream");
    message.dualMono = message.immersive = message.codecName = message.outChannels = nullptr;
    assert(!read());
    message.codecName = &codec;
    g_value_set_string(&codec, "DTSHD");
    assert(read() && event.MemberCount() == 1 && event["track"].GetInt() == 1);
    g_value_unset(&stream); g_value_unset(&codec); g_value_unset(&immersive);
    g_value_unset(&channels); g_value_unset(&dualMono); g_value_unset(&wrongChannels);
}

static void customStreamRepair()
{
    GValue stream = G_VALUE_INIT, codec = G_VALUE_INIT, channels = G_VALUE_INIT;
    g_value_init(&stream, G_TYPE_STRING); g_value_set_string(&stream, "decoder-stream");
    g_value_init(&codec, G_TYPE_STRING); g_value_set_string(&codec, "dts");
    g_value_init(&channels, G_TYPE_INT); g_value_set_int(&channels, 8);
    mediapipeline::AudioInfoMessage message;
    message.structureName = "media-info";
    message.sourceName = "decproxy0";
    message.streamId = &stream;
    message.outChannels = &channels;
    std::vector<std::string> streamIds = {"0"};
    rapidjson::Document event;
    auto read = [&] { return mediapipeline::readCustomAudioInfoEvent(message, streamIds, true, event); };
    assert(read() && event["track"].GetInt() == -1 && streamIds[0] == "0");
    message.codecName = &codec;
    message.messageType = 0x8000;
    assert(!read() && streamIds[0] == "0");
    message.messageType = 0x4000;
    message.hasPlayerHandle = false;
    assert(!read() && streamIds[0] == "0");
    message.hasPlayerHandle = true;
    assert(read() && event["track"].GetInt() == 0 && streamIds[0] == "decoder-stream");
    streamIds = {"different"};
    assert(read() && event["track"].GetInt() == -1 && streamIds[0] == "different");
    streamIds = {"0", "decoder-stream"};
    assert(read() && event["track"].GetInt() == 1 && streamIds[0] == "0");
    streamIds.clear();
    assert(read() && event["track"].GetInt() == -1);
    g_value_unset(&stream); g_value_unset(&codec); g_value_unset(&channels);
}

int main()
{
    using mediapipeline::dropGenericMediaInfo;
    assert(dropGenericMediaInfo("media-info", "decproxy0"));
    assert(dropGenericMediaInfo("media-info", "audio-decproxy0"));
    assert(dropGenericMediaInfo("media-info", "decproxy"));
    assert(!dropGenericMediaInfo("media-info", "Decproxy0"));
    assert(!dropGenericMediaInfo("media-info", "lgaudiodec0"));
    assert(!dropGenericMediaInfo("media-info", nullptr));
    assert(!dropGenericMediaInfo("media-info-extra", "decproxy0"));
    assert(!dropGenericMediaInfo("other", "decproxy0"));
    assert(!dropGenericMediaInfo(nullptr, "decproxy0"));
    audioInfoEvents();
    customStreamRepair();
    std::cout << "libpf media-info routing passed\n";
}