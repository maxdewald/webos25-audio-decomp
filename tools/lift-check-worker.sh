#!/bin/sh
# Container-side worker for lift-check.sh; not a standalone host command.
set -eu
dir=$1
jobs=$2
log=$3
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT HUP INT TERM
set -- "$dir"/*.c
[ -f "$1" ] || { echo "no C files in $dir" >&2; exit 2; }
rc=0
printf '%s\n' "$@" | xargs -P "$jobs" -I '{}' sh -c '
    file=$1
    out=$2/$(basename "$file")
    if arm-linux-gnueabi-gcc -O2 -fno-ipa-icf -mthumb -march=armv7-a -mfpu=neon \
        -mfloat-abi=softfp -fPIC -fno-stack-protector -U_FORTIFY_SOURCE \
        -Isrc/include -w -c "$file" -o "$out.o" > "$out.errors" 2>&1; then
        printf "PASS %s\n" "$file"
    else
        printf "FAIL %s\n" "$file"
        cat "$out.errors"
        exit 1
    fi
' sh '{}' "$work" || rc=$?
cat "$work"/*.errors > "$log"
exit "$rc"
