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
    def snapshot(self):
        target = progress.Target('example', Path('/missing'), 'arm', None, None, 0, 0)
        target.funcs = {
            1: progress.Func(1, 100, 'first', src='first.c:1', match=90.123456),
            2: progress.Func(2, 100, 'second', src='second.c:1', match=50),
        }
        return progress.make_snapshot([target])

    def test_snapshot_rejects_individual_loss_despite_aggregate_gain(self):
        before, after = self.snapshot(), self.snapshot()
        functions = after['targets']['example']['functions']
        functions['0x1']['match'] -= 0.000001
        functions['0x2']['match'] = 100
        self.assertTrue(any('match decreased' in failure for failure in progress.compare_snapshot(before, after)))

    def test_snapshot_rejects_missing_function_and_unmeasured_match(self):
        before, after = self.snapshot(), self.snapshot()
        functions = after['targets']['example']['functions']
        del functions['0x1']
        functions['0x2']['match'] = None
        failures = progress.compare_snapshot(before, after)
        self.assertTrue(any('reconstruction disappeared' in failure for failure in failures))
        self.assertTrue(any('no longer measured' in failure for failure in failures))

    def test_snapshot_accepts_improvements_and_new_coverage(self):
        before, after = self.snapshot(), self.snapshot()
        functions = after['targets']['example']['functions']
        functions['0x1']['match'] = 100
        functions['0x3'] = dict(functions['0x2'])
        self.assertEqual(progress.compare_snapshot(before, after), [])

    def test_snapshot_rejects_changed_measurement_inputs(self):
        before, after = self.snapshot(), self.snapshot()
        after['targets']['example']['cflags_extra'] = ['-O0']
        self.assertIn('example: cflags_extra changed', progress.compare_snapshot(before, after))

    def test_snapshot_rejects_zero_score_emission_loss(self):
        before, after = self.snapshot(), self.snapshot()
        before['targets']['example']['functions']['0x1']['match'] = 0
        updated = after['targets']['example']['functions']['0x1']
        updated['match'] = 0
        updated['note'] = 'not emitted (inlined or failed to compile)'
        self.assertTrue(any('no longer emitted' in failure for failure in progress.compare_snapshot(before, after)))

    def test_snapshot_rejects_missing_target(self):
        before, after = self.snapshot(), self.snapshot()
        del after['targets']['example']
        self.assertEqual(progress.compare_snapshot(before, after), ['example: target disappeared'])

    def test_snapshot_rejects_changed_binary_and_lost_coverage(self):
        before, after = self.snapshot(), self.snapshot()
        target = after['targets']['example']
        target['binary_sha256'] = 'changed'
        target['summary']['reconstructed_bytes'] -= 1
        failures = progress.compare_snapshot(before, after)
        self.assertIn('example: binary_sha256 changed', failures)
        self.assertIn('example: reconstructed bytes decreased', failures)

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
