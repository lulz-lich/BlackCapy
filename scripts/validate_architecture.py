#!/usr/bin/env python3
"""Validate high-level BlackCapy architecture boundaries."""

from __future__ import annotations

from pathlib import Path
import re


ROOT = Path(__file__).resolve().parents[1]
MAIN = ROOT / "main"

SD_TOKENS = ("#include <SD.h>", "SD.open", "SD.exists", "SD.mkdir", "SD.begin")
ALLOWED_SD_PREFIX = Path("main/core/filesystem")
CAPTURE_CATEGORIES = {"ir", "rf", "subghz", "rfid", "nfc", "can", "gps", "lora"}


def relative(path: Path) -> Path:
    return path.relative_to(ROOT)


def main() -> int:
    errors: list[str] = []

    for path in sorted(MAIN.rglob("*")):
        if path.suffix not in {".cpp", ".h"}:
            continue

        rel = relative(path)
        text = path.read_text(encoding="utf-8", errors="ignore")

        if not rel.is_relative_to(ALLOWED_SD_PREFIX):
            for token in SD_TOKENS:
                if token in text:
                    errors.append(f"{rel}: direct SD usage '{token}' outside FileSystem.")

        for match in re.finditer(r'captureWriteLine\s*\(\s*"([^"]+)"', text):
            category = match.group(1)
            if category not in CAPTURE_CATEGORIES:
                errors.append(f"{rel}: unsupported capture category '{category}'.")

    if errors:
        print("Architecture validation failed:")
        for error in errors:
            print(f"- {error}")
        return 1

    print("Architecture validation passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
