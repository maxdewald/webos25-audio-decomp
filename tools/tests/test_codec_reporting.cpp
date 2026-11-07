// @sources src/starfish-media-pipeline/metadata.cpp src/starfish-media-pipeline/track_metadata.cpp
// @cflags -std=c++14 -isystem lg-opensource/public/boost
#include "../../src/starfish-media-pipeline/metadata.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <cassert>
#include <iostream>
#include <limits>
#include <stdexcept>

static void audioReporting()
{
    smp::AudioTrack track;
    track.channels = 6;
    smp::AudioReportingContext state;
    state.selectedTrack = &track;
    state.audioTrackCount = 1;
    state.pipelineId = "pipeline-test";
    state.adecPort = 1;
    for (const char *codec : {"dts", "DTS", "dtshd", "DTSHD", "dts_hd_ma", "audio/x-dts", "dts-hd"}) {
        track.codec = codec;
        auto report = smp::generateAudioDecodeInfo(state);
        assert(report.get<std::string>("categoryInfo.codec") == codec);
        assert(report.get<int>("categoryInfo.channel") == 6);
    }
    track.codec = "x-raw";
    auto report = smp::generateAudioDecodeInfo(state);
    assert(report.get<std::string>("categoryInfo.codec") == "pcm");
    assert(report.get<std::string>("pipelineInfo.resource") == "ADEC1");
    assert(report.get<std::string>("pipelineInfo.pipelinetype") == "audio");
    assert(report.get<std::string>("categoryType") == "audioInputInfo");
    assert(report.get<std::string>("requestType") == "setAudioInputInfo");
    track.codec = "audio/x-raw";
    assert(smp::generateAudioDecodeInfo(state).get<std::string>("categoryInfo.codec") == "audio/x-raw");
    track.codec = "mpeg-h";
    assert(smp::generateAudioDecodeInfo(state).get<std::string>("categoryInfo.codec") == "mpeg_h");
    track.codec = "dts_hd_ma";
    state.codecName = "x-raw";
    assert(smp::generateAudioDecodeInfo(state).get<std::string>("categoryInfo.codec") == "pcm");
    state.codecName = "DTSHD";
    state.outChannels = 8;
    state.immersive = "ATMOS";
    state.mixerPort = 3;
    state.videoTrackCount = 1;
    state.videoSinkType = 4;
    state.params = "format=raw";
    state.hasAudioDropControl = true;
    state.disableAudioDrop = true;
    report = smp::generateAudioDecodeInfo(state);
    assert(report.get<std::string>("categoryInfo.codec") == "DTSHD");
    assert(report.get<int>("categoryInfo.channel") == 8);
    assert(report.get<std::string>("categoryInfo.immersive") == "atmos");
    assert(report.get_child("categoryInfo.params").front().second.data() == "format=raw");
    assert(report.get<std::string>("pipelineInfo.resource") == "AMIXER3");
    assert(report.get<std::string>("pipelineInfo.videoType") == "sub_video");
    assert(!report.get<bool>("pipelineInfo.audioDropEnable"));
    state.reset = true;
    report = smp::generateAudioDecodeInfo(state);
    assert(report.get<std::string>("categoryInfo.codec") == "none");
    assert(report.get<std::string>("categoryInfo.immersive") == "none");
    assert(!report.get_optional<int>("categoryInfo.channel"));
    state.reset = false;
    state.audioTrackCount = 0;
    state.selectedTrack = nullptr;
    assert(smp::generateAudioDecodeInfo(state).get<std::string>("categoryInfo.codec") == "none");
    state.audioTrackCount = 1;
    state.selectedTrack = &track;
    state.codecName.clear();
    state.outChannels = -1;
    track.codec.clear();
    assert(smp::generateAudioDecodeInfo(state).get<std::string>("categoryInfo.codec").empty());
    assert(smp::generateAudioDecodeInfo(state).get<int>("categoryInfo.channel") == 6);
}
static void eventUpdates()
{
    smp::AudioReportingContext state;
    smp::AudioTrack tracks[1];
    smp::Ptree event;
    event.put("track", 0);
    event.put("dualMono", true);
    assert(!smp::handleAudioInfoEvent(state, tracks, 1, event));
    assert(tracks[0].dualMono);
    event.put("codec-name", "x-raw");
    assert(smp::handleAudioInfoEvent(state, tracks, 1, event));
    assert(!smp::handleAudioInfoEvent(state, tracks, 1, event));
    event.put("out-channels", 2);
    assert(smp::handleAudioInfoEvent(state, tracks, 1, event));
    assert(state.outChannels == 2 && state.codecName == "x-raw");
    event.put("track", 2);
    event.put("codec-name", "DTSHD");
    assert(!smp::handleAudioInfoEvent(state, tracks, 1, event));
    assert(state.codecName == "x-raw");
    assert(state.audioInfoEvent.get<std::string>("codec-name") == "DTSHD");
    state.sourceType = -1;
    event.put("track", 0);
    assert(!smp::handleAudioInfoEvent(state, tracks, 1, event));
    assert(state.codecName == "x-raw");
    assert(state.audioInfoEvent.get<int>("track") == 0);
    state.sourceType = 0;
    event.put("immersive", "ATMOS");
    assert(smp::handleAudioInfoEvent(state, tracks, 1, event));
    assert(state.immersive == "ATMOS" && state.internalSourceImmersive == "ATMOS");
    assert(state.audioInfoEvent.get<std::string>("immersive") == "none");
    state.internalSourceImmersive = "unchanged";
    assert(!smp::handleAudioInfoEvent(state, tracks, 1, event));
    assert(state.internalSourceImmersive == "unchanged");
}
static void audioInfoCaller()
{
    smp::AudioTrack tracks[2];
    tracks[0].codec = "dts";
    tracks[0].channels = 6;
    smp::AudioReportingContext state;
    state.selectedTrack = &tracks[0];
    state.audioTrackCount = 2;
    int submissions = 0, notifications = 0;
    std::string order;
    smp::Ptree report, forwarded;
    const std::int64_t number = 0x123456789LL;
    smp::AudioInfoEventCallbacks callbacks;
    callbacks.setDecodeInfo = [&](const std::string &kind, const std::string &json) {
        assert(kind == "audio");
        assert(smp::util::parseJson(json.c_str(), &report));
        assert(report.get<std::string>("requestType") == "setAudioInputInfo");
        ++submissions;
        order += "d";
    };
    callbacks.sendCallbackEvent = [&](int type, std::int64_t payload, const std::string &json) {
        assert(type == 7 && payload == number);
        assert(smp::util::parseJson(json.c_str(), &forwarded));
        ++notifications;
        order += "c";
    };
    const char *update = R"({"track":1,"codec-name":"dts_hd_ma","out-channels":8,"immersive":"ATMOS","extra":"preserved"})";
    smp::callbackFunctionHook(state, tracks, 2, 8, number, update, callbacks);
    smp::callbackFunctionHook(state, tracks, 2, 7, number, nullptr, callbacks);
    smp::callbackFunctionHook(state, tracks, 2, 7, number, "", callbacks);
    assert(submissions == 0 && notifications == 0 && state.audioInfoEvent.empty());
    smp::callbackFunctionHook(state, tracks, 2, 7, number, update, callbacks);
    assert(order == "dc" && submissions == 1 && notifications == 1);
    assert(report.get<std::string>("categoryInfo.codec") == "dts_hd_ma");
    assert(report.get<int>("categoryInfo.channel") == 8);
    assert(report.get<std::string>("categoryInfo.immersive") == "atmos");
    assert(forwarded.get<int>("track") == 1);
    assert(forwarded.get<std::string>("immersive") == "none");
    assert(forwarded.get<std::string>("extra") == "preserved");
    smp::callbackFunctionHook(state, tracks, 2, 7, number, update, callbacks);
    assert(order == "dcc" && submissions == 1 && notifications == 2);
    state.audioInfoDescription = "Dolby Atmos";
    smp::callbackFunctionHook(state, tracks, 2, 7, number, update, callbacks);
    assert(submissions == 1 && forwarded.get<std::string>("immersive") == "ATMOS");
    smp::callbackFunctionHook(state, tracks, 2, 7, number, R"({"track":1,"dualMono":true})", callbacks);
    assert(tracks[1].dualMono && !tracks[0].dualMono && submissions == 1);
    assert(!forwarded.get_optional<std::string>("codec-name"));
    assert(state.codecName == "dts_hd_ma" && state.outChannels == 8);
    state.sourceType = -1;
    smp::callbackFunctionHook(state, tracks, 2, 7, number, R"({"track":0,"codec-name":"dts"})", callbacks);
    assert(submissions == 1 && state.codecName == "dts_hd_ma");
    assert(forwarded.get<std::string>("codec-name") == "dts");
    state.sourceType = 0;
    smp::callbackFunctionHook(state, tracks, 2, 7, number, R"({"track":2,"codec-name":"dts"})", callbacks);
    assert(submissions == 1 && state.codecName == "dts_hd_ma" && forwarded.get<int>("track") == 2);
    int previousNotifications = notifications;
    bool threw = false;
    try { smp::callbackFunctionHook(state, tracks, 2, 7, number, "{", callbacks); }
    catch (const boost::property_tree::json_parser_error &) { threw = true; }
    assert(threw && notifications == previousNotifications && submissions == 1);
    threw = false;
    try { smp::callbackFunctionHook(state, tracks, 2, 7, number, R"({"codec-name":"dts"})", callbacks); }
    catch (const boost::property_tree::ptree_bad_path &) { threw = true; }
    assert(threw && notifications == previousNotifications && submissions == 1);
    assert(state.audioInfoEvent.get<std::string>("codec-name") == "dts");
    assert(!state.audioInfoEvent.get_optional<int>("track"));
    smp::callbackFunctionHook(state, tracks, 2, 7, number,
        R"({"track":0,"dualMono":"invalid","out-channels":"invalid"})", callbacks);
    assert(submissions == 1 && state.outChannels == 8 && !tracks[0].dualMono);
    smp::callbackFunctionHook(state, tracks, 2, 7, number,
        R"({"track":0,"out-channels":-1})", callbacks);
    assert(submissions == 2 && state.outChannels == -1);
    assert(report.get<int>("categoryInfo.channel") == 6);
    state.reset = true;
    smp::callbackFunctionHook(state, tracks, 2, 7, number,
        R"({"track":0,"immersive":"none"})", callbacks);
    assert(submissions == 3 && state.internalSourceImmersive == "none");
    assert(report.get<std::string>("categoryInfo.codec") == "none");
    assert(report.get<std::string>("categoryInfo.immersive") == "none");
    assert(order.substr(order.size() - 4) == "dcdc");
}
static void trackParsers()
{
    smp_internal::AudioTrack audio;
    audio.fromString("audio/x-raw p l 48000 2 en 10 20 0 0 x-raw 7 main 1 none 1 0 r pre text 1 tail");
    assert(audio.mimeType == "audio/x-raw" && audio.codec == "x-raw");
    assert(audio.channels == 2 && audio.sampleRate == 48000);
    assert(audio.audioEmergency && audio.ac4Text == "text 1 tail");
    auto report = audio.makeAudioTrackPtree();
    assert(report.get<std::string>("codec") == "x-raw");
    assert(!report.get_optional<std::string>("mimeType"));
    audio.fromString("");
    assert(audio.codec == "(null)" && audio.channels == 0 && audio.immersive == "none");
    assert(!audio.makeAudioTrackPtree().get_optional<std::string>("codec"));
    assert(audio.makeAudioTrackPtree().get<int>("channels") == 0);
    smp::Ptree missing;
    assert(!smp::util::parseJson(nullptr, &missing));
    assert(!smp::util::parseJson("", &missing));
    assert(!audio.fromJsonString("{"));
    assert(!audio.fromJsonString("{}"));
    assert(audio.fromJsonString(R"({"audioTrackInfo":{"mimeType":"audio/x-dts","codec":"DTSHD","sampleRate":48000,"channels":8,"ratings":[]}})"));
    assert(audio.codec == "DTSHD" && audio.channels == 8 && audio.sampleRate == 48000);
    assert(audio.fromJsonString(R"({"audioTrackInfo":{"ratings":[]}})"));
    assert(audio.codec == "(null)" && audio.mimeType.empty() && !audio.selected);
    assert(!audio.makeAudioTrackPtree().get_optional<std::string>("codec"));
    bool threw = false;
    try { audio.fromJsonString(R"({"audioTrackInfo":{}})"); }
    catch (const boost::property_tree::ptree_bad_path &) { threw = true; }
    assert(threw);
    smp_internal::VideoTrack video;
    video.fromString("0 video/x-h264 high 4 1920 1080 1:1 60 1000 1 2 h264 3 main 0 h264 1 0 r");
    assert(video.width == 1920 && video.frameRate == 60 && video.codec == "h264");
    assert(video.makeVideoTrackPtree().get<std::string>("orgCodec") == "h264");
    video.fromString("");
    assert(video.width == 0 && video.codec == "(null)");
    video.progressive = true;
    assert(video.fromJsonString(R"({"videoTrackInfo":{"width":3840,"height":2160,"frameRate":120,"codec":"h265","ratings":[]}})"));
    assert(video.width == 3840 && video.codec == "h265" && video.progressive);
    smp::AudioTrack publicAudio;
    assert(publicAudio.makeAudioTrackPtree().get<int>("bitRate") == 0);
    assert(!publicAudio.makeAudioTrackPtree().get_optional<int>("channels"));
}
static void videoReporting()
{
    smp::VideoTrack track;
    track.width = 1920; track.height = 1088; track.frameRate = 120;
    assert(!track.isHFR());
    track.height = 1089;
    assert(track.isHFR());
    track.frameRate = 60;
    assert(!track.isHFR());
    track.frameRate = std::numeric_limits<float>::quiet_NaN();
    assert(!track.isHFR());
    smp::VideoReportingContext state;
    state.width = 3840; state.height = 2160; state.frameRate = 120;
    state.hdrType = "hdr10"; state.rotation = 90;
    state.hdr.type = 4;
    state.hdr.colorPrimaries = 9;
    state.hdr.maxContentLightLevel = 0;
    auto report = smp::generateVideoDecodeInfo(state);
    assert(report.get<bool>("videoInfo.hfrEnabled"));
    assert(report.get<std::string>("videoInfo.rotation") == "Deg90");
    assert(report.get<int>("videoInfo.vui.colorPrimaries") == 9);
    assert(!report.get<bool>("videoInfo.vui.videoFullRangeFlag"));
    assert(report.get<int>("videoInfo.sei.maxContentLightLevel") == 0);
    assert(!report.get_optional<std::string>("videoInfo.appStatus"));
    state.playerType = 0x116; state.adaptive = true;
    state.hasAdaptiveTracks = true; state.adaptiveHfr = false;
    report = smp::generateVideoDecodeInfo(state);
    assert(!report.get<bool>("videoInfo.adaptive"));
    assert(!report.get<bool>("videoInfo.hfrEnabled"));
    assert(smp::generateStreamTypeForAudioD(10, false) == "umioverlay");
    assert(smp::generateStreamTypeForAudioD(12, false) == "umimediamusic");
    assert(smp::generateStreamTypeForAudioD(0, true) == "umibluetooth");
    assert(smp::getContentType("com.webos.app.livedmost", "", false, 0) == "ipch");
    assert(smp::getContentType("", "o208k", true, 0x116) == "dtv_8k");
    assert(smp::getContentType("", "", false, 0x113) == "atsc30");
    assert(smp::isAudioOnly(12) && !smp::isAudioOnly(10));
    smp::Ptree scalars;
    scalars.put("codec", "pcm"); scalars.put("channel", "2"); scalars.put("reset", false);
    std::string json = smp::util::generateJsonString(scalars);
    assert(json.find("\"channel\":2") != std::string::npos);
    assert(json.find("\"codec\":\"pcm\"") != std::string::npos);
    assert(json.find("\"reset\":false") != std::string::npos);
}
int main()
{
    audioReporting(); eventUpdates(); audioInfoCaller(); trackParsers(); videoReporting();
    std::cout << "codec reporting: classification, missing fields, wire formats, events, HDR passed\n";
}
