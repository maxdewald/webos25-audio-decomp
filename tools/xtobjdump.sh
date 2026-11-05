#!/bin/sh
# Xtensa listing with FLIX bundles decoded (GNU objdump 2.43.1, Zephyr SDK 1.0.1
# xtensa-intel_tgl_adsp overlay; validated on LG DSP images, see build/scratch/flixdec/README.md).
# Branch slots decode as <op>.w18; only the branch slot of a bundle is validated.
#
# usage: tools/xtobjdump.sh <dsp0|dsp1|dsp2> <start_hex> <end_hex_exclusive>
set -eu
[ $# -eq 3 ] || { sed -n 's/^# usage: //p' "$0"; exit 2; }
root=$(cd "$(dirname "$0")/.." && pwd)
dsp=$1 start=0x${2#0x} end=0x${3#0x}
case "$dsp" in dsp0|dsp1|dsp2) ;; *) echo "invalid DSP: $dsp" >&2; exit 2 ;; esac
case "${start#0x}${end#0x}" in *[!0-9a-fA-F]*|'') echo "addresses must be hexadecimal" >&2; exit 2 ;; esac
[ "$start" != 0x ] && [ "$end" != 0x ] && [ "$((start))" -lt "$((end))" ] || {
    echo "expected a nonempty start < end range" >&2; exit 2;
}
img=$root/build/fw/audio_a0_${dsp}.main_60000000.bin
syms=$root/symbols/${dsp}.syms.tsv
dir=$root/build/scratch/flixdec
od=$dir/od/intel_tgl_adsp
[ -f "$img" ] || { echo "missing $img (make decomp-dsp)" >&2; exit 1; }

if [ ! -x "$od" ]; then
    case "$(uname -s)-$(uname -m)" in
        Darwin-arm64) plat=macos-aarch64 sum=8f38a94b32ec8641f5277a3ec834b3a6633f490e4882c01c3c0af198a68fad56 ;;
        Linux-x86_64) plat=linux-x86_64 sum=3a69c82e1efbdf158b5a57cba633bff398c561fbbf1473138ed51e8ab27bbc6c ;;
        Linux-aarch64) plat=linux-aarch64 sum=cbdbb90b038c5cfbdd0c0c175d3e295aa688bc2b4be2a3dffc5b2acf4c9710bc ;;
        *) echo "no prebuilt objdump for $(uname -s)-$(uname -m)" >&2; exit 1 ;;
    esac
    tc=xtensa-intel_tgl_adsp_zephyr-elf
    tar=$dir/dl/toolchain_gnu_${plat}_${tc}.tar.xz
    mkdir -p "$dir/dl" "$dir/od"
    [ -f "$tar" ] || curl -fsSL -m 600 -o "$tar" \
        "https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v1.0.1/toolchain_gnu_${plat}_${tc}.tar.xz"
    echo "$sum  $tar" | shasum -a 256 -c - >/dev/null
    tar -xJf "$tar" -O "$tc/bin/$tc-objdump" > "$od.part" && chmod +x "$od.part" && mv "$od.part" "$od"
fi

listing=$(mktemp "${TMPDIR:-/tmp}/xtobjdump.XXXXXX")
trap 'rm -f "$listing"' EXIT HUP INT TERM
"$od" -b binary -m xtensa --adjust-vma=0x60000000 --start-address="$start" --stop-address="$end" \
    -D "$img" > "$listing"
awk -F'\t' -v syms="$syms" '
    BEGIN { while ((getline l < syms) > 0) if (l !~ /^#/) { split(l, f, "\t"); s["0x" f[1]] = f[2] } }
    /^ *[0-9a-f]+:\t/ {
        a = $1; sub(/^ */, "", a); sub(/:$/, "", a)
        if (("0x" a) in s) print s["0x" a] ":"
        if (match($0, /call(x)?[048]?\t0x[0-9a-f]+/)) {
            t = substr($0, RSTART, RLENGTH); sub(/.*\t/, "", t)
            if (t in s) $0 = $0 "\t<" s[t] ">"
        }
        print; next
    }' "$listing"
