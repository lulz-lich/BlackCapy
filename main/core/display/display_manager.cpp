#include "display_manager.h"
#include "hardware_config.h"
#include "logger.h"
#include "filesystem.h"
#include "storage_policy.h"

#if BLACKCAPY_DISPLAY_BACKEND_TFT
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#endif

static DisplayTheme currentTheme = THEME_DARK;

#if BLACKCAPY_DISPLAY_BACKEND_TFT
static Adafruit_ILI9341 tft(DISPLAY_TFT_CS_PIN, DISPLAY_TFT_DC_PIN, DISPLAY_TFT_RST_PIN);
static bool tftReady = false;
#endif

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
static bool displayRenderFrameBits(int x, int y, const String& bits, int width, int height, const String& source, int scale);
static int displayRenderAnimationPass(int x, int y, const String& animationData, const String& source, int frameDelayMs, int scale);
#if BLACKCAPY_DISPLAY_BACKEND_TFT
static uint16_t displayColorBackground();
static uint16_t displayColorBorder();
static uint16_t displayColorText();
static uint16_t displayColorPixelOn();
static uint16_t displayColorPixelOff();
static void displayRenderBitmapTFT(int x, int y, const uint8_t* bitmap, int w, int h, int scale);
#endif

void displayInit() {
  logInfo("Display manager initialized.");
  logInfo("Target resolution: " + String(DISPLAY_WIDTH) + "x" + String(DISPLAY_HEIGHT));

#if BLACKCAPY_DISPLAY_BACKEND_TFT
  SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);

  if (DISPLAY_TFT_BL_PIN >= 0) {
    pinMode(DISPLAY_TFT_BL_PIN, OUTPUT);
    digitalWrite(DISPLAY_TFT_BL_PIN, HIGH);
  }

  tft.begin();
  tft.setRotation(DISPLAY_TFT_ROTATION);
  tft.setTextWrap(false);
  tft.fillScreen(displayColorBackground());
  tftReady = true;

  logInfo("Display backend: Adafruit ILI9341 SPI TFT");
  Serial.println("[DISPLAY] TFT backend active; serial mirror enabled.");
#else
  Serial.println("[DISPLAY] Running in serial-render fallback mode.");
#endif
}

void displayClear() {
#if BLACKCAPY_DISPLAY_BACKEND_TFT
  if (tftReady) {
    tft.fillScreen(displayColorBackground());
  }
#endif
  Serial.println();
  Serial.println("[DISPLAY] CLEAR");
}

void displayRefresh() {
#if BLACKCAPY_DISPLAY_BACKEND_TFT
  if (tftReady) {
    return;
  }
#endif
  Serial.println("[DISPLAY] REFRESH");
}

void displayDrawText(int x, int y, const String& text) {
#if BLACKCAPY_DISPLAY_BACKEND_TFT
  if (tftReady) {
    tft.setTextColor(displayColorText(), displayColorBackground());
    tft.setTextSize(1);
    tft.setCursor(x, y);
    tft.print(text);
  }
#endif
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

#if BLACKCAPY_DISPLAY_BACKEND_TFT
  if (tftReady) {
    tft.fillRect(0, 0, DISPLAY_WIDTH, 22, displayColorBorder());
    tft.setTextSize(2);
    tft.setTextColor(displayColorBackground(), displayColorBorder());
    tft.setCursor(6, 4);
    tft.print(title);
  }
#endif

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
#if BLACKCAPY_DISPLAY_BACKEND_TFT
  if (tftReady) {
    int y = DISPLAY_HEIGHT - 14;
    tft.fillRect(0, y, DISPLAY_WIDTH, 14, displayColorBorder());
    tft.setTextSize(1);
    tft.setTextColor(displayColorBackground(), displayColorBorder());
    tft.setCursor(4, y + 3);
    tft.print(status.substring(0, 52));
  }
#endif
  Serial.print("[STATUS] ");
  Serial.println(status);
}

void displayDrawBox(int x, int y, int w, int h) {
#if BLACKCAPY_DISPLAY_BACKEND_TFT
  if (tftReady) {
    tft.drawRect(x, y, w, h, displayColorBorder());
  }
#endif
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

#if BLACKCAPY_DISPLAY_BACKEND_TFT
  if (tftReady) {
    tft.drawRect(x, y, w, h, displayColorBorder());
    if (title.length() > 0) {
      tft.fillRect(x + 1, y + 1, w - 2, 12, displayColorBorder());
      tft.setTextSize(1);
      tft.setTextColor(displayColorBackground(), displayColorBorder());
      tft.setCursor(x + 4, y + 3);
      tft.print(title.substring(0, max(0, (w - 8) / 6)));
    }
  }
#endif

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
  displayDrawPixelIconScaled(x, y, bitmap, w, h, 1);
}

void displayDrawPixelIconScaled(int x, int y, const uint8_t* bitmap, int w, int h, int scale) {
  if (bitmap == nullptr) {
    Serial.println("[DISPLAY] No bitmap data.");
    return;
  }

  int renderScale = scale > 0 ? scale : 1;

#if BLACKCAPY_DISPLAY_BACKEND_TFT
  if (tftReady) {
    displayRenderBitmapTFT(x, y, bitmap, w, h, renderScale);
  }
#endif
  displayRenderBitmapASCII(x, y, bitmap, w, h);
}

void displayDrawIconFromFile(int x, int y, const String& filename) {
  displayDrawIconFromFileScaled(x, y, filename, 1);
}

void displayDrawIconFromFileScaled(int x, int y, const String& filename, int scale) {
  int width, height;
  uint8_t* bitmap = displayLoadBitmap(filename, &width, &height);

  if (bitmap != nullptr) {
    displayDrawPixelIconScaled(x, y, bitmap, width, height, scale);
    free(bitmap);
  } else {
    Serial.println("[DISPLAY] Failed to load icon: " + filename);
  }
}

void displayDrawAnimationFromFile(int x, int y, const String& filename, int frameDelayMs, int loops) {
  displayDrawAnimationFromFileScaled(x, y, filename, frameDelayMs, loops, 1);
}

void displayDrawAnimationFromFileScaled(int x, int y, const String& filename, int frameDelayMs, int loops, int scale) {
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
  int renderScale = scale > 0 ? scale : 1;
  int totalFrames = 0;

  for (int loop = 0; loop < loopCount; loop++) {
    int rendered = displayRenderAnimationPass(x, y, animationData, fullPath, delayMs, renderScale);
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

#if BLACKCAPY_DISPLAY_BACKEND_TFT
static uint16_t displayColorBackground() {
  switch (currentTheme) {
    case THEME_LIGHT:
      return ILI9341_WHITE;
    case THEME_TERMINAL:
    case THEME_MATRIX:
    case THEME_MINIMAL:
    case THEME_DARK:
    default:
      return ILI9341_BLACK;
  }
}

static uint16_t displayColorBorder() {
  switch (currentTheme) {
    case THEME_LIGHT:
      return ILI9341_BLACK;
    case THEME_TERMINAL:
    case THEME_MATRIX:
      return ILI9341_GREEN;
    case THEME_MINIMAL:
      return ILI9341_CYAN;
    case THEME_DARK:
    default:
      return ILI9341_WHITE;
  }
}

static uint16_t displayColorText() {
  switch (currentTheme) {
    case THEME_LIGHT:
      return ILI9341_BLACK;
    case THEME_TERMINAL:
    case THEME_MATRIX:
      return ILI9341_GREEN;
    case THEME_MINIMAL:
      return ILI9341_WHITE;
    case THEME_DARK:
    default:
      return ILI9341_WHITE;
  }
}

static uint16_t displayColorPixelOn() {
  switch (currentTheme) {
    case THEME_LIGHT:
      return ILI9341_BLACK;
    case THEME_TERMINAL:
      return ILI9341_YELLOW;
    case THEME_MATRIX:
      return ILI9341_GREEN;
    case THEME_MINIMAL:
      return ILI9341_CYAN;
    case THEME_DARK:
    default:
      return ILI9341_WHITE;
  }
}

static uint16_t displayColorPixelOff() {
  switch (currentTheme) {
    case THEME_LIGHT:
      return ILI9341_WHITE;
    case THEME_TERMINAL:
    case THEME_MATRIX:
    case THEME_MINIMAL:
    case THEME_DARK:
    default:
      return displayColorBackground();
  }
}

static void displayRenderBitmapTFT(int x, int y, const uint8_t* bitmap, int w, int h, int scale) {
  uint16_t onColor = displayColorPixelOn();
  uint16_t offColor = displayColorPixelOff();
  int renderScale = scale > 0 ? scale : 1;

  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col++) {
      int bitIndex = row * w + col;
      int byteIndex = bitIndex / 8;
      int bitOffset = 7 - (bitIndex % 8);
      bool pixel = (bitmap[byteIndex] & (1 << bitOffset)) != 0;

      int drawX = x + (col * renderScale);
      int drawY = y + (row * renderScale);

      if (drawX < 0 || drawX >= DISPLAY_WIDTH || drawY < 0 || drawY >= DISPLAY_HEIGHT) {
        continue;
      }

      if (renderScale == 1) {
        tft.drawPixel(drawX, drawY, pixel ? onColor : offColor);
      } else {
        tft.fillRect(drawX, drawY, renderScale, renderScale, pixel ? onColor : offColor);
      }
    }
  }
}
#endif

static bool displayRenderFrameBits(int x, int y, const String& bits, int width, int height, const String& source, int scale) {
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

  displayDrawPixelIconScaled(x, y, bitmap, width, height, scale);
  free(bitmap);
  return true;
}

static int displayRenderAnimationPass(int x, int y, const String& animationData, const String& source, int frameDelayMs, int scale) {
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
      if (displayRenderFrameBits(x, y, bits, width, height, source, scale)) {
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

  if (displayRenderFrameBits(x, y, bits, width, height, source, scale)) {
    frameCount++;
    delay(frameDelayMs);
  }

  return frameCount;
}
