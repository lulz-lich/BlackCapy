#include <Arduino.h>
#include <TinyGPS++.h>

#include "hardware_config.h"
#include "logger.h"
#include "module_manager.h"
#include "module_manifest.h"
#include "capture_writer.h"

static TinyGPSPlus gps;
static HardwareSerial gpsSerial(1);

void runGPSStatus() {
  Serial.println();
  Serial.println("========== GPS STATUS ==========");

  if (!moduleManagerHas(MODULE_GPS)) {
    Serial.println("GPS module not detected.");
    logWarn("GPS status blocked: module not detected.");
    return;
  }

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

  String line = "";

  if (gps.location.isValid()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);

    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);

    line += "LAT=";
    line += String(gps.location.lat(), 6);
    line += " LON=";
    line += String(gps.location.lng(), 6);
  } else {
    Serial.println("Location: not fixed.");
    line += "LOCATION=NO_FIX";
  }

  uint32_t satellites =
    gps.satellites.isValid()
      ? gps.satellites.value()
      : 0;

  double hdop =
    gps.hdop.isValid()
      ? gps.hdop.hdop()
      : 0.0;

  Serial.print("Satellites: ");
  Serial.println(satellites);

  Serial.print("HDOP: ");
  Serial.println(hdop);

  line += " SAT=";
  line += String(satellites);

  line += " HDOP=";
  line += String(hdop);

  captureWriteLine("gps", line);

  logInfo("GPS status checked.");
  Serial.println("===============================");
}