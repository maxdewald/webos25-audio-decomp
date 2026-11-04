
orig/usr/lib/gstreamer-1.0/libgstlgaudiosink.so:	file format elf32-littlearm

Disassembly of section .text:

0001673c <lgao_set_audio_info>:
   1673c: b084         	sub	sp, #0x10
   1673e: 4684         	mov	r12, r0
   16740: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
   16744: b095         	sub	sp, #0x54
   16746: ac1f         	add	r4, sp, #0x7c
   16748: e884 000e    	stm.w	r4, {r1, r2, r3}
   1674c: 4b47         	ldr	r3, [pc, #0x11c]        @ 0x1686c <lgao_set_audio_info+0x130>
   1674e: 4a48         	ldr	r2, [pc, #0x120]        @ 0x16870 <lgao_set_audio_info+0x134>
   16750: 447b         	add	r3, pc
   16752: 589b         	ldr	r3, [r3, r2]
   16754: f8d3 b00c    	ldr.w	r11, [r3, #0xc]
   16758: 930e         	str	r3, [sp, #0x38]
   1675a: 2800         	cmp	r0, #0x0
   1675c: d06f         	beq	0x1683e <lgao_set_audio_info+0x102> @ imm = #0xde
   1675e: 9b1f         	ldr	r3, [sp, #0x7c]
   16760: f10d 0eb8    	add.w	lr, sp, #0xb8
   16764: 9a20         	ldr	r2, [sp, #0x80]
   16766: f89d 8084    	ldrb.w	r8, [sp, #0x84]
   1676a: 930f         	str	r3, [sp, #0x3c]
   1676c: e9c0 3209    	strd	r3, r2, [r0, #36]
   16770: 9b22         	ldr	r3, [sp, #0x88]
   16772: f89d 9085    	ldrb.w	r9, [sp, #0x85]
   16776: 9c23         	ldr	r4, [sp, #0x8c]
   16778: 6303         	str	r3, [r0, #0x30]
   1677a: 9311         	str	r3, [sp, #0x44]
   1677c: 9b28         	ldr	r3, [sp, #0xa0]
   1677e: 9d24         	ldr	r5, [sp, #0x90]
   16780: 9e26         	ldr	r6, [sp, #0x98]
   16782: 6483         	str	r3, [r0, #0x48]
   16784: 9312         	str	r3, [sp, #0x48]
   16786: 9b29         	ldr	r3, [sp, #0xa4]
   16788: 9f27         	ldr	r7, [sp, #0x9c]
   1678a: f8dd a0ac    	ldr.w	r10, [sp, #0xac]
   1678e: 64c3         	str	r3, [r0, #0x4c]
   16790: 9b2a         	ldr	r3, [sp, #0xa8]
   16792: 9210         	str	r2, [sp, #0x40]
   16794: f880 802c    	strb.w	r8, [r0, #0x2c]
   16798: 6503         	str	r3, [r0, #0x50]
   1679a: 9313         	str	r3, [sp, #0x4c]
   1679c: 9b2c         	ldr	r3, [sp, #0xb0]
   1679e: f880 902d    	strb.w	r9, [r0, #0x2d]
   167a2: 6344         	str	r4, [r0, #0x34]
   167a4: 6385         	str	r5, [r0, #0x38]
   167a6: 6406         	str	r6, [r0, #0x40]
   167a8: 6447         	str	r7, [r0, #0x44]
   167aa: f8c0 a054    	str.w	r10, [r0, #0x54]
   167ae: 6583         	str	r3, [r0, #0x58]
   167b0: 9b2d         	ldr	r3, [sp, #0xb4]
   167b2: 65c3         	str	r3, [r0, #0x5c]
   167b4: 9b32         	ldr	r3, [sp, #0xc8]
   167b6: 6703         	str	r3, [r0, #0x70]
   167b8: 9b33         	ldr	r3, [sp, #0xcc]
   167ba: 6743         	str	r3, [r0, #0x74]
   167bc: 9b34         	ldr	r3, [sp, #0xd0]
   167be: 6783         	str	r3, [r0, #0x78]
   167c0: 9b35         	ldr	r3, [sp, #0xd4]
   167c2: 67c3         	str	r3, [r0, #0x7c]
   167c4: e8be 000f    	ldm.w	lr!, {r0, r1, r2, r3}
   167c8: f8cc 0060    	str.w	r0, [r12, #0x60]
   167cc: f8cc 1064    	str.w	r1, [r12, #0x64]
   167d0: f8cc 2068    	str.w	r2, [r12, #0x68]
   167d4: f8cc 306c    	str.w	r3, [r12, #0x6c]
   167d8: f8db 0000    	ldr.w	r0, [r11]
   167dc: f010 0008    	ands	r0, r0, #0x8
   167e0: d104         	bne	0x167ec <lgao_set_audio_info+0xb0> @ imm = #0x8
   167e2: b015         	add	sp, #0x54
   167e4: e8bd 4ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
   167e8: b004         	add	sp, #0x10
   167ea: 4770         	bx	lr
   167ec: 9b13         	ldr	r3, [sp, #0x4c]
   167ee: 4a21         	ldr	r2, [pc, #0x84]         @ 0x16874 <lgao_set_audio_info+0x138>
   167f0: 990f         	ldr	r1, [sp, #0x3c]
   167f2: 930c         	str	r3, [sp, #0x30]
   167f4: 9b12         	ldr	r3, [sp, #0x48]
   167f6: 447a         	add	r2, pc
   167f8: f8cd a034    	str.w	r10, [sp, #0x34]
   167fc: 9609         	str	r6, [sp, #0x24]
   167fe: e9cd 730a    	strd	r7, r3, [sp, #40]
   16802: 9b11         	ldr	r3, [sp, #0x44]
   16804: e9cd 4506    	strd	r4, r5, [sp, #24]
   16808: 980e         	ldr	r0, [sp, #0x38]
   1680a: e9cd 9304    	strd	r9, r3, [sp, #16]
   1680e: f8dc 303c    	ldr.w	r3, [r12, #0x3c]
   16812: e9cd 2100    	strd	r2, r1, [sp]
   16816: 2103         	movs	r1, #0x3
   16818: 4a17         	ldr	r2, [pc, #0x5c]         @ 0x16878 <lgao_set_audio_info+0x13c>
   1681a: 9308         	str	r3, [sp, #0x20]
   1681c: 9b10         	ldr	r3, [sp, #0x40]
   1681e: 447a         	add	r2, pc
   16820: f8cd 800c    	str.w	r8, [sp, #0xc]
   16824: f502 724c    	add.w	r2, r2, #0x330
   16828: 9302         	str	r3, [sp, #0x8]
   1682a: f240 53ed    	movw	r3, #0x5ed
   1682e: f7ed ebbe    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x12884
   16832: 2000         	movs	r0, #0x0
   16834: b015         	add	sp, #0x54
   16836: e8bd 4ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
   1683a: b004         	add	sp, #0x10
   1683c: 4770         	bx	lr
   1683e: f8db 3000    	ldr.w	r3, [r11]
   16842: 07db         	lsls	r3, r3, #0x1f
   16844: bf58         	it	pl
   16846: f04f 30ff    	movpl.w	r0, #0xffffffff
   1684a: d5ca         	bpl	0x167e2 <lgao_set_audio_info+0xa6> @ imm = #-0x6c
   1684c: 4a0b         	ldr	r2, [pc, #0x2c]         @ 0x1687c <lgao_set_audio_info+0x140>
   1684e: f240 53ca    	movw	r3, #0x5ca
   16852: 4c0b         	ldr	r4, [pc, #0x2c]         @ 0x16880 <lgao_set_audio_info+0x144>
   16854: 4661         	mov	r1, r12
   16856: 980e         	ldr	r0, [sp, #0x38]
   16858: 447a         	add	r2, pc
   1685a: 447c         	add	r4, pc
   1685c: f502 724c    	add.w	r2, r2, #0x330
   16860: 9400         	str	r4, [sp]
   16862: f7ed eba4    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x128b8
   16866: f04f 30ff    	mov.w	r0, #0xffffffff
   1686a: e7ba         	b	0x167e2 <lgao_set_audio_info+0xa6> @ imm = #-0x8c
   1686c: 88 b4 01 00  	.word	0x0001b488
   16870: 00 04 00 00  	.word	0x00000400
   16874: de 9f 00 00  	.word	0x00009fde
   16878: fa a6 00 00  	.word	0x0000a6fa
   1687c: c0 a6 00 00  	.word	0x0000a6c0
   16880: 7e 99 00 00  	.word	0x0000997e
