#include "input_manager.h"
#include "hardware_config.h"

#define DEBOUNCE_DELAY 50

static unsigned long lastReadTime = 0;
static unsigned long lastOKPressTime = 0;

void inputInit() {
#if BLACKCAPY_INPUT_BACKEND_ADC
  pinMode(BTN_ADC_PIN, INPUT);
#else
  pinMode(BTN_UP_PIN, INPUT_PULLUP);
  pinMode(BTN_DOWN_PIN, INPUT_PULLUP);
  pinMode(BTN_LEFT_PIN, INPUT_PULLUP);
  pinMode(BTN_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BTN_OK_PIN, INPUT_PULLUP);
  pinMode(BTN_BACK_PIN, INPUT_PULLUP);
#endif
}

static bool adcInRange(int value, int target) {
  return value >= target - BTN_ADC_TOLERANCE && value <= target + BTN_ADC_TOLERANCE;
}

InputEvent inputRead() {
  if (millis() - lastReadTime < DEBOUNCE_DELAY) {
    return INPUT_NONE;
  }

  lastReadTime = millis();

#if BLACKCAPY_INPUT_BACKEND_ADC
  int value = analogRead(BTN_ADC_PIN);

  if (adcInRange(value, BTN_UP_ADC_VALUE)) return INPUT_UP;
  if (adcInRange(value, BTN_DOWN_ADC_VALUE)) return INPUT_DOWN;
  if (adcInRange(value, BTN_LEFT_ADC_VALUE)) return INPUT_LEFT;
  if (adcInRange(value, BTN_RIGHT_ADC_VALUE)) return INPUT_RIGHT;
  if (adcInRange(value, BTN_OK_ADC_VALUE)) {
    inputMarkPhysicalOK();
    return INPUT_OK;
  }
  if (adcInRange(value, BTN_BACK_ADC_VALUE)) return INPUT_BACK;
#else
  if (!digitalRead(BTN_UP_PIN)) return INPUT_UP;
  if (!digitalRead(BTN_DOWN_PIN)) return INPUT_DOWN;
  if (!digitalRead(BTN_LEFT_PIN)) return INPUT_LEFT;
  if (!digitalRead(BTN_RIGHT_PIN)) return INPUT_RIGHT;
  if (!digitalRead(BTN_OK_PIN)) {
    inputMarkPhysicalOK();
    return INPUT_OK;
  }
  if (!digitalRead(BTN_BACK_PIN)) return INPUT_BACK;
#endif

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
