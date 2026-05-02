#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

#include "hardware_config.h"
#include "logger.h"

static MCP2515 mcp2515(CAN_CS_PIN);

void runCANMonitor() {
  Serial.println();
  Serial.println("========== CAN MONITOR ==========");

  SPI.begin();

  mcp2515.reset();

  if (mcp2515.setBitrate(CAN_SPEED, MCP_8MHZ) != MCP2515::ERROR_OK) {
    Serial.println("Failed to configure MCP2515 bitrate.");
    logError("CAN bitrate configuration failed.");
    return;
  }

  if (mcp2515.setNormalMode() != MCP2515::ERROR_OK) {
    Serial.println("Failed to enter CAN normal mode.");
    logError("CAN normal mode failed.");
    return;
  }

  Serial.println("CAN monitor active for 8 seconds...");

  unsigned long start = millis();
  struct can_frame frame;
  bool received = false;

  while (millis() - start < 8000) {
    if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
      Serial.print("CAN ID: 0x");
      Serial.print(frame.can_id, HEX);

      Serial.print(" DLC: ");
      Serial.print(frame.can_dlc);

      Serial.print(" DATA: ");

      for (int i = 0; i < frame.can_dlc; i++) {
        if (frame.data[i] < 0x10) {
          Serial.print("0");
        }

        Serial.print(frame.data[i], HEX);
        Serial.print(" ");
      }

      Serial.println();
      received = true;
    }
  }

  if (!received) {
    Serial.println("No CAN frames received.");
  }

  logInfo("CAN monitor finished.");
  Serial.println("=================================");
}