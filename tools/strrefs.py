#!/usr/bin/env python3
"""Resolve PC-relative string literals loaded inside an ARM/Thumb function range.

usage: strrefs.py <elf> <start_hex> <end_hex> [ghidra_base_hex=10000]
Addresses are Ghidra addresses; the base is subtracted for PIE images.
"""
import re, subprocess, sys

base = int(sys.argv[4], 16) if len(sys.argv) > 4 else 0x10000
elf, start, end = sys.argv[1], int(sys.argv[2], 16) - base, int(sys.argv[3], 16) - base
data = open(elf, 'rb').read()

def vaddr_to_off():
    out = subprocess.run(['/opt/homebrew/opt/llvm/bin/llvm-readelf', '-lW', elf],
                         capture_output=True, text=True).stdout
    segs = []
    for ln in out.splitlines():
        p = ln.split()
        if p and p[0] == 'LOAD':
            segs.append((int(p[2], 16), int(p[1], 16), int(p[4], 16)))
    return lambda va: next((va - v + o for v, o, sz in segs if v <= va < v + sz), None)

v2o = vaddr_to_off()

def cstr(va):
    o = v2o(va)
    if o is None or o >= len(data):
        return None
    e = data.find(b'\0', o, o + 200)
    s = data[o:e]
    if len(s) >= 2 and all(32 <= c < 127 for c in s):
        return s.decode()
    return None

dis = subprocess.run(['/opt/homebrew/opt/llvm/bin/llvm-objdump', '-d', '--triple=thumbv7-linux-gnueabi',
                      f'--start-address={start:#x}', f'--stop-address={end:#x}', elf],
                     capture_output=True, text=True).stdout
regs = {}
for ln in dis.splitlines():
    m = re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2} ?)+\s+(\S+)\s*(.*)', ln)
    if not m:
        continue
    pc, op, args = int(m.group(1), 16), m.group(2), m.group(3)
    lm = re.match(r'(r\d+|lr|ip), \[pc, #(-?(?:0x)?[0-9a-f]+)\]', args)
    if op.startswith('ldr') and lm:
        lit = ((pc + 4) & ~3) + int(lm.group(2), 0)
        o = v2o(lit)
        if o is not None:
            regs[lm.group(1)] = int.from_bytes(data[o:o + 4], 'little')
        continue
    am = re.match(r'(r\d+), pc$', args)
    if op.startswith('add') and am and am.group(1) in regs:
        va = (regs.pop(am.group(1)) + pc + 4) & 0xffffffff
        s = cstr(va)
        if s:
            print(f'{pc + base:08x}  {va + base:08x}  {s!r}')
