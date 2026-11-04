
/tmp/k329.Image:	file format coff-arm64

Disassembly of section .text:

0000000000001000 <.text>:
  c30494: d10783ff     	sub	sp, sp, #0x1e0
  c30498: d5384103     	mrs	x3, SP_EL0
  c3049c: a91a7bfd     	stp	x29, x30, [sp, #0x1a0]
  c304a0: 910683fd     	add	x29, sp, #0x1a0
  c304a4: a91b53f3     	stp	x19, x20, [sp, #0x1b0]
  c304a8: 2a0103f3     	mov	w19, w1
  c304ac: a91c5bf5     	stp	x21, x22, [sp, #0x1c0]
  c304b0: d3507676     	ubfx	x22, x19, #16, #14
  c304b4: a91d63f7     	stp	x23, x24, [sp, #0x1d0]
  c304b8: f9422c61     	ldr	x1, [x3, #0x458]
  c304bc: f900cfe1     	str	x1, [sp, #0x198]
  c304c0: d2800001     	mov	x1, #0x0                // =0
  c304c4: f9406417     	ldr	x23, [x0, #0xc8]
  c304c8: 710422df     	cmp	w22, #0x108
  c304cc: 540038a8     	b.hi	0xc30be0 <.text+0xc2fbe0>
  c304d0: aa0203f5     	mov	x21, x2
  c304d4: 37f00513     	tbnz	w19, #0x1e, 0xc30574 <.text+0xc2f574>
  c304d8: 928001b4     	mov	x20, #-0xe              // =-14
  c304dc: 528a41c0     	mov	w0, #0x520e             // =21006
  c304e0: 72a80100     	movk	w0, #0x4008, lsl #16
  c304e4: 6b00027f     	cmp	w19, w0
  c304e8: 54002e40     	b.eq	0xc30ab0 <.text+0xc2fab0>
  c304ec: 54000ba8     	b.hi	0xc30660 <.text+0xc2f660>
  c304f0: 528a4020     	mov	w0, #0x5201             // =20993
  c304f4: 72a80080     	movk	w0, #0x4004, lsl #16
  c304f8: 6b00027f     	cmp	w19, w0
  c304fc: 540030a0     	b.eq	0xc30b10 <.text+0xc2fb10>
  c30500: 540008c8     	b.hi	0xc30618 <.text+0xc2f618>
  c30504: 528a4520     	mov	w0, #0x5229             // =21033
  c30508: 6b00027f     	cmp	w19, w0
  c3050c: 54003100     	b.eq	0xc30b2c <.text+0xc2fb2c>
  c30510: 540006e9     	b.ls	0xc305ec <.text+0xc2f5ec>
  c30514: 528a4280     	mov	w0, #0x5214             // =21012
  c30518: 72a80020     	movk	w0, #0x4001, lsl #16
  c3051c: 6b00027f     	cmp	w19, w0
  c30520: 54002620     	b.eq	0xc309e4 <.text+0xc2f9e4>
  c30524: 11004000     	add	w0, w0, #0x10
  c30528: 6b00027f     	cmp	w19, w0
  c3052c: 54000521     	b.ne	0xc305d0 <.text+0xc2f5d0>
  c30530: 394243e1     	ldrb	w1, [sp, #0x90]
  c30534: b94012e0     	ldr	w0, [x23, #0x10]
  c30538: 94001dfe     	bl	0xc37d30 <.text+0xc36d30>
  c3053c: 93407c14     	sxtw	x20, w0
  c30540: d5384100     	mrs	x0, SP_EL0
  c30544: f940cfe2     	ldr	x2, [sp, #0x198]
  c30548: f9422c01     	ldr	x1, [x0, #0x458]
  c3054c: eb010042     	subs	x2, x2, x1
  c30550: d2800001     	mov	x1, #0x0                // =0
  c30554: 54003cc1     	b.ne	0xc30cec <.text+0xc2fcec>
  c30558: aa1403e0     	mov	x0, x20
  c3055c: a95a7bfd     	ldp	x29, x30, [sp, #0x1a0]
  c30560: a95b53f3     	ldp	x19, x20, [sp, #0x1b0]
  c30564: a95c5bf5     	ldp	x21, x22, [sp, #0x1c0]
  c30568: a95d63f7     	ldp	x23, x24, [sp, #0x1d0]
  c3056c: 910783ff     	add	sp, sp, #0x1e0
  c30570: d65f03c0     	ret
  c30574: 924036c2     	and	x2, x22, #0x3fff
  c30578: aa1503e1     	mov	x1, x21
  c3057c: 910243e0     	add	x0, sp, #0x90
  c30580: d2800014     	mov	x20, #0x0               // =0
  c30584: 97fffb47     	bl	0xc2f2a0 <.text+0xc2e2a0>
  c30588: b4fffaa0     	cbz	x0, 0xc304dc <.text+0xc2f4dc>
  c3058c: b000b0b8     	adrp	x24, 0x2245000
  c30590: b949cb00     	ldr	w0, [x24, #0x9c8]
  c30594: 37f83a40     	tbnz	w0, #0x1f, 0xc30cdc <.text+0xc2fcdc>
  c30598: d0004983     	adrp	x3, 0x1562000 <.text+0x1561000>
  c3059c: 912f4063     	add	x3, x3, #0xbd0
  c305a0: aa1503e7     	mov	x7, x21
  c305a4: 2a1303e6     	mov	w6, w19
  c305a8: 91272300     	add	x0, x24, #0x9c8
  c305ac: 91074063     	add	x3, x3, #0x1d0
  c305b0: b0006165     	adrp	x5, 0x185d000 <.text+0x185c000>
  c305b4: 52804ac4     	mov	w4, #0x256              // =598
  c305b8: 910c20a5     	add	x5, x5, #0x308
  c305bc: 52800002     	mov	w2, #0x0                // =0
  c305c0: d2800001     	mov	x1, #0x0                // =0
  c305c4: 928001b4     	mov	x20, #-0xe              // =-14
  c305c8: 97fdec7e     	bl	0xbab7c0 <.text+0xbaa7c0>
  c305cc: 17ffffdd     	b	0xc30540 <.text+0xc2f540>
  c305d0: 528a4540     	mov	w0, #0x522a             // =21034
  c305d4: 6b00027f     	cmp	w19, w0
  c305d8: 54001841     	b.ne	0xc308e0 <.text+0xc2f8e0>
  c305dc: b94012e0     	ldr	w0, [x23, #0x10]
  c305e0: 94002084     	bl	0xc387f0 <.text+0xc377f0>
  c305e4: 93407c14     	sxtw	x20, w0
  c305e8: 17ffffd6     	b	0xc30540 <.text+0xc2f540>
  c305ec: 528a43e0     	mov	w0, #0x521f             // =21023
  c305f0: 6b00027f     	cmp	w19, w0
  c305f4: 54002480     	b.eq	0xc30a84 <.text+0xc2fa84>
  c305f8: 54001349     	b.ls	0xc30860 <.text+0xc2f860>
  c305fc: 528a4420     	mov	w0, #0x5221             // =21025
  c30600: 6b00027f     	cmp	w19, w0
  c30604: 540016e1     	b.ne	0xc308e0 <.text+0xc2f8e0>
  c30608: b94012e0     	ldr	w0, [x23, #0x10]
  c3060c: 940014d5     	bl	0xc35960 <.text+0xc34960>
  c30610: 93407c14     	sxtw	x20, w0
  c30614: 17ffffcb     	b	0xc30540 <.text+0xc2f540>
  c30618: 528a4140     	mov	w0, #0x520a             // =21002
  c3061c: 72a80080     	movk	w0, #0x4004, lsl #16
  c30620: 6b00027f     	cmp	w19, w0
  c30624: 54002500     	b.eq	0xc30ac4 <.text+0xc2fac4>
  c30628: 540012a9     	b.ls	0xc3087c <.text+0xc2f87c>
  c3062c: 528a4500     	mov	w0, #0x5228             // =21032
  c30630: 72a80080     	movk	w0, #0x4004, lsl #16
  c30634: 6b00027f     	cmp	w19, w0
  c30638: 54001cc0     	b.eq	0xc309d0 <.text+0xc2f9d0>
  c3063c: 528a40e0     	mov	w0, #0x5207             // =20999
  c30640: 72a80100     	movk	w0, #0x4008, lsl #16
  c30644: 6b00027f     	cmp	w19, w0
  c30648: 54000c21     	b.ne	0xc307cc <.text+0xc2f7cc>
  c3064c: b94012e0     	ldr	w0, [x23, #0x10]
  c30650: 29520be1     	ldp	w1, w2, [sp, #0x90]
  c30654: 9400170f     	bl	0xc36290 <.text+0xc35290>
  c30658: 93407c14     	sxtw	x20, w0
  c3065c: 17ffffb9     	b	0xc30540 <.text+0xc2f540>
  c30660: 528a4180     	mov	w0, #0x520c             // =21004
  c30664: 72b00080     	movk	w0, #0x8004, lsl #16
  c30668: 6b00027f     	cmp	w19, w0
  c3066c: 54002360     	b.eq	0xc30ad8 <.text+0xc2fad8>
  c30670: 540008e9     	b.ls	0xc3078c <.text+0xc2f78c>
  c30674: 528a4220     	mov	w0, #0x5211             // =21009
  c30678: 72b00100     	movk	w0, #0x8008, lsl #16
  c3067c: 6b00027f     	cmp	w19, w0
  c30680: 540023a0     	b.eq	0xc30af4 <.text+0xc2faf4>
  c30684: 54000629     	b.ls	0xc30748 <.text+0xc2f748>
  c30688: 528a4200     	mov	w0, #0x5210             // =21008
  c3068c: 72b00600     	movk	w0, #0x8030, lsl #16
  c30690: 6b00027f     	cmp	w19, w0
  c30694: 54001b20     	b.eq	0xc309f8 <.text+0xc2f9f8>
  c30698: 528a44c0     	mov	w0, #0x5226             // =21030
  c3069c: 72b80100     	movk	w0, #0xc008, lsl #16
  c306a0: 6b00027f     	cmp	w19, w0
  c306a4: 540003c1     	b.ne	0xc3071c <.text+0xc2f71c>
  c306a8: b94012e0     	ldr	w0, [x23, #0x10]
  c306ac: 910243e1     	add	x1, sp, #0x90
  c306b0: 94001e60     	bl	0xc38030 <.text+0xc37030>
  c306b4: 93407c14     	sxtw	x20, w0
  c306b8: f100029f     	cmp	x20, #0x0
  c306bc: 7a40aa60     	ccmp	w19, #0x0, #0x0, ge
  c306c0: 54fff40a     	b.ge	0xc30540 <.text+0xc2f540>
  c306c4: 924036c2     	and	x2, x22, #0x3fff
  c306c8: 910243e1     	add	x1, sp, #0x90
  c306cc: aa1503e0     	mov	x0, x21
  c306d0: 97fffacc     	bl	0xc2f200 <.text+0xc2e200>
  c306d4: 34fff360     	cbz	w0, 0xc30540 <.text+0xc2f540>
  c306d8: b000b0b8     	adrp	x24, 0x2245000
  c306dc: b949cb00     	ldr	w0, [x24, #0x9c8]
  c306e0: 37f83020     	tbnz	w0, #0x1f, 0xc30ce4 <.text+0xc2fce4>
  c306e4: d0004983     	adrp	x3, 0x1562000 <.text+0x1561000>
  c306e8: 912f4063     	add	x3, x3, #0xbd0
  c306ec: aa1503e7     	mov	x7, x21
  c306f0: 2a1303e6     	mov	w6, w19
  c306f4: 91272300     	add	x0, x24, #0x9c8
  c306f8: 91074063     	add	x3, x3, #0x1d0
  c306fc: 928001b4     	mov	x20, #-0xe              // =-14
  c30700: b0006165     	adrp	x5, 0x185d000 <.text+0x185c000>
  c30704: 52806c44     	mov	w4, #0x362              // =866
  c30708: 910c20a5     	add	x5, x5, #0x308
  c3070c: 52800002     	mov	w2, #0x0                // =0
  c30710: d2800001     	mov	x1, #0x0                // =0
  c30714: 97fdec2b     	bl	0xbab7c0 <.text+0xbaa7c0>
  c30718: 17ffff8a     	b	0xc30540 <.text+0xc2f540>
  c3071c: 528a44a0     	mov	w0, #0x5225             // =21029
  c30720: 72b00100     	movk	w0, #0x8008, lsl #16
  c30724: 6b00027f     	cmp	w19, w0
  c30728: 54000dc1     	b.ne	0xc308e0 <.text+0xc2f8e0>
  c3072c: b94012e0     	ldr	w0, [x23, #0x10]
  c30730: 910183e1     	add	x1, sp, #0x60
  c30734: 94001dbb     	bl	0xc37e20 <.text+0xc36e20>
  c30738: 93407c14     	sxtw	x20, w0
  c3073c: f94033e1     	ldr	x1, [sp, #0x60]
  c30740: f9004be1     	str	x1, [sp, #0x90]
  c30744: 17ffffdd     	b	0xc306b8 <.text+0xc2f6b8>
  c30748: 528a4440     	mov	w0, #0x5222             // =21026
  c3074c: 72b00080     	movk	w0, #0x8004, lsl #16
  c30750: 6b00027f     	cmp	w19, w0
  c30754: 54001780     	b.eq	0xc30a44 <.text+0xc2fa44>
  c30758: 11000400     	add	w0, w0, #0x1
  c3075c: 6b00027f     	cmp	w19, w0
  c30760: 54000b81     	b.ne	0xc308d0 <.text+0xc2f8d0>
  c30764: 52800022     	mov	w2, #0x1                // =1
  c30768: b90063ff     	str	wzr, [sp, #0x60]
  c3076c: b94012e0     	ldr	w0, [x23, #0x10]
  c30770: 910183e1     	add	x1, sp, #0x60
  c30774: 9400158b     	bl	0xc35da0 <.text+0xc34da0>
  c30778: 93407c14     	sxtw	x20, w0
  c3077c: 35fff9e0     	cbnz	w0, 0xc306b8 <.text+0xc2f6b8>
  c30780: b94063e0     	ldr	w0, [sp, #0x60]
  c30784: b90093e0     	str	w0, [sp, #0x90]
  c30788: 17ffffcc     	b	0xc306b8 <.text+0xc2f6b8>
  c3078c: 528a4240     	mov	w0, #0x5212             // =21010
  c30790: 72a80700     	movk	w0, #0x4038, lsl #16
  c30794: 6b00027f     	cmp	w19, w0
  c30798: 540015c0     	b.eq	0xc30a50 <.text+0xc2fa50>
  c3079c: 54000409     	b.ls	0xc3081c <.text+0xc2f81c>
  c307a0: 528a4060     	mov	w0, #0x5203             // =20995
  c307a4: 72b00080     	movk	w0, #0x8004, lsl #16
  c307a8: 6b00027f     	cmp	w19, w0
  c307ac: 540013c0     	b.eq	0xc30a24 <.text+0xc2fa24>
  c307b0: 11000400     	add	w0, w0, #0x1
  c307b4: 6b00027f     	cmp	w19, w0
  c307b8: 540001c1     	b.ne	0xc307f0 <.text+0xc2f7f0>
  c307bc: 9400da8d     	bl	0xc671f0 <.text+0xc661f0>
  c307c0: d2800014     	mov	x20, #0x0               // =0
  c307c4: b90093e0     	str	w0, [sp, #0x90]
  c307c8: 17ffffbf     	b	0xc306c4 <.text+0xc2f6c4>
  c307cc: 528a44e0     	mov	w0, #0x5227             // =21031
  c307d0: 72a80080     	movk	w0, #0x4004, lsl #16
  c307d4: 6b00027f     	cmp	w19, w0
  c307d8: 54000841     	b.ne	0xc308e0 <.text+0xc2f8e0>
  c307dc: b94012e0     	ldr	w0, [x23, #0x10]
  c307e0: b94093e1     	ldr	w1, [sp, #0x90]
  c307e4: 94001ecb     	bl	0xc38310 <.text+0xc37310>
  c307e8: 93407c14     	sxtw	x20, w0
  c307ec: 17ffff55     	b	0xc30540 <.text+0xc2f540>
  c307f0: 51000800     	sub	w0, w0, #0x2
  c307f4: 6b00027f     	cmp	w19, w0
  c307f8: 54000741     	b.ne	0xc308e0 <.text+0xc2f8e0>
  c307fc: 52800002     	mov	w2, #0x0                // =0
  c30800: b90063ff     	str	wzr, [sp, #0x60]
  c30804: b94012e0     	ldr	w0, [x23, #0x10]
  c30808: 910183e1     	add	x1, sp, #0x60
  c3080c: 940014a9     	bl	0xc35ab0 <.text+0xc34ab0>
  c30810: 93407c14     	sxtw	x20, w0
  c30814: 35fff520     	cbnz	w0, 0xc306b8 <.text+0xc2f6b8>
  c30818: 17ffffda     	b	0xc30780 <.text+0xc2f780>
  c3081c: 528a4100     	mov	w0, #0x5208             // =21000
  c30820: 72a80280     	movk	w0, #0x4014, lsl #16
  c30824: 6b00027f     	cmp	w19, w0
  c30828: 54001360     	b.eq	0xc30a94 <.text+0xc2fa94>
  c3082c: 528a4400     	mov	w0, #0x5220             // =21024
  c30830: 72a80400     	movk	w0, #0x4020, lsl #16
  c30834: 6b00027f     	cmp	w19, w0
  c30838: 54000381     	b.ne	0xc308a8 <.text+0xc2f8a8>
  c3083c: a94907e0     	ldp	x0, x1, [sp, #0x90]
  c30840: a90207e0     	stp	x0, x1, [sp, #0x20]
  c30844: a94a0fe2     	ldp	x2, x3, [sp, #0xa0]
  c30848: a9030fe2     	stp	x2, x3, [sp, #0x30]
  c3084c: 910083e1     	add	x1, sp, #0x20
  c30850: b94012e0     	ldr	w0, [x23, #0x10]
  c30854: 94001cbb     	bl	0xc37b40 <.text+0xc36b40>
  c30858: 93407c14     	sxtw	x20, w0
  c3085c: 17ffff39     	b	0xc30540 <.text+0xc2f540>
  c30860: 528a4260     	mov	w0, #0x5213             // =21011
  c30864: 6b00027f     	cmp	w19, w0
  c30868: 540005c1     	b.ne	0xc30920 <.text+0xc2f920>
  c3086c: b94012e0     	ldr	w0, [x23, #0x10]
  c30870: 94001a84     	bl	0xc37280 <.text+0xc36280>
  c30874: 93407c14     	sxtw	x20, w0
  c30878: 17ffff32     	b	0xc30540 <.text+0xc2f540>
  c3087c: 51001000     	sub	w0, w0, #0x4
  c30880: 6b00027f     	cmp	w19, w0
  c30884: 54000d60     	b.eq	0xc30a30 <.text+0xc2fa30>
  c30888: 11000c00     	add	w0, w0, #0x3
  c3088c: 6b00027f     	cmp	w19, w0
  c30890: 54000561     	b.ne	0xc3093c <.text+0xc2f93c>
  c30894: b94012e0     	ldr	w0, [x23, #0x10]
  c30898: b94093e1     	ldr	w1, [sp, #0x90]
  c3089c: 94001769     	bl	0xc36640 <.text+0xc35640>
  c308a0: 93407c14     	sxtw	x20, w0
  c308a4: 17ffff27     	b	0xc30540 <.text+0xc2f540>
  c308a8: 528a4160     	mov	w0, #0x520b             // =21003
  c308ac: 72a80180     	movk	w0, #0x400c, lsl #16
  c308b0: 6b00027f     	cmp	w19, w0
  c308b4: 54000161     	b.ne	0xc308e0 <.text+0xc2f8e0>
  c308b8: b94012e0     	ldr	w0, [x23, #0x10]
  c308bc: b9409be2     	ldr	w2, [sp, #0x98]
  c308c0: f9404be1     	ldr	x1, [sp, #0x90]
  c308c4: 9400184b     	bl	0xc369f0 <.text+0xc359f0>
  c308c8: 93407c14     	sxtw	x20, w0
  c308cc: 17ffff1d     	b	0xc30540 <.text+0xc2f540>
  c308d0: 51005800     	sub	w0, w0, #0x16
  c308d4: 6b00027f     	cmp	w19, w0
  c308d8: 54ffef00     	b.eq	0xc306b8 <.text+0xc2f6b8>
  c308dc: d503201f     	nop
  c308e0: b000b0b8     	adrp	x24, 0x2245000
  c308e4: b949cb00     	ldr	w0, [x24, #0x9c8]
  c308e8: 37f819e0     	tbnz	w0, #0x1f, 0xc30c24 <.text+0xc2fc24>
  c308ec: d0004983     	adrp	x3, 0x1562000 <.text+0x1561000>
  c308f0: 912f4063     	add	x3, x3, #0xbd0
  c308f4: 2a1303e6     	mov	w6, w19
  c308f8: 91272300     	add	x0, x24, #0x9c8
  c308fc: 91074063     	add	x3, x3, #0x1d0
  c30900: b0006165     	adrp	x5, 0x185d000 <.text+0x185c000>
  c30904: 52806aa4     	mov	w4, #0x355              // =853
  c30908: 910ce0a5     	add	x5, x5, #0x338
  c3090c: 52800002     	mov	w2, #0x0                // =0
  c30910: d2800001     	mov	x1, #0x0                // =0
  c30914: 92800314     	mov	x20, #-0x19             // =-25
  c30918: 97fdebaa     	bl	0xbab7c0 <.text+0xbaa7c0>
  c3091c: 17ffff09     	b	0xc30540 <.text+0xc2f540>
  c30920: 528a43c0     	mov	w0, #0x521e             // =21022
  c30924: 6b00027f     	cmp	w19, w0
  c30928: 54fffdc1     	b.ne	0xc308e0 <.text+0xc2f8e0>
  c3092c: b94012e0     	ldr	w0, [x23, #0x10]
  c30930: 94001b7d     	bl	0xc37724 <.text+0xc36724>
  c30934: 93407c14     	sxtw	x20, w0
  c30938: 17ffff02     	b	0xc30540 <.text+0xc2f540>
  c3093c: 51001000     	sub	w0, w0, #0x4
  c30940: 6b00027f     	cmp	w19, w0
  c30944: 54fffce1     	b.ne	0xc308e0 <.text+0xc2f8e0>
  c30948: d0004994     	adrp	x20, 0x1562000 <.text+0x1561000>
  c3094c: 912f4294     	add	x20, x20, #0xbd0
  c30950: b94012f5     	ldr	w21, [x23, #0x10]
  c30954: 9103c293     	add	x19, x20, #0xf0
  c30958: b94093e6     	ldr	w6, [sp, #0x90]
  c3095c: 52800027     	mov	w7, #0x1                // =1
  c30960: 14000005     	b	0xc30974 <.text+0xc2f974>
  c30964: f9401660     	ldr	x0, [x19, #0x28]
  c30968: 91008273     	add	x19, x19, #0x20
  c3096c: b4000e80     	cbz	x0, 0xc30b3c <.text+0xc2fb3c>
  c30970: b9400267     	ldr	w7, [x19]
  c30974: 6b0600ff     	cmp	w7, w6
  c30978: 54ffff61     	b.ne	0xc30964 <.text+0xc2f964>
  c3097c: b000b0b8     	adrp	x24, 0x2245000
  c30980: b949cb00     	ldr	w0, [x24, #0x9c8]
  c30984: 37f81700     	tbnz	w0, #0x1f, 0xc30c64 <.text+0xc2fc64>
  c30988: f9400666     	ldr	x6, [x19, #0x8]
  c3098c: 9107a283     	add	x3, x20, #0x1e8
  c30990: b9401260     	ldr	w0, [x19, #0x10]
  c30994: b0006165     	adrp	x5, 0x185d000 <.text+0x185c000>
  c30998: f9400e61     	ldr	x1, [x19, #0x18]
  c3099c: f90003e1     	str	x1, [sp]
  c309a0: b9000be0     	str	w0, [sp, #0x8]
  c309a4: d2800001     	mov	x1, #0x0                // =0
  c309a8: 910fe0a5     	add	x5, x5, #0x3f8
  c309ac: 91272300     	add	x0, x24, #0x9c8
  c309b0: 52801d44     	mov	w4, #0xea               // =234
  c309b4: 52800082     	mov	w2, #0x4                // =4
  c309b8: 97fdeb82     	bl	0xbab7c0 <.text+0xbaa7c0>
  c309bc: b9401261     	ldr	w1, [x19, #0x10]
  c309c0: 2a1503e0     	mov	w0, w21
  c309c4: 94001563     	bl	0xc35f50 <.text+0xc34f50>
  c309c8: 93407c14     	sxtw	x20, w0
  c309cc: 17fffedd     	b	0xc30540 <.text+0xc2f540>
  c309d0: b94012e0     	ldr	w0, [x23, #0x10]
  c309d4: b94093e1     	ldr	w1, [sp, #0x90]
  c309d8: 94001eba     	bl	0xc384c0 <.text+0xc374c0>
  c309dc: 93407c14     	sxtw	x20, w0
  c309e0: 17fffed8     	b	0xc30540 <.text+0xc2f540>
  c309e4: 394243e1     	ldrb	w1, [sp, #0x90]
  c309e8: b94012e0     	ldr	w0, [x23, #0x10]
  c309ec: 94001afa     	bl	0xc375d4 <.text+0xc365d4>
  c309f0: 93407c14     	sxtw	x20, w0
  c309f4: 17fffed3     	b	0xc30540 <.text+0xc2f540>
  c309f8: b94012e0     	ldr	w0, [x23, #0x10]
  c309fc: 910183e1     	add	x1, sp, #0x60
  c30a00: 9400194c     	bl	0xc36f30 <.text+0xc35f30>
  c30a04: 93407c14     	sxtw	x20, w0
  c30a08: a94617e4     	ldp	x4, x5, [sp, #0x60]
  c30a0c: a90917e4     	stp	x4, x5, [sp, #0x90]
  c30a10: a9470fe2     	ldp	x2, x3, [sp, #0x70]
  c30a14: a90a0fe2     	stp	x2, x3, [sp, #0xa0]
  c30a18: a94807e0     	ldp	x0, x1, [sp, #0x80]
  c30a1c: a90b07e0     	stp	x0, x1, [sp, #0xb0]
  c30a20: 17ffff26     	b	0xc306b8 <.text+0xc2f6b8>
  c30a24: 52800022     	mov	w2, #0x1                // =1
  c30a28: b90063ff     	str	wzr, [sp, #0x60]
  c30a2c: 17ffff76     	b	0xc30804 <.text+0xc2f804>
  c30a30: 394243e1     	ldrb	w1, [sp, #0x90]
  c30a34: b94012e0     	ldr	w0, [x23, #0x10]
  c30a38: 940015ab     	bl	0xc360e4 <.text+0xc350e4>
  c30a3c: 93407c14     	sxtw	x20, w0
  c30a40: 17fffec0     	b	0xc30540 <.text+0xc2f540>
  c30a44: 52800002     	mov	w2, #0x0                // =0
  c30a48: b90063ff     	str	wzr, [sp, #0x60]
  c30a4c: 17ffff48     	b	0xc3076c <.text+0xc2f76c>
  c30a50: a94907e0     	ldp	x0, x1, [sp, #0x90]
  c30a54: a90207e0     	stp	x0, x1, [sp, #0x20]
  c30a58: a94a0fe2     	ldp	x2, x3, [sp, #0xa0]
  c30a5c: a9030fe2     	stp	x2, x3, [sp, #0x30]
  c30a60: 910083e1     	add	x1, sp, #0x20
  c30a64: a94b0fe2     	ldp	x2, x3, [sp, #0xb0]
  c30a68: a9040fe2     	stp	x2, x3, [sp, #0x40]
  c30a6c: f94063e0     	ldr	x0, [sp, #0xc0]
  c30a70: f9002be0     	str	x0, [sp, #0x50]
  c30a74: b94012e0     	ldr	w0, [x23, #0x10]
  c30a78: 94001a67     	bl	0xc37414 <.text+0xc36414>
  c30a7c: 93407c14     	sxtw	x20, w0
  c30a80: 17fffeb0     	b	0xc30540 <.text+0xc2f540>
  c30a84: b94012e0     	ldr	w0, [x23, #0x10]
  c30a88: 94001bc2     	bl	0xc37990 <.text+0xc36990>
  c30a8c: 93407c14     	sxtw	x20, w0
  c30a90: 17fffeac     	b	0xc30540 <.text+0xc2f540>
  c30a94: b94012e0     	ldr	w0, [x23, #0x10]
  c30a98: 29520be1     	ldp	w1, w2, [sp, #0x90]
  c30a9c: 295313e3     	ldp	w3, w4, [sp, #0x98]
  c30aa0: b940a3e5     	ldr	w5, [sp, #0xa0]
  c30aa4: 9400166f     	bl	0xc36460 <.text+0xc35460>
  c30aa8: 93407c14     	sxtw	x20, w0
  c30aac: 17fffea5     	b	0xc30540 <.text+0xc2f540>
  c30ab0: b94012e0     	ldr	w0, [x23, #0x10]
  c30ab4: f9404be1     	ldr	x1, [sp, #0x90]
  c30ab8: 940018ae     	bl	0xc36d70 <.text+0xc35d70>
  c30abc: 93407c14     	sxtw	x20, w0
  c30ac0: 17fffea0     	b	0xc30540 <.text+0xc2f540>
  c30ac4: b94012e0     	ldr	w0, [x23, #0x10]
  c30ac8: b94093e1     	ldr	w1, [sp, #0x90]
  c30acc: 9400175d     	bl	0xc36840 <.text+0xc35840>
  c30ad0: 93407c14     	sxtw	x20, w0
  c30ad4: 17fffe9b     	b	0xc30540 <.text+0xc2f540>
  c30ad8: b94012e0     	ldr	w0, [x23, #0x10]
  c30adc: 910183e1     	add	x1, sp, #0x60
  c30ae0: 94001839     	bl	0xc36bc4 <.text+0xc35bc4>
  c30ae4: b94063e1     	ldr	w1, [sp, #0x60]
  c30ae8: 93407c14     	sxtw	x20, w0
  c30aec: b90093e1     	str	w1, [sp, #0x90]
  c30af0: 17fffef2     	b	0xc306b8 <.text+0xc2f6b8>
  c30af4: b94012e0     	ldr	w0, [x23, #0x10]
  c30af8: 910183e1     	add	x1, sp, #0x60
  c30afc: 94001931     	bl	0xc36fc0 <.text+0xc35fc0>
  c30b00: 93407c14     	sxtw	x20, w0
  c30b04: f94033e1     	ldr	x1, [sp, #0x60]
  c30b08: f9004be1     	str	x1, [sp, #0x90]
  c30b0c: 17fffeeb     	b	0xc306b8 <.text+0xc2f6b8>
  c30b10: b94093e0     	ldr	w0, [sp, #0x90]
  c30b14: 37f802e0     	tbnz	w0, #0x1f, 0xc30b70 <.text+0xc2fb70>
  c30b18: 940012ab     	bl	0xc355c4 <.text+0xc345c4>
  c30b1c: b94093e1     	ldr	w1, [sp, #0x90]
  c30b20: 93407c14     	sxtw	x20, w0
  c30b24: b90012e1     	str	w1, [x23, #0x10]
  c30b28: 17fffe86     	b	0xc30540 <.text+0xc2f540>
  c30b2c: b94012e0     	ldr	w0, [x23, #0x10]
  c30b30: 94001ecc     	bl	0xc38660 <.text+0xc37660>
  c30b34: 93407c14     	sxtw	x20, w0
  c30b38: 17fffe82     	b	0xc30540 <.text+0xc2f540>
  c30b3c: b000b0b8     	adrp	x24, 0x2245000
  c30b40: b949cb00     	ldr	w0, [x24, #0x9c8]
  c30b44: 37f807e0     	tbnz	w0, #0x1f, 0xc30c40 <.text+0xc2fc40>
  c30b48: d2800001     	mov	x1, #0x0                // =0
  c30b4c: 9107a283     	add	x3, x20, #0x1e8
  c30b50: 91272300     	add	x0, x24, #0x9c8
  c30b54: b0006165     	adrp	x5, 0x185d000 <.text+0x185c000>
  c30b58: 52801e04     	mov	w4, #0xf0               // =240
  c30b5c: 911060a5     	add	x5, x5, #0x418
  c30b60: 52800002     	mov	w2, #0x0                // =0
  c30b64: 97fdeb17     	bl	0xbab7c0 <.text+0xbaa7c0>
  c30b68: 12800001     	mov	w1, #-0x1               // =-1
  c30b6c: 17ffff95     	b	0xc309c0 <.text+0xc2f9c0>
  c30b70: b000b0b8     	adrp	x24, 0x2245000
  c30b74: b949cb01     	ldr	w1, [x24, #0x9c8]
  c30b78: 37f80941     	tbnz	w1, #0x1f, 0xc30ca0 <.text+0xc2fca0>
  c30b7c: 91272313     	add	x19, x24, #0x9c8
  c30b80: d0004994     	adrp	x20, 0x1562000 <.text+0x1561000>
  c30b84: 912f4294     	add	x20, x20, #0xbd0
  c30b88: 2a0003e6     	mov	w6, w0
  c30b8c: 91074283     	add	x3, x20, #0x1d0
  c30b90: aa1303e0     	mov	x0, x19
  c30b94: b0006165     	adrp	x5, 0x185d000 <.text+0x185c000>
  c30b98: 52804c24     	mov	w4, #0x261              // =609
  c30b9c: 910ee0a5     	add	x5, x5, #0x3b8
  c30ba0: 52800042     	mov	w2, #0x2                // =2
  c30ba4: d2800001     	mov	x1, #0x0                // =0
  c30ba8: 97fdeb06     	bl	0xbab7c0 <.text+0xbaa7c0>
  c30bac: b949cb00     	ldr	w0, [x24, #0x9c8]
  c30bb0: 37f806a0     	tbnz	w0, #0x1f, 0xc30c84 <.text+0xc2fc84>
  c30bb4: 91074283     	add	x3, x20, #0x1d0
  c30bb8: aa1303e0     	mov	x0, x19
  c30bbc: b0006165     	adrp	x5, 0x185d000 <.text+0x185c000>
  c30bc0: 52804c44     	mov	w4, #0x262              // =610
  c30bc4: 910f60a5     	add	x5, x5, #0x3d8
  c30bc8: 52800042     	mov	w2, #0x2                // =2
  c30bcc: d2800001     	mov	x1, #0x0                // =0
  c30bd0: 97fdeafc     	bl	0xbab7c0 <.text+0xbaa7c0>
  c30bd4: 52800000     	mov	w0, #0x0                // =0
  c30bd8: b90093ff     	str	wzr, [sp, #0x90]
  c30bdc: 17ffffcf     	b	0xc30b18 <.text+0xc2fb18>
  c30be0: b000b0b8     	adrp	x24, 0x2245000
  c30be4: b949cb00     	ldr	w0, [x24, #0x9c8]
  c30be8: 37f806c0     	tbnz	w0, #0x1f, 0xc30cc0 <.text+0xc2fcc0>
  c30bec: d0004983     	adrp	x3, 0x1562000 <.text+0x1561000>
  c30bf0: 912f4063     	add	x3, x3, #0xbd0
  c30bf4: 2a1603e6     	mov	w6, w22
  c30bf8: 91272300     	add	x0, x24, #0x9c8
  c30bfc: 91074063     	add	x3, x3, #0x1d0
  c30c00: 90006145     	adrp	x5, 0x1858000 <.text+0x1857000>
  c30c04: d2802107     	mov	x7, #0x108              // =264
  c30c08: 910ea0a5     	add	x5, x5, #0x3a8
  c30c0c: 52804964     	mov	w4, #0x24b              // =587
  c30c10: 52800002     	mov	w2, #0x0                // =0
  c30c14: d2800001     	mov	x1, #0x0                // =0
  c30c18: 928001b4     	mov	x20, #-0xe              // =-14
  c30c1c: 97fdeae9     	bl	0xbab7c0 <.text+0xbaa7c0>
  c30c20: 17fffe48     	b	0xc30540 <.text+0xc2f540>
  c30c24: b0006160     	adrp	x0, 0x185d000 <.text+0x185c000>
  c30c28: 9108e000     	add	x0, x0, #0x238
  c30c2c: 97fde719     	bl	0xbaa890 <.text+0xba9890>
  c30c30: b909cb00     	str	w0, [x24, #0x9c8]
  c30c34: 528000e1     	mov	w1, #0x7                // =7
  c30c38: 97fde7c6     	bl	0xbaab50 <.text+0xba9b50>
  c30c3c: 17ffff2c     	b	0xc308ec <.text+0xc2f8ec>
  c30c40: b0006160     	adrp	x0, 0x185d000 <.text+0x185c000>
  c30c44: 9108e000     	add	x0, x0, #0x238
  c30c48: b9001fe6     	str	w6, [sp, #0x1c]
  c30c4c: 97fde711     	bl	0xbaa890 <.text+0xba9890>
  c30c50: 528000e1     	mov	w1, #0x7                // =7
  c30c54: b909cb00     	str	w0, [x24, #0x9c8]
  c30c58: 97fde7be     	bl	0xbaab50 <.text+0xba9b50>
  c30c5c: b9401fe6     	ldr	w6, [sp, #0x1c]
  c30c60: 17ffffba     	b	0xc30b48 <.text+0xc2fb48>
  c30c64: b0006160     	adrp	x0, 0x185d000 <.text+0x185c000>
  c30c68: 9108e000     	add	x0, x0, #0x238
  c30c6c: 97fde709     	bl	0xbaa890 <.text+0xba9890>
  c30c70: b909cb00     	str	w0, [x24, #0x9c8]
  c30c74: 528000e1     	mov	w1, #0x7                // =7
  c30c78: 97fde7b6     	bl	0xbaab50 <.text+0xba9b50>
  c30c7c: b9400267     	ldr	w7, [x19]
  c30c80: 17ffff42     	b	0xc30988 <.text+0xc2f988>
  c30c84: b0006160     	adrp	x0, 0x185d000 <.text+0x185c000>
  c30c88: 9108e000     	add	x0, x0, #0x238
  c30c8c: 97fde701     	bl	0xbaa890 <.text+0xba9890>
  c30c90: b909cb00     	str	w0, [x24, #0x9c8]
  c30c94: 528000e1     	mov	w1, #0x7                // =7
  c30c98: 97fde7ae     	bl	0xbaab50 <.text+0xba9b50>
  c30c9c: 17ffffc6     	b	0xc30bb4 <.text+0xc2fbb4>
  c30ca0: b0006160     	adrp	x0, 0x185d000 <.text+0x185c000>
  c30ca4: 9108e000     	add	x0, x0, #0x238
  c30ca8: 97fde6fa     	bl	0xbaa890 <.text+0xba9890>
  c30cac: b909cb00     	str	w0, [x24, #0x9c8]
  c30cb0: 528000e1     	mov	w1, #0x7                // =7
  c30cb4: 97fde7a7     	bl	0xbaab50 <.text+0xba9b50>
  c30cb8: b94093e0     	ldr	w0, [sp, #0x90]
  c30cbc: 17ffffb0     	b	0xc30b7c <.text+0xc2fb7c>
  c30cc0: b0006160     	adrp	x0, 0x185d000 <.text+0x185c000>
  c30cc4: 9108e000     	add	x0, x0, #0x238
  c30cc8: 97fde6f2     	bl	0xbaa890 <.text+0xba9890>
  c30ccc: b909cb00     	str	w0, [x24, #0x9c8]
  c30cd0: 528000e1     	mov	w1, #0x7                // =7
  c30cd4: 97fde79f     	bl	0xbaab50 <.text+0xba9b50>
  c30cd8: 17ffffc5     	b	0xc30bec <.text+0xc2fbec>
  c30cdc: 97fff935     	bl	0xc2f1b0 <.text+0xc2e1b0>
  c30ce0: 17fffe2e     	b	0xc30598 <.text+0xc2f598>
  c30ce4: 97fff933     	bl	0xc2f1b0 <.text+0xc2e1b0>
  c30ce8: 17fffe7f     	b	0xc306e4 <.text+0xc2f6e4>
  c30cec: 97cffbb9     	bl	0x2fbd0 <.text+0x2ebd0>
  c30cf0: a9bf7bfd     	stp	x29, x30, [sp, #-0x10]!
  c30cf4: 910003fd     	mov	x29, sp
  c30cf8: 97fffde7     	bl	0xc30494 <.text+0xc2f494>
  c30cfc: a8c17bfd     	ldp	x29, x30, [sp], #0x10
  c30d00: d65f03c0     	ret
