#include "screen_manager.h"
#include "display_manager.h"
#include "blackcapy.h"
#include "app_manager.h"
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

  int total = appManagerCount();
  int selected = appManagerSelectedIndex();

  if (total <= 0) {
    displayDrawText(0, 40, "No tools registered.");
    displayDrawStatusBar("TOOLS EMPTY");
    displayRefresh();
    return;
  }

  int start = selected - 2;

  if (start < 0) {
    start = 0;
  }

  int end = start + 5;

  if (end > total) {
    end = total;
  }

  for (int i = start; i < end; i++) {
    AppEntry* app = appManagerGet(i);

    if (app == nullptr) continue;

    String line = "";

    if (i == selected) {
      line += "> ";
    } else {
      line += "  ";
    }

    line += app->name;

    displayDrawText(0, 40 + ((i - start) * 20), line);
  }

  AppEntry* selectedApp = appManagerGetSelected();

  if (selectedApp != nullptr) {
    displayDrawText(0, 160, "Category: " + String(selectedApp->category));
    displayDrawText(0, 180, String(selectedApp->description));
  }

  displayDrawStatusBar("UP/DOWN SELECT | OK RUN");
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
void screenRenderTools() {
  renderTools();
}