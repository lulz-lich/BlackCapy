# BlackCapy Hybrid UI

BlackCapy uses a hybrid UI by default.

ASCII is for technical data, logs, menus and diagnostics. Pixel art is for splash screens, icons, status and visual context.

Performance comes before aesthetics.

---

## Current State

Current rendering uses the real SPI TFT backend plus serial debug mirroring.
Text-bitmap assets load from native microSD.

The firmware can load simple text-based bitmap assets from:

```txt
/assets/icons/
/assets/logo/
/assets/animations/
```

The local development generator prepares those paths with:

```bash
scripts/generate_assets.py --clean
```

---

## Bitmap Format

BlackCapy bitmap assets currently use text rows of `0` and `1`.

Example:

```txt
# Comment lines are ignored
0000000000000000
0000011111000000
0001111111110000
0000011111000000
0000000000000000
```

Rules:

* All non-comment rows must have the same width
* Only `0` and `1` are accepted as pixel data
* Width and height are detected by the loader
* Files use `.bmp` names for project convention, but are text bitmaps today
* Use `scripts/convert_art.py` to convert normal PNG/BMP/JPG/WEBP art into this format

Animated pixel art uses `.anim` files with the same `0` and `1` rows. Separate
frames with a line containing only:

```txt
---
```

Example:

```txt
001100
011110
001100
---
000000
011110
000000
```

Validate assets with:

```bash
scripts/validate_assets.py
```

Convert artwork with:

```bash
python3 -m pip install pillow
scripts/convert_art.py art/icon.png -o assets/icons/icon.bmp --max-width 32 --max-height 32
scripts/convert_art.py art/boot.gif -o assets/animations/boot.anim --max-width 96 --max-height 64
```

---

## Hybrid Functions

Current helper functions:

```cpp
uiShowLogoHybrid();
uiShowIconWiFiHybrid();
uiShowIconBLEHybrid();
uiShowIconGPIOHybrid();
uiShowIconRFHybrid();
uiShowIconSubGHzHybrid();
```

These functions render assets through the TFT backend and keep ASCII output available for serial debug mirroring.

Animation rendering is available through:

```cpp
displayDrawAnimationFromFile(0, 0, "animations/boot_pulse.anim", 120, 2);
displayDrawIconFromFileScaled(0, 0, "icons/wifi_icon.bmp", 2);
displayDrawAnimationFromFileScaled(0, 0, "animations/boot_pulse.anim", 120, 2, 3);
```

---

## Future Work

Planned evolution:

* Hardware-specific polish for the selected target board
* Optional serial mirror toggle
* Static ASCII assets
* Animated ASCII/pixel assets through `.anim` frame files
* Static pixel art assets
* Animated pixel art assets
* Theme-aware palettes
* Asset caching only where it improves performance
