import sys

from pathlib import Path

BIN = Path(__file__).resolve().parents[2] / "build/fw/audio_a0_dsp2.main_60000000.bin"
BASE = 0x60000000
data = open(BIN, "rb").read()
for a in sys.argv[1:]:
    off = int(a, 16) - BASE
    end = data.index(b"\0", off)
    print(f"{a}\t{data[off:end].decode('latin-1')!r}")
