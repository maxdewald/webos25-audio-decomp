#!/usr/bin/env python3
"""Scalar CFG of a range: branch targets, unreached leaders, FLIX bundles and j-back-edges to bundle+8.

usage: cfg.py <start_hex> <end_hex>
"""
import re, sys
from xtlist import sweep

s, e = int(sys.argv[1], 16), int(sys.argv[2], 16)
ins = list(sweep(s, e))
addrs = [p for p, *_ in ins]
targets = {}
for p, raw, mn, ops, note in ins:
    m = re.search(r'0x([0-9a-f]{8})$', ops)
    if m and (mn.startswith(('b', 'j', 'loop')) and mn not in ('break',)):
        targets.setdefault(int(m.group(1), 16), []).append((p, mn))
term = ('j', 'retw', 'retw.n', 'ill', 'jx', 'ret')
flix = [p for p, raw, mn, *_ in ins if mn == 'flix']
print('# FLIX bundles')
for p, raw, mn, ops, note in ins:
    if mn == 'flix':
        back = [(src, m) for src, m in targets.get(p + 8, [])]
        print(f'{p:08x} {raw.hex()} lo={raw[:4][::-1].hex()} hi={raw[4:][::-1].hex()} jumps_to_next={back}')
print('# leaders after terminators with no scalar in-edge')
for i, (p, raw, mn, ops, note) in enumerate(ins[:-1]):
    if mn in term:
        q = ins[i + 1][0]
        if q not in targets:
            print(f'{q:08x} after {mn} @{p:08x}')
print('# targets not on instruction boundary')
for t in sorted(targets):
    if s <= t < e and t not in addrs:
        print(f'{t:08x} from {targets[t]}')
