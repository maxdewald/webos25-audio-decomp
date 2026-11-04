#!/usr/bin/env python3
"""Find CALLn / L32R-literal references into an address window of a raw Xtensa image.

usage: callers.py <main_60000000.bin> <lo_hex> <hi_hex>
Brute-force over every byte offset, so expect a few false positives.
"""
import struct, sys

B = 0x60000000
d = open(sys.argv[1], 'rb').read()
lo, hi = int(sys.argv[2], 16), int(sys.argv[3], 16)
for p in range(len(d) - 3):
    b0 = d[p]
    if b0 & 0xf == 5 and (b0 >> 4) & 3:
        off = (b0 >> 6) | (d[p + 1] << 2) | (d[p + 2] << 10)
        if off & 0x20000:
            off -= 0x40000
        t = ((B + p) & ~3) + (off << 2) + 4
        if lo <= t < hi:
            print(f'call{((b0 >> 4) & 3) * 4} {B + p:#x} -> {t:#x}')
for i in range(0, len(d) - 3, 4):
    w = struct.unpack_from('<I', d, i)[0]
    if lo <= w < hi:
        print(f'literal {B + i:#x} = {w:#x}')
