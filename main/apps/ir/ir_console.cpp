#include <Arduino.h>
#include <IRrecv.h>
#include <IRutils.h>

#include "hardware_config.h"
#include "logger.h"
#include "module_manager.h"
#include "module_manifest.h"
#include "capture_writer.h"

static IRrecv irReceiver(IR_RX_PIN);
static decode_results irResults;

static String uint64ToStringLocal(uint64_t value, uint8_t base) {
  char buffer[65];
  char* ptr = &buffer[64];
  *ptr = '\0';

  do {
    uint8_t digit = value % base;
    *--ptr = digit < 10 ? '0' + digit : 'A' + digit - 10;
    value /= base;
  } while (value > 0);

  return String(ptr);
}

void runIRConsole() {
  Serial.println();
  Serial.println("========== IR CONSOLE ==========");

  if (!moduleManagerHas(MODULE_IR)) {
    Serial.println("IR module not detected.");
    logWarn("IR console blocked: module not detected.");
    return;
  }

  irReceiver.enableIRIn();

  Serial.println("Listening for IR signal for 8 seconds...");

  unsigned long start = millis();
  bool found = false;

  while (millis() - start < 8000) {
    if (irReceiver.decode(&irResults)) {
      String protocol = typeToString(irResults.decode_type);
      String value = uint64ToStringLocal(irResults.value, 16);

      Serial.println("IR signal detected.");

      Serial.print("Protocol: ");
      Serial.println(protocol);

      Serial.print("Bits: ");
      Serial.println(irResults.bits);

      Serial.print("Value: 0x");
      Serial.println(value);

      captureWriteLine(
        "ir",
        "PROTOCOL=" + protocol +
        " BITS=" + String(irResults.bits) +
        " VALUE=0x" + value
      );

      found = true;
      irReceiver.resume();
      break;
    }
  }

  if (!found) {
    Serial.println("No IR signal detected.");
    logWarn("IR console finished with no signal.");
  } else {
    logInfo("IR signal captured.");
  }

  Serial.println("================================");
}