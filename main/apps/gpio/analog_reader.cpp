#include <Arduino.h>

#define ANALOG_PIN 34

void runAnalogReader() {
  Serial.println();
  Serial.println("========== ANALOG READER ==========");

  Serial.print("Reading from GPIO ");
  Serial.println(ANALOG_PIN);

  for (int i = 0; i < 10; i++) {
    int value = analogRead(ANALOG_PIN);

    Serial.print("Value: ");
    Serial.println(value);

    delay(500);
  }

  Serial.println("==================================");
}