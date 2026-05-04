#!/usr/bin/env python3
"""Prepare a microSD directory tree for BlackCapy.

The firmware creates these directories at runtime, but this script gives you a
ready-to-copy tree for development cards and hardware tests.
"""

from __future__ import annotations

import argparse
import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_OUTPUT = ROOT / ".pio" / "blackcapy_sdcard"
SD_DIRS = ("logs", "assets", "scripts", "captures", "modules", "themes", "plugins")


def copy_file(src: Path, dst: Path) -> None:
    dst.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, dst)
    print(f"Copied {src.relative_to(ROOT)} -> {dst.relative_to(ROOT)}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate BlackCapy microSD assets.")
    parser.add_argument(
        "--output",
        type=Path,
        default=DEFAULT_OUTPUT,
        help="Output directory. Defaults to .pio/blackcapy_sdcard.",
    )
    parser.add_argument(
        "--clean",
        action="store_true",
        help="Remove the output directory before generating it.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    output = args.output.resolve()

    if args.clean and output.exists():
        shutil.rmtree(output)

    for dirname in SD_DIRS:
        (output / dirname).mkdir(parents=True, exist_ok=True)

    assets_dir = ROOT / "assets"
    if assets_dir.exists():
        for src in assets_dir.rglob("*"):
            if not src.is_file() or src.name == ".gitkeep":
                continue

            if src.parent == assets_dir and src.suffix in {".txt", ".cfg"}:
                copy_file(src, output / "scripts" / src.name)
            elif src.parent.name == "themes":
                copy_file(src, output / "themes" / src.name)
            else:
                copy_file(src, output / "assets" / src.relative_to(assets_dir))

    scripts_dir = ROOT / "scripts"
    for src in scripts_dir.glob("*.txt"):
        copy_file(src, output / "scripts" / src.name)

    print()
    print(f"microSD tree ready at: {output}")
    print("Expected root directories: " + ", ".join(f"/{d}" for d in SD_DIRS))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
