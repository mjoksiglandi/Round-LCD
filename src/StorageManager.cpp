#include "StorageManager.h"

#include "AppConfig.h"

bool StorageManager::begin()
{
  ready = SD.begin(AppConfig::SdCsPin);
  Serial.println(ready ? F("SD mounted") : F("SD mount failed"));
  return ready;
}

bool StorageManager::isReady() const
{
  return ready;
}

File StorageManager::openRead(const char *path)
{
  if (!ready)
  {
    return File();
  }

  return SD.open(path, "r");
}
