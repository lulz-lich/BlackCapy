#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <Arduino.h>

enum ScreenId {
  SCREEN_HOME,
  SCREEN_TOOLS,
  SCREEN_STATUS,
  SCREEN_SETTINGS
};

void screenManagerInit();
void screenSet(ScreenId screen);
ScreenId screenGet();
void screenRender();

#endif