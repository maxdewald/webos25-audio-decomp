PY ?= python3

.PHONY: status diff todo progress test toolchain verify symbols opensource public-sources public-source-map decomp-arm decomp-dsp

status:            ## coverage + instruction match per target
	$(PY) tools/progress.py status

diff:              ## make diff T=libumi F=setOutputType
	$(PY) tools/progress.py diff $(T) $(F)

todo:              ## make todo T=libumi
	$(PY) tools/progress.py todo $(T)

progress:          ## progress.svg for the README
	$(PY) tools/progress.py svg $(if $(NO_MATCH),--no-match)

test:              ## host tests in tools/tests/; make test T=lgao
	$(PY) tools/runtests.py $(T)
	$(PY) -m unittest discover -s tools/tests -p 'test_*.py'

toolchain:         ## GCC 11.4 armel cross compiler image
	docker build -t webos25-decomp-gcc tools/toolchain

verify:            ## original binaries are the expected ones
	cd orig && shasum -a 256 -c --quiet SHA256SUMS

symbols:           ## ELF symbol tables for targets without a Ghidra export
	$(PY) tools/progress.py symbols

opensource:        ## LG's GPL/LGPL source for this firmware into lg-opensource/
	tools/fetch-opensource.sh lg-opensource

public-sources:    ## pinned public dependencies and webOS reference implementations
	$(PY) tools/fetch-public-sources.py

public-source-map: ## upstream paths per target (not verified binary coverage)
	$(PY) tools/fetch-public-sources.py --list


decomp-arm:        ## full Ghidra export of every ARM target (slow)
	tools/decomp-arm-all.sh

decomp-dsp:        ## split + export dsp0..2 (slow)
	for n in 0 1 2; do \
	  $(PY) tools/split_fw.py orig/lib/firmware/audio_a0_dsp$$n.bin build/fw && \
	  tools/decomp-fw.sh build/fw/audio_a0_dsp$$n.main_60000000.bin symbols/dsp$$n.syms.tsv; \
	done
