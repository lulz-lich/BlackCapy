#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>

enum InputEvent {
  INPUT_NONE,
  INPUT_UP,
  INPUT_DOWN,
  INPUT_LEFT,
  INPUT_RIGHT,
  INPUT_OK,
  INPUT_BACK
};

void inputInit();
InputEvent inputRead();
bool inputPhysicalOKRecentlyPressed(unsigned long windowMs);
void inputMarkPhysicalOK();

#endif
