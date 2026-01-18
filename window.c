#include <raylib.h>

int main() {
  int screenWidth = 640;
  int screenHeight = 480;
  InitWindow(screenWidth, screenHeight, "window");
  SetTargetFPS(60);

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
  }

  return 0;
}
