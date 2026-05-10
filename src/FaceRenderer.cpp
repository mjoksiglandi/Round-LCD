#include "FaceRenderer.h"

namespace
{
constexpr uint16_t BackgroundColor = 0x0000;
constexpr uint16_t FaceColor = 0xFFE0;
constexpr uint16_t EyeColor = 0xFFFF;
constexpr uint16_t PupilColor = 0x001F;
constexpr uint16_t ErrorColor = 0xF800;
constexpr uint16_t ThinkingColor = 0x07FF;
constexpr uint16_t SleepyColor = 0x841F;
}

FaceRenderer::FaceRenderer(DisplayManager &display) : display(display)
{
}

void FaceRenderer::draw(FaceExpression expression, uint32_t nowMs)
{
  Arduino_GFX &gfx = display.gfx();
  const int16_t cx = display.width() / 2;
  const int16_t cy = display.height() / 2;
  const int8_t drift = (int8_t)((nowMs / 550) % 7) - 3;
  const bool closed = expression == FaceExpression::Blink || expression == FaceExpression::Sleepy;

  gfx.fillScreen(BackgroundColor);
  gfx.fillCircle(cx, cy, 108, moodColor(expression));
  gfx.fillCircle(cx - 24, cy - 26, 7, BackgroundColor);
  gfx.fillCircle(cx + 62, cy - 64, 11, BackgroundColor);

  switch (expression)
  {
  case FaceExpression::Happy:
    drawEye(78, 94, 24, 28, drift, false);
    drawEye(162, 94, 24, 28, drift, false);
    break;
  case FaceExpression::Surprised:
    drawEye(78, 90, 28, 34, 0, false);
    drawEye(162, 90, 28, 34, 0, false);
    break;
  case FaceExpression::Thinking:
    drawEye(78, 96, 23, 27, -5, false);
    drawEye(162, 92, 23, 27, -2, false);
    gfx.fillCircle(188, 54, 6, EyeColor);
    gfx.fillCircle(205, 43, 4, EyeColor);
    break;
  case FaceExpression::Error:
    gfx.drawLine(57, 76, 99, 116, ErrorColor);
    gfx.drawLine(99, 76, 57, 116, ErrorColor);
    gfx.drawLine(141, 76, 183, 116, ErrorColor);
    gfx.drawLine(183, 76, 141, 116, ErrorColor);
    break;
  default:
    drawEye(78, 94, 24, closed ? 3 : 28, drift, closed);
    drawEye(162, 94, 24, closed ? 3 : 28, drift, closed);
    break;
  }

  drawMouth(expression);
}

void FaceRenderer::drawEye(int16_t x, int16_t y, int16_t rx, int16_t ry, int8_t pupilOffset, bool closed)
{
  Arduino_GFX &gfx = display.gfx();
  if (closed)
  {
    gfx.fillRoundRect(x - rx, y - 2, rx * 2, 5, 2, EyeColor);
    return;
  }

  gfx.fillEllipse(x, y, rx, ry, EyeColor);
  gfx.fillEllipse(x + pupilOffset, y + 4, rx / 3, ry / 2, PupilColor);
}

void FaceRenderer::drawMouth(FaceExpression expression)
{
  Arduino_GFX &gfx = display.gfx();
  switch (expression)
  {
  case FaceExpression::Happy:
    gfx.fillArc(120, 130, 46, 35, 20, 160, EyeColor);
    gfx.fillArc(120, 126, 43, 28, 20, 160, moodColor(expression));
    break;
  case FaceExpression::Sleepy:
    gfx.fillRoundRect(94, 150, 52, 5, 2, EyeColor);
    break;
  case FaceExpression::Thinking:
    gfx.drawLine(92, 154, 142, 144, EyeColor);
    gfx.drawLine(142, 144, 154, 150, EyeColor);
    break;
  case FaceExpression::Surprised:
    gfx.fillEllipse(120, 150, 18, 24, EyeColor);
    gfx.fillEllipse(120, 150, 10, 15, BackgroundColor);
    break;
  case FaceExpression::Error:
    gfx.drawLine(94, 158, 146, 146, ErrorColor);
    break;
  default:
    gfx.fillArc(120, 142, 38, 33, 35, 145, EyeColor);
    gfx.fillArc(120, 139, 36, 28, 35, 145, moodColor(expression));
    break;
  }
}

uint16_t FaceRenderer::moodColor(FaceExpression expression) const
{
  switch (expression)
  {
  case FaceExpression::Thinking:
    return ThinkingColor;
  case FaceExpression::Sleepy:
    return SleepyColor;
  case FaceExpression::Error:
    return 0x3800;
  default:
    return FaceColor;
  }
}
