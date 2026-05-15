# Firmware Workflow

Flujo operativo para cambios de firmware en Round LCD. Este proyecto usa
PlatformIO con Arduino sobre ESP32 DOIT DevKit V1, pantalla GC9A01 por SPI y SD
opcional. La aplicacion actual dibuja una cara por codigo y conserva modulos
para reproducir GIFs.

## Proposito

Mantener cambios pequenos, verificables y faciles de probar en hardware real.
El objetivo no es solo que compile: cada cambio debe indicar que comportamiento
toca, como se valido y que evidencia queda para confiar en el resultado.

## Cuando Usarlo

Usa este flujo para:

- Cambios de pantalla, SPI, backlight, SD o pinout.
- Cambios en tareas FreeRTOS, periodos, prioridades o stack.
- Nuevas expresiones, rutinas, assets o modos del asistente.
- Correcciones de estabilidad, memoria, watchdog, rendimiento o arranque.
- Ajustes de dependencias en `platformio.ini`.

Para cambios puramente documentales, usa solo las secciones de alcance,
revision y entregables.

## Inputs Requeridos

Antes de implementar, deja claro:

- Objetivo: que comportamiento visible o tecnico debe cambiar.
- Hardware probado: placa ESP32, pantalla GC9A01, SD si aplica, cableado y
  alimentacion.
- Superficie afectada: por ejemplo `DisplayManager`, `FaceRenderer`,
  `FaceController`, `StorageManager`, `GifPlayer`, `AppConfig` o tareas en
  `RoundLcd.cpp`.
- Restricciones: RAM/flash, frecuencia SPI, periodo de UI, dependencia de SD,
  formato/tamano de assets y si requiere prueba en hardware.
- Criterio de exito: que se debe ver por pantalla, por monitor serial o por
  medicion externa.

## Proceso Paso A Paso

1. **Orientacion**
   - Lee `README.md`, `platformio.ini`, `include/AppConfig.h` y el modulo que
     vas a tocar.
   - Revisa `git status --short` y no reviertas cambios ajenos.
   - Identifica si el cambio es de configuracion, render, estado, storage,
     reproduccion de assets o scheduler.

2. **Alcance Minimo**
   - Cambia la menor cantidad de archivos que permita cumplir el objetivo.
   - No hagas refactors oportunistas junto con una correccion funcional.
   - Si aparece deuda tecnica no relacionada, anotala como follow-up.

3. **Diseno Firmware**
   - Manten el pinout y timings centralizados en `include/AppConfig.h`.
   - Para tareas FreeRTOS, documenta cambios de periodo, prioridad, core y stack.
   - Evita asignaciones dinamicas recurrentes dentro de loops o tareas.
   - Ninguna tarea debe bloquear indefinidamente sin una razon explicita.
   - Para drivers/perifericos, maneja error paths y deja una senal visible o log.

4. **Implementacion**
   - Cambia primero el modulo propietario del comportamiento.
   - Manten `setup()` como orquestador de arranque, no como deposito de logica.
   - Usa `Serial.printf` para diagnostico liviano y estados observables.
   - Si agregas assets, define tamano/formato esperado y si dependen de SD.

5. **Build Local**
   - Ejecuta:

     ```bash
     pio run
     ```

   - Si falla, corrige antes de pasar a prueba en hardware.
   - No subas firmware si el build local falla.

6. **Prueba En Hardware**
   - Cuando el cambio toca comportamiento runtime, ejecuta:

     ```bash
     pio run --target upload
     pio device monitor
     ```

   - Verifica arranque limpio, logs esperados, pantalla activa y ausencia de
     resets/watchdog.
   - Para cambios visuales, captura foto o video corto de la pantalla.
   - Para SD/GIF/assets, prueba con tarjeta presente y ausente.

7. **Reality Check**
   - No marques como listo un cambio solo porque compilo.
   - Si no hubo hardware disponible, declaralo como riesgo pendiente.
   - Si el monitor serial muestra resets, heap cayendo sostenidamente o SD
     intermitente, el estado es `NEEDS WORK`.

8. **Revision**
   - Revisa correctness, errores de perifericos, memoria, timing, y facilidad de
     revertir.
   - Confirma que dependencias queden fijadas por version en `platformio.ini`.
   - Mira el diff completo y elimina cualquier linea que no responda al objetivo.

## Validacion Y Checks

Checks minimos por tipo de cambio:

| Tipo de cambio | Check requerido |
| --- | --- |
| Docs | Lectura rapida y enlaces validos |
| Configuracion | `pio run` |
| Render/UI | `pio run`, upload, foto/video de pantalla |
| FreeRTOS/timing | `pio run`, monitor serial, observacion de resets/watchdog |
| SD/assets | `pio run`, prueba con SD presente y ausente |
| Dependencias | `pio run`, versiones fijadas, nota de compatibilidad |

Comandos base:

```bash
pio run
pio run --target upload
pio device monitor
```

## Calibracion De Color GC9A01

Algunos modulos GC9A01 no coinciden con la interpretacion esperada de color de
`Arduino_GFX`. Si el negro aparece blanco, el cian aparece amarillo, o las
mejillas aparecen azules, usa las constantes de `include/AppConfig.h` antes de
tocar la logica del renderer:

- `DisplayColorCalibration`: muestra una grilla temporal RGB565 `R/G/B/C/M/Y/W`
  al arrancar para fotografiar el comportamiento real del panel.
- `DisplayInvertColorsInSoftware`: compensa paneles que muestran el complemento
  del color enviado.
- `DisplaySwapRedBlueInSoftware`: compensa paneles con canales rojo y azul
  intercambiados despues de la inversion.
- `DisplayBgrColorOrder`: ajusta el bit BGR/RGB del registro `MADCTL`.
- `DisplaySwapColorBytes`: queda disponible para diagnosticar variantes con
  bytes RGB565 invertidos.

El modulo probado quedo estable visualmente con inversion y swap rojo/azul en
software, `DisplayInverted = false`, `DisplayBgrColorOrder = false`, y SPI a
`10 MHz`. Persisten glitches minimos de lineas ocasionales; si aumentan,
prioriza revisar cableado corto, GND comun firme, alimentacion y margen de SPI
antes de subir la frecuencia.

## Deliverables

Cada cambio debe cerrar con:

- Resumen de que cambio y por que.
- Archivos tocados.
- Checks ejecutados y resultado.
- Evidencia de hardware si aplica: logs, foto, video o descripcion precisa.
- Riesgos pendientes, especialmente si no se probo en hardware real.
- Follow-ups detectados pero no incluidos.

## Done Criteria

Un cambio queda listo cuando:

- `pio run` pasa.
- El comportamiento esperado fue verificado en hardware si toca runtime.
- No hay resets, watchdogs ni degradacion obvia de heap en monitor serial.
- La pantalla muestra el estado esperado en cold boot.
- Los errores esperados tienen fallback visible o log util.
- El diff esta acotado al objetivo.
- README o docs se actualizan si cambian pinout, comandos, arquitectura,
  dependencias o flujo de uso.

Si falta una de estas pruebas por no tener hardware disponible, el cambio puede
quedar integrado como avance, pero el cierre debe decir explicitamente:
`pendiente prueba en hardware`.
