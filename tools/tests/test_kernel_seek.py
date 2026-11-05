#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_kernel_seek.py; source-extracted seek/clock/FIRST contracts."""
from pathlib import Path
import os
import re
import subprocess

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/scratch/seek-chain-tests'


def definition(text, name):
    pattern = (r'^(?:static )?(?:struct \w+ \*|[\w ]+ )' + re.escape(name)
               + r'\s*\([^;{}]*\)\s*\n\{')
    matches = list(re.finditer(pattern, text, re.M))
    assert len(matches) == 1, (name, len(matches))
    start = matches[0].start()
    opening = text.index('{', start)
    depth, end = 1, opening + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[start:end]


def extract(relative, names):
    source = (ROOT / relative).read_text()
    source = re.sub(r'/\*.*?\*/', '', source, flags=re.S)
    return '\n\n'.join(definition(source, name) for name in names)


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    scopes = {
        'clock': [('src/kernel/aud/notifier.c', ['notifier_set_play_speed', 'notifier_get_play_speed']),
                  ('src/kernel/aud/clock.c', ['_get_output_info', '_update_output_info',
                   '_update_output_sample_rate', 'aud_clock_update_output', 'aud_clock_set_input_index']),
                  ('src/kernel/aud/output.c', ['output_set_format', '_set_format',
                   '_adec_codec_convert_output_codec', '_cb_output_status']),
                  ('src/kernel/aud/alsa_decoder.c', ['sndout_decoder_is_hdmi',
                   'decoder_set_output_mode', 'decoder_set_multi_pcm_out'])],
        'first': [('src/dsp1/module.c', ['foreach_sync', 'adjust_thread_priority',
                   'del_input_port', 'aud_module_disconnect', 'aud_module_connect', 'aud_module_chain_locked',
                   'aud_module_event_flush_locked', 'aud_module_event_disconnect_locked',
                   'aud_module_event_first_au_locked', 'aud_module_event_last_au_locked',
                   'aud_module_event_locked', 'aud_module_event'])],
        'noti': [('src/kernel/aud/kaimc.c', ['noti_handler_work', 'cb_func_local']),
                 ('src/kernel/aud/adec_imc.c', ['process_named_event_handle'])],
        'hal': [('src/kernel/aud/hal_clock.c', ['_aud_get_frpll_fout_settings',
                  'aud_set_earc_frpll', 'aud_hal_set_arc_sample_rate'])],
    }
    for scope, sources in scopes.items():
        generated = OUT / f'{scope}_under_test.inc'
        binary = OUT / f'test_{scope}'
        content = '\n\n'.join(extract(path, names) for path, names in sources)
        if scope == 'hal':
            source = (ROOT / 'src/kernel/aud/hal_clock.c').read_text()
            table = source[source.index('struct frpll_fout_settings\n'):
                           source.index('static struct frpll_fout_settings *_aud_get_frpll_fout_settings')]
            content = table + '\n' + content
        generated.write_text(content + '\n')
        command = [os.environ.get('CC', 'cc'), '-std=gnu11', '-Wall', '-Wextra', '-Werror',
                   '-Wno-unused-parameter', '-Wno-unused-function', '-Wno-unused-variable',
                   '-Wno-sign-compare',
                   '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-g',
                   '-I', str(OUT), str(ROOT / f'tools/tests/test_kernel_seek_{scope}.c'),
                   '-o', str(binary)]
        try:
            subprocess.run(command, check=True, cwd=ROOT)
            subprocess.run([str(binary)], check=True, cwd=ROOT,
                           env={**os.environ, 'UBSAN_OPTIONS': 'halt_on_error=1'})
        finally:
            generated.unlink(missing_ok=True)
            binary.unlink(missing_ok=True)


if __name__ == '__main__':
    main()
