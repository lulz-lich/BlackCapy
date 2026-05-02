#ifndef CAPTURE_WRITER_H
#define CAPTURE_WRITER_H

#include <Arduino.h>

void captureWriteLine(const String& category, const String& line);
String capturePathFor(const String& category);

#endif