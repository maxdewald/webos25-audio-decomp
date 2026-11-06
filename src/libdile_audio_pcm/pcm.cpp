/* All shipped exports return the 32-bit value 0xffffffff without forwarding.
 * No public prototypes were found locally. Ellipsis preserves opaque incoming
 * arguments without inventing their count, types, or meaning; these are ABI
 * entry points, not recovered API declarations. Signedness is also unknown.
 */

extern "C" {

/* @0x104f0: Returns failure without retaining or invoking a callback. */
int DILE_AUDIO_PCM_RegSendPCMCallback(...)
{
    return -1;
}

/* @0x104f8: Returns failure without starting an upload. */
int DILE_AUDIO_PCM_StartUpload(...)
{
    return -1;
}

/* @0x10500: Returns failure without changing upload state. */
int DILE_AUDIO_PCM_StopUpload(...)
{
    return -1;
}

/* @0x10508: Returns failure. The original reserves an unused 8-byte stack
 * frame, but its prototype and the source-level reason are unknown.
 * Only failure semantics are reconstructed; the frame is not reproduced.
 */
int DILE_AUDIO_PCM_SetVolume(...)
{
    return -1;
}

/* @0x10514: Returns failure without reading or applying a channel setting. */
int DILE_AUDIO_PCM_SetChannel(...)
{
    return -1;
}

}
