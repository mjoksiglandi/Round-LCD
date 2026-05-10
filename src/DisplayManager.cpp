#include "DisplayManager.h"

#include "AppConfig.h"

namespace
{
Arduino_DataBus *bus = new Arduino_ESP32SPI(
    AppConfig::LcdDcPin,
    AppConfig::LcdCsPin,
    AppConfig::LcdSckPin,
    AppConfig::LcdMosiPin,
    AppConfig::LcdMisoPin,
    VSPI);

Arduino_GFX *display = new Arduino_GC9A01(bus, AppConfig::LcdRstPin, 0, true);
}

bool DisplayManager::begin()
{
  display->begin();
  ready = true;
  display->fillScreen(BLACK);
  setBacklight(true);
  return ready;
}

void DisplayManager::clear(uint16_t color)
{
  display->fillScreen(color);
}

void DisplayManager::setBacklight(bool enabled)
{
  pinMode(AppConfig::LcdBacklightPin, OUTPUT);
  digitalWrite(AppConfig::LcdBacklightPin, enabled ? HIGH : LOW);
}

void DisplayManager::showError(const __FlashStringHelper *message)
{
  Serial.println(message);
  display->fillScreen(BLACK);
  display->setTextColor(RED);
  display->setCursor(18, 108);
  display->println(message);
}

Arduino_GFX &DisplayManager::gfx()
{
  return *display;
}

int16_t DisplayManager::width() const
{
  return display->width();
}

int16_t DisplayManager::height() const
{
  return display->height();
}
