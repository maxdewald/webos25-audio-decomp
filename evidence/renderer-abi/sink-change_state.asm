
/Users/max/projects/dts_restore_plus/re/webos25-audio-policy/bin/usr/lib/gstreamer-1.0/libgstlgaudiosink.so:	file format elf32-littlearm

Disassembly of section .text:

0000d7c0 <gst_lg_audio_render_change_state>:
    d7c0: e92d 4ff0    	push.w	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
    d7c4: 4607         	mov	r7, r0
    d7c6: b0ab         	sub	sp, #0xac
    d7c8: 460d         	mov	r5, r1
    d7ca: f04f 0900    	mov.w	r9, #0x0
    d7ce: f7f6 ebd4    	blx	0x3f78 <gst_lg_audio_render_get_type@plt> @ imm = #-0x9858
    d7d2: 4601         	mov	r1, r0
    d7d4: 4638         	mov	r0, r7
    d7d6: f7f6 ebd8    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x9850
    d7da: f04f 33ff    	mov.w	r3, #0xffffffff
    d7de: f8df 84f0    	ldr.w	r8, [pc, #0x4f0]        @ 0xdcd0 <gst_lg_audio_render_change_state+0x510>
    d7e2: 9327         	str	r3, [sp, #0x9c]
    d7e4: f005 0207    	and	r2, r5, #0x7
    d7e8: f8d0 3200    	ldr.w	r3, [r0, #0x200]
    d7ec: 4604         	mov	r4, r0
    d7ee: 44f8         	add	r8, pc
    d7f0: f8cd 9098    	str.w	r9, [sp, #0x98]
    d7f4: 9214         	str	r2, [sp, #0x50]
    d7f6: 9315         	str	r3, [sp, #0x54]
    d7f8: 2b00         	cmp	r3, #0x0
    d7fa: f000 8178    	beq.w	0xdaee <gst_lg_audio_render_change_state+0x32e> @ imm = #0x2f0
    d7fe: 4618         	mov	r0, r3
    d800: a926         	add	r1, sp, #0x98
    d802: f7f6 eba8    	blx	0x3f54 <lgao_get_mode_mixer@plt> @ imm = #-0x98b0
    d806: f8df 34cc    	ldr.w	r3, [pc, #0x4cc]        @ 0xdcd4 <gst_lg_audio_render_change_state+0x514>
    d80a: f858 6003    	ldr.w	r6, [r8, r3]
    d80e: b1d8         	cbz	r0, 0xd848 <gst_lg_audio_render_change_state+0x88> @ imm = #0x36
    d810: 6833         	ldr	r3, [r6]
    d812: 2b01         	cmp	r3, #0x1
    d814: d918         	bls	0xd848 <gst_lg_audio_render_change_state+0x88> @ imm = #0x30
    d816: f8df 34c0    	ldr.w	r3, [pc, #0x4c0]        @ 0xdcd8 <gst_lg_audio_render_change_state+0x518>
    d81a: 2102         	movs	r1, #0x2
    d81c: f8df 24bc    	ldr.w	r2, [pc, #0x4bc]        @ 0xdcdc <gst_lg_audio_render_change_state+0x51c>
    d820: f858 3003    	ldr.w	r3, [r8, r3]
    d824: 447a         	add	r2, pc
    d826: f8cd 9004    	str.w	r9, [sp, #0x4]
    d82a: 6818         	ldr	r0, [r3]
    d82c: f8df 34b0    	ldr.w	r3, [pc, #0x4b0]        @ 0xdce0 <gst_lg_audio_render_change_state+0x520>
    d830: 9202         	str	r2, [sp, #0x8]
    d832: f640 3295    	movw	r2, #0xb95
    d836: 9200         	str	r2, [sp]
    d838: f8df 24a8    	ldr.w	r2, [pc, #0x4a8]        @ 0xdce4 <gst_lg_audio_render_change_state+0x524>
    d83c: 447b         	add	r3, pc
    d83e: f503 7312    	add.w	r3, r3, #0x248
    d842: 447a         	add	r2, pc
    d844: f7f6 ea12    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x9bdc
    d848: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    d84c: a927         	add	r1, sp, #0x9c
    d84e: f7f6 ed4a    	blx	0x42e4 <lgao_get_port@plt> @ imm = #-0x956c
    d852: b1e0         	cbz	r0, 0xd88e <gst_lg_audio_render_change_state+0xce> @ imm = #0x38
    d854: 6833         	ldr	r3, [r6]
    d856: 2b01         	cmp	r3, #0x1
    d858: d919         	bls	0xd88e <gst_lg_audio_render_change_state+0xce> @ imm = #0x32
    d85a: f8df 347c    	ldr.w	r3, [pc, #0x47c]        @ 0xdcd8 <gst_lg_audio_render_change_state+0x518>
    d85e: f04f 0e00    	mov.w	lr, #0x0
    d862: f8df 2484    	ldr.w	r2, [pc, #0x484]        @ 0xdce8 <gst_lg_audio_render_change_state+0x528>
    d866: 2102         	movs	r1, #0x2
    d868: f858 3003    	ldr.w	r3, [r8, r3]
    d86c: 447a         	add	r2, pc
    d86e: 6818         	ldr	r0, [r3]
    d870: f8df 3478    	ldr.w	r3, [pc, #0x478]        @ 0xdcec <gst_lg_audio_render_change_state+0x52c>
    d874: 9202         	str	r2, [sp, #0x8]
    d876: f640 3296    	movw	r2, #0xb96
    d87a: e9cd 2e00    	strd	r2, lr, [sp]
    d87e: f8df 2470    	ldr.w	r2, [pc, #0x470]        @ 0xdcf0 <gst_lg_audio_render_change_state+0x530>
    d882: 447b         	add	r3, pc
    d884: f503 7312    	add.w	r3, r3, #0x248
    d888: 447a         	add	r2, pc
    d88a: f7f6 e9f0    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x9c20
    d88e: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    d892: a928         	add	r1, sp, #0xa0
    d894: f7f5 ef86    	blx	0x37a4 <lgao_get_state@plt> @ imm = #-0xa0f4
    d898: b1e0         	cbz	r0, 0xd8d4 <gst_lg_audio_render_change_state+0x114> @ imm = #0x38
    d89a: 6833         	ldr	r3, [r6]
    d89c: 2b01         	cmp	r3, #0x1
    d89e: d919         	bls	0xd8d4 <gst_lg_audio_render_change_state+0x114> @ imm = #0x32
    d8a0: f8df 3434    	ldr.w	r3, [pc, #0x434]        @ 0xdcd8 <gst_lg_audio_render_change_state+0x518>
    d8a4: f04f 0c00    	mov.w	r12, #0x0
    d8a8: f8df 2448    	ldr.w	r2, [pc, #0x448]        @ 0xdcf4 <gst_lg_audio_render_change_state+0x534>
    d8ac: 2102         	movs	r1, #0x2
    d8ae: f858 3003    	ldr.w	r3, [r8, r3]
    d8b2: 447a         	add	r2, pc
    d8b4: 6818         	ldr	r0, [r3]
    d8b6: f8df 3440    	ldr.w	r3, [pc, #0x440]        @ 0xdcf8 <gst_lg_audio_render_change_state+0x538>
    d8ba: 9202         	str	r2, [sp, #0x8]
    d8bc: f640 3297    	movw	r2, #0xb97
    d8c0: e9cd 2c00    	strd	r2, r12, [sp]
    d8c4: f8df 2434    	ldr.w	r2, [pc, #0x434]        @ 0xdcfc <gst_lg_audio_render_change_state+0x53c>
    d8c8: 447b         	add	r3, pc
    d8ca: f503 7312    	add.w	r3, r3, #0x248
    d8ce: 447a         	add	r2, pc
    d8d0: f7f6 e9cc    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x9c68
    d8d4: f8df 3428    	ldr.w	r3, [pc, #0x428]        @ 0xdd00 <gst_lg_audio_render_change_state+0x540>
    d8d8: 447b         	add	r3, pc
    d8da: 68db         	ldr	r3, [r3, #0xc]
    d8dc: 681b         	ldr	r3, [r3]
    d8de: 0759         	lsls	r1, r3, #0x1d
    d8e0: d523         	bpl	0xd92a <gst_lg_audio_render_change_state+0x16a> @ imm = #0x46
    d8e2: 9b26         	ldr	r3, [sp, #0x98]
    d8e4: 2b01         	cmp	r3, #0x1
    d8e6: f000 86d6    	beq.w	0xe696 <gst_lg_audio_render_change_state+0xed6> @ imm = #0xdac
    d8ea: f8df a418    	ldr.w	r10, [pc, #0x418]       @ 0xdd04 <gst_lg_audio_render_change_state+0x544>
    d8ee: 44fa         	add	r10, pc
    d8f0: 08e8         	lsrs	r0, r5, #0x3
    d8f2: f8dd b09c    	ldr.w	r11, [sp, #0x9c]
    d8f6: f7f6 e8e8    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0x9e30
    d8fa: 4681         	mov	r9, r0
    d8fc: 9814         	ldr	r0, [sp, #0x50]
    d8fe: f7f6 e8e4    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0x9e38
    d902: f8df 2404    	ldr.w	r2, [pc, #0x404]        @ 0xdd08 <gst_lg_audio_render_change_state+0x548>
    d906: 2102         	movs	r1, #0x2
    d908: f8df 3400    	ldr.w	r3, [pc, #0x400]        @ 0xdd0c <gst_lg_audio_render_change_state+0x54c>
    d90c: e9cd 9003    	strd	r9, r0, [sp, #12]
    d910: 48ff         	ldr	r0, [pc, #0x3fc]        @ 0xdd10 <gst_lg_audio_render_change_state+0x550>
    d912: 447a         	add	r2, pc
    d914: 447b         	add	r3, pc
    d916: f502 7212    	add.w	r2, r2, #0x248
    d91a: 9300         	str	r3, [sp]
    d91c: f640 3399    	movw	r3, #0xb99
    d920: 4478         	add	r0, pc
    d922: e9cd ab01    	strd	r10, r11, [sp, #4]
    d926: f7f6 eb42    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x997c
    d92a: 6833         	ldr	r3, [r6]
    d92c: 2b03         	cmp	r3, #0x3
    d92e: f200 80b2    	bhi.w	0xda96 <gst_lg_audio_render_change_state+0x2d6> @ imm = #0x164
    d932: 9b14         	ldr	r3, [sp, #0x50]
    d934: 2d13         	cmp	r5, #0x13
    d936: f8c4 321c    	str.w	r3, [r4, #0x21c]
    d93a: f000 810b    	beq.w	0xdb54 <gst_lg_audio_render_change_state+0x394> @ imm = #0x216
    d93e: 2d1c         	cmp	r5, #0x1c
    d940: f000 822c    	beq.w	0xdd9c <gst_lg_audio_render_change_state+0x5dc> @ imm = #0x458
    d944: 2d0a         	cmp	r5, #0xa
    d946: f000 814a    	beq.w	0xdbde <gst_lg_audio_render_change_state+0x41e> @ imm = #0x294
    d94a: 2d24         	cmp	r5, #0x24
    d94c: f000 86a8    	beq.w	0xe6a0 <gst_lg_audio_render_change_state+0xee0> @ imm = #0xd50
    d950: 4bf0         	ldr	r3, [pc, #0x3c0]        @ 0xdd14 <gst_lg_audio_render_change_state+0x554>
    d952: 447b         	add	r3, pc
    d954: f8d3 9000    	ldr.w	r9, [r3]
    d958: f7f6 eb1c    	blx	0x3f94 <gst_element_get_type@plt> @ imm = #-0x99c8
    d95c: 4601         	mov	r1, r0
    d95e: 4648         	mov	r0, r9
    d960: f7f6 e946    	blx	0x3bf0 <g_type_check_class_cast@plt> @ imm = #-0x9d74
    d964: 4629         	mov	r1, r5
    d966: f8d0 308c    	ldr.w	r3, [r0, #0x8c]
    d96a: 4638         	mov	r0, r7
    d96c: 4798         	blx	r3
    d96e: 2d1a         	cmp	r5, #0x1a
    d970: 4607         	mov	r7, r0
    d972: f000 812c    	beq.w	0xdbce <gst_lg_audio_render_change_state+0x40e> @ imm = #0x258
    d976: 2d23         	cmp	r5, #0x23
    d978: f040 8085    	bne.w	0xda86 <gst_lg_audio_render_change_state+0x2c6> @ imm = #0x10a
    d97c: 48e6         	ldr	r0, [pc, #0x398]        @ 0xdd18 <gst_lg_audio_render_change_state+0x558>
    d97e: 2303         	movs	r3, #0x3
    d980: f8c4 3218    	str.w	r3, [r4, #0x218]
    d984: 4478         	add	r0, pc
    d986: 68c3         	ldr	r3, [r0, #0xc]
    d988: 681b         	ldr	r3, [r3]
    d98a: 075a         	lsls	r2, r3, #0x1d
    d98c: f100 8721    	bmi.w	0xe7d2 <gst_lg_audio_render_change_state+0x1012> @ imm = #0xe42
    d990: 6833         	ldr	r3, [r6]
    d992: 2b03         	cmp	r3, #0x3
    d994: d913         	bls	0xd9be <gst_lg_audio_render_change_state+0x1fe> @ imm = #0x26
    d996: 4bd0         	ldr	r3, [pc, #0x340]        @ 0xdcd8 <gst_lg_audio_render_change_state+0x518>
    d998: 2104         	movs	r1, #0x4
    d99a: 4ae0         	ldr	r2, [pc, #0x380]        @ 0xdd1c <gst_lg_audio_render_change_state+0x55c>
    d99c: f858 3003    	ldr.w	r3, [r8, r3]
    d9a0: 447a         	add	r2, pc
    d9a2: 9401         	str	r4, [sp, #0x4]
    d9a4: 6818         	ldr	r0, [r3]
    d9a6: 4bde         	ldr	r3, [pc, #0x378]        @ 0xdd20 <gst_lg_audio_render_change_state+0x560>
    d9a8: 9202         	str	r2, [sp, #0x8]
    d9aa: f640 4224    	movw	r2, #0xc24
    d9ae: 9200         	str	r2, [sp]
    d9b0: 4adc         	ldr	r2, [pc, #0x370]        @ 0xdd24 <gst_lg_audio_render_change_state+0x564>
    d9b2: 447b         	add	r3, pc
    d9b4: f503 7312    	add.w	r3, r3, #0x248
    d9b8: 447a         	add	r2, pc
    d9ba: f7f6 e958    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x9d50
    d9be: 9b28         	ldr	r3, [sp, #0xa0]
    d9c0: 2b02         	cmp	r3, #0x2
    d9c2: f001 8367    	beq.w	0xf094 <gst_lg_audio_render_change_state+0x18d4> @ imm = #0x16ce
    d9c6: f8df 9360    	ldr.w	r9, [pc, #0x360]        @ 0xdd28 <gst_lg_audio_render_change_state+0x568>
    d9ca: 9a15         	ldr	r2, [sp, #0x54]
    d9cc: 44f9         	add	r9, pc
    d9ce: f8d9 300c    	ldr.w	r3, [r9, #0xc]
    d9d2: 681b         	ldr	r3, [r3]
    d9d4: f003 0304    	and	r3, r3, #0x4
    d9d8: 2a00         	cmp	r2, #0x0
    d9da: f000 80cf    	beq.w	0xdb7c <gst_lg_audio_render_change_state+0x3bc> @ imm = #0x19e
    d9de: b30b         	cbz	r3, 0xda24 <gst_lg_audio_render_change_state+0x264> @ imm = #0x42
    d9e0: 9b26         	ldr	r3, [sp, #0x98]
    d9e2: 2b01         	cmp	r3, #0x1
    d9e4: f001 83cf    	beq.w	0xf186 <gst_lg_audio_render_change_state+0x19c6> @ imm = #0x179e
    d9e8: f8df a340    	ldr.w	r10, [pc, #0x340]       @ 0xdd2c <gst_lg_audio_render_change_state+0x56c>
    d9ec: 44fa         	add	r10, pc
    d9ee: 08e8         	lsrs	r0, r5, #0x3
    d9f0: f8dd b09c    	ldr.w	r11, [sp, #0x9c]
    d9f4: f7f6 e868    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0x9f30
    d9f8: 4681         	mov	r9, r0
    d9fa: 9814         	ldr	r0, [sp, #0x50]
    d9fc: f7f6 e864    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0x9f38
    da00: 4acb         	ldr	r2, [pc, #0x32c]        @ 0xdd30 <gst_lg_audio_render_change_state+0x570>
    da02: 2102         	movs	r1, #0x2
    da04: 4bcb         	ldr	r3, [pc, #0x32c]        @ 0xdd34 <gst_lg_audio_render_change_state+0x574>
    da06: e9cd 9003    	strd	r9, r0, [sp, #12]
    da0a: 48cb         	ldr	r0, [pc, #0x32c]        @ 0xdd38 <gst_lg_audio_render_change_state+0x578>
    da0c: 447a         	add	r2, pc
    da0e: 447b         	add	r3, pc
    da10: f502 7212    	add.w	r2, r2, #0x248
    da14: 9300         	str	r3, [sp]
    da16: f640 433f    	movw	r3, #0xc3f
    da1a: 4478         	add	r0, pc
    da1c: e9cd ab01    	strd	r10, r11, [sp, #4]
    da20: f7f6 eac4    	blx	0x3fac <logmm_printl@plt> @ imm = #-0x9a78
    da24: 6833         	ldr	r3, [r6]
    da26: 2b03         	cmp	r3, #0x3
    da28: d929         	bls	0xda7e <gst_lg_audio_render_change_state+0x2be> @ imm = #0x52
    da2a: 9b26         	ldr	r3, [sp, #0x98]
    da2c: 2b01         	cmp	r3, #0x1
    da2e: f001 83a5    	beq.w	0xf17c <gst_lg_audio_render_change_state+0x19bc> @ imm = #0x174a
    da32: 4ec2         	ldr	r6, [pc, #0x308]        @ 0xdd3c <gst_lg_audio_render_change_state+0x57c>
    da34: 447e         	add	r6, pc
    da36: 4ba8         	ldr	r3, [pc, #0x2a0]        @ 0xdcd8 <gst_lg_audio_render_change_state+0x518>
    da38: 08e8         	lsrs	r0, r5, #0x3
    da3a: f858 3003    	ldr.w	r3, [r8, r3]
    da3e: f8dd 809c    	ldr.w	r8, [sp, #0x9c]
    da42: f8d3 9000    	ldr.w	r9, [r3]
    da46: f7f6 e840    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0x9f80
    da4a: 4605         	mov	r5, r0
    da4c: 9814         	ldr	r0, [sp, #0x50]
    da4e: f7f6 e83c    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0x9f88
    da52: 4bbb         	ldr	r3, [pc, #0x2ec]        @ 0xdd40 <gst_lg_audio_render_change_state+0x580>
    da54: f640 413f    	movw	r1, #0xc3f
    da58: 4aba         	ldr	r2, [pc, #0x2e8]        @ 0xdd44 <gst_lg_audio_render_change_state+0x584>
    da5a: e9cd 5005    	strd	r5, r0, [sp, #20]
    da5e: 4648         	mov	r0, r9
    da60: 447b         	add	r3, pc
    da62: 9100         	str	r1, [sp]
    da64: 9302         	str	r3, [sp, #0x8]
    da66: 447a         	add	r2, pc
    da68: 4bb7         	ldr	r3, [pc, #0x2dc]        @ 0xdd48 <gst_lg_audio_render_change_state+0x588>
    da6a: 2104         	movs	r1, #0x4
    da6c: f8cd 8010    	str.w	r8, [sp, #0x10]
    da70: 9603         	str	r6, [sp, #0xc]
    da72: 447b         	add	r3, pc
    da74: 9401         	str	r4, [sp, #0x4]
    da76: f503 7312    	add.w	r3, r3, #0x248
    da7a: f7f6 e8f8    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x9e10
    da7e: 4638         	mov	r0, r7
    da80: b02b         	add	sp, #0xac
    da82: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    da86: 2d11         	cmp	r5, #0x11
    da88: d19d         	bne	0xd9c6 <gst_lg_audio_render_change_state+0x206> @ imm = #-0xc6
    da8a: 2301         	movs	r3, #0x1
    da8c: f8c4 3218    	str.w	r3, [r4, #0x218]
    da90: f7f6 eb12    	blx	0x40b8 <gst_lxlog_deinit@plt> @ imm = #-0x99dc
    da94: e797         	b	0xd9c6 <gst_lg_audio_render_change_state+0x206> @ imm = #-0xd2
    da96: 9b26         	ldr	r3, [sp, #0x98]
    da98: 2b01         	cmp	r3, #0x1
    da9a: f000 85f7    	beq.w	0xe68c <gst_lg_audio_render_change_state+0xecc> @ imm = #0xbee
    da9e: f8df 92ac    	ldr.w	r9, [pc, #0x2ac]        @ 0xdd4c <gst_lg_audio_render_change_state+0x58c>
    daa2: 44f9         	add	r9, pc
    daa4: 4b8c         	ldr	r3, [pc, #0x230]        @ 0xdcd8 <gst_lg_audio_render_change_state+0x518>
    daa6: 08e8         	lsrs	r0, r5, #0x3
    daa8: f858 3003    	ldr.w	r3, [r8, r3]
    daac: f8d3 b000    	ldr.w	r11, [r3]
    dab0: 9b27         	ldr	r3, [sp, #0x9c]
    dab2: 9316         	str	r3, [sp, #0x58]
    dab4: f7f6 e808    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0x9ff0
    dab8: 4682         	mov	r10, r0
    daba: 9814         	ldr	r0, [sp, #0x50]
    dabc: f7f6 e804    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0x9ff8
    dac0: 4aa3         	ldr	r2, [pc, #0x28c]        @ 0xdd50 <gst_lg_audio_render_change_state+0x590>
    dac2: 2104         	movs	r1, #0x4
    dac4: 9b16         	ldr	r3, [sp, #0x58]
    dac6: e9cd a005    	strd	r10, r0, [sp, #20]
    daca: 4658         	mov	r0, r11
    dacc: 447a         	add	r2, pc
    dace: 9401         	str	r4, [sp, #0x4]
    dad0: e9cd 9303    	strd	r9, r3, [sp, #12]
    dad4: 9202         	str	r2, [sp, #0x8]
    dad6: f640 3299    	movw	r2, #0xb99
    dada: 4b9e         	ldr	r3, [pc, #0x278]        @ 0xdd54 <gst_lg_audio_render_change_state+0x594>
    dadc: 9200         	str	r2, [sp]
    dade: 4a9e         	ldr	r2, [pc, #0x278]        @ 0xdd58 <gst_lg_audio_render_change_state+0x598>
    dae0: 447b         	add	r3, pc
    dae2: f503 7312    	add.w	r3, r3, #0x248
    dae6: 447a         	add	r2, pc
    dae8: f7f6 e8c0    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x9e80
    daec: e721         	b	0xd932 <gst_lg_audio_render_change_state+0x172> @ imm = #-0x1be
    daee: 4e9b         	ldr	r6, [pc, #0x26c]        @ 0xdd5c <gst_lg_audio_render_change_state+0x59c>
    daf0: 447e         	add	r6, pc
    daf2: 68f3         	ldr	r3, [r6, #0xc]
    daf4: 681b         	ldr	r3, [r3]
    daf6: 075a         	lsls	r2, r3, #0x1d
    daf8: f101 8326    	bmi.w	0xf148 <gst_lg_audio_render_change_state+0x1988> @ imm = #0x164c
    dafc: 4b75         	ldr	r3, [pc, #0x1d4]        @ 0xdcd4 <gst_lg_audio_render_change_state+0x514>
    dafe: f858 6003    	ldr.w	r6, [r8, r3]
    db02: 6833         	ldr	r3, [r6]
    db04: 2b03         	cmp	r3, #0x3
    db06: f67f af14    	bls.w	0xd932 <gst_lg_audio_render_change_state+0x172> @ imm = #-0x1d8
    db0a: 4b73         	ldr	r3, [pc, #0x1cc]        @ 0xdcd8 <gst_lg_audio_render_change_state+0x518>
    db0c: 08e8         	lsrs	r0, r5, #0x3
    db0e: f858 3003    	ldr.w	r3, [r8, r3]
    db12: f8d3 a000    	ldr.w	r10, [r3]
    db16: f7f5 efd8    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0xa050
    db1a: 4681         	mov	r9, r0
    db1c: 9814         	ldr	r0, [sp, #0x50]
    db1e: f7f5 efd4    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0xa058
    db22: 4a8f         	ldr	r2, [pc, #0x23c]        @ 0xdd60 <gst_lg_audio_render_change_state+0x5a0>
    db24: 2104         	movs	r1, #0x4
    db26: 4b8f         	ldr	r3, [pc, #0x23c]        @ 0xdd64 <gst_lg_audio_render_change_state+0x5a4>
    db28: e9cd 9003    	strd	r9, r0, [sp, #12]
    db2c: 4650         	mov	r0, r10
    db2e: 447a         	add	r2, pc
    db30: 9401         	str	r4, [sp, #0x4]
    db32: 9202         	str	r2, [sp, #0x8]
    db34: f640 329f    	movw	r2, #0xb9f
    db38: 9200         	str	r2, [sp]
    db3a: 447b         	add	r3, pc
    db3c: 4a8a         	ldr	r2, [pc, #0x228]        @ 0xdd68 <gst_lg_audio_render_change_state+0x5a8>
    db3e: f503 7312    	add.w	r3, r3, #0x248
    db42: 447a         	add	r2, pc
    db44: f7f6 e892    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x9edc
    db48: 9b14         	ldr	r3, [sp, #0x50]
    db4a: 2d13         	cmp	r5, #0x13
    db4c: f8c4 321c    	str.w	r3, [r4, #0x21c]
    db50: f47f aef5    	bne.w	0xd93e <gst_lg_audio_render_change_state+0x17e> @ imm = #-0x216
    db54: 2303         	movs	r3, #0x3
    db56: f8c4 3218    	str.w	r3, [r4, #0x218]
    db5a: 4b84         	ldr	r3, [pc, #0x210]        @ 0xdd6c <gst_lg_audio_render_change_state+0x5ac>
    db5c: 447b         	add	r3, pc
    db5e: f8d3 9000    	ldr.w	r9, [r3]
    db62: f7f6 ea18    	blx	0x3f94 <gst_element_get_type@plt> @ imm = #-0x9bd0
    db66: 4601         	mov	r1, r0
    db68: 4648         	mov	r0, r9
    db6a: f7f6 e842    	blx	0x3bf0 <g_type_check_class_cast@plt> @ imm = #-0x9f7c
    db6e: 4629         	mov	r1, r5
    db70: f8d0 308c    	ldr.w	r3, [r0, #0x8c]
    db74: 4638         	mov	r0, r7
    db76: 4798         	blx	r3
    db78: 4607         	mov	r7, r0
    db7a: e724         	b	0xd9c6 <gst_lg_audio_render_change_state+0x206> @ imm = #-0x1b8
    db7c: 2b00         	cmp	r3, #0x0
    db7e: f041 8307    	bne.w	0xf190 <gst_lg_audio_render_change_state+0x19d0> @ imm = #0x160e
    db82: 6833         	ldr	r3, [r6]
    db84: 2b03         	cmp	r3, #0x3
    db86: f67f af7a    	bls.w	0xda7e <gst_lg_audio_render_change_state+0x2be> @ imm = #-0x10c
    db8a: 4b53         	ldr	r3, [pc, #0x14c]        @ 0xdcd8 <gst_lg_audio_render_change_state+0x518>
    db8c: 08e8         	lsrs	r0, r5, #0x3
    db8e: f858 3003    	ldr.w	r3, [r8, r3]
    db92: 681e         	ldr	r6, [r3]
    db94: f7f5 ef98    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0xa0d0
    db98: 4605         	mov	r5, r0
    db9a: 9814         	ldr	r0, [sp, #0x50]
    db9c: f7f5 ef94    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0xa0d8
    dba0: 4b73         	ldr	r3, [pc, #0x1cc]        @ 0xdd70 <gst_lg_audio_render_change_state+0x5b0>
    dba2: f640 4245    	movw	r2, #0xc45
    dba6: e9cd 2400    	strd	r2, r4, [sp]
    dbaa: 2104         	movs	r1, #0x4
    dbac: 4a71         	ldr	r2, [pc, #0x1c4]        @ 0xdd74 <gst_lg_audio_render_change_state+0x5b4>
    dbae: 447b         	add	r3, pc
    dbb0: e9cd 5003    	strd	r5, r0, [sp, #12]
    dbb4: 9302         	str	r3, [sp, #0x8]
    dbb6: 4630         	mov	r0, r6
    dbb8: 4b6f         	ldr	r3, [pc, #0x1bc]        @ 0xdd78 <gst_lg_audio_render_change_state+0x5b8>
    dbba: 447a         	add	r2, pc
    dbbc: 447b         	add	r3, pc
    dbbe: f503 7312    	add.w	r3, r3, #0x248
    dbc2: f7f6 e854    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x9f58
    dbc6: 4638         	mov	r0, r7
    dbc8: b02b         	add	sp, #0xac
    dbca: e8bd 8ff0    	pop.w	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
    dbce: 2302         	movs	r3, #0x2
    dbd0: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    dbd4: f8c4 3218    	str.w	r3, [r4, #0x218]
    dbd8: f7f5 ef4c    	blx	0x3a74 <lgao_flush@plt> @ imm = #-0xa168
    dbdc: e6f3         	b	0xd9c6 <gst_lg_audio_render_change_state+0x206> @ imm = #-0x21a
    dbde: f04f 0902    	mov.w	r9, #0x2
    dbe2: f8c4 9218    	str.w	r9, [r4, #0x218]
    dbe6: f7f5 ee26    	blx	0x3834 <lgao_open@plt>  @ imm = #-0xa3b4
    dbea: e9d4 32a8    	ldrd	r3, r2, [r4, #672]
    dbee: f8c4 0200    	str.w	r0, [r4, #0x200]
    dbf2: 4313         	orrs	r3, r2
    dbf4: d0b1         	beq	0xdb5a <gst_lg_audio_render_change_state+0x39a> @ imm = #-0x9e
    dbf6: 4861         	ldr	r0, [pc, #0x184]        @ 0xdd7c <gst_lg_audio_render_change_state+0x5bc>
    dbf8: 4478         	add	r0, pc
    dbfa: 68c3         	ldr	r3, [r0, #0xc]
    dbfc: 681b         	ldr	r3, [r3]
    dbfe: 075b         	lsls	r3, r3, #0x1d
    dc00: f101 8672    	bmi.w	0xf8e8 <gst_lg_audio_render_change_state+0x2128> @ imm = #0x1ce4
    dc04: 6833         	ldr	r3, [r6]
    dc06: 2b03         	cmp	r3, #0x3
    dc08: d913         	bls	0xdc32 <gst_lg_audio_render_change_state+0x472> @ imm = #0x26
    dc0a: 4b33         	ldr	r3, [pc, #0xcc]         @ 0xdcd8 <gst_lg_audio_render_change_state+0x518>
    dc0c: 2104         	movs	r1, #0x4
    dc0e: 4a5c         	ldr	r2, [pc, #0x170]        @ 0xdd80 <gst_lg_audio_render_change_state+0x5c0>
    dc10: f858 3003    	ldr.w	r3, [r8, r3]
    dc14: 447a         	add	r2, pc
    dc16: 9401         	str	r4, [sp, #0x4]
    dc18: 6818         	ldr	r0, [r3]
    dc1a: 4b5a         	ldr	r3, [pc, #0x168]        @ 0xdd84 <gst_lg_audio_render_change_state+0x5c4>
    dc1c: 9202         	str	r2, [sp, #0x8]
    dc1e: f640 32ab    	movw	r2, #0xbab
    dc22: 9200         	str	r2, [sp]
    dc24: 4a58         	ldr	r2, [pc, #0x160]        @ 0xdd88 <gst_lg_audio_render_change_state+0x5c8>
    dc26: 447b         	add	r3, pc
    dc28: f503 7312    	add.w	r3, r3, #0x248
    dc2c: 447a         	add	r2, pc
    dc2e: f7f6 e81e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0x9fc4
    dc32: 4b56         	ldr	r3, [pc, #0x158]        @ 0xdd8c <gst_lg_audio_render_change_state+0x5cc>
    dc34: f04f 0902    	mov.w	r9, #0x2
    dc38: f8df b154    	ldr.w	r11, [pc, #0x154]       @ 0xdd90 <gst_lg_audio_render_change_state+0x5d0>
    dc3c: 447b         	add	r3, pc
    dc3e: f503 731b    	add.w	r3, r3, #0x26c
    dc42: 44fb         	add	r11, pc
    dc44: 9316         	str	r3, [sp, #0x58]
    dc46: 4b53         	ldr	r3, [pc, #0x14c]        @ 0xdd94 <gst_lg_audio_render_change_state+0x5d4>
    dc48: 447b         	add	r3, pc
    dc4a: f503 731b    	add.w	r3, r3, #0x26c
    dc4e: 9318         	str	r3, [sp, #0x60]
    dc50: 4b51         	ldr	r3, [pc, #0x144]        @ 0xdd98 <gst_lg_audio_render_change_state+0x5d8>
    dc52: 447b         	add	r3, pc
    dc54: 9317         	str	r3, [sp, #0x5c]
    dc56: e9d4 31a8    	ldrd	r3, r1, [r4, #672]
    dc5a: f1a9 0202    	sub.w	r2, r9, #0x2
    dc5e: f109 3aff    	add.w	r10, r9, #0xffffffff
    dc62: 40d3         	lsrs	r3, r2
    dc64: f1c9 0222    	rsb.w	r2, r9, #0x22
    dc68: fa01 f202    	lsl.w	r2, r1, r2
    dc6c: 4313         	orrs	r3, r2
    dc6e: f1a9 0222    	sub.w	r2, r9, #0x22
    dc72: fa21 f202    	lsr.w	r2, r1, r2
    dc76: 4313         	orrs	r3, r2
    dc78: 07d9         	lsls	r1, r3, #0x1f
    dc7a: f140 84ff    	bpl.w	0xe67c <gst_lg_audio_render_change_state+0xebc> @ imm = #0x9fe
    dc7e: f10a 33ff    	add.w	r3, r10, #0xffffffff
    dc82: f8db 200c    	ldr.w	r2, [r11, #0xc]
    dc86: 2b1f         	cmp	r3, #0x1f
    dc88: f200 84f0    	bhi.w	0xe66c <gst_lg_audio_render_change_state+0xeac> @ imm = #0x9e0
    dc8c: e8df f013    	tbh	[pc, r3, lsl #1]
    dc90: ad 09 ee 04  	.word	0x04ee09ad
    dc94: ee 04 ee 04  	.word	0x04ee04ee
    dc98: e5 07 ee 04  	.word	0x04ee07e5
    dc9c: 83 08 25 07  	.word	0x07250883
    dca0: 7f 09 6f 07  	.word	0x076f097f
    dca4: 33 08 d4 06  	.word	0x06d40833
    dca8: d7 09 95 07  	.word	0x079509d7
    dcac: 59 08 fd 06  	.word	0x06fd0859
    dcb0: 58 09 ee 04  	.word	0x04ee0958
    dcb4: ee 04 4a 07  	.word	0x074a04ee
    dcb8: ee 04 ee 04  	.word	0x04ee04ee
    dcbc: 0f 08 ee 04  	.word	0x04ee080f
    dcc0: ee 04 ee 04  	.word	0x04ee04ee
    dcc4: ae 06 88 06  	.word	0x068806ae
    dcc8: 35 06 5e 06  	.word	0x065e0635
    dccc: 0c 06 c1 05  	.word	0x05c1060c
    dcd0: ea 43 02 00  	.word	0x000243ea
    dcd4: 1c 04 00 00  	.word	0x0000041c
    dcd8: 08 04 00 00  	.word	0x00000408
    dcdc: e4 d9 00 00  	.word	0x0000d9e4
    dce0: 58 23 01 00  	.word	0x00012358
    dce4: f6 c0 00 00  	.word	0x0000c0f6
    dce8: d8 d9 00 00  	.word	0x0000d9d8
    dcec: 12 23 01 00  	.word	0x00012312
    dcf0: b0 c0 00 00  	.word	0x0000c0b0
    dcf4: 32 d1 00 00  	.word	0x0000d132
    dcf8: cc 22 01 00  	.word	0x000122cc
    dcfc: 6a c0 00 00  	.word	0x0000c06a
    dd00: 28 47 02 00  	.word	0x00024728
    dd04: 76 d4 00 00  	.word	0x0000d476
    dd08: 82 22 01 00  	.word	0x00012282
    dd0c: 80 f5 00 00  	.word	0x0000f580
    dd10: e0 46 02 00  	.word	0x000246e0
    dd14: 66 47 02 00  	.word	0x00024766
    dd18: 7c 46 02 00  	.word	0x0002467c
    dd1c: f8 02 01 00  	.word	0x000102f8
    dd20: e2 21 01 00  	.word	0x000121e2
    dd24: 80 bf 00 00  	.word	0x0000bf80
    dd28: 34 46 02 00  	.word	0x00024634
    dd2c: 78 d3 00 00  	.word	0x0000d378
    dd30: 88 21 01 00  	.word	0x00012188
    dd34: c6 02 01 00  	.word	0x000102c6
    dd38: e6 45 02 00  	.word	0x000245e6
    dd3c: 30 d3 00 00  	.word	0x0000d330
    dd40: ac 02 01 00  	.word	0x000102ac
    dd44: d2 be 00 00  	.word	0x0000bed2
    dd48: 22 21 01 00  	.word	0x00012122
    dd4c: c2 d2 00 00  	.word	0x0000d2c2
    dd50: f8 f3 00 00  	.word	0x0000f3f8
    dd54: b4 20 01 00  	.word	0x000120b4
    dd58: 52 be 00 00  	.word	0x0000be52
    dd5c: 10 45 02 00  	.word	0x00024510
    dd60: 9e f3 00 00  	.word	0x0000f39e
    dd64: 5a 20 01 00  	.word	0x0001205a
    dd68: f6 bd 00 00  	.word	0x0000bdf6
    dd6c: 5c 45 02 00  	.word	0x0002455c
    dd70: 66 01 01 00  	.word	0x00010166
    dd74: 7e bd 00 00  	.word	0x0000bd7e
    dd78: d8 1f 01 00  	.word	0x00011fd8
    dd7c: 08 44 02 00  	.word	0x00024408
    dd80: 2c f3 00 00  	.word	0x0000f32c
    dd84: 6e 1f 01 00  	.word	0x00011f6e
    dd88: 0c bd 00 00  	.word	0x0000bd0c
    dd8c: 58 1f 01 00  	.word	0x00011f58
    dd90: be 43 02 00  	.word	0x000243be
    dd94: 4c 1f 01 00  	.word	0x00011f4c
    dd98: e6 bc 00 00  	.word	0x0000bce6
    dd9c: f7f6 e980    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0x9d00
    dda0: 4601         	mov	r1, r0
    dda2: 4638         	mov	r0, r7
    dda4: f7f6 e8f0    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x9e20
    dda8: eddf 1bd5    	vldr	d17, [pc, #852]         @ 0xe100 <gst_lg_audio_render_change_state+0x940>
    ddac: edd0 2b40    	vldr	d18, [r0, #256]
    ddb0: eef6 0b00    	vmov.f64	d16, #5.000000e-01
    ddb4: ee42 0ba1    	vmla.f64	d16, d18, d17
    ddb8: ec51 0b30    	vmov	r0, r1, d16
    ddbc: f7f5 ed76    	blx	0x38ac <floor@plt>      @ imm = #-0xa514
    ddc0: ec41 0b30    	vmov	d16, r0, r1
    ddc4: 2304         	movs	r3, #0x4
    ddc6: 48d4         	ldr	r0, [pc, #0x350]        @ 0xe118 <gst_lg_audio_render_change_state+0x958>
    ddc8: f8c4 3218    	str.w	r3, [r4, #0x218]
    ddcc: eefd 7be0    	vcvt.s32.f64	s15, d16
    ddd0: 4478         	add	r0, pc
    ddd2: 68c3         	ldr	r3, [r0, #0xc]
    ddd4: ee17 9a90    	vmov	r9, s15
    ddd8: 681b         	ldr	r3, [r3]
    ddda: 075b         	lsls	r3, r3, #0x1d
    dddc: f100 8509    	bmi.w	0xe7f2 <gst_lg_audio_render_change_state+0x1032> @ imm = #0xa12
    dde0: 6833         	ldr	r3, [r6]
    dde2: 2b03         	cmp	r3, #0x3
    dde4: d913         	bls	0xde0e <gst_lg_audio_render_change_state+0x64e> @ imm = #0x26
    dde6: 4bcd         	ldr	r3, [pc, #0x334]        @ 0xe11c <gst_lg_audio_render_change_state+0x95c>
    dde8: 2104         	movs	r1, #0x4
    ddea: 4acd         	ldr	r2, [pc, #0x334]        @ 0xe120 <gst_lg_audio_render_change_state+0x960>
    ddec: f858 3003    	ldr.w	r3, [r8, r3]
    ddf0: 447a         	add	r2, pc
    ddf2: 9401         	str	r4, [sp, #0x4]
    ddf4: 6818         	ldr	r0, [r3]
    ddf6: 4bcb         	ldr	r3, [pc, #0x32c]        @ 0xe124 <gst_lg_audio_render_change_state+0x964>
    ddf8: 9202         	str	r2, [sp, #0x8]
    ddfa: f640 32ba    	movw	r2, #0xbba
    ddfe: 9200         	str	r2, [sp]
    de00: 4ac9         	ldr	r2, [pc, #0x324]        @ 0xe128 <gst_lg_audio_render_change_state+0x968>
    de02: 447b         	add	r3, pc
    de04: f503 7312    	add.w	r3, r3, #0x248
    de08: 447a         	add	r2, pc
    de0a: f7f5 ef30    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xa1a0
    de0e: f1b9 0f00    	cmp.w	r9, #0x0
    de12: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    de16: bf14         	ite	ne
    de18: 4649         	movne	r1, r9
    de1a: f44f 717a    	moveq.w	r1, #0x3e8
    de1e: f7f6 e91c    	blx	0x4058 <lgao_set_playrate@plt> @ imm = #-0x9dc8
    de22: 6f78         	ldr	r0, [r7, #0x74]
    de24: f7f6 e9a8    	blx	0x4178 <gst_clock_get_time@plt> @ imm = #-0x9cb0
    de28: f8d4 3248    	ldr.w	r3, [r4, #0x248]
    de2c: e9cd 0116    	strd	r0, r1, [sp, #88]
    de30: 2b00         	cmp	r3, #0x0
    de32: f040 8470    	bne.w	0xe716 <gst_lg_audio_render_change_state+0xf56> @ imm = #0x8e0
    de36: f8d4 22e8    	ldr.w	r2, [r4, #0x2e8]
    de3a: 2a00         	cmp	r2, #0x0
    de3c: f041 81c2    	bne.w	0xf1c4 <gst_lg_audio_render_change_state+0x1a04> @ imm = #0x1384
    de40: e9d7 9a1e    	ldrd	r9, r10, [r7, #120]
    de44: f7f6 e92c    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0x9da8
    de48: 4601         	mov	r1, r0
    de4a: 4638         	mov	r0, r7
    de4c: f7f6 e89c    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0x9ec8
    de50: 9b16         	ldr	r3, [sp, #0x58]
    de52: ebb3 0909    	subs.w	r9, r3, r9
    de56: 9b17         	ldr	r3, [sp, #0x5c]
    de58: eb63 0a0a    	sbc.w	r10, r3, r10
    de5c: 4bb3         	ldr	r3, [pc, #0x2cc]        @ 0xe12c <gst_lg_audio_render_change_state+0x96c>
    de5e: 447b         	add	r3, pc
    de60: 68db         	ldr	r3, [r3, #0xc]
    de62: 681a         	ldr	r2, [r3]
    de64: f8d0 3118    	ldr.w	r3, [r0, #0x118]
    de68: ebb9 0103    	subs.w	r1, r9, r3
    de6c: f8d0 311c    	ldr.w	r3, [r0, #0x11c]
    de70: 9118         	str	r1, [sp, #0x60]
    de72: eb6a 0b03    	sbc.w	r11, r10, r3
    de76: 0712         	lsls	r2, r2, #0x1c
    de78: f140 81f3    	bpl.w	0xe262 <gst_lg_audio_render_change_state+0xaa2> @ imm = #0x3e6
    de7c: f1bb 3fff    	cmp.w	r11, #0xffffffff
    de80: bf08         	it	eq
    de82: f1b1 3fff    	cmpeq.w	r1, #0xffffffff
    de86: f002 801f    	beq.w	0xfec8 <gst_lg_audio_render_change_state+0x2708> @ imm = #0x203e
    de8a: f44f 4220    	mov.w	r2, #0xa000
    de8e: 4608         	mov	r0, r1
    de90: f2c3 02b8    	movt	r2, #0x30b8
    de94: f240 3346    	movw	r3, #0x346
    de98: 4659         	mov	r1, r11
    de9a: f648 0989    	movw	r9, #0x8889
    de9e: f00b fc11    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb822
    dea2: a399         	adr	r3, #612 <gst_lg_audio_render_change_state+0x77f>
    dea4: e9d3 2300    	ldrd	r2, r3, [r3]
    dea8: 4659         	mov	r1, r11
    deaa: 901e         	str	r0, [sp, #0x78]
    deac: f6c8 0988    	movt	r9, #0x8888
    deb0: 9818         	ldr	r0, [sp, #0x60]
    deb2: f64e 6aef    	movw	r10, #0xeeef
    deb6: f00b fc05    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb80a
    deba: 1843         	adds	r3, r0, r1
    debc: 4602         	mov	r2, r0
    debe: f143 0300    	adc	r3, r3, #0x0
    dec2: f6ce 6aee    	movt	r10, #0xeeee
    dec6: fba9 c103    	umull	r12, r1, r9, r3
    deca: 9818         	ldr	r0, [sp, #0x60]
    decc: 08c9         	lsrs	r1, r1, #0x3
    dece: ebc1 1101    	rsb	r1, r1, r1, lsl #4
    ded2: 1a5b         	subs	r3, r3, r1
    ded4: 1ad3         	subs	r3, r2, r3
    ded6: fba3 310a    	umull	r3, r1, r3, r10
    deda: 4659         	mov	r1, r11
    dedc: 089b         	lsrs	r3, r3, #0x2
    dede: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    dee2: eba2 0383    	sub.w	r3, r2, r3, lsl #2
    dee6: 931f         	str	r3, [sp, #0x7c]
    dee8: a389         	adr	r3, #548 <gst_lg_audio_render_change_state+0x7b5>
    deea: e9d3 2300    	ldrd	r2, r3, [r3]
    deee: f00b fbe9    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb7d2
    def2: 1843         	adds	r3, r0, r1
    def4: 9219         	str	r2, [sp, #0x64]
    def6: f143 0300    	adc	r3, r3, #0x0
    defa: fba9 1203    	umull	r1, r2, r9, r3
    defe: 08d2         	lsrs	r2, r2, #0x3
    df00: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    df04: 1a9b         	subs	r3, r3, r2
    df06: 1ac3         	subs	r3, r0, r3
    df08: fba3 3a0a    	umull	r3, r10, r3, r10
    df0c: 089b         	lsrs	r3, r3, #0x2
    df0e: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    df12: eba0 0383    	sub.w	r3, r0, r3, lsl #2
    df16: 9320         	str	r3, [sp, #0x80]
    df18: e9dd 3216    	ldrd	r3, r2, [sp, #88]
    df1c: f1b2 3fff    	cmp.w	r2, #0xffffffff
    df20: bf08         	it	eq
    df22: f1b3 3fff    	cmpeq.w	r3, #0xffffffff
    df26: f001 87f9    	beq.w	0xff1c <gst_lg_audio_render_change_state+0x275c> @ imm = #0x1ff2
    df2a: 4611         	mov	r1, r2
    df2c: f44f 4220    	mov.w	r2, #0xa000
    df30: 4618         	mov	r0, r3
    df32: f2c3 02b8    	movt	r2, #0x30b8
    df36: f240 3346    	movw	r3, #0x346
    df3a: f648 0989    	movw	r9, #0x8889
    df3e: f00b fbc1    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb782
    df42: a371         	adr	r3, #452 <gst_lg_audio_render_change_state+0x7f7>
    df44: e9d3 2300    	ldrd	r2, r3, [r3]
    df48: f6c8 0988    	movt	r9, #0x8888
    df4c: 9021         	str	r0, [sp, #0x84]
    df4e: f64e 6aef    	movw	r10, #0xeeef
    df52: e9dd 0116    	ldrd	r0, r1, [sp, #88]
    df56: f6ce 6aee    	movt	r10, #0xeeee
    df5a: f00b fbb3    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb766
    df5e: 1843         	adds	r3, r0, r1
    df60: 4602         	mov	r2, r0
    df62: f143 0300    	adc	r3, r3, #0x0
    df66: 9816         	ldr	r0, [sp, #0x58]
    df68: fba9 c103    	umull	r12, r1, r9, r3
    df6c: 08c9         	lsrs	r1, r1, #0x3
    df6e: ebc1 1101    	rsb	r1, r1, r1, lsl #4
    df72: 1a5b         	subs	r3, r3, r1
    df74: 1ad3         	subs	r3, r2, r3
    df76: fba3 310a    	umull	r3, r1, r3, r10
    df7a: 9917         	ldr	r1, [sp, #0x5c]
    df7c: 089b         	lsrs	r3, r3, #0x2
    df7e: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    df82: eba2 0383    	sub.w	r3, r2, r3, lsl #2
    df86: 9322         	str	r3, [sp, #0x88]
    df88: a361         	adr	r3, #388 <gst_lg_audio_render_change_state+0x82d>
    df8a: e9d3 2300    	ldrd	r2, r3, [r3]
    df8e: f00b fb99    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb732
    df92: 1843         	adds	r3, r0, r1
    df94: 921a         	str	r2, [sp, #0x68]
    df96: f143 0300    	adc	r3, r3, #0x0
    df9a: fba9 1203    	umull	r1, r2, r9, r3
    df9e: 08d2         	lsrs	r2, r2, #0x3
    dfa0: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    dfa4: 1a9b         	subs	r3, r3, r2
    dfa6: 1ac3         	subs	r3, r0, r3
    dfa8: fba3 3a0a    	umull	r3, r10, r3, r10
    dfac: 089b         	lsrs	r3, r3, #0x2
    dfae: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    dfb2: eba0 0383    	sub.w	r3, r0, r3, lsl #2
    dfb6: 9323         	str	r3, [sp, #0x8c]
    dfb8: e9d7 9a1e    	ldrd	r9, r10, [r7, #120]
    dfbc: f1ba 3fff    	cmp.w	r10, #0xffffffff
    dfc0: bf08         	it	eq
    dfc2: f1b9 3fff    	cmpeq.w	r9, #0xffffffff
    dfc6: f001 879f    	beq.w	0xff08 <gst_lg_audio_render_change_state+0x2748> @ imm = #0x1f3e
    dfca: f64e 62ef    	movw	r2, #0xeeef
    dfce: f648 0389    	movw	r3, #0x8889
    dfd2: f6ce 62ee    	movt	r2, #0xeeee
    dfd6: f6c8 0388    	movt	r3, #0x8888
    dfda: 921c         	str	r2, [sp, #0x70]
    dfdc: f44f 4220    	mov.w	r2, #0xa000
    dfe0: 4648         	mov	r0, r9
    dfe2: 4651         	mov	r1, r10
    dfe4: f2c3 02b8    	movt	r2, #0x30b8
    dfe8: 931b         	str	r3, [sp, #0x6c]
    dfea: f240 3346    	movw	r3, #0x346
    dfee: f00b fb69    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb6d2
    dff2: a345         	adr	r3, #276 <gst_lg_audio_render_change_state+0x87b>
    dff4: e9d3 2300    	ldrd	r2, r3, [r3]
    dff8: 4651         	mov	r1, r10
    dffa: 9024         	str	r0, [sp, #0x90]
    dffc: 4648         	mov	r0, r9
    dffe: f00b fb61    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb6c2
    e002: 9a1b         	ldr	r2, [sp, #0x6c]
    e004: 1843         	adds	r3, r0, r1
    e006: f143 0300    	adc	r3, r3, #0x0
    e00a: 901d         	str	r0, [sp, #0x74]
    e00c: 4651         	mov	r1, r10
    e00e: 4648         	mov	r0, r9
    e010: fba2 ec03    	umull	lr, r12, r2, r3
    e014: 9a1d         	ldr	r2, [sp, #0x74]
    e016: ea4f 0cdc    	lsr.w	r12, r12, #0x3
    e01a: ebcc 1c0c    	rsb	r12, r12, r12, lsl #4
    e01e: eba3 030c    	sub.w	r3, r3, r12
    e022: 1ad3         	subs	r3, r2, r3
    e024: 9a1c         	ldr	r2, [sp, #0x70]
    e026: fba3 3c02    	umull	r3, r12, r3, r2
    e02a: 9a1d         	ldr	r2, [sp, #0x74]
    e02c: 089b         	lsrs	r3, r3, #0x2
    e02e: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e032: eba2 0383    	sub.w	r3, r2, r3, lsl #2
    e036: 9325         	str	r3, [sp, #0x94]
    e038: a335         	adr	r3, #212 <gst_lg_audio_render_change_state+0x8b1>
    e03a: e9d3 2300    	ldrd	r2, r3, [r3]
    e03e: f00b fb41    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb682
    e042: 4692         	mov	r10, r2
    e044: 9a1b         	ldr	r2, [sp, #0x6c]
    e046: 1843         	adds	r3, r0, r1
    e048: f143 0300    	adc	r3, r3, #0x0
    e04c: fba2 1203    	umull	r1, r2, r2, r3
    e050: 08d2         	lsrs	r2, r2, #0x3
    e052: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    e056: 1a9b         	subs	r3, r3, r2
    e058: 9a1c         	ldr	r2, [sp, #0x70]
    e05a: 1ac3         	subs	r3, r0, r3
    e05c: fba3 3202    	umull	r3, r2, r3, r2
    e060: 089b         	lsrs	r3, r3, #0x2
    e062: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e066: eba0 0983    	sub.w	r9, r0, r3, lsl #2
    e06a: f7f6 e81a    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0x9fcc
    e06e: 4601         	mov	r1, r0
    e070: 4638         	mov	r0, r7
    e072: f7f5 ef8a    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa0ec
    e076: e9d0 2346    	ldrd	r2, r3, [r0, #280]
    e07a: f1b3 3fff    	cmp.w	r3, #0xffffffff
    e07e: bf08         	it	eq
    e080: f1b2 3fff    	cmpeq.w	r2, #0xffffffff
    e084: bf04         	itt	eq
    e086: 2363         	moveq	r3, #0x63
    e088: 931b         	streq	r3, [sp, #0x6c]
    e08a: d010         	beq	0xe0ae <gst_lg_audio_render_change_state+0x8ee> @ imm = #0x20
    e08c: f7f6 e808    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0x9ff0
    e090: 4601         	mov	r1, r0
    e092: 4638         	mov	r0, r7
    e094: f7f5 ef78    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa110
    e098: f44f 4220    	mov.w	r2, #0xa000
    e09c: e9d0 0146    	ldrd	r0, r1, [r0, #280]
    e0a0: f2c3 02b8    	movt	r2, #0x30b8
    e0a4: f240 3346    	movw	r3, #0x346
    e0a8: f00b fb0c    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb618
    e0ac: 901b         	str	r0, [sp, #0x6c]
    e0ae: f7f5 eff8    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa010
    e0b2: 4601         	mov	r1, r0
    e0b4: 4638         	mov	r0, r7
    e0b6: f7f5 ef68    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa130
    e0ba: e9d0 2346    	ldrd	r2, r3, [r0, #280]
    e0be: f1b3 3fff    	cmp.w	r3, #0xffffffff
    e0c2: bf08         	it	eq
    e0c4: f1b2 3fff    	cmpeq.w	r2, #0xffffffff
    e0c8: bf04         	itt	eq
    e0ca: 2363         	moveq	r3, #0x63
    e0cc: 931c         	streq	r3, [sp, #0x70]
    e0ce: d03f         	beq	0xe150 <gst_lg_audio_render_change_state+0x990> @ imm = #0x7e
    e0d0: f7f5 efe6    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa034
    e0d4: 4601         	mov	r1, r0
    e0d6: 4638         	mov	r0, r7
    e0d8: f7f5 ef56    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa154
    e0dc: a30a         	adr	r3, #40 <gst_lg_audio_render_change_state+0x92a>
    e0de: e9d3 2300    	ldrd	r2, r3, [r3]
    e0e2: e9d0 0146    	ldrd	r0, r1, [r0, #280]
    e0e6: f00b faed    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb5da
    e0ea: f648 0289    	movw	r2, #0x8889
    e0ee: 1843         	adds	r3, r0, r1
    e0f0: f143 0300    	adc	r3, r3, #0x0
    e0f4: f6c8 0288    	movt	r2, #0x8888
    e0f8: fba2 1203    	umull	r1, r2, r2, r3
    e0fc: 08d2         	lsrs	r2, r2, #0x3
    e0fe: e017         	b	0xe130 <gst_lg_audio_render_change_state+0x970> @ imm = #0x2e
    e100: 00 00 00 00  	.word	0x00000000
    e104: 00 40 8f 40  	.word	0x408f4000
    e108: 00 58 47 f8  	.word	0xf8475800
    e10c: 0d 00 00 00  	.word	0x0000000d
    e110: 00 ca 9a 3b  	.word	0x3b9aca00
    e114: 00 00 00 00  	.word	0x00000000
    e118: 30 42 02 00  	.word	0x00024230
    e11c: 08 04 00 00  	.word	0x00000408
    e120: 18 fc 00 00  	.word	0x0000fc18
    e124: 92 1d 01 00  	.word	0x00011d92
    e128: 30 bb 00 00  	.word	0x0000bb30
    e12c: a2 41 02 00  	.word	0x000241a2
    e130: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    e134: 1a9b         	subs	r3, r3, r2
    e136: f64e 62ef    	movw	r2, #0xeeef
    e13a: 1ac3         	subs	r3, r0, r3
    e13c: f6ce 62ee    	movt	r2, #0xeeee
    e140: fba3 3202    	umull	r3, r2, r3, r2
    e144: 089b         	lsrs	r3, r3, #0x2
    e146: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e14a: eba0 0383    	sub.w	r3, r0, r3, lsl #2
    e14e: 931c         	str	r3, [sp, #0x70]
    e150: f7f5 efa6    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa0b4
    e154: 4601         	mov	r1, r0
    e156: 4638         	mov	r0, r7
    e158: f7f5 ef16    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa1d4
    e15c: e9d0 2346    	ldrd	r2, r3, [r0, #280]
    e160: f1b3 3fff    	cmp.w	r3, #0xffffffff
    e164: bf08         	it	eq
    e166: f1b2 3fff    	cmpeq.w	r2, #0xffffffff
    e16a: bf04         	itt	eq
    e16c: 2363         	moveq	r3, #0x63
    e16e: 931d         	streq	r3, [sp, #0x74]
    e170: d027         	beq	0xe1c2 <gst_lg_audio_render_change_state+0xa02> @ imm = #0x4e
    e172: f7f5 ef96    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa0d4
    e176: 4601         	mov	r1, r0
    e178: 4638         	mov	r0, r7
    e17a: f7f5 ef06    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa1f4
    e17e: f60f 4368    	adr.w	r3, #3176
    e182: e9d3 2300    	ldrd	r2, r3, [r3]
    e186: e9d0 0146    	ldrd	r0, r1, [r0, #280]
    e18a: f00b fa9b    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb536
    e18e: f648 0289    	movw	r2, #0x8889
    e192: 1843         	adds	r3, r0, r1
    e194: f143 0300    	adc	r3, r3, #0x0
    e198: f6c8 0288    	movt	r2, #0x8888
    e19c: fba2 1203    	umull	r1, r2, r2, r3
    e1a0: 08d2         	lsrs	r2, r2, #0x3
    e1a2: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    e1a6: 1a9b         	subs	r3, r3, r2
    e1a8: f64e 62ef    	movw	r2, #0xeeef
    e1ac: 1ac3         	subs	r3, r0, r3
    e1ae: f6ce 62ee    	movt	r2, #0xeeee
    e1b2: fba3 3202    	umull	r3, r2, r3, r2
    e1b6: 089b         	lsrs	r3, r3, #0x2
    e1b8: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e1bc: eba0 0383    	sub.w	r3, r0, r3, lsl #2
    e1c0: 931d         	str	r3, [sp, #0x74]
    e1c2: f7f5 ef6e    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa124
    e1c6: 4601         	mov	r1, r0
    e1c8: 4638         	mov	r0, r7
    e1ca: f7f5 eede    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa244
    e1ce: e9d0 2346    	ldrd	r2, r3, [r0, #280]
    e1d2: f1b3 3fff    	cmp.w	r3, #0xffffffff
    e1d6: bf08         	it	eq
    e1d8: f1b2 3fff    	cmpeq.w	r2, #0xffffffff
    e1dc: bf04         	itt	eq
    e1de: f64c 12ff    	movweq	r2, #0xc9ff
    e1e2: f6c3 329a    	movteq	r2, #0x3b9a
    e1e6: d00d         	beq	0xe204 <gst_lg_audio_render_change_state+0xa44> @ imm = #0x1a
    e1e8: f7f5 ef5a    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa14c
    e1ec: 4601         	mov	r1, r0
    e1ee: 4638         	mov	r0, r7
    e1f0: f7f5 eeca    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa26c
    e1f4: f60f 33f0    	adr.w	r3, #3056
    e1f8: e9d3 2300    	ldrd	r2, r3, [r3]
    e1fc: e9d0 0146    	ldrd	r0, r1, [r0, #280]
    e200: f00b fa60    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb4c0
    e204: 9b1c         	ldr	r3, [sp, #0x70]
    e206: 2103         	movs	r1, #0x3
    e208: 9210         	str	r2, [sp, #0x40]
    e20a: f8df 2bec    	ldr.w	r2, [pc, #0xbec]        @ 0xedf8 <gst_lg_audio_render_change_state+0x1638>
    e20e: 930e         	str	r3, [sp, #0x38]
    e210: 9b1b         	ldr	r3, [sp, #0x6c]
    e212: f8df 0be8    	ldr.w	r0, [pc, #0xbe8]        @ 0xedfc <gst_lg_audio_render_change_state+0x163c>
    e216: 447a         	add	r2, pc
    e218: f502 7212    	add.w	r2, r2, #0x248
    e21c: f8cd 902c    	str.w	r9, [sp, #0x2c]
    e220: e9cd a30c    	strd	r10, r3, [sp, #48]
    e224: 9b25         	ldr	r3, [sp, #0x94]
    e226: 4478         	add	r0, pc
    e228: 930a         	str	r3, [sp, #0x28]
    e22a: 9b24         	ldr	r3, [sp, #0x90]
    e22c: 9309         	str	r3, [sp, #0x24]
    e22e: 9b1a         	ldr	r3, [sp, #0x68]
    e230: 9308         	str	r3, [sp, #0x20]
    e232: 9b23         	ldr	r3, [sp, #0x8c]
    e234: 9307         	str	r3, [sp, #0x1c]
    e236: 9b22         	ldr	r3, [sp, #0x88]
    e238: 9306         	str	r3, [sp, #0x18]
    e23a: 9b21         	ldr	r3, [sp, #0x84]
    e23c: 9305         	str	r3, [sp, #0x14]
    e23e: 9b19         	ldr	r3, [sp, #0x64]
    e240: 9304         	str	r3, [sp, #0x10]
    e242: 9b20         	ldr	r3, [sp, #0x80]
    e244: 9303         	str	r3, [sp, #0xc]
    e246: 9b1f         	ldr	r3, [sp, #0x7c]
    e248: 9302         	str	r3, [sp, #0x8]
    e24a: 9b1e         	ldr	r3, [sp, #0x78]
    e24c: 9301         	str	r3, [sp, #0x4]
    e24e: 9b1d         	ldr	r3, [sp, #0x74]
    e250: 930f         	str	r3, [sp, #0x3c]
    e252: f8df 3bac    	ldr.w	r3, [pc, #0xbac]        @ 0xee00 <gst_lg_audio_render_change_state+0x1640>
    e256: 447b         	add	r3, pc
    e258: 9300         	str	r3, [sp]
    e25a: f640 33c9    	movw	r3, #0xbc9
    e25e: f7f5 eea6    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xa2b4
    e262: 6833         	ldr	r3, [r6]
    e264: 2b03         	cmp	r3, #0x3
    e266: f240 81f1    	bls.w	0xe64c <gst_lg_audio_render_change_state+0xe8c> @ imm = #0x3e2
    e26a: 9b18         	ldr	r3, [sp, #0x60]
    e26c: f1bb 3fff    	cmp.w	r11, #0xffffffff
    e270: bf08         	it	eq
    e272: f1b3 3fff    	cmpeq.w	r3, #0xffffffff
    e276: f001 863c    	beq.w	0xfef2 <gst_lg_audio_render_change_state+0x2732> @ imm = #0x1c78
    e27a: f44f 4220    	mov.w	r2, #0xa000
    e27e: 4618         	mov	r0, r3
    e280: f2c3 02b8    	movt	r2, #0x30b8
    e284: f240 3346    	movw	r3, #0x346
    e288: 4659         	mov	r1, r11
    e28a: f648 0989    	movw	r9, #0x8889
    e28e: f00b fa19    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb432
    e292: f60f 335c    	adr.w	r3, #2908
    e296: e9d3 2300    	ldrd	r2, r3, [r3]
    e29a: 4659         	mov	r1, r11
    e29c: 901a         	str	r0, [sp, #0x68]
    e29e: f6c8 0988    	movt	r9, #0x8888
    e2a2: 9818         	ldr	r0, [sp, #0x60]
    e2a4: f64e 6aef    	movw	r10, #0xeeef
    e2a8: f00b fa0c    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb418
    e2ac: 1843         	adds	r3, r0, r1
    e2ae: 4602         	mov	r2, r0
    e2b0: f143 0300    	adc	r3, r3, #0x0
    e2b4: f6ce 6aee    	movt	r10, #0xeeee
    e2b8: fba9 c103    	umull	r12, r1, r9, r3
    e2bc: 9818         	ldr	r0, [sp, #0x60]
    e2be: 08c9         	lsrs	r1, r1, #0x3
    e2c0: ebc1 1101    	rsb	r1, r1, r1, lsl #4
    e2c4: 1a5b         	subs	r3, r3, r1
    e2c6: 1ad3         	subs	r3, r2, r3
    e2c8: fba3 310a    	umull	r3, r1, r3, r10
    e2cc: 4659         	mov	r1, r11
    e2ce: 089b         	lsrs	r3, r3, #0x2
    e2d0: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e2d4: eba2 0383    	sub.w	r3, r2, r3, lsl #2
    e2d8: 931b         	str	r3, [sp, #0x6c]
    e2da: f60f 330c    	adr.w	r3, #2828
    e2de: e9d3 2300    	ldrd	r2, r3, [r3]
    e2e2: f00b f9ef    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb3de
    e2e6: 1843         	adds	r3, r0, r1
    e2e8: 9219         	str	r2, [sp, #0x64]
    e2ea: f143 0300    	adc	r3, r3, #0x0
    e2ee: fba9 1203    	umull	r1, r2, r9, r3
    e2f2: 08d2         	lsrs	r2, r2, #0x3
    e2f4: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    e2f8: 1a9b         	subs	r3, r3, r2
    e2fa: 1ac3         	subs	r3, r0, r3
    e2fc: fba3 320a    	umull	r3, r2, r3, r10
    e300: 089b         	lsrs	r3, r3, #0x2
    e302: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e306: eba0 0383    	sub.w	r3, r0, r3, lsl #2
    e30a: 931c         	str	r3, [sp, #0x70]
    e30c: e9dd 3216    	ldrd	r3, r2, [sp, #88]
    e310: f1b2 3fff    	cmp.w	r2, #0xffffffff
    e314: bf08         	it	eq
    e316: f1b3 3fff    	cmpeq.w	r3, #0xffffffff
    e31a: f001 86a4    	beq.w	0x10066 <gst_lg_audio_render_change_state+0x28a6> @ imm = #0x1d48
    e31e: 4611         	mov	r1, r2
    e320: f44f 4220    	mov.w	r2, #0xa000
    e324: 4618         	mov	r0, r3
    e326: f2c3 02b8    	movt	r2, #0x30b8
    e32a: f240 3346    	movw	r3, #0x346
    e32e: f648 0989    	movw	r9, #0x8889
    e332: f00b f9c7    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb38e
    e336: f60f 23b8    	adr.w	r3, #2744
    e33a: e9d3 2300    	ldrd	r2, r3, [r3]
    e33e: f6c8 0988    	movt	r9, #0x8888
    e342: 9020         	str	r0, [sp, #0x80]
    e344: f64e 6aef    	movw	r10, #0xeeef
    e348: e9dd 0116    	ldrd	r0, r1, [sp, #88]
    e34c: f6ce 6aee    	movt	r10, #0xeeee
    e350: f00b f9b8    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb370
    e354: 1843         	adds	r3, r0, r1
    e356: 4602         	mov	r2, r0
    e358: f143 0300    	adc	r3, r3, #0x0
    e35c: 9917         	ldr	r1, [sp, #0x5c]
    e35e: fba9 ec03    	umull	lr, r12, r9, r3
    e362: 9816         	ldr	r0, [sp, #0x58]
    e364: ea4f 0cdc    	lsr.w	r12, r12, #0x3
    e368: ebcc 1c0c    	rsb	r12, r12, r12, lsl #4
    e36c: eba3 030c    	sub.w	r3, r3, r12
    e370: 1ad3         	subs	r3, r2, r3
    e372: fba3 3c0a    	umull	r3, r12, r3, r10
    e376: 089b         	lsrs	r3, r3, #0x2
    e378: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e37c: eba2 0383    	sub.w	r3, r2, r3, lsl #2
    e380: 9321         	str	r3, [sp, #0x84]
    e382: f60f 2364    	adr.w	r3, #2660
    e386: e9d3 2300    	ldrd	r2, r3, [r3]
    e38a: f00b f99b    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb336
    e38e: 1843         	adds	r3, r0, r1
    e390: 9217         	str	r2, [sp, #0x5c]
    e392: f143 0300    	adc	r3, r3, #0x0
    e396: fba9 1203    	umull	r1, r2, r9, r3
    e39a: 08d2         	lsrs	r2, r2, #0x3
    e39c: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    e3a0: 1a9b         	subs	r3, r3, r2
    e3a2: 1ac3         	subs	r3, r0, r3
    e3a4: fba3 320a    	umull	r3, r2, r3, r10
    e3a8: 089b         	lsrs	r3, r3, #0x2
    e3aa: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e3ae: eba0 0383    	sub.w	r3, r0, r3, lsl #2
    e3b2: 9322         	str	r3, [sp, #0x88]
    e3b4: e9d7 9a1e    	ldrd	r9, r10, [r7, #120]
    e3b8: f1ba 3fff    	cmp.w	r10, #0xffffffff
    e3bc: bf08         	it	eq
    e3be: f1b9 3fff    	cmpeq.w	r9, #0xffffffff
    e3c2: f001 863b    	beq.w	0x1003c <gst_lg_audio_render_change_state+0x287c> @ imm = #0x1c76
    e3c6: f64e 6cef    	movw	r12, #0xeeef
    e3ca: f648 0389    	movw	r3, #0x8889
    e3ce: 4662         	mov	r2, r12
    e3d0: f6c8 0388    	movt	r3, #0x8888
    e3d4: f6ce 62ee    	movt	r2, #0xeeee
    e3d8: 4648         	mov	r0, r9
    e3da: 921d         	str	r2, [sp, #0x74]
    e3dc: f44f 4220    	mov.w	r2, #0xa000
    e3e0: 4651         	mov	r1, r10
    e3e2: f2c3 02b8    	movt	r2, #0x30b8
    e3e6: 9316         	str	r3, [sp, #0x58]
    e3e8: f240 3346    	movw	r3, #0x346
    e3ec: f00b f96a    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb2d4
    e3f0: f60f 13fc    	adr.w	r3, #2556
    e3f4: e9d3 2300    	ldrd	r2, r3, [r3]
    e3f8: 4651         	mov	r1, r10
    e3fa: 9023         	str	r0, [sp, #0x8c]
    e3fc: 4648         	mov	r0, r9
    e3fe: f00b f961    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb2c2
    e402: 9a16         	ldr	r2, [sp, #0x58]
    e404: 1843         	adds	r3, r0, r1
    e406: f143 0300    	adc	r3, r3, #0x0
    e40a: 901e         	str	r0, [sp, #0x78]
    e40c: 4651         	mov	r1, r10
    e40e: 4648         	mov	r0, r9
    e410: fba2 ec03    	umull	lr, r12, r2, r3
    e414: 9a1e         	ldr	r2, [sp, #0x78]
    e416: ea4f 0cdc    	lsr.w	r12, r12, #0x3
    e41a: ebcc 1c0c    	rsb	r12, r12, r12, lsl #4
    e41e: eba3 030c    	sub.w	r3, r3, r12
    e422: 1ad3         	subs	r3, r2, r3
    e424: 9a1d         	ldr	r2, [sp, #0x74]
    e426: fba3 3c02    	umull	r3, r12, r3, r2
    e42a: 9a1e         	ldr	r2, [sp, #0x78]
    e42c: 089b         	lsrs	r3, r3, #0x2
    e42e: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e432: eba2 0c83    	sub.w	r12, r2, r3, lsl #2
    e436: f60f 13b0    	adr.w	r3, #2480
    e43a: e9d3 2300    	ldrd	r2, r3, [r3]
    e43e: f8cd c090    	str.w	r12, [sp, #0x90]
    e442: f00b f93f    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb27e
    e446: 4691         	mov	r9, r2
    e448: 9a16         	ldr	r2, [sp, #0x58]
    e44a: 1843         	adds	r3, r0, r1
    e44c: f143 0300    	adc	r3, r3, #0x0
    e450: fba2 1203    	umull	r1, r2, r2, r3
    e454: 08d2         	lsrs	r2, r2, #0x3
    e456: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    e45a: 1a9b         	subs	r3, r3, r2
    e45c: 9a1d         	ldr	r2, [sp, #0x74]
    e45e: 1ac3         	subs	r3, r0, r3
    e460: fba3 3202    	umull	r3, r2, r3, r2
    e464: 089b         	lsrs	r3, r3, #0x2
    e466: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e46a: eba0 0a83    	sub.w	r10, r0, r3, lsl #2
    e46e: f8df 3994    	ldr.w	r3, [pc, #0x994]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    e472: f858 3003    	ldr.w	r3, [r8, r3]
    e476: 681b         	ldr	r3, [r3]
    e478: 9316         	str	r3, [sp, #0x58]
    e47a: f7f5 ee12    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa3dc
    e47e: 4601         	mov	r1, r0
    e480: 4638         	mov	r0, r7
    e482: f7f5 ed82    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa4fc
    e486: e9d0 2346    	ldrd	r2, r3, [r0, #280]
    e48a: f1b3 3fff    	cmp.w	r3, #0xffffffff
    e48e: bf08         	it	eq
    e490: f1b2 3fff    	cmpeq.w	r2, #0xffffffff
    e494: bf04         	itt	eq
    e496: 2363         	moveq	r3, #0x63
    e498: 931d         	streq	r3, [sp, #0x74]
    e49a: d010         	beq	0xe4be <gst_lg_audio_render_change_state+0xcfe> @ imm = #0x20
    e49c: f7f5 ee00    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa400
    e4a0: 4601         	mov	r1, r0
    e4a2: 4638         	mov	r0, r7
    e4a4: f7f5 ed70    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa520
    e4a8: f44f 4220    	mov.w	r2, #0xa000
    e4ac: e9d0 0146    	ldrd	r0, r1, [r0, #280]
    e4b0: f2c3 02b8    	movt	r2, #0x30b8
    e4b4: f240 3346    	movw	r3, #0x346
    e4b8: f00b f904    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb208
    e4bc: 901d         	str	r0, [sp, #0x74]
    e4be: f7f5 edf0    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa420
    e4c2: 4601         	mov	r1, r0
    e4c4: 4638         	mov	r0, r7
    e4c6: f7f5 ed60    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa540
    e4ca: e9d0 2346    	ldrd	r2, r3, [r0, #280]
    e4ce: f1b3 3fff    	cmp.w	r3, #0xffffffff
    e4d2: bf08         	it	eq
    e4d4: f1b2 3fff    	cmpeq.w	r2, #0xffffffff
    e4d8: bf04         	itt	eq
    e4da: 2363         	moveq	r3, #0x63
    e4dc: 931e         	streq	r3, [sp, #0x78]
    e4de: d028         	beq	0xe532 <gst_lg_audio_render_change_state+0xd72> @ imm = #0x50
    e4e0: f7f5 edde    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa444
    e4e4: 4601         	mov	r1, r0
    e4e6: 4638         	mov	r0, r7
    e4e8: f7f5 ed4e    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa564
    e4ec: f60f 1300    	adr.w	r3, #2304
    e4f0: e9d3 2300    	ldrd	r2, r3, [r3]
    e4f4: e9d0 0146    	ldrd	r0, r1, [r0, #280]
    e4f8: f00b f8e4    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb1c8
    e4fc: 1843         	adds	r3, r0, r1
    e4fe: f648 0189    	movw	r1, #0x8889
    e502: f143 0300    	adc	r3, r3, #0x0
    e506: f6c8 0188    	movt	r1, #0x8888
    e50a: 4602         	mov	r2, r0
    e50c: fba1 0103    	umull	r0, r1, r1, r3
    e510: 08c9         	lsrs	r1, r1, #0x3
    e512: ebc1 1101    	rsb	r1, r1, r1, lsl #4
    e516: 1a5b         	subs	r3, r3, r1
    e518: f64e 61ef    	movw	r1, #0xeeef
    e51c: 1ad3         	subs	r3, r2, r3
    e51e: f6ce 61ee    	movt	r1, #0xeeee
    e522: fba3 3101    	umull	r3, r1, r3, r1
    e526: 089b         	lsrs	r3, r3, #0x2
    e528: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e52c: eba2 0383    	sub.w	r3, r2, r3, lsl #2
    e530: 931e         	str	r3, [sp, #0x78]
    e532: f7f5 edb6    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa494
    e536: 4601         	mov	r1, r0
    e538: 4638         	mov	r0, r7
    e53a: f7f5 ed26    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa5b4
    e53e: e9d0 2346    	ldrd	r2, r3, [r0, #280]
    e542: f1b3 3fff    	cmp.w	r3, #0xffffffff
    e546: bf08         	it	eq
    e548: f1b2 3fff    	cmpeq.w	r2, #0xffffffff
    e54c: bf04         	itt	eq
    e54e: 2363         	moveq	r3, #0x63
    e550: 931f         	streq	r3, [sp, #0x7c]
    e552: d028         	beq	0xe5a6 <gst_lg_audio_render_change_state+0xde6> @ imm = #0x50
    e554: f7f5 eda4    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa4b8
    e558: 4601         	mov	r1, r0
    e55a: 4638         	mov	r0, r7
    e55c: f7f5 ed14    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa5d8
    e560: f60f 0384    	adr.w	r3, #2180
    e564: e9d3 2300    	ldrd	r2, r3, [r3]
    e568: e9d0 0146    	ldrd	r0, r1, [r0, #280]
    e56c: f00b f8aa    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb154
    e570: 1843         	adds	r3, r0, r1
    e572: f648 0189    	movw	r1, #0x8889
    e576: f143 0300    	adc	r3, r3, #0x0
    e57a: f6c8 0188    	movt	r1, #0x8888
    e57e: 4602         	mov	r2, r0
    e580: fba1 0103    	umull	r0, r1, r1, r3
    e584: 08c9         	lsrs	r1, r1, #0x3
    e586: ebc1 1101    	rsb	r1, r1, r1, lsl #4
    e58a: 1a5b         	subs	r3, r3, r1
    e58c: f64e 61ef    	movw	r1, #0xeeef
    e590: 1ad3         	subs	r3, r2, r3
    e592: f6ce 61ee    	movt	r1, #0xeeee
    e596: fba3 3101    	umull	r3, r1, r3, r1
    e59a: 089b         	lsrs	r3, r3, #0x2
    e59c: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    e5a0: eba2 0383    	sub.w	r3, r2, r3, lsl #2
    e5a4: 931f         	str	r3, [sp, #0x7c]
    e5a6: f7f5 ed7c    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa508
    e5aa: 4601         	mov	r1, r0
    e5ac: 4638         	mov	r0, r7
    e5ae: f7f5 ecec    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa628
    e5b2: e9d0 2346    	ldrd	r2, r3, [r0, #280]
    e5b6: f1b3 3fff    	cmp.w	r3, #0xffffffff
    e5ba: bf08         	it	eq
    e5bc: f1b2 3fff    	cmpeq.w	r2, #0xffffffff
    e5c0: bf04         	itt	eq
    e5c2: f64c 12ff    	movweq	r2, #0xc9ff
    e5c6: f6c3 329a    	movteq	r2, #0x3b9a
    e5ca: d00d         	beq	0xe5e8 <gst_lg_audio_render_change_state+0xe28> @ imm = #0x1a
    e5cc: f7f5 ed68    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa530
    e5d0: 4601         	mov	r1, r0
    e5d2: 4638         	mov	r0, r7
    e5d4: f7f5 ecd8    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa650
    e5d8: f60f 030c    	adr.w	r3, #2060
    e5dc: e9d3 2300    	ldrd	r2, r3, [r3]
    e5e0: e9d0 0146    	ldrd	r0, r1, [r0, #280]
    e5e4: f00b f86e    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xb0dc
    e5e8: 9b24         	ldr	r3, [sp, #0x90]
    e5ea: 2104         	movs	r1, #0x4
    e5ec: 9212         	str	r2, [sp, #0x48]
    e5ee: f8df 2818    	ldr.w	r2, [pc, #0x818]        @ 0xee08 <gst_lg_audio_render_change_state+0x1648>
    e5f2: 930c         	str	r3, [sp, #0x30]
    e5f4: 9b23         	ldr	r3, [sp, #0x8c]
    e5f6: 447a         	add	r2, pc
    e5f8: 9816         	ldr	r0, [sp, #0x58]
    e5fa: 9202         	str	r2, [sp, #0x8]
    e5fc: f640 32c9    	movw	r2, #0xbc9
    e600: 930b         	str	r3, [sp, #0x2c]
    e602: 9b17         	ldr	r3, [sp, #0x5c]
    e604: f8cd a034    	str.w	r10, [sp, #0x34]
    e608: 9401         	str	r4, [sp, #0x4]
    e60a: 930a         	str	r3, [sp, #0x28]
    e60c: 9b22         	ldr	r3, [sp, #0x88]
    e60e: 9309         	str	r3, [sp, #0x24]
    e610: 9b21         	ldr	r3, [sp, #0x84]
    e612: 9308         	str	r3, [sp, #0x20]
    e614: 9b20         	ldr	r3, [sp, #0x80]
    e616: 9307         	str	r3, [sp, #0x1c]
    e618: 9b19         	ldr	r3, [sp, #0x64]
    e61a: 9306         	str	r3, [sp, #0x18]
    e61c: 9b1c         	ldr	r3, [sp, #0x70]
    e61e: 9305         	str	r3, [sp, #0x14]
    e620: 9b1b         	ldr	r3, [sp, #0x6c]
    e622: 9304         	str	r3, [sp, #0x10]
    e624: 9b1a         	ldr	r3, [sp, #0x68]
    e626: 9303         	str	r3, [sp, #0xc]
    e628: 9b1f         	ldr	r3, [sp, #0x7c]
    e62a: 9311         	str	r3, [sp, #0x44]
    e62c: 9b1e         	ldr	r3, [sp, #0x78]
    e62e: 9310         	str	r3, [sp, #0x40]
    e630: 9b1d         	ldr	r3, [sp, #0x74]
    e632: e9cd 930e    	strd	r9, r3, [sp, #56]
    e636: f8df 37d4    	ldr.w	r3, [pc, #0x7d4]        @ 0xee0c <gst_lg_audio_render_change_state+0x164c>
    e63a: 9200         	str	r2, [sp]
    e63c: f8df 27d0    	ldr.w	r2, [pc, #0x7d0]        @ 0xee10 <gst_lg_audio_render_change_state+0x1650>
    e640: 447b         	add	r3, pc
    e642: f503 7312    	add.w	r3, r3, #0x248
    e646: 447a         	add	r2, pc
    e648: f7f5 eb10    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xa9e0
    e64c: f7f5 ed28    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa5b0
    e650: 4601         	mov	r1, r0
    e652: 4638         	mov	r0, r7
    e654: f7f5 ec98    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa6d0
    e658: e9d0 014a    	ldrd	r0, r1, [r0, #296]
    e65c: 465b         	mov	r3, r11
    e65e: 9a18         	ldr	r2, [sp, #0x60]
    e660: e9cd 0100    	strd	r0, r1, [sp]
    e664: 4620         	mov	r0, r4
    e666: f7f6 fd77    	bl	0x5158 <gst_lg_audio_render_update_clock> @ imm = #-0x9512
    e66a: e06a         	b	0xe742 <gst_lg_audio_render_change_state+0xf82> @ imm = #0xd4
    e66c: 6813         	ldr	r3, [r2]
    e66e: 071a         	lsls	r2, r3, #0x1c
    e670: f101 814a    	bmi.w	0xf908 <gst_lg_audio_render_change_state+0x2148> @ imm = #0x1294
    e674: 6833         	ldr	r3, [r6]
    e676: 2b03         	cmp	r3, #0x3
    e678: f201 8069    	bhi.w	0xf74e <gst_lg_audio_render_change_state+0x1f8e> @ imm = #0x10d2
    e67c: f1b9 0f22    	cmp.w	r9, #0x22
    e680: f43f aa6b    	beq.w	0xdb5a <gst_lg_audio_render_change_state+0x39a> @ imm = #-0xb2a
    e684: f109 0901    	add.w	r9, r9, #0x1
    e688: f7ff bae5    	b.w	0xdc56 <gst_lg_audio_render_change_state+0x496> @ imm = #-0xa36
    e68c: f8df 9784    	ldr.w	r9, [pc, #0x784]        @ 0xee14 <gst_lg_audio_render_change_state+0x1654>
    e690: 44f9         	add	r9, pc
    e692: f7ff ba07    	b.w	0xdaa4 <gst_lg_audio_render_change_state+0x2e4> @ imm = #-0xbf2
    e696: f8df a780    	ldr.w	r10, [pc, #0x780]       @ 0xee18 <gst_lg_audio_render_change_state+0x1658>
    e69a: 44fa         	add	r10, pc
    e69c: f7ff b928    	b.w	0xd8f0 <gst_lg_audio_render_change_state+0x130> @ imm = #-0xdb0
    e6a0: f8df 0778    	ldr.w	r0, [pc, #0x778]        @ 0xee1c <gst_lg_audio_render_change_state+0x165c>
    e6a4: 4478         	add	r0, pc
    e6a6: 68c3         	ldr	r3, [r0, #0xc]
    e6a8: 681b         	ldr	r3, [r3]
    e6aa: 071b         	lsls	r3, r3, #0x1c
    e6ac: f101 82be    	bmi.w	0xfc2c <gst_lg_audio_render_change_state+0x246c> @ imm = #0x157c
    e6b0: 6833         	ldr	r3, [r6]
    e6b2: 2b03         	cmp	r3, #0x3
    e6b4: d917         	bls	0xe6e6 <gst_lg_audio_render_change_state+0xf26> @ imm = #0x2e
    e6b6: f8df 374c    	ldr.w	r3, [pc, #0x74c]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    e6ba: 2104         	movs	r1, #0x4
    e6bc: f8df 2760    	ldr.w	r2, [pc, #0x760]        @ 0xee20 <gst_lg_audio_render_change_state+0x1660>
    e6c0: f858 3003    	ldr.w	r3, [r8, r3]
    e6c4: 447a         	add	r2, pc
    e6c6: 9401         	str	r4, [sp, #0x4]
    e6c8: 6818         	ldr	r0, [r3]
    e6ca: f8df 3758    	ldr.w	r3, [pc, #0x758]        @ 0xee24 <gst_lg_audio_render_change_state+0x1664>
    e6ce: 9202         	str	r2, [sp, #0x8]
    e6d0: f640 32fd    	movw	r2, #0xbfd
    e6d4: 9200         	str	r2, [sp]
    e6d6: f8df 2750    	ldr.w	r2, [pc, #0x750]        @ 0xee28 <gst_lg_audio_render_change_state+0x1668>
    e6da: 447b         	add	r3, pc
    e6dc: f503 7312    	add.w	r3, r3, #0x248
    e6e0: 447a         	add	r2, pc
    e6e2: f7f5 eac4    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xaa78
    e6e6: f8d4 3218    	ldr.w	r3, [r4, #0x218]
    e6ea: 2b04         	cmp	r3, #0x4
    e6ec: f041 8045    	bne.w	0xf77a <gst_lg_audio_render_change_state+0x1fba> @ imm = #0x108a
    e6f0: f8df 3738    	ldr.w	r3, [pc, #0x738]        @ 0xee2c <gst_lg_audio_render_change_state+0x166c>
    e6f4: 447b         	add	r3, pc
    e6f6: f8d3 9000    	ldr.w	r9, [r3]
    e6fa: f7f5 ec4c    	blx	0x3f94 <gst_element_get_type@plt> @ imm = #-0xa768
    e6fe: 4601         	mov	r1, r0
    e700: 4648         	mov	r0, r9
    e702: f7f5 ea76    	blx	0x3bf0 <g_type_check_class_cast@plt> @ imm = #-0xab14
    e706: 2124         	movs	r1, #0x24
    e708: f8d0 308c    	ldr.w	r3, [r0, #0x8c]
    e70c: 4638         	mov	r0, r7
    e70e: 4798         	blx	r3
    e710: 4607         	mov	r7, r0
    e712: f7ff b958    	b.w	0xd9c6 <gst_lg_audio_render_change_state+0x206> @ imm = #-0xd50
    e716: f7f5 ecc4    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa678
    e71a: 4601         	mov	r1, r0
    e71c: 4620         	mov	r0, r4
    e71e: f7f5 ec34    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa798
    e722: 4681         	mov	r9, r0
    e724: f7f5 ecbc    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xa688
    e728: 4601         	mov	r1, r0
    e72a: 4638         	mov	r0, r7
    e72c: f7f5 ec2c    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xa7a8
    e730: f04f 32ff    	mov.w	r2, #0xffffffff
    e734: f100 01f8    	add.w	r1, r0, #0xf8
    e738: f04f 33ff    	mov.w	r3, #0xffffffff
    e73c: 4648         	mov	r0, r9
    e73e: f7f8 ff47    	bl	0x75d0 <gst_lg_audio_render_prepare_seamless_seek> @ imm = #-0x7172
    e742: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    e746: f7f5 e984    	blx	0x3a50 <lgao_play@plt>  @ imm = #-0xacf8
    e74a: b1e0         	cbz	r0, 0xe786 <gst_lg_audio_render_change_state+0xfc6> @ imm = #0x38
    e74c: 6833         	ldr	r3, [r6]
    e74e: 2b01         	cmp	r3, #0x1
    e750: d919         	bls	0xe786 <gst_lg_audio_render_change_state+0xfc6> @ imm = #0x32
    e752: f8df 36b0    	ldr.w	r3, [pc, #0x6b0]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    e756: f04f 0c00    	mov.w	r12, #0x0
    e75a: f8df 26d4    	ldr.w	r2, [pc, #0x6d4]        @ 0xee30 <gst_lg_audio_render_change_state+0x1670>
    e75e: 2102         	movs	r1, #0x2
    e760: f858 3003    	ldr.w	r3, [r8, r3]
    e764: 447a         	add	r2, pc
    e766: 6818         	ldr	r0, [r3]
    e768: f8df 36c8    	ldr.w	r3, [pc, #0x6c8]        @ 0xee34 <gst_lg_audio_render_change_state+0x1674>
    e76c: 9202         	str	r2, [sp, #0x8]
    e76e: f640 32e5    	movw	r2, #0xbe5
    e772: e9cd 2c00    	strd	r2, r12, [sp]
    e776: f8df 26c0    	ldr.w	r2, [pc, #0x6c0]        @ 0xee38 <gst_lg_audio_render_change_state+0x1678>
    e77a: 447b         	add	r3, pc
    e77c: f503 7312    	add.w	r3, r3, #0x248
    e780: 447a         	add	r2, pc
    e782: f7f5 ea74    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xab18
    e786: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    e78a: f7f5 e872    	blx	0x3870 <lgao_start_renderer@plt> @ imm = #-0xaf1c
    e78e: b1d8         	cbz	r0, 0xe7c8 <gst_lg_audio_render_change_state+0x1008> @ imm = #0x36
    e790: 6833         	ldr	r3, [r6]
    e792: 2b01         	cmp	r3, #0x1
    e794: d918         	bls	0xe7c8 <gst_lg_audio_render_change_state+0x1008> @ imm = #0x30
    e796: f8df 366c    	ldr.w	r3, [pc, #0x66c]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    e79a: 2100         	movs	r1, #0x0
    e79c: f8df 269c    	ldr.w	r2, [pc, #0x69c]        @ 0xee3c <gst_lg_audio_render_change_state+0x167c>
    e7a0: f858 3003    	ldr.w	r3, [r8, r3]
    e7a4: 447a         	add	r2, pc
    e7a6: 6818         	ldr	r0, [r3]
    e7a8: f8df 3694    	ldr.w	r3, [pc, #0x694]        @ 0xee40 <gst_lg_audio_render_change_state+0x1680>
    e7ac: 9202         	str	r2, [sp, #0x8]
    e7ae: f640 32e6    	movw	r2, #0xbe6
    e7b2: e9cd 2100    	strd	r2, r1, [sp]
    e7b6: 2102         	movs	r1, #0x2
    e7b8: f8df 2688    	ldr.w	r2, [pc, #0x688]        @ 0xee44 <gst_lg_audio_render_change_state+0x1684>
    e7bc: 447b         	add	r3, pc
    e7be: f503 7312    	add.w	r3, r3, #0x248
    e7c2: 447a         	add	r2, pc
    e7c4: f7f5 ea52    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xab5c
    e7c8: 2304         	movs	r3, #0x4
    e7ca: f8c4 3218    	str.w	r3, [r4, #0x218]
    e7ce: f7ff b9c4    	b.w	0xdb5a <gst_lg_audio_render_change_state+0x39a> @ imm = #-0xc78
    e7d2: f8df 2674    	ldr.w	r2, [pc, #0x674]        @ 0xee48 <gst_lg_audio_render_change_state+0x1688>
    e7d6: 2102         	movs	r1, #0x2
    e7d8: f8df 3670    	ldr.w	r3, [pc, #0x670]        @ 0xee4c <gst_lg_audio_render_change_state+0x168c>
    e7dc: 447a         	add	r2, pc
    e7de: 447b         	add	r3, pc
    e7e0: f502 7212    	add.w	r2, r2, #0x248
    e7e4: 9300         	str	r3, [sp]
    e7e6: f640 4324    	movw	r3, #0xc24
    e7ea: f7f5 ebe0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xa840
    e7ee: f7ff b8cf    	b.w	0xd990 <gst_lg_audio_render_change_state+0x1d0> @ imm = #-0xe62
    e7f2: f8df 265c    	ldr.w	r2, [pc, #0x65c]        @ 0xee50 <gst_lg_audio_render_change_state+0x1690>
    e7f6: 2102         	movs	r1, #0x2
    e7f8: f8df 3658    	ldr.w	r3, [pc, #0x658]        @ 0xee54 <gst_lg_audio_render_change_state+0x1694>
    e7fc: 447a         	add	r2, pc
    e7fe: 447b         	add	r3, pc
    e800: f502 7212    	add.w	r2, r2, #0x248
    e804: 9300         	str	r3, [sp]
    e806: f640 33ba    	movw	r3, #0xbba
    e80a: f7f5 ebd0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xa860
    e80e: f7ff bae7    	b.w	0xdde0 <gst_lg_audio_render_change_state+0x620> @ imm = #-0xa32
    e812: 6813         	ldr	r3, [r2]
    e814: 0719         	lsls	r1, r3, #0x1c
    e816: d519         	bpl	0xe84c <gst_lg_audio_render_change_state+0x108c> @ imm = #0x32
    e818: f8d4 3310    	ldr.w	r3, [r4, #0x310]
    e81c: 2b00         	cmp	r3, #0x0
    e81e: f041 8213    	bne.w	0xfc48 <gst_lg_audio_render_change_state+0x2488> @ imm = #0x1426
    e822: f8df 3634    	ldr.w	r3, [pc, #0x634]        @ 0xee58 <gst_lg_audio_render_change_state+0x1698>
    e826: 447b         	add	r3, pc
    e828: f8df 2630    	ldr.w	r2, [pc, #0x630]        @ 0xee5c <gst_lg_audio_render_change_state+0x169c>
    e82c: 2103         	movs	r1, #0x3
    e82e: 9301         	str	r3, [sp, #0x4]
    e830: f8df 362c    	ldr.w	r3, [pc, #0x62c]        @ 0xee60 <gst_lg_audio_render_change_state+0x16a0>
    e834: f8df 062c    	ldr.w	r0, [pc, #0x62c]        @ 0xee64 <gst_lg_audio_render_change_state+0x16a4>
    e838: 447a         	add	r2, pc
    e83a: f502 721b    	add.w	r2, r2, #0x26c
    e83e: 447b         	add	r3, pc
    e840: 9300         	str	r3, [sp]
    e842: 4478         	add	r0, pc
    e844: f640 336f    	movw	r3, #0xb6f
    e848: f7f5 ebb0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xa8a0
    e84c: 6833         	ldr	r3, [r6]
    e84e: 2b03         	cmp	r3, #0x3
    e850: d920         	bls	0xe894 <gst_lg_audio_render_change_state+0x10d4> @ imm = #0x40
    e852: f8d4 3310    	ldr.w	r3, [r4, #0x310]
    e856: 2b00         	cmp	r3, #0x0
    e858: f041 81fa    	bne.w	0xfc50 <gst_lg_audio_render_change_state+0x2490> @ imm = #0x13f4
    e85c: f8df 3608    	ldr.w	r3, [pc, #0x608]        @ 0xee68 <gst_lg_audio_render_change_state+0x16a8>
    e860: 447b         	add	r3, pc
    e862: f8df 25a0    	ldr.w	r2, [pc, #0x5a0]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    e866: 2104         	movs	r1, #0x4
    e868: f858 2002    	ldr.w	r2, [r8, r2]
    e86c: 9303         	str	r3, [sp, #0xc]
    e86e: 9401         	str	r4, [sp, #0x4]
    e870: 6810         	ldr	r0, [r2]
    e872: f8df 25f8    	ldr.w	r2, [pc, #0x5f8]        @ 0xee6c <gst_lg_audio_render_change_state+0x16ac>
    e876: f8df 35f8    	ldr.w	r3, [pc, #0x5f8]        @ 0xee70 <gst_lg_audio_render_change_state+0x16b0>
    e87a: 447a         	add	r2, pc
    e87c: 9202         	str	r2, [sp, #0x8]
    e87e: f640 326f    	movw	r2, #0xb6f
    e882: 9200         	str	r2, [sp]
    e884: 447b         	add	r3, pc
    e886: f8df 25ec    	ldr.w	r2, [pc, #0x5ec]        @ 0xee74 <gst_lg_audio_render_change_state+0x16b4>
    e88a: f503 731b    	add.w	r3, r3, #0x26c
    e88e: 447a         	add	r2, pc
    e890: f7f5 e9ec    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xac28
    e894: f8d4 3310    	ldr.w	r3, [r4, #0x310]
    e898: 2b00         	cmp	r3, #0x0
    e89a: f43f aef3    	beq.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x21a
    e89e: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    e8a2: f7f5 ec70    	blx	0x4184 <lgao_set_vsp_mode@plt> @ imm = #-0xa720
    e8a6: e6ed         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x226
    e8a8: 6813         	ldr	r3, [r2]
    e8aa: 0718         	lsls	r0, r3, #0x1c
    e8ac: f101 809b    	bmi.w	0xf9e6 <gst_lg_audio_render_change_state+0x2226> @ imm = #0x1136
    e8b0: 6833         	ldr	r3, [r6]
    e8b2: 2b03         	cmp	r3, #0x3
    e8b4: d91a         	bls	0xe8ec <gst_lg_audio_render_change_state+0x112c> @ imm = #0x34
    e8b6: f8df 354c    	ldr.w	r3, [pc, #0x54c]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    e8ba: 2104         	movs	r1, #0x4
    e8bc: f8df 25b8    	ldr.w	r2, [pc, #0x5b8]        @ 0xee78 <gst_lg_audio_render_change_state+0x16b8>
    e8c0: f858 3003    	ldr.w	r3, [r8, r3]
    e8c4: 447a         	add	r2, pc
    e8c6: 6818         	ldr	r0, [r3]
    e8c8: f8d4 330c    	ldr.w	r3, [r4, #0x30c]
    e8cc: 9202         	str	r2, [sp, #0x8]
    e8ce: f640 3267    	movw	r2, #0xb67
    e8d2: 9200         	str	r2, [sp]
    e8d4: 9303         	str	r3, [sp, #0xc]
    e8d6: f8df 35a4    	ldr.w	r3, [pc, #0x5a4]        @ 0xee7c <gst_lg_audio_render_change_state+0x16bc>
    e8da: f8df 25a4    	ldr.w	r2, [pc, #0x5a4]        @ 0xee80 <gst_lg_audio_render_change_state+0x16c0>
    e8de: 9401         	str	r4, [sp, #0x4]
    e8e0: 447b         	add	r3, pc
    e8e2: f503 731b    	add.w	r3, r3, #0x26c
    e8e6: 447a         	add	r2, pc
    e8e8: f7f5 e9c0    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xac80
    e8ec: f8d4 130c    	ldr.w	r1, [r4, #0x30c]
    e8f0: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    e8f4: f7f5 ec76    	blx	0x41e4 <lgao_set_queue_max_length@plt> @ imm = #-0xa714
    e8f8: e6c4         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x278
    e8fa: 6813         	ldr	r3, [r2]
    e8fc: 071b         	lsls	r3, r3, #0x1c
    e8fe: f101 805f    	bmi.w	0xf9c0 <gst_lg_audio_render_change_state+0x2200> @ imm = #0x10be
    e902: 6833         	ldr	r3, [r6]
    e904: 2b03         	cmp	r3, #0x3
    e906: d91a         	bls	0xe93e <gst_lg_audio_render_change_state+0x117e> @ imm = #0x34
    e908: f8df 34f8    	ldr.w	r3, [pc, #0x4f8]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    e90c: 2104         	movs	r1, #0x4
    e90e: f8df 2574    	ldr.w	r2, [pc, #0x574]        @ 0xee84 <gst_lg_audio_render_change_state+0x16c4>
    e912: f858 3003    	ldr.w	r3, [r8, r3]
    e916: 447a         	add	r2, pc
    e918: 6818         	ldr	r0, [r3]
    e91a: f8d4 3304    	ldr.w	r3, [r4, #0x304]
    e91e: 9202         	str	r2, [sp, #0x8]
    e920: f44f 6236    	mov.w	r2, #0xb60
    e924: 9200         	str	r2, [sp]
    e926: 9303         	str	r3, [sp, #0xc]
    e928: f8df 355c    	ldr.w	r3, [pc, #0x55c]        @ 0xee88 <gst_lg_audio_render_change_state+0x16c8>
    e92c: f8df 255c    	ldr.w	r2, [pc, #0x55c]        @ 0xee8c <gst_lg_audio_render_change_state+0x16cc>
    e930: 9401         	str	r4, [sp, #0x4]
    e932: 447b         	add	r3, pc
    e934: f503 731b    	add.w	r3, r3, #0x26c
    e938: 447a         	add	r2, pc
    e93a: f7f5 e998    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xacd0
    e93e: f8d4 1304    	ldr.w	r1, [r4, #0x304]
    e942: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    e946: f7f5 ea88    	blx	0x3e58 <lgao_set_game_mode@plt> @ imm = #-0xaaf0
    e94a: e69b         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x2ca
    e94c: 6813         	ldr	r3, [r2]
    e94e: 075b         	lsls	r3, r3, #0x1d
    e950: f101 8023    	bmi.w	0xf99a <gst_lg_audio_render_change_state+0x21da> @ imm = #0x1046
    e954: 6833         	ldr	r3, [r6]
    e956: 2b03         	cmp	r3, #0x3
    e958: d91a         	bls	0xe990 <gst_lg_audio_render_change_state+0x11d0> @ imm = #0x34
    e95a: f8df 34a8    	ldr.w	r3, [pc, #0x4a8]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    e95e: 2104         	movs	r1, #0x4
    e960: f8df 252c    	ldr.w	r2, [pc, #0x52c]        @ 0xee90 <gst_lg_audio_render_change_state+0x16d0>
    e964: f858 3003    	ldr.w	r3, [r8, r3]
    e968: 447a         	add	r2, pc
    e96a: 6818         	ldr	r0, [r3]
    e96c: f8d4 3308    	ldr.w	r3, [r4, #0x308]
    e970: 9202         	str	r2, [sp, #0x8]
    e972: f640 22f8    	movw	r2, #0xaf8
    e976: 9200         	str	r2, [sp]
    e978: 9303         	str	r3, [sp, #0xc]
    e97a: f8df 3518    	ldr.w	r3, [pc, #0x518]        @ 0xee94 <gst_lg_audio_render_change_state+0x16d4>
    e97e: f8df 2518    	ldr.w	r2, [pc, #0x518]        @ 0xee98 <gst_lg_audio_render_change_state+0x16d8>
    e982: 9401         	str	r4, [sp, #0x4]
    e984: 447b         	add	r3, pc
    e986: f503 731b    	add.w	r3, r3, #0x26c
    e98a: 447a         	add	r2, pc
    e98c: f7f5 e96e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xad24
    e990: f8d4 2308    	ldr.w	r2, [r4, #0x308]
    e994: 2105         	movs	r1, #0x5
    e996: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    e99a: f7f5 e8c8    	blx	0x3b2c <lgao_set_rts@plt> @ imm = #-0xae70
    e99e: e671         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x31e
    e9a0: 6813         	ldr	r3, [r2]
    e9a2: 071a         	lsls	r2, r3, #0x1c
    e9a4: f100 87e9    	bmi.w	0xf97a <gst_lg_audio_render_change_state+0x21ba> @ imm = #0xfd2
    e9a8: 6833         	ldr	r3, [r6]
    e9aa: 2b03         	cmp	r3, #0x3
    e9ac: d917         	bls	0xe9de <gst_lg_audio_render_change_state+0x121e> @ imm = #0x2e
    e9ae: f8df 3454    	ldr.w	r3, [pc, #0x454]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    e9b2: 2104         	movs	r1, #0x4
    e9b4: f8df 24e4    	ldr.w	r2, [pc, #0x4e4]        @ 0xee9c <gst_lg_audio_render_change_state+0x16dc>
    e9b8: f858 3003    	ldr.w	r3, [r8, r3]
    e9bc: 447a         	add	r2, pc
    e9be: 9401         	str	r4, [sp, #0x4]
    e9c0: 6818         	ldr	r0, [r3]
    e9c2: f8df 34dc    	ldr.w	r3, [pc, #0x4dc]        @ 0xeea0 <gst_lg_audio_render_change_state+0x16e0>
    e9c6: 9202         	str	r2, [sp, #0x8]
    e9c8: f640 3259    	movw	r2, #0xb59
    e9cc: 9200         	str	r2, [sp]
    e9ce: f8df 24d4    	ldr.w	r2, [pc, #0x4d4]        @ 0xeea4 <gst_lg_audio_render_change_state+0x16e4>
    e9d2: 447b         	add	r3, pc
    e9d4: f503 731b    	add.w	r3, r3, #0x26c
    e9d8: 447a         	add	r2, pc
    e9da: f7f5 e948    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xad70
    e9de: f8d4 1300    	ldr.w	r1, [r4, #0x300]
    e9e2: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    e9e6: f7f4 ef50    	blx	0x3888 <lgao_set_pcm_multi_channel@plt> @ imm = #-0xb160
    e9ea: e64b         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x36a
    e9ec: 6813         	ldr	r3, [r2]
    e9ee: 0719         	lsls	r1, r3, #0x1c
    e9f0: f100 87b1    	bmi.w	0xf956 <gst_lg_audio_render_change_state+0x2196> @ imm = #0xf62
    e9f4: 6833         	ldr	r3, [r6]
    e9f6: 2b03         	cmp	r3, #0x3
    e9f8: d917         	bls	0xea2a <gst_lg_audio_render_change_state+0x126a> @ imm = #0x2e
    e9fa: f8df 3408    	ldr.w	r3, [pc, #0x408]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    e9fe: 2104         	movs	r1, #0x4
    ea00: f8df 24a4    	ldr.w	r2, [pc, #0x4a4]        @ 0xeea8 <gst_lg_audio_render_change_state+0x16e8>
    ea04: f858 3003    	ldr.w	r3, [r8, r3]
    ea08: 447a         	add	r2, pc
    ea0a: 9401         	str	r4, [sp, #0x4]
    ea0c: 6818         	ldr	r0, [r3]
    ea0e: f8df 349c    	ldr.w	r3, [pc, #0x49c]        @ 0xeeac <gst_lg_audio_render_change_state+0x16ec>
    ea12: 9202         	str	r2, [sp, #0x8]
    ea14: f640 3252    	movw	r2, #0xb52
    ea18: 9200         	str	r2, [sp]
    ea1a: f8df 2494    	ldr.w	r2, [pc, #0x494]        @ 0xeeb0 <gst_lg_audio_render_change_state+0x16f0>
    ea1e: 447b         	add	r3, pc
    ea20: f503 731b    	add.w	r3, r3, #0x26c
    ea24: 447a         	add	r2, pc
    ea26: f7f5 e922    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xadbc
    ea2a: f8d4 12fc    	ldr.w	r1, [r4, #0x2fc]
    ea2e: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    ea32: f7f4 eee2    	blx	0x37f8 <lgao_set_pcm_bypass@plt> @ imm = #-0xb23c
    ea36: e625         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x3b6
    ea38: 6813         	ldr	r3, [r2]
    ea3a: 0759         	lsls	r1, r3, #0x1d
    ea3c: f100 8775    	bmi.w	0xf92a <gst_lg_audio_render_change_state+0x216a> @ imm = #0xeea
    ea40: 6833         	ldr	r3, [r6]
    ea42: 2b03         	cmp	r3, #0x3
    ea44: d919         	bls	0xea7a <gst_lg_audio_render_change_state+0x12ba> @ imm = #0x32
    ea46: 4bef         	ldr	r3, [pc, #0x3bc]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    ea48: 2104         	movs	r1, #0x4
    ea4a: f8df 2468    	ldr.w	r2, [pc, #0x468]        @ 0xeeb4 <gst_lg_audio_render_change_state+0x16f4>
    ea4e: f858 3003    	ldr.w	r3, [r8, r3]
    ea52: 447a         	add	r2, pc
    ea54: 6818         	ldr	r0, [r3]
    ea56: f8d4 32b4    	ldr.w	r3, [r4, #0x2b4]
    ea5a: 9202         	str	r2, [sp, #0x8]
    ea5c: f640 22ec    	movw	r2, #0xaec
    ea60: 9200         	str	r2, [sp]
    ea62: 9303         	str	r3, [sp, #0xc]
    ea64: f8df 3450    	ldr.w	r3, [pc, #0x450]        @ 0xeeb8 <gst_lg_audio_render_change_state+0x16f8>
    ea68: f8df 2450    	ldr.w	r2, [pc, #0x450]        @ 0xeebc <gst_lg_audio_render_change_state+0x16fc>
    ea6c: 9401         	str	r4, [sp, #0x4]
    ea6e: 447b         	add	r3, pc
    ea70: f503 731b    	add.w	r3, r3, #0x26c
    ea74: 447a         	add	r2, pc
    ea76: f7f5 e8fa    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xae0c
    ea7a: f8d4 22b4    	ldr.w	r2, [r4, #0x2b4]
    ea7e: 2103         	movs	r1, #0x3
    ea80: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    ea84: f7f5 e852    	blx	0x3b2c <lgao_set_rts@plt> @ imm = #-0xaf5c
    ea88: e5fc         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x408
    ea8a: 6813         	ldr	r3, [r2]
    ea8c: 075b         	lsls	r3, r3, #0x1d
    ea8e: f101 806e    	bmi.w	0xfb6e <gst_lg_audio_render_change_state+0x23ae> @ imm = #0x10dc
    ea92: 6833         	ldr	r3, [r6]
    ea94: 2b03         	cmp	r3, #0x3
    ea96: d919         	bls	0xeacc <gst_lg_audio_render_change_state+0x130c> @ imm = #0x32
    ea98: 4bda         	ldr	r3, [pc, #0x368]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    ea9a: 2104         	movs	r1, #0x4
    ea9c: f8df 2420    	ldr.w	r2, [pc, #0x420]        @ 0xeec0 <gst_lg_audio_render_change_state+0x1700>
    eaa0: f858 3003    	ldr.w	r3, [r8, r3]
    eaa4: 447a         	add	r2, pc
    eaa6: 6818         	ldr	r0, [r3]
    eaa8: f8d4 32bc    	ldr.w	r3, [r4, #0x2bc]
    eaac: 9202         	str	r2, [sp, #0x8]
    eaae: f640 3234    	movw	r2, #0xb34
    eab2: 9200         	str	r2, [sp]
    eab4: 9303         	str	r3, [sp, #0xc]
    eab6: f8df 340c    	ldr.w	r3, [pc, #0x40c]        @ 0xeec4 <gst_lg_audio_render_change_state+0x1704>
    eaba: f8df 240c    	ldr.w	r2, [pc, #0x40c]        @ 0xeec8 <gst_lg_audio_render_change_state+0x1708>
    eabe: 9401         	str	r4, [sp, #0x4]
    eac0: 447b         	add	r3, pc
    eac2: f503 731b    	add.w	r3, r3, #0x26c
    eac6: 447a         	add	r2, pc
    eac8: f7f5 e8d0    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xae60
    eacc: f8d4 12bc    	ldr.w	r1, [r4, #0x2bc]
    ead0: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    ead4: f7f4 ef56    	blx	0x3984 <lgao_set_mode_mixer@plt> @ imm = #-0xb154
    ead8: e5d4         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x458
    eada: 6813         	ldr	r3, [r2]
    eadc: 0759         	lsls	r1, r3, #0x1d
    eade: f101 8033    	bmi.w	0xfb48 <gst_lg_audio_render_change_state+0x2388> @ imm = #0x1066
    eae2: 6833         	ldr	r3, [r6]
    eae4: 2b03         	cmp	r3, #0x3
    eae6: d916         	bls	0xeb16 <gst_lg_audio_render_change_state+0x1356> @ imm = #0x2c
    eae8: 4bc6         	ldr	r3, [pc, #0x318]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    eaea: 2104         	movs	r1, #0x4
    eaec: 4af7         	ldr	r2, [pc, #0x3dc]        @ 0xeecc <gst_lg_audio_render_change_state+0x170c>
    eaee: f858 3003    	ldr.w	r3, [r8, r3]
    eaf2: 447a         	add	r2, pc
    eaf4: 6818         	ldr	r0, [r3]
    eaf6: f8d4 328c    	ldr.w	r3, [r4, #0x28c]
    eafa: 9202         	str	r2, [sp, #0x8]
    eafc: f640 22d5    	movw	r2, #0xad5
    eb00: 9200         	str	r2, [sp]
    eb02: 9303         	str	r3, [sp, #0xc]
    eb04: 4bf2         	ldr	r3, [pc, #0x3c8]        @ 0xeed0 <gst_lg_audio_render_change_state+0x1710>
    eb06: 4af3         	ldr	r2, [pc, #0x3cc]        @ 0xeed4 <gst_lg_audio_render_change_state+0x1714>
    eb08: 9401         	str	r4, [sp, #0x4]
    eb0a: 447b         	add	r3, pc
    eb0c: f503 731b    	add.w	r3, r3, #0x26c
    eb10: 447a         	add	r2, pc
    eb12: f7f5 e8ac    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xaea8
    eb16: f8d4 128c    	ldr.w	r1, [r4, #0x28c]
    eb1a: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    eb1e: f7f5 e894    	blx	0x3c48 <lgao_set_nodelay@plt> @ imm = #-0xaed8
    eb22: e5af         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x4a2
    eb24: 6813         	ldr	r3, [r2]
    eb26: 0759         	lsls	r1, r3, #0x1d
    eb28: f100 87fb    	bmi.w	0xfb22 <gst_lg_audio_render_change_state+0x2362> @ imm = #0xff6
    eb2c: 6833         	ldr	r3, [r6]
    eb2e: 2b03         	cmp	r3, #0x3
    eb30: d916         	bls	0xeb60 <gst_lg_audio_render_change_state+0x13a0> @ imm = #0x2c
    eb32: 4bb4         	ldr	r3, [pc, #0x2d0]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    eb34: 2104         	movs	r1, #0x4
    eb36: 4ae8         	ldr	r2, [pc, #0x3a0]        @ 0xeed8 <gst_lg_audio_render_change_state+0x1718>
    eb38: f858 3003    	ldr.w	r3, [r8, r3]
    eb3c: 447a         	add	r2, pc
    eb3e: 6818         	ldr	r0, [r3]
    eb40: f8d4 32c0    	ldr.w	r3, [r4, #0x2c0]
    eb44: 9202         	str	r2, [sp, #0x8]
    eb46: f640 323f    	movw	r2, #0xb3f
    eb4a: 9200         	str	r2, [sp]
    eb4c: 9303         	str	r3, [sp, #0xc]
    eb4e: 4be3         	ldr	r3, [pc, #0x38c]        @ 0xeedc <gst_lg_audio_render_change_state+0x171c>
    eb50: 4ae3         	ldr	r2, [pc, #0x38c]        @ 0xeee0 <gst_lg_audio_render_change_state+0x1720>
    eb52: 9401         	str	r4, [sp, #0x4]
    eb54: 447b         	add	r3, pc
    eb56: f503 731b    	add.w	r3, r3, #0x26c
    eb5a: 447a         	add	r2, pc
    eb5c: f7f5 e886    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xaef4
    eb60: f8d4 12c0    	ldr.w	r1, [r4, #0x2c0]
    eb64: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    eb68: f7f5 eb72    	blx	0x4250 <lgao_set_mute@plt> @ imm = #-0xa91c
    eb6c: e58a         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x4ec
    eb6e: 6813         	ldr	r3, [r2]
    eb70: 075b         	lsls	r3, r3, #0x1d
    eb72: f100 87c3    	bmi.w	0xfafc <gst_lg_audio_render_change_state+0x233c> @ imm = #0xf86
    eb76: 6833         	ldr	r3, [r6]
    eb78: 2b03         	cmp	r3, #0x3
    eb7a: d916         	bls	0xebaa <gst_lg_audio_render_change_state+0x13ea> @ imm = #0x2c
    eb7c: 4ba1         	ldr	r3, [pc, #0x284]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    eb7e: 2104         	movs	r1, #0x4
    eb80: 4ad8         	ldr	r2, [pc, #0x360]        @ 0xeee4 <gst_lg_audio_render_change_state+0x1724>
    eb82: f858 3003    	ldr.w	r3, [r8, r3]
    eb86: 447a         	add	r2, pc
    eb88: 6818         	ldr	r0, [r3]
    eb8a: f8d4 32ac    	ldr.w	r3, [r4, #0x2ac]
    eb8e: 9202         	str	r2, [sp, #0x8]
    eb90: f44f 622e    	mov.w	r2, #0xae0
    eb94: 9200         	str	r2, [sp]
    eb96: 9303         	str	r3, [sp, #0xc]
    eb98: 4bd3         	ldr	r3, [pc, #0x34c]        @ 0xeee8 <gst_lg_audio_render_change_state+0x1728>
    eb9a: 4ad4         	ldr	r2, [pc, #0x350]        @ 0xeeec <gst_lg_audio_render_change_state+0x172c>
    eb9c: 9401         	str	r4, [sp, #0x4]
    eb9e: 447b         	add	r3, pc
    eba0: f503 731b    	add.w	r3, r3, #0x26c
    eba4: 447a         	add	r2, pc
    eba6: f7f5 e862    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xaf3c
    ebaa: f8d4 22ac    	ldr.w	r2, [r4, #0x2ac]
    ebae: 2101         	movs	r1, #0x1
    ebb0: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    ebb4: f7f4 efba    	blx	0x3b2c <lgao_set_rts@plt> @ imm = #-0xb08c
    ebb8: e564         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x538
    ebba: 6813         	ldr	r3, [r2]
    ebbc: 0758         	lsls	r0, r3, #0x1d
    ebbe: f101 8022    	bmi.w	0xfc06 <gst_lg_audio_render_change_state+0x2446> @ imm = #0x1044
    ebc2: 6833         	ldr	r3, [r6]
    ebc4: 2b03         	cmp	r3, #0x3
    ebc6: f200 85a5    	bhi.w	0xf714 <gst_lg_audio_render_change_state+0x1f54> @ imm = #0xb4a
    ebca: f8d4 a224    	ldr.w	r10, [r4, #0x224]
    ebce: f1ba 0f00    	cmp.w	r10, #0x0
    ebd2: f43f ad57    	beq.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x552
    ebd6: 49c6         	ldr	r1, [pc, #0x318]        @ 0xeef0 <gst_lg_audio_render_change_state+0x1730>
    ebd8: 4650         	mov	r0, r10
    ebda: 4479         	add	r1, pc
    ebdc: f7f5 eb5c    	blx	0x4298 <strcmp@plt>     @ imm = #-0xa948
    ebe0: 2800         	cmp	r0, #0x0
    ebe2: f000 8642    	beq.w	0xf86a <gst_lg_audio_render_change_state+0x20aa> @ imm = #0xc84
    ebe6: 49c3         	ldr	r1, [pc, #0x30c]        @ 0xeef4 <gst_lg_audio_render_change_state+0x1734>
    ebe8: 4650         	mov	r0, r10
    ebea: 4479         	add	r1, pc
    ebec: f7f5 eb54    	blx	0x4298 <strcmp@plt>     @ imm = #-0xa958
    ebf0: 2800         	cmp	r0, #0x0
    ebf2: f041 812b    	bne.w	0xfe4c <gst_lg_audio_render_change_state+0x268c> @ imm = #0x1256
    ebf6: 48c0         	ldr	r0, [pc, #0x300]        @ 0xeef8 <gst_lg_audio_render_change_state+0x1738>
    ebf8: 4478         	add	r0, pc
    ebfa: 68c3         	ldr	r3, [r0, #0xc]
    ebfc: 681b         	ldr	r3, [r3]
    ebfe: 071a         	lsls	r2, r3, #0x1c
    ec00: f101 81dc    	bmi.w	0xffbc <gst_lg_audio_render_change_state+0x27fc> @ imm = #0x13b8
    ec04: 6833         	ldr	r3, [r6]
    ec06: 2b03         	cmp	r3, #0x3
    ec08: d913         	bls	0xec32 <gst_lg_audio_render_change_state+0x1472> @ imm = #0x26
    ec0a: 4b7e         	ldr	r3, [pc, #0x1f8]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    ec0c: 2104         	movs	r1, #0x4
    ec0e: 4abb         	ldr	r2, [pc, #0x2ec]        @ 0xeefc <gst_lg_audio_render_change_state+0x173c>
    ec10: f858 3003    	ldr.w	r3, [r8, r3]
    ec14: 447a         	add	r2, pc
    ec16: 9401         	str	r4, [sp, #0x4]
    ec18: 6818         	ldr	r0, [r3]
    ec1a: 4bb9         	ldr	r3, [pc, #0x2e4]        @ 0xef00 <gst_lg_audio_render_change_state+0x1740>
    ec1c: 9202         	str	r2, [sp, #0x8]
    ec1e: f640 3207    	movw	r2, #0xb07
    ec22: 9200         	str	r2, [sp]
    ec24: 4ab7         	ldr	r2, [pc, #0x2dc]        @ 0xef04 <gst_lg_audio_render_change_state+0x1744>
    ec26: 447b         	add	r3, pc
    ec28: f503 731b    	add.w	r3, r3, #0x26c
    ec2c: 447a         	add	r2, pc
    ec2e: f7f5 e81e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xafc4
    ec32: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    ec36: 2102         	movs	r1, #0x2
    ec38: f7f5 eaf2    	blx	0x4220 <lgao_set_apptype@plt> @ imm = #-0xaa1c
    ec3c: 2101         	movs	r1, #0x1
    ec3e: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    ec42: f7f5 e802    	blx	0x3c48 <lgao_set_nodelay@plt> @ imm = #-0xaffc
    ec46: f7f5 ea2c    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xaba8
    ec4a: 4601         	mov	r1, r0
    ec4c: 4620         	mov	r0, r4
    ec4e: f7f5 e99c    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xacc8
    ec52: 2100         	movs	r1, #0x0
    ec54: f7f5 eb3a    	blx	0x42cc <gst_base_sink_set_sync@plt> @ imm = #-0xa98c
    ec58: e514         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x5d8
    ec5a: 6813         	ldr	r3, [r2]
    ec5c: 075b         	lsls	r3, r3, #0x1d
    ec5e: f100 87bf    	bmi.w	0xfbe0 <gst_lg_audio_render_change_state+0x2420> @ imm = #0xf7e
    ec62: 6833         	ldr	r3, [r6]
    ec64: 2b03         	cmp	r3, #0x3
    ec66: d916         	bls	0xec96 <gst_lg_audio_render_change_state+0x14d6> @ imm = #0x2c
    ec68: 4b66         	ldr	r3, [pc, #0x198]        @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    ec6a: 2104         	movs	r1, #0x4
    ec6c: 4aa6         	ldr	r2, [pc, #0x298]        @ 0xef08 <gst_lg_audio_render_change_state+0x1748>
    ec6e: f858 3003    	ldr.w	r3, [r8, r3]
    ec72: 447a         	add	r2, pc
    ec74: 6818         	ldr	r0, [r3]
    ec76: f8d4 3284    	ldr.w	r3, [r4, #0x284]
    ec7a: 9202         	str	r2, [sp, #0x8]
    ec7c: f640 22c8    	movw	r2, #0xac8
    ec80: 9200         	str	r2, [sp]
    ec82: 9303         	str	r3, [sp, #0xc]
    ec84: 4ba1         	ldr	r3, [pc, #0x284]        @ 0xef0c <gst_lg_audio_render_change_state+0x174c>
    ec86: 4aa2         	ldr	r2, [pc, #0x288]        @ 0xef10 <gst_lg_audio_render_change_state+0x1750>
    ec88: 9401         	str	r4, [sp, #0x4]
    ec8a: 447b         	add	r3, pc
    ec8c: f503 731b    	add.w	r3, r3, #0x26c
    ec90: 447a         	add	r2, pc
    ec92: f7f4 efec    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xb028
    ec96: f8d4 1284    	ldr.w	r1, [r4, #0x284]
    ec9a: f06f 4370    	mvn	r3, #0xf0000000
    ec9e: 4299         	cmp	r1, r3
    eca0: f43f acf0    	beq.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x620
    eca4: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    eca8: f7f4 eefc    	blx	0x3aa4 <lgao_set_lipsync_offset@plt> @ imm = #-0xb208
    ecac: e4ea         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x62c
    ecae: 6813         	ldr	r3, [r2]
    ecb0: 075a         	lsls	r2, r3, #0x1d
    ecb2: f100 8782    	bmi.w	0xfbba <gst_lg_audio_render_change_state+0x23fa> @ imm = #0xf04
    ecb6: 6833         	ldr	r3, [r6]
    ecb8: 2b03         	cmp	r3, #0x3
    ecba: f200 850e    	bhi.w	0xf6da <gst_lg_audio_render_change_state+0x1f1a> @ imm = #0xa1c
    ecbe: f8d4 a2c4    	ldr.w	r10, [r4, #0x2c4]
    ecc2: f1ba 0f00    	cmp.w	r10, #0x0
    ecc6: f43f acdd    	beq.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x646
    ecca: 4992         	ldr	r1, [pc, #0x248]        @ 0xef14 <gst_lg_audio_render_change_state+0x1754>
    eccc: 4650         	mov	r0, r10
    ecce: 4479         	add	r1, pc
    ecd0: f7f5 eae2    	blx	0x4298 <strcmp@plt>     @ imm = #-0xaa3c
    ecd4: 2800         	cmp	r0, #0x0
    ecd6: f000 820a    	beq.w	0xf0ee <gst_lg_audio_render_change_state+0x192e> @ imm = #0x414
    ecda: 498f         	ldr	r1, [pc, #0x23c]        @ 0xef18 <gst_lg_audio_render_change_state+0x1758>
    ecdc: 4650         	mov	r0, r10
    ecde: 4479         	add	r1, pc
    ece0: f7f5 eada    	blx	0x4298 <strcmp@plt>     @ imm = #-0xaa4c
    ece4: 2800         	cmp	r0, #0x0
    ece6: f000 8202    	beq.w	0xf0ee <gst_lg_audio_render_change_state+0x192e> @ imm = #0x404
    ecea: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    ecee: 2100         	movs	r1, #0x0
    ecf0: f7f4 ee24    	blx	0x393c <lgao_set_clocktype@plt> @ imm = #-0xb3b8
    ecf4: e4c6         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x674
    ecf6: 6813         	ldr	r3, [r2]
    ecf8: 0758         	lsls	r0, r3, #0x1d
    ecfa: f100 874b    	bmi.w	0xfb94 <gst_lg_audio_render_change_state+0x23d4> @ imm = #0xe96
    ecfe: 6833         	ldr	r3, [r6]
    ed00: 2b03         	cmp	r3, #0x3
    ed02: d916         	bls	0xed32 <gst_lg_audio_render_change_state+0x1572> @ imm = #0x2c
    ed04: 4b3f         	ldr	r3, [pc, #0xfc]         @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    ed06: 2104         	movs	r1, #0x4
    ed08: 4a84         	ldr	r2, [pc, #0x210]        @ 0xef1c <gst_lg_audio_render_change_state+0x175c>
    ed0a: f858 3003    	ldr.w	r3, [r8, r3]
    ed0e: 447a         	add	r2, pc
    ed10: 6818         	ldr	r0, [r3]
    ed12: f8d4 32b0    	ldr.w	r3, [r4, #0x2b0]
    ed16: 9202         	str	r2, [sp, #0x8]
    ed18: f640 22e6    	movw	r2, #0xae6
    ed1c: 9200         	str	r2, [sp]
    ed1e: 9303         	str	r3, [sp, #0xc]
    ed20: 4b7f         	ldr	r3, [pc, #0x1fc]        @ 0xef20 <gst_lg_audio_render_change_state+0x1760>
    ed22: 4a80         	ldr	r2, [pc, #0x200]        @ 0xef24 <gst_lg_audio_render_change_state+0x1764>
    ed24: 9401         	str	r4, [sp, #0x4]
    ed26: 447b         	add	r3, pc
    ed28: f503 731b    	add.w	r3, r3, #0x26c
    ed2c: 447a         	add	r2, pc
    ed2e: f7f4 ef9e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xb0c4
    ed32: f8d4 22b0    	ldr.w	r2, [r4, #0x2b0]
    ed36: 2102         	movs	r1, #0x2
    ed38: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    ed3c: f7f4 eef6    	blx	0x3b2c <lgao_set_rts@plt> @ imm = #-0xb214
    ed40: e4a0         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x6c0
    ed42: 6813         	ldr	r3, [r2]
    ed44: 075a         	lsls	r2, r3, #0x1d
    ed46: f100 86c3    	bmi.w	0xfad0 <gst_lg_audio_render_change_state+0x2310> @ imm = #0xd86
    ed4a: 6833         	ldr	r3, [r6]
    ed4c: 2b03         	cmp	r3, #0x3
    ed4e: d919         	bls	0xed84 <gst_lg_audio_render_change_state+0x15c4> @ imm = #0x32
    ed50: 4b2c         	ldr	r3, [pc, #0xb0]         @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    ed52: 2104         	movs	r1, #0x4
    ed54: f858 3003    	ldr.w	r3, [r8, r3]
    ed58: 6818         	ldr	r0, [r3]
    ed5a: f8d4 327c    	ldr.w	r3, [r4, #0x27c]
    ed5e: 9304         	str	r3, [sp, #0x10]
    ed60: f8d4 2274    	ldr.w	r2, [r4, #0x274]
    ed64: 4b70         	ldr	r3, [pc, #0x1c0]        @ 0xef28 <gst_lg_audio_render_change_state+0x1768>
    ed66: 9401         	str	r4, [sp, #0x4]
    ed68: 9203         	str	r2, [sp, #0xc]
    ed6a: 4a70         	ldr	r2, [pc, #0x1c0]        @ 0xef2c <gst_lg_audio_render_change_state+0x176c>
    ed6c: 447b         	add	r3, pc
    ed6e: f503 731b    	add.w	r3, r3, #0x26c
    ed72: 447a         	add	r2, pc
    ed74: 9202         	str	r2, [sp, #0x8]
    ed76: f640 322e    	movw	r2, #0xb2e
    ed7a: 9200         	str	r2, [sp]
    ed7c: 4a6c         	ldr	r2, [pc, #0x1b0]        @ 0xef30 <gst_lg_audio_render_change_state+0x1770>
    ed7e: 447a         	add	r2, pc
    ed80: f7f4 ef74    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xb118
    ed84: f8d4 227c    	ldr.w	r2, [r4, #0x27c]
    ed88: f8d4 1274    	ldr.w	r1, [r4, #0x274]
    ed8c: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    ed90: f7f5 e9ce    	blx	0x4130 <lgao_set_port@plt> @ imm = #-0xac64
    ed94: e476         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x714
    ed96: 6813         	ldr	r3, [r2]
    ed98: 0758         	lsls	r0, r3, #0x1d
    ed9a: f100 8686    	bmi.w	0xfaaa <gst_lg_audio_render_change_state+0x22ea> @ imm = #0xd0c
    ed9e: 6833         	ldr	r3, [r6]
    eda0: 2b03         	cmp	r3, #0x3
    eda2: d916         	bls	0xedd2 <gst_lg_audio_render_change_state+0x1612> @ imm = #0x2c
    eda4: 4b17         	ldr	r3, [pc, #0x5c]         @ 0xee04 <gst_lg_audio_render_change_state+0x1644>
    eda6: 2104         	movs	r1, #0x4
    eda8: 4a62         	ldr	r2, [pc, #0x188]        @ 0xef34 <gst_lg_audio_render_change_state+0x1774>
    edaa: f858 3003    	ldr.w	r3, [r8, r3]
    edae: 447a         	add	r2, pc
    edb0: 6818         	ldr	r0, [r3]
    edb2: f8d4 3290    	ldr.w	r3, [r4, #0x290]
    edb6: 9202         	str	r2, [sp, #0x8]
    edb8: f640 22ce    	movw	r2, #0xace
    edbc: 9200         	str	r2, [sp]
    edbe: 9303         	str	r3, [sp, #0xc]
    edc0: 4b5d         	ldr	r3, [pc, #0x174]        @ 0xef38 <gst_lg_audio_render_change_state+0x1778>
    edc2: 4a5e         	ldr	r2, [pc, #0x178]        @ 0xef3c <gst_lg_audio_render_change_state+0x177c>
    edc4: 9401         	str	r4, [sp, #0x4]
    edc6: 447b         	add	r3, pc
    edc8: f503 731b    	add.w	r3, r3, #0x26c
    edcc: 447a         	add	r2, pc
    edce: f7f4 ef4e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xb164
    edd2: f8d4 3290    	ldr.w	r3, [r4, #0x290]
    edd6: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    edda: 2b00         	cmp	r3, #0x0
    eddc: bf14         	ite	ne
    edde: 2101         	movne	r1, #0x1
    ede0: 2102         	moveq	r1, #0x2
    ede2: f7f4 efe0    	blx	0x3da4 <lgao_set_lipsync_mode@plt> @ imm = #-0xb040
    ede6: e44d         	b	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x766
    ede8: 00 ca 9a 3b  	.word	0x3b9aca00
    edec: 00 00 00 00  	.word	0x00000000
    edf0: 00 58 47 f8  	.word	0xf8475800
    edf4: 0d 00 00 00  	.word	0x0000000d
    edf8: 7e 19 01 00  	.word	0x0001197e
    edfc: da 3d 02 00  	.word	0x00023dda
    ee00: c2 f7 00 00  	.word	0x0000f7c2
    ee04: 08 04 00 00  	.word	0x00000408
    ee08: a2 f4 00 00  	.word	0x0000f4a2
    ee0c: 54 15 01 00  	.word	0x00011554
    ee10: f2 b2 00 00  	.word	0x0000b2f2
    ee14: d0 c6 00 00  	.word	0x0000c6d0
    ee18: c6 c6 00 00  	.word	0x0000c6c6
    ee1c: 5c 39 02 00  	.word	0x0002395c
    ee20: 90 f5 00 00  	.word	0x0000f590
    ee24: ba 14 01 00  	.word	0x000114ba
    ee28: 58 b2 00 00  	.word	0x0000b258
    ee2c: c4 39 02 00  	.word	0x000239c4
    ee30: cc da 00 00  	.word	0x0000dacc
    ee34: 1a 14 01 00  	.word	0x0001141a
    ee38: b8 b1 00 00  	.word	0x0000b1b8
    ee3c: b0 da 00 00  	.word	0x0000dab0
    ee40: d8 13 01 00  	.word	0x000113d8
    ee44: 76 b1 00 00  	.word	0x0000b176
    ee48: b8 13 01 00  	.word	0x000113b8
    ee4c: 9e f4 00 00  	.word	0x0000f49e
    ee50: 98 13 01 00  	.word	0x00011398
    ee54: ee f1 00 00  	.word	0x0000f1ee
    ee58: 66 e6 00 00  	.word	0x0000e666
    ee5c: 5c 13 01 00  	.word	0x0001135c
    ee60: ca f0 00 00  	.word	0x0000f0ca
    ee64: be 37 02 00  	.word	0x000237be
    ee68: 2c e6 00 00  	.word	0x0000e62c
    ee6c: da f0 00 00  	.word	0x0000f0da
    ee70: 10 13 01 00  	.word	0x00011310
    ee74: aa b0 00 00  	.word	0x0000b0aa
    ee78: 20 f0 00 00  	.word	0x0000f020
    ee7c: b4 12 01 00  	.word	0x000112b4
    ee80: 52 b0 00 00  	.word	0x0000b052
    ee84: 7a ef 00 00  	.word	0x0000ef7a
    ee88: 62 12 01 00  	.word	0x00011262
    ee8c: 00 b0 00 00  	.word	0x0000b000
    ee90: 00 e9 00 00  	.word	0x0000e900
    ee94: 10 12 01 00  	.word	0x00011210
    ee98: ae af 00 00  	.word	0x0000afae
    ee9c: 74 ee 00 00  	.word	0x0000ee74
    eea0: c2 11 01 00  	.word	0x000111c2
    eea4: 60 af 00 00  	.word	0x0000af60
    eea8: a0 ed 00 00  	.word	0x0000eda0
    eeac: 76 11 01 00  	.word	0x00011176
    eeb0: 14 af 00 00  	.word	0x0000af14
    eeb4: 5e e7 00 00  	.word	0x0000e75e
    eeb8: 26 11 01 00  	.word	0x00011126
    eebc: c4 ae 00 00  	.word	0x0000aec4
    eec0: 80 eb 00 00  	.word	0x0000eb80
    eec4: d4 10 01 00  	.word	0x000110d4
    eec8: 72 ae 00 00  	.word	0x0000ae72
    eecc: 7a e5 00 00  	.word	0x0000e57a
    eed0: 8a 10 01 00  	.word	0x0001108a
    eed4: 28 ae 00 00  	.word	0x0000ae28
    eed8: 80 eb 00 00  	.word	0x0000eb80
    eedc: 40 10 01 00  	.word	0x00011040
    eee0: de ad 00 00  	.word	0x0000adde
    eee4: 82 e5 00 00  	.word	0x0000e582
    eee8: f6 0f 01 00  	.word	0x00010ff6
    eeec: 94 ad 00 00  	.word	0x0000ad94
    eef0: d6 c5 00 00  	.word	0x0000c5d6
    eef4: 4e e7 00 00  	.word	0x0000e74e
    eef8: 08 34 02 00  	.word	0x00023408
    eefc: 50 e7 00 00  	.word	0x0000e750
    ef00: 6e 0f 01 00  	.word	0x00010f6e
    ef04: 0c ad 00 00  	.word	0x0000ad0c
    ef08: 6a e3 00 00  	.word	0x0000e36a
    ef0c: 0a 0f 01 00  	.word	0x00010f0a
    ef10: a8 ac 00 00  	.word	0x0000aca8
    ef14: be b1 00 00  	.word	0x0000b1be
    ef18: 42 ea 00 00  	.word	0x0000ea42
    ef1c: 4e e4 00 00  	.word	0x0000e44e
    ef20: 6e 0e 01 00  	.word	0x00010e6e
    ef24: 0c ac 00 00  	.word	0x0000ac0c
    ef28: 28 0e 01 00  	.word	0x00010e28
    ef2c: 5e e8 00 00  	.word	0x0000e85e
    ef30: ba ab 00 00  	.word	0x0000abba
    ef34: 7a e2 00 00  	.word	0x0000e27a
    ef38: ce 0d 01 00  	.word	0x00010dce
    ef3c: 6c ab 00 00  	.word	0x0000ab6c
    ef40: 6813         	ldr	r3, [r2]
    ef42: 0758         	lsls	r0, r3, #0x1d
    ef44: f100 859b    	bmi.w	0xfa7e <gst_lg_audio_render_change_state+0x22be> @ imm = #0xb36
    ef48: 6833         	ldr	r3, [r6]
    ef4a: 2b03         	cmp	r3, #0x3
    ef4c: f67f af1a    	bls.w	0xed84 <gst_lg_audio_render_change_state+0x15c4> @ imm = #-0x1cc
    ef50: f8df 3d14    	ldr.w	r3, [pc, #0xd14]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    ef54: 2104         	movs	r1, #0x4
    ef56: f858 3003    	ldr.w	r3, [r8, r3]
    ef5a: 6818         	ldr	r0, [r3]
    ef5c: f8d4 327c    	ldr.w	r3, [r4, #0x27c]
    ef60: 9304         	str	r3, [sp, #0x10]
    ef62: f8d4 2274    	ldr.w	r2, [r4, #0x274]
    ef66: f8df 3d04    	ldr.w	r3, [pc, #0xd04]        @ 0xfc6c <gst_lg_audio_render_change_state+0x24ac>
    ef6a: 9401         	str	r4, [sp, #0x4]
    ef6c: 9203         	str	r2, [sp, #0xc]
    ef6e: f8df 2d00    	ldr.w	r2, [pc, #0xd00]        @ 0xfc70 <gst_lg_audio_render_change_state+0x24b0>
    ef72: 447b         	add	r3, pc
    ef74: f503 731b    	add.w	r3, r3, #0x26c
    ef78: 447a         	add	r2, pc
    ef7a: 9202         	str	r2, [sp, #0x8]
    ef7c: f640 3239    	movw	r2, #0xb39
    ef80: 9200         	str	r2, [sp]
    ef82: f8df 2cf0    	ldr.w	r2, [pc, #0xcf0]        @ 0xfc74 <gst_lg_audio_render_change_state+0x24b4>
    ef86: 447a         	add	r2, pc
    ef88: f7f4 ee70    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xb320
    ef8c: e6fa         	b	0xed84 <gst_lg_audio_render_change_state+0x15c4> @ imm = #-0x20c
    ef8e: 6813         	ldr	r3, [r2]
    ef90: 075a         	lsls	r2, r3, #0x1d
    ef92: f100 8561    	bmi.w	0xfa58 <gst_lg_audio_render_change_state+0x2298> @ imm = #0xac2
    ef96: 6833         	ldr	r3, [r6]
    ef98: 2b03         	cmp	r3, #0x3
    ef9a: d91a         	bls	0xefd2 <gst_lg_audio_render_change_state+0x1812> @ imm = #0x34
    ef9c: f8df 3cc8    	ldr.w	r3, [pc, #0xcc8]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    efa0: 2104         	movs	r1, #0x4
    efa2: f8df 2cd4    	ldr.w	r2, [pc, #0xcd4]        @ 0xfc78 <gst_lg_audio_render_change_state+0x24b8>
    efa6: f858 3003    	ldr.w	r3, [r8, r3]
    efaa: 447a         	add	r2, pc
    efac: 6818         	ldr	r0, [r3]
    efae: f8d4 3288    	ldr.w	r3, [r4, #0x288]
    efb2: 9202         	str	r2, [sp, #0x8]
    efb4: f640 22da    	movw	r2, #0xada
    efb8: 9200         	str	r2, [sp]
    efba: 9303         	str	r3, [sp, #0xc]
    efbc: f8df 3cbc    	ldr.w	r3, [pc, #0xcbc]        @ 0xfc7c <gst_lg_audio_render_change_state+0x24bc>
    efc0: f8df 2cbc    	ldr.w	r2, [pc, #0xcbc]        @ 0xfc80 <gst_lg_audio_render_change_state+0x24c0>
    efc4: 9401         	str	r4, [sp, #0x4]
    efc6: 447b         	add	r3, pc
    efc8: f503 731b    	add.w	r3, r3, #0x26c
    efcc: 447a         	add	r2, pc
    efce: f7f4 ee4e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xb364
    efd2: f8d4 2288    	ldr.w	r2, [r4, #0x288]
    efd6: 2a06         	cmp	r2, #0x6
    efd8: f43f ab54    	beq.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x958
    efdc: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    efe0: 2100         	movs	r1, #0x0
    efe2: f7f4 eda4    	blx	0x3b2c <lgao_set_rts@plt> @ imm = #-0xb4b8
    efe6: f7ff bb4d    	b.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x966
    efea: 6813         	ldr	r3, [r2]
    efec: 075a         	lsls	r2, r3, #0x1d
    efee: f100 850d    	bmi.w	0xfa0c <gst_lg_audio_render_change_state+0x224c> @ imm = #0xa1a
    eff2: 6833         	ldr	r3, [r6]
    eff4: 2b03         	cmp	r3, #0x3
    eff6: d91a         	bls	0xf02e <gst_lg_audio_render_change_state+0x186e> @ imm = #0x34
    eff8: f8df 3c6c    	ldr.w	r3, [pc, #0xc6c]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    effc: 2104         	movs	r1, #0x4
    effe: f8df 2c84    	ldr.w	r2, [pc, #0xc84]        @ 0xfc84 <gst_lg_audio_render_change_state+0x24c4>
    f002: f858 3003    	ldr.w	r3, [r8, r3]
    f006: 447a         	add	r2, pc
    f008: 6818         	ldr	r0, [r3]
    f00a: f8d4 32a8    	ldr.w	r3, [r4, #0x2a8]
    f00e: 9202         	str	r2, [sp, #0x8]
    f010: f640 22c3    	movw	r2, #0xac3
    f014: 9200         	str	r2, [sp]
    f016: 9303         	str	r3, [sp, #0xc]
    f018: f8df 3c6c    	ldr.w	r3, [pc, #0xc6c]        @ 0xfc88 <gst_lg_audio_render_change_state+0x24c8>
    f01c: f8df 2c6c    	ldr.w	r2, [pc, #0xc6c]        @ 0xfc8c <gst_lg_audio_render_change_state+0x24cc>
    f020: 9401         	str	r4, [sp, #0x4]
    f022: 447b         	add	r3, pc
    f024: f503 731b    	add.w	r3, r3, #0x26c
    f028: 447a         	add	r2, pc
    f02a: f7f4 ee20    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xb3c0
    f02e: f8d4 12a8    	ldr.w	r1, [r4, #0x2a8]
    f032: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    f036: f7f5 e810    	blx	0x4058 <lgao_set_playrate@plt> @ imm = #-0xafe0
    f03a: f7ff bb23    	b.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x9ba
    f03e: 6813         	ldr	r3, [r2]
    f040: 075a         	lsls	r2, r3, #0x1d
    f042: f100 84f6    	bmi.w	0xfa32 <gst_lg_audio_render_change_state+0x2272> @ imm = #0x9ec
    f046: 6833         	ldr	r3, [r6]
    f048: 2b03         	cmp	r3, #0x3
    f04a: d91a         	bls	0xf082 <gst_lg_audio_render_change_state+0x18c2> @ imm = #0x34
    f04c: f8df 3c18    	ldr.w	r3, [pc, #0xc18]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    f050: 2104         	movs	r1, #0x4
    f052: f8df 2c3c    	ldr.w	r2, [pc, #0xc3c]        @ 0xfc90 <gst_lg_audio_render_change_state+0x24d0>
    f056: f858 3003    	ldr.w	r3, [r8, r3]
    f05a: 447a         	add	r2, pc
    f05c: 6818         	ldr	r0, [r3]
    f05e: f8d4 32b8    	ldr.w	r3, [r4, #0x2b8]
    f062: 9202         	str	r2, [sp, #0x8]
    f064: f640 22f2    	movw	r2, #0xaf2
    f068: 9200         	str	r2, [sp]
    f06a: 9303         	str	r3, [sp, #0xc]
    f06c: f8df 3c24    	ldr.w	r3, [pc, #0xc24]        @ 0xfc94 <gst_lg_audio_render_change_state+0x24d4>
    f070: f8df 2c24    	ldr.w	r2, [pc, #0xc24]        @ 0xfc98 <gst_lg_audio_render_change_state+0x24d8>
    f074: 9401         	str	r4, [sp, #0x4]
    f076: 447b         	add	r3, pc
    f078: f503 731b    	add.w	r3, r3, #0x26c
    f07c: 447a         	add	r2, pc
    f07e: f7f4 edf6    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xb414
    f082: f8d4 22b8    	ldr.w	r2, [r4, #0x2b8]
    f086: 2104         	movs	r1, #0x4
    f088: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    f08c: f7f4 ed4e    	blx	0x3b2c <lgao_set_rts@plt> @ imm = #-0xb564
    f090: f7ff baf8    	b.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0xa10
    f094: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    f098: f7f4 ee5a    	blx	0x3d50 <lgao_pause@plt> @ imm = #-0xb34c
    f09c: 2800         	cmp	r0, #0x0
    f09e: f6be ac92    	bge.w	0xd9c6 <gst_lg_audio_render_change_state+0x206> @ imm = #-0x16dc
    f0a2: f8df 0bf8    	ldr.w	r0, [pc, #0xbf8]        @ 0xfc9c <gst_lg_audio_render_change_state+0x24dc>
    f0a6: 4478         	add	r0, pc
    f0a8: 68c3         	ldr	r3, [r0, #0xc]
    f0aa: 681b         	ldr	r3, [r3]
    f0ac: 07db         	lsls	r3, r3, #0x1f
    f0ae: f100 8777    	bmi.w	0xffa0 <gst_lg_audio_render_change_state+0x27e0> @ imm = #0xeee
    f0b2: 6833         	ldr	r3, [r6]
    f0b4: 2b00         	cmp	r3, #0x0
    f0b6: f43e ac86    	beq.w	0xd9c6 <gst_lg_audio_render_change_state+0x206> @ imm = #-0x16f4
    f0ba: f8df 3bac    	ldr.w	r3, [pc, #0xbac]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    f0be: 2101         	movs	r1, #0x1
    f0c0: f8df 2bdc    	ldr.w	r2, [pc, #0xbdc]        @ 0xfca0 <gst_lg_audio_render_change_state+0x24e0>
    f0c4: f858 3003    	ldr.w	r3, [r8, r3]
    f0c8: 447a         	add	r2, pc
    f0ca: 9401         	str	r4, [sp, #0x4]
    f0cc: 6818         	ldr	r0, [r3]
    f0ce: 9202         	str	r2, [sp, #0x8]
    f0d0: f640 4229    	movw	r2, #0xc29
    f0d4: f8df 3bcc    	ldr.w	r3, [pc, #0xbcc]        @ 0xfca4 <gst_lg_audio_render_change_state+0x24e4>
    f0d8: 9200         	str	r2, [sp]
    f0da: f8df 2bcc    	ldr.w	r2, [pc, #0xbcc]        @ 0xfca8 <gst_lg_audio_render_change_state+0x24e8>
    f0de: 447b         	add	r3, pc
    f0e0: f503 7312    	add.w	r3, r3, #0x248
    f0e4: 447a         	add	r2, pc
    f0e6: f7f4 edc2    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xb47c
    f0ea: f7fe bc6c    	b.w	0xd9c6 <gst_lg_audio_render_change_state+0x206> @ imm = #-0x1728
    f0ee: f8df 0bbc    	ldr.w	r0, [pc, #0xbbc]        @ 0xfcac <gst_lg_audio_render_change_state+0x24ec>
    f0f2: 4478         	add	r0, pc
    f0f4: 68c3         	ldr	r3, [r0, #0xc]
    f0f6: 681b         	ldr	r3, [r3]
    f0f8: 071b         	lsls	r3, r3, #0x1c
    f0fa: f100 83e4    	bmi.w	0xf8c6 <gst_lg_audio_render_change_state+0x2106> @ imm = #0x7c8
    f0fe: 6833         	ldr	r3, [r6]
    f100: 2b03         	cmp	r3, #0x3
    f102: d91a         	bls	0xf13a <gst_lg_audio_render_change_state+0x197a> @ imm = #0x34
    f104: f8df 3b60    	ldr.w	r3, [pc, #0xb60]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    f108: 2104         	movs	r1, #0x4
    f10a: f8df 2ba4    	ldr.w	r2, [pc, #0xba4]        @ 0xfcb0 <gst_lg_audio_render_change_state+0x24f0>
    f10e: f858 3003    	ldr.w	r3, [r8, r3]
    f112: 447a         	add	r2, pc
    f114: 6818         	ldr	r0, [r3]
    f116: f8d4 32c4    	ldr.w	r3, [r4, #0x2c4]
    f11a: 9202         	str	r2, [sp, #0x8]
    f11c: f640 3249    	movw	r2, #0xb49
    f120: 9200         	str	r2, [sp]
    f122: 9303         	str	r3, [sp, #0xc]
    f124: f8df 3b8c    	ldr.w	r3, [pc, #0xb8c]        @ 0xfcb4 <gst_lg_audio_render_change_state+0x24f4>
    f128: f8df 2b8c    	ldr.w	r2, [pc, #0xb8c]        @ 0xfcb8 <gst_lg_audio_render_change_state+0x24f8>
    f12c: 9401         	str	r4, [sp, #0x4]
    f12e: 447b         	add	r3, pc
    f130: f503 731b    	add.w	r3, r3, #0x26c
    f134: 447a         	add	r2, pc
    f136: f7f4 ed9a    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xb4cc
    f13a: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    f13e: 2101         	movs	r1, #0x1
    f140: f7f4 ebfc    	blx	0x393c <lgao_set_clocktype@plt> @ imm = #-0xb808
    f144: f7ff ba9e    	b.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0xac4
    f148: 08e8         	lsrs	r0, r5, #0x3
    f14a: f7f4 ecbe    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0xb684
    f14e: 4681         	mov	r9, r0
    f150: 9814         	ldr	r0, [sp, #0x50]
    f152: f7f4 ecba    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0xb68c
    f156: f8df 2b64    	ldr.w	r2, [pc, #0xb64]        @ 0xfcbc <gst_lg_audio_render_change_state+0x24fc>
    f15a: 2102         	movs	r1, #0x2
    f15c: f8df 3b60    	ldr.w	r3, [pc, #0xb60]        @ 0xfcc0 <gst_lg_audio_render_change_state+0x2500>
    f160: e9cd 9001    	strd	r9, r0, [sp, #4]
    f164: 4630         	mov	r0, r6
    f166: 447a         	add	r2, pc
    f168: 447b         	add	r3, pc
    f16a: f502 7212    	add.w	r2, r2, #0x248
    f16e: 9300         	str	r3, [sp]
    f170: f640 339f    	movw	r3, #0xb9f
    f174: f7f4 ef1a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xb1cc
    f178: f7fe bcc0    	b.w	0xdafc <gst_lg_audio_render_change_state+0x33c> @ imm = #-0x1680
    f17c: f8df 6b44    	ldr.w	r6, [pc, #0xb44]        @ 0xfcc4 <gst_lg_audio_render_change_state+0x2504>
    f180: 447e         	add	r6, pc
    f182: f7fe bc58    	b.w	0xda36 <gst_lg_audio_render_change_state+0x276> @ imm = #-0x1750
    f186: f8df ab40    	ldr.w	r10, [pc, #0xb40]       @ 0xfcc8 <gst_lg_audio_render_change_state+0x2508>
    f18a: 44fa         	add	r10, pc
    f18c: f7fe bc2f    	b.w	0xd9ee <gst_lg_audio_render_change_state+0x22e> @ imm = #-0x17a2
    f190: 08e8         	lsrs	r0, r5, #0x3
    f192: f7f4 ec9a    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0xb6cc
    f196: 4682         	mov	r10, r0
    f198: 9814         	ldr	r0, [sp, #0x50]
    f19a: f7f4 ec96    	blx	0x3ac8 <gst_element_state_get_name@plt> @ imm = #-0xb6d4
    f19e: f8df 2b2c    	ldr.w	r2, [pc, #0xb2c]        @ 0xfccc <gst_lg_audio_render_change_state+0x250c>
    f1a2: 2102         	movs	r1, #0x2
    f1a4: f8df 3b28    	ldr.w	r3, [pc, #0xb28]        @ 0xfcd0 <gst_lg_audio_render_change_state+0x2510>
    f1a8: e9cd a001    	strd	r10, r0, [sp, #4]
    f1ac: 4648         	mov	r0, r9
    f1ae: 447a         	add	r2, pc
    f1b0: 447b         	add	r3, pc
    f1b2: f502 7212    	add.w	r2, r2, #0x248
    f1b6: 9300         	str	r3, [sp]
    f1b8: f640 4345    	movw	r3, #0xc45
    f1bc: f7f4 eef6    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xb214
    f1c0: f7fe bcdf    	b.w	0xdb82 <gst_lg_audio_render_change_state+0x3c2> @ imm = #-0x1642
    f1c4: aa29         	add	r2, sp, #0xa4
    f1c6: f8d4 1278    	ldr.w	r1, [r4, #0x278]
    f1ca: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    f1ce: f248 6aa0    	movw	r10, #0x86a0
    f1d2: 9329         	str	r3, [sp, #0xa4]
    f1d4: f2c0 0a01    	movt	r10, #0x1
    f1d8: f7f4 ef06    	blx	0x3fe8 <lgao_get_vdec_current_pts@plt> @ imm = #-0xb1f4
    f1dc: 9b29         	ldr	r3, [sp, #0xa4]
    f1de: f648 6239    	movw	r2, #0x8e39
    f1e2: f6c3 02e3    	movt	r2, #0x38e3
    f1e6: fba2 2903    	umull	r2, r9, r2, r3
    f1ea: f8df 3ae8    	ldr.w	r3, [pc, #0xae8]        @ 0xfcd4 <gst_lg_audio_render_change_state+0x2514>
    f1ee: 447b         	add	r3, pc
    f1f0: 68db         	ldr	r3, [r3, #0xc]
    f1f2: ea4f 0959    	lsr.w	r9, r9, #0x1
    f1f6: 681b         	ldr	r3, [r3]
    f1f8: fba9 a90a    	umull	r10, r9, r9, r10
    f1fc: 071b         	lsls	r3, r3, #0x1c
    f1fe: e9c4 a9bc    	strd	r10, r9, [r4, #752]
    f202: f140 811a    	bpl.w	0xf43a <gst_lg_audio_render_change_state+0x1c7a> @ imm = #0x234
    f206: e9dd 3216    	ldrd	r3, r2, [sp, #88]
    f20a: f1b2 3fff    	cmp.w	r2, #0xffffffff
    f20e: bf08         	it	eq
    f210: f1b3 3fff    	cmpeq.w	r3, #0xffffffff
    f214: f000 864d    	beq.w	0xfeb2 <gst_lg_audio_render_change_state+0x26f2> @ imm = #0xc9a
    f218: 4618         	mov	r0, r3
    f21a: f64e 63ef    	movw	r3, #0xeeef
    f21e: 4611         	mov	r1, r2
    f220: f6ce 63ee    	movt	r3, #0xeeee
    f224: f44f 4220    	mov.w	r2, #0xa000
    f228: 9318         	str	r3, [sp, #0x60]
    f22a: f2c3 02b8    	movt	r2, #0x30b8
    f22e: f240 3346    	movw	r3, #0x346
    f232: f00a fa47    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa48e
    f236: f60f 2320    	adr.w	r3, #2592
    f23a: e9d3 2300    	ldrd	r2, r3, [r3]
    f23e: f648 0b89    	movw	r11, #0x8889
    f242: 901a         	str	r0, [sp, #0x68]
    f244: f6c8 0b88    	movt	r11, #0x8888
    f248: e9dd 0116    	ldrd	r0, r1, [sp, #88]
    f24c: f00a fa3a    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa474
    f250: 1843         	adds	r3, r0, r1
    f252: 4602         	mov	r2, r0
    f254: f143 0300    	adc	r3, r3, #0x0
    f258: 9816         	ldr	r0, [sp, #0x58]
    f25a: fbab c103    	umull	r12, r1, r11, r3
    f25e: 08c9         	lsrs	r1, r1, #0x3
    f260: ebc1 1101    	rsb	r1, r1, r1, lsl #4
    f264: 1a5b         	subs	r3, r3, r1
    f266: 9918         	ldr	r1, [sp, #0x60]
    f268: 1ad3         	subs	r3, r2, r3
    f26a: fba3 3101    	umull	r3, r1, r3, r1
    f26e: 9917         	ldr	r1, [sp, #0x5c]
    f270: 089b         	lsrs	r3, r3, #0x2
    f272: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f276: eba2 0283    	sub.w	r2, r2, r3, lsl #2
    f27a: 921b         	str	r2, [sp, #0x6c]
    f27c: f60f 13e0    	adr.w	r3, #2528
    f280: e9d3 2300    	ldrd	r2, r3, [r3]
    f284: f00a fa1e    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa43c
    f288: 1843         	adds	r3, r0, r1
    f28a: 9219         	str	r2, [sp, #0x64]
    f28c: f143 0300    	adc	r3, r3, #0x0
    f290: fbab 1203    	umull	r1, r2, r11, r3
    f294: 9918         	ldr	r1, [sp, #0x60]
    f296: 08d2         	lsrs	r2, r2, #0x3
    f298: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    f29c: 1a9b         	subs	r3, r3, r2
    f29e: 1ac3         	subs	r3, r0, r3
    f2a0: fba3 3201    	umull	r3, r2, r3, r1
    f2a4: 089b         	lsrs	r3, r3, #0x2
    f2a6: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f2aa: eba0 0383    	sub.w	r3, r0, r3, lsl #2
    f2ae: 931c         	str	r3, [sp, #0x70]
    f2b0: f64e 63ef    	movw	r3, #0xeeef
    f2b4: f44f 4220    	mov.w	r2, #0xa000
    f2b8: f6ce 63ee    	movt	r3, #0xeeee
    f2bc: 4650         	mov	r0, r10
    f2be: 4649         	mov	r1, r9
    f2c0: f2c3 02b8    	movt	r2, #0x30b8
    f2c4: 9318         	str	r3, [sp, #0x60]
    f2c6: f240 3346    	movw	r3, #0x346
    f2ca: f00a f9fb    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa3f6
    f2ce: f60f 1388    	adr.w	r3, #2440
    f2d2: e9d3 2300    	ldrd	r2, r3, [r3]
    f2d6: 4649         	mov	r1, r9
    f2d8: 901d         	str	r0, [sp, #0x74]
    f2da: 4650         	mov	r0, r10
    f2dc: f00a f9f2    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa3e4
    f2e0: f648 0b89    	movw	r11, #0x8889
    f2e4: 1843         	adds	r3, r0, r1
    f2e6: f143 0300    	adc	r3, r3, #0x0
    f2ea: f6c8 0b88    	movt	r11, #0x8888
    f2ee: fbab ec03    	umull	lr, r12, r11, r3
    f2f2: 4602         	mov	r2, r0
    f2f4: 921e         	str	r2, [sp, #0x78]
    f2f6: 4650         	mov	r0, r10
    f2f8: 4649         	mov	r1, r9
    f2fa: ea4f 0cdc    	lsr.w	r12, r12, #0x3
    f2fe: ebcc 1c0c    	rsb	r12, r12, r12, lsl #4
    f302: eba3 030c    	sub.w	r3, r3, r12
    f306: 1ad3         	subs	r3, r2, r3
    f308: 9a18         	ldr	r2, [sp, #0x60]
    f30a: fba3 3c02    	umull	r3, r12, r3, r2
    f30e: 9a1e         	ldr	r2, [sp, #0x78]
    f310: 089b         	lsrs	r3, r3, #0x2
    f312: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f316: eba2 0383    	sub.w	r3, r2, r3, lsl #2
    f31a: 9320         	str	r3, [sp, #0x80]
    f31c: f60f 1340    	adr.w	r3, #2368
    f320: e9d3 2300    	ldrd	r2, r3, [r3]
    f324: f00a f9ce    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa39c
    f328: 1843         	adds	r3, r0, r1
    f32a: 921f         	str	r2, [sp, #0x7c]
    f32c: f143 0300    	adc	r3, r3, #0x0
    f330: e9d4 a9b4    	ldrd	r10, r9, [r4, #720]
    f334: fbab 1203    	umull	r1, r2, r11, r3
    f338: 08d2         	lsrs	r2, r2, #0x3
    f33a: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    f33e: 1a9b         	subs	r3, r3, r2
    f340: 9a18         	ldr	r2, [sp, #0x60]
    f342: 1ac3         	subs	r3, r0, r3
    f344: f1b9 3fff    	cmp.w	r9, #0xffffffff
    f348: bf08         	it	eq
    f34a: f1ba 3fff    	cmpeq.w	r10, #0xffffffff
    f34e: fba3 3202    	umull	r3, r2, r3, r2
    f352: ea4f 0393    	lsr.w	r3, r3, #0x2
    f356: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f35a: eba0 0183    	sub.w	r1, r0, r3, lsl #2
    f35e: 9121         	str	r1, [sp, #0x84]
    f360: f000 859e    	beq.w	0xfea0 <gst_lg_audio_render_change_state+0x26e0> @ imm = #0xb3c
    f364: f44f 4220    	mov.w	r2, #0xa000
    f368: 4649         	mov	r1, r9
    f36a: f2c3 02b8    	movt	r2, #0x30b8
    f36e: f240 3346    	movw	r3, #0x346
    f372: 4650         	mov	r0, r10
    f374: f00a f9a6    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa34c
    f378: f60f 03dc    	adr.w	r3, #2268
    f37c: e9d3 2300    	ldrd	r2, r3, [r3]
    f380: 4649         	mov	r1, r9
    f382: 901e         	str	r0, [sp, #0x78]
    f384: 4650         	mov	r0, r10
    f386: f00a f99d    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa33a
    f38a: 1843         	adds	r3, r0, r1
    f38c: 4602         	mov	r2, r0
    f38e: f143 0300    	adc	r3, r3, #0x0
    f392: 9222         	str	r2, [sp, #0x88]
    f394: fbab ec03    	umull	lr, r12, r11, r3
    f398: 4649         	mov	r1, r9
    f39a: 4650         	mov	r0, r10
    f39c: ea4f 0cdc    	lsr.w	r12, r12, #0x3
    f3a0: ebcc 1c0c    	rsb	r12, r12, r12, lsl #4
    f3a4: eba3 030c    	sub.w	r3, r3, r12
    f3a8: 1ad3         	subs	r3, r2, r3
    f3aa: 9a18         	ldr	r2, [sp, #0x60]
    f3ac: fba3 3c02    	umull	r3, r12, r3, r2
    f3b0: 9a22         	ldr	r2, [sp, #0x88]
    f3b2: 089b         	lsrs	r3, r3, #0x2
    f3b4: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f3b8: eba2 0983    	sub.w	r9, r2, r3, lsl #2
    f3bc: f60f 03a0    	adr.w	r3, #2208
    f3c0: e9d3 2300    	ldrd	r2, r3, [r3]
    f3c4: f00a f97e    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa2fc
    f3c8: 1843         	adds	r3, r0, r1
    f3ca: f143 0300    	adc	r3, r3, #0x0
    f3ce: fbab c103    	umull	r12, r1, r11, r3
    f3d2: 08c9         	lsrs	r1, r1, #0x3
    f3d4: ebc1 1101    	rsb	r1, r1, r1, lsl #4
    f3d8: 1a5b         	subs	r3, r3, r1
    f3da: 9918         	ldr	r1, [sp, #0x60]
    f3dc: 1ac3         	subs	r3, r0, r3
    f3de: fba3 3101    	umull	r3, r1, r3, r1
    f3e2: 089b         	lsrs	r3, r3, #0x2
    f3e4: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f3e8: eba0 0083    	sub.w	r0, r0, r3, lsl #2
    f3ec: 9b1e         	ldr	r3, [sp, #0x78]
    f3ee: 2103         	movs	r1, #0x3
    f3f0: e9cd 020b    	strd	r0, r2, [sp, #44]
    f3f4: f8df 28e0    	ldr.w	r2, [pc, #0x8e0]        @ 0xfcd8 <gst_lg_audio_render_change_state+0x2518>
    f3f8: 9309         	str	r3, [sp, #0x24]
    f3fa: 9b1f         	ldr	r3, [sp, #0x7c]
    f3fc: f8df 08dc    	ldr.w	r0, [pc, #0x8dc]        @ 0xfcdc <gst_lg_audio_render_change_state+0x251c>
    f400: 447a         	add	r2, pc
    f402: f502 7212    	add.w	r2, r2, #0x248
    f406: f8cd 9028    	str.w	r9, [sp, #0x28]
    f40a: 9308         	str	r3, [sp, #0x20]
    f40c: 9b21         	ldr	r3, [sp, #0x84]
    f40e: 4478         	add	r0, pc
    f410: 9307         	str	r3, [sp, #0x1c]
    f412: 9b20         	ldr	r3, [sp, #0x80]
    f414: 9306         	str	r3, [sp, #0x18]
    f416: 9b1d         	ldr	r3, [sp, #0x74]
    f418: 9305         	str	r3, [sp, #0x14]
    f41a: 9b19         	ldr	r3, [sp, #0x64]
    f41c: 9304         	str	r3, [sp, #0x10]
    f41e: 9b1c         	ldr	r3, [sp, #0x70]
    f420: 9303         	str	r3, [sp, #0xc]
    f422: 9b1b         	ldr	r3, [sp, #0x6c]
    f424: 9302         	str	r3, [sp, #0x8]
    f426: 9b1a         	ldr	r3, [sp, #0x68]
    f428: 9301         	str	r3, [sp, #0x4]
    f42a: f8df 38b4    	ldr.w	r3, [pc, #0x8b4]        @ 0xfce0 <gst_lg_audio_render_change_state+0x2520>
    f42e: 447b         	add	r3, pc
    f430: 9300         	str	r3, [sp]
    f432: f640 33d7    	movw	r3, #0xbd7
    f436: f7f4 edba    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xb48c
    f43a: 6833         	ldr	r3, [r6]
    f43c: 2b03         	cmp	r3, #0x3
    f43e: f240 8139    	bls.w	0xf6b4 <gst_lg_audio_render_change_state+0x1ef4> @ imm = #0x272
    f442: e9dd 3216    	ldrd	r3, r2, [sp, #88]
    f446: f1b2 3fff    	cmp.w	r2, #0xffffffff
    f44a: bf08         	it	eq
    f44c: f1b3 3fff    	cmpeq.w	r3, #0xffffffff
    f450: f000 8545    	beq.w	0xfede <gst_lg_audio_render_change_state+0x271e> @ imm = #0xa8a
    f454: 4693         	mov	r11, r2
    f456: 4611         	mov	r1, r2
    f458: f44f 4220    	mov.w	r2, #0xa000
    f45c: 4618         	mov	r0, r3
    f45e: f2c3 02b8    	movt	r2, #0x30b8
    f462: f240 3346    	movw	r3, #0x346
    f466: f00a f92d    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa25a
    f46a: f20f 73ec    	adr.w	r3, #2028
    f46e: e9d3 2300    	ldrd	r2, r3, [r3]
    f472: 4659         	mov	r1, r11
    f474: 9017         	str	r0, [sp, #0x5c]
    f476: f648 0989    	movw	r9, #0x8889
    f47a: 9816         	ldr	r0, [sp, #0x58]
    f47c: f6c8 0988    	movt	r9, #0x8888
    f480: f00a f920    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa240
    f484: 1843         	adds	r3, r0, r1
    f486: 4602         	mov	r2, r0
    f488: f143 0300    	adc	r3, r3, #0x0
    f48c: f64e 6aef    	movw	r10, #0xeeef
    f490: fba9 ec03    	umull	lr, r12, r9, r3
    f494: f6ce 6aee    	movt	r10, #0xeeee
    f498: 9816         	ldr	r0, [sp, #0x58]
    f49a: 4659         	mov	r1, r11
    f49c: ea4f 0cdc    	lsr.w	r12, r12, #0x3
    f4a0: ebcc 1c0c    	rsb	r12, r12, r12, lsl #4
    f4a4: eba3 030c    	sub.w	r3, r3, r12
    f4a8: 1ad3         	subs	r3, r2, r3
    f4aa: fba3 3c0a    	umull	r3, r12, r3, r10
    f4ae: 089b         	lsrs	r3, r3, #0x2
    f4b0: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f4b4: eba2 0383    	sub.w	r3, r2, r3, lsl #2
    f4b8: 9316         	str	r3, [sp, #0x58]
    f4ba: f20f 73a4    	adr.w	r3, #1956
    f4be: e9d3 2300    	ldrd	r2, r3, [r3]
    f4c2: f00a f8ff    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa1fe
    f4c6: 1843         	adds	r3, r0, r1
    f4c8: 4693         	mov	r11, r2
    f4ca: f143 0300    	adc	r3, r3, #0x0
    f4ce: fba9 1203    	umull	r1, r2, r9, r3
    f4d2: 08d2         	lsrs	r2, r2, #0x3
    f4d4: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    f4d8: 1a9b         	subs	r3, r3, r2
    f4da: 1ac3         	subs	r3, r0, r3
    f4dc: fba3 320a    	umull	r3, r2, r3, r10
    f4e0: 089b         	lsrs	r3, r3, #0x2
    f4e2: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f4e6: eba0 0383    	sub.w	r3, r0, r3, lsl #2
    f4ea: 9318         	str	r3, [sp, #0x60]
    f4ec: e9d4 9abc    	ldrd	r9, r10, [r4, #752]
    f4f0: f1ba 3fff    	cmp.w	r10, #0xffffffff
    f4f4: bf08         	it	eq
    f4f6: f1b9 3fff    	cmpeq.w	r9, #0xffffffff
    f4fa: f000 85a9    	beq.w	0x10050 <gst_lg_audio_render_change_state+0x2890> @ imm = #0xb52
    f4fe: f64e 6cef    	movw	r12, #0xeeef
    f502: f648 0389    	movw	r3, #0x8889
    f506: 4662         	mov	r2, r12
    f508: f6c8 0388    	movt	r3, #0x8888
    f50c: f6ce 62ee    	movt	r2, #0xeeee
    f510: 931c         	str	r3, [sp, #0x70]
    f512: 921d         	str	r2, [sp, #0x74]
    f514: f44f 4220    	mov.w	r2, #0xa000
    f518: f2c3 02b8    	movt	r2, #0x30b8
    f51c: f240 3346    	movw	r3, #0x346
    f520: 4648         	mov	r0, r9
    f522: 4651         	mov	r1, r10
    f524: f00a f8ce    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa19c
    f528: f20f 732c    	adr.w	r3, #1836
    f52c: e9d3 2300    	ldrd	r2, r3, [r3]
    f530: 4651         	mov	r1, r10
    f532: 901a         	str	r0, [sp, #0x68]
    f534: 4648         	mov	r0, r9
    f536: f00a f8c5    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa18a
    f53a: 9a1c         	ldr	r2, [sp, #0x70]
    f53c: 1843         	adds	r3, r0, r1
    f53e: f143 0300    	adc	r3, r3, #0x0
    f542: 9019         	str	r0, [sp, #0x64]
    f544: 4651         	mov	r1, r10
    f546: 4648         	mov	r0, r9
    f548: fba2 ec03    	umull	lr, r12, r2, r3
    f54c: 9a19         	ldr	r2, [sp, #0x64]
    f54e: ea4f 0cdc    	lsr.w	r12, r12, #0x3
    f552: ebcc 1c0c    	rsb	r12, r12, r12, lsl #4
    f556: eba3 030c    	sub.w	r3, r3, r12
    f55a: 1ad3         	subs	r3, r2, r3
    f55c: 9a1d         	ldr	r2, [sp, #0x74]
    f55e: fba3 3c02    	umull	r3, r12, r3, r2
    f562: 9a19         	ldr	r2, [sp, #0x64]
    f564: 089b         	lsrs	r3, r3, #0x2
    f566: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f56a: eba2 0c83    	sub.w	r12, r2, r3, lsl #2
    f56e: f20f 63f0    	adr.w	r3, #1776
    f572: e9d3 2300    	ldrd	r2, r3, [r3]
    f576: f8cd c06c    	str.w	r12, [sp, #0x6c]
    f57a: f00a f8a3    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa146
    f57e: 9219         	str	r2, [sp, #0x64]
    f580: 1843         	adds	r3, r0, r1
    f582: 9a1c         	ldr	r2, [sp, #0x70]
    f584: f143 0300    	adc	r3, r3, #0x0
    f588: fba2 1203    	umull	r1, r2, r2, r3
    f58c: 08d2         	lsrs	r2, r2, #0x3
    f58e: ebc2 1202    	rsb	r2, r2, r2, lsl #4
    f592: 1a9b         	subs	r3, r3, r2
    f594: 9a1d         	ldr	r2, [sp, #0x74]
    f596: 1ac3         	subs	r3, r0, r3
    f598: fba3 3202    	umull	r3, r2, r3, r2
    f59c: 089b         	lsrs	r3, r3, #0x2
    f59e: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f5a2: eba0 0383    	sub.w	r3, r0, r3, lsl #2
    f5a6: 931c         	str	r3, [sp, #0x70]
    f5a8: e9d4 9ab4    	ldrd	r9, r10, [r4, #720]
    f5ac: f1ba 3fff    	cmp.w	r10, #0xffffffff
    f5b0: bf08         	it	eq
    f5b2: f1b9 3fff    	cmpeq.w	r9, #0xffffffff
    f5b6: f000 8561    	beq.w	0x1007c <gst_lg_audio_render_change_state+0x28bc> @ imm = #0xac2
    f5ba: f64e 62ef    	movw	r2, #0xeeef
    f5be: f648 0389    	movw	r3, #0x8889
    f5c2: f6ce 62ee    	movt	r2, #0xeeee
    f5c6: f6c8 0388    	movt	r3, #0x8888
    f5ca: 921f         	str	r2, [sp, #0x7c]
    f5cc: f44f 4220    	mov.w	r2, #0xa000
    f5d0: 4648         	mov	r0, r9
    f5d2: f2c3 02b8    	movt	r2, #0x30b8
    f5d6: 931e         	str	r3, [sp, #0x78]
    f5d8: 4651         	mov	r1, r10
    f5da: f240 3346    	movw	r3, #0x346
    f5de: f00a f871    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa0e2
    f5e2: f20f 6374    	adr.w	r3, #1652
    f5e6: e9d3 2300    	ldrd	r2, r3, [r3]
    f5ea: 4651         	mov	r1, r10
    f5ec: 901d         	str	r0, [sp, #0x74]
    f5ee: 4648         	mov	r0, r9
    f5f0: f00a f868    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa0d0
    f5f4: 9a1e         	ldr	r2, [sp, #0x78]
    f5f6: 1843         	adds	r3, r0, r1
    f5f8: f143 0300    	adc	r3, r3, #0x0
    f5fc: 9020         	str	r0, [sp, #0x80]
    f5fe: 4651         	mov	r1, r10
    f600: 4648         	mov	r0, r9
    f602: fba2 ec03    	umull	lr, r12, r2, r3
    f606: 9a20         	ldr	r2, [sp, #0x80]
    f608: ea4f 0cdc    	lsr.w	r12, r12, #0x3
    f60c: ebcc 1c0c    	rsb	r12, r12, r12, lsl #4
    f610: eba3 030c    	sub.w	r3, r3, r12
    f614: 1ad3         	subs	r3, r2, r3
    f616: 9a1f         	ldr	r2, [sp, #0x7c]
    f618: fba3 3c02    	umull	r3, r12, r3, r2
    f61c: 9a20         	ldr	r2, [sp, #0x80]
    f61e: 089b         	lsrs	r3, r3, #0x2
    f620: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f624: eba2 0983    	sub.w	r9, r2, r3, lsl #2
    f628: f20f 6334    	adr.w	r3, #1588
    f62c: e9d3 2300    	ldrd	r2, r3, [r3]
    f630: f00a f848    	bl	0x196c4 <__aeabi_uldivmod> @ imm = #0xa090
    f634: 1843         	adds	r3, r0, r1
    f636: 991e         	ldr	r1, [sp, #0x78]
    f638: f143 0300    	adc	r3, r3, #0x0
    f63c: fba1 c103    	umull	r12, r1, r1, r3
    f640: 08c9         	lsrs	r1, r1, #0x3
    f642: ebc1 1101    	rsb	r1, r1, r1, lsl #4
    f646: 1a5b         	subs	r3, r3, r1
    f648: 991f         	ldr	r1, [sp, #0x7c]
    f64a: 1ac3         	subs	r3, r0, r3
    f64c: fba3 3101    	umull	r3, r1, r3, r1
    f650: 089b         	lsrs	r3, r3, #0x2
    f652: ebc3 1303    	rsb	r3, r3, r3, lsl #4
    f656: eba0 0083    	sub.w	r0, r0, r3, lsl #2
    f65a: f8df 360c    	ldr.w	r3, [pc, #0x60c]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    f65e: 2104         	movs	r1, #0x4
    f660: f858 3003    	ldr.w	r3, [r8, r3]
    f664: e9cd 020d    	strd	r0, r2, [sp, #52]
    f668: 9a1d         	ldr	r2, [sp, #0x74]
    f66a: f8cd 9030    	str.w	r9, [sp, #0x30]
    f66e: 920b         	str	r2, [sp, #0x2c]
    f670: 9a19         	ldr	r2, [sp, #0x64]
    f672: 920a         	str	r2, [sp, #0x28]
    f674: 9a1c         	ldr	r2, [sp, #0x70]
    f676: 9209         	str	r2, [sp, #0x24]
    f678: 9a1b         	ldr	r2, [sp, #0x6c]
    f67a: 9208         	str	r2, [sp, #0x20]
    f67c: 9a1a         	ldr	r2, [sp, #0x68]
    f67e: e9cd b206    	strd	r11, r2, [sp, #24]
    f682: 9a18         	ldr	r2, [sp, #0x60]
    f684: 9205         	str	r2, [sp, #0x14]
    f686: 9a16         	ldr	r2, [sp, #0x58]
    f688: 9204         	str	r2, [sp, #0x10]
    f68a: 9a17         	ldr	r2, [sp, #0x5c]
    f68c: 9203         	str	r2, [sp, #0xc]
    f68e: f8df 2654    	ldr.w	r2, [pc, #0x654]        @ 0xfce4 <gst_lg_audio_render_change_state+0x2524>
    f692: 6818         	ldr	r0, [r3]
    f694: f8df 3650    	ldr.w	r3, [pc, #0x650]        @ 0xfce8 <gst_lg_audio_render_change_state+0x2528>
    f698: 447a         	add	r2, pc
    f69a: 9401         	str	r4, [sp, #0x4]
    f69c: 9202         	str	r2, [sp, #0x8]
    f69e: f640 32d7    	movw	r2, #0xbd7
    f6a2: 9200         	str	r2, [sp]
    f6a4: 447b         	add	r3, pc
    f6a6: f8df 2644    	ldr.w	r2, [pc, #0x644]        @ 0xfcec <gst_lg_audio_render_change_state+0x252c>
    f6aa: f503 7312    	add.w	r3, r3, #0x248
    f6ae: 447a         	add	r2, pc
    f6b0: f7f4 eadc    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xba48
    f6b4: f8d4 32f8    	ldr.w	r3, [r4, #0x2f8]
    f6b8: 2b00         	cmp	r3, #0x0
    f6ba: f040 80d3    	bne.w	0xf864 <gst_lg_audio_render_change_state+0x20a4> @ imm = #0x1a6
    f6be: e9d4 23b4    	ldrd	r2, r3, [r4, #720]
    f6c2: e9cd 2300    	strd	r2, r3, [sp]
    f6c6: 4620         	mov	r0, r4
    f6c8: 2300         	movs	r3, #0x0
    f6ca: 2200         	movs	r2, #0x0
    f6cc: f7f5 fd44    	bl	0x5158 <gst_lg_audio_render_update_clock> @ imm = #-0xa578
    f6d0: 2301         	movs	r3, #0x1
    f6d2: f8c4 32f8    	str.w	r3, [r4, #0x2f8]
    f6d6: f7ff b834    	b.w	0xe742 <gst_lg_audio_render_change_state+0xf82> @ imm = #-0xf98
    f6da: f8df 358c    	ldr.w	r3, [pc, #0x58c]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    f6de: 2104         	movs	r1, #0x4
    f6e0: f8df 260c    	ldr.w	r2, [pc, #0x60c]        @ 0xfcf0 <gst_lg_audio_render_change_state+0x2530>
    f6e4: f858 3003    	ldr.w	r3, [r8, r3]
    f6e8: 447a         	add	r2, pc
    f6ea: 6818         	ldr	r0, [r3]
    f6ec: f8d4 32c4    	ldr.w	r3, [r4, #0x2c4]
    f6f0: 9202         	str	r2, [sp, #0x8]
    f6f2: f640 3244    	movw	r2, #0xb44
    f6f6: 9200         	str	r2, [sp]
    f6f8: 9303         	str	r3, [sp, #0xc]
    f6fa: f8df 35f8    	ldr.w	r3, [pc, #0x5f8]        @ 0xfcf4 <gst_lg_audio_render_change_state+0x2534>
    f6fe: f8df 25f8    	ldr.w	r2, [pc, #0x5f8]        @ 0xfcf8 <gst_lg_audio_render_change_state+0x2538>
    f702: 9401         	str	r4, [sp, #0x4]
    f704: 447b         	add	r3, pc
    f706: f503 731b    	add.w	r3, r3, #0x26c
    f70a: 447a         	add	r2, pc
    f70c: f7f4 eaae    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xbaa4
    f710: f7ff bad5    	b.w	0xecbe <gst_lg_audio_render_change_state+0x14fe> @ imm = #-0xa56
    f714: f8df 3550    	ldr.w	r3, [pc, #0x550]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    f718: 2104         	movs	r1, #0x4
    f71a: f8df 25e0    	ldr.w	r2, [pc, #0x5e0]        @ 0xfcfc <gst_lg_audio_render_change_state+0x253c>
    f71e: f858 3003    	ldr.w	r3, [r8, r3]
    f722: 447a         	add	r2, pc
    f724: 6818         	ldr	r0, [r3]
    f726: f8d4 3224    	ldr.w	r3, [r4, #0x224]
    f72a: 9202         	str	r2, [sp, #0x8]
    f72c: f640 22fe    	movw	r2, #0xafe
    f730: 9200         	str	r2, [sp]
    f732: 9303         	str	r3, [sp, #0xc]
    f734: f8df 35c8    	ldr.w	r3, [pc, #0x5c8]        @ 0xfd00 <gst_lg_audio_render_change_state+0x2540>
    f738: f8df 25c8    	ldr.w	r2, [pc, #0x5c8]        @ 0xfd04 <gst_lg_audio_render_change_state+0x2544>
    f73c: 9401         	str	r4, [sp, #0x4]
    f73e: 447b         	add	r3, pc
    f740: f503 731b    	add.w	r3, r3, #0x26c
    f744: 447a         	add	r2, pc
    f746: f7f4 ea92    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xbadc
    f74a: f7ff ba3e    	b.w	0xebca <gst_lg_audio_render_change_state+0x140a> @ imm = #-0xb84
    f74e: f8df 3518    	ldr.w	r3, [pc, #0x518]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    f752: 2104         	movs	r1, #0x4
    f754: f858 3003    	ldr.w	r3, [r8, r3]
    f758: f8cd a00c    	str.w	r10, [sp, #0xc]
    f75c: 9401         	str	r4, [sp, #0x4]
    f75e: 6818         	ldr	r0, [r3]
    f760: f8df 35a4    	ldr.w	r3, [pc, #0x5a4]        @ 0xfd08 <gst_lg_audio_render_change_state+0x2548>
    f764: 447b         	add	r3, pc
    f766: 9302         	str	r3, [sp, #0x8]
    f768: f44f 6338    	mov.w	r3, #0xb80
    f76c: 9300         	str	r3, [sp]
    f76e: e9dd 3216    	ldrd	r3, r2, [sp, #88]
    f772: f7f4 ea7c    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xbb08
    f776: f7fe bf81    	b.w	0xe67c <gst_lg_audio_render_change_state+0xebc> @ imm = #-0x10fe
    f77a: 2304         	movs	r3, #0x4
    f77c: 6f78         	ldr	r0, [r7, #0x74]
    f77e: f8c4 3218    	str.w	r3, [r4, #0x218]
    f782: f7f4 ecfa    	blx	0x4178 <gst_clock_get_time@plt> @ imm = #-0xb60c
    f786: e9d7 b31e    	ldrd	r11, r3, [r7, #120]
    f78a: 468a         	mov	r10, r1
    f78c: 4681         	mov	r9, r0
    f78e: 9316         	str	r3, [sp, #0x58]
    f790: f7f4 ec86    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xb6f4
    f794: 4601         	mov	r1, r0
    f796: 4638         	mov	r0, r7
    f798: f7f4 ebf6    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xb814
    f79c: 9b16         	ldr	r3, [sp, #0x58]
    f79e: ebb9 090b    	subs.w	r9, r9, r11
    f7a2: eb6a 0a03    	sbc.w	r10, r10, r3
    f7a6: f8d0 3118    	ldr.w	r3, [r0, #0x118]
    f7aa: ebb9 0903    	subs.w	r9, r9, r3
    f7ae: f8d0 311c    	ldr.w	r3, [r0, #0x11c]
    f7b2: eb6a 0a03    	sbc.w	r10, r10, r3
    f7b6: f7f4 ec74    	blx	0x40a0 <gst_base_sink_get_type@plt> @ imm = #-0xb718
    f7ba: 4601         	mov	r1, r0
    f7bc: 4638         	mov	r0, r7
    f7be: f7f4 ebe4    	blx	0x3f88 <g_type_check_instance_cast@plt> @ imm = #-0xb838
    f7c2: e9d0 014a    	ldrd	r0, r1, [r0, #296]
    f7c6: 464a         	mov	r2, r9
    f7c8: 4653         	mov	r3, r10
    f7ca: e9cd 0100    	strd	r0, r1, [sp]
    f7ce: 4620         	mov	r0, r4
    f7d0: f7f5 fcc2    	bl	0x5158 <gst_lg_audio_render_update_clock> @ imm = #-0xa67c
    f7d4: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    f7d8: f7f4 e93a    	blx	0x3a50 <lgao_play@plt>  @ imm = #-0xbd8c
    f7dc: b1e0         	cbz	r0, 0xf818 <gst_lg_audio_render_change_state+0x2058> @ imm = #0x38
    f7de: 6833         	ldr	r3, [r6]
    f7e0: 2b01         	cmp	r3, #0x1
    f7e2: d919         	bls	0xf818 <gst_lg_audio_render_change_state+0x2058> @ imm = #0x32
    f7e4: f8df 3480    	ldr.w	r3, [pc, #0x480]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    f7e8: f04f 0c00    	mov.w	r12, #0x0
    f7ec: f8df 251c    	ldr.w	r2, [pc, #0x51c]        @ 0xfd0c <gst_lg_audio_render_change_state+0x254c>
    f7f0: 2102         	movs	r1, #0x2
    f7f2: f858 3003    	ldr.w	r3, [r8, r3]
    f7f6: 447a         	add	r2, pc
    f7f8: 6818         	ldr	r0, [r3]
    f7fa: f8df 3514    	ldr.w	r3, [pc, #0x514]        @ 0xfd10 <gst_lg_audio_render_change_state+0x2550>
    f7fe: 9202         	str	r2, [sp, #0x8]
    f800: f640 420e    	movw	r2, #0xc0e
    f804: e9cd 2c00    	strd	r2, r12, [sp]
    f808: f8df 2508    	ldr.w	r2, [pc, #0x508]        @ 0xfd14 <gst_lg_audio_render_change_state+0x2554>
    f80c: 447b         	add	r3, pc
    f80e: f503 7312    	add.w	r3, r3, #0x248
    f812: 447a         	add	r2, pc
    f814: f7f4 ea2a    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xbbac
    f818: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    f81c: f7f4 e828    	blx	0x3870 <lgao_start_renderer@plt> @ imm = #-0xbfb0
    f820: 2800         	cmp	r0, #0x0
    f822: f43e af65    	beq.w	0xe6f0 <gst_lg_audio_render_change_state+0xf30> @ imm = #-0x1136
    f826: 6833         	ldr	r3, [r6]
    f828: 2b01         	cmp	r3, #0x1
    f82a: f67e af61    	bls.w	0xe6f0 <gst_lg_audio_render_change_state+0xf30> @ imm = #-0x113e
    f82e: f8df 3438    	ldr.w	r3, [pc, #0x438]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    f832: 2100         	movs	r1, #0x0
    f834: f8df 24e0    	ldr.w	r2, [pc, #0x4e0]        @ 0xfd18 <gst_lg_audio_render_change_state+0x2558>
    f838: f858 3003    	ldr.w	r3, [r8, r3]
    f83c: 447a         	add	r2, pc
    f83e: 6818         	ldr	r0, [r3]
    f840: 9202         	str	r2, [sp, #0x8]
    f842: f640 420f    	movw	r2, #0xc0f
    f846: f8df 34d4    	ldr.w	r3, [pc, #0x4d4]        @ 0xfd1c <gst_lg_audio_render_change_state+0x255c>
    f84a: e9cd 2100    	strd	r2, r1, [sp]
    f84e: 2102         	movs	r1, #0x2
    f850: f8df 24cc    	ldr.w	r2, [pc, #0x4cc]        @ 0xfd20 <gst_lg_audio_render_change_state+0x2560>
    f854: 447b         	add	r3, pc
    f856: f503 7312    	add.w	r3, r3, #0x248
    f85a: 447a         	add	r2, pc
    f85c: f7f4 ea06    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xbbf4
    f860: f7fe bf46    	b.w	0xe6f0 <gst_lg_audio_render_change_state+0xf30> @ imm = #-0x1174
    f864: e9d4 23bc    	ldrd	r2, r3, [r4, #752]
    f868: e72b         	b	0xf6c2 <gst_lg_audio_render_change_state+0x1f02> @ imm = #-0x1aa
    f86a: f8df 04b8    	ldr.w	r0, [pc, #0x4b8]        @ 0xfd24 <gst_lg_audio_render_change_state+0x2564>
    f86e: 4478         	add	r0, pc
    f870: 68c3         	ldr	r3, [r0, #0xc]
    f872: 681b         	ldr	r3, [r3]
    f874: 0719         	lsls	r1, r3, #0x1c
    f876: f100 835c    	bmi.w	0xff32 <gst_lg_audio_render_change_state+0x2772> @ imm = #0x6b8
    f87a: 6833         	ldr	r3, [r6]
    f87c: 2b03         	cmp	r3, #0x3
    f87e: d916         	bls	0xf8ae <gst_lg_audio_render_change_state+0x20ee> @ imm = #0x2c
    f880: 4bf9         	ldr	r3, [pc, #0x3e4]        @ 0xfc68 <gst_lg_audio_render_change_state+0x24a8>
    f882: 2104         	movs	r1, #0x4
    f884: f8df 24a0    	ldr.w	r2, [pc, #0x4a0]        @ 0xfd28 <gst_lg_audio_render_change_state+0x2568>
    f888: f858 3003    	ldr.w	r3, [r8, r3]
    f88c: 447a         	add	r2, pc
    f88e: 9401         	str	r4, [sp, #0x4]
    f890: 6818         	ldr	r0, [r3]
    f892: f8df 3498    	ldr.w	r3, [pc, #0x498]        @ 0xfd2c <gst_lg_audio_render_change_state+0x256c>
    f896: 9202         	str	r2, [sp, #0x8]
    f898: f640 3202    	movw	r2, #0xb02
    f89c: 9200         	str	r2, [sp]
    f89e: f8df 2490    	ldr.w	r2, [pc, #0x490]        @ 0xfd30 <gst_lg_audio_render_change_state+0x2570>
    f8a2: 447b         	add	r3, pc
    f8a4: f503 731b    	add.w	r3, r3, #0x26c
    f8a8: 447a         	add	r2, pc
    f8aa: f7f4 e9e0    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xbc40
    f8ae: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    f8b2: 2104         	movs	r1, #0x4
    f8b4: f7f4 ecb4    	blx	0x4220 <lgao_set_apptype@plt> @ imm = #-0xb698
    f8b8: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    f8bc: 2101         	movs	r1, #0x1
    f8be: f7f4 e9c4    	blx	0x3c48 <lgao_set_nodelay@plt> @ imm = #-0xbc78
    f8c2: f7fe bedf    	b.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x1242
    f8c6: f8df 246c    	ldr.w	r2, [pc, #0x46c]        @ 0xfd34 <gst_lg_audio_render_change_state+0x2574>
    f8ca: 2103         	movs	r1, #0x3
    f8cc: f8df 3468    	ldr.w	r3, [pc, #0x468]        @ 0xfd38 <gst_lg_audio_render_change_state+0x2578>
    f8d0: f8cd a004    	str.w	r10, [sp, #0x4]
    f8d4: 447a         	add	r2, pc
    f8d6: 447b         	add	r3, pc
    f8d8: f502 721b    	add.w	r2, r2, #0x26c
    f8dc: 9300         	str	r3, [sp]
    f8de: f640 3349    	movw	r3, #0xb49
    f8e2: f7f4 eb64    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xb938
    f8e6: e40a         	b	0xf0fe <gst_lg_audio_render_change_state+0x193e> @ imm = #-0x7ec
    f8e8: f8df 2450    	ldr.w	r2, [pc, #0x450]        @ 0xfd3c <gst_lg_audio_render_change_state+0x257c>
    f8ec: 4649         	mov	r1, r9
    f8ee: f8df 3450    	ldr.w	r3, [pc, #0x450]        @ 0xfd40 <gst_lg_audio_render_change_state+0x2580>
    f8f2: 447a         	add	r2, pc
    f8f4: 447b         	add	r3, pc
    f8f6: f502 7212    	add.w	r2, r2, #0x248
    f8fa: 9300         	str	r3, [sp]
    f8fc: f640 33ab    	movw	r3, #0xbab
    f900: f7f4 eb54    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xb958
    f904: f7fe b97e    	b.w	0xdc04 <gst_lg_audio_render_change_state+0x444> @ imm = #-0x1d04
    f908: f8df 3438    	ldr.w	r3, [pc, #0x438]        @ 0xfd44 <gst_lg_audio_render_change_state+0x2584>
    f90c: 2103         	movs	r1, #0x3
    f90e: f8df 0438    	ldr.w	r0, [pc, #0x438]        @ 0xfd48 <gst_lg_audio_render_change_state+0x2588>
    f912: 9a18         	ldr	r2, [sp, #0x60]
    f914: 447b         	add	r3, pc
    f916: f8cd a004    	str.w	r10, [sp, #0x4]
    f91a: 9300         	str	r3, [sp]
    f91c: 4478         	add	r0, pc
    f91e: f44f 6338    	mov.w	r3, #0xb80
    f922: f7f4 eb44    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xb978
    f926: f7fe bea5    	b.w	0xe674 <gst_lg_audio_render_change_state+0xeb4> @ imm = #-0x12b6
    f92a: f8d4 32b4    	ldr.w	r3, [r4, #0x2b4]
    f92e: 2102         	movs	r1, #0x2
    f930: f8df 2418    	ldr.w	r2, [pc, #0x418]        @ 0xfd4c <gst_lg_audio_render_change_state+0x258c>
    f934: f8df 0418    	ldr.w	r0, [pc, #0x418]        @ 0xfd50 <gst_lg_audio_render_change_state+0x2590>
    f938: 9301         	str	r3, [sp, #0x4]
    f93a: f8df 3418    	ldr.w	r3, [pc, #0x418]        @ 0xfd54 <gst_lg_audio_render_change_state+0x2594>
    f93e: 447a         	add	r2, pc
    f940: f502 721b    	add.w	r2, r2, #0x26c
    f944: 4478         	add	r0, pc
    f946: 447b         	add	r3, pc
    f948: 9300         	str	r3, [sp]
    f94a: f640 23ec    	movw	r3, #0xaec
    f94e: f7f4 eb2e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xb9a4
    f952: f7ff b875    	b.w	0xea40 <gst_lg_audio_render_change_state+0x1280> @ imm = #-0xf16
    f956: f8df 2400    	ldr.w	r2, [pc, #0x400]        @ 0xfd58 <gst_lg_audio_render_change_state+0x2598>
    f95a: 2103         	movs	r1, #0x3
    f95c: 4bff         	ldr	r3, [pc, #0x3fc]        @ 0xfd5c <gst_lg_audio_render_change_state+0x259c>
    f95e: f8df 0400    	ldr.w	r0, [pc, #0x400]        @ 0xfd60 <gst_lg_audio_render_change_state+0x25a0>
    f962: 447a         	add	r2, pc
    f964: 447b         	add	r3, pc
    f966: f502 721b    	add.w	r2, r2, #0x26c
    f96a: 9300         	str	r3, [sp]
    f96c: 4478         	add	r0, pc
    f96e: f640 3352    	movw	r3, #0xb52
    f972: f7f4 eb1c    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xb9c8
    f976: f7ff b83d    	b.w	0xe9f4 <gst_lg_audio_render_change_state+0x1234> @ imm = #-0xf86
    f97a: 4afa         	ldr	r2, [pc, #0x3e8]        @ 0xfd64 <gst_lg_audio_render_change_state+0x25a4>
    f97c: 2103         	movs	r1, #0x3
    f97e: 4bfa         	ldr	r3, [pc, #0x3e8]        @ 0xfd68 <gst_lg_audio_render_change_state+0x25a8>
    f980: 48fa         	ldr	r0, [pc, #0x3e8]        @ 0xfd6c <gst_lg_audio_render_change_state+0x25ac>
    f982: 447a         	add	r2, pc
    f984: 447b         	add	r3, pc
    f986: f502 721b    	add.w	r2, r2, #0x26c
    f98a: 9300         	str	r3, [sp]
    f98c: 4478         	add	r0, pc
    f98e: f640 3359    	movw	r3, #0xb59
    f992: f7f4 eb0c    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xb9e8
    f996: f7ff b807    	b.w	0xe9a8 <gst_lg_audio_render_change_state+0x11e8> @ imm = #-0xff2
    f99a: f8d4 3308    	ldr.w	r3, [r4, #0x308]
    f99e: 2102         	movs	r1, #0x2
    f9a0: 4af3         	ldr	r2, [pc, #0x3cc]        @ 0xfd70 <gst_lg_audio_render_change_state+0x25b0>
    f9a2: 48f4         	ldr	r0, [pc, #0x3d0]        @ 0xfd74 <gst_lg_audio_render_change_state+0x25b4>
    f9a4: 9301         	str	r3, [sp, #0x4]
    f9a6: 4bf4         	ldr	r3, [pc, #0x3d0]        @ 0xfd78 <gst_lg_audio_render_change_state+0x25b8>
    f9a8: 447a         	add	r2, pc
    f9aa: f502 721b    	add.w	r2, r2, #0x26c
    f9ae: 4478         	add	r0, pc
    f9b0: 447b         	add	r3, pc
    f9b2: 9300         	str	r3, [sp]
    f9b4: f640 23f8    	movw	r3, #0xaf8
    f9b8: f7f4 eaf8    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xba10
    f9bc: f7fe bfca    	b.w	0xe954 <gst_lg_audio_render_change_state+0x1194> @ imm = #-0x106c
    f9c0: f8d4 3304    	ldr.w	r3, [r4, #0x304]
    f9c4: 2103         	movs	r1, #0x3
    f9c6: 4aed         	ldr	r2, [pc, #0x3b4]        @ 0xfd7c <gst_lg_audio_render_change_state+0x25bc>
    f9c8: 48ed         	ldr	r0, [pc, #0x3b4]        @ 0xfd80 <gst_lg_audio_render_change_state+0x25c0>
    f9ca: 9301         	str	r3, [sp, #0x4]
    f9cc: 4bed         	ldr	r3, [pc, #0x3b4]        @ 0xfd84 <gst_lg_audio_render_change_state+0x25c4>
    f9ce: 447a         	add	r2, pc
    f9d0: f502 721b    	add.w	r2, r2, #0x26c
    f9d4: 4478         	add	r0, pc
    f9d6: 447b         	add	r3, pc
    f9d8: 9300         	str	r3, [sp]
    f9da: f44f 6336    	mov.w	r3, #0xb60
    f9de: f7f4 eae6    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xba34
    f9e2: f7fe bf8e    	b.w	0xe902 <gst_lg_audio_render_change_state+0x1142> @ imm = #-0x10e4
    f9e6: f8d4 330c    	ldr.w	r3, [r4, #0x30c]
    f9ea: 2103         	movs	r1, #0x3
    f9ec: 4ae6         	ldr	r2, [pc, #0x398]        @ 0xfd88 <gst_lg_audio_render_change_state+0x25c8>
    f9ee: 48e7         	ldr	r0, [pc, #0x39c]        @ 0xfd8c <gst_lg_audio_render_change_state+0x25cc>
    f9f0: 9301         	str	r3, [sp, #0x4]
    f9f2: 4be7         	ldr	r3, [pc, #0x39c]        @ 0xfd90 <gst_lg_audio_render_change_state+0x25d0>
    f9f4: 447a         	add	r2, pc
    f9f6: f502 721b    	add.w	r2, r2, #0x26c
    f9fa: 4478         	add	r0, pc
    f9fc: 447b         	add	r3, pc
    f9fe: 9300         	str	r3, [sp]
    fa00: f640 3367    	movw	r3, #0xb67
    fa04: f7f4 ead2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xba5c
    fa08: f7fe bf52    	b.w	0xe8b0 <gst_lg_audio_render_change_state+0x10f0> @ imm = #-0x115c
    fa0c: f8d4 32a8    	ldr.w	r3, [r4, #0x2a8]
    fa10: 2102         	movs	r1, #0x2
    fa12: 4ae0         	ldr	r2, [pc, #0x380]        @ 0xfd94 <gst_lg_audio_render_change_state+0x25d4>
    fa14: 48e0         	ldr	r0, [pc, #0x380]        @ 0xfd98 <gst_lg_audio_render_change_state+0x25d8>
    fa16: 9301         	str	r3, [sp, #0x4]
    fa18: 4be0         	ldr	r3, [pc, #0x380]        @ 0xfd9c <gst_lg_audio_render_change_state+0x25dc>
    fa1a: 447a         	add	r2, pc
    fa1c: f502 721b    	add.w	r2, r2, #0x26c
    fa20: 4478         	add	r0, pc
    fa22: 447b         	add	r3, pc
    fa24: 9300         	str	r3, [sp]
    fa26: f640 23c3    	movw	r3, #0xac3
    fa2a: f7f4 eac0    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xba80
    fa2e: f7ff bae0    	b.w	0xeff2 <gst_lg_audio_render_change_state+0x1832> @ imm = #-0xa40
    fa32: f8d4 32b8    	ldr.w	r3, [r4, #0x2b8]
    fa36: 2102         	movs	r1, #0x2
    fa38: 4ad9         	ldr	r2, [pc, #0x364]        @ 0xfda0 <gst_lg_audio_render_change_state+0x25e0>
    fa3a: 48da         	ldr	r0, [pc, #0x368]        @ 0xfda4 <gst_lg_audio_render_change_state+0x25e4>
    fa3c: 9301         	str	r3, [sp, #0x4]
    fa3e: 4bda         	ldr	r3, [pc, #0x368]        @ 0xfda8 <gst_lg_audio_render_change_state+0x25e8>
    fa40: 447a         	add	r2, pc
    fa42: f502 721b    	add.w	r2, r2, #0x26c
    fa46: 4478         	add	r0, pc
    fa48: 447b         	add	r3, pc
    fa4a: 9300         	str	r3, [sp]
    fa4c: f640 23f2    	movw	r3, #0xaf2
    fa50: f7f4 eaac    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbaa8
    fa54: f7ff baf7    	b.w	0xf046 <gst_lg_audio_render_change_state+0x1886> @ imm = #-0xa12
    fa58: f8d4 3288    	ldr.w	r3, [r4, #0x288]
    fa5c: 2102         	movs	r1, #0x2
    fa5e: 4ad3         	ldr	r2, [pc, #0x34c]        @ 0xfdac <gst_lg_audio_render_change_state+0x25ec>
    fa60: 48d3         	ldr	r0, [pc, #0x34c]        @ 0xfdb0 <gst_lg_audio_render_change_state+0x25f0>
    fa62: 9301         	str	r3, [sp, #0x4]
    fa64: 4bd3         	ldr	r3, [pc, #0x34c]        @ 0xfdb4 <gst_lg_audio_render_change_state+0x25f4>
    fa66: 447a         	add	r2, pc
    fa68: f502 721b    	add.w	r2, r2, #0x26c
    fa6c: 4478         	add	r0, pc
    fa6e: 447b         	add	r3, pc
    fa70: 9300         	str	r3, [sp]
    fa72: f640 23da    	movw	r3, #0xada
    fa76: f7f4 ea9a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbacc
    fa7a: f7ff ba8c    	b.w	0xef96 <gst_lg_audio_render_change_state+0x17d6> @ imm = #-0xae8
    fa7e: f8d4 327c    	ldr.w	r3, [r4, #0x27c]
    fa82: 2102         	movs	r1, #0x2
    fa84: 4acc         	ldr	r2, [pc, #0x330]        @ 0xfdb8 <gst_lg_audio_render_change_state+0x25f8>
    fa86: 48cd         	ldr	r0, [pc, #0x334]        @ 0xfdbc <gst_lg_audio_render_change_state+0x25fc>
    fa88: 9302         	str	r3, [sp, #0x8]
    fa8a: f8d4 3274    	ldr.w	r3, [r4, #0x274]
    fa8e: 447a         	add	r2, pc
    fa90: f502 721b    	add.w	r2, r2, #0x26c
    fa94: 4478         	add	r0, pc
    fa96: 9301         	str	r3, [sp, #0x4]
    fa98: 4bc9         	ldr	r3, [pc, #0x324]        @ 0xfdc0 <gst_lg_audio_render_change_state+0x2600>
    fa9a: 447b         	add	r3, pc
    fa9c: 9300         	str	r3, [sp]
    fa9e: f640 3339    	movw	r3, #0xb39
    faa2: f7f4 ea84    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbaf8
    faa6: f7ff ba4f    	b.w	0xef48 <gst_lg_audio_render_change_state+0x1788> @ imm = #-0xb62
    faaa: f8d4 3290    	ldr.w	r3, [r4, #0x290]
    faae: 2102         	movs	r1, #0x2
    fab0: 4ac4         	ldr	r2, [pc, #0x310]        @ 0xfdc4 <gst_lg_audio_render_change_state+0x2604>
    fab2: 48c5         	ldr	r0, [pc, #0x314]        @ 0xfdc8 <gst_lg_audio_render_change_state+0x2608>
    fab4: 9301         	str	r3, [sp, #0x4]
    fab6: 4bc5         	ldr	r3, [pc, #0x314]        @ 0xfdcc <gst_lg_audio_render_change_state+0x260c>
    fab8: 447a         	add	r2, pc
    faba: f502 721b    	add.w	r2, r2, #0x26c
    fabe: 4478         	add	r0, pc
    fac0: 447b         	add	r3, pc
    fac2: 9300         	str	r3, [sp]
    fac4: f640 23ce    	movw	r3, #0xace
    fac8: f7f4 ea70    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbb20
    facc: f7ff b967    	b.w	0xed9e <gst_lg_audio_render_change_state+0x15de> @ imm = #-0xd32
    fad0: f8d4 327c    	ldr.w	r3, [r4, #0x27c]
    fad4: 2102         	movs	r1, #0x2
    fad6: 4abe         	ldr	r2, [pc, #0x2f8]        @ 0xfdd0 <gst_lg_audio_render_change_state+0x2610>
    fad8: 48be         	ldr	r0, [pc, #0x2f8]        @ 0xfdd4 <gst_lg_audio_render_change_state+0x2614>
    fada: 9302         	str	r3, [sp, #0x8]
    fadc: f8d4 3274    	ldr.w	r3, [r4, #0x274]
    fae0: 447a         	add	r2, pc
    fae2: f502 721b    	add.w	r2, r2, #0x26c
    fae6: 4478         	add	r0, pc
    fae8: 9301         	str	r3, [sp, #0x4]
    faea: 4bbb         	ldr	r3, [pc, #0x2ec]        @ 0xfdd8 <gst_lg_audio_render_change_state+0x2618>
    faec: 447b         	add	r3, pc
    faee: 9300         	str	r3, [sp]
    faf0: f640 332e    	movw	r3, #0xb2e
    faf4: f7f4 ea5a    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbb4c
    faf8: f7ff b927    	b.w	0xed4a <gst_lg_audio_render_change_state+0x158a> @ imm = #-0xdb2
    fafc: f8d4 32ac    	ldr.w	r3, [r4, #0x2ac]
    fb00: 2102         	movs	r1, #0x2
    fb02: 4ab6         	ldr	r2, [pc, #0x2d8]        @ 0xfddc <gst_lg_audio_render_change_state+0x261c>
    fb04: 48b6         	ldr	r0, [pc, #0x2d8]        @ 0xfde0 <gst_lg_audio_render_change_state+0x2620>
    fb06: 9301         	str	r3, [sp, #0x4]
    fb08: 4bb6         	ldr	r3, [pc, #0x2d8]        @ 0xfde4 <gst_lg_audio_render_change_state+0x2624>
    fb0a: 447a         	add	r2, pc
    fb0c: f502 721b    	add.w	r2, r2, #0x26c
    fb10: 4478         	add	r0, pc
    fb12: 447b         	add	r3, pc
    fb14: 9300         	str	r3, [sp]
    fb16: f44f 632e    	mov.w	r3, #0xae0
    fb1a: f7f4 ea48    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbb70
    fb1e: f7ff b82a    	b.w	0xeb76 <gst_lg_audio_render_change_state+0x13b6> @ imm = #-0xfac
    fb22: f8d4 32c0    	ldr.w	r3, [r4, #0x2c0]
    fb26: 2102         	movs	r1, #0x2
    fb28: 4aaf         	ldr	r2, [pc, #0x2bc]        @ 0xfde8 <gst_lg_audio_render_change_state+0x2628>
    fb2a: 48b0         	ldr	r0, [pc, #0x2c0]        @ 0xfdec <gst_lg_audio_render_change_state+0x262c>
    fb2c: 9301         	str	r3, [sp, #0x4]
    fb2e: 4bb0         	ldr	r3, [pc, #0x2c0]        @ 0xfdf0 <gst_lg_audio_render_change_state+0x2630>
    fb30: 447a         	add	r2, pc
    fb32: f502 721b    	add.w	r2, r2, #0x26c
    fb36: 4478         	add	r0, pc
    fb38: 447b         	add	r3, pc
    fb3a: 9300         	str	r3, [sp]
    fb3c: f640 333f    	movw	r3, #0xb3f
    fb40: f7f4 ea34    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbb98
    fb44: f7fe bff2    	b.w	0xeb2c <gst_lg_audio_render_change_state+0x136c> @ imm = #-0x101c
    fb48: f8d4 328c    	ldr.w	r3, [r4, #0x28c]
    fb4c: 2102         	movs	r1, #0x2
    fb4e: 4aa9         	ldr	r2, [pc, #0x2a4]        @ 0xfdf4 <gst_lg_audio_render_change_state+0x2634>
    fb50: 48a9         	ldr	r0, [pc, #0x2a4]        @ 0xfdf8 <gst_lg_audio_render_change_state+0x2638>
    fb52: 9301         	str	r3, [sp, #0x4]
    fb54: 4ba9         	ldr	r3, [pc, #0x2a4]        @ 0xfdfc <gst_lg_audio_render_change_state+0x263c>
    fb56: 447a         	add	r2, pc
    fb58: f502 721b    	add.w	r2, r2, #0x26c
    fb5c: 4478         	add	r0, pc
    fb5e: 447b         	add	r3, pc
    fb60: 9300         	str	r3, [sp]
    fb62: f640 23d5    	movw	r3, #0xad5
    fb66: f7f4 ea22    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbbbc
    fb6a: f7fe bfba    	b.w	0xeae2 <gst_lg_audio_render_change_state+0x1322> @ imm = #-0x108c
    fb6e: f8d4 32bc    	ldr.w	r3, [r4, #0x2bc]
    fb72: 2102         	movs	r1, #0x2
    fb74: 4aa2         	ldr	r2, [pc, #0x288]        @ 0xfe00 <gst_lg_audio_render_change_state+0x2640>
    fb76: 48a3         	ldr	r0, [pc, #0x28c]        @ 0xfe04 <gst_lg_audio_render_change_state+0x2644>
    fb78: 9301         	str	r3, [sp, #0x4]
    fb7a: 4ba3         	ldr	r3, [pc, #0x28c]        @ 0xfe08 <gst_lg_audio_render_change_state+0x2648>
    fb7c: 447a         	add	r2, pc
    fb7e: f502 721b    	add.w	r2, r2, #0x26c
    fb82: 4478         	add	r0, pc
    fb84: 447b         	add	r3, pc
    fb86: 9300         	str	r3, [sp]
    fb88: f640 3334    	movw	r3, #0xb34
    fb8c: f7f4 ea0e    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbbe4
    fb90: f7fe bf7f    	b.w	0xea92 <gst_lg_audio_render_change_state+0x12d2> @ imm = #-0x1102
    fb94: f8d4 32b0    	ldr.w	r3, [r4, #0x2b0]
    fb98: 2102         	movs	r1, #0x2
    fb9a: 4a9c         	ldr	r2, [pc, #0x270]        @ 0xfe0c <gst_lg_audio_render_change_state+0x264c>
    fb9c: 489c         	ldr	r0, [pc, #0x270]        @ 0xfe10 <gst_lg_audio_render_change_state+0x2650>
    fb9e: 9301         	str	r3, [sp, #0x4]
    fba0: 4b9c         	ldr	r3, [pc, #0x270]        @ 0xfe14 <gst_lg_audio_render_change_state+0x2654>
    fba2: 447a         	add	r2, pc
    fba4: f502 721b    	add.w	r2, r2, #0x26c
    fba8: 4478         	add	r0, pc
    fbaa: 447b         	add	r3, pc
    fbac: 9300         	str	r3, [sp]
    fbae: f640 23e6    	movw	r3, #0xae6
    fbb2: f7f4 e9fc    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbc08
    fbb6: f7ff b8a2    	b.w	0xecfe <gst_lg_audio_render_change_state+0x153e> @ imm = #-0xebc
    fbba: f8d4 32c4    	ldr.w	r3, [r4, #0x2c4]
    fbbe: 2102         	movs	r1, #0x2
    fbc0: 4a95         	ldr	r2, [pc, #0x254]        @ 0xfe18 <gst_lg_audio_render_change_state+0x2658>
    fbc2: 4896         	ldr	r0, [pc, #0x258]        @ 0xfe1c <gst_lg_audio_render_change_state+0x265c>
    fbc4: 9301         	str	r3, [sp, #0x4]
    fbc6: 4b96         	ldr	r3, [pc, #0x258]        @ 0xfe20 <gst_lg_audio_render_change_state+0x2660>
    fbc8: 447a         	add	r2, pc
    fbca: f502 721b    	add.w	r2, r2, #0x26c
    fbce: 4478         	add	r0, pc
    fbd0: 447b         	add	r3, pc
    fbd2: 9300         	str	r3, [sp]
    fbd4: f640 3344    	movw	r3, #0xb44
    fbd8: f7f4 e9e8    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbc30
    fbdc: f7ff b86b    	b.w	0xecb6 <gst_lg_audio_render_change_state+0x14f6> @ imm = #-0xf2a
    fbe0: f8d4 3284    	ldr.w	r3, [r4, #0x284]
    fbe4: 2102         	movs	r1, #0x2
    fbe6: 4a8f         	ldr	r2, [pc, #0x23c]        @ 0xfe24 <gst_lg_audio_render_change_state+0x2664>
    fbe8: 488f         	ldr	r0, [pc, #0x23c]        @ 0xfe28 <gst_lg_audio_render_change_state+0x2668>
    fbea: 9301         	str	r3, [sp, #0x4]
    fbec: 4b8f         	ldr	r3, [pc, #0x23c]        @ 0xfe2c <gst_lg_audio_render_change_state+0x266c>
    fbee: 447a         	add	r2, pc
    fbf0: f502 721b    	add.w	r2, r2, #0x26c
    fbf4: 4478         	add	r0, pc
    fbf6: 447b         	add	r3, pc
    fbf8: 9300         	str	r3, [sp]
    fbfa: f640 23c8    	movw	r3, #0xac8
    fbfe: f7f4 e9d6    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbc54
    fc02: f7ff b82e    	b.w	0xec62 <gst_lg_audio_render_change_state+0x14a2> @ imm = #-0xfa4
    fc06: f8d4 3224    	ldr.w	r3, [r4, #0x224]
    fc0a: 2102         	movs	r1, #0x2
    fc0c: 4a88         	ldr	r2, [pc, #0x220]        @ 0xfe30 <gst_lg_audio_render_change_state+0x2670>
    fc0e: 4889         	ldr	r0, [pc, #0x224]        @ 0xfe34 <gst_lg_audio_render_change_state+0x2674>
    fc10: 9301         	str	r3, [sp, #0x4]
    fc12: 4b89         	ldr	r3, [pc, #0x224]        @ 0xfe38 <gst_lg_audio_render_change_state+0x2678>
    fc14: 447a         	add	r2, pc
    fc16: f502 721b    	add.w	r2, r2, #0x26c
    fc1a: 4478         	add	r0, pc
    fc1c: 447b         	add	r3, pc
    fc1e: 9300         	str	r3, [sp]
    fc20: f640 23fe    	movw	r3, #0xafe
    fc24: f7f4 e9c2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbc7c
    fc28: f7fe bfcb    	b.w	0xebc2 <gst_lg_audio_render_change_state+0x1402> @ imm = #-0x106a
    fc2c: 4a83         	ldr	r2, [pc, #0x20c]        @ 0xfe3c <gst_lg_audio_render_change_state+0x267c>
    fc2e: 2103         	movs	r1, #0x3
    fc30: 4b83         	ldr	r3, [pc, #0x20c]        @ 0xfe40 <gst_lg_audio_render_change_state+0x2680>
    fc32: 447a         	add	r2, pc
    fc34: 447b         	add	r3, pc
    fc36: f502 7212    	add.w	r2, r2, #0x248
    fc3a: 9300         	str	r3, [sp]
    fc3c: f640 33fd    	movw	r3, #0xbfd
    fc40: f7f4 e9b4    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbc98
    fc44: f7fe bd34    	b.w	0xe6b0 <gst_lg_audio_render_change_state+0xef0> @ imm = #-0x1598
    fc48: 4b7e         	ldr	r3, [pc, #0x1f8]        @ 0xfe44 <gst_lg_audio_render_change_state+0x2684>
    fc4a: 447b         	add	r3, pc
    fc4c: f7fe bdec    	b.w	0xe828 <gst_lg_audio_render_change_state+0x1068> @ imm = #-0x1428
    fc50: 4b7d         	ldr	r3, [pc, #0x1f4]        @ 0xfe48 <gst_lg_audio_render_change_state+0x2688>
    fc52: 447b         	add	r3, pc
    fc54: f7fe be05    	b.w	0xe862 <gst_lg_audio_render_change_state+0x10a2> @ imm = #-0x13f6
    fc58: 00 58 47 f8  	.word	0xf8475800
    fc5c: 0d 00 00 00  	.word	0x0000000d
    fc60: 00 ca 9a 3b  	.word	0x3b9aca00
    fc64: 00 00 00 00  	.word	0x00000000
    fc68: 08 04 00 00  	.word	0x00000408
    fc6c: 22 0c 01 00  	.word	0x00010c22
    fc70: f8 e6 00 00  	.word	0x0000e6f8
    fc74: b2 a9 00 00  	.word	0x0000a9b2
    fc78: 0e e1 00 00  	.word	0x0000e10e
    fc7c: ce 0b 01 00  	.word	0x00010bce
    fc80: 6c a9 00 00  	.word	0x0000a96c
    fc84: 86 df 00 00  	.word	0x0000df86
    fc88: 72 0b 01 00  	.word	0x00010b72
    fc8c: 10 a9 00 00  	.word	0x0000a910
    fc90: ae e1 00 00  	.word	0x0000e1ae
    fc94: 1e 0b 01 00  	.word	0x00010b1e
    fc98: bc a8 00 00  	.word	0x0000a8bc
    fc9c: 5a 2f 02 00  	.word	0x00022f5a
    fca0: fc eb 00 00  	.word	0x0000ebfc
    fca4: b6 0a 01 00  	.word	0x00010ab6
    fca8: 54 a8 00 00  	.word	0x0000a854
    fcac: 0e 2f 02 00  	.word	0x00022f0e
    fcb0: 36 e6 00 00  	.word	0x0000e636
    fcb4: 66 0a 01 00  	.word	0x00010a66
    fcb8: 04 a8 00 00  	.word	0x0000a804
    fcbc: 2e 0a 01 00  	.word	0x00010a2e
    fcc0: 80 dd 00 00  	.word	0x0000dd80
    fcc4: e0 bb 00 00  	.word	0x0000bbe0
    fcc8: d6 bb 00 00  	.word	0x0000bbd6
    fccc: e6 09 01 00  	.word	0x000109e6
    fcd0: 88 eb 00 00  	.word	0x0000eb88
    fcd4: 12 2e 02 00  	.word	0x00022e12
    fcd8: 94 07 01 00  	.word	0x00010794
    fcdc: f2 2b 02 00  	.word	0x00022bf2
    fce0: de e6 00 00  	.word	0x0000e6de
    fce4: 04 e5 00 00  	.word	0x0000e504
    fce8: f0 04 01 00  	.word	0x000104f0
    fcec: 8a a2 00 00  	.word	0x0000a28a
    fcf0: 18 e0 00 00  	.word	0x0000e018
    fcf4: 90 04 01 00  	.word	0x00010490
    fcf8: 2e a2 00 00  	.word	0x0000a22e
    fcfc: 9e db 00 00  	.word	0x0000db9e
    fd00: 56 04 01 00  	.word	0x00010456
    fd04: f4 a1 00 00  	.word	0x0000a1f4
    fd08: 60 e2 00 00  	.word	0x0000e260
    fd0c: 3a ca 00 00  	.word	0x0000ca3a
    fd10: 88 03 01 00  	.word	0x00010388
    fd14: 26 a1 00 00  	.word	0x0000a126
    fd18: 18 ca 00 00  	.word	0x0000ca18
    fd1c: 40 03 01 00  	.word	0x00010340
    fd20: de a0 00 00  	.word	0x0000a0de
    fd24: 92 27 02 00  	.word	0x00022792
    fd28: 84 da 00 00  	.word	0x0000da84
    fd2c: f2 02 01 00  	.word	0x000102f2
    fd30: 90 a0 00 00  	.word	0x0000a090
    fd34: c0 02 01 00  	.word	0x000102c0
    fd38: 56 de 00 00  	.word	0x0000de56
    fd3c: a2 02 01 00  	.word	0x000102a2
    fd40: 1c d6 00 00  	.word	0x0000d61c
    fd44: 7c e0 00 00  	.word	0x0000e07c
    fd48: e4 26 02 00  	.word	0x000226e4
    fd4c: 56 02 01 00  	.word	0x00010256
    fd50: bc 26 02 00  	.word	0x000226bc
    fd54: 3a d8 00 00  	.word	0x0000d83a
    fd58: 32 02 01 00  	.word	0x00010232
    fd5c: f0 dd 00 00  	.word	0x0000ddf0
    fd60: 94 26 02 00  	.word	0x00022694
    fd64: 12 02 01 00  	.word	0x00010212
    fd68: 6c de 00 00  	.word	0x0000de6c
    fd6c: 74 26 02 00  	.word	0x00022674
    fd70: ec 01 01 00  	.word	0x000101ec
    fd74: 52 26 02 00  	.word	0x00022652
    fd78: 80 d8 00 00  	.word	0x0000d880
    fd7c: c6 01 01 00  	.word	0x000101c6
    fd80: 2c 26 02 00  	.word	0x0002262c
    fd84: 8e de 00 00  	.word	0x0000de8e
    fd88: a0 01 01 00  	.word	0x000101a0
    fd8c: 06 26 02 00  	.word	0x00022606
    fd90: b4 de 00 00  	.word	0x0000deb4
    fd94: 7a 01 01 00  	.word	0x0001017a
    fd98: e0 25 02 00  	.word	0x000225e0
    fd9c: 3e d5 00 00  	.word	0x0000d53e
    fda0: 54 01 01 00  	.word	0x00010154
    fda4: ba 25 02 00  	.word	0x000225ba
    fda8: 8c d7 00 00  	.word	0x0000d78c
    fdac: 2e 01 01 00  	.word	0x0001012e
    fdb0: 94 25 02 00  	.word	0x00022594
    fdb4: 1a d6 00 00  	.word	0x0000d61a
    fdb8: 06 01 01 00  	.word	0x00010106
    fdbc: 6c 25 02 00  	.word	0x0002256c
    fdc0: a6 db 00 00  	.word	0x0000dba6
    fdc4: dc 00 01 00  	.word	0x000100dc
    fdc8: 42 25 02 00  	.word	0x00022542
    fdcc: 40 d5 00 00  	.word	0x0000d540
    fdd0: b4 00 01 00  	.word	0x000100b4
    fdd4: 1a 25 02 00  	.word	0x0002251a
    fdd8: ac da 00 00  	.word	0x0000daac
    fddc: 8a 00 01 00  	.word	0x0001008a
    fde0: f0 24 02 00  	.word	0x000224f0
    fde4: c6 d5 00 00  	.word	0x0000d5c6
    fde8: 64 00 01 00  	.word	0x00010064
    fdec: ca 24 02 00  	.word	0x000224ca
    fdf0: 5c db 00 00  	.word	0x0000db5c
    fdf4: 3e 00 01 00  	.word	0x0001003e
    fdf8: a4 24 02 00  	.word	0x000224a4
    fdfc: e6 d4 00 00  	.word	0x0000d4e6
    fe00: 18 00 01 00  	.word	0x00010018
    fe04: 7e 24 02 00  	.word	0x0002247e
    fe08: 78 da 00 00  	.word	0x0000da78
    fe0c: f2 ff 00 00  	.word	0x0000fff2
    fe10: 58 24 02 00  	.word	0x00022458
    fe14: 82 d5 00 00  	.word	0x0000d582
    fe18: cc ff 00 00  	.word	0x0000ffcc
    fe1c: 32 24 02 00  	.word	0x00022432
    fe20: 04 db 00 00  	.word	0x0000db04
    fe24: a6 ff 00 00  	.word	0x0000ffa6
    fe28: 0c 24 02 00  	.word	0x0002240c
    fe2c: b6 d3 00 00  	.word	0x0000d3b6
    fe30: 80 ff 00 00  	.word	0x0000ff80
    fe34: e6 23 02 00  	.word	0x000223e6
    fe38: 78 d6 00 00  	.word	0x0000d678
    fe3c: 62 ff 00 00  	.word	0x0000ff62
    fe40: ec df 00 00  	.word	0x0000dfec
    fe44: 3a d2 00 00  	.word	0x0000d23a
    fe48: 32 d2 00 00  	.word	0x0000d232
    fe4c: 49ff         	ldr	r1, [pc, #0x3fc]        @ 0x1024c <gst_lg_audio_render_change_state+0x2a8c>
    fe4e: 4650         	mov	r0, r10
    fe50: 4479         	add	r1, pc
    fe52: f7f4 ea22    	blx	0x4298 <strcmp@plt>     @ imm = #-0xbbbc
    fe56: 2800         	cmp	r0, #0x0
    fe58: d178         	bne	0xff4c <gst_lg_audio_render_change_state+0x278c> @ imm = #0xf0
    fe5a: 48fd         	ldr	r0, [pc, #0x3f4]        @ 0x10250 <gst_lg_audio_render_change_state+0x2a90>
    fe5c: 4478         	add	r0, pc
    fe5e: 68c3         	ldr	r3, [r0, #0xc]
    fe60: 681b         	ldr	r3, [r3]
    fe62: 071b         	lsls	r3, r3, #0x1c
    fe64: f100 8113    	bmi.w	0x1008e <gst_lg_audio_render_change_state+0x28ce> @ imm = #0x226
    fe68: 6833         	ldr	r3, [r6]
    fe6a: 2b03         	cmp	r3, #0x3
    fe6c: d913         	bls	0xfe96 <gst_lg_audio_render_change_state+0x26d6> @ imm = #0x26
    fe6e: 4bf9         	ldr	r3, [pc, #0x3e4]        @ 0x10254 <gst_lg_audio_render_change_state+0x2a94>
    fe70: 2104         	movs	r1, #0x4
    fe72: 4af9         	ldr	r2, [pc, #0x3e4]        @ 0x10258 <gst_lg_audio_render_change_state+0x2a98>
    fe74: f858 3003    	ldr.w	r3, [r8, r3]
    fe78: 447a         	add	r2, pc
    fe7a: 9401         	str	r4, [sp, #0x4]
    fe7c: 6818         	ldr	r0, [r3]
    fe7e: 4bf7         	ldr	r3, [pc, #0x3dc]        @ 0x1025c <gst_lg_audio_render_change_state+0x2a9c>
    fe80: 9202         	str	r2, [sp, #0x8]
    fe82: f640 320e    	movw	r2, #0xb0e
    fe86: 9200         	str	r2, [sp]
    fe88: 4af5         	ldr	r2, [pc, #0x3d4]        @ 0x10260 <gst_lg_audio_render_change_state+0x2aa0>
    fe8a: 447b         	add	r3, pc
    fe8c: f503 731b    	add.w	r3, r3, #0x26c
    fe90: 447a         	add	r2, pc
    fe92: f7f3 eeec    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xc228
    fe96: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    fe9a: 2108         	movs	r1, #0x8
    fe9c: f7fe becc    	b.w	0xec38 <gst_lg_audio_render_change_state+0x1478> @ imm = #-0x1268
    fea0: 2063         	movs	r0, #0x63
    fea2: f64c 12ff    	movw	r2, #0xc9ff
    fea6: f6c3 329a    	movt	r2, #0x3b9a
    feaa: 4681         	mov	r9, r0
    feac: 901e         	str	r0, [sp, #0x78]
    feae: f7ff ba9d    	b.w	0xf3ec <gst_lg_audio_render_change_state+0x1c2c> @ imm = #-0xac6
    feb2: f64c 12ff    	movw	r2, #0xc9ff
    feb6: 2363         	movs	r3, #0x63
    feb8: f6c3 329a    	movt	r2, #0x3b9a
    febc: 931c         	str	r3, [sp, #0x70]
    febe: 9219         	str	r2, [sp, #0x64]
    fec0: e9cd 331a    	strd	r3, r3, [sp, #104]
    fec4: f7ff b9f4    	b.w	0xf2b0 <gst_lg_audio_render_change_state+0x1af0> @ imm = #-0xc18
    fec8: f64c 12ff    	movw	r2, #0xc9ff
    fecc: 2363         	movs	r3, #0x63
    fece: f6c3 329a    	movt	r2, #0x3b9a
    fed2: 9320         	str	r3, [sp, #0x80]
    fed4: 9219         	str	r2, [sp, #0x64]
    fed6: e9cd 331e    	strd	r3, r3, [sp, #120]
    feda: f7fe b81d    	b.w	0xdf18 <gst_lg_audio_render_change_state+0x758> @ imm = #-0x1fc6
    fede: 2363         	movs	r3, #0x63
    fee0: f64c 1bff    	movw	r11, #0xc9ff
    fee4: f6c3 3b9a    	movt	r11, #0x3b9a
    fee8: 9318         	str	r3, [sp, #0x60]
    feea: e9cd 3316    	strd	r3, r3, [sp, #88]
    feee: f7ff bafd    	b.w	0xf4ec <gst_lg_audio_render_change_state+0x1d2c> @ imm = #-0xa06
    fef2: f64c 12ff    	movw	r2, #0xc9ff
    fef6: 2363         	movs	r3, #0x63
    fef8: f6c3 329a    	movt	r2, #0x3b9a
    fefc: 931c         	str	r3, [sp, #0x70]
    fefe: 9219         	str	r2, [sp, #0x64]
    ff00: e9cd 331a    	strd	r3, r3, [sp, #104]
    ff04: f7fe ba02    	b.w	0xe30c <gst_lg_audio_render_change_state+0xb4c> @ imm = #-0x1bfc
    ff08: f04f 0963    	mov.w	r9, #0x63
    ff0c: f64c 1aff    	movw	r10, #0xc9ff
    ff10: f6c3 3a9a    	movt	r10, #0x3b9a
    ff14: e9cd 9924    	strd	r9, r9, [sp, #144]
    ff18: f7fe b8a7    	b.w	0xe06a <gst_lg_audio_render_change_state+0x8aa> @ imm = #-0x1eb2
    ff1c: f64c 12ff    	movw	r2, #0xc9ff
    ff20: 2363         	movs	r3, #0x63
    ff22: f6c3 329a    	movt	r2, #0x3b9a
    ff26: 9323         	str	r3, [sp, #0x8c]
    ff28: 921a         	str	r2, [sp, #0x68]
    ff2a: e9cd 3321    	strd	r3, r3, [sp, #132]
    ff2e: f7fe b843    	b.w	0xdfb8 <gst_lg_audio_render_change_state+0x7f8> @ imm = #-0x1f7a
    ff32: 4acc         	ldr	r2, [pc, #0x330]        @ 0x10264 <gst_lg_audio_render_change_state+0x2aa4>
    ff34: 2103         	movs	r1, #0x3
    ff36: 4bcc         	ldr	r3, [pc, #0x330]        @ 0x10268 <gst_lg_audio_render_change_state+0x2aa8>
    ff38: 447a         	add	r2, pc
    ff3a: 447b         	add	r3, pc
    ff3c: f502 721b    	add.w	r2, r2, #0x26c
    ff40: 9300         	str	r3, [sp]
    ff42: f640 3302    	movw	r3, #0xb02
    ff46: f7f4 e832    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xbf9c
    ff4a: e496         	b	0xf87a <gst_lg_audio_render_change_state+0x20ba> @ imm = #-0x6d4
    ff4c: 49c7         	ldr	r1, [pc, #0x31c]        @ 0x1026c <gst_lg_audio_render_change_state+0x2aac>
    ff4e: 4650         	mov	r0, r10
    ff50: 4479         	add	r1, pc
    ff52: f7f4 e9a2    	blx	0x4298 <strcmp@plt>     @ imm = #-0xbcbc
    ff56: 2800         	cmp	r0, #0x0
    ff58: d13e         	bne	0xffd8 <gst_lg_audio_render_change_state+0x2818> @ imm = #0x7c
    ff5a: 48c5         	ldr	r0, [pc, #0x314]        @ 0x10270 <gst_lg_audio_render_change_state+0x2ab0>
    ff5c: 4478         	add	r0, pc
    ff5e: 68c3         	ldr	r3, [r0, #0xc]
    ff60: 681b         	ldr	r3, [r3]
    ff62: 0719         	lsls	r1, r3, #0x1c
    ff64: f100 80a0    	bmi.w	0x100a8 <gst_lg_audio_render_change_state+0x28e8> @ imm = #0x140
    ff68: 6833         	ldr	r3, [r6]
    ff6a: 2b03         	cmp	r3, #0x3
    ff6c: d913         	bls	0xff96 <gst_lg_audio_render_change_state+0x27d6> @ imm = #0x26
    ff6e: 4bb9         	ldr	r3, [pc, #0x2e4]        @ 0x10254 <gst_lg_audio_render_change_state+0x2a94>
    ff70: 2104         	movs	r1, #0x4
    ff72: 4ac0         	ldr	r2, [pc, #0x300]        @ 0x10274 <gst_lg_audio_render_change_state+0x2ab4>
    ff74: f858 3003    	ldr.w	r3, [r8, r3]
    ff78: 447a         	add	r2, pc
    ff7a: 9401         	str	r4, [sp, #0x4]
    ff7c: 6818         	ldr	r0, [r3]
    ff7e: 4bbe         	ldr	r3, [pc, #0x2f8]        @ 0x10278 <gst_lg_audio_render_change_state+0x2ab8>
    ff80: 9202         	str	r2, [sp, #0x8]
    ff82: f640 3213    	movw	r2, #0xb13
    ff86: 9200         	str	r2, [sp]
    ff88: 4abc         	ldr	r2, [pc, #0x2f0]        @ 0x1027c <gst_lg_audio_render_change_state+0x2abc>
    ff8a: 447b         	add	r3, pc
    ff8c: f503 731b    	add.w	r3, r3, #0x26c
    ff90: 447a         	add	r2, pc
    ff92: f7f3 ee6c    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xc328
    ff96: f8d4 0200    	ldr.w	r0, [r4, #0x200]
    ff9a: 2103         	movs	r1, #0x3
    ff9c: f7fe be4c    	b.w	0xec38 <gst_lg_audio_render_change_state+0x1478> @ imm = #-0x1368
    ffa0: 4ab7         	ldr	r2, [pc, #0x2dc]        @ 0x10280 <gst_lg_audio_render_change_state+0x2ac0>
    ffa2: 2100         	movs	r1, #0x0
    ffa4: 4bb7         	ldr	r3, [pc, #0x2dc]        @ 0x10284 <gst_lg_audio_render_change_state+0x2ac4>
    ffa6: 447a         	add	r2, pc
    ffa8: 447b         	add	r3, pc
    ffaa: f502 7212    	add.w	r2, r2, #0x248
    ffae: 9300         	str	r3, [sp]
    ffb0: f640 4329    	movw	r3, #0xc29
    ffb4: f7f3 effa    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xc00c
    ffb8: f7ff b87b    	b.w	0xf0b2 <gst_lg_audio_render_change_state+0x18f2> @ imm = #-0xf0a
    ffbc: 4ab2         	ldr	r2, [pc, #0x2c8]        @ 0x10288 <gst_lg_audio_render_change_state+0x2ac8>
    ffbe: 2103         	movs	r1, #0x3
    ffc0: 4bb2         	ldr	r3, [pc, #0x2c8]        @ 0x1028c <gst_lg_audio_render_change_state+0x2acc>
    ffc2: 447a         	add	r2, pc
    ffc4: 447b         	add	r3, pc
    ffc6: f502 721b    	add.w	r2, r2, #0x26c
    ffca: 9300         	str	r3, [sp]
    ffcc: f640 3307    	movw	r3, #0xb07
    ffd0: f7f3 efec    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xc028
    ffd4: f7fe be16    	b.w	0xec04 <gst_lg_audio_render_change_state+0x1444> @ imm = #-0x13d4
    ffd8: 49ad         	ldr	r1, [pc, #0x2b4]        @ 0x10290 <gst_lg_audio_render_change_state+0x2ad0>
    ffda: 4650         	mov	r0, r10
    ffdc: 4479         	add	r1, pc
    ffde: f7f4 e95c    	blx	0x4298 <strcmp@plt>     @ imm = #-0xbd48
    ffe2: 2800         	cmp	r0, #0x0
    ffe4: f000 8098    	beq.w	0x10118 <gst_lg_audio_render_change_state+0x2958> @ imm = #0x130
    ffe8: 49aa         	ldr	r1, [pc, #0x2a8]        @ 0x10294 <gst_lg_audio_render_change_state+0x2ad4>
    ffea: 4650         	mov	r0, r10
    ffec: 4479         	add	r1, pc
    ffee: f7f4 e954    	blx	0x4298 <strcmp@plt>     @ imm = #-0xbd58
    fff2: 2800         	cmp	r0, #0x0
    fff4: d165         	bne	0x100c2 <gst_lg_audio_render_change_state+0x2902> @ imm = #0xca
    fff6: 48a8         	ldr	r0, [pc, #0x2a0]        @ 0x10298 <gst_lg_audio_render_change_state+0x2ad8>
    fff8: 4478         	add	r0, pc
    fffa: 68c3         	ldr	r3, [r0, #0xc]
    fffc: 681b         	ldr	r3, [r3]
    fffe: 071a         	lsls	r2, r3, #0x1c
   10000: f100 80e6    	bmi.w	0x101d0 <gst_lg_audio_render_change_state+0x2a10> @ imm = #0x1cc
   10004: 6833         	ldr	r3, [r6]
   10006: 2b03         	cmp	r3, #0x3
   10008: d913         	bls	0x10032 <gst_lg_audio_render_change_state+0x2872> @ imm = #0x26
   1000a: 4b92         	ldr	r3, [pc, #0x248]        @ 0x10254 <gst_lg_audio_render_change_state+0x2a94>
   1000c: 2104         	movs	r1, #0x4
   1000e: 4aa3         	ldr	r2, [pc, #0x28c]        @ 0x1029c <gst_lg_audio_render_change_state+0x2adc>
   10010: f858 3003    	ldr.w	r3, [r8, r3]
   10014: 447a         	add	r2, pc
   10016: 9401         	str	r4, [sp, #0x4]
   10018: 6818         	ldr	r0, [r3]
   1001a: 4ba1         	ldr	r3, [pc, #0x284]        @ 0x102a0 <gst_lg_audio_render_change_state+0x2ae0>
   1001c: 9202         	str	r2, [sp, #0x8]
   1001e: f640 321a    	movw	r2, #0xb1a
   10022: 9200         	str	r2, [sp]
   10024: 4a9f         	ldr	r2, [pc, #0x27c]        @ 0x102a4 <gst_lg_audio_render_change_state+0x2ae4>
   10026: 447b         	add	r3, pc
   10028: f503 731b    	add.w	r3, r3, #0x26c
   1002c: 447a         	add	r2, pc
   1002e: f7f3 ee1e    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xc3c4
   10032: f8d4 0200    	ldr.w	r0, [r4, #0x200]
   10036: 2101         	movs	r1, #0x1
   10038: f7fe bdfe    	b.w	0xec38 <gst_lg_audio_render_change_state+0x1478> @ imm = #-0x1404
   1003c: f04f 0a63    	mov.w	r10, #0x63
   10040: f64c 19ff    	movw	r9, #0xc9ff
   10044: f6c3 399a    	movt	r9, #0x3b9a
   10048: e9cd aa23    	strd	r10, r10, [sp, #140]
   1004c: f7fe ba0f    	b.w	0xe46e <gst_lg_audio_render_change_state+0xcae> @ imm = #-0x1be2
   10050: f64c 12ff    	movw	r2, #0xc9ff
   10054: 2363         	movs	r3, #0x63
   10056: f6c3 329a    	movt	r2, #0x3b9a
   1005a: 931c         	str	r3, [sp, #0x70]
   1005c: 9219         	str	r2, [sp, #0x64]
   1005e: e9cd 331a    	strd	r3, r3, [sp, #104]
   10062: f7ff baa1    	b.w	0xf5a8 <gst_lg_audio_render_change_state+0x1de8> @ imm = #-0xabe
   10066: f64c 12ff    	movw	r2, #0xc9ff
   1006a: 2363         	movs	r3, #0x63
   1006c: f6c3 329a    	movt	r2, #0x3b9a
   10070: 9322         	str	r3, [sp, #0x88]
   10072: 9217         	str	r2, [sp, #0x5c]
   10074: e9cd 3320    	strd	r3, r3, [sp, #128]
   10078: f7fe b99c    	b.w	0xe3b4 <gst_lg_audio_render_change_state+0xbf4> @ imm = #-0x1cc8
   1007c: 2063         	movs	r0, #0x63
   1007e: f64c 12ff    	movw	r2, #0xc9ff
   10082: f6c3 329a    	movt	r2, #0x3b9a
   10086: 4681         	mov	r9, r0
   10088: 901d         	str	r0, [sp, #0x74]
   1008a: f7ff bae6    	b.w	0xf65a <gst_lg_audio_render_change_state+0x1e9a> @ imm = #-0xa34
   1008e: 4a86         	ldr	r2, [pc, #0x218]        @ 0x102a8 <gst_lg_audio_render_change_state+0x2ae8>
   10090: 2103         	movs	r1, #0x3
   10092: 4b86         	ldr	r3, [pc, #0x218]        @ 0x102ac <gst_lg_audio_render_change_state+0x2aec>
   10094: 447a         	add	r2, pc
   10096: 447b         	add	r3, pc
   10098: f502 721b    	add.w	r2, r2, #0x26c
   1009c: 9300         	str	r3, [sp]
   1009e: f640 330e    	movw	r3, #0xb0e
   100a2: f7f3 ef84    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xc0f8
   100a6: e6df         	b	0xfe68 <gst_lg_audio_render_change_state+0x26a8> @ imm = #-0x242
   100a8: 4a81         	ldr	r2, [pc, #0x204]        @ 0x102b0 <gst_lg_audio_render_change_state+0x2af0>
   100aa: 2103         	movs	r1, #0x3
   100ac: 4b81         	ldr	r3, [pc, #0x204]        @ 0x102b4 <gst_lg_audio_render_change_state+0x2af4>
   100ae: 447a         	add	r2, pc
   100b0: 447b         	add	r3, pc
   100b2: f502 721b    	add.w	r2, r2, #0x26c
   100b6: 9300         	str	r3, [sp]
   100b8: f640 3313    	movw	r3, #0xb13
   100bc: f7f3 ef76    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xc114
   100c0: e752         	b	0xff68 <gst_lg_audio_render_change_state+0x27a8> @ imm = #-0x15c
   100c2: 497d         	ldr	r1, [pc, #0x1f4]        @ 0x102b8 <gst_lg_audio_render_change_state+0x2af8>
   100c4: 4650         	mov	r0, r10
   100c6: 4479         	add	r1, pc
   100c8: f7f4 e8e6    	blx	0x4298 <strcmp@plt>     @ imm = #-0xbe34
   100cc: bb58         	cbnz	r0, 0x10126 <gst_lg_audio_render_change_state+0x2966> @ imm = #0x56
   100ce: 487b         	ldr	r0, [pc, #0x1ec]        @ 0x102bc <gst_lg_audio_render_change_state+0x2afc>
   100d0: 4478         	add	r0, pc
   100d2: 68c3         	ldr	r3, [r0, #0xc]
   100d4: 681b         	ldr	r3, [r3]
   100d6: 071b         	lsls	r3, r3, #0x1c
   100d8: f100 8087    	bmi.w	0x101ea <gst_lg_audio_render_change_state+0x2a2a> @ imm = #0x10e
   100dc: 6833         	ldr	r3, [r6]
   100de: 2b03         	cmp	r3, #0x3
   100e0: d913         	bls	0x1010a <gst_lg_audio_render_change_state+0x294a> @ imm = #0x26
   100e2: 4b5c         	ldr	r3, [pc, #0x170]        @ 0x10254 <gst_lg_audio_render_change_state+0x2a94>
   100e4: 2104         	movs	r1, #0x4
   100e6: 4a76         	ldr	r2, [pc, #0x1d8]        @ 0x102c0 <gst_lg_audio_render_change_state+0x2b00>
   100e8: f858 3003    	ldr.w	r3, [r8, r3]
   100ec: 447a         	add	r2, pc
   100ee: 9401         	str	r4, [sp, #0x4]
   100f0: 6818         	ldr	r0, [r3]
   100f2: 4b74         	ldr	r3, [pc, #0x1d0]        @ 0x102c4 <gst_lg_audio_render_change_state+0x2b04>
   100f4: 9202         	str	r2, [sp, #0x8]
   100f6: f44f 6232    	mov.w	r2, #0xb20
   100fa: 9200         	str	r2, [sp]
   100fc: 4a72         	ldr	r2, [pc, #0x1c8]        @ 0x102c8 <gst_lg_audio_render_change_state+0x2b08>
   100fe: 447b         	add	r3, pc
   10100: f503 731b    	add.w	r3, r3, #0x26c
   10104: 447a         	add	r2, pc
   10106: f7f3 edb2    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xc49c
   1010a: f8d4 0200    	ldr.w	r0, [r4, #0x200]
   1010e: 2106         	movs	r1, #0x6
   10110: f7f4 e886    	blx	0x4220 <lgao_set_apptype@plt> @ imm = #-0xbef4
   10114: f7fe bab6    	b.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x1a94
   10118: f8d4 0200    	ldr.w	r0, [r4, #0x200]
   1011c: 2105         	movs	r1, #0x5
   1011e: f7f4 e880    	blx	0x4220 <lgao_set_apptype@plt> @ imm = #-0xbf00
   10122: f7fe baaf    	b.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x1aa2
   10126: 4969         	ldr	r1, [pc, #0x1a4]        @ 0x102cc <gst_lg_audio_render_change_state+0x2b0c>
   10128: 4650         	mov	r0, r10
   1012a: 4479         	add	r1, pc
   1012c: f7f4 e8b4    	blx	0x4298 <strcmp@plt>     @ imm = #-0xbe98
   10130: bb18         	cbnz	r0, 0x1017a <gst_lg_audio_render_change_state+0x29ba> @ imm = #0x46
   10132: 4b67         	ldr	r3, [pc, #0x19c]        @ 0x102d0 <gst_lg_audio_render_change_state+0x2b10>
   10134: 447b         	add	r3, pc
   10136: 68db         	ldr	r3, [r3, #0xc]
   10138: 681b         	ldr	r3, [r3]
   1013a: 0718         	lsls	r0, r3, #0x1c
   1013c: d476         	bmi	0x1022c <gst_lg_audio_render_change_state+0x2a6c> @ imm = #0xec
   1013e: 6833         	ldr	r3, [r6]
   10140: 2b03         	cmp	r3, #0x3
   10142: d913         	bls	0x1016c <gst_lg_audio_render_change_state+0x29ac> @ imm = #0x26
   10144: 4b43         	ldr	r3, [pc, #0x10c]        @ 0x10254 <gst_lg_audio_render_change_state+0x2a94>
   10146: 2104         	movs	r1, #0x4
   10148: 4a62         	ldr	r2, [pc, #0x188]        @ 0x102d4 <gst_lg_audio_render_change_state+0x2b14>
   1014a: f858 3003    	ldr.w	r3, [r8, r3]
   1014e: 447a         	add	r2, pc
   10150: 9401         	str	r4, [sp, #0x4]
   10152: 6818         	ldr	r0, [r3]
   10154: 4b60         	ldr	r3, [pc, #0x180]        @ 0x102d8 <gst_lg_audio_render_change_state+0x2b18>
   10156: 9202         	str	r2, [sp, #0x8]
   10158: f640 3223    	movw	r2, #0xb23
   1015c: 9200         	str	r2, [sp]
   1015e: 4a5f         	ldr	r2, [pc, #0x17c]        @ 0x102dc <gst_lg_audio_render_change_state+0x2b1c>
   10160: 447b         	add	r3, pc
   10162: f503 731b    	add.w	r3, r3, #0x26c
   10166: 447a         	add	r2, pc
   10168: f7f3 ed80    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xc500
   1016c: f8d4 0200    	ldr.w	r0, [r4, #0x200]
   10170: 2107         	movs	r1, #0x7
   10172: f7f4 e856    	blx	0x4220 <lgao_set_apptype@plt> @ imm = #-0xbf54
   10176: f7fe ba85    	b.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x1af6
   1017a: 4959         	ldr	r1, [pc, #0x164]        @ 0x102e0 <gst_lg_audio_render_change_state+0x2b20>
   1017c: 4650         	mov	r0, r10
   1017e: 4479         	add	r1, pc
   10180: f7f4 e88a    	blx	0x4298 <strcmp@plt>     @ imm = #-0xbeec
   10184: 2800         	cmp	r0, #0x0
   10186: d14a         	bne	0x1021e <gst_lg_audio_render_change_state+0x2a5e> @ imm = #0x94
   10188: 4856         	ldr	r0, [pc, #0x158]        @ 0x102e4 <gst_lg_audio_render_change_state+0x2b24>
   1018a: 4478         	add	r0, pc
   1018c: 68c3         	ldr	r3, [r0, #0xc]
   1018e: 681b         	ldr	r3, [r3]
   10190: 0719         	lsls	r1, r3, #0x1c
   10192: d437         	bmi	0x10204 <gst_lg_audio_render_change_state+0x2a44> @ imm = #0x6e
   10194: 6833         	ldr	r3, [r6]
   10196: 2b03         	cmp	r3, #0x3
   10198: d913         	bls	0x101c2 <gst_lg_audio_render_change_state+0x2a02> @ imm = #0x26
   1019a: 4b2e         	ldr	r3, [pc, #0xb8]         @ 0x10254 <gst_lg_audio_render_change_state+0x2a94>
   1019c: 2104         	movs	r1, #0x4
   1019e: 4a52         	ldr	r2, [pc, #0x148]        @ 0x102e8 <gst_lg_audio_render_change_state+0x2b28>
   101a0: f858 3003    	ldr.w	r3, [r8, r3]
   101a4: 447a         	add	r2, pc
   101a6: 9401         	str	r4, [sp, #0x4]
   101a8: 6818         	ldr	r0, [r3]
   101aa: 4b50         	ldr	r3, [pc, #0x140]        @ 0x102ec <gst_lg_audio_render_change_state+0x2b2c>
   101ac: 9202         	str	r2, [sp, #0x8]
   101ae: f640 3226    	movw	r2, #0xb26
   101b2: 9200         	str	r2, [sp]
   101b4: 4a4e         	ldr	r2, [pc, #0x138]        @ 0x102f0 <gst_lg_audio_render_change_state+0x2b30>
   101b6: 447b         	add	r3, pc
   101b8: f503 731b    	add.w	r3, r3, #0x26c
   101bc: 447a         	add	r2, pc
   101be: f7f3 ed56    	blx	0x3c6c <gst_debug_log@plt> @ imm = #-0xc554
   101c2: f8d4 0200    	ldr.w	r0, [r4, #0x200]
   101c6: 2109         	movs	r1, #0x9
   101c8: f7f4 e82a    	blx	0x4220 <lgao_set_apptype@plt> @ imm = #-0xbfac
   101cc: f7fe ba5a    	b.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x1b4c
   101d0: 4a48         	ldr	r2, [pc, #0x120]        @ 0x102f4 <gst_lg_audio_render_change_state+0x2b34>
   101d2: 2103         	movs	r1, #0x3
   101d4: 4b48         	ldr	r3, [pc, #0x120]        @ 0x102f8 <gst_lg_audio_render_change_state+0x2b38>
   101d6: 447a         	add	r2, pc
   101d8: 447b         	add	r3, pc
   101da: f502 721b    	add.w	r2, r2, #0x26c
   101de: 9300         	str	r3, [sp]
   101e0: f640 331a    	movw	r3, #0xb1a
   101e4: f7f3 eee2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xc23c
   101e8: e70c         	b	0x10004 <gst_lg_audio_render_change_state+0x2844> @ imm = #-0x1e8
   101ea: 4a44         	ldr	r2, [pc, #0x110]        @ 0x102fc <gst_lg_audio_render_change_state+0x2b3c>
   101ec: 2103         	movs	r1, #0x3
   101ee: 4b44         	ldr	r3, [pc, #0x110]        @ 0x10300 <gst_lg_audio_render_change_state+0x2b40>
   101f0: 447a         	add	r2, pc
   101f2: 447b         	add	r3, pc
   101f4: f502 721b    	add.w	r2, r2, #0x26c
   101f8: 9300         	str	r3, [sp]
   101fa: f44f 6332    	mov.w	r3, #0xb20
   101fe: f7f3 eed6    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xc254
   10202: e76b         	b	0x100dc <gst_lg_audio_render_change_state+0x291c> @ imm = #-0x12a
   10204: 4a3f         	ldr	r2, [pc, #0xfc]         @ 0x10304 <gst_lg_audio_render_change_state+0x2b44>
   10206: 2103         	movs	r1, #0x3
   10208: 4b3f         	ldr	r3, [pc, #0xfc]         @ 0x10308 <gst_lg_audio_render_change_state+0x2b48>
   1020a: 447a         	add	r2, pc
   1020c: 447b         	add	r3, pc
   1020e: f502 721b    	add.w	r2, r2, #0x26c
   10212: 9300         	str	r3, [sp]
   10214: f640 3326    	movw	r3, #0xb26
   10218: f7f3 eec8    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xc270
   1021c: e7ba         	b	0x10194 <gst_lg_audio_render_change_state+0x29d4> @ imm = #-0x8c
   1021e: f8d4 0200    	ldr.w	r0, [r4, #0x200]
   10222: 2100         	movs	r1, #0x0
   10224: f7f3 effc    	blx	0x4220 <lgao_set_apptype@plt> @ imm = #-0xc008
   10228: f7fe ba2c    	b.w	0xe684 <gst_lg_audio_render_change_state+0xec4> @ imm = #-0x1ba8
   1022c: 4a37         	ldr	r2, [pc, #0xdc]         @ 0x1030c <gst_lg_audio_render_change_state+0x2b4c>
   1022e: 2103         	movs	r1, #0x3
   10230: 4b37         	ldr	r3, [pc, #0xdc]         @ 0x10310 <gst_lg_audio_render_change_state+0x2b50>
   10232: 4838         	ldr	r0, [pc, #0xe0]         @ 0x10314 <gst_lg_audio_render_change_state+0x2b54>
   10234: 447a         	add	r2, pc
   10236: 447b         	add	r3, pc
   10238: f502 721b    	add.w	r2, r2, #0x26c
   1023c: 9300         	str	r3, [sp]
   1023e: 4478         	add	r0, pc
   10240: f640 3323    	movw	r3, #0xb23
   10244: f7f3 eeb2    	blx	0x3fac <logmm_printl@plt> @ imm = #-0xc29c
   10248: e779         	b	0x1013e <gst_lg_audio_render_change_state+0x297e> @ imm = #-0x10e
   1024a: bf00         	nop
   1024c: 6c d5 00 00  	.word	0x0000d56c
   10250: a4 21 02 00  	.word	0x000221a4
   10254: 08 04 00 00  	.word	0x00000408
   10258: 30 d5 00 00  	.word	0x0000d530
   1025c: 0a fd 00 00  	.word	0x0000fd0a
   10260: a8 9a 00 00  	.word	0x00009aa8
   10264: 5c fc 00 00  	.word	0x0000fc5c
   10268: a2 d3 00 00  	.word	0x0000d3a2
   1026c: 70 d4 00 00  	.word	0x0000d470
   10270: a4 20 02 00  	.word	0x000220a4
   10274: 80 d4 00 00  	.word	0x0000d480
   10278: 0a fc 00 00  	.word	0x0000fc0a
   1027c: a8 99 00 00  	.word	0x000099a8
   10280: ee fb 00 00  	.word	0x0000fbee
   10284: 00 dd 00 00  	.word	0x0000dd00
   10288: d2 fb 00 00  	.word	0x0000fbd2
   1028c: 78 d3 00 00  	.word	0x0000d378
   10290: 3c d4 00 00  	.word	0x0000d43c
   10294: 34 d4 00 00  	.word	0x0000d434
   10298: 08 20 02 00  	.word	0x00022008
   1029c: 54 d4 00 00  	.word	0x0000d454
   102a0: 6e fb 00 00  	.word	0x0000fb6e
   102a4: 0c 99 00 00  	.word	0x0000990c
   102a8: 00 fb 00 00  	.word	0x0000fb00
   102ac: ea d2 00 00  	.word	0x0000d2ea
   102b0: e6 fa 00 00  	.word	0x0000fae6
   102b4: 1c d3 00 00  	.word	0x0000d31c
   102b8: 32 a2 00 00  	.word	0x0000a232
   102bc: 30 1f 02 00  	.word	0x00021f30
   102c0: e0 d3 00 00  	.word	0x0000d3e0
   102c4: 96 fa 00 00  	.word	0x0000fa96
   102c8: 34 98 00 00  	.word	0x00009834
   102cc: 12 d4 00 00  	.word	0x0000d412
   102d0: cc 1e 02 00  	.word	0x00021ecc
   102d4: d6 d3 00 00  	.word	0x0000d3d6
   102d8: 34 fa 00 00  	.word	0x0000fa34
   102dc: d2 97 00 00  	.word	0x000097d2
   102e0: f2 d3 00 00  	.word	0x0000d3f2
   102e4: 76 1e 02 00  	.word	0x00021e76
   102e8: d0 d3 00 00  	.word	0x0000d3d0
   102ec: de f9 00 00  	.word	0x0000f9de
   102f0: 7c 97 00 00  	.word	0x0000977c
   102f4: be f9 00 00  	.word	0x0000f9be
   102f8: 54 d2 00 00  	.word	0x0000d254
   102fc: a4 f9 00 00  	.word	0x0000f9a4
   10300: a2 d2 00 00  	.word	0x0000d2a2
   10304: 8a f9 00 00  	.word	0x0000f98a
   10308: 38 d3 00 00  	.word	0x0000d338
   1030c: 60 f9 00 00  	.word	0x0000f960
   10310: be d2 00 00  	.word	0x0000d2be
   10314: c2 1d 02 00  	.word	0x00021dc2
