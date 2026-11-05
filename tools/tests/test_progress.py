#!/usr/bin/env python3
"""Progress rendering regressions: python3 tools/tests/test_progress.py."""
import argparse
import importlib.util
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location('progress', ROOT / 'tools/progress.py')
progress = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = progress
spec.loader.exec_module(progress)


class ProgressTests(unittest.TestCase):
    def test_coverage_only_svg_does_not_compile_or_claim_match(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            target = progress.Target('example', root / 'orig/example', 'arm', None, root / 'src', 0, 0)
            target.funcs = {1: progress.Func(1, 100, 'example', src='src/example.c:1')}
            with patch.object(progress, 'ROOT', root), patch.object(progress, 'load_targets', return_value=[target]), patch.object(progress, 'score_all') as score:
                progress.cmd_svg(argparse.Namespace(no_match=True))
                score.assert_not_called()
            svg = (root / 'progress.svg').read_text()
            self.assertIn('100.0%', svg)
            self.assertIn('>arm</text>', svg)
            self.assertNotIn('% match', svg)
            self.assertIn('class="cov" x="330"', svg)

    def test_default_svg_keeps_match_workflow(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            target = progress.Target('example', root / 'orig/example', 'arm', None, root / 'src', 0, 0)
            target.funcs = {1: progress.Func(1, 100, 'example', src='src/example.c:1', match=75)}
            with patch.object(progress, 'ROOT', root), patch.object(progress, 'load_targets', return_value=[target]), patch.object(progress, 'score_all') as score:
                progress.cmd_svg(argparse.Namespace(no_match=False))
                score.assert_called_once_with([target])
            self.assertIn('75.0% match', (root / 'progress.svg').read_text())


if __name__ == '__main__':
    unittest.main()
