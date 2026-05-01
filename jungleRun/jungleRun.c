#include <raylib.h>
#include <stdlib.h>

enum entityType {
  PLAYER,
  MOB,
  BLOCK,
};

enum playerType {
  SWORDSMEN,
  RANGER,
  MAGE
};

enum itemType {
  HEALTH,
  COIN,
  SPEED,
  BOMB
};

enum blockType {
  MOBSPAWNER,
  GRASS,
  DIRT,
  LEAVES,
  TRUNK 
};

enum mobType {
  SLIME,
  GOBLIN,
  GOLEM,
  BAT
};

enum blockCheck {
  NONE,
  UP,
  RIGHT,
  DOWN,
  LEFT
};

enum {
  SLIME_FRAME = 3
};

struct Item {
  enum itemType it;
  Rectangle sourceRect;
  Rectangle destRect;
  Texture2D texture;
  bool isActive;
  Color color;
};

struct entityList {
  int capacity;
  int counter;
  struct Entity * ents;
};

struct Entity {
  enum entityType et;
  Rectangle sourceRect;
  Rectangle destRect;
  Texture2D texture;
  bool isActive;
  Color color;
 union {
   struct {
     int health;
     int speed;
     enum playerType pt;
   } p;
   struct {
     int health;
     int speed;
     enum mobType mt;
     struct Item dropTable[2];
   } m;
   struct {
     Rectangle blockChecker;
     enum blockCheck bc;
     enum blockType bt;
     int blockWeight;
     struct {
       bool UP, RIGHT, DOWN, LEFT;
     } blockBools;
     struct {
      int maxEntities, timeCounter;
      enum mobType mt;
      struct entityList mobs;
     } mobSpawnerProps;
   } b;
 }; 
};

struct itemList {
  int capacity;
  int counter;
  struct Item * items;
};

//Entity Functions
void initEntity(struct Entity *, enum entityType, Vector2);
void initEntityPlayer(struct Entity *, enum entityType, enum playerType, Vector2);
void initEntityMob(struct Entity *, enum entityType, enum mobType, Vector2);
void initEntityBlock(struct Entity *, enum entityType, enum blockType, Vector2);
void initEntityBlockSpawner(struct Entity *, enum entityType, enum blockType, enum mobType, Vector2);
void drawEntity(struct Entity *);

//Entity List Function
void reallocEntityList(struct entityList *);

//Item List Function
void initItem(struct Item *, enum itemType, Vector2); 
void drawItem(struct Item *);
void reallocItemList(struct itemList *);

//Player Functions
void playerPhysics(struct Entity *, struct entityList *);
void playerControls(struct Entity *);

//Mob Functions
void mobPhysics(struct Entity *, struct entityList *); 
void mobAnimations(struct Entity *);

//Mob Spawner Functions
void mobSpawnerProcess(struct Entity *);

//Block Functions
void blockWeightChecker(struct Entity *, struct Entity *, int);
void blockSourceDestChecker(struct Entity *);

int main() {
  int screenWidth = 640;
  int screenHeight = 480;
  int playerScore = 0;

  InitWindow(screenWidth, screenHeight, "Jungle Run");
  SetTargetFPS(60);

  //all things player
  /* ---------------------------------------------------------- */
  struct Entity player;
  initEntityPlayer(&player, PLAYER, RANGER, (Vector2){0, 16});
  /* ---------------------------------------------------------- */

  //all things mobs
  /* ---------------------------------------------------------- */
  struct Entity slimeSpawner;
  initEntityBlockSpawner(&slimeSpawner, BLOCK, MOBSPAWNER, SLIME, (Vector2){96, 48});
  /* ---------------------------------------------------------- */


  // all the grass on the screen
  /* ---------------------------------------------------------- */
  struct entityList grassBlocks;
  grassBlocks.capacity = 1550;
  grassBlocks.counter = 0;
  grassBlocks.ents = malloc(grassBlocks.capacity * sizeof (struct Entity));

  for(int i = 0; i < 25; i++) {
    for(int j = 0; j < 3; j++) {
       initEntity(&grassBlocks.ents[grassBlocks.counter], BLOCK, (Vector2){i * 16, j * 16 + 64});
       grassBlocks.counter++;
    }
  }

  for(int i = 0; i < grassBlocks.counter; i++) {
    blockWeightChecker(&grassBlocks.ents[i], grassBlocks.ents, grassBlocks.capacity);
    blockSourceDestChecker(&grassBlocks.ents[i]);
  }
  /* ---------------------------------------------------------- */
  
  //camera functionalities
  Camera2D camera = { 0 };
  camera.offset = (Vector2){screenWidth / 2, screenHeight / 2};
  camera.target = (Vector2){player.destRect.x + 8, player.destRect.y + 8};
  camera.rotation = 0.0f;
  camera.zoom = 2.0f;

  while(!WindowShouldClose()) {
    camera.target = (Vector2){player.destRect.x + 8, player.destRect.y + 8};
    playerPhysics(&player, &grassBlocks);
    playerControls(&player);

    mobSpawnerProcess(&slimeSpawner);
    for(int i = 0; i < slimeSpawner.b.mobSpawnerProps.mobs.counter; i++) {
      mobAnimations(&slimeSpawner.b.mobSpawnerProps.mobs.ents[i]);
      mobPhysics(&slimeSpawner.b.mobSpawnerProps.mobs.ents[i], &grassBlocks);
    }
    
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText(TextFormat("Score: %d", playerScore), 0, 0, 32, WHITE);
    BeginMode2D(camera);

      for(int i = 0; i < grassBlocks.counter; i++) {
        // blockWeightChecker(&grassTiles[i], grassTiles, 100);
        // blockSourceDestChecker(&grassBlocks.ents[i]);
        drawEntity(&grassBlocks.ents[i]);
      }

      drawEntity(&slimeSpawner);
      for(int i = 0; i < slimeSpawner.b.mobSpawnerProps.mobs.counter; i++) {
        drawEntity(&slimeSpawner.b.mobSpawnerProps.mobs.ents[i]);
      }

      drawEntity(&player);
    EndMode2D();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}


void initEntity(struct Entity * ent, enum entityType et, Vector2 pos) {

  ent->et = et;
  ent->isActive = true;

  switch(ent->et) {
    case PLAYER:
      ent->destRect = (Rectangle){pos.x, pos.y, 16, 16};
      ent->sourceRect = (Rectangle){pos.x, pos.y, 16, 16};
      ent->texture = LoadTexture("./assets/playerChar.png");
      ent->color = WHITE;
      break;
    case MOB:
      ent->destRect = (Rectangle){pos.x, pos.y, 16, 16};
      ent->sourceRect = (Rectangle){0, 0, 16, 16};
      ent->color = WHITE;
      break;
    case BLOCK:
      ent->destRect = (Rectangle){pos.x, pos.y, 16, 16};
      ent->sourceRect = (Rectangle){16, 16, 16, 16};
      ent->texture = LoadTexture("./assets/grassTileSet.png");
      ent->color = WHITE;
      ent->b.blockChecker = (Rectangle){ent->destRect.x + 8, ent->destRect.y + 8, 8, 8};
      ent->b.bc = NONE;
      ent->b.blockWeight = 0;
      ent->b.blockBools.UP = false;
      ent->b.blockBools.RIGHT = false;
      ent->b.blockBools.DOWN = false;
      ent->b.blockBools.LEFT = false;
      break;
  }
}


void initEntityPlayer(struct Entity * ent, enum entityType et, enum playerType pt, Vector2 pos) {
  ent->et = et;
  ent->isActive = true;
  ent->destRect = (Rectangle){pos.x, pos.y, 16, 16};
  ent->sourceRect = (Rectangle){pos.x, pos.y, 16, 16};
  ent->color = WHITE;
  ent->p.pt = pt;

  switch(ent->p.pt) {
    case SWORDSMEN:
      ent->texture = LoadTexture("./assets/swordsmen.png");
      ent->p.health = 5;
      ent->p.speed = 3;
      break;
    case RANGER:
      ent->texture = LoadTexture("./assets/ranger.png");
      ent->p.health = 3;
      ent->p.speed = 5;
      break;
    case MAGE:
      ent->texture = LoadTexture("./assets/mage.png");
      ent->p.health = 4;
      ent->p.speed = 4;
      break;
  }
}

void initEntityMob(struct Entity * ent, enum entityType et, enum mobType mt, Vector2 pos) {
  ent->et = et;
  ent->isActive = true;
  ent->destRect = (Rectangle){pos.x, pos.y, 16, 16};
  ent->sourceRect = (Rectangle){0, 0, 16, 16};
  ent->color = WHITE;
  ent->m.mt = mt;

  switch(ent->m.mt) {
    case SLIME:
    ent->texture = LoadTexture("./assets/slime.png");
    ent->m.health = 1;
    ent->m.speed = 1;
      break;
    case GOBLIN:
    ent->texture = LoadTexture("./assets/goblin.png");
    ent->m.health = 2;
    ent->m.speed = 1;
      break;
    case GOLEM:
    ent->texture = LoadTexture("./assets/golem.png");
    ent->m.health = 3;
    ent->m.speed = 1;
      break;
    case BAT:
    ent->texture = LoadTexture("./assets/bat.png");
    ent->m.health = 1;
    ent->m.speed = 1;
      break;
  }
}

void initEntityBlock(struct Entity * ent, enum entityType et, enum blockType bt, Vector2 pos) {
  ent->et = et;
  ent->isActive = true;
  ent->destRect = (Rectangle){pos.x, pos.y, 16, 16};
  ent->sourceRect = (Rectangle){0, 0, 16, 16};
  ent->color = WHITE;
  ent->b.bt = bt;
  ent->b.blockChecker = (Rectangle){ent->destRect.x + 8, ent->destRect.y + 8, 8, 8};
  ent->b.bc = NONE;
  ent->b.blockWeight = 0;
  ent->b.blockBools.UP = false;
  ent->b.blockBools.RIGHT = false;
  ent->b.blockBools.DOWN = false;
  ent->b.blockBools.LEFT = false;

  switch(ent->b.bt) {
    case GRASS:
      ent->texture = LoadTexture("./assets/grassTileSet.png");
      break;
    case DIRT:
      ent->texture = LoadTexture("./assets/dirtTileSet.png");
      break;
    case LEAVES:
      ent->texture = LoadTexture("./assets/leavesTileSet.png");
      break;
    case TRUNK:
      ent->texture = LoadTexture("./assets/trunksTileSet.png");
      break;
  }
}

void initEntityBlockSpawner(struct Entity * ent, enum entityType et, enum blockType bt, enum mobType mt, Vector2 pos) {
  ent->et = et;
  ent->isActive = true;
  ent->destRect = (Rectangle){pos.x, pos.y, 16, 16};
  ent->texture = LoadTexture("./assets/enemySpawners.png");
  ent->color = WHITE;
  ent->b.bt = bt;
  ent->b.blockChecker = (Rectangle){ent->destRect.x + 8, ent->destRect.y + 8, 8, 8};
  ent->b.bc = NONE;
  ent->b.blockWeight = 0;
  ent->b.blockBools.UP = false;
  ent->b.blockBools.RIGHT = false;
  ent->b.blockBools.DOWN = false;
  ent->b.blockBools.LEFT = false;
  ent->b.mobSpawnerProps.mt = mt;
  ent->b.mobSpawnerProps.mobs.counter = 0;
  ent->b.mobSpawnerProps.mobs.capacity = 1;
  ent->b.mobSpawnerProps.mobs.ents = malloc(sizeof(struct Entity));

  switch(ent->b.mobSpawnerProps.mt) {
    case SLIME:
      ent->sourceRect = (Rectangle){0, 0, 16, 16};
      ent->b.mobSpawnerProps.maxEntities = 5;
      ent->b.mobSpawnerProps.timeCounter = 30;
      break;
    case GOBLIN:
      ent->sourceRect = (Rectangle){16, 0, 16, 16};
      ent->b.mobSpawnerProps.maxEntities = 5;
      ent->b.mobSpawnerProps.timeCounter = 45;
      break;
    case GOLEM:
      ent->sourceRect = (Rectangle){32, 0, 16, 16};
      ent->b.mobSpawnerProps.maxEntities = 5;
      ent->b.mobSpawnerProps.timeCounter = 60;
      break;
    case BAT:
      ent->sourceRect = (Rectangle){48, 0, 16, 16};
      ent->b.mobSpawnerProps.maxEntities = 5;
      ent->b.mobSpawnerProps.timeCounter = 30;
      break;
  }
}

void drawEntity(struct Entity * ent) {

  if(ent->isActive) {
    DrawTexturePro(ent->texture, ent->sourceRect, ent->destRect, (Vector2){0.0, 0.0}, 0.0f, ent->color);
    if(ent->et == BLOCK)
      DrawText(TextFormat("%d", ent->b.blockWeight), ent->destRect.x, ent->destRect.y, 8, WHITE); 
  }
}

void initItem(struct Item * item, enum itemType it, Vector2 pos) {
  item->it = it;
  item->isActive = true;

  switch(item->it) {
    case HEALTH:
      break;
    case COIN:
      item->destRect = (Rectangle){pos.x, pos.y, 8, 8};
      item->sourceRect = (Rectangle){0, 0, 8, 8};
      item->texture = LoadTexture("./assets/coin.png");
      item->color = WHITE;
      break;
    case SPEED:
      break;
    case BOMB:
      break;
  }
}

void drawItem(struct Item * item) {

  if(item->isActive) {
    DrawTexturePro(item->texture, item->sourceRect, item->destRect, (Vector2){0.0, 0.0}, 0.0f, item->color);
    // if(ent->et == BLOCK)
    //   DrawText(TextFormat("%d", ent->b.blockWeight), ent->destRect.x, ent->destRect.y, 8, WHITE); 
  }
}
void reallocEntityList(struct entityList * entList) {
  if(entList->counter >= entList->capacity / 2) {
    entList->capacity *= 2;
  }

  entList->ents = realloc(entList->ents, entList->capacity * sizeof (struct Entity));
}

void reallocItemList(struct itemList * itemList) {
  if(itemList->counter >= itemList->capacity / 2) {
    itemList->capacity *= 2;
  }

  itemList->items = realloc(itemList->items, itemList->capacity * sizeof (struct Entity));
}

void playerPhysics(struct Entity * player, struct entityList * blocks) {

    static int fallSpeed = 1;
    static int fallCounter = 0;
    player->destRect.y += fallSpeed;
  

    for(int i = 0; i < blocks->capacity; i++) {
      if(CheckCollisionRecs(player->destRect, blocks->ents[i].destRect)) {
        player->destRect.y -= fallSpeed;
        fallCounter = 0;
      }
    }

    if(IsKeyDown(KEY_W) && fallCounter < 30) {
      player->destRect.y -= 2;
      fallCounter++;
    }

}

void playerControls(struct Entity * player) {

    if(IsKeyDown(KEY_D)) {
      player->destRect.x += 2;
    }
  
    if(IsKeyDown(KEY_A)) {
      player->destRect.x -= 2;
    }
}

void mobPhysics(struct Entity * mob, struct entityList * blocks) {
  static int fallSpeed = 1;
  mob->destRect.y += fallSpeed;
  mob->destRect.x += mob->m.speed;
  
    for(int i = 0; i < blocks->capacity; i++) {
      if(CheckCollisionRecs(mob->destRect, blocks->ents[i].destRect)) {
        mob->destRect.y -= fallSpeed;
        if (blocks->ents[i].b.blockWeight == 2) {
          mob->m.speed *= -1;
        }
      }
    }
}

void mobAnimations(struct Entity * mob) {
  static int i = 0;
  switch(mob->m.mt) {
    case SLIME:
      if(i % 8 == 0)
        mob->sourceRect.x += 16;
      break;
    case GOBLIN:
      break;
    case GOLEM:
      break;
    case BAT:
      break;
  }
  i++;
}

void mobSpawnerProcess(struct Entity * ent) {
  
  static int timerCount = 0;

  if((timerCount % ent->b.mobSpawnerProps.timeCounter == 0) && (ent->b.mobSpawnerProps.mobs.capacity <= ent->b.mobSpawnerProps.maxEntities)) {
    ent->b.mobSpawnerProps.mobs.capacity++;
    ent->b.mobSpawnerProps.mobs.ents = realloc(ent->b.mobSpawnerProps.mobs.ents, ent->b.mobSpawnerProps.mobs.capacity * sizeof (struct Entity));
    initEntityMob(&ent->b.mobSpawnerProps.mobs.ents[ent->b.mobSpawnerProps.mobs.counter], MOB, ent->b.mobSpawnerProps.mt, (Vector2){ent->destRect.x, ent->destRect.y});
    ent->b.mobSpawnerProps.mobs.counter++;
  }

  timerCount++;
}

void blockWeightChecker(struct Entity * block, struct Entity * blocks, int blockAmount) {

    block->b.blockWeight = 0;

    switch(block->b.bc) {
      case NONE:
      case UP:
        block->b.blockChecker.y -= 16;
        for(int i = 0; i < blockAmount; i++) {
          if(CheckCollisionRecs(block->b.blockChecker, blocks[i].destRect)) {
            block->b.blockBools.UP = true;
            block->b.blockWeight++;
            break;
          }
        }
        block->b.blockChecker.y += 16;
      case RIGHT:
        block->b.blockChecker.x += 16;
        for(int i = 0; i < blockAmount; i++) {
          if(CheckCollisionRecs(block->b.blockChecker, blocks[i].destRect)) {
            block->b.blockBools.RIGHT = true;
            block->b.blockWeight++;
            break;
          }
        }
        block->b.blockChecker.x -= 16;
      case DOWN:
        block->b.blockChecker.y += 16;
        for(int i = 0; i < blockAmount; i++) {
          if(CheckCollisionRecs(block->b.blockChecker, blocks[i].destRect)) {
            block->b.blockBools.DOWN = true;
            block->b.blockWeight++;
            break;
          }
        }
        block->b.blockChecker.y -= 16;
      case LEFT:
        block->b.blockChecker.x -= 16;
        for(int i = 0; i < blockAmount; i++) {
          if(CheckCollisionRecs(block->b.blockChecker, blocks[i].destRect)) {
            block->b.blockBools.LEFT = true;
            block->b.blockWeight++;
            break;
          }
        }
        block->b.blockChecker.x += 16;
        break;
    }
}

void blockSourceDestChecker(struct Entity * ent) {

  // if(ent->b.blockBools.DOWN == true && ent->b.blockBools.RIGHT == true && ent->b.blockBools.LEFT == true && ent->b.blockBools.UP == false) {
  //   ent->sourceRect.x = 16;
  //   ent->sourceRect.y = 0;
  // }
  
  //middle top grass block
  if((ent->b.blockWeight == 3 || ent->b.blockWeight == 1 || ent->b.blockWeight == 2)&& ent->b.blockBools.UP == false) {
    ent->sourceRect.x = 16;
    ent->sourceRect.y = 0;
  }

  //inner dirt block
  if(ent->b.blockWeight > 3) {
    ent->sourceRect.x = 16;
    ent->sourceRect.y = 16;
  }

  //side grass block right
   if(ent->b.blockWeight == 2 && ent->b.blockBools.UP == false && ent->b.blockBools.RIGHT == false) {
     ent->sourceRect.x = 32;
     ent->sourceRect.y = 0;
   }

   //side grass block left
   if(ent->b.blockWeight == 2 && ent->b.blockBools.UP == false && ent->b.blockBools.LEFT == false) {
     ent->sourceRect.x = 0;
     ent->sourceRect.y = 0;
   }
  
  //side dirt block right
  if(ent->b.blockWeight == 3 && ent->b.blockBools.RIGHT == false) {
    ent->sourceRect.x = 32;
    ent->sourceRect.y = 16;
  }

  //side dirt block left
  if(ent->b.blockWeight == 3 && ent->b.blockBools.LEFT == false) {
    ent->sourceRect.x = 0;
    ent->sourceRect.y = 16;
  }

  //side dirt bottom right
   if(ent->b.blockWeight == 2 && ent->b.blockBools.DOWN  == false && ent->b.blockBools.RIGHT == false) {
     ent->sourceRect.x = 32;
     ent->sourceRect.y = 32;
   }

   //side grass block left
   if(ent->b.blockWeight == 2 && ent->b.blockBools.DOWN == false && ent->b.blockBools.LEFT == false) {
     ent->sourceRect.x = 0;
     ent->sourceRect.y = 32;
   }

  //bottom middle dirt block
  if(ent->b.blockWeight == 3 && ent->b.blockBools.DOWN == false) {
    ent->sourceRect.x = 16;
    ent->sourceRect.y = 32;
  }
}
