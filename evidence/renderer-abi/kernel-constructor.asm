
/tmp/k329.Image:	file format coff-arm64

Disassembly of section .text:

0000000000001000 <.text>:
  c21894: a9bd7bfd     	stp	x29, x30, [sp, #-0x30]!
  c21898: 910003fd     	mov	x29, sp
  c2189c: a90153f3     	stp	x19, x20, [sp, #0x10]
  c218a0: aa0003f3     	mov	x19, x0
  c218a4: f90013f5     	str	x21, [sp, #0x20]
  c218a8: 12001c35     	and	w21, w1, #0xff
  c218ac: 97ffdbf1     	bl	0xc18870 <.text+0xc17870>
  c218b0: 2a0003f4     	mov	w20, w0
  c218b4: f9400e60     	ldr	x0, [x19, #0x18]
  c218b8: b4000040     	cbz	x0, 0xc218c0 <.text+0xc208c0>
  c218bc: 97ffb2f5     	bl	0xc0e490 <.text+0xc0d490>
  c218c0: 2a1403e0     	mov	w0, w20
  c218c4: b00061c1     	adrp	x1, 0x185a000 <.text+0x1859000>
  c218c8: 9125a021     	add	x1, x1, #0x968
  c218cc: 97ffb26d     	bl	0xc0e280 <.text+0xc0d280>
  c218d0: f9000e60     	str	x0, [x19, #0x18]
  c218d4: 34000655     	cbz	w21, 0xc2199c <.text+0xc2099c>
  c218d8: b00063d4     	adrp	x20, 0x189a000 <.text+0x1899000>
  c218dc: 91116294     	add	x20, x20, #0x458
  c218e0: aa1403e1     	mov	x1, x20
  c218e4: 97ffb613     	bl	0xc0f130 <.text+0xc0e130>
  c218e8: b9002e60     	str	w0, [x19, #0x2c]
  c218ec: b00061c1     	adrp	x1, 0x185a000 <.text+0x1859000>
  c218f0: f9400e60     	ldr	x0, [x19, #0x18]
  c218f4: 9125e021     	add	x1, x1, #0x978
  c218f8: 97ffb60e     	bl	0xc0f130 <.text+0xc0e130>
  c218fc: b9003660     	str	w0, [x19, #0x34]
  c21900: b9402e62     	ldr	w2, [x19, #0x2c]
  c21904: aa1403e1     	mov	x1, x20
  c21908: aa1303e0     	mov	x0, x19
  c2190c: 97fffbad     	bl	0xc207c0 <.text+0xc1f7c0>
  c21910: aa1303e0     	mov	x0, x19
  c21914: 52800041     	mov	w1, #0x2                // =2
  c21918: 97ffff2e     	bl	0xc215d0 <.text+0xc205d0>
  c2191c: f9400e60     	ldr	x0, [x19, #0x18]
  c21920: b00061d4     	adrp	x20, 0x185a000 <.text+0x1859000>
  c21924: 91260294     	add	x20, x20, #0x980
  c21928: aa1403e1     	mov	x1, x20
  c2192c: 97ffb601     	bl	0xc0f130 <.text+0xc0e130>
  c21930: 2a0003e2     	mov	w2, w0
  c21934: b9004262     	str	w2, [x19, #0x40]
  c21938: aa1403e1     	mov	x1, x20
  c2193c: aa1303e0     	mov	x0, x19
  c21940: 97fffba0     	bl	0xc207c0 <.text+0xc1f7c0>
  c21944: aa1303e0     	mov	x0, x19
  c21948: 52800021     	mov	w1, #0x1                // =1
  c2194c: 97ffff21     	bl	0xc215d0 <.text+0xc205d0>
  c21950: f9400e63     	ldr	x3, [x19, #0x18]
  c21954: aa1303e0     	mov	x0, x19
  c21958: 52800004     	mov	w4, #0x0                // =0
  c2195c: 52802002     	mov	w2, #0x100              // =256
  c21960: 52800021     	mov	w1, #0x1                // =1
  c21964: 97ffdeeb     	bl	0xc19510 <.text+0xc18510>
  c21968: f9400e63     	ldr	x3, [x19, #0x18]
  c2196c: aa1303e0     	mov	x0, x19
  c21970: 52800004     	mov	w4, #0x0                // =0
  c21974: 52802002     	mov	w2, #0x100              // =256
  c21978: 52800001     	mov	w1, #0x0                // =0
  c2197c: 97ffdee5     	bl	0xc19510 <.text+0xc18510>
  c21980: 39013675     	strb	w21, [x19, #0x4d]
  c21984: 52800020     	mov	w0, #0x1                // =1
  c21988: 39013260     	strb	w0, [x19, #0x4c]
  c2198c: a94153f3     	ldp	x19, x20, [sp, #0x10]
  c21990: f94013f5     	ldr	x21, [sp, #0x20]
  c21994: a8c37bfd     	ldp	x29, x30, [sp], #0x30
  c21998: d65f03c0     	ret
  c2199c: f00057c1     	adrp	x1, 0x171c000 <.text+0x171b000>
  c219a0: 91050021     	add	x1, x1, #0x140
  c219a4: 97ffb5e3     	bl	0xc0f130 <.text+0xc0e130>
  c219a8: 17ffffdd     	b	0xc2191c <.text+0xc2091c>
