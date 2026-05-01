#include <Arduino.h>
#include "blackcapy.h"

void runGPIOConsole() {
  Serial.println();
  Serial.println("========== GPIO CONSOLE ==========");

  pinMode(BLACKCAPY_STATUS_LED, OUTPUT);

  Serial.print("Testing status LED on GPIO ");
  Serial.println(BLACKCAPY_STATUS_LED);

  for (int i = 0; i < 5; i++) {
    digitalWrite(BLACKCAPY_STATUS_LED, HIGH);
    Serial.println("LED: HIGH");
    delay(250);

    digitalWrite(BLACKCAPY_STATUS_LED, LOW);
    Serial.println("LED: LOW");
    delay(250);
  }

  Serial.println("GPIO output test finished.");
  Serial.println("==================================");
}
