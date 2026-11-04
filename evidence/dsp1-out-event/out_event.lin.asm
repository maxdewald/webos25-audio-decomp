out_event:
60123814  36c101            entry    a1, 0xe0
60123817  0c0c              movi.n   a12, 0
60123819  81e916            l32r     a8, . 0x600e93c0   ; [600e93c0]=0x600067d0 'out_event'
6012381c  619d16            l32r     a6, . 0x600e9290   ; [600e9290]=0x4ff900cc
6012381f  52a0c0            movi     a5, 0xc0
60123822  722243            l32i     a7, a2, 0x10c
60123825  d22244            l32i     a13, a2, 0x110
60123828  b834              l32i.n   a11, a4, 0xc
6012382a  e22240            l32i     a14, a2, 0x100
6012382d  f22245            l32i     a15, a2, 0x114
60123830  0e1bb000410c400d  flix     opaque8 lo=00b01b0e hi=0d400c41
60123838  262b56            beqi     a11, 2, 0x60123892
6012383b  0cdc              movi.n   a12, 0xd
6012383d  c79b41            bne      a11, a12, 0x60123882
60123840  d806              l32i.n   a13, a6, 0
60123842  37ed75            bbsi     a13, 3, 0x601238bb
60123845  40a420            or       a10, a4, a4
60123848  b1df16            l32r     a11, . 0x600e93c4   ; [600e93c4]=0x600b190c 'is_first_data'
6012384b  a58acf            call8    0x600f30f4   ; <aud_event_has_name>
6012384e  164a02            beqz     a10, 0x60123876
60123851  e22600            l32i     a14, a6, 0
60123854  52d201            addmi    a5, a2, 0x100
60123857  52c5c0            addi     a5, a5, -0x40
6012385a  37ee7a            bbsi     a14, 3, 0x601238d8
6012385d  0c08              movi.n   a8, 0
6012385f  0c19              movi.n   a9, 1
60123861  9245c8            s8i      a9, a5, 0xc8
60123864  826539            s32i     a8, a5, 0xe4
60123867  826538            s32i     a8, a5, 0xe0
6012386a  826537            s32i     a8, a5, 0xdc
6012386d  826536            s32i     a8, a5, 0xd8
60123870  826535            s32i     a8, a5, 0xd4
60123873  826534            s32i     a8, a5, 0xd0
60123876  40a420            or       a10, a4, a4
60123879  a5a9ce            call8    0x600f2314   ; <aud_object_put>
6012387c  22a000            movi     a2, 0
6012387f  900000            retw     
60123882  20a220            or       a10, a2, a2
60123885  30b320            or       a11, a3, a3
60123888  40c420            or       a12, a4, a4
6012388b  e573db            call8    0x600fefc8   ; <aud_module_default_event>
6012388e  2d0a              mov.n    a2, a10
60123890  1df0              retw.n   
60123892  a806              l32i.n   a10, a6, 0
60123894  3e2ab0704116000d  flix     opaque8 lo=70b02a3e hi=0d001641
6012389c  c2626a            s32i     a12, a2, 0x1a8
6012389f  20a220            or       a10, a2, a2
601238a2  e509ff            call8    0x60122940   ; <out_stop>
601238a5  a2221a            l32i     a10, a2, 0x68
601238a8  b22201            l32i     a11, a2, 4
601238ab  c1c716            l32r     a12, . 0x600e93c8   ; [600e93c8]=0x600a5274 'last'
601238ae  d2a000            movi     a13, 0
601238b1  0c0e              movi.n   a14, 0
601238b3  653edf            call8    0x60102c98   ; <aimc_raise_named_event>
601238b6  b834              l32i.n   a11, a4, 0xc
601238b8  c6dfff            j        0x6012383b
601238bb  a17616            l32r     a10, . 0x600e9294   ; [600e9294]=0x60280cf0
601238be  b2a003            movi     a11, 3
601238c1  c1bf16            l32r     a12, . 0x600e93c0   ; [600e93c0]=0x600067d0 'out_event'
601238c4  50f280            add      a15, a2, a5
601238c7  f22f0c            l32i     a15, a15, 0x30
601238ca  d2a7df            movi     a13, 0x7df
601238cd  e1bf16            l32r     a14, . 0x600e93cc   ; [600e93cc]=0x600b54a4 '%8s: received custom event\n'
601238d0  f80f              l32i.n   a15, a15, 0
601238d2  a5f2df            call8    0x601037fc   ; <logmm_printl>
601238d5  06dbff            j        0x60123845
601238d8  a16f16            l32r     a10, . 0x600e9294   ; [600e9294]=0x60280cf0
601238db  c1b916            l32r     a12, . 0x600e93c0   ; [600e93c0]=0x600067d0 'out_event'
601238de  0c3b              movi.n   a11, 3
601238e0  f2223c            l32i     a15, a2, 0xf0
601238e3  d2a7e3            movi     a13, 0x7e3
601238e6  e1ba16            l32r     a14, . 0x600e93d0   ; [600e93d0]=0x600b54c0 '%8s: get is_first_data event\n'
601238e9  f80f              l32i.n   a15, a15, 0
601238eb  25f1df            call8    0x601037fc   ; <logmm_printl>
601238ee  c6daff            j        0x6012385d
601238f1  000000            ill      
601238f4  c1b916            l32r     a12, . 0x600e93d8   ; [600e93d8]=0x600a40e0 'pcm'
601238f7  f2612a            s32i     a15, a1, 0xa8
601238fa  e26129            s32i     a14, a1, 0xa4
601238fd  d26128            s32i     a13, a1, 0xa0
60123900  0c0b              movi.n   a11, 0
60123902  a858              l32i.n   a10, a8, 0x14
60123904  9868              l32i.n   a9, a8, 0x18
60123906  99a1              s32i.n   a9, a1, 0x28
60123908  a991              s32i.n   a10, a1, 0x24
6012390a  b981              s32i.n   a11, a1, 0x20
6012390c  ad04              mov.n    a10, a4
6012390e  b1b116            l32r     a11, . 0x600e93d4   ; [600e93d4]=0x600aeb54 'codec'
60123911  8878              l32i.n   a8, a8, 0x1c
60123913  89b1              s32i.n   a8, a1, 0x2c
60123915  656fcf            call8    0x600f300c   ; <aud_event_value_get_str>
60123918  b1b116            l32r     a11, . 0x600e93dc   ; [600e93dc]=0x600b8a64 'channels'
6012391b  a2612c            s32i     a10, a1, 0xb0
6012391e  0c2c              movi.n   a12, 2
60123920  ad04              mov.n    a10, a4
60123922  6568cf            call8    0x600f2fa8   ; <aud_event_value_get_int>
60123925  b1ae16            l32r     a11, . 0x600e93e0   ; [600e93e0]=0x600ae4fc 'data-rate'
60123928  a991              s32i.n   a10, a1, 0x24
6012392a  c18116            l32r     a12, . 0x600e9330   ; [600e9330]=0x0000bb80
6012392d  ad04              mov.n    a10, a4
6012392f  a567cf            call8    0x600f2fa8   ; <aud_event_value_get_int>
60123932  b1ac16            l32r     a11, . 0x600e93e4   ; [600e93e4]=0x600a867c 'acmod'
60123935  a9a1              s32i.n   a10, a1, 0x28
60123937  0c2c              movi.n   a12, 2
60123939  ad04              mov.n    a10, a4
6012393b  e566cf            call8    0x600f2fa8   ; <aud_event_value_get_int>
6012393e  b1aa16            l32r     a11, . 0x600e93e8   ; [600e93e8]=0x600cc6c0 'es'
60123941  a9b1              s32i.n   a10, a1, 0x2c
60123943  0c0c              movi.n   a12, 0
60123945  ad04              mov.n    a10, a4
60123947  2569cf            call8    0x600f2fd8   ; <aud_event_value_get_bool>
6012394a  b1a816            l32r     a11, . 0x600e93ec   ; [600e93ec]=0x600b5330 'ch-mapping-lfe-c'
6012394d  52d201            addmi    a5, a2, 0x100
60123950  0c0c              movi.n   a12, 0
60123952  52c5c0            addi     a5, a5, -0x40
60123955  a245a4            s8i      a10, a5, 0xa4
60123958  ad04              mov.n    a10, a4
6012395a  e567cf            call8    0x600f2fd8   ; <aud_event_value_get_bool>
6012395d  b1a416            l32r     a11, . 0x600e93f0   ; [600e93f0]=0x600ad7fc 'bits-per-sample'
60123960  2c0c              movi.n   a12, 0x20
60123962  a24558            s8i      a10, a5, 0x58
60123965  8891              l32i.n   a8, a1, 0x24
60123967  ad04              mov.n    a10, a4
60123969  826510            s32i     a8, a5, 0x40
6012396c  a563cf            call8    0x600f2fa8   ; <aud_event_value_get_int>
6012396f  a26511            s32i     a10, a5, 0x44
60123972  c8a1              l32i.n   a12, a1, 0x28
60123974  0c0d              movi.n   a13, 0
60123976  9205a4            l8ui     a9, a5, 0xa4
60123979  d2653a            s32i     a13, a5, 0xe8
6012397c  c26514            s32i     a12, a5, 0x50
6012397f  16f91c            beqz     a9, 0x60123b52
60123982  a2212c            l32i     a10, a1, 0xb0
60123985  b19416            l32r     a11, . 0x600e93d8   ; [600e93d8]=0x600a40e0 'pcm'
60123988  c2a003            movi     a12, 3
6012398b  81f515            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
6012398e  e00800            callx8   a8
60123991  565a16            bnez     a10, 0x60123afa
60123994  0c0e              movi.n   a14, 0
60123996  e981              s32i.n   a14, a1, 0x20
60123998  922240            l32i     a9, a2, 0x100
6012399b  0e29b0b0412f400d  flix     opaque8 lo=b0b0290e hi=0d402f41
601239a3  b19416            l32r     a11, . 0x600e93f4   ; [600e93f4]=0x60122a38 <output_spdif> '6a'
601239a6  92612e            s32i     a9, a1, 0xb8
601239a9  0ccc              movi.n   a12, 0xc
601239ab  b2624c            s32i     a11, a2, 0x130
601239ae  a802              l32i.n   a10, a2, 0
601239b0  a2612b            s32i     a10, a1, 0xac
601239b3  b19116            l32r     a11, . 0x600e93f8   ; [600e93f8]=0x600b5360 'output_spdif'
601239b6  81ea15            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
601239b9  e00800            callx8   a8
601239bc  16ba2f            beqz     a10, 0x60123cbb
601239bf  a2212b            l32i     a10, a1, 0xac
601239c2  b18e16            l32r     a11, . 0x600e93fc   ; [600e93fc]=0x600b5370 'output_arc'
601239c5  0cac              movi.n   a12, 0xa
601239c7  81e615            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
601239ca  e00800            callx8   a8
601239cd  0c0c              movi.n   a12, 0
601239cf  92212e            l32i     a9, a1, 0xb8
601239d2  16ba3f            beqz     a10, 0x60123dd1
601239d5  d22241            l32i     a13, a2, 0x104
601239d8  90dd82            mull     a13, a13, a9
601239db  0c08              movi.n   a8, 0
601239dd  7bbd              addi.n   a11, a13, 7
601239df  d0bdb3            movgez   a11, a13, a13
601239e2  b0b321            srai     a11, a11, 3
601239e5  b08872            salt     a8, a8, a11
601239e8  0bfb              addi.n   a15, a11, -1
601239ea  80bf93            movnez   a11, a15, a8
601239ed  16eb3b            beqz     a11, 0x60123daf
601239f0  0c09              movi.n   a9, 0
601239f2  768006            loop     a0, 0x601239fc
601239f5  1b99              addi.n   a9, a9, 1
601239f7  b0b121            srai     a11, a11, 1
601239fa  9cfb              beqz.n   a11, 0x60123a1d
601239fc  86fcff            j        0x601239f2
601239ff  a12516            l32r     a10, . 0x600e9294   ; [600e9294]=0x60280cf0
60123a02  b2a003            movi     a11, 3
60123a05  c16e16            l32r     a12, . 0x600e93c0   ; [600e93c0]=0x600067d0 'out_event'
60123a08  f2223c            l32i     a15, a2, 0xf0
60123a0b  d2a7d7            movi     a13, 0x7d7
60123a0e  e17c16            l32r     a14, . 0x600e9400   ; [600e9400]=0x600b5498 '%8s: stop\n'
60123a11  f22f00            l32i     a15, a15, 0
60123a14  65dedf            call8    0x601037fc   ; <logmm_printl>
60123a17  c2a000            movi     a12, 0
60123a1a  869fff            j        0x6012389c
60123a1d  d2223e            l32i     a13, a2, 0xf8
60123a20  926242            s32i     a9, a2, 0x108
60123a23  0c0a              movi.n   a10, 0
60123a25  b0dd11            slli     a13, a13, 5
60123a28  7bbd              addi.n   a11, a13, 7
60123a2a  d0bdb3            movgez   a11, a13, a13
60123a2d  b0b321            srai     a11, a11, 3
60123a30  b0aa72            salt     a10, a10, a11
60123a33  0b8b              addi.n   a8, a11, -1
60123a35  a0b893            movnez   a11, a8, a10
60123a38  168b37            beqz     a11, 0x60123db4
60123a3b  0c09              movi.n   a9, 0
60123a3d  768006            loop     a0, 0x60123a47
60123a40  1b99              addi.n   a9, a9, 1
60123a42  b0b121            srai     a11, a11, 1
60123a45  8c1b              beqz.n   a11, 0x60123a4a
60123a47  86fcff            j        0x60123a3d
60123a4a  92623f            s32i     a9, a2, 0xfc
60123a4d  e22600            l32i     a14, a6, 0
60123a50  92612f            s32i     a9, a1, 0xbc
60123a53  376e43            bbci     a14, 3, 0x60123a9a
60123a56  a205a4            l8ui     a10, a5, 0xa4
60123a59  815f16            l32r     a8, . 0x600e93d8   ; [600e93d8]=0x600a40e0 'pcm'
60123a5c  f2250c            l32i     a15, a5, 0x30
60123a5f  916216            l32r     a9, . 0x600e93e8   ; [600e93e8]=0x600cc6c0 'es'
60123a62  f22f00            l32i     a15, a15, 0
60123a65  a09883            moveqz   a9, a8, a10
60123a68  9901              s32i.n   a9, a1, 0
60123a6a  a2212f            l32i     a10, a1, 0xbc
60123a6d  8881              l32i.n   a8, a1, 0x20
60123a6f  8911              s32i.n   a8, a1, 4
60123a71  e22510            l32i     a14, a5, 0x40
60123a74  e921              s32i.n   a14, a1, 8
60123a76  e16316            l32r     a14, . 0x600e9404   ; [600e9404]=0x600b537c '%8s: start %s, codec %d, %dch, %drate, %dbpp, stride_shift %d, output_stride_shift %d\n'
60123a79  d8a1              l32i.n   a13, a1, 0x28
60123a7b  d931              s32i.n   a13, a1, 0xc
60123a7d  c22511            l32i     a12, a5, 0x44
60123a80  d2a762            movi     a13, 0x762
60123a83  c941              s32i.n   a12, a1, 0x10
60123a85  c14e16            l32r     a12, . 0x600e93c0   ; [600e93c0]=0x600067d0 'out_event'
60123a88  b22512            l32i     a11, a5, 0x48
60123a8b  a961              s32i.n   a10, a1, 0x18
60123a8d  b951              s32i.n   a11, a1, 0x14
60123a8f  a10116            l32r     a10, . 0x600e9294   ; [600e9294]=0x60280cf0
60123a92  b2a003            movi     a11, 3
60123a95  65d6df            call8    0x601037fc   ; <logmm_printl>
60123a98  0c0c              movi.n   a12, 0
60123a9a  b8b1              l32i.n   a11, a1, 0x2c
60123a9c  d881              l32i.n   a13, a1, 0x20
60123a9e  d26513            s32i     a13, a5, 0x4c
60123aa1  b26515            s32i     a11, a5, 0x54
60123aa4  779d69            bne      a13, a7, 0x60123b11
60123aa7  f22128            l32i     a15, a1, 0xa0
60123aaa  e22514            l32i     a14, a5, 0x50
60123aad  f79e60            bne      a14, a15, 0x60123b11
60123ab0  922129            l32i     a9, a1, 0xa4
60123ab3  822510            l32i     a8, a5, 0x40
60123ab6  979857            bne      a8, a9, 0x60123b11
60123ab9  a2212a            l32i     a10, a1, 0xa8
60123abc  a71b02            beq      a11, a10, . +6
60123abf  16ed04            beqz     a13, 0x60123b11
60123ac2  0c07              movi.n   a7, 0
60123ac4  b8c5              l32i.n   a11, a5, 0x30
60123ac6  b22b11            l32i     a11, a11, 0x44
60123ac9  16eb28            beqz     a11, 0x60123d5b
60123acc  52a0c0            movi     a5, 0xc0
60123acf  ac27              beqz.n   a7, 0x60123af5
60123ad1  7802              l32i.n   a7, a2, 0
60123ad3  b14916            l32r     a11, . 0x600e93f8   ; [600e93f8]=0x600b5360 'output_spdif'
60123ad6  0ccc              movi.n   a12, 0xc
60123ad8  ad07              mov.n    a10, a7
60123ada  81a115            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123add  e00800            callx8   a8
60123ae0  16fa1b            beqz     a10, 0x60123ca3
60123ae3  ad07              mov.n    a10, a7
60123ae5  b14516            l32r     a11, . 0x600e93fc   ; [600e93fc]=0x600b5370 'output_arc'
60123ae8  0cac              movi.n   a12, 0xa
60123aea  819d15            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123aed  e00800            callx8   a8
60123af0  0c0c              movi.n   a12, 0
60123af2  16da1a            beqz     a10, 0x60123ca3
60123af5  b834              l32i.n   a11, a4, 0xc
60123af7  464fff            j        0x60123838
60123afa  a2212c            l32i     a10, a1, 0xb0
60123afd  b14216            l32r     a11, . 0x600e9408   ; [600e9408]=0x600b585c 'ac3'
60123b00  0c3c              movi.n   a12, 3
60123b02  819715            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123b05  e00800            callx8   a8
60123b08  dc6a              bnez.n   a10, 0x60123b22
60123b0a  0c1c              movi.n   a12, 1
60123b0c  c981              s32i.n   a12, a1, 0x20
60123b0e  86a1ff            j        0x60123998
60123b11  ad02              mov.n    a10, a2
60123b13  e5e2fe            call8    0x60122940   ; <out_stop>
60123b16  ad02              mov.n    a10, a2
60123b18  e5bdfe            call8    0x601226f8   ; <out_start>
60123b1b  0c0c              movi.n   a12, 0
60123b1d  0c17              movi.n   a7, 1
60123b1f  46e8ff            j        0x60123ac4
60123b22  a2212c            l32i     a10, a1, 0xb0
60123b25  b13916            l32r     a11, . 0x600e940c   ; [600e940c]=0x600b5864 'eac3'
60123b28  0c4c              movi.n   a12, 4
60123b2a  818d15            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123b2d  e00800            callx8   a8
60123b30  cc5a              bnez.n   a10, 0x60123b39
60123b32  0c2d              movi.n   a13, 2
60123b34  d981              s32i.n   a13, a1, 0x20
60123b36  8697ff            j        0x60123998
60123b39  a2212c            l32i     a10, a1, 0xb0
60123b3c  b13516            l32r     a11, . 0x600e9410   ; [600e9410]=0x600b5854 'aac'
60123b3f  c2a003            movi     a12, 3
60123b42  818715            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123b45  e00800            callx8   a8
60123b48  565a13            bnez     a10, 0x60123c81
60123b4b  0c3e              movi.n   a14, 3
60123b4d  e981              s32i.n   a14, a1, 0x20
60123b4f  4691ff            j        0x60123998
60123b52  b12916            l32r     a11, . 0x600e93f8   ; [600e93f8]=0x600b5360 'output_spdif'
60123b55  0ccc              movi.n   a12, 0xc
60123b57  a802              l32i.n   a10, a2, 0
60123b59  a2612b            s32i     a10, a1, 0xac
60123b5c  818115            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123b5f  e00800            callx8   a8
60123b62  562a04            bnez     a10, 0x60123ba8
60123b65  c02000            memw     
60123b68  91b915            l32r     a9, . 0x600e924c   ; [600e924c]=0xc0fffe00
60123b6b  d229c9            l32i     a13, a9, 0x324
60123b6e  7cee              movi.n   a14, -2
60123b70  e0dd10            and      a13, a13, a14
60123b73  c02000            memw     
60123b76  d269c9            s32i     a13, a9, 0x324
60123b79  c02000            memw     
60123b7c  b229cc            l32i     a11, a9, 0x330
60123b7f  7cdc              movi.n   a12, -3
60123b81  c0bb10            and      a11, a11, a12
60123b84  c02000            memw     
60123b87  b269cc            s32i     a11, a9, 0x330
60123b8a  c02000            memw     
60123b8d  8229cd            l32i     a8, a9, 0x334
60123b90  7c0a              movi.n   a10, -0x10
60123b92  a08810            and      a8, a8, a10
60123b95  0c2a              movi.n   a10, 2
60123b97  a08820            or       a8, a8, a10
60123b9a  c02000            memw     
60123b9d  8269cd            s32i     a8, a9, 0x334
60123ba0  f802              l32i.n   a15, a2, 0
60123ba2  f2612b            s32i     a15, a1, 0xac
60123ba5  060400            j        0x60123bb9
60123ba8  a2212b            l32i     a10, a1, 0xac
60123bab  b11416            l32r     a11, . 0x600e93fc   ; [600e93fc]=0x600b5370 'output_arc'
60123bae  0cac              movi.n   a12, 0xa
60123bb0  816c15            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123bb3  e00800            callx8   a8
60123bb6  168a2d            beqz     a10, 0x60123e92
60123bb9  d22241            l32i     a13, a2, 0x104
60123bbc  c22240            l32i     a12, a2, 0x100
60123bbf  d0cc82            mull     a12, a12, a13
60123bc2  0c08              movi.n   a8, 0
60123bc4  7bbc              addi.n   a11, a12, 7
60123bc6  c0bcb3            movgez   a11, a12, a12
60123bc9  b0b321            srai     a11, a11, 3
60123bcc  b08872            salt     a8, a8, a11
60123bcf  0bfb              addi.n   a15, a11, -1
60123bd1  80bf93            movnez   a11, a15, a8
60123bd4  16bb28            beqz     a11, 0x60123e63
60123bd7  0c09              movi.n   a9, 0
60123bd9  1b99              addi.n   a9, a9, 1
60123bdb  b0b121            srai     a11, a11, 1
60123bde  567bff            bnez     a11, . -5
60123be1  e2223e            l32i     a14, a2, 0xf8
60123be4  926242            s32i     a9, a2, 0x108
60123be7  0c0f              movi.n   a15, 0
60123be9  b0ee11            slli     a14, a14, 5
60123bec  7bde              addi.n   a13, a14, 7
60123bee  e0deb3            movgez   a13, a14, a14
60123bf1  d0d321            srai     a13, a13, 3
60123bf4  d0ff72            salt     a15, a15, a13
60123bf7  0bed              addi.n   a14, a13, -1
60123bf9  f0de93            movnez   a13, a14, a15
60123bfc  168d26            beqz     a13, 0x60123e68
60123bff  0c0b              movi.n   a11, 0
60123c01  1bbb              addi.n   a11, a11, 1
60123c03  d0d121            srai     a13, a13, 1
60123c06  567dff            bnez     a13, . -5
60123c09  a2212b            l32i     a10, a1, 0xac
60123c0c  b2623f            s32i     a11, a2, 0xfc
60123c0f  0cac              movi.n   a12, 0xa
60123c11  b1fa15            l32r     a11, . 0x600e93fc   ; [600e93fc]=0x600b5370 'output_arc'
60123c14  815315            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123c17  e00800            callx8   a8
60123c1a  0c0c              movi.n   a12, 0
60123c1c  91fe15            l32r     a9, . 0x600e9414   ; [600e9414]=0x60122f90 <cpy_00> '6A'
60123c1f  d22510            l32i     a13, a5, 0x40
60123c22  560a0d            bnez     a10, 0x60123cf6
60123c25  0e2db000411f400d  flix     opaque8 lo=00b02d0e hi=0d401f41
60123c2d  f20558            l8ui     a15, a5, 0x58
60123c30  160f27            beqz     a15, 0x60123ea4
60123c33  81f915            l32r     a8, . 0x600e9418   ; [600e9418]=0x60122af8 <cpy_8> '6A'
60123c36  82651c            s32i     a8, a5, 0x70
60123c39  922600            l32i     a9, a6, 0
60123c3c  3e09b0a041e53f0d  flix     opaque8 lo=a0b0093e hi=0d3fe541
60123c44  9205a4            l8ui     a9, a5, 0xa4
60123c47  81e415            l32r     a8, . 0x600e93d8   ; [600e93d8]=0x600a40e0 'pcm'
60123c4a  f2250c            l32i     a15, a5, 0x30
60123c4d  e1e615            l32r     a14, . 0x600e93e8   ; [600e93e8]=0x600cc6c0 'es'
60123c50  f22f00            l32i     a15, a15, 0
60123c53  90e883            moveqz   a14, a8, a9
60123c56  e901              s32i.n   a14, a1, 0
60123c58  e1f115            l32r     a14, . 0x600e941c   ; [600e941c]=0x600b53f8 '%8s: start %s, %dch, %dbpp, stride_shift %d, output_stride_shift %d\n'
60123c5b  d22510            l32i     a13, a5, 0x40
60123c5e  d911              s32i.n   a13, a1, 4
60123c60  c22511            l32i     a12, a5, 0x44
60123c63  d2a7ab            movi     a13, 0x7ab
60123c66  c921              s32i.n   a12, a1, 8
60123c68  c1d615            l32r     a12, . 0x600e93c0   ; [600e93c0]=0x600067d0 'out_event'
60123c6b  b22512            l32i     a11, a5, 0x48
60123c6e  b931              s32i.n   a11, a1, 0xc
60123c70  a8f5              l32i.n   a10, a5, 0x3c
60123c72  0c3b              movi.n   a11, 3
60123c74  a941              s32i.n   a10, a1, 0x10
60123c76  a18715            l32r     a10, . 0x600e9294   ; [600e9294]=0x60280cf0
60123c79  25b8df            call8    0x601037fc   ; <logmm_printl>
60123c7c  0c0c              movi.n   a12, 0
60123c7e  0686ff            j        0x60123a9a
60123c81  a2212c            l32i     a10, a1, 0xb0
60123c84  b1e715            l32r     a11, . 0x600e9420   ; [600e9420]=0x600b5870 'mat'
60123c87  c2a003            movi     a12, 3
60123c8a  813515            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123c8d  e00800            callx8   a8
60123c90  56aa04            bnez     a10, 0x60123cde
60123c93  0c4a              movi.n   a10, 4
60123c95  a981              s32i.n   a10, a1, 0x20
60123c97  463fff            j        0x60123998
60123c9a  b1e215            l32r     a11, . 0x600e9424   ; [600e9424]=0x601229ac <output_status> '6a'
60123c9d  92612e            s32i     a9, a1, 0xb8
60123ca0  4641ff            j        0x601239a9
60123ca3  a2221a            l32i     a10, a2, 0x68
60123ca6  b22201            l32i     a11, a2, 4
60123ca9  c1df15            l32r     a12, . 0x600e9428   ; [600e9428]=0x600b5488 'output_status'
60123cac  d2c120            addi     a13, a1, 0x20
60123caf  e2a010            movi     a14, 0x10
60123cb2  65fede            call8    0x60102c98   ; <aimc_raise_named_event>
60123cb5  c2a000            movi     a12, 0
60123cb8  468eff            j        0x60123af5
60123cbb  c02000            memw     
60123cbe  c16315            l32r     a12, . 0x600e924c   ; [600e924c]=0xc0fffe00
60123cc1  b22cc9            l32i     a11, a12, 0x324
60123cc4  d2a001            movi     a13, 1
60123cc7  d0bb20            or       a11, a11, a13
60123cca  c02000            memw     
60123ccd  b26cc9            s32i     a11, a12, 0x324
60123cd0  a2210a            l32i     a10, a1, 0x28
60123cd3  e570fd            call8    0x601213e0   ; <HAL_SetSpdifEsMode>
60123cd6  0c0c              movi.n   a12, 0
60123cd8  922240            l32i     a9, a2, 0x100
60123cdb  863dff            j        0x601239d5
60123cde  a2212c            l32i     a10, a1, 0xb0
60123ce1  b1d215            l32r     a11, . 0x600e942c   ; [600e942c]=0x600b5344 'dts_hd_ma'
60123ce4  0c9c              movi.n   a12, 9
60123ce6  811e15            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123ce9  e00800            callx8   a8
60123cec  0c79              movi.n   a9, 7
60123cee  561a05            bnez     a10, 0x60123d43
60123cf1  9981              s32i.n   a9, a1, 0x20
60123cf3  4628ff            j        0x60123998
60123cf6  a2c130            addi     a10, a1, 0x30
60123cf9  b1cd15            l32r     a11, . 0x600e9430   ; [600e9430]=0x600067f0
60123cfc  c2a06c            movi     a12, 0x6c
60123cff  d2612d            s32i     a13, a1, 0xb4
60123d02  814912            l32r     a8, . 0x600e8628   ; [600e8628]=0x60277ff4 <AuiLogger_AppendData> '6!'
60123d05  e00800            callx8   a8
60123d08  d2212d            l32i     a13, a1, 0xb4
60123d0b  0c0e              movi.n   a14, 0
60123d0d  b2c130            addi     a11, a1, 0x30
60123d10  0c9f              movi.n   a15, 9
60123d12  0c0c              movi.n   a12, 0
60123d14  c2651c            s32i     a12, a5, 0x70
60123d17  76af12            loopgtz  a15, 0x60123d2d
60123d1a  881b              l32i.n   a8, a11, 4
60123d1c  d7980b            bne      a8, a13, 0x60123d2b
60123d1f  a8e5              l32i.n   a10, a5, 0x38
60123d21  982b              l32i.n   a9, a11, 8
60123d23  ae89b0b0410b000d  flix     opaque8 lo=b0b089ae hi=0d000b41
60123d2b  cbbb              addi.n   a11, a11, 0xc
60123d2d  568ef0            bnez     a14, 0x60123c39
60123d30  b806              l32i.n   a11, a6, 0
60123d32  0e2bb0704118000d  flix     opaque8 lo=70b02b0e hi=0d001841
60123d3a  d1b615            l32r     a13, . 0x600e9414   ; [600e9414]=0x60122f90 <cpy_00> '6A'
60123d3d  d2651c            s32i     a13, a5, 0x70
60123d40  46bdff            j        0x60123c39
60123d43  a2212c            l32i     a10, a1, 0xb0
60123d46  b1bb15            l32r     a11, . 0x600e9434   ; [600e9434]=0x600b5878 'dtsx_p1'
60123d49  0c7c              movi.n   a12, 7
60123d4b  810515            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123d4e  e00800            callx8   a8
60123d51  564a06            bnez     a10, 0x60123db9
60123d54  0c8e              movi.n   a14, 8
60123d56  e981              s32i.n   a14, a1, 0x20
60123d58  060fff            j        0x60123998
60123d5b  6554fd            call8    0x601212a0   ; <irq_save>
60123d5e  82224f            l32i     a8, a2, 0x13c
60123d61  522253            l32i     a5, a2, 0x14c
60123d64  3df0              .byte    
60123d66  8055c0            sub      a5, a5, a8
60123d69  6555fd            call8    0x601212c0   ; <irq_restore>
60123d6c  9806              l32i.n   a9, a6, 0
60123d6e  0c0c              movi.n   a12, 0
60123d70  3e29b050410c000d  flix     opaque8 lo=50b0293e hi=0d000c41
60123d78  6e15b00041d57f0d  flix     opaque8 lo=00b0156e hi=0d7fd541
60123d80  a22242            l32i     a10, a2, 0x108
60123d83  f02000            nop      
60123d86  001a40            ssl      a10
60123d89  00a5a1            sll      a10, a5
60123d8c  25f8cf            call8    0x600f3d10   ; <aud_data_new_zero>
60123d8f  b22600            l32i     a11, a6, 0
60123d92  a05a20            or       a5, a10, a10
60123d95  c2a000            movi     a12, 0
60123d98  3e2bb010410d000d  flix     opaque8 lo=10b02b3e hi=0d000d41
60123da0  1685d2            beqz     a5, 0x60123acc
60123da3  bd05              mov.n    a11, a5
60123da5  ad03              mov.n    a10, a3
60123da7  e588ff            call8    0x60123634   ; <out_chai>
60123daa  0c0c              movi.n   a12, 0
60123dac  0647ff            j        0x60123acc
60123daf  0c09              movi.n   a9, 0
60123db1  061aff            j        0x60123a1d
60123db4  0c09              movi.n   a9, 0
60123db6  0624ff            j        0x60123a4a
60123db9  a2212c            l32i     a10, a1, 0xb0
60123dbc  b19f15            l32r     a11, . 0x600e9438   ; [600e9438]=0x600b5350 'dtsx_p2'
60123dbf  c2a007            movi     a12, 7
60123dc2  81e714            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123dc5  e00800            callx8   a8
60123dc8  dcea              bnez.n   a10, 0x60123dea
60123dca  0c9c              movi.n   a12, 9
60123dcc  c981              s32i.n   a12, a1, 0x20
60123dce  86f1fe            j        0x60123998
60123dd1  0c1a              movi.n   a10, 1
60123dd3  0c5b              movi.n   a11, 5
60123dd5  2552fd            call8    0x601212f8   ; <HAL_SetIEC1958ValidBit>
60123dd8  a8a1              l32i.n   a10, a1, 0x28
60123dda  e57afd            call8    0x60121588   ; <HAL_SetArcEsMode>
60123ddd  0c0c              movi.n   a12, 0
60123ddf  46bdff            j        0x60123cd8
60123de2  e80b              l32i.n   a14, a11, 0
60123de4  e2651c            s32i     a14, a5, 0x70
60123de7  86d0ff            j        0x60123d2d
60123dea  a2212c            l32i     a10, a1, 0xb0
60123ded  b19315            l32r     a11, . 0x600e943c   ; [600e943c]=0x600b5884 'dts_ma'
60123df0  0c6c              movi.n   a12, 6
60123df2  81db14            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123df5  e00800            callx8   a8
60123df8  cc5a              bnez.n   a10, 0x60123e01
60123dfa  0c7d              movi.n   a13, 7
60123dfc  d981              s32i.n   a13, a1, 0x20
60123dfe  86e5fe            j        0x60123998
60123e01  a2212c            l32i     a10, a1, 0xb0
60123e04  b18f15            l32r     a11, . 0x600e9440   ; [600e9440]=0x600b5358 'dts_hd'
60123e07  c2a006            movi     a12, 6
60123e0a  81d514            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123e0d  e00800            callx8   a8
60123e10  ccba              bnez.n   a10, 0x60123e1f
60123e12  0c6e              movi.n   a14, 6
60123e14  e981              s32i.n   a14, a1, 0x20
60123e16  86dffe            j        0x60123998
60123e19  92651c            s32i     a9, a5, 0x70
60123e1c  4686ff            j        0x60123c39
60123e1f  a2212c            l32i     a10, a1, 0xb0
60123e22  b18815            l32r     a11, . 0x600e9444   ; [600e9444]=0x600b5898 'dts'
60123e25  0c3c              movi.n   a12, 3
60123e27  81ce14            l32r     a8, . 0x600e9160   ; [600e9160]=0x60278280 <Remove> '6A'
60123e2a  e00800            callx8   a8
60123e2d  f881              l32i.n   a15, a1, 0x20
60123e2f  0c58              movi.n   a8, 5
60123e31  a0f883            moveqz   a15, a8, a10
60123e34  f981              s32i.n   a15, a1, 0x20
60123e36  86d7fe            j        0x60123998
60123e39  a11615            l32r     a10, . 0x600e9294   ; [600e9294]=0x60280cf0
60123e3c  b2a003            movi     a11, 3
60123e3f  c16015            l32r     a12, . 0x600e93c0   ; [600e93c0]=0x600067d0 'out_event'
60123e42  d2a7c1            movi     a13, 0x7c1
60123e45  f2223c            l32i     a15, a2, 0xf0
60123e48  e18015            l32r     a14, . 0x600e9448   ; [600e9448]=0x600b5440 '%8s: max %d, queued %d, zero %d\n'
60123e4b  822253            l32i     a8, a2, 0x14c
60123e4e  f22f00            l32i     a15, a15, 0
60123e51  826100            s32i     a8, a1, 0
60123e54  92224f            l32i     a9, a2, 0x13c
60123e57  9911              s32i.n   a9, a1, 4
60123e59  5921              s32i.n   a5, a1, 8
60123e5b  259adf            call8    0x601037fc   ; <logmm_printl>
60123e5e  0c0c              movi.n   a12, 0
60123e60  06c5ff            j        0x60123d78
60123e63  0c09              movi.n   a9, 0
60123e65  065eff            j        0x60123be1
60123e68  0c0b              movi.n   a11, 0
60123e6a  c666ff            j        0x60123c09
60123e6d  a10915            l32r     a10, . 0x600e9294   ; [600e9294]=0x60280cf0
60123e70  b2a003            movi     a11, 3
60123e73  c15315            l32r     a12, . 0x600e93c0   ; [600e93c0]=0x600067d0 'out_event'
60123e76  f2223c            l32i     a15, a2, 0xf0
60123e79  d2a7c6            movi     a13, 0x7c6
60123e7c  e17415            l32r     a14, . 0x600e944c   ; [600e944c]=0x600b5464 '%8s: zero_data %p, stride_shift %d\n'
60123e7f  f22f00            l32i     a15, a15, 0
60123e82  526100            s32i     a5, a1, 0
60123e85  822242            l32i     a8, a2, 0x108
60123e88  8911              s32i.n   a8, a1, 4
60123e8a  2597df            call8    0x601037fc   ; <logmm_printl>
60123e8d  0c0c              movi.n   a12, 0
60123e8f  46c3ff            j        0x60123da0
60123e92  0c0a              movi.n   a10, 0
60123e94  0c5b              movi.n   a11, 5
60123e96  2546fd            call8    0x601212f8   ; <HAL_SetIEC1958ValidBit>
60123e99  254bfd            call8    0x6012134c   ; <HAL_SetArcPcmMode>
60123e9c  8802              l32i.n   a8, a2, 0
60123e9e  82612b            s32i     a8, a1, 0xac
60123ea1  0645ff            j        0x60123bb9
60123ea4  ad04              mov.n    a10, a4
60123ea6  b16b15            l32r     a11, . 0x600e9454   ; [600e9454]=0x600ae4ec 'cap-ch-remap'
60123ea9  c2a001            movi     a12, 1
60123eac  916915            l32r     a9, . 0x600e9450   ; [600e9450]=0x60122bf8 <cpy_8_swap_c_lfe> '6A'
60123eaf  92651c            s32i     a9, a5, 0x70
60123eb2  a5e2ce            call8    0x600f2cdc   ; <aud_event_value_set_int>
60123eb5  0c0c              movi.n   a12, 0
60123eb7  865fff            j        0x60123c39
60123eba  000000            ill      
60123ebd  a1f514            l32r     a10, . 0x600e9294   ; [600e9294]=0x60280cf0
60123ec0  b2a000            movi     a11, 0
60123ec3  c13f15            l32r     a12, . 0x600e93c0   ; [600e93c0]=0x600067d0 'out_event'
60123ec6  d2a7a4            movi     a13, 0x7a4
60123ec9  f2250c            l32i     a15, a5, 0x30
60123ecc  e16315            l32r     a14, . 0x600e9458   ; [600e9458]=0x600b53d4 '%8s: unknown in/out channel. %d/%d\n'
60123ecf  92212d            l32i     a9, a1, 0xb4
60123ed2  f22f00            l32i     a15, a15, 0
60123ed5  9901              s32i.n   a9, a1, 0
60123ed7  88e5              l32i.n   a8, a5, 0x38
60123ed9  8911              s32i.n   a8, a1, 4
60123edb  2592df            call8    0x601037fc   ; <logmm_printl>
60123ede  0c0c              movi.n   a12, 0
60123ee0  8695ff            j        0x60123d3a
60123ee3  003601            slli     a3, a6, 
