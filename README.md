# webos25-audio-decomp

Decompilation of the LG webOS 25 audio output path: how an LG TV decides what goes out
over eARC, and whether native DTS-HD MA / DTS:X pass-through can be enabled. It supports
[dts_restore_plus](../dts_restore_plus).

Source device: LG OLED42C27LA (C2), Rockhopper 10.3.1-3006, kernel 5.4.268-329.ptl4tv.5.

> The original binaries are LG proprietary. They are **not** in this repository and must
> never be committed or redistributed. Only their SHA-256 hashes are tracked
> ([orig/SHA256SUMS](orig/SHA256SUMS)). `src/` is clean-room reconstructed C, not LG source.

## Layout

| Path | Contents | In git |
|---|---|---|
| `orig/` | Original files pulled from the TV, mirroring `/` (`usr/lib`, `usr/sbin`, `lib/firmware`, `boot`, `etc`) | hashes only |
| `lg-opensource/` | LG's published source for the same release, for reference: `kdriver/` (audio kernel driver), `gstreamer/` (git repo incl. `gst-plugins-cool`/decproxy), `pulseaudio/`, `kernel-k25lp-5.4.268-292/` (build tree with `.config`, `System.map`, `vmlinux`). Older than the shipped build | no |
| `src/<target>/` | Reconstructed C, one directory per original binary. `src/include/` holds shared types; `src/include/gst-gen/` an ILP32 `glibconfig.h` for the GStreamer targets. `src/libgstdecproxy/` (LGPL-2.0) and `src/kernel/aud/` (GPL-2.0) start from LG's published source, adapted to the shipped binaries | yes |
| `symbols/` | Function tables per target (`addr<TAB>size<TAB>name`, Ghidra addresses) and recovered DSP symbol tables | yes |
| `evidence/<topic>/` | Per-topic reports (`findings.json`/`.txt`) with address anchors, plus the scripts and validators that produced them | yes |
| `FINDINGS.md` | What the decompilation has answered so far, and what is still unknown | yes |
| `decomp.toml` | Target list: original binary, symbol table, `src/` directory, compiler flags | yes |
| `tools/` | Ghidra headless scripts, DSP firmware tools, the progress CLI, the toolchain image | yes |
| `decomp/` | Full Ghidra pseudocode per binary (reference only) | no |
| `build/` | Ghidra databases, split DSP images, compiled objects | no |
| `tmp/` | Scratch: Ghidra logs, old handover notes | no |

## Setup

1. Put the original files in `orig/` and check them: `make verify`. Pull them from a
   rooted TV with the same firmware (paths are in `orig/SHA256SUMS`).
2. Build the compiler image: `make toolchain`. It is the exact compiler LG used
   (GCC 11.4.0, ARMv7-A Thumb-2, soft-float EABI5).
3. Optional, LG's source for reference: from opensource.lge.com, model OLED42C27LA,
   "webOS25 WG ReNew" (osSeq 512189). Tarball 3 gives `kdriver/`. Tarball 2 holds
   `gstreamer_open_source.zip`, `openPulseAudio.tar.gz` and the k25lp kernel `.tgz`.
   Unpack them into `lg-opensource/` under the names above.
4. Optional, for the Xtensa evidence scripts: `uv sync`.
5. Optional, to regenerate `decomp/` and `build/`: Ghidra 12.2 and
   `JAVA_HOME=/opt/homebrew/opt/openjdk@21`, plus the Xtensa language in
   `tools/ghidra-ext/XtensaHiFi` copied into `~/Library/ghidra/ghidra_12.2_PUBLIC/Extensions/`.
   Then `make decomp-arm` and `make decomp-dsp`.

## Progress

```sh
make status                          # per target: coverage of src/ and instruction match
make todo T=libumi                   # largest functions not yet reconstructed
make diff T=libumi F=setOutputType   # instruction diff, original vs compiled src/
make validate                        # re-check every evidence report against orig/
```

- **Coverage** is the share of a target's function bytes that has a reconstruction in `src/`.
- **Match** compiles `src/` with the original compiler and compares each function's
  instructions with the original (addresses and literal-pool offsets are ignored).
  100% means the function is byte-for-byte equivalent.
- objdiff is not used: its ARM backend stops at ARMv6K, and these targets are ARMv7
  Thumb-2 and Xtensa.
- DSP targets (Xtensa) and `kernel` (aarch64, LG's `aud` driver functions only) report
  coverage only, as no matching compiler is set up for them.
- Matching compares by address range, so GCC clones (`foo.constprop.0`) and functions
  with inline jump tables are scored correctly. All ARM targets are built without stack
  protector or `_FORTIFY_SOURCE`, like LG's build.
- CI (`.github/workflows/progress.yml`) compiles `src/` and reports coverage on every push.

## Adding a function

1. Find it: `make todo T=<target>`, then read `decomp/<binary>.c` at that address.
2. Write it in `src/<target>/`, tagged with the Ghidra address on the line before it:

   ```c
   /* @0x4fc24: what it does, if not obvious */
   static void setOutputType(struct umi_tv_audio *a, int ct, int mo)
   ```

3. Iterate with `make diff` until the match is as high as it can go.
4. Record anything it answers in `FINDINGS.md`, with the address as evidence.

## Addresses

- ARM binaries: Ghidra address = raw ELF VA + 0x10000. Code is Thumb. A target that
  tags raw VAs sets `src_addr_bias` in `decomp.toml`.
- DSP images: linked at 0x60000000. Split them with `tools/split_fw.py`.

## Pitfalls

- DSP symbol tables are `(func_ptr, name_ptr)` pairs. Literal pools hold look-alike
  pairs, so `tools/fwsyms.py` prefers entries inside a run. Confirm a name with
  `tools/symtab_probe.py` and the function's own logged `__func__` string.
- Disassemble the ARM libraries with `--triple=armv7 --mattr=+thumb-mode`. Plain
  `thumbv7` crashes on barrier instructions, and ARM mode gives garbage.
- Xtensa FLIX bundles (8 bytes, op0 0xE/0xF) are opaque to Ghidra (~125k truncations).
  Conditions inside them are marked `[FLIX]` in `src/` and are not guessed.
- Ghidra drops the string arguments of `std::string::compare`. Recover them with
  `tools/funcstrings.py orig/<bin> symbols/<bin>.functions.tsv`.
- On the TV: never use kprobes on this kernel (it crashes).

## Open targets

In priority order:

1. `starfish-media-pipeline` `generateAudioDecodeInfo` (0x58630): does DTS ever get
   reported as `dts_hd_ma`? The decompiler lost the use of a `find("dts")` result; read
   the raw Thumb around VA 0x48b00-0x48bb0.
2. `audiod`: full Ghidra export (`tools/decomp.sh orig/usr/sbin/audiod`) to find other
   callers of `mixDigitalSoundOutput` and other policy feeding `setMixer`.
3. `libpf` and `libgstdecproxy`: sender and receiver of `acquired-resource` (core-type,
   audio-port, mixer-port). Compare decproxy with
   `lg-opensource/gstreamer/subprojects/gst-plugins-cool/gst/decproxy/gstdecproxy2.c`.
4. `kernel`: check the shipped `_get_output_format` / `_set_output` against
   `lg-opensource/kdriver/core/aud/alsa/alsa_sndout.c` (`ct<=9` limit differs). Load as AARCH64 with kallsyms.
5. `dsp1`: re-export. 68 functions in `decomp/audio_a0_dsp1…c` still carry wrong names.
6. DSP FLIX bundles: needs the core's TIE/ISA config or an `xt-objdump` for this core.
7. `libaa`, `dsp3`: not looked at yet.
8. Small plumbing libraries (`libumedia_api`, `libums_*`, `libresource_mgr_client_c`,
   `libdile_audio_pcm`): unlikely to change conclusions.

Known decompile failures: the `libgstlgaudiosink` event handler (`Duplicate
VariablePiece`; its gates were verified in raw instructions instead), and one function
each in `libgstlgaudiodec` and the sound-engine adaptation layer.
