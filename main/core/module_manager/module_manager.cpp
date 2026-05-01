#include <Arduino.h>
#include <Wire.h>

#define I2C_SDA 21
#define I2C_SCL 22

void runModuleManager() {
  Serial.println();
  Serial.println("========== MODULE MANAGER ==========");

  Wire.begin(I2C_SDA, I2C_SCL);

  int devices = 0;

  Serial.println("Scanning I2C expansion bus...");

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at 0x");

      if (address < 16) {
        Serial.print("0");
      }

      Serial.println(address, HEX);
      devices++;
    }
  }

  if (devices == 0) {
    Serial.println("No I2C modules detected.");
  } else {
    Serial.print("Detected modules/devices: ");
    Serial.println(devices);
  }

  Serial.println("SPI bus: reserved");
  Serial.println("UART bus: reserved");
  Serial.println("Module API: ready");

  Serial.println("====================================");
}
