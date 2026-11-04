
/Users/max/projects/dts_restore_plus/re/webos25-audio-policy/bin/usr/lib/gstreamer-1.0/libgstlgaudiosink.so:	file format elf32-littlearm

Disassembly of section .text:

000182d0 <lgao_push_first_event>:
   182d0: f8df 3488    	ldr.w	r3, [pc, #0x488]        @ 0x1875c <lgao_push_first_event+0x48c>
   182d4: f8df 2488    	ldr.w	r2, [pc, #0x488]        @ 0x18760 <lgao_push_first_event+0x490>
   182d8: e92d 43f0    	push.w	{r4, r5, r6, r7, r8, r9, lr}
   182dc: 4604         	mov	r4, r0
   182de: 447b         	add	r3, pc
   182e0: b097         	sub	sp, #0x5c
   182e2: 589e         	ldr	r6, [r3, r2]
   182e4: 68f3         	ldr	r3, [r6, #0xc]
   182e6: 681a         	ldr	r2, [r3]
   182e8: 2800         	cmp	r0, #0x0
   182ea: f000 81d0    	beq.w	0x1868e <lgao_push_first_event+0x3be> @ imm = #0x3a0
   182ee: 6887         	ldr	r7, [r0, #0x8]
   182f0: 1c79         	adds	r1, r7, #0x1
   182f2: f000 81d4    	beq.w	0x1869e <lgao_push_first_event+0x3ce> @ imm = #0x3a8
   182f6: 0715         	lsls	r5, r2, #0x1c
   182f8: 6c43         	ldr	r3, [r0, #0x44]
   182fa: f100 811a    	bmi.w	0x18532 <lgao_push_first_event+0x262> @ imm = #0x234
   182fe: 930d         	str	r3, [sp, #0x34]
   18300: f8df 3460    	ldr.w	r3, [pc, #0x460]        @ 0x18764 <lgao_push_first_event+0x494>
   18304: 6d22         	ldr	r2, [r4, #0x50]
   18306: 6aa1         	ldr	r1, [r4, #0x28]
   18308: 447b         	add	r3, pc
   1830a: f894 002d    	ldrb.w	r0, [r4, #0x2d]
   1830e: 9213         	str	r2, [sp, #0x4c]
   18310: 9302         	str	r3, [sp, #0x8]
   18312: f8df 2454    	ldr.w	r2, [pc, #0x454]        @ 0x18768 <lgao_push_first_event+0x498>
   18316: f8df 3454    	ldr.w	r3, [pc, #0x454]        @ 0x1876c <lgao_push_first_event+0x49c>
   1831a: 9104         	str	r1, [sp, #0x10]
   1831c: 2100         	movs	r1, #0x0
   1831e: 447a         	add	r2, pc
   18320: 9001         	str	r0, [sp, #0x4]
   18322: 447b         	add	r3, pc
   18324: 9211         	str	r2, [sp, #0x44]
   18326: 930e         	str	r3, [sp, #0x38]
   18328: 2201         	movs	r2, #0x1
   1832a: f8df 3444    	ldr.w	r3, [pc, #0x444]        @ 0x18770 <lgao_push_first_event+0x4a0>
   1832e: 9103         	str	r1, [sp, #0xc]
   18330: 9100         	str	r1, [sp]
   18332: 9114         	str	r1, [sp, #0x50]
   18334: 447b         	add	r3, pc
   18336: 9112         	str	r1, [sp, #0x48]
   18338: e9cd 120f    	strd	r1, r2, [sp, #60]
   1833c: 6be0         	ldr	r0, [r4, #0x3c]
   1833e: 920c         	str	r2, [sp, #0x30]
   18340: 9109         	str	r1, [sp, #0x24]
   18342: e9cd 030a    	strd	r0, r3, [sp, #40]
   18346: f8df 342c    	ldr.w	r3, [pc, #0x42c]        @ 0x18774 <lgao_push_first_event+0x4a4>
   1834a: f8df 042c    	ldr.w	r0, [pc, #0x42c]        @ 0x18778 <lgao_push_first_event+0x4a8>
   1834e: 9106         	str	r1, [sp, #0x18]
   18350: 6ba2         	ldr	r2, [r4, #0x38]
   18352: 447b         	add	r3, pc
   18354: 4478         	add	r0, pc
   18356: 9305         	str	r3, [sp, #0x14]
   18358: f8df 3420    	ldr.w	r3, [pc, #0x420]        @ 0x1877c <lgao_push_first_event+0x4ac>
   1835c: 9207         	str	r2, [sp, #0x1c]
   1835e: f894 202c    	ldrb.w	r2, [r4, #0x2c]
   18362: 9008         	str	r0, [sp, #0x20]
   18364: 447b         	add	r3, pc
   18366: f8df 0418    	ldr.w	r0, [pc, #0x418]        @ 0x18780 <lgao_push_first_event+0x4b0>
   1836a: 4478         	add	r0, pc
   1836c: f7eb ed50    	blx	0x3e10 <aud_event_newv_first@plt> @ imm = #-0x14560
   18370: 6da2         	ldr	r2, [r4, #0x58]
   18372: 4605         	mov	r5, r0
   18374: 2a00         	cmp	r2, #0x0
   18376: f040 80d7    	bne.w	0x18528 <lgao_push_first_event+0x258> @ imm = #0x1ae
   1837a: f894 31dd    	ldrb.w	r3, [r4, #0x1dd]
   1837e: 2b00         	cmp	r3, #0x0
   18380: f040 80cb    	bne.w	0x1851a <lgao_push_first_event+0x24a> @ imm = #0x196
   18384: 6fe3         	ldr	r3, [r4, #0x7c]
   18386: 2b00         	cmp	r3, #0x0
   18388: f040 80c0    	bne.w	0x1850c <lgao_push_first_event+0x23c> @ imm = #0x180
   1838c: 2d00         	cmp	r5, #0x0
   1838e: f000 81a4    	beq.w	0x186da <lgao_push_first_event+0x40a> @ imm = #0x348
   18392: 4628         	mov	r0, r5
   18394: f7eb ec70    	blx	0x3c78 <aud_event_str@plt> @ imm = #-0x14720
   18398: 4607         	mov	r7, r0
   1839a: b138         	cbz	r0, 0x183ac <lgao_push_first_event+0xdc> @ imm = #0xe
   1839c: 68f3         	ldr	r3, [r6, #0xc]
   1839e: 681b         	ldr	r3, [r3]
   183a0: 075b         	lsls	r3, r3, #0x1d
   183a2: f100 811d    	bmi.w	0x185e0 <lgao_push_first_event+0x310> @ imm = #0x23a
   183a6: 4638         	mov	r0, r7
   183a8: f7eb ebde    	blx	0x3b68 <free@plt>       @ imm = #-0x14844
   183ac: 6c23         	ldr	r3, [r4, #0x40]
   183ae: 2b00         	cmp	r3, #0x0
   183b0: f040 810c    	bne.w	0x185cc <lgao_push_first_event+0x2fc> @ imm = #0x218
   183b4: f8b4 31d0    	ldrh.w	r3, [r4, #0x1d0]
   183b8: 2b00         	cmp	r3, #0x0
   183ba: d03c         	beq	0x18436 <lgao_push_first_event+0x166> @ imm = #0x78
   183bc: 68f3         	ldr	r3, [r6, #0xc]
   183be: 681b         	ldr	r3, [r3]
   183c0: 075f         	lsls	r7, r3, #0x1d
   183c2: f100 8139    	bmi.w	0x18638 <lgao_push_first_event+0x368> @ imm = #0x272
   183c6: f894 302c    	ldrb.w	r3, [r4, #0x2c]
   183ca: 2200         	movs	r2, #0x0
   183cc: 4fed         	ldr	r7, [pc, #0x3b4]        @ 0x18784 <lgao_push_first_event+0x4b4>
   183ce: 4628         	mov	r0, r5
   183d0: 9200         	str	r2, [sp]
   183d2: 2b03         	cmp	r3, #0x3
   183d4: bf34         	ite	lo
   183d6: f04f 0902    	movlo.w	r9, #0x2
   183da: f04f 0907    	movhs.w	r9, #0x7
   183de: 447f         	add	r7, pc
   183e0: 464b         	mov	r3, r9
   183e2: 4639         	mov	r1, r7
   183e4: f7eb ec0a    	blx	0x3bfc <aud_event_dup_and_set@plt> @ imm = #-0x147ec
   183e8: 4680         	mov	r8, r0
   183ea: 2800         	cmp	r0, #0x0
   183ec: f000 8193    	beq.w	0x18716 <lgao_push_first_event+0x446> @ imm = #0x326
   183f0: 4639         	mov	r1, r7
   183f2: 464a         	mov	r2, r9
   183f4: 4628         	mov	r0, r5
   183f6: f7eb ec9a    	blx	0x3d2c <aud_event_value_set_int@plt> @ imm = #-0x146cc
   183fa: 68f3         	ldr	r3, [r6, #0xc]
   183fc: 681b         	ldr	r3, [r3]
   183fe: 0758         	lsls	r0, r3, #0x1d
   18400: f100 8135    	bmi.w	0x1866e <lgao_push_first_event+0x39e> @ imm = #0x26a
   18404: f894 31d0    	ldrb.w	r3, [r4, #0x1d0]
   18408: 2b00         	cmp	r3, #0x0
   1840a: f040 8123    	bne.w	0x18654 <lgao_push_first_event+0x384> @ imm = #0x246
   1840e: f8d4 71e0    	ldr.w	r7, [r4, #0x1e0]
   18412: 2f00         	cmp	r7, #0x0
   18414: d068         	beq	0x184e8 <lgao_push_first_event+0x218> @ imm = #0xd0
   18416: 68f3         	ldr	r3, [r6, #0xc]
   18418: 681b         	ldr	r3, [r3]
   1841a: 075b         	lsls	r3, r3, #0x1d
   1841c: f100 80fe    	bmi.w	0x1861c <lgao_push_first_event+0x34c> @ imm = #0x1fc
   18420: 4628         	mov	r0, r5
   18422: 2700         	movs	r7, #0x0
   18424: f7eb eeba    	blx	0x419c <aud_object_put@plt> @ imm = #-0x1428c
   18428: 4640         	mov	r0, r8
   1842a: f7eb eeb8    	blx	0x419c <aud_object_put@plt> @ imm = #-0x14290
   1842e: 4638         	mov	r0, r7
   18430: b017         	add	sp, #0x5c
   18432: e8bd 83f0    	pop.w	{r4, r5, r6, r7, r8, r9, pc}
   18436: 6f61         	ldr	r1, [r4, #0x74]
   18438: f04f 0902    	mov.w	r9, #0x2
   1843c: 6fa2         	ldr	r2, [r4, #0x78]
   1843e: 4628         	mov	r0, r5
   18440: 2900         	cmp	r1, #0x0
   18442: bfde         	ittt	le
   18444: 2302         	movle	r3, #0x2
   18446: 6763         	strle	r3, [r4, #0x74]
   18448: 4619         	movle	r1, r3
   1844a: 6f23         	ldr	r3, [r4, #0x70]
   1844c: 2b00         	cmp	r3, #0x0
   1844e: bfdc         	itt	le
   18450: 2310         	movle	r3, #0x10
   18452: 6723         	strle	r3, [r4, #0x70]
   18454: 2a00         	cmp	r2, #0x0
   18456: bfdc         	itt	le
   18458: f64b 3280    	movwle	r2, #0xbb80
   1845c: 67a2         	strle	r2, [r4, #0x78]
   1845e: 930e         	str	r3, [sp, #0x38]
   18460: 4bc9         	ldr	r3, [pc, #0x324]        @ 0x18788 <lgao_push_first_event+0x4b8>
   18462: 9211         	str	r2, [sp, #0x44]
   18464: 4ac9         	ldr	r2, [pc, #0x324]        @ 0x1878c <lgao_push_first_event+0x4bc>
   18466: 447b         	add	r3, pc
   18468: 910b         	str	r1, [sp, #0x2c]
   1846a: 9309         	str	r3, [sp, #0x24]
   1846c: 4bc8         	ldr	r3, [pc, #0x320]        @ 0x18790 <lgao_push_first_event+0x4c0>
   1846e: 447a         	add	r2, pc
   18470: 920c         	str	r2, [sp, #0x30]
   18472: 4ac8         	ldr	r2, [pc, #0x320]        @ 0x18794 <lgao_push_first_event+0x4c4>
   18474: 447b         	add	r3, pc
   18476: 6ce1         	ldr	r1, [r4, #0x4c]
   18478: 9306         	str	r3, [sp, #0x18]
   1847a: 4bc7         	ldr	r3, [pc, #0x31c]        @ 0x18798 <lgao_push_first_event+0x4c8>
   1847c: 447a         	add	r2, pc
   1847e: e9cd 1202    	strd	r1, r2, [sp, #8]
   18482: f104 0260    	add.w	r2, r4, #0x60
   18486: 9208         	str	r2, [sp, #0x20]
   18488: 2101         	movs	r1, #0x1
   1848a: 447b         	add	r3, pc
   1848c: f8cd 901c    	str.w	r9, [sp, #0x1c]
   18490: 9300         	str	r3, [sp]
   18492: 6ca2         	ldr	r2, [r4, #0x48]
   18494: 6ba3         	ldr	r3, [r4, #0x38]
   18496: e9cd 1104    	strd	r1, r1, [sp, #16]
   1849a: 9214         	str	r2, [sp, #0x50]
   1849c: 4abf         	ldr	r2, [pc, #0x2fc]        @ 0x1879c <lgao_push_first_event+0x4cc>
   1849e: 9101         	str	r1, [sp, #0x4]
   184a0: 9113         	str	r1, [sp, #0x4c]
   184a2: 447a         	add	r2, pc
   184a4: 49be         	ldr	r1, [pc, #0x2f8]        @ 0x187a0 <lgao_push_first_event+0x4d0>
   184a6: 9212         	str	r2, [sp, #0x48]
   184a8: 4abe         	ldr	r2, [pc, #0x2f8]        @ 0x187a4 <lgao_push_first_event+0x4d4>
   184aa: 4479         	add	r1, pc
   184ac: 447a         	add	r2, pc
   184ae: 920f         	str	r2, [sp, #0x3c]
   184b0: 2200         	movs	r2, #0x0
   184b2: 9215         	str	r2, [sp, #0x54]
   184b4: 9210         	str	r2, [sp, #0x40]
   184b6: 920d         	str	r2, [sp, #0x34]
   184b8: 920a         	str	r2, [sp, #0x28]
   184ba: f7eb eba0    	blx	0x3bfc <aud_event_dup_and_set@plt> @ imm = #-0x148c0
   184be: 4680         	mov	r8, r0
   184c0: 2800         	cmp	r0, #0x0
   184c2: f000 811e    	beq.w	0x18702 <lgao_push_first_event+0x432> @ imm = #0x23c
   184c6: f7eb ebd8    	blx	0x3c78 <aud_event_str@plt> @ imm = #-0x14850
   184ca: 4607         	mov	r7, r0
   184cc: 2800         	cmp	r0, #0x0
   184ce: d09e         	beq	0x1840e <lgao_push_first_event+0x13e> @ imm = #-0xc4
   184d0: 68f3         	ldr	r3, [r6, #0xc]
   184d2: 681b         	ldr	r3, [r3]
   184d4: 075a         	lsls	r2, r3, #0x1d
   184d6: f100 8092    	bmi.w	0x185fe <lgao_push_first_event+0x32e> @ imm = #0x124
   184da: 4638         	mov	r0, r7
   184dc: f7eb eb44    	blx	0x3b68 <free@plt>       @ imm = #-0x14978
   184e0: f8d4 71e0    	ldr.w	r7, [r4, #0x1e0]
   184e4: 2f00         	cmp	r7, #0x0
   184e6: d196         	bne	0x18416 <lgao_push_first_event+0x146> @ imm = #-0xd4
   184e8: 4629         	mov	r1, r5
   184ea: f8d4 01c4    	ldr.w	r0, [r4, #0x1c4]
   184ee: f7eb ec0c    	blx	0x3d08 <module_appsrc_push_event@plt> @ imm = #-0x147e8
   184f2: f8d4 01d8    	ldr.w	r0, [r4, #0x1d8]
   184f6: 4641         	mov	r1, r8
   184f8: f7eb ec06    	blx	0x3d08 <module_appsrc_push_event@plt> @ imm = #-0x147f4
   184fc: 4638         	mov	r0, r7
   184fe: f8c4 7098    	str.w	r7, [r4, #0x98]
   18502: f884 709c    	strb.w	r7, [r4, #0x9c]
   18506: b017         	add	sp, #0x5c
   18508: e8bd 83f0    	pop.w	{r4, r5, r6, r7, r8, r9, pc}
   1850c: 49a6         	ldr	r1, [pc, #0x298]        @ 0x187a8 <lgao_push_first_event+0x4d8>
   1850e: 2201         	movs	r2, #0x1
   18510: 4628         	mov	r0, r5
   18512: 4479         	add	r1, pc
   18514: f7eb ee54    	blx	0x41c0 <aud_event_value_set_bool@plt> @ imm = #-0x14358
   18518: e738         	b	0x1838c <lgao_push_first_event+0xbc> @ imm = #-0x190
   1851a: 49a4         	ldr	r1, [pc, #0x290]        @ 0x187ac <lgao_push_first_event+0x4dc>
   1851c: 2201         	movs	r2, #0x1
   1851e: 4628         	mov	r0, r5
   18520: 4479         	add	r1, pc
   18522: f7eb ee4e    	blx	0x41c0 <aud_event_value_set_bool@plt> @ imm = #-0x14364
   18526: e72d         	b	0x18384 <lgao_push_first_event+0xb4> @ imm = #-0x1a6
   18528: 49a1         	ldr	r1, [pc, #0x284]        @ 0x187b0 <lgao_push_first_event+0x4e0>
   1852a: 4479         	add	r1, pc
   1852c: f7eb ebfe    	blx	0x3d2c <aud_event_value_set_int@plt> @ imm = #-0x14804
   18530: e723         	b	0x1837a <lgao_push_first_event+0xaa> @ imm = #-0x1ba
   18532: 4da0         	ldr	r5, [pc, #0x280]        @ 0x187b4 <lgao_push_first_event+0x4e4>
   18534: 2103         	movs	r1, #0x3
   18536: 4aa0         	ldr	r2, [pc, #0x280]        @ 0x187b8 <lgao_push_first_event+0x4e8>
   18538: 4630         	mov	r0, r6
   1853a: 9301         	str	r3, [sp, #0x4]
   1853c: f640 23ed    	movw	r3, #0xaed
   18540: 447d         	add	r5, pc
   18542: 447a         	add	r2, pc
   18544: f205 5564    	addw	r5, r5, #0x564
   18548: 9200         	str	r2, [sp]
   1854a: 462a         	mov	r2, r5
   1854c: f7eb ed2e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x145a4
   18550: 68f3         	ldr	r3, [r6, #0xc]
   18552: 681b         	ldr	r3, [r3]
   18554: 0718         	lsls	r0, r3, #0x1c
   18556: d537         	bpl	0x185c8 <lgao_push_first_event+0x2f8> @ imm = #0x6e
   18558: 4b98         	ldr	r3, [pc, #0x260]        @ 0x187bc <lgao_push_first_event+0x4ec>
   1855a: 462a         	mov	r2, r5
   1855c: 6fe1         	ldr	r1, [r4, #0x7c]
   1855e: 4630         	mov	r0, r6
   18560: 447b         	add	r3, pc
   18562: e9cd 3100    	strd	r3, r1, [sp]
   18566: f640 23ee    	movw	r3, #0xaee
   1856a: 2103         	movs	r1, #0x3
   1856c: f7eb ed1e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x145c4
   18570: 68f3         	ldr	r3, [r6, #0xc]
   18572: 681b         	ldr	r3, [r3]
   18574: 0719         	lsls	r1, r3, #0x1c
   18576: d527         	bpl	0x185c8 <lgao_push_first_event+0x2f8> @ imm = #0x4e
   18578: 7923         	ldrb	r3, [r4, #0x4]
   1857a: bb73         	cbnz	r3, 0x185da <lgao_push_first_event+0x30a> @ imm = #0x5c
   1857c: 4b90         	ldr	r3, [pc, #0x240]        @ 0x187c0 <lgao_push_first_event+0x4f0>
   1857e: 447b         	add	r3, pc
   18580: 9301         	str	r3, [sp, #0x4]
   18582: edd4 7a36    	vldr	s15, [r4, #216]
   18586: eddf 6a74    	vldr	s13, [pc, #464]         @ 0x18758 <lgao_push_first_event+0x488>
   1858a: e9d4 013a    	ldrd	r0, r1, [r4, #232]
   1858e: 4a8d         	ldr	r2, [pc, #0x234]        @ 0x187c4 <lgao_push_first_event+0x4f4>
   18590: eeb8 7ae7    	vcvt.f32.s32	s14, s15
   18594: e9cd 0106    	strd	r0, r1, [sp, #24]
   18598: e9d4 0138    	ldrd	r0, r1, [r4, #224]
   1859c: 447a         	add	r2, pc
   1859e: 4b8a         	ldr	r3, [pc, #0x228]        @ 0x187c8 <lgao_push_first_event+0x4f8>
   185a0: f202 5264    	addw	r2, r2, #0x564
   185a4: eec7 7a26    	vdiv.f32	s15, s14, s13
   185a8: e9cd 0104    	strd	r0, r1, [sp, #16]
   185ac: 4630         	mov	r0, r6
   185ae: 68a1         	ldr	r1, [r4, #0x8]
   185b0: 447b         	add	r3, pc
   185b2: 9300         	str	r3, [sp]
   185b4: f44f 632f    	mov.w	r3, #0xaf0
   185b8: 9102         	str	r1, [sp, #0x8]
   185ba: 2103         	movs	r1, #0x3
   185bc: eef7 0ae7    	vcvt.f64.f32	d16, s15
   185c0: edcd 0b08    	vstr	d16, [sp, #32]
   185c4: f7eb ecf2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x1461c
   185c8: 6c63         	ldr	r3, [r4, #0x44]
   185ca: e698         	b	0x182fe <lgao_push_first_event+0x2e> @ imm = #-0x2d0
   185cc: 497f         	ldr	r1, [pc, #0x1fc]        @ 0x187cc <lgao_push_first_event+0x4fc>
   185ce: 2201         	movs	r2, #0x1
   185d0: 4628         	mov	r0, r5
   185d2: 4479         	add	r1, pc
   185d4: f7eb edf4    	blx	0x41c0 <aud_event_value_set_bool@plt> @ imm = #-0x14418
   185d8: e6ec         	b	0x183b4 <lgao_push_first_event+0xe4> @ imm = #-0x228
   185da: 4b7d         	ldr	r3, [pc, #0x1f4]        @ 0x187d0 <lgao_push_first_event+0x500>
   185dc: 447b         	add	r3, pc
   185de: e7cf         	b	0x18580 <lgao_push_first_event+0x2b0> @ imm = #-0x62
   185e0: 4a7c         	ldr	r2, [pc, #0x1f0]        @ 0x187d4 <lgao_push_first_event+0x504>
   185e2: 2102         	movs	r1, #0x2
   185e4: 4b7c         	ldr	r3, [pc, #0x1f0]        @ 0x187d8 <lgao_push_first_event+0x508>
   185e6: 9001         	str	r0, [sp, #0x4]
   185e8: 4630         	mov	r0, r6
   185ea: 447a         	add	r2, pc
   185ec: 447b         	add	r3, pc
   185ee: f202 5264    	addw	r2, r2, #0x564
   185f2: 9300         	str	r3, [sp]
   185f4: f640 331d    	movw	r3, #0xb1d
   185f8: f7eb ecd8    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x14650
   185fc: e6d3         	b	0x183a6 <lgao_push_first_event+0xd6> @ imm = #-0x25a
   185fe: 4a77         	ldr	r2, [pc, #0x1dc]        @ 0x187dc <lgao_push_first_event+0x50c>
   18600: 4649         	mov	r1, r9
   18602: 4b77         	ldr	r3, [pc, #0x1dc]        @ 0x187e0 <lgao_push_first_event+0x510>
   18604: 9001         	str	r0, [sp, #0x4]
   18606: 4630         	mov	r0, r6
   18608: 447a         	add	r2, pc
   1860a: 447b         	add	r3, pc
   1860c: f202 5264    	addw	r2, r2, #0x564
   18610: 9300         	str	r3, [sp]
   18612: f640 3354    	movw	r3, #0xb54
   18616: f7eb ecca    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x1466c
   1861a: e75e         	b	0x184da <lgao_push_first_event+0x20a> @ imm = #-0x144
   1861c: 4a71         	ldr	r2, [pc, #0x1c4]        @ 0x187e4 <lgao_push_first_event+0x514>
   1861e: 4630         	mov	r0, r6
   18620: 4b71         	ldr	r3, [pc, #0x1c4]        @ 0x187e8 <lgao_push_first_event+0x518>
   18622: 2102         	movs	r1, #0x2
   18624: 447a         	add	r2, pc
   18626: 447b         	add	r3, pc
   18628: f202 5264    	addw	r2, r2, #0x564
   1862c: 9300         	str	r3, [sp]
   1862e: f640 335a    	movw	r3, #0xb5a
   18632: f7eb ecbc    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x14688
   18636: e6f3         	b	0x18420 <lgao_push_first_event+0x150> @ imm = #-0x21a
   18638: 4a6c         	ldr	r2, [pc, #0x1b0]        @ 0x187ec <lgao_push_first_event+0x51c>
   1863a: 2102         	movs	r1, #0x2
   1863c: 4b6c         	ldr	r3, [pc, #0x1b0]        @ 0x187f0 <lgao_push_first_event+0x520>
   1863e: 4630         	mov	r0, r6
   18640: 447a         	add	r2, pc
   18642: 447b         	add	r3, pc
   18644: f202 5264    	addw	r2, r2, #0x564
   18648: 9300         	str	r3, [sp]
   1864a: f640 3326    	movw	r3, #0xb26
   1864e: f7eb ecae    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x146a4
   18652: e6b8         	b	0x183c6 <lgao_push_first_event+0xf6> @ imm = #-0x290
   18654: 4f67         	ldr	r7, [pc, #0x19c]        @ 0x187f4 <lgao_push_first_event+0x524>
   18656: 2201         	movs	r2, #0x1
   18658: 4628         	mov	r0, r5
   1865a: 447f         	add	r7, pc
   1865c: 4639         	mov	r1, r7
   1865e: f7eb edb0    	blx	0x41c0 <aud_event_value_set_bool@plt> @ imm = #-0x144a0
   18662: 4639         	mov	r1, r7
   18664: 2201         	movs	r2, #0x1
   18666: 4640         	mov	r0, r8
   18668: f7eb edaa    	blx	0x41c0 <aud_event_value_set_bool@plt> @ imm = #-0x144ac
   1866c: e6cf         	b	0x1840e <lgao_push_first_event+0x13e> @ imm = #-0x262
   1866e: 4b62         	ldr	r3, [pc, #0x188]        @ 0x187f8 <lgao_push_first_event+0x528>
   18670: 2102         	movs	r1, #0x2
   18672: 4a62         	ldr	r2, [pc, #0x188]        @ 0x187fc <lgao_push_first_event+0x52c>
   18674: 4630         	mov	r0, r6
   18676: f8cd 9004    	str.w	r9, [sp, #0x4]
   1867a: 447b         	add	r3, pc
   1867c: 447a         	add	r2, pc
   1867e: 9300         	str	r3, [sp]
   18680: f202 5264    	addw	r2, r2, #0x564
   18684: f640 3333    	movw	r3, #0xb33
   18688: f7eb ec90    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x146e0
   1868c: e6ba         	b	0x18404 <lgao_push_first_event+0x134> @ imm = #-0x28c
   1868e: 07d0         	lsls	r0, r2, #0x1f
   18690: d415         	bmi	0x186be <lgao_push_first_event+0x3ee> @ imm = #0x2a
   18692: f04f 37ff    	mov.w	r7, #0xffffffff
   18696: 4638         	mov	r0, r7
   18698: b017         	add	sp, #0x5c
   1869a: e8bd 83f0    	pop.w	{r4, r5, r6, r7, r8, r9, pc}
   1869e: 07d3         	lsls	r3, r2, #0x1f
   186a0: d5f7         	bpl	0x18692 <lgao_push_first_event+0x3c2> @ imm = #-0x12
   186a2: 4a57         	ldr	r2, [pc, #0x15c]        @ 0x18800 <lgao_push_first_event+0x530>
   186a4: f640 23eb    	movw	r3, #0xaeb
   186a8: 4956         	ldr	r1, [pc, #0x158]        @ 0x18804 <lgao_push_first_event+0x534>
   186aa: 4630         	mov	r0, r6
   186ac: 447a         	add	r2, pc
   186ae: 4479         	add	r1, pc
   186b0: f202 5264    	addw	r2, r2, #0x564
   186b4: 9100         	str	r1, [sp]
   186b6: 2100         	movs	r1, #0x0
   186b8: f7eb ec78    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x14710
   186bc: e6b7         	b	0x1842e <lgao_push_first_event+0x15e> @ imm = #-0x292
   186be: 4a52         	ldr	r2, [pc, #0x148]        @ 0x18808 <lgao_push_first_event+0x538>
   186c0: f640 23ea    	movw	r3, #0xaea
   186c4: 4d51         	ldr	r5, [pc, #0x144]        @ 0x1880c <lgao_push_first_event+0x53c>
   186c6: 4621         	mov	r1, r4
   186c8: 4630         	mov	r0, r6
   186ca: 447a         	add	r2, pc
   186cc: 447d         	add	r5, pc
   186ce: f202 5264    	addw	r2, r2, #0x564
   186d2: 9500         	str	r5, [sp]
   186d4: f7eb ec6a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x1472c
   186d8: e7db         	b	0x18692 <lgao_push_first_event+0x3c2> @ imm = #-0x4a
   186da: 68f3         	ldr	r3, [r6, #0xc]
   186dc: 681b         	ldr	r3, [r3]
   186de: 07da         	lsls	r2, r3, #0x1f
   186e0: d5d7         	bpl	0x18692 <lgao_push_first_event+0x3c2> @ imm = #-0x52
   186e2: 4a4b         	ldr	r2, [pc, #0x12c]        @ 0x18810 <lgao_push_first_event+0x540>
   186e4: f640 3317    	movw	r3, #0xb17
   186e8: 4c4a         	ldr	r4, [pc, #0x128]        @ 0x18814 <lgao_push_first_event+0x544>
   186ea: 4629         	mov	r1, r5
   186ec: 4630         	mov	r0, r6
   186ee: f04f 37ff    	mov.w	r7, #0xffffffff
   186f2: 447a         	add	r2, pc
   186f4: 447c         	add	r4, pc
   186f6: f202 5264    	addw	r2, r2, #0x564
   186fa: 9400         	str	r4, [sp]
   186fc: f7eb ec56    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x14754
   18700: e695         	b	0x1842e <lgao_push_first_event+0x15e> @ imm = #-0x2d6
   18702: 68f3         	ldr	r3, [r6, #0xc]
   18704: 681b         	ldr	r3, [r3]
   18706: 07d9         	lsls	r1, r3, #0x1f
   18708: d417         	bmi	0x1873a <lgao_push_first_event+0x46a> @ imm = #0x2e
   1870a: 4628         	mov	r0, r5
   1870c: f04f 37ff    	mov.w	r7, #0xffffffff
   18710: f7eb ed44    	blx	0x419c <aud_object_put@plt> @ imm = #-0x14578
   18714: e68b         	b	0x1842e <lgao_push_first_event+0x15e> @ imm = #-0x2ea
   18716: 68f3         	ldr	r3, [r6, #0xc]
   18718: 681b         	ldr	r3, [r3]
   1871a: 07dc         	lsls	r4, r3, #0x1f
   1871c: d5f5         	bpl	0x1870a <lgao_push_first_event+0x43a> @ imm = #-0x16
   1871e: 4a3e         	ldr	r2, [pc, #0xf8]         @ 0x18818 <lgao_push_first_event+0x548>
   18720: 4601         	mov	r1, r0
   18722: 4b3e         	ldr	r3, [pc, #0xf8]         @ 0x1881c <lgao_push_first_event+0x54c>
   18724: 4630         	mov	r0, r6
   18726: 447a         	add	r2, pc
   18728: 447b         	add	r3, pc
   1872a: f202 5264    	addw	r2, r2, #0x564
   1872e: 9300         	str	r3, [sp]
   18730: f640 332d    	movw	r3, #0xb2d
   18734: f7eb ec3a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x1478c
   18738: e7e7         	b	0x1870a <lgao_push_first_event+0x43a> @ imm = #-0x32
   1873a: 4a39         	ldr	r2, [pc, #0xe4]         @ 0x18820 <lgao_push_first_event+0x550>
   1873c: 4601         	mov	r1, r0
   1873e: 4b39         	ldr	r3, [pc, #0xe4]         @ 0x18824 <lgao_push_first_event+0x554>
   18740: 4630         	mov	r0, r6
   18742: 447a         	add	r2, pc
   18744: 447b         	add	r3, pc
   18746: f202 5264    	addw	r2, r2, #0x564
   1874a: 9300         	str	r3, [sp]
   1874c: f640 334e    	movw	r3, #0xb4e
   18750: f7eb ec2c    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x147a8
   18754: e7d9         	b	0x1870a <lgao_push_first_event+0x43a> @ imm = #-0x4e
   18756: bf00         	nop
   18758: 00 00 7a 44  	.word	0x447a0000
   1875c: fa 98 01 00  	.word	0x000198fa
   18760: 00 04 00 00  	.word	0x00000400
   18764: 04 32 00 00  	.word	0x00003204
   18768: 46 38 00 00  	.word	0x00003846
   1876c: b2 89 00 00  	.word	0x000089b2
   18770: 58 36 00 00  	.word	0x00003658
   18774: 0e 34 00 00  	.word	0x0000340e
   18778: 28 35 00 00  	.word	0x00003528
   1877c: 60 89 00 00  	.word	0x00008960
   18780: fe 37 00 00  	.word	0x000037fe
   18784: 82 33 00 00  	.word	0x00003382
   18788: 02 37 00 00  	.word	0x00003702
   1878c: 56 88 00 00  	.word	0x00008856
   18790: 34 38 00 00  	.word	0x00003834
   18794: 94 38 00 00  	.word	0x00003894
   18798: 42 36 00 00  	.word	0x00003642
   1879c: 52 35 00 00  	.word	0x00003552
   187a0: b6 32 00 00  	.word	0x000032b6
   187a4: cc 38 00 00  	.word	0x000038cc
   187a8: 1e 39 00 00  	.word	0x0000391e
   187ac: c0 87 00 00  	.word	0x000087c0
   187b0: 36 37 00 00  	.word	0x00003736
   187b4: d8 89 00 00  	.word	0x000089d8
   187b8: 2a 87 00 00  	.word	0x0000872a
   187bc: 18 87 00 00  	.word	0x00008718
   187c0: e6 27 00 00  	.word	0x000027e6
   187c4: 7c 89 00 00  	.word	0x0000897c
   187c8: dc 86 00 00  	.word	0x000086dc
   187cc: 3a 33 00 00  	.word	0x0000333a
   187d0: 84 27 00 00  	.word	0x00002784
   187d4: 2e 89 00 00  	.word	0x0000892e
   187d8: d0 78 00 00  	.word	0x000078d0
   187dc: 10 89 00 00  	.word	0x00008910
   187e0: 52 87 00 00  	.word	0x00008752
   187e4: f4 88 00 00  	.word	0x000088f4
   187e8: 4a 87 00 00  	.word	0x0000874a
   187ec: d8 88 00 00  	.word	0x000088d8
   187f0: c6 86 00 00  	.word	0x000086c6
   187f4: ee 86 00 00  	.word	0x000086ee
   187f8: aa 86 00 00  	.word	0x000086aa
   187fc: 9c 88 00 00  	.word	0x0000889c
   18800: 6c 88 00 00  	.word	0x0000886c
   18804: a6 7b 00 00  	.word	0x00007ba6
   18808: 4e 88 00 00  	.word	0x0000884e
   1880c: 0c 7b 00 00  	.word	0x00007b0c
   18810: 26 88 00 00  	.word	0x00008826
   18814: fc 85 00 00  	.word	0x000085fc
   18818: f2 87 00 00  	.word	0x000087f2
   1881c: c8 85 00 00  	.word	0x000085c8
   18820: d6 87 00 00  	.word	0x000087d6
   18824: ac 85 00 00  	.word	0x000085ac
