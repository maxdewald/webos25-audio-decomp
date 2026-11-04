#!/usr/bin/env python3
"""Score candidate instruction-length rules for op0=0xE/0xF (FLIX) by linear sweep between
known function entries: a rule is right when the sweep stays in sync and reaches the next
entry, allowing 0..2 bytes of overshoot from zero alignment padding.

usage: flixlen.py <main_60000000.bin> <syms.tsv>
"""
import itertools, sys

BASE = 0x60000000
d = open(sys.argv[1], 'rb').read()
ents = sorted({int(l.split('\t')[0], 16) - BASE for l in open(sys.argv[2]) if not l.startswith('#')})
pairs = [(a, b) for a, b in zip(ents, ents[1:]) if b - a <= 0x2000]

def is_ret(p):
    return d[p:p + 3] in (b'\x90\x00\x00', b'\x80\x00\x00') or d[p:p + 2] in (b'\x1d\xf0', b'\x0d\xf0')

def score(rule):
    ok = 0
    for a, b in pairs:
        p, hit = a, False
        while p < b:
            if b - p <= 16 and is_ret(p):
                hit = True
            p += rule(d[p])
        ok += hit
    return ok

def base(b0):
    op0 = b0 & 0xf
    return 3 if op0 < 8 else 2 if op0 < 0xe else None

if len(sys.argv) > 3:
    # coordinate descent: per (op0, high nibble) length in {8, 16}
    tab = {(o, h): 8 for o in (0xe, 0xf) for h in range(16)}
    rule = lambda b0: base(b0) or tab[(b0 & 0xf, b0 >> 4)]
    best = score(rule)
    for _ in range(2):
        for k in tab:
            for L in (4, 8, 12, 16):
                old = tab[k]
                tab[k] = L
                s = score(rule)
                if s > best:
                    best = s
                else:
                    tab[k] = old
    print(best, {f'{o:x}/{h:x}': L for (o, h), L in tab.items() if L != 8})
    sys.exit()

res = []
for le, lf in itertools.product((3, 4, 8, 11, 16), repeat=2):
    res.append((score(lambda b0, le=le, lf=lf: base(b0) or (le if b0 & 0xf == 0xe else lf)), f'e={le} f={lf}'))
for le, f0, f1 in itertools.product((8, 16), (8, 11, 16), (8, 11, 16)):
    res.append((score(lambda b0, le=le, f0=f0, f1=f1: base(b0) or (le if b0 & 0xf == 0xe else (f1 if b0 & 0x80 else f0))),
                f'e={le} f(b7=0)={f0} f(b7=1)={f1}'))
print(f'{len(pairs)} function pairs')
for s, n in sorted(res, reverse=True)[:12]:
    print(s, n)
