
orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so:	file format elf32-littlearm

Disassembly of section .text:

00020848 <lgadec_set_es_bypass>:
   20848: 4b4f         	ldr	r3, [pc, #0x13c]        @ 0x20988 <lgadec_set_es_bypass+0x140>
   2084a: 4a50         	ldr	r2, [pc, #0x140]        @ 0x2098c <lgadec_set_es_bypass+0x144>
   2084c: b5f0         	push	{r4, r5, r6, r7, lr}
   2084e: 4604         	mov	r4, r0
   20850: 447b         	add	r3, pc
   20852: b083         	sub	sp, #0xc
   20854: 589e         	ldr	r6, [r3, r2]
   20856: 68f3         	ldr	r3, [r6, #0xc]
   20858: 681b         	ldr	r3, [r3]
   2085a: 2800         	cmp	r0, #0x0
   2085c: d07f         	beq	0x2095e <lgadec_set_es_bypass+0x116> @ imm = #0xfe
   2085e: 460d         	mov	r5, r1
   20860: 0759         	lsls	r1, r3, #0x1d
   20862: d52a         	bpl	0x208ba <lgadec_set_es_bypass+0x72> @ imm = #0x54
   20864: 2d00         	cmp	r5, #0x0
   20866: d060         	beq	0x2092a <lgadec_set_es_bypass+0xe2> @ imm = #0xc0
   20868: 4b49         	ldr	r3, [pc, #0x124]        @ 0x20990 <lgadec_set_es_bypass+0x148>
   2086a: 2102         	movs	r1, #0x2
   2086c: 4a49         	ldr	r2, [pc, #0x124]        @ 0x20994 <lgadec_set_es_bypass+0x14c>
   2086e: 4630         	mov	r0, r6
   20870: 447b         	add	r3, pc
   20872: 9301         	str	r3, [sp, #0x4]
   20874: 447a         	add	r2, pc
   20876: 4b48         	ldr	r3, [pc, #0x120]        @ 0x20998 <lgadec_set_es_bypass+0x150>
   20878: f602 3254    	addw	r2, r2, #0xb54
   2087c: 447b         	add	r3, pc
   2087e: 9300         	str	r3, [sp]
   20880: f241 034f    	movw	r3, #0x104f
   20884: f7e5 ee2e    	blx	0x64e4 <logmm_printl@plt> @ imm = #-0x1a3a4
   20888: f894 3369    	ldrb.w	r3, [r4, #0x369]
   2088c: bb7b         	cbnz	r3, 0x208ee <lgadec_set_es_bypass+0xa6> @ imm = #0x5e
   2088e: 68f3         	ldr	r3, [r6, #0xc]
   20890: 681b         	ldr	r3, [r3]
   20892: 075a         	lsls	r2, r3, #0x1d
   20894: d50c         	bpl	0x208b0 <lgadec_set_es_bypass+0x68> @ imm = #0x18
   20896: 4a41         	ldr	r2, [pc, #0x104]        @ 0x2099c <lgadec_set_es_bypass+0x154>
   20898: 4630         	mov	r0, r6
   2089a: 4b41         	ldr	r3, [pc, #0x104]        @ 0x209a0 <lgadec_set_es_bypass+0x158>
   2089c: 2102         	movs	r1, #0x2
   2089e: 447a         	add	r2, pc
   208a0: 447b         	add	r3, pc
   208a2: f602 3254    	addw	r2, r2, #0xb54
   208a6: 9300         	str	r3, [sp]
   208a8: f241 0352    	movw	r3, #0x1052
   208ac: f7e5 ee1a    	blx	0x64e4 <logmm_printl@plt> @ imm = #-0x1a3cc
   208b0: 2000         	movs	r0, #0x0
   208b2: f884 5368    	strb.w	r5, [r4, #0x368]
   208b6: b003         	add	sp, #0xc
   208b8: bdf0         	pop	{r4, r5, r6, r7, pc}
   208ba: f890 3369    	ldrb.w	r3, [r0, #0x369]
   208be: 2b00         	cmp	r3, #0x0
   208c0: d0f6         	beq	0x208b0 <lgadec_set_es_bypass+0x68> @ imm = #-0x14
   208c2: f104 07ac    	add.w	r7, r4, #0xac
   208c6: 4638         	mov	r0, r7
   208c8: f7e5 ebc6    	blx	0x6058 <pthread_mutex_lock@plt> @ imm = #-0x1a874
   208cc: b9a5         	cbnz	r5, 0x208f8 <lgadec_set_es_bypass+0xb0> @ imm = #0x28
   208ce: 4935         	ldr	r1, [pc, #0xd4]         @ 0x209a4 <lgadec_set_es_bypass+0x15c>
   208d0: 2204         	movs	r2, #0x4
   208d2: 68e0         	ldr	r0, [r4, #0xc]
   208d4: 4479         	add	r1, pc
   208d6: f7e5 eaf4    	blx	0x5ec0 <write@plt>      @ imm = #-0x1aa18
   208da: 68f3         	ldr	r3, [r6, #0xc]
   208dc: 681b         	ldr	r3, [r3]
   208de: 075b         	lsls	r3, r3, #0x1d
   208e0: d414         	bmi	0x2090c <lgadec_set_es_bypass+0xc4> @ imm = #0x28
   208e2: 4638         	mov	r0, r7
   208e4: f7e5 eb4e    	blx	0x5f84 <pthread_mutex_unlock@plt> @ imm = #-0x1a964
   208e8: 2000         	movs	r0, #0x0
   208ea: b003         	add	sp, #0xc
   208ec: bdf0         	pop	{r4, r5, r6, r7, pc}
   208ee: f104 07ac    	add.w	r7, r4, #0xac
   208f2: 4638         	mov	r0, r7
   208f4: f7e5 ebb0    	blx	0x6058 <pthread_mutex_lock@plt> @ imm = #-0x1a8a0
   208f8: 492b         	ldr	r1, [pc, #0xac]         @ 0x209a8 <lgadec_set_es_bypass+0x160>
   208fa: 2204         	movs	r2, #0x4
   208fc: 68e0         	ldr	r0, [r4, #0xc]
   208fe: 4479         	add	r1, pc
   20900: f7e5 eade    	blx	0x5ec0 <write@plt>      @ imm = #-0x1aa44
   20904: 68f3         	ldr	r3, [r6, #0xc]
   20906: 681b         	ldr	r3, [r3]
   20908: 075b         	lsls	r3, r3, #0x1d
   2090a: d5ea         	bpl	0x208e2 <lgadec_set_es_bypass+0x9a> @ imm = #-0x2c
   2090c: 4a27         	ldr	r2, [pc, #0x9c]         @ 0x209ac <lgadec_set_es_bypass+0x164>
   2090e: 4630         	mov	r0, r6
   20910: 4b27         	ldr	r3, [pc, #0x9c]         @ 0x209b0 <lgadec_set_es_bypass+0x168>
   20912: 2102         	movs	r1, #0x2
   20914: 9501         	str	r5, [sp, #0x4]
   20916: 447a         	add	r2, pc
   20918: 447b         	add	r3, pc
   2091a: f602 3254    	addw	r2, r2, #0xb54
   2091e: 9300         	str	r3, [sp]
   20920: f241 035c    	movw	r3, #0x105c
   20924: f7e5 edde    	blx	0x64e4 <logmm_printl@plt> @ imm = #-0x1a444
   20928: e7db         	b	0x208e2 <lgadec_set_es_bypass+0x9a> @ imm = #-0x4a
   2092a: 4b22         	ldr	r3, [pc, #0x88]         @ 0x209b4 <lgadec_set_es_bypass+0x16c>
   2092c: 2102         	movs	r1, #0x2
   2092e: 4a22         	ldr	r2, [pc, #0x88]         @ 0x209b8 <lgadec_set_es_bypass+0x170>
   20930: 4630         	mov	r0, r6
   20932: 447b         	add	r3, pc
   20934: 9301         	str	r3, [sp, #0x4]
   20936: 447a         	add	r2, pc
   20938: 4b20         	ldr	r3, [pc, #0x80]         @ 0x209bc <lgadec_set_es_bypass+0x174>
   2093a: f602 3254    	addw	r2, r2, #0xb54
   2093e: 447b         	add	r3, pc
   20940: 9300         	str	r3, [sp]
   20942: f241 034f    	movw	r3, #0x104f
   20946: f7e5 edce    	blx	0x64e4 <logmm_printl@plt> @ imm = #-0x1a464
   2094a: f894 3369    	ldrb.w	r3, [r4, #0x369]
   2094e: 2b00         	cmp	r3, #0x0
   20950: d09d         	beq	0x2088e <lgadec_set_es_bypass+0x46> @ imm = #-0xc6
   20952: f104 07ac    	add.w	r7, r4, #0xac
   20956: 4638         	mov	r0, r7
   20958: f7e5 eb7e    	blx	0x6058 <pthread_mutex_lock@plt> @ imm = #-0x1a904
   2095c: e7b7         	b	0x208ce <lgadec_set_es_bypass+0x86> @ imm = #-0x92
   2095e: 07d8         	lsls	r0, r3, #0x1f
   20960: bf58         	it	pl
   20962: f04f 30ff    	movpl.w	r0, #0xffffffff
   20966: d5c0         	bpl	0x208ea <lgadec_set_es_bypass+0xa2> @ imm = #-0x80
   20968: 4a15         	ldr	r2, [pc, #0x54]         @ 0x209c0 <lgadec_set_es_bypass+0x178>
   2096a: f241 034d    	movw	r3, #0x104d
   2096e: 4d15         	ldr	r5, [pc, #0x54]         @ 0x209c4 <lgadec_set_es_bypass+0x17c>
   20970: 4621         	mov	r1, r4
   20972: 4630         	mov	r0, r6
   20974: 447a         	add	r2, pc
   20976: 447d         	add	r5, pc
   20978: f602 3254    	addw	r2, r2, #0xb54
   2097c: 9500         	str	r5, [sp]
   2097e: f7e5 edb2    	blx	0x64e4 <logmm_printl@plt> @ imm = #-0x1a49c
   20982: f04f 30ff    	mov.w	r0, #0xffffffff
   20986: e7b0         	b	0x208ea <lgadec_set_es_bypass+0xa2> @ imm = #-0xa0
   20988: 18 e2 01 00  	.word	0x0001e218
   2098c: 60 05 00 00  	.word	0x00000560
   20990: 50 b0 00 00  	.word	0x0000b050
   20994: 2c b4 00 00  	.word	0x0000b42c
   20998: 58 b0 00 00  	.word	0x0000b058
   2099c: 02 b4 00 00  	.word	0x0000b402
   209a0: ec af 00 00  	.word	0x0000afec
   209a4: 4c af 00 00  	.word	0x0000af4c
   209a8: a6 af 00 00  	.word	0x0000afa6
   209ac: 8a b3 00 00  	.word	0x0000b38a
   209b0: 94 af 00 00  	.word	0x0000af94
   209b4: ae af 00 00  	.word	0x0000afae
   209b8: 6a b3 00 00  	.word	0x0000b36a
   209bc: 96 af 00 00  	.word	0x0000af96
   209c0: 2c b3 00 00  	.word	0x0000b32c
   209c4: a2 95 00 00  	.word	0x000095a2
