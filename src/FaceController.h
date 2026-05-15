#ifndef FACE_CONTROLLER_H
#define FACE_CONTROLLER_H

#include <Arduino.h>

#include "AppConfig.h"
#include "AppState.h"

class FaceController
{
public:
  FaceExpression tick(uint32_t nowMs);
  void setExpression(FaceExpression expression);
  void notifyInteraction(FaceExpression expression = FaceExpression::Happy, uint32_t durationMs = 0);
  void notifyStorageMissing();
  FaceExpression current() const;

private:
  FaceExpression homeExpression(uint32_t nowMs) const;

  FaceExpression expression = FaceExpression::Happy;
  FaceExpression transientExpression = FaceExpression::Happy;
  uint32_t lastBlinkMs = 0;
  uint32_t transientUntilMs = AppConfig::GreetingMoodMs;
};

#endif
