#!/usr/bin/env python3
"""Host wrapper semantics: python3 tools/tests/test_source_assisted_wrappers.py [--sanitize]."""
import argparse
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize', action='store_true', help='enable ASan and UBSan')
    args = parser.parse_args()
    output = ROOT / 'build/scratch/wrapper-source-assist'
    output.mkdir(parents=True, exist_ok=True)
    executable = output / 'test_source_assisted_wrappers'
    command = [os.environ.get('CXX', 'clang++'), '-std=c++14', '-Wall', '-Wextra',
               '-Werror', '-fno-sized-deallocation',
               '-I' + str(ROOT / 'lg-opensource/public/umediaserver/include/public'),
               str(ROOT / 'tools/tests/test_source_assisted_wrappers.cpp'),
               '-o', str(executable)]
    if args.sanitize:
        command[1:1] = ['-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer']
    try:
        subprocess.run(command, cwd=ROOT, check=True)
        subprocess.run([str(executable)], cwd=ROOT, check=True)
    finally:
        executable.unlink(missing_ok=True)


if __name__ == '__main__':
    main()
