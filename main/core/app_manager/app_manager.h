#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <Arduino.h>

#include "app_contract.h"

#define APP_MANAGER_VISIBLE_ITEMS 4

struct AppEntry {
  int id;
  const char* name;
  const char* category;
  const char* description;
  int permissions;
  AppStatus status;
  void (*run)();
};
bool appManagerIsRunnable(AppEntry* app);
void appManagerPrintApp(AppEntry* app);
void appManagerInit(AppEntry* apps, int count);

int appManagerCount();
AppEntry* appManagerGet(int index);
AppEntry* appManagerGetById(int id);
AppEntry* appManagerGetSelected();

void appManagerNext();
void appManagerPrevious();
void appManagerNextPage();
void appManagerPreviousPage();

void appManagerRunSelected();
bool appManagerRunById(int id);

void appManagerPrintAll();

int appManagerSelectedIndex();

#endif
