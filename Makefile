PY ?= python3

.PHONY: status diff todo toolchain verify validate symbols decomp-arm decomp-dsp

status:            ## coverage + instruction match per target
	$(PY) tools/progress.py status

diff:              ## make diff T=libumi F=setOutputType
	$(PY) tools/progress.py diff $(T) $(F)

todo:              ## make todo T=libumi
	$(PY) tools/progress.py todo $(T)

toolchain:         ## GCC 11.4 armel cross compiler image
	docker build -t webos25-decomp-gcc tools/toolchain

verify:            ## original binaries are the expected ones
	cd orig && shasum -a 256 -c --quiet SHA256SUMS

validate: verify   ## re-check every evidence report against orig/
	$(PY) evidence/audiod-mixer/index_audiod.py > /dev/null
	$(PY) evidence/audiod-mixer/validate_findings.py
	node evidence/luna-contract/verify-evidence.mjs > /dev/null
	$(PY) evidence/renderer-abi/validate.py
	node evidence/resource-booking/verify-evidence.mjs --check

symbols:           ## ELF symbol tables for targets without a Ghidra export
	$(PY) tools/progress.py symbols

decomp-arm:        ## full Ghidra export of every ARM target (slow)
	tools/decomp-arm-all.sh

decomp-dsp:        ## split + export dsp0..2 (slow)
	for n in 0 1 2; do \
	  $(PY) tools/split_fw.py orig/lib/firmware/audio_a0_dsp$$n.bin build/fw && \
	  tools/decomp-fw.sh build/fw/audio_a0_dsp$$n.main_60000000.bin symbols/dsp$$n.syms.tsv; \
	done
