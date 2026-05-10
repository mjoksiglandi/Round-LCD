#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Arduino.h>
#include <FS.h>
#include <SD.h>

class StorageManager
{
public:
  bool begin();
  bool isReady() const;
  File openRead(const char *path);

private:
  bool ready = false;
};

#endif
