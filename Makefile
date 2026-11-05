PY ?= python3

.PHONY: status diff todo progress toolchain verify symbols opensource public-sources test-libpf decomp-arm decomp-dsp

status:            ## coverage + instruction match per target
	$(PY) tools/progress.py status

diff:              ## make diff T=libumi F=setOutputType
	$(PY) tools/progress.py diff $(T) $(F)

todo:              ## make todo T=libumi
	$(PY) tools/progress.py todo $(T)

progress:          ## progress.svg for the README
	$(PY) tools/progress.py svg

toolchain:         ## GCC 11.4 armel cross compiler image
	docker build -t webos25-decomp-gcc tools/toolchain

verify:            ## original binaries are the expected ones
	cd orig && shasum -a 256 -c --quiet SHA256SUMS

symbols:           ## ELF symbol tables for targets without a Ghidra export
	$(PY) tools/progress.py symbols

opensource:        ## LG's GPL/LGPL source for this firmware into lg-opensource/
	tools/fetch-opensource.sh lg-opensource

public-sources:   ## pinned Boost/RapidJSON dependencies and LG reference source
	$(PY) tools/fetch-public-sources.py

test-libpf:        ## host sanitizer tests; requires public-sources and native GLib
	$(PY) tools/test_libpf.py

decomp-arm:        ## full Ghidra export of every ARM target (slow)
	tools/decomp-arm-all.sh

decomp-dsp:        ## split + export dsp0..2 (slow)
	for n in 0 1 2; do \
	  $(PY) tools/split_fw.py orig/lib/firmware/audio_a0_dsp$$n.bin build/fw && \
	  tools/decomp-fw.sh build/fw/audio_a0_dsp$$n.main_60000000.bin symbols/dsp$$n.syms.tsv; \
	done
