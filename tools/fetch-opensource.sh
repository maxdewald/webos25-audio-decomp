#!/bin/sh
# Download LG's published GPL/LGPL source for this firmware from opensource.lge.com
# (model OLED42C27LA, "webOS25 WG ReNew") and unpack the audio-relevant parts:
#   gstreamer/  kdriver/  kernel-k25lp-5.4.268-292/
set -eu

OS_SEQ=512189
MODEL=OLED42C27LA
KERNEL=kernel-k25lp-5.4.268-292
DEST=${1:-lg-opensource}
DL=$DEST/.download
TMP=$DEST/.unpack

mkdir -p "$DL"

fetch() { # fetch <release file index>: $DL/release_<index>.tar.gz
	out=$DL/release_$1.tar.gz
	[ -s "$out" ] && [ ! -e "$out.aria2" ] && return
	url=$(curl -fsS -m 60 -X POST https://opensource.lge.com/download/releaseFileDownloadUrl \
		--data "osSeq=$OS_SEQ&fileType=Op&fileIdx=$1&modelName=$MODEL" |
		python3 -c 'import json, sys; print(json.load(sys.stdin)["data"])')
	echo "downloading $out"
	if command -v aria2c >/dev/null; then
		aria2c -q -c -x 16 -s 16 -k 8M -d "$DL" -o "release_$1.tar.gz" "$url"
	else
		curl -fL --retry 5 -C - -o "$out" "$url"
	fi
}

# Each part is unpacked into $TMP and moved into place only when complete.
fresh() { rm -rf "$TMP"; mkdir -p "$TMP"; echo "unpacking $DEST/$1"; }

fetch 2
fetch 3

if [ ! -d "$DEST/gstreamer" ]; then
	fresh gstreamer
	tar xzf "$DL/release_2.tar.gz" -C "$TMP" code/gstreamer_open_source.zip
	unzip -p "$TMP/code/gstreamer_open_source.zip" gstreamer_open_source.tar.gz | tar xzf - -C "$TMP"
	mv "$TMP/gstreamer" "$DEST/gstreamer"
fi

if [ ! -d "$DEST/$KERNEL" ]; then
	fresh "$KERNEL"
	tar xzf "$DL/release_2.tar.gz" -C "$TMP" code/linux-rockhopper_build_k25lp_5-4-268-292.tgz
	mkdir "$TMP/$KERNEL"
	tar xzf "$TMP/code/linux-rockhopper_build_k25lp_5-4-268-292.tgz" -C "$TMP/$KERNEL" \
		--exclude '*.tgz'
	mv "$TMP/$KERNEL" "$DEST/$KERNEL"
fi

if [ ! -d "$DEST/kdriver" ]; then
	fresh kdriver
	tar xzf "$DL/release_3.tar.gz" -C "$TMP" soc_GO/linux-and-kdriver_paluma-5-241025-0301.tar.gz
	tar xzf "$TMP/soc_GO/linux-and-kdriver_paluma-5-241025-0301.tar.gz" -C "$TMP" \
		linux-and-kdriver_paluma-5/kdriver
	mv "$TMP/linux-and-kdriver_paluma-5/kdriver" "$DEST/kdriver"
fi

rm -rf "$TMP"
echo "done: $DEST (downloads cached in $DL)"
