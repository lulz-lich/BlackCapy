#include <Arduino.h>
#include <RadioLib.h>

#include "hardware_config.h"
#include "logger.h"
#include "module_manager.h"
#include "module_manifest.h"
#include "capture_writer.h"

static SX1278 lora = new Module(
  LORA_CS_PIN,
  LORA_DIO0_PIN,
  LORA_RST_PIN,
  LORA_DIO1_PIN
);

void runLoRaStatus() {
  Serial.println();
  Serial.println("========== LORA STATUS ==========");

  if (!moduleManagerHas(MODULE_LORA)) {
    Serial.println("LoRa module not detected.");
    logWarn("LoRa status blocked: module not detected.");
    return;
  }

  int state = lora.begin(LORA_FREQUENCY);

  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("LoRa init failed. Code: ");
    Serial.println(state);
    logError("LoRa module initialization failed.");
    return;
  }

  float rssi = lora.getRSSI();

  Serial.println("LoRa module detected.");

  Serial.print("Frequency: ");
  Serial.print(LORA_FREQUENCY);
  Serial.println(" MHz");

  Serial.print("RSSI: ");
  Serial.print(rssi);
  Serial.println(" dBm");

  captureWriteLine(
    "lora",
    "FREQ=" + String(LORA_FREQUENCY) +
    " RSSI=" + String(rssi)
  );

  logInfo("LoRa status checked.");
  Serial.println("=================================");
}