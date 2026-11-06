#!/usr/bin/env python3
"""Usage: python3 tools/test_appsrc.py; offline host appsrc/libaa boundary mocks.

Runs recovered queue/sync/teardown bodies, not LG binaries or DSP workers.
"""
import os
from pathlib import Path
import shutil
import subprocess
import sys
from uuid import uuid4

from test_output import extract

ROOT = Path(__file__).resolve().parent.parent
FUNCTIONS = [
    'add_pending_obj_locked', 'aud_module_push_data', 'aud_module_sync',
    'aud_port_sync_locked', 'foreach_sync', 'foreach_push_event',
    'aud_module_push_event', 'del_output_port',
]


def main():
    cc = shutil.which(os.environ.get('CC', 'cc'))
    if cc is None:
        raise SystemExit('C compiler missing; set CC to an installed compiler')
    text = (ROOT / 'src/libaa/module.c').read_text()
    bodies = '\n\n'.join(extract(text, name) for name in FUNCTIONS)
    fixture = (ROOT / 'tools/tests/test_appsrc.c').read_text()
    scratch = ROOT / 'build/scratch' / f'appsrc-boundary-tests-{uuid4().hex}'
    scratch.mkdir(parents=True, exist_ok=True)
    source, binary = scratch / 'test_appsrc.c', scratch / 'test_appsrc'
    source.write_text(fixture.replace('/* RECOVERED_FUNCTIONS */', bodies))
    try:
        for name, extra in [('strict', ['-O2']),
                            ('sanitized', ['-O1', '-fsanitize=address,undefined',
                                           '-fno-omit-frame-pointer',
                                           '-fno-sanitize-recover=all'])]:
            command = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror',
                       '-Wno-unused-parameter', '-g', *extra,
                       '-I', str(ROOT / 'src'), str(source), '-o', str(binary), '-pthread']
            print(name + ': ' + ' '.join(command), flush=True)
            subprocess.run(command, cwd=ROOT, check=True)
            subprocess.run([str(binary)], cwd=ROOT, check=True, timeout=15,
                           env={**os.environ, 'UBSAN_OPTIONS': 'halt_on_error=1',
                                'ASAN_OPTIONS': ('detect_leaks=0' if sys.platform == 'darwin'
                                                 else 'detect_leaks=1')})
    finally:
        source.unlink(missing_ok=True)
        binary.unlink(missing_ok=True)
        shutil.rmtree(binary.with_suffix('.dSYM'), ignore_errors=True)
        scratch.rmdir()


if __name__ == '__main__':
    main()
