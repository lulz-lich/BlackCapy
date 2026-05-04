#ifndef CAPTURE_WRITER_H
#define CAPTURE_WRITER_H

#include <Arduino.h>

void captureWriteLine(const String& category, const String& line);
String capturePathFor(const String& category);
bool captureCategoryIsSupported(const String& category);
int captureCategoryCount();
const char* captureCategoryAt(int index);
bool captureExportToCSV(const String& category, const String& outputPath);
void capturePrintStats(const String& category);

#endif
