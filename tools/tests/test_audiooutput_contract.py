#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_audiooutput_contract.py [--sanitize]. No installs required."""
from pathlib import Path
import subprocess
import shutil
import re
import struct
import hashlib
import sys


def verify_shipped_contract(root):
    binary = (root / 'orig/usr/sbin/audiooutputd').read_bytes()
    assert hashlib.sha256(binary).hexdigest() == '026331391098d078a1941a95ec78fe8a23c05c5d944555c7bab431d8410d15f4'
    assert binary[:6] == b'\x7fELF\x01\x01'
    headers, = struct.unpack_from('<I', binary, 32)
    stride, count = struct.unpack_from('<HH', binary, 46)
    sections = [struct.unpack_from('<10I', binary, headers + i * stride) for i in range(count)]

    def at(address, size):
        for section in sections:
            _, kind, _, base, offset, length, *_ = section
            if kind != 8 and base <= address and address + size <= base + length:
                return binary[offset + address - base:offset + address - base + size]
        raise AssertionError(hex(address))

    def string(address):
        result = bytearray()
        while at(address, 1) != b'\0':
            result += at(address, 1)
            address += 1
        return result.decode()

    tables = {
        0x20b78: [('connect', 0xa4f1), ('disconnect', 0xa4f9), ('getStatus', 0xa519),
                  ('getHeadphoneStatus', 0xa521), ('mute', 0xa509), ('removePipelineInfo', 0xa529),
                  ('setInputVolume', 0xa511), ('setSoundOut', 0xa501)],
        0x20be4: [('setSettings', 0xb9bd), ('setQualityAttribute', 0xb9cd), ('getSettings', 0xb9c5)],
        0x20c14: [('up', 0xd5dd), ('down', 0xd5e5), ('set', 0xd5d5),
                  ('getStatus', 0xd5f5), ('muteSoundOut', 0xd5ed)],
    }
    for address, methods in tables.items():
        for index, (name, callback) in enumerate(methods):
            pointer, function, flags = struct.unpack('<III', at(address + index * 12, 12))
            assert (string(pointer), function, flags) == (name, callback, 0), (string(pointer), hex(function), name, hex(callback))
        assert at(address + len(methods) * 12, 12) == bytes(12)
    source = (root / 'src/audiooutputd/contract.cpp').read_text()
    schemas = re.findall(r'R"\((\{"type".*?\})\)"', source)
    assert len(schemas) == 14
    for schema in schemas:
        assert schema.encode() + b'\0' in binary, schema
    # Raw r0=500 followed by g_timeout_add, and unconditional category validity.
    assert at(0x9422, 4) == bytes.fromhex('4ff4fa70')
    assert at(0xa7f8, 4) == bytes.fromhex('01207047')
    assert b'com.webos.service.audiooutput\0' in binary
    print('shipped evidence: SHA256, 16 method/thunk/flag entries, 14 schemas, 500ms timer verified')

ROOT = Path(__file__).resolve().parents[2]
OUTPUT = ROOT / 'build/scratch/broad/audiooutput'
OUTPUT.mkdir(parents=True, exist_ok=True)
verify_shipped_contract(ROOT)
compiler = shutil.which('clang++') or shutil.which('c++')
sources = sorted((ROOT / 'src/audiooutputd').glob('*.cpp'))
sources = [p for p in sources if p.name != 'main.cpp']
command = [compiler, '-std=c++14', '-O2', '-Wall', '-Wextra', '-Werror',
           '-DAUDIOOUTPUT_HOST_TEST', '-Isrc/audiooutputd', '-Itools/tests',
           '-isystem', 'lg-opensource/public/rapidjson/include',
           *map(str, sources), 'tools/tests/test_audiooutput_contract.cpp',
           '-o', str(OUTPUT / 'test_audiooutput_contract')]
if '--sanitize' in sys.argv:
    command[1:1] = ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']
subprocess.run(command, cwd=ROOT, check=True)
subprocess.run([str(OUTPUT / 'test_audiooutput_contract')], cwd=ROOT, check=True)
