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

constexpr bool DisplayIpsMode = false;
constexpr bool DisplayInverted = false;
constexpr bool DisplayBgrColorOrder = false;
constexpr bool DisplaySwapColorBytes = false;
constexpr bool DisplayInvertColorsInSoftware = true;
constexpr bool DisplaySwapRedBlueInSoftware = true;
constexpr bool DisplayColorCalibration = false;
constexpr uint32_t DisplayColorCalibrationMs = 6000;
constexpr uint8_t DisplayRotation = 2;
constexpr uint32_t DisplaySpiSpeed = 10000000;

constexpr uint32_t UiTaskPeriodMs = 50;
constexpr uint32_t SystemTaskPeriodMs = 2000;
constexpr uint32_t BlinkIntervalMs = 5200;
constexpr uint32_t BlinkDurationMs = 120;
constexpr uint32_t GreetingMoodMs = 3500;
constexpr uint32_t InteractionMoodMs = 2500;
constexpr uint32_t IdleHappyPulseMs = 60000;
constexpr uint32_t IdleHappyDurationMs = 1800;
constexpr uint32_t SleepyAfterMs = 900000;

constexpr uint32_t UiTaskStackSize = 4096;
constexpr uint32_t SystemTaskStackSize = 3072;
constexpr UBaseType_t UiTaskPriority = 2;
constexpr UBaseType_t SystemTaskPriority = 1;
}

#endif
