#!/usr/bin/env python3
"""Convert common artwork formats into BlackCapy text assets.

Static images become BlackCapy text-bitmaps (`.bmp` project convention).
Animated images such as GIFs become `.anim` frame files.
"""

from __future__ import annotations

import argparse
from pathlib import Path


STATIC_EXTENSIONS = {".png", ".bmp", ".jpg", ".jpeg", ".webp"}
ANIMATION_EXTENSIONS = {".gif", ".apng"}


def load_pillow():
    try:
        from PIL import Image, ImageSequence
    except ImportError as exc:
        raise SystemExit(
            "Pillow is required for art conversion. Install it with: "
            "python3 -m pip install pillow"
        ) from exc

    return Image, ImageSequence


def image_to_rows(image, threshold: int, invert: bool, max_width: int | None, max_height: int | None) -> list[str]:
    image = image.convert("RGBA")

    if max_width or max_height:
      image.thumbnail((max_width or image.width, max_height or image.height))

    rows: list[str] = []
    pixels = image.load()

    for y in range(image.height):
        row = []
        for x in range(image.width):
            red, green, blue, alpha = pixels[x, y]
            if alpha == 0:
                enabled = False
            else:
                luminance = int((red * 0.299) + (green * 0.587) + (blue * 0.114))
                enabled = luminance < threshold

            if invert:
                enabled = not enabled

            row.append("1" if enabled else "0")

        rows.append("".join(row))

    return rows


def write_static_asset(input_path: Path, output_path: Path, rows: list[str]) -> None:
    content = [
        f"# Converted from {input_path.name}",
        "# BlackCapy text bitmap: 1 = pixel on, 0 = pixel off",
        *rows,
    ]
    output_path.write_text("\n".join(content) + "\n", encoding="utf-8")


def write_animation_asset(input_path: Path, output_path: Path, frames: list[list[str]]) -> None:
    content = [
        f"# Converted from {input_path.name}",
        "# BlackCapy text animation: frames separated by ---",
    ]

    for index, rows in enumerate(frames):
        if index > 0:
            content.append("---")
        content.extend(rows)

    output_path.write_text("\n".join(content) + "\n", encoding="utf-8")


def default_output_path(input_path: Path, animated: bool) -> Path:
    suffix = ".anim" if animated else ".bmp"
    return input_path.with_suffix(suffix)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Convert PNG/BMP/JPG/GIF art to BlackCapy text assets.")
    parser.add_argument("input", type=Path, help="Input image. PNG/BMP/JPG/WEBP become .bmp; GIF/APNG become .anim.")
    parser.add_argument("-o", "--output", type=Path, help="Output asset path.")
    parser.add_argument("--threshold", type=int, default=128, help="Luminance threshold for monochrome conversion.")
    parser.add_argument("--invert", action="store_true", help="Invert generated pixels.")
    parser.add_argument("--max-width", type=int, help="Resize input to fit this width before conversion.")
    parser.add_argument("--max-height", type=int, help="Resize input to fit this height before conversion.")
    parser.add_argument(
        "--mode",
        choices=("auto", "static", "animation"),
        default="auto",
        help="Force static or animation output. Defaults to extension-based auto detection.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    input_path = args.input.resolve()

    if not input_path.exists():
        print(f"Input file not found: {input_path}")
        return 1

    Image, ImageSequence = load_pillow()
    suffix = input_path.suffix.lower()

    animated = suffix in ANIMATION_EXTENSIONS
    if args.mode == "static":
        animated = False
    elif args.mode == "animation":
        animated = True
    elif suffix not in STATIC_EXTENSIONS and suffix not in ANIMATION_EXTENSIONS:
        print(f"Unsupported extension: {suffix}")
        return 1

    output_path = (args.output or default_output_path(input_path, animated)).resolve()
    output_path.parent.mkdir(parents=True, exist_ok=True)

    with Image.open(input_path) as image:
        if animated:
            frames: list[list[str]] = []
            for frame in ImageSequence.Iterator(image):
                frames.append(
                    image_to_rows(frame, args.threshold, args.invert, args.max_width, args.max_height)
                )

            if not frames:
                print("No frames found.")
                return 1

            write_animation_asset(input_path, output_path, frames)
            print(f"Wrote {len(frames)} frames -> {output_path}")
        else:
            rows = image_to_rows(image, args.threshold, args.invert, args.max_width, args.max_height)
            write_static_asset(input_path, output_path, rows)
            print(f"Wrote {len(rows[0])}x{len(rows)} bitmap -> {output_path}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
