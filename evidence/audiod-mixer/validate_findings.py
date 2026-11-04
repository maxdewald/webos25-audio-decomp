import hashlib
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parent
BINARY = ROOT.parent.parent / "orig/usr/sbin/audiod"
binary = BINARY.read_bytes()
report = json.loads((ROOT / "findings.json").read_text())
checks = []

def require(condition, description):
    if not condition:
        raise AssertionError(description)
    checks.append(description)

require(hashlib.sha256(binary).hexdigest() == report["binary"]["sha256"], "binary SHA256 matches report")
require(binary[:6] == b"\x7fELF\x01\x01", "ELF32 little endian")
require(struct.unpack_from("<H", binary, 18)[0] == 40, "ELF machine is ARM")
literal_checks = {
    0x004a2aec: "setMixer",
    0x004a0b2c: "mix",
    0x004a19ac: "mixDigitalSoundOutput",
    0x0049ff08: "/tv",
    0x0049201c: "category",
    0x004a5090: "value",
    0x004925bc: "luna://com.webos.service.audiooutput/quality/setSettings",
}
for address, expected in literal_checks.items():
    actual = binary[address:binary.index(b"\0", address)].decode()
    require(actual == expected, f"literal at raw 0x{address:08x}: {expected}")
schema_address = 0x004a0a8c
schema = json.loads(binary[schema_address:binary.index(b"\0", schema_address)].decode())
require(schema == report["evidence"][2]["schema"], "request schema matches ELF literal")
require(struct.unpack_from("<II", binary, 0x00504294) == (0x004a19ac, 0x001c186d), "method table name and Thumb callback pointer match")
references = json.loads((ROOT / "call-refs.json").read_text())
sender_calls = [item for item in references if item["target_raw"] == "0x001dc608"]
require({item["site_raw"] for item in sender_calls} == {"0x0019566a", "0x00195910"}, "recovered direct mixer sender calls are both arbitration branches")
arbitration_calls = [item for item in references if item["target_raw"] == "0x00195480"]
require({item["site_raw"] for item in arbitration_calls} == {"0x001c19fa"}, "recovered direct arbitration caller is registered callback")
require(any(item["site_raw"] == "0x000c18c0" and item["plt_symbol"] == "LSCallOneReply" for item in references), "outbound call is LSCallOneReply")
require(any(item["site_raw"] == "0x001dc67c" and "hasKey" in (item["plt_symbol"] or "") for item in references), "sender tests mix key existence, not boolean truth")
strings = json.loads((ROOT / "string-refs.json").read_text())
require([item["site_raw"] for item in strings if item["text"] == "setMixer"] == ["0x001dc6ee"], "one recovered PC-relative setMixer constructor reference")
evidence_ids = {item["id"] for item in report["evidence"]}
require(len(evidence_ids) == len(report["evidence"]), "evidence IDs unique")
for item in report["claims"] + report["decision_table"]:
    require(item["status"] in {"observed", "inferred", "unknown"}, f"valid claim status: {item.get('id', item.get('action'))}")
    require(set(item["evidence_ids"]) <= evidence_ids, f"valid evidence references: {item.get('id', item.get('action'))}")
for evidence in report["evidence"]:
    require(all(int(ghidra, 16) == int(raw, 16) + 0x10000 for raw, ghidra in zip(evidence["raw_elf_addresses"], evidence["ghidra_addresses"])), f"address bias matches: {evidence['id']}")
for before, request, expected_after, expected_send in [
    (set(), True, {"A"}, True),
    ({"A"}, True, {"A"}, True),
    ({"A"}, False, set(), False),
    ({"A", "B"}, False, {"B"}, None),
    (set(), False, set(), False),
    ({"B"}, False, {"B"}, None),
]:
    holders = set(before)
    if request:
        holders.add("A")
        sent = True
    else:
        holders.discard("A")
        sent = False if not holders else None
    require(holders == expected_after and sent == expected_send, f"reconstructed policy case: before={sorted(before)}, request={request}")
configuration = ROOT.parent.parent / "orig/etc/palm/audiod"
configuration_hits = [file.name for file in configuration.glob("*.json") if '"setMixer"' in file.read_text() or '"mix"' in file.read_text()]
require(not configuration_hits, "acquired audiod configuration has no literal setMixer/mix mapping")
result = {"status":"PASS", "kind":"local static evidence validation, not TV/runtime validation", "binary_sha256":report["binary"]["sha256"], "check_count":len(checks), "checks":checks, "limitations":["decision cases test a reconstruction, not original ARM execution", "direct-call/string-index completeness does not cover arbitrary computed paths"]}
(ROOT / "validation.json").write_text(json.dumps(result, indent=2) + "\n")
print(f"PASS: {len(checks)} checks; SHA256, ELF literals, callback registration, call sites, evidence links and reconstructed arbitration cases")