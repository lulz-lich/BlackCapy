#include <Arduino.h>

#include "hardware_config.h"

void runPWMGenerator() {
  Serial.println();
  Serial.println("========== PWM GENERATOR ==========");

  ledcSetup(PWM_TEST_CHANNEL, PWM_TEST_FREQUENCY, PWM_TEST_RESOLUTION);
  ledcAttachPin(PWM_TEST_PIN, PWM_TEST_CHANNEL);

  Serial.print("Output on GPIO ");
  Serial.println(PWM_TEST_PIN);

  for (int duty = 0; duty <= 255; duty += 25) {
    ledcWrite(PWM_TEST_CHANNEL, duty);

    Serial.print("Duty: ");
    Serial.println(duty);

    delay(300);
  }

  ledcWrite(PWM_TEST_CHANNEL, 0);

  Serial.println("PWM test completed.");
  Serial.println("===================================");
}
