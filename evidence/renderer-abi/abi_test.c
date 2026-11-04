#include <stddef.h>
#include "renderer_abi_reconstructed.h"

_Static_assert(sizeof(struct rabi_msgpid_to_signal) == 8, "notification size");
_Static_assert(offsetof(struct rabi_msgpid_to_signal, msgq_id) == 0, "queue offset");
_Static_assert(offsetof(struct rabi_msgpid_to_signal, pid) == 4, "pid offset");
_Static_assert(sizeof(struct rabi_pcm_info) == 32, "PCM payload size");
_Static_assert(offsetof(struct rabi_pcm_info, pcm_bypass) == 24, "PCM bypass offset");
_Static_assert(offsetof(struct rabi_pcm_info, pcm_multi_ch) == 28, "PCM multichannel offset");
_Static_assert(RABI_SET_INDEX == UINT32_C(0x40045201), "SET_INDEX encoding");
_Static_assert(RABI_GET_UID == UINT32_C(0x80045202), "GET_UID encoding");
_Static_assert(RABI_GET_BYPASS_UID == UINT32_C(0x80045203), "GET_BYPASS_UID encoding");
_Static_assert(RABI_SET_PCM_INFO == UINT32_C(0x40205220), "SET_PCM_INFO encoding");
_Static_assert(RABI_ADD_QUEUE == UINT32_C(0x5221), "ADD_QUEUE encoding");
_Static_assert(RABI_GET_TAIL_UID == UINT32_C(0x80045222), "GET_TAIL_UID encoding");
_Static_assert(RABI_SET_PROC_ID == UINT32_C(0x40045228), "SET_PROC_ID encoding");
_Static_assert(RABI_SET_MSGPID_TO_SIGNAL == UINT32_C(0x4008522b), "notification encoding");