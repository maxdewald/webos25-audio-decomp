import fs from 'node:fs';
import path from 'node:path';
import assert from 'node:assert/strict';
import crypto from 'node:crypto';

const root = path.dirname(new URL(import.meta.url).pathname);
const report = JSON.parse(fs.readFileSync(path.join(root, 'findings.json'), 'utf8'));
const ids = new Set(report.evidence.map(entry => entry.id));
assert.equal(ids.size, report.evidence.length);
let referenceCount = 0;
function references(value) {
  if (!value || typeof value !== 'object') return;
  if (value.evidence_ids) for (const identifier of value.evidence_ids) {
    assert(ids.has(identifier), `Unknown evidence ${identifier}`);
    referenceCount++;
  }
  for (const child of Object.values(value)) references(child);
}
references(report);
const anchors = [];
for (const target of report.targets) {
  const binary = fs.readFileSync(path.resolve(root, target.path));
  assert.equal(crypto.createHash('sha256').update(binary).digest('hex'), target.sha256);
  const listing = fs.readFileSync(path.join(root, `${target.id}.annotated.asm`), 'utf8');
  const instructions = new Map();
  for (const line of listing.split('\n')) {
    const parsed = line.match(/^\s*([0-9a-f]+):\s+((?:[0-9a-f]{4} ?){1,2})\s+(.+)$/);
    if (parsed) instructions.set(Number.parseInt(parsed[1], 16), {words: parsed[2].trim().split(/\s+/), text: parsed[3]});
  }
  function fileOffset(address) {
    for (let index = 0; index < binary.readUInt16LE(44); index++) {
      const offset = binary.readUInt32LE(28) + index * binary.readUInt16LE(42);
      if (binary.readUInt32LE(offset) !== 1) continue;
      const virtual = binary.readUInt32LE(offset + 8);
      if (address >= virtual && address < virtual + binary.readUInt32LE(offset + 16)) return address - virtual + binary.readUInt32LE(offset + 4);
    }
    throw new Error(`Unmapped ${address}`);
  }
  for (const entry of report.evidence.filter(value => value.target === target.id)) {
    assert.equal(Number(entry.ghidra_address), Number(entry.raw_address) + 0x10000);
    for (const site of entry.sites_raw || [entry.raw_address]) {
      const address = Number(site);
      const instruction = instructions.get(address);
      assert(instruction, `${entry.id}: no instruction at ${site}`);
      const offset = fileOffset(address);
      instruction.words.forEach((word, index) => assert.equal(binary.readUInt16LE(offset + index * 2), Number.parseInt(word, 16), `${entry.id} byte mismatch`));
      anchors.push({evidence_id: entry.id, target: target.id, raw_address: site, ghidra_address: `0x${(address + 0x10000).toString(16)}`, file_offset: offset, bytes: binary.subarray(offset, offset + instruction.words.length * 2).toString('hex'), instruction: instruction.text});
    }
  }
  if (target.id === 'libumi') {
    for (const row of [...report.decision_tables.pass_through.ordinary_rows, ...report.decision_tables.pass_through.japan_aac_rows]) {
      const instruction = instructions.get(Number(row.ct_raw));
      assert(instruction?.text.includes('r8,'), `Missing eARC ct destination ${row.ct_raw}`);
      assert(instruction.text.includes(`#0x${row.ct.toString(16)}`), `Wrong ct immediate ${row.ct_raw}`);
      assert(binary.includes(Buffer.from(row.capability + '\0')), `Missing capability literal ${row.capability}`);
      for (const codec of Array.isArray(row.codec) ? row.codec : [row.codec]) assert(binary.includes(Buffer.from(codec + '\0')));
    }
    const absentTokens = ['inputInfo', 'categoryType'].map(token => ({token, present_as_nul_terminated_literal: binary.includes(Buffer.from(token + '\0'))}));
    console.log(JSON.stringify({bounded_literal_search: absentTokens}));
  }
}
for (const payload of report.payloads) {
  assert.equal(payload.actionable, false);
  assert.equal(payload.executed, false);
  if (payload.metadata) {
    assert.equal(typeof payload.metadata.pipelineInfo.resource, 'string');
    assert.equal(payload.metadata.categoryInfo.resource, undefined);
  }
}
fs.writeFileSync(path.join(root, 'validated-anchors.json'), JSON.stringify(anchors, null, 2) + '\n');
console.log(JSON.stringify({result: 'pass', binary_hashes: report.targets.length, unique_evidence_ids: ids.size, evidence_references: referenceCount, instruction_byte_anchors: anchors.length, direct_codec_rows: report.decision_tables.pass_through.ordinary_rows.length, japan_aac_rows: report.decision_tables.pass_through.japan_aac_rows.length, payloads_not_executed: report.payloads.length}, null, 2));