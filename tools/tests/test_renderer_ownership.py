#!/usr/bin/env python3
"""Host-test actual scoped source bodies: python3 tools/tests/test_renderer_ownership.py [--sanitize]."""
import argparse
import os
from pathlib import Path
import re
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/scratch/broad/ownership'


def function(text, name):
    match = re.search(r'^(?:static )?(?:int|void|SINT32) ' + re.escape(name) + r'\s*\(', text, re.M)
    assert match, name
    start = text.index('{', match.start())
    depth, end = 1, start + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[match.start():end]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize', action='store_true')
    args = parser.parse_args()
    OUT.mkdir(parents=True, exist_ok=True)
    media = (ROOT / 'src/kernel/aud/aapi_media.c').read_text()
    driver = (ROOT / 'src/kernel/aud/driver.c').read_text()
    renderer = (ROOT / 'src/kernel/aud/renderer.c').read_text()
    max_slots = re.search(r'^#define AREN_MAX \d+', media, re.M).group()
    macro = media[media.index('#define ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK'):media.index('struct str_table')]
    selected = driver[driver.index('case AREN_IOW_SET_INDEX:'):driver.index('case AREN_IO_ADD_QUEUE:')]
    body = selected[selected.index('{') + 1:selected.rindex('}')].replace('break;', 'return ret;')
    generated = max_slots + '\n' + macro
    for name in ['param_src', 'param_gain']:
        generated += re.search(r'struct renderer_param ' + name + r'\[\] = \{.*?^};',
                               renderer, re.M | re.S).group() + '\n'
    generated += function(renderer, 'renderer_run_param') + '\n'
    generated += '\n'.join(function(renderer, n) for n in ['renderer_set_default',
        'renderer_create_bypass_pipeline', 'renderer_get_uid_pcm_bypass_head',
        'renderer_get_uid_es_bypass_head', 'renderer_set_pcm_bypass_rts_out_channels'])
    generated += '\n' + '\n'.join(function(media, n) for n in ['aren_open', 'aren_close', 'aren_get_uid'])
    generated += '\n' + '\n'.join(function(driver, n) for n in ['driver_aren_open', 'driver_aren_close'])
    generated += '\nstatic int select_index(struct file *filp, int index) {\n'
    generated += 'struct audio_renderer_info *info = filp->private_data;\nstruct {int index;} args = {index}; int ret;\n'
    generated += body + '\n}\n'
    inc = OUT / 'renderer_ownership_scoped.inc'
    executable = OUT / 'test_renderer_ownership'
    inc.write_text(generated)
    command = [os.environ.get('CC', 'cc'), '-std=gnu11', '-Wall', '-Wextra', '-Werror',
               '-Wno-unused-parameter', '-Wno-unused-variable', '-Wno-unused-but-set-variable',
               '-Wno-missing-field-initializers',
               '-I' + str(OUT),
               str(ROOT / 'tools/tests/test_renderer_ownership.c'), '-o', str(executable)]
    if args.sanitize:
        command[1:1] = ['-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer']
    try:
        subprocess.run(command, cwd=ROOT, check=True)
        subprocess.run([str(executable)], cwd=ROOT, check=True)
    finally:
        inc.unlink(missing_ok=True)
        executable.unlink(missing_ok=True)
        shutil.rmtree(str(executable) + '.dSYM', ignore_errors=True)


if __name__ == '__main__':
    main()
