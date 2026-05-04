#!/usr/bin/env python3
"""Validate BlackCapy documentation basics."""

from __future__ import annotations

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DOC_PATHS = [
    ROOT / "README.md",
    ROOT / "usage.md",
    *sorted((ROOT / "docs").glob("*.md")),
    *sorted((ROOT / "hardware").glob("*.md")),
    *sorted((ROOT / "hardware" / "pinout").glob("*.md")),
]

FORBIDDEN_PHRASES = (
    "Tool registry system",
    "Tool registry initialized",
    "main/core/registry",
)


def main() -> int:
    errors: list[str] = []

    for path in DOC_PATHS:
        if not path.exists():
            errors.append(f"Missing doc: {path.relative_to(ROOT)}")
            continue

        text = path.read_text(encoding="utf-8")
        if not text.strip():
            errors.append(f"Empty doc: {path.relative_to(ROOT)}")
            continue

        for phrase in FORBIDDEN_PHRASES:
            if phrase in text:
                errors.append(f"{path.relative_to(ROOT)} contains obsolete phrase: {phrase}")

    if errors:
        print("Documentation validation failed:")
        for error in errors:
            print(f"- {error}")
        return 1

    print(f"Documentation validation passed: {len(DOC_PATHS)} files.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
