#include <Arduino.h>

#include "hardware_config.h"

HardwareSerial BlackCapyUART(2);

void runUARTMonitor() {
  Serial.println();
  Serial.println("========== UART MONITOR ==========");

  BlackCapyUART.begin(UART_BAUDRATE, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

  Serial.print("UART2 started at ");
  Serial.print(UART_BAUDRATE);
  Serial.println(" baud");

  Serial.print("RX: GPIO ");
  Serial.println(UART_RX_PIN);

  Serial.print("TX: GPIO ");
  Serial.println(UART_TX_PIN);

  Serial.println();
  Serial.println("Listening for 5 seconds...");

  unsigned long start = millis();

  while (millis() - start < 5000) {
    if (BlackCapyUART.available()) {
      char c = BlackCapyUART.read();
      Serial.write(c);
    }
  }

  Serial.println();
  Serial.println("UART monitor finished.");
  Serial.println("==================================");
}
