#include <Arduino.h>

#include "button_calibration.h"

#include "display_manager.h"
#include "hardware_config.h"
#include "ui.h"

struct ButtonTarget {
  const char* name;
  int expected;
};

static int readButtonADCStable() {
  const int samples = 20;
  long total = 0;

  for (int i = 0; i < samples; i++) {
    total += analogRead(BTN_ADC_PIN);
    displayDelay(5);
  }

  return total / samples;
}

static void printButtonTarget(const ButtonTarget& target) {
  Serial.print(target.name);
  Serial.print(": ");
  Serial.print(target.expected - BTN_ADC_TOLERANCE);
  Serial.print("..");
  Serial.println(target.expected + BTN_ADC_TOLERANCE);
}

void runButtonCalibration() {
  Serial.println();
  Serial.println("========== BUTTON CALIBRATION ==========");

  displayClear();
  displayDrawTitle("Button Calibration");
  displayDrawText(8, 32, "ADC ladder live sample");
  displayStartAnimationLoopFromFileScaled(248, 118, "animations/boot_pulse.anim", 140, 2);

  pinMode(BTN_ADC_PIN, INPUT);

  ButtonTarget targets[] = {
    {"UP", BTN_UP_ADC_VALUE},
    {"DOWN", BTN_DOWN_ADC_VALUE},
    {"LEFT", BTN_LEFT_ADC_VALUE},
    {"RIGHT", BTN_RIGHT_ADC_VALUE},
    {"OK", BTN_OK_ADC_VALUE},
    {"BACK", BTN_BACK_ADC_VALUE}
  };

  const int targetCount = sizeof(targets) / sizeof(targets[0]);

  Serial.print("ADC pin: GPIO ");
  Serial.println(BTN_ADC_PIN);
  Serial.print("Tolerance: +/-");
  Serial.println(BTN_ADC_TOLERANCE);
  Serial.println();
  Serial.println("Expected ranges:");

  for (int i = 0; i < targetCount; i++) {
    printButtonTarget(targets[i]);
    displayDrawText(8, 56 + i * 18, String(targets[i].name) + " " + String(targets[i].expected));
  }

  int minimum = 4095;
  int maximum = 0;
  long total = 0;
  const int samples = 30;

  Serial.println();
  Serial.println("Sampling current ladder value:");

  for (int i = 0; i < samples; i++) {
    int value = readButtonADCStable();
    minimum = min(minimum, value);
    maximum = max(maximum, value);
    total += value;

    Serial.print("sample ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(value);

    displayDrawStatusBar(String("ADC ") + String(value) + " sample " + String(i + 1) + "/" + String(samples));
    displayDelay(60);
  }

  int average = total / samples;

  displayStopAnimation();
  displayDrawStatusBar(String("ADC avg ") + String(average));

  Serial.println();
  Serial.print("Average: ");
  Serial.println(average);
  Serial.print("Min: ");
  Serial.println(minimum);
  Serial.print("Max: ");
  Serial.println(maximum);

  if (maximum - minimum <= BTN_ADC_TOLERANCE) {
    uiShowStatusOK("Button ADC sample stable.");
  } else {
    uiShowStatusError("Button ADC sample noisy.");
  }

  Serial.println("========================================");
}
