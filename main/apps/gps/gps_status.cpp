#include <Arduino.h>
#include <TinyGPS++.h>

#include "hardware_config.h"
#include "logger.h"

static TinyGPSPlus gps;
static HardwareSerial gpsSerial(1);

void runGPSStatus() {
  Serial.println();
  Serial.println("========== GPS STATUS ==========");

  gpsSerial.begin(
    GPS_BAUDRATE,
    SERIAL_8N1,
    GPS_RX_PIN,
    GPS_TX_PIN
  );

  Serial.println("Reading GPS data for 12 seconds...");

  unsigned long start = millis();

  while (millis() - start < 12000) {
    while (gpsSerial.available()) {
      gps.encode(gpsSerial.read());
    }
  }

  if (gps.location.isValid()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);

    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println("Location: not fixed.");
  }

  Serial.print("Satellites: ");
  Serial.println(gps.satellites.isValid() ? gps.satellites.value() : 0);

  Serial.print("HDOP: ");
  Serial.println(gps.hdop.isValid() ? gps.hdop.hdop() : 0.0);

  logInfo("GPS status checked.");
  Serial.println("===============================");
}