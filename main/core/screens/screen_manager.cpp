#include "screen_manager.h"
#include "display_manager.h"
#include "blackcapy.h"

static ScreenId currentScreen = SCREEN_HOME;

void screenManagerInit() {
  currentScreen = SCREEN_HOME;
}

void screenSet(ScreenId screen) {
  currentScreen = screen;
  screenRender();
}

ScreenId screenGet() {
  return currentScreen;
}

void renderHome() {
  displayClear();
  displayDrawTitle("BlackCapy");
  displayDrawText(0, 40, "Professional Toolkit");
  displayDrawText(0, 60, "Hybrid UI: ASCII + Pixel Art");
  displayDrawText(0, 80, "Display: 320x240");
  displayDrawStatusBar("READY");
  displayRefresh();
}

void renderTools() {
  displayClear();
  displayDrawTitle("Tools");
  displayDrawText(0, 40, "> System Monitor");
  displayDrawText(0, 60, "  WiFi Scanner");
  displayDrawText(0, 80, "  GPIO Console");
  displayDrawText(0, 100, "  Module Manager");
  displayDrawStatusBar("TOOLS");
  displayRefresh();
}

void renderStatus() {
  displayClear();
  displayDrawTitle("Status");
  displayDrawText(0, 40, "System: OK");
  displayDrawText(0, 60, "Runtime: Active");
  displayDrawText(0, 80, "Input: Buttons");
  displayDrawStatusBar("STATUS");
  displayRefresh();
}

void renderSettings() {
  displayClear();
  displayDrawTitle("Settings");
  displayDrawText(0, 40, "Profile: Operator");
  displayDrawText(0, 60, "Theme: Dark");
  displayDrawText(0, 80, "Animations: Enabled");
  displayDrawStatusBar("SETTINGS");
  displayRefresh();
}

void screenRender() {
  switch (currentScreen) {
    case SCREEN_HOME:
      renderHome();
      break;

    case SCREEN_TOOLS:
      renderTools();
      break;

    case SCREEN_STATUS:
      renderStatus();
      break;

    case SCREEN_SETTINGS:
      renderSettings();
      break;

    default:
      renderHome();
      break;
  }
}