#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>

enum DisplayTheme {
  THEME_DARK,
  THEME_LIGHT,
  THEME_TERMINAL,
  THEME_MATRIX,
  THEME_MINIMAL
};

struct ThemeConfig {
  const char* name;
  const char* borderChar;
  const char* fillChar;
  const char* pixelOn;
  const char* pixelOff;
  bool useColors;
  const char* colorBorder;    // ANSI color code for borders
  const char* colorText;      // ANSI color code for text
  const char* colorPixelOn;   // ANSI color code for active pixels
  const char* colorPixelOff;  // ANSI color code for inactive pixels
  const char* colorReset;     // ANSI reset code
};

void displayInit();

void displayClear();
void displayRefresh();

void displayDrawText(int x, int y, const String& text);
void displayDrawTitle(const String& title);
void displayDrawStatusBar(const String& status);

void displayDrawBox(int x, int y, int w, int h);
void displayDrawAsciiFrame(int x, int y, int w, int h, const String& title);
void displayDrawPixelIcon(int x, int y, const uint8_t* bitmap, int w, int h);
void displayDrawIconFromFile(int x, int y, const String& filename);
void displayDrawAnimationFromFile(int x, int y, const String& filename, int frameDelayMs, int loops);

void displaySetTheme(DisplayTheme theme);
DisplayTheme displayGetTheme();
const ThemeConfig* displayGetThemeConfig();

#endif
