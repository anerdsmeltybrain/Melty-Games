#include <raylib.h>
#include <stdbool.h>

struct Ball {
  int x, y, radius;
  Color color;
  bool isActive;
};

struct Paddle {
  Rectangle rect;
  Color color;
  bool isActive;
};

//paddle functions
void drawPaddle(struct Paddle *);

int main() {
  const int screenWidth = 640;
  const int screenHeight = 480;

  InitWindow(screenWidth, screenHeight, "pong dong long fong");
  SetTargetFPS(60);

  int ballSpeedX = 2;
  int ballSpeedY = 2;
  int playerSpeed = 1; 

  int playerScore = 0;
  int enemyScore = 0;

  struct Ball ball = { screenWidth / 2, screenHeight / 2, 16, RAYWHITE, true };

  struct Paddle player = {(Rectangle){32, 128, 32, 96}, RAYWHITE, true};

  struct Paddle enemy = {(Rectangle){screenWidth - 64, 128, 32, 96}, RAYWHITE, true};

  while(!WindowShouldClose()) {

    ball.x += ballSpeedX;
    ball.y += ballSpeedY;

    //ball screen bound collisions
    if((ball.y + ball.radius) > screenHeight) {
      ballSpeedY *= -1;
      ball.y -= ball.radius;
      if(ballSpeedY < 16) {
        ballSpeedY *= 1.2;
      }
    }

    if((ball.y - ball.radius) < 0) {
      ballSpeedY *= -1;
      ball.y += ball.radius;
      if(ballSpeedY < 16) {
        ballSpeedY *= 1.2;
      }
    }

    if(((ball.x + ball.radius) > screenWidth) || ((ball.x - ball.radius) < 0)) {
      ballSpeedX *= -1;
    }

    //ball scoring
    if((ball.x < (player.rect.x + player.rect.width))) {
      enemyScore += 1;
      ball.x = (screenWidth / 2);
      ballSpeedX *= -1;
      ballSpeedX = 2;
    }

    if(((ball.x + ball.radius) > enemy.rect.x)) {
      playerScore += 1;
      ball.x = (screenWidth / 2);
      ballSpeedX *= -1;
      ballSpeedX = -2;
    }

    //paddle to ball
    if(CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius, player.rect)) {
      ball.x += ball.radius;
      ballSpeedX *= -1;
      ballSpeedY = 2;
    }

    if(CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius, enemy.rect)) {
      ball.x -= ball.radius;
      ballSpeedX *= -1;
      ballSpeedY = 2;
    }

    //player controls
    if(IsKeyDown(KEY_W)) {
      player.rect.y -= playerSpeed;
    }

    if(IsKeyDown(KEY_S)) {
      player.rect.y += playerSpeed;
    }

    //enemy AI
    if(enemy.rect.y > ball.y) {
      enemy.rect.y -= 1;
    }
    
    if((enemy.rect.y + enemy.rect.height) < ball.y) {
      enemy.rect.y += 1;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    DrawText(TextFormat("%d", enemyScore), ((screenWidth / 2) + 64 ),  64, 16, RAYWHITE);
    DrawText(TextFormat("%d", playerScore), ((screenWidth / 2) - 64 ),  64, 16, RAYWHITE);

    if(ball.isActive) {
      DrawCircle(ball.x, ball.y, ball.radius, ball.color);
    }

    if(player.isActive) {
      drawPaddle(&player);
    }
    
    if(enemy.isActive) {
      drawPaddle(&enemy);
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

void drawPaddle(struct Paddle * paddle) {

  DrawRectangleRec(paddle->rect, paddle->color);

}
