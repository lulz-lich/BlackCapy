#include <Arduino.h>

void runSystemBenchmark() {
  Serial.println();
  Serial.println("========== SYSTEM BENCHMARK ==========");

  unsigned long start = millis();

  volatile long sum = 0;

  for (long i = 0; i < 1000000; i++) {
    sum += i;
  }

  unsigned long end = millis();

  Serial.print("Loop time (1M ops): ");
  Serial.print(end - start);
  Serial.println(" ms");

  Serial.print("Result checksum: ");
  Serial.println(sum);

  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());

  Serial.println("=====================================");
}
