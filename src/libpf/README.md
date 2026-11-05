# libpf reconstruction

`orig/usr/lib/libpf-1.0.so.1.0.0`: the starfish media pipeline framework (C++, namespace
`mediapipeline`). It builds the GStreamer pipelines for every player type and sends
`acquired-resource` (core-type, video-port, audio-port, mixer-port) downstream.

- One `.cpp` per original source file, named after its `__FILE__` string
  (`airplayer.cpp`, `pipeline.cpp`, ...). Very large files are split as
  `<file>_<n>.cpp`. Functions that have no `__FILE__` string go in `runtime.cpp`.
- Each file declares, at its top, only the classes and members it defines. Object
  layout is accessed through `FIELD()` from [libpf.h](libpf.h), with each offset
  listed in the file header. Layouts are not shared between files.
- Logging: `PF_LOG_DEBUG` (level 7, formatted only at context level 7) and
  `PF_LOG_INFO`/`PF_LOG_WARNING`/`PF_LOG_ERROR` (msgid `SFPIPELINE`).
- Ghidra drops string literals (e.g. `std::string::compare` arguments, trailing
  `gst_structure_new` fields). Recover them from `build/decomp/libpf-1.0.so.1.0.0.funcstrings.tsv`.
- Boost, libstdc++ and other library template instances (`boost::…`, `std::…`,
  `_INIT_n`) are not reconstructed.
