#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "libgstlgaudiosink/lgao.h"
#include "include/lg_audio_policy.h"

#define LOGE(...) ((void)0)
#define LOGN(...) ((void)0)
#define LOGI(...) ((void)0)
#define LOGD(...) ((void)0)

// @match src/libgstlgaudiosink/lgao.c typedef struct \{ unsigned int clock_base_time, stream_base_time; \} aren_base_time_t;
// @match src/libgstlgaudiosink/lgao.c typedef struct \{\n    unsigned int lower_bound[^}]*\} aren_lipsync_param_t;
// @match src/libgstlgaudiosink/lgao.c (?m)^#define AREN_(GET_GSTC|SET_CLOCKTYPE|SET_ENABLE_LIPSYNC|SET_BASETIME|SET_LIPSYNC_PARAM|SET_SPEED) .*$

enum { AUD_EVENT_TYPE_LAST_AU = 2, AUD_EVENT_TYPE_CUSTOM = 13 };
enum appsink_event_return { appsink_event_handled = 0, appsink_event_not_handled = 1 };
struct aud_event { int type; const char *eos; };

static unsigned long reqs[16];
static unsigned vals[16][2];
static int nreq, msgs, last_msg, gstc = 1000000;

static int mock_ioctl(int fd, unsigned long req, void *arg)
{
    assert(nreq < 16);
    reqs[nreq] = req;
    if (req == AREN_GET_GSTC)
        *(unsigned *)arg = gstc;
    else if (req == AREN_SET_ENABLE_LIPSYNC)
        vals[nreq][0] = *(bool *)arg;   /* AREN_IOC size 4, but lgao passes a 1-byte bool */
    else
        memcpy(vals[nreq], arg, req == AREN_SET_BASETIME ? 8 : 4);
    ++nreq;
    return 0;
}
static int mock_msgsnd(int mq, const void *msg, size_t size, int flags)
{
    const lgao_mq_item *m = msg;
    assert(mq == 42 && size == sizeof(m->event) && m->mtype == LGAO_MSG_TYPE_EVENT);
    last_msg = m->event;
    ++msgs;
    return 0;
}
static char *aud_event_str(struct aud_event *e) { return NULL; }
static const char *aud_event_name(struct aud_event *e) { return "ev"; }
static const char *aud_event_value_get_str(struct aud_event *e, const char *k, const char *def)
{
    assert(!strcmp(k, "eos"));
    return e->eos ? e->eos : def;
}
#define ioctl mock_ioctl
#define msgsnd mock_msgsnd

// @extract src/libgstlgaudiosink/lgao.c sns_to_pts _lgao_set_basetime lgao_set_basetime _lgao_set_playspeed _lgao_set_lipsync_param lgao_play lgao_appsink_handle_event

static lgao ctx(void)
{
    lgao ao = {0};
    ao.fd = 7; ao.index = 0; ao.playSpeed = 1000; ao.lipsyncOffset = 0xfffffff;
    ao.lipsyncMode = LGAO_LIPSYNC_AUTO; ao.clock_type = AUD_CLK_TYPE_GSTC;
    ao.audioStartInfo.codecType = LGAO_CODEC_UNKNOWN + 1;
    ao.callback_mq_id = 42;
    nreq = msgs = last_msg = 0;
    return ao;
}

int main(void)
{
    const long long s = 1000000000LL;
    lgao ao = ctx();

    /* basetime: 90 kHz, 28-bit; clock base = GSTC - running time, stream base = segment start */
    assert(lgao_set_basetime(&ao, 2 * s, 10 * s) == 0);
    assert(nreq == 2 && reqs[0] == AREN_GET_GSTC && reqs[1] == AREN_SET_BASETIME);
    assert(vals[1][0] == 1000000 - 180000 && vals[1][1] == 900000);
    gstc = 1000;
    ao = ctx();
    assert(lgao_set_basetime(&ao, 20 * s, 0) == 0);
    assert(vals[1][0] == ((1000u - 1800000u) & 0xfffffff) && vals[1][1] == 0);
    /* invalid segment start: nothing reaches the renderer, cached bases are kept */
    ao = ctx(); ao.clockBasetime = 5; ao.streamBasetime = 6;
    assert(lgao_set_basetime(&ao, 0, -1) == -1 && nreq == 0);
    assert(ao.clockBasetime == 5 && ao.streamBasetime == 6);

    /* play: speed -> clocktype -> lipsync param -> cached basetime -> lipsync on; no START */
    ao = ctx(); ao.clockBasetime = 111; ao.streamBasetime = 222;
    assert(lgao_play(&ao) == 0 && ao.rendererState == LGAO_STATE_PLAY);
    assert(nreq == 5);
    assert(reqs[0] == AREN_SET_SPEED && vals[0][0] == 1000);
    assert(reqs[1] == AREN_SET_CLOCKTYPE && vals[1][0] == 0x100);
    assert(reqs[2] == AREN_SET_LIPSYNC_PARAM);
    assert(reqs[3] == AREN_SET_BASETIME && vals[3][0] == 111 && vals[3][1] == 222);
    assert(reqs[4] == AREN_SET_ENABLE_LIPSYNC && (vals[4][0] & 0xff) == 1);
    /* never-programmed bases are sent as zero, not as -1 */
    ao = ctx();
    assert(lgao_play(&ao) == 0 && vals[3][0] == 0 && vals[3][1] == 0);
    /* mixer AUTO: lipsync off; vsp: basetime skipped */
    ao = ctx(); ao.isMixer = true; ao.vsp_mode = true;
    assert(lgao_play(&ao) == 0 && nreq == 4 && reqs[3] == AREN_SET_ENABLE_LIPSYNC);
    assert((vals[3][0] & 0xff) == 0);
    ao = ctx(); ao.audioStartInfo.codecType = LGAO_CODEC_UNKNOWN;
    assert(lgao_play(&ao) == -1 && nreq == 0 && ao.rendererState != LGAO_STATE_PLAY);

    /* PRESENT_END only for the custom eos=last marker; plain LAST and others are ignored */
    struct aud_event last = { AUD_EVENT_TYPE_LAST_AU, "last" };
    struct aud_event other = { AUD_EVENT_TYPE_CUSTOM, "no" };
    struct aud_event marker = { AUD_EVENT_TYPE_CUSTOM, "last" };
    ao = ctx();
    assert(lgao_appsink_handle_event(&ao, 0, &last) == appsink_event_not_handled && !msgs);
    assert(lgao_appsink_handle_event(&ao, 0, &other) == appsink_event_not_handled && !msgs);
    assert(lgao_appsink_handle_event(&ao, 0, &marker) == appsink_event_not_handled);
    assert(msgs == 1 && last_msg == LGAO_EVENT_PRESENT_END);

    puts("PASS lgao clock: basetime units/mask, play ioctl order, PRESENT_END trigger");
    return 0;
}
