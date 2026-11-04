import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';

const root = path.dirname(new URL(import.meta.url).pathname);
const targetName = process.argv[2] || 'libumi';
const binaryPath = path.resolve(root, targetName === 'libumi' ? '../../orig/usr/lib/libumi.so.1.0.0' : '../../orig/usr/sbin/audiooutputd');
const binary = fs.readFileSync(binaryPath);
if (binary.readUInt32LE(0) !== 0x464c457f || binary[4] !== 1 || binary[5] !== 1) throw new Error('Expected little-endian ELF32');
const segments = [];
for (let index = 0; index < binary.readUInt16LE(44); index++) {
  const offset = binary.readUInt32LE(28) + index * binary.readUInt16LE(42);
  if (binary.readUInt32LE(offset) === 1) segments.push({offset: binary.readUInt32LE(offset + 4), virtual: binary.readUInt32LE(offset + 8), size: binary.readUInt32LE(offset + 16)});
}
function fileOffset(address) {
  const segment = segments.find(entry => address >= entry.virtual && address < entry.virtual + entry.size);
  return segment ? address - segment.virtual + segment.offset : null;
}
function word(address) {
  const offset = fileOffset(address);
  return offset === null || offset + 4 > binary.length ? null : binary.readUInt32LE(offset);
}
function text(address) {
  const offset = fileOffset(address);
  if (offset === null) return null;
  let end = offset;
  while (end < binary.length && end - offset < 500 && binary[end] !== 0) {
    if (binary[end] < 32 || binary[end] > 126) return null;
    end++;
  }
  return end > offset && binary[end] === 0 ? binary.toString('ascii', offset, end) : null;
}
const input = fs.readFileSync(path.join(root, `${targetName}.asm`), 'utf8');
const registers = new Map();
const output = [];
for (const line of input.split('\n')) {
  if (/^[0-9a-f]+ </.test(line)) registers.clear();
  const parsed = line.match(/^\s*([0-9a-f]+):\s+(?:[0-9a-f]+\s+)+\s*([a-z][a-z0-9.]*)\s+(.*)$/);
  let annotation = '';
  if (parsed) {
    const address = Number.parseInt(parsed[1], 16);
    const operation = parsed[2];
    const operands = parsed[3];
    const literal = operands.match(/^(r\d+|lr), \[pc, #(-?(?:0x[0-9a-f]+|\d+))\]/);
    if (operation.startsWith('ldr') && literal) {
      const value = word(((address + 4) & ~3) + Number(literal[2]));
      if (value !== null) {
        registers.set(literal[1], value);
        annotation = ` literal=0x${value.toString(16)}`;
      }
    }
    const addition = operands.match(/^(r\d+|lr), pc$/);
    if (operation.startsWith('add') && addition && registers.has(addition[1])) {
      const value = (registers.get(addition[1]) + address + 4) >>> 0;
      const resolved = text(value);
      annotation = ` => raw=0x${value.toString(16)} ghidra=0x${(value + 0x10000).toString(16)} file=${fileOffset(value)}${resolved === null ? '' : ` ${JSON.stringify(resolved)}`}`;
      registers.set(addition[1], value);
    }
    if (/^blx?$/.test(operation)) for (const register of ['r0', 'r1', 'r2', 'r3', 'lr']) registers.delete(register);
  }
  output.push(line + annotation);
}
const destination = path.join(root, `${targetName}.annotated.asm`);
fs.writeFileSync(destination, output.join('\n'));
console.log(JSON.stringify({binary: binaryPath, sha256: crypto.createHash('sha256').update(binary).digest('hex'), segments, destination, annotations: output.filter(line => line.includes(' => raw=')).length}, null, 2));