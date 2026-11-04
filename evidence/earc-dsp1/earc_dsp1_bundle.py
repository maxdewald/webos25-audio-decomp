import argparse
import hashlib
import json
import os
import re
import struct
import subprocess
from pathlib import Path


OUTPUT = Path(__file__).resolve().parent
ROOT = OUTPUT.parent.parent
BASE = 0x60000000
EXPECTED_CONTAINER_SHA = "8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4"
SELECT = re.compile(
    r"^(queue_buf_|HAL_(SetArc|SetSpdif|SetIEC|GetCurrentStch)|"
    r"write_(2ch|4ch|es)$|out_|irq_handle|fill_bc$|process_data_|"
    r"mute_output_|start_(arc|spdif)$|stop_(arc|spdif)$|"
    r"cpy_|outctrlbin_|gain_|delay_|Pcmcvt_|pcmcvt_|set_prop_|"
    r"_?LGRFT|RFT_|Glue_|.*[Gg]luep[wr]|CommonBuffer_|"
    r"InterBuffer_|SharedBuffer_|ashbuf_|glue_|glue[prw]|"
    r"aud_data_|aud_module_(push_data|sync)|srcbin_|lgsrc_(process|get|set))")


def digest(filename):
    return hashlib.sha256(filename.read_bytes()).hexdigest()


def save_json(filename, value):
    filename.write_text(json.dumps(value, indent=2, ensure_ascii=True) + "\n")


def symbols():
    entries = []
    for line in (ROOT / "symbols/dsp1.syms.tsv").read_text().splitlines():
        address, name = line.split("\t", 1)
        entries.append((int(address, 16), name))
    return sorted(entries)


def prepare():
    OUTPUT.mkdir(parents=True, exist_ok=True)
    inputs = [
        ROOT / "build/fw/audio_a0_dsp1.main_60000000.bin",
        ROOT / "orig/lib/firmware/audio_a0_dsp1.bin",
        ROOT / "symbols/dsp1.syms.tsv",
        ROOT / "decomp/audio_a0_dsp1.main_60000000.bin.c",
        ROOT / "tools/ListInsns.java",
    ]
    identities = [
        {"path": str(filename), "size": filename.stat().st_size, "sha256": digest(filename)}
        for filename in inputs
    ]
    if identities[1]["sha256"] != EXPECTED_CONTAINER_SHA:
        raise ValueError("DSP1 container hash does not match the authorized input")
    entries = symbols()
    selected = []
    for index, (start, name) in enumerate(entries[:-1]):
        if SELECT.search(name):
            stop = entries[index + 1][0]
            selected.append((start, stop, name))
    event = next(entry for entry in selected if entry[0] == 0x60123814)
    assert event[1] == 0x60123EE4, event
    (OUTPUT / "ranges.tsv").write_text("".join(
        f"{start:08x}\t{stop:08x}\t{name}\n" for start, stop, name in selected
    ))
    text = inputs[3].read_text()
    headers = list(re.finditer(r"^/\* @([0-9a-fA-F]+) ([^\n]+) \*/$", text, re.M))
    with (OUTPUT / "existing-selected.c").open("w") as destination:
        destination.write("/* Existing saved export; synthetic boundaries and opaque FLIX remain limitations. */\n")
        for index, header in enumerate(headers):
            address = int(header.group(1), 16)
            if any(start <= address < stop for start, stop, name in selected):
                end = headers[index + 1].start() if index + 1 < len(headers) else len(text)
                destination.write(text[header.start():end])
    save_json(OUTPUT / "inputs.json", {
        "evidence_provider": "local fallback; REA tools absent, already diagnosed by parent",
        "linked_base": hex(BASE),
        "inputs": identities,
        "expected_container_sha256": EXPECTED_CONTAINER_SHA,
        "ranges_are": "Symbol-adjacent half-open ranges, not proven function boundaries",
        "selected_ranges": len(selected),
        "selected_bytes": sum(stop - start for start, stop, name in selected),
        "constraints": ["offline only", "no saved program DB mutation", "no language changes"],
    })
    print(json.dumps({"prepare": "passed", "ranges": len(selected), "out_event_end": hex(event[1])}))


def export():
    for directory in ["home", "tmp", "settings", "cache"]:
        (OUTPUT / directory).mkdir(exist_ok=True)
    project = ROOT / "build/ghidra/audio_a0_dsp1.main_60000000.bin"
    before = {str(filename.relative_to(project)): digest(filename)
              for filename in project.rglob("*") if filename.is_file() and not filename.name.endswith(".lock")}
    save_json(OUTPUT / "database-before.json", before)
    environment = dict(os.environ)
    environment.update({
        "JAVA_HOME": "/opt/homebrew/opt/openjdk@21",
        "JAVA_TOOL_OPTIONS": f"-Duser.home={OUTPUT / 'home'} -Djava.io.tmpdir={OUTPUT / 'tmp'}",
        "GHIDRA_HEADLESS_JAVA_OPTIONS": f"-Dapplication.settingsdir={OUTPUT / 'settings'} -Dapplication.cachedir={OUTPUT / 'cache'}",
        "XDG_CONFIG_HOME": str(OUTPUT / "home/.config"),
        "XDG_CACHE_HOME": str(OUTPUT / "cache"),
    })
    command = [
        "/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless", str(project),
        "audio_a0_dsp1.main_60000000.bin", "-process", "audio_a0_dsp1.main_60000000.bin",
        "-readOnly", "-noanalysis", "-scriptPath", str(OUTPUT),
        "-postScript", "EarcDsp1Export.java", str(OUTPUT),
        "-log", str(OUTPUT / "ghidra.log"), "-scriptlog", str(OUTPUT / "script.log"), "-max-cpu", "2",
    ]
    save_json(OUTPUT / "invocation.json", {"command": command, "scoped_environment": {
        key: environment[key] for key in ["JAVA_HOME", "JAVA_TOOL_OPTIONS", "GHIDRA_HEADLESS_JAVA_OPTIONS", "XDG_CONFIG_HOME", "XDG_CACHE_HOME"]}})
    completed = subprocess.run(command, env=environment, stdin=subprocess.DEVNULL, timeout=900, check=False)
    after = {str(filename.relative_to(project)): digest(filename)
             for filename in project.rglob("*") if filename.is_file() and not filename.name.endswith(".lock")}
    unchanged = before == after
    save_json(OUTPUT / "database-after.json", after)
    save_json(OUTPUT / "export-check.json", {"process_exit_code": completed.returncode, "database_unchanged": unchanged})
    if not unchanged or completed.returncode or not (OUTPUT / "program.json").exists():
        raise RuntimeError("Export failed or source project changed; inspect bundle logs")
    print(json.dumps({"export": "passed", "database_unchanged": unchanged}))


def static_export():
    import capstone

    prepare()
    image = (ROOT / "build/fw/audio_a0_dsp1.main_60000000.bin").read_bytes()
    decoder = capstone.Cs(capstone.CS_ARCH_XTENSA, capstone.CS_MODE_LITTLE_ENDIAN)
    entries = symbols()
    symbol_map = dict(entries)
    ranges = []
    for line in (OUTPUT / "ranges.tsv").read_text().splitlines():
        start, stop, name = line.split("\t")
        ranges.append((int(start, 16), int(stop, 16), name))
    text = (ROOT / "decomp/audio_a0_dsp1.main_60000000.bin.c").read_text()
    headers = list(re.finditer(r"^/\* @([0-9a-fA-F]+) ([^\n]+) \*/$", text, re.M))
    bodies = []
    for index, header in enumerate(headers):
        end = headers[index + 1].start() if index + 1 < len(headers) else len(text)
        bodies.append({"entry": int(header.group(1), 16), "name": header.group(2),
                       "start_offset": header.start(), "end_offset": end,
                       "source_line": text.count("\n", 0, header.start()) + 1})
    literals = []
    references = []
    coverage = []
    contextual = set()
    instructions = []

    def string_at(address):
        offset = address - BASE
        if not 0 <= offset < len(image):
            return None
        end = image.find(b"\0", offset, min(offset + 512, len(image)))
        if end < 0 or end - offset < 2:
            return None
        value = image[offset:end]
        if all(32 <= character < 127 or character in (9, 10, 13) for character in value):
            return value.decode("ascii")
        return None

    def literal_at(site, slot, name, evidence):
        offset = slot - BASE
        if not 0 <= offset <= len(image) - 4:
            return None
        value = struct.unpack_from("<I", image, offset)[0]
        literal = {"site": f"{site:08x}", "slot": f"{slot:08x}", "value": f"{value:08x}",
                   "range": name, "evidence": evidence}
        content = string_at(value)
        if content is not None:
            literal["text"] = content
        if value in symbol_map:
            literal["symbol_at_value"] = symbol_map[value]
            contextual.add(value)
        literals.append(literal)
        return literal

    def listing(start, stop, name, context=False):
        directory = OUTPUT / ("context" if context else "ranges")
        directory.mkdir(exist_ok=True)
        filename = directory / f"{start:08x}_{name}.asm"
        position = start
        opaque = 0
        unknown = 0
        count = 0
        with filename.open("w") as destination:
            destination.write(f"RANGE\t{start:08x}\t{stop:08x}\t{name}\tend_exclusive\n")
            destination.write("PROVIDER\tlocal Capstone scalar decode; E/F opaque8 per saved language; no FLIX semantics\n")
            while position < stop:
                offset = position - BASE
                first = image[offset]
                low = first & 15
                length = 8 if low >= 14 else 2 if low >= 8 else 3
                length = min(length, stop - position)
                raw = image[offset:offset + length]
                record = {"address": f"{position:08x}", "bytes": raw.hex(), "range": name,
                          "context": context, "length": length}
                if low >= 14:
                    record.update(mnemonic="opaque8", operands="", semantics="unknown")
                    opaque += 1
                else:
                    decoded = next(decoder.disasm(raw, position, count=1), None)
                    if decoded is not None and decoded.size == length:
                        record.update(mnemonic=decoded.mnemonic, operands=decoded.op_str, semantics="scalar")
                    else:
                        record.update(mnemonic=".byte", operands="undecoded scalar/padding", semantics="unknown")
                        unknown += 1
                if low == 1 and length == 3:
                    immediate = struct.unpack_from("<H", raw, 1)[0]
                    slot = (((position + 3) & ~3) + ((immediate | 0xFFFF0000) << 2)) & 0xFFFFFFFF
                    record["l32r_slot"] = f"{slot:08x}"
                    literal = literal_at(position, slot, name, "raw scalar L32R encoding")
                    if literal is not None:
                        record["literal"] = literal
                if record["mnemonic"].startswith(("call", "j", "b")):
                    reference = {"site": f"{position:08x}", "mnemonic": record["mnemonic"],
                                 "operands": record["operands"], "range": name,
                                 "scope": "scalar instruction only; opaque paths missing"}
                    matches = re.findall(r"0x([0-9a-fA-F]+)", record["operands"])
                    if matches:
                        target = int(matches[-1], 16)
                        reference["target"] = f"{target:08x}"
                        reference["symbol_at_target"] = symbol_map.get(target)
                        if record["mnemonic"].startswith("call") and target in symbol_map:
                            contextual.add(target)
                    references.append(reference)
                instructions.append(record)
                destination.write(f"{position:08x}\t{raw.hex()}\t{record['mnemonic']} {record['operands']}")
                if "literal" in record:
                    destination.write("\t" + json.dumps(record["literal"], ensure_ascii=True))
                destination.write("\n")
                count += 1
                position += length
        coverage.append({"name": name, "start": f"{start:08x}", "end_exclusive": f"{stop:08x}",
                         "span_bytes": stop - start, "listed_bytes": position - start,
                         "instruction_count": count, "opaque_instruction_count": opaque,
                         "unknown_scalar_count": unknown, "context": context,
                         "boundary_status": "linear assumed E/F=8, not validated across opaque instructions"})

    for start, stop, name in ranges:
        listing(start, stop, name)
    selected_bodies = [body for body in bodies if any(start <= body["entry"] < stop for start, stop, name in ranges)]
    for body in selected_bodies:
        content = text[body["start_offset"]:body["end_offset"]]
        for slot_text in set(re.findall(r"\b(?:PTR|DAT|SUB|_DAT)_[A-Za-z0-9_]*?([0-9a-f]{8})(?:\b|_)", content)):
            literal_at(body["entry"], int(slot_text, 16), body["name"], "saved C token; site is body entry, not instruction site")
    chosen_starts = {start for start, stop, name in ranges}
    context_starts = contextual - chosen_starts
    context_ranges = [(start, entries[index + 1][0], name)
                      for index, (start, name) in enumerate(entries[:-1]) if start in context_starts]
    for start, stop, name in context_ranges:
        if stop - start < 0x2000:
            listing(start, stop, name, context=True)
    selected_bodies.extend(body for body in bodies if body not in selected_bodies and
                           any(start <= body["entry"] < stop for start, stop, name in context_ranges if stop - start < 0x2000))
    selected_bodies.sort(key=lambda body: body["entry"])
    with (OUTPUT / "focused.c").open("w") as destination:
        destination.write("/* LOCAL FALLBACK: extracted existing saved C, NOT a fresh Ghidra run. */\n")
        destination.write("/* Synthetic boundaries, wrong pointer labels, opaque FLIX, and incomplete CFG remain. */\n")
        for body in selected_bodies:
            body["export_line"] = 1
            destination.write(text[body["start_offset"]:body["end_offset"]])
    function_records = [{"entry": f"{body['entry']:08x}", "saved_name": body["name"],
                         "synthetic": "__blk_" in body["name"], "source_line": body["source_line"],
                         "source": str(ROOT / "decomp/audio_a0_dsp1.main_60000000.bin.c")}
                        for body in selected_bodies]
    pairing = []
    for start, stop, name in ranges:
        strings = [literal for literal in literals if start <= int(literal["site"], 16) < stop and "text" in literal]
        candidates = [literal for literal in strings if re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", literal["text"])]
        pairing.append({"symbol_address": f"{start:08x}", "symbol_name": name,
                        "func_literal_candidates": candidates,
                        "exact_spelling_corroborated": any(literal["text"] == name for literal in candidates),
                        "warning": "Physical reference candidates only; do not treat symbol table or global string presence as __func__ proof"})
    save_json(OUTPUT / "functions.json", function_records)
    save_json(OUTPUT / "literals.json", literals)
    save_json(OUTPUT / "references.json", references)
    save_json(OUTPUT / "instructions.json", instructions)
    save_json(OUTPUT / "coverage.json", coverage)
    save_json(OUTPUT / "symbol-pairing.json", pairing)
    save_json(OUTPUT / "program.json", {
        "provider": "local fallback, not REA Evidence", "capstone_version": capstone.__version__,
        "fresh_decompile": False, "saved_language": "Xtensa:LE:32:hifi_lg",
        "ghidra_blocker": "Installed Ghidra 12.2 requires JDK25; explicitly requested JAVA_HOME21 rejected before DB open",
        "listed_ranges": len(coverage), "saved_c_bodies": len(function_records),
        "semantics_limit": "All E/F instructions emitted as opaque8. No FLIX branch or dataflow claim is established.",
    })
    print(json.dumps({"static_export": "passed", "ranges": len(coverage), "saved_c_bodies": len(function_records),
                      "opaque": sum(item["opaque_instruction_count"] for item in coverage)}))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("phase", choices=["prepare", "export", "static"])
    arguments = parser.parse_args()
    if arguments.phase == "prepare":
        prepare()
    elif arguments.phase == "export":
        export()
    elif arguments.phase == "static":
        static_export()


if __name__ == "__main__":
    main()