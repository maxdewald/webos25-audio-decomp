#!/usr/bin/env python3
"""Split an LG audio DSP image into boot stub (@0x4ff80000) and main segment (@0x60000000).

usage: split_fw.py <image> <outdir>
The main segment starts at the first `36 41 00` (entry a1,32) after the 0xff padding.
"""
import os, re, sys

img, out = sys.argv[1], sys.argv[2]
d = open(img, 'rb').read()
pad = re.search(rb'\xff{64,}', d)
main = pad.end()
assert d[main:main + 2] == b'\x36\x41', hex(main)
name = os.path.basename(img).removesuffix('.bin')
os.makedirs(out, exist_ok=True)
open(f'{out}/{name}.boot_4ff80000.bin', 'wb').write(d[:pad.start()])
open(f'{out}/{name}.main_60000000.bin', 'wb').write(d[main:])
print(f'{name}: boot 0..{pad.start():#x}, main @file {main:#x} -> 0x60000000, size {len(d) - main:#x}')
