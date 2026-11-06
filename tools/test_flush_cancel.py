#!/usr/bin/env python3
"""Usage: python3 tools/test_flush_cancel.py; source-extracted offline pthread tests.

Runs recovered libaa bodies against deterministic host ring/peer mocks, not LG code.
"""
import os
from pathlib import Path
import shutil
import subprocess
import sys
from uuid import uuid4

import re


def extract(text, name):
    match = re.search(r'^int ' + re.escape(name) + r'\s*\([^;{]*\)\s*\{|'
                      r'^void ' + re.escape(name) + r'\s*\([^;{]*\)\s*\{', text, re.M)
    if match is None:
        raise RuntimeError('Function definition missing: ' + name)
    start = text.index('{', match.start())
    depth, end = 1, start + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[match.start():end]

ROOT = Path(__file__).resolve().parent.parent
FUNCTIONS = [
    'aud_port_sync_locked', 'foreach_sync', 'aud_module_sync',
    'foreach_push_event', 'aud_module_push_event', 'aud_port_push_event',
    'foreach_down_peer_push_event', 'foreach_down_peer_push_data',
    'aud_module_event_locked', 'aud_module_event', 'aud_module_chain_locked',
    'aud_module_unblock_locked', 'aud_port_check_legacy',
    'aud_module_check_legacy_output',
]


def main():
    cc = shutil.which(os.environ.get('CC', 'cc'))
    if cc is None:
        raise SystemExit('C compiler missing; set CC')
    text = (ROOT / 'src/libaa/module.c').read_text()
    bodies = '\n\n'.join(extract(text, name) for name in FUNCTIONS)
    bodies += '\n\n' + extract((ROOT / 'src/libaabase/appsrc.c').read_text(),
                                'module_appsrc_push_event')
    fixture = (ROOT / 'tools/tests/test_flush_cancel.c').read_text()
    scratch = ROOT / 'build/scratch' / ('flush-cancel-' + uuid4().hex)
    scratch.mkdir(parents=True)
    source, binary = scratch / 'test.c', scratch / 'test'
    source.write_text(fixture.replace('/* RECOVERED_FUNCTIONS */', bodies))
    try:
        for name, flags in [('strict', ['-O2']),
                            ('asan-ubsan', ['-O1', '-fsanitize=address,undefined',
                                            '-fno-sanitize-recover=all',
                                            '-fno-omit-frame-pointer'])]:
            command = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror',
                       '-Wno-unused-parameter', '-g', *flags, '-I', str(ROOT / 'src'),
                       str(source), '-o', str(binary), '-pthread']
            print(name + ': ' + ' '.join(command), flush=True)
            subprocess.run(command, check=True, cwd=ROOT, timeout=45)
            subprocess.run([str(binary)], check=True, cwd=ROOT, timeout=20,
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
