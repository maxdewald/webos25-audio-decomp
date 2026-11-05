# Agent rules

Setup, workflow and pitfalls: [README.md](README.md).

## Folders

Use only these. Do not create other top-level folders.

| Path | Put here | In git |
|---|---|---|
| `src/<target>/` | Reconstructed C/C++, one directory per original file | yes |
| `symbols/` | Function and symbol tables | yes |
| `tools/` | Reusable scripts (take paths or a target as arguments) | yes |
| `orig/` | LG's original files, mirroring the TV's `/` | `SHA256SUMS` only |
| `lg-opensource/` | LG's published source (`make opensource`) | no |
| `build/decomp/` | Ghidra pseudocode and string tables | no |
| `build/ghidra/`, `build/fw/`, `build/obj/` | Ghidra projects, split DSP images, objects | no |
| `build/logs/` | Logs of headless and batch runs | no |
| `build/scratch/` | One-off scripts, listings, dumps, notes | no |

## Never commit

- Anything from `orig/` or `lg-opensource/`.
- Anything derived from LG binaries: disassembly, pseudocode, hex or string dumps.
  These go in `build/`. Comments in `src/` may quote addresses and short log strings.

## Conventions

- Mark code you could not recover (`[FLIX]`, decompiler gaps); never guess it.
- Record what a function answers as a comment next to it, not in separate notes.
- A script worth keeping goes in `tools/` with a usage line; everything else in
  `build/scratch/`.
- After adding functions, run `make progress` and commit `progress.svg`.
