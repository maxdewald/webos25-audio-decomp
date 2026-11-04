#!/usr/bin/env python3
"""Per-function list of PC-relative string literals (Thumb-2 PIE, Ghidra base 0x10000).

usage: funcstrings.py <elf> <functions.tsv> > out.tsv
"""
import bisect, re, subprocess, sys

BASE = 0x10000
elf, ftsv = sys.argv[1], sys.argv[2]
data = open(elf, 'rb').read()
LLVM = '/opt/homebrew/opt/llvm/bin/'

segs = []
for ln in subprocess.run([LLVM + 'llvm-readelf', '-lW', elf], capture_output=True, text=True).stdout.splitlines():
    p = ln.split()
    if p and p[0] == 'LOAD':
        segs.append((int(p[2], 16), int(p[1], 16), int(p[4], 16)))

def v2o(va):
    return next((va - v + o for v, o, sz in segs if v <= va < v + sz), None)

def cstr(va):
    o = v2o(va)
    if o is None:
        return None
    e = data.find(b'\0', o, o + 300)
    s = data[o:e] if e > 0 else b''
    return s.decode() if len(s) >= 2 and all(32 <= c < 127 or c in (9, 10) for c in s) else None

funcs = []
for ln in open(ftsv):
    a, _, n = ln.rstrip('\n').split('\t', 2)
    funcs.append((int(a, 16) - BASE, n))
funcs.sort()
starts = [f[0] for f in funcs]

dis = subprocess.run([LLVM + 'llvm-objdump', '-d', '--no-show-raw-insn', '--triple=thumbv7-linux-gnueabi',
                      '-j', '.text', elf], capture_output=True, text=True).stdout
regs, out = {}, {}
for ln in dis.splitlines():
    m = re.match(r'\s*([0-9a-f]+):\s+(\S+)\s*(.*)', ln)
    if not m:
        continue
    pc, op, args = int(m.group(1), 16), m.group(2), m.group(3)
    lm = re.match(r'(r\d+|lr|ip), \[pc, #(-?(?:0x)?[0-9a-f]+)\]', args)
    if op.startswith('ldr') and lm:
        o = v2o(((pc + 4) & ~3) + int(lm.group(2), 0))
        if o is not None:
            regs[lm.group(1)] = int.from_bytes(data[o:o + 4], 'little')
        continue
    am = re.match(r'(r\d+), pc$', args)
    if op.startswith('add') and am and am.group(1) in regs:
        s = cstr((regs.pop(am.group(1)) + pc + 4) & 0xffffffff)
        if s is not None:
            i = bisect.bisect_right(starts, pc) - 1
            if i >= 0:
                out.setdefault(funcs[i][1], []).append((pc + BASE, s))

for fn, lst in out.items():
    for pc, s in lst:
        print(f'{fn}\t{pc:08x}\t{s!r}')
