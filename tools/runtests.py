#!/usr/bin/env python3
"""Build and run the host tests in tools/tests/ under ASan/UBSan.

usage: tools/runtests.py [name ...]   run tests whose path contains a name (default: all)

A test is any tools/tests/**/*.{c,cc,cpp} with `int main(`. Lines of the form
`// @directive args`, each on its own line, configure it:
  @extract <file> <function>...  insert these function definitions from <file>
  @declare <file> <function>...  insert their prototypes
  @match <file> <regex>          insert every match of a Python regex in <file>
  @sources <path or glob>...     compile these files into the test too
  @cflags <flag>...              extra compiler flags
  @pkg <package>...              pkg-config packages
  @hangs <arg>...                the binary run with <arg> must not return within 2 s
A test that names a missing lg-opensource/ path or pkg-config package is skipped.
"""
import concurrent.futures
import os
import re
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TESTS = ROOT / 'tools/tests'
OUT = ROOT / 'build/obj/tests'
DIRECTIVE = re.compile(r'^[ \t]*//[ \t]*@(\w+)[ \t]*(.*?)[ \t]*$', re.M)
INSERTS = {'extract', 'declare', 'match'}
FLAGS = ['-O1', '-g', '-Wall', '-Wextra', '-Werror', '-Wno-unused-parameter',
         '-Wno-unused-function', '-Wno-unused-variable', '-Wno-unused-but-set-variable',
         '-Wno-unused-const-variable', '-Wno-missing-field-initializers', '-Wno-sign-compare',
         '-Wno-deprecated-declarations', '-fsanitize=address,undefined',
         '-fno-sanitize-recover=all', '-fno-omit-frame-pointer', '-ffunction-sections',
         '-fdata-sections', '-pthread',
         '-Wl,-dead_strip' if sys.platform == 'darwin' else '-Wl,--gc-sections']
TOKEN = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/|//[^\n]*', re.S)


class Skip(Exception):
    pass


def strip_comments(text):
    """Blank out comments, keeping line numbers and string literals."""
    return TOKEN.sub(lambda m: m[0] if m[0][0] in '"\'' else ' ' + '\n' * m[0].count('\n'), text)


def close(text, i):
    """Index after the bracket matching the opening bracket at text[i]."""
    opening, closing, depth = text[i], {'(': ')', '{': '}'}[text[i]], 0
    while True:
        c = text[i]
        if c in '"\'':
            i += 1
            while text[i] != c:
                i += 2 if text[i] == '\\' else 1
        elif c == opening:
            depth += 1
        elif c == closing:
            depth -= 1
            if not depth:
                return i + 1
        i += 1


def definition(text, name):
    """(start, signature end, end) of the single column-0 definition of name."""
    found = []
    for m in re.finditer(r'(?<![\w.>:])' + re.escape(name) + r'\s*\(', text):
        start = text.rfind('\n', 0, m.start()) + 1
        if start == m.start():  # return type on the previous line
            start = text.rfind('\n', 0, start - 1) + 1
        if not re.fullmatch(r'[A-Za-z_][\w \t*\n]*', text[start:m.start()]):
            continue
        brace = re.compile(r'\s*\{').match(text, close(text, m.end() - 1))
        if brace:
            found.append((start, brace.end() - 1, close(text, brace.end() - 1)))
    if len(found) != 1:
        raise ValueError(f'{name}: {len(found)} definitions')
    return found[0]


def insert(kind, args):
    file, rest = args.split(None, 1)
    raw = (ROOT / file).read_text()
    text = raw if kind == 'match' else strip_comments(raw)
    if kind == 'match':
        spans = [m.span() for m in re.finditer(rest, text)]
        if not spans:
            raise ValueError(f'{file}: no match for {rest}')
    else:
        spans = [definition(text, name) for name in rest.split()]
        spans = [(s, e if kind == 'extract' else sig) for s, sig, e in spans]
    parts = [f'#line {text.count(chr(10), 0, s) + 1} "{file}"\n{text[s:e]}'
             + (';' if kind == 'declare' else '') for s, e in spans]
    return '\n'.join(parts)


def run(path):
    try:
        return build_and_run(path)
    except Exception as error:  # one broken test must not hide the others' results
        return str(path.relative_to(TESTS).with_suffix('')), 'FAIL', repr(error)


def build_and_run(path):
    name = str(path.relative_to(TESTS).with_suffix(''))
    text = path.read_text()
    opts = {}
    for m in DIRECTIVE.finditer(text):
        opts.setdefault(m[1], []).append(m[2])
    flags = shlex.split(' '.join(opts.get('cflags', [])))
    sources = [str(p.relative_to(ROOT)) for pattern in ' '.join(opts.get('sources', [])).split()
               for p in sorted(ROOT.glob(pattern))]
    paths = [re.sub(r'^-(?:I|isystem)', '', f) for f in flags] + sources
    paths += [a.split(None, 1)[0] for k in INSERTS for a in opts.get(k, [])]
    try:
        for p in paths:
            if p.startswith('lg-opensource/') and not (ROOT / p).exists():
                raise Skip(f'missing {p}')
        packages = ' '.join(opts.get('pkg', [])).split()
        if packages:
            if not shutil.which('pkg-config'):
                raise Skip('pkg-config not installed')
            result = subprocess.run(['pkg-config', '--cflags', '--libs', *packages],
                                    capture_output=True, text=True)
            if result.returncode:
                raise Skip(f'pkg-config {" ".join(packages)} failed')
            flags += shlex.split(result.stdout)
    except Skip as skip:
        return name, 'SKIP', str(skip)
    binary = OUT / name
    binary.parent.mkdir(parents=True, exist_ok=True)
    source = path
    if INSERTS & opts.keys():
        source = binary.with_suffix(path.suffix)
        rel = path.relative_to(ROOT)
        lines = [f'#line 1 "{rel}"']
        for number, line in enumerate(text.split('\n'), 1):
            m = DIRECTIVE.match(line)
            if m and m[1] in INSERTS:
                try:
                    line = insert(m[1], m[2]) + f'\n#line {number + 1} "{rel}"'
                except (ValueError, OSError) as error:
                    return name, 'FAIL', f'{rel}:{number}: {error}'
            lines.append(line)
        source.write_text('\n'.join(lines))
    cxx = path.suffix != '.c'
    compiler = os.environ.get('CXX', 'c++') if cxx else os.environ.get('CC', 'cc')
    command = [compiler, '-std=c++17' if cxx else '-std=gnu11', *FLAGS,
               f'-I{path.parent.relative_to(ROOT)}', '-Isrc', str(source), *sources,
               '-o', str(binary), *flags, *([] if cxx else ['-lm'])]
    result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if result.returncode:
        return name, 'FAIL', shlex.join(command) + '\n' + result.stdout + result.stderr
    try:
        result = subprocess.run([binary], cwd=ROOT, capture_output=True, text=True, timeout=120)
    except subprocess.TimeoutExpired:
        return name, 'FAIL', 'timed out'
    if result.returncode:
        return name, 'FAIL', result.stdout + result.stderr
    for arg in ' '.join(opts.get('hangs', [])).split():
        try:
            subprocess.run([binary, arg], cwd=ROOT, capture_output=True, timeout=2)
            return name, 'FAIL', f'{name} {arg} returned instead of blocking'
        except subprocess.TimeoutExpired:
            pass
    return name, 'PASS', (result.stdout.strip().splitlines() or [''])[-1]


def main():
    names = sys.argv[1:]
    tests = [p for p in sorted(TESTS.rglob('*')) if p.suffix in ('.c', '.cc', '.cpp')
             and re.search(r'^int\s+main\s*\(', p.read_text(), re.M)
             and (not names or any(n in str(p.relative_to(TESTS)) for n in names))]
    failed = 0
    with concurrent.futures.ThreadPoolExecutor(os.cpu_count()) as pool:
        for name, status, detail in pool.map(run, tests):
            print(f'{status} {name}: {detail}' if status != 'FAIL' else f'FAIL {name}\n{detail}',
                  flush=True)
            failed += status == 'FAIL'
    print(f'{len(tests) - failed}/{len(tests)} tests did not fail')
    sys.exit(1 if failed or not tests else 0)


if __name__ == '__main__':
    main()
