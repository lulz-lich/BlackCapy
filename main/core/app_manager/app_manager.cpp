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
    if (!moduleManagerHas(MODULE_IR)) {
      return false;
    }
  }

  if (app->permissions & APP_PERMISSION_RF) {
    if (!moduleManagerHas(MODULE_RF)) {
      return false;
    }
  }

  if (app->permissions & APP_PERMISSION_RFID) {
    if (!moduleManagerHas(MODULE_RFID)) {
      return false;
    }
  }

  if (app->permissions & APP_PERMISSION_NFC) {
    if (!moduleManagerHas(MODULE_NFC)) {
      return false;
    }
  }

  if (app->permissions & APP_PERMISSION_CAN) {
    if (!moduleManagerHas(MODULE_CAN)) {
      return false;
    }
  }

  if (app->permissions & APP_PERMISSION_GPS) {
    if (!moduleManagerHas(MODULE_GPS)) {
      return false;
    }
  }

  if (app->permissions & APP_PERMISSION_LORA) {
    if (!moduleManagerHas(MODULE_LORA)) {
      return false;
    }
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

void appManagerNextPage() {
  if (registeredAppCount <= 0) {
    return;
  }

  const int visibleItems = APP_MANAGER_VISIBLE_ITEMS;
  int page = selectedAppIndex / visibleItems;
  int nextIndex = (page + 1) * visibleItems;

  if (nextIndex >= registeredAppCount) {
    nextIndex = 0;
  }

  selectedAppIndex = nextIndex;
}

void appManagerPreviousPage() {
  if (registeredAppCount <= 0) {
    return;
  }

  const int visibleItems = APP_MANAGER_VISIBLE_ITEMS;
  int page = selectedAppIndex / visibleItems;
  int previousIndex = (page - 1) * visibleItems;

  if (page == 0) {
    int lastPage = (registeredAppCount - 1) / visibleItems;
    previousIndex = lastPage * visibleItems;
  }

  selectedAppIndex = previousIndex;
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

void appManagerPrintAll() {
  Serial.println();
  Serial.println("========== APP MANAGER ==========");

  if (registeredApps == nullptr || registeredAppCount <= 0) {
    Serial.println("No apps registered.");
    Serial.println("=================================");
    return;
  }

  for (int i = 0; i < registeredAppCount; i++) {
    appManagerPrintApp(&registeredApps[i]);
  }

  Serial.println("=================================");
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
