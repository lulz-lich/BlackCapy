#include "app_manager.h"

#include "logger.h"
#include "event_bus.h"
#include "status_manager.h"

static AppEntry* registeredApps = nullptr;
static int registeredAppCount = 0;
static int selectedAppIndex = 0;

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

static void runApp(AppEntry* app) {
  if (!appManagerIsRunnable(app)) {
  logError("App cannot be executed.");
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

int appManagerSelectedIndex() {
  return selectedAppIndex;
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
  Serial.println(appPermissionToString(app->permissions));

  Serial.print("    ");
  Serial.println(app->description);
}