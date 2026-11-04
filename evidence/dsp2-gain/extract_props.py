#!/usr/bin/env python3
"""Dump the gain module aud_module_property table(s) (10 words / 40 bytes per row)."""
import json
import struct
from pathlib import Path

here = Path(__file__).resolve().parent
root = here.parent.parent
image = (root / "build/fw/audio_a0_dsp2.main_60000000.bin").read_bytes()
base = 0x60000000
syms = {}
for line in (root / "symbols/dsp2.syms.tsv").read_text().splitlines():
    a, n = line.split("\t", 1)
    syms[int(a, 16)] = n


def read_name(address):
    off = address - base
    if not 0 <= off < len(image):
        return None
    end = image.find(b"\0", off, off + 120)
    raw = image[off:end] if end > 0 else b""
    if not raw or any(c < 32 or c > 126 for c in raw):
        return None
    return raw.decode()


def sym(v):
    return syms.get(v, "") if v else ""


def row(off):
    if off < 0 or off + 40 > len(image):
        return None
    w = struct.unpack_from("<10I", image, off)
    name = read_name(w[0])
    if name is None or w[1] > 8 or w[2] > 0x1000 or w[3] > 0xff:
        return None
    return {
        "row_address": hex(base + off), "name": name, "type": w[1], "offset": hex(w[2]),
        "flag": w[3], "set": hex(w[4]), "set_sym": sym(w[4]), "gets": hex(w[5]), "gets_sym": sym(w[5]),
        "geti": hex(w[6]), "geti_sym": sym(w[6]), "priv": hex(w[7]),
        "link": [hex(w[8]), hex(w[9])], "raw_words": [hex(x) for x in w],
    }


tables = {}
for anchor in ("mute-input", "mute-output", "current-gain", "final-gain", "fade-gain", "{ease-volume}", "stop-after"):
    hits = []
    pos = 0
    while (pos := image.find(b"\0" + anchor.encode() + b"\0", pos)) >= 0:
        hits.append(base + pos + 1)
        pos += 1
    for name_addr in hits:
        ptr = struct.pack("<I", name_addr)
        for off in range(0, len(image) - 40, 4):
            if image[off:off + 4] == ptr and row(off):
                start = off
                while row(start - 40):
                    start -= 40
                key = hex(base + start)
                if key in tables:
                    continue
                rows = []
                while (r := row(start)) is not None:
                    rows.append(r)
                    start += 40
                term = struct.unpack_from("<10I", image, start)
                tables[key] = {"anchor": anchor, "anchor_string": hex(name_addr), "rows": rows,
                               "terminator_row": {"address": hex(base + start), "raw_words": [hex(x) for x in term]}}

out = here / "gain-properties.json"
out.write_text(json.dumps({
    "firmware": "build/fw/audio_a0_dsp2.main_60000000.bin", "base": hex(base),
    "row_layout": "name,type,offset,flag,set,gets,geti,priv,link[2] (40 bytes); offset relative to module struct",
    "tables": tables}, indent=2) + "\n")
for k, t in tables.items():
    print(k, t["anchor"])
    for r in t["rows"]:
        print(f"  {r['row_address']} {r['name']:<16} type={r['type']} off={r['offset']:<6} flag={r['flag']} "
              f"set={r['set']}({r['set_sym']}) gets={r['gets']}({r['gets_sym']}) geti={r['geti']}({r['geti_sym']}) priv={r['priv']}")
