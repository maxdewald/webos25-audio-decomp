/*
 * Shared types for the reconstructed LG webOS 25 audio output policy
 * (OLED42C27LA, Rockhopper 10.3.1-3006, kernel 5.4.268-329.ptl4tv.5).
 *
 * RECONSTRUCTION, not LG source. Every value here was read from the shipped
 * binaries; see ../README.md for the evidence map and confidence of each fact.
 */
#ifndef LG_AUDIO_POLICY_H
#define LG_AUDIO_POLICY_H

#include <stdbool.h>
#include <stdint.h>

/* ---------------------------------------------------------------------------
 * eARC control word written by libumi to ALSA "Sndout EARC Outputtype":
 *   [st, ct, cn, sr, mo]   (libumi umiTVAudio::setEArcOutputType @0x41b64)
 * ------------------------------------------------------------------------- */
enum umi_sndout_output_codec {          /* the "ct" field */
    CT_PCM        = 1,
    CT_DD         = 2,                  /* ac3 */
    CT_AAC        = 3,
    CT_DTS        = 4,
    CT_DDP        = 5,                  /* eac3 */
    CT_TRUEHD     = 6,
    CT_MAT2PCM    = 7,
    CT_DTSHD      = 8,
    CT_MPEG4AACLC = 9,
    CT_DTSHD_MA   = 10,
    CT_DTSX_P1    = 11,                 /* also chosen for DTSX_P2 capability */
};

enum sndout_output_mix_option {         /* the "mo" field */
    MO_NONE = 0,                        /* main audio only -> kernel may bypass */
    MO_ALL  = 1,                        /* mix main + system sounds */
};

struct earc_output_type {
    int32_t st;                         /* always 2 from this writer */
    int32_t ct;                         /* enum umi_sndout_output_codec */
    int32_t cn;                         /* 0, or 2/6/8 for multichannel PCM */
    int32_t sr;                         /* always 0 from this writer */
    int32_t mo;                         /* enum sndout_output_mix_option */
};

/* ---------------------------------------------------------------------------
 * dsp1 output module codec ids (out_event first-event parser, dsp1 0x601238f4)
 * ------------------------------------------------------------------------- */
enum dsp1_out_codec {
    OUT_CODEC_PCM     = 0,
    OUT_CODEC_AC3     = 1,
    OUT_CODEC_EAC3    = 2,
    OUT_CODEC_AAC     = 3,
    OUT_CODEC_MAT     = 4,
    OUT_CODEC_DTS     = 5,
    OUT_CODEC_DTS_HD  = 6,
    OUT_CODEC_DTS_MA  = 7,              /* "dts_hd_ma" and "dts_ma" */
    OUT_CODEC_DTSX_P1 = 8,
    OUT_CODEC_DTSX_P2 = 9,
};

/* ---------------------------------------------------------------------------
 * /dev/lg/aren* renderer ioctls used by libgstlgaudiosink ('R' = 0x52).
 * Request numbers and sizes verified in the sink and the shipped kernel.
 * ------------------------------------------------------------------------- */
#define AREN_IOC(dir, nr, size) \
    (((uint32_t)(dir) << 30) | ((uint32_t)(size) << 16) | (UINT32_C(0x52) << 8) | (uint32_t)(nr))

struct aren_msgpid_to_signal {          /* DWARF name aren_msgpid_to_signal_t */
    int32_t msgq_id;
    int32_t pid;
};

struct aren_pcm_info {                  /* 32-byte SET_PCM_INFO payload */
    uint32_t sampling_rate;
    uint32_t channels;
    uint32_t bits_per_sample;
    uint32_t byte_align;
    uint32_t sign;
    uint32_t clock_x4;
    uint32_t pcm_bypass;
    uint32_t pcm_multi_ch;
};

#define AREN_SET_INDEX              AREN_IOC(1, 1, 4)
#define AREN_GET_UID                AREN_IOC(2, 2, 4)
#define AREN_GET_BYPASS_UID         AREN_IOC(2, 3, 4)
#define AREN_SET_PCM_INFO           AREN_IOC(1, 32, sizeof(struct aren_pcm_info))
#define AREN_ADD_QUEUE              AREN_IOC(0, 33, 0)
#define AREN_GET_TAIL_UID           AREN_IOC(2, 34, 4)
#define AREN_SET_PROC_ID            AREN_IOC(1, 40, 4)
/* 0x4008522b: sent by the sink, rejected by the shipped kernel with -ENOTTY. */
#define AREN_SET_MSGPID_TO_SIGNAL   AREN_IOC(1, 43, sizeof(struct aren_msgpid_to_signal))

#endif
