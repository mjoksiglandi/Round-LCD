#ifndef APP_STATE_H
#define APP_STATE_H

enum class FaceExpression
{
  Neutral,
  Happy,
  Blink,
  Sleepy,
  Thinking,
  Surprised,
  Error
};

enum class AssistantMode
{
  Face,
  Routine,
  Error
};

#endif
