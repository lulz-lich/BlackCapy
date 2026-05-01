#include "ui_controller.h"

#include "input_manager.h"
#include "screen_manager.h"
#include "logger.h"

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

void uiControllerUpdate() {
  InputEvent event = inputRead();

  if (event == INPUT_NONE) {
    return;
  }

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

    case INPUT_OK:
      logInfo("OK pressed on current screen.");
      break;

    case INPUT_BACK:
      currentTab = 0;
      screenSet(SCREEN_HOME);
      break;

    default:
      break;
  }
}