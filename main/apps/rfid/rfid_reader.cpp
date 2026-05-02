#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#include "hardware_config.h"
#include "logger.h"
#include "module_manager.h"
#include "module_manifest.h"
#include "capture_writer.h"

static MFRC522 rfid(
  RFID_CS_PIN,
  RFID_RST_PIN
);

void runRFIDReader() {
  Serial.println();
  Serial.println("========== RFID READER ==========");

  if (!moduleManagerHas(MODULE_RFID)) {
    Serial.println("RFID module not detected.");
    logWarn("RFID reader blocked: module not detected.");
    return;
  }

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Waiting for RFID card for 8 seconds...");

  unsigned long start = millis();
  bool found = false;

  while (millis() - start < 8000) {
    if (!rfid.PICC_IsNewCardPresent()) {
      delay(50);
      continue;
    }

    if (!rfid.PICC_ReadCardSerial()) {
      delay(50);
      continue;
    }

    String uidString = "";

    Serial.print("UID: ");

    for (byte i = 0; i < rfid.uid.size; i++) {
      if (rfid.uid.uidByte[i] < 0x10) {
        Serial.print("0");
        uidString += "0";
      }

      Serial.print(rfid.uid.uidByte[i], HEX);
      Serial.print(" ");

      uidString += String(rfid.uid.uidByte[i], HEX);
      uidString += " ";
    }

    Serial.println();

    captureWriteLine(
      "rfid",
      "UID=" + uidString +
      " SIZE=" + String(rfid.uid.size)
    );

    found = true;

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    break;
  }

  if (!found) {
    Serial.println("No RFID card detected.");
    logWarn("RFID reader finished with no card.");
  } else {
    logInfo("RFID UID captured.");
  }

  Serial.println("=================================");
}