#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef int gint;
typedef uint8_t guint8;
typedef int GstAudioFormat;
typedef int GstFlowReturn;
enum { GST_FLOW_OK, GST_FLOW_ERROR = -5, TRUE = 1, G_LITTLE_ENDIAN = 1234,
       G_TYPE_INT = 24, GST_LEVEL_INFO = 4 };
typedef struct { int refs, bits, rate, channels, dummy, bypass; } GstCaps;
typedef struct { int bits, rate, channels; } GstAudioInfo;
typedef struct { void *adec, *pSrcPad; } GstLgAudioDec;
static unsigned _gst_debug_min;
static GstCaps created, current;
static bool equal_caps, accept_event, fail_caps;
static int pushed, getter_error;
#define GST_CAT_DEFAULT 0
#define GST_PTR_FORMAT "p"
#define G_UNLIKELY(value) (value)
#define GST_ERROR(...) ((void)0)
#define GST_INFO(...) ((void)0)
#define GST_DEBUG(...) ((void)0)
static void gst_debug_log(int category, int level, const char *file, const char *function,
                          int line, void *object, const char *format, ...) {}
static int lgadec_get_es_bypass(void *adec) { return 1; }
static int lgadec_get_bits_per_sample(void *adec, guint8 *bits)
{
    *bits = 24;
    return getter_error;
}
static int lgadec_get_number_of_channel(void *adec, guint8 *channels)
{
    *channels = 6;
    return 0;
}
static int lgadec_get_freq(void *adec, gint *rate) { *rate = 48000; return 0; }
static GstAudioFormat gst_audio_format_build_integer(int sign, int endian, int width, int depth)
{
    assert(sign && endian == G_LITTLE_ENDIAN && width == depth);
    return width;
}
static void gst_audio_info_init(GstAudioInfo *info) { *info = (GstAudioInfo){0}; }
static void gst_audio_info_set_format(GstAudioInfo *info, GstAudioFormat format,
                                     int rate, int channels, void *positions)
{
    *info = (GstAudioInfo){format, rate, channels};
}
static GstCaps *gst_audio_info_to_caps(GstAudioInfo *info)
{
    created = (GstCaps){1, info->bits, info->rate, info->channels};
    return fail_caps ? NULL : &created;
}
static void gst_caps_set_simple(GstCaps *caps, const char *field, int type, int value, void *end)
{
    assert(type == G_TYPE_INT);
    if (field[0] == 'D') caps->dummy = value;
    else caps->bypass = value;
}
static GstCaps *gst_pad_get_current_caps(void *pad)
{
    if (!equal_caps) return NULL;
    current.refs++;
    return &current;
}
static bool gst_caps_is_equal(GstCaps *first, GstCaps *second) { return equal_caps; }
static GstCaps *gst_event_new_caps(GstCaps *caps) { caps->refs++; return caps; }
static void gst_caps_unref(GstCaps *caps) { assert(caps->refs > 0); caps->refs--; }
static bool gst_pad_push_event(void *pad, GstCaps *caps)
{
    pushed++;
    gst_caps_unref(caps);
    return accept_event;
}

// @extract src/libgstlgaudiodec/gstlgaudiodec.c _gst_lg_adec_set_caps_srcpad_dummy _gst_lg_adec_set_caps_srcpad_no_dec

int main(void)
{
    GstLgAudioDec decoder = {0};
    assert(_gst_lg_adec_set_caps_srcpad_dummy(NULL) == GST_FLOW_ERROR);
    assert(_gst_lg_adec_set_caps_srcpad_no_dec(NULL) == GST_FLOW_ERROR);
    for (int accepted = 0; accepted < 2; accepted++) {
        accept_event = accepted;
        pushed = 0;
        assert(_gst_lg_adec_set_caps_srcpad_dummy(&decoder) ==
               (accepted ? GST_FLOW_ERROR : GST_FLOW_OK));
        assert(pushed == 1 && created.refs == 0);
        assert(created.bits == 8 && created.rate == 8000 && created.channels == 1);
        assert(created.dummy == 1 && created.bypass == 1);
        pushed = 0;
        assert(_gst_lg_adec_set_caps_srcpad_no_dec(&decoder) ==
               (accepted ? GST_FLOW_OK : GST_FLOW_ERROR));
        assert(pushed == 1 && created.refs == 0);
        assert(created.bits == 24 && created.rate == 48000 && created.channels == 6);
    }
    equal_caps = true;
    current.refs = 1;
    pushed = 0;
    assert(_gst_lg_adec_set_caps_srcpad_no_dec(&decoder) == GST_FLOW_OK);
    assert(pushed == 0 && current.refs == 1 && created.refs == 0);
    getter_error = -1;
    assert(_gst_lg_adec_set_caps_srcpad_no_dec(&decoder) == GST_FLOW_ERROR);
    getter_error = 0;
    fail_caps = true;
    assert(_gst_lg_adec_set_caps_srcpad_no_dec(&decoder) == GST_FLOW_ERROR);
    assert(_gst_lg_adec_set_caps_srcpad_dummy(&decoder) == GST_FLOW_ERROR);
    puts("decoder caps event outcomes and ownership passed");
    return 0;
}