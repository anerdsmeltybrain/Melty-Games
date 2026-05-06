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

enum playerAbilities {
  NOABS,
  //SWORDSMEN ABILTITIES
  AIRSLASH,
  RISINGSLASH,
  PARRY,
  //RANGER ABILITIES
  SUPERCHARGEDSHOT,
  BACKROLL,
  SNARETRAP,
  //MAGE ABILITIES
  GALVANICWALL,
  BOUNCEFORCE,
  BUBBLESHIELD
};

enum itemType {
  NAI,
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

enum projType {
  BASIC_ATTACK,
  PLAYER_ABILITY,
  USE_ITEM
};

struct Item {
  enum itemType it;
  Rectangle sourceRect;
  Rectangle destRect;
  Texture2D texture;
  bool isActive;
  Color color;
  int quantity;
  int gravCounter;
};

struct entityList {
  int capacity;
  int counter;
  struct Entity * ents;
};

struct itemList {
  int capacity;
  int counter;
  struct Item * items;
};

struct abilities {
  Texture2D UIBanner;
  enum playerAbilities pa[3];
  Texture2D textures[8];
  Vector2 positions[8];
  int timers[3];
};

struct projectile {
  enum projType pt;
  enum blockCheck dir;
  int speed;
  int damage;
  int frameSpeed;
  Texture2D texture;
  Rectangle destRect;
  Rectangle sourceRect;
  bool isActive;
  union {
    enum itemType it;
    enum playerAbilities pa;
  };
};

struct projList {
  int capacity;
  int counter;
  struct projectile * projectiles;
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
     int overHeal;
     int speed;
     int coins;
     int boosts;
     int bombs;
     int level;
     int maxHealth;
     int maxOverHeal;
     int maxBoosts;
     int maxBombs;
     enum playerType pt;
     enum blockCheck dir;
     struct abilities abs;
     struct projList projectiles;
   } p;
   struct {
     int health;
     int speed;
     enum mobType mt;
     int EXP;
     bool isDefeated;
     struct itemList drops;
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
      int maxEntities, timeCounter, numOfMobs;
      enum mobType mt;
      struct entityList mobs;
     } mobSpawnerProps;
   } b;
 }; 
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
void gameMobPlayerInter(struct game *);

//Item List Function
void initItem(struct Item *, enum itemType, Vector2); 
void drawItem(struct Item *);
void itemPhysics(struct game *, struct Item *);
void reallocItemList(struct itemList *);

//Player Functions
void drawAbilities(struct Entity *); 
void initProjectileBasic(struct projectile *, struct Entity *, Vector2);
void initProjectileAbility(struct projectile *, struct Entity *, enum playerAbilities, Vector2);
void initProjectileItem(struct projectile *, struct Entity *, enum itemType, Vector2);
void addProjectileListBasic(struct Entity *, Vector2);
void addProjectileListAbility(struct Entity *, struct projectile *, enum playerAbilities, Vector2);
void addProjectileListItem(struct Entity *, struct projectile *, enum itemType, Vector2);
void drawProjectile(struct projectile *);
void reallocProjList(struct projList *); 
void projectilePhysics(struct game *);
void playerPhysics(struct Entity *, struct entityList *);
void playerControls(struct Entity *);

//Mob Functions
void mobPhysics(struct game *, struct Entity *, struct entityList *); 
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
  addGamePlayer(&mainGame, mainGame.players, PLAYER, SWORDSMEN, (Vector2){0, 16});
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

    for(int i = 0; i < mainGame.blocks->counter; i++) {
      if(mainGame.blocks->ents[i].b.bt == MOBSPAWNER && mainGame.blocks->ents[i].b.mobSpawnerProps.mt == SLIME) {
        mobSpawnerProcess(&mainGame.blocks->ents[i], &mainGame, MOB, SLIME, (Vector2){mainGame.blocks->ents[i].destRect.x, mainGame.blocks->ents[i].destRect.y});
        for(int j = 0; j < mainGame.blocks->ents[i].b.mobSpawnerProps.mobs.counter; j++) {
          mobPhysics(&mainGame, &mainGame.blocks->ents[i].b.mobSpawnerProps.mobs.ents[j], mainGame.blocks);
          mobAnimations(&mainGame.blocks->ents[i].b.mobSpawnerProps.mobs.ents[j]);
        }
      }

    }

    for(int i = 0; i < mainGame.mobs->counter; i++) {
      mobPhysics(&mainGame, &mainGame.mobs->ents[i], mainGame.blocks);
      mobAnimations(&mainGame.mobs->ents[i]);
    }

    gameMobPlayerInter(&mainGame);

    for(int i = 0; i < mainGame.items->counter; i++) {
      itemPhysics(&mainGame, &mainGame.items->items[i]);
    }

    projectilePhysics(&mainGame);
    
    BeginDrawing();
    ClearBackground(BLACK);

    // DrawText(TextFormat("Score: %d", playerScore), 0, 0, 32, WHITE);
      drawAbilities(&mainGame.players->ents[0]);
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
  static int Scale = 3;
  ent->et = et;
  ent->isActive = true;
  ent->destRect = (Rectangle){pos.x, pos.y, 16, 16};
  ent->sourceRect = (Rectangle){pos.x, pos.y, 16, 16};
  ent->color = WHITE;
  ent->p.pt = pt;
  ent->p.dir = RIGHT;
  ent->p.overHeal = 3;
  ent->p.bombs = 3;
  ent->p.boosts = 0;
  ent->p.coins = 50;
  ent->p.level = 1;
  ent->p.maxBombs = 3;
  ent->p.maxBoosts = 3;
  ent->p.maxOverHeal = 5;
  ent->p.abs.positions[0] = (Vector2){33 * Scale, 18 * Scale};
  ent->p.abs.positions[1] = (Vector2){51 * Scale, 18 * Scale};
  ent->p.abs.positions[2] = (Vector2){69 * Scale, 18 * Scale};
  ent->p.abs.positions[3] = (Vector2){0 * Scale, 0 * Scale};
  ent->p.abs.positions[4] = (Vector2){0 * Scale, 15 * Scale};
  ent->p.abs.positions[5] = (Vector2){0 * Scale, 23 * Scale};
  ent->p.abs.positions[6] = (Vector2){0 * Scale, 31 * Scale};
  ent->p.abs.positions[7] = (Vector2){0 * Scale, 39 * Scale};
  ent->p.abs.timers[0] = 0;
  ent->p.abs.timers[1] = 0;
  ent->p.abs.timers[2] = 0;
  ent->p.abs.textures[3] = LoadTexture("./assets/healthBar.png");
  ent->p.abs.textures[4] = LoadTexture("./assets/overHeal.png");
  ent->p.abs.textures[5] = LoadTexture("./assets/speed.png");
  ent->p.abs.textures[6] = LoadTexture("./assets/bomb.png");
  ent->p.abs.textures[7] = LoadTexture("./assets/coin.png");

  ent->p.projectiles.capacity = 1;
  ent->p.projectiles.counter = 0;
  ent->p.projectiles.projectiles = malloc(ent->p.projectiles.capacity * sizeof ( struct projectile ));

  switch(ent->p.pt) {
    case SWORDSMEN:
      ent->texture = LoadTexture("./assets/swordsmen.png");
      ent->p.maxHealth = 7;
      ent->p.health = 5;
      ent->p.speed = 3;
      ent->p.abs.UIBanner = LoadTexture("./assets/swordsmenUI.png");
      ent->p.abs.pa[0] = AIRSLASH;
      ent->p.abs.pa[1] = RISINGSLASH;
      ent->p.abs.pa[2] = PARRY;
      ent->p.abs.textures[0] = LoadTexture("./assets/airSlash.png");
      ent->p.abs.textures[1] = LoadTexture("./assets/risingSlash.png");
      ent->p.abs.textures[2] = LoadTexture("./assets/parry.png");
      break;
    case RANGER:
      ent->texture = LoadTexture("./assets/ranger.png");
      ent->p.maxHealth = 5;
      ent->p.health = 3;
      ent->p.speed = 5;
      ent->p.abs.UIBanner = LoadTexture("./assets/rangerUI.png");
      ent->p.abs.pa[0] = SUPERCHARGEDSHOT;
      ent->p.abs.pa[1] = BACKROLL;
      ent->p.abs.pa[2] = SNARETRAP;
      ent->p.abs.textures[0] = LoadTexture("./assets/superchargedshot.png");
      ent->p.abs.textures[1] = LoadTexture("./assets/backroll.png");
      ent->p.abs.textures[2] = LoadTexture("./assets/snaretrap.png");
      break;
    case MAGE:
      ent->texture = LoadTexture("./assets/mage.png");
      ent->p.maxHealth = 6;
      ent->p.health = 4;
      ent->p.speed = 4;
      ent->p.abs.UIBanner = LoadTexture("./assets/mageUI.png");
      ent->p.abs.pa[0] = GALVANICWALL;
      ent->p.abs.pa[1] = BOUNCEFORCE;
      ent->p.abs.pa[2] = BUBBLESHIELD;
      ent->p.abs.textures[0] = LoadTexture("./assets/galvanicwall.png");
      ent->p.abs.textures[1] = LoadTexture("./assets/bounceforce.png");
      ent->p.abs.textures[2] = LoadTexture("./assets/bubbleshield.png");
      break;
  }

}

void drawAbilities(struct Entity * ent) {

  DrawTextureEx(ent->p.abs.UIBanner, (Vector2){0,0}, 0, 3, WHITE);
  for(int i = 0; i < ent->p.health; i++) {
    DrawTextureEx(ent->p.abs.textures[3], (Vector2){ent->p.abs.positions[3].x + (i * 32), ent->p.abs.positions[3].y}, 0, 3, WHITE);
  }
  for(int i = 0; i < ent->p.overHeal; i++) {
    DrawTextureEx(ent->p.abs.textures[4], (Vector2){ent->p.abs.positions[4].x + (i * 16), ent->p.abs.positions[4].y}, 0, 3, WHITE);
  }
  DrawTextureEx(ent->p.abs.textures[0], ent->p.abs.positions[0], 0, 3, WHITE);
  if(ent->p.abs.timers[0] > 0) {
    DrawText(TextFormat("%d", ent->p.abs.timers[0]), ent->p.abs.positions[0].x, ent->p.abs.positions[0].y, 12, WHITE);
  }
  DrawTextureEx(ent->p.abs.textures[1], ent->p.abs.positions[1], 0, 3, WHITE);
  if(ent->p.abs.timers[1] > 0) {
    DrawText(TextFormat("%d", ent->p.abs.timers[1]), ent->p.abs.positions[1].x, ent->p.abs.positions[1].y, 12, WHITE);
  }
  DrawTextureEx(ent->p.abs.textures[2], ent->p.abs.positions[2], 0, 3, WHITE);
  if(ent->p.abs.timers[2] > 0) {
    DrawText(TextFormat("%d", ent->p.abs.timers[2]), ent->p.abs.positions[2].x, ent->p.abs.positions[2].y, 12, WHITE);
  }
  DrawTextureEx(ent->p.abs.textures[5], ent->p.abs.positions[5], 0, 3, WHITE);
  for(int i = 0; i < ent->p.boosts; i++) {
    DrawTextureEx(ent->p.abs.textures[5], (Vector2){ent->p.abs.positions[5].x + (i * 24), ent->p.abs.positions[5].y}, 0, 3, WHITE);
  }
  DrawTextureEx(ent->p.abs.textures[6], ent->p.abs.positions[6], 0, 3, WHITE);
  for(int i = 0; i < ent->p.bombs; i++) {
    DrawTextureEx(ent->p.abs.textures[6], (Vector2){ent->p.abs.positions[6].x + (i * 24), ent->p.abs.positions[6].y}, 0, 3, WHITE);
  }
  DrawTextureEx(ent->p.abs.textures[7], ent->p.abs.positions[7], 0, 3, WHITE);
  DrawText(TextFormat("%d", ent->p.coins), ent->p.abs.positions[7].x + 24, ent->p.abs.positions[7].y + 16, 12, WHITE);

}

void initProjectileBasic(struct projectile * proj, struct Entity * ent, Vector2 pos) {
  proj->pt = BASIC_ATTACK;
  proj->destRect = (Rectangle){pos.x, pos.y, 6, 13};
  proj->sourceRect = (Rectangle){0, 0, 6, 13};
  proj->isActive = true;

  switch(ent->p.pt) {
    case SWORDSMEN:
      proj->damage = ent->p.level * 2;
      proj->speed = 1;
      proj->frameSpeed = 5;
      proj->texture = LoadTexture("./assets/swordsmenBasicAttack.png");
      proj->dir = ent->p.dir;
      break;
    case RANGER:
      break;
    case MAGE:
      break;
  }

  if(proj->dir == LEFT) {
    proj->speed *= -1;
    proj->sourceRect.width *= -1;
  }
  
}

void initProjectileAbility(struct projectile *, struct Entity *, enum playerAbilities, Vector2) {
  
}

void initProjectileItem(struct projectile *, struct Entity *, enum itemType, Vector2) {
  
}

void addProjectileListBasic(struct Entity * ent, Vector2 pos) {
  
  if (ent->p.projectiles.counter == 1) {
    // ent->p.projectiles->projectiles->ents = realloc(ent->p.projectiles->projectiles->ents, sizeof (struct Entity));
    initProjectileBasic(&ent->p.projectiles.projectiles[ent->p.projectiles.counter], ent, pos);
    ent->p.projectiles.counter++;
  } else {
    ent->p.projectiles.capacity++;
    // ent->p.projectiles.projectiles = realloc(ent->p.projectiles.projectiles, ent->p.projectiles.capacity * sizeof (struct projectile));
    reallocProjList(&ent->p.projectiles);
    initProjectileBasic(&ent->p.projectiles.projectiles[ent->p.projectiles.counter], ent, pos);
    ent->p.projectiles.counter++;
  }
}

void addProjectileListAbility(struct Entity *, struct projectile *, enum playerAbilities, Vector2) {
  
}

void addProjectileListItem(struct Entity * ent, struct projectile * proj, enum itemType it, Vector2 pos) {

}

void drawProjectile(struct projectile * proj) {
  if(proj->isActive == true) {
    DrawTexturePro(proj->texture, proj->sourceRect, proj->destRect, (Vector2){0,0}, 0.0f, WHITE);
  }
}

void projectilePhysics(struct game * gm) {

  for(int i = 0; i < gm->players->counter; i++) {
    for(int j = 0; j < gm->players->ents[i].p.projectiles.counter; j++) {
      gm->players->ents[i].p.projectiles.projectiles[j].destRect.x += gm->players->ents[i].p.projectiles.projectiles[j].speed;
      for(int k = 0; k < gm->blocks->counter; k++) {
          if(gm->blocks->ents[k].b.bt == MOBSPAWNER) {
            for(int h = 0; h < gm->blocks->ents[k].b.mobSpawnerProps.mobs.counter; h++) {
              if(CheckCollisionRecs(gm->players->ents[i].p.projectiles.projectiles[j].destRect, gm->blocks->ents[k].b.mobSpawnerProps.mobs.ents[h].destRect))
                gm->players->ents[i].p.projectiles.projectiles[j].isActive = false;
            }
              
        }
      }
    }
  }
    
}


void initEntityMob(struct Entity * ent, enum entityType et, enum mobType mt, Vector2 pos) {
  ent->et = et;
  ent->isActive = true;
  ent->destRect = (Rectangle){pos.x + GetRandomValue(0, 32), pos.y, 16, 16};
  ent->sourceRect = (Rectangle){0, 0, 16, 16};
  ent->color = WHITE;
  ent->m.mt = mt;
  ent->m.isDefeated = false;

  switch(ent->m.mt) {
    case SLIME:
    ent->texture = LoadTexture("./assets/slime.png");
    ent->m.health = 1;
    ent->m.speed = 1;
    ent->m.EXP = 10;
    ent->m.drops.capacity = 1;
    ent->m.drops.counter = 0;
    ent->m.drops.items = malloc(ent->m.drops.capacity * sizeof (struct Item));
      break;
    case GOBLIN:
    ent->texture = LoadTexture("./assets/goblin.png");
    ent->m.health = 2;
    ent->m.speed = 1;
    ent->m.EXP = 20;
    ent->m.drops.capacity = 2;
    ent->m.drops.counter = 0;
    ent->m.drops.items = malloc(ent->m.drops.capacity * sizeof (struct Item));
      break;
    case GOLEM:
    ent->texture = LoadTexture("./assets/golem.png");
    ent->m.health = 3;
    ent->m.speed = 1;
    ent->m.EXP = 30;
    ent->m.drops.capacity = 3;
    ent->m.drops.counter = 0;
    ent->m.drops.items = malloc(ent->m.drops.capacity * sizeof (struct Item));
      break;
    case BAT:
    ent->texture = LoadTexture("./assets/bat.png");
    ent->m.health = 1;
    ent->m.speed = 1;
    ent->m.EXP = 10;
    ent->m.drops.capacity = 1;
    ent->m.drops.counter = 0;
    ent->m.drops.items = malloc(ent->m.drops.capacity * sizeof (struct Item));
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

  ent->b.mobSpawnerProps.numOfMobs = 0;
  ent->b.mobSpawnerProps.mobs.capacity = ent->b.mobSpawnerProps.maxEntities;
  ent->b.mobSpawnerProps.mobs.counter = 0;
  ent->b.mobSpawnerProps.mobs.ents = malloc(ent->b.mobSpawnerProps.mobs.capacity * sizeof(struct Entity));
}

void drawEntity(struct Entity * ent) {

  if(ent->isActive) {
    DrawTexturePro(ent->texture, ent->sourceRect, ent->destRect, (Vector2){0.0, 0.0}, 0.0f, ent->color);
    // if(ent->et == BLOCK)
    //   DrawText(TextFormat("%d", ent->b.blockWeight), ent->destRect.x, ent->destRect.y, 8, WHITE); 
    if((ent->b.bt == MOBSPAWNER) && (ent->et == BLOCK)) {
      DrawText(TextFormat("%d", ent->b.mobSpawnerProps.numOfMobs), ent->destRect.x, ent->destRect.y, 8, WHITE); 
    }
    if((ent->et == MOB)) {
      DrawText(TextFormat("%d", ent->m.health), ent->destRect.x, ent->destRect.y, 8, WHITE); 
    }
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
  if (gm->mobs->counter == 1) {
    // gm->mobs->ents = realloc(gm->mobs->ents, sizeof (struct Entity));
    initEntityMob(&gm->mobs->ents[gm->mobs->counter], et, mt, pos);
    gm->mobs->counter++;
  } else {
    gm->mobs->capacity++;
    gm->mobs->ents = realloc(gm->mobs->ents, gm->mobs->capacity * sizeof (struct Entity));
    // reallocEntityList(el);
    initEntityMob(&gm->mobs->ents[gm->mobs->counter], et, mt, pos);
    gm->mobs->counter++;
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

void addGameItem(struct game * gm, struct itemList * el, enum itemType it, Vector2 pos) {
  
  if (gm->items->counter == 0) {
    // gm->items->ents = realloc(gm->items->ents, sizeof (struct Entity));
    initItem(&gm->items->items[gm->items->counter], it, pos);
    gm->items->counter++;
  } else {
    gm->items->capacity++;
    // gm->items->ents = realloc(gm->items->ents, gm->items->capacity * sizeof (struct Entity));
    reallocItemList(gm->items);
    initItem(&gm->items->items[gm->items->counter], it, pos);
    gm->items->counter++;
  }
}

void drawGame(struct game * gm) {

  for(int i = 0; i < gm->blocks->counter; i++) {
    drawEntity(&gm->blocks->ents[i]);
    if(gm->blocks->ents[i].b.bt == MOBSPAWNER) {
      for(int j = 0; j < gm->blocks->ents[i].b.mobSpawnerProps.mobs.counter; j++) {
        drawEntity(&gm->blocks->ents[i].b.mobSpawnerProps.mobs.ents[j]);
      }
    }
  }

  for(int i = 0; i < gm->mobs->counter; i++) {
    drawEntity(&gm->mobs->ents[i]);
  }

  for(int i = 0; i < gm->players->counter; i++) {
    drawEntity(&gm->players->ents[i]);
    for(int j = 0; j < gm->players->ents[i].p.projectiles.counter; j++) {
      drawProjectile(&gm->players->ents[i].p.projectiles.projectiles[j]);
    }
  }
  
  for(int i = 0; i < gm->items->counter; i++) {
    drawItem(&gm->items->items[i]);
  }
}

void gameMobPlayerInter(struct game * gm) {

  for(int i = 0; i < gm->players->counter; i++) {
    for(int j = 0; j < gm->mobs->counter; j++) {
      if(CheckCollisionRecs(gm->players->ents[i].destRect, gm->mobs->ents[j].destRect)) {
        gm->mobs->ents[j].m.health -= 1;
      }
    }

    for(int j = 0; j < gm->blocks->counter; j++) {
      if(gm->blocks->ents[j].b.bt == MOBSPAWNER) {
        for(int k = 0; k < gm->blocks->ents[j].b.mobSpawnerProps.mobs.counter; k++) {
          if(CheckCollisionRecs(gm->players->ents[i].destRect, gm->blocks->ents[j].b.mobSpawnerProps.mobs.ents[k].destRect)) {
            gm->blocks->ents[j].b.mobSpawnerProps.mobs.ents[k].m.health -= 1;
          }
        }
      }
    }
  }  
}

void initItem(struct Item * item, enum itemType it, Vector2 pos) {
  item->it = it;
  item->isActive = true;
  item->gravCounter = 0;

  switch(item->it) {
    case HEALTH:
      item->destRect = (Rectangle){pos.x, pos.y, 8, 8};
      item->sourceRect = (Rectangle){0, 0, 8, 8};
      item->texture = LoadTexture("./assets/heart.png");
      item->color = WHITE;
      item->quantity = 1;
      break;
    case COIN:
      item->destRect = (Rectangle){pos.x, pos.y, 8, 8};
      item->sourceRect = (Rectangle){0, 0, 8, 8};
      item->texture = LoadTexture("./assets/coin.png");
      item->color = WHITE;
      item->quantity = GetRandomValue(5, 30);
      break;
    case SPEED:
      item->destRect = (Rectangle){pos.x, pos.y, 8, 8};
      item->sourceRect = (Rectangle){0, 0, 8, 8};
      item->texture = LoadTexture("./assets/speed.png");
      item->color = WHITE;
      item->quantity = 1;
      break;
    case BOMB:
      item->destRect = (Rectangle){pos.x, pos.y, 8, 8};
      item->sourceRect = (Rectangle){0, 0, 8, 8};
      item->texture = LoadTexture("./assets/bomb.png");
      item->color = WHITE;
      item->quantity = 1;
      break;
  }
}

void drawItem(struct Item * item) {

  if(item->isActive) {
    DrawTexturePro(item->texture, item->sourceRect, item->destRect, (Vector2){0.0, 0.0}, 0.0f, item->color);
    if(item->quantity > 1)
      DrawText(TextFormat("%d", item->quantity), item->destRect.x, item->destRect.y - 8, 4, WHITE);
  }
}


void itemPhysics(struct game * gm, struct Item * it) {

  if(it->isActive == true) {
    float fallSpeed = 0.1f;
    if(it->gravCounter > 15) 
      it->destRect.y += fallSpeed;
    else
      it->destRect.y -= fallSpeed;

    for(int i = 0; i < gm->blocks->counter; i++) {
      if(CheckCollisionRecs(it->destRect, gm->blocks->ents[i].destRect)) {
        it->destRect.y -= fallSpeed;
        it->gravCounter = 0;
      }
    }

    for(int i = 0; i < gm->players->counter; i++) {
      if(CheckCollisionRecs(it->destRect, gm->players->ents[i].destRect)) {
        switch(it->it) {
          case NAI:
            break;
          case COIN:
              gm->players->ents[i].p.coins += it->quantity; 
              it->isActive = false;
            break;
          case HEALTH:
            if(gm->players->ents[i].p.health < gm->players->ents[i].p.maxHealth) {
              gm->players->ents[i].p.health += it->quantity; 
              it->isActive = false;
            } else if (gm->players->ents[i].p.overHeal < gm->players->ents[i].p.maxOverHeal) {
              gm->players->ents[i].p.overHeal += it->quantity; 
              it->isActive = false;
            }
            break;
          case SPEED:
            if(gm->players->ents[i].p.boosts < gm->players->ents[i].p.maxBoosts) {
              gm->players->ents[i].p.boosts += it->quantity; 
              it->isActive = false;
            }
            break;
          case BOMB:
            if(gm->players->ents[i].p.bombs < gm->players->ents[i].p.maxBombs) {
              gm->players->ents[i].p.bombs += it->quantity; 
              it->isActive = false;
            }
            break;
        }
      }
    }

    it->gravCounter++;
  
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

  itemList->items = realloc(itemList->items, itemList->capacity * sizeof (struct Item));
}

void reallocProjList(struct projList * projList) {
  if(projList->counter >= projList->capacity / 2) {
    projList->capacity *= 2;
  }

  projList->projectiles = realloc(projList->projectiles, projList->capacity * sizeof (struct projectile));
}

void playerPhysics(struct Entity * player, struct entityList * blocks) {

    static int timerCounter = 0;

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

    for(int i = 0; i < 3; i++) {
      if(player->p.abs.timers[i] > 0) {
        if(timerCounter % 60 == 0) {
          player->p.abs.timers[i]--;
          timerCounter = 0;
        }
      }
    }

    timerCounter++;
}

void playerControls(struct Entity * player) {

    if(IsKeyDown(KEY_D)) {
      player->destRect.x += 2;
      if(player->p.dir == LEFT) {
        player->p.dir = RIGHT;
        player->sourceRect.width *= -1;
      }
    }
  
    if(IsKeyDown(KEY_A)) {
      player->destRect.x -= 2;
      if(player->p.dir == RIGHT) {
        player->p.dir = LEFT;
        player->sourceRect.width *= -1;
      }
    }

    if(IsKeyPressed(KEY_H)) {
      addProjectileListBasic(player, (Vector2){player->destRect.x, player->destRect.y});
    }

    if(IsKeyPressed(KEY_J) && player->p.abs.timers[0] <= 0) {
      player->p.abs.timers[0] = 5;
    }

    if(IsKeyPressed(KEY_K) && player->p.abs.timers[0] <= 0) {
      player->p.abs.timers[1] = 8;
    }

    if(IsKeyPressed(KEY_L) && player->p.abs.timers[0] <= 0) {
      player->p.abs.timers[2] = 12;
    }
}

void mobPhysics(struct game * gm, struct Entity * mob, struct entityList * blocks) {
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

    if(mob->m.health <= 0) {
      if(mob->m.isDefeated == false) {
        for(int i = 0; i < mob->m.drops.capacity; i++) {
          int choice = GetRandomValue(1, 4);
          // initItem(&mob->m.drops.items[mob->m.drops.counter], choice, (Vector2){mob->destRect.x + GetRandomValue(-32, 32), mob->destRect.y});
          addGameItem(gm, gm->items, choice, (Vector2){mob->destRect.x + GetRandomValue(-32, 32), mob->destRect.y});
          mob->m.drops.counter++;
        }
        // mob->m.isDefeated = true;
      }
      mob->isActive = false;
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

void mobSpawnerProcess(struct Entity * mobSpawner, struct game * gm, enum entityType et, enum mobType mt, Vector2 pos) {
  
  static int timerCount = 0;

  if((timerCount % mobSpawner->b.mobSpawnerProps.timeCounter == 0) && (mobSpawner->b.mobSpawnerProps.numOfMobs < mobSpawner->b.mobSpawnerProps.maxEntities)) {
    // addGameMob(gm, gm->mobs, et, mt, pos);

    initEntityMob(&mobSpawner->b.mobSpawnerProps.mobs.ents[mobSpawner->b.mobSpawnerProps.mobs.counter], MOB, mobSpawner->b.mobSpawnerProps.mt, pos);


    // mobSpawner->b.mobSpawnerProps.mobs.ents[mobSpawner->b.mobSpawnerProps.mobs.counter] = gm->mobs->ents[gm->mobs->counter - 1];
    mobSpawner->b.mobSpawnerProps.numOfMobs++;
    mobSpawner->b.mobSpawnerProps.mobs.counter++;

    reallocEntityList(&mobSpawner->b.mobSpawnerProps.mobs);

  }

  for(int i = 0; i < mobSpawner->b.mobSpawnerProps.mobs.counter; i++) {
    if(mobSpawner->b.mobSpawnerProps.mobs.ents[i].isActive == false && mobSpawner->b.mobSpawnerProps.mobs.ents[i].m.isDefeated == false) {
      mobSpawner->b.mobSpawnerProps.numOfMobs--;
      mobSpawner->b.mobSpawnerProps.mobs.ents[i].m.isDefeated = true;
    }
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
