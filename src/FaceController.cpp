#include "FaceController.h"

#include "AppConfig.h"

FaceExpression FaceController::tick(uint32_t nowMs)
{
  if (nowMs - lastBlinkMs >= AppConfig::BlinkIntervalMs)
  {
    lastBlinkMs = nowMs;
  }

  if (nowMs - lastBlinkMs < AppConfig::BlinkDurationMs)
  {
    return FaceExpression::Blink;
  }

  if (nowMs - expressionStartedMs >= AppConfig::ExpressionPeriodMs)
  {
    expressionStartedMs = nowMs;
    switch (expression)
    {
    case FaceExpression::Neutral:
      expression = FaceExpression::Happy;
      break;
    case FaceExpression::Happy:
      expression = FaceExpression::Thinking;
      break;
    case FaceExpression::Thinking:
      expression = FaceExpression::Sleepy;
      break;
    case FaceExpression::Sleepy:
      expression = FaceExpression::Surprised;
      break;
    default:
      expression = FaceExpression::Neutral;
      break;
    }
  }

  return expression;
}

void FaceController::setExpression(FaceExpression nextExpression)
{
  expression = nextExpression;
  expressionStartedMs = millis();
}

FaceExpression FaceController::current() const
{
  return expression;
}
