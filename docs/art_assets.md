# BlackCapy Art Assets

This guide explains how to add static pixel art and animations to BlackCapy.

BlackCapy uses artist-friendly source files during development and lightweight text assets on the ESP32.

---

## Supported Artist Inputs

Artists can work in common tools such as Aseprite, Piskel, Krita, Photoshop or similar editors.

Converter inputs:

```txt
Static art: PNG, BMP, JPG, JPEG, WEBP
Animation: GIF, APNG
```

Firmware runtime formats:

```txt
.bmp   BlackCapy text bitmap, not binary BMP
.anim  BlackCapy text animation
```

The firmware reads runtime assets from native microSD under:

```txt
/assets/icons/
/assets/logo/
/assets/animations/
```

---

## Convert Static Art

Example:

```bash
python3 -m pip install pillow
scripts/convert_art.py art/wifi.png \
  -o assets/icons/wifi_icon.bmp \
  --max-width 32 \
  --max-height 32
```

For a logo:

```bash
scripts/convert_art.py art/blackcapy_logo.png \
  -o assets/logo/blackcapy_logo.bmp \
  --max-width 96 \
  --max-height 48
```

If the result looks inverted, add:

```bash
--invert
```

If the art has too many or too few lit pixels, adjust:

```bash
--threshold 160
```

Lower threshold values require darker pixels to turn on. Higher values turn on more pixels.

---

## Convert Animation

Export the animation as GIF or APNG, then convert:

```bash
scripts/convert_art.py art/boot.gif \
  -o assets/animations/boot.anim \
  --max-width 96 \
  --max-height 64
```

The converter writes frames separated by:

```txt
---
```

---

## Prepare The microSD Card

After adding or converting assets, run:

```bash
scripts/validate_assets.py
scripts/generate_assets.py --clean
```

Copy the generated tree to the card:

```txt
.pio/blackcapy_sdcard/
```

For packaged releases, `scripts/release_gate.py` creates:

```txt
.pio/blackcapy_release/microsd/
```

---

## Draw Art In Screens

Use screen code when the art belongs to Home, Tools, Status or Settings.

File:

```txt
main/core/screens/screen_manager.cpp
```

Example for Home:

```cpp
static void renderHome() {
  displayClear();
  displayDrawTitle("BlackCapy");
  displayDrawIconFromFileScaled(224, 34, "logo/blackcapy_logo.bmp", 2);
  displayDrawText(0, 40, "Professional Toolkit");
  displayDrawStatusBar("HOME | READY");
  displayRefresh();
}
```

Example for a Tools icon:

```cpp
static void renderToolIcon(int toolId, int x, int y) {
  switch (toolId) {
    case TOOL_WIFI_SCANNER:
      displayDrawIconFromFileScaled(x, y, "icons/wifi_icon.bmp", 2);
      break;
    default:
      displayDrawAsciiFrame(x, y, 20, 6, "TOOL");
      break;
  }
}
```

Paths passed to display functions are relative to `/assets`.

This path:

```cpp
"icons/wifi_icon.bmp"
```

loads:

```txt
/assets/icons/wifi_icon.bmp
```

---

## Draw Art Inside A Tool

Use app code when the art belongs to a tool workflow.

Example:

```cpp
#include "display_manager.h"

void runMyTool() {
  displayClear();
  displayDrawTitle("My Tool");
  displayDrawIconFromFileScaled(12, 34, "icons/gpio_icon.bmp", 2);
  displayDrawText(54, 42, "Reading hardware...");
  displayDrawStatusBar("MY TOOL | RUNNING");
  displayRefresh();

  // Do the actual tool work here.
}
```

For a short progress animation:

```cpp
#include "display_manager.h"

void runMyTool() {
  displayClear();
  displayDrawTitle("My Tool");
  displayDrawText(0, 40, "Starting scan...");
  displayDrawAnimationFromFileScaled(112, 72, "animations/boot_pulse.anim", 100, 2, 3);
  displayDrawStatusBar("SCAN | ACTIVE");
  displayRefresh();
}
```

Keep animations short inside tools. Long blocking animation loops delay input and app flow.

---

## Scale Rules

Scale is an integer pixel multiplier.

```cpp
displayDrawIconFromFileScaled(0, 0, "icons/wifi_icon.bmp", 1); // original size
displayDrawIconFromFileScaled(0, 0, "icons/wifi_icon.bmp", 2); // double size
displayDrawIconFromFileScaled(0, 0, "icons/wifi_icon.bmp", 3); // triple size
```

Good defaults for 320x240:

```txt
16x16 icon at scale 2 -> 32x32 on screen
32x32 icon at scale 2 -> 64x64 on screen
64x32 logo at scale 1 -> 64x32 on screen
96x48 logo at scale 1 -> 96x48 on screen
```

---

## Performance Rules

Use static art for persistent UI elements.

Use animation for:

* boot pulse
* short loading states
* scan started feedback
* success/failure confirmation

Avoid:

* long animation loops
* full-screen high-frame-count animation
* drawing over text without clearing the region first
* loading very large assets repeatedly inside tight loops

Recommended limits:

```txt
Icons:       16x16 to 32x32
Logos:       up to 96x48
Animations:  up to 96x64, short loops
```

---

## Checklist

When adding art:

```txt
1. Add source art under your local art folder
2. Convert it with scripts/convert_art.py
3. Save output under assets/icons, assets/logo or assets/animations
4. Use displayDrawIconFromFileScaled or displayDrawAnimationFromFileScaled
5. Run scripts/validate_assets.py
6. Run scripts/generate_assets.py --clean
7. Run scripts/build.sh or scripts/release_gate.py
```
