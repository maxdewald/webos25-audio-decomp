#!/usr/bin/env python3
"""Lift Ghidra's decompiler output into C that compiles with the original toolchain.

usage: lift.py <target>        (needs build/lift/<binary>.tokens.jsonl + .types.json,
                                 written by tools/ExportTokens.java via tools/lift.sh)

Writes build/lift/<target>/<module>.c plus decls.h. Ghidra addresses and original
symbol labels are retained for comparison, but generated files are excluded from
source coverage. Functions tagged in hand-written src/<target>/ files are skipped.

The output is mechanical: byte offsets instead of fields, Ghidra's variable names, no
C++ classes. It is a compilable baseline, not a clean reconstruction.
"""
import hashlib
import json
import re
import subprocess
import sys
import tomllib
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CFG = tomllib.loads((ROOT / 'decomp.toml').read_text())
LLVM = '/opt/homebrew/opt/llvm/bin/'
IGNORE = re.compile(r'^(_init|_fini|_DT_INIT|_DT_FINI|_FINI_\d+|_INIT_\d+|call_weak_fn|'
                    r'(de)?register_tm_clones|__do_global_dtors_aux|frame_dummy)$')
IDENT = re.compile(r'^[A-Za-z_]\w*$')
PIECE = re.compile(r'^_(\d+)_(\d+)_$')
INSTACK = re.compile(r'^in_stack_([0-9a-f]{8})$')
LIB_PREFIX = (('boost::', 'boost'), ('std::', 'stl'), ('__gnu_cxx::', 'stl'),
              ('rapidjson::', 'rapidjson'))


def target(name):
    for t in CFG['target']:
        if t['name'] == name:
            return t
    sys.exit(f'unknown target {name}')


class Types:
    def __init__(self, types):
        self.t = types
        self.structs = {}      # type id -> C name, for composites with fields

    def get(self, tid):
        return self.t.get(tid) if tid else None

    def c(self, tid, text=None):
        """C spelling of a base type (no pointer stars)."""
        if text == 'code':
            return 'code'
        d = self.get(tid)
        if d is None:
            return 'uint32_t'
        k, size, name = d['k'], d['size'], d['name']
        if k == 'td':
            return self.c(d['t'])
        if k == 'ptr':
            inner = self.get(d.get('t'))
            if inner is not None and inner['k'] == 'fn':
                return 'code *'
            return (self.c(d.get('t')) if d.get('t') else 'void') + ' *'
        if k == 'arr':
            return self.c(d['t'])
        if k == 'fn':
            return 'code'
        if k == 'float':
            return {4: 'float', 8: 'double'}.get(size, 'long double')
        if k == 'int':
            if name == 'bool':
                return '_Bool'
            if name == 'char':
                return 'char'
            s = {1: 8, 2: 16, 4: 32, 8: 64}.get(size, 32)
            return f'{"" if d.get("signed") else "u"}int{s}_t'
        if k in ('struct', 'union'):
            if not d.get('fields') or size <= 1:
                return 'uint8_t'
            return self.struct_name(tid)
        if k == 'enum':
            return {1: 'int8_t', 2: 'int16_t', 8: 'int64_t'}.get(size, 'int32_t')
        if name == 'void':
            return 'void'
        if name == 'string':
            return 'char'
        m = re.match(r'(?:undefined|u?int)(\d)$', name)
        n = int(m[1]) if m else max(size, 1)
        return {1: 'uint8_t', 2: 'uint16_t', 3: 'uint32_t', 4: 'uint32_t'}.get(n, 'uint64_t')

    def struct_name(self, tid):
        if tid not in self.structs:
            base = re.sub(r'\W', '_', self.t[tid]['name'])
            name = f'gs_{base}'
            while name in self.structs.values():
                name += '_'
            self.structs[tid] = name
        return self.structs[tid]

    def struct_defs(self):
        out, done = [], set()

        def emit(tid):
            if tid in done:
                return
            done.add(tid)
            d = self.t[tid]
            fields, cur = [], 0
            for i, f in enumerate(d['fields']):
                ft = self.get(f['t'])
                while ft is not None and ft['k'] == 'td':
                    f = dict(f, t=ft['t'])
                    ft = self.get(f['t'])
                if f['bits'] or ft is None or ft['size'] <= 0 or f['off'] < cur:
                    continue
                if ft['k'] in ('struct', 'union') and ft.get('fields') and ft['size'] > 1:
                    self.struct_name(f['t'])
                    emit(f['t'])
                if d['k'] == 'struct' and f['off'] > cur:
                    fields.append(f'uint8_t _pad{cur:x}[{f["off"] - cur}];')
                fname = re.sub(r'\W', '_', f['name'] or f'field_{f["off"]:x}')
                if ft['k'] == 'arr':
                    fields.append(f'{self.c(ft["t"])} {fname}[{ft["n"]}];')
                else:
                    fields.append(f'{self.c(f["t"])} {fname};')
                if d['k'] == 'struct':
                    cur = f['off'] + ft['size']
            if d['k'] == 'struct' and d['size'] > cur:
                fields.append(f'uint8_t _pad{cur:x}[{d["size"] - cur}];')
            body = ' '.join(fields) or 'uint8_t _empty;'
            out.append(f'typedef {d["k"]} __attribute__((packed)) {self.structs[tid]} '
                       f'{{ {body} }} {self.structs[tid]};')

        for tid in list(self.structs):
            emit(tid)
        fwd = [f'typedef {self.t[t]["k"]} {n} {n};' for t, n in self.structs.items()]
        return fwd + out


def elf_symbols(binary, bias):
    """addr -> ELF symbol (functions at even addresses, data as-is), Ghidra addresses."""
    funcs, data = {}, {}
    r = subprocess.run([LLVM + 'llvm-nm', '-D', '--defined-only', str(binary)],
                       capture_output=True, text=True).stdout
    for line in r.splitlines():
        p = line.split()
        if len(p) != 3:
            continue
        a = int(p[0], 16)
        if p[1] in 'TtWw' and a & 1:
            funcs.setdefault((a & ~1) + bias, p[2])
        elif p[1] in 'TtWw':
            funcs.setdefault(a + bias, p[2])
        else:
            data.setdefault(a + bias, p[2])
    return funcs, data


def ident(name):
    """Readable C identifier for a Ghidra qualified name."""
    n = name.replace('<EXTERNAL>::', '')
    if n.startswith('mediapipeline::'):
        n = n[len('mediapipeline::'):]
    depth, s = 0, []
    for ch in n:
        if ch == '<':
            depth += 1
        elif ch == '>':
            depth -= 1
        elif depth == 0:
            s.append(ch)
    n = ''.join(s)
    parts = n.split('::')
    if len(parts) >= 2 and parts[-1] == parts[-2]:
        parts[-1] = 'ctor'
    if parts[-1].startswith('~'):
        parts[-1] = 'dtor'
    n = '_'.join(parts)
    n = re.sub(r'operator\W+', 'operator_', n)
    n = re.sub(r'\W+', '_', n).strip('_') or 'anon'
    return n if not n[0].isdigit() else '_' + n


def main():
    tname = sys.argv[1]
    t = target(tname)
    binary = ROOT / t['binary']
    src = ROOT / t['src']
    out_dir = ROOT / 'build/lift' / tname
    stem = binary.name
    toks_path = ROOT / 'build/lift' / f'{stem}.tokens.jsonl'
    meta = json.loads((ROOT / 'build/lift' / f'{stem}.types.json').read_text())
    types = Types(meta['types'])
    bias = t.get('image_bias', 0x10000)
    elf_funcs, elf_data = elf_symbols(binary, bias)

    # functions already reconstructed by hand
    skip = set()
    for f in src.rglob('*'):
        if f.suffix in ('.c', '.cpp') and 'lifted' not in f.parts:
            for m in re.finditer(r'^/\*\s*@0x([0-9a-fA-F]+)', f.read_text(), re.M):
                skip.add(int(m[1], 16) + t.get('src_addr_bias', 0))

    # identifiers for every known function
    fn_ident, fn_label, used = {}, {}, Counter()
    def key_of(f):
        return ('ext', f['import']) if f['ext'] else ('addr', f['addr'])
    sigs = {}
    for f in meta['funcs']:
        k = key_of(f)
        if k in sigs:
            continue
        sigs[k] = f
        if f['ext']:
            imp = f['import'] or f['name']
            fn_label[k] = imp if not IDENT.match(imp) or imp.startswith('_Z') else None
            base = imp if IDENT.match(imp) and not imp.startswith('_Z') else ident(f['name'])
        else:
            fn_label[k] = elf_funcs.get(f['addr'])
            base = ident(f['name'])
        fn_ident[k] = base
        used[base] += 1
    for k, base in fn_ident.items():
        if used[base] > 1 or base in ('main',):
            fn_ident[k] = f'{base}_{k[1]:x}' if k[0] == 'addr' else f'{base}_{hashlib.sha256(k[1].encode()).hexdigest()[:8]}'
    for k, lab in list(fn_label.items()):
        if lab == fn_ident[k]:
            fn_label[k] = None

    # typeinfo / vtable symbols by demangled class name
    statics, static_decls = {}, []
    allsyms = subprocess.run([LLVM + 'llvm-nm', '-D', str(binary)], capture_output=True, text=True).stdout.split()
    tv = sorted({x.split('@')[0] for x in allsyms if x.startswith(('_ZTI', '_ZTV'))})
    dem = subprocess.run([LLVM + 'llvm-cxxfilt'], input='\n'.join(tv), capture_output=True, text=True).stdout.splitlines()
    for sym, dn in zip(tv, dem):
        kind = 'typeinfo' if sym.startswith('_ZTI') else 'vtable'
        cls = re.sub(r'^(typeinfo|vtable) for ', '', dn).replace(' ', '')
        sym = sym.split('@')[0]
        cname = 'g_' + re.sub(r'\W', '_', sym)
        statics[(cls, kind)] = cname
        static_decls.append(f'extern uint8_t {cname}[] __asm__("{sym}");')

    # most common use type of each global across all functions
    use_types = defaultdict(Counter)
    for line in open(toks_path):
        for x in json.loads(line).get('toks') or ():
            if x[0] == 'Variable' and len(x) > 2 and 'g' in x[2] and x[2].get('t'):
                use_types[x[2]['g']][x[2]['t']] += 1
    for g in meta['globals']:
        ut = use_types.get(g['addr'])
        if ut and not g['t']:
            ptrs = [t for t, _ in ut.most_common() if (types.get(t) or {}).get('k') == 'ptr']
            g['t'] = ptrs[0] if ptrs else ut.most_common(1)[0][0]

    # globals: one C name per address
    gnames, gtext = {}, defaultdict(set)
    for g in meta['globals']:
        gtext[re.sub(r'.*::', '', g['text'])].add(g['addr'])
    gdecl = {}
    scalar_ok = set()
    for g in meta['globals']:
        text = re.sub(r'.*::', '', g['text'])
        name = text if len(gtext[text]) == 1 and IDENT.match(text) else f'{re.sub(r"\W", "_", text)}_{g["addr"]:x}'
        if name in fn_ident.values():
            name = f'g_{name}'
        gnames[g['addr']] = name
        dt = types.get(g['data'])
        label = elf_data.get(g['addr']) or elf_funcs.get(g['addr'])
        if dt is not None and dt['k'] == 'arr':
            decl = f'extern {types.c(dt["t"])} {name}[{dt["n"]}]'
        elif g['t']:
            decl = f'extern {types.c(g["t"])} {name}'
        elif dt is not None and dt['k'] not in ('base',) and dt['size'] > 0:
            decl = f'extern {types.c(g["data"])} {name}'
        elif dt is not None and dt['k'] in ('base', 'int') and 0 < dt['size'] <= 8:
            decl = f'extern {types.c(g["data"])} {name}'
            scalar_ok.add(name)
        elif dt is not None and dt['k'] == 'arr':
            decl = f'extern {types.c(dt["t"])} {name}[{dt["n"]}]'
        else:
            decl = f'extern uint8_t {name}[]'
        if label and label != name:
            decl += f' __asm__("{label}")'
        gdecl[g['addr']] = decl + ';'

    def fn_ref(ref):
        if not ref:
            return None
        k = ('ext', ref['ext']) if 'ext' in ref else ('addr', ref['addr'])
        return k if k in fn_ident else None

    # ---- per-function lifting ----
    modules = defaultdict(list)
    protos = {}
    cur_module = 'runtime'
    n_lifted = 0
    for line in open(toks_path):
        d = json.loads(line)
        addr, name = d['addr'], d['name']
        toks = d.get('toks')
        files = [x[1] for x in (toks or []) if x[0] == 'Variable' and x[1].startswith('"')
                 and re.match(r'^"[\w\-]+\.(cpp|cc|c)"$', x[1])]
        if files:
            cur_module = Counter(files).most_common(1)[0][0].strip('"').rsplit('.', 1)[0]
        module = cur_module
        for pre, mod in LIB_PREFIX:
            if name.startswith(pre):
                module = mod
        if IGNORE.match(name) or addr in skip or toks is None:
            continue
        r = lift_function(d, types, fn_ident, fn_label, fn_ref, gnames, elf_funcs, scalar_ok, statics, set(fn_ident.values()))
        if r is None:
            continue
        header, body, sigcomment = r
        k = ('addr', addr)
        label = fn_label.get(k)
        # Unprototyped: Ghidra disagrees with itself about arity between call sites.
        proto = re.sub(r'\(.*\)$', '()', header)
        proto = re.sub(r'^void\s+(?!\*)', 'uint32_t ', proto)
        protos[k] = proto + (f' __asm__("{label}")' if label else '') + ';'
        header = re.sub(r'^void\s+(?!\*)', 'uint32_t ', header)
        note = sigcomment or name
        fid = fn_ident[k]
        sym = label or fid
        # Callers see an unprototyped "fid()" (Ghidra's arity differs between call
        # sites); the definition is a separate C name bound to the same symbol.
        protos[k] = re.sub(r'__asm__\("[^"]*"\);$', '', protos[k]).rstrip(';').rstrip() + f' __asm__("{sym}");'
        header = re.sub(rf'\b{re.escape(fid)}\s*\(', f'{fid}__def(', header, count=1)
        modules[module].append(f'{header} __asm__("{sym}");\n/* @0x{addr:x}: {note} */\n{header}\n{body}\n')
        n_lifted += 1

    for k, f in sigs.items():
        if k in protos:
            continue
        ret = 'uint32_t' if types.get(f['ret']) is None or types.get(f['ret'])['name'] == 'undefined' \
            else types.c(f['ret'])
        if not f['ext'] or (f['src'] != 'IMPORTED'):
            params = ''
        else:
            ps = [f'{types.c(p["t"])} {safe_param(p["name"], i)}' for i, p in enumerate(f['params'])]
            if f['varargs']:
                ps.append('...')
            params = ', '.join(ps) if ps else 'void'
        label = fn_label.get(k)
        protos[k] = f'{ret} {fn_ident[k]}({params})' + (f' __asm__("{label}")' if label else '') + ';'

    out_dir.mkdir(parents=True, exist_ok=True)
    for old in out_dir.glob('*.c'):
        old.unlink()
    hdr = [f'/* Generated by tools/lift.py from {t["binary"]}: declarations for lifted/*.c. */',
           '#pragma once', '#include "ghidra.h"',
           '']
    hdr += types.struct_defs() + ['']
    hdr += sorted(set(gdecl.values())) + static_decls + ['']
    for k in sorted(protos, key=lambda k: (k[0], str(k[1]))):
        hdr.append(protos[k])
    (out_dir / 'decls.h').write_text('\n'.join(hdr) + '\n')
    for mod, funcs in sorted(modules.items()):
        text = (f'/* Lifted mechanically from {t["binary"]} ({mod}) by tools/lift.py.\n'
                f' * Ghidra pseudocode made compilable; not yet hand-reconstructed. */\n'
                +
                f'#include "decls.h"\n\n' + '\n'.join(funcs))
        (out_dir / f'{mod}.c').write_text(text)
    print(f'lifted {n_lifted} functions into {len(modules)} files, skipped {len(skip)} hand-written')


def safe_param(n, i):
    return n if n and IDENT.match(n) else f'param_{i + 1}'


def lift_function(d, types, fn_ident, fn_label, fn_ref, gnames, elf_funcs, scalars=frozenset(), statics={}, fn_names=frozenset()):
    toks = d['toks']
    rt = next((x for x in toks if x[0] == 'Type'), None)
    ret_c = types.c(rt[2].get('t'), rt[1]) if rt and len(rt) > 2 else 'uint32_t'
    if ret_c == 'void':
        ret_c = 'int'
    # signature comment (demangled prototype) before the definition
    first_fn = next((i for i, x in enumerate(toks) if x[0] == 'FuncName' and not
                     (len(x) > 2 and x[2].get('call'))), None)
    if first_fn is None:
        return None
    sigc = ' '.join(x[1] for x in toks[:first_fn] if x[0] == 'Comment')
    sigc = re.sub(r'\s+', ' ', sigc.replace('/*', '').replace('*/', '')).strip()
    body_start = next(i for i in range(first_fn, len(toks)) if toks[i][0] == 'Syntax' and toks[i][1] == '{')

    # drop qualifier runs (ns::Cls::) before names, and Ghidra-only keywords
    keep = []
    i = 0
    sig = [j for j, x in enumerate(toks) if not (x[0] == 'Break' or (x[0] == 'Syntax' and not x[1].strip()))]
    nxt = {sig[a]: sig[a + 1] for a in range(len(sig) - 1)}
    drop = set()
    for a, j in enumerate(sig):
        x = toks[j]
        if x[0] == 'Syntax' and x[1] in ('__thiscall', '__stdcall', '__cdecl'):
            drop.add(j)
        elif x[1] == '::':
            drop.add(j)
            if a > 0 and toks[sig[a - 1]][0] in ('Syntax', 'Op') and toks[sig[a - 1]][1] not in \
                    (';', '{', '}', '(', ')', ',', '=', '::') and re.match(r'^[\w~(]', toks[sig[a - 1]][1]):
                drop.add(sig[a - 1])
    # Qualified statics ("Cls::typeinfo", "Cls::vtable"): remember the qualifier so the
    # reference can be resolved to its ELF symbol.
    qual = {}
    for a, j in enumerate(sig):
        if toks[j][0] == 'Variable' and toks[j][1] in ('typeinfo', 'vtable') and a >= 2 \
                and toks[sig[a - 1]][1] == '::':
            q, b = [], a - 1
            while b >= 1 and toks[sig[b]][1] == '::':
                q.insert(0, toks[sig[b - 1]][1])
                b -= 2
            qual[j] = '::'.join(q)

    declared = set()
    undeclared = {}
    instack = {}
    out = []
    prev_sig = None
    decl_ctx = False
    for j, x in enumerate(toks):
        if j in drop or (j < first_fn and x[0] == 'Comment'):
            continue
        kind, text = x[0], x[1]
        info = x[2] if len(x) > 2 else {}
        if kind == 'Break':
            if out and out[-1] in ('.', '->', 'goto'):
                continue
            out.append(('\n' + ' ' * info.get('indent', 0)) if j > body_start else ' ')
            continue
        if kind == 'Label':
            text = re.sub(r'\W', '_', text)
        if kind == 'Syntax' and text == '':
            continue
        if kind == 'Type':
            out.append(types.c(info.get('t'), text))
        elif kind == 'FuncName':
            k = fn_ref(info.get('fn'))
            out.append(fn_ident[k] if k else ident(text))
        elif kind == 'Variable':
            if text.startswith('"') or text.startswith("'"):
                out.append(text)
            elif info.get('hv') == 'HighConstant' or re.match(r'^-?(0x[0-9a-fA-F]+|\d)', text):
                if types.get(info.get('t')) and types.get(info.get('t'))['name'] == 'float' \
                        and re.match(r'^-?\d+\.\d*(e[-+]?\d+)?$', text):
                    text += 'f'
                out.append(text)
            elif j in qual and (qual[j], text) in statics:
                out.append(f'(*(uint8_t (*)[])&{statics[(qual[j], text)]})')
                prev_sig = (kind, text)
                continue
            elif 'fn' in info and fn_ref(info['fn']):
                out.append(fn_ident[fn_ref(info['fn'])])
            elif 'g' in info and info['g'] in gnames:
                out.append(gnames[info['g']])
            else:
                v = text.split('::')[-1]
                if not IDENT.match(v):
                    v = re.sub(r'\W', '_', v)
                if decl_ctx:
                    declared.add(v)
                else:
                    undeclared.setdefault(v, info.get('t'))
                m = INSTACK.match(v)
                if m:
                    instack[v] = (int(m[1], 16), info.get('t'))
                out.append(v)
        elif kind == 'Syntax' and PIECE.match(text) and out and out[-1] in ('.', '->'):
            arrow = out[-1] == '->'
            m = PIECE.match(text)
            out.pop()
            # rewrite "<lvalue>._A_B_" as PIECE(lvalue, A, B)
            expr = []
            while out and not out[-1].strip():
                out.pop()
            while out and (IDENT.match(out[-1]) or out[-1] in (')', ']')):
                if out[-1] in (')', ']'):
                    depth, buf = 0, []
                    while out:
                        tkn = out.pop()
                        buf.append(tkn)
                        depth += tkn.count(')') + tkn.count(']') - tkn.count('(') - tkn.count('[')
                        if depth <= 0:
                            break
                    expr = buf[::-1] + expr
                    if out and IDENT.match(out[-1]):
                        expr = [out.pop()] + expr
                    break
                expr = [out.pop()] + expr
                break
            e = ''.join(expr)
            out.append(f'PIECE({"(*" + e + ")" if arrow else e},{m[1]},{m[2]})')
        elif kind == 'Syntax' and text == ':' and prev_sig is not None and prev_sig[0] == 'Label':
            out.append(':;')
        else:
            out.append(text)
        if kind != 'Break' and text.strip():
            prev_sig = (kind, text)
            if kind == 'Type':
                decl_ctx = True
            elif text in (';', '{', '}', '=', ')') or kind in ('Op',) and text not in ('*',):
                decl_ctx = False
            elif kind == 'Syntax' and text in ('(', ','):
                decl_ctx = False

    code = ''.join(out)
    code = re.sub(r'\(&(DAT_[0-9a-f]+|\w+_[0-9a-f]{5,6})\)\[', lambda m: m[0] if m[1] in scalars else f'{m[1]}[', code)
    # byte offsets into untyped data: &DAT_x + n  ->  (uint8_t *)DAT_x + n
    code = re.sub(r'\(&(DAT_[0-9a-f]+) \+', r'((uint8_t *)\1 +', code)
    code = re.sub(r'&(DAT_[0-9a-f]+) \+', r'(uint8_t *)\1 +', code)
    code = re.sub(r'\(&(__digits)\)\[', r'(\1)[', code)
    # Ghidra names GOT slots after an import even when they hold a vtable (iostream
    # virtual-base offsets: "x + vtbl[-3]"); index the slot as a pointer array.
    code = re.sub(r'(?<![\w.])(\w+)\[-3\]', r'((int32_t *)*(void **)&\1)[-3]', code)
    # stores of 0 through a code* ("*(code *)p = (code)0x0")
    code = re.sub(r'\*\(code \*\)(\([^;]*\) = \(code\)0x0)', r'*(uint8_t *)\1', code).replace('(code)0x0', '0')
    code = re.sub(r'\(&__digits\)\[', '(__digits)[', code)
    code = re.sub(r'\(\s*&\s*__digits\s*\)', '(__digits)', code)
    # duplicate join labels (Ghidra duplicates the block): rename later copies
    labs = re.findall(r'^\s*(\w+):;', code, re.M)
    for lab, n in Counter(labs).items():
        if n > 1:
            parts = re.split(rf'(^\s*{lab}:;)', code, flags=re.M)
            k = 0
            for i in range(1, len(parts), 2):
                k += 1
                if k > 1:
                    parts[i] = parts[i].replace(f'{lab}:;', f'{lab}_dup{k}:;')
                    # gotos after this copy until the next copy jump here
                    parts[i + 1] = re.sub(rf'goto {lab};', f'goto {lab}_dup{k};', parts[i + 1])
            code = ''.join(parts)
    labels = set(re.findall(r'^\s*(\w+):;', code, re.M))
    code = re.sub(r'goto\s+(\w+);', lambda m: m[0] if m[1] in labels else
                  f'/* tail call */ return (__typeof__(({ret_c})0)){m[1]}();', code)
    head, sep, body = code.partition('{')
    head = re.sub(r'\s+', ' ', head).strip()
    head = re.sub(r'\(\s*', '(', head)
    head = re.sub(r'\s*\)', ')', head)
    head = re.sub(r'\s*,\s*', ', ', head)
    head = re.sub(r'\s+\*', ' *', head)
    head = head.replace('( void)', '(void)')
    # stack-passed arguments Ghidra did not put in the signature
    nparams = 0 if head.endswith('(void)') else head[head.index('(') + 1:-1].count(',') + 1
    extra = []
    if instack and '...' not in head:
        slot = max(nparams - 4, 0) * 4
        for v, (off, tid) in sorted(instack.items(), key=lambda kv: kv[1][0]):
            size = types.get(tid)['size'] if types.get(tid) else 4
            if off % 4 or off < slot or off > 0x100:
                continue
            while slot < off:
                extra.append(f'uint32_t _stack_{slot:x}')
                slot += 4
            extra.append(f'{types.c(tid)} {v}')
            slot += max(4, size)
        if extra:
            sep_ = '' if nparams == 0 else ', '
            head = head[:-1].replace('(void', '(') + sep_ + ', '.join(extra) + ')'
            head = head.replace('(, ', '(')
    params = set(re.findall(r'(\w+)\s*(?:,|\)$)', head))
    body = sep + body
    # drop local declarations of variables that became parameters
    for e in extra:
        v = e.split()[-1]
        body = re.sub(rf'\n\s*[\w ]+\**\s*{re.escape(v)};', '', body, count=1)
    # Ghidra reuses one stack slot as integer and object pointer ("local_220[0x6ec] = 1").
    # Retype only stack locals declared as 4-byte ints that are subscripted directly.
    # AbstractPlayer::getPipeline() returns boost::shared_ptr<Pipeline> through a hidden
    # pointer (r0) with this in r1; Ghidra drops both. The result is the first local read
    # after the call.
    def _sret(m):
        after = body[m.end():m.end() + 400]
        loc = re.search(r'\b((?:local|[a-z]Stack)_[0-9a-f]+)\b', after)
        if not loc:
            return m[0]
        return f'{m[1]}AbstractPlayer_getPipeline(&{loc[1]}, this);'
    if re.search(r'\bthis\b', head):
        body = re.sub(r'^(\s*)AbstractPlayer_getPipeline\(\);', _sret, body, flags=re.M)
    cand = set(re.findall(r'(?<![&\w])((?:local|[a-z]Stack)_\w+)\[(?:0x|\d)', body))
    cand |= set(re.findall(r'(?:^|[(=,;!&|?:]|return)\s*\*\s*((?:local|[a-z]Stack)_\w+)\s*(?:[+=;,)]|\s)', body, re.M))
    for v in cand:
        body = re.sub(rf'^(\s*)u?int32_t {v};', rf'\1uint8_t *{v};', body, count=1, flags=re.M)
        body = re.sub(rf'\(\*\*\(code \*\*\)\(\*{v} \+', f'(**(code **)(*(uint8_t **){v} +', body)
        body = re.sub(rf'\*\(code \*\*\)\(\*{v} \+', f'*(code **)(*(uint8_t **){v} +', body)
    body = re.sub(r'(\w+) = \*\(char \(\*\) \[(\d+)\]\)\(([^;]+)\);', r'__builtin_memcpy(\1, (void *)(\3), \2);', body)
    # float casts of locals that were retyped as pointers / ints: reinterpret bits
    body = re.sub(r'\(float\)(local_\w+)\b(?!\s*[\[(])', r'PUN_F(\1)', body) if re.search(r'uint8_t \*local_', body) else body
    missing = [v for v in undeclared if v not in declared and v not in params]
    # Remaining names Ghidra could not bind: an import name used as a value (a GOT slot
    # it mislabelled), or a static whose class was lost. Bind to a placeholder symbol.
    for v in [v for v in missing if v in ('typeinfo', 'vtable') or v in fn_names]:
        missing.remove(v)
        body = '{\n  extern uint8_t ' + v + '__ref[] __asm__("' + v + '__unresolved");' + \
            re.sub(rf'\b{v}\b', f'(*(uint32_t *){v}__ref)', body[1:])
    if missing:
        decl = ''.join(f'\n  {types.c(undeclared[v])} {v};' for v in missing)
        body = '{' + decl + body[1:]
    return head, body, d['name'] if not d['name'] else (d['name'] if not sigc else sigc)


def _after_type(out):
    for tkn in reversed(out[:-1]):
        if tkn.strip() in ('', '*'):
            continue
        return re.match(r'^(u?int\d+_t|char|float|double|void|_Bool|code|gs_\w+|long double)$', tkn) is not None
    return False


if __name__ == '__main__':
    main()
