#!/bin/zsh
# usage: tools/pull-orig.sh [ssh-host]   (default webos-tv; needs root on the TV)
# Copies every file in orig/SHA256SUMS that exists on the TV, plus /proc/kallsyms and
# the stock aspectratiodrv.ko, into orig/ under the TV's own paths, then verifies.
set -eu
host=${1:-webos-tv}
root=${0:A:h:h}
orig=$root/orig
files=(${(f)"$(awk '{print $2}' $orig/SHA256SUMS | grep -v '^boot/Image-')"})
release=$(ssh -o BatchMode=yes -o ConnectTimeout=10 $host uname -r)
files+=(lib/modules/$release/kernel/aspectratiodrv.ko)
ssh -o BatchMode=yes $host "cd / && tar cf - ${files[*]}" | tar xf - -C $orig
mkdir -p $orig/proc
ssh -o BatchMode=yes $host 'cat /proc/kallsyms' > $orig/proc/kallsyms
print "pulled ${#files} files + proc/kallsyms from $host ($release)"
cd $orig && shasum -a 256 -c --quiet SHA256SUMS
