#!/usr/bin/env python3
"""Show raw words around the symbol-table entry that references a given name."""
import struct, sys

BASE = 0x60000000
d = open(sys.argv[1], 'rb').read()
name = sys.argv[2].encode() + b'\0'
so = d.find(b'\0' + name) + 1
sva = BASE + so
n = len(d) // 4
words = struct.unpack_from(f'<{n}I', d)
for i, w in enumerate(words):
    if w == sva:
        print(f'name {sys.argv[2]} @ {sva:#x}, referenced at word {BASE + i * 4:#x}')
        for j in range(i - 6, i + 7):
            v = words[j]
            tag = ''
            o = v - BASE
            if 0 <= o < len(d):
                if d[o] == 0x36:
                    tag = 'FUNC'
                else:
                    e = d.find(b'\0', o, o + 40)
                    s = d[o:e]
                    if s and all(32 <= c < 127 for c in s):
                        tag = repr(s.decode())
            print(f'  {BASE + j * 4:#x}: {v:#010x} {tag}')
        break
