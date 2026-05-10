#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <Arduino.h>

namespace AppConfig
{
constexpr uint32_t SerialBaud = 115200;

constexpr int16_t DisplayWidth = 240;
constexpr int16_t DisplayHeight = 240;

constexpr int8_t LcdDcPin = 27;
constexpr int8_t LcdCsPin = 2;
constexpr int8_t LcdSckPin = 18;
constexpr int8_t LcdMosiPin = 23;
constexpr int8_t LcdMisoPin = -1;
constexpr int8_t LcdRstPin = 33;
constexpr int8_t LcdBacklightPin = 22;

constexpr int8_t SdCsPin = SS;
constexpr const char *DefaultGifPath = "/ezgif.com-gif-maker.gif";

constexpr uint32_t UiTaskPeriodMs = 50;
constexpr uint32_t SystemTaskPeriodMs = 2000;
constexpr uint32_t BlinkIntervalMs = 4200;
constexpr uint32_t BlinkDurationMs = 130;
constexpr uint32_t ExpressionPeriodMs = 7000;

constexpr uint32_t UiTaskStackSize = 4096;
constexpr uint32_t SystemTaskStackSize = 3072;
constexpr UBaseType_t UiTaskPriority = 2;
constexpr UBaseType_t SystemTaskPriority = 1;
}

#endif
