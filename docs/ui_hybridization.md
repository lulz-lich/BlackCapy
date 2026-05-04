# BlackCapy Hybrid UI

BlackCapy uses a hybrid UI by default.

ASCII is for technical data, logs, menus and diagnostics. Pixel art is for splash screens, icons, status and visual context.

Performance comes before aesthetics.

---

## Current State

Current rendering is serial fallback plus text-bitmap asset loading from native microSD.

The firmware can load simple text-based bitmap assets from:

```txt
/assets/icons/
/assets/logo/
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

Validate assets with:

```bash
scripts/validate_assets.py
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

These functions attempt asset rendering and keep ASCII output available for serial fallback.

---

## Future Work

Planned evolution:

* Real 320x240 display backend
* Pixel art without serial duplication
* Static ASCII assets
* Animated ASCII assets
* Static pixel art assets
* Animated pixel art assets
* Theme-aware palettes
* Asset caching only where it improves performance
