#!/usr/bin/env python3
"""Hypothesis decoder for the branch slot of 8-byte FLIX bundles seen in gain.asm.

Bytes b0..b7 of a bundle with b2==0xb0, b4==0x41, b7==0x0d are read as:
  ra = b1 & 0xf, rb_or_imm = b0 >> 4, cond = b1 >> 4, kind = b6 & 0x40,
  target = pc + 4 + sext12((b5 << 4) | (b3 >> 4)).
Validated by comparing targets with Ghidra's synthetic __blk_ starts (blocks
that have no other incoming reference). Inference, not an ISA spec.
"""
import re
import sys
from pathlib import Path

here = Path(__file__).resolve().parent
asm = (here / "gain.asm").read_text().splitlines()
decomp = here.parent.parent / "decomp/audio_a0_dsp2.main_60000000.bin.c"
blocks = {int(m.group(1), 16): m.group(2) for m in re.finditer(
    r"^/\* @([0-9a-f]+) (\S+) \*/", decomp.read_text(), re.M)}
starts = {int(l.split("\t")[0], 16) for l in asm}

hit = miss = 0
for line in asm:
    addr, raw, *rest = line.split("\t")
    if len(raw) != 16:
        continue
    b = bytes.fromhex(raw)
    pc = int(addr, 16)
    if not (b[2] == 0xB0 and b[4] == 0x41 and b[7] == 0x0D):
        print(f"{pc:08x}\t{raw}\tnon-branch-shape")
        continue
    off = (b[5] << 4) | (b[3] >> 4)
    if off & 0x800:
        off -= 0x1000
    tgt = pc + 4 + off
    name = blocks.get(tgt)
    ok = name is not None or tgt in starts
    hit += name is not None
    miss += name is None
    print(f"{pc:08x}\t{raw}\tra=a{b[1] & 0xf}\tb0hi={b[0] >> 4:x}\tcond={b[1] >> 4:x}"
          f"\tkind={'B' if b[6] & 0x40 else 'A'}\ttarget={tgt:08x}\t{name or ('insn-boundary' if ok else 'NO-MATCH')}")
print(f"targets on __blk_/function starts: {hit}, others: {miss}", file=sys.stderr)
