#!/usr/bin/env python3
"""Exercise recovered lgao open/connect against mocked renderer and IMC services.

usage: python3 tools/test_lgao_connection.py
No device or LG process is opened. Kernel route selection is an external contract.
"""
import os
from pathlib import Path
import shutil
import subprocess

from test_output import extract

ROOT = Path(__file__).resolve().parent.parent


def main():
    cc = shutil.which(os.environ.get('CC', 'cc'))
    if cc is None:
        raise SystemExit('C compiler missing; set CC')
    text = (ROOT / 'src/libgstlgaudiosink/lgao.c').read_text()
    names = ['_lgao_open_renderer', '_lgao_connect_renderer',
             'lgao_disconnect_renderer', 'lgao_close_renderer',
             'lgao_open_renderer', 'lgao_compare_audio_info']
    bodies = '\n\n'.join(extract(text, name) for name in names)
    fixture = (ROOT / 'tools/tests/test_lgao_connection.c').read_text()
    scratch = ROOT / 'build/scratch/lgao-connection-tests'
    scratch.mkdir(parents=True, exist_ok=True)
    source, binary = scratch / 'test.c', scratch / 'test'
    source.write_text(fixture.replace('/* RECOVERED_FUNCTIONS */', bodies))
    try:
        for flags in (['-O2'], ['-O1', '-fsanitize=address,undefined',
                               '-fno-sanitize-recover=all']):
            command = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror',
                       '-Wno-unused-parameter', *flags, '-I', str(ROOT / 'src'),
                       str(source), '-o', str(binary)]
            print('+', ' '.join(command), flush=True)
            subprocess.run(command, cwd=ROOT, check=True)
            subprocess.run([str(binary)], cwd=ROOT, check=True, timeout=15)
    finally:
        source.unlink(missing_ok=True)
        binary.unlink(missing_ok=True)
        scratch.rmdir()


if __name__ == '__main__':
    main()
