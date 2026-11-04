
/Users/max/projects/dts_restore_plus/re/webos25-audio-policy/bin/usr/sbin/audiooutputd:	file format elf32-littlearm

Disassembly of section .init:

00003cb4 <.init>:
    3cb4: 4008         	ands	r0, r1
    3cb6: e92d 04d9    	push.w	{r0, r3, r4, r6, r7, r10}
    3cba: eb00 8008    	add.w	r0, r0, r8
    3cbe: e8bd         	<unknown>

Disassembly of section .plt:

00003cc0 <.plt>:
    3cc0: e004         	b	0x3ccc <.plt+0xc>       @ imm = #0x8
    3cc2: e52d         	b	0x3720                  @ imm = #-0x5a6
    3cc4: e004         	b	0x3cd0 <.plt+0x10>      @ imm = #0x8
    3cc6: e59f         	b	0x3808                  @ imm = #-0x4c2
    3cc8: e00e         	b	0x3ce8 <_ZN9umiClient15setOutputVolumeENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEh@plt+0x8> @ imm = #0x1c
    3cca: e08f         	b	0x3dec <_ZNK7pbnjson6JValue6asBoolERb@plt+0x4> @ imm = #0x11e
    3ccc: f008 e5be    	blx	0x40c84c <_end+0x3eb3b0> @ imm = #0x408b7c
    3cd0: d0f0         	beq	0x3cb4 <.init>          @ imm = #-0x20
    3cd2: 0001         	movs	r1, r0

00003cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt>:
    3cd4: c600         	<unknown>
    3cd6: e28f         	b	0x41f8 <exit@plt>       @ imm = #0x51e
    3cd8: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3cda: e28c         	b	0x41f6 <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt+0xa> @ imm = #0x518
    3cdc: f0f0 e5bc    	blx	0x4f4858 <_end+0x4d33bc> @ imm = #0x4f0b78

00003ce0 <_ZN9umiClient15setOutputVolumeENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEh@plt>:
    3ce0: c600         	<unknown>
    3ce2: e28f         	b	0x4204 <_ZSt9terminatev@plt> @ imm = #0x51e
    3ce4: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3ce6: e28c         	b	0x4202 <exit@plt+0xa>   @ imm = #0x518
    3ce8: f0e8 e5bc    	blx	0x4ec864 <_end+0x4cb3c8> @ imm = #0x4e8b78

00003cec <g_main_loop_unref@plt>:
    3cec: c600         	<unknown>
    3cee: e28f         	b	0x4210 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt> @ imm = #0x51e
    3cf0: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3cf2: e28c         	b	0x420e <_ZSt9terminatev@plt+0xa> @ imm = #0x518
    3cf4: f0e0 e5bc    	blx	0x4e4870 <_end+0x4c33d4> @ imm = #0x4e0b78

00003cf8 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13_S_copy_charsEPcPKcS7_@plt>:
    3cf8: c600         	<unknown>
    3cfa: e28f         	b	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #0x51e
    3cfc: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3cfe: e28c         	b	0x421a <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt+0xa> @ imm = #0x518
    3d00: f0d8 e5bc    	blx	0x4dc87c <_end+0x4bb3e0> @ imm = #0x4d8b78

00003d04 <_ZSt19__throw_logic_errorPKc@plt>:
    3d04: c600         	<unknown>
    3d06: e28f         	b	0x4228 <LSMessageRespond@plt> @ imm = #0x51e
    3d08: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d0a: e28c         	b	0x4226 <_ZN7pbnjson6JValueC1Ev@plt+0xa> @ imm = #0x518
    3d0c: f0d0 e5bc    	blx	0x4d4888 <_end+0x4b33ec> @ imm = #0x4d0b78

00003d10 <__cxa_begin_catch@plt>:
    3d10: c600         	<unknown>
    3d12: e28f         	b	0x4234 <_ZNK7pbnjson6JValueeqEb@plt> @ imm = #0x51e
    3d14: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d16: e28c         	b	0x4232 <LSMessageRespond@plt+0xa> @ imm = #0x518
    3d18: f0c8 e5bc    	blx	0x4cc894 <_end+0x4ab3f8> @ imm = #0x4c8b78

00003d1c <g_option_context_new@plt>:
    3d1c: c600         	<unknown>
    3d1e: e28f         	b	0x4240 <LSRegister@plt> @ imm = #0x51e
    3d20: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d22: e28c         	b	0x423e <_ZNK7pbnjson6JValueeqEb@plt+0xa> @ imm = #0x518
    3d24: f0c0 e5bc    	blx	0x4c48a0 <_end+0x4a3404> @ imm = #0x4c0b78

00003d28 <_ZNSt9exceptionD2Ev@plt>:
    3d28: c600         	<unknown>
    3d2a: e28f         	b	0x424c <_ZN9umiClient18removePipelineInfoENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #0x51e
    3d2c: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d2e: e28c         	b	0x424a <LSRegister@plt+0xa> @ imm = #0x518
    3d30: f0b8 e5bc    	blx	0x4bc8ac <_end+0x49b410> @ imm = #0x4b8b78

00003d34 <g_io_channel_set_close_on_unref@plt>:
    3d34: c600         	<unknown>
    3d36: e28f         	b	0x4258 <read@plt>       @ imm = #0x51e
    3d38: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d3a: e28c         	b	0x4256 <_ZN9umiClient18removePipelineInfoENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt+0xa> @ imm = #0x518
    3d3c: f0b0 e5bc    	blx	0x4b48b8 <_end+0x49341c> @ imm = #0x4b0b78

00003d40 <sigprocmask@plt>:
    3d40: c600         	<unknown>
    3d42: e28f         	b	0x4264 <LSUnregister@plt> @ imm = #0x51e
    3d44: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d46: e28c         	b	0x4262 <read@plt+0xa>   @ imm = #0x518
    3d48: f0a8 e5bc    	blx	0x4ac8c4 <_end+0x48b428> @ imm = #0x4a8b78

00003d4c <_ZN7pbnjson10JDomParser5parseERKNS_6JInputERKNS_7JSchemaE@plt>:
    3d4c: c600         	<unknown>
    3d4e: e28f         	b	0x4270 <_ZN9umiClient11getInstanceEv@plt> @ imm = #0x51e
    3d50: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d52: e28c         	b	0x426e <LSUnregister@plt+0xa> @ imm = #0x518
    3d54: f0a0 e5bc    	blx	0x4a48d0 <_end+0x483434> @ imm = #0x4a0b78

00003d58 <_ZN7pbnjson6JValueD1Ev@plt>:
    3d58: c600         	<unknown>
    3d5a: e28f         	b	0x427c <_ZN9umiClient19setQualityAttributeEN7pbnjson6JValueE@plt> @ imm = #0x51e
    3d5c: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d5e: e28c         	b	0x427a <_ZN9umiClient11getInstanceEv@plt+0xa> @ imm = #0x518
    3d60: f098 e5bc    	blx	0x49c8dc <_end+0x47b440> @ imm = #0x498b78

00003d64 <_ZN9umiClient16getDefaultVolumeEv@plt>:
    3d64: c600         	<unknown>
    3d66: e28f         	b	0x4288 <__libc_start_main@plt> @ imm = #0x51e
    3d68: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d6a: e28c         	b	0x4286 <_ZN9umiClient19setQualityAttributeEN7pbnjson6JValueE@plt+0xa> @ imm = #0x518
    3d6c: f090 e5bc    	blx	0x4948e8 <_end+0x47344c> @ imm = #0x490b78

00003d70 <strstr@plt>:
    3d70: c600         	<unknown>
    3d72: e28f         	b	0x4294 <_ZN9umiClient11getSettingsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt> @ imm = #0x51e
    3d74: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d76: e28c         	b	0x4292 <__libc_start_main@plt+0xa> @ imm = #0x518
    3d78: f088 e5bc    	blx	0x48c8f4 <_end+0x46b458> @ imm = #0x488b78

00003d7c <_ZN7pbnjson6JValue6appendERKS0_@plt>:
    3d7c: c600         	<unknown>
    3d7e: e28f         	b	0x42a0 <_ZN9umiClient11getSettingsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt+0xc> @ imm = #0x51e
    3d80: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d82: e28c         	b	0x429e <_ZN9umiClient11getSettingsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt+0xa> @ imm = #0x518
    3d84: f080 e5bc    	blx	0x484900 <_end+0x463464> @ imm = #0x480b78

00003d88 <sigaddset@plt>:
    3d88: c600         	<unknown>
    3d8a: e28f         	b	0x42ac                  @ imm = #0x51e
    3d8c: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d8e: e28c         	b	0x42aa <_ZN9umiClient11getSettingsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt+0x16> @ imm = #0x518
    3d90: f078 e5bc    	blx	0x47c90c <_end+0x45b470> @ imm = #0x478b78

00003d94 <_ZN7pbnjson10JDomParserC1Ev@plt>:
    3d94: c600         	<unknown>
    3d96: e28f         	b	0x42b8                  @ imm = #0x51e
    3d98: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3d9a: e28c         	b	0x42b6                  @ imm = #0x518
    3d9c: f070 e5bc    	blx	0x474918 <_end+0x45347c> @ imm = #0x470b78

00003da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt>:
    3da0: c600         	<unknown>
    3da2: e28f         	b	0x42c4                  @ imm = #0x51e
    3da4: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3da6: e28c         	b	0x42c2                  @ imm = #0x518
    3da8: f068 e5bc    	blx	0x46c924 <_end+0x44b488> @ imm = #0x468b78

00003dac <_ZN9umiClient14setSoundOutputENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt>:
    3dac: c600         	<unknown>
    3dae: e28f         	b	0x42d0                  @ imm = #0x51e
    3db0: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3db2: e28c         	b	0x42ce                  @ imm = #0x518
    3db4: f060 e5bc    	blx	0x464930 <_end+0x443494> @ imm = #0x460b78

00003db8 <_ZN7pbnjson6ObjectEv@plt>:
    3db8: c600         	<unknown>
    3dba: e28f         	b	0x42dc                  @ imm = #0x51e
    3dbc: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3dbe: e28c         	b	0x42da                  @ imm = #0x518
    3dc0: f058 e5bc    	blx	0x45c93c <_end+0x43b4a0> @ imm = #0x458b78

00003dc4 <_ZN7pbnjson10JDomParser8getErrorEv@plt>:
    3dc4: c600         	<unknown>
    3dc6: e28f         	b	0x42e8                  @ imm = #0x51e
    3dc8: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3dca: e28c         	b	0x42e6                  @ imm = #0x518
    3dcc: f050 e5bc    	blx	0x454948 <_end+0x4334ac> @ imm = #0x450b78

00003dd0 <g_main_loop_new@plt>:
    3dd0: c600         	<unknown>
    3dd2: e28f         	b	0x42f4                  @ imm = #0x51e
    3dd4: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3dd6: e28c         	b	0x42f2                  @ imm = #0x518
    3dd8: f048 e5bc    	blx	0x44c954 <_end+0x42b4b8> @ imm = #0x448b78

00003ddc <g_option_context_free@plt>:
    3ddc: c600         	<unknown>
    3dde: e28f         	b	0x4300                  @ imm = #0x51e
    3de0: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3de2: e28c         	b	0x42fe                  @ imm = #0x518
    3de4: f040 e5bc    	blx	0x444960 <_end+0x4234c4> @ imm = #0x440b78

00003de8 <_ZNK7pbnjson6JValue6asBoolERb@plt>:
    3de8: c600         	<unknown>
    3dea: e28f         	b	0x430c                  @ imm = #0x51e
    3dec: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3dee: e28c         	b	0x430a                  @ imm = #0x518
    3df0: f038 e5bc    	blx	0x43c96c <_end+0x41b4d0> @ imm = #0x438b78

00003df4 <_ZN7pbnjson6JValueC1Eb@plt>:
    3df4: c600         	<unknown>
    3df6: e28f         	b	0x4318                  @ imm = #0x51e
    3df8: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3dfa: e28c         	b	0x4316                  @ imm = #0x518
    3dfc: f030 e5bc    	blx	0x434978 <_end+0x4134dc> @ imm = #0x430b78

00003e00 <_ZN9umiClient18getHeadphoneStatusEPbRNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_@plt>:
    3e00: c600         	<unknown>
    3e02: e28f         	b	0x4324                  @ imm = #0x51e
    3e04: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3e06: e28c         	b	0x4322                  @ imm = #0x518
    3e08: f028 e5bc    	blx	0x42c984 <_end+0x40b4e8> @ imm = #0x428b78

00003e0c <g_option_context_parse@plt>:
    3e0c: c600         	<unknown>
    3e0e: e28f         	b	0x4330                  @ imm = #0x51e
    3e10: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3e12: e28c         	b	0x432e                  @ imm = #0x518
    3e14: f020 e5bc    	blx	0x424990 <_end+0x4034f4> @ imm = #0x420b78

00003e18 <LSCallCancelNotificationAdd@plt>:
    3e18: c600         	<unknown>
    3e1a: e28f         	b	0x433c                  @ imm = #0x51e
    3e1c: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3e1e: e28c         	b	0x433a                  @ imm = #0x518
    3e20: f018 e5bc    	blx	0x41c99c <_end+0x3fb500> @ imm = #0x418b78

00003e24 <_ZN7pbnjson15JSchemaFragmentD1Ev@plt>:
    3e24: c600         	<unknown>
    3e26: e28f         	b	0x4348                  @ imm = #0x51e
    3e28: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3e2a: e28c         	b	0x4346                  @ imm = #0x518
    3e2c: f010 e5bc    	blx	0x4149a8 <_end+0x3f350c> @ imm = #0x410b78

00003e30 <_ZN7pbnjson7JSchema4swapERS0_@plt>:
    3e30: c600         	<unknown>
    3e32: e28f         	b	0x4354                  @ imm = #0x51e
    3e34: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3e36: e28c         	b	0x4352                  @ imm = #0x518
    3e38: f008 e5bc    	blx	0x40c9b4 <_end+0x3eb518> @ imm = #0x408b78

00003e3c <_ZN9umiClient14setInputVolumeENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_h@plt>:
    3e3c: c600         	<unknown>
    3e3e: e28f         	b	0x4360                  @ imm = #0x51e
    3e40: ca1d         	ldm	r2, {r0, r2, r3, r4}
    3e42: e28c         	b	0x435e                  @ imm = #0x518
    3e44: f000 e5bc    	blx	0x4049c0 <_end+0x3e3524> @ imm = #0x400b78

00003e48 <_ZN2LS17SubscriptionPoint9cleanItemEPNS0_16SubscriptionItemE@plt>:
    3e48: c600         	<unknown>
    3e4a: e28f         	b	0x436c                  @ imm = #0x51e
    3e4c: ca1c         	ldm	r2, {r2, r3, r4}
    3e4e: e28c         	b	0x436a                  @ imm = #0x518
    3e50: fff8 e5bc    	vsli.64	d30, d28, #0x38

00003e54 <g_source_remove@plt>:
    3e54: c600         	<unknown>
    3e56: e28f         	b	0x4378                  @ imm = #0x51e
    3e58: ca1c         	ldm	r2, {r2, r3, r4}
    3e5a: e28c         	b	0x4376                  @ imm = #0x518
    3e5c: fff0 e5bc    	vsli.64	d30, d28, #0x30

00003e60 <_ZSt16__throw_bad_castv@plt>:
    3e60: c600         	<unknown>
    3e62: e28f         	b	0x4384                  @ imm = #0x51e
    3e64: ca1c         	ldm	r2, {r2, r3, r4}
    3e66: e28c         	b	0x4382                  @ imm = #0x518
    3e68: ffe8 e5bc    	vsli.64	d30, d28, #0x28

00003e6c <_ZN9umiClient7setMuteENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_b@plt>:
    3e6c: c600         	<unknown>
    3e6e: e28f         	b	0x4390                  @ imm = #0x51e
    3e70: ca1c         	ldm	r2, {r2, r3, r4}
    3e72: e28c         	b	0x438e                  @ imm = #0x518
    3e74: ffe0 e5bc    	vsli.64	d30, d28, #0x20

00003e78 <_ZN9umiClient12connectInputENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_N7pbnjson6JValueE@plt>:
    3e78: c600         	<unknown>
    3e7a: e28f         	b	0x439c                  @ imm = #0x51e
    3e7c: ca1c         	ldm	r2, {r2, r3, r4}
    3e7e: e28c         	b	0x439a                  @ imm = #0x518
    3e80: ffd8 e5bc    	vsli.64	d30, d28, #0x18

00003e84 <__cxa_allocate_exception@plt>:
    3e84: c600         	<unknown>
    3e86: e28f         	b	0x43a8                  @ imm = #0x51e
    3e88: ca1c         	ldm	r2, {r2, r3, r4}
    3e8a: e28c         	b	0x43a6                  @ imm = #0x518
    3e8c: ffd0 e5bc    	vsli.64	d30, d28, #0x10

00003e90 <g_option_context_add_main_entries@plt>:
    3e90: c600         	<unknown>
    3e92: e28f         	b	0x43b4                  @ imm = #0x51e
    3e94: ca1c         	ldm	r2, {r2, r3, r4}
    3e96: e28c         	b	0x43b2                  @ imm = #0x518
    3e98: ffc8 e5bc    	vsli.64	d30, d28, #0x8

00003e9c <__gmon_start__@plt>:
    3e9c: c600         	<unknown>
    3e9e: e28f         	b	0x43c0                  @ imm = #0x51e
    3ea0: ca1c         	ldm	r2, {r2, r3, r4}
    3ea2: e28c         	b	0x43be                  @ imm = #0x518
    3ea4: ffc0 e5bc    	vsli.64	d30, d28, #0x0

00003ea8 <g_error_free@plt>:
    3ea8: c600         	<unknown>
    3eaa: e28f         	b	0x43cc                  @ imm = #0x51e
    3eac: ca1c         	ldm	r2, {r2, r3, r4}
    3eae: e28c         	b	0x43ca                  @ imm = #0x518
    3eb0: ffb8 e5bc    	vsli.64	d14, d28, #0x38

00003eb4 <LSSetDisconnectHandler@plt>:
    3eb4: c600         	<unknown>
    3eb6: e28f         	b	0x43d8                  @ imm = #0x51e
    3eb8: ca1c         	ldm	r2, {r2, r3, r4}
    3eba: e28c         	b	0x43d6                  @ imm = #0x518
    3ebc: ffb0 e5bc    	vsli.64	d14, d28, #0x30

00003ec0 <_ZN9umiClient12deinitializeEv@plt>:
    3ec0: c600         	<unknown>
    3ec2: e28f         	b	0x43e4                  @ imm = #0x51e
    3ec4: ca1c         	ldm	r2, {r2, r3, r4}
    3ec6: e28c         	b	0x43e2                  @ imm = #0x518
    3ec8: ffa8 e5bc    	vsli.64	d14, d28, #0x28

00003ecc <_ZN7pbnjson6JValueC1ERKS0_@plt>:
    3ecc: c600         	<unknown>
    3ece: e28f         	b	0x43f0                  @ imm = #0x51e
    3ed0: ca1c         	ldm	r2, {r2, r3, r4}
    3ed2: e28c         	b	0x43ee                  @ imm = #0x518
    3ed4: ffa0 e5bc    	vsli.64	d14, d28, #0x20

00003ed8 <sigemptyset@plt>:
    3ed8: c600         	<unknown>
    3eda: e28f         	b	0x43fc                  @ imm = #0x51e
    3edc: ca1c         	ldm	r2, {r2, r3, r4}
    3ede: e28c         	b	0x43fa                  @ imm = #0x518
    3ee0: ff98 e5bc    	vsli.64	d14, d28, #0x18

00003ee4 <_ZNKSt5ctypeIcE13_M_widen_initEv@plt>:
    3ee4: c600         	<unknown>
    3ee6: e28f         	b	0x4408                  @ imm = #0x51e
    3ee8: ca1c         	ldm	r2, {r2, r3, r4}
    3eea: e28c         	b	0x4406                  @ imm = #0x518
    3eec: ff90 e5bc    	vsli.64	d14, d28, #0x10

00003ef0 <_ZN7pbnjson7JSchemaC1ERKS0_@plt>:
    3ef0: c600         	<unknown>
    3ef2: e28f         	b	0x4414                  @ imm = #0x51e
    3ef4: ca1c         	ldm	r2, {r2, r3, r4}
    3ef6: e28c         	b	0x4412                  @ imm = #0x518
    3ef8: ff88 e5bc    	vsli.64	d14, d28, #0x8
    3efc: 4778         	bx	pc
    3efe: e7fd         	b	0x3efc <_ZN7pbnjson7JSchemaC1ERKS0_@plt+0xc> @ imm = #-0x6

00003f00 <_ZdlPvj@plt>:
    3f00: c600         	<unknown>
    3f02: e28f         	b	0x4424                  @ imm = #0x51e
    3f04: ca1c         	ldm	r2, {r2, r3, r4}
    3f06: e28c         	b	0x4422                  @ imm = #0x518
    3f08: ff7c e5bc    	vqrshl.u64	d30, d28, d28

00003f0c <_ZNK7pbnjson6JValueixEPKc@plt>:
    3f0c: c600         	<unknown>
    3f0e: e28f         	b	0x4430                  @ imm = #0x51e
    3f10: ca1c         	ldm	r2, {r2, r3, r4}
    3f12: e28c         	b	0x442e                  @ imm = #0x518
    3f14: ff74 e5bc    	vqrshl.u64	d30, d28, d20
    3f18: 4778         	bx	pc
    3f1a: e7fd         	b	0x3f18 <_ZNK7pbnjson6JValueixEPKc@plt+0xc> @ imm = #-0x6

00003f1c <_ZNSo5flushEv@plt>:
    3f1c: c600         	<unknown>
    3f1e: e28f         	b	0x4440                  @ imm = #0x51e
    3f20: ca1c         	ldm	r2, {r2, r3, r4}
    3f22: e28c         	b	0x443e                  @ imm = #0x518
    3f24: ff68 e5bc    	vqrshl.u32	d30, d28, d24

00003f28 <LSErrorInit@plt>:
    3f28: c600         	<unknown>
    3f2a: e28f         	b	0x444c                  @ imm = #0x51e
    3f2c: ca1c         	ldm	r2, {r2, r3, r4}
    3f2e: e28c         	b	0x444a                  @ imm = #0x518
    3f30: ff60 e5bc    	vqrshl.u32	d30, d28, d16

00003f34 <LSGmainAttach@plt>:
    3f34: c600         	<unknown>
    3f36: e28f         	b	0x4458                  @ imm = #0x51e
    3f38: ca1c         	ldm	r2, {r2, r3, r4}
    3f3a: e28c         	b	0x4456                  @ imm = #0x518
    3f3c: ff58 e5bc    	vqrshl.u16	d30, d28, d24

00003f40 <_ZN9umiClient10initializeEv@plt>:
    3f40: c600         	<unknown>
    3f42: e28f         	b	0x4464                  @ imm = #0x51e
    3f44: ca1c         	ldm	r2, {r2, r3, r4}
    3f46: e28c         	b	0x4462                  @ imm = #0x518
    3f48: ff50 e5bc    	vqrshl.u16	d30, d28, d16

00003f4c <_ZN7pbnjson6JValueD2Ev@plt>:
    3f4c: c600         	<unknown>
    3f4e: e28f         	b	0x4470                  @ imm = #0x51e
    3f50: ca1c         	ldm	r2, {r2, r3, r4}
    3f52: e28c         	b	0x446e                  @ imm = #0x518
    3f54: ff48 e5bc    	vqrshl.u8	d30, d28, d24

00003f58 <_ZN9umiClient11setSettingsENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt>:
    3f58: c600         	<unknown>
    3f5a: e28f         	b	0x447c                  @ imm = #0x51e
    3f5c: ca1c         	ldm	r2, {r2, r3, r4}
    3f5e: e28c         	b	0x447a                  @ imm = #0x518
    3f60: ff40 e5bc    	vqrshl.u8	d30, d28, d16

00003f64 <_ZNSo3putEc@plt>:
    3f64: c600         	<unknown>
    3f66: e28f         	b	0x4488                  @ imm = #0x51e
    3f68: ca1c         	ldm	r2, {r2, r3, r4}
    3f6a: e28c         	b	0x4486                  @ imm = #0x518
    3f6c: ff38 e5bc    	vqrshl.u64	d14, d28, d24

00003f70 <g_io_add_watch@plt>:
    3f70: c600         	<unknown>
    3f72: e28f         	b	0x4494                  @ imm = #0x51e
    3f74: ca1c         	ldm	r2, {r2, r3, r4}
    3f76: e28c         	b	0x4492                  @ imm = #0x518
    3f78: ff30 e5bc    	vqrshl.u64	d14, d28, d16

00003f7c <g_io_channel_set_encoding@plt>:
    3f7c: c600         	<unknown>
    3f7e: e28f         	b	0x44a0                  @ imm = #0x51e
    3f80: ca1c         	ldm	r2, {r2, r3, r4}
    3f82: e28c         	b	0x449e                  @ imm = #0x518
    3f84: ff28 e5bc    	vqrshl.u32	d14, d28, d24

00003f88 <memcpy@plt>:
    3f88: c600         	<unknown>
    3f8a: e28f         	b	0x44ac                  @ imm = #0x51e
    3f8c: ca1c         	ldm	r2, {r2, r3, r4}
    3f8e: e28c         	b	0x44aa                  @ imm = #0x518
    3f90: ff20 e5bc    	vqrshl.u32	d14, d28, d16

00003f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>:
    3f94: c600         	<unknown>
    3f96: e28f         	b	0x44b8                  @ imm = #0x51e
    3f98: ca1c         	ldm	r2, {r2, r3, r4}
    3f9a: e28c         	b	0x44b6                  @ imm = #0x518
    3f9c: ff18 e5bc    	vqrshl.u16	d14, d28, d24

00003fa0 <g_io_channel_unref@plt>:
    3fa0: c600         	<unknown>
    3fa2: e28f         	b	0x44c4                  @ imm = #0x51e
    3fa4: ca1c         	ldm	r2, {r2, r3, r4}
    3fa6: e28c         	b	0x44c2                  @ imm = #0x518
    3fa8: ff10 e5bc    	vqrshl.u16	d14, d28, d16

00003fac <LSCategorySetData@plt>:
    3fac: c600         	<unknown>
    3fae: e28f         	b	0x44d0                  @ imm = #0x51e
    3fb0: ca1c         	ldm	r2, {r2, r3, r4}
    3fb2: e28c         	b	0x44ce                  @ imm = #0x518
    3fb4: ff08 e5bc    	vqrshl.u8	d14, d28, d24

00003fb8 <_Znwj@plt>:
    3fb8: c600         	<unknown>
    3fba: e28f         	b	0x44dc <main>           @ imm = #0x51e
    3fbc: ca1c         	ldm	r2, {r2, r3, r4}
    3fbe: e28c         	b	0x44da                  @ imm = #0x518
    3fc0: ff00 e5bc    	vqrshl.u8	d14, d28, d16

00003fc4 <__gxx_personality_v0@plt>:
    3fc4: c600         	<unknown>
    3fc6: e28f         	b	0x44e8 <main+0xc>       @ imm = #0x51e
    3fc8: ca1c         	ldm	r2, {r2, r3, r4}
    3fca: e28c         	b	0x44e6 <main+0xa>       @ imm = #0x518
    3fcc: fef8 e5bc    	mrc2	p5, #0x7, lr, c8, c12, #0x5

00003fd0 <_ZSt20__throw_length_errorPKc@plt>:
    3fd0: c600         	<unknown>
    3fd2: e28f         	b	0x44f4 <main+0x18>      @ imm = #0x51e
    3fd4: ca1c         	ldm	r2, {r2, r3, r4}
    3fd6: e28c         	b	0x44f2 <main+0x16>      @ imm = #0x518
    3fd8: fef0 e5bc    	mrc2	p5, #0x7, lr, c0, c12, #0x5

00003fdc <g_io_channel_set_buffered@plt>:
    3fdc: c600         	<unknown>
    3fde: e28f         	b	0x4500 <main+0x24>      @ imm = #0x51e
    3fe0: ca1c         	ldm	r2, {r2, r3, r4}
    3fe2: e28c         	b	0x44fe <main+0x22>      @ imm = #0x518
    3fe4: fee8 e5bc    	mcr2	p5, #0x7, lr, c8, c12, #0x5

00003fe8 <__cxa_throw@plt>:
    3fe8: c600         	<unknown>
    3fea: e28f         	b	0x450c <main+0x30>      @ imm = #0x51e
    3fec: ca1c         	ldm	r2, {r2, r3, r4}
    3fee: e28c         	b	0x450a <main+0x2e>      @ imm = #0x518
    3ff0: fee0 e5bc    	mcr2	p5, #0x7, lr, c0, c12, #0x5
    3ff4: 4778         	bx	pc
    3ff6: e7fd         	b	0x3ff4 <__cxa_throw@plt+0xc> @ imm = #-0x6

00003ff8 <PmLogGetContext@plt>:
    3ff8: c600         	<unknown>
    3ffa: e28f         	b	0x451c <main+0x40>      @ imm = #0x51e
    3ffc: ca1c         	ldm	r2, {r2, r3, r4}
    3ffe: e28c         	b	0x451a <main+0x3e>      @ imm = #0x518
    4000: fed4 e5bc    	mrc2	p5, #0x6, lr, c4, c12, #0x5

00004004 <LSErrorLogDefault@plt>:
    4004: c600         	<unknown>
    4006: e28f         	b	0x4528 <main+0x4c>      @ imm = #0x51e
    4008: ca1c         	ldm	r2, {r2, r3, r4}
    400a: e28c         	b	0x4526 <main+0x4a>      @ imm = #0x518
    400c: fecc e5bc    	mcr2	p5, #0x6, lr, c12, c12, #0x5

00004010 <_ZN2LS17SubscriptionPoint4postEPKc@plt>:
    4010: c600         	<unknown>
    4012: e28f         	b	0x4534 <main+0x58>      @ imm = #0x51e
    4014: ca1c         	ldm	r2, {r2, r3, r4}
    4016: e28c         	b	0x4532 <main+0x56>      @ imm = #0x518
    4018: fec4 e5bc    	mcr2	p5, #0x6, lr, c4, c12, #0x5

0000401c <_PmLogMsgKV@plt>:
    401c: c600         	<unknown>
    401e: e28f         	b	0x4540 <main+0x64>      @ imm = #0x51e
    4020: ca1c         	ldm	r2, {r2, r3, r4}
    4022: e28c         	b	0x453e <main+0x62>      @ imm = #0x518
    4024: febc e5bc    	mrc2	p5, #0x5, lr, c12, c12, #0x5

00004028 <_ZN7pbnjson10JGeneratorD1Ev@plt>:
    4028: c600         	<unknown>
    402a: e28f         	b	0x454c <main+0x70>      @ imm = #0x51e
    402c: ca1c         	ldm	r2, {r2, r3, r4}
    402e: e28c         	b	0x454a <main+0x6e>      @ imm = #0x518
    4030: feb4 e5bc    	mrc2	p5, #0x5, lr, c4, c12, #0x5

00004034 <__cxa_finalize@plt>:
    4034: c600         	<unknown>
    4036: e28f         	b	0x4558 <main+0x7c>      @ imm = #0x51e
    4038: ca1c         	ldm	r2, {r2, r3, r4}
    403a: e28c         	b	0x4556 <main+0x7a>      @ imm = #0x518
    403c: feac e5bc    	mcr2	p5, #0x5, lr, c12, c12, #0x5

00004040 <_ZN7pbnjson7JSchema9AllSchemaEv@plt>:
    4040: c600         	<unknown>
    4042: e28f         	b	0x4564 <main+0x88>      @ imm = #0x51e
    4044: ca1c         	ldm	r2, {r2, r3, r4}
    4046: e28c         	b	0x4562 <main+0x86>      @ imm = #0x518
    4048: fea4 e5bc    	mcr2	p5, #0x5, lr, c4, c12, #0x5

0000404c <LSMessageUnref@plt>:
    404c: c600         	<unknown>
    404e: e28f         	b	0x4570 <main+0x94>      @ imm = #0x51e
    4050: ca1c         	ldm	r2, {r2, r3, r4}
    4052: e28c         	b	0x456e <main+0x92>      @ imm = #0x518
    4054: fe9c e5bc    	mrc2	p5, #0x4, lr, c12, c12, #0x5

00004058 <_ZN7pbnjson10JGeneratorC1EPNS_9JResolverE@plt>:
    4058: c600         	<unknown>
    405a: e28f         	b	0x457c <main+0xa0>      @ imm = #0x51e
    405c: ca1c         	ldm	r2, {r2, r3, r4}
    405e: e28c         	b	0x457a <main+0x9e>      @ imm = #0x518
    4060: fe94 e5bc    	mrc2	p5, #0x4, lr, c4, c12, #0x5
    4064: 4778         	bx	pc
    4066: e7fd         	b	0x4064 <_ZN7pbnjson10JGeneratorC1EPNS_9JResolverE@plt+0xc> @ imm = #-0x6

00004068 <g_main_loop_quit@plt>:
    4068: c600         	<unknown>
    406a: e28f         	b	0x458c <main+0xb0>      @ imm = #0x51e
    406c: ca1c         	ldm	r2, {r2, r3, r4}
    406e: e28c         	b	0x458a <main+0xae>      @ imm = #0x518
    4070: fe88 e5bc    	mcr2	p5, #0x4, lr, c8, c12, #0x5

00004074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt>:
    4074: c600         	<unknown>
    4076: e28f         	b	0x4598 <main+0xbc>      @ imm = #0x51e
    4078: ca1c         	ldm	r2, {r2, r3, r4}
    407a: e28c         	b	0x4596 <main+0xba>      @ imm = #0x518
    407c: fe80 e5bc    	mcr2	p5, #0x4, lr, c0, c12, #0x5

00004080 <_ZN7pbnjson15JSchemaFragmentC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt>:
    4080: c600         	<unknown>
    4082: e28f         	b	0x45a4 <main+0xc8>      @ imm = #0x51e
    4084: ca1c         	ldm	r2, {r2, r3, r4}
    4086: e28c         	b	0x45a2 <main+0xc6>      @ imm = #0x518
    4088: fe78 e5bc    	mrc2	p5, #0x3, lr, c8, c12, #0x5

0000408c <g_io_channel_unix_get_fd@plt>:
    408c: c600         	<unknown>
    408e: e28f         	b	0x45b0 <main+0xd4>      @ imm = #0x51e
    4090: ca1c         	ldm	r2, {r2, r3, r4}
    4092: e28c         	b	0x45ae <main+0xd2>      @ imm = #0x518
    4094: fe70 e5bc    	mrc2	p5, #0x3, lr, c0, c12, #0x5

00004098 <_ZN7pbnjson10JGenerator8toStringERKNS_6JValueERKNS_7JSchemaERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt>:
    4098: c600         	<unknown>
    409a: e28f         	b	0x45bc <main+0xe0>      @ imm = #0x51e
    409c: ca1c         	ldm	r2, {r2, r3, r4}
    409e: e28c         	b	0x45ba <main+0xde>      @ imm = #0x518
    40a0: fe68 e5bc    	mcr2	p5, #0x3, lr, c8, c12, #0x5

000040a4 <strlen@plt>:
    40a4: c600         	<unknown>
    40a6: e28f         	b	0x45c8 <main+0xec>      @ imm = #0x51e
    40a8: ca1c         	ldm	r2, {r2, r3, r4}
    40aa: e28c         	b	0x45c6 <main+0xea>      @ imm = #0x518
    40ac: fe60 e5bc    	mcr2	p5, #0x3, lr, c0, c12, #0x5

000040b0 <__cxa_end_catch@plt>:
    40b0: c600         	<unknown>
    40b2: e28f         	b	0x45d4 <main+0xf8>      @ imm = #0x51e
    40b4: ca1c         	ldm	r2, {r2, r3, r4}
    40b6: e28c         	b	0x45d2 <main+0xf6>      @ imm = #0x518
    40b8: fe58 e5bc    	mrc2	p5, #0x2, lr, c8, c12, #0x5

000040bc <LSCallCancelNotificationRemove@plt>:
    40bc: c600         	<unknown>
    40be: e28f         	b	0x45e0 <main+0x104>     @ imm = #0x51e
    40c0: ca1c         	ldm	r2, {r2, r3, r4}
    40c2: e28c         	b	0x45de <main+0x102>     @ imm = #0x518
    40c4: fe50 e5bc    	mrc2	p5, #0x2, lr, c0, c12, #0x5

000040c8 <g_io_channel_unix_new@plt>:
    40c8: c600         	<unknown>
    40ca: e28f         	b	0x45ec <main+0x110>     @ imm = #0x51e
    40cc: ca1c         	ldm	r2, {r2, r3, r4}
    40ce: e28c         	b	0x45ea <main+0x10e>     @ imm = #0x518
    40d0: fe48 e5bc    	mcr2	p5, #0x2, lr, c8, c12, #0x5
    40d4: 4778         	bx	pc
    40d6: e7fd         	b	0x40d4 <g_io_channel_unix_new@plt+0xc> @ imm = #-0x6

000040d8 <__aeabi_atexit@plt>:
    40d8: c600         	<unknown>
    40da: e28f         	b	0x45fc <main+0x120>     @ imm = #0x51e
    40dc: ca1c         	ldm	r2, {r2, r3, r4}
    40de: e28c         	b	0x45fa <main+0x11e>     @ imm = #0x518
    40e0: fe3c e5bc    	mrc2	p5, #0x1, lr, c12, c12, #0x5

000040e4 <LSMessageRef@plt>:
    40e4: c600         	<unknown>
    40e6: e28f         	b	0x4608 <main+0x12c>     @ imm = #0x51e
    40e8: ca1c         	ldm	r2, {r2, r3, r4}
    40ea: e28c         	b	0x4606 <main+0x12a>     @ imm = #0x518
    40ec: fe34 e5bc    	mrc2	p5, #0x1, lr, c4, c12, #0x5

000040f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt>:
    40f0: c600         	<unknown>
    40f2: e28f         	b	0x4614 <main+0x138>     @ imm = #0x51e
    40f4: ca1c         	ldm	r2, {r2, r3, r4}
    40f6: e28c         	b	0x4612 <main+0x136>     @ imm = #0x518
    40f8: fe2c e5bc    	mcr2	p5, #0x1, lr, c12, c12, #0x5

000040fc <_ZN9umiClient15disconnectInputENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_@plt>:
    40fc: c600         	<unknown>
    40fe: e28f         	b	0x4620 <main+0x144>     @ imm = #0x51e
    4100: ca1c         	ldm	r2, {r2, r3, r4}
    4102: e28c         	b	0x461e <main+0x142>     @ imm = #0x518
    4104: fe24 e5bc    	mcr2	p5, #0x1, lr, c4, c12, #0x5

00004108 <g_main_loop_run@plt>:
    4108: c600         	<unknown>
    410a: e28f         	b	0x462c <main+0x150>     @ imm = #0x51e
    410c: ca1c         	ldm	r2, {r2, r3, r4}
    410e: e28c         	b	0x462a <main+0x14e>     @ imm = #0x518
    4110: fe1c e5bc    	mrc2	p5, #0x0, lr, c12, c12, #0x5

00004114 <jarray_create@plt>:
    4114: c600         	<unknown>
    4116: e28f         	b	0x4638 <main+0x15c>     @ imm = #0x51e
    4118: ca1c         	ldm	r2, {r2, r3, r4}
    411a: e28c         	b	0x4636 <main+0x15a>     @ imm = #0x518
    411c: fe14 e5bc    	mrc2	p5, #0x0, lr, c4, c12, #0x5

00004120 <LSRegisterCategory@plt>:
    4120: c600         	<unknown>
    4122: e28f         	b	0x4644 <main+0x168>     @ imm = #0x51e
    4124: ca1c         	ldm	r2, {r2, r3, r4}
    4126: e28c         	b	0x4642 <main+0x166>     @ imm = #0x518
    4128: fe0c e5bc    	mcr2	p5, #0x0, lr, c12, c12, #0x5

0000412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt>:
    412c: c600         	<unknown>
    412e: e28f         	b	0x4650 <main+0x174>     @ imm = #0x51e
    4130: ca1c         	ldm	r2, {r2, r3, r4}
    4132: e28c         	b	0x464e <main+0x172>     @ imm = #0x518
    4134: fe04 e5bc    	mcr2	p5, #0x0, lr, c4, c12, #0x5

00004138 <_ZN7pbnjson6JValueC1Ei@plt>:
    4138: c600         	<unknown>
    413a: e28f         	b	0x465c <main+0x180>     @ imm = #0x51e
    413c: ca1c         	ldm	r2, {r2, r3, r4}
    413e: e28c         	b	0x465a <main+0x17e>     @ imm = #0x518
    4140: fdfc e5bc    	ldc2l	p5, c14, [r12, #752]!

00004144 <signalfd@plt>:
    4144: c600         	<unknown>
    4146: e28f         	b	0x4668 <main+0x18c>     @ imm = #0x51e
    4148: ca1c         	ldm	r2, {r2, r3, r4}
    414a: e28c         	b	0x4666 <main+0x18a>     @ imm = #0x518
    414c: fdf4 e5bc    	ldc2l	p5, c14, [r4, #752]!

00004150 <memcmp@plt>:
    4150: c600         	<unknown>
    4152: e28f         	b	0x4674 <main+0x198>     @ imm = #0x51e
    4154: ca1c         	ldm	r2, {r2, r3, r4}
    4156: e28c         	b	0x4672 <main+0x196>     @ imm = #0x518
    4158: fdec e5bc    	stc2l	p5, c14, [r12, #752]!

0000415c <_ZN7pbnjson10JDomParser6getDomEv@plt>:
    415c: c600         	<unknown>
    415e: e28f         	b	0x4680 <main+0x1a4>     @ imm = #0x51e
    4160: ca1c         	ldm	r2, {r2, r3, r4}
    4162: e28c         	b	0x467e <main+0x1a2>     @ imm = #0x518
    4164: fde4 e5bc    	stc2l	p5, c14, [r4, #752]!

00004168 <_ZN7pbnjson10JDomParserD1Ev@plt>:
    4168: c600         	<unknown>
    416a: e28f         	b	0x468c <main+0x1b0>     @ imm = #0x51e
    416c: ca1c         	ldm	r2, {r2, r3, r4}
    416e: e28c         	b	0x468a <main+0x1ae>     @ imm = #0x518
    4170: fddc e5bc    	ldc2l	p5, c14, [r12, #752]

00004174 <g_timeout_add@plt>:
    4174: c600         	<unknown>
    4176: e28f         	b	0x4698 <main+0x1bc>     @ imm = #0x51e
    4178: ca1c         	ldm	r2, {r2, r3, r4}
    417a: e28c         	b	0x4696 <main+0x1ba>     @ imm = #0x518
    417c: fdd4 e5bc    	ldc2l	p5, c14, [r4, #752]

00004180 <__cxa_end_cleanup@plt>:
    4180: c600         	<unknown>
    4182: e28f         	b	0x46a4 <main+0x1c8>     @ imm = #0x51e
    4184: ca1c         	ldm	r2, {r2, r3, r4}
    4186: e28c         	b	0x46a2 <main+0x1c6>     @ imm = #0x518
    4188: fdcc e5bc    	stc2l	p5, c14, [r12, #752]

0000418c <LSErrorFree@plt>:
    418c: c600         	<unknown>
    418e: e28f         	b	0x46b0 <main+0x1d4>     @ imm = #0x51e
    4190: ca1c         	ldm	r2, {r2, r3, r4}
    4192: e28c         	b	0x46ae <main+0x1d2>     @ imm = #0x518
    4194: fdc4 e5bc    	stc2l	p5, c14, [r4, #752]

00004198 <LSMessageGetPayload@plt>:
    4198: c600         	<unknown>
    419a: e28f         	b	0x46bc <main+0x1e0>     @ imm = #0x51e
    419c: ca1c         	ldm	r2, {r2, r3, r4}
    419e: e28c         	b	0x46ba <main+0x1de>     @ imm = #0x518
    41a0: fdbc e5bc    	ldc2	p5, c14, [r12, #752]!

000041a4 <_ZNSt8ios_base4InitC1Ev@plt>:
    41a4: c600         	<unknown>
    41a6: e28f         	b	0x46c8 <main+0x1ec>     @ imm = #0x51e
    41a8: ca1c         	ldm	r2, {r2, r3, r4}
    41aa: e28c         	b	0x46c6 <main+0x1ea>     @ imm = #0x518
    41ac: fdb4 e5bc    	ldc2	p5, c14, [r4, #752]!

000041b0 <_ZNK7pbnjson6JValue8asNumberIiEET_v@plt>:
    41b0: c600         	<unknown>
    41b2: e28f         	b	0x46d4 <main+0x1f8>     @ imm = #0x51e
    41b4: ca1c         	ldm	r2, {r2, r3, r4}
    41b6: e28c         	b	0x46d2 <main+0x1f6>     @ imm = #0x518
    41b8: fdac e5bc    	stc2	p5, c14, [r12, #752]!

000041bc <abort@plt>:
    41bc: c600         	<unknown>
    41be: e28f         	b	0x46e0 <main+0x204>     @ imm = #0x51e
    41c0: ca1c         	ldm	r2, {r2, r3, r4}
    41c2: e28c         	b	0x46de <main+0x202>     @ imm = #0x518
    41c4: fda4 e5bc    	stc2	p5, c14, [r4, #752]!

000041c8 <_ZN9umiClient13setOutputMuteENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEb@plt>:
    41c8: c600         	<unknown>
    41ca: e28f         	b	0x46ec <main+0x210>     @ imm = #0x51e
    41cc: ca1c         	ldm	r2, {r2, r3, r4}
    41ce: e28c         	b	0x46ea <main+0x20e>     @ imm = #0x518
    41d0: fd9c e5bc    	ldc2	p5, c14, [r12, #752]

000041d4 <_ZN2LS17SubscriptionPoint9subscribeERNS_7MessageE@plt>:
    41d4: c600         	<unknown>
    41d6: e28f         	b	0x46f8 <main+0x21c>     @ imm = #0x51e
    41d8: ca1c         	ldm	r2, {r2, r3, r4}
    41da: e28c         	b	0x46f6 <main+0x21a>     @ imm = #0x518
    41dc: fd94 e5bc    	ldc2	p5, c14, [r4, #752]

000041e0 <_ZN7pbnjson7JSchemaD1Ev@plt>:
    41e0: c600         	<unknown>
    41e2: e28f         	b	0x4704 <main+0x228>     @ imm = #0x51e
    41e4: ca1c         	ldm	r2, {r2, r3, r4}
    41e6: e28c         	b	0x4702 <main+0x226>     @ imm = #0x518
    41e8: fd8c e5bc    	stc2	p5, c14, [r12, #752]

000041ec <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt>:
    41ec: c600         	<unknown>
    41ee: e28f         	b	0x4710 <main+0x234>     @ imm = #0x51e
    41f0: ca1c         	ldm	r2, {r2, r3, r4}
    41f2: e28c         	b	0x470e <main+0x232>     @ imm = #0x518
    41f4: fd84 e5bc    	stc2	p5, c14, [r4, #752]

000041f8 <exit@plt>:
    41f8: c600         	<unknown>
    41fa: e28f         	b	0x471c <main+0x240>     @ imm = #0x51e
    41fc: ca1c         	ldm	r2, {r2, r3, r4}
    41fe: e28c         	b	0x471a <main+0x23e>     @ imm = #0x518
    4200: fd7c e5bc    	ldc2l	p5, c14, [r12, #-752]!

00004204 <_ZSt9terminatev@plt>:
    4204: c600         	<unknown>
    4206: e28f         	b	0x4728 <main+0x24c>     @ imm = #0x51e
    4208: ca1c         	ldm	r2, {r2, r3, r4}
    420a: e28c         	b	0x4726 <main+0x24a>     @ imm = #0x518
    420c: fd74 e5bc    	ldc2l	p5, c14, [r4, #-752]!

00004210 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt>:
    4210: c600         	<unknown>
    4212: e28f         	b	0x4734 <main+0x258>     @ imm = #0x51e
    4214: ca1c         	ldm	r2, {r2, r3, r4}
    4216: e28c         	b	0x4732 <main+0x256>     @ imm = #0x518
    4218: fd6c e5bc    	stc2l	p5, c14, [r12, #-752]!

0000421c <_ZN7pbnjson6JValueC1Ev@plt>:
    421c: c600         	<unknown>
    421e: e28f         	b	0x4740 <main+0x264>     @ imm = #0x51e
    4220: ca1c         	ldm	r2, {r2, r3, r4}
    4222: e28c         	b	0x473e <main+0x262>     @ imm = #0x518
    4224: fd64 e5bc    	stc2l	p5, c14, [r4, #-752]!

00004228 <LSMessageRespond@plt>:
    4228: c600         	<unknown>
    422a: e28f         	b	0x474c <main+0x270>     @ imm = #0x51e
    422c: ca1c         	ldm	r2, {r2, r3, r4}
    422e: e28c         	b	0x474a <main+0x26e>     @ imm = #0x518
    4230: fd5c e5bc    	ldc2l	p5, c14, [r12, #-752]

00004234 <_ZNK7pbnjson6JValueeqEb@plt>:
    4234: c600         	<unknown>
    4236: e28f         	b	0x4758 <main+0x27c>     @ imm = #0x51e
    4238: ca1c         	ldm	r2, {r2, r3, r4}
    423a: e28c         	b	0x4756 <main+0x27a>     @ imm = #0x518
    423c: fd54 e5bc    	ldc2l	p5, c14, [r4, #-752]

00004240 <LSRegister@plt>:
    4240: c600         	<unknown>
    4242: e28f         	b	0x4764 <main+0x288>     @ imm = #0x51e
    4244: ca1c         	ldm	r2, {r2, r3, r4}
    4246: e28c         	b	0x4762 <main+0x286>     @ imm = #0x518
    4248: fd4c e5bc    	stc2l	p5, c14, [r12, #-752]

0000424c <_ZN9umiClient18removePipelineInfoENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt>:
    424c: c600         	<unknown>
    424e: e28f         	b	0x4770 <main+0x294>     @ imm = #0x51e
    4250: ca1c         	ldm	r2, {r2, r3, r4}
    4252: e28c         	b	0x476e <main+0x292>     @ imm = #0x518
    4254: fd44 e5bc    	stc2l	p5, c14, [r4, #-752]

00004258 <read@plt>:
    4258: c600         	<unknown>
    425a: e28f         	b	0x477c <main+0x2a0>     @ imm = #0x51e
    425c: ca1c         	ldm	r2, {r2, r3, r4}
    425e: e28c         	b	0x477a <main+0x29e>     @ imm = #0x518
    4260: fd3c e5bc    	ldc2	p5, c14, [r12, #-752]!

00004264 <LSUnregister@plt>:
    4264: c600         	<unknown>
    4266: e28f         	b	0x4788 <main+0x2ac>     @ imm = #0x51e
    4268: ca1c         	ldm	r2, {r2, r3, r4}
    426a: e28c         	b	0x4786 <main+0x2aa>     @ imm = #0x518
    426c: fd34 e5bc    	ldc2	p5, c14, [r4, #-752]!

00004270 <_ZN9umiClient11getInstanceEv@plt>:
    4270: c600         	<unknown>
    4272: e28f         	b	0x4794 <main+0x2b8>     @ imm = #0x51e
    4274: ca1c         	ldm	r2, {r2, r3, r4}
    4276: e28c         	b	0x4792 <main+0x2b6>     @ imm = #0x518
    4278: fd2c e5bc    	stc2	p5, c14, [r12, #-752]!

0000427c <_ZN9umiClient19setQualityAttributeEN7pbnjson6JValueE@plt>:
    427c: c600         	<unknown>
    427e: e28f         	b	0x47a0 <main+0x2c4>     @ imm = #0x51e
    4280: ca1c         	ldm	r2, {r2, r3, r4}
    4282: e28c         	b	0x479e <main+0x2c2>     @ imm = #0x518
    4284: fd24 e5bc    	stc2	p5, c14, [r4, #-752]!

00004288 <__libc_start_main@plt>:
    4288: c600         	<unknown>
    428a: e28f         	b	0x47ac <main+0x2d0>     @ imm = #0x51e
    428c: ca1c         	ldm	r2, {r2, r3, r4}
    428e: e28c         	b	0x47aa <main+0x2ce>     @ imm = #0x518
    4290: fd1c e5bc    	ldc2	p5, c14, [r12, #-752]

00004294 <_ZN9umiClient11getSettingsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt>:
    4294: c600         	<unknown>
    4296: e28f         	b	0x47b8 <main+0x2dc>     @ imm = #0x51e
    4298: ca1c         	ldm	r2, {r2, r3, r4}
    429a: e28c         	b	0x47b6 <main+0x2da>     @ imm = #0x518
    429c: fd14 e5bc    	ldc2	p5, c14, [r4, #-752]
    42a0: c600         	<unknown>
    42a2: e28f         	b	0x47c4 <main+0x2e8>     @ imm = #0x51e
    42a4: ca1c         	ldm	r2, {r2, r3, r4}
    42a6: e28c         	b	0x47c2 <main+0x2e6>     @ imm = #0x518
    42a8: fd0c e5bc    	stc2	p5, c14, [r12, #-752]

Disassembly of section .text:

000042ac <.text>:
    42ac: b5f0         	push	{r4, r5, r6, r7, lr}
    42ae: f100 0708    	add.w	r7, r0, #0x8
    42b2: b083         	sub	sp, #0xc
    42b4: 6007         	str	r7, [r0]
    42b6: b331         	cbz	r1, 0x4306 <.text+0x5a> @ imm = #0x4c
    42b8: 4604         	mov	r4, r0
    42ba: 4608         	mov	r0, r1
    42bc: 460e         	mov	r6, r1
    42be: f7ff eef2    	blx	0x40a4 <strlen@plt>     @ imm = #-0x21c
    42c2: 280f         	cmp	r0, #0xf
    42c4: 4605         	mov	r5, r0
    42c6: 9001         	str	r0, [sp, #0x4]
    42c8: d80e         	bhi	0x42e8 <.text+0x3c>     @ imm = #0x1c
    42ca: 2801         	cmp	r0, #0x1
    42cc: bf04         	itt	eq
    42ce: 7833         	ldrbeq	r3, [r6]
    42d0: 7223         	strbeq	r3, [r4, #0x8]
    42d2: d106         	bne	0x42e2 <.text+0x36>     @ imm = #0xc
    42d4: 9b01         	ldr	r3, [sp, #0x4]
    42d6: 2100         	movs	r1, #0x0
    42d8: 6822         	ldr	r2, [r4]
    42da: 6063         	str	r3, [r4, #0x4]
    42dc: 54d1         	strb	r1, [r2, r3]
    42de: b003         	add	sp, #0xc
    42e0: bdf0         	pop	{r4, r5, r6, r7, pc}
    42e2: 2800         	cmp	r0, #0x0
    42e4: d0f6         	beq	0x42d4 <.text+0x28>     @ imm = #-0x14
    42e6: e008         	b	0x42fa <.text+0x4e>     @ imm = #0x10
    42e8: 2200         	movs	r2, #0x0
    42ea: a901         	add	r1, sp, #0x4
    42ec: 4620         	mov	r0, r4
    42ee: f7ff eec2    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x27c
    42f2: 9b01         	ldr	r3, [sp, #0x4]
    42f4: 4607         	mov	r7, r0
    42f6: 6020         	str	r0, [r4]
    42f8: 60a3         	str	r3, [r4, #0x8]
    42fa: 462a         	mov	r2, r5
    42fc: 4631         	mov	r1, r6
    42fe: 4638         	mov	r0, r7
    4300: f7ff ee42    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x37c
    4304: e7e6         	b	0x42d4 <.text+0x28>     @ imm = #-0x34
    4306: 4802         	ldr	r0, [pc, #0x8]          @ 0x4310 <.text+0x64> literal=0x938c
    4308: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    430a: f7ff ecfc    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x608
    430e: bf00         	nop
    4310: 938c         	str	r3, [sp, #0x230]
    4312: 0000         	movs	r0, r0
    4314: e92d 47f0    	push.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    4318: 4c4a         	ldr	r4, [pc, #0x128]        @ 0x4444 <.text+0x198> literal=0x1cd24
    431a: 4e4b         	ldr	r6, [pc, #0x12c]        @ 0x4448 <.text+0x19c> literal=0x1ca90
    431c: 447c         	add	r4, pc => raw=0x21044 ghidra=0x31044 file=null
    431e: 4d4b         	ldr	r5, [pc, #0x12c]        @ 0x444c <.text+0x1a0> literal=0x1ccd6
    4320: 4620         	mov	r0, r4
    4322: 1d27         	adds	r7, r4, #0x4
    4324: f7ff ef3e    	blx	0x41a4 <_ZNSt8ios_base4InitC1Ev@plt> @ imm = #-0x184
    4328: 4b49         	ldr	r3, [pc, #0x124]        @ 0x4450 <.text+0x1a4> literal=0x214
    432a: 447d         	add	r5, pc => raw=0x21004 ghidra=0x31004 file=69636
    432c: 447e         	add	r6, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    432e: 462a         	mov	r2, r5
    4330: 4620         	mov	r0, r4
    4332: f104 0a1c    	add.w	r10, r4, #0x1c
    4336: 58f1         	ldr	r1, [r6, r3]
    4338: f104 0934    	add.w	r9, r4, #0x34
    433c: f7ff eecc    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x268
    4340: 4944         	ldr	r1, [pc, #0x110]        @ 0x4454 <.text+0x1a8> literal=0x9948
    4342: 4638         	mov	r0, r7
    4344: f104 084c    	add.w	r8, r4, #0x4c
    4348: 4479         	add	r1, pc => raw=0xdc94 ghidra=0x1dc94 file=56468 "Unknown error"
    434a: f7ff ffaf    	bl	0x42ac <.text>          @ imm = #-0xa2
    434e: 4b42         	ldr	r3, [pc, #0x108]        @ 0x4458 <.text+0x1ac> literal=0x220
    4350: 462a         	mov	r2, r5
    4352: 4638         	mov	r0, r7
    4354: f104 0764    	add.w	r7, r4, #0x64
    4358: 58f6         	ldr	r6, [r6, r3]
    435a: 4631         	mov	r1, r6
    435c: f7ff eebc    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x288
    4360: 493e         	ldr	r1, [pc, #0xf8]         @ 0x445c <.text+0x1b0> literal=0x993c
    4362: 4650         	mov	r0, r10
    4364: 4479         	add	r1, pc => raw=0xdca4 ghidra=0x1dca4 file=56484 "Failed to validate against schema"
    4366: f7ff ffa1    	bl	0x42ac <.text>          @ imm = #-0xbe
    436a: 462a         	mov	r2, r5
    436c: 4631         	mov	r1, r6
    436e: 4650         	mov	r0, r10
    4370: f7ff eeb2    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x29c
    4374: 493a         	ldr	r1, [pc, #0xe8]         @ 0x4460 <.text+0x1b4> literal=0x994c
    4376: 4648         	mov	r0, r9
    4378: 4479         	add	r1, pc => raw=0xdcc8 ghidra=0x1dcc8 file=56520 "Invalid parameters"
    437a: f7ff ff97    	bl	0x42ac <.text>          @ imm = #-0xd2
    437e: 462a         	mov	r2, r5
    4380: 4631         	mov	r1, r6
    4382: 4648         	mov	r0, r9
    4384: f7ff eea8    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x2b0
    4388: 4936         	ldr	r1, [pc, #0xd8]         @ 0x4464 <.text+0x1b8> literal=0x994c
    438a: 4640         	mov	r0, r8
    438c: 4479         	add	r1, pc => raw=0xdcdc ghidra=0x1dcdc file=56540 "Not implemented"
    438e: f7ff ff8d    	bl	0x42ac <.text>          @ imm = #-0xe6
    4392: 462a         	mov	r2, r5
    4394: 4631         	mov	r1, r6
    4396: 4640         	mov	r0, r8
    4398: f7ff ee9e    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x2c4
    439c: 4932         	ldr	r1, [pc, #0xc8]         @ 0x4468 <.text+0x1bc> literal=0x9948
    439e: 4638         	mov	r0, r7
    43a0: 4479         	add	r1, pc => raw=0xdcec ghidra=0x1dcec file=56556 "Driver error while executing the command"
    43a2: f7ff ff83    	bl	0x42ac <.text>          @ imm = #-0xfa
    43a6: 462a         	mov	r2, r5
    43a8: 4631         	mov	r1, r6
    43aa: 4638         	mov	r0, r7
    43ac: f104 077c    	add.w	r7, r4, #0x7c
    43b0: f7ff ee92    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x2dc
    43b4: 492d         	ldr	r1, [pc, #0xb4]         @ 0x446c <.text+0x1c0> literal=0x995c
    43b6: 4638         	mov	r0, r7
    43b8: 4479         	add	r1, pc => raw=0xdd18 ghidra=0x1dd18 file=56600 "Audio not connected"
    43ba: f7ff ff77    	bl	0x42ac <.text>          @ imm = #-0x112
    43be: 462a         	mov	r2, r5
    43c0: 4631         	mov	r1, r6
    43c2: 4638         	mov	r0, r7
    43c4: f104 0794    	add.w	r7, r4, #0x94
    43c8: f7ff ee86    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x2f4
    43cc: 4928         	ldr	r1, [pc, #0xa0]         @ 0x4470 <.text+0x1c4> literal=0x9958
    43ce: 4638         	mov	r0, r7
    43d0: 4479         	add	r1, pc => raw=0xdd2c ghidra=0x1dd2c file=56620 "soundOutput not implemented"
    43d2: f7ff ff6b    	bl	0x42ac <.text>          @ imm = #-0x12a
    43d6: 462a         	mov	r2, r5
    43d8: 4631         	mov	r1, r6
    43da: 4638         	mov	r0, r7
    43dc: f104 07ac    	add.w	r7, r4, #0xac
    43e0: f7ff ee7a    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x30c
    43e4: 4923         	ldr	r1, [pc, #0x8c]         @ 0x4474 <.text+0x1c8> literal=0x995c
    43e6: 4638         	mov	r0, r7
    43e8: 4479         	add	r1, pc => raw=0xdd48 ghidra=0x1dd48 file=56648 "Volume out of range"
    43ea: f7ff ff5f    	bl	0x42ac <.text>          @ imm = #-0x142
    43ee: 462a         	mov	r2, r5
    43f0: 4631         	mov	r1, r6
    43f2: 4638         	mov	r0, r7
    43f4: f104 07c4    	add.w	r7, r4, #0xc4
    43f8: f7ff ee6e    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x324
    43fc: 491e         	ldr	r1, [pc, #0x78]         @ 0x4478 <.text+0x1cc> literal=0x9958
    43fe: 4638         	mov	r0, r7
    4400: 4479         	add	r1, pc => raw=0xdd5c ghidra=0x1dd5c file=56668 "Volume already at max/min"
    4402: f7ff ff53    	bl	0x42ac <.text>          @ imm = #-0x15a
    4406: 462a         	mov	r2, r5
    4408: 4631         	mov	r1, r6
    440a: 4638         	mov	r0, r7
    440c: f104 07dc    	add.w	r7, r4, #0xdc
    4410: f7ff ee62    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x33c
    4414: 4919         	ldr	r1, [pc, #0x64]         @ 0x447c <.text+0x1d0> literal=0x995a
    4416: 4638         	mov	r0, r7
    4418: 34f4         	adds	r4, #0xf4
    441a: 4479         	add	r1, pc => raw=0xdd78 ghidra=0x1dd78 file=56696 "Connection not possible"
    441c: f7ff ff46    	bl	0x42ac <.text>          @ imm = #-0x174
    4420: 462a         	mov	r2, r5
    4422: 4631         	mov	r1, r6
    4424: 4638         	mov	r0, r7
    4426: f7ff ee58    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0x350
    442a: 4915         	ldr	r1, [pc, #0x54]         @ 0x4480 <.text+0x1d4> literal=0x995e
    442c: 4620         	mov	r0, r4
    442e: 4479         	add	r1, pc => raw=0xdd90 ghidra=0x1dd90 file=56720 "Volume control is not supported"
    4430: f7ff ff3c    	bl	0x42ac <.text>          @ imm = #-0x188
    4434: 462a         	mov	r2, r5
    4436: 4631         	mov	r1, r6
    4438: 4620         	mov	r0, r4
    443a: e8bd 47f0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    443e: f7ff be49    	b.w	0x40d4 <g_io_channel_unix_new@plt+0xc> @ imm = #-0x36e
    4442: bf00         	nop
    4444: cd24         	ldm	r5, {r2, r5}
    4446: 0001         	movs	r1, r0
    4448: ca90         	ldm	r2!, {r4, r7}
    444a: 0001         	movs	r1, r0
    444c: ccd6         	ldm	r4, {r1, r2, r4, r6, r7}
    444e: 0001         	movs	r1, r0
    4450: 0214         	lsls	r4, r2, #0x8
    4452: 0000         	movs	r0, r0
    4454: 9948         	ldr	r1, [sp, #0x120]
    4456: 0000         	movs	r0, r0
    4458: 0220         	lsls	r0, r4, #0x8
    445a: 0000         	movs	r0, r0
    445c: 993c         	ldr	r1, [sp, #0xf0]
    445e: 0000         	movs	r0, r0
    4460: 994c         	ldr	r1, [sp, #0x130]
    4462: 0000         	movs	r0, r0
    4464: 994c         	ldr	r1, [sp, #0x130]
    4466: 0000         	movs	r0, r0
    4468: 9948         	ldr	r1, [sp, #0x120]
    446a: 0000         	movs	r0, r0
    446c: 995c         	ldr	r1, [sp, #0x170]
    446e: 0000         	movs	r0, r0
    4470: 9958         	ldr	r1, [sp, #0x160]
    4472: 0000         	movs	r0, r0
    4474: 995c         	ldr	r1, [sp, #0x170]
    4476: 0000         	movs	r0, r0
    4478: 9958         	ldr	r1, [sp, #0x160]
    447a: 0000         	movs	r0, r0
    447c: 995a         	ldr	r1, [sp, #0x168]
    447e: 0000         	movs	r0, r0
    4480: 995e         	ldr	r1, [sp, #0x178]
    4482: 0000         	movs	r0, r0
    4484: b5f0         	push	{r4, r5, r6, r7, lr}
    4486: f100 0608    	add.w	r6, r0, #0x8
    448a: b083         	sub	sp, #0xc
    448c: 6006         	str	r6, [r0]
    448e: b1f9         	cbz	r1, 0x44d0 <.text+0x224> @ imm = #0x3e
    4490: 4604         	mov	r4, r0
    4492: 4608         	mov	r0, r1
    4494: 460d         	mov	r5, r1
    4496: f7ff ee06    	blx	0x40a4 <strlen@plt>     @ imm = #-0x3f4
    449a: 280f         	cmp	r0, #0xf
    449c: eb05 0700    	add.w	r7, r5, r0
    44a0: 9001         	str	r0, [sp, #0x4]
    44a2: d80b         	bhi	0x44bc <.text+0x210>    @ imm = #0x16
    44a4: 463a         	mov	r2, r7
    44a6: 4629         	mov	r1, r5
    44a8: 4630         	mov	r0, r6
    44aa: f7ff ec26    	blx	0x3cf8 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13_S_copy_charsEPcPKcS7_@plt> @ imm = #-0x7b4
    44ae: 9b01         	ldr	r3, [sp, #0x4]
    44b0: 2100         	movs	r1, #0x0
    44b2: 6822         	ldr	r2, [r4]
    44b4: 6063         	str	r3, [r4, #0x4]
    44b6: 54d1         	strb	r1, [r2, r3]
    44b8: b003         	add	sp, #0xc
    44ba: bdf0         	pop	{r4, r5, r6, r7, pc}
    44bc: 2200         	movs	r2, #0x0
    44be: a901         	add	r1, sp, #0x4
    44c0: 4620         	mov	r0, r4
    44c2: f7ff edd8    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x450
    44c6: 9b01         	ldr	r3, [sp, #0x4]
    44c8: 4606         	mov	r6, r0
    44ca: 6020         	str	r0, [r4]
    44cc: 60a3         	str	r3, [r4, #0x8]
    44ce: e7e9         	b	0x44a4 <.text+0x1f8>    @ imm = #-0x2e
    44d0: 4801         	ldr	r0, [pc, #0x4]          @ 0x44d8 <.text+0x22c> literal=0x91c2
    44d2: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    44d4: f7ff ec16    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x7d4
    44d8: 91c2         	str	r1, [sp, #0x308]
    44da: 0000         	movs	r0, r0

000044dc <main>:
    44dc: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    44e0: 2500         	movs	r5, #0x0
    44e2: b0c1         	sub	sp, #0x104
    44e4: f8df 7520    	ldr.w	r7, [pc, #0x520]        @ 0x4a08 <main+0x52c> literal=0x1c8ce
    44e8: 9009         	str	r0, [sp, #0x24]
    44ea: 4628         	mov	r0, r5
    44ec: 9108         	str	r1, [sp, #0x20]
    44ee: 447f         	add	r7, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    44f0: 950b         	str	r5, [sp, #0x2c]
    44f2: f7ff ec14    	blx	0x3d1c <g_option_context_new@plt> @ imm = #-0x7d8
    44f6: f8df 1514    	ldr.w	r1, [pc, #0x514]        @ 0x4a0c <main+0x530> literal=0x1cb06
    44fa: 4604         	mov	r4, r0
    44fc: 462a         	mov	r2, r5
    44fe: 4479         	add	r1, pc => raw=0x21008 ghidra=0x31008 file=69640
    4500: f7ff ecc6    	blx	0x3e90 <g_option_context_add_main_entries@plt> @ imm = #-0x674
    4504: ab0b         	add	r3, sp, #0x2c
    4506: aa08         	add	r2, sp, #0x20
    4508: a909         	add	r1, sp, #0x24
    450a: 4620         	mov	r0, r4
    450c: f7ff ec7e    	blx	0x3e0c <g_option_context_parse@plt> @ imm = #-0x704
    4510: 2800         	cmp	r0, #0x0
    4512: f000 8174    	beq.w	0x47fe <main+0x322>     @ imm = #0x2e8
    4516: f8df 54f8    	ldr.w	r5, [pc, #0x4f8]        @ 0x4a10 <main+0x534> literal=0xa258
    451a: 4620         	mov	r0, r4
    451c: f7ff ec5e    	blx	0x3ddc <g_option_context_free@plt> @ imm = #-0x744
    4520: 447d         	add	r5, pc => raw=0xe77c ghidra=0x1e77c file=59260 "audiooutputd"
    4522: 4628         	mov	r0, r5
    4524: f006 f804    	bl	0xa530 <setPmLogContext(char const*)> @ imm = #0x6008
    4528: 4604         	mov	r4, r0
    452a: 2800         	cmp	r0, #0x0
    452c: f040 817b    	bne.w	0x4826 <main+0x34a>     @ imm = #0x2f6
    4530: 4601         	mov	r1, r0
    4532: ad20         	add	r5, sp, #0x80
    4534: f7ff ec4c    	blx	0x3dd0 <g_main_loop_new@plt> @ imm = #-0x768
    4538: f8df 34d8    	ldr.w	r3, [pc, #0x4d8]        @ 0x4a14 <main+0x538> literal=0x1cc14
    453c: 447b         	add	r3, pc => raw=0x21154 ghidra=0x31154 file=null
    453e: 6058         	str	r0, [r3, #0x4]
    4540: 4628         	mov	r0, r5
    4542: f7ff ecca    	blx	0x3ed8 <sigemptyset@plt> @ imm = #-0x66c
    4546: 2102         	movs	r1, #0x2
    4548: 4628         	mov	r0, r5
    454a: f7ff ec1e    	blx	0x3d88 <sigaddset@plt>  @ imm = #-0x7c4
    454e: 210f         	movs	r1, #0xf
    4550: 4628         	mov	r0, r5
    4552: f7ff ec1a    	blx	0x3d88 <sigaddset@plt>  @ imm = #-0x7cc
    4556: 210a         	movs	r1, #0xa
    4558: 4628         	mov	r0, r5
    455a: f7ff ec16    	blx	0x3d88 <sigaddset@plt>  @ imm = #-0x7d4
    455e: 4622         	mov	r2, r4
    4560: 4629         	mov	r1, r5
    4562: 4620         	mov	r0, r4
    4564: f7ff ebec    	blx	0x3d40 <sigprocmask@plt> @ imm = #-0x828
    4568: 2800         	cmp	r0, #0x0
    456a: f2c0 80f5    	blt.w	0x4758 <main+0x27c>     @ imm = #0x1ea
    456e: 4622         	mov	r2, r4
    4570: 4629         	mov	r1, r5
    4572: f04f 30ff    	mov.w	r0, #0xffffffff
    4576: f7ff ede6    	blx	0x4144 <signalfd@plt>   @ imm = #-0x434
    457a: 2800         	cmp	r0, #0x0
    457c: f2c0 810a    	blt.w	0x4794 <main+0x2b8>     @ imm = #0x214
    4580: f7ff eda2    	blx	0x40c8 <g_io_channel_unix_new@plt> @ imm = #-0x4bc
    4584: 2101         	movs	r1, #0x1
    4586: 4606         	mov	r6, r0
    4588: f7ff ebd4    	blx	0x3d34 <g_io_channel_set_close_on_unref@plt> @ imm = #-0x858
    458c: 4622         	mov	r2, r4
    458e: 4621         	mov	r1, r4
    4590: 4630         	mov	r0, r6
    4592: f7ff ecf4    	blx	0x3f7c <g_io_channel_set_encoding@plt> @ imm = #-0x618
    4596: 4621         	mov	r1, r4
    4598: 4630         	mov	r0, r6
    459a: f7ff ed20    	blx	0x3fdc <g_io_channel_set_buffered@plt> @ imm = #-0x5c0
    459e: f8df 2478    	ldr.w	r2, [pc, #0x478]        @ 0x4a18 <main+0x53c> literal=0x5fb1
    45a2: 4623         	mov	r3, r4
    45a4: 2139         	movs	r1, #0x39
    45a6: 4630         	mov	r0, r6
    45a8: 447a         	add	r2, pc => raw=0xa55d ghidra=0x1a55d file=42333
    45aa: f7ff ece2    	blx	0x3f70 <g_io_add_watch@plt> @ imm = #-0x63c
    45ae: 4604         	mov	r4, r0
    45b0: 4630         	mov	r0, r6
    45b2: f7ff ecf6    	blx	0x3fa0 <g_io_channel_unref@plt> @ imm = #-0x614
    45b6: f7ff ee5c    	blx	0x4270 <_ZN9umiClient11getInstanceEv@plt> @ imm = #-0x348
    45ba: 4606         	mov	r6, r0
    45bc: b110         	cbz	r0, 0x45c4 <main+0xe8>  @ imm = #0x4
    45be: f7ff ecc0    	blx	0x3f40 <_ZN9umiClient10initializeEv@plt> @ imm = #-0x680
    45c2: bb20         	cbnz	r0, 0x460e <main+0x132> @ imm = #0x48
    45c4: f005 ffba    	bl	0xa53c <getPmLogContext()> @ imm = #0x5f74
    45c8: b120         	cbz	r0, 0x45d4 <main+0xf8>  @ imm = #0x8
    45ca: f005 ffb7    	bl	0xa53c <getPmLogContext()> @ imm = #0x5f6e
    45ce: 6803         	ldr	r3, [r0]
    45d0: 2b02         	cmp	r3, #0x2
    45d2: dd0f         	ble	0x45f4 <main+0x118>     @ imm = #0x1e
    45d4: f005 ffb2    	bl	0xa53c <getPmLogContext()> @ imm = #0x5f64
    45d8: f8df 1440    	ldr.w	r1, [pc, #0x440]        @ 0x4a1c <main+0x540> literal=0xa246
    45dc: 2200         	movs	r2, #0x0
    45de: f8df 3440    	ldr.w	r3, [pc, #0x440]        @ 0x4a20 <main+0x544> literal=0xa230
    45e2: e9cd 2201    	strd	r2, r2, [sp, #4]
    45e6: 4479         	add	r1, pc => raw=0xe830 ghidra=0x1e830 file=59440 "UMI init failed!stop AudiooutputD Service."
    45e8: 9200         	str	r2, [sp]
    45ea: 9103         	str	r1, [sp, #0xc]
    45ec: 447b         	add	r3, pc => raw=0xe820 ghidra=0x1e820 file=59424 "HAL_INIT_ERROR"
    45ee: 2103         	movs	r1, #0x3
    45f0: f7ff ed14    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x5d8
    45f4: 2004         	movs	r0, #0x4
    45f6: f7ff ec46    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0x774
    45fa: f8df 2428    	ldr.w	r2, [pc, #0x428]        @ 0x4a24 <main+0x548> literal=0xa1fe
    45fe: f8df 3428    	ldr.w	r3, [pc, #0x428]        @ 0x4a28 <main+0x54c> literal=0x234
    4602: 447a         	add	r2, pc => raw=0xe804 ghidra=0x1e804 file=59396 "stop AudiooutputD Service"
    4604: 6002         	str	r2, [r0]
    4606: 2200         	movs	r2, #0x0
    4608: 58f9         	ldr	r1, [r7, r3]
    460a: f7ff ecee    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x624
    460e: f8df 341c    	ldr.w	r3, [pc, #0x41c]        @ 0x4a2c <main+0x550> literal=0x1cb38
    4612: f10d 0984    	add.w	r9, sp, #0x84
    4616: 4648         	mov	r0, r9
    4618: 447b         	add	r3, pc => raw=0x21154 ghidra=0x31154 file=null
    461a: f8d3 a008    	ldr.w	r10, [r3, #0x8]
    461e: f8df 3410    	ldr.w	r3, [pc, #0x410]        @ 0x4a30 <main+0x554> literal=0x22c
    4622: f857 8003    	ldr.w	r8, [r7, r3]
    4626: f108 0308    	add.w	r3, r8, #0x8
    462a: 9320         	str	r3, [sp, #0x80]
    462c: f7ff ec7c    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x708
    4630: 4650         	mov	r0, r10
    4632: f10d 0a30    	add.w	r10, sp, #0x30
    4636: 464a         	mov	r2, r9
    4638: 4651         	mov	r1, r10
    463a: f7ff ee02    	blx	0x4240 <LSRegister@plt> @ imm = #-0x3fc
    463e: 2800         	cmp	r0, #0x0
    4640: f000 81d3    	beq.w	0x49ea <main+0x50e>     @ imm = #0x3a6
    4644: 4648         	mov	r0, r9
    4646: f108 0308    	add.w	r3, r8, #0x8
    464a: 9320         	str	r3, [sp, #0x80]
    464c: f10d 095c    	add.w	r9, sp, #0x5c
    4650: f7ff ed9c    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x4c8
    4654: 4628         	mov	r0, r5
    4656: f7ff eb68    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x930
    465a: 4632         	mov	r2, r6
    465c: 4651         	mov	r1, r10
    465e: 4648         	mov	r0, r9
    4660: f007 f9f2    	bl	0xba48 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)> @ imm = #0x73e4
    4664: 464a         	mov	r2, r9
    4666: 4633         	mov	r3, r6
    4668: 4651         	mov	r1, r10
    466a: 4628         	mov	r0, r5
    466c: f002 f95e    	bl	0x692c <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)> @ imm = #0x22bc
    4670: 4632         	mov	r2, r6
    4672: 4651         	mov	r1, r10
    4674: a80d         	add	r0, sp, #0x34
    4676: f006 f801    	bl	0xa67c <QualityManager::QualityManager(LS::Handle&, umiClient*)> @ imm = #0x6002
    467a: 4bee         	ldr	r3, [pc, #0x3b8]        @ 0x4a34 <main+0x558> literal=0x1cace
    467c: f10d 0940    	add.w	r9, sp, #0x40
    4680: 4648         	mov	r0, r9
    4682: 447b         	add	r3, pc => raw=0x21154 ghidra=0x31154 file=null
    4684: f8d3 b004    	ldr.w	r11, [r3, #0x4]
    4688: f108 0308    	add.w	r3, r8, #0x8
    468c: 930f         	str	r3, [sp, #0x3c]
    468e: f7ff ec4c    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x768
    4692: 4659         	mov	r1, r11
    4694: 980c         	ldr	r0, [sp, #0x30]
    4696: 464a         	mov	r2, r9
    4698: f10d 0b3c    	add.w	r11, sp, #0x3c
    469c: f7ff ec4a    	blx	0x3f34 <LSGmainAttach@plt> @ imm = #-0x76c
    46a0: 2800         	cmp	r0, #0x0
    46a2: f000 8133    	beq.w	0x490c <main+0x430>     @ imm = #0x266
    46a6: f108 0308    	add.w	r3, r8, #0x8
    46aa: 4648         	mov	r0, r9
    46ac: f10d 0b3c    	add.w	r11, sp, #0x3c
    46b0: 930f         	str	r3, [sp, #0x3c]
    46b2: 9307         	str	r3, [sp, #0x1c]
    46b4: f7ff ed6a    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x52c
    46b8: 4658         	mov	r0, r11
    46ba: f7ff eb36    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x994
    46be: 9b07         	ldr	r3, [sp, #0x1c]
    46c0: 4648         	mov	r0, r9
    46c2: 930f         	str	r3, [sp, #0x3c]
    46c4: f7ff ec30    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x7a0
    46c8: 49db         	ldr	r1, [pc, #0x36c]        @ 0x4a38 <main+0x55c> literal=0x5e75
    46ca: 464b         	mov	r3, r9
    46cc: 980c         	ldr	r0, [sp, #0x30]
    46ce: 2200         	movs	r2, #0x0
    46d0: 4479         	add	r1, pc => raw=0xa549 ghidra=0x1a549 file=42313
    46d2: f7ff ebf0    	blx	0x3eb4 <LSSetDisconnectHandler@plt> @ imm = #-0x820
    46d6: 2800         	cmp	r0, #0x0
    46d8: f000 8169    	beq.w	0x49ae <main+0x4d2>     @ imm = #0x2d2
    46dc: f108 0308    	add.w	r3, r8, #0x8
    46e0: 4648         	mov	r0, r9
    46e2: 930f         	str	r3, [sp, #0x3c]
    46e4: f7ff ed52    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x55c
    46e8: 4658         	mov	r0, r11
    46ea: f7ff eb1e    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x9c4
    46ee: 4bd3         	ldr	r3, [pc, #0x34c]        @ 0x4a3c <main+0x560> literal=0x1ca60
    46f0: 447b         	add	r3, pc => raw=0x21154 ghidra=0x31154 file=null
    46f2: 6858         	ldr	r0, [r3, #0x4]
    46f4: f7ff ed08    	blx	0x4108 <g_main_loop_run@plt> @ imm = #-0x5f0
    46f8: 4628         	mov	r0, r5
    46fa: f001 ff35    	bl	0x6568 <ConnectionManager::~ConnectionManager()> @ imm = #0x1e6a
    46fe: a819         	add	r0, sp, #0x64
    4700: f7ff ecf6    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x614
    4704: 4650         	mov	r0, r10
    4706: f005 ff87    	bl	0xa618 <getPmLogContext()+0xdc> @ imm = #0x5f0e
    470a: f005 ff17    	bl	0xa53c <getPmLogContext()> @ imm = #0x5e2e
    470e: b120         	cbz	r0, 0x471a <main+0x23e> @ imm = #0x8
    4710: f005 ff14    	bl	0xa53c <getPmLogContext()> @ imm = #0x5e28
    4714: 6803         	ldr	r3, [r0]
    4716: 2b05         	cmp	r3, #0x5
    4718: dd0d         	ble	0x4736 <main+0x25a>     @ imm = #0x1a
    471a: f005 ff0f    	bl	0xa53c <getPmLogContext()> @ imm = #0x5e1e
    471e: 4bc8         	ldr	r3, [pc, #0x320]        @ 0x4a40 <main+0x564> literal=0xa13a
    4720: 2200         	movs	r2, #0x0
    4722: 2106         	movs	r1, #0x6
    4724: e9cd 2201    	strd	r2, r2, [sp, #4]
    4728: 9200         	str	r2, [sp]
    472a: 447b         	add	r3, pc => raw=0xe868 ghidra=0x1e868 file=59496 "Terminating"
    472c: 9303         	str	r3, [sp, #0xc]
    472e: 4bc5         	ldr	r3, [pc, #0x314]        @ 0x4a44 <main+0x568> literal=0xa128
    4730: 447b         	add	r3, pc => raw=0xe85c ghidra=0x1e85c file=59484 "TERMINATING"
    4732: f7ff ec74    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x718
    4736: 4620         	mov	r0, r4
    4738: f7ff eb8c    	blx	0x3e54 <g_source_remove@plt> @ imm = #-0x8e8
    473c: 4bc2         	ldr	r3, [pc, #0x308]        @ 0x4a48 <main+0x56c> literal=0x1ca12
    473e: 447b         	add	r3, pc => raw=0x21154 ghidra=0x31154 file=null
    4740: 6858         	ldr	r0, [r3, #0x4]
    4742: f7ff ead4    	blx	0x3cec <g_main_loop_unref@plt> @ imm = #-0xa58
    4746: 4630         	mov	r0, r6
    4748: f7ff ebba    	blx	0x3ec0 <_ZN9umiClient12deinitializeEv@plt> @ imm = #-0x88c
    474c: 2800         	cmp	r0, #0x0
    474e: d03f         	beq	0x47d0 <main+0x2f4>     @ imm = #0x7e
    4750: 2000         	movs	r0, #0x0
    4752: b041         	add	sp, #0x104
    4754: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    4758: 4bbc         	ldr	r3, [pc, #0x2f0]        @ 0x4a4c <main+0x570> literal=0x23c
    475a: 49bd         	ldr	r1, [pc, #0x2f4]        @ 0x4a50 <main+0x574> literal=0xa04a
    475c: 58f8         	ldr	r0, [r7, r3]
    475e: 4479         	add	r1, pc => raw=0xe7ac ghidra=0x1e7ac file=59308 "Failed to set signal mask"
    4760: f7ff ec18    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x7d0
    4764: f005 feea    	bl	0xa53c <getPmLogContext()> @ imm = #0x5dd4
    4768: b128         	cbz	r0, 0x4776 <main+0x29a> @ imm = #0xa
    476a: f005 fee7    	bl	0xa53c <getPmLogContext()> @ imm = #0x5dce
    476e: 6803         	ldr	r3, [r0]
    4770: 2b02         	cmp	r3, #0x2
    4772: f77f af20    	ble.w	0x45b6 <main+0xda>      @ imm = #-0x1c0
    4776: f005 fee1    	bl	0xa53c <getPmLogContext()> @ imm = #0x5dc2
    477a: 4bb6         	ldr	r3, [pc, #0x2d8]        @ 0x4a54 <main+0x578> literal=0xa022
    477c: 2200         	movs	r2, #0x0
    477e: 2103         	movs	r1, #0x3
    4780: e9cd 2201    	strd	r2, r2, [sp, #4]
    4784: 9200         	str	r2, [sp]
    4786: 447b         	add	r3, pc => raw=0xe7ac ghidra=0x1e7ac file=59308 "Failed to set signal mask"
    4788: 9303         	str	r3, [sp, #0xc]
    478a: 4bb3         	ldr	r3, [pc, #0x2cc]        @ 0x4a58 <main+0x57c> literal=0xa038
    478c: 447b         	add	r3, pc => raw=0xe7c8 ghidra=0x1e7c8 file=59336 "SIGNAL_HANDLER_ERROR"
    478e: f7ff ec46    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x774
    4792: e710         	b	0x45b6 <main+0xda>      @ imm = #-0x1e0
    4794: 4bad         	ldr	r3, [pc, #0x2b4]        @ 0x4a4c <main+0x570> literal=0x23c
    4796: 49b1         	ldr	r1, [pc, #0x2c4]        @ 0x4a5c <main+0x580> literal=0xa042
    4798: 58f8         	ldr	r0, [r7, r3]
    479a: 4479         	add	r1, pc => raw=0xe7e0 ghidra=0x1e7e0 file=59360 "Failed to create signal descriptor"
    479c: f7ff ebfa    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x80c
    47a0: f005 fecc    	bl	0xa53c <getPmLogContext()> @ imm = #0x5d98
    47a4: b128         	cbz	r0, 0x47b2 <main+0x2d6> @ imm = #0xa
    47a6: f005 fec9    	bl	0xa53c <getPmLogContext()> @ imm = #0x5d92
    47aa: 6803         	ldr	r3, [r0]
    47ac: 2b02         	cmp	r3, #0x2
    47ae: f77f af02    	ble.w	0x45b6 <main+0xda>      @ imm = #-0x1fc
    47b2: f005 fec3    	bl	0xa53c <getPmLogContext()> @ imm = #0x5d86
    47b6: 4baa         	ldr	r3, [pc, #0x2a8]        @ 0x4a60 <main+0x584> literal=0xa01a
    47b8: 2200         	movs	r2, #0x0
    47ba: 2103         	movs	r1, #0x3
    47bc: e9cd 2201    	strd	r2, r2, [sp, #4]
    47c0: 9200         	str	r2, [sp]
    47c2: 447b         	add	r3, pc => raw=0xe7e0 ghidra=0x1e7e0 file=59360 "Failed to create signal descriptor"
    47c4: 9303         	str	r3, [sp, #0xc]
    47c6: 4ba7         	ldr	r3, [pc, #0x29c]        @ 0x4a64 <main+0x588> literal=0x9ffc
    47c8: 447b         	add	r3, pc => raw=0xe7c8 ghidra=0x1e7c8 file=59336 "SIGNAL_HANDLER_ERROR"
    47ca: f7ff ec28    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x7b0
    47ce: e6f2         	b	0x45b6 <main+0xda>      @ imm = #-0x21c
    47d0: f005 feb4    	bl	0xa53c <getPmLogContext()> @ imm = #0x5d68
    47d4: b120         	cbz	r0, 0x47e0 <main+0x304> @ imm = #0x8
    47d6: f005 feb1    	bl	0xa53c <getPmLogContext()> @ imm = #0x5d62
    47da: 6803         	ldr	r3, [r0]
    47dc: 2b02         	cmp	r3, #0x2
    47de: ddb7         	ble	0x4750 <main+0x274>     @ imm = #-0x92
    47e0: f005 feac    	bl	0xa53c <getPmLogContext()> @ imm = #0x5d58
    47e4: 49a0         	ldr	r1, [pc, #0x280]        @ 0x4a68 <main+0x58c> literal=0xa096
    47e6: 2200         	movs	r2, #0x0
    47e8: 4ba0         	ldr	r3, [pc, #0x280]        @ 0x4a6c <main+0x590> literal=0xa07c
    47ea: e9cd 2200    	strd	r2, r2, [sp]
    47ee: 4479         	add	r1, pc => raw=0xe888 ghidra=0x1e888 file=59528 "UMI deinitialization error. See logs for details."
    47f0: e9cd 2102    	strd	r2, r1, [sp, #8]
    47f4: 447b         	add	r3, pc => raw=0xe874 ghidra=0x1e874 file=59508 "HAL_DEINIT_ERROR"
    47f6: 2103         	movs	r1, #0x3
    47f8: f7ff ec10    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x7e0
    47fc: e7a8         	b	0x4750 <main+0x274>     @ imm = #-0xb0
    47fe: 9b0b         	ldr	r3, [sp, #0x2c]
    4800: b31b         	cbz	r3, 0x484a <main+0x36e> @ imm = #0x46
    4802: 4b92         	ldr	r3, [pc, #0x248]        @ 0x4a4c <main+0x570> literal=0x23c
    4804: 499a         	ldr	r1, [pc, #0x268]        @ 0x4a70 <main+0x594> literal=0x9f44
    4806: 58f8         	ldr	r0, [r7, r3]
    4808: 4479         	add	r1, pc => raw=0xe750 ghidra=0x1e750 file=59216 "[audiooutputd] "
    480a: f7ff ebc4    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x878
    480e: 9b0b         	ldr	r3, [sp, #0x2c]
    4810: 6899         	ldr	r1, [r3, #0x8]
    4812: f7ff ebc0    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x880
    4816: f005 fed3    	bl	0xa5c0 <getPmLogContext()+0x84> @ imm = #0x5da6
    481a: 980b         	ldr	r0, [sp, #0x2c]
    481c: f7ff eb44    	blx	0x3ea8 <g_error_free@plt> @ imm = #-0x978
    4820: 2001         	movs	r0, #0x1
    4822: f7ff ecea    	blx	0x41f8 <exit@plt>       @ imm = #-0x62c
    4826: 4b89         	ldr	r3, [pc, #0x224]        @ 0x4a4c <main+0x570> literal=0x23c
    4828: 4992         	ldr	r1, [pc, #0x248]        @ 0x4a74 <main+0x598> literal=0x9f20
    482a: 58f8         	ldr	r0, [r7, r3]
    482c: 4479         	add	r1, pc => raw=0xe750 ghidra=0x1e750 file=59216 "[audiooutputd] "
    482e: f7ff ebb2    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x89c
    4832: 4991         	ldr	r1, [pc, #0x244]        @ 0x4a78 <main+0x59c> literal=0x9f54
    4834: 4479         	add	r1, pc => raw=0xe78c ghidra=0x1e78c file=59276 "Failed to setup up log context "
    4836: f7ff ebae    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x8a4
    483a: 4629         	mov	r1, r5
    483c: f7ff ebaa    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x8ac
    4840: f005 febe    	bl	0xa5c0 <getPmLogContext()+0x84> @ imm = #0x5d7c
    4844: 2001         	movs	r0, #0x1
    4846: f7ff ecd8    	blx	0x41f8 <exit@plt>       @ imm = #-0x650
    484a: 4b80         	ldr	r3, [pc, #0x200]        @ 0x4a4c <main+0x570> literal=0x23c
    484c: 498b         	ldr	r1, [pc, #0x22c]        @ 0x4a7c <main+0x5a0> literal=0x9efc
    484e: 58f8         	ldr	r0, [r7, r3]
    4850: 4479         	add	r1, pc => raw=0xe750 ghidra=0x1e750 file=59216 "[audiooutputd] "
    4852: f7ff eba0    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x8c0
    4856: 498a         	ldr	r1, [pc, #0x228]        @ 0x4a80 <main+0x5a4> literal=0x9f04
    4858: 4479         	add	r1, pc => raw=0xe760 ghidra=0x1e760 file=59232 "An unknown error occurred"
    485a: f7ff eb9c    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x8c8
    485e: f005 feaf    	bl	0xa5c0 <getPmLogContext()+0x84> @ imm = #0x5d5e
    4862: e7dd         	b	0x4820 <main+0x344>     @ imm = #-0x46
    4864: 4604         	mov	r4, r0
    4866: 460d         	mov	r5, r1
    4868: e00e         	b	0x4888 <main+0x3ac>     @ imm = #0x1c
    486a: 4604         	mov	r4, r0
    486c: 4658         	mov	r0, r11
    486e: 460e         	mov	r6, r1
    4870: f005 f8f0    	bl	0x9a54 <ConnectionManager::removePipelineInfo(LSMessage&)+0x484> @ imm = #0x51e0
    4874: e004         	b	0x4880 <main+0x3a4>     @ imm = #0x8
    4876: 4604         	mov	r4, r0
    4878: 4658         	mov	r0, r11
    487a: 460e         	mov	r6, r1
    487c: f7ff ea54    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0xb58
    4880: 4628         	mov	r0, r5
    4882: 4635         	mov	r5, r6
    4884: f001 fe70    	bl	0x6568 <ConnectionManager::~ConnectionManager()> @ imm = #0x1ce0
    4888: a819         	add	r0, sp, #0x64
    488a: f7ff ec32    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x79c
    488e: 4650         	mov	r0, r10
    4890: f005 fec2    	bl	0xa618 <getPmLogContext()+0xdc> @ imm = #0x5d84
    4894: 4620         	mov	r0, r4
    4896: 462b         	mov	r3, r5
    4898: 2b01         	cmp	r3, #0x1
    489a: d146         	bne	0x492a <main+0x44e>     @ imm = #0x8c
    489c: f7ff ea38    	blx	0x3d10 <__cxa_begin_catch@plt> @ imm = #-0xb90
    48a0: 4b6a         	ldr	r3, [pc, #0x1a8]        @ 0x4a4c <main+0x570> literal=0x23c
    48a2: 4604         	mov	r4, r0
    48a4: 4977         	ldr	r1, [pc, #0x1dc]        @ 0x4a84 <main+0x5a8> literal=0x9ea4
    48a6: 58f8         	ldr	r0, [r7, r3]
    48a8: 4479         	add	r1, pc => raw=0xe750 ghidra=0x1e750 file=59216 "[audiooutputd] "
    48aa: f7ff eb74    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x918
    48ae: 4976         	ldr	r1, [pc, #0x1d8]        @ 0x4a88 <main+0x5ac> literal=0xa008
    48b0: 4479         	add	r1, pc => raw=0xe8bc ghidra=0x1e8bc file=59580 "Caught exception: '"
    48b2: f7ff eb70    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x920
    48b6: 6823         	ldr	r3, [r4]
    48b8: 4605         	mov	r5, r0
    48ba: 4620         	mov	r0, r4
    48bc: 689b         	ldr	r3, [r3, #0x8]
    48be: 4798         	blx	r3
    48c0: 4601         	mov	r1, r0
    48c2: 4628         	mov	r0, r5
    48c4: f7ff eb66    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x934
    48c8: 4970         	ldr	r1, [pc, #0x1c0]        @ 0x4a8c <main+0x5b0> literal=0xa002
    48ca: 4479         	add	r1, pc => raw=0xe8d0 ghidra=0x1e8d0 file=59600 "' exiting"
    48cc: f7ff eb62    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x93c
    48d0: f005 fe76    	bl	0xa5c0 <getPmLogContext()+0x84> @ imm = #0x5cec
    48d4: f005 fe32    	bl	0xa53c <getPmLogContext()> @ imm = #0x5c64
    48d8: 2800         	cmp	r0, #0x0
    48da: d14c         	bne	0x4976 <main+0x49a>     @ imm = #0x98
    48dc: f005 fe2e    	bl	0xa53c <getPmLogContext()> @ imm = #0x5c5c
    48e0: 6822         	ldr	r2, [r4]
    48e2: 4603         	mov	r3, r0
    48e4: 4620         	mov	r0, r4
    48e6: 461c         	mov	r4, r3
    48e8: 6893         	ldr	r3, [r2, #0x8]
    48ea: 4798         	blx	r3
    48ec: 4968         	ldr	r1, [pc, #0x1a0]        @ 0x4a90 <main+0x5b4> literal=0x9ff8
    48ee: 4605         	mov	r5, r0
    48f0: 4b68         	ldr	r3, [pc, #0x1a0]        @ 0x4a94 <main+0x5b8> literal=0x9fd6
    48f2: 2200         	movs	r2, #0x0
    48f4: 4620         	mov	r0, r4
    48f6: 9504         	str	r5, [sp, #0x10]
    48f8: 4479         	add	r1, pc => raw=0xe8f4 ghidra=0x1e8f4 file=59636 "%s, exiting."
    48fa: e9cd 2200    	strd	r2, r2, [sp]
    48fe: e9cd 2102    	strd	r2, r1, [sp, #8]
    4902: 447b         	add	r3, pc => raw=0xe8dc ghidra=0x1e8dc file=59612 "UNEXPECTED_EXCEPTION"
    4904: 2103         	movs	r1, #0x3
    4906: f7ff eb8a    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x8ec
    490a: e789         	b	0x4820 <main+0x344>     @ imm = #-0xee
    490c: 2020         	movs	r0, #0x20
    490e: f7ff eaba    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0xa8c
    4912: 4659         	mov	r1, r11
    4914: 4604         	mov	r4, r0
    4916: f005 f911    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #0x5222
    491a: 4b5f         	ldr	r3, [pc, #0x17c]        @ 0x4a98 <main+0x5bc> literal=0x228
    491c: 4620         	mov	r0, r4
    491e: 58fa         	ldr	r2, [r7, r3]
    4920: 4b5e         	ldr	r3, [pc, #0x178]        @ 0x4a9c <main+0x5c0> literal=0x208
    4922: 58f9         	ldr	r1, [r7, r3]
    4924: f7ff eb60    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x940
    4928: e79f         	b	0x486a <main+0x38e>     @ imm = #-0xc2
    492a: f7ff e9f2    	blx	0x3d10 <__cxa_begin_catch@plt> @ imm = #-0xc1c
    492e: 4b47         	ldr	r3, [pc, #0x11c]        @ 0x4a4c <main+0x570> literal=0x23c
    4930: 495b         	ldr	r1, [pc, #0x16c]        @ 0x4aa0 <main+0x5c4> literal=0x9e18
    4932: 58f8         	ldr	r0, [r7, r3]
    4934: 4479         	add	r1, pc => raw=0xe750 ghidra=0x1e750 file=59216 "[audiooutputd] "
    4936: f7ff eb2e    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x9a4
    493a: 495a         	ldr	r1, [pc, #0x168]        @ 0x4aa4 <main+0x5c8> literal=0x9fc4
    493c: 4479         	add	r1, pc => raw=0xe904 ghidra=0x1e904 file=59652 "Caught exception, exiting"
    493e: f7ff eb2a    	blx	0x3f94 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt> @ imm = #-0x9ac
    4942: f005 fe3d    	bl	0xa5c0 <getPmLogContext()+0x84> @ imm = #0x5c7a
    4946: f005 fdf9    	bl	0xa53c <getPmLogContext()> @ imm = #0x5bf2
    494a: b970         	cbnz	r0, 0x496a <main+0x48e> @ imm = #0x1c
    494c: f005 fdf6    	bl	0xa53c <getPmLogContext()> @ imm = #0x5bec
    4950: 4955         	ldr	r1, [pc, #0x154]        @ 0x4aa8 <main+0x5cc> literal=0x9fc2
    4952: 2200         	movs	r2, #0x0
    4954: 4b55         	ldr	r3, [pc, #0x154]        @ 0x4aac <main+0x5d0> literal=0x9f78
    4956: e9cd 2201    	strd	r2, r2, [sp, #4]
    495a: 4479         	add	r1, pc => raw=0xe920 ghidra=0x1e920 file=59680 "Exiting "
    495c: 9200         	str	r2, [sp]
    495e: 9103         	str	r1, [sp, #0xc]
    4960: 447b         	add	r3, pc => raw=0xe8dc ghidra=0x1e8dc file=59612 "UNEXPECTED_EXCEPTION"
    4962: 2103         	movs	r1, #0x3
    4964: f7ff eb5a    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x94c
    4968: e75a         	b	0x4820 <main+0x344>     @ imm = #-0x14c
    496a: f005 fde7    	bl	0xa53c <getPmLogContext()> @ imm = #0x5bce
    496e: 6803         	ldr	r3, [r0]
    4970: 2b02         	cmp	r3, #0x2
    4972: dceb         	bgt	0x494c <main+0x470>     @ imm = #-0x2a
    4974: e754         	b	0x4820 <main+0x344>     @ imm = #-0x158
    4976: f005 fde1    	bl	0xa53c <getPmLogContext()> @ imm = #0x5bc2
    497a: 6803         	ldr	r3, [r0]
    497c: 2b02         	cmp	r3, #0x2
    497e: dcad         	bgt	0x48dc <main+0x400>     @ imm = #-0xa6
    4980: e74e         	b	0x4820 <main+0x344>     @ imm = #-0x164
    4982: f7ff eb96    	blx	0x40b0 <__cxa_end_catch@plt> @ imm = #-0x8d4
    4986: f7ff ebfc    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x808
    498a: f7ff eb92    	blx	0x40b0 <__cxa_end_catch@plt> @ imm = #-0x8dc
    498e: f7ff ebf8    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x810
    4992: 4604         	mov	r4, r0
    4994: 460e         	mov	r6, r1
    4996: e773         	b	0x4880 <main+0x3a4>     @ imm = #-0x11a
    4998: 4604         	mov	r4, r0
    499a: a80f         	add	r0, sp, #0x3c
    499c: 460e         	mov	r6, r1
    499e: f7ff e9c4    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0xc78
    49a2: e76d         	b	0x4880 <main+0x3a4>     @ imm = #-0x126
    49a4: 4604         	mov	r4, r0
    49a6: 460d         	mov	r5, r1
    49a8: e771         	b	0x488e <main+0x3b2>     @ imm = #-0x11e
    49aa: 460b         	mov	r3, r1
    49ac: e774         	b	0x4898 <main+0x3bc>     @ imm = #-0x118
    49ae: 2020         	movs	r0, #0x20
    49b0: f7ff ea68    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0xb30
    49b4: 4659         	mov	r1, r11
    49b6: 4604         	mov	r4, r0
    49b8: f005 f8c0    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #0x5180
    49bc: 4b36         	ldr	r3, [pc, #0xd8]         @ 0x4a98 <main+0x5bc> literal=0x228
    49be: 4620         	mov	r0, r4
    49c0: 58fa         	ldr	r2, [r7, r3]
    49c2: 4b36         	ldr	r3, [pc, #0xd8]         @ 0x4a9c <main+0x5c0> literal=0x208
    49c4: 58f9         	ldr	r1, [r7, r3]
    49c6: f7ff eb10    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x9e0
    49ca: 4604         	mov	r4, r0
    49cc: 460e         	mov	r6, r1
    49ce: 4628         	mov	r0, r5
    49d0: f7ff e9aa    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0xcac
    49d4: 4620         	mov	r0, r4
    49d6: 4633         	mov	r3, r6
    49d8: e75e         	b	0x4898 <main+0x3bc>     @ imm = #-0x144
    49da: 4604         	mov	r4, r0
    49dc: 460e         	mov	r6, r1
    49de: 4628         	mov	r0, r5
    49e0: f005 f838    	bl	0x9a54 <ConnectionManager::removePipelineInfo(LSMessage&)+0x484> @ imm = #0x5070
    49e4: 4620         	mov	r0, r4
    49e6: 4633         	mov	r3, r6
    49e8: e756         	b	0x4898 <main+0x3bc>     @ imm = #-0x154
    49ea: 2020         	movs	r0, #0x20
    49ec: f7ff ea4a    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0xb6c
    49f0: 4629         	mov	r1, r5
    49f2: 4604         	mov	r4, r0
    49f4: f005 f8a2    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #0x5144
    49f8: 4a27         	ldr	r2, [pc, #0x9c]         @ 0x4a98 <main+0x5bc> literal=0x228
    49fa: 4620         	mov	r0, r4
    49fc: 4b27         	ldr	r3, [pc, #0x9c]         @ 0x4a9c <main+0x5c0> literal=0x208
    49fe: 58ba         	ldr	r2, [r7, r2]
    4a00: 58f9         	ldr	r1, [r7, r3]
    4a02: f7ff eaf2    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0xa1c
    4a06: bf00         	nop
    4a08: c8ce         	ldm	r0!, {r1, r2, r3, r6, r7}
    4a0a: 0001         	movs	r1, r0
    4a0c: cb06         	ldm	r3!, {r1, r2}
    4a0e: 0001         	movs	r1, r0
    4a10: a258         	adr	r2, #352 <main+0x590>
    4a12: 0000         	movs	r0, r0
    4a14: cc14         	ldm	r4, {r2, r4}
    4a16: 0001         	movs	r1, r0
    4a18: 5fb1         	ldrsh	r1, [r6, r6]
    4a1a: 0000         	movs	r0, r0
    4a1c: a246         	adr	r2, #280 <main+0x58a>
    4a1e: 0000         	movs	r0, r0
    4a20: a230         	adr	r2, #192 <main+0x578>
    4a22: 0000         	movs	r0, r0
    4a24: a1fe         	adr	r1, #1016 <main+0x64a>
    4a26: 0000         	movs	r0, r0
    4a28: 0234         	lsls	r4, r6, #0x8
    4a2a: 0000         	movs	r0, r0
    4a2c: cb38         	ldm	r3, {r3, r4, r5}
    4a2e: 0001         	movs	r1, r0
    4a30: 022c         	lsls	r4, r5, #0x8
    4a32: 0000         	movs	r0, r0
    4a34: cace         	ldm	r2, {r1, r2, r3, r6, r7}
    4a36: 0001         	movs	r1, r0
    4a38: 5e75         	ldrsh	r5, [r6, r1]
    4a3a: 0000         	movs	r0, r0
    4a3c: ca60         	ldm	r2!, {r5, r6}
    4a3e: 0001         	movs	r1, r0
    4a40: a13a         	adr	r1, #232 <main+0x5a2>
    4a42: 0000         	movs	r0, r0
    4a44: a128         	adr	r1, #160 <main+0x594>
    4a46: 0000         	movs	r0, r0
    4a48: ca12         	ldm	r2!, {r1, r4}
    4a4a: 0001         	movs	r1, r0
    4a4c: 023c         	lsls	r4, r7, #0x8
    4a4e: 0000         	movs	r0, r0
    4a50: a04a         	adr	r0, #296 <main+0x5c2>
    4a52: 0000         	movs	r0, r0
    4a54: a022         	adr	r0, #136 <main+0x59e>
    4a56: 0000         	movs	r0, r0
    4a58: a038         	adr	r0, #224 <main+0x5b8>
    4a5a: 0000         	movs	r0, r0
    4a5c: a042         	adr	r0, #264 <main+0x5c6>
    4a5e: 0000         	movs	r0, r0
    4a60: a01a         	adr	r0, #104 <main+0x5a2>
    4a62: 0000         	movs	r0, r0
    4a64: 9ffc         	ldr	r7, [sp, #0x3f0]
    4a66: 0000         	movs	r0, r0
    4a68: a096         	adr	r0, #600 <main+0x626>
    4a6a: 0000         	movs	r0, r0
    4a6c: a07c         	adr	r0, #496 <main+0x610>
    4a6e: 0000         	movs	r0, r0
    4a70: 9f44         	ldr	r7, [sp, #0x110]
    4a72: 0000         	movs	r0, r0
    4a74: 9f20         	ldr	r7, [sp, #0x80]
    4a76: 0000         	movs	r0, r0
    4a78: 9f54         	ldr	r7, [sp, #0x150]
    4a7a: 0000         	movs	r0, r0
    4a7c: 9efc         	ldr	r6, [sp, #0x3f0]
    4a7e: 0000         	movs	r0, r0
    4a80: 9f04         	ldr	r7, [sp, #0x10]
    4a82: 0000         	movs	r0, r0
    4a84: 9ea4         	ldr	r6, [sp, #0x290]
    4a86: 0000         	movs	r0, r0
    4a88: a008         	adr	r0, #32 <main+0x5b8>
    4a8a: 0000         	movs	r0, r0
    4a8c: a002         	adr	r0, #8 <main+0x5b6>
    4a8e: 0000         	movs	r0, r0
    4a90: 9ff8         	ldr	r7, [sp, #0x3e0]
    4a92: 0000         	movs	r0, r0
    4a94: 9fd6         	ldr	r7, [sp, #0x358]
    4a96: 0000         	movs	r0, r0
    4a98: 0228         	lsls	r0, r5, #0x8
    4a9a: 0000         	movs	r0, r0
    4a9c: 0208         	lsls	r0, r1, #0x8
    4a9e: 0000         	movs	r0, r0
    4aa0: 9e18         	ldr	r6, [sp, #0x60]
    4aa2: 0000         	movs	r0, r0
    4aa4: 9fc4         	ldr	r7, [sp, #0x310]
    4aa6: 0000         	movs	r0, r0
    4aa8: 9fc2         	ldr	r7, [sp, #0x308]
    4aaa: 0000         	movs	r0, r0
    4aac: 9f78         	ldr	r7, [sp, #0x1e0]
    4aae: 0000         	movs	r0, r0
    4ab0: e92d 47f0    	push.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    4ab4: 4c51         	ldr	r4, [pc, #0x144]        @ 0x4bfc <main+0x720> literal=0x1c698
    4ab6: 4e52         	ldr	r6, [pc, #0x148]        @ 0x4c00 <main+0x724> literal=0x1c2f6
    4ab8: 447c         	add	r4, pc => raw=0x21154 ghidra=0x31154 file=null
    4aba: 4d52         	ldr	r5, [pc, #0x148]        @ 0x4c04 <main+0x728> literal=0x1c532
    4abc: f104 0720    	add.w	r7, r4, #0x20
    4ac0: f104 0a3c    	add.w	r10, r4, #0x3c
    4ac4: 4638         	mov	r0, r7
    4ac6: 447e         	add	r6, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    4ac8: f7ff eb6c    	blx	0x41a4 <_ZNSt8ios_base4InitC1Ev@plt> @ imm = #-0x928
    4acc: 4b4e         	ldr	r3, [pc, #0x138]        @ 0x4c08 <main+0x72c> literal=0x214
    4ace: 447d         	add	r5, pc => raw=0x21004 ghidra=0x31004 file=69636
    4ad0: 462a         	mov	r2, r5
    4ad2: 4638         	mov	r0, r7
    4ad4: f104 0724    	add.w	r7, r4, #0x24
    4ad8: f104 0954    	add.w	r9, r4, #0x54
    4adc: 58f1         	ldr	r1, [r6, r3]
    4ade: f104 086c    	add.w	r8, r4, #0x6c
    4ae2: f7ff eafa    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xa0c
    4ae6: 4949         	ldr	r1, [pc, #0x124]        @ 0x4c0c <main+0x730> literal=0x91a6
    4ae8: 4638         	mov	r0, r7
    4aea: 4479         	add	r1, pc => raw=0xdc94 ghidra=0x1dc94 file=56468 "Unknown error"
    4aec: f7ff fcca    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x66c
    4af0: 4b47         	ldr	r3, [pc, #0x11c]        @ 0x4c10 <main+0x734> literal=0x220
    4af2: 462a         	mov	r2, r5
    4af4: 4638         	mov	r0, r7
    4af6: f104 0784    	add.w	r7, r4, #0x84
    4afa: 58f6         	ldr	r6, [r6, r3]
    4afc: 4631         	mov	r1, r6
    4afe: f7ff eaec    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xa28
    4b02: 4944         	ldr	r1, [pc, #0x110]        @ 0x4c14 <main+0x738> literal=0x919a
    4b04: 4650         	mov	r0, r10
    4b06: 4479         	add	r1, pc => raw=0xdca4 ghidra=0x1dca4 file=56484 "Failed to validate against schema"
    4b08: f7ff fcbc    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x688
    4b0c: 462a         	mov	r2, r5
    4b0e: 4631         	mov	r1, r6
    4b10: 4650         	mov	r0, r10
    4b12: f7ff eae2    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xa3c
    4b16: 4940         	ldr	r1, [pc, #0x100]        @ 0x4c18 <main+0x73c> literal=0x91aa
    4b18: 4648         	mov	r0, r9
    4b1a: 4479         	add	r1, pc => raw=0xdcc8 ghidra=0x1dcc8 file=56520 "Invalid parameters"
    4b1c: f7ff fcb2    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x69c
    4b20: 462a         	mov	r2, r5
    4b22: 4631         	mov	r1, r6
    4b24: 4648         	mov	r0, r9
    4b26: f7ff ead8    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xa50
    4b2a: 493c         	ldr	r1, [pc, #0xf0]         @ 0x4c1c <main+0x740> literal=0x91aa
    4b2c: 4640         	mov	r0, r8
    4b2e: 4479         	add	r1, pc => raw=0xdcdc ghidra=0x1dcdc file=56540 "Not implemented"
    4b30: f7ff fca8    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x6b0
    4b34: 462a         	mov	r2, r5
    4b36: 4631         	mov	r1, r6
    4b38: 4640         	mov	r0, r8
    4b3a: f7ff eace    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xa64
    4b3e: 4938         	ldr	r1, [pc, #0xe0]         @ 0x4c20 <main+0x744> literal=0x91a6
    4b40: 4638         	mov	r0, r7
    4b42: 4479         	add	r1, pc => raw=0xdcec ghidra=0x1dcec file=56556 "Driver error while executing the command"
    4b44: f7ff fc9e    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x6c4
    4b48: 462a         	mov	r2, r5
    4b4a: 4631         	mov	r1, r6
    4b4c: 4638         	mov	r0, r7
    4b4e: f104 079c    	add.w	r7, r4, #0x9c
    4b52: f7ff eac2    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xa7c
    4b56: 4933         	ldr	r1, [pc, #0xcc]         @ 0x4c24 <main+0x748> literal=0x91ba
    4b58: 4638         	mov	r0, r7
    4b5a: 4479         	add	r1, pc => raw=0xdd18 ghidra=0x1dd18 file=56600 "Audio not connected"
    4b5c: f7ff fc92    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x6dc
    4b60: 462a         	mov	r2, r5
    4b62: 4631         	mov	r1, r6
    4b64: 4638         	mov	r0, r7
    4b66: f104 07b4    	add.w	r7, r4, #0xb4
    4b6a: f7ff eab6    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xa94
    4b6e: 492e         	ldr	r1, [pc, #0xb8]         @ 0x4c28 <main+0x74c> literal=0x91b6
    4b70: 4638         	mov	r0, r7
    4b72: 4479         	add	r1, pc => raw=0xdd2c ghidra=0x1dd2c file=56620 "soundOutput not implemented"
    4b74: f7ff fc86    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x6f4
    4b78: 462a         	mov	r2, r5
    4b7a: 4631         	mov	r1, r6
    4b7c: 4638         	mov	r0, r7
    4b7e: f104 07cc    	add.w	r7, r4, #0xcc
    4b82: f7ff eaaa    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xaac
    4b86: 4929         	ldr	r1, [pc, #0xa4]         @ 0x4c2c <main+0x750> literal=0x91ba
    4b88: 4638         	mov	r0, r7
    4b8a: 4479         	add	r1, pc => raw=0xdd48 ghidra=0x1dd48 file=56648 "Volume out of range"
    4b8c: f7ff fc7a    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x70c
    4b90: 462a         	mov	r2, r5
    4b92: 4631         	mov	r1, r6
    4b94: 4638         	mov	r0, r7
    4b96: f104 07e4    	add.w	r7, r4, #0xe4
    4b9a: f7ff ea9e    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xac4
    4b9e: 4924         	ldr	r1, [pc, #0x90]         @ 0x4c30 <main+0x754> literal=0x91b6
    4ba0: 4638         	mov	r0, r7
    4ba2: 4479         	add	r1, pc => raw=0xdd5c ghidra=0x1dd5c file=56668 "Volume already at max/min"
    4ba4: f7ff fc6e    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x724
    4ba8: 462a         	mov	r2, r5
    4baa: 4631         	mov	r1, r6
    4bac: 4638         	mov	r0, r7
    4bae: f104 07fc    	add.w	r7, r4, #0xfc
    4bb2: f7ff ea92    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xadc
    4bb6: 491f         	ldr	r1, [pc, #0x7c]         @ 0x4c34 <main+0x758> literal=0x91ba
    4bb8: 4638         	mov	r0, r7
    4bba: 4479         	add	r1, pc => raw=0xdd78 ghidra=0x1dd78 file=56696 "Connection not possible"
    4bbc: f7ff fc62    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x73c
    4bc0: 462a         	mov	r2, r5
    4bc2: 4631         	mov	r1, r6
    4bc4: 4638         	mov	r0, r7
    4bc6: f504 778a    	add.w	r7, r4, #0x114
    4bca: f7ff ea86    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xaf4
    4bce: 491a         	ldr	r1, [pc, #0x68]         @ 0x4c38 <main+0x75c> literal=0x91b8
    4bd0: 4638         	mov	r0, r7
    4bd2: 3408         	adds	r4, #0x8
    4bd4: 4479         	add	r1, pc => raw=0xdd90 ghidra=0x1dd90 file=56720 "Volume control is not supported"
    4bd6: f7ff fc55    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x756
    4bda: 462a         	mov	r2, r5
    4bdc: 4631         	mov	r1, r6
    4bde: 4638         	mov	r0, r7
    4be0: f7ff ea7a    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xb0c
    4be4: 4915         	ldr	r1, [pc, #0x54]         @ 0x4c3c <main+0x760> literal=0x9d40
    4be6: 4620         	mov	r0, r4
    4be8: 4479         	add	r1, pc => raw=0xe92c ghidra=0x1e92c file=59692 "com.webos.service.audiooutput"
    4bea: f7ff fc4b    	bl	0x4484 <.text+0x1d8>    @ imm = #-0x76a
    4bee: 462a         	mov	r2, r5
    4bf0: 4631         	mov	r1, r6
    4bf2: 4620         	mov	r0, r4
    4bf4: e8bd 47f0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    4bf8: f7ff ba6c    	b.w	0x40d4 <g_io_channel_unix_new@plt+0xc> @ imm = #-0xb28
    4bfc: c698         	stm	r6!, {r3, r4, r7}
    4bfe: 0001         	movs	r1, r0
    4c00: c2f6         	stm	r2!, {r1, r2, r4, r5, r6, r7}
    4c02: 0001         	movs	r1, r0
    4c04: c532         	stm	r5!, {r1, r4, r5}
    4c06: 0001         	movs	r1, r0
    4c08: 0214         	lsls	r4, r2, #0x8
    4c0a: 0000         	movs	r0, r0
    4c0c: 91a6         	str	r1, [sp, #0x298]
    4c0e: 0000         	movs	r0, r0
    4c10: 0220         	lsls	r0, r4, #0x8
    4c12: 0000         	movs	r0, r0
    4c14: 919a         	str	r1, [sp, #0x268]
    4c16: 0000         	movs	r0, r0
    4c18: 91aa         	str	r1, [sp, #0x2a8]
    4c1a: 0000         	movs	r0, r0
    4c1c: 91aa         	str	r1, [sp, #0x2a8]
    4c1e: 0000         	movs	r0, r0
    4c20: 91a6         	str	r1, [sp, #0x298]
    4c22: 0000         	movs	r0, r0
    4c24: 91ba         	str	r1, [sp, #0x2e8]
    4c26: 0000         	movs	r0, r0
    4c28: 91b6         	str	r1, [sp, #0x2d8]
    4c2a: 0000         	movs	r0, r0
    4c2c: 91ba         	str	r1, [sp, #0x2e8]
    4c2e: 0000         	movs	r0, r0
    4c30: 91b6         	str	r1, [sp, #0x2d8]
    4c32: 0000         	movs	r0, r0
    4c34: 91ba         	str	r1, [sp, #0x2e8]
    4c36: 0000         	movs	r0, r0
    4c38: 91b8         	str	r1, [sp, #0x2e0]
    4c3a: 0000         	movs	r0, r0
    4c3c: 9d40         	ldr	r5, [sp, #0x100]
    4c3e: 0000         	movs	r0, r0
    4c40: b5f0         	push	{r4, r5, r6, r7, lr}
    4c42: f100 0708    	add.w	r7, r0, #0x8
    4c46: b083         	sub	sp, #0xc
    4c48: 6007         	str	r7, [r0]
    4c4a: b331         	cbz	r1, 0x4c9a <main+0x7be> @ imm = #0x4c
    4c4c: 4604         	mov	r4, r0
    4c4e: 4608         	mov	r0, r1
    4c50: 460e         	mov	r6, r1
    4c52: f7ff ea28    	blx	0x40a4 <strlen@plt>     @ imm = #-0xbb0
    4c56: 280f         	cmp	r0, #0xf
    4c58: 4605         	mov	r5, r0
    4c5a: 9001         	str	r0, [sp, #0x4]
    4c5c: d80e         	bhi	0x4c7c <main+0x7a0>     @ imm = #0x1c
    4c5e: 2801         	cmp	r0, #0x1
    4c60: bf04         	itt	eq
    4c62: 7833         	ldrbeq	r3, [r6]
    4c64: 7223         	strbeq	r3, [r4, #0x8]
    4c66: d106         	bne	0x4c76 <main+0x79a>     @ imm = #0xc
    4c68: 9b01         	ldr	r3, [sp, #0x4]
    4c6a: 2100         	movs	r1, #0x0
    4c6c: 6822         	ldr	r2, [r4]
    4c6e: 6063         	str	r3, [r4, #0x4]
    4c70: 54d1         	strb	r1, [r2, r3]
    4c72: b003         	add	sp, #0xc
    4c74: bdf0         	pop	{r4, r5, r6, r7, pc}
    4c76: 2800         	cmp	r0, #0x0
    4c78: d0f6         	beq	0x4c68 <main+0x78c>     @ imm = #-0x14
    4c7a: e008         	b	0x4c8e <main+0x7b2>     @ imm = #0x10
    4c7c: 2200         	movs	r2, #0x0
    4c7e: a901         	add	r1, sp, #0x4
    4c80: 4620         	mov	r0, r4
    4c82: f7ff e9f8    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0xc10
    4c86: 9b01         	ldr	r3, [sp, #0x4]
    4c88: 4607         	mov	r7, r0
    4c8a: 6020         	str	r0, [r4]
    4c8c: 60a3         	str	r3, [r4, #0x8]
    4c8e: 462a         	mov	r2, r5
    4c90: 4631         	mov	r1, r6
    4c92: 4638         	mov	r0, r7
    4c94: f7ff e978    	blx	0x3f88 <memcpy@plt>     @ imm = #-0xd10
    4c98: e7e6         	b	0x4c68 <main+0x78c>     @ imm = #-0x34
    4c9a: 4802         	ldr	r0, [pc, #0x8]          @ 0x4ca4 <main+0x7c8> literal=0x89f8
    4c9c: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    4c9e: f7ff e832    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0xf9c
    4ca2: bf00         	nop
    4ca4: 89f8         	ldrh	r0, [r7, #0xe]
    4ca6: 0000         	movs	r0, r0
    4ca8: e92d 47f0    	push.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    4cac: 4c4a         	ldr	r4, [pc, #0x128]        @ 0x4dd8 <main+0x8fc> literal=0x1c5d0
    4cae: 4e4b         	ldr	r6, [pc, #0x12c]        @ 0x4ddc <main+0x900> literal=0x1c0fc
    4cb0: 447c         	add	r4, pc => raw=0x21284 ghidra=0x31284 file=null
    4cb2: 4d4b         	ldr	r5, [pc, #0x12c]        @ 0x4de0 <main+0x904> literal=0x1c342
    4cb4: 4620         	mov	r0, r4
    4cb6: 1d27         	adds	r7, r4, #0x4
    4cb8: f7ff ea74    	blx	0x41a4 <_ZNSt8ios_base4InitC1Ev@plt> @ imm = #-0xb18
    4cbc: 4b49         	ldr	r3, [pc, #0x124]        @ 0x4de4 <main+0x908> literal=0x214
    4cbe: 447d         	add	r5, pc => raw=0x21004 ghidra=0x31004 file=69636
    4cc0: 447e         	add	r6, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    4cc2: 462a         	mov	r2, r5
    4cc4: 4620         	mov	r0, r4
    4cc6: f104 0a1c    	add.w	r10, r4, #0x1c
    4cca: 58f1         	ldr	r1, [r6, r3]
    4ccc: f104 0934    	add.w	r9, r4, #0x34
    4cd0: f7ff ea02    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xbfc
    4cd4: 4944         	ldr	r1, [pc, #0x110]        @ 0x4de8 <main+0x90c> literal=0x8fb4
    4cd6: 4638         	mov	r0, r7
    4cd8: f104 084c    	add.w	r8, r4, #0x4c
    4cdc: 4479         	add	r1, pc => raw=0xdc94 ghidra=0x1dc94 file=56468 "Unknown error"
    4cde: f7ff ffaf    	bl	0x4c40 <main+0x764>     @ imm = #-0xa2
    4ce2: 4b42         	ldr	r3, [pc, #0x108]        @ 0x4dec <main+0x910> literal=0x220
    4ce4: 462a         	mov	r2, r5
    4ce6: 4638         	mov	r0, r7
    4ce8: f104 0764    	add.w	r7, r4, #0x64
    4cec: 58f6         	ldr	r6, [r6, r3]
    4cee: 4631         	mov	r1, r6
    4cf0: f7ff e9f2    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xc1c
    4cf4: 493e         	ldr	r1, [pc, #0xf8]         @ 0x4df0 <main+0x914> literal=0x8fa8
    4cf6: 4650         	mov	r0, r10
    4cf8: 4479         	add	r1, pc => raw=0xdca4 ghidra=0x1dca4 file=56484 "Failed to validate against schema"
    4cfa: f7ff ffa1    	bl	0x4c40 <main+0x764>     @ imm = #-0xbe
    4cfe: 462a         	mov	r2, r5
    4d00: 4631         	mov	r1, r6
    4d02: 4650         	mov	r0, r10
    4d04: f7ff e9e8    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xc30
    4d08: 493a         	ldr	r1, [pc, #0xe8]         @ 0x4df4 <main+0x918> literal=0x8fb8
    4d0a: 4648         	mov	r0, r9
    4d0c: 4479         	add	r1, pc => raw=0xdcc8 ghidra=0x1dcc8 file=56520 "Invalid parameters"
    4d0e: f7ff ff97    	bl	0x4c40 <main+0x764>     @ imm = #-0xd2
    4d12: 462a         	mov	r2, r5
    4d14: 4631         	mov	r1, r6
    4d16: 4648         	mov	r0, r9
    4d18: f7ff e9de    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xc44
    4d1c: 4936         	ldr	r1, [pc, #0xd8]         @ 0x4df8 <main+0x91c> literal=0x8fb8
    4d1e: 4640         	mov	r0, r8
    4d20: 4479         	add	r1, pc => raw=0xdcdc ghidra=0x1dcdc file=56540 "Not implemented"
    4d22: f7ff ff8d    	bl	0x4c40 <main+0x764>     @ imm = #-0xe6
    4d26: 462a         	mov	r2, r5
    4d28: 4631         	mov	r1, r6
    4d2a: 4640         	mov	r0, r8
    4d2c: f7ff e9d4    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xc58
    4d30: 4932         	ldr	r1, [pc, #0xc8]         @ 0x4dfc <main+0x920> literal=0x8fb4
    4d32: 4638         	mov	r0, r7
    4d34: 4479         	add	r1, pc => raw=0xdcec ghidra=0x1dcec file=56556 "Driver error while executing the command"
    4d36: f7ff ff83    	bl	0x4c40 <main+0x764>     @ imm = #-0xfa
    4d3a: 462a         	mov	r2, r5
    4d3c: 4631         	mov	r1, r6
    4d3e: 4638         	mov	r0, r7
    4d40: f104 077c    	add.w	r7, r4, #0x7c
    4d44: f7ff e9c8    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xc70
    4d48: 492d         	ldr	r1, [pc, #0xb4]         @ 0x4e00 <main+0x924> literal=0x8fc8
    4d4a: 4638         	mov	r0, r7
    4d4c: 4479         	add	r1, pc => raw=0xdd18 ghidra=0x1dd18 file=56600 "Audio not connected"
    4d4e: f7ff ff77    	bl	0x4c40 <main+0x764>     @ imm = #-0x112
    4d52: 462a         	mov	r2, r5
    4d54: 4631         	mov	r1, r6
    4d56: 4638         	mov	r0, r7
    4d58: f104 0794    	add.w	r7, r4, #0x94
    4d5c: f7ff e9bc    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xc88
    4d60: 4928         	ldr	r1, [pc, #0xa0]         @ 0x4e04 <main+0x928> literal=0x8fc4
    4d62: 4638         	mov	r0, r7
    4d64: 4479         	add	r1, pc => raw=0xdd2c ghidra=0x1dd2c file=56620 "soundOutput not implemented"
    4d66: f7ff ff6b    	bl	0x4c40 <main+0x764>     @ imm = #-0x12a
    4d6a: 462a         	mov	r2, r5
    4d6c: 4631         	mov	r1, r6
    4d6e: 4638         	mov	r0, r7
    4d70: f104 07ac    	add.w	r7, r4, #0xac
    4d74: f7ff e9b0    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xca0
    4d78: 4923         	ldr	r1, [pc, #0x8c]         @ 0x4e08 <main+0x92c> literal=0x8fc8
    4d7a: 4638         	mov	r0, r7
    4d7c: 4479         	add	r1, pc => raw=0xdd48 ghidra=0x1dd48 file=56648 "Volume out of range"
    4d7e: f7ff ff5f    	bl	0x4c40 <main+0x764>     @ imm = #-0x142
    4d82: 462a         	mov	r2, r5
    4d84: 4631         	mov	r1, r6
    4d86: 4638         	mov	r0, r7
    4d88: f104 07c4    	add.w	r7, r4, #0xc4
    4d8c: f7ff e9a4    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xcb8
    4d90: 491e         	ldr	r1, [pc, #0x78]         @ 0x4e0c <main+0x930> literal=0x8fc4
    4d92: 4638         	mov	r0, r7
    4d94: 4479         	add	r1, pc => raw=0xdd5c ghidra=0x1dd5c file=56668 "Volume already at max/min"
    4d96: f7ff ff53    	bl	0x4c40 <main+0x764>     @ imm = #-0x15a
    4d9a: 462a         	mov	r2, r5
    4d9c: 4631         	mov	r1, r6
    4d9e: 4638         	mov	r0, r7
    4da0: f104 07dc    	add.w	r7, r4, #0xdc
    4da4: f7ff e998    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xcd0
    4da8: 4919         	ldr	r1, [pc, #0x64]         @ 0x4e10 <main+0x934> literal=0x8fc6
    4daa: 4638         	mov	r0, r7
    4dac: 34f4         	adds	r4, #0xf4
    4dae: 4479         	add	r1, pc => raw=0xdd78 ghidra=0x1dd78 file=56696 "Connection not possible"
    4db0: f7ff ff46    	bl	0x4c40 <main+0x764>     @ imm = #-0x174
    4db4: 462a         	mov	r2, r5
    4db6: 4631         	mov	r1, r6
    4db8: 4638         	mov	r0, r7
    4dba: f7ff e98e    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xce4
    4dbe: 4915         	ldr	r1, [pc, #0x54]         @ 0x4e14 <main+0x938> literal=0x8fca
    4dc0: 4620         	mov	r0, r4
    4dc2: 4479         	add	r1, pc => raw=0xdd90 ghidra=0x1dd90 file=56720 "Volume control is not supported"
    4dc4: f7ff ff3c    	bl	0x4c40 <main+0x764>     @ imm = #-0x188
    4dc8: 462a         	mov	r2, r5
    4dca: 4631         	mov	r1, r6
    4dcc: 4620         	mov	r0, r4
    4dce: e8bd 47f0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    4dd2: f7ff b97f    	b.w	0x40d4 <g_io_channel_unix_new@plt+0xc> @ imm = #-0xd02
    4dd6: bf00         	nop
    4dd8: c5d0         	stm	r5!, {r4, r6, r7}
    4dda: 0001         	movs	r1, r0
    4ddc: c0fc         	stm	r0!, {r2, r3, r4, r5, r6, r7}
    4dde: 0001         	movs	r1, r0
    4de0: c342         	stm	r3!, {r1, r6}
    4de2: 0001         	movs	r1, r0
    4de4: 0214         	lsls	r4, r2, #0x8
    4de6: 0000         	movs	r0, r0
    4de8: 8fb4         	ldrh	r4, [r6, #0x3c]
    4dea: 0000         	movs	r0, r0
    4dec: 0220         	lsls	r0, r4, #0x8
    4dee: 0000         	movs	r0, r0
    4df0: 8fa8         	ldrh	r0, [r5, #0x3c]
    4df2: 0000         	movs	r0, r0
    4df4: 8fb8         	ldrh	r0, [r7, #0x3c]
    4df6: 0000         	movs	r0, r0
    4df8: 8fb8         	ldrh	r0, [r7, #0x3c]
    4dfa: 0000         	movs	r0, r0
    4dfc: 8fb4         	ldrh	r4, [r6, #0x3c]
    4dfe: 0000         	movs	r0, r0
    4e00: 8fc8         	ldrh	r0, [r1, #0x3e]
    4e02: 0000         	movs	r0, r0
    4e04: 8fc4         	ldrh	r4, [r0, #0x3e]
    4e06: 0000         	movs	r0, r0
    4e08: 8fc8         	ldrh	r0, [r1, #0x3e]
    4e0a: 0000         	movs	r0, r0
    4e0c: 8fc4         	ldrh	r4, [r0, #0x3e]
    4e0e: 0000         	movs	r0, r0
    4e10: 8fc6         	ldrh	r6, [r0, #0x3e]
    4e12: 0000         	movs	r0, r0
    4e14: 8fca         	ldrh	r2, [r1, #0x3e]
    4e16: 0000         	movs	r0, r0
    4e18: b5f0         	push	{r4, r5, r6, r7, lr}
    4e1a: f100 0708    	add.w	r7, r0, #0x8
    4e1e: b083         	sub	sp, #0xc
    4e20: 6007         	str	r7, [r0]
    4e22: b331         	cbz	r1, 0x4e72 <main+0x996> @ imm = #0x4c
    4e24: 4604         	mov	r4, r0
    4e26: 4608         	mov	r0, r1
    4e28: 460e         	mov	r6, r1
    4e2a: f7ff e93c    	blx	0x40a4 <strlen@plt>     @ imm = #-0xd88
    4e2e: 280f         	cmp	r0, #0xf
    4e30: 4605         	mov	r5, r0
    4e32: 9001         	str	r0, [sp, #0x4]
    4e34: d80e         	bhi	0x4e54 <main+0x978>     @ imm = #0x1c
    4e36: 2801         	cmp	r0, #0x1
    4e38: bf04         	itt	eq
    4e3a: 7833         	ldrbeq	r3, [r6]
    4e3c: 7223         	strbeq	r3, [r4, #0x8]
    4e3e: d106         	bne	0x4e4e <main+0x972>     @ imm = #0xc
    4e40: 9b01         	ldr	r3, [sp, #0x4]
    4e42: 2100         	movs	r1, #0x0
    4e44: 6822         	ldr	r2, [r4]
    4e46: 6063         	str	r3, [r4, #0x4]
    4e48: 54d1         	strb	r1, [r2, r3]
    4e4a: b003         	add	sp, #0xc
    4e4c: bdf0         	pop	{r4, r5, r6, r7, pc}
    4e4e: 2800         	cmp	r0, #0x0
    4e50: d0f6         	beq	0x4e40 <main+0x964>     @ imm = #-0x14
    4e52: e008         	b	0x4e66 <main+0x98a>     @ imm = #0x10
    4e54: 2200         	movs	r2, #0x0
    4e56: a901         	add	r1, sp, #0x4
    4e58: 4620         	mov	r0, r4
    4e5a: f7ff e90c    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0xde8
    4e5e: 9b01         	ldr	r3, [sp, #0x4]
    4e60: 4607         	mov	r7, r0
    4e62: 6020         	str	r0, [r4]
    4e64: 60a3         	str	r3, [r4, #0x8]
    4e66: 462a         	mov	r2, r5
    4e68: 4631         	mov	r1, r6
    4e6a: 4638         	mov	r0, r7
    4e6c: f7ff e88c    	blx	0x3f88 <memcpy@plt>     @ imm = #-0xee8
    4e70: e7e6         	b	0x4e40 <main+0x964>     @ imm = #-0x34
    4e72: 4802         	ldr	r0, [pc, #0x8]          @ 0x4e7c <main+0x9a0> literal=0x8820
    4e74: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    4e76: f7fe ef46    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x1174
    4e7a: bf00         	nop
    4e7c: 8820         	ldrh	r0, [r4]
    4e7e: 0000         	movs	r0, r0
    4e80: e92d 47f0    	push.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    4e84: 4c4a         	ldr	r4, [pc, #0x128]        @ 0x4fb0 <main+0xad4> literal=0x1c504
    4e86: 4e4b         	ldr	r6, [pc, #0x12c]        @ 0x4fb4 <main+0xad8> literal=0x1bf24
    4e88: 447c         	add	r4, pc => raw=0x21390 ghidra=0x31390 file=null
    4e8a: 4d4b         	ldr	r5, [pc, #0x12c]        @ 0x4fb8 <main+0xadc> literal=0x1c16a
    4e8c: 4620         	mov	r0, r4
    4e8e: 1d27         	adds	r7, r4, #0x4
    4e90: f7ff e988    	blx	0x41a4 <_ZNSt8ios_base4InitC1Ev@plt> @ imm = #-0xcf0
    4e94: 4b49         	ldr	r3, [pc, #0x124]        @ 0x4fbc <main+0xae0> literal=0x214
    4e96: 447d         	add	r5, pc => raw=0x21004 ghidra=0x31004 file=69636
    4e98: 447e         	add	r6, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    4e9a: 462a         	mov	r2, r5
    4e9c: 4620         	mov	r0, r4
    4e9e: f104 0a1c    	add.w	r10, r4, #0x1c
    4ea2: 58f1         	ldr	r1, [r6, r3]
    4ea4: f104 0934    	add.w	r9, r4, #0x34
    4ea8: f7ff e916    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xdd4
    4eac: 4944         	ldr	r1, [pc, #0x110]        @ 0x4fc0 <main+0xae4> literal=0x8ddc
    4eae: 4638         	mov	r0, r7
    4eb0: f104 084c    	add.w	r8, r4, #0x4c
    4eb4: 4479         	add	r1, pc => raw=0xdc94 ghidra=0x1dc94 file=56468 "Unknown error"
    4eb6: f7ff ffaf    	bl	0x4e18 <main+0x93c>     @ imm = #-0xa2
    4eba: 4b42         	ldr	r3, [pc, #0x108]        @ 0x4fc4 <main+0xae8> literal=0x220
    4ebc: 462a         	mov	r2, r5
    4ebe: 4638         	mov	r0, r7
    4ec0: f104 0764    	add.w	r7, r4, #0x64
    4ec4: 58f6         	ldr	r6, [r6, r3]
    4ec6: 4631         	mov	r1, r6
    4ec8: f7ff e906    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xdf4
    4ecc: 493e         	ldr	r1, [pc, #0xf8]         @ 0x4fc8 <main+0xaec> literal=0x8dd0
    4ece: 4650         	mov	r0, r10
    4ed0: 4479         	add	r1, pc => raw=0xdca4 ghidra=0x1dca4 file=56484 "Failed to validate against schema"
    4ed2: f7ff ffa1    	bl	0x4e18 <main+0x93c>     @ imm = #-0xbe
    4ed6: 462a         	mov	r2, r5
    4ed8: 4631         	mov	r1, r6
    4eda: 4650         	mov	r0, r10
    4edc: f7ff e8fc    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xe08
    4ee0: 493a         	ldr	r1, [pc, #0xe8]         @ 0x4fcc <main+0xaf0> literal=0x8de0
    4ee2: 4648         	mov	r0, r9
    4ee4: 4479         	add	r1, pc => raw=0xdcc8 ghidra=0x1dcc8 file=56520 "Invalid parameters"
    4ee6: f7ff ff97    	bl	0x4e18 <main+0x93c>     @ imm = #-0xd2
    4eea: 462a         	mov	r2, r5
    4eec: 4631         	mov	r1, r6
    4eee: 4648         	mov	r0, r9
    4ef0: f7ff e8f2    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xe1c
    4ef4: 4936         	ldr	r1, [pc, #0xd8]         @ 0x4fd0 <main+0xaf4> literal=0x8de0
    4ef6: 4640         	mov	r0, r8
    4ef8: 4479         	add	r1, pc => raw=0xdcdc ghidra=0x1dcdc file=56540 "Not implemented"
    4efa: f7ff ff8d    	bl	0x4e18 <main+0x93c>     @ imm = #-0xe6
    4efe: 462a         	mov	r2, r5
    4f00: 4631         	mov	r1, r6
    4f02: 4640         	mov	r0, r8
    4f04: f7ff e8e8    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xe30
    4f08: 4932         	ldr	r1, [pc, #0xc8]         @ 0x4fd4 <main+0xaf8> literal=0x8ddc
    4f0a: 4638         	mov	r0, r7
    4f0c: 4479         	add	r1, pc => raw=0xdcec ghidra=0x1dcec file=56556 "Driver error while executing the command"
    4f0e: f7ff ff83    	bl	0x4e18 <main+0x93c>     @ imm = #-0xfa
    4f12: 462a         	mov	r2, r5
    4f14: 4631         	mov	r1, r6
    4f16: 4638         	mov	r0, r7
    4f18: f104 077c    	add.w	r7, r4, #0x7c
    4f1c: f7ff e8dc    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xe48
    4f20: 492d         	ldr	r1, [pc, #0xb4]         @ 0x4fd8 <main+0xafc> literal=0x8df0
    4f22: 4638         	mov	r0, r7
    4f24: 4479         	add	r1, pc => raw=0xdd18 ghidra=0x1dd18 file=56600 "Audio not connected"
    4f26: f7ff ff77    	bl	0x4e18 <main+0x93c>     @ imm = #-0x112
    4f2a: 462a         	mov	r2, r5
    4f2c: 4631         	mov	r1, r6
    4f2e: 4638         	mov	r0, r7
    4f30: f104 0794    	add.w	r7, r4, #0x94
    4f34: f7ff e8d0    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xe60
    4f38: 4928         	ldr	r1, [pc, #0xa0]         @ 0x4fdc <main+0xb00> literal=0x8dec
    4f3a: 4638         	mov	r0, r7
    4f3c: 4479         	add	r1, pc => raw=0xdd2c ghidra=0x1dd2c file=56620 "soundOutput not implemented"
    4f3e: f7ff ff6b    	bl	0x4e18 <main+0x93c>     @ imm = #-0x12a
    4f42: 462a         	mov	r2, r5
    4f44: 4631         	mov	r1, r6
    4f46: 4638         	mov	r0, r7
    4f48: f104 07ac    	add.w	r7, r4, #0xac
    4f4c: f7ff e8c4    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xe78
    4f50: 4923         	ldr	r1, [pc, #0x8c]         @ 0x4fe0 <main+0xb04> literal=0x8df0
    4f52: 4638         	mov	r0, r7
    4f54: 4479         	add	r1, pc => raw=0xdd48 ghidra=0x1dd48 file=56648 "Volume out of range"
    4f56: f7ff ff5f    	bl	0x4e18 <main+0x93c>     @ imm = #-0x142
    4f5a: 462a         	mov	r2, r5
    4f5c: 4631         	mov	r1, r6
    4f5e: 4638         	mov	r0, r7
    4f60: f104 07c4    	add.w	r7, r4, #0xc4
    4f64: f7ff e8b8    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xe90
    4f68: 491e         	ldr	r1, [pc, #0x78]         @ 0x4fe4 <main+0xb08> literal=0x8dec
    4f6a: 4638         	mov	r0, r7
    4f6c: 4479         	add	r1, pc => raw=0xdd5c ghidra=0x1dd5c file=56668 "Volume already at max/min"
    4f6e: f7ff ff53    	bl	0x4e18 <main+0x93c>     @ imm = #-0x15a
    4f72: 462a         	mov	r2, r5
    4f74: 4631         	mov	r1, r6
    4f76: 4638         	mov	r0, r7
    4f78: f104 07dc    	add.w	r7, r4, #0xdc
    4f7c: f7ff e8ac    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xea8
    4f80: 4919         	ldr	r1, [pc, #0x64]         @ 0x4fe8 <main+0xb0c> literal=0x8dee
    4f82: 4638         	mov	r0, r7
    4f84: 34f4         	adds	r4, #0xf4
    4f86: 4479         	add	r1, pc => raw=0xdd78 ghidra=0x1dd78 file=56696 "Connection not possible"
    4f88: f7ff ff46    	bl	0x4e18 <main+0x93c>     @ imm = #-0x174
    4f8c: 462a         	mov	r2, r5
    4f8e: 4631         	mov	r1, r6
    4f90: 4638         	mov	r0, r7
    4f92: f7ff e8a2    	blx	0x40d8 <__aeabi_atexit@plt> @ imm = #-0xebc
    4f96: 4915         	ldr	r1, [pc, #0x54]         @ 0x4fec <main+0xb10> literal=0x8df2
    4f98: 4620         	mov	r0, r4
    4f9a: 4479         	add	r1, pc => raw=0xdd90 ghidra=0x1dd90 file=56720 "Volume control is not supported"
    4f9c: f7ff ff3c    	bl	0x4e18 <main+0x93c>     @ imm = #-0x188
    4fa0: 462a         	mov	r2, r5
    4fa2: 4631         	mov	r1, r6
    4fa4: 4620         	mov	r0, r4
    4fa6: e8bd 47f0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    4faa: f7ff b893    	b.w	0x40d4 <g_io_channel_unix_new@plt+0xc> @ imm = #-0xeda
    4fae: bf00         	nop
    4fb0: c504         	stm	r5!, {r2}
    4fb2: 0001         	movs	r1, r0
    4fb4: bf24         	itt	hs
    4fb6: 0001         	movs	r1, r0
    4fb8: c16a         	stmhs	r1!, {r1, r3, r5, r6}
    4fba: 0001         	movs	r1, r0
    4fbc: 0214         	lsls	r4, r2, #0x8
    4fbe: 0000         	movs	r0, r0
    4fc0: 8ddc         	ldrh	r4, [r3, #0x2e]
    4fc2: 0000         	movs	r0, r0
    4fc4: 0220         	lsls	r0, r4, #0x8
    4fc6: 0000         	movs	r0, r0
    4fc8: 8dd0         	ldrh	r0, [r2, #0x2e]
    4fca: 0000         	movs	r0, r0
    4fcc: 8de0         	ldrh	r0, [r4, #0x2e]
    4fce: 0000         	movs	r0, r0
    4fd0: 8de0         	ldrh	r0, [r4, #0x2e]
    4fd2: 0000         	movs	r0, r0
    4fd4: 8ddc         	ldrh	r4, [r3, #0x2e]
    4fd6: 0000         	movs	r0, r0
    4fd8: 8df0         	ldrh	r0, [r6, #0x2e]
    4fda: 0000         	movs	r0, r0
    4fdc: 8dec         	ldrh	r4, [r5, #0x2e]
    4fde: 0000         	movs	r0, r0
    4fe0: 8df0         	ldrh	r0, [r6, #0x2e]
    4fe2: 0000         	movs	r0, r0
    4fe4: 8dec         	ldrh	r4, [r5, #0x2e]
    4fe6: 0000         	movs	r0, r0
    4fe8: 8dee         	ldrh	r6, [r5, #0x2e]
    4fea: 0000         	movs	r0, r0
    4fec: 8df2         	ldrh	r2, [r6, #0x2e]
    4fee: 0000         	movs	r0, r0

00004ff0 <_start>:
    4ff0: f04f 0b00    	mov.w	r11, #0x0
    4ff4: f04f 0e00    	mov.w	lr, #0x0
    4ff8: bc02         	pop	{r1}
    4ffa: 466a         	mov	r2, sp
    4ffc: b404         	push	{r2}
    4ffe: b401         	push	{r0}
    5000: f8df a018    	ldr.w	r10, [pc, #0x18]        @ 0x501c <_start+0x2c> literal=0x1bda4
    5004: a305         	adr	r3, #20 <_start+0x1d>
    5006: 449a         	add	r10, r3
    5008: f04f 0300    	mov.w	r3, #0x0
    500c: b408         	push	{r3}
    500e: 4804         	ldr	r0, [pc, #0x10]         @ 0x5020 <_start+0x30> literal=0x1f8
    5010: f85a 0000    	ldr.w	r0, [r10, r0]
    5014: f7ff e938    	blx	0x4288 <__libc_start_main@plt> @ imm = #-0xd90
    5018: f7ff e8d0    	blx	0x41bc <abort@plt>      @ imm = #-0xe60
    501c: bda4         	pop	{r2, r5, r7, pc}
    501e: 0001         	movs	r1, r0
    5020: 01f8         	lsls	r0, r7, #0x7
    5022: 0000         	movs	r0, r0
    5024: 3014         	adds	r0, #0x14
    5026: e59f         	b	0x4b68 <main+0x68c>     @ imm = #-0x4c2
    5028: 2014         	movs	r0, #0x14
    502a: e59f         	b	0x4b6c <main+0x690>     @ imm = #-0x4c2
    502c: 3003         	adds	r0, #0x3
    502e: e08f         	b	0x5150 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x64> @ imm = #0x11e
    5030: 2002         	movs	r0, #0x2
    5032: e793         	b	0x4f5c <main+0xa80>     @ imm = #-0xda
    5034: 0000         	movs	r0, r0
    5036: e352         	b	0x56de <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0xda> @ imm = #0x6a4
    5038: ff1e 012f    	vrhadd.u16	d0, d14, d31
    503c: fb96 eaff    	<unknown>
    5040: bd8c         	pop	{r2, r3, r7, pc}
    5042: 0001         	movs	r1, r0
    5044: 0204         	lsls	r4, r0, #0x8
    5046: 0000         	movs	r0, r0
    5048: 4806         	ldr	r0, [pc, #0x18]         @ 0x5064 <_start+0x74> literal=0x1bfee
    504a: 4b07         	ldr	r3, [pc, #0x1c]         @ 0x5068 <_start+0x78> literal=0x1bfec
    504c: 4a07         	ldr	r2, [pc, #0x1c]         @ 0x506c <_start+0x7c> literal=0x1bd68
    504e: 4478         	add	r0, pc => raw=0x21040 ghidra=0x31040 file=null
    5050: 447b         	add	r3, pc => raw=0x21040 ghidra=0x31040 file=null
    5052: 4283         	cmp	r3, r0
    5054: 447a         	add	r2, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    5056: d003         	beq	0x5060 <_start+0x70>    @ imm = #0x6
    5058: 4b05         	ldr	r3, [pc, #0x14]         @ 0x5070 <_start+0x80> literal=0x1fc
    505a: 58d3         	ldr	r3, [r2, r3]
    505c: b103         	cbz	r3, 0x5060 <_start+0x70> @ imm = #0x0
    505e: 4718         	bx	r3
    5060: 4770         	bx	lr
    5062: bf00         	nop
    5064: bfee         	itee	al                      @ unpredictable IT predicate sequence
    5066: 0001         	movs	r1, r0
    5068: bfec         	ite	al                      @ unpredictable IT predicate sequence
    506a: 0001         	movs	r1, r0
    506c: bd68         	pop<und>	{r3, r5, r6, pc}
    506e: 0001         	movs	r1, r0
    5070: 01fc         	lsls	r4, r7, #0x7
    5072: 0000         	movs	r0, r0
    5074: 4808         	ldr	r0, [pc, #0x20]         @ 0x5098 <_start+0xa8> literal=0x1bfc2
    5076: 4b09         	ldr	r3, [pc, #0x24]         @ 0x509c <_start+0xac> literal=0x1bfc0
    5078: 4a09         	ldr	r2, [pc, #0x24]         @ 0x50a0 <_start+0xb0> literal=0x1bd3c
    507a: 4478         	add	r0, pc => raw=0x21040 ghidra=0x31040 file=null
    507c: 447b         	add	r3, pc => raw=0x21040 ghidra=0x31040 file=null
    507e: 1a1b         	subs	r3, r3, r0
    5080: 447a         	add	r2, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    5082: 0fd9         	lsrs	r1, r3, #0x1f
    5084: eb01 01a3    	add.w	r1, r1, r3, asr #2
    5088: 1049         	asrs	r1, r1, #0x1
    508a: d003         	beq	0x5094 <_start+0xa4>    @ imm = #0x6
    508c: 4b05         	ldr	r3, [pc, #0x14]         @ 0x50a4 <_start+0xb4> literal=0x238
    508e: 58d3         	ldr	r3, [r2, r3]
    5090: b103         	cbz	r3, 0x5094 <_start+0xa4> @ imm = #0x0
    5092: 4718         	bx	r3
    5094: 4770         	bx	lr
    5096: bf00         	nop
    5098: bfc2         	ittt	gt
    509a: 0001         	movs	r1, r0
    509c: bfc0         	hintgt	#0xc
    509e: 0001         	movs	r1, r0
    50a0: bd3c         	pop	{r2, r3, r4, r5, pc}
    50a2: 0001         	movs	r1, r0
    50a4: 0238         	lsls	r0, r7, #0x8
    50a6: 0000         	movs	r0, r0
    50a8: b508         	push	{r3, lr}
    50aa: 4b0a         	ldr	r3, [pc, #0x28]         @ 0x50d4 <_start+0xe4> literal=0x1bf8e
    50ac: 4a0a         	ldr	r2, [pc, #0x28]         @ 0x50d8 <_start+0xe8> literal=0x1bd0a
    50ae: 447b         	add	r3, pc => raw=0x21040 ghidra=0x31040 file=null
    50b0: 781b         	ldrb	r3, [r3]
    50b2: 447a         	add	r2, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    50b4: b96b         	cbnz	r3, 0x50d2 <_start+0xe2> @ imm = #0x1a
    50b6: 4b09         	ldr	r3, [pc, #0x24]         @ 0x50dc <_start+0xec> literal=0x218
    50b8: 58d3         	ldr	r3, [r2, r3]
    50ba: b123         	cbz	r3, 0x50c6 <_start+0xd6> @ imm = #0x8
    50bc: 4b08         	ldr	r3, [pc, #0x20]         @ 0x50e0 <_start+0xf0> literal=0x1bf42
    50be: 447b         	add	r3, pc => raw=0x21004 ghidra=0x31004 file=69636
    50c0: 6818         	ldr	r0, [r3]
    50c2: f7fe efb8    	blx	0x4034 <__cxa_finalize@plt> @ imm = #-0x1090
    50c6: f7ff ffbf    	bl	0x5048 <_start+0x58>    @ imm = #-0x82
    50ca: 4b06         	ldr	r3, [pc, #0x18]         @ 0x50e4 <_start+0xf4> literal=0x1bf6e
    50cc: 2201         	movs	r2, #0x1
    50ce: 447b         	add	r3, pc => raw=0x21040 ghidra=0x31040 file=null
    50d0: 701a         	strb	r2, [r3]
    50d2: bd08         	pop	{r3, pc}
    50d4: bf8e         	itee	hi
    50d6: 0001         	movs	r1, r0
    50d8: bd0a         	popls	{r1, r3, pc}
    50da: 0001         	movs	r1, r0
    50dc: 0218         	lsls	r0, r3, #0x8
    50de: 0000         	movs	r0, r0
    50e0: bf42         	ittt	mi
    50e2: 0001         	movs	r1, r0
    50e4: bf6e         	itee	vs
    50e6: 0001         	movs	r1, r0
    50e8: e7c4         	bvc	0x5074 <_start+0x84>    @ imm = #-0x78
    50ea: bf00         	nopvc

000050ec <ConnectionManager::_headphoneStatusUpdateCallBack(void*)>:
    50ec: e92d 4ff0    	pushmi.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    50f0: b0b3         	sub	sp, #0xcc
    50f2: 2300         	movs	r3, #0x0
    50f4: aa22         	add	r2, sp, #0x88
    50f6: a928         	add	r1, sp, #0xa0
    50f8: f10d 0b80    	add.w	r11, sp, #0x80
    50fc: f88d 3043    	strb.w	r3, [sp, #0x43]
    5100: e9cd 2320    	strd	r2, r3, [sp, #128]
    5104: f88d 3088    	strb.w	r3, [sp, #0x88]
    5108: e9cd 1326    	strd	r1, r3, [sp, #152]
    510c: f88d 30a0    	strb.w	r3, [sp, #0xa0]
    5110: ab26         	add	r3, sp, #0x98
    5112: 920a         	str	r2, [sp, #0x28]
    5114: 910b         	str	r1, [sp, #0x2c]
    5116: 930c         	str	r3, [sp, #0x30]
    5118: b138         	cbz	r0, 0x512a <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x3e> @ imm = #0xe
    511a: 4604         	mov	r4, r0
    511c: 465a         	mov	r2, r11
    511e: 6c00         	ldr	r0, [r0, #0x40]
    5120: f10d 0143    	add.w	r1, sp, #0x43
    5124: f7fe ee6c    	blx	0x3e00 <_ZN9umiClient18getHeadphoneStatusEPbRNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_@plt> @ imm = #-0x1328
    5128: b360         	cbz	r0, 0x5184 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x98> @ imm = #0x58
    512a: f005 fa07    	bl	0xa53c <getPmLogContext()> @ imm = #0x540e
    512e: b120         	cbz	r0, 0x513a <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4e> @ imm = #0x8
    5130: f005 fa04    	bl	0xa53c <getPmLogContext()> @ imm = #0x5408
    5134: 6803         	ldr	r3, [r0]
    5136: 2b02         	cmp	r3, #0x2
    5138: dd0f         	ble	0x515a <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x6e> @ imm = #0x1e
    513a: f005 f9ff    	bl	0xa53c <getPmLogContext()> @ imm = #0x53fe
    513e: f8df 3494    	ldr.w	r3, [pc, #0x494]        @ 0x55d4 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4e8> literal=0x86e0
    5142: 2200         	movs	r2, #0x0
    5144: 2103         	movs	r1, #0x3
    5146: e9cd 2201    	strd	r2, r2, [sp, #4]
    514a: 9200         	str	r2, [sp]
    514c: 447b         	add	r3, pc => raw=0xd830 ghidra=0x1d830 file=55344 "getHeadphoneStatus return error"
    514e: 9303         	str	r3, [sp, #0xc]
    5150: f8df 3484    	ldr.w	r3, [pc, #0x484]        @ 0x55d8 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4ec> literal=0x86c4
    5154: 447b         	add	r3, pc => raw=0xd81c ghidra=0x1d81c file=55324 "AUDIO_DRIVER_ERROR"
    5156: f7fe ef62    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x113c
    515a: 9d26         	ldr	r5, [sp, #0x98]
    515c: 9b0b         	ldr	r3, [sp, #0x2c]
    515e: 429d         	cmp	r5, r3
    5160: d004         	beq	0x516c <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x80> @ imm = #0x8
    5162: 9928         	ldr	r1, [sp, #0xa0]
    5164: 4628         	mov	r0, r5
    5166: 3101         	adds	r1, #0x1
    5168: f7fe eeca    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x126c
    516c: 9820         	ldr	r0, [sp, #0x80]
    516e: 9b0a         	ldr	r3, [sp, #0x28]
    5170: 4298         	cmp	r0, r3
    5172: d003         	beq	0x517c <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x90> @ imm = #0x6
    5174: 9922         	ldr	r1, [sp, #0x88]
    5176: 3101         	adds	r1, #0x1
    5178: f7fe eec2    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x127c
    517c: 2001         	movs	r0, #0x1
    517e: b033         	add	sp, #0xcc
    5180: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    5184: f89d 6043    	ldrb.w	r6, [sp, #0x43]
    5188: f894 3048    	ldrb.w	r3, [r4, #0x48]
    518c: 42b3         	cmp	r3, r6
    518e: d10d         	bne	0x51ac <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0xc0> @ imm = #0x1a
    5190: 9a21         	ldr	r2, [sp, #0x84]
    5192: 6d23         	ldr	r3, [r4, #0x50]
    5194: 429a         	cmp	r2, r3
    5196: d109         	bne	0x51ac <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0xc0> @ imm = #0x12
    5198: 9820         	ldr	r0, [sp, #0x80]
    519a: 6ce1         	ldr	r1, [r4, #0x4c]
    519c: 2a00         	cmp	r2, #0x0
    519e: f000 81cc    	beq.w	0x553a <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x44e> @ imm = #0x398
    51a2: f7fe efd6    	blx	0x4150 <memcmp@plt>     @ imm = #-0x1054
    51a6: 2800         	cmp	r0, #0x0
    51a8: f000 81c7    	beq.w	0x553a <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x44e> @ imm = #0x38e
    51ac: f104 0a4c    	add.w	r10, r4, #0x4c
    51b0: 4659         	mov	r1, r11
    51b2: 4650         	mov	r0, r10
    51b4: f884 6048    	strb.w	r6, [r4, #0x48]
    51b8: f7ff e82a    	blx	0x4210 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt> @ imm = #-0xfac
    51bc: f104 0364    	add.w	r3, r4, #0x64
    51c0: 990c         	ldr	r1, [sp, #0x30]
    51c2: 4618         	mov	r0, r3
    51c4: 930f         	str	r3, [sp, #0x3c]
    51c6: f7ff e824    	blx	0x4210 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt> @ imm = #-0xfb8
    51ca: ab11         	add	r3, sp, #0x44
    51cc: 4618         	mov	r0, r3
    51ce: 4698         	mov	r8, r3
    51d0: 930e         	str	r3, [sp, #0x38]
    51d2: f7fe edf2    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x141c
    51d6: f89d 1043    	ldrb.w	r1, [sp, #0x43]
    51da: ad1d         	add	r5, sp, #0x74
    51dc: 4628         	mov	r0, r5
    51de: f081 0101    	eor	r1, r1, #0x1
    51e2: f7fe ee08    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x13f0
    51e6: f247 536d    	movw	r3, #0x756d
    51ea: af2c         	add	r7, sp, #0xb0
    51ec: f2c6 5374    	movt	r3, #0x6574
    51f0: ae2e         	add	r6, sp, #0xb8
    51f2: 932e         	str	r3, [sp, #0xb8]
    51f4: 2304         	movs	r3, #0x4
    51f6: 932d         	str	r3, [sp, #0xb4]
    51f8: 462a         	mov	r2, r5
    51fa: 2300         	movs	r3, #0x0
    51fc: 4639         	mov	r1, r7
    51fe: 4640         	mov	r0, r8
    5200: 962c         	str	r6, [sp, #0xb0]
    5202: f88d 30bc    	strb.w	r3, [sp, #0xbc]
    5206: f7fe ed66    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x1534
    520a: 982c         	ldr	r0, [sp, #0xb0]
    520c: 42b0         	cmp	r0, r6
    520e: d003         	beq	0x5218 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x12c> @ imm = #0x6
    5210: 992e         	ldr	r1, [sp, #0xb8]
    5212: 3101         	adds	r1, #0x1
    5214: f7fe ee74    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1318
    5218: 4628         	mov	r0, r5
    521a: f7fe ed9e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x14c4
    521e: 2316         	movs	r3, #0x16
    5220: 2200         	movs	r2, #0x0
    5222: 4629         	mov	r1, r5
    5224: 4638         	mov	r0, r7
    5226: f8d4 8040    	ldr.w	r8, [r4, #0x40]
    522a: 962c         	str	r6, [sp, #0xb0]
    522c: 931d         	str	r3, [sp, #0x74]
    522e: f7fe ef22    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x11bc
    5232: f8df e3a8    	ldr.w	lr, [pc, #0x3a8]        @ 0x55dc <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4f0> literal=0x8480
    5236: 4684         	mov	r12, r0
    5238: 9b1d         	ldr	r3, [sp, #0x74]
    523a: f04f 0900    	mov.w	r9, #0x0
    523e: 902c         	str	r0, [sp, #0xb0]
    5240: 44fe         	add	lr, pc => raw=0xd6c4 ghidra=0x1d6c4 file=54980 "headphoneDetectionMute"
    5242: 932e         	str	r3, [sp, #0xb8]
    5244: e8be 000f    	ldm.w	lr!, {r0, r1, r2, r3}
    5248: f8cc 0000    	str.w	r0, [r12]
    524c: f8cc 300c    	str.w	r3, [r12, #0xc]
    5250: f8de 0000    	ldr.w	r0, [lr]
    5254: f8be 3004    	ldrh.w	r3, [lr, #0x4]
    5258: f8cc 1004    	str.w	r1, [r12, #0x4]
    525c: f8cc 0010    	str.w	r0, [r12, #0x10]
    5260: 4628         	mov	r0, r5
    5262: f8cc 2008    	str.w	r2, [r12, #0x8]
    5266: f8ac 3014    	strh.w	r3, [r12, #0x14]
    526a: 9b1d         	ldr	r3, [sp, #0x74]
    526c: 9a2c         	ldr	r2, [sp, #0xb0]
    526e: 990e         	ldr	r1, [sp, #0x38]
    5270: 932d         	str	r3, [sp, #0xb4]
    5272: f802 9003    	strb.w	r9, [r2, r3]
    5276: f7fe ee2a    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x13ac
    527a: 4640         	mov	r0, r8
    527c: 462a         	mov	r2, r5
    527e: 4639         	mov	r1, r7
    5280: f7fe ee6a    	blx	0x3f58 <_ZN9umiClient11setSettingsENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt> @ imm = #-0x132c
    5284: 4628         	mov	r0, r5
    5286: f7fe ed68    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1530
    528a: 982c         	ldr	r0, [sp, #0xb0]
    528c: 42b0         	cmp	r0, r6
    528e: d003         	beq	0x5298 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x1ac> @ imm = #0x6
    5290: 992e         	ldr	r1, [sp, #0xb8]
    5292: 3101         	adds	r1, #0x1
    5294: f7fe ee34    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1398
    5298: f10d 0850    	add.w	r8, sp, #0x50
    529c: 4640         	mov	r0, r8
    529e: f7fe ed8c    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x14e8
    52a2: ab17         	add	r3, sp, #0x5c
    52a4: 4618         	mov	r0, r3
    52a6: 930d         	str	r3, [sp, #0x34]
    52a8: f7fe ed86    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x14f4
    52ac: f10d 0968    	add.w	r9, sp, #0x68
    52b0: 4648         	mov	r0, r9
    52b2: f7fe ed82    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x14fc
    52b6: f894 1048    	ldrb.w	r1, [r4, #0x48]
    52ba: 4628         	mov	r0, r5
    52bc: f7fe ed9a    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x14cc
    52c0: 4bc7         	ldr	r3, [pc, #0x31c]        @ 0x55e0 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4f4> literal=0x840e
    52c2: 46b4         	mov	r12, r6
    52c4: 962c         	str	r6, [sp, #0xb0]
    52c6: f04f 0e00    	mov.w	lr, #0x0
    52ca: 447b         	add	r3, pc => raw=0xd6dc ghidra=0x1d6dc file=55004 "headPhoneStatus"
    52cc: cb0f         	ldm	r3, {r0, r1, r2, r3}
    52ce: e8ac 0007    	stm.w	r12!, {r0, r1, r2}
    52d2: 462a         	mov	r2, r5
    52d4: f82c 3b02    	strh	r3, [r12], #2
    52d8: 0c1b         	lsrs	r3, r3, #0x10
    52da: 4639         	mov	r1, r7
    52dc: f88c 3000    	strb.w	r3, [r12]
    52e0: 4640         	mov	r0, r8
    52e2: 230f         	movs	r3, #0xf
    52e4: f88d e0c7    	strb.w	lr, [sp, #0xc7]
    52e8: 932d         	str	r3, [sp, #0xb4]
    52ea: f7fe ecf4    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x1618
    52ee: 982c         	ldr	r0, [sp, #0xb0]
    52f0: 42b0         	cmp	r0, r6
    52f2: d003         	beq	0x52fc <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x210> @ imm = #0x6
    52f4: 992e         	ldr	r1, [sp, #0xb8]
    52f6: 3101         	adds	r1, #0x1
    52f8: f7fe ee02    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x13fc
    52fc: 4628         	mov	r0, r5
    52fe: f7fe ed2c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x15a8
    5302: 4651         	mov	r1, r10
    5304: 4628         	mov	r0, r5
    5306: f7fe ef12    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x11dc
    530a: 4bb6         	ldr	r3, [pc, #0x2d8]        @ 0x55e4 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4f8> literal=0x83d0
    530c: 46b4         	mov	r12, r6
    530e: 962c         	str	r6, [sp, #0xb0]
    5310: f04f 0a0d    	mov.w	r10, #0xd
    5314: f04f 0e00    	mov.w	lr, #0x0
    5318: 447b         	add	r3, pc => raw=0xd6ec ghidra=0x1d6ec file=55020 "connectStatus"
    531a: cb0f         	ldm	r3, {r0, r1, r2, r3}
    531c: e8ac 0007    	stm.w	r12!, {r0, r1, r2}
    5320: 462a         	mov	r2, r5
    5322: f88c 3000    	strb.w	r3, [r12]
    5326: 4639         	mov	r1, r7
    5328: 980d         	ldr	r0, [sp, #0x34]
    532a: f8cd a0b4    	str.w	r10, [sp, #0xb4]
    532e: f88d e0c5    	strb.w	lr, [sp, #0xc5]
    5332: f7fe ecd0    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x1660
    5336: 982c         	ldr	r0, [sp, #0xb0]
    5338: 42b0         	cmp	r0, r6
    533a: d003         	beq	0x5344 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x258> @ imm = #0x6
    533c: 992e         	ldr	r1, [sp, #0xb8]
    533e: 3101         	adds	r1, #0x1
    5340: f7fe edde    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1444
    5344: 4628         	mov	r0, r5
    5346: f7fe ed08    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x15f0
    534a: 2312         	movs	r3, #0x12
    534c: 2200         	movs	r2, #0x0
    534e: 4629         	mov	r1, r5
    5350: 4638         	mov	r0, r7
    5352: 962c         	str	r6, [sp, #0xb0]
    5354: 931d         	str	r3, [sp, #0x74]
    5356: f7fe ee8e    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x12e4
    535a: f8df e28c    	ldr.w	lr, [pc, #0x28c]        @ 0x55e8 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4fc> literal=0x8390
    535e: 4684         	mov	r12, r0
    5360: 9b1d         	ldr	r3, [sp, #0x74]
    5362: f04f 0a00    	mov.w	r10, #0x0
    5366: 902c         	str	r0, [sp, #0xb0]
    5368: 44fe         	add	lr, pc => raw=0xd6fc ghidra=0x1d6fc file=55036 "internalMicrophone"
    536a: 932e         	str	r3, [sp, #0xb8]
    536c: e8be 000f    	ldm.w	lr!, {r0, r1, r2, r3}
    5370: f8cc 300c    	str.w	r3, [r12, #0xc]
    5374: f8be 3000    	ldrh.w	r3, [lr]
    5378: f8cc 0000    	str.w	r0, [r12]
    537c: 4640         	mov	r0, r8
    537e: f8cc 2008    	str.w	r2, [r12, #0x8]
    5382: f8cc 1004    	str.w	r1, [r12, #0x4]
    5386: f8ac 3010    	strh.w	r3, [r12, #0x10]
    538a: 9b1d         	ldr	r3, [sp, #0x74]
    538c: 992c         	ldr	r1, [sp, #0xb0]
    538e: 9a0d         	ldr	r2, [sp, #0x34]
    5390: 932d         	str	r3, [sp, #0xb4]
    5392: f801 a003    	strb.w	r10, [r1, r3]
    5396: 4639         	mov	r1, r7
    5398: f7fe ec9c    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x16c8
    539c: 982c         	ldr	r0, [sp, #0xb0]
    539e: 42b0         	cmp	r0, r6
    53a0: d003         	beq	0x53aa <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x2be> @ imm = #0x6
    53a2: 992e         	ldr	r1, [sp, #0xb8]
    53a4: 3101         	adds	r1, #0x1
    53a6: f7fe edac    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x14a8
    53aa: 990f         	ldr	r1, [sp, #0x3c]
    53ac: 4628         	mov	r0, r5
    53ae: f7fe eebe    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x1284
    53b2: 4b8e         	ldr	r3, [pc, #0x238]        @ 0x55ec <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x500> literal=0x8328
    53b4: 46b4         	mov	r12, r6
    53b6: 962c         	str	r6, [sp, #0xb0]
    53b8: f04f 0a0d    	mov.w	r10, #0xd
    53bc: f04f 0e00    	mov.w	lr, #0x0
    53c0: 447b         	add	r3, pc => raw=0xd6ec ghidra=0x1d6ec file=55020 "connectStatus"
    53c2: cb0f         	ldm	r3, {r0, r1, r2, r3}
    53c4: e8ac 0007    	stm.w	r12!, {r0, r1, r2}
    53c8: 462a         	mov	r2, r5
    53ca: f88c 3000    	strb.w	r3, [r12]
    53ce: 4639         	mov	r1, r7
    53d0: 4648         	mov	r0, r9
    53d2: f8cd a0b4    	str.w	r10, [sp, #0xb4]
    53d6: f88d e0c5    	strb.w	lr, [sp, #0xc5]
    53da: f7fe ec7c    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x1708
    53de: 982c         	ldr	r0, [sp, #0xb0]
    53e0: 42b0         	cmp	r0, r6
    53e2: d003         	beq	0x53ec <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x300> @ imm = #0x6
    53e4: 992e         	ldr	r1, [sp, #0xb8]
    53e6: 3101         	adds	r1, #0x1
    53e8: f7fe ed8a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x14ec
    53ec: 4628         	mov	r0, r5
    53ee: f7fe ecb4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1698
    53f2: 2312         	movs	r3, #0x12
    53f4: 2200         	movs	r2, #0x0
    53f6: 4629         	mov	r1, r5
    53f8: 4638         	mov	r0, r7
    53fa: 962c         	str	r6, [sp, #0xb0]
    53fc: 931d         	str	r3, [sp, #0x74]
    53fe: f7fe ee3a    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x138c
    5402: f8df e1ec    	ldr.w	lr, [pc, #0x1ec]        @ 0x55f0 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x504> literal=0x82fc
    5406: 4684         	mov	r12, r0
    5408: 9b1d         	ldr	r3, [sp, #0x74]
    540a: f04f 0a00    	mov.w	r10, #0x0
    540e: 902c         	str	r0, [sp, #0xb0]
    5410: 44fe         	add	lr, pc => raw=0xd710 ghidra=0x1d710 file=55056 "externalMicrophone"
    5412: 932e         	str	r3, [sp, #0xb8]
    5414: e8be 000f    	ldm.w	lr!, {r0, r1, r2, r3}
    5418: f8cc 300c    	str.w	r3, [r12, #0xc]
    541c: f8be 3000    	ldrh.w	r3, [lr]
    5420: f8cc 0000    	str.w	r0, [r12]
    5424: 4640         	mov	r0, r8
    5426: f8cc 2008    	str.w	r2, [r12, #0x8]
    542a: 464a         	mov	r2, r9
    542c: f8cc 1004    	str.w	r1, [r12, #0x4]
    5430: f8ac 3010    	strh.w	r3, [r12, #0x10]
    5434: 9b1d         	ldr	r3, [sp, #0x74]
    5436: 992c         	ldr	r1, [sp, #0xb0]
    5438: 932d         	str	r3, [sp, #0xb4]
    543a: f801 a003    	strb.w	r10, [r1, r3]
    543e: 4639         	mov	r1, r7
    5440: f7fe ec48    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x1770
    5444: 982c         	ldr	r0, [sp, #0xb0]
    5446: 42b0         	cmp	r0, r6
    5448: d003         	beq	0x5452 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x366> @ imm = #0x6
    544a: 992e         	ldr	r1, [sp, #0xb8]
    544c: 3101         	adds	r1, #0x1
    544e: f7fe ed58    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1550
    5452: 2101         	movs	r1, #0x1
    5454: 4628         	mov	r0, r5
    5456: f7fe ecce    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x1664
    545a: 4a66         	ldr	r2, [pc, #0x198]        @ 0x55f4 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x508> literal=0x82b8
    545c: 4633         	mov	r3, r6
    545e: 962c         	str	r6, [sp, #0xb0]
    5460: f04f 0e0b    	mov.w	lr, #0xb
    5464: f04f 0c00    	mov.w	r12, #0x0
    5468: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    546a: ca07         	ldm	r2, {r0, r1, r2}
    546c: c303         	stm	r3!, {r0, r1}
    546e: 4639         	mov	r1, r7
    5470: f823 2b02    	strh	r2, [r3], #2
    5474: 0c12         	lsrs	r2, r2, #0x10
    5476: 4640         	mov	r0, r8
    5478: 701a         	strb	r2, [r3]
    547a: 462a         	mov	r2, r5
    547c: f8cd e0b4    	str.w	lr, [sp, #0xb4]
    5480: f88d c0c3    	strb.w	r12, [sp, #0xc3]
    5484: f7fe ec26    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x17b4
    5488: 982c         	ldr	r0, [sp, #0xb0]
    548a: 42b0         	cmp	r0, r6
    548c: d003         	beq	0x5496 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x3aa> @ imm = #0x6
    548e: 992e         	ldr	r1, [sp, #0xb8]
    5490: 3101         	adds	r1, #0x1
    5492: f7fe ed36    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1594
    5496: 4628         	mov	r0, r5
    5498: f7fe ec5e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1744
    549c: f005 f84e    	bl	0xa53c <getPmLogContext()> @ imm = #0x509c
    54a0: b380         	cbz	r0, 0x5504 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x418> @ imm = #0x60
    54a2: f005 f84b    	bl	0xa53c <getPmLogContext()> @ imm = #0x5096
    54a6: 6803         	ldr	r3, [r0]
    54a8: 2b06         	cmp	r3, #0x6
    54aa: dc2b         	bgt	0x5504 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x418> @ imm = #0x56
    54ac: 2100         	movs	r1, #0x0
    54ae: 4628         	mov	r0, r5
    54b0: 962c         	str	r6, [sp, #0xb0]
    54b2: 912d         	str	r1, [sp, #0xb4]
    54b4: f88d 10b8    	strb.w	r1, [sp, #0xb8]
    54b8: f7fe edce    	blx	0x4058 <_ZN7pbnjson10JGeneratorC1EPNS_9JResolverE@plt> @ imm = #-0x1464
    54bc: f7fe edc0    	blx	0x4040 <_ZN7pbnjson7JSchema9AllSchemaEv@plt> @ imm = #-0x1480
    54c0: 4602         	mov	r2, r0
    54c2: 463b         	mov	r3, r7
    54c4: 4641         	mov	r1, r8
    54c6: 4628         	mov	r0, r5
    54c8: f7fe ede6    	blx	0x4098 <_ZN7pbnjson10JGenerator8toStringERKNS_6JValueERKNS_7JSchemaERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x1434
    54cc: 4628         	mov	r0, r5
    54ce: f7fe edac    	blx	0x4028 <_ZN7pbnjson10JGeneratorD1Ev@plt> @ imm = #-0x14a8
    54d2: 992c         	ldr	r1, [sp, #0xb0]
    54d4: 4620         	mov	r0, r4
    54d6: f7fe ed9c    	blx	0x4010 <_ZN2LS17SubscriptionPoint4postEPKc@plt> @ imm = #-0x14c8
    54da: 982c         	ldr	r0, [sp, #0xb0]
    54dc: 42b0         	cmp	r0, r6
    54de: d003         	beq	0x54e8 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x3fc> @ imm = #0x6
    54e0: 992e         	ldr	r1, [sp, #0xb8]
    54e2: 3101         	adds	r1, #0x1
    54e4: f7fe ed0c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x15e8
    54e8: 4648         	mov	r0, r9
    54ea: f7fe ec36    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1794
    54ee: 980d         	ldr	r0, [sp, #0x34]
    54f0: f7fe ec32    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x179c
    54f4: 4640         	mov	r0, r8
    54f6: f7fe ec30    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x17a0
    54fa: 980e         	ldr	r0, [sp, #0x38]
    54fc: f7fe ec2c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x17a8
    5500: 9d26         	ldr	r5, [sp, #0x98]
    5502: e62b         	b	0x515c <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x70> @ imm = #-0x3aa
    5504: f005 f81a    	bl	0xa53c <getPmLogContext()> @ imm = #0x5034
    5508: 6e63         	ldr	r3, [r4, #0x64]
    550a: 2107         	movs	r1, #0x7
    550c: 9308         	str	r3, [sp, #0x20]
    550e: 6ce3         	ldr	r3, [r4, #0x4c]
    5510: 9307         	str	r3, [sp, #0x1c]
    5512: f894 3048    	ldrb.w	r3, [r4, #0x48]
    5516: 9306         	str	r3, [sp, #0x18]
    5518: 4b37         	ldr	r3, [pc, #0xdc]         @ 0x55f8 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x50c> literal=0x8212
    551a: 447b         	add	r3, pc => raw=0xd730 ghidra=0x1d730 file=55088 "_headphoneStatusUpdateCallBack"
    551c: 9305         	str	r3, [sp, #0x14]
    551e: 4b37         	ldr	r3, [pc, #0xdc]         @ 0x55fc <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x510> literal=0x822c
    5520: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    5522: 9304         	str	r3, [sp, #0x10]
    5524: 4b36         	ldr	r3, [pc, #0xd8]         @ 0x5600 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x514> literal=0x827e
    5526: 447b         	add	r3, pc => raw=0xd7a8 ghidra=0x1d7a8 file=55208 "%s:%s() Send notification for headphone connection status: %d mInternalConnectStatus:%s mExternalConnectStatus:%s"
    5528: 9303         	str	r3, [sp, #0xc]
    552a: 2300         	movs	r3, #0x0
    552c: 461a         	mov	r2, r3
    552e: e9cd 3301    	strd	r3, r3, [sp, #4]
    5532: 9300         	str	r3, [sp]
    5534: f7fe ed72    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x151c
    5538: e7b8         	b	0x54ac <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x3c0> @ imm = #-0x90
    553a: 9a27         	ldr	r2, [sp, #0x9c]
    553c: 6ea3         	ldr	r3, [r4, #0x68]
    553e: 429a         	cmp	r2, r3
    5540: f47f ae34    	bne.w	0x51ac <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0xc0> @ imm = #-0x398
    5544: 9d26         	ldr	r5, [sp, #0x98]
    5546: 6e61         	ldr	r1, [r4, #0x64]
    5548: 2a00         	cmp	r2, #0x0
    554a: f43f ae07    	beq.w	0x515c <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x70> @ imm = #-0x3f2
    554e: 4628         	mov	r0, r5
    5550: f7fe edfe    	blx	0x4150 <memcmp@plt>     @ imm = #-0x1404
    5554: 2800         	cmp	r0, #0x0
    5556: f43f ae01    	beq.w	0x515c <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x70> @ imm = #-0x3fe
    555a: e627         	b	0x51ac <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0xc0> @ imm = #-0x3b2
    555c: 4648         	mov	r0, r9
    555e: f7fe ebfc    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1808
    5562: 980d         	ldr	r0, [sp, #0x34]
    5564: f7fe ebf8    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1810
    5568: 4640         	mov	r0, r8
    556a: f7fe ebf6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1814
    556e: 980e         	ldr	r0, [sp, #0x38]
    5570: f7fe ebf2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x181c
    5574: 980c         	ldr	r0, [sp, #0x30]
    5576: f7fe edbc    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x1488
    557a: 4658         	mov	r0, r11
    557c: f7fe edb8    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x1490
    5580: f7fe edfe    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x1404
    5584: 4638         	mov	r0, r7
    5586: f7fe edb4    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x1498
    558a: 4628         	mov	r0, r5
    558c: f7fe ebe4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1838
    5590: e7e4         	b	0x555c <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x470> @ imm = #-0x38
    5592: 4628         	mov	r0, r5
    5594: f7fe ed48    	blx	0x4028 <_ZN7pbnjson10JGeneratorD1Ev@plt> @ imm = #-0x1570
    5598: 4638         	mov	r0, r7
    559a: f7fe edaa    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x14ac
    559e: e7dd         	b	0x555c <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x470> @ imm = #-0x46
    55a0: e7e8         	b	0x5574 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x488> @ imm = #-0x30
    55a2: 4638         	mov	r0, r7
    55a4: f7fe eda4    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x14b8
    55a8: 4628         	mov	r0, r5
    55aa: f7fe ebd6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1854
    55ae: e7de         	b	0x556e <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x482> @ imm = #-0x44
    55b0: e002         	b	0x55b8 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4cc> @ imm = #0x4
    55b2: 4628         	mov	r0, r5
    55b4: f7fe ebd0    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1860
    55b8: 4638         	mov	r0, r7
    55ba: f7fe ed9a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x14cc
    55be: e7d6         	b	0x556e <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x482> @ imm = #-0x54
    55c0: e7ea         	b	0x5598 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4ac> @ imm = #-0x2c
    55c2: e7df         	b	0x5584 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x498> @ imm = #-0x42
    55c4: e7de         	b	0x5584 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x498> @ imm = #-0x44
    55c6: e7e7         	b	0x5598 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4ac> @ imm = #-0x32
    55c8: e7dc         	b	0x5584 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x498> @ imm = #-0x48
    55ca: e7e5         	b	0x5598 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x4ac> @ imm = #-0x36
    55cc: e7cc         	b	0x5568 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x47c> @ imm = #-0x68
    55ce: e7c8         	b	0x5562 <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x476> @ imm = #-0x70
    55d0: e7cd         	b	0x556e <ConnectionManager::_headphoneStatusUpdateCallBack(void*)+0x482> @ imm = #-0x66
    55d2: bf00         	nop
    55d4: 86e0         	strh	r0, [r4, #0x36]
    55d6: 0000         	movs	r0, r0
    55d8: 86c4         	strh	r4, [r0, #0x36]
    55da: 0000         	movs	r0, r0
    55dc: 8480         	strh	r0, [r0, #0x24]
    55de: 0000         	movs	r0, r0
    55e0: 840e         	strh	r6, [r1, #0x20]
    55e2: 0000         	movs	r0, r0
    55e4: 83d0         	strh	r0, [r2, #0x1e]
    55e6: 0000         	movs	r0, r0
    55e8: 8390         	strh	r0, [r2, #0x1c]
    55ea: 0000         	movs	r0, r0
    55ec: 8328         	strh	r0, [r5, #0x18]
    55ee: 0000         	movs	r0, r0
    55f0: 82fc         	strh	r4, [r7, #0x16]
    55f2: 0000         	movs	r0, r0
    55f4: 82b8         	strh	r0, [r7, #0x14]
    55f6: 0000         	movs	r0, r0
    55f8: 8212         	strh	r2, [r2, #0x10]
    55fa: 0000         	movs	r0, r0
    55fc: 822c         	strh	r4, [r5, #0x10]
    55fe: 0000         	movs	r0, r0
    5600: 827e         	strh	r6, [r7, #0x12]
    5602: 0000         	movs	r0, r0

00005604 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)>:
    5604: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    5608: b09b         	sub	sp, #0x6c
    560a: 4604         	mov	r4, r0
    560c: 4689         	mov	r9, r1
    560e: 4617         	mov	r7, r2
    5610: 461d         	mov	r5, r3
    5612: f004 ff93    	bl	0xa53c <getPmLogContext()> @ imm = #0x4f26
    5616: 2800         	cmp	r0, #0x0
    5618: d07f         	beq	0x571a <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x116> @ imm = #0xfe
    561a: f004 ff8f    	bl	0xa53c <getPmLogContext()> @ imm = #0x4f1e
    561e: 6803         	ldr	r3, [r0]
    5620: 2b05         	cmp	r3, #0x5
    5622: dc7a         	bgt	0x571a <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x116> @ imm = #0xf4
    5624: f8d4 8040    	ldr.w	r8, [r4, #0x40]
    5628: f1b8 0f00    	cmp.w	r8, #0x0
    562c: f000 80a2    	beq.w	0x5774 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x170> @ imm = #0x144
    5630: e9d9 9400    	ldrd	r9, r4, [r9]
    5634: ab10         	add	r3, sp, #0x40
    5636: 9309         	str	r3, [sp, #0x24]
    5638: f10d 0b38    	add.w	r11, sp, #0x38
    563c: 930e         	str	r3, [sp, #0x38]
    563e: fab9 f289    	clz	r2, r9
    5642: 4263         	rsbs	r3, r4, #0
    5644: 0952         	lsrs	r2, r2, #0x5
    5646: 4599         	cmp	r9, r3
    5648: bf08         	it	eq
    564a: 2200         	moveq	r2, #0x0
    564c: 2a00         	cmp	r2, #0x0
    564e: f040 80ce    	bne.w	0x57ee <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x1ea> @ imm = #0x19c
    5652: 2c0f         	cmp	r4, #0xf
    5654: 9414         	str	r4, [sp, #0x50]
    5656: f200 8098    	bhi.w	0x578a <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x186> @ imm = #0x130
    565a: 2c01         	cmp	r4, #0x1
    565c: f040 80a4    	bne.w	0x57a8 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x1a4> @ imm = #0x148
    5660: 9a09         	ldr	r2, [sp, #0x24]
    5662: ae14         	add	r6, sp, #0x50
    5664: f899 3000    	ldrb.w	r3, [r9]
    5668: f88d 3040    	strb.w	r3, [sp, #0x40]
    566c: 2300         	movs	r3, #0x0
    566e: 940f         	str	r4, [sp, #0x3c]
    5670: 5513         	strb	r3, [r2, r4]
    5672: f10d 0958    	add.w	r9, sp, #0x58
    5676: e9d7 a400    	ldrd	r10, r4, [r7]
    567a: f8cd 9050    	str.w	r9, [sp, #0x50]
    567e: 4263         	rsbs	r3, r4, #0
    5680: faba f28a    	clz	r2, r10
    5684: 0952         	lsrs	r2, r2, #0x5
    5686: 459a         	cmp	r10, r3
    5688: bf08         	it	eq
    568a: 2200         	moveq	r2, #0x0
    568c: 2a00         	cmp	r2, #0x0
    568e: f040 80aa    	bne.w	0x57e6 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x1e2> @ imm = #0x154
    5692: 2c0f         	cmp	r4, #0xf
    5694: 940b         	str	r4, [sp, #0x2c]
    5696: f200 8097    	bhi.w	0x57c8 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x1c4> @ imm = #0x12e
    569a: 2c01         	cmp	r4, #0x1
    569c: f040 808c    	bne.w	0x57b8 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x1b4> @ imm = #0x118
    56a0: f89a 3000    	ldrb.w	r3, [r10]
    56a4: 464a         	mov	r2, r9
    56a6: af0b         	add	r7, sp, #0x2c
    56a8: f88d 3058    	strb.w	r3, [sp, #0x58]
    56ac: 2300         	movs	r3, #0x0
    56ae: 9415         	str	r4, [sp, #0x54]
    56b0: 4629         	mov	r1, r5
    56b2: 4638         	mov	r0, r7
    56b4: 5513         	strb	r3, [r2, r4]
    56b6: f7fe ec0a    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x17ec
    56ba: 4640         	mov	r0, r8
    56bc: 463b         	mov	r3, r7
    56be: 4632         	mov	r2, r6
    56c0: 4659         	mov	r1, r11
    56c2: f7fe ebda    	blx	0x3e78 <_ZN9umiClient12connectInputENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_N7pbnjson6JValueE@plt> @ imm = #-0x184c
    56c6: 4604         	mov	r4, r0
    56c8: 4638         	mov	r0, r7
    56ca: f7fe eb46    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1974
    56ce: 9814         	ldr	r0, [sp, #0x50]
    56d0: 4548         	cmp	r0, r9
    56d2: d003         	beq	0x56dc <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0xd8> @ imm = #0x6
    56d4: 9916         	ldr	r1, [sp, #0x58]
    56d6: 3101         	adds	r1, #0x1
    56d8: f7fe ec12    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x17dc
    56dc: 980e         	ldr	r0, [sp, #0x38]
    56de: 9b09         	ldr	r3, [sp, #0x24]
    56e0: 4298         	cmp	r0, r3
    56e2: d048         	beq	0x5776 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x172> @ imm = #0x90
    56e4: 9910         	ldr	r1, [sp, #0x40]
    56e6: 3101         	adds	r1, #0x1
    56e8: f7fe ec0a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x17ec
    56ec: f004 ff26    	bl	0xa53c <getPmLogContext()> @ imm = #0x4e4c
    56f0: 2800         	cmp	r0, #0x0
    56f2: d144         	bne	0x577e <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x17a> @ imm = #0x88
    56f4: f004 ff22    	bl	0xa53c <getPmLogContext()> @ imm = #0x4e44
    56f8: 4949         	ldr	r1, [pc, #0x124]        @ 0x5820 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x21c> literal=0x81a4
    56fa: 2200         	movs	r2, #0x0
    56fc: 4b49         	ldr	r3, [pc, #0x124]        @ 0x5824 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x220> literal=0x8142
    56fe: 9404         	str	r4, [sp, #0x10]
    5700: 4479         	add	r1, pc => raw=0xd8a8 ghidra=0x1d8a8 file=55464 "Audio connect umi returnStatus %d"
    5702: e9cd 2200    	strd	r2, r2, [sp]
    5706: e9cd 2102    	strd	r2, r1, [sp, #8]
    570a: 447b         	add	r3, pc => raw=0xd850 ghidra=0x1d850 file=55376 "AUDIO_CONNECT_CALL"
    570c: 2106         	movs	r1, #0x6
    570e: f7fe ec86    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x16f4
    5712: 4620         	mov	r0, r4
    5714: b01b         	add	sp, #0x6c
    5716: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    571a: ae14         	add	r6, sp, #0x50
    571c: f004 ff0e    	bl	0xa53c <getPmLogContext()> @ imm = #0x4e1c
    5720: 2200         	movs	r2, #0x0
    5722: 4680         	mov	r8, r0
    5724: 4629         	mov	r1, r5
    5726: 4630         	mov	r0, r6
    5728: f8d9 b000    	ldr.w	r11, [r9]
    572c: f8d7 a000    	ldr.w	r10, [r7]
    5730: f7fe ed5c    	blx	0x41ec <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt> @ imm = #-0x1548
    5734: 9b14         	ldr	r3, [sp, #0x50]
    5736: 2200         	movs	r2, #0x0
    5738: 4640         	mov	r0, r8
    573a: 2106         	movs	r1, #0x6
    573c: e9cd ba04    	strd	r11, r10, [sp, #16]
    5740: 9306         	str	r3, [sp, #0x18]
    5742: 4b39         	ldr	r3, [pc, #0xe4]         @ 0x5828 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x224> literal=0x8116
    5744: e9cd 2201    	strd	r2, r2, [sp, #4]
    5748: 9200         	str	r2, [sp]
    574a: 447b         	add	r3, pc => raw=0xd864 ghidra=0x1d864 file=55396 "Audio connect request for source:%s, sink:%s, pipelineAttribute:%s"
    574c: 9303         	str	r3, [sp, #0xc]
    574e: 4b37         	ldr	r3, [pc, #0xdc]         @ 0x582c <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x228> literal=0x80fc
    5750: 447b         	add	r3, pc => raw=0xd850 ghidra=0x1d850 file=55376 "AUDIO_CONNECT_CALL"
    5752: f7fe ec64    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x1738
    5756: 9814         	ldr	r0, [sp, #0x50]
    5758: ab16         	add	r3, sp, #0x58
    575a: 4298         	cmp	r0, r3
    575c: f43f af62    	beq.w	0x5624 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x20> @ imm = #-0x13c
    5760: 9916         	ldr	r1, [sp, #0x58]
    5762: 3101         	adds	r1, #0x1
    5764: f7fe ebcc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1868
    5768: f8d4 8040    	ldr.w	r8, [r4, #0x40]
    576c: f1b8 0f00    	cmp.w	r8, #0x0
    5770: f47f af5e    	bne.w	0x5630 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x2c> @ imm = #-0x144
    5774: 2402         	movs	r4, #0x2
    5776: f004 fee1    	bl	0xa53c <getPmLogContext()> @ imm = #0x4dc2
    577a: 2800         	cmp	r0, #0x0
    577c: d0ba         	beq	0x56f4 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0xf0> @ imm = #-0x8c
    577e: f004 fedd    	bl	0xa53c <getPmLogContext()> @ imm = #0x4dba
    5782: 6803         	ldr	r3, [r0]
    5784: 2b05         	cmp	r3, #0x5
    5786: dcb5         	bgt	0x56f4 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0xf0> @ imm = #-0x96
    5788: e7c3         	b	0x5712 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x10e> @ imm = #-0x7a
    578a: ae14         	add	r6, sp, #0x50
    578c: 4658         	mov	r0, r11
    578e: 4631         	mov	r1, r6
    5790: f7fe ec70    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x1720
    5794: 9b14         	ldr	r3, [sp, #0x50]
    5796: 900e         	str	r0, [sp, #0x38]
    5798: 9310         	str	r3, [sp, #0x40]
    579a: 4622         	mov	r2, r4
    579c: 4649         	mov	r1, r9
    579e: f7fe ebf4    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x1818
    57a2: 9c14         	ldr	r4, [sp, #0x50]
    57a4: 9a0e         	ldr	r2, [sp, #0x38]
    57a6: e761         	b	0x566c <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x68> @ imm = #-0x13e
    57a8: 9a09         	ldr	r2, [sp, #0x24]
    57aa: ae14         	add	r6, sp, #0x50
    57ac: 2c00         	cmp	r4, #0x0
    57ae: f43f af5d    	beq.w	0x566c <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x68> @ imm = #-0x146
    57b2: 9809         	ldr	r0, [sp, #0x24]
    57b4: ae14         	add	r6, sp, #0x50
    57b6: e7f0         	b	0x579a <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x196> @ imm = #-0x20
    57b8: 464a         	mov	r2, r9
    57ba: af0b         	add	r7, sp, #0x2c
    57bc: 2c00         	cmp	r4, #0x0
    57be: f43f af75    	beq.w	0x56ac <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0xa8> @ imm = #-0x116
    57c2: 4648         	mov	r0, r9
    57c4: af0b         	add	r7, sp, #0x2c
    57c6: e007         	b	0x57d8 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x1d4> @ imm = #0xe
    57c8: af0b         	add	r7, sp, #0x2c
    57ca: 4630         	mov	r0, r6
    57cc: 4639         	mov	r1, r7
    57ce: f7fe ec52    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x175c
    57d2: 9b0b         	ldr	r3, [sp, #0x2c]
    57d4: 9014         	str	r0, [sp, #0x50]
    57d6: 9316         	str	r3, [sp, #0x58]
    57d8: 4622         	mov	r2, r4
    57da: 4651         	mov	r1, r10
    57dc: f7fe ebd4    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x1858
    57e0: 9c0b         	ldr	r4, [sp, #0x2c]
    57e2: 9a14         	ldr	r2, [sp, #0x50]
    57e4: e762         	b	0x56ac <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0xa8> @ imm = #-0x13c
    57e6: 4812         	ldr	r0, [pc, #0x48]         @ 0x5830 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x22c> literal=0x7eac
    57e8: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    57ea: f7fe ea8c    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x1ae8
    57ee: 4811         	ldr	r0, [pc, #0x44]         @ 0x5834 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x230> literal=0x7ea4
    57f0: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    57f2: f7fe ea88    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x1af0
    57f6: 4658         	mov	r0, r11
    57f8: f7fe ec7a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x170c
    57fc: f7fe ecc0    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x1680
    5800: e7fc         	b	0x57fc <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x1f8> @ imm = #-0x8
    5802: e7fb         	b	0x57fc <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x1f8> @ imm = #-0xa
    5804: e7f7         	b	0x57f6 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x1f2> @ imm = #-0x12
    5806: 4630         	mov	r0, r6
    5808: f7fe ec72    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x171c
    580c: f7fe ecb8    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x1690
    5810: e002         	b	0x5818 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x214> @ imm = #0x4
    5812: 4638         	mov	r0, r7
    5814: f7fe eaa0    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1ac0
    5818: 4630         	mov	r0, r6
    581a: f7fe ec6a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x172c
    581e: e7ea         	b	0x57f6 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)+0x1f2> @ imm = #-0x2c
    5820: 81a4         	strh	r4, [r4, #0xc]
    5822: 0000         	movs	r0, r0
    5824: 8142         	strh	r2, [r0, #0xa]
    5826: 0000         	movs	r0, r0
    5828: 8116         	strh	r6, [r2, #0x8]
    582a: 0000         	movs	r0, r0
    582c: 80fc         	strh	r4, [r7, #0x6]
    582e: 0000         	movs	r0, r0
    5830: 7eac         	ldrb	r4, [r5, #0x1a]
    5832: 0000         	movs	r0, r0
    5834: 7ea4         	ldrb	r4, [r4, #0x1a]
    5836: 0000         	movs	r0, r0

00005838 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)>:
    5838: e92d 41f0    	push.w	{r4, r5, r6, r7, r8, lr}
    583c: b08e         	sub	sp, #0x38
    583e: 4605         	mov	r5, r0
    5840: 460c         	mov	r4, r1
    5842: f004 fe7b    	bl	0xa53c <getPmLogContext()> @ imm = #0x4cf6
    5846: 2800         	cmp	r0, #0x0
    5848: d04a         	beq	0x58e0 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xa8> @ imm = #0x94
    584a: f004 fe77    	bl	0xa53c <getPmLogContext()> @ imm = #0x4cee
    584e: 6803         	ldr	r3, [r0]
    5850: 2b05         	cmp	r3, #0x5
    5852: dc45         	bgt	0x58e0 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xa8> @ imm = #0x8a
    5854: 6c2e         	ldr	r6, [r5, #0x40]
    5856: 2e00         	cmp	r6, #0x0
    5858: d055         	beq	0x5906 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xce> @ imm = #0xaa
    585a: e9d4 5400    	ldrd	r5, r4, [r4]
    585e: af0a         	add	r7, sp, #0x28
    5860: f10d 0820    	add.w	r8, sp, #0x20
    5864: 9708         	str	r7, [sp, #0x20]
    5866: 4263         	rsbs	r3, r4, #0
    5868: fab5 f285    	clz	r2, r5
    586c: 0952         	lsrs	r2, r2, #0x5
    586e: 429d         	cmp	r5, r3
    5870: bf08         	it	eq
    5872: 2200         	moveq	r2, #0x0
    5874: 2a00         	cmp	r2, #0x0
    5876: d15b         	bne	0x5930 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xf8> @ imm = #0xb6
    5878: 2c0f         	cmp	r4, #0xf
    587a: 9407         	str	r4, [sp, #0x1c]
    587c: d84a         	bhi	0x5914 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xdc> @ imm = #0x94
    587e: 2c01         	cmp	r4, #0x1
    5880: d143         	bne	0x590a <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xd2> @ imm = #0x86
    5882: 782b         	ldrb	r3, [r5]
    5884: f88d 3028    	strb.w	r3, [sp, #0x28]
    5888: 463b         	mov	r3, r7
    588a: 2200         	movs	r2, #0x0
    588c: 9409         	str	r4, [sp, #0x24]
    588e: 4630         	mov	r0, r6
    5890: 4641         	mov	r1, r8
    5892: 551a         	strb	r2, [r3, r4]
    5894: f7fe ea8a    	blx	0x3dac <_ZN9umiClient14setSoundOutputENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x1aec
    5898: 9b08         	ldr	r3, [sp, #0x20]
    589a: 4604         	mov	r4, r0
    589c: 42bb         	cmp	r3, r7
    589e: d004         	beq	0x58aa <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x72> @ imm = #0x8
    58a0: 990a         	ldr	r1, [sp, #0x28]
    58a2: 4618         	mov	r0, r3
    58a4: 3101         	adds	r1, #0x1
    58a6: f7fe eb2c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x19a8
    58aa: f004 fe47    	bl	0xa53c <getPmLogContext()> @ imm = #0x4c8e
    58ae: b120         	cbz	r0, 0x58ba <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x82> @ imm = #0x8
    58b0: f004 fe44    	bl	0xa53c <getPmLogContext()> @ imm = #0x4c88
    58b4: 6803         	ldr	r3, [r0]
    58b6: 2b05         	cmp	r3, #0x5
    58b8: dd0e         	ble	0x58d8 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xa0> @ imm = #0x1c
    58ba: f004 fe3f    	bl	0xa53c <getPmLogContext()> @ imm = #0x4c7e
    58be: 4921         	ldr	r1, [pc, #0x84]         @ 0x5944 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x10c> literal=0x804e
    58c0: 2200         	movs	r2, #0x0
    58c2: 4b21         	ldr	r3, [pc, #0x84]         @ 0x5948 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x110> literal=0x7ff8
    58c4: 9404         	str	r4, [sp, #0x10]
    58c6: 4479         	add	r1, pc => raw=0xd918 ghidra=0x1d918 file=55576 "Audio setSoundOutput umi returnStatus %d"
    58c8: e9cd 2200    	strd	r2, r2, [sp]
    58cc: e9cd 2102    	strd	r2, r1, [sp, #8]
    58d0: 447b         	add	r3, pc => raw=0xd8cc ghidra=0x1d8cc file=55500 "AUDIO_SET_SOUNDOUTPUT_CALL"
    58d2: 2106         	movs	r1, #0x6
    58d4: f7fe eba2    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x18bc
    58d8: 4620         	mov	r0, r4
    58da: b00e         	add	sp, #0x38
    58dc: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
    58e0: f004 fe2c    	bl	0xa53c <getPmLogContext()> @ imm = #0x4c58
    58e4: 4b19         	ldr	r3, [pc, #0x64]         @ 0x594c <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x114> literal=0x7ffa
    58e6: 2106         	movs	r1, #0x6
    58e8: 6822         	ldr	r2, [r4]
    58ea: 447b         	add	r3, pc => raw=0xd8e8 ghidra=0x1d8e8 file=55528 "Audio setSoundOut request for soundOutput:%s"
    58ec: e9cd 3203    	strd	r3, r2, [sp, #12]
    58f0: 2200         	movs	r2, #0x0
    58f2: 4b17         	ldr	r3, [pc, #0x5c]         @ 0x5950 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x118> literal=0x7fce
    58f4: e9cd 2201    	strd	r2, r2, [sp, #4]
    58f8: 9200         	str	r2, [sp]
    58fa: 447b         	add	r3, pc => raw=0xd8cc ghidra=0x1d8cc file=55500 "AUDIO_SET_SOUNDOUTPUT_CALL"
    58fc: f7fe eb8e    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x18e4
    5900: 6c2e         	ldr	r6, [r5, #0x40]
    5902: 2e00         	cmp	r6, #0x0
    5904: d1a9         	bne	0x585a <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x22> @ imm = #-0xae
    5906: 2402         	movs	r4, #0x2
    5908: e7cf         	b	0x58aa <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x72> @ imm = #-0x62
    590a: 463b         	mov	r3, r7
    590c: 2c00         	cmp	r4, #0x0
    590e: d0bc         	beq	0x588a <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x52> @ imm = #-0x88
    5910: 4638         	mov	r0, r7
    5912: e006         	b	0x5922 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xea> @ imm = #0xc
    5914: a907         	add	r1, sp, #0x1c
    5916: 4640         	mov	r0, r8
    5918: f7fe ebac    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x18a8
    591c: 9b07         	ldr	r3, [sp, #0x1c]
    591e: 9008         	str	r0, [sp, #0x20]
    5920: 930a         	str	r3, [sp, #0x28]
    5922: 4622         	mov	r2, r4
    5924: 4629         	mov	r1, r5
    5926: f7fe eb30    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x19a0
    592a: e9dd 4307    	ldrd	r4, r3, [sp, #28]
    592e: e7ac         	b	0x588a <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x52> @ imm = #-0xa8
    5930: 4808         	ldr	r0, [pc, #0x20]         @ 0x5954 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x11c> literal=0x7d62
    5932: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    5934: f7fe e9e6    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x1c34
    5938: 4640         	mov	r0, r8
    593a: f7fe ebda    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x184c
    593e: f7fe ec20    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x17c0
    5942: bf00         	nop
    5944: 804e         	strh	r6, [r1, #0x2]
    5946: 0000         	movs	r0, r0
    5948: 7ff8         	ldrb	r0, [r7, #0x1f]
    594a: 0000         	movs	r0, r0
    594c: 7ffa         	ldrb	r2, [r7, #0x1f]
    594e: 0000         	movs	r0, r0
    5950: 7fce         	ldrb	r6, [r1, #0x1f]
    5952: 0000         	movs	r0, r0
    5954: 7d62         	ldrb	r2, [r4, #0x15]
    5956: 0000         	movs	r0, r0

00005958 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)>:
    5958: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    595c: b099         	sub	sp, #0x64
    595e: 4604         	mov	r4, r0
    5960: 460f         	mov	r7, r1
    5962: 4615         	mov	r5, r2
    5964: 461e         	mov	r6, r3
    5966: f004 fde9    	bl	0xa53c <getPmLogContext()> @ imm = #0x4bd2
    596a: 2800         	cmp	r0, #0x0
    596c: d078         	beq	0x5a60 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x108> @ imm = #0xf0
    596e: f004 fde5    	bl	0xa53c <getPmLogContext()> @ imm = #0x4bca
    5972: 6803         	ldr	r3, [r0]
    5974: 2b05         	cmp	r3, #0x5
    5976: dc73         	bgt	0x5a60 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x108> @ imm = #0xe6
    5978: f8d4 8040    	ldr.w	r8, [r4, #0x40]
    597c: f1b8 0f00    	cmp.w	r8, #0x0
    5980: f000 8088    	beq.w	0x5a94 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x13c> @ imm = #0x110
    5984: e9d7 b400    	ldrd	r11, r4, [r7]
    5988: ab0e         	add	r3, sp, #0x38
    598a: 9309         	str	r3, [sp, #0x24]
    598c: f10d 0a30    	add.w	r10, sp, #0x30
    5990: 930c         	str	r3, [sp, #0x30]
    5992: fabb f78b    	clz	r7, r11
    5996: 4263         	rsbs	r3, r4, #0
    5998: 097f         	lsrs	r7, r7, #0x5
    599a: 459b         	cmp	r11, r3
    599c: bf08         	it	eq
    599e: 2700         	moveq	r7, #0x0
    59a0: 2f00         	cmp	r7, #0x0
    59a2: f040 80ac    	bne.w	0x5afe <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1a6> @ imm = #0x158
    59a6: 2c0f         	cmp	r4, #0xf
    59a8: 9412         	str	r4, [sp, #0x48]
    59aa: d875         	bhi	0x5a98 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x140> @ imm = #0xea
    59ac: 2c01         	cmp	r4, #0x1
    59ae: f040 8084    	bne.w	0x5aba <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x162> @ imm = #0x108
    59b2: 9a09         	ldr	r2, [sp, #0x24]
    59b4: f10d 0948    	add.w	r9, sp, #0x48
    59b8: f89b 3000    	ldrb.w	r3, [r11]
    59bc: f88d 3038    	strb.w	r3, [sp, #0x38]
    59c0: 2300         	movs	r3, #0x0
    59c2: 940d         	str	r4, [sp, #0x34]
    59c4: 5513         	strb	r3, [r2, r4]
    59c6: e9d5 b400    	ldrd	r11, r4, [r5]
    59ca: ad14         	add	r5, sp, #0x50
    59cc: 9512         	str	r5, [sp, #0x48]
    59ce: 4263         	rsbs	r3, r4, #0
    59d0: fabb f78b    	clz	r7, r11
    59d4: 097f         	lsrs	r7, r7, #0x5
    59d6: 459b         	cmp	r11, r3
    59d8: bf08         	it	eq
    59da: 2700         	moveq	r7, #0x0
    59dc: 2f00         	cmp	r7, #0x0
    59de: f040 808a    	bne.w	0x5af6 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x19e> @ imm = #0x114
    59e2: 2c0f         	cmp	r4, #0xf
    59e4: 940b         	str	r4, [sp, #0x2c]
    59e6: d877         	bhi	0x5ad8 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x180> @ imm = #0xee
    59e8: 2c01         	cmp	r4, #0x1
    59ea: d170         	bne	0x5ace <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x176> @ imm = #0xe0
    59ec: f89b 3000    	ldrb.w	r3, [r11]
    59f0: 462a         	mov	r2, r5
    59f2: f88d 3050    	strb.w	r3, [sp, #0x50]
    59f6: 2300         	movs	r3, #0x0
    59f8: 9413         	str	r4, [sp, #0x4c]
    59fa: 4640         	mov	r0, r8
    59fc: 5513         	strb	r3, [r2, r4]
    59fe: 4651         	mov	r1, r10
    5a00: 7833         	ldrb	r3, [r6]
    5a02: 464a         	mov	r2, r9
    5a04: f7fe ea1a    	blx	0x3e3c <_ZN9umiClient14setInputVolumeENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_h@plt> @ imm = #-0x1bcc
    5a08: 9b12         	ldr	r3, [sp, #0x48]
    5a0a: 4604         	mov	r4, r0
    5a0c: 42ab         	cmp	r3, r5
    5a0e: d004         	beq	0x5a1a <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xc2> @ imm = #0x8
    5a10: 9914         	ldr	r1, [sp, #0x50]
    5a12: 4618         	mov	r0, r3
    5a14: 3101         	adds	r1, #0x1
    5a16: f7fe ea74    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1b18
    5a1a: 980c         	ldr	r0, [sp, #0x30]
    5a1c: 9b09         	ldr	r3, [sp, #0x24]
    5a1e: 4298         	cmp	r0, r3
    5a20: d003         	beq	0x5a2a <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xd2> @ imm = #0x6
    5a22: 990e         	ldr	r1, [sp, #0x38]
    5a24: 3101         	adds	r1, #0x1
    5a26: f7fe ea6c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1b28
    5a2a: f004 fd87    	bl	0xa53c <getPmLogContext()> @ imm = #0x4b0e
    5a2e: b120         	cbz	r0, 0x5a3a <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xe2> @ imm = #0x8
    5a30: f004 fd84    	bl	0xa53c <getPmLogContext()> @ imm = #0x4b08
    5a34: 6803         	ldr	r3, [r0]
    5a36: 2b05         	cmp	r3, #0x5
    5a38: dd0e         	ble	0x5a58 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x100> @ imm = #0x1c
    5a3a: f004 fd7f    	bl	0xa53c <getPmLogContext()> @ imm = #0x4afe
    5a3e: 4938         	ldr	r1, [pc, #0xe0]         @ 0x5b20 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1c8> literal=0x7f52
    5a40: 2200         	movs	r2, #0x0
    5a42: 4b38         	ldr	r3, [pc, #0xe0]         @ 0x5b24 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1cc> literal=0x7ef0
    5a44: 9404         	str	r4, [sp, #0x10]
    5a46: 4479         	add	r1, pc => raw=0xd99c ghidra=0x1d99c file=55708 "Audio setinputvolume umi returnStatus %d"
    5a48: e9cd 2200    	strd	r2, r2, [sp]
    5a4c: e9cd 2102    	strd	r2, r1, [sp, #8]
    5a50: 447b         	add	r3, pc => raw=0xd944 ghidra=0x1d944 file=55620 "AUDIO_SET_INPUT_VOLUME"
    5a52: 2106         	movs	r1, #0x6
    5a54: f7fe eae2    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x1a3c
    5a58: 4620         	mov	r0, r4
    5a5a: b019         	add	sp, #0x64
    5a5c: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    5a60: f004 fd6c    	bl	0xa53c <getPmLogContext()> @ imm = #0x4ad8
    5a64: 6833         	ldr	r3, [r6]
    5a66: 2200         	movs	r2, #0x0
    5a68: 2106         	movs	r1, #0x6
    5a6a: 9306         	str	r3, [sp, #0x18]
    5a6c: 682b         	ldr	r3, [r5]
    5a6e: 9305         	str	r3, [sp, #0x14]
    5a70: 683b         	ldr	r3, [r7]
    5a72: e9cd 2201    	strd	r2, r2, [sp, #4]
    5a76: 9200         	str	r2, [sp]
    5a78: 9304         	str	r3, [sp, #0x10]
    5a7a: 4b2b         	ldr	r3, [pc, #0xac]         @ 0x5b28 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1d0> literal=0x7edc
    5a7c: 447b         	add	r3, pc => raw=0xd95c ghidra=0x1d95c file=55644 "Audio setinputvolume request for source:%s, sink:%s, volume:%d"
    5a7e: 9303         	str	r3, [sp, #0xc]
    5a80: 4b2a         	ldr	r3, [pc, #0xa8]         @ 0x5b2c <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1d4> literal=0x7ebe
    5a82: 447b         	add	r3, pc => raw=0xd944 ghidra=0x1d944 file=55620 "AUDIO_SET_INPUT_VOLUME"
    5a84: f7fe eaca    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x1a6c
    5a88: f8d4 8040    	ldr.w	r8, [r4, #0x40]
    5a8c: f1b8 0f00    	cmp.w	r8, #0x0
    5a90: f47f af78    	bne.w	0x5984 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x2c> @ imm = #-0x110
    5a94: 2402         	movs	r4, #0x2
    5a96: e7c8         	b	0x5a2a <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xd2> @ imm = #-0x70
    5a98: f10d 0948    	add.w	r9, sp, #0x48
    5a9c: 463a         	mov	r2, r7
    5a9e: 4649         	mov	r1, r9
    5aa0: 4650         	mov	r0, r10
    5aa2: f7fe eae8    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x1a30
    5aa6: 9b12         	ldr	r3, [sp, #0x48]
    5aa8: 900c         	str	r0, [sp, #0x30]
    5aaa: 930e         	str	r3, [sp, #0x38]
    5aac: 4622         	mov	r2, r4
    5aae: 4659         	mov	r1, r11
    5ab0: f7fe ea6a    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x1b2c
    5ab4: 9c12         	ldr	r4, [sp, #0x48]
    5ab6: 9a0c         	ldr	r2, [sp, #0x30]
    5ab8: e782         	b	0x59c0 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x68> @ imm = #-0xfc
    5aba: 9a09         	ldr	r2, [sp, #0x24]
    5abc: f10d 0948    	add.w	r9, sp, #0x48
    5ac0: 2c00         	cmp	r4, #0x0
    5ac2: f43f af7d    	beq.w	0x59c0 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x68> @ imm = #-0x106
    5ac6: 9809         	ldr	r0, [sp, #0x24]
    5ac8: f10d 0948    	add.w	r9, sp, #0x48
    5acc: e7ee         	b	0x5aac <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x154> @ imm = #-0x24
    5ace: 462a         	mov	r2, r5
    5ad0: 2c00         	cmp	r4, #0x0
    5ad2: d090         	beq	0x59f6 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x9e> @ imm = #-0xe0
    5ad4: 4628         	mov	r0, r5
    5ad6: e007         	b	0x5ae8 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x190> @ imm = #0xe
    5ad8: 463a         	mov	r2, r7
    5ada: a90b         	add	r1, sp, #0x2c
    5adc: 4648         	mov	r0, r9
    5ade: f7fe eaca    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x1a6c
    5ae2: 9b0b         	ldr	r3, [sp, #0x2c]
    5ae4: 9012         	str	r0, [sp, #0x48]
    5ae6: 9314         	str	r3, [sp, #0x50]
    5ae8: 4622         	mov	r2, r4
    5aea: 4659         	mov	r1, r11
    5aec: f7fe ea4c    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x1b68
    5af0: 9c0b         	ldr	r4, [sp, #0x2c]
    5af2: 9a12         	ldr	r2, [sp, #0x48]
    5af4: e77f         	b	0x59f6 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x9e> @ imm = #-0x102
    5af6: 480e         	ldr	r0, [pc, #0x38]         @ 0x5b30 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1d8> literal=0x7b9c
    5af8: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    5afa: f7fe e904    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x1df8
    5afe: 480d         	ldr	r0, [pc, #0x34]         @ 0x5b34 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1dc> literal=0x7b94
    5b00: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    5b02: f7fe e900    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x1e00
    5b06: f7fe eb3c    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x1988
    5b0a: 2f00         	cmp	r7, #0x0
    5b0c: d0fb         	beq	0x5b06 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1ae> @ imm = #-0xa
    5b0e: 4650         	mov	r0, r10
    5b10: f7fe eaee    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x1a24
    5b14: e7f7         	b	0x5b06 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1ae> @ imm = #-0x12
    5b16: e7fa         	b	0x5b0e <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1b6> @ imm = #-0xc
    5b18: 4648         	mov	r0, r9
    5b1a: f7fe eaea    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x1a2c
    5b1e: e7f6         	b	0x5b0e <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x1b6> @ imm = #-0x14
    5b20: 7f52         	ldrb	r2, [r2, #0x1d]
    5b22: 0000         	movs	r0, r0
    5b24: 7ef0         	ldrb	r0, [r6, #0x1b]
    5b26: 0000         	movs	r0, r0
    5b28: 7edc         	ldrb	r4, [r3, #0x1b]
    5b2a: 0000         	movs	r0, r0
    5b2c: 7ebe         	ldrb	r6, [r7, #0x1a]
    5b2e: 0000         	movs	r0, r0
    5b30: 7b9c         	ldrb	r4, [r3, #0xe]
    5b32: 0000         	movs	r0, r0
    5b34: 7b94         	ldrb	r4, [r2, #0xe]
    5b36: 0000         	movs	r0, r0

00005b38 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)>:
    5b38: b5f0         	push	{r4, r5, r6, r7, lr}
    5b3a: b089         	sub	sp, #0x24
    5b3c: 4607         	mov	r7, r0
    5b3e: 460c         	mov	r4, r1
    5b40: 4615         	mov	r5, r2
    5b42: 461e         	mov	r6, r3
    5b44: f004 fcfa    	bl	0xa53c <getPmLogContext()> @ imm = #0x49f4
    5b48: b368         	cbz	r0, 0x5ba6 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)+0x6e> @ imm = #0x5a
    5b4a: f004 fcf7    	bl	0xa53c <getPmLogContext()> @ imm = #0x49ee
    5b4e: 6803         	ldr	r3, [r0]
    5b50: 2b05         	cmp	r3, #0x5
    5b52: dc28         	bgt	0x5ba6 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)+0x6e> @ imm = #0x50
    5b54: 6c38         	ldr	r0, [r7, #0x40]
    5b56: 2702         	movs	r7, #0x2
    5b58: b128         	cbz	r0, 0x5b66 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)+0x2e> @ imm = #0xa
    5b5a: 4633         	mov	r3, r6
    5b5c: 462a         	mov	r2, r5
    5b5e: 4621         	mov	r1, r4
    5b60: f7fe e94e    	blx	0x3e00 <_ZN9umiClient18getHeadphoneStatusEPbRNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_@plt> @ imm = #-0x1d64
    5b64: 4607         	mov	r7, r0
    5b66: f004 fce9    	bl	0xa53c <getPmLogContext()> @ imm = #0x49d2
    5b6a: b120         	cbz	r0, 0x5b76 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)+0x3e> @ imm = #0x8
    5b6c: f004 fce6    	bl	0xa53c <getPmLogContext()> @ imm = #0x49cc
    5b70: 6803         	ldr	r3, [r0]
    5b72: 2b05         	cmp	r3, #0x5
    5b74: dd14         	ble	0x5ba0 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)+0x68> @ imm = #0x28
    5b76: f004 fce1    	bl	0xa53c <getPmLogContext()> @ imm = #0x49c2
    5b7a: 6831         	ldr	r1, [r6]
    5b7c: 682a         	ldr	r2, [r5]
    5b7e: 7823         	ldrb	r3, [r4]
    5b80: e9cd 1706    	strd	r1, r7, [sp, #24]
    5b84: 490f         	ldr	r1, [pc, #0x3c]         @ 0x5bc4 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)+0x8c> literal=0x7e7a
    5b86: e9cd 3204    	strd	r3, r2, [sp, #16]
    5b8a: 2200         	movs	r2, #0x0
    5b8c: 4b0e         	ldr	r3, [pc, #0x38]         @ 0x5bc8 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)+0x90> literal=0x7e2a
    5b8e: 4479         	add	r1, pc => raw=0xda0c ghidra=0x1da0c file=55820 "Audio umi getHeadphoneStatus:%d internalConnectStatus:%s externalConnectStatus:%s returnStatus %d"
    5b90: e9cd 2200    	strd	r2, r2, [sp]
    5b94: e9cd 2102    	strd	r2, r1, [sp, #8]
    5b98: 2106         	movs	r1, #0x6
    5b9a: 447b         	add	r3, pc => raw=0xd9c8 ghidra=0x1d9c8 file=55752 "AUDIO_GET_HEADPHONE_STATUS_CALL"
    5b9c: f7fe ea3e    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x1b84
    5ba0: 4638         	mov	r0, r7
    5ba2: b009         	add	sp, #0x24
    5ba4: bdf0         	pop	{r4, r5, r6, r7, pc}
    5ba6: f004 fcc9    	bl	0xa53c <getPmLogContext()> @ imm = #0x4992
    5baa: 4b08         	ldr	r3, [pc, #0x20]         @ 0x5bcc <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)+0x94> literal=0x7e2e
    5bac: 2200         	movs	r2, #0x0
    5bae: 2106         	movs	r1, #0x6
    5bb0: e9cd 2201    	strd	r2, r2, [sp, #4]
    5bb4: 9200         	str	r2, [sp]
    5bb6: 447b         	add	r3, pc => raw=0xd9e8 ghidra=0x1d9e8 file=55784 "Audio getHeadphoneStatus request"
    5bb8: 9303         	str	r3, [sp, #0xc]
    5bba: 4b05         	ldr	r3, [pc, #0x14]         @ 0x5bd0 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)+0x98> literal=0x7e08
    5bbc: 447b         	add	r3, pc => raw=0xd9c8 ghidra=0x1d9c8 file=55752 "AUDIO_GET_HEADPHONE_STATUS_CALL"
    5bbe: f7fe ea2e    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x1ba4
    5bc2: e7c7         	b	0x5b54 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)+0x1c> @ imm = #-0x72
    5bc4: 7e7a         	ldrb	r2, [r7, #0x19]
    5bc6: 0000         	movs	r0, r0
    5bc8: 7e2a         	ldrb	r2, [r5, #0x18]
    5bca: 0000         	movs	r0, r0
    5bcc: 7e2e         	ldrb	r6, [r5, #0x18]
    5bce: 0000         	movs	r0, r0
    5bd0: 7e08         	ldrb	r0, [r1, #0x18]
    5bd2: 0000         	movs	r0, r0

00005bd4 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)>:
    5bd4: e92d 41f0    	push.w	{r4, r5, r6, r7, r8, lr}
    5bd8: b08e         	sub	sp, #0x38
    5bda: 4605         	mov	r5, r0
    5bdc: 460c         	mov	r4, r1
    5bde: f004 fcad    	bl	0xa53c <getPmLogContext()> @ imm = #0x495a
    5be2: 2800         	cmp	r0, #0x0
    5be4: d04a         	beq	0x5c7c <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xa8> @ imm = #0x94
    5be6: f004 fca9    	bl	0xa53c <getPmLogContext()> @ imm = #0x4952
    5bea: 6803         	ldr	r3, [r0]
    5bec: 2b05         	cmp	r3, #0x5
    5bee: dc45         	bgt	0x5c7c <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xa8> @ imm = #0x8a
    5bf0: 6c2e         	ldr	r6, [r5, #0x40]
    5bf2: 2e00         	cmp	r6, #0x0
    5bf4: d055         	beq	0x5ca2 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xce> @ imm = #0xaa
    5bf6: e9d4 5400    	ldrd	r5, r4, [r4]
    5bfa: af0a         	add	r7, sp, #0x28
    5bfc: f10d 0820    	add.w	r8, sp, #0x20
    5c00: 9708         	str	r7, [sp, #0x20]
    5c02: 4263         	rsbs	r3, r4, #0
    5c04: fab5 f285    	clz	r2, r5
    5c08: 0952         	lsrs	r2, r2, #0x5
    5c0a: 429d         	cmp	r5, r3
    5c0c: bf08         	it	eq
    5c0e: 2200         	moveq	r2, #0x0
    5c10: 2a00         	cmp	r2, #0x0
    5c12: d15b         	bne	0x5ccc <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xf8> @ imm = #0xb6
    5c14: 2c0f         	cmp	r4, #0xf
    5c16: 9407         	str	r4, [sp, #0x1c]
    5c18: d84a         	bhi	0x5cb0 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xdc> @ imm = #0x94
    5c1a: 2c01         	cmp	r4, #0x1
    5c1c: d143         	bne	0x5ca6 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xd2> @ imm = #0x86
    5c1e: 782b         	ldrb	r3, [r5]
    5c20: f88d 3028    	strb.w	r3, [sp, #0x28]
    5c24: 463b         	mov	r3, r7
    5c26: 2200         	movs	r2, #0x0
    5c28: 9409         	str	r4, [sp, #0x24]
    5c2a: 4630         	mov	r0, r6
    5c2c: 4641         	mov	r1, r8
    5c2e: 551a         	strb	r2, [r3, r4]
    5c30: f7fe eb0c    	blx	0x424c <_ZN9umiClient18removePipelineInfoENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x19e8
    5c34: 9b08         	ldr	r3, [sp, #0x20]
    5c36: 4604         	mov	r4, r0
    5c38: 42bb         	cmp	r3, r7
    5c3a: d004         	beq	0x5c46 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x72> @ imm = #0x8
    5c3c: 990a         	ldr	r1, [sp, #0x28]
    5c3e: 4618         	mov	r0, r3
    5c40: 3101         	adds	r1, #0x1
    5c42: f7fe e95e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1d44
    5c46: f004 fc79    	bl	0xa53c <getPmLogContext()> @ imm = #0x48f2
    5c4a: b120         	cbz	r0, 0x5c56 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x82> @ imm = #0x8
    5c4c: f004 fc76    	bl	0xa53c <getPmLogContext()> @ imm = #0x48ec
    5c50: 6803         	ldr	r3, [r0]
    5c52: 2b05         	cmp	r3, #0x5
    5c54: dd0e         	ble	0x5c74 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xa0> @ imm = #0x1c
    5c56: f004 fc71    	bl	0xa53c <getPmLogContext()> @ imm = #0x48e2
    5c5a: 4921         	ldr	r1, [pc, #0x84]         @ 0x5ce0 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x10c> literal=0x7e5e
    5c5c: 2200         	movs	r2, #0x0
    5c5e: 4b21         	ldr	r3, [pc, #0x84]         @ 0x5ce4 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x110> literal=0x7e00
    5c60: 9404         	str	r4, [sp, #0x10]
    5c62: 4479         	add	r1, pc => raw=0xdac4 ghidra=0x1dac4 file=56004 "Audio removePipelineInfo umi returnStatus %d"
    5c64: e9cd 2200    	strd	r2, r2, [sp]
    5c68: e9cd 2102    	strd	r2, r1, [sp, #8]
    5c6c: 447b         	add	r3, pc => raw=0xda70 ghidra=0x1da70 file=55920 "AUDIO_REMOVE_PIPELINEINFO_CALL"
    5c6e: 2106         	movs	r1, #0x6
    5c70: f7fe e9d4    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x1c58
    5c74: 4620         	mov	r0, r4
    5c76: b00e         	add	sp, #0x38
    5c78: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
    5c7c: f004 fc5e    	bl	0xa53c <getPmLogContext()> @ imm = #0x48bc
    5c80: 4b19         	ldr	r3, [pc, #0x64]         @ 0x5ce8 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x114> literal=0x7e06
    5c82: 2106         	movs	r1, #0x6
    5c84: 6822         	ldr	r2, [r4]
    5c86: 447b         	add	r3, pc => raw=0xda90 ghidra=0x1da90 file=55952 "Audio removePipelineInfo request for pipelineId:%s"
    5c88: e9cd 3203    	strd	r3, r2, [sp, #12]
    5c8c: 2200         	movs	r2, #0x0
    5c8e: 4b17         	ldr	r3, [pc, #0x5c]         @ 0x5cec <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x118> literal=0x7dd6
    5c90: e9cd 2201    	strd	r2, r2, [sp, #4]
    5c94: 9200         	str	r2, [sp]
    5c96: 447b         	add	r3, pc => raw=0xda70 ghidra=0x1da70 file=55920 "AUDIO_REMOVE_PIPELINEINFO_CALL"
    5c98: f7fe e9c0    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x1c80
    5c9c: 6c2e         	ldr	r6, [r5, #0x40]
    5c9e: 2e00         	cmp	r6, #0x0
    5ca0: d1a9         	bne	0x5bf6 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x22> @ imm = #-0xae
    5ca2: 2402         	movs	r4, #0x2
    5ca4: e7cf         	b	0x5c46 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x72> @ imm = #-0x62
    5ca6: 463b         	mov	r3, r7
    5ca8: 2c00         	cmp	r4, #0x0
    5caa: d0bc         	beq	0x5c26 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x52> @ imm = #-0x88
    5cac: 4638         	mov	r0, r7
    5cae: e006         	b	0x5cbe <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xea> @ imm = #0xc
    5cb0: a907         	add	r1, sp, #0x1c
    5cb2: 4640         	mov	r0, r8
    5cb4: f7fe e9de    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x1c44
    5cb8: 9b07         	ldr	r3, [sp, #0x1c]
    5cba: 9008         	str	r0, [sp, #0x20]
    5cbc: 930a         	str	r3, [sp, #0x28]
    5cbe: 4622         	mov	r2, r4
    5cc0: 4629         	mov	r1, r5
    5cc2: f7fe e962    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x1d3c
    5cc6: e9dd 4307    	ldrd	r4, r3, [sp, #28]
    5cca: e7ac         	b	0x5c26 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x52> @ imm = #-0xa8
    5ccc: 4808         	ldr	r0, [pc, #0x20]         @ 0x5cf0 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x11c> literal=0x79c6
    5cce: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    5cd0: f7fe e818    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x1fd0
    5cd4: 4640         	mov	r0, r8
    5cd6: f7fe ea0c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x1be8
    5cda: f7fe ea52    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x1b5c
    5cde: bf00         	nop
    5ce0: 7e5e         	ldrb	r6, [r3, #0x19]
    5ce2: 0000         	movs	r0, r0
    5ce4: 7e00         	ldrb	r0, [r0, #0x18]
    5ce6: 0000         	movs	r0, r0
    5ce8: 7e06         	ldrb	r6, [r0, #0x18]
    5cea: 0000         	movs	r0, r0
    5cec: 7dd6         	ldrb	r6, [r2, #0x17]
    5cee: 0000         	movs	r0, r0
    5cf0: 79c6         	ldrb	r6, [r0, #0x7]
    5cf2: 0000         	movs	r0, r0

00005cf4 <ConnectionManager::buildAudioStatus(AudioConnection const&)>:
    5cf4: e92d 41f0    	push.w	{r4, r5, r6, r7, r8, lr}
    5cf8: b08a         	sub	sp, #0x28
    5cfa: 4617         	mov	r7, r2
    5cfc: ac01         	add	r4, sp, #0x4
    5cfe: 4606         	mov	r6, r0
    5d00: f7fe e85a    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x1f4c
    5d04: f107 0118    	add.w	r1, r7, #0x18
    5d08: 4620         	mov	r0, r4
    5d0a: f7fe ea10    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x1be0
    5d0e: f646 1373    	movw	r3, #0x6973
    5d12: f10d 0810    	add.w	r8, sp, #0x10
    5d16: f6c6 336e    	movt	r3, #0x6b6e
    5d1a: ad06         	add	r5, sp, #0x18
    5d1c: 9306         	str	r3, [sp, #0x18]
    5d1e: 2304         	movs	r3, #0x4
    5d20: 9305         	str	r3, [sp, #0x14]
    5d22: 4622         	mov	r2, r4
    5d24: 2300         	movs	r3, #0x0
    5d26: 4641         	mov	r1, r8
    5d28: 4630         	mov	r0, r6
    5d2a: 9504         	str	r5, [sp, #0x10]
    5d2c: f88d 301c    	strb.w	r3, [sp, #0x1c]
    5d30: f7fd efd0    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x2060
    5d34: 9804         	ldr	r0, [sp, #0x10]
    5d36: 42a8         	cmp	r0, r5
    5d38: d003         	beq	0x5d42 <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x4e> @ imm = #0x6
    5d3a: 9906         	ldr	r1, [sp, #0x18]
    5d3c: 3101         	adds	r1, #0x1
    5d3e: f7fe e8e0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1e40
    5d42: 4620         	mov	r0, r4
    5d44: f7fe e808    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x1ff0
    5d48: 4639         	mov	r1, r7
    5d4a: 4620         	mov	r0, r4
    5d4c: f7fe e9ee    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x1c24
    5d50: 4b48         	ldr	r3, [pc, #0x120]        @ 0x5e74 <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x180> literal=0x7d92
    5d52: 2206         	movs	r2, #0x6
    5d54: 9205         	str	r2, [sp, #0x14]
    5d56: 2200         	movs	r2, #0x0
    5d58: f88d 201e    	strb.w	r2, [sp, #0x1e]
    5d5c: 4622         	mov	r2, r4
    5d5e: 447b         	add	r3, pc => raw=0xdaf4 ghidra=0x1daf4 file=56052 "source"
    5d60: 9504         	str	r5, [sp, #0x10]
    5d62: e893 0003    	ldm.w	r3, {r0, r1}
    5d66: 9006         	str	r0, [sp, #0x18]
    5d68: 4630         	mov	r0, r6
    5d6a: f8ad 101c    	strh.w	r1, [sp, #0x1c]
    5d6e: 4641         	mov	r1, r8
    5d70: f7fd efb0    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x20a0
    5d74: 9804         	ldr	r0, [sp, #0x10]
    5d76: 42a8         	cmp	r0, r5
    5d78: d003         	beq	0x5d82 <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x8e> @ imm = #0x6
    5d7a: 9906         	ldr	r1, [sp, #0x18]
    5d7c: 3101         	adds	r1, #0x1
    5d7e: f7fe e8c0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1e80
    5d82: 4620         	mov	r0, r4
    5d84: f7fd efe8    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x2030
    5d88: 6b7b         	ldr	r3, [r7, #0x34]
    5d8a: 2b00         	cmp	r3, #0x0
    5d8c: d147         	bne	0x5e1e <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x12a> @ imm = #0x8e
    5d8e: 493a         	ldr	r1, [pc, #0xe8]         @ 0x5e78 <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x184> literal=0x7d66
    5d90: 4620         	mov	r0, r4
    5d92: 4479         	add	r1, pc => raw=0xdafc ghidra=0x1dafc file=56060 "null"
    5d94: f7fe ea84    	blx	0x42a0 <_ZN9umiClient11getSettingsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt+0xc> @ imm = #-0x1af8
    5d98: 4a38         	ldr	r2, [pc, #0xe0]         @ 0x5e7c <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x188> literal=0x7d5a
    5d9a: 462b         	mov	r3, r5
    5d9c: 9504         	str	r5, [sp, #0x10]
    5d9e: f04f 0e0a    	mov.w	lr, #0xa
    5da2: f04f 0c00    	mov.w	r12, #0x0
    5da6: 447a         	add	r2, pc => raw=0xdb04 ghidra=0x1db04 file=56068 "outputMode"
    5da8: ca07         	ldm	r2, {r0, r1, r2}
    5daa: c303         	stm	r3!, {r0, r1}
    5dac: 4641         	mov	r1, r8
    5dae: 801a         	strh	r2, [r3]
    5db0: 4630         	mov	r0, r6
    5db2: 4622         	mov	r2, r4
    5db4: f8cd e014    	str.w	lr, [sp, #0x14]
    5db8: f88d c022    	strb.w	r12, [sp, #0x22]
    5dbc: f7fd ef8a    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x20ec
    5dc0: 9804         	ldr	r0, [sp, #0x10]
    5dc2: 42a8         	cmp	r0, r5
    5dc4: d003         	beq	0x5dce <ConnectionManager::buildAudioStatus(AudioConnection const&)+0xda> @ imm = #0x6
    5dc6: 9906         	ldr	r1, [sp, #0x18]
    5dc8: 3101         	adds	r1, #0x1
    5dca: f7fe e89a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1ecc
    5dce: 4620         	mov	r0, r4
    5dd0: f7fd efc2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x207c
    5dd4: f897 1048    	ldrb.w	r1, [r7, #0x48]
    5dd8: 4620         	mov	r0, r4
    5dda: f7fe e80c    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x1fe8
    5dde: 4b28         	ldr	r3, [pc, #0xa0]         @ 0x5e80 <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x18c> literal=0x7d20
    5de0: 2205         	movs	r2, #0x5
    5de2: 9205         	str	r2, [sp, #0x14]
    5de4: 2200         	movs	r2, #0x0
    5de6: f88d 201d    	strb.w	r2, [sp, #0x1d]
    5dea: 4622         	mov	r2, r4
    5dec: 447b         	add	r3, pc => raw=0xdb10 ghidra=0x1db10 file=56080 "muted"
    5dee: 9504         	str	r5, [sp, #0x10]
    5df0: e893 0003    	ldm.w	r3, {r0, r1}
    5df4: 9006         	str	r0, [sp, #0x18]
    5df6: 4630         	mov	r0, r6
    5df8: f88d 101c    	strb.w	r1, [sp, #0x1c]
    5dfc: 4641         	mov	r1, r8
    5dfe: f7fd ef6a    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x212c
    5e02: 9804         	ldr	r0, [sp, #0x10]
    5e04: 42a8         	cmp	r0, r5
    5e06: d003         	beq	0x5e10 <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x11c> @ imm = #0x6
    5e08: 9906         	ldr	r1, [sp, #0x18]
    5e0a: 3101         	adds	r1, #0x1
    5e0c: f7fe e878    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x1f10
    5e10: 4620         	mov	r0, r4
    5e12: f7fd efa2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x20bc
    5e16: 4630         	mov	r0, r6
    5e18: b00a         	add	sp, #0x28
    5e1a: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
    5e1e: f107 0130    	add.w	r1, r7, #0x30
    5e22: 4620         	mov	r0, r4
    5e24: f7fe e982    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x1cfc
    5e28: 4a16         	ldr	r2, [pc, #0x58]         @ 0x5e84 <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x190> literal=0x7cca
    5e2a: 462b         	mov	r3, r5
    5e2c: 9504         	str	r5, [sp, #0x10]
    5e2e: f04f 0e0a    	mov.w	lr, #0xa
    5e32: f04f 0c00    	mov.w	r12, #0x0
    5e36: 447a         	add	r2, pc => raw=0xdb04 ghidra=0x1db04 file=56068 "outputMode"
    5e38: ca07         	ldm	r2, {r0, r1, r2}
    5e3a: c303         	stm	r3!, {r0, r1}
    5e3c: 4641         	mov	r1, r8
    5e3e: 801a         	strh	r2, [r3]
    5e40: 4630         	mov	r0, r6
    5e42: 4622         	mov	r2, r4
    5e44: f8cd e014    	str.w	lr, [sp, #0x14]
    5e48: f88d c022    	strb.w	r12, [sp, #0x22]
    5e4c: f7fd ef42    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x217c
    5e50: e7b6         	b	0x5dc0 <ConnectionManager::buildAudioStatus(AudioConnection const&)+0xcc> @ imm = #-0x94
    5e52: 4630         	mov	r0, r6
    5e54: f7fd ef80    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x2100
    5e58: f7fe e992    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x1cdc
    5e5c: 4640         	mov	r0, r8
    5e5e: f7fe e948    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x1d70
    5e62: 4620         	mov	r0, r4
    5e64: f7fd ef78    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x2110
    5e68: e7f3         	b	0x5e52 <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x15e> @ imm = #-0x1a
    5e6a: e7f7         	b	0x5e5c <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x168> @ imm = #-0x12
    5e6c: e7f6         	b	0x5e5c <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x168> @ imm = #-0x14
    5e6e: e7f5         	b	0x5e5c <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x168> @ imm = #-0x16
    5e70: e7f4         	b	0x5e5c <ConnectionManager::buildAudioStatus(AudioConnection const&)+0x168> @ imm = #-0x18
    5e72: bf00         	nop
    5e74: 7d92         	ldrb	r2, [r2, #0x16]
    5e76: 0000         	movs	r0, r0
    5e78: 7d66         	ldrb	r6, [r4, #0x15]
    5e7a: 0000         	movs	r0, r0
    5e7c: 7d5a         	ldrb	r2, [r3, #0x15]
    5e7e: 0000         	movs	r0, r0
    5e80: 7d20         	ldrb	r0, [r4, #0x14]
    5e82: 0000         	movs	r0, r0
    5e84: 7cca         	ldrb	r2, [r1, #0x13]
    5e86: 0000         	movs	r0, r0

00005e88 <ConnectionManager::buildStatus()>:
    5e88: e92d 47f0    	push.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    5e8c: 4681         	mov	r9, r0
    5e8e: b08c         	sub	sp, #0x30
    5e90: 2000         	movs	r0, #0x0
    5e92: 460e         	mov	r6, r1
    5e94: f7fe e93e    	blx	0x4114 <jarray_create@plt> @ imm = #-0x1d84
    5e98: 4a49         	ldr	r2, [pc, #0x124]        @ 0x5fc0 <ConnectionManager::buildStatus()+0x138> literal=0x1af1c
    5e9a: 2300         	movs	r3, #0x0
    5e9c: 9301         	str	r3, [sp, #0x4]
    5e9e: 9002         	str	r0, [sp, #0x8]
    5ea0: 447a         	add	r2, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    5ea2: 2800         	cmp	r0, #0x0
    5ea4: d06a         	beq	0x5f7c <ConnectionManager::buildStatus()+0xf4> @ imm = #0xd4
    5ea6: 4b47         	ldr	r3, [pc, #0x11c]        @ 0x5fc4 <ConnectionManager::buildStatus()+0x13c> literal=0x20c
    5ea8: 4648         	mov	r0, r9
    5eaa: f852 a003    	ldr.w	r10, [r2, r3]
    5eae: f10a 0308    	add.w	r3, r10, #0x8
    5eb2: 9300         	str	r3, [sp]
    5eb4: f7fd ef80    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x2100
    5eb8: e9d6 480c    	ldrd	r4, r8, [r6, #48]
    5ebc: 45a0         	cmp	r8, r4
    5ebe: bf08         	it	eq
    5ec0: 466f         	moveq	r7, sp
    5ec2: d010         	beq	0x5ee6 <ConnectionManager::buildStatus()+0x5e> @ imm = #0x20
    5ec4: ad06         	add	r5, sp, #0x18
    5ec6: 466f         	mov	r7, sp
    5ec8: 4622         	mov	r2, r4
    5eca: 4631         	mov	r1, r6
    5ecc: 4628         	mov	r0, r5
    5ece: f7ff ff11    	bl	0x5cf4 <ConnectionManager::buildAudioStatus(AudioConnection const&)> @ imm = #-0x1de
    5ed2: 4629         	mov	r1, r5
    5ed4: 4638         	mov	r0, r7
    5ed6: f7fd ef52    	blx	0x3d7c <_ZN7pbnjson6JValue6appendERKS0_@plt> @ imm = #-0x215c
    5eda: 4628         	mov	r0, r5
    5edc: 344c         	adds	r4, #0x4c
    5ede: f7fd ef3c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x2188
    5ee2: 45a0         	cmp	r8, r4
    5ee4: d1f0         	bne	0x5ec8 <ConnectionManager::buildStatus()+0x40> @ imm = #-0x20
    5ee6: ae03         	add	r6, sp, #0xc
    5ee8: 2101         	movs	r1, #0x1
    5eea: 4630         	mov	r0, r6
    5eec: f7fd ef82    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x20fc
    5ef0: 4a35         	ldr	r2, [pc, #0xd4]         @ 0x5fc8 <ConnectionManager::buildStatus()+0x140> literal=0x7822
    5ef2: ac08         	add	r4, sp, #0x20
    5ef4: 4623         	mov	r3, r4
    5ef6: 9406         	str	r4, [sp, #0x18]
    5ef8: ad06         	add	r5, sp, #0x18
    5efa: f04f 0e0b    	mov.w	lr, #0xb
    5efe: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    5f00: f04f 0c00    	mov.w	r12, #0x0
    5f04: ca07         	ldm	r2, {r0, r1, r2}
    5f06: c303         	stm	r3!, {r0, r1}
    5f08: 4629         	mov	r1, r5
    5f0a: f823 2b02    	strh	r2, [r3], #2
    5f0e: 0c12         	lsrs	r2, r2, #0x10
    5f10: 4648         	mov	r0, r9
    5f12: 701a         	strb	r2, [r3]
    5f14: 4632         	mov	r2, r6
    5f16: f8cd e01c    	str.w	lr, [sp, #0x1c]
    5f1a: f88d c02b    	strb.w	r12, [sp, #0x2b]
    5f1e: f7fd eeda    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x224c
    5f22: 9806         	ldr	r0, [sp, #0x18]
    5f24: 42a0         	cmp	r0, r4
    5f26: d003         	beq	0x5f30 <ConnectionManager::buildStatus()+0xa8> @ imm = #0x6
    5f28: 9908         	ldr	r1, [sp, #0x20]
    5f2a: 3101         	adds	r1, #0x1
    5f2c: f7fd efe8    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2030
    5f30: 4630         	mov	r0, r6
    5f32: f7fd ef12    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x21dc
    5f36: 4b25         	ldr	r3, [pc, #0x94]         @ 0x5fcc <ConnectionManager::buildStatus()+0x144> literal=0x7bd0
    5f38: 2205         	movs	r2, #0x5
    5f3a: 9207         	str	r2, [sp, #0x1c]
    5f3c: 2200         	movs	r2, #0x0
    5f3e: f88d 2025    	strb.w	r2, [sp, #0x25]
    5f42: 463a         	mov	r2, r7
    5f44: 447b         	add	r3, pc => raw=0xdb18 ghidra=0x1db18 file=56088 "audio"
    5f46: 9406         	str	r4, [sp, #0x18]
    5f48: e893 0003    	ldm.w	r3, {r0, r1}
    5f4c: 9008         	str	r0, [sp, #0x20]
    5f4e: 4648         	mov	r0, r9
    5f50: f88d 1024    	strb.w	r1, [sp, #0x24]
    5f54: 4629         	mov	r1, r5
    5f56: f7fd eebe    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x2284
    5f5a: 9806         	ldr	r0, [sp, #0x18]
    5f5c: 42a0         	cmp	r0, r4
    5f5e: d003         	beq	0x5f68 <ConnectionManager::buildStatus()+0xe0> @ imm = #0x6
    5f60: 9908         	ldr	r1, [sp, #0x20]
    5f62: 3101         	adds	r1, #0x1
    5f64: f7fd efcc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2068
    5f68: f10a 0308    	add.w	r3, r10, #0x8
    5f6c: 4638         	mov	r0, r7
    5f6e: 9300         	str	r3, [sp]
    5f70: f7fd efec    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x2028
    5f74: 4648         	mov	r0, r9
    5f76: b00c         	add	sp, #0x30
    5f78: e8bd 87f0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, pc}
    5f7c: 4b14         	ldr	r3, [pc, #0x50]         @ 0x5fd0 <ConnectionManager::buildStatus()+0x148> literal=0x230
    5f7e: 58d3         	ldr	r3, [r2, r3]
    5f80: 689b         	ldr	r3, [r3, #0x8]
    5f82: 9302         	str	r3, [sp, #0x8]
    5f84: e78f         	b	0x5ea6 <ConnectionManager::buildStatus()+0x1e> @ imm = #-0xe2
    5f86: 4628         	mov	r0, r5
    5f88: f7fe e8b2    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x1e9c
    5f8c: 4648         	mov	r0, r9
    5f8e: f7fd eee4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x2238
    5f92: f10a 0308    	add.w	r3, r10, #0x8
    5f96: 4638         	mov	r0, r7
    5f98: 9300         	str	r3, [sp]
    5f9a: f7fd efd8    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x2050
    5f9e: f7fe e8f0    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x1e20
    5fa2: 4628         	mov	r0, r5
    5fa4: f7fe e8a4    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x1eb8
    5fa8: 4630         	mov	r0, r6
    5faa: f7fd eed6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x2254
    5fae: e7ed         	b	0x5f8c <ConnectionManager::buildStatus()+0x104> @ imm = #-0x26
    5fb0: 4628         	mov	r0, r5
    5fb2: f7fd eed2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x225c
    5fb6: e7e9         	b	0x5f8c <ConnectionManager::buildStatus()+0x104> @ imm = #-0x2e
    5fb8: e7e8         	b	0x5f8c <ConnectionManager::buildStatus()+0x104> @ imm = #-0x30
    5fba: 466f         	mov	r7, sp
    5fbc: e7e9         	b	0x5f92 <ConnectionManager::buildStatus()+0x10a> @ imm = #-0x2e
    5fbe: bf00         	nop
    5fc0: af1c         	add	r7, sp, #0x70
    5fc2: 0001         	movs	r1, r0
    5fc4: 020c         	lsls	r4, r1, #0x8
    5fc6: 0000         	movs	r0, r0
    5fc8: 7822         	ldrb	r2, [r4]
    5fca: 0000         	movs	r0, r0
    5fcc: 7bd0         	ldrb	r0, [r2, #0xf]
    5fce: 0000         	movs	r0, r0
    5fd0: 0230         	lsls	r0, r6, #0x8
    5fd2: 0000         	movs	r0, r0

00005fd4 <ConnectionManager::getStatus()>:
    5fd4: b570         	push	{r4, r5, r6, lr}
    5fd6: b08c         	sub	sp, #0x30
    5fd8: 4604         	mov	r4, r0
    5fda: 460d         	mov	r5, r1
    5fdc: f004 faae    	bl	0xa53c <getPmLogContext()> @ imm = #0x455c
    5fe0: b398         	cbz	r0, 0x604a <ConnectionManager::getStatus()+0x76> @ imm = #0x66
    5fe2: f004 faab    	bl	0xa53c <getPmLogContext()> @ imm = #0x4556
    5fe6: 6803         	ldr	r3, [r0]
    5fe8: 2b05         	cmp	r3, #0x5
    5fea: dc2e         	bgt	0x604a <ConnectionManager::getStatus()+0x76> @ imm = #0x5c
    5fec: 4629         	mov	r1, r5
    5fee: 4620         	mov	r0, r4
    5ff0: f7ff ff4a    	bl	0x5e88 <ConnectionManager::buildStatus()> @ imm = #-0x16c
    5ff4: f004 faa2    	bl	0xa53c <getPmLogContext()> @ imm = #0x4544
    5ff8: b120         	cbz	r0, 0x6004 <ConnectionManager::getStatus()+0x30> @ imm = #0x8
    5ffa: f004 fa9f    	bl	0xa53c <getPmLogContext()> @ imm = #0x453e
    5ffe: 6803         	ldr	r3, [r0]
    6000: 2b05         	cmp	r3, #0x5
    6002: dd1f         	ble	0x6044 <ConnectionManager::getStatus()+0x70> @ imm = #0x3e
    6004: f004 fa9a    	bl	0xa53c <getPmLogContext()> @ imm = #0x4534
    6008: ae06         	add	r6, sp, #0x18
    600a: 4605         	mov	r5, r0
    600c: 2200         	movs	r2, #0x0
    600e: 4621         	mov	r1, r4
    6010: 4630         	mov	r0, r6
    6012: f7fe e8ec    	blx	0x41ec <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt> @ imm = #-0x1e28
    6016: 4b19         	ldr	r3, [pc, #0x64]         @ 0x607c <ConnectionManager::getStatus()+0xa8> literal=0x7b2e
    6018: 4628         	mov	r0, r5
    601a: 9a06         	ldr	r2, [sp, #0x18]
    601c: 2106         	movs	r1, #0x6
    601e: 447b         	add	r3, pc => raw=0xdb50 ghidra=0x1db50 file=56144 "Audio getStatus response:%s"
    6020: e9cd 3203    	strd	r3, r2, [sp, #12]
    6024: 2200         	movs	r2, #0x0
    6026: 4b16         	ldr	r3, [pc, #0x58]         @ 0x6080 <ConnectionManager::getStatus()+0xac> literal=0x7aee
    6028: e9cd 2201    	strd	r2, r2, [sp, #4]
    602c: 9200         	str	r2, [sp]
    602e: 447b         	add	r3, pc => raw=0xdb20 ghidra=0x1db20 file=56096 "AUDIO_GET_STATUS_CALL"
    6030: f7fd eff4    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x2018
    6034: 9806         	ldr	r0, [sp, #0x18]
    6036: ab08         	add	r3, sp, #0x20
    6038: 4298         	cmp	r0, r3
    603a: d003         	beq	0x6044 <ConnectionManager::getStatus()+0x70> @ imm = #0x6
    603c: 9908         	ldr	r1, [sp, #0x20]
    603e: 3101         	adds	r1, #0x1
    6040: f7fd ef5e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2144
    6044: 4620         	mov	r0, r4
    6046: b00c         	add	sp, #0x30
    6048: bd70         	pop	{r4, r5, r6, pc}
    604a: f004 fa77    	bl	0xa53c <getPmLogContext()> @ imm = #0x44ee
    604e: 4b0d         	ldr	r3, [pc, #0x34]         @ 0x6084 <ConnectionManager::getStatus()+0xb0> literal=0x7ada
    6050: 2200         	movs	r2, #0x0
    6052: 2106         	movs	r1, #0x6
    6054: e9cd 2201    	strd	r2, r2, [sp, #4]
    6058: 9200         	str	r2, [sp]
    605a: 447b         	add	r3, pc => raw=0xdb38 ghidra=0x1db38 file=56120 "Audio getStatus request"
    605c: 9303         	str	r3, [sp, #0xc]
    605e: 4b0a         	ldr	r3, [pc, #0x28]         @ 0x6088 <ConnectionManager::getStatus()+0xb4> literal=0x7abc
    6060: 447b         	add	r3, pc => raw=0xdb20 ghidra=0x1db20 file=56096 "AUDIO_GET_STATUS_CALL"
    6062: f7fd efdc    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x2048
    6066: e7c1         	b	0x5fec <ConnectionManager::getStatus()+0x18> @ imm = #-0x7e
    6068: 4630         	mov	r0, r6
    606a: f7fe e842    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x1f7c
    606e: 4620         	mov	r0, r4
    6070: f7fd ee72    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x231c
    6074: f7fe e884    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x1ef8
    6078: e7f9         	b	0x606e <ConnectionManager::getStatus()+0x9a> @ imm = #-0xe
    607a: bf00         	nop
    607c: 7b2e         	ldrb	r6, [r5, #0xc]
    607e: 0000         	movs	r0, r0
    6080: 7aee         	ldrb	r6, [r5, #0xb]
    6082: 0000         	movs	r0, r0
    6084: 7ada         	ldrb	r2, [r3, #0xb]
    6086: 0000         	movs	r0, r0
    6088: 7abc         	ldrb	r4, [r7, #0xa]
    608a: 0000         	movs	r0, r0

0000608c <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)>:
    608c: e92d 41f0    	push.w	{r4, r5, r6, r7, r8, lr}
    6090: e9d0 460c    	ldrd	r4, r6, [r0, #48]
    6094: 42a6         	cmp	r6, r4
    6096: d01f         	beq	0x60d8 <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x4c> @ imm = #0x3e
    6098: e9d1 8500    	ldrd	r8, r5, [r1]
    609c: 4617         	mov	r7, r2
    609e: e002         	b	0x60a6 <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1a> @ imm = #0x4
    60a0: 344c         	adds	r4, #0x4c
    60a2: 42a6         	cmp	r6, r4
    60a4: d018         	beq	0x60d8 <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x4c> @ imm = #0x30
    60a6: 6863         	ldr	r3, [r4, #0x4]
    60a8: 42ab         	cmp	r3, r5
    60aa: d1f9         	bne	0x60a0 <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x14> @ imm = #-0xe
    60ac: 462a         	mov	r2, r5
    60ae: 4641         	mov	r1, r8
    60b0: b125         	cbz	r5, 0x60bc <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x30> @ imm = #0x8
    60b2: 6820         	ldr	r0, [r4]
    60b4: f7fe e84c    	blx	0x4150 <memcmp@plt>     @ imm = #-0x1f68
    60b8: 2800         	cmp	r0, #0x0
    60ba: d1f1         	bne	0x60a0 <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x14> @ imm = #-0x1e
    60bc: 69e2         	ldr	r2, [r4, #0x1c]
    60be: 687b         	ldr	r3, [r7, #0x4]
    60c0: 429a         	cmp	r2, r3
    60c2: d1ed         	bne	0x60a0 <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x14> @ imm = #-0x26
    60c4: b12a         	cbz	r2, 0x60d2 <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x46> @ imm = #0xa
    60c6: 6839         	ldr	r1, [r7]
    60c8: 69a0         	ldr	r0, [r4, #0x18]
    60ca: f7fe e842    	blx	0x4150 <memcmp@plt>     @ imm = #-0x1f7c
    60ce: 2800         	cmp	r0, #0x0
    60d0: d1e6         	bne	0x60a0 <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x14> @ imm = #-0x34
    60d2: 4620         	mov	r0, r4
    60d4: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
    60d8: 2400         	movs	r4, #0x0
    60da: 4620         	mov	r0, r4
    60dc: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}

000060e0 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)>:
    60e0: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    60e4: b083         	sub	sp, #0xc
    60e6: e9d0 460c    	ldrd	r4, r6, [r0, #48]
    60ea: 42b4         	cmp	r4, r6
    60ec: f000 810a    	beq.w	0x6304 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x224> @ imm = #0x214
    60f0: e9d1 a500    	ldrd	r10, r5, [r1]
    60f4: 4680         	mov	r8, r0
    60f6: 4691         	mov	r9, r2
    60f8: e003         	b	0x6102 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x22> @ imm = #0x6
    60fa: 344c         	adds	r4, #0x4c
    60fc: 42b4         	cmp	r4, r6
    60fe: f000 8101    	beq.w	0x6304 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x224> @ imm = #0x202
    6102: 6863         	ldr	r3, [r4, #0x4]
    6104: 42ab         	cmp	r3, r5
    6106: d1f8         	bne	0x60fa <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1a> @ imm = #-0x10
    6108: 6827         	ldr	r7, [r4]
    610a: 462a         	mov	r2, r5
    610c: 4651         	mov	r1, r10
    610e: b125         	cbz	r5, 0x611a <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x3a> @ imm = #0x8
    6110: 4638         	mov	r0, r7
    6112: f7fe e81e    	blx	0x4150 <memcmp@plt>     @ imm = #-0x1fc4
    6116: 2800         	cmp	r0, #0x0
    6118: d1ef         	bne	0x60fa <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1a> @ imm = #-0x22
    611a: 69e2         	ldr	r2, [r4, #0x1c]
    611c: f8d9 3004    	ldr.w	r3, [r9, #0x4]
    6120: 429a         	cmp	r2, r3
    6122: d1ea         	bne	0x60fa <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1a> @ imm = #-0x2c
    6124: 69a0         	ldr	r0, [r4, #0x18]
    6126: f8d9 1000    	ldr.w	r1, [r9]
    612a: b11a         	cbz	r2, 0x6134 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x54> @ imm = #0x6
    612c: f7fe e810    	blx	0x4150 <memcmp@plt>     @ imm = #-0x1fe0
    6130: 2800         	cmp	r0, #0x0
    6132: d1e2         	bne	0x60fa <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1a> @ imm = #-0x3c
    6134: f104 0a4c    	add.w	r10, r4, #0x4c
    6138: 45b2         	cmp	r10, r6
    613a: f000 80c3    	beq.w	0x62c4 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1e4> @ imm = #0x186
    613e: eba6 030a    	sub.w	r3, r6, r10
    6142: f64c 221b    	movw	r2, #0xca1b
    6146: ea4f 0ba3    	asr.w	r11, r3, #0x2
    614a: 2b00         	cmp	r3, #0x0
    614c: f6c2 026b    	movt	r2, #0x286b
    6150: fb02 fb0b    	mul	r11, r2, r11
    6154: f340 80b6    	ble.w	0x62c4 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1e4> @ imm = #0x16c
    6158: f104 0598    	add.w	r5, r4, #0x98
    615c: f104 036c    	add.w	r3, r4, #0x6c
    6160: f104 0984    	add.w	r9, r4, #0x84
    6164: 2600         	movs	r6, #0x0
    6166: e071         	b	0x624c <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x16c> @ imm = #0xe2
    6168: f855 cc48    	ldr	r12, [r5, #-72]
    616c: f104 0208    	add.w	r2, r4, #0x8
    6170: 4297         	cmp	r7, r2
    6172: e945 1c26    	strd	r1, r12, [r5, #-152]
    6176: f000 80e4    	beq.w	0x6342 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x262> @ imm = #0x1c8
    617a: f855 1c44    	ldr	r1, [r5, #-68]
    617e: f855 2c90    	ldr	r2, [r5, #-144]
    6182: f845 1c90    	str	r1, [r5, #-144]
    6186: 2f00         	cmp	r7, #0x0
    6188: f000 80df    	beq.w	0x634a <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x26a> @ imm = #0x1be
    618c: f845 2c44    	str	r2, [r5, #-68]
    6190: f845 7c4c    	str	r7, [r5, #-76]
    6194: f845 6c48    	str	r6, [r5, #-72]
    6198: 703e         	strb	r6, [r7]
    619a: f855 1c34    	ldr	r1, [r5, #-52]
    619e: f855 0c80    	ldr	r0, [r5, #-128]
    61a2: 4299         	cmp	r1, r3
    61a4: 4607         	mov	r7, r0
    61a6: d074         	beq	0x6292 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1b2> @ imm = #0xe8
    61a8: f104 0220    	add.w	r2, r4, #0x20
    61ac: f855 cc30    	ldr	r12, [r5, #-48]
    61b0: 4290         	cmp	r0, r2
    61b2: f855 2c2c    	ldr	r2, [r5, #-44]
    61b6: e945 1c20    	strd	r1, r12, [r5, #-128]
    61ba: bf08         	it	eq
    61bc: f845 2c78    	streq	r2, [r5, #-120]
    61c0: f000 80c7    	beq.w	0x6352 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x272> @ imm = #0x18e
    61c4: f855 1c78    	ldr	r1, [r5, #-120]
    61c8: f845 2c78    	str	r2, [r5, #-120]
    61cc: 2800         	cmp	r0, #0x0
    61ce: f000 80c0    	beq.w	0x6352 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x272> @ imm = #0x180
    61d2: f845 0c34    	str	r0, [r5, #-52]
    61d6: f845 1c2c    	str	r1, [r5, #-44]
    61da: f845 6c30    	str	r6, [r5, #-48]
    61de: 703e         	strb	r6, [r7]
    61e0: f855 2c1c    	ldr	r2, [r5, #-28]
    61e4: f855 0c68    	ldr	r0, [r5, #-104]
    61e8: 454a         	cmp	r2, r9
    61ea: 4601         	mov	r1, r0
    61ec: bf18         	it	ne
    61ee: 3438         	addne	r4, #0x38
    61f0: f000 8093    	beq.w	0x631a <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x23a> @ imm = #0x126
    61f4: f855 7c18    	ldr	r7, [r5, #-24]
    61f8: 42a0         	cmp	r0, r4
    61fa: e945 271a    	strd	r2, r7, [r5, #-104]
    61fe: f855 2c14    	ldr	r2, [r5, #-20]
    6202: bf08         	it	eq
    6204: f845 2c60    	streq	r2, [r5, #-96]
    6208: f000 80a7    	beq.w	0x635a <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x27a> @ imm = #0x14e
    620c: f855 4c60    	ldr	r4, [r5, #-96]
    6210: f845 2c60    	str	r2, [r5, #-96]
    6214: 2800         	cmp	r0, #0x0
    6216: f000 80a0    	beq.w	0x635a <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x27a> @ imm = #0x140
    621a: f845 0c1c    	str	r0, [r5, #-28]
    621e: f845 4c14    	str	r4, [r5, #-20]
    6222: f845 6c18    	str	r6, [r5, #-24]
    6226: f1bb 0b01    	subs.w	r11, r11, #0x1
    622a: 700e         	strb	r6, [r1]
    622c: f103 034c    	add.w	r3, r3, #0x4c
    6230: f815 2c04    	ldrb	r2, [r5, #-4]
    6234: f109 094c    	add.w	r9, r9, #0x4c
    6238: 4654         	mov	r4, r10
    623a: f105 054c    	add.w	r5, r5, #0x4c
    623e: f805 2c9c    	strb	r2, [r5, #-156]
    6242: d03d         	beq	0x62c0 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1e0> @ imm = #0x7a
    6244: f855 7c98    	ldr	r7, [r5, #-152]
    6248: f10a 0a4c    	add.w	r10, r10, #0x4c
    624c: f855 1c4c    	ldr	r1, [r5, #-76]
    6250: f104 0054    	add.w	r0, r4, #0x54
    6254: 4281         	cmp	r1, r0
    6256: d187         	bne	0x6168 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x88> @ imm = #-0xf2
    6258: f855 2c48    	ldr	r2, [r5, #-72]
    625c: b152         	cbz	r2, 0x6274 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x194> @ imm = #0x14
    625e: 2a01         	cmp	r2, #0x1
    6260: d07f         	beq	0x6362 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x282> @ imm = #0xfe
    6262: 4638         	mov	r0, r7
    6264: 9301         	str	r3, [sp, #0x4]
    6266: f7fd ee90    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x22e0
    626a: f855 2c48    	ldr	r2, [r5, #-72]
    626e: f855 7c98    	ldr	r7, [r5, #-152]
    6272: 9b01         	ldr	r3, [sp, #0x4]
    6274: f845 2c94    	str	r2, [r5, #-148]
    6278: 54be         	strb	r6, [r7, r2]
    627a: f855 2c4c    	ldr	r2, [r5, #-76]
    627e: f845 6c48    	str	r6, [r5, #-72]
    6282: 7016         	strb	r6, [r2]
    6284: f855 1c34    	ldr	r1, [r5, #-52]
    6288: f855 0c80    	ldr	r0, [r5, #-128]
    628c: 428b         	cmp	r3, r1
    628e: 4607         	mov	r7, r0
    6290: d18a         	bne	0x61a8 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xc8> @ imm = #-0xec
    6292: f855 2c30    	ldr	r2, [r5, #-48]
    6296: b152         	cbz	r2, 0x62ae <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1ce> @ imm = #0x14
    6298: 2a01         	cmp	r2, #0x1
    629a: d06a         	beq	0x6372 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x292> @ imm = #0xd4
    629c: 4619         	mov	r1, r3
    629e: 9301         	str	r3, [sp, #0x4]
    62a0: f7fd ee72    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x231c
    62a4: f855 2c30    	ldr	r2, [r5, #-48]
    62a8: f855 0c80    	ldr	r0, [r5, #-128]
    62ac: 9b01         	ldr	r3, [sp, #0x4]
    62ae: f845 2c7c    	str	r2, [r5, #-124]
    62b2: 5486         	strb	r6, [r0, r2]
    62b4: f855 2c34    	ldr	r2, [r5, #-52]
    62b8: f845 6c30    	str	r6, [r5, #-48]
    62bc: 7016         	strb	r6, [r2]
    62be: e78f         	b	0x61e0 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x100> @ imm = #-0xe2
    62c0: f8d8 6034    	ldr.w	r6, [r8, #0x34]
    62c4: f856 0c1c    	ldr	r0, [r6, #-28]
    62c8: f1a6 0314    	sub.w	r3, r6, #0x14
    62cc: f1a6 024c    	sub.w	r2, r6, #0x4c
    62d0: f8c8 2034    	str.w	r2, [r8, #0x34]
    62d4: 4298         	cmp	r0, r3
    62d6: d004         	beq	0x62e2 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x202> @ imm = #0x8
    62d8: f856 1c14    	ldr	r1, [r6, #-20]
    62dc: 3101         	adds	r1, #0x1
    62de: f7fd ee10    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x23e0
    62e2: f856 0c34    	ldr	r0, [r6, #-52]
    62e6: f1a6 032c    	sub.w	r3, r6, #0x2c
    62ea: 4298         	cmp	r0, r3
    62ec: d004         	beq	0x62f8 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x218> @ imm = #0x8
    62ee: f856 1c2c    	ldr	r1, [r6, #-44]
    62f2: 3101         	adds	r1, #0x1
    62f4: f7fd ee04    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x23f8
    62f8: f856 0c4c    	ldr	r0, [r6, #-76]
    62fc: f1a6 0344    	sub.w	r3, r6, #0x44
    6300: 4298         	cmp	r0, r3
    6302: d102         	bne	0x630a <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x22a> @ imm = #0x4
    6304: b003         	add	sp, #0xc
    6306: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    630a: f856 1c44    	ldr	r1, [r6, #-68]
    630e: 3101         	adds	r1, #0x1
    6310: b003         	add	sp, #0xc
    6312: e8bd 4ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    6316: f7fd bdf1    	b.w	0x3efc <_ZN7pbnjson7JSchemaC1ERKS0_@plt+0xc> @ imm = #-0x241e
    631a: f855 2c18    	ldr	r2, [r5, #-24]
    631e: b152         	cbz	r2, 0x6336 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x256> @ imm = #0x14
    6320: 2a01         	cmp	r2, #0x1
    6322: d02e         	beq	0x6382 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x2a2> @ imm = #0x5c
    6324: 4649         	mov	r1, r9
    6326: 9301         	str	r3, [sp, #0x4]
    6328: f7fd ee2e    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x23a4
    632c: f855 2c18    	ldr	r2, [r5, #-24]
    6330: f855 0c68    	ldr	r0, [r5, #-104]
    6334: 9b01         	ldr	r3, [sp, #0x4]
    6336: f845 2c64    	str	r2, [r5, #-100]
    633a: 5486         	strb	r6, [r0, r2]
    633c: f855 1c1c    	ldr	r1, [r5, #-28]
    6340: e76f         	b	0x6222 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x142> @ imm = #-0x122
    6342: f855 2c44    	ldr	r2, [r5, #-68]
    6346: f845 2c90    	str	r2, [r5, #-144]
    634a: 4607         	mov	r7, r0
    634c: f845 0c4c    	str	r0, [r5, #-76]
    6350: e720         	b	0x6194 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xb4> @ imm = #-0x1c0
    6352: 461f         	mov	r7, r3
    6354: f845 3c34    	str	r3, [r5, #-52]
    6358: e73f         	b	0x61da <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xfa> @ imm = #-0x182
    635a: 4649         	mov	r1, r9
    635c: f845 9c1c    	str	r9, [r5, #-28]
    6360: e75f         	b	0x6222 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x142> @ imm = #-0x142
    6362: f815 2c44    	ldrb	r2, [r5, #-68]
    6366: 703a         	strb	r2, [r7]
    6368: f855 2c48    	ldr	r2, [r5, #-72]
    636c: f855 7c98    	ldr	r7, [r5, #-152]
    6370: e780         	b	0x6274 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x194> @ imm = #-0x100
    6372: f815 2c2c    	ldrb	r2, [r5, #-44]
    6376: 7002         	strb	r2, [r0]
    6378: f855 2c30    	ldr	r2, [r5, #-48]
    637c: f855 0c80    	ldr	r0, [r5, #-128]
    6380: e795         	b	0x62ae <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1ce> @ imm = #-0xd6
    6382: f899 2000    	ldrb.w	r2, [r9]
    6386: 7002         	strb	r2, [r0]
    6388: f855 2c18    	ldr	r2, [r5, #-24]
    638c: f855 0c68    	ldr	r0, [r5, #-104]
    6390: e7d1         	b	0x6336 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x256> @ imm = #-0x5e
    6392: bf00         	nop

00006394 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)>:
    6394: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    6398: b095         	sub	sp, #0x54
    639a: 4606         	mov	r6, r0
    639c: 460c         	mov	r4, r1
    639e: 4615         	mov	r5, r2
    63a0: f004 f8cc    	bl	0xa53c <getPmLogContext()> @ imm = #0x4198
    63a4: 2800         	cmp	r0, #0x0
    63a6: d074         	beq	0x6492 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xfe> @ imm = #0xe8
    63a8: f004 f8c8    	bl	0xa53c <getPmLogContext()> @ imm = #0x4190
    63ac: 6803         	ldr	r3, [r0]
    63ae: 2b05         	cmp	r3, #0x5
    63b0: dc6f         	bgt	0x6492 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xfe> @ imm = #0xde
    63b2: 6c37         	ldr	r7, [r6, #0x40]
    63b4: 2f00         	cmp	r7, #0x0
    63b6: f000 8082    	beq.w	0x64be <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x12a> @ imm = #0x104
    63ba: e9d4 b400    	ldrd	r11, r4, [r4]
    63be: f10d 0828    	add.w	r8, sp, #0x28
    63c2: f10d 0a20    	add.w	r10, sp, #0x20
    63c6: f8cd 8020    	str.w	r8, [sp, #0x20]
    63ca: 4263         	rsbs	r3, r4, #0
    63cc: fabb f68b    	clz	r6, r11
    63d0: 0976         	lsrs	r6, r6, #0x5
    63d2: 459b         	cmp	r11, r3
    63d4: bf08         	it	eq
    63d6: 2600         	moveq	r6, #0x0
    63d8: 2e00         	cmp	r6, #0x0
    63da: f040 80a4    	bne.w	0x6526 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x192> @ imm = #0x148
    63de: 2c0f         	cmp	r4, #0xf
    63e0: 940e         	str	r4, [sp, #0x38]
    63e2: d86e         	bhi	0x64c2 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x12e> @ imm = #0xdc
    63e4: 2c01         	cmp	r4, #0x1
    63e6: d17d         	bne	0x64e4 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x150> @ imm = #0xfa
    63e8: f89b 3000    	ldrb.w	r3, [r11]
    63ec: 4642         	mov	r2, r8
    63ee: f10d 0938    	add.w	r9, sp, #0x38
    63f2: f88d 3028    	strb.w	r3, [sp, #0x28]
    63f6: 2300         	movs	r3, #0x0
    63f8: 9409         	str	r4, [sp, #0x24]
    63fa: 5513         	strb	r3, [r2, r4]
    63fc: e9d5 b400    	ldrd	r11, r4, [r5]
    6400: ad10         	add	r5, sp, #0x40
    6402: 950e         	str	r5, [sp, #0x38]
    6404: 4263         	rsbs	r3, r4, #0
    6406: fabb f68b    	clz	r6, r11
    640a: 0976         	lsrs	r6, r6, #0x5
    640c: 459b         	cmp	r11, r3
    640e: bf08         	it	eq
    6410: 2600         	moveq	r6, #0x0
    6412: 2e00         	cmp	r6, #0x0
    6414: f040 8083    	bne.w	0x651e <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x18a> @ imm = #0x106
    6418: 2c0f         	cmp	r4, #0xf
    641a: 9407         	str	r4, [sp, #0x1c]
    641c: d870         	bhi	0x6500 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x16c> @ imm = #0xe0
    641e: 2c01         	cmp	r4, #0x1
    6420: d169         	bne	0x64f6 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x162> @ imm = #0xd2
    6422: f89b 3000    	ldrb.w	r3, [r11]
    6426: 462a         	mov	r2, r5
    6428: f88d 3040    	strb.w	r3, [sp, #0x40]
    642c: 2300         	movs	r3, #0x0
    642e: 940f         	str	r4, [sp, #0x3c]
    6430: 4638         	mov	r0, r7
    6432: 5513         	strb	r3, [r2, r4]
    6434: 4651         	mov	r1, r10
    6436: 464a         	mov	r2, r9
    6438: f7fd ee60    	blx	0x40fc <_ZN9umiClient15disconnectInputENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_@plt> @ imm = #-0x2340
    643c: 9b0e         	ldr	r3, [sp, #0x38]
    643e: 4604         	mov	r4, r0
    6440: 42ab         	cmp	r3, r5
    6442: d004         	beq	0x644e <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xba> @ imm = #0x8
    6444: 9910         	ldr	r1, [sp, #0x40]
    6446: 4618         	mov	r0, r3
    6448: 3101         	adds	r1, #0x1
    644a: f7fd ed5a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x254c
    644e: 9808         	ldr	r0, [sp, #0x20]
    6450: 4540         	cmp	r0, r8
    6452: d003         	beq	0x645c <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xc8> @ imm = #0x6
    6454: 990a         	ldr	r1, [sp, #0x28]
    6456: 3101         	adds	r1, #0x1
    6458: f7fd ed52    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x255c
    645c: f004 f86e    	bl	0xa53c <getPmLogContext()> @ imm = #0x40dc
    6460: b120         	cbz	r0, 0x646c <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xd8> @ imm = #0x8
    6462: f004 f86b    	bl	0xa53c <getPmLogContext()> @ imm = #0x40d6
    6466: 6803         	ldr	r3, [r0]
    6468: 2b05         	cmp	r3, #0x5
    646a: dd0e         	ble	0x648a <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xf6> @ imm = #0x1c
    646c: f004 f866    	bl	0xa53c <getPmLogContext()> @ imm = #0x40cc
    6470: 4935         	ldr	r1, [pc, #0xd4]         @ 0x6548 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1b4> literal=0x7738
    6472: 2200         	movs	r2, #0x0
    6474: 4b35         	ldr	r3, [pc, #0xd4]         @ 0x654c <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1b8> literal=0x76e6
    6476: 9404         	str	r4, [sp, #0x10]
    6478: 4479         	add	r1, pc => raw=0xdbb4 ghidra=0x1dbb4 file=56244 "Audio disconnect umi returnStatus %d"
    647a: e9cd 2200    	strd	r2, r2, [sp]
    647e: e9cd 2102    	strd	r2, r1, [sp, #8]
    6482: 447b         	add	r3, pc => raw=0xdb6c ghidra=0x1db6c file=56172 "AUDIO_DISCONNECT_CALL"
    6484: 2106         	movs	r1, #0x6
    6486: f7fd edca    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x246c
    648a: 4620         	mov	r0, r4
    648c: b015         	add	sp, #0x54
    648e: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    6492: f004 f853    	bl	0xa53c <getPmLogContext()> @ imm = #0x40a6
    6496: 682b         	ldr	r3, [r5]
    6498: 2200         	movs	r2, #0x0
    649a: 2106         	movs	r1, #0x6
    649c: 9305         	str	r3, [sp, #0x14]
    649e: 6823         	ldr	r3, [r4]
    64a0: e9cd 2201    	strd	r2, r2, [sp, #4]
    64a4: 9200         	str	r2, [sp]
    64a6: 9304         	str	r3, [sp, #0x10]
    64a8: 4b29         	ldr	r3, [pc, #0xa4]         @ 0x6550 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1bc> literal=0x76d6
    64aa: 447b         	add	r3, pc => raw=0xdb84 ghidra=0x1db84 file=56196 "Audio disconnect request for source:%s, sink:%s"
    64ac: 9303         	str	r3, [sp, #0xc]
    64ae: 4b29         	ldr	r3, [pc, #0xa4]         @ 0x6554 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1c0> literal=0x76b8
    64b0: 447b         	add	r3, pc => raw=0xdb6c ghidra=0x1db6c file=56172 "AUDIO_DISCONNECT_CALL"
    64b2: f7fd edb4    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x2498
    64b6: 6c37         	ldr	r7, [r6, #0x40]
    64b8: 2f00         	cmp	r7, #0x0
    64ba: f47f af7e    	bne.w	0x63ba <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x26> @ imm = #-0x104
    64be: 2402         	movs	r4, #0x2
    64c0: e7cc         	b	0x645c <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xc8> @ imm = #-0x68
    64c2: f10d 0938    	add.w	r9, sp, #0x38
    64c6: 4632         	mov	r2, r6
    64c8: 4649         	mov	r1, r9
    64ca: 4650         	mov	r0, r10
    64cc: f7fd edd2    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x245c
    64d0: 9b0e         	ldr	r3, [sp, #0x38]
    64d2: 9008         	str	r0, [sp, #0x20]
    64d4: 930a         	str	r3, [sp, #0x28]
    64d6: 4622         	mov	r2, r4
    64d8: 4659         	mov	r1, r11
    64da: f7fd ed56    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x2554
    64de: 9c0e         	ldr	r4, [sp, #0x38]
    64e0: 9a08         	ldr	r2, [sp, #0x20]
    64e2: e788         	b	0x63f6 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x62> @ imm = #-0xf0
    64e4: 4642         	mov	r2, r8
    64e6: f10d 0938    	add.w	r9, sp, #0x38
    64ea: 2c00         	cmp	r4, #0x0
    64ec: d083         	beq	0x63f6 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x62> @ imm = #-0xfa
    64ee: 4640         	mov	r0, r8
    64f0: f10d 0938    	add.w	r9, sp, #0x38
    64f4: e7ef         	b	0x64d6 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x142> @ imm = #-0x22
    64f6: 462a         	mov	r2, r5
    64f8: 2c00         	cmp	r4, #0x0
    64fa: d097         	beq	0x642c <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x98> @ imm = #-0xd2
    64fc: 4628         	mov	r0, r5
    64fe: e007         	b	0x6510 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x17c> @ imm = #0xe
    6500: 4632         	mov	r2, r6
    6502: a907         	add	r1, sp, #0x1c
    6504: 4648         	mov	r0, r9
    6506: f7fd edb6    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x2494
    650a: 9b07         	ldr	r3, [sp, #0x1c]
    650c: 900e         	str	r0, [sp, #0x38]
    650e: 9310         	str	r3, [sp, #0x40]
    6510: 4622         	mov	r2, r4
    6512: 4659         	mov	r1, r11
    6514: f7fd ed38    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x2590
    6518: 9c07         	ldr	r4, [sp, #0x1c]
    651a: 9a0e         	ldr	r2, [sp, #0x38]
    651c: e786         	b	0x642c <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x98> @ imm = #-0xf4
    651e: 480e         	ldr	r0, [pc, #0x38]         @ 0x6558 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1c4> literal=0x7174
    6520: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    6522: f7fd ebf0    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x2820
    6526: 480d         	ldr	r0, [pc, #0x34]         @ 0x655c <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1c8> literal=0x716c
    6528: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    652a: f7fd ebec    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x2828
    652e: f7fd ee28    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x23b0
    6532: 2e00         	cmp	r6, #0x0
    6534: d0fb         	beq	0x652e <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x19a> @ imm = #-0xa
    6536: 4650         	mov	r0, r10
    6538: f7fd edda    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x244c
    653c: e7f7         	b	0x652e <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x19a> @ imm = #-0x12
    653e: e7fa         	b	0x6536 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1a2> @ imm = #-0xc
    6540: 4648         	mov	r0, r9
    6542: f7fd edd6    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x2454
    6546: e7f6         	b	0x6536 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1a2> @ imm = #-0x14
    6548: 7738         	strb	r0, [r7, #0x1c]
    654a: 0000         	movs	r0, r0
    654c: 76e6         	strb	r6, [r4, #0x1b]
    654e: 0000         	movs	r0, r0
    6550: 76d6         	strb	r6, [r2, #0x1b]
    6552: 0000         	movs	r0, r0
    6554: 76b8         	strb	r0, [r7, #0x1a]
    6556: 0000         	movs	r0, r0
    6558: 7174         	strb	r4, [r6, #0x5]
    655a: 0000         	movs	r0, r0
    655c: 716c         	strb	r4, [r5, #0x5]
    655e: 0000         	movs	r0, r0

00006560 <ConnectionManager::doDisconnectAudio(AudioConnection&)>:
    6560: f101 0218    	add.w	r2, r1, #0x18
    6564: f7ff bf16    	b.w	0x6394 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)> @ imm = #-0x1d4

00006568 <ConnectionManager::~ConnectionManager()>:
    6568: e92d 41f0    	push.w	{r4, r5, r6, r7, r8, lr}
    656c: 4605         	mov	r5, r0
    656e: e9d0 470c    	ldrd	r4, r7, [r0, #48]
    6572: b088         	sub	sp, #0x20
    6574: 4e64         	ldr	r6, [pc, #0x190]        @ 0x6708 <ConnectionManager::~ConnectionManager()+0x1a0> literal=0x1a844
    6576: 42a7         	cmp	r7, r4
    6578: 447e         	add	r6, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    657a: d02d         	beq	0x65d8 <ConnectionManager::~ConnectionManager()+0x70> @ imm = #0x5a
    657c: f104 0218    	add.w	r2, r4, #0x18
    6580: 4621         	mov	r1, r4
    6582: 4628         	mov	r0, r5
    6584: 344c         	adds	r4, #0x4c
    6586: f7ff ff05    	bl	0x6394 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)> @ imm = #-0x1f6
    658a: 42a7         	cmp	r7, r4
    658c: d1f6         	bne	0x657c <ConnectionManager::~ConnectionManager()+0x14> @ imm = #-0x14
    658e: e9d5 870c    	ldrd	r8, r7, [r5, #48]
    6592: 45b8         	cmp	r8, r7
    6594: d020         	beq	0x65d8 <ConnectionManager::~ConnectionManager()+0x70> @ imm = #0x40
    6596: 4644         	mov	r4, r8
    6598: 6b20         	ldr	r0, [r4, #0x30]
    659a: f104 0338    	add.w	r3, r4, #0x38
    659e: 4298         	cmp	r0, r3
    65a0: d003         	beq	0x65aa <ConnectionManager::~ConnectionManager()+0x42> @ imm = #0x6
    65a2: 6ba1         	ldr	r1, [r4, #0x38]
    65a4: 3101         	adds	r1, #0x1
    65a6: f7fd ecac    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x26a8
    65aa: 69a0         	ldr	r0, [r4, #0x18]
    65ac: f104 0320    	add.w	r3, r4, #0x20
    65b0: 4298         	cmp	r0, r3
    65b2: d003         	beq	0x65bc <ConnectionManager::~ConnectionManager()+0x54> @ imm = #0x6
    65b4: 6a21         	ldr	r1, [r4, #0x20]
    65b6: 3101         	adds	r1, #0x1
    65b8: f7fd eca2    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x26bc
    65bc: 4623         	mov	r3, r4
    65be: f853 0b08    	ldr	r0, [r3], #8
    65c2: 4298         	cmp	r0, r3
    65c4: d003         	beq	0x65ce <ConnectionManager::~ConnectionManager()+0x66> @ imm = #0x6
    65c6: 68a1         	ldr	r1, [r4, #0x8]
    65c8: 3101         	adds	r1, #0x1
    65ca: f7fd ec9a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x26cc
    65ce: 344c         	adds	r4, #0x4c
    65d0: 42a7         	cmp	r7, r4
    65d2: d1e1         	bne	0x6598 <ConnectionManager::~ConnectionManager()+0x30> @ imm = #-0x3e
    65d4: f8c5 8034    	str.w	r8, [r5, #0x34]
    65d8: 6c68         	ldr	r0, [r5, #0x44]
    65da: 2800         	cmp	r0, #0x0
    65dc: f040 8085    	bne.w	0x66ea <ConnectionManager::~ConnectionManager()+0x182> @ imm = #0x10a
    65e0: 6e68         	ldr	r0, [r5, #0x64]
    65e2: f105 036c    	add.w	r3, r5, #0x6c
    65e6: 4298         	cmp	r0, r3
    65e8: d003         	beq	0x65f2 <ConnectionManager::~ConnectionManager()+0x8a> @ imm = #0x6
    65ea: 6ee9         	ldr	r1, [r5, #0x6c]
    65ec: 3101         	adds	r1, #0x1
    65ee: f7fd ec88    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x26f0
    65f2: 6ce8         	ldr	r0, [r5, #0x4c]
    65f4: f105 0354    	add.w	r3, r5, #0x54
    65f8: 4298         	cmp	r0, r3
    65fa: d003         	beq	0x6604 <ConnectionManager::~ConnectionManager()+0x9c> @ imm = #0x6
    65fc: 6d69         	ldr	r1, [r5, #0x54]
    65fe: 3101         	adds	r1, #0x1
    6600: f7fd ec7e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2704
    6604: e9d5 470c    	ldrd	r4, r7, [r5, #48]
    6608: 42bc         	cmp	r4, r7
    660a: d01e         	beq	0x664a <ConnectionManager::~ConnectionManager()+0xe2> @ imm = #0x3c
    660c: 6b20         	ldr	r0, [r4, #0x30]
    660e: f104 0338    	add.w	r3, r4, #0x38
    6612: 4298         	cmp	r0, r3
    6614: d003         	beq	0x661e <ConnectionManager::~ConnectionManager()+0xb6> @ imm = #0x6
    6616: 6ba1         	ldr	r1, [r4, #0x38]
    6618: 3101         	adds	r1, #0x1
    661a: f7fd ec72    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x271c
    661e: 69a0         	ldr	r0, [r4, #0x18]
    6620: f104 0320    	add.w	r3, r4, #0x20
    6624: 4298         	cmp	r0, r3
    6626: d003         	beq	0x6630 <ConnectionManager::~ConnectionManager()+0xc8> @ imm = #0x6
    6628: 6a21         	ldr	r1, [r4, #0x20]
    662a: 3101         	adds	r1, #0x1
    662c: f7fd ec68    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2730
    6630: 4623         	mov	r3, r4
    6632: f853 0b08    	ldr	r0, [r3], #8
    6636: 4298         	cmp	r0, r3
    6638: d003         	beq	0x6642 <ConnectionManager::~ConnectionManager()+0xda> @ imm = #0x6
    663a: 68a1         	ldr	r1, [r4, #0x8]
    663c: 3101         	adds	r1, #0x1
    663e: f7fd ec60    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2740
    6642: 344c         	adds	r4, #0x4c
    6644: 42a7         	cmp	r7, r4
    6646: d1e1         	bne	0x660c <ConnectionManager::~ConnectionManager()+0xa4> @ imm = #-0x3e
    6648: 6b2f         	ldr	r7, [r5, #0x30]
    664a: b127         	cbz	r7, 0x6656 <ConnectionManager::~ConnectionManager()+0xee> @ imm = #0x8
    664c: 6ba9         	ldr	r1, [r5, #0x38]
    664e: 4638         	mov	r0, r7
    6650: 1bc9         	subs	r1, r1, r7
    6652: f7fd ec56    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2754
    6656: 682b         	ldr	r3, [r5]
    6658: b1cb         	cbz	r3, 0x668e <ConnectionManager::~ConnectionManager()+0x126> @ imm = #0x32
    665a: 4a2c         	ldr	r2, [pc, #0xb0]         @ 0x670c <ConnectionManager::~ConnectionManager()+0x1a4> literal=0x22c
    665c: af01         	add	r7, sp, #0x4
    665e: f8d3 8000    	ldr.w	r8, [r3]
    6662: 4638         	mov	r0, r7
    6664: 58b4         	ldr	r4, [r6, r2]
    6666: f104 0308    	add.w	r3, r4, #0x8
    666a: 9300         	str	r3, [sp]
    666c: f7fd ec5c    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x2748
    6670: 4b27         	ldr	r3, [pc, #0x9c]         @ 0x6710 <ConnectionManager::~ConnectionManager()+0x1a8> literal=0x224
    6672: 4640         	mov	r0, r8
    6674: 462a         	mov	r2, r5
    6676: 58f1         	ldr	r1, [r6, r3]
    6678: 463b         	mov	r3, r7
    667a: f7fd ed20    	blx	0x40bc <LSCallCancelNotificationRemove@plt> @ imm = #-0x25c0
    667e: 4638         	mov	r0, r7
    6680: 3408         	adds	r4, #0x8
    6682: 9400         	str	r4, [sp]
    6684: f7fd ed82    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x24fc
    6688: 4668         	mov	r0, sp
    668a: f7fd eb4e    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x2964
    668e: e9d5 6701    	ldrd	r6, r7, [r5, #4]
    6692: 42be         	cmp	r6, r7
    6694: d10a         	bne	0x66ac <ConnectionManager::~ConnectionManager()+0x144> @ imm = #0x14
    6696: e015         	b	0x66c4 <ConnectionManager::~ConnectionManager()+0x15c> @ imm = #0x2a
    6698: 6820         	ldr	r0, [r4]
    669a: b108         	cbz	r0, 0x66a0 <ConnectionManager::~ConnectionManager()+0x138> @ imm = #0x2
    669c: f7fd ecd6    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x2654
    66a0: 210c         	movs	r1, #0xc
    66a2: 4620         	mov	r0, r4
    66a4: f7fd ec2c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x27a8
    66a8: 42b7         	cmp	r7, r6
    66aa: d00b         	beq	0x66c4 <ConnectionManager::~ConnectionManager()+0x15c> @ imm = #0x16
    66ac: f856 4b04    	ldr	r4, [r6], #4
    66b0: 2c00         	cmp	r4, #0x0
    66b2: d0f9         	beq	0x66a8 <ConnectionManager::~ConnectionManager()+0x140> @ imm = #-0xe
    66b4: 68a3         	ldr	r3, [r4, #0x8]
    66b6: 2b00         	cmp	r3, #0x0
    66b8: d0ee         	beq	0x6698 <ConnectionManager::~ConnectionManager()+0x130> @ imm = #-0x24
    66ba: 6860         	ldr	r0, [r4, #0x4]
    66bc: 4621         	mov	r1, r4
    66be: f7fd ebc4    	blx	0x3e48 <_ZN2LS17SubscriptionPoint9cleanItemEPNS0_16SubscriptionItemE@plt> @ imm = #-0x2878
    66c2: e7e9         	b	0x6698 <ConnectionManager::~ConnectionManager()+0x130> @ imm = #-0x2e
    66c4: 6928         	ldr	r0, [r5, #0x10]
    66c6: f105 0318    	add.w	r3, r5, #0x18
    66ca: 4298         	cmp	r0, r3
    66cc: d003         	beq	0x66d6 <ConnectionManager::~ConnectionManager()+0x16e> @ imm = #0x6
    66ce: 69a9         	ldr	r1, [r5, #0x18]
    66d0: 3101         	adds	r1, #0x1
    66d2: f7fd ec16    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x27d4
    66d6: 6868         	ldr	r0, [r5, #0x4]
    66d8: b118         	cbz	r0, 0x66e2 <ConnectionManager::~ConnectionManager()+0x17a> @ imm = #0x6
    66da: 68e9         	ldr	r1, [r5, #0xc]
    66dc: 1a09         	subs	r1, r1, r0
    66de: f7fd ec10    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x27e0
    66e2: 4628         	mov	r0, r5
    66e4: b008         	add	sp, #0x20
    66e6: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
    66ea: f7fd ebb4    	blx	0x3e54 <g_source_remove@plt> @ imm = #-0x2898
    66ee: 2300         	movs	r3, #0x0
    66f0: 646b         	str	r3, [r5, #0x44]
    66f2: e775         	b	0x65e0 <ConnectionManager::~ConnectionManager()+0x78> @ imm = #-0x116
    66f4: 4668         	mov	r0, sp
    66f6: f003 f9ad    	bl	0x9a54 <ConnectionManager::removePipelineInfo(LSMessage&)+0x484> @ imm = #0x335a
    66fa: f7fd ed84    	blx	0x4204 <_ZSt9terminatev@plt> @ imm = #-0x24f8
    66fe: 4668         	mov	r0, sp
    6700: f7fd eb12    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x29dc
    6704: e7f9         	b	0x66fa <ConnectionManager::~ConnectionManager()+0x192> @ imm = #-0xe
    6706: bf00         	nop
    6708: a844         	add	r0, sp, #0x110
    670a: 0001         	movs	r1, r0
    670c: 022c         	lsls	r4, r5, #0x8
    670e: 0000         	movs	r0, r0
    6710: 0224         	lsls	r4, r4, #0x8
    6712: 0000         	movs	r0, r0

00006714 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)>:
    6714: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    6718: b099         	sub	sp, #0x64
    671a: 4604         	mov	r4, r0
    671c: 460f         	mov	r7, r1
    671e: 4615         	mov	r5, r2
    6720: 461e         	mov	r6, r3
    6722: f003 ff0b    	bl	0xa53c <getPmLogContext()> @ imm = #0x3e16
    6726: 2800         	cmp	r0, #0x0
    6728: d078         	beq	0x681c <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x108> @ imm = #0xf0
    672a: f003 ff07    	bl	0xa53c <getPmLogContext()> @ imm = #0x3e0e
    672e: 6803         	ldr	r3, [r0]
    6730: 2b05         	cmp	r3, #0x5
    6732: dc73         	bgt	0x681c <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x108> @ imm = #0xe6
    6734: f8d4 8040    	ldr.w	r8, [r4, #0x40]
    6738: f1b8 0f00    	cmp.w	r8, #0x0
    673c: f000 8088    	beq.w	0x6850 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x13c> @ imm = #0x110
    6740: e9d7 b400    	ldrd	r11, r4, [r7]
    6744: ab0e         	add	r3, sp, #0x38
    6746: 9309         	str	r3, [sp, #0x24]
    6748: f10d 0a30    	add.w	r10, sp, #0x30
    674c: 930c         	str	r3, [sp, #0x30]
    674e: fabb f78b    	clz	r7, r11
    6752: 4263         	rsbs	r3, r4, #0
    6754: 097f         	lsrs	r7, r7, #0x5
    6756: 459b         	cmp	r11, r3
    6758: bf08         	it	eq
    675a: 2700         	moveq	r7, #0x0
    675c: 2f00         	cmp	r7, #0x0
    675e: f040 80ac    	bne.w	0x68ba <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1a6> @ imm = #0x158
    6762: 2c0f         	cmp	r4, #0xf
    6764: 9412         	str	r4, [sp, #0x48]
    6766: d875         	bhi	0x6854 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x140> @ imm = #0xea
    6768: 2c01         	cmp	r4, #0x1
    676a: f040 8084    	bne.w	0x6876 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x162> @ imm = #0x108
    676e: 9a09         	ldr	r2, [sp, #0x24]
    6770: f10d 0948    	add.w	r9, sp, #0x48
    6774: f89b 3000    	ldrb.w	r3, [r11]
    6778: f88d 3038    	strb.w	r3, [sp, #0x38]
    677c: 2300         	movs	r3, #0x0
    677e: 940d         	str	r4, [sp, #0x34]
    6780: 5513         	strb	r3, [r2, r4]
    6782: e9d5 b400    	ldrd	r11, r4, [r5]
    6786: ad14         	add	r5, sp, #0x50
    6788: 9512         	str	r5, [sp, #0x48]
    678a: 4263         	rsbs	r3, r4, #0
    678c: fabb f78b    	clz	r7, r11
    6790: 097f         	lsrs	r7, r7, #0x5
    6792: 459b         	cmp	r11, r3
    6794: bf08         	it	eq
    6796: 2700         	moveq	r7, #0x0
    6798: 2f00         	cmp	r7, #0x0
    679a: f040 808a    	bne.w	0x68b2 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x19e> @ imm = #0x114
    679e: 2c0f         	cmp	r4, #0xf
    67a0: 940b         	str	r4, [sp, #0x2c]
    67a2: d877         	bhi	0x6894 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x180> @ imm = #0xee
    67a4: 2c01         	cmp	r4, #0x1
    67a6: d170         	bne	0x688a <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x176> @ imm = #0xe0
    67a8: f89b 3000    	ldrb.w	r3, [r11]
    67ac: 462a         	mov	r2, r5
    67ae: f88d 3050    	strb.w	r3, [sp, #0x50]
    67b2: 2300         	movs	r3, #0x0
    67b4: 9413         	str	r4, [sp, #0x4c]
    67b6: 4640         	mov	r0, r8
    67b8: 5513         	strb	r3, [r2, r4]
    67ba: 4651         	mov	r1, r10
    67bc: 7833         	ldrb	r3, [r6]
    67be: 464a         	mov	r2, r9
    67c0: f7fd eb54    	blx	0x3e6c <_ZN9umiClient7setMuteENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_b@plt> @ imm = #-0x2958
    67c4: 9b12         	ldr	r3, [sp, #0x48]
    67c6: 4604         	mov	r4, r0
    67c8: 42ab         	cmp	r3, r5
    67ca: d004         	beq	0x67d6 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0xc2> @ imm = #0x8
    67cc: 9914         	ldr	r1, [sp, #0x50]
    67ce: 4618         	mov	r0, r3
    67d0: 3101         	adds	r1, #0x1
    67d2: f7fd eb96    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x28d4
    67d6: 980c         	ldr	r0, [sp, #0x30]
    67d8: 9b09         	ldr	r3, [sp, #0x24]
    67da: 4298         	cmp	r0, r3
    67dc: d003         	beq	0x67e6 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0xd2> @ imm = #0x6
    67de: 990e         	ldr	r1, [sp, #0x38]
    67e0: 3101         	adds	r1, #0x1
    67e2: f7fd eb8e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x28e4
    67e6: f003 fea9    	bl	0xa53c <getPmLogContext()> @ imm = #0x3d52
    67ea: b120         	cbz	r0, 0x67f6 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0xe2> @ imm = #0x8
    67ec: f003 fea6    	bl	0xa53c <getPmLogContext()> @ imm = #0x3d4c
    67f0: 6803         	ldr	r3, [r0]
    67f2: 2b05         	cmp	r3, #0x5
    67f4: dd0e         	ble	0x6814 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x100> @ imm = #0x1c
    67f6: f003 fea1    	bl	0xa53c <getPmLogContext()> @ imm = #0x3d42
    67fa: 4938         	ldr	r1, [pc, #0xe0]         @ 0x68dc <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1c8> literal=0x741a
    67fc: 2200         	movs	r2, #0x0
    67fe: 4b38         	ldr	r3, [pc, #0xe0]         @ 0x68e0 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1cc> literal=0x73cc
    6800: 9404         	str	r4, [sp, #0x10]
    6802: 4479         	add	r1, pc => raw=0xdc20 ghidra=0x1dc20 file=56352 "Audio mute umi returnStatus %d"
    6804: e9cd 2200    	strd	r2, r2, [sp]
    6808: e9cd 2102    	strd	r2, r1, [sp, #8]
    680c: 447b         	add	r3, pc => raw=0xdbdc ghidra=0x1dbdc file=56284 "AUDIO_MUTE_CALL"
    680e: 2106         	movs	r1, #0x6
    6810: f7fd ec04    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x27f8
    6814: 4620         	mov	r0, r4
    6816: b019         	add	sp, #0x64
    6818: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    681c: f003 fe8e    	bl	0xa53c <getPmLogContext()> @ imm = #0x3d1c
    6820: 7833         	ldrb	r3, [r6]
    6822: 2200         	movs	r2, #0x0
    6824: 2106         	movs	r1, #0x6
    6826: 9306         	str	r3, [sp, #0x18]
    6828: 682b         	ldr	r3, [r5]
    682a: 9305         	str	r3, [sp, #0x14]
    682c: 683b         	ldr	r3, [r7]
    682e: e9cd 2201    	strd	r2, r2, [sp, #4]
    6832: 9200         	str	r2, [sp]
    6834: 9304         	str	r3, [sp, #0x10]
    6836: 4b2b         	ldr	r3, [pc, #0xac]         @ 0x68e4 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1d0> literal=0x73b0
    6838: 447b         	add	r3, pc => raw=0xdbec ghidra=0x1dbec file=56300 "Audio mute request for source:%s, sink:%s, mute:%d"
    683a: 9303         	str	r3, [sp, #0xc]
    683c: 4b2a         	ldr	r3, [pc, #0xa8]         @ 0x68e8 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1d4> literal=0x739a
    683e: 447b         	add	r3, pc => raw=0xdbdc ghidra=0x1dbdc file=56284 "AUDIO_MUTE_CALL"
    6840: f7fd ebec    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x2828
    6844: f8d4 8040    	ldr.w	r8, [r4, #0x40]
    6848: f1b8 0f00    	cmp.w	r8, #0x0
    684c: f47f af78    	bne.w	0x6740 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x2c> @ imm = #-0x110
    6850: 2402         	movs	r4, #0x2
    6852: e7c8         	b	0x67e6 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0xd2> @ imm = #-0x70
    6854: f10d 0948    	add.w	r9, sp, #0x48
    6858: 463a         	mov	r2, r7
    685a: 4649         	mov	r1, r9
    685c: 4650         	mov	r0, r10
    685e: f7fd ec0a    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x27ec
    6862: 9b12         	ldr	r3, [sp, #0x48]
    6864: 900c         	str	r0, [sp, #0x30]
    6866: 930e         	str	r3, [sp, #0x38]
    6868: 4622         	mov	r2, r4
    686a: 4659         	mov	r1, r11
    686c: f7fd eb8c    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x28e8
    6870: 9c12         	ldr	r4, [sp, #0x48]
    6872: 9a0c         	ldr	r2, [sp, #0x30]
    6874: e782         	b	0x677c <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x68> @ imm = #-0xfc
    6876: 9a09         	ldr	r2, [sp, #0x24]
    6878: f10d 0948    	add.w	r9, sp, #0x48
    687c: 2c00         	cmp	r4, #0x0
    687e: f43f af7d    	beq.w	0x677c <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x68> @ imm = #-0x106
    6882: 9809         	ldr	r0, [sp, #0x24]
    6884: f10d 0948    	add.w	r9, sp, #0x48
    6888: e7ee         	b	0x6868 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x154> @ imm = #-0x24
    688a: 462a         	mov	r2, r5
    688c: 2c00         	cmp	r4, #0x0
    688e: d090         	beq	0x67b2 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x9e> @ imm = #-0xe0
    6890: 4628         	mov	r0, r5
    6892: e007         	b	0x68a4 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x190> @ imm = #0xe
    6894: 463a         	mov	r2, r7
    6896: a90b         	add	r1, sp, #0x2c
    6898: 4648         	mov	r0, r9
    689a: f7fd ebec    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x2828
    689e: 9b0b         	ldr	r3, [sp, #0x2c]
    68a0: 9012         	str	r0, [sp, #0x48]
    68a2: 9314         	str	r3, [sp, #0x50]
    68a4: 4622         	mov	r2, r4
    68a6: 4659         	mov	r1, r11
    68a8: f7fd eb6e    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x2924
    68ac: 9c0b         	ldr	r4, [sp, #0x2c]
    68ae: 9a12         	ldr	r2, [sp, #0x48]
    68b0: e77f         	b	0x67b2 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x9e> @ imm = #-0x102
    68b2: 480e         	ldr	r0, [pc, #0x38]         @ 0x68ec <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1d8> literal=0x6de0
    68b4: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    68b6: f7fd ea26    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x2bb4
    68ba: 480d         	ldr	r0, [pc, #0x34]         @ 0x68f0 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1dc> literal=0x6dd8
    68bc: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    68be: f7fd ea22    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x2bbc
    68c2: f7fd ec5e    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x2744
    68c6: 2f00         	cmp	r7, #0x0
    68c8: d0fb         	beq	0x68c2 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1ae> @ imm = #-0xa
    68ca: 4650         	mov	r0, r10
    68cc: f7fd ec10    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x27e0
    68d0: e7f7         	b	0x68c2 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1ae> @ imm = #-0x12
    68d2: e7fa         	b	0x68ca <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1b6> @ imm = #-0xc
    68d4: 4648         	mov	r0, r9
    68d6: f7fd ec0c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x27e8
    68da: e7f6         	b	0x68ca <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)+0x1b6> @ imm = #-0x14
    68dc: 741a         	strb	r2, [r3, #0x10]
    68de: 0000         	movs	r0, r0
    68e0: 73cc         	strb	r4, [r1, #0xf]
    68e2: 0000         	movs	r0, r0
    68e4: 73b0         	strb	r0, [r6, #0xe]
    68e6: 0000         	movs	r0, r0
    68e8: 739a         	strb	r2, [r3, #0xe]
    68ea: 0000         	movs	r0, r0
    68ec: 6de0         	ldr	r0, [r4, #0x5c]
    68ee: 0000         	movs	r0, r0
    68f0: 6dd8         	ldr	r0, [r3, #0x5c]
    68f2: 0000         	movs	r0, r0

000068f4 <ConnectionManager::doMuteAudio(AudioConnection&, bool)>:
    68f4: f891 3048    	ldrb.w	r3, [r1, #0x48]
    68f8: b510         	push	{r4, lr}
    68fa: b082         	sub	sp, #0x8
    68fc: f88d 2007    	strb.w	r2, [sp, #0x7]
    6900: 4293         	cmp	r3, r2
    6902: bf08         	it	eq
    6904: 2001         	moveq	r0, #0x1
    6906: d00c         	beq	0x6922 <ConnectionManager::doMuteAudio(AudioConnection&, bool)+0x2e> @ imm = #0x18
    6908: f10d 0307    	add.w	r3, sp, #0x7
    690c: f101 0218    	add.w	r2, r1, #0x18
    6910: 460c         	mov	r4, r1
    6912: f7ff feff    	bl	0x6714 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)> @ imm = #-0x202
    6916: b930         	cbnz	r0, 0x6926 <ConnectionManager::doMuteAudio(AudioConnection&, bool)+0x32> @ imm = #0xc
    6918: f89d 3007    	ldrb.w	r3, [sp, #0x7]
    691c: 2001         	movs	r0, #0x1
    691e: f884 3048    	strb.w	r3, [r4, #0x48]
    6922: b002         	add	sp, #0x8
    6924: bd10         	pop	{r4, pc}
    6926: 2000         	movs	r0, #0x0
    6928: b002         	add	sp, #0x8
    692a: bd10         	pop	{r4, pc}

0000692c <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)>:
    692c: e92d 43f0    	push.w	{r4, r5, r6, r7, r8, r9, lr}
    6930: b08f         	sub	sp, #0x3c
    6932: 62c2         	str	r2, [r0, #0x2c]
    6934: f100 0254    	add.w	r2, r0, #0x54
    6938: ad07         	add	r5, sp, #0x1c
    693a: 4604         	mov	r4, r0
    693c: f8df 81a4    	ldr.w	r8, [pc, #0x1a4]        @ 0x6ae4 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1b8> literal=0x1a46e
    6940: 460f         	mov	r7, r1
    6942: e9c0 130f    	strd	r1, r3, [r0, #60]
    6946: f100 036c    	add.w	r3, r0, #0x6c
    694a: 64c2         	str	r2, [r0, #0x4c]
    694c: 4a66         	ldr	r2, [pc, #0x198]        @ 0x6ae8 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1bc> literal=0x22c
    694e: 44f8         	add	r8, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    6950: 6643         	str	r3, [r0, #0x64]
    6952: 2300         	movs	r3, #0x0
    6954: e9c0 3300    	strd	r3, r3, [r0]
    6958: e9c0 3302    	strd	r3, r3, [r0, #8]
    695c: 6143         	str	r3, [r0, #0x14]
    695e: 7603         	strb	r3, [r0, #0x18]
    6960: f880 3028    	strb.w	r3, [r0, #0x28]
    6964: e9c0 330c    	strd	r3, r3, [r0, #48]
    6968: 6383         	str	r3, [r0, #0x38]
    696a: 6503         	str	r3, [r0, #0x50]
    696c: f880 3054    	strb.w	r3, [r0, #0x54]
    6970: 6683         	str	r3, [r0, #0x68]
    6972: f880 306c    	strb.w	r3, [r0, #0x6c]
    6976: f100 0318    	add.w	r3, r0, #0x18
    697a: 6103         	str	r3, [r0, #0x10]
    697c: 4628         	mov	r0, r5
    697e: f858 6002    	ldr.w	r6, [r8, r2]
    6982: f106 0308    	add.w	r3, r6, #0x8
    6986: 9306         	str	r3, [sp, #0x18]
    6988: f7fd eace    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x2a64
    698c: 4a57         	ldr	r2, [pc, #0x15c]        @ 0x6aec <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1c0> literal=0x1a1de
    698e: 2300         	movs	r3, #0x0
    6990: 4957         	ldr	r1, [pc, #0x15c]        @ 0x6af0 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1c4> literal=0x72a2
    6992: 6838         	ldr	r0, [r7]
    6994: af06         	add	r7, sp, #0x18
    6996: 447a         	add	r2, pc => raw=0x20b78 ghidra=0x30b78 file=68472
    6998: 9501         	str	r5, [sp, #0x4]
    699a: 4479         	add	r1, pc => raw=0xdc40 ghidra=0x1dc40 file=56384 "/audio"
    699c: 9300         	str	r3, [sp]
    699e: f7fd ebc0    	blx	0x4120 <LSRegisterCategory@plt> @ imm = #-0x2880
    69a2: 2800         	cmp	r0, #0x0
    69a4: d073         	beq	0x6a8e <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x162> @ imm = #0xe6
    69a6: f106 0908    	add.w	r9, r6, #0x8
    69aa: 4628         	mov	r0, r5
    69ac: af06         	add	r7, sp, #0x18
    69ae: f8cd 9018    	str.w	r9, [sp, #0x18]
    69b2: f7fd ebec    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x2828
    69b6: 4638         	mov	r0, r7
    69b8: f7fd e9b6    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x2c94
    69bc: 4628         	mov	r0, r5
    69be: f8cd 9018    	str.w	r9, [sp, #0x18]
    69c2: f8d4 903c    	ldr.w	r9, [r4, #0x3c]
    69c6: f7fd eab0    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x2aa0
    69ca: 494a         	ldr	r1, [pc, #0x128]        @ 0x6af4 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1c8> literal=0x7268
    69cc: 462b         	mov	r3, r5
    69ce: f8d9 0000    	ldr.w	r0, [r9]
    69d2: 4622         	mov	r2, r4
    69d4: 4479         	add	r1, pc => raw=0xdc40 ghidra=0x1dc40 file=56384 "/audio"
    69d6: f7fd eaea    	blx	0x3fac <LSCategorySetData@plt> @ imm = #-0x2a2c
    69da: 2800         	cmp	r0, #0x0
    69dc: d039         	beq	0x6a52 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x126> @ imm = #0x72
    69de: 4628         	mov	r0, r5
    69e0: 3608         	adds	r6, #0x8
    69e2: 9606         	str	r6, [sp, #0x18]
    69e4: f7fd ebd2    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x285c
    69e8: 4638         	mov	r0, r7
    69ea: f7fd e99e    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x2cc4
    69ee: 2300         	movs	r3, #0x0
    69f0: 4620         	mov	r0, r4
    69f2: 6463         	str	r3, [r4, #0x44]
    69f4: f884 3048    	strb.w	r3, [r4, #0x48]
    69f8: b00f         	add	sp, #0x3c
    69fa: e8bd 83f0    	pop.w	{r4, r5, r6, r7, r8, r9, pc}
    69fe: 460e         	mov	r6, r1
    6a00: 4605         	mov	r5, r0
    6a02: a806         	add	r0, sp, #0x18
    6a04: f7fd e990    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x2ce0
    6a08: 4633         	mov	r3, r6
    6a0a: 2b01         	cmp	r3, #0x1
    6a0c: d152         	bne	0x6ab4 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x188> @ imm = #0xa4
    6a0e: 4628         	mov	r0, r5
    6a10: f7fd e97e    	blx	0x3d10 <__cxa_begin_catch@plt> @ imm = #-0x2d04
    6a14: 4605         	mov	r5, r0
    6a16: f003 fd91    	bl	0xa53c <getPmLogContext()> @ imm = #0x3b22
    6a1a: 2800         	cmp	r0, #0x0
    6a1c: d15b         	bne	0x6ad6 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1aa> @ imm = #0xb6
    6a1e: f003 fd8d    	bl	0xa53c <getPmLogContext()> @ imm = #0x3b1a
    6a22: 682a         	ldr	r2, [r5]
    6a24: 4603         	mov	r3, r0
    6a26: 4628         	mov	r0, r5
    6a28: 461d         	mov	r5, r3
    6a2a: 6893         	ldr	r3, [r2, #0x8]
    6a2c: 4798         	blx	r3
    6a2e: 4b32         	ldr	r3, [pc, #0xc8]         @ 0x6af8 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1cc> literal=0x7222
    6a30: 4601         	mov	r1, r0
    6a32: 2200         	movs	r2, #0x0
    6a34: 9104         	str	r1, [sp, #0x10]
    6a36: 4628         	mov	r0, r5
    6a38: 2103         	movs	r1, #0x3
    6a3a: 447b         	add	r3, pc => raw=0xdc60 ghidra=0x1dc60 file=56416 "%s - ConnectionManager API's registration Failed."
    6a3c: e9cd 2201    	strd	r2, r2, [sp, #4]
    6a40: 9303         	str	r3, [sp, #0xc]
    6a42: 4b2e         	ldr	r3, [pc, #0xb8]         @ 0x6afc <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1d0> literal=0x71fe
    6a44: 9200         	str	r2, [sp]
    6a46: 447b         	add	r3, pc => raw=0xdc48 ghidra=0x1dc48 file=56392 "LS2_SUBSCRIBE_FAILED"
    6a48: f7fd eae8    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x2a30
    6a4c: f7fd eb30    	blx	0x40b0 <__cxa_end_catch@plt> @ imm = #-0x29a0
    6a50: e7cd         	b	0x69ee <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0xc2> @ imm = #-0x66
    6a52: 2020         	movs	r0, #0x20
    6a54: f7fd ea16    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0x2bd4
    6a58: 4639         	mov	r1, r7
    6a5a: 4605         	mov	r5, r0
    6a5c: f003 f86e    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #0x30dc
    6a60: 4b27         	ldr	r3, [pc, #0x9c]         @ 0x6b00 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1d4> literal=0x228
    6a62: 4628         	mov	r0, r5
    6a64: f858 2003    	ldr.w	r2, [r8, r3]
    6a68: 4b26         	ldr	r3, [pc, #0x98]         @ 0x6b04 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1d8> literal=0x208
    6a6a: f858 1003    	ldr.w	r1, [r8, r3]
    6a6e: f7fd eabc    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x2a88
    6a72: 460e         	mov	r6, r1
    6a74: 4605         	mov	r5, r0
    6a76: 4638         	mov	r0, r7
    6a78: f002 ffec    	bl	0x9a54 <ConnectionManager::removePipelineInfo(LSMessage&)+0x484> @ imm = #0x2fd8
    6a7c: 4633         	mov	r3, r6
    6a7e: e7c4         	b	0x6a0a <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0xde> @ imm = #-0x78
    6a80: 460e         	mov	r6, r1
    6a82: 4605         	mov	r5, r0
    6a84: 4638         	mov	r0, r7
    6a86: f7fd e950    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x2d60
    6a8a: 4633         	mov	r3, r6
    6a8c: e7bd         	b	0x6a0a <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0xde> @ imm = #-0x86
    6a8e: 2020         	movs	r0, #0x20
    6a90: f7fd e9f8    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0x2c10
    6a94: 4639         	mov	r1, r7
    6a96: 4605         	mov	r5, r0
    6a98: f003 f850    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #0x30a0
    6a9c: 4b18         	ldr	r3, [pc, #0x60]         @ 0x6b00 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1d4> literal=0x228
    6a9e: 4628         	mov	r0, r5
    6aa0: f858 2003    	ldr.w	r2, [r8, r3]
    6aa4: 4b17         	ldr	r3, [pc, #0x5c]         @ 0x6b04 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x1d8> literal=0x208
    6aa6: f858 1003    	ldr.w	r1, [r8, r3]
    6aaa: f7fd ea9e    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x2ac4
    6aae: e7e0         	b	0x6a72 <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x146> @ imm = #-0x40
    6ab0: f7fd eafe    	blx	0x40b0 <__cxa_end_catch@plt> @ imm = #-0x2a04
    6ab4: f104 0064    	add.w	r0, r4, #0x64
    6ab8: f7fd eb1a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x29cc
    6abc: f104 004c    	add.w	r0, r4, #0x4c
    6ac0: f7fd eb16    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x29d4
    6ac4: f104 0030    	add.w	r0, r4, #0x30
    6ac8: f003 fa90    	bl	0x9fec <ConnectionManager::removePipelineInfo(LSMessage&)+0xa1c> @ imm = #0x3520
    6acc: 4620         	mov	r0, r4
    6ace: f003 f85d    	bl	0x9b8c <ConnectionManager::removePipelineInfo(LSMessage&)+0x5bc> @ imm = #0x30ba
    6ad2: f7fd eb56    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x2954
    6ad6: f003 fd31    	bl	0xa53c <getPmLogContext()> @ imm = #0x3a62
    6ada: 6803         	ldr	r3, [r0]
    6adc: 2b02         	cmp	r3, #0x2
    6ade: ddb5         	ble	0x6a4c <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0x120> @ imm = #-0x96
    6ae0: e79d         	b	0x6a1e <ConnectionManager::ConnectionManager(LS::Handle&, VolumeManager&, umiClient*)+0xf2> @ imm = #-0xc6
    6ae2: bf00         	nop
    6ae4: a46e         	adr	r4, #440 <ConnectionManager::connect(LSMessage&)+0x4e>
    6ae6: 0001         	movs	r1, r0
    6ae8: 022c         	lsls	r4, r5, #0x8
    6aea: 0000         	movs	r0, r0
    6aec: a1de         	adr	r1, #888 <ConnectionManager::connect(LSMessage&)+0xc6>
    6aee: 0001         	movs	r1, r0
    6af0: 72a2         	strb	r2, [r4, #0xa]
    6af2: 0000         	movs	r0, r0
    6af4: 7268         	strb	r0, [r5, #0x9]
    6af6: 0000         	movs	r0, r0
    6af8: 7222         	strb	r2, [r4, #0x8]
    6afa: 0000         	movs	r0, r0
    6afc: 71fe         	strb	r6, [r7, #0x7]
    6afe: 0000         	movs	r0, r0
    6b00: 0228         	lsls	r0, r5, #0x8
    6b02: 0000         	movs	r0, r0
    6b04: 0208         	lsls	r0, r1, #0x8
    6b06: 0000         	movs	r0, r0

00006b08 <ConnectionManager::connect(LSMessage&)>:
    6b08: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    6b0c: b0cf         	sub	sp, #0x13c
    6b0e: f8df 999c    	ldr.w	r9, [pc, #0x99c]        @ 0x74ac <ConnectionManager::connect(LSMessage&)+0x9a4> literal=0x1a29c
    6b12: 4603         	mov	r3, r0
    6b14: f10d 085c    	add.w	r8, sp, #0x5c
    6b18: 4608         	mov	r0, r1
    6b1a: 460c         	mov	r4, r1
    6b1c: 9311         	str	r3, [sp, #0x44]
    6b1e: 9115         	str	r1, [sp, #0x54]
    6b20: 44f9         	add	r9, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    6b22: f7fd eae0    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x2a40
    6b26: 4640         	mov	r0, r8
    6b28: f7fd eb78    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x2910
    6b2c: 2300         	movs	r3, #0x0
    6b2e: a81a         	add	r0, sp, #0x68
    6b30: 9316         	str	r3, [sp, #0x58]
    6b32: 9324         	str	r3, [sp, #0x90]
    6b34: f88d 3094    	strb.w	r3, [sp, #0x94]
    6b38: 932a         	str	r3, [sp, #0xa8]
    6b3a: f88d 30ac    	strb.w	r3, [sp, #0xac]
    6b3e: ab25         	add	r3, sp, #0x94
    6b40: 930d         	str	r3, [sp, #0x34]
    6b42: 9323         	str	r3, [sp, #0x8c]
    6b44: ab2b         	add	r3, sp, #0xac
    6b46: 900c         	str	r0, [sp, #0x30]
    6b48: 930e         	str	r3, [sp, #0x38]
    6b4a: 9329         	str	r3, [sp, #0xa4]
    6b4c: f7fd eb66    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x2934
    6b50: ab31         	add	r3, sp, #0xc4
    6b52: ae3b         	add	r6, sp, #0xec
    6b54: a82f         	add	r0, sp, #0xbc
    6b56: 9310         	str	r3, [sp, #0x40]
    6b58: 932f         	str	r3, [sp, #0xbc]
    6b5a: 2200         	movs	r2, #0x0
    6b5c: 23b4         	movs	r3, #0xb4
    6b5e: 4631         	mov	r1, r6
    6b60: 900f         	str	r0, [sp, #0x3c]
    6b62: 933b         	str	r3, [sp, #0xec]
    6b64: f7fd ea86    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x2af4
    6b68: f8df 1944    	ldr.w	r1, [pc, #0x944]        @ 0x74b0 <ConnectionManager::connect(LSMessage&)+0x9a8> literal=0x7236
    6b6c: 22b4         	movs	r2, #0xb4
    6b6e: 9b3b         	ldr	r3, [sp, #0xec]
    6b70: f10d 0b8c    	add.w	r11, sp, #0x8c
    6b74: 902f         	str	r0, [sp, #0xbc]
    6b76: 4479         	add	r1, pc => raw=0xddb0 ghidra=0x1ddb0 file=56752 "{\"type\":\"object\",\"properties\":{\"source\":{\"type\":\"string\"},\"sink\":{\"type\":\"string\"},\"pipelineAttribute\":{\"type\":\"object\"}},\"required\":[\"source\",\"sink\"],\"additionalProperties\":false}"
    6b78: 9331         	str	r3, [sp, #0xc4]
    6b7a: f7fd ea06    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x2bf4
    6b7e: 9b3b         	ldr	r3, [sp, #0xec]
    6b80: 2100         	movs	r1, #0x0
    6b82: 9a2f         	ldr	r2, [sp, #0xbc]
    6b84: 4620         	mov	r0, r4
    6b86: 9330         	str	r3, [sp, #0xc0]
    6b88: 54d1         	strb	r1, [r2, r3]
    6b8a: ab29         	add	r3, sp, #0xa4
    6b8c: 930b         	str	r3, [sp, #0x2c]
    6b8e: f7fd eb04    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x29f8
    6b92: ac3d         	add	r4, sp, #0xf4
    6b94: 4682         	mov	r10, r0
    6b96: 943b         	str	r4, [sp, #0xec]
    6b98: 2800         	cmp	r0, #0x0
    6b9a: f000 840a    	beq.w	0x73b2 <ConnectionManager::connect(LSMessage&)+0x8aa> @ imm = #0x814
    6b9e: f7fd ea82    	blx	0x40a4 <strlen@plt>     @ imm = #-0x2afc
    6ba2: 280f         	cmp	r0, #0xf
    6ba4: 4605         	mov	r5, r0
    6ba6: 9035         	str	r0, [sp, #0xd4]
    6ba8: f200 8216    	bhi.w	0x6fd8 <ConnectionManager::connect(LSMessage&)+0x4d0> @ imm = #0x42c
    6bac: 2801         	cmp	r0, #0x1
    6bae: f040 81ea    	bne.w	0x6f86 <ConnectionManager::connect(LSMessage&)+0x47e> @ imm = #0x3d4
    6bb2: f89a 3000    	ldrb.w	r3, [r10]
    6bb6: f88d 30f4    	strb.w	r3, [sp, #0xf4]
    6bba: 4623         	mov	r3, r4
    6bbc: 2200         	movs	r2, #0x0
    6bbe: 953c         	str	r5, [sp, #0xf0]
    6bc0: 4641         	mov	r1, r8
    6bc2: 555a         	strb	r2, [r3, r5]
    6bc4: 4630         	mov	r0, r6
    6bc6: 9a0f         	ldr	r2, [sp, #0x3c]
    6bc8: ab16         	add	r3, sp, #0x58
    6bca: f003 fc05    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #0x380a
    6bce: 4605         	mov	r5, r0
    6bd0: 983b         	ldr	r0, [sp, #0xec]
    6bd2: 42a0         	cmp	r0, r4
    6bd4: d003         	beq	0x6bde <ConnectionManager::connect(LSMessage&)+0xd6> @ imm = #0x6
    6bd6: 993d         	ldr	r1, [sp, #0xf4]
    6bd8: 3101         	adds	r1, #0x1
    6bda: f7fd e992    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2cdc
    6bde: 2d00         	cmp	r5, #0x0
    6be0: f000 81ec    	beq.w	0x6fbc <ConnectionManager::connect(LSMessage&)+0x4b4> @ imm = #0x3d8
    6be4: f8df 28cc    	ldr.w	r2, [pc, #0x8cc]        @ 0x74b4 <ConnectionManager::connect(LSMessage&)+0x9ac> literal=0x7270
    6be8: ab29         	add	r3, sp, #0xa4
    6bea: 4641         	mov	r1, r8
    6bec: 4630         	mov	r0, r6
    6bee: f10d 0b8c    	add.w	r11, sp, #0x8c
    6bf2: 930b         	str	r3, [sp, #0x2c]
    6bf4: 447a         	add	r2, pc => raw=0xde68 ghidra=0x1de68 file=56936 "sink"
    6bf6: f7fd e98a    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x2cec
    6bfa: ad35         	add	r5, sp, #0xd4
    6bfc: 2300         	movs	r3, #0x0
    6bfe: af37         	add	r7, sp, #0xdc
    6c00: 4629         	mov	r1, r5
    6c02: 4630         	mov	r0, r6
    6c04: 9336         	str	r3, [sp, #0xd8]
    6c06: f88d 30dc    	strb.w	r3, [sp, #0xdc]
    6c0a: 9735         	str	r7, [sp, #0xd4]
    6c0c: f7fd e8c8    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x2e70
    6c10: 9b35         	ldr	r3, [sp, #0xd4]
    6c12: 9823         	ldr	r0, [sp, #0x8c]
    6c14: 9a36         	ldr	r2, [sp, #0xd8]
    6c16: 42bb         	cmp	r3, r7
    6c18: f000 82fb    	beq.w	0x7212 <ConnectionManager::connect(LSMessage&)+0x70a> @ imm = #0x5f6
    6c1c: 990d         	ldr	r1, [sp, #0x34]
    6c1e: f8dd c0dc    	ldr.w	r12, [sp, #0xdc]
    6c22: e9cd 3223    	strd	r3, r2, [sp, #140]
    6c26: 4288         	cmp	r0, r1
    6c28: bf08         	it	eq
    6c2a: f8cd c094    	streq.w	r12, [sp, #0x94]
    6c2e: f000 82fe    	beq.w	0x722e <ConnectionManager::connect(LSMessage&)+0x726> @ imm = #0x5fc
    6c32: 9b25         	ldr	r3, [sp, #0x94]
    6c34: f8cd c094    	str.w	r12, [sp, #0x94]
    6c38: 2800         	cmp	r0, #0x0
    6c3a: f000 82f8    	beq.w	0x722e <ConnectionManager::connect(LSMessage&)+0x726> @ imm = #0x5f0
    6c3e: 9337         	str	r3, [sp, #0xdc]
    6c40: 9035         	str	r0, [sp, #0xd4]
    6c42: 2300         	movs	r3, #0x0
    6c44: 9336         	str	r3, [sp, #0xd8]
    6c46: 7003         	strb	r3, [r0]
    6c48: 9835         	ldr	r0, [sp, #0xd4]
    6c4a: 42b8         	cmp	r0, r7
    6c4c: d003         	beq	0x6c56 <ConnectionManager::connect(LSMessage&)+0x14e> @ imm = #0x6
    6c4e: 9937         	ldr	r1, [sp, #0xdc]
    6c50: 3101         	adds	r1, #0x1
    6c52: f7fd e956    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2d54
    6c56: f8df 3860    	ldr.w	r3, [pc, #0x860]        @ 0x74b8 <ConnectionManager::connect(LSMessage&)+0x9b0> literal=0x210
    6c5a: 4630         	mov	r0, r6
    6c5c: f10d 0b8c    	add.w	r11, sp, #0x8c
    6c60: f859 9003    	ldr.w	r9, [r9, r3]
    6c64: ab29         	add	r3, sp, #0xa4
    6c66: 930b         	str	r3, [sp, #0x2c]
    6c68: f109 0308    	add.w	r3, r9, #0x8
    6c6c: 933b         	str	r3, [sp, #0xec]
    6c6e: f7fd e96e    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x2d24
    6c72: f8df 2848    	ldr.w	r2, [pc, #0x848]        @ 0x74bc <ConnectionManager::connect(LSMessage&)+0x9b4> literal=0x6e76
    6c76: 4641         	mov	r1, r8
    6c78: 4628         	mov	r0, r5
    6c7a: 447a         	add	r2, pc => raw=0xdaf4 ghidra=0x1daf4 file=56052 "source"
    6c7c: f7fd e946    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x2d74
    6c80: 2300         	movs	r3, #0x0
    6c82: 4631         	mov	r1, r6
    6c84: 4628         	mov	r0, r5
    6c86: 943b         	str	r4, [sp, #0xec]
    6c88: 933c         	str	r3, [sp, #0xf0]
    6c8a: f88d 30f4    	strb.w	r3, [sp, #0xf4]
    6c8e: f7fd e888    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x2ef0
    6c92: 9b3b         	ldr	r3, [sp, #0xec]
    6c94: 9829         	ldr	r0, [sp, #0xa4]
    6c96: 9a3c         	ldr	r2, [sp, #0xf0]
    6c98: 42a3         	cmp	r3, r4
    6c9a: f000 82ac    	beq.w	0x71f6 <ConnectionManager::connect(LSMessage&)+0x6ee> @ imm = #0x558
    6c9e: 9f0e         	ldr	r7, [sp, #0x38]
    6ca0: 993d         	ldr	r1, [sp, #0xf4]
    6ca2: e9cd 3229    	strd	r3, r2, [sp, #164]
    6ca6: 42b8         	cmp	r0, r7
    6ca8: bf08         	it	eq
    6caa: 912b         	streq	r1, [sp, #0xac]
    6cac: f000 82c3    	beq.w	0x7236 <ConnectionManager::connect(LSMessage&)+0x72e> @ imm = #0x586
    6cb0: 9b2b         	ldr	r3, [sp, #0xac]
    6cb2: 912b         	str	r1, [sp, #0xac]
    6cb4: 2800         	cmp	r0, #0x0
    6cb6: f000 82be    	beq.w	0x7236 <ConnectionManager::connect(LSMessage&)+0x72e> @ imm = #0x57c
    6cba: 933d         	str	r3, [sp, #0xf4]
    6cbc: 903b         	str	r0, [sp, #0xec]
    6cbe: 2300         	movs	r3, #0x0
    6cc0: 933c         	str	r3, [sp, #0xf0]
    6cc2: 7003         	strb	r3, [r0]
    6cc4: 983b         	ldr	r0, [sp, #0xec]
    6cc6: 42a0         	cmp	r0, r4
    6cc8: d003         	beq	0x6cd2 <ConnectionManager::connect(LSMessage&)+0x1ca> @ imm = #0x6
    6cca: 993d         	ldr	r1, [sp, #0xf4]
    6ccc: 3101         	adds	r1, #0x1
    6cce: f7fd e918    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2dd0
    6cd2: f109 0308    	add.w	r3, r9, #0x8
    6cd6: 4628         	mov	r0, r5
    6cd8: 9335         	str	r3, [sp, #0xd4]
    6cda: f10d 0a74    	add.w	r10, sp, #0x74
    6cde: f7fd e936    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x2d94
    6ce2: f8df 27dc    	ldr.w	r2, [pc, #0x7dc]        @ 0x74c0 <ConnectionManager::connect(LSMessage&)+0x9b8> literal=0x717a
    6ce6: ab29         	add	r3, sp, #0xa4
    6ce8: 4641         	mov	r1, r8
    6cea: 4650         	mov	r0, r10
    6cec: f10d 0b8c    	add.w	r11, sp, #0x8c
    6cf0: 930b         	str	r3, [sp, #0x2c]
    6cf2: 447a         	add	r2, pc => raw=0xde70 ghidra=0x1de70 file=56944 "pipelineAttribute"
    6cf4: f7fd e90a    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x2dec
    6cf8: f109 0308    	add.w	r3, r9, #0x8
    6cfc: 9a1b         	ldr	r2, [sp, #0x6c]
    6cfe: 931d         	str	r3, [sp, #0x74]
    6d00: 4650         	mov	r0, r10
    6d02: 9b1e         	ldr	r3, [sp, #0x78]
    6d04: 921e         	str	r2, [sp, #0x78]
    6d06: 9a1c         	ldr	r2, [sp, #0x70]
    6d08: 931b         	str	r3, [sp, #0x6c]
    6d0a: 9b1f         	ldr	r3, [sp, #0x7c]
    6d0c: 921f         	str	r2, [sp, #0x7c]
    6d0e: 931c         	str	r3, [sp, #0x70]
    6d10: f7fd e91c    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x2dc8
    6d14: ab29         	add	r3, sp, #0xa4
    6d16: 930b         	str	r3, [sp, #0x2c]
    6d18: f003 fc10    	bl	0xa53c <getPmLogContext()> @ imm = #0x3820
    6d1c: b140         	cbz	r0, 0x6d30 <ConnectionManager::connect(LSMessage&)+0x228> @ imm = #0x10
    6d1e: ab29         	add	r3, sp, #0xa4
    6d20: f10d 0b8c    	add.w	r11, sp, #0x8c
    6d24: 930b         	str	r3, [sp, #0x2c]
    6d26: f003 fc09    	bl	0xa53c <getPmLogContext()> @ imm = #0x3812
    6d2a: 6803         	ldr	r3, [r0]
    6d2c: 2b06         	cmp	r3, #0x6
    6d2e: dd31         	ble	0x6d94 <ConnectionManager::connect(LSMessage&)+0x28c> @ imm = #0x62
    6d30: ab29         	add	r3, sp, #0xa4
    6d32: f10d 0b8c    	add.w	r11, sp, #0x8c
    6d36: 930b         	str	r3, [sp, #0x2c]
    6d38: f003 fc00    	bl	0xa53c <getPmLogContext()> @ imm = #0x3800
    6d3c: 9b23         	ldr	r3, [sp, #0x8c]
    6d3e: 4681         	mov	r9, r0
    6d40: 990c         	ldr	r1, [sp, #0x30]
    6d42: 2200         	movs	r2, #0x0
    6d44: 4630         	mov	r0, r6
    6d46: f8dd a0a4    	ldr.w	r10, [sp, #0xa4]
    6d4a: 461f         	mov	r7, r3
    6d4c: ab29         	add	r3, sp, #0xa4
    6d4e: 930b         	str	r3, [sp, #0x2c]
    6d50: f7fd ea4c    	blx	0x41ec <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt> @ imm = #-0x2b68
    6d54: 9b3b         	ldr	r3, [sp, #0xec]
    6d56: 4648         	mov	r0, r9
    6d58: 2107         	movs	r1, #0x7
    6d5a: e9cd a706    	strd	r10, r7, [sp, #24]
    6d5e: 9308         	str	r3, [sp, #0x20]
    6d60: f8df 3760    	ldr.w	r3, [pc, #0x760]        @ 0x74c4 <ConnectionManager::connect(LSMessage&)+0x9bc> literal=0x711c
    6d64: 447b         	add	r3, pc => raw=0xde84 ghidra=0x1de84 file=56964 "connect"
    6d66: 9305         	str	r3, [sp, #0x14]
    6d68: f8df 375c    	ldr.w	r3, [pc, #0x75c]        @ 0x74c8 <ConnectionManager::connect(LSMessage&)+0x9c0> literal=0x69e0
    6d6c: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    6d6e: 9304         	str	r3, [sp, #0x10]
    6d70: f8df 3758    	ldr.w	r3, [pc, #0x758]        @ 0x74cc <ConnectionManager::connect(LSMessage&)+0x9c4> literal=0x7114
    6d74: 447b         	add	r3, pc => raw=0xde8c ghidra=0x1de8c file=56972 "%s:%s() Audio connect request for source %s, sink %s, pipelineAttribute %s"
    6d76: 9303         	str	r3, [sp, #0xc]
    6d78: 2300         	movs	r3, #0x0
    6d7a: 461a         	mov	r2, r3
    6d7c: e9cd 3301    	strd	r3, r3, [sp, #4]
    6d80: 9300         	str	r3, [sp]
    6d82: f7fd e94c    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x2d68
    6d86: 983b         	ldr	r0, [sp, #0xec]
    6d88: 42a0         	cmp	r0, r4
    6d8a: d003         	beq	0x6d94 <ConnectionManager::connect(LSMessage&)+0x28c> @ imm = #0x6
    6d8c: 993d         	ldr	r1, [sp, #0xf4]
    6d8e: 3101         	adds	r1, #0x1
    6d90: f7fd e8b6    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2e94
    6d94: ab29         	add	r3, sp, #0xa4
    6d96: f10d 0b8c    	add.w	r11, sp, #0x8c
    6d9a: 9811         	ldr	r0, [sp, #0x44]
    6d9c: 465a         	mov	r2, r11
    6d9e: 4619         	mov	r1, r3
    6da0: f10d 0980    	add.w	r9, sp, #0x80
    6da4: 930b         	str	r3, [sp, #0x2c]
    6da6: f7ff f971    	bl	0x608c <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)> @ imm = #-0xd1e
    6daa: 4682         	mov	r10, r0
    6dac: 4648         	mov	r0, r9
    6dae: f7fd e804    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x2ff8
    6db2: f1ba 0f00    	cmp.w	r10, #0x0
    6db6: f000 8122    	beq.w	0x6ffe <ConnectionManager::connect(LSMessage&)+0x4f6> @ imm = #0x244
    6dba: f003 fbbf    	bl	0xa53c <getPmLogContext()> @ imm = #0x377e
    6dbe: 2800         	cmp	r0, #0x0
    6dc0: f000 80e7    	beq.w	0x6f92 <ConnectionManager::connect(LSMessage&)+0x48a> @ imm = #0x1ce
    6dc4: f003 fbba    	bl	0xa53c <getPmLogContext()> @ imm = #0x3774
    6dc8: 6803         	ldr	r3, [r0]
    6dca: 2b05         	cmp	r3, #0x5
    6dcc: f300 80e1    	bgt.w	0x6f92 <ConnectionManager::connect(LSMessage&)+0x48a> @ imm = #0x1c2
    6dd0: 2100         	movs	r1, #0x0
    6dd2: 4628         	mov	r0, r5
    6dd4: f7fd e80e    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x2fe4
    6dd8: f8df 26f4    	ldr.w	r2, [pc, #0x6f4]        @ 0x74d0 <ConnectionManager::connect(LSMessage&)+0x9c8> literal=0x6938
    6ddc: 4623         	mov	r3, r4
    6dde: 943b         	str	r4, [sp, #0xec]
    6de0: f04f 0e0b    	mov.w	lr, #0xb
    6de4: f04f 0c00    	mov.w	r12, #0x0
    6de8: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    6dea: ca07         	ldm	r2, {r0, r1, r2}
    6dec: c303         	stm	r3!, {r0, r1}
    6dee: 4631         	mov	r1, r6
    6df0: f823 2b02    	strh	r2, [r3], #2
    6df4: 0c12         	lsrs	r2, r2, #0x10
    6df6: 4648         	mov	r0, r9
    6df8: 701a         	strb	r2, [r3]
    6dfa: 462a         	mov	r2, r5
    6dfc: f8cd e0f0    	str.w	lr, [sp, #0xf0]
    6e00: f88d c0ff    	strb.w	r12, [sp, #0xff]
    6e04: f7fc ef66    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3134
    6e08: 983b         	ldr	r0, [sp, #0xec]
    6e0a: 42a0         	cmp	r0, r4
    6e0c: d003         	beq	0x6e16 <ConnectionManager::connect(LSMessage&)+0x30e> @ imm = #0x6
    6e0e: 993d         	ldr	r1, [sp, #0xf4]
    6e10: 3101         	adds	r1, #0x1
    6e12: f7fd e876    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2f14
    6e16: 4628         	mov	r0, r5
    6e18: f7fc ef9e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x30c4
    6e1c: f8df 16b4    	ldr.w	r1, [pc, #0x6b4]        @ 0x74d4 <ConnectionManager::connect(LSMessage&)+0x9cc> literal=0x1a21e
    6e20: 4628         	mov	r0, r5
    6e22: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    6e24: 31dc         	adds	r1, #0xdc
    6e26: f7fd e982    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x2cfc
    6e2a: f8df 26ac    	ldr.w	r2, [pc, #0x6ac]        @ 0x74d8 <ConnectionManager::connect(LSMessage&)+0x9d0> literal=0x781e
    6e2e: 4623         	mov	r3, r4
    6e30: 943b         	str	r4, [sp, #0xec]
    6e32: f04f 0e09    	mov.w	lr, #0x9
    6e36: f04f 0c00    	mov.w	r12, #0x0
    6e3a: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    6e3c: ca07         	ldm	r2, {r0, r1, r2}
    6e3e: c303         	stm	r3!, {r0, r1}
    6e40: 4631         	mov	r1, r6
    6e42: 701a         	strb	r2, [r3]
    6e44: 4648         	mov	r0, r9
    6e46: 462a         	mov	r2, r5
    6e48: f8cd e0f0    	str.w	lr, [sp, #0xf0]
    6e4c: f88d c0fd    	strb.w	r12, [sp, #0xfd]
    6e50: f7fc ef40    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3180
    6e54: 983b         	ldr	r0, [sp, #0xec]
    6e56: 42a0         	cmp	r0, r4
    6e58: d003         	beq	0x6e62 <ConnectionManager::connect(LSMessage&)+0x35a> @ imm = #0x6
    6e5a: 993d         	ldr	r1, [sp, #0xf4]
    6e5c: 3101         	adds	r1, #0x1
    6e5e: f7fd e850    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2f60
    6e62: 4628         	mov	r0, r5
    6e64: f7fc ef78    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3110
    6e68: 21cd         	movs	r1, #0xcd
    6e6a: 4628         	mov	r0, r5
    6e6c: f7fd e964    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x2d38
    6e70: f8df 2668    	ldr.w	r2, [pc, #0x668]        @ 0x74dc <ConnectionManager::connect(LSMessage&)+0x9d4> literal=0x77e4
    6e74: 4623         	mov	r3, r4
    6e76: 943b         	str	r4, [sp, #0xec]
    6e78: f04f 0e09    	mov.w	lr, #0x9
    6e7c: f04f 0c00    	mov.w	r12, #0x0
    6e80: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    6e82: ca07         	ldm	r2, {r0, r1, r2}
    6e84: c303         	stm	r3!, {r0, r1}
    6e86: 4631         	mov	r1, r6
    6e88: 701a         	strb	r2, [r3]
    6e8a: 4648         	mov	r0, r9
    6e8c: 462a         	mov	r2, r5
    6e8e: f8cd e0f0    	str.w	lr, [sp, #0xf0]
    6e92: f88d c0fd    	strb.w	r12, [sp, #0xfd]
    6e96: f7fc ef1e    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x31c4
    6e9a: 983b         	ldr	r0, [sp, #0xec]
    6e9c: 42a0         	cmp	r0, r4
    6e9e: d003         	beq	0x6ea8 <ConnectionManager::connect(LSMessage&)+0x3a0> @ imm = #0x6
    6ea0: 993d         	ldr	r1, [sp, #0xf4]
    6ea2: 3101         	adds	r1, #0x1
    6ea4: f7fd e82c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2fa8
    6ea8: 4628         	mov	r0, r5
    6eaa: f7fc ef56    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3154
    6eae: 990b         	ldr	r1, [sp, #0x2c]
    6eb0: 4628         	mov	r0, r5
    6eb2: f7fd e93c    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x2d88
    6eb6: f8df 3628    	ldr.w	r3, [pc, #0x628]        @ 0x74e0 <ConnectionManager::connect(LSMessage&)+0x9d8> literal=0x6c2a
    6eba: 2206         	movs	r2, #0x6
    6ebc: 923c         	str	r2, [sp, #0xf0]
    6ebe: 2200         	movs	r2, #0x0
    6ec0: f88d 20fa    	strb.w	r2, [sp, #0xfa]
    6ec4: 462a         	mov	r2, r5
    6ec6: 447b         	add	r3, pc => raw=0xdaf4 ghidra=0x1daf4 file=56052 "source"
    6ec8: 943b         	str	r4, [sp, #0xec]
    6eca: e893 0003    	ldm.w	r3, {r0, r1}
    6ece: 903d         	str	r0, [sp, #0xf4]
    6ed0: 4648         	mov	r0, r9
    6ed2: f8ad 10f8    	strh.w	r1, [sp, #0xf8]
    6ed6: 4631         	mov	r1, r6
    6ed8: f7fc eefc    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3208
    6edc: 983b         	ldr	r0, [sp, #0xec]
    6ede: 42a0         	cmp	r0, r4
    6ee0: d003         	beq	0x6eea <ConnectionManager::connect(LSMessage&)+0x3e2> @ imm = #0x6
    6ee2: 993d         	ldr	r1, [sp, #0xf4]
    6ee4: 3101         	adds	r1, #0x1
    6ee6: f7fd e80c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x2fe8
    6eea: 4628         	mov	r0, r5
    6eec: f7fc ef34    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3198
    6ef0: 4659         	mov	r1, r11
    6ef2: 4628         	mov	r0, r5
    6ef4: f7fd e91a    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x2dcc
    6ef8: f646 1373    	movw	r3, #0x6973
    6efc: 462a         	mov	r2, r5
    6efe: f6c6 336e    	movt	r3, #0x6b6e
    6f02: 4631         	mov	r1, r6
    6f04: 933d         	str	r3, [sp, #0xf4]
    6f06: 2304         	movs	r3, #0x4
    6f08: 933c         	str	r3, [sp, #0xf0]
    6f0a: 4648         	mov	r0, r9
    6f0c: 2300         	movs	r3, #0x0
    6f0e: 943b         	str	r4, [sp, #0xec]
    6f10: f88d 30f8    	strb.w	r3, [sp, #0xf8]
    6f14: f7fc eede    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3244
    6f18: 983b         	ldr	r0, [sp, #0xec]
    6f1a: 42a0         	cmp	r0, r4
    6f1c: d003         	beq	0x6f26 <ConnectionManager::connect(LSMessage&)+0x41e> @ imm = #0x6
    6f1e: 993d         	ldr	r1, [sp, #0xf4]
    6f20: 3101         	adds	r1, #0x1
    6f22: f7fc efee    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3024
    6f26: 4628         	mov	r0, r5
    6f28: f7fc ef16    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x31d4
    6f2c: 4649         	mov	r1, r9
    6f2e: a815         	add	r0, sp, #0x54
    6f30: f002 ffae    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #0x2f5c
    6f34: 4648         	mov	r0, r9
    6f36: f7fc ef10    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x31e0
    6f3a: 982f         	ldr	r0, [sp, #0xbc]
    6f3c: 9b10         	ldr	r3, [sp, #0x40]
    6f3e: 4298         	cmp	r0, r3
    6f40: d003         	beq	0x6f4a <ConnectionManager::connect(LSMessage&)+0x442> @ imm = #0x6
    6f42: 9931         	ldr	r1, [sp, #0xc4]
    6f44: 3101         	adds	r1, #0x1
    6f46: f7fc efdc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3048
    6f4a: 980c         	ldr	r0, [sp, #0x30]
    6f4c: f7fc ef04    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x31f8
    6f50: 9829         	ldr	r0, [sp, #0xa4]
    6f52: 9b0e         	ldr	r3, [sp, #0x38]
    6f54: 4298         	cmp	r0, r3
    6f56: d003         	beq	0x6f60 <ConnectionManager::connect(LSMessage&)+0x458> @ imm = #0x6
    6f58: 992b         	ldr	r1, [sp, #0xac]
    6f5a: 3101         	adds	r1, #0x1
    6f5c: f7fc efd0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3060
    6f60: 9823         	ldr	r0, [sp, #0x8c]
    6f62: 9b0d         	ldr	r3, [sp, #0x34]
    6f64: 4298         	cmp	r0, r3
    6f66: d003         	beq	0x6f70 <ConnectionManager::connect(LSMessage&)+0x468> @ imm = #0x6
    6f68: 9925         	ldr	r1, [sp, #0x94]
    6f6a: 3101         	adds	r1, #0x1
    6f6c: f7fc efc8    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3070
    6f70: 4640         	mov	r0, r8
    6f72: f7fc eef2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x321c
    6f76: 9815         	ldr	r0, [sp, #0x54]
    6f78: b108         	cbz	r0, 0x6f7e <ConnectionManager::connect(LSMessage&)+0x476> @ imm = #0x2
    6f7a: f7fd e868    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x2f30
    6f7e: 2001         	movs	r0, #0x1
    6f80: b04f         	add	sp, #0x13c
    6f82: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    6f86: 4623         	mov	r3, r4
    6f88: 2800         	cmp	r0, #0x0
    6f8a: f43f ae17    	beq.w	0x6bbc <ConnectionManager::connect(LSMessage&)+0xb4> @ imm = #-0x3d2
    6f8e: 4620         	mov	r0, r4
    6f90: e02e         	b	0x6ff0 <ConnectionManager::connect(LSMessage&)+0x4e8> @ imm = #0x5c
    6f92: f003 fad3    	bl	0xa53c <getPmLogContext()> @ imm = #0x35a6
    6f96: 9b23         	ldr	r3, [sp, #0x8c]
    6f98: 2200         	movs	r2, #0x0
    6f9a: 2106         	movs	r1, #0x6
    6f9c: e9cd 2201    	strd	r2, r2, [sp, #4]
    6fa0: 9200         	str	r2, [sp]
    6fa2: 9305         	str	r3, [sp, #0x14]
    6fa4: 9b29         	ldr	r3, [sp, #0xa4]
    6fa6: 9304         	str	r3, [sp, #0x10]
    6fa8: f8df 3538    	ldr.w	r3, [pc, #0x538]        @ 0x74e4 <ConnectionManager::connect(LSMessage&)+0x9dc> literal=0x6f60
    6fac: 447b         	add	r3, pc => raw=0xdf10 ghidra=0x1df10 file=57104 "Audio is already connected (source:%s, sink:%s)"
    6fae: 9303         	str	r3, [sp, #0xc]
    6fb0: f8df 3534    	ldr.w	r3, [pc, #0x534]        @ 0x74e8 <ConnectionManager::connect(LSMessage&)+0x9e0> literal=0x6898
    6fb4: 447b         	add	r3, pc => raw=0xd850 ghidra=0x1d850 file=55376 "AUDIO_CONNECT_CALL"
    6fb6: f7fd e832    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x2f9c
    6fba: e709         	b	0x6dd0 <ConnectionManager::connect(LSMessage&)+0x2c8> @ imm = #-0x1ee
    6fbc: f8df 152c    	ldr.w	r1, [pc, #0x52c]        @ 0x74ec <ConnectionManager::connect(LSMessage&)+0x9e4> literal=0x1a074
    6fc0: ac29         	add	r4, sp, #0xa4
    6fc2: 462b         	mov	r3, r5
    6fc4: 2203         	movs	r2, #0x3
    6fc6: a815         	add	r0, sp, #0x54
    6fc8: f10d 0b8c    	add.w	r11, sp, #0x8c
    6fcc: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    6fce: 940b         	str	r4, [sp, #0x2c]
    6fd0: 311c         	adds	r1, #0x1c
    6fd2: f002 fe3b    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0x2c76
    6fd6: e7b0         	b	0x6f3a <ConnectionManager::connect(LSMessage&)+0x432> @ imm = #-0xa0
    6fd8: ab29         	add	r3, sp, #0xa4
    6fda: 2200         	movs	r2, #0x0
    6fdc: a935         	add	r1, sp, #0xd4
    6fde: 4630         	mov	r0, r6
    6fe0: f10d 0b8c    	add.w	r11, sp, #0x8c
    6fe4: 930b         	str	r3, [sp, #0x2c]
    6fe6: f7fd e846    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x2f74
    6fea: 9b35         	ldr	r3, [sp, #0xd4]
    6fec: 903b         	str	r0, [sp, #0xec]
    6fee: 933d         	str	r3, [sp, #0xf4]
    6ff0: 462a         	mov	r2, r5
    6ff2: 4651         	mov	r1, r10
    6ff4: f7fc efc8    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x3070
    6ff8: 9d35         	ldr	r5, [sp, #0xd4]
    6ffa: 9b3b         	ldr	r3, [sp, #0xec]
    6ffc: e5de         	b	0x6bbc <ConnectionManager::connect(LSMessage&)+0xb4> @ imm = #-0x444
    6ffe: 9b0c         	ldr	r3, [sp, #0x30]
    7000: 465a         	mov	r2, r11
    7002: 990b         	ldr	r1, [sp, #0x2c]
    7004: 9811         	ldr	r0, [sp, #0x44]
    7006: f7fe fafd    	bl	0x5604 <ConnectionManager::connect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue&)> @ imm = #-0x1a06
    700a: 2800         	cmp	r0, #0x0
    700c: f040 8117    	bne.w	0x723e <ConnectionManager::connect(LSMessage&)+0x736> @ imm = #0x22e
    7010: f003 fa94    	bl	0xa53c <getPmLogContext()> @ imm = #0x3528
    7014: b120         	cbz	r0, 0x7020 <ConnectionManager::connect(LSMessage&)+0x518> @ imm = #0x8
    7016: f003 fa91    	bl	0xa53c <getPmLogContext()> @ imm = #0x3522
    701a: 6803         	ldr	r3, [r0]
    701c: 2b06         	cmp	r3, #0x6
    701e: dd15         	ble	0x704c <ConnectionManager::connect(LSMessage&)+0x544> @ imm = #0x2a
    7020: f003 fa8c    	bl	0xa53c <getPmLogContext()> @ imm = #0x3518
    7024: f8df 34c8    	ldr.w	r3, [pc, #0x4c8]        @ 0x74f0 <ConnectionManager::connect(LSMessage&)+0x9e8> literal=0x6e56
    7028: 2107         	movs	r1, #0x7
    702a: 447b         	add	r3, pc => raw=0xde84 ghidra=0x1de84 file=56964 "connect"
    702c: 9305         	str	r3, [sp, #0x14]
    702e: f8df 34c4    	ldr.w	r3, [pc, #0x4c4]        @ 0x74f4 <ConnectionManager::connect(LSMessage&)+0x9ec> literal=0x671a
    7032: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    7034: 9304         	str	r3, [sp, #0x10]
    7036: f8df 34c0    	ldr.w	r3, [pc, #0x4c0]        @ 0x74f8 <ConnectionManager::connect(LSMessage&)+0x9f0> literal=0x6e9a
    703a: 447b         	add	r3, pc => raw=0xded8 ghidra=0x1ded8 file=57048 "%s:%s() Audio connect success"
    703c: 9303         	str	r3, [sp, #0xc]
    703e: 2300         	movs	r3, #0x0
    7040: 461a         	mov	r2, r3
    7042: e9cd 3301    	strd	r3, r3, [sp, #4]
    7046: 9300         	str	r3, [sp]
    7048: f7fc efe8    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x3030
    704c: 9a11         	ldr	r2, [sp, #0x44]
    704e: 943b         	str	r4, [sp, #0xec]
    7050: e9d2 3c0c    	ldrd	r3, r12, [r2, #48]
    7054: 6b92         	ldr	r2, [r2, #0x38]
    7056: ebac 0a03    	sub.w	r10, r12, r3
    705a: 2300         	movs	r3, #0x0
    705c: 933c         	str	r3, [sp, #0xf0]
    705e: 4594         	cmp	r12, r2
    7060: f88d 30f4    	strb.w	r3, [sp, #0xf4]
    7064: 9342         	str	r3, [sp, #0x108]
    7066: f88d 310c    	strb.w	r3, [sp, #0x10c]
    706a: 9348         	str	r3, [sp, #0x120]
    706c: f88d 3124    	strb.w	r3, [sp, #0x124]
    7070: f88d 3134    	strb.w	r3, [sp, #0x134]
    7074: ab43         	add	r3, sp, #0x10c
    7076: 9312         	str	r3, [sp, #0x48]
    7078: 9341         	str	r3, [sp, #0x104]
    707a: ab49         	add	r3, sp, #0x124
    707c: 9313         	str	r3, [sp, #0x4c]
    707e: 9347         	str	r3, [sp, #0x11c]
    7080: f000 816e    	beq.w	0x7360 <ConnectionManager::connect(LSMessage&)+0x858> @ imm = #0x2dc
    7084: f10c 0e08    	add.w	lr, r12, #0x8
    7088: f8cc e000    	str.w	lr, [r12]
    708c: 9b3b         	ldr	r3, [sp, #0xec]
    708e: 42a3         	cmp	r3, r4
    7090: f000 814f    	beq.w	0x7332 <ConnectionManager::connect(LSMessage&)+0x82a> @ imm = #0x29e
    7094: f8cc 3000    	str.w	r3, [r12]
    7098: 9b3d         	ldr	r3, [sp, #0xf4]
    709a: f8cc 3008    	str.w	r3, [r12, #0x8]
    709e: 9b3c         	ldr	r3, [sp, #0xf0]
    70a0: f10c 0e20    	add.w	lr, r12, #0x20
    70a4: 9a12         	ldr	r2, [sp, #0x48]
    70a6: f8cc 3004    	str.w	r3, [r12, #0x4]
    70aa: 2300         	movs	r3, #0x0
    70ac: 933c         	str	r3, [sp, #0xf0]
    70ae: f88d 30f4    	strb.w	r3, [sp, #0xf4]
    70b2: 943b         	str	r4, [sp, #0xec]
    70b4: f8cc e018    	str.w	lr, [r12, #0x18]
    70b8: 9b41         	ldr	r3, [sp, #0x104]
    70ba: 4293         	cmp	r3, r2
    70bc: f000 8163    	beq.w	0x7386 <ConnectionManager::connect(LSMessage&)+0x87e> @ imm = #0x2c6
    70c0: f8cc 3018    	str.w	r3, [r12, #0x18]
    70c4: 9b43         	ldr	r3, [sp, #0x10c]
    70c6: f8cc 3020    	str.w	r3, [r12, #0x20]
    70ca: 9b42         	ldr	r3, [sp, #0x108]
    70cc: f10c 0e38    	add.w	lr, r12, #0x38
    70d0: 9a13         	ldr	r2, [sp, #0x4c]
    70d2: f8cc 301c    	str.w	r3, [r12, #0x1c]
    70d6: 9b12         	ldr	r3, [sp, #0x48]
    70d8: 9341         	str	r3, [sp, #0x104]
    70da: 2300         	movs	r3, #0x0
    70dc: 9342         	str	r3, [sp, #0x108]
    70de: f88d 310c    	strb.w	r3, [sp, #0x10c]
    70e2: f8cc e030    	str.w	lr, [r12, #0x30]
    70e6: 9b47         	ldr	r3, [sp, #0x11c]
    70e8: 4293         	cmp	r3, r2
    70ea: f000 8157    	beq.w	0x739c <ConnectionManager::connect(LSMessage&)+0x894> @ imm = #0x2ae
    70ee: f8cc 3030    	str.w	r3, [r12, #0x30]
    70f2: 9b49         	ldr	r3, [sp, #0x124]
    70f4: f8cc 3038    	str.w	r3, [r12, #0x38]
    70f8: 9911         	ldr	r1, [sp, #0x44]
    70fa: 9a48         	ldr	r2, [sp, #0x120]
    70fc: 6b4b         	ldr	r3, [r1, #0x34]
    70fe: f8cc 2034    	str.w	r2, [r12, #0x34]
    7102: f89d 2134    	ldrb.w	r2, [sp, #0x134]
    7106: 334c         	adds	r3, #0x4c
    7108: f88c 2048    	strb.w	r2, [r12, #0x48]
    710c: 634b         	str	r3, [r1, #0x34]
    710e: 6b0b         	ldr	r3, [r1, #0x30]
    7110: 449a         	add	r10, r3
    7112: 9841         	ldr	r0, [sp, #0x104]
    7114: 9b12         	ldr	r3, [sp, #0x48]
    7116: 4298         	cmp	r0, r3
    7118: d003         	beq	0x7122 <ConnectionManager::connect(LSMessage&)+0x61a> @ imm = #0x6
    711a: 9943         	ldr	r1, [sp, #0x10c]
    711c: 3101         	adds	r1, #0x1
    711e: f7fc eef0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3220
    7122: 983b         	ldr	r0, [sp, #0xec]
    7124: 42a0         	cmp	r0, r4
    7126: d003         	beq	0x7130 <ConnectionManager::connect(LSMessage&)+0x628> @ imm = #0x6
    7128: 993d         	ldr	r1, [sp, #0xf4]
    712a: 3101         	adds	r1, #0x1
    712c: f7fc eee8    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3230
    7130: 4659         	mov	r1, r11
    7132: f10a 0018    	add.w	r0, r10, #0x18
    7136: f7fd e86c    	blx	0x4210 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt> @ imm = #-0x2f28
    713a: 990b         	ldr	r1, [sp, #0x2c]
    713c: 4650         	mov	r0, r10
    713e: f7fd e868    	blx	0x4210 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt> @ imm = #-0x2f30
    7142: 2101         	movs	r1, #0x1
    7144: 4628         	mov	r0, r5
    7146: f7fc ee56    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x3354
    714a: 4aec         	ldr	r2, [pc, #0x3b0]        @ 0x74fc <ConnectionManager::connect(LSMessage&)+0x9f4> literal=0x65c8
    714c: 4623         	mov	r3, r4
    714e: 943b         	str	r4, [sp, #0xec]
    7150: f04f 0e0b    	mov.w	lr, #0xb
    7154: f04f 0c00    	mov.w	r12, #0x0
    7158: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    715a: ca07         	ldm	r2, {r0, r1, r2}
    715c: c303         	stm	r3!, {r0, r1}
    715e: 4631         	mov	r1, r6
    7160: f823 2b02    	strh	r2, [r3], #2
    7164: 0c12         	lsrs	r2, r2, #0x10
    7166: 4648         	mov	r0, r9
    7168: 701a         	strb	r2, [r3]
    716a: 462a         	mov	r2, r5
    716c: f8cd e0f0    	str.w	lr, [sp, #0xf0]
    7170: f88d c0ff    	strb.w	r12, [sp, #0xff]
    7174: f7fc edae    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x34a4
    7178: 983b         	ldr	r0, [sp, #0xec]
    717a: 42a0         	cmp	r0, r4
    717c: d003         	beq	0x7186 <ConnectionManager::connect(LSMessage&)+0x67e> @ imm = #0x6
    717e: 993d         	ldr	r1, [sp, #0xf4]
    7180: 3101         	adds	r1, #0x1
    7182: f7fc eebe    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3284
    7186: 4628         	mov	r0, r5
    7188: f7fc ede6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3434
    718c: 990b         	ldr	r1, [sp, #0x2c]
    718e: 4628         	mov	r0, r5
    7190: f7fc efcc    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x3068
    7194: 4bda         	ldr	r3, [pc, #0x368]        @ 0x7500 <ConnectionManager::connect(LSMessage&)+0x9f8> literal=0x694e
    7196: 2206         	movs	r2, #0x6
    7198: 923c         	str	r2, [sp, #0xf0]
    719a: 2200         	movs	r2, #0x0
    719c: f88d 20fa    	strb.w	r2, [sp, #0xfa]
    71a0: 462a         	mov	r2, r5
    71a2: 447b         	add	r3, pc => raw=0xdaf4 ghidra=0x1daf4 file=56052 "source"
    71a4: 943b         	str	r4, [sp, #0xec]
    71a6: e893 0003    	ldm.w	r3, {r0, r1}
    71aa: 903d         	str	r0, [sp, #0xf4]
    71ac: 4648         	mov	r0, r9
    71ae: f8ad 10f8    	strh.w	r1, [sp, #0xf8]
    71b2: 4631         	mov	r1, r6
    71b4: f7fc ed8e    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x34e4
    71b8: 983b         	ldr	r0, [sp, #0xec]
    71ba: 42a0         	cmp	r0, r4
    71bc: d003         	beq	0x71c6 <ConnectionManager::connect(LSMessage&)+0x6be> @ imm = #0x6
    71be: 993d         	ldr	r1, [sp, #0xf4]
    71c0: 3101         	adds	r1, #0x1
    71c2: f7fc ee9e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x32c4
    71c6: 4628         	mov	r0, r5
    71c8: f7fc edc6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3474
    71cc: 4659         	mov	r1, r11
    71ce: 4628         	mov	r0, r5
    71d0: f7fc efac    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x30a8
    71d4: f646 1373    	movw	r3, #0x6973
    71d8: 462a         	mov	r2, r5
    71da: f6c6 336e    	movt	r3, #0x6b6e
    71de: 4631         	mov	r1, r6
    71e0: 933d         	str	r3, [sp, #0xf4]
    71e2: 2304         	movs	r3, #0x4
    71e4: 933c         	str	r3, [sp, #0xf0]
    71e6: 4648         	mov	r0, r9
    71e8: 2300         	movs	r3, #0x0
    71ea: 943b         	str	r4, [sp, #0xec]
    71ec: f88d 30f8    	strb.w	r3, [sp, #0xf8]
    71f0: f7fc ed70    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3520
    71f4: e690         	b	0x6f18 <ConnectionManager::connect(LSMessage&)+0x410> @ imm = #-0x2e0
    71f6: b13a         	cbz	r2, 0x7208 <ConnectionManager::connect(LSMessage&)+0x700> @ imm = #0xe
    71f8: 2a01         	cmp	r2, #0x1
    71fa: f000 80ab    	beq.w	0x7354 <ConnectionManager::connect(LSMessage&)+0x84c> @ imm = #0x156
    71fe: 4621         	mov	r1, r4
    7200: f7fc eec2    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x327c
    7204: 9a3c         	ldr	r2, [sp, #0xf0]
    7206: 9829         	ldr	r0, [sp, #0xa4]
    7208: 2300         	movs	r3, #0x0
    720a: 922a         	str	r2, [sp, #0xa8]
    720c: 5483         	strb	r3, [r0, r2]
    720e: 983b         	ldr	r0, [sp, #0xec]
    7210: e555         	b	0x6cbe <ConnectionManager::connect(LSMessage&)+0x1b6> @ imm = #-0x556
    7212: b13a         	cbz	r2, 0x7224 <ConnectionManager::connect(LSMessage&)+0x71c> @ imm = #0xe
    7214: 2a01         	cmp	r2, #0x1
    7216: f000 8097    	beq.w	0x7348 <ConnectionManager::connect(LSMessage&)+0x840> @ imm = #0x12e
    721a: 4639         	mov	r1, r7
    721c: f7fc eeb4    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x3298
    7220: 9a36         	ldr	r2, [sp, #0xd8]
    7222: 9823         	ldr	r0, [sp, #0x8c]
    7224: 2300         	movs	r3, #0x0
    7226: 9224         	str	r2, [sp, #0x90]
    7228: 5483         	strb	r3, [r0, r2]
    722a: 9835         	ldr	r0, [sp, #0xd4]
    722c: e509         	b	0x6c42 <ConnectionManager::connect(LSMessage&)+0x13a> @ imm = #-0x5ee
    722e: 9735         	str	r7, [sp, #0xd4]
    7230: af37         	add	r7, sp, #0xdc
    7232: 4638         	mov	r0, r7
    7234: e505         	b	0x6c42 <ConnectionManager::connect(LSMessage&)+0x13a> @ imm = #-0x5f6
    7236: 943b         	str	r4, [sp, #0xec]
    7238: ac3d         	add	r4, sp, #0xf4
    723a: 4620         	mov	r0, r4
    723c: e53f         	b	0x6cbe <ConnectionManager::connect(LSMessage&)+0x1b6> @ imm = #-0x582
    723e: f003 f97d    	bl	0xa53c <getPmLogContext()> @ imm = #0x32fa
    7242: 2800         	cmp	r0, #0x0
    7244: d066         	beq	0x7314 <ConnectionManager::connect(LSMessage&)+0x80c> @ imm = #0xcc
    7246: f003 f979    	bl	0xa53c <getPmLogContext()> @ imm = #0x32f2
    724a: 6803         	ldr	r3, [r0]
    724c: 2b02         	cmp	r3, #0x2
    724e: dc61         	bgt	0x7314 <ConnectionManager::connect(LSMessage&)+0x80c> @ imm = #0xc2
    7250: 2100         	movs	r1, #0x0
    7252: 4628         	mov	r0, r5
    7254: f7fc edce    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x3464
    7258: 4aaa         	ldr	r2, [pc, #0x2a8]        @ 0x7504 <ConnectionManager::connect(LSMessage&)+0x9fc> literal=0x64ba
    725a: 4623         	mov	r3, r4
    725c: 943b         	str	r4, [sp, #0xec]
    725e: f04f 0e0b    	mov.w	lr, #0xb
    7262: f04f 0c00    	mov.w	r12, #0x0
    7266: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    7268: ca07         	ldm	r2, {r0, r1, r2}
    726a: c303         	stm	r3!, {r0, r1}
    726c: 4631         	mov	r1, r6
    726e: f823 2b02    	strh	r2, [r3], #2
    7272: 0c12         	lsrs	r2, r2, #0x10
    7274: 4648         	mov	r0, r9
    7276: 701a         	strb	r2, [r3]
    7278: 462a         	mov	r2, r5
    727a: f8cd e0f0    	str.w	lr, [sp, #0xf0]
    727e: f88d c0ff    	strb.w	r12, [sp, #0xff]
    7282: f7fc ed28    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x35b0
    7286: 983b         	ldr	r0, [sp, #0xec]
    7288: 42a0         	cmp	r0, r4
    728a: d003         	beq	0x7294 <ConnectionManager::connect(LSMessage&)+0x78c> @ imm = #0x6
    728c: 993d         	ldr	r1, [sp, #0xf4]
    728e: 3101         	adds	r1, #0x1
    7290: f7fc ee36    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3394
    7294: 4628         	mov	r0, r5
    7296: f7fc ed60    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3540
    729a: 499b         	ldr	r1, [pc, #0x26c]        @ 0x7508 <ConnectionManager::connect(LSMessage&)+0xa00> literal=0x19da2
    729c: 4628         	mov	r0, r5
    729e: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    72a0: 3164         	adds	r1, #0x64
    72a2: f7fc ef44    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x3178
    72a6: 4a99         	ldr	r2, [pc, #0x264]        @ 0x750c <ConnectionManager::connect(LSMessage&)+0xa04> literal=0x73a4
    72a8: 4623         	mov	r3, r4
    72aa: 943b         	str	r4, [sp, #0xec]
    72ac: f04f 0e09    	mov.w	lr, #0x9
    72b0: f04f 0c00    	mov.w	r12, #0x0
    72b4: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    72b6: ca07         	ldm	r2, {r0, r1, r2}
    72b8: c303         	stm	r3!, {r0, r1}
    72ba: 4631         	mov	r1, r6
    72bc: 701a         	strb	r2, [r3]
    72be: 4648         	mov	r0, r9
    72c0: 462a         	mov	r2, r5
    72c2: f8cd e0f0    	str.w	lr, [sp, #0xf0]
    72c6: f88d c0fd    	strb.w	r12, [sp, #0xfd]
    72ca: f7fc ed04    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x35f8
    72ce: 983b         	ldr	r0, [sp, #0xec]
    72d0: 42a0         	cmp	r0, r4
    72d2: d003         	beq	0x72dc <ConnectionManager::connect(LSMessage&)+0x7d4> @ imm = #0x6
    72d4: 993d         	ldr	r1, [sp, #0xf4]
    72d6: 3101         	adds	r1, #0x1
    72d8: f7fc ee12    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x33dc
    72dc: 4628         	mov	r0, r5
    72de: f7fc ed3c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3588
    72e2: 2114         	movs	r1, #0x14
    72e4: 4628         	mov	r0, r5
    72e6: f7fc ef28    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x31b0
    72ea: 4a89         	ldr	r2, [pc, #0x224]        @ 0x7510 <ConnectionManager::connect(LSMessage&)+0xa08> literal=0x736c
    72ec: 4623         	mov	r3, r4
    72ee: 943b         	str	r4, [sp, #0xec]
    72f0: f04f 0e09    	mov.w	lr, #0x9
    72f4: f04f 0c00    	mov.w	r12, #0x0
    72f8: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    72fa: ca07         	ldm	r2, {r0, r1, r2}
    72fc: c303         	stm	r3!, {r0, r1}
    72fe: 4631         	mov	r1, r6
    7300: 701a         	strb	r2, [r3]
    7302: 4648         	mov	r0, r9
    7304: 462a         	mov	r2, r5
    7306: f8cd e0f0    	str.w	lr, [sp, #0xf0]
    730a: f88d c0fd    	strb.w	r12, [sp, #0xfd]
    730e: f7fc ece2    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x363c
    7312: e601         	b	0x6f18 <ConnectionManager::connect(LSMessage&)+0x410> @ imm = #-0x3fe
    7314: f003 f912    	bl	0xa53c <getPmLogContext()> @ imm = #0x3224
    7318: 4b7e         	ldr	r3, [pc, #0x1f8]        @ 0x7514 <ConnectionManager::connect(LSMessage&)+0xa0c> literal=0x6bd0
    731a: 2200         	movs	r2, #0x0
    731c: 2103         	movs	r1, #0x3
    731e: e9cd 2201    	strd	r2, r2, [sp, #4]
    7322: 9200         	str	r2, [sp]
    7324: 447b         	add	r3, pc => raw=0xdef8 ghidra=0x1def8 file=57080 "Audio is not connected"
    7326: 9303         	str	r3, [sp, #0xc]
    7328: 4b7b         	ldr	r3, [pc, #0x1ec]        @ 0x7518 <ConnectionManager::connect(LSMessage&)+0xa10> literal=0x64ee
    732a: 447b         	add	r3, pc => raw=0xd81c ghidra=0x1d81c file=55324 "AUDIO_DRIVER_ERROR"
    732c: f7fc ee76    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x3314
    7330: e78e         	b	0x7250 <ConnectionManager::connect(LSMessage&)+0x748> @ imm = #-0xe4
    7332: 4627         	mov	r7, r4
    7334: cf0f         	ldm	r7!, {r0, r1, r2, r3}
    7336: f8cc 0008    	str.w	r0, [r12, #0x8]
    733a: f8cc 100c    	str.w	r1, [r12, #0xc]
    733e: f8cc 2010    	str.w	r2, [r12, #0x10]
    7342: f8cc 3014    	str.w	r3, [r12, #0x14]
    7346: e6aa         	b	0x709e <ConnectionManager::connect(LSMessage&)+0x596> @ imm = #-0x2ac
    7348: f89d 30dc    	ldrb.w	r3, [sp, #0xdc]
    734c: 7003         	strb	r3, [r0]
    734e: 9a36         	ldr	r2, [sp, #0xd8]
    7350: 9823         	ldr	r0, [sp, #0x8c]
    7352: e767         	b	0x7224 <ConnectionManager::connect(LSMessage&)+0x71c> @ imm = #-0x132
    7354: f89d 30f4    	ldrb.w	r3, [sp, #0xf4]
    7358: 7003         	strb	r3, [r0]
    735a: 9a3c         	ldr	r2, [sp, #0xf0]
    735c: 9829         	ldr	r0, [sp, #0xa4]
    735e: e753         	b	0x7208 <ConnectionManager::connect(LSMessage&)+0x700> @ imm = #-0x15a
    7360: 9f11         	ldr	r7, [sp, #0x44]
    7362: 4661         	mov	r1, r12
    7364: 4632         	mov	r2, r6
    7366: f107 0030    	add.w	r0, r7, #0x30
    736a: f002 fe6d    	bl	0xa048 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)> @ imm = #0x2cda
    736e: 6b3b         	ldr	r3, [r7, #0x30]
    7370: 9847         	ldr	r0, [sp, #0x11c]
    7372: 449a         	add	r10, r3
    7374: 9b13         	ldr	r3, [sp, #0x4c]
    7376: 4298         	cmp	r0, r3
    7378: f43f aecb    	beq.w	0x7112 <ConnectionManager::connect(LSMessage&)+0x60a> @ imm = #-0x26a
    737c: 9949         	ldr	r1, [sp, #0x124]
    737e: 3101         	adds	r1, #0x1
    7380: f7fc edbe    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3484
    7384: e6c5         	b	0x7112 <ConnectionManager::connect(LSMessage&)+0x60a> @ imm = #-0x276
    7386: 4617         	mov	r7, r2
    7388: cf0f         	ldm	r7!, {r0, r1, r2, r3}
    738a: f8cc 0020    	str.w	r0, [r12, #0x20]
    738e: f8cc 1024    	str.w	r1, [r12, #0x24]
    7392: f8cc 2028    	str.w	r2, [r12, #0x28]
    7396: f8cc 302c    	str.w	r3, [r12, #0x2c]
    739a: e696         	b	0x70ca <ConnectionManager::connect(LSMessage&)+0x5c2> @ imm = #-0x2d4
    739c: 4617         	mov	r7, r2
    739e: cf0f         	ldm	r7!, {r0, r1, r2, r3}
    73a0: f8cc 0038    	str.w	r0, [r12, #0x38]
    73a4: f8cc 103c    	str.w	r1, [r12, #0x3c]
    73a8: f8cc 2040    	str.w	r2, [r12, #0x40]
    73ac: f8cc 3044    	str.w	r3, [r12, #0x44]
    73b0: e6a2         	b	0x70f8 <ConnectionManager::connect(LSMessage&)+0x5f0> @ imm = #-0x2bc
    73b2: 485a         	ldr	r0, [pc, #0x168]        @ 0x751c <ConnectionManager::connect(LSMessage&)+0xa14> literal=0x62d8
    73b4: ab29         	add	r3, sp, #0xa4
    73b6: f10d 0b8c    	add.w	r11, sp, #0x8c
    73ba: 930b         	str	r3, [sp, #0x2c]
    73bc: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    73be: f7fc eca2    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x36bc
    73c2: 4630         	mov	r0, r6
    73c4: f7fc ee94    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x32d8
    73c8: 4628         	mov	r0, r5
    73ca: f7fc ecc6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3674
    73ce: 4648         	mov	r0, r9
    73d0: f7fc ecc2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x367c
    73d4: 980f         	ldr	r0, [sp, #0x3c]
    73d6: f7fc ee8c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x32e8
    73da: 980c         	ldr	r0, [sp, #0x30]
    73dc: f7fc ecbc    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3688
    73e0: 980b         	ldr	r0, [sp, #0x2c]
    73e2: f7fc ee86    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x32f4
    73e6: 4658         	mov	r0, r11
    73e8: f7fc ee82    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x32fc
    73ec: 4640         	mov	r0, r8
    73ee: f7fc ecb4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3698
    73f2: 9815         	ldr	r0, [sp, #0x54]
    73f4: b108         	cbz	r0, 0x73fa <ConnectionManager::connect(LSMessage&)+0x8f2> @ imm = #0x2
    73f6: f7fc ee2a    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x33ac
    73fa: f7fc eec2    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x327c
    73fe: e7e0         	b	0x73c2 <ConnectionManager::connect(LSMessage&)+0x8ba> @ imm = #-0x40
    7400: e7df         	b	0x73c2 <ConnectionManager::connect(LSMessage&)+0x8ba> @ imm = #-0x42
    7402: e7de         	b	0x73c2 <ConnectionManager::connect(LSMessage&)+0x8ba> @ imm = #-0x44
    7404: e7dd         	b	0x73c2 <ConnectionManager::connect(LSMessage&)+0x8ba> @ imm = #-0x46
    7406: e7e5         	b	0x73d4 <ConnectionManager::connect(LSMessage&)+0x8cc> @ imm = #-0x36
    7408: ab29         	add	r3, sp, #0xa4
    740a: f10d 0b8c    	add.w	r11, sp, #0x8c
    740e: 930b         	str	r3, [sp, #0x2c]
    7410: e7e3         	b	0x73da <ConnectionManager::connect(LSMessage&)+0x8d2> @ imm = #-0x3a
    7412: 4628         	mov	r0, r5
    7414: f10d 0b8c    	add.w	r11, sp, #0x8c
    7418: f7fc ee6a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x332c
    741c: 4b26         	ldr	r3, [pc, #0x98]         @ 0x74b8 <ConnectionManager::connect(LSMessage&)+0x9b0> literal=0x210
    741e: 4630         	mov	r0, r6
    7420: f859 3003    	ldr.w	r3, [r9, r3]
    7424: 3308         	adds	r3, #0x8
    7426: 933b         	str	r3, [sp, #0xec]
    7428: f7fc ed90    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x34e0
    742c: ab29         	add	r3, sp, #0xa4
    742e: 930b         	str	r3, [sp, #0x2c]
    7430: e7d0         	b	0x73d4 <ConnectionManager::connect(LSMessage&)+0x8cc> @ imm = #-0x60
    7432: 4630         	mov	r0, r6
    7434: f10d 0b8c    	add.w	r11, sp, #0x8c
    7438: f7fc ee5a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x334c
    743c: f109 0308    	add.w	r3, r9, #0x8
    7440: 4628         	mov	r0, r5
    7442: 9335         	str	r3, [sp, #0xd4]
    7444: f7fc ed82    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x34fc
    7448: ab29         	add	r3, sp, #0xa4
    744a: 930b         	str	r3, [sp, #0x2c]
    744c: e7c2         	b	0x73d4 <ConnectionManager::connect(LSMessage&)+0x8cc> @ imm = #-0x7c
    744e: 4630         	mov	r0, r6
    7450: f10d 0b8c    	add.w	r11, sp, #0x8c
    7454: f7fc ee4c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x3368
    7458: ab29         	add	r3, sp, #0xa4
    745a: 930b         	str	r3, [sp, #0x2c]
    745c: e7ba         	b	0x73d4 <ConnectionManager::connect(LSMessage&)+0x8cc> @ imm = #-0x8c
    745e: e7b0         	b	0x73c2 <ConnectionManager::connect(LSMessage&)+0x8ba> @ imm = #-0xa0
    7460: e7af         	b	0x73c2 <ConnectionManager::connect(LSMessage&)+0x8ba> @ imm = #-0xa2
    7462: e7b4         	b	0x73ce <ConnectionManager::connect(LSMessage&)+0x8c6> @ imm = #-0x98
    7464: e7ad         	b	0x73c2 <ConnectionManager::connect(LSMessage&)+0x8ba> @ imm = #-0xa6
    7466: e7ac         	b	0x73c2 <ConnectionManager::connect(LSMessage&)+0x8ba> @ imm = #-0xa8
    7468: e7ab         	b	0x73c2 <ConnectionManager::connect(LSMessage&)+0x8ba> @ imm = #-0xaa
    746a: 9815         	ldr	r0, [sp, #0x54]
    746c: e7c3         	b	0x73f6 <ConnectionManager::connect(LSMessage&)+0x8ee> @ imm = #-0x7a
    746e: e7a8         	b	0x73c2 <ConnectionManager::connect(LSMessage&)+0x8ba> @ imm = #-0xb0
    7470: 9847         	ldr	r0, [sp, #0x11c]
    7472: 9b13         	ldr	r3, [sp, #0x4c]
    7474: 4298         	cmp	r0, r3
    7476: d003         	beq	0x7480 <ConnectionManager::connect(LSMessage&)+0x978> @ imm = #0x6
    7478: 9949         	ldr	r1, [sp, #0x124]
    747a: 3101         	adds	r1, #0x1
    747c: f7fc ed40    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3580
    7480: 9841         	ldr	r0, [sp, #0x104]
    7482: 9b12         	ldr	r3, [sp, #0x48]
    7484: 4298         	cmp	r0, r3
    7486: d003         	beq	0x7490 <ConnectionManager::connect(LSMessage&)+0x988> @ imm = #0x6
    7488: 9943         	ldr	r1, [sp, #0x10c]
    748a: 3101         	adds	r1, #0x1
    748c: f7fc ed38    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3590
    7490: 983b         	ldr	r0, [sp, #0xec]
    7492: 42a0         	cmp	r0, r4
    7494: d09b         	beq	0x73ce <ConnectionManager::connect(LSMessage&)+0x8c6> @ imm = #-0xca
    7496: 993d         	ldr	r1, [sp, #0xf4]
    7498: 3101         	adds	r1, #0x1
    749a: f7fc ed32    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x359c
    749e: e796         	b	0x73ce <ConnectionManager::connect(LSMessage&)+0x8c6> @ imm = #-0xd4
    74a0: ab29         	add	r3, sp, #0xa4
    74a2: f10d 0b8c    	add.w	r11, sp, #0x8c
    74a6: 930b         	str	r3, [sp, #0x2c]
    74a8: e79a         	b	0x73e0 <ConnectionManager::connect(LSMessage&)+0x8d8> @ imm = #-0xcc
    74aa: e7d0         	b	0x744e <ConnectionManager::connect(LSMessage&)+0x946> @ imm = #-0x60
    74ac: a29c         	adr	r2, #624 <ConnectionManager::disconnect(LSMessage&)+0x2c>
    74ae: 0001         	movs	r1, r0
    74b0: 7236         	strb	r6, [r6, #0x8]
    74b2: 0000         	movs	r0, r0
    74b4: 7270         	strb	r0, [r6, #0x9]
    74b6: 0000         	movs	r0, r0
    74b8: 0210         	lsls	r0, r2, #0x8
    74ba: 0000         	movs	r0, r0
    74bc: 6e76         	ldr	r6, [r6, #0x64]
    74be: 0000         	movs	r0, r0
    74c0: 717a         	strb	r2, [r7, #0x5]
    74c2: 0000         	movs	r0, r0
    74c4: 711c         	strb	r4, [r3, #0x4]
    74c6: 0000         	movs	r0, r0
    74c8: 69e0         	ldr	r0, [r4, #0x1c]
    74ca: 0000         	movs	r0, r0
    74cc: 7114         	strb	r4, [r2, #0x4]
    74ce: 0000         	movs	r0, r0
    74d0: 6938         	ldr	r0, [r7, #0x10]
    74d2: 0000         	movs	r0, r0
    74d4: a21e         	adr	r2, #120 <ConnectionManager::connect(LSMessage&)+0x9ee>
    74d6: 0001         	movs	r1, r0
    74d8: 781e         	ldrb	r6, [r3]
    74da: 0000         	movs	r0, r0
    74dc: 77e4         	strb	r4, [r4, #0x1f]
    74de: 0000         	movs	r0, r0
    74e0: 6c2a         	ldr	r2, [r5, #0x40]
    74e2: 0000         	movs	r0, r0
    74e4: 6f60         	ldr	r0, [r4, #0x74]
    74e6: 0000         	movs	r0, r0
    74e8: 6898         	ldr	r0, [r3, #0x8]
    74ea: 0000         	movs	r0, r0
    74ec: a074         	adr	r0, #464 <ConnectionManager::disconnect(LSMessage&)+0x44>
    74ee: 0001         	movs	r1, r0
    74f0: 6e56         	ldr	r6, [r2, #0x64]
    74f2: 0000         	movs	r0, r0
    74f4: 671a         	str	r2, [r3, #0x70]
    74f6: 0000         	movs	r0, r0
    74f8: 6e9a         	ldr	r2, [r3, #0x68]
    74fa: 0000         	movs	r0, r0
    74fc: 65c8         	str	r0, [r1, #0x5c]
    74fe: 0000         	movs	r0, r0
    7500: 694e         	ldr	r6, [r1, #0x14]
    7502: 0000         	movs	r0, r0
    7504: 64ba         	str	r2, [r7, #0x48]
    7506: 0000         	movs	r0, r0
    7508: 9da2         	ldr	r5, [sp, #0x288]
    750a: 0001         	movs	r1, r0
    750c: 73a4         	strb	r4, [r4, #0xe]
    750e: 0000         	movs	r0, r0
    7510: 736c         	strb	r4, [r5, #0xd]
    7512: 0000         	movs	r0, r0
    7514: 6bd0         	ldr	r0, [r2, #0x3c]
    7516: 0000         	movs	r0, r0
    7518: 64ee         	str	r6, [r5, #0x4c]
    751a: 0000         	movs	r0, r0
    751c: 62d8         	str	r0, [r3, #0x2c]
    751e: 0000         	movs	r0, r0

00007520 <ConnectionManager::disconnect(LSMessage&)>:
    7520: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    7524: 4681         	mov	r9, r0
    7526: b0b5         	sub	sp, #0xd4
    7528: 4608         	mov	r0, r1
    752a: 460c         	mov	r4, r1
    752c: f10d 0b40    	add.w	r11, sp, #0x40
    7530: 910e         	str	r1, [sp, #0x38]
    7532: f7fc edd8    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x3450
    7536: f8df 35b8    	ldr.w	r3, [pc, #0x5b8]        @ 0x7af0 <ConnectionManager::disconnect(LSMessage&)+0x5d0> literal=0x19880
    753a: 4658         	mov	r0, r11
    753c: 447b         	add	r3, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    753e: 930d         	str	r3, [sp, #0x34]
    7540: f7fc ee6c    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x3328
    7544: ab18         	add	r3, sp, #0x60
    7546: ae2e         	add	r6, sp, #0xb8
    7548: 9309         	str	r3, [sp, #0x24]
    754a: 2200         	movs	r2, #0x0
    754c: 9316         	str	r3, [sp, #0x58]
    754e: ab1e         	add	r3, sp, #0x78
    7550: 930a         	str	r3, [sp, #0x28]
    7552: a822         	add	r0, sp, #0x88
    7554: 931c         	str	r3, [sp, #0x70]
    7556: ab24         	add	r3, sp, #0x90
    7558: 930c         	str	r3, [sp, #0x30]
    755a: 4631         	mov	r1, r6
    755c: 9322         	str	r3, [sp, #0x88]
    755e: 238e         	movs	r3, #0x8e
    7560: 920f         	str	r2, [sp, #0x3c]
    7562: 9217         	str	r2, [sp, #0x5c]
    7564: f88d 2060    	strb.w	r2, [sp, #0x60]
    7568: 921d         	str	r2, [sp, #0x74]
    756a: f88d 2078    	strb.w	r2, [sp, #0x78]
    756e: 900b         	str	r0, [sp, #0x2c]
    7570: 932e         	str	r3, [sp, #0xb8]
    7572: f7fc ed80    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x3500
    7576: f8df 157c    	ldr.w	r1, [pc, #0x57c]        @ 0x7af4 <ConnectionManager::disconnect(LSMessage&)+0x5d4> literal=0x69b6
    757a: 228e         	movs	r2, #0x8e
    757c: 9b2e         	ldr	r3, [sp, #0xb8]
    757e: af16         	add	r7, sp, #0x58
    7580: 9022         	str	r0, [sp, #0x88]
    7582: f10d 0870    	add.w	r8, sp, #0x70
    7586: 4479         	add	r1, pc => raw=0xdf40 ghidra=0x1df40 file=57152 "{\"type\":\"object\",\"properties\":{\"sink\":{\"type\":\"string\"},\"source\":{\"type\":\"string\"}},\"required\":[\"source\",\"sink\"],\"additionalProperties\":false}"
    7588: 9324         	str	r3, [sp, #0x90]
    758a: f7fc ecfe    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x3604
    758e: 9b2e         	ldr	r3, [sp, #0xb8]
    7590: 2100         	movs	r1, #0x0
    7592: 9a22         	ldr	r2, [sp, #0x88]
    7594: 4620         	mov	r0, r4
    7596: 9323         	str	r3, [sp, #0x8c]
    7598: 54d1         	strb	r1, [r2, r3]
    759a: f7fc edfe    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x3404
    759e: ac30         	add	r4, sp, #0xc0
    75a0: 4682         	mov	r10, r0
    75a2: 942e         	str	r4, [sp, #0xb8]
    75a4: 2800         	cmp	r0, #0x0
    75a6: f000 8250    	beq.w	0x7a4a <ConnectionManager::disconnect(LSMessage&)+0x52a> @ imm = #0x4a0
    75aa: f7fc ed7c    	blx	0x40a4 <strlen@plt>     @ imm = #-0x3508
    75ae: 280f         	cmp	r0, #0xf
    75b0: 4605         	mov	r5, r0
    75b2: 9028         	str	r0, [sp, #0xa0]
    75b4: f200 818d    	bhi.w	0x78d2 <ConnectionManager::disconnect(LSMessage&)+0x3b2> @ imm = #0x31a
    75b8: 2801         	cmp	r0, #0x1
    75ba: f040 8156    	bne.w	0x786a <ConnectionManager::disconnect(LSMessage&)+0x34a> @ imm = #0x2ac
    75be: f89a 3000    	ldrb.w	r3, [r10]
    75c2: f88d 30c0    	strb.w	r3, [sp, #0xc0]
    75c6: 4623         	mov	r3, r4
    75c8: 2200         	movs	r2, #0x0
    75ca: 952f         	str	r5, [sp, #0xbc]
    75cc: 4659         	mov	r1, r11
    75ce: 555a         	strb	r2, [r3, r5]
    75d0: 4630         	mov	r0, r6
    75d2: 9a0b         	ldr	r2, [sp, #0x2c]
    75d4: ab0f         	add	r3, sp, #0x3c
    75d6: f002 feff    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #0x2dfe
    75da: 4605         	mov	r5, r0
    75dc: 982e         	ldr	r0, [sp, #0xb8]
    75de: 42a0         	cmp	r0, r4
    75e0: d003         	beq	0x75ea <ConnectionManager::disconnect(LSMessage&)+0xca> @ imm = #0x6
    75e2: 9930         	ldr	r1, [sp, #0xc0]
    75e4: 3101         	adds	r1, #0x1
    75e6: f7fc ec8c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x36e8
    75ea: 2d00         	cmp	r5, #0x0
    75ec: f000 8143    	beq.w	0x7876 <ConnectionManager::disconnect(LSMessage&)+0x356> @ imm = #0x286
    75f0: f8df 2504    	ldr.w	r2, [pc, #0x504]        @ 0x7af8 <ConnectionManager::disconnect(LSMessage&)+0x5d8> literal=0x6866
    75f4: 4659         	mov	r1, r11
    75f6: 4630         	mov	r0, r6
    75f8: af16         	add	r7, sp, #0x58
    75fa: f10d 0870    	add.w	r8, sp, #0x70
    75fe: 447a         	add	r2, pc => raw=0xde68 ghidra=0x1de68 file=56936 "sink"
    7600: f7fc ec84    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x36f8
    7604: ad28         	add	r5, sp, #0xa0
    7606: 2300         	movs	r3, #0x0
    7608: af2a         	add	r7, sp, #0xa8
    760a: 4629         	mov	r1, r5
    760c: 4630         	mov	r0, r6
    760e: 9329         	str	r3, [sp, #0xa4]
    7610: f88d 30a8    	strb.w	r3, [sp, #0xa8]
    7614: 9728         	str	r7, [sp, #0xa0]
    7616: f7fc ebc4    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x3878
    761a: 9b28         	ldr	r3, [sp, #0xa0]
    761c: 9816         	ldr	r0, [sp, #0x58]
    761e: 9a29         	ldr	r2, [sp, #0xa4]
    7620: 42bb         	cmp	r3, r7
    7622: f000 81e8    	beq.w	0x79f6 <ConnectionManager::disconnect(LSMessage&)+0x4d6> @ imm = #0x3d0
    7626: 9909         	ldr	r1, [sp, #0x24]
    7628: f8dd c0a8    	ldr.w	r12, [sp, #0xa8]
    762c: e9cd 3216    	strd	r3, r2, [sp, #88]
    7630: 4288         	cmp	r0, r1
    7632: bf08         	it	eq
    7634: f8cd c060    	streq.w	r12, [sp, #0x60]
    7638: f000 81ea    	beq.w	0x7a10 <ConnectionManager::disconnect(LSMessage&)+0x4f0> @ imm = #0x3d4
    763c: 9b18         	ldr	r3, [sp, #0x60]
    763e: f8cd c060    	str.w	r12, [sp, #0x60]
    7642: 2800         	cmp	r0, #0x0
    7644: f000 81e4    	beq.w	0x7a10 <ConnectionManager::disconnect(LSMessage&)+0x4f0> @ imm = #0x3c8
    7648: 932a         	str	r3, [sp, #0xa8]
    764a: 9028         	str	r0, [sp, #0xa0]
    764c: 2300         	movs	r3, #0x0
    764e: 9329         	str	r3, [sp, #0xa4]
    7650: 7003         	strb	r3, [r0]
    7652: 9828         	ldr	r0, [sp, #0xa0]
    7654: 42b8         	cmp	r0, r7
    7656: d003         	beq	0x7660 <ConnectionManager::disconnect(LSMessage&)+0x140> @ imm = #0x6
    7658: 992a         	ldr	r1, [sp, #0xa8]
    765a: 3101         	adds	r1, #0x1
    765c: f7fc ec50    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3760
    7660: 9a0d         	ldr	r2, [sp, #0x34]
    7662: 4630         	mov	r0, r6
    7664: f8df 3494    	ldr.w	r3, [pc, #0x494]        @ 0x7afc <ConnectionManager::disconnect(LSMessage&)+0x5dc> literal=0x210
    7668: af16         	add	r7, sp, #0x58
    766a: f10d 0870    	add.w	r8, sp, #0x70
    766e: f852 a003    	ldr.w	r10, [r2, r3]
    7672: f10a 0308    	add.w	r3, r10, #0x8
    7676: 932e         	str	r3, [sp, #0xb8]
    7678: f7fc ec68    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x3730
    767c: f8df 2480    	ldr.w	r2, [pc, #0x480]        @ 0x7b00 <ConnectionManager::disconnect(LSMessage&)+0x5e0> literal=0x646c
    7680: 4659         	mov	r1, r11
    7682: 4628         	mov	r0, r5
    7684: 447a         	add	r2, pc => raw=0xdaf4 ghidra=0x1daf4 file=56052 "source"
    7686: f7fc ec42    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x377c
    768a: 2300         	movs	r3, #0x0
    768c: 4631         	mov	r1, r6
    768e: 4628         	mov	r0, r5
    7690: 942e         	str	r4, [sp, #0xb8]
    7692: 932f         	str	r3, [sp, #0xbc]
    7694: f88d 30c0    	strb.w	r3, [sp, #0xc0]
    7698: f7fc eb82    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x38fc
    769c: 9b2e         	ldr	r3, [sp, #0xb8]
    769e: 981c         	ldr	r0, [sp, #0x70]
    76a0: 9a2f         	ldr	r2, [sp, #0xbc]
    76a2: 42a3         	cmp	r3, r4
    76a4: f000 819a    	beq.w	0x79dc <ConnectionManager::disconnect(LSMessage&)+0x4bc> @ imm = #0x334
    76a8: 9f0a         	ldr	r7, [sp, #0x28]
    76aa: 9930         	ldr	r1, [sp, #0xc0]
    76ac: e9cd 321c    	strd	r3, r2, [sp, #112]
    76b0: 42b8         	cmp	r0, r7
    76b2: bf08         	it	eq
    76b4: 911e         	streq	r1, [sp, #0x78]
    76b6: f000 81af    	beq.w	0x7a18 <ConnectionManager::disconnect(LSMessage&)+0x4f8> @ imm = #0x35e
    76ba: 9b1e         	ldr	r3, [sp, #0x78]
    76bc: 911e         	str	r1, [sp, #0x78]
    76be: 2800         	cmp	r0, #0x0
    76c0: f000 81aa    	beq.w	0x7a18 <ConnectionManager::disconnect(LSMessage&)+0x4f8> @ imm = #0x354
    76c4: 9330         	str	r3, [sp, #0xc0]
    76c6: 902e         	str	r0, [sp, #0xb8]
    76c8: 2300         	movs	r3, #0x0
    76ca: 932f         	str	r3, [sp, #0xbc]
    76cc: 7003         	strb	r3, [r0]
    76ce: 982e         	ldr	r0, [sp, #0xb8]
    76d0: 42a0         	cmp	r0, r4
    76d2: d003         	beq	0x76dc <ConnectionManager::disconnect(LSMessage&)+0x1bc> @ imm = #0x6
    76d4: 9930         	ldr	r1, [sp, #0xc0]
    76d6: 3101         	adds	r1, #0x1
    76d8: f7fc ec12    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x37dc
    76dc: f10a 0308    	add.w	r3, r10, #0x8
    76e0: 4628         	mov	r0, r5
    76e2: 9328         	str	r3, [sp, #0xa0]
    76e4: af16         	add	r7, sp, #0x58
    76e6: f10d 0870    	add.w	r8, sp, #0x70
    76ea: f7fc ec30    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x37a0
    76ee: f002 ff25    	bl	0xa53c <getPmLogContext()> @ imm = #0x2e4a
    76f2: b138         	cbz	r0, 0x7704 <ConnectionManager::disconnect(LSMessage&)+0x1e4> @ imm = #0xe
    76f4: af16         	add	r7, sp, #0x58
    76f6: f10d 0870    	add.w	r8, sp, #0x70
    76fa: f002 ff1f    	bl	0xa53c <getPmLogContext()> @ imm = #0x2e3e
    76fe: 6803         	ldr	r3, [r0]
    7700: 2b06         	cmp	r3, #0x6
    7702: dd19         	ble	0x7738 <ConnectionManager::disconnect(LSMessage&)+0x218> @ imm = #0x32
    7704: af16         	add	r7, sp, #0x58
    7706: f10d 0870    	add.w	r8, sp, #0x70
    770a: f002 ff17    	bl	0xa53c <getPmLogContext()> @ imm = #0x2e2e
    770e: 9b16         	ldr	r3, [sp, #0x58]
    7710: 2107         	movs	r1, #0x7
    7712: 9307         	str	r3, [sp, #0x1c]
    7714: 9b1c         	ldr	r3, [sp, #0x70]
    7716: 9306         	str	r3, [sp, #0x18]
    7718: 4bfa         	ldr	r3, [pc, #0x3e8]        @ 0x7b04 <ConnectionManager::disconnect(LSMessage&)+0x5e4> literal=0x68b2
    771a: 447b         	add	r3, pc => raw=0xdfd0 ghidra=0x1dfd0 file=57296 "disconnect"
    771c: 9305         	str	r3, [sp, #0x14]
    771e: 4bfa         	ldr	r3, [pc, #0x3e8]        @ 0x7b08 <ConnectionManager::disconnect(LSMessage&)+0x5e8> literal=0x602c
    7720: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    7722: 9304         	str	r3, [sp, #0x10]
    7724: 4bf9         	ldr	r3, [pc, #0x3e4]        @ 0x7b0c <ConnectionManager::disconnect(LSMessage&)+0x5ec> literal=0x68b2
    7726: 447b         	add	r3, pc => raw=0xdfdc ghidra=0x1dfdc file=57308 "%s:%s() Audio disconnect request for source %s, sink %s"
    7728: 9303         	str	r3, [sp, #0xc]
    772a: 2300         	movs	r3, #0x0
    772c: 461a         	mov	r2, r3
    772e: e9cd 3301    	strd	r3, r3, [sp, #4]
    7732: 9300         	str	r3, [sp]
    7734: f7fc ec72    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x371c
    7738: af16         	add	r7, sp, #0x58
    773a: f10d 0870    	add.w	r8, sp, #0x70
    773e: 4641         	mov	r1, r8
    7740: 463a         	mov	r2, r7
    7742: 4648         	mov	r0, r9
    7744: f7fe fca2    	bl	0x608c <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)> @ imm = #-0x16bc
    7748: 4601         	mov	r1, r0
    774a: 2800         	cmp	r0, #0x0
    774c: f000 8168    	beq.w	0x7a20 <ConnectionManager::disconnect(LSMessage&)+0x500> @ imm = #0x2d0
    7750: f100 0218    	add.w	r2, r0, #0x18
    7754: 4648         	mov	r0, r9
    7756: f7fe fe1d    	bl	0x6394 <ConnectionManager::disconnect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)> @ imm = #-0x13c6
    775a: 4682         	mov	r10, r0
    775c: 463a         	mov	r2, r7
    775e: 4648         	mov	r0, r9
    7760: 4641         	mov	r1, r8
    7762: f10d 094c    	add.w	r9, sp, #0x4c
    7766: f7fe fcbb    	bl	0x60e0 <ConnectionManager::removeAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)> @ imm = #-0x168a
    776a: 4648         	mov	r0, r9
    776c: f7fc eb24    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x39b8
    7770: f1ba 0f00    	cmp.w	r10, #0x0
    7774: f040 80bf    	bne.w	0x78f6 <ConnectionManager::disconnect(LSMessage&)+0x3d6> @ imm = #0x17e
    7778: f002 fee0    	bl	0xa53c <getPmLogContext()> @ imm = #0x2dc0
    777c: b120         	cbz	r0, 0x7788 <ConnectionManager::disconnect(LSMessage&)+0x268> @ imm = #0x8
    777e: f002 fedd    	bl	0xa53c <getPmLogContext()> @ imm = #0x2dba
    7782: 6803         	ldr	r3, [r0]
    7784: 2b06         	cmp	r3, #0x6
    7786: dd16         	ble	0x77b6 <ConnectionManager::disconnect(LSMessage&)+0x296> @ imm = #0x2c
    7788: f002 fed8    	bl	0xa53c <getPmLogContext()> @ imm = #0x2db0
    778c: 9b16         	ldr	r3, [sp, #0x58]
    778e: 2107         	movs	r1, #0x7
    7790: 9307         	str	r3, [sp, #0x1c]
    7792: 9b1c         	ldr	r3, [sp, #0x70]
    7794: 9306         	str	r3, [sp, #0x18]
    7796: 4bde         	ldr	r3, [pc, #0x378]        @ 0x7b10 <ConnectionManager::disconnect(LSMessage&)+0x5f0> literal=0x6834
    7798: 447b         	add	r3, pc => raw=0xdfd0 ghidra=0x1dfd0 file=57296 "disconnect"
    779a: 9305         	str	r3, [sp, #0x14]
    779c: 4bdd         	ldr	r3, [pc, #0x374]        @ 0x7b14 <ConnectionManager::disconnect(LSMessage&)+0x5f4> literal=0x5fae
    779e: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    77a0: 9304         	str	r3, [sp, #0x10]
    77a2: 4bdd         	ldr	r3, [pc, #0x374]        @ 0x7b18 <ConnectionManager::disconnect(LSMessage&)+0x5f8> literal=0x686c
    77a4: 447b         	add	r3, pc => raw=0xe014 ghidra=0x1e014 file=57364 "%s:%s() Audio disconnect with source %s and sink %s"
    77a6: 9303         	str	r3, [sp, #0xc]
    77a8: 2300         	movs	r3, #0x0
    77aa: 461a         	mov	r2, r3
    77ac: e9cd 3301    	strd	r3, r3, [sp, #4]
    77b0: 9300         	str	r3, [sp]
    77b2: f7fc ec34    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x3798
    77b6: 2101         	movs	r1, #0x1
    77b8: 4628         	mov	r0, r5
    77ba: f7fc eb1c    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x39c8
    77be: 4ad7         	ldr	r2, [pc, #0x35c]        @ 0x7b1c <ConnectionManager::disconnect(LSMessage&)+0x5fc> literal=0x5f54
    77c0: 4623         	mov	r3, r4
    77c2: 942e         	str	r4, [sp, #0xb8]
    77c4: f04f 0e0b    	mov.w	lr, #0xb
    77c8: f04f 0c00    	mov.w	r12, #0x0
    77cc: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    77ce: ca07         	ldm	r2, {r0, r1, r2}
    77d0: c303         	stm	r3!, {r0, r1}
    77d2: 4631         	mov	r1, r6
    77d4: f823 2b02    	strh	r2, [r3], #2
    77d8: 0c12         	lsrs	r2, r2, #0x10
    77da: 4648         	mov	r0, r9
    77dc: 701a         	strb	r2, [r3]
    77de: 462a         	mov	r2, r5
    77e0: f8cd e0bc    	str.w	lr, [sp, #0xbc]
    77e4: f88d c0cb    	strb.w	r12, [sp, #0xcb]
    77e8: f7fc ea74    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3b18
    77ec: 982e         	ldr	r0, [sp, #0xb8]
    77ee: 42a0         	cmp	r0, r4
    77f0: d003         	beq	0x77fa <ConnectionManager::disconnect(LSMessage&)+0x2da> @ imm = #0x6
    77f2: 9930         	ldr	r1, [sp, #0xc0]
    77f4: 3101         	adds	r1, #0x1
    77f6: f7fc eb84    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x38f8
    77fa: 4628         	mov	r0, r5
    77fc: f7fc eaac    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3aa8
    7800: 4641         	mov	r1, r8
    7802: 4628         	mov	r0, r5
    7804: f7fc ec92    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x36dc
    7808: 4bc5         	ldr	r3, [pc, #0x314]        @ 0x7b20 <ConnectionManager::disconnect(LSMessage&)+0x600> literal=0x62da
    780a: 2206         	movs	r2, #0x6
    780c: 922f         	str	r2, [sp, #0xbc]
    780e: 2200         	movs	r2, #0x0
    7810: f88d 20c6    	strb.w	r2, [sp, #0xc6]
    7814: 462a         	mov	r2, r5
    7816: 447b         	add	r3, pc => raw=0xdaf4 ghidra=0x1daf4 file=56052 "source"
    7818: 942e         	str	r4, [sp, #0xb8]
    781a: e893 0003    	ldm.w	r3, {r0, r1}
    781e: 9030         	str	r0, [sp, #0xc0]
    7820: 4648         	mov	r0, r9
    7822: f8ad 10c4    	strh.w	r1, [sp, #0xc4]
    7826: 4631         	mov	r1, r6
    7828: f7fc ea54    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3b58
    782c: 982e         	ldr	r0, [sp, #0xb8]
    782e: 42a0         	cmp	r0, r4
    7830: d003         	beq	0x783a <ConnectionManager::disconnect(LSMessage&)+0x31a> @ imm = #0x6
    7832: 9930         	ldr	r1, [sp, #0xc0]
    7834: 3101         	adds	r1, #0x1
    7836: f7fc eb64    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3938
    783a: 4628         	mov	r0, r5
    783c: f7fc ea8c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3ae8
    7840: 4639         	mov	r1, r7
    7842: 4628         	mov	r0, r5
    7844: f7fc ec72    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x371c
    7848: f646 1373    	movw	r3, #0x6973
    784c: 462a         	mov	r2, r5
    784e: f6c6 336e    	movt	r3, #0x6b6e
    7852: 4631         	mov	r1, r6
    7854: 9330         	str	r3, [sp, #0xc0]
    7856: 2304         	movs	r3, #0x4
    7858: 932f         	str	r3, [sp, #0xbc]
    785a: 4648         	mov	r0, r9
    785c: 2300         	movs	r3, #0x0
    785e: 942e         	str	r4, [sp, #0xb8]
    7860: f88d 30c4    	strb.w	r3, [sp, #0xc4]
    7864: f7fc ea36    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3b94
    7868: e0a6         	b	0x79b8 <ConnectionManager::disconnect(LSMessage&)+0x498> @ imm = #0x14c
    786a: 4623         	mov	r3, r4
    786c: 2800         	cmp	r0, #0x0
    786e: f43f aeab    	beq.w	0x75c8 <ConnectionManager::disconnect(LSMessage&)+0xa8> @ imm = #-0x2aa
    7872: 4620         	mov	r0, r4
    7874: e038         	b	0x78e8 <ConnectionManager::disconnect(LSMessage&)+0x3c8> @ imm = #0x70
    7876: 49ab         	ldr	r1, [pc, #0x2ac]        @ 0x7b24 <ConnectionManager::disconnect(LSMessage&)+0x604> literal=0x197bc
    7878: 462b         	mov	r3, r5
    787a: 2203         	movs	r2, #0x3
    787c: a80e         	add	r0, sp, #0x38
    787e: af16         	add	r7, sp, #0x58
    7880: f10d 0870    	add.w	r8, sp, #0x70
    7884: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    7886: 311c         	adds	r1, #0x1c
    7888: f002 f9e0    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0x23c0
    788c: 9822         	ldr	r0, [sp, #0x88]
    788e: 9b0c         	ldr	r3, [sp, #0x30]
    7890: 4298         	cmp	r0, r3
    7892: d003         	beq	0x789c <ConnectionManager::disconnect(LSMessage&)+0x37c> @ imm = #0x6
    7894: 9924         	ldr	r1, [sp, #0x90]
    7896: 3101         	adds	r1, #0x1
    7898: f7fc eb32    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x399c
    789c: 981c         	ldr	r0, [sp, #0x70]
    789e: 9b0a         	ldr	r3, [sp, #0x28]
    78a0: 4298         	cmp	r0, r3
    78a2: d003         	beq	0x78ac <ConnectionManager::disconnect(LSMessage&)+0x38c> @ imm = #0x6
    78a4: 991e         	ldr	r1, [sp, #0x78]
    78a6: 3101         	adds	r1, #0x1
    78a8: f7fc eb2a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x39ac
    78ac: 9816         	ldr	r0, [sp, #0x58]
    78ae: 9b09         	ldr	r3, [sp, #0x24]
    78b0: 4298         	cmp	r0, r3
    78b2: d003         	beq	0x78bc <ConnectionManager::disconnect(LSMessage&)+0x39c> @ imm = #0x6
    78b4: 9918         	ldr	r1, [sp, #0x60]
    78b6: 3101         	adds	r1, #0x1
    78b8: f7fc eb22    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x39bc
    78bc: 4658         	mov	r0, r11
    78be: f7fc ea4c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3b68
    78c2: 980e         	ldr	r0, [sp, #0x38]
    78c4: b108         	cbz	r0, 0x78ca <ConnectionManager::disconnect(LSMessage&)+0x3aa> @ imm = #0x2
    78c6: f7fc ebc2    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x387c
    78ca: 2001         	movs	r0, #0x1
    78cc: b035         	add	sp, #0xd4
    78ce: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    78d2: 2200         	movs	r2, #0x0
    78d4: a928         	add	r1, sp, #0xa0
    78d6: 4630         	mov	r0, r6
    78d8: af16         	add	r7, sp, #0x58
    78da: f10d 0870    	add.w	r8, sp, #0x70
    78de: f7fc ebca    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x386c
    78e2: 9b28         	ldr	r3, [sp, #0xa0]
    78e4: 902e         	str	r0, [sp, #0xb8]
    78e6: 9330         	str	r3, [sp, #0xc0]
    78e8: 462a         	mov	r2, r5
    78ea: 4651         	mov	r1, r10
    78ec: f7fc eb4c    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x3968
    78f0: 9d28         	ldr	r5, [sp, #0xa0]
    78f2: 9b2e         	ldr	r3, [sp, #0xb8]
    78f4: e668         	b	0x75c8 <ConnectionManager::disconnect(LSMessage&)+0xa8> @ imm = #-0x330
    78f6: 2100         	movs	r1, #0x0
    78f8: 4628         	mov	r0, r5
    78fa: f7fc ea7c    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x3b08
    78fe: 4a8a         	ldr	r2, [pc, #0x228]        @ 0x7b28 <ConnectionManager::disconnect(LSMessage&)+0x608> literal=0x5e14
    7900: 4623         	mov	r3, r4
    7902: 942e         	str	r4, [sp, #0xb8]
    7904: f04f 0e0b    	mov.w	lr, #0xb
    7908: f04f 0c00    	mov.w	r12, #0x0
    790c: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    790e: ca07         	ldm	r2, {r0, r1, r2}
    7910: c303         	stm	r3!, {r0, r1}
    7912: 4631         	mov	r1, r6
    7914: f823 2b02    	strh	r2, [r3], #2
    7918: 0c12         	lsrs	r2, r2, #0x10
    791a: 4648         	mov	r0, r9
    791c: 701a         	strb	r2, [r3]
    791e: 462a         	mov	r2, r5
    7920: f8cd e0bc    	str.w	lr, [sp, #0xbc]
    7924: f88d c0cb    	strb.w	r12, [sp, #0xcb]
    7928: f7fc e9d4    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3c58
    792c: 982e         	ldr	r0, [sp, #0xb8]
    792e: 42a0         	cmp	r0, r4
    7930: d003         	beq	0x793a <ConnectionManager::disconnect(LSMessage&)+0x41a> @ imm = #0x6
    7932: 9930         	ldr	r1, [sp, #0xc0]
    7934: 3101         	adds	r1, #0x1
    7936: f7fc eae4    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3a38
    793a: 4628         	mov	r0, r5
    793c: f7fc ea0c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3be8
    7940: 497a         	ldr	r1, [pc, #0x1e8]        @ 0x7b2c <ConnectionManager::disconnect(LSMessage&)+0x60c> literal=0x196fc
    7942: 4628         	mov	r0, r5
    7944: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    7946: 3164         	adds	r1, #0x64
    7948: f7fc ebf0    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x3820
    794c: 4a78         	ldr	r2, [pc, #0x1e0]        @ 0x7b30 <ConnectionManager::disconnect(LSMessage&)+0x610> literal=0x6cfe
    794e: 4623         	mov	r3, r4
    7950: 942e         	str	r4, [sp, #0xb8]
    7952: f04f 0e09    	mov.w	lr, #0x9
    7956: f04f 0c00    	mov.w	r12, #0x0
    795a: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    795c: ca07         	ldm	r2, {r0, r1, r2}
    795e: c303         	stm	r3!, {r0, r1}
    7960: 4631         	mov	r1, r6
    7962: 701a         	strb	r2, [r3]
    7964: 4648         	mov	r0, r9
    7966: 462a         	mov	r2, r5
    7968: f8cd e0bc    	str.w	lr, [sp, #0xbc]
    796c: f88d c0c9    	strb.w	r12, [sp, #0xc9]
    7970: f7fc e9b0    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3ca0
    7974: 982e         	ldr	r0, [sp, #0xb8]
    7976: 42a0         	cmp	r0, r4
    7978: d003         	beq	0x7982 <ConnectionManager::disconnect(LSMessage&)+0x462> @ imm = #0x6
    797a: 9930         	ldr	r1, [sp, #0xc0]
    797c: 3101         	adds	r1, #0x1
    797e: f7fc eac0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3a80
    7982: 4628         	mov	r0, r5
    7984: f7fc e9e8    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3c30
    7988: 2114         	movs	r1, #0x14
    798a: 4628         	mov	r0, r5
    798c: f7fc ebd4    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x3858
    7990: 4a68         	ldr	r2, [pc, #0x1a0]        @ 0x7b34 <ConnectionManager::disconnect(LSMessage&)+0x614> literal=0x6cc6
    7992: 4623         	mov	r3, r4
    7994: 942e         	str	r4, [sp, #0xb8]
    7996: f04f 0e09    	mov.w	lr, #0x9
    799a: f04f 0c00    	mov.w	r12, #0x0
    799e: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    79a0: ca07         	ldm	r2, {r0, r1, r2}
    79a2: c303         	stm	r3!, {r0, r1}
    79a4: 4631         	mov	r1, r6
    79a6: 701a         	strb	r2, [r3]
    79a8: 4648         	mov	r0, r9
    79aa: 462a         	mov	r2, r5
    79ac: f8cd e0bc    	str.w	lr, [sp, #0xbc]
    79b0: f88d c0c9    	strb.w	r12, [sp, #0xc9]
    79b4: f7fc e98e    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x3ce4
    79b8: 982e         	ldr	r0, [sp, #0xb8]
    79ba: 42a0         	cmp	r0, r4
    79bc: d003         	beq	0x79c6 <ConnectionManager::disconnect(LSMessage&)+0x4a6> @ imm = #0x6
    79be: 9930         	ldr	r1, [sp, #0xc0]
    79c0: 3101         	adds	r1, #0x1
    79c2: f7fc ea9e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3ac4
    79c6: 4628         	mov	r0, r5
    79c8: f7fc e9c6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3c74
    79cc: 4649         	mov	r1, r9
    79ce: a80e         	add	r0, sp, #0x38
    79d0: f002 fa5e    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #0x24bc
    79d4: 4648         	mov	r0, r9
    79d6: f7fc e9c0    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3c80
    79da: e757         	b	0x788c <ConnectionManager::disconnect(LSMessage&)+0x36c> @ imm = #-0x152
    79dc: b132         	cbz	r2, 0x79ec <ConnectionManager::disconnect(LSMessage&)+0x4cc> @ imm = #0xc
    79de: 2a01         	cmp	r2, #0x1
    79e0: d027         	beq	0x7a32 <ConnectionManager::disconnect(LSMessage&)+0x512> @ imm = #0x4e
    79e2: 4621         	mov	r1, r4
    79e4: f7fc ead0    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x3a60
    79e8: 9a2f         	ldr	r2, [sp, #0xbc]
    79ea: 981c         	ldr	r0, [sp, #0x70]
    79ec: 2300         	movs	r3, #0x0
    79ee: 921d         	str	r2, [sp, #0x74]
    79f0: 5483         	strb	r3, [r0, r2]
    79f2: 982e         	ldr	r0, [sp, #0xb8]
    79f4: e668         	b	0x76c8 <ConnectionManager::disconnect(LSMessage&)+0x1a8> @ imm = #-0x330
    79f6: b132         	cbz	r2, 0x7a06 <ConnectionManager::disconnect(LSMessage&)+0x4e6> @ imm = #0xc
    79f8: 2a01         	cmp	r2, #0x1
    79fa: d020         	beq	0x7a3e <ConnectionManager::disconnect(LSMessage&)+0x51e> @ imm = #0x40
    79fc: 4639         	mov	r1, r7
    79fe: f7fc eac4    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x3a78
    7a02: 9a29         	ldr	r2, [sp, #0xa4]
    7a04: 9816         	ldr	r0, [sp, #0x58]
    7a06: 2300         	movs	r3, #0x0
    7a08: 9217         	str	r2, [sp, #0x5c]
    7a0a: 5483         	strb	r3, [r0, r2]
    7a0c: 9828         	ldr	r0, [sp, #0xa0]
    7a0e: e61d         	b	0x764c <ConnectionManager::disconnect(LSMessage&)+0x12c> @ imm = #-0x3c6
    7a10: 9728         	str	r7, [sp, #0xa0]
    7a12: af2a         	add	r7, sp, #0xa8
    7a14: 4638         	mov	r0, r7
    7a16: e619         	b	0x764c <ConnectionManager::disconnect(LSMessage&)+0x12c> @ imm = #-0x3ce
    7a18: 942e         	str	r4, [sp, #0xb8]
    7a1a: ac30         	add	r4, sp, #0xc0
    7a1c: 4620         	mov	r0, r4
    7a1e: e653         	b	0x76c8 <ConnectionManager::disconnect(LSMessage&)+0x1a8> @ imm = #-0x35a
    7a20: 4945         	ldr	r1, [pc, #0x114]        @ 0x7b38 <ConnectionManager::disconnect(LSMessage&)+0x618> literal=0x19618
    7a22: 4603         	mov	r3, r0
    7a24: 22c8         	movs	r2, #0xc8
    7a26: a80e         	add	r0, sp, #0x38
    7a28: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    7a2a: 317c         	adds	r1, #0x7c
    7a2c: f002 f90e    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0x221c
    7a30: e72c         	b	0x788c <ConnectionManager::disconnect(LSMessage&)+0x36c> @ imm = #-0x1a8
    7a32: f89d 30c0    	ldrb.w	r3, [sp, #0xc0]
    7a36: 7003         	strb	r3, [r0]
    7a38: 9a2f         	ldr	r2, [sp, #0xbc]
    7a3a: 981c         	ldr	r0, [sp, #0x70]
    7a3c: e7d6         	b	0x79ec <ConnectionManager::disconnect(LSMessage&)+0x4cc> @ imm = #-0x54
    7a3e: f89d 30a8    	ldrb.w	r3, [sp, #0xa8]
    7a42: 7003         	strb	r3, [r0]
    7a44: 9a29         	ldr	r2, [sp, #0xa4]
    7a46: 9816         	ldr	r0, [sp, #0x58]
    7a48: e7dd         	b	0x7a06 <ConnectionManager::disconnect(LSMessage&)+0x4e6> @ imm = #-0x46
    7a4a: 483c         	ldr	r0, [pc, #0xf0]         @ 0x7b3c <ConnectionManager::disconnect(LSMessage&)+0x61c> literal=0x5c42
    7a4c: af16         	add	r7, sp, #0x58
    7a4e: f10d 0870    	add.w	r8, sp, #0x70
    7a52: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    7a54: f7fc e956    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x3d54
    7a58: e005         	b	0x7a66 <ConnectionManager::disconnect(LSMessage&)+0x546> @ imm = #0xa
    7a5a: 4630         	mov	r0, r6
    7a5c: f7fc eb48    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x3970
    7a60: 4628         	mov	r0, r5
    7a62: f7fc e97a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3d0c
    7a66: 4648         	mov	r0, r9
    7a68: f7fc e976    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3d14
    7a6c: 980b         	ldr	r0, [sp, #0x2c]
    7a6e: f7fc eb40    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x3980
    7a72: 4640         	mov	r0, r8
    7a74: f7fc eb3c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x3988
    7a78: 4638         	mov	r0, r7
    7a7a: f7fc eb3a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x398c
    7a7e: 4658         	mov	r0, r11
    7a80: f7fc e96a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3d2c
    7a84: 980e         	ldr	r0, [sp, #0x38]
    7a86: b108         	cbz	r0, 0x7a8c <ConnectionManager::disconnect(LSMessage&)+0x56c> @ imm = #0x2
    7a88: f7fc eae0    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x3a40
    7a8c: f7fc eb78    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x3910
    7a90: 980e         	ldr	r0, [sp, #0x38]
    7a92: e7f9         	b	0x7a88 <ConnectionManager::disconnect(LSMessage&)+0x568> @ imm = #-0xe
    7a94: e7e1         	b	0x7a5a <ConnectionManager::disconnect(LSMessage&)+0x53a> @ imm = #-0x3e
    7a96: e7e0         	b	0x7a5a <ConnectionManager::disconnect(LSMessage&)+0x53a> @ imm = #-0x40
    7a98: e7df         	b	0x7a5a <ConnectionManager::disconnect(LSMessage&)+0x53a> @ imm = #-0x42
    7a9a: e7de         	b	0x7a5a <ConnectionManager::disconnect(LSMessage&)+0x53a> @ imm = #-0x44
    7a9c: e7dd         	b	0x7a5a <ConnectionManager::disconnect(LSMessage&)+0x53a> @ imm = #-0x46
    7a9e: e7e5         	b	0x7a6c <ConnectionManager::disconnect(LSMessage&)+0x54c> @ imm = #-0x36
    7aa0: af16         	add	r7, sp, #0x58
    7aa2: f10d 0870    	add.w	r8, sp, #0x70
    7aa6: e7e4         	b	0x7a72 <ConnectionManager::disconnect(LSMessage&)+0x552> @ imm = #-0x38
    7aa8: 4630         	mov	r0, r6
    7aaa: af16         	add	r7, sp, #0x58
    7aac: f10d 0870    	add.w	r8, sp, #0x70
    7ab0: f7fc eb1e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x39c4
    7ab4: e7da         	b	0x7a6c <ConnectionManager::disconnect(LSMessage&)+0x54c> @ imm = #-0x4c
    7ab6: 4628         	mov	r0, r5
    7ab8: af16         	add	r7, sp, #0x58
    7aba: f7fc eb1a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x39cc
    7abe: 9a0d         	ldr	r2, [sp, #0x34]
    7ac0: 4630         	mov	r0, r6
    7ac2: 4b0e         	ldr	r3, [pc, #0x38]         @ 0x7afc <ConnectionManager::disconnect(LSMessage&)+0x5dc> literal=0x210
    7ac4: f10d 0870    	add.w	r8, sp, #0x70
    7ac8: 58d3         	ldr	r3, [r2, r3]
    7aca: 3308         	adds	r3, #0x8
    7acc: 932e         	str	r3, [sp, #0xb8]
    7ace: f7fc ea3e    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x3b84
    7ad2: e7cb         	b	0x7a6c <ConnectionManager::disconnect(LSMessage&)+0x54c> @ imm = #-0x6a
    7ad4: 4630         	mov	r0, r6
    7ad6: af16         	add	r7, sp, #0x58
    7ad8: f7fc eb0a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x39ec
    7adc: f10a 0308    	add.w	r3, r10, #0x8
    7ae0: 4628         	mov	r0, r5
    7ae2: 9328         	str	r3, [sp, #0xa0]
    7ae4: f10d 0870    	add.w	r8, sp, #0x70
    7ae8: f7fc ea30    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x3ba0
    7aec: e7be         	b	0x7a6c <ConnectionManager::disconnect(LSMessage&)+0x54c> @ imm = #-0x84
    7aee: bf00         	nop
    7af0: 9880         	ldr	r0, [sp, #0x200]
    7af2: 0001         	movs	r1, r0
    7af4: 69b6         	ldr	r6, [r6, #0x18]
    7af6: 0000         	movs	r0, r0
    7af8: 6866         	ldr	r6, [r4, #0x4]
    7afa: 0000         	movs	r0, r0
    7afc: 0210         	lsls	r0, r2, #0x8
    7afe: 0000         	movs	r0, r0
    7b00: 646c         	str	r4, [r5, #0x44]
    7b02: 0000         	movs	r0, r0
    7b04: 68b2         	ldr	r2, [r6, #0x8]
    7b06: 0000         	movs	r0, r0
    7b08: 602c         	str	r4, [r5]
    7b0a: 0000         	movs	r0, r0
    7b0c: 68b2         	ldr	r2, [r6, #0x8]
    7b0e: 0000         	movs	r0, r0
    7b10: 6834         	ldr	r4, [r6]
    7b12: 0000         	movs	r0, r0
    7b14: 5fae         	ldrsh	r6, [r5, r6]
    7b16: 0000         	movs	r0, r0
    7b18: 686c         	ldr	r4, [r5, #0x4]
    7b1a: 0000         	movs	r0, r0
    7b1c: 5f54         	ldrsh	r4, [r2, r5]
    7b1e: 0000         	movs	r0, r0
    7b20: 62da         	str	r2, [r3, #0x2c]
    7b22: 0000         	movs	r0, r0
    7b24: 97bc         	str	r7, [sp, #0x2f0]
    7b26: 0001         	movs	r1, r0
    7b28: 5e14         	ldrsh	r4, [r2, r0]
    7b2a: 0000         	movs	r0, r0
    7b2c: 96fc         	str	r6, [sp, #0x3f0]
    7b2e: 0001         	movs	r1, r0
    7b30: 6cfe         	ldr	r6, [r7, #0x4c]
    7b32: 0000         	movs	r0, r0
    7b34: 6cc6         	ldr	r6, [r0, #0x4c]
    7b36: 0000         	movs	r0, r0
    7b38: 9618         	str	r6, [sp, #0x60]
    7b3a: 0001         	movs	r1, r0
    7b3c: 5c42         	ldrb	r2, [r0, r1]
    7b3e: 0000         	movs	r0, r0

00007b40 <ConnectionManager::setSoundOut(LSMessage&)>:
    7b40: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    7b44: b0ab         	sub	sp, #0xac
    7b46: f8df 8458    	ldr.w	r8, [pc, #0x458]        @ 0x7fa0 <ConnectionManager::setSoundOut(LSMessage&)+0x460> literal=0x19262
    7b4a: 4681         	mov	r9, r0
    7b4c: f10d 0b3c    	add.w	r11, sp, #0x3c
    7b50: 4608         	mov	r0, r1
    7b52: 460c         	mov	r4, r1
    7b54: f8cd 9028    	str.w	r9, [sp, #0x28]
    7b58: 910d         	str	r1, [sp, #0x34]
    7b5a: 44f8         	add	r8, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    7b5c: f7fc eac2    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x3a7c
    7b60: 4658         	mov	r0, r11
    7b62: f7fc eb5c    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x3948
    7b66: ab1a         	add	r3, sp, #0x68
    7b68: f10d 0978    	add.w	r9, sp, #0x78
    7b6c: af24         	add	r7, sp, #0x90
    7b6e: 9309         	str	r3, [sp, #0x24]
    7b70: 9318         	str	r3, [sp, #0x60]
    7b72: ab20         	add	r3, sp, #0x80
    7b74: 2200         	movs	r2, #0x0
    7b76: 930b         	str	r3, [sp, #0x2c]
    7b78: 931e         	str	r3, [sp, #0x78]
    7b7a: 4639         	mov	r1, r7
    7b7c: 2372         	movs	r3, #0x72
    7b7e: 4648         	mov	r0, r9
    7b80: 920e         	str	r2, [sp, #0x38]
    7b82: 9219         	str	r2, [sp, #0x64]
    7b84: f88d 2068    	strb.w	r2, [sp, #0x68]
    7b88: 9324         	str	r3, [sp, #0x90]
    7b8a: f7fc ea74    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x3b18
    7b8e: f8df 1414    	ldr.w	r1, [pc, #0x414]        @ 0x7fa4 <ConnectionManager::setSoundOut(LSMessage&)+0x464> literal=0x64ac
    7b92: 2272         	movs	r2, #0x72
    7b94: 9b24         	ldr	r3, [sp, #0x90]
    7b96: 901e         	str	r0, [sp, #0x78]
    7b98: 4479         	add	r1, pc => raw=0xe048 ghidra=0x1e048 file=57416 "{\"type\":\"object\",\"properties\":{\"soundOut\":{\"type\":\"string\"}},\"required\":[\"soundOut\"],\"additionalProperties\":false}"
    7b9a: 9320         	str	r3, [sp, #0x80]
    7b9c: f7fc e9f4    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x3c18
    7ba0: 9b24         	ldr	r3, [sp, #0x90]
    7ba2: 2100         	movs	r1, #0x0
    7ba4: 9a1e         	ldr	r2, [sp, #0x78]
    7ba6: 4620         	mov	r0, r4
    7ba8: 931f         	str	r3, [sp, #0x7c]
    7baa: 54d1         	strb	r1, [r2, r3]
    7bac: f7fc eaf4    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x3a18
    7bb0: ac26         	add	r4, sp, #0x98
    7bb2: 4606         	mov	r6, r0
    7bb4: 9424         	str	r4, [sp, #0x90]
    7bb6: 2800         	cmp	r0, #0x0
    7bb8: f000 81b5    	beq.w	0x7f26 <ConnectionManager::setSoundOut(LSMessage&)+0x3e6> @ imm = #0x36a
    7bbc: f7fc ea72    	blx	0x40a4 <strlen@plt>     @ imm = #-0x3b1c
    7bc0: 280f         	cmp	r0, #0xf
    7bc2: 4605         	mov	r5, r0
    7bc4: 9015         	str	r0, [sp, #0x54]
    7bc6: f200 8188    	bhi.w	0x7eda <ConnectionManager::setSoundOut(LSMessage&)+0x39a> @ imm = #0x310
    7bca: 2801         	cmp	r0, #0x1
    7bcc: f040 8101    	bne.w	0x7dd2 <ConnectionManager::setSoundOut(LSMessage&)+0x292> @ imm = #0x202
    7bd0: 7833         	ldrb	r3, [r6]
    7bd2: 4622         	mov	r2, r4
    7bd4: f88d 3098    	strb.w	r3, [sp, #0x98]
    7bd8: 2300         	movs	r3, #0x0
    7bda: 9525         	str	r5, [sp, #0x94]
    7bdc: 4659         	mov	r1, r11
    7bde: 5553         	strb	r3, [r2, r5]
    7be0: 4638         	mov	r0, r7
    7be2: ab0e         	add	r3, sp, #0x38
    7be4: 464a         	mov	r2, r9
    7be6: f002 fbf7    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #0x27ee
    7bea: 4605         	mov	r5, r0
    7bec: 9824         	ldr	r0, [sp, #0x90]
    7bee: 42a0         	cmp	r0, r4
    7bf0: d003         	beq	0x7bfa <ConnectionManager::setSoundOut(LSMessage&)+0xba> @ imm = #0x6
    7bf2: 9926         	ldr	r1, [sp, #0x98]
    7bf4: 3101         	adds	r1, #0x1
    7bf6: f7fc e984    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3cf8
    7bfa: 2d00         	cmp	r5, #0x0
    7bfc: f000 8105    	beq.w	0x7e0a <ConnectionManager::setSoundOut(LSMessage&)+0x2ca> @ imm = #0x20a
    7c00: 4ae9         	ldr	r2, [pc, #0x3a4]        @ 0x7fa8 <ConnectionManager::setSoundOut(LSMessage&)+0x468> literal=0x64b0
    7c02: ae15         	add	r6, sp, #0x54
    7c04: 4659         	mov	r1, r11
    7c06: 4630         	mov	r0, r6
    7c08: 447a         	add	r2, pc => raw=0xe0bc ghidra=0x1e0bc file=57532 "soundOut"
    7c0a: f7fc e980    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x3d00
    7c0e: 2300         	movs	r3, #0x0
    7c10: 4639         	mov	r1, r7
    7c12: 4630         	mov	r0, r6
    7c14: 9424         	str	r4, [sp, #0x90]
    7c16: 9325         	str	r3, [sp, #0x94]
    7c18: f88d 3098    	strb.w	r3, [sp, #0x98]
    7c1c: f7fc e8c0    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x3e80
    7c20: 9b24         	ldr	r3, [sp, #0x90]
    7c22: 9818         	ldr	r0, [sp, #0x60]
    7c24: 9a25         	ldr	r2, [sp, #0x94]
    7c26: 42a3         	cmp	r3, r4
    7c28: f000 8166    	beq.w	0x7ef8 <ConnectionManager::setSoundOut(LSMessage&)+0x3b8> @ imm = #0x2cc
    7c2c: 9d09         	ldr	r5, [sp, #0x24]
    7c2e: 9926         	ldr	r1, [sp, #0x98]
    7c30: e9cd 3218    	strd	r3, r2, [sp, #96]
    7c34: 42a8         	cmp	r0, r5
    7c36: bf08         	it	eq
    7c38: 911a         	streq	r1, [sp, #0x68]
    7c3a: f000 816a    	beq.w	0x7f12 <ConnectionManager::setSoundOut(LSMessage&)+0x3d2> @ imm = #0x2d4
    7c3e: 9b1a         	ldr	r3, [sp, #0x68]
    7c40: 911a         	str	r1, [sp, #0x68]
    7c42: 2800         	cmp	r0, #0x0
    7c44: f000 8165    	beq.w	0x7f12 <ConnectionManager::setSoundOut(LSMessage&)+0x3d2> @ imm = #0x2ca
    7c48: 9326         	str	r3, [sp, #0x98]
    7c4a: 9024         	str	r0, [sp, #0x90]
    7c4c: 2300         	movs	r3, #0x0
    7c4e: 9325         	str	r3, [sp, #0x94]
    7c50: 7003         	strb	r3, [r0]
    7c52: 9824         	ldr	r0, [sp, #0x90]
    7c54: 42a0         	cmp	r0, r4
    7c56: d003         	beq	0x7c60 <ConnectionManager::setSoundOut(LSMessage&)+0x120> @ imm = #0x6
    7c58: 9926         	ldr	r1, [sp, #0x98]
    7c5a: 3101         	adds	r1, #0x1
    7c5c: f7fc e950    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3d60
    7c60: 4bd2         	ldr	r3, [pc, #0x348]        @ 0x7fac <ConnectionManager::setSoundOut(LSMessage&)+0x46c> literal=0x210
    7c62: 4630         	mov	r0, r6
    7c64: f858 3003    	ldr.w	r3, [r8, r3]
    7c68: 3308         	adds	r3, #0x8
    7c6a: 9315         	str	r3, [sp, #0x54]
    7c6c: f7fc e96e    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x3d24
    7c70: f002 fc64    	bl	0xa53c <getPmLogContext()> @ imm = #0x28c8
    7c74: b120         	cbz	r0, 0x7c80 <ConnectionManager::setSoundOut(LSMessage&)+0x140> @ imm = #0x8
    7c76: f002 fc61    	bl	0xa53c <getPmLogContext()> @ imm = #0x28c2
    7c7a: 6803         	ldr	r3, [r0]
    7c7c: 2b06         	cmp	r3, #0x6
    7c7e: dd14         	ble	0x7caa <ConnectionManager::setSoundOut(LSMessage&)+0x16a> @ imm = #0x28
    7c80: f002 fc5c    	bl	0xa53c <getPmLogContext()> @ imm = #0x28b8
    7c84: 9b18         	ldr	r3, [sp, #0x60]
    7c86: 2107         	movs	r1, #0x7
    7c88: 9306         	str	r3, [sp, #0x18]
    7c8a: 4bc9         	ldr	r3, [pc, #0x324]        @ 0x7fb0 <ConnectionManager::setSoundOut(LSMessage&)+0x470> literal=0x6438
    7c8c: 447b         	add	r3, pc => raw=0xe0c8 ghidra=0x1e0c8 file=57544 "setSoundOut"
    7c8e: 9305         	str	r3, [sp, #0x14]
    7c90: 4bc8         	ldr	r3, [pc, #0x320]        @ 0x7fb4 <ConnectionManager::setSoundOut(LSMessage&)+0x474> literal=0x5aba
    7c92: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    7c94: 9304         	str	r3, [sp, #0x10]
    7c96: 4bc8         	ldr	r3, [pc, #0x320]        @ 0x7fb8 <ConnectionManager::setSoundOut(LSMessage&)+0x478> literal=0x6438
    7c98: 447b         	add	r3, pc => raw=0xe0d4 ghidra=0x1e0d4 file=57556 "%s:%s() Audio setSoundOut request for soundOut %s"
    7c9a: 9303         	str	r3, [sp, #0xc]
    7c9c: 2300         	movs	r3, #0x0
    7c9e: 461a         	mov	r2, r3
    7ca0: e9cd 3301    	strd	r3, r3, [sp, #4]
    7ca4: 9300         	str	r3, [sp]
    7ca6: f7fc e9ba    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x3c8c
    7caa: f10d 0a48    	add.w	r10, sp, #0x48
    7cae: 4650         	mov	r0, r10
    7cb0: f7fc e882    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x3efc
    7cb4: f10d 0860    	add.w	r8, sp, #0x60
    7cb8: 980a         	ldr	r0, [sp, #0x28]
    7cba: 4641         	mov	r1, r8
    7cbc: f7fd fdbc    	bl	0x5838 <ConnectionManager::setSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)> @ imm = #-0x2488
    7cc0: 2800         	cmp	r0, #0x0
    7cc2: f040 80ab    	bne.w	0x7e1c <ConnectionManager::setSoundOut(LSMessage&)+0x2dc> @ imm = #0x156
    7cc6: f002 fc39    	bl	0xa53c <getPmLogContext()> @ imm = #0x2872
    7cca: 2800         	cmp	r0, #0x0
    7ccc: f000 8087    	beq.w	0x7dde <ConnectionManager::setSoundOut(LSMessage&)+0x29e> @ imm = #0x10e
    7cd0: f002 fc34    	bl	0xa53c <getPmLogContext()> @ imm = #0x2868
    7cd4: 6803         	ldr	r3, [r0]
    7cd6: 2b06         	cmp	r3, #0x6
    7cd8: f300 8081    	bgt.w	0x7dde <ConnectionManager::setSoundOut(LSMessage&)+0x29e> @ imm = #0x102
    7cdc: 9b0a         	ldr	r3, [sp, #0x28]
    7cde: e9d3 530c    	ldrd	r5, r3, [r3, #48]
    7ce2: 429d         	cmp	r5, r3
    7ce4: 9308         	str	r3, [sp, #0x20]
    7ce6: d008         	beq	0x7cfa <ConnectionManager::setSoundOut(LSMessage&)+0x1ba> @ imm = #0x10
    7ce8: 4641         	mov	r1, r8
    7cea: f105 0030    	add.w	r0, r5, #0x30
    7cee: f7fc ea90    	blx	0x4210 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt> @ imm = #-0x3ae0
    7cf2: 9b08         	ldr	r3, [sp, #0x20]
    7cf4: 354c         	adds	r5, #0x4c
    7cf6: 42ab         	cmp	r3, r5
    7cf8: d1f6         	bne	0x7ce8 <ConnectionManager::setSoundOut(LSMessage&)+0x1a8> @ imm = #-0x14
    7cfa: 9b0a         	ldr	r3, [sp, #0x28]
    7cfc: 4641         	mov	r1, r8
    7cfe: 6ad8         	ldr	r0, [r3, #0x2c]
    7d00: f003 ffaa    	bl	0xbc58 <VolumeManager::updateSoundOutput(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)> @ imm = #0x3f54
    7d04: 2101         	movs	r1, #0x1
    7d06: 4630         	mov	r0, r6
    7d08: f7fc e874    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x3f18
    7d0c: 4aab         	ldr	r2, [pc, #0x2ac]        @ 0x7fbc <ConnectionManager::setSoundOut(LSMessage&)+0x47c> literal=0x5a08
    7d0e: 4623         	mov	r3, r4
    7d10: 9424         	str	r4, [sp, #0x90]
    7d12: f04f 0c0b    	mov.w	r12, #0xb
    7d16: 2500         	movs	r5, #0x0
    7d18: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    7d1a: ca07         	ldm	r2, {r0, r1, r2}
    7d1c: c303         	stm	r3!, {r0, r1}
    7d1e: 4639         	mov	r1, r7
    7d20: f823 2b02    	strh	r2, [r3], #2
    7d24: 0c12         	lsrs	r2, r2, #0x10
    7d26: 4650         	mov	r0, r10
    7d28: 701a         	strb	r2, [r3]
    7d2a: 4632         	mov	r2, r6
    7d2c: f8cd c094    	str.w	r12, [sp, #0x94]
    7d30: f88d 50a3    	strb.w	r5, [sp, #0xa3]
    7d34: f7fb efce    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4064
    7d38: 9824         	ldr	r0, [sp, #0x90]
    7d3a: 42a0         	cmp	r0, r4
    7d3c: d003         	beq	0x7d46 <ConnectionManager::setSoundOut(LSMessage&)+0x206> @ imm = #0x6
    7d3e: 9926         	ldr	r1, [sp, #0x98]
    7d40: 3101         	adds	r1, #0x1
    7d42: f7fc e8de    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3e44
    7d46: 4630         	mov	r0, r6
    7d48: f7fc e806    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x3ff4
    7d4c: 4641         	mov	r1, r8
    7d4e: 4630         	mov	r0, r6
    7d50: f7fc e9ec    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x3c28
    7d54: 4b9a         	ldr	r3, [pc, #0x268]        @ 0x7fc0 <ConnectionManager::setSoundOut(LSMessage&)+0x480> literal=0x6358
    7d56: f04f 0c08    	mov.w	r12, #0x8
    7d5a: 2500         	movs	r5, #0x0
    7d5c: 9424         	str	r4, [sp, #0x90]
    7d5e: 4632         	mov	r2, r6
    7d60: 447b         	add	r3, pc => raw=0xe0bc ghidra=0x1e0bc file=57532 "soundOut"
    7d62: e893 0003    	ldm.w	r3, {r0, r1}
    7d66: e884 0003    	stm.w	r4, {r0, r1}
    7d6a: 4639         	mov	r1, r7
    7d6c: 4650         	mov	r0, r10
    7d6e: f8cd c094    	str.w	r12, [sp, #0x94]
    7d72: f88d 50a0    	strb.w	r5, [sp, #0xa0]
    7d76: f7fb efae    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x40a4
    7d7a: 9824         	ldr	r0, [sp, #0x90]
    7d7c: 42a0         	cmp	r0, r4
    7d7e: d003         	beq	0x7d88 <ConnectionManager::setSoundOut(LSMessage&)+0x248> @ imm = #0x6
    7d80: 9926         	ldr	r1, [sp, #0x98]
    7d82: 3101         	adds	r1, #0x1
    7d84: f7fc e8bc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3e88
    7d88: 4630         	mov	r0, r6
    7d8a: f7fb efe6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4034
    7d8e: 4651         	mov	r1, r10
    7d90: a80d         	add	r0, sp, #0x34
    7d92: f002 f87d    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #0x20fa
    7d96: 4650         	mov	r0, r10
    7d98: f7fb efde    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4044
    7d9c: 981e         	ldr	r0, [sp, #0x78]
    7d9e: 9b0b         	ldr	r3, [sp, #0x2c]
    7da0: 4298         	cmp	r0, r3
    7da2: d003         	beq	0x7dac <ConnectionManager::setSoundOut(LSMessage&)+0x26c> @ imm = #0x6
    7da4: 9920         	ldr	r1, [sp, #0x80]
    7da6: 3101         	adds	r1, #0x1
    7da8: f7fc e8aa    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3eac
    7dac: 9818         	ldr	r0, [sp, #0x60]
    7dae: 9b09         	ldr	r3, [sp, #0x24]
    7db0: 4298         	cmp	r0, r3
    7db2: d003         	beq	0x7dbc <ConnectionManager::setSoundOut(LSMessage&)+0x27c> @ imm = #0x6
    7db4: 991a         	ldr	r1, [sp, #0x68]
    7db6: 3101         	adds	r1, #0x1
    7db8: f7fc e8a2    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3ebc
    7dbc: 4658         	mov	r0, r11
    7dbe: f7fb efcc    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4068
    7dc2: 980d         	ldr	r0, [sp, #0x34]
    7dc4: b108         	cbz	r0, 0x7dca <ConnectionManager::setSoundOut(LSMessage&)+0x28a> @ imm = #0x2
    7dc6: f7fc e942    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x3d7c
    7dca: 2001         	movs	r0, #0x1
    7dcc: b02b         	add	sp, #0xac
    7dce: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    7dd2: 4622         	mov	r2, r4
    7dd4: 2800         	cmp	r0, #0x0
    7dd6: f43f aeff    	beq.w	0x7bd8 <ConnectionManager::setSoundOut(LSMessage&)+0x98> @ imm = #-0x202
    7dda: 4620         	mov	r0, r4
    7ddc: e085         	b	0x7eea <ConnectionManager::setSoundOut(LSMessage&)+0x3aa> @ imm = #0x10a
    7dde: f002 fbad    	bl	0xa53c <getPmLogContext()> @ imm = #0x275a
    7de2: 9b18         	ldr	r3, [sp, #0x60]
    7de4: 2107         	movs	r1, #0x7
    7de6: 9306         	str	r3, [sp, #0x18]
    7de8: 4b76         	ldr	r3, [pc, #0x1d8]        @ 0x7fc4 <ConnectionManager::setSoundOut(LSMessage&)+0x484> literal=0x62da
    7dea: 447b         	add	r3, pc => raw=0xe0c8 ghidra=0x1e0c8 file=57544 "setSoundOut"
    7dec: 9305         	str	r3, [sp, #0x14]
    7dee: 4b76         	ldr	r3, [pc, #0x1d8]        @ 0x7fc8 <ConnectionManager::setSoundOut(LSMessage&)+0x488> literal=0x595c
    7df0: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    7df2: 9304         	str	r3, [sp, #0x10]
    7df4: 4b75         	ldr	r3, [pc, #0x1d4]        @ 0x7fcc <ConnectionManager::setSoundOut(LSMessage&)+0x48c> literal=0x630e
    7df6: 447b         	add	r3, pc => raw=0xe108 ghidra=0x1e108 file=57608 "%s:%s() Audio routing to soundOut %s  is success"
    7df8: 9303         	str	r3, [sp, #0xc]
    7dfa: 2300         	movs	r3, #0x0
    7dfc: 461a         	mov	r2, r3
    7dfe: e9cd 3301    	strd	r3, r3, [sp, #4]
    7e02: 9300         	str	r3, [sp]
    7e04: f7fc e90a    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x3dec
    7e08: e768         	b	0x7cdc <ConnectionManager::setSoundOut(LSMessage&)+0x19c> @ imm = #-0x130
    7e0a: 4971         	ldr	r1, [pc, #0x1c4]        @ 0x7fd0 <ConnectionManager::setSoundOut(LSMessage&)+0x490> literal=0x1922e
    7e0c: 462b         	mov	r3, r5
    7e0e: 2203         	movs	r2, #0x3
    7e10: a80d         	add	r0, sp, #0x34
    7e12: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    7e14: 311c         	adds	r1, #0x1c
    7e16: f001 ff19    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0x1e32
    7e1a: e7bf         	b	0x7d9c <ConnectionManager::setSoundOut(LSMessage&)+0x25c> @ imm = #-0x82
    7e1c: 2100         	movs	r1, #0x0
    7e1e: 4630         	mov	r0, r6
    7e20: f7fb efe8    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x4030
    7e24: 4a6b         	ldr	r2, [pc, #0x1ac]        @ 0x7fd4 <ConnectionManager::setSoundOut(LSMessage&)+0x494> literal=0x58f0
    7e26: 4623         	mov	r3, r4
    7e28: 9424         	str	r4, [sp, #0x90]
    7e2a: f04f 0c0b    	mov.w	r12, #0xb
    7e2e: 2500         	movs	r5, #0x0
    7e30: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    7e32: ca07         	ldm	r2, {r0, r1, r2}
    7e34: c303         	stm	r3!, {r0, r1}
    7e36: 4639         	mov	r1, r7
    7e38: f823 2b02    	strh	r2, [r3], #2
    7e3c: 0c12         	lsrs	r2, r2, #0x10
    7e3e: 4650         	mov	r0, r10
    7e40: 701a         	strb	r2, [r3]
    7e42: 4632         	mov	r2, r6
    7e44: f8cd c094    	str.w	r12, [sp, #0x94]
    7e48: f88d 50a3    	strb.w	r5, [sp, #0xa3]
    7e4c: f7fb ef42    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x417c
    7e50: 9824         	ldr	r0, [sp, #0x90]
    7e52: 42a0         	cmp	r0, r4
    7e54: d003         	beq	0x7e5e <ConnectionManager::setSoundOut(LSMessage&)+0x31e> @ imm = #0x6
    7e56: 9926         	ldr	r1, [sp, #0x98]
    7e58: 3101         	adds	r1, #0x1
    7e5a: f7fc e852    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3f5c
    7e5e: 4630         	mov	r0, r6
    7e60: f7fb ef7a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x410c
    7e64: 495c         	ldr	r1, [pc, #0x170]        @ 0x7fd8 <ConnectionManager::setSoundOut(LSMessage&)+0x498> literal=0x191d8
    7e66: 4630         	mov	r0, r6
    7e68: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    7e6a: 3164         	adds	r1, #0x64
    7e6c: f7fc e95e    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x3d44
    7e70: 4a5a         	ldr	r2, [pc, #0x168]        @ 0x7fdc <ConnectionManager::setSoundOut(LSMessage&)+0x49c> literal=0x67dc
    7e72: 4623         	mov	r3, r4
    7e74: 9424         	str	r4, [sp, #0x90]
    7e76: f04f 0c09    	mov.w	r12, #0x9
    7e7a: 2500         	movs	r5, #0x0
    7e7c: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    7e7e: ca07         	ldm	r2, {r0, r1, r2}
    7e80: c303         	stm	r3!, {r0, r1}
    7e82: 4639         	mov	r1, r7
    7e84: 701a         	strb	r2, [r3]
    7e86: 4650         	mov	r0, r10
    7e88: 4632         	mov	r2, r6
    7e8a: f8cd c094    	str.w	r12, [sp, #0x94]
    7e8e: f88d 50a1    	strb.w	r5, [sp, #0xa1]
    7e92: f7fb ef20    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x41c0
    7e96: 9824         	ldr	r0, [sp, #0x90]
    7e98: 42a0         	cmp	r0, r4
    7e9a: d003         	beq	0x7ea4 <ConnectionManager::setSoundOut(LSMessage&)+0x364> @ imm = #0x6
    7e9c: 9926         	ldr	r1, [sp, #0x98]
    7e9e: 3101         	adds	r1, #0x1
    7ea0: f7fc e82e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x3fa4
    7ea4: 4630         	mov	r0, r6
    7ea6: f7fb ef58    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4150
    7eaa: 2114         	movs	r1, #0x14
    7eac: 4630         	mov	r0, r6
    7eae: f7fc e944    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x3d78
    7eb2: 4a4b         	ldr	r2, [pc, #0x12c]        @ 0x7fe0 <ConnectionManager::setSoundOut(LSMessage&)+0x4a0> literal=0x67a6
    7eb4: 4623         	mov	r3, r4
    7eb6: 9424         	str	r4, [sp, #0x90]
    7eb8: f04f 0c09    	mov.w	r12, #0x9
    7ebc: 2500         	movs	r5, #0x0
    7ebe: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    7ec0: ca07         	ldm	r2, {r0, r1, r2}
    7ec2: c303         	stm	r3!, {r0, r1}
    7ec4: 4639         	mov	r1, r7
    7ec6: 701a         	strb	r2, [r3]
    7ec8: 4650         	mov	r0, r10
    7eca: 4632         	mov	r2, r6
    7ecc: f8cd c094    	str.w	r12, [sp, #0x94]
    7ed0: f88d 50a1    	strb.w	r5, [sp, #0xa1]
    7ed4: f7fb eefe    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4204
    7ed8: e74f         	b	0x7d7a <ConnectionManager::setSoundOut(LSMessage&)+0x23a> @ imm = #-0x162
    7eda: 2200         	movs	r2, #0x0
    7edc: a915         	add	r1, sp, #0x54
    7ede: 4638         	mov	r0, r7
    7ee0: f7fc e8c8    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x3e70
    7ee4: 9b15         	ldr	r3, [sp, #0x54]
    7ee6: 9024         	str	r0, [sp, #0x90]
    7ee8: 9326         	str	r3, [sp, #0x98]
    7eea: 462a         	mov	r2, r5
    7eec: 4631         	mov	r1, r6
    7eee: f7fc e84c    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x3f68
    7ef2: 9d15         	ldr	r5, [sp, #0x54]
    7ef4: 9a24         	ldr	r2, [sp, #0x90]
    7ef6: e66f         	b	0x7bd8 <ConnectionManager::setSoundOut(LSMessage&)+0x98> @ imm = #-0x322
    7ef8: b132         	cbz	r2, 0x7f08 <ConnectionManager::setSoundOut(LSMessage&)+0x3c8> @ imm = #0xc
    7efa: 2a01         	cmp	r2, #0x1
    7efc: d00d         	beq	0x7f1a <ConnectionManager::setSoundOut(LSMessage&)+0x3da> @ imm = #0x1a
    7efe: 4621         	mov	r1, r4
    7f00: f7fc e842    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x3f7c
    7f04: 9a25         	ldr	r2, [sp, #0x94]
    7f06: 9818         	ldr	r0, [sp, #0x60]
    7f08: 2300         	movs	r3, #0x0
    7f0a: 9219         	str	r2, [sp, #0x64]
    7f0c: 5483         	strb	r3, [r0, r2]
    7f0e: 9824         	ldr	r0, [sp, #0x90]
    7f10: e69c         	b	0x7c4c <ConnectionManager::setSoundOut(LSMessage&)+0x10c> @ imm = #-0x2c8
    7f12: 9424         	str	r4, [sp, #0x90]
    7f14: ac26         	add	r4, sp, #0x98
    7f16: 4620         	mov	r0, r4
    7f18: e698         	b	0x7c4c <ConnectionManager::setSoundOut(LSMessage&)+0x10c> @ imm = #-0x2d0
    7f1a: f89d 3098    	ldrb.w	r3, [sp, #0x98]
    7f1e: 7003         	strb	r3, [r0]
    7f20: 9a25         	ldr	r2, [sp, #0x94]
    7f22: 9818         	ldr	r0, [sp, #0x60]
    7f24: e7f0         	b	0x7f08 <ConnectionManager::setSoundOut(LSMessage&)+0x3c8> @ imm = #-0x20
    7f26: 482f         	ldr	r0, [pc, #0xbc]         @ 0x7fe4 <ConnectionManager::setSoundOut(LSMessage&)+0x4a4> literal=0x576c
    7f28: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    7f2a: f7fb eeec    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x4228
    7f2e: f10d 0860    	add.w	r8, sp, #0x60
    7f32: 4640         	mov	r0, r8
    7f34: f7fc e8dc    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x3e48
    7f38: 4658         	mov	r0, r11
    7f3a: f7fb ef0e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x41e4
    7f3e: 980d         	ldr	r0, [sp, #0x34]
    7f40: b108         	cbz	r0, 0x7f46 <ConnectionManager::setSoundOut(LSMessage&)+0x406> @ imm = #0x2
    7f42: f7fc e884    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x3ef8
    7f46: f7fc e91c    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x3dc8
    7f4a: f10d 0860    	add.w	r8, sp, #0x60
    7f4e: 4648         	mov	r0, r9
    7f50: f7fc e8ce    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x3e64
    7f54: e7ed         	b	0x7f32 <ConnectionManager::setSoundOut(LSMessage&)+0x3f2> @ imm = #-0x26
    7f56: 4638         	mov	r0, r7
    7f58: f7fc e8ca    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x3e6c
    7f5c: 4630         	mov	r0, r6
    7f5e: f7fb eefc    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4208
    7f62: 4650         	mov	r0, r10
    7f64: f7fb eef8    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4210
    7f68: e7f1         	b	0x7f4e <ConnectionManager::setSoundOut(LSMessage&)+0x40e> @ imm = #-0x1e
    7f6a: 4638         	mov	r0, r7
    7f6c: f10d 0860    	add.w	r8, sp, #0x60
    7f70: f7fc e8be    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x3e84
    7f74: e7eb         	b	0x7f4e <ConnectionManager::setSoundOut(LSMessage&)+0x40e> @ imm = #-0x2a
    7f76: 4638         	mov	r0, r7
    7f78: f7fc e8ba    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x3e8c
    7f7c: 4b0b         	ldr	r3, [pc, #0x2c]         @ 0x7fac <ConnectionManager::setSoundOut(LSMessage&)+0x46c> literal=0x210
    7f7e: 4630         	mov	r0, r6
    7f80: f858 3003    	ldr.w	r3, [r8, r3]
    7f84: f10d 0860    	add.w	r8, sp, #0x60
    7f88: 3308         	adds	r3, #0x8
    7f8a: 9315         	str	r3, [sp, #0x54]
    7f8c: f7fb efde    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x4044
    7f90: e7dd         	b	0x7f4e <ConnectionManager::setSoundOut(LSMessage&)+0x40e> @ imm = #-0x46
    7f92: e7e0         	b	0x7f56 <ConnectionManager::setSoundOut(LSMessage&)+0x416> @ imm = #-0x40
    7f94: e7df         	b	0x7f56 <ConnectionManager::setSoundOut(LSMessage&)+0x416> @ imm = #-0x42
    7f96: e7de         	b	0x7f56 <ConnectionManager::setSoundOut(LSMessage&)+0x416> @ imm = #-0x44
    7f98: e7dd         	b	0x7f56 <ConnectionManager::setSoundOut(LSMessage&)+0x416> @ imm = #-0x46
    7f9a: e7e2         	b	0x7f62 <ConnectionManager::setSoundOut(LSMessage&)+0x422> @ imm = #-0x3c
    7f9c: 980d         	ldr	r0, [sp, #0x34]
    7f9e: e7d0         	b	0x7f42 <ConnectionManager::setSoundOut(LSMessage&)+0x402> @ imm = #-0x60
    7fa0: 9262         	str	r2, [sp, #0x188]
    7fa2: 0001         	movs	r1, r0
    7fa4: 64ac         	str	r4, [r5, #0x48]
    7fa6: 0000         	movs	r0, r0
    7fa8: 64b0         	str	r0, [r6, #0x48]
    7faa: 0000         	movs	r0, r0
    7fac: 0210         	lsls	r0, r2, #0x8
    7fae: 0000         	movs	r0, r0
    7fb0: 6438         	str	r0, [r7, #0x40]
    7fb2: 0000         	movs	r0, r0
    7fb4: 5aba         	ldrh	r2, [r7, r2]
    7fb6: 0000         	movs	r0, r0
    7fb8: 6438         	str	r0, [r7, #0x40]
    7fba: 0000         	movs	r0, r0
    7fbc: 5a08         	ldrh	r0, [r1, r0]
    7fbe: 0000         	movs	r0, r0
    7fc0: 6358         	str	r0, [r3, #0x34]
    7fc2: 0000         	movs	r0, r0
    7fc4: 62da         	str	r2, [r3, #0x2c]
    7fc6: 0000         	movs	r0, r0
    7fc8: 595c         	ldr	r4, [r3, r5]
    7fca: 0000         	movs	r0, r0
    7fcc: 630e         	str	r6, [r1, #0x30]
    7fce: 0000         	movs	r0, r0
    7fd0: 922e         	str	r2, [sp, #0xb8]
    7fd2: 0001         	movs	r1, r0
    7fd4: 58f0         	ldr	r0, [r6, r3]
    7fd6: 0000         	movs	r0, r0
    7fd8: 91d8         	str	r1, [sp, #0x360]
    7fda: 0001         	movs	r1, r0
    7fdc: 67dc         	str	r4, [r3, #0x7c]
    7fde: 0000         	movs	r0, r0
    7fe0: 67a6         	str	r6, [r4, #0x78]
    7fe2: 0000         	movs	r0, r0
    7fe4: 576c         	ldrsb	r4, [r5, r5]
    7fe6: 0000         	movs	r0, r0

00007fe8 <ConnectionManager::mute(LSMessage&)>:
    7fe8: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    7fec: b0b9         	sub	sp, #0xe4
    7fee: f8df a61c    	ldr.w	r10, [pc, #0x61c]       @ 0x860c <ConnectionManager::mute(LSMessage&)+0x624> literal=0x18dba
    7ff2: 4681         	mov	r9, r0
    7ff4: f10d 0850    	add.w	r8, sp, #0x50
    7ff8: 4608         	mov	r0, r1
    7ffa: 460c         	mov	r4, r1
    7ffc: f8cd 9030    	str.w	r9, [sp, #0x30]
    8000: 9112         	str	r1, [sp, #0x48]
    8002: 44fa         	add	r10, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    8004: f7fc e86e    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x3f24
    8008: 4640         	mov	r0, r8
    800a: f7fc e908    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x3df0
    800e: ab1c         	add	r3, sp, #0x70
    8010: ad32         	add	r5, sp, #0xc8
    8012: 930a         	str	r3, [sp, #0x28]
    8014: 2200         	movs	r2, #0x0
    8016: 931a         	str	r3, [sp, #0x68]
    8018: ab22         	add	r3, sp, #0x88
    801a: 930b         	str	r3, [sp, #0x2c]
    801c: a826         	add	r0, sp, #0x98
    801e: 9320         	str	r3, [sp, #0x80]
    8020: ab28         	add	r3, sp, #0xa0
    8022: 930e         	str	r3, [sp, #0x38]
    8024: 4629         	mov	r1, r5
    8026: 9326         	str	r3, [sp, #0x98]
    8028: 23af         	movs	r3, #0xaf
    802a: 9213         	str	r2, [sp, #0x4c]
    802c: 921b         	str	r2, [sp, #0x6c]
    802e: f88d 2070    	strb.w	r2, [sp, #0x70]
    8032: 9221         	str	r2, [sp, #0x84]
    8034: f88d 2088    	strb.w	r2, [sp, #0x88]
    8038: 900d         	str	r0, [sp, #0x34]
    803a: 9332         	str	r3, [sp, #0xc8]
    803c: f7fc e81a    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x3fcc
    8040: f8df 15cc    	ldr.w	r1, [pc, #0x5cc]        @ 0x8610 <ConnectionManager::mute(LSMessage&)+0x628> literal=0x60e8
    8044: 22af         	movs	r2, #0xaf
    8046: 9b32         	ldr	r3, [sp, #0xc8]
    8048: f10d 0968    	add.w	r9, sp, #0x68
    804c: 9026         	str	r0, [sp, #0x98]
    804e: af20         	add	r7, sp, #0x80
    8050: 4479         	add	r1, pc => raw=0xe13c ghidra=0x1e13c file=57660 "{\"type\":\"object\",\"properties\":{\"source\":{\"type\":\"string\"},\"sink\":{\"type\":\"string\"},\"mute\":{\"type\":\"boolean\"}},\"required\":[\"source\",\"sink\",\"mute\"],\"additionalProperties\":false}"
    8052: 9328         	str	r3, [sp, #0xa0]
    8054: f7fb ef98    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x40d0
    8058: 9b32         	ldr	r3, [sp, #0xc8]
    805a: 2100         	movs	r1, #0x0
    805c: 9a26         	ldr	r2, [sp, #0x98]
    805e: 4620         	mov	r0, r4
    8060: 9327         	str	r3, [sp, #0x9c]
    8062: 54d1         	strb	r1, [r2, r3]
    8064: f7fc e898    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x3ed0
    8068: ac34         	add	r4, sp, #0xd0
    806a: 4683         	mov	r11, r0
    806c: 9432         	str	r4, [sp, #0xc8]
    806e: 2800         	cmp	r0, #0x0
    8070: f000 826f    	beq.w	0x8552 <ConnectionManager::mute(LSMessage&)+0x56a> @ imm = #0x4de
    8074: f7fc e816    	blx	0x40a4 <strlen@plt>     @ imm = #-0x3fd4
    8078: 280f         	cmp	r0, #0xf
    807a: 4606         	mov	r6, r0
    807c: 902c         	str	r0, [sp, #0xb0]
    807e: f200 81bd    	bhi.w	0x83fc <ConnectionManager::mute(LSMessage&)+0x414> @ imm = #0x37a
    8082: 2801         	cmp	r0, #0x1
    8084: f040 81a8    	bne.w	0x83d8 <ConnectionManager::mute(LSMessage&)+0x3f0> @ imm = #0x350
    8088: f89b 3000    	ldrb.w	r3, [r11]
    808c: f88d 30d0    	strb.w	r3, [sp, #0xd0]
    8090: 4623         	mov	r3, r4
    8092: 2200         	movs	r2, #0x0
    8094: 9633         	str	r6, [sp, #0xcc]
    8096: 4641         	mov	r1, r8
    8098: 559a         	strb	r2, [r3, r6]
    809a: 4628         	mov	r0, r5
    809c: 9a0d         	ldr	r2, [sp, #0x34]
    809e: ab13         	add	r3, sp, #0x4c
    80a0: f002 f99a    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #0x2334
    80a4: 4606         	mov	r6, r0
    80a6: 9832         	ldr	r0, [sp, #0xc8]
    80a8: 42a0         	cmp	r0, r4
    80aa: d003         	beq	0x80b4 <ConnectionManager::mute(LSMessage&)+0xcc> @ imm = #0x6
    80ac: 9934         	ldr	r1, [sp, #0xd0]
    80ae: 3101         	adds	r1, #0x1
    80b0: f7fb ef26    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x41b4
    80b4: 2e00         	cmp	r6, #0x0
    80b6: f000 8195    	beq.w	0x83e4 <ConnectionManager::mute(LSMessage&)+0x3fc> @ imm = #0x32a
    80ba: f8df 2558    	ldr.w	r2, [pc, #0x558]        @ 0x8614 <ConnectionManager::mute(LSMessage&)+0x62c> literal=0x5d9c
    80be: 4641         	mov	r1, r8
    80c0: 4628         	mov	r0, r5
    80c2: f10d 0968    	add.w	r9, sp, #0x68
    80c6: af20         	add	r7, sp, #0x80
    80c8: 447a         	add	r2, pc => raw=0xde68 ghidra=0x1de68 file=56936 "sink"
    80ca: f7fb ef20    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x41c0
    80ce: ae2c         	add	r6, sp, #0xb0
    80d0: 2300         	movs	r3, #0x0
    80d2: af2e         	add	r7, sp, #0xb8
    80d4: 4631         	mov	r1, r6
    80d6: 4628         	mov	r0, r5
    80d8: 932d         	str	r3, [sp, #0xb4]
    80da: f88d 30b8    	strb.w	r3, [sp, #0xb8]
    80de: 972c         	str	r7, [sp, #0xb0]
    80e0: f7fb ee5e    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x4344
    80e4: 9b2c         	ldr	r3, [sp, #0xb0]
    80e6: 981a         	ldr	r0, [sp, #0x68]
    80e8: 9a2d         	ldr	r2, [sp, #0xb4]
    80ea: 42bb         	cmp	r3, r7
    80ec: f000 8207    	beq.w	0x84fe <ConnectionManager::mute(LSMessage&)+0x516> @ imm = #0x40e
    80f0: 990a         	ldr	r1, [sp, #0x28]
    80f2: f8dd c0b8    	ldr.w	r12, [sp, #0xb8]
    80f6: e9cd 321a    	strd	r3, r2, [sp, #104]
    80fa: 4288         	cmp	r0, r1
    80fc: bf08         	it	eq
    80fe: f8cd c070    	streq.w	r12, [sp, #0x70]
    8102: f000 820d    	beq.w	0x8520 <ConnectionManager::mute(LSMessage&)+0x538> @ imm = #0x41a
    8106: 9b1c         	ldr	r3, [sp, #0x70]
    8108: f8cd c070    	str.w	r12, [sp, #0x70]
    810c: 2800         	cmp	r0, #0x0
    810e: f000 8207    	beq.w	0x8520 <ConnectionManager::mute(LSMessage&)+0x538> @ imm = #0x40e
    8112: 932e         	str	r3, [sp, #0xb8]
    8114: 902c         	str	r0, [sp, #0xb0]
    8116: 2300         	movs	r3, #0x0
    8118: 932d         	str	r3, [sp, #0xb4]
    811a: 7003         	strb	r3, [r0]
    811c: 982c         	ldr	r0, [sp, #0xb0]
    811e: 42b8         	cmp	r0, r7
    8120: d003         	beq	0x812a <ConnectionManager::mute(LSMessage&)+0x142> @ imm = #0x6
    8122: 992e         	ldr	r1, [sp, #0xb8]
    8124: 3101         	adds	r1, #0x1
    8126: f7fb eeec    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4228
    812a: f8df 34ec    	ldr.w	r3, [pc, #0x4ec]        @ 0x8618 <ConnectionManager::mute(LSMessage&)+0x630> literal=0x210
    812e: 4628         	mov	r0, r5
    8130: f10d 0968    	add.w	r9, sp, #0x68
    8134: af20         	add	r7, sp, #0x80
    8136: f85a a003    	ldr.w	r10, [r10, r3]
    813a: f10a 0308    	add.w	r3, r10, #0x8
    813e: 9332         	str	r3, [sp, #0xc8]
    8140: f7fb ef04    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x41f8
    8144: f8df 24d4    	ldr.w	r2, [pc, #0x4d4]        @ 0x861c <ConnectionManager::mute(LSMessage&)+0x634> literal=0x59a4
    8148: 4641         	mov	r1, r8
    814a: 4630         	mov	r0, r6
    814c: 447a         	add	r2, pc => raw=0xdaf4 ghidra=0x1daf4 file=56052 "source"
    814e: f7fb eede    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x4244
    8152: 2300         	movs	r3, #0x0
    8154: 4629         	mov	r1, r5
    8156: 4630         	mov	r0, r6
    8158: 9432         	str	r4, [sp, #0xc8]
    815a: 9333         	str	r3, [sp, #0xcc]
    815c: f88d 30d0    	strb.w	r3, [sp, #0xd0]
    8160: f7fb ee1e    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x43c4
    8164: 9b32         	ldr	r3, [sp, #0xc8]
    8166: 9820         	ldr	r0, [sp, #0x80]
    8168: 9a33         	ldr	r2, [sp, #0xcc]
    816a: 42a3         	cmp	r3, r4
    816c: f000 81ba    	beq.w	0x84e4 <ConnectionManager::mute(LSMessage&)+0x4fc> @ imm = #0x374
    8170: 9f0b         	ldr	r7, [sp, #0x2c]
    8172: 9934         	ldr	r1, [sp, #0xd0]
    8174: e9cd 3220    	strd	r3, r2, [sp, #128]
    8178: 42b8         	cmp	r0, r7
    817a: bf08         	it	eq
    817c: 9122         	streq	r1, [sp, #0x88]
    817e: f000 81cb    	beq.w	0x8518 <ConnectionManager::mute(LSMessage&)+0x530> @ imm = #0x396
    8182: 9b22         	ldr	r3, [sp, #0x88]
    8184: 9122         	str	r1, [sp, #0x88]
    8186: 2800         	cmp	r0, #0x0
    8188: f000 81c6    	beq.w	0x8518 <ConnectionManager::mute(LSMessage&)+0x530> @ imm = #0x38c
    818c: 9334         	str	r3, [sp, #0xd0]
    818e: 9032         	str	r0, [sp, #0xc8]
    8190: 2300         	movs	r3, #0x0
    8192: 9333         	str	r3, [sp, #0xcc]
    8194: 7003         	strb	r3, [r0]
    8196: 9832         	ldr	r0, [sp, #0xc8]
    8198: 42a0         	cmp	r0, r4
    819a: d003         	beq	0x81a4 <ConnectionManager::mute(LSMessage&)+0x1bc> @ imm = #0x6
    819c: 9934         	ldr	r1, [sp, #0xd0]
    819e: 3101         	adds	r1, #0x1
    81a0: f7fb eeae    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x42a4
    81a4: f10a 0308    	add.w	r3, r10, #0x8
    81a8: 4630         	mov	r0, r6
    81aa: 932c         	str	r3, [sp, #0xb0]
    81ac: f10d 0968    	add.w	r9, sp, #0x68
    81b0: f7fb eecc    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x4268
    81b4: f8df 2468    	ldr.w	r2, [pc, #0x468]        @ 0x8620 <ConnectionManager::mute(LSMessage&)+0x638> literal=0x602a
    81b8: 4641         	mov	r1, r8
    81ba: 4628         	mov	r0, r5
    81bc: af20         	add	r7, sp, #0x80
    81be: 447a         	add	r2, pc => raw=0xe1ec ghidra=0x1e1ec file=57836 "mute"
    81c0: f7fb eea4    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x42b8
    81c4: f10d 0146    	add.w	r1, sp, #0x46
    81c8: 4628         	mov	r0, r5
    81ca: f7fb ee0e    	blx	0x3de8 <_ZNK7pbnjson6JValue6asBoolERb@plt> @ imm = #-0x43e4
    81ce: f10a 0308    	add.w	r3, r10, #0x8
    81d2: 4628         	mov	r0, r5
    81d4: 9332         	str	r3, [sp, #0xc8]
    81d6: f89d 3046    	ldrb.w	r3, [sp, #0x46]
    81da: 930f         	str	r3, [sp, #0x3c]
    81dc: f7fb eeb6    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x4294
    81e0: f002 f9ac    	bl	0xa53c <getPmLogContext()> @ imm = #0x2358
    81e4: b138         	cbz	r0, 0x81f6 <ConnectionManager::mute(LSMessage&)+0x20e> @ imm = #0xe
    81e6: f10d 0968    	add.w	r9, sp, #0x68
    81ea: af20         	add	r7, sp, #0x80
    81ec: f002 f9a6    	bl	0xa53c <getPmLogContext()> @ imm = #0x234c
    81f0: 6803         	ldr	r3, [r0]
    81f2: 2b06         	cmp	r3, #0x6
    81f4: dd1e         	ble	0x8234 <ConnectionManager::mute(LSMessage&)+0x24c> @ imm = #0x3c
    81f6: f10d 0968    	add.w	r9, sp, #0x68
    81fa: af20         	add	r7, sp, #0x80
    81fc: f002 f99e    	bl	0xa53c <getPmLogContext()> @ imm = #0x233c
    8200: 9b0f         	ldr	r3, [sp, #0x3c]
    8202: 2107         	movs	r1, #0x7
    8204: 9308         	str	r3, [sp, #0x20]
    8206: 9b1a         	ldr	r3, [sp, #0x68]
    8208: 9307         	str	r3, [sp, #0x1c]
    820a: 9b20         	ldr	r3, [sp, #0x80]
    820c: 9306         	str	r3, [sp, #0x18]
    820e: f8df 3414    	ldr.w	r3, [pc, #0x414]        @ 0x8624 <ConnectionManager::mute(LSMessage&)+0x63c> literal=0x5fd6
    8212: 447b         	add	r3, pc => raw=0xe1ec ghidra=0x1e1ec file=57836 "mute"
    8214: 9305         	str	r3, [sp, #0x14]
    8216: f8df 3410    	ldr.w	r3, [pc, #0x410]        @ 0x8628 <ConnectionManager::mute(LSMessage&)+0x640> literal=0x5532
    821a: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    821c: 9304         	str	r3, [sp, #0x10]
    821e: f8df 340c    	ldr.w	r3, [pc, #0x40c]        @ 0x862c <ConnectionManager::mute(LSMessage&)+0x644> literal=0x5fce
    8222: 447b         	add	r3, pc => raw=0xe1f4 ghidra=0x1e1f4 file=57844 "%s:%s() Audio mute called for source %s, sink %s, mute %d"
    8224: 9303         	str	r3, [sp, #0xc]
    8226: 2300         	movs	r3, #0x0
    8228: 461a         	mov	r2, r3
    822a: e9cd 3301    	strd	r3, r3, [sp, #4]
    822e: 9300         	str	r3, [sp]
    8230: f7fb eef4    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x4218
    8234: f10d 0968    	add.w	r9, sp, #0x68
    8238: af20         	add	r7, sp, #0x80
    823a: 980c         	ldr	r0, [sp, #0x30]
    823c: 464a         	mov	r2, r9
    823e: 4639         	mov	r1, r7
    8240: f7fd ff24    	bl	0x608c <ConnectionManager::findAudioConnection(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)> @ imm = #-0x21b8
    8244: 4682         	mov	r10, r0
    8246: 2800         	cmp	r0, #0x0
    8248: f000 816e    	beq.w	0x8528 <ConnectionManager::mute(LSMessage&)+0x540> @ imm = #0x2dc
    824c: f10d 0b5c    	add.w	r11, sp, #0x5c
    8250: 4658         	mov	r0, r11
    8252: f7fb edb2    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x449c
    8256: 9a0f         	ldr	r2, [sp, #0x3c]
    8258: f89a 3048    	ldrb.w	r3, [r10, #0x48]
    825c: f88d 2047    	strb.w	r2, [sp, #0x47]
    8260: 4293         	cmp	r3, r2
    8262: d00e         	beq	0x8282 <ConnectionManager::mute(LSMessage&)+0x29a> @ imm = #0x1c
    8264: 980c         	ldr	r0, [sp, #0x30]
    8266: f10d 0347    	add.w	r3, sp, #0x47
    826a: f10a 0218    	add.w	r2, r10, #0x18
    826e: 4651         	mov	r1, r10
    8270: f7fe fa50    	bl	0x6714 <ConnectionManager::mute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool const&)> @ imm = #-0x1b60
    8274: 2800         	cmp	r0, #0x0
    8276: f040 80d3    	bne.w	0x8420 <ConnectionManager::mute(LSMessage&)+0x438> @ imm = #0x1a6
    827a: f89d 3047    	ldrb.w	r3, [sp, #0x47]
    827e: f88a 3048    	strb.w	r3, [r10, #0x48]
    8282: 2101         	movs	r1, #0x1
    8284: 4630         	mov	r0, r6
    8286: f7fb edb6    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x4494
    828a: 4ae9         	ldr	r2, [pc, #0x3a4]        @ 0x8630 <ConnectionManager::mute(LSMessage&)+0x648> literal=0x5488
    828c: 4623         	mov	r3, r4
    828e: 9432         	str	r4, [sp, #0xc8]
    8290: f04f 0e0b    	mov.w	lr, #0xb
    8294: f04f 0c00    	mov.w	r12, #0x0
    8298: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    829a: ca07         	ldm	r2, {r0, r1, r2}
    829c: c303         	stm	r3!, {r0, r1}
    829e: 4629         	mov	r1, r5
    82a0: f823 2b02    	strh	r2, [r3], #2
    82a4: 0c12         	lsrs	r2, r2, #0x10
    82a6: 4658         	mov	r0, r11
    82a8: 701a         	strb	r2, [r3]
    82aa: 4632         	mov	r2, r6
    82ac: f8cd e0cc    	str.w	lr, [sp, #0xcc]
    82b0: f88d c0db    	strb.w	r12, [sp, #0xdb]
    82b4: f7fb ed0e    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x45e4
    82b8: 9832         	ldr	r0, [sp, #0xc8]
    82ba: 42a0         	cmp	r0, r4
    82bc: d003         	beq	0x82c6 <ConnectionManager::mute(LSMessage&)+0x2de> @ imm = #0x6
    82be: 9934         	ldr	r1, [sp, #0xd0]
    82c0: 3101         	adds	r1, #0x1
    82c2: f7fb ee1e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x43c4
    82c6: 4630         	mov	r0, r6
    82c8: f7fb ed46    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4574
    82cc: 4649         	mov	r1, r9
    82ce: 4630         	mov	r0, r6
    82d0: f7fb ef2c    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x41a8
    82d4: f646 1373    	movw	r3, #0x6973
    82d8: 4632         	mov	r2, r6
    82da: f6c6 336e    	movt	r3, #0x6b6e
    82de: 4629         	mov	r1, r5
    82e0: 9334         	str	r3, [sp, #0xd0]
    82e2: 2304         	movs	r3, #0x4
    82e4: 9333         	str	r3, [sp, #0xcc]
    82e6: 4658         	mov	r0, r11
    82e8: 2300         	movs	r3, #0x0
    82ea: 9432         	str	r4, [sp, #0xc8]
    82ec: f88d 30d4    	strb.w	r3, [sp, #0xd4]
    82f0: f7fb ecf0    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4620
    82f4: 9832         	ldr	r0, [sp, #0xc8]
    82f6: 42a0         	cmp	r0, r4
    82f8: d003         	beq	0x8302 <ConnectionManager::mute(LSMessage&)+0x31a> @ imm = #0x6
    82fa: 9934         	ldr	r1, [sp, #0xd0]
    82fc: 3101         	adds	r1, #0x1
    82fe: f7fb ee00    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4400
    8302: 4630         	mov	r0, r6
    8304: f7fb ed28    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x45b0
    8308: 4639         	mov	r1, r7
    830a: 4630         	mov	r0, r6
    830c: f7fb ef0e    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x41e4
    8310: 4bc8         	ldr	r3, [pc, #0x320]        @ 0x8634 <ConnectionManager::mute(LSMessage&)+0x64c> literal=0x57d2
    8312: 2206         	movs	r2, #0x6
    8314: 9233         	str	r2, [sp, #0xcc]
    8316: 2200         	movs	r2, #0x0
    8318: f88d 20d6    	strb.w	r2, [sp, #0xd6]
    831c: 4632         	mov	r2, r6
    831e: 447b         	add	r3, pc => raw=0xdaf4 ghidra=0x1daf4 file=56052 "source"
    8320: 9432         	str	r4, [sp, #0xc8]
    8322: e893 0003    	ldm.w	r3, {r0, r1}
    8326: 9034         	str	r0, [sp, #0xd0]
    8328: 4658         	mov	r0, r11
    832a: f8ad 10d4    	strh.w	r1, [sp, #0xd4]
    832e: 4629         	mov	r1, r5
    8330: f7fb ecd0    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4660
    8334: 9832         	ldr	r0, [sp, #0xc8]
    8336: 42a0         	cmp	r0, r4
    8338: d003         	beq	0x8342 <ConnectionManager::mute(LSMessage&)+0x35a> @ imm = #0x6
    833a: 9934         	ldr	r1, [sp, #0xd0]
    833c: 3101         	adds	r1, #0x1
    833e: f7fb ede0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4440
    8342: 4630         	mov	r0, r6
    8344: f7fb ed08    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x45f0
    8348: 990f         	ldr	r1, [sp, #0x3c]
    834a: 4630         	mov	r0, r6
    834c: f7fb ed52    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x455c
    8350: f247 536d    	movw	r3, #0x756d
    8354: 4632         	mov	r2, r6
    8356: f2c6 5374    	movt	r3, #0x6574
    835a: 4629         	mov	r1, r5
    835c: 9334         	str	r3, [sp, #0xd0]
    835e: 2304         	movs	r3, #0x4
    8360: 9333         	str	r3, [sp, #0xcc]
    8362: 4658         	mov	r0, r11
    8364: 2300         	movs	r3, #0x0
    8366: 9432         	str	r4, [sp, #0xc8]
    8368: f88d 30d4    	strb.w	r3, [sp, #0xd4]
    836c: f7fb ecb2    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x469c
    8370: 9832         	ldr	r0, [sp, #0xc8]
    8372: 42a0         	cmp	r0, r4
    8374: d003         	beq	0x837e <ConnectionManager::mute(LSMessage&)+0x396> @ imm = #0x6
    8376: 9934         	ldr	r1, [sp, #0xd0]
    8378: 3101         	adds	r1, #0x1
    837a: f7fb edc2    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x447c
    837e: 4630         	mov	r0, r6
    8380: f7fb ecea    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x462c
    8384: 4659         	mov	r1, r11
    8386: a812         	add	r0, sp, #0x48
    8388: f001 fd82    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #0x1b04
    838c: 4658         	mov	r0, r11
    838e: f7fb ece4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4638
    8392: 9826         	ldr	r0, [sp, #0x98]
    8394: 9b0e         	ldr	r3, [sp, #0x38]
    8396: 4298         	cmp	r0, r3
    8398: d003         	beq	0x83a2 <ConnectionManager::mute(LSMessage&)+0x3ba> @ imm = #0x6
    839a: 9928         	ldr	r1, [sp, #0xa0]
    839c: 3101         	adds	r1, #0x1
    839e: f7fb edb0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x44a0
    83a2: 9820         	ldr	r0, [sp, #0x80]
    83a4: 9b0b         	ldr	r3, [sp, #0x2c]
    83a6: 4298         	cmp	r0, r3
    83a8: d003         	beq	0x83b2 <ConnectionManager::mute(LSMessage&)+0x3ca> @ imm = #0x6
    83aa: 9922         	ldr	r1, [sp, #0x88]
    83ac: 3101         	adds	r1, #0x1
    83ae: f7fb eda8    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x44b0
    83b2: 981a         	ldr	r0, [sp, #0x68]
    83b4: 9b0a         	ldr	r3, [sp, #0x28]
    83b6: 4298         	cmp	r0, r3
    83b8: d003         	beq	0x83c2 <ConnectionManager::mute(LSMessage&)+0x3da> @ imm = #0x6
    83ba: 991c         	ldr	r1, [sp, #0x70]
    83bc: 3101         	adds	r1, #0x1
    83be: f7fb eda0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x44c0
    83c2: 4640         	mov	r0, r8
    83c4: f7fb ecc8    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4670
    83c8: 9812         	ldr	r0, [sp, #0x48]
    83ca: b108         	cbz	r0, 0x83d0 <ConnectionManager::mute(LSMessage&)+0x3e8> @ imm = #0x2
    83cc: f7fb ee3e    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x4384
    83d0: 2001         	movs	r0, #0x1
    83d2: b039         	add	sp, #0xe4
    83d4: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    83d8: 4623         	mov	r3, r4
    83da: 2800         	cmp	r0, #0x0
    83dc: f43f ae59    	beq.w	0x8092 <ConnectionManager::mute(LSMessage&)+0xaa> @ imm = #-0x34e
    83e0: 4620         	mov	r0, r4
    83e2: e016         	b	0x8412 <ConnectionManager::mute(LSMessage&)+0x42a> @ imm = #0x2c
    83e4: 4994         	ldr	r1, [pc, #0x250]        @ 0x8638 <ConnectionManager::mute(LSMessage&)+0x650> literal=0x18c4e
    83e6: 4633         	mov	r3, r6
    83e8: 2203         	movs	r2, #0x3
    83ea: a812         	add	r0, sp, #0x48
    83ec: f10d 0968    	add.w	r9, sp, #0x68
    83f0: af20         	add	r7, sp, #0x80
    83f2: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    83f4: 311c         	adds	r1, #0x1c
    83f6: f001 fc29    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0x1852
    83fa: e7ca         	b	0x8392 <ConnectionManager::mute(LSMessage&)+0x3aa> @ imm = #-0x6c
    83fc: 2200         	movs	r2, #0x0
    83fe: a92c         	add	r1, sp, #0xb0
    8400: 4628         	mov	r0, r5
    8402: f10d 0968    	add.w	r9, sp, #0x68
    8406: af20         	add	r7, sp, #0x80
    8408: f7fb ee34    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x4398
    840c: 9b2c         	ldr	r3, [sp, #0xb0]
    840e: 9032         	str	r0, [sp, #0xc8]
    8410: 9334         	str	r3, [sp, #0xd0]
    8412: 4632         	mov	r2, r6
    8414: 4659         	mov	r1, r11
    8416: f7fb edb8    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x4490
    841a: 9e2c         	ldr	r6, [sp, #0xb0]
    841c: 9b32         	ldr	r3, [sp, #0xc8]
    841e: e638         	b	0x8092 <ConnectionManager::mute(LSMessage&)+0xaa> @ imm = #-0x390
    8420: 2100         	movs	r1, #0x0
    8422: 4630         	mov	r0, r6
    8424: f7fb ece6    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x4634
    8428: 4a84         	ldr	r2, [pc, #0x210]        @ 0x863c <ConnectionManager::mute(LSMessage&)+0x654> literal=0x52ea
    842a: 4623         	mov	r3, r4
    842c: 9432         	str	r4, [sp, #0xc8]
    842e: f04f 0e0b    	mov.w	lr, #0xb
    8432: f04f 0c00    	mov.w	r12, #0x0
    8436: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    8438: ca07         	ldm	r2, {r0, r1, r2}
    843a: c303         	stm	r3!, {r0, r1}
    843c: 4629         	mov	r1, r5
    843e: f823 2b02    	strh	r2, [r3], #2
    8442: 0c12         	lsrs	r2, r2, #0x10
    8444: 4658         	mov	r0, r11
    8446: 701a         	strb	r2, [r3]
    8448: 4632         	mov	r2, r6
    844a: f8cd e0cc    	str.w	lr, [sp, #0xcc]
    844e: f88d c0db    	strb.w	r12, [sp, #0xdb]
    8452: f7fb ec40    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4780
    8456: 9832         	ldr	r0, [sp, #0xc8]
    8458: 42a0         	cmp	r0, r4
    845a: d003         	beq	0x8464 <ConnectionManager::mute(LSMessage&)+0x47c> @ imm = #0x6
    845c: 9934         	ldr	r1, [sp, #0xd0]
    845e: 3101         	adds	r1, #0x1
    8460: f7fb ed4e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4564
    8464: 4630         	mov	r0, r6
    8466: f7fb ec78    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4710
    846a: 4975         	ldr	r1, [pc, #0x1d4]        @ 0x8640 <ConnectionManager::mute(LSMessage&)+0x658> literal=0x18bd2
    846c: 4630         	mov	r0, r6
    846e: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    8470: 3164         	adds	r1, #0x64
    8472: f7fb ee5c    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x4348
    8476: 4a73         	ldr	r2, [pc, #0x1cc]        @ 0x8644 <ConnectionManager::mute(LSMessage&)+0x65c> literal=0x61d4
    8478: 4623         	mov	r3, r4
    847a: 9432         	str	r4, [sp, #0xc8]
    847c: f04f 0e09    	mov.w	lr, #0x9
    8480: f04f 0c00    	mov.w	r12, #0x0
    8484: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    8486: ca07         	ldm	r2, {r0, r1, r2}
    8488: c303         	stm	r3!, {r0, r1}
    848a: 4629         	mov	r1, r5
    848c: 701a         	strb	r2, [r3]
    848e: 4658         	mov	r0, r11
    8490: 4632         	mov	r2, r6
    8492: f8cd e0cc    	str.w	lr, [sp, #0xcc]
    8496: f88d c0d9    	strb.w	r12, [sp, #0xd9]
    849a: f7fb ec1c    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x47c8
    849e: 9832         	ldr	r0, [sp, #0xc8]
    84a0: 42a0         	cmp	r0, r4
    84a2: d003         	beq	0x84ac <ConnectionManager::mute(LSMessage&)+0x4c4> @ imm = #0x6
    84a4: 9934         	ldr	r1, [sp, #0xd0]
    84a6: 3101         	adds	r1, #0x1
    84a8: f7fb ed2a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x45ac
    84ac: 4630         	mov	r0, r6
    84ae: f7fb ec54    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4758
    84b2: 2114         	movs	r1, #0x14
    84b4: 4630         	mov	r0, r6
    84b6: f7fb ee40    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x4380
    84ba: 4a63         	ldr	r2, [pc, #0x18c]        @ 0x8648 <ConnectionManager::mute(LSMessage&)+0x660> literal=0x619c
    84bc: 4623         	mov	r3, r4
    84be: 9432         	str	r4, [sp, #0xc8]
    84c0: f04f 0e09    	mov.w	lr, #0x9
    84c4: f04f 0c00    	mov.w	r12, #0x0
    84c8: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    84ca: ca07         	ldm	r2, {r0, r1, r2}
    84cc: c303         	stm	r3!, {r0, r1}
    84ce: 4629         	mov	r1, r5
    84d0: 701a         	strb	r2, [r3]
    84d2: 4658         	mov	r0, r11
    84d4: 4632         	mov	r2, r6
    84d6: f8cd e0cc    	str.w	lr, [sp, #0xcc]
    84da: f88d c0d9    	strb.w	r12, [sp, #0xd9]
    84de: f7fb ebfa    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x480c
    84e2: e745         	b	0x8370 <ConnectionManager::mute(LSMessage&)+0x388> @ imm = #-0x176
    84e4: b132         	cbz	r2, 0x84f4 <ConnectionManager::mute(LSMessage&)+0x50c> @ imm = #0xc
    84e6: 2a01         	cmp	r2, #0x1
    84e8: d027         	beq	0x853a <ConnectionManager::mute(LSMessage&)+0x552> @ imm = #0x4e
    84ea: 4621         	mov	r1, r4
    84ec: f7fb ed4c    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x4568
    84f0: 9a33         	ldr	r2, [sp, #0xcc]
    84f2: 9820         	ldr	r0, [sp, #0x80]
    84f4: 2300         	movs	r3, #0x0
    84f6: 9221         	str	r2, [sp, #0x84]
    84f8: 5483         	strb	r3, [r0, r2]
    84fa: 9832         	ldr	r0, [sp, #0xc8]
    84fc: e648         	b	0x8190 <ConnectionManager::mute(LSMessage&)+0x1a8> @ imm = #-0x370
    84fe: b132         	cbz	r2, 0x850e <ConnectionManager::mute(LSMessage&)+0x526> @ imm = #0xc
    8500: 2a01         	cmp	r2, #0x1
    8502: d020         	beq	0x8546 <ConnectionManager::mute(LSMessage&)+0x55e> @ imm = #0x40
    8504: 4639         	mov	r1, r7
    8506: f7fb ed40    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x4580
    850a: 9a2d         	ldr	r2, [sp, #0xb4]
    850c: 981a         	ldr	r0, [sp, #0x68]
    850e: 2300         	movs	r3, #0x0
    8510: 921b         	str	r2, [sp, #0x6c]
    8512: 5483         	strb	r3, [r0, r2]
    8514: 982c         	ldr	r0, [sp, #0xb0]
    8516: e5fe         	b	0x8116 <ConnectionManager::mute(LSMessage&)+0x12e> @ imm = #-0x404
    8518: 9432         	str	r4, [sp, #0xc8]
    851a: ac34         	add	r4, sp, #0xd0
    851c: 4620         	mov	r0, r4
    851e: e637         	b	0x8190 <ConnectionManager::mute(LSMessage&)+0x1a8> @ imm = #-0x392
    8520: 972c         	str	r7, [sp, #0xb0]
    8522: af2e         	add	r7, sp, #0xb8
    8524: 4638         	mov	r0, r7
    8526: e5f6         	b	0x8116 <ConnectionManager::mute(LSMessage&)+0x12e> @ imm = #-0x414
    8528: 4948         	ldr	r1, [pc, #0x120]        @ 0x864c <ConnectionManager::mute(LSMessage&)+0x664> literal=0x18b10
    852a: 4603         	mov	r3, r0
    852c: 22c8         	movs	r2, #0xc8
    852e: a812         	add	r0, sp, #0x48
    8530: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    8532: 317c         	adds	r1, #0x7c
    8534: f001 fb8a    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0x1714
    8538: e72b         	b	0x8392 <ConnectionManager::mute(LSMessage&)+0x3aa> @ imm = #-0x1aa
    853a: f89d 30d0    	ldrb.w	r3, [sp, #0xd0]
    853e: 7003         	strb	r3, [r0]
    8540: 9a33         	ldr	r2, [sp, #0xcc]
    8542: 9820         	ldr	r0, [sp, #0x80]
    8544: e7d6         	b	0x84f4 <ConnectionManager::mute(LSMessage&)+0x50c> @ imm = #-0x54
    8546: f89d 30b8    	ldrb.w	r3, [sp, #0xb8]
    854a: 7003         	strb	r3, [r0]
    854c: 9a2d         	ldr	r2, [sp, #0xb4]
    854e: 981a         	ldr	r0, [sp, #0x68]
    8550: e7dd         	b	0x850e <ConnectionManager::mute(LSMessage&)+0x526> @ imm = #-0x46
    8552: 483f         	ldr	r0, [pc, #0xfc]         @ 0x8650 <ConnectionManager::mute(LSMessage&)+0x668> literal=0x513a
    8554: f10d 0968    	add.w	r9, sp, #0x68
    8558: af20         	add	r7, sp, #0x80
    855a: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    855c: f7fb ebd2    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x485c
    8560: 4628         	mov	r0, r5
    8562: f7fb edc6    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4474
    8566: 4630         	mov	r0, r6
    8568: f7fb ebf6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4814
    856c: 4658         	mov	r0, r11
    856e: f7fb ebf4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4818
    8572: 980d         	ldr	r0, [sp, #0x34]
    8574: f7fb edbc    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4488
    8578: 4638         	mov	r0, r7
    857a: f7fb edba    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x448c
    857e: 4648         	mov	r0, r9
    8580: f7fb edb6    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4494
    8584: 4640         	mov	r0, r8
    8586: f7fb ebe8    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4830
    858a: 9812         	ldr	r0, [sp, #0x48]
    858c: b108         	cbz	r0, 0x8592 <ConnectionManager::mute(LSMessage&)+0x5aa> @ imm = #0x2
    858e: f7fb ed5e    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x4544
    8592: f7fb edf6    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x4414
    8596: e7e3         	b	0x8560 <ConnectionManager::mute(LSMessage&)+0x578> @ imm = #-0x3a
    8598: e7e2         	b	0x8560 <ConnectionManager::mute(LSMessage&)+0x578> @ imm = #-0x3c
    859a: f10d 0968    	add.w	r9, sp, #0x68
    859e: af20         	add	r7, sp, #0x80
    85a0: e7ea         	b	0x8578 <ConnectionManager::mute(LSMessage&)+0x590> @ imm = #-0x2c
    85a2: 4628         	mov	r0, r5
    85a4: f10d 0968    	add.w	r9, sp, #0x68
    85a8: af20         	add	r7, sp, #0x80
    85aa: f7fb eda2    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x44bc
    85ae: e7e0         	b	0x8572 <ConnectionManager::mute(LSMessage&)+0x58a> @ imm = #-0x40
    85b0: 4628         	mov	r0, r5
    85b2: f10d 0968    	add.w	r9, sp, #0x68
    85b6: f7fb ed9c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x44c8
    85ba: f10a 0308    	add.w	r3, r10, #0x8
    85be: 4630         	mov	r0, r6
    85c0: 932c         	str	r3, [sp, #0xb0]
    85c2: af20         	add	r7, sp, #0x80
    85c4: f7fb ecc2    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x467c
    85c8: e7d3         	b	0x8572 <ConnectionManager::mute(LSMessage&)+0x58a> @ imm = #-0x5a
    85ca: 4630         	mov	r0, r6
    85cc: f10d 0968    	add.w	r9, sp, #0x68
    85d0: f7fb ed8e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x44e4
    85d4: 4b10         	ldr	r3, [pc, #0x40]         @ 0x8618 <ConnectionManager::mute(LSMessage&)+0x630> literal=0x210
    85d6: 4628         	mov	r0, r5
    85d8: af20         	add	r7, sp, #0x80
    85da: f85a 3003    	ldr.w	r3, [r10, r3]
    85de: 3308         	adds	r3, #0x8
    85e0: 9332         	str	r3, [sp, #0xc8]
    85e2: f7fb ecb4    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x4698
    85e6: e7c4         	b	0x8572 <ConnectionManager::mute(LSMessage&)+0x58a> @ imm = #-0x78
    85e8: e7ba         	b	0x8560 <ConnectionManager::mute(LSMessage&)+0x578> @ imm = #-0x8c
    85ea: e7b9         	b	0x8560 <ConnectionManager::mute(LSMessage&)+0x578> @ imm = #-0x8e
    85ec: 9812         	ldr	r0, [sp, #0x48]
    85ee: e7ce         	b	0x858e <ConnectionManager::mute(LSMessage&)+0x5a6> @ imm = #-0x64
    85f0: e7bf         	b	0x8572 <ConnectionManager::mute(LSMessage&)+0x58a> @ imm = #-0x82
    85f2: e7b5         	b	0x8560 <ConnectionManager::mute(LSMessage&)+0x578> @ imm = #-0x96
    85f4: f10a 0308    	add.w	r3, r10, #0x8
    85f8: 4628         	mov	r0, r5
    85fa: 9332         	str	r3, [sp, #0xc8]
    85fc: f10d 0968    	add.w	r9, sp, #0x68
    8600: af20         	add	r7, sp, #0x80
    8602: f7fb eca4    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x46b8
    8606: e7b4         	b	0x8572 <ConnectionManager::mute(LSMessage&)+0x58a> @ imm = #-0x98
    8608: e7b0         	b	0x856c <ConnectionManager::mute(LSMessage&)+0x584> @ imm = #-0xa0
    860a: e7a9         	b	0x8560 <ConnectionManager::mute(LSMessage&)+0x578> @ imm = #-0xae
    860c: 8dba         	ldrh	r2, [r7, #0x2c]
    860e: 0001         	movs	r1, r0
    8610: 60e8         	str	r0, [r5, #0xc]
    8612: 0000         	movs	r0, r0
    8614: 5d9c         	ldrb	r4, [r3, r6]
    8616: 0000         	movs	r0, r0
    8618: 0210         	lsls	r0, r2, #0x8
    861a: 0000         	movs	r0, r0
    861c: 59a4         	ldr	r4, [r4, r6]
    861e: 0000         	movs	r0, r0
    8620: 602a         	str	r2, [r5]
    8622: 0000         	movs	r0, r0
    8624: 5fd6         	ldrsh	r6, [r2, r7]
    8626: 0000         	movs	r0, r0
    8628: 5532         	strb	r2, [r6, r4]
    862a: 0000         	movs	r0, r0
    862c: 5fce         	ldrsh	r6, [r1, r7]
    862e: 0000         	movs	r0, r0
    8630: 5488         	strb	r0, [r1, r2]
    8632: 0000         	movs	r0, r0
    8634: 57d2         	ldrsb	r2, [r2, r7]
    8636: 0000         	movs	r0, r0
    8638: 8c4e         	ldrh	r6, [r1, #0x22]
    863a: 0001         	movs	r1, r0
    863c: 52ea         	strh	r2, [r5, r3]
    863e: 0000         	movs	r0, r0
    8640: 8bd2         	ldrh	r2, [r2, #0x1e]
    8642: 0001         	movs	r1, r0
    8644: 61d4         	str	r4, [r2, #0x1c]
    8646: 0000         	movs	r0, r0
    8648: 619c         	str	r4, [r3, #0x18]
    864a: 0000         	movs	r0, r0
    864c: 8b10         	ldrh	r0, [r2, #0x18]
    864e: 0001         	movs	r1, r0
    8650: 513a         	str	r2, [r7, r4]
    8652: 0000         	movs	r0, r0

00008654 <ConnectionManager::setInputVolume(LSMessage&)>:
    8654: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    8658: b0b5         	sub	sp, #0xd4
    865a: f8df a62c    	ldr.w	r10, [pc, #0x62c]       @ 0x8c88 <ConnectionManager::setInputVolume(LSMessage&)+0x634> literal=0x1874e
    865e: 4680         	mov	r8, r0
    8660: af10         	add	r7, sp, #0x40
    8662: 4608         	mov	r0, r1
    8664: 460c         	mov	r4, r1
    8666: 910e         	str	r1, [sp, #0x38]
    8668: f7fb ed3c    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x4588
    866c: 4638         	mov	r0, r7
    866e: 44fa         	add	r10, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    8670: f7fb edd4    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x4458
    8674: ab18         	add	r3, sp, #0x60
    8676: f10d 0988    	add.w	r9, sp, #0x88
    867a: 930a         	str	r3, [sp, #0x28]
    867c: ad2e         	add	r5, sp, #0xb8
    867e: 9316         	str	r3, [sp, #0x58]
    8680: ab1e         	add	r3, sp, #0x78
    8682: 930b         	str	r3, [sp, #0x2c]
    8684: 2200         	movs	r2, #0x0
    8686: 931c         	str	r3, [sp, #0x70]
    8688: ab24         	add	r3, sp, #0x90
    868a: 930c         	str	r3, [sp, #0x30]
    868c: 4629         	mov	r1, r5
    868e: 9322         	str	r3, [sp, #0x88]
    8690: 4648         	mov	r0, r9
    8692: 23b3         	movs	r3, #0xb3
    8694: 920f         	str	r2, [sp, #0x3c]
    8696: 9217         	str	r2, [sp, #0x5c]
    8698: f88d 2060    	strb.w	r2, [sp, #0x60]
    869c: 921d         	str	r2, [sp, #0x74]
    869e: f88d 2078    	strb.w	r2, [sp, #0x78]
    86a2: 932e         	str	r3, [sp, #0xb8]
    86a4: f7fb ece6    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x4634
    86a8: f8df 15e0    	ldr.w	r1, [pc, #0x5e0]        @ 0x8c8c <ConnectionManager::setInputVolume(LSMessage&)+0x638> literal=0x5b7a
    86ac: 22b3         	movs	r2, #0xb3
    86ae: 9b2e         	ldr	r3, [sp, #0xb8]
    86b0: 9022         	str	r0, [sp, #0x88]
    86b2: 4479         	add	r1, pc => raw=0xe230 ghidra=0x1e230 file=57904 "{\"type\":\"object\",\"properties\":{\"source\":{\"type\":\"string\"},\"sink\":{\"type\":\"string\"},\"volume\":{\"type\":\"integer\"}},\"required\":[\"source\",\"sink\",\"volume\"],\"additionalProperties\":false}"
    86b4: 9324         	str	r3, [sp, #0x90]
    86b6: f7fb ec68    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x4730
    86ba: 9b2e         	ldr	r3, [sp, #0xb8]
    86bc: 2100         	movs	r1, #0x0
    86be: 9a22         	ldr	r2, [sp, #0x88]
    86c0: 4620         	mov	r0, r4
    86c2: 9323         	str	r3, [sp, #0x8c]
    86c4: 54d1         	strb	r1, [r2, r3]
    86c6: f7fb ed68    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x4530
    86ca: ac30         	add	r4, sp, #0xc0
    86cc: 4683         	mov	r11, r0
    86ce: 942e         	str	r4, [sp, #0xb8]
    86d0: 2800         	cmp	r0, #0x0
    86d2: f000 8275    	beq.w	0x8bc0 <ConnectionManager::setInputVolume(LSMessage&)+0x56c> @ imm = #0x4ea
    86d6: f7fb ece6    	blx	0x40a4 <strlen@plt>     @ imm = #-0x4634
    86da: 280f         	cmp	r0, #0xf
    86dc: 4606         	mov	r6, r0
    86de: 9028         	str	r0, [sp, #0xa0]
    86e0: f200 81b3    	bhi.w	0x8a4a <ConnectionManager::setInputVolume(LSMessage&)+0x3f6> @ imm = #0x366
    86e4: 2801         	cmp	r0, #0x1
    86e6: f040 817f    	bne.w	0x89e8 <ConnectionManager::setInputVolume(LSMessage&)+0x394> @ imm = #0x2fe
    86ea: f89b 3000    	ldrb.w	r3, [r11]
    86ee: f88d 30c0    	strb.w	r3, [sp, #0xc0]
    86f2: 4623         	mov	r3, r4
    86f4: 2200         	movs	r2, #0x0
    86f6: 962f         	str	r6, [sp, #0xbc]
    86f8: 4639         	mov	r1, r7
    86fa: 559a         	strb	r2, [r3, r6]
    86fc: 4628         	mov	r0, r5
    86fe: ab0f         	add	r3, sp, #0x3c
    8700: 464a         	mov	r2, r9
    8702: f001 fe69    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #0x1cd2
    8706: 4606         	mov	r6, r0
    8708: 982e         	ldr	r0, [sp, #0xb8]
    870a: 42a0         	cmp	r0, r4
    870c: d003         	beq	0x8716 <ConnectionManager::setInputVolume(LSMessage&)+0xc2> @ imm = #0x6
    870e: 9930         	ldr	r1, [sp, #0xc0]
    8710: 3101         	adds	r1, #0x1
    8712: f7fb ebf6    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4814
    8716: 2e00         	cmp	r6, #0x0
    8718: f000 816c    	beq.w	0x89f4 <ConnectionManager::setInputVolume(LSMessage&)+0x3a0> @ imm = #0x2d8
    871c: f8df 2570    	ldr.w	r2, [pc, #0x570]        @ 0x8c90 <ConnectionManager::setInputVolume(LSMessage&)+0x63c> literal=0x53cc
    8720: 4639         	mov	r1, r7
    8722: 4628         	mov	r0, r5
    8724: 447a         	add	r2, pc => raw=0xdaf4 ghidra=0x1daf4 file=56052 "source"
    8726: f7fb ebf2    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x481c
    872a: ae28         	add	r6, sp, #0xa0
    872c: 2300         	movs	r3, #0x0
    872e: f10d 0ba8    	add.w	r11, sp, #0xa8
    8732: 4631         	mov	r1, r6
    8734: 4628         	mov	r0, r5
    8736: 9329         	str	r3, [sp, #0xa4]
    8738: f88d 30a8    	strb.w	r3, [sp, #0xa8]
    873c: f8cd b0a0    	str.w	r11, [sp, #0xa0]
    8740: f7fb eb2e    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x49a4
    8744: 9b28         	ldr	r3, [sp, #0xa0]
    8746: 9816         	ldr	r0, [sp, #0x58]
    8748: 9a29         	ldr	r2, [sp, #0xa4]
    874a: 455b         	cmp	r3, r11
    874c: f000 8215    	beq.w	0x8b7a <ConnectionManager::setInputVolume(LSMessage&)+0x526> @ imm = #0x42a
    8750: 990a         	ldr	r1, [sp, #0x28]
    8752: f8dd c0a8    	ldr.w	r12, [sp, #0xa8]
    8756: e9cd 3216    	strd	r3, r2, [sp, #88]
    875a: 4288         	cmp	r0, r1
    875c: bf08         	it	eq
    875e: f8cd c060    	streq.w	r12, [sp, #0x60]
    8762: f000 8217    	beq.w	0x8b94 <ConnectionManager::setInputVolume(LSMessage&)+0x540> @ imm = #0x42e
    8766: 9b18         	ldr	r3, [sp, #0x60]
    8768: f8cd c060    	str.w	r12, [sp, #0x60]
    876c: 2800         	cmp	r0, #0x0
    876e: f000 8211    	beq.w	0x8b94 <ConnectionManager::setInputVolume(LSMessage&)+0x540> @ imm = #0x422
    8772: 932a         	str	r3, [sp, #0xa8]
    8774: 9028         	str	r0, [sp, #0xa0]
    8776: 2300         	movs	r3, #0x0
    8778: 9329         	str	r3, [sp, #0xa4]
    877a: 7003         	strb	r3, [r0]
    877c: 9828         	ldr	r0, [sp, #0xa0]
    877e: 4558         	cmp	r0, r11
    8780: d003         	beq	0x878a <ConnectionManager::setInputVolume(LSMessage&)+0x136> @ imm = #0x6
    8782: 992a         	ldr	r1, [sp, #0xa8]
    8784: 3101         	adds	r1, #0x1
    8786: f7fb ebbc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4888
    878a: f8df 3508    	ldr.w	r3, [pc, #0x508]        @ 0x8c94 <ConnectionManager::setInputVolume(LSMessage&)+0x640> literal=0x210
    878e: 4628         	mov	r0, r5
    8790: f85a b003    	ldr.w	r11, [r10, r3]
    8794: f10b 0308    	add.w	r3, r11, #0x8
    8798: 932e         	str	r3, [sp, #0xb8]
    879a: f7fb ebd8    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x4850
    879e: f8df 24f8    	ldr.w	r2, [pc, #0x4f8]        @ 0x8c98 <ConnectionManager::setInputVolume(LSMessage&)+0x644> literal=0x56be
    87a2: 4639         	mov	r1, r7
    87a4: 4630         	mov	r0, r6
    87a6: 447a         	add	r2, pc => raw=0xde68 ghidra=0x1de68 file=56936 "sink"
    87a8: f7fb ebb0    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x48a0
    87ac: 2300         	movs	r3, #0x0
    87ae: 4629         	mov	r1, r5
    87b0: 4630         	mov	r0, r6
    87b2: 942e         	str	r4, [sp, #0xb8]
    87b4: 932f         	str	r3, [sp, #0xbc]
    87b6: f88d 30c0    	strb.w	r3, [sp, #0xc0]
    87ba: f7fb eaf2    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x4a1c
    87be: 9b2e         	ldr	r3, [sp, #0xb8]
    87c0: 981c         	ldr	r0, [sp, #0x70]
    87c2: 9a2f         	ldr	r2, [sp, #0xbc]
    87c4: 42a3         	cmp	r3, r4
    87c6: f000 81cb    	beq.w	0x8b60 <ConnectionManager::setInputVolume(LSMessage&)+0x50c> @ imm = #0x396
    87ca: 990b         	ldr	r1, [sp, #0x2c]
    87cc: f8dd c0c0    	ldr.w	r12, [sp, #0xc0]
    87d0: e9cd 321c    	strd	r3, r2, [sp, #112]
    87d4: 4288         	cmp	r0, r1
    87d6: bf08         	it	eq
    87d8: f8cd c078    	streq.w	r12, [sp, #0x78]
    87dc: f000 81e0    	beq.w	0x8ba0 <ConnectionManager::setInputVolume(LSMessage&)+0x54c> @ imm = #0x3c0
    87e0: 9b1e         	ldr	r3, [sp, #0x78]
    87e2: f8cd c078    	str.w	r12, [sp, #0x78]
    87e6: 2800         	cmp	r0, #0x0
    87e8: f000 81da    	beq.w	0x8ba0 <ConnectionManager::setInputVolume(LSMessage&)+0x54c> @ imm = #0x3b4
    87ec: 9330         	str	r3, [sp, #0xc0]
    87ee: 902e         	str	r0, [sp, #0xb8]
    87f0: 2300         	movs	r3, #0x0
    87f2: 932f         	str	r3, [sp, #0xbc]
    87f4: 7003         	strb	r3, [r0]
    87f6: 982e         	ldr	r0, [sp, #0xb8]
    87f8: 42a0         	cmp	r0, r4
    87fa: d003         	beq	0x8804 <ConnectionManager::setInputVolume(LSMessage&)+0x1b0> @ imm = #0x6
    87fc: 9930         	ldr	r1, [sp, #0xc0]
    87fe: 3101         	adds	r1, #0x1
    8800: f7fb eb7e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4904
    8804: f10b 0308    	add.w	r3, r11, #0x8
    8808: 4630         	mov	r0, r6
    880a: 9328         	str	r3, [sp, #0xa0]
    880c: f7fb eb9e    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x48c4
    8810: f8df 2488    	ldr.w	r2, [pc, #0x488]        @ 0x8c9c <ConnectionManager::setInputVolume(LSMessage&)+0x648> literal=0x5ac8
    8814: 4639         	mov	r1, r7
    8816: 4628         	mov	r0, r5
    8818: 447a         	add	r2, pc => raw=0xe2e4 ghidra=0x1e2e4 file=58084 "volume"
    881a: f7fb eb78    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x4910
    881e: 4628         	mov	r0, r5
    8820: f7fb ecc6    	blx	0x41b0 <_ZNK7pbnjson6JValue8asNumberIiEET_v@plt> @ imm = #-0x4674
    8824: f10b 0308    	add.w	r3, r11, #0x8
    8828: 4682         	mov	r10, r0
    882a: 4628         	mov	r0, r5
    882c: fa4f fb8a    	sxtb.w	r11, r10
    8830: 932e         	str	r3, [sp, #0xb8]
    8832: f7fb eb8c    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x48e8
    8836: f001 fe81    	bl	0xa53c <getPmLogContext()> @ imm = #0x1d02
    883a: b120         	cbz	r0, 0x8846 <ConnectionManager::setInputVolume(LSMessage&)+0x1f2> @ imm = #0x8
    883c: f001 fe7e    	bl	0xa53c <getPmLogContext()> @ imm = #0x1cfc
    8840: 6803         	ldr	r3, [r0]
    8842: 2b06         	cmp	r3, #0x6
    8844: dd1b         	ble	0x887e <ConnectionManager::setInputVolume(LSMessage&)+0x22a> @ imm = #0x36
    8846: f001 fe79    	bl	0xa53c <getPmLogContext()> @ imm = #0x1cf2
    884a: 9b1c         	ldr	r3, [sp, #0x70]
    884c: 2107         	movs	r1, #0x7
    884e: f8cd b020    	str.w	r11, [sp, #0x20]
    8852: 9307         	str	r3, [sp, #0x1c]
    8854: 9b16         	ldr	r3, [sp, #0x58]
    8856: 9306         	str	r3, [sp, #0x18]
    8858: f8df 3444    	ldr.w	r3, [pc, #0x444]        @ 0x8ca0 <ConnectionManager::setInputVolume(LSMessage&)+0x64c> literal=0x5a8c
    885c: 447b         	add	r3, pc => raw=0xe2ec ghidra=0x1e2ec file=58092 "setInputVolume"
    885e: 9305         	str	r3, [sp, #0x14]
    8860: f8df 3440    	ldr.w	r3, [pc, #0x440]        @ 0x8ca4 <ConnectionManager::setInputVolume(LSMessage&)+0x650> literal=0x4ee8
    8864: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    8866: 9304         	str	r3, [sp, #0x10]
    8868: f8df 343c    	ldr.w	r3, [pc, #0x43c]        @ 0x8ca8 <ConnectionManager::setInputVolume(LSMessage&)+0x654> literal=0x5a8c
    886c: 447b         	add	r3, pc => raw=0xe2fc ghidra=0x1e2fc file=58108 "%s:%s() Audio setInputVolume called for source %s and sink %s with volume %d"
    886e: 9303         	str	r3, [sp, #0xc]
    8870: 2300         	movs	r3, #0x0
    8872: 461a         	mov	r2, r3
    8874: e9cd 3301    	strd	r3, r3, [sp, #4]
    8878: 9300         	str	r3, [sp]
    887a: f7fb ebd0    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x4860
    887e: fa5f fa8a    	uxtb.w	r10, r10
    8882: f1ba 0f64    	cmp.w	r10, #0x64
    8886: f200 80ef    	bhi.w	0x8a68 <ConnectionManager::setInputVolume(LSMessage&)+0x414> @ imm = #0x1de
    888a: f10d 0a4c    	add.w	r10, sp, #0x4c
    888e: 4650         	mov	r0, r10
    8890: f7fb ea92    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x4adc
    8894: ab16         	add	r3, sp, #0x58
    8896: 4640         	mov	r0, r8
    8898: f10d 0870    	add.w	r8, sp, #0x70
    889c: 4619         	mov	r1, r3
    889e: 930d         	str	r3, [sp, #0x34]
    88a0: 4642         	mov	r2, r8
    88a2: 462b         	mov	r3, r5
    88a4: f8cd b0b8    	str.w	r11, [sp, #0xb8]
    88a8: f7fd f856    	bl	0x5958 <ConnectionManager::setInputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)> @ imm = #-0x2f54
    88ac: 2800         	cmp	r0, #0x0
    88ae: f040 80e4    	bne.w	0x8a7a <ConnectionManager::setInputVolume(LSMessage&)+0x426> @ imm = #0x1c8
    88b2: f001 fe43    	bl	0xa53c <getPmLogContext()> @ imm = #0x1c86
    88b6: b120         	cbz	r0, 0x88c2 <ConnectionManager::setInputVolume(LSMessage&)+0x26e> @ imm = #0x8
    88b8: f001 fe40    	bl	0xa53c <getPmLogContext()> @ imm = #0x1c80
    88bc: 6803         	ldr	r3, [r0]
    88be: 2b06         	cmp	r3, #0x6
    88c0: dd18         	ble	0x88f4 <ConnectionManager::setInputVolume(LSMessage&)+0x2a0> @ imm = #0x30
    88c2: f001 fe3b    	bl	0xa53c <getPmLogContext()> @ imm = #0x1c76
    88c6: 9b1c         	ldr	r3, [sp, #0x70]
    88c8: 2107         	movs	r1, #0x7
    88ca: f8cd b020    	str.w	r11, [sp, #0x20]
    88ce: 9307         	str	r3, [sp, #0x1c]
    88d0: 9b16         	ldr	r3, [sp, #0x58]
    88d2: 9306         	str	r3, [sp, #0x18]
    88d4: 4bf5         	ldr	r3, [pc, #0x3d4]        @ 0x8cac <ConnectionManager::setInputVolume(LSMessage&)+0x658> literal=0x5a12
    88d6: 447b         	add	r3, pc => raw=0xe2ec ghidra=0x1e2ec file=58092 "setInputVolume"
    88d8: 9305         	str	r3, [sp, #0x14]
    88da: 4bf5         	ldr	r3, [pc, #0x3d4]        @ 0x8cb0 <ConnectionManager::setInputVolume(LSMessage&)+0x65c> literal=0x4e70
    88dc: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    88de: 9304         	str	r3, [sp, #0x10]
    88e0: 4bf4         	ldr	r3, [pc, #0x3d0]        @ 0x8cb4 <ConnectionManager::setInputVolume(LSMessage&)+0x660> literal=0x5a66
    88e2: 447b         	add	r3, pc => raw=0xe34c ghidra=0x1e34c file=58188 "%s:%s() Audio setInputVolume is success for source %s and sink %s with volume %d"
    88e4: 9303         	str	r3, [sp, #0xc]
    88e6: 2300         	movs	r3, #0x0
    88e8: 461a         	mov	r2, r3
    88ea: e9cd 3301    	strd	r3, r3, [sp, #4]
    88ee: 9300         	str	r3, [sp]
    88f0: f7fb eb94    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x48d8
    88f4: 2101         	movs	r1, #0x1
    88f6: 4630         	mov	r0, r6
    88f8: f7fb ea7c    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x4b08
    88fc: 4aee         	ldr	r2, [pc, #0x3b8]        @ 0x8cb8 <ConnectionManager::setInputVolume(LSMessage&)+0x664> literal=0x4e16
    88fe: 4623         	mov	r3, r4
    8900: 942e         	str	r4, [sp, #0xb8]
    8902: f04f 0e0b    	mov.w	lr, #0xb
    8906: f04f 0c00    	mov.w	r12, #0x0
    890a: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    890c: ca07         	ldm	r2, {r0, r1, r2}
    890e: c303         	stm	r3!, {r0, r1}
    8910: 4629         	mov	r1, r5
    8912: f823 2b02    	strh	r2, [r3], #2
    8916: 0c12         	lsrs	r2, r2, #0x10
    8918: 4650         	mov	r0, r10
    891a: 701a         	strb	r2, [r3]
    891c: 4632         	mov	r2, r6
    891e: f8cd e0bc    	str.w	lr, [sp, #0xbc]
    8922: f88d c0cb    	strb.w	r12, [sp, #0xcb]
    8926: f7fb e9d6    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4c54
    892a: 982e         	ldr	r0, [sp, #0xb8]
    892c: 42a0         	cmp	r0, r4
    892e: d003         	beq	0x8938 <ConnectionManager::setInputVolume(LSMessage&)+0x2e4> @ imm = #0x6
    8930: 9930         	ldr	r1, [sp, #0xc0]
    8932: 3101         	adds	r1, #0x1
    8934: f7fb eae4    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4a38
    8938: 4630         	mov	r0, r6
    893a: f7fb ea0e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4be4
    893e: 990d         	ldr	r1, [sp, #0x34]
    8940: 4630         	mov	r0, r6
    8942: f7fb ebf4    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x4818
    8946: 4bdd         	ldr	r3, [pc, #0x374]        @ 0x8cbc <ConnectionManager::setInputVolume(LSMessage&)+0x668> literal=0x519c
    8948: 2206         	movs	r2, #0x6
    894a: 922f         	str	r2, [sp, #0xbc]
    894c: 2200         	movs	r2, #0x0
    894e: f88d 20c6    	strb.w	r2, [sp, #0xc6]
    8952: 4632         	mov	r2, r6
    8954: 447b         	add	r3, pc => raw=0xdaf4 ghidra=0x1daf4 file=56052 "source"
    8956: 942e         	str	r4, [sp, #0xb8]
    8958: e893 0003    	ldm.w	r3, {r0, r1}
    895c: 9030         	str	r0, [sp, #0xc0]
    895e: 4650         	mov	r0, r10
    8960: f8ad 10c4    	strh.w	r1, [sp, #0xc4]
    8964: 4629         	mov	r1, r5
    8966: f7fb e9b6    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4c94
    896a: 982e         	ldr	r0, [sp, #0xb8]
    896c: 42a0         	cmp	r0, r4
    896e: d003         	beq	0x8978 <ConnectionManager::setInputVolume(LSMessage&)+0x324> @ imm = #0x6
    8970: 9930         	ldr	r1, [sp, #0xc0]
    8972: 3101         	adds	r1, #0x1
    8974: f7fb eac4    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4a78
    8978: 4630         	mov	r0, r6
    897a: f7fb e9ee    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4c24
    897e: 4641         	mov	r1, r8
    8980: 4630         	mov	r0, r6
    8982: f7fb ebd4    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x4858
    8986: f646 1373    	movw	r3, #0x6973
    898a: 4632         	mov	r2, r6
    898c: f6c6 336e    	movt	r3, #0x6b6e
    8990: 4629         	mov	r1, r5
    8992: 9330         	str	r3, [sp, #0xc0]
    8994: 2304         	movs	r3, #0x4
    8996: 932f         	str	r3, [sp, #0xbc]
    8998: 4650         	mov	r0, r10
    899a: 2300         	movs	r3, #0x0
    899c: 942e         	str	r4, [sp, #0xb8]
    899e: f88d 30c4    	strb.w	r3, [sp, #0xc4]
    89a2: f7fb e998    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4cd0
    89a6: 982e         	ldr	r0, [sp, #0xb8]
    89a8: 42a0         	cmp	r0, r4
    89aa: d003         	beq	0x89b4 <ConnectionManager::setInputVolume(LSMessage&)+0x360> @ imm = #0x6
    89ac: 9930         	ldr	r1, [sp, #0xc0]
    89ae: 3101         	adds	r1, #0x1
    89b0: f7fb eaa6    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4ab4
    89b4: 4630         	mov	r0, r6
    89b6: f7fb e9d0    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4c60
    89ba: 4659         	mov	r1, r11
    89bc: 4630         	mov	r0, r6
    89be: f7fb ebbc    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x4888
    89c2: 4bbf         	ldr	r3, [pc, #0x2fc]        @ 0x8cc0 <ConnectionManager::setInputVolume(LSMessage&)+0x66c> literal=0x5910
    89c4: 2206         	movs	r2, #0x6
    89c6: 922f         	str	r2, [sp, #0xbc]
    89c8: 2200         	movs	r2, #0x0
    89ca: f88d 20c6    	strb.w	r2, [sp, #0xc6]
    89ce: 4632         	mov	r2, r6
    89d0: 447b         	add	r3, pc => raw=0xe2e4 ghidra=0x1e2e4 file=58084 "volume"
    89d2: 942e         	str	r4, [sp, #0xb8]
    89d4: e893 0003    	ldm.w	r3, {r0, r1}
    89d8: 9030         	str	r0, [sp, #0xc0]
    89da: 4650         	mov	r0, r10
    89dc: f8ad 10c4    	strh.w	r1, [sp, #0xc4]
    89e0: 4629         	mov	r1, r5
    89e2: f7fb e978    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4d10
    89e6: e0a9         	b	0x8b3c <ConnectionManager::setInputVolume(LSMessage&)+0x4e8> @ imm = #0x152
    89e8: 4623         	mov	r3, r4
    89ea: 2800         	cmp	r0, #0x0
    89ec: f43f ae82    	beq.w	0x86f4 <ConnectionManager::setInputVolume(LSMessage&)+0xa0> @ imm = #-0x2fc
    89f0: 4620         	mov	r0, r4
    89f2: e032         	b	0x8a5a <ConnectionManager::setInputVolume(LSMessage&)+0x406> @ imm = #0x64
    89f4: 49b3         	ldr	r1, [pc, #0x2cc]        @ 0x8cc4 <ConnectionManager::setInputVolume(LSMessage&)+0x670> literal=0x18644
    89f6: 4633         	mov	r3, r6
    89f8: 2203         	movs	r2, #0x3
    89fa: a80e         	add	r0, sp, #0x38
    89fc: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    89fe: 311c         	adds	r1, #0x1c
    8a00: f001 f924    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0x1248
    8a04: 9822         	ldr	r0, [sp, #0x88]
    8a06: 9b0c         	ldr	r3, [sp, #0x30]
    8a08: 4298         	cmp	r0, r3
    8a0a: d003         	beq	0x8a14 <ConnectionManager::setInputVolume(LSMessage&)+0x3c0> @ imm = #0x6
    8a0c: 9924         	ldr	r1, [sp, #0x90]
    8a0e: 3101         	adds	r1, #0x1
    8a10: f7fb ea76    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4b14
    8a14: 981c         	ldr	r0, [sp, #0x70]
    8a16: 9b0b         	ldr	r3, [sp, #0x2c]
    8a18: 4298         	cmp	r0, r3
    8a1a: d003         	beq	0x8a24 <ConnectionManager::setInputVolume(LSMessage&)+0x3d0> @ imm = #0x6
    8a1c: 991e         	ldr	r1, [sp, #0x78]
    8a1e: 3101         	adds	r1, #0x1
    8a20: f7fb ea6e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4b24
    8a24: 9816         	ldr	r0, [sp, #0x58]
    8a26: 9b0a         	ldr	r3, [sp, #0x28]
    8a28: 4298         	cmp	r0, r3
    8a2a: d003         	beq	0x8a34 <ConnectionManager::setInputVolume(LSMessage&)+0x3e0> @ imm = #0x6
    8a2c: 9918         	ldr	r1, [sp, #0x60]
    8a2e: 3101         	adds	r1, #0x1
    8a30: f7fb ea66    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4b34
    8a34: 4638         	mov	r0, r7
    8a36: f7fb e990    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4ce0
    8a3a: 980e         	ldr	r0, [sp, #0x38]
    8a3c: b108         	cbz	r0, 0x8a42 <ConnectionManager::setInputVolume(LSMessage&)+0x3ee> @ imm = #0x2
    8a3e: f7fb eb06    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x49f4
    8a42: 2001         	movs	r0, #0x1
    8a44: b035         	add	sp, #0xd4
    8a46: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    8a4a: 2200         	movs	r2, #0x0
    8a4c: a928         	add	r1, sp, #0xa0
    8a4e: 4628         	mov	r0, r5
    8a50: f7fb eb10    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x49e0
    8a54: 9b28         	ldr	r3, [sp, #0xa0]
    8a56: 902e         	str	r0, [sp, #0xb8]
    8a58: 9330         	str	r3, [sp, #0xc0]
    8a5a: 4632         	mov	r2, r6
    8a5c: 4659         	mov	r1, r11
    8a5e: f7fb ea94    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x4ad8
    8a62: 9e28         	ldr	r6, [sp, #0xa0]
    8a64: 9b2e         	ldr	r3, [sp, #0xb8]
    8a66: e645         	b	0x86f4 <ConnectionManager::setInputVolume(LSMessage&)+0xa0> @ imm = #-0x376
    8a68: 4997         	ldr	r1, [pc, #0x25c]        @ 0x8cc8 <ConnectionManager::setInputVolume(LSMessage&)+0x674> literal=0x185d0
    8a6a: 2300         	movs	r3, #0x0
    8a6c: 22ca         	movs	r2, #0xca
    8a6e: a80e         	add	r0, sp, #0x38
    8a70: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    8a72: 31ac         	adds	r1, #0xac
    8a74: f001 f8ea    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0x11d4
    8a78: e7c4         	b	0x8a04 <ConnectionManager::setInputVolume(LSMessage&)+0x3b0> @ imm = #-0x78
    8a7a: 2100         	movs	r1, #0x0
    8a7c: 4630         	mov	r0, r6
    8a7e: f7fb e9ba    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x4c8c
    8a82: 4a92         	ldr	r2, [pc, #0x248]        @ 0x8ccc <ConnectionManager::setInputVolume(LSMessage&)+0x678> literal=0x4c90
    8a84: 4623         	mov	r3, r4
    8a86: 942e         	str	r4, [sp, #0xb8]
    8a88: f04f 0e0b    	mov.w	lr, #0xb
    8a8c: f04f 0c00    	mov.w	r12, #0x0
    8a90: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    8a92: ca07         	ldm	r2, {r0, r1, r2}
    8a94: c303         	stm	r3!, {r0, r1}
    8a96: 4629         	mov	r1, r5
    8a98: f823 2b02    	strh	r2, [r3], #2
    8a9c: 0c12         	lsrs	r2, r2, #0x10
    8a9e: 4650         	mov	r0, r10
    8aa0: 701a         	strb	r2, [r3]
    8aa2: 4632         	mov	r2, r6
    8aa4: f8cd e0bc    	str.w	lr, [sp, #0xbc]
    8aa8: f88d c0cb    	strb.w	r12, [sp, #0xcb]
    8aac: f7fb e912    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4ddc
    8ab0: 982e         	ldr	r0, [sp, #0xb8]
    8ab2: 42a0         	cmp	r0, r4
    8ab4: d003         	beq	0x8abe <ConnectionManager::setInputVolume(LSMessage&)+0x46a> @ imm = #0x6
    8ab6: 9930         	ldr	r1, [sp, #0xc0]
    8ab8: 3101         	adds	r1, #0x1
    8aba: f7fb ea22    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4bbc
    8abe: 4630         	mov	r0, r6
    8ac0: f7fb e94a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4d6c
    8ac4: 4982         	ldr	r1, [pc, #0x208]        @ 0x8cd0 <ConnectionManager::setInputVolume(LSMessage&)+0x67c> literal=0x18578
    8ac6: 4630         	mov	r0, r6
    8ac8: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    8aca: 3164         	adds	r1, #0x64
    8acc: f7fb eb2e    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x49a4
    8ad0: 4a80         	ldr	r2, [pc, #0x200]        @ 0x8cd4 <ConnectionManager::setInputVolume(LSMessage&)+0x680> literal=0x5b7a
    8ad2: 4623         	mov	r3, r4
    8ad4: 942e         	str	r4, [sp, #0xb8]
    8ad6: f04f 0e09    	mov.w	lr, #0x9
    8ada: f04f 0c00    	mov.w	r12, #0x0
    8ade: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    8ae0: ca07         	ldm	r2, {r0, r1, r2}
    8ae2: c303         	stm	r3!, {r0, r1}
    8ae4: 4629         	mov	r1, r5
    8ae6: 701a         	strb	r2, [r3]
    8ae8: 4650         	mov	r0, r10
    8aea: 4632         	mov	r2, r6
    8aec: f8cd e0bc    	str.w	lr, [sp, #0xbc]
    8af0: f88d c0c9    	strb.w	r12, [sp, #0xc9]
    8af4: f7fb e8ee    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4e24
    8af8: 982e         	ldr	r0, [sp, #0xb8]
    8afa: 42a0         	cmp	r0, r4
    8afc: d003         	beq	0x8b06 <ConnectionManager::setInputVolume(LSMessage&)+0x4b2> @ imm = #0x6
    8afe: 9930         	ldr	r1, [sp, #0xc0]
    8b00: 3101         	adds	r1, #0x1
    8b02: f7fb e9fe    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4c04
    8b06: 4630         	mov	r0, r6
    8b08: f7fb e926    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4db4
    8b0c: 2114         	movs	r1, #0x14
    8b0e: 4630         	mov	r0, r6
    8b10: f7fb eb12    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x49dc
    8b14: 4a70         	ldr	r2, [pc, #0x1c0]        @ 0x8cd8 <ConnectionManager::setInputVolume(LSMessage&)+0x684> literal=0x5b42
    8b16: 4623         	mov	r3, r4
    8b18: 942e         	str	r4, [sp, #0xb8]
    8b1a: f04f 0e09    	mov.w	lr, #0x9
    8b1e: f04f 0c00    	mov.w	r12, #0x0
    8b22: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    8b24: ca07         	ldm	r2, {r0, r1, r2}
    8b26: c303         	stm	r3!, {r0, r1}
    8b28: 4629         	mov	r1, r5
    8b2a: 701a         	strb	r2, [r3]
    8b2c: 4650         	mov	r0, r10
    8b2e: 4632         	mov	r2, r6
    8b30: f8cd e0bc    	str.w	lr, [sp, #0xbc]
    8b34: f88d c0c9    	strb.w	r12, [sp, #0xc9]
    8b38: f7fb e8cc    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x4e68
    8b3c: 982e         	ldr	r0, [sp, #0xb8]
    8b3e: 42a0         	cmp	r0, r4
    8b40: d003         	beq	0x8b4a <ConnectionManager::setInputVolume(LSMessage&)+0x4f6> @ imm = #0x6
    8b42: 9930         	ldr	r1, [sp, #0xc0]
    8b44: 3101         	adds	r1, #0x1
    8b46: f7fb e9dc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4c48
    8b4a: 4630         	mov	r0, r6
    8b4c: f7fb e904    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4df8
    8b50: 4651         	mov	r1, r10
    8b52: a80e         	add	r0, sp, #0x38
    8b54: f001 f99c    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #0x1338
    8b58: 4650         	mov	r0, r10
    8b5a: f7fb e8fe    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4e04
    8b5e: e751         	b	0x8a04 <ConnectionManager::setInputVolume(LSMessage&)+0x3b0> @ imm = #-0x15e
    8b60: b132         	cbz	r2, 0x8b70 <ConnectionManager::setInputVolume(LSMessage&)+0x51c> @ imm = #0xc
    8b62: 2a01         	cmp	r2, #0x1
    8b64: d020         	beq	0x8ba8 <ConnectionManager::setInputVolume(LSMessage&)+0x554> @ imm = #0x40
    8b66: 4621         	mov	r1, r4
    8b68: f7fb ea0e    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x4be4
    8b6c: 9a2f         	ldr	r2, [sp, #0xbc]
    8b6e: 981c         	ldr	r0, [sp, #0x70]
    8b70: 2300         	movs	r3, #0x0
    8b72: 921d         	str	r2, [sp, #0x74]
    8b74: 5483         	strb	r3, [r0, r2]
    8b76: 982e         	ldr	r0, [sp, #0xb8]
    8b78: e63a         	b	0x87f0 <ConnectionManager::setInputVolume(LSMessage&)+0x19c> @ imm = #-0x38c
    8b7a: b132         	cbz	r2, 0x8b8a <ConnectionManager::setInputVolume(LSMessage&)+0x536> @ imm = #0xc
    8b7c: 2a01         	cmp	r2, #0x1
    8b7e: d019         	beq	0x8bb4 <ConnectionManager::setInputVolume(LSMessage&)+0x560> @ imm = #0x32
    8b80: 4659         	mov	r1, r11
    8b82: f7fb ea02    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x4bfc
    8b86: 9a29         	ldr	r2, [sp, #0xa4]
    8b88: 9816         	ldr	r0, [sp, #0x58]
    8b8a: 2300         	movs	r3, #0x0
    8b8c: 9217         	str	r2, [sp, #0x5c]
    8b8e: 5483         	strb	r3, [r0, r2]
    8b90: 9828         	ldr	r0, [sp, #0xa0]
    8b92: e5f0         	b	0x8776 <ConnectionManager::setInputVolume(LSMessage&)+0x122> @ imm = #-0x420
    8b94: f8cd b0a0    	str.w	r11, [sp, #0xa0]
    8b98: f10d 0ba8    	add.w	r11, sp, #0xa8
    8b9c: 4658         	mov	r0, r11
    8b9e: e5ea         	b	0x8776 <ConnectionManager::setInputVolume(LSMessage&)+0x122> @ imm = #-0x42c
    8ba0: 942e         	str	r4, [sp, #0xb8]
    8ba2: ac30         	add	r4, sp, #0xc0
    8ba4: 4620         	mov	r0, r4
    8ba6: e623         	b	0x87f0 <ConnectionManager::setInputVolume(LSMessage&)+0x19c> @ imm = #-0x3ba
    8ba8: f89d 30c0    	ldrb.w	r3, [sp, #0xc0]
    8bac: 7003         	strb	r3, [r0]
    8bae: 9a2f         	ldr	r2, [sp, #0xbc]
    8bb0: 981c         	ldr	r0, [sp, #0x70]
    8bb2: e7dd         	b	0x8b70 <ConnectionManager::setInputVolume(LSMessage&)+0x51c> @ imm = #-0x46
    8bb4: f89d 30a8    	ldrb.w	r3, [sp, #0xa8]
    8bb8: 7003         	strb	r3, [r0]
    8bba: 9a29         	ldr	r2, [sp, #0xa4]
    8bbc: 9816         	ldr	r0, [sp, #0x58]
    8bbe: e7e4         	b	0x8b8a <ConnectionManager::setInputVolume(LSMessage&)+0x536> @ imm = #-0x38
    8bc0: 4846         	ldr	r0, [pc, #0x118]        @ 0x8cdc <ConnectionManager::setInputVolume(LSMessage&)+0x688> literal=0x4ad2
    8bc2: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    8bc4: f7fb e89e    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x4ec4
    8bc8: 4628         	mov	r0, r5
    8bca: f7fb ea92    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4adc
    8bce: 4630         	mov	r0, r6
    8bd0: f7fb e8c2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4e7c
    8bd4: 4650         	mov	r0, r10
    8bd6: f7fb e8c0    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4e80
    8bda: 4648         	mov	r0, r9
    8bdc: f7fb ea88    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4af0
    8be0: 4640         	mov	r0, r8
    8be2: f7fb ea86    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4af4
    8be6: 980d         	ldr	r0, [sp, #0x34]
    8be8: f7fb ea82    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4afc
    8bec: 4638         	mov	r0, r7
    8bee: f7fb e8b4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x4e98
    8bf2: 980e         	ldr	r0, [sp, #0x38]
    8bf4: b108         	cbz	r0, 0x8bfa <ConnectionManager::setInputVolume(LSMessage&)+0x5a6> @ imm = #0x2
    8bf6: f7fb ea2a    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x4bac
    8bfa: f7fb eac2    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x4a7c
    8bfe: e7e3         	b	0x8bc8 <ConnectionManager::setInputVolume(LSMessage&)+0x574> @ imm = #-0x3a
    8c00: e7e2         	b	0x8bc8 <ConnectionManager::setInputVolume(LSMessage&)+0x574> @ imm = #-0x3c
    8c02: 4628         	mov	r0, r5
    8c04: f10d 0870    	add.w	r8, sp, #0x70
    8c08: f7fb ea72    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4b1c
    8c0c: ab16         	add	r3, sp, #0x58
    8c0e: 930d         	str	r3, [sp, #0x34]
    8c10: e7e3         	b	0x8bda <ConnectionManager::setInputVolume(LSMessage&)+0x586> @ imm = #-0x3a
    8c12: 4630         	mov	r0, r6
    8c14: f10d 0870    	add.w	r8, sp, #0x70
    8c18: f7fb ea6a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4b2c
    8c1c: 4b1d         	ldr	r3, [pc, #0x74]         @ 0x8c94 <ConnectionManager::setInputVolume(LSMessage&)+0x640> literal=0x210
    8c1e: 4628         	mov	r0, r5
    8c20: f85a 3003    	ldr.w	r3, [r10, r3]
    8c24: 3308         	adds	r3, #0x8
    8c26: 932e         	str	r3, [sp, #0xb8]
    8c28: f7fb e990    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x4ce0
    8c2c: ab16         	add	r3, sp, #0x58
    8c2e: 930d         	str	r3, [sp, #0x34]
    8c30: e7d3         	b	0x8bda <ConnectionManager::setInputVolume(LSMessage&)+0x586> @ imm = #-0x5a
    8c32: 4628         	mov	r0, r5
    8c34: f10d 0870    	add.w	r8, sp, #0x70
    8c38: f7fb ea5a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4b4c
    8c3c: f10b 0308    	add.w	r3, r11, #0x8
    8c40: 4630         	mov	r0, r6
    8c42: 9328         	str	r3, [sp, #0xa0]
    8c44: f7fb e982    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x4cfc
    8c48: ab16         	add	r3, sp, #0x58
    8c4a: 930d         	str	r3, [sp, #0x34]
    8c4c: e7c5         	b	0x8bda <ConnectionManager::setInputVolume(LSMessage&)+0x586> @ imm = #-0x76
    8c4e: e7bb         	b	0x8bc8 <ConnectionManager::setInputVolume(LSMessage&)+0x574> @ imm = #-0x8a
    8c50: e7ba         	b	0x8bc8 <ConnectionManager::setInputVolume(LSMessage&)+0x574> @ imm = #-0x8c
    8c52: e7b9         	b	0x8bc8 <ConnectionManager::setInputVolume(LSMessage&)+0x574> @ imm = #-0x8e
    8c54: e7b8         	b	0x8bc8 <ConnectionManager::setInputVolume(LSMessage&)+0x574> @ imm = #-0x90
    8c56: e7bd         	b	0x8bd4 <ConnectionManager::setInputVolume(LSMessage&)+0x580> @ imm = #-0x86
    8c58: 980e         	ldr	r0, [sp, #0x38]
    8c5a: e7cc         	b	0x8bf6 <ConnectionManager::setInputVolume(LSMessage&)+0x5a2> @ imm = #-0x68
    8c5c: f10b 0308    	add.w	r3, r11, #0x8
    8c60: 4628         	mov	r0, r5
    8c62: 932e         	str	r3, [sp, #0xb8]
    8c64: f10d 0870    	add.w	r8, sp, #0x70
    8c68: f7fb e970    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x4d20
    8c6c: ab16         	add	r3, sp, #0x58
    8c6e: 930d         	str	r3, [sp, #0x34]
    8c70: e7b3         	b	0x8bda <ConnectionManager::setInputVolume(LSMessage&)+0x586> @ imm = #-0x9a
    8c72: ab16         	add	r3, sp, #0x58
    8c74: f10d 0870    	add.w	r8, sp, #0x70
    8c78: 930d         	str	r3, [sp, #0x34]
    8c7a: e7ae         	b	0x8bda <ConnectionManager::setInputVolume(LSMessage&)+0x586> @ imm = #-0xa4
    8c7c: ab16         	add	r3, sp, #0x58
    8c7e: f10d 0870    	add.w	r8, sp, #0x70
    8c82: 930d         	str	r3, [sp, #0x34]
    8c84: e7ac         	b	0x8be0 <ConnectionManager::setInputVolume(LSMessage&)+0x58c> @ imm = #-0xa8
    8c86: bf00         	nop
    8c88: 874e         	strh	r6, [r1, #0x3a]
    8c8a: 0001         	movs	r1, r0
    8c8c: 5b7a         	ldrh	r2, [r7, r5]
    8c8e: 0000         	movs	r0, r0
    8c90: 53cc         	strh	r4, [r1, r7]
    8c92: 0000         	movs	r0, r0
    8c94: 0210         	lsls	r0, r2, #0x8
    8c96: 0000         	movs	r0, r0
    8c98: 56be         	ldrsb	r6, [r7, r2]
    8c9a: 0000         	movs	r0, r0
    8c9c: 5ac8         	ldrh	r0, [r1, r3]
    8c9e: 0000         	movs	r0, r0
    8ca0: 5a8c         	ldrh	r4, [r1, r2]
    8ca2: 0000         	movs	r0, r0
    8ca4: 4ee8         	ldr	r6, [pc, #0x3a0]        @ 0x9048 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x1d0> literal=0x9938d003
    8ca6: 0000         	movs	r0, r0
    8ca8: 5a8c         	ldrh	r4, [r1, r2]
    8caa: 0000         	movs	r0, r0
    8cac: 5a12         	ldrh	r2, [r2, r0]
    8cae: 0000         	movs	r0, r0
    8cb0: 4e70         	ldr	r6, [pc, #0x1c0]        @ 0x8e74 <ConnectionManager::getStatus(LSMessage&)+0x194> literal=0x485c
    8cb2: 0000         	movs	r0, r0
    8cb4: 5a66         	ldrh	r6, [r4, r1]
    8cb6: 0000         	movs	r0, r0
    8cb8: 4e16         	ldr	r6, [pc, #0x58]         @ 0x8d14 <ConnectionManager::getStatus(LSMessage&)+0x34> literal=0xe9aef7fb
    8cba: 0000         	movs	r0, r0
    8cbc: 519c         	str	r4, [r3, r6]
    8cbe: 0000         	movs	r0, r0
    8cc0: 5910         	ldr	r0, [r2, r4]
    8cc2: 0000         	movs	r0, r0
    8cc4: 8644         	strh	r4, [r0, #0x32]
    8cc6: 0001         	movs	r1, r0
    8cc8: 85d0         	strh	r0, [r2, #0x2e]
    8cca: 0001         	movs	r1, r0
    8ccc: 4c90         	ldr	r4, [pc, #0x240]        @ 0x8f10 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x98> literal=0x80cdf200
    8cce: 0000         	movs	r0, r0
    8cd0: 8578         	strh	r0, [r7, #0x2a]
    8cd2: 0001         	movs	r1, r0
    8cd4: 5b7a         	ldrh	r2, [r7, r5]
    8cd6: 0000         	movs	r0, r0
    8cd8: 5b42         	ldrh	r2, [r0, r5]
    8cda: 0000         	movs	r0, r0
    8cdc: 4ad2         	ldr	r2, [pc, #0x348]        @ 0x9028 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x1b0> literal=0xf10deffa
    8cde: 0000         	movs	r0, r0

00008ce0 <ConnectionManager::getStatus(LSMessage&)>:
    8ce0: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    8ce4: b093         	sub	sp, #0x4c
    8ce6: 4607         	mov	r7, r0
    8ce8: ae03         	add	r6, sp, #0xc
    8cea: 4608         	mov	r0, r1
    8cec: 460c         	mov	r4, r1
    8cee: 9100         	str	r1, [sp]
    8cf0: f7fb e9f8    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x4c10
    8cf4: 4630         	mov	r0, r6
    8cf6: f7fb ea92    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x4adc
    8cfa: f10d 0818    	add.w	r8, sp, #0x18
    8cfe: ad0c         	add	r5, sp, #0x30
    8d00: 2200         	movs	r2, #0x0
    8d02: f10d 0920    	add.w	r9, sp, #0x20
    8d06: 233e         	movs	r3, #0x3e
    8d08: 4629         	mov	r1, r5
    8d0a: 4640         	mov	r0, r8
    8d0c: 9201         	str	r2, [sp, #0x4]
    8d0e: f8cd 9018    	str.w	r9, [sp, #0x18]
    8d12: 930c         	str	r3, [sp, #0x30]
    8d14: f7fb e9ae    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x4ca4
    8d18: f8df e150    	ldr.w	lr, [pc, #0x150]        @ 0x8e6c <ConnectionManager::getStatus(LSMessage&)+0x18c> literal=0x567a
    8d1c: 4684         	mov	r12, r0
    8d1e: 9b0c         	ldr	r3, [sp, #0x30]
    8d20: 9006         	str	r0, [sp, #0x18]
    8d22: 44fe         	add	lr, pc => raw=0xe3a0 ghidra=0x1e3a0 file=58272 "{\"type\":\"object\",\"properties\":{},\"additionalProperties\":false}"
    8d24: f10e 0b30    	add.w	r11, lr, #0x30
    8d28: 9308         	str	r3, [sp, #0x20]
    8d2a: 46f2         	mov	r10, lr
    8d2c: f10c 0c10    	add.w	r12, r12, #0x10
    8d30: e8ba 000f    	ldm.w	r10!, {r0, r1, r2, r3}
    8d34: f10e 0e10    	add.w	lr, lr, #0x10
    8d38: 45da         	cmp	r10, r11
    8d3a: f84c 0c10    	str	r0, [r12, #-16]
    8d3e: f84c 1c0c    	str	r1, [r12, #-12]
    8d42: f84c 2c08    	str	r2, [r12, #-8]
    8d46: f84c 3c04    	str	r3, [r12, #-4]
    8d4a: d1ee         	bne	0x8d2a <ConnectionManager::getStatus(LSMessage&)+0x4a> @ imm = #-0x24
    8d4c: 4673         	mov	r3, lr
    8d4e: f04f 0e00    	mov.w	lr, #0x0
    8d52: cb07         	ldm	r3!, {r0, r1, r2}
    8d54: 881b         	ldrh	r3, [r3]
    8d56: f8cc 0000    	str.w	r0, [r12]
    8d5a: 4620         	mov	r0, r4
    8d5c: f8cc 1004    	str.w	r1, [r12, #0x4]
    8d60: f8cc 2008    	str.w	r2, [r12, #0x8]
    8d64: f8ac 300c    	strh.w	r3, [r12, #0xc]
    8d68: 9b0c         	ldr	r3, [sp, #0x30]
    8d6a: 9a06         	ldr	r2, [sp, #0x18]
    8d6c: 9307         	str	r3, [sp, #0x1c]
    8d6e: f802 e003    	strb.w	lr, [r2, r3]
    8d72: f7fb ea12    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x4bdc
    8d76: f10d 0b38    	add.w	r11, sp, #0x38
    8d7a: 4682         	mov	r10, r0
    8d7c: f8cd b030    	str.w	r11, [sp, #0x30]
    8d80: 2800         	cmp	r0, #0x0
    8d82: d058         	beq	0x8e36 <ConnectionManager::getStatus(LSMessage&)+0x156> @ imm = #0xb0
    8d84: f7fb e98e    	blx	0x40a4 <strlen@plt>     @ imm = #-0x4ce4
    8d88: 280f         	cmp	r0, #0xf
    8d8a: 4604         	mov	r4, r0
    8d8c: 9002         	str	r0, [sp, #0x8]
    8d8e: d843         	bhi	0x8e18 <ConnectionManager::getStatus(LSMessage&)+0x138> @ imm = #0x86
    8d90: 2801         	cmp	r0, #0x1
    8d92: d133         	bne	0x8dfc <ConnectionManager::getStatus(LSMessage&)+0x11c> @ imm = #0x66
    8d94: f89a 3000    	ldrb.w	r3, [r10]
    8d98: f88d 3038    	strb.w	r3, [sp, #0x38]
    8d9c: 465b         	mov	r3, r11
    8d9e: 2200         	movs	r2, #0x0
    8da0: 940d         	str	r4, [sp, #0x34]
    8da2: 4631         	mov	r1, r6
    8da4: 551a         	strb	r2, [r3, r4]
    8da6: 4628         	mov	r0, r5
    8da8: ab01         	add	r3, sp, #0x4
    8daa: 4642         	mov	r2, r8
    8dac: f001 fb14    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #0x1628
    8db0: 4604         	mov	r4, r0
    8db2: 980c         	ldr	r0, [sp, #0x30]
    8db4: 4558         	cmp	r0, r11
    8db6: d003         	beq	0x8dc0 <ConnectionManager::getStatus(LSMessage&)+0xe0> @ imm = #0x6
    8db8: 990e         	ldr	r1, [sp, #0x38]
    8dba: 3101         	adds	r1, #0x1
    8dbc: f7fb e8a0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4ec0
    8dc0: b30c         	cbz	r4, 0x8e06 <ConnectionManager::getStatus(LSMessage&)+0x126> @ imm = #0x42
    8dc2: 4639         	mov	r1, r7
    8dc4: 4628         	mov	r0, r5
    8dc6: f7fd f905    	bl	0x5fd4 <ConnectionManager::getStatus()> @ imm = #-0x2df6
    8dca: 4629         	mov	r1, r5
    8dcc: 4668         	mov	r0, sp
    8dce: f001 f85f    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #0x10be
    8dd2: 4628         	mov	r0, r5
    8dd4: f7fa efc0    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5080
    8dd8: 9806         	ldr	r0, [sp, #0x18]
    8dda: 4548         	cmp	r0, r9
    8ddc: d003         	beq	0x8de6 <ConnectionManager::getStatus(LSMessage&)+0x106> @ imm = #0x6
    8dde: 9908         	ldr	r1, [sp, #0x20]
    8de0: 3101         	adds	r1, #0x1
    8de2: f7fb e88e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x4ee4
    8de6: 4630         	mov	r0, r6
    8de8: f7fa efb6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5094
    8dec: 9800         	ldr	r0, [sp]
    8dee: b108         	cbz	r0, 0x8df4 <ConnectionManager::getStatus(LSMessage&)+0x114> @ imm = #0x2
    8df0: f7fb e92c    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x4da8
    8df4: 2001         	movs	r0, #0x1
    8df6: b013         	add	sp, #0x4c
    8df8: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    8dfc: 465b         	mov	r3, r11
    8dfe: 2800         	cmp	r0, #0x0
    8e00: d0cd         	beq	0x8d9e <ConnectionManager::getStatus(LSMessage&)+0xbe> @ imm = #-0x66
    8e02: 4658         	mov	r0, r11
    8e04: e010         	b	0x8e28 <ConnectionManager::getStatus(LSMessage&)+0x148> @ imm = #0x20
    8e06: 491a         	ldr	r1, [pc, #0x68]         @ 0x8e70 <ConnectionManager::getStatus(LSMessage&)+0x190> literal=0x18232
    8e08: 4623         	mov	r3, r4
    8e0a: 2203         	movs	r2, #0x3
    8e0c: 4668         	mov	r0, sp
    8e0e: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    8e10: 311c         	adds	r1, #0x1c
    8e12: f000 ff1b    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0xe36
    8e16: e7df         	b	0x8dd8 <ConnectionManager::getStatus(LSMessage&)+0xf8> @ imm = #-0x42
    8e18: 2200         	movs	r2, #0x0
    8e1a: a902         	add	r1, sp, #0x8
    8e1c: 4628         	mov	r0, r5
    8e1e: f7fb e92a    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x4dac
    8e22: 9b02         	ldr	r3, [sp, #0x8]
    8e24: 900c         	str	r0, [sp, #0x30]
    8e26: 930e         	str	r3, [sp, #0x38]
    8e28: 4622         	mov	r2, r4
    8e2a: 4651         	mov	r1, r10
    8e2c: f7fb e8ac    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x4ea8
    8e30: 9c02         	ldr	r4, [sp, #0x8]
    8e32: 9b0c         	ldr	r3, [sp, #0x30]
    8e34: e7b3         	b	0x8d9e <ConnectionManager::getStatus(LSMessage&)+0xbe> @ imm = #-0x9a
    8e36: 480f         	ldr	r0, [pc, #0x3c]         @ 0x8e74 <ConnectionManager::getStatus(LSMessage&)+0x194> literal=0x485c
    8e38: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    8e3a: f7fa ef64    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x5138
    8e3e: 4628         	mov	r0, r5
    8e40: f7fa ef8a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x50ec
    8e44: 4640         	mov	r0, r8
    8e46: f7fb e954    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4d58
    8e4a: 4630         	mov	r0, r6
    8e4c: f7fa ef84    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x50f8
    8e50: 9800         	ldr	r0, [sp]
    8e52: b108         	cbz	r0, 0x8e58 <ConnectionManager::getStatus(LSMessage&)+0x178> @ imm = #0x2
    8e54: f7fb e8fa    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x4e0c
    8e58: f7fb e992    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x4cdc
    8e5c: 9800         	ldr	r0, [sp]
    8e5e: e7f9         	b	0x8e54 <ConnectionManager::getStatus(LSMessage&)+0x174> @ imm = #-0xe
    8e60: e7f0         	b	0x8e44 <ConnectionManager::getStatus(LSMessage&)+0x164> @ imm = #-0x20
    8e62: e7f2         	b	0x8e4a <ConnectionManager::getStatus(LSMessage&)+0x16a> @ imm = #-0x1c
    8e64: 4628         	mov	r0, r5
    8e66: f7fb e944    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x4d78
    8e6a: e7eb         	b	0x8e44 <ConnectionManager::getStatus(LSMessage&)+0x164> @ imm = #-0x2a
    8e6c: 567a         	ldrsb	r2, [r7, r1]
    8e6e: 0000         	movs	r0, r0
    8e70: 8232         	strh	r2, [r6, #0x10]
    8e72: 0001         	movs	r1, r0
    8e74: 485c         	ldr	r0, [pc, #0x170]        @ 0x8fe8 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x170> literal=0x8c0f10d
    8e76: 0000         	movs	r0, r0

00008e78 <ConnectionManager::getHeadphoneStatus(LSMessage&)>:
    8e78: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    8e7c: b0c5         	sub	sp, #0x114
    8e7e: f8df 56ec    	ldr.w	r5, [pc, #0x6ec]        @ 0x956c <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x6f4> literal=0x17f30
    8e82: 2300         	movs	r3, #0x0
    8e84: aa2c         	add	r2, sp, #0xb0
    8e86: ac32         	add	r4, sp, #0xc8
    8e88: 4682         	mov	r10, r0
    8e8a: 4608         	mov	r0, r1
    8e8c: 447d         	add	r5, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    8e8e: 920d         	str	r2, [sp, #0x34]
    8e90: 940e         	str	r4, [sp, #0x38]
    8e92: 950b         	str	r5, [sp, #0x2c]
    8e94: 9316         	str	r3, [sp, #0x58]
    8e96: f88d 3057    	strb.w	r3, [sp, #0x57]
    8e9a: e9cd 232a    	strd	r2, r3, [sp, #168]
    8e9e: f88d 30b0    	strb.w	r3, [sp, #0xb0]
    8ea2: e9cd 4330    	strd	r4, r3, [sp, #192]
    8ea6: f88d 30c8    	strb.w	r3, [sp, #0xc8]
    8eaa: 9117         	str	r1, [sp, #0x5c]
    8eac: f7fb e91a    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x4dcc
    8eb0: f10d 0960    	add.w	r9, sp, #0x60
    8eb4: 4648         	mov	r0, r9
    8eb6: f7fb e9b2    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x4c9c
    8eba: ab38         	add	r3, sp, #0xe0
    8ebc: ae3c         	add	r6, sp, #0xf0
    8ebe: a836         	add	r0, sp, #0xd8
    8ec0: 930f         	str	r3, [sp, #0x3c]
    8ec2: 9336         	str	r3, [sp, #0xd8]
    8ec4: 2200         	movs	r2, #0x0
    8ec6: 235c         	movs	r3, #0x5c
    8ec8: 4631         	mov	r1, r6
    8eca: 900c         	str	r0, [sp, #0x30]
    8ecc: 933c         	str	r3, [sp, #0xf0]
    8ece: f7fb e8d2    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x4e5c
    8ed2: f8df 169c    	ldr.w	r1, [pc, #0x69c]        @ 0x9570 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x6f8> literal=0x54fa
    8ed6: 225c         	movs	r2, #0x5c
    8ed8: 9b3c         	ldr	r3, [sp, #0xf0]
    8eda: f10d 08c0    	add.w	r8, sp, #0xc0
    8ede: 9036         	str	r0, [sp, #0xd8]
    8ee0: af2a         	add	r7, sp, #0xa8
    8ee2: 4479         	add	r1, pc => raw=0xe3e0 ghidra=0x1e3e0 file=58336 "{\"type\":\"object\",\"properties\":{\"subscribe\":{\"type\":\"boolean\"}},\"additionalProperties\":false}"
    8ee4: 9338         	str	r3, [sp, #0xe0]
    8ee6: f7fb e850    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x4f60
    8eea: 9b3c         	ldr	r3, [sp, #0xf0]
    8eec: 2100         	movs	r1, #0x0
    8eee: 9a36         	ldr	r2, [sp, #0xd8]
    8ef0: 9337         	str	r3, [sp, #0xdc]
    8ef2: 54d1         	strb	r1, [r2, r3]
    8ef4: 9817         	ldr	r0, [sp, #0x5c]
    8ef6: f7fb e950    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x4d60
    8efa: ad3e         	add	r5, sp, #0xf8
    8efc: 4683         	mov	r11, r0
    8efe: 953c         	str	r5, [sp, #0xf0]
    8f00: 2800         	cmp	r0, #0x0
    8f02: f000 82be    	beq.w	0x9482 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x60a> @ imm = #0x57c
    8f06: f7fb e8ce    	blx	0x40a4 <strlen@plt>     @ imm = #-0x4e64
    8f0a: 280f         	cmp	r0, #0xf
    8f0c: 4604         	mov	r4, r0
    8f0e: 9027         	str	r0, [sp, #0x9c]
    8f10: f200 80cd    	bhi.w	0x90ae <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x236> @ imm = #0x19a
    8f14: 2801         	cmp	r0, #0x1
    8f16: f040 80b7    	bne.w	0x9088 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x210> @ imm = #0x16e
    8f1a: f89b 3000    	ldrb.w	r3, [r11]
    8f1e: f88d 30f8    	strb.w	r3, [sp, #0xf8]
    8f22: 462b         	mov	r3, r5
    8f24: 2200         	movs	r2, #0x0
    8f26: 943d         	str	r4, [sp, #0xf4]
    8f28: 4649         	mov	r1, r9
    8f2a: 551a         	strb	r2, [r3, r4]
    8f2c: 4630         	mov	r0, r6
    8f2e: 9a0c         	ldr	r2, [sp, #0x30]
    8f30: ab16         	add	r3, sp, #0x58
    8f32: f001 fa51    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #0x14a2
    8f36: 4604         	mov	r4, r0
    8f38: 983c         	ldr	r0, [sp, #0xf0]
    8f3a: 42a8         	cmp	r0, r5
    8f3c: d003         	beq	0x8f46 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0xce> @ imm = #0x6
    8f3e: 993e         	ldr	r1, [sp, #0xf8]
    8f40: 3101         	adds	r1, #0x1
    8f42: f7fa efde    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5044
    8f46: 2c00         	cmp	r4, #0x0
    8f48: f000 80a4    	beq.w	0x9094 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x21c> @ imm = #0x148
    8f4c: f8df 2624    	ldr.w	r2, [pc, #0x624]        @ 0x9574 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x6fc> literal=0x54e2
    8f50: 4649         	mov	r1, r9
    8f52: 4630         	mov	r0, r6
    8f54: f10d 08c0    	add.w	r8, sp, #0xc0
    8f58: af2a         	add	r7, sp, #0xa8
    8f5a: 447a         	add	r2, pc => raw=0xe440 ghidra=0x1e440 file=58432 "subscribe"
    8f5c: f7fa efd6    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x5054
    8f60: ac27         	add	r4, sp, #0x9c
    8f62: 4630         	mov	r0, r6
    8f64: 4621         	mov	r1, r4
    8f66: f7fa ef40    	blx	0x3de8 <_ZNK7pbnjson6JValue6asBoolERb@plt> @ imm = #-0x5180
    8f6a: f8df 360c    	ldr.w	r3, [pc, #0x60c]        @ 0x9578 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x700> literal=0x210
    8f6e: 4630         	mov	r0, r6
    8f70: 9a0b         	ldr	r2, [sp, #0x2c]
    8f72: f89d 709c    	ldrb.w	r7, [sp, #0x9c]
    8f76: 58d3         	ldr	r3, [r2, r3]
    8f78: 3308         	adds	r3, #0x8
    8f7a: 933c         	str	r3, [sp, #0xf0]
    8f7c: f7fa efe6    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x5034
    8f80: b367         	cbz	r7, 0x8fdc <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x164> @ imm = #0x58
    8f82: f8da 303c    	ldr.w	r3, [r10, #0x3c]
    8f86: f8ca 3000    	str.w	r3, [r10]
    8f8a: b1f3         	cbz	r3, 0x8fca <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x152> @ imm = #0x3c
    8f8c: f8d3 b000    	ldr.w	r11, [r3]
    8f90: f10d 08f4    	add.w	r8, sp, #0xf4
    8f94: f8df 35e4    	ldr.w	r3, [pc, #0x5e4]        @ 0x957c <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x704> literal=0x22c
    8f98: 4640         	mov	r0, r8
    8f9a: 9a0b         	ldr	r2, [sp, #0x2c]
    8f9c: 58d7         	ldr	r7, [r2, r3]
    8f9e: f107 0308    	add.w	r3, r7, #0x8
    8fa2: 933c         	str	r3, [sp, #0xf0]
    8fa4: f7fa efc0    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x5080
    8fa8: f8df 35d4    	ldr.w	r3, [pc, #0x5d4]        @ 0x9580 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x708> literal=0x224
    8fac: 4658         	mov	r0, r11
    8fae: 9a0b         	ldr	r2, [sp, #0x2c]
    8fb0: 58d1         	ldr	r1, [r2, r3]
    8fb2: 4643         	mov	r3, r8
    8fb4: 4652         	mov	r2, r10
    8fb6: f7fa ef30    	blx	0x3e18 <LSCallCancelNotificationAdd@plt> @ imm = #-0x51a0
    8fba: 4640         	mov	r0, r8
    8fbc: 3708         	adds	r7, #0x8
    8fbe: 973c         	str	r7, [sp, #0xf0]
    8fc0: f7fb e8e4    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x4e38
    8fc4: 4630         	mov	r0, r6
    8fc6: f7fa eeb0    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x52a0
    8fca: a917         	add	r1, sp, #0x5c
    8fcc: 4650         	mov	r0, r10
    8fce: f7fb e902    	blx	0x41d4 <_ZN2LS17SubscriptionPoint9subscribeERNS_7MessageE@plt> @ imm = #-0x4dfc
    8fd2: f8da 3044    	ldr.w	r3, [r10, #0x44]
    8fd6: 2b00         	cmp	r3, #0x0
    8fd8: f000 8221    	beq.w	0x941e <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x5a6> @ imm = #0x442
    8fdc: f10d 08c0    	add.w	r8, sp, #0xc0
    8fe0: af2a         	add	r7, sp, #0xa8
    8fe2: f001 faab    	bl	0xa53c <getPmLogContext()> @ imm = #0x1556
    8fe6: b138         	cbz	r0, 0x8ff8 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x180> @ imm = #0xe
    8fe8: f10d 08c0    	add.w	r8, sp, #0xc0
    8fec: af2a         	add	r7, sp, #0xa8
    8fee: f001 faa5    	bl	0xa53c <getPmLogContext()> @ imm = #0x154a
    8ff2: 6803         	ldr	r3, [r0]
    8ff4: 2b06         	cmp	r3, #0x6
    8ff6: dd18         	ble	0x902a <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x1b2> @ imm = #0x30
    8ff8: f10d 08c0    	add.w	r8, sp, #0xc0
    8ffc: af2a         	add	r7, sp, #0xa8
    8ffe: f001 fa9d    	bl	0xa53c <getPmLogContext()> @ imm = #0x153a
    9002: f8df 3580    	ldr.w	r3, [pc, #0x580]        @ 0x9584 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x70c> literal=0x5440
    9006: 2107         	movs	r1, #0x7
    9008: 447b         	add	r3, pc => raw=0xe44c ghidra=0x1e44c file=58444 "getHeadphoneStatus"
    900a: 9305         	str	r3, [sp, #0x14]
    900c: f8df 3578    	ldr.w	r3, [pc, #0x578]        @ 0x9588 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x710> literal=0x473c
    9010: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    9012: 9304         	str	r3, [sp, #0x10]
    9014: f8df 3574    	ldr.w	r3, [pc, #0x574]        @ 0x958c <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x714> literal=0x548c
    9018: 447b         	add	r3, pc => raw=0xe4a8 ghidra=0x1e4a8 file=58536 "%s:%s() umi->getHeadphoneStatus"
    901a: 9303         	str	r3, [sp, #0xc]
    901c: 2300         	movs	r3, #0x0
    901e: 461a         	mov	r2, r3
    9020: e9cd 3301    	strd	r3, r3, [sp, #4]
    9024: 9300         	str	r3, [sp]
    9026: f7fa effa    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x500c
    902a: f10d 08c0    	add.w	r8, sp, #0xc0
    902e: af2a         	add	r7, sp, #0xa8
    9030: 4643         	mov	r3, r8
    9032: 463a         	mov	r2, r7
    9034: f10d 0157    	add.w	r1, sp, #0x57
    9038: 4650         	mov	r0, r10
    903a: f7fc fd7d    	bl	0x5b38 <ConnectionManager::getHeadphoneStatus(bool*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)> @ imm = #-0x3506
    903e: 2800         	cmp	r0, #0x0
    9040: d047         	beq	0x90d2 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x25a> @ imm = #0x8e
    9042: 9836         	ldr	r0, [sp, #0xd8]
    9044: 9b0f         	ldr	r3, [sp, #0x3c]
    9046: 4298         	cmp	r0, r3
    9048: d003         	beq	0x9052 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x1da> @ imm = #0x6
    904a: 9938         	ldr	r1, [sp, #0xe0]
    904c: 3101         	adds	r1, #0x1
    904e: f7fa ef58    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5150
    9052: 4648         	mov	r0, r9
    9054: f7fa ee80    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5300
    9058: 9817         	ldr	r0, [sp, #0x5c]
    905a: b108         	cbz	r0, 0x9060 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x1e8> @ imm = #0x2
    905c: f7fa eff6    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x5014
    9060: 9830         	ldr	r0, [sp, #0xc0]
    9062: 9b0e         	ldr	r3, [sp, #0x38]
    9064: 4298         	cmp	r0, r3
    9066: d003         	beq	0x9070 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x1f8> @ imm = #0x6
    9068: 9932         	ldr	r1, [sp, #0xc8]
    906a: 3101         	adds	r1, #0x1
    906c: f7fa ef48    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5170
    9070: 982a         	ldr	r0, [sp, #0xa8]
    9072: 9b0d         	ldr	r3, [sp, #0x34]
    9074: 4298         	cmp	r0, r3
    9076: d003         	beq	0x9080 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x208> @ imm = #0x6
    9078: 992c         	ldr	r1, [sp, #0xb0]
    907a: 3101         	adds	r1, #0x1
    907c: f7fa ef40    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5180
    9080: 2001         	movs	r0, #0x1
    9082: b045         	add	sp, #0x114
    9084: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    9088: 462b         	mov	r3, r5
    908a: 2800         	cmp	r0, #0x0
    908c: f43f af4a    	beq.w	0x8f24 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0xac> @ imm = #-0x16c
    9090: 4628         	mov	r0, r5
    9092: e017         	b	0x90c4 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x24c> @ imm = #0x2e
    9094: f8df 14f8    	ldr.w	r1, [pc, #0x4f8]        @ 0x9590 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x718> literal=0x17f9c
    9098: 4623         	mov	r3, r4
    909a: 2203         	movs	r2, #0x3
    909c: a817         	add	r0, sp, #0x5c
    909e: f10d 08c0    	add.w	r8, sp, #0xc0
    90a2: af2a         	add	r7, sp, #0xa8
    90a4: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    90a6: 311c         	adds	r1, #0x1c
    90a8: f000 fdd0    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0xba0
    90ac: e7c9         	b	0x9042 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x1ca> @ imm = #-0x6e
    90ae: 2200         	movs	r2, #0x0
    90b0: a927         	add	r1, sp, #0x9c
    90b2: 4630         	mov	r0, r6
    90b4: f10d 08c0    	add.w	r8, sp, #0xc0
    90b8: af2a         	add	r7, sp, #0xa8
    90ba: f7fa efdc    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x5048
    90be: 9b27         	ldr	r3, [sp, #0x9c]
    90c0: 903c         	str	r0, [sp, #0xf0]
    90c2: 933e         	str	r3, [sp, #0xf8]
    90c4: 4622         	mov	r2, r4
    90c6: 4659         	mov	r1, r11
    90c8: f7fa ef5e    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x5144
    90cc: 9c27         	ldr	r4, [sp, #0x9c]
    90ce: 9b3c         	ldr	r3, [sp, #0xf0]
    90d0: e728         	b	0x8f24 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0xac> @ imm = #-0x1b0
    90d2: f89d 3057    	ldrb.w	r3, [sp, #0x57]
    90d6: 4639         	mov	r1, r7
    90d8: f88a 3048    	strb.w	r3, [r10, #0x48]
    90dc: f10a 034c    	add.w	r3, r10, #0x4c
    90e0: 4618         	mov	r0, r3
    90e2: 9312         	str	r3, [sp, #0x48]
    90e4: f7fb e894    	blx	0x4210 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt> @ imm = #-0x4ed8
    90e8: f10a 0364    	add.w	r3, r10, #0x64
    90ec: 4641         	mov	r1, r8
    90ee: 4618         	mov	r0, r3
    90f0: 9313         	str	r3, [sp, #0x4c]
    90f2: f7fb e88e    	blx	0x4210 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt> @ imm = #-0x4ee4
    90f6: ab1b         	add	r3, sp, #0x6c
    90f8: 4618         	mov	r0, r3
    90fa: 469b         	mov	r11, r3
    90fc: 9311         	str	r3, [sp, #0x44]
    90fe: f7fa ee5c    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x5348
    9102: f89a 1048    	ldrb.w	r1, [r10, #0x48]
    9106: 4620         	mov	r0, r4
    9108: f081 0101    	eor	r1, r1, #0x1
    910c: f7fa ee72    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x531c
    9110: f247 536d    	movw	r3, #0x756d
    9114: 4622         	mov	r2, r4
    9116: f2c6 5374    	movt	r3, #0x6574
    911a: 4631         	mov	r1, r6
    911c: 933e         	str	r3, [sp, #0xf8]
    911e: 2304         	movs	r3, #0x4
    9120: 933d         	str	r3, [sp, #0xf4]
    9122: 4658         	mov	r0, r11
    9124: 2300         	movs	r3, #0x0
    9126: 953c         	str	r5, [sp, #0xf0]
    9128: f88d 30fc    	strb.w	r3, [sp, #0xfc]
    912c: f7fa edd2    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x545c
    9130: 983c         	ldr	r0, [sp, #0xf0]
    9132: 42a8         	cmp	r0, r5
    9134: d003         	beq	0x913e <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x2c6> @ imm = #0x6
    9136: 993e         	ldr	r1, [sp, #0xf8]
    9138: 3101         	adds	r1, #0x1
    913a: f7fa eee2    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x523c
    913e: 4620         	mov	r0, r4
    9140: f7fa ee0a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x53ec
    9144: 2316         	movs	r3, #0x16
    9146: 2200         	movs	r2, #0x0
    9148: 4621         	mov	r1, r4
    914a: 4630         	mov	r0, r6
    914c: f8da b040    	ldr.w	r11, [r10, #0x40]
    9150: 953c         	str	r5, [sp, #0xf0]
    9152: 9327         	str	r3, [sp, #0x9c]
    9154: f7fa ef8e    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x50e4
    9158: f8df e438    	ldr.w	lr, [pc, #0x438]        @ 0x9594 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x71c> literal=0x455e
    915c: 4684         	mov	r12, r0
    915e: 9b27         	ldr	r3, [sp, #0x9c]
    9160: 903c         	str	r0, [sp, #0xf0]
    9162: 44fe         	add	lr, pc => raw=0xd6c4 ghidra=0x1d6c4 file=54980 "headphoneDetectionMute"
    9164: 933e         	str	r3, [sp, #0xf8]
    9166: e8be 000f    	ldm.w	lr!, {r0, r1, r2, r3}
    916a: f8cc 0000    	str.w	r0, [r12]
    916e: f8cc 300c    	str.w	r3, [r12, #0xc]
    9172: f8de 0000    	ldr.w	r0, [lr]
    9176: f8be 3004    	ldrh.w	r3, [lr, #0x4]
    917a: f8cc 1004    	str.w	r1, [r12, #0x4]
    917e: f8cc 0010    	str.w	r0, [r12, #0x10]
    9182: f04f 0000    	mov.w	r0, #0x0
    9186: f8cc 2008    	str.w	r2, [r12, #0x8]
    918a: f8ac 3014    	strh.w	r3, [r12, #0x14]
    918e: 9b27         	ldr	r3, [sp, #0x9c]
    9190: 9a3c         	ldr	r2, [sp, #0xf0]
    9192: 9911         	ldr	r1, [sp, #0x44]
    9194: 933d         	str	r3, [sp, #0xf4]
    9196: 54d0         	strb	r0, [r2, r3]
    9198: 4620         	mov	r0, r4
    919a: f7fa ee98    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x52d0
    919e: 4658         	mov	r0, r11
    91a0: 4622         	mov	r2, r4
    91a2: 4631         	mov	r1, r6
    91a4: f7fa eed8    	blx	0x3f58 <_ZN9umiClient11setSettingsENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt> @ imm = #-0x5250
    91a8: 4620         	mov	r0, r4
    91aa: f7fa edd6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5454
    91ae: 983c         	ldr	r0, [sp, #0xf0]
    91b0: 42a8         	cmp	r0, r5
    91b2: d003         	beq	0x91bc <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x344> @ imm = #0x6
    91b4: 993e         	ldr	r1, [sp, #0xf8]
    91b6: 3101         	adds	r1, #0x1
    91b8: f7fa eea2    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x52bc
    91bc: f10d 0b78    	add.w	r11, sp, #0x78
    91c0: 4658         	mov	r0, r11
    91c2: f7fa edfa    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x540c
    91c6: ab21         	add	r3, sp, #0x84
    91c8: 4618         	mov	r0, r3
    91ca: 930b         	str	r3, [sp, #0x2c]
    91cc: f7fa edf4    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x5418
    91d0: ab24         	add	r3, sp, #0x90
    91d2: 4618         	mov	r0, r3
    91d4: 9310         	str	r3, [sp, #0x40]
    91d6: f7fa edf0    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x5420
    91da: f89a 1048    	ldrb.w	r1, [r10, #0x48]
    91de: 4620         	mov	r0, r4
    91e0: f7fa ee08    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x53f0
    91e4: 4bec         	ldr	r3, [pc, #0x3b0]        @ 0x9598 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x720> literal=0x44ea
    91e6: 46ac         	mov	r12, r5
    91e8: 953c         	str	r5, [sp, #0xf0]
    91ea: f04f 0e00    	mov.w	lr, #0x0
    91ee: 447b         	add	r3, pc => raw=0xd6dc ghidra=0x1d6dc file=55004 "headPhoneStatus"
    91f0: cb0f         	ldm	r3, {r0, r1, r2, r3}
    91f2: e8ac 0007    	stm.w	r12!, {r0, r1, r2}
    91f6: 4622         	mov	r2, r4
    91f8: f82c 3b02    	strh	r3, [r12], #2
    91fc: 0c1b         	lsrs	r3, r3, #0x10
    91fe: 4631         	mov	r1, r6
    9200: f88c 3000    	strb.w	r3, [r12]
    9204: 4658         	mov	r0, r11
    9206: 230f         	movs	r3, #0xf
    9208: f88d e107    	strb.w	lr, [sp, #0x107]
    920c: 933d         	str	r3, [sp, #0xf4]
    920e: f7fa ed62    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x553c
    9212: 983c         	ldr	r0, [sp, #0xf0]
    9214: 42a8         	cmp	r0, r5
    9216: d003         	beq	0x9220 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x3a8> @ imm = #0x6
    9218: 993e         	ldr	r1, [sp, #0xf8]
    921a: 3101         	adds	r1, #0x1
    921c: f7fa ee70    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5320
    9220: 4620         	mov	r0, r4
    9222: f7fa ed9a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x54cc
    9226: 9912         	ldr	r1, [sp, #0x48]
    9228: 4620         	mov	r0, r4
    922a: f7fa ef80    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x5100
    922e: 4bdb         	ldr	r3, [pc, #0x36c]        @ 0x959c <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x724> literal=0x44b0
    9230: 46ac         	mov	r12, r5
    9232: 953c         	str	r5, [sp, #0xf0]
    9234: f04f 0e00    	mov.w	lr, #0x0
    9238: 447b         	add	r3, pc => raw=0xd6ec ghidra=0x1d6ec file=55020 "connectStatus"
    923a: cb0f         	ldm	r3, {r0, r1, r2, r3}
    923c: e8ac 0007    	stm.w	r12!, {r0, r1, r2}
    9240: 4622         	mov	r2, r4
    9242: f88c 3000    	strb.w	r3, [r12]
    9246: 4631         	mov	r1, r6
    9248: 230d         	movs	r3, #0xd
    924a: 980b         	ldr	r0, [sp, #0x2c]
    924c: 933d         	str	r3, [sp, #0xf4]
    924e: f88d e105    	strb.w	lr, [sp, #0x105]
    9252: f7fa ed40    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x5580
    9256: 983c         	ldr	r0, [sp, #0xf0]
    9258: 42a8         	cmp	r0, r5
    925a: d003         	beq	0x9264 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x3ec> @ imm = #0x6
    925c: 993e         	ldr	r1, [sp, #0xf8]
    925e: 3101         	adds	r1, #0x1
    9260: f7fa ee4e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5364
    9264: 4620         	mov	r0, r4
    9266: f7fa ed78    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5510
    926a: 2312         	movs	r3, #0x12
    926c: 2200         	movs	r2, #0x0
    926e: 4621         	mov	r1, r4
    9270: 4630         	mov	r0, r6
    9272: 953c         	str	r5, [sp, #0xf0]
    9274: 9327         	str	r3, [sp, #0x9c]
    9276: f7fa eefe    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x5204
    927a: f8df e324    	ldr.w	lr, [pc, #0x324]        @ 0x95a0 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x728> literal=0x4474
    927e: 4684         	mov	r12, r0
    9280: 9b27         	ldr	r3, [sp, #0x9c]
    9282: 903c         	str	r0, [sp, #0xf0]
    9284: 44fe         	add	lr, pc => raw=0xd6fc ghidra=0x1d6fc file=55036 "internalMicrophone"
    9286: 933e         	str	r3, [sp, #0xf8]
    9288: e8be 000f    	ldm.w	lr!, {r0, r1, r2, r3}
    928c: f8cc 300c    	str.w	r3, [r12, #0xc]
    9290: f8be 3000    	ldrh.w	r3, [lr]
    9294: f8cc 0000    	str.w	r0, [r12]
    9298: f04f 0000    	mov.w	r0, #0x0
    929c: f8cc 2008    	str.w	r2, [r12, #0x8]
    92a0: f8cc 1004    	str.w	r1, [r12, #0x4]
    92a4: f8ac 3010    	strh.w	r3, [r12, #0x10]
    92a8: 9b27         	ldr	r3, [sp, #0x9c]
    92aa: 993c         	ldr	r1, [sp, #0xf0]
    92ac: 9a0b         	ldr	r2, [sp, #0x2c]
    92ae: 933d         	str	r3, [sp, #0xf4]
    92b0: 54c8         	strb	r0, [r1, r3]
    92b2: 4631         	mov	r1, r6
    92b4: 4658         	mov	r0, r11
    92b6: f7fa ed0e    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x55e4
    92ba: 983c         	ldr	r0, [sp, #0xf0]
    92bc: 42a8         	cmp	r0, r5
    92be: d003         	beq	0x92c8 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x450> @ imm = #0x6
    92c0: 993e         	ldr	r1, [sp, #0xf8]
    92c2: 3101         	adds	r1, #0x1
    92c4: f7fa ee1c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x53c8
    92c8: 9913         	ldr	r1, [sp, #0x4c]
    92ca: 4620         	mov	r0, r4
    92cc: f7fa ef2e    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x51a4
    92d0: 4bb4         	ldr	r3, [pc, #0x2d0]        @ 0x95a4 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x72c> literal=0x440e
    92d2: 46ac         	mov	r12, r5
    92d4: 953c         	str	r5, [sp, #0xf0]
    92d6: f04f 0e00    	mov.w	lr, #0x0
    92da: 447b         	add	r3, pc => raw=0xd6ec ghidra=0x1d6ec file=55020 "connectStatus"
    92dc: cb0f         	ldm	r3, {r0, r1, r2, r3}
    92de: e8ac 0007    	stm.w	r12!, {r0, r1, r2}
    92e2: 4622         	mov	r2, r4
    92e4: f88c 3000    	strb.w	r3, [r12]
    92e8: 4631         	mov	r1, r6
    92ea: 230d         	movs	r3, #0xd
    92ec: 9810         	ldr	r0, [sp, #0x40]
    92ee: 933d         	str	r3, [sp, #0xf4]
    92f0: f88d e105    	strb.w	lr, [sp, #0x105]
    92f4: f7fa ecee    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x5624
    92f8: 983c         	ldr	r0, [sp, #0xf0]
    92fa: 42a8         	cmp	r0, r5
    92fc: d003         	beq	0x9306 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x48e> @ imm = #0x6
    92fe: 993e         	ldr	r1, [sp, #0xf8]
    9300: 3101         	adds	r1, #0x1
    9302: f7fa edfe    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5404
    9306: 4620         	mov	r0, r4
    9308: f7fa ed26    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x55b4
    930c: 2312         	movs	r3, #0x12
    930e: 2200         	movs	r2, #0x0
    9310: 4621         	mov	r1, r4
    9312: 4630         	mov	r0, r6
    9314: 953c         	str	r5, [sp, #0xf0]
    9316: 9327         	str	r3, [sp, #0x9c]
    9318: f7fa eeac    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x52a8
    931c: f8df e288    	ldr.w	lr, [pc, #0x288]        @ 0x95a8 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x730> literal=0x43e6
    9320: 4684         	mov	r12, r0
    9322: 9b27         	ldr	r3, [sp, #0x9c]
    9324: 903c         	str	r0, [sp, #0xf0]
    9326: 44fe         	add	lr, pc => raw=0xd710 ghidra=0x1d710 file=55056 "externalMicrophone"
    9328: 933e         	str	r3, [sp, #0xf8]
    932a: e8be 000f    	ldm.w	lr!, {r0, r1, r2, r3}
    932e: f8cc 300c    	str.w	r3, [r12, #0xc]
    9332: f8be 3000    	ldrh.w	r3, [lr]
    9336: f8cc 0000    	str.w	r0, [r12]
    933a: f04f 0000    	mov.w	r0, #0x0
    933e: f8cc 2008    	str.w	r2, [r12, #0x8]
    9342: f8cc 1004    	str.w	r1, [r12, #0x4]
    9346: f8ac 3010    	strh.w	r3, [r12, #0x10]
    934a: 9b27         	ldr	r3, [sp, #0x9c]
    934c: 993c         	ldr	r1, [sp, #0xf0]
    934e: 9a10         	ldr	r2, [sp, #0x40]
    9350: 933d         	str	r3, [sp, #0xf4]
    9352: 54c8         	strb	r0, [r1, r3]
    9354: 4631         	mov	r1, r6
    9356: 4658         	mov	r0, r11
    9358: f7fa ecbc    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x5688
    935c: 983c         	ldr	r0, [sp, #0xf0]
    935e: 42a8         	cmp	r0, r5
    9360: d003         	beq	0x936a <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x4f2> @ imm = #0x6
    9362: 993e         	ldr	r1, [sp, #0xf8]
    9364: 3101         	adds	r1, #0x1
    9366: f7fa edcc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5468
    936a: 2101         	movs	r1, #0x1
    936c: 4620         	mov	r0, r4
    936e: f7fa ed42    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x557c
    9372: 4a8e         	ldr	r2, [pc, #0x238]        @ 0x95ac <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x734> literal=0x43a0
    9374: 462b         	mov	r3, r5
    9376: 953c         	str	r5, [sp, #0xf0]
    9378: f04f 0e0b    	mov.w	lr, #0xb
    937c: f04f 0c00    	mov.w	r12, #0x0
    9380: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    9382: ca07         	ldm	r2, {r0, r1, r2}
    9384: c303         	stm	r3!, {r0, r1}
    9386: 4631         	mov	r1, r6
    9388: f823 2b02    	strh	r2, [r3], #2
    938c: 0c12         	lsrs	r2, r2, #0x10
    938e: 4658         	mov	r0, r11
    9390: 701a         	strb	r2, [r3]
    9392: 4622         	mov	r2, r4
    9394: f8cd e0f4    	str.w	lr, [sp, #0xf4]
    9398: f88d c103    	strb.w	r12, [sp, #0x103]
    939c: f7fa ec9a    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x56cc
    93a0: 983c         	ldr	r0, [sp, #0xf0]
    93a2: 42a8         	cmp	r0, r5
    93a4: d003         	beq	0x93ae <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x536> @ imm = #0x6
    93a6: 993e         	ldr	r1, [sp, #0xf8]
    93a8: 3101         	adds	r1, #0x1
    93aa: f7fa edaa    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x54ac
    93ae: 4620         	mov	r0, r4
    93b0: f7fa ecd2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x565c
    93b4: f001 f8c2    	bl	0xa53c <getPmLogContext()> @ imm = #0x1184
    93b8: b120         	cbz	r0, 0x93c4 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x54c> @ imm = #0x8
    93ba: f001 f8bf    	bl	0xa53c <getPmLogContext()> @ imm = #0x117e
    93be: 6803         	ldr	r3, [r0]
    93c0: 2b06         	cmp	r3, #0x6
    93c2: dd1b         	ble	0x93fc <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x584> @ imm = #0x36
    93c4: f001 f8ba    	bl	0xa53c <getPmLogContext()> @ imm = #0x1174
    93c8: f8da 3064    	ldr.w	r3, [r10, #0x64]
    93cc: 2107         	movs	r1, #0x7
    93ce: 9308         	str	r3, [sp, #0x20]
    93d0: f8da 304c    	ldr.w	r3, [r10, #0x4c]
    93d4: 9307         	str	r3, [sp, #0x1c]
    93d6: f89a 3048    	ldrb.w	r3, [r10, #0x48]
    93da: 9306         	str	r3, [sp, #0x18]
    93dc: 4b74         	ldr	r3, [pc, #0x1d0]        @ 0x95b0 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x738> literal=0x506a
    93de: 447b         	add	r3, pc => raw=0xe44c ghidra=0x1e44c file=58444 "getHeadphoneStatus"
    93e0: 9305         	str	r3, [sp, #0x14]
    93e2: 4b74         	ldr	r3, [pc, #0x1d0]        @ 0x95b4 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x73c> literal=0x4368
    93e4: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    93e6: 9304         	str	r3, [sp, #0x10]
    93e8: 4b73         	ldr	r3, [pc, #0x1cc]        @ 0x95b8 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x740> literal=0x50da
    93ea: 447b         	add	r3, pc => raw=0xe4c8 ghidra=0x1e4c8 file=58568 "%s:%s() Send headphone connection status: %d mInternalConnectStatus:%s mExternalConnectStatus:%s"
    93ec: 9303         	str	r3, [sp, #0xc]
    93ee: 2300         	movs	r3, #0x0
    93f0: 461a         	mov	r2, r3
    93f2: e9cd 3301    	strd	r3, r3, [sp, #4]
    93f6: 9300         	str	r3, [sp]
    93f8: f7fa ee10    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x53e0
    93fc: 4659         	mov	r1, r11
    93fe: a817         	add	r0, sp, #0x5c
    9400: f000 fd46    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #0xa8c
    9404: 9810         	ldr	r0, [sp, #0x40]
    9406: f7fa eca8    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x56b0
    940a: 980b         	ldr	r0, [sp, #0x2c]
    940c: f7fa eca4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x56b8
    9410: 4658         	mov	r0, r11
    9412: f7fa eca2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x56bc
    9416: 9811         	ldr	r0, [sp, #0x44]
    9418: f7fa ec9e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x56c4
    941c: e611         	b	0x9042 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x1ca> @ imm = #-0x3de
    941e: 4967         	ldr	r1, [pc, #0x19c]        @ 0x95bc <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x744> literal=0xffffbcbd
    9420: 4652         	mov	r2, r10
    9422: f44f 70fa    	mov.w	r0, #0x1f4
    9426: f10d 08c0    	add.w	r8, sp, #0xc0
    942a: af2a         	add	r7, sp, #0xa8
    942c: 4479         	add	r1, pc => raw=0x50ed ghidra=0x150ed file=20717
    942e: f7fa eea2    	blx	0x4174 <g_timeout_add@plt> @ imm = #-0x52bc
    9432: f8ca 0044    	str.w	r0, [r10, #0x44]
    9436: f001 f881    	bl	0xa53c <getPmLogContext()> @ imm = #0x1102
    943a: b140         	cbz	r0, 0x944e <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x5d6> @ imm = #0x10
    943c: f10d 08c0    	add.w	r8, sp, #0xc0
    9440: af2a         	add	r7, sp, #0xa8
    9442: f001 f87b    	bl	0xa53c <getPmLogContext()> @ imm = #0x10f6
    9446: 6803         	ldr	r3, [r0]
    9448: 2b06         	cmp	r3, #0x6
    944a: f77f adc7    	ble.w	0x8fdc <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x164> @ imm = #-0x472
    944e: f10d 08c0    	add.w	r8, sp, #0xc0
    9452: af2a         	add	r7, sp, #0xa8
    9454: f001 f872    	bl	0xa53c <getPmLogContext()> @ imm = #0x10e4
    9458: f8da 3044    	ldr.w	r3, [r10, #0x44]
    945c: 2107         	movs	r1, #0x7
    945e: 9306         	str	r3, [sp, #0x18]
    9460: 4b57         	ldr	r3, [pc, #0x15c]        @ 0x95c0 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x748> literal=0x4fe6
    9462: 447b         	add	r3, pc => raw=0xe44c ghidra=0x1e44c file=58444 "getHeadphoneStatus"
    9464: 9305         	str	r3, [sp, #0x14]
    9466: 4b57         	ldr	r3, [pc, #0x15c]        @ 0x95c4 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x74c> literal=0x42e4
    9468: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    946a: 9304         	str	r3, [sp, #0x10]
    946c: 4b56         	ldr	r3, [pc, #0x158]        @ 0x95c8 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x750> literal=0x4fee
    946e: 447b         	add	r3, pc => raw=0xe460 ghidra=0x1e460 file=58464 "%s:%s() Timer for 5msec is created to monitor headphone status- id = %d"
    9470: 9303         	str	r3, [sp, #0xc]
    9472: 2300         	movs	r3, #0x0
    9474: 461a         	mov	r2, r3
    9476: e9cd 3301    	strd	r3, r3, [sp, #4]
    947a: 9300         	str	r3, [sp]
    947c: f7fa edce    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x5464
    9480: e5ac         	b	0x8fdc <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x164> @ imm = #-0x4a8
    9482: 4852         	ldr	r0, [pc, #0x148]        @ 0x95cc <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x754> literal=0x420a
    9484: f10d 08c0    	add.w	r8, sp, #0xc0
    9488: af2a         	add	r7, sp, #0xa8
    948a: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    948c: f7fa ec3a    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x578c
    9490: 4630         	mov	r0, r6
    9492: f7fa ee2e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x53a4
    9496: 4620         	mov	r0, r4
    9498: f7fa ec5e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5744
    949c: 9810         	ldr	r0, [sp, #0x40]
    949e: f7fa ec5c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5748
    94a2: 980b         	ldr	r0, [sp, #0x2c]
    94a4: f7fa ec58    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5750
    94a8: 4658         	mov	r0, r11
    94aa: f7fa ec56    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5754
    94ae: 9811         	ldr	r0, [sp, #0x44]
    94b0: f7fa ec52    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x575c
    94b4: 980c         	ldr	r0, [sp, #0x30]
    94b6: f7fa ee1c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x53c8
    94ba: 4648         	mov	r0, r9
    94bc: f7fa ec4c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5768
    94c0: 9817         	ldr	r0, [sp, #0x5c]
    94c2: b108         	cbz	r0, 0x94c8 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x650> @ imm = #0x2
    94c4: f7fa edc2    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x547c
    94c8: 4640         	mov	r0, r8
    94ca: f7fa ee12    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x53dc
    94ce: 4638         	mov	r0, r7
    94d0: f7fa ee0e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x53e4
    94d4: f7fa ee54    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x5358
    94d8: e7e6         	b	0x94a8 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x630> @ imm = #-0x34
    94da: e7df         	b	0x949c <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x624> @ imm = #-0x42
    94dc: e7e1         	b	0x94a2 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x62a> @ imm = #-0x3e
    94de: e7d7         	b	0x9490 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x618> @ imm = #-0x52
    94e0: 4630         	mov	r0, r6
    94e2: f10d 08c0    	add.w	r8, sp, #0xc0
    94e6: af2a         	add	r7, sp, #0xa8
    94e8: f000 fab4    	bl	0x9a54 <ConnectionManager::removePipelineInfo(LSMessage&)+0x484> @ imm = #0x568
    94ec: e7e2         	b	0x94b4 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x63c> @ imm = #-0x3c
    94ee: 4630         	mov	r0, r6
    94f0: f10d 08c0    	add.w	r8, sp, #0xc0
    94f4: af2a         	add	r7, sp, #0xa8
    94f6: f7fa ec18    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x57d0
    94fa: e7db         	b	0x94b4 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x63c> @ imm = #-0x4a
    94fc: e7c8         	b	0x9490 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x618> @ imm = #-0x70
    94fe: 4630         	mov	r0, r6
    9500: f7fa edf6    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x5414
    9504: e7ca         	b	0x949c <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x624> @ imm = #-0x6c
    9506: e7c3         	b	0x9490 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x618> @ imm = #-0x7a
    9508: e7f9         	b	0x94fe <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x686> @ imm = #-0xe
    950a: f10d 08c0    	add.w	r8, sp, #0xc0
    950e: af2a         	add	r7, sp, #0xa8
    9510: e7d3         	b	0x94ba <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x642> @ imm = #-0x5a
    9512: f10d 08c0    	add.w	r8, sp, #0xc0
    9516: af2a         	add	r7, sp, #0xa8
    9518: e7d2         	b	0x94c0 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x648> @ imm = #-0x5c
    951a: 4630         	mov	r0, r6
    951c: f10d 08c0    	add.w	r8, sp, #0xc0
    9520: af2a         	add	r7, sp, #0xa8
    9522: f7fa ede6    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x5434
    9526: e7c5         	b	0x94b4 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x63c> @ imm = #-0x76
    9528: 9a0b         	ldr	r2, [sp, #0x2c]
    952a: 4630         	mov	r0, r6
    952c: 4b12         	ldr	r3, [pc, #0x48]         @ 0x9578 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x700> literal=0x210
    952e: f10d 08c0    	add.w	r8, sp, #0xc0
    9532: af2a         	add	r7, sp, #0xa8
    9534: 58d3         	ldr	r3, [r2, r3]
    9536: 3308         	adds	r3, #0x8
    9538: 933c         	str	r3, [sp, #0xf0]
    953a: f7fa ed08    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x55f0
    953e: e7b9         	b	0x94b4 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x63c> @ imm = #-0x8e
    9540: e7b5         	b	0x94ae <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x636> @ imm = #-0x96
    9542: 4630         	mov	r0, r6
    9544: f7fa edd4    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x5458
    9548: 4620         	mov	r0, r4
    954a: f7fa ec06    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x57f4
    954e: e7ae         	b	0x94ae <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x636> @ imm = #-0xa4
    9550: e002         	b	0x9558 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x6e0> @ imm = #0x4
    9552: 4620         	mov	r0, r4
    9554: f7fa ec00    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5800
    9558: 4630         	mov	r0, r6
    955a: f7fa edca    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x546c
    955e: e7a6         	b	0x94ae <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x636> @ imm = #-0xb4
    9560: f10d 08c0    	add.w	r8, sp, #0xc0
    9564: af2a         	add	r7, sp, #0xa8
    9566: e7af         	b	0x94c8 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x650> @ imm = #-0xa2
    9568: e7a4         	b	0x94b4 <ConnectionManager::getHeadphoneStatus(LSMessage&)+0x63c> @ imm = #-0xb8
    956a: bf00         	nop
    956c: 7f30         	ldrb	r0, [r6, #0x1c]
    956e: 0001         	movs	r1, r0
    9570: 54fa         	strb	r2, [r7, r3]
    9572: 0000         	movs	r0, r0
    9574: 54e2         	strb	r2, [r4, r3]
    9576: 0000         	movs	r0, r0
    9578: 0210         	lsls	r0, r2, #0x8
    957a: 0000         	movs	r0, r0
    957c: 022c         	lsls	r4, r5, #0x8
    957e: 0000         	movs	r0, r0
    9580: 0224         	lsls	r4, r4, #0x8
    9582: 0000         	movs	r0, r0
    9584: 5440         	strb	r0, [r0, r1]
    9586: 0000         	movs	r0, r0
    9588: 473c         	bx	r7
    958a: 0000         	movs	r0, r0
    958c: 548c         	strb	r4, [r1, r2]
    958e: 0000         	movs	r0, r0
    9590: 7f9c         	ldrb	r4, [r3, #0x1e]
    9592: 0001         	movs	r1, r0
    9594: 455e         	cmp	r6, r11
    9596: 0000         	movs	r0, r0
    9598: 44ea         	add	r10, sp, r10
    959a: 0000         	movs	r0, r0
    959c: 44b0         	add	r8, r6
    959e: 0000         	movs	r0, r0
    95a0: 4474         	add	r4, lr
    95a2: 0000         	movs	r0, r0
    95a4: 440e         	add	r6, r1
    95a6: 0000         	movs	r0, r0
    95a8: 43e6         	mvns	r6, r4
    95aa: 0000         	movs	r0, r0
    95ac: 43a0         	bics	r0, r4
    95ae: 0000         	movs	r0, r0
    95b0: 506a         	str	r2, [r5, r1]
    95b2: 0000         	movs	r0, r0
    95b4: 4368         	muls	r0, r5, r0
    95b6: 0000         	movs	r0, r0
    95b8: 50da         	str	r2, [r3, r3]
    95ba: 0000         	movs	r0, r0
    95bc: bcbd         	pop	{r0, r2, r3, r4, r5, r7}
    95be: ffff 4fe6    	<unknown>
    95c2: 0000         	movs	r0, r0
    95c4: 42e4         	cmn	r4, r4
    95c6: 0000         	movs	r0, r0
    95c8: 4fee         	ldr	r7, [pc, #0x3b8]        @ 0x9984 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3b4> literal=0x9a237003
    95ca: 0000         	movs	r0, r0
    95cc: 420a         	tst	r2, r1
    95ce: 0000         	movs	r0, r0

000095d0 <ConnectionManager::removePipelineInfo(LSMessage&)>:
    95d0: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    95d4: 4681         	mov	r9, r0
    95d6: b0a9         	sub	sp, #0xa4
    95d8: 4608         	mov	r0, r1
    95da: 460c         	mov	r4, r1
    95dc: f10d 0834    	add.w	r8, sp, #0x34
    95e0: 910b         	str	r1, [sp, #0x2c]
    95e2: f7fa ed80    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x5500
    95e6: f8df 3420    	ldr.w	r3, [pc, #0x420]        @ 0x9a08 <ConnectionManager::removePipelineInfo(LSMessage&)+0x438> literal=0x177d0
    95ea: 4640         	mov	r0, r8
    95ec: 447b         	add	r3, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    95ee: 9309         	str	r3, [sp, #0x24]
    95f0: f7fa ee14    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x53d8
    95f4: ab1e         	add	r3, sp, #0x78
    95f6: f10d 0a70    	add.w	r10, sp, #0x70
    95fa: ae22         	add	r6, sp, #0x88
    95fc: 2200         	movs	r2, #0x0
    95fe: f10d 0b60    	add.w	r11, sp, #0x60
    9602: 9308         	str	r3, [sp, #0x20]
    9604: 931c         	str	r3, [sp, #0x70]
    9606: 4631         	mov	r1, r6
    9608: 2376         	movs	r3, #0x76
    960a: 4650         	mov	r0, r10
    960c: 920c         	str	r2, [sp, #0x30]
    960e: 9217         	str	r2, [sp, #0x5c]
    9610: f88d 2060    	strb.w	r2, [sp, #0x60]
    9614: f8cd b058    	str.w	r11, [sp, #0x58]
    9618: 9322         	str	r3, [sp, #0x88]
    961a: f7fa ed2c    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x55a8
    961e: 49fb         	ldr	r1, [pc, #0x3ec]        @ 0x9a0c <ConnectionManager::removePipelineInfo(LSMessage&)+0x43c> literal=0x4f02
    9620: 2276         	movs	r2, #0x76
    9622: 9b22         	ldr	r3, [sp, #0x88]
    9624: 901c         	str	r0, [sp, #0x70]
    9626: 4479         	add	r1, pc => raw=0xe52c ghidra=0x1e52c file=58668 "{\"type\":\"object\",\"properties\":{\"pipelineId\":{\"type\":\"string\"}},\"required\":[\"pipelineId\"],\"additionalProperties\":false}"
    9628: 931e         	str	r3, [sp, #0x78]
    962a: f7fa ecae    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x56a4
    962e: 9b22         	ldr	r3, [sp, #0x88]
    9630: 2100         	movs	r1, #0x0
    9632: 9a1c         	ldr	r2, [sp, #0x70]
    9634: 4620         	mov	r0, r4
    9636: 931d         	str	r3, [sp, #0x74]
    9638: 54d1         	strb	r1, [r2, r3]
    963a: f7fa edae    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x54a4
    963e: ac24         	add	r4, sp, #0x90
    9640: 4607         	mov	r7, r0
    9642: 9422         	str	r4, [sp, #0x88]
    9644: 2800         	cmp	r0, #0x0
    9646: f000 81a1    	beq.w	0x998c <ConnectionManager::removePipelineInfo(LSMessage&)+0x3bc> @ imm = #0x342
    964a: f7fa ed2c    	blx	0x40a4 <strlen@plt>     @ imm = #-0x55a8
    964e: 280f         	cmp	r0, #0xf
    9650: 4605         	mov	r5, r0
    9652: 9013         	str	r0, [sp, #0x4c]
    9654: f200 8174    	bhi.w	0x9940 <ConnectionManager::removePipelineInfo(LSMessage&)+0x370> @ imm = #0x2e8
    9658: 2801         	cmp	r0, #0x1
    965a: f040 80ea    	bne.w	0x9832 <ConnectionManager::removePipelineInfo(LSMessage&)+0x262> @ imm = #0x1d4
    965e: 783b         	ldrb	r3, [r7]
    9660: f88d 3090    	strb.w	r3, [sp, #0x90]
    9664: 4623         	mov	r3, r4
    9666: 2200         	movs	r2, #0x0
    9668: 9523         	str	r5, [sp, #0x8c]
    966a: 4641         	mov	r1, r8
    966c: 555a         	strb	r2, [r3, r5]
    966e: 4630         	mov	r0, r6
    9670: ab0c         	add	r3, sp, #0x30
    9672: 4652         	mov	r2, r10
    9674: f000 feb0    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #0xd60
    9678: 4605         	mov	r5, r0
    967a: 9822         	ldr	r0, [sp, #0x88]
    967c: 42a0         	cmp	r0, r4
    967e: d003         	beq	0x9688 <ConnectionManager::removePipelineInfo(LSMessage&)+0xb8> @ imm = #0x6
    9680: 9924         	ldr	r1, [sp, #0x90]
    9682: 3101         	adds	r1, #0x1
    9684: f7fa ec3c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5788
    9688: 2d00         	cmp	r5, #0x0
    968a: f000 80ee    	beq.w	0x986a <ConnectionManager::removePipelineInfo(LSMessage&)+0x29a> @ imm = #0x1dc
    968e: 4ae0         	ldr	r2, [pc, #0x380]        @ 0x9a10 <ConnectionManager::removePipelineInfo(LSMessage&)+0x440> literal=0x4f0a
    9690: ad13         	add	r5, sp, #0x4c
    9692: 4641         	mov	r1, r8
    9694: 4628         	mov	r0, r5
    9696: 447a         	add	r2, pc => raw=0xe5a4 ghidra=0x1e5a4 file=58788 "pipelineId"
    9698: f7fa ec38    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x5790
    969c: 2300         	movs	r3, #0x0
    969e: 4631         	mov	r1, r6
    96a0: 4628         	mov	r0, r5
    96a2: 9422         	str	r4, [sp, #0x88]
    96a4: 9323         	str	r3, [sp, #0x8c]
    96a6: f88d 3090    	strb.w	r3, [sp, #0x90]
    96aa: f7fa eb7a    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x590c
    96ae: 9b22         	ldr	r3, [sp, #0x88]
    96b0: 9816         	ldr	r0, [sp, #0x58]
    96b2: 9a23         	ldr	r2, [sp, #0x8c]
    96b4: 42a3         	cmp	r3, r4
    96b6: f000 8152    	beq.w	0x995e <ConnectionManager::removePipelineInfo(LSMessage&)+0x38e> @ imm = #0x2a4
    96ba: 9924         	ldr	r1, [sp, #0x90]
    96bc: 4558         	cmp	r0, r11
    96be: e9cd 3216    	strd	r3, r2, [sp, #88]
    96c2: bf08         	it	eq
    96c4: 9118         	streq	r1, [sp, #0x60]
    96c6: f000 8157    	beq.w	0x9978 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3a8> @ imm = #0x2ae
    96ca: 9b18         	ldr	r3, [sp, #0x60]
    96cc: 9118         	str	r1, [sp, #0x60]
    96ce: 2800         	cmp	r0, #0x0
    96d0: f000 8152    	beq.w	0x9978 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3a8> @ imm = #0x2a4
    96d4: 9324         	str	r3, [sp, #0x90]
    96d6: 9022         	str	r0, [sp, #0x88]
    96d8: 2300         	movs	r3, #0x0
    96da: 9323         	str	r3, [sp, #0x8c]
    96dc: 7003         	strb	r3, [r0]
    96de: 9822         	ldr	r0, [sp, #0x88]
    96e0: 42a0         	cmp	r0, r4
    96e2: d003         	beq	0x96ec <ConnectionManager::removePipelineInfo(LSMessage&)+0x11c> @ imm = #0x6
    96e4: 9924         	ldr	r1, [sp, #0x90]
    96e6: 3101         	adds	r1, #0x1
    96e8: f7fa ec0a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x57ec
    96ec: 9a09         	ldr	r2, [sp, #0x24]
    96ee: 4628         	mov	r0, r5
    96f0: 4bc8         	ldr	r3, [pc, #0x320]        @ 0x9a14 <ConnectionManager::removePipelineInfo(LSMessage&)+0x444> literal=0x210
    96f2: 58d3         	ldr	r3, [r2, r3]
    96f4: 3308         	adds	r3, #0x8
    96f6: 9313         	str	r3, [sp, #0x4c]
    96f8: f7fa ec28    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x57b0
    96fc: f000 ff1e    	bl	0xa53c <getPmLogContext()> @ imm = #0xe3c
    9700: b120         	cbz	r0, 0x970c <ConnectionManager::removePipelineInfo(LSMessage&)+0x13c> @ imm = #0x8
    9702: f000 ff1b    	bl	0xa53c <getPmLogContext()> @ imm = #0xe36
    9706: 6803         	ldr	r3, [r0]
    9708: 2b06         	cmp	r3, #0x6
    970a: dd14         	ble	0x9736 <ConnectionManager::removePipelineInfo(LSMessage&)+0x166> @ imm = #0x28
    970c: f000 ff16    	bl	0xa53c <getPmLogContext()> @ imm = #0xe2c
    9710: 9b16         	ldr	r3, [sp, #0x58]
    9712: 2107         	movs	r1, #0x7
    9714: 9306         	str	r3, [sp, #0x18]
    9716: 4bc0         	ldr	r3, [pc, #0x300]        @ 0x9a18 <ConnectionManager::removePipelineInfo(LSMessage&)+0x448> literal=0x4e94
    9718: 447b         	add	r3, pc => raw=0xe5b0 ghidra=0x1e5b0 file=58800 "removePipelineInfo"
    971a: 9305         	str	r3, [sp, #0x14]
    971c: 4bbf         	ldr	r3, [pc, #0x2fc]        @ 0x9a1c <ConnectionManager::removePipelineInfo(LSMessage&)+0x44c> literal=0x402e
    971e: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    9720: 9304         	str	r3, [sp, #0x10]
    9722: 4bbf         	ldr	r3, [pc, #0x2fc]        @ 0x9a20 <ConnectionManager::removePipelineInfo(LSMessage&)+0x450> literal=0x4e9c
    9724: 447b         	add	r3, pc => raw=0xe5c4 ghidra=0x1e5c4 file=58820 "%s:%s() Audio removePipelineInfo request for pipelineId %s"
    9726: 9303         	str	r3, [sp, #0xc]
    9728: 2300         	movs	r3, #0x0
    972a: 461a         	mov	r2, r3
    972c: e9cd 3301    	strd	r3, r3, [sp, #4]
    9730: 9300         	str	r3, [sp]
    9732: f7fa ec74    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x5718
    9736: af10         	add	r7, sp, #0x40
    9738: 4638         	mov	r0, r7
    973a: f7fa eb3e    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x5984
    973e: 4648         	mov	r0, r9
    9740: f10d 0958    	add.w	r9, sp, #0x58
    9744: 4649         	mov	r1, r9
    9746: f7fc fa45    	bl	0x5bd4 <ConnectionManager::removePipelineInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)> @ imm = #-0x3b76
    974a: 2800         	cmp	r0, #0x0
    974c: f040 8096    	bne.w	0x987c <ConnectionManager::removePipelineInfo(LSMessage&)+0x2ac> @ imm = #0x12c
    9750: f000 fef4    	bl	0xa53c <getPmLogContext()> @ imm = #0xde8
    9754: 2800         	cmp	r0, #0x0
    9756: d072         	beq	0x983e <ConnectionManager::removePipelineInfo(LSMessage&)+0x26e> @ imm = #0xe4
    9758: f000 fef0    	bl	0xa53c <getPmLogContext()> @ imm = #0xde0
    975c: 6803         	ldr	r3, [r0]
    975e: 2b06         	cmp	r3, #0x6
    9760: dc6d         	bgt	0x983e <ConnectionManager::removePipelineInfo(LSMessage&)+0x26e> @ imm = #0xda
    9762: 2101         	movs	r1, #0x1
    9764: 4628         	mov	r0, r5
    9766: f7fa eb46    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x5974
    976a: 4aae         	ldr	r2, [pc, #0x2b8]        @ 0x9a24 <ConnectionManager::removePipelineInfo(LSMessage&)+0x454> literal=0x3fa8
    976c: 4623         	mov	r3, r4
    976e: 9422         	str	r4, [sp, #0x88]
    9770: f04f 0e0b    	mov.w	lr, #0xb
    9774: f04f 0c00    	mov.w	r12, #0x0
    9778: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    977a: ca07         	ldm	r2, {r0, r1, r2}
    977c: c303         	stm	r3!, {r0, r1}
    977e: 4631         	mov	r1, r6
    9780: f823 2b02    	strh	r2, [r3], #2
    9784: 0c12         	lsrs	r2, r2, #0x10
    9786: 4638         	mov	r0, r7
    9788: 701a         	strb	r2, [r3]
    978a: 462a         	mov	r2, r5
    978c: f8cd e08c    	str.w	lr, [sp, #0x8c]
    9790: f88d c09b    	strb.w	r12, [sp, #0x9b]
    9794: f7fa ea9e    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x5ac4
    9798: 9822         	ldr	r0, [sp, #0x88]
    979a: 42a0         	cmp	r0, r4
    979c: d003         	beq	0x97a6 <ConnectionManager::removePipelineInfo(LSMessage&)+0x1d6> @ imm = #0x6
    979e: 9924         	ldr	r1, [sp, #0x90]
    97a0: 3101         	adds	r1, #0x1
    97a2: f7fa ebae    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x58a4
    97a6: 4628         	mov	r0, r5
    97a8: f7fa ead6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5a54
    97ac: 4649         	mov	r1, r9
    97ae: 4628         	mov	r0, r5
    97b0: f7fa ecbc    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x5688
    97b4: 4a9c         	ldr	r2, [pc, #0x270]        @ 0x9a28 <ConnectionManager::removePipelineInfo(LSMessage&)+0x458> literal=0x4dde
    97b6: 4623         	mov	r3, r4
    97b8: 9422         	str	r4, [sp, #0x88]
    97ba: f04f 0e0a    	mov.w	lr, #0xa
    97be: f04f 0c00    	mov.w	r12, #0x0
    97c2: 447a         	add	r2, pc => raw=0xe5a4 ghidra=0x1e5a4 file=58788 "pipelineId"
    97c4: ca07         	ldm	r2, {r0, r1, r2}
    97c6: c303         	stm	r3!, {r0, r1}
    97c8: 4631         	mov	r1, r6
    97ca: 801a         	strh	r2, [r3]
    97cc: 4638         	mov	r0, r7
    97ce: 462a         	mov	r2, r5
    97d0: f8cd e08c    	str.w	lr, [sp, #0x8c]
    97d4: f88d c09a    	strb.w	r12, [sp, #0x9a]
    97d8: f7fa ea7c    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x5b08
    97dc: 9822         	ldr	r0, [sp, #0x88]
    97de: 42a0         	cmp	r0, r4
    97e0: d003         	beq	0x97ea <ConnectionManager::removePipelineInfo(LSMessage&)+0x21a> @ imm = #0x6
    97e2: 9924         	ldr	r1, [sp, #0x90]
    97e4: 3101         	adds	r1, #0x1
    97e6: f7fa eb8c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x58e8
    97ea: 4628         	mov	r0, r5
    97ec: f7fa eab4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5a98
    97f0: 4639         	mov	r1, r7
    97f2: a80b         	add	r0, sp, #0x2c
    97f4: f000 fb4c    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #0x698
    97f8: 4638         	mov	r0, r7
    97fa: f7fa eaae    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5aa4
    97fe: 981c         	ldr	r0, [sp, #0x70]
    9800: 9b08         	ldr	r3, [sp, #0x20]
    9802: 4298         	cmp	r0, r3
    9804: d003         	beq	0x980e <ConnectionManager::removePipelineInfo(LSMessage&)+0x23e> @ imm = #0x6
    9806: 991e         	ldr	r1, [sp, #0x78]
    9808: 3101         	adds	r1, #0x1
    980a: f7fa eb7a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x590c
    980e: 9816         	ldr	r0, [sp, #0x58]
    9810: 4558         	cmp	r0, r11
    9812: d003         	beq	0x981c <ConnectionManager::removePipelineInfo(LSMessage&)+0x24c> @ imm = #0x6
    9814: 9918         	ldr	r1, [sp, #0x60]
    9816: 3101         	adds	r1, #0x1
    9818: f7fa eb72    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x591c
    981c: 4640         	mov	r0, r8
    981e: f7fa ea9c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5ac8
    9822: 980b         	ldr	r0, [sp, #0x2c]
    9824: b108         	cbz	r0, 0x982a <ConnectionManager::removePipelineInfo(LSMessage&)+0x25a> @ imm = #0x2
    9826: f7fa ec12    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x57dc
    982a: 2001         	movs	r0, #0x1
    982c: b029         	add	sp, #0xa4
    982e: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    9832: 4623         	mov	r3, r4
    9834: 2800         	cmp	r0, #0x0
    9836: f43f af16    	beq.w	0x9666 <ConnectionManager::removePipelineInfo(LSMessage&)+0x96> @ imm = #-0x1d4
    983a: 4620         	mov	r0, r4
    983c: e088         	b	0x9950 <ConnectionManager::removePipelineInfo(LSMessage&)+0x380> @ imm = #0x110
    983e: f000 fe7d    	bl	0xa53c <getPmLogContext()> @ imm = #0xcfa
    9842: 9b16         	ldr	r3, [sp, #0x58]
    9844: 2107         	movs	r1, #0x7
    9846: 9306         	str	r3, [sp, #0x18]
    9848: 4b78         	ldr	r3, [pc, #0x1e0]        @ 0x9a2c <ConnectionManager::removePipelineInfo(LSMessage&)+0x45c> literal=0x4d62
    984a: 447b         	add	r3, pc => raw=0xe5b0 ghidra=0x1e5b0 file=58800 "removePipelineInfo"
    984c: 9305         	str	r3, [sp, #0x14]
    984e: 4b78         	ldr	r3, [pc, #0x1e0]        @ 0x9a30 <ConnectionManager::removePipelineInfo(LSMessage&)+0x460> literal=0x3efc
    9850: 447b         	add	r3, pc => raw=0xd750 ghidra=0x1d750 file=55120 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/connectionmanager.cpp"
    9852: 9304         	str	r3, [sp, #0x10]
    9854: 4b77         	ldr	r3, [pc, #0x1dc]        @ 0x9a34 <ConnectionManager::removePipelineInfo(LSMessage&)+0x464> literal=0x4da6
    9856: 447b         	add	r3, pc => raw=0xe600 ghidra=0x1e600 file=58880 "%s:%s() Audio removePipelineInfo removed pipelineId %s is success"
    9858: 9303         	str	r3, [sp, #0xc]
    985a: 2300         	movs	r3, #0x0
    985c: 461a         	mov	r2, r3
    985e: e9cd 3301    	strd	r3, r3, [sp, #4]
    9862: 9300         	str	r3, [sp]
    9864: f7fa ebda    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x584c
    9868: e77b         	b	0x9762 <ConnectionManager::removePipelineInfo(LSMessage&)+0x192> @ imm = #-0x10a
    986a: 4973         	ldr	r1, [pc, #0x1cc]        @ 0x9a38 <ConnectionManager::removePipelineInfo(LSMessage&)+0x468> literal=0x177ce
    986c: 462b         	mov	r3, r5
    986e: 2203         	movs	r2, #0x3
    9870: a80b         	add	r0, sp, #0x2c
    9872: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    9874: 311c         	adds	r1, #0x1c
    9876: f000 f9e9    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #0x3d2
    987a: e7c0         	b	0x97fe <ConnectionManager::removePipelineInfo(LSMessage&)+0x22e> @ imm = #-0x80
    987c: 2100         	movs	r1, #0x0
    987e: 4628         	mov	r0, r5
    9880: f7fa eab8    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x5a90
    9884: 4a6d         	ldr	r2, [pc, #0x1b4]        @ 0x9a3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x46c> literal=0x3e8e
    9886: 4623         	mov	r3, r4
    9888: 9422         	str	r4, [sp, #0x88]
    988a: f04f 0e0b    	mov.w	lr, #0xb
    988e: f04f 0c00    	mov.w	r12, #0x0
    9892: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    9894: ca07         	ldm	r2, {r0, r1, r2}
    9896: c303         	stm	r3!, {r0, r1}
    9898: 4631         	mov	r1, r6
    989a: f823 2b02    	strh	r2, [r3], #2
    989e: 0c12         	lsrs	r2, r2, #0x10
    98a0: 4638         	mov	r0, r7
    98a2: 701a         	strb	r2, [r3]
    98a4: 462a         	mov	r2, r5
    98a6: f8cd e08c    	str.w	lr, [sp, #0x8c]
    98aa: f88d c09b    	strb.w	r12, [sp, #0x9b]
    98ae: f7fa ea12    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x5bdc
    98b2: 9822         	ldr	r0, [sp, #0x88]
    98b4: 42a0         	cmp	r0, r4
    98b6: d003         	beq	0x98c0 <ConnectionManager::removePipelineInfo(LSMessage&)+0x2f0> @ imm = #0x6
    98b8: 9924         	ldr	r1, [sp, #0x90]
    98ba: 3101         	adds	r1, #0x1
    98bc: f7fa eb20    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x59c0
    98c0: 4628         	mov	r0, r5
    98c2: f7fa ea4a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5b6c
    98c6: 495e         	ldr	r1, [pc, #0x178]        @ 0x9a40 <ConnectionManager::removePipelineInfo(LSMessage&)+0x470> literal=0x17776
    98c8: 4628         	mov	r0, r5
    98ca: 4479         	add	r1, pc => raw=0x21044 ghidra=0x31044 file=null
    98cc: 3164         	adds	r1, #0x64
    98ce: f7fa ec2e    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x57a4
    98d2: 4a5c         	ldr	r2, [pc, #0x170]        @ 0x9a44 <ConnectionManager::removePipelineInfo(LSMessage&)+0x474> literal=0x4d78
    98d4: 4623         	mov	r3, r4
    98d6: 9422         	str	r4, [sp, #0x88]
    98d8: f04f 0e09    	mov.w	lr, #0x9
    98dc: f04f 0c00    	mov.w	r12, #0x0
    98e0: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    98e2: ca07         	ldm	r2, {r0, r1, r2}
    98e4: c303         	stm	r3!, {r0, r1}
    98e6: 4631         	mov	r1, r6
    98e8: 701a         	strb	r2, [r3]
    98ea: 4638         	mov	r0, r7
    98ec: 462a         	mov	r2, r5
    98ee: f8cd e08c    	str.w	lr, [sp, #0x8c]
    98f2: f88d c099    	strb.w	r12, [sp, #0x99]
    98f6: f7fa e9ee    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x5c24
    98fa: 9822         	ldr	r0, [sp, #0x88]
    98fc: 42a0         	cmp	r0, r4
    98fe: d003         	beq	0x9908 <ConnectionManager::removePipelineInfo(LSMessage&)+0x338> @ imm = #0x6
    9900: 9924         	ldr	r1, [sp, #0x90]
    9902: 3101         	adds	r1, #0x1
    9904: f7fa eafc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5a08
    9908: 4628         	mov	r0, r5
    990a: f7fa ea26    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5bb4
    990e: 2114         	movs	r1, #0x14
    9910: 4628         	mov	r0, r5
    9912: f7fa ec12    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x57dc
    9916: 4a4c         	ldr	r2, [pc, #0x130]        @ 0x9a48 <ConnectionManager::removePipelineInfo(LSMessage&)+0x478> literal=0x4d40
    9918: 4623         	mov	r3, r4
    991a: 9422         	str	r4, [sp, #0x88]
    991c: f04f 0e09    	mov.w	lr, #0x9
    9920: f04f 0c00    	mov.w	r12, #0x0
    9924: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    9926: ca07         	ldm	r2, {r0, r1, r2}
    9928: c303         	stm	r3!, {r0, r1}
    992a: 4631         	mov	r1, r6
    992c: 701a         	strb	r2, [r3]
    992e: 4638         	mov	r0, r7
    9930: 462a         	mov	r2, r5
    9932: f8cd e08c    	str.w	lr, [sp, #0x8c]
    9936: f88d c099    	strb.w	r12, [sp, #0x99]
    993a: f7fa e9cc    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x5c68
    993e: e74d         	b	0x97dc <ConnectionManager::removePipelineInfo(LSMessage&)+0x20c> @ imm = #-0x166
    9940: 2200         	movs	r2, #0x0
    9942: a913         	add	r1, sp, #0x4c
    9944: 4630         	mov	r0, r6
    9946: f7fa eb96    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x58d4
    994a: 9b13         	ldr	r3, [sp, #0x4c]
    994c: 9022         	str	r0, [sp, #0x88]
    994e: 9324         	str	r3, [sp, #0x90]
    9950: 462a         	mov	r2, r5
    9952: 4639         	mov	r1, r7
    9954: f7fa eb18    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x59d0
    9958: 9d13         	ldr	r5, [sp, #0x4c]
    995a: 9b22         	ldr	r3, [sp, #0x88]
    995c: e683         	b	0x9666 <ConnectionManager::removePipelineInfo(LSMessage&)+0x96> @ imm = #-0x2fa
    995e: b132         	cbz	r2, 0x996e <ConnectionManager::removePipelineInfo(LSMessage&)+0x39e> @ imm = #0xc
    9960: 2a01         	cmp	r2, #0x1
    9962: d00d         	beq	0x9980 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3b0> @ imm = #0x1a
    9964: 4621         	mov	r1, r4
    9966: f7fa eb10    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x59e0
    996a: 9a23         	ldr	r2, [sp, #0x8c]
    996c: 9816         	ldr	r0, [sp, #0x58]
    996e: 2300         	movs	r3, #0x0
    9970: 9217         	str	r2, [sp, #0x5c]
    9972: 5483         	strb	r3, [r0, r2]
    9974: 9822         	ldr	r0, [sp, #0x88]
    9976: e6af         	b	0x96d8 <ConnectionManager::removePipelineInfo(LSMessage&)+0x108> @ imm = #-0x2a2
    9978: 9422         	str	r4, [sp, #0x88]
    997a: ac24         	add	r4, sp, #0x90
    997c: 4620         	mov	r0, r4
    997e: e6ab         	b	0x96d8 <ConnectionManager::removePipelineInfo(LSMessage&)+0x108> @ imm = #-0x2aa
    9980: f89d 3090    	ldrb.w	r3, [sp, #0x90]
    9984: 7003         	strb	r3, [r0]
    9986: 9a23         	ldr	r2, [sp, #0x8c]
    9988: 9816         	ldr	r0, [sp, #0x58]
    998a: e7f0         	b	0x996e <ConnectionManager::removePipelineInfo(LSMessage&)+0x39e> @ imm = #-0x20
    998c: 482f         	ldr	r0, [pc, #0xbc]         @ 0x9a4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x47c> literal=0x3d06
    998e: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    9990: f7fa e9b8    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x5c90
    9994: f10d 0958    	add.w	r9, sp, #0x58
    9998: 4648         	mov	r0, r9
    999a: f7fa ebaa    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x58ac
    999e: 4640         	mov	r0, r8
    99a0: f7fa e9da    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5c4c
    99a4: 980b         	ldr	r0, [sp, #0x2c]
    99a6: b108         	cbz	r0, 0x99ac <ConnectionManager::removePipelineInfo(LSMessage&)+0x3dc> @ imm = #0x2
    99a8: f7fa eb50    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x5960
    99ac: f7fa ebe8    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x5830
    99b0: 4630         	mov	r0, r6
    99b2: f7fa eb9e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x58c4
    99b6: 4628         	mov	r0, r5
    99b8: f7fa e9ce    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5c64
    99bc: 4638         	mov	r0, r7
    99be: f7fa e9cc    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5c68
    99c2: 4650         	mov	r0, r10
    99c4: f7fa eb94    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x58d8
    99c8: e7e6         	b	0x9998 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3c8> @ imm = #-0x34
    99ca: e7f1         	b	0x99b0 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3e0> @ imm = #-0x1e
    99cc: 4630         	mov	r0, r6
    99ce: f10d 0958    	add.w	r9, sp, #0x58
    99d2: f7fa eb8e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x58e4
    99d6: e7f4         	b	0x99c2 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3f2> @ imm = #-0x18
    99d8: 4630         	mov	r0, r6
    99da: f10d 0958    	add.w	r9, sp, #0x58
    99de: f7fa eb88    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x58f0
    99e2: 9a09         	ldr	r2, [sp, #0x24]
    99e4: 4628         	mov	r0, r5
    99e6: 4b0b         	ldr	r3, [pc, #0x2c]         @ 0x9a14 <ConnectionManager::removePipelineInfo(LSMessage&)+0x444> literal=0x210
    99e8: 58d3         	ldr	r3, [r2, r3]
    99ea: 3308         	adds	r3, #0x8
    99ec: 9313         	str	r3, [sp, #0x4c]
    99ee: f7fa eaae    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x5aa4
    99f2: e7e6         	b	0x99c2 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3f2> @ imm = #-0x34
    99f4: e7dc         	b	0x99b0 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3e0> @ imm = #-0x48
    99f6: 980b         	ldr	r0, [sp, #0x2c]
    99f8: e7d6         	b	0x99a8 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3d8> @ imm = #-0x54
    99fa: f10d 0958    	add.w	r9, sp, #0x58
    99fe: e7e0         	b	0x99c2 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3f2> @ imm = #-0x40
    9a00: e7d6         	b	0x99b0 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3e0> @ imm = #-0x54
    9a02: e7db         	b	0x99bc <ConnectionManager::removePipelineInfo(LSMessage&)+0x3ec> @ imm = #-0x4a
    9a04: e7d4         	b	0x99b0 <ConnectionManager::removePipelineInfo(LSMessage&)+0x3e0> @ imm = #-0x58
    9a06: bf00         	nop
    9a08: 77d0         	strb	r0, [r2, #0x1f]
    9a0a: 0001         	movs	r1, r0
    9a0c: 4f02         	ldr	r7, [pc, #0x8]          @ 0x9a18 <ConnectionManager::removePipelineInfo(LSMessage&)+0x448> literal=0x4e94
    9a0e: 0000         	movs	r0, r0
    9a10: 4f0a         	ldr	r7, [pc, #0x28]         @ 0x9a3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x46c> literal=0x3e8e
    9a12: 0000         	movs	r0, r0
    9a14: 0210         	lsls	r0, r2, #0x8
    9a16: 0000         	movs	r0, r0
    9a18: 4e94         	ldr	r6, [pc, #0x250]        @ 0x9c6c <ConnectionManager::removePipelineInfo(LSMessage&)+0x69c> literal=0xac0680ac
    9a1a: 0000         	movs	r0, r0
    9a1c: 402e         	ands	r6, r5
    9a1e: 0000         	movs	r0, r0
    9a20: 4e9c         	ldr	r6, [pc, #0x270]        @ 0x9c94 <ConnectionManager::removePipelineInfo(LSMessage&)+0x6c4> literal=0xc122b02
    9a22: 0000         	movs	r0, r0
    9a24: 3fa8         	subs	r7, #0xa8
    9a26: 0000         	movs	r0, r0
    9a28: 4dde         	ldr	r5, [pc, #0x378]        @ 0x9da4 <ConnectionManager::removePipelineInfo(LSMessage&)+0x7d4> literal=0x4628e9f4
    9a2a: 0000         	movs	r0, r0
    9a2c: 4d62         	ldr	r5, [pc, #0x188]        @ 0x9bb8 <ConnectionManager::removePipelineInfo(LSMessage&)+0x5e8> literal=0x463b58e1
    9a2e: 0000         	movs	r0, r0
    9a30: 3efc         	subs	r6, #0xfc
    9a32: 0000         	movs	r0, r0
    9a34: 4da6         	ldr	r5, [pc, #0x298]        @ 0x9cd0 <ConnectionManager::removePipelineInfo(LSMessage&)+0x700> literal=0xc00f04f
    9a36: 0000         	movs	r0, r0
    9a38: 77ce         	strb	r6, [r1, #0x1f]
    9a3a: 0001         	movs	r1, r0
    9a3c: 3e8e         	subs	r6, #0x8e
    9a3e: 0000         	movs	r0, r0
    9a40: 7776         	strb	r6, [r6, #0x1d]
    9a42: 0001         	movs	r1, r0
    9a44: 4d78         	ldr	r5, [pc, #0x1e0]        @ 0x9c28 <ConnectionManager::removePipelineInfo(LSMessage&)+0x658> literal=0x81f0e8bd
    9a46: 0000         	movs	r0, r0
    9a48: 4d40         	ldr	r5, [pc, #0x100]        @ 0x9b4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x57c> literal=0xb04f843
    9a4a: 0000         	movs	r0, r0
    9a4c: 3d06         	subs	r5, #0x6
    9a4e: 0000         	movs	r0, r0
    9a50: 6880         	ldr	r0, [r0, #0x8]
    9a52: 4770         	bx	lr
    9a54: 4b07         	ldr	r3, [pc, #0x1c]         @ 0x9a74 <ConnectionManager::removePipelineInfo(LSMessage&)+0x4a4> literal=0x17360
    9a56: 4a08         	ldr	r2, [pc, #0x20]         @ 0x9a78 <ConnectionManager::removePipelineInfo(LSMessage&)+0x4a8> literal=0x22c
    9a58: b510         	push	{r4, lr}
    9a5a: 4604         	mov	r4, r0
    9a5c: 447b         	add	r3, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    9a5e: 589b         	ldr	r3, [r3, r2]
    9a60: 3308         	adds	r3, #0x8
    9a62: f840 3b04    	str	r3, [r0], #4
    9a66: f7fa eb92    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x58dc
    9a6a: 4620         	mov	r0, r4
    9a6c: f7fa e95c    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x5d48
    9a70: 4620         	mov	r0, r4
    9a72: bd10         	pop	{r4, pc}
    9a74: 7360         	strb	r0, [r4, #0xd]
    9a76: 0001         	movs	r1, r0
    9a78: 022c         	lsls	r4, r5, #0x8
    9a7a: 0000         	movs	r0, r0
    9a7c: 4b05         	ldr	r3, [pc, #0x14]         @ 0x9a94 <ConnectionManager::removePipelineInfo(LSMessage&)+0x4c4> literal=0x17338
    9a7e: 4a06         	ldr	r2, [pc, #0x18]         @ 0x9a98 <ConnectionManager::removePipelineInfo(LSMessage&)+0x4c8> literal=0x210
    9a80: b510         	push	{r4, lr}
    9a82: 4604         	mov	r4, r0
    9a84: 447b         	add	r3, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    9a86: 589b         	ldr	r3, [r3, r2]
    9a88: 3308         	adds	r3, #0x8
    9a8a: 6003         	str	r3, [r0]
    9a8c: f7fa ea5e    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x5b44
    9a90: 4620         	mov	r0, r4
    9a92: bd10         	pop	{r4, pc}
    9a94: 7338         	strb	r0, [r7, #0xc]
    9a96: 0001         	movs	r1, r0
    9a98: 0210         	lsls	r0, r2, #0x8
    9a9a: 0000         	movs	r0, r0
    9a9c: 4b07         	ldr	r3, [pc, #0x1c]         @ 0x9abc <ConnectionManager::removePipelineInfo(LSMessage&)+0x4ec> literal=0x17318
    9a9e: 4a08         	ldr	r2, [pc, #0x20]         @ 0x9ac0 <ConnectionManager::removePipelineInfo(LSMessage&)+0x4f0> literal=0x210
    9aa0: b510         	push	{r4, lr}
    9aa2: 4604         	mov	r4, r0
    9aa4: 447b         	add	r3, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    9aa6: 589b         	ldr	r3, [r3, r2]
    9aa8: 3308         	adds	r3, #0x8
    9aaa: 6003         	str	r3, [r0]
    9aac: f7fa ea4e    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x5b64
    9ab0: 4620         	mov	r0, r4
    9ab2: 210c         	movs	r1, #0xc
    9ab4: f7fa ea24    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5bb8
    9ab8: 4620         	mov	r0, r4
    9aba: bd10         	pop	{r4, pc}
    9abc: 7318         	strb	r0, [r3, #0xc]
    9abe: 0001         	movs	r1, r0
    9ac0: 0210         	lsls	r0, r2, #0x8
    9ac2: 0000         	movs	r0, r0
    9ac4: 4b05         	ldr	r3, [pc, #0x14]         @ 0x9adc <ConnectionManager::removePipelineInfo(LSMessage&)+0x50c> literal=0x172f0
    9ac6: 4a06         	ldr	r2, [pc, #0x18]         @ 0x9ae0 <ConnectionManager::removePipelineInfo(LSMessage&)+0x510> literal=0x20c
    9ac8: b510         	push	{r4, lr}
    9aca: 4604         	mov	r4, r0
    9acc: 447b         	add	r3, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    9ace: 589b         	ldr	r3, [r3, r2]
    9ad0: 3308         	adds	r3, #0x8
    9ad2: 6003         	str	r3, [r0]
    9ad4: f7fa ea3a    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x5b8c
    9ad8: 4620         	mov	r0, r4
    9ada: bd10         	pop	{r4, pc}
    9adc: 72f0         	strb	r0, [r6, #0xb]
    9ade: 0001         	movs	r1, r0
    9ae0: 020c         	lsls	r4, r1, #0x8
    9ae2: 0000         	movs	r0, r0
    9ae4: 4b07         	ldr	r3, [pc, #0x1c]         @ 0x9b04 <ConnectionManager::removePipelineInfo(LSMessage&)+0x534> literal=0x172d0
    9ae6: 4a08         	ldr	r2, [pc, #0x20]         @ 0x9b08 <ConnectionManager::removePipelineInfo(LSMessage&)+0x538> literal=0x20c
    9ae8: b510         	push	{r4, lr}
    9aea: 4604         	mov	r4, r0
    9aec: 447b         	add	r3, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    9aee: 589b         	ldr	r3, [r3, r2]
    9af0: 3308         	adds	r3, #0x8
    9af2: 6003         	str	r3, [r0]
    9af4: f7fa ea2a    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x5bac
    9af8: 4620         	mov	r0, r4
    9afa: 210c         	movs	r1, #0xc
    9afc: f7fa ea00    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5c00
    9b00: 4620         	mov	r0, r4
    9b02: bd10         	pop	{r4, pc}
    9b04: 72d0         	strb	r0, [r2, #0xb]
    9b06: 0001         	movs	r1, r0
    9b08: 020c         	lsls	r4, r1, #0x8
    9b0a: 0000         	movs	r0, r0
    9b0c: 4b09         	ldr	r3, [pc, #0x24]         @ 0x9b34 <ConnectionManager::removePipelineInfo(LSMessage&)+0x564> literal=0x172a8
    9b0e: 4a0a         	ldr	r2, [pc, #0x28]         @ 0x9b38 <ConnectionManager::removePipelineInfo(LSMessage&)+0x568> literal=0x22c
    9b10: b510         	push	{r4, lr}
    9b12: 4604         	mov	r4, r0
    9b14: 447b         	add	r3, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    9b16: 589b         	ldr	r3, [r3, r2]
    9b18: 3308         	adds	r3, #0x8
    9b1a: f840 3b04    	str	r3, [r0], #4
    9b1e: f7fa eb36    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x5994
    9b22: 4620         	mov	r0, r4
    9b24: f7fa e900    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x5e00
    9b28: 4620         	mov	r0, r4
    9b2a: 2120         	movs	r1, #0x20
    9b2c: f7fa e9e8    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5c30
    9b30: 4620         	mov	r0, r4
    9b32: bd10         	pop	{r4, pc}
    9b34: 72a8         	strb	r0, [r5, #0xa]
    9b36: 0001         	movs	r1, r0
    9b38: 022c         	lsls	r4, r5, #0x8
    9b3a: 0000         	movs	r0, r0
    9b3c: 4a11         	ldr	r2, [pc, #0x44]         @ 0x9b84 <ConnectionManager::removePipelineInfo(LSMessage&)+0x5b4> literal=0x17278
    9b3e: 4b12         	ldr	r3, [pc, #0x48]         @ 0x9b88 <ConnectionManager::removePipelineInfo(LSMessage&)+0x5b8> literal=0x22c
    9b40: b510         	push	{r4, lr}
    9b42: 4604         	mov	r4, r0
    9b44: 447a         	add	r2, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    9b46: 58d0         	ldr	r0, [r2, r3]
    9b48: 4623         	mov	r3, r4
    9b4a: 3008         	adds	r0, #0x8
    9b4c: f843 0b04    	str	r0, [r3], #4
    9b50: 6848         	ldr	r0, [r1, #0x4]
    9b52: f8d1 c008    	ldr.w	r12, [r1, #0x8]
    9b56: 68ca         	ldr	r2, [r1, #0xc]
    9b58: f8d1 e010    	ldr.w	lr, [r1, #0x10]
    9b5c: 6060         	str	r0, [r4, #0x4]
    9b5e: f8c3 c004    	str.w	r12, [r3, #0x4]
    9b62: 609a         	str	r2, [r3, #0x8]
    9b64: f8c3 e00c    	str.w	lr, [r3, #0xc]
    9b68: 6948         	ldr	r0, [r1, #0x14]
    9b6a: f8d1 c018    	ldr.w	r12, [r1, #0x18]
    9b6e: 69ca         	ldr	r2, [r1, #0x1c]
    9b70: 6118         	str	r0, [r3, #0x10]
    9b72: 1d08         	adds	r0, r1, #0x4
    9b74: f8c3 c014    	str.w	r12, [r3, #0x14]
    9b78: 619a         	str	r2, [r3, #0x18]
    9b7a: f7fa e9d6    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x5c54
    9b7e: 4620         	mov	r0, r4
    9b80: bd10         	pop	{r4, pc}
    9b82: bf00         	nop
    9b84: 7278         	strb	r0, [r7, #0x9]
    9b86: 0001         	movs	r1, r0
    9b88: 022c         	lsls	r4, r5, #0x8
    9b8a: 0000         	movs	r0, r0
    9b8c: 6803         	ldr	r3, [r0]
    9b8e: e92d 41f0    	push.w	{r4, r5, r6, r7, r8, lr}
    9b92: 4606         	mov	r6, r0
    9b94: 4c2a         	ldr	r4, [pc, #0xa8]         @ 0x9c40 <ConnectionManager::removePipelineInfo(LSMessage&)+0x670> literal=0x17224
    9b96: b088         	sub	sp, #0x20
    9b98: 447c         	add	r4, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    9b9a: b1cb         	cbz	r3, 0x9bd0 <ConnectionManager::removePipelineInfo(LSMessage&)+0x600> @ imm = #0x32
    9b9c: 4a29         	ldr	r2, [pc, #0xa4]         @ 0x9c44 <ConnectionManager::removePipelineInfo(LSMessage&)+0x674> literal=0x22c
    9b9e: af01         	add	r7, sp, #0x4
    9ba0: f8d3 8000    	ldr.w	r8, [r3]
    9ba4: 4638         	mov	r0, r7
    9ba6: 58a5         	ldr	r5, [r4, r2]
    9ba8: f105 0308    	add.w	r3, r5, #0x8
    9bac: 9300         	str	r3, [sp]
    9bae: f7fa e9bc    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x5c88
    9bb2: 4b25         	ldr	r3, [pc, #0x94]         @ 0x9c48 <ConnectionManager::removePipelineInfo(LSMessage&)+0x678> literal=0x224
    9bb4: 4640         	mov	r0, r8
    9bb6: 4632         	mov	r2, r6
    9bb8: 58e1         	ldr	r1, [r4, r3]
    9bba: 463b         	mov	r3, r7
    9bbc: f7fa ea7e    	blx	0x40bc <LSCallCancelNotificationRemove@plt> @ imm = #-0x5b04
    9bc0: 4638         	mov	r0, r7
    9bc2: 3508         	adds	r5, #0x8
    9bc4: 9500         	str	r5, [sp]
    9bc6: f7fa eae2    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x5a3c
    9bca: 4668         	mov	r0, sp
    9bcc: f7fa e8ac    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x5ea8
    9bd0: e9d6 5701    	ldrd	r5, r7, [r6, #4]
    9bd4: 42af         	cmp	r7, r5
    9bd6: d10a         	bne	0x9bee <ConnectionManager::removePipelineInfo(LSMessage&)+0x61e> @ imm = #0x14
    9bd8: e015         	b	0x9c06 <ConnectionManager::removePipelineInfo(LSMessage&)+0x636> @ imm = #0x2a
    9bda: 6820         	ldr	r0, [r4]
    9bdc: b108         	cbz	r0, 0x9be2 <ConnectionManager::removePipelineInfo(LSMessage&)+0x612> @ imm = #0x2
    9bde: f7fa ea36    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x5b94
    9be2: 210c         	movs	r1, #0xc
    9be4: 4620         	mov	r0, r4
    9be6: f7fa e98c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5ce8
    9bea: 42af         	cmp	r7, r5
    9bec: d00b         	beq	0x9c06 <ConnectionManager::removePipelineInfo(LSMessage&)+0x636> @ imm = #0x16
    9bee: f855 4b04    	ldr	r4, [r5], #4
    9bf2: 2c00         	cmp	r4, #0x0
    9bf4: d0f9         	beq	0x9bea <ConnectionManager::removePipelineInfo(LSMessage&)+0x61a> @ imm = #-0xe
    9bf6: 68a3         	ldr	r3, [r4, #0x8]
    9bf8: 2b00         	cmp	r3, #0x0
    9bfa: d0ee         	beq	0x9bda <ConnectionManager::removePipelineInfo(LSMessage&)+0x60a> @ imm = #-0x24
    9bfc: 6860         	ldr	r0, [r4, #0x4]
    9bfe: 4621         	mov	r1, r4
    9c00: f7fa e922    	blx	0x3e48 <_ZN2LS17SubscriptionPoint9cleanItemEPNS0_16SubscriptionItemE@plt> @ imm = #-0x5dbc
    9c04: e7e9         	b	0x9bda <ConnectionManager::removePipelineInfo(LSMessage&)+0x60a> @ imm = #-0x2e
    9c06: 6930         	ldr	r0, [r6, #0x10]
    9c08: f106 0318    	add.w	r3, r6, #0x18
    9c0c: 4298         	cmp	r0, r3
    9c0e: d003         	beq	0x9c18 <ConnectionManager::removePipelineInfo(LSMessage&)+0x648> @ imm = #0x6
    9c10: 69b1         	ldr	r1, [r6, #0x18]
    9c12: 3101         	adds	r1, #0x1
    9c14: f7fa e974    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5d18
    9c18: 6870         	ldr	r0, [r6, #0x4]
    9c1a: b118         	cbz	r0, 0x9c24 <ConnectionManager::removePipelineInfo(LSMessage&)+0x654> @ imm = #0x6
    9c1c: 68f1         	ldr	r1, [r6, #0xc]
    9c1e: 1a09         	subs	r1, r1, r0
    9c20: f7fa e96e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5d24
    9c24: 4630         	mov	r0, r6
    9c26: b008         	add	sp, #0x20
    9c28: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
    9c2c: 4668         	mov	r0, sp
    9c2e: f7fa e87c    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x5f08
    9c32: f7fa eae8    	blx	0x4204 <_ZSt9terminatev@plt> @ imm = #-0x5a30
    9c36: 4668         	mov	r0, sp
    9c38: f7ff ff0c    	bl	0x9a54 <ConnectionManager::removePipelineInfo(LSMessage&)+0x484> @ imm = #-0x1e8
    9c3c: e7f9         	b	0x9c32 <ConnectionManager::removePipelineInfo(LSMessage&)+0x662> @ imm = #-0xe
    9c3e: bf00         	nop
    9c40: 7224         	strb	r4, [r4, #0x8]
    9c42: 0001         	movs	r1, r0
    9c44: 022c         	lsls	r4, r5, #0x8
    9c46: 0000         	movs	r0, r0
    9c48: 0224         	lsls	r4, r4, #0x8
    9c4a: 0000         	movs	r0, r0
    9c4c: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    9c50: b095         	sub	sp, #0x54
    9c52: f8df 921c    	ldr.w	r9, [pc, #0x21c]        @ 0x9e70 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8a0> literal=0x1715c
    9c56: 461c         	mov	r4, r3
    9c58: ae03         	add	r6, sp, #0xc
    9c5a: 4680         	mov	r8, r0
    9c5c: 4630         	mov	r0, r6
    9c5e: 468b         	mov	r11, r1
    9c60: 44f9         	add	r9, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    9c62: 4692         	mov	r10, r2
    9c64: f7fa e8a8    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x5eb0
    9c68: 2c00         	cmp	r4, #0x0
    9c6a: f040 80ac    	bne.w	0x9dc6 <ConnectionManager::removePipelineInfo(LSMessage&)+0x7f6> @ imm = #0x158
    9c6e: ac06         	add	r4, sp, #0x18
    9c70: 2100         	movs	r1, #0x0
    9c72: 4620         	mov	r0, r4
    9c74: f7fa e8be    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x5e84
    9c78: 4a7e         	ldr	r2, [pc, #0x1f8]        @ 0x9e74 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8a4> literal=0x3a9a
    9c7a: af0e         	add	r7, sp, #0x38
    9c7c: 463b         	mov	r3, r7
    9c7e: 970c         	str	r7, [sp, #0x30]
    9c80: ad0c         	add	r5, sp, #0x30
    9c82: f04f 0e0b    	mov.w	lr, #0xb
    9c86: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    9c88: f04f 0c00    	mov.w	r12, #0x0
    9c8c: ca07         	ldm	r2, {r0, r1, r2}
    9c8e: c303         	stm	r3!, {r0, r1}
    9c90: 4629         	mov	r1, r5
    9c92: f823 2b02    	strh	r2, [r3], #2
    9c96: 0c12         	lsrs	r2, r2, #0x10
    9c98: 4630         	mov	r0, r6
    9c9a: 701a         	strb	r2, [r3]
    9c9c: 4622         	mov	r2, r4
    9c9e: f8cd e034    	str.w	lr, [sp, #0x34]
    9ca2: f88d c043    	strb.w	r12, [sp, #0x43]
    9ca6: f7fa e816    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x5fd4
    9caa: 980c         	ldr	r0, [sp, #0x30]
    9cac: 42b8         	cmp	r0, r7
    9cae: d003         	beq	0x9cb8 <ConnectionManager::removePipelineInfo(LSMessage&)+0x6e8> @ imm = #0x6
    9cb0: 990e         	ldr	r1, [sp, #0x38]
    9cb2: 3101         	adds	r1, #0x1
    9cb4: f7fa e924    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5db8
    9cb8: 4620         	mov	r0, r4
    9cba: f7fa e84e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5f64
    9cbe: 4659         	mov	r1, r11
    9cc0: 4620         	mov	r0, r4
    9cc2: f7fa ea34    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x5b98
    9cc6: 4a6c         	ldr	r2, [pc, #0x1b0]        @ 0x9e78 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8a8> literal=0x4984
    9cc8: 463b         	mov	r3, r7
    9cca: 970c         	str	r7, [sp, #0x30]
    9ccc: f04f 0e09    	mov.w	lr, #0x9
    9cd0: f04f 0c00    	mov.w	r12, #0x0
    9cd4: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    9cd6: ca07         	ldm	r2, {r0, r1, r2}
    9cd8: c303         	stm	r3!, {r0, r1}
    9cda: 4629         	mov	r1, r5
    9cdc: 701a         	strb	r2, [r3]
    9cde: 4630         	mov	r0, r6
    9ce0: 4622         	mov	r2, r4
    9ce2: f8cd e034    	str.w	lr, [sp, #0x34]
    9ce6: f88d c041    	strb.w	r12, [sp, #0x41]
    9cea: f7f9 eff4    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x6018
    9cee: 980c         	ldr	r0, [sp, #0x30]
    9cf0: 42b8         	cmp	r0, r7
    9cf2: d003         	beq	0x9cfc <ConnectionManager::removePipelineInfo(LSMessage&)+0x72c> @ imm = #0x6
    9cf4: 990e         	ldr	r1, [sp, #0x38]
    9cf6: 3101         	adds	r1, #0x1
    9cf8: f7fa e902    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5dfc
    9cfc: 4620         	mov	r0, r4
    9cfe: f7fa e82c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5fa8
    9d02: 4651         	mov	r1, r10
    9d04: 4620         	mov	r0, r4
    9d06: f7fa ea18    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x5bd0
    9d0a: 4a5c         	ldr	r2, [pc, #0x170]        @ 0x9e7c <ConnectionManager::removePipelineInfo(LSMessage&)+0x8ac> literal=0x494c
    9d0c: 463b         	mov	r3, r7
    9d0e: 970c         	str	r7, [sp, #0x30]
    9d10: f04f 0e09    	mov.w	lr, #0x9
    9d14: f04f 0c00    	mov.w	r12, #0x0
    9d18: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    9d1a: ca07         	ldm	r2, {r0, r1, r2}
    9d1c: c303         	stm	r3!, {r0, r1}
    9d1e: 4629         	mov	r1, r5
    9d20: 701a         	strb	r2, [r3]
    9d22: 4630         	mov	r0, r6
    9d24: 4622         	mov	r2, r4
    9d26: f8cd e034    	str.w	lr, [sp, #0x34]
    9d2a: f88d c041    	strb.w	r12, [sp, #0x41]
    9d2e: f7f9 efd2    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x605c
    9d32: 980c         	ldr	r0, [sp, #0x30]
    9d34: 42b8         	cmp	r0, r7
    9d36: d003         	beq	0x9d40 <ConnectionManager::removePipelineInfo(LSMessage&)+0x770> @ imm = #0x6
    9d38: 990e         	ldr	r1, [sp, #0x38]
    9d3a: 3101         	adds	r1, #0x1
    9d3c: f7fa e8e0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5e40
    9d40: 4620         	mov	r0, r4
    9d42: f10d 0a20    	add.w	r10, sp, #0x20
    9d46: f7fa e808    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x5ff0
    9d4a: 2100         	movs	r1, #0x0
    9d4c: 4628         	mov	r0, r5
    9d4e: 9107         	str	r1, [sp, #0x1c]
    9d50: f88d 1020    	strb.w	r1, [sp, #0x20]
    9d54: f8cd a018    	str.w	r10, [sp, #0x18]
    9d58: f7fa e97e    	blx	0x4058 <_ZN7pbnjson10JGeneratorC1EPNS_9JResolverE@plt> @ imm = #-0x5d04
    9d5c: f7fa e970    	blx	0x4040 <_ZN7pbnjson7JSchema9AllSchemaEv@plt> @ imm = #-0x5d20
    9d60: 4602         	mov	r2, r0
    9d62: 4623         	mov	r3, r4
    9d64: 4631         	mov	r1, r6
    9d66: 4628         	mov	r0, r5
    9d68: f7fa e996    	blx	0x4098 <_ZN7pbnjson10JGenerator8toStringERKNS_6JValueERKNS_7JSchemaERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x5cd4
    9d6c: 4628         	mov	r0, r5
    9d6e: af0d         	add	r7, sp, #0x34
    9d70: f7fa e95a    	blx	0x4028 <_ZN7pbnjson10JGeneratorD1Ev@plt> @ imm = #-0x5d4c
    9d74: 4b42         	ldr	r3, [pc, #0x108]        @ 0x9e80 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8b0> literal=0x22c
    9d76: 4638         	mov	r0, r7
    9d78: f8dd b018    	ldr.w	r11, [sp, #0x18]
    9d7c: f859 3003    	ldr.w	r3, [r9, r3]
    9d80: 9301         	str	r3, [sp, #0x4]
    9d82: 3308         	adds	r3, #0x8
    9d84: 930c         	str	r3, [sp, #0x30]
    9d86: f7fa e8d0    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x5e60
    9d8a: f8d8 0000    	ldr.w	r0, [r8]
    9d8e: 4659         	mov	r1, r11
    9d90: 463a         	mov	r2, r7
    9d92: f7fa ea4a    	blx	0x4228 <LSMessageRespond@plt> @ imm = #-0x5b6c
    9d96: 2800         	cmp	r0, #0x0
    9d98: d046         	beq	0x9e28 <ConnectionManager::removePipelineInfo(LSMessage&)+0x858> @ imm = #0x8c
    9d9a: 9b01         	ldr	r3, [sp, #0x4]
    9d9c: 4638         	mov	r0, r7
    9d9e: 3308         	adds	r3, #0x8
    9da0: 930c         	str	r3, [sp, #0x30]
    9da2: f7fa e9f4    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x5c18
    9da6: 4628         	mov	r0, r5
    9da8: f7f9 efbe    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x6084
    9dac: 9806         	ldr	r0, [sp, #0x18]
    9dae: 4550         	cmp	r0, r10
    9db0: d003         	beq	0x9dba <ConnectionManager::removePipelineInfo(LSMessage&)+0x7ea> @ imm = #0x6
    9db2: 9908         	ldr	r1, [sp, #0x20]
    9db4: 3101         	adds	r1, #0x1
    9db6: f7fa e8a4    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5eb8
    9dba: 4630         	mov	r0, r6
    9dbc: f7f9 efcc    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6068
    9dc0: b015         	add	sp, #0x54
    9dc2: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    9dc6: ac06         	add	r4, sp, #0x18
    9dc8: 2100         	movs	r1, #0x0
    9dca: 4620         	mov	r0, r4
    9dcc: f7fa e812    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x5fdc
    9dd0: 4a2c         	ldr	r2, [pc, #0xb0]         @ 0x9e84 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8b4> literal=0x486e
    9dd2: af0e         	add	r7, sp, #0x38
    9dd4: ad0c         	add	r5, sp, #0x30
    9dd6: 463b         	mov	r3, r7
    9dd8: 970c         	str	r7, [sp, #0x30]
    9dda: f04f 0e0a    	mov.w	lr, #0xa
    9dde: 447a         	add	r2, pc => raw=0xe650 ghidra=0x1e650 file=58960 "subscribed"
    9de0: f04f 0c00    	mov.w	r12, #0x0
    9de4: ca07         	ldm	r2, {r0, r1, r2}
    9de6: c303         	stm	r3!, {r0, r1}
    9de8: 4629         	mov	r1, r5
    9dea: 801a         	strh	r2, [r3]
    9dec: 4630         	mov	r0, r6
    9dee: 4622         	mov	r2, r4
    9df0: f8cd e034    	str.w	lr, [sp, #0x34]
    9df4: f88d c042    	strb.w	r12, [sp, #0x42]
    9df8: f7f9 ef6c    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x6128
    9dfc: 980c         	ldr	r0, [sp, #0x30]
    9dfe: 42b8         	cmp	r0, r7
    9e00: d003         	beq	0x9e0a <ConnectionManager::removePipelineInfo(LSMessage&)+0x83a> @ imm = #0x6
    9e02: 990e         	ldr	r1, [sp, #0x38]
    9e04: 3101         	adds	r1, #0x1
    9e06: f7fa e87c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x5f08
    9e0a: 4620         	mov	r0, r4
    9e0c: f7f9 efa4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x60b8
    9e10: e72e         	b	0x9c70 <ConnectionManager::removePipelineInfo(LSMessage&)+0x6a0> @ imm = #-0x1a4
    9e12: 4628         	mov	r0, r5
    9e14: f7fa e96c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x5d28
    9e18: 4620         	mov	r0, r4
    9e1a: f7f9 ef9e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x60c4
    9e1e: 4630         	mov	r0, r6
    9e20: f7f9 ef9a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x60cc
    9e24: f7fa e9ac    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x5ca8
    9e28: 2020         	movs	r0, #0x20
    9e2a: f7fa e82c    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0x5fa8
    9e2e: 4629         	mov	r1, r5
    9e30: 4607         	mov	r7, r0
    9e32: f7ff fe83    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #-0x2fa
    9e36: 4b14         	ldr	r3, [pc, #0x50]         @ 0x9e88 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8b8> literal=0x228
    9e38: 4638         	mov	r0, r7
    9e3a: f859 2003    	ldr.w	r2, [r9, r3]
    9e3e: 4b13         	ldr	r3, [pc, #0x4c]         @ 0x9e8c <ConnectionManager::removePipelineInfo(LSMessage&)+0x8bc> literal=0x208
    9e40: f859 1003    	ldr.w	r1, [r9, r3]
    9e44: f7fa e8d0    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x5e60
    9e48: 4628         	mov	r0, r5
    9e4a: f7ff fe03    	bl	0x9a54 <ConnectionManager::removePipelineInfo(LSMessage&)+0x484> @ imm = #-0x3fa
    9e4e: 4620         	mov	r0, r4
    9e50: f7fa e94e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x5d64
    9e54: e7e3         	b	0x9e1e <ConnectionManager::removePipelineInfo(LSMessage&)+0x84e> @ imm = #-0x3a
    9e56: 4628         	mov	r0, r5
    9e58: f7f9 ef66    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x6134
    9e5c: e7f7         	b	0x9e4e <ConnectionManager::removePipelineInfo(LSMessage&)+0x87e> @ imm = #-0x12
    9e5e: 4628         	mov	r0, r5
    9e60: f7fa e8e2    	blx	0x4028 <_ZN7pbnjson10JGeneratorD1Ev@plt> @ imm = #-0x5e3c
    9e64: e7f3         	b	0x9e4e <ConnectionManager::removePipelineInfo(LSMessage&)+0x87e> @ imm = #-0x1a
    9e66: e7f2         	b	0x9e4e <ConnectionManager::removePipelineInfo(LSMessage&)+0x87e> @ imm = #-0x1c
    9e68: e7d3         	b	0x9e12 <ConnectionManager::removePipelineInfo(LSMessage&)+0x842> @ imm = #-0x5a
    9e6a: e7d2         	b	0x9e12 <ConnectionManager::removePipelineInfo(LSMessage&)+0x842> @ imm = #-0x5c
    9e6c: e7d1         	b	0x9e12 <ConnectionManager::removePipelineInfo(LSMessage&)+0x842> @ imm = #-0x5e
    9e6e: e7d6         	b	0x9e1e <ConnectionManager::removePipelineInfo(LSMessage&)+0x84e> @ imm = #-0x54
    9e70: 715c         	strb	r4, [r3, #0x5]
    9e72: 0001         	movs	r1, r0
    9e74: 3a9a         	subs	r2, #0x9a
    9e76: 0000         	movs	r0, r0
    9e78: 4984         	ldr	r1, [pc, #0x210]        @ 0xa08c <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x44> literal=0x8196f040
    9e7a: 0000         	movs	r0, r0
    9e7c: 494c         	ldr	r1, [pc, #0x130]        @ 0x9fb0 <ConnectionManager::removePipelineInfo(LSMessage&)+0x9e0> literal=0xf7ff4620
    9e7e: 0000         	movs	r0, r0
    9e80: 022c         	lsls	r4, r5, #0x8
    9e82: 0000         	movs	r0, r0
    9e84: 486e         	ldr	r0, [pc, #0x1b8]        @ 0xa040 <ConnectionManager::removePipelineInfo(LSMessage&)+0xa70> literal=0x4630ef60
    9e86: 0000         	movs	r0, r0
    9e88: 0228         	lsls	r0, r5, #0x8
    9e8a: 0000         	movs	r0, r0
    9e8c: 0208         	lsls	r0, r1, #0x8
    9e8e: 0000         	movs	r0, r0
    9e90: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    9e94: b097         	sub	sp, #0x5c
    9e96: f8df 9138    	ldr.w	r9, [pc, #0x138]        @ 0x9fd0 <ConnectionManager::removePipelineInfo(LSMessage&)+0xa00> literal=0x16f12
    9e9a: 2300         	movs	r3, #0x0
    9e9c: ac0e         	add	r4, sp, #0x38
    9e9e: f10d 0828    	add.w	r8, sp, #0x28
    9ea2: 4607         	mov	r7, r0
    9ea4: 460d         	mov	r5, r1
    9ea6: 4620         	mov	r0, r4
    9ea8: 4619         	mov	r1, r3
    9eaa: 44f9         	add	r9, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    9eac: e9cd 8308    	strd	r8, r3, [sp, #32]
    9eb0: f88d 3028    	strb.w	r3, [sp, #0x28]
    9eb4: f7fa e8d0    	blx	0x4058 <_ZN7pbnjson10JGeneratorC1EPNS_9JResolverE@plt> @ imm = #-0x5e60
    9eb8: f10d 0a20    	add.w	r10, sp, #0x20
    9ebc: f7fa e8c0    	blx	0x4040 <_ZN7pbnjson7JSchema9AllSchemaEv@plt> @ imm = #-0x5e80
    9ec0: 4602         	mov	r2, r0
    9ec2: 4629         	mov	r1, r5
    9ec4: 4653         	mov	r3, r10
    9ec6: 4620         	mov	r0, r4
    9ec8: f7fa e8e6    	blx	0x4098 <_ZN7pbnjson10JGenerator8toStringERKNS_6JValueERKNS_7JSchemaERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x5e34
    9ecc: 4620         	mov	r0, r4
    9ece: ae0f         	add	r6, sp, #0x3c
    9ed0: f7fa e8aa    	blx	0x4028 <_ZN7pbnjson10JGeneratorD1Ev@plt> @ imm = #-0x5eac
    9ed4: 4b3f         	ldr	r3, [pc, #0xfc]         @ 0x9fd4 <ConnectionManager::removePipelineInfo(LSMessage&)+0xa04> literal=0x22c
    9ed6: 4630         	mov	r0, r6
    9ed8: f8dd b020    	ldr.w	r11, [sp, #0x20]
    9edc: f859 5003    	ldr.w	r5, [r9, r3]
    9ee0: f105 0308    	add.w	r3, r5, #0x8
    9ee4: 930e         	str	r3, [sp, #0x38]
    9ee6: f7fa e820    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x5fc0
    9eea: 6838         	ldr	r0, [r7]
    9eec: 4659         	mov	r1, r11
    9eee: 4632         	mov	r2, r6
    9ef0: f7fa e99a    	blx	0x4228 <LSMessageRespond@plt> @ imm = #-0x5ccc
    9ef4: b1a0         	cbz	r0, 0x9f20 <ConnectionManager::removePipelineInfo(LSMessage&)+0x950> @ imm = #0x28
    9ef6: 4630         	mov	r0, r6
    9ef8: 3508         	adds	r5, #0x8
    9efa: 950e         	str	r5, [sp, #0x38]
    9efc: f7fa e946    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x5d74
    9f00: 4620         	mov	r0, r4
    9f02: f7f9 ef12    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x61dc
    9f06: 9808         	ldr	r0, [sp, #0x20]
    9f08: 4540         	cmp	r0, r8
    9f0a: d003         	beq	0x9f14 <ConnectionManager::removePipelineInfo(LSMessage&)+0x944> @ imm = #0x6
    9f0c: 990a         	ldr	r1, [sp, #0x28]
    9f0e: 3101         	adds	r1, #0x1
    9f10: f7f9 eff6    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6014
    9f14: b017         	add	sp, #0x5c
    9f16: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    9f1a: f10d 0a20    	add.w	r10, sp, #0x20
    9f1e: e040         	b	0x9fa2 <ConnectionManager::removePipelineInfo(LSMessage&)+0x9d2> @ imm = #0x80
    9f20: 2020         	movs	r0, #0x20
    9f22: f7f9 efb0    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0x60a0
    9f26: 4621         	mov	r1, r4
    9f28: 4605         	mov	r5, r0
    9f2a: f7ff fe07    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #-0x3f2
    9f2e: 4b2a         	ldr	r3, [pc, #0xa8]         @ 0x9fd8 <ConnectionManager::removePipelineInfo(LSMessage&)+0xa08> literal=0x228
    9f30: 4628         	mov	r0, r5
    9f32: f859 2003    	ldr.w	r2, [r9, r3]
    9f36: 4b29         	ldr	r3, [pc, #0xa4]         @ 0x9fdc <ConnectionManager::removePipelineInfo(LSMessage&)+0xa0c> literal=0x208
    9f38: f859 1003    	ldr.w	r1, [r9, r3]
    9f3c: f7fa e854    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x5f58
    9f40: 4605         	mov	r5, r0
    9f42: 460e         	mov	r6, r1
    9f44: 4620         	mov	r0, r4
    9f46: f7f9 eef0    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x6220
    9f4a: 4628         	mov	r0, r5
    9f4c: 4633         	mov	r3, r6
    9f4e: 2b01         	cmp	r3, #0x1
    9f50: d127         	bne	0x9fa2 <ConnectionManager::removePipelineInfo(LSMessage&)+0x9d2> @ imm = #0x4e
    9f52: f7f9 eede    	blx	0x3d10 <__cxa_begin_catch@plt> @ imm = #-0x6244
    9f56: 4604         	mov	r4, r0
    9f58: f000 faf0    	bl	0xa53c <getPmLogContext()> @ imm = #0x5e0
    9f5c: bb88         	cbnz	r0, 0x9fc2 <ConnectionManager::removePipelineInfo(LSMessage&)+0x9f2> @ imm = #0x62
    9f5e: f000 faed    	bl	0xa53c <getPmLogContext()> @ imm = #0x5da
    9f62: 6822         	ldr	r2, [r4]
    9f64: 4603         	mov	r3, r0
    9f66: 4620         	mov	r0, r4
    9f68: 461c         	mov	r4, r3
    9f6a: 6893         	ldr	r3, [r2, #0x8]
    9f6c: 4798         	blx	r3
    9f6e: 4b1c         	ldr	r3, [pc, #0x70]         @ 0x9fe0 <ConnectionManager::removePipelineInfo(LSMessage&)+0xa10> literal=0x46fa
    9f70: 4602         	mov	r2, r0
    9f72: 2107         	movs	r1, #0x7
    9f74: 4620         	mov	r0, r4
    9f76: 447b         	add	r3, pc => raw=0xe674 ghidra=0x1e674 file=58996 "postToClient"
    9f78: e9cd 3205    	strd	r3, r2, [sp, #20]
    9f7c: 4a19         	ldr	r2, [pc, #0x64]         @ 0x9fe4 <ConnectionManager::removePipelineInfo(LSMessage&)+0xa14> literal=0x4700
    9f7e: 4b1a         	ldr	r3, [pc, #0x68]         @ 0x9fe8 <ConnectionManager::removePipelineInfo(LSMessage&)+0xa18> literal=0x474a
    9f80: 447a         	add	r2, pc => raw=0xe684 ghidra=0x1e684 file=59012 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/include/utils.h"
    9f82: 447b         	add	r3, pc => raw=0xe6d0 ghidra=0x1e6d0 file=59088 "%s:%s() Error  %s"
    9f84: e9cd 3203    	strd	r3, r2, [sp, #12]
    9f88: 2300         	movs	r3, #0x0
    9f8a: 461a         	mov	r2, r3
    9f8c: e9cd 3301    	strd	r3, r3, [sp, #4]
    9f90: 9300         	str	r3, [sp]
    9f92: f7fa e844    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x5f78
    9f96: f7fa e88c    	blx	0x40b0 <__cxa_end_catch@plt> @ imm = #-0x5ee8
    9f9a: e7b4         	b	0x9f06 <ConnectionManager::removePipelineInfo(LSMessage&)+0x936> @ imm = #-0x98
    9f9c: 4620         	mov	r0, r4
    9f9e: f7fa e844    	blx	0x4028 <_ZN7pbnjson10JGeneratorD1Ev@plt> @ imm = #-0x5f78
    9fa2: 4650         	mov	r0, r10
    9fa4: f7fa e8a4    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x5eb8
    9fa8: f7fa e8ea    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x5e2c
    9fac: 4605         	mov	r5, r0
    9fae: 460e         	mov	r6, r1
    9fb0: 4620         	mov	r0, r4
    9fb2: f7ff fd4f    	bl	0x9a54 <ConnectionManager::removePipelineInfo(LSMessage&)+0x484> @ imm = #-0x562
    9fb6: 4628         	mov	r0, r5
    9fb8: 4633         	mov	r3, r6
    9fba: e7c8         	b	0x9f4e <ConnectionManager::removePipelineInfo(LSMessage&)+0x97e> @ imm = #-0x70
    9fbc: f7fa e878    	blx	0x40b0 <__cxa_end_catch@plt> @ imm = #-0x5f10
    9fc0: e7ef         	b	0x9fa2 <ConnectionManager::removePipelineInfo(LSMessage&)+0x9d2> @ imm = #-0x22
    9fc2: f000 fabb    	bl	0xa53c <getPmLogContext()> @ imm = #0x576
    9fc6: 6803         	ldr	r3, [r0]
    9fc8: 2b06         	cmp	r3, #0x6
    9fca: dde4         	ble	0x9f96 <ConnectionManager::removePipelineInfo(LSMessage&)+0x9c6> @ imm = #-0x38
    9fcc: e7c7         	b	0x9f5e <ConnectionManager::removePipelineInfo(LSMessage&)+0x98e> @ imm = #-0x72
    9fce: bf00         	nop
    9fd0: 6f12         	ldr	r2, [r2, #0x70]
    9fd2: 0001         	movs	r1, r0
    9fd4: 022c         	lsls	r4, r5, #0x8
    9fd6: 0000         	movs	r0, r0
    9fd8: 0228         	lsls	r0, r5, #0x8
    9fda: 0000         	movs	r0, r0
    9fdc: 0208         	lsls	r0, r1, #0x8
    9fde: 0000         	movs	r0, r0
    9fe0: 46fa         	mov	r10, pc
    9fe2: 0000         	movs	r0, r0
    9fe4: 4700         	bx	r0
    9fe6: 0000         	movs	r0, r0
    9fe8: 474a         	bx	r9
    9fea: 0000         	movs	r0, r0
    9fec: b570         	push	{r4, r5, r6, lr}
    9fee: 4606         	mov	r6, r0
    9ff0: e9d0 4500    	ldrd	r4, r5, [r0]
    9ff4: 42ac         	cmp	r4, r5
    9ff6: d01e         	beq	0xa036 <ConnectionManager::removePipelineInfo(LSMessage&)+0xa66> @ imm = #0x3c
    9ff8: 6b20         	ldr	r0, [r4, #0x30]
    9ffa: f104 0338    	add.w	r3, r4, #0x38
    9ffe: 4298         	cmp	r0, r3
    a000: d003         	beq	0xa00a <ConnectionManager::removePipelineInfo(LSMessage&)+0xa3a> @ imm = #0x6
    a002: 6ba1         	ldr	r1, [r4, #0x38]
    a004: 3101         	adds	r1, #0x1
    a006: f7f9 ef7c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6108
    a00a: 69a0         	ldr	r0, [r4, #0x18]
    a00c: f104 0320    	add.w	r3, r4, #0x20
    a010: 4298         	cmp	r0, r3
    a012: d003         	beq	0xa01c <ConnectionManager::removePipelineInfo(LSMessage&)+0xa4c> @ imm = #0x6
    a014: 6a21         	ldr	r1, [r4, #0x20]
    a016: 3101         	adds	r1, #0x1
    a018: f7f9 ef72    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x611c
    a01c: 4623         	mov	r3, r4
    a01e: f853 0b08    	ldr	r0, [r3], #8
    a022: 4298         	cmp	r0, r3
    a024: d003         	beq	0xa02e <ConnectionManager::removePipelineInfo(LSMessage&)+0xa5e> @ imm = #0x6
    a026: 68a1         	ldr	r1, [r4, #0x8]
    a028: 3101         	adds	r1, #0x1
    a02a: f7f9 ef6a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x612c
    a02e: 344c         	adds	r4, #0x4c
    a030: 42a5         	cmp	r5, r4
    a032: d1e1         	bne	0x9ff8 <ConnectionManager::removePipelineInfo(LSMessage&)+0xa28> @ imm = #-0x3e
    a034: 6835         	ldr	r5, [r6]
    a036: b125         	cbz	r5, 0xa042 <ConnectionManager::removePipelineInfo(LSMessage&)+0xa72> @ imm = #0x8
    a038: 68b1         	ldr	r1, [r6, #0x8]
    a03a: 4628         	mov	r0, r5
    a03c: 1b49         	subs	r1, r1, r5
    a03e: f7f9 ef60    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6140
    a042: 4630         	mov	r0, r6
    a044: bd70         	pop	{r4, r5, r6, pc}
    a046: bf00         	nop

0000a048 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)>:
    a048: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    a04c: 4615         	mov	r5, r2
    a04e: e9d0 a700    	ldrd	r10, r7, [r0]
    a052: f64c 221b    	movw	r2, #0xca1b
    a056: f6c2 026b    	movt	r2, #0x286b
    a05a: b085         	sub	sp, #0x14
    a05c: 9002         	str	r0, [sp, #0x8]
    a05e: eba7 030a    	sub.w	r3, r7, r10
    a062: 109b         	asrs	r3, r3, #0x2
    a064: fb02 f303    	mul	r3, r2, r3
    a068: f642 026b    	movw	r2, #0x286b
    a06c: f2c0 12af    	movt	r2, #0x1af
    a070: 4293         	cmp	r3, r2
    a072: f000 81aa    	beq.w	0xa3ca <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x382> @ imm = #0x354
    a076: 2b01         	cmp	r3, #0x1
    a078: 4688         	mov	r8, r1
    a07a: 4619         	mov	r1, r3
    a07c: bf38         	it	lo
    a07e: 2101         	movlo	r1, #0x1
    a080: 185b         	adds	r3, r3, r1
    a082: eba8 060a    	sub.w	r6, r8, r10
    a086: f080 8171    	bhs.w	0xa36c <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x324> @ imm = #0x2e2
    a08a: 2b00         	cmp	r3, #0x0
    a08c: f040 8196    	bne.w	0xa3bc <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x374> @ imm = #0x32c
    a090: 244c         	movs	r4, #0x4c
    a092: 9303         	str	r3, [sp, #0xc]
    a094: 9301         	str	r3, [sp, #0x4]
    a096: 9a01         	ldr	r2, [sp, #0x4]
    a098: 4629         	mov	r1, r5
    a09a: f851 cb08    	ldr	r12, [r1], #8
    a09e: 4613         	mov	r3, r2
    a0a0: 4433         	add	r3, r6
    a0a2: 458c         	cmp	r12, r1
    a0a4: f103 0008    	add.w	r0, r3, #0x8
    a0a8: 5190         	str	r0, [r2, r6]
    a0aa: f000 8175    	beq.w	0xa398 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x350> @ imm = #0x2ea
    a0ae: 9a01         	ldr	r2, [sp, #0x4]
    a0b0: f842 c006    	str.w	r12, [r2, r6]
    a0b4: 68aa         	ldr	r2, [r5, #0x8]
    a0b6: 609a         	str	r2, [r3, #0x8]
    a0b8: 686a         	ldr	r2, [r5, #0x4]
    a0ba: 69a8         	ldr	r0, [r5, #0x18]
    a0bc: 6029         	str	r1, [r5]
    a0be: f105 0120    	add.w	r1, r5, #0x20
    a0c2: 605a         	str	r2, [r3, #0x4]
    a0c4: 2200         	movs	r2, #0x0
    a0c6: 4288         	cmp	r0, r1
    a0c8: 606a         	str	r2, [r5, #0x4]
    a0ca: 722a         	strb	r2, [r5, #0x8]
    a0cc: f103 0220    	add.w	r2, r3, #0x20
    a0d0: 619a         	str	r2, [r3, #0x18]
    a0d2: f000 816a    	beq.w	0xa3aa <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x362> @ imm = #0x2d4
    a0d6: 6a2a         	ldr	r2, [r5, #0x20]
    a0d8: 6198         	str	r0, [r3, #0x18]
    a0da: 621a         	str	r2, [r3, #0x20]
    a0dc: 69ea         	ldr	r2, [r5, #0x1c]
    a0de: 6b28         	ldr	r0, [r5, #0x30]
    a0e0: 61a9         	str	r1, [r5, #0x18]
    a0e2: f105 0138    	add.w	r1, r5, #0x38
    a0e6: 61da         	str	r2, [r3, #0x1c]
    a0e8: 2200         	movs	r2, #0x0
    a0ea: 4288         	cmp	r0, r1
    a0ec: 61ea         	str	r2, [r5, #0x1c]
    a0ee: f885 2020    	strb.w	r2, [r5, #0x20]
    a0f2: f103 0238    	add.w	r2, r3, #0x38
    a0f6: 631a         	str	r2, [r3, #0x30]
    a0f8: f000 8145    	beq.w	0xa386 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x33e> @ imm = #0x28a
    a0fc: 6baa         	ldr	r2, [r5, #0x38]
    a0fe: 6318         	str	r0, [r3, #0x30]
    a100: 639a         	str	r2, [r3, #0x38]
    a102: 6b6a         	ldr	r2, [r5, #0x34]
    a104: 45d0         	cmp	r8, r10
    a106: f04f 0b00    	mov.w	r11, #0x0
    a10a: 6329         	str	r1, [r5, #0x30]
    a10c: f8c5 b034    	str.w	r11, [r5, #0x34]
    a110: 635a         	str	r2, [r3, #0x34]
    a112: f895 2048    	ldrb.w	r2, [r5, #0x48]
    a116: f885 b038    	strb.w	r11, [r5, #0x38]
    a11a: f883 2048    	strb.w	r2, [r3, #0x48]
    a11e: f000 8097    	beq.w	0xa250 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x208> @ imm = #0x12e
    a122: 9b01         	ldr	r3, [sp, #0x4]
    a124: f10a 0608    	add.w	r6, r10, #0x8
    a128: f10a 0438    	add.w	r4, r10, #0x38
    a12c: f103 054c    	add.w	r5, r3, #0x4c
    a130: e057         	b	0xa1e2 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x19a> @ imm = #0xae
    a132: f845 3c4c    	str	r3, [r5, #-76]
    a136: f854 3c30    	ldr	r3, [r4, #-48]
    a13a: f845 3c44    	str	r3, [r5, #-68]
    a13e: f854 3c34    	ldr	r3, [r4, #-52]
    a142: f1a5 012c    	sub.w	r1, r5, #0x2c
    a146: e944 6b0e    	strd	r6, r11, [r4, #-56]
    a14a: f845 3c48    	str	r3, [r5, #-72]
    a14e: f1a4 0318    	sub.w	r3, r4, #0x18
    a152: f804 bc30    	strb	r11, [r4, #-48]
    a156: f845 1c34    	str	r1, [r5, #-52]
    a15a: f854 2c20    	ldr	r2, [r4, #-32]
    a15e: 429a         	cmp	r2, r3
    a160: d065         	beq	0xa22e <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x1e6> @ imm = #0xca
    a162: f845 2c34    	str	r2, [r5, #-52]
    a166: f854 2c18    	ldr	r2, [r4, #-24]
    a16a: f845 2c2c    	str	r2, [r5, #-44]
    a16e: f854 2c1c    	ldr	r2, [r4, #-28]
    a172: f1a5 0114    	sub.w	r1, r5, #0x14
    a176: f844 3c20    	str	r3, [r4, #-32]
    a17a: f844 bc1c    	str	r11, [r4, #-28]
    a17e: f845 2c30    	str	r2, [r5, #-48]
    a182: f804 bc18    	strb	r11, [r4, #-24]
    a186: f845 1c1c    	str	r1, [r5, #-28]
    a18a: f854 2c08    	ldr	r2, [r4, #-8]
    a18e: 4294         	cmp	r4, r2
    a190: d03e         	beq	0xa210 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x1c8> @ imm = #0x7c
    a192: f845 2c1c    	str	r2, [r5, #-28]
    a196: 6822         	ldr	r2, [r4]
    a198: f845 2c14    	str	r2, [r5, #-20]
    a19c: f854 2c04    	ldr	r2, [r4, #-4]
    a1a0: f854 0c20    	ldr	r0, [r4, #-32]
    a1a4: f845 2c18    	str	r2, [r5, #-24]
    a1a8: 7c22         	ldrb	r2, [r4, #0x10]
    a1aa: 4298         	cmp	r0, r3
    a1ac: f805 2c04    	strb	r2, [r5, #-4]
    a1b0: d004         	beq	0xa1bc <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x174> @ imm = #0x8
    a1b2: f854 1c18    	ldr	r1, [r4, #-24]
    a1b6: 3101         	adds	r1, #0x1
    a1b8: f7f9 eea2    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x62bc
    a1bc: f854 0c38    	ldr	r0, [r4, #-56]
    a1c0: 4286         	cmp	r6, r0
    a1c2: d004         	beq	0xa1ce <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x186> @ imm = #0x8
    a1c4: f854 1c30    	ldr	r1, [r4, #-48]
    a1c8: 3101         	adds	r1, #0x1
    a1ca: f7f9 ee9a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x62cc
    a1ce: f104 034c    	add.w	r3, r4, #0x4c
    a1d2: 3414         	adds	r4, #0x14
    a1d4: 45a0         	cmp	r8, r4
    a1d6: f105 054c    	add.w	r5, r5, #0x4c
    a1da: f106 064c    	add.w	r6, r6, #0x4c
    a1de: d035         	beq	0xa24c <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x204> @ imm = #0x6a
    a1e0: 461c         	mov	r4, r3
    a1e2: f1a5 0244    	sub.w	r2, r5, #0x44
    a1e6: 46a9         	mov	r9, r5
    a1e8: f845 2c4c    	str	r2, [r5, #-76]
    a1ec: f854 3c38    	ldr	r3, [r4, #-56]
    a1f0: 42b3         	cmp	r3, r6
    a1f2: d19e         	bne	0xa132 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0xea> @ imm = #-0xc4
    a1f4: f8d6 c000    	ldr.w	r12, [r6]
    a1f8: 6870         	ldr	r0, [r6, #0x4]
    a1fa: 68b1         	ldr	r1, [r6, #0x8]
    a1fc: 68f3         	ldr	r3, [r6, #0xc]
    a1fe: f845 cc44    	str	r12, [r5, #-68]
    a202: f845 0c40    	str	r0, [r5, #-64]
    a206: f845 1c3c    	str	r1, [r5, #-60]
    a20a: f845 3c38    	str	r3, [r5, #-56]
    a20e: e796         	b	0xa13e <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0xf6> @ imm = #-0xd4
    a210: f8d4 e000    	ldr.w	lr, [r4]
    a214: f8d4 c004    	ldr.w	r12, [r4, #0x4]
    a218: 68a0         	ldr	r0, [r4, #0x8]
    a21a: 68e2         	ldr	r2, [r4, #0xc]
    a21c: f845 ec14    	str	lr, [r5, #-20]
    a220: f845 cc10    	str	r12, [r5, #-16]
    a224: f845 0c0c    	str	r0, [r5, #-12]
    a228: f845 2c08    	str	r2, [r5, #-8]
    a22c: e7b6         	b	0xa19c <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x154> @ imm = #-0x94
    a22e: f854 ec18    	ldr	lr, [r4, #-24]
    a232: f8d3 c004    	ldr.w	r12, [r3, #0x4]
    a236: 6898         	ldr	r0, [r3, #0x8]
    a238: 68da         	ldr	r2, [r3, #0xc]
    a23a: f845 ec2c    	str	lr, [r5, #-44]
    a23e: f845 cc28    	str	r12, [r5, #-40]
    a242: f845 0c24    	str	r0, [r5, #-36]
    a246: f845 2c20    	str	r2, [r5, #-32]
    a24a: e790         	b	0xa16e <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x126> @ imm = #-0xe0
    a24c: f109 044c    	add.w	r4, r9, #0x4c
    a250: 45b8         	cmp	r8, r7
    a252: d078         	beq	0xa346 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x2fe> @ imm = #0xf0
    a254: f104 0208    	add.w	r2, r4, #0x8
    a258: f108 0338    	add.w	r3, r8, #0x38
    a25c: 4640         	mov	r0, r8
    a25e: 2500         	movs	r5, #0x0
    a260: e03e         	b	0xa2e0 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x298> @ imm = #0x7c
    a262: f842 1c08    	str	r1, [r2, #-8]
    a266: f853 1c30    	ldr	r1, [r3, #-48]
    a26a: 6011         	str	r1, [r2]
    a26c: f853 1c34    	ldr	r1, [r3, #-52]
    a270: f102 0c18    	add.w	r12, r2, #0x18
    a274: f843 6c38    	str	r6, [r3, #-56]
    a278: f100 0620    	add.w	r6, r0, #0x20
    a27c: f803 5c30    	strb	r5, [r3, #-48]
    a280: f842 1c04    	str	r1, [r2, #-4]
    a284: f853 1c20    	ldr	r1, [r3, #-32]
    a288: f8c2 c010    	str.w	r12, [r2, #0x10]
    a28c: f843 5c34    	str	r5, [r3, #-52]
    a290: 42b1         	cmp	r1, r6
    a292: d03f         	beq	0xa314 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x2cc> @ imm = #0x7e
    a294: 6111         	str	r1, [r2, #0x10]
    a296: f853 1c18    	ldr	r1, [r3, #-24]
    a29a: 6191         	str	r1, [r2, #0x18]
    a29c: f853 1c1c    	ldr	r1, [r3, #-28]
    a2a0: f843 6c20    	str	r6, [r3, #-32]
    a2a4: f102 0630    	add.w	r6, r2, #0x30
    a2a8: f803 5c18    	strb	r5, [r3, #-24]
    a2ac: 6151         	str	r1, [r2, #0x14]
    a2ae: f853 1c08    	ldr	r1, [r3, #-8]
    a2b2: 6296         	str	r6, [r2, #0x28]
    a2b4: f843 5c1c    	str	r5, [r3, #-28]
    a2b8: 4299         	cmp	r1, r3
    a2ba: d022         	beq	0xa302 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x2ba> @ imm = #0x44
    a2bc: 6291         	str	r1, [r2, #0x28]
    a2be: 6819         	ldr	r1, [r3]
    a2c0: 6311         	str	r1, [r2, #0x30]
    a2c2: f853 1c04    	ldr	r1, [r3, #-4]
    a2c6: 304c         	adds	r0, #0x4c
    a2c8: 4287         	cmp	r7, r0
    a2ca: f103 034c    	add.w	r3, r3, #0x4c
    a2ce: f102 024c    	add.w	r2, r2, #0x4c
    a2d2: f842 1c20    	str	r1, [r2, #-32]
    a2d6: f813 1c3c    	ldrb	r1, [r3, #-60]
    a2da: f802 1c0c    	strb	r1, [r2, #-12]
    a2de: d022         	beq	0xa326 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x2de> @ imm = #0x44
    a2e0: f853 1c38    	ldr	r1, [r3, #-56]
    a2e4: f100 0608    	add.w	r6, r0, #0x8
    a2e8: f842 2c08    	str	r2, [r2, #-8]
    a2ec: 42b1         	cmp	r1, r6
    a2ee: d1b8         	bne	0xa262 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x21a> @ imm = #-0x90
    a2f0: 6881         	ldr	r1, [r0, #0x8]
    a2f2: 6011         	str	r1, [r2]
    a2f4: 6871         	ldr	r1, [r6, #0x4]
    a2f6: 6051         	str	r1, [r2, #0x4]
    a2f8: 68b1         	ldr	r1, [r6, #0x8]
    a2fa: 6091         	str	r1, [r2, #0x8]
    a2fc: 68f1         	ldr	r1, [r6, #0xc]
    a2fe: 60d1         	str	r1, [r2, #0xc]
    a300: e7b4         	b	0xa26c <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x224> @ imm = #-0x98
    a302: 6819         	ldr	r1, [r3]
    a304: 6311         	str	r1, [r2, #0x30]
    a306: 6859         	ldr	r1, [r3, #0x4]
    a308: 6351         	str	r1, [r2, #0x34]
    a30a: 6899         	ldr	r1, [r3, #0x8]
    a30c: 6391         	str	r1, [r2, #0x38]
    a30e: 68d9         	ldr	r1, [r3, #0xc]
    a310: 63d1         	str	r1, [r2, #0x3c]
    a312: e7d6         	b	0xa2c2 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x27a> @ imm = #-0x54
    a314: 6a01         	ldr	r1, [r0, #0x20]
    a316: 6191         	str	r1, [r2, #0x18]
    a318: 6871         	ldr	r1, [r6, #0x4]
    a31a: 61d1         	str	r1, [r2, #0x1c]
    a31c: 68b1         	ldr	r1, [r6, #0x8]
    a31e: 6211         	str	r1, [r2, #0x20]
    a320: 68f1         	ldr	r1, [r6, #0xc]
    a322: 6251         	str	r1, [r2, #0x24]
    a324: e7ba         	b	0xa29c <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x254> @ imm = #-0x8c
    a326: eba7 0308    	sub.w	r3, r7, r8
    a32a: f64c 221b    	movw	r2, #0xca1b
    a32e: 3b4c         	subs	r3, #0x4c
    a330: f6c2 026b    	movt	r2, #0x286b
    a334: 089b         	lsrs	r3, r3, #0x2
    a336: fb02 f303    	mul	r3, r2, r3
    a33a: 224c         	movs	r2, #0x4c
    a33c: f023 4340    	bic	r3, r3, #0xc0000000
    a340: fb03 2202    	mla	r2, r3, r2, r2
    a344: 4414         	add	r4, r2
    a346: f1ba 0f00    	cmp.w	r10, #0x0
    a34a: d006         	beq	0xa35a <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x312> @ imm = #0xc
    a34c: 9b02         	ldr	r3, [sp, #0x8]
    a34e: 4650         	mov	r0, r10
    a350: 6899         	ldr	r1, [r3, #0x8]
    a352: eba1 010a    	sub.w	r1, r1, r10
    a356: f7f9 edd4    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6458
    a35a: e9dd 2301    	ldrd	r2, r3, [sp, #4]
    a35e: e9c3 2400    	strd	r2, r4, [r3]
    a362: 9a03         	ldr	r2, [sp, #0xc]
    a364: 609a         	str	r2, [r3, #0x8]
    a366: b005         	add	sp, #0x14
    a368: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    a36c: f64f 74c4    	movw	r4, #0xffc4
    a370: f6c7 74ff    	movt	r4, #0x7fff
    a374: 4620         	mov	r0, r4
    a376: f7f9 ee20    	blx	0x3fb8 <_Znwj@plt>      @ imm = #-0x63c0
    a37a: 1902         	adds	r2, r0, r4
    a37c: 9001         	str	r0, [sp, #0x4]
    a37e: f100 044c    	add.w	r4, r0, #0x4c
    a382: 9203         	str	r2, [sp, #0xc]
    a384: e687         	b	0xa096 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x4e> @ imm = #-0x2f2
    a386: 6ba8         	ldr	r0, [r5, #0x38]
    a388: 6398         	str	r0, [r3, #0x38]
    a38a: 6848         	ldr	r0, [r1, #0x4]
    a38c: 63d8         	str	r0, [r3, #0x3c]
    a38e: 6888         	ldr	r0, [r1, #0x8]
    a390: 6418         	str	r0, [r3, #0x40]
    a392: 68c8         	ldr	r0, [r1, #0xc]
    a394: 6458         	str	r0, [r3, #0x44]
    a396: e6b4         	b	0xa102 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0xba> @ imm = #-0x298
    a398: 68aa         	ldr	r2, [r5, #0x8]
    a39a: 609a         	str	r2, [r3, #0x8]
    a39c: 684a         	ldr	r2, [r1, #0x4]
    a39e: 60da         	str	r2, [r3, #0xc]
    a3a0: 688a         	ldr	r2, [r1, #0x8]
    a3a2: 611a         	str	r2, [r3, #0x10]
    a3a4: 68ca         	ldr	r2, [r1, #0xc]
    a3a6: 615a         	str	r2, [r3, #0x14]
    a3a8: e686         	b	0xa0b8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x70> @ imm = #-0x2f4
    a3aa: 6a28         	ldr	r0, [r5, #0x20]
    a3ac: 6218         	str	r0, [r3, #0x20]
    a3ae: 6848         	ldr	r0, [r1, #0x4]
    a3b0: 6258         	str	r0, [r3, #0x24]
    a3b2: 6888         	ldr	r0, [r1, #0x8]
    a3b4: 6298         	str	r0, [r3, #0x28]
    a3b6: 68c8         	ldr	r0, [r1, #0xc]
    a3b8: 62d8         	str	r0, [r3, #0x2c]
    a3ba: e68f         	b	0xa0dc <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x94> @ imm = #-0x2e2
    a3bc: 4293         	cmp	r3, r2
    a3be: bf28         	it	hs
    a3c0: 4613         	movhs	r3, r2
    a3c2: 224c         	movs	r2, #0x4c
    a3c4: fb02 f403    	mul	r4, r2, r3
    a3c8: e7d4         	b	0xa374 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x32c> @ imm = #-0x58
    a3ca: 4802         	ldr	r0, [pc, #0x8]          @ 0xa3d4 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x38c> literal=0x4314
    a3cc: 4478         	add	r0, pc => raw=0xe6e4 ghidra=0x1e6e4 file=59108 "vector::_M_realloc_insert"
    a3ce: f7f9 ee00    	blx	0x3fd0 <_ZSt20__throw_length_errorPKc@plt> @ imm = #-0x6400
    a3d2: bf00         	nop
    a3d4: 4314         	orrs	r4, r2
    a3d6: 0000         	movs	r0, r0
    a3d8: e92d 47f0    	push.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    a3dc: b0a6         	sub	sp, #0x98
    a3de: f10d 0804    	add.w	r8, sp, #0x4
    a3e2: 4614         	mov	r4, r2
    a3e4: 4699         	mov	r9, r3
    a3e6: 460e         	mov	r6, r1
    a3e8: 4605         	mov	r5, r0
    a3ea: f7f9 ee2a    	blx	0x4040 <_ZN7pbnjson7JSchema9AllSchemaEv@plt> @ imm = #-0x63ac
    a3ee: 4601         	mov	r1, r0
    a3f0: 4640         	mov	r0, r8
    a3f2: f7f9 ed7e    	blx	0x3ef0 <_ZN7pbnjson7JSchemaC1ERKS0_@plt> @ imm = #-0x6504
    a3f6: 6863         	ldr	r3, [r4, #0x4]
    a3f8: 2b00         	cmp	r3, #0x0
    a3fa: d159         	bne	0xa4b0 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x468> @ imm = #0xb2
    a3fc: ac0a         	add	r4, sp, #0x28
    a3fe: 4620         	mov	r0, r4
    a400: f7f9 ecc8    	blx	0x3d94 <_ZN7pbnjson10JDomParserC1Ev@plt> @ imm = #-0x6670
    a404: 682b         	ldr	r3, [r5]
    a406: f10d 0a1c    	add.w	r10, sp, #0x1c
    a40a: 4642         	mov	r2, r8
    a40c: 4651         	mov	r1, r10
    a40e: 4620         	mov	r0, r4
    a410: 9307         	str	r3, [sp, #0x1c]
    a412: 686b         	ldr	r3, [r5, #0x4]
    a414: 9308         	str	r3, [sp, #0x20]
    a416: f7f9 ec9a    	blx	0x3d4c <_ZN7pbnjson10JDomParser5parseERKNS_6JInputERKNS_7JSchemaE@plt> @ imm = #-0x66cc
    a41a: 4607         	mov	r7, r0
    a41c: 2800         	cmp	r0, #0x0
    a41e: d137         	bne	0xa490 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x448> @ imm = #0x6e
    a420: 4620         	mov	r0, r4
    a422: f7f9 ecd0    	blx	0x3dc4 <_ZN7pbnjson10JDomParser8getErrorEv@plt> @ imm = #-0x6660
    a426: 4931         	ldr	r1, [pc, #0xc4]         @ 0xa4ec <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x4a4> literal=0x42d4
    a428: 4479         	add	r1, pc => raw=0xe700 ghidra=0x1e700 file=59136 "Schema error"
    a42a: f7f9 eca2    	blx	0x3d70 <strstr@plt>     @ imm = #-0x66bc
    a42e: b328         	cbz	r0, 0xa47c <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x434> @ imm = #0x4a
    a430: f7f9 ee06    	blx	0x4040 <_ZN7pbnjson7JSchema9AllSchemaEv@plt> @ imm = #-0x63f4
    a434: 4601         	mov	r1, r0
    a436: 4650         	mov	r0, r10
    a438: f7f9 ed5a    	blx	0x3ef0 <_ZN7pbnjson7JSchemaC1ERKS0_@plt> @ imm = #-0x654c
    a43c: 682b         	ldr	r3, [r5]
    a43e: 4652         	mov	r2, r10
    a440: 4620         	mov	r0, r4
    a442: 9304         	str	r3, [sp, #0x10]
    a444: 686b         	ldr	r3, [r5, #0x4]
    a446: ad04         	add	r5, sp, #0x10
    a448: 4629         	mov	r1, r5
    a44a: 9305         	str	r3, [sp, #0x14]
    a44c: f7f9 ec7e    	blx	0x3d4c <_ZN7pbnjson10JDomParser5parseERKNS_6JInputERKNS_7JSchemaE@plt> @ imm = #-0x6704
    a450: b188         	cbz	r0, 0xa476 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x42e> @ imm = #0x22
    a452: 2303         	movs	r3, #0x3
    a454: 4621         	mov	r1, r4
    a456: 4628         	mov	r0, r5
    a458: f8c9 3000    	str.w	r3, [r9]
    a45c: f7f9 ee7e    	blx	0x415c <_ZN7pbnjson10JDomParser6getDomEv@plt> @ imm = #-0x6304
    a460: 6873         	ldr	r3, [r6, #0x4]
    a462: 4628         	mov	r0, r5
    a464: e9dd 1205    	ldrd	r1, r2, [sp, #20]
    a468: 9305         	str	r3, [sp, #0x14]
    a46a: 68b3         	ldr	r3, [r6, #0x8]
    a46c: 6071         	str	r1, [r6, #0x4]
    a46e: 60b2         	str	r2, [r6, #0x8]
    a470: 9306         	str	r3, [sp, #0x18]
    a472: f7f9 ec72    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x671c
    a476: 4650         	mov	r0, r10
    a478: f7f9 eeb2    	blx	0x41e0 <_ZN7pbnjson7JSchemaD1Ev@plt> @ imm = #-0x629c
    a47c: 4620         	mov	r0, r4
    a47e: f7f9 ee74    	blx	0x4168 <_ZN7pbnjson10JDomParserD1Ev@plt> @ imm = #-0x6318
    a482: 4640         	mov	r0, r8
    a484: f7f9 eeac    	blx	0x41e0 <_ZN7pbnjson7JSchemaD1Ev@plt> @ imm = #-0x62a8
    a488: 4638         	mov	r0, r7
    a48a: b026         	add	sp, #0x98
    a48c: e8bd 87f0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, pc}
    a490: 4621         	mov	r1, r4
    a492: 4650         	mov	r0, r10
    a494: f7f9 ee62    	blx	0x415c <_ZN7pbnjson10JDomParser6getDomEv@plt> @ imm = #-0x633c
    a498: 6873         	ldr	r3, [r6, #0x4]
    a49a: 4650         	mov	r0, r10
    a49c: e9dd 1208    	ldrd	r1, r2, [sp, #32]
    a4a0: 9308         	str	r3, [sp, #0x20]
    a4a2: 68b3         	ldr	r3, [r6, #0x8]
    a4a4: 6071         	str	r1, [r6, #0x4]
    a4a6: 60b2         	str	r2, [r6, #0x8]
    a4a8: 9309         	str	r3, [sp, #0x24]
    a4aa: f7f9 ec56    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6754
    a4ae: e7e5         	b	0xa47c <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x434> @ imm = #-0x36
    a4b0: 4621         	mov	r1, r4
    a4b2: ac0a         	add	r4, sp, #0x28
    a4b4: 4620         	mov	r0, r4
    a4b6: f7f9 ede4    	blx	0x4080 <_ZN7pbnjson15JSchemaFragmentC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x6438
    a4ba: 4621         	mov	r1, r4
    a4bc: 4640         	mov	r0, r8
    a4be: f7f9 ecb8    	blx	0x3e30 <_ZN7pbnjson7JSchema4swapERS0_@plt> @ imm = #-0x6690
    a4c2: 4620         	mov	r0, r4
    a4c4: f7f9 ecae    	blx	0x3e24 <_ZN7pbnjson15JSchemaFragmentD1Ev@plt> @ imm = #-0x66a4
    a4c8: e799         	b	0xa3fe <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x3b6> @ imm = #-0xce
    a4ca: 4650         	mov	r0, r10
    a4cc: f7f9 ee88    	blx	0x41e0 <_ZN7pbnjson7JSchemaD1Ev@plt> @ imm = #-0x62f0
    a4d0: 4620         	mov	r0, r4
    a4d2: f7f9 ee4a    	blx	0x4168 <_ZN7pbnjson10JDomParserD1Ev@plt> @ imm = #-0x636c
    a4d6: 4640         	mov	r0, r8
    a4d8: f7f9 ee82    	blx	0x41e0 <_ZN7pbnjson7JSchemaD1Ev@plt> @ imm = #-0x62fc
    a4dc: f7f9 ee50    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x6360
    a4e0: e7f6         	b	0xa4d0 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x488> @ imm = #-0x14
    a4e2: e7f8         	b	0xa4d6 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x48e> @ imm = #-0x10
    a4e4: 4620         	mov	r0, r4
    a4e6: f7f9 ec9e    	blx	0x3e24 <_ZN7pbnjson15JSchemaFragmentD1Ev@plt> @ imm = #-0x66c4
    a4ea: e7f4         	b	0xa4d6 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x48e> @ imm = #-0x18
    a4ec: 42d4         	cmn	r4, r2
    a4ee: 0000         	movs	r0, r0
    a4f0: 4610         	mov	r0, r2
    a4f2: f7fc bb09    	b.w	0x6b08 <ConnectionManager::connect(LSMessage&)> @ imm = #-0x39ee
    a4f6: bf00         	nop
    a4f8: 4610         	mov	r0, r2
    a4fa: f7fd b811    	b.w	0x7520 <ConnectionManager::disconnect(LSMessage&)> @ imm = #-0x2fde
    a4fe: bf00         	nop
    a500: 4610         	mov	r0, r2
    a502: f7fd bb1d    	b.w	0x7b40 <ConnectionManager::setSoundOut(LSMessage&)> @ imm = #-0x29c6
    a506: bf00         	nop
    a508: 4610         	mov	r0, r2
    a50a: f7fd bd6d    	b.w	0x7fe8 <ConnectionManager::mute(LSMessage&)> @ imm = #-0x2526
    a50e: bf00         	nop
    a510: 4610         	mov	r0, r2
    a512: f7fe b89f    	b.w	0x8654 <ConnectionManager::setInputVolume(LSMessage&)> @ imm = #-0x1ec2
    a516: bf00         	nop
    a518: 4610         	mov	r0, r2
    a51a: f7fe bbe1    	b.w	0x8ce0 <ConnectionManager::getStatus(LSMessage&)> @ imm = #-0x183e
    a51e: bf00         	nop
    a520: 4610         	mov	r0, r2
    a522: f7fe bca9    	b.w	0x8e78 <ConnectionManager::getHeadphoneStatus(LSMessage&)> @ imm = #-0x16ae
    a526: bf00         	nop
    a528: 4610         	mov	r0, r2
    a52a: f7ff b851    	b.w	0x95d0 <ConnectionManager::removePipelineInfo(LSMessage&)> @ imm = #-0xf5e
    a52e: bf00         	nop

0000a530 <setPmLogContext(char const*)>:
    a530: 4901         	ldr	r1, [pc, #0x4]          @ 0xa538 <setPmLogContext(char const*)+0x8> literal=0x16c1a
    a532: 4479         	add	r1, pc => raw=0x21150 ghidra=0x31150 file=null
    a534: f7f9 bd5e    	b.w	0x3ff4 <__cxa_throw@plt+0xc> @ imm = #-0x6544
    a538: 6c1a         	ldr	r2, [r3, #0x40]
    a53a: 0001         	movs	r1, r0

0000a53c <getPmLogContext()>:
    a53c: 4b01         	ldr	r3, [pc, #0x4]          @ 0xa544 <getPmLogContext()+0x8> literal=0x16c0e
    a53e: 447b         	add	r3, pc => raw=0x21150 ghidra=0x31150 file=null
    a540: 6818         	ldr	r0, [r3]
    a542: 4770         	bx	lr
    a544: 6c0e         	ldr	r6, [r1, #0x40]
    a546: 0001         	movs	r1, r0
    a548: 4b03         	ldr	r3, [pc, #0xc]          @ 0xa558 <getPmLogContext()+0x1c> literal=0x16c04
    a54a: 2201         	movs	r2, #0x1
    a54c: 447b         	add	r3, pc => raw=0x21154 ghidra=0x31154 file=null
    a54e: 6858         	ldr	r0, [r3, #0x4]
    a550: 701a         	strb	r2, [r3]
    a552: f7f9 bd87    	b.w	0x4064 <_ZN7pbnjson10JGeneratorC1EPNS_9JResolverE@plt+0xc> @ imm = #-0x64f2
    a556: bf00         	nop
    a558: 6c04         	ldr	r4, [r0, #0x40]
    a55a: 0001         	movs	r1, r0
    a55c: b510         	push	{r4, lr}
    a55e: f011 0438    	ands	r4, r1, #0x38
    a562: b0a0         	sub	sp, #0x80
    a564: bf18         	it	ne
    a566: 2400         	movne	r4, #0x0
    a568: d002         	beq	0xa570 <getPmLogContext()+0x34> @ imm = #0x4
    a56a: 4620         	mov	r0, r4
    a56c: b020         	add	sp, #0x80
    a56e: bd10         	pop	{r4, pc}
    a570: f7f9 ed8c    	blx	0x408c <g_io_channel_unix_get_fd@plt> @ imm = #-0x64e8
    a574: 2280         	movs	r2, #0x80
    a576: 4669         	mov	r1, sp
    a578: f7f9 ee6e    	blx	0x4258 <read@plt>       @ imm = #-0x6324
    a57c: 2880         	cmp	r0, #0x80
    a57e: d1f4         	bne	0xa56a <getPmLogContext()+0x2e> @ imm = #-0x18
    a580: 9b00         	ldr	r3, [sp]
    a582: 2b0a         	cmp	r3, #0xa
    a584: d015         	beq	0xa5b2 <getPmLogContext()+0x76> @ imm = #0x2a
    a586: 2b0f         	cmp	r3, #0xf
    a588: d003         	beq	0xa592 <getPmLogContext()+0x56> @ imm = #0x6
    a58a: 2b02         	cmp	r3, #0x2
    a58c: bf18         	it	ne
    a58e: 2401         	movne	r4, #0x1
    a590: d1eb         	bne	0xa56a <getPmLogContext()+0x2e> @ imm = #-0x2a
    a592: 4b09         	ldr	r3, [pc, #0x24]         @ 0xa5b8 <getPmLogContext()+0x7c> literal=0x16bbc
    a594: 447b         	add	r3, pc => raw=0x21154 ghidra=0x31154 file=null
    a596: 781a         	ldrb	r2, [r3]
    a598: b13a         	cbz	r2, 0xa5aa <getPmLogContext()+0x6e> @ imm = #0xe
    a59a: 4b08         	ldr	r3, [pc, #0x20]         @ 0xa5bc <getPmLogContext()+0x80> literal=0x16bae
    a59c: 2201         	movs	r2, #0x1
    a59e: 4614         	mov	r4, r2
    a5a0: 4620         	mov	r0, r4
    a5a2: 447b         	add	r3, pc => raw=0x21154 ghidra=0x31154 file=null
    a5a4: 701a         	strb	r2, [r3]
    a5a6: b020         	add	sp, #0x80
    a5a8: bd10         	pop	{r4, pc}
    a5aa: 6858         	ldr	r0, [r3, #0x4]
    a5ac: f7f9 ed5c    	blx	0x4068 <g_main_loop_quit@plt> @ imm = #-0x6548
    a5b0: e7f3         	b	0xa59a <getPmLogContext()+0x5e> @ imm = #-0x1a
    a5b2: 2001         	movs	r0, #0x1
    a5b4: f7f9 ee20    	blx	0x41f8 <exit@plt>       @ imm = #-0x63c0
    a5b8: 6bbc         	ldr	r4, [r7, #0x38]
    a5ba: 0001         	movs	r1, r0
    a5bc: 6bae         	ldr	r6, [r5, #0x38]
    a5be: 0001         	movs	r1, r0
    a5c0: 6803         	ldr	r3, [r0]
    a5c2: b570         	push	{r4, r5, r6, lr}
    a5c4: 4e11         	ldr	r6, [pc, #0x44]         @ 0xa60c <getPmLogContext()+0xd0> literal=0x167f2
    a5c6: f853 3c0c    	ldr	r3, [r3, #-12]
    a5ca: 447e         	add	r6, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    a5cc: 4403         	add	r3, r0
    a5ce: 6fdd         	ldr	r5, [r3, #0x7c]
    a5d0: b1cd         	cbz	r5, 0xa606 <getPmLogContext()+0xca> @ imm = #0x32
    a5d2: 7f2b         	ldrb	r3, [r5, #0x1c]
    a5d4: 4604         	mov	r4, r0
    a5d6: b143         	cbz	r3, 0xa5ea <getPmLogContext()+0xae> @ imm = #0x10
    a5d8: f895 1027    	ldrb.w	r1, [r5, #0x27]
    a5dc: 4620         	mov	r0, r4
    a5de: f7f9 ecc2    	blx	0x3f64 <_ZNSo3putEc@plt> @ imm = #-0x667c
    a5e2: e8bd 4070    	pop.w	{r4, r5, r6, lr}
    a5e6: f7f9 bc97    	b.w	0x3f18 <_ZNK7pbnjson6JValueixEPKc@plt+0xc> @ imm = #-0x66d2
    a5ea: 4628         	mov	r0, r5
    a5ec: f7f9 ec7a    	blx	0x3ee4 <_ZNKSt5ctypeIcE13_M_widen_initEv@plt> @ imm = #-0x670c
    a5f0: 682a         	ldr	r2, [r5]
    a5f2: 210a         	movs	r1, #0xa
    a5f4: 4b06         	ldr	r3, [pc, #0x18]         @ 0xa610 <getPmLogContext()+0xd4> literal=0x21c
    a5f6: 6992         	ldr	r2, [r2, #0x18]
    a5f8: 58f3         	ldr	r3, [r6, r3]
    a5fa: 429a         	cmp	r2, r3
    a5fc: d0ee         	beq	0xa5dc <getPmLogContext()+0xa0> @ imm = #-0x24
    a5fe: 4628         	mov	r0, r5
    a600: 4790         	blx	r2
    a602: 4601         	mov	r1, r0
    a604: e7ea         	b	0xa5dc <getPmLogContext()+0xa0> @ imm = #-0x2c
    a606: f7f9 ec2c    	blx	0x3e60 <_ZSt16__throw_bad_castv@plt> @ imm = #-0x67a8
    a60a: bf00         	nop
    a60c: 67f2         	str	r2, [r6, #0x7c]
    a60e: 0001         	movs	r1, r0
    a610: 021c         	lsls	r4, r3, #0x8
    a612: 0000         	movs	r0, r0
    a614: 4608         	mov	r0, r1
    a616: 4770         	bx	lr
    a618: 6802         	ldr	r2, [r0]
    a61a: 4b15         	ldr	r3, [pc, #0x54]         @ 0xa670 <getPmLogContext()+0x134> literal=0x1679a
    a61c: b570         	push	{r4, r5, r6, lr}
    a61e: 4604         	mov	r4, r0
    a620: b088         	sub	sp, #0x20
    a622: 447b         	add	r3, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    a624: b1aa         	cbz	r2, 0xa652 <getPmLogContext()+0x116> @ imm = #0x2a
    a626: 4a13         	ldr	r2, [pc, #0x4c]         @ 0xa674 <getPmLogContext()+0x138> literal=0x22c
    a628: ae01         	add	r6, sp, #0x4
    a62a: 4630         	mov	r0, r6
    a62c: 589d         	ldr	r5, [r3, r2]
    a62e: f105 0308    	add.w	r3, r5, #0x8
    a632: 9300         	str	r3, [sp]
    a634: f7f9 ec78    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x6710
    a638: 6820         	ldr	r0, [r4]
    a63a: 4631         	mov	r1, r6
    a63c: f7f9 ee12    	blx	0x4264 <LSUnregister@plt> @ imm = #-0x63dc
    a640: b150         	cbz	r0, 0xa658 <getPmLogContext()+0x11c> @ imm = #0x14
    a642: 4630         	mov	r0, r6
    a644: 3508         	adds	r5, #0x8
    a646: 9500         	str	r5, [sp]
    a648: f7f9 eda0    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x64c0
    a64c: 4668         	mov	r0, sp
    a64e: f7f9 eb6c    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x6928
    a652: 4620         	mov	r0, r4
    a654: b008         	add	sp, #0x20
    a656: bd70         	pop	{r4, r5, r6, pc}
    a658: 4807         	ldr	r0, [pc, #0x1c]         @ 0xa678 <getPmLogContext()+0x13c> literal=0x4318
    a65a: 4631         	mov	r1, r6
    a65c: 4478         	add	r0, pc => raw=0xe978 ghidra=0x1e978 file=59768 "LS_FAILED_TO_UNREG"
    a65e: f7f9 ecd2    	blx	0x4004 <LSErrorLogDefault@plt> @ imm = #-0x665c
    a662: e7ee         	b	0xa642 <getPmLogContext()+0x106> @ imm = #-0x24
    a664: 4668         	mov	r0, sp
    a666: f7f9 eb60    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x6940
    a66a: f7f9 edcc    	blx	0x4204 <_ZSt9terminatev@plt> @ imm = #-0x6468
    a66e: bf00         	nop
    a670: 679a         	str	r2, [r3, #0x78]
    a672: 0001         	movs	r1, r0
    a674: 022c         	lsls	r4, r5, #0x8
    a676: 0000         	movs	r0, r0
    a678: 4318         	orrs	r0, r3
    a67a: 0000         	movs	r0, r0

0000a67c <QualityManager::QualityManager(LS::Handle&, umiClient*)>:
    a67c: e92d 43f0    	push.w	{r4, r5, r6, r7, r8, r9, lr}
    a680: b08f         	sub	sp, #0x3c
    a682: f8df 8150    	ldr.w	r8, [pc, #0x150]        @ 0xa7d4 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x158> literal=0x1672e
    a686: ad07         	add	r5, sp, #0x1c
    a688: 4604         	mov	r4, r0
    a68a: 460f         	mov	r7, r1
    a68c: 4b52         	ldr	r3, [pc, #0x148]        @ 0xa7d8 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x15c> literal=0x22c
    a68e: 44f8         	add	r8, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    a690: e9c0 1200    	strd	r1, r2, [r0]
    a694: 4628         	mov	r0, r5
    a696: f858 6003    	ldr.w	r6, [r8, r3]
    a69a: f106 0308    	add.w	r3, r6, #0x8
    a69e: 9306         	str	r3, [sp, #0x18]
    a6a0: f7f9 ec42    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x677c
    a6a4: 4a4d         	ldr	r2, [pc, #0x134]        @ 0xa7dc <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x160> literal=0x16532
    a6a6: 2300         	movs	r3, #0x0
    a6a8: 494d         	ldr	r1, [pc, #0x134]        @ 0xa7e0 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x164> literal=0x42d6
    a6aa: 6838         	ldr	r0, [r7]
    a6ac: af06         	add	r7, sp, #0x18
    a6ae: 447a         	add	r2, pc => raw=0x20be4 ghidra=0x30be4 file=68580
    a6b0: 9501         	str	r5, [sp, #0x4]
    a6b2: 4479         	add	r1, pc => raw=0xe98c ghidra=0x1e98c file=59788 "/quality"
    a6b4: 9300         	str	r3, [sp]
    a6b6: f7f9 ed34    	blx	0x4120 <LSRegisterCategory@plt> @ imm = #-0x6598
    a6ba: 2800         	cmp	r0, #0x0
    a6bc: d06f         	beq	0xa79e <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x122> @ imm = #0xde
    a6be: f106 0908    	add.w	r9, r6, #0x8
    a6c2: 4628         	mov	r0, r5
    a6c4: af06         	add	r7, sp, #0x18
    a6c6: f8cd 9018    	str.w	r9, [sp, #0x18]
    a6ca: f7f9 ed60    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x6540
    a6ce: 4638         	mov	r0, r7
    a6d0: f7f9 eb2a    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x69ac
    a6d4: 4628         	mov	r0, r5
    a6d6: f8cd 9018    	str.w	r9, [sp, #0x18]
    a6da: f8d4 9000    	ldr.w	r9, [r4]
    a6de: f7f9 ec24    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x67b8
    a6e2: 4940         	ldr	r1, [pc, #0x100]        @ 0xa7e4 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x168> literal=0x429c
    a6e4: 462b         	mov	r3, r5
    a6e6: f8d9 0000    	ldr.w	r0, [r9]
    a6ea: 4622         	mov	r2, r4
    a6ec: 4479         	add	r1, pc => raw=0xe98c ghidra=0x1e98c file=59788 "/quality"
    a6ee: f7f9 ec5e    	blx	0x3fac <LSCategorySetData@plt> @ imm = #-0x6744
    a6f2: 2800         	cmp	r0, #0x0
    a6f4: d035         	beq	0xa762 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0xe6> @ imm = #0x6a
    a6f6: 4628         	mov	r0, r5
    a6f8: 3608         	adds	r6, #0x8
    a6fa: 9606         	str	r6, [sp, #0x18]
    a6fc: f7f9 ed46    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x6574
    a700: 4638         	mov	r0, r7
    a702: f7f9 eb12    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x69dc
    a706: 4620         	mov	r0, r4
    a708: b00f         	add	sp, #0x3c
    a70a: e8bd 83f0    	pop.w	{r4, r5, r6, r7, r8, r9, pc}
    a70e: 460e         	mov	r6, r1
    a710: 4605         	mov	r5, r0
    a712: a806         	add	r0, sp, #0x18
    a714: f7f9 eb08    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x69f0
    a718: 4633         	mov	r3, r6
    a71a: 2b01         	cmp	r3, #0x1
    a71c: d152         	bne	0xa7c4 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x148> @ imm = #0xa4
    a71e: 4628         	mov	r0, r5
    a720: f7f9 eaf6    	blx	0x3d10 <__cxa_begin_catch@plt> @ imm = #-0x6a14
    a724: 4605         	mov	r5, r0
    a726: f7ff ff09    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1ee
    a72a: 2800         	cmp	r0, #0x0
    a72c: d14c         	bne	0xa7c8 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x14c> @ imm = #0x98
    a72e: f7ff ff05    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1f6
    a732: 682a         	ldr	r2, [r5]
    a734: 4603         	mov	r3, r0
    a736: 4628         	mov	r0, r5
    a738: 461d         	mov	r5, r3
    a73a: 6893         	ldr	r3, [r2, #0x8]
    a73c: 4798         	blx	r3
    a73e: 4b2a         	ldr	r3, [pc, #0xa8]         @ 0xa7e8 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x16c> literal=0x424a
    a740: 4601         	mov	r1, r0
    a742: 2200         	movs	r2, #0x0
    a744: 9104         	str	r1, [sp, #0x10]
    a746: 4628         	mov	r0, r5
    a748: 2103         	movs	r1, #0x3
    a74a: 447b         	add	r3, pc => raw=0xe998 ghidra=0x1e998 file=59800 "%s - QualityManager API's registration Failed."
    a74c: e9cd 2201    	strd	r2, r2, [sp, #4]
    a750: 9303         	str	r3, [sp, #0xc]
    a752: 4b26         	ldr	r3, [pc, #0x98]         @ 0xa7ec <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x170> literal=0x34ee
    a754: 9200         	str	r2, [sp]
    a756: 447b         	add	r3, pc => raw=0xdc48 ghidra=0x1dc48 file=56392 "LS2_SUBSCRIBE_FAILED"
    a758: f7f9 ec60    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x6740
    a75c: f7f9 eca8    	blx	0x40b0 <__cxa_end_catch@plt> @ imm = #-0x66b0
    a760: e7d1         	b	0xa706 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x8a> @ imm = #-0x5e
    a762: 2020         	movs	r0, #0x20
    a764: f7f9 eb8e    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0x68e4
    a768: 4639         	mov	r1, r7
    a76a: 4605         	mov	r5, r0
    a76c: f7ff f9e6    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #-0xc34
    a770: 4b1f         	ldr	r3, [pc, #0x7c]         @ 0xa7f0 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x174> literal=0x228
    a772: 4628         	mov	r0, r5
    a774: f858 2003    	ldr.w	r2, [r8, r3]
    a778: 4b1e         	ldr	r3, [pc, #0x78]         @ 0xa7f4 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x178> literal=0x208
    a77a: f858 1003    	ldr.w	r1, [r8, r3]
    a77e: f7f9 ec34    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x6798
    a782: 460e         	mov	r6, r1
    a784: 4605         	mov	r5, r0
    a786: 4638         	mov	r0, r7
    a788: f7ff f964    	bl	0x9a54 <ConnectionManager::removePipelineInfo(LSMessage&)+0x484> @ imm = #-0xd38
    a78c: 4633         	mov	r3, r6
    a78e: e7c4         	b	0xa71a <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x9e> @ imm = #-0x78
    a790: 460e         	mov	r6, r1
    a792: 4605         	mov	r5, r0
    a794: 4638         	mov	r0, r7
    a796: f7f9 eac8    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x6a70
    a79a: 4633         	mov	r3, r6
    a79c: e7bd         	b	0xa71a <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x9e> @ imm = #-0x86
    a79e: 2020         	movs	r0, #0x20
    a7a0: f7f9 eb70    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0x6920
    a7a4: 4639         	mov	r1, r7
    a7a6: 4605         	mov	r5, r0
    a7a8: f7ff f9c8    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #-0xc70
    a7ac: 4b10         	ldr	r3, [pc, #0x40]         @ 0xa7f0 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x174> literal=0x228
    a7ae: 4628         	mov	r0, r5
    a7b0: f858 2003    	ldr.w	r2, [r8, r3]
    a7b4: 4b0f         	ldr	r3, [pc, #0x3c]         @ 0xa7f4 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x178> literal=0x208
    a7b6: f858 1003    	ldr.w	r1, [r8, r3]
    a7ba: f7f9 ec16    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x67d4
    a7be: e7e0         	b	0xa782 <QualityManager::QualityManager(LS::Handle&, umiClient*)+0x106> @ imm = #-0x40
    a7c0: f7f9 ec76    	blx	0x40b0 <__cxa_end_catch@plt> @ imm = #-0x6714
    a7c4: f7f9 ecdc    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x6648
    a7c8: f7ff feb8    	bl	0xa53c <getPmLogContext()> @ imm = #-0x290
    a7cc: 6803         	ldr	r3, [r0]
    a7ce: 2b02         	cmp	r3, #0x2
    a7d0: ddc4         	ble	0xa75c <QualityManager::QualityManager(LS::Handle&, umiClient*)+0xe0> @ imm = #-0x78
    a7d2: e7ac         	b	0xa72e <QualityManager::QualityManager(LS::Handle&, umiClient*)+0xb2> @ imm = #-0xa8
    a7d4: 672e         	str	r6, [r5, #0x70]
    a7d6: 0001         	movs	r1, r0
    a7d8: 022c         	lsls	r4, r5, #0x8
    a7da: 0000         	movs	r0, r0
    a7dc: 6532         	str	r2, [r6, #0x50]
    a7de: 0001         	movs	r1, r0
    a7e0: 42d6         	cmn	r6, r2
    a7e2: 0000         	movs	r0, r0
    a7e4: 429c         	cmp	r4, r3
    a7e6: 0000         	movs	r0, r0
    a7e8: 424a         	rsbs	r2, r1, #0
    a7ea: 0000         	movs	r0, r0
    a7ec: 34ee         	adds	r4, #0xee
    a7ee: 0000         	movs	r0, r0
    a7f0: 0228         	lsls	r0, r5, #0x8
    a7f2: 0000         	movs	r0, r0
    a7f4: 0208         	lsls	r0, r1, #0x8
    a7f6: 0000         	movs	r0, r0

0000a7f8 <QualityManager::isValidCategory(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)>:
    a7f8: 2001         	movs	r0, #0x1
    a7fa: 4770         	bx	lr

0000a7fc <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)>:
    a7fc: e92d 47f0    	push.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    a800: b092         	sub	sp, #0x48
    a802: ad06         	add	r5, sp, #0x18
    a804: 4606         	mov	r6, r0
    a806: 460c         	mov	r4, r1
    a808: 4628         	mov	r0, r5
    a80a: 4611         	mov	r1, r2
    a80c: f7f9 eb5e    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x6944
    a810: f7ff fe94    	bl	0xa53c <getPmLogContext()> @ imm = #-0x2d8
    a814: b120         	cbz	r0, 0xa820 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x24> @ imm = #0x8
    a816: f7ff fe91    	bl	0xa53c <getPmLogContext()> @ imm = #-0x2de
    a81a: 6803         	ldr	r3, [r0]
    a81c: 2b05         	cmp	r3, #0x5
    a81e: dd23         	ble	0xa868 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x6c> @ imm = #0x46
    a820: f7ff fe8c    	bl	0xa53c <getPmLogContext()> @ imm = #-0x2e8
    a824: af0c         	add	r7, sp, #0x30
    a826: 4680         	mov	r8, r0
    a828: 2200         	movs	r2, #0x0
    a82a: 4629         	mov	r1, r5
    a82c: 4638         	mov	r0, r7
    a82e: f8d4 9000    	ldr.w	r9, [r4]
    a832: f7f9 ecdc    	blx	0x41ec <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt> @ imm = #-0x6648
    a836: 9b0c         	ldr	r3, [sp, #0x30]
    a838: 2200         	movs	r2, #0x0
    a83a: 4640         	mov	r0, r8
    a83c: 2106         	movs	r1, #0x6
    a83e: f8cd 9010    	str.w	r9, [sp, #0x10]
    a842: 9305         	str	r3, [sp, #0x14]
    a844: 4b49         	ldr	r3, [pc, #0x124]        @ 0xa96c <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x170> literal=0x4190
    a846: e9cd 2201    	strd	r2, r2, [sp, #4]
    a84a: 9200         	str	r2, [sp]
    a84c: 447b         	add	r3, pc => raw=0xe9e0 ghidra=0x1e9e0 file=59872 "setSettings request for category:%s with value:%s"
    a84e: 9303         	str	r3, [sp, #0xc]
    a850: 4b47         	ldr	r3, [pc, #0x11c]        @ 0xa970 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x174> literal=0x4172
    a852: 447b         	add	r3, pc => raw=0xe9c8 ghidra=0x1e9c8 file=59848 "AUDIO_SET_SETTINGS_CALL"
    a854: f7f9 ebe2    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x683c
    a858: 980c         	ldr	r0, [sp, #0x30]
    a85a: ab0e         	add	r3, sp, #0x38
    a85c: 4298         	cmp	r0, r3
    a85e: d003         	beq	0xa868 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x6c> @ imm = #0x6
    a860: 990e         	ldr	r1, [sp, #0x38]
    a862: 3101         	adds	r1, #0x1
    a864: f7f9 eb4c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6968
    a868: f8d6 8004    	ldr.w	r8, [r6, #0x4]
    a86c: f1b8 0f00    	cmp.w	r8, #0x0
    a870: d05e         	beq	0xa930 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x134> @ imm = #0xbc
    a872: e9d4 a400    	ldrd	r10, r4, [r4]
    a876: f10d 0938    	add.w	r9, sp, #0x38
    a87a: af0c         	add	r7, sp, #0x30
    a87c: f8cd 9030    	str.w	r9, [sp, #0x30]
    a880: 4263         	rsbs	r3, r4, #0
    a882: faba f28a    	clz	r2, r10
    a886: 0952         	lsrs	r2, r2, #0x5
    a888: 459a         	cmp	r10, r3
    a88a: bf08         	it	eq
    a88c: 2200         	moveq	r2, #0x0
    a88e: 2a00         	cmp	r2, #0x0
    a890: d157         	bne	0xa942 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x146> @ imm = #0xae
    a892: 2c0f         	cmp	r4, #0xf
    a894: 9409         	str	r4, [sp, #0x24]
    a896: d83c         	bhi	0xa912 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x116> @ imm = #0x78
    a898: 2c01         	cmp	r4, #0x1
    a89a: d14b         	bne	0xa934 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x138> @ imm = #0x96
    a89c: f89a 3000    	ldrb.w	r3, [r10]
    a8a0: 464a         	mov	r2, r9
    a8a2: ae09         	add	r6, sp, #0x24
    a8a4: f88d 3038    	strb.w	r3, [sp, #0x38]
    a8a8: 2300         	movs	r3, #0x0
    a8aa: 940d         	str	r4, [sp, #0x34]
    a8ac: 4629         	mov	r1, r5
    a8ae: 4630         	mov	r0, r6
    a8b0: 5513         	strb	r3, [r2, r4]
    a8b2: f7f9 eb0c    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x69e8
    a8b6: 4640         	mov	r0, r8
    a8b8: 4632         	mov	r2, r6
    a8ba: 4639         	mov	r1, r7
    a8bc: f7f9 eb4c    	blx	0x3f58 <_ZN9umiClient11setSettingsENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt> @ imm = #-0x6968
    a8c0: 4604         	mov	r4, r0
    a8c2: 4630         	mov	r0, r6
    a8c4: f7f9 ea48    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6b70
    a8c8: 980c         	ldr	r0, [sp, #0x30]
    a8ca: 4548         	cmp	r0, r9
    a8cc: d003         	beq	0xa8d6 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0xda> @ imm = #0x6
    a8ce: 990e         	ldr	r1, [sp, #0x38]
    a8d0: 3101         	adds	r1, #0x1
    a8d2: f7f9 eb16    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x69d4
    a8d6: f7ff fe31    	bl	0xa53c <getPmLogContext()> @ imm = #-0x39e
    a8da: b120         	cbz	r0, 0xa8e6 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0xea> @ imm = #0x8
    a8dc: f7ff fe2e    	bl	0xa53c <getPmLogContext()> @ imm = #-0x3a4
    a8e0: 6803         	ldr	r3, [r0]
    a8e2: 2b05         	cmp	r3, #0x5
    a8e4: dd0e         	ble	0xa904 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x108> @ imm = #0x1c
    a8e6: f7ff fe29    	bl	0xa53c <getPmLogContext()> @ imm = #-0x3ae
    a8ea: 4b22         	ldr	r3, [pc, #0x88]         @ 0xa974 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x178> literal=0x411a
    a8ec: 2200         	movs	r2, #0x0
    a8ee: 2106         	movs	r1, #0x6
    a8f0: 9404         	str	r4, [sp, #0x10]
    a8f2: e9cd 2201    	strd	r2, r2, [sp, #4]
    a8f6: 447b         	add	r3, pc => raw=0xea14 ghidra=0x1ea14 file=59924 "setSettings umi returnStatus %d"
    a8f8: 9200         	str	r2, [sp]
    a8fa: 9303         	str	r3, [sp, #0xc]
    a8fc: 4b1e         	ldr	r3, [pc, #0x78]         @ 0xa978 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x17c> literal=0x40c6
    a8fe: 447b         	add	r3, pc => raw=0xe9c8 ghidra=0x1e9c8 file=59848 "AUDIO_SET_SETTINGS_CALL"
    a900: f7f9 eb8c    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x68e8
    a904: 4628         	mov	r0, r5
    a906: f7f9 ea28    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6bb0
    a90a: 4620         	mov	r0, r4
    a90c: b012         	add	sp, #0x48
    a90e: e8bd 87f0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, pc}
    a912: ae09         	add	r6, sp, #0x24
    a914: 4638         	mov	r0, r7
    a916: 4631         	mov	r1, r6
    a918: f7f9 ebac    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x68a8
    a91c: 9b09         	ldr	r3, [sp, #0x24]
    a91e: 900c         	str	r0, [sp, #0x30]
    a920: 930e         	str	r3, [sp, #0x38]
    a922: 4622         	mov	r2, r4
    a924: 4651         	mov	r1, r10
    a926: f7f9 eb30    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x69a0
    a92a: 9c09         	ldr	r4, [sp, #0x24]
    a92c: 9a0c         	ldr	r2, [sp, #0x30]
    a92e: e7bb         	b	0xa8a8 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0xac> @ imm = #-0x8a
    a930: 2402         	movs	r4, #0x2
    a932: e7d0         	b	0xa8d6 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0xda> @ imm = #-0x60
    a934: 464a         	mov	r2, r9
    a936: ae09         	add	r6, sp, #0x24
    a938: 2c00         	cmp	r4, #0x0
    a93a: d0b5         	beq	0xa8a8 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0xac> @ imm = #-0x96
    a93c: 4648         	mov	r0, r9
    a93e: ae09         	add	r6, sp, #0x24
    a940: e7ef         	b	0xa922 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x126> @ imm = #-0x22
    a942: 480e         	ldr	r0, [pc, #0x38]         @ 0xa97c <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x180> literal=0x2d50
    a944: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    a946: f7f9 e9de    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x6c44
    a94a: 4638         	mov	r0, r7
    a94c: f7f9 ebd0    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x6860
    a950: 4628         	mov	r0, r5
    a952: f7f9 ea02    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6bfc
    a956: f7f9 ec14    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x67d8
    a95a: e7f9         	b	0xa950 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x154> @ imm = #-0xe
    a95c: e7f5         	b	0xa94a <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x14e> @ imm = #-0x16
    a95e: 4630         	mov	r0, r6
    a960: f7f9 e9fa    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6c0c
    a964: e7f1         	b	0xa94a <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x14e> @ imm = #-0x1e
    a966: e7f3         	b	0xa950 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x154> @ imm = #-0x1a
    a968: e7f2         	b	0xa950 <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x154> @ imm = #-0x1c
    a96a: bf00         	nop
    a96c: 4190         	sbcs	r0, r2
    a96e: 0000         	movs	r0, r0
    a970: 4172         	adcs	r2, r6
    a972: 0000         	movs	r0, r0
    a974: 411a         	asrs	r2, r3
    a976: 0000         	movs	r0, r0
    a978: 40c6         	lsrs	r6, r0
    a97a: 0000         	movs	r0, r0
    a97c: 2d50         	cmp	r5, #0x50
    a97e: 0000         	movs	r0, r0

0000a980 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)>:
    a980: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    a984: b097         	sub	sp, #0x5c
    a986: f10d 091c    	add.w	r9, sp, #0x1c
    a98a: 4606         	mov	r6, r0
    a98c: 468a         	mov	r10, r1
    a98e: 4648         	mov	r0, r9
    a990: 4619         	mov	r1, r3
    a992: 461c         	mov	r4, r3
    a994: 4615         	mov	r5, r2
    a996: f7f9 ea9a    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x6acc
    a99a: f7ff fdcf    	bl	0xa53c <getPmLogContext()> @ imm = #-0x462
    a99e: 2800         	cmp	r0, #0x0
    a9a0: f000 80ad    	beq.w	0xaafe <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x17e> @ imm = #0x15a
    a9a4: f7ff fdca    	bl	0xa53c <getPmLogContext()> @ imm = #-0x46c
    a9a8: 6803         	ldr	r3, [r0]
    a9aa: 2b05         	cmp	r3, #0x5
    a9ac: f300 80a7    	bgt.w	0xaafe <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x17e> @ imm = #0x14e
    a9b0: 4630         	mov	r0, r6
    a9b2: f7f9 ea02    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x6bfc
    a9b6: f10d 0828    	add.w	r8, sp, #0x28
    a9ba: 4640         	mov	r0, r8
    a9bc: f7f9 e9fc    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x6c08
    a9c0: f8da 7004    	ldr.w	r7, [r10, #0x4]
    a9c4: 2f00         	cmp	r7, #0x0
    a9c6: f000 8100    	beq.w	0xabca <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x24a> @ imm = #0x200
    a9ca: 4621         	mov	r1, r4
    a9cc: ac0d         	add	r4, sp, #0x34
    a9ce: 4620         	mov	r0, r4
    a9d0: f7f9 ea7c    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x6b08
    a9d4: 4639         	mov	r1, r7
    a9d6: af10         	add	r7, sp, #0x40
    a9d8: 462a         	mov	r2, r5
    a9da: 4623         	mov	r3, r4
    a9dc: 4638         	mov	r0, r7
    a9de: f7f9 ec5a    	blx	0x4294 <_ZN9umiClient11getSettingsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt> @ imm = #-0x674c
    a9e2: 9a0b         	ldr	r2, [sp, #0x2c]
    a9e4: 4638         	mov	r0, r7
    a9e6: 9b11         	ldr	r3, [sp, #0x44]
    a9e8: 9211         	str	r2, [sp, #0x44]
    a9ea: 930b         	str	r3, [sp, #0x2c]
    a9ec: 9a0c         	ldr	r2, [sp, #0x30]
    a9ee: 9b12         	ldr	r3, [sp, #0x48]
    a9f0: 9212         	str	r2, [sp, #0x48]
    a9f2: 930c         	str	r3, [sp, #0x30]
    a9f4: f7f9 e9b0    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6ca0
    a9f8: 4620         	mov	r0, r4
    a9fa: f7f9 e9ae    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6ca4
    a9fe: f7ff fd9d    	bl	0xa53c <getPmLogContext()> @ imm = #-0x4c6
    aa02: b120         	cbz	r0, 0xaa0e <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x8e> @ imm = #0x8
    aa04: f7ff fd9a    	bl	0xa53c <getPmLogContext()> @ imm = #-0x4cc
    aa08: 6803         	ldr	r3, [r0]
    aa0a: 2b05         	cmp	r3, #0x5
    aa0c: dd1e         	ble	0xaa4c <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0xcc> @ imm = #0x3c
    aa0e: f7ff fd95    	bl	0xa53c <getPmLogContext()> @ imm = #-0x4d6
    aa12: 4605         	mov	r5, r0
    aa14: 2200         	movs	r2, #0x0
    aa16: 4641         	mov	r1, r8
    aa18: 4638         	mov	r0, r7
    aa1a: f7f9 ebe8    	blx	0x41ec <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt> @ imm = #-0x6830
    aa1e: 9b10         	ldr	r3, [sp, #0x40]
    aa20: 2200         	movs	r2, #0x0
    aa22: 4628         	mov	r0, r5
    aa24: 2106         	movs	r1, #0x6
    aa26: e9cd 2201    	strd	r2, r2, [sp, #4]
    aa2a: 9304         	str	r3, [sp, #0x10]
    aa2c: 4bab         	ldr	r3, [pc, #0x2ac]        @ 0xacdc <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x35c> literal=0x4050
    aa2e: 9200         	str	r2, [sp]
    aa30: 447b         	add	r3, pc => raw=0xea84 ghidra=0x1ea84 file=60036 "Audio getSettings response:%s"
    aa32: 9303         	str	r3, [sp, #0xc]
    aa34: 4baa         	ldr	r3, [pc, #0x2a8]        @ 0xace0 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x360> literal=0x3ffa
    aa36: 447b         	add	r3, pc => raw=0xea34 ghidra=0x1ea34 file=59956 "AUDIO_GET_SETTINGS_CALL"
    aa38: f7f9 eaf0    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x6a20
    aa3c: 9810         	ldr	r0, [sp, #0x40]
    aa3e: ab12         	add	r3, sp, #0x48
    aa40: 4298         	cmp	r0, r3
    aa42: d003         	beq	0xaa4c <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0xcc> @ imm = #0x6
    aa44: 9912         	ldr	r1, [sp, #0x48]
    aa46: 3101         	adds	r1, #0x1
    aa48: f7f9 ea5a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6b4c
    aa4c: 2101         	movs	r1, #0x1
    aa4e: 4640         	mov	r0, r8
    aa50: f7f9 ebf0    	blx	0x4234 <_ZNK7pbnjson6JValueeqEb@plt> @ imm = #-0x6820
    aa54: 2101         	movs	r1, #0x1
    aa56: 2800         	cmp	r0, #0x0
    aa58: d077         	beq	0xab4a <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x1ca> @ imm = #0xee
    aa5a: 4620         	mov	r0, r4
    aa5c: f7f9 e9ca    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x6c6c
    aa60: 4aa0         	ldr	r2, [pc, #0x280]        @ 0xace4 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x364> literal=0x2cb0
    aa62: ad12         	add	r5, sp, #0x48
    aa64: 462b         	mov	r3, r5
    aa66: 9510         	str	r5, [sp, #0x40]
    aa68: f04f 0e0b    	mov.w	lr, #0xb
    aa6c: f04f 0c00    	mov.w	r12, #0x0
    aa70: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    aa72: ca07         	ldm	r2, {r0, r1, r2}
    aa74: c303         	stm	r3!, {r0, r1}
    aa76: 4639         	mov	r1, r7
    aa78: f823 2b02    	strh	r2, [r3], #2
    aa7c: 0c12         	lsrs	r2, r2, #0x10
    aa7e: 4630         	mov	r0, r6
    aa80: 701a         	strb	r2, [r3]
    aa82: 4622         	mov	r2, r4
    aa84: f8cd e044    	str.w	lr, [sp, #0x44]
    aa88: f88d c053    	strb.w	r12, [sp, #0x53]
    aa8c: f7f9 e922    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x6dbc
    aa90: 9810         	ldr	r0, [sp, #0x40]
    aa92: 42a8         	cmp	r0, r5
    aa94: d003         	beq	0xaa9e <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x11e> @ imm = #0x6
    aa96: 9912         	ldr	r1, [sp, #0x48]
    aa98: 3101         	adds	r1, #0x1
    aa9a: f7f9 ea32    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6b9c
    aa9e: 4620         	mov	r0, r4
    aaa0: f7f9 e95a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6d4c
    aaa4: 4990         	ldr	r1, [pc, #0x240]        @ 0xace8 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x368> literal=0x3050
    aaa6: 4620         	mov	r0, r4
    aaa8: 4479         	add	r1, pc => raw=0xdafc ghidra=0x1dafc file=56060 "null"
    aaaa: f7f9 ebfa    	blx	0x42a0 <_ZN9umiClient11getSettingsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE@plt+0xc> @ imm = #-0x680c
    aaae: 4b8f         	ldr	r3, [pc, #0x23c]        @ 0xacec <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x36c> literal=0x3fe4
    aab0: f04f 0e08    	mov.w	lr, #0x8
    aab4: f04f 0c00    	mov.w	r12, #0x0
    aab8: 9510         	str	r5, [sp, #0x40]
    aaba: 4622         	mov	r2, r4
    aabc: 447b         	add	r3, pc => raw=0xeaa4 ghidra=0x1eaa4 file=60068 "settings"
    aabe: e893 0003    	ldm.w	r3, {r0, r1}
    aac2: e885 0003    	stm.w	r5, {r0, r1}
    aac6: 4639         	mov	r1, r7
    aac8: 4630         	mov	r0, r6
    aaca: f8cd e044    	str.w	lr, [sp, #0x44]
    aace: f88d c050    	strb.w	r12, [sp, #0x50]
    aad2: f7f9 e900    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x6e00
    aad6: 9810         	ldr	r0, [sp, #0x40]
    aad8: 42a8         	cmp	r0, r5
    aada: d003         	beq	0xaae4 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x164> @ imm = #0x6
    aadc: 9912         	ldr	r1, [sp, #0x48]
    aade: 3101         	adds	r1, #0x1
    aae0: f7f9 ea0e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6be4
    aae4: 4620         	mov	r0, r4
    aae6: f7f9 e938    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6d90
    aaea: 4640         	mov	r0, r8
    aaec: f7f9 e934    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6d98
    aaf0: 4648         	mov	r0, r9
    aaf2: f7f9 e932    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6d9c
    aaf6: 4630         	mov	r0, r6
    aaf8: b017         	add	sp, #0x5c
    aafa: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    aafe: f7ff fd1d    	bl	0xa53c <getPmLogContext()> @ imm = #-0x5c6
    ab02: af10         	add	r7, sp, #0x40
    ab04: 4680         	mov	r8, r0
    ab06: 2200         	movs	r2, #0x0
    ab08: 4649         	mov	r1, r9
    ab0a: 4638         	mov	r0, r7
    ab0c: f8d5 b000    	ldr.w	r11, [r5]
    ab10: f7f9 eb6c    	blx	0x41ec <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt> @ imm = #-0x6928
    ab14: 9b10         	ldr	r3, [sp, #0x40]
    ab16: 2200         	movs	r2, #0x0
    ab18: 4640         	mov	r0, r8
    ab1a: 2106         	movs	r1, #0x6
    ab1c: f8cd b010    	str.w	r11, [sp, #0x10]
    ab20: 9305         	str	r3, [sp, #0x14]
    ab22: 4b73         	ldr	r3, [pc, #0x1cc]        @ 0xacf0 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x370> literal=0x3f1e
    ab24: e9cd 2201    	strd	r2, r2, [sp, #4]
    ab28: 9200         	str	r2, [sp]
    ab2a: 447b         	add	r3, pc => raw=0xea4c ghidra=0x1ea4c file=59980 "Audio getSettings request for category:%s, settings:%s"
    ab2c: 9303         	str	r3, [sp, #0xc]
    ab2e: 4b71         	ldr	r3, [pc, #0x1c4]        @ 0xacf4 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x374> literal=0x3f00
    ab30: 447b         	add	r3, pc => raw=0xea34 ghidra=0x1ea34 file=59956 "AUDIO_GET_SETTINGS_CALL"
    ab32: f7f9 ea74    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x6b18
    ab36: 9810         	ldr	r0, [sp, #0x40]
    ab38: ab12         	add	r3, sp, #0x48
    ab3a: 4298         	cmp	r0, r3
    ab3c: f43f af38    	beq.w	0xa9b0 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x30> @ imm = #-0x190
    ab40: 9912         	ldr	r1, [sp, #0x48]
    ab42: 3101         	adds	r1, #0x1
    ab44: f7f9 e9dc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6c48
    ab48: e732         	b	0xa9b0 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x30> @ imm = #-0x19c
    ab4a: 4620         	mov	r0, r4
    ab4c: f7f9 e952    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x6d5c
    ab50: 4a69         	ldr	r2, [pc, #0x1a4]        @ 0xacf8 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x378> literal=0x2bc0
    ab52: ad12         	add	r5, sp, #0x48
    ab54: 462b         	mov	r3, r5
    ab56: 9510         	str	r5, [sp, #0x40]
    ab58: f04f 0e0b    	mov.w	lr, #0xb
    ab5c: f04f 0c00    	mov.w	r12, #0x0
    ab60: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    ab62: ca07         	ldm	r2, {r0, r1, r2}
    ab64: c303         	stm	r3!, {r0, r1}
    ab66: 4639         	mov	r1, r7
    ab68: f823 2b02    	strh	r2, [r3], #2
    ab6c: 0c12         	lsrs	r2, r2, #0x10
    ab6e: 4630         	mov	r0, r6
    ab70: 701a         	strb	r2, [r3]
    ab72: 4622         	mov	r2, r4
    ab74: f8cd e044    	str.w	lr, [sp, #0x44]
    ab78: f88d c053    	strb.w	r12, [sp, #0x53]
    ab7c: f7f9 e8aa    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x6eac
    ab80: 9810         	ldr	r0, [sp, #0x40]
    ab82: 42a8         	cmp	r0, r5
    ab84: d003         	beq	0xab8e <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x20e> @ imm = #0x6
    ab86: 9912         	ldr	r1, [sp, #0x48]
    ab88: 3101         	adds	r1, #0x1
    ab8a: f7f9 e9ba    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6c8c
    ab8e: 4620         	mov	r0, r4
    ab90: 2400         	movs	r4, #0x0
    ab92: f7f9 e8e2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6e3c
    ab96: 4b59         	ldr	r3, [pc, #0x164]        @ 0xacfc <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x37c> literal=0x3f00
    ab98: f04f 0c08    	mov.w	r12, #0x8
    ab9c: 9510         	str	r5, [sp, #0x40]
    ab9e: 4642         	mov	r2, r8
    aba0: 447b         	add	r3, pc => raw=0xeaa4 ghidra=0x1eaa4 file=60068 "settings"
    aba2: e893 0003    	ldm.w	r3, {r0, r1}
    aba6: e885 0003    	stm.w	r5, {r0, r1}
    abaa: 4639         	mov	r1, r7
    abac: 4630         	mov	r0, r6
    abae: f8cd c044    	str.w	r12, [sp, #0x44]
    abb2: f88d 4050    	strb.w	r4, [sp, #0x50]
    abb6: f7f9 e88e    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x6ee4
    abba: 9810         	ldr	r0, [sp, #0x40]
    abbc: 42a8         	cmp	r0, r5
    abbe: d094         	beq	0xaaea <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x16a> @ imm = #-0xd8
    abc0: 9912         	ldr	r1, [sp, #0x48]
    abc2: 3101         	adds	r1, #0x1
    abc4: f7f9 e99c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6cc8
    abc8: e78f         	b	0xaaea <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x16a> @ imm = #-0xe2
    abca: ac0d         	add	r4, sp, #0x34
    abcc: 4639         	mov	r1, r7
    abce: 4620         	mov	r0, r4
    abd0: f7f9 e910    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x6de0
    abd4: 4a4a         	ldr	r2, [pc, #0x128]        @ 0xad00 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x380> literal=0x2b3e
    abd6: ad12         	add	r5, sp, #0x48
    abd8: 462b         	mov	r3, r5
    abda: 9510         	str	r5, [sp, #0x40]
    abdc: af10         	add	r7, sp, #0x40
    abde: f04f 0e0b    	mov.w	lr, #0xb
    abe2: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    abe4: f04f 0c00    	mov.w	r12, #0x0
    abe8: ca07         	ldm	r2, {r0, r1, r2}
    abea: c303         	stm	r3!, {r0, r1}
    abec: 4639         	mov	r1, r7
    abee: f823 2b02    	strh	r2, [r3], #2
    abf2: 0c12         	lsrs	r2, r2, #0x10
    abf4: 4630         	mov	r0, r6
    abf6: 701a         	strb	r2, [r3]
    abf8: 4622         	mov	r2, r4
    abfa: f8cd e044    	str.w	lr, [sp, #0x44]
    abfe: f88d c053    	strb.w	r12, [sp, #0x53]
    ac02: f7f9 e868    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x6f30
    ac06: 9810         	ldr	r0, [sp, #0x40]
    ac08: 42a8         	cmp	r0, r5
    ac0a: d003         	beq	0xac14 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x294> @ imm = #0x6
    ac0c: 9912         	ldr	r1, [sp, #0x48]
    ac0e: 3101         	adds	r1, #0x1
    ac10: f7f9 e976    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6d14
    ac14: 4620         	mov	r0, r4
    ac16: f7f9 e8a0    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6ec0
    ac1a: 493a         	ldr	r1, [pc, #0xe8]         @ 0xad04 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x384> literal=0x16662
    ac1c: 4620         	mov	r0, r4
    ac1e: 4479         	add	r1, pc => raw=0x21284 ghidra=0x31284 file=null
    ac20: 3164         	adds	r1, #0x64
    ac22: f7f9 ea84    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x6af8
    ac26: 4a38         	ldr	r2, [pc, #0xe0]         @ 0xad08 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x388> literal=0x3a24
    ac28: 462b         	mov	r3, r5
    ac2a: 9510         	str	r5, [sp, #0x40]
    ac2c: f04f 0e09    	mov.w	lr, #0x9
    ac30: f04f 0c00    	mov.w	r12, #0x0
    ac34: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    ac36: ca07         	ldm	r2, {r0, r1, r2}
    ac38: c303         	stm	r3!, {r0, r1}
    ac3a: 4639         	mov	r1, r7
    ac3c: 701a         	strb	r2, [r3]
    ac3e: 4630         	mov	r0, r6
    ac40: 4622         	mov	r2, r4
    ac42: f8cd e044    	str.w	lr, [sp, #0x44]
    ac46: f88d c051    	strb.w	r12, [sp, #0x51]
    ac4a: f7f9 e844    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x6f78
    ac4e: 9810         	ldr	r0, [sp, #0x40]
    ac50: 42a8         	cmp	r0, r5
    ac52: d003         	beq	0xac5c <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x2dc> @ imm = #0x6
    ac54: 9912         	ldr	r1, [sp, #0x48]
    ac56: 3101         	adds	r1, #0x1
    ac58: f7f9 e952    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6d5c
    ac5c: 4620         	mov	r0, r4
    ac5e: f7f9 e87c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6f08
    ac62: 2114         	movs	r1, #0x14
    ac64: 4620         	mov	r0, r4
    ac66: f7f9 ea68    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x6b30
    ac6a: 4a28         	ldr	r2, [pc, #0xa0]         @ 0xad0c <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x38c> literal=0x39ec
    ac6c: 462b         	mov	r3, r5
    ac6e: 9510         	str	r5, [sp, #0x40]
    ac70: f04f 0e09    	mov.w	lr, #0x9
    ac74: f04f 0c00    	mov.w	r12, #0x0
    ac78: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    ac7a: ca07         	ldm	r2, {r0, r1, r2}
    ac7c: c303         	stm	r3!, {r0, r1}
    ac7e: 4639         	mov	r1, r7
    ac80: 701a         	strb	r2, [r3]
    ac82: 4630         	mov	r0, r6
    ac84: 4622         	mov	r2, r4
    ac86: f8cd e044    	str.w	lr, [sp, #0x44]
    ac8a: f88d c051    	strb.w	r12, [sp, #0x51]
    ac8e: f7f9 e822    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x6fbc
    ac92: e720         	b	0xaad6 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x156> @ imm = #-0x1c0
    ac94: 4638         	mov	r0, r7
    ac96: f7f9 ea2c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x6ba8
    ac9a: 4620         	mov	r0, r4
    ac9c: f7f9 e85c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6f48
    aca0: 4640         	mov	r0, r8
    aca2: f7f9 e85a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6f4c
    aca6: 4630         	mov	r0, r6
    aca8: f7f9 e856    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6f54
    acac: 4648         	mov	r0, r9
    acae: f7f9 e854    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6f58
    acb2: f7f9 ea66    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x6b34
    acb6: e7ed         	b	0xac94 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x314> @ imm = #-0x26
    acb8: e7f5         	b	0xaca6 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x326> @ imm = #-0x16
    acba: e7f7         	b	0xacac <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x32c> @ imm = #-0x12
    acbc: e7f0         	b	0xaca0 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x320> @ imm = #-0x20
    acbe: e7e9         	b	0xac94 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x314> @ imm = #-0x2e
    acc0: e7e8         	b	0xac94 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x314> @ imm = #-0x30
    acc2: e7e7         	b	0xac94 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x314> @ imm = #-0x32
    acc4: 4638         	mov	r0, r7
    acc6: f7f9 ea14    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x6bd8
    acca: e7e9         	b	0xaca0 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x320> @ imm = #-0x2e
    accc: e7e5         	b	0xac9a <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x31a> @ imm = #-0x36
    acce: e7f9         	b	0xacc4 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x344> @ imm = #-0xe
    acd0: e7e0         	b	0xac94 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x314> @ imm = #-0x40
    acd2: 4638         	mov	r0, r7
    acd4: f7f9 ea0c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x6be8
    acd8: e7e8         	b	0xacac <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)+0x32c> @ imm = #-0x30
    acda: bf00         	nop
    acdc: 4050         	eors	r0, r2
    acde: 0000         	movs	r0, r0
    ace0: 3ffa         	subs	r7, #0xfa
    ace2: 0000         	movs	r0, r0
    ace4: 2cb0         	cmp	r4, #0xb0
    ace6: 0000         	movs	r0, r0
    ace8: 3050         	adds	r0, #0x50
    acea: 0000         	movs	r0, r0
    acec: 3fe4         	subs	r7, #0xe4
    acee: 0000         	movs	r0, r0
    acf0: 3f1e         	subs	r7, #0x1e
    acf2: 0000         	movs	r0, r0
    acf4: 3f00         	subs	r7, #0x0
    acf6: 0000         	movs	r0, r0
    acf8: 2bc0         	cmp	r3, #0xc0
    acfa: 0000         	movs	r0, r0
    acfc: 3f00         	subs	r7, #0x0
    acfe: 0000         	movs	r0, r0
    ad00: 2b3e         	cmp	r3, #0x3e
    ad02: 0000         	movs	r0, r0
    ad04: 6662         	str	r2, [r4, #0x64]
    ad06: 0001         	movs	r1, r0
    ad08: 3a24         	subs	r2, #0x24
    ad0a: 0000         	movs	r0, r0
    ad0c: 39ec         	subs	r1, #0xec
    ad0e: 0000         	movs	r0, r0

0000ad10 <QualityManager::setQualityAttribute(pbnjson::JValue)>:
    ad10: b5f0         	push	{r4, r5, r6, r7, lr}
    ad12: b091         	sub	sp, #0x44
    ad14: ad07         	add	r5, sp, #0x1c
    ad16: 4606         	mov	r6, r0
    ad18: 4628         	mov	r0, r5
    ad1a: f7f9 e8d8    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x6e50
    ad1e: f7ff fc0d    	bl	0xa53c <getPmLogContext()> @ imm = #-0x7e6
    ad22: b390         	cbz	r0, 0xad8a <QualityManager::setQualityAttribute(pbnjson::JValue)+0x7a> @ imm = #0x64
    ad24: f7ff fc0a    	bl	0xa53c <getPmLogContext()> @ imm = #-0x7ec
    ad28: 6803         	ldr	r3, [r0]
    ad2a: 2b05         	cmp	r3, #0x5
    ad2c: dc2d         	bgt	0xad8a <QualityManager::setQualityAttribute(pbnjson::JValue)+0x7a> @ imm = #0x5a
    ad2e: 6876         	ldr	r6, [r6, #0x4]
    ad30: 2e00         	cmp	r6, #0x0
    ad32: d04d         	beq	0xadd0 <QualityManager::setQualityAttribute(pbnjson::JValue)+0xc0> @ imm = #0x9a
    ad34: ac0a         	add	r4, sp, #0x28
    ad36: 4629         	mov	r1, r5
    ad38: 4620         	mov	r0, r4
    ad3a: f7f9 e8c8    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x6e70
    ad3e: 4621         	mov	r1, r4
    ad40: 4630         	mov	r0, r6
    ad42: f7f9 ea9c    	blx	0x427c <_ZN9umiClient19setQualityAttributeEN7pbnjson6JValueE@plt> @ imm = #-0x6ac8
    ad46: 4603         	mov	r3, r0
    ad48: 4620         	mov	r0, r4
    ad4a: 461c         	mov	r4, r3
    ad4c: f7f9 e804    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x6ff8
    ad50: f7ff fbf4    	bl	0xa53c <getPmLogContext()> @ imm = #-0x818
    ad54: b120         	cbz	r0, 0xad60 <QualityManager::setQualityAttribute(pbnjson::JValue)+0x50> @ imm = #0x8
    ad56: f7ff fbf1    	bl	0xa53c <getPmLogContext()> @ imm = #-0x81e
    ad5a: 6803         	ldr	r3, [r0]
    ad5c: 2b05         	cmp	r3, #0x5
    ad5e: dd0e         	ble	0xad7e <QualityManager::setQualityAttribute(pbnjson::JValue)+0x6e> @ imm = #0x1c
    ad60: f7ff fbec    	bl	0xa53c <getPmLogContext()> @ imm = #-0x828
    ad64: 4b22         	ldr	r3, [pc, #0x88]         @ 0xadf0 <QualityManager::setQualityAttribute(pbnjson::JValue)+0xe0> literal=0x3d84
    ad66: 2200         	movs	r2, #0x0
    ad68: 2106         	movs	r1, #0x6
    ad6a: 9404         	str	r4, [sp, #0x10]
    ad6c: e9cd 2201    	strd	r2, r2, [sp, #4]
    ad70: 447b         	add	r3, pc => raw=0xeaf8 ghidra=0x1eaf8 file=60152 "setQualityAttribute umi returnStatus %d"
    ad72: 9200         	str	r2, [sp]
    ad74: 9303         	str	r3, [sp, #0xc]
    ad76: 4b1f         	ldr	r3, [pc, #0x7c]         @ 0xadf4 <QualityManager::setQualityAttribute(pbnjson::JValue)+0xe4> literal=0x3d34
    ad78: 447b         	add	r3, pc => raw=0xeab0 ghidra=0x1eab0 file=60080 "AUDIO_SET_QUALITY_ATTRIBUTE_CALL"
    ad7a: f7f9 e950    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x6d60
    ad7e: 4628         	mov	r0, r5
    ad80: f7f8 efea    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x702c
    ad84: 4620         	mov	r0, r4
    ad86: b011         	add	sp, #0x44
    ad88: bdf0         	pop	{r4, r5, r6, r7, pc}
    ad8a: f7ff fbd7    	bl	0xa53c <getPmLogContext()> @ imm = #-0x852
    ad8e: ac0a         	add	r4, sp, #0x28
    ad90: 4607         	mov	r7, r0
    ad92: 2200         	movs	r2, #0x0
    ad94: 4629         	mov	r1, r5
    ad96: 4620         	mov	r0, r4
    ad98: f7f9 ea28    	blx	0x41ec <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt> @ imm = #-0x6bb0
    ad9c: 9b0a         	ldr	r3, [sp, #0x28]
    ad9e: 2200         	movs	r2, #0x0
    ada0: 4638         	mov	r0, r7
    ada2: 2106         	movs	r1, #0x6
    ada4: e9cd 2201    	strd	r2, r2, [sp, #4]
    ada8: 9304         	str	r3, [sp, #0x10]
    adaa: 4b13         	ldr	r3, [pc, #0x4c]         @ 0xadf8 <QualityManager::setQualityAttribute(pbnjson::JValue)+0xe8> literal=0x3d22
    adac: 9200         	str	r2, [sp]
    adae: 447b         	add	r3, pc => raw=0xead4 ghidra=0x1ead4 file=60116 "setQualityAttribute request for:%s"
    adb0: 9303         	str	r3, [sp, #0xc]
    adb2: 4b12         	ldr	r3, [pc, #0x48]         @ 0xadfc <QualityManager::setQualityAttribute(pbnjson::JValue)+0xec> literal=0x3cf8
    adb4: 447b         	add	r3, pc => raw=0xeab0 ghidra=0x1eab0 file=60080 "AUDIO_SET_QUALITY_ATTRIBUTE_CALL"
    adb6: f7f9 e932    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x6d9c
    adba: 980a         	ldr	r0, [sp, #0x28]
    adbc: ab0c         	add	r3, sp, #0x30
    adbe: 4298         	cmp	r0, r3
    adc0: d0b5         	beq	0xad2e <QualityManager::setQualityAttribute(pbnjson::JValue)+0x1e> @ imm = #-0x96
    adc2: 990c         	ldr	r1, [sp, #0x30]
    adc4: 3101         	adds	r1, #0x1
    adc6: f7f9 e89c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6ec8
    adca: 6876         	ldr	r6, [r6, #0x4]
    adcc: 2e00         	cmp	r6, #0x0
    adce: d1b1         	bne	0xad34 <QualityManager::setQualityAttribute(pbnjson::JValue)+0x24> @ imm = #-0x9e
    add0: 2402         	movs	r4, #0x2
    add2: e7bd         	b	0xad50 <QualityManager::setQualityAttribute(pbnjson::JValue)+0x40> @ imm = #-0x86
    add4: 4620         	mov	r0, r4
    add6: f7f8 efc0    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7080
    adda: 4628         	mov	r0, r5
    addc: f7f8 efbc    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7088
    ade0: f7f9 e9ce    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x6c64
    ade4: e7f9         	b	0xadda <QualityManager::setQualityAttribute(pbnjson::JValue)+0xca> @ imm = #-0xe
    ade6: 4620         	mov	r0, r4
    ade8: f7f9 e982    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x6cfc
    adec: e7f5         	b	0xadda <QualityManager::setQualityAttribute(pbnjson::JValue)+0xca> @ imm = #-0x16
    adee: bf00         	nop
    adf0: 3d84         	subs	r5, #0x84
    adf2: 0000         	movs	r0, r0
    adf4: 3d34         	subs	r5, #0x34
    adf6: 0000         	movs	r0, r0
    adf8: 3d22         	subs	r5, #0x22
    adfa: 0000         	movs	r0, r0
    adfc: 3cf8         	subs	r4, #0xf8
    adfe: 0000         	movs	r0, r0

0000ae00 <QualityManager::setSettings(LSMessage&)>:
    ae00: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    ae04: b0ad         	sub	sp, #0xb4
    ae06: f8df a530    	ldr.w	r10, [pc, #0x530]       @ 0xb338 <QualityManager::setSettings(LSMessage&)+0x538> literal=0x15fa0
    ae0a: 4681         	mov	r9, r0
    ae0c: af0e         	add	r7, sp, #0x38
    ae0e: 4608         	mov	r0, r1
    ae10: 460c         	mov	r4, r1
    ae12: e9cd 910b    	strd	r9, r1, [sp, #44]
    ae16: f7f9 e966    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x6d34
    ae1a: 4638         	mov	r0, r7
    ae1c: 44fa         	add	r10, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    ae1e: f7f9 e9fe    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x6c04
    ae22: 2300         	movs	r3, #0x0
    ae24: f10d 0844    	add.w	r8, sp, #0x44
    ae28: 930d         	str	r3, [sp, #0x34]
    ae2a: 4640         	mov	r0, r8
    ae2c: 931b         	str	r3, [sp, #0x6c]
    ae2e: f88d 3070    	strb.w	r3, [sp, #0x70]
    ae32: ab1c         	add	r3, sp, #0x70
    ae34: 9309         	str	r3, [sp, #0x24]
    ae36: 931a         	str	r3, [sp, #0x68]
    ae38: f7f9 e9f0    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x6c20
    ae3c: f7ff fb7e    	bl	0xa53c <getPmLogContext()> @ imm = #-0x904
    ae40: 2800         	cmp	r0, #0x0
    ae42: f000 808a    	beq.w	0xaf5a <QualityManager::setSettings(LSMessage&)+0x15a> @ imm = #0x114
    ae46: f7ff fb79    	bl	0xa53c <getPmLogContext()> @ imm = #-0x90e
    ae4a: 6803         	ldr	r3, [r0]
    ae4c: 2b06         	cmp	r3, #0x6
    ae4e: f300 8084    	bgt.w	0xaf5a <QualityManager::setSettings(LSMessage&)+0x15a> @ imm = #0x108
    ae52: ab22         	add	r3, sp, #0x88
    ae54: f10d 0980    	add.w	r9, sp, #0x80
    ae58: ae26         	add	r6, sp, #0x98
    ae5a: 930a         	str	r3, [sp, #0x28]
    ae5c: 9320         	str	r3, [sp, #0x80]
    ae5e: 2200         	movs	r2, #0x0
    ae60: 2394         	movs	r3, #0x94
    ae62: 4631         	mov	r1, r6
    ae64: 4648         	mov	r0, r9
    ae66: 9326         	str	r3, [sp, #0x98]
    ae68: f7f9 e904    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x6df8
    ae6c: f8df 14cc    	ldr.w	r1, [pc, #0x4cc]        @ 0xb33c <QualityManager::setSettings(LSMessage&)+0x53c> literal=0x3d2a
    ae70: 2294         	movs	r2, #0x94
    ae72: 9b26         	ldr	r3, [sp, #0x98]
    ae74: 9020         	str	r0, [sp, #0x80]
    ae76: 4479         	add	r1, pc => raw=0xeba4 ghidra=0x1eba4 file=60324 "{\"type\":\"object\",\"properties\":{\"category\":{\"type\":\"string\"},\"value\":{\"type\":\"object\"}},\"required\":[\"category\",\"value\"],\"additionalProperties\":false}"
    ae78: 9322         	str	r3, [sp, #0x88]
    ae7a: f7f9 e886    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x6ef4
    ae7e: 9b26         	ldr	r3, [sp, #0x98]
    ae80: 2100         	movs	r1, #0x0
    ae82: 9a20         	ldr	r2, [sp, #0x80]
    ae84: 4620         	mov	r0, r4
    ae86: 9321         	str	r3, [sp, #0x84]
    ae88: 54d1         	strb	r1, [r2, r3]
    ae8a: f7f9 e986    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x6cf4
    ae8e: ac28         	add	r4, sp, #0xa0
    ae90: 4683         	mov	r11, r0
    ae92: 9426         	str	r4, [sp, #0x98]
    ae94: 2800         	cmp	r0, #0x0
    ae96: f000 8207    	beq.w	0xb2a8 <QualityManager::setSettings(LSMessage&)+0x4a8> @ imm = #0x40e
    ae9a: f7f9 e904    	blx	0x40a4 <strlen@plt>     @ imm = #-0x6df8
    ae9e: 280f         	cmp	r0, #0xf
    aea0: 4605         	mov	r5, r0
    aea2: 9017         	str	r0, [sp, #0x5c]
    aea4: f200 8167    	bhi.w	0xb176 <QualityManager::setSettings(LSMessage&)+0x376> @ imm = #0x2ce
    aea8: 2801         	cmp	r0, #0x1
    aeaa: d16a         	bne	0xaf82 <QualityManager::setSettings(LSMessage&)+0x182> @ imm = #0xd4
    aeac: f89b 3000    	ldrb.w	r3, [r11]
    aeb0: f88d 30a0    	strb.w	r3, [sp, #0xa0]
    aeb4: 4623         	mov	r3, r4
    aeb6: 2200         	movs	r2, #0x0
    aeb8: 9527         	str	r5, [sp, #0x9c]
    aeba: 4639         	mov	r1, r7
    aebc: 555a         	strb	r2, [r3, r5]
    aebe: 4630         	mov	r0, r6
    aec0: ab0d         	add	r3, sp, #0x34
    aec2: 464a         	mov	r2, r9
    aec4: f7ff fa88    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #-0xaf0
    aec8: 4605         	mov	r5, r0
    aeca: 9826         	ldr	r0, [sp, #0x98]
    aecc: 42a0         	cmp	r0, r4
    aece: d003         	beq	0xaed8 <QualityManager::setSettings(LSMessage&)+0xd8> @ imm = #0x6
    aed0: 9928         	ldr	r1, [sp, #0xa0]
    aed2: 3101         	adds	r1, #0x1
    aed4: f7f9 e814    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x6fd8
    aed8: 2d00         	cmp	r5, #0x0
    aeda: d157         	bne	0xaf8c <QualityManager::setSettings(LSMessage&)+0x18c> @ imm = #0xae
    aedc: f8df 1460    	ldr.w	r1, [pc, #0x460]        @ 0xb340 <QualityManager::setSettings(LSMessage&)+0x540> literal=0x1639a
    aee0: 462b         	mov	r3, r5
    aee2: 2203         	movs	r2, #0x3
    aee4: a80c         	add	r0, sp, #0x30
    aee6: 4479         	add	r1, pc => raw=0x21284 ghidra=0x31284 file=null
    aee8: 311c         	adds	r1, #0x1c
    aeea: f7fe feaf    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x12a2
    aeee: f7ff fb25    	bl	0xa53c <getPmLogContext()> @ imm = #-0x9b6
    aef2: b120         	cbz	r0, 0xaefe <QualityManager::setSettings(LSMessage&)+0xfe> @ imm = #0x8
    aef4: f7ff fb22    	bl	0xa53c <getPmLogContext()> @ imm = #-0x9bc
    aef8: 6803         	ldr	r3, [r0]
    aefa: 2b05         	cmp	r3, #0x5
    aefc: dd0f         	ble	0xaf1e <QualityManager::setSettings(LSMessage&)+0x11e> @ imm = #0x1e
    aefe: f7ff fb1d    	bl	0xa53c <getPmLogContext()> @ imm = #-0x9c6
    af02: f8df 3440    	ldr.w	r3, [pc, #0x440]        @ 0xb344 <QualityManager::setSettings(LSMessage&)+0x544> literal=0x3d3c
    af06: 2200         	movs	r2, #0x0
    af08: 2106         	movs	r1, #0x6
    af0a: e9cd 2201    	strd	r2, r2, [sp, #4]
    af0e: 9200         	str	r2, [sp]
    af10: 447b         	add	r3, pc => raw=0xec50 ghidra=0x1ec50 file=60496 "error for schema"
    af12: 9303         	str	r3, [sp, #0xc]
    af14: f8df 3430    	ldr.w	r3, [pc, #0x430]        @ 0xb348 <QualityManager::setSettings(LSMessage&)+0x548> literal=0x3d20
    af18: 447b         	add	r3, pc => raw=0xec3c ghidra=0x1ec3c file=60476 "JSON_PARSE_ERROR"
    af1a: f7f9 e880    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x6f00
    af1e: 9820         	ldr	r0, [sp, #0x80]
    af20: 9b0a         	ldr	r3, [sp, #0x28]
    af22: 4298         	cmp	r0, r3
    af24: d003         	beq	0xaf2e <QualityManager::setSettings(LSMessage&)+0x12e> @ imm = #0x6
    af26: 9922         	ldr	r1, [sp, #0x88]
    af28: 3101         	adds	r1, #0x1
    af2a: f7f8 efea    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x702c
    af2e: 4640         	mov	r0, r8
    af30: f7f8 ef12    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x71dc
    af34: 981a         	ldr	r0, [sp, #0x68]
    af36: 9b09         	ldr	r3, [sp, #0x24]
    af38: 4298         	cmp	r0, r3
    af3a: d003         	beq	0xaf44 <QualityManager::setSettings(LSMessage&)+0x144> @ imm = #0x6
    af3c: 991c         	ldr	r1, [sp, #0x70]
    af3e: 3101         	adds	r1, #0x1
    af40: f7f8 efde    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7044
    af44: 4638         	mov	r0, r7
    af46: f7f8 ef08    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x71f0
    af4a: 980c         	ldr	r0, [sp, #0x30]
    af4c: b108         	cbz	r0, 0xaf52 <QualityManager::setSettings(LSMessage&)+0x152> @ imm = #0x2
    af4e: f7f9 e87e    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x6f04
    af52: 2001         	movs	r0, #0x1
    af54: b02d         	add	sp, #0xb4
    af56: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    af5a: f7ff faef    	bl	0xa53c <getPmLogContext()> @ imm = #-0xa22
    af5e: 4bfb         	ldr	r3, [pc, #0x3ec]        @ 0xb34c <QualityManager::setSettings(LSMessage&)+0x54c> literal=0x3bba
    af60: 2107         	movs	r1, #0x7
    af62: 447b         	add	r3, pc => raw=0xeb20 ghidra=0x1eb20 file=60192 "setSettings"
    af64: 9305         	str	r3, [sp, #0x14]
    af66: 4bfa         	ldr	r3, [pc, #0x3e8]        @ 0xb350 <QualityManager::setSettings(LSMessage&)+0x550> literal=0x3bc0
    af68: 447b         	add	r3, pc => raw=0xeb2c ghidra=0x1eb2c file=60204 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/qualitymanager.cpp"
    af6a: 9304         	str	r3, [sp, #0x10]
    af6c: 4bf9         	ldr	r3, [pc, #0x3e4]        @ 0xb354 <QualityManager::setSettings(LSMessage&)+0x554> literal=0x3c0e
    af6e: 447b         	add	r3, pc => raw=0xeb80 ghidra=0x1eb80 file=60288 "%s:%s() Quality setSettings  call"
    af70: 9303         	str	r3, [sp, #0xc]
    af72: 2300         	movs	r3, #0x0
    af74: 461a         	mov	r2, r3
    af76: e9cd 3301    	strd	r3, r3, [sp, #4]
    af7a: 9300         	str	r3, [sp]
    af7c: f7f9 e84e    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x6f64
    af80: e767         	b	0xae52 <QualityManager::setSettings(LSMessage&)+0x52> @ imm = #-0x132
    af82: 4623         	mov	r3, r4
    af84: 2800         	cmp	r0, #0x0
    af86: d096         	beq	0xaeb6 <QualityManager::setSettings(LSMessage&)+0xb6> @ imm = #-0xd4
    af88: 4620         	mov	r0, r4
    af8a: e0fc         	b	0xb186 <QualityManager::setSettings(LSMessage&)+0x386> @ imm = #0x1f8
    af8c: 4af2         	ldr	r2, [pc, #0x3c8]        @ 0xb358 <QualityManager::setSettings(LSMessage&)+0x558> literal=0x3ccc
    af8e: ad17         	add	r5, sp, #0x5c
    af90: 4639         	mov	r1, r7
    af92: 4628         	mov	r0, r5
    af94: 447a         	add	r2, pc => raw=0xec64 ghidra=0x1ec64 file=60516 "category"
    af96: f7f8 efba    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x708c
    af9a: 2300         	movs	r3, #0x0
    af9c: 4631         	mov	r1, r6
    af9e: 4628         	mov	r0, r5
    afa0: 9426         	str	r4, [sp, #0x98]
    afa2: 9327         	str	r3, [sp, #0x9c]
    afa4: f88d 30a0    	strb.w	r3, [sp, #0xa0]
    afa8: f7f8 eefa    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x720c
    afac: 9b26         	ldr	r3, [sp, #0x98]
    afae: 981a         	ldr	r0, [sp, #0x68]
    afb0: 9a27         	ldr	r2, [sp, #0x9c]
    afb2: 42a3         	cmp	r3, r4
    afb4: f000 8161    	beq.w	0xb27a <QualityManager::setSettings(LSMessage&)+0x47a> @ imm = #0x2c2
    afb8: 9909         	ldr	r1, [sp, #0x24]
    afba: f8dd c0a0    	ldr.w	r12, [sp, #0xa0]
    afbe: e9cd 321a    	strd	r3, r2, [sp, #104]
    afc2: 4288         	cmp	r0, r1
    afc4: bf08         	it	eq
    afc6: f8cd c070    	streq.w	r12, [sp, #0x70]
    afca: f000 8163    	beq.w	0xb294 <QualityManager::setSettings(LSMessage&)+0x494> @ imm = #0x2c6
    afce: 9b1c         	ldr	r3, [sp, #0x70]
    afd0: f8cd c070    	str.w	r12, [sp, #0x70]
    afd4: 2800         	cmp	r0, #0x0
    afd6: f000 815d    	beq.w	0xb294 <QualityManager::setSettings(LSMessage&)+0x494> @ imm = #0x2ba
    afda: 9328         	str	r3, [sp, #0xa0]
    afdc: 9026         	str	r0, [sp, #0x98]
    afde: 2300         	movs	r3, #0x0
    afe0: 9327         	str	r3, [sp, #0x9c]
    afe2: 7003         	strb	r3, [r0]
    afe4: 9826         	ldr	r0, [sp, #0x98]
    afe6: 42a0         	cmp	r0, r4
    afe8: d003         	beq	0xaff2 <QualityManager::setSettings(LSMessage&)+0x1f2> @ imm = #0x6
    afea: 9928         	ldr	r1, [sp, #0xa0]
    afec: 3101         	adds	r1, #0x1
    afee: f7f8 ef88    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x70f0
    aff2: 4bda         	ldr	r3, [pc, #0x368]        @ 0xb35c <QualityManager::setSettings(LSMessage&)+0x55c> literal=0x210
    aff4: 4628         	mov	r0, r5
    aff6: f85a a003    	ldr.w	r10, [r10, r3]
    affa: f10a 0308    	add.w	r3, r10, #0x8
    affe: 9317         	str	r3, [sp, #0x5c]
    b000: f7f8 efa4    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x70b8
    b004: 4ad6         	ldr	r2, [pc, #0x358]        @ 0xb360 <QualityManager::setSettings(LSMessage&)+0x560> literal=0x3c62
    b006: 4639         	mov	r1, r7
    b008: 4628         	mov	r0, r5
    b00a: 447a         	add	r2, pc => raw=0xec70 ghidra=0x1ec70 file=60528 "value"
    b00c: f7f8 ef7e    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x7104
    b010: f10a 0308    	add.w	r3, r10, #0x8
    b014: 9a12         	ldr	r2, [sp, #0x48]
    b016: 9317         	str	r3, [sp, #0x5c]
    b018: 4628         	mov	r0, r5
    b01a: 9b18         	ldr	r3, [sp, #0x60]
    b01c: 9218         	str	r2, [sp, #0x60]
    b01e: 9a13         	ldr	r2, [sp, #0x4c]
    b020: 9312         	str	r3, [sp, #0x48]
    b022: 9b19         	ldr	r3, [sp, #0x64]
    b024: 9219         	str	r2, [sp, #0x64]
    b026: 9313         	str	r3, [sp, #0x4c]
    b028: f7f8 ef90    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x70e0
    b02c: f7ff fa86    	bl	0xa53c <getPmLogContext()> @ imm = #-0xaf4
    b030: 2800         	cmp	r0, #0x0
    b032: d077         	beq	0xb124 <QualityManager::setSettings(LSMessage&)+0x324> @ imm = #0xee
    b034: f7ff fa82    	bl	0xa53c <getPmLogContext()> @ imm = #-0xafc
    b038: 6803         	ldr	r3, [r0]
    b03a: 2b06         	cmp	r3, #0x6
    b03c: dc72         	bgt	0xb124 <QualityManager::setSettings(LSMessage&)+0x324> @ imm = #0xe4
    b03e: f10d 0a50    	add.w	r10, sp, #0x50
    b042: 4650         	mov	r0, r10
    b044: f7f8 eeb8    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x7290
    b048: 4641         	mov	r1, r8
    b04a: 4630         	mov	r0, r6
    b04c: f10d 0b68    	add.w	r11, sp, #0x68
    b050: f7f8 ef3c    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x7188
    b054: 980b         	ldr	r0, [sp, #0x2c]
    b056: 4632         	mov	r2, r6
    b058: 4659         	mov	r1, r11
    b05a: f7ff fbcf    	bl	0xa7fc <QualityManager::setSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)> @ imm = #-0x862
    b05e: 900b         	str	r0, [sp, #0x2c]
    b060: 4630         	mov	r0, r6
    b062: f7f8 ee7a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x730c
    b066: 9b0b         	ldr	r3, [sp, #0x2c]
    b068: 2b00         	cmp	r3, #0x0
    b06a: f040 8093    	bne.w	0xb194 <QualityManager::setSettings(LSMessage&)+0x394> @ imm = #0x126
    b06e: f7ff fa65    	bl	0xa53c <getPmLogContext()> @ imm = #-0xb36
    b072: b120         	cbz	r0, 0xb07e <QualityManager::setSettings(LSMessage&)+0x27e> @ imm = #0x8
    b074: f7ff fa62    	bl	0xa53c <getPmLogContext()> @ imm = #-0xb3c
    b078: 6803         	ldr	r3, [r0]
    b07a: 2b06         	cmp	r3, #0x6
    b07c: dd14         	ble	0xb0a8 <QualityManager::setSettings(LSMessage&)+0x2a8> @ imm = #0x28
    b07e: f7ff fa5d    	bl	0xa53c <getPmLogContext()> @ imm = #-0xb46
    b082: 9b1a         	ldr	r3, [sp, #0x68]
    b084: 2107         	movs	r1, #0x7
    b086: 9306         	str	r3, [sp, #0x18]
    b088: 4bb6         	ldr	r3, [pc, #0x2d8]        @ 0xb364 <QualityManager::setSettings(LSMessage&)+0x564> literal=0x3a92
    b08a: 447b         	add	r3, pc => raw=0xeb20 ghidra=0x1eb20 file=60192 "setSettings"
    b08c: 9305         	str	r3, [sp, #0x14]
    b08e: 4bb6         	ldr	r3, [pc, #0x2d8]        @ 0xb368 <QualityManager::setSettings(LSMessage&)+0x568> literal=0x3a98
    b090: 447b         	add	r3, pc => raw=0xeb2c ghidra=0x1eb2c file=60204 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/qualitymanager.cpp"
    b092: 9304         	str	r3, [sp, #0x10]
    b094: 4bb5         	ldr	r3, [pc, #0x2d4]        @ 0xb36c <QualityManager::setSettings(LSMessage&)+0x56c> literal=0x3c1e
    b096: 447b         	add	r3, pc => raw=0xecb8 ghidra=0x1ecb8 file=60600 "%s:%s() umi setSettings for category %s is success"
    b098: 9303         	str	r3, [sp, #0xc]
    b09a: 2300         	movs	r3, #0x0
    b09c: 461a         	mov	r2, r3
    b09e: e9cd 3301    	strd	r3, r3, [sp, #4]
    b0a2: 9300         	str	r3, [sp]
    b0a4: f7f8 efba    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x708c
    b0a8: 2101         	movs	r1, #0x1
    b0aa: 4628         	mov	r0, r5
    b0ac: f7f8 eea2    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x72bc
    b0b0: 4aaf         	ldr	r2, [pc, #0x2bc]        @ 0xb370 <QualityManager::setSettings(LSMessage&)+0x570> literal=0x2662
    b0b2: 4623         	mov	r3, r4
    b0b4: 9426         	str	r4, [sp, #0x98]
    b0b6: f04f 0e0b    	mov.w	lr, #0xb
    b0ba: f04f 0c00    	mov.w	r12, #0x0
    b0be: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    b0c0: ca07         	ldm	r2, {r0, r1, r2}
    b0c2: c303         	stm	r3!, {r0, r1}
    b0c4: 4631         	mov	r1, r6
    b0c6: f823 2b02    	strh	r2, [r3], #2
    b0ca: 0c12         	lsrs	r2, r2, #0x10
    b0cc: 4650         	mov	r0, r10
    b0ce: 701a         	strb	r2, [r3]
    b0d0: 462a         	mov	r2, r5
    b0d2: f8cd e09c    	str.w	lr, [sp, #0x9c]
    b0d6: f88d c0ab    	strb.w	r12, [sp, #0xab]
    b0da: f7f8 edfc    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x7408
    b0de: 9826         	ldr	r0, [sp, #0x98]
    b0e0: 42a0         	cmp	r0, r4
    b0e2: d003         	beq	0xb0ec <QualityManager::setSettings(LSMessage&)+0x2ec> @ imm = #0x6
    b0e4: 9928         	ldr	r1, [sp, #0xa0]
    b0e6: 3101         	adds	r1, #0x1
    b0e8: f7f8 ef0a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x71ec
    b0ec: 4628         	mov	r0, r5
    b0ee: f7f8 ee34    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7398
    b0f2: 4659         	mov	r1, r11
    b0f4: 4628         	mov	r0, r5
    b0f6: f7f9 e81a    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x6fcc
    b0fa: 4b9e         	ldr	r3, [pc, #0x278]        @ 0xb374 <QualityManager::setSettings(LSMessage&)+0x574> literal=0x3b58
    b0fc: f04f 0e08    	mov.w	lr, #0x8
    b100: f04f 0c00    	mov.w	r12, #0x0
    b104: 9426         	str	r4, [sp, #0x98]
    b106: 462a         	mov	r2, r5
    b108: 447b         	add	r3, pc => raw=0xec64 ghidra=0x1ec64 file=60516 "category"
    b10a: e893 0003    	ldm.w	r3, {r0, r1}
    b10e: e884 0003    	stm.w	r4, {r0, r1}
    b112: 4631         	mov	r1, r6
    b114: 4650         	mov	r0, r10
    b116: f8cd e09c    	str.w	lr, [sp, #0x9c]
    b11a: f88d c0a8    	strb.w	r12, [sp, #0xa8]
    b11e: f7f8 edda    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x744c
    b122: e098         	b	0xb256 <QualityManager::setSettings(LSMessage&)+0x456> @ imm = #0x130
    b124: f7ff fa0a    	bl	0xa53c <getPmLogContext()> @ imm = #-0xbec
    b128: 4682         	mov	r10, r0
    b12a: 2200         	movs	r2, #0x0
    b12c: 4641         	mov	r1, r8
    b12e: 4630         	mov	r0, r6
    b130: f8dd b068    	ldr.w	r11, [sp, #0x68]
    b134: f7f9 e85a    	blx	0x41ec <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt> @ imm = #-0x6f4c
    b138: 9b26         	ldr	r3, [sp, #0x98]
    b13a: 4650         	mov	r0, r10
    b13c: 2107         	movs	r1, #0x7
    b13e: f8cd b018    	str.w	r11, [sp, #0x18]
    b142: 9307         	str	r3, [sp, #0x1c]
    b144: 4b8c         	ldr	r3, [pc, #0x230]        @ 0xb378 <QualityManager::setSettings(LSMessage&)+0x578> literal=0x39d6
    b146: 447b         	add	r3, pc => raw=0xeb20 ghidra=0x1eb20 file=60192 "setSettings"
    b148: 9305         	str	r3, [sp, #0x14]
    b14a: 4b8c         	ldr	r3, [pc, #0x230]        @ 0xb37c <QualityManager::setSettings(LSMessage&)+0x57c> literal=0x39dc
    b14c: 447b         	add	r3, pc => raw=0xeb2c ghidra=0x1eb2c file=60204 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/qualitymanager.cpp"
    b14e: 9304         	str	r3, [sp, #0x10]
    b150: 4b8b         	ldr	r3, [pc, #0x22c]        @ 0xb380 <QualityManager::setSettings(LSMessage&)+0x580> literal=0x3b22
    b152: 447b         	add	r3, pc => raw=0xec78 ghidra=0x1ec78 file=60536 "%s:%s() Quality setSettings Request for category %s, value %s"
    b154: 9303         	str	r3, [sp, #0xc]
    b156: 2300         	movs	r3, #0x0
    b158: 461a         	mov	r2, r3
    b15a: e9cd 3301    	strd	r3, r3, [sp, #4]
    b15e: 9300         	str	r3, [sp]
    b160: f7f8 ef5c    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x7148
    b164: 9826         	ldr	r0, [sp, #0x98]
    b166: 42a0         	cmp	r0, r4
    b168: f43f af69    	beq.w	0xb03e <QualityManager::setSettings(LSMessage&)+0x23e> @ imm = #-0x12e
    b16c: 9928         	ldr	r1, [sp, #0xa0]
    b16e: 3101         	adds	r1, #0x1
    b170: f7f8 eec6    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7274
    b174: e763         	b	0xb03e <QualityManager::setSettings(LSMessage&)+0x23e> @ imm = #-0x13a
    b176: 2200         	movs	r2, #0x0
    b178: a917         	add	r1, sp, #0x5c
    b17a: 4630         	mov	r0, r6
    b17c: f7f8 ef7a    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x710c
    b180: 9b17         	ldr	r3, [sp, #0x5c]
    b182: 9026         	str	r0, [sp, #0x98]
    b184: 9328         	str	r3, [sp, #0xa0]
    b186: 462a         	mov	r2, r5
    b188: 4659         	mov	r1, r11
    b18a: f7f8 eefe    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x7204
    b18e: 9d17         	ldr	r5, [sp, #0x5c]
    b190: 9b26         	ldr	r3, [sp, #0x98]
    b192: e690         	b	0xaeb6 <QualityManager::setSettings(LSMessage&)+0xb6> @ imm = #-0x2e0
    b194: 2100         	movs	r1, #0x0
    b196: 4628         	mov	r0, r5
    b198: f7f8 ee2c    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x73a8
    b19c: 4a79         	ldr	r2, [pc, #0x1e4]        @ 0xb384 <QualityManager::setSettings(LSMessage&)+0x584> literal=0x2576
    b19e: 4623         	mov	r3, r4
    b1a0: 9426         	str	r4, [sp, #0x98]
    b1a2: f04f 0e0b    	mov.w	lr, #0xb
    b1a6: f04f 0c00    	mov.w	r12, #0x0
    b1aa: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    b1ac: ca07         	ldm	r2, {r0, r1, r2}
    b1ae: c303         	stm	r3!, {r0, r1}
    b1b0: 4631         	mov	r1, r6
    b1b2: f823 2b02    	strh	r2, [r3], #2
    b1b6: 0c12         	lsrs	r2, r2, #0x10
    b1b8: 4650         	mov	r0, r10
    b1ba: 701a         	strb	r2, [r3]
    b1bc: 462a         	mov	r2, r5
    b1be: f8cd e09c    	str.w	lr, [sp, #0x9c]
    b1c2: f88d c0ab    	strb.w	r12, [sp, #0xab]
    b1c6: f7f8 ed86    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x74f4
    b1ca: 9826         	ldr	r0, [sp, #0x98]
    b1cc: 42a0         	cmp	r0, r4
    b1ce: d003         	beq	0xb1d8 <QualityManager::setSettings(LSMessage&)+0x3d8> @ imm = #0x6
    b1d0: 9928         	ldr	r1, [sp, #0xa0]
    b1d2: 3101         	adds	r1, #0x1
    b1d4: f7f8 ee94    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x72d8
    b1d8: 4628         	mov	r0, r5
    b1da: f7f8 edbe    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7484
    b1de: 496a         	ldr	r1, [pc, #0x1a8]        @ 0xb388 <QualityManager::setSettings(LSMessage&)+0x588> literal=0x1609e
    b1e0: 4628         	mov	r0, r5
    b1e2: 4479         	add	r1, pc => raw=0x21284 ghidra=0x31284 file=null
    b1e4: 3164         	adds	r1, #0x64
    b1e6: f7f8 efa2    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x70bc
    b1ea: 4a68         	ldr	r2, [pc, #0x1a0]        @ 0xb38c <QualityManager::setSettings(LSMessage&)+0x58c> literal=0x3460
    b1ec: 4623         	mov	r3, r4
    b1ee: 9426         	str	r4, [sp, #0x98]
    b1f0: f04f 0e09    	mov.w	lr, #0x9
    b1f4: f04f 0c00    	mov.w	r12, #0x0
    b1f8: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    b1fa: ca07         	ldm	r2, {r0, r1, r2}
    b1fc: c303         	stm	r3!, {r0, r1}
    b1fe: 4631         	mov	r1, r6
    b200: 701a         	strb	r2, [r3]
    b202: 4650         	mov	r0, r10
    b204: 462a         	mov	r2, r5
    b206: f8cd e09c    	str.w	lr, [sp, #0x9c]
    b20a: f88d c0a9    	strb.w	r12, [sp, #0xa9]
    b20e: f7f8 ed62    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x753c
    b212: 9826         	ldr	r0, [sp, #0x98]
    b214: 42a0         	cmp	r0, r4
    b216: d003         	beq	0xb220 <QualityManager::setSettings(LSMessage&)+0x420> @ imm = #0x6
    b218: 9928         	ldr	r1, [sp, #0xa0]
    b21a: 3101         	adds	r1, #0x1
    b21c: f7f8 ee70    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7320
    b220: 4628         	mov	r0, r5
    b222: f7f8 ed9a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x74cc
    b226: 2114         	movs	r1, #0x14
    b228: 4628         	mov	r0, r5
    b22a: f7f8 ef86    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x70f4
    b22e: 4a58         	ldr	r2, [pc, #0x160]        @ 0xb390 <QualityManager::setSettings(LSMessage&)+0x590> literal=0x3428
    b230: 4623         	mov	r3, r4
    b232: 9426         	str	r4, [sp, #0x98]
    b234: f04f 0e09    	mov.w	lr, #0x9
    b238: f04f 0c00    	mov.w	r12, #0x0
    b23c: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    b23e: ca07         	ldm	r2, {r0, r1, r2}
    b240: c303         	stm	r3!, {r0, r1}
    b242: 4631         	mov	r1, r6
    b244: 701a         	strb	r2, [r3]
    b246: 4650         	mov	r0, r10
    b248: 462a         	mov	r2, r5
    b24a: f8cd e09c    	str.w	lr, [sp, #0x9c]
    b24e: f88d c0a9    	strb.w	r12, [sp, #0xa9]
    b252: f7f8 ed40    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x7580
    b256: 9826         	ldr	r0, [sp, #0x98]
    b258: 42a0         	cmp	r0, r4
    b25a: d003         	beq	0xb264 <QualityManager::setSettings(LSMessage&)+0x464> @ imm = #0x6
    b25c: 9928         	ldr	r1, [sp, #0xa0]
    b25e: 3101         	adds	r1, #0x1
    b260: f7f8 ee4e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7364
    b264: 4628         	mov	r0, r5
    b266: f7f8 ed78    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7510
    b26a: 4651         	mov	r1, r10
    b26c: a80c         	add	r0, sp, #0x30
    b26e: f7fe fe0f    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #-0x13e2
    b272: 4650         	mov	r0, r10
    b274: f7f8 ed70    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7520
    b278: e651         	b	0xaf1e <QualityManager::setSettings(LSMessage&)+0x11e> @ imm = #-0x35e
    b27a: b132         	cbz	r2, 0xb28a <QualityManager::setSettings(LSMessage&)+0x48a> @ imm = #0xc
    b27c: 2a01         	cmp	r2, #0x1
    b27e: d00d         	beq	0xb29c <QualityManager::setSettings(LSMessage&)+0x49c> @ imm = #0x1a
    b280: 4621         	mov	r1, r4
    b282: f7f8 ee82    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x72fc
    b286: 9a27         	ldr	r2, [sp, #0x9c]
    b288: 981a         	ldr	r0, [sp, #0x68]
    b28a: 2300         	movs	r3, #0x0
    b28c: 921b         	str	r2, [sp, #0x6c]
    b28e: 5483         	strb	r3, [r0, r2]
    b290: 9826         	ldr	r0, [sp, #0x98]
    b292: e6a4         	b	0xafde <QualityManager::setSettings(LSMessage&)+0x1de> @ imm = #-0x2b8
    b294: 9426         	str	r4, [sp, #0x98]
    b296: ac28         	add	r4, sp, #0xa0
    b298: 4620         	mov	r0, r4
    b29a: e6a0         	b	0xafde <QualityManager::setSettings(LSMessage&)+0x1de> @ imm = #-0x2c0
    b29c: f89d 30a0    	ldrb.w	r3, [sp, #0xa0]
    b2a0: 7003         	strb	r3, [r0]
    b2a2: 9a27         	ldr	r2, [sp, #0x9c]
    b2a4: 981a         	ldr	r0, [sp, #0x68]
    b2a6: e7f0         	b	0xb28a <QualityManager::setSettings(LSMessage&)+0x48a> @ imm = #-0x20
    b2a8: 483a         	ldr	r0, [pc, #0xe8]         @ 0xb394 <QualityManager::setSettings(LSMessage&)+0x594> literal=0x23ea
    b2aa: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    b2ac: f7f8 ed2a    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x75ac
    b2b0: f10d 0b68    	add.w	r11, sp, #0x68
    b2b4: 4640         	mov	r0, r8
    b2b6: f7f8 ed50    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7560
    b2ba: 4658         	mov	r0, r11
    b2bc: f7f8 ef18    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x71d0
    b2c0: 4638         	mov	r0, r7
    b2c2: f7f8 ed4a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x756c
    b2c6: 980c         	ldr	r0, [sp, #0x30]
    b2c8: b108         	cbz	r0, 0xb2ce <QualityManager::setSettings(LSMessage&)+0x4ce> @ imm = #0x2
    b2ca: f7f8 eec0    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x7280
    b2ce: f7f8 ef58    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x7150
    b2d2: f10d 0b68    	add.w	r11, sp, #0x68
    b2d6: e7f0         	b	0xb2ba <QualityManager::setSettings(LSMessage&)+0x4ba> @ imm = #-0x20
    b2d8: 4630         	mov	r0, r6
    b2da: f10d 0b68    	add.w	r11, sp, #0x68
    b2de: f7f8 ef08    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x71f0
    b2e2: e008         	b	0xb2f6 <QualityManager::setSettings(LSMessage&)+0x4f6> @ imm = #0x10
    b2e4: 4630         	mov	r0, r6
    b2e6: f7f8 ef04    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x71f8
    b2ea: 4628         	mov	r0, r5
    b2ec: f7f8 ed34    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7598
    b2f0: 4650         	mov	r0, r10
    b2f2: f7f8 ed32    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x759c
    b2f6: 4648         	mov	r0, r9
    b2f8: f7f8 eefa    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x720c
    b2fc: e7da         	b	0xb2b4 <QualityManager::setSettings(LSMessage&)+0x4b4> @ imm = #-0x4c
    b2fe: 980c         	ldr	r0, [sp, #0x30]
    b300: e7e3         	b	0xb2ca <QualityManager::setSettings(LSMessage&)+0x4ca> @ imm = #-0x3a
    b302: f10d 0b68    	add.w	r11, sp, #0x68
    b306: e7f6         	b	0xb2f6 <QualityManager::setSettings(LSMessage&)+0x4f6> @ imm = #-0x14
    b308: e7ec         	b	0xb2e4 <QualityManager::setSettings(LSMessage&)+0x4e4> @ imm = #-0x28
    b30a: e7eb         	b	0xb2e4 <QualityManager::setSettings(LSMessage&)+0x4e4> @ imm = #-0x2a
    b30c: e7e4         	b	0xb2d8 <QualityManager::setSettings(LSMessage&)+0x4d8> @ imm = #-0x38
    b30e: 4630         	mov	r0, r6
    b310: f7f8 ed22    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x75bc
    b314: e7ec         	b	0xb2f0 <QualityManager::setSettings(LSMessage&)+0x4f0> @ imm = #-0x28
    b316: e7eb         	b	0xb2f0 <QualityManager::setSettings(LSMessage&)+0x4f0> @ imm = #-0x2a
    b318: 4630         	mov	r0, r6
    b31a: f10d 0b68    	add.w	r11, sp, #0x68
    b31e: f7f8 eee8    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7230
    b322: 4b0e         	ldr	r3, [pc, #0x38]         @ 0xb35c <QualityManager::setSettings(LSMessage&)+0x55c> literal=0x210
    b324: 4628         	mov	r0, r5
    b326: f85a 3003    	ldr.w	r3, [r10, r3]
    b32a: 3308         	adds	r3, #0x8
    b32c: 9317         	str	r3, [sp, #0x5c]
    b32e: f7f8 ee0e    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x73e4
    b332: e7e0         	b	0xb2f6 <QualityManager::setSettings(LSMessage&)+0x4f6> @ imm = #-0x40
    b334: e7d6         	b	0xb2e4 <QualityManager::setSettings(LSMessage&)+0x4e4> @ imm = #-0x54
    b336: e7d5         	b	0xb2e4 <QualityManager::setSettings(LSMessage&)+0x4e4> @ imm = #-0x56
    b338: 5fa0         	ldrsh	r0, [r4, r6]
    b33a: 0001         	movs	r1, r0
    b33c: 3d2a         	subs	r5, #0x2a
    b33e: 0000         	movs	r0, r0
    b340: 639a         	str	r2, [r3, #0x38]
    b342: 0001         	movs	r1, r0
    b344: 3d3c         	subs	r5, #0x3c
    b346: 0000         	movs	r0, r0
    b348: 3d20         	subs	r5, #0x20
    b34a: 0000         	movs	r0, r0
    b34c: 3bba         	subs	r3, #0xba
    b34e: 0000         	movs	r0, r0
    b350: 3bc0         	subs	r3, #0xc0
    b352: 0000         	movs	r0, r0
    b354: 3c0e         	subs	r4, #0xe
    b356: 0000         	movs	r0, r0
    b358: 3ccc         	subs	r4, #0xcc
    b35a: 0000         	movs	r0, r0
    b35c: 0210         	lsls	r0, r2, #0x8
    b35e: 0000         	movs	r0, r0
    b360: 3c62         	subs	r4, #0x62
    b362: 0000         	movs	r0, r0
    b364: 3a92         	subs	r2, #0x92
    b366: 0000         	movs	r0, r0
    b368: 3a98         	subs	r2, #0x98
    b36a: 0000         	movs	r0, r0
    b36c: 3c1e         	subs	r4, #0x1e
    b36e: 0000         	movs	r0, r0
    b370: 2662         	movs	r6, #0x62
    b372: 0000         	movs	r0, r0
    b374: 3b58         	subs	r3, #0x58
    b376: 0000         	movs	r0, r0
    b378: 39d6         	subs	r1, #0xd6
    b37a: 0000         	movs	r0, r0
    b37c: 39dc         	subs	r1, #0xdc
    b37e: 0000         	movs	r0, r0
    b380: 3b22         	subs	r3, #0x22
    b382: 0000         	movs	r0, r0
    b384: 2576         	movs	r5, #0x76
    b386: 0000         	movs	r0, r0
    b388: 609e         	str	r6, [r3, #0x8]
    b38a: 0001         	movs	r1, r0
    b38c: 3460         	adds	r4, #0x60
    b38e: 0000         	movs	r0, r0
    b390: 3428         	adds	r4, #0x28
    b392: 0000         	movs	r0, r0
    b394: 23ea         	movs	r3, #0xea
    b396: 0000         	movs	r0, r0

0000b398 <QualityManager::getSettings(LSMessage&)>:
    b398: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    b39c: 4603         	mov	r3, r0
    b39e: b0a9         	sub	sp, #0xa4
    b3a0: 4608         	mov	r0, r1
    b3a2: 460c         	mov	r4, r1
    b3a4: af07         	add	r7, sp, #0x1c
    b3a6: 9303         	str	r3, [sp, #0xc]
    b3a8: 9105         	str	r1, [sp, #0x14]
    b3aa: f7f8 ee9c    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x72c8
    b3ae: 4bad         	ldr	r3, [pc, #0x2b4]        @ 0xb664 <QualityManager::getSettings(LSMessage&)+0x2cc> literal=0x15a0a
    b3b0: 4638         	mov	r0, r7
    b3b2: 447b         	add	r3, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    b3b4: 9302         	str	r3, [sp, #0x8]
    b3b6: f7f8 ef32    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x719c
    b3ba: f10d 0928    	add.w	r9, sp, #0x28
    b3be: 2300         	movs	r3, #0x0
    b3c0: f10d 0a60    	add.w	r10, sp, #0x60
    b3c4: 4648         	mov	r0, r9
    b3c6: f8cd a058    	str.w	r10, [sp, #0x58]
    b3ca: 9317         	str	r3, [sp, #0x5c]
    b3cc: f88d 3060    	strb.w	r3, [sp, #0x60]
    b3d0: f7f8 ef24    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x71b8
    b3d4: ab1e         	add	r3, sp, #0x78
    b3d6: f10d 0b70    	add.w	r11, sp, #0x70
    b3da: ae22         	add	r6, sp, #0x88
    b3dc: 2200         	movs	r2, #0x0
    b3de: 9301         	str	r3, [sp, #0x4]
    b3e0: 4631         	mov	r1, r6
    b3e2: 931c         	str	r3, [sp, #0x70]
    b3e4: 4658         	mov	r0, r11
    b3e6: 2377         	movs	r3, #0x77
    b3e8: 9206         	str	r2, [sp, #0x18]
    b3ea: 9322         	str	r3, [sp, #0x88]
    b3ec: f7f8 ee42    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x737c
    b3f0: 499d         	ldr	r1, [pc, #0x274]        @ 0xb668 <QualityManager::getSettings(LSMessage&)+0x2d0> literal=0x38f0
    b3f2: 2277         	movs	r2, #0x77
    b3f4: 9b22         	ldr	r3, [sp, #0x88]
    b3f6: 901c         	str	r0, [sp, #0x70]
    b3f8: 4479         	add	r1, pc => raw=0xecec ghidra=0x1ecec file=60652 "{\"type\":\"object\",\"properties\":{\"category\":{\"type\":\"string\"},\"settings\":{\"type\":\"object\"}},\"additionalProperties\":false}"
    b3fa: 931e         	str	r3, [sp, #0x78]
    b3fc: f7f8 edc4    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x7478
    b400: 9b22         	ldr	r3, [sp, #0x88]
    b402: 2100         	movs	r1, #0x0
    b404: 9a1c         	ldr	r2, [sp, #0x70]
    b406: 4620         	mov	r0, r4
    b408: 931d         	str	r3, [sp, #0x74]
    b40a: 54d1         	strb	r1, [r2, r3]
    b40c: f7f8 eec4    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x7278
    b410: ad24         	add	r5, sp, #0x90
    b412: 4680         	mov	r8, r0
    b414: 9522         	str	r5, [sp, #0x88]
    b416: 2800         	cmp	r0, #0x0
    b418: f000 80e7    	beq.w	0xb5ea <QualityManager::getSettings(LSMessage&)+0x252> @ imm = #0x1ce
    b41c: f7f8 ee42    	blx	0x40a4 <strlen@plt>     @ imm = #-0x737c
    b420: 280f         	cmp	r0, #0xf
    b422: 4604         	mov	r4, r0
    b424: 9013         	str	r0, [sp, #0x4c]
    b426: f200 80ba    	bhi.w	0xb59e <QualityManager::getSettings(LSMessage&)+0x206> @ imm = #0x174
    b42a: 2801         	cmp	r0, #0x1
    b42c: f040 80a8    	bne.w	0xb580 <QualityManager::getSettings(LSMessage&)+0x1e8> @ imm = #0x150
    b430: f898 3000    	ldrb.w	r3, [r8]
    b434: f88d 3090    	strb.w	r3, [sp, #0x90]
    b438: 462b         	mov	r3, r5
    b43a: 2200         	movs	r2, #0x0
    b43c: 9423         	str	r4, [sp, #0x8c]
    b43e: 4639         	mov	r1, r7
    b440: 551a         	strb	r2, [r3, r4]
    b442: 4630         	mov	r0, r6
    b444: ab06         	add	r3, sp, #0x18
    b446: 465a         	mov	r2, r11
    b448: f7fe ffc6    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #-0x1074
    b44c: 4604         	mov	r4, r0
    b44e: 9822         	ldr	r0, [sp, #0x88]
    b450: 42a8         	cmp	r0, r5
    b452: d003         	beq	0xb45c <QualityManager::getSettings(LSMessage&)+0xc4> @ imm = #0x6
    b454: 9924         	ldr	r1, [sp, #0x90]
    b456: 3101         	adds	r1, #0x1
    b458: f7f8 ed52    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x755c
    b45c: 2c00         	cmp	r4, #0x0
    b45e: f000 8095    	beq.w	0xb58c <QualityManager::getSettings(LSMessage&)+0x1f4> @ imm = #0x12a
    b462: 4a82         	ldr	r2, [pc, #0x208]        @ 0xb66c <QualityManager::getSettings(LSMessage&)+0x2d4> literal=0x37f6
    b464: ac13         	add	r4, sp, #0x4c
    b466: 4639         	mov	r1, r7
    b468: 4620         	mov	r0, r4
    b46a: 447a         	add	r2, pc => raw=0xec64 ghidra=0x1ec64 file=60516 "category"
    b46c: f7f8 ed4e    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x7564
    b470: 2300         	movs	r3, #0x0
    b472: 4631         	mov	r1, r6
    b474: 4620         	mov	r0, r4
    b476: 9522         	str	r5, [sp, #0x88]
    b478: 9323         	str	r3, [sp, #0x8c]
    b47a: f88d 3090    	strb.w	r3, [sp, #0x90]
    b47e: f7f8 ec90    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x76e0
    b482: 9b22         	ldr	r3, [sp, #0x88]
    b484: 9816         	ldr	r0, [sp, #0x58]
    b486: 9a23         	ldr	r2, [sp, #0x8c]
    b488: 42ab         	cmp	r3, r5
    b48a: f000 8097    	beq.w	0xb5bc <QualityManager::getSettings(LSMessage&)+0x224> @ imm = #0x12e
    b48e: 9924         	ldr	r1, [sp, #0x90]
    b490: 4550         	cmp	r0, r10
    b492: e9cd 3216    	strd	r3, r2, [sp, #88]
    b496: bf08         	it	eq
    b498: 9118         	streq	r1, [sp, #0x60]
    b49a: f000 809c    	beq.w	0xb5d6 <QualityManager::getSettings(LSMessage&)+0x23e> @ imm = #0x138
    b49e: 9b18         	ldr	r3, [sp, #0x60]
    b4a0: 9118         	str	r1, [sp, #0x60]
    b4a2: 2800         	cmp	r0, #0x0
    b4a4: f000 8097    	beq.w	0xb5d6 <QualityManager::getSettings(LSMessage&)+0x23e> @ imm = #0x12e
    b4a8: 9324         	str	r3, [sp, #0x90]
    b4aa: 9022         	str	r0, [sp, #0x88]
    b4ac: 2300         	movs	r3, #0x0
    b4ae: 9323         	str	r3, [sp, #0x8c]
    b4b0: 7003         	strb	r3, [r0]
    b4b2: 9822         	ldr	r0, [sp, #0x88]
    b4b4: 42a8         	cmp	r0, r5
    b4b6: d003         	beq	0xb4c0 <QualityManager::getSettings(LSMessage&)+0x128> @ imm = #0x6
    b4b8: 9924         	ldr	r1, [sp, #0x90]
    b4ba: 3101         	adds	r1, #0x1
    b4bc: f7f8 ed20    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x75c0
    b4c0: 9a02         	ldr	r2, [sp, #0x8]
    b4c2: 4620         	mov	r0, r4
    b4c4: 4b6a         	ldr	r3, [pc, #0x1a8]        @ 0xb670 <QualityManager::getSettings(LSMessage&)+0x2d8> literal=0x210
    b4c6: f10d 0834    	add.w	r8, sp, #0x34
    b4ca: 58d5         	ldr	r5, [r2, r3]
    b4cc: f105 0308    	add.w	r3, r5, #0x8
    b4d0: 9313         	str	r3, [sp, #0x4c]
    b4d2: f7f8 ed3c    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x7588
    b4d6: 4a67         	ldr	r2, [pc, #0x19c]        @ 0xb674 <QualityManager::getSettings(LSMessage&)+0x2dc> literal=0x35c4
    b4d8: 4639         	mov	r1, r7
    b4da: 4640         	mov	r0, r8
    b4dc: 447a         	add	r2, pc => raw=0xeaa4 ghidra=0x1eaa4 file=60068 "settings"
    b4de: f7f8 ed16    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x75d4
    b4e2: 9a0b         	ldr	r2, [sp, #0x2c]
    b4e4: 3508         	adds	r5, #0x8
    b4e6: 9b0e         	ldr	r3, [sp, #0x38]
    b4e8: 4640         	mov	r0, r8
    b4ea: 950d         	str	r5, [sp, #0x34]
    b4ec: ad10         	add	r5, sp, #0x40
    b4ee: 920e         	str	r2, [sp, #0x38]
    b4f0: 930b         	str	r3, [sp, #0x2c]
    b4f2: 9a0c         	ldr	r2, [sp, #0x30]
    b4f4: 9b0f         	ldr	r3, [sp, #0x3c]
    b4f6: 920f         	str	r2, [sp, #0x3c]
    b4f8: 930c         	str	r3, [sp, #0x30]
    b4fa: f7f8 ed28    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x75b0
    b4fe: 4628         	mov	r0, r5
    b500: f7f8 ec5a    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x774c
    b504: 4649         	mov	r1, r9
    b506: 4630         	mov	r0, r6
    b508: f10d 0858    	add.w	r8, sp, #0x58
    b50c: f7f8 ecde    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x7644
    b510: 9903         	ldr	r1, [sp, #0xc]
    b512: 4633         	mov	r3, r6
    b514: 4642         	mov	r2, r8
    b516: 4620         	mov	r0, r4
    b518: f7ff fa32    	bl	0xa980 <QualityManager::getSettings(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, pbnjson::JValue)> @ imm = #-0xb9c
    b51c: 9a11         	ldr	r2, [sp, #0x44]
    b51e: 4620         	mov	r0, r4
    b520: 9b14         	ldr	r3, [sp, #0x50]
    b522: 9214         	str	r2, [sp, #0x50]
    b524: 9311         	str	r3, [sp, #0x44]
    b526: 9a12         	ldr	r2, [sp, #0x48]
    b528: 9b15         	ldr	r3, [sp, #0x54]
    b52a: 9215         	str	r2, [sp, #0x54]
    b52c: 9312         	str	r3, [sp, #0x48]
    b52e: f7f8 ec14    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x77d8
    b532: 4630         	mov	r0, r6
    b534: f7f8 ec10    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x77e0
    b538: 4629         	mov	r1, r5
    b53a: a805         	add	r0, sp, #0x14
    b53c: f7fe fca8    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #-0x16b0
    b540: 4628         	mov	r0, r5
    b542: f7f8 ec0a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x77ec
    b546: 981c         	ldr	r0, [sp, #0x70]
    b548: 9b01         	ldr	r3, [sp, #0x4]
    b54a: 4298         	cmp	r0, r3
    b54c: d003         	beq	0xb556 <QualityManager::getSettings(LSMessage&)+0x1be> @ imm = #0x6
    b54e: 991e         	ldr	r1, [sp, #0x78]
    b550: 3101         	adds	r1, #0x1
    b552: f7f8 ecd6    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7654
    b556: 4648         	mov	r0, r9
    b558: f7f8 ebfe    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7804
    b55c: 9816         	ldr	r0, [sp, #0x58]
    b55e: 4550         	cmp	r0, r10
    b560: d003         	beq	0xb56a <QualityManager::getSettings(LSMessage&)+0x1d2> @ imm = #0x6
    b562: 9918         	ldr	r1, [sp, #0x60]
    b564: 3101         	adds	r1, #0x1
    b566: f7f8 eccc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7668
    b56a: 4638         	mov	r0, r7
    b56c: f7f8 ebf4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7818
    b570: 9805         	ldr	r0, [sp, #0x14]
    b572: b108         	cbz	r0, 0xb578 <QualityManager::getSettings(LSMessage&)+0x1e0> @ imm = #0x2
    b574: f7f8 ed6a    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x752c
    b578: 2001         	movs	r0, #0x1
    b57a: b029         	add	sp, #0xa4
    b57c: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    b580: 462b         	mov	r3, r5
    b582: 2800         	cmp	r0, #0x0
    b584: f43f af59    	beq.w	0xb43a <QualityManager::getSettings(LSMessage&)+0xa2> @ imm = #-0x14e
    b588: 4628         	mov	r0, r5
    b58a: e010         	b	0xb5ae <QualityManager::getSettings(LSMessage&)+0x216> @ imm = #0x20
    b58c: 493a         	ldr	r1, [pc, #0xe8]         @ 0xb678 <QualityManager::getSettings(LSMessage&)+0x2e0> literal=0x15cec
    b58e: 4623         	mov	r3, r4
    b590: 2203         	movs	r2, #0x3
    b592: a805         	add	r0, sp, #0x14
    b594: 4479         	add	r1, pc => raw=0x21284 ghidra=0x31284 file=null
    b596: 311c         	adds	r1, #0x1c
    b598: f7fe fb58    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x1950
    b59c: e7d3         	b	0xb546 <QualityManager::getSettings(LSMessage&)+0x1ae> @ imm = #-0x5a
    b59e: 2200         	movs	r2, #0x0
    b5a0: a913         	add	r1, sp, #0x4c
    b5a2: 4630         	mov	r0, r6
    b5a4: f7f8 ed66    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x7534
    b5a8: 9b13         	ldr	r3, [sp, #0x4c]
    b5aa: 9022         	str	r0, [sp, #0x88]
    b5ac: 9324         	str	r3, [sp, #0x90]
    b5ae: 4622         	mov	r2, r4
    b5b0: 4641         	mov	r1, r8
    b5b2: f7f8 ecea    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x762c
    b5b6: 9c13         	ldr	r4, [sp, #0x4c]
    b5b8: 9b22         	ldr	r3, [sp, #0x88]
    b5ba: e73e         	b	0xb43a <QualityManager::getSettings(LSMessage&)+0xa2> @ imm = #-0x184
    b5bc: b132         	cbz	r2, 0xb5cc <QualityManager::getSettings(LSMessage&)+0x234> @ imm = #0xc
    b5be: 2a01         	cmp	r2, #0x1
    b5c0: d00d         	beq	0xb5de <QualityManager::getSettings(LSMessage&)+0x246> @ imm = #0x1a
    b5c2: 4629         	mov	r1, r5
    b5c4: f7f8 ece0    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x7640
    b5c8: 9a23         	ldr	r2, [sp, #0x8c]
    b5ca: 9816         	ldr	r0, [sp, #0x58]
    b5cc: 2300         	movs	r3, #0x0
    b5ce: 9217         	str	r2, [sp, #0x5c]
    b5d0: 5483         	strb	r3, [r0, r2]
    b5d2: 9822         	ldr	r0, [sp, #0x88]
    b5d4: e76a         	b	0xb4ac <QualityManager::getSettings(LSMessage&)+0x114> @ imm = #-0x12c
    b5d6: 9522         	str	r5, [sp, #0x88]
    b5d8: ad24         	add	r5, sp, #0x90
    b5da: 4628         	mov	r0, r5
    b5dc: e766         	b	0xb4ac <QualityManager::getSettings(LSMessage&)+0x114> @ imm = #-0x134
    b5de: f89d 3090    	ldrb.w	r3, [sp, #0x90]
    b5e2: 7003         	strb	r3, [r0]
    b5e4: 9a23         	ldr	r2, [sp, #0x8c]
    b5e6: 9816         	ldr	r0, [sp, #0x58]
    b5e8: e7f0         	b	0xb5cc <QualityManager::getSettings(LSMessage&)+0x234> @ imm = #-0x20
    b5ea: 4824         	ldr	r0, [pc, #0x90]         @ 0xb67c <QualityManager::getSettings(LSMessage&)+0x2e4> literal=0x20a8
    b5ec: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    b5ee: f7f8 eb8a    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x78ec
    b5f2: f10d 0858    	add.w	r8, sp, #0x58
    b5f6: 4658         	mov	r0, r11
    b5f8: f7f8 ed7a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x750c
    b5fc: 4648         	mov	r0, r9
    b5fe: f7f8 ebac    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x78a8
    b602: 4640         	mov	r0, r8
    b604: f7f8 ed74    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7518
    b608: 4638         	mov	r0, r7
    b60a: f7f8 eba6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x78b4
    b60e: 9805         	ldr	r0, [sp, #0x14]
    b610: b108         	cbz	r0, 0xb616 <QualityManager::getSettings(LSMessage&)+0x27e> @ imm = #0x2
    b612: f7f8 ed1c    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x75c8
    b616: f7f8 edb4    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x7498
    b61a: f10d 0858    	add.w	r8, sp, #0x58
    b61e: e7ed         	b	0xb5fc <QualityManager::getSettings(LSMessage&)+0x264> @ imm = #-0x26
    b620: 4630         	mov	r0, r6
    b622: f7f8 eb9a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x78cc
    b626: 4628         	mov	r0, r5
    b628: f7f8 eb96    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x78d4
    b62c: e7e3         	b	0xb5f6 <QualityManager::getSettings(LSMessage&)+0x25e> @ imm = #-0x3a
    b62e: 9805         	ldr	r0, [sp, #0x14]
    b630: e7ef         	b	0xb612 <QualityManager::getSettings(LSMessage&)+0x27a> @ imm = #-0x22
    b632: f10d 0858    	add.w	r8, sp, #0x58
    b636: e7e4         	b	0xb602 <QualityManager::getSettings(LSMessage&)+0x26a> @ imm = #-0x38
    b638: 4630         	mov	r0, r6
    b63a: f10d 0858    	add.w	r8, sp, #0x58
    b63e: f7f8 ed58    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7550
    b642: e7d8         	b	0xb5f6 <QualityManager::getSettings(LSMessage&)+0x25e> @ imm = #-0x50
    b644: e7ef         	b	0xb626 <QualityManager::getSettings(LSMessage&)+0x28e> @ imm = #-0x22
    b646: 4630         	mov	r0, r6
    b648: f10d 0858    	add.w	r8, sp, #0x58
    b64c: f7f8 ed50    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7560
    b650: 9a02         	ldr	r2, [sp, #0x8]
    b652: 4620         	mov	r0, r4
    b654: 4b06         	ldr	r3, [pc, #0x18]         @ 0xb670 <QualityManager::getSettings(LSMessage&)+0x2d8> literal=0x210
    b656: 58d3         	ldr	r3, [r2, r3]
    b658: 3308         	adds	r3, #0x8
    b65a: 9313         	str	r3, [sp, #0x4c]
    b65c: f7f8 ec76    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x7714
    b660: e7c9         	b	0xb5f6 <QualityManager::getSettings(LSMessage&)+0x25e> @ imm = #-0x6e
    b662: bf00         	nop
    b664: 5a0a         	ldrh	r2, [r1, r0]
    b666: 0001         	movs	r1, r0
    b668: 38f0         	subs	r0, #0xf0
    b66a: 0000         	movs	r0, r0
    b66c: 37f6         	adds	r7, #0xf6
    b66e: 0000         	movs	r0, r0
    b670: 0210         	lsls	r0, r2, #0x8
    b672: 0000         	movs	r0, r0
    b674: 35c4         	adds	r5, #0xc4
    b676: 0000         	movs	r0, r0
    b678: 5cec         	ldrb	r4, [r5, r3]
    b67a: 0001         	movs	r1, r0
    b67c: 20a8         	movs	r0, #0xa8
    b67e: 0000         	movs	r0, r0

0000b680 <QualityManager::setQualityAttribute(LSMessage&)>:
    b680: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    b684: b09f         	sub	sp, #0x7c
    b686: 4680         	mov	r8, r0
    b688: af09         	add	r7, sp, #0x24
    b68a: 4608         	mov	r0, r1
    b68c: 460c         	mov	r4, r1
    b68e: 9107         	str	r1, [sp, #0x1c]
    b690: f7f8 ed28    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x75b0
    b694: 4638         	mov	r0, r7
    b696: f7f8 edc2    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x747c
    b69a: 2300         	movs	r3, #0x0
    b69c: 9308         	str	r3, [sp, #0x20]
    b69e: f7fe ff4d    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1166
    b6a2: 2800         	cmp	r0, #0x0
    b6a4: d07b         	beq	0xb79e <QualityManager::setQualityAttribute(LSMessage&)+0x11e> @ imm = #0xf6
    b6a6: f7fe ff49    	bl	0xa53c <getPmLogContext()> @ imm = #-0x116e
    b6aa: 6803         	ldr	r3, [r0]
    b6ac: 2b06         	cmp	r3, #0x6
    b6ae: dc76         	bgt	0xb79e <QualityManager::setQualityAttribute(LSMessage&)+0x11e> @ imm = #0xec
    b6b0: f10d 0948    	add.w	r9, sp, #0x48
    b6b4: ae18         	add	r6, sp, #0x60
    b6b6: f10d 0a50    	add.w	r10, sp, #0x50
    b6ba: f240 132b    	movw	r3, #0x12b
    b6be: 2200         	movs	r2, #0x0
    b6c0: 4631         	mov	r1, r6
    b6c2: 4648         	mov	r0, r9
    b6c4: f8cd a048    	str.w	r10, [sp, #0x48]
    b6c8: 9318         	str	r3, [sp, #0x60]
    b6ca: f7f8 ecd4    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x7658
    b6ce: 49ae         	ldr	r1, [pc, #0x2b8]        @ 0xb988 <QualityManager::setQualityAttribute(LSMessage&)+0x308> literal=0x36d0
    b6d0: f240 122b    	movw	r2, #0x12b
    b6d4: 9b18         	ldr	r3, [sp, #0x60]
    b6d6: 9012         	str	r0, [sp, #0x48]
    b6d8: 4479         	add	r1, pc => raw=0xedac ghidra=0x1edac file=60844 "{\"type\":\"object\",\"properties\":{\"categoryType\":{\"type\":\"string\"},\"requestType\":{\"type\":\"string\"},\"categoryInfo\":{\"type\":\"object\"},\"isGameMode\":{\"type\":\"boolean\"},\"pipelineInfo\":{\"type\":\"object\"},\"activeState\":{\"type\":\"boolean\"}},\"required\":[\"categoryType\",\"requestType\"],\"additionalProperties\":false}"
    b6da: 9314         	str	r3, [sp, #0x50]
    b6dc: f7f8 ec54    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x7758
    b6e0: 9b18         	ldr	r3, [sp, #0x60]
    b6e2: 2100         	movs	r1, #0x0
    b6e4: 9a12         	ldr	r2, [sp, #0x48]
    b6e6: 4620         	mov	r0, r4
    b6e8: 9313         	str	r3, [sp, #0x4c]
    b6ea: 54d1         	strb	r1, [r2, r3]
    b6ec: f7f8 ed54    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x7558
    b6f0: ad1a         	add	r5, sp, #0x68
    b6f2: 4683         	mov	r11, r0
    b6f4: 9518         	str	r5, [sp, #0x60]
    b6f6: 2800         	cmp	r0, #0x0
    b6f8: f000 811d    	beq.w	0xb936 <QualityManager::setQualityAttribute(LSMessage&)+0x2b6> @ imm = #0x23a
    b6fc: f7f8 ecd2    	blx	0x40a4 <strlen@plt>     @ imm = #-0x765c
    b700: 280f         	cmp	r0, #0xf
    b702: 4604         	mov	r4, r0
    b704: 900f         	str	r0, [sp, #0x3c]
    b706: f200 80a4    	bhi.w	0xb852 <QualityManager::setQualityAttribute(LSMessage&)+0x1d2> @ imm = #0x148
    b70a: 2801         	cmp	r0, #0x1
    b70c: d15b         	bne	0xb7c6 <QualityManager::setQualityAttribute(LSMessage&)+0x146> @ imm = #0xb6
    b70e: f89b 3000    	ldrb.w	r3, [r11]
    b712: f88d 3068    	strb.w	r3, [sp, #0x68]
    b716: 462b         	mov	r3, r5
    b718: 2200         	movs	r2, #0x0
    b71a: 9419         	str	r4, [sp, #0x64]
    b71c: 4639         	mov	r1, r7
    b71e: 551a         	strb	r2, [r3, r4]
    b720: 4630         	mov	r0, r6
    b722: ab08         	add	r3, sp, #0x20
    b724: 464a         	mov	r2, r9
    b726: f7fe fe57    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #-0x1352
    b72a: 4604         	mov	r4, r0
    b72c: 9818         	ldr	r0, [sp, #0x60]
    b72e: 42a8         	cmp	r0, r5
    b730: d003         	beq	0xb73a <QualityManager::setQualityAttribute(LSMessage&)+0xba> @ imm = #0x6
    b732: 991a         	ldr	r1, [sp, #0x68]
    b734: 3101         	adds	r1, #0x1
    b736: f7f8 ebe4    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7838
    b73a: 2c00         	cmp	r4, #0x0
    b73c: d148         	bne	0xb7d0 <QualityManager::setQualityAttribute(LSMessage&)+0x150> @ imm = #0x90
    b73e: 4993         	ldr	r1, [pc, #0x24c]        @ 0xb98c <QualityManager::setQualityAttribute(LSMessage&)+0x30c> literal=0x15b3a
    b740: 4623         	mov	r3, r4
    b742: 2203         	movs	r2, #0x3
    b744: a807         	add	r0, sp, #0x1c
    b746: 4479         	add	r1, pc => raw=0x21284 ghidra=0x31284 file=null
    b748: 311c         	adds	r1, #0x1c
    b74a: f7fe fa7f    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x1b02
    b74e: f7fe fef5    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1216
    b752: b120         	cbz	r0, 0xb75e <QualityManager::setQualityAttribute(LSMessage&)+0xde> @ imm = #0x8
    b754: f7fe fef2    	bl	0xa53c <getPmLogContext()> @ imm = #-0x121c
    b758: 6803         	ldr	r3, [r0]
    b75a: 2b05         	cmp	r3, #0x5
    b75c: dd0d         	ble	0xb77a <QualityManager::setQualityAttribute(LSMessage&)+0xfa> @ imm = #0x1a
    b75e: f7fe feed    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1226
    b762: 4b8b         	ldr	r3, [pc, #0x22c]        @ 0xb990 <QualityManager::setQualityAttribute(LSMessage&)+0x310> literal=0x34de
    b764: 2200         	movs	r2, #0x0
    b766: 2106         	movs	r1, #0x6
    b768: e9cd 2201    	strd	r2, r2, [sp, #4]
    b76c: 9200         	str	r2, [sp]
    b76e: 447b         	add	r3, pc => raw=0xec50 ghidra=0x1ec50 file=60496 "error for schema"
    b770: 9303         	str	r3, [sp, #0xc]
    b772: 4b88         	ldr	r3, [pc, #0x220]        @ 0xb994 <QualityManager::setQualityAttribute(LSMessage&)+0x314> literal=0x34c4
    b774: 447b         	add	r3, pc => raw=0xec3c ghidra=0x1ec3c file=60476 "JSON_PARSE_ERROR"
    b776: f7f8 ec52    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x775c
    b77a: 9812         	ldr	r0, [sp, #0x48]
    b77c: 4550         	cmp	r0, r10
    b77e: d003         	beq	0xb788 <QualityManager::setQualityAttribute(LSMessage&)+0x108> @ imm = #0x6
    b780: 9914         	ldr	r1, [sp, #0x50]
    b782: 3101         	adds	r1, #0x1
    b784: f7f8 ebbc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7888
    b788: 4638         	mov	r0, r7
    b78a: f7f8 eae6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7a34
    b78e: 9807         	ldr	r0, [sp, #0x1c]
    b790: b108         	cbz	r0, 0xb796 <QualityManager::setQualityAttribute(LSMessage&)+0x116> @ imm = #0x2
    b792: f7f8 ec5c    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x7748
    b796: 2001         	movs	r0, #0x1
    b798: b01f         	add	sp, #0x7c
    b79a: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    b79e: f7fe fecd    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1266
    b7a2: 4b7d         	ldr	r3, [pc, #0x1f4]        @ 0xb998 <QualityManager::setQualityAttribute(LSMessage&)+0x318> literal=0x35ba
    b7a4: 2107         	movs	r1, #0x7
    b7a6: 447b         	add	r3, pc => raw=0xed64 ghidra=0x1ed64 file=60772 "setQualityAttribute"
    b7a8: 9305         	str	r3, [sp, #0x14]
    b7aa: 4b7c         	ldr	r3, [pc, #0x1f0]        @ 0xb99c <QualityManager::setQualityAttribute(LSMessage&)+0x31c> literal=0x337c
    b7ac: 447b         	add	r3, pc => raw=0xeb2c ghidra=0x1eb2c file=60204 "/usr/src/debug/lib32-audiooutputd/1.0.0-1.jcl4tv.23-r1/git/src/qualitymanager.cpp"
    b7ae: 9304         	str	r3, [sp, #0x10]
    b7b0: 4b7b         	ldr	r3, [pc, #0x1ec]        @ 0xb9a0 <QualityManager::setQualityAttribute(LSMessage&)+0x320> literal=0x35c2
    b7b2: 447b         	add	r3, pc => raw=0xed78 ghidra=0x1ed78 file=60792 "%s:%s() QualityManager::setQualityAttribute call"
    b7b4: 9303         	str	r3, [sp, #0xc]
    b7b6: 2300         	movs	r3, #0x0
    b7b8: 461a         	mov	r2, r3
    b7ba: e9cd 3301    	strd	r3, r3, [sp, #4]
    b7be: 9300         	str	r3, [sp]
    b7c0: f7f8 ec2c    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x77a8
    b7c4: e774         	b	0xb6b0 <QualityManager::setQualityAttribute(LSMessage&)+0x30> @ imm = #-0x118
    b7c6: 462b         	mov	r3, r5
    b7c8: 2800         	cmp	r0, #0x0
    b7ca: d0a5         	beq	0xb718 <QualityManager::setQualityAttribute(LSMessage&)+0x98> @ imm = #-0xb6
    b7cc: 4628         	mov	r0, r5
    b7ce: e048         	b	0xb862 <QualityManager::setQualityAttribute(LSMessage&)+0x1e2> @ imm = #0x90
    b7d0: f10d 0b30    	add.w	r11, sp, #0x30
    b7d4: 4658         	mov	r0, r11
    b7d6: f7f8 eaf0    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x7a20
    b7da: 4639         	mov	r1, r7
    b7dc: 4630         	mov	r0, r6
    b7de: f7f8 eb76    	blx	0x3ecc <_ZN7pbnjson6JValueC1ERKS0_@plt> @ imm = #-0x7914
    b7e2: 4631         	mov	r1, r6
    b7e4: 4640         	mov	r0, r8
    b7e6: f7ff fa93    	bl	0xad10 <QualityManager::setQualityAttribute(pbnjson::JValue)> @ imm = #-0xada
    b7ea: 4604         	mov	r4, r0
    b7ec: 4630         	mov	r0, r6
    b7ee: f7f8 eab4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7a98
    b7f2: 2c00         	cmp	r4, #0x0
    b7f4: d13c         	bne	0xb870 <QualityManager::setQualityAttribute(LSMessage&)+0x1f0> @ imm = #0x78
    b7f6: ac0f         	add	r4, sp, #0x3c
    b7f8: 2101         	movs	r1, #0x1
    b7fa: 4620         	mov	r0, r4
    b7fc: f7f8 eafa    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x7a0c
    b800: 4a68         	ldr	r2, [pc, #0x1a0]        @ 0xb9a4 <QualityManager::setQualityAttribute(LSMessage&)+0x324> literal=0x1f12
    b802: 462b         	mov	r3, r5
    b804: 9518         	str	r5, [sp, #0x60]
    b806: f04f 0e0b    	mov.w	lr, #0xb
    b80a: f04f 0c00    	mov.w	r12, #0x0
    b80e: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    b810: ca07         	ldm	r2, {r0, r1, r2}
    b812: c303         	stm	r3!, {r0, r1}
    b814: 4631         	mov	r1, r6
    b816: f823 2b02    	strh	r2, [r3], #2
    b81a: 0c12         	lsrs	r2, r2, #0x10
    b81c: 4658         	mov	r0, r11
    b81e: 701a         	strb	r2, [r3]
    b820: 4622         	mov	r2, r4
    b822: f8cd e064    	str.w	lr, [sp, #0x64]
    b826: f88d c073    	strb.w	r12, [sp, #0x73]
    b82a: f7f8 ea54    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x7b58
    b82e: 9818         	ldr	r0, [sp, #0x60]
    b830: 42a8         	cmp	r0, r5
    b832: d003         	beq	0xb83c <QualityManager::setQualityAttribute(LSMessage&)+0x1bc> @ imm = #0x6
    b834: 991a         	ldr	r1, [sp, #0x68]
    b836: 3101         	adds	r1, #0x1
    b838: f7f8 eb62    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x793c
    b83c: 4620         	mov	r0, r4
    b83e: f7f8 ea8c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7ae8
    b842: 4659         	mov	r1, r11
    b844: a807         	add	r0, sp, #0x1c
    b846: f7fe fb23    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #-0x19ba
    b84a: 4658         	mov	r0, r11
    b84c: f7f8 ea84    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7af8
    b850: e793         	b	0xb77a <QualityManager::setQualityAttribute(LSMessage&)+0xfa> @ imm = #-0xda
    b852: 2200         	movs	r2, #0x0
    b854: a90f         	add	r1, sp, #0x3c
    b856: 4630         	mov	r0, r6
    b858: f7f8 ec0c    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x77e8
    b85c: 9b0f         	ldr	r3, [sp, #0x3c]
    b85e: 9018         	str	r0, [sp, #0x60]
    b860: 931a         	str	r3, [sp, #0x68]
    b862: 4622         	mov	r2, r4
    b864: 4659         	mov	r1, r11
    b866: f7f8 eb90    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x78e0
    b86a: 9c0f         	ldr	r4, [sp, #0x3c]
    b86c: 9b18         	ldr	r3, [sp, #0x60]
    b86e: e753         	b	0xb718 <QualityManager::setQualityAttribute(LSMessage&)+0x98> @ imm = #-0x15a
    b870: ac0f         	add	r4, sp, #0x3c
    b872: 2100         	movs	r1, #0x0
    b874: 4620         	mov	r0, r4
    b876: f7f8 eabe    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x7a84
    b87a: 4a4b         	ldr	r2, [pc, #0x12c]        @ 0xb9a8 <QualityManager::setQualityAttribute(LSMessage&)+0x328> literal=0x1e98
    b87c: 462b         	mov	r3, r5
    b87e: 9518         	str	r5, [sp, #0x60]
    b880: f04f 0e0b    	mov.w	lr, #0xb
    b884: f04f 0c00    	mov.w	r12, #0x0
    b888: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    b88a: ca07         	ldm	r2, {r0, r1, r2}
    b88c: c303         	stm	r3!, {r0, r1}
    b88e: 4631         	mov	r1, r6
    b890: f823 2b02    	strh	r2, [r3], #2
    b894: 0c12         	lsrs	r2, r2, #0x10
    b896: 4658         	mov	r0, r11
    b898: 701a         	strb	r2, [r3]
    b89a: 4622         	mov	r2, r4
    b89c: f8cd e064    	str.w	lr, [sp, #0x64]
    b8a0: f88d c073    	strb.w	r12, [sp, #0x73]
    b8a4: f7f8 ea16    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x7bd4
    b8a8: 9818         	ldr	r0, [sp, #0x60]
    b8aa: 42a8         	cmp	r0, r5
    b8ac: d003         	beq	0xb8b6 <QualityManager::setQualityAttribute(LSMessage&)+0x236> @ imm = #0x6
    b8ae: 991a         	ldr	r1, [sp, #0x68]
    b8b0: 3101         	adds	r1, #0x1
    b8b2: f7f8 eb26    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x79b4
    b8b6: 4620         	mov	r0, r4
    b8b8: f7f8 ea4e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7b64
    b8bc: 493b         	ldr	r1, [pc, #0xec]         @ 0xb9ac <QualityManager::setQualityAttribute(LSMessage&)+0x32c> literal=0x159c0
    b8be: 4620         	mov	r0, r4
    b8c0: 4479         	add	r1, pc => raw=0x21284 ghidra=0x31284 file=null
    b8c2: 3164         	adds	r1, #0x64
    b8c4: f7f8 ec32    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x779c
    b8c8: 4a39         	ldr	r2, [pc, #0xe4]         @ 0xb9b0 <QualityManager::setQualityAttribute(LSMessage&)+0x330> literal=0x2d82
    b8ca: 462b         	mov	r3, r5
    b8cc: 9518         	str	r5, [sp, #0x60]
    b8ce: f04f 0e09    	mov.w	lr, #0x9
    b8d2: f04f 0c00    	mov.w	r12, #0x0
    b8d6: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    b8d8: ca07         	ldm	r2, {r0, r1, r2}
    b8da: c303         	stm	r3!, {r0, r1}
    b8dc: 4631         	mov	r1, r6
    b8de: 701a         	strb	r2, [r3]
    b8e0: 4658         	mov	r0, r11
    b8e2: 4622         	mov	r2, r4
    b8e4: f8cd e064    	str.w	lr, [sp, #0x64]
    b8e8: f88d c071    	strb.w	r12, [sp, #0x71]
    b8ec: f7f8 e9f2    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x7c1c
    b8f0: 9818         	ldr	r0, [sp, #0x60]
    b8f2: 42a8         	cmp	r0, r5
    b8f4: d003         	beq	0xb8fe <QualityManager::setQualityAttribute(LSMessage&)+0x27e> @ imm = #0x6
    b8f6: 991a         	ldr	r1, [sp, #0x68]
    b8f8: 3101         	adds	r1, #0x1
    b8fa: f7f8 eb02    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x79fc
    b8fe: 4620         	mov	r0, r4
    b900: f7f8 ea2a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7bac
    b904: 2114         	movs	r1, #0x14
    b906: 4620         	mov	r0, r4
    b908: f7f8 ec16    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x77d4
    b90c: 4a29         	ldr	r2, [pc, #0xa4]         @ 0xb9b4 <QualityManager::setQualityAttribute(LSMessage&)+0x334> literal=0x2d4a
    b90e: 462b         	mov	r3, r5
    b910: 9518         	str	r5, [sp, #0x60]
    b912: f04f 0e09    	mov.w	lr, #0x9
    b916: f04f 0c00    	mov.w	r12, #0x0
    b91a: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    b91c: ca07         	ldm	r2, {r0, r1, r2}
    b91e: c303         	stm	r3!, {r0, r1}
    b920: 4631         	mov	r1, r6
    b922: 701a         	strb	r2, [r3]
    b924: 4658         	mov	r0, r11
    b926: 4622         	mov	r2, r4
    b928: f8cd e064    	str.w	lr, [sp, #0x64]
    b92c: f88d c071    	strb.w	r12, [sp, #0x71]
    b930: f7f8 e9d0    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x7c60
    b934: e77b         	b	0xb82e <QualityManager::setQualityAttribute(LSMessage&)+0x1ae> @ imm = #-0x10a
    b936: 4820         	ldr	r0, [pc, #0x80]         @ 0xb9b8 <QualityManager::setQualityAttribute(LSMessage&)+0x338> literal=0x1d5c
    b938: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    b93a: f7f8 e9e4    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x7c38
    b93e: 4630         	mov	r0, r6
    b940: f7f8 ebd6    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7854
    b944: 4620         	mov	r0, r4
    b946: f7f8 ea08    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7bf0
    b94a: 4658         	mov	r0, r11
    b94c: f7f8 ea04    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7bf8
    b950: 4648         	mov	r0, r9
    b952: f7f8 ebce    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7864
    b956: 4638         	mov	r0, r7
    b958: f7f8 e9fe    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7c04
    b95c: 9807         	ldr	r0, [sp, #0x1c]
    b95e: b108         	cbz	r0, 0xb964 <QualityManager::setQualityAttribute(LSMessage&)+0x2e4> @ imm = #0x2
    b960: f7f8 eb74    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x7918
    b964: f7f8 ec0c    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x77e8
    b968: e7e9         	b	0xb93e <QualityManager::setQualityAttribute(LSMessage&)+0x2be> @ imm = #-0x2e
    b96a: 4630         	mov	r0, r6
    b96c: f7f8 e9f4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x7c18
    b970: e7eb         	b	0xb94a <QualityManager::setQualityAttribute(LSMessage&)+0x2ca> @ imm = #-0x2a
    b972: e7ed         	b	0xb950 <QualityManager::setQualityAttribute(LSMessage&)+0x2d0> @ imm = #-0x26
    b974: e7e3         	b	0xb93e <QualityManager::setQualityAttribute(LSMessage&)+0x2be> @ imm = #-0x3a
    b976: e7ee         	b	0xb956 <QualityManager::setQualityAttribute(LSMessage&)+0x2d6> @ imm = #-0x24
    b978: 4630         	mov	r0, r6
    b97a: f7f8 ebba    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x788c
    b97e: e7e7         	b	0xb950 <QualityManager::setQualityAttribute(LSMessage&)+0x2d0> @ imm = #-0x32
    b980: e7e3         	b	0xb94a <QualityManager::setQualityAttribute(LSMessage&)+0x2ca> @ imm = #-0x3a
    b982: e7dc         	b	0xb93e <QualityManager::setQualityAttribute(LSMessage&)+0x2be> @ imm = #-0x48
    b984: 9807         	ldr	r0, [sp, #0x1c]
    b986: e7eb         	b	0xb960 <QualityManager::setQualityAttribute(LSMessage&)+0x2e0> @ imm = #-0x2a
    b988: 36d0         	adds	r6, #0xd0
    b98a: 0000         	movs	r0, r0
    b98c: 5b3a         	ldrh	r2, [r7, r4]
    b98e: 0001         	movs	r1, r0
    b990: 34de         	adds	r4, #0xde
    b992: 0000         	movs	r0, r0
    b994: 34c4         	adds	r4, #0xc4
    b996: 0000         	movs	r0, r0
    b998: 35ba         	adds	r5, #0xba
    b99a: 0000         	movs	r0, r0
    b99c: 337c         	adds	r3, #0x7c
    b99e: 0000         	movs	r0, r0
    b9a0: 35c2         	adds	r5, #0xc2
    b9a2: 0000         	movs	r0, r0
    b9a4: 1f12         	subs	r2, r2, #0x4
    b9a6: 0000         	movs	r0, r0
    b9a8: 1e98         	subs	r0, r3, #0x2
    b9aa: 0000         	movs	r0, r0
    b9ac: 59c0         	ldr	r0, [r0, r7]
    b9ae: 0001         	movs	r1, r0
    b9b0: 2d82         	cmp	r5, #0x82
    b9b2: 0000         	movs	r0, r0
    b9b4: 2d4a         	cmp	r5, #0x4a
    b9b6: 0000         	movs	r0, r0
    b9b8: 1d5c         	adds	r4, r3, #0x5
    b9ba: 0000         	movs	r0, r0
    b9bc: 4610         	mov	r0, r2
    b9be: f7ff ba1f    	b.w	0xae00 <QualityManager::setSettings(LSMessage&)> @ imm = #-0xbc2
    b9c2: bf00         	nop
    b9c4: 4610         	mov	r0, r2
    b9c6: f7ff bce7    	b.w	0xb398 <QualityManager::getSettings(LSMessage&)> @ imm = #-0x632
    b9ca: bf00         	nop
    b9cc: 4610         	mov	r0, r2
    b9ce: f7ff be57    	b.w	0xb680 <QualityManager::setQualityAttribute(LSMessage&)> @ imm = #-0x352
    b9d2: bf00         	nop
    b9d4: b570         	push	{r4, r5, r6, lr}
    b9d6: 4614         	mov	r4, r2
    b9d8: 3a00         	subs	r2, #0x0
    b9da: b082         	sub	sp, #0x8
    b9dc: bf18         	it	ne
    b9de: 2201         	movne	r2, #0x1
    b9e0: 2900         	cmp	r1, #0x0
    b9e2: bf18         	it	ne
    b9e4: 2200         	movne	r2, #0x0
    b9e6: bb42         	cbnz	r2, 0xba3a <QualityManager::setQualityAttribute(LSMessage&)+0x3ba> @ imm = #0x50
    b9e8: 1a64         	subs	r4, r4, r1
    b9ea: 4605         	mov	r5, r0
    b9ec: 2c0f         	cmp	r4, #0xf
    b9ee: 460e         	mov	r6, r1
    b9f0: 9401         	str	r4, [sp, #0x4]
    b9f2: d811         	bhi	0xba18 <QualityManager::setQualityAttribute(LSMessage&)+0x398> @ imm = #0x22
    b9f4: 2c01         	cmp	r4, #0x1
    b9f6: 6800         	ldr	r0, [r0]
    b9f8: d108         	bne	0xba0c <QualityManager::setQualityAttribute(LSMessage&)+0x38c> @ imm = #0x10
    b9fa: 780b         	ldrb	r3, [r1]
    b9fc: 7003         	strb	r3, [r0]
    b9fe: 2300         	movs	r3, #0x0
    ba00: 9c01         	ldr	r4, [sp, #0x4]
    ba02: 6828         	ldr	r0, [r5]
    ba04: 606c         	str	r4, [r5, #0x4]
    ba06: 5503         	strb	r3, [r0, r4]
    ba08: b002         	add	sp, #0x8
    ba0a: bd70         	pop	{r4, r5, r6, pc}
    ba0c: b954         	cbnz	r4, 0xba24 <QualityManager::setQualityAttribute(LSMessage&)+0x3a4> @ imm = #0x14
    ba0e: 2300         	movs	r3, #0x0
    ba10: 606c         	str	r4, [r5, #0x4]
    ba12: 5503         	strb	r3, [r0, r4]
    ba14: b002         	add	sp, #0x8
    ba16: bd70         	pop	{r4, r5, r6, pc}
    ba18: a901         	add	r1, sp, #0x4
    ba1a: f7f8 eb2c    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x79a8
    ba1e: 9b01         	ldr	r3, [sp, #0x4]
    ba20: 6028         	str	r0, [r5]
    ba22: 60ab         	str	r3, [r5, #0x8]
    ba24: 4622         	mov	r2, r4
    ba26: 4631         	mov	r1, r6
    ba28: f7f8 eaae    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x7aa4
    ba2c: 9c01         	ldr	r4, [sp, #0x4]
    ba2e: 2300         	movs	r3, #0x0
    ba30: 6828         	ldr	r0, [r5]
    ba32: 606c         	str	r4, [r5, #0x4]
    ba34: 5503         	strb	r3, [r0, r4]
    ba36: b002         	add	sp, #0x8
    ba38: bd70         	pop	{r4, r5, r6, pc}
    ba3a: 4802         	ldr	r0, [pc, #0x8]          @ 0xba44 <QualityManager::setQualityAttribute(LSMessage&)+0x3c4> literal=0x1c58
    ba3c: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    ba3e: f7f8 e962    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x7d3c
    ba42: bf00         	nop
    ba44: 1c58         	adds	r0, r3, #0x1
    ba46: 0000         	movs	r0, r0

0000ba48 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)>:
    ba48: f100 0310    	add.w	r3, r0, #0x10
    ba4c: e92d 47f0    	push.w	{r4, r5, r6, r7, r8, r9, r10, lr}
    ba50: 6083         	str	r3, [r0, #0x8]
    ba52: 2300         	movs	r3, #0x0
    ba54: b08e         	sub	sp, #0x38
    ba56: 4604         	mov	r4, r0
    ba58: f8df 8160    	ldr.w	r8, [pc, #0x160]        @ 0xbbbc <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x174> literal=0x15352
    ba5c: ad07         	add	r5, sp, #0x1c
    ba5e: 60c3         	str	r3, [r0, #0xc]
    ba60: f100 0908    	add.w	r9, r0, #0x8
    ba64: 7403         	strb	r3, [r0, #0x10]
    ba66: 460f         	mov	r7, r1
    ba68: 4b55         	ldr	r3, [pc, #0x154]        @ 0xbbc0 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x178> literal=0x22c
    ba6a: 44f8         	add	r8, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    ba6c: e9c0 1200    	strd	r1, r2, [r0]
    ba70: f44f 42c8    	mov.w	r2, #0x6400
    ba74: 8402         	strh	r2, [r0, #0x20]
    ba76: 4628         	mov	r0, r5
    ba78: f858 6003    	ldr.w	r6, [r8, r3]
    ba7c: f106 0308    	add.w	r3, r6, #0x8
    ba80: 9306         	str	r3, [sp, #0x18]
    ba82: f7f8 ea52    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x7b5c
    ba86: 4a4f         	ldr	r2, [pc, #0x13c]        @ 0xbbc4 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x17c> literal=0x15180
    ba88: 2300         	movs	r3, #0x0
    ba8a: 494f         	ldr	r1, [pc, #0x13c]        @ 0xbbc8 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x180> literal=0x344c
    ba8c: 6838         	ldr	r0, [r7]
    ba8e: af06         	add	r7, sp, #0x18
    ba90: 447a         	add	r2, pc => raw=0x20c14 ghidra=0x30c14 file=68628
    ba92: 9501         	str	r5, [sp, #0x4]
    ba94: 4479         	add	r1, pc => raw=0xeee4 ghidra=0x1eee4 file=61156 "/audio/volume"
    ba96: 9300         	str	r3, [sp]
    ba98: f7f8 eb42    	blx	0x4120 <LSRegisterCategory@plt> @ imm = #-0x797c
    ba9c: 2800         	cmp	r0, #0x0
    ba9e: d06f         	beq	0xbb80 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x138> @ imm = #0xde
    baa0: f106 0a08    	add.w	r10, r6, #0x8
    baa4: 4628         	mov	r0, r5
    baa6: af06         	add	r7, sp, #0x18
    baa8: f8cd a018    	str.w	r10, [sp, #0x18]
    baac: f7f8 eb6e    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x7924
    bab0: 4638         	mov	r0, r7
    bab2: f7f8 e93a    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x7d8c
    bab6: 4628         	mov	r0, r5
    bab8: f8cd a018    	str.w	r10, [sp, #0x18]
    babc: f8d4 a000    	ldr.w	r10, [r4]
    bac0: f7f8 ea32    	blx	0x3f28 <LSErrorInit@plt> @ imm = #-0x7b9c
    bac4: 4941         	ldr	r1, [pc, #0x104]        @ 0xbbcc <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x184> literal=0x3412
    bac6: 462b         	mov	r3, r5
    bac8: f8da 0000    	ldr.w	r0, [r10]
    bacc: 4622         	mov	r2, r4
    bace: 4479         	add	r1, pc => raw=0xeee4 ghidra=0x1eee4 file=61156 "/audio/volume"
    bad0: f7f8 ea6c    	blx	0x3fac <LSCategorySetData@plt> @ imm = #-0x7b28
    bad4: 2800         	cmp	r0, #0x0
    bad6: d035         	beq	0xbb44 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0xfc> @ imm = #0x6a
    bad8: 4628         	mov	r0, r5
    bada: 3608         	adds	r6, #0x8
    badc: 9606         	str	r6, [sp, #0x18]
    bade: f7f8 eb56    	blx	0x418c <LSErrorFree@plt> @ imm = #-0x7954
    bae2: 4638         	mov	r0, r7
    bae4: f7f8 e920    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x7dc0
    bae8: 4620         	mov	r0, r4
    baea: b00e         	add	sp, #0x38
    baec: e8bd 87f0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, pc}
    baf0: 460e         	mov	r6, r1
    baf2: 4605         	mov	r5, r0
    baf4: a806         	add	r0, sp, #0x18
    baf6: f7f8 e918    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x7dd0
    bafa: 4633         	mov	r3, r6
    bafc: 2b01         	cmp	r3, #0x1
    bafe: d152         	bne	0xbba6 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x15e> @ imm = #0xa4
    bb00: 4628         	mov	r0, r5
    bb02: f7f8 e906    	blx	0x3d10 <__cxa_begin_catch@plt> @ imm = #-0x7df4
    bb06: 4605         	mov	r5, r0
    bb08: f7fe fd18    	bl	0xa53c <getPmLogContext()> @ imm = #-0x15d0
    bb0c: 2800         	cmp	r0, #0x0
    bb0e: d14f         	bne	0xbbb0 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x168> @ imm = #0x9e
    bb10: f7fe fd14    	bl	0xa53c <getPmLogContext()> @ imm = #-0x15d8
    bb14: 682a         	ldr	r2, [r5]
    bb16: 4603         	mov	r3, r0
    bb18: 4628         	mov	r0, r5
    bb1a: 461d         	mov	r5, r3
    bb1c: 6893         	ldr	r3, [r2, #0x8]
    bb1e: 4798         	blx	r3
    bb20: 4b2b         	ldr	r3, [pc, #0xac]         @ 0xbbd0 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x188> literal=0x33c8
    bb22: 4602         	mov	r2, r0
    bb24: 2103         	movs	r1, #0x3
    bb26: 4628         	mov	r0, r5
    bb28: 447b         	add	r3, pc => raw=0xeef4 ghidra=0x1eef4 file=61172 "%s - VolumeManager API's registration Failed."
    bb2a: e9cd 3203    	strd	r3, r2, [sp, #12]
    bb2e: 2200         	movs	r2, #0x0
    bb30: 4b28         	ldr	r3, [pc, #0xa0]         @ 0xbbd4 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x18c> literal=0x210c
    bb32: e9cd 2201    	strd	r2, r2, [sp, #4]
    bb36: 9200         	str	r2, [sp]
    bb38: 447b         	add	r3, pc => raw=0xdc48 ghidra=0x1dc48 file=56392 "LS2_SUBSCRIBE_FAILED"
    bb3a: f7f8 ea70    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x7b20
    bb3e: f7f8 eab8    	blx	0x40b0 <__cxa_end_catch@plt> @ imm = #-0x7a90
    bb42: e7d1         	b	0xbae8 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0xa0> @ imm = #-0x5e
    bb44: 2020         	movs	r0, #0x20
    bb46: f7f8 e99e    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0x7cc4
    bb4a: 4639         	mov	r1, r7
    bb4c: 4605         	mov	r5, r0
    bb4e: f7fd fff5    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #-0x2016
    bb52: 4b21         	ldr	r3, [pc, #0x84]         @ 0xbbd8 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x190> literal=0x228
    bb54: 4628         	mov	r0, r5
    bb56: f858 2003    	ldr.w	r2, [r8, r3]
    bb5a: 4b20         	ldr	r3, [pc, #0x80]         @ 0xbbdc <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x194> literal=0x208
    bb5c: f858 1003    	ldr.w	r1, [r8, r3]
    bb60: f7f8 ea42    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x7b7c
    bb64: 460e         	mov	r6, r1
    bb66: 4605         	mov	r5, r0
    bb68: 4638         	mov	r0, r7
    bb6a: f7fd ff73    	bl	0x9a54 <ConnectionManager::removePipelineInfo(LSMessage&)+0x484> @ imm = #-0x211a
    bb6e: 4633         	mov	r3, r6
    bb70: e7c4         	b	0xbafc <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0xb4> @ imm = #-0x78
    bb72: 460e         	mov	r6, r1
    bb74: 4605         	mov	r5, r0
    bb76: 4638         	mov	r0, r7
    bb78: f7f8 e8d6    	blx	0x3d28 <_ZNSt9exceptionD2Ev@plt> @ imm = #-0x7e54
    bb7c: 4633         	mov	r3, r6
    bb7e: e7bd         	b	0xbafc <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0xb4> @ imm = #-0x86
    bb80: 2020         	movs	r0, #0x20
    bb82: f7f8 e980    	blx	0x3e84 <__cxa_allocate_exception@plt> @ imm = #-0x7d00
    bb86: 4639         	mov	r1, r7
    bb88: 4605         	mov	r5, r0
    bb8a: f7fd ffd7    	bl	0x9b3c <ConnectionManager::removePipelineInfo(LSMessage&)+0x56c> @ imm = #-0x2052
    bb8e: 4b12         	ldr	r3, [pc, #0x48]         @ 0xbbd8 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x190> literal=0x228
    bb90: 4628         	mov	r0, r5
    bb92: f858 2003    	ldr.w	r2, [r8, r3]
    bb96: 4b11         	ldr	r3, [pc, #0x44]         @ 0xbbdc <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x194> literal=0x208
    bb98: f858 1003    	ldr.w	r1, [r8, r3]
    bb9c: f7f8 ea24    	blx	0x3fe8 <__cxa_throw@plt> @ imm = #-0x7bb8
    bba0: e7e0         	b	0xbb64 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0x11c> @ imm = #-0x40
    bba2: f7f8 ea86    	blx	0x40b0 <__cxa_end_catch@plt> @ imm = #-0x7af4
    bba6: 4648         	mov	r0, r9
    bba8: f7f8 eaa2    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7abc
    bbac: f7f8 eae8    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x7a30
    bbb0: f7fe fcc4    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1678
    bbb4: 6803         	ldr	r3, [r0]
    bbb6: 2b02         	cmp	r3, #0x2
    bbb8: ddc1         	ble	0xbb3e <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0xf6> @ imm = #-0x7e
    bbba: e7a9         	b	0xbb10 <VolumeManager::VolumeManager(LS::Handle&, umiClient*)+0xc8> @ imm = #-0xae
    bbbc: 5352         	strh	r2, [r2, r5]
    bbbe: 0001         	movs	r1, r0
    bbc0: 022c         	lsls	r4, r5, #0x8
    bbc2: 0000         	movs	r0, r0
    bbc4: 5180         	str	r0, [r0, r6]
    bbc6: 0001         	movs	r1, r0
    bbc8: 344c         	adds	r4, #0x4c
    bbca: 0000         	movs	r0, r0
    bbcc: 3412         	adds	r4, #0x12
    bbce: 0000         	movs	r0, r0
    bbd0: 33c8         	adds	r3, #0xc8
    bbd2: 0000         	movs	r0, r0
    bbd4: 210c         	movs	r1, #0xc
    bbd6: 0000         	movs	r0, r0
    bbd8: 0228         	lsls	r0, r5, #0x8
    bbda: 0000         	movs	r0, r0
    bbdc: 0208         	lsls	r0, r1, #0x8
    bbde: 0000         	movs	r0, r0

0000bbe0 <VolumeManager::setSoundOutputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, unsigned char)>:
    bbe0: b538         	push	{r3, r4, r5, lr}
    bbe2: 4604         	mov	r4, r0
    bbe4: 684b         	ldr	r3, [r1, #0x4]
    bbe6: 68c0         	ldr	r0, [r0, #0xc]
    bbe8: 4283         	cmp	r3, r0
    bbea: d000         	beq	0xbbee <VolumeManager::setSoundOutputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, unsigned char)+0xe> @ imm = #0x0
    bbec: bd38         	pop	{r3, r4, r5, pc}
    bbee: 6808         	ldr	r0, [r1]
    bbf0: 4615         	mov	r5, r2
    bbf2: 68a1         	ldr	r1, [r4, #0x8]
    bbf4: b123         	cbz	r3, 0xbc00 <VolumeManager::setSoundOutputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, unsigned char)+0x20> @ imm = #0x8
    bbf6: 461a         	mov	r2, r3
    bbf8: f7f8 eaaa    	blx	0x4150 <memcmp@plt>     @ imm = #-0x7aac
    bbfc: 2800         	cmp	r0, #0x0
    bbfe: d1f5         	bne	0xbbec <VolumeManager::setSoundOutputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, unsigned char)+0xc> @ imm = #-0x16
    bc00: f884 5021    	strb.w	r5, [r4, #0x21]
    bc04: bd38         	pop	{r3, r4, r5, pc}
    bc06: bf00         	nop

0000bc08 <VolumeManager::getSoundOutputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)>:
    bc08: 684a         	ldr	r2, [r1, #0x4]
    bc0a: 460b         	mov	r3, r1
    bc0c: 68c1         	ldr	r1, [r0, #0xc]
    bc0e: 428a         	cmp	r2, r1
    bc10: d001         	beq	0xbc16 <VolumeManager::getSoundOutputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0xe> @ imm = #0x2
    bc12: 2064         	movs	r0, #0x64
    bc14: 4770         	bx	lr
    bc16: b510         	push	{r4, lr}
    bc18: 4604         	mov	r4, r0
    bc1a: b122         	cbz	r2, 0xbc26 <VolumeManager::getSoundOutputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x1e> @ imm = #0x8
    bc1c: 6881         	ldr	r1, [r0, #0x8]
    bc1e: 6818         	ldr	r0, [r3]
    bc20: f7f8 ea96    	blx	0x4150 <memcmp@plt>     @ imm = #-0x7ad4
    bc24: b910         	cbnz	r0, 0xbc2c <VolumeManager::getSoundOutputVolume(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x24> @ imm = #0x4
    bc26: f894 0021    	ldrb.w	r0, [r4, #0x21]
    bc2a: bd10         	pop	{r4, pc}
    bc2c: 2064         	movs	r0, #0x64
    bc2e: bd10         	pop	{r4, pc}

0000bc30 <VolumeManager::setSoundOutputMute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool)>:
    bc30: b538         	push	{r3, r4, r5, lr}
    bc32: 4604         	mov	r4, r0
    bc34: 684b         	ldr	r3, [r1, #0x4]
    bc36: 68c0         	ldr	r0, [r0, #0xc]
    bc38: 4283         	cmp	r3, r0
    bc3a: d000         	beq	0xbc3e <VolumeManager::setSoundOutputMute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool)+0xe> @ imm = #0x0
    bc3c: bd38         	pop	{r3, r4, r5, pc}
    bc3e: 6808         	ldr	r0, [r1]
    bc40: 4615         	mov	r5, r2
    bc42: 68a1         	ldr	r1, [r4, #0x8]
    bc44: b123         	cbz	r3, 0xbc50 <VolumeManager::setSoundOutputMute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool)+0x20> @ imm = #0x8
    bc46: 461a         	mov	r2, r3
    bc48: f7f8 ea82    	blx	0x4150 <memcmp@plt>     @ imm = #-0x7afc
    bc4c: 2800         	cmp	r0, #0x0
    bc4e: d1f5         	bne	0xbc3c <VolumeManager::setSoundOutputMute(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, bool)+0xc> @ imm = #-0x16
    bc50: f884 5020    	strb.w	r5, [r4, #0x20]
    bc54: bd38         	pop	{r3, r4, r5, pc}
    bc56: bf00         	nop

0000bc58 <VolumeManager::updateSoundOutput(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)>:
    bc58: b538         	push	{r3, r4, r5, lr}
    bc5a: 4604         	mov	r4, r0
    bc5c: 684a         	ldr	r2, [r1, #0x4]
    bc5e: 460d         	mov	r5, r1
    bc60: 68c3         	ldr	r3, [r0, #0xc]
    bc62: 6808         	ldr	r0, [r1]
    bc64: 68a1         	ldr	r1, [r4, #0x8]
    bc66: 429a         	cmp	r2, r3
    bc68: d00d         	beq	0xbc86 <VolumeManager::updateSoundOutput(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x2e> @ imm = #0x1a
    bc6a: 4629         	mov	r1, r5
    bc6c: f104 0008    	add.w	r0, r4, #0x8
    bc70: f7f8 eace    	blx	0x4210 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@plt> @ imm = #-0x7a64
    bc74: 6860         	ldr	r0, [r4, #0x4]
    bc76: f7f8 e876    	blx	0x3d64 <_ZN9umiClient16getDefaultVolumeEv@plt> @ imm = #-0x7f14
    bc7a: 2300         	movs	r3, #0x0
    bc7c: f884 0021    	strb.w	r0, [r4, #0x21]
    bc80: f884 3020    	strb.w	r3, [r4, #0x20]
    bc84: bd38         	pop	{r3, r4, r5, pc}
    bc86: 2a00         	cmp	r2, #0x0
    bc88: d0fc         	beq	0xbc84 <VolumeManager::updateSoundOutput(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x2c> @ imm = #-0x8
    bc8a: f7f8 ea62    	blx	0x4150 <memcmp@plt>     @ imm = #-0x7b3c
    bc8e: 2800         	cmp	r0, #0x0
    bc90: d1eb         	bne	0xbc6a <VolumeManager::updateSoundOutput(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)+0x12> @ imm = #-0x2a
    bc92: bd38         	pop	{r3, r4, r5, pc}

0000bc94 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)>:
    bc94: e92d 41f0    	push.w	{r4, r5, r6, r7, r8, lr}
    bc98: b08c         	sub	sp, #0x30
    bc9a: 4606         	mov	r6, r0
    bc9c: 460c         	mov	r4, r1
    bc9e: 4615         	mov	r5, r2
    bca0: f7fe fc4c    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1768
    bca4: 2800         	cmp	r0, #0x0
    bca6: d03b         	beq	0xbd20 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x8c> @ imm = #0x76
    bca8: f7fe fc48    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1770
    bcac: 6803         	ldr	r3, [r0]
    bcae: 2b05         	cmp	r3, #0x5
    bcb0: dc36         	bgt	0xbd20 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x8c> @ imm = #0x6c
    bcb2: 6876         	ldr	r6, [r6, #0x4]
    bcb4: 2e00         	cmp	r6, #0x0
    bcb6: d048         	beq	0xbd4a <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xb6> @ imm = #0x90
    bcb8: e9d4 1200    	ldrd	r1, r2, [r4]
    bcbc: af06         	add	r7, sp, #0x18
    bcbe: 4638         	mov	r0, r7
    bcc0: f10d 0820    	add.w	r8, sp, #0x20
    bcc4: f8cd 8018    	str.w	r8, [sp, #0x18]
    bcc8: 440a         	add	r2, r1
    bcca: f7ff fe83    	bl	0xb9d4 <QualityManager::setQualityAttribute(LSMessage&)+0x354> @ imm = #-0x2fa
    bcce: 782a         	ldrb	r2, [r5]
    bcd0: 4639         	mov	r1, r7
    bcd2: 4630         	mov	r0, r6
    bcd4: f7f8 e804    	blx	0x3ce0 <_ZN9umiClient15setOutputVolumeENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEh@plt> @ imm = #-0x7ff8
    bcd8: 9b06         	ldr	r3, [sp, #0x18]
    bcda: 4604         	mov	r4, r0
    bcdc: 4543         	cmp	r3, r8
    bcde: d004         	beq	0xbcea <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x56> @ imm = #0x8
    bce0: 9908         	ldr	r1, [sp, #0x20]
    bce2: 4618         	mov	r0, r3
    bce4: 3101         	adds	r1, #0x1
    bce6: f7f8 e90c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7de8
    bcea: f7fe fc27    	bl	0xa53c <getPmLogContext()> @ imm = #-0x17b2
    bcee: b120         	cbz	r0, 0xbcfa <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x66> @ imm = #0x8
    bcf0: f7fe fc24    	bl	0xa53c <getPmLogContext()> @ imm = #-0x17b8
    bcf4: 6803         	ldr	r3, [r0]
    bcf6: 2b05         	cmp	r3, #0x5
    bcf8: dd0e         	ble	0xbd18 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x84> @ imm = #0x1c
    bcfa: f7fe fc1f    	bl	0xa53c <getPmLogContext()> @ imm = #-0x17c2
    bcfe: 4916         	ldr	r1, [pc, #0x58]         @ 0xbd58 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xc4> literal=0x3262
    bd00: 2200         	movs	r2, #0x0
    bd02: 4b16         	ldr	r3, [pc, #0x58]         @ 0xbd5c <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xc8> literal=0x3210
    bd04: 9404         	str	r4, [sp, #0x10]
    bd06: 4479         	add	r1, pc => raw=0xef6c ghidra=0x1ef6c file=61292 "set volume umi returnStatus %d"
    bd08: e9cd 2200    	strd	r2, r2, [sp]
    bd0c: e9cd 2102    	strd	r2, r1, [sp, #8]
    bd10: 447b         	add	r3, pc => raw=0xef24 ghidra=0x1ef24 file=61220 "AUDIO_SET_VOLUME_CALL"
    bd12: 2106         	movs	r1, #0x6
    bd14: f7f8 e982    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x7cfc
    bd18: 4620         	mov	r0, r4
    bd1a: b00c         	add	sp, #0x30
    bd1c: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
    bd20: f7fe fc0c    	bl	0xa53c <getPmLogContext()> @ imm = #-0x17e8
    bd24: 682b         	ldr	r3, [r5]
    bd26: 2200         	movs	r2, #0x0
    bd28: 2106         	movs	r1, #0x6
    bd2a: 9305         	str	r3, [sp, #0x14]
    bd2c: 6823         	ldr	r3, [r4]
    bd2e: e9cd 2201    	strd	r2, r2, [sp, #4]
    bd32: 9200         	str	r2, [sp]
    bd34: 9304         	str	r3, [sp, #0x10]
    bd36: 4b0a         	ldr	r3, [pc, #0x28]         @ 0xbd60 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xcc> literal=0x3200
    bd38: 447b         	add	r3, pc => raw=0xef3c ghidra=0x1ef3c file=61244 "set volume request for soundOutput:%s volume:%d"
    bd3a: 9303         	str	r3, [sp, #0xc]
    bd3c: 4b09         	ldr	r3, [pc, #0x24]         @ 0xbd64 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xd0> literal=0x31e2
    bd3e: 447b         	add	r3, pc => raw=0xef24 ghidra=0x1ef24 file=61220 "AUDIO_SET_VOLUME_CALL"
    bd40: f7f8 e96c    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x7d28
    bd44: 6876         	ldr	r6, [r6, #0x4]
    bd46: 2e00         	cmp	r6, #0x0
    bd48: d1b6         	bne	0xbcb8 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x24> @ imm = #-0x94
    bd4a: 2402         	movs	r4, #0x2
    bd4c: e7cd         	b	0xbcea <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x56> @ imm = #-0x66
    bd4e: 4638         	mov	r0, r7
    bd50: f7f8 e9ce    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7c64
    bd54: f7f8 ea14    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x7bd8
    bd58: 3262         	adds	r2, #0x62
    bd5a: 0000         	movs	r0, r0
    bd5c: 3210         	adds	r2, #0x10
    bd5e: 0000         	movs	r0, r0
    bd60: 3200         	adds	r2, #0x0
    bd62: 0000         	movs	r0, r0
    bd64: 31e2         	adds	r1, #0xe2
    bd66: 0000         	movs	r0, r0

0000bd68 <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)>:
    bd68: e92d 41f0    	push.w	{r4, r5, r6, r7, r8, lr}
    bd6c: b08c         	sub	sp, #0x30
    bd6e: 4606         	mov	r6, r0
    bd70: 460c         	mov	r4, r1
    bd72: 4615         	mov	r5, r2
    bd74: f7fe fbe2    	bl	0xa53c <getPmLogContext()> @ imm = #-0x183c
    bd78: 2800         	cmp	r0, #0x0
    bd7a: d03e         	beq	0xbdfa <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x92> @ imm = #0x7c
    bd7c: f7fe fbde    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1844
    bd80: 6803         	ldr	r3, [r0]
    bd82: 2b05         	cmp	r3, #0x5
    bd84: dc39         	bgt	0xbdfa <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x92> @ imm = #0x72
    bd86: 6876         	ldr	r6, [r6, #0x4]
    bd88: 2e00         	cmp	r6, #0x0
    bd8a: d04b         	beq	0xbe24 <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xbc> @ imm = #0x96
    bd8c: e9d4 1200    	ldrd	r1, r2, [r4]
    bd90: af06         	add	r7, sp, #0x18
    bd92: 4638         	mov	r0, r7
    bd94: f10d 0820    	add.w	r8, sp, #0x20
    bd98: f8cd 8018    	str.w	r8, [sp, #0x18]
    bd9c: 440a         	add	r2, r1
    bd9e: f7ff fe19    	bl	0xb9d4 <QualityManager::setQualityAttribute(LSMessage&)+0x354> @ imm = #-0x3ce
    bda2: 682a         	ldr	r2, [r5]
    bda4: 4639         	mov	r1, r7
    bda6: 4630         	mov	r0, r6
    bda8: 3a00         	subs	r2, #0x0
    bdaa: bf18         	it	ne
    bdac: 2201         	movne	r2, #0x1
    bdae: f7f8 ea0c    	blx	0x41c8 <_ZN9umiClient13setOutputMuteENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEb@plt> @ imm = #-0x7be8
    bdb2: 9b06         	ldr	r3, [sp, #0x18]
    bdb4: 4604         	mov	r4, r0
    bdb6: 4543         	cmp	r3, r8
    bdb8: d004         	beq	0xbdc4 <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x5c> @ imm = #0x8
    bdba: 9908         	ldr	r1, [sp, #0x20]
    bdbc: 4618         	mov	r0, r3
    bdbe: 3101         	adds	r1, #0x1
    bdc0: f7f8 e89e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7ec4
    bdc4: f7fe fbba    	bl	0xa53c <getPmLogContext()> @ imm = #-0x188c
    bdc8: b120         	cbz	r0, 0xbdd4 <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x6c> @ imm = #0x8
    bdca: f7fe fbb7    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1892
    bdce: 6803         	ldr	r3, [r0]
    bdd0: 2b05         	cmp	r3, #0x5
    bdd2: dd0e         	ble	0xbdf2 <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x8a> @ imm = #0x1c
    bdd4: f7fe fbb2    	bl	0xa53c <getPmLogContext()> @ imm = #-0x189c
    bdd8: 4916         	ldr	r1, [pc, #0x58]         @ 0xbe34 <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xcc> literal=0x31f0
    bdda: 2200         	movs	r2, #0x0
    bddc: 4b16         	ldr	r3, [pc, #0x58]         @ 0xbe38 <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xd0> literal=0x319e
    bdde: 9404         	str	r4, [sp, #0x10]
    bde0: 4479         	add	r1, pc => raw=0xefd4 ghidra=0x1efd4 file=61396 "muteSoundOut request umi returnStatus %d"
    bde2: e9cd 2200    	strd	r2, r2, [sp]
    bde6: e9cd 2102    	strd	r2, r1, [sp, #8]
    bdea: 447b         	add	r3, pc => raw=0xef8c ghidra=0x1ef8c file=61324 "AUDIO_MUTE_SOUNDOUTPUT_CALL"
    bdec: 2106         	movs	r1, #0x6
    bdee: f7f8 e916    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x7dd4
    bdf2: 4620         	mov	r0, r4
    bdf4: b00c         	add	sp, #0x30
    bdf6: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
    bdfa: f7fe fb9f    	bl	0xa53c <getPmLogContext()> @ imm = #-0x18c2
    bdfe: 682b         	ldr	r3, [r5]
    be00: 2200         	movs	r2, #0x0
    be02: 2106         	movs	r1, #0x6
    be04: 9305         	str	r3, [sp, #0x14]
    be06: 6823         	ldr	r3, [r4]
    be08: e9cd 2201    	strd	r2, r2, [sp, #4]
    be0c: 9200         	str	r2, [sp]
    be0e: 9304         	str	r3, [sp, #0x10]
    be10: 4b0a         	ldr	r3, [pc, #0x28]         @ 0xbe3c <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xd4> literal=0x3192
    be12: 447b         	add	r3, pc => raw=0xefa8 ghidra=0x1efa8 file=61352 "muteSoundOut for soundOutput:%s muteFlag:%d"
    be14: 9303         	str	r3, [sp, #0xc]
    be16: 4b0a         	ldr	r3, [pc, #0x28]         @ 0xbe40 <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0xd8> literal=0x3170
    be18: 447b         	add	r3, pc => raw=0xef8c ghidra=0x1ef8c file=61324 "AUDIO_MUTE_SOUNDOUTPUT_CALL"
    be1a: f7f8 e900    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x7e00
    be1e: 6876         	ldr	r6, [r6, #0x4]
    be20: 2e00         	cmp	r6, #0x0
    be22: d1b3         	bne	0xbd8c <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x24> @ imm = #-0x9a
    be24: 2402         	movs	r4, #0x2
    be26: e7cd         	b	0xbdc4 <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)+0x5c> @ imm = #-0x66
    be28: 4638         	mov	r0, r7
    be2a: f7f8 e962    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7d3c
    be2e: f7f8 e9a8    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x7cb0
    be32: bf00         	nop
    be34: 31f0         	adds	r1, #0xf0
    be36: 0000         	movs	r0, r0
    be38: 319e         	adds	r1, #0x9e
    be3a: 0000         	movs	r0, r0
    be3c: 3192         	adds	r1, #0x92
    be3e: 0000         	movs	r0, r0
    be40: 3170         	adds	r1, #0x70
    be42: 0000         	movs	r0, r0

0000be44 <VolumeManager::buildAudioStatus(AudioOutput&)>:
    be44: e92d 41f0    	push.w	{r4, r5, r6, r7, r8, lr}
    be48: b08a         	sub	sp, #0x28
    be4a: 4617         	mov	r7, r2
    be4c: ac01         	add	r4, sp, #0x4
    be4e: 4606         	mov	r6, r0
    be50: f7f7 efb2    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x809c
    be54: 4639         	mov	r1, r7
    be56: 4620         	mov	r0, r4
    be58: f7f8 e968    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x7d30
    be5c: 4a3a         	ldr	r2, [pc, #0xe8]         @ 0xbf48 <VolumeManager::buildAudioStatus(AudioOutput&)+0x104> literal=0x3190
    be5e: ad06         	add	r5, sp, #0x18
    be60: 462b         	mov	r3, r5
    be62: 9504         	str	r5, [sp, #0x10]
    be64: f10d 0810    	add.w	r8, sp, #0x10
    be68: f04f 0e0b    	mov.w	lr, #0xb
    be6c: 447a         	add	r2, pc => raw=0xf000 ghidra=0x1f000 file=61440 "soundOutput"
    be6e: f04f 0c00    	mov.w	r12, #0x0
    be72: ca07         	ldm	r2, {r0, r1, r2}
    be74: c303         	stm	r3!, {r0, r1}
    be76: 4641         	mov	r1, r8
    be78: f823 2b02    	strh	r2, [r3], #2
    be7c: 0c12         	lsrs	r2, r2, #0x10
    be7e: 4630         	mov	r0, r6
    be80: 701a         	strb	r2, [r3]
    be82: 4622         	mov	r2, r4
    be84: f8cd e014    	str.w	lr, [sp, #0x14]
    be88: f88d c023    	strb.w	r12, [sp, #0x23]
    be8c: f7f7 ef22    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x81bc
    be90: 9804         	ldr	r0, [sp, #0x10]
    be92: 42a8         	cmp	r0, r5
    be94: d003         	beq	0xbe9e <VolumeManager::buildAudioStatus(AudioOutput&)+0x5a> @ imm = #0x6
    be96: 9906         	ldr	r1, [sp, #0x18]
    be98: 3101         	adds	r1, #0x1
    be9a: f7f8 e832    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7f9c
    be9e: 4620         	mov	r0, r4
    bea0: f7f7 ef5a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x814c
    bea4: 7e79         	ldrb	r1, [r7, #0x19]
    bea6: 4620         	mov	r0, r4
    bea8: f7f8 e946    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x7d74
    beac: 4b27         	ldr	r3, [pc, #0x9c]         @ 0xbf4c <VolumeManager::buildAudioStatus(AudioOutput&)+0x108> literal=0x2426
    beae: 2206         	movs	r2, #0x6
    beb0: 9205         	str	r2, [sp, #0x14]
    beb2: 2200         	movs	r2, #0x0
    beb4: f88d 201e    	strb.w	r2, [sp, #0x1e]
    beb8: 4622         	mov	r2, r4
    beba: 447b         	add	r3, pc => raw=0xe2e4 ghidra=0x1e2e4 file=58084 "volume"
    bebc: 9504         	str	r5, [sp, #0x10]
    bebe: e893 0003    	ldm.w	r3, {r0, r1}
    bec2: 9006         	str	r0, [sp, #0x18]
    bec4: 4630         	mov	r0, r6
    bec6: f8ad 101c    	strh.w	r1, [sp, #0x1c]
    beca: 4641         	mov	r1, r8
    becc: f7f7 ef02    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x81fc
    bed0: 9804         	ldr	r0, [sp, #0x10]
    bed2: 42a8         	cmp	r0, r5
    bed4: d003         	beq	0xbede <VolumeManager::buildAudioStatus(AudioOutput&)+0x9a> @ imm = #0x6
    bed6: 9906         	ldr	r1, [sp, #0x18]
    bed8: 3101         	adds	r1, #0x1
    beda: f7f8 e812    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x7fdc
    bede: 4620         	mov	r0, r4
    bee0: f7f7 ef3a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x818c
    bee4: 7e39         	ldrb	r1, [r7, #0x18]
    bee6: 4620         	mov	r0, r4
    bee8: f7f7 ef84    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x80f8
    beec: 4b18         	ldr	r3, [pc, #0x60]         @ 0xbf50 <VolumeManager::buildAudioStatus(AudioOutput&)+0x10c> literal=0x1c12
    beee: 2205         	movs	r2, #0x5
    bef0: 9205         	str	r2, [sp, #0x14]
    bef2: 2200         	movs	r2, #0x0
    bef4: f88d 201d    	strb.w	r2, [sp, #0x1d]
    bef8: 4622         	mov	r2, r4
    befa: 447b         	add	r3, pc => raw=0xdb10 ghidra=0x1db10 file=56080 "muted"
    befc: 9504         	str	r5, [sp, #0x10]
    befe: e893 0003    	ldm.w	r3, {r0, r1}
    bf02: 9006         	str	r0, [sp, #0x18]
    bf04: 4630         	mov	r0, r6
    bf06: f88d 101c    	strb.w	r1, [sp, #0x1c]
    bf0a: 4641         	mov	r1, r8
    bf0c: f7f7 eee2    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x823c
    bf10: 9804         	ldr	r0, [sp, #0x10]
    bf12: 42a8         	cmp	r0, r5
    bf14: d003         	beq	0xbf1e <VolumeManager::buildAudioStatus(AudioOutput&)+0xda> @ imm = #0x6
    bf16: 9906         	ldr	r1, [sp, #0x18]
    bf18: 3101         	adds	r1, #0x1
    bf1a: f7f7 eff2    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x801c
    bf1e: 4620         	mov	r0, r4
    bf20: f7f7 ef1a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x81cc
    bf24: 4630         	mov	r0, r6
    bf26: b00a         	add	sp, #0x28
    bf28: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
    bf2c: 4630         	mov	r0, r6
    bf2e: f7f7 ef14    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x81d8
    bf32: f7f8 e926    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x7db4
    bf36: 4640         	mov	r0, r8
    bf38: f7f8 e8da    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7e4c
    bf3c: 4620         	mov	r0, r4
    bf3e: f7f7 ef0c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x81e8
    bf42: e7f3         	b	0xbf2c <VolumeManager::buildAudioStatus(AudioOutput&)+0xe8> @ imm = #-0x1a
    bf44: e7f7         	b	0xbf36 <VolumeManager::buildAudioStatus(AudioOutput&)+0xf2> @ imm = #-0x12
    bf46: e7f6         	b	0xbf36 <VolumeManager::buildAudioStatus(AudioOutput&)+0xf2> @ imm = #-0x14
    bf48: 3190         	adds	r1, #0x90
    bf4a: 0000         	movs	r0, r0
    bf4c: 2426         	movs	r4, #0x26
    bf4e: 0000         	movs	r0, r0
    bf50: 1c12         	adds	r2, r2, #0x0
    bf52: 0000         	movs	r0, r0

0000bf54 <VolumeManager::buildAudioStatus()>:
    bf54: e92d 43f0    	push.w	{r4, r5, r6, r7, r8, r9, lr}
    bf58: 4605         	mov	r5, r0
    bf5a: b08d         	sub	sp, #0x34
    bf5c: 2000         	movs	r0, #0x0
    bf5e: 460c         	mov	r4, r1
    bf60: f7f8 e8d8    	blx	0x4114 <jarray_create@plt> @ imm = #-0x7e50
    bf64: 4a43         	ldr	r2, [pc, #0x10c]        @ 0xc074 <VolumeManager::buildAudioStatus()+0x120> literal=0x14e50
    bf66: 2300         	movs	r3, #0x0
    bf68: 9301         	str	r3, [sp, #0x4]
    bf6a: 9002         	str	r0, [sp, #0x8]
    bf6c: 447a         	add	r2, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    bf6e: 2800         	cmp	r0, #0x0
    bf70: d05f         	beq	0xc032 <VolumeManager::buildAudioStatus()+0xde> @ imm = #0xbe
    bf72: 4b41         	ldr	r3, [pc, #0x104]        @ 0xc078 <VolumeManager::buildAudioStatus()+0x124> literal=0x20c
    bf74: 4628         	mov	r0, r5
    bf76: 58d7         	ldr	r7, [r2, r3]
    bf78: f107 0308    	add.w	r3, r7, #0x8
    bf7c: 9300         	str	r3, [sp]
    bf7e: f7f7 ef1c    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x81c8
    bf82: ae06         	add	r6, sp, #0x18
    bf84: f104 0208    	add.w	r2, r4, #0x8
    bf88: 4621         	mov	r1, r4
    bf8a: 4630         	mov	r0, r6
    bf8c: 46e8         	mov	r8, sp
    bf8e: f7ff ff59    	bl	0xbe44 <VolumeManager::buildAudioStatus(AudioOutput&)> @ imm = #-0x14e
    bf92: 4631         	mov	r1, r6
    bf94: 4640         	mov	r0, r8
    bf96: f7f7 eef2    	blx	0x3d7c <_ZN7pbnjson6JValue6appendERKS0_@plt> @ imm = #-0x821c
    bf9a: 4630         	mov	r0, r6
    bf9c: f10d 090c    	add.w	r9, sp, #0xc
    bfa0: f7f7 eeda    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x824c
    bfa4: 2101         	movs	r1, #0x1
    bfa6: 4648         	mov	r0, r9
    bfa8: f7f7 ef24    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x81b8
    bfac: 4a33         	ldr	r2, [pc, #0xcc]         @ 0xc07c <VolumeManager::buildAudioStatus()+0x128> literal=0x1764
    bfae: ac08         	add	r4, sp, #0x20
    bfb0: 4623         	mov	r3, r4
    bfb2: 9406         	str	r4, [sp, #0x18]
    bfb4: f04f 0e0b    	mov.w	lr, #0xb
    bfb8: f04f 0c00    	mov.w	r12, #0x0
    bfbc: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    bfbe: ca07         	ldm	r2, {r0, r1, r2}
    bfc0: c303         	stm	r3!, {r0, r1}
    bfc2: 4631         	mov	r1, r6
    bfc4: f823 2b02    	strh	r2, [r3], #2
    bfc8: 0c12         	lsrs	r2, r2, #0x10
    bfca: 4628         	mov	r0, r5
    bfcc: 701a         	strb	r2, [r3]
    bfce: 464a         	mov	r2, r9
    bfd0: f8cd e01c    	str.w	lr, [sp, #0x1c]
    bfd4: f88d c02b    	strb.w	r12, [sp, #0x2b]
    bfd8: f7f7 ee7c    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8308
    bfdc: 9806         	ldr	r0, [sp, #0x18]
    bfde: 42a0         	cmp	r0, r4
    bfe0: d003         	beq	0xbfea <VolumeManager::buildAudioStatus()+0x96> @ imm = #0x6
    bfe2: 9908         	ldr	r1, [sp, #0x20]
    bfe4: 3101         	adds	r1, #0x1
    bfe6: f7f7 ef8c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x80e8
    bfea: 4648         	mov	r0, r9
    bfec: f7f7 eeb4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8298
    bff0: 4b23         	ldr	r3, [pc, #0x8c]         @ 0xc080 <VolumeManager::buildAudioStatus()+0x12c> literal=0x300c
    bff2: 220c         	movs	r2, #0xc
    bff4: 9207         	str	r2, [sp, #0x1c]
    bff6: 2200         	movs	r2, #0x0
    bff8: f88d 202c    	strb.w	r2, [sp, #0x2c]
    bffc: 447b         	add	r3, pc => raw=0xf00c ghidra=0x1f00c file=61452 "volumeStatus"
    bffe: 9406         	str	r4, [sp, #0x18]
    c000: e893 0007    	ldm.w	r3, {r0, r1, r2}
    c004: e884 0007    	stm.w	r4, {r0, r1, r2}
    c008: 4642         	mov	r2, r8
    c00a: 4631         	mov	r1, r6
    c00c: 4628         	mov	r0, r5
    c00e: f7f7 ee62    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x833c
    c012: 9806         	ldr	r0, [sp, #0x18]
    c014: 42a0         	cmp	r0, r4
    c016: d003         	beq	0xc020 <VolumeManager::buildAudioStatus()+0xcc> @ imm = #0x6
    c018: 9908         	ldr	r1, [sp, #0x20]
    c01a: 3101         	adds	r1, #0x1
    c01c: f7f7 ef70    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8120
    c020: 3708         	adds	r7, #0x8
    c022: 4640         	mov	r0, r8
    c024: 9700         	str	r7, [sp]
    c026: f7f7 ef92    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x80dc
    c02a: 4628         	mov	r0, r5
    c02c: b00d         	add	sp, #0x34
    c02e: e8bd 83f0    	pop.w	{r4, r5, r6, r7, r8, r9, pc}
    c032: 4b14         	ldr	r3, [pc, #0x50]         @ 0xc084 <VolumeManager::buildAudioStatus()+0x130> literal=0x230
    c034: 58d3         	ldr	r3, [r2, r3]
    c036: 689b         	ldr	r3, [r3, #0x8]
    c038: 9302         	str	r3, [sp, #0x8]
    c03a: e79a         	b	0xbf72 <VolumeManager::buildAudioStatus()+0x1e> @ imm = #-0xcc
    c03c: 4630         	mov	r0, r6
    c03e: f7f8 e858    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7f50
    c042: 4628         	mov	r0, r5
    c044: f7f7 ee88    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x82f0
    c048: 3708         	adds	r7, #0x8
    c04a: 4640         	mov	r0, r8
    c04c: 9700         	str	r7, [sp]
    c04e: f7f7 ef7e    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x8104
    c052: f7f8 e896    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x7ed4
    c056: 4630         	mov	r0, r6
    c058: f7f8 e84a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x7f6c
    c05c: 4648         	mov	r0, r9
    c05e: f7f7 ee7c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8308
    c062: e7ee         	b	0xc042 <VolumeManager::buildAudioStatus()+0xee> @ imm = #-0x24
    c064: 4630         	mov	r0, r6
    c066: f7f7 ee78    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8310
    c06a: e7ea         	b	0xc042 <VolumeManager::buildAudioStatus()+0xee> @ imm = #-0x2c
    c06c: e7e9         	b	0xc042 <VolumeManager::buildAudioStatus()+0xee> @ imm = #-0x2e
    c06e: 46e8         	mov	r8, sp
    c070: e7ea         	b	0xc048 <VolumeManager::buildAudioStatus()+0xf4> @ imm = #-0x2c
    c072: bf00         	nop
    c074: 4e50         	ldr	r6, [pc, #0x140]        @ 0xc1b8 <VolumeManager::set(LSMessage&)+0x78> literal=0xef74f7f7
    c076: 0001         	movs	r1, r0
    c078: 020c         	lsls	r4, r1, #0x8
    c07a: 0000         	movs	r0, r0
    c07c: 1764         	asrs	r4, r4, #0x1d
    c07e: 0000         	movs	r0, r0
    c080: 300c         	adds	r0, #0xc
    c082: 0000         	movs	r0, r0
    c084: 0230         	lsls	r0, r6, #0x8
    c086: 0000         	movs	r0, r0

0000c088 <VolumeManager::getStatus()>:
    c088: b570         	push	{r4, r5, r6, lr}
    c08a: b08c         	sub	sp, #0x30
    c08c: 4604         	mov	r4, r0
    c08e: 460d         	mov	r5, r1
    c090: f7fe fa54    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1b58
    c094: b398         	cbz	r0, 0xc0fe <VolumeManager::getStatus()+0x76> @ imm = #0x66
    c096: f7fe fa51    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1b5e
    c09a: 6803         	ldr	r3, [r0]
    c09c: 2b05         	cmp	r3, #0x5
    c09e: dc2e         	bgt	0xc0fe <VolumeManager::getStatus()+0x76> @ imm = #0x5c
    c0a0: 4629         	mov	r1, r5
    c0a2: 4620         	mov	r0, r4
    c0a4: f7ff ff56    	bl	0xbf54 <VolumeManager::buildAudioStatus()> @ imm = #-0x154
    c0a8: f7fe fa48    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1b70
    c0ac: b120         	cbz	r0, 0xc0b8 <VolumeManager::getStatus()+0x30> @ imm = #0x8
    c0ae: f7fe fa45    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1b76
    c0b2: 6803         	ldr	r3, [r0]
    c0b4: 2b05         	cmp	r3, #0x5
    c0b6: dd1f         	ble	0xc0f8 <VolumeManager::getStatus()+0x70> @ imm = #0x3e
    c0b8: f7fe fa40    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1b80
    c0bc: ae06         	add	r6, sp, #0x18
    c0be: 4605         	mov	r5, r0
    c0c0: 2200         	movs	r2, #0x0
    c0c2: 4621         	mov	r1, r4
    c0c4: 4630         	mov	r0, r6
    c0c6: f7f8 e892    	blx	0x41ec <_ZN7pbnjson6JValue9stringifyB5cxx11EPKc@plt> @ imm = #-0x7edc
    c0ca: 4b19         	ldr	r3, [pc, #0x64]         @ 0xc130 <VolumeManager::getStatus()+0xa8> literal=0x2f62
    c0cc: 4628         	mov	r0, r5
    c0ce: 9a06         	ldr	r2, [sp, #0x18]
    c0d0: 2106         	movs	r1, #0x6
    c0d2: 447b         	add	r3, pc => raw=0xf038 ghidra=0x1f038 file=61496 "Volume getStatus response:%s"
    c0d4: e9cd 3203    	strd	r3, r2, [sp, #12]
    c0d8: 2200         	movs	r2, #0x0
    c0da: 4b16         	ldr	r3, [pc, #0x58]         @ 0xc134 <VolumeManager::getStatus()+0xac> literal=0x1a3a
    c0dc: e9cd 2201    	strd	r2, r2, [sp, #4]
    c0e0: 9200         	str	r2, [sp]
    c0e2: 447b         	add	r3, pc => raw=0xdb20 ghidra=0x1db20 file=56096 "AUDIO_GET_STATUS_CALL"
    c0e4: f7f7 ef9a    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x80cc
    c0e8: 9806         	ldr	r0, [sp, #0x18]
    c0ea: ab08         	add	r3, sp, #0x20
    c0ec: 4298         	cmp	r0, r3
    c0ee: d003         	beq	0xc0f8 <VolumeManager::getStatus()+0x70> @ imm = #0x6
    c0f0: 9908         	ldr	r1, [sp, #0x20]
    c0f2: 3101         	adds	r1, #0x1
    c0f4: f7f7 ef04    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x81f8
    c0f8: 4620         	mov	r0, r4
    c0fa: b00c         	add	sp, #0x30
    c0fc: bd70         	pop	{r4, r5, r6, pc}
    c0fe: f7fe fa1d    	bl	0xa53c <getPmLogContext()> @ imm = #-0x1bc6
    c102: 4b0d         	ldr	r3, [pc, #0x34]         @ 0xc138 <VolumeManager::getStatus()+0xb0> literal=0x2f0a
    c104: 2200         	movs	r2, #0x0
    c106: 2106         	movs	r1, #0x6
    c108: e9cd 2201    	strd	r2, r2, [sp, #4]
    c10c: 9200         	str	r2, [sp]
    c10e: 447b         	add	r3, pc => raw=0xf01c ghidra=0x1f01c file=61468 "Volume getStatus request"
    c110: 9303         	str	r3, [sp, #0xc]
    c112: 4b0a         	ldr	r3, [pc, #0x28]         @ 0xc13c <VolumeManager::getStatus()+0xb4> literal=0x1a08
    c114: 447b         	add	r3, pc => raw=0xdb20 ghidra=0x1db20 file=56096 "AUDIO_GET_STATUS_CALL"
    c116: f7f7 ef82    	blx	0x401c <_PmLogMsgKV@plt> @ imm = #-0x80fc
    c11a: e7c1         	b	0xc0a0 <VolumeManager::getStatus()+0x18> @ imm = #-0x7e
    c11c: 4630         	mov	r0, r6
    c11e: f7f7 efe8    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8030
    c122: 4620         	mov	r0, r4
    c124: f7f7 ee18    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x83d0
    c128: f7f8 e82a    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x7fac
    c12c: e7f9         	b	0xc122 <VolumeManager::getStatus()+0x9a> @ imm = #-0xe
    c12e: bf00         	nop
    c130: 2f62         	cmp	r7, #0x62
    c132: 0000         	movs	r0, r0
    c134: 1a3a         	subs	r2, r7, r0
    c136: 0000         	movs	r0, r0
    c138: 2f0a         	cmp	r7, #0xa
    c13a: 0000         	movs	r0, r0
    c13c: 1a08         	subs	r0, r1, r0
    c13e: 0000         	movs	r0, r0

0000c140 <VolumeManager::set(LSMessage&)>:
    c140: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    c144: b0a3         	sub	sp, #0x8c
    c146: f8df a49c    	ldr.w	r10, [pc, #0x49c]       @ 0xc5e4 <VolumeManager::set(LSMessage&)+0x4a4> literal=0x14c68
    c14a: 2300         	movs	r3, #0x0
    c14c: aa12         	add	r2, sp, #0x48
    c14e: 4680         	mov	r8, r0
    c150: 4608         	mov	r0, r1
    c152: 460c         	mov	r4, r1
    c154: 44fa         	add	r10, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    c156: 9200         	str	r2, [sp]
    c158: e9cd 2310    	strd	r2, r3, [sp, #64]
    c15c: f88d 3048    	strb.w	r3, [sp, #0x48]
    c160: e9cd 3105    	strd	r3, r1, [sp, #20]
    c164: f7f7 efbe    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x8084
    c168: af07         	add	r7, sp, #0x1c
    c16a: 4638         	mov	r0, r7
    c16c: f7f8 e856    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x7f54
    c170: ab18         	add	r3, sp, #0x60
    c172: f10d 0958    	add.w	r9, sp, #0x58
    c176: ae1c         	add	r6, sp, #0x70
    c178: 9301         	str	r3, [sp, #0x4]
    c17a: 9316         	str	r3, [sp, #0x58]
    c17c: 2200         	movs	r2, #0x0
    c17e: 239d         	movs	r3, #0x9d
    c180: 4631         	mov	r1, r6
    c182: 4648         	mov	r0, r9
    c184: 931c         	str	r3, [sp, #0x70]
    c186: f7f7 ef76    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x8114
    c18a: f8df 145c    	ldr.w	r1, [pc, #0x45c]        @ 0xc5e8 <VolumeManager::set(LSMessage&)+0x4a8> literal=0x2ec0
    c18e: 229d         	movs	r2, #0x9d
    c190: 9b1c         	ldr	r3, [sp, #0x70]
    c192: 9016         	str	r0, [sp, #0x58]
    c194: 4479         	add	r1, pc => raw=0xf058 ghidra=0x1f058 file=61528 "{\"type\":\"object\",\"properties\":{\"soundOutput\":{\"type\":\"string\"},\"volume\":{\"type\":\"integer\"}},\"required\":[\"soundOutput\",\"volume\"],\"additionalProperties\":false}"
    c196: 9318         	str	r3, [sp, #0x60]
    c198: f7f7 eef6    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x8214
    c19c: 9b1c         	ldr	r3, [sp, #0x70]
    c19e: 2100         	movs	r1, #0x0
    c1a0: 9a16         	ldr	r2, [sp, #0x58]
    c1a2: 4620         	mov	r0, r4
    c1a4: 9317         	str	r3, [sp, #0x5c]
    c1a6: 54d1         	strb	r1, [r2, r3]
    c1a8: f7f7 eff6    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x8014
    c1ac: ac1e         	add	r4, sp, #0x78
    c1ae: 4683         	mov	r11, r0
    c1b0: 941c         	str	r4, [sp, #0x70]
    c1b2: 2800         	cmp	r0, #0x0
    c1b4: f000 81c9    	beq.w	0xc54a <VolumeManager::set(LSMessage&)+0x40a> @ imm = #0x392
    c1b8: f7f7 ef74    	blx	0x40a4 <strlen@plt>     @ imm = #-0x8118
    c1bc: 280f         	cmp	r0, #0xf
    c1be: 4605         	mov	r5, r0
    c1c0: 900d         	str	r0, [sp, #0x34]
    c1c2: f200 8122    	bhi.w	0xc40a <VolumeManager::set(LSMessage&)+0x2ca> @ imm = #0x244
    c1c6: 2801         	cmp	r0, #0x1
    c1c8: f040 8110    	bne.w	0xc3ec <VolumeManager::set(LSMessage&)+0x2ac> @ imm = #0x220
    c1cc: f89b 3000    	ldrb.w	r3, [r11]
    c1d0: f88d 3078    	strb.w	r3, [sp, #0x78]
    c1d4: 4623         	mov	r3, r4
    c1d6: 2200         	movs	r2, #0x0
    c1d8: 951d         	str	r5, [sp, #0x74]
    c1da: 4639         	mov	r1, r7
    c1dc: 555a         	strb	r2, [r3, r5]
    c1de: 4630         	mov	r0, r6
    c1e0: ab05         	add	r3, sp, #0x14
    c1e2: 464a         	mov	r2, r9
    c1e4: f7fe f8f8    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #-0x1e10
    c1e8: 4605         	mov	r5, r0
    c1ea: 981c         	ldr	r0, [sp, #0x70]
    c1ec: 42a0         	cmp	r0, r4
    c1ee: d003         	beq	0xc1f8 <VolumeManager::set(LSMessage&)+0xb8> @ imm = #0x6
    c1f0: 991e         	ldr	r1, [sp, #0x78]
    c1f2: 3101         	adds	r1, #0x1
    c1f4: f7f7 ee84    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x82f8
    c1f8: 2d00         	cmp	r5, #0x0
    c1fa: f000 80fd    	beq.w	0xc3f8 <VolumeManager::set(LSMessage&)+0x2b8> @ imm = #0x1fa
    c1fe: 4afb         	ldr	r2, [pc, #0x3ec]        @ 0xc5ec <VolumeManager::set(LSMessage&)+0x4ac> literal=0x2df6
    c200: ad0d         	add	r5, sp, #0x34
    c202: 4639         	mov	r1, r7
    c204: 4628         	mov	r0, r5
    c206: 447a         	add	r2, pc => raw=0xf000 ghidra=0x1f000 file=61440 "soundOutput"
    c208: f7f7 ee80    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x8300
    c20c: 2300         	movs	r3, #0x0
    c20e: 4631         	mov	r1, r6
    c210: 4628         	mov	r0, r5
    c212: 941c         	str	r4, [sp, #0x70]
    c214: 931d         	str	r3, [sp, #0x74]
    c216: f88d 3078    	strb.w	r3, [sp, #0x78]
    c21a: f7f7 edc2    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x847c
    c21e: 9b1c         	ldr	r3, [sp, #0x70]
    c220: 9810         	ldr	r0, [sp, #0x40]
    c222: 9a1d         	ldr	r2, [sp, #0x74]
    c224: 42a3         	cmp	r3, r4
    c226: f000 8117    	beq.w	0xc458 <VolumeManager::set(LSMessage&)+0x318> @ imm = #0x22e
    c22a: 9900         	ldr	r1, [sp]
    c22c: f8dd c078    	ldr.w	r12, [sp, #0x78]
    c230: e9cd 3210    	strd	r3, r2, [sp, #64]
    c234: 4288         	cmp	r0, r1
    c236: bf08         	it	eq
    c238: f8cd c048    	streq.w	r12, [sp, #0x48]
    c23c: f000 817b    	beq.w	0xc536 <VolumeManager::set(LSMessage&)+0x3f6> @ imm = #0x2f6
    c240: 9b12         	ldr	r3, [sp, #0x48]
    c242: f8cd c048    	str.w	r12, [sp, #0x48]
    c246: 2800         	cmp	r0, #0x0
    c248: f000 8175    	beq.w	0xc536 <VolumeManager::set(LSMessage&)+0x3f6> @ imm = #0x2ea
    c24c: 931e         	str	r3, [sp, #0x78]
    c24e: 901c         	str	r0, [sp, #0x70]
    c250: 2300         	movs	r3, #0x0
    c252: 931d         	str	r3, [sp, #0x74]
    c254: 7003         	strb	r3, [r0]
    c256: 981c         	ldr	r0, [sp, #0x70]
    c258: 42a0         	cmp	r0, r4
    c25a: d003         	beq	0xc264 <VolumeManager::set(LSMessage&)+0x124> @ imm = #0x6
    c25c: 991e         	ldr	r1, [sp, #0x78]
    c25e: 3101         	adds	r1, #0x1
    c260: f7f7 ee4e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8364
    c264: 4be2         	ldr	r3, [pc, #0x388]        @ 0xc5f0 <VolumeManager::set(LSMessage&)+0x4b0> literal=0x210
    c266: 4628         	mov	r0, r5
    c268: f85a a003    	ldr.w	r10, [r10, r3]
    c26c: f10a 0308    	add.w	r3, r10, #0x8
    c270: 930d         	str	r3, [sp, #0x34]
    c272: f7f7 ee6c    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x8328
    c276: 4adf         	ldr	r2, [pc, #0x37c]        @ 0xc5f4 <VolumeManager::set(LSMessage&)+0x4b4> literal=0x2064
    c278: 4639         	mov	r1, r7
    c27a: 4630         	mov	r0, r6
    c27c: 447a         	add	r2, pc => raw=0xe2e4 ghidra=0x1e2e4 file=58084 "volume"
    c27e: f7f7 ee46    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x8374
    c282: 4630         	mov	r0, r6
    c284: f7f7 ef94    	blx	0x41b0 <_ZNK7pbnjson6JValue8asNumberIiEET_v@plt> @ imm = #-0x80d8
    c288: f10a 0308    	add.w	r3, r10, #0x8
    c28c: 4683         	mov	r11, r0
    c28e: f10d 0a28    	add.w	r10, sp, #0x28
    c292: 4630         	mov	r0, r6
    c294: 931c         	str	r3, [sp, #0x70]
    c296: f7f7 ee5a    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x834c
    c29a: 4650         	mov	r0, r10
    c29c: f7f7 ed8c    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x84e8
    c2a0: fa5f f38b    	uxtb.w	r3, r11
    c2a4: 2b64         	cmp	r3, #0x64
    c2a6: 9302         	str	r3, [sp, #0x8]
    c2a8: f200 80be    	bhi.w	0xc428 <VolumeManager::set(LSMessage&)+0x2e8> @ imm = #0x17c
    c2ac: fa4f f38b    	sxtb.w	r3, r11
    c2b0: f10d 0b40    	add.w	r11, sp, #0x40
    c2b4: 4632         	mov	r2, r6
    c2b6: 4659         	mov	r1, r11
    c2b8: 4640         	mov	r0, r8
    c2ba: 9303         	str	r3, [sp, #0xc]
    c2bc: 931c         	str	r3, [sp, #0x70]
    c2be: f7ff fce9    	bl	0xbc94 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)> @ imm = #-0x62e
    c2c2: 2800         	cmp	r0, #0x0
    c2c4: f040 80d5    	bne.w	0xc472 <VolumeManager::set(LSMessage&)+0x332> @ imm = #0x1aa
    c2c8: 9a11         	ldr	r2, [sp, #0x44]
    c2ca: f8d8 300c    	ldr.w	r3, [r8, #0xc]
    c2ce: 429a         	cmp	r2, r3
    c2d0: f000 80b5    	beq.w	0xc43e <VolumeManager::set(LSMessage&)+0x2fe> @ imm = #0x16a
    c2d4: 2101         	movs	r1, #0x1
    c2d6: 4628         	mov	r0, r5
    c2d8: f7f7 ed8c    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x84e8
    c2dc: 4ac6         	ldr	r2, [pc, #0x318]        @ 0xc5f8 <VolumeManager::set(LSMessage&)+0x4b8> literal=0x1436
    c2de: 4623         	mov	r3, r4
    c2e0: 941c         	str	r4, [sp, #0x70]
    c2e2: f04f 0e0b    	mov.w	lr, #0xb
    c2e6: f04f 0c00    	mov.w	r12, #0x0
    c2ea: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    c2ec: ca07         	ldm	r2, {r0, r1, r2}
    c2ee: c303         	stm	r3!, {r0, r1}
    c2f0: 4631         	mov	r1, r6
    c2f2: f823 2b02    	strh	r2, [r3], #2
    c2f6: 0c12         	lsrs	r2, r2, #0x10
    c2f8: 4650         	mov	r0, r10
    c2fa: 701a         	strb	r2, [r3]
    c2fc: 462a         	mov	r2, r5
    c2fe: f8cd e074    	str.w	lr, [sp, #0x74]
    c302: f88d c083    	strb.w	r12, [sp, #0x83]
    c306: f7f7 ece6    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8634
    c30a: 981c         	ldr	r0, [sp, #0x70]
    c30c: 42a0         	cmp	r0, r4
    c30e: d003         	beq	0xc318 <VolumeManager::set(LSMessage&)+0x1d8> @ imm = #0x6
    c310: 991e         	ldr	r1, [sp, #0x78]
    c312: 3101         	adds	r1, #0x1
    c314: f7f7 edf4    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8418
    c318: 4628         	mov	r0, r5
    c31a: f7f7 ed1e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x85c4
    c31e: 4659         	mov	r1, r11
    c320: 4628         	mov	r0, r5
    c322: f7f7 ef04    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x81f8
    c326: 4ab5         	ldr	r2, [pc, #0x2d4]        @ 0xc5fc <VolumeManager::set(LSMessage&)+0x4bc> literal=0x2cc8
    c328: 4623         	mov	r3, r4
    c32a: 941c         	str	r4, [sp, #0x70]
    c32c: f04f 0e0b    	mov.w	lr, #0xb
    c330: f04f 0c00    	mov.w	r12, #0x0
    c334: 447a         	add	r2, pc => raw=0xf000 ghidra=0x1f000 file=61440 "soundOutput"
    c336: ca07         	ldm	r2, {r0, r1, r2}
    c338: c303         	stm	r3!, {r0, r1}
    c33a: 4631         	mov	r1, r6
    c33c: f823 2b02    	strh	r2, [r3], #2
    c340: 0c12         	lsrs	r2, r2, #0x10
    c342: 4650         	mov	r0, r10
    c344: 701a         	strb	r2, [r3]
    c346: 462a         	mov	r2, r5
    c348: f8cd e074    	str.w	lr, [sp, #0x74]
    c34c: f88d c083    	strb.w	r12, [sp, #0x83]
    c350: f7f7 ecc0    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8680
    c354: 981c         	ldr	r0, [sp, #0x70]
    c356: 42a0         	cmp	r0, r4
    c358: d003         	beq	0xc362 <VolumeManager::set(LSMessage&)+0x222> @ imm = #0x6
    c35a: 991e         	ldr	r1, [sp, #0x78]
    c35c: 3101         	adds	r1, #0x1
    c35e: f7f7 edd0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8460
    c362: 4628         	mov	r0, r5
    c364: f7f7 ecf8    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8610
    c368: 9903         	ldr	r1, [sp, #0xc]
    c36a: 4628         	mov	r0, r5
    c36c: f7f7 eee4    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x8238
    c370: 4ba3         	ldr	r3, [pc, #0x28c]        @ 0xc600 <VolumeManager::set(LSMessage&)+0x4c0> literal=0x1f62
    c372: 2206         	movs	r2, #0x6
    c374: 921d         	str	r2, [sp, #0x74]
    c376: 2200         	movs	r2, #0x0
    c378: f88d 207e    	strb.w	r2, [sp, #0x7e]
    c37c: 462a         	mov	r2, r5
    c37e: 447b         	add	r3, pc => raw=0xe2e4 ghidra=0x1e2e4 file=58084 "volume"
    c380: 941c         	str	r4, [sp, #0x70]
    c382: e893 0003    	ldm.w	r3, {r0, r1}
    c386: 901e         	str	r0, [sp, #0x78]
    c388: 4650         	mov	r0, r10
    c38a: f8ad 107c    	strh.w	r1, [sp, #0x7c]
    c38e: 4631         	mov	r1, r6
    c390: f7f7 eca0    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x86c0
    c394: 981c         	ldr	r0, [sp, #0x70]
    c396: 42a0         	cmp	r0, r4
    c398: d003         	beq	0xc3a2 <VolumeManager::set(LSMessage&)+0x262> @ imm = #0x6
    c39a: 991e         	ldr	r1, [sp, #0x78]
    c39c: 3101         	adds	r1, #0x1
    c39e: f7f7 edb0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x84a0
    c3a2: 4628         	mov	r0, r5
    c3a4: f7f7 ecd8    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8650
    c3a8: 4651         	mov	r1, r10
    c3aa: a806         	add	r0, sp, #0x18
    c3ac: f7fd fd70    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #-0x2520
    c3b0: 4650         	mov	r0, r10
    c3b2: f7f7 ecd2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x865c
    c3b6: 9816         	ldr	r0, [sp, #0x58]
    c3b8: 9b01         	ldr	r3, [sp, #0x4]
    c3ba: 4298         	cmp	r0, r3
    c3bc: d003         	beq	0xc3c6 <VolumeManager::set(LSMessage&)+0x286> @ imm = #0x6
    c3be: 9918         	ldr	r1, [sp, #0x60]
    c3c0: 3101         	adds	r1, #0x1
    c3c2: f7f7 ed9e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x84c4
    c3c6: 4638         	mov	r0, r7
    c3c8: f7f7 ecc6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8674
    c3cc: 9806         	ldr	r0, [sp, #0x18]
    c3ce: b108         	cbz	r0, 0xc3d4 <VolumeManager::set(LSMessage&)+0x294> @ imm = #0x2
    c3d0: f7f7 ee3c    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x8388
    c3d4: 9810         	ldr	r0, [sp, #0x40]
    c3d6: 9b00         	ldr	r3, [sp]
    c3d8: 4298         	cmp	r0, r3
    c3da: d003         	beq	0xc3e4 <VolumeManager::set(LSMessage&)+0x2a4> @ imm = #0x6
    c3dc: 9912         	ldr	r1, [sp, #0x48]
    c3de: 3101         	adds	r1, #0x1
    c3e0: f7f7 ed8e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x84e4
    c3e4: 2001         	movs	r0, #0x1
    c3e6: b023         	add	sp, #0x8c
    c3e8: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    c3ec: 4623         	mov	r3, r4
    c3ee: 2800         	cmp	r0, #0x0
    c3f0: f43f aef1    	beq.w	0xc1d6 <VolumeManager::set(LSMessage&)+0x96> @ imm = #-0x21e
    c3f4: 4620         	mov	r0, r4
    c3f6: e010         	b	0xc41a <VolumeManager::set(LSMessage&)+0x2da> @ imm = #0x20
    c3f8: 4982         	ldr	r1, [pc, #0x208]        @ 0xc604 <VolumeManager::set(LSMessage&)+0x4c4> literal=0x14f8c
    c3fa: 462b         	mov	r3, r5
    c3fc: 2203         	movs	r2, #0x3
    c3fe: a806         	add	r0, sp, #0x18
    c400: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    c402: 311c         	adds	r1, #0x1c
    c404: f7fd fc22    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x27bc
    c408: e7d5         	b	0xc3b6 <VolumeManager::set(LSMessage&)+0x276> @ imm = #-0x56
    c40a: 2200         	movs	r2, #0x0
    c40c: a90d         	add	r1, sp, #0x34
    c40e: 4630         	mov	r0, r6
    c410: f7f7 ee30    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x83a0
    c414: 9b0d         	ldr	r3, [sp, #0x34]
    c416: 901c         	str	r0, [sp, #0x70]
    c418: 931e         	str	r3, [sp, #0x78]
    c41a: 462a         	mov	r2, r5
    c41c: 4659         	mov	r1, r11
    c41e: f7f7 edb4    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x8498
    c422: 9d0d         	ldr	r5, [sp, #0x34]
    c424: 9b1c         	ldr	r3, [sp, #0x70]
    c426: e6d6         	b	0xc1d6 <VolumeManager::set(LSMessage&)+0x96> @ imm = #-0x254
    c428: 4977         	ldr	r1, [pc, #0x1dc]        @ 0xc608 <VolumeManager::set(LSMessage&)+0x4c8> literal=0x14f58
    c42a: 2300         	movs	r3, #0x0
    c42c: 22ca         	movs	r2, #0xca
    c42e: a806         	add	r0, sp, #0x18
    c430: f10d 0b40    	add.w	r11, sp, #0x40
    c434: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    c436: 31ac         	adds	r1, #0xac
    c438: f7fd fc08    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x27f0
    c43c: e7b8         	b	0xc3b0 <VolumeManager::set(LSMessage&)+0x270> @ imm = #-0x90
    c43e: 9810         	ldr	r0, [sp, #0x40]
    c440: f8d8 1008    	ldr.w	r1, [r8, #0x8]
    c444: b122         	cbz	r2, 0xc450 <VolumeManager::set(LSMessage&)+0x310> @ imm = #0x8
    c446: f7f7 ee84    	blx	0x4150 <memcmp@plt>     @ imm = #-0x82f8
    c44a: 2800         	cmp	r0, #0x0
    c44c: f47f af42    	bne.w	0xc2d4 <VolumeManager::set(LSMessage&)+0x194> @ imm = #-0x17c
    c450: 9b02         	ldr	r3, [sp, #0x8]
    c452: f888 3021    	strb.w	r3, [r8, #0x21]
    c456: e73d         	b	0xc2d4 <VolumeManager::set(LSMessage&)+0x194> @ imm = #-0x186
    c458: b132         	cbz	r2, 0xc468 <VolumeManager::set(LSMessage&)+0x328> @ imm = #0xc
    c45a: 2a01         	cmp	r2, #0x1
    c45c: d06f         	beq	0xc53e <VolumeManager::set(LSMessage&)+0x3fe> @ imm = #0xde
    c45e: 4621         	mov	r1, r4
    c460: f7f7 ed92    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x84dc
    c464: 9a1d         	ldr	r2, [sp, #0x74]
    c466: 9810         	ldr	r0, [sp, #0x40]
    c468: 2300         	movs	r3, #0x0
    c46a: 9211         	str	r2, [sp, #0x44]
    c46c: 5483         	strb	r3, [r0, r2]
    c46e: 981c         	ldr	r0, [sp, #0x70]
    c470: e6ee         	b	0xc250 <VolumeManager::set(LSMessage&)+0x110> @ imm = #-0x224
    c472: 2100         	movs	r1, #0x0
    c474: 4628         	mov	r0, r5
    c476: f7f7 ecbe    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x8684
    c47a: 4a64         	ldr	r2, [pc, #0x190]        @ 0xc60c <VolumeManager::set(LSMessage&)+0x4cc> literal=0x1298
    c47c: 4623         	mov	r3, r4
    c47e: 941c         	str	r4, [sp, #0x70]
    c480: f04f 0e0b    	mov.w	lr, #0xb
    c484: f04f 0c00    	mov.w	r12, #0x0
    c488: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    c48a: ca07         	ldm	r2, {r0, r1, r2}
    c48c: c303         	stm	r3!, {r0, r1}
    c48e: 4631         	mov	r1, r6
    c490: f823 2b02    	strh	r2, [r3], #2
    c494: 0c12         	lsrs	r2, r2, #0x10
    c496: 4650         	mov	r0, r10
    c498: 701a         	strb	r2, [r3]
    c49a: 462a         	mov	r2, r5
    c49c: f8cd e074    	str.w	lr, [sp, #0x74]
    c4a0: f88d c083    	strb.w	r12, [sp, #0x83]
    c4a4: f7f7 ec16    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x87d4
    c4a8: 981c         	ldr	r0, [sp, #0x70]
    c4aa: 42a0         	cmp	r0, r4
    c4ac: d003         	beq	0xc4b6 <VolumeManager::set(LSMessage&)+0x376> @ imm = #0x6
    c4ae: 991e         	ldr	r1, [sp, #0x78]
    c4b0: 3101         	adds	r1, #0x1
    c4b2: f7f7 ed26    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x85b4
    c4b6: 4628         	mov	r0, r5
    c4b8: f7f7 ec4e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8764
    c4bc: 4954         	ldr	r1, [pc, #0x150]        @ 0xc610 <VolumeManager::set(LSMessage&)+0x4d0> literal=0x14ecc
    c4be: 4628         	mov	r0, r5
    c4c0: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    c4c2: 3164         	adds	r1, #0x64
    c4c4: f7f7 ee32    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x839c
    c4c8: 4a52         	ldr	r2, [pc, #0x148]        @ 0xc614 <VolumeManager::set(LSMessage&)+0x4d4> literal=0x2182
    c4ca: 4623         	mov	r3, r4
    c4cc: 941c         	str	r4, [sp, #0x70]
    c4ce: f04f 0e09    	mov.w	lr, #0x9
    c4d2: f04f 0c00    	mov.w	r12, #0x0
    c4d6: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    c4d8: ca07         	ldm	r2, {r0, r1, r2}
    c4da: c303         	stm	r3!, {r0, r1}
    c4dc: 4631         	mov	r1, r6
    c4de: 701a         	strb	r2, [r3]
    c4e0: 4650         	mov	r0, r10
    c4e2: 462a         	mov	r2, r5
    c4e4: f8cd e074    	str.w	lr, [sp, #0x74]
    c4e8: f88d c081    	strb.w	r12, [sp, #0x81]
    c4ec: f7f7 ebf2    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x881c
    c4f0: 981c         	ldr	r0, [sp, #0x70]
    c4f2: 42a0         	cmp	r0, r4
    c4f4: d003         	beq	0xc4fe <VolumeManager::set(LSMessage&)+0x3be> @ imm = #0x6
    c4f6: 991e         	ldr	r1, [sp, #0x78]
    c4f8: 3101         	adds	r1, #0x1
    c4fa: f7f7 ed02    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x85fc
    c4fe: 4628         	mov	r0, r5
    c500: f7f7 ec2a    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x87ac
    c504: 2114         	movs	r1, #0x14
    c506: 4628         	mov	r0, r5
    c508: f7f7 ee16    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x83d4
    c50c: 4a42         	ldr	r2, [pc, #0x108]        @ 0xc618 <VolumeManager::set(LSMessage&)+0x4d8> literal=0x214a
    c50e: 4623         	mov	r3, r4
    c510: 941c         	str	r4, [sp, #0x70]
    c512: f04f 0e09    	mov.w	lr, #0x9
    c516: f04f 0c00    	mov.w	r12, #0x0
    c51a: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    c51c: ca07         	ldm	r2, {r0, r1, r2}
    c51e: c303         	stm	r3!, {r0, r1}
    c520: 4631         	mov	r1, r6
    c522: 701a         	strb	r2, [r3]
    c524: 4650         	mov	r0, r10
    c526: 462a         	mov	r2, r5
    c528: f8cd e074    	str.w	lr, [sp, #0x74]
    c52c: f88d c081    	strb.w	r12, [sp, #0x81]
    c530: f7f7 ebd0    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8860
    c534: e72e         	b	0xc394 <VolumeManager::set(LSMessage&)+0x254> @ imm = #-0x1a4
    c536: 941c         	str	r4, [sp, #0x70]
    c538: ac1e         	add	r4, sp, #0x78
    c53a: 4620         	mov	r0, r4
    c53c: e688         	b	0xc250 <VolumeManager::set(LSMessage&)+0x110> @ imm = #-0x2f0
    c53e: f89d 3078    	ldrb.w	r3, [sp, #0x78]
    c542: 7003         	strb	r3, [r0]
    c544: 9a1d         	ldr	r2, [sp, #0x74]
    c546: 9810         	ldr	r0, [sp, #0x40]
    c548: e78e         	b	0xc468 <VolumeManager::set(LSMessage&)+0x328> @ imm = #-0xe4
    c54a: 4834         	ldr	r0, [pc, #0xd0]         @ 0xc61c <VolumeManager::set(LSMessage&)+0x4dc> literal=0x1148
    c54c: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    c54e: f7f7 ebda    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x884c
    c552: 4630         	mov	r0, r6
    c554: f7f7 edcc    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8468
    c558: 4628         	mov	r0, r5
    c55a: f7f7 ebfe    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8804
    c55e: 4650         	mov	r0, r10
    c560: f7f7 ebfa    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x880c
    c564: 4648         	mov	r0, r9
    c566: f7f7 edc4    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8478
    c56a: 4638         	mov	r0, r7
    c56c: f7f7 ebf4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8818
    c570: 9806         	ldr	r0, [sp, #0x18]
    c572: b108         	cbz	r0, 0xc578 <VolumeManager::set(LSMessage&)+0x438> @ imm = #0x2
    c574: f7f7 ed6a    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x852c
    c578: 4658         	mov	r0, r11
    c57a: f7f7 edba    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x848c
    c57e: f7f7 ee00    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x8400
    c582: e7e6         	b	0xc552 <VolumeManager::set(LSMessage&)+0x412> @ imm = #-0x34
    c584: e7e5         	b	0xc552 <VolumeManager::set(LSMessage&)+0x412> @ imm = #-0x36
    c586: e7e4         	b	0xc552 <VolumeManager::set(LSMessage&)+0x412> @ imm = #-0x38
    c588: f10a 0308    	add.w	r3, r10, #0x8
    c58c: 4630         	mov	r0, r6
    c58e: f10d 0b40    	add.w	r11, sp, #0x40
    c592: 931c         	str	r3, [sp, #0x70]
    c594: f7f7 ecda    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x864c
    c598: e7e4         	b	0xc564 <VolumeManager::set(LSMessage&)+0x424> @ imm = #-0x38
    c59a: e7e0         	b	0xc55e <VolumeManager::set(LSMessage&)+0x41e> @ imm = #-0x40
    c59c: f10d 0b40    	add.w	r11, sp, #0x40
    c5a0: e7e3         	b	0xc56a <VolumeManager::set(LSMessage&)+0x42a> @ imm = #-0x3a
    c5a2: 9806         	ldr	r0, [sp, #0x18]
    c5a4: f10d 0b40    	add.w	r11, sp, #0x40
    c5a8: e7e4         	b	0xc574 <VolumeManager::set(LSMessage&)+0x434> @ imm = #-0x38
    c5aa: 4630         	mov	r0, r6
    c5ac: f10d 0b40    	add.w	r11, sp, #0x40
    c5b0: f7f7 ed9e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x84c4
    c5b4: e7d6         	b	0xc564 <VolumeManager::set(LSMessage&)+0x424> @ imm = #-0x54
    c5b6: 4630         	mov	r0, r6
    c5b8: f10d 0b40    	add.w	r11, sp, #0x40
    c5bc: f7f7 ed98    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x84d0
    c5c0: 4b0b         	ldr	r3, [pc, #0x2c]         @ 0xc5f0 <VolumeManager::set(LSMessage&)+0x4b0> literal=0x210
    c5c2: 4628         	mov	r0, r5
    c5c4: f85a 3003    	ldr.w	r3, [r10, r3]
    c5c8: 3308         	adds	r3, #0x8
    c5ca: 930d         	str	r3, [sp, #0x34]
    c5cc: f7f7 ecbe    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x8684
    c5d0: e7c8         	b	0xc564 <VolumeManager::set(LSMessage&)+0x424> @ imm = #-0x70
    c5d2: f10d 0b40    	add.w	r11, sp, #0x40
    c5d6: e7cf         	b	0xc578 <VolumeManager::set(LSMessage&)+0x438> @ imm = #-0x62
    c5d8: f10d 0b40    	add.w	r11, sp, #0x40
    c5dc: e7c2         	b	0xc564 <VolumeManager::set(LSMessage&)+0x424> @ imm = #-0x7c
    c5de: e7b8         	b	0xc552 <VolumeManager::set(LSMessage&)+0x412> @ imm = #-0x90
    c5e0: e7b7         	b	0xc552 <VolumeManager::set(LSMessage&)+0x412> @ imm = #-0x92
    c5e2: bf00         	nop
    c5e4: 4c68         	ldr	r4, [pc, #0x1a0]        @ 0xc788 <VolumeManager::up(LSMessage&)+0x168> literal=0xd0034298
    c5e6: 0001         	movs	r1, r0
    c5e8: 2ec0         	cmp	r6, #0xc0
    c5ea: 0000         	movs	r0, r0
    c5ec: 2df6         	cmp	r5, #0xf6
    c5ee: 0000         	movs	r0, r0
    c5f0: 0210         	lsls	r0, r2, #0x8
    c5f2: 0000         	movs	r0, r0
    c5f4: 2064         	movs	r0, #0x64
    c5f6: 0000         	movs	r0, r0
    c5f8: 1436         	asrs	r6, r6, #0x10
    c5fa: 0000         	movs	r0, r0
    c5fc: 2cc8         	cmp	r4, #0xc8
    c5fe: 0000         	movs	r0, r0
    c600: 1f62         	subs	r2, r4, #0x5
    c602: 0000         	movs	r0, r0
    c604: 4f8c         	ldr	r7, [pc, #0x230]        @ 0xc838 <VolumeManager::up(LSMessage&)+0x218> literal=0xc0bf04f
    c606: 0001         	movs	r1, r0
    c608: 4f58         	ldr	r7, [pc, #0x160]        @ 0xc76c <VolumeManager::up(LSMessage&)+0x14c> literal=0x22ca2300
    c60a: 0001         	movs	r1, r0
    c60c: 1298         	asrs	r0, r3, #0xa
    c60e: 0000         	movs	r0, r0
    c610: 4ecc         	ldr	r6, [pc, #0x330]        @ 0xc944 <VolumeManager::up(LSMessage&)+0x324> literal=0x4620eade
    c612: 0001         	movs	r1, r0
    c614: 2182         	movs	r1, #0x82
    c616: 0000         	movs	r0, r0
    c618: 214a         	movs	r1, #0x4a
    c61a: 0000         	movs	r0, r0
    c61c: 1148         	asrs	r0, r1, #0x5
    c61e: 0000         	movs	r0, r0

0000c620 <VolumeManager::up(LSMessage&)>:
    c620: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    c624: b0a3         	sub	sp, #0x8c
    c626: f8df b468    	ldr.w	r11, [pc, #0x468]       @ 0xca90 <VolumeManager::up(LSMessage&)+0x470> literal=0x14788
    c62a: 2300         	movs	r3, #0x0
    c62c: aa12         	add	r2, sp, #0x48
    c62e: 4606         	mov	r6, r0
    c630: 4608         	mov	r0, r1
    c632: 460c         	mov	r4, r1
    c634: 44fb         	add	r11, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    c636: 9201         	str	r2, [sp, #0x4]
    c638: e9cd 2310    	strd	r2, r3, [sp, #64]
    c63c: f88d 3048    	strb.w	r3, [sp, #0x48]
    c640: e9cd 3105    	strd	r3, r1, [sp, #20]
    c644: f7f7 ed4e    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x8564
    c648: f10d 091c    	add.w	r9, sp, #0x1c
    c64c: 4648         	mov	r0, r9
    c64e: f7f7 ede6    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x8434
    c652: ab18         	add	r3, sp, #0x60
    c654: f10d 0a58    	add.w	r10, sp, #0x58
    c658: af1c         	add	r7, sp, #0x70
    c65a: 9302         	str	r3, [sp, #0x8]
    c65c: 9316         	str	r3, [sp, #0x58]
    c65e: 2200         	movs	r2, #0x0
    c660: 2378         	movs	r3, #0x78
    c662: 4639         	mov	r1, r7
    c664: 4650         	mov	r0, r10
    c666: 931c         	str	r3, [sp, #0x70]
    c668: f7f7 ed04    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x85f8
    c66c: f8df 1424    	ldr.w	r1, [pc, #0x424]        @ 0xca94 <VolumeManager::up(LSMessage&)+0x474> literal=0x2a7e
    c670: 2278         	movs	r2, #0x78
    c672: 9b1c         	ldr	r3, [sp, #0x70]
    c674: 9016         	str	r0, [sp, #0x58]
    c676: 4479         	add	r1, pc => raw=0xf0f8 ghidra=0x1f0f8 file=61688 "{\"type\":\"object\",\"properties\":{\"soundOutput\":{\"type\":\"string\"}},\"required\":[\"soundOutput\"],\"additionalProperties\":false}"
    c678: 9318         	str	r3, [sp, #0x60]
    c67a: f7f7 ec86    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x86f4
    c67e: 9b1c         	ldr	r3, [sp, #0x70]
    c680: 2100         	movs	r1, #0x0
    c682: 9a16         	ldr	r2, [sp, #0x58]
    c684: 4620         	mov	r0, r4
    c686: 9317         	str	r3, [sp, #0x5c]
    c688: 54d1         	strb	r1, [r2, r3]
    c68a: f7f7 ed86    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x84f4
    c68e: ad1e         	add	r5, sp, #0x78
    c690: 4680         	mov	r8, r0
    c692: 951c         	str	r5, [sp, #0x70]
    c694: 2800         	cmp	r0, #0x0
    c696: f000 81b7    	beq.w	0xca08 <VolumeManager::up(LSMessage&)+0x3e8> @ imm = #0x36e
    c69a: f7f7 ed04    	blx	0x40a4 <strlen@plt>     @ imm = #-0x85f8
    c69e: 280f         	cmp	r0, #0xf
    c6a0: 4604         	mov	r4, r0
    c6a2: 900d         	str	r0, [sp, #0x34]
    c6a4: f200 8098    	bhi.w	0xc7d8 <VolumeManager::up(LSMessage&)+0x1b8> @ imm = #0x130
    c6a8: 2801         	cmp	r0, #0x1
    c6aa: f040 8086    	bne.w	0xc7ba <VolumeManager::up(LSMessage&)+0x19a> @ imm = #0x10c
    c6ae: f898 3000    	ldrb.w	r3, [r8]
    c6b2: f88d 3078    	strb.w	r3, [sp, #0x78]
    c6b6: 462b         	mov	r3, r5
    c6b8: 2200         	movs	r2, #0x0
    c6ba: 941d         	str	r4, [sp, #0x74]
    c6bc: 4649         	mov	r1, r9
    c6be: 551a         	strb	r2, [r3, r4]
    c6c0: 4638         	mov	r0, r7
    c6c2: ab05         	add	r3, sp, #0x14
    c6c4: 4652         	mov	r2, r10
    c6c6: f7fd fe87    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #-0x22f2
    c6ca: 4604         	mov	r4, r0
    c6cc: 981c         	ldr	r0, [sp, #0x70]
    c6ce: 42a8         	cmp	r0, r5
    c6d0: d003         	beq	0xc6da <VolumeManager::up(LSMessage&)+0xba> @ imm = #0x6
    c6d2: 991e         	ldr	r1, [sp, #0x78]
    c6d4: 3101         	adds	r1, #0x1
    c6d6: f7f7 ec14    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x87d8
    c6da: 2c00         	cmp	r4, #0x0
    c6dc: d073         	beq	0xc7c6 <VolumeManager::up(LSMessage&)+0x1a6> @ imm = #0xe6
    c6de: 4aee         	ldr	r2, [pc, #0x3b8]        @ 0xca98 <VolumeManager::up(LSMessage&)+0x478> literal=0x2916
    c6e0: ac0d         	add	r4, sp, #0x34
    c6e2: 4649         	mov	r1, r9
    c6e4: 4620         	mov	r0, r4
    c6e6: 447a         	add	r2, pc => raw=0xf000 ghidra=0x1f000 file=61440 "soundOutput"
    c6e8: f7f7 ec10    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x87e0
    c6ec: 2300         	movs	r3, #0x0
    c6ee: 4639         	mov	r1, r7
    c6f0: 4620         	mov	r0, r4
    c6f2: 951c         	str	r5, [sp, #0x70]
    c6f4: 931d         	str	r3, [sp, #0x74]
    c6f6: f88d 3078    	strb.w	r3, [sp, #0x78]
    c6fa: f7f7 eb52    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x895c
    c6fe: 9b1c         	ldr	r3, [sp, #0x70]
    c700: 9810         	ldr	r0, [sp, #0x40]
    c702: 9a1d         	ldr	r2, [sp, #0x74]
    c704: 42ab         	cmp	r3, r5
    c706: f000 815c    	beq.w	0xc9c2 <VolumeManager::up(LSMessage&)+0x3a2> @ imm = #0x2b8
    c70a: 9901         	ldr	r1, [sp, #0x4]
    c70c: f8dd c078    	ldr.w	r12, [sp, #0x78]
    c710: e9cd 3210    	strd	r3, r2, [sp, #64]
    c714: 4288         	cmp	r0, r1
    c716: bf08         	it	eq
    c718: f8cd c048    	streq.w	r12, [sp, #0x48]
    c71c: f000 815e    	beq.w	0xc9dc <VolumeManager::up(LSMessage&)+0x3bc> @ imm = #0x2bc
    c720: 9b12         	ldr	r3, [sp, #0x48]
    c722: f8cd c048    	str.w	r12, [sp, #0x48]
    c726: 2800         	cmp	r0, #0x0
    c728: f000 8158    	beq.w	0xc9dc <VolumeManager::up(LSMessage&)+0x3bc> @ imm = #0x2b0
    c72c: 931e         	str	r3, [sp, #0x78]
    c72e: 901c         	str	r0, [sp, #0x70]
    c730: 2300         	movs	r3, #0x0
    c732: 931d         	str	r3, [sp, #0x74]
    c734: 7003         	strb	r3, [r0]
    c736: 981c         	ldr	r0, [sp, #0x70]
    c738: 42a8         	cmp	r0, r5
    c73a: d003         	beq	0xc744 <VolumeManager::up(LSMessage&)+0x124> @ imm = #0x6
    c73c: 991e         	ldr	r1, [sp, #0x78]
    c73e: 3101         	adds	r1, #0x1
    c740: f7f7 ebde    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8844
    c744: 4bd5         	ldr	r3, [pc, #0x354]        @ 0xca9c <VolumeManager::up(LSMessage&)+0x47c> literal=0x210
    c746: 4620         	mov	r0, r4
    c748: f10d 0828    	add.w	r8, sp, #0x28
    c74c: f85b 3003    	ldr.w	r3, [r11, r3]
    c750: 3308         	adds	r3, #0x8
    c752: 930d         	str	r3, [sp, #0x34]
    c754: f7f7 ebfa    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x880c
    c758: 4640         	mov	r0, r8
    c75a: f7f7 eb2e    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x89a4
    c75e: 9a11         	ldr	r2, [sp, #0x44]
    c760: 68f3         	ldr	r3, [r6, #0xc]
    c762: 9810         	ldr	r0, [sp, #0x40]
    c764: 68b1         	ldr	r1, [r6, #0x8]
    c766: 429a         	cmp	r2, r3
    c768: d045         	beq	0xc7f6 <VolumeManager::up(LSMessage&)+0x1d6> @ imm = #0x8a
    c76a: 49cd         	ldr	r1, [pc, #0x334]        @ 0xcaa0 <VolumeManager::up(LSMessage&)+0x480> literal=0x14c16
    c76c: 2300         	movs	r3, #0x0
    c76e: 22ca         	movs	r2, #0xca
    c770: a806         	add	r0, sp, #0x18
    c772: f10d 0b40    	add.w	r11, sp, #0x40
    c776: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    c778: 31c4         	adds	r1, #0xc4
    c77a: f7fd fa67    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x2b32
    c77e: 4640         	mov	r0, r8
    c780: f7f7 eaea    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8a2c
    c784: 9816         	ldr	r0, [sp, #0x58]
    c786: 9b02         	ldr	r3, [sp, #0x8]
    c788: 4298         	cmp	r0, r3
    c78a: d003         	beq	0xc794 <VolumeManager::up(LSMessage&)+0x174> @ imm = #0x6
    c78c: 9918         	ldr	r1, [sp, #0x60]
    c78e: 3101         	adds	r1, #0x1
    c790: f7f7 ebb6    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8894
    c794: 4648         	mov	r0, r9
    c796: f7f7 eae0    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8a40
    c79a: 9806         	ldr	r0, [sp, #0x18]
    c79c: b108         	cbz	r0, 0xc7a2 <VolumeManager::up(LSMessage&)+0x182> @ imm = #0x2
    c79e: f7f7 ec56    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x8754
    c7a2: 9810         	ldr	r0, [sp, #0x40]
    c7a4: 9b01         	ldr	r3, [sp, #0x4]
    c7a6: 4298         	cmp	r0, r3
    c7a8: d003         	beq	0xc7b2 <VolumeManager::up(LSMessage&)+0x192> @ imm = #0x6
    c7aa: 9912         	ldr	r1, [sp, #0x48]
    c7ac: 3101         	adds	r1, #0x1
    c7ae: f7f7 eba8    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x88b0
    c7b2: 2001         	movs	r0, #0x1
    c7b4: b023         	add	sp, #0x8c
    c7b6: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    c7ba: 462b         	mov	r3, r5
    c7bc: 2800         	cmp	r0, #0x0
    c7be: f43f af7b    	beq.w	0xc6b8 <VolumeManager::up(LSMessage&)+0x98> @ imm = #-0x10a
    c7c2: 4628         	mov	r0, r5
    c7c4: e010         	b	0xc7e8 <VolumeManager::up(LSMessage&)+0x1c8> @ imm = #0x20
    c7c6: 49b7         	ldr	r1, [pc, #0x2dc]        @ 0xcaa4 <VolumeManager::up(LSMessage&)+0x484> literal=0x14bbe
    c7c8: 4623         	mov	r3, r4
    c7ca: 2203         	movs	r2, #0x3
    c7cc: a806         	add	r0, sp, #0x18
    c7ce: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    c7d0: 311c         	adds	r1, #0x1c
    c7d2: f7fd fa3b    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x2b8a
    c7d6: e7d5         	b	0xc784 <VolumeManager::up(LSMessage&)+0x164> @ imm = #-0x56
    c7d8: 2200         	movs	r2, #0x0
    c7da: a90d         	add	r1, sp, #0x34
    c7dc: 4638         	mov	r0, r7
    c7de: f7f7 ec4a    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x876c
    c7e2: 9b0d         	ldr	r3, [sp, #0x34]
    c7e4: 901c         	str	r0, [sp, #0x70]
    c7e6: 931e         	str	r3, [sp, #0x78]
    c7e8: 4622         	mov	r2, r4
    c7ea: 4641         	mov	r1, r8
    c7ec: f7f7 ebcc    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x8868
    c7f0: 9c0d         	ldr	r4, [sp, #0x34]
    c7f2: 9b1c         	ldr	r3, [sp, #0x70]
    c7f4: e760         	b	0xc6b8 <VolumeManager::up(LSMessage&)+0x98> @ imm = #-0x140
    c7f6: b11a         	cbz	r2, 0xc800 <VolumeManager::up(LSMessage&)+0x1e0> @ imm = #0x6
    c7f8: f7f7 ecaa    	blx	0x4150 <memcmp@plt>     @ imm = #-0x86ac
    c7fc: 2800         	cmp	r0, #0x0
    c7fe: d1b4         	bne	0xc76a <VolumeManager::up(LSMessage&)+0x14a> @ imm = #-0x98
    c800: f896 3021    	ldrb.w	r3, [r6, #0x21]
    c804: 2b64         	cmp	r3, #0x64
    c806: d0b0         	beq	0xc76a <VolumeManager::up(LSMessage&)+0x14a> @ imm = #-0xa0
    c808: f10d 0b40    	add.w	r11, sp, #0x40
    c80c: 3301         	adds	r3, #0x1
    c80e: 463a         	mov	r2, r7
    c810: 4659         	mov	r1, r11
    c812: 4630         	mov	r0, r6
    c814: 9303         	str	r3, [sp, #0xc]
    c816: 931c         	str	r3, [sp, #0x70]
    c818: f7ff fa3c    	bl	0xbc94 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)> @ imm = #-0xb88
    c81c: 2800         	cmp	r0, #0x0
    c81e: d171         	bne	0xc904 <VolumeManager::up(LSMessage&)+0x2e4> @ imm = #0xe2
    c820: 9a11         	ldr	r2, [sp, #0x44]
    c822: 68f3         	ldr	r3, [r6, #0xc]
    c824: 429a         	cmp	r2, r3
    c826: f000 80dd    	beq.w	0xc9e4 <VolumeManager::up(LSMessage&)+0x3c4> @ imm = #0x1ba
    c82a: 2101         	movs	r1, #0x1
    c82c: 4620         	mov	r0, r4
    c82e: f7f7 eae2    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x8a3c
    c832: 4a9d         	ldr	r2, [pc, #0x274]        @ 0xcaa8 <VolumeManager::up(LSMessage&)+0x488> literal=0xee2
    c834: 462b         	mov	r3, r5
    c836: 951c         	str	r5, [sp, #0x70]
    c838: f04f 0c0b    	mov.w	r12, #0xb
    c83c: 2600         	movs	r6, #0x0
    c83e: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    c840: ca07         	ldm	r2, {r0, r1, r2}
    c842: c303         	stm	r3!, {r0, r1}
    c844: 4639         	mov	r1, r7
    c846: f823 2b02    	strh	r2, [r3], #2
    c84a: 0c12         	lsrs	r2, r2, #0x10
    c84c: 4640         	mov	r0, r8
    c84e: 701a         	strb	r2, [r3]
    c850: 4622         	mov	r2, r4
    c852: f8cd c074    	str.w	r12, [sp, #0x74]
    c856: f88d 6083    	strb.w	r6, [sp, #0x83]
    c85a: f7f7 ea3c    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8b88
    c85e: 981c         	ldr	r0, [sp, #0x70]
    c860: 42a8         	cmp	r0, r5
    c862: d003         	beq	0xc86c <VolumeManager::up(LSMessage&)+0x24c> @ imm = #0x6
    c864: 991e         	ldr	r1, [sp, #0x78]
    c866: 3101         	adds	r1, #0x1
    c868: f7f7 eb4a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x896c
    c86c: 4620         	mov	r0, r4
    c86e: f7f7 ea74    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8b18
    c872: 4659         	mov	r1, r11
    c874: 4620         	mov	r0, r4
    c876: f7f7 ec5a    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x874c
    c87a: 4a8c         	ldr	r2, [pc, #0x230]        @ 0xcaac <VolumeManager::up(LSMessage&)+0x48c> literal=0x2776
    c87c: 462b         	mov	r3, r5
    c87e: 951c         	str	r5, [sp, #0x70]
    c880: f04f 0c0b    	mov.w	r12, #0xb
    c884: 2600         	movs	r6, #0x0
    c886: 447a         	add	r2, pc => raw=0xf000 ghidra=0x1f000 file=61440 "soundOutput"
    c888: ca07         	ldm	r2, {r0, r1, r2}
    c88a: c303         	stm	r3!, {r0, r1}
    c88c: 4639         	mov	r1, r7
    c88e: f823 2b02    	strh	r2, [r3], #2
    c892: 0c12         	lsrs	r2, r2, #0x10
    c894: 4640         	mov	r0, r8
    c896: 701a         	strb	r2, [r3]
    c898: 4622         	mov	r2, r4
    c89a: f8cd c074    	str.w	r12, [sp, #0x74]
    c89e: f88d 6083    	strb.w	r6, [sp, #0x83]
    c8a2: f7f7 ea18    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8bd0
    c8a6: 981c         	ldr	r0, [sp, #0x70]
    c8a8: 42a8         	cmp	r0, r5
    c8aa: d003         	beq	0xc8b4 <VolumeManager::up(LSMessage&)+0x294> @ imm = #0x6
    c8ac: 991e         	ldr	r1, [sp, #0x78]
    c8ae: 3101         	adds	r1, #0x1
    c8b0: f7f7 eb26    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x89b4
    c8b4: 4620         	mov	r0, r4
    c8b6: f7f7 ea50    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8b60
    c8ba: 9903         	ldr	r1, [sp, #0xc]
    c8bc: 4620         	mov	r0, r4
    c8be: f7f7 ec3c    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x8788
    c8c2: 4b7b         	ldr	r3, [pc, #0x1ec]        @ 0xcab0 <VolumeManager::up(LSMessage&)+0x490> literal=0x1a10
    c8c4: 2206         	movs	r2, #0x6
    c8c6: 921d         	str	r2, [sp, #0x74]
    c8c8: 2200         	movs	r2, #0x0
    c8ca: f88d 207e    	strb.w	r2, [sp, #0x7e]
    c8ce: 4622         	mov	r2, r4
    c8d0: 447b         	add	r3, pc => raw=0xe2e4 ghidra=0x1e2e4 file=58084 "volume"
    c8d2: 951c         	str	r5, [sp, #0x70]
    c8d4: e893 0003    	ldm.w	r3, {r0, r1}
    c8d8: 901e         	str	r0, [sp, #0x78]
    c8da: 4640         	mov	r0, r8
    c8dc: f8ad 107c    	strh.w	r1, [sp, #0x7c]
    c8e0: 4639         	mov	r1, r7
    c8e2: f7f7 e9f8    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8c10
    c8e6: 981c         	ldr	r0, [sp, #0x70]
    c8e8: 42a8         	cmp	r0, r5
    c8ea: d003         	beq	0xc8f4 <VolumeManager::up(LSMessage&)+0x2d4> @ imm = #0x6
    c8ec: 991e         	ldr	r1, [sp, #0x78]
    c8ee: 3101         	adds	r1, #0x1
    c8f0: f7f7 eb06    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x89f4
    c8f4: 4620         	mov	r0, r4
    c8f6: f7f7 ea30    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8ba0
    c8fa: 4641         	mov	r1, r8
    c8fc: a806         	add	r0, sp, #0x18
    c8fe: f7fd fac7    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #-0x2a72
    c902: e73c         	b	0xc77e <VolumeManager::up(LSMessage&)+0x15e> @ imm = #-0x188
    c904: 2100         	movs	r1, #0x0
    c906: 4620         	mov	r0, r4
    c908: f7f7 ea74    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x8b18
    c90c: 4a69         	ldr	r2, [pc, #0x1a4]        @ 0xcab4 <VolumeManager::up(LSMessage&)+0x494> literal=0xe08
    c90e: 462b         	mov	r3, r5
    c910: 951c         	str	r5, [sp, #0x70]
    c912: f04f 0c0b    	mov.w	r12, #0xb
    c916: 2600         	movs	r6, #0x0
    c918: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    c91a: ca07         	ldm	r2, {r0, r1, r2}
    c91c: c303         	stm	r3!, {r0, r1}
    c91e: 4639         	mov	r1, r7
    c920: f823 2b02    	strh	r2, [r3], #2
    c924: 0c12         	lsrs	r2, r2, #0x10
    c926: 4640         	mov	r0, r8
    c928: 701a         	strb	r2, [r3]
    c92a: 4622         	mov	r2, r4
    c92c: f8cd c074    	str.w	r12, [sp, #0x74]
    c930: f88d 6083    	strb.w	r6, [sp, #0x83]
    c934: f7f7 e9ce    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8c64
    c938: 981c         	ldr	r0, [sp, #0x70]
    c93a: 42a8         	cmp	r0, r5
    c93c: d003         	beq	0xc946 <VolumeManager::up(LSMessage&)+0x326> @ imm = #0x6
    c93e: 991e         	ldr	r1, [sp, #0x78]
    c940: 3101         	adds	r1, #0x1
    c942: f7f7 eade    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8a44
    c946: 4620         	mov	r0, r4
    c948: f7f7 ea06    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8bf4
    c94c: 495a         	ldr	r1, [pc, #0x168]        @ 0xcab8 <VolumeManager::up(LSMessage&)+0x498> literal=0x14a3c
    c94e: 4620         	mov	r0, r4
    c950: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    c952: 3164         	adds	r1, #0x64
    c954: f7f7 ebea    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x882c
    c958: 4a58         	ldr	r2, [pc, #0x160]        @ 0xcabc <VolumeManager::up(LSMessage&)+0x49c> literal=0x1cf4
    c95a: 462b         	mov	r3, r5
    c95c: 951c         	str	r5, [sp, #0x70]
    c95e: f04f 0c09    	mov.w	r12, #0x9
    c962: 2600         	movs	r6, #0x0
    c964: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    c966: ca07         	ldm	r2, {r0, r1, r2}
    c968: c303         	stm	r3!, {r0, r1}
    c96a: 4639         	mov	r1, r7
    c96c: 701a         	strb	r2, [r3]
    c96e: 4640         	mov	r0, r8
    c970: 4622         	mov	r2, r4
    c972: f8cd c074    	str.w	r12, [sp, #0x74]
    c976: f88d 6081    	strb.w	r6, [sp, #0x81]
    c97a: f7f7 e9ac    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8ca8
    c97e: 981c         	ldr	r0, [sp, #0x70]
    c980: 42a8         	cmp	r0, r5
    c982: d003         	beq	0xc98c <VolumeManager::up(LSMessage&)+0x36c> @ imm = #0x6
    c984: 991e         	ldr	r1, [sp, #0x78]
    c986: 3101         	adds	r1, #0x1
    c988: f7f7 eaba    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8a8c
    c98c: 4620         	mov	r0, r4
    c98e: f7f7 e9e4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8c38
    c992: 2114         	movs	r1, #0x14
    c994: 4620         	mov	r0, r4
    c996: f7f7 ebd0    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x8860
    c99a: 4a49         	ldr	r2, [pc, #0x124]        @ 0xcac0 <VolumeManager::up(LSMessage&)+0x4a0> literal=0x1cbe
    c99c: 462b         	mov	r3, r5
    c99e: 951c         	str	r5, [sp, #0x70]
    c9a0: f04f 0c09    	mov.w	r12, #0x9
    c9a4: 2600         	movs	r6, #0x0
    c9a6: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    c9a8: ca07         	ldm	r2, {r0, r1, r2}
    c9aa: c303         	stm	r3!, {r0, r1}
    c9ac: 4639         	mov	r1, r7
    c9ae: 701a         	strb	r2, [r3]
    c9b0: 4640         	mov	r0, r8
    c9b2: 4622         	mov	r2, r4
    c9b4: f8cd c074    	str.w	r12, [sp, #0x74]
    c9b8: f88d 6081    	strb.w	r6, [sp, #0x81]
    c9bc: f7f7 e98a    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8cec
    c9c0: e791         	b	0xc8e6 <VolumeManager::up(LSMessage&)+0x2c6> @ imm = #-0xde
    c9c2: b132         	cbz	r2, 0xc9d2 <VolumeManager::up(LSMessage&)+0x3b2> @ imm = #0xc
    c9c4: 2a01         	cmp	r2, #0x1
    c9c6: d019         	beq	0xc9fc <VolumeManager::up(LSMessage&)+0x3dc> @ imm = #0x32
    c9c8: 4629         	mov	r1, r5
    c9ca: f7f7 eade    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x8a44
    c9ce: 9a1d         	ldr	r2, [sp, #0x74]
    c9d0: 9810         	ldr	r0, [sp, #0x40]
    c9d2: 2300         	movs	r3, #0x0
    c9d4: 9211         	str	r2, [sp, #0x44]
    c9d6: 5483         	strb	r3, [r0, r2]
    c9d8: 981c         	ldr	r0, [sp, #0x70]
    c9da: e6a9         	b	0xc730 <VolumeManager::up(LSMessage&)+0x110> @ imm = #-0x2ae
    c9dc: 951c         	str	r5, [sp, #0x70]
    c9de: ad1e         	add	r5, sp, #0x78
    c9e0: 4628         	mov	r0, r5
    c9e2: e6a5         	b	0xc730 <VolumeManager::up(LSMessage&)+0x110> @ imm = #-0x2b6
    c9e4: 9810         	ldr	r0, [sp, #0x40]
    c9e6: 68b1         	ldr	r1, [r6, #0x8]
    c9e8: b122         	cbz	r2, 0xc9f4 <VolumeManager::up(LSMessage&)+0x3d4> @ imm = #0x8
    c9ea: f7f7 ebb2    	blx	0x4150 <memcmp@plt>     @ imm = #-0x889c
    c9ee: 2800         	cmp	r0, #0x0
    c9f0: f47f af1b    	bne.w	0xc82a <VolumeManager::up(LSMessage&)+0x20a> @ imm = #-0x1ca
    c9f4: 9b03         	ldr	r3, [sp, #0xc]
    c9f6: f886 3021    	strb.w	r3, [r6, #0x21]
    c9fa: e716         	b	0xc82a <VolumeManager::up(LSMessage&)+0x20a> @ imm = #-0x1d4
    c9fc: f89d 3078    	ldrb.w	r3, [sp, #0x78]
    ca00: 7003         	strb	r3, [r0]
    ca02: 9a1d         	ldr	r2, [sp, #0x74]
    ca04: 9810         	ldr	r0, [sp, #0x40]
    ca06: e7e4         	b	0xc9d2 <VolumeManager::up(LSMessage&)+0x3b2> @ imm = #-0x38
    ca08: 482e         	ldr	r0, [pc, #0xb8]         @ 0xcac4 <VolumeManager::up(LSMessage&)+0x4a4> literal=0xc8a
    ca0a: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    ca0c: f7f7 e97a    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x8d0c
    ca10: f10d 0b40    	add.w	r11, sp, #0x40
    ca14: 4658         	mov	r0, r11
    ca16: f7f7 eb6c    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8928
    ca1a: f7f7 ebb2    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x889c
    ca1e: 4638         	mov	r0, r7
    ca20: f7f7 eb66    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8934
    ca24: 4620         	mov	r0, r4
    ca26: f7f7 e998    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8cd0
    ca2a: 4640         	mov	r0, r8
    ca2c: f7f7 e994    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8cd8
    ca30: 4650         	mov	r0, r10
    ca32: f7f7 eb5e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8944
    ca36: 4648         	mov	r0, r9
    ca38: f7f7 e98e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8ce4
    ca3c: 9806         	ldr	r0, [sp, #0x18]
    ca3e: 2800         	cmp	r0, #0x0
    ca40: d0e8         	beq	0xca14 <VolumeManager::up(LSMessage&)+0x3f4> @ imm = #-0x30
    ca42: f7f7 eb04    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x89f8
    ca46: e7e5         	b	0xca14 <VolumeManager::up(LSMessage&)+0x3f4> @ imm = #-0x36
    ca48: f10d 0b40    	add.w	r11, sp, #0x40
    ca4c: e7f0         	b	0xca30 <VolumeManager::up(LSMessage&)+0x410> @ imm = #-0x20
    ca4e: e7e6         	b	0xca1e <VolumeManager::up(LSMessage&)+0x3fe> @ imm = #-0x34
    ca50: f10d 0b40    	add.w	r11, sp, #0x40
    ca54: e7ef         	b	0xca36 <VolumeManager::up(LSMessage&)+0x416> @ imm = #-0x22
    ca56: e7e2         	b	0xca1e <VolumeManager::up(LSMessage&)+0x3fe> @ imm = #-0x3c
    ca58: e7e1         	b	0xca1e <VolumeManager::up(LSMessage&)+0x3fe> @ imm = #-0x3e
    ca5a: e7e0         	b	0xca1e <VolumeManager::up(LSMessage&)+0x3fe> @ imm = #-0x40
    ca5c: e7df         	b	0xca1e <VolumeManager::up(LSMessage&)+0x3fe> @ imm = #-0x42
    ca5e: e7e4         	b	0xca2a <VolumeManager::up(LSMessage&)+0x40a> @ imm = #-0x38
    ca60: 4638         	mov	r0, r7
    ca62: f7f7 eb46    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8974
    ca66: 4b0d         	ldr	r3, [pc, #0x34]         @ 0xca9c <VolumeManager::up(LSMessage&)+0x47c> literal=0x210
    ca68: 4620         	mov	r0, r4
    ca6a: f85b 3003    	ldr.w	r3, [r11, r3]
    ca6e: f10d 0b40    	add.w	r11, sp, #0x40
    ca72: 3308         	adds	r3, #0x8
    ca74: 930d         	str	r3, [sp, #0x34]
    ca76: f7f7 ea6a    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x8b2c
    ca7a: e7d9         	b	0xca30 <VolumeManager::up(LSMessage&)+0x410> @ imm = #-0x4e
    ca7c: 9806         	ldr	r0, [sp, #0x18]
    ca7e: f10d 0b40    	add.w	r11, sp, #0x40
    ca82: e7de         	b	0xca42 <VolumeManager::up(LSMessage&)+0x422> @ imm = #-0x44
    ca84: 4638         	mov	r0, r7
    ca86: f10d 0b40    	add.w	r11, sp, #0x40
    ca8a: f7f7 eb32    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x899c
    ca8e: e7cf         	b	0xca30 <VolumeManager::up(LSMessage&)+0x410> @ imm = #-0x62
    ca90: 4788         	blx	r1
    ca92: 0001         	movs	r1, r0
    ca94: 2a7e         	cmp	r2, #0x7e
    ca96: 0000         	movs	r0, r0
    ca98: 2916         	cmp	r1, #0x16
    ca9a: 0000         	movs	r0, r0
    ca9c: 0210         	lsls	r0, r2, #0x8
    ca9e: 0000         	movs	r0, r0
    caa0: 4c16         	ldr	r4, [pc, #0x58]         @ 0xcafc <VolumeManager::down(LSMessage&)+0x34> literal=0x958f10d
    caa2: 0001         	movs	r1, r0
    caa4: 4bbe         	ldr	r3, [pc, #0x2f8]        @ 0xcda0 <VolumeManager::down(LSMessage&)+0x2d8> literal=0x2b009303
    caa6: 0001         	movs	r1, r0
    caa8: 0ee2         	lsrs	r2, r4, #0x1b
    caaa: 0000         	movs	r0, r0
    caac: 2776         	movs	r7, #0x76
    caae: 0000         	movs	r0, r0
    cab0: 1a10         	subs	r0, r2, r0
    cab2: 0000         	movs	r0, r0
    cab4: 0e08         	lsrs	r0, r1, #0x18
    cab6: 0000         	movs	r0, r0
    cab8: 4a3c         	ldr	r2, [pc, #0xf0]         @ 0xcbac <VolumeManager::down(LSMessage&)+0xe4> literal=0x42a39a1d
    caba: 0001         	movs	r1, r0
    cabc: 1cf4         	adds	r4, r6, #0x3
    cabe: 0000         	movs	r0, r0
    cac0: 1cbe         	adds	r6, r7, #0x2
    cac2: 0000         	movs	r0, r0
    cac4: 0c8a         	lsrs	r2, r1, #0x12
    cac6: 0000         	movs	r0, r0

0000cac8 <VolumeManager::down(LSMessage&)>:
    cac8: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    cacc: b0a3         	sub	sp, #0x8c
    cace: f8df a480    	ldr.w	r10, [pc, #0x480]       @ 0xcf50 <VolumeManager::down(LSMessage&)+0x488> literal=0x142e0
    cad2: 2300         	movs	r3, #0x0
    cad4: aa12         	add	r2, sp, #0x48
    cad6: 4607         	mov	r7, r0
    cad8: 4608         	mov	r0, r1
    cada: 460c         	mov	r4, r1
    cadc: 44fa         	add	r10, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    cade: 9200         	str	r2, [sp]
    cae0: e9cd 2310    	strd	r2, r3, [sp, #64]
    cae4: f88d 3048    	strb.w	r3, [sp, #0x48]
    cae8: e9cd 3105    	strd	r3, r1, [sp, #20]
    caec: f7f7 eafa    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x8a0c
    caf0: f10d 081c    	add.w	r8, sp, #0x1c
    caf4: 4640         	mov	r0, r8
    caf6: f7f7 eb92    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x88dc
    cafa: ab18         	add	r3, sp, #0x60
    cafc: f10d 0958    	add.w	r9, sp, #0x58
    cb00: ae1c         	add	r6, sp, #0x70
    cb02: 9301         	str	r3, [sp, #0x4]
    cb04: 9316         	str	r3, [sp, #0x58]
    cb06: 2200         	movs	r2, #0x0
    cb08: 2378         	movs	r3, #0x78
    cb0a: 4631         	mov	r1, r6
    cb0c: 4648         	mov	r0, r9
    cb0e: 931c         	str	r3, [sp, #0x70]
    cb10: f7f7 eab0    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x8aa0
    cb14: f8df 143c    	ldr.w	r1, [pc, #0x43c]        @ 0xcf54 <VolumeManager::down(LSMessage&)+0x48c> literal=0x25d6
    cb18: 2278         	movs	r2, #0x78
    cb1a: 9b1c         	ldr	r3, [sp, #0x70]
    cb1c: 9016         	str	r0, [sp, #0x58]
    cb1e: 4479         	add	r1, pc => raw=0xf0f8 ghidra=0x1f0f8 file=61688 "{\"type\":\"object\",\"properties\":{\"soundOutput\":{\"type\":\"string\"}},\"required\":[\"soundOutput\"],\"additionalProperties\":false}"
    cb20: 9318         	str	r3, [sp, #0x60]
    cb22: f7f7 ea32    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x8b9c
    cb26: 9b1c         	ldr	r3, [sp, #0x70]
    cb28: 2100         	movs	r1, #0x0
    cb2a: 9a16         	ldr	r2, [sp, #0x58]
    cb2c: 4620         	mov	r0, r4
    cb2e: 9317         	str	r3, [sp, #0x5c]
    cb30: 54d1         	strb	r1, [r2, r3]
    cb32: f7f7 eb32    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x899c
    cb36: ac1e         	add	r4, sp, #0x78
    cb38: 4683         	mov	r11, r0
    cb3a: 941c         	str	r4, [sp, #0x70]
    cb3c: 2800         	cmp	r0, #0x0
    cb3e: f000 81c3    	beq.w	0xcec8 <VolumeManager::down(LSMessage&)+0x400> @ imm = #0x386
    cb42: f7f7 eab0    	blx	0x40a4 <strlen@plt>     @ imm = #-0x8aa0
    cb46: 280f         	cmp	r0, #0xf
    cb48: 4605         	mov	r5, r0
    cb4a: 900d         	str	r0, [sp, #0x34]
    cb4c: f200 8111    	bhi.w	0xcd72 <VolumeManager::down(LSMessage&)+0x2aa> @ imm = #0x222
    cb50: 2801         	cmp	r0, #0x1
    cb52: f040 80ff    	bne.w	0xcd54 <VolumeManager::down(LSMessage&)+0x28c> @ imm = #0x1fe
    cb56: f89b 3000    	ldrb.w	r3, [r11]
    cb5a: f88d 3078    	strb.w	r3, [sp, #0x78]
    cb5e: 4623         	mov	r3, r4
    cb60: 2200         	movs	r2, #0x0
    cb62: 951d         	str	r5, [sp, #0x74]
    cb64: 4641         	mov	r1, r8
    cb66: 555a         	strb	r2, [r3, r5]
    cb68: 4630         	mov	r0, r6
    cb6a: ab05         	add	r3, sp, #0x14
    cb6c: 464a         	mov	r2, r9
    cb6e: f7fd fc33    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #-0x279a
    cb72: 4605         	mov	r5, r0
    cb74: 981c         	ldr	r0, [sp, #0x70]
    cb76: 42a0         	cmp	r0, r4
    cb78: d003         	beq	0xcb82 <VolumeManager::down(LSMessage&)+0xba> @ imm = #0x6
    cb7a: 991e         	ldr	r1, [sp, #0x78]
    cb7c: 3101         	adds	r1, #0x1
    cb7e: f7f7 e9c0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8c80
    cb82: 2d00         	cmp	r5, #0x0
    cb84: f000 80ec    	beq.w	0xcd60 <VolumeManager::down(LSMessage&)+0x298> @ imm = #0x1d8
    cb88: 4af3         	ldr	r2, [pc, #0x3cc]        @ 0xcf58 <VolumeManager::down(LSMessage&)+0x490> literal=0x246c
    cb8a: ad0d         	add	r5, sp, #0x34
    cb8c: 4641         	mov	r1, r8
    cb8e: 4628         	mov	r0, r5
    cb90: 447a         	add	r2, pc => raw=0xf000 ghidra=0x1f000 file=61440 "soundOutput"
    cb92: f7f7 e9bc    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x8c88
    cb96: 2300         	movs	r3, #0x0
    cb98: 4631         	mov	r1, r6
    cb9a: 4628         	mov	r0, r5
    cb9c: 941c         	str	r4, [sp, #0x70]
    cb9e: 931d         	str	r3, [sp, #0x74]
    cba0: f88d 3078    	strb.w	r3, [sp, #0x78]
    cba4: f7f7 e8fc    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x8e08
    cba8: 9b1c         	ldr	r3, [sp, #0x70]
    cbaa: 9810         	ldr	r0, [sp, #0x40]
    cbac: 9a1d         	ldr	r2, [sp, #0x74]
    cbae: 42a3         	cmp	r3, r4
    cbb0: f000 815b    	beq.w	0xce6a <VolumeManager::down(LSMessage&)+0x3a2> @ imm = #0x2b6
    cbb4: 9900         	ldr	r1, [sp]
    cbb6: f8dd c078    	ldr.w	r12, [sp, #0x78]
    cbba: e9cd 3210    	strd	r3, r2, [sp, #64]
    cbbe: 4288         	cmp	r0, r1
    cbc0: bf08         	it	eq
    cbc2: f8cd c048    	streq.w	r12, [sp, #0x48]
    cbc6: f000 815d    	beq.w	0xce84 <VolumeManager::down(LSMessage&)+0x3bc> @ imm = #0x2ba
    cbca: 9b12         	ldr	r3, [sp, #0x48]
    cbcc: f8cd c048    	str.w	r12, [sp, #0x48]
    cbd0: 2800         	cmp	r0, #0x0
    cbd2: f000 8157    	beq.w	0xce84 <VolumeManager::down(LSMessage&)+0x3bc> @ imm = #0x2ae
    cbd6: 931e         	str	r3, [sp, #0x78]
    cbd8: 901c         	str	r0, [sp, #0x70]
    cbda: 2300         	movs	r3, #0x0
    cbdc: 931d         	str	r3, [sp, #0x74]
    cbde: 7003         	strb	r3, [r0]
    cbe0: 981c         	ldr	r0, [sp, #0x70]
    cbe2: 42a0         	cmp	r0, r4
    cbe4: d003         	beq	0xcbee <VolumeManager::down(LSMessage&)+0x126> @ imm = #0x6
    cbe6: 991e         	ldr	r1, [sp, #0x78]
    cbe8: 3101         	adds	r1, #0x1
    cbea: f7f7 e98a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8cec
    cbee: 4bdb         	ldr	r3, [pc, #0x36c]        @ 0xcf5c <VolumeManager::down(LSMessage&)+0x494> literal=0x210
    cbf0: 4628         	mov	r0, r5
    cbf2: f85a 3003    	ldr.w	r3, [r10, r3]
    cbf6: f10d 0a28    	add.w	r10, sp, #0x28
    cbfa: 3308         	adds	r3, #0x8
    cbfc: 930d         	str	r3, [sp, #0x34]
    cbfe: f7f7 e9a6    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x8cb4
    cc02: 4650         	mov	r0, r10
    cc04: f7f7 e8d8    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x8e50
    cc08: 9a11         	ldr	r2, [sp, #0x44]
    cc0a: 68fb         	ldr	r3, [r7, #0xc]
    cc0c: 9810         	ldr	r0, [sp, #0x40]
    cc0e: 68b9         	ldr	r1, [r7, #0x8]
    cc10: 429a         	cmp	r2, r3
    cc12: f000 80bd    	beq.w	0xcd90 <VolumeManager::down(LSMessage&)+0x2c8> @ imm = #0x17a
    cc16: 2263         	movs	r2, #0x63
    cc18: 2364         	movs	r3, #0x64
    cc1a: e9cd 2302    	strd	r2, r3, [sp, #8]
    cc1e: 9b02         	ldr	r3, [sp, #0x8]
    cc20: f10d 0b40    	add.w	r11, sp, #0x40
    cc24: 4632         	mov	r2, r6
    cc26: 4659         	mov	r1, r11
    cc28: 4638         	mov	r0, r7
    cc2a: 931c         	str	r3, [sp, #0x70]
    cc2c: f7ff f832    	bl	0xbc94 <VolumeManager::set(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)> @ imm = #-0xf9c
    cc30: 2800         	cmp	r0, #0x0
    cc32: f040 80bb    	bne.w	0xcdac <VolumeManager::down(LSMessage&)+0x2e4> @ imm = #0x176
    cc36: 9a11         	ldr	r2, [sp, #0x44]
    cc38: 68fb         	ldr	r3, [r7, #0xc]
    cc3a: 429a         	cmp	r2, r3
    cc3c: f000 8126    	beq.w	0xce8c <VolumeManager::down(LSMessage&)+0x3c4> @ imm = #0x24c
    cc40: 2101         	movs	r1, #0x1
    cc42: 4628         	mov	r0, r5
    cc44: f7f7 e8d6    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x8e54
    cc48: 4ac5         	ldr	r2, [pc, #0x314]        @ 0xcf60 <VolumeManager::down(LSMessage&)+0x498> literal=0xacc
    cc4a: 4623         	mov	r3, r4
    cc4c: 941c         	str	r4, [sp, #0x70]
    cc4e: f04f 0c0b    	mov.w	r12, #0xb
    cc52: 2700         	movs	r7, #0x0
    cc54: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    cc56: ca07         	ldm	r2, {r0, r1, r2}
    cc58: c303         	stm	r3!, {r0, r1}
    cc5a: 4631         	mov	r1, r6
    cc5c: f823 2b02    	strh	r2, [r3], #2
    cc60: 0c12         	lsrs	r2, r2, #0x10
    cc62: 4650         	mov	r0, r10
    cc64: 701a         	strb	r2, [r3]
    cc66: 462a         	mov	r2, r5
    cc68: f8cd c074    	str.w	r12, [sp, #0x74]
    cc6c: f88d 7083    	strb.w	r7, [sp, #0x83]
    cc70: f7f7 e830    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8fa0
    cc74: 981c         	ldr	r0, [sp, #0x70]
    cc76: 42a0         	cmp	r0, r4
    cc78: d003         	beq	0xcc82 <VolumeManager::down(LSMessage&)+0x1ba> @ imm = #0x6
    cc7a: 991e         	ldr	r1, [sp, #0x78]
    cc7c: 3101         	adds	r1, #0x1
    cc7e: f7f7 e940    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8d80
    cc82: 4628         	mov	r0, r5
    cc84: f7f7 e868    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8f30
    cc88: 4659         	mov	r1, r11
    cc8a: 4628         	mov	r0, r5
    cc8c: f7f7 ea4e    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x8b64
    cc90: 4ab4         	ldr	r2, [pc, #0x2d0]        @ 0xcf64 <VolumeManager::down(LSMessage&)+0x49c> literal=0x2360
    cc92: 4623         	mov	r3, r4
    cc94: 941c         	str	r4, [sp, #0x70]
    cc96: f04f 0c0b    	mov.w	r12, #0xb
    cc9a: 2700         	movs	r7, #0x0
    cc9c: 447a         	add	r2, pc => raw=0xf000 ghidra=0x1f000 file=61440 "soundOutput"
    cc9e: ca07         	ldm	r2, {r0, r1, r2}
    cca0: c303         	stm	r3!, {r0, r1}
    cca2: 4631         	mov	r1, r6
    cca4: f823 2b02    	strh	r2, [r3], #2
    cca8: 0c12         	lsrs	r2, r2, #0x10
    ccaa: 4650         	mov	r0, r10
    ccac: 701a         	strb	r2, [r3]
    ccae: 462a         	mov	r2, r5
    ccb0: f8cd c074    	str.w	r12, [sp, #0x74]
    ccb4: f88d 7083    	strb.w	r7, [sp, #0x83]
    ccb8: f7f7 e80c    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x8fe8
    ccbc: 981c         	ldr	r0, [sp, #0x70]
    ccbe: 42a0         	cmp	r0, r4
    ccc0: d003         	beq	0xccca <VolumeManager::down(LSMessage&)+0x202> @ imm = #0x6
    ccc2: 991e         	ldr	r1, [sp, #0x78]
    ccc4: 3101         	adds	r1, #0x1
    ccc6: f7f7 e91c    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8dc8
    ccca: 4628         	mov	r0, r5
    cccc: f7f7 e844    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8f78
    ccd0: 9902         	ldr	r1, [sp, #0x8]
    ccd2: 4628         	mov	r0, r5
    ccd4: f7f7 ea30    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x8ba0
    ccd8: 4ba3         	ldr	r3, [pc, #0x28c]        @ 0xcf68 <VolumeManager::down(LSMessage&)+0x4a0> literal=0x15fa
    ccda: 2206         	movs	r2, #0x6
    ccdc: 921d         	str	r2, [sp, #0x74]
    ccde: 2200         	movs	r2, #0x0
    cce0: f88d 207e    	strb.w	r2, [sp, #0x7e]
    cce4: 462a         	mov	r2, r5
    cce6: 447b         	add	r3, pc => raw=0xe2e4 ghidra=0x1e2e4 file=58084 "volume"
    cce8: 941c         	str	r4, [sp, #0x70]
    ccea: e893 0003    	ldm.w	r3, {r0, r1}
    ccee: 901e         	str	r0, [sp, #0x78]
    ccf0: 4650         	mov	r0, r10
    ccf2: f8ad 107c    	strh.w	r1, [sp, #0x7c]
    ccf6: 4631         	mov	r1, r6
    ccf8: f7f6 efec    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x9028
    ccfc: 981c         	ldr	r0, [sp, #0x70]
    ccfe: 42a0         	cmp	r0, r4
    cd00: d003         	beq	0xcd0a <VolumeManager::down(LSMessage&)+0x242> @ imm = #0x6
    cd02: 991e         	ldr	r1, [sp, #0x78]
    cd04: 3101         	adds	r1, #0x1
    cd06: f7f7 e8fc    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8e08
    cd0a: 4628         	mov	r0, r5
    cd0c: f7f7 e824    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8fb8
    cd10: 4651         	mov	r1, r10
    cd12: a806         	add	r0, sp, #0x18
    cd14: f7fd f8bc    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #-0x2e88
    cd18: 4650         	mov	r0, r10
    cd1a: f7f7 e81e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8fc4
    cd1e: 9816         	ldr	r0, [sp, #0x58]
    cd20: 9b01         	ldr	r3, [sp, #0x4]
    cd22: 4298         	cmp	r0, r3
    cd24: d003         	beq	0xcd2e <VolumeManager::down(LSMessage&)+0x266> @ imm = #0x6
    cd26: 9918         	ldr	r1, [sp, #0x60]
    cd28: 3101         	adds	r1, #0x1
    cd2a: f7f7 e8ea    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8e2c
    cd2e: 4640         	mov	r0, r8
    cd30: f7f7 e812    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x8fdc
    cd34: 9806         	ldr	r0, [sp, #0x18]
    cd36: b108         	cbz	r0, 0xcd3c <VolumeManager::down(LSMessage&)+0x274> @ imm = #0x2
    cd38: f7f7 e988    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x8cf0
    cd3c: 9810         	ldr	r0, [sp, #0x40]
    cd3e: 9b00         	ldr	r3, [sp]
    cd40: 4298         	cmp	r0, r3
    cd42: d003         	beq	0xcd4c <VolumeManager::down(LSMessage&)+0x284> @ imm = #0x6
    cd44: 9912         	ldr	r1, [sp, #0x48]
    cd46: 3101         	adds	r1, #0x1
    cd48: f7f7 e8da    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8e4c
    cd4c: 2001         	movs	r0, #0x1
    cd4e: b023         	add	sp, #0x8c
    cd50: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    cd54: 4623         	mov	r3, r4
    cd56: 2800         	cmp	r0, #0x0
    cd58: f43f af02    	beq.w	0xcb60 <VolumeManager::down(LSMessage&)+0x98> @ imm = #-0x1fc
    cd5c: 4620         	mov	r0, r4
    cd5e: e010         	b	0xcd82 <VolumeManager::down(LSMessage&)+0x2ba> @ imm = #0x20
    cd60: 4982         	ldr	r1, [pc, #0x208]        @ 0xcf6c <VolumeManager::down(LSMessage&)+0x4a4> literal=0x14624
    cd62: 462b         	mov	r3, r5
    cd64: 2203         	movs	r2, #0x3
    cd66: a806         	add	r0, sp, #0x18
    cd68: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    cd6a: 311c         	adds	r1, #0x1c
    cd6c: f7fc ff6e    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x3124
    cd70: e7d5         	b	0xcd1e <VolumeManager::down(LSMessage&)+0x256> @ imm = #-0x56
    cd72: 2200         	movs	r2, #0x0
    cd74: a90d         	add	r1, sp, #0x34
    cd76: 4630         	mov	r0, r6
    cd78: f7f7 e97c    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x8d08
    cd7c: 9b0d         	ldr	r3, [sp, #0x34]
    cd7e: 901c         	str	r0, [sp, #0x70]
    cd80: 931e         	str	r3, [sp, #0x78]
    cd82: 462a         	mov	r2, r5
    cd84: 4659         	mov	r1, r11
    cd86: f7f7 e900    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x8e00
    cd8a: 9d0d         	ldr	r5, [sp, #0x34]
    cd8c: 9b1c         	ldr	r3, [sp, #0x70]
    cd8e: e6e7         	b	0xcb60 <VolumeManager::down(LSMessage&)+0x98> @ imm = #-0x232
    cd90: b122         	cbz	r2, 0xcd9c <VolumeManager::down(LSMessage&)+0x2d4> @ imm = #0x8
    cd92: f7f7 e9de    	blx	0x4150 <memcmp@plt>     @ imm = #-0x8c44
    cd96: 2800         	cmp	r0, #0x0
    cd98: f47f af3d    	bne.w	0xcc16 <VolumeManager::down(LSMessage&)+0x14e> @ imm = #-0x186
    cd9c: f897 3021    	ldrb.w	r3, [r7, #0x21]
    cda0: 9303         	str	r3, [sp, #0xc]
    cda2: 2b00         	cmp	r3, #0x0
    cda4: d07f         	beq	0xcea6 <VolumeManager::down(LSMessage&)+0x3de> @ imm = #0xfe
    cda6: 3b01         	subs	r3, #0x1
    cda8: 9302         	str	r3, [sp, #0x8]
    cdaa: e738         	b	0xcc1e <VolumeManager::down(LSMessage&)+0x156> @ imm = #-0x190
    cdac: 2100         	movs	r1, #0x0
    cdae: 4628         	mov	r0, r5
    cdb0: f7f7 e820    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x8fc0
    cdb4: 4a6e         	ldr	r2, [pc, #0x1b8]        @ 0xcf70 <VolumeManager::down(LSMessage&)+0x4a8> literal=0x960
    cdb6: 4623         	mov	r3, r4
    cdb8: 941c         	str	r4, [sp, #0x70]
    cdba: f04f 0c0b    	mov.w	r12, #0xb
    cdbe: 2700         	movs	r7, #0x0
    cdc0: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    cdc2: ca07         	ldm	r2, {r0, r1, r2}
    cdc4: c303         	stm	r3!, {r0, r1}
    cdc6: 4631         	mov	r1, r6
    cdc8: f823 2b02    	strh	r2, [r3], #2
    cdcc: 0c12         	lsrs	r2, r2, #0x10
    cdce: 4650         	mov	r0, r10
    cdd0: 701a         	strb	r2, [r3]
    cdd2: 462a         	mov	r2, r5
    cdd4: f8cd c074    	str.w	r12, [sp, #0x74]
    cdd8: f88d 7083    	strb.w	r7, [sp, #0x83]
    cddc: f7f6 ef7a    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x910c
    cde0: 981c         	ldr	r0, [sp, #0x70]
    cde2: 42a0         	cmp	r0, r4
    cde4: d003         	beq	0xcdee <VolumeManager::down(LSMessage&)+0x326> @ imm = #0x6
    cde6: 991e         	ldr	r1, [sp, #0x78]
    cde8: 3101         	adds	r1, #0x1
    cdea: f7f7 e88a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8eec
    cdee: 4628         	mov	r0, r5
    cdf0: f7f6 efb2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x909c
    cdf4: 495f         	ldr	r1, [pc, #0x17c]        @ 0xcf74 <VolumeManager::down(LSMessage&)+0x4ac> literal=0x14594
    cdf6: 4628         	mov	r0, r5
    cdf8: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    cdfa: 3164         	adds	r1, #0x64
    cdfc: f7f7 e996    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x8cd4
    ce00: 4a5d         	ldr	r2, [pc, #0x174]        @ 0xcf78 <VolumeManager::down(LSMessage&)+0x4b0> literal=0x184c
    ce02: 4623         	mov	r3, r4
    ce04: 941c         	str	r4, [sp, #0x70]
    ce06: f04f 0c09    	mov.w	r12, #0x9
    ce0a: 2700         	movs	r7, #0x0
    ce0c: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    ce0e: ca07         	ldm	r2, {r0, r1, r2}
    ce10: c303         	stm	r3!, {r0, r1}
    ce12: 4631         	mov	r1, r6
    ce14: 701a         	strb	r2, [r3]
    ce16: 4650         	mov	r0, r10
    ce18: 462a         	mov	r2, r5
    ce1a: f8cd c074    	str.w	r12, [sp, #0x74]
    ce1e: f88d 7081    	strb.w	r7, [sp, #0x81]
    ce22: f7f6 ef58    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x9150
    ce26: 981c         	ldr	r0, [sp, #0x70]
    ce28: 42a0         	cmp	r0, r4
    ce2a: d003         	beq	0xce34 <VolumeManager::down(LSMessage&)+0x36c> @ imm = #0x6
    ce2c: 991e         	ldr	r1, [sp, #0x78]
    ce2e: 3101         	adds	r1, #0x1
    ce30: f7f7 e866    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x8f34
    ce34: 4628         	mov	r0, r5
    ce36: f7f6 ef90    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x90e0
    ce3a: 2114         	movs	r1, #0x14
    ce3c: 4628         	mov	r0, r5
    ce3e: f7f7 e97c    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x8d08
    ce42: 4a4e         	ldr	r2, [pc, #0x138]        @ 0xcf7c <VolumeManager::down(LSMessage&)+0x4b4> literal=0x1816
    ce44: 4623         	mov	r3, r4
    ce46: 941c         	str	r4, [sp, #0x70]
    ce48: f04f 0c09    	mov.w	r12, #0x9
    ce4c: 2700         	movs	r7, #0x0
    ce4e: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    ce50: ca07         	ldm	r2, {r0, r1, r2}
    ce52: c303         	stm	r3!, {r0, r1}
    ce54: 4631         	mov	r1, r6
    ce56: 701a         	strb	r2, [r3]
    ce58: 4650         	mov	r0, r10
    ce5a: 462a         	mov	r2, r5
    ce5c: f8cd c074    	str.w	r12, [sp, #0x74]
    ce60: f88d 7081    	strb.w	r7, [sp, #0x81]
    ce64: f7f6 ef36    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x9194
    ce68: e748         	b	0xccfc <VolumeManager::down(LSMessage&)+0x234> @ imm = #-0x170
    ce6a: b132         	cbz	r2, 0xce7a <VolumeManager::down(LSMessage&)+0x3b2> @ imm = #0xc
    ce6c: 2a01         	cmp	r2, #0x1
    ce6e: d025         	beq	0xcebc <VolumeManager::down(LSMessage&)+0x3f4> @ imm = #0x4a
    ce70: 4621         	mov	r1, r4
    ce72: f7f7 e88a    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x8eec
    ce76: 9a1d         	ldr	r2, [sp, #0x74]
    ce78: 9810         	ldr	r0, [sp, #0x40]
    ce7a: 2300         	movs	r3, #0x0
    ce7c: 9211         	str	r2, [sp, #0x44]
    ce7e: 5483         	strb	r3, [r0, r2]
    ce80: 981c         	ldr	r0, [sp, #0x70]
    ce82: e6aa         	b	0xcbda <VolumeManager::down(LSMessage&)+0x112> @ imm = #-0x2ac
    ce84: 941c         	str	r4, [sp, #0x70]
    ce86: ac1e         	add	r4, sp, #0x78
    ce88: 4620         	mov	r0, r4
    ce8a: e6a6         	b	0xcbda <VolumeManager::down(LSMessage&)+0x112> @ imm = #-0x2b4
    ce8c: 9810         	ldr	r0, [sp, #0x40]
    ce8e: 68b9         	ldr	r1, [r7, #0x8]
    ce90: b122         	cbz	r2, 0xce9c <VolumeManager::down(LSMessage&)+0x3d4> @ imm = #0x8
    ce92: f7f7 e95e    	blx	0x4150 <memcmp@plt>     @ imm = #-0x8d44
    ce96: 2800         	cmp	r0, #0x0
    ce98: f47f aed2    	bne.w	0xcc40 <VolumeManager::down(LSMessage&)+0x178> @ imm = #-0x25c
    ce9c: 9b03         	ldr	r3, [sp, #0xc]
    ce9e: 3b01         	subs	r3, #0x1
    cea0: f887 3021    	strb.w	r3, [r7, #0x21]
    cea4: e6cc         	b	0xcc40 <VolumeManager::down(LSMessage&)+0x178> @ imm = #-0x268
    cea6: 4936         	ldr	r1, [pc, #0xd8]         @ 0xcf80 <VolumeManager::down(LSMessage&)+0x4b8> literal=0x144da
    cea8: 22ca         	movs	r2, #0xca
    ceaa: 9b03         	ldr	r3, [sp, #0xc]
    ceac: a806         	add	r0, sp, #0x18
    ceae: f10d 0b40    	add.w	r11, sp, #0x40
    ceb2: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    ceb4: 31c4         	adds	r1, #0xc4
    ceb6: f7fc fec9    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x326e
    ceba: e72d         	b	0xcd18 <VolumeManager::down(LSMessage&)+0x250> @ imm = #-0x1a6
    cebc: f89d 3078    	ldrb.w	r3, [sp, #0x78]
    cec0: 7003         	strb	r3, [r0]
    cec2: 9a1d         	ldr	r2, [sp, #0x74]
    cec4: 9810         	ldr	r0, [sp, #0x40]
    cec6: e7d8         	b	0xce7a <VolumeManager::down(LSMessage&)+0x3b2> @ imm = #-0x50
    cec8: 482e         	ldr	r0, [pc, #0xb8]         @ 0xcf84 <VolumeManager::down(LSMessage&)+0x4bc> literal=0x7ca
    ceca: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    cecc: f7f6 ef1a    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x91cc
    ced0: 4630         	mov	r0, r6
    ced2: f7f7 e90e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8de4
    ced6: 4628         	mov	r0, r5
    ced8: f7f6 ef3e    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x9184
    cedc: 4650         	mov	r0, r10
    cede: f7f6 ef3c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x9188
    cee2: e007         	b	0xcef4 <VolumeManager::down(LSMessage&)+0x42c> @ imm = #0xe
    cee4: f10d 0b40    	add.w	r11, sp, #0x40
    cee8: e007         	b	0xcefa <VolumeManager::down(LSMessage&)+0x432> @ imm = #0xe
    ceea: 4630         	mov	r0, r6
    ceec: f10d 0b40    	add.w	r11, sp, #0x40
    cef0: f7f7 e8fe    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8e04
    cef4: 4648         	mov	r0, r9
    cef6: f7f7 e8fc    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8e08
    cefa: 4640         	mov	r0, r8
    cefc: f7f6 ef2c    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x91a8
    cf00: 9806         	ldr	r0, [sp, #0x18]
    cf02: b108         	cbz	r0, 0xcf08 <VolumeManager::down(LSMessage&)+0x440> @ imm = #0x2
    cf04: f7f7 e8a2    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x8ebc
    cf08: 4658         	mov	r0, r11
    cf0a: f7f7 e8f2    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8e1c
    cf0e: f7f7 e938    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x8d90
    cf12: 9806         	ldr	r0, [sp, #0x18]
    cf14: f10d 0b40    	add.w	r11, sp, #0x40
    cf18: e7f4         	b	0xcf04 <VolumeManager::down(LSMessage&)+0x43c> @ imm = #-0x18
    cf1a: 4630         	mov	r0, r6
    cf1c: f10d 0b40    	add.w	r11, sp, #0x40
    cf20: f7f7 e8e6    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x8e34
    cf24: 4b0d         	ldr	r3, [pc, #0x34]         @ 0xcf5c <VolumeManager::down(LSMessage&)+0x494> literal=0x210
    cf26: 4628         	mov	r0, r5
    cf28: f85a 3003    	ldr.w	r3, [r10, r3]
    cf2c: 3308         	adds	r3, #0x8
    cf2e: 930d         	str	r3, [sp, #0x34]
    cf30: f7f7 e80c    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x8fe8
    cf34: e7de         	b	0xcef4 <VolumeManager::down(LSMessage&)+0x42c> @ imm = #-0x44
    cf36: e7cb         	b	0xced0 <VolumeManager::down(LSMessage&)+0x408> @ imm = #-0x6a
    cf38: e7ca         	b	0xced0 <VolumeManager::down(LSMessage&)+0x408> @ imm = #-0x6c
    cf3a: f10d 0b40    	add.w	r11, sp, #0x40
    cf3e: e7e3         	b	0xcf08 <VolumeManager::down(LSMessage&)+0x440> @ imm = #-0x3a
    cf40: f10d 0b40    	add.w	r11, sp, #0x40
    cf44: e7d6         	b	0xcef4 <VolumeManager::down(LSMessage&)+0x42c> @ imm = #-0x54
    cf46: e7c3         	b	0xced0 <VolumeManager::down(LSMessage&)+0x408> @ imm = #-0x7a
    cf48: e7c2         	b	0xced0 <VolumeManager::down(LSMessage&)+0x408> @ imm = #-0x7c
    cf4a: e7c7         	b	0xcedc <VolumeManager::down(LSMessage&)+0x414> @ imm = #-0x72
    cf4c: e7c0         	b	0xced0 <VolumeManager::down(LSMessage&)+0x408> @ imm = #-0x80
    cf4e: bf00         	nop
    cf50: 42e0         	cmn	r0, r4
    cf52: 0001         	movs	r1, r0
    cf54: 25d6         	movs	r5, #0xd6
    cf56: 0000         	movs	r0, r0
    cf58: 246c         	movs	r4, #0x6c
    cf5a: 0000         	movs	r0, r0
    cf5c: 0210         	lsls	r0, r2, #0x8
    cf5e: 0000         	movs	r0, r0
    cf60: 0acc         	lsrs	r4, r1, #0xb
    cf62: 0000         	movs	r0, r0
    cf64: 2360         	movs	r3, #0x60
    cf66: 0000         	movs	r0, r0
    cf68: 15fa         	asrs	r2, r7, #0x17
    cf6a: 0000         	movs	r0, r0
    cf6c: 4624         	mov	r4, r4
    cf6e: 0001         	movs	r1, r0
    cf70: 0960         	lsrs	r0, r4, #0x5
    cf72: 0000         	movs	r0, r0
    cf74: 4594         	cmp	r12, r2
    cf76: 0001         	movs	r1, r0
    cf78: 184c         	adds	r4, r1, r1
    cf7a: 0000         	movs	r0, r0
    cf7c: 1816         	adds	r6, r2, r0
    cf7e: 0000         	movs	r0, r0
    cf80: 44da         	add	r10, r11
    cf82: 0001         	movs	r1, r0
    cf84: 07ca         	lsls	r2, r1, #0x1f
    cf86: 0000         	movs	r0, r0

0000cf88 <VolumeManager::muteSoundOut(LSMessage&)>:
    cf88: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    cf8c: b0a3         	sub	sp, #0x8c
    cf8e: f8df a478    	ldr.w	r10, [pc, #0x478]       @ 0xd408 <VolumeManager::muteSoundOut(LSMessage&)+0x480> literal=0x13e20
    cf92: 2300         	movs	r3, #0x0
    cf94: aa12         	add	r2, sp, #0x48
    cf96: 4680         	mov	r8, r0
    cf98: 4608         	mov	r0, r1
    cf9a: 460c         	mov	r4, r1
    cf9c: 44fa         	add	r10, pc => raw=0x20dc0 ghidra=0x30dc0 file=69056
    cf9e: 9201         	str	r2, [sp, #0x4]
    cfa0: e9cd 2310    	strd	r2, r3, [sp, #64]
    cfa4: f88d 3048    	strb.w	r3, [sp, #0x48]
    cfa8: e9cd 3105    	strd	r3, r1, [sp, #20]
    cfac: f7f7 e89a    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x8ecc
    cfb0: af07         	add	r7, sp, #0x1c
    cfb2: 4638         	mov	r0, r7
    cfb4: f7f7 e932    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x8d9c
    cfb8: ab18         	add	r3, sp, #0x60
    cfba: f10d 0958    	add.w	r9, sp, #0x58
    cfbe: ae1c         	add	r6, sp, #0x70
    cfc0: 9302         	str	r3, [sp, #0x8]
    cfc2: 9316         	str	r3, [sp, #0x58]
    cfc4: 2200         	movs	r2, #0x0
    cfc6: 2399         	movs	r3, #0x99
    cfc8: 4631         	mov	r1, r6
    cfca: 4648         	mov	r0, r9
    cfcc: 931c         	str	r3, [sp, #0x70]
    cfce: f7f7 e852    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x8f5c
    cfd2: f8df 1438    	ldr.w	r1, [pc, #0x438]        @ 0xd40c <VolumeManager::muteSoundOut(LSMessage&)+0x484> literal=0x2194
    cfd6: 2299         	movs	r2, #0x99
    cfd8: 9b1c         	ldr	r3, [sp, #0x70]
    cfda: 9016         	str	r0, [sp, #0x58]
    cfdc: 4479         	add	r1, pc => raw=0xf174 ghidra=0x1f174 file=61812 "{\"type\":\"object\",\"properties\":{\"soundOutput\":{\"type\":\"string\"},\"mute\":{\"type\":\"boolean\"}},\"required\":[\"soundOutput\",\"mute\"],\"additionalProperties\":false}"
    cfde: 9318         	str	r3, [sp, #0x60]
    cfe0: f7f6 efd2    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x905c
    cfe4: 9b1c         	ldr	r3, [sp, #0x70]
    cfe6: 2100         	movs	r1, #0x0
    cfe8: 9a16         	ldr	r2, [sp, #0x58]
    cfea: 4620         	mov	r0, r4
    cfec: 9317         	str	r3, [sp, #0x5c]
    cfee: 54d1         	strb	r1, [r2, r3]
    cff0: f7f7 e8d2    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x8e5c
    cff4: ac1e         	add	r4, sp, #0x78
    cff6: 4683         	mov	r11, r0
    cff8: 941c         	str	r4, [sp, #0x70]
    cffa: 2800         	cmp	r0, #0x0
    cffc: f000 81b7    	beq.w	0xd36e <VolumeManager::muteSoundOut(LSMessage&)+0x3e6> @ imm = #0x36e
    d000: f7f7 e850    	blx	0x40a4 <strlen@plt>     @ imm = #-0x8f60
    d004: 280f         	cmp	r0, #0xf
    d006: 4605         	mov	r5, r0
    d008: 900d         	str	r0, [sp, #0x34]
    d00a: f200 811b    	bhi.w	0xd244 <VolumeManager::muteSoundOut(LSMessage&)+0x2bc> @ imm = #0x236
    d00e: 2801         	cmp	r0, #0x1
    d010: f040 8109    	bne.w	0xd226 <VolumeManager::muteSoundOut(LSMessage&)+0x29e> @ imm = #0x212
    d014: f89b 3000    	ldrb.w	r3, [r11]
    d018: f88d 3078    	strb.w	r3, [sp, #0x78]
    d01c: 4623         	mov	r3, r4
    d01e: 2200         	movs	r2, #0x0
    d020: 951d         	str	r5, [sp, #0x74]
    d022: 4639         	mov	r1, r7
    d024: 555a         	strb	r2, [r3, r5]
    d026: 4630         	mov	r0, r6
    d028: ab05         	add	r3, sp, #0x14
    d02a: 464a         	mov	r2, r9
    d02c: f7fd f9d4    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #-0x2c58
    d030: 4605         	mov	r5, r0
    d032: 981c         	ldr	r0, [sp, #0x70]
    d034: 42a0         	cmp	r0, r4
    d036: d003         	beq	0xd040 <VolumeManager::muteSoundOut(LSMessage&)+0xb8> @ imm = #0x6
    d038: 991e         	ldr	r1, [sp, #0x78]
    d03a: 3101         	adds	r1, #0x1
    d03c: f7f6 ef60    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x9140
    d040: 2d00         	cmp	r5, #0x0
    d042: f000 80f6    	beq.w	0xd232 <VolumeManager::muteSoundOut(LSMessage&)+0x2aa> @ imm = #0x1ec
    d046: 4af2         	ldr	r2, [pc, #0x3c8]        @ 0xd410 <VolumeManager::muteSoundOut(LSMessage&)+0x488> literal=0x1fae
    d048: ad0d         	add	r5, sp, #0x34
    d04a: 4639         	mov	r1, r7
    d04c: 4628         	mov	r0, r5
    d04e: 447a         	add	r2, pc => raw=0xf000 ghidra=0x1f000 file=61440 "soundOutput"
    d050: f7f6 ef5c    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x9148
    d054: 2300         	movs	r3, #0x0
    d056: 4631         	mov	r1, r6
    d058: 4628         	mov	r0, r5
    d05a: 941c         	str	r4, [sp, #0x70]
    d05c: 931d         	str	r3, [sp, #0x74]
    d05e: f88d 3078    	strb.w	r3, [sp, #0x78]
    d062: f7f6 ee9e    	blx	0x3da0 <_ZNK7pbnjson6JValue8asStringERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x92c4
    d066: 9b1c         	ldr	r3, [sp, #0x70]
    d068: 9810         	ldr	r0, [sp, #0x40]
    d06a: 9a1d         	ldr	r2, [sp, #0x74]
    d06c: 42a3         	cmp	r3, r4
    d06e: f000 815a    	beq.w	0xd326 <VolumeManager::muteSoundOut(LSMessage&)+0x39e> @ imm = #0x2b4
    d072: 9901         	ldr	r1, [sp, #0x4]
    d074: f8dd c078    	ldr.w	r12, [sp, #0x78]
    d078: e9cd 3210    	strd	r3, r2, [sp, #64]
    d07c: 4288         	cmp	r0, r1
    d07e: bf08         	it	eq
    d080: f8cd c048    	streq.w	r12, [sp, #0x48]
    d084: f000 8169    	beq.w	0xd35a <VolumeManager::muteSoundOut(LSMessage&)+0x3d2> @ imm = #0x2d2
    d088: 9b12         	ldr	r3, [sp, #0x48]
    d08a: f8cd c048    	str.w	r12, [sp, #0x48]
    d08e: 2800         	cmp	r0, #0x0
    d090: f000 8163    	beq.w	0xd35a <VolumeManager::muteSoundOut(LSMessage&)+0x3d2> @ imm = #0x2c6
    d094: 931e         	str	r3, [sp, #0x78]
    d096: 901c         	str	r0, [sp, #0x70]
    d098: 2300         	movs	r3, #0x0
    d09a: 931d         	str	r3, [sp, #0x74]
    d09c: 7003         	strb	r3, [r0]
    d09e: 981c         	ldr	r0, [sp, #0x70]
    d0a0: 42a0         	cmp	r0, r4
    d0a2: d003         	beq	0xd0ac <VolumeManager::muteSoundOut(LSMessage&)+0x124> @ imm = #0x6
    d0a4: 991e         	ldr	r1, [sp, #0x78]
    d0a6: 3101         	adds	r1, #0x1
    d0a8: f7f6 ef2a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x91ac
    d0ac: 4bd9         	ldr	r3, [pc, #0x364]        @ 0xd414 <VolumeManager::muteSoundOut(LSMessage&)+0x48c> literal=0x210
    d0ae: 4628         	mov	r0, r5
    d0b0: f85a a003    	ldr.w	r10, [r10, r3]
    d0b4: f10a 0308    	add.w	r3, r10, #0x8
    d0b8: 930d         	str	r3, [sp, #0x34]
    d0ba: f7f6 ef48    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x9170
    d0be: 4ad6         	ldr	r2, [pc, #0x358]        @ 0xd418 <VolumeManager::muteSoundOut(LSMessage&)+0x490> literal=0x1124
    d0c0: 4639         	mov	r1, r7
    d0c2: 4630         	mov	r0, r6
    d0c4: 447a         	add	r2, pc => raw=0xe1ec ghidra=0x1e1ec file=57836 "mute"
    d0c6: f7f6 ef22    	blx	0x3f0c <_ZNK7pbnjson6JValueixEPKc@plt> @ imm = #-0x91bc
    d0ca: 4629         	mov	r1, r5
    d0cc: 4630         	mov	r0, r6
    d0ce: f7f6 ee8c    	blx	0x3de8 <_ZNK7pbnjson6JValue6asBoolERb@plt> @ imm = #-0x92e8
    d0d2: f10a 0308    	add.w	r3, r10, #0x8
    d0d6: 4630         	mov	r0, r6
    d0d8: 931c         	str	r3, [sp, #0x70]
    d0da: f10d 0a28    	add.w	r10, sp, #0x28
    d0de: f89d 3034    	ldrb.w	r3, [sp, #0x34]
    d0e2: 9303         	str	r3, [sp, #0xc]
    d0e4: f7f6 ef32    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x919c
    d0e8: 4650         	mov	r0, r10
    d0ea: f7f6 ee66    	blx	0x3db8 <_ZN7pbnjson6ObjectEv@plt> @ imm = #-0x9334
    d0ee: 9b03         	ldr	r3, [sp, #0xc]
    d0f0: f10d 0b40    	add.w	r11, sp, #0x40
    d0f4: 4632         	mov	r2, r6
    d0f6: 4659         	mov	r1, r11
    d0f8: 4640         	mov	r0, r8
    d0fa: 931c         	str	r3, [sp, #0x70]
    d0fc: f7fe fe34    	bl	0xbd68 <VolumeManager::muteSoundOut(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int const&)> @ imm = #-0x1398
    d100: 2800         	cmp	r0, #0x0
    d102: f040 80ae    	bne.w	0xd262 <VolumeManager::muteSoundOut(LSMessage&)+0x2da> @ imm = #0x15c
    d106: 9a11         	ldr	r2, [sp, #0x44]
    d108: f8d8 300c    	ldr.w	r3, [r8, #0xc]
    d10c: 429a         	cmp	r2, r3
    d10e: f000 8117    	beq.w	0xd340 <VolumeManager::muteSoundOut(LSMessage&)+0x3b8> @ imm = #0x22e
    d112: 2101         	movs	r1, #0x1
    d114: 4628         	mov	r0, r5
    d116: f7f6 ee6e    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x9324
    d11a: 4ac0         	ldr	r2, [pc, #0x300]        @ 0xd41c <VolumeManager::muteSoundOut(LSMessage&)+0x494> literal=0x5f8
    d11c: 4623         	mov	r3, r4
    d11e: 941c         	str	r4, [sp, #0x70]
    d120: f04f 0e0b    	mov.w	lr, #0xb
    d124: f04f 0c00    	mov.w	r12, #0x0
    d128: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    d12a: ca07         	ldm	r2, {r0, r1, r2}
    d12c: c303         	stm	r3!, {r0, r1}
    d12e: 4631         	mov	r1, r6
    d130: f823 2b02    	strh	r2, [r3], #2
    d134: 0c12         	lsrs	r2, r2, #0x10
    d136: 4650         	mov	r0, r10
    d138: 701a         	strb	r2, [r3]
    d13a: 462a         	mov	r2, r5
    d13c: f8cd e074    	str.w	lr, [sp, #0x74]
    d140: f88d c083    	strb.w	r12, [sp, #0x83]
    d144: f7f6 edc6    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x9474
    d148: 981c         	ldr	r0, [sp, #0x70]
    d14a: 42a0         	cmp	r0, r4
    d14c: d003         	beq	0xd156 <VolumeManager::muteSoundOut(LSMessage&)+0x1ce> @ imm = #0x6
    d14e: 991e         	ldr	r1, [sp, #0x78]
    d150: 3101         	adds	r1, #0x1
    d152: f7f6 eed6    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x9254
    d156: 4628         	mov	r0, r5
    d158: f7f6 edfe    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x9404
    d15c: 4659         	mov	r1, r11
    d15e: 4628         	mov	r0, r5
    d160: f7f6 efe4    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x9038
    d164: 4aae         	ldr	r2, [pc, #0x2b8]        @ 0xd420 <VolumeManager::muteSoundOut(LSMessage&)+0x498> literal=0x1e8a
    d166: 4623         	mov	r3, r4
    d168: 941c         	str	r4, [sp, #0x70]
    d16a: f04f 0e0b    	mov.w	lr, #0xb
    d16e: f04f 0c00    	mov.w	r12, #0x0
    d172: 447a         	add	r2, pc => raw=0xf000 ghidra=0x1f000 file=61440 "soundOutput"
    d174: ca07         	ldm	r2, {r0, r1, r2}
    d176: c303         	stm	r3!, {r0, r1}
    d178: 4631         	mov	r1, r6
    d17a: f823 2b02    	strh	r2, [r3], #2
    d17e: 0c12         	lsrs	r2, r2, #0x10
    d180: 4650         	mov	r0, r10
    d182: 701a         	strb	r2, [r3]
    d184: 462a         	mov	r2, r5
    d186: f8cd e074    	str.w	lr, [sp, #0x74]
    d18a: f88d c083    	strb.w	r12, [sp, #0x83]
    d18e: f7f6 eda2    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x94bc
    d192: 981c         	ldr	r0, [sp, #0x70]
    d194: 42a0         	cmp	r0, r4
    d196: d003         	beq	0xd1a0 <VolumeManager::muteSoundOut(LSMessage&)+0x218> @ imm = #0x6
    d198: 991e         	ldr	r1, [sp, #0x78]
    d19a: 3101         	adds	r1, #0x1
    d19c: f7f6 eeb0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x92a0
    d1a0: 4628         	mov	r0, r5
    d1a2: f7f6 edda    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x944c
    d1a6: 9903         	ldr	r1, [sp, #0xc]
    d1a8: 4628         	mov	r0, r5
    d1aa: f7f6 ee24    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x93b8
    d1ae: f247 536d    	movw	r3, #0x756d
    d1b2: 462a         	mov	r2, r5
    d1b4: f2c6 5374    	movt	r3, #0x6574
    d1b8: 4631         	mov	r1, r6
    d1ba: 931e         	str	r3, [sp, #0x78]
    d1bc: 2304         	movs	r3, #0x4
    d1be: 931d         	str	r3, [sp, #0x74]
    d1c0: 4650         	mov	r0, r10
    d1c2: 2300         	movs	r3, #0x0
    d1c4: 941c         	str	r4, [sp, #0x70]
    d1c6: f88d 307c    	strb.w	r3, [sp, #0x7c]
    d1ca: f7f6 ed84    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x94f8
    d1ce: 981c         	ldr	r0, [sp, #0x70]
    d1d0: 42a0         	cmp	r0, r4
    d1d2: d003         	beq	0xd1dc <VolumeManager::muteSoundOut(LSMessage&)+0x254> @ imm = #0x6
    d1d4: 991e         	ldr	r1, [sp, #0x78]
    d1d6: 3101         	adds	r1, #0x1
    d1d8: f7f6 ee92    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x92dc
    d1dc: 4628         	mov	r0, r5
    d1de: f7f6 edbc    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x9488
    d1e2: 4651         	mov	r1, r10
    d1e4: a806         	add	r0, sp, #0x18
    d1e6: f7fc fe53    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #-0x335a
    d1ea: 4650         	mov	r0, r10
    d1ec: f7f6 edb4    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x9498
    d1f0: 9816         	ldr	r0, [sp, #0x58]
    d1f2: 9b02         	ldr	r3, [sp, #0x8]
    d1f4: 4298         	cmp	r0, r3
    d1f6: d003         	beq	0xd200 <VolumeManager::muteSoundOut(LSMessage&)+0x278> @ imm = #0x6
    d1f8: 9918         	ldr	r1, [sp, #0x60]
    d1fa: 3101         	adds	r1, #0x1
    d1fc: f7f6 ee80    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x9300
    d200: 4638         	mov	r0, r7
    d202: f7f6 edaa    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x94ac
    d206: 9806         	ldr	r0, [sp, #0x18]
    d208: b108         	cbz	r0, 0xd20e <VolumeManager::muteSoundOut(LSMessage&)+0x286> @ imm = #0x2
    d20a: f7f6 ef20    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x91c0
    d20e: 9810         	ldr	r0, [sp, #0x40]
    d210: 9b01         	ldr	r3, [sp, #0x4]
    d212: 4298         	cmp	r0, r3
    d214: d003         	beq	0xd21e <VolumeManager::muteSoundOut(LSMessage&)+0x296> @ imm = #0x6
    d216: 9912         	ldr	r1, [sp, #0x48]
    d218: 3101         	adds	r1, #0x1
    d21a: f7f6 ee72    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x931c
    d21e: 2001         	movs	r0, #0x1
    d220: b023         	add	sp, #0x8c
    d222: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    d226: 4623         	mov	r3, r4
    d228: 2800         	cmp	r0, #0x0
    d22a: f43f aef8    	beq.w	0xd01e <VolumeManager::muteSoundOut(LSMessage&)+0x96> @ imm = #-0x210
    d22e: 4620         	mov	r0, r4
    d230: e010         	b	0xd254 <VolumeManager::muteSoundOut(LSMessage&)+0x2cc> @ imm = #0x20
    d232: 497c         	ldr	r1, [pc, #0x1f0]        @ 0xd424 <VolumeManager::muteSoundOut(LSMessage&)+0x49c> literal=0x14152
    d234: 462b         	mov	r3, r5
    d236: 2203         	movs	r2, #0x3
    d238: a806         	add	r0, sp, #0x18
    d23a: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    d23c: 311c         	adds	r1, #0x1c
    d23e: f7fc fd05    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x35f6
    d242: e7d5         	b	0xd1f0 <VolumeManager::muteSoundOut(LSMessage&)+0x268> @ imm = #-0x56
    d244: 2200         	movs	r2, #0x0
    d246: a90d         	add	r1, sp, #0x34
    d248: 4630         	mov	r0, r6
    d24a: f7f6 ef14    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x91d8
    d24e: 9b0d         	ldr	r3, [sp, #0x34]
    d250: 901c         	str	r0, [sp, #0x70]
    d252: 931e         	str	r3, [sp, #0x78]
    d254: 462a         	mov	r2, r5
    d256: 4659         	mov	r1, r11
    d258: f7f6 ee96    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x92d4
    d25c: 9d0d         	ldr	r5, [sp, #0x34]
    d25e: 9b1c         	ldr	r3, [sp, #0x70]
    d260: e6dd         	b	0xd01e <VolumeManager::muteSoundOut(LSMessage&)+0x96> @ imm = #-0x246
    d262: 2100         	movs	r1, #0x0
    d264: 4628         	mov	r0, r5
    d266: f7f6 edc6    	blx	0x3df4 <_ZN7pbnjson6JValueC1Eb@plt> @ imm = #-0x9474
    d26a: 4a6f         	ldr	r2, [pc, #0x1bc]        @ 0xd428 <VolumeManager::muteSoundOut(LSMessage&)+0x4a0> literal=0x4a8
    d26c: 4623         	mov	r3, r4
    d26e: 941c         	str	r4, [sp, #0x70]
    d270: f04f 0e0b    	mov.w	lr, #0xb
    d274: f04f 0c00    	mov.w	r12, #0x0
    d278: 447a         	add	r2, pc => raw=0xd724 ghidra=0x1d724 file=55076 "returnValue"
    d27a: ca07         	ldm	r2, {r0, r1, r2}
    d27c: c303         	stm	r3!, {r0, r1}
    d27e: 4631         	mov	r1, r6
    d280: f823 2b02    	strh	r2, [r3], #2
    d284: 0c12         	lsrs	r2, r2, #0x10
    d286: 4650         	mov	r0, r10
    d288: 701a         	strb	r2, [r3]
    d28a: 462a         	mov	r2, r5
    d28c: f8cd e074    	str.w	lr, [sp, #0x74]
    d290: f88d c083    	strb.w	r12, [sp, #0x83]
    d294: f7f6 ed1e    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x95c4
    d298: 981c         	ldr	r0, [sp, #0x70]
    d29a: 42a0         	cmp	r0, r4
    d29c: d003         	beq	0xd2a6 <VolumeManager::muteSoundOut(LSMessage&)+0x31e> @ imm = #0x6
    d29e: 991e         	ldr	r1, [sp, #0x78]
    d2a0: 3101         	adds	r1, #0x1
    d2a2: f7f6 ee2e    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x93a4
    d2a6: 4628         	mov	r0, r5
    d2a8: f7f6 ed56    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x9554
    d2ac: 495f         	ldr	r1, [pc, #0x17c]        @ 0xd42c <VolumeManager::muteSoundOut(LSMessage&)+0x4a4> literal=0x140dc
    d2ae: 4628         	mov	r0, r5
    d2b0: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    d2b2: 3164         	adds	r1, #0x64
    d2b4: f7f6 ef3a    	blx	0x412c <_ZN7pbnjson6JValueC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@plt> @ imm = #-0x918c
    d2b8: 4a5d         	ldr	r2, [pc, #0x174]        @ 0xd430 <VolumeManager::muteSoundOut(LSMessage&)+0x4a8> literal=0x1392
    d2ba: 4623         	mov	r3, r4
    d2bc: 941c         	str	r4, [sp, #0x70]
    d2be: f04f 0e09    	mov.w	lr, #0x9
    d2c2: f04f 0c00    	mov.w	r12, #0x0
    d2c6: 447a         	add	r2, pc => raw=0xe65c ghidra=0x1e65c file=58972 "errorText"
    d2c8: ca07         	ldm	r2, {r0, r1, r2}
    d2ca: c303         	stm	r3!, {r0, r1}
    d2cc: 4631         	mov	r1, r6
    d2ce: 701a         	strb	r2, [r3]
    d2d0: 4650         	mov	r0, r10
    d2d2: 462a         	mov	r2, r5
    d2d4: f8cd e074    	str.w	lr, [sp, #0x74]
    d2d8: f88d c081    	strb.w	r12, [sp, #0x81]
    d2dc: f7f6 ecfa    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x960c
    d2e0: 981c         	ldr	r0, [sp, #0x70]
    d2e2: 42a0         	cmp	r0, r4
    d2e4: d003         	beq	0xd2ee <VolumeManager::muteSoundOut(LSMessage&)+0x366> @ imm = #0x6
    d2e6: 991e         	ldr	r1, [sp, #0x78]
    d2e8: 3101         	adds	r1, #0x1
    d2ea: f7f6 ee0a    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x93ec
    d2ee: 4628         	mov	r0, r5
    d2f0: f7f6 ed32    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x959c
    d2f4: 2114         	movs	r1, #0x14
    d2f6: 4628         	mov	r0, r5
    d2f8: f7f6 ef1e    	blx	0x4138 <_ZN7pbnjson6JValueC1Ei@plt> @ imm = #-0x91c4
    d2fc: 4a4d         	ldr	r2, [pc, #0x134]        @ 0xd434 <VolumeManager::muteSoundOut(LSMessage&)+0x4ac> literal=0x135a
    d2fe: 4623         	mov	r3, r4
    d300: 941c         	str	r4, [sp, #0x70]
    d302: f04f 0e09    	mov.w	lr, #0x9
    d306: f04f 0c00    	mov.w	r12, #0x0
    d30a: 447a         	add	r2, pc => raw=0xe668 ghidra=0x1e668 file=58984 "errorCode"
    d30c: ca07         	ldm	r2, {r0, r1, r2}
    d30e: c303         	stm	r3!, {r0, r1}
    d310: 4631         	mov	r1, r6
    d312: 701a         	strb	r2, [r3]
    d314: 4650         	mov	r0, r10
    d316: 462a         	mov	r2, r5
    d318: f8cd e074    	str.w	lr, [sp, #0x74]
    d31c: f88d c081    	strb.w	r12, [sp, #0x81]
    d320: f7f6 ecd8    	blx	0x3cd4 <_ZN7pbnjson6JValue3putERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKS0_@plt> @ imm = #-0x9650
    d324: e753         	b	0xd1ce <VolumeManager::muteSoundOut(LSMessage&)+0x246> @ imm = #-0x15a
    d326: b132         	cbz	r2, 0xd336 <VolumeManager::muteSoundOut(LSMessage&)+0x3ae> @ imm = #0xc
    d328: 2a01         	cmp	r2, #0x1
    d32a: d01a         	beq	0xd362 <VolumeManager::muteSoundOut(LSMessage&)+0x3da> @ imm = #0x34
    d32c: 4621         	mov	r1, r4
    d32e: f7f6 ee2c    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x93a8
    d332: 9a1d         	ldr	r2, [sp, #0x74]
    d334: 9810         	ldr	r0, [sp, #0x40]
    d336: 2300         	movs	r3, #0x0
    d338: 9211         	str	r2, [sp, #0x44]
    d33a: 5483         	strb	r3, [r0, r2]
    d33c: 981c         	ldr	r0, [sp, #0x70]
    d33e: e6ab         	b	0xd098 <VolumeManager::muteSoundOut(LSMessage&)+0x110> @ imm = #-0x2aa
    d340: 9810         	ldr	r0, [sp, #0x40]
    d342: f8d8 1008    	ldr.w	r1, [r8, #0x8]
    d346: b122         	cbz	r2, 0xd352 <VolumeManager::muteSoundOut(LSMessage&)+0x3ca> @ imm = #0x8
    d348: f7f6 ef02    	blx	0x4150 <memcmp@plt>     @ imm = #-0x91fc
    d34c: 2800         	cmp	r0, #0x0
    d34e: f47f aee0    	bne.w	0xd112 <VolumeManager::muteSoundOut(LSMessage&)+0x18a> @ imm = #-0x240
    d352: 9b03         	ldr	r3, [sp, #0xc]
    d354: f888 3020    	strb.w	r3, [r8, #0x20]
    d358: e6db         	b	0xd112 <VolumeManager::muteSoundOut(LSMessage&)+0x18a> @ imm = #-0x24a
    d35a: 941c         	str	r4, [sp, #0x70]
    d35c: ac1e         	add	r4, sp, #0x78
    d35e: 4620         	mov	r0, r4
    d360: e69a         	b	0xd098 <VolumeManager::muteSoundOut(LSMessage&)+0x110> @ imm = #-0x2cc
    d362: f89d 3078    	ldrb.w	r3, [sp, #0x78]
    d366: 7003         	strb	r3, [r0]
    d368: 9a1d         	ldr	r2, [sp, #0x74]
    d36a: 9810         	ldr	r0, [sp, #0x40]
    d36c: e7e3         	b	0xd336 <VolumeManager::muteSoundOut(LSMessage&)+0x3ae> @ imm = #-0x3a
    d36e: 4832         	ldr	r0, [pc, #0xc8]         @ 0xd438 <VolumeManager::muteSoundOut(LSMessage&)+0x4b0> literal=0x324
    d370: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    d372: f7f6 ecc8    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x9670
    d376: f10d 0b40    	add.w	r11, sp, #0x40
    d37a: 4658         	mov	r0, r11
    d37c: f7f6 eeb8    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x9290
    d380: f7f6 eefe    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x9204
    d384: f10d 0b40    	add.w	r11, sp, #0x40
    d388: 4648         	mov	r0, r9
    d38a: f7f6 eeb2    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x929c
    d38e: 4638         	mov	r0, r7
    d390: f7f6 ece2    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x963c
    d394: 9806         	ldr	r0, [sp, #0x18]
    d396: 2800         	cmp	r0, #0x0
    d398: d0ef         	beq	0xd37a <VolumeManager::muteSoundOut(LSMessage&)+0x3f2> @ imm = #-0x22
    d39a: f7f6 ee58    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x9350
    d39e: e7ec         	b	0xd37a <VolumeManager::muteSoundOut(LSMessage&)+0x3f2> @ imm = #-0x28
    d3a0: f10d 0b40    	add.w	r11, sp, #0x40
    d3a4: e7f3         	b	0xd38e <VolumeManager::muteSoundOut(LSMessage&)+0x406> @ imm = #-0x1a
    d3a6: 4630         	mov	r0, r6
    d3a8: f10d 0b40    	add.w	r11, sp, #0x40
    d3ac: f7f6 eea0    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x92c0
    d3b0: e7ea         	b	0xd388 <VolumeManager::muteSoundOut(LSMessage&)+0x400> @ imm = #-0x2c
    d3b2: 4630         	mov	r0, r6
    d3b4: f10d 0b40    	add.w	r11, sp, #0x40
    d3b8: f7f6 ee9a    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x92cc
    d3bc: 4b15         	ldr	r3, [pc, #0x54]         @ 0xd414 <VolumeManager::muteSoundOut(LSMessage&)+0x48c> literal=0x210
    d3be: 4628         	mov	r0, r5
    d3c0: f85a 3003    	ldr.w	r3, [r10, r3]
    d3c4: 3308         	adds	r3, #0x8
    d3c6: 930d         	str	r3, [sp, #0x34]
    d3c8: f7f6 edc0    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x9480
    d3cc: e7dc         	b	0xd388 <VolumeManager::muteSoundOut(LSMessage&)+0x400> @ imm = #-0x48
    d3ce: 9806         	ldr	r0, [sp, #0x18]
    d3d0: f10d 0b40    	add.w	r11, sp, #0x40
    d3d4: e7e1         	b	0xd39a <VolumeManager::muteSoundOut(LSMessage&)+0x412> @ imm = #-0x3e
    d3d6: f10a 0308    	add.w	r3, r10, #0x8
    d3da: 4630         	mov	r0, r6
    d3dc: f10d 0b40    	add.w	r11, sp, #0x40
    d3e0: 931c         	str	r3, [sp, #0x70]
    d3e2: f7f6 edb4    	blx	0x3f4c <_ZN7pbnjson6JValueD2Ev@plt> @ imm = #-0x9498
    d3e6: e7cf         	b	0xd388 <VolumeManager::muteSoundOut(LSMessage&)+0x400> @ imm = #-0x62
    d3e8: 4650         	mov	r0, r10
    d3ea: f7f6 ecb6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x9694
    d3ee: e7cb         	b	0xd388 <VolumeManager::muteSoundOut(LSMessage&)+0x400> @ imm = #-0x6a
    d3f0: 4630         	mov	r0, r6
    d3f2: f7f6 ee7e    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x9304
    d3f6: 4628         	mov	r0, r5
    d3f8: f7f6 ecae    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x96a4
    d3fc: e7f4         	b	0xd3e8 <VolumeManager::muteSoundOut(LSMessage&)+0x460> @ imm = #-0x18
    d3fe: e7f7         	b	0xd3f0 <VolumeManager::muteSoundOut(LSMessage&)+0x468> @ imm = #-0x12
    d400: e7f6         	b	0xd3f0 <VolumeManager::muteSoundOut(LSMessage&)+0x468> @ imm = #-0x14
    d402: e7f5         	b	0xd3f0 <VolumeManager::muteSoundOut(LSMessage&)+0x468> @ imm = #-0x16
    d404: e7f4         	b	0xd3f0 <VolumeManager::muteSoundOut(LSMessage&)+0x468> @ imm = #-0x18
    d406: e7f3         	b	0xd3f0 <VolumeManager::muteSoundOut(LSMessage&)+0x468> @ imm = #-0x1a
    d408: 3e20         	subs	r6, #0x20
    d40a: 0001         	movs	r1, r0
    d40c: 2194         	movs	r1, #0x94
    d40e: 0000         	movs	r0, r0
    d410: 1fae         	subs	r6, r5, #0x6
    d412: 0000         	movs	r0, r0
    d414: 0210         	lsls	r0, r2, #0x8
    d416: 0000         	movs	r0, r0
    d418: 1124         	asrs	r4, r4, #0x4
    d41a: 0000         	movs	r0, r0
    d41c: 05f8         	lsls	r0, r7, #0x17
    d41e: 0000         	movs	r0, r0
    d420: 1e8a         	subs	r2, r1, #0x2
    d422: 0000         	movs	r0, r0
    d424: 4152         	adcs	r2, r2
    d426: 0001         	movs	r1, r0
    d428: 04a8         	lsls	r0, r5, #0x12
    d42a: 0000         	movs	r0, r0
    d42c: 40dc         	lsrs	r4, r3
    d42e: 0001         	movs	r1, r0
    d430: 1392         	asrs	r2, r2, #0xe
    d432: 0000         	movs	r0, r0
    d434: 135a         	asrs	r2, r3, #0xd
    d436: 0000         	movs	r0, r0
    d438: 0324         	lsls	r4, r4, #0xc
    d43a: 0000         	movs	r0, r0

0000d43c <VolumeManager::getStatus(LSMessage&)>:
    d43c: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    d440: b093         	sub	sp, #0x4c
    d442: 4607         	mov	r7, r0
    d444: ae03         	add	r6, sp, #0xc
    d446: 4608         	mov	r0, r1
    d448: 460c         	mov	r4, r1
    d44a: 9100         	str	r1, [sp]
    d44c: f7f6 ee4a    	blx	0x40e4 <LSMessageRef@plt> @ imm = #-0x936c
    d450: 4630         	mov	r0, r6
    d452: f7f6 eee4    	blx	0x421c <_ZN7pbnjson6JValueC1Ev@plt> @ imm = #-0x9238
    d456: f10d 0818    	add.w	r8, sp, #0x18
    d45a: ad0c         	add	r5, sp, #0x30
    d45c: 2200         	movs	r2, #0x0
    d45e: f10d 0920    	add.w	r9, sp, #0x20
    d462: 233e         	movs	r3, #0x3e
    d464: 4629         	mov	r1, r5
    d466: 4640         	mov	r0, r8
    d468: 9201         	str	r2, [sp, #0x4]
    d46a: f8cd 9018    	str.w	r9, [sp, #0x18]
    d46e: 930c         	str	r3, [sp, #0x30]
    d470: f7f6 ee00    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x9400
    d474: f8df e150    	ldr.w	lr, [pc, #0x150]        @ 0xd5c8 <VolumeManager::getStatus(LSMessage&)+0x18c> literal=0xf1e
    d478: 4684         	mov	r12, r0
    d47a: 9b0c         	ldr	r3, [sp, #0x30]
    d47c: 9006         	str	r0, [sp, #0x18]
    d47e: 44fe         	add	lr, pc => raw=0xe3a0 ghidra=0x1e3a0 file=58272 "{\"type\":\"object\",\"properties\":{},\"additionalProperties\":false}"
    d480: f10e 0b30    	add.w	r11, lr, #0x30
    d484: 9308         	str	r3, [sp, #0x20]
    d486: 46f2         	mov	r10, lr
    d488: f10c 0c10    	add.w	r12, r12, #0x10
    d48c: e8ba 000f    	ldm.w	r10!, {r0, r1, r2, r3}
    d490: f10e 0e10    	add.w	lr, lr, #0x10
    d494: 45da         	cmp	r10, r11
    d496: f84c 0c10    	str	r0, [r12, #-16]
    d49a: f84c 1c0c    	str	r1, [r12, #-12]
    d49e: f84c 2c08    	str	r2, [r12, #-8]
    d4a2: f84c 3c04    	str	r3, [r12, #-4]
    d4a6: d1ee         	bne	0xd486 <VolumeManager::getStatus(LSMessage&)+0x4a> @ imm = #-0x24
    d4a8: 4673         	mov	r3, lr
    d4aa: f04f 0e00    	mov.w	lr, #0x0
    d4ae: cb07         	ldm	r3!, {r0, r1, r2}
    d4b0: 881b         	ldrh	r3, [r3]
    d4b2: f8cc 0000    	str.w	r0, [r12]
    d4b6: 4620         	mov	r0, r4
    d4b8: f8cc 1004    	str.w	r1, [r12, #0x4]
    d4bc: f8cc 2008    	str.w	r2, [r12, #0x8]
    d4c0: f8ac 300c    	strh.w	r3, [r12, #0xc]
    d4c4: 9b0c         	ldr	r3, [sp, #0x30]
    d4c6: 9a06         	ldr	r2, [sp, #0x18]
    d4c8: 9307         	str	r3, [sp, #0x1c]
    d4ca: f802 e003    	strb.w	lr, [r2, r3]
    d4ce: f7f6 ee64    	blx	0x4198 <LSMessageGetPayload@plt> @ imm = #-0x9338
    d4d2: f10d 0b38    	add.w	r11, sp, #0x38
    d4d6: 4682         	mov	r10, r0
    d4d8: f8cd b030    	str.w	r11, [sp, #0x30]
    d4dc: 2800         	cmp	r0, #0x0
    d4de: d058         	beq	0xd592 <VolumeManager::getStatus(LSMessage&)+0x156> @ imm = #0xb0
    d4e0: f7f6 ede0    	blx	0x40a4 <strlen@plt>     @ imm = #-0x9440
    d4e4: 280f         	cmp	r0, #0xf
    d4e6: 4604         	mov	r4, r0
    d4e8: 9002         	str	r0, [sp, #0x8]
    d4ea: d843         	bhi	0xd574 <VolumeManager::getStatus(LSMessage&)+0x138> @ imm = #0x86
    d4ec: 2801         	cmp	r0, #0x1
    d4ee: d133         	bne	0xd558 <VolumeManager::getStatus(LSMessage&)+0x11c> @ imm = #0x66
    d4f0: f89a 3000    	ldrb.w	r3, [r10]
    d4f4: f88d 3038    	strb.w	r3, [sp, #0x38]
    d4f8: 465b         	mov	r3, r11
    d4fa: 2200         	movs	r2, #0x0
    d4fc: 940d         	str	r4, [sp, #0x34]
    d4fe: 4631         	mov	r1, r6
    d500: 551a         	strb	r2, [r3, r4]
    d502: 4628         	mov	r0, r5
    d504: ab01         	add	r3, sp, #0x4
    d506: 4642         	mov	r2, r8
    d508: f7fc ff66    	bl	0xa3d8 <void std::vector<AudioConnection, std::allocator<AudioConnection>>::_M_realloc_insert<AudioConnection>(__gnu_cxx::__normal_iterator<AudioConnection*, std::vector<AudioConnection, std::allocator<AudioConnection>>>, AudioConnection&&)+0x390> @ imm = #-0x3134
    d50c: 4604         	mov	r4, r0
    d50e: 980c         	ldr	r0, [sp, #0x30]
    d510: 4558         	cmp	r0, r11
    d512: d003         	beq	0xd51c <VolumeManager::getStatus(LSMessage&)+0xe0> @ imm = #0x6
    d514: 990e         	ldr	r1, [sp, #0x38]
    d516: 3101         	adds	r1, #0x1
    d518: f7f6 ecf2    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x961c
    d51c: b30c         	cbz	r4, 0xd562 <VolumeManager::getStatus(LSMessage&)+0x126> @ imm = #0x42
    d51e: 4639         	mov	r1, r7
    d520: 4628         	mov	r0, r5
    d522: f7fe fdb1    	bl	0xc088 <VolumeManager::getStatus()> @ imm = #-0x149e
    d526: 4629         	mov	r1, r5
    d528: 4668         	mov	r0, sp
    d52a: f7fc fcb1    	bl	0x9e90 <ConnectionManager::removePipelineInfo(LSMessage&)+0x8c0> @ imm = #-0x369e
    d52e: 4628         	mov	r0, r5
    d530: f7f6 ec12    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x97dc
    d534: 9806         	ldr	r0, [sp, #0x18]
    d536: 4548         	cmp	r0, r9
    d538: d003         	beq	0xd542 <VolumeManager::getStatus(LSMessage&)+0x106> @ imm = #0x6
    d53a: 9908         	ldr	r1, [sp, #0x20]
    d53c: 3101         	adds	r1, #0x1
    d53e: f7f6 ece0    	blx	0x3f00 <_ZdlPvj@plt>    @ imm = #-0x9640
    d542: 4630         	mov	r0, r6
    d544: f7f6 ec08    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x97f0
    d548: 9800         	ldr	r0, [sp]
    d54a: b108         	cbz	r0, 0xd550 <VolumeManager::getStatus(LSMessage&)+0x114> @ imm = #0x2
    d54c: f7f6 ed7e    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x9504
    d550: 2001         	movs	r0, #0x1
    d552: b013         	add	sp, #0x4c
    d554: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    d558: 465b         	mov	r3, r11
    d55a: 2800         	cmp	r0, #0x0
    d55c: d0cd         	beq	0xd4fa <VolumeManager::getStatus(LSMessage&)+0xbe> @ imm = #-0x66
    d55e: 4658         	mov	r0, r11
    d560: e010         	b	0xd584 <VolumeManager::getStatus(LSMessage&)+0x148> @ imm = #0x20
    d562: 491a         	ldr	r1, [pc, #0x68]         @ 0xd5cc <VolumeManager::getStatus(LSMessage&)+0x190> literal=0x13e22
    d564: 4623         	mov	r3, r4
    d566: 2203         	movs	r2, #0x3
    d568: 4668         	mov	r0, sp
    d56a: 4479         	add	r1, pc => raw=0x21390 ghidra=0x31390 file=null
    d56c: 311c         	adds	r1, #0x1c
    d56e: f7fc fb6d    	bl	0x9c4c <ConnectionManager::removePipelineInfo(LSMessage&)+0x67c> @ imm = #-0x3926
    d572: e7df         	b	0xd534 <VolumeManager::getStatus(LSMessage&)+0xf8> @ imm = #-0x42
    d574: 2200         	movs	r2, #0x0
    d576: a902         	add	r1, sp, #0x8
    d578: 4628         	mov	r0, r5
    d57a: f7f6 ed7c    	blx	0x4074 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj@plt> @ imm = #-0x9508
    d57e: 9b02         	ldr	r3, [sp, #0x8]
    d580: 900c         	str	r0, [sp, #0x30]
    d582: 930e         	str	r3, [sp, #0x38]
    d584: 4622         	mov	r2, r4
    d586: 4651         	mov	r1, r10
    d588: f7f6 ecfe    	blx	0x3f88 <memcpy@plt>     @ imm = #-0x9604
    d58c: 9c02         	ldr	r4, [sp, #0x8]
    d58e: 9b0c         	ldr	r3, [sp, #0x30]
    d590: e7b3         	b	0xd4fa <VolumeManager::getStatus(LSMessage&)+0xbe> @ imm = #-0x9a
    d592: 480f         	ldr	r0, [pc, #0x3c]         @ 0xd5d0 <VolumeManager::getStatus(LSMessage&)+0x194> literal=0x100
    d594: 4478         	add	r0, pc => raw=0xd698 ghidra=0x1d698 file=54936 "basic_string::_M_construct null not valid"
    d596: f7f6 ebb6    	blx	0x3d04 <_ZSt19__throw_logic_errorPKc@plt> @ imm = #-0x9894
    d59a: 4628         	mov	r0, r5
    d59c: f7f6 ebdc    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x9848
    d5a0: 4640         	mov	r0, r8
    d5a2: f7f6 eda6    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x94b4
    d5a6: 4630         	mov	r0, r6
    d5a8: f7f6 ebd6    	blx	0x3d58 <_ZN7pbnjson6JValueD1Ev@plt> @ imm = #-0x9854
    d5ac: 9800         	ldr	r0, [sp]
    d5ae: b108         	cbz	r0, 0xd5b4 <VolumeManager::getStatus(LSMessage&)+0x178> @ imm = #0x2
    d5b0: f7f6 ed4c    	blx	0x404c <LSMessageUnref@plt> @ imm = #-0x9568
    d5b4: f7f6 ede4    	blx	0x4180 <__cxa_end_cleanup@plt> @ imm = #-0x9438
    d5b8: 9800         	ldr	r0, [sp]
    d5ba: e7f9         	b	0xd5b0 <VolumeManager::getStatus(LSMessage&)+0x174> @ imm = #-0xe
    d5bc: e7f0         	b	0xd5a0 <VolumeManager::getStatus(LSMessage&)+0x164> @ imm = #-0x20
    d5be: e7f2         	b	0xd5a6 <VolumeManager::getStatus(LSMessage&)+0x16a> @ imm = #-0x1c
    d5c0: 4628         	mov	r0, r5
    d5c2: f7f6 ed96    	blx	0x40f0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@plt> @ imm = #-0x94d4
    d5c6: e7eb         	b	0xd5a0 <VolumeManager::getStatus(LSMessage&)+0x164> @ imm = #-0x2a
    d5c8: 0f1e         	lsrs	r6, r3, #0x1c
    d5ca: 0000         	movs	r0, r0
    d5cc: 3e22         	subs	r6, #0x22
    d5ce: 0001         	movs	r1, r0
    d5d0: 0100         	lsls	r0, r0, #0x4
    d5d2: 0000         	movs	r0, r0
    d5d4: 4610         	mov	r0, r2
    d5d6: f7fe bdb3    	b.w	0xc140 <VolumeManager::set(LSMessage&)> @ imm = #-0x149a
    d5da: bf00         	nop
    d5dc: 4610         	mov	r0, r2
    d5de: f7ff b81f    	b.w	0xc620 <VolumeManager::up(LSMessage&)> @ imm = #-0xfc2
    d5e2: bf00         	nop
    d5e4: 4610         	mov	r0, r2
    d5e6: f7ff ba6f    	b.w	0xcac8 <VolumeManager::down(LSMessage&)> @ imm = #-0xb22
    d5ea: bf00         	nop
    d5ec: 4610         	mov	r0, r2
    d5ee: f7ff bccb    	b.w	0xcf88 <VolumeManager::muteSoundOut(LSMessage&)> @ imm = #-0x66a
    d5f2: bf00         	nop
    d5f4: 4610         	mov	r0, r2
    d5f6: f7ff bf21    	b.w	0xd43c <VolumeManager::getStatus(LSMessage&)> @ imm = #-0x1be
    d5fa: bf00         	nop

Disassembly of section .fini:

0000d5fc <.fini>:
    d5fc: 4008         	ands	r0, r1
    d5fe: e92d 8008    	<unknown>
    d602: e8bd         	<unknown>
