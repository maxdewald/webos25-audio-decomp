#!/bin/sh
# usage: tools/lift-check.sh build/lift/<target> [jobs]
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$root"
dir=${1:-build/lift/libpf}
jobs=${2:-4}
case "$jobs" in ''|*[!0-9]*|0) echo 'jobs must be a positive integer' >&2; exit 2;; esac
case "$dir" in build/lift/*) ;; *) echo 'input must be under build/lift/' >&2; exit 2;; esac
[ -d "$dir" ] || { echo "missing directory: $dir" >&2; exit 2; }
mkdir -p build/logs
name=$(basename "$dir")
docker run --rm -v "$root:/w" -w /w webos25-decomp-gcc \
    sh tools/lift-check-worker.sh "$dir" "$jobs" "build/logs/lift-$name.errors"
