#include "screen_manager.h"

#include <Arduino.h>

#include "blackcapy.h"
#include "display_manager.h"
#include "app_manager.h"
#include "logger.h"
#include "ui.h"

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

static void renderToolIcon(int toolId, int x, int y) {
  switch (toolId) {
    case TOOL_WIFI_SCANNER:
      uiShowIconWiFiHybrid();
      break;
    case TOOL_BLE_SCANNER:
      uiShowIconBLEHybrid();
      break;
    case TOOL_GPIO_CONSOLE:
      uiShowIconGPIOHybrid();
      break;
    case TOOL_RF_ANALYZER:
      uiShowIconRFHybrid();
      break;
    case TOOL_SUBGHZ_SCANNER:
      uiShowIconSubGHzHybrid();
      break;
    default:
      // No specific icon, show generic
      displayDrawAsciiFrame(x, y, 20, 6, "TOOL");
      break;
  }
}

static void renderHome() {
  displayClear();

  // Show logo
  uiShowLogoHybrid();

  displayDrawTitle("BlackCapy");

  displayDrawText(0, 40, "Professional Toolkit");
  displayDrawText(0, 60, "Hybrid UI: Pixel Art + ASCII");
  displayDrawText(0, 80, "Display: 320x240 (Serial Fallback)");
  displayDrawText(0, 100, "Assets: microSD /assets");
  displayDrawText(0, 120, "Scripts: microSD /scripts");
  displayDrawText(0, 140, "LEFT/RIGHT: screens");
  displayDrawText(0, 160, "OK: select | BACK: home");

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

  const int visibleItems = APP_MANAGER_VISIBLE_ITEMS;
  int page = selected / visibleItems;
  int start = page * visibleItems;
  int end = start + visibleItems;

  if (end > total) {
    end = total;
  }

  for (int i = start; i < end; i++) {
    AppEntry* app = appManagerGet(i);

    if (app == nullptr) {
      continue;
    }

    // Render icon for this tool
    int iconY = 35 + ((i - start) * 24);
    renderToolIcon(app->id, 0, iconY);

    // Render text
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

    displayDrawText(25, 40 + ((i - start) * 24), line);
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

  int pageCount = (total + visibleItems - 1) / visibleItems;
  displayDrawStatusBar(
    "UP/DOWN SEL | LEFT/RIGHT PAGE " + String(page + 1) + "/" + String(pageCount)
  );
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

  const ThemeConfig* currentThemeConfig = displayGetThemeConfig();

  displayDrawText(0, 40, "Profile: Operator");
  displayDrawText(0, 60, String("Theme: ") + currentThemeConfig->name);
  displayDrawText(0, 80, "UI: Hybrid (Pixel + ASCII)");
  displayDrawText(0, 100, "ASCII: Enabled");
  displayDrawText(0, 120, "Pixel Art: Implemented");
  displayDrawText(0, 140, "Scripts: Enabled");
  displayDrawText(0, 160, "Sub-GHz: Active");
  displayDrawText(0, 180, "UP/DOWN: Change Theme");

  displayDrawStatusBar("SETTINGS | UP/DOWN: Theme");
  displayRefresh();
}

void screenRenderSettings() {
  renderSettings();
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
