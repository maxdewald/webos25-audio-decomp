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

// @match src/libgstlgaudiosink/lgao.c typedef enum \{\n    AUD_GAIN_DURATION_MODE_LINEAR[^}]*\} aud_gain_duration_mode_t;
// @match src/libgstlgaudiosink/lgao.c typedef struct \{\n    unsigned int volume;[^}]*\} aren_ease_volume_t;
// @match src/libgstlgaudiosink/lgao.c (?m)^#define (Q23_UNITY|UNSET|AREN_SET_EASE_VOLUME) .*$
// @match src/libgstlgaudiosink/lgao.c typedef struct \{ unsigned int clock_base_time, stream_base_time; \} aren_base_time_t;
// @match src/libgstlgaudiosink/lgao.c typedef struct \{\n    unsigned int lower_bound[^}]*\} aren_lipsync_param_t;
// @match src/libgstlgaudiosink/lgao.c (?m)^#define AREN_(GET_GSTC|SET_CLOCKTYPE|SET_ENABLE_LIPSYNC|SET_BASETIME|SET_LIPSYNC_PARAM|SET_SPEED) .*$
// @match src/libgstlgaudiosink/lgao.c (?m)^#define (RTS_MS|AREN_SET_RTS_PARAM).*\n

enum { AUD_EVENT_TYPE_LAST_AU = 2, AUD_EVENT_TYPE_CUSTOM = 13 };
enum appsink_event_return { appsink_event_handled = 0, appsink_event_not_handled = 1 };
struct aud_event { int type; const char *eos; };

static unsigned long reqs[16];
static unsigned vals[16][2];
static int nreq, msgs, last_msg, gstc = 1000000;
static int current_volume;
static aren_ease_volume_t fade_values[16];
static aren_rts_param_t rts_value;

static int _lgao_get_ease_volume(lgao *ao) { return current_volume; }

static int mock_ioctl(int fd, unsigned long req, void *arg)
{
    assert(nreq < 16);
    reqs[nreq] = req;
    if (req == AREN_SET_RTS_PARAM)
        rts_value = *(aren_rts_param_t *)arg;
    else if (req == AREN_SET_EASE_VOLUME)
        fade_values[nreq] = *(aren_ease_volume_t *)arg;
    else if (req == AREN_GET_GSTC)
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

// @extract src/libgstlgaudiosink/lgao.c sns_to_pts _lgao_set_basetime lgao_set_basetime _lgao_set_playspeed _lgao_set_lipsync_param lgao_play lgao_appsink_handle_event _lgao_convert_ease_type lgao_keep_fade _lgao_config_set_easevol _lgao_config_set_rts_param

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

    for (int mode = LGAO_APPTYPE_UNKNOWN; mode < LGAO_APPTYPE_MAX; ++mode) {
        ao = ctx();
        memset(&ao.rts_params, 0xff, sizeof(ao.rts_params));
        _lgao_config_set_rts_param(&ao, mode);
        assert(nreq == 1 && reqs[0] == AREN_SET_RTS_PARAM);
        if (mode == LGAO_APPTYPE_DIRECTMEDIA || mode == LGAO_APPTYPE_RTC ||
            mode == LGAO_APPTYPE_MIRACAST || mode == LGAO_APPTYPE_AIRPLAY ||
            mode == LGAO_APPTYPE_R2R) {
            assert(rts_value.onoff && rts_value.opmode == 2);
            assert(rts_value.normal_freq == 48000 && rts_value.slow_freq == 48000);
            assert(rts_value.fast_freq == 47250);
            assert(rts_value.mix_fade_len == 8 && rts_value.mix_wait_len == 2048);
            assert(!rts_value.lower_threshold);
            if (mode == LGAO_APPTYPE_DIRECTMEDIA) {
                assert(rts_value.upper_threshold == 80 * 384);
                assert(rts_value.recover_upper == 60 * 384);
                assert(rts_value.recover_lower == 70 * 384);
                assert(rts_value.skipping_threshold == 400 * 384);
                assert(rts_value.recover_skipping == 60 * 384);
            } else if (mode == LGAO_APPTYPE_AIRPLAY) {
                assert(rts_value.upper_threshold == 15 * 384);
                assert(rts_value.recover_upper == 5 * 384);
                assert(rts_value.recover_lower == 150 * 384);
                assert(rts_value.skipping_threshold == 200 * 384);
                assert(rts_value.recover_skipping == 5 * 384);
            } else if (mode == LGAO_APPTYPE_R2R) {
                assert(rts_value.upper_threshold == 400 * 384);
                assert(rts_value.recover_upper == 100 * 384);
                assert(rts_value.recover_lower == 300 * 384);
                assert(rts_value.skipping_threshold == 700 * 384);
                assert(rts_value.recover_skipping == 100 * 384);
                assert(rts_value.buffering_level == 200);
            } else {
                assert(rts_value.upper_threshold == 200 * 384);
                assert(rts_value.recover_upper == 100 * 384);
                assert(rts_value.recover_lower == 150 * 384);
                assert(rts_value.skipping_threshold == 400 * 384);
                assert(rts_value.recover_skipping == 100 * 384);
            }
        } else {
            const aren_rts_param_t zero = {0};
            assert(!memcmp(&rts_value, &zero, sizeof(zero)));
        }
    }
    for (int mode = LGAO_APPTYPE_DIRECTMEDIA; mode <= LGAO_APPTYPE_R2R; ++mode) {
        if (mode > LGAO_APPTYPE_AIRPLAY && mode != LGAO_APPTYPE_R2R) continue;
        ao = ctx();
        ao.rts_params = (aren_rts_param_t){
            .opmode = 7, .upper_threshold = 11, .lower_threshold = 12,
            .recover_upper = 13, .recover_lower = 14, .skipping_threshold = 15,
            .recover_skipping = 16, .normal_freq = 48001, .slow_freq = 48002,
            .fast_freq = 47000, .mix_fade_len = 9, .mix_wait_len = 17,
            .buffering_level = 50,
        };
        _lgao_config_set_rts_param(&ao, mode);
        assert(rts_value.opmode == 7 && rts_value.normal_freq == 48001);
        assert(rts_value.slow_freq == 48002 && rts_value.fast_freq == 47000);
        assert(rts_value.mix_fade_len == 9 && rts_value.mix_wait_len == 17);
        if (mode == LGAO_APPTYPE_R2R) {
            assert(rts_value.upper_threshold == 250 * 384);
            assert(rts_value.recover_upper == 50u * 384 - 100u * 384);
            assert(rts_value.recover_lower == 150 * 384);
            assert(rts_value.skipping_threshold == 550 * 384);
            assert(rts_value.recover_skipping == rts_value.recover_upper);
            assert(rts_value.buffering_level == 50 && !rts_value.lower_threshold);
        } else {
            assert(rts_value.upper_threshold == 11 * 384);
            assert(rts_value.lower_threshold == 12 * 384);
            assert(rts_value.recover_upper == 13 * 384);
            assert(rts_value.recover_lower == 14 * 384);
            assert(rts_value.skipping_threshold == 15 * 384);
            assert(rts_value.recover_skipping == 16 * 384);
            assert(!rts_value.buffering_level);
        }
    }
    ao = ctx();
    _lgao_config_set_rts_param(&ao, LGAO_APPTYPE_RTC);
    assert(rts_value.onoff && !rts_value.opmode && !rts_value.fast_freq);
    assert(!rts_value.upper_threshold && !rts_value.mix_wait_len);
    ao = ctx();

    ao.targetVolume = 100; ao.duration = 1000; current_volume = 25;
    assert(lgao_keep_fade(&ao) == 0 && ao.currVolume == 25 && ao.remainDuration == 750);
    ao.fadeType = LGAO_EASE_INCUBIC;
    _lgao_config_set_easevol(&ao);
    assert(nreq == 2 && reqs[0] == AREN_SET_EASE_VOLUME && reqs[1] == AREN_SET_EASE_VOLUME);
    assert(fade_values[0].volume == 0x200000 && !fade_values[0].duration);
    assert(fade_values[0].type == AUD_GAIN_DURATION_MODE_LINEAR);
    assert(fade_values[1].volume == 0x800000 && fade_values[1].duration == 750);
    assert(fade_values[1].type == AUD_GAIN_DURATION_MODE_INCUBIC);
    ao = ctx(); ao.targetVolume = 50; ao.duration = 1000; current_volume = 0;
    assert(lgao_keep_fade(&ao) == 0 && ao.currVolume == 0 && ao.remainDuration == 500);
    current_volume = 75;
    assert(lgao_keep_fade(&ao) == 0 && ao.currVolume == 75 && ao.remainDuration == 500);
    current_volume = -1;
    assert(lgao_keep_fade(&ao) == 0 && ao.currVolume == UNSET && ao.remainDuration == UNSET);
    ao.targetVolume = UNSET; ao.currVolume = 17; ao.remainDuration = 23;
    assert(lgao_keep_fade(&ao) == 0 && ao.currVolume == 17 && ao.remainDuration == 23);
    assert(lgao_keep_fade(NULL) == -1);
    ao = ctx(); ao.targetVolume = 50; ao.duration = 1000;
    ao.currVolume = UNSET; ao.fadeType = LGAO_EASE_OUTCUBIC;
    _lgao_config_set_easevol(&ao);
    assert(nreq == 1 && fade_values[0].volume == 0x400000 && fade_values[0].duration == 1000);
    assert(fade_values[0].type == AUD_GAIN_DURATION_MODE_OUTCUBIC);
    ao = ctx(); ao.targetVolume = 50; ao.duration = 1000;
    ao.currVolume = 25; ao.remainDuration = 500; ao.fadeType = LGAO_EASE_NONE;
    _lgao_config_set_easevol(&ao);
    assert(nreq == 1 && fade_values[0].volume == 0x400000 && fade_values[0].duration == 500);
    assert(fade_values[0].type == AUD_GAIN_DURATION_MODE_LINEAR);
    ao = ctx(); ao.targetVolume = ao.duration = UNSET; ao.fadeType = LGAO_EASE_NONE;
    _lgao_config_set_easevol(&ao);
    assert(!nreq);
    ao = ctx();

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
