import hashlib
import json
from pathlib import Path
import struct
import subprocess


OUT = Path(__file__).resolve().parent
findings = json.loads((OUT / 'findings.json').read_text())
artifacts = json.loads((OUT / 'artifacts.json').read_text())
checks = json.loads((OUT / 'address-checks.json').read_text())
identity = json.loads((OUT / 'kernel-identity.json').read_text())
evidence_ids = {item['id'] for item in findings['evidence']}
assert len(evidence_ids) == len(findings['evidence']) == 15
assert evidence_ids == {f'LOCAL-RABI-{index:03d}' for index in range(1, 16)}
for section in ['creation_sequence', 'gate_table', 'claims']:
    for item in findings[section]:
        assert item['status'] in ['observed', 'inferred', 'unknown']
        assert set(item['evidence_ids']) <= evidence_ids
assert set(findings['ioctl']['evidence_ids']) <= evidence_ids
for item in findings['evidence']:
    for filename in item.get('files', []) + ([item['file']] if 'file' in item else []):
        assert (OUT / filename).is_file(), filename
    if 'raw_evidence' in item:
        assert (OUT / item['raw_evidence']).is_file()
for name, expected in findings['artifact_hashes'].items():
    artifact = artifacts[name]
    assert artifact['sha256'] == expected
    assert hashlib.sha256(Path(artifact['path']).read_bytes()).hexdigest() == expected
assert identity['exact_match'] and identity['matching_blocks'] == identity['block_count'] == 140
assert identity['decoded_sha256'] == findings['artifact_hashes']['kernel_image']
assert checks['notification_ioctl'] == {'value': '0x4008522b', 'direction': 1, 'size': 8, 'type': 82, 'number': 43}
assert checks['branch_witness']['result'] == 'default: ENOTTY'
assert checks['branch_witness']['default_offset'] == '0xc308e0'
image = Path(artifacts['kernel_image']['path']).read_bytes()
for offset, expected in {0xc30914: 0x92800314, 0xc30558: 0xaa1403e0,
                         0xc25394: 0x97fff140, 0xc21988: 0x39013260}.items():
    assert struct.unpack_from('<I', image, offset)[0] == expected
subprocess.run(['clang', '-std=c11', '-Wall', '-Wextra', '-Werror', '-fsyntax-only',
                str(OUT / 'abi_test.c')], check=True)
assert findings['next_runtime_experiment']['executed'] is False
assert findings['status'] == 'complete_with_limitations'
print('PASS: 15 stable evidence IDs, all claim links and files, five artifact hashes, 140 kernel blocks, raw branch/constructor instructions, C11 ABI assertions; no runtime experiment executed.')