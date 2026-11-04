
orig/usr/lib/gstreamer-1.0/libgstlgaudiosink.so:	file format elf32-littlearm

Disassembly of section .text:

00016fc4 <_lgao_config_set_renderer_param>:
   16fc4: b530         	push	{r4, r5, lr}
   16fc6: b08b         	sub	sp, #0x2c
   16fc8: ad02         	add	r5, sp, #0x8
   16fca: 4604         	mov	r4, r0
   16fcc: 2220         	movs	r2, #0x20
   16fce: 2100         	movs	r1, #0x0
   16fd0: 4628         	mov	r0, r5
   16fd2: f7ec ed2c    	blx	0x3a2c <memset@plt>     @ imm = #-0x135a8
   16fd6: f8d4 312c    	ldr.w	r3, [r4, #0x12c]
   16fda: f245 2114    	movw	r1, #0x5214
   16fde: f10d 0207    	add.w	r2, sp, #0x7
   16fe2: 6820         	ldr	r0, [r4]
   16fe4: f2c4 0101    	movt	r1, #0x4001
   16fe8: f1a3 0305    	sub.w	r3, r3, #0x5
   16fec: fab3 f383    	clz	r3, r3
   16ff0: 095b         	lsrs	r3, r3, #0x5
   16ff2: f88d 3007    	strb.w	r3, [sp, #0x7]
   16ff6: f7ec ee34    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x13398
   16ffa: f894 102d    	ldrb.w	r1, [r4, #0x2d]
   16ffe: 462a         	mov	r2, r5
   17000: f894 302c    	ldrb.w	r3, [r4, #0x2c]
   17004: 6aa0         	ldr	r0, [r4, #0x28]
   17006: f894 51d0    	ldrb.w	r5, [r4, #0x1d0]
   1700a: e9cd 3103    	strd	r3, r1, [sp, #12]
   1700e: e9d4 310c    	ldrd	r3, r1, [r4, #48]
   17012: 9002         	str	r0, [sp, #0x8]
   17014: 6820         	ldr	r0, [r4]
   17016: 9305         	str	r3, [sp, #0x14]
   17018: 6ca3         	ldr	r3, [r4, #0x48]
   1701a: e9cd 1306    	strd	r1, r3, [sp, #24]
   1701e: f245 2120    	movw	r1, #0x5220
   17022: f894 31d1    	ldrb.w	r3, [r4, #0x1d1]
   17026: f2c4 0120    	movt	r1, #0x4020
   1702a: e9cd 5308    	strd	r5, r3, [sp, #32]
   1702e: f7ec ee18    	blx	0x3c60 <ioctl@plt>      @ imm = #-0x133d0
   17032: b00b         	add	sp, #0x2c
   17034: bd30         	pop	{r4, r5, pc}
   17036: bf00         	nop
