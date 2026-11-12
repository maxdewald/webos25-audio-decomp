#!/bin/zsh
# usage: decomp-fw.sh <main_60000000.bin> <syms.tsv>   (Xtensa LE raw image linked at 0x60000000)
set -eu
here=${0:A:h}
root=${here:h}
bin=${1:A}
syms=${2:A}
name=${bin:t}
export JAVA_HOME=/opt/homebrew/opt/openjdk@25
mkdir -p "$root/build/ghidra/$name" "$root/build/decomp" "$root/build/logs"
/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless "$root/build/ghidra/$name" "$name" \
  -import "$bin" -overwrite \
  -loader BinaryLoader -loader-baseAddr 0x60000000 \
  -processor Xtensa:LE:32:hifi_lg \
  -scriptPath "$here" -preScript ApplySyms.java "$syms" -preScript XtensaSeed.java \
  -postScript XtensaFill.java -postScript ExportDecomp.java "$root/build/decomp" \
  -max-cpu 6 > "$root/build/logs/$name.log" 2>&1
mv "$root/build/decomp/$name.functions.tsv" "$root/symbols/"
grep -E 'APPLY_SYMS|XTENSA_SEED|XTENSA_FILL|EXPORTED' "$root/build/logs/$name.log" | head -5
