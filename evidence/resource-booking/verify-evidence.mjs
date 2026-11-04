import assert from 'node:assert/strict';
import { createHash } from 'node:crypto';
import { readFileSync, writeFileSync } from 'node:fs';
import { dirname, join, relative, resolve } from 'node:path';
import { fileURLToPath } from 'node:url';
import { execFileSync } from 'node:child_process';

const outputDirectory = dirname(fileURLToPath(import.meta.url));
const policyDirectory = resolve(outputDirectory, '../..');
const projectDirectory = policyDirectory;
const objdump = '/opt/homebrew/opt/llvm/bin/llvm-objdump';
const sha256 = bytes => createHash('sha256').update(bytes).digest('hex');
const hex = value => `0x${value.toString(16)}`;
const checkOnly = process.argv.includes('--check');

function retainEvidence(name, contents) {
  const destination = join(outputDirectory, name);
  if (checkOnly) {
    assert.equal(readFileSync(destination, 'utf8'), contents, `${name} matches acquired inputs`);
  } else {
    writeFileSync(destination, contents);
  }
}

function openElf(localPath) {
  const absolutePath = join(policyDirectory, 'orig', localPath);
  const bytes = readFileSync(absolutePath);
  assert.equal(bytes.subarray(0, 4).toString('hex'), '7f454c46');
  assert.equal(bytes[4], 1, 'ELF32 required');
  assert.equal(bytes[5], 1, 'little-endian ELF required');
  assert.equal(bytes.readUInt16LE(18), 40, 'ARM required');
  const segments = [];
  const programOffset = bytes.readUInt32LE(28);
  const programSize = bytes.readUInt16LE(42);
  const programCount = bytes.readUInt16LE(44);
  for (let index = 0; index < programCount; index += 1) {
    const offset = programOffset + index * programSize;
    if (bytes.readUInt32LE(offset) !== 1) continue;
    segments.push({
      offset: bytes.readUInt32LE(offset + 4),
      virtualAddress: bytes.readUInt32LE(offset + 8),
      fileSize: bytes.readUInt32LE(offset + 16),
      memorySize: bytes.readUInt32LE(offset + 20),
      flags: bytes.readUInt32LE(offset + 24),
    });
  }
  function fileOffset(virtualAddress) {
    const segment = segments.find(candidate => virtualAddress >= candidate.virtualAddress &&
      virtualAddress < candidate.virtualAddress + candidate.fileSize);
    assert.ok(segment, `file-backed ELF VA ${hex(virtualAddress)}`);
    return segment.offset + virtualAddress - segment.virtualAddress;
  }
  return {
    absolutePath, bytes, fileOffset,
    identity: {
      path: relative(projectDirectory, absolutePath), sha256: sha256(bytes),
      size: bytes.length, machine: 'ARM', elfClass: 32, endianness: 'little',
      elfType: bytes.readUInt16LE(16), loadSegments: segments,
      addressConvention: 'Existing Ghidra exports use ELF virtual address + 0x10000; file offsets use PT_LOAD mapping, not this bias.',
    },
  };
}

const targets = [
  { id: 'RB-ELF-SMP', path: 'usr/sbin/starfish-media-pipeline', exportName: 'starfish-media-pipeline',
    functions: [
      { id: 'RB-SMP-GENERATE', ghidra: 0x58630 },
      { id: 'RB-SMP-SWPLATFORM', ghidra: 0x7fdf0 },
      { id: 'RB-SMP-CODEC', ghidra: 0x97214 },
      { id: 'RB-SMP-INDEX', ghidra: 0x992dc },
      { id: 'RB-SMP-REQUEST', ghidra: 0x9c63c },
      { id: 'RB-SMP-REQUEST-ONE', ghidra: 0x9b5e0 },
      { id: 'RB-SMP-ACQUIRE', ghidra: 0x9e2d4 },
      { id: 'RB-SMP-ADAPTIVE', ghidra: 0x9cc50 },
      { id: 'RB-SMP-CLONED', ghidra: 0x9c544 },
    ],
    strings: [{ ghidra: 0xab43c, expected: 'ADEC' }, { ghidra: 0xab444, expected: 'AMIXER' }] },
  { id: 'RB-ELF-MRC', path: 'usr/lib/libmedia-resource-calculator.so.1', exportName: 'libmedia-resource-calculator.so.1',
    functions: [{ id: 'RB-MRC-ADEC', ghidra: 0x158d8 }], strings: [] },
  { id: 'RB-ELF-RM', path: 'usr/lib/libresource_mgr.so.1.0.0', exportName: 'libresource_mgr.so.1.0.0',
    functions: [
      { id: 'RB-RM-PARSE', ghidra: 0x1ef6c },
      { id: 'RB-RM-POOL', ghidra: 0x25650 },
      { id: 'RB-RM-DECODE', ghidra: 0x31838 },
      { id: 'RB-RM-ACQUIRE', ghidra: 0x325b0 },
    ], strings: [] },
  { id: 'RB-ELF-RMC', path: 'usr/lib/libresource_mgr_client.so.1.0.0', exportName: 'libresource_mgr_client.so.1.0.0',
    functions: [
      { id: 'RB-RMC-ACQUIRE', ghidra: 0x1a350 },
      { id: 'RB-RMC-COMPLETE', ghidra: 0x2109c },
    ], strings: [] },
  { id: 'RB-ELF-PF', path: 'usr/lib/libpf-1.0.so.1.0.0',
    functions: [
      { id: 'RB-PF-SETRESOURCE', ghidra: 0x75724, size: 0x210 },
      { id: 'RB-PF-UPDATE', ghidra: 0x85534, size: 0x498 },
    ], strings: [] },
];

const evidence = [];
let allDecompiled = '';
let allAssembly = '';
let allStringArguments = '';
for (const target of targets) {
  const elf = openElf(target.path);
  evidence.push({ id: target.id, kind: 'binary_identity', ...elf.identity });
  const exportPath = target.exportName ? join(policyDirectory, 'decomp', `${target.exportName}.c`) : null;
  const exported = exportPath ? readFileSync(exportPath, 'utf8') : '';
  const functionRows = target.exportName ? readFileSync(join(policyDirectory, 'symbols', `${target.exportName}.functions.tsv`), 'utf8').trim().split('\n').map(row => row.split('\t')) : [];
  for (const expected of target.strings) {
    const virtualAddress = expected.ghidra - 0x10000;
    const offset = elf.fileOffset(virtualAddress);
    const actual = elf.bytes.subarray(offset, elf.bytes.indexOf(0, offset)).toString('utf8');
    assert.equal(actual, expected.expected, `Ghidra string ${hex(expected.ghidra)}`);
  }
  for (const procedure of target.functions) {
    const addressText = procedure.ghidra.toString(16).padStart(8, '0');
    const start = exported.indexOf(`/* @${addressText} `);
    if (exportPath) assert.ok(start >= 0, `export contains ${addressText}`);
    const next = start < 0 ? -1 : exported.indexOf('\n/* @', start + 1);
    const excerpt = start < 0 ? '' : exported.slice(start, next < 0 ? undefined : next);
    const functionRow = functionRows.find(row => row[0] === addressText);
    const size = procedure.size ?? Number(functionRow?.[1]);
    assert.ok(size > 0, `function size for ${procedure.id}`);
    const rawAddress = procedure.ghidra - 0x10000;
    const assembly = execFileSync(objdump, [
      '--disassemble', '--triple=armv7-linux-gnueabi', '--mattr=+thumb-mode', '--no-show-raw-insn', `--start-address=${hex(rawAddress)}`,
      `--stop-address=${hex(rawAddress + size)}`, elf.absolutePath,
    ], { encoding: 'utf8', maxBuffer: 4 * 1024 * 1024 });
    assert.ok(assembly.includes(`${rawAddress.toString(16)}:`), 'disassembly reaches function entry');
    allDecompiled += `\nLOCAL EVIDENCE ${procedure.id}\n${excerpt}`;
    allAssembly += `\nLOCAL EVIDENCE ${procedure.id}\n${assembly}`;
    if (target.exportName === 'starfish-media-pipeline' && functionRow) {
      const argumentsPath = join(policyDirectory, 'decomp/starfish-media-pipeline.funcstrings.tsv');
      const argumentRows = readFileSync(argumentsPath, 'utf8').split('\n').filter(row => row.split('\t')[0] === functionRow[2]);
      allStringArguments += `\nLOCAL EVIDENCE ${procedure.id}\n${argumentRows.join('\n')}\n`;
    }
    evidence.push({
      id: procedure.id, kind: 'existing_decompilation_and_original_binary_disassembly', binaryId: target.id,
      functionName: functionRow?.[2] ?? 'raw binary symbol; see disassembly',
      ghidraAddress: hex(procedure.ghidra), rawElfVirtualAddress: hex(rawAddress),
      fileOffset: hex(elf.fileOffset(rawAddress)), requestedDisassemblyBytes: size,
      exportPath: exportPath ? relative(projectDirectory, exportPath) : null,
      exportSha256: exportPath ? sha256(Buffer.from(exported)) : null,
      startLine: exportPath ? exported.slice(0, start).split('\n').length : null,
      endLine: exportPath ? exported.slice(0, start + excerpt.length).split('\n').length : null,
      excerptSha256: excerpt ? sha256(Buffer.from(excerpt)) : null, disassemblySha256: sha256(Buffer.from(assembly)),
      limitation: 'Decompiled pseudocode is not original source. A disassembly span may include data or gaps; addresses are not runtime relocated addresses.',
    });
  }
}
retainEvidence('evidence-decompiled.txt', allDecompiled);
retainEvidence('evidence-disassembly.txt', allAssembly);
retainEvidence('evidence-string-arguments.txt', allStringArguments);
retainEvidence('observed-evidence.json', `${JSON.stringify(evidence, null, 2)}\n`);
console.log(`PASS: ${evidence.length} local evidence records; ELF32 ARM mappings, string bias, function entries verified; mode=${checkOnly ? 'check' : 'generate'}.`);