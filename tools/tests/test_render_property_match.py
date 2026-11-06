#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_render_property_match.py

Execute recovered property getter and timestamp-offset branches with host mocks.
These are source regression tests, not execution of LG's binary.
"""
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

from test_sink_context import function

ROOT = Path(__file__).resolve().parents[2]


def main():
    cc = shutil.which(os.environ.get('CC', 'cc'))
    if not cc:
        raise SystemExit('C compiler missing')
    props = (ROOT / 'src/libgstlgaudiosink/render_props.c').read_text()
    element = (ROOT / 'src/libgstlgaudiosink/render_element.c').read_text()
    header = (ROOT / 'src/libgstlgaudiosink/render_element.h').read_text()
    getter = function(props, 'gst_lg_audio_render_get_property')
    offset = function(element, 'gst_lg_audio_render_update_ts_offset')
    enum = re.search(r'enum \{\s*PROP_0,.*?\n\};', header, re.S).group()
    calls = sorted(set(re.findall(r'\b(lgao_\w+)\s*\(', getter)))
    fixture = r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libgstlgaudiosink/lgao.h"
typedef int gint; typedef unsigned guint; typedef char gchar;
typedef uint64_t guint64; typedef int64_t GstClockTimeDiff;
typedef struct { uint64_t currentPTS; int disableBasesinkSync, isSeamless, vsp_mode;
                 int lipsync_offset; char *pAppType; void *ao; } GstLgAudioRender;
typedef GstLgAudioRender GObject;
typedef struct { const char *name; } GParamSpec;
typedef struct { unsigned g_type; int64_t number; const char *string; } GValue;
#define GST_LG_AUDIO_RENDER(p) (p)
#define GST_BASE_SINK(p) (p)
#define G_LIKELY(p) (p)
#define GST_MSECOND INT64_C(1000000)
#define G_VALUE_TYPE(v) ((v)->g_type)
#define G_PARAM_SPEC_TYPE(p) 0
#define G_OBJECT_TYPE_NAME(p) "object"
#define LG_SINK_FILE "gstlgaudiosink.c"
#define G_LOG_LEVEL_WARNING 16
#define AUDIOSINK_VERSION 20180329u
#define FALSE 0
#define PROP_LOG(...) ((void)0)
#define RENDER_LOG(...) ((void)0)
#define SINK_CHECK(call,line) ((void)(call))
#define g_log(...) (++warnings)
static int warnings, fail, supplied;
static unsigned calls;
static const char *last_getter;
static int64_t ts;
static int read_int(const char *name, void *output) {
    ++calls; last_getter=name;
    if (!fail) memcpy(output, &supplied, sizeof supplied);
    return fail ? -1 : 0;
}
static int read_size(void *output) {
    uint64_t size=123456789; ++calls;last_getter="lgao_get_buffer_status";
    if (!fail) memcpy(output,&size,sizeof size);
    return fail ? -1 : 0;
}
static char *g_strdup_value_contents(const GValue *v) { (void)v; return NULL; }
#define g_free free
static void g_value_set_int(GValue *v, int n) { v->number=n; }
static void g_value_set_uint(GValue *v, unsigned n) { v->number=n; }
static void g_value_set_uint64(GValue *v, uint64_t n) { v->number=n; }
static void g_value_set_boolean(GValue *v, int n) { v->number=n; }
static void g_value_set_string(GValue *v, const char *s) { v->string=s; }
static void gst_base_sink_set_ts_offset(GstLgAudioRender *self, int64_t n) {
    (void)self; ts=n;
}
'''
    fixture += enum + '\n'
    for name in calls:
        if name == 'lgao_get_buffer_status':
            fixture += f'#define {name}(ao,out) read_size(out)\n'
        elif name in {'lgao_get_rts', 'lgao_get_fade'}:
            fixture += f'#define {name}(ao,kind,out) read_int("{name}",out)\n'
        elif name == 'lgao_get_buffer_size_for_directmedia':
            fixture += f'#define {name}(ao,out,kind) read_int("{name}",out)\n'
        else:
            fixture += f'#define {name}(ao,out) read_int("{name}",out)\n'
    fixture += 'static void get(GObject *object,guint prop_id,GValue *value,GParamSpec *pspec)\n' + getter
    fixture += '\nstatic void update(GstLgAudioRender *pAudioRen,gint playspeed)\n' + offset
    fixture += r'''
int main(void) {
    GstLgAudioRender sink={.currentPTS=987654321,.disableBasesinkSync=1,
                           .isSeamless=1,.vsp_mode=1};
    GParamSpec spec={"property"};
    GValue value={0};
    unsigned ids[]={PROP_PLAY_SPEED,PROP_LIPSYNC_OFFSET,PROP_LIPSYNC,PROP_NODELAY,
        PROP_NODELAY_MODE,PROP_NODELAY_SKIP_UP,PROP_NODELAY_FAST_UP,PROP_NODELAY_RECOVER,
        PROP_NODELAY_FAST_FREQ,PROP_MIXER,PROP_AUDIO_INDEX,PROP_MUTE,
        PROP_DIRECTMEDIA_TOTAL_BUF_SIZE,PROP_DIRECTMEDIA_USED_BUF_SIZE,PROP_PCM_BYPASS,
        PROP_PCM_MULTI_CH,PROP_GAME_MODE,PROP_RENDER_BUFFER_THRESHOLD,PROP_QUEUE_MAX_LENGTH};
    for (unsigned i=0;i<sizeof ids/sizeof *ids;++i) {
        unsigned id=ids[i];fail=1;value.number=999;calls=0;
        get(&sink,id,&value,&spec);assert(calls==1);
        assert(value.number==(id==PROP_AUDIO_INDEX ? UINT32_MAX : id==PROP_LIPSYNC ? 1 : 0));
        fail=0;supplied=2;get(&sink,id,&value,&spec);
        assert(value.number==(id==PROP_LIPSYNC ? 0 : 2));
    }
    fail=0;supplied=17;get(&sink,PROP_DECODED_SIZE,&value,&spec);
    assert(value.number==0 && !strcmp(last_getter,"lgao_get_mode_mixer"));
    fail=1;get(&sink,PROP_UNDECODED_SIZE,&value,&spec);assert(value.number==0);
    fail=0;get(&sink,PROP_UNDECODED_SIZE,&value,&spec);assert(value.number==123456789);
    fail=1;get(&sink,PROP_FADE_VOLUME,&value,&spec);assert(value.number==100);
    const char *apps[]={"UNKNOWN","DIRECTMEDIA","RTC","MIRACAST","UNKNOWN","JP4K",
                        "ID-GAPLESS","UNKNOWN","R2R","SYE","UNKNOWN"};
    fail=0;
    for (supplied=0;supplied<=10;++supplied) {
        get(&sink,PROP_APP_TYPE,&value,&spec);assert(!strcmp(value.string,apps[supplied]));
    }
    get(&sink,PROP_CURRENT_PTS,&value,&spec);assert(value.number==987654321);
    get(&sink,PROP_DISABLE_GSTSYNC,&value,&spec);assert(value.number==1);
    get(&sink,PROP_SEAMLESS,&value,&spec);assert(value.number==1);
    get(&sink,PROP_VARIABLE_SPEED_PLAYBACK,&value,&spec);assert(value.number==1);
    get(&sink,PROP_AUDIO_ONLY,&value,&spec);assert(value.number==0);
    get(&sink,PROP_CLOCK_TYPE,&value,&spec);assert(value.string==NULL);
    get(&sink,PROP_AUDIO_BE_DELAY,&value,&spec);assert(value.number==85);
    get(&sink,PROP_VERSION,&value,&spec);assert(value.number==20180329);
    get(&sink,PROP_RESOURCE_INFO,&value,&spec);assert(warnings==1);
    sink.lipsync_offset=400;
    int speeds[]={-1,0,249,250,500,999,1000,2000};
    for (unsigned i=0;i<sizeof speeds/sizeof *speeds;++i) {
        int rate=speeds[i];update(&sink,rate);
        assert(ts==((unsigned)(rate-250)<750 ? -INT64_C(475000000000)/rate : 0));
    }
    sink.lipsync_offset=399;update(&sink,1000);assert(ts==-1000000);
    sink.lipsync_offset=0;update(&sink,1000);assert(ts==-400000000);
    sink.pAppType="ID-GAPLESS";sink.lipsync_offset=399;
    update(&sink,1000);assert(ts==-400000000);
    sink.lipsync_offset=400;update(&sink,1000);assert(ts==0);
    puts("PASS render properties: getter defaults/types/app mappings; timestamp speed/offset boundaries");
}
'''
    scratch = ROOT / 'build/scratch'
    scratch.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='render-property-match-', dir=scratch) as temporary:
        src, binary = Path(temporary) / 'properties.c', Path(temporary) / 'properties'
        src.write_text(fixture)
        for label, flags in [('strict', ['-O2']),
                             ('sanitized', ['-O1', '-fsanitize=address,undefined',
                                            '-fno-sanitize-recover=all'])]:
            command = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror',
                       '-Wno-unused-parameter', '-Wno-unused-variable', *flags,
                       '-I', str(ROOT / 'src'), str(src), '-o', str(binary)]
            print(label + ': ' + ' '.join(command), flush=True)
            subprocess.run(command, cwd=ROOT, check=True, timeout=60)
            subprocess.run([str(binary)], cwd=ROOT, check=True, timeout=15)


if __name__ == '__main__':
    main()
