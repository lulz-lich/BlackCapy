#include "module_manager.h"

#include <Arduino.h>
#include <Wire.h>

#include "hardware_config.h"
#include "logger.h"

static ModuleType detectedModules[16];
static int detectedModuleCount = 0;

void moduleManagerInit() {
  detectedModuleCount = 0;
  logInfo("Module manager initialized.");
}

void moduleManagerClearDetected() {
  detectedModuleCount = 0;
}

void moduleManagerRegister(ModuleType type) {
  if (type == MODULE_UNKNOWN) {
    return;
  }

  if (moduleManagerHas(type)) {
    return;
  }

  if (detectedModuleCount >= 16) {
    logWarn("Module registry full.");
    return;
  }

  detectedModules[detectedModuleCount] = type;
  detectedModuleCount++;

  logInfo("Module registered: " + String(moduleTypeToString(type)));
}

bool moduleManagerHas(ModuleType type) {
  for (int i = 0; i < detectedModuleCount; i++) {
    if (detectedModules[i] == type) {
      return true;
    }
  }

  return false;
}

bool moduleManagerHas(const String& type) {
  return moduleManagerHas(moduleTypeFromString(type));
}

int moduleManagerReadADCStable() {
  const int samples = 20;
  long total = 0;

  pinMode(MODULE_ID_ADC_PIN, INPUT);

  for (int i = 0; i < samples; i++) {
    total += analogRead(MODULE_ID_ADC_PIN);
    delay(5);
  }

  return total / samples;
}

ModuleType moduleManagerTypeFromADC(int value) {
  if (value >= 300 && value <= 600) {
    return MODULE_IR;
  }

  if (value >= 750 && value <= 1050) {
    return MODULE_RF;
  }

  if (value >= 1200 && value <= 1450) {
    return MODULE_RFID;
  }

  if (value >= 1600 && value <= 1850) {
    return MODULE_NFC;
  }

  if (value >= 2000 && value <= 2250) {
    return MODULE_CAN;
  }

  if (value >= 2400 && value <= 2650) {
    return MODULE_GPS;
  }

  if (value >= 2800 && value <= 3100) {
    return MODULE_LORA;
  }

  return MODULE_UNKNOWN;
}

void moduleManagerDetectByADC() {
  int value = moduleManagerReadADCStable();
  ModuleType type = moduleManagerTypeFromADC(value);

  Serial.print("Module ID ADC stable value: ");
  Serial.println(value);

  if (type == MODULE_UNKNOWN) {
    logWarn("No valid module ID detected.");
    return;
  }

  moduleManagerRegister(type);
  logInfo("Module detected by ADC: " + String(moduleTypeToString(type)));
}

void moduleManagerDetectMock() {
  /*
    Development-only mock detection.

    Descomente os módulos que quiser simular enquanto
    ainda não existe hardware real conectado.

    Exemplo:
    moduleManagerRegister(MODULE_NFC);
  */

  // moduleManagerRegister(MODULE_IR);
  // moduleManagerRegister(MODULE_RF);
  // moduleManagerRegister(MODULE_RFID);
  // moduleManagerRegister(MODULE_NFC);
  // moduleManagerRegister(MODULE_CAN);
  // moduleManagerRegister(MODULE_GPS);
  // moduleManagerRegister(MODULE_LORA);
}

void moduleManagerPrintDetected() {
  Serial.println();
  Serial.println("========== DETECTED MODULES ==========");

  if (detectedModuleCount == 0) {
    Serial.println("No modules registered.");
    Serial.println("======================================");
    return;
  }

  for (int i = 0; i < detectedModuleCount; i++) {
    Serial.print("[");
    Serial.print(i + 1);
    Serial.print("] ");
    Serial.println(moduleTypeToString(detectedModules[i]));
  }

  Serial.println("======================================");
}

void runModuleManager() {
  Serial.println();
  Serial.println("========== MODULE MANAGER ==========");

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  Serial.println("Scanning I2C bus...");

  int i2cDevices = 0;

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at 0x");

      if (address < 16) {
        Serial.print("0");
      }

      Serial.println(address, HEX);
      i2cDevices++;
    }
  }

  if (i2cDevices == 0) {
    Serial.println("No I2C devices found.");
  } else {
    Serial.print("I2C devices found: ");
    Serial.println(i2cDevices);
  }

  Serial.println();
  Serial.println("Running module detection...");

  moduleManagerDetectMock();
  moduleManagerDetectByADC();

  moduleManagerPrintDetected();

  Serial.println("SPI bus: ready");
  Serial.println("UART bus: ready");
  Serial.println("ADC module ID: ready");
  Serial.println("Module API: ready");

  Serial.println("====================================");
}