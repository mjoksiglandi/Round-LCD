#include "FaceRenderer.h"

#include "AppConfig.h"

namespace
{
constexpr uint16_t BackgroundColor = 0x0000;
constexpr uint16_t WhiteColor = 0xFFFF;
constexpr uint16_t HighlightColor = 0xFFFF;
constexpr uint16_t NeonColor = 0x07FF;
constexpr uint16_t GlowColor = 0x0258;
constexpr uint16_t BlushColor = 0xF9AE;
constexpr uint16_t BlushGlowColor = 0x7804;
constexpr uint16_t HeartColor = 0xFB0F;
constexpr uint16_t CalmColor = NeonColor;
constexpr uint16_t HappyColor = NeonColor;
constexpr uint16_t SleepyColor = NeonColor;
constexpr uint16_t ThinkingColor = NeonColor;
constexpr uint16_t SurprisedColor = NeonColor;
constexpr uint16_t ErrorColor = NeonColor;

uint16_t panelColor(uint16_t rgb565)
{
  if (AppConfig::DisplayInvertColorsInSoftware)
  {
    rgb565 = ~rgb565;
  }

  if (AppConfig::DisplaySwapRedBlueInSoftware)
  {
    const uint16_t red = (rgb565 & 0xF800) >> 11;
    const uint16_t green = rgb565 & 0x07E0;
    const uint16_t blue = rgb565 & 0x001F;
    rgb565 = (blue << 11) | green | red;
  }

  if (!AppConfig::DisplaySwapColorBytes)
  {
    return rgb565;
  }

  return (rgb565 >> 8) | (rgb565 << 8);
}
}

FaceRenderer::FaceRenderer(DisplayManager &display) : display(display)
{
}

void FaceRenderer::draw(FaceExpression expression, uint32_t nowMs)
{
  Arduino_GFX &gfx = display.gfx();
  (void)nowMs;

  const bool closed = expression == FaceExpression::Blink || expression == FaceExpression::Sleepy;
  const uint16_t color = panelColor(expressionColor(expression));

  if (!needsFullRedraw && expression == lastExpression)
  {
    return;
  }

  gfx.fillScreen(panelColor(BackgroundColor));

  switch (expression)
  {
  case FaceExpression::Happy:
    drawBrow(54, 72, 72, 68, color);
    drawClosedEye(76, 112, 30, 1, color);
    drawClosedEye(164, 112, 30, 1, color);
    drawHeart(184, 62);
    break;
  case FaceExpression::Surprised:
    drawBrow(54, 66, 74, 58, color);
    drawBrow(146, 58, 166, 66, color);
    drawArcEye(78, 103, 32, 200.0F, 520.0F, color);
    drawArcEye(162, 103, 32, 200.0F, 520.0F, color);
    break;
  case FaceExpression::Thinking:
    drawBrow(56, 70, 94, 78, color);
    drawBrow(146, 66, 166, 62, color);
    drawOpenEye(78, 104, 27, 31, 5, -4, color);
    drawOpenEye(162, 104, 27, 31, 4, -4, color);
    break;
  case FaceExpression::Error:
    drawBrow(52, 70, 94, 78, color);
    drawBrow(146, 78, 188, 70, color);
    drawOpenEye(78, 104, 25, 30, 4, -5, color);
    drawOpenEye(162, 104, 25, 30, 2, -5, color);
    break;
  default:
    if (closed)
    {
      drawBrow(54, 70, 72, 67, color);
      drawBrow(146, 67, 166, 70, color);
      drawClosedEye(76, 106, 31, 1, color);
      drawClosedEye(164, 106, 31, 1, color);
      if (expression == FaceExpression::Sleepy)
      {
        drawSleepMarks();
      }
    }
    else
    {
      drawBrow(54, 70, 72, 67, color);
      drawBrow(146, 67, 166, 70, color);
      drawOpenEye(78, 104, 27, 31, 5, -4, color);
      drawOpenEye(162, 104, 27, 31, 5, -4, color);
    }
    break;
  }

  drawBlush(50, 136);
  drawBlush(190, 136);
  drawMouth(expression);
  lastExpression = expression;
  needsFullRedraw = false;
}

void FaceRenderer::forceRedraw()
{
  needsFullRedraw = true;
}

void FaceRenderer::drawOpenEye(int16_t x, int16_t y, int16_t rx, int16_t ry, int8_t pupilX, int8_t pupilY, uint16_t color)
{
  Arduino_GFX &gfx = display.gfx();

  drawEyeRing(x, y, rx + 3, ry + 3, 5, panelColor(GlowColor));
  drawEyeRing(x, y, rx, ry, 5, color);
  gfx.fillCircle(x + 10 + pupilX, y - 11 + pupilY, 9, panelColor(HighlightColor));
  gfx.fillCircle(x + 4 + pupilX, y - 14 + pupilY, 3, panelColor(WhiteColor));
}

void FaceRenderer::drawArcEye(int16_t x, int16_t y, int16_t radius, float start, float end, uint16_t color)
{
  drawGlowArc(x, y, radius + 7, radius + 1, start, end, panelColor(GlowColor));
  drawGlowArc(x, y, radius, radius - 7, start, end, color);
  display.gfx().fillCircle(x + 14, y - 15, 9, panelColor(HighlightColor));
  display.gfx().fillCircle(x + 8, y - 18, 3, panelColor(WhiteColor));
}

void FaceRenderer::drawClosedEye(int16_t x, int16_t y, int16_t radius, int8_t curve, uint16_t color)
{
  const float start = curve > 0 ? 205.0F : 25.0F;
  const float end = curve > 0 ? 335.0F : 155.0F;

  drawGlowArc(x, y, radius + 5, radius - 1, start, end, panelColor(GlowColor));
  drawGlowArc(x, y, radius, radius - 7, start, end, color);
}

void FaceRenderer::drawEyeRing(int16_t x, int16_t y, int16_t rx, int16_t ry, uint8_t thickness, uint16_t color)
{
  Arduino_GFX &gfx = display.gfx();

  for (uint8_t offset = 0; offset < thickness; offset++)
  {
    gfx.drawEllipse(x, y, rx - offset, ry - offset, color);
  }
}

void FaceRenderer::drawMouth(FaceExpression expression)
{
  Arduino_GFX &gfx = display.gfx();
  const uint16_t color = panelColor(expressionColor(expression));

  switch (expression)
  {
  case FaceExpression::Happy:
    drawSmile(color);
    break;
  case FaceExpression::Sleepy:
    drawTinyMouth(color);
    break;
  case FaceExpression::Thinking:
    drawConfusedMouth(color);
    break;
  case FaceExpression::Surprised:
    gfx.fillEllipse(120, 156, 13, 18, panelColor(GlowColor));
    gfx.fillEllipse(120, 156, 9, 14, color);
    gfx.fillEllipse(120, 157, 4, 8, panelColor(BackgroundColor));
    break;
  case FaceExpression::Error:
    drawFrown(color);
    break;
  default:
    drawSoftSmile(color);
    break;
  }
}

void FaceRenderer::drawBrow(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  drawThickLine(x1, y1, x2, y2, 8, panelColor(GlowColor));
  drawThickLine(x1, y1, x2, y2, 5, color);
}

void FaceRenderer::drawBlush(int16_t x, int16_t y)
{
  drawThickLine(x - 12, y + 5, x - 5, y - 5, 6, panelColor(BlushGlowColor));
  drawThickLine(x + 1, y + 5, x + 8, y - 5, 6, panelColor(BlushGlowColor));
  drawThickLine(x - 12, y + 5, x - 5, y - 5, 3, panelColor(BlushColor));
  drawThickLine(x + 1, y + 5, x + 8, y - 5, 3, panelColor(BlushColor));
}

void FaceRenderer::drawHeart(int16_t x, int16_t y)
{
  Arduino_GFX &gfx = display.gfx();

  gfx.fillCircle(x - 8, y - 5, 10, panelColor(BlushGlowColor));
  gfx.fillCircle(x + 8, y - 5, 10, panelColor(BlushGlowColor));
  gfx.fillTriangle(x - 22, y - 2, x + 22, y - 2, x, y + 24, panelColor(BlushGlowColor));
  gfx.fillCircle(x - 7, y - 6, 8, panelColor(HeartColor));
  gfx.fillCircle(x + 7, y - 6, 8, panelColor(HeartColor));
  gfx.fillTriangle(x - 18, y - 3, x + 18, y - 3, x, y + 20, panelColor(HeartColor));
}

void FaceRenderer::drawSleepMarks()
{
  Arduino_GFX &gfx = display.gfx();

  gfx.setTextColor(panelColor(GlowColor));
  gfx.setTextSize(3);
  gfx.setCursor(170, 42);
  gfx.print('z');
  gfx.setCursor(160, 70);
  gfx.print('z');
  gfx.setTextColor(panelColor(NeonColor));
  gfx.setCursor(172, 40);
  gfx.print('z');
  gfx.setCursor(162, 68);
  gfx.print('z');
}

void FaceRenderer::drawThickLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t thickness, uint16_t color)
{
  Arduino_GFX &gfx = display.gfx();
  const int8_t radius = thickness / 2;

  for (int8_t offset = -radius; offset <= radius; offset++)
  {
    gfx.drawLine(x1, y1 + offset, x2, y2 + offset, color);
    gfx.drawLine(x1 + offset, y1, x2 + offset, y2, color);
  }
  gfx.fillCircle(x1, y1, radius, color);
  gfx.fillCircle(x2, y2, radius, color);
}

void FaceRenderer::drawGlowArc(int16_t x, int16_t y, int16_t outerRadius, int16_t innerRadius, float start, float end, uint16_t color)
{
  Arduino_GFX &gfx = display.gfx();

  for (int16_t radius = innerRadius; radius <= outerRadius; radius++)
  {
    gfx.drawArc(x, y, radius, radius, start, end, color);
  }
}

void FaceRenderer::drawSmile(uint16_t color)
{
  Arduino_GFX &gfx = display.gfx();
  constexpr int16_t cx = 120;
  constexpr int16_t cy = 142;

  (void)gfx;
  drawGlowArc(cx, cy, 47, 36, 25.0F, 155.0F, panelColor(GlowColor));
  drawGlowArc(cx, cy, 42, 35, 25.0F, 155.0F, color);
}

void FaceRenderer::drawFrown(uint16_t color)
{
  drawThickLine(100, 156, 140, 154, 8, panelColor(GlowColor));
  drawThickLine(100, 156, 140, 154, 5, color);
}

void FaceRenderer::drawSoftSmile(uint16_t color)
{
  drawGlowArc(120, 139, 42, 33, 35.0F, 145.0F, panelColor(GlowColor));
  drawGlowArc(120, 139, 38, 32, 35.0F, 145.0F, color);
}

void FaceRenderer::drawTinyMouth(uint16_t color)
{
  Arduino_GFX &gfx = display.gfx();

  gfx.fillEllipse(120, 156, 13, 10, panelColor(GlowColor));
  gfx.fillEllipse(120, 156, 9, 6, color);
  gfx.fillEllipse(120, 156, 4, 2, panelColor(BackgroundColor));
}

void FaceRenderer::drawConfusedMouth(uint16_t color)
{
  drawThickLine(92, 158, 100, 140, 8, panelColor(GlowColor));
  drawThickLine(100, 140, 116, 166, 8, panelColor(GlowColor));
  drawThickLine(116, 166, 154, 142, 8, panelColor(GlowColor));
  drawThickLine(154, 142, 146, 176, 8, panelColor(GlowColor));
  drawThickLine(92, 158, 100, 140, 5, color);
  drawThickLine(100, 140, 116, 166, 5, color);
  drawThickLine(116, 166, 154, 142, 5, color);
  drawThickLine(154, 142, 146, 176, 5, color);
}

uint16_t FaceRenderer::expressionColor(FaceExpression expression) const
{
  switch (expression)
  {
  case FaceExpression::Happy:
    return HappyColor;
  case FaceExpression::Sleepy:
  case FaceExpression::Blink:
    return SleepyColor;
  case FaceExpression::Thinking:
    return ThinkingColor;
  case FaceExpression::Surprised:
    return SurprisedColor;
  case FaceExpression::Error:
    return ErrorColor;
  default:
    return CalmColor;
  }
}
