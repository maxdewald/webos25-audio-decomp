import hashlib
import json
import os
import subprocess
from pathlib import Path


OUTPUT = Path(__file__).resolve().parent
ROOT = OUTPUT.parent.parent
GHIDRA = Path("/opt/homebrew/opt/ghidra/libexec")
PROJECT_NAME = "audio_a0_dsp2.main_60000000.bin"
PROJECT = ROOT / "build/ghidra" / PROJECT_NAME


def snapshot():
    result = {}
    for file_path in sorted(PROJECT.rglob("*")):
        if file_path.is_file():
            result[str(file_path.relative_to(PROJECT))] = {
                "size": file_path.stat().st_size,
                "mtime_ns": file_path.stat().st_mtime_ns,
                "sha256": hashlib.sha256(file_path.read_bytes()).hexdigest(),
            }
    return result


def main():
    before = snapshot()
    assert before, "Missing source project"
    for directory in ("ghidra-settings", "java-prefs", "ghidra-cache", "home"):
        (OUTPUT / directory).mkdir(exist_ok=True)
    environment = os.environ.copy()
    environment["JAVA_HOME"] = "/opt/homebrew/opt/openjdk@21"
    environment["HOME"] = str(OUTPUT / "home")
    environment["JAVA_TOOL_OPTIONS"] = " ".join([
        f"-Duser.home={OUTPUT / 'home'}",
        f"-Djava.util.prefs.userRoot={OUTPUT / 'java-prefs'}",
        f"-Dapplication.settingsdir={OUTPUT / 'ghidra-settings'}",
        f"-Dapplication.cachedir={OUTPUT / 'ghidra-cache'}",
    ])
    command = [str(GHIDRA / "support/analyzeHeadless"), str(PROJECT), PROJECT_NAME,
               "-process", PROJECT_NAME, "-readOnly", "-noanalysis", "-max-cpu", "2",
               "-scriptPath", f"{OUTPUT};{ROOT / 'tools'}", "-postScript", "EarcDsp2Export.java", str(OUTPUT),
               "-log", str(OUTPUT / "headless.log"), "-scriptlog", str(OUTPUT / "script.log")]
    record = {"command": command, "scoped_environment": {name: environment[name] for name in
              ("JAVA_HOME", "HOME", "JAVA_TOOL_OPTIONS")}, "before": before}
    try:
        completed = subprocess.run(command, env=environment, timeout=240, check=False)
        record["returncode"] = completed.returncode
    finally:
        record["after"] = snapshot()
        record["source_project_unchanged"] = record["before"] == record["after"]
        (OUTPUT / "headless-provenance.json").write_text(json.dumps(record, indent=2) + "\n")
    assert record["source_project_unchanged"], "Source Ghidra project changed"
    assert record["returncode"] == 0, "Ghidra failed"
    assert (OUTPUT / "assembly-coverage.tsv").exists(), "No assembly coverage export"
    print("EARC_DSP2_HEADLESS_OK source_project_unchanged=true")


if __name__ == "__main__":
    main()