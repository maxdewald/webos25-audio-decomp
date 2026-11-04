import assert from 'node:assert/strict';
import { createHash } from 'node:crypto';
import { readFileSync, writeFileSync } from 'node:fs';
import { dirname, join, resolve } from 'node:path';
import { fileURLToPath } from 'node:url';
import { execFileSync } from 'node:child_process';

const directory = dirname(fileURLToPath(import.meta.url));
const root = resolve(directory, '../..');
const policy = '.';
const llvm = '/opt/homebrew/opt/llvm/bin/llvm-objdump';
const checkOnly = process.argv.includes('--check');
const digest = bytes => createHash('sha256').update(bytes).digest('hex');
const hex = value => `0x${value.toString(16)}`;
const records = [];
let decompiled = '';
let disassembly = '';

function retain(name, contents) {
  const destination = join(directory, name);
  if (checkOnly) assert.equal(readFileSync(destination, 'utf8'), contents, name);
  else writeFileSync(destination, contents);
}

function elf(binaryPath) {
  const absolute = join(root, binaryPath);
  const bytes = readFileSync(absolute);
  assert.equal(bytes.subarray(0, 6).toString('hex'), '7f454c460101');
  assert.equal(bytes.readUInt16LE(18), 40);
  const segments = [];
  for (let index = 0; index < bytes.readUInt16LE(44); index += 1) {
    const offset = bytes.readUInt32LE(28) + index * bytes.readUInt16LE(42);
    if (bytes.readUInt32LE(offset) === 1) segments.push({
      offset: bytes.readUInt32LE(offset + 4), address: bytes.readUInt32LE(offset + 8),
      size: bytes.readUInt32LE(offset + 16),
    });
  }
  function fileOffset(address) {
    const segment = segments.find(item => address >= item.address && address < item.address + item.size);
    assert.ok(segment, `file-backed ${hex(address)}`);
    return segment.offset + address - segment.address;
  }
  function stringAt(address) {
    const offset = fileOffset(address);
    const end = bytes.indexOf(0, offset);
    assert.ok(end >= offset);
    return bytes.subarray(offset, end).toString('utf8');
  }
  return { absolute, bytes, fileOffset, stringAt, sha256: digest(bytes) };
}

const targets = [
  { name: 'starfish-media-pipeline', binary: 'usr/sbin/starfish-media-pipeline', functions: [
    ['RB-SMP-ALLOCATE', 0x51410], ['RB-SMP-LOADOPTIONS', 0x5433c],
    ['RB-SMP-RMCTOR', 0x97b00],
  ] },
  { name: 'libmedia-resource-calculator.so.1', binary: 'usr/lib/libmedia-resource-calculator.so.1', functions: [
    ['RB-MRC-LOOKUP', 0x14c04], ['RB-MRC-CTOR', 0x16064],
  ] },
  { name: 'umediaserver', binary: 'usr/sbin/umediaserver', functions: [
    ['RB-UMS-ACQUIRE', 0x67cb4],
  ] },
];

for (const target of targets) {
  const binaryPath = `${policy}/orig/${target.binary}`;
  const binary = elf(binaryPath);
  const exportPath = `${policy}/decomp/${target.name}.c`;
  const exported = readFileSync(join(root, exportPath), 'utf8');
  const rows = readFileSync(join(root, `${policy}/symbols/${target.name}.functions.tsv`), 'utf8')
    .trim().split('\n').map(row => row.split('\t'));
  for (const [id, ghidra] of target.functions) {
    const addressText = ghidra.toString(16).padStart(8, '0');
    const row = rows.find(item => item[0] === addressText);
    assert.ok(row, id);
    const start = exported.indexOf(`/* @${addressText} `);
    assert.ok(start >= 0, id);
    const next = exported.indexOf('\n/* @', start + 1);
    const excerpt = exported.slice(start, next < 0 ? undefined : next);
    const raw = ghidra - 0x10000;
    const size = Number(row[1]);
    const assembly = execFileSync(llvm, ['--disassemble', '--triple=thumbv7-linux-gnueabi',
      '--no-show-raw-insn', `--start-address=${hex(raw)}`, `--stop-address=${hex(raw + size)}`,
      binary.absolute], { encoding: 'utf8', maxBuffer: 8 * 1024 * 1024 });
    assert.ok(assembly.includes(`${raw.toString(16)}:`), id);
    decompiled += `\nLOCAL EVIDENCE ${id}\n${excerpt}`;
    disassembly += `\nLOCAL EVIDENCE ${id}\n${assembly}`;
    records.push({ id, kind: 'existing_export_and_local_llvm', binaryPath,
      binarySha256: binary.sha256, exportPath, exportSha256: digest(exported),
      functionName: row[2], ghidraAddress: hex(ghidra), rawElfVirtualAddress: hex(raw),
      fileOffset: hex(binary.fileOffset(raw)), disassemblyBytes: size,
      startLine: exported.slice(0, start).split('\n').length,
      endLine: exported.slice(0, start + excerpt.length).split('\n').length,
      excerptSha256: digest(excerpt), disassemblySha256: digest(assembly) });
  }
}

const pfPath = `${policy}/orig/usr/lib/libpf-1.0.so.1.0.0`;
const pf = elf(pfPath);
const pfAssembly = execFileSync(llvm, ['--disassemble', '--triple=thumbv7-linux-gnueabi',
  '--no-show-raw-insn', '--start-address=0x65724', '--stop-address=0x65a38', pf.absolute],
{ encoding: 'utf8', maxBuffer: 2 * 1024 * 1024 });
disassembly += `\nLOCAL EVIDENCE RB-PF-SETRESOURCE-FULL\n${pfAssembly}`;
const literalSites = [
  [0x65826, 0x659e0], [0x65830, 0x659e4], [0x65838, 0x659e8],
  [0x65858, 0x659ec], [0x65864, 0x659f0], [0x65868, 0x659f4],
  [0x65870, 0x659f8], [0x6587a, 0x659fc], [0x65882, 0x65a00],
  [0x6588c, 0x65a04], [0x658d6, 0x65a0c], [0x658ea, 0x65a10],
  [0x658ee, 0x65a14], [0x658f4, 0x65a18], [0x658f8, 0x65a1c],
].map(([instruction, literal]) => {
  const displacement = pf.bytes.readInt32LE(pf.fileOffset(literal));
  const address = (instruction + 4 + displacement) >>> 0;
  return { addPcRaw: hex(instruction), literalRaw: hex(literal),
    stringRaw: hex(address), stringGhidra: hex(address + 0x10000), value: pf.stringAt(address) };
});
for (const expected of ['acquired-resource', 'core-type', 'audio-port', 'mixer-port', 'active']) {
  assert.ok(literalSites.some(site => site.value === expected), expected);
}
records.push({ id: 'RB-PF-SETRESOURCE-FULL', kind: 'symbol_bounded_local_llvm_and_literal_resolution',
  binaryPath: pfPath, binarySha256: pf.sha256, ghidraAddress: '0x75724',
  rawElfVirtualAddress: '0x65724', fileOffset: '0x65724', elfThumbSymbolValue: '0x65725',
  disassemblyBytes: 788, disassemblySha256: digest(pfAssembly), literalSites });

const mrcPath = `${policy}/orig/usr/lib/libmedia-resource-calculator.so.1`;
const mrc = elf(mrcPath);
const tableStart = mrc.bytes.indexOf(Buffer.from('{  "aac6": {'));
assert.ok(tableStart >= 0);
const tableEnd = mrc.bytes.indexOf(0, tableStart);
const tableBytes = mrc.bytes.subarray(tableStart, tableEnd);
const table = JSON.parse(tableBytes.toString('utf8'));
assert.deepEqual(table.default, { ADEC: 1, ADEC_BANDWIDTH: 2 });
assert.equal(Object.hasOwn(table, 'dts'), false);
assert.equal(Object.hasOwn(table, 'pcm'), false);
records.push({ id: 'RB-MRC-DEFAULT-TABLE', kind: 'original_binary_embedded_json',
  binaryPath: mrcPath, binarySha256: mrc.sha256, fileOffset: hex(tableStart),
  rawElfVirtualAddress: hex(tableStart), ghidraAddress: hex(tableStart + 0x10000),
  byteLength: tableBytes.length, sha256: digest(tableBytes), table });

const publicRoot = '../dts_restore_plus/re/lge-src/code/gstreamer/subprojects';
const sourceRanges = [
  ['RB-SRC-DECPROXY-EVENT', `${publicRoot}/gst-plugins-cool/gst/decproxy/gstdecproxy2.c`, 1360, 1403],
  ['RB-SRC-DECPROXY-SELECT', `${publicRoot}/gst-plugins-cool/gst/decproxy/gstdecproxy2.c`, 1421, 1562],
  ['RB-SRC-PLAYSINK', `${publicRoot}/gst-plugins-base/gst/playback/gstplaysink.c`, 2979, 3006],
  ['RB-CFG-RESOURCES', `${policy}/orig/etc/umediaserver/umediaserver_resource_config.txt`, 63, 164],
];
for (const [id, sourcePath, startLine, endLine] of sourceRanges) {
  const bytes = readFileSync(join(root, sourcePath));
  const excerpt = bytes.toString('utf8').split('\n').slice(startLine - 1, endLine).join('\n');
  decompiled += `\nLOCAL EVIDENCE ${id}\n${excerpt}\n`;
  records.push({ id, kind: 'local_source_or_config_excerpt', sourcePath, sha256: digest(bytes),
    startLine, endLine, excerptSha256: digest(excerpt),
    limitation: id.startsWith('RB-SRC-') ? 'Public source is not proof that a shipped plugin is byte-identical.' : 'Packaged configuration is not a live resource-pool snapshot.' });
}

retain('supplemental-decompiled.txt', decompiled);
retain('supplemental-disassembly.txt', disassembly);
retain('supplemental-evidence.json', `${JSON.stringify(records, null, 2)}\n`);
console.log(`PASS: ${records.length} supplemental records; embedded table, ELF entry bounds, event literals and source hashes; mode=${checkOnly ? 'check' : 'generate'}.`);