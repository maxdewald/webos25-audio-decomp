#!/usr/bin/env python3
"""Offline tests for pinned public sources: python3 tools/tests/test_public_sources.py."""
import contextlib
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import tarfile
import tempfile
import unittest
from unittest.mock import patch
import tomllib

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location('public_sources', ROOT / 'tools/fetch-public-sources.py')
sources = importlib.util.module_from_spec(spec)
spec.loader.exec_module(sources)


class PublicSourcesTests(unittest.TestCase):
    def test_manifest_references(self):
        manifest = json.loads(sources.MANIFEST.read_text())
        targets = {target['name'] for target in tomllib.loads((ROOT / 'decomp.toml').read_text())['target']}
        for name, entry in manifest.items():
            with self.subTest(source=name):
                self.assertEqual(len(entry['sha256']), 64)
                self.assertTrue(entry['license'])
                self.assertTrue(entry['prefix'].endswith('/'))
                for target, paths in entry.get('targets', {}).items():
                    self.assertIn(target, targets)
                    self.assertTrue(paths)
                    for path in paths:
                        self.assertFalse(Path(path).is_absolute())
                        self.assertNotIn('..', Path(path).parts)

    def test_reference_metadata_does_not_invalidate_source(self):
        entry = {'revision': 'test', 'url': 'https://example.invalid/source.tar',
                 'sha256': '0' * 64, 'prefix': 'source/', 'license': 'MIT'}
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            destination = root / 'lg-opensource/public/example'
            destination.mkdir(parents=True)
            (destination / '.source.json').write_text(json.dumps(entry, sort_keys=True) + '\n')
            with patch.object(sources, 'ROOT', root), patch.object(sources.subprocess, 'run') as run:
                sources.fetch('example', entry | {'targets': {'libpf': ['include']}})
                run.assert_not_called()

    def test_fetch_retains_license_and_uses_archive_digest(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            cache = root / 'lg-opensource/.download/public'
            cache.mkdir(parents=True)
            archive = cache / 'example-test.tar'
            with tarfile.open(archive, 'w') as output:
                for name, data in [('LICENSE', b'Example license\n'), ('source.c', b'int f(void);\n')]:
                    member = tarfile.TarInfo('source/' + name)
                    member.size = len(data)
                    output.addfile(member, io.BytesIO(data))
            entry = {'revision': 'test', 'url': 'https://example.invalid/source.tar',
                     'sha256': hashlib.sha256(archive.read_bytes()).hexdigest(),
                     'prefix': 'source/', 'license': 'MIT', 'targets': {'libpf': ['source.c']}}
            with patch.object(sources, 'ROOT', root), patch.object(sources.subprocess, 'run') as run:
                sources.fetch('example', entry)
                run.assert_not_called()
            destination = root / 'lg-opensource/public/example'
            self.assertEqual((destination / 'LICENSE').read_bytes(), b'Example license\n')
            identity = json.loads((destination / '.source.json').read_text())
            self.assertNotIn('targets', identity)
            self.assertEqual(identity['sha256'], entry['sha256'])

    def test_digest_mismatch_does_not_extract(self):
        entry = {'revision': 'test', 'url': 'https://example.invalid/source.tar',
                 'sha256': '0' * 64, 'prefix': 'source/', 'license': 'MIT'}
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            cache = root / 'lg-opensource/.download/public'
            cache.mkdir(parents=True)
            (cache / 'example-test.tar').write_bytes(b'not the expected archive')
            with patch.object(sources, 'ROOT', root), patch.object(sources.subprocess, 'run') as run:
                with self.assertRaisesRegex(RuntimeError, 'SHA-256 mismatch'):
                    sources.fetch('example', entry)
                run.assert_not_called()
            self.assertFalse((root / 'lg-opensource/public/example').exists())

    def test_list_does_not_fetch_or_claim_verified_coverage(self):
        entry = {'revision': 'test', 'license': 'MIT', 'targets': {'libpf': ['source.c']}}
        with tempfile.TemporaryDirectory() as directory:
            with patch.object(sources, 'ROOT', Path(directory)), patch.object(sources, 'fetch') as fetch:
                output = io.StringIO()
                with contextlib.redirect_stdout(output):
                    sources.list_sources({'example': entry}, ['example'])
                fetch.assert_not_called()
                self.assertIn('libpf', output.getvalue())
                self.assertIn('missing', output.getvalue())
                self.assertIn('not verified binary coverage', output.getvalue())


if __name__ == '__main__':
    unittest.main()
