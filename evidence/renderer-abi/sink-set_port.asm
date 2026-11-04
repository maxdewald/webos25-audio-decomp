
/Users/max/projects/dts_restore_plus/re/webos25-audio-policy/bin/usr/lib/gstreamer-1.0/libgstlgaudiosink.so:	file format elf32-littlearm

Disassembly of section .text:

00015b58 <lgao_set_port>:
   15b58: f8df c198    	ldr.w	r12, [pc, #0x198]       @ 0x15cf4 <lgao_set_port+0x19c>
   15b5c: b570         	push	{r4, r5, r6, lr}
   15b5e: 4605         	mov	r5, r0
   15b60: f8df e194    	ldr.w	lr, [pc, #0x194]        @ 0x15cf8 <lgao_set_port+0x1a0>
   15b64: b084         	sub	sp, #0x10
   15b66: 44fc         	add	r12, pc
   15b68: f85c 600e    	ldr.w	r6, [r12, lr]
   15b6c: 68f0         	ldr	r0, [r6, #0xc]
   15b6e: 2d00         	cmp	r5, #0x0
   15b70: d075         	beq	0x15c5e <lgao_set_port+0x106> @ imm = #0xea
   15b72: ea01 0402    	and.w	r4, r1, r2
   15b76: 460b         	mov	r3, r1
   15b78: f1b4 3fff    	cmp.w	r4, #0xffffffff
   15b7c: f000 80a7    	beq.w	0x15cce <lgao_set_port+0x176> @ imm = #0x14e
   15b80: 2a06         	cmp	r2, #0x6
   15b82: bfd8         	it	le
   15b84: 2906         	cmple	r1, #0x6
   15b86: bfcc         	ite	gt
   15b88: 2101         	movgt	r1, #0x1
   15b8a: 2100         	movle	r1, #0x0
   15b8c: f300 8089    	bgt.w	0x15ca2 <lgao_set_port+0x14a> @ imm = #0x112
   15b90: 1c5c         	adds	r4, r3, #0x1
   15b92: d011         	beq	0x15bb8 <lgao_set_port+0x60> @ imm = #0x22
   15b94: 2b01         	cmp	r3, #0x1
   15b96: d907         	bls	0x15ba8 <lgao_set_port+0x50> @ imm = #0xe
   15b98: 6802         	ldr	r2, [r0]
   15b9a: 07d2         	lsls	r2, r2, #0x1f
   15b9c: d44d         	bmi	0x15c3a <lgao_set_port+0xe2> @ imm = #0x9a
   15b9e: f04f 34ff    	mov.w	r4, #0xffffffff
   15ba2: 4620         	mov	r0, r4
   15ba4: b004         	add	sp, #0x10
   15ba6: bd70         	pop	{r4, r5, r6, pc}
   15ba8: 60ab         	str	r3, [r5, #0x8]
   15baa: 6802         	ldr	r2, [r0]
   15bac: 06d0         	lsls	r0, r2, #0x1b
   15bae: d434         	bmi	0x15c1a <lgao_set_port+0xc2> @ imm = #0x68
   15bb0: 2400         	movs	r4, #0x0
   15bb2: 4620         	mov	r0, r4
   15bb4: b004         	add	sp, #0x10
   15bb6: bd70         	pop	{r4, r5, r6, pc}
   15bb8: 2a00         	cmp	r2, #0x0
   15bba: db19         	blt	0x15bf0 <lgao_set_port+0x98> @ imm = #0x32
   15bbc: 60aa         	str	r2, [r5, #0x8]
   15bbe: 6803         	ldr	r3, [r0]
   15bc0: 06db         	lsls	r3, r3, #0x1b
   15bc2: d45f         	bmi	0x15c84 <lgao_set_port+0x12c> @ imm = #0xbe
   15bc4: 2101         	movs	r1, #0x1
   15bc6: 4628         	mov	r0, r5
   15bc8: f7ed eedc    	blx	0x3984 <lgao_set_mode_mixer@plt> @ imm = #-0x12248
   15bcc: 68f3         	ldr	r3, [r6, #0xc]
   15bce: 681b         	ldr	r3, [r3]
   15bd0: 06dc         	lsls	r4, r3, #0x1b
   15bd2: d5ed         	bpl	0x15bb0 <lgao_set_port+0x58> @ imm = #-0x26
   15bd4: 4a49         	ldr	r2, [pc, #0x124]        @ 0x15cfc <lgao_set_port+0x1a4>
   15bd6: f240 4329    	movw	r3, #0x429
   15bda: 4c49         	ldr	r4, [pc, #0x124]        @ 0x15d00 <lgao_set_port+0x1a8>
   15bdc: 2104         	movs	r1, #0x4
   15bde: 4630         	mov	r0, r6
   15be0: 447a         	add	r2, pc
   15be2: 447c         	add	r4, pc
   15be4: f502 720b    	add.w	r2, r2, #0x22c
   15be8: 9400         	str	r4, [sp]
   15bea: f7ee e9e0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x11c40
   15bee: e7df         	b	0x15bb0 <lgao_set_port+0x58> @ imm = #-0x42
   15bf0: 6800         	ldr	r0, [r0]
   15bf2: 07c0         	lsls	r0, r0, #0x1f
   15bf4: bf58         	it	pl
   15bf6: 461c         	movpl	r4, r3
   15bf8: d5d3         	bpl	0x15ba2 <lgao_set_port+0x4a> @ imm = #-0x5a
   15bfa: f8df c108    	ldr.w	r12, [pc, #0x108]       @ 0x15d04 <lgao_set_port+0x1ac>
   15bfe: 461c         	mov	r4, r3
   15c00: 4d41         	ldr	r5, [pc, #0x104]        @ 0x15d08 <lgao_set_port+0x1b0>
   15c02: 4630         	mov	r0, r6
   15c04: 9201         	str	r2, [sp, #0x4]
   15c06: f240 432b    	movw	r3, #0x42b
   15c0a: 44fc         	add	r12, pc
   15c0c: 447d         	add	r5, pc
   15c0e: f50c 720b    	add.w	r2, r12, #0x22c
   15c12: 9500         	str	r5, [sp]
   15c14: f7ee e9ca    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x11c6c
   15c18: e7c3         	b	0x15ba2 <lgao_set_port+0x4a> @ imm = #-0x7a
   15c1a: 4a3c         	ldr	r2, [pc, #0xf0]         @ 0x15d0c <lgao_set_port+0x1b4>
   15c1c: 460c         	mov	r4, r1
   15c1e: 4d3c         	ldr	r5, [pc, #0xf0]         @ 0x15d10 <lgao_set_port+0x1b8>
   15c20: 4630         	mov	r0, r6
   15c22: 9301         	str	r3, [sp, #0x4]
   15c24: 2104         	movs	r1, #0x4
   15c26: 447a         	add	r2, pc
   15c28: f240 431f    	movw	r3, #0x41f
   15c2c: 447d         	add	r5, pc
   15c2e: f502 720b    	add.w	r2, r2, #0x22c
   15c32: 9500         	str	r5, [sp]
   15c34: f7ee e9ba    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x11c8c
   15c38: e7b3         	b	0x15ba2 <lgao_set_port+0x4a> @ imm = #-0x9a
   15c3a: 4a36         	ldr	r2, [pc, #0xd8]         @ 0x15d14 <lgao_set_port+0x1bc>
   15c3c: 4630         	mov	r0, r6
   15c3e: 4c36         	ldr	r4, [pc, #0xd8]         @ 0x15d18 <lgao_set_port+0x1c0>
   15c40: 9301         	str	r3, [sp, #0x4]
   15c42: f240 4321    	movw	r3, #0x421
   15c46: 447a         	add	r2, pc
   15c48: 447c         	add	r4, pc
   15c4a: f502 720b    	add.w	r2, r2, #0x22c
   15c4e: 9400         	str	r4, [sp]
   15c50: f04f 34ff    	mov.w	r4, #0xffffffff
   15c54: f7ee e9aa    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x11cac
   15c58: 4620         	mov	r0, r4
   15c5a: b004         	add	sp, #0x10
   15c5c: bd70         	pop	{r4, r5, r6, pc}
   15c5e: 6803         	ldr	r3, [r0]
   15c60: 07db         	lsls	r3, r3, #0x1f
   15c62: d59c         	bpl	0x15b9e <lgao_set_port+0x46> @ imm = #-0xc8
   15c64: 4a2d         	ldr	r2, [pc, #0xb4]         @ 0x15d1c <lgao_set_port+0x1c4>
   15c66: f240 430f    	movw	r3, #0x40f
   15c6a: 4c2d         	ldr	r4, [pc, #0xb4]         @ 0x15d20 <lgao_set_port+0x1c8>
   15c6c: 4629         	mov	r1, r5
   15c6e: 4630         	mov	r0, r6
   15c70: 447a         	add	r2, pc
   15c72: 447c         	add	r4, pc
   15c74: f502 720b    	add.w	r2, r2, #0x22c
   15c78: 9400         	str	r4, [sp]
   15c7a: f04f 34ff    	mov.w	r4, #0xffffffff
   15c7e: f7ee e996    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x11cd4
   15c82: e78e         	b	0x15ba2 <lgao_set_port+0x4a> @ imm = #-0xe4
   15c84: 4927         	ldr	r1, [pc, #0x9c]         @ 0x15d24 <lgao_set_port+0x1cc>
   15c86: 4630         	mov	r0, r6
   15c88: 4b27         	ldr	r3, [pc, #0x9c]         @ 0x15d28 <lgao_set_port+0x1d0>
   15c8a: 9201         	str	r2, [sp, #0x4]
   15c8c: 4479         	add	r1, pc
   15c8e: 447b         	add	r3, pc
   15c90: f501 720b    	add.w	r2, r1, #0x22c
   15c94: 9300         	str	r3, [sp]
   15c96: 2104         	movs	r1, #0x4
   15c98: f240 4327    	movw	r3, #0x427
   15c9c: f7ee e986    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x11cf4
   15ca0: e790         	b	0x15bc4 <lgao_set_port+0x6c> @ imm = #-0xe0
   15ca2: 6801         	ldr	r1, [r0]
   15ca4: 07cd         	lsls	r5, r1, #0x1f
   15ca6: f57f af7a    	bpl.w	0x15b9e <lgao_set_port+0x46> @ imm = #-0x10c
   15caa: 4920         	ldr	r1, [pc, #0x80]         @ 0x15d2c <lgao_set_port+0x1d4>
   15cac: 4630         	mov	r0, r6
   15cae: 4c20         	ldr	r4, [pc, #0x80]         @ 0x15d30 <lgao_set_port+0x1d8>
   15cb0: e9cd 3201    	strd	r3, r2, [sp, #4]
   15cb4: f240 4317    	movw	r3, #0x417
   15cb8: 4479         	add	r1, pc
   15cba: 447c         	add	r4, pc
   15cbc: f501 720b    	add.w	r2, r1, #0x22c
   15cc0: 9400         	str	r4, [sp]
   15cc2: 2100         	movs	r1, #0x0
   15cc4: f04f 34ff    	mov.w	r4, #0xffffffff
   15cc8: f7ee e970    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x11d20
   15ccc: e769         	b	0x15ba2 <lgao_set_port+0x4a> @ imm = #-0x12e
   15cce: 6803         	ldr	r3, [r0]
   15cd0: 07db         	lsls	r3, r3, #0x1f
   15cd2: f57f af66    	bpl.w	0x15ba2 <lgao_set_port+0x4a> @ imm = #-0x134
   15cd6: 4a17         	ldr	r2, [pc, #0x5c]         @ 0x15d34 <lgao_set_port+0x1dc>
   15cd8: f240 4312    	movw	r3, #0x412
   15cdc: 4916         	ldr	r1, [pc, #0x58]         @ 0x15d38 <lgao_set_port+0x1e0>
   15cde: 4630         	mov	r0, r6
   15ce0: 447a         	add	r2, pc
   15ce2: 4479         	add	r1, pc
   15ce4: f502 720b    	add.w	r2, r2, #0x22c
   15ce8: 9100         	str	r1, [sp]
   15cea: 2100         	movs	r1, #0x0
   15cec: f7ee e95e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x11d44
   15cf0: e757         	b	0x15ba2 <lgao_set_port+0x4a> @ imm = #-0x152
   15cf2: bf00         	nop
   15cf4: 72 c0 01 00  	.word	0x0001c072
   15cf8: 00 04 00 00  	.word	0x00000400
   15cfc: 38 b3 00 00  	.word	0x0000b338
   15d00: 82 a9 00 00  	.word	0x0000a982
   15d04: 0e b3 00 00  	.word	0x0000b30e
   15d08: 68 a9 00 00  	.word	0x0000a968
   15d0c: f2 b2 00 00  	.word	0x0000b2f2
   15d10: c8 a8 00 00  	.word	0x0000a8c8
   15d14: d2 b2 00 00  	.word	0x0000b2d2
   15d18: d4 a8 00 00  	.word	0x0000a8d4
   15d1c: a8 b2 00 00  	.word	0x0000b2a8
   15d20: 66 a5 00 00  	.word	0x0000a566
   15d24: 8c b2 00 00  	.word	0x0000b28c
   15d28: ae a8 00 00  	.word	0x0000a8ae
   15d2c: 60 b2 00 00  	.word	0x0000b260
   15d30: 1e a8 00 00  	.word	0x0000a81e
   15d34: 38 b2 00 00  	.word	0x0000b238
   15d38: de a7 00 00  	.word	0x0000a7de
