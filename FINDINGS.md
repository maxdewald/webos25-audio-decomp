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
lgaudiosink                renderer.c              opens /dev/lg/aren, creates the bypass head
kernel alsa_sndout.c       (lg-opensource/kdriver) _get_output_format / _set_output
dsp2                       renderer_path.c         renderer bypass head: first-event replay, lipsync clock
dsp1                       output_arc.c            output_arc: ES/HBR lane writer, eARC rate
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
3. **dsp1:**
   - `es=true` with more than 2 channels selects `cpy_es8`, a 4-lane writer that
     byte-swaps each 16-bit word. Input must therefore be complete big-endian
     IEC 61937; dsp1 adds no preamble.
   - On `output_arc` it clears the IEC 60958 valid bit and calls
     `HAL_SetArcEsMode(data-rate)`; 768000 maps to lane code 9.
   - dsp1 restarts and raises `output_status` only when codec, rate, channels or acmod
     changed.
4. **dsp0:** no DTS decoder exists in the image or at runtime. Only HDMI DTS parsing
   remains, so restoring native DTS decoding is not possible.

## Kernel/userspace mismatch found

The sink sends ioctl `0x4008522b` (`SET_MSGPID_TO_SIGNAL`), and the shipped kernel
rejects it with `-ENOTTY`. The sink ignores the result. This affects only signal
registration, not pipeline creation. The log `bypass is not yet created` is printed
just before the ES head is built; it is not an error.

## Still unknown

- Whether DTS caps ever reach libumi as codec `dts_hd_ma`: a `find("dts")` in
  `generateAudioDecodeInfo` has an unrecovered use.
- The SoC name the C2 reports, so whether the PCMMC conversion applies.
- Conditions inside dsp1/dsp2 FLIX bundles, including the exact `cpy_es` vs
  `cpy_es8` channel test.
- LS2 permissions for calling `mixDigitalSoundOutput` and `setQualityAttribute` from
  a homebrew service.
- The running kernel's maximum accepted `ct`.
- Proposed live experiments (none run yet) are at the end of `findings.txt` in
  `evidence/audiod-mixer`, `evidence/luna-contract` and `evidence/renderer-abi`.
