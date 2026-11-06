#!/usr/bin/env python3
"""Test recovered gain-off bytes, control state, mixer slots and local raw anchors.

Usage: python3 tools/tests/test_bypass_gain.py
No DSP arithmetic emulation; all builds stay in build/scratch/broad/gain/.
"""
import hashlib
import os
from pathlib import Path
import shutil
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/scratch/broad/gain'
IMAGE = ROOT / 'build/fw/audio_a0_dsp2.main_60000000.bin'


def run(args):
    print('+', ' '.join(map(str, args)), flush=True)
    subprocess.run(args, cwd=ROOT, check=True)


def anchors():
    if not IMAGE.exists():
        print('SKIP raw firmware anchors: run make decomp-dsp to supply image')
        return
    image = IMAGE.read_bytes()
    assert hashlib.sha256(image).hexdigest() == 'd1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd'
    exact = {
        # Gain samples: loads, repeated scalar output increment and stores.
        0x6009dc03: '68726901',
        0x6009dc07: 'a6154d',
        0x6009cf66: '0e18b0f041f27f0d',
        0x6009de45: '26130b0c055962',
        0x6009dc0a: '0411b6',
        0x6009dc14: '9f25030401dd62eb',
        0x6009dc1c: '2f43203f8020fb08',
        0x6009dc24: '3ef500ff202ffbc7',
        0x6009dc2f: '8904',
        0x6009dc40: '4f440000012062eb',
        0x6009dc48: '040884040a2b3430253904',
        # Explicit command reads raw gain then enable, not reversed.
        0x6009b474: '522400322401',
        0x6009b480: '30b32050c520a59b02',
        # Constructor passes disable, retaining an existing active gain word.
        0x6009c182: 'c18a13b2a000',
        0x6009c19f: '25ca01',
        # Proven scalar SALTU and no-main TODO_MIXING bypass branch.
        0x60092ff6: '402262',
        0x60092e88: '0e14b0d04114400d',
        # Clamp boundary is 0x7ffe, not 0x7fff.
        0x60092d17: '913e35',
        0x60092d1f: '37a907',
    }
    for address, text in exact.items():
        raw = bytes.fromhex(text)
        offset = address - 0x60000000
        assert image[offset:offset+len(raw)] == raw, hex(address)
    def word(address):
        return struct.unpack_from('<I', image, address - 0x60000000)[0]
    assert word(0x60060210) == 0x7ffe
    assert word(0x60060fac) == 0x800000
    assert word(0x60060ea4) == 0x50040008
    table = [
        ('in-sampling-rate',2,0x188,0,0x6009c6d0),
        ('in-channels',2,0x180,0,0x6009c744),
        ('in-bit-per-sample',2,0x184,0,0x6009c7b4),
        ('out-sampling-rate',2,0x19c,0,0x6009ca30),
        ('out-channels',2,0x194,0,0x6009ca98),
        ('out-bit-per-sample',2,0x198,0,0x6009c9c0),
        ('generate-lfe',1,0x1a4,0,0x6009c900),
        ('out-samples',2,0x1a0,0,0x6009cb08),
        ('over-protect',2,0xf8,0,0x6009cba0),
        ('running-mode',2,0x118,0,0x6009cc28),
        ('gain',2,0x1a8,0,0x6009cc6c),
        ('gain-enable',1,0x1a5,0,0x6009ccf8),
        ('acmod',1,0x1ac,0,0x6009cd64),
        ('endian',2,0x18c,0,0x6009c824),
        ('sign',2,0x190,0,0x6009c890),
        ('available',1,0x1b0,0,0x6009cdb4),
    ]
    assert word(0x60103600) == 0x60103630
    for index, (name, typ, offset, flags, setter) in enumerate(table):
        addr = 0x60103630 + index*0x28
        ptr = word(addr)-0x60000000
        assert image[ptr:image.index(b'\0',ptr)].decode() == name
        assert [word(addr+i*4) for i in range(1,5)] == [typ,offset,flags,setter]
    print(f'PASS raw identity, {len(exact)} instruction anchors, 16 property descriptors')


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    cc = shutil.which(os.environ.get('CC','cc'))
    if not cc:
        raise SystemExit('C compiler missing')
    run([cc, '-std=c11','-Wall','-Wextra','-Werror','-fsyntax-only',
         *[ROOT / f'src/dsp2/{name}.c' for name in ('rft','pcmcvt','mixer')]])
    anchors()
    mixer = (ROOT / 'src/dsp2/mixer.c').read_text()
    for address in ('600925b8', '60092660'):
        assert f'/* PARTIAL @0x{address}' in mixer
        assert not any(line.startswith(f'/* @0x{address}') for line in mixer.splitlines()), \
            'Incomplete DSP helpers must not increase tagged-byte coverage'
    linker = '-Wl,-dead_strip' if os.uname().sysname == 'Darwin' else '-Wl,--gc-sections'
    for name in ('test_bypass_gain','test_bypass_gain_mixer'):
        binary = OUT / name
        run([cc, '-std=c11','-Wall','-Wextra','-Werror','-fsanitize=address,undefined',
             '-ffunction-sections','-fdata-sections',linker,
             ROOT / f'tools/tests/{name}.c','-o',binary])
        try:
            run([binary])
        finally:
            binary.unlink(missing_ok=True)
    print('PASS recovered gain/control/mixer behavior (not Xtensa execution)')


if __name__ == '__main__':
    main()
