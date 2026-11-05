#!/usr/bin/env python3
"""Name unnamed C runtime / libgcc functions in ARM symbol tables.

Every LG library links the same crtstuff (call_weak_fn, register_tm_clones, ...) and
libgcc helpers (__aeabi_uidiv, __udivmoddi4, ...). Where Ghidra left them as FUN_xxx,
compare their bytes with the named copies in the other targets and adopt the name.
Literal-pool words differ per binary, so only instruction-sized prefixes are compared.

usage: name_crt.py [--write]
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import progress as p  # noqa: E402

PREFIX = 12   # bytes compared; long enough to be unique, short enough to skip literals


def body(t, addr, size):
    data = t.binary.read_bytes()
    off = file_offset(t, addr - t.image_bias)
    return data[off:off + size] if off is not None else b''


def rows_of(t):
    return [l.split('\t') for l in t.symbols.read_text().splitlines()]


def file_offset(t, va):
    import subprocess
    if not hasattr(t, '_segs'):
        out = subprocess.run([p.tool('llvm-readelf'), '-lW', str(t.binary)],
                             capture_output=True, text=True).stdout
        t._segs = []
        for ln in out.splitlines():
            s = ln.split()
            if s and s[0] == 'LOAD':
                t._segs.append((int(s[2], 16), int(s[1], 16), int(s[4], 16)))
    for vaddr, off, filesz in t._segs:
        if vaddr <= va < vaddr + filesz:
            return off + va - vaddr
    return None


def main():
    write = '--write' in sys.argv
    targets = [t for t in p.load_targets() if t.arch == 'arm' and t.has_binary and t.symbols.is_file()]
    known = {}
    for t in targets:
        for a, size, name in rows_of(t):
            if p.IGNORE.match(name) and int(size) >= PREFIX:
                known.setdefault((int(size), body(t, int(a, 16), int(size))[:PREFIX]), name)
    for t in targets:
        rows = rows_of(t)
        changed = 0
        for r in rows:
            if not r[2].startswith('FUN_'):
                continue
            a, size = int(r[0], 16), int(r[1])
            name = known.get((size, body(t, a, size)[:PREFIX]))
            if name:
                r[2] = name
                changed += 1
        if changed:
            print(f'{t.name}: {changed} runtime functions named')
            if write:
                t.symbols.write_text(''.join('\t'.join(r) + '\n' for r in rows))


if __name__ == '__main__':
    main()
