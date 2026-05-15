# Round LCD

Firmware PlatformIO para ESP32 DOIT DevKit V1. El proyecto esta evolucionando desde un reproductor de GIF para una pantalla redonda GC9A01 hacia un mini asistente de escritorio con una cara dibujada por codigo.

## Hardware

- Board: ESP32 DOIT DevKit V1
- Display: GC9A01 SPI round LCD
- Storage: SD card

## Pines

| Funcion | Pin ESP32 |
| --- | --- |
| LCD DC | 27 |
| LCD CS | 2 |
| LCD SCK | 18 |
| LCD MOSI | 23 |
| LCD MISO | no conectado |
| LCD RST | 33 |
| LCD BL | 22 |
| SD CS | `SS` del board |

## Arquitectura

- `include/AppConfig.h`: pinout, timings, paths y parametros generales.
- `include/AppState.h`: modos y expresiones disponibles.
- `src/DisplayManager.*`: inicializacion de pantalla, backlight y errores visuales.
- `src/StorageManager.*`: montaje de SD y apertura de archivos.
- `src/FaceRenderer.*`: dibujo de la cara con primitivas de `Arduino_GFX`.
- `src/FaceController.*`: expresion actual, parpadeo y rotacion simple de estados.
- `src/GifPlayer.*`: reproductor GIF encapsulado para rutinas futuras.
- `src/RoundLcd.cpp`: arranque de la aplicacion y tareas FreeRTOS.

## FreeRTOS

- `uiTask`: evalua la cara cada `50 ms`, pero solo redibuja cuando cambia una expresion o un detalle pequeno de ojos.
- `systemTask`: publica estado basico por serial cada `2000 ms`.
- `loop()`: queda libre y solo cede tiempo al scheduler.

## Pantalla y color

El modulo GC9A01 probado requiere compensacion de color en software:
`DisplayInvertColorsInSoftware = true` y `DisplaySwapRedBlueInSoftware = true`.
La pantalla queda a `10 MHz` de SPI para reducir glitches de lineas con el
cableado actual. Si cambias de modulo, activa `DisplayColorCalibration` en
`include/AppConfig.h` para mostrar la grilla `R/G/B/C/M/Y/W` al arranque y
ajustar los flags sin tocar `FaceRenderer`.

## SD

La cara principal no depende de la SD. La tarjeta queda disponible para configuraciones, rutinas o assets especiales.

## GIF heredado

El reproductor GIF sigue disponible como modulo para rutinas futuras. Si se usa, el archivo heredado esperado es:

```text
/ezgif.com-gif-maker.gif
```

Conviene usar GIFs pequenos, idealmente de 240 x 240 px o menos, porque el frame completo se guarda en RAM.

## Build y upload

```bash
pio run
pio run --target upload
pio device monitor
```

La velocidad del monitor serial es `115200`.

## Flujo de firmware

El flujo operativo para cambios de firmware queda documentado en
[`docs/firmware-workflow.md`](docs/firmware-workflow.md). Usalo para definir
alcance, validacion, evidencia y criterios de cierre antes de marcar un cambio
como listo.

## Dependencias

`platformio.ini` fija la plataforma ESP32 y declara las librerias necesarias. `Arduino_GFX` queda fijada en `1.2.8` porque versiones mas nuevas compilan fuentes QSPI/RGBPanel que no calzan con el core Arduino ESP32 estable de PlatformIO para esta placa.

- `moononournation/GFX Library for Arduino`
- `bitbank2/PNGdec`

`PNGdec` solo se usa en `alternativ/pngviwer.cpp`, que queda como ejemplo alternativo fuera del build principal.
