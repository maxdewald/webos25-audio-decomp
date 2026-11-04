/*
 * Media pipeline resource booking (ADEC / PCMMC) and the audio input info it reports.
 * RECONSTRUCTION from:
 *   usr/sbin/starfish-media-pipeline       e9ecb5e99ee05058235a03051a92e74f65409882bd6241f074e56770b8fd9c65
 *   usr/lib/libresource_mgr.so.1.0.0       13b7efc096ca4544eb251606ea858f5ef1c0501094a5784cfce6dd6bff44b034
 * ADEC cost per stream: ../libmedia-resource-calculator/calc_adec.c
 *   etc/umediaserver/umediaserver_resource_config.txt (ADEC qty 2, ADEC_BANDWIDTH 6, PCMMC 5)
 * Addresses: Ghidra (= raw ELF VA + 0x10000). Evidence: ../../evidence/resource-booking/
 *
 * The ADEC index booked here becomes the kernel's main_audio (decproxy only receives
 * the "acquired-resource" event with core-type/active/audio-port/mixer-port).
 */
#include <stdbool.h>
#include <string.h>

/* SoCs that may decode audio in software. The C2's SoC name string at runtime
 * was not captured; if it is not in this list the ADEC->PCMMC conversion never runs. */
static const char *const sw_audio_soc[] = {
    "m16p", "m16plite", "m16pstb", "m16pbno", "m16pp", "m16pplite", "m16ppez", "m16p3",
    "o18", "o18k", "o20", "o208k", "o20n", "e60", "e60n",
    "k3l", "k3lp", "k4lp", "k5lp", "k5ap", "k6lp", "k6hp", "k6ap",
};

/* @0x7fdf0 */
static bool isPlatformSupportSoftwareAudioDecoder(const char *soc)
{
    for (unsigned i = 0; i < sizeof(sw_audio_soc) / sizeof(sw_audio_soc[0]); i++)
        if (strcmp(soc, sw_audio_soc[i]) == 0)
            return true;
    return false;
}

struct resource_req { char type[16]; int qty; };

/* @0x9e2d4 smp::ResourceManager::acquireResources (same shape in acquireAdaptiveResources). */
void acquireResources(const char *soc, bool use_sw_audio, bool use_pcmmc,
                      struct resource_req *req, int n)
{
    if (!isPlatformSupportSoftwareAudioDecoder(soc) || (!use_sw_audio && !use_pcmmc))
        return;
    for (int i = 0; i < n; i++) {
        if (strcmp(req[i].type, "ADEC") == 0)
            strcpy(req[i].type, "PCMMC");        /* log "convert ADEC to PCMMC" */
        if (strcmp(req[i].type, "ADEC_BANDWIDTH") == 0)
            req[i].qty = 0;                       /* log "remove ADEC_BANDWIDTH" */
    }
}

struct acquired { const char *resource; int index; };

/* @0x992dc: from the uMediaServer acquire response, the lowest "index" of all entries
 * whose "resource" equals the wanted type (e.g. "ADEC"); -1 if none. */
int lowest_resource_index(const struct acquired *res, int n, const char *wanted)
{
    int best = -1;
    for (int i = 0; i < n; i++)
        if (strcmp(res[i].resource, wanted) == 0 && (best == -1 || res[i].index <= best))
            best = res[i].index;
    return best;
}

/*
 * @0x58630 smp::MediaApiPrivate::generateAudioDecodeInfo -> audiooutputd setQualityAttribute
 *   pipelineInfo: pipelineId, streamType, contentType, sourceType="MEDIA",
 *                 resource="ADEC<n>" / "AMIXER<n>", pipelinePurpose, sourceAttribute,
 *                 reset, gameInfo{isGameMode}, pipelinetype, videoType, audioDropEnable
 *   categoryInfo: codec, channel, immersive("atmos"|"none"), params
 *   codec rules:  no audio track or reset -> "none"
 *                 caps "x-raw"            -> "pcm"
 *                 "mpeg-h"                -> "mpeg_h"
 *                 otherwise the stored codec string; a find("dts") runs first but how
 *                 its result is used is UNVERIFIED (decompiler lost it).
 * Open question: whether audio/x-dts caps ever produce codec "dts_hd_ma". If not, the
 * direct DTS-HD MA pass-through row in libumi cannot match from this path.
 */
