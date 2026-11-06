#!/usr/bin/env python3
"""Usage: python3 tools/test_libaa_match_regression.py; offline read/property mocks."""
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
from uuid import uuid4

ROOT = Path(__file__).resolve().parent.parent


def extract(text, name):
    match = re.search(r'^(?:static )?(?:int|bool) ' + name + r'\s*\(', text, re.M)
    if match is None:
        raise RuntimeError('Missing function: ' + name)
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
    module = (ROOT / 'src/libaa/module.c').read_text()
    buffers = (ROOT / 'src/libaa/buffers.c').read_text()
    bodies = '\n\n'.join([extract(module, 'parse_bool'),
                             extract(module, 'aud_module_set_prop_locked'),
                             extract(buffers, 'CommonBuffer_ReadData')])
    fixture = (ROOT / 'tools/tests/test_libaa_match_regression.c').read_text()
    scratch = ROOT / 'build/scratch' / ('libaa-match-tests-' + uuid4().hex)
    scratch.mkdir(parents=True)
    source, binary = scratch / 'test.c', scratch / 'test'
    source.write_text(fixture.replace('/* RECOVERED_FUNCTIONS */', bodies))
    try:
        for name, flags in [('strict', ['-O2']),
                            ('sanitized', ['-O1', '-fsanitize=address,undefined',
                                           '-fno-sanitize-recover=all',
                                           '-fno-omit-frame-pointer'])]:
            command = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror',
                       '-Wno-unused-parameter', '-g', *flags, '-I', str(ROOT / 'src'),
                       str(source), '-o', str(binary), '-pthread']
            print(name + ': ' + ' '.join(command), flush=True)
            subprocess.run(command, check=True, cwd=ROOT, timeout=45)
            subprocess.run([str(binary)], check=True, cwd=ROOT, timeout=15,
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
