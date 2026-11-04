import argparse
import csv
import hashlib
import json
import re
from datetime import datetime, timezone
from pathlib import Path


OUTPUT = Path(__file__).resolve().parent
ROOT = OUTPUT.parent.parent
BASE = 0x60000000
EXPECTED_ORIGINAL = "c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d"
INPUTS = {
    "raw": ROOT / "build/fw/audio_a0_dsp2.main_60000000.bin",
    "original": ROOT / "orig/lib/firmware/audio_a0_dsp2.bin",
    "symbols": ROOT / "symbols/dsp2.syms.tsv",
    "decompile": ROOT / "decomp/audio_a0_dsp2.main_60000000.bin.c",
    "functions": ROOT / "symbols/audio_a0_dsp2.main_60000000.bin.functions.tsv",
    "listing_script": ROOT / "tools/ListInsns.java",
}
SELECTED = {
    "bypass_chai", "bypass_event", "gain_control", "gain_resend_first_event_in_transition",
    "gain_chain", "gain_event", "delay_chai", "delay_event", "delay_queue_push_locked",
    "delay_push_output", "delay_queue_fadeout", "delay_queue_flush", "delay_set_delay",
    "add_pending_obj_locked", "add_pending_obj", "aud_port_push_data", "aud_module_push_data",
    "foreach_down_peer_push_data", "aud_port_push_event", "aud_module_default_event",
    "aud_module_event_locked", "_lipsync_event",
    "fader_chain", "fader_event", "fader_init", "fader_bypass_work", "_fader_push_data",
    "fader_disconnect_port", "aud_module_set_prop", "aud_module_get_prop",
    "aud_module_prop_str",
}


def digest(file_path):
    checksum = hashlib.sha256()
    with file_path.open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            checksum.update(chunk)
    return checksum.hexdigest()


def save_json(name, value):
    (OUTPUT / name).write_text(json.dumps(value, indent=2, ensure_ascii=True) + "\n")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--check-only", action="store_true")
    arguments = parser.parse_args()
    with INPUTS["symbols"].open() as source:
        symbols = [(int(row[0], 16), row[1]) for row in csv.reader(source, delimiter="\t") if row]
    assert symbols == sorted(symbols), "Symbol addresses must be sorted"
    candidates = [name for _, name in symbols if "queu" in name or "queue" in name]
    ranges = []
    for index, (start, name) in enumerate(symbols[:-1]):
        if name not in SELECTED and not re.match(r"^_?queue?_?(chai[n]?|event)$", name):
            continue
        stop = symbols[index + 1][0]
        ranges.append({"start": f"0x{start:08x}", "stop_exclusive": f"0x{stop:08x}",
                       "symbol": name, "stem": f"{start:08x}_{name}"})
    found = {item["symbol"] for item in ranges}
    assert {"bypass_chai", "bypass_event", "gain_chain", "gain_event", "delay_chai",
            "delay_event", "aud_module_push_data", "add_pending_obj_locked", "_lipsync_event"} <= found
    image = INPUTS["raw"].read_bytes()
    for item in ranges:
        assert BASE <= int(item["start"], 16) < int(item["stop_exclusive"], 16) <= BASE + len(image)
    original_sha = digest(INPUTS["original"])
    assert original_sha == EXPECTED_ORIGINAL, f"Unexpected original firmware SHA256: {original_sha}"
    print(json.dumps({"check": "selection-and-original-digest", "ranges": len(ranges),
                      "missing_optional_symbols": sorted(SELECTED - found),
                      "queue_candidates": candidates, "original_sha256": original_sha}, indent=2))
    if arguments.check_only:
        return
    decompile = INPUTS["decompile"].read_text()
    markers = list(re.finditer(r"^/\* @([0-9a-fA-F]+) ([^\n]+) \*/", decompile, re.MULTILINE))
    sections = []
    for index, marker in enumerate(markers):
        stop = markers[index + 1].start() if index + 1 < len(markers) else len(decompile)
        sections.append((int(marker[1], 16), marker[2], decompile[marker.start():stop]))
    for item in ranges:
        start, stop = int(item["start"], 16), int(item["stop_exclusive"], 16)
        selected = [(address, name, body) for address, name, body in sections if start <= address < stop]
        assert selected, f"No decompile sections for {item['symbol']}"
        header = ("/* LOCAL FALLBACK: existing Ghidra pseudocode, not original source.\n"
                  " * Opaque FLIX semantics and missing control flow remain unknown.\n"
                  f" * Symbol interval [{start:08x}, {stop:08x}); names require body corroboration. */\n\n")
        (OUTPUT / (item["stem"] + ".c")).write_text(header + "".join(body for _, _, body in selected))
        (OUTPUT / (item["stem"] + ".bin")).write_bytes(image[start - BASE:stop - BASE])
        item["c_sections"] = [{"entry": f"0x{address:08x}", "name": name} for address, name, _ in selected]
        item["c_flix_calls"] = sum(len(re.findall(r"\bflix\(", body)) for _, _, body in selected)
        item["c_warning_lines"] = sum(len(re.findall(r"WARNING:", body)) for _, _, body in selected)
    with (OUTPUT / "selected-ranges.tsv").open("w") as target:
        for item in ranges:
            target.write(f"{item['start'][2:]}\t{item['stop_exclusive'][2:]}\t{item['stem']}\n")
    string_start, string_stop = 0x60043254, 0x60043320
    string_bytes = image[string_start - BASE:string_stop - BASE]
    (OUTPUT / "60043254_60043320_strings.bin").write_bytes(string_bytes)
    string_records = []
    position = 0
    while position < len(string_bytes):
        end = string_bytes.find(b"\0", position)
        terminated = end >= 0
        if not terminated:
            end = len(string_bytes)
        value = string_bytes[position:end]
        if value:
            string_records.append({"address": f"0x{string_start + position:08x}",
                                   "text": value.decode("ascii", errors="backslashreplace"),
                                   "terminated_in_range": terminated})
        position = end + 1
    slot = 0x600A083C
    slot_bytes = image[slot - BASE:slot - BASE + 4]
    pointee = int.from_bytes(slot_bytes, "little")
    literal = {"slot": f"0x{slot:08x}", "raw_hex": slot_bytes.hex(),
               "u32_le": f"0x{pointee:08x}", "source": str(INPUTS["raw"].relative_to(ROOT))}
    if BASE <= pointee < BASE + len(image):
        offset = pointee - BASE
        end = image.find(b"\0", offset, offset + 512)
        literal["target_terminated"] = end >= 0
        literal["target_raw_hex"] = image[offset:end + 1 if end >= 0 else offset + 64].hex()
        if end >= 0:
            literal["target_ascii"] = image[offset:end].decode("ascii", errors="backslashreplace")
    save_json("raw-literals.json", {"pointer_literal": literal, "pre_first_strings": string_records})
    save_json("selection.json", {"ranges": ranges, "queue_candidates": candidates})
    save_json("provenance.json", {
        "created_utc": datetime.now(timezone.utc).isoformat(),
        "mode": "LOCAL_FALLBACK_EXISTING_GHIDRA_DB", "rea": "MCP tools unavailable; already diagnosed by user; no configuration repair attempted",
        "image_base": f"0x{BASE:08x}", "language": "Xtensa:LE:32:hifi_lg",
        "inputs": {name: {"path": str(file_path.relative_to(ROOT)), "sha256": digest(file_path),
                          "size": file_path.stat().st_size} for name, file_path in INPUTS.items()},
        "scope": "New artifacts only in evidence/earc-dsp2; no TV, production, source DB, global config or git mutations",
        "decompile_source": "Existing export, whole symbol intervals including recovered __blk functions; not fresh complete decompilation",
        "limitations": ["All E/F eight-byte FLIX bundles are opaque; branches and register mutations inside remain unknown",
                        "Symbol table names may be truncated or assigned to adjacent bodies; corroborate log tags and assembly",
                        "No runtime observation; data caching, replay and ES conversion cannot be inferred across unknown control flow"],
    })
    print(f"EXTRACTED ranges={len(ranges)} c_sections={sum(len(item['c_sections']) for item in ranges)} pointer={literal}")


if __name__ == "__main__":
    main()