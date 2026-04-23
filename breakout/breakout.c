#include <stdbool.h>
#include <stdio.h>
#include <raylib.h>

#define SCREEN_WIDTH 640;
#define SCREEN_HEIGHT 480;

enum entityType {
  BALL,
  PADDLE,
  BRICK
};

struct Ball {
  int speedX, speedY;
};
struct Paddle {
  int speed;
};
struct Brick {
  int health;
};

struct Entity {
  Rectangle rect;
  Color color;
  bool isActive;
  enum entityType et;
  union {
    struct Ball ball;
    struct Paddle paddle;
    struct Brick brick;
  };
};

//entity Functions
void initEntity(struct Entity *);
void drawEntity(struct Entity *);
void behaviorEntity(struct Entity *);

//paddle functions
void paddleControls(struct Entity *);

//ball functions
void ballCollisions(struct Entity *, struct Entity *, struct Entity *, struct Sound);
void ballBounceSound(struct Sound);

int main() {

  int screenWidth = 640;
  int screenHeight = 480;

  InitWindow(screenWidth, screenHeight, "breakout");
  InitAudioDevice();
  SetTargetFPS(60);

  Sound ballBounce = LoadSound("./sounds/ballBounce.wav");
  Sound backgroundMusic = LoadSound("./sounds/bgMusic.wav");
  struct Entity paddle = {{128, screenHeight - 64, 96, 32}, WHITE, true, PADDLE};
  initEntity(&paddle);
  struct Entity ball = {{screenWidth / 2, screenHeight - 128, 16, 16}, WHITE, true, BALL};
  initEntity(&ball);
  struct Entity bricks[40];

  int brickCounter = 0;

  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 5; j++) {

      bricks[brickCounter].rect = (Rectangle){ 32 + (i * 72), 32 + (j * 48), 64, 32};
      bricks[brickCounter].color = WHITE;
      bricks[brickCounter].isActive = true;
      bricks[brickCounter].et = BRICK;
      initEntity(&bricks[brickCounter]);
      brickCounter++;
      
    }
  }

  while(!WindowShouldClose()) {

    if(!IsSoundPlaying(backgroundMusic)) {
      PlaySound(backgroundMusic);
    }

    behaviorEntity(&paddle);
    behaviorEntity(&ball);
    ballCollisions(&ball, &paddle, bricks, ballBounce);
    
    for(int i = 0; i < 40; i++) {
      behaviorEntity(&bricks[i]);
    }

    BeginDrawing();
    ClearBackground(BLACK);
    drawEntity(&paddle);
    drawEntity(&ball);

    for(int i = 0; i < 40; i++) {
      drawEntity(&bricks[i]);
    }
    EndDrawing();

  }

  CloseWindow();
  return 0;
}

void initEntity(struct Entity * ent) {
  switch(ent->et) {
    case BALL:
      ent->ball.speedX = 1;
      ent->ball.speedY = 1;
      break;
    case PADDLE:
      ent->paddle.speed = 2;
      break;
    case BRICK:
      ent->brick.health = 1;
      break;
  }
}

void drawEntity(struct Entity * ent) {
  if(ent->isActive == true) 
    DrawRectangleRec(ent->rect, ent->color);
}

void behaviorEntity(struct Entity * ent) {
  switch(ent->et) {
    case BALL:
      ent->rect.x += ent->ball.speedX;
      ent->rect.y += ent->ball.speedY;
      break;
    case PADDLE:
      paddleControls(ent);
      break;
    case BRICK:
      if (ent->brick.health < 1) {
        ent->isActive = false;
      }
      break;
  }
}

void paddleControls(struct Entity * ent) {

  if(IsKeyDown(KEY_A)) {
    ent->rect.x -= ent->paddle.speed;
  }
  if(IsKeyDown(KEY_D)) {
    ent->rect.x += ent->paddle.speed;
  }
}

void ballCollisions(struct Entity * ball, struct Entity * paddle, struct Entity * bricks, struct Sound sound) {

  if (ball->rect.x < 0) {
    ball->rect.x += 2;
    ball->ball.speedX *= -1;
    ballBounceSound(sound);
  }

  if(ball->rect.x + ball->rect.width > 640) {
    ball->rect.x -= 2;
    ball->ball.speedX *= -1;
    ballBounceSound(sound);
  }
  
  if (ball->rect.y < 0 ) {
    ball->rect.y += 2;
    ball->ball.speedY *= -1;
    ballBounceSound(sound);
  }

  if(ball->rect.y + ball->rect.height > 480) {
    ball->rect.y -= 2;
    ball->ball.speedY *= -1;
    ballBounceSound(sound);
  }

  if(CheckCollisionRecs(ball->rect, paddle->rect)) {
    ball->ball.speedY *= -1;
    ballBounceSound(sound);
  }

  for(int i = 0; i < 40; i++) {
    if(bricks[i].isActive)
      if(CheckCollisionRecs(ball->rect, bricks[i].rect)) {
        bricks[i].brick.health = 0;
        ball->ball.speedY *= -1;
        ballBounceSound(sound);
      }
  }
}

void ballBounceSound(struct Sound sound) {
  PlaySound(sound);
}
