#!/usr/bin/env python3
"""Test recovered lgao transport/FIRST/lifetime contracts with mocked appsrc/ioctls.

usage: python3 tools/test_lgao.py
Host-only function extraction; does not run LG binaries or establish playback.
Mocked push consumes one handed-off reference; worker scheduling/drain is not tested.
"""
import os
from pathlib import Path
import shutil
import subprocess

from test_output import extract

ROOT = Path(__file__).resolve().parent.parent
FUNCTIONS = [
    '_lgao_config_set_renderer_param', 'lgao_config',
    'lgao_set_first_render', 'lgao_set_pcm_bypass', 'lgao_set_pcm_multi_channel',
    'lgao_set_audio_info', 'lgao_close_renderer', 'lgao_flush', 'lgao_eos',
    '_lgao_set_playspeed', 'lgao_set_playrate', 'lgao_compare_audio_info',
    'lgao_set_mute', 'lgao_disconnect_renderer',
    'lgao_push_first_event', 'lgao_feed', 'lgao_set_drop_renderer',
]


def main():
    cc = shutil.which(os.environ.get('CC', 'cc'))
    if cc is None:
        raise SystemExit('C compiler missing; set CC to an installed compiler')
    text = (ROOT / 'src/libgstlgaudiosink/lgao.c').read_text()
    text = text.replace('static inline void ', 'static void ')
    bodies = '\n\n'.join(extract(text, name) for name in FUNCTIONS)
    fixture = (ROOT / 'tools/tests/test_lgao.c').read_text()
    scratch = ROOT / 'build/scratch/lgao-transport-tests'
    scratch.mkdir(parents=True, exist_ok=True)
    source, binary = scratch / 'test_lgao.c', scratch / 'test_lgao'
    source.write_text(fixture.replace('/* RECOVERED_FUNCTIONS */', bodies))
    try:
        command = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror',
                   '-Wno-unused-parameter', '-Wno-unused-variable',
                   '-I', str(ROOT / 'src'), str(source), '-o', str(binary), '-pthread']
        print('+', ' '.join(command), flush=True)
        subprocess.run(command, cwd=ROOT, check=True)
        subprocess.run([str(binary)], cwd=ROOT, check=True)
    finally:
        source.unlink(missing_ok=True)
        binary.unlink(missing_ok=True)
        scratch.rmdir()


if __name__ == '__main__':
    main()
