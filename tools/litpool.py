#!/usr/bin/env python3
"""Resolve `ldr rX,[pc,#N]` + `add rX,pc` string pointers at raw (file-vaddr) PCs.

usage: litpool.py <elf> <start_hex> <end_hex>   (raw ELF vaddrs, no Ghidra base)
"""
import re, subprocess, sys

elf, start, end = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
data = open(elf, 'rb').read()
LLVM = '/opt/homebrew/opt/llvm/bin/'
segs = [(int(p[2], 16), int(p[1], 16), int(p[4], 16))
        for p in (l.split() for l in subprocess.run([LLVM + 'llvm-readelf', '-lW', elf],
                  capture_output=True, text=True).stdout.splitlines()) if p and p[0] == 'LOAD']
v2o = lambda va: next((va - v + o for v, o, sz in segs if v <= va < v + sz), None)

def cstr(va):
    o = v2o(va)
    if o is None:
        return None
    s = data[o:data.find(b'\0', o, o + 200)]
    return s.decode(errors='replace')

dis = subprocess.run([LLVM + 'llvm-objdump', '-d', '--no-show-raw-insn', '--triple=thumbv7-linux-gnueabi',
                      f'--start-address={start:#x}', f'--stop-address={end:#x}', elf],
                     capture_output=True, text=True).stdout
regs = {}
for ln in dis.splitlines():
    m = re.match(r'\s*([0-9a-f]+):\s+(\S+)\s*(.*)', ln)
    if not m:
        continue
    pc, op, args = int(m.group(1), 16), m.group(2), m.group(3)
    lm = re.match(r'(r\d+|lr|ip), \[pc, #(-?(?:0x)?[0-9a-f]+)\]', args)
    if op.startswith('ldr') and lm:
        regs[lm.group(1)] = int.from_bytes(data[v2o(((pc + 4) & ~3) + int(lm.group(2), 0)):][:4], 'little')
        continue
    am = re.match(r'(r\d+), pc$', args)
    if op.startswith('add') and am and am.group(1) in regs:
        va = (regs.pop(am.group(1)) + pc + 4) & 0xffffffff
        print(f'{pc:08x} {va:08x} {cstr(va)!r}')
    elif op.startswith('bl'):
        print(f'{pc:08x} {op} {args.split("<")[-1][:90]}')
