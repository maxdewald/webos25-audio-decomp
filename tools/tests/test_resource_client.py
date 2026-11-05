#!/usr/bin/env python3
"""Strict, sanitized host core tests: python3 tools/tests/test_resource_client.py."""
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def main():
    output = ROOT / 'build/scratch/resource-client-test'
    output.mkdir(parents=True, exist_ok=True)
    executable = output / 'rmc-core-test'
    command = [
        'clang++', '-std=c++14', '-O2', '-DRMC_HOST_TEST',
        '-Wall', '-Wextra', '-Werror', '-Wno-deprecated-declarations',
        '-fsanitize=address,undefined', '-pthread',
        '-Itools/tests/resource_client',
        '-Ilg-opensource/public/rapidjson/include',
        'src/libresource_mgr_client/ResourceManagerClient.cpp',
        'tools/tests/resource_client/core_test.cc',
        '-o', str(executable),
    ]
    try:
        subprocess.run(command, cwd=ROOT, check=True, timeout=120)
        subprocess.run([str(executable)], cwd=ROOT, check=True, timeout=30)
    finally:
        executable.unlink(missing_ok=True)


if __name__ == '__main__':
    main()
