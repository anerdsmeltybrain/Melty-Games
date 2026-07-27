#include "raylib.h"

enum timeOfDay { MORNING, AFTERNOON, EVENING };

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    // Initialize render texture with virtual screen size
    const int virtualWidth = 320;
    const int virtualHeight = 180;
    RenderTexture2D target = LoadRenderTexture(virtualWidth, virtualHeight);

    SetTargetFPS(60);

    enum timeOfDay time = MORNING;
    Color colors[] = { SKYBLUE, DARKBLUE, DARKPURPLE };

    while (!WindowShouldClose())
    {
        // Update
        // TODO: Update variables here

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            time += 1;
        }

        // Draw to the render texture
        BeginTextureMode(target);
            ClearBackground(colors[time % 3]);
        EndTextureMode();

        // Draw scaled render texture to the window
        BeginDrawing();
            ClearBackground(BLACK);

            // Draw texture inverted vertically because OpenGL texture coordinates are Y-flipped
            DrawTexturePro(target.texture,
                (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height },
                (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
                (Vector2){ 0, 0 }, 0.0f, WHITE);

        EndDrawing();
    }

    // Unload render texture from GPU VRAM
    UnloadRenderTexture(target);

    CloseWindow();

    return 0;
}
