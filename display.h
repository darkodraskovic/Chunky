#ifndef DISPLAY_H
#define DISPLAY_H

#include "raylib.h"

typedef struct {
  int scale;
  int width;
  int height;
  char* windowTitle;
} DisplayConfig;

typedef struct {
  unsigned int* drawBuffer;
  unsigned int* clearBuffer;
  Texture2D bufferTexture;
  int bufferSize;
  RenderTexture2D renderTexture;
} Framebuffer;

typedef struct {
  int width;
  int height;
  int scale;
} Screen;

int Run();

extern Color ColorScale(Color color, float s);
extern Color ColorMix(Color color1, Color color2, float ratio);
extern unsigned int ColorToUnsignedInt(Color color);

void SetPixel(unsigned int x, unsigned int y, unsigned int color);

#endif /* DISPLAY_H */
