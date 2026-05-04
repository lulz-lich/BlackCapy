#include "display_manager.h"
#include "hardware_config.h"
#include "logger.h"
#include "filesystem.h"
#include "storage_policy.h"

static DisplayTheme currentTheme = THEME_DARK;

// Theme configurations
static const ThemeConfig themes[] = {
  // THEME_DARK
  {
    "Dark",
    "=",
    " ",
    "█",
    " ",
    true,
    "\033[37m",  // White borders
    "\033[37m",  // White text
    "\033[37m",  // White pixels on
    "\033[30m",  // Black pixels off
    "\033[0m"    // Reset
  },
  // THEME_LIGHT
  {
    "Light",
    "-",
    " ",
    "▓",
    "░",
    true,
    "\033[30m",  // Black borders
    "\033[30m",  // Black text
    "\033[37m",  // White pixels on
    "\033[30m",  // Black pixels off
    "\033[0m"    // Reset
  },
  // THEME_TERMINAL
  {
    "Terminal",
    "#",
    ".",
    "@",
    " ",
    true,
    "\033[32m",  // Green borders
    "\033[32m",  // Green text
    "\033[33m",  // Yellow pixels on
    "\033[30m",  // Black pixels off
    "\033[0m"    // Reset
  },
  // THEME_MATRIX
  {
    "Matrix",
    "│",
    " ",
    "█",
    " ",
    true,
    "\033[32m",  // Green borders
    "\033[32m",  // Green text
    "\033[32m",  // Green pixels on
    "\033[30m",  // Black pixels off
    "\033[0m"    // Reset
  },
  // THEME_MINIMAL
  {
    "Minimal",
    "─",
    " ",
    "•",
    " ",
    true,
    "\033[37m",  // White borders
    "\033[37m",  // White text
    "\033[36m",  // Cyan pixels on
    "\033[30m",  // Black pixels off
    "\033[0m"    // Reset
  }
};

static const int themeCount = sizeof(themes) / sizeof(themes[0]);

// Pixel art rendering functions
static uint8_t* displayLoadBitmap(const String& filename, int* width, int* height);
static void displayRenderBitmapASCII(int x, int y, const uint8_t* bitmap, int w, int h);

void displayInit() {
  logInfo("Display manager initialized.");
  logInfo("Target resolution: " + String(DISPLAY_WIDTH) + "x" + String(DISPLAY_HEIGHT));

  Serial.println("[DISPLAY] Running in serial-render fallback mode.");
}

void displayClear() {
  Serial.println();
  Serial.println("[DISPLAY] CLEAR");
}

void displayRefresh() {
  Serial.println("[DISPLAY] REFRESH");
}

void displayDrawText(int x, int y, const String& text) {
  Serial.print("[TEXT x=");
  Serial.print(x);
  Serial.print(" y=");
  Serial.print(y);
  Serial.print("] ");
  Serial.println(text);
}

void displayDrawTitle(const String& title) {
  const ThemeConfig* theme = &themes[currentTheme];
  String border(40, theme->borderChar[0]);

  Serial.println();
  if (theme->useColors) {
    Serial.print(theme->colorBorder);
  }
  Serial.print(border);
  if (theme->useColors) {
    Serial.print(theme->colorReset);
  }
  Serial.println();

  if (theme->useColors) {
    Serial.print(theme->colorText);
  }
  Serial.print(" ");
  Serial.print(title);
  if (theme->useColors) {
    Serial.print(theme->colorReset);
  }
  Serial.println();

  if (theme->useColors) {
    Serial.print(theme->colorBorder);
  }
  Serial.print(border);
  if (theme->useColors) {
    Serial.print(theme->colorReset);
  }
}

void displayDrawStatusBar(const String& status) {
  Serial.print("[STATUS] ");
  Serial.println(status);
}

void displayDrawBox(int x, int y, int w, int h) {
  Serial.print("[BOX x=");
  Serial.print(x);
  Serial.print(" y=");
  Serial.print(y);
  Serial.print(" w=");
  Serial.print(w);
  Serial.print(" h=");
  Serial.print(h);
  Serial.println("]");
}

void displayDrawAsciiFrame(int x, int y, int w, int h, const String& title) {
  const ThemeConfig* theme = &themes[currentTheme];

  Serial.println();
  String topBottom = "+";
  for (int i = 0; i < w - 2; i++) {
    topBottom += theme->borderChar;
  }
  topBottom += "+";

  if (theme->useColors) {
    Serial.print(theme->colorBorder);
  }
  Serial.println(topBottom);
  if (theme->useColors) {
    Serial.print(theme->colorReset);
  }

  for (int row = 1; row < h - 1; row++) {
    if (theme->useColors) {
      Serial.print(theme->colorBorder);
    }
    Serial.print("|");
    if (theme->useColors) {
      Serial.print(theme->colorReset);
    }

    if (row == 1 && title.length() > 0) {
      if (theme->useColors) {
        Serial.print(theme->colorText);
      }
      String text = " " + title;
      while (text.length() < (unsigned)(w - 1)) {
        text += theme->fillChar;
      }
      Serial.print(text);
      if (theme->useColors) {
        Serial.print(theme->colorReset);
      }
    } else {
      for (int col = 1; col < w - 1; col++) {
        Serial.print(theme->fillChar);
      }
    }

    if (theme->useColors) {
      Serial.print(theme->colorBorder);
    }
    Serial.println("|");
    if (theme->useColors) {
      Serial.print(theme->colorReset);
    }
  }

  if (theme->useColors) {
    Serial.print(theme->colorBorder);
  }
  Serial.println(topBottom);
  if (theme->useColors) {
    Serial.print(theme->colorReset);
  }
}

void displayDrawPixelIcon(int x, int y, const uint8_t* bitmap, int w, int h) {
  if (bitmap == nullptr) {
    Serial.println("[DISPLAY] No bitmap data.");
    return;
  }

  displayRenderBitmapASCII(x, y, bitmap, w, h);
}

void displayDrawIconFromFile(int x, int y, const String& filename) {
  int width, height;
  uint8_t* bitmap = displayLoadBitmap(filename, &width, &height);

  if (bitmap != nullptr) {
    displayDrawPixelIcon(x, y, bitmap, width, height);
    free(bitmap);
  } else {
    Serial.println("[DISPLAY] Failed to load icon: " + filename);
  }
}

void displaySetTheme(DisplayTheme theme) {
  if (theme >= 0 && theme < themeCount) {
    currentTheme = theme;
    logInfo(String("Display theme changed to: ") + themes[theme].name);
  } else {
    logWarn("Invalid theme requested");
  }
}

DisplayTheme displayGetTheme() {
  return currentTheme;
}

const ThemeConfig* displayGetThemeConfig() {
  return &themes[currentTheme];
}

// Pixel art rendering functions
static uint8_t* displayLoadBitmap(const String& filename, int* width, int* height) {
  String assetsPath = storagePolicyGetAssetsPath();
  String fullPath = assetsPath + "/" + filename;

  if (!fileSystemExists(fullPath)) {
    logWarn("Bitmap file not found: " + fullPath);
    return nullptr;
  }

  String bitmapData = fileSystemRead(fullPath);
  if (bitmapData.length() == 0) {
    logWarn("Bitmap file is empty or unreadable: " + fullPath);
    return nullptr;
  }

  String bits = "";
  int detectedWidth = 0;
  int detectedHeight = 0;
  int lineStart = 0;

  while (lineStart < bitmapData.length()) {
    int lineEnd = bitmapData.indexOf('\n', lineStart);

    if (lineEnd < 0) {
      lineEnd = bitmapData.length();
    }

    String line = bitmapData.substring(lineStart, lineEnd);
    line.trim();
    lineStart = lineEnd + 1;

    if (line.length() == 0 || line.startsWith("#")) {
      continue;
    }

    bool binaryLine = true;

    for (int i = 0; i < line.length(); i++) {
      if (line[i] != '0' && line[i] != '1') {
        binaryLine = false;
        break;
      }
    }

    if (!binaryLine) {
      continue;
    }

    if (detectedWidth == 0) {
      detectedWidth = line.length();
    }

    if (line.length() != detectedWidth) {
      logWarn("Bitmap line width mismatch: " + fullPath);
      return nullptr;
    }

    bits += line;
    detectedHeight++;
  }

  if (detectedWidth <= 0 || detectedHeight <= 0) {
    logWarn("Bitmap file has no binary pixel data: " + fullPath);
    return nullptr;
  }

  *width = detectedWidth;
  *height = detectedHeight;

  size_t dataSize = ((*width) * (*height) + 7) / 8;
  uint8_t* bitmap = (uint8_t*)calloc(dataSize, sizeof(uint8_t));

  if (bitmap == nullptr) {
    logWarn("Failed to allocate bitmap memory");
    return nullptr;
  }

  for (int i = 0; i < bits.length(); i++) {
    if (bits[i] == '1') {
      bitmap[i / 8] |= (1 << (7 - (i % 8)));
    }
  }

  return bitmap;
}

static void displayRenderBitmapASCII(int x, int y, const uint8_t* bitmap, int w, int h) {
  const ThemeConfig* theme = &themes[currentTheme];

  Serial.println();
  Serial.print("[BITMAP ");
  Serial.print(w);
  Serial.print("x");
  Serial.print(h);
  Serial.println("]");

  for (int row = 0; row < h; row++) {
    Serial.print("  ");
    for (int col = 0; col < w; col++) {
      int bitIndex = row * w + col;
      int byteIndex = bitIndex / 8;
      int bitOffset = 7 - (bitIndex % 8);

      bool pixel = (bitmap[byteIndex] & (1 << bitOffset)) != 0;
      if (theme->useColors) {
        Serial.print(pixel ? theme->colorPixelOn : theme->colorPixelOff);
      }
      Serial.print(pixel ? theme->pixelOn : theme->pixelOff);
      if (theme->useColors) {
        Serial.print(theme->colorReset);
      }
    }
    Serial.println();
  }
}
