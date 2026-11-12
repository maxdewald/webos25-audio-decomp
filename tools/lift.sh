#!/bin/sh
# usage: tools/lift.sh <binary> <target>   e.g. tools/lift.sh libpf-1.0.so.1.0.0 libpf
# Exports Ghidra's token trees from build/ghidra/<binary> (made by tools/decomp.sh),
# then writes experimental C to build/lift/<target>/ and compile-checks it.
set -eu
[ "$#" -eq 2 ] || { echo "usage: $0 <binary> <target>" >&2; exit 2; }
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$root"
bin=$1; target=$2
case "$bin:$target" in *[!a-zA-Z0-9_.:+-]*) echo 'invalid binary or target name' >&2; exit 2;; esac
mkdir -p build/lift build/logs
if ! JAVA_HOME=${JAVA_HOME:-/opt/homebrew/opt/openjdk@25} \
  "${GHIDRA_HEADLESS:-/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless}" \
  "build/ghidra/$bin" "$bin" -process "$bin" -noanalysis -readOnly -scriptPath tools \
  -postScript ExportTokens.java build/lift > "build/logs/lift-$bin.log" 2>&1; then
    tail -n 40 "build/logs/lift-$bin.log"
    exit 1
fi
python3 tools/lift.py "$target"
tools/lift-check.sh "build/lift/$target"
