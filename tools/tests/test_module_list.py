#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_module_list.py; compile restored GPL functions with kernel mocks."""
from pathlib import Path
import os
import re
import subprocess

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/scratch/module-list-verify'
OUT.mkdir(parents=True, exist_ok=True)
source = (ROOT / 'src/kernel/aud/module_list.c').read_text()


def block(start):
    opening = source.index('{', start)
    depth = 1
    i = opening + 1
    # GPL source has no brace literals in the selected functions/types.
    while depth:
        depth += (source[i] == '{') - (source[i] == '}')
        i += 1
    return source[start:i]


types = ['aud_mod_owner', 'jumper', 'port', 'aud_mod_impl_port_buf_def',
         'aud_mod_inst', 'aud_mod_impl', 'aud_proc_info', 'aud_impl_helper']
functions = ['unregister_impl', 'unregister_processor', 'aud_modlist_unregister_processor',
             'put_instance', 'aud_modlist_del_owner', 'get_input_port', 'get_output_port',
             'disconnect_in_processor', 'connect_in_processor', 'jumper_inter_output_glue',
             'jumper_inter_input_glue', 'connect_between_processor', 'destroy_jumper',
             'inst_by_uid', 'owner_inst_by_uid', 'aud_modlist_disconnect_inst_output',
             'aud_modlist_disconnect_inst_input', 'aud_inst_connect',
             'aud_inst_disconnect_output', 'aud_inst_disconnect_input', 'aud_modlist_connect_inst']
parts = ['/* Extracted from restored GPL source by tools/tests/test_module_list.py. */']
parts.extend(block(source.index('struct ' + name + '\n{')) + ';' for name in types)
bodies = []
for name in functions:
    pattern = re.compile(r'^(?:static )?(?:struct \w+ \*|(?:int|void|unsigned int) )' + name + r' \(', re.M)
    matches = list(pattern.finditer(source))
    assert len(matches) == 1, name
    bodies.append(block(matches[0].start()))
parts.extend(body[:body.index('{')].rstrip() + ';' for body in bodies)
parts.append('#include "test_module_list_mocks.inc"')
parts.extend(bodies)
(OUT / 'module_list_under_test.inc').write_text('\n\n'.join(parts) + '\n')
compiler = os.environ.get('CC', 'cc')
command = [compiler, '-std=gnu11', '-Wall', '-Wextra', '-Werror',
           '-Wno-unused-parameter', '-Wno-missing-field-initializers',
           '-fsanitize=address,undefined', '-g', '-I', str(OUT), '-I', str(ROOT / 'tools/tests'),
           str(ROOT / 'tools/tests/test_module_list.c'), '-o', str(OUT / 'test_module_list')]
subprocess.run(command, cwd=ROOT, check=True)
subprocess.run([str(OUT / 'test_module_list')], cwd=ROOT, check=True,
               env={**os.environ, 'UBSAN_OPTIONS': 'halt_on_error=1'})
