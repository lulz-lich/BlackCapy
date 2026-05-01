#include <Arduino.h>

void runTimerTool() {
  Serial.println();
  Serial.println("========== TIMER TOOL ==========");

  Serial.println("Measuring execution delay...");

  unsigned long start = micros();

  delay(100);

  unsigned long end = micros();

  Serial.print("Elapsed time: ");
  Serial.print(end - start);
  Serial.println(" microseconds");

  Serial.println("================================");
}