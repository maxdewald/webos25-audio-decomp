#!/usr/bin/env python3
"""Verify DILE stubs: python3 tools/tests/test_dile_audio.py [--sanitize] [--match]."""
import argparse
import importlib.util
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
TARGETS = ('libdile_audio_direct', 'libdile_audio_pcm')


def instruction_matches():
    spec = importlib.util.spec_from_file_location('progress', ROOT / 'tools/progress.py')
    progress = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = progress
    spec.loader.exec_module(progress)
    targets = [target for target in progress.load_targets() if target.name in TARGETS]
    assert len(targets) == 2, 'Both DILE targets must be configured'
    assert progress.score_all(targets), 'ARM compilation failed'
    for target in targets:
        result = progress.summary(target)
        print(f'{target.name}: {result}', flush=True)
        assert result['coverage_pct'] == 100.0, result
        assert result['match_status'] == 'measured', result
        for function in sorted(target.funcs.values(), key=lambda function: function.addr):
            assert function.match is not None, function.name
            print(f'  {function.name} @0x{function.addr:x}: '
                  f'{function.match:.3f}%', flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize', action='store_true')
    parser.add_argument('--match', action='store_true', help='also compile/compare ARM instructions')
    args = parser.parse_args()
    output = ROOT / 'build/scratch/dile-audio-tests'
    output.mkdir(parents=True, exist_ok=True)
    executable = output / 'test_dile_audio'
    command = [os.environ.get('CXX', 'clang++'), '-std=c++14', '-O2', '-Wall', '-Wextra',
               '-Werror', '-Wpedantic', '-Wconversion', '-Wsign-conversion', '-Wshadow',
               str(ROOT / 'tools/tests/test_dile_audio.cpp'), '-o', str(executable)]
    if args.sanitize:
        command[1:1] = ['-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer']
    try:
        subprocess.run(command, cwd=ROOT, check=True)
        subprocess.run([str(executable)], cwd=ROOT, check=True)
        if args.match:
            instruction_matches()
    finally:
        executable.unlink(missing_ok=True)
        debug_symbols = executable.with_suffix('.dSYM')
        if debug_symbols.is_dir():
            shutil.rmtree(debug_symbols)
        output.rmdir()


if __name__ == '__main__':
    main()
