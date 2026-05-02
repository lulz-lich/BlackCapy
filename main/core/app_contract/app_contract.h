#ifndef APP_CONTRACT_H
#define APP_CONTRACT_H

#include <Arduino.h>

enum AppPermission {
  APP_PERMISSION_NONE      = 0,
  APP_PERMISSION_WIFI      = 1 << 0,
  APP_PERMISSION_BLE       = 1 << 1,
  APP_PERMISSION_GPIO      = 1 << 2,
  APP_PERMISSION_STORAGE   = 1 << 3,
  APP_PERMISSION_MODULES   = 1 << 4,
  APP_PERMISSION_IR        = 1 << 6,
  APP_PERMISSION_RF        = 1 << 7,
  APP_PERMISSION_RFID      = 1 << 8,
  APP_PERMISSION_NFC       = 1 << 9,
  APP_PERMISSION_CAN       = 1 << 10,
  APP_PERMISSION_GPS       = 1 << 11,
  APP_PERMISSION_LORA      = 1 << 12,
  APP_PERMISSION_SYSTEM    = 1 << 5
};

enum AppStatus {
  APP_STATUS_STABLE,
  APP_STATUS_EXPERIMENTAL,
  APP_STATUS_DISABLED
};

const char* appPermissionToString(int permissions);
const char* appStatusToString(AppStatus status);

#endif