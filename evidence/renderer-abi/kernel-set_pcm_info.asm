
/tmp/k329.Image:	file format coff-arm64

Disassembly of section .text:

0000000000001000 <.text>:
  c37b40: d10203ff     	sub	sp, sp, #0x80
  c37b44: a9037bfd     	stp	x29, x30, [sp, #0x30]
  c37b48: 9100c3fd     	add	x29, sp, #0x30
  c37b4c: a90453f3     	stp	x19, x20, [sp, #0x40]
  c37b50: 2a0003f3     	mov	w19, w0
  c37b54: 7100241f     	cmp	w0, #0x9
  c37b58: 54000888     	b.hi	0xc37c68 <.text+0xc36c68>
  c37b5c: a9055bf5     	stp	x21, x22, [sp, #0x50]
  c37b60: d000b076     	adrp	x22, 0x2245000
  c37b64: 52800b14     	mov	w20, #0x58              // =88
  c37b68: a90663f7     	stp	x23, x24, [sp, #0x60]
  c37b6c: 913962d8     	add	x24, x22, #0xe58
  c37b70: 93407c17     	sxtw	x23, w0
  c37b74: f9003bf9     	str	x25, [sp, #0x70]
  c37b78: 9101e319     	add	x25, x24, #0x78
  c37b7c: aa0103f5     	mov	x21, x1
  c37b80: 9b346414     	smaddl	x20, w0, w20, x25
  c37b84: 91002294     	add	x20, x20, #0x8
  c37b88: aa1403e0     	mov	x0, x20
  c37b8c: 940f1595     	bl	0xffd1e0 <.text+0xffc1e0>
  c37b90: 8b33c6e2     	add	x2, x23, w19, sxtw #1
  c37b94: d37ef442     	lsl	x2, x2, #2
  c37b98: cb170042     	sub	x2, x2, x23
  c37b9c: 8b020f22     	add	x2, x25, x2, lsl #3
  c37ba0: 3940a040     	ldrb	w0, [x2, #0x28]
  c37ba4: 34000820     	cbz	w0, 0xc37ca8 <.text+0xc36ca8>
  c37ba8: a94006a3     	ldp	x3, x1, [x21]
  c37bac: a9030443     	stp	x3, x1, [x2, #0x30]
  c37bb0: aa1403e0     	mov	x0, x20
  c37bb4: a94106a3     	ldp	x3, x1, [x21, #0x10]
  c37bb8: a9040443     	stp	x3, x1, [x2, #0x40]
  c37bbc: 940f1119     	bl	0xffc020 <.text+0xffb020>
  c37bc0: b94e5ac0     	ldr	w0, [x22, #0xe58]
  c37bc4: 37f80440     	tbnz	w0, #0x1f, 0xc37c4c <.text+0xc36c4c>
  c37bc8: 8b1706e8     	add	x8, x23, x23, lsl #1
  c37bcc: 9101e301     	add	x1, x24, #0x78
  c37bd0: f0004943     	adrp	x3, 0x1562000 <.text+0x1561000>
  c37bd4: 9137a063     	add	x3, x3, #0xde8
  c37bd8: d37ef508     	lsl	x8, x8, #2
  c37bdc: aa1803e0     	mov	x0, x24
  c37be0: cb170108     	sub	x8, x8, x23
  c37be4: 91176063     	add	x3, x3, #0x5d8
  c37be8: f0006125     	adrp	x5, 0x185e000 <.text+0x185d000>
  c37bec: 5280c9a4     	mov	w4, #0x64d              // =1613
  c37bf0: 8b080c28     	add	x8, x1, x8, lsl #3
  c37bf4: 910780a5     	add	x5, x5, #0x1e0
  c37bf8: 52800062     	mov	w2, #0x3                // =3
  c37bfc: d2800001     	mov	x1, #0x0                // =0
  c37c00: 29461d0d     	ldp	w13, w7, [x8, #0x30]
  c37c04: 29473106     	ldp	w6, w12, [x8, #0x38]
  c37c08: 2948290b     	ldp	w11, w10, [x8, #0x40]
  c37c0c: 29492109     	ldp	w9, w8, [x8, #0x48]
  c37c10: b90003ed     	str	w13, [sp]
  c37c14: b9000bec     	str	w12, [sp, #0x8]
  c37c18: b90013eb     	str	w11, [sp, #0x10]
  c37c1c: b9001bea     	str	w10, [sp, #0x18]
  c37c20: b90023e9     	str	w9, [sp, #0x20]
  c37c24: b9002be8     	str	w8, [sp, #0x28]
  c37c28: 97fdcee6     	bl	0xbab7c0 <.text+0xbaa7c0>
  c37c2c: a9455bf5     	ldp	x21, x22, [sp, #0x50]
  c37c30: 52800000     	mov	w0, #0x0                // =0
  c37c34: a94663f7     	ldp	x23, x24, [sp, #0x60]
  c37c38: f9403bf9     	ldr	x25, [sp, #0x70]
  c37c3c: a9437bfd     	ldp	x29, x30, [sp, #0x30]
  c37c40: a94453f3     	ldp	x19, x20, [sp, #0x40]
  c37c44: 910203ff     	add	sp, sp, #0x80
  c37c48: d65f03c0     	ret
  c37c4c: d0006120     	adrp	x0, 0x185d000 <.text+0x185c000>
  c37c50: 9117c000     	add	x0, x0, #0x5f0
  c37c54: 97fdcb0f     	bl	0xbaa890 <.text+0xba9890>
  c37c58: b90e5ac0     	str	w0, [x22, #0xe58]
  c37c5c: 528000e1     	mov	w1, #0x7                // =7
  c37c60: 97fdcbbc     	bl	0xbaab50 <.text+0xba9b50>
  c37c64: 17ffffd9     	b	0xc37bc8 <.text+0xc36bc8>
  c37c68: d000b074     	adrp	x20, 0x2245000
  c37c6c: b94e5a80     	ldr	w0, [x20, #0xe58]
  c37c70: 37f80420     	tbnz	w0, #0x1f, 0xc37cf4 <.text+0xc36cf4>
  c37c74: f0004943     	adrp	x3, 0x1562000 <.text+0x1561000>
  c37c78: 9137a063     	add	x3, x3, #0xde8
  c37c7c: 2a1303e6     	mov	w6, w19
  c37c80: 91396280     	add	x0, x20, #0xe58
  c37c84: 91176063     	add	x3, x3, #0x5d8
  c37c88: d0006125     	adrp	x5, 0x185d000 <.text+0x185c000>
  c37c8c: 5280c7c4     	mov	w4, #0x63e              // =1598
  c37c90: 913740a5     	add	x5, x5, #0xdd0
  c37c94: 52800002     	mov	w2, #0x0                // =0
  c37c98: d2800001     	mov	x1, #0x0                // =0
  c37c9c: 97fdcec9     	bl	0xbab7c0 <.text+0xbaa7c0>
  c37ca0: 12800000     	mov	w0, #-0x1               // =-1
  c37ca4: 17ffffe6     	b	0xc37c3c <.text+0xc36c3c>
  c37ca8: b94e5ac0     	ldr	w0, [x22, #0xe58]
  c37cac: 37f80320     	tbnz	w0, #0x1f, 0xc37d10 <.text+0xc36d10>
  c37cb0: f0004943     	adrp	x3, 0x1562000 <.text+0x1561000>
  c37cb4: 9137a063     	add	x3, x3, #0xde8
  c37cb8: 91176063     	add	x3, x3, #0x5d8
  c37cbc: d0006125     	adrp	x5, 0x185d000 <.text+0x185c000>
  c37cc0: 913940a5     	add	x5, x5, #0xe50
  c37cc4: 5280c7c4     	mov	w4, #0x63e              // =1598
  c37cc8: 52800002     	mov	w2, #0x0                // =0
  c37ccc: d2800001     	mov	x1, #0x0                // =0
  c37cd0: aa1803e0     	mov	x0, x24
  c37cd4: 97fdcebb     	bl	0xbab7c0 <.text+0xbaa7c0>
  c37cd8: aa1403e0     	mov	x0, x20
  c37cdc: 940f10d1     	bl	0xffc020 <.text+0xffb020>
  c37ce0: 12800000     	mov	w0, #-0x1               // =-1
  c37ce4: a9455bf5     	ldp	x21, x22, [sp, #0x50]
  c37ce8: a94663f7     	ldp	x23, x24, [sp, #0x60]
  c37cec: f9403bf9     	ldr	x25, [sp, #0x70]
  c37cf0: 17ffffd3     	b	0xc37c3c <.text+0xc36c3c>
  c37cf4: d0006120     	adrp	x0, 0x185d000 <.text+0x185c000>
  c37cf8: 9117c000     	add	x0, x0, #0x5f0
  c37cfc: 97fdcae5     	bl	0xbaa890 <.text+0xba9890>
  c37d00: b90e5a80     	str	w0, [x20, #0xe58]
  c37d04: 528000e1     	mov	w1, #0x7                // =7
  c37d08: 97fdcb92     	bl	0xbaab50 <.text+0xba9b50>
  c37d0c: 17ffffda     	b	0xc37c74 <.text+0xc36c74>
  c37d10: d0006120     	adrp	x0, 0x185d000 <.text+0x185c000>
  c37d14: 9117c000     	add	x0, x0, #0x5f0
  c37d18: 97fdcade     	bl	0xbaa890 <.text+0xba9890>
  c37d1c: b90e5ac0     	str	w0, [x22, #0xe58]
  c37d20: 528000e1     	mov	w1, #0x7                // =7
  c37d24: 97fdcb8b     	bl	0xbaab50 <.text+0xba9b50>
  c37d28: 17ffffe2     	b	0xc37cb0 <.text+0xc36cb0>
  c37d2c: d503201f     	nop
