#include <Arduino.h>
#include <Wire.h>

#include "hardware_config.h"

void runI2CScanner() {
  Serial.println();
  Serial.println("========== I2C SCANNER ==========");

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  int devices = 0;

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Device found at 0x");

      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);

      devices++;
    }
  }

  if (devices == 0) {
    Serial.println("No I2C devices found.");
  } else {
    Serial.print("Total devices: ");
    Serial.println(devices);
  }

  Serial.println("=================================");
}
