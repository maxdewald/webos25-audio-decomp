#!/usr/bin/env python3
"""Add the functions Ghidra missed to an ARM target's symbol table, using .ARM.exidx.

Every function compiled with -funwind-tables has an exidx entry at its start. Ghidra
sometimes folds a function into its predecessor (it then reports one oversized
function) or never creates it. For each exidx start that is not in the table:
  - if it lies inside a known function, that function is cut there;
  - the new function runs to the next exidx start or table entry, whichever is first.
A name for the new row is taken from the src/ tag at that address if one exists,
else FUN_<addr>.

usage: exidx_split.py <target> [--write]
"""
import bisect
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import progress as p  # noqa: E402


def exidx_starts(t):
    out = subprocess.run([p.tool('llvm-readelf'), '-u', str(t.binary)],
                         capture_output=True, text=True).stdout
    return sorted({int(m, 16) + t.image_bias
                   for m in re.findall(r'FunctionAddress: 0x([0-9A-Fa-f]+)', out)})


def main():
    name = sys.argv[1]
    write = '--write' in sys.argv
    t = next(t for t in p.load_targets() if t.name == name)
    rows = {}
    for line in t.symbols.read_text().splitlines():
        a, size, n = line.split('\t', 2)
        rows[int(a, 16)] = [int(size), n]
    tags = {f.addr: f.src_name for f in t.funcs.values() if f.src}
    starts = exidx_starts(t)
    added = 0
    for s in starts:
        if s in rows:
            continue
        keys = sorted(rows)
        i = bisect.bisect_right(keys, s) - 1
        if i >= 0 and keys[i] + rows[keys[i]][0] > s:
            prev = keys[i]
            end = prev + rows[prev][0]
            rows[prev][0] = s - prev            # cut the function that swallowed it
        else:
            end = None
        nxt = [x for x in starts if x > s][:1] + [k for k in keys if k > s][:1]
        limit = min(nxt) if nxt else s
        if end is not None:
            limit = min(limit, end)
        if limit <= s:
            continue
        rows[s] = [limit - s, tags.get(s, f'FUN_{s:08x}')]
        added += 1
    print(f'{name}: {added} functions added from .ARM.exidx')
    if write:
        t.symbols.write_text(''.join(f'{a:08x}\t{v[0]}\t{v[1]}\n' for a, v in sorted(rows.items())))


if __name__ == '__main__':
    main()
