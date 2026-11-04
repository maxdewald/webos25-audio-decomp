#!/bin/zsh
set -eu
root=${0:A:h:h:h}
owned="$root/evidence/earc-arm"
ghidra=/opt/homebrew/opt/ghidra/libexec
mkdir -p "$owned/runtime/home" "$owned/runtime/tmp" "$owned/runtime/cache" "$owned/runtime/settings"
JAVA_HOME=/opt/homebrew/opt/openjdk@21 HOME="$owned/runtime/home" XDG_CACHE_HOME="$owned/runtime/cache" \
    /opt/homebrew/opt/openjdk@21/bin/java -Xmx2G -XX:ParallelGCThreads=2 -XX:CICompilerCount=2 \
    -Djava.awt.headless=true -Duser.home="$owned/runtime/home" -Djava.io.tmpdir="$owned/runtime/tmp" \
    -Dapplication.settingsdir="$owned/runtime/settings" -Dapplication.cachedir="$owned/runtime/cache" \
    -cp "$ghidra/Ghidra/Framework/Utility/lib/Utility.jar" ghidra.Ghidra ghidra.app.util.headless.AnalyzeHeadless \
    "$root/build/ghidra/libgstlgaudiosink.so" libgstlgaudiosink.so \
    -process libgstlgaudiosink.so -readOnly -noanalysis -max-cpu 2 \
    -scriptPath "$owned" -postScript EarcArmSinkExport20261104.java "$owned" \
    -log "$owned/ghidra-local.log" -scriptlog "$owned/ghidra-script.log"