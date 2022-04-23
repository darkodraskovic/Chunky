#include "Display.h"

#include <stdlib.h>
#include <string.h>

#include "Hooks.h"
#include "raylib.h"

/* Image image; */
unsigned int* drawBuffer;
unsigned int* clearBuffer;
Texture2D texture;
int width;
int scale;
int bufferSize;

void InitRenderer(DisplayConfig displayConfig) {
  InitWindow(displayConfig.width * displayConfig.scale,
             displayConfig.height * displayConfig.scale,
             displayConfig.windowTitle);

  SetTargetFPS(60);

  Image image = GenImageColor(displayConfig.width, displayConfig.height, BLACK);
  drawBuffer = image.data;
  image = GenImageColor(displayConfig.width, displayConfig.height, BLACK);
  clearBuffer = image.data;
  texture = LoadTextureFromImage(image);

  width = displayConfig.width;
  scale = displayConfig.scale;
  bufferSize = width * displayConfig.height * sizeof(unsigned int);
}

// TODO: handle alpha
inline Color ColorScale(Color color, float s) {
  if (s > 1) s = 1;
  color.r *= s;
  color.b *= s;
  color.g *= s;
  return color;
}

// TODO: handle alpha
inline Color ColorMix(Color color1, Color color2, float ratio) {
  color1 = ColorScale(color1, 1 - ratio);
  color2 = ColorScale(color2, ratio);
  return (Color){color1.r + color2.r, color1.g + color2.g, color1.b + color2.b,
                 255};
}

inline unsigned int ColorToUnsignedInt(Color color) {
  return (((unsigned int)color.a << 24) | ((unsigned int)color.b << 16) |
          ((unsigned int)color.g << 8) | (unsigned int)color.r);
}

void SetPixel(unsigned int x, unsigned int y, unsigned int color) {
  *(drawBuffer + y * width + x) = color;
}

static inline void clearBackground() {
  memcpy(drawBuffer, clearBuffer, bufferSize);
}

int Run(DisplayConfig displayConfig) {
  InitRenderer(displayConfig);
  Init();

  while (!WindowShouldClose()) {
    Update();      // hook
    PostUpdate();  // hook

    BeginDrawing();

    clearBackground();
    // set buffer values
    Draw();  // hook

    // draw buffer
    UpdateTexture(texture, drawBuffer);
    DrawTextureEx(texture, (Vector2){0, 0}, 0, scale, WHITE);

    PostDraw();  // hook

    EndDrawing();
  }

  Close();
  CloseWindow();  // Close window and OpenGL context

  return 0;
}
