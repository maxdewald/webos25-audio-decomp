/*
 * libgstlgaudiosink: renderer open / bypass head creation on /dev/lg/aren<N>.
 * RECONSTRUCTION from usr/lib/gstreamer-1.0/libgstlgaudiosink.so
 *   sha256 90dba3ef32704ce4096bf1e6045d2879d0c2192d507bc04d1e8e98437f420a43
 * Kernel side: Image.lz4-5.4.268-329.ptl4tv.5
 *   sha256 2b835f62a57891f72f715cfa0293f51113aeaa8c90838ba5b5d400525a8b68cd
 * Sink addresses are raw ELF VAs; kernel addresses are decompressed Image offsets.
 * Evidence: build/scratch/evidence/renderer-abi/
 */
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "../include/lg_audio_policy.h"

struct lgao {
    int  fd;
    int  index;          /* sink "index" property / resource-info audio-port */
    bool is_mixer;       /* audio-port == -1 with mixer-port 0..6 */
    bool drop_renderer;  /* caps BYPASS != 0 */
    bool es;             /* caps es=true: metadata only (ao+0x5c), no constructor */
    struct aren_pcm_info pcm;
    unsigned uid, tail_uid, bypass_uid;
};

/* @0x14924 lgao_open_renderer */
static int lgao_open_renderer(struct lgao *ao)
{
    ao->fd = open("/dev/lg/aren0", O_RDWR);
    int idx = ao->index + (ao->is_mixer ? 4 : 0);
    ioctl(ao->fd, AREN_SET_INDEX, &idx);   /* @0x14956, return ignored; kernel accepts 0..9 */
    return ao->fd;
}

/* @0x16fc4 lgao_config: PCM setters only cache; serialised here unless already PLAY. */
static void lgao_config(struct lgao *ao)
{
    ioctl(ao->fd, AREN_SET_PCM_INFO, &ao->pcm);           /* @0x1702e */
}

/* @0x1441c lgao_connect_renderer */
static void lgao_connect_renderer(struct lgao *ao, int msgq_id)
{
    ioctl(ao->fd, AREN_ADD_QUEUE);
    ioctl(ao->fd, AREN_GET_UID, &ao->uid);
    ioctl(ao->fd, AREN_GET_TAIL_UID, &ao->tail_uid);
    /* @0x14728: kernel aren_get_uid(index, bypass=true) @0xc35ab0
     *   pcm_bypass || pcm_multi_ch ? PCM getter @0xc251d0 (rts->srcbin->lipsync)
     *                              : ES  getter @0xc25340 (queue->lipsync)
     * ES getter logs "bypass is not yet created" and then immediately calls
     * renderer_create_bypass_pipeline(false) @0xc21894 -- that log is NOT a failure.
     * ES getter reuses ANY existing bypass (even a PCM one); PCM getter only a PCM one. */
    ioctl(ao->fd, AREN_GET_BYPASS_UID, &ao->bypass_uid);

    struct aren_msgpid_to_signal n = { msgq_id, getpid() };
    /* @0x147d4: shipped kernel dispatch @0xc30494 has no nr 43 -> -ENOTTY, ignored. */
    ioctl(ao->fd, AREN_SET_MSGPID_TO_SIGNAL, &n);
}

/* change_state READY->PAUSED; first events reuse already-connected appsrcs. */
int lgao_start(struct lgao *ao, int msgq_id)
{
    if (ao->drop_renderer || ao->index < 0)
        return -1;
    lgao_open_renderer(ao);
    lgao_config(ao);
    lgao_connect_renderer(ao, msgq_id);
    return 0;
}
