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
  void forceRedraw();

private:
  void drawOpenEye(int16_t x, int16_t y, int16_t rx, int16_t ry, int8_t pupilX, int8_t pupilY, uint16_t color);
  void drawArcEye(int16_t x, int16_t y, int16_t radius, float start, float end, uint16_t color);
  void drawClosedEye(int16_t x, int16_t y, int16_t radius, int8_t curve, uint16_t color);
  void drawEyeRing(int16_t x, int16_t y, int16_t rx, int16_t ry, uint8_t thickness, uint16_t color);
  void drawMouth(FaceExpression expression);
  void drawBrow(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void drawBlush(int16_t x, int16_t y);
  void drawHeart(int16_t x, int16_t y);
  void drawSleepMarks();
  void drawThickLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t thickness, uint16_t color);
  void drawGlowArc(int16_t x, int16_t y, int16_t outerRadius, int16_t innerRadius, float start, float end, uint16_t color);
  void drawSmile(uint16_t color);
  void drawFrown(uint16_t color);
  void drawSoftSmile(uint16_t color);
  void drawTinyMouth(uint16_t color);
  void drawConfusedMouth(uint16_t color);
  uint16_t expressionColor(FaceExpression expression) const;

  DisplayManager &display;
  FaceExpression lastExpression = FaceExpression::Error;
  bool needsFullRedraw = true;
};

#endif
