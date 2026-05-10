#ifndef GIF_PLAYER_H
#define GIF_PLAYER_H

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <FS.h>

#include "GifClass.h"

class GifPlayer
{
public:
  bool playOnce(File &gifFile, Arduino_GFX &gfx);

private:
  GifClass decoder;
};

#endif
