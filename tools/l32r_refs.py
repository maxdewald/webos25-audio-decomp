#!/usr/bin/env python3
"""Find every L32R that loads a given literal slot (brute force over byte offsets).

usage: l32r_refs.py <main_60000000.bin> <slot_hex>...
"""
import struct, sys

B = 0x60000000
d = open(sys.argv[1], 'rb').read()
slots = {int(a, 16) for a in sys.argv[2:]}
for p in range(len(d) - 3):
    if d[p] & 0xf != 1:
        continue
    imm = struct.unpack_from('<H', d, p + 1)[0]
    t = ((B + p + 3) & ~3) + (((imm | 0xffff0000) - (1 << 32)) << 2)
    if t in slots:
        print(f'{B + p:#x} a{d[p] >> 4} <- [{t:#x}]')
