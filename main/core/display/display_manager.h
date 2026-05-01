#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>

enum DisplayTheme {
  THEME_DARK,
  THEME_LIGHT,
  THEME_TERMINAL
};

void displayInit();

void displayClear();
void displayRefresh();

void displayDrawText(int x, int y, const String& text);
void displayDrawTitle(const String& title);
void displayDrawStatusBar(const String& status);

void displayDrawBox(int x, int y, int w, int h);
void displayDrawPixelIcon(int x, int y, const uint8_t* bitmap, int w, int h);

void displaySetTheme(DisplayTheme theme);
DisplayTheme displayGetTheme();

#endif