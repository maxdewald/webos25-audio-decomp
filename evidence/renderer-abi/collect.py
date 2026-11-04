import ctypes
import hashlib
import json
from pathlib import Path
import struct
import subprocess


ROOT = Path(__file__).resolve().parents[2]
OUT = Path(__file__).resolve().parent
LLVM = Path('/opt/homebrew/opt/llvm/bin')
ARTIFACTS = {
    'sink': ROOT / 'orig/usr/lib/gstreamer-1.0/libgstlgaudiosink.so',
    'decoder': ROOT / 'orig/usr/lib/gstreamer-1.0/libgstlgaudiodec.so',
    'kadaptor': ROOT / 'orig/usr/lib/libkadaptor.so.2.0.1',
    'kernel_image': ROOT / 'orig/boot/Image-5.4.268-329.decompressed',
    'kernel_compressed': ROOT / 'orig/boot/Image.lz4-5.4.268-329.ptl4tv.5',
}
RANGES = {
    'sink-connect': ('sink', 0x1441c, 0x14924),
    'sink-open_renderer': ('sink', 0x14924, 0x14ae0),
    'sink-config_renderer': ('sink', 0x16fc4, 0x17038),
    'sink-event': ('sink', 0x8240, 0xbbf4),
    'sink-change_state': ('sink', 0xd7c0, 0x10318),
    'sink-set_port': ('sink', 0x15b58, 0x15d3c),
    'sink-first_event': ('sink', 0x182d0, 0x18828),
    'sink-set_audio_info': ('sink', 0x1673c, 0x16884),
    'sink-open': ('sink', 0x14be0, 0x14e00),
    'decoder-set_es_bypass': ('decoder', 0x20848, 0x209c8),
    'kadaptor-init': ('kadaptor', 0x61b00, 0x61c7c),
    'kernel-dispatch': ('kernel_image', 0xc30494, 0xc30d04),
    'kernel-get_uid': ('kernel_image', 0xc35ab0, 0xc35da0),
    'kernel-getters': ('kernel_image', 0xc251d0, 0xc253c8),
    'kernel-constructor': ('kernel_image', 0xc21894, 0xc219ac),
    'kernel-open': ('kernel_image', 0xc355c4, 0xc35798),
    'kernel-set_pcm_info': ('kernel_image', 0xc37b40, 0xc37d30),
}


def digest(filename):
    hasher = hashlib.sha256()
    with filename.open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b''):
            hasher.update(chunk)
    return hasher.hexdigest()


def output_command(filename, arguments):
    result = subprocess.run(arguments, check=True, capture_output=True, text=True)
    (OUT / filename).write_text(result.stdout)


def kernel_identity(image):
    container = ARTIFACTS['kernel_compressed'].read_bytes()
    assert container[:4] == b'LZ4P'
    total, compressed_size, block_size, block_count = struct.unpack_from('<IIII', container, 4)
    block_sizes = struct.unpack_from(f'<{block_count}I', container, 0x20)
    decoder = ctypes.CDLL('/opt/homebrew/lib/liblz4.dylib').LZ4_decompress_safe
    decoder.argtypes = [ctypes.c_char_p, ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
    decoder.restype = ctypes.c_int
    compressed_offset = 0x20 + 4 * block_count
    decoded_offset = 0
    hasher = hashlib.sha256()
    matching_blocks = 0
    for length in block_sizes:
        expected = min(block_size, total - decoded_offset)
        output = ctypes.create_string_buffer(expected)
        actual = decoder(container[compressed_offset:compressed_offset + length], output, length, expected)
        assert actual == expected, (actual, expected)
        decoded = output.raw[:actual]
        matching_blocks += decoded == image[decoded_offset:decoded_offset + actual]
        hasher.update(decoded)
        compressed_offset += length
        decoded_offset += actual
    identity = {'container_magic': 'LZ4P', 'decoded_size': decoded_offset,
                'decoded_sha256': hasher.hexdigest(), 'matching_blocks': matching_blocks,
                'block_count': block_count, 'exact_match': matching_blocks == block_count and decoded_offset == len(image)}
    assert identity['exact_match'], identity
    return identity


def branch_witness(image, request):
    instruction_offset = 0xc304dc
    register_zero = 0
    comparison = 0
    trace = []
    for count in range(100):
        if instruction_offset == 0xc308e0:
            return {'request': hex(request), 'result': 'default: ENOTTY', 'default_offset': hex(instruction_offset), 'trace': trace}
        word = struct.unpack_from('<I', image, instruction_offset)[0]
        trace.append({'offset': hex(instruction_offset), 'word': hex(word)})
        if word & 0xff80001f == 0x52800000:
            register_zero = ((word >> 5) & 65535) << (((word >> 21) & 3) * 16)
        elif word & 0xff80001f == 0x72800000:
            shift = ((word >> 21) & 3) * 16
            register_zero = (register_zero & ~(65535 << shift)) | (((word >> 5) & 65535) << shift)
        elif word == 0x6b00027f:
            comparison = (request > register_zero) - (request < register_zero)
        elif word & 0xff000010 == 0x54000000:
            condition = word & 15
            taken = {0: comparison == 0, 1: comparison != 0, 8: comparison > 0, 9: comparison <= 0}[condition]
            if taken:
                displacement = (word >> 5) & 0x7ffff
                if displacement & 0x40000:
                    displacement -= 0x80000
                instruction_offset += displacement * 4
                continue
        else:
            raise AssertionError((hex(instruction_offset), hex(word)))
        instruction_offset += 4
    raise AssertionError('Branch witness did not terminate')


def collect():
    metadata = {}
    for name, filename in ARTIFACTS.items():
        metadata[name] = {'path': str(filename), 'sha256': digest(filename), 'size': filename.stat().st_size}
    source_directory = ROOT / '../dts_restore_plus/re/lge-src/linux-and-kdriver_paluma-5/kdriver'
    for relative in ['core/aud/renderer.c', 'core/aud/driver.c', 'core/aud/aapi_media.c',
                     'core/aud/block_manager.c', 'include/aud_kapi_renderer.h']:
        filename = source_directory / relative
        metadata[relative] = {'path': str(filename), 'sha256': digest(filename), 'size': filename.stat().st_size}
    (OUT / 'artifacts.json').write_text(json.dumps(metadata, indent=2) + '\n')
    image = ARTIFACTS['kernel_image'].read_bytes()
    (OUT / 'kernel-identity.json').write_text(json.dumps(kernel_identity(image), indent=2) + '\n')
    for name, (artifact, start, end) in RANGES.items():
        arguments = [str(LLVM / 'llvm-objdump'), '-d', f'--start-address={start}', f'--stop-address={end}']
        if artifact == 'kadaptor':
            arguments.append('--triple=thumbv7-linux-gnueabi')
        output_command(name + '.asm', arguments + [str(ARTIFACTS[artifact])])
    output_command('notification-dwarf.txt', [str(LLVM / 'llvm-dwarfdump'), '--debug-info=0x186d0',
                                           '--show-children', str(ARTIFACTS['sink'])])
    output_command('notification-typedef.txt', [str(LLVM / 'llvm-dwarfdump'), '--name=aren_msgpid_to_signal_t', str(ARTIFACTS['sink'])])
    output_command('sink-layout.txt', [str(LLVM / 'llvm-readelf'), '-h', '-l', str(ARTIFACTS['sink'])])
    sink = ARTIFACTS['sink'].read_bytes()
    literals = {}
    for load, addition, literal in [(0x97b0, 0x97b8, 0x9d84), (0x88ea, 0x88f4, 0x8f20),
                                    (0x89a0, 0x89a8, 0x8f44), (0x9084, 0x908a, 0x9c20)]:
        address = addition + 4 + struct.unpack_from('<I', sink, literal)[0]
        literals[hex(load)] = {'raw_address': hex(address), 'ghidra_address': hex(address + 0x10000),
                              'string': sink[address:sink.index(0, address)].decode()}
    strings = {}
    for address in [0x1562da0, 0x185d338, 0x185b0f8, 0x185a968, 0x185a978,
                    0x185a980, 0x189a458, 0x171c140, 0x185aff0]:
        strings[hex(address)] = image[address:image.index(0, address)].decode()
    checks = {'sink_literals': literals, 'kernel_strings': strings,
              'notification_ioctl': {'value': '0x4008522b', 'direction': (0x4008522b >> 30) & 3,
                                     'size': (0x4008522b >> 16) & 0x3fff,
                                     'type': (0x4008522b >> 8) & 255, 'number': 0x4008522b & 255},
              'branch_witness': branch_witness(image, 0x4008522b)}
    assert checks['notification_ioctl'] == {'value': '0x4008522b', 'direction': 1, 'size': 8, 'type': 82, 'number': 43}
    assert literals['0x97b0']['string'] == 'es'
    assert literals['0x88ea']['string'] == 'BYPASS'
    assert strings['0x1562da0'] == 'driver_aren_ioctl'
    assert strings['0x185d338'] == 'Invalid IOCTL Call!!! %x\n'
    (OUT / 'address-checks.json').write_text(json.dumps(checks, indent=2) + '\n')
    print(json.dumps({'artifacts': len(metadata), 'disassembly_slices': len(RANGES),
                      'kernel_identity': '140/140 blocks exact', 'notification': checks['notification_ioctl'],
                      'branch_witness': checks['branch_witness']['result'], 'caps_literals': literals}, indent=2))


if __name__ == '__main__':
    collect()