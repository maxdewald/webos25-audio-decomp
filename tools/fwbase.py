#!/usr/bin/env python3
"""Infer the load base of a raw Xtensa firmware image: base b maximises the number of
aligned 32-bit words w for which (w - b) is the file offset of a printable C string.

usage: fwbase.py <image>
"""
import re, sys
import numpy as np

d = open(sys.argv[1], 'rb').read()
starts = np.array([m.start() for m in re.finditer(rb'(?<=\x00)[\x20-\x7e]{5,}\x00', d)], dtype=np.int64)
words = np.frombuffer(d[:len(d) // 4 * 4], dtype='<u4').astype(np.int64)
words = words[(words > 0x10000000) & (words < 0xf0000000)]
uw = np.unique(words)
rng = np.random.default_rng(0)
sw = rng.choice(uw, min(3000, len(uw)), replace=False)
ss = rng.choice(starts, min(3000, len(starts)), replace=False)
vals, cnt = np.unique((sw[:, None] - ss[None, :]).ravel(), return_counts=True)
cands = vals[np.argsort(-cnt)[:60]]
sset = np.sort(starts)
res = []
for b in cands:
    off = words - b
    idx = np.clip(np.searchsorted(sset, off), 0, len(sset) - 1)
    res.append((int((sset[idx] == off).sum()), int(b)))
print(f'strings={len(starts)} pointer-like words={len(words)}')
for score, b in sorted(res, reverse=True)[:10]:
    print(f'base {b:#010x}  hits {score}')
