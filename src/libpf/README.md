# libpf reconstruction

`orig/usr/lib/libpf-1.0.so.1.0.0`: the starfish media pipeline framework (C++, namespace
`mediapipeline`). It builds the GStreamer pipelines for every player type and sends
`acquired-resource` (core-type, video-port, audio-port, mixer-port) downstream.

- Name reconstructed files after the original `__FILE__` string where known;
  related resource-delivery functions may share a file.
- Opaque layouts use `FIELD()` from [libpf.h](libpf.h); recovered types use headers.
  `make public-sources` restores pinned Boost/RapidJSON headers; upstream source
  remains under `lg-opensource/public/` and is not counted as reconstructed coverage.
- Logging: `PF_LOG_DEBUG` (level 7, formatted only at context level 7) and
  `PF_LOG_INFO`/`PF_LOG_WARNING`/`PF_LOG_ERROR` (msgid `SFPIPELINE`).
- Ghidra drops string literals (e.g. `std::string::compare` arguments, trailing
  `gst_structure_new` fields). Recover them from `build/decomp/libpf-1.0.so.1.0.0.funcstrings.tsv`.
- `tools/lift.sh libpf-1.0.so.1.0.0 libpf` writes experimental compilable C under
  `build/lift/libpf/`. It retains decompiler gaps and is excluded from source coverage.
  The complete unmodified Ghidra export is under `build/decomp/`.
