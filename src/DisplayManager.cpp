#include "DisplayManager.h"

#include "AppConfig.h"

namespace
{
constexpr uint8_t MadctlCommand = 0x36;
constexpr uint8_t MadctlBgrBit = 0x08;

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    AppConfig::LcdDcPin,
    AppConfig::LcdCsPin,
    AppConfig::LcdSckPin,
    AppConfig::LcdMosiPin,
    AppConfig::LcdMisoPin,
    VSPI);

Arduino_GFX *display = new Arduino_GC9A01(
    bus,
    AppConfig::LcdRstPin,
    AppConfig::DisplayRotation,
    AppConfig::DisplayIpsMode,
    AppConfig::DisplayWidth,
    AppConfig::DisplayHeight);

uint8_t madctlForRotation(uint8_t rotation)
{
  uint8_t madctl = 0;

  switch (rotation & 0x03)
  {
  case 3:
    madctl = 0xA0;
    break;
  case 2:
    madctl = 0xC0;
    break;
  case 1:
    madctl = 0x60;
    break;
  default:
    madctl = 0x00;
    break;
  }

  if (AppConfig::DisplayBgrColorOrder)
  {
    madctl |= MadctlBgrBit;
  }

  return madctl;
}

void applyPanelColorOrder()
{
  bus->beginWrite();
  bus->writeCommand(MadctlCommand);
  bus->write(madctlForRotation(AppConfig::DisplayRotation));
  bus->endWrite();
}

void drawColorCalibration()
{
  if (!AppConfig::DisplayColorCalibration)
  {
    return;
  }

  display->fillScreen(BLACK);
  display->setTextSize(1);
  display->setTextColor(WHITE, BLACK);

  struct Swatch
  {
    const char *label;
    uint16_t color;
    int16_t x;
    int16_t y;
  };

  const Swatch swatches[] = {
      {"R", 0xF800, 46, 38},
      {"G", 0x07E0, 102, 38},
      {"B", 0x001F, 158, 38},
      {"C", 0x07FF, 46, 104},
      {"M", 0xF81F, 102, 104},
      {"Y", 0xFFE0, 158, 104},
      {"W", 0xFFFF, 102, 170},
  };

  for (const Swatch &swatch : swatches)
  {
    display->fillCircle(swatch.x, swatch.y, 22, swatch.color);
    display->setCursor(swatch.x - 3, swatch.y + 30);
    display->print(swatch.label);
  }

  display->setCursor(54, 215);
  display->print("RGB565 CAL");
  delay(AppConfig::DisplayColorCalibrationMs);
}
}

bool DisplayManager::begin()
{
  display->begin(AppConfig::DisplaySpiSpeed);
  applyPanelColorOrder();
  display->invertDisplay(AppConfig::DisplayInverted);
  ready = true;
  drawColorCalibration();
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
  display->setTextColor(0xFFE0);
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
