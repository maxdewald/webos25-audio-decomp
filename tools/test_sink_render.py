#!/usr/bin/env python3
"""Execute source-extracted render chain, inline caps and event with host mocks.

usage: python3 tools/test_sink_render.py
No LG code is executed, no device is opened, and mock success is not playback.
"""
import os
from pathlib import Path
import re
import shutil
import subprocess

ROOT = Path(__file__).resolve().parent.parent
FUNCTIONS = ['gst_lg_audio_render_set_caps', 'gst_lg_audio_render_event',
             'gst_lg_audio_render_chain']


def extract(text, name):
    match = re.search(r'^(?:static inline )?(?:gboolean|GstFlowReturn)\s*\n?'
                      + re.escape(name) + r'\s*\(', text, re.M)
    if not match:
        raise RuntimeError(f'Function missing: {name}')
    start = text.index('{', match.start())
    depth, end = 1, start + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[match.start():end]


def main():
    cc = shutil.which(os.environ.get('CC', 'cc'))
    if cc is None:
        raise SystemExit('C compiler missing; set CC')
    text = (ROOT / 'src/libgstlgaudiosink/render_element.c').read_text()
    flags = text[text.index('#define LG_BUFFER_FLAG_METADATA '):
                 text.index('static const char *\nout_type_name')]
    bodies = '\n\n'.join(extract(text, name) for name in FUNCTIONS)
    fixture = (ROOT / 'tools/tests/test_sink_render.c').read_text()
    scratch = ROOT / 'build/scratch/sink-render-tests'
    scratch.mkdir(parents=True, exist_ok=True)
    source, binary = scratch / 'test_sink_render.c', scratch / 'test_sink_render'
    source.write_text(fixture.replace('/* RECOVERED_FUNCTIONS */', flags + bodies))
    try:
        command = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror',
                   '-Wno-unused-parameter', '-Wno-unused-variable',
                   '-Wno-unused-function', '-I', str(ROOT / 'src'),
                   str(source), '-o', str(binary), '-lm']
        print('+', ' '.join(command), flush=True)
        subprocess.run(command, cwd=ROOT, check=True)
        subprocess.run([str(binary)], cwd=ROOT, check=True, timeout=15)
    finally:
        source.unlink(missing_ok=True)
        binary.unlink(missing_ok=True)
        scratch.rmdir()


if __name__ == '__main__':
    main()
