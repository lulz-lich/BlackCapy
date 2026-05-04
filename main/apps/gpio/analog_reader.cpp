#include <Arduino.h>

#include "hardware_config.h"

void runAnalogReader() {
  Serial.println();
  Serial.println("========== ANALOG READER ==========");

  Serial.print("Reading from GPIO ");
  Serial.println(ANALOG_TEST_PIN);

  for (int i = 0; i < 10; i++) {
    int value = analogRead(ANALOG_TEST_PIN);

    Serial.print("Value: ");
    Serial.println(value);

    delay(500);
  }

  Serial.println("==================================");
}
