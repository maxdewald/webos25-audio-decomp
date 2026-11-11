# webos25-audio-decomp

Source-assisted reconstruction of the LG webOS 25 audio output path, from the media pipeline
down to the eARC output on the DSP. Firmware: LG OLED42C27LA (C2), Rockhopper
10.3.1-3006, kernel 5.4.268-329.ptl4tv.5.

> Original proprietary binaries are never committed; only their hashes are tracked
> ([orig/SHA256SUMS](orig/SHA256SUMS)). `src/` contains reconstructions; public reference
> sources and their license notices stay under `lg-opensource/`.

![Decompilation progress per original file](progress.svg)

## Setup

```sh
make verify        # orig/ holds the expected files (pull them from a rooted TV)
make toolchain     # GCC 11.4.0 ARMv7-A Thumb-2 image, the compiler LG used
make public-sources # pinned public dependencies and webOS reference implementations
make opensource     # optional: LG's firmware GPL/LGPL source (~4.4 GB)
```

To regenerate `build/decomp/`: Ghidra 12.2 requires **JDK 25**. The existing
`decomp-*.sh` wrappers use macOS/Homebrew paths; on Linux invoke
`support/analyzeHeadless` from your Ghidra installation with `JAVA_HOME` pointing
to a JDK 25 installation. Install `tools/ghidra-ext/XtensaHiFi` in Ghidra's
`Ghidra/Processors/` directory. Keep raw exports under `build/decomp/`.
`tools/xtobjdump.sh` also works directly on Linux without Ghidra; split the
hash-verified DSP image with `tools/split_fw.py` first.

`src/dsp0/hdmi.c` reconstructs `_hdmi_push_first_event` and `hdmi_prop_updated`
from the hash-matched C2 DSP0 image. Its comments distinguish FIRST metadata
from payload processing: codec 4 announces MAT and codec 6 announces DTS-HD MA,
but neither the property callback nor the FIRST builder proves transparent
end-to-end relabelling. `make test T=dsp0_hdmi` checks the recovered behavior.

## Workflow

```sh
make status                          # coverage and instruction match per target
make todo T=libumi                   # largest functions not yet reconstructed
make diff T=libumi F=setOutputType   # instruction diff, original vs compiled src/
make progress                        # regenerate chart; NO_MATCH=1 for coverage only
make test                            # host tests in tools/tests/ (ASan/UBSan); T=name filters
```

1. Check `make public-source-map` first (references, not verified coverage), then `make todo`.
2. Write it in `src/<target>/`, tagged with its Ghidra address on the line before:

   ```c
   /* @0x4fc24: what it does, if not obvious */
   static void setOutputType(struct umi_tv_audio *a, int ct, int mo)
   ```

3. Iterate with `make diff` until the match is as high as it goes.

**Coverage** is the share of a target's function bytes with a reconstruction. **Match**
compiles `src/` with LG's compiler and compares instructions per function (addresses
ignored); 100% is byte-for-byte equivalent. DSP (Xtensa) and kernel (aarch64) targets
report coverage only.

## Addresses

- ARM binaries: Ghidra address = raw ELF VA + 0x10000. Code is Thumb.
- DSP images are linked at 0x60000000.

## Pitfalls

- DSP symbol tables are `(func_ptr, name_ptr)` pairs; literal pools hold look-alikes.
  Confirm a name with `tools/symtab_probe.py` and the function's logged `__func__`.
- Disassemble ARM with `--triple=armv7 --mattr=+thumb-mode`. Plain `thumbv7` crashes
  on barrier instructions.
- Ghidra cannot decode Xtensa FLIX. `tools/xtobjdump.sh dsp2 START END` decodes validated
  branch slots; unresolved SIMD/float slots remain marked `[FLIX]`.
- Ghidra drops the string arguments of `std::string::compare`. Recover them with
  `tools/funcstrings.py`.
