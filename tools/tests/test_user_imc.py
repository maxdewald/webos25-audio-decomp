#!/usr/bin/env python3
"""Compile/run scoped user IMC behavior: python3 tools/tests/test_user_imc.py."""
from pathlib import Path
import argparse
import os
import re
import subprocess

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/scratch/user-imc-verify'
SOURCE = ROOT / 'src/kernel/aud/user_imc.c'


def function(text, name):
    match = re.search(r'^static (?:int|long) ' + re.escape(name) + r'\s*\(', text, re.M)
    assert match, name
    start = text.index('{', match.start())
    depth = 1
    end = start + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[match.start():end]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize', action='store_true', help='enable ASan and UBSan')
    args = parser.parse_args()
    OUT.mkdir(parents=True, exist_ok=True)
    source = SOURCE.read_text()
    names = ['open', 'release', 'connect_module', 'disconnect_module_output',
             'disconnect_module_input', 'ioctl', 'compat_ioctl']
    structs = source[source.index('struct hma_shmem\n'):source.index('static DEFINE_MUTEX')]
    abi = (ROOT / 'lg-opensource/kdriver/include/audio_uimc.h').read_text()
    abi = abi[abi.index('struct auimc_ipc_header'):abi.rindex('#endif')]
    generated = abi + '\n' + structs + '\n' + '\n\n'.join(function(source, n) for n in names)
    (OUT / 'user_imc_scoped.inc').write_text(generated)
    executable = OUT / ('test_user_imc_sanitized' if args.sanitize else 'test_user_imc')
    command = [os.environ.get('CC', 'cc'), '-std=gnu11', '-Wall', '-Wextra',
               '-Werror', '-Wno-unused-parameter', '-Wno-unused-variable',
               '-I' + str(OUT), str(ROOT / 'tools/tests/test_user_imc.c'), '-o', str(executable)]
    if args.sanitize:
        command[1:1] = ['-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer']
    try:
        subprocess.run(command, check=True, cwd=ROOT)
        subprocess.run([str(executable)], check=True, cwd=ROOT)
    finally:
        executable.unlink(missing_ok=True)
        (OUT / 'user_imc_scoped.inc').unlink(missing_ok=True)


if __name__ == '__main__':
    main()
