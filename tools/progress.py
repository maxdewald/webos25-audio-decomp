#!/usr/bin/env python3
"""Decompilation progress: compare src/ against the original LG binaries.

  progress.py status [--json] [--no-match]   coverage + instruction match per target
  progress.py diff <target> <function|0xaddr> instruction diff, original vs src
  progress.py todo <target> [-n N]           largest functions not yet in src/
  progress.py symbols                        write symbols/<target>.elf.tsv for ARM targets without one
  progress.py svg [--no-match]               write progress.svg: original files with a reconstruction
"""
import argparse
import difflib
import json
import re
import shlex
import shutil
import subprocess
import sys
import tomllib
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CFG = tomllib.loads((ROOT / 'decomp.toml').read_text())
TAG = re.compile(r'^/\*\s*@0x([0-9a-fA-F]+)')
DEF = re.compile(r'^(?:static\s+)?(?:inline\s+)?[\w\s\*&:<>,]*?\b((?:\w+::)*~?\w+)\s*\([^;]*$')
SRC_EXT = ('*.c', '*.cpp')
INSN = re.compile(r'^\s+[0-9a-f]+:\s+(.+)$')
# Compiler runtime linked into every binary (crtstuff, libgcc, PLT-veneer padding). LG
# never wrote it, so it is left out of both coverage and match.
IGNORE = re.compile(r'^(_init|_fini|_DT_INIT|_DT_FINI|_FINI_\d+|_INIT_\d+|call_weak_fn|'
                    r'(de)?register_tm_clones|__do_global_dtors_aux|frame_dummy|'
                    r'__aeabi_\w+|__(u)?divmod[sd]i4|__udivmoddi4|__(add|sub|mul|div)[sd]f3|'
                    r'__float\w+|__fix\w+|__extend\w+|__trunc\w+|__(u)?(div|mod)si3|'
                    r'__clz\w+|__ctz\w+|__gnu_\w+|__cmp[sd]f2|__(eq|ne|lt|le|gt|ge|un)[sd]f2)$')


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
                if IGNORE.match(name):
                    continue
                t.funcs[int(a, 16)] = Func(int(a, 16), int(size), name)
        if t.arch == 'arm' and t.has_binary:
            for f in elf_funcs(t):
                if not IGNORE.match(f.name):
                    t.funcs.setdefault(f.addr, f)
        if t.src and t.src.is_dir():
            for f in sorted(p for ext in SRC_EXT for p in t.src.rglob(ext)):
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
        # "/* @0xADDR = symbol */" names a function a macro expands to (G_DEFINE_TYPE,
        # GST_PLUGIN_DEFINE, ...); no definition line follows it.
        alias = re.match(r'^/\*\s*@0x[0-9a-fA-F]+\s*=\s*(\w+)\b', lines[i])
        while '*/' not in lines[i]:
            i += 1
        i += 1
        if alias:
            name = alias[1]
        else:
            d = DEF.match(lines[i]) if i < len(lines) else None
            if not d:
                continue
            name = d[1]
            # An explicit asm label binds ABI wrappers to the original symbol.
            lab = re.search(r'__asm__\("([^"]+)"\)', lines[start - 1]) if start else None
            if lab:
                name = lab[1]
            elif name.endswith('__def'):
                name = name[:-5]
        addr = int(m[1], 16) + t.src_bias
        f = t.funcs.setdefault(addr, Func(addr, 0, f'?{addr:x}', note='not in symbol table'))
        f.src = f'{path.relative_to(ROOT)}:{start + 1}'
        f.src_name = name


def obj_path(t, c_file):
    return ROOT / 'build/obj' / t.name / (c_file.stem + '.o')


def compile_targets(targets, only=None):
    cc = CFG['compiler']
    jobs = []
    if not targets:
        return True
    for t in targets:
        for c in sorted(p for ext in SRC_EXT for p in t.src.rglob(ext)):
            if only and c != only:
                continue
            o = obj_path(t, c)
            o.parent.mkdir(parents=True, exist_ok=True)
            cpp = c.suffix == '.cpp'
            jobs.append([cc['cxx'] if cpp else cc['cc'], *cc['cflags'],
                         *(cc.get('cxxflags', []) if cpp else []), *t.cflags_extra, '-c',
                         str(c.relative_to(ROOT)),
                         '-o', str(o.relative_to(ROOT))])
    if not jobs:
        return True
    script = ' && '.join(shlex.join(j) for j in jobs)
    native = shutil.which(cc['cc'])
    cmd = ['sh', '-c', script] if native else [
        'docker', 'run', '--rm', '-v', f'{ROOT}:/w', '-w', '/w', cc['image'], 'sh', '-c', script]
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    if r.stderr.strip():
        print(r.stderr.strip(), file=sys.stderr)
    return r.returncode == 0


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
        # C++: also index by demangled name without the parameter list
        # ("umiTVAudio::setOutputType"), which is what src tags name.
        mangled = [n for n in syms if n.startswith('_Z')]
        if mangled:
            dem = subprocess.run([tool('llvm-cxxfilt')], input='\n'.join(mangled),
                                 capture_output=True, text=True).stdout.splitlines()
            for m_, d in zip(mangled, dem):
                syms.setdefault(d.split('(')[0], syms[m_])
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
    want = (f.src_name, f.name, f.name.split('(')[0])
    if f.src_name in syms:
        want = (f.src_name,)
    names = sorted((n for n in syms if n in want or n.startswith(f.src_name + '.')),
                   key=lambda n: (n not in want, n))
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
        'match_pct': round(weighted / total, 3) if total and scored else None,
        'symbols': bool(t.funcs),
    }


def bar(pct, width=20):
    n = round(width * pct / 100)
    return '#' * n + '.' * (width - n)


def score_all(targets):
    ok = compile_targets([t for t in targets if t.arch == 'arm' and t.src])
    for t in targets:
        if t.arch == 'arm' and t.src and t.has_binary:
            score(t)
    return ok


def cmd_status(args):
    targets = load_targets()
    ok = args.no_match or score_all(targets)
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
    if not compile_targets([t], only=ROOT / f.src.split(':')[0]):
        sys.exit('compile failed; refusing to compare a stale object')
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


SVG_STYLE = '''
  text { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif; fill: #1f2328; }
  .title { font-size: 18px; font-weight: 600; }
  .sub, .path, .legend { font-size: 12px; fill: #656d76; }
  .name { font-size: 14px; font-weight: 600; }
  .pct { font-size: 13px; font-weight: 600; font-variant-numeric: tabular-nums; }
  .card { fill: #ffffff; stroke: #d0d7de; }
  .track { fill: #eaeef2; }
  .cov { fill: #9fd5ad; }
  .match { fill: #1f883d; }
  .dsp { fill: #b6c8f5; }
  @media (prefers-color-scheme: dark) {
    text { fill: #e6edf3; }
    .sub, .path, .legend { fill: #8d96a0; }
    .card { fill: #0d1117; stroke: #30363d; }
    .track { fill: #21262d; }
    .cov { fill: #2e6b3d; }
    .match { fill: #3fb950; }
    .dsp { fill: #3b5bab; }
  }
'''


def progress_svg(rows):
    """Bar chart: one row per original file with a reconstruction."""
    from html import escape
    W, PAD, ROW, HEAD = 820, 24, 46, 92
    BX, BW = 330, 340
    h = HEAD + ROW * len(rows) + 52
    cov = sum(r['reconstructed_bytes'] for r in rows)
    tot = sum(r['bytes'] for r in rows)
    out = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{h}" viewBox="0 0 {W} {h}" '
           f'role="img" aria-label="Decompilation progress">',
           f'<style>{SVG_STYLE}</style>',
           f'<rect class="card" x="0.5" y="0.5" width="{W - 1}" height="{h - 1}" rx="10"/>',
           f'<text class="title" x="{PAD}" y="38">Decompilation progress</text>',
           f'<text class="sub" x="{PAD}" y="60">{len(rows)} original files · '
           f'{cov / 1024:,.0f} of {tot / 1024:,.0f} KiB of their code reconstructed '
           f'({100 * cov / tot:.1f}%)</text>']
    for i, r in enumerate(rows):
        y = HEAD + i * ROW
        c, m = r['coverage_pct'], r['match_pct']
        arm = r['arch'] == 'arm'
        out += [f'<text class="name" x="{PAD}" y="{y + 16}">{escape(r["target"])}</text>',
                f'<text class="path" x="{PAD}" y="{y + 33}">{escape(r["path"])}</text>',
                f'<rect class="track" x="{BX}" y="{y + 10}" width="{BW}" height="14" rx="7"/>']
        if c:
            out.append(f'<rect class="{"cov" if arm else "dsp"}" x="{BX}" y="{y + 10}" '
                       f'width="{max(BW * c / 100, 3):.1f}" height="14" rx="7"/>')
        if arm and m:
            out.append(f'<rect class="match" x="{BX}" y="{y + 10}" '
                       f'width="{max(BW * m / 100, 3):.1f}" height="14" rx="7"/>')
        out.append(f'<text class="pct" x="{BX + BW + 16}" y="{y + 22}">{c:.1f}%</text>')
        out.append(f'<text class="path" x="{BX + BW + 16}" y="{y + 38}">'
                   f'{f"{m:.1f}% match" if arm and m is not None else r["arch"]}</text>')
    y = h - 26
    for x, cls, label in ((PAD, 'match', 'matches original instructions'),
                          (PAD + 230, 'cov', 'reconstructed (ARM)'),
                          (PAD + 400, 'dsp', 'reconstructed (Xtensa / aarch64, no matching compiler)')):
        out += [f'<rect class="{cls}" x="{x}" y="{y - 10}" width="12" height="12" rx="3"/>',
                f'<text class="legend" x="{x + 18}" y="{y}">{label}</text>']
    out.append('</svg>\n')
    return '\n'.join(out)


def cmd_svg(args):
    targets = [t for t in load_targets() if t.src and any(f.src for f in t.funcs.values())]
    if not args.no_match:
        score_all(targets)
    rows = [summary(t) | {'path': '/' + str(t.binary.relative_to(ROOT / 'orig'))} for t in targets]
    rows.sort(key=lambda r: (-r['coverage_pct'], -(r['match_pct'] or 0)))
    out = ROOT / 'progress.svg'
    out.write_text(progress_svg(rows))
    print(f'{out.relative_to(ROOT)}: {len(rows)} files')


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
    svg = sub.add_parser('svg')
    svg.add_argument('--no-match', action='store_true', help='skip compiling and instruction matching')
    svg.set_defaults(fn=cmd_svg)
    args = p.parse_args()
    args.fn(args)


if __name__ == '__main__':
    main()
