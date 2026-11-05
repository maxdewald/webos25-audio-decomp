#!/usr/bin/env python3
"""Decompilation progress: compare src/ against the original LG binaries.

  progress.py status [--json] [--no-match]   coverage + instruction match per target
  progress.py diff <target> <function|0xaddr> instruction diff, original vs src
  progress.py todo <target> [-n N]           largest functions not yet in src/
  progress.py symbols                        write symbols/<target>.elf.tsv for ARM targets without one
"""
import argparse
import difflib
import json
import re
import shutil
import subprocess
import sys
import tomllib
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CFG = tomllib.loads((ROOT / 'decomp.toml').read_text())
TAG = re.compile(r'^/\*\s*@0x([0-9a-fA-F]+)')
DEF = re.compile(r'^(?:static\s+)?(?:inline\s+)?[\w\s\*]*?\b(\w+)\s*\([^;]*$')
INSN = re.compile(r'^\s+[0-9a-f]+:\s+(.+)$')


def tool(name):
    return shutil.which(name) or f'/opt/homebrew/opt/llvm/bin/{name}'


@dataclass
class Func:
    addr: int
    size: int
    name: str
    src: str = ''          # file:line of the reconstruction
    src_name: str = ''     # C name of the reconstruction
    match: float | None = None
    note: str = ''


@dataclass
class Target:
    name: str
    binary: Path
    arch: str
    symbols: Path | None
    src: Path | None
    image_bias: int
    src_bias: int
    cflags_extra: list = field(default_factory=list)
    funcs: dict = field(default_factory=dict)

    @property
    def has_binary(self):
        return self.binary.is_file()


def load_targets():
    out = []
    for t in CFG['target']:
        arch = t.get('arch', 'arm')
        out.append(Target(
            name=t['name'], binary=ROOT / t['binary'], arch=arch,
            symbols=ROOT / t.get('symbols', f'symbols/{t["name"]}.elf.tsv'),
            src=ROOT / t['src'] if 'src' in t else None,
            image_bias=t.get('image_bias', 0x10000 if arch == 'arm' else 0),
            src_bias=t.get('src_addr_bias', 0),
            cflags_extra=t.get('cflags_extra', [])))
    for t in out:
        if t.symbols.is_file():
            for line in t.symbols.read_text().splitlines():
                a, size, name = line.split('\t', 2)
                t.funcs[int(a, 16)] = Func(int(a, 16), int(size), name)
        if t.arch == 'arm' and t.has_binary:
            for f in elf_funcs(t):
                t.funcs.setdefault(f.addr, f)
        if t.src and t.src.is_dir():
            for f in sorted(t.src.rglob('*.c')):
                scan_source(t, f)
    return out


def elf_funcs(t):
    """Sized function symbols from the ELF itself (dynamic symbols survive stripping)."""
    r = subprocess.run([tool('llvm-nm'), '--defined-only', '-S', '-D', str(t.binary)],
                       capture_output=True, text=True)
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) == 4 and p[2] in 'Tt' and int(p[1], 16):
            yield Func((int(p[0], 16) & ~1) + t.image_bias, int(p[1], 16), p[3])


def scan_source(t, path):
    lines = path.read_text().splitlines()
    i = 0
    while i < len(lines):
        m = TAG.match(lines[i])
        if not m:
            i += 1
            continue
        start = i
        while '*/' not in lines[i]:
            i += 1
        i += 1
        d = DEF.match(lines[i]) if i < len(lines) else None
        if not d:
            continue
        addr = int(m[1], 16) + t.src_bias
        f = t.funcs.setdefault(addr, Func(addr, 0, f'?{addr:x}', note='not in symbol table'))
        f.src = f'{path.relative_to(ROOT)}:{start + 1}'
        f.src_name = d[1]


def obj_path(t, c_file):
    return ROOT / 'build/obj' / t.name / (c_file.stem + '.o')


def compile_targets(targets):
    cc = CFG['compiler']
    jobs = []
    if not targets:
        return True
    for t in targets:
        for c in sorted(t.src.rglob('*.c')):
            o = obj_path(t, c)
            o.parent.mkdir(parents=True, exist_ok=True)
            jobs.append([cc['cc'], *cc['cflags'], *t.cflags_extra, '-c', str(c.relative_to(ROOT)),
                         '-o', str(o.relative_to(ROOT))])
    script = ' ; '.join(' '.join(j) for j in jobs)
    native = shutil.which(cc['cc'])
    cmd = ['sh', '-c', script] if native else [
        'docker', 'run', '--rm', '-v', f'{ROOT}:/w', '-w', '/w', cc['image'], 'sh', '-c', script]
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    if r.stderr.strip():
        print(r.stderr.strip(), file=sys.stderr)
    return ' error: ' not in r.stderr


def normalise(text):
    out = []
    for line in text.splitlines():
        m = INSN.match(line)
        if not m or 'R_ARM_' in m[1]:
            continue
        s = m[1].split(' @ ')[0]
        if re.search(r'(^|\s)\.(word|short|byte|long)\s', s + ' '):
            continue    # literal pools / jump-table data, not instructions
        s = re.sub(r'0x[0-9a-f]+ <[^>]*>', '<target>', s)
        s = re.sub(r'\[pc, #-?0x[0-9a-f]+\]', '[pc, #lit]', s)
        s = re.sub(r'^blx\b', 'bl', s)
        out.append(' '.join(s.split()))
    return out


def disasm_orig(t, f):
    va = f.addr - t.image_bias
    r = subprocess.run([tool('llvm-objdump'), '-d', '--no-show-raw-insn', '--triple=armv7',
                        '--mattr=+thumb-mode', f'--start-address={va:#x}',
                        f'--stop-address={va + f.size:#x}', str(t.binary)],
                       capture_output=True, text=True)
    return normalise(r.stdout)


_objcache = {}


def _obj_listing(o):
    """(symbol -> (addr, size), full disassembly lines) for one object, cached by mtime."""
    key = (o, o.stat().st_mtime)
    if key not in _objcache:
        syms = {}
        for l in subprocess.run([tool('llvm-objdump'), '-t', str(o)],
                                capture_output=True, text=True).stdout.splitlines():
            m = re.match(r'([0-9a-f]+) .*F \.text\S*\s+([0-9a-f]+) (\S+)', l)
            if m:
                syms[m[3]] = (int(m[1], 16), int(m[2], 16))
        full = subprocess.run([tool('llvm-objdump'), '-d', '--no-show-raw-insn', str(o)],
                              capture_output=True, text=True).stdout.splitlines()
        _objcache[key] = (syms, full)
    return _objcache[key]


def disasm_src(t, f):
    """Instructions of f in its object, by address range: --disassemble-symbols stops at
    the first inline jump table, which under-scores functions with a switch."""
    o = obj_path(t, ROOT / f.src.split(':')[0])
    if not o.is_file():
        return None
    syms, full = _obj_listing(o)
    # GCC may emit a clone (foo.constprop.0, foo.isra.0, foo.part.0) instead of foo.
    names = sorted((n for n in syms if n == f.src_name or n.startswith(f.src_name + '.')),
                   key=lambda n: (n != f.src_name, n))
    if not names:
        return None
    a, size = syms[names[0]]
    out = []
    for l in full:
        m = re.match(r'\s*([0-9a-f]+):', l)
        if m and a <= int(m[1], 16) < a + size:
            out.append(l)
    return normalise('\n'.join(out)) or None


def score(t):
    for f in t.funcs.values():
        if not f.src or not f.size:
            continue
        mine = disasm_src(t, f)
        if mine is None:
            f.match, f.note = 0.0, 'not emitted (inlined or failed to compile)'
            continue
        f.match = 100.0 * difflib.SequenceMatcher(None, disasm_orig(t, f), mine).ratio()


def summary(t):
    total = sum(f.size for f in t.funcs.values())
    done = [f for f in t.funcs.values() if f.src]
    covered = sum(f.size for f in done)
    scored = [f for f in done if f.match is not None and f.size]
    weighted = sum(f.match * f.size for f in scored)
    return {
        'target': t.name, 'arch': t.arch, 'binary_present': t.has_binary,
        'functions': len(t.funcs), 'bytes': total,
        'reconstructed_functions': len(done), 'reconstructed_bytes': covered,
        'coverage_pct': round(100 * covered / total, 3) if total else 0.0,
        'matching_functions': sum(1 for f in scored if f.match == 100.0),
        'match_pct': round(weighted / total, 3) if total else None,
        'symbols': bool(t.funcs),
    }


def bar(pct, width=20):
    n = round(width * pct / 100)
    return '#' * n + '.' * (width - n)


def cmd_status(args):
    targets = load_targets()
    ok = True
    if not args.no_match:
        ok = compile_targets([t for t in targets if t.arch == 'arm' and t.src])
        for t in targets:
            if t.arch == 'arm' and t.src and t.has_binary:
                score(t)
    rows = [summary(t) for t in targets]
    if args.json:
        print(json.dumps(rows, indent=2))
        sys.exit(0 if ok else 1)
    print(f'{"target":40} {"funcs":>7} {"src":>4} {"coverage":>30} {"match":>8}')
    for r in rows:
        if not r['symbols']:
            print(f'{r["target"]:40} {"-":>7} {"-":>4} {"no symbol table yet":>30}')
            continue
        match = f'{r["match_pct"]:7.3f}%' if r['match_pct'] is not None and r['arch'] == 'arm' \
            and not args.no_match and r['binary_present'] else '     n/a'
        print(f'{r["target"]:40} {r["functions"]:7} {r["reconstructed_functions"]:4} '
              f'{bar(r["coverage_pct"])} {r["coverage_pct"]:8.3f}% {match}')
    tot = sum(r['bytes'] for r in rows)
    cov = sum(r['reconstructed_bytes'] for r in rows)
    print(f'\ntotal: {cov}/{tot} bytes reconstructed ({100 * cov / tot:.3f}%), '
          f'{sum(1 for r in rows if not r["symbols"])} targets without a symbol table')
    sys.exit(0 if ok else 1)


def find(targets, name):
    for t in targets:
        if t.name == name:
            return t
    sys.exit(f'unknown target {name}; see decomp.toml')


def cmd_diff(args):
    t = find(load_targets(), args.target)
    if args.function.startswith('0x'):
        f = t.funcs.get(int(args.function, 16))
    else:
        f = next((f for f in t.funcs.values()
                  if args.function in (f.src_name, f.name) or f.name.endswith('::' + args.function)), None)
    if not f:
        sys.exit('function not found')
    orig = disasm_orig(t, f)
    if not f.src:
        print('\n'.join(orig))
        sys.exit(f'{f.name} @0x{f.addr:x} has no reconstruction yet')
    compile_targets([t])
    mine = disasm_src(t, f) or []
    ratio = 100.0 * difflib.SequenceMatcher(None, orig, mine).ratio()
    print(f'{f.name} @0x{f.addr:x} <-> {f.src_name} ({f.src}): {ratio:.2f}%')
    sys.stdout.writelines(l + '\n' for l in difflib.unified_diff(
        orig, mine, 'original', 'src', lineterm='', n=3))


def cmd_symbols(args):
    for t in load_targets():
        if t.arch != 'arm' or not t.has_binary or t.symbols.is_file():
            continue
        rows = sorted(elf_funcs(t), key=lambda f: f.addr)
        t.symbols.write_text(''.join(f'{f.addr:08x}\t{f.size}\t{f.name}\n' for f in rows))
        print(f'{t.symbols.relative_to(ROOT)}: {len(rows)} functions')


def cmd_todo(args):
    t = find(load_targets(), args.target)
    left = sorted((f for f in t.funcs.values() if not f.src), key=lambda f: -f.size)
    for f in left[:args.n]:
        print(f'0x{f.addr:08x} {f.size:7} {f.name}')


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest='cmd', required=True)
    s = sub.add_parser('status')
    s.add_argument('--json', action='store_true')
    s.add_argument('--no-match', action='store_true', help='skip compiling and instruction matching')
    s.set_defaults(fn=cmd_status)
    d = sub.add_parser('diff')
    d.add_argument('target')
    d.add_argument('function')
    d.set_defaults(fn=cmd_diff)
    t = sub.add_parser('todo')
    t.add_argument('target')
    t.add_argument('-n', type=int, default=25)
    t.set_defaults(fn=cmd_todo)
    sub.add_parser('symbols').set_defaults(fn=cmd_symbols)
    args = p.parse_args()
    args.fn(args)


if __name__ == '__main__':
    main()
