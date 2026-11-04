
orig/usr/lib/gstreamer-1.0/libgstlgaudiosink.so:	file format elf32-littlearm

Disassembly of section .text:

00008240 <gst_lg_audio_render_event>:
    8240: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    8244: 4606         	mov	r6, r0
    8246: f8df 82e8    	ldr.w	r8, [pc, #0x2e8]        @ 0x8530 <gst_lg_audio_render_event+0x2f0>
    824a: b0c9         	sub	sp, #0x124
    824c: 460d         	mov	r5, r1
    824e: f7fb ee94    	blx	0x3f78 <gst_lg_audio_render_get_type@plt> @ imm = #-0x42d8
    8252: 44f8         	add	r8, pc
    8254: 4601         	mov	r1, r0
    8256: 4630         	mov	r0, r6
    8258: 4fb6         	ldr	r7, [pc, #0x2d8]        @ 0x8534 <gst_lg_audio_render_event+0x2f4>
    825a: f7fb ee96    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x42d4
    825e: f8d8 300c    	ldr.w	r3, [r8, #0xc]
    8262: 4604         	mov	r4, r0
    8264: 447f         	add	r7, pc
    8266: f8d5 9024    	ldr.w	r9, [r5, #0x24]
    826a: 681b         	ldr	r3, [r3]
    826c: 06d8         	lsls	r0, r3, #0x1b
    826e: f100 82d2    	bmi.w	0x8816 <gst_lg_audio_render_event+0x5d6> @ imm = #0x5a4
    8272: 4bb1         	ldr	r3, [pc, #0x2c4]        @ 0x8538 <gst_lg_audio_render_event+0x2f8>
    8274: f857 8003    	ldr.w	r8, [r7, r3]
    8278: f8d8 3000    	ldr.w	r3, [r8]
    827c: 2b04         	cmp	r3, #0x4
    827e: d918         	bls	0x82b2 <gst_lg_audio_render_event+0x72> @ imm = #0x30
    8280: 4bae         	ldr	r3, [pc, #0x2b8]        @ 0x853c <gst_lg_audio_render_event+0x2fc>
    8282: 6a68         	ldr	r0, [r5, #0x24]
    8284: 58fb         	ldr	r3, [r7, r3]
    8286: f8d3 a000    	ldr.w	r10, [r3]
    828a: f7fb ed92    	blx	0x3db0 <gst_event_type_get_name@plt> @ imm = #-0x44dc
    828e: 4aac         	ldr	r2, [pc, #0x2b0]        @ 0x8540 <gst_lg_audio_render_event+0x300>
    8290: 2105         	movs	r1, #0x5
    8292: 4bac         	ldr	r3, [pc, #0x2b0]        @ 0x8544 <gst_lg_audio_render_event+0x304>
    8294: 9003         	str	r0, [sp, #0xc]
    8296: 4650         	mov	r0, r10
    8298: 447a         	add	r2, pc
    829a: 9401         	str	r4, [sp, #0x4]
    829c: 9202         	str	r2, [sp, #0x8]
    829e: f640 02ad    	movw	r2, #0x8ad
    82a2: 9200         	str	r2, [sp]
    82a4: 447b         	add	r3, pc
    82a6: 4aa8         	ldr	r2, [pc, #0x2a0]        @ 0x8548 <gst_lg_audio_render_event+0x308>
    82a8: f503 73b8    	add.w	r3, r3, #0x170
    82ac: 447a         	add	r2, pc
    82ae: f7fb ecde    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4644
    82b2: f244 630e    	movw	r3, #0x460e
    82b6: 4599         	cmp	r9, r3
    82b8: d02c         	beq	0x8314 <gst_lg_audio_render_event+0xd4> @ imm = #0x58
    82ba: f240 817b    	bls.w	0x85b4 <gst_lg_audio_render_event+0x374> @ imm = #0x2f6
    82be: f641 0306    	movw	r3, #0x1806
    82c2: f2c0 0301    	movt	r3, #0x1
    82c6: 4599         	cmp	r9, r3
    82c8: f000 8450    	beq.w	0x8b6c <gst_lg_audio_render_event+0x92c> @ imm = #0x8a0
    82cc: f240 8104    	bls.w	0x84d8 <gst_lg_audio_render_event+0x298> @ imm = #0x208
    82d0: f242 2302    	movw	r3, #0x2202
    82d4: f2c0 0301    	movt	r3, #0x1
    82d8: 4599         	cmp	r9, r3
    82da: f040 80ec    	bne.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #0x1d8
    82de: 499b         	ldr	r1, [pc, #0x26c]        @ 0x854c <gst_lg_audio_render_event+0x30c>
    82e0: 4628         	mov	r0, r5
    82e2: 4479         	add	r1, pc
    82e4: f7fb ed52    	blx	0x3d8c <gst_event_has_name@plt> @ imm = #-0x455c
    82e8: 2800         	cmp	r0, #0x0
    82ea: f000 80e4    	beq.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #0x1c8
    82ee: 4898         	ldr	r0, [pc, #0x260]        @ 0x8550 <gst_lg_audio_render_event+0x310>
    82f0: 4478         	add	r0, pc
    82f2: 68c3         	ldr	r3, [r0, #0xc]
    82f4: 681b         	ldr	r3, [r3]
    82f6: 071e         	lsls	r6, r3, #0x1c
    82f8: f101 8635    	bmi.w	0x9f66 <gst_lg_audio_render_event+0x1d26> @ imm = #0x1c6a
    82fc: f8d8 3000    	ldr.w	r3, [r8]
    8300: 2b03         	cmp	r3, #0x3
    8302: f201 846e    	bhi.w	0x9be2 <gst_lg_audio_render_event+0x19a2> @ imm = #0x18dc
    8306: 4628         	mov	r0, r5
    8308: f7fb ecf8    	blx	0x3cfc <gst_mini_object_unref@plt> @ imm = #-0x4610
    830c: 2001         	movs	r0, #0x1
    830e: b049         	add	sp, #0x124
    8310: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    8314: 488f         	ldr	r0, [pc, #0x23c]        @ 0x8554 <gst_lg_audio_render_event+0x314>
    8316: 4478         	add	r0, pc
    8318: 68c3         	ldr	r3, [r0, #0xc]
    831a: 681b         	ldr	r3, [r3]
    831c: 071a         	lsls	r2, r3, #0x1c
    831e: f101 8656    	bmi.w	0x9fce <gst_lg_audio_render_event+0x1d8e> @ imm = #0x1cac
    8322: f8d8 3000    	ldr.w	r3, [r8]
    8326: 2b03         	cmp	r3, #0x3
    8328: d912         	bls	0x8350 <gst_lg_audio_render_event+0x110> @ imm = #0x24
    832a: 4b84         	ldr	r3, [pc, #0x210]        @ 0x853c <gst_lg_audio_render_event+0x2fc>
    832c: 2104         	movs	r1, #0x4
    832e: 4a8a         	ldr	r2, [pc, #0x228]        @ 0x8558 <gst_lg_audio_render_event+0x318>
    8330: 58fb         	ldr	r3, [r7, r3]
    8332: 447a         	add	r2, pc
    8334: 9401         	str	r4, [sp, #0x4]
    8336: 6818         	ldr	r0, [r3]
    8338: 4b88         	ldr	r3, [pc, #0x220]        @ 0x855c <gst_lg_audio_render_event+0x31c>
    833a: 9202         	str	r2, [sp, #0x8]
    833c: f640 02ef    	movw	r2, #0x8ef
    8340: 9200         	str	r2, [sp]
    8342: 4a87         	ldr	r2, [pc, #0x21c]        @ 0x8560 <gst_lg_audio_render_event+0x320>
    8344: 447b         	add	r3, pc
    8346: f503 73b8    	add.w	r3, r3, #0x170
    834a: 447a         	add	r2, pc
    834c: f7fb ec8e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x46e4
    8350: a930         	add	r1, sp, #0xc0
    8352: 4628         	mov	r0, r5
    8354: f7fb eb0a    	blx	0x396c <gst_event_parse_segment@plt> @ imm = #-0x49ec
    8358: 4882         	ldr	r0, [pc, #0x208]        @ 0x8564 <gst_lg_audio_render_event+0x324>
    835a: 9930         	ldr	r1, [sp, #0xc0]
    835c: 4478         	add	r0, pc
    835e: f7fb ea76    	blx	0x384c <gst_info_strdup_printf@plt> @ imm = #-0x4b14
    8362: 4681         	mov	r9, r0
    8364: 4880         	ldr	r0, [pc, #0x200]        @ 0x8568 <gst_lg_audio_render_event+0x328>
    8366: 4478         	add	r0, pc
    8368: 68c3         	ldr	r3, [r0, #0xc]
    836a: 681b         	ldr	r3, [r3]
    836c: 071b         	lsls	r3, r3, #0x1c
    836e: f101 861c    	bmi.w	0x9faa <gst_lg_audio_render_event+0x1d6a> @ imm = #0x1c38
    8372: f8d8 3000    	ldr.w	r3, [r8]
    8376: 2b03         	cmp	r3, #0x3
    8378: d914         	bls	0x83a4 <gst_lg_audio_render_event+0x164> @ imm = #0x28
    837a: 4b70         	ldr	r3, [pc, #0x1c0]        @ 0x853c <gst_lg_audio_render_event+0x2fc>
    837c: 2104         	movs	r1, #0x4
    837e: 4a7b         	ldr	r2, [pc, #0x1ec]        @ 0x856c <gst_lg_audio_render_event+0x32c>
    8380: 58fb         	ldr	r3, [r7, r3]
    8382: 447a         	add	r2, pc
    8384: f8cd 900c    	str.w	r9, [sp, #0xc]
    8388: 9401         	str	r4, [sp, #0x4]
    838a: 6818         	ldr	r0, [r3]
    838c: 4b78         	ldr	r3, [pc, #0x1e0]        @ 0x8570 <gst_lg_audio_render_event+0x330>
    838e: 9202         	str	r2, [sp, #0x8]
    8390: f640 02f3    	movw	r2, #0x8f3
    8394: 9200         	str	r2, [sp]
    8396: 4a77         	ldr	r2, [pc, #0x1dc]        @ 0x8574 <gst_lg_audio_render_event+0x334>
    8398: 447b         	add	r3, pc
    839a: f503 73b8    	add.w	r3, r3, #0x170
    839e: 447a         	add	r2, pc
    83a0: f7fb ec64    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4738
    83a4: 4648         	mov	r0, r9
    83a6: f7fb e9ec    	blx	0x3780 <g_free@plt>     @ imm = #-0x4c28
    83aa: 9b30         	ldr	r3, [sp, #0xc0]
    83ac: eddf 2b5e    	vldr	d18, [pc, #376]         @ 0x8528 <gst_lg_audio_render_event+0x2e8>
    83b0: eef6 1b00    	vmov.f64	d17, #5.000000e-01
    83b4: edd3 0b02    	vldr	d16, [r3, #8]
    83b8: e9d3 010c    	ldrd	r0, r1, [r3, #48]
    83bc: e9d3 2312    	ldrd	r2, r3, [r3, #72]
    83c0: edc4 0b94    	vstr	d16, [r4, #592]
    83c4: ee40 1ba2    	vmla.f64	d17, d16, d18
    83c8: e9c4 01b4    	strd	r0, r1, [r4, #720]
    83cc: e9c4 23b6    	strd	r2, r3, [r4, #728]
    83d0: ec51 0b31    	vmov	r0, r1, d17
    83d4: f7fb ea6a    	blx	0x38ac <floor@plt>      @ imm = #-0x4b2c
    83d8: ec41 0b30    	vmov	d16, r0, r1
    83dc: eefd 7be0    	vcvt.s32.f64	s15, d16
    83e0: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    83e4: ee17 1a90    	vmov	r1, s15
    83e8: ee17 9a90    	vmov	r9, s15
    83ec: f7fb ee34    	blx	0x4058 <lgao_set_playrate@plt> @ imm = #-0x4398
    83f0: b300         	cbz	r0, 0x8434 <gst_lg_audio_render_event+0x1f4> @ imm = #0x40
    83f2: 4861         	ldr	r0, [pc, #0x184]        @ 0x8578 <gst_lg_audio_render_event+0x338>
    83f4: 4478         	add	r0, pc
    83f6: 68c3         	ldr	r3, [r0, #0xc]
    83f8: 681b         	ldr	r3, [r3]
    83fa: 07d9         	lsls	r1, r3, #0x1f
    83fc: f101 85c3    	bmi.w	0x9f86 <gst_lg_audio_render_event+0x1d46> @ imm = #0x1b86
    8400: f8d8 3000    	ldr.w	r3, [r8]
    8404: 2b00         	cmp	r3, #0x0
    8406: f001 83cf    	beq.w	0x9ba8 <gst_lg_audio_render_event+0x1968> @ imm = #0x179e
    840a: 4b4c         	ldr	r3, [pc, #0x130]        @ 0x853c <gst_lg_audio_render_event+0x2fc>
    840c: 2101         	movs	r1, #0x1
    840e: 4a5b         	ldr	r2, [pc, #0x16c]        @ 0x857c <gst_lg_audio_render_event+0x33c>
    8410: 58fb         	ldr	r3, [r7, r3]
    8412: 447a         	add	r2, pc
    8414: f8cd 900c    	str.w	r9, [sp, #0xc]
    8418: 9401         	str	r4, [sp, #0x4]
    841a: 6818         	ldr	r0, [r3]
    841c: 4b58         	ldr	r3, [pc, #0x160]        @ 0x8580 <gst_lg_audio_render_event+0x340>
    841e: 9202         	str	r2, [sp, #0x8]
    8420: f640 02fb    	movw	r2, #0x8fb
    8424: 9200         	str	r2, [sp]
    8426: 4a57         	ldr	r2, [pc, #0x15c]        @ 0x8584 <gst_lg_audio_render_event+0x344>
    8428: 447b         	add	r3, pc
    842a: f503 73b8    	add.w	r3, r3, #0x170
    842e: 447a         	add	r2, pc
    8430: f7fb ec1c    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x47c8
    8434: 4b54         	ldr	r3, [pc, #0x150]        @ 0x8588 <gst_lg_audio_render_event+0x348>
    8436: 447b         	add	r3, pc
    8438: 68db         	ldr	r3, [r3, #0xc]
    843a: 681b         	ldr	r3, [r3]
    843c: 071a         	lsls	r2, r3, #0x1c
    843e: f101 83ba    	bmi.w	0x9bb6 <gst_lg_audio_render_event+0x1976> @ imm = #0x1774
    8442: f8d8 3000    	ldr.w	r3, [r8]
    8446: 2b03         	cmp	r3, #0x3
    8448: d916         	bls	0x8478 <gst_lg_audio_render_event+0x238> @ imm = #0x2c
    844a: 4b3c         	ldr	r3, [pc, #0xf0]         @ 0x853c <gst_lg_audio_render_event+0x2fc>
    844c: f640 0cff    	movw	r12, #0x8ff
    8450: e9d4 ab82    	ldrd	r10, r11, [r4, #520]
    8454: 2104         	movs	r1, #0x4
    8456: 4a4d         	ldr	r2, [pc, #0x134]        @ 0x858c <gst_lg_audio_render_event+0x34c>
    8458: 58fb         	ldr	r3, [r7, r3]
    845a: 4f4d         	ldr	r7, [pc, #0x134]        @ 0x8590 <gst_lg_audio_render_event+0x350>
    845c: 447a         	add	r2, pc
    845e: 6818         	ldr	r0, [r3]
    8460: 4b4c         	ldr	r3, [pc, #0x130]        @ 0x8594 <gst_lg_audio_render_event+0x354>
    8462: 447f         	add	r7, pc
    8464: e9cd ab04    	strd	r10, r11, [sp, #16]
    8468: 9702         	str	r7, [sp, #0x8]
    846a: 447b         	add	r3, pc
    846c: e9cd c400    	strd	r12, r4, [sp]
    8470: f503 73b8    	add.w	r3, r3, #0x170
    8474: f7fb ebfa    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x480c
    8478: f8d4 3218    	ldr.w	r3, [r4, #0x218]
    847c: efc0 0010    	vmov.i32	d16, #0x0
    8480: 2b04         	cmp	r3, #0x4
    8482: edc4 0b82    	vstr	d16, [r4, #520]
    8486: f001 8694    	beq.w	0xa1b2 <gst_lg_audio_render_event+0x1f72> @ imm = #0x1d28
    848a: f8d4 7224    	ldr.w	r7, [r4, #0x224]
    848e: b17f         	cbz	r7, 0x84b0 <gst_lg_audio_render_event+0x270> @ imm = #0x1e
    8490: 4941         	ldr	r1, [pc, #0x104]        @ 0x8598 <gst_lg_audio_render_event+0x358>
    8492: 4638         	mov	r0, r7
    8494: 4479         	add	r1, pc
    8496: f7fb ef00    	blx	0x4298 <strcmp@plt>     @ imm = #-0x4200
    849a: 2800         	cmp	r0, #0x0
    849c: f041 84f2    	bne.w	0x9e84 <gst_lg_audio_render_event+0x1c44> @ imm = #0x19e4
    84a0: 493e         	ldr	r1, [pc, #0xf8]         @ 0x859c <gst_lg_audio_render_event+0x35c>
    84a2: 4638         	mov	r0, r7
    84a4: 4479         	add	r1, pc
    84a6: f7fb eef8    	blx	0x4298 <strcmp@plt>     @ imm = #-0x4210
    84aa: 2800         	cmp	r0, #0x0
    84ac: f001 84de    	beq.w	0x9e6c <gst_lg_audio_render_event+0x1c2c> @ imm = #0x19bc
    84b0: 2300         	movs	r3, #0x0
    84b2: f8c4 32f8    	str.w	r3, [r4, #0x2f8]
    84b6: 4b3a         	ldr	r3, [pc, #0xe8]         @ 0x85a0 <gst_lg_audio_render_event+0x360>
    84b8: 447b         	add	r3, pc
    84ba: 681c         	ldr	r4, [r3]
    84bc: f7fb edf0    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0x4420
    84c0: 4601         	mov	r1, r0
    84c2: 4620         	mov	r0, r4
    84c4: f7fb eb94    	blx	0x3bf0 <g_type_check_class_cast@plt> @ imm = #-0x48d8
    84c8: 4629         	mov	r1, r5
    84ca: f8d0 3124    	ldr.w	r3, [r0, #0x124]
    84ce: 4630         	mov	r0, r6
    84d0: b049         	add	sp, #0x124
    84d2: e8bd 4ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    84d6: 4718         	bx	r3
    84d8: f646 630e    	movw	r3, #0x6e0e
    84dc: 4599         	cmp	r9, r3
    84de: f000 8417    	beq.w	0x8d10 <gst_lg_audio_render_event+0xad0> @ imm = #0x82e
    84e2: f24a 0306    	movw	r3, #0xa006
    84e6: 4599         	cmp	r9, r3
    84e8: d1e5         	bne	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x36
    84ea: 482e         	ldr	r0, [pc, #0xb8]         @ 0x85a4 <gst_lg_audio_render_event+0x364>
    84ec: 4478         	add	r0, pc
    84ee: 68c3         	ldr	r3, [r0, #0xc]
    84f0: 681b         	ldr	r3, [r3]
    84f2: 0719         	lsls	r1, r3, #0x1c
    84f4: f100 84c3    	bmi.w	0x8e7e <gst_lg_audio_render_event+0xc3e> @ imm = #0x986
    84f8: f8d8 3000    	ldr.w	r3, [r8]
    84fc: 2b03         	cmp	r3, #0x3
    84fe: d9da         	bls	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x4c
    8500: 4a0e         	ldr	r2, [pc, #0x38]         @ 0x853c <gst_lg_audio_render_event+0x2fc>
    8502: 4b29         	ldr	r3, [pc, #0xa4]         @ 0x85a8 <gst_lg_audio_render_event+0x368>
    8504: 58b9         	ldr	r1, [r7, r2]
    8506: f640 02b2    	movw	r2, #0x8b2
    850a: 9200         	str	r2, [sp]
    850c: 447b         	add	r3, pc
    850e: 9401         	str	r4, [sp, #0x4]
    8510: f503 73b8    	add.w	r3, r3, #0x170
    8514: 6808         	ldr	r0, [r1]
    8516: 4925         	ldr	r1, [pc, #0x94]         @ 0x85ac <gst_lg_audio_render_event+0x36c>
    8518: 4a25         	ldr	r2, [pc, #0x94]         @ 0x85b0 <gst_lg_audio_render_event+0x370>
    851a: 4479         	add	r1, pc
    851c: 9102         	str	r1, [sp, #0x8]
    851e: 447a         	add	r2, pc
    8520: 2104         	movs	r1, #0x4
    8522: f7fb eba4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x48b8
    8526: e7c6         	b	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x74
    8528: 00 00 00 00  	.word	0x00000000
    852c: 00 40 8f 40  	.word	0x408f4000
    8530: ae 9d 02 00  	.word	0x00029dae
    8534: 74 99 02 00  	.word	0x00029974
    8538: 1c 04 00 00  	.word	0x0000041c
    853c: 08 04 00 00  	.word	0x00000408
    8540: ec 2a 01 00  	.word	0x00012aec
    8544: f0 78 01 00  	.word	0x000178f0
    8548: 8c 16 01 00  	.word	0x0001168c
    854c: ca 43 01 00  	.word	0x000143ca
    8550: 10 9d 02 00  	.word	0x00029d10
    8554: ea 9c 02 00  	.word	0x00029cea
    8558: 7a 2d 01 00  	.word	0x00012d7a
    855c: 50 78 01 00  	.word	0x00017850
    8560: ee 15 01 00  	.word	0x000115ee
    8564: 74 2d 01 00  	.word	0x00012d74
    8568: 9a 9c 02 00  	.word	0x00029c9a
    856c: ea 89 01 00  	.word	0x000189ea
    8570: fc 77 01 00  	.word	0x000177fc
    8574: 9a 15 01 00  	.word	0x0001159a
    8578: 0c 9c 02 00  	.word	0x00029c0c
    857c: 0a 2d 01 00  	.word	0x00012d0a
    8580: 6c 77 01 00  	.word	0x0001776c
    8584: 0a 15 01 00  	.word	0x0001150a
    8588: ca 9b 02 00  	.word	0x00029bca
    858c: dc 14 01 00  	.word	0x000114dc
    8590: 1e 2d 01 00  	.word	0x00012d1e
    8594: 2a 77 01 00  	.word	0x0001772a
    8598: 1c 2d 01 00  	.word	0x00012d1c
    859c: 54 1e 01 00  	.word	0x00011e54
    85a0: 00 9c 02 00  	.word	0x00029c00
    85a4: 14 9b 02 00  	.word	0x00029b14
    85a8: 88 76 01 00  	.word	0x00017688
    85ac: a6 28 01 00  	.word	0x000128a6
    85b0: 1a 14 01 00  	.word	0x0001141a
    85b4: f642 030e    	movw	r3, #0x280e
    85b8: 4599         	cmp	r9, r3
    85ba: f000 8221    	beq.w	0x8a00 <gst_lg_audio_render_event+0x7c0> @ imm = #0x442
    85be: f240 80ed    	bls.w	0x879c <gst_lg_audio_render_event+0x55c> @ imm = #0x1da
    85c2: f243 230e    	movw	r3, #0x320e
    85c6: 4599         	cmp	r9, r3
    85c8: f47f af75    	bne.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x116
    85cc: f8df 08cc    	ldr.w	r0, [pc, #0x8cc]        @ 0x8e9c <gst_lg_audio_render_event+0xc5c>
    85d0: 4478         	add	r0, pc
    85d2: 68c3         	ldr	r3, [r0, #0xc]
    85d4: 681b         	ldr	r3, [r3]
    85d6: 071b         	lsls	r3, r3, #0x1c
    85d8: f100 8436    	bmi.w	0x8e48 <gst_lg_audio_render_event+0xc08> @ imm = #0x86c
    85dc: f8d8 3000    	ldr.w	r3, [r8]
    85e0: 2b03         	cmp	r3, #0x3
    85e2: d916         	bls	0x8612 <gst_lg_audio_render_event+0x3d2> @ imm = #0x2c
    85e4: f8df 38b8    	ldr.w	r3, [pc, #0x8b8]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    85e8: 2104         	movs	r1, #0x4
    85ea: f8df 28b8    	ldr.w	r2, [pc, #0x8b8]        @ 0x8ea4 <gst_lg_audio_render_event+0xc64>
    85ee: 58fb         	ldr	r3, [r7, r3]
    85f0: 447a         	add	r2, pc
    85f2: 9401         	str	r4, [sp, #0x4]
    85f4: 6818         	ldr	r0, [r3]
    85f6: f8df 38b0    	ldr.w	r3, [pc, #0x8b0]        @ 0x8ea8 <gst_lg_audio_render_event+0xc68>
    85fa: 9202         	str	r2, [sp, #0x8]
    85fc: f640 121c    	movw	r2, #0x91c
    8600: 9200         	str	r2, [sp]
    8602: f8df 28a8    	ldr.w	r2, [pc, #0x8a8]        @ 0x8eac <gst_lg_audio_render_event+0xc6c>
    8606: 447b         	add	r3, pc
    8608: f503 73b8    	add.w	r3, r3, #0x170
    860c: 447a         	add	r2, pc
    860e: f7fb eb2e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x49a4
    8612: 4628         	mov	r0, r5
    8614: a919         	add	r1, sp, #0x64
    8616: f7fb ec80    	blx	0x3f18 <gst_event_parse_caps@plt> @ imm = #-0x4700
    861a: f8d4 022c    	ldr.w	r0, [r4, #0x22c]
    861e: b120         	cbz	r0, 0x862a <gst_lg_audio_render_event+0x3ea> @ imm = #0x8
    8620: f7fb eb6c    	blx	0x3cfc <gst_mini_object_unref@plt> @ imm = #-0x4928
    8624: 2300         	movs	r3, #0x0
    8626: f8c4 322c    	str.w	r3, [r4, #0x22c]
    862a: 9819         	ldr	r0, [sp, #0x64]
    862c: f7fb eb12    	blx	0x3c54 <gst_mini_object_copy@plt> @ imm = #-0x49dc
    8630: ab30         	add	r3, sp, #0xc0
    8632: 2100         	movs	r1, #0x0
    8634: f8c4 022c    	str.w	r0, [r4, #0x22c]
    8638: 225c         	movs	r2, #0x5c
    863a: 4618         	mov	r0, r3
    863c: 4689         	mov	r9, r1
    863e: 9314         	str	r3, [sp, #0x50]
    8640: f8dd a064    	ldr.w	r10, [sp, #0x64]
    8644: f7fb e9f2    	blx	0x3a2c <memset@plt>     @ imm = #-0x4c1c
    8648: f04f 33ff    	mov.w	r3, #0xffffffff
    864c: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8650: a929         	add	r1, sp, #0xa4
    8652: e9cd 9925    	strd	r9, r9, [sp, #148]
    8656: e9cd 9927    	strd	r9, r9, [sp, #156]
    865a: f8cd 90a4    	str.w	r9, [sp, #0xa4]
    865e: e9cd 992b    	strd	r9, r9, [sp, #172]
    8662: 932a         	str	r3, [sp, #0xa8]
    8664: f7fb ec76    	blx	0x3f54 <lgao_get_mode_mixer@plt> @ imm = #-0x4714
    8668: b1d8         	cbz	r0, 0x86a2 <gst_lg_audio_render_event+0x462> @ imm = #0x36
    866a: f8d8 3000    	ldr.w	r3, [r8]
    866e: 2b01         	cmp	r3, #0x1
    8670: d917         	bls	0x86a2 <gst_lg_audio_render_event+0x462> @ imm = #0x2e
    8672: f8df 382c    	ldr.w	r3, [pc, #0x82c]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8676: 2102         	movs	r1, #0x2
    8678: f8df 2834    	ldr.w	r2, [pc, #0x834]        @ 0x8eb0 <gst_lg_audio_render_event+0xc70>
    867c: 58fb         	ldr	r3, [r7, r3]
    867e: 447a         	add	r2, pc
    8680: f8cd 9004    	str.w	r9, [sp, #0x4]
    8684: 6818         	ldr	r0, [r3]
    8686: f8df 382c    	ldr.w	r3, [pc, #0x82c]        @ 0x8eb4 <gst_lg_audio_render_event+0xc74>
    868a: 9202         	str	r2, [sp, #0x8]
    868c: f240 426a    	movw	r2, #0x46a
    8690: 9200         	str	r2, [sp]
    8692: f8df 2824    	ldr.w	r2, [pc, #0x824]        @ 0x8eb8 <gst_lg_audio_render_event+0xc78>
    8696: 447b         	add	r3, pc
    8698: f503 73c6    	add.w	r3, r3, #0x18c
    869c: 447a         	add	r2, pc
    869e: f7fb eae6    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4a34
    86a2: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    86a6: a92a         	add	r1, sp, #0xa8
    86a8: f7fb ee1c    	blx	0x42e4 <lgao_get_port@plt> @ imm = #-0x43c8
    86ac: b1e0         	cbz	r0, 0x86e8 <gst_lg_audio_render_event+0x4a8> @ imm = #0x38
    86ae: f8d8 3000    	ldr.w	r3, [r8]
    86b2: 2b01         	cmp	r3, #0x1
    86b4: d918         	bls	0x86e8 <gst_lg_audio_render_event+0x4a8> @ imm = #0x30
    86b6: f8df 37e8    	ldr.w	r3, [pc, #0x7e8]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    86ba: f04f 0c00    	mov.w	r12, #0x0
    86be: f8df 27fc    	ldr.w	r2, [pc, #0x7fc]        @ 0x8ebc <gst_lg_audio_render_event+0xc7c>
    86c2: 2102         	movs	r1, #0x2
    86c4: 58fb         	ldr	r3, [r7, r3]
    86c6: 447a         	add	r2, pc
    86c8: 6818         	ldr	r0, [r3]
    86ca: f8df 37f4    	ldr.w	r3, [pc, #0x7f4]        @ 0x8ec0 <gst_lg_audio_render_event+0xc80>
    86ce: 9202         	str	r2, [sp, #0x8]
    86d0: f240 426b    	movw	r2, #0x46b
    86d4: e9cd 2c00    	strd	r2, r12, [sp]
    86d8: f8df 27e8    	ldr.w	r2, [pc, #0x7e8]        @ 0x8ec4 <gst_lg_audio_render_event+0xc84>
    86dc: 447b         	add	r3, pc
    86de: f503 73c6    	add.w	r3, r3, #0x18c
    86e2: 447a         	add	r2, pc
    86e4: f7fb eac2    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4a7c
    86e8: 4650         	mov	r0, r10
    86ea: f7fb ea76    	blx	0x3bd8 <gst_caps_to_string@plt> @ imm = #-0x4b14
    86ee: 4681         	mov	r9, r0
    86f0: 2800         	cmp	r0, #0x0
    86f2: f000 80f2    	beq.w	0x88da <gst_lg_audio_render_event+0x69a> @ imm = #0x1e4
    86f6: f7fb eaf0    	blx	0x3cd8 <strlen@plt>     @ imm = #-0x4a20
    86fa: f8df 37cc    	ldr.w	r3, [pc, #0x7cc]        @ 0x8ec8 <gst_lg_audio_render_event+0xc88>
    86fe: f5b0 6f80    	cmp.w	r0, #0x400
    8702: 447b         	add	r3, pc
    8704: 68da         	ldr	r2, [r3, #0xc]
    8706: f080 80c2    	bhs.w	0x888e <gst_lg_audio_render_event+0x64e> @ imm = #0x184
    870a: 6813         	ldr	r3, [r2]
    870c: 0719         	lsls	r1, r3, #0x1c
    870e: d51c         	bpl	0x874a <gst_lg_audio_render_event+0x50a> @ imm = #0x38
    8710: 9b29         	ldr	r3, [sp, #0xa4]
    8712: 2b01         	cmp	r3, #0x1
    8714: f001 860b    	beq.w	0xa32e <gst_lg_audio_render_event+0x20ee> @ imm = #0x1c16
    8718: f8df 37b0    	ldr.w	r3, [pc, #0x7b0]        @ 0x8ecc <gst_lg_audio_render_event+0xc8c>
    871c: 447b         	add	r3, pc
    871e: 9301         	str	r3, [sp, #0x4]
    8720: 2103         	movs	r1, #0x3
    8722: 9b2a         	ldr	r3, [sp, #0xa8]
    8724: f8df 27a8    	ldr.w	r2, [pc, #0x7a8]        @ 0x8ed0 <gst_lg_audio_render_event+0xc90>
    8728: f8df 07a8    	ldr.w	r0, [pc, #0x7a8]        @ 0x8ed4 <gst_lg_audio_render_event+0xc94>
    872c: 9302         	str	r3, [sp, #0x8]
    872e: f8df 37a8    	ldr.w	r3, [pc, #0x7a8]        @ 0x8ed8 <gst_lg_audio_render_event+0xc98>
    8732: 447a         	add	r2, pc
    8734: f502 72c6    	add.w	r2, r2, #0x18c
    8738: 4478         	add	r0, pc
    873a: f8cd 900c    	str.w	r9, [sp, #0xc]
    873e: 447b         	add	r3, pc
    8740: 9300         	str	r3, [sp]
    8742: f240 436f    	movw	r3, #0x46f
    8746: f7fb ec32    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x479c
    874a: f8d8 3000    	ldr.w	r3, [r8]
    874e: 2b03         	cmp	r3, #0x3
    8750: f240 80c0    	bls.w	0x88d4 <gst_lg_audio_render_event+0x694> @ imm = #0x180
    8754: 9b29         	ldr	r3, [sp, #0xa4]
    8756: 2b01         	cmp	r3, #0x1
    8758: f001 85e4    	beq.w	0xa324 <gst_lg_audio_render_event+0x20e4> @ imm = #0x1bc8
    875c: f8df 377c    	ldr.w	r3, [pc, #0x77c]        @ 0x8edc <gst_lg_audio_render_event+0xc9c>
    8760: 447b         	add	r3, pc
    8762: f8df 273c    	ldr.w	r2, [pc, #0x73c]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8766: 2104         	movs	r1, #0x4
    8768: 58ba         	ldr	r2, [r7, r2]
    876a: 9303         	str	r3, [sp, #0xc]
    876c: f8cd 9014    	str.w	r9, [sp, #0x14]
    8770: 9401         	str	r4, [sp, #0x4]
    8772: 6810         	ldr	r0, [r2]
    8774: 9a2a         	ldr	r2, [sp, #0xa8]
    8776: f8df 3768    	ldr.w	r3, [pc, #0x768]        @ 0x8ee0 <gst_lg_audio_render_event+0xca0>
    877a: 9204         	str	r2, [sp, #0x10]
    877c: f8df 2764    	ldr.w	r2, [pc, #0x764]        @ 0x8ee4 <gst_lg_audio_render_event+0xca4>
    8780: 447b         	add	r3, pc
    8782: f503 73c6    	add.w	r3, r3, #0x18c
    8786: 447a         	add	r2, pc
    8788: 9202         	str	r2, [sp, #0x8]
    878a: f240 426f    	movw	r2, #0x46f
    878e: 9200         	str	r2, [sp]
    8790: f8df 2754    	ldr.w	r2, [pc, #0x754]        @ 0x8ee8 <gst_lg_audio_render_event+0xca8>
    8794: 447a         	add	r2, pc
    8796: f7fb ea6a    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4b2c
    879a: e09b         	b	0x88d4 <gst_lg_audio_render_event+0x694> @ imm = #0x136
    879c: f640 2303    	movw	r3, #0xa03
    87a0: 4599         	cmp	r9, r3
    87a2: d04c         	beq	0x883e <gst_lg_audio_render_event+0x5fe> @ imm = #0x98
    87a4: f241 4307    	movw	r3, #0x1407
    87a8: 4599         	cmp	r9, r3
    87aa: f47f ae84    	bne.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x2f8
    87ae: f8df 073c    	ldr.w	r0, [pc, #0x73c]        @ 0x8eec <gst_lg_audio_render_event+0xcac>
    87b2: 4478         	add	r0, pc
    87b4: 68c3         	ldr	r3, [r0, #0xc]
    87b6: 681b         	ldr	r3, [r3]
    87b8: 071b         	lsls	r3, r3, #0x1c
    87ba: f100 8353    	bmi.w	0x8e64 <gst_lg_audio_render_event+0xc24> @ imm = #0x6a6
    87be: f8d8 3000    	ldr.w	r3, [r8]
    87c2: 2b03         	cmp	r3, #0x3
    87c4: d916         	bls	0x87f4 <gst_lg_audio_render_event+0x5b4> @ imm = #0x2c
    87c6: f8df 26d8    	ldr.w	r2, [pc, #0x6d8]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    87ca: 2104         	movs	r1, #0x4
    87cc: f8df 3720    	ldr.w	r3, [pc, #0x720]        @ 0x8ef0 <gst_lg_audio_render_event+0xcb0>
    87d0: 58ba         	ldr	r2, [r7, r2]
    87d2: 9401         	str	r4, [sp, #0x4]
    87d4: 447b         	add	r3, pc
    87d6: f503 73b8    	add.w	r3, r3, #0x170
    87da: 6810         	ldr	r0, [r2]
    87dc: f8df 2714    	ldr.w	r2, [pc, #0x714]        @ 0x8ef4 <gst_lg_audio_render_event+0xcb4>
    87e0: 447a         	add	r2, pc
    87e2: 9202         	str	r2, [sp, #0x8]
    87e4: f640 02be    	movw	r2, #0x8be
    87e8: 9200         	str	r2, [sp]
    87ea: f8df 270c    	ldr.w	r2, [pc, #0x70c]        @ 0x8ef8 <gst_lg_audio_render_event+0xcb8>
    87ee: 447a         	add	r2, pc
    87f0: f7fb ea3c    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4b88
    87f4: 2100         	movs	r1, #0x0
    87f6: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    87fa: f7fb ec2e    	blx	0x4058 <lgao_set_playrate@plt> @ imm = #-0x47a4
    87fe: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8802: f7fb e938    	blx	0x3a74 <lgao_flush@plt> @ imm = #-0x4d90
    8806: efc0 0010    	vmov.i32	d16, #0x0
    880a: 2300         	movs	r3, #0x0
    880c: f8c4 3244    	str.w	r3, [r4, #0x244]
    8810: edc4 0b82    	vstr	d16, [r4, #520]
    8814: e64f         	b	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x362
    8816: 4648         	mov	r0, r9
    8818: f7fb eaca    	blx	0x3db0 <gst_event_type_get_name@plt> @ imm = #-0x4a6c
    881c: f8df 26dc    	ldr.w	r2, [pc, #0x6dc]        @ 0x8efc <gst_lg_audio_render_event+0xcbc>
    8820: 2104         	movs	r1, #0x4
    8822: f8df 36dc    	ldr.w	r3, [pc, #0x6dc]        @ 0x8f00 <gst_lg_audio_render_event+0xcc0>
    8826: 9001         	str	r0, [sp, #0x4]
    8828: 4640         	mov	r0, r8
    882a: 447a         	add	r2, pc
    882c: 447b         	add	r3, pc
    882e: f502 72b8    	add.w	r2, r2, #0x170
    8832: 9300         	str	r3, [sp]
    8834: f640 03ad    	movw	r3, #0x8ad
    8838: f7fb ebb8    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x4890
    883c: e519         	b	0x8272 <gst_lg_audio_render_event+0x32> @ imm = #-0x5ce
    883e: f8df 06c4    	ldr.w	r0, [pc, #0x6c4]        @ 0x8f04 <gst_lg_audio_render_event+0xcc4>
    8842: 4478         	add	r0, pc
    8844: 68c3         	ldr	r3, [r0, #0xc]
    8846: 681b         	ldr	r3, [r3]
    8848: 071a         	lsls	r2, r3, #0x1c
    884a: f101 8404    	bmi.w	0xa056 <gst_lg_audio_render_event+0x1e16> @ imm = #0x1808
    884e: f8d8 3000    	ldr.w	r3, [r8]
    8852: 2b03         	cmp	r3, #0x3
    8854: d916         	bls	0x8884 <gst_lg_audio_render_event+0x644> @ imm = #0x2c
    8856: f8df 2648    	ldr.w	r2, [pc, #0x648]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    885a: 2104         	movs	r1, #0x4
    885c: f8df 36a8    	ldr.w	r3, [pc, #0x6a8]        @ 0x8f08 <gst_lg_audio_render_event+0xcc8>
    8860: 58ba         	ldr	r2, [r7, r2]
    8862: 9401         	str	r4, [sp, #0x4]
    8864: 447b         	add	r3, pc
    8866: f503 73b8    	add.w	r3, r3, #0x170
    886a: 6810         	ldr	r0, [r2]
    886c: f8df 269c    	ldr.w	r2, [pc, #0x69c]        @ 0x8f0c <gst_lg_audio_render_event+0xccc>
    8870: 447a         	add	r2, pc
    8872: 9202         	str	r2, [sp, #0x8]
    8874: f640 02b7    	movw	r2, #0x8b7
    8878: 9200         	str	r2, [sp]
    887a: f8df 2694    	ldr.w	r2, [pc, #0x694]        @ 0x8f10 <gst_lg_audio_render_event+0xcd0>
    887e: 447a         	add	r2, pc
    8880: f7fb e9f4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4c18
    8884: 2201         	movs	r2, #0x1
    8886: 2300         	movs	r3, #0x0
    8888: e9c4 2391    	strd	r2, r3, [r4, #580]
    888c: e613         	b	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x3da
    888e: 6812         	ldr	r2, [r2]
    8890: 0712         	lsls	r2, r2, #0x1c
    8892: f101 84f4    	bmi.w	0xa27e <gst_lg_audio_render_event+0x203e> @ imm = #0x19e8
    8896: f8d8 3000    	ldr.w	r3, [r8]
    889a: 2b03         	cmp	r3, #0x3
    889c: d91a         	bls	0x88d4 <gst_lg_audio_render_event+0x694> @ imm = #0x34
    889e: 4648         	mov	r0, r9
    88a0: f7fb ea1a    	blx	0x3cd8 <strlen@plt>     @ imm = #-0x4bcc
    88a4: f8df 35f8    	ldr.w	r3, [pc, #0x5f8]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    88a8: 2104         	movs	r1, #0x4
    88aa: f8df 2668    	ldr.w	r2, [pc, #0x668]        @ 0x8f14 <gst_lg_audio_render_event+0xcd4>
    88ae: 58fb         	ldr	r3, [r7, r3]
    88b0: 447a         	add	r2, pc
    88b2: 9003         	str	r0, [sp, #0xc]
    88b4: 9401         	str	r4, [sp, #0x4]
    88b6: 6818         	ldr	r0, [r3]
    88b8: f8df 365c    	ldr.w	r3, [pc, #0x65c]        @ 0x8f18 <gst_lg_audio_render_event+0xcd8>
    88bc: 9202         	str	r2, [sp, #0x8]
    88be: f240 4272    	movw	r2, #0x472
    88c2: 9200         	str	r2, [sp]
    88c4: f8df 2654    	ldr.w	r2, [pc, #0x654]        @ 0x8f1c <gst_lg_audio_render_event+0xcdc>
    88c8: 447b         	add	r3, pc
    88ca: f503 73c6    	add.w	r3, r3, #0x18c
    88ce: 447a         	add	r2, pc
    88d0: f7fb e9cc    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4c68
    88d4: 4648         	mov	r0, r9
    88d6: f7fa ef54    	blx	0x3780 <g_free@plt>     @ imm = #-0x5158
    88da: 4650         	mov	r0, r10
    88dc: 2100         	movs	r1, #0x0
    88de: f7fb e8be    	blx	0x3a5c <gst_caps_get_structure@plt> @ imm = #-0x4e84
    88e2: 4681         	mov	r9, r0
    88e4: 2800         	cmp	r0, #0x0
    88e6: f001 83db    	beq.w	0xa0a0 <gst_lg_audio_render_event+0x1e60> @ imm = #0x17b6
    88ea: f8df 1634    	ldr.w	r1, [pc, #0x634]        @ 0x8f20 <gst_lg_audio_render_event+0xce0>
    88ee: f10d 0a68    	add.w	r10, sp, #0x68
    88f2: 4652         	mov	r2, r10
    88f4: 4479         	add	r1, pc
    88f6: f7fb e888    	blx	0x3a08 <gst_structure_get_int@plt> @ imm = #-0x4ef0
    88fa: 2800         	cmp	r0, #0x0
    88fc: d050         	beq	0x89a0 <gst_lg_audio_render_event+0x760> @ imm = #0xa0
    88fe: f8df 0624    	ldr.w	r0, [pc, #0x624]        @ 0x8f24 <gst_lg_audio_render_event+0xce4>
    8902: 4478         	add	r0, pc
    8904: 68c3         	ldr	r3, [r0, #0xc]
    8906: 681b         	ldr	r3, [r3]
    8908: 071a         	lsls	r2, r3, #0x1c
    890a: f101 84ca    	bmi.w	0xa2a2 <gst_lg_audio_render_event+0x2062> @ imm = #0x1994
    890e: f8d8 3000    	ldr.w	r3, [r8]
    8912: 2b03         	cmp	r3, #0x3
    8914: d918         	bls	0x8948 <gst_lg_audio_render_event+0x708> @ imm = #0x30
    8916: f8df 3588    	ldr.w	r3, [pc, #0x588]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    891a: 2104         	movs	r1, #0x4
    891c: 9a1a         	ldr	r2, [sp, #0x68]
    891e: 58fb         	ldr	r3, [r7, r3]
    8920: 9401         	str	r4, [sp, #0x4]
    8922: 6818         	ldr	r0, [r3]
    8924: 9203         	str	r2, [sp, #0xc]
    8926: f8df 2600    	ldr.w	r2, [pc, #0x600]        @ 0x8f28 <gst_lg_audio_render_event+0xce8>
    892a: f8df 3600    	ldr.w	r3, [pc, #0x600]        @ 0x8f2c <gst_lg_audio_render_event+0xcec>
    892e: 447a         	add	r2, pc
    8930: 9202         	str	r2, [sp, #0x8]
    8932: f240 427e    	movw	r2, #0x47e
    8936: 9200         	str	r2, [sp]
    8938: 447b         	add	r3, pc
    893a: f8df 25f4    	ldr.w	r2, [pc, #0x5f4]        @ 0x8f30 <gst_lg_audio_render_event+0xcf0>
    893e: f503 73c6    	add.w	r3, r3, #0x18c
    8942: 447a         	add	r2, pc
    8944: f7fb e992    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4cdc
    8948: 9b1a         	ldr	r3, [sp, #0x68]
    894a: b34b         	cbz	r3, 0x89a0 <gst_lg_audio_render_event+0x760> @ imm = #0x52
    894c: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8950: 2101         	movs	r1, #0x1
    8952: f7fb ea8e    	blx	0x3e70 <lgao_set_drop_renderer@plt> @ imm = #-0x4ae4
    8956: f8df 05dc    	ldr.w	r0, [pc, #0x5dc]        @ 0x8f34 <gst_lg_audio_render_event+0xcf4>
    895a: 4478         	add	r0, pc
    895c: 68c3         	ldr	r3, [r0, #0xc]
    895e: 681b         	ldr	r3, [r3]
    8960: 071b         	lsls	r3, r3, #0x1c
    8962: f102 8328    	bmi.w	0xafb6 <gst_lg_audio_render_event+0x2d76> @ imm = #0x2650
    8966: f8d8 3000    	ldr.w	r3, [r8]
    896a: 2b03         	cmp	r3, #0x3
    896c: f67f ada3    	bls.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x4ba
    8970: f8df 252c    	ldr.w	r2, [pc, #0x52c]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8974: f8df 35c0    	ldr.w	r3, [pc, #0x5c0]        @ 0x8f38 <gst_lg_audio_render_event+0xcf8>
    8978: 58b9         	ldr	r1, [r7, r2]
    897a: f240 4281    	movw	r2, #0x481
    897e: 9200         	str	r2, [sp]
    8980: 447b         	add	r3, pc
    8982: 9401         	str	r4, [sp, #0x4]
    8984: f503 73c6    	add.w	r3, r3, #0x18c
    8988: 6808         	ldr	r0, [r1]
    898a: f8df 15b0    	ldr.w	r1, [pc, #0x5b0]        @ 0x8f3c <gst_lg_audio_render_event+0xcfc>
    898e: f8df 25b0    	ldr.w	r2, [pc, #0x5b0]        @ 0x8f40 <gst_lg_audio_render_event+0xd00>
    8992: 4479         	add	r1, pc
    8994: 9102         	str	r1, [sp, #0x8]
    8996: 447a         	add	r2, pc
    8998: 2104         	movs	r1, #0x4
    899a: f7fb e968    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4d30
    899e: e58a         	b	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x4ec
    89a0: f8df 15a0    	ldr.w	r1, [pc, #0x5a0]        @ 0x8f44 <gst_lg_audio_render_event+0xd04>
    89a4: 4652         	mov	r2, r10
    89a6: 4648         	mov	r0, r9
    89a8: 4479         	add	r1, pc
    89aa: f7fb e82e    	blx	0x3a08 <gst_structure_get_int@plt> @ imm = #-0x4fa4
    89ae: 9015         	str	r0, [sp, #0x54]
    89b0: 2800         	cmp	r0, #0x0
    89b2: f000 833b    	beq.w	0x902c <gst_lg_audio_render_event+0xdec> @ imm = #0x676
    89b6: f8df 0590    	ldr.w	r0, [pc, #0x590]        @ 0x8f48 <gst_lg_audio_render_event+0xd08>
    89ba: 4478         	add	r0, pc
    89bc: 68c3         	ldr	r3, [r0, #0xc]
    89be: 681b         	ldr	r3, [r3]
    89c0: 0719         	lsls	r1, r3, #0x1c
    89c2: f101 848f    	bmi.w	0xa2e4 <gst_lg_audio_render_event+0x20a4> @ imm = #0x191e
    89c6: f8d8 3000    	ldr.w	r3, [r8]
    89ca: 2b03         	cmp	r3, #0x3
    89cc: f67f ad73    	bls.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x51a
    89d0: f8df 24cc    	ldr.w	r2, [pc, #0x4cc]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    89d4: f8df 3574    	ldr.w	r3, [pc, #0x574]        @ 0x8f4c <gst_lg_audio_render_event+0xd0c>
    89d8: 58b9         	ldr	r1, [r7, r2]
    89da: f240 4287    	movw	r2, #0x487
    89de: 9200         	str	r2, [sp]
    89e0: 447b         	add	r3, pc
    89e2: 9401         	str	r4, [sp, #0x4]
    89e4: f503 73c6    	add.w	r3, r3, #0x18c
    89e8: 6808         	ldr	r0, [r1]
    89ea: f8df 1564    	ldr.w	r1, [pc, #0x564]        @ 0x8f50 <gst_lg_audio_render_event+0xd10>
    89ee: f8df 2564    	ldr.w	r2, [pc, #0x564]        @ 0x8f54 <gst_lg_audio_render_event+0xd14>
    89f2: 4479         	add	r1, pc
    89f4: 9102         	str	r1, [sp, #0x8]
    89f6: 447a         	add	r2, pc
    89f8: 2104         	movs	r1, #0x4
    89fa: f7fb e938    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4d90
    89fe: e55a         	b	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x54c
    8a00: f8df 0554    	ldr.w	r0, [pc, #0x554]        @ 0x8f58 <gst_lg_audio_render_event+0xd18>
    8a04: 2300         	movs	r3, #0x0
    8a06: 9330         	str	r3, [sp, #0xc0]
    8a08: 4478         	add	r0, pc
    8a0a: 68c3         	ldr	r3, [r0, #0xc]
    8a0c: 681b         	ldr	r3, [r3]
    8a0e: 071b         	lsls	r3, r3, #0x1c
    8a10: f101 82ed    	bmi.w	0x9fee <gst_lg_audio_render_event+0x1dae> @ imm = #0x15da
    8a14: f8d8 3000    	ldr.w	r3, [r8]
    8a18: 2b03         	cmp	r3, #0x3
    8a1a: d916         	bls	0x8a4a <gst_lg_audio_render_event+0x80a> @ imm = #0x2c
    8a1c: f8df 3480    	ldr.w	r3, [pc, #0x480]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8a20: 2104         	movs	r1, #0x4
    8a22: f8df 2538    	ldr.w	r2, [pc, #0x538]        @ 0x8f5c <gst_lg_audio_render_event+0xd1c>
    8a26: 58fb         	ldr	r3, [r7, r3]
    8a28: 447a         	add	r2, pc
    8a2a: 9401         	str	r4, [sp, #0x4]
    8a2c: 6818         	ldr	r0, [r3]
    8a2e: f8df 3530    	ldr.w	r3, [pc, #0x530]        @ 0x8f60 <gst_lg_audio_render_event+0xd20>
    8a32: 9202         	str	r2, [sp, #0x8]
    8a34: f640 123f    	movw	r2, #0x93f
    8a38: 9200         	str	r2, [sp]
    8a3a: f8df 2528    	ldr.w	r2, [pc, #0x528]        @ 0x8f64 <gst_lg_audio_render_event+0xd24>
    8a3e: 447b         	add	r3, pc
    8a40: f503 73b8    	add.w	r3, r3, #0x170
    8a44: 447a         	add	r2, pc
    8a46: f7fb e912    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4ddc
    8a4a: 4628         	mov	r0, r5
    8a4c: a930         	add	r1, sp, #0xc0
    8a4e: f7fb ebf4    	blx	0x4238 <gst_event_parse_stream_start@plt> @ imm = #-0x4818
    8a52: 9830         	ldr	r0, [sp, #0xc0]
    8a54: 2800         	cmp	r0, #0x0
    8a56: f001 8386    	beq.w	0xa166 <gst_lg_audio_render_event+0x1f26> @ imm = #0x170c
    8a5a: f8df 350c    	ldr.w	r3, [pc, #0x50c]        @ 0x8f68 <gst_lg_audio_render_event+0xd28>
    8a5e: 447b         	add	r3, pc
    8a60: 68da         	ldr	r2, [r3, #0xc]
    8a62: 6812         	ldr	r2, [r2]
    8a64: 0712         	lsls	r2, r2, #0x1c
    8a66: f101 8218    	bmi.w	0x9e9a <gst_lg_audio_render_event+0x1c5a> @ imm = #0x1430
    8a6a: f8d8 3000    	ldr.w	r3, [r8]
    8a6e: 2b03         	cmp	r3, #0x3
    8a70: d918         	bls	0x8aa4 <gst_lg_audio_render_event+0x864> @ imm = #0x30
    8a72: f8df 342c    	ldr.w	r3, [pc, #0x42c]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8a76: 2104         	movs	r1, #0x4
    8a78: 9a30         	ldr	r2, [sp, #0xc0]
    8a7a: 58fb         	ldr	r3, [r7, r3]
    8a7c: 9401         	str	r4, [sp, #0x4]
    8a7e: 6818         	ldr	r0, [r3]
    8a80: 9203         	str	r2, [sp, #0xc]
    8a82: f8df 24e8    	ldr.w	r2, [pc, #0x4e8]        @ 0x8f6c <gst_lg_audio_render_event+0xd2c>
    8a86: f8df 34e8    	ldr.w	r3, [pc, #0x4e8]        @ 0x8f70 <gst_lg_audio_render_event+0xd30>
    8a8a: 447a         	add	r2, pc
    8a8c: 9202         	str	r2, [sp, #0x8]
    8a8e: f640 1247    	movw	r2, #0x947
    8a92: 9200         	str	r2, [sp]
    8a94: 447b         	add	r3, pc
    8a96: f8df 24dc    	ldr.w	r2, [pc, #0x4dc]        @ 0x8f74 <gst_lg_audio_render_event+0xd34>
    8a9a: f503 73b8    	add.w	r3, r3, #0x170
    8a9e: 447a         	add	r2, pc
    8aa0: f7fb e8e4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4e38
    8aa4: f8d4 3204    	ldr.w	r3, [r4, #0x204]
    8aa8: f8d4 9240    	ldr.w	r9, [r4, #0x240]
    8aac: 2b00         	cmp	r3, #0x0
    8aae: d151         	bne	0x8b54 <gst_lg_audio_render_event+0x914> @ imm = #0xa2
    8ab0: 9830         	ldr	r0, [sp, #0xc0]
    8ab2: f1b9 0f00    	cmp.w	r9, #0x0
    8ab6: d054         	beq	0x8b62 <gst_lg_audio_render_event+0x922> @ imm = #0xa8
    8ab8: 4601         	mov	r1, r0
    8aba: 4648         	mov	r0, r9
    8abc: f7fb ebec    	blx	0x4298 <strcmp@plt>     @ imm = #-0x4828
    8ac0: 2800         	cmp	r0, #0x0
    8ac2: d04a         	beq	0x8b5a <gst_lg_audio_render_event+0x91a> @ imm = #0x94
    8ac4: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8ac8: f7fa eef6    	blx	0x38b8 <lgao_keep_fade@plt> @ imm = #-0x5214
    8acc: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8ad0: f7fb e986    	blx	0x3de0 <lgao_stop_renderer@plt> @ imm = #-0x4cf4
    8ad4: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8ad8: f7fb eb5a    	blx	0x4190 <lgao_disconnect_renderer@plt> @ imm = #-0x494c
    8adc: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8ae0: f7fa ef86    	blx	0x39f0 <lgao_close_renderer@plt> @ imm = #-0x50f4
    8ae4: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8ae8: f7fb e8cc    	blx	0x3c84 <lgao_open_renderer@plt> @ imm = #-0x4e68
    8aec: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8af0: f7fa ee70    	blx	0x37d4 <lgao_config@plt> @ imm = #-0x5320
    8af4: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8af8: f7fb eae4    	blx	0x40c4 <lgao_connect_renderer@plt> @ imm = #-0x4a38
    8afc: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8b00: f7fa efa6    	blx	0x3a50 <lgao_play@plt>  @ imm = #-0x50b4
    8b04: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8b08: f7fa eeb2    	blx	0x3870 <lgao_start_renderer@plt> @ imm = #-0x529c
    8b0c: f8df 0468    	ldr.w	r0, [pc, #0x468]        @ 0x8f78 <gst_lg_audio_render_event+0xd38>
    8b10: 4478         	add	r0, pc
    8b12: 68c3         	ldr	r3, [r0, #0xc]
    8b14: 681b         	ldr	r3, [r3]
    8b16: 071b         	lsls	r3, r3, #0x1c
    8b18: f102 82bf    	bmi.w	0xb09a <gst_lg_audio_render_event+0x2e5a> @ imm = #0x257e
    8b1c: f8d8 3000    	ldr.w	r3, [r8]
    8b20: 2b03         	cmp	r3, #0x3
    8b22: d915         	bls	0x8b50 <gst_lg_audio_render_event+0x910> @ imm = #0x2a
    8b24: 4ade         	ldr	r2, [pc, #0x378]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8b26: 2104         	movs	r1, #0x4
    8b28: f8df 3450    	ldr.w	r3, [pc, #0x450]        @ 0x8f7c <gst_lg_audio_render_event+0xd3c>
    8b2c: 58ba         	ldr	r2, [r7, r2]
    8b2e: 9401         	str	r4, [sp, #0x4]
    8b30: 447b         	add	r3, pc
    8b32: f503 73b8    	add.w	r3, r3, #0x170
    8b36: 6810         	ldr	r0, [r2]
    8b38: f8df 2444    	ldr.w	r2, [pc, #0x444]        @ 0x8f80 <gst_lg_audio_render_event+0xd40>
    8b3c: 447a         	add	r2, pc
    8b3e: 9202         	str	r2, [sp, #0x8]
    8b40: f640 1258    	movw	r2, #0x958
    8b44: 9200         	str	r2, [sp]
    8b46: f8df 243c    	ldr.w	r2, [pc, #0x43c]        @ 0x8f84 <gst_lg_audio_render_event+0xd44>
    8b4a: 447a         	add	r2, pc
    8b4c: f7fb e88e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4ee4
    8b50: f8d4 9240    	ldr.w	r9, [r4, #0x240]
    8b54: f1b9 0f00    	cmp.w	r9, #0x0
    8b58: d002         	beq	0x8b60 <gst_lg_audio_render_event+0x920> @ imm = #0x4
    8b5a: 4648         	mov	r0, r9
    8b5c: f7fa ee10    	blx	0x3780 <g_free@plt>     @ imm = #-0x53e0
    8b60: 9830         	ldr	r0, [sp, #0xc0]
    8b62: f7fa eefe    	blx	0x3960 <g_strdup@plt>   @ imm = #-0x5204
    8b66: f8c4 0240    	str.w	r0, [r4, #0x240]
    8b6a: e4a4         	b	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x6b8
    8b6c: f8df 9418    	ldr.w	r9, [pc, #0x418]        @ 0x8f88 <gst_lg_audio_render_event+0xd48>
    8b70: 44f9         	add	r9, pc
    8b72: f8d9 300c    	ldr.w	r3, [r9, #0xc]
    8b76: 681b         	ldr	r3, [r3]
    8b78: 0759         	lsls	r1, r3, #0x1d
    8b7a: f101 827c    	bmi.w	0xa076 <gst_lg_audio_render_event+0x1e36> @ imm = #0x14f8
    8b7e: f8d8 3000    	ldr.w	r3, [r8]
    8b82: 2b03         	cmp	r3, #0x3
    8b84: d918         	bls	0x8bb8 <gst_lg_audio_render_event+0x978> @ imm = #0x30
    8b86: 4bc6         	ldr	r3, [pc, #0x318]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8b88: 6a68         	ldr	r0, [r5, #0x24]
    8b8a: 58fb         	ldr	r3, [r7, r3]
    8b8c: f8d3 9000    	ldr.w	r9, [r3]
    8b90: f7fb e90e    	blx	0x3db0 <gst_event_type_get_name@plt> @ imm = #-0x4de4
    8b94: 4afd         	ldr	r2, [pc, #0x3f4]        @ 0x8f8c <gst_lg_audio_render_event+0xd4c>
    8b96: 2104         	movs	r1, #0x4
    8b98: 4bfd         	ldr	r3, [pc, #0x3f4]        @ 0x8f90 <gst_lg_audio_render_event+0xd50>
    8b9a: 9003         	str	r0, [sp, #0xc]
    8b9c: 4648         	mov	r0, r9
    8b9e: 447a         	add	r2, pc
    8ba0: 9401         	str	r4, [sp, #0x4]
    8ba2: 9202         	str	r2, [sp, #0x8]
    8ba4: f640 1264    	movw	r2, #0x964
    8ba8: 9200         	str	r2, [sp]
    8baa: 447b         	add	r3, pc
    8bac: 4af9         	ldr	r2, [pc, #0x3e4]        @ 0x8f94 <gst_lg_audio_render_event+0xd54>
    8bae: f503 73b8    	add.w	r3, r3, #0x170
    8bb2: 447a         	add	r2, pc
    8bb4: f7fb e85a    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4f4c
    8bb8: ffc7 0e1f    	vmov.i8	d16, #0xff
    8bbc: 4628         	mov	r0, r5
    8bbe: edcd 0b2e    	vstr	d16, [sp, #184]
    8bc2: edcd 0b30    	vstr	d16, [sp, #192]
    8bc6: f7fb e8ca    	blx	0x3d5c <gst_event_get_structure@plt> @ imm = #-0x4e6c
    8bca: 4681         	mov	r9, r0
    8bcc: 2800         	cmp	r0, #0x0
    8bce: f43f ac72    	beq.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x71c
    8bd2: 49f1         	ldr	r1, [pc, #0x3c4]        @ 0x8f98 <gst_lg_audio_render_event+0xd58>
    8bd4: 4479         	add	r1, pc
    8bd6: f7fa ee1c    	blx	0x3810 <gst_structure_has_name@plt> @ imm = #-0x53c8
    8bda: 2800         	cmp	r0, #0x0
    8bdc: f43f ac6b    	beq.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x72a
    8be0: f8df a3b8    	ldr.w	r10, [pc, #0x3b8]       @ 0x8f9c <gst_lg_audio_render_event+0xd5c>
    8be4: 4648         	mov	r0, r9
    8be6: 44fa         	add	r10, pc
    8be8: 4651         	mov	r1, r10
    8bea: f7fb e9da    	blx	0x3fa0 <gst_structure_has_field@plt> @ imm = #-0x4c4c
    8bee: 2800         	cmp	r0, #0x0
    8bf0: f042 8107    	bne.w	0xae02 <gst_lg_audio_render_event+0x2bc2> @ imm = #0x220e
    8bf4: 48ea         	ldr	r0, [pc, #0x3a8]        @ 0x8fa0 <gst_lg_audio_render_event+0xd60>
    8bf6: 4478         	add	r0, pc
    8bf8: 68c3         	ldr	r3, [r0, #0xc]
    8bfa: 681b         	ldr	r3, [r3]
    8bfc: 075a         	lsls	r2, r3, #0x1d
    8bfe: f102 8177    	bmi.w	0xaef0 <gst_lg_audio_render_event+0x2cb0> @ imm = #0x22ee
    8c02: f8d8 3000    	ldr.w	r3, [r8]
    8c06: 2b03         	cmp	r3, #0x3
    8c08: d912         	bls	0x8c30 <gst_lg_audio_render_event+0x9f0> @ imm = #0x24
    8c0a: 4ba5         	ldr	r3, [pc, #0x294]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8c0c: 2104         	movs	r1, #0x4
    8c0e: 4ae5         	ldr	r2, [pc, #0x394]        @ 0x8fa4 <gst_lg_audio_render_event+0xd64>
    8c10: 58fb         	ldr	r3, [r7, r3]
    8c12: 447a         	add	r2, pc
    8c14: 9401         	str	r4, [sp, #0x4]
    8c16: 6818         	ldr	r0, [r3]
    8c18: 4be3         	ldr	r3, [pc, #0x38c]        @ 0x8fa8 <gst_lg_audio_render_event+0xd68>
    8c1a: 9202         	str	r2, [sp, #0x8]
    8c1c: f640 1272    	movw	r2, #0x972
    8c20: 9200         	str	r2, [sp]
    8c22: 4ae2         	ldr	r2, [pc, #0x388]        @ 0x8fac <gst_lg_audio_render_event+0xd6c>
    8c24: 447b         	add	r3, pc
    8c26: f503 73b8    	add.w	r3, r3, #0x170
    8c2a: 447a         	add	r2, pc
    8c2c: f7fb e81e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x4fc4
    8c30: f8df a37c    	ldr.w	r10, [pc, #0x37c]       @ 0x8fb0 <gst_lg_audio_render_event+0xd70>
    8c34: 4648         	mov	r0, r9
    8c36: 44fa         	add	r10, pc
    8c38: 4651         	mov	r1, r10
    8c3a: f7fb e9b2    	blx	0x3fa0 <gst_structure_has_field@plt> @ imm = #-0x4c9c
    8c3e: 2800         	cmp	r0, #0x0
    8c40: f042 80e6    	bne.w	0xae10 <gst_lg_audio_render_event+0x2bd0> @ imm = #0x21cc
    8c44: 48db         	ldr	r0, [pc, #0x36c]        @ 0x8fb4 <gst_lg_audio_render_event+0xd74>
    8c46: 4478         	add	r0, pc
    8c48: 68c3         	ldr	r3, [r0, #0xc]
    8c4a: 681b         	ldr	r3, [r3]
    8c4c: 075b         	lsls	r3, r3, #0x1d
    8c4e: f102 815f    	bmi.w	0xaf10 <gst_lg_audio_render_event+0x2cd0> @ imm = #0x22be
    8c52: f8d8 3000    	ldr.w	r3, [r8]
    8c56: 2b03         	cmp	r3, #0x3
    8c58: d912         	bls	0x8c80 <gst_lg_audio_render_event+0xa40> @ imm = #0x24
    8c5a: 4b91         	ldr	r3, [pc, #0x244]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8c5c: 2104         	movs	r1, #0x4
    8c5e: 4ad6         	ldr	r2, [pc, #0x358]        @ 0x8fb8 <gst_lg_audio_render_event+0xd78>
    8c60: 58fb         	ldr	r3, [r7, r3]
    8c62: 447a         	add	r2, pc
    8c64: 9401         	str	r4, [sp, #0x4]
    8c66: 6818         	ldr	r0, [r3]
    8c68: 4bd4         	ldr	r3, [pc, #0x350]        @ 0x8fbc <gst_lg_audio_render_event+0xd7c>
    8c6a: 9202         	str	r2, [sp, #0x8]
    8c6c: f640 1278    	movw	r2, #0x978
    8c70: 9200         	str	r2, [sp]
    8c72: 4ad3         	ldr	r2, [pc, #0x34c]        @ 0x8fc0 <gst_lg_audio_render_event+0xd80>
    8c74: 447b         	add	r3, pc
    8c76: f503 73b8    	add.w	r3, r3, #0x170
    8c7a: 447a         	add	r2, pc
    8c7c: f7fa eff6    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5014
    8c80: e9dd 232e    	ldrd	r2, r3, [sp, #184]
    8c84: f1b3 3fff    	cmp.w	r3, #0xffffffff
    8c88: bf08         	it	eq
    8c8a: f1b2 3fff    	cmpeq.w	r2, #0xffffffff
    8c8e: f43f ac12    	beq.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x7dc
    8c92: e9dd 0930    	ldrd	r0, r9, [sp, #192]
    8c96: f1b9 3fff    	cmp.w	r9, #0xffffffff
    8c9a: bf08         	it	eq
    8c9c: f1b0 3fff    	cmpeq.w	r0, #0xffffffff
    8ca0: f43f ac09    	beq.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x7ee
    8ca4: f8df c31c    	ldr.w	r12, [pc, #0x31c]       @ 0x8fc4 <gst_lg_audio_render_event+0xd84>
    8ca8: 44fc         	add	r12, pc
    8caa: f8dc 100c    	ldr.w	r1, [r12, #0xc]
    8cae: f8d1 e000    	ldr.w	lr, [r1]
    8cb2: f01e 0f04    	tst.w	lr, #0x4
    8cb6: f042 8200    	bne.w	0xb0ba <gst_lg_audio_render_event+0x2e7a> @ imm = #0x2400
    8cba: f8d8 c000    	ldr.w	r12, [r8]
    8cbe: f1bc 0f03    	cmp.w	r12, #0x3
    8cc2: d91c         	bls	0x8cfe <gst_lg_audio_render_event+0xabe> @ imm = #0x38
    8cc4: f8df c1d8    	ldr.w	r12, [pc, #0x1d8]       @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8cc8: 2104         	movs	r1, #0x4
    8cca: f857 700c    	ldr.w	r7, [r7, r12]
    8cce: e9cd 2304    	strd	r2, r3, [sp, #16]
    8cd2: 4bbd         	ldr	r3, [pc, #0x2f4]        @ 0x8fc8 <gst_lg_audio_render_event+0xd88>
    8cd4: e9cd 0906    	strd	r0, r9, [sp, #24]
    8cd8: 6838         	ldr	r0, [r7]
    8cda: 447b         	add	r3, pc
    8cdc: 4abb         	ldr	r2, [pc, #0x2ec]        @ 0x8fcc <gst_lg_audio_render_event+0xd8c>
    8cde: e9cd 4301    	strd	r4, r3, [sp, #4]
    8ce2: f640 137c    	movw	r3, #0x97c
    8ce6: 9300         	str	r3, [sp]
    8ce8: 4bb9         	ldr	r3, [pc, #0x2e4]        @ 0x8fd0 <gst_lg_audio_render_event+0xd90>
    8cea: 447a         	add	r2, pc
    8cec: 447b         	add	r3, pc
    8cee: f503 73b8    	add.w	r3, r3, #0x170
    8cf2: f7fa efbc    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5088
    8cf6: e9dd 232e    	ldrd	r2, r3, [sp, #184]
    8cfa: e9dd 0930    	ldrd	r0, r9, [sp, #192]
    8cfe: f8d4 4200    	ldr.w	r4, [r4, #0x200]
    8d02: e9cd 0900    	strd	r0, r9, [sp]
    8d06: 4620         	mov	r0, r4
    8d08: f7fa efe0    	blx	0x3ccc <lgao_audio_gap@plt> @ imm = #-0x5040
    8d0c: f7ff bbd3    	b.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x85a
    8d10: 48b0         	ldr	r0, [pc, #0x2c0]        @ 0x8fd4 <gst_lg_audio_render_event+0xd94>
    8d12: 2300         	movs	r3, #0x0
    8d14: 932e         	str	r3, [sp, #0xb8]
    8d16: 9330         	str	r3, [sp, #0xc0]
    8d18: 4478         	add	r0, pc
    8d1a: 68c3         	ldr	r3, [r0, #0xc]
    8d1c: 681b         	ldr	r3, [r3]
    8d1e: 0719         	lsls	r1, r3, #0x1c
    8d20: f101 8189    	bmi.w	0xa036 <gst_lg_audio_render_event+0x1df6> @ imm = #0x1312
    8d24: f8d8 3000    	ldr.w	r3, [r8]
    8d28: 2b03         	cmp	r3, #0x3
    8d2a: d912         	bls	0x8d52 <gst_lg_audio_render_event+0xb12> @ imm = #0x24
    8d2c: 4b5c         	ldr	r3, [pc, #0x170]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8d2e: 2104         	movs	r1, #0x4
    8d30: 4aa9         	ldr	r2, [pc, #0x2a4]        @ 0x8fd8 <gst_lg_audio_render_event+0xd98>
    8d32: 58fb         	ldr	r3, [r7, r3]
    8d34: 447a         	add	r2, pc
    8d36: 9401         	str	r4, [sp, #0x4]
    8d38: 6818         	ldr	r0, [r3]
    8d3a: 4ba8         	ldr	r3, [pc, #0x2a0]        @ 0x8fdc <gst_lg_audio_render_event+0xd9c>
    8d3c: 9202         	str	r2, [sp, #0x8]
    8d3e: f640 02cc    	movw	r2, #0x8cc
    8d42: 9200         	str	r2, [sp]
    8d44: 4aa6         	ldr	r2, [pc, #0x298]        @ 0x8fe0 <gst_lg_audio_render_event+0xda0>
    8d46: 447b         	add	r3, pc
    8d48: f503 73b8    	add.w	r3, r3, #0x170
    8d4c: 447a         	add	r2, pc
    8d4e: f7fa ef8e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x50e4
    8d52: aa2e         	add	r2, sp, #0xb8
    8d54: 2100         	movs	r1, #0x0
    8d56: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8d5a: f7fb ead6    	blx	0x4308 <lgao_get_size_buffered@plt> @ imm = #-0x4a54
    8d5e: aa30         	add	r2, sp, #0xc0
    8d60: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8d64: 2100         	movs	r1, #0x0
    8d66: f7fb e9c0    	blx	0x40e8 <lgao_get_first_render@plt> @ imm = #-0x4c80
    8d6a: 489e         	ldr	r0, [pc, #0x278]        @ 0x8fe4 <gst_lg_audio_render_event+0xda4>
    8d6c: 4478         	add	r0, pc
    8d6e: 68c3         	ldr	r3, [r0, #0xc]
    8d70: 681b         	ldr	r3, [r3]
    8d72: 071a         	lsls	r2, r3, #0x1c
    8d74: f101 814b    	bmi.w	0xa00e <gst_lg_audio_render_event+0x1dce> @ imm = #0x1296
    8d78: f8d8 3000    	ldr.w	r3, [r8]
    8d7c: 2b03         	cmp	r3, #0x3
    8d7e: d916         	bls	0x8dae <gst_lg_audio_render_event+0xb6e> @ imm = #0x2c
    8d80: 4b47         	ldr	r3, [pc, #0x11c]        @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8d82: 2104         	movs	r1, #0x4
    8d84: 9a30         	ldr	r2, [sp, #0xc0]
    8d86: 58fb         	ldr	r3, [r7, r3]
    8d88: 9401         	str	r4, [sp, #0x4]
    8d8a: 6818         	ldr	r0, [r3]
    8d8c: 9204         	str	r2, [sp, #0x10]
    8d8e: 9a2e         	ldr	r2, [sp, #0xb8]
    8d90: 4b95         	ldr	r3, [pc, #0x254]        @ 0x8fe8 <gst_lg_audio_render_event+0xda8>
    8d92: 9203         	str	r2, [sp, #0xc]
    8d94: 4a95         	ldr	r2, [pc, #0x254]        @ 0x8fec <gst_lg_audio_render_event+0xdac>
    8d96: 447b         	add	r3, pc
    8d98: f503 73b8    	add.w	r3, r3, #0x170
    8d9c: 447a         	add	r2, pc
    8d9e: 9202         	str	r2, [sp, #0x8]
    8da0: f640 02d2    	movw	r2, #0x8d2
    8da4: 9200         	str	r2, [sp]
    8da6: 4a92         	ldr	r2, [pc, #0x248]        @ 0x8ff0 <gst_lg_audio_render_event+0xdb0>
    8da8: 447a         	add	r2, pc
    8daa: f7fa ef60    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5140
    8dae: 9b2e         	ldr	r3, [sp, #0xb8]
    8db0: b91b         	cbnz	r3, 0x8dba <gst_lg_audio_render_event+0xb7a> @ imm = #0x6
    8db2: 9b30         	ldr	r3, [sp, #0xc0]
    8db4: 2b01         	cmp	r3, #0x1
    8db6: f001 8082    	beq.w	0x9ebe <gst_lg_audio_render_event+0x1c7e> @ imm = #0x1104
    8dba: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    8dbe: f7fa ed82    	blx	0x38c4 <lgao_eos@plt>   @ imm = #-0x54fc
    8dc2: 2800         	cmp	r0, #0x0
    8dc4: f341 821a    	ble.w	0xa1fc <gst_lg_audio_render_event+0x1fbc> @ imm = #0x1434
    8dc8: 488a         	ldr	r0, [pc, #0x228]        @ 0x8ff4 <gst_lg_audio_render_event+0xdb4>
    8dca: 4478         	add	r0, pc
    8dcc: 68c3         	ldr	r3, [r0, #0xc]
    8dce: 681b         	ldr	r3, [r3]
    8dd0: 0759         	lsls	r1, r3, #0x1d
    8dd2: f101 809a    	bmi.w	0x9f0a <gst_lg_audio_render_event+0x1cca> @ imm = #0x1134
    8dd6: f8d8 3000    	ldr.w	r3, [r8]
    8dda: 2b03         	cmp	r3, #0x3
    8ddc: f67f ab6b    	bls.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x92a
    8de0: 4b2f         	ldr	r3, [pc, #0xbc]         @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8de2: 2104         	movs	r1, #0x4
    8de4: 4a84         	ldr	r2, [pc, #0x210]        @ 0x8ff8 <gst_lg_audio_render_event+0xdb8>
    8de6: 58fb         	ldr	r3, [r7, r3]
    8de8: 447a         	add	r2, pc
    8dea: 9401         	str	r4, [sp, #0x4]
    8dec: 6818         	ldr	r0, [r3]
    8dee: 4b83         	ldr	r3, [pc, #0x20c]        @ 0x8ffc <gst_lg_audio_render_event+0xdbc>
    8df0: 9202         	str	r2, [sp, #0x8]
    8df2: f640 02d9    	movw	r2, #0x8d9
    8df6: 9200         	str	r2, [sp]
    8df8: 4a81         	ldr	r2, [pc, #0x204]        @ 0x9000 <gst_lg_audio_render_event+0xdc0>
    8dfa: 447b         	add	r3, pc
    8dfc: f503 73b8    	add.w	r3, r3, #0x170
    8e00: 447a         	add	r2, pc
    8e02: f7fa ef34    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5198
    8e06: 4b7f         	ldr	r3, [pc, #0x1fc]        @ 0x9004 <gst_lg_audio_render_event+0xdc4>
    8e08: 447b         	add	r3, pc
    8e0a: 68db         	ldr	r3, [r3, #0xc]
    8e0c: 681b         	ldr	r3, [r3]
    8e0e: 0759         	lsls	r1, r3, #0x1d
    8e10: f101 8096    	bmi.w	0x9f40 <gst_lg_audio_render_event+0x1d00> @ imm = #0x112c
    8e14: f8d8 3000    	ldr.w	r3, [r8]
    8e18: 2b03         	cmp	r3, #0x3
    8e1a: f67f ab4c    	bls.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x968
    8e1e: 4a20         	ldr	r2, [pc, #0x80]         @ 0x8ea0 <gst_lg_audio_render_event+0xc60>
    8e20: 4b79         	ldr	r3, [pc, #0x1e4]        @ 0x9008 <gst_lg_audio_render_event+0xdc8>
    8e22: 58b9         	ldr	r1, [r7, r2]
    8e24: f640 02e6    	movw	r2, #0x8e6
    8e28: 9200         	str	r2, [sp]
    8e2a: 447b         	add	r3, pc
    8e2c: 9401         	str	r4, [sp, #0x4]
    8e2e: f503 73b8    	add.w	r3, r3, #0x170
    8e32: 6808         	ldr	r0, [r1]
    8e34: 4975         	ldr	r1, [pc, #0x1d4]        @ 0x900c <gst_lg_audio_render_event+0xdcc>
    8e36: 4a76         	ldr	r2, [pc, #0x1d8]        @ 0x9010 <gst_lg_audio_render_event+0xdd0>
    8e38: 4479         	add	r1, pc
    8e3a: 9102         	str	r1, [sp, #0x8]
    8e3c: 447a         	add	r2, pc
    8e3e: 2104         	movs	r1, #0x4
    8e40: f7fa ef14    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x51d8
    8e44: f7ff bb37    	b.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x992
    8e48: 4a72         	ldr	r2, [pc, #0x1c8]        @ 0x9014 <gst_lg_audio_render_event+0xdd4>
    8e4a: 2103         	movs	r1, #0x3
    8e4c: 4b72         	ldr	r3, [pc, #0x1c8]        @ 0x9018 <gst_lg_audio_render_event+0xdd8>
    8e4e: 447a         	add	r2, pc
    8e50: 447b         	add	r3, pc
    8e52: f502 72b8    	add.w	r2, r2, #0x170
    8e56: 9300         	str	r3, [sp]
    8e58: f640 131c    	movw	r3, #0x91c
    8e5c: f7fb e8a6    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x4eb4
    8e60: f7ff bbbc    	b.w	0x85dc <gst_lg_audio_render_event+0x39c> @ imm = #-0x888
    8e64: 4a6d         	ldr	r2, [pc, #0x1b4]        @ 0x901c <gst_lg_audio_render_event+0xddc>
    8e66: 2103         	movs	r1, #0x3
    8e68: 4b6d         	ldr	r3, [pc, #0x1b4]        @ 0x9020 <gst_lg_audio_render_event+0xde0>
    8e6a: 447a         	add	r2, pc
    8e6c: 447b         	add	r3, pc
    8e6e: f502 72b8    	add.w	r2, r2, #0x170
    8e72: 9300         	str	r3, [sp]
    8e74: f640 03be    	movw	r3, #0x8be
    8e78: f7fb e898    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x4ed0
    8e7c: e49f         	b	0x87be <gst_lg_audio_render_event+0x57e> @ imm = #-0x6c2
    8e7e: 4a69         	ldr	r2, [pc, #0x1a4]        @ 0x9024 <gst_lg_audio_render_event+0xde4>
    8e80: 2103         	movs	r1, #0x3
    8e82: 4b69         	ldr	r3, [pc, #0x1a4]        @ 0x9028 <gst_lg_audio_render_event+0xde8>
    8e84: 447a         	add	r2, pc
    8e86: 447b         	add	r3, pc
    8e88: f502 72b8    	add.w	r2, r2, #0x170
    8e8c: 9300         	str	r3, [sp]
    8e8e: f640 03b2    	movw	r3, #0x8b2
    8e92: f7fb e88c    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x4ee8
    8e96: f7ff bb2f    	b.w	0x84f8 <gst_lg_audio_render_event+0x2b8> @ imm = #-0x9a2
    8e9a: bf00         	nop
    8e9c: 30 9a 02 00  	.word	0x00029a30
    8ea0: 08 04 00 00  	.word	0x00000408
    8ea4: f8 2b 01 00  	.word	0x00012bf8
    8ea8: 8e 75 01 00  	.word	0x0001758e
    8eac: 2c 13 01 00  	.word	0x0001132c
    8eb0: 8a 2b 01 00  	.word	0x00012b8a
    8eb4: fe 74 01 00  	.word	0x000174fe
    8eb8: 9c 12 01 00  	.word	0x0001129c
    8ebc: 7e 2b 01 00  	.word	0x00012b7e
    8ec0: b8 74 01 00  	.word	0x000174b8
    8ec4: 56 12 01 00  	.word	0x00011256
    8ec8: fe 98 02 00  	.word	0x000298fe
    8ecc: 48 26 01 00  	.word	0x00012648
    8ed0: 62 74 01 00  	.word	0x00017462
    8ed4: c8 98 02 00  	.word	0x000298c8
    8ed8: 3a 2b 01 00  	.word	0x00012b3a
    8edc: 04 26 01 00  	.word	0x00012604
    8ee0: 14 74 01 00  	.word	0x00017414
    8ee4: 12 2b 01 00  	.word	0x00012b12
    8ee8: a4 11 01 00  	.word	0x000111a4
    8eec: 4e 98 02 00  	.word	0x0002984e
    8ef0: c0 73 01 00  	.word	0x000173c0
    8ef4: 90 26 01 00  	.word	0x00012690
    8ef8: 4a 11 01 00  	.word	0x0001114a
    8efc: 6a 73 01 00  	.word	0x0001736a
    8f00: 3c 25 01 00  	.word	0x0001253c
    8f04: be 97 02 00  	.word	0x000297be
    8f08: 30 73 01 00  	.word	0x00017330
    8f0c: a4 25 01 00  	.word	0x000125a4
    8f10: ba 10 01 00  	.word	0x000110ba
    8f14: 44 2a 01 00  	.word	0x00012a44
    8f18: cc 72 01 00  	.word	0x000172cc
    8f1c: 6a 10 01 00  	.word	0x0001106a
    8f20: 90 2a 01 00  	.word	0x00012a90
    8f24: fe 96 02 00  	.word	0x000296fe
    8f28: 7a 2a 01 00  	.word	0x00012a7a
    8f2c: 5c 72 01 00  	.word	0x0001725c
    8f30: f6 0f 01 00  	.word	0x00010ff6
    8f34: a6 96 02 00  	.word	0x000296a6
    8f38: 14 72 01 00  	.word	0x00017214
    8f3c: 52 2a 01 00  	.word	0x00012a52
    8f40: a2 0f 01 00  	.word	0x00010fa2
    8f44: 58 2a 01 00  	.word	0x00012a58
    8f48: 46 96 02 00  	.word	0x00029646
    8f4c: b4 71 01 00  	.word	0x000171b4
    8f50: 46 2a 01 00  	.word	0x00012a46
    8f54: 42 0f 01 00  	.word	0x00010f42
    8f58: f8 95 02 00  	.word	0x000295f8
    8f5c: 24 3a 01 00  	.word	0x00013a24
    8f60: 56 71 01 00  	.word	0x00017156
    8f64: f4 0e 01 00  	.word	0x00010ef4
    8f68: a2 95 02 00  	.word	0x000295a2
    8f6c: 3a 3a 01 00  	.word	0x00013a3a
    8f70: 00 71 01 00  	.word	0x00017100
    8f74: 9a 0e 01 00  	.word	0x00010e9a
    8f78: f0 94 02 00  	.word	0x000294f0
    8f7c: 64 70 01 00  	.word	0x00017064
    8f80: d8 39 01 00  	.word	0x000139d8
    8f84: ee 0d 01 00  	.word	0x00010dee
    8f88: 90 94 02 00  	.word	0x00029490
    8f8c: e6 21 01 00  	.word	0x000121e6
    8f90: ea 6f 01 00  	.word	0x00016fea
    8f94: 86 0d 01 00  	.word	0x00010d86
    8f98: 74 39 01 00  	.word	0x00013974
    8f9c: 6e 39 01 00  	.word	0x0001396e
    8fa0: 0a 94 02 00  	.word	0x0002940a
    8fa4: 86 39 01 00  	.word	0x00013986
    8fa8: 70 6f 01 00  	.word	0x00016f70
    8fac: 0e 0d 01 00  	.word	0x00010d0e
    8fb0: 8a 39 01 00  	.word	0x0001398a
    8fb4: ba 93 02 00  	.word	0x000293ba
    8fb8: a2 39 01 00  	.word	0x000139a2
    8fbc: 20 6f 01 00  	.word	0x00016f20
    8fc0: be 0c 01 00  	.word	0x00010cbe
    8fc4: 58 93 02 00  	.word	0x00029358
    8fc8: 96 39 01 00  	.word	0x00013996
    8fcc: 4e 0c 01 00  	.word	0x00010c4e
    8fd0: a8 6e 01 00  	.word	0x00016ea8
    8fd4: e8 92 02 00  	.word	0x000292e8
    8fd8: 90 21 01 00  	.word	0x00012190
    8fdc: 4e 6e 01 00  	.word	0x00016e4e
    8fe0: ec 0b 01 00  	.word	0x00010bec
    8fe4: 94 92 02 00  	.word	0x00029294
    8fe8: fe 6d 01 00  	.word	0x00016dfe
    8fec: 80 21 01 00  	.word	0x00012180
    8ff0: 90 0b 01 00  	.word	0x00010b90
    8ff4: 36 92 02 00  	.word	0x00029236
    8ff8: c8 21 01 00  	.word	0x000121c8
    8ffc: 9a 6d 01 00  	.word	0x00016d9a
    9000: 38 0b 01 00  	.word	0x00010b38
    9004: f8 91 02 00  	.word	0x000291f8
    9008: 6a 6d 01 00  	.word	0x00016d6a
    900c: 3c 22 01 00  	.word	0x0001223c
    9010: fc 0a 01 00  	.word	0x00010afc
    9014: 46 6d 01 00  	.word	0x00016d46
    9018: 68 23 01 00  	.word	0x00012368
    901c: 2a 6d 01 00  	.word	0x00016d2a
    9020: d0 1f 01 00  	.word	0x00011fd0
    9024: 10 6d 01 00  	.word	0x00016d10
    9028: 0e 1f 01 00  	.word	0x00011f0e
    902c: 4648         	mov	r0, r9
    902e: f7fa ec32    	blx	0x3894 <gst_structure_get_name@plt> @ imm = #-0x579c
    9032: 4683         	mov	r11, r0
    9034: 2800         	cmp	r0, #0x0
    9036: f001 86f2    	beq.w	0xae1e <gst_lg_audio_render_event+0x2bde> @ imm = #0x1de4
    903a: f8df 0bd0    	ldr.w	r0, [pc, #0xbd0]        @ 0x9c0c <gst_lg_audio_render_event+0x19cc>
    903e: 4478         	add	r0, pc
    9040: 68c3         	ldr	r3, [r0, #0xc]
    9042: 681b         	ldr	r3, [r3]
    9044: 06db         	lsls	r3, r3, #0x1b
    9046: f101 8177    	bmi.w	0xa338 <gst_lg_audio_render_event+0x20f8> @ imm = #0x12ee
    904a: f8d8 3000    	ldr.w	r3, [r8]
    904e: 2b04         	cmp	r3, #0x4
    9050: d918         	bls	0x9084 <gst_lg_audio_render_event+0xe44> @ imm = #0x30
    9052: f8df 3bbc    	ldr.w	r3, [pc, #0xbbc]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9056: 2105         	movs	r1, #0x5
    9058: f8df 2bb8    	ldr.w	r2, [pc, #0xbb8]        @ 0x9c14 <gst_lg_audio_render_event+0x19d4>
    905c: 58fb         	ldr	r3, [r7, r3]
    905e: 447a         	add	r2, pc
    9060: f8cd b00c    	str.w	r11, [sp, #0xc]
    9064: 9401         	str	r4, [sp, #0x4]
    9066: 6818         	ldr	r0, [r3]
    9068: f8df 3bac    	ldr.w	r3, [pc, #0xbac]        @ 0x9c18 <gst_lg_audio_render_event+0x19d8>
    906c: 9202         	str	r2, [sp, #0x8]
    906e: f240 428f    	movw	r2, #0x48f
    9072: 9200         	str	r2, [sp]
    9074: f8df 2ba4    	ldr.w	r2, [pc, #0xba4]        @ 0x9c1c <gst_lg_audio_render_event+0x19dc>
    9078: 447b         	add	r3, pc
    907a: f503 73c6    	add.w	r3, r3, #0x18c
    907e: 447a         	add	r2, pc
    9080: f7fa edf4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5418
    9084: f8df 1b98    	ldr.w	r1, [pc, #0xb98]        @ 0x9c20 <gst_lg_audio_render_event+0x19e0>
    9088: 4658         	mov	r0, r11
    908a: 4479         	add	r1, pc
    908c: f7fa ef02    	blx	0x3e94 <strstr@plt>     @ imm = #-0x51fc
    9090: 2800         	cmp	r0, #0x0
    9092: f001 8177    	beq.w	0xa384 <gst_lg_audio_render_event+0x2144> @ imm = #0x12ee
    9096: f8df 1b8c    	ldr.w	r1, [pc, #0xb8c]        @ 0x9c24 <gst_lg_audio_render_event+0x19e4>
    909a: 4652         	mov	r2, r10
    909c: 4648         	mov	r0, r9
    909e: 4479         	add	r1, pc
    90a0: f7fa ecb2    	blx	0x3a08 <gst_structure_get_int@plt> @ imm = #-0x569c
    90a4: b330         	cbz	r0, 0x90f4 <gst_lg_audio_render_event+0xeb4> @ imm = #0x4c
    90a6: f8df 0b80    	ldr.w	r0, [pc, #0xb80]        @ 0x9c28 <gst_lg_audio_render_event+0x19e8>
    90aa: 9b1a         	ldr	r3, [sp, #0x68]
    90ac: 4478         	add	r0, pc
    90ae: 68c2         	ldr	r2, [r0, #0xc]
    90b0: 9331         	str	r3, [sp, #0xc4]
    90b2: 6812         	ldr	r2, [r2]
    90b4: 06d1         	lsls	r1, r2, #0x1b
    90b6: f101 87ce    	bmi.w	0xb056 <gst_lg_audio_render_event+0x2e16> @ imm = #0x1f9c
    90ba: f8d8 3000    	ldr.w	r3, [r8]
    90be: 2b04         	cmp	r3, #0x4
    90c0: d918         	bls	0x90f4 <gst_lg_audio_render_event+0xeb4> @ imm = #0x30
    90c2: f8df 3b4c    	ldr.w	r3, [pc, #0xb4c]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    90c6: 2105         	movs	r1, #0x5
    90c8: 9a1a         	ldr	r2, [sp, #0x68]
    90ca: 58fb         	ldr	r3, [r7, r3]
    90cc: 9401         	str	r4, [sp, #0x4]
    90ce: 6818         	ldr	r0, [r3]
    90d0: 9203         	str	r2, [sp, #0xc]
    90d2: f8df 2b58    	ldr.w	r2, [pc, #0xb58]        @ 0x9c2c <gst_lg_audio_render_event+0x19ec>
    90d6: f8df 3b58    	ldr.w	r3, [pc, #0xb58]        @ 0x9c30 <gst_lg_audio_render_event+0x19f0>
    90da: 447a         	add	r2, pc
    90dc: 9202         	str	r2, [sp, #0x8]
    90de: f44f 6293    	mov.w	r2, #0x498
    90e2: 9200         	str	r2, [sp]
    90e4: 447b         	add	r3, pc
    90e6: f8df 2b4c    	ldr.w	r2, [pc, #0xb4c]        @ 0x9c34 <gst_lg_audio_render_event+0x19f4>
    90ea: f503 73c6    	add.w	r3, r3, #0x18c
    90ee: 447a         	add	r2, pc
    90f0: f7fa edbc    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5488
    90f4: f8df 1b40    	ldr.w	r1, [pc, #0xb40]        @ 0x9c38 <gst_lg_audio_render_event+0x19f8>
    90f8: 4652         	mov	r2, r10
    90fa: 4648         	mov	r0, r9
    90fc: 4479         	add	r1, pc
    90fe: f7fa ec84    	blx	0x3a08 <gst_structure_get_int@plt> @ imm = #-0x56f8
    9102: b338         	cbz	r0, 0x9154 <gst_lg_audio_render_event+0xf14> @ imm = #0x4e
    9104: f8df 0b34    	ldr.w	r0, [pc, #0xb34]        @ 0x9c3c <gst_lg_audio_render_event+0x19fc>
    9108: 9b1a         	ldr	r3, [sp, #0x68]
    910a: 4478         	add	r0, pc
    910c: 68c2         	ldr	r2, [r0, #0xc]
    910e: f88d 30c8    	strb.w	r3, [sp, #0xc8]
    9112: 6812         	ldr	r2, [r2]
    9114: 06d2         	lsls	r2, r2, #0x1b
    9116: f101 87af    	bmi.w	0xb078 <gst_lg_audio_render_event+0x2e38> @ imm = #0x1f5e
    911a: f8d8 3000    	ldr.w	r3, [r8]
    911e: 2b04         	cmp	r3, #0x4
    9120: d918         	bls	0x9154 <gst_lg_audio_render_event+0xf14> @ imm = #0x30
    9122: f8df 3aec    	ldr.w	r3, [pc, #0xaec]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9126: 2105         	movs	r1, #0x5
    9128: 9a1a         	ldr	r2, [sp, #0x68]
    912a: 58fb         	ldr	r3, [r7, r3]
    912c: 9401         	str	r4, [sp, #0x4]
    912e: 6818         	ldr	r0, [r3]
    9130: 9203         	str	r2, [sp, #0xc]
    9132: f8df 2b0c    	ldr.w	r2, [pc, #0xb0c]        @ 0x9c40 <gst_lg_audio_render_event+0x1a00>
    9136: f8df 3b0c    	ldr.w	r3, [pc, #0xb0c]        @ 0x9c44 <gst_lg_audio_render_event+0x1a04>
    913a: 447a         	add	r2, pc
    913c: 9202         	str	r2, [sp, #0x8]
    913e: f240 429d    	movw	r2, #0x49d
    9142: 9200         	str	r2, [sp]
    9144: 447b         	add	r3, pc
    9146: f8df 2b00    	ldr.w	r2, [pc, #0xb00]        @ 0x9c48 <gst_lg_audio_render_event+0x1a08>
    914a: f503 73c6    	add.w	r3, r3, #0x18c
    914e: 447a         	add	r2, pc
    9150: f7fa ed8c    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x54e8
    9154: f8df 1af4    	ldr.w	r1, [pc, #0xaf4]        @ 0x9c4c <gst_lg_audio_render_event+0x1a0c>
    9158: 4648         	mov	r0, r9
    915a: 4479         	add	r1, pc
    915c: f7fa ede0    	blx	0x3d20 <gst_structure_get_string@plt> @ imm = #-0x5440
    9160: 4683         	mov	r11, r0
    9162: 2800         	cmp	r0, #0x0
    9164: f001 874a    	beq.w	0xaffc <gst_lg_audio_render_event+0x2dbc> @ imm = #0x1e94
    9168: f8df 0ae4    	ldr.w	r0, [pc, #0xae4]        @ 0x9c50 <gst_lg_audio_render_event+0x1a10>
    916c: 4478         	add	r0, pc
    916e: 68c3         	ldr	r3, [r0, #0xc]
    9170: 681b         	ldr	r3, [r3]
    9172: 06db         	lsls	r3, r3, #0x1b
    9174: f101 869a    	bmi.w	0xaeac <gst_lg_audio_render_event+0x2c6c> @ imm = #0x1d34
    9178: f8d8 3000    	ldr.w	r3, [r8]
    917c: 2b04         	cmp	r3, #0x4
    917e: d918         	bls	0x91b2 <gst_lg_audio_render_event+0xf72> @ imm = #0x30
    9180: f8df 3a8c    	ldr.w	r3, [pc, #0xa8c]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9184: 2105         	movs	r1, #0x5
    9186: f8df 2acc    	ldr.w	r2, [pc, #0xacc]        @ 0x9c54 <gst_lg_audio_render_event+0x1a14>
    918a: 58fb         	ldr	r3, [r7, r3]
    918c: 447a         	add	r2, pc
    918e: f8cd b00c    	str.w	r11, [sp, #0xc]
    9192: 9401         	str	r4, [sp, #0x4]
    9194: 6818         	ldr	r0, [r3]
    9196: f8df 3ac0    	ldr.w	r3, [pc, #0xac0]        @ 0x9c58 <gst_lg_audio_render_event+0x1a18>
    919a: 9202         	str	r2, [sp, #0x8]
    919c: f240 42a5    	movw	r2, #0x4a5
    91a0: 9200         	str	r2, [sp]
    91a2: f8df 2ab8    	ldr.w	r2, [pc, #0xab8]        @ 0x9c5c <gst_lg_audio_render_event+0x1a1c>
    91a6: 447b         	add	r3, pc
    91a8: f503 73c6    	add.w	r3, r3, #0x18c
    91ac: 447a         	add	r2, pc
    91ae: f7fa ed5e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5544
    91b2: 4658         	mov	r0, r11
    91b4: f7fa ee3e    	blx	0x3e34 <gst_audio_format_from_string@plt> @ imm = #-0x5384
    91b8: f8df 3aa4    	ldr.w	r3, [pc, #0xaa4]        @ 0x9c60 <gst_lg_audio_render_event+0x1a20>
    91bc: 4601         	mov	r1, r0
    91be: 447b         	add	r3, pc
    91c0: 68da         	ldr	r2, [r3, #0xc]
    91c2: 2800         	cmp	r0, #0x0
    91c4: f001 81ac    	beq.w	0xa520 <gst_lg_audio_render_event+0x22e0> @ imm = #0x1358
    91c8: e9cd 2316    	strd	r2, r3, [sp, #88]
    91cc: f7fa ec88    	blx	0x3ae0 <gst_audio_format_get_info@plt> @ imm = #-0x56f0
    91d0: e9dd 2316    	ldrd	r2, r3, [sp, #88]
    91d4: 4683         	mov	r11, r0
    91d6: 2800         	cmp	r0, #0x0
    91d8: f002 819e    	beq.w	0xb518 <gst_lg_audio_render_event+0x32d8> @ imm = #0x233c
    91dc: 68c1         	ldr	r1, [r0, #0xc]
    91de: 07c9         	lsls	r1, r1, #0x1f
    91e0: f141 87a9    	bpl.w	0xb136 <gst_lg_audio_render_event+0x2ef6> @ imm = #0x1f52
    91e4: 6941         	ldr	r1, [r0, #0x14]
    91e6: f88d 10c9    	strb.w	r1, [sp, #0xc9]
    91ea: 6812         	ldr	r2, [r2]
    91ec: 06d0         	lsls	r0, r2, #0x1b
    91ee: f102 830e    	bmi.w	0xb80e <gst_lg_audio_render_event+0x35ce> @ imm = #0x261c
    91f2: f8d8 3000    	ldr.w	r3, [r8]
    91f6: 2b04         	cmp	r3, #0x4
    91f8: d919         	bls	0x922e <gst_lg_audio_render_event+0xfee> @ imm = #0x32
    91fa: f8df 3a14    	ldr.w	r3, [pc, #0xa14]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    91fe: 2105         	movs	r1, #0x5
    9200: f89d 20c9    	ldrb.w	r2, [sp, #0xc9]
    9204: 58fb         	ldr	r3, [r7, r3]
    9206: 9401         	str	r4, [sp, #0x4]
    9208: 6818         	ldr	r0, [r3]
    920a: 9203         	str	r2, [sp, #0xc]
    920c: f8df 2a54    	ldr.w	r2, [pc, #0xa54]        @ 0x9c64 <gst_lg_audio_render_event+0x1a24>
    9210: f8df 3a54    	ldr.w	r3, [pc, #0xa54]        @ 0x9c68 <gst_lg_audio_render_event+0x1a28>
    9214: 447a         	add	r2, pc
    9216: 9202         	str	r2, [sp, #0x8]
    9218: f240 42ae    	movw	r2, #0x4ae
    921c: 9200         	str	r2, [sp]
    921e: 447b         	add	r3, pc
    9220: f8df 2a48    	ldr.w	r2, [pc, #0xa48]        @ 0x9c6c <gst_lg_audio_render_event+0x1a2c>
    9224: f503 73c6    	add.w	r3, r3, #0x18c
    9228: 447a         	add	r2, pc
    922a: f7fa ed20    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x55c0
    922e: f8df 0a40    	ldr.w	r0, [pc, #0xa40]        @ 0x9c70 <gst_lg_audio_render_event+0x1a30>
    9232: f8db 300c    	ldr.w	r3, [r11, #0xc]
    9236: 4478         	add	r0, pc
    9238: 68c2         	ldr	r2, [r0, #0xc]
    923a: f083 0304    	eor	r3, r3, #0x4
    923e: f3c3 0380    	ubfx	r3, r3, #0x2, #0x1
    9242: 9334         	str	r3, [sp, #0xd0]
    9244: 6812         	ldr	r2, [r2]
    9246: 06d1         	lsls	r1, r2, #0x1b
    9248: f102 8301    	bmi.w	0xb84e <gst_lg_audio_render_event+0x360e> @ imm = #0x2602
    924c: f8d8 3000    	ldr.w	r3, [r8]
    9250: 2b04         	cmp	r3, #0x4
    9252: d918         	bls	0x9286 <gst_lg_audio_render_event+0x1046> @ imm = #0x30
    9254: f8df 39b8    	ldr.w	r3, [pc, #0x9b8]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9258: 2105         	movs	r1, #0x5
    925a: 9a34         	ldr	r2, [sp, #0xd0]
    925c: 58fb         	ldr	r3, [r7, r3]
    925e: 9401         	str	r4, [sp, #0x4]
    9260: 6818         	ldr	r0, [r3]
    9262: 9203         	str	r2, [sp, #0xc]
    9264: f8df 2a0c    	ldr.w	r2, [pc, #0xa0c]        @ 0x9c74 <gst_lg_audio_render_event+0x1a34>
    9268: f8df 3a0c    	ldr.w	r3, [pc, #0xa0c]        @ 0x9c78 <gst_lg_audio_render_event+0x1a38>
    926c: 447a         	add	r2, pc
    926e: 9202         	str	r2, [sp, #0x8]
    9270: f240 42b4    	movw	r2, #0x4b4
    9274: 9200         	str	r2, [sp]
    9276: 447b         	add	r3, pc
    9278: f8df 2a00    	ldr.w	r2, [pc, #0xa00]        @ 0x9c7c <gst_lg_audio_render_event+0x1a3c>
    927c: f503 73c6    	add.w	r3, r3, #0x18c
    9280: 447a         	add	r2, pc
    9282: f7fa ecf4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5618
    9286: f8df 09f8    	ldr.w	r0, [pc, #0x9f8]        @ 0x9c80 <gst_lg_audio_render_event+0x1a40>
    928a: f240 42d2    	movw	r2, #0x4d2
    928e: f8db 3010    	ldr.w	r3, [r11, #0x10]
    9292: 4478         	add	r0, pc
    9294: 1a9b         	subs	r3, r3, r2
    9296: 68c2         	ldr	r2, [r0, #0xc]
    9298: bf18         	it	ne
    929a: 2301         	movne	r3, #0x1
    929c: 9333         	str	r3, [sp, #0xcc]
    929e: 6812         	ldr	r2, [r2]
    92a0: 06d2         	lsls	r2, r2, #0x1b
    92a2: f102 82c5    	bmi.w	0xb830 <gst_lg_audio_render_event+0x35f0> @ imm = #0x258a
    92a6: f8d8 3000    	ldr.w	r3, [r8]
    92aa: 2b04         	cmp	r3, #0x4
    92ac: d918         	bls	0x92e0 <gst_lg_audio_render_event+0x10a0> @ imm = #0x30
    92ae: f8df 3960    	ldr.w	r3, [pc, #0x960]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    92b2: 2105         	movs	r1, #0x5
    92b4: 9a33         	ldr	r2, [sp, #0xcc]
    92b6: 58fb         	ldr	r3, [r7, r3]
    92b8: 9401         	str	r4, [sp, #0x4]
    92ba: 6818         	ldr	r0, [r3]
    92bc: 9203         	str	r2, [sp, #0xc]
    92be: f8df 29c4    	ldr.w	r2, [pc, #0x9c4]        @ 0x9c84 <gst_lg_audio_render_event+0x1a44>
    92c2: f8df 39c4    	ldr.w	r3, [pc, #0x9c4]        @ 0x9c88 <gst_lg_audio_render_event+0x1a48>
    92c6: 447a         	add	r2, pc
    92c8: 9202         	str	r2, [sp, #0x8]
    92ca: f240 42ba    	movw	r2, #0x4ba
    92ce: 9200         	str	r2, [sp]
    92d0: 447b         	add	r3, pc
    92d2: f8df 29b8    	ldr.w	r2, [pc, #0x9b8]        @ 0x9c8c <gst_lg_audio_render_event+0x1a4c>
    92d6: f503 73c6    	add.w	r3, r3, #0x18c
    92da: 447a         	add	r2, pc
    92dc: f7fa ecc6    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5674
    92e0: f8df 19ac    	ldr.w	r1, [pc, #0x9ac]        @ 0x9c90 <gst_lg_audio_render_event+0x1a50>
    92e4: aa1b         	add	r2, sp, #0x6c
    92e6: 4648         	mov	r0, r9
    92e8: 4479         	add	r1, pc
    92ea: f7fa ebe8    	blx	0x3abc <gst_structure_get_uint@plt> @ imm = #-0x5830
    92ee: 2800         	cmp	r0, #0x0
    92f0: f001 8530    	beq.w	0xad54 <gst_lg_audio_render_event+0x2b14> @ imm = #0x1a60
    92f4: f8df 099c    	ldr.w	r0, [pc, #0x99c]        @ 0x9c94 <gst_lg_audio_render_event+0x1a54>
    92f8: 9b1b         	ldr	r3, [sp, #0x6c]
    92fa: 4478         	add	r0, pc
    92fc: 68c2         	ldr	r2, [r0, #0xc]
    92fe: 9335         	str	r3, [sp, #0xd4]
    9300: 6812         	ldr	r2, [r2]
    9302: 06d1         	lsls	r1, r2, #0x1b
    9304: f102 800c    	bmi.w	0xb320 <gst_lg_audio_render_event+0x30e0> @ imm = #0x2018
    9308: f8d8 3000    	ldr.w	r3, [r8]
    930c: 2b04         	cmp	r3, #0x4
    930e: d918         	bls	0x9342 <gst_lg_audio_render_event+0x1102> @ imm = #0x30
    9310: f8df 38fc    	ldr.w	r3, [pc, #0x8fc]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9314: 2105         	movs	r1, #0x5
    9316: 9a35         	ldr	r2, [sp, #0xd4]
    9318: 58fb         	ldr	r3, [r7, r3]
    931a: 9401         	str	r4, [sp, #0x4]
    931c: 6818         	ldr	r0, [r3]
    931e: 9203         	str	r2, [sp, #0xc]
    9320: f8df 2974    	ldr.w	r2, [pc, #0x974]        @ 0x9c98 <gst_lg_audio_render_event+0x1a58>
    9324: f8df 3974    	ldr.w	r3, [pc, #0x974]        @ 0x9c9c <gst_lg_audio_render_event+0x1a5c>
    9328: 447a         	add	r2, pc
    932a: 9202         	str	r2, [sp, #0x8]
    932c: f240 42cc    	movw	r2, #0x4cc
    9330: 9200         	str	r2, [sp]
    9332: 447b         	add	r3, pc
    9334: f8df 2968    	ldr.w	r2, [pc, #0x968]        @ 0x9ca0 <gst_lg_audio_render_event+0x1a60>
    9338: f503 73c6    	add.w	r3, r3, #0x18c
    933c: 447a         	add	r2, pc
    933e: f7fa ec96    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x56d4
    9342: f8df 1960    	ldr.w	r1, [pc, #0x960]        @ 0x9ca4 <gst_lg_audio_render_event+0x1a64>
    9346: aa1c         	add	r2, sp, #0x70
    9348: 4648         	mov	r0, r9
    934a: 4479         	add	r1, pc
    934c: f7fa ebb6    	blx	0x3abc <gst_structure_get_uint@plt> @ imm = #-0x5894
    9350: 2800         	cmp	r0, #0x0
    9352: f001 8107    	beq.w	0xa564 <gst_lg_audio_render_event+0x2324> @ imm = #0x120e
    9356: f8df 0950    	ldr.w	r0, [pc, #0x950]        @ 0x9ca8 <gst_lg_audio_render_event+0x1a68>
    935a: 9b1c         	ldr	r3, [sp, #0x70]
    935c: 4478         	add	r0, pc
    935e: 68c2         	ldr	r2, [r0, #0xc]
    9360: 9336         	str	r3, [sp, #0xd8]
    9362: 6812         	ldr	r2, [r2]
    9364: 06d2         	lsls	r2, r2, #0x1b
    9366: f101 8778    	bmi.w	0xb25a <gst_lg_audio_render_event+0x301a> @ imm = #0x1ef0
    936a: f8d8 3000    	ldr.w	r3, [r8]
    936e: 2b04         	cmp	r3, #0x4
    9370: d918         	bls	0x93a4 <gst_lg_audio_render_event+0x1164> @ imm = #0x30
    9372: f8df 389c    	ldr.w	r3, [pc, #0x89c]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9376: 2105         	movs	r1, #0x5
    9378: 9a36         	ldr	r2, [sp, #0xd8]
    937a: 58fb         	ldr	r3, [r7, r3]
    937c: 9401         	str	r4, [sp, #0x4]
    937e: 6818         	ldr	r0, [r3]
    9380: 9203         	str	r2, [sp, #0xc]
    9382: f8df 2928    	ldr.w	r2, [pc, #0x928]        @ 0x9cac <gst_lg_audio_render_event+0x1a6c>
    9386: f8df 3928    	ldr.w	r3, [pc, #0x928]        @ 0x9cb0 <gst_lg_audio_render_event+0x1a70>
    938a: 447a         	add	r2, pc
    938c: 9202         	str	r2, [sp, #0x8]
    938e: f240 42e7    	movw	r2, #0x4e7
    9392: 9200         	str	r2, [sp]
    9394: 447b         	add	r3, pc
    9396: f8df 291c    	ldr.w	r2, [pc, #0x91c]        @ 0x9cb4 <gst_lg_audio_render_event+0x1a74>
    939a: f503 73c6    	add.w	r3, r3, #0x18c
    939e: 447a         	add	r2, pc
    93a0: f7fa ec64    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5738
    93a4: f8df 1910    	ldr.w	r1, [pc, #0x910]        @ 0x9cb8 <gst_lg_audio_render_event+0x1a78>
    93a8: aa1d         	add	r2, sp, #0x74
    93aa: 4648         	mov	r0, r9
    93ac: 4479         	add	r1, pc
    93ae: f7fa eeae    	blx	0x410c <gst_structure_get_boolean@plt> @ imm = #-0x52a4
    93b2: 2800         	cmp	r0, #0x0
    93b4: f001 8066    	beq.w	0xa484 <gst_lg_audio_render_event+0x2244> @ imm = #0x10cc
    93b8: f8df 0900    	ldr.w	r0, [pc, #0x900]        @ 0x9cbc <gst_lg_audio_render_event+0x1a7c>
    93bc: 9b1d         	ldr	r3, [sp, #0x74]
    93be: 4478         	add	r0, pc
    93c0: 68c2         	ldr	r2, [r0, #0xc]
    93c2: 9337         	str	r3, [sp, #0xdc]
    93c4: 6812         	ldr	r2, [r2]
    93c6: 06d2         	lsls	r2, r2, #0x1b
    93c8: f101 8778    	bmi.w	0xb2bc <gst_lg_audio_render_event+0x307c> @ imm = #0x1ef0
    93cc: f8d8 3000    	ldr.w	r3, [r8]
    93d0: 2b04         	cmp	r3, #0x4
    93d2: d918         	bls	0x9406 <gst_lg_audio_render_event+0x11c6> @ imm = #0x30
    93d4: f8df 3838    	ldr.w	r3, [pc, #0x838]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    93d8: 2105         	movs	r1, #0x5
    93da: 9a37         	ldr	r2, [sp, #0xdc]
    93dc: 58fb         	ldr	r3, [r7, r3]
    93de: 9401         	str	r4, [sp, #0x4]
    93e0: 6818         	ldr	r0, [r3]
    93e2: 9203         	str	r2, [sp, #0xc]
    93e4: f8df 28d8    	ldr.w	r2, [pc, #0x8d8]        @ 0x9cc0 <gst_lg_audio_render_event+0x1a80>
    93e8: f8df 38d8    	ldr.w	r3, [pc, #0x8d8]        @ 0x9cc4 <gst_lg_audio_render_event+0x1a84>
    93ec: 447a         	add	r2, pc
    93ee: 9202         	str	r2, [sp, #0x8]
    93f0: f240 42ef    	movw	r2, #0x4ef
    93f4: 9200         	str	r2, [sp]
    93f6: 447b         	add	r3, pc
    93f8: f8df 28cc    	ldr.w	r2, [pc, #0x8cc]        @ 0x9cc8 <gst_lg_audio_render_event+0x1a88>
    93fc: f503 73c6    	add.w	r3, r3, #0x18c
    9400: 447a         	add	r2, pc
    9402: f7fa ec34    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5798
    9406: f8df 18c4    	ldr.w	r1, [pc, #0x8c4]        @ 0x9ccc <gst_lg_audio_render_event+0x1a8c>
    940a: aa1e         	add	r2, sp, #0x78
    940c: 4648         	mov	r0, r9
    940e: 4479         	add	r1, pc
    9410: f7fa ee7c    	blx	0x410c <gst_structure_get_boolean@plt> @ imm = #-0x5308
    9414: 2800         	cmp	r0, #0x0
    9416: f001 800e    	beq.w	0xa436 <gst_lg_audio_render_event+0x21f6> @ imm = #0x101c
    941a: f8df 08b4    	ldr.w	r0, [pc, #0x8b4]        @ 0x9cd0 <gst_lg_audio_render_event+0x1a90>
    941e: 9b1e         	ldr	r3, [sp, #0x78]
    9420: 4478         	add	r0, pc
    9422: 68c2         	ldr	r2, [r0, #0xc]
    9424: 9338         	str	r3, [sp, #0xe0]
    9426: 6812         	ldr	r2, [r2]
    9428: 06d2         	lsls	r2, r2, #0x1b
    942a: f101 8768    	bmi.w	0xb2fe <gst_lg_audio_render_event+0x30be> @ imm = #0x1ed0
    942e: f8d8 3000    	ldr.w	r3, [r8]
    9432: 2b04         	cmp	r3, #0x4
    9434: d918         	bls	0x9468 <gst_lg_audio_render_event+0x1228> @ imm = #0x30
    9436: f8df 37d8    	ldr.w	r3, [pc, #0x7d8]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    943a: 2105         	movs	r1, #0x5
    943c: 9a38         	ldr	r2, [sp, #0xe0]
    943e: 58fb         	ldr	r3, [r7, r3]
    9440: 9401         	str	r4, [sp, #0x4]
    9442: 6818         	ldr	r0, [r3]
    9444: 9203         	str	r2, [sp, #0xc]
    9446: f8df 288c    	ldr.w	r2, [pc, #0x88c]        @ 0x9cd4 <gst_lg_audio_render_event+0x1a94>
    944a: f8df 388c    	ldr.w	r3, [pc, #0x88c]        @ 0x9cd8 <gst_lg_audio_render_event+0x1a98>
    944e: 447a         	add	r2, pc
    9450: 9202         	str	r2, [sp, #0x8]
    9452: f240 42f7    	movw	r2, #0x4f7
    9456: 9200         	str	r2, [sp]
    9458: 447b         	add	r3, pc
    945a: f8df 2880    	ldr.w	r2, [pc, #0x880]        @ 0x9cdc <gst_lg_audio_render_event+0x1a9c>
    945e: f503 73c6    	add.w	r3, r3, #0x18c
    9462: 447a         	add	r2, pc
    9464: f7fa ec02    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x57fc
    9468: f8df 1874    	ldr.w	r1, [pc, #0x874]        @ 0x9ce0 <gst_lg_audio_render_event+0x1aa0>
    946c: aa1f         	add	r2, sp, #0x7c
    946e: 4648         	mov	r0, r9
    9470: 4479         	add	r1, pc
    9472: f7fa ee4c    	blx	0x410c <gst_structure_get_boolean@plt> @ imm = #-0x5368
    9476: 2800         	cmp	r0, #0x0
    9478: f001 802b    	beq.w	0xa4d2 <gst_lg_audio_render_event+0x2292> @ imm = #0x1056
    947c: f8df 0864    	ldr.w	r0, [pc, #0x864]        @ 0x9ce4 <gst_lg_audio_render_event+0x1aa4>
    9480: 9b1f         	ldr	r3, [sp, #0x7c]
    9482: 4478         	add	r0, pc
    9484: 68c2         	ldr	r2, [r0, #0xc]
    9486: 9339         	str	r3, [sp, #0xe4]
    9488: 6812         	ldr	r2, [r2]
    948a: 06d2         	lsls	r2, r2, #0x1b
    948c: f101 8685    	bmi.w	0xb19a <gst_lg_audio_render_event+0x2f5a> @ imm = #0x1d0a
    9490: f8d8 3000    	ldr.w	r3, [r8]
    9494: 2b04         	cmp	r3, #0x4
    9496: d918         	bls	0x94ca <gst_lg_audio_render_event+0x128a> @ imm = #0x30
    9498: f8df 3774    	ldr.w	r3, [pc, #0x774]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    949c: 2105         	movs	r1, #0x5
    949e: 9a39         	ldr	r2, [sp, #0xe4]
    94a0: 58fb         	ldr	r3, [r7, r3]
    94a2: 9401         	str	r4, [sp, #0x4]
    94a4: 6818         	ldr	r0, [r3]
    94a6: 9203         	str	r2, [sp, #0xc]
    94a8: f8df 283c    	ldr.w	r2, [pc, #0x83c]        @ 0x9ce8 <gst_lg_audio_render_event+0x1aa8>
    94ac: f8df 383c    	ldr.w	r3, [pc, #0x83c]        @ 0x9cec <gst_lg_audio_render_event+0x1aac>
    94b0: 447a         	add	r2, pc
    94b2: 9202         	str	r2, [sp, #0x8]
    94b4: f240 42ff    	movw	r2, #0x4ff
    94b8: 9200         	str	r2, [sp]
    94ba: 447b         	add	r3, pc
    94bc: f8df 2830    	ldr.w	r2, [pc, #0x830]        @ 0x9cf0 <gst_lg_audio_render_event+0x1ab0>
    94c0: f503 73c6    	add.w	r3, r3, #0x18c
    94c4: 447a         	add	r2, pc
    94c6: f7fa ebd2    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x585c
    94ca: f8df 0828    	ldr.w	r0, [pc, #0x828]        @ 0x9cf4 <gst_lg_audio_render_event+0x1ab4>
    94ce: 4478         	add	r0, pc
    94d0: f7fa ea0a    	blx	0x38e8 <gst_structure_new_empty@plt> @ imm = #-0x5bec
    94d4: 4682         	mov	r10, r0
    94d6: f8d4 00b8    	ldr.w	r0, [r4, #0xb8]
    94da: f7fa eca6    	blx	0x3e28 <gst_pad_get_stream_id@plt> @ imm = #-0x56b4
    94de: 9016         	str	r0, [sp, #0x58]
    94e0: b370         	cbz	r0, 0x9540 <gst_lg_audio_render_event+0x1300> @ imm = #0x5c
    94e2: f8df 1814    	ldr.w	r1, [pc, #0x814]        @ 0x9cf8 <gst_lg_audio_render_event+0x1ab8>
    94e6: 2300         	movs	r3, #0x0
    94e8: 9300         	str	r3, [sp]
    94ea: 2240         	movs	r2, #0x40
    94ec: 4603         	mov	r3, r0
    94ee: 4650         	mov	r0, r10
    94f0: 4479         	add	r1, pc
    94f2: f7fa ef16    	blx	0x4320 <gst_structure_set@plt> @ imm = #-0x51d4
    94f6: f8df 0804    	ldr.w	r0, [pc, #0x804]        @ 0x9cfc <gst_lg_audio_render_event+0x1abc>
    94fa: 4478         	add	r0, pc
    94fc: 68c3         	ldr	r3, [r0, #0xc]
    94fe: 681b         	ldr	r3, [r3]
    9500: 0719         	lsls	r1, r3, #0x1c
    9502: f101 85f3    	bmi.w	0xb0ec <gst_lg_audio_render_event+0x2eac> @ imm = #0x1be6
    9506: f8d8 3000    	ldr.w	r3, [r8]
    950a: 2b03         	cmp	r3, #0x3
    950c: d918         	bls	0x9540 <gst_lg_audio_render_event+0x1300> @ imm = #0x30
    950e: 9a16         	ldr	r2, [sp, #0x58]
    9510: 2104         	movs	r1, #0x4
    9512: f8df 36fc    	ldr.w	r3, [pc, #0x6fc]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9516: 58fb         	ldr	r3, [r7, r3]
    9518: 9203         	str	r2, [sp, #0xc]
    951a: f8df 27e4    	ldr.w	r2, [pc, #0x7e4]        @ 0x9d00 <gst_lg_audio_render_event+0x1ac0>
    951e: 9401         	str	r4, [sp, #0x4]
    9520: 6818         	ldr	r0, [r3]
    9522: 447a         	add	r2, pc
    9524: f8df 37dc    	ldr.w	r3, [pc, #0x7dc]        @ 0x9d04 <gst_lg_audio_render_event+0x1ac4>
    9528: 9202         	str	r2, [sp, #0x8]
    952a: f240 520b    	movw	r2, #0x50b
    952e: 9200         	str	r2, [sp]
    9530: f8df 27d4    	ldr.w	r2, [pc, #0x7d4]        @ 0x9d08 <gst_lg_audio_render_event+0x1ac8>
    9534: 447b         	add	r3, pc
    9536: f503 73c6    	add.w	r3, r3, #0x18c
    953a: 447a         	add	r2, pc
    953c: f7fa eb96    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x58d4
    9540: f8df 17c8    	ldr.w	r1, [pc, #0x7c8]        @ 0x9d0c <gst_lg_audio_render_event+0x1acc>
    9544: aa20         	add	r2, sp, #0x80
    9546: 4648         	mov	r0, r9
    9548: 4479         	add	r1, pc
    954a: f7fa ede0    	blx	0x410c <gst_structure_get_boolean@plt> @ imm = #-0x5440
    954e: 2800         	cmp	r0, #0x0
    9550: f000 873f    	beq.w	0xa3d2 <gst_lg_audio_render_event+0x2192> @ imm = #0xe7e
    9554: f8df 07b8    	ldr.w	r0, [pc, #0x7b8]        @ 0x9d10 <gst_lg_audio_render_event+0x1ad0>
    9558: 9b20         	ldr	r3, [sp, #0x80]
    955a: 4478         	add	r0, pc
    955c: 68c2         	ldr	r2, [r0, #0xc]
    955e: 933a         	str	r3, [sp, #0xe8]
    9560: 6812         	ldr	r2, [r2]
    9562: 0712         	lsls	r2, r2, #0x1c
    9564: f101 863a    	bmi.w	0xb1dc <gst_lg_audio_render_event+0x2f9c> @ imm = #0x1c74
    9568: f8d8 3000    	ldr.w	r3, [r8]
    956c: 2b03         	cmp	r3, #0x3
    956e: d918         	bls	0x95a2 <gst_lg_audio_render_event+0x1362> @ imm = #0x30
    9570: f8df 369c    	ldr.w	r3, [pc, #0x69c]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9574: 2104         	movs	r1, #0x4
    9576: 9a3a         	ldr	r2, [sp, #0xe8]
    9578: 58fb         	ldr	r3, [r7, r3]
    957a: 9401         	str	r4, [sp, #0x4]
    957c: 6818         	ldr	r0, [r3]
    957e: 9203         	str	r2, [sp, #0xc]
    9580: f8df 2790    	ldr.w	r2, [pc, #0x790]        @ 0x9d14 <gst_lg_audio_render_event+0x1ad4>
    9584: f8df 3790    	ldr.w	r3, [pc, #0x790]        @ 0x9d18 <gst_lg_audio_render_event+0x1ad8>
    9588: 447a         	add	r2, pc
    958a: 9202         	str	r2, [sp, #0x8]
    958c: f44f 62a2    	mov.w	r2, #0x510
    9590: 9200         	str	r2, [sp]
    9592: 447b         	add	r3, pc
    9594: f8df 2784    	ldr.w	r2, [pc, #0x784]        @ 0x9d1c <gst_lg_audio_render_event+0x1adc>
    9598: f503 73c6    	add.w	r3, r3, #0x18c
    959c: 447a         	add	r2, pc
    959e: f7fa eb66    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5934
    95a2: 2300         	movs	r3, #0x0
    95a4: f8df 1778    	ldr.w	r1, [pc, #0x778]        @ 0x9d20 <gst_lg_audio_render_event+0x1ae0>
    95a8: 9300         	str	r3, [sp]
    95aa: 2240         	movs	r2, #0x40
    95ac: f8df 3774    	ldr.w	r3, [pc, #0x774]        @ 0x9d24 <gst_lg_audio_render_event+0x1ae4>
    95b0: 4650         	mov	r0, r10
    95b2: 4479         	add	r1, pc
    95b4: 447b         	add	r3, pc
    95b6: f7fa eeb4    	blx	0x4320 <gst_structure_set@plt> @ imm = #-0x5298
    95ba: f1ba 0f00    	cmp.w	r10, #0x0
    95be: d035         	beq	0x962c <gst_lg_audio_render_event+0x13ec> @ imm = #0x6a
    95c0: 4650         	mov	r0, r10
    95c2: f7fa ec80    	blx	0x3ec4 <gst_structure_to_string@plt> @ imm = #-0x5700
    95c6: 4683         	mov	r11, r0
    95c8: b1f8         	cbz	r0, 0x960a <gst_lg_audio_render_event+0x13ca> @ imm = #0x3e
    95ca: f8d8 3000    	ldr.w	r3, [r8]
    95ce: 2b03         	cmp	r3, #0x3
    95d0: d918         	bls	0x9604 <gst_lg_audio_render_event+0x13c4> @ imm = #0x30
    95d2: f8df 363c    	ldr.w	r3, [pc, #0x63c]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    95d6: 2100         	movs	r1, #0x0
    95d8: f8df 274c    	ldr.w	r2, [pc, #0x74c]        @ 0x9d28 <gst_lg_audio_render_event+0x1ae8>
    95dc: 58fb         	ldr	r3, [r7, r3]
    95de: 447a         	add	r2, pc
    95e0: 9003         	str	r0, [sp, #0xc]
    95e2: 6818         	ldr	r0, [r3]
    95e4: f8df 3744    	ldr.w	r3, [pc, #0x744]        @ 0x9d2c <gst_lg_audio_render_event+0x1aec>
    95e8: 9202         	str	r2, [sp, #0x8]
    95ea: f240 521e    	movw	r2, #0x51e
    95ee: e9cd 2100    	strd	r2, r1, [sp]
    95f2: 2104         	movs	r1, #0x4
    95f4: f8df 2738    	ldr.w	r2, [pc, #0x738]        @ 0x9d30 <gst_lg_audio_render_event+0x1af0>
    95f8: 447b         	add	r3, pc
    95fa: f503 73c6    	add.w	r3, r3, #0x18c
    95fe: 447a         	add	r2, pc
    9600: f7fa eb34    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5998
    9604: 4658         	mov	r0, r11
    9606: f7fa e8bc    	blx	0x3780 <g_free@plt>     @ imm = #-0x5e88
    960a: f7fa ecc4    	blx	0x3f94 <gst_element_get_type@plt> @ imm = #-0x5678
    960e: 4601         	mov	r1, r0
    9610: 4620         	mov	r0, r4
    9612: f7fa ecba    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x568c
    9616: 4621         	mov	r1, r4
    9618: 4683         	mov	r11, r0
    961a: 4652         	mov	r2, r10
    961c: f44f 4080    	mov.w	r0, #0x4000
    9620: f7fa e9ce    	blx	0x39c0 <gst_message_new_custom@plt> @ imm = #-0x5c64
    9624: 4601         	mov	r1, r0
    9626: 4658         	mov	r0, r11
    9628: f7fa ec9a    	blx	0x3f60 <gst_element_post_message@plt> @ imm = #-0x56cc
    962c: f8df 1704    	ldr.w	r1, [pc, #0x704]        @ 0x9d34 <gst_lg_audio_render_event+0x1af4>
    9630: aa22         	add	r2, sp, #0x88
    9632: 4648         	mov	r0, r9
    9634: 4479         	add	r1, pc
    9636: f7fa e9e8    	blx	0x3a08 <gst_structure_get_int@plt> @ imm = #-0x5c30
    963a: 2800         	cmp	r0, #0x0
    963c: f001 8236    	beq.w	0xaaac <gst_lg_audio_render_event+0x286c> @ imm = #0x146c
    9640: f8df 06f4    	ldr.w	r0, [pc, #0x6f4]        @ 0x9d38 <gst_lg_audio_render_event+0x1af8>
    9644: 9b22         	ldr	r3, [sp, #0x88]
    9646: 4478         	add	r0, pc
    9648: 68c2         	ldr	r2, [r0, #0xc]
    964a: 933b         	str	r3, [sp, #0xec]
    964c: 6812         	ldr	r2, [r2]
    964e: 06d2         	lsls	r2, r2, #0x1b
    9650: f101 86ed    	bmi.w	0xb42e <gst_lg_audio_render_event+0x31ee> @ imm = #0x1dda
    9654: f8d8 3000    	ldr.w	r3, [r8]
    9658: 2b04         	cmp	r3, #0x4
    965a: d918         	bls	0x968e <gst_lg_audio_render_event+0x144e> @ imm = #0x30
    965c: f8df 35b0    	ldr.w	r3, [pc, #0x5b0]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9660: 2105         	movs	r1, #0x5
    9662: 9a22         	ldr	r2, [sp, #0x88]
    9664: 58fb         	ldr	r3, [r7, r3]
    9666: 9401         	str	r4, [sp, #0x4]
    9668: 6818         	ldr	r0, [r3]
    966a: 9203         	str	r2, [sp, #0xc]
    966c: f8df 26cc    	ldr.w	r2, [pc, #0x6cc]        @ 0x9d3c <gst_lg_audio_render_event+0x1afc>
    9670: f8df 36cc    	ldr.w	r3, [pc, #0x6cc]        @ 0x9d40 <gst_lg_audio_render_event+0x1b00>
    9674: 447a         	add	r2, pc
    9676: 9202         	str	r2, [sp, #0x8]
    9678: f240 5229    	movw	r2, #0x529
    967c: 9200         	str	r2, [sp]
    967e: 447b         	add	r3, pc
    9680: f8df 26c0    	ldr.w	r2, [pc, #0x6c0]        @ 0x9d44 <gst_lg_audio_render_event+0x1b04>
    9684: f503 73c6    	add.w	r3, r3, #0x18c
    9688: 447a         	add	r2, pc
    968a: f7fa eaf0    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5a20
    968e: f8df 16b8    	ldr.w	r1, [pc, #0x6b8]        @ 0x9d48 <gst_lg_audio_render_event+0x1b08>
    9692: aa23         	add	r2, sp, #0x8c
    9694: 4648         	mov	r0, r9
    9696: 4479         	add	r1, pc
    9698: f7fa ed38    	blx	0x410c <gst_structure_get_boolean@plt> @ imm = #-0x5590
    969c: 2800         	cmp	r0, #0x0
    969e: f001 81e3    	beq.w	0xaa68 <gst_lg_audio_render_event+0x2828> @ imm = #0x13c6
    96a2: f8df 06a8    	ldr.w	r0, [pc, #0x6a8]        @ 0x9d4c <gst_lg_audio_render_event+0x1b0c>
    96a6: 9b23         	ldr	r3, [sp, #0x8c]
    96a8: 4478         	add	r0, pc
    96aa: 68c2         	ldr	r2, [r0, #0xc]
    96ac: 933c         	str	r3, [sp, #0xf0]
    96ae: 6812         	ldr	r2, [r2]
    96b0: 06d2         	lsls	r2, r2, #0x1b
    96b2: f101 870f    	bmi.w	0xb4d4 <gst_lg_audio_render_event+0x3294> @ imm = #0x1e1e
    96b6: f8d8 3000    	ldr.w	r3, [r8]
    96ba: 2b04         	cmp	r3, #0x4
    96bc: d918         	bls	0x96f0 <gst_lg_audio_render_event+0x14b0> @ imm = #0x30
    96be: f8df 3550    	ldr.w	r3, [pc, #0x550]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    96c2: 2105         	movs	r1, #0x5
    96c4: 9a23         	ldr	r2, [sp, #0x8c]
    96c6: 58fb         	ldr	r3, [r7, r3]
    96c8: 9401         	str	r4, [sp, #0x4]
    96ca: 6818         	ldr	r0, [r3]
    96cc: 9203         	str	r2, [sp, #0xc]
    96ce: f8df 2680    	ldr.w	r2, [pc, #0x680]        @ 0x9d50 <gst_lg_audio_render_event+0x1b10>
    96d2: f8df 3680    	ldr.w	r3, [pc, #0x680]        @ 0x9d54 <gst_lg_audio_render_event+0x1b14>
    96d6: 447a         	add	r2, pc
    96d8: 9202         	str	r2, [sp, #0x8]
    96da: f240 5231    	movw	r2, #0x531
    96de: 9200         	str	r2, [sp]
    96e0: 447b         	add	r3, pc
    96e2: f8df 2674    	ldr.w	r2, [pc, #0x674]        @ 0x9d58 <gst_lg_audio_render_event+0x1b18>
    96e6: f503 73c6    	add.w	r3, r3, #0x18c
    96ea: 447a         	add	r2, pc
    96ec: f7fa eabe    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5a84
    96f0: f8df 1668    	ldr.w	r1, [pc, #0x668]        @ 0x9d5c <gst_lg_audio_render_event+0x1b1c>
    96f4: aa24         	add	r2, sp, #0x90
    96f6: 4648         	mov	r0, r9
    96f8: 4479         	add	r1, pc
    96fa: f7fa e986    	blx	0x3a08 <gst_structure_get_int@plt> @ imm = #-0x5cf4
    96fe: b330         	cbz	r0, 0x974e <gst_lg_audio_render_event+0x150e> @ imm = #0x4c
    9700: f8df 065c    	ldr.w	r0, [pc, #0x65c]        @ 0x9d60 <gst_lg_audio_render_event+0x1b20>
    9704: 9b24         	ldr	r3, [sp, #0x90]
    9706: 4478         	add	r0, pc
    9708: 68c2         	ldr	r2, [r0, #0xc]
    970a: 933d         	str	r3, [sp, #0xf4]
    970c: 6812         	ldr	r2, [r2]
    970e: 0711         	lsls	r1, r2, #0x1c
    9710: f101 86f1    	bmi.w	0xb4f6 <gst_lg_audio_render_event+0x32b6> @ imm = #0x1de2
    9714: f8d8 3000    	ldr.w	r3, [r8]
    9718: 2b03         	cmp	r3, #0x3
    971a: d918         	bls	0x974e <gst_lg_audio_render_event+0x150e> @ imm = #0x30
    971c: f8df 34f0    	ldr.w	r3, [pc, #0x4f0]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9720: 2104         	movs	r1, #0x4
    9722: 9a24         	ldr	r2, [sp, #0x90]
    9724: 58fb         	ldr	r3, [r7, r3]
    9726: 9401         	str	r4, [sp, #0x4]
    9728: 6818         	ldr	r0, [r3]
    972a: 9203         	str	r2, [sp, #0xc]
    972c: f8df 2634    	ldr.w	r2, [pc, #0x634]        @ 0x9d64 <gst_lg_audio_render_event+0x1b24>
    9730: f8df 3634    	ldr.w	r3, [pc, #0x634]        @ 0x9d68 <gst_lg_audio_render_event+0x1b28>
    9734: 447a         	add	r2, pc
    9736: 9202         	str	r2, [sp, #0x8]
    9738: f44f 62a9    	mov.w	r2, #0x548
    973c: 9200         	str	r2, [sp]
    973e: 447b         	add	r3, pc
    9740: f8df 2628    	ldr.w	r2, [pc, #0x628]        @ 0x9d6c <gst_lg_audio_render_event+0x1b2c>
    9744: f503 73c6    	add.w	r3, r3, #0x18c
    9748: 447a         	add	r2, pc
    974a: f7fa ea90    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5ae0
    974e: f8df 1620    	ldr.w	r1, [pc, #0x620]        @ 0x9d70 <gst_lg_audio_render_event+0x1b30>
    9752: aa2d         	add	r2, sp, #0xb4
    9754: 4648         	mov	r0, r9
    9756: 4479         	add	r1, pc
    9758: f7fa e9b0    	blx	0x3abc <gst_structure_get_uint@plt> @ imm = #-0x5ca0
    975c: 2800         	cmp	r0, #0x0
    975e: f001 8161    	beq.w	0xaa24 <gst_lg_audio_render_event+0x27e4> @ imm = #0x12c2
    9762: f8df 0610    	ldr.w	r0, [pc, #0x610]        @ 0x9d74 <gst_lg_audio_render_event+0x1b34>
    9766: 9b2d         	ldr	r3, [sp, #0xb4]
    9768: 4478         	add	r0, pc
    976a: 68c2         	ldr	r2, [r0, #0xc]
    976c: 9330         	str	r3, [sp, #0xc0]
    976e: 6812         	ldr	r2, [r2]
    9770: 06d2         	lsls	r2, r2, #0x1b
    9772: f101 867d    	bmi.w	0xb470 <gst_lg_audio_render_event+0x3230> @ imm = #0x1cfa
    9776: f8d8 3000    	ldr.w	r3, [r8]
    977a: 2b04         	cmp	r3, #0x4
    977c: d918         	bls	0x97b0 <gst_lg_audio_render_event+0x1570> @ imm = #0x30
    977e: f8df 3490    	ldr.w	r3, [pc, #0x490]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9782: 2105         	movs	r1, #0x5
    9784: 9a2d         	ldr	r2, [sp, #0xb4]
    9786: 58fb         	ldr	r3, [r7, r3]
    9788: 9401         	str	r4, [sp, #0x4]
    978a: 6818         	ldr	r0, [r3]
    978c: 9203         	str	r2, [sp, #0xc]
    978e: f8df 25e8    	ldr.w	r2, [pc, #0x5e8]        @ 0x9d78 <gst_lg_audio_render_event+0x1b38>
    9792: f8df 35e8    	ldr.w	r3, [pc, #0x5e8]        @ 0x9d7c <gst_lg_audio_render_event+0x1b3c>
    9796: 447a         	add	r2, pc
    9798: 9202         	str	r2, [sp, #0x8]
    979a: f240 524d    	movw	r2, #0x54d
    979e: 9200         	str	r2, [sp]
    97a0: 447b         	add	r3, pc
    97a2: f8df 25dc    	ldr.w	r2, [pc, #0x5dc]        @ 0x9d80 <gst_lg_audio_render_event+0x1b40>
    97a6: f503 73c6    	add.w	r3, r3, #0x18c
    97aa: 447a         	add	r2, pc
    97ac: f7fa ea5e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5b44
    97b0: f8df 15d0    	ldr.w	r1, [pc, #0x5d0]        @ 0x9d84 <gst_lg_audio_render_event+0x1b44>
    97b4: aa25         	add	r2, sp, #0x94
    97b6: 4648         	mov	r0, r9
    97b8: 4479         	add	r1, pc
    97ba: f7fa eca8    	blx	0x410c <gst_structure_get_boolean@plt> @ imm = #-0x56b0
    97be: b330         	cbz	r0, 0x980e <gst_lg_audio_render_event+0x15ce> @ imm = #0x4c
    97c0: f8df 05c4    	ldr.w	r0, [pc, #0x5c4]        @ 0x9d88 <gst_lg_audio_render_event+0x1b48>
    97c4: 9b25         	ldr	r3, [sp, #0x94]
    97c6: 4478         	add	r0, pc
    97c8: 68c2         	ldr	r2, [r0, #0xc]
    97ca: 933e         	str	r3, [sp, #0xf8]
    97cc: 6812         	ldr	r2, [r2]
    97ce: 06d1         	lsls	r1, r2, #0x1b
    97d0: f101 85d9    	bmi.w	0xb386 <gst_lg_audio_render_event+0x3146> @ imm = #0x1bb2
    97d4: f8d8 3000    	ldr.w	r3, [r8]
    97d8: 2b04         	cmp	r3, #0x4
    97da: d918         	bls	0x980e <gst_lg_audio_render_event+0x15ce> @ imm = #0x30
    97dc: f8df 3430    	ldr.w	r3, [pc, #0x430]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    97e0: 2105         	movs	r1, #0x5
    97e2: 9a25         	ldr	r2, [sp, #0x94]
    97e4: 58fb         	ldr	r3, [r7, r3]
    97e6: 9401         	str	r4, [sp, #0x4]
    97e8: 6818         	ldr	r0, [r3]
    97ea: 9203         	str	r2, [sp, #0xc]
    97ec: f8df 259c    	ldr.w	r2, [pc, #0x59c]        @ 0x9d8c <gst_lg_audio_render_event+0x1b4c>
    97f0: f8df 359c    	ldr.w	r3, [pc, #0x59c]        @ 0x9d90 <gst_lg_audio_render_event+0x1b50>
    97f4: 447a         	add	r2, pc
    97f6: 9202         	str	r2, [sp, #0x8]
    97f8: f240 5255    	movw	r2, #0x555
    97fc: 9200         	str	r2, [sp]
    97fe: 447b         	add	r3, pc
    9800: f8df 2590    	ldr.w	r2, [pc, #0x590]        @ 0x9d94 <gst_lg_audio_render_event+0x1b54>
    9804: f503 73c6    	add.w	r3, r3, #0x18c
    9808: 447a         	add	r2, pc
    980a: f7fa ea30    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5ba0
    980e: f8df 1588    	ldr.w	r1, [pc, #0x588]        @ 0x9d98 <gst_lg_audio_render_event+0x1b58>
    9812: aa26         	add	r2, sp, #0x98
    9814: 4648         	mov	r0, r9
    9816: 4479         	add	r1, pc
    9818: f7fa e8f6    	blx	0x3a08 <gst_structure_get_int@plt> @ imm = #-0x5e14
    981c: b328         	cbz	r0, 0x986a <gst_lg_audio_render_event+0x162a> @ imm = #0x4a
    981e: f8df 057c    	ldr.w	r0, [pc, #0x57c]        @ 0x9d9c <gst_lg_audio_render_event+0x1b5c>
    9822: 9b26         	ldr	r3, [sp, #0x98]
    9824: 4478         	add	r0, pc
    9826: 68c2         	ldr	r2, [r0, #0xc]
    9828: 9344         	str	r3, [sp, #0x110]
    982a: 6812         	ldr	r2, [r2]
    982c: 06d2         	lsls	r2, r2, #0x1b
    982e: f101 85bb    	bmi.w	0xb3a8 <gst_lg_audio_render_event+0x3168> @ imm = #0x1b76
    9832: f8d8 3000    	ldr.w	r3, [r8]
    9836: 2b04         	cmp	r3, #0x4
    9838: d917         	bls	0x986a <gst_lg_audio_render_event+0x162a> @ imm = #0x2e
    983a: 4bf5         	ldr	r3, [pc, #0x3d4]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    983c: 2105         	movs	r1, #0x5
    983e: 9a26         	ldr	r2, [sp, #0x98]
    9840: 58fb         	ldr	r3, [r7, r3]
    9842: 9401         	str	r4, [sp, #0x4]
    9844: 6818         	ldr	r0, [r3]
    9846: 9203         	str	r2, [sp, #0xc]
    9848: f8df 2554    	ldr.w	r2, [pc, #0x554]        @ 0x9da0 <gst_lg_audio_render_event+0x1b60>
    984c: f8df 3554    	ldr.w	r3, [pc, #0x554]        @ 0x9da4 <gst_lg_audio_render_event+0x1b64>
    9850: 447a         	add	r2, pc
    9852: 9202         	str	r2, [sp, #0x8]
    9854: f240 525a    	movw	r2, #0x55a
    9858: 9200         	str	r2, [sp]
    985a: 447b         	add	r3, pc
    985c: f8df 2548    	ldr.w	r2, [pc, #0x548]        @ 0x9da8 <gst_lg_audio_render_event+0x1b68>
    9860: f503 73c6    	add.w	r3, r3, #0x18c
    9864: 447a         	add	r2, pc
    9866: f7fa ea02    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5bfc
    986a: f8df 1540    	ldr.w	r1, [pc, #0x540]        @ 0x9dac <gst_lg_audio_render_event+0x1b6c>
    986e: aa27         	add	r2, sp, #0x9c
    9870: 4648         	mov	r0, r9
    9872: 4479         	add	r1, pc
    9874: f7fa e8c8    	blx	0x3a08 <gst_structure_get_int@plt> @ imm = #-0x5e70
    9878: b328         	cbz	r0, 0x98c6 <gst_lg_audio_render_event+0x1686> @ imm = #0x4a
    987a: f8df 0534    	ldr.w	r0, [pc, #0x534]        @ 0x9db0 <gst_lg_audio_render_event+0x1b70>
    987e: 9b27         	ldr	r3, [sp, #0x9c]
    9880: 4478         	add	r0, pc
    9882: 68c2         	ldr	r2, [r0, #0xc]
    9884: 9345         	str	r3, [sp, #0x114]
    9886: 6812         	ldr	r2, [r2]
    9888: 06d1         	lsls	r1, r2, #0x1b
    988a: f101 859e    	bmi.w	0xb3ca <gst_lg_audio_render_event+0x318a> @ imm = #0x1b3c
    988e: f8d8 3000    	ldr.w	r3, [r8]
    9892: 2b04         	cmp	r3, #0x4
    9894: d917         	bls	0x98c6 <gst_lg_audio_render_event+0x1686> @ imm = #0x2e
    9896: 4bde         	ldr	r3, [pc, #0x378]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9898: 2105         	movs	r1, #0x5
    989a: 9a27         	ldr	r2, [sp, #0x9c]
    989c: 58fb         	ldr	r3, [r7, r3]
    989e: 9401         	str	r4, [sp, #0x4]
    98a0: 6818         	ldr	r0, [r3]
    98a2: 9203         	str	r2, [sp, #0xc]
    98a4: f8df 250c    	ldr.w	r2, [pc, #0x50c]        @ 0x9db4 <gst_lg_audio_render_event+0x1b74>
    98a8: f8df 350c    	ldr.w	r3, [pc, #0x50c]        @ 0x9db8 <gst_lg_audio_render_event+0x1b78>
    98ac: 447a         	add	r2, pc
    98ae: 9202         	str	r2, [sp, #0x8]
    98b0: f240 525f    	movw	r2, #0x55f
    98b4: 9200         	str	r2, [sp]
    98b6: 447b         	add	r3, pc
    98b8: f8df 2500    	ldr.w	r2, [pc, #0x500]        @ 0x9dbc <gst_lg_audio_render_event+0x1b7c>
    98bc: f503 73c6    	add.w	r3, r3, #0x18c
    98c0: 447a         	add	r2, pc
    98c2: f7fa e9d4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5c58
    98c6: f8df 14f8    	ldr.w	r1, [pc, #0x4f8]        @ 0x9dc0 <gst_lg_audio_render_event+0x1b80>
    98ca: aa28         	add	r2, sp, #0xa0
    98cc: 4648         	mov	r0, r9
    98ce: 4479         	add	r1, pc
    98d0: f7fa e89a    	blx	0x3a08 <gst_structure_get_int@plt> @ imm = #-0x5ecc
    98d4: b328         	cbz	r0, 0x9922 <gst_lg_audio_render_event+0x16e2> @ imm = #0x4a
    98d6: f8df 04ec    	ldr.w	r0, [pc, #0x4ec]        @ 0x9dc4 <gst_lg_audio_render_event+0x1b84>
    98da: 9b28         	ldr	r3, [sp, #0xa0]
    98dc: 4478         	add	r0, pc
    98de: 68c2         	ldr	r2, [r0, #0xc]
    98e0: 9343         	str	r3, [sp, #0x10c]
    98e2: 6812         	ldr	r2, [r2]
    98e4: 06d2         	lsls	r2, r2, #0x1b
    98e6: f101 8581    	bmi.w	0xb3ec <gst_lg_audio_render_event+0x31ac> @ imm = #0x1b02
    98ea: f8d8 3000    	ldr.w	r3, [r8]
    98ee: 2b04         	cmp	r3, #0x4
    98f0: d917         	bls	0x9922 <gst_lg_audio_render_event+0x16e2> @ imm = #0x2e
    98f2: 4bc7         	ldr	r3, [pc, #0x31c]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    98f4: 2105         	movs	r1, #0x5
    98f6: 9a28         	ldr	r2, [sp, #0xa0]
    98f8: 58fb         	ldr	r3, [r7, r3]
    98fa: 9401         	str	r4, [sp, #0x4]
    98fc: 6818         	ldr	r0, [r3]
    98fe: 9203         	str	r2, [sp, #0xc]
    9900: f8df 24c4    	ldr.w	r2, [pc, #0x4c4]        @ 0x9dc8 <gst_lg_audio_render_event+0x1b88>
    9904: f8df 34c4    	ldr.w	r3, [pc, #0x4c4]        @ 0x9dcc <gst_lg_audio_render_event+0x1b8c>
    9908: 447a         	add	r2, pc
    990a: 9202         	str	r2, [sp, #0x8]
    990c: f240 5264    	movw	r2, #0x564
    9910: 9200         	str	r2, [sp]
    9912: 447b         	add	r3, pc
    9914: f8df 24b8    	ldr.w	r2, [pc, #0x4b8]        @ 0x9dd0 <gst_lg_audio_render_event+0x1b90>
    9918: f503 73c6    	add.w	r3, r3, #0x18c
    991c: 447a         	add	r2, pc
    991e: f7fa e9a6    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5cb4
    9922: f8df 14b0    	ldr.w	r1, [pc, #0x4b0]        @ 0x9dd4 <gst_lg_audio_render_event+0x1b94>
    9926: 4648         	mov	r0, r9
    9928: 4479         	add	r1, pc
    992a: f7fa e9fa    	blx	0x3d20 <gst_structure_get_string@plt> @ imm = #-0x5c0c
    992e: 4682         	mov	r10, r0
    9930: b380         	cbz	r0, 0x9994 <gst_lg_audio_render_event+0x1754> @ imm = #0x60
    9932: f8df e4a4    	ldr.w	lr, [pc, #0x4a4]        @ 0x9dd8 <gst_lg_audio_render_event+0x1b98>
    9936: f10d 0bfc    	add.w	r11, sp, #0xfc
    993a: 6800         	ldr	r0, [r0]
    993c: 46dc         	mov	r12, r11
    993e: f8da 300c    	ldr.w	r3, [r10, #0xc]
    9942: f8da 1004    	ldr.w	r1, [r10, #0x4]
    9946: 44fe         	add	lr, pc
    9948: f8da 2008    	ldr.w	r2, [r10, #0x8]
    994c: e8ac 000f    	stm.w	r12!, {r0, r1, r2, r3}
    9950: f8de 300c    	ldr.w	r3, [lr, #0xc]
    9954: 681b         	ldr	r3, [r3]
    9956: 06d8         	lsls	r0, r3, #0x1b
    9958: f101 833d    	bmi.w	0xafd6 <gst_lg_audio_render_event+0x2d96> @ imm = #0x167a
    995c: f8d8 3000    	ldr.w	r3, [r8]
    9960: 2b04         	cmp	r3, #0x4
    9962: d917         	bls	0x9994 <gst_lg_audio_render_event+0x1754> @ imm = #0x2e
    9964: 4baa         	ldr	r3, [pc, #0x2a8]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9966: 2105         	movs	r1, #0x5
    9968: f8df 2470    	ldr.w	r2, [pc, #0x470]        @ 0x9ddc <gst_lg_audio_render_event+0x1b9c>
    996c: 58fb         	ldr	r3, [r7, r3]
    996e: 447a         	add	r2, pc
    9970: e9cd ab03    	strd	r10, r11, [sp, #12]
    9974: 9401         	str	r4, [sp, #0x4]
    9976: 6818         	ldr	r0, [r3]
    9978: f8df 3464    	ldr.w	r3, [pc, #0x464]        @ 0x9de0 <gst_lg_audio_render_event+0x1ba0>
    997c: 9202         	str	r2, [sp, #0x8]
    997e: f240 5269    	movw	r2, #0x569
    9982: 9200         	str	r2, [sp]
    9984: f8df 245c    	ldr.w	r2, [pc, #0x45c]        @ 0x9de4 <gst_lg_audio_render_event+0x1ba4>
    9988: 447b         	add	r3, pc
    998a: f503 73c6    	add.w	r3, r3, #0x18c
    998e: 447a         	add	r2, pc
    9990: f7fa e96c    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5d28
    9994: f8df 1450    	ldr.w	r1, [pc, #0x450]        @ 0x9de8 <gst_lg_audio_render_event+0x1ba8>
    9998: 4648         	mov	r0, r9
    999a: aa21         	add	r2, sp, #0x84
    999c: 4479         	add	r1, pc
    999e: f7fa ebb6    	blx	0x410c <gst_structure_get_boolean@plt> @ imm = #-0x5894
    99a2: 2800         	cmp	r0, #0x0
    99a4: f001 801c    	beq.w	0xa9e0 <gst_lg_audio_render_event+0x27a0> @ imm = #0x1038
    99a8: f8df 0440    	ldr.w	r0, [pc, #0x440]        @ 0x9dec <gst_lg_audio_render_event+0x1bac>
    99ac: 9b21         	ldr	r3, [sp, #0x84]
    99ae: 4478         	add	r0, pc
    99b0: 68c2         	ldr	r2, [r0, #0xc]
    99b2: 9346         	str	r3, [sp, #0x118]
    99b4: 6812         	ldr	r2, [r2]
    99b6: 06d1         	lsls	r1, r2, #0x1b
    99b8: f101 857b    	bmi.w	0xb4b2 <gst_lg_audio_render_event+0x3272> @ imm = #0x1af6
    99bc: f8d8 3000    	ldr.w	r3, [r8]
    99c0: 2b04         	cmp	r3, #0x4
    99c2: d917         	bls	0x99f4 <gst_lg_audio_render_event+0x17b4> @ imm = #0x2e
    99c4: 4b92         	ldr	r3, [pc, #0x248]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    99c6: 2105         	movs	r1, #0x5
    99c8: 9a46         	ldr	r2, [sp, #0x118]
    99ca: 58fb         	ldr	r3, [r7, r3]
    99cc: 9401         	str	r4, [sp, #0x4]
    99ce: 6818         	ldr	r0, [r3]
    99d0: 9203         	str	r2, [sp, #0xc]
    99d2: f8df 241c    	ldr.w	r2, [pc, #0x41c]        @ 0x9df0 <gst_lg_audio_render_event+0x1bb0>
    99d6: f8df 341c    	ldr.w	r3, [pc, #0x41c]        @ 0x9df4 <gst_lg_audio_render_event+0x1bb4>
    99da: 447a         	add	r2, pc
    99dc: 9202         	str	r2, [sp, #0x8]
    99de: f240 526f    	movw	r2, #0x56f
    99e2: 9200         	str	r2, [sp]
    99e4: 447b         	add	r3, pc
    99e6: f8df 2410    	ldr.w	r2, [pc, #0x410]        @ 0x9df8 <gst_lg_audio_render_event+0x1bb8>
    99ea: f503 73c6    	add.w	r3, r3, #0x18c
    99ee: 447a         	add	r2, pc
    99f0: f7fa e93c    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5d88
    99f4: 9a31         	ldr	r2, [sp, #0xc4]
    99f6: f44f 435c    	mov.w	r3, #0xdc00
    99fa: f2c0 0305    	movt	r3, #0x5
    99fe: f04f 0900    	mov.w	r9, #0x0
    9a02: f8c4 9228    	str.w	r9, [r4, #0x228]
    9a06: 429a         	cmp	r2, r3
    9a08: f240 85d4    	bls.w	0xa5b4 <gst_lg_audio_render_event+0x2374> @ imm = #0xba8
    9a0c: 48fb         	ldr	r0, [pc, #0x3ec]        @ 0x9dfc <gst_lg_audio_render_event+0x1bbc>
    9a0e: 4478         	add	r0, pc
    9a10: 68c3         	ldr	r3, [r0, #0xc]
    9a12: 681b         	ldr	r3, [r3]
    9a14: 07db         	lsls	r3, r3, #0x1f
    9a16: f101 84a5    	bmi.w	0xb364 <gst_lg_audio_render_event+0x3124> @ imm = #0x194a
    9a1a: f8d8 3000    	ldr.w	r3, [r8]
    9a1e: b1a3         	cbz	r3, 0x9a4a <gst_lg_audio_render_event+0x180a> @ imm = #0x28
    9a20: 4b7b         	ldr	r3, [pc, #0x1ec]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9a22: 2101         	movs	r1, #0x1
    9a24: 9a31         	ldr	r2, [sp, #0xc4]
    9a26: 58fb         	ldr	r3, [r7, r3]
    9a28: 9401         	str	r4, [sp, #0x4]
    9a2a: 6818         	ldr	r0, [r3]
    9a2c: 9203         	str	r2, [sp, #0xc]
    9a2e: 4af4         	ldr	r2, [pc, #0x3d0]        @ 0x9e00 <gst_lg_audio_render_event+0x1bc0>
    9a30: 4bf4         	ldr	r3, [pc, #0x3d0]        @ 0x9e04 <gst_lg_audio_render_event+0x1bc4>
    9a32: 447a         	add	r2, pc
    9a34: 9202         	str	r2, [sp, #0x8]
    9a36: f44f 62af    	mov.w	r2, #0x578
    9a3a: 9200         	str	r2, [sp]
    9a3c: 447b         	add	r3, pc
    9a3e: 4af2         	ldr	r2, [pc, #0x3c8]        @ 0x9e08 <gst_lg_audio_render_event+0x1bc8>
    9a40: f503 73c6    	add.w	r3, r3, #0x18c
    9a44: 447a         	add	r2, pc
    9a46: f7fa e912    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5ddc
    9a4a: 2301         	movs	r3, #0x1
    9a4c: f8c4 3228    	str.w	r3, [r4, #0x228]
    9a50: f89d 30c8    	ldrb.w	r3, [sp, #0xc8]
    9a54: 2b0a         	cmp	r3, #0xa
    9a56: d922         	bls	0x9a9e <gst_lg_audio_render_event+0x185e> @ imm = #0x44
    9a58: 48ec         	ldr	r0, [pc, #0x3b0]        @ 0x9e0c <gst_lg_audio_render_event+0x1bcc>
    9a5a: 4478         	add	r0, pc
    9a5c: 68c2         	ldr	r2, [r0, #0xc]
    9a5e: 6812         	ldr	r2, [r2]
    9a60: 07d1         	lsls	r1, r2, #0x1f
    9a62: f101 846e    	bmi.w	0xb342 <gst_lg_audio_render_event+0x3102> @ imm = #0x18dc
    9a66: f8d8 3000    	ldr.w	r3, [r8]
    9a6a: b1ab         	cbz	r3, 0x9a98 <gst_lg_audio_render_event+0x1858> @ imm = #0x2a
    9a6c: 4b68         	ldr	r3, [pc, #0x1a0]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9a6e: 2101         	movs	r1, #0x1
    9a70: f89d 20c8    	ldrb.w	r2, [sp, #0xc8]
    9a74: 58fb         	ldr	r3, [r7, r3]
    9a76: 9401         	str	r4, [sp, #0x4]
    9a78: 6818         	ldr	r0, [r3]
    9a7a: 9203         	str	r2, [sp, #0xc]
    9a7c: 4ae4         	ldr	r2, [pc, #0x390]        @ 0x9e10 <gst_lg_audio_render_event+0x1bd0>
    9a7e: 4be5         	ldr	r3, [pc, #0x394]        @ 0x9e14 <gst_lg_audio_render_event+0x1bd4>
    9a80: 447a         	add	r2, pc
    9a82: 9202         	str	r2, [sp, #0x8]
    9a84: f240 527e    	movw	r2, #0x57e
    9a88: 9200         	str	r2, [sp]
    9a8a: 447b         	add	r3, pc
    9a8c: 4ae2         	ldr	r2, [pc, #0x388]        @ 0x9e18 <gst_lg_audio_render_event+0x1bd8>
    9a8e: f503 73c6    	add.w	r3, r3, #0x18c
    9a92: 447a         	add	r2, pc
    9a94: f7fa e8ea    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5e2c
    9a98: 2301         	movs	r3, #0x1
    9a9a: f8c4 3228    	str.w	r3, [r4, #0x228]
    9a9e: 48df         	ldr	r0, [pc, #0x37c]        @ 0x9e1c <gst_lg_audio_render_event+0x1bdc>
    9aa0: 4478         	add	r0, pc
    9aa2: 68c3         	ldr	r3, [r0, #0xc]
    9aa4: 681b         	ldr	r3, [r3]
    9aa6: 07da         	lsls	r2, r3, #0x1f
    9aa8: f101 85a4    	bmi.w	0xb5f4 <gst_lg_audio_render_event+0x33b4> @ imm = #0x1b48
    9aac: f8d8 3000    	ldr.w	r3, [r8]
    9ab0: b193         	cbz	r3, 0x9ad8 <gst_lg_audio_render_event+0x1898> @ imm = #0x24
    9ab2: 4b57         	ldr	r3, [pc, #0x15c]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9ab4: 2101         	movs	r1, #0x1
    9ab6: 4ada         	ldr	r2, [pc, #0x368]        @ 0x9e20 <gst_lg_audio_render_event+0x1be0>
    9ab8: 58fb         	ldr	r3, [r7, r3]
    9aba: 447a         	add	r2, pc
    9abc: 9401         	str	r4, [sp, #0x4]
    9abe: 6818         	ldr	r0, [r3]
    9ac0: 4bd8         	ldr	r3, [pc, #0x360]        @ 0x9e24 <gst_lg_audio_render_event+0x1be4>
    9ac2: 9202         	str	r2, [sp, #0x8]
    9ac4: f240 5285    	movw	r2, #0x585
    9ac8: 9200         	str	r2, [sp]
    9aca: 4ad7         	ldr	r2, [pc, #0x35c]        @ 0x9e28 <gst_lg_audio_render_event+0x1be8>
    9acc: 447b         	add	r3, pc
    9ace: f503 73c6    	add.w	r3, r3, #0x18c
    9ad2: 447a         	add	r2, pc
    9ad4: f7fa e8ca    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5e6c
    9ad8: f8df a350    	ldr.w	r10, [pc, #0x350]       @ 0x9e2c <gst_lg_audio_render_event+0x1bec>
    9adc: 44fa         	add	r10, pc
    9ade: 4650         	mov	r0, r10
    9ae0: f7fa eaa8    	blx	0x4034 <_gst_element_error_printf@plt> @ imm = #-0x5ab0
    9ae4: 4681         	mov	r9, r0
    9ae6: 4650         	mov	r0, r10
    9ae8: f7fa eaa4    	blx	0x4034 <_gst_element_error_printf@plt> @ imm = #-0x5ab8
    9aec: 4682         	mov	r10, r0
    9aee: f1b9 0f00    	cmp.w	r9, #0x0
    9af2: d018         	beq	0x9b26 <gst_lg_audio_render_event+0x18e6> @ imm = #0x30
    9af4: f8d8 3000    	ldr.w	r3, [r8]
    9af8: 2b01         	cmp	r3, #0x1
    9afa: d930         	bls	0x9b5e <gst_lg_audio_render_event+0x191e> @ imm = #0x60
    9afc: 4b44         	ldr	r3, [pc, #0x110]        @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9afe: 2102         	movs	r1, #0x2
    9b00: 4acb         	ldr	r2, [pc, #0x32c]        @ 0x9e30 <gst_lg_audio_render_event+0x1bf0>
    9b02: 58fb         	ldr	r3, [r7, r3]
    9b04: 447a         	add	r2, pc
    9b06: f8cd 900c    	str.w	r9, [sp, #0xc]
    9b0a: 9401         	str	r4, [sp, #0x4]
    9b0c: 6818         	ldr	r0, [r3]
    9b0e: 4bc9         	ldr	r3, [pc, #0x324]        @ 0x9e34 <gst_lg_audio_render_event+0x1bf4>
    9b10: 9202         	str	r2, [sp, #0x8]
    9b12: f240 5287    	movw	r2, #0x587
    9b16: 9200         	str	r2, [sp]
    9b18: 4ac7         	ldr	r2, [pc, #0x31c]        @ 0x9e38 <gst_lg_audio_render_event+0x1bf8>
    9b1a: 447b         	add	r3, pc
    9b1c: f503 73c6    	add.w	r3, r3, #0x18c
    9b20: 447a         	add	r2, pc
    9b22: f7fa e8a4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5eb8
    9b26: f1ba 0f00    	cmp.w	r10, #0x0
    9b2a: d018         	beq	0x9b5e <gst_lg_audio_render_event+0x191e> @ imm = #0x30
    9b2c: f8d8 3000    	ldr.w	r3, [r8]
    9b30: 2b01         	cmp	r3, #0x1
    9b32: d914         	bls	0x9b5e <gst_lg_audio_render_event+0x191e> @ imm = #0x28
    9b34: 4b36         	ldr	r3, [pc, #0xd8]         @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9b36: 2102         	movs	r1, #0x2
    9b38: 4ac0         	ldr	r2, [pc, #0x300]        @ 0x9e3c <gst_lg_audio_render_event+0x1bfc>
    9b3a: 58fb         	ldr	r3, [r7, r3]
    9b3c: 447a         	add	r2, pc
    9b3e: f8cd a00c    	str.w	r10, [sp, #0xc]
    9b42: 9401         	str	r4, [sp, #0x4]
    9b44: 6818         	ldr	r0, [r3]
    9b46: 4bbe         	ldr	r3, [pc, #0x2f8]        @ 0x9e40 <gst_lg_audio_render_event+0x1c00>
    9b48: 9202         	str	r2, [sp, #0x8]
    9b4a: f240 5287    	movw	r2, #0x587
    9b4e: 9200         	str	r2, [sp]
    9b50: 4abc         	ldr	r2, [pc, #0x2f0]        @ 0x9e44 <gst_lg_audio_render_event+0x1c04>
    9b52: 447b         	add	r3, pc
    9b54: f503 73c6    	add.w	r3, r3, #0x18c
    9b58: 447a         	add	r2, pc
    9b5a: f7fa e888    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5ef0
    9b5e: f7fa ea1a    	blx	0x3f94 <gst_element_get_type@plt> @ imm = #-0x5bcc
    9b62: 4601         	mov	r1, r0
    9b64: 4620         	mov	r0, r4
    9b66: f7fa ea10    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x5be0
    9b6a: 4604         	mov	r4, r0
    9b6c: f7fa eb22    	blx	0x41b4 <gst_stream_error_quark@plt> @ imm = #-0x59bc
    9b70: 4bb5         	ldr	r3, [pc, #0x2d4]        @ 0x9e48 <gst_lg_audio_render_event+0x1c08>
    9b72: 4602         	mov	r2, r0
    9b74: 2104         	movs	r1, #0x4
    9b76: 4620         	mov	r0, r4
    9b78: e9cd 9a00    	strd	r9, r10, [sp]
    9b7c: 447b         	add	r3, pc
    9b7e: f503 73c6    	add.w	r3, r3, #0x18c
    9b82: 9303         	str	r3, [sp, #0xc]
    9b84: 4bb1         	ldr	r3, [pc, #0x2c4]        @ 0x9e4c <gst_lg_audio_render_event+0x1c0c>
    9b86: 447b         	add	r3, pc
    9b88: 9302         	str	r3, [sp, #0x8]
    9b8a: f240 5387    	movw	r3, #0x587
    9b8e: 9304         	str	r3, [sp, #0x10]
    9b90: 2306         	movs	r3, #0x6
    9b92: f7f9 ef10    	blx	0x39b4 <gst_element_message_full@plt> @ imm = #-0x61e0
    9b96: 9b16         	ldr	r3, [sp, #0x58]
    9b98: 2b00         	cmp	r3, #0x0
    9b9a: f43e ac8c    	beq.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x16e8
    9b9e: 4618         	mov	r0, r3
    9ba0: f7f9 edee    	blx	0x3780 <g_free@plt>     @ imm = #-0x6424
    9ba4: f7fe bc87    	b.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x16f2
    9ba8: 4ba9         	ldr	r3, [pc, #0x2a4]        @ 0x9e50 <gst_lg_audio_render_event+0x1c10>
    9baa: 447b         	add	r3, pc
    9bac: 68db         	ldr	r3, [r3, #0xc]
    9bae: 681b         	ldr	r3, [r3]
    9bb0: 071a         	lsls	r2, r3, #0x1c
    9bb2: f57e ac61    	bpl.w	0x8478 <gst_lg_audio_render_event+0x238> @ imm = #-0x173e
    9bb6: 4aa7         	ldr	r2, [pc, #0x29c]        @ 0x9e54 <gst_lg_audio_render_event+0x1c14>
    9bb8: f640 03ff    	movw	r3, #0x8ff
    9bbc: f8df c298    	ldr.w	r12, [pc, #0x298]       @ 0x9e58 <gst_lg_audio_render_event+0x1c18>
    9bc0: 2103         	movs	r1, #0x3
    9bc2: e9d4 ab82    	ldrd	r10, r11, [r4, #520]
    9bc6: 48a5         	ldr	r0, [pc, #0x294]        @ 0x9e5c <gst_lg_audio_render_event+0x1c1c>
    9bc8: 447a         	add	r2, pc
    9bca: 44fc         	add	r12, pc
    9bcc: f502 72b8    	add.w	r2, r2, #0x170
    9bd0: e9cd ab02    	strd	r10, r11, [sp, #8]
    9bd4: f8cd c000    	str.w	r12, [sp]
    9bd8: 4478         	add	r0, pc
    9bda: f7fa e9e8    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x5c30
    9bde: f7fe bc30    	b.w	0x8442 <gst_lg_audio_render_event+0x202> @ imm = #-0x17a0
    9be2: 4a0b         	ldr	r2, [pc, #0x2c]         @ 0x9c10 <gst_lg_audio_render_event+0x19d0>
    9be4: f640 1189    	movw	r1, #0x989
    9be8: 4b9d         	ldr	r3, [pc, #0x274]        @ 0x9e60 <gst_lg_audio_render_event+0x1c20>
    9bea: 58b8         	ldr	r0, [r7, r2]
    9bec: 4a9d         	ldr	r2, [pc, #0x274]        @ 0x9e64 <gst_lg_audio_render_event+0x1c24>
    9bee: 447b         	add	r3, pc
    9bf0: 9401         	str	r4, [sp, #0x4]
    9bf2: f503 73b8    	add.w	r3, r3, #0x170
    9bf6: 6800         	ldr	r0, [r0]
    9bf8: 447a         	add	r2, pc
    9bfa: 9100         	str	r1, [sp]
    9bfc: 9202         	str	r2, [sp, #0x8]
    9bfe: 2104         	movs	r1, #0x4
    9c00: 4a99         	ldr	r2, [pc, #0x264]        @ 0x9e68 <gst_lg_audio_render_event+0x1c28>
    9c02: 447a         	add	r2, pc
    9c04: f7fa e832    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x5f9c
    9c08: f7fe bb7d    	b.w	0x8306 <gst_lg_audio_render_event+0xc6> @ imm = #-0x1906
    9c0c: c2 8f 02 00  	.word	0x00028fc2
    9c10: 08 04 00 00  	.word	0x00000408
    9c14: 56 24 01 00  	.word	0x00012456
    9c18: 1c 6b 01 00  	.word	0x00016b1c
    9c1c: ba 08 01 00  	.word	0x000108ba
    9c20: 36 24 01 00  	.word	0x00012436
    9c24: 6e 24 01 00  	.word	0x0001246e
    9c28: 54 8f 02 00  	.word	0x00028f54
    9c2c: 5a 24 01 00  	.word	0x0001245a
    9c30: b0 6a 01 00  	.word	0x00016ab0
    9c34: 4a 08 01 00  	.word	0x0001084a
    9c38: 6c 2a 01 00  	.word	0x00012a6c
    9c3c: f6 8e 02 00  	.word	0x00028ef6
    9c40: 5e 2a 01 00  	.word	0x00012a5e
    9c44: 50 6a 01 00  	.word	0x00016a50
    9c48: ea 07 01 00  	.word	0x000107ea
    9c4c: 0a 24 01 00  	.word	0x0001240a
    9c50: 94 8e 02 00  	.word	0x00028e94
    9c54: 00 24 01 00  	.word	0x00012400
    9c58: ee 69 01 00  	.word	0x000169ee
    9c5c: 8c 07 01 00  	.word	0x0001078c
    9c60: 42 8e 02 00  	.word	0x00028e42
    9c64: a8 23 01 00  	.word	0x000123a8
    9c68: 76 69 01 00  	.word	0x00016976
    9c6c: 10 07 01 00  	.word	0x00010710
    9c70: ca 8d 02 00  	.word	0x00028dca
    9c74: 80 23 01 00  	.word	0x00012380
    9c78: 1e 69 01 00  	.word	0x0001691e
    9c7c: b8 06 01 00  	.word	0x000106b8
    9c80: 6e 8d 02 00  	.word	0x00028d6e
    9c84: 52 23 01 00  	.word	0x00012352
    9c88: c4 68 01 00  	.word	0x000168c4
    9c8c: 5e 06 01 00  	.word	0x0001065e
    9c90: 78 24 01 00  	.word	0x00012478
    9c94: 06 8d 02 00  	.word	0x00028d06
    9c98: cc 24 01 00  	.word	0x000124cc
    9c9c: 62 68 01 00  	.word	0x00016862
    9ca0: fc 05 01 00  	.word	0x000105fc
    9ca4: 32 25 01 00  	.word	0x00012532
    9ca8: a4 8c 02 00  	.word	0x00028ca4
    9cac: 22 25 01 00  	.word	0x00012522
    9cb0: 00 68 01 00  	.word	0x00016800
    9cb4: 9a 05 01 00  	.word	0x0001059a
    9cb8: 60 25 01 00  	.word	0x00012560
    9cbc: 42 8c 02 00  	.word	0x00028c42
    9cc0: 4c 25 01 00  	.word	0x0001254c
    9cc4: 9e 67 01 00  	.word	0x0001679e
    9cc8: 38 05 01 00  	.word	0x00010538
    9ccc: 7e 25 01 00  	.word	0x0001257e
    9cd0: e0 8b 02 00  	.word	0x00028be0
    9cd4: 5e 25 01 00  	.word	0x0001255e
    9cd8: 3c 67 01 00  	.word	0x0001673c
    9cdc: d6 04 01 00  	.word	0x000104d6
    9ce0: 84 25 01 00  	.word	0x00012584
    9ce4: 7e 8b 02 00  	.word	0x00028b7e
    9ce8: 74 25 01 00  	.word	0x00012574
    9cec: da 66 01 00  	.word	0x000166da
    9cf0: 74 04 01 00  	.word	0x00010474
    9cf4: ba 25 01 00  	.word	0x000125ba
    9cf8: a4 25 01 00  	.word	0x000125a4
    9cfc: 06 8b 02 00  	.word	0x00028b06
    9d00: 9a 25 01 00  	.word	0x0001259a
    9d04: 60 66 01 00  	.word	0x00016660
    9d08: fe 03 01 00  	.word	0x000103fe
    9d0c: 84 25 01 00  	.word	0x00012584
    9d10: a6 8a 02 00  	.word	0x00028aa6
    9d14: 64 25 01 00  	.word	0x00012564
    9d18: 02 66 01 00  	.word	0x00016602
    9d1c: 9c 03 01 00  	.word	0x0001039c
    9d20: 4e 25 01 00  	.word	0x0001254e
    9d24: 44 25 01 00  	.word	0x00012544
    9d28: 72 25 01 00  	.word	0x00012572
    9d2c: 9c 65 01 00  	.word	0x0001659c
    9d30: 3a 03 01 00  	.word	0x0001033a
    9d34: 30 25 01 00  	.word	0x00012530
    9d38: ba 89 02 00  	.word	0x000289ba
    9d3c: 20 25 01 00  	.word	0x00012520
    9d40: 16 65 01 00  	.word	0x00016516
    9d44: b0 02 01 00  	.word	0x000102b0
    9d48: 5a 25 01 00  	.word	0x0001255a
    9d4c: 58 89 02 00  	.word	0x00028958
    9d50: 3e 25 01 00  	.word	0x0001253e
    9d54: b4 64 01 00  	.word	0x000164b4
    9d58: 4e 02 01 00  	.word	0x0001024e
    9d5c: 68 25 01 00  	.word	0x00012568
    9d60: fa 88 02 00  	.word	0x000288fa
    9d64: 60 25 01 00  	.word	0x00012560
    9d68: 56 64 01 00  	.word	0x00016456
    9d6c: f0 01 01 00  	.word	0x000101f0
    9d70: 52 25 01 00  	.word	0x00012552
    9d74: 98 88 02 00  	.word	0x00028898
    9d78: 32 25 01 00  	.word	0x00012532
    9d7c: f4 63 01 00  	.word	0x000163f4
    9d80: 8e 01 01 00  	.word	0x0001018e
    9d84: 58 25 01 00  	.word	0x00012558
    9d88: 3a 88 02 00  	.word	0x0002883a
    9d8c: 38 25 01 00  	.word	0x00012538
    9d90: 96 63 01 00  	.word	0x00016396
    9d94: 30 01 01 00  	.word	0x00010130
    9d98: 1e 25 01 00  	.word	0x0001251e
    9d9c: dc 87 02 00  	.word	0x000287dc
    9da0: 14 25 01 00  	.word	0x00012514
    9da4: 3a 63 01 00  	.word	0x0001633a
    9da8: d4 00 01 00  	.word	0x000100d4
    9dac: 06 25 01 00  	.word	0x00012506
    9db0: 80 87 02 00  	.word	0x00028780
    9db4: f4 24 01 00  	.word	0x000124f4
    9db8: de 62 01 00  	.word	0x000162de
    9dbc: 78 00 01 00  	.word	0x00010078
    9dc0: e2 24 01 00  	.word	0x000124e2
    9dc4: 24 87 02 00  	.word	0x00028724
    9dc8: d0 24 01 00  	.word	0x000124d0
    9dcc: 82 62 01 00  	.word	0x00016282
    9dd0: 1c 00 01 00  	.word	0x0001001c
    9dd4: c0 24 01 00  	.word	0x000124c0
    9dd8: ba 86 02 00  	.word	0x000286ba
    9ddc: aa 24 01 00  	.word	0x000124aa
    9de0: 0c 62 01 00  	.word	0x0001620c
    9de4: aa ff 00 00  	.word	0x0000ffaa
    9de8: 94 24 01 00  	.word	0x00012494
    9dec: 52 86 02 00  	.word	0x00028652
    9df0: 7a 24 01 00  	.word	0x0001247a
    9df4: b0 61 01 00  	.word	0x000161b0
    9df8: 4a ff 00 00  	.word	0x0000ff4a
    9dfc: f2 85 02 00  	.word	0x000285f2
    9e00: 96 24 01 00  	.word	0x00012496
    9e04: 58 61 01 00  	.word	0x00016158
    9e08: f4 fe 00 00  	.word	0x0000fef4
    9e0c: a6 85 02 00  	.word	0x000285a6
    9e10: 90 24 01 00  	.word	0x00012490
    9e14: 0a 61 01 00  	.word	0x0001610a
    9e18: a6 fe 00 00  	.word	0x0000fea6
    9e1c: 60 85 02 00  	.word	0x00028560
    9e20: aa 24 01 00  	.word	0x000124aa
    9e24: c8 60 01 00  	.word	0x000160c8
    9e28: 66 fe 00 00  	.word	0x0000fe66
    9e2c: b0 24 01 00  	.word	0x000124b0
    9e30: a4 24 01 00  	.word	0x000124a4
    9e34: 7a 60 01 00  	.word	0x0001607a
    9e38: 18 fe 00 00  	.word	0x0000fe18
    9e3c: 6c 24 01 00  	.word	0x0001246c
    9e40: 42 60 01 00  	.word	0x00016042
    9e44: e0 fd 00 00  	.word	0x0000fde0
    9e48: 18 60 01 00  	.word	0x00016018
    9e4c: b2 fd 00 00  	.word	0x0000fdb2
    9e50: 56 84 02 00  	.word	0x00028456
    9e54: cc 5f 01 00  	.word	0x00015fcc
    9e58: 7a 15 01 00  	.word	0x0001157a
    9e5c: 28 84 02 00  	.word	0x00028428
    9e60: a6 5f 01 00  	.word	0x00015fa6
    9e64: 00 2b 01 00  	.word	0x00012b00
    9e68: 36 fd 00 00  	.word	0x0000fd36
    9e6c: f7fa e918    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0x5dd0
    9e70: 4601         	mov	r1, r0
    9e72: 4620         	mov	r0, r4
    9e74: f7fa e888    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x5ef0
    9e78: 2200         	movs	r2, #0x0
    9e7a: 2300         	movs	r3, #0x0
    9e7c: f7f9 ee8c    	blx	0x3b98 <gst_base_sink_set_render_delay@plt> @ imm = #-0x62e8
    9e80: f7fe bb16    	b.w	0x84b0 <gst_lg_audio_render_event+0x270> @ imm = #-0x19d4
    9e84: 4649         	mov	r1, r9
    9e86: 4620         	mov	r0, r4
    9e88: f7fb f86a    	bl	0x4f60 <gst_lg_audio_render_update_ts_offset> @ imm = #-0x4f2c
    9e8c: f8d4 7224    	ldr.w	r7, [r4, #0x224]
    9e90: 2f00         	cmp	r7, #0x0
    9e92: f47e ab05    	bne.w	0x84a0 <gst_lg_audio_render_event+0x260> @ imm = #-0x19f6
    9e96: f7fe bb0b    	b.w	0x84b0 <gst_lg_audio_render_event+0x270> @ imm = #-0x19ea
    9e9a: 9001         	str	r0, [sp, #0x4]
    9e9c: 4618         	mov	r0, r3
    9e9e: f8df 2c54    	ldr.w	r2, [pc, #0xc54]        @ 0xaaf4 <gst_lg_audio_render_event+0x28b4>
    9ea2: 2103         	movs	r1, #0x3
    9ea4: f8df 3c50    	ldr.w	r3, [pc, #0xc50]        @ 0xaaf8 <gst_lg_audio_render_event+0x28b8>
    9ea8: 447a         	add	r2, pc
    9eaa: 447b         	add	r3, pc
    9eac: f502 72b8    	add.w	r2, r2, #0x170
    9eb0: 9300         	str	r3, [sp]
    9eb2: f640 1347    	movw	r3, #0x947
    9eb6: f7fa e87a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x5f0c
    9eba: f7fe bdd6    	b.w	0x8a6a <gst_lg_audio_render_event+0x82a> @ imm = #-0x1454
    9ebe: f8df 0c3c    	ldr.w	r0, [pc, #0xc3c]        @ 0xaafc <gst_lg_audio_render_event+0x28bc>
    9ec2: 4478         	add	r0, pc
    9ec4: 68c3         	ldr	r3, [r0, #0xc]
    9ec6: 681b         	ldr	r3, [r3]
    9ec8: 075b         	lsls	r3, r3, #0x1d
    9eca: f100 8247    	bmi.w	0xa35c <gst_lg_audio_render_event+0x211c> @ imm = #0x48e
    9ece: f8d8 3000    	ldr.w	r3, [r8]
    9ed2: 2b03         	cmp	r3, #0x3
    9ed4: f67e aaef    	bls.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x1a22
    9ed8: f8df 3c24    	ldr.w	r3, [pc, #0xc24]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    9edc: 2104         	movs	r1, #0x4
    9ede: f8df 2c24    	ldr.w	r2, [pc, #0xc24]        @ 0xab04 <gst_lg_audio_render_event+0x28c4>
    9ee2: 58fb         	ldr	r3, [r7, r3]
    9ee4: 447a         	add	r2, pc
    9ee6: 9401         	str	r4, [sp, #0x4]
    9ee8: 6818         	ldr	r0, [r3]
    9eea: 9202         	str	r2, [sp, #0x8]
    9eec: f640 02d6    	movw	r2, #0x8d6
    9ef0: f8df 3c14    	ldr.w	r3, [pc, #0xc14]        @ 0xab08 <gst_lg_audio_render_event+0x28c8>
    9ef4: 9200         	str	r2, [sp]
    9ef6: f8df 2c14    	ldr.w	r2, [pc, #0xc14]        @ 0xab0c <gst_lg_audio_render_event+0x28cc>
    9efa: 447b         	add	r3, pc
    9efc: f503 73b8    	add.w	r3, r3, #0x170
    9f00: 447a         	add	r2, pc
    9f02: f7f9 eeb4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6298
    9f06: f7fe bf7e    	b.w	0x8e06 <gst_lg_audio_render_event+0xbc6> @ imm = #-0x1104
    9f0a: f8df 2c04    	ldr.w	r2, [pc, #0xc04]        @ 0xab10 <gst_lg_audio_render_event+0x28d0>
    9f0e: 2102         	movs	r1, #0x2
    9f10: f8df 3c00    	ldr.w	r3, [pc, #0xc00]        @ 0xab14 <gst_lg_audio_render_event+0x28d4>
    9f14: 447a         	add	r2, pc
    9f16: 447b         	add	r3, pc
    9f18: f502 72b8    	add.w	r2, r2, #0x170
    9f1c: 9300         	str	r3, [sp]
    9f1e: f640 03d9    	movw	r3, #0x8d9
    9f22: f7fa e844    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x5f78
    9f26: f8d8 3000    	ldr.w	r3, [r8]
    9f2a: 2b03         	cmp	r3, #0x3
    9f2c: f63e af58    	bhi.w	0x8de0 <gst_lg_audio_render_event+0xba0> @ imm = #-0x1150
    9f30: f8df 3be4    	ldr.w	r3, [pc, #0xbe4]        @ 0xab18 <gst_lg_audio_render_event+0x28d8>
    9f34: 447b         	add	r3, pc
    9f36: 68db         	ldr	r3, [r3, #0xc]
    9f38: 681b         	ldr	r3, [r3]
    9f3a: 075b         	lsls	r3, r3, #0x1d
    9f3c: f57e aabb    	bpl.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x1a8a
    9f40: f8df 2bd8    	ldr.w	r2, [pc, #0xbd8]        @ 0xab1c <gst_lg_audio_render_event+0x28dc>
    9f44: 2102         	movs	r1, #0x2
    9f46: f8df 3bd8    	ldr.w	r3, [pc, #0xbd8]        @ 0xab20 <gst_lg_audio_render_event+0x28e0>
    9f4a: f8df 0bd8    	ldr.w	r0, [pc, #0xbd8]        @ 0xab24 <gst_lg_audio_render_event+0x28e4>
    9f4e: 447a         	add	r2, pc
    9f50: 447b         	add	r3, pc
    9f52: f502 72b8    	add.w	r2, r2, #0x170
    9f56: 9300         	str	r3, [sp]
    9f58: 4478         	add	r0, pc
    9f5a: f640 03e6    	movw	r3, #0x8e6
    9f5e: f7fa e826    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x5fb4
    9f62: f7fe bf57    	b.w	0x8e14 <gst_lg_audio_render_event+0xbd4> @ imm = #-0x1152
    9f66: f8df 2bc0    	ldr.w	r2, [pc, #0xbc0]        @ 0xab28 <gst_lg_audio_render_event+0x28e8>
    9f6a: 2103         	movs	r1, #0x3
    9f6c: f8df 3bbc    	ldr.w	r3, [pc, #0xbbc]        @ 0xab2c <gst_lg_audio_render_event+0x28ec>
    9f70: 447a         	add	r2, pc
    9f72: 447b         	add	r3, pc
    9f74: f502 72b8    	add.w	r2, r2, #0x170
    9f78: 9300         	str	r3, [sp]
    9f7a: f640 1389    	movw	r3, #0x989
    9f7e: f7fa e816    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x5fd4
    9f82: f7fe b9bb    	b.w	0x82fc <gst_lg_audio_render_event+0xbc> @ imm = #-0x1c8a
    9f86: f8df 2ba8    	ldr.w	r2, [pc, #0xba8]        @ 0xab30 <gst_lg_audio_render_event+0x28f0>
    9f8a: 2100         	movs	r1, #0x0
    9f8c: f8df 3ba4    	ldr.w	r3, [pc, #0xba4]        @ 0xab34 <gst_lg_audio_render_event+0x28f4>
    9f90: f8cd 9004    	str.w	r9, [sp, #0x4]
    9f94: 447a         	add	r2, pc
    9f96: 447b         	add	r3, pc
    9f98: f502 72b8    	add.w	r2, r2, #0x170
    9f9c: 9300         	str	r3, [sp]
    9f9e: f640 03fb    	movw	r3, #0x8fb
    9fa2: f7fa e804    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x5ff8
    9fa6: f7fe ba2b    	b.w	0x8400 <gst_lg_audio_render_event+0x1c0> @ imm = #-0x1baa
    9faa: f8df 2b8c    	ldr.w	r2, [pc, #0xb8c]        @ 0xab38 <gst_lg_audio_render_event+0x28f8>
    9fae: 2103         	movs	r1, #0x3
    9fb0: f8df 3b88    	ldr.w	r3, [pc, #0xb88]        @ 0xab3c <gst_lg_audio_render_event+0x28fc>
    9fb4: f8cd 9004    	str.w	r9, [sp, #0x4]
    9fb8: 447a         	add	r2, pc
    9fba: 447b         	add	r3, pc
    9fbc: f502 72b8    	add.w	r2, r2, #0x170
    9fc0: 9300         	str	r3, [sp]
    9fc2: f640 03f3    	movw	r3, #0x8f3
    9fc6: f7f9 eff2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x601c
    9fca: f7fe b9d2    	b.w	0x8372 <gst_lg_audio_render_event+0x132> @ imm = #-0x1c5c
    9fce: f8df 2b70    	ldr.w	r2, [pc, #0xb70]        @ 0xab40 <gst_lg_audio_render_event+0x2900>
    9fd2: 2103         	movs	r1, #0x3
    9fd4: f8df 3b6c    	ldr.w	r3, [pc, #0xb6c]        @ 0xab44 <gst_lg_audio_render_event+0x2904>
    9fd8: 447a         	add	r2, pc
    9fda: 447b         	add	r3, pc
    9fdc: f502 72b8    	add.w	r2, r2, #0x170
    9fe0: 9300         	str	r3, [sp]
    9fe2: f640 03ef    	movw	r3, #0x8ef
    9fe6: f7f9 efe2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x603c
    9fea: f7fe b99a    	b.w	0x8322 <gst_lg_audio_render_event+0xe2> @ imm = #-0x1ccc
    9fee: f8df 2b58    	ldr.w	r2, [pc, #0xb58]        @ 0xab48 <gst_lg_audio_render_event+0x2908>
    9ff2: 2103         	movs	r1, #0x3
    9ff4: f8df 3b54    	ldr.w	r3, [pc, #0xb54]        @ 0xab4c <gst_lg_audio_render_event+0x290c>
    9ff8: 447a         	add	r2, pc
    9ffa: 447b         	add	r3, pc
    9ffc: f502 72b8    	add.w	r2, r2, #0x170
    a000: 9300         	str	r3, [sp]
    a002: f640 133f    	movw	r3, #0x93f
    a006: f7f9 efd2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x605c
    a00a: f7fe bd03    	b.w	0x8a14 <gst_lg_audio_render_event+0x7d4> @ imm = #-0x15fa
    a00e: 9b30         	ldr	r3, [sp, #0xc0]
    a010: 2103         	movs	r1, #0x3
    a012: f8df 2b3c    	ldr.w	r2, [pc, #0xb3c]        @ 0xab50 <gst_lg_audio_render_event+0x2910>
    a016: 9302         	str	r3, [sp, #0x8]
    a018: 9b2e         	ldr	r3, [sp, #0xb8]
    a01a: 447a         	add	r2, pc
    a01c: f502 72b8    	add.w	r2, r2, #0x170
    a020: 9301         	str	r3, [sp, #0x4]
    a022: f8df 3b30    	ldr.w	r3, [pc, #0xb30]        @ 0xab54 <gst_lg_audio_render_event+0x2914>
    a026: 447b         	add	r3, pc
    a028: 9300         	str	r3, [sp]
    a02a: f640 03d2    	movw	r3, #0x8d2
    a02e: f7f9 efbe    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x6084
    a032: f7fe bea1    	b.w	0x8d78 <gst_lg_audio_render_event+0xb38> @ imm = #-0x12be
    a036: f8df 2b20    	ldr.w	r2, [pc, #0xb20]        @ 0xab58 <gst_lg_audio_render_event+0x2918>
    a03a: 2103         	movs	r1, #0x3
    a03c: f8df 3b1c    	ldr.w	r3, [pc, #0xb1c]        @ 0xab5c <gst_lg_audio_render_event+0x291c>
    a040: 447a         	add	r2, pc
    a042: 447b         	add	r3, pc
    a044: f502 72b8    	add.w	r2, r2, #0x170
    a048: 9300         	str	r3, [sp]
    a04a: f640 03cc    	movw	r3, #0x8cc
    a04e: f7f9 efae    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x60a4
    a052: f7fe be67    	b.w	0x8d24 <gst_lg_audio_render_event+0xae4> @ imm = #-0x1332
    a056: f8df 2b08    	ldr.w	r2, [pc, #0xb08]        @ 0xab60 <gst_lg_audio_render_event+0x2920>
    a05a: 2103         	movs	r1, #0x3
    a05c: f8df 3b04    	ldr.w	r3, [pc, #0xb04]        @ 0xab64 <gst_lg_audio_render_event+0x2924>
    a060: 447a         	add	r2, pc
    a062: 447b         	add	r3, pc
    a064: f502 72b8    	add.w	r2, r2, #0x170
    a068: 9300         	str	r3, [sp]
    a06a: f640 03b7    	movw	r3, #0x8b7
    a06e: f7f9 ef9e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x60c4
    a072: f7fe bbec    	b.w	0x884e <gst_lg_audio_render_event+0x60e> @ imm = #-0x1828
    a076: 6a68         	ldr	r0, [r5, #0x24]
    a078: f7f9 ee9a    	blx	0x3db0 <gst_event_type_get_name@plt> @ imm = #-0x62cc
    a07c: f8df 2ae8    	ldr.w	r2, [pc, #0xae8]        @ 0xab68 <gst_lg_audio_render_event+0x2928>
    a080: 2102         	movs	r1, #0x2
    a082: f8df 3ae8    	ldr.w	r3, [pc, #0xae8]        @ 0xab6c <gst_lg_audio_render_event+0x292c>
    a086: 9001         	str	r0, [sp, #0x4]
    a088: 4648         	mov	r0, r9
    a08a: 447a         	add	r2, pc
    a08c: 447b         	add	r3, pc
    a08e: f502 72b8    	add.w	r2, r2, #0x170
    a092: 9300         	str	r3, [sp]
    a094: f640 1364    	movw	r3, #0x964
    a098: f7f9 ef88    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x60f0
    a09c: f7fe bd6f    	b.w	0x8b7e <gst_lg_audio_render_event+0x93e> @ imm = #-0x1522
    a0a0: f8df 0acc    	ldr.w	r0, [pc, #0xacc]        @ 0xab70 <gst_lg_audio_render_event+0x2930>
    a0a4: 4478         	add	r0, pc
    a0a6: 68c3         	ldr	r3, [r0, #0xc]
    a0a8: 681b         	ldr	r3, [r3]
    a0aa: 07de         	lsls	r6, r3, #0x1f
    a0ac: f100 810b    	bmi.w	0xa2c6 <gst_lg_audio_render_event+0x2086> @ imm = #0x216
    a0b0: f8d8 3000    	ldr.w	r3, [r8]
    a0b4: 2b00         	cmp	r3, #0x0
    a0b6: d03d         	beq	0xa134 <gst_lg_audio_render_event+0x1ef4> @ imm = #0x7a
    a0b8: f8df 2a44    	ldr.w	r2, [pc, #0xa44]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a0bc: 2101         	movs	r1, #0x1
    a0be: f8df 3ab4    	ldr.w	r3, [pc, #0xab4]        @ 0xab74 <gst_lg_audio_render_event+0x2934>
    a0c2: 58ba         	ldr	r2, [r7, r2]
    a0c4: 9401         	str	r4, [sp, #0x4]
    a0c6: 447b         	add	r3, pc
    a0c8: f503 73c6    	add.w	r3, r3, #0x18c
    a0cc: 6810         	ldr	r0, [r2]
    a0ce: f8df 2aa8    	ldr.w	r2, [pc, #0xaa8]        @ 0xab78 <gst_lg_audio_render_event+0x2938>
    a0d2: 447a         	add	r2, pc
    a0d4: 9202         	str	r2, [sp, #0x8]
    a0d6: f240 4279    	movw	r2, #0x479
    a0da: 9200         	str	r2, [sp]
    a0dc: f8df 2a9c    	ldr.w	r2, [pc, #0xa9c]        @ 0xab7c <gst_lg_audio_render_event+0x293c>
    a0e0: 447a         	add	r2, pc
    a0e2: f7f9 edc4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6478
    a0e6: f8df 3a98    	ldr.w	r3, [pc, #0xa98]        @ 0xab80 <gst_lg_audio_render_event+0x2940>
    a0ea: 447b         	add	r3, pc
    a0ec: 68db         	ldr	r3, [r3, #0xc]
    a0ee: 681b         	ldr	r3, [r3]
    a0f0: 07d9         	lsls	r1, r3, #0x1f
    a0f2: d426         	bmi	0xa142 <gst_lg_audio_render_event+0x1f02> @ imm = #0x4c
    a0f4: f8d8 3000    	ldr.w	r3, [r8]
    a0f8: b1b3         	cbz	r3, 0xa128 <gst_lg_audio_render_event+0x1ee8> @ imm = #0x2c
    a0fa: f8df 2a04    	ldr.w	r2, [pc, #0xa04]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a0fe: f640 1128    	movw	r1, #0x928
    a102: f8df 3a80    	ldr.w	r3, [pc, #0xa80]        @ 0xab84 <gst_lg_audio_render_event+0x2944>
    a106: 58b8         	ldr	r0, [r7, r2]
    a108: f8df 2a7c    	ldr.w	r2, [pc, #0xa7c]        @ 0xab88 <gst_lg_audio_render_event+0x2948>
    a10c: 447b         	add	r3, pc
    a10e: 9401         	str	r4, [sp, #0x4]
    a110: f503 73b8    	add.w	r3, r3, #0x170
    a114: 6800         	ldr	r0, [r0]
    a116: 447a         	add	r2, pc
    a118: 9100         	str	r1, [sp]
    a11a: 9202         	str	r2, [sp, #0x8]
    a11c: 2101         	movs	r1, #0x1
    a11e: f8df 2a6c    	ldr.w	r2, [pc, #0xa6c]        @ 0xab8c <gst_lg_audio_render_event+0x294c>
    a122: 447a         	add	r2, pc
    a124: f7f9 eda2    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x64bc
    a128: 4628         	mov	r0, r5
    a12a: f7f9 ede8    	blx	0x3cfc <gst_mini_object_unref@plt> @ imm = #-0x6430
    a12e: 2000         	movs	r0, #0x0
    a130: f7fe b8ed    	b.w	0x830e <gst_lg_audio_render_event+0xce> @ imm = #-0x1e26
    a134: f8df 3a58    	ldr.w	r3, [pc, #0xa58]        @ 0xab90 <gst_lg_audio_render_event+0x2950>
    a138: 447b         	add	r3, pc
    a13a: 68db         	ldr	r3, [r3, #0xc]
    a13c: 681b         	ldr	r3, [r3]
    a13e: 07d9         	lsls	r1, r3, #0x1f
    a140: d5f2         	bpl	0xa128 <gst_lg_audio_render_event+0x1ee8> @ imm = #-0x1c
    a142: f8df 2a50    	ldr.w	r2, [pc, #0xa50]        @ 0xab94 <gst_lg_audio_render_event+0x2954>
    a146: 2100         	movs	r1, #0x0
    a148: f8df 3a4c    	ldr.w	r3, [pc, #0xa4c]        @ 0xab98 <gst_lg_audio_render_event+0x2958>
    a14c: f8df 0a4c    	ldr.w	r0, [pc, #0xa4c]        @ 0xab9c <gst_lg_audio_render_event+0x295c>
    a150: 447a         	add	r2, pc
    a152: 447b         	add	r3, pc
    a154: f502 72b8    	add.w	r2, r2, #0x170
    a158: 9300         	str	r3, [sp]
    a15a: 4478         	add	r0, pc
    a15c: f640 1328    	movw	r3, #0x928
    a160: f7f9 ef24    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x61b8
    a164: e7c6         	b	0xa0f4 <gst_lg_audio_render_event+0x1eb4> @ imm = #-0x74
    a166: f8df 3a38    	ldr.w	r3, [pc, #0xa38]        @ 0xaba0 <gst_lg_audio_render_event+0x2960>
    a16a: 447b         	add	r3, pc
    a16c: 68da         	ldr	r2, [r3, #0xc]
    a16e: 6812         	ldr	r2, [r2]
    a170: 07d1         	lsls	r1, r2, #0x1f
    a172: f100 80c7    	bmi.w	0xa304 <gst_lg_audio_render_event+0x20c4> @ imm = #0x18e
    a176: f8d8 3000    	ldr.w	r3, [r8]
    a17a: 2b00         	cmp	r3, #0x0
    a17c: f43e a99b    	beq.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x1cca
    a180: f8df 297c    	ldr.w	r2, [pc, #0x97c]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a184: f8df 3a1c    	ldr.w	r3, [pc, #0xa1c]        @ 0xaba4 <gst_lg_audio_render_event+0x2964>
    a188: 58b9         	ldr	r1, [r7, r2]
    a18a: f640 1243    	movw	r2, #0x943
    a18e: 9200         	str	r2, [sp]
    a190: 447b         	add	r3, pc
    a192: 9401         	str	r4, [sp, #0x4]
    a194: f503 73b8    	add.w	r3, r3, #0x170
    a198: 6808         	ldr	r0, [r1]
    a19a: f8df 1a0c    	ldr.w	r1, [pc, #0xa0c]        @ 0xaba8 <gst_lg_audio_render_event+0x2968>
    a19e: f8df 2a0c    	ldr.w	r2, [pc, #0xa0c]        @ 0xabac <gst_lg_audio_render_event+0x296c>
    a1a2: 4479         	add	r1, pc
    a1a4: 9102         	str	r1, [sp, #0x8]
    a1a6: 447a         	add	r2, pc
    a1a8: 2101         	movs	r1, #0x1
    a1aa: f7f9 ed60    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6540
    a1ae: f7fe b982    	b.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x1cfc
    a1b2: f7f9 eef0    	blx	0x3f94 <gst_element_get_type@plt> @ imm = #-0x6220
    a1b6: 4601         	mov	r1, r0
    a1b8: 4630         	mov	r0, r6
    a1ba: f7f9 eee6    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x6234
    a1be: 6f40         	ldr	r0, [r0, #0x74]
    a1c0: f7f9 efda    	blx	0x4178 <gst_clock_get_time@plt> @ imm = #-0x604c
    a1c4: 4688         	mov	r8, r1
    a1c6: 4607         	mov	r7, r0
    a1c8: f7f9 eee4    	blx	0x3f94 <gst_element_get_type@plt> @ imm = #-0x6238
    a1cc: 4601         	mov	r1, r0
    a1ce: 4630         	mov	r0, r6
    a1d0: f7f9 eeda    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x624c
    a1d4: 9930         	ldr	r1, [sp, #0xc0]
    a1d6: e9d0 2c1e    	ldrd	r2, r12, [r0, #120]
    a1da: 4620         	mov	r0, r4
    a1dc: 6a0b         	ldr	r3, [r1, #0x20]
    a1de: e9d1 ab0c    	ldrd	r10, r11, [r1, #48]
    a1e2: 1aff         	subs	r7, r7, r3
    a1e4: 6a4b         	ldr	r3, [r1, #0x24]
    a1e6: e9cd ab00    	strd	r10, r11, [sp]
    a1ea: eb68 0303    	sbc.w	r3, r8, r3
    a1ee: 1aba         	subs	r2, r7, r2
    a1f0: eb63 030c    	sbc.w	r3, r3, r12
    a1f4: f7fa ffb0    	bl	0x5158 <gst_lg_audio_render_update_clock> @ imm = #-0x50a0
    a1f8: f7fe b947    	b.w	0x848a <gst_lg_audio_render_event+0x24a> @ imm = #-0x1d72
    a1fc: f504 7a16    	add.w	r10, r4, #0x258
    a200: f7f9 ef6c    	blx	0x40dc <g_get_monotonic_time@plt> @ imm = #-0x6128
    a204: 4681         	mov	r9, r0
    a206: 4650         	mov	r0, r10
    a208: 468b         	mov	r11, r1
    a20a: f7f9 ef62    	blx	0x40d0 <g_mutex_lock@plt> @ imm = #-0x613c
    a20e: f244 2340    	movw	r3, #0x4240
    a212: 4651         	mov	r1, r10
    a214: f2c0 030f    	movt	r3, #0xf
    a218: f504 7018    	add.w	r0, r4, #0x260
    a21c: eb19 0203    	adds.w	r2, r9, r3
    a220: f14b 0300    	adc	r3, r11, #0x0
    a224: f7f9 eb3c    	blx	0x38a0 <g_cond_wait_until@plt> @ imm = #-0x6988
    a228: 2801         	cmp	r0, #0x1
    a22a: f000 85c4    	beq.w	0xadb6 <gst_lg_audio_render_event+0x2b76> @ imm = #0xb88
    a22e: f8df 0980    	ldr.w	r0, [pc, #0x980]        @ 0xabb0 <gst_lg_audio_render_event+0x2970>
    a232: 4478         	add	r0, pc
    a234: 68c3         	ldr	r3, [r0, #0xc]
    a236: 681b         	ldr	r3, [r3]
    a238: 071b         	lsls	r3, r3, #0x1c
    a23a: f100 8649    	bmi.w	0xaed0 <gst_lg_audio_render_event+0x2c90> @ imm = #0xc92
    a23e: f8d8 3000    	ldr.w	r3, [r8]
    a242: 2b03         	cmp	r3, #0x3
    a244: d916         	bls	0xa274 <gst_lg_audio_render_event+0x2034> @ imm = #0x2c
    a246: f8df 38b8    	ldr.w	r3, [pc, #0x8b8]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a24a: 2104         	movs	r1, #0x4
    a24c: f8df 2964    	ldr.w	r2, [pc, #0x964]        @ 0xabb4 <gst_lg_audio_render_event+0x2974>
    a250: 58fb         	ldr	r3, [r7, r3]
    a252: 447a         	add	r2, pc
    a254: 9401         	str	r4, [sp, #0x4]
    a256: 6818         	ldr	r0, [r3]
    a258: f8df 395c    	ldr.w	r3, [pc, #0x95c]        @ 0xabb8 <gst_lg_audio_render_event+0x2978>
    a25c: 9202         	str	r2, [sp, #0x8]
    a25e: f640 02e1    	movw	r2, #0x8e1
    a262: 9200         	str	r2, [sp]
    a264: f8df 2954    	ldr.w	r2, [pc, #0x954]        @ 0xabbc <gst_lg_audio_render_event+0x297c>
    a268: 447b         	add	r3, pc
    a26a: f503 73b8    	add.w	r3, r3, #0x170
    a26e: 447a         	add	r2, pc
    a270: f7f9 ecfc    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6608
    a274: 4650         	mov	r0, r10
    a276: f7f9 ed0c    	blx	0x3c90 <g_mutex_unlock@plt> @ imm = #-0x65e8
    a27a: f7fe bdc4    	b.w	0x8e06 <gst_lg_audio_render_event+0xbc6> @ imm = #-0x1478
    a27e: 9001         	str	r0, [sp, #0x4]
    a280: 4618         	mov	r0, r3
    a282: f8df 293c    	ldr.w	r2, [pc, #0x93c]        @ 0xabc0 <gst_lg_audio_render_event+0x2980>
    a286: 2103         	movs	r1, #0x3
    a288: f8df 3938    	ldr.w	r3, [pc, #0x938]        @ 0xabc4 <gst_lg_audio_render_event+0x2984>
    a28c: 447a         	add	r2, pc
    a28e: 447b         	add	r3, pc
    a290: f502 72c6    	add.w	r2, r2, #0x18c
    a294: 9300         	str	r3, [sp]
    a296: f240 4372    	movw	r3, #0x472
    a29a: f7f9 ee88    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x62f0
    a29e: f7fe bafa    	b.w	0x8896 <gst_lg_audio_render_event+0x656> @ imm = #-0x1a0c
    a2a2: 9b1a         	ldr	r3, [sp, #0x68]
    a2a4: 2103         	movs	r1, #0x3
    a2a6: f8df 2920    	ldr.w	r2, [pc, #0x920]        @ 0xabc8 <gst_lg_audio_render_event+0x2988>
    a2aa: 9301         	str	r3, [sp, #0x4]
    a2ac: f8df 391c    	ldr.w	r3, [pc, #0x91c]        @ 0xabcc <gst_lg_audio_render_event+0x298c>
    a2b0: 447a         	add	r2, pc
    a2b2: f502 72c6    	add.w	r2, r2, #0x18c
    a2b6: 447b         	add	r3, pc
    a2b8: 9300         	str	r3, [sp]
    a2ba: f240 437e    	movw	r3, #0x47e
    a2be: f7f9 ee76    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x6314
    a2c2: f7fe bb24    	b.w	0x890e <gst_lg_audio_render_event+0x6ce> @ imm = #-0x19b8
    a2c6: f8df 2908    	ldr.w	r2, [pc, #0x908]        @ 0xabd0 <gst_lg_audio_render_event+0x2990>
    a2ca: 4649         	mov	r1, r9
    a2cc: f8df 3904    	ldr.w	r3, [pc, #0x904]        @ 0xabd4 <gst_lg_audio_render_event+0x2994>
    a2d0: 447a         	add	r2, pc
    a2d2: 447b         	add	r3, pc
    a2d4: f502 72c6    	add.w	r2, r2, #0x18c
    a2d8: 9300         	str	r3, [sp]
    a2da: f240 4379    	movw	r3, #0x479
    a2de: f7f9 ee66    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x6334
    a2e2: e6e5         	b	0xa0b0 <gst_lg_audio_render_event+0x1e70> @ imm = #-0x236
    a2e4: f8df 28f0    	ldr.w	r2, [pc, #0x8f0]        @ 0xabd8 <gst_lg_audio_render_event+0x2998>
    a2e8: 2103         	movs	r1, #0x3
    a2ea: f8df 38f0    	ldr.w	r3, [pc, #0x8f0]        @ 0xabdc <gst_lg_audio_render_event+0x299c>
    a2ee: 447a         	add	r2, pc
    a2f0: 447b         	add	r3, pc
    a2f2: f502 72c6    	add.w	r2, r2, #0x18c
    a2f6: 9300         	str	r3, [sp]
    a2f8: f240 4387    	movw	r3, #0x487
    a2fc: f7f9 ee56    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x6354
    a300: f7fe bb61    	b.w	0x89c6 <gst_lg_audio_render_event+0x786> @ imm = #-0x193e
    a304: 4601         	mov	r1, r0
    a306: f8df 28d8    	ldr.w	r2, [pc, #0x8d8]        @ 0xabe0 <gst_lg_audio_render_event+0x29a0>
    a30a: 4618         	mov	r0, r3
    a30c: f8df 38d4    	ldr.w	r3, [pc, #0x8d4]        @ 0xabe4 <gst_lg_audio_render_event+0x29a4>
    a310: 447a         	add	r2, pc
    a312: 447b         	add	r3, pc
    a314: f502 72b8    	add.w	r2, r2, #0x170
    a318: 9300         	str	r3, [sp]
    a31a: f640 1343    	movw	r3, #0x943
    a31e: f7f9 ee46    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x6374
    a322: e728         	b	0xa176 <gst_lg_audio_render_event+0x1f36> @ imm = #-0x1b0
    a324: f8df 38c0    	ldr.w	r3, [pc, #0x8c0]        @ 0xabe8 <gst_lg_audio_render_event+0x29a8>
    a328: 447b         	add	r3, pc
    a32a: f7fe ba1a    	b.w	0x8762 <gst_lg_audio_render_event+0x522> @ imm = #-0x1bcc
    a32e: f8df 38bc    	ldr.w	r3, [pc, #0x8bc]        @ 0xabec <gst_lg_audio_render_event+0x29ac>
    a332: 447b         	add	r3, pc
    a334: f7fe b9f3    	b.w	0x871e <gst_lg_audio_render_event+0x4de> @ imm = #-0x1c1a
    a338: f8df 28b4    	ldr.w	r2, [pc, #0x8b4]        @ 0xabf0 <gst_lg_audio_render_event+0x29b0>
    a33c: 2104         	movs	r1, #0x4
    a33e: f8df 38b4    	ldr.w	r3, [pc, #0x8b4]        @ 0xabf4 <gst_lg_audio_render_event+0x29b4>
    a342: f8cd b004    	str.w	r11, [sp, #0x4]
    a346: 447a         	add	r2, pc
    a348: 447b         	add	r3, pc
    a34a: f502 72c6    	add.w	r2, r2, #0x18c
    a34e: 9300         	str	r3, [sp]
    a350: f240 438f    	movw	r3, #0x48f
    a354: f7f9 ee2a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x63ac
    a358: f7fe be77    	b.w	0x904a <gst_lg_audio_render_event+0xe0a> @ imm = #-0x1312
    a35c: f8df 2898    	ldr.w	r2, [pc, #0x898]        @ 0xabf8 <gst_lg_audio_render_event+0x29b8>
    a360: 2102         	movs	r1, #0x2
    a362: f8df 3898    	ldr.w	r3, [pc, #0x898]        @ 0xabfc <gst_lg_audio_render_event+0x29bc>
    a366: 447a         	add	r2, pc
    a368: 447b         	add	r3, pc
    a36a: f502 72b8    	add.w	r2, r2, #0x170
    a36e: 9300         	str	r3, [sp]
    a370: f640 03d6    	movw	r3, #0x8d6
    a374: f7f9 ee1a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x63cc
    a378: f8d8 3000    	ldr.w	r3, [r8]
    a37c: 2b03         	cmp	r3, #0x3
    a37e: f67f add7    	bls.w	0x9f30 <gst_lg_audio_render_event+0x1cf0> @ imm = #-0x452
    a382: e5a9         	b	0x9ed8 <gst_lg_audio_render_event+0x1c98> @ imm = #-0x4ae
    a384: f8df 3878    	ldr.w	r3, [pc, #0x878]        @ 0xac00 <gst_lg_audio_render_event+0x29c0>
    a388: 447b         	add	r3, pc
    a38a: 68da         	ldr	r2, [r3, #0xc]
    a38c: 6812         	ldr	r2, [r2]
    a38e: 07d6         	lsls	r6, r2, #0x1f
    a390: f100 86be    	bmi.w	0xb110 <gst_lg_audio_render_event+0x2ed0> @ imm = #0xd7c
    a394: f8d8 3000    	ldr.w	r3, [r8]
    a398: 2b00         	cmp	r3, #0x0
    a39a: f43f aecb    	beq.w	0xa134 <gst_lg_audio_render_event+0x1ef4> @ imm = #-0x26a
    a39e: f8df 3760    	ldr.w	r3, [pc, #0x760]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a3a2: 2101         	movs	r1, #0x1
    a3a4: f8df 285c    	ldr.w	r2, [pc, #0x85c]        @ 0xac04 <gst_lg_audio_render_event+0x29c4>
    a3a8: 58fb         	ldr	r3, [r7, r3]
    a3aa: 447a         	add	r2, pc
    a3ac: f8cd b00c    	str.w	r11, [sp, #0xc]
    a3b0: 9401         	str	r4, [sp, #0x4]
    a3b2: 6818         	ldr	r0, [r3]
    a3b4: 9202         	str	r2, [sp, #0x8]
    a3b6: f240 4292    	movw	r2, #0x492
    a3ba: f8df 384c    	ldr.w	r3, [pc, #0x84c]        @ 0xac08 <gst_lg_audio_render_event+0x29c8>
    a3be: 9200         	str	r2, [sp]
    a3c0: f8df 2848    	ldr.w	r2, [pc, #0x848]        @ 0xac0c <gst_lg_audio_render_event+0x29cc>
    a3c4: 447b         	add	r3, pc
    a3c6: f503 73c6    	add.w	r3, r3, #0x18c
    a3ca: 447a         	add	r2, pc
    a3cc: f7f9 ec4e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6764
    a3d0: e689         	b	0xa0e6 <gst_lg_audio_render_event+0x1ea6> @ imm = #-0x2ee
    a3d2: f8df 083c    	ldr.w	r0, [pc, #0x83c]        @ 0xac10 <gst_lg_audio_render_event+0x29d0>
    a3d6: 4478         	add	r0, pc
    a3d8: 68c3         	ldr	r3, [r0, #0xc]
    a3da: 681b         	ldr	r3, [r3]
    a3dc: 071b         	lsls	r3, r3, #0x1c
    a3de: f100 871d    	bmi.w	0xb21c <gst_lg_audio_render_event+0x2fdc> @ imm = #0xe3a
    a3e2: f8d8 3000    	ldr.w	r3, [r8]
    a3e6: 2b03         	cmp	r3, #0x3
    a3e8: d916         	bls	0xa418 <gst_lg_audio_render_event+0x21d8> @ imm = #0x2c
    a3ea: f8df 3714    	ldr.w	r3, [pc, #0x714]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a3ee: 2104         	movs	r1, #0x4
    a3f0: f8df 2820    	ldr.w	r2, [pc, #0x820]        @ 0xac14 <gst_lg_audio_render_event+0x29d4>
    a3f4: 58fb         	ldr	r3, [r7, r3]
    a3f6: 447a         	add	r2, pc
    a3f8: 9401         	str	r4, [sp, #0x4]
    a3fa: 6818         	ldr	r0, [r3]
    a3fc: f8df 3818    	ldr.w	r3, [pc, #0x818]        @ 0xac18 <gst_lg_audio_render_event+0x29d8>
    a400: 9202         	str	r2, [sp, #0x8]
    a402: f240 5214    	movw	r2, #0x514
    a406: 9200         	str	r2, [sp]
    a408: f8df 2810    	ldr.w	r2, [pc, #0x810]        @ 0xac1c <gst_lg_audio_render_event+0x29dc>
    a40c: 447b         	add	r3, pc
    a40e: f503 73c6    	add.w	r3, r3, #0x18c
    a412: 447a         	add	r2, pc
    a414: f7f9 ec2a    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x67ac
    a418: 2300         	movs	r3, #0x0
    a41a: f8df 1804    	ldr.w	r1, [pc, #0x804]        @ 0xac20 <gst_lg_audio_render_event+0x29e0>
    a41e: 9300         	str	r3, [sp]
    a420: 2240         	movs	r2, #0x40
    a422: 933a         	str	r3, [sp, #0xe8]
    a424: 4650         	mov	r0, r10
    a426: f8df 37fc    	ldr.w	r3, [pc, #0x7fc]        @ 0xac24 <gst_lg_audio_render_event+0x29e4>
    a42a: 4479         	add	r1, pc
    a42c: 447b         	add	r3, pc
    a42e: f7f9 ef78    	blx	0x4320 <gst_structure_set@plt> @ imm = #-0x6110
    a432: f7ff b8c2    	b.w	0x95ba <gst_lg_audio_render_event+0x137a> @ imm = #-0xe7c
    a436: f8df 07f0    	ldr.w	r0, [pc, #0x7f0]        @ 0xac28 <gst_lg_audio_render_event+0x29e8>
    a43a: 4478         	add	r0, pc
    a43c: 68c3         	ldr	r3, [r0, #0xc]
    a43e: 681b         	ldr	r3, [r3]
    a440: 06d9         	lsls	r1, r3, #0x1b
    a442: f100 874c    	bmi.w	0xb2de <gst_lg_audio_render_event+0x309e> @ imm = #0xe98
    a446: f8d8 3000    	ldr.w	r3, [r8]
    a44a: 2b04         	cmp	r3, #0x4
    a44c: d916         	bls	0xa47c <gst_lg_audio_render_event+0x223c> @ imm = #0x2c
    a44e: f8df 36b0    	ldr.w	r3, [pc, #0x6b0]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a452: 2105         	movs	r1, #0x5
    a454: f8df 27d4    	ldr.w	r2, [pc, #0x7d4]        @ 0xac2c <gst_lg_audio_render_event+0x29ec>
    a458: 58fb         	ldr	r3, [r7, r3]
    a45a: 447a         	add	r2, pc
    a45c: 9401         	str	r4, [sp, #0x4]
    a45e: 6818         	ldr	r0, [r3]
    a460: f8df 37cc    	ldr.w	r3, [pc, #0x7cc]        @ 0xac30 <gst_lg_audio_render_event+0x29f0>
    a464: 9202         	str	r2, [sp, #0x8]
    a466: f240 42f9    	movw	r2, #0x4f9
    a46a: 9200         	str	r2, [sp]
    a46c: f8df 27c4    	ldr.w	r2, [pc, #0x7c4]        @ 0xac34 <gst_lg_audio_render_event+0x29f4>
    a470: 447b         	add	r3, pc
    a472: f503 73c6    	add.w	r3, r3, #0x18c
    a476: 447a         	add	r2, pc
    a478: f7f9 ebf8    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6810
    a47c: 2300         	movs	r3, #0x0
    a47e: 9338         	str	r3, [sp, #0xe0]
    a480: f7fe bff2    	b.w	0x9468 <gst_lg_audio_render_event+0x1228> @ imm = #-0x101c
    a484: f8df 07b0    	ldr.w	r0, [pc, #0x7b0]        @ 0xac38 <gst_lg_audio_render_event+0x29f8>
    a488: 4478         	add	r0, pc
    a48a: 68c3         	ldr	r3, [r0, #0xc]
    a48c: 681b         	ldr	r3, [r3]
    a48e: 06d9         	lsls	r1, r3, #0x1b
    a490: f100 8704    	bmi.w	0xb29c <gst_lg_audio_render_event+0x305c> @ imm = #0xe08
    a494: f8d8 3000    	ldr.w	r3, [r8]
    a498: 2b04         	cmp	r3, #0x4
    a49a: d916         	bls	0xa4ca <gst_lg_audio_render_event+0x228a> @ imm = #0x2c
    a49c: f8df 3660    	ldr.w	r3, [pc, #0x660]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a4a0: 2105         	movs	r1, #0x5
    a4a2: f8df 2798    	ldr.w	r2, [pc, #0x798]        @ 0xac3c <gst_lg_audio_render_event+0x29fc>
    a4a6: 58fb         	ldr	r3, [r7, r3]
    a4a8: 447a         	add	r2, pc
    a4aa: 9401         	str	r4, [sp, #0x4]
    a4ac: 6818         	ldr	r0, [r3]
    a4ae: f8df 3790    	ldr.w	r3, [pc, #0x790]        @ 0xac40 <gst_lg_audio_render_event+0x2a00>
    a4b2: 9202         	str	r2, [sp, #0x8]
    a4b4: f240 42f1    	movw	r2, #0x4f1
    a4b8: 9200         	str	r2, [sp]
    a4ba: f8df 2788    	ldr.w	r2, [pc, #0x788]        @ 0xac44 <gst_lg_audio_render_event+0x2a04>
    a4be: 447b         	add	r3, pc
    a4c0: f503 73c6    	add.w	r3, r3, #0x18c
    a4c4: 447a         	add	r2, pc
    a4c6: f7f9 ebd2    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x685c
    a4ca: 2300         	movs	r3, #0x0
    a4cc: 9337         	str	r3, [sp, #0xdc]
    a4ce: f7fe bf9a    	b.w	0x9406 <gst_lg_audio_render_event+0x11c6> @ imm = #-0x10cc
    a4d2: f8df 0774    	ldr.w	r0, [pc, #0x774]        @ 0xac48 <gst_lg_audio_render_event+0x2a08>
    a4d6: 4478         	add	r0, pc
    a4d8: 68c3         	ldr	r3, [r0, #0xc]
    a4da: 681b         	ldr	r3, [r3]
    a4dc: 06db         	lsls	r3, r3, #0x1b
    a4de: f100 864c    	bmi.w	0xb17a <gst_lg_audio_render_event+0x2f3a> @ imm = #0xc98
    a4e2: f8d8 3000    	ldr.w	r3, [r8]
    a4e6: 2b04         	cmp	r3, #0x4
    a4e8: d916         	bls	0xa518 <gst_lg_audio_render_event+0x22d8> @ imm = #0x2c
    a4ea: f8df 3614    	ldr.w	r3, [pc, #0x614]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a4ee: 2105         	movs	r1, #0x5
    a4f0: f8df 2758    	ldr.w	r2, [pc, #0x758]        @ 0xac4c <gst_lg_audio_render_event+0x2a0c>
    a4f4: 58fb         	ldr	r3, [r7, r3]
    a4f6: 447a         	add	r2, pc
    a4f8: 9401         	str	r4, [sp, #0x4]
    a4fa: 6818         	ldr	r0, [r3]
    a4fc: f8df 3750    	ldr.w	r3, [pc, #0x750]        @ 0xac50 <gst_lg_audio_render_event+0x2a10>
    a500: 9202         	str	r2, [sp, #0x8]
    a502: f240 5201    	movw	r2, #0x501
    a506: 9200         	str	r2, [sp]
    a508: f8df 2748    	ldr.w	r2, [pc, #0x748]        @ 0xac54 <gst_lg_audio_render_event+0x2a14>
    a50c: 447b         	add	r3, pc
    a50e: f503 73c6    	add.w	r3, r3, #0x18c
    a512: 447a         	add	r2, pc
    a514: f7f9 ebaa    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x68ac
    a518: 2300         	movs	r3, #0x0
    a51a: 9339         	str	r3, [sp, #0xe4]
    a51c: f7fe bfd5    	b.w	0x94ca <gst_lg_audio_render_event+0x128a> @ imm = #-0x1056
    a520: 6812         	ldr	r2, [r2]
    a522: 07d2         	lsls	r2, r2, #0x1f
    a524: f100 86aa    	bmi.w	0xb27c <gst_lg_audio_render_event+0x303c> @ imm = #0xd54
    a528: f8d8 3000    	ldr.w	r3, [r8]
    a52c: 2b00         	cmp	r3, #0x0
    a52e: f43e aed7    	beq.w	0x92e0 <gst_lg_audio_render_event+0x10a0> @ imm = #-0x1252
    a532: f8df 35cc    	ldr.w	r3, [pc, #0x5cc]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a536: 2101         	movs	r1, #0x1
    a538: f8df 271c    	ldr.w	r2, [pc, #0x71c]        @ 0xac58 <gst_lg_audio_render_event+0x2a18>
    a53c: 58fb         	ldr	r3, [r7, r3]
    a53e: 447a         	add	r2, pc
    a540: 9401         	str	r4, [sp, #0x4]
    a542: 6818         	ldr	r0, [r3]
    a544: 9202         	str	r2, [sp, #0x8]
    a546: f240 42c2    	movw	r2, #0x4c2
    a54a: f8df 3710    	ldr.w	r3, [pc, #0x710]        @ 0xac5c <gst_lg_audio_render_event+0x2a1c>
    a54e: 9200         	str	r2, [sp]
    a550: f8df 270c    	ldr.w	r2, [pc, #0x70c]        @ 0xac60 <gst_lg_audio_render_event+0x2a20>
    a554: 447b         	add	r3, pc
    a556: f503 73c6    	add.w	r3, r3, #0x18c
    a55a: 447a         	add	r2, pc
    a55c: f7f9 eb86    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x68f4
    a560: f7fe bebe    	b.w	0x92e0 <gst_lg_audio_render_event+0x10a0> @ imm = #-0x1284
    a564: f8df 06fc    	ldr.w	r0, [pc, #0x6fc]        @ 0xac64 <gst_lg_audio_render_event+0x2a24>
    a568: 4478         	add	r0, pc
    a56a: 68c3         	ldr	r3, [r0, #0xc]
    a56c: 681b         	ldr	r3, [r3]
    a56e: 06d9         	lsls	r1, r3, #0x1b
    a570: f100 8624    	bmi.w	0xb1bc <gst_lg_audio_render_event+0x2f7c> @ imm = #0xc48
    a574: f8d8 3000    	ldr.w	r3, [r8]
    a578: 2b04         	cmp	r3, #0x4
    a57a: d916         	bls	0xa5aa <gst_lg_audio_render_event+0x236a> @ imm = #0x2c
    a57c: f8df 3580    	ldr.w	r3, [pc, #0x580]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a580: 2105         	movs	r1, #0x5
    a582: f8df 26e4    	ldr.w	r2, [pc, #0x6e4]        @ 0xac68 <gst_lg_audio_render_event+0x2a28>
    a586: 58fb         	ldr	r3, [r7, r3]
    a588: 447a         	add	r2, pc
    a58a: 9401         	str	r4, [sp, #0x4]
    a58c: 6818         	ldr	r0, [r3]
    a58e: f8df 36dc    	ldr.w	r3, [pc, #0x6dc]        @ 0xac6c <gst_lg_audio_render_event+0x2a2c>
    a592: 9202         	str	r2, [sp, #0x8]
    a594: f240 42e9    	movw	r2, #0x4e9
    a598: 9200         	str	r2, [sp]
    a59a: f8df 26d4    	ldr.w	r2, [pc, #0x6d4]        @ 0xac70 <gst_lg_audio_render_event+0x2a30>
    a59e: 447b         	add	r3, pc
    a5a0: f503 73c6    	add.w	r3, r3, #0x18c
    a5a4: 447a         	add	r2, pc
    a5a6: f7f9 eb62    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x693c
    a5aa: f04f 33ff    	mov.w	r3, #0xffffffff
    a5ae: 9336         	str	r3, [sp, #0xd8]
    a5b0: f7fe bef8    	b.w	0x93a4 <gst_lg_audio_render_event+0x1164> @ imm = #-0x1210
    a5b4: f89d 30c8    	ldrb.w	r3, [sp, #0xc8]
    a5b8: 2b0a         	cmp	r3, #0xa
    a5ba: f63f aa4d    	bhi.w	0x9a58 <gst_lg_audio_render_event+0x1818> @ imm = #-0xb66
    a5be: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a5c2: aa2b         	add	r2, sp, #0xac
    a5c4: 2106         	movs	r1, #0x6
    a5c6: f7f9 ebdc    	blx	0x3d80 <lgao_get_audio_info@plt> @ imm = #-0x6848
    a5ca: b1d8         	cbz	r0, 0xa604 <gst_lg_audio_render_event+0x23c4> @ imm = #0x36
    a5cc: f8d8 3000    	ldr.w	r3, [r8]
    a5d0: 2b01         	cmp	r3, #0x1
    a5d2: d917         	bls	0xa604 <gst_lg_audio_render_event+0x23c4> @ imm = #0x2e
    a5d4: f8df 3528    	ldr.w	r3, [pc, #0x528]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a5d8: 2102         	movs	r1, #0x2
    a5da: f8df 2698    	ldr.w	r2, [pc, #0x698]        @ 0xac74 <gst_lg_audio_render_event+0x2a34>
    a5de: 58fb         	ldr	r3, [r7, r3]
    a5e0: 447a         	add	r2, pc
    a5e2: f8cd 9004    	str.w	r9, [sp, #0x4]
    a5e6: 6818         	ldr	r0, [r3]
    a5e8: f8df 368c    	ldr.w	r3, [pc, #0x68c]        @ 0xac78 <gst_lg_audio_render_event+0x2a38>
    a5ec: 9202         	str	r2, [sp, #0x8]
    a5ee: f240 528f    	movw	r2, #0x58f
    a5f2: 9200         	str	r2, [sp]
    a5f4: f8df 2684    	ldr.w	r2, [pc, #0x684]        @ 0xac7c <gst_lg_audio_render_event+0x2a3c>
    a5f8: 447b         	add	r3, pc
    a5fa: f503 73c6    	add.w	r3, r3, #0x18c
    a5fe: 447a         	add	r2, pc
    a600: f7f9 eb34    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6998
    a604: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a608: 9914         	ldr	r1, [sp, #0x50]
    a60a: f7f9 ed3e    	blx	0x4088 <lgao_compare_audio_info@plt> @ imm = #-0x6584
    a60e: 1c43         	adds	r3, r0, #0x1
    a610: 4681         	mov	r9, r0
    a612: 2b03         	cmp	r3, #0x3
    a614: f200 8163    	bhi.w	0xa8de <gst_lg_audio_render_event+0x269e> @ imm = #0x2c6
    a618: e8df f013    	tbh	[pc, r3, lsl #1]
    a61c: 97 01 71 01  	.word	0x01710197
    a620: 33 01 04 00  	.word	0x00040133
    a624: f8df 0658    	ldr.w	r0, [pc, #0x658]        @ 0xac80 <gst_lg_audio_render_event+0x2a40>
    a628: 4478         	add	r0, pc
    a62a: 68c3         	ldr	r3, [r0, #0xc]
    a62c: 681b         	ldr	r3, [r3]
    a62e: 075b         	lsls	r3, r3, #0x1d
    a630: f101 812a    	bmi.w	0xb888 <gst_lg_audio_render_event+0x3648> @ imm = #0x1254
    a634: f8d8 3000    	ldr.w	r3, [r8]
    a638: 2b03         	cmp	r3, #0x3
    a63a: f240 878f    	bls.w	0xb55c <gst_lg_audio_render_event+0x331c> @ imm = #0xf1e
    a63e: f8df 34c0    	ldr.w	r3, [pc, #0x4c0]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a642: 2104         	movs	r1, #0x4
    a644: f8df 263c    	ldr.w	r2, [pc, #0x63c]        @ 0xac84 <gst_lg_audio_render_event+0x2a44>
    a648: f10d 09cc    	add.w	r9, sp, #0xcc
    a64c: 58fb         	ldr	r3, [r7, r3]
    a64e: 447a         	add	r2, pc
    a650: 9401         	str	r4, [sp, #0x4]
    a652: 6818         	ldr	r0, [r3]
    a654: 9202         	str	r2, [sp, #0x8]
    a656: f240 52b2    	movw	r2, #0x5b2
    a65a: f8df 362c    	ldr.w	r3, [pc, #0x62c]        @ 0xac88 <gst_lg_audio_render_event+0x2a48>
    a65e: 9200         	str	r2, [sp]
    a660: f8df 2628    	ldr.w	r2, [pc, #0x628]        @ 0xac8c <gst_lg_audio_render_event+0x2a4c>
    a664: 447b         	add	r3, pc
    a666: f503 73c6    	add.w	r3, r3, #0x18c
    a66a: 447a         	add	r2, pc
    a66c: f7f9 eafe    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6a04
    a670: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a674: aa2c         	add	r2, sp, #0xb0
    a676: 210e         	movs	r1, #0xe
    a678: f7f9 eb82    	blx	0x3d80 <lgao_get_audio_info@plt> @ imm = #-0x68fc
    a67c: b1e0         	cbz	r0, 0xa6b8 <gst_lg_audio_render_event+0x2478> @ imm = #0x38
    a67e: f8d8 3000    	ldr.w	r3, [r8]
    a682: 2b01         	cmp	r3, #0x1
    a684: d918         	bls	0xa6b8 <gst_lg_audio_render_event+0x2478> @ imm = #0x30
    a686: f8df 3478    	ldr.w	r3, [pc, #0x478]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a68a: f04f 0e00    	mov.w	lr, #0x0
    a68e: f8df 2600    	ldr.w	r2, [pc, #0x600]        @ 0xac90 <gst_lg_audio_render_event+0x2a50>
    a692: 2102         	movs	r1, #0x2
    a694: 58fb         	ldr	r3, [r7, r3]
    a696: 447a         	add	r2, pc
    a698: 6818         	ldr	r0, [r3]
    a69a: f8df 35f8    	ldr.w	r3, [pc, #0x5f8]        @ 0xac94 <gst_lg_audio_render_event+0x2a54>
    a69e: 9202         	str	r2, [sp, #0x8]
    a6a0: f240 52d3    	movw	r2, #0x5d3
    a6a4: e9cd 2e00    	strd	r2, lr, [sp]
    a6a8: f8df 25ec    	ldr.w	r2, [pc, #0x5ec]        @ 0xac98 <gst_lg_audio_render_event+0x2a58>
    a6ac: 447b         	add	r3, pc
    a6ae: f503 73c6    	add.w	r3, r3, #0x18c
    a6b2: 447a         	add	r2, pc
    a6b4: f7f9 eada    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6a4c
    a6b8: 4649         	mov	r1, r9
    a6ba: 2250         	movs	r2, #0x50
    a6bc: 4668         	mov	r0, sp
    a6be: f8d4 9200    	ldr.w	r9, [r4, #0x200]
    a6c2: f7f9 eb28    	blx	0x3d14 <memcpy@plt>     @ imm = #-0x69b0
    a6c6: 9b14         	ldr	r3, [sp, #0x50]
    a6c8: 4648         	mov	r0, r9
    a6ca: cb0e         	ldm	r3, {r1, r2, r3}
    a6cc: f7f9 ec4e    	blx	0x3f6c <lgao_set_audio_info@plt> @ imm = #-0x6764
    a6d0: b1e0         	cbz	r0, 0xa70c <gst_lg_audio_render_event+0x24cc> @ imm = #0x38
    a6d2: f8d8 3000    	ldr.w	r3, [r8]
    a6d6: 2b01         	cmp	r3, #0x1
    a6d8: d918         	bls	0xa70c <gst_lg_audio_render_event+0x24cc> @ imm = #0x30
    a6da: f8df 3424    	ldr.w	r3, [pc, #0x424]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a6de: f04f 0c00    	mov.w	r12, #0x0
    a6e2: f8df 25b8    	ldr.w	r2, [pc, #0x5b8]        @ 0xac9c <gst_lg_audio_render_event+0x2a5c>
    a6e6: 2102         	movs	r1, #0x2
    a6e8: 58fb         	ldr	r3, [r7, r3]
    a6ea: 447a         	add	r2, pc
    a6ec: 6818         	ldr	r0, [r3]
    a6ee: f8df 35b0    	ldr.w	r3, [pc, #0x5b0]        @ 0xaca0 <gst_lg_audio_render_event+0x2a60>
    a6f2: 9202         	str	r2, [sp, #0x8]
    a6f4: f240 52d6    	movw	r2, #0x5d6
    a6f8: e9cd 2c00    	strd	r2, r12, [sp]
    a6fc: f8df 25a4    	ldr.w	r2, [pc, #0x5a4]        @ 0xaca4 <gst_lg_audio_render_event+0x2a64>
    a700: 447b         	add	r3, pc
    a702: f503 73c6    	add.w	r3, r3, #0x18c
    a706: 447a         	add	r2, pc
    a708: f7f9 eab0    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6aa0
    a70c: 9a35         	ldr	r2, [sp, #0xd4]
    a70e: 9b2b         	ldr	r3, [sp, #0xac]
    a710: 429a         	cmp	r2, r3
    a712: f000 8499    	beq.w	0xb048 <gst_lg_audio_render_event+0x2e08> @ imm = #0x932
    a716: f8df 0590    	ldr.w	r0, [pc, #0x590]        @ 0xaca8 <gst_lg_audio_render_event+0x2a68>
    a71a: 4478         	add	r0, pc
    a71c: 68c1         	ldr	r1, [r0, #0xc]
    a71e: 6809         	ldr	r1, [r1]
    a720: 0709         	lsls	r1, r1, #0x1c
    a722: f100 8721    	bmi.w	0xb568 <gst_lg_audio_render_event+0x3328> @ imm = #0xe42
    a726: f8d8 3000    	ldr.w	r3, [r8]
    a72a: 2b03         	cmp	r3, #0x3
    a72c: f240 839d    	bls.w	0xae6a <gst_lg_audio_render_event+0x2c2a> @ imm = #0x73a
    a730: 4bf3         	ldr	r3, [pc, #0x3cc]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a732: 2104         	movs	r1, #0x4
    a734: f8df 2574    	ldr.w	r2, [pc, #0x574]        @ 0xacac <gst_lg_audio_render_event+0x2a6c>
    a738: 58fb         	ldr	r3, [r7, r3]
    a73a: 9401         	str	r4, [sp, #0x4]
    a73c: 447a         	add	r2, pc
    a73e: 6818         	ldr	r0, [r3]
    a740: 9b35         	ldr	r3, [sp, #0xd4]
    a742: 9202         	str	r2, [sp, #0x8]
    a744: f44f 62bb    	mov.w	r2, #0x5d8
    a748: 9200         	str	r2, [sp]
    a74a: 9304         	str	r3, [sp, #0x10]
    a74c: 9b2b         	ldr	r3, [sp, #0xac]
    a74e: f8df 2560    	ldr.w	r2, [pc, #0x560]        @ 0xacb0 <gst_lg_audio_render_event+0x2a70>
    a752: 9303         	str	r3, [sp, #0xc]
    a754: f8df 355c    	ldr.w	r3, [pc, #0x55c]        @ 0xacb4 <gst_lg_audio_render_event+0x2a74>
    a758: 447a         	add	r2, pc
    a75a: 447b         	add	r3, pc
    a75c: f503 73c6    	add.w	r3, r3, #0x18c
    a760: f7f9 ea84    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6af8
    a764: 9b46         	ldr	r3, [sp, #0x118]
    a766: 9a2c         	ldr	r2, [sp, #0xb0]
    a768: 4293         	cmp	r3, r2
    a76a: d025         	beq	0xa7b8 <gst_lg_audio_render_event+0x2578> @ imm = #0x4a
    a76c: f8df 1548    	ldr.w	r1, [pc, #0x548]        @ 0xacb8 <gst_lg_audio_render_event+0x2a78>
    a770: 4479         	add	r1, pc
    a772: 68c9         	ldr	r1, [r1, #0xc]
    a774: 6809         	ldr	r1, [r1]
    a776: 0708         	lsls	r0, r1, #0x1c
    a778: f100 8384    	bmi.w	0xae84 <gst_lg_audio_render_event+0x2c44> @ imm = #0x708
    a77c: f8d8 3000    	ldr.w	r3, [r8]
    a780: 2b03         	cmp	r3, #0x3
    a782: d919         	bls	0xa7b8 <gst_lg_audio_render_event+0x2578> @ imm = #0x32
    a784: 4bde         	ldr	r3, [pc, #0x378]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a786: 2104         	movs	r1, #0x4
    a788: 9a46         	ldr	r2, [sp, #0x118]
    a78a: 58fb         	ldr	r3, [r7, r3]
    a78c: 9401         	str	r4, [sp, #0x4]
    a78e: 6818         	ldr	r0, [r3]
    a790: 9204         	str	r2, [sp, #0x10]
    a792: 9a2c         	ldr	r2, [sp, #0xb0]
    a794: f8df 3524    	ldr.w	r3, [pc, #0x524]        @ 0xacbc <gst_lg_audio_render_event+0x2a7c>
    a798: 9203         	str	r2, [sp, #0xc]
    a79a: f8df 2524    	ldr.w	r2, [pc, #0x524]        @ 0xacc0 <gst_lg_audio_render_event+0x2a80>
    a79e: 447b         	add	r3, pc
    a7a0: f503 73c6    	add.w	r3, r3, #0x18c
    a7a4: 447a         	add	r2, pc
    a7a6: 9202         	str	r2, [sp, #0x8]
    a7a8: f240 52dd    	movw	r2, #0x5dd
    a7ac: 9200         	str	r2, [sp]
    a7ae: f8df 2514    	ldr.w	r2, [pc, #0x514]        @ 0xacc4 <gst_lg_audio_render_event+0x2a84>
    a7b2: 447a         	add	r2, pc
    a7b4: f7f9 ea5a    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6b4c
    a7b8: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a7bc: f7f9 e804    	blx	0x37c8 <lgao_push_first_event@plt> @ imm = #-0x6ff8
    a7c0: 9b16         	ldr	r3, [sp, #0x58]
    a7c2: b113         	cbz	r3, 0xa7ca <gst_lg_audio_render_event+0x258a> @ imm = #0x4
    a7c4: 4618         	mov	r0, r3
    a7c6: f7f8 efdc    	blx	0x3780 <g_free@plt>     @ imm = #-0x7048
    a7ca: f8d4 3218    	ldr.w	r3, [r4, #0x218]
    a7ce: 2b04         	cmp	r3, #0x4
    a7d0: f47d ae71    	bne.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x231e
    a7d4: 9b15         	ldr	r3, [sp, #0x54]
    a7d6: 2b00         	cmp	r3, #0x0
    a7d8: f43d ae6d    	beq.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x2326
    a7dc: f8df 04e8    	ldr.w	r0, [pc, #0x4e8]        @ 0xacc8 <gst_lg_audio_render_event+0x2a88>
    a7e0: 4478         	add	r0, pc
    a7e2: 68c3         	ldr	r3, [r0, #0xc]
    a7e4: 681b         	ldr	r3, [r3]
    a7e6: 071a         	lsls	r2, r3, #0x1c
    a7e8: f101 80a4    	bmi.w	0xb934 <gst_lg_audio_render_event+0x36f4> @ imm = #0x1148
    a7ec: f8d8 3000    	ldr.w	r3, [r8]
    a7f0: 2b03         	cmp	r3, #0x3
    a7f2: d915         	bls	0xa820 <gst_lg_audio_render_event+0x25e0> @ imm = #0x2a
    a7f4: 4ac2         	ldr	r2, [pc, #0x308]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a7f6: 2104         	movs	r1, #0x4
    a7f8: f8df 34d0    	ldr.w	r3, [pc, #0x4d0]        @ 0xaccc <gst_lg_audio_render_event+0x2a8c>
    a7fc: 58ba         	ldr	r2, [r7, r2]
    a7fe: 9401         	str	r4, [sp, #0x4]
    a800: 447b         	add	r3, pc
    a802: f503 73b8    	add.w	r3, r3, #0x170
    a806: 6810         	ldr	r0, [r2]
    a808: f8df 24c4    	ldr.w	r2, [pc, #0x4c4]        @ 0xacd0 <gst_lg_audio_render_event+0x2a90>
    a80c: 447a         	add	r2, pc
    a80e: 9202         	str	r2, [sp, #0x8]
    a810: f640 122e    	movw	r2, #0x92e
    a814: 9200         	str	r2, [sp]
    a816: f8df 24bc    	ldr.w	r2, [pc, #0x4bc]        @ 0xacd4 <gst_lg_audio_render_event+0x2a94>
    a81a: 447a         	add	r2, pc
    a81c: f7f9 ea26    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6bb4
    a820: f7f9 ebb8    	blx	0x3f94 <gst_element_get_type@plt> @ imm = #-0x6890
    a824: 4601         	mov	r1, r0
    a826: 4630         	mov	r0, r6
    a828: f7f9 ebae    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x68a4
    a82c: 4681         	mov	r9, r0
    a82e: 6f40         	ldr	r0, [r0, #0x74]
    a830: f7f9 eca2    	blx	0x4178 <gst_clock_get_time@plt> @ imm = #-0x66bc
    a834: e9d9 ba1e    	ldrd	r11, r10, [r9, #120]
    a838: 4688         	mov	r8, r1
    a83a: 4607         	mov	r7, r0
    a83c: f7f9 ec30    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0x67a0
    a840: 4601         	mov	r1, r0
    a842: 4648         	mov	r0, r9
    a844: f7f9 eba0    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x68c0
    a848: f8d0 3118    	ldr.w	r3, [r0, #0x118]
    a84c: 1aff         	subs	r7, r7, r3
    a84e: f8d0 311c    	ldr.w	r3, [r0, #0x11c]
    a852: eb68 0803    	sbc.w	r8, r8, r3
    a856: ebb7 070b    	subs.w	r7, r7, r11
    a85a: eb68 080a    	sbc.w	r8, r8, r10
    a85e: f7f9 ec20    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0x67c0
    a862: 4601         	mov	r1, r0
    a864: 4648         	mov	r0, r9
    a866: f7f9 eb90    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x68e0
    a86a: 4601         	mov	r1, r0
    a86c: 4643         	mov	r3, r8
    a86e: e9d1 894a    	ldrd	r8, r9, [r1, #296]
    a872: 463a         	mov	r2, r7
    a874: 4620         	mov	r0, r4
    a876: e9cd 8900    	strd	r8, r9, [sp]
    a87a: f7fa fc6d    	bl	0x5158 <gst_lg_audio_render_update_clock> @ imm = #-0x5726
    a87e: f7fd be1a    	b.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x23cc
    a882: f8df 0454    	ldr.w	r0, [pc, #0x454]        @ 0xacd8 <gst_lg_audio_render_event+0x2a98>
    a886: 4478         	add	r0, pc
    a888: 68c3         	ldr	r3, [r0, #0xc]
    a88a: 681b         	ldr	r3, [r3]
    a88c: 075b         	lsls	r3, r3, #0x1d
    a88e: f101 8017    	bmi.w	0xb8c0 <gst_lg_audio_render_event+0x3680> @ imm = #0x102e
    a892: f8d8 3000    	ldr.w	r3, [r8]
    a896: 2b03         	cmp	r3, #0x3
    a898: d915         	bls	0xa8c6 <gst_lg_audio_render_event+0x2686> @ imm = #0x2a
    a89a: 4b99         	ldr	r3, [pc, #0x264]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a89c: 2104         	movs	r1, #0x4
    a89e: f8df 243c    	ldr.w	r2, [pc, #0x43c]        @ 0xacdc <gst_lg_audio_render_event+0x2a9c>
    a8a2: 58fb         	ldr	r3, [r7, r3]
    a8a4: 447a         	add	r2, pc
    a8a6: 9401         	str	r4, [sp, #0x4]
    a8a8: 6818         	ldr	r0, [r3]
    a8aa: f8df 3434    	ldr.w	r3, [pc, #0x434]        @ 0xace0 <gst_lg_audio_render_event+0x2aa0>
    a8ae: 9202         	str	r2, [sp, #0x8]
    a8b0: f240 5293    	movw	r2, #0x593
    a8b4: 9200         	str	r2, [sp]
    a8b6: f8df 242c    	ldr.w	r2, [pc, #0x42c]        @ 0xace4 <gst_lg_audio_render_event+0x2aa4>
    a8ba: 447b         	add	r3, pc
    a8bc: f503 73c6    	add.w	r3, r3, #0x18c
    a8c0: 447a         	add	r2, pc
    a8c2: f7f9 e9d4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6c58
    a8c6: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a8ca: f7f9 ea8a    	blx	0x3de0 <lgao_stop_renderer@plt> @ imm = #-0x6aec
    a8ce: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a8d2: f7f9 ec5e    	blx	0x4190 <lgao_disconnect_renderer@plt> @ imm = #-0x6744
    a8d6: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a8da: f7f9 e88a    	blx	0x39f0 <lgao_close_renderer@plt> @ imm = #-0x6eec
    a8de: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a8e2: f7f9 ec38    	blx	0x4154 <lgao_get_drop_renderer@plt> @ imm = #-0x6790
    a8e6: 4681         	mov	r9, r0
    a8e8: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a8ec: f1b9 0f00    	cmp.w	r9, #0x0
    a8f0: f000 8690    	beq.w	0xb614 <gst_lg_audio_render_event+0x33d4> @ imm = #0xd20
    a8f4: f10d 09cc    	add.w	r9, sp, #0xcc
    a8f8: 2301         	movs	r3, #0x1
    a8fa: 9315         	str	r3, [sp, #0x54]
    a8fc: e6ba         	b	0xa674 <gst_lg_audio_render_event+0x2434> @ imm = #-0x28c
    a8fe: 48fa         	ldr	r0, [pc, #0x3e8]        @ 0xace8 <gst_lg_audio_render_event+0x2aa8>
    a900: 4478         	add	r0, pc
    a902: 68c3         	ldr	r3, [r0, #0xc]
    a904: 681b         	ldr	r3, [r3]
    a906: 071a         	lsls	r2, r3, #0x1c
    a908: f100 87cc    	bmi.w	0xb8a4 <gst_lg_audio_render_event+0x3664> @ imm = #0xf98
    a90c: f8d8 3000    	ldr.w	r3, [r8]
    a910: 2b03         	cmp	r3, #0x3
    a912: f240 8623    	bls.w	0xb55c <gst_lg_audio_render_event+0x331c> @ imm = #0xc46
    a916: 4b7a         	ldr	r3, [pc, #0x1e8]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a918: 2104         	movs	r1, #0x4
    a91a: f8cd 9054    	str.w	r9, [sp, #0x54]
    a91e: f10d 09cc    	add.w	r9, sp, #0xcc
    a922: 4af2         	ldr	r2, [pc, #0x3c8]        @ 0xacec <gst_lg_audio_render_event+0x2aac>
    a924: 58fb         	ldr	r3, [r7, r3]
    a926: 9401         	str	r4, [sp, #0x4]
    a928: 447a         	add	r2, pc
    a92a: 6818         	ldr	r0, [r3]
    a92c: 9202         	str	r2, [sp, #0x8]
    a92e: f240 52ad    	movw	r2, #0x5ad
    a932: 4bef         	ldr	r3, [pc, #0x3bc]        @ 0xacf0 <gst_lg_audio_render_event+0x2ab0>
    a934: 9200         	str	r2, [sp]
    a936: 4aef         	ldr	r2, [pc, #0x3bc]        @ 0xacf4 <gst_lg_audio_render_event+0x2ab4>
    a938: 447b         	add	r3, pc
    a93a: f503 73c6    	add.w	r3, r3, #0x18c
    a93e: 447a         	add	r2, pc
    a940: f7f9 e994    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6cd8
    a944: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a948: e694         	b	0xa674 <gst_lg_audio_render_event+0x2434> @ imm = #-0x2d8
    a94a: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a94e: f04f 33ff    	mov.w	r3, #0xffffffff
    a952: a92e         	add	r1, sp, #0xb8
    a954: 932e         	str	r3, [sp, #0xb8]
    a956: f7f9 ecc6    	blx	0x42e4 <lgao_get_port@plt> @ imm = #-0x6674
    a95a: b1c0         	cbz	r0, 0xa98e <gst_lg_audio_render_event+0x274e> @ imm = #0x30
    a95c: f8d8 3000    	ldr.w	r3, [r8]
    a960: 2b01         	cmp	r3, #0x1
    a962: d914         	bls	0xa98e <gst_lg_audio_render_event+0x274e> @ imm = #0x28
    a964: 4b66         	ldr	r3, [pc, #0x198]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a966: f04f 0c00    	mov.w	r12, #0x0
    a96a: 4ae3         	ldr	r2, [pc, #0x38c]        @ 0xacf8 <gst_lg_audio_render_event+0x2ab8>
    a96c: 2102         	movs	r1, #0x2
    a96e: 58fb         	ldr	r3, [r7, r3]
    a970: 447a         	add	r2, pc
    a972: 6818         	ldr	r0, [r3]
    a974: 4be1         	ldr	r3, [pc, #0x384]        @ 0xacfc <gst_lg_audio_render_event+0x2abc>
    a976: 9202         	str	r2, [sp, #0x8]
    a978: f240 529f    	movw	r2, #0x59f
    a97c: e9cd 2c00    	strd	r2, r12, [sp]
    a980: 4adf         	ldr	r2, [pc, #0x37c]        @ 0xad00 <gst_lg_audio_render_event+0x2ac0>
    a982: 447b         	add	r3, pc
    a984: f503 73c6    	add.w	r3, r3, #0x18c
    a988: 447a         	add	r2, pc
    a98a: f7f9 e970    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6d20
    a98e: 9b2e         	ldr	r3, [sp, #0xb8]
    a990: 2b00         	cmp	r3, #0x0
    a992: daa4         	bge	0xa8de <gst_lg_audio_render_event+0x269e> @ imm = #-0xb8
    a994: 48db         	ldr	r0, [pc, #0x36c]        @ 0xad04 <gst_lg_audio_render_event+0x2ac4>
    a996: 4478         	add	r0, pc
    a998: 68c2         	ldr	r2, [r0, #0xc]
    a99a: 6812         	ldr	r2, [r2]
    a99c: 0751         	lsls	r1, r2, #0x1d
    a99e: f100 87ba    	bmi.w	0xb916 <gst_lg_audio_render_event+0x36d6> @ imm = #0xf74
    a9a2: f8d8 3000    	ldr.w	r3, [r8]
    a9a6: 2b03         	cmp	r3, #0x3
    a9a8: f240 85d8    	bls.w	0xb55c <gst_lg_audio_render_event+0x331c> @ imm = #0xbb0
    a9ac: 4b54         	ldr	r3, [pc, #0x150]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a9ae: 2104         	movs	r1, #0x4
    a9b0: 4ad5         	ldr	r2, [pc, #0x354]        @ 0xad08 <gst_lg_audio_render_event+0x2ac8>
    a9b2: f10d 09cc    	add.w	r9, sp, #0xcc
    a9b6: 58fb         	ldr	r3, [r7, r3]
    a9b8: 9401         	str	r4, [sp, #0x4]
    a9ba: 447a         	add	r2, pc
    a9bc: 6818         	ldr	r0, [r3]
    a9be: 9b2e         	ldr	r3, [sp, #0xb8]
    a9c0: 9202         	str	r2, [sp, #0x8]
    a9c2: f240 52a5    	movw	r2, #0x5a5
    a9c6: 9200         	str	r2, [sp]
    a9c8: 9303         	str	r3, [sp, #0xc]
    a9ca: 4bd0         	ldr	r3, [pc, #0x340]        @ 0xad0c <gst_lg_audio_render_event+0x2acc>
    a9cc: 4ad0         	ldr	r2, [pc, #0x340]        @ 0xad10 <gst_lg_audio_render_event+0x2ad0>
    a9ce: 447b         	add	r3, pc
    a9d0: f503 73c6    	add.w	r3, r3, #0x18c
    a9d4: 447a         	add	r2, pc
    a9d6: f7f9 e94a    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6d6c
    a9da: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    a9de: e649         	b	0xa674 <gst_lg_audio_render_event+0x2434> @ imm = #-0x36e
    a9e0: 48cc         	ldr	r0, [pc, #0x330]        @ 0xad14 <gst_lg_audio_render_event+0x2ad4>
    a9e2: 4478         	add	r0, pc
    a9e4: 68c3         	ldr	r3, [r0, #0xc]
    a9e6: 681b         	ldr	r3, [r3]
    a9e8: 06da         	lsls	r2, r3, #0x1b
    a9ea: f100 8552    	bmi.w	0xb492 <gst_lg_audio_render_event+0x3252> @ imm = #0xaa4
    a9ee: f8d8 3000    	ldr.w	r3, [r8]
    a9f2: 2b04         	cmp	r3, #0x4
    a9f4: d912         	bls	0xaa1c <gst_lg_audio_render_event+0x27dc> @ imm = #0x24
    a9f6: 4b42         	ldr	r3, [pc, #0x108]        @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    a9f8: 2105         	movs	r1, #0x5
    a9fa: 4ac7         	ldr	r2, [pc, #0x31c]        @ 0xad18 <gst_lg_audio_render_event+0x2ad8>
    a9fc: 58fb         	ldr	r3, [r7, r3]
    a9fe: 447a         	add	r2, pc
    aa00: 9401         	str	r4, [sp, #0x4]
    aa02: 6818         	ldr	r0, [r3]
    aa04: 4bc5         	ldr	r3, [pc, #0x314]        @ 0xad1c <gst_lg_audio_render_event+0x2adc>
    aa06: 9202         	str	r2, [sp, #0x8]
    aa08: f240 5271    	movw	r2, #0x571
    aa0c: 9200         	str	r2, [sp]
    aa0e: 4ac4         	ldr	r2, [pc, #0x310]        @ 0xad20 <gst_lg_audio_render_event+0x2ae0>
    aa10: 447b         	add	r3, pc
    aa12: f503 73c6    	add.w	r3, r3, #0x18c
    aa16: 447a         	add	r2, pc
    aa18: f7f9 e928    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6db0
    aa1c: 2300         	movs	r3, #0x0
    aa1e: 9346         	str	r3, [sp, #0x118]
    aa20: f7fe bfe8    	b.w	0x99f4 <gst_lg_audio_render_event+0x17b4> @ imm = #-0x1030
    aa24: 48bf         	ldr	r0, [pc, #0x2fc]        @ 0xad24 <gst_lg_audio_render_event+0x2ae4>
    aa26: 4478         	add	r0, pc
    aa28: 68c3         	ldr	r3, [r0, #0xc]
    aa2a: 681b         	ldr	r3, [r3]
    aa2c: 06db         	lsls	r3, r3, #0x1b
    aa2e: f100 850f    	bmi.w	0xb450 <gst_lg_audio_render_event+0x3210> @ imm = #0xa1e
    aa32: f8d8 3000    	ldr.w	r3, [r8]
    aa36: 2b04         	cmp	r3, #0x4
    aa38: d912         	bls	0xaa60 <gst_lg_audio_render_event+0x2820> @ imm = #0x24
    aa3a: 4b31         	ldr	r3, [pc, #0xc4]         @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    aa3c: 2105         	movs	r1, #0x5
    aa3e: 4aba         	ldr	r2, [pc, #0x2e8]        @ 0xad28 <gst_lg_audio_render_event+0x2ae8>
    aa40: 58fb         	ldr	r3, [r7, r3]
    aa42: 447a         	add	r2, pc
    aa44: 9401         	str	r4, [sp, #0x4]
    aa46: 6818         	ldr	r0, [r3]
    aa48: 4bb8         	ldr	r3, [pc, #0x2e0]        @ 0xad2c <gst_lg_audio_render_event+0x2aec>
    aa4a: 9202         	str	r2, [sp, #0x8]
    aa4c: f240 524f    	movw	r2, #0x54f
    aa50: 9200         	str	r2, [sp]
    aa52: 4ab7         	ldr	r2, [pc, #0x2dc]        @ 0xad30 <gst_lg_audio_render_event+0x2af0>
    aa54: 447b         	add	r3, pc
    aa56: f503 73c6    	add.w	r3, r3, #0x18c
    aa5a: 447a         	add	r2, pc
    aa5c: f7f9 e906    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6df4
    aa60: 2301         	movs	r3, #0x1
    aa62: 9330         	str	r3, [sp, #0xc0]
    aa64: f7fe bea4    	b.w	0x97b0 <gst_lg_audio_render_event+0x1570> @ imm = #-0x12b8
    aa68: 48b2         	ldr	r0, [pc, #0x2c8]        @ 0xad34 <gst_lg_audio_render_event+0x2af4>
    aa6a: 4478         	add	r0, pc
    aa6c: 68c3         	ldr	r3, [r0, #0xc]
    aa6e: 681b         	ldr	r3, [r3]
    aa70: 06db         	lsls	r3, r3, #0x1b
    aa72: f100 84cc    	bmi.w	0xb40e <gst_lg_audio_render_event+0x31ce> @ imm = #0x998
    aa76: f8d8 3000    	ldr.w	r3, [r8]
    aa7a: 2b04         	cmp	r3, #0x4
    aa7c: d912         	bls	0xaaa4 <gst_lg_audio_render_event+0x2864> @ imm = #0x24
    aa7e: 4b20         	ldr	r3, [pc, #0x80]         @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    aa80: 2105         	movs	r1, #0x5
    aa82: 4aad         	ldr	r2, [pc, #0x2b4]        @ 0xad38 <gst_lg_audio_render_event+0x2af8>
    aa84: 58fb         	ldr	r3, [r7, r3]
    aa86: 447a         	add	r2, pc
    aa88: 9401         	str	r4, [sp, #0x4]
    aa8a: 6818         	ldr	r0, [r3]
    aa8c: 4bab         	ldr	r3, [pc, #0x2ac]        @ 0xad3c <gst_lg_audio_render_event+0x2afc>
    aa8e: 9202         	str	r2, [sp, #0x8]
    aa90: f240 5233    	movw	r2, #0x533
    aa94: 9200         	str	r2, [sp]
    aa96: 4aaa         	ldr	r2, [pc, #0x2a8]        @ 0xad40 <gst_lg_audio_render_event+0x2b00>
    aa98: 447b         	add	r3, pc
    aa9a: f503 73c6    	add.w	r3, r3, #0x18c
    aa9e: 447a         	add	r2, pc
    aaa0: f7f9 e8e4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6e38
    aaa4: 2300         	movs	r3, #0x0
    aaa6: 933c         	str	r3, [sp, #0xf0]
    aaa8: f7fe be22    	b.w	0x96f0 <gst_lg_audio_render_event+0x14b0> @ imm = #-0x13bc
    aaac: 48a5         	ldr	r0, [pc, #0x294]        @ 0xad44 <gst_lg_audio_render_event+0x2b04>
    aaae: 4478         	add	r0, pc
    aab0: 68c3         	ldr	r3, [r0, #0xc]
    aab2: 681b         	ldr	r3, [r3]
    aab4: 06d9         	lsls	r1, r3, #0x1b
    aab6: f100 83c1    	bmi.w	0xb23c <gst_lg_audio_render_event+0x2ffc> @ imm = #0x782
    aaba: f8d8 3000    	ldr.w	r3, [r8]
    aabe: 2b04         	cmp	r3, #0x4
    aac0: d912         	bls	0xaae8 <gst_lg_audio_render_event+0x28a8> @ imm = #0x24
    aac2: 4b0f         	ldr	r3, [pc, #0x3c]         @ 0xab00 <gst_lg_audio_render_event+0x28c0>
    aac4: 2105         	movs	r1, #0x5
    aac6: 4aa0         	ldr	r2, [pc, #0x280]        @ 0xad48 <gst_lg_audio_render_event+0x2b08>
    aac8: 58fb         	ldr	r3, [r7, r3]
    aaca: 447a         	add	r2, pc
    aacc: 9401         	str	r4, [sp, #0x4]
    aace: 6818         	ldr	r0, [r3]
    aad0: 4b9e         	ldr	r3, [pc, #0x278]        @ 0xad4c <gst_lg_audio_render_event+0x2b0c>
    aad2: 9202         	str	r2, [sp, #0x8]
    aad4: f240 522b    	movw	r2, #0x52b
    aad8: 9200         	str	r2, [sp]
    aada: 4a9d         	ldr	r2, [pc, #0x274]        @ 0xad50 <gst_lg_audio_render_event+0x2b10>
    aadc: 447b         	add	r3, pc
    aade: f503 73c6    	add.w	r3, r3, #0x18c
    aae2: 447a         	add	r2, pc
    aae4: f7f9 e8c2    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x6e7c
    aae8: f04f 33ff    	mov.w	r3, #0xffffffff
    aaec: 933b         	str	r3, [sp, #0xec]
    aaee: f7fe bdce    	b.w	0x968e <gst_lg_audio_render_event+0x144e> @ imm = #-0x1464
    aaf2: bf00         	nop
    aaf4: ec 5c 01 00  	.word	0x00015cec
    aaf8: fe 25 01 00  	.word	0x000125fe
    aafc: 3e 81 02 00  	.word	0x0002813e
    ab00: 08 04 00 00  	.word	0x00000408
    ab04: 8c 10 01 00  	.word	0x0001108c
    ab08: 9a 5c 01 00  	.word	0x00015c9a
    ab0c: 38 fa 00 00  	.word	0x0000fa38
    ab10: 80 5c 01 00  	.word	0x00015c80
    ab14: 7a 10 01 00  	.word	0x0001107a
    ab18: cc 80 02 00  	.word	0x000280cc
    ab1c: 46 5c 01 00  	.word	0x00015c46
    ab20: 0c 11 01 00  	.word	0x0001110c
    ab24: a8 80 02 00  	.word	0x000280a8
    ab28: 24 5c 01 00  	.word	0x00015c24
    ab2c: 4a 27 01 00  	.word	0x0001274a
    ab30: 00 5c 01 00  	.word	0x00015c00
    ab34: 52 11 01 00  	.word	0x00011152
    ab38: dc 5b 01 00  	.word	0x00015bdc
    ab3c: 1e 11 01 00  	.word	0x0001111e
    ab40: bc 5b 01 00  	.word	0x00015bbc
    ab44: a2 10 01 00  	.word	0x000110a2
    ab48: 9c 5b 01 00  	.word	0x00015b9c
    ab4c: 1a 24 01 00  	.word	0x0001241a
    ab50: 7a 5b 01 00  	.word	0x00015b7a
    ab54: be 0e 01 00  	.word	0x00010ebe
    ab58: 54 5b 01 00  	.word	0x00015b54
    ab5c: 56 0e 01 00  	.word	0x00010e56
    ab60: 34 5b 01 00  	.word	0x00015b34
    ab64: 7e 0d 01 00  	.word	0x00010d7e
    ab68: 0a 5b 01 00  	.word	0x00015b0a
    ab6c: dc 0c 01 00  	.word	0x00010cdc
    ab70: 5c 7f 02 00  	.word	0x00027f5c
    ab74: ce 5a 01 00  	.word	0x00015ace
    ab78: 8e 12 01 00  	.word	0x0001128e
    ab7c: 58 f8 00 00  	.word	0x0000f858
    ab80: 16 7f 02 00  	.word	0x00027f16
    ab84: 88 5a 01 00  	.word	0x00015a88
    ab88: 7a 22 01 00  	.word	0x0001227a
    ab8c: 16 f8 00 00  	.word	0x0000f816
    ab90: c8 7e 02 00  	.word	0x00027ec8
    ab94: 44 5a 01 00  	.word	0x00015a44
    ab98: 0a 22 01 00  	.word	0x0001220a
    ab9c: a6 7e 02 00  	.word	0x00027ea6
    aba0: 96 7e 02 00  	.word	0x00027e96
    aba4: 04 5a 01 00  	.word	0x00015a04
    aba8: f2 22 01 00  	.word	0x000122f2
    abac: 92 f7 00 00  	.word	0x0000f792
    abb0: ce 7d 02 00  	.word	0x00027dce
    abb4: ea 0d 01 00  	.word	0x00010dea
    abb8: 2c 59 01 00  	.word	0x0001592c
    abbc: ca f6 00 00  	.word	0x0000f6ca
    abc0: 08 59 01 00  	.word	0x00015908
    abc4: 1e 10 01 00  	.word	0x0001101e
    abc8: e4 58 01 00  	.word	0x000158e4
    abcc: d6 10 01 00  	.word	0x000110d6
    abd0: c4 58 01 00  	.word	0x000158c4
    abd4: 5e 10 01 00  	.word	0x0001105e
    abd8: a6 58 01 00  	.word	0x000158a6
    abdc: 18 11 01 00  	.word	0x00011118
    abe0: 84 58 01 00  	.word	0x00015884
    abe4: 62 21 01 00  	.word	0x00012162
    abe8: 38 0a 01 00  	.word	0x00010a38
    abec: 2e 0a 01 00  	.word	0x00010a2e
    abf0: 4e 58 01 00  	.word	0x0001584e
    abf4: 54 11 01 00  	.word	0x00011154
    abf8: 2e 58 01 00  	.word	0x0001582e
    abfc: dc 0b 01 00  	.word	0x00010bdc
    ac00: 78 7c 02 00  	.word	0x00027c78
    ac04: 46 11 01 00  	.word	0x00011146
    ac08: d0 57 01 00  	.word	0x000157d0
    ac0c: 6e f5 00 00  	.word	0x0000f56e
    ac10: 2a 7c 02 00  	.word	0x00027c2a
    ac14: 3a 17 01 00  	.word	0x0001173a
    ac18: 88 57 01 00  	.word	0x00015788
    ac1c: 26 f5 00 00  	.word	0x0000f526
    ac20: d6 16 01 00  	.word	0x000116d6
    ac24: 1c 17 01 00  	.word	0x0001171c
    ac28: c6 7b 02 00  	.word	0x00027bc6
    ac2c: 82 15 01 00  	.word	0x00011582
    ac30: 24 57 01 00  	.word	0x00015724
    ac34: c2 f4 00 00  	.word	0x0000f4c2
    ac38: 78 7b 02 00  	.word	0x00027b78
    ac3c: c8 14 01 00  	.word	0x000114c8
    ac40: d6 56 01 00  	.word	0x000156d6
    ac44: 74 f4 00 00  	.word	0x0000f474
    ac48: 2a 7b 02 00  	.word	0x00027b2a
    ac4c: 72 15 01 00  	.word	0x00011572
    ac50: 88 56 01 00  	.word	0x00015688
    ac54: 26 f4 00 00  	.word	0x0000f426
    ac58: ae 11 01 00  	.word	0x000111ae
    ac5c: 40 56 01 00  	.word	0x00015640
    ac60: de f3 00 00  	.word	0x0000f3de
    ac64: 98 7a 02 00  	.word	0x00027a98
    ac68: 64 13 01 00  	.word	0x00011364
    ac6c: f6 55 01 00  	.word	0x000155f6
    ac70: 94 f3 00 00  	.word	0x0000f394
    ac74: d4 19 01 00  	.word	0x000119d4
    ac78: 9c 55 01 00  	.word	0x0001559c
    ac7c: 3a f3 00 00  	.word	0x0000f33a
    ac80: d8 79 02 00  	.word	0x000279d8
    ac84: 0e 1b 01 00  	.word	0x00011b0e
    ac88: 30 55 01 00  	.word	0x00015530
    ac8c: ce f2 00 00  	.word	0x0000f2ce
    ac90: ea 1b 01 00  	.word	0x00011bea
    ac94: e8 54 01 00  	.word	0x000154e8
    ac98: 86 f2 00 00  	.word	0x0000f286
    ac9c: ba 1a 01 00  	.word	0x00011aba
    aca0: 94 54 01 00  	.word	0x00015494
    aca4: 32 f2 00 00  	.word	0x0000f232
    aca8: e6 78 02 00  	.word	0x000278e6
    acac: bc 1b 01 00  	.word	0x00011bbc
    acb0: e0 f1 00 00  	.word	0x0000f1e0
    acb4: 3a 54 01 00  	.word	0x0001543a
    acb8: 90 78 02 00  	.word	0x00027890
    acbc: f6 53 01 00  	.word	0x000153f6
    acc0: 98 1b 01 00  	.word	0x00011b98
    acc4: 86 f1 00 00  	.word	0x0000f186
    acc8: 20 78 02 00  	.word	0x00027820
    accc: 94 53 01 00  	.word	0x00015394
    acd0: e0 1b 01 00  	.word	0x00011be0
    acd4: 1e f1 00 00  	.word	0x0000f11e
    acd8: 7a 77 02 00  	.word	0x0002777a
    acdc: 88 17 01 00  	.word	0x00011788
    ace0: da 52 01 00  	.word	0x000152da
    ace4: 78 f0 00 00  	.word	0x0000f078
    ace8: 00 77 02 00  	.word	0x00027700
    acec: e0 17 01 00  	.word	0x000117e0
    acf0: 5c 52 01 00  	.word	0x0001525c
    acf4: fa ef 00 00  	.word	0x0000effa
    acf8: d8 16 01 00  	.word	0x000116d8
    acfc: 12 52 01 00  	.word	0x00015212
    ad00: b0 ef 00 00  	.word	0x0000efb0
    ad04: 6a 76 02 00  	.word	0x0002766a
    ad08: f2 16 01 00  	.word	0x000116f2
    ad0c: c6 51 01 00  	.word	0x000151c6
    ad10: 64 ef 00 00  	.word	0x0000ef64
    ad14: 1e 76 02 00  	.word	0x0002761e
    ad18: 8a 14 01 00  	.word	0x0001148a
    ad1c: 84 51 01 00  	.word	0x00015184
    ad20: 22 ef 00 00  	.word	0x0000ef22
    ad24: da 75 02 00  	.word	0x000275da
    ad28: b6 12 01 00  	.word	0x000112b6
    ad2c: 40 51 01 00  	.word	0x00015140
    ad30: de ee 00 00  	.word	0x0000eede
    ad34: 96 75 02 00  	.word	0x00027596
    ad38: c2 11 01 00  	.word	0x000111c2
    ad3c: fc 50 01 00  	.word	0x000150fc
    ad40: 9a ee 00 00  	.word	0x0000ee9a
    ad44: 52 75 02 00  	.word	0x00027552
    ad48: 0a 11 01 00  	.word	0x0001110a
    ad4c: b8 50 01 00  	.word	0x000150b8
    ad50: 56 ee 00 00  	.word	0x0000ee56
    ad54: f89d 30c8    	ldrb.w	r3, [sp, #0xc8]
    ad58: 2b01         	cmp	r3, #0x1
    ad5a: f040 80e9    	bne.w	0xaf30 <gst_lg_audio_render_event+0x2cf0> @ imm = #0x1d2
    ad5e: f8df 0bf0    	ldr.w	r0, [pc, #0xbf0]        @ 0xb950 <gst_lg_audio_render_event+0x3710>
    ad62: 9335         	str	r3, [sp, #0xd4]
    ad64: 4478         	add	r0, pc
    ad66: 68c2         	ldr	r2, [r0, #0xc]
    ad68: 6812         	ldr	r2, [r2]
    ad6a: 06d2         	lsls	r2, r2, #0x1b
    ad6c: f100 8524    	bmi.w	0xb7b8 <gst_lg_audio_render_event+0x3578> @ imm = #0xa48
    ad70: f8d8 3000    	ldr.w	r3, [r8]
    ad74: 2b04         	cmp	r3, #0x4
    ad76: f67e aae4    	bls.w	0x9342 <gst_lg_audio_render_event+0x1102> @ imm = #-0x1a38
    ad7a: f8df 3bd8    	ldr.w	r3, [pc, #0xbd8]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    ad7e: 2105         	movs	r1, #0x5
    ad80: 9a35         	ldr	r2, [sp, #0xd4]
    ad82: 58fb         	ldr	r3, [r7, r3]
    ad84: 9401         	str	r4, [sp, #0x4]
    ad86: 6818         	ldr	r0, [r3]
    ad88: 9204         	str	r2, [sp, #0x10]
    ad8a: f89d 20c8    	ldrb.w	r2, [sp, #0xc8]
    ad8e: f8df 3bc8    	ldr.w	r3, [pc, #0xbc8]        @ 0xb958 <gst_lg_audio_render_event+0x3718>
    ad92: 9203         	str	r2, [sp, #0xc]
    ad94: f8df 2bc4    	ldr.w	r2, [pc, #0xbc4]        @ 0xb95c <gst_lg_audio_render_event+0x371c>
    ad98: 447b         	add	r3, pc
    ad9a: f503 73c6    	add.w	r3, r3, #0x18c
    ad9e: 447a         	add	r2, pc
    ada0: 9202         	str	r2, [sp, #0x8]
    ada2: f44f 629a    	mov.w	r2, #0x4d0
    ada6: 9200         	str	r2, [sp]
    ada8: f8df 2bb4    	ldr.w	r2, [pc, #0xbb4]        @ 0xb960 <gst_lg_audio_render_event+0x3720>
    adac: 447a         	add	r2, pc
    adae: f7f8 ef5e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x7144
    adb2: f7fe bac6    	b.w	0x9342 <gst_lg_audio_render_event+0x1102> @ imm = #-0x1a74
    adb6: f8df 0bac    	ldr.w	r0, [pc, #0xbac]        @ 0xb964 <gst_lg_audio_render_event+0x3724>
    adba: 4478         	add	r0, pc
    adbc: 68c3         	ldr	r3, [r0, #0xc]
    adbe: 681b         	ldr	r3, [r3]
    adc0: 071a         	lsls	r2, r3, #0x1c
    adc2: f100 821c    	bmi.w	0xb1fe <gst_lg_audio_render_event+0x2fbe> @ imm = #0x438
    adc6: f8d8 3000    	ldr.w	r3, [r8]
    adca: 2b03         	cmp	r3, #0x3
    adcc: f67f aa52    	bls.w	0xa274 <gst_lg_audio_render_event+0x2034> @ imm = #-0xb5c
    add0: f8df 3b80    	ldr.w	r3, [pc, #0xb80]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    add4: 2104         	movs	r1, #0x4
    add6: f8df 2b90    	ldr.w	r2, [pc, #0xb90]        @ 0xb968 <gst_lg_audio_render_event+0x3728>
    adda: 58fb         	ldr	r3, [r7, r3]
    addc: 447a         	add	r2, pc
    adde: 9401         	str	r4, [sp, #0x4]
    ade0: 6818         	ldr	r0, [r3]
    ade2: 9202         	str	r2, [sp, #0x8]
    ade4: f640 02df    	movw	r2, #0x8df
    ade8: f8df 3b80    	ldr.w	r3, [pc, #0xb80]        @ 0xb96c <gst_lg_audio_render_event+0x372c>
    adec: 9200         	str	r2, [sp]
    adee: f8df 2b80    	ldr.w	r2, [pc, #0xb80]        @ 0xb970 <gst_lg_audio_render_event+0x3730>
    adf2: 447b         	add	r3, pc
    adf4: f503 73b8    	add.w	r3, r3, #0x170
    adf8: 447a         	add	r2, pc
    adfa: f7f8 ef38    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x7190
    adfe: f7ff ba39    	b.w	0xa274 <gst_lg_audio_render_event+0x2034> @ imm = #-0xb8e
    ae02: 4651         	mov	r1, r10
    ae04: aa2e         	add	r2, sp, #0xb8
    ae06: 4648         	mov	r0, r9
    ae08: f7f9 ea4c    	blx	0x42a4 <gst_structure_get_uint64@plt> @ imm = #-0x6b68
    ae0c: f7fd bf10    	b.w	0x8c30 <gst_lg_audio_render_event+0x9f0> @ imm = #-0x21e0
    ae10: 4651         	mov	r1, r10
    ae12: 4648         	mov	r0, r9
    ae14: aa30         	add	r2, sp, #0xc0
    ae16: f7f9 ea46    	blx	0x42a4 <gst_structure_get_uint64@plt> @ imm = #-0x6b74
    ae1a: f7fd bf31    	b.w	0x8c80 <gst_lg_audio_render_event+0xa40> @ imm = #-0x219e
    ae1e: f8df 0b54    	ldr.w	r0, [pc, #0xb54]        @ 0xb974 <gst_lg_audio_render_event+0x3734>
    ae22: 4478         	add	r0, pc
    ae24: 68c3         	ldr	r3, [r0, #0xc]
    ae26: 681b         	ldr	r3, [r3]
    ae28: 07da         	lsls	r2, r3, #0x1f
    ae2a: f100 83af    	bmi.w	0xb58c <gst_lg_audio_render_event+0x334c> @ imm = #0x75e
    ae2e: f8d8 3000    	ldr.w	r3, [r8]
    ae32: 2b00         	cmp	r3, #0x0
    ae34: f43f a97e    	beq.w	0xa134 <gst_lg_audio_render_event+0x1ef4> @ imm = #-0xd04
    ae38: f8df 2b18    	ldr.w	r2, [pc, #0xb18]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    ae3c: 2101         	movs	r1, #0x1
    ae3e: f8df 3b38    	ldr.w	r3, [pc, #0xb38]        @ 0xb978 <gst_lg_audio_render_event+0x3738>
    ae42: 58ba         	ldr	r2, [r7, r2]
    ae44: 9401         	str	r4, [sp, #0x4]
    ae46: 447b         	add	r3, pc
    ae48: f503 73c6    	add.w	r3, r3, #0x18c
    ae4c: 6810         	ldr	r0, [r2]
    ae4e: f8df 2b2c    	ldr.w	r2, [pc, #0xb2c]        @ 0xb97c <gst_lg_audio_render_event+0x373c>
    ae52: 447a         	add	r2, pc
    ae54: 9202         	str	r2, [sp, #0x8]
    ae56: f240 428c    	movw	r2, #0x48c
    ae5a: 9200         	str	r2, [sp]
    ae5c: f8df 2b20    	ldr.w	r2, [pc, #0xb20]        @ 0xb980 <gst_lg_audio_render_event+0x3740>
    ae60: 447a         	add	r2, pc
    ae62: f7f8 ef04    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x71f8
    ae66: f7ff b93e    	b.w	0xa0e6 <gst_lg_audio_render_event+0x1ea6> @ imm = #-0xd84
    ae6a: 9b46         	ldr	r3, [sp, #0x118]
    ae6c: 9a2c         	ldr	r2, [sp, #0xb0]
    ae6e: 4293         	cmp	r3, r2
    ae70: f43f aca2    	beq.w	0xa7b8 <gst_lg_audio_render_event+0x2578> @ imm = #-0x6bc
    ae74: f8df 1b0c    	ldr.w	r1, [pc, #0xb0c]        @ 0xb984 <gst_lg_audio_render_event+0x3744>
    ae78: 4479         	add	r1, pc
    ae7a: 68c9         	ldr	r1, [r1, #0xc]
    ae7c: 6809         	ldr	r1, [r1]
    ae7e: 0709         	lsls	r1, r1, #0x1c
    ae80: f57f ac9a    	bpl.w	0xa7b8 <gst_lg_audio_render_event+0x2578> @ imm = #-0x6cc
    ae84: e9cd 2301    	strd	r2, r3, [sp, #4]
    ae88: 2103         	movs	r1, #0x3
    ae8a: f8df 2afc    	ldr.w	r2, [pc, #0xafc]        @ 0xb988 <gst_lg_audio_render_event+0x3748>
    ae8e: f8df 3afc    	ldr.w	r3, [pc, #0xafc]        @ 0xb98c <gst_lg_audio_render_event+0x374c>
    ae92: f8df 0afc    	ldr.w	r0, [pc, #0xafc]        @ 0xb990 <gst_lg_audio_render_event+0x3750>
    ae96: 447a         	add	r2, pc
    ae98: 447b         	add	r3, pc
    ae9a: f502 72c6    	add.w	r2, r2, #0x18c
    ae9e: 9300         	str	r3, [sp]
    aea0: 4478         	add	r0, pc
    aea2: f240 53dd    	movw	r3, #0x5dd
    aea6: f7f9 e882    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x6efc
    aeaa: e467         	b	0xa77c <gst_lg_audio_render_event+0x253c> @ imm = #-0x732
    aeac: f8df 2ae4    	ldr.w	r2, [pc, #0xae4]        @ 0xb994 <gst_lg_audio_render_event+0x3754>
    aeb0: 2104         	movs	r1, #0x4
    aeb2: f8df 3ae4    	ldr.w	r3, [pc, #0xae4]        @ 0xb998 <gst_lg_audio_render_event+0x3758>
    aeb6: f8cd b004    	str.w	r11, [sp, #0x4]
    aeba: 447a         	add	r2, pc
    aebc: 447b         	add	r3, pc
    aebe: f502 72c6    	add.w	r2, r2, #0x18c
    aec2: 9300         	str	r3, [sp]
    aec4: f240 43a5    	movw	r3, #0x4a5
    aec8: f7f9 e870    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x6f20
    aecc: f7fe b954    	b.w	0x9178 <gst_lg_audio_render_event+0xf38> @ imm = #-0x1d58
    aed0: f8df 2ac8    	ldr.w	r2, [pc, #0xac8]        @ 0xb99c <gst_lg_audio_render_event+0x375c>
    aed4: 2103         	movs	r1, #0x3
    aed6: f8df 3ac8    	ldr.w	r3, [pc, #0xac8]        @ 0xb9a0 <gst_lg_audio_render_event+0x3760>
    aeda: 447a         	add	r2, pc
    aedc: 447b         	add	r3, pc
    aede: f502 72b8    	add.w	r2, r2, #0x170
    aee2: 9300         	str	r3, [sp]
    aee4: f640 03e1    	movw	r3, #0x8e1
    aee8: f7f9 e860    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x6f40
    aeec: f7ff b9a7    	b.w	0xa23e <gst_lg_audio_render_event+0x1ffe> @ imm = #-0xcb2
    aef0: f8df 2ab0    	ldr.w	r2, [pc, #0xab0]        @ 0xb9a4 <gst_lg_audio_render_event+0x3764>
    aef4: 2102         	movs	r1, #0x2
    aef6: f8df 3ab0    	ldr.w	r3, [pc, #0xab0]        @ 0xb9a8 <gst_lg_audio_render_event+0x3768>
    aefa: 447a         	add	r2, pc
    aefc: 447b         	add	r3, pc
    aefe: f502 72b8    	add.w	r2, r2, #0x170
    af02: 9300         	str	r3, [sp]
    af04: f640 1372    	movw	r3, #0x972
    af08: f7f9 e850    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x6f60
    af0c: f7fd be79    	b.w	0x8c02 <gst_lg_audio_render_event+0x9c2> @ imm = #-0x230e
    af10: f8df 2a98    	ldr.w	r2, [pc, #0xa98]        @ 0xb9ac <gst_lg_audio_render_event+0x376c>
    af14: 2102         	movs	r1, #0x2
    af16: f8df 3a98    	ldr.w	r3, [pc, #0xa98]        @ 0xb9b0 <gst_lg_audio_render_event+0x3770>
    af1a: 447a         	add	r2, pc
    af1c: 447b         	add	r3, pc
    af1e: f502 72b8    	add.w	r2, r2, #0x170
    af22: 9300         	str	r3, [sp]
    af24: f640 1378    	movw	r3, #0x978
    af28: f7f9 e840    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x6f80
    af2c: f7fd be91    	b.w	0x8c52 <gst_lg_audio_render_event+0xa12> @ imm = #-0x22de
    af30: f8df 2a80    	ldr.w	r2, [pc, #0xa80]        @ 0xb9b4 <gst_lg_audio_render_event+0x3774>
    af34: 4653         	mov	r3, r10
    af36: f8df 1a80    	ldr.w	r1, [pc, #0xa80]        @ 0xb9b8 <gst_lg_audio_render_event+0x3778>
    af3a: 58ba         	ldr	r2, [r7, r2]
    af3c: 4479         	add	r1, pc
    af3e: 9000         	str	r0, [sp]
    af40: 4648         	mov	r0, r9
    af42: 6812         	ldr	r2, [r2]
    af44: f7f9 e894    	blx	0x4070 <gst_structure_get@plt> @ imm = #-0x6ed8
    af48: 2800         	cmp	r0, #0x0
    af4a: f000 832e    	beq.w	0xb5aa <gst_lg_audio_render_event+0x336a> @ imm = #0x65c
    af4e: 9b1a         	ldr	r3, [sp, #0x68]
    af50: 2b02         	cmp	r3, #0x2
    af52: bf08         	it	eq
    af54: 2201         	moveq	r2, #0x1
    af56: d003         	beq	0xaf60 <gst_lg_audio_render_event+0x2d20> @ imm = #0x6
    af58: 2b01         	cmp	r3, #0x1
    af5a: bf0c         	ite	eq
    af5c: 2202         	moveq	r2, #0x2
    af5e: 2207         	movne	r2, #0x7
    af60: f8df 0a58    	ldr.w	r0, [pc, #0xa58]        @ 0xb9bc <gst_lg_audio_render_event+0x377c>
    af64: 9235         	str	r2, [sp, #0xd4]
    af66: 4478         	add	r0, pc
    af68: 68c1         	ldr	r1, [r0, #0xc]
    af6a: 6809         	ldr	r1, [r1]
    af6c: 06c9         	lsls	r1, r1, #0x1b
    af6e: f100 84b5    	bmi.w	0xb8dc <gst_lg_audio_render_event+0x369c> @ imm = #0x96a
    af72: f8d8 3000    	ldr.w	r3, [r8]
    af76: 2b04         	cmp	r3, #0x4
    af78: f67e a9e3    	bls.w	0x9342 <gst_lg_audio_render_event+0x1102> @ imm = #-0x1c3a
    af7c: f8df 39d4    	ldr.w	r3, [pc, #0x9d4]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    af80: 2105         	movs	r1, #0x5
    af82: 9a35         	ldr	r2, [sp, #0xd4]
    af84: 58fb         	ldr	r3, [r7, r3]
    af86: 9401         	str	r4, [sp, #0x4]
    af88: 6818         	ldr	r0, [r3]
    af8a: 9204         	str	r2, [sp, #0x10]
    af8c: 9a1a         	ldr	r2, [sp, #0x68]
    af8e: f8df 3a30    	ldr.w	r3, [pc, #0xa30]        @ 0xb9c0 <gst_lg_audio_render_event+0x3780>
    af92: 9203         	str	r2, [sp, #0xc]
    af94: f8df 2a2c    	ldr.w	r2, [pc, #0xa2c]        @ 0xb9c4 <gst_lg_audio_render_event+0x3784>
    af98: 447b         	add	r3, pc
    af9a: f503 73c6    	add.w	r3, r3, #0x18c
    af9e: 447a         	add	r2, pc
    afa0: 9202         	str	r2, [sp, #0x8]
    afa2: f240 42dd    	movw	r2, #0x4dd
    afa6: 9200         	str	r2, [sp]
    afa8: f8df 2a1c    	ldr.w	r2, [pc, #0xa1c]        @ 0xb9c8 <gst_lg_audio_render_event+0x3788>
    afac: 447a         	add	r2, pc
    afae: f7f8 ee5e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x7344
    afb2: f7fe b9c6    	b.w	0x9342 <gst_lg_audio_render_event+0x1102> @ imm = #-0x1c74
    afb6: f8df 2a14    	ldr.w	r2, [pc, #0xa14]        @ 0xb9cc <gst_lg_audio_render_event+0x378c>
    afba: 2103         	movs	r1, #0x3
    afbc: f8df 3a10    	ldr.w	r3, [pc, #0xa10]        @ 0xb9d0 <gst_lg_audio_render_event+0x3790>
    afc0: 447a         	add	r2, pc
    afc2: 447b         	add	r3, pc
    afc4: f502 72c6    	add.w	r2, r2, #0x18c
    afc8: 9300         	str	r3, [sp]
    afca: f240 4381    	movw	r3, #0x481
    afce: f7f8 efee    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7024
    afd2: f7fd bcc8    	b.w	0x8966 <gst_lg_audio_render_event+0x726> @ imm = #-0x2670
    afd6: f8df 29fc    	ldr.w	r2, [pc, #0x9fc]        @ 0xb9d4 <gst_lg_audio_render_event+0x3794>
    afda: 4670         	mov	r0, lr
    afdc: f8df 39f8    	ldr.w	r3, [pc, #0x9f8]        @ 0xb9d8 <gst_lg_audio_render_event+0x3798>
    afe0: 2104         	movs	r1, #0x4
    afe2: e9cd ab01    	strd	r10, r11, [sp, #4]
    afe6: 447a         	add	r2, pc
    afe8: 447b         	add	r3, pc
    afea: f502 72c6    	add.w	r2, r2, #0x18c
    afee: 9300         	str	r3, [sp]
    aff0: f240 5369    	movw	r3, #0x569
    aff4: f7f8 efda    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x704c
    aff8: f7fe bcb0    	b.w	0x995c <gst_lg_audio_render_event+0x171c> @ imm = #-0x16a0
    affc: f8df 09dc    	ldr.w	r0, [pc, #0x9dc]        @ 0xb9dc <gst_lg_audio_render_event+0x379c>
    b000: 4478         	add	r0, pc
    b002: 68c3         	ldr	r3, [r0, #0xc]
    b004: 681b         	ldr	r3, [r3]
    b006: 07db         	lsls	r3, r3, #0x1f
    b008: f100 83e6    	bmi.w	0xb7d8 <gst_lg_audio_render_event+0x3598> @ imm = #0x7cc
    b00c: f8d8 3000    	ldr.w	r3, [r8]
    b010: 2b00         	cmp	r3, #0x0
    b012: f43d aa50    	beq.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x2b60
    b016: f8df 293c    	ldr.w	r2, [pc, #0x93c]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    b01a: f8df 39c4    	ldr.w	r3, [pc, #0x9c4]        @ 0xb9e0 <gst_lg_audio_render_event+0x37a0>
    b01e: 58b9         	ldr	r1, [r7, r2]
    b020: f240 42c6    	movw	r2, #0x4c6
    b024: 9200         	str	r2, [sp]
    b026: 447b         	add	r3, pc
    b028: 9401         	str	r4, [sp, #0x4]
    b02a: f503 73c6    	add.w	r3, r3, #0x18c
    b02e: 6808         	ldr	r0, [r1]
    b030: f8df 19b0    	ldr.w	r1, [pc, #0x9b0]        @ 0xb9e4 <gst_lg_audio_render_event+0x37a4>
    b034: f8df 29b0    	ldr.w	r2, [pc, #0x9b0]        @ 0xb9e8 <gst_lg_audio_render_event+0x37a8>
    b038: 4479         	add	r1, pc
    b03a: 9102         	str	r1, [sp, #0x8]
    b03c: 447a         	add	r2, pc
    b03e: 2101         	movs	r1, #0x1
    b040: f7f8 ee14    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x73d8
    b044: f7fd ba37    	b.w	0x84b6 <gst_lg_audio_render_event+0x276> @ imm = #-0x2b92
    b048: 9b46         	ldr	r3, [sp, #0x118]
    b04a: 9a2c         	ldr	r2, [sp, #0xb0]
    b04c: 4293         	cmp	r3, r2
    b04e: f47f ab8d    	bne.w	0xa76c <gst_lg_audio_render_event+0x252c> @ imm = #-0x8e6
    b052: f7ff bbb5    	b.w	0xa7c0 <gst_lg_audio_render_event+0x2580> @ imm = #-0x896
    b056: 9301         	str	r3, [sp, #0x4]
    b058: 2104         	movs	r1, #0x4
    b05a: f8df 2990    	ldr.w	r2, [pc, #0x990]        @ 0xb9ec <gst_lg_audio_render_event+0x37ac>
    b05e: f8df 3990    	ldr.w	r3, [pc, #0x990]        @ 0xb9f0 <gst_lg_audio_render_event+0x37b0>
    b062: 447a         	add	r2, pc
    b064: 447b         	add	r3, pc
    b066: f502 72c6    	add.w	r2, r2, #0x18c
    b06a: 9300         	str	r3, [sp]
    b06c: f44f 6393    	mov.w	r3, #0x498
    b070: f7f8 ef9c    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x70c8
    b074: f7fe b821    	b.w	0x90ba <gst_lg_audio_render_event+0xe7a> @ imm = #-0x1fbe
    b078: 9301         	str	r3, [sp, #0x4]
    b07a: 2104         	movs	r1, #0x4
    b07c: f8df 2974    	ldr.w	r2, [pc, #0x974]        @ 0xb9f4 <gst_lg_audio_render_event+0x37b4>
    b080: f8df 3974    	ldr.w	r3, [pc, #0x974]        @ 0xb9f8 <gst_lg_audio_render_event+0x37b8>
    b084: 447a         	add	r2, pc
    b086: 447b         	add	r3, pc
    b088: f502 72c6    	add.w	r2, r2, #0x18c
    b08c: 9300         	str	r3, [sp]
    b08e: f240 439d    	movw	r3, #0x49d
    b092: f7f8 ef8c    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x70e8
    b096: f7fe b840    	b.w	0x911a <gst_lg_audio_render_event+0xeda> @ imm = #-0x1f80
    b09a: f8df 2960    	ldr.w	r2, [pc, #0x960]        @ 0xb9fc <gst_lg_audio_render_event+0x37bc>
    b09e: 2103         	movs	r1, #0x3
    b0a0: f8df 395c    	ldr.w	r3, [pc, #0x95c]        @ 0xba00 <gst_lg_audio_render_event+0x37c0>
    b0a4: 447a         	add	r2, pc
    b0a6: 447b         	add	r3, pc
    b0a8: f502 72b8    	add.w	r2, r2, #0x170
    b0ac: 9300         	str	r3, [sp]
    b0ae: f640 1358    	movw	r3, #0x958
    b0b2: f7f8 ef7c    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7108
    b0b6: f7fd bd31    	b.w	0x8b1c <gst_lg_audio_render_event+0x8dc> @ imm = #-0x259e
    b0ba: e9cd 2302    	strd	r2, r3, [sp, #8]
    b0be: 2102         	movs	r1, #0x2
    b0c0: f8df 2940    	ldr.w	r2, [pc, #0x940]        @ 0xba04 <gst_lg_audio_render_event+0x37c4>
    b0c4: f8df 3940    	ldr.w	r3, [pc, #0x940]        @ 0xba08 <gst_lg_audio_render_event+0x37c8>
    b0c8: e9cd 0904    	strd	r0, r9, [sp, #16]
    b0cc: 4660         	mov	r0, r12
    b0ce: 447a         	add	r2, pc
    b0d0: 447b         	add	r3, pc
    b0d2: f502 72b8    	add.w	r2, r2, #0x170
    b0d6: 9300         	str	r3, [sp]
    b0d8: f640 137c    	movw	r3, #0x97c
    b0dc: f7f8 ef66    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7134
    b0e0: e9dd 232e    	ldrd	r2, r3, [sp, #184]
    b0e4: e9dd 0930    	ldrd	r0, r9, [sp, #192]
    b0e8: f7fd bde7    	b.w	0x8cba <gst_lg_audio_render_event+0xa7a> @ imm = #-0x2432
    b0ec: 9b16         	ldr	r3, [sp, #0x58]
    b0ee: 2103         	movs	r1, #0x3
    b0f0: f8df 2918    	ldr.w	r2, [pc, #0x918]        @ 0xba0c <gst_lg_audio_render_event+0x37cc>
    b0f4: 9301         	str	r3, [sp, #0x4]
    b0f6: f8df 3918    	ldr.w	r3, [pc, #0x918]        @ 0xba10 <gst_lg_audio_render_event+0x37d0>
    b0fa: 447a         	add	r2, pc
    b0fc: f502 72c6    	add.w	r2, r2, #0x18c
    b100: 447b         	add	r3, pc
    b102: 9300         	str	r3, [sp]
    b104: f240 530b    	movw	r3, #0x50b
    b108: f7f8 ef50    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7160
    b10c: f7fe b9fb    	b.w	0x9506 <gst_lg_audio_render_event+0x12c6> @ imm = #-0x1c0a
    b110: 4601         	mov	r1, r0
    b112: f8df 2900    	ldr.w	r2, [pc, #0x900]        @ 0xba14 <gst_lg_audio_render_event+0x37d4>
    b116: 4618         	mov	r0, r3
    b118: f8df 38fc    	ldr.w	r3, [pc, #0x8fc]        @ 0xba18 <gst_lg_audio_render_event+0x37d8>
    b11c: f8cd b004    	str.w	r11, [sp, #0x4]
    b120: 447a         	add	r2, pc
    b122: 447b         	add	r3, pc
    b124: f502 72c6    	add.w	r2, r2, #0x18c
    b128: 9300         	str	r3, [sp]
    b12a: f240 4392    	movw	r3, #0x492
    b12e: f7f8 ef3e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7184
    b132: f7ff b92f    	b.w	0xa394 <gst_lg_audio_render_event+0x2154> @ imm = #-0xda2
    b136: 6812         	ldr	r2, [r2]
    b138: 06d0         	lsls	r0, r2, #0x1b
    b13a: f100 835a    	bmi.w	0xb7f2 <gst_lg_audio_render_event+0x35b2> @ imm = #0x6b4
    b13e: f8d8 3000    	ldr.w	r3, [r8]
    b142: 2b04         	cmp	r3, #0x4
    b144: f67e a8cc    	bls.w	0x92e0 <gst_lg_audio_render_event+0x10a0> @ imm = #-0x1e68
    b148: f8df 3808    	ldr.w	r3, [pc, #0x808]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    b14c: 2105         	movs	r1, #0x5
    b14e: f8df 28cc    	ldr.w	r2, [pc, #0x8cc]        @ 0xba1c <gst_lg_audio_render_event+0x37dc>
    b152: 58fb         	ldr	r3, [r7, r3]
    b154: 447a         	add	r2, pc
    b156: 9401         	str	r4, [sp, #0x4]
    b158: 6818         	ldr	r0, [r3]
    b15a: 9202         	str	r2, [sp, #0x8]
    b15c: f240 42bc    	movw	r2, #0x4bc
    b160: f8df 38bc    	ldr.w	r3, [pc, #0x8bc]        @ 0xba20 <gst_lg_audio_render_event+0x37e0>
    b164: 9200         	str	r2, [sp]
    b166: f8df 28bc    	ldr.w	r2, [pc, #0x8bc]        @ 0xba24 <gst_lg_audio_render_event+0x37e4>
    b16a: 447b         	add	r3, pc
    b16c: f503 73c6    	add.w	r3, r3, #0x18c
    b170: 447a         	add	r2, pc
    b172: f7f8 ed7c    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x7508
    b176: f7fe b8b3    	b.w	0x92e0 <gst_lg_audio_render_event+0x10a0> @ imm = #-0x1e9a
    b17a: f8df 28ac    	ldr.w	r2, [pc, #0x8ac]        @ 0xba28 <gst_lg_audio_render_event+0x37e8>
    b17e: 2104         	movs	r1, #0x4
    b180: f8df 38a8    	ldr.w	r3, [pc, #0x8a8]        @ 0xba2c <gst_lg_audio_render_event+0x37ec>
    b184: 447a         	add	r2, pc
    b186: 447b         	add	r3, pc
    b188: f502 72c6    	add.w	r2, r2, #0x18c
    b18c: 9300         	str	r3, [sp]
    b18e: f240 5301    	movw	r3, #0x501
    b192: f7f8 ef0c    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x71e8
    b196: f7ff b9a4    	b.w	0xa4e2 <gst_lg_audio_render_event+0x22a2> @ imm = #-0xcb8
    b19a: 9301         	str	r3, [sp, #0x4]
    b19c: 2104         	movs	r1, #0x4
    b19e: f8df 2890    	ldr.w	r2, [pc, #0x890]        @ 0xba30 <gst_lg_audio_render_event+0x37f0>
    b1a2: f8df 3890    	ldr.w	r3, [pc, #0x890]        @ 0xba34 <gst_lg_audio_render_event+0x37f4>
    b1a6: 447a         	add	r2, pc
    b1a8: 447b         	add	r3, pc
    b1aa: f502 72c6    	add.w	r2, r2, #0x18c
    b1ae: 9300         	str	r3, [sp]
    b1b0: f240 43ff    	movw	r3, #0x4ff
    b1b4: f7f8 eefa    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x720c
    b1b8: f7fe b96a    	b.w	0x9490 <gst_lg_audio_render_event+0x1250> @ imm = #-0x1d2c
    b1bc: f8df 2878    	ldr.w	r2, [pc, #0x878]        @ 0xba38 <gst_lg_audio_render_event+0x37f8>
    b1c0: 2104         	movs	r1, #0x4
    b1c2: f8df 3878    	ldr.w	r3, [pc, #0x878]        @ 0xba3c <gst_lg_audio_render_event+0x37fc>
    b1c6: 447a         	add	r2, pc
    b1c8: 447b         	add	r3, pc
    b1ca: f502 72c6    	add.w	r2, r2, #0x18c
    b1ce: 9300         	str	r3, [sp]
    b1d0: f240 43e9    	movw	r3, #0x4e9
    b1d4: f7f8 eeea    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x722c
    b1d8: f7ff b9cc    	b.w	0xa574 <gst_lg_audio_render_event+0x2334> @ imm = #-0xc68
    b1dc: 9301         	str	r3, [sp, #0x4]
    b1de: 2103         	movs	r1, #0x3
    b1e0: f8df 285c    	ldr.w	r2, [pc, #0x85c]        @ 0xba40 <gst_lg_audio_render_event+0x3800>
    b1e4: f8df 385c    	ldr.w	r3, [pc, #0x85c]        @ 0xba44 <gst_lg_audio_render_event+0x3804>
    b1e8: 447a         	add	r2, pc
    b1ea: 447b         	add	r3, pc
    b1ec: f502 72c6    	add.w	r2, r2, #0x18c
    b1f0: 9300         	str	r3, [sp]
    b1f2: f44f 63a2    	mov.w	r3, #0x510
    b1f6: f7f8 eeda    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x724c
    b1fa: f7fe b9b5    	b.w	0x9568 <gst_lg_audio_render_event+0x1328> @ imm = #-0x1c96
    b1fe: f8df 2848    	ldr.w	r2, [pc, #0x848]        @ 0xba48 <gst_lg_audio_render_event+0x3808>
    b202: 2103         	movs	r1, #0x3
    b204: f8df 3844    	ldr.w	r3, [pc, #0x844]        @ 0xba4c <gst_lg_audio_render_event+0x380c>
    b208: 447a         	add	r2, pc
    b20a: 447b         	add	r3, pc
    b20c: f502 72b8    	add.w	r2, r2, #0x170
    b210: 9300         	str	r3, [sp]
    b212: f640 03df    	movw	r3, #0x8df
    b216: f7f8 eeca    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x726c
    b21a: e5d4         	b	0xadc6 <gst_lg_audio_render_event+0x2b86> @ imm = #-0x458
    b21c: f8df 2830    	ldr.w	r2, [pc, #0x830]        @ 0xba50 <gst_lg_audio_render_event+0x3810>
    b220: 2103         	movs	r1, #0x3
    b222: f8df 3830    	ldr.w	r3, [pc, #0x830]        @ 0xba54 <gst_lg_audio_render_event+0x3814>
    b226: 447a         	add	r2, pc
    b228: 447b         	add	r3, pc
    b22a: f502 72c6    	add.w	r2, r2, #0x18c
    b22e: 9300         	str	r3, [sp]
    b230: f240 5314    	movw	r3, #0x514
    b234: f7f8 eeba    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x728c
    b238: f7ff b8d3    	b.w	0xa3e2 <gst_lg_audio_render_event+0x21a2> @ imm = #-0xe5a
    b23c: f8df 2818    	ldr.w	r2, [pc, #0x818]        @ 0xba58 <gst_lg_audio_render_event+0x3818>
    b240: 2104         	movs	r1, #0x4
    b242: f8df 3818    	ldr.w	r3, [pc, #0x818]        @ 0xba5c <gst_lg_audio_render_event+0x381c>
    b246: 447a         	add	r2, pc
    b248: 447b         	add	r3, pc
    b24a: f502 72c6    	add.w	r2, r2, #0x18c
    b24e: 9300         	str	r3, [sp]
    b250: f240 532b    	movw	r3, #0x52b
    b254: f7f8 eeaa    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x72ac
    b258: e42f         	b	0xaaba <gst_lg_audio_render_event+0x287a> @ imm = #-0x7a2
    b25a: 9301         	str	r3, [sp, #0x4]
    b25c: 2104         	movs	r1, #0x4
    b25e: f8df 2800    	ldr.w	r2, [pc, #0x800]        @ 0xba60 <gst_lg_audio_render_event+0x3820>
    b262: f8df 3800    	ldr.w	r3, [pc, #0x800]        @ 0xba64 <gst_lg_audio_render_event+0x3824>
    b266: 447a         	add	r2, pc
    b268: 447b         	add	r3, pc
    b26a: f502 72c6    	add.w	r2, r2, #0x18c
    b26e: 9300         	str	r3, [sp]
    b270: f240 43e7    	movw	r3, #0x4e7
    b274: f7f8 ee9a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x72cc
    b278: f7fe b877    	b.w	0x936a <gst_lg_audio_render_event+0x112a> @ imm = #-0x1f12
    b27c: f8df 27e8    	ldr.w	r2, [pc, #0x7e8]        @ 0xba68 <gst_lg_audio_render_event+0x3828>
    b280: 4618         	mov	r0, r3
    b282: f8df 37e8    	ldr.w	r3, [pc, #0x7e8]        @ 0xba6c <gst_lg_audio_render_event+0x382c>
    b286: 447a         	add	r2, pc
    b288: 447b         	add	r3, pc
    b28a: f502 72c6    	add.w	r2, r2, #0x18c
    b28e: 9300         	str	r3, [sp]
    b290: f240 43c2    	movw	r3, #0x4c2
    b294: f7f8 ee8a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x72ec
    b298: f7ff b946    	b.w	0xa528 <gst_lg_audio_render_event+0x22e8> @ imm = #-0xd74
    b29c: f8df 27d0    	ldr.w	r2, [pc, #0x7d0]        @ 0xba70 <gst_lg_audio_render_event+0x3830>
    b2a0: 2104         	movs	r1, #0x4
    b2a2: f8df 37d0    	ldr.w	r3, [pc, #0x7d0]        @ 0xba74 <gst_lg_audio_render_event+0x3834>
    b2a6: 447a         	add	r2, pc
    b2a8: 447b         	add	r3, pc
    b2aa: f502 72c6    	add.w	r2, r2, #0x18c
    b2ae: 9300         	str	r3, [sp]
    b2b0: f240 43f1    	movw	r3, #0x4f1
    b2b4: f7f8 ee7a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x730c
    b2b8: f7ff b8ec    	b.w	0xa494 <gst_lg_audio_render_event+0x2254> @ imm = #-0xe28
    b2bc: 9301         	str	r3, [sp, #0x4]
    b2be: 2104         	movs	r1, #0x4
    b2c0: f8df 27b4    	ldr.w	r2, [pc, #0x7b4]        @ 0xba78 <gst_lg_audio_render_event+0x3838>
    b2c4: f8df 37b4    	ldr.w	r3, [pc, #0x7b4]        @ 0xba7c <gst_lg_audio_render_event+0x383c>
    b2c8: 447a         	add	r2, pc
    b2ca: 447b         	add	r3, pc
    b2cc: f502 72c6    	add.w	r2, r2, #0x18c
    b2d0: 9300         	str	r3, [sp]
    b2d2: f240 43ef    	movw	r3, #0x4ef
    b2d6: f7f8 ee6a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x732c
    b2da: f7fe b877    	b.w	0x93cc <gst_lg_audio_render_event+0x118c> @ imm = #-0x1f12
    b2de: f8df 27a0    	ldr.w	r2, [pc, #0x7a0]        @ 0xba80 <gst_lg_audio_render_event+0x3840>
    b2e2: 2104         	movs	r1, #0x4
    b2e4: f8df 379c    	ldr.w	r3, [pc, #0x79c]        @ 0xba84 <gst_lg_audio_render_event+0x3844>
    b2e8: 447a         	add	r2, pc
    b2ea: 447b         	add	r3, pc
    b2ec: f502 72c6    	add.w	r2, r2, #0x18c
    b2f0: 9300         	str	r3, [sp]
    b2f2: f240 43f9    	movw	r3, #0x4f9
    b2f6: f7f8 ee5a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x734c
    b2fa: f7ff b8a4    	b.w	0xa446 <gst_lg_audio_render_event+0x2206> @ imm = #-0xeb8
    b2fe: 9301         	str	r3, [sp, #0x4]
    b300: 2104         	movs	r1, #0x4
    b302: f8df 2784    	ldr.w	r2, [pc, #0x784]        @ 0xba88 <gst_lg_audio_render_event+0x3848>
    b306: f8df 3784    	ldr.w	r3, [pc, #0x784]        @ 0xba8c <gst_lg_audio_render_event+0x384c>
    b30a: 447a         	add	r2, pc
    b30c: 447b         	add	r3, pc
    b30e: f502 72c6    	add.w	r2, r2, #0x18c
    b312: 9300         	str	r3, [sp]
    b314: f240 43f7    	movw	r3, #0x4f7
    b318: f7f8 ee48    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7370
    b31c: f7fe b887    	b.w	0x942e <gst_lg_audio_render_event+0x11ee> @ imm = #-0x1ef2
    b320: 9301         	str	r3, [sp, #0x4]
    b322: 2104         	movs	r1, #0x4
    b324: f8df 2768    	ldr.w	r2, [pc, #0x768]        @ 0xba90 <gst_lg_audio_render_event+0x3850>
    b328: f8df 3768    	ldr.w	r3, [pc, #0x768]        @ 0xba94 <gst_lg_audio_render_event+0x3854>
    b32c: 447a         	add	r2, pc
    b32e: 447b         	add	r3, pc
    b330: f502 72c6    	add.w	r2, r2, #0x18c
    b334: 9300         	str	r3, [sp]
    b336: f240 43cc    	movw	r3, #0x4cc
    b33a: f7f8 ee38    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7390
    b33e: f7fd bfe3    	b.w	0x9308 <gst_lg_audio_render_event+0x10c8> @ imm = #-0x203a
    b342: 9301         	str	r3, [sp, #0x4]
    b344: 2100         	movs	r1, #0x0
    b346: f8df 2750    	ldr.w	r2, [pc, #0x750]        @ 0xba98 <gst_lg_audio_render_event+0x3858>
    b34a: f8df 3750    	ldr.w	r3, [pc, #0x750]        @ 0xba9c <gst_lg_audio_render_event+0x385c>
    b34e: 447a         	add	r2, pc
    b350: 447b         	add	r3, pc
    b352: f502 72c6    	add.w	r2, r2, #0x18c
    b356: 9300         	str	r3, [sp]
    b358: f240 537e    	movw	r3, #0x57e
    b35c: f7f8 ee26    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x73b4
    b360: f7fe bb81    	b.w	0x9a66 <gst_lg_audio_render_event+0x1826> @ imm = #-0x18fe
    b364: 9201         	str	r2, [sp, #0x4]
    b366: 4649         	mov	r1, r9
    b368: f8df 2734    	ldr.w	r2, [pc, #0x734]        @ 0xbaa0 <gst_lg_audio_render_event+0x3860>
    b36c: f8df 3734    	ldr.w	r3, [pc, #0x734]        @ 0xbaa4 <gst_lg_audio_render_event+0x3864>
    b370: 447a         	add	r2, pc
    b372: 447b         	add	r3, pc
    b374: f502 72c6    	add.w	r2, r2, #0x18c
    b378: 9300         	str	r3, [sp]
    b37a: f44f 63af    	mov.w	r3, #0x578
    b37e: f7f8 ee16    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x73d4
    b382: f7fe bb4a    	b.w	0x9a1a <gst_lg_audio_render_event+0x17da> @ imm = #-0x196c
    b386: 9301         	str	r3, [sp, #0x4]
    b388: 2104         	movs	r1, #0x4
    b38a: f8df 271c    	ldr.w	r2, [pc, #0x71c]        @ 0xbaa8 <gst_lg_audio_render_event+0x3868>
    b38e: f8df 371c    	ldr.w	r3, [pc, #0x71c]        @ 0xbaac <gst_lg_audio_render_event+0x386c>
    b392: 447a         	add	r2, pc
    b394: 447b         	add	r3, pc
    b396: f502 72c6    	add.w	r2, r2, #0x18c
    b39a: 9300         	str	r3, [sp]
    b39c: f240 5355    	movw	r3, #0x555
    b3a0: f7f8 ee04    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x73f8
    b3a4: f7fe ba16    	b.w	0x97d4 <gst_lg_audio_render_event+0x1594> @ imm = #-0x1bd4
    b3a8: 9301         	str	r3, [sp, #0x4]
    b3aa: 2104         	movs	r1, #0x4
    b3ac: f8df 2700    	ldr.w	r2, [pc, #0x700]        @ 0xbab0 <gst_lg_audio_render_event+0x3870>
    b3b0: f8df 3700    	ldr.w	r3, [pc, #0x700]        @ 0xbab4 <gst_lg_audio_render_event+0x3874>
    b3b4: 447a         	add	r2, pc
    b3b6: 447b         	add	r3, pc
    b3b8: f502 72c6    	add.w	r2, r2, #0x18c
    b3bc: 9300         	str	r3, [sp]
    b3be: f240 535a    	movw	r3, #0x55a
    b3c2: f7f8 edf4    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7418
    b3c6: f7fe ba34    	b.w	0x9832 <gst_lg_audio_render_event+0x15f2> @ imm = #-0x1b98
    b3ca: 9301         	str	r3, [sp, #0x4]
    b3cc: 2104         	movs	r1, #0x4
    b3ce: f8df 26e8    	ldr.w	r2, [pc, #0x6e8]        @ 0xbab8 <gst_lg_audio_render_event+0x3878>
    b3d2: f8df 36e8    	ldr.w	r3, [pc, #0x6e8]        @ 0xbabc <gst_lg_audio_render_event+0x387c>
    b3d6: 447a         	add	r2, pc
    b3d8: 447b         	add	r3, pc
    b3da: f502 72c6    	add.w	r2, r2, #0x18c
    b3de: 9300         	str	r3, [sp]
    b3e0: f240 535f    	movw	r3, #0x55f
    b3e4: f7f8 ede2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x743c
    b3e8: f7fe ba51    	b.w	0x988e <gst_lg_audio_render_event+0x164e> @ imm = #-0x1b5e
    b3ec: 9301         	str	r3, [sp, #0x4]
    b3ee: 2104         	movs	r1, #0x4
    b3f0: f8df 26cc    	ldr.w	r2, [pc, #0x6cc]        @ 0xbac0 <gst_lg_audio_render_event+0x3880>
    b3f4: f8df 36cc    	ldr.w	r3, [pc, #0x6cc]        @ 0xbac4 <gst_lg_audio_render_event+0x3884>
    b3f8: 447a         	add	r2, pc
    b3fa: 447b         	add	r3, pc
    b3fc: f502 72c6    	add.w	r2, r2, #0x18c
    b400: 9300         	str	r3, [sp]
    b402: f240 5364    	movw	r3, #0x564
    b406: f7f8 edd2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x745c
    b40a: f7fe ba6e    	b.w	0x98ea <gst_lg_audio_render_event+0x16aa> @ imm = #-0x1b24
    b40e: f8df 26b8    	ldr.w	r2, [pc, #0x6b8]        @ 0xbac8 <gst_lg_audio_render_event+0x3888>
    b412: 2104         	movs	r1, #0x4
    b414: f8df 36b4    	ldr.w	r3, [pc, #0x6b4]        @ 0xbacc <gst_lg_audio_render_event+0x388c>
    b418: 447a         	add	r2, pc
    b41a: 447b         	add	r3, pc
    b41c: f502 72c6    	add.w	r2, r2, #0x18c
    b420: 9300         	str	r3, [sp]
    b422: f240 5333    	movw	r3, #0x533
    b426: f7f8 edc2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x747c
    b42a: f7ff bb24    	b.w	0xaa76 <gst_lg_audio_render_event+0x2836> @ imm = #-0x9b8
    b42e: 9301         	str	r3, [sp, #0x4]
    b430: 2104         	movs	r1, #0x4
    b432: f8df 269c    	ldr.w	r2, [pc, #0x69c]        @ 0xbad0 <gst_lg_audio_render_event+0x3890>
    b436: f8df 369c    	ldr.w	r3, [pc, #0x69c]        @ 0xbad4 <gst_lg_audio_render_event+0x3894>
    b43a: 447a         	add	r2, pc
    b43c: 447b         	add	r3, pc
    b43e: f502 72c6    	add.w	r2, r2, #0x18c
    b442: 9300         	str	r3, [sp]
    b444: f240 5329    	movw	r3, #0x529
    b448: f7f8 edb0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x74a0
    b44c: f7fe b902    	b.w	0x9654 <gst_lg_audio_render_event+0x1414> @ imm = #-0x1dfc
    b450: f8df 2684    	ldr.w	r2, [pc, #0x684]        @ 0xbad8 <gst_lg_audio_render_event+0x3898>
    b454: 2104         	movs	r1, #0x4
    b456: f8df 3684    	ldr.w	r3, [pc, #0x684]        @ 0xbadc <gst_lg_audio_render_event+0x389c>
    b45a: 447a         	add	r2, pc
    b45c: 447b         	add	r3, pc
    b45e: f502 72c6    	add.w	r2, r2, #0x18c
    b462: 9300         	str	r3, [sp]
    b464: f240 534f    	movw	r3, #0x54f
    b468: f7f8 eda0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x74c0
    b46c: f7ff bae1    	b.w	0xaa32 <gst_lg_audio_render_event+0x27f2> @ imm = #-0xa3e
    b470: 9301         	str	r3, [sp, #0x4]
    b472: 2104         	movs	r1, #0x4
    b474: f8df 2668    	ldr.w	r2, [pc, #0x668]        @ 0xbae0 <gst_lg_audio_render_event+0x38a0>
    b478: f8df 3668    	ldr.w	r3, [pc, #0x668]        @ 0xbae4 <gst_lg_audio_render_event+0x38a4>
    b47c: 447a         	add	r2, pc
    b47e: 447b         	add	r3, pc
    b480: f502 72c6    	add.w	r2, r2, #0x18c
    b484: 9300         	str	r3, [sp]
    b486: f240 534d    	movw	r3, #0x54d
    b48a: f7f8 ed90    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x74e0
    b48e: f7fe b972    	b.w	0x9776 <gst_lg_audio_render_event+0x1536> @ imm = #-0x1d1c
    b492: f8df 2654    	ldr.w	r2, [pc, #0x654]        @ 0xbae8 <gst_lg_audio_render_event+0x38a8>
    b496: 2104         	movs	r1, #0x4
    b498: f8df 3650    	ldr.w	r3, [pc, #0x650]        @ 0xbaec <gst_lg_audio_render_event+0x38ac>
    b49c: 447a         	add	r2, pc
    b49e: 447b         	add	r3, pc
    b4a0: f502 72c6    	add.w	r2, r2, #0x18c
    b4a4: 9300         	str	r3, [sp]
    b4a6: f240 5371    	movw	r3, #0x571
    b4aa: f7f8 ed80    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7500
    b4ae: f7ff ba9e    	b.w	0xa9ee <gst_lg_audio_render_event+0x27ae> @ imm = #-0xac4
    b4b2: 9301         	str	r3, [sp, #0x4]
    b4b4: 2104         	movs	r1, #0x4
    b4b6: f8df 2638    	ldr.w	r2, [pc, #0x638]        @ 0xbaf0 <gst_lg_audio_render_event+0x38b0>
    b4ba: f8df 3638    	ldr.w	r3, [pc, #0x638]        @ 0xbaf4 <gst_lg_audio_render_event+0x38b4>
    b4be: 447a         	add	r2, pc
    b4c0: 447b         	add	r3, pc
    b4c2: f502 72c6    	add.w	r2, r2, #0x18c
    b4c6: 9300         	str	r3, [sp]
    b4c8: f240 536f    	movw	r3, #0x56f
    b4cc: f7f8 ed6e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7524
    b4d0: f7fe ba74    	b.w	0x99bc <gst_lg_audio_render_event+0x177c> @ imm = #-0x1b18
    b4d4: 9301         	str	r3, [sp, #0x4]
    b4d6: 2104         	movs	r1, #0x4
    b4d8: f8df 261c    	ldr.w	r2, [pc, #0x61c]        @ 0xbaf8 <gst_lg_audio_render_event+0x38b8>
    b4dc: f8df 361c    	ldr.w	r3, [pc, #0x61c]        @ 0xbafc <gst_lg_audio_render_event+0x38bc>
    b4e0: 447a         	add	r2, pc
    b4e2: 447b         	add	r3, pc
    b4e4: f502 72c6    	add.w	r2, r2, #0x18c
    b4e8: 9300         	str	r3, [sp]
    b4ea: f240 5331    	movw	r3, #0x531
    b4ee: f7f8 ed5e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7544
    b4f2: f7fe b8e0    	b.w	0x96b6 <gst_lg_audio_render_event+0x1476> @ imm = #-0x1e40
    b4f6: 9301         	str	r3, [sp, #0x4]
    b4f8: 2103         	movs	r1, #0x3
    b4fa: f8df 2604    	ldr.w	r2, [pc, #0x604]        @ 0xbb00 <gst_lg_audio_render_event+0x38c0>
    b4fe: f8df 3604    	ldr.w	r3, [pc, #0x604]        @ 0xbb04 <gst_lg_audio_render_event+0x38c4>
    b502: 447a         	add	r2, pc
    b504: 447b         	add	r3, pc
    b506: f502 72c6    	add.w	r2, r2, #0x18c
    b50a: 9300         	str	r3, [sp]
    b50c: f44f 63a9    	mov.w	r3, #0x548
    b510: f7f8 ed4c    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7568
    b514: f7fe b8fe    	b.w	0x9714 <gst_lg_audio_render_event+0x14d4> @ imm = #-0x1e04
    b518: 6812         	ldr	r2, [r2]
    b51a: 07d1         	lsls	r1, r2, #0x1f
    b51c: f100 81a6    	bmi.w	0xb86c <gst_lg_audio_render_event+0x362c> @ imm = #0x34c
    b520: f8d8 3000    	ldr.w	r3, [r8]
    b524: 2b00         	cmp	r3, #0x0
    b526: f43d aedb    	beq.w	0x92e0 <gst_lg_audio_render_event+0x10a0> @ imm = #-0x224a
    b52a: f8df 3428    	ldr.w	r3, [pc, #0x428]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    b52e: 2101         	movs	r1, #0x1
    b530: f8df 25d4    	ldr.w	r2, [pc, #0x5d4]        @ 0xbb08 <gst_lg_audio_render_event+0x38c8>
    b534: 58fb         	ldr	r3, [r7, r3]
    b536: 447a         	add	r2, pc
    b538: 9401         	str	r4, [sp, #0x4]
    b53a: 6818         	ldr	r0, [r3]
    b53c: 9202         	str	r2, [sp, #0x8]
    b53e: f240 42bf    	movw	r2, #0x4bf
    b542: f8df 35c8    	ldr.w	r3, [pc, #0x5c8]        @ 0xbb0c <gst_lg_audio_render_event+0x38cc>
    b546: 9200         	str	r2, [sp]
    b548: f8df 25c4    	ldr.w	r2, [pc, #0x5c4]        @ 0xbb10 <gst_lg_audio_render_event+0x38d0>
    b54c: 447b         	add	r3, pc
    b54e: f503 73c6    	add.w	r3, r3, #0x18c
    b552: 447a         	add	r2, pc
    b554: f7f8 eb8a    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x78ec
    b558: f7fd bec2    	b.w	0x92e0 <gst_lg_audio_render_event+0x10a0> @ imm = #-0x227c
    b55c: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    b560: f10d 09cc    	add.w	r9, sp, #0xcc
    b564: f7ff b886    	b.w	0xa674 <gst_lg_audio_render_event+0x2434> @ imm = #-0xef4
    b568: e9cd 3201    	strd	r3, r2, [sp, #4]
    b56c: 2103         	movs	r1, #0x3
    b56e: f8df 25a4    	ldr.w	r2, [pc, #0x5a4]        @ 0xbb14 <gst_lg_audio_render_event+0x38d4>
    b572: f8df 35a4    	ldr.w	r3, [pc, #0x5a4]        @ 0xbb18 <gst_lg_audio_render_event+0x38d8>
    b576: 447a         	add	r2, pc
    b578: 447b         	add	r3, pc
    b57a: f502 72c6    	add.w	r2, r2, #0x18c
    b57e: 9300         	str	r3, [sp]
    b580: f44f 63bb    	mov.w	r3, #0x5d8
    b584: f7f8 ed12    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x75dc
    b588: f7ff b8cd    	b.w	0xa726 <gst_lg_audio_render_event+0x24e6> @ imm = #-0xe66
    b58c: f8df 258c    	ldr.w	r2, [pc, #0x58c]        @ 0xbb1c <gst_lg_audio_render_event+0x38dc>
    b590: 4659         	mov	r1, r11
    b592: f8df 358c    	ldr.w	r3, [pc, #0x58c]        @ 0xbb20 <gst_lg_audio_render_event+0x38e0>
    b596: 447a         	add	r2, pc
    b598: 447b         	add	r3, pc
    b59a: f502 72c6    	add.w	r2, r2, #0x18c
    b59e: 9300         	str	r3, [sp]
    b5a0: f240 438c    	movw	r3, #0x48c
    b5a4: f7f8 ed02    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x75fc
    b5a8: e441         	b	0xae2e <gst_lg_audio_render_event+0x2bee> @ imm = #-0x77e
    b5aa: f8df 0578    	ldr.w	r0, [pc, #0x578]        @ 0xbb24 <gst_lg_audio_render_event+0x38e4>
    b5ae: 4478         	add	r0, pc
    b5b0: 68c3         	ldr	r3, [r0, #0xc]
    b5b2: 681b         	ldr	r3, [r3]
    b5b4: 06d9         	lsls	r1, r3, #0x1b
    b5b6: f100 81a1    	bmi.w	0xb8fc <gst_lg_audio_render_event+0x36bc> @ imm = #0x342
    b5ba: f8d8 3000    	ldr.w	r3, [r8]
    b5be: 2b04         	cmp	r3, #0x4
    b5c0: f67d aebf    	bls.w	0x9342 <gst_lg_audio_render_event+0x1102> @ imm = #-0x2282
    b5c4: 4be3         	ldr	r3, [pc, #0x38c]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    b5c6: 2105         	movs	r1, #0x5
    b5c8: f8df 255c    	ldr.w	r2, [pc, #0x55c]        @ 0xbb28 <gst_lg_audio_render_event+0x38e8>
    b5cc: 58fb         	ldr	r3, [r7, r3]
    b5ce: 447a         	add	r2, pc
    b5d0: 9401         	str	r4, [sp, #0x4]
    b5d2: 6818         	ldr	r0, [r3]
    b5d4: 9202         	str	r2, [sp, #0x8]
    b5d6: f44f 629c    	mov.w	r2, #0x4e0
    b5da: f8df 3550    	ldr.w	r3, [pc, #0x550]        @ 0xbb2c <gst_lg_audio_render_event+0x38ec>
    b5de: 9200         	str	r2, [sp]
    b5e0: f8df 254c    	ldr.w	r2, [pc, #0x54c]        @ 0xbb30 <gst_lg_audio_render_event+0x38f0>
    b5e4: 447b         	add	r3, pc
    b5e6: f503 73c6    	add.w	r3, r3, #0x18c
    b5ea: 447a         	add	r2, pc
    b5ec: f7f8 eb3e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x7984
    b5f0: f7fd bea7    	b.w	0x9342 <gst_lg_audio_render_event+0x1102> @ imm = #-0x22b2
    b5f4: f8df 253c    	ldr.w	r2, [pc, #0x53c]        @ 0xbb34 <gst_lg_audio_render_event+0x38f4>
    b5f8: 2100         	movs	r1, #0x0
    b5fa: f8df 353c    	ldr.w	r3, [pc, #0x53c]        @ 0xbb38 <gst_lg_audio_render_event+0x38f8>
    b5fe: 447a         	add	r2, pc
    b600: 447b         	add	r3, pc
    b602: f502 72c6    	add.w	r2, r2, #0x18c
    b606: 9300         	str	r3, [sp]
    b608: f240 5385    	movw	r3, #0x585
    b60c: f7f8 ecce    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7664
    b610: f7fe ba4c    	b.w	0x9aac <gst_lg_audio_render_event+0x186c> @ imm = #-0x1b68
    b614: f7f8 eb36    	blx	0x3c84 <lgao_open_renderer@plt> @ imm = #-0x7994
    b618: b1d0         	cbz	r0, 0xb650 <gst_lg_audio_render_event+0x3410> @ imm = #0x34
    b61a: f8d8 3000    	ldr.w	r3, [r8]
    b61e: 2b01         	cmp	r3, #0x1
    b620: d916         	bls	0xb650 <gst_lg_audio_render_event+0x3410> @ imm = #0x2c
    b622: 4bcc         	ldr	r3, [pc, #0x330]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    b624: 2102         	movs	r1, #0x2
    b626: f8df 2514    	ldr.w	r2, [pc, #0x514]        @ 0xbb3c <gst_lg_audio_render_event+0x38fc>
    b62a: 58fb         	ldr	r3, [r7, r3]
    b62c: 447a         	add	r2, pc
    b62e: f8cd 9004    	str.w	r9, [sp, #0x4]
    b632: 6818         	ldr	r0, [r3]
    b634: f8df 3508    	ldr.w	r3, [pc, #0x508]        @ 0xbb40 <gst_lg_audio_render_event+0x3900>
    b638: 9202         	str	r2, [sp, #0x8]
    b63a: f240 52bb    	movw	r2, #0x5bb
    b63e: 9200         	str	r2, [sp]
    b640: f8df 2500    	ldr.w	r2, [pc, #0x500]        @ 0xbb44 <gst_lg_audio_render_event+0x3904>
    b644: 447b         	add	r3, pc
    b646: f503 73c6    	add.w	r3, r3, #0x18c
    b64a: 447a         	add	r2, pc
    b64c: f7f8 eb0e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x79e4
    b650: f10d 09cc    	add.w	r9, sp, #0xcc
    b654: 2250         	movs	r2, #0x50
    b656: 4649         	mov	r1, r9
    b658: 4668         	mov	r0, sp
    b65a: f8d4 a200    	ldr.w	r10, [r4, #0x200]
    b65e: f7f8 eb5a    	blx	0x3d14 <memcpy@plt>     @ imm = #-0x794c
    b662: 9b14         	ldr	r3, [sp, #0x50]
    b664: 4650         	mov	r0, r10
    b666: cb0e         	ldm	r3, {r1, r2, r3}
    b668: f7f8 ec80    	blx	0x3f6c <lgao_set_audio_info@plt> @ imm = #-0x7700
    b66c: b1d8         	cbz	r0, 0xb6a6 <gst_lg_audio_render_event+0x3466> @ imm = #0x36
    b66e: f8d8 3000    	ldr.w	r3, [r8]
    b672: 2b01         	cmp	r3, #0x1
    b674: d917         	bls	0xb6a6 <gst_lg_audio_render_event+0x3466> @ imm = #0x2e
    b676: 4bb7         	ldr	r3, [pc, #0x2dc]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    b678: f04f 0e00    	mov.w	lr, #0x0
    b67c: f8df 24c8    	ldr.w	r2, [pc, #0x4c8]        @ 0xbb48 <gst_lg_audio_render_event+0x3908>
    b680: 2102         	movs	r1, #0x2
    b682: 58fb         	ldr	r3, [r7, r3]
    b684: 447a         	add	r2, pc
    b686: 6818         	ldr	r0, [r3]
    b688: f8df 34c0    	ldr.w	r3, [pc, #0x4c0]        @ 0xbb4c <gst_lg_audio_render_event+0x390c>
    b68c: 9202         	str	r2, [sp, #0x8]
    b68e: f240 52bc    	movw	r2, #0x5bc
    b692: e9cd 2e00    	strd	r2, lr, [sp]
    b696: f8df 24b8    	ldr.w	r2, [pc, #0x4b8]        @ 0xbb50 <gst_lg_audio_render_event+0x3910>
    b69a: 447b         	add	r3, pc
    b69c: f503 73c6    	add.w	r3, r3, #0x18c
    b6a0: 447a         	add	r2, pc
    b6a2: f7f8 eae4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x7a38
    b6a6: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    b6aa: f7f8 e894    	blx	0x37d4 <lgao_config@plt> @ imm = #-0x7ed8
    b6ae: b1d8         	cbz	r0, 0xb6e8 <gst_lg_audio_render_event+0x34a8> @ imm = #0x36
    b6b0: f8d8 3000    	ldr.w	r3, [r8]
    b6b4: 2b01         	cmp	r3, #0x1
    b6b6: d917         	bls	0xb6e8 <gst_lg_audio_render_event+0x34a8> @ imm = #0x2e
    b6b8: 4ba6         	ldr	r3, [pc, #0x298]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    b6ba: f04f 0c00    	mov.w	r12, #0x0
    b6be: f8df 2494    	ldr.w	r2, [pc, #0x494]        @ 0xbb54 <gst_lg_audio_render_event+0x3914>
    b6c2: 2102         	movs	r1, #0x2
    b6c4: 58fb         	ldr	r3, [r7, r3]
    b6c6: 447a         	add	r2, pc
    b6c8: 6818         	ldr	r0, [r3]
    b6ca: f8df 348c    	ldr.w	r3, [pc, #0x48c]        @ 0xbb58 <gst_lg_audio_render_event+0x3918>
    b6ce: 9202         	str	r2, [sp, #0x8]
    b6d0: f240 52bd    	movw	r2, #0x5bd
    b6d4: e9cd 2c00    	strd	r2, r12, [sp]
    b6d8: f8df 2480    	ldr.w	r2, [pc, #0x480]        @ 0xbb5c <gst_lg_audio_render_event+0x391c>
    b6dc: 447b         	add	r3, pc
    b6de: f503 73c6    	add.w	r3, r3, #0x18c
    b6e2: 447a         	add	r2, pc
    b6e4: f7f8 eac2    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x7a7c
    b6e8: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    b6ec: f7f8 ecea    	blx	0x40c4 <lgao_connect_renderer@plt> @ imm = #-0x762c
    b6f0: b1d0         	cbz	r0, 0xb728 <gst_lg_audio_render_event+0x34e8> @ imm = #0x34
    b6f2: f8d8 3000    	ldr.w	r3, [r8]
    b6f6: 2b01         	cmp	r3, #0x1
    b6f8: d916         	bls	0xb728 <gst_lg_audio_render_event+0x34e8> @ imm = #0x2c
    b6fa: 4b96         	ldr	r3, [pc, #0x258]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    b6fc: 2100         	movs	r1, #0x0
    b6fe: f8df 2460    	ldr.w	r2, [pc, #0x460]        @ 0xbb60 <gst_lg_audio_render_event+0x3920>
    b702: 58fb         	ldr	r3, [r7, r3]
    b704: 447a         	add	r2, pc
    b706: 6818         	ldr	r0, [r3]
    b708: f8df 3458    	ldr.w	r3, [pc, #0x458]        @ 0xbb64 <gst_lg_audio_render_event+0x3924>
    b70c: 9202         	str	r2, [sp, #0x8]
    b70e: f240 52be    	movw	r2, #0x5be
    b712: e9cd 2100    	strd	r2, r1, [sp]
    b716: 2102         	movs	r1, #0x2
    b718: f8df 244c    	ldr.w	r2, [pc, #0x44c]        @ 0xbb68 <gst_lg_audio_render_event+0x3928>
    b71c: 447b         	add	r3, pc
    b71e: f503 73c6    	add.w	r3, r3, #0x18c
    b722: 447a         	add	r2, pc
    b724: f7f8 eaa2    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x7abc
    b728: f8d4 3218    	ldr.w	r3, [r4, #0x218]
    b72c: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    b730: 2b04         	cmp	r3, #0x4
    b732: f47f a8e1    	bne.w	0xa8f8 <gst_lg_audio_render_event+0x26b8> @ imm = #-0xe3e
    b736: f7f8 e98c    	blx	0x3a50 <lgao_play@plt>  @ imm = #-0x7ce8
    b73a: b1d8         	cbz	r0, 0xb774 <gst_lg_audio_render_event+0x3534> @ imm = #0x36
    b73c: f8d8 3000    	ldr.w	r3, [r8]
    b740: 2b01         	cmp	r3, #0x1
    b742: d917         	bls	0xb774 <gst_lg_audio_render_event+0x3534> @ imm = #0x2e
    b744: 4b83         	ldr	r3, [pc, #0x20c]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    b746: f04f 0b00    	mov.w	r11, #0x0
    b74a: f8df 2420    	ldr.w	r2, [pc, #0x420]        @ 0xbb6c <gst_lg_audio_render_event+0x392c>
    b74e: 2102         	movs	r1, #0x2
    b750: 58fb         	ldr	r3, [r7, r3]
    b752: 447a         	add	r2, pc
    b754: 6818         	ldr	r0, [r3]
    b756: f8df 3418    	ldr.w	r3, [pc, #0x418]        @ 0xbb70 <gst_lg_audio_render_event+0x3930>
    b75a: 9202         	str	r2, [sp, #0x8]
    b75c: f240 52c2    	movw	r2, #0x5c2
    b760: e9cd 2b00    	strd	r2, r11, [sp]
    b764: f8df 240c    	ldr.w	r2, [pc, #0x40c]        @ 0xbb74 <gst_lg_audio_render_event+0x3934>
    b768: 447b         	add	r3, pc
    b76a: f503 73c6    	add.w	r3, r3, #0x18c
    b76e: 447a         	add	r2, pc
    b770: f7f8 ea7c    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x7b08
    b774: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    b778: f7f8 e87a    	blx	0x3870 <lgao_start_renderer@plt> @ imm = #-0x7f0c
    b77c: b1c0         	cbz	r0, 0xb7b0 <gst_lg_audio_render_event+0x3570> @ imm = #0x30
    b77e: f8d8 3000    	ldr.w	r3, [r8]
    b782: 2b01         	cmp	r3, #0x1
    b784: d914         	bls	0xb7b0 <gst_lg_audio_render_event+0x3570> @ imm = #0x28
    b786: 4b73         	ldr	r3, [pc, #0x1cc]        @ 0xb954 <gst_lg_audio_render_event+0x3714>
    b788: f04f 0a00    	mov.w	r10, #0x0
    b78c: 4afa         	ldr	r2, [pc, #0x3e8]        @ 0xbb78 <gst_lg_audio_render_event+0x3938>
    b78e: 2102         	movs	r1, #0x2
    b790: 58fb         	ldr	r3, [r7, r3]
    b792: 447a         	add	r2, pc
    b794: 6818         	ldr	r0, [r3]
    b796: 4bf9         	ldr	r3, [pc, #0x3e4]        @ 0xbb7c <gst_lg_audio_render_event+0x393c>
    b798: 9202         	str	r2, [sp, #0x8]
    b79a: f240 52c3    	movw	r2, #0x5c3
    b79e: e9cd 2a00    	strd	r2, r10, [sp]
    b7a2: 4af7         	ldr	r2, [pc, #0x3dc]        @ 0xbb80 <gst_lg_audio_render_event+0x3940>
    b7a4: 447b         	add	r3, pc
    b7a6: f503 73c6    	add.w	r3, r3, #0x18c
    b7aa: 447a         	add	r2, pc
    b7ac: f7f8 ea5e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x7b44
    b7b0: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    b7b4: f7ff b8a0    	b.w	0xa8f8 <gst_lg_audio_render_event+0x26b8> @ imm = #-0xec0
    b7b8: e9cd 3301    	strd	r3, r3, [sp, #4]
    b7bc: 2104         	movs	r1, #0x4
    b7be: 4af1         	ldr	r2, [pc, #0x3c4]        @ 0xbb84 <gst_lg_audio_render_event+0x3944>
    b7c0: 4bf1         	ldr	r3, [pc, #0x3c4]        @ 0xbb88 <gst_lg_audio_render_event+0x3948>
    b7c2: 447a         	add	r2, pc
    b7c4: 447b         	add	r3, pc
    b7c6: f502 72c6    	add.w	r2, r2, #0x18c
    b7ca: 9300         	str	r3, [sp]
    b7cc: f44f 639a    	mov.w	r3, #0x4d0
    b7d0: f7f8 ebec    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7828
    b7d4: f7ff bacc    	b.w	0xad70 <gst_lg_audio_render_event+0x2b30> @ imm = #-0xa68
    b7d8: 4aec         	ldr	r2, [pc, #0x3b0]        @ 0xbb8c <gst_lg_audio_render_event+0x394c>
    b7da: 4659         	mov	r1, r11
    b7dc: 4bec         	ldr	r3, [pc, #0x3b0]        @ 0xbb90 <gst_lg_audio_render_event+0x3950>
    b7de: 447a         	add	r2, pc
    b7e0: 447b         	add	r3, pc
    b7e2: f502 72c6    	add.w	r2, r2, #0x18c
    b7e6: 9300         	str	r3, [sp]
    b7e8: f240 43c6    	movw	r3, #0x4c6
    b7ec: f7f8 ebde    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7844
    b7f0: e40c         	b	0xb00c <gst_lg_audio_render_event+0x2dcc> @ imm = #-0x7e8
    b7f2: 4ae8         	ldr	r2, [pc, #0x3a0]        @ 0xbb94 <gst_lg_audio_render_event+0x3954>
    b7f4: 4618         	mov	r0, r3
    b7f6: 4be8         	ldr	r3, [pc, #0x3a0]        @ 0xbb98 <gst_lg_audio_render_event+0x3958>
    b7f8: 2104         	movs	r1, #0x4
    b7fa: 447a         	add	r2, pc
    b7fc: 447b         	add	r3, pc
    b7fe: f502 72c6    	add.w	r2, r2, #0x18c
    b802: 9300         	str	r3, [sp]
    b804: f240 43bc    	movw	r3, #0x4bc
    b808: f7f8 ebd0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7860
    b80c: e497         	b	0xb13e <gst_lg_audio_render_event+0x2efe> @ imm = #-0x6d2
    b80e: 4ae3         	ldr	r2, [pc, #0x38c]        @ 0xbb9c <gst_lg_audio_render_event+0x395c>
    b810: 4618         	mov	r0, r3
    b812: 4be3         	ldr	r3, [pc, #0x38c]        @ 0xbba0 <gst_lg_audio_render_event+0x3960>
    b814: b2c9         	uxtb	r1, r1
    b816: 447a         	add	r2, pc
    b818: 9101         	str	r1, [sp, #0x4]
    b81a: 447b         	add	r3, pc
    b81c: f502 72c6    	add.w	r2, r2, #0x18c
    b820: 9300         	str	r3, [sp]
    b822: 2104         	movs	r1, #0x4
    b824: f240 43ae    	movw	r3, #0x4ae
    b828: f7f8 ebc0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7880
    b82c: f7fd bce1    	b.w	0x91f2 <gst_lg_audio_render_event+0xfb2> @ imm = #-0x263e
    b830: 9301         	str	r3, [sp, #0x4]
    b832: 2104         	movs	r1, #0x4
    b834: 4adb         	ldr	r2, [pc, #0x36c]        @ 0xbba4 <gst_lg_audio_render_event+0x3964>
    b836: 4bdc         	ldr	r3, [pc, #0x370]        @ 0xbba8 <gst_lg_audio_render_event+0x3968>
    b838: 447a         	add	r2, pc
    b83a: 447b         	add	r3, pc
    b83c: f502 72c6    	add.w	r2, r2, #0x18c
    b840: 9300         	str	r3, [sp]
    b842: f240 43ba    	movw	r3, #0x4ba
    b846: f7f8 ebb2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x789c
    b84a: f7fd bd2c    	b.w	0x92a6 <gst_lg_audio_render_event+0x1066> @ imm = #-0x25a8
    b84e: 9301         	str	r3, [sp, #0x4]
    b850: 2104         	movs	r1, #0x4
    b852: 4ad6         	ldr	r2, [pc, #0x358]        @ 0xbbac <gst_lg_audio_render_event+0x396c>
    b854: 4bd6         	ldr	r3, [pc, #0x358]        @ 0xbbb0 <gst_lg_audio_render_event+0x3970>
    b856: 447a         	add	r2, pc
    b858: 447b         	add	r3, pc
    b85a: f502 72c6    	add.w	r2, r2, #0x18c
    b85e: 9300         	str	r3, [sp]
    b860: f240 43b4    	movw	r3, #0x4b4
    b864: f7f8 eba2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x78bc
    b868: f7fd bcf0    	b.w	0x924c <gst_lg_audio_render_event+0x100c> @ imm = #-0x2620
    b86c: 4601         	mov	r1, r0
    b86e: 4ad1         	ldr	r2, [pc, #0x344]        @ 0xbbb4 <gst_lg_audio_render_event+0x3974>
    b870: 4618         	mov	r0, r3
    b872: 4bd1         	ldr	r3, [pc, #0x344]        @ 0xbbb8 <gst_lg_audio_render_event+0x3978>
    b874: 447a         	add	r2, pc
    b876: 447b         	add	r3, pc
    b878: f502 72c6    	add.w	r2, r2, #0x18c
    b87c: 9300         	str	r3, [sp]
    b87e: f240 43bf    	movw	r3, #0x4bf
    b882: f7f8 eb94    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x78d8
    b886: e64b         	b	0xb520 <gst_lg_audio_render_event+0x32e0> @ imm = #-0x36a
    b888: 4acc         	ldr	r2, [pc, #0x330]        @ 0xbbbc <gst_lg_audio_render_event+0x397c>
    b88a: 2102         	movs	r1, #0x2
    b88c: 4bcc         	ldr	r3, [pc, #0x330]        @ 0xbbc0 <gst_lg_audio_render_event+0x3980>
    b88e: 447a         	add	r2, pc
    b890: 447b         	add	r3, pc
    b892: f502 72c6    	add.w	r2, r2, #0x18c
    b896: 9300         	str	r3, [sp]
    b898: f240 53b2    	movw	r3, #0x5b2
    b89c: f7f8 eb86    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x78f4
    b8a0: f7fe bec8    	b.w	0xa634 <gst_lg_audio_render_event+0x23f4> @ imm = #-0x1270
    b8a4: 4ac7         	ldr	r2, [pc, #0x31c]        @ 0xbbc4 <gst_lg_audio_render_event+0x3984>
    b8a6: 2103         	movs	r1, #0x3
    b8a8: 4bc7         	ldr	r3, [pc, #0x31c]        @ 0xbbc8 <gst_lg_audio_render_event+0x3988>
    b8aa: 447a         	add	r2, pc
    b8ac: 447b         	add	r3, pc
    b8ae: f502 72c6    	add.w	r2, r2, #0x18c
    b8b2: 9300         	str	r3, [sp]
    b8b4: f240 53ad    	movw	r3, #0x5ad
    b8b8: f7f8 eb78    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7910
    b8bc: f7ff b826    	b.w	0xa90c <gst_lg_audio_render_event+0x26cc> @ imm = #-0xfb4
    b8c0: 4ac2         	ldr	r2, [pc, #0x308]        @ 0xbbcc <gst_lg_audio_render_event+0x398c>
    b8c2: 2102         	movs	r1, #0x2
    b8c4: 4bc2         	ldr	r3, [pc, #0x308]        @ 0xbbd0 <gst_lg_audio_render_event+0x3990>
    b8c6: 447a         	add	r2, pc
    b8c8: 447b         	add	r3, pc
    b8ca: f502 72c6    	add.w	r2, r2, #0x18c
    b8ce: 9300         	str	r3, [sp]
    b8d0: f240 5393    	movw	r3, #0x593
    b8d4: f7f8 eb6a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x792c
    b8d8: f7fe bfdb    	b.w	0xa892 <gst_lg_audio_render_event+0x2652> @ imm = #-0x104a
    b8dc: e9cd 3201    	strd	r3, r2, [sp, #4]
    b8e0: 2104         	movs	r1, #0x4
    b8e2: 4abc         	ldr	r2, [pc, #0x2f0]        @ 0xbbd4 <gst_lg_audio_render_event+0x3994>
    b8e4: 4bbc         	ldr	r3, [pc, #0x2f0]        @ 0xbbd8 <gst_lg_audio_render_event+0x3998>
    b8e6: 447a         	add	r2, pc
    b8e8: 447b         	add	r3, pc
    b8ea: f502 72c6    	add.w	r2, r2, #0x18c
    b8ee: 9300         	str	r3, [sp]
    b8f0: f240 43dd    	movw	r3, #0x4dd
    b8f4: f7f8 eb5a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x794c
    b8f8: f7ff bb3b    	b.w	0xaf72 <gst_lg_audio_render_event+0x2d32> @ imm = #-0x98a
    b8fc: 4ab7         	ldr	r2, [pc, #0x2dc]        @ 0xbbdc <gst_lg_audio_render_event+0x399c>
    b8fe: 2104         	movs	r1, #0x4
    b900: 4bb7         	ldr	r3, [pc, #0x2dc]        @ 0xbbe0 <gst_lg_audio_render_event+0x39a0>
    b902: 447a         	add	r2, pc
    b904: 447b         	add	r3, pc
    b906: f502 72c6    	add.w	r2, r2, #0x18c
    b90a: 9300         	str	r3, [sp]
    b90c: f44f 639c    	mov.w	r3, #0x4e0
    b910: f7f8 eb4c    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7968
    b914: e651         	b	0xb5ba <gst_lg_audio_render_event+0x337a> @ imm = #-0x35e
    b916: 9301         	str	r3, [sp, #0x4]
    b918: 2102         	movs	r1, #0x2
    b91a: 4ab2         	ldr	r2, [pc, #0x2c8]        @ 0xbbe4 <gst_lg_audio_render_event+0x39a4>
    b91c: 4bb2         	ldr	r3, [pc, #0x2c8]        @ 0xbbe8 <gst_lg_audio_render_event+0x39a8>
    b91e: 447a         	add	r2, pc
    b920: 447b         	add	r3, pc
    b922: f502 72c6    	add.w	r2, r2, #0x18c
    b926: 9300         	str	r3, [sp]
    b928: f240 53a5    	movw	r3, #0x5a5
    b92c: f7f8 eb3e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x7984
    b930: f7ff b837    	b.w	0xa9a2 <gst_lg_audio_render_event+0x2762> @ imm = #-0xf92
    b934: 4aad         	ldr	r2, [pc, #0x2b4]        @ 0xbbec <gst_lg_audio_render_event+0x39ac>
    b936: 2103         	movs	r1, #0x3
    b938: 4bad         	ldr	r3, [pc, #0x2b4]        @ 0xbbf0 <gst_lg_audio_render_event+0x39b0>
    b93a: 447a         	add	r2, pc
    b93c: 447b         	add	r3, pc
    b93e: f502 72b8    	add.w	r2, r2, #0x170
    b942: 9300         	str	r3, [sp]
    b944: f640 132e    	movw	r3, #0x92e
    b948: f7f8 eb30    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x79a0
    b94c: f7fe bf4e    	b.w	0xa7ec <gst_lg_audio_render_event+0x25ac> @ imm = #-0x1164
    b950: 9c 72 02 00  	.word	0x0002729c
    b954: 08 04 00 00  	.word	0x00000408
    b958: fc 4d 01 00  	.word	0x00014dfc
    b95c: 0a 0a 01 00  	.word	0x00010a0a
    b960: 8c eb 00 00  	.word	0x0000eb8c
    b964: 46 72 02 00  	.word	0x00027246
    b968: 14 02 01 00  	.word	0x00010214
    b96c: a2 4d 01 00  	.word	0x00014da2
    b970: 40 eb 00 00  	.word	0x0000eb40
    b974: de 71 02 00  	.word	0x000271de
    b978: 4e 4d 01 00  	.word	0x00014d4e
    b97c: 32 06 01 00  	.word	0x00010632
    b980: d8 ea 00 00  	.word	0x0000ead8
    b984: 88 71 02 00  	.word	0x00027188
    b988: fe 4c 01 00  	.word	0x00014cfe
    b98c: 78 14 01 00  	.word	0x00011478
    b990: 60 71 02 00  	.word	0x00027160
    b994: da 4c 01 00  	.word	0x00014cda
    b998: b0 06 01 00  	.word	0x000106b0
    b99c: ba 4c 01 00  	.word	0x00014cba
    b9a0: 34 01 01 00  	.word	0x00010134
    b9a4: 9a 4c 01 00  	.word	0x00014c9a
    b9a8: 68 16 01 00  	.word	0x00011668
    b9ac: 7a 4c 01 00  	.word	0x00014c7a
    b9b0: b4 16 01 00  	.word	0x000116b4
    b9b4: 04 04 00 00  	.word	0x00000404
    b9b8: 88 08 01 00  	.word	0x00010888
    b9bc: 9a 70 02 00  	.word	0x0002709a
    b9c0: fc 4b 01 00  	.word	0x00014bfc
    b9c4: 62 08 01 00  	.word	0x00010862
    b9c8: 8c e9 00 00  	.word	0x0000e98c
    b9cc: d4 4b 01 00  	.word	0x00014bd4
    b9d0: f6 03 01 00  	.word	0x000103f6
    b9d4: ae 4b 01 00  	.word	0x00014bae
    b9d8: 0c 0e 01 00  	.word	0x00010e0c
    b9dc: 00 70 02 00  	.word	0x00027000
    b9e0: 6e 4b 01 00  	.word	0x00014b6e
    b9e4: 14 07 01 00  	.word	0x00010714
    b9e8: fc e8 00 00  	.word	0x0000e8fc
    b9ec: 32 4b 01 00  	.word	0x00014b32
    b9f0: b0 04 01 00  	.word	0x000104b0
    b9f4: 10 4b 01 00  	.word	0x00014b10
    b9f8: c2 04 01 00  	.word	0x000104c2
    b9fc: f0 4a 01 00  	.word	0x00014af0
    ba00: 2e 14 01 00  	.word	0x0001142e
    ba04: c6 4a 01 00  	.word	0x00014ac6
    ba08: 58 15 01 00  	.word	0x00011558
    ba0c: 9a 4a 01 00  	.word	0x00014a9a
    ba10: a0 09 01 00  	.word	0x000109a0
    ba14: 74 4a 01 00  	.word	0x00014a74
    ba18: a6 03 01 00  	.word	0x000103a6
    ba1c: ec 04 01 00  	.word	0x000104ec
    ba20: 2a 4a 01 00  	.word	0x00014a2a
    ba24: c8 e7 00 00  	.word	0x0000e7c8
    ba28: 10 4a 01 00  	.word	0x00014a10
    ba2c: b6 08 01 00  	.word	0x000108b6
    ba30: ee 49 01 00  	.word	0x000149ee
    ba34: 58 08 01 00  	.word	0x00010858
    ba38: ce 49 01 00  	.word	0x000149ce
    ba3c: f8 06 01 00  	.word	0x000106f8
    ba40: ac 49 01 00  	.word	0x000149ac
    ba44: ea 08 01 00  	.word	0x000108ea
    ba48: 8c 49 01 00  	.word	0x0001498c
    ba4c: ba fd 00 00  	.word	0x0000fdba
    ba50: 6e 49 01 00  	.word	0x0001496e
    ba54: e4 08 01 00  	.word	0x000108e4
    ba58: 4e 49 01 00  	.word	0x0001494e
    ba5c: 60 09 01 00  	.word	0x00010960
    ba60: 2e 49 01 00  	.word	0x0001492e
    ba64: 24 06 01 00  	.word	0x00010624
    ba68: 0e 49 01 00  	.word	0x0001490e
    ba6c: 18 04 01 00  	.word	0x00010418
    ba70: ee 48 01 00  	.word	0x000148ee
    ba74: a0 06 01 00  	.word	0x000106a0
    ba78: cc 48 01 00  	.word	0x000148cc
    ba7c: 4e 06 01 00  	.word	0x0001064e
    ba80: ac 48 01 00  	.word	0x000148ac
    ba84: ce 06 01 00  	.word	0x000106ce
    ba88: 8a 48 01 00  	.word	0x0001488a
    ba8c: 88 06 01 00  	.word	0x00010688
    ba90: 68 48 01 00  	.word	0x00014868
    ba94: 3a 04 01 00  	.word	0x0001043a
    ba98: 46 48 01 00  	.word	0x00014846
    ba9c: 94 0b 01 00  	.word	0x00010b94
    baa0: 24 48 01 00  	.word	0x00014824
    baa4: 2e 0b 01 00  	.word	0x00010b2e
    baa8: 02 48 01 00  	.word	0x00014802
    baac: 80 09 01 00  	.word	0x00010980
    bab0: e0 47 01 00  	.word	0x000147e0
    bab4: 8e 09 01 00  	.word	0x0001098e
    bab8: be 47 01 00  	.word	0x000147be
    babc: ac 09 01 00  	.word	0x000109ac
    bac0: 9c 47 01 00  	.word	0x0001479c
    bac4: c2 09 01 00  	.word	0x000109c2
    bac8: 7c 47 01 00  	.word	0x0001477c
    bacc: 0a 08 01 00  	.word	0x0001080a
    bad0: 5a 47 01 00  	.word	0x0001475a
    bad4: 38 07 01 00  	.word	0x00010738
    bad8: 3a 47 01 00  	.word	0x0001473a
    badc: 78 08 01 00  	.word	0x00010878
    bae0: 18 47 01 00  	.word	0x00014718
    bae4: 32 08 01 00  	.word	0x00010832
    bae8: f8 46 01 00  	.word	0x000146f8
    baec: c6 09 01 00  	.word	0x000109c6
    baf0: d6 46 01 00  	.word	0x000146d6
    baf4: 78 09 01 00  	.word	0x00010978
    baf8: b4 46 01 00  	.word	0x000146b4
    bafc: 16 07 01 00  	.word	0x00010716
    bb00: 92 46 01 00  	.word	0x00014692
    bb04: 6c 07 01 00  	.word	0x0001076c
    bb08: 4a 01 01 00  	.word	0x0001014a
    bb0c: 48 46 01 00  	.word	0x00014648
    bb10: e6 e3 00 00  	.word	0x0000e3e6
    bb14: 1e 46 01 00  	.word	0x0001461e
    bb18: 5c 0d 01 00  	.word	0x00010d5c
    bb1c: fe 45 01 00  	.word	0x000145fe
    bb20: c4 fe 00 00  	.word	0x0000fec4
    bb24: 52 6a 02 00  	.word	0x00026a52
    bb28: 86 02 01 00  	.word	0x00010286
    bb2c: b0 45 01 00  	.word	0x000145b0
    bb30: 4e e3 00 00  	.word	0x0000e34e
    bb34: 96 45 01 00  	.word	0x00014596
    bb38: 30 09 01 00  	.word	0x00010930
    bb3c: 4c 0b 01 00  	.word	0x00010b4c
    bb40: 50 45 01 00  	.word	0x00014550
    bb44: ee e2 00 00  	.word	0x0000e2ee
    bb48: 20 0b 01 00  	.word	0x00010b20
    bb4c: fa 44 01 00  	.word	0x000144fa
    bb50: 98 e2 00 00  	.word	0x0000e298
    bb54: 16 0b 01 00  	.word	0x00010b16
    bb58: b8 44 01 00  	.word	0x000144b8
    bb5c: 56 e2 00 00  	.word	0x0000e256
    bb60: fc 0a 01 00  	.word	0x00010afc
    bb64: 78 44 01 00  	.word	0x00014478
    bb68: 16 e2 00 00  	.word	0x0000e216
    bb6c: de 0a 01 00  	.word	0x00010ade
    bb70: 2c 44 01 00  	.word	0x0001442c
    bb74: ca e1 00 00  	.word	0x0000e1ca
    bb78: c2 0a 01 00  	.word	0x00010ac2
    bb7c: f0 43 01 00  	.word	0x000143f0
    bb80: 8e e1 00 00  	.word	0x0000e18e
    bb84: d2 43 01 00  	.word	0x000143d2
    bb88: bc ff 00 00  	.word	0x0000ffbc
    bb8c: b6 43 01 00  	.word	0x000143b6
    bb90: 4c ff 00 00  	.word	0x0000ff4c
    bb94: 9a 43 01 00  	.word	0x0001439a
    bb98: 28 fe 00 00  	.word	0x0000fe28
    bb9c: 7e 43 01 00  	.word	0x0001437e
    bba0: 82 fd 00 00  	.word	0x0000fd82
    bba4: 5c 43 01 00  	.word	0x0001435c
    bba8: c2 fd 00 00  	.word	0x0000fdc2
    bbac: 3e 43 01 00  	.word	0x0001433e
    bbb0: 78 fd 00 00  	.word	0x0000fd78
    bbb4: 20 43 01 00  	.word	0x00014320
    bbb8: da fd 00 00  	.word	0x0000fdda
    bbbc: 06 43 01 00  	.word	0x00014306
    bbc0: a4 08 01 00  	.word	0x000108a4
    bbc4: ea 42 01 00  	.word	0x000142ea
    bbc8: 24 08 01 00  	.word	0x00010824
    bbcc: ce 42 01 00  	.word	0x000142ce
    bbd0: 3c 07 01 00  	.word	0x0001073c
    bbd4: ae 42 01 00  	.word	0x000142ae
    bbd8: ec fe 00 00  	.word	0x0000feec
    bbdc: 92 42 01 00  	.word	0x00014292
    bbe0: 1c ff 00 00  	.word	0x0000ff1c
    bbe4: 76 42 01 00  	.word	0x00014276
    bbe8: 5c 07 01 00  	.word	0x0001075c
    bbec: 5a 42 01 00  	.word	0x0001425a
    bbf0: 7c 0a 01 00  	.word	0x00010a7c
