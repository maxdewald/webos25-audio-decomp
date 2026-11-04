import bisect
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess

ROOT = Path(__file__).resolve().parent
BINARY = ROOT.parent.parent / "orig/usr/sbin/audiod"
LLVM = Path("/opt/homebrew/opt/llvm/bin")
data = BINARY.read_bytes()
section_offset = struct.unpack_from("<I", data, 32)[0]
section_size, section_count, names_index = struct.unpack_from("<HHH", data, 46)
sections = [struct.unpack_from("<10I", data, section_offset + index * section_size) for index in range(section_count)]
names_section = sections[names_index]
names_data = data[names_section[4]:names_section[4] + names_section[5]]
section_map = {names_data[item[0]:names_data.find(b"\0", item[0])].decode(): item for item in sections}
dynamic_symbols = section_map[".dynsym"]
dynamic_strings = section_map[".dynstr"]
plt_section = section_map[".plt"]
relocations = section_map[".rel.plt"]
plt_names = {}
for index in range(relocations[5] // relocations[9]):
    relocation = struct.unpack_from("<II", data, relocations[4] + index * relocations[9])
    symbol_index = relocation[1] >> 8
    symbol_name = struct.unpack_from("<I", data, dynamic_symbols[4] + symbol_index * dynamic_symbols[9])[0] + dynamic_strings[4]
    name = data[symbol_name:data.find(b"\0", symbol_name)].decode()
    plt_names[plt_section[3] + 20 + index * 12] = name
symbols = subprocess.check_output([LLVM / "llvm-nm", "-D", "-C", "--defined-only", BINARY], text=True)
functions = []
for line in symbols.splitlines():
    match = re.match(r"([0-9a-f]+) [TW] (.*)", line)
    if match:
        functions.append((int(match[1], 16) & ~1, match[2]))
functions = sorted(set(functions))
starts = [address for address, name in functions]
assembly = subprocess.check_output([LLVM / "llvm-objdump", "-d", "--no-show-raw-insn", "--triple=thumbv7-linux-gnueabi", "-j", ".text", BINARY], text=True)
(ROOT / "symbols.txt").write_text(symbols)
registers = {}
references = []
calls = []
focused_assembly = []
focused_owners = {0x000b6ec4, 0x000c15d8, 0x00195480, 0x001c186c, 0x001dc608}
for line in assembly.splitlines():
    match = re.match(r"\s*([0-9a-f]+):\s+(\S+)\s*(.*)", line)
    if not match:
        continue
    address, operation, operands = int(match[1], 16), match[2], match[3]
    owner_index = bisect.bisect_right(starts, address) - 1
    owner = functions[owner_index] if owner_index >= 0 else (0, "unknown")
    if owner[0] in focused_owners or 0x001b2650 <= address <= 0x001b2804 or 0x000832a0 <= address <= 0x000832ec:
        focused_assembly.append(line)
    load = re.match(r"(r\d+|lr|ip), \[pc, #(-?(?:0x)?[0-9a-f]+)\]", operands)
    if operation.startswith("ldr") and load:
        offset = ((address + 4) & ~3) + int(load[2], 0)
        if 0 <= offset < 0x4efe90:
            registers[load[1]] = struct.unpack_from("<I", data, offset)[0]
        continue
    addition = re.match(r"(r\d+|lr|ip), pc$", operands)
    if operation.startswith("add") and addition and addition[1] in registers:
        target = (registers.pop(addition[1]) + address + 4) & 0xffffffff
        if 0 <= target < 0x4efe94:
            end = data.find(b"\0", target, min(target + 4096, len(data)))
            text = data[target:end]
            if end > target and all(32 <= byte < 127 or byte in (9, 10, 13) for byte in text):
                references.append({"function_raw": f"0x{owner[0]:08x}", "function": owner[1], "site_raw": f"0x{address:08x}", "site_ghidra": f"0x{address + 0x10000:08x}", "string_raw": f"0x{target:08x}", "text": text.decode()})
    branch = re.match(r"(?:#)?0x([0-9a-f]+)", operands)
    if operation in ("bl", "blx", "b", "b.w") and branch:
        target = int(branch[1], 16)
        annotation = re.search(r"<([^>]+)@plt>", operands)
        calls.append({"owner_raw": f"0x{owner[0]:08x}", "owner": owner[1], "site_raw": f"0x{address:08x}", "target_raw": f"0x{target:08x}", "plt_symbol": annotation[1] if annotation else plt_names.get(target)})
(ROOT / "string-refs.json").write_text(json.dumps(references, indent=2) + "\n")
(ROOT / "call-refs.json").write_text(json.dumps(calls, indent=2) + "\n")
(ROOT / "audiod.asm").write_text("LOCAL STATIC EVIDENCE: llvm-objdump raw ELF addresses; focused mixer and registration slices only.\n" + "\n".join(focused_assembly) + "\n")
selected = [item for item in references if item["text"] in ("mix", "setMixer") or "mixDigitalSoundOutput" in item["function"]]
pointers = []
for target in (0x001dc609, 0x00195481, 0x001c186d, 0x004a19ac, 0x004a2aec, 0x004a0b2c):
    start = 0
    while (offset := data.find(struct.pack("<I", target), start)) >= 0:
        pointers.append({"target_raw": f"0x{target:08x}", "file_offset": f"0x{offset:08x}", "raw_va": f"0x{offset + (0x10000 if offset >= 0x4f06b0 else 0):08x}"})
        start = offset + 1
(ROOT / "data-pointers.json").write_text(json.dumps(pointers, indent=2) + "\n")
boundary_calls = [item for item in calls if item["owner_raw"] in ("0x001c186c", "0x001dc608", "0x000c15d8") and item["target_raw"] in ("0x00081f2c", "0x00081ad0", "0x00081b9c", "0x00081b6c", "0x0008183c", "0x000821c0")]
(ROOT / "boundary-calls.json").write_text(json.dumps(boundary_calls, indent=2) + "\n")
print(json.dumps({"sha256": hashlib.sha256(data).hexdigest(), "functions": len(functions), "string_references": len(references), "plt_entries": len(plt_names), "mixer_callers": [item for item in calls if "mixDigitalSoundOutput" in (item["plt_symbol"] or "")], "boundary_calls": boundary_calls, "data_pointers": pointers}, indent=2))