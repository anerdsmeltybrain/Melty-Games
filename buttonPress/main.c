#include <raylib.h>

struct Button {
  Texture2D texture;
  Rectangle Source;
  Rectangle Dest;
};

void initButton(struct Button *);
void updateButton(struct Button *);
void drawButton(struct Button *);

int main() {

  int screenWidth = 640;
  int screenHeight = 480;

  InitWindow(screenWidth, screenHeight, "Button Press");
  SetTargetFPS(60);

  struct Button butt;
  initButton(&butt);

  int score = 0;

  while(!WindowShouldClose()) {

    if(CheckCollisionPointRec((Vector2){GetMouseX(), GetMouseY()}, butt.Dest)) {
      if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        butt.Source.x = 16;
      } else if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        butt.Source.x = 0;
        score++;
      }
    } 
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText(TextFormat("%d", score), GetScreenWidth() / 2 - 16, GetScreenHeight() / 2 - 32, 32, WHITE);

    drawButton(&butt);

    EndDrawing();
  }

  UnloadTexture(butt.texture);
  CloseWindow();
  return 0;
}

void initButton(struct Button * b) {

  b->texture = LoadTexture("assets/buttonPress.png");
  b->Dest = (Rectangle){GetScreenWidth() / 2 - 8, GetScreenHeight() / 2, 16, 16};
  b->Source = (Rectangle){0,0, 16, 16};
   
}

void updateButton(struct Button * b) {
  
  
}

void drawButton(struct Button * b) {

  DrawTexturePro(b->texture, b->Source, b->Dest, (Vector2){0,0},0.0f, WHITE);
  // DrawTexture(b->texture, 0, 0, WHITE);

}
