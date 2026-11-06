#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_output_routing.py; host topology + optional shipped-Image checks."""
import hashlib
import os
from pathlib import Path
import struct
import subprocess
import unittest

from test_kernel_seek import extract

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/scratch/broad/routing'
IMAGE = ROOT / 'orig/boot/Image-5.4.268-329.decompressed'
BASE = 0xffffffc010088000
DIGEST = '0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db'


class HostTopology(unittest.TestCase):
    def test_source_extracted_format_predicates(self):
        OUT.mkdir(parents=True, exist_ok=True)
        generated = OUT / 'format_under_test.inc'
        binary = OUT / 'test_routing_format'
        generated.write_text(extract('src/kernel/aud/alsa_sndout.c', [
            'sndout_codec_convert_output_codec', 'input_codec_convert_output_codec',
            'input_codec_convert_sndout_codec', '_use_output', '_org_output',
            '_get_output_format_set_type', '_is_decoder_bypass', '_get_output_format']))
        try:
            subprocess.run([os.environ.get('CC', 'cc'), '-std=gnu11', '-Wall', '-Wextra',
                '-Werror', '-Wno-unused-parameter', '-Wno-unused-function',
                '-Wno-unused-variable', '-Wno-sign-compare', '-fsanitize=address,undefined',
                '-fno-omit-frame-pointer', '-g', '-I', str(OUT),
                str(ROOT / 'tools/tests/test_output_routing_format.c'), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True,
                           env={**os.environ, 'UBSAN_OPTIONS': 'halt_on_error=1'})
        finally:
            generated.unlink(missing_ok=True)
            binary.unlink(missing_ok=True)

    def test_source_extracted_topology(self):
        OUT.mkdir(parents=True, exist_ok=True)
        generated = OUT / 'routing_under_test.inc'
        binary = OUT / 'test_routing'
        generated.write_text(extract('src/kernel/aud/alsa_sndout.c', [
            '_connect_pcm', '_set_output', '_connect_output', '_old_connect_output',
            '_old_arc_connect_output', '_connect_output_all', 'sndout_connect_output',
            'default_hp_connection', 'sndout_earc_onoff', 'sndout_earc_output_type']) + '\n' +
            extract('src/kernel/aud/notifier.c', ['notifier_output_changed',
            'notifier_input_changed', 'notifier_set_play_speed', 'notifier_get_play_speed']))
        try:
            subprocess.run([os.environ.get('CC', 'cc'), '-std=gnu11', '-Wall', '-Wextra',
                '-Werror', '-Wno-unused-parameter', '-Wno-unused-function',
                '-Wno-unused-variable', '-Wno-sign-compare', '-fsanitize=address,undefined',
                '-fno-omit-frame-pointer', '-g', '-I', str(OUT),
                str(ROOT / 'tools/tests/test_output_routing.c'), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True,
                           env={**os.environ, 'UBSAN_OPTIONS': 'halt_on_error=1'})
        finally:
            generated.unlink(missing_ok=True)
            binary.unlink(missing_ok=True)


@unittest.skipUnless(IMAGE.exists(), 'private shipped Image not present')
class ShippedInstructions(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import capstone
        from capstone import aarch64
        cls.data = IMAGE.read_bytes()
        assert hashlib.sha256(cls.data).hexdigest() == DIGEST
        cls.md = capstone.Cs(capstone.CS_ARCH_AARCH64, capstone.CS_MODE_LITTLE_ENDIAN)
        cls.md.detail = True
        cls.imm = aarch64.AARCH64_OP_IMM

    def instruction(self, address):
        return next(self.md.disasm(self.data[address-BASE:address-BASE+4], address))

    def immediate(self, address, mnemonic, operand, value):
        ins = self.instruction(address)
        self.assertEqual(ins.mnemonic, mnemonic, hex(address))
        self.assertEqual(ins.operands[operand].type, self.imm)
        self.assertEqual(ins.operands[operand].imm & ((1 << 64)-1), value, hex(address))

    def call(self, address, target):
        self.immediate(address, 'bl', 0, target)

    def test_hp_default_and_independent_ports(self):
        self.immediate(0xffffffc010cd3fd8, 'mov', 1, 0x10)
        self.call(0xffffffc010cd3fdc, 0xffffffc010cd3a00)
        self.call(0xffffffc010cd4034, 0xffffffc010cd2870)
        self.immediate(0xffffffc010cd6e00, 'cmp', 1, 0x10)
        self.immediate(0xffffffc010cd6e04, 'b.eq', 0, 0xffffffc010cd6ef0)
        self.immediate(0xffffffc010cd2910, 'mov', 1, 0)
        self.immediate(0xffffffc010cd29bc, 'mov', 1, 2)
        self.immediate(0xffffffc010cd482c, 'mov', 1, 0x100)
        self.immediate(0xffffffc010cd4648, 'mov', 1, 1)
        self.call(0xffffffc010cd4b70, 0xffffffc010cd2870)

    def test_speed_override_and_legacy_escape(self):
        self.call(0xffffffc010cd30c8, 0xffffffc010cc3430)
        self.immediate(0xffffffc010cd30cc, 'cmp', 1, 1000)
        for address in (0xffffffc010cd30d4, 0xffffffc010cd30d8):
            ins = self.instruction(address)
            self.assertEqual(self.md.reg_name(ins.operands[0].reg), 'wzr')
        self.immediate(0xffffffc010cd3184, 'cbnz', 1, 0xffffffc010cd30c4)
        self.call(0xffffffc010cd319c, 0xffffffc010cab0a0)
        self.immediate(0xffffffc010cd30bc, 'tst', 1, 0xffff0000)
        self.assertEqual(self.instruction(0xffffffc010cd3270).operands[1].mem.disp, 0x10)
        self.immediate(0xffffffc010cd3274, 'cbnz', 1, 0xffffffc010cd3284)
        self.assertEqual(self.instruction(0xffffffc010cd3280).mnemonic, 'strb')
        self.call(0xffffffc010cd4e8c, 0xffffffc010cd3010)
        self.call(0xffffffc010cd50c0, 0xffffffc010cd3010)
        self.immediate(0xffffffc010cd50a0, 'cbz', 1, 0xffffffc010cd5140)
        self.call(0xffffffc010cd5144, 0xffffffc010cd4f64)
        self.call(0xffffffc010cd4f84, 0xffffffc010cd2000)
        self.call(0xffffffc010cc3404, 0xffffffc010cdc580)

    def test_cache_omits_mix_option_and_rate(self):
        # Cache channel exception is entered only for eARC non-bypass PCM.
        self.immediate(0xffffffc010cd4bac, 'cmp', 1, 0x20)
        self.immediate(0xffffffc010cd4bcc, 'b.eq', 0, 0xffffffc010cd4db0)
        ins = self.instruction(0xffffffc010cd4db8)
        self.assertEqual((ins.mnemonic, ins.operands[1].mem.disp), ('ldr', 0x1c0))
        self.immediate(0xffffffc010cd4b70, 'bl', 0, 0xffffffc010cd2870)
        self.immediate(0xffffffc010cd625c, 'cmp', 1, 0x10)
        self.assertEqual(self.instruction(0xffffffc010cd6208).operands[1].mem.disp, 0x31)

    def test_decoded_info_change_predicates(self):
        self.immediate(0xffffffc010ca61f0, 'cmp', 1, 0x2c)
        self.immediate(0xffffffc010ca61f4, 'b.ne', 0, 0xffffffc010ca63cc)
        for address in (0xffffffc010ca6238, 0xffffffc010ca6248,
                        0xffffffc010ca6350, 0xffffffc010ca635c):
            self.immediate(address, 'b.ne', 0, 0xffffffc010ca6260)
        self.immediate(0xffffffc010ca6258, 'b.eq', 0, 0xffffffc010ca6344)
        self.call(0xffffffc010ca626c, 0xffffffc010ca5994)
        self.call(0xffffffc010ca62d8, 0xffffffc010cc3310)
        self.immediate(0xffffffc010ca63c8, 'b', 0, 0xffffffc010ca62e8)
        # Payload rate/channels/bits/acmod offsets 8/12/16/20.
        for address, offset in ((0xffffffc010ca6240, 8), (0xffffffc010ca6250, 12),
                                (0xffffffc010ca6348, 16), (0xffffffc010ca6354, 20)):
            self.assertEqual(self.instruction(address).operands[1].mem.disp, offset)

    def test_control_gates_and_notifications(self):
        self.immediate(0xffffffc010cd9330, 'b.eq', 0, 0xffffffc010cd9340)
        self.immediate(0xffffffc010cd9338, 'tbnz', 1, 5)
        self.immediate(0xffffffc010cd933c, 'tbnz', 1, 9)
        self.call(0xffffffc010cd93b0, 0xffffffc010cb3a30)
        self.call(0xffffffc010cd9348, 0xffffffc010cc32a0)
        self.immediate(0xffffffc010cd90a4, 'cmp', 1, 2)
        self.immediate(0xffffffc010cd90a8, 'ccmp', 1, 9)
        self.immediate(0xffffffc010cd9168, 'cbz', 1, 0xffffffc010cd90f8)
        self.call(0xffffffc010cd9100, 0xffffffc010cc32a0)
        self.call(0xffffffc010cc32b0, 0xffffffc010cad3f0)

    def test_output_selection_queue_and_defaults(self):
        self.immediate(0xffffffc010cb3bec, 'cmp', 1, 1)
        self.immediate(0xffffffc010cb3bf0, 'b.ne', 0, 0xffffffc010cb3c94)
        self.call(0xffffffc010cb3c9c, 0xffffffc010c96f50)
        self.immediate(0xffffffc010cb3cc4, 'cmp', 1, 2)
        self.immediate(0xffffffc010cb3cd4, 'cmp', 1, 3)
        self.call(0xffffffc010cb3d2c, 0xffffffc010c8d240)
        self.immediate(0xffffffc010cdcc08, 'mov', 1, 0x800000)
        self.immediate(0xffffffc010cdcc28, 'mov', 1, 0x800000)
        self.immediate(0xffffffc010cdcc2c, 'mov', 1, 1)
        self.assertEqual(self.instruction(0xffffffc010cdcc30).operands[2].mem.disp, 0xd8)
        self.assertEqual(self.instruction(0xffffffc010cdcc30).mnemonic, 'stp')

    def test_output_notification_does_not_reconnect(self):
        self.assertEqual(self.instruction(0xffffffc010cad3f0).mnemonic, 'ret')
        self.immediate(0xffffffc010cb2794, 'mov', 1, 0x10020000)
        self.immediate(0xffffffc010cb27e4, 'mov', 1, 0x10010000)
        self.call(0xffffffc010cb2e20, 0xffffffc010cb2694)
        for start, end in ((0xffffffc010cb2d54, 0xffffffc010cb2fa0),
                           (0xffffffc010cb3230, 0xffffffc010cb32e0)):
            targets = {ins.operands[0].imm & ((1 << 64)-1)
                       for ins in self.md.disasm(self.data[start-BASE:end-BASE], start)
                       if ins.mnemonic in ('b', 'bl')}
            self.assertNotIn(0xffffffc010cdc580, targets)
            self.assertNotIn(0xffffffc010cd51f0, targets)
            self.assertNotIn(0xffffffc010cd44e0, targets)
            self.assertNotIn(0xffffffc010c9fa40, targets)
        # Exact physical conversion table: ARC variants share OUTPUT_ARC=3.
        table = 0xffffffc0115f0658
        expected = [(1,0),(2,2),(4,2),(8,5),(16,1),(32,3),(64,5),
                    (128,5),(1024,5),(256,5),(512,3),(2048,3),(4096,5)]
        actual = [(struct.unpack_from('<I', self.data, table-BASE+i*40)[0],
                   struct.unpack_from('<I', self.data, table-BASE+i*40+16)[0])
                  for i in range(len(expected))]
        self.assertEqual(actual, expected)

    def test_exact_direct_reconnect_triggers(self):
        # This bounded aligned scan does not rule out indirect callers.
        expected = {
            0xffffffc010cc32a0: {0xffffffc010cd5a18, 0xffffffc010cd5cd0,
                0xffffffc010cd6670, 0xffffffc010cd8d24, 0xffffffc010cd9100,
                0xffffffc010cd9348, 0xffffffc010cd99dc, 0xffffffc010cd9f3c},
            0xffffffc010cdc580: {0xffffffc010cc3320, 0xffffffc010cc3404},
            0xffffffc010cc3310: {0xffffffc010ca62d8},
            0xffffffc010cc3380: {0xffffffc010cbe75c},
            0xffffffc010cd51f0: {0xffffffc010cd6bc8, 0xffffffc010cda2e0,
                               0xffffffc010cdc5a0},
        }
        symbols = ROOT / 'build/scratch/a4/kallsyms.txt'
        self.assertTrue(symbols.exists(), 'extract kallsyms before static verification')
        end = next(int(line.split()[0], 16) for line in symbols.read_text().splitlines()
                   if len(line.split()) == 3 and line.split()[2] == '_etext')
        found = {target:set() for target in expected}
        for offset in range(0, end-BASE, 4):
            word = struct.unpack_from('<I', self.data, offset)[0]
            if word & 0x7c000000 != 0x14000000:
                continue
            relative = word & 0x3ffffff
            if relative & 0x2000000:
                relative -= 0x4000000
            address = BASE+offset
            target = address+relative*4
            if target in found:
                found[target].add(address)
        self.assertEqual(found, expected)


if __name__ == '__main__':
    unittest.main(verbosity=2)
