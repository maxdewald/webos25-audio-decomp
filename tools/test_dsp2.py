#!/usr/bin/env python3
"""Syntax-check DSP sources and test recovered ES and discovery semantics.

usage: python3 tools/test_dsp2.py
Tests use mocked firmware services; they do not execute or verify DSP firmware.
"""
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent.parent


def run(args):
    print("+", " ".join(map(str, args)), flush=True)
    subprocess.run(args, cwd=ROOT, check=True)


def main():
    cc = shutil.which(os.environ.get("CC", "cc"))
    if cc is None:
        raise SystemExit("C compiler missing; set CC to an installed compiler")
    sources = [ROOT / "src/dsp2" / f"{name}.c"
               for name in ("gain", "delay", "debug", "glue")]
    run([cc, "-std=c11", "-Wall", "-Wextra", "-Werror", "-fsyntax-only", *sources])
    tests = ROOT / "tools/tests/dsp2"
    with tempfile.TemporaryDirectory(prefix="dsp2-tests-") as temporary:
        for name in ("gain", "delay", "transport", "debug"):
            output = Path(temporary) / name
            inputs = [tests / f"{name}.c"]
            if name == "delay":
                inputs.append(tests / "delay_stubs.c")
            flags = ["-Wl,-dead_strip"] if sys.platform == "darwin" else ["-Wl,--gc-sections"]
            run([cc, "-std=c11", "-Wall", "-Wextra", "-Werror", "-ffunction-sections",
                 "-fdata-sections", *flags, *inputs, "-o", output])
            run([output])
    run(["sh", "-n", ROOT / "tools/xtobjdump.sh"])
    print("PASS: source syntax, ES/discovery semantics and decoder shell syntax", flush=True)


if __name__ == "__main__":
    main()
