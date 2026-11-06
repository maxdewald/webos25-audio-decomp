#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_umi_policy_evidence.py; verify shipped Thumb predicates."""
import hashlib
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[2]
BINARY = ROOT / 'orig/usr/lib/libumi.so.1.0.0'


def main():
    assert hashlib.sha256(BINARY.read_bytes()).hexdigest() == (
        'f1ac7b61ac389af0329f36112e3b316c1f8575b1fecf79aed07986d1a873a764')
    objdump = shutil.which('llvm-objdump') or '/opt/homebrew/opt/llvm/bin/llvm-objdump'

    def code(start, end):
        result = subprocess.run(
            [objdump, '-d', '--no-show-raw-insn', '--triple=armv7', '--mattr=+thumb-mode',
             f'--start-address={start}', f'--stop-address={end}', str(BINARY)],
            check=True, capture_output=True, text=True)
        return [' '.join(line.split('@')[0].split()) for line in result.stdout.splitlines()]

    def require(lines, *fragments):
        for fragment in fragments:
            assert any(fragment in line for line in lines), fragment

    mixer = code(0x427ae, 0x427ca)
    require(mixer, '427ae: ldrb.w r3, [r6, #0x7c]', '427b2: cmp r3, r5',
            '427b4: beq 0x42750', '427ba: strb.w r5, [r6, #0x7c]',
            'getSoundOutputDigitalMode', 'setSoundOutputDigital')
    direct = code(0x40304, 0x40382)
    require(direct, '40306: ldrb.w r2, [r4, #0x7c]', '4030c: cbnz r2, 0x4037e',
            '4037e: movs r2, #0x1', '40310: blx', 'setOutputTypeE')
    invalid = code(0x3fd4c, 0x3fe50)
    require(invalid, '3fd54: cmp r0, #0x0', '3fe1e: movs r5, #0x0',
            '3fe20: movs r7, #0x1', '3fe24: ldr r1, [r4, #0x40]',
            '3fe2e: beq 0x3fd58', '3fe38: cmp r2, r9', '3fe3c: movlt r5, r2',
            '3fe3e: movlt r7, #0x1', '3fe44: movge r5, r9')
    cap = code(0x42c82, 0x42d28)
    require(cap, 'asNumber', '42cc4: cmp r7, #0x0', '42cc6: bne 0x42c2c',
            '42ccc: cmp r2, r3', '42cd0: movlt r2, r3',
            'convertDeviceChannelCapability', '42d24: str r0, [r5, #0x40]')
    fallback = code(0x40436, 0x4044a)
    require(fallback, 'checkReEncodePriority', '40440: bne.w 0x401d6',
            '40448: b 0x40306')
    iamf = code(0x4905c, 0x4906a)
    require(iamf, '[r0, #0x70]', 'cbnz r0, 0x49068', '[r3, #0x71]')
    valid = code(0x38858, 0x3886a)
    require(valid, 'cmp r1, #0x8', '#0x144', 'lsrls r0, r1', 'andls r0, r0, #0x1')
    ctor = code(0x26f14, 0x27158)
    require(ctor, '26f84: movs r3, #0x0', '26fd8: strd r3, r3, [r4, #124]')
    singleton = code(0x27188, 0x271d6)
    require(singleton, '__cxa_guard_acquire', '__aeabi_atexit', '__cxa_guard_release')
    deinit = code(0x1fc50, 0x1fc60)
    require(deinit, 'ldrd r3, r2, [r0, #204]', 'strne.w r3, [r0, #0xd0]')
    print('UMI evidence: original SHA256 and 10 Thumb predicate/lifetime groups verified')


if __name__ == '__main__':
    main()
