#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <Arduino.h>

struct AppEntry {
  int id;
  const char* name;
  const char* category;
  const char* description;
  void (*run)();
};

void appManagerInit(AppEntry* apps, int count);

int appManagerCount();
AppEntry* appManagerGet(int index);
AppEntry* appManagerGetById(int id);
AppEntry* appManagerGetSelected();

void appManagerNext();
void appManagerPrevious();

void appManagerRunSelected();
bool appManagerRunById(int id);

int appManagerSelectedIndex();

#endif