import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import subprocess


ROOT = Path(__file__).resolve().parents[2]
OWNED = ROOT / 'evidence/earc-arm'
ELF = ROOT / 'orig/usr/lib/gstreamer-1.0/libgstlgaudiosink.so'
DECOMP = ROOT / 'decomp/libgstlgaudiosink.so.c'
INDEX = ROOT / 'symbols/libgstlgaudiosink.so.functions.tsv'
LLVM = Path('/opt/homebrew/opt/llvm/bin')
BASE = 0x10000
SELECTED = [
    'gst_lg_audio_render_event', 'lgao_open_renderer', '_lgao_open_renderer',
    '_lgao_connect_renderer', 'lgao_connect_renderer', '_lgao_config_set_renderer_param',
    'lgao_config', 'lgao_set_audio_info', 'lgao_set_clocktype', 'lgao_push_first_event',
    '_sig_handle', 'gst_lg_audio_render_callback_task', 'lgao_open', 'lgao_play',
    'lgao_feed', 'lgao_get_event_mq_id',
]


def digest(file):
    return hashlib.sha256(file.read_bytes()).hexdigest()


def save_json(name, value):
    (OWNED / name).write_text(json.dumps(value, indent=2) + '\n', encoding='ascii')


def tool(name, *arguments):
    command = [str(LLVM / name), *map(str, arguments), str(ELF)]
    result = subprocess.run(command, check=True, capture_output=True, text=True)
    return result.stdout, command


def elf_metadata(data):
    assert data[:7] == b'\x7fELF\x01\x01\x01'
    header = struct.unpack_from('<16sHHIIIIIHHHHHH', data)
    sections = []
    for index in range(header[12]):
        section = struct.unpack_from('<IIIIIIIIII', data, header[6] + index * header[11])
        sections.append(dict(zip(['name_offset', 'type', 'flags', 'va', 'offset', 'size',
                                  'link', 'info', 'align', 'entsize'], section)))
    names = sections[header[13]]
    strings = data[names['offset']:names['offset'] + names['size']]
    for section in sections:
        start = section['name_offset']
        section['name'] = strings[start:strings.index(b'\0', start)].decode('ascii')
    segments = []
    for index in range(header[10]):
        values = struct.unpack_from('<IIIIIIII', data, header[5] + index * header[9])
        if values[0] == 1:
            segments.append(dict(zip(['type', 'offset', 'va', 'pa', 'filesz', 'memsz', 'flags', 'align'], values)))
    symbols = []
    for section in sections:
        if section['type'] != 2:
            continue
        table = sections[section['link']]
        strings = data[table['offset']:table['offset'] + table['size']]
        for offset in range(section['offset'], section['offset'] + section['size'], section['entsize']):
            name, value, size, info, other, section_index = struct.unpack_from('<IIIBBH', data, offset)
            if name:
                symbols.append({'name': strings[name:strings.index(b'\0', name)].decode('ascii'),
                                'value': value, 'size': size, 'type': info & 15, 'section': section_index})
    return sections, segments, symbols


def parse_dwarf(text):
    records = {}
    current = None
    for line in text.splitlines():
        match = re.match(r'0x([0-9a-f]+):\s+(DW_TAG_\w+)', line)
        if match:
            current = {'offset': int(match[1], 16), 'tag': match[2], 'lines': [line], 'attributes': {}}
            records[current['offset']] = current
        elif re.match(r'0x[0-9a-f]+:', line):
            current = None
        elif current is not None:
            current['lines'].append(line)
            attribute = re.search(r'(DW_AT_\w+)\s+(.*)', line)
            if attribute:
                current['attributes'][attribute[1]] = attribute[2]
    for record in records.values():
        name = record['attributes'].get('DW_AT_name', '')
        quoted = re.findall(r'"([^"]+)"', name)
        record['name'] = quoted[-1] if quoted else None
        origin = record['attributes'].get('DW_AT_abstract_origin', '')
        match = re.search(r'\{0x([0-9a-f]+)\}', origin)
        record['abstract_origin'] = int(match[1], 16) if match else None
    return records


def build():
    data = ELF.read_bytes()
    sections, segments, symbols = elf_metadata(data)
    original = DECOMP.read_text()
    expected = re.search(r'sha256=([a-f0-9]{64})', original)[1]
    assert digest(ELF) == expected, 'Cached decompile and ELF digest differ'
    chunks = list(re.finditer(r'^/\* @([0-9a-f]+) ([^\n]+) \*/\n', original, re.M))
    recovered = {}
    for index, match in enumerate(chunks):
        end = chunks[index + 1].start() if index + 1 < len(chunks) else len(original)
        recovered[match[2]] = (int(match[1], 16), original[match.start():end], original.count('\n', 0, match.start()) + 1)
    index_rows = {}
    for line in INDEX.read_text().splitlines():
        entry, size, name = line.split('\t')
        index_rows[name] = {'entry': int(entry, 16), 'ghidra_body_byte_count_not_span': int(size)}
    disassembly, dis_command = tool('llvm-objdump', '-d', '--triple=thumbv7-linux-gnueabi', '-j', '.text')
    dwarf, dwarf_command = tool('llvm-dwarfdump', '--debug-info', '--verbose')
    records = parse_dwarf(dwarf)
    requested_dwarf = set(SELECTED + ['gst_lg_audio_render_set_caps', '_lgao_set_msgpid_to_signal',
        'aren_pcm_info_t', 'aren_msgpid_to_signal_t', '_LGAO_AUDIO_START_INFO_T', 'lgao',
        'LGAO_CLOCK_TYPE_T', 'lgao_mq_item', 'LGAO_EVENT_UNDERFLOW'])
    selected_records = {offset for offset, record in records.items() if record['name'] in requested_dwarf}
    for offset, record in records.items():
        if record['abstract_origin'] in selected_records:
            selected_records.add(offset)
    offsets = sorted(records)
    focused_dwarf = []
    for index, offset in enumerate(offsets):
        if offset not in selected_records:
            continue
        record = records[offset]
        focused_dwarf.extend(record['lines'])
        for following in offsets[index + 1:]:
            child = records[following]
            first_indent = len(record['lines'][0].split(':', 1)[1]) - len(record['lines'][0].split(':', 1)[1].lstrip())
            next_indent = len(child['lines'][0].split(':', 1)[1]) - len(child['lines'][0].split(':', 1)[1].lstrip())
            if next_indent <= first_indent:
                break
            focused_dwarf.extend(child['lines'])
    (OWNED / 'sink-focused.dwarf.txt').write_text('\n'.join(focused_dwarf) + '\n')
    symbol_by_name = {symbol['name']: symbol for symbol in symbols if symbol['type'] == 2}
    mapping_symbols = sorted((symbol['value'], symbol['name'][1]) for symbol in symbols
                             if symbol['name'].startswith(('$t', '$a', '$d')))
    listing = []
    for line in disassembly.splitlines():
        match = re.match(r'\s*([0-9a-f]+):\s+((?:(?:[0-9a-f]{4}|[0-9a-f]{2})\s+)+)(.*)', line)
        if match:
            raw = b''.join(bytes.fromhex(token)[::-1] if len(token) == 4 else bytes.fromhex(token)
                           for token in match[2].split())
            listing.append((int(match[1], 16), raw, match[3], line))

    def file_offset(va):
        for segment in segments:
            if segment['va'] <= va < segment['va'] + segment['filesz']:
                return va - segment['va'] + segment['offset']
        return None

    def string_at(va):
        offset = file_offset(va)
        if offset is None:
            return None
        end = data.find(b'\0', offset, offset + 512)
        raw = data[offset:end] if end > offset else b''
        if len(raw) >= 2 and all(32 <= value <= 126 or value in (9, 10) for value in raw):
            return raw.decode('ascii')
        return None

    functions = []
    literals = {}
    byte_checks = 0
    assembly_lines = ['function\telf_va\tghidra_address\tfile_offset\tbytes\tarm_or_data\tannotations']
    code = ['/* Uncertain LOCAL Ghidra 12.2 cached pseudocode, not original source. No target execution. */',
            '/* ELF SHA256 ' + digest(ELF) + '. Ghidra addresses = ELF VA + 0x10000; Thumb bit cleared. */',
            '/* set_caps and _lgao_set_msgpid_to_signal are inline: full containing functions retained. */']
    for name in SELECTED:
        ghidra_entry, body, source_line = recovered[name]
        symbol = symbol_by_name[name]
        start = symbol['value'] & ~1
        end = start + symbol['size']
        assert start + BASE == ghidra_entry == index_rows[name]['entry']
        code.append(body)
        instructions = []
        for va, raw, instruction, original_line in listing:
            if not start <= va < end:
                continue
            offset = file_offset(va)
            assert offset is not None and data[offset:offset + len(raw)] == raw, original_line
            byte_checks += 1
            annotations = []
            ldr = re.search(r'\[pc, #(0x[0-9a-f]+|[0-9]+)\]', instruction)
            if instruction.startswith('ldr') and ldr:
                target = ((va + 4) & ~3) + int(ldr[1], 0)
                target_offset = file_offset(target)
                if target_offset is not None:
                    word = struct.unpack_from('<I', data, target_offset)[0]
                    key = f'0x{target + BASE:08x}'
                    literal = literals.setdefault(key, {'ghidra_address': key, 'elf_va': hex(target),
                        'file_offset': hex(target_offset), 'raw_le': data[target_offset:target_offset + 4].hex(),
                        'u32': f'0x{word:08x}', 'instruction_users': []})
                    literal['instruction_users'].append(hex(va + BASE))
                    annotations.append('literal=' + key + ' value=' + literal['u32'])
                    literal_string = string_at(word)
                    if literal_string:
                        literal['absolute_string_candidate'] = literal_string
            add = re.match(r'add\s+(r[0-9]+), pc\s*$', instruction)
            if add:
                register = add[1]
                for old_va, old_raw, old_instruction, old_line in reversed(instructions[-20:]):
                    load = re.match(r'ldr(?:\.w)?\s+' + register + r', \[pc, #(0x[0-9a-f]+|[0-9]+)\]', old_instruction)
                    if load:
                        pool = ((old_va + 4) & ~3) + int(load[1], 0)
                        pool_offset = file_offset(pool)
                        if pool_offset is not None:
                            relative = struct.unpack_from('<I', data, pool_offset)[0]
                            target = (relative + va + 4) & 0xffffffff
                            text = string_at(target)
                            if text:
                                annotations.append('pc_relative_string_candidate=' + json.dumps(text) + ' at=' + hex(target + BASE))
                        break
                    if re.match(r'\w+(?:\.w)?\s+' + register + r',', old_instruction):
                        break
            call = re.search(r'\bblx?\s+(0x[0-9a-f]+)', instruction)
            if call:
                annotations.append('call_ghidra=' + hex(int(call[1], 16) + BASE))
            assembly_lines.append('\t'.join([name, hex(va), hex(va + BASE), hex(offset), raw.hex(), instruction, '; '.join(annotations)]))
            instructions.append((va, raw, instruction, original_line))
        inline_records = []
        for offset in sorted(selected_records):
            record = records[offset]
            if record['tag'] == 'DW_TAG_inlined_subroutine':
                inline_records.append({'die': hex(offset), 'origin': hex(record['abstract_origin']),
                                       'attributes': record['attributes']})
        functions.append({'name': name, 'ghidra_entry': hex(ghidra_entry), 'elf_symbol_value': hex(symbol['value']),
            'elf_entry_thumb_bit_cleared': hex(start), 'elf_symbol_size_including_literal_pools': symbol['size'],
            'elf_symbol_range_exclusive': [hex(start), hex(end)],
            **index_rows[name], 'pseudocode_source': str(DECOMP), 'pseudocode_source_line': source_line,
            'pseudocode_sha256': hashlib.sha256(body.encode()).hexdigest(),
            'assembly_rows_verified_against_original_bytes': len(instructions),
            'mapping_symbols': [{'elf_va': hex(va), 'kind': kind} for va, kind in mapping_symbols if start <= va < end]})
    (OWNED / 'sink-focused.c').write_text('\n'.join(code))
    (OWNED / 'sink-focused.arm.tsv').write_text('\n'.join(assembly_lines) + '\n')
    save_json('sink-focused.literals.json', literals)
    save_json('sink-focused.coverage.json', {'evidence_origin': 'local cached Ghidra export + original ELF LLVM disassembly/DWARF',
        'not_rea_mcp_evidence': True, 'target_executed': False, 'functions': functions,
        'functions_completed': len(functions), 'function_lookup_failures': 0, 'verified_listing_rows': byte_checks,
        'inline_dies': inline_records, 'set_caps_abstract_and_inline_dies': [records[offset] for offset in sorted(selected_records)
            if records[offset]['name'] == 'gst_lg_audio_render_set_caps' or records[offset]['abstract_origin'] in
            {item for item, record in records.items() if record['name'] == 'gst_lg_audio_render_set_caps'}],
        'limits': ['Cached function byte count is not a contiguous body span; assembly uses ELF symbol bounds and ARM mapping symbols.',
                   'Inline functions have no fabricated callable entry; their full containing decompilation is retained.',
                   'PC-relative string annotations are candidates from bounded local register tracking, not proven dataflow.',
                   'No fresh Ghidra decompilation was possible with authorized Java 21; cached digest verified instead.',
                   'No kernel implementation, DSP, physical clock or sample counter behavior established by this sink-only bundle.']})
    save_json('sink-focused.provenance.json', {'schema_version': 1, 'evidence_origin': 'local, not REA',
        'representation': 'uncertain decompilation and static ARM/DWARF observations, not original source',
        'target_executed': False, 'binary': {'path': str(ELF), 'sha256': digest(ELF), 'size': len(data),
            'architecture': 'ELF32 little-endian ARM EABI5, Thumb-2 functions', 'ghidra_image_base': hex(BASE),
            'runtime_load_base': None, 'pt_load': segments},
        'cached_decompile': {'path': str(DECOMP), 'sha256': digest(DECOMP), 'declared_binary_sha256': expected,
            'ghidra_version': '12.2', 'functions_index': str(INDEX), 'functions_index_sha256': digest(INDEX)},
        'database': {'path': str(ROOT / 'build/ghidra/libgstlgaudiosink.so'), 'reimported': False,
            'opened_this_run': False, 'modified': False},
        'fresh_decompile_attempt': {'runner': str(OWNED / 'earc-arm-run-20261104.sh'), 'succeeded': False,
            'java_home': '/opt/homebrew/opt/openjdk@21', 'blocker': 'UnsupportedClassVersionError: installed Ghidra class version 69, Java 21 maximum 65',
            'global_configuration_changed': False},
        'commands': [dis_command, dwarf_command], 'elf_sections': sections,
        'checks': {'cached_binary_sha256_matches': True, 'selected_entries_match_elf_symbols': True,
            'all_emitted_disassembly_bytes_match_elf': True, 'verified_listing_rows': byte_checks},
        'scope': {'writes': str(OWNED), 'tv_access': False, 'production_edits': False, 'git_commands': False,
            'tool_installations': False, 'background_processes_started': False}})
    print(json.dumps({'bundle': str(OWNED), 'functions': len(functions), 'verified_listing_rows': byte_checks,
                      'elf_sha256': digest(ELF), 'literal_words': len(literals)}))


def validate():
    coverage = json.loads((OWNED / 'sink-focused.coverage.json').read_text())
    provenance = json.loads((OWNED / 'sink-focused.provenance.json').read_text())
    assert coverage['functions_completed'] == len(SELECTED)
    assert coverage['verified_listing_rows'] > 1000
    assert provenance['binary']['sha256'] == digest(ELF)
    assert provenance['cached_decompile']['sha256'] == digest(DECOMP)
    assert 'ioctl(ao->fd,0x4008522b,&value)' in (OWNED / 'sink-focused.c').read_text()
    assert coverage['set_caps_abstract_and_inline_dies']
    files = {}
    for file in sorted(OWNED.iterdir()):
        if file.is_file() and file.name != 'sink-focused.sha256.json':
            files[file.name] = {'path': str(file), 'sha256': digest(file), 'size': file.stat().st_size}
    save_json('sink-focused.sha256.json', files)
    print(json.dumps({'validation': 'passed', 'files_hashed': len(files), 'verified_listing_rows': coverage['verified_listing_rows']}))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--validate', action='store_true')
    options = parser.parse_args()
    if not options.validate:
        build()
    validate()