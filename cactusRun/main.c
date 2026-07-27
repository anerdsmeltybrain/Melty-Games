#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define V_SCREEN_WIDTH 320
#define V_SCREEN_HEIGHT 180

enum entityType {
  PLAYER,
  MOB,
  BLOCK
};

struct Entity {
  enum entityType et;
  Texture2D texture;
  Rectangle source;
  Rectangle dest;
};

struct entityList {
  int capacity, count;
  struct Entity * list; 
};

//entity functions
void initEntity(struct Entity *, enum entityType, Vector2);
void drawEntity(struct Entity *);

//player functions
void playerPhysics(struct Entity *, bool *, struct entityList *, struct entityList *);
void playerControls(struct Entity *, bool *);

//entity list functions
void initEntityList(struct entityList *);
void addEntityList(struct entityList *, enum entityType, Vector2);
void freeEntityList(struct entityList *);
void drawEntityList(struct entityList *);

int main() {

  srand(time(NULL));

  int screenWidth = 1280;
  int screenHeight = 720;

  InitWindow(screenWidth, screenHeight, "Cactus Run");
  SetTargetFPS(60);

  RenderTexture target = LoadRenderTexture(V_SCREEN_WIDTH, V_SCREEN_HEIGHT);
  Rectangle targetSource = {0, 0, V_SCREEN_WIDTH, -V_SCREEN_HEIGHT};
  Rectangle targetDest = {0, 0, screenWidth, screenHeight};

  struct Entity player;
  bool isGrounded = false;
  initEntity(&player, PLAYER, (Vector2){32, 64});

  struct entityList blocks;
  initEntityList(&blocks);
  Vector2 blockPos = (Vector2){0, 112};
  addEntityList(&blocks, BLOCK, blockPos);

  // for(int i = 0; i < 5; i++) {
  //   blockPos.x += 16;
  //   addEntityList(&blocks, BLOCK, blockPos);
  // }

  struct entityList mobs;
  initEntityList(&mobs);

  int counter = 0;

  while(!WindowShouldClose()) {

    counter++;

    if(counter >= 120) {
      addEntityList(&mobs, MOB, (Vector2){V_SCREEN_WIDTH, 96});
      counter = 0;
    }

    if(mobs.count > 0) {
      for(int i = 0; i < mobs.count; i++) {
        mobs.list[i].dest.x -= 1;
      }
    }

    playerPhysics(&player, &isGrounded, &blocks, &mobs);
    playerControls(&player, &isGrounded);

    if(blockPos.x < 320) {
      addEntityList(&blocks, BLOCK, blockPos);
      blockPos.x += 16;
    }

    BeginTextureMode(target);
      ClearBackground(BLACK);
      drawEntityList(&blocks);
      drawEntityList(&mobs);
      drawEntity(&player);
    EndTextureMode();

    BeginDrawing();
      ClearBackground(BLACK);
      DrawTexturePro(target.texture, targetSource, targetDest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    EndDrawing();
  }

  freeEntityList(&blocks);
  freeEntityList(&mobs);
  CloseWindow();
  return 0;
}

void initEntity(struct Entity * ent, enum entityType et, Vector2 pos) {
  ent->et = et;
  ent->dest = (Rectangle){pos.x, pos.y, 16, 16};
  ent->source = (Rectangle){pos.x, pos.y, 16, 16};

  switch(ent->et) {
    case PLAYER:
      ent->texture = LoadTexture("assets/sheriff.png");
      break;
    case MOB:
      ent->texture = LoadTexture("assets/cactus.png");
      int randInt = rand() % 2;
      ent->source = (Rectangle){pos.x, pos.y + (16 * randInt), 16, 16};
      break;
    case BLOCK:
      ent->texture = LoadTexture("assets/sand.png");
      break;
  }
}

void drawEntity(struct Entity * ent) {
  DrawTexturePro(ent->texture, ent->source, ent->dest, (Vector2){0, 0}, 0.0f, WHITE);
}

void playerPhysics(struct Entity * player, bool * isGrounded, struct entityList * blocks, struct entityList * mobs) {

  static int jumpCounter = 0;
    
  if(*isGrounded == false) {
    player->dest.y += 1;
  } else if (*isGrounded == true) {
    player->dest.y -= 1;
    jumpCounter++;
    if (jumpCounter >= 45) {
      *isGrounded = false;
    }
  }

  for(int i = 0; i < blocks->count; i++) {
    if(CheckCollisionRecs(player->dest, blocks->list[i].dest)) {
      player->dest.y -= 1;
      jumpCounter = 0;
      // *isGrounded = true;
    }
  }
}

void playerControls(struct Entity * player, bool * isGrounded) {

  if(IsKeyDown(KEY_SPACE) && *isGrounded == false) {
    *isGrounded = true;
  }
}

void initEntityList(struct entityList * etl) {

  etl->capacity = 1;
  etl->count = 0;
  etl->list = NULL;
  
}

void addEntityList(struct entityList * etl, enum entityType et, Vector2 pos) {

  if(etl->count == 0) {
    etl->list = malloc(etl->capacity * sizeof (struct Entity));
    initEntity(&etl->list[etl->count], et, pos);
    etl->count++;
    etl->capacity++;
  } else {
    etl->list = realloc(etl->list, etl->capacity * sizeof (struct Entity));
    initEntity(&etl->list[etl->count], et, pos);
    etl->count++;
    etl->capacity++;
  }
  
}

void freeEntityList(struct entityList * etl) {
  free(etl->list);
}

void drawEntityList(struct entityList * etl) {
  for(int i = 0; i < etl->count; i++) {
    drawEntity(&etl->list[i]);
  }
}
