
#include <Arduino_GFX_Library.h>

#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else
Arduino_DataBus *bus = new Arduino_ESP32SPI(27 /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 33 /* RST */, 0 /* rotation */, true /* IPS */);
#endif

void setup(void)
{
    gfx->begin();
    gfx->fillScreen(BLACK);

#ifdef DF_GFX_BL
    pinMode(DF_GFX_BL, OUTPUT);
    digitalWrite(DF_GFX_BL, HIGH);
#endif

    gfx->setCursor(20, 100);
    gfx->setTextSize(4,4,2);
    gfx->setTextColor(RED);
    gfx->println("Wena Wena");

    delay(5000); // 5 seconds
}

void loop()
{
    // gfx->setCursor(random(gfx->width()), random(gfx->height()));
    // gfx->setTextColor(random(0xffff), random(0xffff));
    // gfx->setTextSize(random(6) /* x scale */, random(6) /* y scale */, random(2) /* pixel_margin */);
    // gfx->println("Wena Wena");
    //
    // delay(1000); // 1 second
}
