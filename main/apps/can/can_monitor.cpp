#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

#include "hardware_config.h"
#include "logger.h"
#include "module_manager.h"
#include "module_manifest.h"
#include "capture_writer.h"

static MCP2515 mcp2515(CAN_CS_PIN);

void runCANMonitor() {
  Serial.println();
  Serial.println("========== CAN MONITOR ==========");

  if (!moduleManagerHas(MODULE_CAN)) {
    Serial.println("CAN module not detected.");
    logWarn("CAN monitor blocked: module not detected.");
    return;
  }

  SPI.begin();

  mcp2515.reset();

  if (
    mcp2515.setBitrate(
      CAN_SPEED,
      MCP_8MHZ
    ) != MCP2515::ERROR_OK
  ) {
    Serial.println("CAN bitrate setup failed.");
    logError("CAN bitrate setup failed.");
    return;
  }

  if (
    mcp2515.setNormalMode()
    != MCP2515::ERROR_OK
  ) {
    Serial.println("CAN normal mode failed.");
    logError("CAN normal mode failed.");
    return;
  }

  Serial.println("CAN monitor active for 8 seconds...");

  unsigned long start = millis();
  bool received = false;

  while (millis() - start < 8000) {
    struct can_frame frame;

    MCP2515::ERROR error =
      mcp2515.readMessage(&frame);

    if (error != MCP2515::ERROR_OK) {
      delay(10);
      continue;
    }

    received = true;

    String line = "";

    line += "ID=0x";
    line += String(frame.can_id, HEX);

    line += " DLC=";
    line += String(frame.can_dlc);

    line += " DATA=";

    Serial.print("CAN ID: 0x");
    Serial.print(frame.can_id, HEX);

    Serial.print(" DLC: ");
    Serial.print(frame.can_dlc);

    Serial.print(" DATA: ");

    for (int i = 0; i < frame.can_dlc; i++) {
      if (frame.data[i] < 0x10) {
        Serial.print("0");
        line += "0";
      }

      Serial.print(frame.data[i], HEX);
      Serial.print(" ");

      line += String(frame.data[i], HEX);
      line += " ";
    }

    Serial.println();

    captureWriteLine("can", line);
  }

  if (!received) {
    Serial.println("No CAN frames received.");
    logWarn("CAN monitor finished with no frames.");
  } else {
    logInfo("CAN monitor captured frames.");
  }

  Serial.println("=================================");
}