// SPDX-License-Identifier: Apache-2.0
/* Copyright (c) 2008-2021 LG Electronics, Inc.
 * Licensed under the Apache License, Version 2.0; see the upstream notices in
 * lg-opensource/public/media-resource-calculator/.
 * Source-assisted TV declarations: unlike OSE, only ADEC and VDEC tables exist.
 * JSON parsing and platform table initialization are not recovered here.
 */
#ifndef TV_RESOURCE_CALCULATOR_H
#define TV_RESOURCE_CALCULATOR_H
#include "resource.h"
#include <list>

namespace mrc {
class ResourceTable {
public:
    ResourceTable();
    virtual ~ResourceTable();
    bool setData(const std::string &data);
    const ResourceList *lookup(const std::string &key) const;
private:
    std::map<std::string, ResourceList> resourceData;
};

struct VideoInfo {
    int width;
    int height;
    int frameRate;
    bool isDefault;
};
using ResourcePair = std::pair<VideoInfo, ResourceListOptions>;
using CodecTable = std::list<ResourcePair>;

class VideoResourceTable {
public:
    VideoResourceTable();
    virtual ~VideoResourceTable();
    bool setData(const std::string &data);
    const ResourceListOptions *lookup(std::string codec, int width, int height, int frameRate) const;
    bool getMaxResolution(std::string codec, int *width, int *height, int *frameRate);
    int getMaxFrameRate(std::string codec, int width, int height);
private:
    std::map<std::string, CodecTable> resourceTable;
};

class ResourceCalculator {
public:
    static ResourceCalculator *create();
    virtual ~ResourceCalculator();
    enum VideoCodec {
        kVideoEtc = 1, kVideoH264 = 2, kVideoH265 = 4, kVideoMPEG = 8,
        kVideoMVC = 16, kVideoSVC = 32, kVideoVP9 = 64, kVideoRM = 128,
        kVideoAVS = 256, kVideoVP8 = 512, kVideoMJPEG = 1024,
        kVideoMPEG4 = 2048, kVideoJPEG = 4096, kVideoAV1 = 8192,
        kVideoWMV = 16384, kVideoH266 = 32768, kVideoH263 = 65536
    };
    using VideoCodecs = int;
    enum ScanType { kScanProgressive, kScanInterlaced };
    enum _3DType { k3DNone, k3DSequential, k3DMultiStream };
    virtual ResourceList calcVdecResources(VideoCodecs codecs, int width, int height,
                                         int frameRate, ScanType scan, _3DType mode);
    virtual ResourceListOptions calcVdecResourceOptions(VideoCodecs codecs, int width, int height,
                                                       int frameRate, ScanType scan, _3DType mode);
    enum AudioCodec {
        kAudioEtc = 1, kAudioMPEG = 2, kAudioPCM = 4, kAudioDTS = 8,
        kAudioDTSE = 16, kAudioMPEGH = 32, kAudioAC4 = 64,
        kAudioATMOS = 128, kAudioDescription = 256, kAudioMAT = 512,
        kAudioMAX = 1024, kAudioIAMF = 2048
    };
    using AudioCodecs = int;
    virtual ResourceList calcAdecResources(AudioCodecs codecs, int version, int channel);
    virtual ResourceList calcMiscResources(bool useSecureVideoPath, bool useSecureReformatter);
    virtual bool getMaxVideoResolution(VideoCodecs codecs, int *width, int *height, int *frameRate);
    virtual int getMaxFrameRate(VideoCodecs codecs, int width, int height);
protected:
    ResourceCalculator();
    std::string videoCodecToString(VideoCodec codec) const;
    ResourceTable adecTable;
    VideoResourceTable vdecTable;
};
}
#endif
