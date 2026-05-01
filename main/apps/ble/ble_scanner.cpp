#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>

void runBLEScanner() {
  Serial.println();
  Serial.println("========== BLE SCANNER ==========");

  BLEDevice::init("");
  BLEScan* scanner = BLEDevice::getScan();

  scanner->setActiveScan(true);
  BLEScanResults results = scanner->start(5, false);

  int count = results.getCount();

  Serial.print("Devices found: ");
  Serial.println(count);

  for (int i = 0; i < count; i++) {
    BLEAdvertisedDevice device = results.getDevice(i);

    Serial.print("[");
    Serial.print(i + 1);
    Serial.print("] ");

    Serial.print("Address: ");
    Serial.print(device.getAddress().toString().c_str());

    Serial.print(" | RSSI: ");
    Serial.print(device.getRSSI());

    if (device.haveName()) {
      Serial.print(" | Name: ");
      Serial.print(device.getName().c_str());
    }

    Serial.println();
  }

  Serial.println("=================================");
}
