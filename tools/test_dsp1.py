#!/usr/bin/env python3
"""Host tests for recovered DSP1 eARC-path contracts; not firmware execution.

usage: python3 tools/test_dsp1.py
"""
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent.parent


def run(args):
    print('+', ' '.join(map(str, args)), flush=True)
    subprocess.run(args, cwd=ROOT, check=True)


def main():
    cc = shutil.which(os.environ.get('CC', 'cc'))
    if cc is None:
        raise SystemExit('C compiler missing; set CC to an installed compiler')
    sources = [ROOT / 'src/dsp1' / f'{name}.c'
               for name in ('fader_queue', 'glue_reader', 'out_module', 'output_arc', 'outctrlbin')]
    run([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-fsyntax-only', *sources])
    tests = ROOT / 'tools/tests/dsp1'
    flags = ['-Wl,-dead_strip'] if sys.platform == 'darwin' else ['-Wl,--gc-sections']
    with tempfile.TemporaryDirectory(prefix='dsp1-earc-') as temporary:
        for name in ('fader_queue', 'glue_reader', 'output_bc', 'outctrlbin', 'hal_arc'):
            output = Path(temporary) / name
            run([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-ffunction-sections',
                 '-fdata-sections', *flags, tests / f'{name}.c', '-o', output])
            run([output])
    print('PASS: DSP1 eARC queue/receiver/output contracts', flush=True)


if __name__ == '__main__':
    main()
