#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_common_reconstruction.py; libaudcommon host regressions."""
import os
from pathlib import Path
import subprocess
import sys
from tempfile import TemporaryDirectory

ROOT = Path(__file__).resolve().parents[2]
SCRATCH = ROOT / 'build/scratch'
SCRATCH.mkdir(parents=True, exist_ok=True)
flags = ['-std=gnu11', '-Wall', '-Wextra', '-Werror', '-Wno-missing-field-initializers',
         '-Wno-sign-compare', '-g', '-pthread', '-ffunction-sections', '-fdata-sections',
         '-Wl,-dead_strip' if sys.platform == 'darwin' else '-Wl,--gc-sections']
sources = ['tools/tests/test_common_reconstruction.c', 'src/libaudcommon/adapter.c',
           'src/libaudcommon/data.c', 'src/libaudcommon/event.c']
with TemporaryDirectory(prefix='common-tests-', dir=SCRATCH) as output:
    for name, extra in [('strict', ['-O2']),
                        ('sanitized', ['-O1', '-fsanitize=address,undefined',
                                       '-fno-omit-frame-pointer', '-fno-sanitize-recover=all'])]:
        executable = Path(output) / name
        command = [os.environ.get('CC', 'cc'), *flags, *extra, *sources, '-o', str(executable)]
        print(f'{name}: ' + ' '.join(command), flush=True)
        subprocess.run(command, cwd=ROOT, check=True, timeout=120)
        subprocess.run([str(executable)], cwd=ROOT, check=True, timeout=30,
                       env={**os.environ, 'UBSAN_OPTIONS': 'halt_on_error=1',
                            'ASAN_OPTIONS': ('detect_leaks=0' if sys.platform == 'darwin'
                                             else 'detect_leaks=1')})
