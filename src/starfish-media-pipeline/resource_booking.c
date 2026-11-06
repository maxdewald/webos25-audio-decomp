/*
 * Media pipeline resource booking (ADEC / PCMMC) and the audio input info it reports.
 * RECONSTRUCTION from:
 *   usr/sbin/starfish-media-pipeline       e9ecb5e99ee05058235a03051a92e74f65409882bd6241f074e56770b8fd9c65
 *   usr/lib/libresource_mgr.so.1.0.0       13b7efc096ca4544eb251606ea858f5ef1c0501094a5784cfce6dd6bff44b034
 * ADEC cost per stream: ../libmedia-resource-calculator/calc_adec.c
 *   etc/umediaserver/umediaserver_resource_config.txt (ADEC qty 2, ADEC_BANDWIDTH 6, PCMMC 5)
 * Addresses: Ghidra (= raw ELF VA + 0x10000). Evidence: build/scratch/evidence/resource-booking/
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
                      struct resource_req *requests, int request_count)
{
    if (!isPlatformSupportSoftwareAudioDecoder(soc) || (!use_sw_audio && !use_pcmmc))
        return;
    for (int index = 0; index < request_count; index++) {
        if (strcmp(requests[index].type, "ADEC") == 0)
            strcpy(requests[index].type, "PCMMC");        /* log "convert ADEC to PCMMC" */
        if (strcmp(requests[index].type, "ADEC_BANDWIDTH") == 0)
            requests[index].qty = 0;                       /* log "remove ADEC_BANDWIDTH" */
    }
}

struct acquired { const char *resource; int index; };

/* @0x992dc: from the uMediaServer acquire response, the lowest "index" of all entries
 * whose "resource" equals the wanted type (e.g. "ADEC"); -1 if none.
 * acquireResources stores the lowest ADEC as adecPort (+0x16c) and the lowest PCMMC as
 * mixerPort (+0x170). allocateResource @0x51410 and every libpf setResource (e.g.
 * AirPlayer @0xa3ee8) pass them unchanged as acquired-resource audio-port/mixer-port.
 * The sink's lgao_set_port then uses audio-port as renderer index, or the mixer path
 * only when it is -1; a second booked ADEC is never forwarded. */
int lowest_resource_index(const struct acquired *resources, int resource_count, const char *wanted)
{
    int lowest_index = -1;
    for (int index = 0; index < resource_count; index++)
        if (strcmp(resources[index].resource, wanted) == 0 && (lowest_index == -1 || resources[index].index <= lowest_index))
            lowest_index = resources[index].index;
    return lowest_index;
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
 *                 otherwise the stored codec string, with nonempty codec-name override.
 * Raw Thumb 0x48e88..0x48e9e proves find("dts")'s result is discarded; no subtype
 * normalization occurs. See metadata.cpp for the recovered builder and host tests.
 * Whether a particular player supplies dts_hd_ma is unknown; no custom hdbitstream
 * artifact or runtime caps capture was available.
 */
