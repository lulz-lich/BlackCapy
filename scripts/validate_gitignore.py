#!/usr/bin/env python3
"""Validate repository ignore rules needed by BlackCapy tooling."""

from __future__ import annotations

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
GITIGNORE = ROOT / ".gitignore"

REQUIRED_PATTERNS = {
    ".pio/",
    ".vscode/",
    ".env",
    "__pycache__/",
    "*.py[cod]",
}


def main() -> int:
    if not GITIGNORE.exists():
        print("Gitignore validation failed:")
        print("- Missing .gitignore")
        return 1

    patterns = {
        line.strip()
        for line in GITIGNORE.read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.strip().startswith("#")
    }

    missing = sorted(REQUIRED_PATTERNS - patterns)
    if missing:
        print("Gitignore validation failed:")
        for pattern in missing:
            print(f"- Missing required pattern: {pattern}")
        return 1

    print("Gitignore validation passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
