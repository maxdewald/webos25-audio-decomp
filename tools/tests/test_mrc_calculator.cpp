// @sources src/libmedia-resource-calculator/resource.cpp src/libmedia-resource-calculator/calculator.cpp src/libmedia-resource-calculator/calculator_destructor.cpp src/libmedia-resource-calculator/video_info.cpp
// @cflags -std=c++14 -Isrc/libmedia-resource-calculator -Ilg-opensource/public/media-resource-calculator/calculator
/* Host-only fixtures; no claim that these synthetic tables are TV defaults. */
#include <map>
#include <list>
#include <string>
#include <vector>
#define private public
#define protected public
#include "calculator.h"
#undef protected
#undef private
#include <cassert>
#include <iostream>

namespace mrc {
// Production platform-table constructor remains unresolved, not reconstructed.
ResourceCalculator::ResourceCalculator() {}
}

int main()
{
    using namespace mrc;
    for (const auto &row : std::map<std::string, VideoInfo>{
             {"1920*1088 60p", {1920, 1088, 60}},
             {"2k 59i", {2048, 1080, 29}}, {"4K 120P", {4096, 2160, 120}},
             {"2K 4k 61I", {4096, 2160, 30}}, {"", {0, 0, 0}},
             {"unknown  30p", {0, 0, 30}}, {"default", {0, 0, 0}},
             {"max", {0, 0, 0, true}}, {"xxmaxxx", {0, 0, 0, true}}, {"prefixmaxsuffix", {0, 0, 0}},
             {"max 1920*1080 60p", {1920, 1080, 60, true}},
             {"1920*1080 60p max", {1920, 1080, 60, true}},
             {"* 60p", {0, 0, 60}}, {"-3*4 -61i", {-3, 4, -30}}}) {
        const VideoInfo parsed(row.first);
        assert(parsed.width == row.second.width && parsed.height == row.second.height);
        assert(parsed.frameRate == row.second.frameRate && parsed.isDefault == row.second.isDefault);
    }
    assert(VideoInfo(100, 100, 30).isSubsetOf(VideoInfo(200, 50, 30)));
    assert(!VideoInfo(100, 100, 31).isSubsetOf(VideoInfo(200, 50, 30)));
    assert(VideoInfo(4000, 4000, 1000).isSubsetOf(VideoInfo("max")));
    ResourceCalculator calculator;
    auto &audio = calculator.adecTable.resourceData;
    audio["aac6"] = {{"ADEC", 1}, {"NOTSUPPORTED", 1}};
    audio["pcm"] = {{"ADEC", 1}, {"PCM", 1}};
    audio["dts"] = {{"ADEC", 1}, {"DTS", 1}};
    audio["dtse"] = {{"ADEC", 1}, {"BW", 4}};
    audio["mpeg-h"] = {{"ADEC", 1}, {"MPEGH", 1}};
    audio["ac4"] = {{"ADEC", 1}, {"AC4", 1}};
    audio["atmos"] = {{"ADEC", 1}, {"BW", 3}};
    audio["description"] = {{"ADEC", 1}, {"AD", 1}};
    audio["mat"] = {{"ADEC", 1}, {"MAT", 1}};
    audio["max"] = {{"ADEC", 1}, {"MAX", 1}};
    audio["iamf"] = {{"ADEC", 1}, {"IAMF", 1}};
    audio["default"] = {{"ADEC", 1}, {"BW", 2}};
    assert(calculator.adecTable.lookup("missing") == nullptr);
    assert(calculator.adecTable.lookup("mat") == &audio["mat"]);
    assert(calculator.calcAdecResources(0, 0, 0)[1].quantity == 2);
    for (int version : {2, 4}) {
        auto aac = calculator.calcAdecResources(ResourceCalculator::kAudioMPEG, version, 6);
        assert(aac.size() == 2 && aac[1].type == "NOTSUPPORTED");
    }
    for (int version : {0, 1, 3, 5})
        assert(calculator.calcAdecResources(ResourceCalculator::kAudioMPEG, version, 6)[1].type == "BW");
    assert(calculator.calcAdecResources(ResourceCalculator::kAudioMPEG, 4, 2)[1].type == "BW");
    auto combined = calculator.calcAdecResources(ResourceCalculator::kAudioDTSE | ResourceCalculator::kAudioATMOS | ResourceCalculator::kAudioEtc, 0, 0);
    assert(combined.size() == 2 && combined[1].quantity == 4);
    for (const auto &item : std::map<int, std::string>{
             {4, "PCM"}, {8, "DTS"}, {32, "MPEGH"}, {64, "AC4"},
             {256, "AD"}, {512, "MAT"}, {1024, "MAX"}, {2048, "IAMF"}}) {
        auto result = calculator.calcAdecResources(item.first, 0, 0);
        assert(result.size() == 2 && result[1].type == item.second);
    }
    audio.erase("iamf");
    assert(calculator.calcAdecResources(ResourceCalculator::kAudioIAMF, 0, 0)[1].type == "BW");
    for (bool secure : {false, true}) {
        for (bool reformatter : {false, true}) {
            auto result = calculator.calcMiscResources(secure, reformatter);
            assert(result.size() == (secure ? 1u : 0u));
            if (secure)
                assert(result[0].type == "SVP_CPB" && result[0].quantity == 1);
        }
    }
    const char *names[] = {"default", "H264", "H265", "MPEG", "MVC", "SVC", "VP9", "RM", "AVS", "VP8", "MJPEG", "MPEG4", "JPEG", "AV1", "WMV", "H266", "H263"};
    for (int bit = 0; bit < 17; ++bit)
        assert(calculator.videoCodecToString(static_cast<ResourceCalculator::VideoCodec>(1 << bit)) == names[bit]);
    assert(calculator.videoCodecToString(static_cast<ResourceCalculator::VideoCodec>(0)) == "default");
    assert(calculator.videoCodecToString(static_cast<ResourceCalculator::VideoCodec>(6)) == "default");

    auto &video = calculator.vdecTable.resourceTable;
    video["H264"] = {{{1280, 720, 30, false}, {{{"VDEC", 1}}, {{"ALT", 1}}}},
                     {{1920, 1080, 60, false}, {{{"VDEC", 2}}, {{"ALT", 2}}}}};
    video["H265"] = {{{1920, 1080, 60, false}, {{{"VDEC", 3}}}}};
    assert(!calculator.vdecTable.lookup("missing", 1, 1, 1));
    assert(!calculator.vdecTable.lookup("H264", 4000, 4000, 1000));
    auto first = calculator.calcVdecResources(2, 1280, 720, 30, ResourceCalculator::kScanProgressive, ResourceCalculator::k3DNone);
    assert(first.size() == 1 && first[0].quantity == 1);
    auto multi = calculator.calcVdecResources(6, 1280, 720, 30, ResourceCalculator::kScanProgressive, ResourceCalculator::k3DNone);
    assert(multi.size() == 1 && multi[0].quantity == 3);
    auto interlaced = calculator.calcVdecResources(2, 1280, 720, 60, ResourceCalculator::kScanInterlaced, ResourceCalculator::k3DNone);
    assert(interlaced[0].quantity == 1);
    auto stereo = calculator.calcVdecResources(2, 1280, 720, 30, ResourceCalculator::kScanProgressive, ResourceCalculator::k3DSequential);
    assert(stereo[0].quantity == 2);
    auto unsupported = calculator.calcVdecResources(2, 4000, 4000, 30, ResourceCalculator::kScanProgressive, ResourceCalculator::k3DNone);
    assert(unsupported.size() == 1 && unsupported[0].type == "NOTSUPPORTED");
    assert(calculator.calcVdecResources(0, 0, 0, 0, ResourceCalculator::kScanProgressive, ResourceCalculator::k3DNone).empty());
    auto options = calculator.calcVdecResourceOptions(6, 1280, 720, 30, ResourceCalculator::kScanProgressive, ResourceCalculator::k3DNone);
    assert(options.size() == 2 && options[0].size() == 1 && options[0][0].quantity == 3);
    assert(options[1].size() == 2 && options[1][0].type == "ALT" && options[1][1].quantity == 3);
    auto badOptions = calculator.calcVdecResourceOptions(2, 4000, 4000, 30, ResourceCalculator::kScanProgressive, ResourceCalculator::k3DNone);
    assert(badOptions.size() == 1 && badOptions[0][0].type == "NOTSUPPORTED");
    assert(calculator.calcVdecResourceOptions(0, 0, 0, 0, ResourceCalculator::kScanProgressive, ResourceCalculator::k3DNone).empty());
    int width = -1, height = -1, rate = -1;
    assert(!calculator.getMaxVideoResolution(0, &width, &height, &rate));
    assert(width == -1 && height == -1 && rate == -1);
    assert(calculator.getMaxFrameRate(0, 1, 1) == 0);
    assert(calculator.getMaxVideoResolution(6, &width, &height, &rate));
    assert(width == 1920 && height == 1080 && rate == 60);
    assert(calculator.getMaxFrameRate(6, 1280, 720) == 30);
    assert(calculator.getMaxFrameRate(2, 4000, 4000) == 0);
    video["default"] = {{{0, 0, 24, true}, {{{"FALLBACK", 1}}}}};
    assert(calculator.vdecTable.lookup("unknown", 40000, 40000, 40000)->front()[0].type == "FALLBACK");
    assert(calculator.vdecTable.getMaxFrameRate("unknown", 40000, 40000) == 24);
    assert(!calculator.vdecTable.getMaxResolution("unknown", &width, &height, &rate));
    video["H264"].push_back({{640, 2000, 50, false}, {{{"VDEC", 4}}}});
    assert(calculator.getMaxVideoResolution(2, &width, &height, &rate));
    assert(width == 640 && height == 2000 && rate == 50);
    auto *allocated = ResourceCalculator::create();
    delete allocated;
    std::cout << "PASS: TV audio codec extensions, secure-path quantity, video alternatives/defaults and query selection\n";
}
