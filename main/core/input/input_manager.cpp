#include "input_manager.h"
#include "hardware_config.h"

#define DEBOUNCE_DELAY 50

static unsigned long lastReadTime = 0;
static unsigned long lastOKPressTime = 0;

void inputInit() {
  pinMode(BTN_UP_PIN, INPUT_PULLUP);
  pinMode(BTN_DOWN_PIN, INPUT_PULLUP);
  pinMode(BTN_LEFT_PIN, INPUT_PULLUP);
  pinMode(BTN_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BTN_OK_PIN, INPUT_PULLUP);
  pinMode(BTN_BACK_PIN, INPUT_PULLUP);
}

InputEvent inputRead() {
  if (millis() - lastReadTime < DEBOUNCE_DELAY) {
    return INPUT_NONE;
  }

  lastReadTime = millis();

  if (!digitalRead(BTN_UP_PIN)) return INPUT_UP;
  if (!digitalRead(BTN_DOWN_PIN)) return INPUT_DOWN;
  if (!digitalRead(BTN_LEFT_PIN)) return INPUT_LEFT;
  if (!digitalRead(BTN_RIGHT_PIN)) return INPUT_RIGHT;
  if (!digitalRead(BTN_OK_PIN)) {
    inputMarkPhysicalOK();
    return INPUT_OK;
  }
  if (!digitalRead(BTN_BACK_PIN)) return INPUT_BACK;

  return INPUT_NONE;
}

bool inputPhysicalOKRecentlyPressed(unsigned long windowMs) {
  if (lastOKPressTime == 0) {
    return false;
  }

  return millis() - lastOKPressTime <= windowMs;
}

void inputMarkPhysicalOK() {
  lastOKPressTime = millis();
}
