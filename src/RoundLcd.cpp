
/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#define GIF_FILENAME "/ezgif.com-gif-maker.gif"
#elif defined(ARDUINO_RASPBERRY_PI_PICO)
#define GIF_FILENAME "/ezgif.com-gif-maker.gif"
#elif defined(ESP32)
#define GIF_FILENAME "/ezgif.com-gif-maker.gif"
#else
#define GIF_FILENAME "/ezgif.com-gif-maker.gif"
#endif

#include <Arduino_GFX_Library.h>

#define GFX_BL 22 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else
Arduino_DataBus *bus = new Arduino_ESP32SPI(27 /* DC */, 2 /* CS */, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 33 /* RST */, 0 /* rotation */, true /* IPS */);
#endif

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#include <Seeed_FS.h>
#include <SD/Seeed_SD.h>
#elif defined(ARDUINO_RASPBERRY_PI_PICO)
//#include <LittleFS.h>
#include <SD.h>
#elif defined(ESP32)
#include <FFat.h>
//#include <LittleFS.h>
#include <SPIFFS.h>
#include <SD.h>
#elif defined(ESP8266)
//#include <LittleFS.h>
#include <SD.h>
#else
#include <SD.h>
#endif

#include "GifClass.h"
static GifClass gifClass;

void setup()
{
  //Serial.begin(115200);

  // Init Display
  gfx->begin();
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
#endif

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL))
#elif defined(ARDUINO_RASPBERRY_PI_PICO)
  //if (!LittleFS.begin())
   if (!SD.begin(SS))
#elif defined(ESP32)
  // if (!FFat.begin())
  //if (!LittleFS.begin())
  // if (!SPIFFS.begin())
 if (!SD.begin(SS))
#elif defined(ESP8266)
//  if (!LittleFS.begin())
   if (!SD.begin(SS))
#else
  if (!SD.begin())
#endif
  {
    //Serial.println(F("ERROR: File System Mount Failed!"));
    //gfx->println(F("ERROR: File System Mount Failed!"));
    exit(0);
  }
}

void loop()
{
/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
  File gifFile = SD.open(GIF_FILENAME, "r");
#elif defined(ARDUINO_RASPBERRY_PI_PICO)
  //File gifFile = LittleFS.open(GIF_FILENAME, "r");
   File gifFile = SD.open(GIF_FILENAME, "r");
#elif defined(ESP32)
  // File gifFile = FFat.open(GIF_FILENAME, "r");
  //File gifFile = LittleFS.open(GIF_FILENAME, "r");
  // File gifFile = SPIFFS.open(GIF_FILENAME, "r");
   File gifFile = SD.open(GIF_FILENAME, "r");
#elif defined(ESP8266)
//  File gifFile = LittleFS.open(GIF_FILENAME, "r");
   File gifFile = SD.open(GIF_FILENAME, "r");
#else
  File gifFile = SD.open(GIF_FILENAME, FILE_READ);
#endif
  if (!gifFile || gifFile.isDirectory())
  {
  //  Serial.println(F("ERROR: open gifFile Failed!"));
    gfx->println(F("ERROR: open gifFile Failed!"));
  }
  else
  {
    // read GIF file header
    gd_GIF *gif = gifClass.gd_open_gif(&gifFile);
    if (!gif)
    {
      //Serial.println(F("gd_open_gif() failed!"));
    }
    else
    {
      uint8_t *buf = (uint8_t *)malloc(gif->width * gif->height);
      if (!buf)
      {
      //  Serial.println(F("buf malloc failed!"));
      }
      else
      {
        int16_t x = (gfx->width() - gif->width) / 2;
        int16_t y = (gfx->height() - gif->height) / 2;

        //Serial.println(F("GIF video start"));
        uint32_t t_fstart, t_delay = 0, t_real_delay, delay_until;
        int32_t res = 1;
        uint32_t duration = 0, remain = 0;
        while (res > 0)
        {
          t_fstart = millis();
          t_delay = gif->gce.delay * 10;
          res = gifClass.gd_get_frame(gif, buf);
          if (res < 0)
          {
            //Serial.println(F("ERROR: gd_get_frame() failed!"));
            break;
          }
          else if (res > 0)
          {
            gfx->drawIndexedBitmap(x, y, buf, gif->palette->colors, gif->width, gif->height);

            t_real_delay = t_delay - (millis() - t_fstart);
            duration += t_delay;
            remain += t_real_delay;
            delay_until = millis() + t_real_delay;
            do
            {
              delay(1);
            } while (millis() < delay_until);
          }
        }
        // Serial.println(F("GIF video end"));
        // Serial.print(F("duration: "));
        // Serial.print(duration);
        // Serial.print(F(", remain: "));
        // Serial.print(remain);
        // Serial.print(F(" ("));
        // Serial.print(100.0 * remain / duration);
        // Serial.println(F("%)"));

        gifClass.gd_close_gif(gif);
        free(buf);
      }
    }
  }
}
