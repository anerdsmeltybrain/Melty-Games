#include <raylib.h>
#include <stdlib.h>

#define VIRTUAL_SCREEN_WIDTH 480
#define VIRTUAL_SCREEN_HEIGHT 270

enum tileType {
  GRASS,
  DIRT
};

struct Tile {
  enum tileType type;
  Texture2D texture;
  Rectangle source;
  Rectangle dest;
  int blockWeight;
  struct {
    bool NORTH;
    bool NORTHEAST;
    bool EAST;
    bool SOUTHEAST;
    bool SOUTH;
    bool WEST;
    bool SOUTHWEST;
    bool NORTHWEST;
  } blockBools;
};

struct Grid {
  int width, height;
  Vector2 * positions;
  int capacity, count;
  struct Tile * tiles;
};

//tile functions
void initTile(struct Tile *, enum tileType, Vector2);

//grid functions
void initGrid(struct Grid *, enum tileType, int, int);
void blockCheck(struct Grid *);
void setTexture(struct Grid *);
void drawGrid(struct Grid *);

int main() {
  int screenWidth = 640;
  int screenHeight = 360;

  InitWindow(screenWidth, screenHeight, "grass encounter");
  SetTargetFPS(60);

  RenderTexture2D target = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);

  struct Grid grid;
  initGrid(&grid, DIRT, 5, 5);

  blockCheck(&grid);

  Vector2 mousePos;

  while(!WindowShouldClose()) {
    mousePos.x = GetMouseX();
    mousePos.y = GetMouseY();
    
    for(int i = 0; i < grid.count; i++) {
      if(CheckCollisionPointRec(mousePos, grid.tiles[i].dest)){
        grid.tiles[i].type = GRASS;
        setTexture(&grid);
        blockCheck(&grid);
      }
    }
    //virtual screen
    BeginTextureMode(target);
      ClearBackground(BLACK);
      drawGrid(&grid);
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

void initTile(struct Tile * tile, enum tileType tt, Vector2 pos) {

  tile->type = tt;
  tile->blockWeight = 0;
  tile->blockBools.EAST = false;
  tile->blockBools.NORTHEAST = false;
  tile->blockBools.SOUTHEAST = false;
  tile->blockBools.NORTH = false;
  tile->blockBools.SOUTH = false;
  tile->blockBools.SOUTHWEST = false;
  tile->blockBools.NORTHWEST = false;
  tile->blockBools.WEST = false;

  switch(tile->type) {
    case GRASS:
      tile->texture = LoadTexture("./assets/grassTileSet.png");
      tile->dest = (Rectangle){pos.x, pos.y, 16, 16};
      tile->source = (Rectangle){16, 16, 16, 16};
      break;
    case DIRT:
      tile->texture = LoadTexture("./assets/grassTileSet.png");
      tile->dest = (Rectangle){pos.x, pos.y, 16, 16};
      tile->source = (Rectangle){48, 0, 16, 16};
      break;
  }
}


void initGrid(struct Grid * grid, enum tileType tt, int width, int height) {
  grid->width = width;
  grid->height = height;
  grid->capacity = width * height;
  grid->count = 0;
  grid->positions = malloc(grid->capacity * sizeof(Vector2));
  grid->tiles = malloc(grid->capacity * sizeof(struct Tile));

  for(int i = 0; i < grid->capacity; i++) {
    for(int w = 0; w < grid->width; w++) {
      for(int h = 0; h < grid->height; h++) {
        grid->positions[i].x = w * 16;
        grid->positions[i].y = h * 16;
        initTile(&grid->tiles[i], tt, grid->positions[i]);
        grid->count++;
      }
    }
  }

  // for(int i = 0; i < grid->width; i++) {
  //   for(int j = 0; j < grid->height; j++) {
  //     int index = i + j * grid->width;
  //     grid->positions[index].x = i * 16;
  //     grid->positions[index].y = j * 16;
  //     initTile(&grid->tiles[index], tt, grid->positions[index]);
  //     grid->count++;
  //   }
  // }
}

void blockCheck(struct Grid * grid) {

  Rectangle blockChecker = {grid->positions[0].x, grid->positions[0].y, 8, 8};

  for(int i = 0; i < grid->count; i++) {
    blockChecker.x = grid->positions[i].x - 8;
    blockChecker.y = grid->positions[i].y - 8;
    

    for(int j = 0; j < grid->count; j++) {
      if(CheckCollisionRecs(blockChecker, grid->tiles[j].dest)) {
        if(grid->tiles[j].type == GRASS) {
          grid->tiles[i].blockWeight++;
          grid->tiles[i].blockBools.NORTHWEST = true;
        }
      }
    }

    blockChecker.x += 16;

    for(int j = 0; j < grid->count; j++) {
      if(CheckCollisionRecs(blockChecker, grid->tiles[j].dest)) {
        if(grid->tiles[j].type == GRASS) {
          grid->tiles[i].blockWeight++;
          grid->tiles[i].blockBools.NORTH = true;
        }
      }
    }

    blockChecker.x += 16;
    
    for(int j = 0; j < grid->count; j++) {
      if(CheckCollisionRecs(blockChecker, grid->tiles[j].dest)) {
        if(grid->tiles[j].type == GRASS) {
          grid->tiles[i].blockWeight++;
          grid->tiles[i].blockBools.NORTHEAST = true;
        }
      }
    }

    blockChecker.y += 16;

    for(int j = 0; j < grid->count; j++) {
      if(CheckCollisionRecs(blockChecker, grid->tiles[j].dest)) {
        if(grid->tiles[j].type == GRASS) {
          grid->tiles[i].blockWeight++;
          grid->tiles[i].blockBools.EAST = true;
        }
      }
    }

    blockChecker.y += 16;

    for(int j = 0; j < grid->count; j++) {
      if(CheckCollisionRecs(blockChecker, grid->tiles[j].dest)) {
        if(grid->tiles[j].type == GRASS) {
          grid->tiles[i].blockWeight++;
          grid->tiles[i].blockBools.SOUTHEAST = true;
        }
      }
    }

    blockChecker.x -= 16;

    for(int j = 0; j < grid->count; j++) {
      if(CheckCollisionRecs(blockChecker, grid->tiles[j].dest)) {
        if(grid->tiles[j].type == GRASS) {
          grid->tiles[i].blockWeight++;
          grid->tiles[i].blockBools.SOUTH = true;
        }
      }
    }

    blockChecker.x -= 16;

    for(int j = 0; j < grid->count; j++) {
      if(CheckCollisionRecs(blockChecker, grid->tiles[j].dest)) {
        if(grid->tiles[j].type == GRASS) {
          grid->tiles[i].blockWeight++;
          grid->tiles[i].blockBools.SOUTHWEST = true;
        }
      }
    }

    blockChecker.y -= 16;

    for(int j = 0; j < grid->count; j++) {
      if(CheckCollisionRecs(blockChecker, grid->tiles[j].dest)) {
        if(grid->tiles[j].type == GRASS) {
          grid->tiles[i].blockWeight++;
          grid->tiles[i].blockBools.WEST = true;
        }
      }
    }

  }
  
}

void setTexture(struct Grid * grid) {
  for(int i  = 0; i < grid->count; i++) {
    // initTile(&grid->tiles[i], grid->tiles[i].type, grid->positions[i]);

    //NORTH WEST
    if(grid->tiles[i].blockBools.NORTHWEST == true && grid->tiles[i].blockWeight > 0) {
      grid->tiles[i].source.x = 0;
      grid->tiles[i].source.y = 0;
    }
    //NORTH
    //NORTH EAST
    //EAST
    //SOUTH EAST
    //SOUTH
    //SOUTH WEST
    //WEST
  }
}

void drawGrid(struct Grid * grid) {
  for(int i  = 0; i < grid->count; i++) {
    DrawTexturePro(grid->tiles[i].texture,
                   grid->tiles[i].source,
                   grid->tiles[i].dest,
                   (Vector2){0, 0},
                   0.0f, WHITE);
    DrawText(TextFormat("%d", grid->tiles[i].blockWeight),
                        grid->positions[i].x, grid->positions[i].y, 8, WHITE);
  }
}


