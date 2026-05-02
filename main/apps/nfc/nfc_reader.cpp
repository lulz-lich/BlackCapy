#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

#include "hardware_config.h"
#include "logger.h"
#include "module_manager.h"
#include "module_manifest.h"
#include "capture_writer.h"

static Adafruit_PN532 nfc(
  NFC_IRQ_PIN,
  NFC_RST_PIN
);

void runNFCReader() {
  Serial.println();
  Serial.println("========== NFC READER ==========");

  if (!moduleManagerHas(MODULE_NFC)) {
    Serial.println("NFC module not detected.");
    logWarn("NFC reader blocked: module not detected.");
    return;
  }

  nfc.begin();

  uint32_t version = nfc.getFirmwareVersion();

  if (!version) {
    Serial.println("PN532 not detected.");
    logWarn("PN532 not detected.");
    return;
  }

  Serial.print("PN532 firmware: ");
  Serial.print((version >> 16) & 0xFF, DEC);
  Serial.print(".");
  Serial.println((version >> 8) & 0xFF, DEC);

  nfc.SAMConfig();

  Serial.println("Waiting for NFC tag for 8 seconds...");

  uint8_t uid[7];
  uint8_t uidLength;

  bool success = nfc.readPassiveTargetID(
    PN532_MIFARE_ISO14443A,
    uid,
    &uidLength,
    8000
  );

  if (!success) {
    Serial.println("No NFC tag detected.");
    logWarn("NFC reader finished with no tag.");
    return;
  }

  String uidString = "";

  Serial.print("UID Length: ");
  Serial.println(uidLength);

  Serial.print("UID: ");

  for (uint8_t i = 0; i < uidLength; i++) {
    if (uid[i] < 0x10) {
      Serial.print("0");
      uidString += "0";
    }

    Serial.print(uid[i], HEX);
    Serial.print(" ");

    uidString += String(uid[i], HEX);
    uidString += " ";
  }

  Serial.println();

  captureWriteLine(
    "nfc",
    "UID=" + uidString +
    " LENGTH=" + String(uidLength)
  );

  logInfo("NFC UID captured.");
  Serial.println("================================");
}