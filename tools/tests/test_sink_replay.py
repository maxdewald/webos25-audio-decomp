#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_sink_replay.py

Execute the actual deferred-property replay switch with generated host service
mocks. No GStreamer dependency or runtime hooking.
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
    header = (ROOT / 'src/libgstlgaudiosink/render_element.h').read_text()
    text = (ROOT / 'src/libgstlgaudiosink/render_props.c').read_text()
    body = function(text, '_gst_lg_audio_render_set_preconditions')
    enum = re.search(r'enum \{\s*PROP_0,.*?\n\};', header, re.S).group()
    calls = sorted(set(re.findall(r'\b(lgao_\w+)\s*\(', body)))
    fields = sorted(set(re.findall(r'pAudioRen->(\w+)', body)) - {'ao', 'preopen_conditions'})
    string_fields = {'pAppType', 'clocktype'}
    declarations = '\n'.join(('const char *' if f in string_fields else 'int ') + f + ';' for f in fields)
    constants = sorted(set(re.findall(r'\bLGAO_[A-Z0-9_]+\b', body)) - {'LGAO_APPTYPE_T'})
    constants_text = '\n'.join(f'#define {name} {i+1}' for i, name in enumerate(constants))
    mock_calls = '\n'.join(
        f'#define {name}(ao,value) record_pcm("{name}",(ao),(value))'
        if name in {'lgao_set_pcm_bypass', 'lgao_set_pcm_multi_channel'}
        else f'#define {name}(...) record("{name}")' for name in calls)
    fixture = '''#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef unsigned guint; typedef int gint; typedef const char gchar; typedef int LGAO_APPTYPE_T;
#define FALSE 0
#define GST_BASE_SINK(p) (p)
#define gst_base_sink_set_sync(p,v) ((void)(p),(void)(v))
#define SINK_LOG(...) ((void)0)
#define PREOPEN_BIT(p) (UINT64_C(1)<<((p)-1))
#define strcmp(a,b) replay_compare(a,b)
static int replay_compare(const char *a,const char *b) { assert(a && b); return __builtin_strcmp(a,b); }
'''
    fixture += enum + '\n' + constants_text + '\n'
    fixture += 'typedef struct { void *ao; uint64_t preopen_conditions;\n' + declarations + '\n} GstLgAudioRender;\n'
    fixture += '''static const char *trace[64]; static unsigned count, pcm_calls;
static void *last_ao; static int last_pcm;
static int record(const char *name) { assert(count<64); trace[count++]=name; return -1; }
static int record_pcm(const char *name,void *ao,int value) { ++pcm_calls;last_ao=ao;last_pcm=value;return record(name); }
'''
    fixture += mock_calls + '\nstatic void replay(GstLgAudioRender *pAudioRen)\n' + body
    fixture += '''
int main(void) {
    int token;
    GstLgAudioRender s={.ao=&token,.pcm_bypass=1,.pcm_multi_ch=1};
    s.preopen_conditions=PREOPEN_BIT(PROP_PCM_BYPASS)|PREOPEN_BIT(PROP_PCM_MULTI_CH)|PREOPEN_BIT(PROP_QUEUE_MAX_LENGTH);
    uint64_t original=s.preopen_conditions;
    replay(&s);
    assert(count==3 && !strcmp(trace[0],"lgao_set_pcm_bypass"));
    assert(!strcmp(trace[1],"lgao_set_pcm_multi_channel") && !strcmp(trace[2],"lgao_set_queue_max_length"));
    assert(s.preopen_conditions==original && pcm_calls==2 && last_ao==&token && last_pcm==1);
    replay(&s);assert(count==6 && pcm_calls==4 && s.preopen_conditions==original);
    s.preopen_conditions=0;count=0;replay(&s);assert(!count);
    s.preopen_conditions=PREOPEN_BIT(PROP_PCM_MULTI_CH);s.ao=NULL;replay(&s);
    assert(count==1 && !strcmp(trace[0],"lgao_set_pcm_multi_channel") && last_ao==NULL && last_pcm==1);
    puts("PASS sink replay: property-ID order, cached PCM flags, failed replay retains mask, no ao readiness gate");
    return 0;
}
'''
    with tempfile.TemporaryDirectory(prefix='sink-replay-') as temporary:
        source, binary = Path(temporary)/'replay.c', Path(temporary)/'replay'
        source.write_text(fixture)
        for label, extra in [('strict', ['-O2']), ('sanitized', ['-O1', '-fsanitize=address,undefined'])]:
            command = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror',
                       '-Wno-unused-but-set-variable', *extra, str(source), '-o', str(binary)]
            print(label + ': ' + ' '.join(command), flush=True)
            subprocess.run(command, cwd=ROOT, check=True, timeout=60)
            subprocess.run([str(binary)], cwd=ROOT, check=True, timeout=15)


if __name__ == '__main__':
    main()
