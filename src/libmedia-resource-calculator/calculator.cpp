// SPDX-License-Identifier: Apache-2.0
/* Copyright (c) 2008-2021 LG Electronics, Inc.
 * Licensed under the Apache License, Version 2.0; see upstream notices in
 * lg-opensource/public/media-resource-calculator/.
 * Adapted from pinned calculator/resource_calculator.cpp and resource_table.cpp.
 * Verified against saved Ghidra bodies, LLVM ARM disassembly and ELF relocations;
 * not an exact-GCC instruction-match claim. Original SHA-256:
 * 9cf65c00a778d9b408b1dd701349642be55338394f4720f6008b5187aeed2d06.
 * TV JSON/platform constructors remain
 * unresolved; they must not be replaced by OSE's Raspberry Pi resource tables.
 */
#include "calculator.h"
#include <cstdint>

namespace mrc {
#if UINTPTR_MAX == UINT32_MAX
static_assert(sizeof(ResourceTable) == 0x1c, "TV resource table ABI");
static_assert(sizeof(VideoResourceTable) == 0x1c, "TV video table ABI");
static_assert(sizeof(ResourceCalculator) == 0x3c, "TV calculator ABI");
static_assert(sizeof(VideoInfo) == 0x10, "TV default-video marker ABI");
#endif

/* @0x14bd8 */
ResourceTable::ResourceTable() {}

/* @0x14ba0 = _ZN3mrc13ResourceTableD1Ev */
ResourceTable::~ResourceTable() {}
/* @0x14bc4 = _ZN3mrc13ResourceTableD0Ev */

/* @0x14c04: missing codec entries contribute no resource requirements. */
const ResourceList *ResourceTable::lookup(const std::string &key) const
{
    const auto found = resourceData.find(key);
    return found == resourceData.end() ? nullptr : &found->second;
}

/* @0x165f0 */
VideoResourceTable::VideoResourceTable() {}

/* @0x16a74 = _ZN3mrc18VideoResourceTableD1Ev */
VideoResourceTable::~VideoResourceTable() {}
/* @0x16ac4 = _ZN3mrc18VideoResourceTableD0Ev */

/* @0x16ad8: TV's default-video marker bypasses resolution/frame-rate checks. */
const ResourceListOptions *VideoResourceTable::lookup(std::string codec, int width, int height, int frameRate) const
{
    auto found = resourceTable.find(codec);
    if (found == resourceTable.end()) {
        found = resourceTable.find("default");
        if (found == resourceTable.end())
            return nullptr;
    }
    for (const auto &entry : found->second) {
        const auto &info = entry.first;
        if (info.isDefault || (width * height <= info.width * info.height && frameRate <= info.frameRate))
            return &entry.second;
    }
    return nullptr;
}

/* @0x16b70: either nondecreasing dimension replaces the previous selected row. */
bool VideoResourceTable::getMaxResolution(std::string codec, int *width, int *height, int *frameRate)
{
    auto found = resourceTable.find(codec);
    if (found == resourceTable.end()) {
        found = resourceTable.find("default");
        if (found == resourceTable.end())
            return false;
    }
    int maxWidth = 0, maxHeight = 0, maxRate = 0;
    for (const auto &entry : found->second) {
        const auto &info = entry.first;
        if (maxWidth <= info.width || maxHeight <= info.height) {
            maxWidth = info.width;
            maxHeight = info.height;
            maxRate = info.frameRate;
        }
    }
    if (!maxWidth || !maxHeight)
        return false;
    *width = maxWidth;
    *height = maxHeight;
    *frameRate = maxRate;
    return true;
}

/* @0x16c14: first fitting row wins, not the highest rate among all rows. */
int VideoResourceTable::getMaxFrameRate(std::string codec, int width, int height)
{
    auto found = resourceTable.find(codec);
    if (found == resourceTable.end()) {
        found = resourceTable.find("default");
        if (found == resourceTable.end())
            return 0;
    }
    for (const auto &entry : found->second) {
        const auto &info = entry.first;
        if (info.isDefault || (width * height <= info.width * info.height && info.frameRate >= 0))
            return info.frameRate;
    }
    return 0;
}

/* @0x1564c: TV adds AV1, WMV, H266 and H263 beyond the public codec list. */
std::string ResourceCalculator::videoCodecToString(VideoCodec codec) const
{
    switch (codec) {
    case kVideoH264: return "H264";
    case kVideoH265: return "H265";
    case kVideoMPEG: return "MPEG";
    case kVideoMVC: return "MVC";
    case kVideoSVC: return "SVC";
    case kVideoVP9: return "VP9";
    case kVideoRM: return "RM";
    case kVideoAVS: return "AVS";
    case kVideoVP8: return "VP8";
    case kVideoMJPEG: return "MJPEG";
    case kVideoMPEG4: return "MPEG4";
    case kVideoJPEG: return "JPEG";
    case kVideoAV1: return "AV1";
    case kVideoWMV: return "WMV";
    case kVideoH266: return "H266";
    case kVideoH263: return "H263";
    default: return "default";
    }
}

/* @0x157fc: only the least-significant selected codec is queried. */
bool ResourceCalculator::getMaxVideoResolution(VideoCodecs codecs, int *width, int *height, int *frameRate)
{
    for (int i = 0; codecs >> i != 0; ++i) {
        int codec = 1 << i;
        if (codecs & codec)
            return vdecTable.getMaxResolution(videoCodecToString(static_cast<VideoCodec>(codec)), width, height, frameRate);
    }
    return false;
}

/* @0x1586c: only the least-significant selected codec is queried. */
int ResourceCalculator::getMaxFrameRate(VideoCodecs codecs, int width, int height)
{
    for (int i = 0; codecs >> i != 0; ++i) {
        int codec = 1 << i;
        if (codecs & codec)
            return vdecTable.getMaxFrameRate(videoCodecToString(static_cast<VideoCodec>(codec)), width, height);
    }
    return 0;
}

/* @0x158d8: TV extends upstream audio lookup with MAT, MAX and IAMF. */
ResourceList ResourceCalculator::calcAdecResources(AudioCodecs codecs, int version, int channel)
{
    ResourceList adec;
    if ((codecs & kAudioMPEG) && channel == 6 && (version == 4 || version == 2))
        concatResourceList(&adec, adecTable.lookup("aac6"));
    if (codecs & kAudioPCM)
        concatResourceList(&adec, adecTable.lookup("pcm"));
    if (codecs & kAudioDTS)
        concatResourceList(&adec, adecTable.lookup("dts"));
    if (codecs & kAudioDTSE)
        concatResourceList(&adec, adecTable.lookup("dtse"));
    if (codecs & kAudioMPEGH)
        concatResourceList(&adec, adecTable.lookup("mpeg-h"));
    if (codecs & kAudioAC4)
        concatResourceList(&adec, adecTable.lookup("ac4"));
    if (codecs & kAudioATMOS)
        concatResourceList(&adec, adecTable.lookup("atmos"));
    if (codecs & kAudioDescription)
        concatResourceList(&adec, adecTable.lookup("description"));
    if (codecs & kAudioMAT)
        concatResourceList(&adec, adecTable.lookup("mat"));
    if (codecs & kAudioMAX)
        concatResourceList(&adec, adecTable.lookup("max"));
    if (codecs & kAudioIAMF)
        concatResourceList(&adec, adecTable.lookup("iamf"));
    if (adec.empty() || (codecs & kAudioEtc))
        concatResourceList(&adec, adecTable.lookup("default"));
    return adec;
}

/* @0x15cbc */
ResourceList ResourceCalculator::calcVdecResources(VideoCodecs codecs, int width, int height, int frameRate, ScanType scan, _3DType mode)
{
    ResourceList result;
    ResourceList notSupported{{"NOTSUPPORTED", 1}};
    if (scan == kScanInterlaced)
        frameRate /= 2;
    if (mode != k3DNone)
        frameRate *= 2;
    for (int i = 0; codecs >> i != 0; ++i) {
        int codec = 1 << i;
        if (!(codecs & codec))
            continue;
        const auto *options = vdecTable.lookup(videoCodecToString(static_cast<VideoCodec>(codec)), width, height, frameRate);
        if (!options)
            return notSupported;
        concatResourceList(&result, &*options->begin());
    }
    return result;
}

/* @0x15e48: unlike upstream's quantity 2, the TV reserves one SVP_CPB. */
ResourceList ResourceCalculator::calcMiscResources(bool useSecureVideoPath, bool useSecureReformatter)
{
    ResourceList misc;
    if (useSecureVideoPath)
        misc.push_back(Resource("SVP_CPB", 1));
    return misc;
}

/* @0x15e98 */
ResourceListOptions ResourceCalculator::calcVdecResourceOptions(VideoCodecs codecs, int width, int height, int frameRate, ScanType scan, _3DType mode)
{
    ResourceListOptions result;
    ResourceList notSupported{{"NOTSUPPORTED", 1}};
    if (scan == kScanInterlaced)
        frameRate /= 2;
    if (mode != k3DNone)
        frameRate *= 2;
    for (int i = 0; codecs >> i != 0; ++i) {
        int codec = 1 << i;
        if (!(codecs & codec))
            continue;
        const auto *options = vdecTable.lookup(videoCodecToString(static_cast<VideoCodec>(codec)), width, height, frameRate);
        if (!options) {
            result.clear();
            result.push_back(notSupported);
            return result;
        }
        concatResourceListOptions(&result, options);
    }
    return result;
}

/* @0x16150: calls the still-unrecovered TV platform-table constructor. */
ResourceCalculator *ResourceCalculator::create()
{
    return new ResourceCalculator();
}

/* @0x16170 = _ZN3mrc18ResourceCalculatorD1Ev */
ResourceCalculator::~ResourceCalculator() {}
/* @0x16198 = _ZN3mrc18ResourceCalculatorD0Ev */
}
