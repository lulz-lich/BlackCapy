#!/usr/bin/env python3
"""Create a BlackCapy release package from the latest PlatformIO build."""

from __future__ import annotations

import argparse
import hashlib
import re
import shutil
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BUILD_DIR = ROOT / ".pio" / "build" / "esp32dev"
DEFAULT_OUTPUT = ROOT / ".pio" / "blackcapy_release"
BLACKCAPY_H = ROOT / "main" / "firmware" / "include" / "blackcapy.h"

FIRMWARE_FILES = (
    "bootloader.bin",
    "partitions.bin",
    "firmware.bin",
)


def firmware_version() -> str:
    text = BLACKCAPY_H.read_text(encoding="utf-8")
    match = re.search(r'#define\s+BLACKCAPY_VERSION\s+"([^"]+)"', text)
    return match.group(1) if match else "unknown"


def git_commit() -> str:
    try:
        return subprocess.check_output(
            ["git", "rev-parse", "--short", "HEAD"],
            cwd=ROOT,
            text=True,
            stderr=subprocess.DEVNULL,
        ).strip()
    except subprocess.CalledProcessError:
        return "unknown"


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def write_checksums(output: Path) -> None:
    lines: list[str] = []
    for path in sorted(output.rglob("*")):
        if path.is_file() and path.name != "SHA256SUMS":
            lines.append(f"{sha256(path)}  {path.relative_to(output)}")

    (output / "SHA256SUMS").write_text("\n".join(lines) + "\n", encoding="utf-8")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Package BlackCapy release artifacts.")
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT, help="Release package output directory.")
    parser.add_argument("--clean", action="store_true", help="Remove the output directory before packaging.")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    output = args.output.resolve()

    missing = [name for name in FIRMWARE_FILES if not (BUILD_DIR / name).exists()]
    if missing:
        print("Missing build artifacts: " + ", ".join(missing))
        print("Run scripts/build.sh or scripts/release_gate.py first.")
        return 1

    if args.clean and output.exists():
        shutil.rmtree(output)

    firmware_dir = output / "firmware"
    firmware_dir.mkdir(parents=True, exist_ok=True)

    for name in FIRMWARE_FILES:
        shutil.copy2(BUILD_DIR / name, firmware_dir / name)

    microsd_dir = output / "microsd"
    subprocess.check_call(
        ["python3", "scripts/generate_assets.py", "--clean", "--output", str(microsd_dir)],
        cwd=ROOT,
    )

    release_text = "\n".join(
        [
            "BlackCapy Release Package",
            f"Version: {firmware_version()}",
            f"Commit: {git_commit()}",
            "",
            "Firmware:",
            "  firmware/bootloader.bin",
            "  firmware/partitions.bin",
            "  firmware/firmware.bin",
            "",
            "microSD tree:",
            "  microsd/",
            "",
            "Verify integrity with SHA256SUMS before flashing or copying to media.",
        ]
    )
    (output / "RELEASE.txt").write_text(release_text + "\n", encoding="utf-8")

    write_checksums(output)
    print(f"Release package ready: {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
