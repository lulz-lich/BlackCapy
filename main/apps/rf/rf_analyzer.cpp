#include <Arduino.h>
#include <RadioLib.h>

#include "hardware_config.h"
#include "logger.h"
#include "module_manager.h"
#include "module_manifest.h"
#include "capture_writer.h"

static CC1101 rf = new Module(
  RF_CS_PIN,
  RF_GDO0_PIN,
  RF_RST_PIN,
  RF_GDO2_PIN
);

void runRFAnalyzer() {
  Serial.println();
  Serial.println("========== RF ANALYZER ==========");

  if (!moduleManagerHas(MODULE_RF)) {
    Serial.println("RF module not detected.");
    logWarn("RF analyzer blocked: module not detected.");
    return;
  }

  int state = rf.begin(RF_FREQUENCY);

  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("CC1101 init failed. Code: ");
    Serial.println(state);
    logError("RF module initialization failed.");
    return;
  }

  Serial.println("RF module detected.");

  Serial.print("Frequency: ");
  Serial.print(RF_FREQUENCY);
  Serial.println(" MHz");

  Serial.println("Sampling RSSI for 8 seconds...");

  unsigned long start = millis();

  while (millis() - start < 8000) {
    float rssi = rf.getRSSI();

    Serial.print("RSSI: ");
    Serial.print(rssi);
    Serial.println(" dBm");

    captureWriteLine(
      "rf",
      "FREQ=" + String(RF_FREQUENCY) +
      " RSSI=" + String(rssi)
    );

    delay(500);
  }

  logInfo("RF analyzer finished.");
  Serial.println("=================================");
}