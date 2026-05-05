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
      displayDrawIconFromFileScaled(x, y, "icons/wifi_icon.bmp", 2);
      break;
    case TOOL_BLE_SCANNER:
      displayDrawIconFromFileScaled(x, y, "icons/ble_icon.bmp", 2);
      break;
    case TOOL_GPIO_CONSOLE:
      displayDrawIconFromFileScaled(x, y, "icons/gpio_icon.bmp", 2);
      break;
    case TOOL_RF_ANALYZER:
      displayDrawIconFromFileScaled(x, y, "icons/rf_icon.bmp", 2);
      break;
    case TOOL_SUBGHZ_SCANNER:
      displayDrawIconFromFileScaled(x, y, "icons/subghz_icon.bmp", 2);
      break;
    case TOOL_SIGNAL_DATABASE:
      displayDrawIconFromFileScaled(x, y, "icons/signal_icon.bmp", 2);
      break;
    case TOOL_AI_ANALYZER:
      displayDrawIconFromFileScaled(x, y, "icons/ai_icon.bmp", 2);
      break;
    case TOOL_AI_REPORT_VIEWER:
      displayDrawIconFromFileScaled(x, y, "icons/ai_icon.bmp", 2);
      break;
    default:
      // No specific icon, show generic
      displayDrawAsciiFrame(x, y, 20, 6, "TOOL");
      break;
  }
}

static String compactText(const String& text, int maxChars) {
  if (text.length() <= (unsigned)maxChars) {
    return text;
  }

  if (maxChars <= 3) {
    return text.substring(0, maxChars);
  }

  return text.substring(0, maxChars - 3) + "...";
}

static void renderHome() {
  displayClear();

  displayDrawTitle("BlackCapy");
  displayDrawIconFromFile(248, 34, "logo/blackcapy_logo.bmp");

  displayDrawText(0, 40, "Professional Toolkit");
  displayDrawText(0, 60, "Hybrid UI: Pixel Art + ASCII");
  displayDrawText(0, 80, "Display: 320x240 SPI TFT");
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

  int page = appManagerCurrentPage();
  int pageCount = appManagerPageCount();
  int start = appManagerPageStartIndex();
  int end = appManagerPageEndIndex();

  for (int i = start; i < end; i++) {
    AppEntry* app = appManagerGet(i);

    if (app == nullptr) {
      continue;
    }

    // Render icon for this tool
    int iconY = 34 + ((i - start) * 28);
    renderToolIcon(app->id, 0, iconY);

    // Render text
    String line = "";

    if (i == selected) {
      line += "> ";
    } else {
      line += "  ";
    }

    line += compactText(app->name, 22);

    if (!appManagerIsRunnable(app)) {
      line += " [LOCK]";
    }

    displayDrawText(42, 42 + ((i - start) * 28), line);
  }

  AppEntry* selectedApp = appManagerGetSelected();

  if (selectedApp != nullptr) {
    displayDrawText(0, 150, "ID: " + String(selectedApp->id) + " | Category: " + String(selectedApp->category));

    displayDrawText(
      0,
      172,
      compactText(String(selectedApp->description), 42)
    );

    if (!appManagerIsRunnable(selectedApp)) {
      displayDrawText(0, 205, "Module required.");
    }
  }

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
