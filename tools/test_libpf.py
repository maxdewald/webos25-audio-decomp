#!/usr/bin/env python3
"""Run libpf and calculator host regression tests (no target execution)."""
from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent.parent


def main():
    compiler = shutil.which('clang++') or shutil.which('g++')
    if compiler is None:
        sys.exit('a host C++17 compiler is required')
    scratch = ROOT / 'build/scratch'
    scratch.mkdir(parents=True, exist_ok=True)
    implementation = '\n'.join((ROOT / 'src/libpf' / name).read_text()
                               for name in ('resource_delivery.cpp', 'airplayer.cpp'))
    implementation = implementation.replace('#include "libpf.h"', '')
    harness = (ROOT / 'tools/test_libpf_resource.cpp').read_text()
    with tempfile.TemporaryDirectory(prefix='libpf-test-', dir=scratch) as directory:
        source = Path(directory) / 'resource_test.cpp'
        binary = Path(directory) / 'resource_test'
        source.write_text(harness.replace('// RESOURCE_DELIVERY_IMPLEMENTATION', implementation))
        subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Wno-unused-parameter',
                        '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
                        '-g', str(source), '-o', str(binary)],
                       check=True)
        subprocess.run([str(binary)], check=True)
        flags = shlex.split(subprocess.check_output(
            ['pkg-config', '--cflags', '--libs', 'glib-2.0'], text=True))
        # Only the platform logging/GStreamer header is mocked; Boost, RapidJSON,
        # GLib and the reconstructed implementation execute unchanged.
        log_header = (ROOT / 'src/libpf/libpf.h').read_text()
        log_header = log_header[log_header.index('/* PmLogLib'):log_header.index('/* Custom event')]
        options = Path(directory) / 'options.cpp'
        options.write_text((ROOT / 'src/libpf/options.cpp').read_text().replace(
            '#include "libpf.h"', '''#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/syscall.h>
#ifndef SYS_gettid
#define SYS_gettid SYS_getpid
#endif
static long libpf_test_syscall(long) { return static_cast<long>(getpid()); }
#define syscall libpf_test_syscall
''' + log_header))
        public = Path(directory) / 'public_test'
        subprocess.run([compiler, '-std=c++14', '-Wall', '-Wextra',
                        '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
                        '-Isrc/libpf', '-isystem', 'lg-opensource/public/boost',
                        '-isystem', 'lg-opensource/public/rapidjson/include',
                        str(options), 'src/libpf/audio_metadata.cpp', 'tools/test_libpf_public.cpp',
                        *flags, '-o', str(public)], cwd=ROOT, check=True)
        subprocess.run([str(public)], check=True)
        mrc = Path(directory) / 'mrc_test'
        subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra',
                        '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
                        '-Ilg-opensource/public/media-resource-calculator/calculator',
                        'src/libmedia-resource-calculator/resource.cpp', 'tools/test_mrc_public.cpp',
                        '-o', str(mrc)], cwd=ROOT, check=True)
        subprocess.run([str(mrc)], check=True)


if __name__ == '__main__':
    main()
