
#define GIF_FILENAME "/ezgif.com-gif-maker.gif"

#include <Arduino_GFX_Library.h>

#define GFX_BL 22 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

Arduino_DataBus *bus = new Arduino_ESP32SPI(27 /* DC */, 2 /* CS */, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 33 /* RST */, 0 /* rotation */, true /* IPS */);

#include <SD.h>


#include "GifClass.h"
static GifClass gifClass;

void setup(){
  gfx->begin();
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
#endif

 if (!SD.begin(SS)){
    exit(0);
  }
}

void loop() {
   File gifFile = SD.open(GIF_FILENAME, "r");
  if (!gifFile || gifFile.isDirectory()){
    gfx->println(F("ERROR: open gifFile Failed!"));
  }
  else {
    // read GIF file header
    gd_GIF *gif = gifClass.gd_open_gif(&gifFile);
    if (!gif){
    }
    else {
      uint8_t *buf = (uint8_t *)malloc(gif->width * gif->height);
      if (!buf) {
      }
      else{
        int16_t x = (gfx->width() - gif->width) / 2;
        int16_t y = (gfx->height() - gif->height) / 2;
        uint32_t t_fstart, t_delay = 0, t_real_delay, delay_until;
        int32_t res = 1;
        uint32_t duration = 0, remain = 0;
        while (res > 0){
          t_fstart = millis();
          t_delay = gif->gce.delay * 5;
          res = gifClass.gd_get_frame(gif, buf);
          if (res < 0){
            break;
          }
          else if (res > 0){
            gfx->drawIndexedBitmap(x, y, buf, gif->palette->colors, gif->width, gif->height);
            t_real_delay = t_delay - (millis() - t_fstart);
            duration += t_delay;
            remain += t_real_delay;
            delay_until = millis() + t_real_delay;
            do{
              delay(1);
            } while (millis() < delay_until);
          }
        }

        gifClass.gd_close_gif(gif);
        free(buf);
      }
    }
  }
}
