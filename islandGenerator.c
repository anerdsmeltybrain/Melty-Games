#include <raylib.h>
#include <stdlib.h>

enum blockType {
  GRASS,
  DIRT,
  STONE,
};

struct Block {
  enum blockType bt;
  Rectangle rect;
  Color color;
};

struct Level {
  int numBlocks;
  int startBlock;
  int endBlock;
  struct Block * blocks;
};

struct levelContainer {
  int numLevels;
  struct Level * levels;
};

int main() {
  int screenWidth = 640;
  int screenHeight = 480;

  InitWindow(screenWidth, screenHeight, "island gen");
  SetTargetFPS(60);

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
  }


  CloseWindow();
  return 0;
}

void generateLevel(struct Level * l) {

  l->numBlocks = 10;

  l->blocks = malloc(10 * sizeof (struct Block));

  for(int i = 0; i < 10;) {
    for(;;) {
      
    }
  }
}
