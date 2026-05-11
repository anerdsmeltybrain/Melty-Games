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

enum playerState {
  IDLE,
  MOVING,
  JUMPING,
  ONLADDER
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
  TRUNK,
  LADDER
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
  SLIME_FRAME = 3,
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
     int damageBuffer;
     int speed;
     int speedBuff;
     int coins;
     int boosts;
     int bombs;
     int level;
     int maxHealth;
     int maxOverHeal;
     int maxBoosts;
     int maxBombs;
     enum playerType pt;
     enum playerState ps;
     int idleBuffer;
     bool idleBool;
     int speedBuffer;
     bool speedBool;
     enum blockCheck dir;
     bool damageBuff;
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
      int maxEntities, timeCount, timeCounter, numOfMobs;
      enum mobType mt;
      struct entityList mobs;
      Rectangle toggleRect;
      bool rectToggled;
     } mobSpawnerProps;
   } b;
 }; 
};

struct chunk {
  int width;
  int height;
  Vector2 pos;
  bool hasSpawner;
  bool hasLadder;
  bool willExtend;
};

struct level {
 int depth;
 int chunkCounter;
 struct chunk * chunks;
 Vector2 pos;
 };

struct damageNumber {
  Vector2 pos;
  int fontSize;
  int num;
  int r, g, b, a;
  bool isActive;
};

struct damageNumList {
  int capacity;
  int counter;
  struct damageNumber * numbers;
};

struct game {
  struct entityList * players;
  struct entityList * mobs;
  struct entityList * blocks;
  struct itemList * items;
  struct damageNumList * numbers;
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
void initGameNumbers(struct game *, int, int);
void initGame(struct game *, int, int);
void addGamePlayer(struct game *, struct entityList *, enum entityType, enum playerType, Vector2);
void addGameMob(struct game *, struct entityList *, enum entityType, enum mobType, Vector2);
void addGameBlock(struct game *, struct entityList *, enum entityType, enum blockType, Vector2);
void addGameBlockSpawner(struct game *, struct entityList *, enum entityType, enum blockType, enum mobType, Vector2);
void addGameNumber(struct game *, struct damageNumList *, int, int, int, int, int, int, Vector2);
void drawGame(struct game *);
void gameMobPlayerInter(struct game *);

//damage number
void initDamageNum(struct damageNumber *, int, int, int, int, int, int, Vector2);
void damagePhysics(struct game *);
void reallocNumList(struct damageNumList *);

//Level Generation Functions
// void addChunk(struct game *, int, int, Vector2);
void initLevel(struct game *, struct level *, int, int, Vector2 *);
void addLevelSpawners(struct game *, struct level *); 
void addChunk(struct game *, struct chunk *, enum blockType);
void initChunk(struct chunk *, int, int, bool, bool, Vector2);
void addChunkSpawner(struct game *, struct chunk *); 

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
void playerAnimations(struct Entity *);

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

  struct level mainLevel;
  initLevel(&mainGame, &mainLevel, 15, 0, &(Vector2){0,48});

  addLevelSpawners(&mainGame, &mainLevel);
  
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
    playerAnimations(&mainGame.players->ents[0]);

    // for(int i = 0; i < mainGame.blocks->counter; i++) {
    //   if(mainGame.blocks->ents[i].b.bt == MOBSPAWNER && mainGame.blocks->ents[i].isActive == true) {
    //     mobSpawnerProcess(&mainGame.blocks->ents[i], &mainGame, MOB, mainGame.blocks->ents[i].b.mobSpawnerProps.mt, (Vector2){mainGame.blocks->ents[i].destRect.x, mainGame.blocks->ents[i].destRect.y});
    //     for(int j = 0; j < mainGame.blocks->ents[i].b.mobSpawnerProps.mobs.counter; j++) {
    //       mobPhysics(&mainGame, &mainGame.blocks->ents[i].b.mobSpawnerProps.mobs.ents[j], mainGame.blocks);
    //       mobAnimations(&mainGame.blocks->ents[i].b.mobSpawnerProps.mobs.ents[j]);
    //     }
    //   }

    //   }
    
    for(int i = 0; i < mainGame.blocks->counter; i++) {
      if(mainGame.blocks->ents[i].b.bt == MOBSPAWNER) {
        mobSpawnerProcess(&mainGame.blocks->ents[i], &mainGame, MOB, mainGame.blocks->ents[i].b.mobSpawnerProps.mt, (Vector2){mainGame.blocks->ents[i].destRect.x, mainGame.blocks->ents[i].destRect.y});
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

    damagePhysics(&mainGame);
    
    BeginDrawing();
    ClearBackground(BLACK);

    // DrawText(TextFormat("Score: %d", playerScore), 0, 0, 32, WHITE);
    BeginMode2D(camera);

    drawGame(&mainGame);

    EndMode2D();
    drawAbilities(&mainGame.players->ents[0]);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

void initChunk(struct chunk * ch, int height, int width, bool hasL, bool hasS, Vector2 pos) {
  ch->height = height;
  ch->width = width;
  ch->hasLadder = hasL;
  ch->hasSpawner = hasS;
  ch->pos = pos;
}

void addChunk(struct game * gm, struct chunk * ch, enum blockType bt) {
  for(int i = 0; i < ch->width; i++) {
    for(int j = 0; j < ch->height; j++) {
      addGameBlock(gm, gm->blocks, BLOCK, bt, (Vector2){ch->pos.x + (i * 16), ch->pos.y + (j * 16)});
    }
  }

  for(int i = 0; i < gm->blocks->counter; i++) {
    if(gm->blocks->ents[i].b.bt != MOBSPAWNER) {
      blockWeightChecker(&gm->blocks->ents[i], gm->blocks->ents, gm->blocks->counter);
      blockSourceDestChecker(&gm->blocks->ents[i]);
    }
  }
}

void addChunkSpawner(struct game* gm, struct chunk * ch) {
  if(ch->hasSpawner == true) {
    addGameBlockSpawner(gm, gm->blocks, BLOCK, MOBSPAWNER, GetRandomValue(0, 3), (Vector2){ch->pos.x + (GetRandomValue(32, ch->width * 16 - 32)), ch->pos.y - 16});
  }
}

void initLevel(struct game * gm, struct level * lvl, int depth, int counter, Vector2 * pos) {
  lvl->depth = depth;
  lvl->chunkCounter = counter;
  lvl->pos = *pos;  
  lvl->chunks = malloc(lvl->depth * sizeof(struct chunk));

  for(int i = 0; i < lvl->depth; i++) {
    if(i %  4 == 0) {
      initChunk(&lvl->chunks[lvl->chunkCounter], 3, GetRandomValue(10, 12), 0, 1, lvl->pos);
      addChunk(gm, &lvl->chunks[lvl->chunkCounter], GRASS);
    } else if ( i % 2 == 0) {
      initChunk(&lvl->chunks[lvl->chunkCounter], GetRandomValue(5, 8), 1, 0, 0, lvl->pos);
      addChunk(gm, &lvl->chunks[lvl->chunkCounter], LADDER);
      lvl->pos.y += (lvl->chunks[lvl->chunkCounter].height * 16);
    } else {
      initChunk(&lvl->chunks[lvl->chunkCounter], 1, GetRandomValue(3, 7), 0, 0, lvl->pos);
      addChunk(gm, &lvl->chunks[lvl->chunkCounter], DIRT);
    }

    lvl->pos.x += (lvl->chunks[lvl->chunkCounter].width * 16);
    lvl->chunkCounter++;
  }
}

void addLevelSpawners(struct game * gm, struct level * lvl) {
  for(int i = 0; i < lvl->depth; i++) {
    addChunkSpawner(gm, &lvl->chunks[i]);
  }
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
  ent->p.ps = IDLE;
  ent->p.dir = RIGHT;
  ent->p.damageBuffer = 0;
  ent->p.overHeal = 3;
  ent->p.bombs = 3;
  ent->p.boosts = 3;
  ent->p.coins = 50;
  ent->p.level = 1;
  ent->p.maxBombs = 3;
  ent->p.maxBoosts = 3;
  ent->p.maxOverHeal = 5;
  ent->p.idleBool = true;
  ent->p.idleBuffer = 0;
  ent->p.speedBool = false;
  ent->p.speedBuffer = 0;
  ent->p.speedBuff = 0;
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
      ent->p.speed = 1 + ent->p.speedBuff;
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
      ent->p.speed = 1 + ent->p.speedBuff;
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
      ent->p.speed = 1 + ent->p.speedBuff;
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

  // DrawTextureEx(ent->p.abs.UIBanner, (Vector2){0,0}, 0, 3, WHITE);
  for(int i = 0; i < ent->p.health; i++) {
    DrawTextureEx(ent->p.abs.textures[3], (Vector2){ent->p.abs.positions[3].x + (i * 32), ent->p.abs.positions[3].y}, 0, 3, WHITE);
  }
  for(int i = 0; i < ent->p.overHeal; i++) {
    DrawTextureEx(ent->p.abs.textures[4], (Vector2){ent->p.abs.positions[4].x + (i * 16), ent->p.abs.positions[4].y}, 0, 3, WHITE);
  }
  // DrawTextureEx(ent->p.abs.textures[0], ent->p.abs.positions[0], 0, 3, WHITE);
  // if(ent->p.abs.timers[0] > 0) {
  //   DrawText(TextFormat("%d", ent->p.abs.timers[0]), ent->p.abs.positions[0].x, ent->p.abs.positions[0].y, 12, WHITE);
  // }
  // DrawTextureEx(ent->p.abs.textures[1], ent->p.abs.positions[1], 0, 3, WHITE);
  // if(ent->p.abs.timers[1] > 0) {
  //   DrawText(TextFormat("%d", ent->p.abs.timers[1]), ent->p.abs.positions[1].x, ent->p.abs.positions[1].y, 12, WHITE);
  // }
  // DrawTextureEx(ent->p.abs.textures[2], ent->p.abs.positions[2], 0, 3, WHITE);
  // if(ent->p.abs.timers[2] > 0) {
  //   DrawText(TextFormat("%d", ent->p.abs.timers[2]), ent->p.abs.positions[2].x, ent->p.abs.positions[2].y, 12, WHITE);
  // }
  if(ent->p.boosts > 0)
    DrawTextureEx(ent->p.abs.textures[5], ent->p.abs.positions[5], 0, 3, WHITE);
  for(int i = 0; i < ent->p.boosts; i++) {
    DrawTextureEx(ent->p.abs.textures[5], (Vector2){ent->p.abs.positions[5].x + (i * 24), ent->p.abs.positions[5].y}, 0, 3, WHITE);
  }
  if(ent->p.bombs > 0)
    DrawTextureEx(ent->p.abs.textures[6], ent->p.abs.positions[6], 0, 3, WHITE);
  for(int i = 0; i < ent->p.bombs; i++) {
    DrawTextureEx(ent->p.abs.textures[6], (Vector2){ent->p.abs.positions[6].x + (i * 24), ent->p.abs.positions[6].y}, 0, 3, WHITE);
  }
  DrawTextureEx(ent->p.abs.textures[7], ent->p.abs.positions[7], 0, 3, WHITE);
  DrawText(TextFormat("%d", ent->p.coins), ent->p.abs.positions[7].x + 24, ent->p.abs.positions[7].y + 16, 12, WHITE);

}

void initProjectileBasic(struct projectile * proj, struct Entity * ent, Vector2 pos) {
  proj->pt = BASIC_ATTACK;
  proj->isActive = true;

  switch(ent->p.pt) {
    case SWORDSMEN:
      proj->destRect = (Rectangle){pos.x, pos.y, 6, 13};
      proj->sourceRect = (Rectangle){0, 0, 6, 13};
      proj->damage = ent->p.level * 2;
      proj->speed = 2;
      proj->frameSpeed = 5;
      proj->texture = LoadTexture("./assets/swordsmenBasicAttack.png");
      proj->dir = ent->p.dir;
      break;
    case RANGER:
      proj->destRect = (Rectangle){pos.x, pos.y, 10, 7};
      proj->sourceRect = (Rectangle){0, 0, 10, 7};
      proj->damage = ent->p.level * 2;
      proj->speed = 2;
      proj->frameSpeed = 5;
      proj->texture = LoadTexture("./assets/rangerBasicAttack.png");
      proj->dir = ent->p.dir;
      break;
    case MAGE:
      proj->destRect = (Rectangle){pos.x, pos.y, 8, 8};
      proj->sourceRect = (Rectangle){0, 0, 8, 8};
      proj->damage = ent->p.level * 2;
      proj->speed = 2;
      proj->frameSpeed = 5;
      proj->texture = LoadTexture("./assets/mageBasicAttack.png");
      proj->dir = ent->p.dir;
      break;
  }

  if(proj->dir == LEFT) {
    proj->speed *= -1;
    proj->sourceRect.width *= -1;
  }
  
}

void initProjectileAbility(struct projectile *, struct Entity *, enum playerAbilities, Vector2) {
  
}

void initProjectileItem(struct projectile * proj, struct Entity * ent, enum itemType it, Vector2 pos) {
  
  proj->pt = USE_ITEM;
  proj->isActive = true;

  switch(it) {
    case BOMB:
      proj->destRect = (Rectangle){pos.x, pos.y, 16, 16};
      proj->sourceRect = (Rectangle){0, 0, 16, 16};
      proj->damage = ent->p.level * 3;
      proj->speed = 0;
      proj->frameSpeed = 5;
      proj->texture = LoadTexture("./assets/bigBomb.png");
      proj->dir = ent->p.dir;
      break;
  }

  if(proj->dir == LEFT) {
    proj->speed *= -1;
    proj->sourceRect.width *= -1;
  }
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

  if (ent->p.projectiles.counter == 1) {
    // ent->p.projectiles->projectiles->ents = realloc(ent->p.projectiles->projectiles->ents, sizeof (struct Entity));
    initProjectileItem(&ent->p.projectiles.projectiles[ent->p.projectiles.counter], ent, it, pos);
    ent->p.projectiles.counter++;
  } else {
    ent->p.projectiles.capacity++;
    // ent->p.projectiles.projectiles = realloc(ent->p.projectiles.projectiles, ent->p.projectiles.capacity * sizeof (struct projectile));
    reallocProjList(&ent->p.projectiles);
    initProjectileItem(&ent->p.projectiles.projectiles[ent->p.projectiles.counter], ent, it, pos);
    ent->p.projectiles.counter++;
  }
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

      if(gm->players->ents[i].p.projectiles.projectiles[j].it == BOMB) {
        gm->players->ents[i].p.projectiles.projectiles[j].destRect.y += 1;
      }
      for(int k = 0; k < gm->blocks->counter; k++) {
          if(gm->blocks->ents[k].b.bt == MOBSPAWNER) {
            if(CheckCollisionRecs(gm->blocks->ents[k].destRect, gm->players->ents[i].p.projectiles.projectiles[j].destRect)) {
              if(gm->players->ents[i].p.projectiles.projectiles[j].isActive == true) {
                gm->blocks->ents[k].isActive = false;
              }
            }
            for(int h = 0; h < gm->blocks->ents[k].b.mobSpawnerProps.mobs.counter; h++) {
              if(CheckCollisionRecs(gm->players->ents[i].p.projectiles.projectiles[j].destRect, gm->blocks->ents[k].b.mobSpawnerProps.mobs.ents[h].destRect)) {
                if((gm->blocks->ents[k].b.mobSpawnerProps.mobs.ents[h].isActive == true) && (gm->players->ents[i].p.projectiles.projectiles[j].isActive == true)) {
                  gm->blocks->ents[k].b.mobSpawnerProps.mobs.ents[h].m.health -= gm->players->ents[i].p.projectiles.projectiles[j].damage;
                  addGameNumber(gm, gm->numbers, 4, gm->players->ents[i].p.projectiles.projectiles[j].damage, 255, 255, 255, 255, (Vector2){gm->blocks->ents[k].b.mobSpawnerProps.mobs.ents[h].destRect.x, gm->blocks->ents[k].b.mobSpawnerProps.mobs.ents[h].destRect.y});
                  gm->players->ents[i].p.projectiles.projectiles[j].isActive = false;
                }
              }
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
    case LADDER:
      ent->texture = LoadTexture("./assets/ladder.png");
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
  ent->b.mobSpawnerProps.timeCount = 0;
  ent->b.mobSpawnerProps.toggleRect = (Rectangle){ent->destRect.x - (ent->destRect.width * 3),ent->destRect.y - (ent->destRect.height * 3), ent->destRect.width * 6, ent->destRect.height * 6};
  ent->b.mobSpawnerProps.rectToggled = false;

  switch(ent->b.mobSpawnerProps.mt) {
    case SLIME:
      ent->sourceRect = (Rectangle){0, 0, 16, 16};
      ent->b.mobSpawnerProps.maxEntities = 5;
      ent->b.mobSpawnerProps.timeCounter = 300;
      break;
    case GOBLIN:
      ent->sourceRect = (Rectangle){16, 0, 16, 16};
      ent->b.mobSpawnerProps.maxEntities = 5;
      ent->b.mobSpawnerProps.timeCounter = 45;
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
    // if((ent->et == MOB)) {
    //   DrawText(TextFormat("%d", ent->m.health), ent->destRect.x, ent->destRect.y, 8, WHITE); 
    // }
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

void initGameNumbers(struct game * gm, int cap, int count) {
  gm->numbers = malloc(sizeof(struct damageNumList));
  gm->numbers->capacity = cap;
  gm->numbers->counter = count;
  gm->numbers->numbers = malloc(sizeof(struct damageNumber));
}

void initGame(struct game * gm, int cap, int count) {
  initGamePlayers(gm, cap, count);
  initGameMobs(gm, cap, count);
  initGameBlocks(gm, cap, count);
  initGameItems(gm, cap, count);
  initGameNumbers(gm, cap, count);
}

void initDamageNum(struct damageNumber * dm, int fontSize, int num, int r, int b, int g, int a, Vector2 pos) {
  dm->isActive = true;
  dm->fontSize = fontSize;
  dm->num = num;
  dm->r = r;
  dm->b = b;
  dm->g = g;
  dm->a = a;
  dm->pos = pos;
}

void drawDamageNum(struct damageNumber * dm) {
  if(dm->isActive == true) {
    DrawText(TextFormat("%d", dm->num), dm->pos.x, dm->pos.y, dm->fontSize, (Color){dm->r, dm->g, dm->b, dm->a});
  }
}

void damagePhysics(struct game * gm) {
  for(int i = 0; i < gm->numbers->counter; i++) {
    if(gm->numbers->numbers[i].isActive == true) {
      gm->numbers->numbers[i].a -= 1;
      gm->numbers->numbers[i].pos.y -= 0.2;
      if(gm->numbers->numbers[i].a < 10) {
        gm->numbers->numbers[i].isActive = false;
      }
    }
  } 
}

void addGameNumber(struct game * gm, struct damageNumList * dl, int fontSize, int num, int r, int g, int b, int a, Vector2 pos) {
  if (gm->numbers->counter == 0) {
    // gm->numbers->ents = realloc(gm->numbers->ents, sizeof (struct Entity));
    initDamageNum(&gm->numbers->numbers[gm->numbers->counter], fontSize, num, r, b, g, a, pos);
    gm->numbers->counter++;
  } else {
    gm->numbers->capacity++;
    // gm->numbers->ents = realloc(gm->numbers->ents, gm->numbers->capacity * sizeof (struct Entity));
    reallocNumList(gm->numbers);
    initDamageNum(&gm->numbers->numbers[gm->numbers->counter], fontSize, num, r, b, g, a, pos);
    gm->numbers->counter++;
  }
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

  for(int i = 0; i < gm->numbers->counter; i++) {
    drawDamageNum(&gm->numbers->numbers[i]);    
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
            if(gm->blocks->ents[j].b.mobSpawnerProps.mobs.ents[k].isActive == true && gm->players->ents[i].p.damageBuff == false) {
              gm->players->ents[i].p.health -= 1;
              gm->players->ents[i].p.damageBuff = true;
              addGameNumber(gm, gm->numbers, 4, 1, 250, 5, 7, 255, (Vector2){gm->players->ents[i].destRect.x, gm->players->ents[i].destRect.y});
            }
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

void reallocNumList(struct damageNumList * numList) {
  if(numList->counter >= numList->capacity / 2) {
    numList->capacity *= 2;
  }

  numList->numbers = realloc(numList->numbers, numList->capacity * sizeof (struct projectile));
  
}

void playerPhysics(struct Entity * player, struct entityList * blocks) {

    static int timerCounter = 0;

    static int fallSpeed = 1;
    static int fallCounter = 0;
    player->destRect.y += fallSpeed;
  

    for(int i = 0; i < blocks->capacity; i++) {
      if((CheckCollisionRecs(player->destRect, blocks->ents[i].destRect)) && (blocks->ents[i].b.bt == LADDER) && (IsKeyDown(KEY_S))) {
        // player->destRect.y -= fallSpeed;
        fallCounter = 0;
        player->destRect.y += fallSpeed;
        player->p.ps = ONLADDER;
        player->p.idleBool = true;
      }

      if((CheckCollisionRecs(player->destRect, blocks->ents[i].destRect)) && (blocks->ents[i].b.bt != MOBSPAWNER)) {
        player->destRect.y -= fallSpeed;
        fallCounter = 0;
        player->p.idleBool = true;
        // player->p.ps = IDLE;
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

    if(player->p.damageBuff == true) {
      player->p.damageBuffer++;
      if(player->p.damageBuffer > 60) {
        player->p.damageBuffer = 0;
        player->p.damageBuff = false;
      }
    }

    if(player->p.idleBool == true) {
      player->p.idleBuffer++;
      if(player->p.idleBuffer > 60) {
        player->p.idleBuffer = 0;
        player->p.idleBool = false;
        player->p.ps = IDLE;
      }
    }

    if(player->p.speedBool == true) {
      player->p.speedBuffer++;
      if(player->p.speedBuffer > 300) {
        player->p.speedBuffer = 0;
        player->p.speedBool = false;
        player->p.speed = 1;
        player->p.ps = IDLE;
      }
    }
    timerCounter++;
}

void playerAnimations(struct Entity * player) {
  static int i = 0;
  switch(player->p.ps) {
    case IDLE:
      player->sourceRect.y = 0;
      if(i % 8 == 0)
        player->sourceRect.x += 16;
      break;
    case MOVING:
      player->sourceRect.y = 16;
      if(i % 8 == 0)
        if(player->p.dir == RIGHT) {
          player->sourceRect.x += 16;
        } else if (player->p.dir == LEFT) {
          player->sourceRect.x += 16;
        }
      break;
    case ONLADDER:
      player->sourceRect.y = 32;
      if(i % 8 == 0)
        player->sourceRect.x += 16;
      break;
  }
  i++;
}

void playerControls(struct Entity * player) {

    if(IsKeyDown(KEY_D)) {
      player->p.ps = MOVING;
      player->p.idleBool = true;
      player->destRect.x += player->p.speed;
      if(player->p.dir == LEFT) {
        player->p.dir = RIGHT;
        player->sourceRect.width *= -1;
      }
    } else if(IsKeyDown(KEY_A)) {
      player->p.ps = MOVING;
      player->p.idleBool = true;
      player->destRect.x -= player->p.speed;
      if(player->p.dir == RIGHT) {
        player->p.dir = LEFT;
        player->sourceRect.width *= -1;
      }
    }

    if(IsKeyPressed(KEY_H)) {
      player->p.ps = IDLE;
    }

    if(IsKeyPressed(KEY_H) && player->p.ps == IDLE) {
      addProjectileListBasic(player, (Vector2){player->destRect.x, player->destRect.y});
    }

    if(IsKeyPressed(KEY_J) && player->p.bombs > 0) {
      player->p.ps = IDLE;
      addProjectileListItem(player, &player->p.projectiles.projectiles[player->p.projectiles.counter], BOMB, (Vector2){player->destRect.x, player->destRect.y});
      player->p.bombs--;
    }
    if(IsKeyPressed(KEY_K) && player->p.boosts > 0) {
      player->p.ps = IDLE;
      player->p.speed += 1;
      player->p.speedBool = true;
      player->p.speedBuffer = 0;
      player->p.boosts--;
    }
    // if(IsKeyPressed(KEY_J) && player->p.abs.timers[0] <= 0) {
    //   player->p.abs.timers[0] = 5;
    // }

    // if(IsKeyPressed(KEY_K) && player->p.abs.timers[0] <= 0) {
    //   player->p.abs.timers[1] = 8;
    // }

    // if(IsKeyPressed(KEY_L) && player->p.abs.timers[0] <= 0) {
    //   player->p.abs.timers[2] = 12;
    // }
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
          mob->sourceRect.width *= -1;
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
  
  mobSpawner->b.mobSpawnerProps.timeCount++;

  for(int i = 0; i < gm->players->counter; i++) {
    if(CheckCollisionRecs(mobSpawner->b.mobSpawnerProps.toggleRect, gm->players->ents[i].destRect)) {
      mobSpawner->b.mobSpawnerProps.rectToggled = true;
    }
  }

  if((mobSpawner->b.mobSpawnerProps.timeCount % mobSpawner->b.mobSpawnerProps.timeCounter == 0) && (mobSpawner->b.mobSpawnerProps.numOfMobs < mobSpawner->b.mobSpawnerProps.maxEntities) && (mobSpawner->isActive == true) && (mobSpawner->b.mobSpawnerProps.rectToggled == true)) {
    // addGameMob(gm, gm->mobs, et, mt, pos);

    initEntityMob(&mobSpawner->b.mobSpawnerProps.mobs.ents[mobSpawner->b.mobSpawnerProps.mobs.counter], MOB, mobSpawner->b.mobSpawnerProps.mt, pos);


    // mobSpawner->b.mobSpawnerProps.mobs.ents[mobSpawner->b.mobSpawnerProps.mobs.counter] = gm->mobs->ents[gm->mobs->counter - 1];
    mobSpawner->b.mobSpawnerProps.numOfMobs++;
    mobSpawner->b.mobSpawnerProps.mobs.counter++;

    reallocEntityList(&mobSpawner->b.mobSpawnerProps.mobs);

    mobSpawner->b.mobSpawnerProps.timeCount = 0;
  }

  for(int i = 0; i < mobSpawner->b.mobSpawnerProps.mobs.counter; i++) {
    if(mobSpawner->b.mobSpawnerProps.mobs.ents[i].isActive == false && mobSpawner->b.mobSpawnerProps.mobs.ents[i].m.isDefeated == false) {
      mobSpawner->b.mobSpawnerProps.numOfMobs--;
      mobSpawner->b.mobSpawnerProps.mobs.ents[i].m.isDefeated = true;
    }
  }

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
