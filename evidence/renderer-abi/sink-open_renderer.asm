
orig/usr/lib/gstreamer-1.0/libgstlgaudiosink.so:	file format elf32-littlearm

Disassembly of section .text:

00014924 <_lgao_open_renderer>:
   14924: 7902         	ldrb	r2, [r0, #0x4]
   14926: b5f0         	push	{r4, r5, r6, r7, lr}
   14928: 4604         	mov	r4, r0
   1492a: 4e5f         	ldr	r6, [pc, #0x17c]        @ 0x14aa8 <_lgao_open_renderer+0x184>
   1492c: b085         	sub	sp, #0x14
   1492e: 6883         	ldr	r3, [r0, #0x8]
   14930: 447e         	add	r6, pc
   14932: b102         	cbz	r2, 0x14936 <_lgao_open_renderer+0x12> @ imm = #0x0
   14934: 3304         	adds	r3, #0x4
   14936: 485d         	ldr	r0, [pc, #0x174]        @ 0x14aac <_lgao_open_renderer+0x188>
   14938: 2102         	movs	r1, #0x2
   1493a: f2c0 0108    	movt	r1, #0x8
   1493e: 9302         	str	r3, [sp, #0x8]
   14940: 4478         	add	r0, pc
   14942: f7ee ef24    	blx	0x378c <open@plt>       @ imm = #-0x111b8
   14946: 2800         	cmp	r0, #0x0
   14948: 6020         	str	r0, [r4]
   1494a: db19         	blt	0x14980 <_lgao_open_renderer+0x5c> @ imm = #0x32
   1494c: f245 2101    	movw	r1, #0x5201
   14950: aa02         	add	r2, sp, #0x8
   14952: f2c4 0104    	movt	r1, #0x4004
   14956: f7ef e984    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x10cf8
   1495a: f8d4 51b4    	ldr.w	r5, [r4, #0x1b4]
   1495e: b1cd         	cbz	r5, 0x14994 <_lgao_open_renderer+0x70> @ imm = #0x32
   14960: 4628         	mov	r0, r5
   14962: f7ef ea9e    	blx	0x3ea0 <aimc_get_processor_id@plt> @ imm = #-0x10ac4
   14966: f245 2128    	movw	r1, #0x5228
   1496a: 4603         	mov	r3, r0
   1496c: aa03         	add	r2, sp, #0xc
   1496e: 6820         	ldr	r0, [r4]
   14970: f2c4 0104    	movt	r1, #0x4004
   14974: 9303         	str	r3, [sp, #0xc]
   14976: f7ef e974    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x10d18
   1497a: 2000         	movs	r0, #0x0
   1497c: b005         	add	sp, #0x14
   1497e: bdf0         	pop	{r4, r5, r6, r7, pc}
   14980: 4b4b         	ldr	r3, [pc, #0x12c]        @ 0x14ab0 <_lgao_open_renderer+0x18c>
   14982: 58f4         	ldr	r4, [r6, r3]
   14984: 68e3         	ldr	r3, [r4, #0xc]
   14986: 681b         	ldr	r3, [r3]
   14988: 07df         	lsls	r7, r3, #0x1f
   1498a: d422         	bmi	0x149d2 <_lgao_open_renderer+0xae> @ imm = #0x44
   1498c: f04f 30ff    	mov.w	r0, #0xffffffff
   14990: b005         	add	sp, #0x14
   14992: bdf0         	pop	{r4, r5, r6, r7, pc}
   14994: 4b46         	ldr	r3, [pc, #0x118]        @ 0x14ab0 <_lgao_open_renderer+0x18c>
   14996: 58f7         	ldr	r7, [r6, r3]
   14998: 68fb         	ldr	r3, [r7, #0xc]
   1499a: 681b         	ldr	r3, [r3]
   1499c: 0718         	lsls	r0, r3, #0x1c
   1499e: d442         	bmi	0x14a26 <_lgao_open_renderer+0x102> @ imm = #0x84
   149a0: f7ef eac0    	blx	0x3f24 <aimc_create@plt> @ imm = #-0x10a80
   149a4: 4605         	mov	r5, r0
   149a6: 2800         	cmp	r0, #0x0
   149a8: d04a         	beq	0x14a40 <_lgao_open_renderer+0x11c> @ imm = #0x94
   149aa: f7ef ea38    	blx	0x3e1c <aimc_create_handler@plt> @ imm = #-0x10b90
   149ae: 4628         	mov	r0, r5
   149b0: f7ee effa    	blx	0x39a8 <ModuleRegistry_Init@plt> @ imm = #-0x1100c
   149b4: 4606         	mov	r6, r0
   149b6: b340         	cbz	r0, 0x14a0a <_lgao_open_renderer+0xe6> @ imm = #0x50
   149b8: 4628         	mov	r0, r5
   149ba: f7ef ea72    	blx	0x3ea0 <aimc_get_processor_id@plt> @ imm = #-0x10b1c
   149be: 4629         	mov	r1, r5
   149c0: f7ee eeea    	blx	0x3798 <Connector_Create@plt> @ imm = #-0x1122c
   149c4: 2800         	cmp	r0, #0x0
   149c6: d04c         	beq	0x14a62 <_lgao_open_renderer+0x13e> @ imm = #0x98
   149c8: f8c4 51b4    	str.w	r5, [r4, #0x1b4]
   149cc: e9c4 606e    	strd	r6, r0, [r4, #440]
   149d0: e7c6         	b	0x14960 <_lgao_open_renderer+0x3c> @ imm = #-0x74
   149d2: 4d38         	ldr	r5, [pc, #0xe0]         @ 0x14ab4 <_lgao_open_renderer+0x190>
   149d4: f240 733a    	movw	r3, #0x73a
   149d8: 4a37         	ldr	r2, [pc, #0xdc]         @ 0x14ab8 <_lgao_open_renderer+0x194>
   149da: 2100         	movs	r1, #0x0
   149dc: 4620         	mov	r0, r4
   149de: 447d         	add	r5, pc
   149e0: 447a         	add	r2, pc
   149e2: 3584         	adds	r5, #0x84
   149e4: 9200         	str	r2, [sp]
   149e6: 462a         	mov	r2, r5
   149e8: f7ef eae0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10a40
   149ec: 68e3         	ldr	r3, [r4, #0xc]
   149ee: 681b         	ldr	r3, [r3]
   149f0: 07de         	lsls	r6, r3, #0x1f
   149f2: d5cb         	bpl	0x1498c <_lgao_open_renderer+0x68> @ imm = #-0x6a
   149f4: 4e31         	ldr	r6, [pc, #0xc4]         @ 0x14abc <_lgao_open_renderer+0x198>
   149f6: f240 734b    	movw	r3, #0x74b
   149fa: 462a         	mov	r2, r5
   149fc: 2100         	movs	r1, #0x0
   149fe: 4620         	mov	r0, r4
   14a00: 447e         	add	r6, pc
   14a02: 9600         	str	r6, [sp]
   14a04: f7ef ead2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10a5c
   14a08: e7c0         	b	0x1498c <_lgao_open_renderer+0x68> @ imm = #-0x80
   14a0a: 68fb         	ldr	r3, [r7, #0xc]
   14a0c: 681b         	ldr	r3, [r3]
   14a0e: 07da         	lsls	r2, r3, #0x1f
   14a10: d42f         	bmi	0x14a72 <_lgao_open_renderer+0x14e> @ imm = #0x5e
   14a12: 4628         	mov	r0, r5
   14a14: f7ef e9c0    	blx	0x3d98 <aimc_finalize@plt> @ imm = #-0x10c80
   14a18: f8d4 51b4    	ldr.w	r5, [r4, #0x1b4]
   14a1c: 4628         	mov	r0, r5
   14a1e: 2d00         	cmp	r5, #0x0
   14a20: d19e         	bne	0x14960 <_lgao_open_renderer+0x3c> @ imm = #-0xc4
   14a22: b005         	add	sp, #0x14
   14a24: bdf0         	pop	{r4, r5, r6, r7, pc}
   14a26: 4a26         	ldr	r2, [pc, #0x98]         @ 0x14ac0 <_lgao_open_renderer+0x19c>
   14a28: f240 1395    	movw	r3, #0x195
   14a2c: 4925         	ldr	r1, [pc, #0x94]         @ 0x14ac4 <_lgao_open_renderer+0x1a0>
   14a2e: 4638         	mov	r0, r7
   14a30: 447a         	add	r2, pc
   14a32: 4479         	add	r1, pc
   14a34: 3298         	adds	r2, #0x98
   14a36: 9100         	str	r1, [sp]
   14a38: 2103         	movs	r1, #0x3
   14a3a: f7ef eab8    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10a90
   14a3e: e7af         	b	0x149a0 <_lgao_open_renderer+0x7c> @ imm = #-0xa2
   14a40: 68fb         	ldr	r3, [r7, #0xc]
   14a42: 681b         	ldr	r3, [r3]
   14a44: 07d9         	lsls	r1, r3, #0x1f
   14a46: d5e7         	bpl	0x14a18 <_lgao_open_renderer+0xf4> @ imm = #-0x32
   14a48: 4a1f         	ldr	r2, [pc, #0x7c]         @ 0x14ac8 <_lgao_open_renderer+0x1a4>
   14a4a: 4601         	mov	r1, r0
   14a4c: 4b1f         	ldr	r3, [pc, #0x7c]         @ 0x14acc <_lgao_open_renderer+0x1a8>
   14a4e: 4638         	mov	r0, r7
   14a50: 447a         	add	r2, pc
   14a52: 447b         	add	r3, pc
   14a54: 3298         	adds	r2, #0x98
   14a56: 9300         	str	r3, [sp]
   14a58: f240 1399    	movw	r3, #0x199
   14a5c: f7ef eaa6    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10ab4
   14a60: e7da         	b	0x14a18 <_lgao_open_renderer+0xf4> @ imm = #-0x4c
   14a62: 68fb         	ldr	r3, [r7, #0xc]
   14a64: 681b         	ldr	r3, [r3]
   14a66: 07db         	lsls	r3, r3, #0x1f
   14a68: d410         	bmi	0x14a8c <_lgao_open_renderer+0x168> @ imm = #0x20
   14a6a: 4630         	mov	r0, r6
   14a6c: f7ee ef36    	blx	0x38dc <ModuleRegistry_Cleanup@plt> @ imm = #-0x11194
   14a70: e7cf         	b	0x14a12 <_lgao_open_renderer+0xee> @ imm = #-0x62
   14a72: 4a17         	ldr	r2, [pc, #0x5c]         @ 0x14ad0 <_lgao_open_renderer+0x1ac>
   14a74: 4601         	mov	r1, r0
   14a76: 4b17         	ldr	r3, [pc, #0x5c]         @ 0x14ad4 <_lgao_open_renderer+0x1b0>
   14a78: 4638         	mov	r0, r7
   14a7a: 447a         	add	r2, pc
   14a7c: 447b         	add	r3, pc
   14a7e: 3298         	adds	r2, #0x98
   14a80: 9300         	str	r3, [sp]
   14a82: f44f 73d0    	mov.w	r3, #0x1a0
   14a86: f7ef ea92    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10adc
   14a8a: e7c2         	b	0x14a12 <_lgao_open_renderer+0xee> @ imm = #-0x7c
   14a8c: 4a12         	ldr	r2, [pc, #0x48]         @ 0x14ad8 <_lgao_open_renderer+0x1b4>
   14a8e: 4601         	mov	r1, r0
   14a90: 4b12         	ldr	r3, [pc, #0x48]         @ 0x14adc <_lgao_open_renderer+0x1b8>
   14a92: 4638         	mov	r0, r7
   14a94: 447a         	add	r2, pc
   14a96: 447b         	add	r3, pc
   14a98: 3298         	adds	r2, #0x98
   14a9a: 9300         	str	r3, [sp]
   14a9c: f240 13a7    	movw	r3, #0x1a7
   14aa0: f7ef ea84    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10af8
   14aa4: e7e1         	b	0x14a6a <_lgao_open_renderer+0x146> @ imm = #-0x3e
   14aa6: bf00         	nop
   14aa8: a8 d2 01 00  	.word	0x0001d2a8
   14aac: 64 b7 00 00  	.word	0x0000b764
   14ab0: 00 04 00 00  	.word	0x00000400
   14ab4: 3a c5 00 00  	.word	0x0000c53a
   14ab8: d4 b6 00 00  	.word	0x0000b6d4
   14abc: 04 b7 00 00  	.word	0x0000b704
   14ac0: e8 c4 00 00  	.word	0x0000c4e8
   14ac4: 96 b6 00 00  	.word	0x0000b696
   14ac8: c8 c4 00 00  	.word	0x0000c4c8
   14acc: 82 b6 00 00  	.word	0x0000b682
   14ad0: 9e c4 00 00  	.word	0x0000c49e
   14ad4: 6c b6 00 00  	.word	0x0000b66c
   14ad8: 84 c4 00 00  	.word	0x0000c484
   14adc: 5e b6 00 00  	.word	0x0000b65e
