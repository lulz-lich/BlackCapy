#include "display_manager.h"
#include "hardware_config.h"
#include "logger.h"

static DisplayTheme currentTheme = THEME_DARK;

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
  Serial.println();
  Serial.println("========================================");
  Serial.print(" ");
  Serial.println(title);
  Serial.println("========================================");
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

void displayDrawPixelIcon(int x, int y, const uint8_t* bitmap, int w, int h) {
  Serial.print("[PIXEL ICON x=");
  Serial.print(x);
  Serial.print(" y=");
  Serial.print(y);
  Serial.print(" w=");
  Serial.print(w);
  Serial.print(" h=");
  Serial.print(h);
  Serial.println("]");

  if (bitmap == nullptr) {
    Serial.println("[DISPLAY] No bitmap data.");
  }
}

void displaySetTheme(DisplayTheme theme) {
  currentTheme = theme;
  logInfo("Display theme changed.");
}

DisplayTheme displayGetTheme() {
  return currentTheme;
}