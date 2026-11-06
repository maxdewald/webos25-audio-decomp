#!/usr/bin/env python3
"""Usage: python3 tools/test_registry_lifetime.py; source-extracted registry lifetime tests.

Runs reconstructed C with deterministic dependency/failure doubles, not the ARM
binary or a concurrency proof. Generated source/binaries stay in build/scratch.
"""
import hashlib
import os
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'build/scratch/registry-lifetime-tests'


def extract(source, name):
    source = re.sub(r'/\*.*?\*/', '', source, flags=re.S)
    match = re.search(r'^([^;{}\n]*\b' + re.escape(name) +
                      r'\s*\([^;{}]*\))\s*\n\{', source, re.M)
    if not match:
        raise AssertionError(f'missing function {name}')
    start = source.index('{', match.start())
    depth, end = 1, start + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[match.start():end]


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    registry = (ROOT / 'src/libaa/registry.c').read_text()
    module = (ROOT / 'src/libaa/module.c').read_text()
    names = ['aud_mregistry_add_class', 'ModuleRegistry_Register',
             'ModuleRegistry_SearchInstByUid', 'ModuleRegistry_SearchInstByName',
             'ModuleRegistry_SearchModuleImpl', 'get_empty_internal_index',
             'aud_registry_create_instance_param', 'aud_registry_create_instance',
             'regcmd_create_module', 'aud_registry_destroy_instance',
             'ModuleRegistry_ProcessCmd', 'aud_connector_connect',
             'aud_connector_disconnect', 'Connector_Cmd', 'Connector_Create',
             'Connector_Destroy', 'ModuleRegistry_Cleanup']
    prefix = registry[registry.index('#define REGISTRY_HANDLER'):registry.index('/* @0x274a4')]
    for name in ['ModuleRegistry_SearchInstByUid', 'ModuleRegistry_SearchInstByName',
                 'aud_registry_destroy_instance', 'Connector_Cmd']:
        body = extract(registry, name)
        assert not re.search(r'\b(?:pthread_mutex_lock|aud_object_get|aud_object_put|'
                             r'aimc_wait_until_process_header)\s*\(', body), name
    bodies = [extract(module, n) for n in ['aud_module_constructor', 'aud_module_destructor']]
    bodies += [extract(registry, n) for n in names]
    (OUT / 'registry_under_test.inc').write_text(prefix + '\n\n' + '\n\n'.join(bodies))
    binary = ROOT / 'orig/usr/lib/libaa.so.2.0.0'
    if binary.exists():
        assert hashlib.sha256(binary.read_bytes()).hexdigest() == (
            '301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2')
    command = [os.environ.get('CC', 'cc'), '-std=gnu11', '-Wall', '-Wextra', '-Werror',
               '-Wno-unused-parameter', '-fsanitize=address,undefined', '-g',
               '-I', str(OUT), str(ROOT / 'tools/tests/test_registry_lifetime.c'),
               '-o', str(OUT / 'test_registry_lifetime')]
    subprocess.run(command, cwd=ROOT, check=True)
    subprocess.run([str(OUT / 'test_registry_lifetime')], cwd=ROOT, check=True,
                   env={**os.environ, 'UBSAN_OPTIONS': 'halt_on_error=1'})
    # Shipped allocator skips its termination cursor when wrapping to occupied
    # index 1, then increments to 2. Isolate the actual loop with a hard bound.
    try:
        subprocess.run([str(OUT / 'test_registry_lifetime'), '--full-cursor-one'],
                       cwd=ROOT, check=True, timeout=2)
    except subprocess.TimeoutExpired:
        print('registry lifetime: full registry/cursor=1 nontermination confirmed (2s bound)')
    else:
        raise AssertionError('expected shipped full-registry cursor=1 loop')


if __name__ == '__main__':
    main()
