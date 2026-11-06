#!/usr/bin/env python3
"""Offline host regression: python3 tools/tests/test_codec_reporting.py."""
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def main():
    output = ROOT / 'build/scratch/broad/metadata'
    output.mkdir(parents=True, exist_ok=True)
    executable = output / 'codec-reporting-test'
    command = [
        'clang++', '-std=c++14', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
        '-Wno-deprecated-declarations', '-fsanitize=address,undefined',
        '-isystem', 'lg-opensource/public/boost',
        'src/starfish-media-pipeline/metadata.cpp',
        'src/starfish-media-pipeline/track_metadata.cpp',
        'tools/tests/test_codec_reporting.cpp', '-o', str(executable),
    ]
    try:
        subprocess.run(command, cwd=ROOT, check=True, timeout=180)
        subprocess.run([str(executable)], cwd=ROOT, check=True, timeout=30)
    finally:
        executable.unlink(missing_ok=True)


if __name__ == '__main__':
    main()
