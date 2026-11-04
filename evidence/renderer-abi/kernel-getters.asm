
/tmp/k329.Image:	file format coff-arm64

Disassembly of section .text:

0000000000001000 <.text>:
  c251d0: a9be7bfd     	stp	x29, x30, [sp, #-0x20]!
  c251d4: 910003fd     	mov	x29, sp
  c251d8: a90153f3     	stp	x19, x20, [sp, #0x10]
  c251dc: aa0003f3     	mov	x19, x0
  c251e0: d000b0f4     	adrp	x20, 0x2243000
  c251e4: 39413000     	ldrb	w0, [x0, #0x4c]
  c251e8: 34000060     	cbz	w0, 0xc251f4 <.text+0xc241f4>
  c251ec: 39413660     	ldrb	w0, [x19, #0x4d]
  c251f0: 350002a0     	cbnz	w0, 0xc25244 <.text+0xc24244>
  c251f4: aa1303e0     	mov	x0, x19
  c251f8: 52800021     	mov	w1, #0x1                // =1
  c251fc: 97fff1a6     	bl	0xc21894 <.text+0xc20894>
  c25200: b94f0280     	ldr	w0, [x20, #0xf00]
  c25204: 37f80420     	tbnz	w0, #0x1f, 0xc25288 <.text+0xc24288>
  c25208: b9402e66     	ldr	w6, [x19, #0x2c]
  c2520c: 900049e3     	adrp	x3, 0x1561000 <.text+0x1560000>
  c25210: 9127a063     	add	x3, x3, #0x9e8
  c25214: 913c0280     	add	x0, x20, #0xf00
  c25218: 91164063     	add	x3, x3, #0x590
  c2521c: 5280c3c4     	mov	w4, #0x61e              // =1566
  c25220: 52800062     	mov	w2, #0x3                // =3
  c25224: d2800001     	mov	x1, #0x0                // =0
  c25228: d00061a5     	adrp	x5, 0x185b000 <.text+0x185a000>
  c2522c: 910300a5     	add	x5, x5, #0xc0
  c25230: 97fe1964     	bl	0xbab7c0 <.text+0xbaa7c0>
  c25234: b9402e60     	ldr	w0, [x19, #0x2c]
  c25238: a94153f3     	ldp	x19, x20, [sp, #0x10]
  c2523c: a8c27bfd     	ldp	x29, x30, [sp], #0x20
  c25240: d65f03c0     	ret
  c25244: b94f0280     	ldr	w0, [x20, #0xf00]
  c25248: 37f802e0     	tbnz	w0, #0x1f, 0xc252a4 <.text+0xc242a4>
  c2524c: b9402e66     	ldr	w6, [x19, #0x2c]
  c25250: 900049e3     	adrp	x3, 0x1561000 <.text+0x1560000>
  c25254: 9127a063     	add	x3, x3, #0x9e8
  c25258: 913c0280     	add	x0, x20, #0xf00
  c2525c: 91164063     	add	x3, x3, #0x590
  c25260: 5280c304     	mov	w4, #0x618              // =1560
  c25264: 52800082     	mov	w2, #0x4                // =4
  c25268: d2800001     	mov	x1, #0x0                // =0
  c2526c: d00061a5     	adrp	x5, 0x185b000 <.text+0x185a000>
  c25270: 910280a5     	add	x5, x5, #0xa0
  c25274: 97fe1953     	bl	0xbab7c0 <.text+0xbaa7c0>
  c25278: b9402e60     	ldr	w0, [x19, #0x2c]
  c2527c: a94153f3     	ldp	x19, x20, [sp, #0x10]
  c25280: a8c27bfd     	ldp	x29, x30, [sp], #0x20
  c25284: d65f03c0     	ret
  c25288: b00061a0     	adrp	x0, 0x185a000 <.text+0x1859000>
  c2528c: 91172000     	add	x0, x0, #0x5c8
  c25290: 97fe1580     	bl	0xbaa890 <.text+0xba9890>
  c25294: b90f0280     	str	w0, [x20, #0xf00]
  c25298: 528000e1     	mov	w1, #0x7                // =7
  c2529c: 97fe162d     	bl	0xbaab50 <.text+0xba9b50>
  c252a0: 17ffffda     	b	0xc25208 <.text+0xc24208>
  c252a4: b00061a0     	adrp	x0, 0x185a000 <.text+0x1859000>
  c252a8: 91172000     	add	x0, x0, #0x5c8
  c252ac: 97fe1579     	bl	0xbaa890 <.text+0xba9890>
  c252b0: b90f0280     	str	w0, [x20, #0xf00]
  c252b4: 528000e1     	mov	w1, #0x7                // =7
  c252b8: 97fe1626     	bl	0xbaab50 <.text+0xba9b50>
  c252bc: 17ffffe4     	b	0xc2524c <.text+0xc2424c>
  c252c0: a9be7bfd     	stp	x29, x30, [sp, #-0x20]!
  c252c4: 910003fd     	mov	x29, sp
  c252c8: 39413001     	ldrb	w1, [x0, #0x4c]
  c252cc: 340000a1     	cbz	w1, 0xc252e0 <.text+0xc242e0>
  c252d0: f9400c00     	ldr	x0, [x0, #0x18]
  c252d4: 97ffa500     	bl	0xc0e6d4 <.text+0xc0d6d4>
  c252d8: a8c27bfd     	ldp	x29, x30, [sp], #0x20
  c252dc: d65f03c0     	ret
  c252e0: f9000bf3     	str	x19, [sp, #0x10]
  c252e4: d000b0f3     	adrp	x19, 0x2243000
  c252e8: b94f0260     	ldr	w0, [x19, #0xf00]
  c252ec: 37f801c0     	tbnz	w0, #0x1f, 0xc25324 <.text+0xc24324>
  c252f0: 900049e3     	adrp	x3, 0x1561000 <.text+0x1560000>
  c252f4: 9127a063     	add	x3, x3, #0x9e8
  c252f8: 913c0260     	add	x0, x19, #0xf00
  c252fc: 9116e063     	add	x3, x3, #0x5b8
  c25300: d00061a5     	adrp	x5, 0x185b000 <.text+0x185a000>
  c25304: 5280c544     	mov	w4, #0x62a              // =1578
  c25308: 9103e0a5     	add	x5, x5, #0xf8
  c2530c: 52800062     	mov	w2, #0x3                // =3
  c25310: d2800001     	mov	x1, #0x0                // =0
  c25314: 97fe192b     	bl	0xbab7c0 <.text+0xbaa7c0>
  c25318: 12800000     	mov	w0, #-0x1               // =-1
  c2531c: f9400bf3     	ldr	x19, [sp, #0x10]
  c25320: 17ffffee     	b	0xc252d8 <.text+0xc242d8>
  c25324: b00061a0     	adrp	x0, 0x185a000 <.text+0x1859000>
  c25328: 91172000     	add	x0, x0, #0x5c8
  c2532c: 97fe1559     	bl	0xbaa890 <.text+0xba9890>
  c25330: b90f0260     	str	w0, [x19, #0xf00]
  c25334: 528000e1     	mov	w1, #0x7                // =7
  c25338: 97fe1606     	bl	0xbaab50 <.text+0xba9b50>
  c2533c: 17ffffed     	b	0xc252f0 <.text+0xc242f0>
  c25340: a9be7bfd     	stp	x29, x30, [sp, #-0x20]!
  c25344: 910003fd     	mov	x29, sp
  c25348: a90153f3     	stp	x19, x20, [sp, #0x10]
  c2534c: aa0003f3     	mov	x19, x0
  c25350: 39413000     	ldrb	w0, [x0, #0x4c]
  c25354: 35000220     	cbnz	w0, 0xc25398 <.text+0xc24398>
  c25358: d000b0f4     	adrp	x20, 0x2243000
  c2535c: b94f0280     	ldr	w0, [x20, #0xf00]
  c25360: 37f80260     	tbnz	w0, #0x1f, 0xc253ac <.text+0xc243ac>
  c25364: 900049e3     	adrp	x3, 0x1561000 <.text+0x1560000>
  c25368: 9127a063     	add	x3, x3, #0x9e8
  c2536c: 91176063     	add	x3, x3, #0x5d8
  c25370: d00061a5     	adrp	x5, 0x185b000 <.text+0x185a000>
  c25374: 9103e0a5     	add	x5, x5, #0xf8
  c25378: 5280c6e4     	mov	w4, #0x637              // =1591
  c2537c: 52800002     	mov	w2, #0x0                // =0
  c25380: d2800001     	mov	x1, #0x0                // =0
  c25384: 913c0280     	add	x0, x20, #0xf00
  c25388: 97fe190e     	bl	0xbab7c0 <.text+0xbaa7c0>
  c2538c: aa1303e0     	mov	x0, x19
  c25390: 52800001     	mov	w1, #0x0                // =0
  c25394: 97fff140     	bl	0xc21894 <.text+0xc20894>
  c25398: f9400e60     	ldr	x0, [x19, #0x18]
  c2539c: 97ffa4a9     	bl	0xc0e640 <.text+0xc0d640>
  c253a0: a94153f3     	ldp	x19, x20, [sp, #0x10]
  c253a4: a8c27bfd     	ldp	x29, x30, [sp], #0x20
  c253a8: d65f03c0     	ret
  c253ac: b00061a0     	adrp	x0, 0x185a000 <.text+0x1859000>
  c253b0: 91172000     	add	x0, x0, #0x5c8
  c253b4: 97fe1537     	bl	0xbaa890 <.text+0xba9890>
  c253b8: b90f0280     	str	w0, [x20, #0xf00]
  c253bc: 528000e1     	mov	w1, #0x7                // =7
  c253c0: 97fe15e4     	bl	0xbaab50 <.text+0xba9b50>
  c253c4: 17ffffe8     	b	0xc25364 <.text+0xc24364>
