#ifndef FACE_CONTROLLER_H
#define FACE_CONTROLLER_H

#include <Arduino.h>

#include "AppState.h"

class FaceController
{
public:
  FaceExpression tick(uint32_t nowMs);
  void setExpression(FaceExpression expression);
  FaceExpression current() const;

private:
  FaceExpression expression = FaceExpression::Neutral;
  uint32_t lastBlinkMs = 0;
  uint32_t expressionStartedMs = 0;
};

#endif
