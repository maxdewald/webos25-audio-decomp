/* All shipped exports return the 32-bit value 0xffffffff without forwarding.
 * No public prototypes were found locally. Ellipsis preserves opaque incoming
 * arguments without inventing their count, types, or meaning; these are ABI
 * entry points, not recovered API declarations. Signedness is also unknown.
 */

extern "C" {

/* @0x105f8: Rejects every request without reading arguments or writing output. */
int DILE_AUDIO_DIRECT_Open(...)
{
    return -1;
}

/* @0x10600: Rejects every request. The original saves r0-r3 in a 16-byte
 * stack frame, but its prototype and the source-level reason are unknown.
 * Only failure semantics are reconstructed; the frame is not reproduced.
 */
int DILE_AUDIO_DIRECT_Start(...)
{
    return -1;
}

/* @0x10610: Rejects every request without touching caller memory. */
int DILE_AUDIO_DIRECT_Write(...)
{
    return -1;
}

/* @0x10618: Returns failure without changing playback state. */
int DILE_AUDIO_DIRECT_Stop(...)
{
    return -1;
}

/* @0x10620: Returns failure without releasing any resource. */
int DILE_AUDIO_DIRECT_Close(...)
{
    return -1;
}

/* @0x10628: Returns failure without changing playback state. */
int DILE_AUDIO_DIRECT_Pause(...)
{
    return -1;
}

/* @0x10630: Returns failure without changing playback state. */
int DILE_AUDIO_DIRECT_Resume(...)
{
    return -1;
}

/* @0x10638: Returns failure without flushing anything. */
int DILE_AUDIO_DIRECT_Flush(...)
{
    return -1;
}

/* @0x10640: Returns failure without reading or filling a buffer description. */
int DILE_AUDIO_DIRECT_GetBufferInfo(...)
{
    return -1;
}

/* @0x10648: Returns failure without reading or applying parameters. */
int DILE_AUDIO_DIRECT_SetNoDelayParam(...)
{
    return -1;
}

}
