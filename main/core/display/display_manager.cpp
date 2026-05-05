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
static bool displayRenderFrameBits(int x, int y, const String& bits, int width, int height, const String& source);
static int displayRenderAnimationPass(int x, int y, const String& animationData, const String& source, int frameDelayMs);

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

void displayDrawAnimationFromFile(int x, int y, const String& filename, int frameDelayMs, int loops) {
  String assetsPath = storagePolicyGetAssetsPath();
  String fullPath = assetsPath + "/" + filename;

  if (!fileSystemExists(fullPath)) {
    logWarn("Animation file not found: " + fullPath);
    Serial.println("[DISPLAY] Failed to load animation: " + filename);
    return;
  }

  String animationData = fileSystemRead(fullPath);
  if (animationData.length() == 0) {
    logWarn("Animation file is empty or unreadable: " + fullPath);
    Serial.println("[DISPLAY] Failed to load animation: " + filename);
    return;
  }

  int delayMs = frameDelayMs > 0 ? frameDelayMs : 120;
  int loopCount = loops > 0 ? loops : 1;
  int totalFrames = 0;

  for (int loop = 0; loop < loopCount; loop++) {
    int rendered = displayRenderAnimationPass(x, y, animationData, fullPath, delayMs);
    if (rendered <= 0) {
      logWarn("Animation file has no renderable frames: " + fullPath);
      return;
    }
    totalFrames += rendered;
  }

  logInfo("Animation rendered: " + filename + " frames=" + String(totalFrames));
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

static bool displayRenderFrameBits(int x, int y, const String& bits, int width, int height, const String& source) {
  if (width <= 0 || height <= 0 || bits.length() == 0) {
    return false;
  }

  size_t dataSize = (bits.length() + 7) / 8;
  uint8_t* bitmap = (uint8_t*)calloc(dataSize, sizeof(uint8_t));

  if (bitmap == nullptr) {
    logWarn("Failed to allocate animation frame memory: " + source);
    return false;
  }

  for (int i = 0; i < bits.length(); i++) {
    if (bits[i] == '1') {
      bitmap[i / 8] |= (1 << (7 - (i % 8)));
    }
  }

  displayDrawPixelIcon(x, y, bitmap, width, height);
  free(bitmap);
  return true;
}

static int displayRenderAnimationPass(int x, int y, const String& animationData, const String& source, int frameDelayMs) {
  String bits = "";
  int width = 0;
  int height = 0;
  int frameCount = 0;
  int lineStart = 0;

  while (lineStart < animationData.length()) {
    int lineEnd = animationData.indexOf('\n', lineStart);

    if (lineEnd < 0) {
      lineEnd = animationData.length();
    }

    String line = animationData.substring(lineStart, lineEnd);
    line.trim();
    lineStart = lineEnd + 1;

    if (line.length() == 0 || line.startsWith("#")) {
      continue;
    }

    if (line == "---") {
      if (displayRenderFrameBits(x, y, bits, width, height, source)) {
        frameCount++;
        delay(frameDelayMs);
      }
      bits = "";
      width = 0;
      height = 0;
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
      logWarn("Animation ignored non-binary line: " + source);
      continue;
    }

    if (width == 0) {
      width = line.length();
    }

    if (line.length() != width) {
      logWarn("Animation frame width mismatch: " + source);
      bits = "";
      width = 0;
      height = 0;
      continue;
    }

    bits += line;
    height++;
  }

  if (displayRenderFrameBits(x, y, bits, width, height, source)) {
    frameCount++;
    delay(frameDelayMs);
  }

  return frameCount;
}
