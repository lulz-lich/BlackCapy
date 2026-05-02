#include "screen_manager.h"

#include <Arduino.h>

#include "blackcapy.h"
#include "display_manager.h"
#include "app_manager.h"
#include "logger.h"

static ScreenId currentScreen = SCREEN_HOME;

void screenManagerInit() {
  currentScreen = SCREEN_HOME;
  logInfo("Screen manager initialized.");
}

void screenSet(ScreenId screen) {
  currentScreen = screen;
  screenRender();
}

ScreenId screenGet() {
  return currentScreen;
}

static void renderHome() {
  displayClear();

  displayDrawTitle("BlackCapy");

  displayDrawText(0, 40, "Professional Toolkit");
  displayDrawText(0, 60, "Hybrid UI: ASCII + Pixel Art");
  displayDrawText(0, 80, "Display: 320x240");
  displayDrawText(0, 110, "LEFT/RIGHT: screens");
  displayDrawText(0, 130, "OK: select");
  displayDrawText(0, 150, "BACK: home");

  displayDrawStatusBar("HOME | READY");
  displayRefresh();
}

static void renderTools() {
  displayClear();

  displayDrawTitle("Tools");

  int total = appManagerCount();
  int selected = appManagerSelectedIndex();

  if (total <= 0) {
    displayDrawText(0, 40, "No apps registered.");
    displayDrawStatusBar("TOOLS EMPTY");
    displayRefresh();
    return;
  }

  const int visibleItems = 4;

  int start = selected - 1;

  if (start < 0) {
    start = 0;
  }

  int end = start + visibleItems;

  if (end > total) {
    end = total;
    start = end - visibleItems;

    if (start < 0) {
      start = 0;
    }
  }

  for (int i = start; i < end; i++) {
    AppEntry* app = appManagerGet(i);

    if (app == nullptr) {
      continue;
    }

    String line = "";

    if (i == selected) {
      line += "> ";
    } else {
      line += "  ";
    }

    line += app->name;

    if (!appManagerIsRunnable(app)) {
      line += " [LOCK]";
    }

    displayDrawText(0, 40 + ((i - start) * 24), line);
  }

  AppEntry* selectedApp = appManagerGetSelected();

  if (selectedApp != nullptr) {
    displayDrawText(
      0,
      150,
      "Category: " + String(selectedApp->category)
    );

    displayDrawText(
      0,
      172,
      String(selectedApp->description)
    );

    if (!appManagerIsRunnable(selectedApp)) {
      displayDrawText(0, 205, "Module required.");
    }
  }

  displayDrawStatusBar("UP/DOWN SELECT | OK RUN");
  displayRefresh();
}

static void renderStatus() {
  displayClear();

  displayDrawTitle("Status");

  displayDrawText(0, 40, "System: OK");
  displayDrawText(0, 60, "Runtime: Active");
  displayDrawText(0, 80, "Input: Buttons");
  displayDrawText(0, 100, "Storage: microSD native");
  displayDrawText(0, 120, "Modules: Expansion bus");

  displayDrawStatusBar("STATUS");
  displayRefresh();
}

static void renderSettings() {
  displayClear();

  displayDrawTitle("Settings");

  displayDrawText(0, 40, "Profile: Operator");
  displayDrawText(0, 60, "UI: Hybrid");
  displayDrawText(0, 80, "ASCII: Enabled");
  displayDrawText(0, 100, "Pixel Art: Planned");
  displayDrawText(0, 120, "AI: Planned");

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

void screenRenderTools() {
  renderTools();
}