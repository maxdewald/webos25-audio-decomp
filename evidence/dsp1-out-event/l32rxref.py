#!/usr/bin/env python3
"""Find L32R sites (any byte offset, brute force) loading the given literal slots; print enclosing symbol.

usage: l32rxref.py <slot_hex>...
"""
import bisect, struct, sys
from xtlist import img, syms, BASE

want = {int(a, 16) for a in sys.argv[1:]}
starts = sorted(syms)
for o in range(len(img) - 3):
    b0 = img[o]
    if b0 & 15 != 1:
        continue
    p = BASE + o
    imm = struct.unpack_from('<H', img, o + 1)[0]
    slot = (((p + 3) & ~3) + ((imm | 0xFFFF0000) << 2)) & 0xFFFFFFFF
    if slot in want:
        f = starts[bisect.bisect_right(starts, p) - 1]
        print(f'{p:08x}  slot {slot:08x}  a{b0 >> 4}  in {syms[f]}+{p - f:#x}')
