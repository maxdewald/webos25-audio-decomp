
orig/usr/lib/libkadaptor.so.2.0.1:	file format elf32-littlearm

Disassembly of section .text:

00061b00 <KADP_InitSystem>:
   61b00: e92d 47f0    	push.w	{r4, r5, r6, r7, r8, r9, r10, lr}
   61b04: b084         	sub	sp, #0x10
   61b06: 4c4b         	ldr	r4, [pc, #0x12c]        @ 0x61c34 <KADP_InitSystem+0x134>
   61b08: 4e4b         	ldr	r6, [pc, #0x12c]        @ 0x61c38 <KADP_InitSystem+0x138>
   61b0a: 447c         	add	r4, pc
   61b0c: 6823         	ldr	r3, [r4]
   61b0e: 447e         	add	r6, pc
   61b10: 2b00         	cmp	r3, #0x0
   61b12: d07f         	beq	0x61c14 <KADP_InitSystem+0x114> @ imm = #0xfe
   61b14: 4b49         	ldr	r3, [pc, #0x124]        @ 0x61c3c <KADP_InitSystem+0x13c>
   61b16: 447b         	add	r3, pc
   61b18: 6858         	ldr	r0, [r3, #0x4]
   61b1a: b360         	cbz	r0, 0x61b76 <KADP_InitSystem+0x76> @ imm = #0x58
   61b1c: 4f48         	ldr	r7, [pc, #0x120]        @ 0x61c40 <KADP_InitSystem+0x140>
   61b1e: 2400         	movs	r4, #0x0
   61b20: f8df 8120    	ldr.w	r8, [pc, #0x120]        @ 0x61c44 <KADP_InitSystem+0x144>
   61b24: 2502         	movs	r5, #0x2
   61b26: f8df 9120    	ldr.w	r9, [pc, #0x120]        @ 0x61c48 <KADP_InitSystem+0x148>
   61b2a: f7e7 ee32    	blx	0x49790 <sem_trywait@plt> @ imm = #-0x1839c
   61b2e: 4603         	mov	r3, r0
   61b30: 447f         	add	r7, pc
   61b32: 44f8         	add	r8, pc
   61b34: 44f9         	add	r9, pc
   61b36: f242 7010    	movw	r0, #0x2710
   61b3a: b1e3         	cbz	r3, 0x61b76 <KADP_InitSystem+0x76> @ imm = #0x38
   61b3c: f7e4 ef04    	blx	0x46948 <usleep@plt>    @ imm = #-0x1b1f8
   61b40: 4842         	ldr	r0, [pc, #0x108]        @ 0x61c4c <KADP_InitSystem+0x14c>
   61b42: 23fb         	movs	r3, #0xfb
   61b44: 463a         	mov	r2, r7
   61b46: 4629         	mov	r1, r5
   61b48: f856 a000    	ldr.w	r10, [r6, r0]
   61b4c: e9cd 4501    	strd	r4, r5, [sp, #4]
   61b50: 3401         	adds	r4, #0x1
   61b52: f8cd 8000    	str.w	r8, [sp]
   61b56: f8da 0000    	ldr.w	r0, [r10]
   61b5a: f7e9 eec6    	blx	0x4b8e8 <KADP_LOGM_WriteLog@plt> @ imm = #-0x16274
   61b5e: 2c03         	cmp	r4, #0x3
   61b60: d05d         	beq	0x61c1e <KADP_InitSystem+0x11e> @ imm = #0xba
   61b62: f8d9 0004    	ldr.w	r0, [r9, #0x4]
   61b66: 2502         	movs	r5, #0x2
   61b68: f7e7 ee12    	blx	0x49790 <sem_trywait@plt> @ imm = #-0x183dc
   61b6c: 4603         	mov	r3, r0
   61b6e: f242 7010    	movw	r0, #0x2710
   61b72: 2b00         	cmp	r3, #0x0
   61b74: d1e2         	bne	0x61b3c <KADP_InitSystem+0x3c> @ imm = #-0x3c
   61b76: 4b36         	ldr	r3, [pc, #0xd8]         @ 0x61c50 <KADP_InitSystem+0x150>
   61b78: 447b         	add	r3, pc
   61b7a: 68dc         	ldr	r4, [r3, #0xc]
   61b7c: 3401         	adds	r4, #0x1
   61b7e: 2c01         	cmp	r4, #0x1
   61b80: 60dc         	str	r4, [r3, #0xc]
   61b82: dc3b         	bgt	0x61bfc <KADP_InitSystem+0xfc> @ imm = #0x76
   61b84: 689b         	ldr	r3, [r3, #0x8]
   61b86: 2b01         	cmp	r3, #0x1
   61b88: d038         	beq	0x61bfc <KADP_InitSystem+0xfc> @ imm = #0x70
   61b8a: 4a30         	ldr	r2, [pc, #0xc0]         @ 0x61c4c <KADP_InitSystem+0x14c>
   61b8c: 4b31         	ldr	r3, [pc, #0xc4]         @ 0x61c54 <KADP_InitSystem+0x154>
   61b8e: f856 8002    	ldr.w	r8, [r6, r2]
   61b92: 58f3         	ldr	r3, [r6, r3]
   61b94: f8d8 7000    	ldr.w	r7, [r8]
   61b98: 681d         	ldr	r5, [r3]
   61b9a: f7e4 efea    	blx	0x46b70 <KADP_OSA_GetCurrThreadId@plt> @ imm = #-0x1b02c
   61b9e: 4b2e         	ldr	r3, [pc, #0xb8]         @ 0x61c58 <KADP_InitSystem+0x158>
   61ba0: 2102         	movs	r1, #0x2
   61ba2: 4a2e         	ldr	r2, [pc, #0xb8]         @ 0x61c5c <KADP_InitSystem+0x15c>
   61ba4: 9003         	str	r0, [sp, #0xc]
   61ba6: 4638         	mov	r0, r7
   61ba8: 447b         	add	r3, pc
   61baa: 9401         	str	r4, [sp, #0x4]
   61bac: 447a         	add	r2, pc
   61bae: 9502         	str	r5, [sp, #0x8]
   61bb0: 9300         	str	r3, [sp]
   61bb2: f240 1305    	movw	r3, #0x105
   61bb6: f7e9 ee98    	blx	0x4b8e8 <KADP_LOGM_WriteLog@plt> @ imm = #-0x162d0
   61bba: f7e8 efe8    	blx	0x4ab8c <KADP_OSA_Initialize@plt> @ imm = #-0x17030
   61bbe: 4605         	mov	r5, r0
   61bc0: 4c27         	ldr	r4, [pc, #0x9c]         @ 0x61c60 <KADP_InitSystem+0x160>
   61bc2: f7e8 ebde    	blx	0x4a380 <KADP_DBG_Initialize@plt> @ imm = #-0x17844
   61bc6: 4b27         	ldr	r3, [pc, #0x9c]         @ 0x61c64 <KADP_InitSystem+0x164>
   61bc8: 4607         	mov	r7, r0
   61bca: 447c         	add	r4, pc
   61bcc: 58f3         	ldr	r3, [r6, r3]
   61bce: 4798         	blx	r3
   61bd0: f854 3f04    	ldr	r3, [r4, #4]!
   61bd4: 2b00         	cmp	r3, #0x0
   61bd6: d1fa         	bne	0x61bce <KADP_InitSystem+0xce> @ imm = #-0xc
   61bd8: 4b23         	ldr	r3, [pc, #0x8c]         @ 0x61c68 <KADP_InitSystem+0x168>
   61bda: 2201         	movs	r2, #0x1
   61bdc: 4c23         	ldr	r4, [pc, #0x8c]         @ 0x61c6c <KADP_InitSystem+0x16c>
   61bde: 2102         	movs	r1, #0x2
   61be0: f8d8 0000    	ldr.w	r0, [r8]
   61be4: 447b         	add	r3, pc
   61be6: 9702         	str	r7, [sp, #0x8]
   61be8: 9501         	str	r5, [sp, #0x4]
   61bea: 447c         	add	r4, pc
   61bec: 609a         	str	r2, [r3, #0x8]
   61bee: f240 1317    	movw	r3, #0x117
   61bf2: 4a1f         	ldr	r2, [pc, #0x7c]         @ 0x61c70 <KADP_InitSystem+0x170>
   61bf4: 9400         	str	r4, [sp]
   61bf6: 447a         	add	r2, pc
   61bf8: f7e9 ee76    	blx	0x4b8e8 <KADP_LOGM_WriteLog@plt> @ imm = #-0x16314
   61bfc: 4b1d         	ldr	r3, [pc, #0x74]         @ 0x61c74 <KADP_InitSystem+0x174>
   61bfe: 447b         	add	r3, pc
   61c00: 6858         	ldr	r0, [r3, #0x4]
   61c02: b120         	cbz	r0, 0x61c0e <KADP_InitSystem+0x10e> @ imm = #0x8
   61c04: b004         	add	sp, #0x10
   61c06: e8bd 47f0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, lr}
   61c0a: f7e7 ba87    	b.w	0x4911c <TEEC_InitializeContext@plt+0xc> @ imm = #-0x18af2
   61c0e: b004         	add	sp, #0x10
   61c10: e8bd 87f0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, pc}
   61c14: f7e9 eb06    	blx	0x4b224 <KADP_LOGM_Init@plt> @ imm = #-0x169f4
   61c18: 2301         	movs	r3, #0x1
   61c1a: 6023         	str	r3, [r4]
   61c1c: e77a         	b	0x61b14 <KADP_InitSystem+0x14> @ imm = #-0x10c
   61c1e: 4b16         	ldr	r3, [pc, #0x58]         @ 0x61c78 <KADP_InitSystem+0x178>
   61c20: 463a         	mov	r2, r7
   61c22: f8da 0000    	ldr.w	r0, [r10]
   61c26: 4629         	mov	r1, r5
   61c28: 447b         	add	r3, pc
   61c2a: 9300         	str	r3, [sp]
   61c2c: 23fe         	movs	r3, #0xfe
   61c2e: f7e9 ee5c    	blx	0x4b8e8 <KADP_LOGM_WriteLog@plt> @ imm = #-0x16348
   61c32: e7a0         	b	0x61b76 <KADP_InitSystem+0x76> @ imm = #-0xc0
   61c34: d67a         	bvs	0x61d2c <KADP_ShutdownSystem+0xb0> @ imm = #0xf4
   61c36: 002d         	movs	r5, r5
   61c38: d50a         	bpl	0x61c50 <KADP_InitSystem+0x150> @ imm = #0x14
   61c3a: 0023         	movs	r3, r4
   61c3c: d66e         	bvs	0x61d1c <KADP_ShutdownSystem+0xa0> @ imm = #0xdc
   61c3e: 002d         	movs	r5, r5
   61c40: a7dc         	adr	r7, #880 <KADP_ShutdownSystem+0xa4>
   61c42: 0014         	movs	r4, r2
   61c44: a6ee         	adr	r6, #952 <KADP_DBG_DebugMain+0x2>
   61c46: 0014         	movs	r4, r2
   61c48: d650         	bvs	0x61cec <KADP_ShutdownSystem+0x70> @ imm = #0xa0
   61c4a: 002d         	movs	r5, r5
   61c4c: 2b8c         	cmp	r3, #0x8c
   61c4e: 0000         	movs	r0, r0
   61c50: d60c         	bvs	0x61c6c <KADP_InitSystem+0x16c> @ imm = #0x18
   61c52: 002d         	movs	r5, r5
   61c54: 2ac4         	cmp	r2, #0xc4
   61c56: 0000         	movs	r0, r0
   61c58: a698         	adr	r6, #608 <KADP_ShutdownSystem+0x78>
   61c5a: 0014         	movs	r4, r2
   61c5c: a760         	adr	r7, #384 <KADP_ShutdownSystem+0x44>
   61c5e: 0014         	movs	r4, r2
   61c60: d11a         	bne	0x61c98 <KADP_ShutdownSystem+0x1c> @ imm = #0x34
   61c62: 0023         	movs	r3, r4
   61c64: 2fa4         	cmp	r7, #0xa4
   61c66: 0000         	movs	r0, r0
   61c68: d5a0         	bpl	0x61bac <KADP_InitSystem+0xac> @ imm = #-0xc0
   61c6a: 002d         	movs	r5, r5
   61c6c: a69e         	adr	r6, #632 <KADP_ShutdownSystem+0x92>
   61c6e: 0014         	movs	r4, r2
   61c70: a716         	adr	r7, #88 <KADP_ShutdownSystem+0xe>
   61c72: 0014         	movs	r4, r2
   61c74: d586         	bpl	0x61b84 <KADP_InitSystem+0x84> @ imm = #-0xf4
   61c76: 002d         	movs	r5, r5
   61c78: a68c         	adr	r6, #560 <KADP_ShutdownSystem+0x8c>
   61c7a: 0014         	movs	r4, r2
