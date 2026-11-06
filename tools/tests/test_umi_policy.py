#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_umi_policy.py [--sanitize]."""
import argparse
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize', action='store_true')
    args = parser.parse_args()
    output = ROOT / 'build/scratch/broad/policy'
    output.mkdir(parents=True, exist_ok=True)
    executable = output / 'test_umi_policy'
    command = [os.environ.get('CC', 'clang'), '-std=gnu11', '-Wall', '-Wextra',
               '-Werror', str(ROOT / 'tools/tests/test_umi_policy.c'),
               *map(str, sorted((ROOT / 'src/libumi').glob('*.c'))),
               '-o', str(executable)]
    if args.sanitize:
        command[1:1] = ['-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer']
    try:
        subprocess.run(command, cwd=ROOT, check=True)
        subprocess.run([str(executable)], cwd=ROOT, check=True,
                       env={**os.environ, 'UBSAN_OPTIONS': 'halt_on_error=1'})
    finally:
        executable.unlink(missing_ok=True)


if __name__ == '__main__':
    main()
