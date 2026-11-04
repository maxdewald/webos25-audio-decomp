#!/usr/bin/env python3
"""Resolve logm handles (mask word + logm struct) used by DSP functions to their module names.

usage: logm_names.py <main_60000000.bin> <litpool_addr_hex>...
Each literal-pool slot holds a pointer; we print the pointed-to words and any C string nearby.
"""
import struct, sys

B = 0x60000000
d = open(sys.argv[1], 'rb').read()

def word(va):
    return struct.unpack_from('<I', d, va - B)[0]

def cstr(va):
    o = va - B
    if not 0 <= o < len(d):
        return None
    e = d.find(b'\0', o, o + 64)
    s = d[o:e]
    return s.decode() if len(s) >= 2 and all(32 <= c < 127 for c in s) else None

for a in sys.argv[2:]:
    slot = int(a, 16)
    p = word(slot)
    print(f'slot {slot:#x} -> {p:#x}')
    if not B <= p < B + len(d):
        print('  (outside image: runtime/bss)')
        continue
    for k in range(8):
        w = word(p + 4 * k)
        print(f'  +{4 * k:#04x}: {w:#010x} {cstr(w) or ""}')
