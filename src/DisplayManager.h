#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Arduino_GFX_Library.h>

class DisplayManager
{
public:
  bool begin();
  void clear(uint16_t color = BLACK);
  void setBacklight(bool enabled);
  void showError(const __FlashStringHelper *message);

  Arduino_GFX &gfx();
  int16_t width() const;
  int16_t height() const;

private:
  bool ready = false;
};

#endif
