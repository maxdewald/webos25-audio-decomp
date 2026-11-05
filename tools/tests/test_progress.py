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
            self.assertIn('>not measured</text>', svg)
            self.assertIn('instruction match not measured', svg)
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

    def test_measured_zero_remains_distinct_from_unmeasured(self):
        target = progress.Target('zero', Path('/missing'), 'arm', None, None, 0, 0)
        target.funcs = {1: progress.Func(1, 100, 'zero', src='zero.c:1', match=0)}
        svg = progress.progress_svg([progress.summary(target) | {'path': '/original'}])
        self.assertIn('>0.0% match</text>', svg)
        self.assertNotIn('>not measured</text>', svg)
        self.assertNotIn('<rect class="match" x="330"', svg)

    def test_failed_build_is_not_zero_and_does_not_block_other_targets(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            binary = root / 'original'
            binary.touch()
            failed = progress.Target('failed', binary, 'arm', None, root, 0, 0)
            healthy = progress.Target('healthy', binary, 'arm', None, root, 0, 0)
            failed.funcs = {1: progress.Func(1, 100, 'failed', src='failed.c:1', match=90)}
            healthy.funcs = {2: progress.Func(2, 100, 'healthy', src='healthy.c:1')}
            def score(target):
                target.funcs[2].match = 75
            with patch.object(progress, 'compile_targets', side_effect=[False, True]) as compile_targets, patch.object(progress, 'score', side_effect=score) as scorer:
                self.assertFalse(progress.score_all([failed, healthy]))
            self.assertEqual(compile_targets.call_args_list[0].args, ([failed],))
            self.assertEqual(compile_targets.call_args_list[1].args, ([healthy],))
            scorer.assert_called_once_with(healthy)
            self.assertIsNone(progress.summary(failed)['match_pct'])
            self.assertEqual(progress.summary(failed)['match_status'], 'build failed')
            self.assertEqual(progress.summary(healthy)['match_pct'], 75)
            rows = [progress.summary(target) | {'path': '/original'} for target in [failed, healthy]]
            svg = progress.progress_svg(rows)
            self.assertIn('>build failed</text>', svg)
            self.assertIn('75.0% match', svg)
            self.assertNotIn('0.0% match', svg)


if __name__ == '__main__':
    unittest.main()
