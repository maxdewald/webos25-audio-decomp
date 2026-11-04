
/tmp/k329.Image:	file format coff-arm64

Disassembly of section .text:

0000000000001000 <.text>:
  c355c4: a9bb7bfd     	stp	x29, x30, [sp, #-0x50]!
  c355c8: 910003fd     	mov	x29, sp
  c355cc: a90153f3     	stp	x19, x20, [sp, #0x10]
  c355d0: 2a0003f3     	mov	w19, w0
  c355d4: 7100241f     	cmp	w0, #0x9
  c355d8: 54000768     	b.hi	0xc356c4 <.text+0xc346c4>
  c355dc: a9025bf5     	stp	x21, x22, [sp, #0x20]
  c355e0: 9000b096     	adrp	x22, 0x2245000
  c355e4: 52800b15     	mov	w21, #0x58              // =88
  c355e8: a90363f7     	stp	x23, x24, [sp, #0x30]
  c355ec: 913962d7     	add	x23, x22, #0xe58
  c355f0: 9101e2f8     	add	x24, x23, #0x78
  c355f4: f90023f9     	str	x25, [sp, #0x40]
  c355f8: 9b356015     	smaddl	x21, w0, w21, x24
  c355fc: 910022b5     	add	x21, x21, #0x8
  c35600: aa1503e0     	mov	x0, x21
  c35604: 940f1ef7     	bl	0xffd1e0 <.text+0xffc1e0>
  c35608: 93407e60     	sxtw	x0, w19
  c3560c: 8b33c414     	add	x20, x0, w19, sxtw #1
  c35610: d37ef694     	lsl	x20, x20, #2
  c35614: cb000294     	sub	x20, x20, x0
  c35618: d37df294     	lsl	x20, x20, #3
  c3561c: 8b140319     	add	x25, x24, x20
  c35620: 3940a320     	ldrb	w0, [x25, #0x28]
  c35624: 35000700     	cbnz	w0, 0xc35704 <.text+0xc34704>
  c35628: 2a1303e1     	mov	w1, w19
  c3562c: b0006120     	adrp	x0, 0x185a000 <.text+0x1859000>
  c35630: 913fc000     	add	x0, x0, #0xff0
  c35634: 97ff8b7b     	bl	0xc18420 <.text+0xc17420>
  c35638: aa0003e3     	mov	x3, x0
  c3563c: 52800022     	mov	w2, #0x1                // =1
  c35640: 12800001     	mov	w1, #-0x1               // =-1
  c35644: f8346b03     	str	x3, [x24, x20]
  c35648: 3900a322     	strb	w2, [x25, #0x28]
  c3564c: aa1503e0     	mov	x0, x21
  c35650: b9005721     	str	w1, [x25, #0x54]
  c35654: 940f1a73     	bl	0xffc020 <.text+0xffb020>
  c35658: b94e5ac0     	ldr	w0, [x22, #0xe58]
  c3565c: 37f80260     	tbnz	w0, #0x1f, 0xc356a8 <.text+0xc346a8>
  c35660: b0004963     	adrp	x3, 0x1562000 <.text+0x1561000>
  c35664: 9137a063     	add	x3, x3, #0xde8
  c35668: aa1703e0     	mov	x0, x23
  c3566c: 2a1303e6     	mov	w6, w19
  c35670: 9110a063     	add	x3, x3, #0x428
  c35674: 90006145     	adrp	x5, 0x185d000 <.text+0x185c000>
  c35678: 528091e4     	mov	w4, #0x48f              // =1167
  c3567c: 9138e0a5     	add	x5, x5, #0xe38
  c35680: 52800062     	mov	w2, #0x3                // =3
  c35684: d2800001     	mov	x1, #0x0                // =0
  c35688: 97fdd84e     	bl	0xbab7c0 <.text+0xbaa7c0>
  c3568c: a9425bf5     	ldp	x21, x22, [sp, #0x20]
  c35690: 52800000     	mov	w0, #0x0                // =0
  c35694: a94363f7     	ldp	x23, x24, [sp, #0x30]
  c35698: f94023f9     	ldr	x25, [sp, #0x40]
  c3569c: a94153f3     	ldp	x19, x20, [sp, #0x10]
  c356a0: a8c57bfd     	ldp	x29, x30, [sp], #0x50
  c356a4: d65f03c0     	ret
  c356a8: 90006140     	adrp	x0, 0x185d000 <.text+0x185c000>
  c356ac: 9117c000     	add	x0, x0, #0x5f0
  c356b0: 97fdd478     	bl	0xbaa890 <.text+0xba9890>
  c356b4: b90e5ac0     	str	w0, [x22, #0xe58]
  c356b8: 528000e1     	mov	w1, #0x7                // =7
  c356bc: 97fdd525     	bl	0xbaab50 <.text+0xba9b50>
  c356c0: 17ffffe8     	b	0xc35660 <.text+0xc34660>
  c356c4: 9000b094     	adrp	x20, 0x2245000
  c356c8: b94e5a80     	ldr	w0, [x20, #0xe58]
  c356cc: 37f80440     	tbnz	w0, #0x1f, 0xc35754 <.text+0xc34754>
  c356d0: b0004963     	adrp	x3, 0x1562000 <.text+0x1561000>
  c356d4: 9137a063     	add	x3, x3, #0xde8
  c356d8: 2a1303e6     	mov	w6, w19
  c356dc: 91396280     	add	x0, x20, #0xe58
  c356e0: 9110a063     	add	x3, x3, #0x428
  c356e4: 90006145     	adrp	x5, 0x185d000 <.text+0x185c000>
  c356e8: 52808f24     	mov	w4, #0x479              // =1145
  c356ec: 913740a5     	add	x5, x5, #0xdd0
  c356f0: 52800002     	mov	w2, #0x0                // =0
  c356f4: d2800001     	mov	x1, #0x0                // =0
  c356f8: 97fdd832     	bl	0xbab7c0 <.text+0xbaa7c0>
  c356fc: 12800000     	mov	w0, #-0x1               // =-1
  c35700: 17ffffe7     	b	0xc3569c <.text+0xc3469c>
  c35704: b94e5ac0     	ldr	w0, [x22, #0xe58]
  c35708: 37f80340     	tbnz	w0, #0x1f, 0xc35770 <.text+0xc34770>
  c3570c: b0004963     	adrp	x3, 0x1562000 <.text+0x1561000>
  c35710: 9137a063     	add	x3, x3, #0xde8
  c35714: 2a1303e6     	mov	w6, w19
  c35718: 9110a063     	add	x3, x3, #0x428
  c3571c: 90006145     	adrp	x5, 0x185d000 <.text+0x185c000>
  c35720: 913800a5     	add	x5, x5, #0xe00
  c35724: 52809064     	mov	w4, #0x483              // =1155
  c35728: 52800002     	mov	w2, #0x0                // =0
  c3572c: d2800001     	mov	x1, #0x0                // =0
  c35730: aa1703e0     	mov	x0, x23
  c35734: 97fdd823     	bl	0xbab7c0 <.text+0xbaa7c0>
  c35738: aa1503e0     	mov	x0, x21
  c3573c: 940f1a39     	bl	0xffc020 <.text+0xffb020>
  c35740: 12800000     	mov	w0, #-0x1               // =-1
  c35744: a9425bf5     	ldp	x21, x22, [sp, #0x20]
  c35748: a94363f7     	ldp	x23, x24, [sp, #0x30]
  c3574c: f94023f9     	ldr	x25, [sp, #0x40]
  c35750: 17ffffd3     	b	0xc3569c <.text+0xc3469c>
  c35754: 90006140     	adrp	x0, 0x185d000 <.text+0x185c000>
  c35758: 9117c000     	add	x0, x0, #0x5f0
  c3575c: 97fdd44d     	bl	0xbaa890 <.text+0xba9890>
  c35760: b90e5a80     	str	w0, [x20, #0xe58]
  c35764: 528000e1     	mov	w1, #0x7                // =7
  c35768: 97fdd4fa     	bl	0xbaab50 <.text+0xba9b50>
  c3576c: 17ffffd9     	b	0xc356d0 <.text+0xc346d0>
  c35770: 90006140     	adrp	x0, 0x185d000 <.text+0x185c000>
  c35774: 9117c000     	add	x0, x0, #0x5f0
  c35778: 97fdd446     	bl	0xbaa890 <.text+0xba9890>
  c3577c: b90e5ac0     	str	w0, [x22, #0xe58]
  c35780: 528000e1     	mov	w1, #0x7                // =7
  c35784: 97fdd4f3     	bl	0xbaab50 <.text+0xba9b50>
  c35788: 17ffffe1     	b	0xc3570c <.text+0xc3470c>
  c3578c: d503201f     	nop
  c35790: a9bb7bfd     	stp	x29, x30, [sp, #-0x50]!
  c35794: 910003fd     	mov	x29, sp
