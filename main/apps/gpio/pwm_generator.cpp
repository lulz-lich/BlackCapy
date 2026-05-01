#include <Arduino.h>

#define PWM_PIN 18
#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

void runPWMGenerator() {
  Serial.println();
  Serial.println("========== PWM GENERATOR ==========");

  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWM_PIN, PWM_CHANNEL);

  Serial.print("Output on GPIO ");
  Serial.println(PWM_PIN);

  for (int duty = 0; duty <= 255; duty += 25) {
    ledcWrite(PWM_CHANNEL, duty);

    Serial.print("Duty: ");
    Serial.println(duty);

    delay(300);
  }

  ledcWrite(PWM_CHANNEL, 0);

  Serial.println("PWM test completed.");
  Serial.println("===================================");
}