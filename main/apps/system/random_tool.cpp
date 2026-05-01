#include <Arduino.h>

void runRandomTool() {
  Serial.println();
  Serial.println("========== RANDOM GENERATOR ==========");

  randomSeed(esp_random());

  for (int i = 0; i < 10; i++) {
    int value = random(0, 1000);

    Serial.print("Random: ");
    Serial.println(value);
  }

  Serial.println("=====================================");
}