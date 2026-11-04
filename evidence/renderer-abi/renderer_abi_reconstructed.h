#ifndef RENDERER_ABI_RECONSTRUCTED_H
#define RENDERER_ABI_RECONSTRUCTED_H

#include <stdint.h>

#define RABI_IOC(direction, number, size) \
    (((uint32_t)(direction) << 30) | ((uint32_t)(size) << 16) | \
     (UINT32_C(0x52) << 8) | (uint32_t)(number))

struct rabi_msgpid_to_signal {
    int32_t msgq_id;
    int32_t pid;
};

struct rabi_pcm_info {
    uint32_t sampling_rate;
    uint32_t channels;
    uint32_t bits_per_sample;
    uint32_t byte_align;
    uint32_t sign;
    uint32_t clock_x4;
    uint32_t pcm_bypass;
    uint32_t pcm_multi_ch;
};

#define RABI_SET_INDEX RABI_IOC(1, 1, 4)
#define RABI_GET_UID RABI_IOC(2, 2, 4)
#define RABI_GET_BYPASS_UID RABI_IOC(2, 3, 4)
#define RABI_SET_PCM_INFO RABI_IOC(1, 32, sizeof(struct rabi_pcm_info))
#define RABI_ADD_QUEUE RABI_IOC(0, 33, 0)
#define RABI_GET_TAIL_UID RABI_IOC(2, 34, 4)
#define RABI_SET_PROC_ID RABI_IOC(1, 40, 4)
#define RABI_SET_MSGPID_TO_SIGNAL RABI_IOC(1, 43, sizeof(struct rabi_msgpid_to_signal))

#endif