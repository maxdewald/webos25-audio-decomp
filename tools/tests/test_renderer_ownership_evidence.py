#!/usr/bin/env python3
"""Offline shipped ARM64 assertions: python3 tools/tests/test_renderer_ownership_evidence.py.

Requires the supplied raw Image and Capstone; does not execute kernel code.
Addresses are raw VAs at base 0xffffffc010088000, never ELF/Thumb addresses.
"""
import hashlib
from pathlib import Path
import struct
import unittest
import capstone

ROOT = Path(__file__).resolve().parents[2]
BASE = 0xffffffc010088000
IMAGE = ROOT / 'orig/boot/Image-5.4.268-329.decompressed'
SHA256 = '0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db'


class ShippedOwnership(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not IMAGE.exists():
            raise unittest.SkipTest('operator-supplied shipped Image not present')
        cls.data = IMAGE.read_bytes()
        if hashlib.sha256(cls.data).hexdigest() != SHA256:
            raise AssertionError('wrong shipped Image digest')
        cls.md = capstone.Cs(capstone.CS_ARCH_AARCH64, capstone.CS_MODE_LITTLE_ENDIAN)

    def ins(self, address, mnemonic, operands):
        decoded = list(self.md.disasm(self.data[address-BASE:address-BASE+4], address))
        self.assertEqual(len(decoded), 1)
        self.assertEqual((decoded[0].mnemonic, decoded[0].op_str), (mnemonic, operands), hex(address))

    def span(self, start, end):
        result = list(self.md.disasm(self.data[start-BASE:end-BASE], start))
        self.assertEqual(len(result)*4, end-start)
        return result

    def text(self, address):
        return self.data[address-BASE:address-BASE+256].split(b'\0', 1)[0].decode('ascii')

    def test_whole_bodies_bind_negative_lifecycle_claims(self):
        ranges = [
            (0xffffffc010cb7604, 0xffffffc010cb76e0, '0e90ae59eea38c477c51917546f3ffc8844f52721cf9c205ec57e80202ff2147'),
            (0xffffffc010cb7400, 0xffffffc010cb74a0, '2121c7ff51e3fec1d0d08551d3a739fef0034277c40ad2669d355ac8931a7450'),
            (0xffffffc010cb8494, 0xffffffc010cb8cf0, '7c800d1a9f58f23f532ff0c8f934ab4965406be339032d869bb9a4cab2a27e05'),
            (0xffffffc010cbd5c4, 0xffffffc010cbdda0, '554d009ad02953a784ceda33454e2d28ac7718bdfeb58ad07586ece52786e84f'),
            (0xffffffc010ca9894, 0xffffffc010ca99b0, 'e186b7426bc149d6d314451c1fcad188dce2b8bbac4c56a7d22abf7891cc9307'),
            (0xffffffc010ca95d0, 0xffffffc010ca9894, '0f634796824707c20bdeae7aba0f51618361f07bb640e044af52a27b822e3400'),
            (0xffffffc010c99a40, 0xffffffc010c99d40, '15e3893666b8918e48bba04c2e973f079336850c2a97552c050792a049b0c196'),
            (0xffffffc010c9a550, 0xffffffc010c9b0a0, 'dc8242ea025528978774b8d843a751a3c31013d18cb7bcb3a12fed03bca22ad4')]
        for start, end, digest in ranges:
            self.assertEqual(hashlib.sha256(self.data[start-BASE:end-BASE]).hexdigest(), digest)
            self.span(start, end)

    def test_fd_is_zero_initialized_not_acquired(self):
        self.ins(0xffffffc010cb7654, 'mov', 'x2, #0x18')
        self.ins(0xffffffc010cb7658, 'mov', 'w1, #0xdc0')
        self.ins(0xffffffc010cb7660, 'bl', '0xffffffc010269934')
        self.ins(0xffffffc010cb7668, 'str', 'x2, [x20, #0xc8]')
        calls = [i.op_str for i in self.span(0xffffffc010cb7604, 0xffffffc010cb76e0) if i.mnemonic == 'bl']
        self.assertNotIn('0xffffffc010cbd5c4', calls)

    def test_set_index_stores_even_failure(self):
        self.ins(0xffffffc010cb8b10, 'ldr', 'w0, [sp, #0x90]')
        self.ins(0xffffffc010cb8b14, 'tbnz', 'w0, #0x1f, 0xffffffc010cb8b70')
        self.ins(0xffffffc010cb8b18, 'bl', '0xffffffc010cbd5c4')
        self.ins(0xffffffc010cb8b1c, 'ldr', 'w1, [sp, #0x90]')
        self.ins(0xffffffc010cb8b20, 'sxtw', 'x20, w0')
        self.ins(0xffffffc010cb8b24, 'str', 'w1, [x23, #0x10]')

    def test_range_slot_lock_and_boolean_not_refcount(self):
        for address in [0xffffffc010cbd5d4, 0xffffffc010cbd7a4, 0xffffffc010cbdac4]:
            self.ins(address, 'cmp', 'w0, #9')
            self.assertEqual(self.span(address+4, address+8)[0].mnemonic, 'b.hi')
        self.ins(0xffffffc010cbd5e4, 'mov', 'w21, #0x58')
        self.ins(0xffffffc010cbd5fc, 'add', 'x21, x21, #8')
        self.ins(0xffffffc010cbd604, 'bl', '0xffffffc0110851e0')
        self.ins(0xffffffc010cbd620, 'ldrb', 'w0, [x25, #0x28]')
        self.ins(0xffffffc010cbd624, 'cbnz', 'w0, 0xffffffc010cbd704')
        self.ins(0xffffffc010cbd648, 'strb', 'w2, [x25, #0x28]')
        self.ins(0xffffffc010cbd650, 'str', 'w1, [x25, #0x54]')

    def test_release_and_destructor_chain(self):
        self.ins(0xffffffc010cb744c, 'ldr', 'w0, [x20, #0x10]')
        self.ins(0xffffffc010cb7454, 'bl', '0xffffffc010cbd790')
        self.ins(0xffffffc010cb745c, 'bl', '0xffffffc0110851e0')
        self.ins(0xffffffc010cbd808, 'bl', '0xffffffc010c9edc0')
        self.ins(0xffffffc010cbd810, 'bl', '0xffffffc010c9f4b0')
        self.ins(0xffffffc010cbd814, 'strb', 'wzr, [x26, #0x28]')
        self.ins(0xffffffc010c9f244, 'ldr', 'x1, [x0, #0x28]')
        self.ins(0xffffffc010c9f250, 'blr', 'x1')
        self.ins(0xffffffc010c9f268, 'bl', '0xffffffc01026afe0')
        self.ins(0xffffffc010cac3e4, 'bl', '0xffffffc010c96490')
        self.ins(0xffffffc010cac3f0, 'bl', '0xffffffc010c96490')

    def test_bypass_query_uses_flags_and_constructs(self):
        self.ins(0xffffffc010cb8a24, 'mov', 'w2, #1')
        self.ins(0xffffffc010cbdb24, 'ldr', 'w2, [x1, #0x48]')
        self.ins(0xffffffc010cbdb2c, 'ldr', 'w1, [x1, #0x4c]')
        self.ins(0xffffffc010cbdb34, 'bl', '0xffffffc010cad1d0')
        self.ins(0xffffffc010cbdbdc, 'bl', '0xffffffc010cad340')
        self.ins(0xffffffc010ca98fc, 'str', 'w0, [x19, #0x34]')
        self.assertEqual(self.text(0xffffffc0118e2978), 'srcbin')
        self.ins(0xffffffc010ca990c, 'bl', '0xffffffc010ca87c0')
        self.ins(0xffffffc010ca9940, 'bl', '0xffffffc010ca87c0')
        registrations = [i for i in self.span(0xffffffc010ca9894, 0xffffffc010ca99b0)
                         if i.mnemonic == 'bl' and i.op_str == '0xffffffc010ca87c0']
        self.assertEqual(len(registrations), 2)  # rts and lipsync, never src

    def test_bypass_default_is_channels_not_gain(self):
        self.ins(0xffffffc010ca9748, 'ldr', 'w1, [x19, #0x34]')
        self.ins(0xffffffc010ca974c, 'mov', 'w3, w20')
        self.assertEqual(self.text(0xffffffc0118e2940), 'out-channels')
        self.ins(0xffffffc010ca975c, 'bl', '0xffffffc010ca9390')
        self.ins(0xffffffc010ca96b0, 'ldr', 'w1, [x19, #0x44]')
        self.ins(0xffffffc010ca96bc, 'mov', 'w3, #0x800000')
        self.assertEqual(self.text(0xffffffc0118e32f8), 'gain')
        self.ins(0xffffffc010cacb38, 'str', 'w3, [x19, #0x30]')
        self.ins(0xffffffc010cacb4c, 'add', 'x2, x20, #0x738')
        self.ins(0xffffffc010cac248, 'stp', 'w21, w3, [x0, #0x20]')

    def test_shipped_property_tables_match_source(self):
        text = (ROOT / 'src/kernel/aud/renderer.c').read_text()
        for table, address in [('param_src', 0xffffffc0122cc638), ('param_gain', 0xffffffc0122cc3d8)]:
            import re
            body = text.split('struct renderer_param '+table+'[] = {', 1)[1].split('\n};', 1)[0]
            expected = re.findall(r'\{"([^"]+)"(?:,\s*"([^"]+)")?', body)
            actual = []
            for index in range(32):
                name, alias, setter, getter = struct.unpack_from('<4Q', self.data, address-BASE+32*index)
                if name == 0: break
                actual.append((self.text(name), self.text(alias) if alias else ''))
                self.assertEqual((setter, getter), (0, 0))
            self.assertEqual(actual, expected)

    def test_property_command_and_debug_are_generic(self):
        self.ins(0xffffffc010c8ed18, 'mov', 'w1, #4')
        self.ins(0xffffffc010c8ed1c, 'movk', 'w1, #0x1018, lsl #16')
        self.ins(0xffffffc010c8ed30, 'bl', '0xffffffc010c8e310')
        self.ins(0xffffffc010c91e98, 'movk', 'w1, #0x1018, lsl #16')
        self.ins(0xffffffc010c92780, 'ldr', 'w1, [sp, #0x8c]')
        self.ins(0xffffffc010c92790, 'bl', '0xffffffc010d09140')
        # Bounded MOVK search, not a proof against arbitrary IPC/data/computed constants.
        self.assertFalse([i for i in self.span(0xffffffc010c80000, 0xffffffc010d20000)
                          if i.mnemonic == 'movk' and '#0x5004,' in i.op_str])

    def test_uimc_identity_is_explicit_advisory_pid(self):
        self.ins(0xffffffc010c99b68, 'mov', 'x2, #0xc8')
        self.ins(0xffffffc010c99bcc, 'mov', 'x0, x19')
        self.ins(0xffffffc010c99bd0, 'bl', '0xffffffc010c92c20')
        self.ins(0xffffffc010c99c04, 'str', 'w1, [x19, #0xc0]')
        self.ins(0xffffffc010c9a75c, 'str', 'w0, [x23, #0xc0]')
        self.ins(0xffffffc010c9b23c, 'ldr', 'w0, [x19, #0xc0]')
        self.ins(0xffffffc010c9b27c, 'bl', '0xffffffc0100dcd20')
        self.ins(0xffffffc010c9b284, 'bl', '0xffffffc0100dcc20')
        self.ins(0xffffffc010c9b2cc, 'mov', 'w0, #0x404')
        self.ins(0xffffffc010c90aa0, 'str', 'x22, [x19, #0x48]')
        self.ins(0xffffffc010c92dbc, 'mov', 'x2, #0x70')
        self.ins(0xffffffc010c8c898, 'mov', 'x2, #0x20')
        self.ins(0xffffffc010c8c870, 'stp', 'x21, x22, [x0, #0x10]')
        self.ins(0xffffffc010c9a574, 'ldr', 'x1, [x3, #0x458]')
        self.ins(0xffffffc010c9a648, 'ldr', 'x2, [x1, #0x458]')
        self.ins(0xffffffc010c9a048, 'b.hi', '0xffffffc010c9a010')
        self.ins(0xffffffc010c9a208, 'add', 'w19, w19, #0x20')
        self.ins(0xffffffc010c9088c, 'ldrb', 'w0, [x24, #0x38]')
        self.ins(0xffffffc010c90898, 'bl', '0xffffffc010c8f764')
        self.assertEqual(self.data[0xffffffc0122caf78-BASE], 1)
        self.ins(0xffffffc010c93d04, 'str', 'xzr, [x19, #0x68]')
        self.ins(0xffffffc010c93d20, 'bl', '0xffffffc010c939a0')
        self.ins(0xffffffc010c939b8, 'add', 'x1, x0, #0x40')
        self.ins(0xffffffc010c939c4, 'ldaddal', 'w0, w2, [x1]')
        self.ins(0xffffffc010c939cc, 'cbnz', 'w0, 0xffffffc010c93bf4')

    def test_sink_maps_mixer_port_plus_four(self):
        image = ROOT / 'orig/usr/lib/gstreamer-1.0/libgstlgaudiosink.so'
        if not image.exists():
            self.skipTest('operator-supplied sink ELF not present')
        data = image.read_bytes()
        self.assertEqual(hashlib.sha256(data).hexdigest(),
                         '90dba3ef32704ce4096bf1e6045d2879d0c2192d507bc04d1e8e98437f420a43')
        phoff = struct.unpack_from('<I', data, 28)[0]
        entsize, count = struct.unpack_from('<HH', data, 42)
        for index in range(count):
            kind, offset, va, _, size, *_ = struct.unpack_from('<8I', data, phoff + index*entsize)
            if kind == 1 and va <= 0x14924 < va + size:
                raw = data[offset+0x14924-va:offset+0x1495a-va]
                break
        else:
            self.fail('sink function outside ELF load segments')
        thumb = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
        instructions = {i.address: (i.mnemonic, i.op_str) for i in thumb.disasm(raw, 0x14924)}
        expected = {0x14924: ('ldrb', 'r2, [r0, #4]'), 0x1492e: ('ldr', 'r3, [r0, #8]'),
                    0x14932: ('cbz', 'r2, 0x14936'), 0x14934: ('adds', 'r3, #4'),
                    0x1493e: ('str', 'r3, [sp, #8]'), 0x1494c: ('movw', 'r1, #0x5201'),
                    0x14950: ('add', 'r2, sp, #8'), 0x14952: ('movt', 'r1, #0x4004'),
                    0x14956: ('blx', '0x3c60')}
        for address, ins in expected.items():
            self.assertEqual(instructions[address], ins, hex(address))

    def test_output_notification_is_noop(self):
        self.ins(0xffffffc010cad3f0, 'ret', '')


if __name__ == '__main__':
    unittest.main(verbosity=2)
