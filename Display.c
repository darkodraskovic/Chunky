#include "Display.h"

#include <stdlib.h>
#include <string.h>

#include "Hooks.h"
#include "raylib.h"

// framebuffer
unsigned int* drawBuffer;
unsigned int* clearBuffer;
Texture2D bufferTexture;
int bufferSize;

// render texture
RenderTexture2D renderTexture;

int width;
int height;
int scale;

void InitRenderer(DisplayConfig displayConfig) {
  InitWindow(displayConfig.width * displayConfig.scale, displayConfig.height * displayConfig.scale,
             displayConfig.windowTitle);

  SetTargetFPS(60);

  renderTexture = LoadRenderTexture(displayConfig.width, displayConfig.height);

  Image image = GenImageColor(displayConfig.width, displayConfig.height, BLACK);
  drawBuffer = image.data;
  // (Color){0} is transparent: draw render texture over buffer texture
  image = GenImageColor(displayConfig.width, displayConfig.height, (Color){0});
  clearBuffer = image.data;
  bufferTexture = LoadTextureFromImage(image);
  bufferSize = displayConfig.width * displayConfig.height * sizeof(unsigned int);

  width = displayConfig.width;
  height = displayConfig.height;
  scale = displayConfig.scale;
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
  return (Color){color1.r + color2.r, color1.g + color2.g, color1.b + color2.b, 255};
}

inline unsigned int ColorToUnsignedInt(Color color) {
  return (((unsigned int)color.a << 24) | ((unsigned int)color.b << 16) | ((unsigned int)color.g << 8) |
          (unsigned int)color.r);
}

void SetPixel(unsigned int x, unsigned int y, unsigned int color) { *(drawBuffer + y * width + x) = color; }

static inline void ClearBuffer() { memcpy(drawBuffer, clearBuffer, bufferSize); }

int Run(DisplayConfig displayConfig) {
  InitRenderer(displayConfig);
  Init();

  while (!WindowShouldClose()) {
    Update();      // hook
    PostUpdate();  // hook

    BeginTextureMode(renderTexture);

    ClearBackground(BLACK);
    ClearBuffer();

    Draw();  // hook

    EndTextureMode();

    BeginDrawing();

    DrawTexturePro(renderTexture.texture, (Rectangle){0, 0, width, -height},
                   (Rectangle){0, 0, width * scale, height * scale}, (Vector2){0, 0}, 0, WHITE);

    UpdateTexture(bufferTexture, drawBuffer);
    DrawTextureEx(bufferTexture, (Vector2){0, 0}, 0, scale, WHITE);

    PostDraw();  // hook

    EndDrawing();
  }

  Close();  // hook

  UnloadRenderTexture(renderTexture);
  CloseWindow();  // Close window and OpenGL context

  return 0;
}
