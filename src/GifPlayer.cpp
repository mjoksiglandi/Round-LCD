#include "GifPlayer.h"

bool GifPlayer::playOnce(File &gifFile, Arduino_GFX &gfx)
{
  if (!gifFile || gifFile.isDirectory())
  {
    return false;
  }

  gd_GIF *gif = decoder.gd_open_gif(&gifFile);
  if (!gif)
  {
    gifFile.close();
    return false;
  }

  size_t frameSize = (size_t)gif->width * gif->height;
  uint8_t *buffer = (uint8_t *)malloc(frameSize);
  if (!buffer)
  {
    decoder.gd_close_gif(gif);
    return false;
  }

  memset(buffer, gif->bgindex, frameSize);
  int16_t x = (gfx.width() - gif->width) / 2;
  int16_t y = (gfx.height() - gif->height) / 2;
  int32_t result = 1;

  while (result > 0)
  {
    uint32_t frameStart = millis();
    result = decoder.gd_get_frame(gif, buffer);
    if (result < 0)
    {
      break;
    }

    if (result > 0)
    {
      gfx.drawIndexedBitmap(x, y, buffer, gif->palette->colors, gif->width, gif->height);
      uint32_t frameDelay = gif->gce.delay * 10UL;
      uint32_t elapsed = millis() - frameStart;
      if (frameDelay > elapsed)
      {
        delay(frameDelay - elapsed);
      }
    }
  }

  decoder.gd_close_gif(gif);
  free(buffer);
  return result >= 0;
}
