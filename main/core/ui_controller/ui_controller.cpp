#include "ui_controller.h"

#include "input_manager.h"
#include "screen_manager.h"
#include "app_manager.h"
#include "logger.h"
#include "display_manager.h"

static int currentTab = 0;

static const ScreenId tabs[] = {
  SCREEN_HOME,
  SCREEN_TOOLS,
  SCREEN_STATUS,
  SCREEN_SETTINGS
};

static const int tabCount = sizeof(tabs) / sizeof(tabs[0]);

void uiControllerInit() {
  currentTab = 0;
  screenSet(tabs[currentTab]);
  logInfo("UI controller initialized.");
}

void handleToolsInput(InputEvent event) {
  switch (event) {
    case INPUT_UP:
      appManagerPrevious();
      screenRenderTools();
      break;

    case INPUT_DOWN:
      appManagerNext();
      screenRenderTools();
      break;

    case INPUT_OK:
      appManagerRunSelected();
      screenRenderTools();
      break;

    case INPUT_BACK:
      currentTab = 0;
      screenSet(SCREEN_HOME);
      break;

    case INPUT_LEFT:
      appManagerPreviousPage();
      screenRenderTools();
      break;

    case INPUT_RIGHT:
      appManagerNextPage();
      screenRenderTools();
      break;

    default:
      break;
  }
}

void handleSettingsInput(InputEvent event) {
  switch (event) {
    case INPUT_UP:
      // Cycle to previous theme
      {
        DisplayTheme current = displayGetTheme();
        DisplayTheme next = (current == 0) ? THEME_MINIMAL : (DisplayTheme)(current - 1);
        displaySetTheme(next);
        screenRenderSettings();
      }
      break;

    case INPUT_DOWN:
      // Cycle to next theme
      {
        DisplayTheme current = displayGetTheme();
        DisplayTheme next = (current == THEME_MINIMAL) ? THEME_DARK : (DisplayTheme)(current + 1);
        displaySetTheme(next);
        screenRenderSettings();
      }
      break;

    case INPUT_OK:
      logInfo("Theme setting confirmed");
      break;

    case INPUT_BACK:
      currentTab = 0;
      screenSet(SCREEN_HOME);
      break;

    default:
      break;
  }
}

void handleTabNavigation(InputEvent event) {
  switch (event) {
    case INPUT_LEFT:
      if (currentTab > 0) {
        currentTab--;
      } else {
        currentTab = tabCount - 1;
      }

      screenSet(tabs[currentTab]);
      break;

    case INPUT_RIGHT:
      if (currentTab < tabCount - 1) {
        currentTab++;
      } else {
        currentTab = 0;
      }

      screenSet(tabs[currentTab]);
      break;

    case INPUT_BACK:
      currentTab = 0;
      screenSet(SCREEN_HOME);
      break;

    default:
      break;
  }
}

void uiControllerUpdate() {
  InputEvent event = inputRead();

  if (event == INPUT_NONE) {
    return;
  }

  if (screenGet() == SCREEN_TOOLS) {
    handleToolsInput(event);
    return;
  }

  if (screenGet() == SCREEN_SETTINGS) {
    handleSettingsInput(event);
    return;
  }

  handleTabNavigation(event);
}
