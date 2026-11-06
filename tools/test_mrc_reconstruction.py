#!/usr/bin/env python3
"""Run calculator regression tests with ASan/UBSan: python3 tools/test_mrc_reconstruction.py."""
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def main():
    compiler = os.environ.get('CXX', 'clang++')
    flags = ['-std=c++14', '-Wall', '-Wextra', '-Wno-unused-parameter',
             '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
             '-Isrc/libmedia-resource-calculator',
             '-Ilg-opensource/public/media-resource-calculator/calculator']
    source = 'src/libmedia-resource-calculator/'
    core = [source + name for name in ('resource.cpp', 'calculator.cpp',
                                     'calculator_destructor.cpp', 'video_info.cpp')]
    cases = [('public', [source + 'resource.cpp'], 'tools/test_mrc_public.cpp'),
             ('core', core, 'tools/test_mrc_calculator.cpp'),
             ('c-api', core + [source + 'c_api.cpp'], 'tools/test_mrc_c_api.cpp')]
    (ROOT / 'build/scratch').mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='calculator-tests-', dir=ROOT / 'build/scratch') as temporary:
        for name, sources, test in cases:
            executable = Path(temporary) / name
            command = [compiler, *flags, *sources, test, '-o', str(executable)]
            print('+ ' + ' '.join(command), flush=True)
            subprocess.run(command, cwd=ROOT, check=True, timeout=120)
            subprocess.run([str(executable)], cwd=ROOT, check=True, timeout=30)


if __name__ == '__main__':
    main()
