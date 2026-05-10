#include <Arduino.h>

#include "AppConfig.h"
#include "DisplayManager.h"
#include "FaceController.h"
#include "FaceRenderer.h"
#include "StorageManager.h"

namespace
{
DisplayManager display;
StorageManager storage;
FaceController faceController;
FaceRenderer faceRenderer(display);

void createTask(TaskFunction_t task, const char *name, uint32_t stackSize, UBaseType_t priority, BaseType_t core)
{
  BaseType_t result = xTaskCreatePinnedToCore(
      task,
      name,
      stackSize,
      nullptr,
      priority,
      nullptr,
      core);

  if (result != pdPASS)
  {
    Serial.printf("Task creation failed: %s\n", name);
    faceController.setExpression(FaceExpression::Error);
  }
}

void uiTask(void *parameter)
{
  (void)parameter;

  TickType_t lastWake = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(AppConfig::UiTaskPeriodMs);

  for (;;)
  {
    uint32_t now = millis();
    FaceExpression expression = faceController.tick(now);
    faceRenderer.draw(expression, now);
    vTaskDelayUntil(&lastWake, period);
  }
}

void systemTask(void *parameter)
{
  (void)parameter;

  TickType_t lastWake = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(AppConfig::SystemTaskPeriodMs);

  for (;;)
  {
    Serial.printf("uptime=%lu heap=%u sd=%s\n",
                  millis(),
                  ESP.getFreeHeap(),
                  storage.isReady() ? "ready" : "offline");
    vTaskDelayUntil(&lastWake, period);
  }
}
}

void setup()
{
  Serial.begin(AppConfig::SerialBaud);
  delay(100);
  Serial.println(F("Round LCD desktop assistant"));

  if (!display.begin())
  {
    Serial.println(F("Display begin failed"));
  }

  if (!storage.begin())
  {
    faceController.setExpression(FaceExpression::Thinking);
  }

  createTask(
      uiTask,
      "ui",
      AppConfig::UiTaskStackSize,
      AppConfig::UiTaskPriority,
      APP_CPU_NUM);

  createTask(
      systemTask,
      "system",
      AppConfig::SystemTaskStackSize,
      AppConfig::SystemTaskPriority,
      PRO_CPU_NUM);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
