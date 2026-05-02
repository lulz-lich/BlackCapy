#include <Arduino.h>
#include <RadioLib.h>

#include "hardware_config.h"
#include "logger.h"

static CC1101 rf = new Module(
  RF_CS_PIN,
  RF_GDO0_PIN,
  RF_RST_PIN,
  RF_GDO2_PIN
);

void runRFAnalyzer() {
  Serial.println();
  Serial.println("========== RF ANALYZER ==========");

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
    Serial.print("RSSI: ");
    Serial.print(rf.getRSSI());
    Serial.println(" dBm");

    delay(500);
  }

  logInfo("RF analyzer finished.");
  Serial.println("=================================");
}