
/tmp/k329.Image:	file format coff-arm64

Disassembly of section .text:

0000000000001000 <.text>:
  c35ab0: a9bb7bfd     	stp	x29, x30, [sp, #-0x50]!
  c35ab4: 910003fd     	mov	x29, sp
  c35ab8: a90153f3     	stp	x19, x20, [sp, #0x10]
  c35abc: 2a0003f3     	mov	w19, w0
  c35ac0: a9025bf5     	stp	x21, x22, [sp, #0x20]
  c35ac4: 7100241f     	cmp	w0, #0x9
  c35ac8: 54000ca8     	b.hi	0xc35c5c <.text+0xc34c5c>
  c35acc: 9000b096     	adrp	x22, 0x2245000
  c35ad0: a90363f7     	stp	x23, x24, [sp, #0x30]
  c35ad4: 913962d7     	add	x23, x22, #0xe58
  c35ad8: f90023f9     	str	x25, [sp, #0x40]
  c35adc: 9101e2f9     	add	x25, x23, #0x78
  c35ae0: 52800b14     	mov	w20, #0x58              // =88
  c35ae4: aa0103f5     	mov	x21, x1
  c35ae8: 12001c58     	and	w24, w2, #0xff
  c35aec: 9b346414     	smaddl	x20, w0, w20, x25
  c35af0: 91002294     	add	x20, x20, #0x8
  c35af4: aa1403e0     	mov	x0, x20
  c35af8: 940f1dba     	bl	0xffd1e0 <.text+0xffc1e0>
  c35afc: 93407e60     	sxtw	x0, w19
  c35b00: 8b33c413     	add	x19, x0, w19, sxtw #1
  c35b04: d37ef673     	lsl	x19, x19, #2
  c35b08: cb000273     	sub	x19, x19, x0
  c35b0c: d37df273     	lsl	x19, x19, #3
  c35b10: 8b130321     	add	x1, x25, x19
  c35b14: 3940a020     	ldrb	w0, [x1, #0x28]
  c35b18: 34000fe0     	cbz	w0, 0xc35d14 <.text+0xc34d14>
  c35b1c: f8736b20     	ldr	x0, [x25, x19]
  c35b20: 340003f8     	cbz	w24, 0xc35b9c <.text+0xc34b9c>
  c35b24: b9404822     	ldr	w2, [x1, #0x48]
  c35b28: 35000062     	cbnz	w2, 0xc35b34 <.text+0xc34b34>
  c35b2c: b9404c21     	ldr	w1, [x1, #0x4c]
  c35b30: 34000561     	cbz	w1, 0xc35bdc <.text+0xc34bdc>
  c35b34: 97ffbda7     	bl	0xc251d0 <.text+0xc241d0>
  c35b38: b90002a0     	str	w0, [x21]
  c35b3c: 2a0003e6     	mov	w6, w0
  c35b40: b94e5ac0     	ldr	w0, [x22, #0xe58]
  c35b44: 37f806c0     	tbnz	w0, #0x1f, 0xc35c1c <.text+0xc34c1c>
  c35b48: b0004973     	adrp	x19, 0x1562000 <.text+0x1561000>
  c35b4c: 9137a273     	add	x19, x19, #0xde8
  c35b50: 90006145     	adrp	x5, 0x185d000 <.text+0x185c000>
  c35b54: 91116263     	add	x3, x19, #0x458
  c35b58: 913a60a5     	add	x5, x5, #0xe98
  c35b5c: aa1703e0     	mov	x0, x23
  c35b60: 528098e4     	mov	w4, #0x4c7              // =1223
  c35b64: 52800062     	mov	w2, #0x3                // =3
  c35b68: d2800001     	mov	x1, #0x0                // =0
  c35b6c: 97fdd715     	bl	0xbab7c0 <.text+0xbaa7c0>
  c35b70: b94002a0     	ldr	w0, [x21]
  c35b74: 34000940     	cbz	w0, 0xc35c9c <.text+0xc34c9c>
  c35b78: aa1403e0     	mov	x0, x20
  c35b7c: 940f1929     	bl	0xffc020 <.text+0xffb020>
  c35b80: a94363f7     	ldp	x23, x24, [sp, #0x30]
  c35b84: 52800000     	mov	w0, #0x0                // =0
  c35b88: f94023f9     	ldr	x25, [sp, #0x40]
  c35b8c: a94153f3     	ldp	x19, x20, [sp, #0x10]
  c35b90: a9425bf5     	ldp	x21, x22, [sp, #0x20]
  c35b94: a8c57bfd     	ldp	x29, x30, [sp], #0x50
  c35b98: d65f03c0     	ret
  c35b9c: 97ff8a7a     	bl	0xc18584 <.text+0xc17584>
  c35ba0: b90002a0     	str	w0, [x21]
  c35ba4: 2a0003e6     	mov	w6, w0
  c35ba8: b94e5ac0     	ldr	w0, [x22, #0xe58]
  c35bac: 37f80480     	tbnz	w0, #0x1f, 0xc35c3c <.text+0xc34c3c>
  c35bb0: b0004973     	adrp	x19, 0x1562000 <.text+0x1561000>
  c35bb4: 9137a273     	add	x19, x19, #0xde8
  c35bb8: 91116263     	add	x3, x19, #0x458
  c35bbc: aa1703e0     	mov	x0, x23
  c35bc0: 90006145     	adrp	x5, 0x185d000 <.text+0x185c000>
  c35bc4: 52809a44     	mov	w4, #0x4d2              // =1234
  c35bc8: 913b40a5     	add	x5, x5, #0xed0
  c35bcc: 52800062     	mov	w2, #0x3                // =3
  c35bd0: d2800001     	mov	x1, #0x0                // =0
  c35bd4: 97fdd6fb     	bl	0xbab7c0 <.text+0xbaa7c0>
  c35bd8: 17ffffe6     	b	0xc35b70 <.text+0xc34b70>
  c35bdc: 97ffbdd9     	bl	0xc25340 <.text+0xc24340>
  c35be0: b90002a0     	str	w0, [x21]
  c35be4: 2a0003e6     	mov	w6, w0
  c35be8: b94e5ac0     	ldr	w0, [x22, #0xe58]
  c35bec: 37f80840     	tbnz	w0, #0x1f, 0xc35cf4 <.text+0xc34cf4>
  c35bf0: b0004973     	adrp	x19, 0x1562000 <.text+0x1561000>
  c35bf4: 9137a273     	add	x19, x19, #0xde8
  c35bf8: 91116263     	add	x3, x19, #0x458
  c35bfc: aa1703e0     	mov	x0, x23
  c35c00: 90006145     	adrp	x5, 0x185d000 <.text+0x185c000>
  c35c04: 52809984     	mov	w4, #0x4cc              // =1228
  c35c08: 913ae0a5     	add	x5, x5, #0xeb8
  c35c0c: 52800062     	mov	w2, #0x3                // =3
  c35c10: d2800001     	mov	x1, #0x0                // =0
  c35c14: 97fdd6eb     	bl	0xbab7c0 <.text+0xbaa7c0>
  c35c18: 17ffffd6     	b	0xc35b70 <.text+0xc34b70>
  c35c1c: 90006140     	adrp	x0, 0x185d000 <.text+0x185c000>
  c35c20: 9117c000     	add	x0, x0, #0x5f0
  c35c24: 97fdd31b     	bl	0xbaa890 <.text+0xba9890>
  c35c28: b90e5ac0     	str	w0, [x22, #0xe58]
  c35c2c: 528000e1     	mov	w1, #0x7                // =7
  c35c30: 97fdd3c8     	bl	0xbaab50 <.text+0xba9b50>
  c35c34: b94002a6     	ldr	w6, [x21]
  c35c38: 17ffffc4     	b	0xc35b48 <.text+0xc34b48>
  c35c3c: 90006140     	adrp	x0, 0x185d000 <.text+0x185c000>
  c35c40: 9117c000     	add	x0, x0, #0x5f0
  c35c44: 97fdd313     	bl	0xbaa890 <.text+0xba9890>
  c35c48: b90e5ac0     	str	w0, [x22, #0xe58]
  c35c4c: 528000e1     	mov	w1, #0x7                // =7
  c35c50: 97fdd3c0     	bl	0xbaab50 <.text+0xba9b50>
  c35c54: b94002a6     	ldr	w6, [x21]
  c35c58: 17ffffd6     	b	0xc35bb0 <.text+0xc34bb0>
  c35c5c: 9000b096     	adrp	x22, 0x2245000
  c35c60: b94e5ac0     	ldr	w0, [x22, #0xe58]
  c35c64: 37f80720     	tbnz	w0, #0x1f, 0xc35d48 <.text+0xc34d48>
  c35c68: b0004963     	adrp	x3, 0x1562000 <.text+0x1561000>
  c35c6c: 9137a063     	add	x3, x3, #0xde8
  c35c70: 2a1303e6     	mov	w6, w19
  c35c74: 913962c0     	add	x0, x22, #0xe58
  c35c78: 91116063     	add	x3, x3, #0x458
  c35c7c: 90006145     	adrp	x5, 0x185d000 <.text+0x185c000>
  c35c80: 528097c4     	mov	w4, #0x4be              // =1214
  c35c84: 913740a5     	add	x5, x5, #0xdd0
  c35c88: 52800002     	mov	w2, #0x0                // =0
  c35c8c: d2800001     	mov	x1, #0x0                // =0
  c35c90: 97fdd6cc     	bl	0xbab7c0 <.text+0xbaa7c0>
  c35c94: 12800000     	mov	w0, #-0x1               // =-1
  c35c98: 17ffffbd     	b	0xc35b8c <.text+0xc34b8c>
  c35c9c: b94e5ac0     	ldr	w0, [x22, #0xe58]
  c35ca0: 37f80620     	tbnz	w0, #0x1f, 0xc35d64 <.text+0xc34d64>
  c35ca4: 7100031f     	cmp	w24, #0x0
  c35ca8: b0006120     	adrp	x0, 0x185a000 <.text+0x1859000>
  c35cac: 90006146     	adrp	x6, 0x185d000 <.text+0x185c000>
  c35cb0: 913fc000     	add	x0, x0, #0xff0
  c35cb4: 913a20c6     	add	x6, x6, #0xe88
  c35cb8: 90006145     	adrp	x5, 0x185d000 <.text+0x185c000>
  c35cbc: 9a8010c6     	csel	x6, x6, x0, ne
  c35cc0: 91116263     	add	x3, x19, #0x458
  c35cc4: 913b80a5     	add	x5, x5, #0xee0
  c35cc8: aa1703e0     	mov	x0, x23
  c35ccc: 52809ae4     	mov	w4, #0x4d7              // =1239
  c35cd0: 52800002     	mov	w2, #0x0                // =0
  c35cd4: d2800001     	mov	x1, #0x0                // =0
  c35cd8: 97fdd6ba     	bl	0xbab7c0 <.text+0xbaa7c0>
  c35cdc: aa1403e0     	mov	x0, x20
  c35ce0: 940f18d0     	bl	0xffc020 <.text+0xffb020>
  c35ce4: 12800000     	mov	w0, #-0x1               // =-1
  c35ce8: a94363f7     	ldp	x23, x24, [sp, #0x30]
  c35cec: f94023f9     	ldr	x25, [sp, #0x40]
  c35cf0: 17ffffa7     	b	0xc35b8c <.text+0xc34b8c>
  c35cf4: 90006140     	adrp	x0, 0x185d000 <.text+0x185c000>
  c35cf8: 9117c000     	add	x0, x0, #0x5f0
  c35cfc: 97fdd2e5     	bl	0xbaa890 <.text+0xba9890>
  c35d00: b90e5ac0     	str	w0, [x22, #0xe58]
  c35d04: 528000e1     	mov	w1, #0x7                // =7
  c35d08: 97fdd392     	bl	0xbaab50 <.text+0xba9b50>
  c35d0c: b94002a6     	ldr	w6, [x21]
  c35d10: 17ffffb8     	b	0xc35bf0 <.text+0xc34bf0>
  c35d14: b94e5ac0     	ldr	w0, [x22, #0xe58]
  c35d18: 37f80340     	tbnz	w0, #0x1f, 0xc35d80 <.text+0xc34d80>
  c35d1c: b0004963     	adrp	x3, 0x1562000 <.text+0x1561000>
  c35d20: 9137a063     	add	x3, x3, #0xde8
  c35d24: 91116063     	add	x3, x3, #0x458
  c35d28: aa1703e0     	mov	x0, x23
  c35d2c: 90006145     	adrp	x5, 0x185d000 <.text+0x185c000>
  c35d30: 528097c4     	mov	w4, #0x4be              // =1214
  c35d34: 913940a5     	add	x5, x5, #0xe50
  c35d38: 52800002     	mov	w2, #0x0                // =0
  c35d3c: d2800001     	mov	x1, #0x0                // =0
  c35d40: 97fdd6a0     	bl	0xbab7c0 <.text+0xbaa7c0>
  c35d44: 17ffffe6     	b	0xc35cdc <.text+0xc34cdc>
  c35d48: 90006140     	adrp	x0, 0x185d000 <.text+0x185c000>
  c35d4c: 9117c000     	add	x0, x0, #0x5f0
  c35d50: 97fdd2d0     	bl	0xbaa890 <.text+0xba9890>
  c35d54: b90e5ac0     	str	w0, [x22, #0xe58]
  c35d58: 528000e1     	mov	w1, #0x7                // =7
  c35d5c: 97fdd37d     	bl	0xbaab50 <.text+0xba9b50>
  c35d60: 17ffffc2     	b	0xc35c68 <.text+0xc34c68>
  c35d64: 90006140     	adrp	x0, 0x185d000 <.text+0x185c000>
  c35d68: 9117c000     	add	x0, x0, #0x5f0
  c35d6c: 97fdd2c9     	bl	0xbaa890 <.text+0xba9890>
  c35d70: b90e5ac0     	str	w0, [x22, #0xe58]
  c35d74: 528000e1     	mov	w1, #0x7                // =7
  c35d78: 97fdd376     	bl	0xbaab50 <.text+0xba9b50>
  c35d7c: 17ffffca     	b	0xc35ca4 <.text+0xc34ca4>
  c35d80: 90006140     	adrp	x0, 0x185d000 <.text+0x185c000>
  c35d84: 9117c000     	add	x0, x0, #0x5f0
  c35d88: 97fdd2c2     	bl	0xbaa890 <.text+0xba9890>
  c35d8c: b90e5ac0     	str	w0, [x22, #0xe58]
  c35d90: 528000e1     	mov	w1, #0x7                // =7
  c35d94: 97fdd36f     	bl	0xbaab50 <.text+0xba9b50>
  c35d98: 17ffffe1     	b	0xc35d1c <.text+0xc34d1c>
  c35d9c: d503201f     	nop
