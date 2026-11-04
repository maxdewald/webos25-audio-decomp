import fs from 'node:fs';
import path from 'node:path';
import assert from 'node:assert/strict';

const root = path.dirname(new URL(import.meta.url).pathname);
const names = ['setSettings', 'setQualityAttribute', 'setAudioVideoCategoryInfo', 'updatePipelineInfo', 'storePipelineInfoParameters', 'updateExistingPipelineInfo', 'updateAudioCategoryInfo', 'setAudioInputInfo', 'supportOutputChannel', 'setOutputType', 'setOutputTypeForPassThrough', 'setEArcOutputType', 'setArcOutputType', 'setMixerSettings', 'setSoundOutputModeSettings', 'setExternalArcSettings', 'setSoundOutputDigitalSettings', 'setAlsaDriverSettings', 'setAllSoundOutputDigital', 'isEarcCapabilityAvailable', 'getEarcAndARCOutputCodec', 'setOutputTypeForDTS', 'convertDeviceChannelCapability', 'isValidChannel', 'isSupportIAMF', 'setArcOnOff', 'setArcOnOffAndDelaySetting', 'getSoundOutputDigitalMode', 'storeParameters'];
for (const target of ['libumi', 'audiooutputd']) {
  const listing = fs.readFileSync(path.join(root, `${target}.annotated.asm`), 'utf8');
  const functions = [];
  let current;
  for (const line of listing.split('\n')) {
    const entry = line.match(/^([0-9a-f]+) <(.+)>:$/);
    if (entry) {
      const functionName = entry[2].split('(')[0].replace('[abi:cxx11]', '').split('::').at(-1);
      current = names.includes(functionName) ? {raw_address: `0x${entry[1]}`, ghidra_address: `0x${(Number.parseInt(entry[1], 16) + 0x10000).toString(16)}`, name: entry[2], lines: []} : null;
      if (current) functions.push(current);
    }
    if (current) current.lines.push(line.replace(/ <[^>]*(?:>[^@\n]*)?(?= @ imm =)/g, '').replace(/ @ imm =.*?(?= literal=| => raw=|$)/, ''));
  }
  fs.writeFileSync(path.join(root, `${target}.focused.json`), JSON.stringify(functions, null, 2) + '\n');
  console.log(`${target}: ${functions.length} focused function bodies retained`);
}
const binary = fs.readFileSync(path.resolve(root, '../../orig/usr/sbin/audiooutputd'));
function fileOffset(address) {
  for (let index = 0; index < binary.readUInt16LE(44); index++) {
    const offset = binary.readUInt32LE(28) + index * binary.readUInt16LE(42);
    if (binary.readUInt32LE(offset) !== 1) continue;
    const virtual = binary.readUInt32LE(offset + 8);
    if (address >= virtual && address < virtual + binary.readUInt32LE(offset + 16)) return address - virtual + binary.readUInt32LE(offset + 4);
  }
  throw new Error(`Unmapped address ${address}`);
}
function text(address) {
  const offset = fileOffset(address);
  return binary.toString('ascii', offset, binary.indexOf(0, offset));
}
const methods = [];
for (let index = 0; index < 4; index++) {
  const address = 0x20be4 + index * 12;
  const offset = fileOffset(address);
  const name = binary.readUInt32LE(offset);
  const callback = binary.readUInt32LE(offset + 4);
  methods.push({table_raw_address: `0x${address.toString(16)}`, table_ghidra_address: `0x${(address + 0x10000).toString(16)}`, file_offset: offset, name: name ? text(name) : null, callback_pointer: `0x${callback.toString(16)}`, callback_raw_address: `0x${(callback & ~1).toString(16)}`, callback_ghidra_address: `0x${((callback & ~1) + 0x10000).toString(16)}`, flags_raw: binary.readUInt32LE(offset + 8)});
}
assert.equal(methods[0].name, 'setSettings');
assert.equal(methods[1].name, 'setQualityAttribute');
assert.equal(methods[2].name, 'getSettings');
assert.equal(methods[3].name, null);
fs.writeFileSync(path.join(root, 'quality-method-table.json'), JSON.stringify(methods, null, 2) + '\n');
console.log(JSON.stringify(methods, null, 2));