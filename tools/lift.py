#!/usr/bin/env python3
"""Lift Ghidra's decompiler output into C that compiles with the original toolchain.

usage: lift.py <target>        (needs build/lift/<binary>.tokens.jsonl + .types.json,
                                 written by tools/ExportTokens.java via tools/lift.sh)

Writes <src>/lifted/<module>.c (one file per original source file, from the __FILE__
strings in the code) plus <src>/lifted/decls.h. Every function is tagged with its Ghidra
address and gets the original symbol name through an asm label, so progress.py scores it
like any hand-written reconstruction. Functions already tagged in hand-written files of
<src>/ are skipped: move a function out of lifted/ by rewriting it by hand, then re-lift.

The output is mechanical: byte offsets instead of fields, Ghidra's variable names, no
C++ classes. It is a compilable baseline, not a clean reconstruction.
"""
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
                if f['bits'] or ft is None or ft['size'] <= 0 or f['off'] < cur:
                    continue
                if ft['k'] in ('struct', 'union') and ft.get('fields') and ft['size'] > 1:
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
    out_dir = src / 'lifted'
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
            fn_ident[k] = f'{base}_{k[1]:x}' if k[0] == 'addr' else f'{base}_{abs(hash(k[1])) % 0xfffff:x}'
    for k, lab in list(fn_label.items()):
        if lab == fn_ident[k]:
            fn_label[k] = None

    # globals: one C name per address
    gnames, gtext = {}, defaultdict(set)
    for g in meta['globals']:
        gtext[re.sub(r'.*::', '', g['text'])].add(g['addr'])
    gdecl = {}
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
        r = lift_function(d, types, fn_ident, fn_label, fn_ref, gnames, elf_funcs)
        if r is None:
            continue
        header, body, sigcomment = r
        k = ('addr', addr)
        label = fn_label.get(k)
        protos[k] = header + (f' __asm__("{label}")' if label else '') + ';'
        note = sigcomment or name
        modules[module].append(f'/* @0x{addr:x}: {note} */\n{header}\n{body}\n')
        n_lifted += 1

    for k, f in sigs.items():
        if k in protos:
            continue
        ret = 'uint32_t' if types.get(f['ret']) is None or types.get(f['ret'])['name'] == 'undefined' \
            else types.c(f['ret'])
        if f['src'] == 'DEFAULT' and not f['params']:
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
           '#pragma once', '#include "ghidra.h"', '']
    hdr += types.struct_defs() + ['']
    hdr += sorted(set(gdecl.values())) + ['']
    hdr += [protos[k] for k in sorted(protos, key=lambda k: (k[0], str(k[1])))]
    (out_dir / 'decls.h').write_text('\n'.join(hdr) + '\n')
    for mod, funcs in sorted(modules.items()):
        text = (f'/* Lifted mechanically from {t["binary"]} ({mod}) by tools/lift.py.\n'
                f' * Ghidra pseudocode made compilable; not yet hand-reconstructed. */\n'
                f'#include "decls.h"\n\n' + '\n'.join(funcs))
        (out_dir / f'{mod}.c').write_text(text)
    print(f'lifted {n_lifted} functions into {len(modules)} files, skipped {len(skip)} hand-written')


def safe_param(n, i):
    return n if n and IDENT.match(n) else f'param_{i + 1}'


def lift_function(d, types, fn_ident, fn_label, fn_ref, gnames, elf_funcs):
    toks = d['toks']
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
    for j in sig:
        x = toks[j]
        if x[0] == 'Syntax' and x[1].strip() and x[1] != '::' and j in nxt and toks[nxt[j]][1] == '::':
            drop.add(j)
            drop.add(nxt[j])
        elif x[0] == 'Syntax' and x[1] in ('__thiscall', '__stdcall', '__cdecl'):
            drop.add(j)

    declared = set()
    undeclared = {}
    instack = {}
    out = []
    prev_sig = None
    for j, x in enumerate(toks):
        if j in drop or (j < first_fn and x[0] == 'Comment'):
            continue
        kind, text = x[0], x[1]
        info = x[2] if len(x) > 2 else {}
        if kind == 'Break':
            out.append(('\n' + ' ' * info.get('indent', 0)) if j > body_start else ' ')
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
            elif 'fn' in info and fn_ref(info['fn']):
                out.append(fn_ident[fn_ref(info['fn'])])
            elif 'g' in info and info['g'] in gnames:
                out.append(gnames[info['g']])
            else:
                v = text.split('::')[-1]
                if not IDENT.match(v):
                    v = re.sub(r'\W', '_', v)
                ps = prev_sig
                if ps is not None and ps[0] in ('Type',) or (ps is not None and ps[1] == '*' and
                                                           out and out[-1] == '*' and _after_type(out)):
                    declared.add(v)
                else:
                    undeclared.setdefault(v, info.get('t'))
                m = INSTACK.match(v)
                if m:
                    instack[v] = (int(m[1], 16), info.get('t'))
                out.append(v)
        elif kind == 'Syntax' and PIECE.match(text) and out and out[-1] == '.':
            m = PIECE.match(text)
            out.pop()
            # rewrite "<lvalue>._A_B_" as PIECE(lvalue, A, B)
            expr = []
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
            out.append(f'PIECE({"".join(expr)},{m[1]},{m[2]})')
        elif kind == 'Label' and text and out and out[-1].startswith('\n'):
            out.append(text)
            out.append(':;')
            toks[j + 1:j + 2] = [['Syntax', '']] if j + 1 < len(toks) and toks[j + 1][1] == ':' else toks[j + 1:j + 2]
        else:
            out.append(text)
        if kind != 'Break' and text.strip():
            prev_sig = (kind, text)

    code = ''.join(out)
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
            if off % 4 or off < slot:
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
    missing = [v for v in undeclared if v not in declared and v not in params]
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
