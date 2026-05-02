#include "app_manager.h"

#include "logger.h"
#include "event_bus.h"
#include "status_manager.h"
#include "module_manager.h"
#include "module_manifest.h"

static AppEntry* registeredApps = nullptr;
static int registeredAppCount = 0;
static int selectedAppIndex = 0;

static bool appRequiredModuleAvailable(AppEntry* app) {
  if (app == nullptr) {
    return false;
  }

  if (app->permissions & APP_PERMISSION_IR) {
    return moduleManagerHas(MODULE_IR);
  }

  if (app->permissions & APP_PERMISSION_RF) {
    return moduleManagerHas(MODULE_RF);
  }

  if (app->permissions & APP_PERMISSION_RFID) {
    return moduleManagerHas(MODULE_RFID);
  }

  if (app->permissions & APP_PERMISSION_NFC) {
    return moduleManagerHas(MODULE_NFC);
  }

  if (app->permissions & APP_PERMISSION_CAN) {
    return moduleManagerHas(MODULE_CAN);
  }

  if (app->permissions & APP_PERMISSION_GPS) {
    return moduleManagerHas(MODULE_GPS);
  }

  if (app->permissions & APP_PERMISSION_LORA) {
    return moduleManagerHas(MODULE_LORA);
  }

  return true;
}

void appManagerInit(AppEntry* apps, int count) {
  registeredApps = apps;
  registeredAppCount = count;
  selectedAppIndex = 0;

  logInfo("App manager initialized.");
  logInfo("Apps loaded: " + String(count));
}

int appManagerCount() {
  return registeredAppCount;
}

int appManagerSelectedIndex() {
  return selectedAppIndex;
}

AppEntry* appManagerGet(int index) {
  if (registeredApps == nullptr) {
    return nullptr;
  }

  if (index < 0 || index >= registeredAppCount) {
    return nullptr;
  }

  return &registeredApps[index];
}

AppEntry* appManagerGetById(int id) {
  if (registeredApps == nullptr) {
    return nullptr;
  }

  for (int i = 0; i < registeredAppCount; i++) {
    if (registeredApps[i].id == id) {
      return &registeredApps[i];
    }
  }

  return nullptr;
}

AppEntry* appManagerGetSelected() {
  return appManagerGet(selectedAppIndex);
}

void appManagerNext() {
  if (registeredAppCount <= 0) {
    return;
  }

  selectedAppIndex++;

  if (selectedAppIndex >= registeredAppCount) {
    selectedAppIndex = 0;
  }
}

void appManagerPrevious() {
  if (registeredAppCount <= 0) {
    return;
  }

  selectedAppIndex--;

  if (selectedAppIndex < 0) {
    selectedAppIndex = registeredAppCount - 1;
  }
}

bool appManagerIsRunnable(AppEntry* app) {
  if (app == nullptr) {
    return false;
  }

  if (app->status == APP_STATUS_DISABLED) {
    return false;
  }

  if (app->run == nullptr) {
    return false;
  }

  if (!appRequiredModuleAvailable(app)) {
    return false;
  }

  return true;
}

void appManagerPrintApp(AppEntry* app) {
  if (app == nullptr) {
    return;
  }

  Serial.print("[");
  Serial.print(app->id);
  Serial.print("] ");

  Serial.print(app->name);
  Serial.print(" | ");
  Serial.print(app->category);
  Serial.print(" | ");
  Serial.print(appStatusToString(app->status));
  Serial.print(" | perm: ");
  Serial.print(appPermissionToString(app->permissions));

  if (!appManagerIsRunnable(app)) {
    Serial.print(" | LOCKED");
  }

  Serial.println();

  Serial.print("    ");
  Serial.println(app->description);
}

static void runApp(AppEntry* app) {
  if (app == nullptr) {
    logError("Invalid app execution request.");
    return;
  }

  if (app->status == APP_STATUS_DISABLED) {
    logWarn(String("App disabled: ") + app->name);
    return;
  }

  if (app->run == nullptr) {
    logError(String("App has no run function: ") + app->name);
    return;
  }

  if (!appRequiredModuleAvailable(app)) {
    logWarn(String("Required module not detected for app: ") + app->name);

    Serial.println();
    Serial.println("This app requires an external module.");
    Serial.println("Connect the module or enable development mock detection.");

    return;
  }

  statusSet(STATUS_RUNNING_TOOL);
  eventEmit(EVENT_TOOL_STARTED, app->name);

  Serial.println();
  Serial.print("[APP] Running: ");
  Serial.println(app->name);

  app->run();

  eventEmit(EVENT_TOOL_FINISHED, app->name);
  statusSet(STATUS_IDLE);
}

void appManagerRunSelected() {
  runApp(appManagerGetSelected());
}

bool appManagerRunById(int id) {
  AppEntry* app = appManagerGetById(id);

  if (app == nullptr) {
    return false;
  }

  runApp(app);
  return true;
}