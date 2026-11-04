#!/usr/bin/env python3
"""Capstone Xtensa disassembly of a raw image window.

usage: xtdis.py <image> <file_off_hex> <len_hex> [vaddr_hex]
"""
import sys
from capstone import Cs, CS_ARCH_XTENSA, CS_MODE_LITTLE_ENDIAN

img, off, ln = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
va = int(sys.argv[4], 16) if len(sys.argv) > 4 else off
code = open(img, 'rb').read()[off:off + ln]
md = Cs(CS_ARCH_XTENSA, CS_MODE_LITTLE_ENDIAN)
md.skipdata = True
for i in md.disasm(code, va):
    print(f'{i.address:08x}: {i.bytes.hex():<12} {i.mnemonic} {i.op_str}')
