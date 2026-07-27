#include <raylib.h>

#define VIRTUAL_SCREEN_WIDTH 480
#define VIRTUAL_SCREEN_HEIGHT 270

int main() {
  int screenWidth = 640;
  int screenHeight = 360;

  InitWindow(screenWidth, screenHeight, "boardGame");
  SetTargetFPS(60);

  RenderTexture2D target = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);

  while(!WindowShouldClose()) {

    //virtual screen
    BeginTextureMode(target);
    ClearBackground(BLACK);
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);

      DrawTexturePro(target.texture,
                     (Rectangle){0,0, (float)target.texture.width, (float)-target.texture.height},
                     (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                     (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
