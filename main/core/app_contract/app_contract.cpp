#include "app_contract.h"

const char* appPermissionToString(int permissions) {
  if (permissions == APP_PERMISSION_NONE) {
    return "none";
  }

  if (permissions & APP_PERMISSION_SYSTEM) {
    return "system";
  }

  if (permissions & APP_PERMISSION_MODULES) {
    return "modules";
  }

  if (permissions & APP_PERMISSION_STORAGE) {
    return "storage";
  }

  if (permissions & APP_PERMISSION_GPIO) {
    return "gpio";
  }

  if (permissions & APP_PERMISSION_BLE) {
    return "ble";
  }

  if (permissions & APP_PERMISSION_WIFI) {
    return "wifi";
  }

  if (permissions & APP_PERMISSION_IR) {
    return "ir";
  }

  if (permissions & APP_PERMISSION_RF) {
    return "rf";
  }

  if (permissions & APP_PERMISSION_RFID) {
    return "rfid";
  }

  if (permissions & APP_PERMISSION_NFC) {
    return "nfc";
  }

  if (permissions & APP_PERMISSION_CAN) {
    return "can";
  }

  if (permissions & APP_PERMISSION_GPS) {
    return "gps";
  }

  if (permissions & APP_PERMISSION_LORA) {
    return "lora";
  }

  return "unknown";
}

const char* appStatusToString(AppStatus status) {
  switch (status) {
    case APP_STATUS_STABLE:
      return "stable";

    case APP_STATUS_EXPERIMENTAL:
      return "experimental";

    case APP_STATUS_DISABLED:
      return "disabled";

    default:
      return "unknown";
  }
}
