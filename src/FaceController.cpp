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

  if (transientUntilMs != 0 && nowMs < transientUntilMs)
  {
    return transientExpression;
  }

  if (AppConfig::IdleHappyPulseMs > 0 &&
      nowMs > AppConfig::GreetingMoodMs &&
      nowMs % AppConfig::IdleHappyPulseMs < AppConfig::IdleHappyDurationMs)
  {
    return FaceExpression::Happy;
  }

  expression = homeExpression(nowMs);
  return expression;
}

void FaceController::setExpression(FaceExpression nextExpression)
{
  expression = nextExpression;
  transientExpression = nextExpression;
  transientUntilMs = 0;
}

void FaceController::notifyInteraction(FaceExpression nextExpression, uint32_t durationMs)
{
  transientExpression = nextExpression;
  transientUntilMs = millis() + (durationMs == 0 ? AppConfig::InteractionMoodMs : durationMs);
}

void FaceController::notifyStorageMissing()
{
  notifyInteraction(FaceExpression::Thinking, AppConfig::InteractionMoodMs);
}

FaceExpression FaceController::current() const
{
  return expression;
}

FaceExpression FaceController::homeExpression(uint32_t nowMs) const
{
  if (nowMs >= AppConfig::SleepyAfterMs)
  {
    return FaceExpression::Sleepy;
  }

  return FaceExpression::Neutral;
}
