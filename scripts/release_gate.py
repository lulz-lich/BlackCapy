#!/usr/bin/env python3
"""Run BlackCapy release-readiness checks."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
HARDWARE_CONFIG = ROOT / "main" / "firmware" / "include" / "hardware_config.h"
BLACKCAPY_H = ROOT / "main" / "firmware" / "include" / "blackcapy.h"


REQUIRED_DEFINES = {
    "BLACKCAPY_DISPLAY_BACKEND_TFT": "1",
    "BLACKCAPY_INPUT_BACKEND_ADC": "1",
    "BLACKCAPY_ENABLE_MODULE_MOCKS": "0",
}


def run(command: list[str]) -> int:
    print("+ " + " ".join(command))
    return subprocess.call(command, cwd=ROOT)


def parse_defines(path: Path) -> dict[str, str]:
    defines: dict[str, str] = {}
    pattern = re.compile(r"^#define\s+([A-Z0-9_]+)\s+(.+?)\s*$")

    for line in path.read_text(encoding="utf-8").splitlines():
        match = pattern.match(line.strip())
        if match:
            defines[match.group(1)] = match.group(2).strip().strip('"')

    return defines


def validate_release_defines() -> list[str]:
    errors: list[str] = []
    defines = parse_defines(HARDWARE_CONFIG)
    blackcapy_defines = parse_defines(BLACKCAPY_H)

    version = blackcapy_defines.get("BLACKCAPY_VERSION", "")
    if not re.fullmatch(r"\d+\.\d+\.\d+", version):
        errors.append("BLACKCAPY_VERSION must be a stable semantic version like 1.0.0.")

    for name, expected in REQUIRED_DEFINES.items():
        actual = defines.get(name)
        if actual != expected:
            errors.append(f"{name} must be {expected}, got {actual!r}.")

    return errors


def main() -> int:
    errors = validate_release_defines()
    if errors:
        print("Release define validation failed:")
        for error in errors:
            print(f"- {error}")
        return 1

    commands = [
        ["python3", "scripts/validate_apps.py"],
        ["python3", "scripts/validate_assets.py"],
        ["python3", "scripts/validate_ai.py"],
        ["python3", "scripts/validate_docs.py"],
        ["python3", "scripts/validate_architecture.py"],
        ["python3", "scripts/validate_platformio.py"],
        ["python3", "scripts/validate_gitignore.py"],
        ["python3", "scripts/check_hardware_config.py", "--strict"],
        ["git", "diff", "--check"],
        ["pio", "run", "-e", "esp32dev"],
        ["python3", "scripts/package_release.py", "--clean"],
    ]

    for command in commands:
        code = run(command)
        if code != 0:
            return code

    print("Release gate passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
