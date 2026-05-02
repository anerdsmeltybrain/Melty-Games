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

struct game {
  struct entityList * players;
  struct entityList * mobs;
  struct entityList * blocks;
  struct itemList * items;
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

//Game Struct Functions
void initGamePlayers(struct game *, int, int);
void initGameMobs(struct game *, int, int);
void initGameBlocks(struct game *, int, int);
void initGameItems(struct game *, int, int);
void initGame(struct game *, int, int);
void addGamePlayer(struct game *, struct entityList *, enum entityType, enum playerType, Vector2);
void addGameMob(struct game *, struct entityList *, enum entityType, enum mobType, Vector2);
void addGameBlock(struct game *, struct entityList *, enum entityType, enum blockType, Vector2);
void addGameBlockSpawner(struct game *, struct entityList *, enum entityType, enum blockType, enum mobType, Vector2);
void drawGame(struct game *);

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
void mobSpawnerProcess(struct Entity *, struct game *, enum entityType, enum mobType, Vector2);

//Block Functions
void blockWeightChecker(struct Entity *, struct Entity *, int);
void blockSourceDestChecker(struct Entity *);

int main() {
  int screenWidth = 640;
  int screenHeight = 480;
  int playerScore = 0;

  InitWindow(screenWidth, screenHeight, "Jungle Run");
  SetTargetFPS(60);

  //the whole damn game
  struct game mainGame;
  initGame(&mainGame, 5, 0);

  //all things player
  /* ---------------------------------------------------------- */
  addGamePlayer(&mainGame, mainGame.players, PLAYER, MAGE, (Vector2){0, 16});
  /* ---------------------------------------------------------- */

  //all things mobs
  /* ---------------------------------------------------------- */
  /* ---------------------------------------------------------- */


  // all things blocks
  /* ---------------------------------------------------------- */
  for(int i = 0; i < 25; i++) {
    for(int j = 0; j < 4; j++) {
      addGameBlock(&mainGame, mainGame.blocks, BLOCK, GRASS, (Vector2){i * 16, j * 16 + 48});
    }
  }

  for(int i = 0; i < mainGame.blocks->counter; i++) {
    blockWeightChecker(&mainGame.blocks->ents[i], mainGame.blocks->ents, mainGame.blocks->counter);
    blockSourceDestChecker(&mainGame.blocks->ents[i]);
  }

  addGameBlockSpawner(&mainGame, mainGame.blocks, BLOCK, MOBSPAWNER, SLIME, (Vector2){32, 32});
  /* ---------------------------------------------------------- */
  
  //camera functionalities
  Camera2D camera = { 0 };
  camera.offset = (Vector2){screenWidth / 2, screenHeight / 2};
  camera.target = (Vector2){mainGame.players->ents[0].destRect.x + 8, mainGame.players->ents[0].destRect.y + 8};
  camera.rotation = 0.0f;
  camera.zoom = 2.0f;

  while(!WindowShouldClose()) {
    camera.target = (Vector2){mainGame.players->ents[0].destRect.x + 8, mainGame.players->ents[0].destRect.y + 8};
    playerPhysics(&mainGame.players->ents[0], mainGame.blocks);
    playerControls(&mainGame.players->ents[0]);
    
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText(TextFormat("Score: %d", playerScore), 0, 0, 32, WHITE);
    BeginMode2D(camera);

      drawGame(&mainGame);

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
      ent->b.mobSpawnerProps.timeCounter = 300;
      break;
    case GOBLIN:
      ent->sourceRect = (Rectangle){16, 0, 16, 16};
      ent->b.mobSpawnerProps.maxEntities = 5;
      ent->b.mobSpawnerProps.timeCounter = 450;
      break;
    case GOLEM:
      ent->sourceRect = (Rectangle){32, 0, 16, 16};
      ent->b.mobSpawnerProps.maxEntities = 5;
      ent->b.mobSpawnerProps.timeCounter = 600;
      break;
    case BAT:
      ent->sourceRect = (Rectangle){48, 0, 16, 16};
      ent->b.mobSpawnerProps.maxEntities = 5;
      ent->b.mobSpawnerProps.timeCounter = 300;
      break;
  }
}

void drawEntity(struct Entity * ent) {

  if(ent->isActive) {
    DrawTexturePro(ent->texture, ent->sourceRect, ent->destRect, (Vector2){0.0, 0.0}, 0.0f, ent->color);
    // if(ent->et == BLOCK)
    //   DrawText(TextFormat("%d", ent->b.blockWeight), ent->destRect.x, ent->destRect.y, 8, WHITE); 
  }
}

void initGamePlayers(struct game * gm, int cap, int count) {
  gm->players = malloc(sizeof(struct entityList));
  gm->players->capacity = cap;
  gm->players->counter = count;
  gm->players->ents = malloc(sizeof (struct Entity));
}

void initGameMobs(struct game * gm, int cap, int count) {
  gm->mobs = malloc(sizeof(struct entityList));
  gm->mobs->capacity = cap;
  gm->mobs->counter = count;
  gm->mobs->ents = malloc(sizeof (struct Entity));
}

void initGameBlocks(struct game * gm, int cap, int count) {
  gm->blocks = malloc(sizeof(struct entityList));
  gm->blocks->capacity = cap;
  gm->blocks->counter = count;
  gm->blocks->ents = malloc(sizeof (struct Entity));
}

void initGameItems(struct game * gm, int cap, int count) {
  gm->items = malloc(sizeof(struct itemList));
  gm->items->capacity = cap;
  gm->items->counter = count;
  gm->items->items = malloc(sizeof (struct Item));
}

void initGame(struct game * gm, int cap, int count) {
  initGamePlayers(gm, cap, count);
  initGameMobs(gm, cap, count);
  initGameBlocks(gm, cap, count);
  initGameItems(gm, cap, count);
}

void addGamePlayer(struct game * gm, struct entityList * el, enum entityType et, enum playerType pt, Vector2 pos) {
  if (gm->players->counter == 1) {
    // gm->players->ents = realloc(gm->players->ents, sizeof (struct Entity));
    initEntityPlayer(&gm->players->ents[gm->players->counter], et, pt, pos);
    gm->players->counter++;
  } else {
    gm->players->capacity++;
    gm->players->ents = realloc(gm->players->ents, gm->players->capacity * sizeof (struct Entity));
    initEntityPlayer(&gm->players->ents[gm->players->counter], et, pt, pos);
    gm->players->counter++;
  }
}

void addGameMob(struct game * gm, struct entityList * el, enum entityType et, enum mobType mt, Vector2 pos) {
  if (gm->players->counter == 1) {
    // gm->players->ents = realloc(gm->players->ents, sizeof (struct Entity));
    initEntityMob(&gm->players->ents[gm->players->counter], et, mt, pos);
    gm->players->counter++;
  } else {
    gm->players->capacity++;
    // gm->players->ents = realloc(gm->players->ents, gm->players->capacity * sizeof (struct Entity));
    reallocEntityList(el);
    initEntityMob(&gm->players->ents[gm->players->counter], et, mt, pos);
    gm->players->counter++;
  }
  
}

void addGameBlock(struct game * gm, struct entityList * el, enum entityType et, enum blockType bt, Vector2 pos) {
  
  if (gm->blocks->counter == 0) {
    // gm->blocks->ents = realloc(gm->blocks->ents, sizeof (struct Entity));
    initEntityBlock(&gm->blocks->ents[gm->blocks->counter], et, bt, pos);
    gm->blocks->counter++;
  } else {
    gm->blocks->capacity++;
    // gm->blocks->ents = realloc(gm->blocks->ents, gm->blocks->capacity * sizeof (struct Entity));
    reallocEntityList(gm->blocks);
    initEntityBlock(&gm->blocks->ents[gm->blocks->counter], et, bt, pos);
    gm->blocks->counter++;
  }
}

void addGameBlockSpawner(struct game * gm, struct entityList * el, enum entityType et, enum blockType bt, enum mobType mt, Vector2 pos) {
  
  if (gm->blocks->counter == 0) {
    // gm->blocks->ents = realloc(gm->blocks->ents, sizeof (struct Entity));
    initEntityBlockSpawner(&gm->blocks->ents[gm->blocks->counter], et, bt, mt, pos);
    gm->blocks->counter++;
  } else {
    gm->blocks->capacity++;
    // gm->blocks->ents = realloc(gm->blocks->ents, gm->blocks->capacity * sizeof (struct Entity));
    reallocEntityList(gm->blocks);
    initEntityBlockSpawner(&gm->blocks->ents[gm->blocks->counter], et, bt, mt, pos);
    gm->blocks->counter++;
  }
}

void drawGame(struct game * gm) {

  for(int i = 0; i < gm->blocks->counter; i++) {
    drawEntity(&gm->blocks->ents[i]);
  }

  for(int i = 0; i < gm->mobs->counter; i++) {
    drawEntity(&gm->mobs->ents[i]);
  }

  for(int i = 0; i < gm->players->counter; i++) {
    drawEntity(&gm->players->ents[i]);
  }
  
  // for(int i = 0; i < gm->items->counter; i++) {
  //   drawItem(&gm->items->items[i]);
  // }
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
      if((CheckCollisionRecs(player->destRect, blocks->ents[i].destRect)) && (blocks->ents[i].b.bt != MOBSPAWNER)) {
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

// void addGameMob(struct game * gm, struct entityList * el, enum entityType et, enum mobType mt, Vector2 pos) 
void mobSpawnerProcess(struct Entity * mobSpawner, struct game * gm, enum entityType et, enum mobType mt, Vector2 pos) {
  
  static int timerCount = 0;

  if((timerCount % mobSpawner->b.mobSpawnerProps.timeCounter == 0) && (mobSpawner->b.mobSpawnerProps.mobs.capacity <= mobSpawner->b.mobSpawnerProps.maxEntities)) {
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
