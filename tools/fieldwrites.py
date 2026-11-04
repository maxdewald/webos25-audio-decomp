#!/usr/bin/env python3
"""Report loads/stores to module-object offsets through derived base registers (a2/a4 + addmi/addi).

usage: fieldwrites.py <listing.asm> <hex_offset>...   (listing from ListInsns.java)
Bases reset at each `entry`; a register is tracked as obj+K after `addmi/addi aX, aB, imm`
or `mov aX, aB` where aB is a tracked base. Function argument a2 is the object pointer.
"""
import re, sys

want = {int(x, 16) for x in sys.argv[2:]}
base = {}
for ln in open(sys.argv[1]):
    p = ln.rstrip('\n').split('\t')
    if len(p) < 3:
        continue
    addr, text = p[0], p[2]
    m = re.match(r'(\S+)\s+(.*)', text)
    if not m:
        continue
    op, args = m.group(1), [a.strip() for a in m.group(2).split(',')]
    if op == 'entry':
        base = {'a2': 0}
        continue
    if op in ('addmi', 'addi', 'addi.n') and len(args) == 3 and args[1] in base:
        base[args[0]] = base[args[1]] + int(args[2], 0)
        continue
    if op in ('mov', 'mov.n') and len(args) == 2 and args[1] in base:
        base[args[0]] = base[args[1]]
        continue
    mm = re.match(r'(s32i|l32i|s8i|l8ui|s16i|l16ui)(\.n)?$', op)
    if mm and len(args) == 3 and args[1] in base:
        off = base[args[1]] + int(args[2], 0)
        if off in want:
            kind = 'WRITE' if op.startswith('s') else 'read'
            print(f'{addr} {kind:5} +{off:#x} {text}')
        continue
    if args and re.match(r'a\d+$', args[0]) and not op.startswith(('s', 'b', 'j', 'call', 'ret')):
        base.pop(args[0], None)
