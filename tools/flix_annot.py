#!/usr/bin/env python3
"""Annotate FLIX bundles in an xtlist listing with the branch-slot hypothesis
(b2==0xb0,b4==0x41,b7==0x0d): kind A = RRI8 branch (r=b1>>4 cond, s=b1&f, t=b0>>4),
kind B = BRI8 immediate branch (n=(b0>>4)&3, m=b0>>6, s=b1&f, r=b1>>4 -> b4const).
Inference from Ghidra block starts, not an ISA spec.

usage: flix_annot.py <xtlist.py listing>
"""
import re, sys
RR = {0:'bnone',1:'beq',2:'blt',3:'bltu',4:'ball',5:'bbc',6:'bbci',7:'bbci',8:'bany',9:'bne',10:'bge',11:'bgeu',12:'bnall',13:'bbs',14:'bbsi',15:'bbsi'}
B4 = [-1,1,2,3,4,5,6,7,8,10,12,16,32,64,128,256]
B4U = [32768,65536,2,3,4,5,6,7,8,10,12,16,32,64,128,256]
for line in open(sys.argv[1]):
    line = line.rstrip('\n')
    m = re.match(r'([0-9a-f]{8})  ([0-9a-f]{16})  flix', line)
    if m:
        pc = int(m.group(1), 16); b = bytes.fromhex(m.group(2))
        if b[2] == 0xB0 and b[4] == 0x41 and b[7] == 0x0D:
            off = ((b[6] & 0x3f) << 12) | (b[5] << 4) | (b[3] >> 4)
            if off & 0x20000: off -= 0x40000
            tgt = pc + 4 + off
            s = b[1] & 15; r = b[1] >> 4; t = b[0] >> 4
            if b[6] & 0x40:
                n = t & 3; mm = t >> 2
                if t == 0:
                    d = f'beqi a{s}, {B4[r]}'
                elif t == 6:
                    d = f'blti a{s}, {B4[r]}'
                elif n == 1:
                    op = ['beqz','bnez','bltz','bgez'][mm]; d = f'{op} a{s}'
                elif n == 2:
                    op = ['beqi','bnei','blti','bgei'][mm]; d = f'{op} a{s}, {B4[r]}'
                elif n == 3 and mm >= 2:
                    op = ['bltui','bgeui'][mm-2]; d = f'{op} a{s}, {B4U[r]}'
                else:
                    d = f'B? n={n} m={mm} a{s} r={r}'
            else:
                A = {0:f'bbci a{s}, {t}',2:f'bbsi a{s}, {t}',9:f'bne a{s}, a{t}',10:f'bge a{s}, a{t} (s>=t)',
                     11:f'A11(bne/bltu?) a{s}, a{t}',12:f'A12(s>t unsigned?) a{s}, a{t}',14:f'A14(t>=s unsigned?) a{s}, a{t}',8:f'A8? a{s}, a{t}'}
                d = A.get(r, f'A{r}? a{s}, a{t}')
            line += f'   ; FLIX? {d}, {tgt:#010x}'
        else:
            line += '   ; FLIX non-branch'
    print(line)
