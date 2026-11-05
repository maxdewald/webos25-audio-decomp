#!/usr/bin/env python3
"""Fetch pinned public reference sources: fetch-public-sources.py [name ...]."""
import concurrent.futures
import hashlib
import json
from pathlib import Path, PurePosixPath
import shutil
import subprocess
import sys
import tarfile
import tempfile

ROOT = Path(__file__).resolve().parent.parent
MANIFEST = ROOT / 'tools/public-sources.json'


def fetch(name, entry):
    parent = ROOT / 'lg-opensource/public'
    destination = parent / name
    identity = json.dumps(entry, sort_keys=True) + '\n'
    marker = destination / '.source.json'
    if marker.is_file() and marker.read_text() == identity:
        print(f'cached {name} {entry["revision"]}', flush=True)
        return
    if destination.exists():
        raise RuntimeError(f'{destination} exists but does not match the pinned source')
    cache = ROOT / 'lg-opensource/.download/public'
    cache.mkdir(parents=True, exist_ok=True)
    archive = cache / f'{name}-{entry["revision"]}.tar'
    if not archive.is_file():
        partial = archive.with_suffix('.partial')
        try:
            subprocess.run(['curl', '-fsSL', '--connect-timeout', '15', '--max-time', '300',
                            '--retry', '2', '-o', str(partial), entry['url']], check=True)
            partial.replace(archive)
        finally:
            partial.unlink(missing_ok=True)
    digest = hashlib.sha256(archive.read_bytes()).hexdigest()
    expected = entry.get('sha256')
    if expected and digest != expected:
        raise RuntimeError(f'{name}: SHA-256 mismatch: {digest} != {expected}')
    if not expected:
        raise RuntimeError(f'{name}: manifest needs archive SHA-256 {digest}')
    parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix=f'.{name}-', dir=parent) as temporary:
        temporary = Path(temporary)
        with tarfile.open(archive) as source:
            members = source.getmembers()
            for member in members:
                parts = PurePosixPath(member.name).parts
                if member.name.startswith('/') or '..' in parts or not (
                        member.name == entry['prefix'].rstrip('/') or member.name.startswith(entry['prefix'])):
                    raise RuntimeError(f'{name}: unexpected archive member {member.name}')
            source.extractall(temporary, filter='data')
        extracted = temporary / entry['prefix'].rstrip('/')
        (extracted / '.source.json').write_text(identity)
        shutil.move(extracted, destination)
    print(f'fetched {name} {entry["revision"]}', flush=True)


def main():
    manifest = json.loads(MANIFEST.read_text())
    names = sys.argv[1:] or list(manifest)
    for name in names:
        if name not in manifest:
            sys.exit(f'unknown source {name}')
    with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
        futures = [pool.submit(fetch, name, manifest[name]) for name in names]
        for future in futures:
            future.result()


if __name__ == '__main__':
    main()
