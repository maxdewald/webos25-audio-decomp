#!/bin/zsh
# usage: decomp.sh <binary> [extra analyzeHeadless args...]
set -eu
here=${0:A:h}
root=${here:h}
bin=${1:A}; shift
name=${bin:t}
export JAVA_HOME=/opt/homebrew/opt/openjdk@21
mkdir -p "$root/build/ghidra/$name" "$root/build/decomp" "$root/build/logs"
/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless "$root/build/ghidra/$name" "$name" \
  -import "$bin" -overwrite "$@" \
  -scriptPath "$here" -postScript ExportDecomp.java "$root/build/decomp" \
  -max-cpu 4 > "$root/build/logs/$name.log" 2>&1
mv "$root/build/decomp/$name.functions.tsv" "$root/symbols/"
grep -E 'EXPORTED|ERROR' "$root/build/logs/$name.log" | head -5
