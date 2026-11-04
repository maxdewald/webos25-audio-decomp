#!/usr/bin/env python3
"""Extract address-bound lipsync pseudocode from the existing Ghidra export."""

import hashlib
import json
from pathlib import Path
import re
import struct


root = Path(__file__).resolve().parent.parent
output = root / "lipsync"
output.mkdir(exist_ok=True)
program = "audio_a0_dsp2.main_60000000.bin"
source = root / "decomp" / (program + ".c")
firmware = root / "orig/lib/firmware/audio_a0_dsp2.bin"
text = source.read_text()
marker = re.compile(r"^/\* @([0-9a-f]+) ([^\n]+) \*/", re.MULTILINE)
matches = list(marker.finditer(text))
selected = []
shared = []
inventory = []

for index, match in enumerate(matches):
    address = int(match.group(1), 16)
    name = match.group(2)
    end = matches[index + 1].start() if index + 1 < len(matches) else len(text)
    body = text[match.start():end]
    if 0x600A0ED8 <= address < 0x600A46BC:
        selected.append(body)
        inventory.append({"address": match.group(1), "name": name})
    elif name.split("__blk_", 1)[0] in {
        "aud_module_push_data", "aud_module_set_prop", "aud_module_get_prop",
    }:
        shared.append(body)

assert any(item["name"] == "_lipsync_get_stc" for item in inventory)
assert any(item["name"] == "_lipsync_event" for item in inventory)
notice = (
    "/* Ghidra pseudocode, not original source. FLIX operations remain opaque;\n"
    " * __blk_ routines are synthetic exports, not proven independent functions. */\n\n"
)
(output / "dsp2-lipsync.c").write_text(notice + "".join(selected))
(output / "dsp2-module-output.c").write_text(notice + "".join(shared))
manifest = {
    "firmware": str(firmware.relative_to(root)),
    "sha256": hashlib.sha256(firmware.read_bytes()).hexdigest(),
    "program": program,
    "source_export": str(source.relative_to(root)),
    "selected_address_start": "0x600a0ed8",
    "selected_address_end_exclusive": "0x600a46bc",
    "limitations": [
        "Static Ghidra observations, not runtime execution or recovered original source.",
        "FLIX bundle lengths inferred; slot semantics not implemented in this language.",
        "Synthetic block function boundaries and their arguments are not trustworthy.",
    ],
    "functions_and_synthetic_blocks": inventory,
}
image = (root / "fw" / program).read_bytes()
base = 0x60000000


def read_name(address):
    offset = address - base
    if not 0 <= offset < len(image):
        return None
    end = image.find(b"\0", offset, offset + 120)
    if end < 0:
        return None
    raw = image[offset:end]
    if not raw or any(value < 32 or value > 126 for value in raw):
        return None
    return raw.decode("ascii")


def property_at(offset):
    if offset < 0 or offset + 40 > len(image):
        return None
    words = struct.unpack_from("<10I", image, offset)
    name = read_name(words[0])
    if name is None or words[1] > 6 or words[2] > 0x1000 or words[3] > 3:
        return None
    return {
        "address": hex(base + offset), "name": name, "type": words[1],
        "offset": hex(words[2]), "flags": words[3],
        "setter": hex(words[4]), "getter_string": hex(words[5]),
        "getter_int": hex(words[6]), "raw_words": [hex(value) for value in words],
    }


tables = {}
for anchor in ("clockType", "drop-output-data"):
    name_offset = image.find(b"\0" + anchor.encode() + b"\0")
    assert name_offset >= 0, anchor
    pointer = struct.pack("<I", base + name_offset + 1)
    candidates = [offset for offset in range(0, len(image) - 40, 4)
                  if image[offset:offset + 4] == pointer and property_at(offset)]
    assert len(candidates) == 1, (anchor, candidates)
    start = candidates[0]
    while property_at(start - 40):
        start -= 40
    rows = []
    while (row := property_at(start)) is not None:
        rows.append(row)
        start += 40
    tables[anchor] = rows

assert next(row for row in tables["clockType"] if row["name"] == "clockType")["offset"] == "0x200"
assert next(row for row in tables["drop-output-data"] if row["name"] == "drop-output-data")["offset"] == "0x94"
(output / "dsp2-properties.json").write_text(json.dumps(tables, indent=2) + "\n")
manifest["property_tables"] = "evidence/dsp2-lipsync/dsp2-properties.json"
(output / "dsp2-lipsync.json").write_text(json.dumps(manifest, indent=2) + "\n")
print(f"Extracted {len(selected)} lipsync functions/blocks and {len(shared)} shared functions/blocks")
print("Verified properties: clockType at 0x200; drop-output-data at 0x94")
print(f"Firmware SHA-256: {manifest['sha256']}")