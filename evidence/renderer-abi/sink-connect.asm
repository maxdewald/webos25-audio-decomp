
/Users/max/projects/dts_restore_plus/re/webos25-audio-policy/bin/usr/lib/gstreamer-1.0/libgstlgaudiosink.so:	file format elf32-littlearm

Disassembly of section .text:

0001441c <_lgao_connect_renderer>:
   1441c: e92d 41f0    	push.w	{r4, r5, r6, r7, r8, lr}
   14420: 2500         	movs	r5, #0x0
   14422: f8df 7468    	ldr.w	r7, [pc, #0x468]        @ 0x1488c <_lgao_connect_renderer+0x470>
   14426: b0ac         	sub	sp, #0xb0
   14428: 4604         	mov	r4, r0
   1442a: 462a         	mov	r2, r5
   1442c: f8df 6460    	ldr.w	r6, [pc, #0x460]        @ 0x14890 <_lgao_connect_renderer+0x474>
   14430: 447f         	add	r7, pc
   14432: f8d0 01b4    	ldr.w	r0, [r0, #0x1b4]
   14436: 4639         	mov	r1, r7
   14438: e9cd 5505    	strd	r5, r5, [sp, #20]
   1443c: 447e         	add	r6, pc
   1443e: f7ef ed78    	blx	0x3f30 <aimc_module_create@plt> @ imm = #-0x10510
   14442: f8c4 01c0    	str.w	r0, [r4, #0x1c0]
   14446: b9d8         	cbnz	r0, 0x14480 <_lgao_connect_renderer+0x64> @ imm = #0x36
   14448: f8df 3448    	ldr.w	r3, [pc, #0x448]        @ 0x14894 <_lgao_connect_renderer+0x478>
   1444c: 58f3         	ldr	r3, [r6, r3]
   1444e: 68da         	ldr	r2, [r3, #0xc]
   14450: 6812         	ldr	r2, [r2]
   14452: 07d1         	lsls	r1, r2, #0x1f
   14454: f100 808d    	bmi.w	0x14572 <_lgao_connect_renderer+0x156> @ imm = #0x11a
   14458: f8d4 11c8    	ldr.w	r1, [r4, #0x1c8]
   1445c: b131         	cbz	r1, 0x1446c <_lgao_connect_renderer+0x50> @ imm = #0xc
   1445e: f8d4 01b4    	ldr.w	r0, [r4, #0x1b4]
   14462: f7ef eefc    	blx	0x425c <aimc_module_destroy@plt> @ imm = #-0x10208
   14466: 2300         	movs	r3, #0x0
   14468: e9c4 3372    	strd	r3, r3, [r4, #456]
   1446c: f8d4 11d4    	ldr.w	r1, [r4, #0x1d4]
   14470: f04f 35ff    	mov.w	r5, #0xffffffff
   14474: 2900         	cmp	r1, #0x0
   14476: d16f         	bne	0x14558 <_lgao_connect_renderer+0x13c> @ imm = #0xde
   14478: 4628         	mov	r0, r5
   1447a: b02c         	add	sp, #0xb0
   1447c: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
   14480: f8d4 01b4    	ldr.w	r0, [r4, #0x1b4]
   14484: 462a         	mov	r2, r5
   14486: 4639         	mov	r1, r7
   14488: f7ef ed52    	blx	0x3f30 <aimc_module_create@plt> @ imm = #-0x1055c
   1448c: f8df 3404    	ldr.w	r3, [pc, #0x404]        @ 0x14894 <_lgao_connect_renderer+0x478>
   14490: f8c4 01d4    	str.w	r0, [r4, #0x1d4]
   14494: 2800         	cmp	r0, #0x0
   14496: f000 8096    	beq.w	0x145c6 <_lgao_connect_renderer+0x1aa> @ imm = #0x12c
   1449a: 58f7         	ldr	r7, [r6, r3]
   1449c: 68fb         	ldr	r3, [r7, #0xc]
   1449e: 681b         	ldr	r3, [r3]
   144a0: 071b         	lsls	r3, r3, #0x1c
   144a2: f100 80a2    	bmi.w	0x145ea <_lgao_connect_renderer+0x1ce> @ imm = #0x144
   144a6: f8d4 01b4    	ldr.w	r0, [r4, #0x1b4]
   144aa: f7ef ed30    	blx	0x3f0c <aimc_handler_sync@plt> @ imm = #-0x105a0
   144ae: f8d4 01c0    	ldr.w	r0, [r4, #0x1c0]
   144b2: f7ef eb66    	blx	0x3b80 <ModuleRegistry_SearchInstByUid@plt> @ imm = #-0x10934
   144b6: f8c4 01c4    	str.w	r0, [r4, #0x1c4]
   144ba: 2800         	cmp	r0, #0x0
   144bc: f000 80a6    	beq.w	0x1460c <_lgao_connect_renderer+0x1f0> @ imm = #0x14c
   144c0: f8d4 01d4    	ldr.w	r0, [r4, #0x1d4]
   144c4: f7ef eb5c    	blx	0x3b80 <ModuleRegistry_SearchInstByUid@plt> @ imm = #-0x10948
   144c8: f8c4 01d8    	str.w	r0, [r4, #0x1d8]
   144cc: 2800         	cmp	r0, #0x0
   144ce: d069         	beq	0x145a4 <_lgao_connect_renderer+0x188> @ imm = #0xd2
   144d0: f245 2121    	movw	r1, #0x5221
   144d4: 6820         	ldr	r0, [r4]
   144d6: f7ef ebc4    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x10878
   144da: f10d 0814    	add.w	r8, sp, #0x14
   144de: f245 2102    	movw	r1, #0x5202
   144e2: 4642         	mov	r2, r8
   144e4: f2c8 0104    	movt	r1, #0x8004
   144e8: 6820         	ldr	r0, [r4]
   144ea: f7ef ebba    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x1088c
   144ee: f245 2122    	movw	r1, #0x5222
   144f2: aa06         	add	r2, sp, #0x18
   144f4: 6820         	ldr	r0, [r4]
   144f6: f2c8 0104    	movt	r1, #0x8004
   144fa: f7ef ebb2    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x1089c
   144fe: 68fb         	ldr	r3, [r7, #0xc]
   14500: 681b         	ldr	r3, [r3]
   14502: 071a         	lsls	r2, r3, #0x1c
   14504: f100 80a2    	bmi.w	0x1464c <_lgao_connect_renderer+0x230> @ imm = #0x144
   14508: 2500         	movs	r5, #0x0
   1450a: 9b05         	ldr	r3, [sp, #0x14]
   1450c: 9500         	str	r5, [sp]
   1450e: 462a         	mov	r2, r5
   14510: f8d4 11c0    	ldr.w	r1, [r4, #0x1c0]
   14514: f8d4 01b4    	ldr.w	r0, [r4, #0x1b4]
   14518: f7ef ecc8    	blx	0x3eac <aimc_module_connect@plt> @ imm = #-0x10670
   1451c: 68fb         	ldr	r3, [r7, #0xc]
   1451e: 681b         	ldr	r3, [r3]
   14520: 2800         	cmp	r0, #0x0
   14522: f040 8084    	bne.w	0x1462e <_lgao_connect_renderer+0x212> @ imm = #0x108
   14526: 0719         	lsls	r1, r3, #0x1c
   14528: f100 80e2    	bmi.w	0x146f0 <_lgao_connect_renderer+0x2d4> @ imm = #0x1c4
   1452c: 49da         	ldr	r1, [pc, #0x368]        @ 0x14898 <_lgao_connect_renderer+0x47c>
   1452e: 2200         	movs	r2, #0x0
   14530: f8d4 01b4    	ldr.w	r0, [r4, #0x1b4]
   14534: 4479         	add	r1, pc
   14536: f7ef ecfc    	blx	0x3f30 <aimc_module_create@plt> @ imm = #-0x10608
   1453a: 4601         	mov	r1, r0
   1453c: f8c4 01c8    	str.w	r0, [r4, #0x1c8]
   14540: 2800         	cmp	r0, #0x0
   14542: f040 80b2    	bne.w	0x146aa <_lgao_connect_renderer+0x28e> @ imm = #0x164
   14546: 68fb         	ldr	r3, [r7, #0xc]
   14548: 681b         	ldr	r3, [r3]
   1454a: 07da         	lsls	r2, r3, #0x1f
   1454c: f100 80a1    	bmi.w	0x14692 <_lgao_connect_renderer+0x276> @ imm = #0x142
   14550: f8d4 11c0    	ldr.w	r1, [r4, #0x1c0]
   14554: b9f1         	cbnz	r1, 0x14594 <_lgao_connect_renderer+0x178> @ imm = #0x3c
   14556: e789         	b	0x1446c <_lgao_connect_renderer+0x50> @ imm = #-0xee
   14558: f8d4 01b4    	ldr.w	r0, [r4, #0x1b4]
   1455c: f04f 35ff    	mov.w	r5, #0xffffffff
   14560: f7ef ee7c    	blx	0x425c <aimc_module_destroy@plt> @ imm = #-0x10308
   14564: 2300         	movs	r3, #0x0
   14566: 4628         	mov	r0, r5
   14568: e9c4 3375    	strd	r3, r3, [r4, #468]
   1456c: b02c         	add	sp, #0xb0
   1456e: e8bd 81f0    	pop.w	{r4, r5, r6, r7, r8, pc}
   14572: 4aca         	ldr	r2, [pc, #0x328]        @ 0x1489c <_lgao_connect_renderer+0x480>
   14574: 4601         	mov	r1, r0
   14576: 4dca         	ldr	r5, [pc, #0x328]        @ 0x148a0 <_lgao_connect_renderer+0x484>
   14578: 4618         	mov	r0, r3
   1457a: f240 737d    	movw	r3, #0x77d
   1457e: 447a         	add	r2, pc
   14580: 447d         	add	r5, pc
   14582: 3250         	adds	r2, #0x50
   14584: 9500         	str	r5, [sp]
   14586: f7ef ed12    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x105dc
   1458a: f8d4 11c0    	ldr.w	r1, [r4, #0x1c0]
   1458e: 2900         	cmp	r1, #0x0
   14590: f43f af62    	beq.w	0x14458 <_lgao_connect_renderer+0x3c> @ imm = #-0x13c
   14594: f8d4 01b4    	ldr.w	r0, [r4, #0x1b4]
   14598: f7ef ee60    	blx	0x425c <aimc_module_destroy@plt> @ imm = #-0x10340
   1459c: 2300         	movs	r3, #0x0
   1459e: e9c4 3370    	strd	r3, r3, [r4, #448]
   145a2: e759         	b	0x14458 <_lgao_connect_renderer+0x3c> @ imm = #-0x14e
   145a4: 68fb         	ldr	r3, [r7, #0xc]
   145a6: 681b         	ldr	r3, [r3]
   145a8: 07d9         	lsls	r1, r3, #0x1f
   145aa: d5ee         	bpl	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x24
   145ac: 4abd         	ldr	r2, [pc, #0x2f4]        @ 0x148a4 <_lgao_connect_renderer+0x488>
   145ae: 4601         	mov	r1, r0
   145b0: 4bbd         	ldr	r3, [pc, #0x2f4]        @ 0x148a8 <_lgao_connect_renderer+0x48c>
   145b2: 4638         	mov	r0, r7
   145b4: 447a         	add	r2, pc
   145b6: 447b         	add	r3, pc
   145b8: 3250         	adds	r2, #0x50
   145ba: 9300         	str	r3, [sp]
   145bc: f240 7394    	movw	r3, #0x794
   145c0: f7ef ecf4    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10618
   145c4: e7e1         	b	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x3e
   145c6: 58f3         	ldr	r3, [r6, r3]
   145c8: 68da         	ldr	r2, [r3, #0xc]
   145ca: 6812         	ldr	r2, [r2]
   145cc: 07d2         	lsls	r2, r2, #0x1f
   145ce: d5dc         	bpl	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x48
   145d0: 4ab6         	ldr	r2, [pc, #0x2d8]        @ 0x148ac <_lgao_connect_renderer+0x490>
   145d2: 4601         	mov	r1, r0
   145d4: 4db6         	ldr	r5, [pc, #0x2d8]        @ 0x148b0 <_lgao_connect_renderer+0x494>
   145d6: 4618         	mov	r0, r3
   145d8: f240 7383    	movw	r3, #0x783
   145dc: 447a         	add	r2, pc
   145de: 447d         	add	r5, pc
   145e0: 3250         	adds	r2, #0x50
   145e2: 9500         	str	r5, [sp]
   145e4: f7ef ece2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x1063c
   145e8: e7cf         	b	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x62
   145ea: 9002         	str	r0, [sp, #0x8]
   145ec: 2103         	movs	r1, #0x3
   145ee: f8d4 31c0    	ldr.w	r3, [r4, #0x1c0]
   145f2: 4638         	mov	r0, r7
   145f4: 4aaf         	ldr	r2, [pc, #0x2bc]        @ 0x148b4 <_lgao_connect_renderer+0x498>
   145f6: 9301         	str	r3, [sp, #0x4]
   145f8: 4baf         	ldr	r3, [pc, #0x2bc]        @ 0x148b8 <_lgao_connect_renderer+0x49c>
   145fa: 447a         	add	r2, pc
   145fc: 3250         	adds	r2, #0x50
   145fe: 447b         	add	r3, pc
   14600: 9300         	str	r3, [sp]
   14602: f240 7387    	movw	r3, #0x787
   14606: f7ef ecd2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x1065c
   1460a: e74c         	b	0x144a6 <_lgao_connect_renderer+0x8a> @ imm = #-0x168
   1460c: 68fb         	ldr	r3, [r7, #0xc]
   1460e: 681b         	ldr	r3, [r3]
   14610: 07dd         	lsls	r5, r3, #0x1f
   14612: d5ba         	bpl	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x8c
   14614: 4aa9         	ldr	r2, [pc, #0x2a4]        @ 0x148bc <_lgao_connect_renderer+0x4a0>
   14616: 4601         	mov	r1, r0
   14618: 4ba9         	ldr	r3, [pc, #0x2a4]        @ 0x148c0 <_lgao_connect_renderer+0x4a4>
   1461a: 4638         	mov	r0, r7
   1461c: 447a         	add	r2, pc
   1461e: 447b         	add	r3, pc
   14620: 3250         	adds	r2, #0x50
   14622: 9300         	str	r3, [sp]
   14624: f240 738e    	movw	r3, #0x78e
   14628: f7ef ecc0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10680
   1462c: e7ad         	b	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0xa6
   1462e: 07d8         	lsls	r0, r3, #0x1f
   14630: d5ab         	bpl	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0xaa
   14632: 4aa4         	ldr	r2, [pc, #0x290]        @ 0x148c4 <_lgao_connect_renderer+0x4a8>
   14634: 4629         	mov	r1, r5
   14636: 4ba4         	ldr	r3, [pc, #0x290]        @ 0x148c8 <_lgao_connect_renderer+0x4ac>
   14638: 4638         	mov	r0, r7
   1463a: 447a         	add	r2, pc
   1463c: 447b         	add	r3, pc
   1463e: 3250         	adds	r2, #0x50
   14640: 9300         	str	r3, [sp]
   14642: f44f 63f4    	mov.w	r3, #0x7a0
   14646: f7ef ecb2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x1069c
   1464a: e79e         	b	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0xc4
   1464c: f8d4 31c4    	ldr.w	r3, [r4, #0x1c4]
   14650: 2103         	movs	r1, #0x3
   14652: 4d9e         	ldr	r5, [pc, #0x278]        @ 0x148cc <_lgao_connect_renderer+0x4b0>
   14654: 4638         	mov	r0, r7
   14656: 681b         	ldr	r3, [r3]
   14658: 447d         	add	r5, pc
   1465a: 3550         	adds	r5, #0x50
   1465c: 9301         	str	r3, [sp, #0x4]
   1465e: 462a         	mov	r2, r5
   14660: 4b9b         	ldr	r3, [pc, #0x26c]        @ 0x148d0 <_lgao_connect_renderer+0x4b4>
   14662: 447b         	add	r3, pc
   14664: 9300         	str	r3, [sp]
   14666: f240 739c    	movw	r3, #0x79c
   1466a: f7ef eca0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x106c0
   1466e: 68fb         	ldr	r3, [r7, #0xc]
   14670: 681b         	ldr	r3, [r3]
   14672: 071b         	lsls	r3, r3, #0x1c
   14674: f57f af48    	bpl.w	0x14508 <_lgao_connect_renderer+0xec> @ imm = #-0x170
   14678: 9b05         	ldr	r3, [sp, #0x14]
   1467a: 462a         	mov	r2, r5
   1467c: 2103         	movs	r1, #0x3
   1467e: 4638         	mov	r0, r7
   14680: 9301         	str	r3, [sp, #0x4]
   14682: 4b94         	ldr	r3, [pc, #0x250]        @ 0x148d4 <_lgao_connect_renderer+0x4b8>
   14684: 447b         	add	r3, pc
   14686: 9300         	str	r3, [sp]
   14688: f240 739d    	movw	r3, #0x79d
   1468c: f7ef ec8e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x106e4
   14690: e73a         	b	0x14508 <_lgao_connect_renderer+0xec> @ imm = #-0x18c
   14692: 4a91         	ldr	r2, [pc, #0x244]        @ 0x148d8 <_lgao_connect_renderer+0x4bc>
   14694: 4638         	mov	r0, r7
   14696: 4b91         	ldr	r3, [pc, #0x244]        @ 0x148dc <_lgao_connect_renderer+0x4c0>
   14698: 447a         	add	r2, pc
   1469a: 447b         	add	r3, pc
   1469c: 3250         	adds	r2, #0x50
   1469e: 9300         	str	r3, [sp]
   146a0: f240 73a9    	movw	r3, #0x7a9
   146a4: f7ef ec82    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x106fc
   146a8: e76f         	b	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x122
   146aa: f7ef ea6a    	blx	0x3b80 <ModuleRegistry_SearchInstByUid@plt> @ imm = #-0x10b2c
   146ae: f8c4 01cc    	str.w	r0, [r4, #0x1cc]
   146b2: 2800         	cmp	r0, #0x0
   146b4: d05c         	beq	0x14770 <_lgao_connect_renderer+0x354> @ imm = #0xb8
   146b6: 2500         	movs	r5, #0x0
   146b8: 9906         	ldr	r1, [sp, #0x18]
   146ba: 9500         	str	r5, [sp]
   146bc: 2201         	movs	r2, #0x1
   146be: f8d4 31c8    	ldr.w	r3, [r4, #0x1c8]
   146c2: f8d4 01b4    	ldr.w	r0, [r4, #0x1b4]
   146c6: f7ef ebf2    	blx	0x3eac <aimc_module_connect@plt> @ imm = #-0x1081c
   146ca: b300         	cbz	r0, 0x1470e <_lgao_connect_renderer+0x2f2> @ imm = #0x40
   146cc: 68fb         	ldr	r3, [r7, #0xc]
   146ce: 681b         	ldr	r3, [r3]
   146d0: 07de         	lsls	r6, r3, #0x1f
   146d2: f57f af5a    	bpl.w	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x14c
   146d6: 4a82         	ldr	r2, [pc, #0x208]        @ 0x148e0 <_lgao_connect_renderer+0x4c4>
   146d8: 4629         	mov	r1, r5
   146da: 4b82         	ldr	r3, [pc, #0x208]        @ 0x148e4 <_lgao_connect_renderer+0x4c8>
   146dc: 4638         	mov	r0, r7
   146de: 447a         	add	r2, pc
   146e0: 447b         	add	r3, pc
   146e2: 3250         	adds	r2, #0x50
   146e4: 9300         	str	r3, [sp]
   146e6: f240 73b4    	movw	r3, #0x7b4
   146ea: f7ef ec60    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10740
   146ee: e74c         	b	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x168
   146f0: 9b06         	ldr	r3, [sp, #0x18]
   146f2: 2103         	movs	r1, #0x3
   146f4: 4a7c         	ldr	r2, [pc, #0x1f0]        @ 0x148e8 <_lgao_connect_renderer+0x4cc>
   146f6: 4638         	mov	r0, r7
   146f8: 9301         	str	r3, [sp, #0x4]
   146fa: 4b7c         	ldr	r3, [pc, #0x1f0]        @ 0x148ec <_lgao_connect_renderer+0x4d0>
   146fc: 447a         	add	r2, pc
   146fe: 3250         	adds	r2, #0x50
   14700: 447b         	add	r3, pc
   14702: 9300         	str	r3, [sp]
   14704: f240 73a4    	movw	r3, #0x7a4
   14708: f7ef ec50    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10760
   1470c: e70e         	b	0x1452c <_lgao_connect_renderer+0x110> @ imm = #-0x1e4
   1470e: 4b78         	ldr	r3, [pc, #0x1e0]        @ 0x148f0 <_lgao_connect_renderer+0x4d4>
   14710: 4622         	mov	r2, r4
   14712: f8d4 01cc    	ldr.w	r0, [r4, #0x1cc]
   14716: 58f1         	ldr	r1, [r6, r3]
   14718: f7ef e9dc    	blx	0x3ad4 <module_appsink_set_callback@plt> @ imm = #-0x10c48
   1471c: f245 2103    	movw	r1, #0x5203
   14720: 6820         	ldr	r0, [r4]
   14722: 4642         	mov	r2, r8
   14724: f2c8 0104    	movt	r1, #0x8004
   14728: f7ef ea9a    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x10acc
   1472c: 68fb         	ldr	r3, [r7, #0xc]
   1472e: 681b         	ldr	r3, [r3]
   14730: 071d         	lsls	r5, r3, #0x1c
   14732: d477         	bmi	0x14824 <_lgao_connect_renderer+0x408> @ imm = #0xee
   14734: 2600         	movs	r6, #0x0
   14736: 9b05         	ldr	r3, [sp, #0x14]
   14738: 9600         	str	r6, [sp]
   1473a: 4632         	mov	r2, r6
   1473c: f8d4 11d4    	ldr.w	r1, [r4, #0x1d4]
   14740: f8d4 01b4    	ldr.w	r0, [r4, #0x1b4]
   14744: f7ef ebb2    	blx	0x3eac <aimc_module_connect@plt> @ imm = #-0x1089c
   14748: 4605         	mov	r5, r0
   1474a: b318         	cbz	r0, 0x14794 <_lgao_connect_renderer+0x378> @ imm = #0x46
   1474c: 68fb         	ldr	r3, [r7, #0xc]
   1474e: 681b         	ldr	r3, [r3]
   14750: 07d9         	lsls	r1, r3, #0x1f
   14752: f57f af1a    	bpl.w	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x1cc
   14756: 4a67         	ldr	r2, [pc, #0x19c]        @ 0x148f4 <_lgao_connect_renderer+0x4d8>
   14758: 4631         	mov	r1, r6
   1475a: 4b67         	ldr	r3, [pc, #0x19c]        @ 0x148f8 <_lgao_connect_renderer+0x4dc>
   1475c: 4638         	mov	r0, r7
   1475e: 447a         	add	r2, pc
   14760: 447b         	add	r3, pc
   14762: 3250         	adds	r2, #0x50
   14764: 9300         	str	r3, [sp]
   14766: f44f 63f8    	mov.w	r3, #0x7c0
   1476a: f7ef ec20    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x107c0
   1476e: e70c         	b	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x1e8
   14770: 68fb         	ldr	r3, [r7, #0xc]
   14772: 681b         	ldr	r3, [r3]
   14774: 07db         	lsls	r3, r3, #0x1f
   14776: f57f af08    	bpl.w	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x1f0
   1477a: 4a60         	ldr	r2, [pc, #0x180]        @ 0x148fc <_lgao_connect_renderer+0x4e0>
   1477c: 4601         	mov	r1, r0
   1477e: 4b60         	ldr	r3, [pc, #0x180]        @ 0x14900 <_lgao_connect_renderer+0x4e4>
   14780: 4638         	mov	r0, r7
   14782: 447a         	add	r2, pc
   14784: 447b         	add	r3, pc
   14786: 3250         	adds	r2, #0x50
   14788: 9300         	str	r3, [sp]
   1478a: f240 73af    	movw	r3, #0x7af
   1478e: f7ef ec0e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x107e4
   14792: e6fa         	b	0x1458a <_lgao_connect_renderer+0x16e> @ imm = #-0x20c
   14794: f245 2124    	movw	r1, #0x5224
   14798: f504 72ee    	add.w	r2, r4, #0x1dc
   1479c: f2c4 0101    	movt	r1, #0x4001
   147a0: 6820         	ldr	r0, [r4]
   147a2: f7ef ea5e    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x10b44
   147a6: f245 2127    	movw	r1, #0x5227
   147aa: f504 72f4    	add.w	r2, r4, #0x1e8
   147ae: f2c4 0104    	movt	r1, #0x4004
   147b2: 6820         	ldr	r0, [r4]
   147b4: f7ef ea54    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x10b58
   147b8: f8d4 31b0    	ldr.w	r3, [r4, #0x1b0]
   147bc: e9cd 3507    	strd	r3, r5, [sp, #28]
   147c0: f7ef ec9e    	blx	0x4100 <getpid@plt>     @ imm = #-0x106c4
   147c4: f245 212b    	movw	r1, #0x522b
   147c8: 4603         	mov	r3, r0
   147ca: aa07         	add	r2, sp, #0x1c
   147cc: 6820         	ldr	r0, [r4]
   147ce: f2c4 0108    	movt	r1, #0x4008
   147d2: 9308         	str	r3, [sp, #0x20]
   147d4: f7ef ea44    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x10b78
   147d8: 68fb         	ldr	r3, [r7, #0xc]
   147da: 681b         	ldr	r3, [r3]
   147dc: 071a         	lsls	r2, r3, #0x1c
   147de: d444         	bmi	0x1486a <_lgao_connect_renderer+0x44e> @ imm = #0x88
   147e0: 4b48         	ldr	r3, [pc, #0x120]        @ 0x14904 <_lgao_connect_renderer+0x4e8>
   147e2: 2204         	movs	r2, #0x4
   147e4: a80a         	add	r0, sp, #0x28
   147e6: 922a         	str	r2, [sp, #0xa8]
   147e8: 447b         	add	r3, pc
   147ea: 9309         	str	r3, [sp, #0x24]
   147ec: f7ee efe0    	blx	0x37b0 <sigemptyset@plt> @ imm = #-0x11040
   147f0: 2200         	movs	r2, #0x0
   147f2: a909         	add	r1, sp, #0x24
   147f4: 200a         	movs	r0, #0xa
   147f6: f7ef ebec    	blx	0x3fd0 <sigaction@plt>  @ imm = #-0x10828
   147fa: 3001         	adds	r0, #0x1
   147fc: f47f ae3c    	bne.w	0x14478 <_lgao_connect_renderer+0x5c> @ imm = #-0x388
   14800: 68fb         	ldr	r3, [r7, #0xc]
   14802: 681b         	ldr	r3, [r3]
   14804: 07db         	lsls	r3, r3, #0x1f
   14806: f57f ae37    	bpl.w	0x14478 <_lgao_connect_renderer+0x5c> @ imm = #-0x392
   1480a: 4a3f         	ldr	r2, [pc, #0xfc]         @ 0x14908 <_lgao_connect_renderer+0x4ec>
   1480c: f240 736f    	movw	r3, #0x76f
   14810: 493e         	ldr	r1, [pc, #0xf8]         @ 0x1490c <_lgao_connect_renderer+0x4f0>
   14812: 4638         	mov	r0, r7
   14814: 447a         	add	r2, pc
   14816: 4479         	add	r1, pc
   14818: 3268         	adds	r2, #0x68
   1481a: 9100         	str	r1, [sp]
   1481c: 2100         	movs	r1, #0x0
   1481e: f7ef ebc6    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10874
   14822: e629         	b	0x14478 <_lgao_connect_renderer+0x5c> @ imm = #-0x3ae
   14824: f8d4 21d8    	ldr.w	r2, [r4, #0x1d8]
   14828: 2103         	movs	r1, #0x3
   1482a: 4d39         	ldr	r5, [pc, #0xe4]         @ 0x14910 <_lgao_connect_renderer+0x4f4>
   1482c: 4638         	mov	r0, r7
   1482e: 4b39         	ldr	r3, [pc, #0xe4]         @ 0x14914 <_lgao_connect_renderer+0x4f8>
   14830: 6812         	ldr	r2, [r2]
   14832: 447d         	add	r5, pc
   14834: 447b         	add	r3, pc
   14836: 3550         	adds	r5, #0x50
   14838: 9300         	str	r3, [sp]
   1483a: f240 73bc    	movw	r3, #0x7bc
   1483e: 9201         	str	r2, [sp, #0x4]
   14840: 462a         	mov	r2, r5
   14842: f7ef ebb4    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x10898
   14846: 68fb         	ldr	r3, [r7, #0xc]
   14848: 681b         	ldr	r3, [r3]
   1484a: 0718         	lsls	r0, r3, #0x1c
   1484c: f57f af72    	bpl.w	0x14734 <_lgao_connect_renderer+0x318> @ imm = #-0x11c
   14850: 4b31         	ldr	r3, [pc, #0xc4]         @ 0x14918 <_lgao_connect_renderer+0x4fc>
   14852: 462a         	mov	r2, r5
   14854: 9905         	ldr	r1, [sp, #0x14]
   14856: 4638         	mov	r0, r7
   14858: 447b         	add	r3, pc
   1485a: e9cd 3100    	strd	r3, r1, [sp]
   1485e: f240 73bd    	movw	r3, #0x7bd
   14862: 2103         	movs	r1, #0x3
   14864: f7ef eba2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x108bc
   14868: e764         	b	0x14734 <_lgao_connect_renderer+0x318> @ imm = #-0x138
   1486a: e9dd 3107    	ldrd	r3, r1, [sp, #28]
   1486e: 4638         	mov	r0, r7
   14870: 4a2a         	ldr	r2, [pc, #0xa8]         @ 0x1491c <_lgao_connect_renderer+0x500>
   14872: e9cd 3101    	strd	r3, r1, [sp, #4]
   14876: f44f 63ed    	mov.w	r3, #0x768
   1487a: 4929         	ldr	r1, [pc, #0xa4]         @ 0x14920 <_lgao_connect_renderer+0x504>
   1487c: 447a         	add	r2, pc
   1487e: 3268         	adds	r2, #0x68
   14880: 4479         	add	r1, pc
   14882: 9100         	str	r1, [sp]
   14884: 2103         	movs	r1, #0x3
   14886: f7ef eb92    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x108dc
   1488a: e7a9         	b	0x147e0 <_lgao_connect_renderer+0x3c4> @ imm = #-0xae
   1488c: e4 ba 00 00  	.word	0x0000bae4
   14890: 9c d7 01 00  	.word	0x0001d79c
   14894: 00 04 00 00  	.word	0x00000400
   14898: c8 ba 00 00  	.word	0x0000bac8
   1489c: 9a c9 00 00  	.word	0x0000c99a
   148a0: 80 b9 00 00  	.word	0x0000b980
   148a4: 64 c9 00 00  	.word	0x0000c964
   148a8: ca b9 00 00  	.word	0x0000b9ca
   148ac: 3c c9 00 00  	.word	0x0000c93c
   148b0: 3e b9 00 00  	.word	0x0000b93e
   148b4: 1e c9 00 00  	.word	0x0000c91e
   148b8: 42 b9 00 00  	.word	0x0000b942
   148bc: fc c8 00 00  	.word	0x0000c8fc
   148c0: 4a b9 00 00  	.word	0x0000b94a
   148c4: de c8 00 00  	.word	0x0000c8de
   148c8: 8c b9 00 00  	.word	0x0000b98c
   148cc: c0 c8 00 00  	.word	0x0000c8c0
   148d0: 3e b9 00 00  	.word	0x0000b93e
   148d4: 28 b9 00 00  	.word	0x0000b928
   148d8: 80 c8 00 00  	.word	0x0000c880
   148dc: 6a b9 00 00  	.word	0x0000b96a
   148e0: 3a c8 00 00  	.word	0x0000c83a
   148e4: e8 b8 00 00  	.word	0x0000b8e8
   148e8: 1c c8 00 00  	.word	0x0000c81c
   148ec: e0 b8 00 00  	.word	0x0000b8e0
   148f0: 14 04 00 00  	.word	0x00000414
   148f4: ba c7 00 00  	.word	0x0000c7ba
   148f8: 68 b8 00 00  	.word	0x0000b868
   148fc: 96 c7 00 00  	.word	0x0000c796
   14900: a0 b8 00 00  	.word	0x0000b8a0
   14904: 41 03 00 00  	.word	0x00000341
   14908: 04 c7 00 00  	.word	0x0000c704
   1490c: 7a b8 00 00  	.word	0x0000b87a
   14910: e6 c6 00 00  	.word	0x0000c6e6
   14914: 14 b8 00 00  	.word	0x0000b814
   14918: 04 b8 00 00  	.word	0x0000b804
   1491c: 9c c6 00 00  	.word	0x0000c69c
   14920: f8 b7 00 00  	.word	0x0000b7f8
