#!/usr/bin/env python3
"""Usage: python3 tools/test_sink_state.py

Execute recovered sink state/finalize functions with host mocks, not GStreamer
or LG binaries. Parent-state effects are explicit mocks, not lock guarantees.
"""
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

import sys
sys.path.insert(0, str(Path(__file__).resolve().parent / 'tests'))
from test_sink_context import function

ROOT = Path(__file__).resolve().parent.parent


def main():
    cc = shutil.which(os.environ.get('CC', 'cc'))
    if cc is None:
        raise SystemExit('C compiler missing')
    source = (ROOT / 'src/libgstlgaudiosink/render_props.c').read_text()
    # Retain complete declarations, not just the brace-delimited body.
    import re
    extracted = []
    for name in ('gst_lg_audio_render_change_state', 'gst_lg_audio_render_finalize'):
        body = function(source, name)
        pattern = r'^(?:GstStateChangeReturn|void)\s+' + name + r'\s*\([^;{]*\)'
        match = re.search(pattern, source, re.M)
        assert match, name
        extracted.append(match.group() + '\n' + body)
    bodies = '\n\n'.join(extracted)
    fixture = (ROOT / 'tools/tests/test_sink_state.c').read_text()
    with tempfile.TemporaryDirectory(prefix='sink-state-') as temporary:
        src = Path(temporary) / 'state.c'
        output = Path(temporary) / 'state'
        src.write_text(fixture.replace('/* RECOVERED_FUNCTIONS */', bodies))
        for label, flags in [('strict', ['-O2']),
                             ('sanitized', ['-O1', '-fsanitize=address,undefined',
                                            '-fno-sanitize-recover=all'])]:
            command = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror',
                       '-Wno-unused-parameter', '-Wno-unused-variable',
                       *flags, str(src), '-o', str(output), '-lm']
            print(label + ': ' + ' '.join(command), flush=True)
            subprocess.run(command, cwd=ROOT, check=True, timeout=60)
            subprocess.run([str(output)], cwd=ROOT, check=True, timeout=15)


if __name__ == '__main__':
    main()
