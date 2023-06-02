#include "display.h"

#include <stdlib.h>
#include <string.h>

#include "hooks.h"
#include "raylib.h"

Framebuffer framebuffer;
Screen screen;

void InitRenderer(DisplayConfig displayConfig) {
  InitWindow(displayConfig.width * displayConfig.scale, displayConfig.height * displayConfig.scale,
             displayConfig.windowTitle);

  SetTargetFPS(60);

  framebuffer.renderTexture = LoadRenderTexture(displayConfig.width, displayConfig.height);

  Image image = GenImageColor(displayConfig.width, displayConfig.height, BLACK);
  framebuffer.drawBuffer = image.data;
  // (Color){0} is transparent: draw render texture over buffer texture
  image = GenImageColor(displayConfig.width, displayConfig.height, (Color){0});
  framebuffer.clearBuffer = image.data;
  framebuffer.bufferTexture = LoadTextureFromImage(image);
  framebuffer.bufferSize = displayConfig.width * displayConfig.height * sizeof(unsigned int);

  screen.width = displayConfig.width;
  screen.height = displayConfig.height;
  screen.scale = displayConfig.scale;
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

void SetPixel(unsigned int x, unsigned int y, unsigned int color) {
  *(framebuffer.drawBuffer + y * screen.width + x) = color;
}

static inline void ClearBuffer() { memcpy(framebuffer.drawBuffer, framebuffer.clearBuffer, framebuffer.bufferSize); }

int Run(DisplayConfig displayConfig) {
  InitRenderer(displayConfig);
  Init();

  while (!WindowShouldClose()) {
    Update();      // hook
    PostUpdate();  // hook

    BeginTextureMode(framebuffer.renderTexture);

    ClearBackground(BLACK);
    ClearBuffer();

    Draw();  // hook

    EndTextureMode();

    BeginDrawing();

    DrawTexturePro(framebuffer.renderTexture.texture, (Rectangle){0, 0, screen.width, -screen.height},
                   (Rectangle){0, 0, screen.width * screen.scale, screen.height * screen.scale}, (Vector2){0, 0}, 0,
                   WHITE);

    UpdateTexture(framebuffer.bufferTexture, framebuffer.drawBuffer);
    DrawTextureEx(framebuffer.bufferTexture, (Vector2){0, 0}, 0, screen.scale, WHITE);

    PostDraw();  // hook

    EndDrawing();
  }

  Close();  // hook

  UnloadRenderTexture(framebuffer.renderTexture);
  CloseWindow();  // Close window and OpenGL context

  return 0;
}
