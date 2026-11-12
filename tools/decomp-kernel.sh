#!/bin/zsh
# usage: tools/decomp-kernel.sh import                 raw Image + kallsyms -> build/ghidra/kernel (once)
#        tools/decomp-kernel.sh fn OUT.c NAME|0xADDR... decompile functions (with callers) into OUT.c
# Needs orig/boot/Image-5.4.268-329.decompressed and orig/proc/kallsyms (tools/pull-orig.sh).
# KPROJ=name uses a copy build/ghidra/<name>, so parallel runs don't share the project lock.
set -eu
here=${0:A:h}
root=${here:h}
img=$root/orig/boot/Image-5.4.268-329.decompressed
proj=$root/build/ghidra/${KPROJ:-kernel}
[[ -d $proj || $1 == import ]] || cp -R $root/build/ghidra/kernel $proj
export JAVA_HOME=/opt/homebrew/opt/openjdk@25
export GHIDRA_HEADLESS_MAXMEM=${GHIDRA_HEADLESS_MAXMEM:-16G}
headless=/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless
mkdir -p $proj $root/build/logs $root/build/kernel
case $1 in
import)
  # 5.4 arm64: link base of file offset 0 = (_stext & ~0xfffff) + text_offset 0x88000.
  syms=$root/build/kernel/kallsyms.text.tsv
  awk '$2 ~ /^[Tt]$/ && $1 >= "ffffffc010089000" && $1 < "ffffffc0110a0000" {print substr($1,1) "\t" $3}' \
    $root/orig/proc/kallsyms | sort -u -k1,1 > $syms
  $headless $proj Image -import $img -overwrite -noanalysis \
    -processor AARCH64:LE:64:v8A -loader BinaryLoader -loader-baseAddr 0xffffffc010088000 \
    -scriptPath $here -postScript ApplySyms.java $syms -max-cpu 8 \
    > $root/build/logs/kernel-import.log 2>&1
  grep -E 'APPLY_SYMS|ERROR' $root/build/logs/kernel-import.log | head -5 ;;
fn)
  out=${2:A}; shift 2
  $headless $proj Image -process ${img:t} -noanalysis -readOnly \
    -scriptPath $here -postScript DecompFuncs.java $out "$@" \
    > $root/build/logs/kernel-fn.$$.log 2>&1
  grep -c '^/\* @' $out ;;
esac
