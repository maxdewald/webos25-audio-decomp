#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_umi_policy_match.py; focused LG ARM compile/match regression."""
import importlib.util
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[2]


def main():
    spec = importlib.util.spec_from_file_location('umi_progress', ROOT / 'tools/progress.py')
    progress = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = progress
    spec.loader.exec_module(progress)
    target = progress.find(progress.load_targets(), 'libumi')
    assert progress.compile_targets([target]), 'libumi target compilation failed'
    progress.score(target)
    target.match_status = 'measured'
    # Before this family expansion, only these nine addresses were credited;
    # five were optimized away. Preserve every baseline instruction match.
    baseline = {0x41b64: 0, 0x49440: 0, 0x498a0: 0,
                0x4fc24: 9.580838323353294, 0x4ff14: 0,
                0x50180: 4.395604395604396, 0x501a0: 0,
                0x50978: 24, 0x526f4: 1.9607843137254901}
    for address, minimum in baseline.items():
        function = target.funcs[address]
        assert function.src and function.match >= minimum, (function.name, function.match, minimum)
    family = [function for function in target.funcs.values() if function.src]
    assert len(family) >= 40
    assert all(not function.note for function in family), 'tagged function not emitted'
    print(json.dumps(progress.summary(target), indent=2))
    print('UMI focused match: all nine pre-existing function scores preserved')


if __name__ == '__main__':
    main()
