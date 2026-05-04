#include <Arduino.h>

#include "hardware_config.h"

void runGPIOConsole() {
  Serial.println();
  Serial.println("========== GPIO CONSOLE ==========");

  pinMode(STATUS_LED_PIN, OUTPUT);

  Serial.print("Testing status LED on GPIO ");
  Serial.println(STATUS_LED_PIN);

  for (int i = 0; i < 5; i++) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    Serial.println("LED: HIGH");
    delay(250);

    digitalWrite(STATUS_LED_PIN, LOW);
    Serial.println("LED: LOW");
    delay(250);
  }

  Serial.println("GPIO output test finished.");
  Serial.println("==================================");
}
