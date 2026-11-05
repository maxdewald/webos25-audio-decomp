#!/usr/bin/env python3
"""Run source-extracted block-manager contract checks; usage: python3 tools/tests/test_block_manager.py."""
from pathlib import Path
import argparse
import re
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'src/kernel/aud/block_manager.c'
SCRATCH = ROOT / 'build/scratch/block-manager-verify'
FUNCTIONS = (
    '_bm_find_block_impl_by_type', '_bm_find_block_in_reserved_list',
    '_bm_find_block_by_index', '_bm_find_block_by_block_info',
    '_bm_get_block_port', 'bm_get_port_forward', '_bm_add_port_connection',
    '_bm_connect_port', '_bm_del_port', '_bm_add_port',
    '_bm_disconnect_block_locked', 'bm_disconnect_block', '_bm_connect_block_locked',
    '_bm_connect_block_reserved', 'bm_connect_block_reserved_out',
    '_bm_disconnect_blocks_without_update', '_bm_reconnect_blocks', 'bm_suspend', 'bm_resume',
)


def definition(source, pattern):
    match = re.search(pattern, source, re.M)
    if not match:
        raise AssertionError(f'missing source definition: {pattern}')
    opening = source.index('{', match.start())
    depth = 1
    end = opening + 1
    while depth:
        if source[end] == '{':
            depth += 1
        elif source[end] == '}':
            depth -= 1
        end += 1
    return source[match.start():end]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize', action='store_true', help='enable host address/undefined sanitizers')
    args = parser.parse_args()
    source = re.sub(r'/\*.*?\*/', '', SOURCE.read_text(), flags=re.S)
    declarations = []
    for name in ('bm_list_block', 'bm_list_block_port', 'bm_list_connected_port',
                 'bm_list_port_forward', 'bm_list_reserved_conncection', 'bm_list_work_farm',
                 'aud_bm_info'):
        declarations.append(definition(source, rf'^struct {name}\n') + ';')
    declarations.append('struct aud_bm_info _bm_info = { };')
    declarations.append('static int _bm_connect_block_locked(struct aud_block_info *, int, struct aud_block_info *, int);')
    declarations.extend(definition(source, rf'^(?:static )?[^\n;{{}}]*\b{re.escape(name)}\s*\(')
                        for name in FUNCTIONS)
    SCRATCH.mkdir(parents=True, exist_ok=True)
    generated = SCRATCH / 'block_manager_scope.inc'
    binary = SCRATCH / 'test_block_manager'
    generated.write_text('\n\n'.join(declarations) + '\n')
    compiler = shutil.which('clang') or shutil.which('cc')
    if not compiler:
        raise SystemExit('A host C compiler is required')
    try:
        flags = ['-fsanitize=address,undefined', '-fno-omit-frame-pointer'] if args.sanitize else []
        subprocess.run([compiler, '-std=gnu11', '-Wall', '-Wextra', '-Werror',
                        '-Wno-unused-parameter', '-Wno-unused-variable', '-Wno-unused-function',
                        *flags, '-I', str(SCRATCH),
                        str(Path(__file__).with_name('test_block_manager_harness.c')),
                        '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    finally:
        generated.unlink(missing_ok=True)
        binary.unlink(missing_ok=True)


if __name__ == '__main__':
    main()
