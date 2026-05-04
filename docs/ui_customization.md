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

Theme persistence is future work.

---

## microSD Assets

UI assets are loaded from native microSD:

```txt
/assets/icons/
/assets/logo/
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

Current `.bmp` files are text bitmaps, not binary BMP files.

Rules:

* Comment lines begin with `#`
* Pixel rows use only `0` and `1`
* All pixel rows in one file must have the same width
* Width and height are detected by the loader

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
* Support animated ASCII assets
* Support static pixel art
* Support animated pixel art
* Add real 320x240 display rendering
* Add theme-aware color palettes
