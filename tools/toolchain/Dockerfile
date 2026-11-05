# Same compiler as the shipped LG userspace (.comment "GCC: (GNU) 11.4.0"):
# ARMv7-A Thumb-2, soft-float EABI5 (ELF flags 0x5000200).
# glib/gstreamer dev packages only supply the arch-independent headers for
# libgstdecproxy; src/include/gst-gen/glibconfig.h overrides the 64-bit glibconfig.h.
FROM ubuntu:22.04
RUN apt-get update -qq \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y -qq --no-install-recommends \
      gcc-11-arm-linux-gnueabi g++-11-arm-linux-gnueabi libc6-dev-armel-cross make \
      libglib2.0-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
 && rm -rf /var/lib/apt/lists/* \
 && ln -s /usr/bin/arm-linux-gnueabi-gcc-11 /usr/local/bin/arm-linux-gnueabi-gcc \
 && ln -s /usr/bin/arm-linux-gnueabi-g++-11 /usr/local/bin/arm-linux-gnueabi-g++
WORKDIR /w
