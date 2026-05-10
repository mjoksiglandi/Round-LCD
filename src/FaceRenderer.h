#ifndef FACE_RENDERER_H
#define FACE_RENDERER_H

#include <Arduino.h>

#include "AppState.h"
#include "DisplayManager.h"

class FaceRenderer
{
public:
  explicit FaceRenderer(DisplayManager &display);

  void draw(FaceExpression expression, uint32_t nowMs);

private:
  void drawEye(int16_t x, int16_t y, int16_t rx, int16_t ry, int8_t pupilOffset, bool closed);
  void drawMouth(FaceExpression expression);
  uint16_t moodColor(FaceExpression expression) const;

  DisplayManager &display;
};

#endif
