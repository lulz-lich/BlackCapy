#include <Arduino.h>
#include <IRrecv.h>
#include <IRutils.h>

#include "hardware_config.h"
#include "logger.h"

static IRrecv irReceiver(IR_RX_PIN);
static decode_results irResults;

void runIRConsole() {
  Serial.println();
  Serial.println("========== IR CONSOLE ==========");

  irReceiver.enableIRIn();

  Serial.println("Listening for IR signal for 8 seconds...");

  unsigned long start = millis();
  bool found = false;

  while (millis() - start < 8000) {
    if (irReceiver.decode(&irResults)) {
      Serial.println("IR signal detected.");
      Serial.print("Protocol: ");
      Serial.println(typeToString(irResults.decode_type));

      Serial.print("Bits: ");
      Serial.println(irResults.bits);

      Serial.print("Value: 0x");
      serialPrintUint64(irResults.value, HEX);
      Serial.println();

      found = true;
      irReceiver.resume();
      break;
    }
  }

  if (!found) {
    Serial.println("No IR signal detected.");
  }

  logInfo("IR console finished.");
  Serial.println("================================");
}