#!/bin/zsh
here=${0:A:h}
b=${here:h}/orig
print -l \
  $b/usr/lib/libumi.so.1.0.0 \
  $b/usr/lib/libaudio-adaptation-layer-soundengine.so.1.0.0 \
  $b/usr/lib/libaudio-adaptation-layer-amplifier.so.1.0.0 \
  $b/usr/lib/libaudio-adaptation-layer-delay-gain.so.1.0.0 \
  $b/usr/lib/libkadaptor.so.2.0.1 \
  $b/usr/lib/libAudioResources.so.1.0.0 \
  $b/usr/lib/libmedia-resource-calculator.so.1 \
  $b/usr/lib/libresource_mgr.so.1.0.0 \
  $b/usr/lib/libresource_mgr_client.so.1.0.0 \
  $b/usr/lib/libums_pipeline_mgr.so.1.0.0 \
  $b/usr/lib/libums_pipeline.so.1.0.0 \
  $b/usr/lib/libav-proxy.so.1.0.0 \
  $b/usr/lib/libav-connector.so.1.0.0 \
  $b/usr/lib/libavacontroller.so.1.0.0 \
  $b/usr/lib/libdile_audio_direct.so.1.0.0 \
  $b/usr/lib/gstreamer-1.0/libgstlgaudiosink.so \
  $b/usr/lib/gstreamer-1.0/libgstlgaudiodec.so \
  $b/usr/sbin/umediaserver \
  $b/usr/sbin/starfish-media-pipeline \
| xargs -P 4 -I{} $here/decomp.sh {}
