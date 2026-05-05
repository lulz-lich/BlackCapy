# BlackCapy UI Customization

BlackCapy UI customization is intentionally lightweight in the firmware.

The current goal is to support operator-friendly themes and assets without slowing the device or coupling UI choices to business logic.

---

## Current Customization Points

### Runtime Theme

Themes are handled by `DisplayManager`.

Available themes:

```txt
dark
light
terminal
matrix
minimal
```

Serial shell:

```txt
theme
theme dark
theme matrix
```

Physical UI:

```txt
Settings -> UP/DOWN
```

Theme selection is persisted in internal `Storage`.

---

## microSD Assets

UI assets are loaded from native microSD:

```txt
/assets/icons/
/assets/logo/
/assets/animations/
/themes/
```

Generate a development tree:

```bash
scripts/generate_assets.py --clean
```

Validate assets:

```bash
scripts/validate_assets.py
```

---

## Text Bitmap Assets

Current project `.bmp` files are BlackCapy text bitmaps, not binary BMP files.

Rules:

* Comment lines begin with `#`
* Pixel rows use only `0` and `1`
* All pixel rows in one file must have the same width
* Width and height are detected by the loader

For lightweight animation, use `.anim` files under `/assets/animations/`.
Each frame uses the same `0` and `1` row format, and frames are separated by a
line containing only `---`.

Artists can work in common tools and export normal image files. Convert them with:

```bash
python3 -m pip install pillow
scripts/convert_art.py art/icon.png -o assets/icons/icon.bmp --max-width 32 --max-height 32
scripts/convert_art.py art/boot.gif -o assets/animations/boot.anim --max-width 96 --max-height 64
```

Supported converter inputs:

```txt
Static: PNG, BMP, JPG, JPEG, WEBP
Animated: GIF, APNG
```

---

## Performance Rules

UI customization must not:

* Block input handling
* Allocate large persistent buffers
* Change app behavior
* Replace Logger or CaptureWriter responsibilities
* Make serial the permanent primary interface

Rendering should stay cheap and predictable.

---

## Future Customization

Planned areas:

* Persist selected theme in `Storage`
* Load custom theme configs from `/themes`
* Support static ASCII assets
* Support animated ASCII/pixel assets with `.anim` files
* Support static pixel art
* Add theme-aware color palettes
