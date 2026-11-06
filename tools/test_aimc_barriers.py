#!/usr/bin/env python3
"""Source-extracted AIMC completion contracts; usage: python3 tools/test_aimc_barriers.py [--sanitize]."""
import argparse
import os
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]
NAMES = [
    'handler_thread', 'aimc_handler_sync', 'aimc_handler_destory',
    'aimc_finalize', 'aimc_wait_until_process_header',
    'register_command_handler', 'aimc_unregister_command_handler',
    'aimc_dispatch_command', 'aimc_command_response',
    'aimc_module_create', 'aimc_module_destroy', 'aimc_module_connect',
    'aimc_module_disconnect_output', 'aimc_module_disconnect_input',
    'aimc_send_command', 'aimc_send_command_sync', 'aimc_add_processor_done',
]


def extract(text, name):
    match = re.search(r'^(?:unsigned int|int|void) ' + re.escape(name) + r'\s*\(', text, re.M)
    if not match:
        raise RuntimeError(f'missing function: {name}')
    start = text.index('{', match.start())
    end, depth = start + 1, 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[match.start():end]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize', action='store_true')
    args = parser.parse_args()
    source = (ROOT / 'src/libaa/aimc.c').read_text()
    fixture = (ROOT / 'tools/tests/test_aimc_barriers.c').read_text()
    out = ROOT / 'build/scratch/aimc-barrier-tests'
    out.mkdir(parents=True, exist_ok=True)
    generated, executable = out / 'aimc_scoped.c', out / 'aimc_tests'
    generated.write_text(fixture.replace('/* RECOVERED_FUNCTIONS */', '\n\n'.join(
        extract(source, name) for name in NAMES)))
    command = [os.environ.get('CC', 'cc'), '-std=gnu11', '-Wall', '-Wextra', '-Werror',
               '-Wno-unused-parameter', '-pthread', str(generated), '-o', str(executable)]
    if args.sanitize:
        command[1:1] = ['-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer']
    try:
        print('+', ' '.join(command), flush=True)
        subprocess.run(command, cwd=ROOT, check=True, timeout=60)
        subprocess.run([str(executable)], cwd=ROOT, check=True, timeout=20)
        # Bound deadlocks in isolated children, not the test runner's worker.
        for mode in ('self-sync', 'process-lock'):
            try:
                subprocess.run([str(executable), mode], cwd=ROOT, check=True, timeout=1)
            except subprocess.TimeoutExpired:
                print(f'PASS: callback {mode} blocks (bounded child terminated)', flush=True)
            else:
                raise AssertionError(f'callback {mode} unexpectedly returned')
    finally:
        generated.unlink(missing_ok=True)
        executable.unlink(missing_ok=True)
        dsym = out / 'aimc_tests.dSYM'
        if dsym.exists():
            import shutil
            shutil.rmtree(dsym)
        out.rmdir()


if __name__ == '__main__':
    main()
