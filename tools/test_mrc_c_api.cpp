/* Synthetic fixtures for the TV C facade; no platform-table defaults claimed. */
#include <map>
#include <list>
#include <string>
#include <vector>
#define private public
#define protected public
#include "calculator.h"
#undef protected
#undef private
#include "resource_calculator_c.h"
#include <cassert>
#include <cstring>
#include <iostream>

extern "C" void MRCDeleteResourceListOptions(MRCResourceListOptions);
static int constructed;
namespace mrc {
ResourceCalculator::ResourceCalculator()
{
    ++constructed;
    adecTable.resourceData["aac6"] = {{"AAC6", 2}};
    adecTable.resourceData["default"] = {{"DEFAULT", 1}};
    vdecTable.resourceTable["H264"] = {{{1280, 720, 30, false}, {{{"FIRST", 1}}, {{"SECOND", 2}}}}};
}
}

int main()
{
    MRCDeleteResourceList(nullptr);
    MRCDeleteResourceListOptions(nullptr);
    assert(!MRCCalcVdecResources(0, 0, 0, 0, kScanProgressive, k3DNone));
    auto audio = MRCCalcAdecResources(kAudioMPEG, 4, 6);
    assert(audio && audio[0] && !audio[1]);
    assert(std::strcmp(audio[0]->type, "AAC6") == 0 && audio[0]->quantity == 2);
    audio[0]->type[0] = 'X';
    MRCDeleteResourceList(audio);
    audio = MRCCalcAdecResources(kAudioMPEG, 4, 6);
    assert(std::strcmp(audio[0]->type, "AAC6") == 0);
    MRCDeleteResourceList(audio);
    audio = MRCCalcAdecResources(kAudioMPEG, 6, 4);
    assert(std::strcmp(audio[0]->type, "DEFAULT") == 0);
    MRCDeleteResourceList(audio);
    assert(!MRCCalcMiscResources(0, -2));
    auto secure = MRCCalcMiscResources(-7, 0);
    assert(secure && secure[0] && !secure[1]);
    assert(std::strcmp(secure[0]->type, "SVP_CPB") == 0 && secure[0]->quantity == 1);
    MRCDeleteResourceList(secure);
    auto video = MRCCalcVdecResources(kVideoH264, 1280, 720, 60, kScanInterlaced, k3DNone);
    assert(video && std::strcmp(video[0]->type, "FIRST") == 0 && !video[1]);
    MRCDeleteResourceList(video);
    video = MRCCalcVdecResources(kVideoH264, 0, 0, 0, kScanProgressive, k3DNone);
    assert(video && std::strcmp(video[0]->type, "FIRST") == 0);
    MRCDeleteResourceList(video);
    assert(!MRCCalcVdecResourceOptions(0, 0, 0, 0, kScanProgressive, k3DNone));
    auto options = MRCCalcVdecResourceOptions(kVideoH264, 1280, 720, 30, kScanProgressive, k3DNone);
    assert(options && options[0] && options[1] && !options[2]);
    assert(!options[0][1] && !options[1][1]);
    assert(std::strcmp(options[0][0]->type, "FIRST") == 0);
    assert(std::strcmp(options[1][0]->type, "SECOND") == 0 && options[1][0]->quantity == 2);
    MRCDeleteResourceListOptions(options);
    options = MRCCalcVdecResourceOptions(kVideoH264, 1280, 720, 60, kScanProgressive, k3DSequential);
    assert(options && options[0] && !options[1]);
    assert(std::strcmp(options[0][0]->type, "NOTSUPPORTED") == 0);
    MRCDeleteResourceListOptions(options);
    assert(constructed == 1);
    std::cout << "PASS: TV C ownership, termination, singleton and argument forwarding\n";
}
