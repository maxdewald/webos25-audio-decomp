#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_aabase.py; strict and ASan/UBSan host behavior checks."""
import os
import sys
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/scratch/aabase-pass/tests'
OUT.mkdir(parents=True, exist_ok=True)
flags = ['-std=c11', '-Wall', '-Wextra', '-Werror', '-Wno-unused-parameter', '-g']
if sys.platform == 'darwin':
    # The shipped loader uses sprintf, which Apple's libc marks deprecated.
    flags.append('-Wno-deprecated-declarations')
for name, extra in [('strict', ['-O2']),
                    ('sanitized', ['-O1', '-fsanitize=address,undefined',
                                   '-fno-omit-frame-pointer', '-fno-sanitize-recover=all'])]:
    executable = OUT / name
    command = [os.environ.get('CC', 'cc'), *flags, *extra,
               str(ROOT / 'tools/tests/test_aabase.c'), '-o', str(executable)]
    print(f'{name}: ' + ' '.join(command), flush=True)
    subprocess.run(command, cwd=ROOT, check=True)
    subprocess.run([str(executable)], cwd=ROOT, check=True,
                   env={**os.environ, 'UBSAN_OPTIONS': 'halt_on_error=1',
                        'ASAN_OPTIONS': ('detect_leaks=0' if sys.platform == 'darwin'
                                         else 'detect_leaks=1')})
