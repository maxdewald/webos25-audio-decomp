#!/usr/bin/env python3
"""Linear-sweep Xtensa listing with L32R literal/string/symbol resolution; FLIX (op0 E/F) shown as opaque8.

usage: xtlist.py <dsp0|dsp1|dsp2> <start_hex> <end_hex_exclusive> [out]
Needs build/fw/ from `make decomp-dsp`.
"""
import re, struct, sys
from pathlib import Path
import capstone

ROOT = Path(__file__).resolve().parent.parent
BASE = 0x60000000
DSP = sys.argv.pop(1) if __name__ == '__main__' else 'dsp1'
img = (ROOT / f'build/fw/audio_a0_{DSP}.main_60000000.bin').read_bytes()
syms = {}
for line in (ROOT / f'symbols/{DSP}.syms.tsv').read_text().splitlines():
    if line and not line.startswith('#'):
        a, n = line.split('\t', 1)
        syms.setdefault(int(a, 16), n)
cs = capstone.Cs(capstone.CS_ARCH_XTENSA, capstone.CS_MODE_LITTLE_ENDIAN)


def word(a):
    o = a - BASE
    return struct.unpack_from('<I', img, o)[0] if 0 <= o <= len(img) - 4 else None


def cstr(a):
    o = a - BASE
    if not 0 <= o < len(img):
        return None
    e = img.find(b'\0', o, o + 200)
    if e - o < 2:
        return None
    s = img[o:e]
    return s.decode() if all(32 <= c < 127 or c in (9, 10) for c in s) else None


def describe(v):
    out = [f'{v:#010x}']
    if v in syms:
        out.append(f'<{syms[v]}>')
    s = cstr(v)
    if s:
        out.append(repr(s))
    return ' '.join(out)


def sweep(start, end):
    p = start
    while p < end:
        b0 = img[p - BASE]
        op0 = b0 & 15
        n = 8 if op0 >= 14 else 2 if op0 >= 8 else 3
        raw = img[p - BASE:p - BASE + n]
        if op0 >= 14:
            yield p, raw, 'flix', f'opaque8 lo={struct.unpack_from("<I", raw)[0]:08x} hi={struct.unpack_from("<I", raw, 4)[0]:08x}', None
        elif op0 == 0xC and b0 & 0x80:
            imm = ((b0 >> 4) & 3) << 4 | raw[1] >> 4
            t = p + 4 + imm
            yield p, raw, 'bnez.n' if b0 & 0x40 else 'beqz.n', f'a{raw[1] & 15}, {t:#010x}', syms.get(t) and f'<{syms[t]}>'
        elif op0 == 0 and raw[2] == 0x72 or op0 == 0 and raw[2] == 0x62:
            yield p, raw, 'salt' if raw[2] == 0x72 else 'saltu', f'a{raw[1] >> 4}, a{raw[1] & 15}, a{b0 >> 4}', None
        else:
            ins = next(cs.disasm(raw, p, 1), None)
            if ins is None or ins.size != n:
                yield p, raw, '.byte', '', None
            else:
                note = None
                if op0 == 1 and n == 3:
                    imm = struct.unpack_from('<H', raw, 1)[0]
                    slot = (((p + 3) & ~3) + ((imm | 0xFFFF0000) << 2)) & 0xFFFFFFFF
                    v = word(slot)
                    note = f'[{slot:08x}]={describe(v)}' if v is not None else f'[{slot:08x}]'
                ops = ins.op_str
                rel = re.search(r'\. ([+-])0x([0-9a-f]+)', ops)
                if rel:
                    t = p + (1 if rel.group(1) == '+' else -1) * int(rel.group(2), 16)
                    if ins.mnemonic.startswith('call') and op0 == 5:
                        off = (b0 >> 6) | raw[1] << 2 | raw[2] << 10
                        off -= 0x40000 if off & 0x20000 else 0
                        t = (p & ~3) + (off << 2) + 4
                    if ins.mnemonic != 'l32r':
                        ops = ops[:rel.start()] + f'{t:#010x}'
                        if t in syms:
                            note = f'<{syms[t]}>'
                yield p, raw, ins.mnemonic, ops, note
        p += n


if __name__ == '__main__':
    s, e = int(sys.argv[1], 16), int(sys.argv[2], 16)
    out = open(sys.argv[3], 'w') if len(sys.argv) > 3 else sys.stdout
    for p, raw, mn, ops, note in sweep(s, e):
        lab = f'{syms[p]}:\n' if p in syms else ''
        out.write(f'{lab}{p:08x}  {raw.hex():16s}  {mn:8s} {ops}' + (f'   ; {note}' if note else '') + '\n')
