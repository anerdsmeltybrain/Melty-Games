#include "raylib.h"
#include <stdlib.h>

#define V_SCREEN_WIDTH 160
#define V_SCREEN_HEIGHT 90
#define NUM_OF_BLOCKS 10

#define PIXEL 16

enum entityType {
    PLAYER,
    MOB,
    BLOCK,
    BLOCK2
};

enum itemType {
    COIN
};

enum direction {
    LEFT,
    RIGHT
};

enum state {
    IDLE,
    WALK,
    RUN,
    JUMP
};

struct playerStats {
    int x, y;
    float velocityX, velocityY, speed;
    bool isGrounded;
    enum direction dir;
    enum state state;
};

struct mobStats {
    int x, y;
    int velocityX, velocityY;
    enum direction dir;
    enum itemType item;
};

struct Entity {
    enum entityType et;
    Rectangle source;
    Rectangle dest;
    Texture2D texture;
    bool isActive;
    union {
        struct playerStats ps;
        struct mobStats ms;
    };
};


//entity functions
void initEntity(struct Entity *, enum entityType, Vector2);
void drawEntity(struct Entity *);

//player functions
void playerPhysics(struct Entity *);
void playerBlockCollisions(struct Entity *, struct Entity *, int);
void playerMobCollisions(struct Entity *, struct Entity *, int);

int main(void) {
    const int screenWidth = 640;
    const int screenHeight = 360;

    InitWindow(screenWidth, screenHeight, "slimeJump - Raylib Window");
    SetTargetFPS(60);

    RenderTexture target = LoadRenderTexture(V_SCREEN_WIDTH, V_SCREEN_HEIGHT);
    Rectangle targetSource = {0, 0, V_SCREEN_WIDTH, -V_SCREEN_HEIGHT};
    Rectangle targetDest = {0, 0, screenWidth, screenHeight};

    struct Entity player;
    initEntity(&player, PLAYER, (Vector2){16, 48});

    int mobCount = 100;
    struct Entity * mobs = malloc(mobCount * sizeof (struct Entity));
    for(int i = 0; i < mobCount; i++) {
        initEntity((mobs + i), MOB, (Vector2){64, 48});
    }

    struct Entity * blocks = malloc(NUM_OF_BLOCKS * sizeof (struct Entity));
    for(int i = 0; i < NUM_OF_BLOCKS; i++) {
        initEntity((blocks + i), BLOCK, (Vector2){i * PIXEL, 64});
    }

    struct Entity * blocks2 = malloc(NUM_OF_BLOCKS * sizeof (struct Entity));
    for(int i = 0; i < NUM_OF_BLOCKS; i++) {
        initEntity((blocks2 + i), BLOCK2, (Vector2){i * PIXEL, 80});
    }

    int frameCounter = 0;
    bool debug = true;

    while (!WindowShouldClose()) {

        // frameCounter++;

        // if(frameCounter > 119) {
        //     for(int  i = 0; i < mobCount; i++) {
        //         if(mobs[i].isActive == false) {
        //             mobs[i].isActive = true;
        //             break;
        //         }
        //     }
        //     frameCounter = 0;
        // }

        playerPhysics(&player);
        playerBlockCollisions(&player, blocks, NUM_OF_BLOCKS);
        playerMobCollisions(&player, mobs, mobCount);

        BeginTextureMode(target);
          ClearBackground(BLACK);
          for(int i = 0; i < mobCount; i++) {
              drawEntity((mobs + i));
          }
          for(int i = 0; i < NUM_OF_BLOCKS; i++) {
              drawEntity((blocks + i));
          }
          for(int i = 0; i < NUM_OF_BLOCKS; i++) {
              drawEntity((blocks2 + i));
          }
          drawEntity(&player);

          if (debug) {
              DrawText(TextFormat("VX: %lf", player.ps.velocityX), 0, 0, 16, WHITE);
              DrawText(TextFormat("VY: %lf", player.ps.velocityY), 0, 16, 16, WHITE);
          }
        EndTextureMode();

        BeginDrawing();
          ClearBackground(BLACK);
          DrawTexturePro(target.texture, targetSource, targetDest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        EndDrawing();
        // Update logic here

    }

    free(mobs);
    free(blocks);
    CloseWindow();
    return 0;
}

void initEntity(struct Entity * ent, enum entityType et, Vector2 pos) {
    
    ent->et = et;
    ent->dest = (Rectangle){pos.x, pos.y, 16, 16};
    ent->source = (Rectangle){pos.x, pos.y, 16, 16};
    ent->isActive = true;

    switch(ent->et) {
        case PLAYER:
            ent->texture =  LoadTexture("./assets/player.png");
            ent->ps.x = pos.x;
            ent->ps.y = pos.y;
            ent->ps.velocityX = 0;
            ent->ps.velocityY = 0;
            ent->ps.isGrounded = false;
            ent->ps.speed = 0.1;
            ent->ps.dir = RIGHT;
            ent->ps.state = IDLE;
            break;
        case MOB:
            ent->texture =  LoadTexture("./assets/mob.png");
            ent->isActive = false;
            ent->ms.x = pos.x;
            ent->ms.y = pos.y;
            ent->ms.velocityX = 0;
            ent->ms.velocityY = 0;
            ent->ms.dir = RIGHT;
            ent->ms.item = COIN;
            break;
        case BLOCK:
            ent->texture =  LoadTexture("./assets/block.png");
            break;
        case BLOCK2:
            ent->texture =  LoadTexture("./assets/block2.png");
            break;
    }
}

void drawEntity(struct Entity * ent) {
    if(ent->isActive) {
        DrawTexturePro(ent->texture, ent->source, ent->dest, (Vector2){0, 0}, 0.0f, WHITE);
    }
}

void playerPhysics(struct Entity * player) {
    if(!player->ps.isGrounded)
        player->dest.y += player->ps.speed;

    if(IsKeyDown(KEY_A)) {
        if(player->source.width > 0 && player->ps.dir == RIGHT) {
            player->source.width *= -1;
            player->ps.dir = LEFT;
        }
        player->ps.velocityX -= player->ps.speed;
        if(player->ps.velocityX > 2.5)
            player->ps.velocityX = 2.5;
        player->dest.x += player->ps.velocityX;
    }

    if(IsKeyDown(KEY_D)) {
        if(player->source.width > 0 && player->ps.dir == LEFT) {
            player->source.width *= -1;
            player->ps.dir = RIGHT;
        }
        player->ps.velocityX += player->ps.speed;
        if(player->ps.velocityX < -2.5)
            player->ps.velocityX = -2.5;
        player->dest.x += player->ps.velocityX;
    }

    if(IsKeyDown(KEY_SPACE)) {
        player->ps.velocityY += player->ps.speed;
        player->dest.y -= player->ps.velocityY;
    }

    if (player->ps.velocityX > 0) {
        player->ps.velocityX -= player->ps.speed / 2;
    } else if (player->ps.velocityX < 0) {
        player->ps.velocityX += player->ps.speed / 2;
    }

    if (player->ps.velocityY > 0) {
        player->ps.velocityY -= player->ps.speed / 2;
    } else if (player->ps.velocityY < 0) {
        player->ps.velocityY += player->ps.speed / 2;
    }
}

void playerBlockCollisions(struct Entity * player, struct Entity * blocks, int blockCount) {
    for(int i = 0; i < blockCount; i++) {
        if(CheckCollisionRecs(player->dest, blocks[i].dest)) {
            player->dest.y = blocks[i].dest.y - 16;
            player->ps.isGrounded = true;
        } else {
            player->ps.isGrounded = false;
        }
    }
}

void playerMobCollisions(struct Entity * player, struct Entity * mobs, int mobCount) {
    for(int i = 0; i < mobCount; i++) {
        if(CheckCollisionRecs(player->dest, mobs[i].dest)) {
            if(mobs[i].isActive) {
                if(player->dest.y < mobs[i].dest.y - 4) {
                    mobs[i].isActive = false;
                    player->dest.y -= 16;
                }
            }
        }
    }
}


