#!/usr/bin/env python3
"""Test recovered kernel output/callback/routing contracts with mocked services.

usage: python3 tools/test_output.py
Extracts the source functions into a host C harness; does not run kernel code.
"""
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parent.parent
FUNCTIONS = {
    'output.c': ['output_set_mute', 'output_set_delay', '_set_format',
                 '_adec_codec_convert_output_codec', '_cb_output_status',
                 'output_pre_connect', 'output_post_connect',
                 'output_pre_disconnect', 'output_post_disconnect'],
    'alsa_sndout.c': ['_set_output', 'sndout_earc_output_type'],
}


def extract(text, name):
    import re
    match = re.search(r'^(?:static )?(?:int|void|enum output_codec_t) ' + re.escape(name)
                      + r'\s*\(', text, re.M)
    if not match:
        raise RuntimeError(f'Function missing: {name}')
    start = text.index('{', match.start())
    depth = 1
    end = start + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[match.start():end]


def main():
    cc = shutil.which(os.environ.get('CC', 'cc'))
    if cc is None:
        raise SystemExit('C compiler missing; set CC to an installed compiler')
    bodies = []
    for filename, names in FUNCTIONS.items():
        text = (ROOT / 'src/kernel/aud' / filename).read_text()
        bodies.extend(extract(text, name) for name in names)
    fixture = (ROOT / 'tools/tests/test_output.c').read_text()
    generated = fixture.replace('/* RECOVERED_FUNCTIONS */', '\n\n'.join(bodies))
    scratch = ROOT / 'build/scratch/output-path-tests'
    scratch.mkdir(parents=True, exist_ok=True)
    source, binary = scratch / 'output_test.c', scratch / 'output_test'
    source.write_text(generated)
    try:
        command = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror',
                   '-Wno-unused-parameter', '-Wno-unused-variable',
                   '-Wno-unused-const-variable', '-Wno-sign-compare',
                   str(source), '-o', str(binary)]
        print('+', ' '.join(command), flush=True)
        subprocess.run(command, cwd=ROOT, check=True)
        subprocess.run([str(binary)], cwd=ROOT, check=True)
    finally:
        source.unlink(missing_ok=True)
        binary.unlink(missing_ok=True)
        scratch.rmdir()


if __name__ == '__main__':
    main()
