# BlackCapy Theme System

The current theme system lives in `DisplayManager` and affects serial fallback rendering plus future display rendering.

Themes are runtime state today. Persistent theme storage is planned, but not required for the current firmware base.

---

## Built-In Themes

```txt
THEME_DARK
THEME_LIGHT
THEME_TERMINAL
THEME_MATRIX
THEME_MINIMAL
```

Each theme defines:

* Border character
* Fill character
* Pixel-on character
* Pixel-off character
* Optional ANSI color strings for serial-capable terminals

---

## Switching Themes

Physical UI:

```txt
Settings screen
UP/DOWN   Cycle theme
OK        Confirm current runtime choice
BACK      Home
```

Serial shell:

```txt
theme
theme dark
theme light
theme terminal
theme matrix
theme minimal
```

---

## Implementation

Primary functions:

```cpp
displaySetTheme(theme);
displayGetTheme();
displayGetThemeConfig();
```

Rendering functions read the active `ThemeConfig`.

The theme system must stay cheap: no heap-heavy state, no blocking transitions and no visual effect that reduces responsiveness.

---

## Future Work

Planned improvements:

* Persist selected theme through `Storage`
* Support real 320x240 color palettes
* Support static and animated ASCII
* Support static and animated pixel art
* Load custom themes from `/themes` on native microSD
