#include <U8g2lib.h>
#include <Arduino_GFX_Library.h>

#define GFX_BL 22 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else
Arduino_DataBus *bus = new Arduino_ESP32SPI(27 /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 33 /* RST */, 0 /* rotation */, true /* IPS */);
#endif
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* more fonts at: https://github.com/moononournation/ArduinoFreeFontFile.git */

void setup(void)
{
    gfx->begin();
    gfx->fillScreen(BLACK);

#ifdef GFX_BL
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
#endif

    gfx->setCursor(30, 100);
    /* U8g2 font list: https://github.com/olikraus/u8g2/wiki/fntlistall */
    gfx->setFont(u8g2_font_ncenB24_te);
    gfx->setTextColor(PURPLE);
    gfx->println("Wena Wena");

    delay(5000); // 5 seconds
}

void loop()
{
    // gfx->setCursor(random(gfx->width()), random(gfx->height()));
    // gfx->setTextColor(random(0xffff));
    //
    // gfx->println("Hello World!");
    //
    // delay(1000); // 1 second
}
