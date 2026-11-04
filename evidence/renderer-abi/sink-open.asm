
orig/usr/lib/gstreamer-1.0/libgstlgaudiosink.so:	file format elf32-littlearm

Disassembly of section .text:

00014be0 <lgao_open>:
   14be0: b5f0         	push	{r4, r5, r6, r7, lr}
   14be2: f44f 71f8    	mov.w	r1, #0x1f0
   14be6: b095         	sub	sp, #0x54
   14be8: 4d77         	ldr	r5, [pc, #0x1dc]        @ 0x14dc8 <lgao_open+0x1e8>
   14bea: 2001         	movs	r0, #0x1
   14bec: f7ee eefa    	blx	0x39e4 <calloc@plt>     @ imm = #-0x1120c
   14bf0: 4604         	mov	r4, r0
   14bf2: 447d         	add	r5, pc
   14bf4: f7ef e876    	blx	0x3ce4 <KADP_InitSystem@plt> @ imm = #-0x10f14
   14bf8: 4a74         	ldr	r2, [pc, #0x1d0]        @ 0x14dcc <lgao_open+0x1ec>
   14bfa: 462b         	mov	r3, r5
   14bfc: 58ad         	ldr	r5, [r5, r2]
   14bfe: 68eb         	ldr	r3, [r5, #0xc]
   14c00: 681b         	ldr	r3, [r3]
   14c02: 06d9         	lsls	r1, r3, #0x1b
   14c04: f100 80bb    	bmi.w	0x14d7e <lgao_open+0x19e> @ imm = #0x176
   14c08: f240 1201    	movw	r2, #0x101
   14c0c: ffc7 0e1f    	vmov.i8	d16, #0xff
   14c10: f8a4 2098    	strh.w	r2, [r4, #0x98]
   14c14: 2201         	movs	r2, #0x1
   14c16: f884 209a    	strb.w	r2, [r4, #0x9a]
   14c1a: f06f 4270    	mvn	r2, #0xf0000000
   14c1e: 2000         	movs	r0, #0x0
   14c20: f04f 33ff    	mov.w	r3, #0xffffffff
   14c24: f8c4 20a8    	str.w	r2, [r4, #0xa8]
   14c28: f44f 7160    	mov.w	r1, #0x380
   14c2c: f44f 727a    	mov.w	r2, #0x3e8
   14c30: 60a3         	str	r3, [r4, #0x8]
   14c32: f8c4 20d8    	str.w	r2, [r4, #0xd8]
   14c36: e9c4 3304    	strd	r3, r3, [r4, #16]
   14c3a: e9c4 0306    	strd	r0, r3, [r4, #24]
   14c3e: 6223         	str	r3, [r4, #0x20]
   14c40: 7120         	strb	r0, [r4, #0x4]
   14c42: 7320         	strb	r0, [r4, #0xc]
   14c44: f8c4 01e8    	str.w	r0, [r4, #0x1e8]
   14c48: f8a4 01dc    	strh.w	r0, [r4, #0x1dc]
   14c4c: f8c4 0080    	str.w	r0, [r4, #0x80]
   14c50: e9c4 0021    	strd	r0, r0, [r4, #132]
   14c54: f8c4 008c    	str.w	r0, [r4, #0x8c]
   14c58: edc4 0b2c    	vstr	d16, [r4, #176]
   14c5c: edc4 0b2e    	vstr	d16, [r4, #184]
   14c60: edc4 0b30    	vstr	d16, [r4, #192]
   14c64: f8c4 00a0    	str.w	r0, [r4, #0xa0]
   14c68: f8c4 00a4    	str.w	r0, [r4, #0xa4]
   14c6c: f884 00f0    	strb.w	r0, [r4, #0xf0]
   14c70: f8c4 012c    	str.w	r0, [r4, #0x12c]
   14c74: f884 0130    	strb.w	r0, [r4, #0x130]
   14c78: f7ee ee42    	blx	0x3900 <msgget@plt>     @ imm = #-0x1137c
   14c7c: 68eb         	ldr	r3, [r5, #0xc]
   14c7e: f8c4 01b0    	str.w	r0, [r4, #0x1b0]
   14c82: 681b         	ldr	r3, [r3]
   14c84: 071a         	lsls	r2, r3, #0x1c
   14c86: d46b         	bmi	0x14d60 <lgao_open+0x180> @ imm = #0xd6
   14c88: 4851         	ldr	r0, [pc, #0x144]        @ 0x14dd0 <lgao_open+0x1f0>
   14c8a: 2600         	movs	r6, #0x0
   14c8c: f04f 33ff    	mov.w	r3, #0xffffffff
   14c90: 2701         	movs	r7, #0x1
   14c92: 4631         	mov	r1, r6
   14c94: f884 70f4    	strb.w	r7, [r4, #0xf4]
   14c98: 4478         	add	r0, pc
   14c9a: e9c4 333e    	strd	r3, r3, [r4, #248]
   14c9e: e9c4 3340    	strd	r3, r3, [r4, #256]
   14ca2: e9c4 3342    	strd	r3, r3, [r4, #264]
   14ca6: e9c4 3344    	strd	r3, r3, [r4, #272]
   14caa: e9c4 3346    	strd	r3, r3, [r4, #280]
   14cae: e9c4 3348    	strd	r3, r3, [r4, #288]
   14cb2: f8c4 3128    	str.w	r3, [r4, #0x128]
   14cb6: f884 6131    	strb.w	r6, [r4, #0x131]
   14cba: f7ee ef74    	blx	0x3ba4 <access@plt>     @ imm = #-0x11118
   14cbe: b3b0         	cbz	r0, 0x14d2e <lgao_open+0x14e> @ imm = #0x6c
   14cc0: f894 3131    	ldrb.w	r3, [r4, #0x131]
   14cc4: f8c4 6134    	str.w	r6, [r4, #0x134]
   14cc8: bbab         	cbnz	r3, 0x14d36 <lgao_open+0x156> @ imm = #0x6a
   14cca: 2500         	movs	r5, #0x0
   14ccc: 225c         	movs	r2, #0x5c
   14cce: 4629         	mov	r1, r5
   14cd0: f104 0024    	add.w	r0, r4, #0x24
   14cd4: e9c4 554e    	strd	r5, r5, [r4, #312]
   14cd8: f7ee eea8    	blx	0x3a2c <memset@plt>     @ imm = #-0x112b0
   14cdc: f44f 7180    	mov.w	r1, #0x100
   14ce0: 2207         	movs	r2, #0x7
   14ce2: f04f 33ff    	mov.w	r3, #0xffffffff
   14ce6: f8c4 1140    	str.w	r1, [r4, #0x140]
   14cea: f504 70bc    	add.w	r0, r4, #0x178
   14cee: 4629         	mov	r1, r5
   14cf0: e9c4 5251    	strd	r5, r2, [r4, #324]
   14cf4: f8c4 214c    	str.w	r2, [r4, #0x14c]
   14cf8: e9c4 3555    	strd	r3, r5, [r4, #340]
   14cfc: 6023         	str	r3, [r4]
   14cfe: f8c4 51e4    	str.w	r5, [r4, #0x1e4]
   14d02: f8c4 515c    	str.w	r5, [r4, #0x15c]
   14d06: e9c4 5575    	strd	r5, r5, [r4, #468]
   14d0a: e9c4 5570    	strd	r5, r5, [r4, #448]
   14d0e: e9c4 5572    	strd	r5, r5, [r4, #456]
   14d12: f8a4 51d0    	strh.w	r5, [r4, #0x1d0]
   14d16: f8c4 51e0    	str.w	r5, [r4, #0x1e0]
   14d1a: f7ef ea70    	blx	0x41fc <pthread_cond_init@plt> @ imm = #-0x10b20
   14d1e: 4629         	mov	r1, r5
   14d20: f504 70b0    	add.w	r0, r4, #0x160
   14d24: f7ef e8aa    	blx	0x3e7c <pthread_mutex_init@plt> @ imm = #-0x10eac
   14d28: 4620         	mov	r0, r4
   14d2a: b015         	add	sp, #0x54
   14d2c: bdf0         	pop	{r4, r5, r6, r7, pc}
   14d2e: f884 7131    	strb.w	r7, [r4, #0x131]
   14d32: f8c4 0134    	str.w	r0, [r4, #0x134]
   14d36: 4e27         	ldr	r6, [pc, #0x9c]         @ 0x14dd4 <lgao_open+0x1f4>
   14d38: af04         	add	r7, sp, #0x10
   14d3a: 4927         	ldr	r1, [pc, #0x9c]         @ 0x14dd8 <lgao_open+0x1f8>
   14d3c: 4638         	mov	r0, r7
   14d3e: 447e         	add	r6, pc
   14d40: 6832         	ldr	r2, [r6]
   14d42: 4479         	add	r1, pc
   14d44: f7ef eada    	blx	0x42fc <sprintf@plt>    @ imm = #-0x10a4c
   14d48: 4924         	ldr	r1, [pc, #0x90]         @ 0x14ddc <lgao_open+0x1fc>
   14d4a: 4638         	mov	r0, r7
   14d4c: 4479         	add	r1, pc
   14d4e: f7ee ee56    	blx	0x39fc <fopen@plt>      @ imm = #-0x11354
   14d52: f8c4 0134    	str.w	r0, [r4, #0x134]
   14d56: b328         	cbz	r0, 0x14da4 <lgao_open+0x1c4> @ imm = #0x4a
   14d58: 6833         	ldr	r3, [r6]
   14d5a: 3301         	adds	r3, #0x1
   14d5c: 6033         	str	r3, [r6]
   14d5e: e7b4         	b	0x14cca <lgao_open+0xea> @ imm = #-0x98
   14d60: 4a1f         	ldr	r2, [pc, #0x7c]         @ 0x14de0 <lgao_open+0x200>
   14d62: 2103         	movs	r1, #0x3
   14d64: 4b1f         	ldr	r3, [pc, #0x7c]         @ 0x14de4 <lgao_open+0x204>
   14d66: e9cd 0401    	strd	r0, r4, [sp, #4]
   14d6a: 4628         	mov	r0, r5
   14d6c: 447a         	add	r2, pc
   14d6e: 447b         	add	r3, pc
   14d70: 32d8         	adds	r2, #0xd8
   14d72: 9300         	str	r3, [sp]
   14d74: f44f 73f1    	mov.w	r3, #0x1e2
   14d78: f7ef e918    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10dd0
   14d7c: e784         	b	0x14c88 <lgao_open+0xa8> @ imm = #-0xf8
   14d7e: 491a         	ldr	r1, [pc, #0x68]         @ 0x14de8 <lgao_open+0x208>
   14d80: 4628         	mov	r0, r5
   14d82: 4b1a         	ldr	r3, [pc, #0x68]         @ 0x14dec <lgao_open+0x20c>
   14d84: 4a1a         	ldr	r2, [pc, #0x68]         @ 0x14df0 <lgao_open+0x210>
   14d86: 4479         	add	r1, pc
   14d88: 447b         	add	r3, pc
   14d8a: e9cd 3101    	strd	r3, r1, [sp, #4]
   14d8e: 447a         	add	r2, pc
   14d90: 4918         	ldr	r1, [pc, #0x60]         @ 0x14df4 <lgao_open+0x214>
   14d92: 32d8         	adds	r2, #0xd8
   14d94: f240 13bb    	movw	r3, #0x1bb
   14d98: 4479         	add	r1, pc
   14d9a: 9100         	str	r1, [sp]
   14d9c: 2104         	movs	r1, #0x4
   14d9e: f7ef e906    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10df4
   14da2: e731         	b	0x14c08 <lgao_open+0x28> @ imm = #-0x19e
   14da4: 68eb         	ldr	r3, [r5, #0xc]
   14da6: 681b         	ldr	r3, [r3]
   14da8: 07db         	lsls	r3, r3, #0x1f
   14daa: d58e         	bpl	0x14cca <lgao_open+0xea> @ imm = #-0xe4
   14dac: 4a12         	ldr	r2, [pc, #0x48]         @ 0x14df8 <lgao_open+0x218>
   14dae: 4601         	mov	r1, r0
   14db0: 4b12         	ldr	r3, [pc, #0x48]         @ 0x14dfc <lgao_open+0x21c>
   14db2: 4628         	mov	r0, r5
   14db4: 9701         	str	r7, [sp, #0x4]
   14db6: 447a         	add	r2, pc
   14db8: 447b         	add	r3, pc
   14dba: 32e4         	adds	r2, #0xe4
   14dbc: 9300         	str	r3, [sp]
   14dbe: f44f 73c3    	mov.w	r3, #0x186
   14dc2: f7ef e8f4    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10e18
   14dc6: e780         	b	0x14cca <lgao_open+0xea> @ imm = #-0x100
   14dc8: e6 cf 01 00  	.word	0x0001cfe6
   14dcc: 00 04 00 00  	.word	0x00000400
   14dd0: f8 b4 00 00  	.word	0x0000b4f8
   14dd4: a6 d3 01 00  	.word	0x0001d3a6
   14dd8: 66 b4 00 00  	.word	0x0000b466
   14ddc: d4 5a 00 00  	.word	0x00005ad4
   14de0: ac c1 00 00  	.word	0x0000c1ac
   14de4: fa b3 00 00  	.word	0x0000b3fa
   14de8: 02 4c 00 00  	.word	0x00004c02
   14dec: 0c 4c 00 00  	.word	0x00004c0c
   14df0: 8a c1 00 00  	.word	0x0000c18a
   14df4: 2c 4c 00 00  	.word	0x00004c2c
   14df8: 62 c1 00 00  	.word	0x0000c162
   14dfc: 0c b4 00 00  	.word	0x0000b40c
