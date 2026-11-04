#!/bin/sh
set -eu
cat /etc/starfish-release
for instance in /proc/aaudio/processors/dsp2/lipsync/[0-9]*; do
    printf '\nINSTANCE %s\n' "$instance"
    for item in status lstatus prop; do
        printf '\n%s\n' "$item"
        cat "$instance/$item"
    done
done