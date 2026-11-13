#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define info(...) ((void) 0)
#define noti(...) ((void) 0)
#define error(...) ((void) 0)
struct apipeline;
struct aud_block_info { const char *block_type; bool isOpened; struct apipeline *pipeline; };
struct list_head { struct list_head *next, *prev; };
enum audio_decoder_codec { AUDIO_DECODER_CODEC_PCM = 1, AUDIO_DECODER_CODEC_AC3,
    AUDIO_DECODER_CODEC_EAC3, AUDIO_DECODER_CODEC_AC4, AUDIO_DECODER_CODEC_MAT,
    AUDIO_DECODER_CODEC_TRUE_HD, AUDIO_DECODER_CODEC_AAC };
// @match lg-opensource/kdriver/core/aud/input.h (?s)enum input_type\n\{.*?\};
// @match lg-opensource/kdriver/core/aud/input.h (?s)enum hdmi_audio_codec\n\{.*?\};
enum hdmi_codec { HDMI_CODEC_PCM, HDMI_CODEC_AC3, HDMI_CODEC_EAC3, HDMI_CODEC_AAC,
    HDMI_CODEC_MAT, HDMI_CODEC_DTS, HDMI_CODEC_DTS_HD };
enum HDMI_INPUT_MODE { HDMI_MODE_PCM, HDMI_MODE_BITSTREAM, HDMI_MODE_IEC60958_ES,
    HDMI_MODE_DTS_CD, HDMI_MODE_BITSTREAM_HBR, HDMI_MODE_BITSTREAM_HBR_HRA,
    HDMI_MODE_BYTE0_PC_VALID, HDMI_MODE_BYTE0_PC_VALID_HBR, HDMI_MODE_BYTE0_PC_VALID_HBR_HRA };
typedef int LX_HDMI_LINK_NUMBER_T;
typedef int LX_HDMI_AUDIO_TYPE_T;
typedef int LX_HDMI_SAMPLING_FREQ_T;
typedef int LX_HDMI_AUDIO_STREAM_T;
typedef int LX_HDMI_MUTE_MODE_T;
typedef unsigned int UINT32;
typedef unsigned int BOOLEAN;
enum { LX_HDMI_AUDIO_MUTE = 2 };
// @match lg-opensource/kdriver/include/hdmi_kapi.h (?s)typedef struct\n\{[^{}]*\} LX_HDMI_AUDIO_INFO_T;
// @match lg-opensource/kdriver/include/hdmi_kapi.h (?s)typedef struct\n\{[^{}]*\} LX_HDMI_MUTE_CTRL_T;
// @match src/kernel/aud/input.c (?s)struct aud_input_info\n\{.*?\};

_Static_assert(sizeof(struct aud_block_info) == 0x18, "parent size");
_Static_assert(offsetof(struct aud_input_info, uid) == 0x44, "uid offset");
_Static_assert(offsetof(struct aud_input_info, hdmi_codec) == 0x5c, "codec offset");
_Static_assert(offsetof(struct aud_input_info, drop) == 0x68, "drop offset");
_Static_assert(sizeof(LX_HDMI_AUDIO_INFO_T) == 0x28, "C5 5.4 HDMI info size");
_Static_assert(sizeof(LX_HDMI_MUTE_CTRL_T) == 0x10, "HDMI mute size");

struct property { int uid; char name[32]; int value; };
static struct property properties[16];
static size_t property_count;
static LX_HDMI_AUDIO_INFO_T audio;
static const char *buffer_name;
static int clock_updates, disabled_index, aad_starts;
static bool aad_mode;

static int aimc_set_property(int uid, const char *fmt, ...)
{
    assert(property_count < sizeof(properties) / sizeof(properties[0]));
    struct property *property = &properties[property_count++];
    property->uid = uid;
    snprintf(property->name, sizeof(property->name), "%s", fmt);
    va_list args;
    va_start(args, fmt);
    property->value = va_arg(args, int);
    va_end(args);
    return -1;
}
static int value(const char *name, int uid)
{
    for (size_t index = 0; index < property_count; index++)
        if (properties[index].uid == uid && strcmp(properties[index].name, name) == 0)
            return properties[index].value;
    return -1;
}
static int KDRV_HDMI21_HAL_Rx_Set_AudioMute(unsigned char port, LX_HDMI_MUTE_CTRL_T *arg, int size)
{
    assert(port == 1 && arg->linkNumber == 1 && size == sizeof(*arg));
    assert(!arg->bVideoMute && !arg->bAudioMute && arg->eMode == LX_HDMI_AUDIO_MUTE);
    return 0;
}
static int KDRV_HDMI21_HAL_Rx_GetAudioInfo(unsigned char port, LX_HDMI_AUDIO_INFO_T *arg, int size)
{
    assert(port == 1 && arg->linkNumber == 1 && size == sizeof(*arg));
    *arg = audio;
    return 0;
}
static int aud_hal_bc_buffer_init(const char *name) { buffer_name = name; return 0; }
static int input_set_clock(struct aud_block_info *block) { clock_updates++; return -1; }
static int aud_clock_disable_input(unsigned int index) { disabled_index = index; return -1; }
static int AUDIO_RestartAad(bool set_audio_mode) { aad_starts++; aad_mode = set_audio_mode; return 0; }
// @extract src/kernel/aud/input.c input_set_hdmi input_start input_stop input_drop_data input_is_drop_data

int main(void)
{
    struct aud_input_info input = { .input = INPUT_HDMI, .index = 1, .uid = 10,
        .uid_queue = 11, .uid_queue_bypass = 12 };
    struct aud_block_info *block = &input.parent;
    const int dts_types[] = { HDMI_AUDIO_DTS, HDMI_AUDIO_DTS_HD, HDMI_AUDIO_DTS_HD_HRA,
        HDMI_AUDIO_UNKNOWN_DTS, HDMI_AUDIO_UNKNOWN_DTS_HD, HDMI_AUDIO_UNKNOWN_DTS_HD_HRA };
    audio.samplingFreq = 192000;
    audio.countOfChannel = 8;
    for (size_t index = 0; index < sizeof(dts_types) / sizeof(dts_types[0]); index++)
    {
        property_count = 0;
        audio.audioType = dts_types[index];
        assert(input_set_hdmi(block, false) == 0);
        assert(input.hdmi_codec == AUDIO_DECODER_CODEC_PCM);
        assert(value("codec=%d", 10) == HDMI_CODEC_PCM);
        assert(value("mode=%d", 10) == HDMI_MODE_BITSTREAM);
        assert(value("max-length=%d", 11) == -1);
    }
    property_count = 0;
    assert(input_set_hdmi(block, true) == 0);
    assert(input.forced_pcm && input.channel_num == 8);
    assert(value("mode=%d", 10) == HDMI_MODE_PCM);
    assert(value("max-length=%d", 11) == 0x20000);
    assert(value("max-length=%d", 12) == 0x20000);

    property_count = 0;
    audio.audioType = HDMI_AUDIO_UNKNOWN_MAT;
    assert(input_set_hdmi(block, false) == 0);
    assert(value("mode=%d", 10) == HDMI_MODE_BYTE0_PC_VALID_HBR);
    assert(value("codec=%d", 10) == HDMI_CODEC_MAT);
    assert(input.hdmi_codec == AUDIO_DECODER_CODEC_MAT);

    property_count = 0;
    audio.audioType = HDMI_AUDIO_EAC3_IEC60958;
    assert(input_set_hdmi(block, false) == 0);
    assert(value("mode=%d", 10) == HDMI_MODE_BYTE0_PC_VALID);
    assert(value("codec=%d", 10) == HDMI_CODEC_PCM);

    input.hdmi_codec = AUDIO_DECODER_CODEC_PCM;
    input.org_hdmi_freq = 31999;
    property_count = 0;
    assert(input_start(block) == -1 && !input.is_start);
    assert(!clock_updates && property_count == 0 && !buffer_name);
    input.org_hdmi_freq = 48000;
    assert(input_start(block) == 0 && input.is_start);
    assert(clock_updates == 1 && strcmp(buffer_name, "bc-sai0") == 0);
    assert(value("start=%d", 10) == 1);

    property_count = 0;
    assert(input_drop_data(block, true) == 0 && input_is_drop_data(block));
    assert(property_count == 2);
    assert(value("drop=%d", 10) == 1 && value("drop-output-data=%d", 10) == 1);
    property_count = 0;
    assert(input_stop(block) == 0 && !input.is_start && input.drop);
    assert(disabled_index == 0 && property_count == 3);
    assert(value("start=%d", 10) == 0);
    assert(value("set-flush=%d", 11) == 1 && value("set-flush=%d", 12) == 1);
    property_count = 0;
    assert(input_drop_data(block, false) == 0 && !input_is_drop_data(block));
    assert(value("drop=%d", 10) == 0 && value("drop-output-data=%d", 10) == 0);

    input.input = INPUT_SIF;
    property_count = 0;
    assert(input_start(block) == 0 && input.is_start);
    assert(aad_starts == 1 && !aad_mode && property_count == 0);
    puts("PASS: C5 HDMI mapping, argument layouts, input start/stop and drop state");
    return 0;
}