# Reconstructed LG webOS 25 audio output path (C2, OLED42C27LA)

Firmware: Rockhopper 10.3.1-3006, kernel 5.4.268-329.ptl4tv.5. Hashes are in
[orig/SHA256SUMS](orig/SHA256SUMS) and at the top of each file.

This is **reconstructed C, not LG source**. It holds only the logic we recovered and
checked against the binaries. Every function names the address it came from. Code
that is still opaque (Xtensa FLIX bundles, decompiler gaps) is marked as such and not
filled in by guessing.

## Path, top to bottom

```
app / media pipeline
  starfish-media-pipeline  resource_booking.c      books ADEC<n>; that n becomes kernel main_audio
                           (generateAudioDecodeInfo) reports codec/channel to audiooutputd
  media-resource-calc      calc_adec.c             ADEC cost per stream
audiod                     mixer.c                 mixDigitalSoundOutput -> setMixer{mix}
audiooutputd + libumi      output_policy.c         picks ct/cn/mo, writes "Sndout EARC Outputtype"
decproxy                   src/libgstdecproxy      decoder host bin; buffers pass through untouched
lgaudiodec                 src/libgstlgaudiodec    DSP decoder control; es-bypass = drop; no DTS
lgaudiosink                lgao.c, render_*.c      opens /dev/lg/aren, appsrc/bypass_appsrc, lgao_feed
libaabase/libaudcommon     appsrc.c, data.c        appsrc push, aud_data/aud_event (first-event wire format)
libaa                      src/libaa               module core, aimc/IMC, apipeline grammar, shm glue
kernel aud                 src/kernel/aud          renderer/mixer/output chains, _get_output_format (DTS compiled out)
dsp2                       src/dsp2                fader(queue), lipsync, delay, gain, bypass, mixers, glue
dsp1                       src/dsp1                outctrlbin, output_arc: ES/HBR lane writer, eARC rate
dsp0                       decoders.c              decoder inventory (no DTS decoder)
```

## Answers to the four questions

1. **What makes libumi choose `mo=0`?** Only `setOutputTypeForPassThrough`, and only
   when all of these hold:
   - digital mode is `passThrough`;
   - the output is not Harmony and input downmix is off;
   - the input codec string has a direct row in the pass-through table;
   - the eARC sink advertises the matching capability token;
   - `mix` is false.
   Every fallback path writes `mo=1`.
   - `mix` comes from `setMixer`, which audiod forwards from
     `tv/mixDigitalSoundOutput` and suppresses `false` while any other caller ID still
     holds mixing.
   - Multichannel PCM keeps `mo=0` only when input channels ≤ device channels.
     Omitting `advancedEarcCapability` sets the device limit to 0.
2. **Resource booking:**
   - The calculator charges one ADEC per stream; DTS/PCM/MAT/IAMF have no rows.
   - The config allows 2 ADEC slots and 5 PCMMC slots.
   - ADEC is converted to PCMMC only on SoCs in the software-decoder list.
   - The pipeline reports the lowest returned `ADEC` index as its `resource`. That
     index is the kernel's `main_audio`, which is why the renderer index can differ.
3. **dsp1** (`out_event` @0x60123814):
   - **Writer selection:** `es=true` with exactly 2 channels selects `cpy_es`
     (1 lane). Any other channel count selects `cpy_es8` (4 lanes × 2 slots).
     - Both writers byte-swap each 16-bit word into bits 8–23. Input must therefore be
       complete big-endian IEC 61937; dsp1 adds no preamble.
     - `fill_bc` advances all 4 lanes only when channels == 8 [FLIX @0x60121efd].
       4- or 6-channel ES would therefore be corrupted (inferred).
   - **eARC programming:** on `output_arc` it calls
     `HAL_SetIEC1958ValidBit(1, 5)` (non-audio) and `HAL_SetArcEsMode(data-rate)`
     @0x60121588. 768000 maps to lane code 9.
   - **Codec reporting:** dsp1 numbers the codecs pcm 0, ac3 1, eac3 2, aac 3, mat 4,
     dts 5, dts_hd 6, dts_hd_ma/dts_ma 7, dtsx_p1 8, dtsx_p2 9.
     - It raises `output_status{codec, channels, data_rate, acmod}` only when one of
       those changed.
     - For DTS it is a label only; the data is written like any other ES.
   - **Muting:** gain inside `outctrlbin` starts with `mute-output=1`, so ES is zeroed
     (at whole-packet boundaries) until the kernel calls `output_set_mute(false)`.
4. **dsp0:** no DTS decoder exists in the image or at runtime. Only HDMI DTS parsing
   remains, so restoring native DTS decoding is not possible.

## ES push path, sink → eARC

Reconstructed end to end: [src/libgstlgaudiosink](src/libgstlgaudiosink),
[src/libaabase](src/libaabase), [src/libaudcommon](src/libaudcommon),
[src/libaa](src/libaa), [src/kernel/aud](src/kernel/aud), [src/dsp2](src/dsp2),
[src/dsp1](src/dsp1). Plus [src/libgstdecproxy](src/libgstdecproxy) and
[src/libgstlgaudiodec](src/libgstlgaudiodec), which sit upstream of the sink.

```
decproxy ── lgadec ── lgaudiosink ─ lgao_feed ── bypass_appsrc (ARM, libaabase)
                                                     │  aimc_module_connect → kernel GET_BYPASS_UID head
kernel renderer_bypass (dsp2):   queue(fader) → lipsync
kernel mixer_byin/bypass (dsp2): delay → gain(0x800000) → bypass ── gluepw ═ shm ═ gluepr
kernel output_ARC (dsp1):        queue → outctrlbin → output_arc (cpy_es / cpy_es8 → I2S lanes)
```

### Upstream: decproxy and lgadec

- **decproxy** passes buffers through unchanged. It never copies them or clears
  flags (`gst_dec_bridge_chain` @0x13614, `_internal_chain` @0x15bb8).
  - The only drop is the first DECODE_ONLY|DROPPABLE buffer
    (`droppable_buffer_drop_probe` @0x178c8).
  - It has no `lipsync` property, event or child proxy. The sink sits outside its bin,
    so decproxy cannot block a `lipsync` setting.
  - The shipped binary is LG's published gst-plugins-cool 1.24 source, with no
    behavioural changes (93% instruction match).
- **lgadec has no DTS at all.**
  - There is no DTS MIME entry, sink template, codec id or feeder.
  - Its kernel ABI renumbers the decoder codecs (OPUS=17, AAC_BYPASS=18, …), where
    the published header has DTS=17 and DTS_HD=18.
  - The DSP pass-through output (`LGADEC_DATA_ES`, buffer flag 0x200000) exists only
    for AC3, EAC3, AAC, MAT and EAC3_ATMOS (`lgadec_select_codec` @0x2d444).
- **`es-bypass` means "drop".** The property (`lgadec_set_es_bypass` @0x30848, logs
  "ES BYPASS & DROP") closes the DSP decoder.
  - Input bytes are forwarded flagged 0x1000000, behind dummy caps `BYPASS=1`
    (`_gst_lg_adec_set_caps_srcpad_dummy` @0x16a4c).
  - The sink reacts to either signal with `lgao_set_drop_renderer(1)`, which tears the
    renderer down and discards every buffer ("DROP!!!!!").
  - In the shipped decoder, the push-result test in
    `_gst_lg_adec_set_caps_srcpad_dummy` is inverted (raw 0x6ae4). After a DSP reset,
    a *successful* dummy-caps push is reported as a failure.

### Sink: lgaudiosink

- The lgao layer is in [lgao.c](src/libgstlgaudiosink/lgao.c).
- **Module creation:** `_lgao_connect_renderer` @0x1441c creates `appsrc`,
  `bypass_appsrc` and `appsink` through aimc.
  - It connects `appsrc` to the GET_UID head and `bypass_appsrc` to the
    GET_BYPASS_UID head.
  - No `apipeline` or `aimc_set_property` call exists in the sink; the kernel builds
    all DSP chains.
- **Data routing (`lgao_feed` @0x18828):**
  - ES buffers (`aud_data.info.type` = 1) go **only** to `bypass_appsrc`.
  - PCM goes to `appsrc`, and also to `bypass_appsrc` when `pcm_bypass` or
    `pcm_multi_ch` is set.
- **While `drop_renderer` is set** (`lgao_set_drop_renderer` @0x18e50):
  - `lgao_feed` drops every buffer, logging "drop...".
  - `lgao_push_first_event` skips sending the first event.
  - EOS completes immediately.
- **The ES first event** on `bypass_appsrc` carries `es=true`, `codec`, `atmos`,
  `clock-x4`, `channels`, `bits-per-sample` and `data-rate`. Defaults are 2 ch,
  16 bit, 48000.

### ARM transport: libaabase, libaudcommon and libaa

- **`module_appsrc_push_data`** (@0x10c0c) and **`push_event`** always return 0. The
  sink never sees a drop.
- **Silent drop points in `aud_module_push_data`** (libaa @0x2d454), each logged only
  at debug level:
  - the `drop-output-data` flag is set;
  - `drop-output-meta` is set and the data is meta;
  - output port 0 does not exist.
- **`aud_port_sync_locked` @0x2b22c** carries data on to the next stage:
  - It releases the data silently when no downstream peer exists.
  - Toward the DSP it writes into the shared ring (gluepw). It blocks while the ring
    is full and ignores `insert_au` errors.
- **`aud_data_adapter_get_data`** (libaudcommon @0x13efc) returns a reassembled
  multi-buffer region with `info.type` reset to PCM. This path is not used by the
  appsrc push.

### Kernel (shipped Image vs LG's GPL kdriver)

- **DTS is compiled out of the shipped Image:**
  - `aud_hal_dts_supported` returns 0.
  - The output codec list stops at MAT.
  - `_cb_output_status` treats dsp1 codec codes 5–9 (DTS) as PCM, so eARC is programmed
    PCM.
  - Every DTS case in `_get_output_format` / `_set_output` is gone.
  - The eARC output-type setter rejects DTSHD_MA (10) and DTSX_P1 (11).
- **Bypass head:** `aren_get_uid` gives the ES head (`queue → lipsync`) when
  `pcm_bypass` and `pcm_multi_ch` are both 0. The ES queue's uid is never registered,
  so `block-queue` and `queue-max-length` never reach it.
- **The renderer→mixer→output bypass links (port 0x100) exist only when `_set_output`
  decides `bypass`:**
  - With no decoder block at main_audio (sink-only playback), that needs
    `renderer_get_pcm_bypass`, which is false for the ES head.
  - With a decoder block, the codec must be AC3/EAC3/MAT/TrueHD/AAC.
  - **ES from the sink therefore never gets the bypass connection.** It reaches the
    bypass lipsync on dsp2 and stops there.
- **The kernel's drop flag has no effect:** `renderer_preconnect` and `postconnect`
  set `drop_output_data=1/0` with underscores. dsp2 matches property names with
  `strcmp` and only knows `drop-output-data`, so both are silently ignored.

### DSP stages

- **dsp2 fader ("queue"):**
  - It blocks upstream while full.
  - It drops data on a disabled port or after an async FLUSH.
  - With `es`, fades become plain copies.
- **dsp2 lipsync:**
  - ES mode applies when the first event's `codec` ≠ "pcm"; a missing `codec` key
    defaults to "pcm".
  - It drops input with no connected output port, in its SKIP/IDLE actions, and while
    `over_protection` or a pending discard is set.
  - It holds FIRST back until the first real output.
- **dsp2 gain:** the `es_mode` flag is taken only from the FIRST_AU `es` key.
  - ES-typed data that arrives without the FIRST_AU `es` flag is dropped while muted
    ("ES drop?"); metadata is dropped while muted ("META drop?").
  - PCM is replaced by zeros while muted.
  - ES data with `es_mode` is replaced by zero packets while muted, at packet
    boundaries.
  - The constructor starts muted; the kernel unmutes in `renderer_set_default`.
- **dsp2 delay / bypass / mixer:**
  - They pass ES through unchanged. Nothing in mixerbin, msmixer, pcmren, mux or the
    connectors reads `es`.
  - dsp2 → dsp1 goes through gluepw → shm → gluepr.
- **dsp1 output_arc:** see section 3 above, and [src/dsp1](src/dsp1).
  - `out_chain` always drops metadata.
  - `out_chain_bc` has no format check. It stops writing on a disabled port, on
    underflow, or when the writer returns 0.
  - DTS-HD MA would be labelled codec 7 and written as HBR with code 9.
  - The kernel then programs eARC as PCM, because `_cb_output_status` maps codecs
    above 4 to PCM.

## Kernel/userspace mismatch found

The sink sends ioctl `0x4008522b` (`SET_MSGPID_TO_SIGNAL`), and the shipped kernel
rejects it with `-ENOTTY`. The sink ignores the result. This affects only signal
registration, not pipeline creation. The log `bypass is not yet created` is printed
just before the ES head is built; it is not an error.

## Still unknown

- Whether DTS caps ever reach libumi as codec `dts_hd_ma`: a `find("dts")` in
  `generateAudioDecodeInfo` has an unrecovered use.
- The SoC name the C2 reports, so whether the PCMMC conversion applies.
- Conditions inside the remaining dsp1/dsp2 FLIX bundles: three in dsp1 `out_cmd`, and
  the PCM branches of dsp2 gain and `_lipsync_state`.
- LS2 permissions for calling `mixDigitalSoundOutput` and `setQualityAttribute` from
  a homebrew service.
- The running kernel's maximum accepted `ct`.
- Proposed live experiments (none run yet) are at the end of `findings.txt` in
  `evidence/audiod-mixer`, `evidence/luna-contract` and `evidence/renderer-abi`.
