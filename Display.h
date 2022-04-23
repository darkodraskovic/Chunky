#ifndef DISPLAY_H
#define DISPLAY_H

#include "raylib.h"

typedef struct {
  int scale;
  int width;
  int height;
  char* windowTitle;
} DisplayConfig;

int Run();

extern Color ColorScale(Color color, float s);
extern Color ColorMix(Color color1, Color color2, float ratio);
extern unsigned int ColorToUnsignedInt(Color color);

void SetPixel(unsigned int x, unsigned int y, unsigned int color);

#endif /* DISPLAY_H */
