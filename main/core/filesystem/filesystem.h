#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>

bool fileSystemInit();

bool fileSystemAvailable();

bool fileSystemCreateDirectory(const String& path);

bool fileSystemAppend(
    const String& file,
    const String& content
);

bool fileSystemWrite(
    const String& file,
    const String& content
);

String fileSystemRead(
    const String& file
);

bool fileSystemExists(const String& path);
String fileSystemListDirectory(const String& path);

#endif