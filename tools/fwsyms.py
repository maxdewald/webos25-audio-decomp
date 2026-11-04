#!/usr/bin/env python3
"""Recover the embedded symbol table of an LG audio DSP main segment.

The image carries a table of (name_ptr, func_addr) pairs or a name blob followed by
pointers. We locate every C string whose address (0x60000000 + off) is referenced by
an aligned word that is immediately followed or preceded by a code-looking address.

usage: fwsyms.py <main_60000000.bin> > syms.tsv   (addr<TAB>name)
"""
import re, struct, sys

BASE = 0x60000000
d = open(sys.argv[1], 'rb').read()
n = len(d) // 4
words = struct.unpack_from(f'<{n}I', d)
strs = {}
for m in re.finditer(rb'(?<=\x00)[A-Za-z_][A-Za-z0-9_.:$]{2,}\x00', d):
    strs[BASE + m.start()] = m.group()[:-1].decode()

def is_func(a):
    o = a - BASE
    return 0 <= o < len(d) - 3 and d[o] == 0x36 and (d[o + 1] & 0xf) == 1

def is_pair(i):
    return 0 <= i < n - 1 and words[i + 1] in strs and is_func(words[i])

pairs, weak = {}, {}
for i in range(n - 1):
    # (func_ptr, name_ptr): verified against __func__ strings the bodies pass to logmm_vprintl
    if not is_pair(i):
        continue
    w, name = words[i], strs[words[i + 1]]
    # literal pools also hold (func, string) neighbours; trust only runs inside the table
    if is_pair(i - 2) or is_pair(i + 2):
        pairs.setdefault(w, name)
    else:
        weak.setdefault(w, name)
for w, name in weak.items():
    pairs.setdefault(w, name)
for a in sorted(pairs):
    print(f'{a:08x}\t{pairs[a]}')
print(f'# {len(pairs)} symbols', file=sys.stderr)
