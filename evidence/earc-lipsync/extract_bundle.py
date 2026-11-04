#!/usr/bin/env python3
"""Export saved, local DSP2 evidence without opening or changing a database."""

import hashlib
import json
from pathlib import Path
import re
import struct


OUTPUT = Path(__file__).resolve().parent
ROOT = OUTPUT.parent.parent
TARGETS = {
    0x600A2078: "_lipsync_get_pts_for_media",
    0x600A2644: "_lipsync_state",
    0x600A2EAC: "_lipsync_get_pts",
    0x600A2F08: "_lipsync_get_stc",
    0x600A3040: "_lipsync_core",
    0x600A38DD: "FUN_600a38dd",
    0x600A39AB: "FUN_600a39ab",
    0x600A40B9: "FUN_600a40b9",
}
SUPPORT = {
    "_lipsync_constructor", "_lipsync_put_data", "_lipsync_write_metadata",
    "_lipsync_write_output", "_lipsync_write_lipsync_state", "_lipsync_chain",
    "_lipsync_get_maxpts", "_lipsync_get_delay_seamless_mode", "aud_module_set_prop",
}
EXTRA_RANGES = ((0x600A1391, 0x600A13F4),)
MARKER = re.compile(r"^/\* @([0-9a-f]+) ([^\n]+) \*/", re.MULTILINE)


def digest(filename):
    return hashlib.sha256(filename.read_bytes()).hexdigest()


def write_json(name, value):
    (OUTPUT / name).write_text(json.dumps(value, indent=2) + "\n")


def main():
    export = ROOT / "decomp/audio_a0_dsp2.main_60000000.bin.c"
    listing = ROOT / "evidence/dsp2-lipsync/dsp2-lipsync.asm"
    properties_file = ROOT / "evidence/dsp2-lipsync/dsp2-properties.json"
    image_file = ROOT / "build/fw/audio_a0_dsp2.main_60000000.bin"
    symbols_file = ROOT / "symbols/dsp2.syms.tsv"
    image = image_file.read_bytes()
    source = export.read_text()
    symbol_rows = [line.split("\t", 1) for line in symbols_file.read_text().splitlines()
                   if "\t" in line]
    symbols = {int(address, 16): name for address, name in symbol_rows
               if re.fullmatch(r"[0-9a-fA-F]{8}", address)}
    markers = list(MARKER.finditer(source))
    sections = []
    for index, match in enumerate(markers):
        address = int(match.group(1), 16)
        name = match.group(2)
        owner = name.split("__blk_", 1)[0]
        if owner not in TARGETS.values() and owner not in SUPPORT and not any(
            start <= address < end for start, end in EXTRA_RANGES):
            continue
        next_match = markers[index + 1] if index + 1 < len(markers) else None
        end_offset = next_match.start() if next_match else len(source)
        sections.append({
            "address": hex(address), "name": name,
            "end_exclusive": hex(int(next_match.group(1), 16)) if next_match else None,
            "source_line": source.count("\n", 0, match.start()) + 1,
            "text": source[match.start():end_offset],
            "synthetic_block": "__blk_" in name or name.startswith("FUN_"),
            "selection": "target" if owner in TARGETS.values() else "nearby support",
        })
    by_address = {int(section["address"], 16): section for section in sections}
    for address, name in TARGETS.items():
        assert by_address[address]["name"] == name, (hex(address), name)

    instructions = []
    all_instructions = {}
    for line_number, line in enumerate(listing.read_text().splitlines(), 1):
        fields = line.split("\t")
        if len(fields) < 3 or not re.fullmatch(r"[0-9a-f]{8}", fields[0]):
            continue
        address = int(fields[0], 16)
        row = {
            "address": hex(address), "bytes": fields[1], "instruction": fields[2],
            "annotations": fields[3:], "source_line": line_number,
        }
        all_instructions[address] = row
        raw = bytes.fromhex(row["bytes"])
        offset = address - 0x60000000
        assert image[offset:offset + len(raw)] == raw, hex(address)
        if any(int(section["address"], 16) <= address < int(section["end_exclusive"], 16)
             for section in sections if section["end_exclusive"]) or any(
                 start <= address < end for start, end in EXTRA_RANGES):
            instructions.append(row)

    anchors = {
        0x600A2652: "l8ui a8,a9,0x30",
        0x600A2686: "l32i.n a8,a4,0x30",
        0x600A2693: "s32i.n a8,a3,0x4",
        0x600A27C4: "l8ui a12,a9,0x31",
    }
    for address, expected in anchors.items():
        row = all_instructions[address]
        assert row["instruction"] == expected, row
        offset = address - 0x60000000
        assert image[offset:offset + len(bytes.fromhex(row["bytes"]))].hex() == row["bytes"]

    properties = json.loads(properties_file.read_text())
    for rows in properties.values():
        for row in rows:
            offset = int(row["address"], 16) - 0x60000000
            raw = struct.unpack_from("<10I", image, offset)
            assert list(raw) == [int(value, 16) for value in row["raw_words"]]
    clock = next(row for row in properties["clockType"] if row["name"] == "clockType")
    assert clock["offset"] == "0x200" and clock["setter"] == "0x0" and clock["type"] == 2

    def word(address):
        return struct.unpack_from("<I", image, address - 0x60000000)[0]

    def string(address):
        offset = address - 0x60000000
        if not 0 <= offset < len(image):
            return None
        end = image.find(b"\0", offset, offset + 512)
        if end < 0:
            return None
        raw = image[offset:end]
        if not raw or any(value not in (9, 10, 13) and not 32 <= value < 127 for value in raw):
            return None
        return raw.decode("ascii")

    state_names = [string(word(0x601062C0 + index * 4)) for index in range(14)]
    tables = []
    for table_name, address in (("state_dispatch", 0x60007380), ("action_dispatch", 0x600073D0)):
        rows = [{"index": index, "state_name": state_names[index],
                 "entry_address": hex(address + index * 4),
                 "target": hex(word(address + index * 4)),
                 "target_listing_present": word(address + index * 4) in all_instructions}
                for index in range(14)]
        tables.append({"name": table_name, "address": hex(address), "rows": rows,
                       "coverage": "14 entries selected from explicit action index bound; state guard is opaque FLIX"})
    assert state_names[3].strip() == "SKIP" and state_names[4] == "REPEAT", state_names
    assert all(row["target_listing_present"] for table in tables for row in table["rows"])
    write_json("dispatch-tables.json", tables)

    calls = []
    literals = []
    for row in instructions:
        if row["instruction"].split()[0].startswith("call"):
            target = re.search(r"0x([0-9a-f]+)", row["instruction"])
            calls.append({**row, "target": hex(int(target.group(1), 16)) if target else None,
                          "symbol": symbols.get(int(target.group(1), 16)) if target else None,
                          "confidence": "listing call mnemonic; unknown-byte regions may contain false decode"})
        if row["instruction"].startswith("l32r "):
            literal_address = int(re.search(r"0x([0-9a-f]+)", row["instruction"]).group(1), 16)
            value = word(literal_address)
            literals.append({"use_address": row["address"], "literal_address": hex(literal_address),
                             "value": hex(value), "string_if_printable": string(value),
                             "symbol_if_exact": symbols.get(value), "source_line": row["source_line"]})

    write_json("properties.json", properties)
    write_json("slices.json", sections)
    notice = "/* Saved Ghidra pseudocode, not original source. FLIX effects are opaque. */\n\n"
    (OUTPUT / "dsp2-focused.c").write_text(notice + "".join(section["text"] for section in sections))
    (OUTPUT / "dsp2-focused.asm").write_text("\n".join(
        "\t".join([row["address"][2:], row["bytes"], row["instruction"], *row["annotations"]])
        for row in instructions) + "\n")
    write_json("instructions.json", instructions)
    write_json("calls.json", calls)
    write_json("literals.json", literals)
    write_json("opaque-regions.json", {
        "warning": "hifi_lg generic FLIX userop has no implemented register/memory effects; never interpret as no-op",
        "flix": [row for row in instructions if row["instruction"].startswith("FLIX")],
        "unknown_code_units": [row for row in instructions if row["instruction"].startswith("??")],
    })
    selected_symbols = [line for line in symbols_file.read_text().splitlines()
                        if "lipsync" in line.lower() or any(f"{address:08x}" in line.lower()
                                                           for address in TARGETS)]
    write_json("symbols.json", selected_symbols)
    write_json("manifest.json", {
        "evidence_kind": "local saved artifacts; REA tools absent as previously diagnosed",
        "root": str(ROOT), "scope": str(OUTPUT),
        "inputs": [{"path": str(filename.relative_to(ROOT)), "sha256": digest(filename)}
                   for filename in (export, listing, properties_file, image_file, symbols_file)],
        "targets": {hex(address): name for address, name in TARGETS.items()},
        "slices": len(sections), "instructions": len(instructions),
        "checks": ["eight target entry markers present", "all saved listing bytes match image",
               "reset-copy anchors match saved listing and image", "all exported property words match image",
               "clockType +0x200, type 2, setter zero", "14 action/state table targets have saved listing entries"],
        "limitations": [
            "No TV access, runtime observation, production changes or database writes.",
            "Saved hifi_lg FLIX userops have no implemented register or memory effects.",
            "Synthetic block boundaries and decompiler arguments are not independent function evidence.",
            "A saved listing reference is not necessarily a direct call or a resolved indirect target.",
        ],
    })
    print(json.dumps({"validated": True, "slices": len(sections),
                      "instructions": len(instructions), "output": str(OUTPUT)}))


if __name__ == "__main__":
    main()