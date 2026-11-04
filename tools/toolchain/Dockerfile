# Same compiler as the shipped LG userspace (.comment "GCC: (GNU) 11.4.0"):
# ARMv7-A Thumb-2, soft-float EABI5 (ELF flags 0x5000200).
FROM ubuntu:22.04
RUN apt-get update -qq \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y -qq --no-install-recommends \
      gcc-11-arm-linux-gnueabi libc6-dev-armel-cross make \
 && rm -rf /var/lib/apt/lists/* \
 && ln -s /usr/bin/arm-linux-gnueabi-gcc-11 /usr/local/bin/arm-linux-gnueabi-gcc
WORKDIR /w
