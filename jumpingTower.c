#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

#define GRAVITY 1

enum blockType {
	DIRT,
	STONE,
	GRASS,
	WATER,
	LADDER
};

struct Block {
	enum blockType bt;
	Rectangle rect;
	Color color;
};

struct Level {
	int numBlocks;
	int startNum;
	int endNum;
	struct Block * blocks;
	void (*init)(struct Level *);
	void (*update)(struct Level *);
	void (*draw)(struct Level *);
};

struct Player {
	int x, y, width, height;
	Color color;
	bool canJump;
	bool isGrounded;
	int jumpCounter;
	float runSpeed;
	float jumpSpeed;
	float jumpBuffer;
	float fallSpeed;
	void (*init)(struct Player *);
	void (*update)(struct Player *, struct Level *);
	void (*draw)(struct Player *);
};

void chooseBlockColor(struct Block *);
void initPlayer(struct Player *);
void updatePlayer(struct Player *, struct Level *);
void drawPlayer(struct Player *);

void initLevel (struct Level *);
void updateLevel (struct Level *);
void drawLevel (struct Level *);

int main() {
	int screenWidth = 640;
	int screenHeight = 480;
	InitWindow(screenWidth, screenHeight, "tower climb");
	SetTargetFPS(60);

	int fallSpeed = 1;

	struct Player player;

	player.init = initPlayer;
	player.update = updatePlayer;
	player.draw = drawPlayer;

	player.init(&player);

	struct Level level;

	level.init = initLevel;
	level.update = updateLevel;
	level.draw = drawLevel;

	level.init(&level);

	Camera2D camera = { 0 };
	camera.target = (Vector2){player.x, player.y};
	camera.offset = (Vector2){screenWidth / 2.0, screenHeight / 2.0};
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	
	while(!WindowShouldClose()) {

		player.update(&player, &level);
		camera.target = (Vector2){player.x, player.y};

		BeginDrawing();
		ClearBackground(BLACK);

		// DrawRectangle(player.x, player.y, player.width, player.height, WHITE);
		BeginMode2D(camera);
			player.draw(&player);
			level.draw(&level);
		EndMode2D();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

void chooseBlockColor( struct Block * b ) {
	switch(b->bt) {
		case DIRT:
			b->color = BROWN;
			break;
		case STONE:
			b->color = GRAY;
			break;
		case GRASS:
			b->color = GREEN;
			break;
		case WATER:
			b->color = BLUE;
			break;
		case LADDER:
			b->color = DARKBROWN;
			break;
	}
}

void initPlayer( struct Player * p ) {

	p->x = 0;
	p->y = 0;
	p->width = 32;
	p->height = 32;
	p->color = WHITE;
	p->canJump = false;
	p->isGrounded = false;
	p->jumpCounter = 0;
	p->jumpSpeed = 0.2;
	p->runSpeed = 1;
	p->jumpBuffer = 1.0;
	p->fallSpeed = 0.0;

}

void updatePlayer( struct Player * p, struct Level * l ) {


	if(IsKeyDown(KEY_A)) {
		p->x -= p->runSpeed;
	} else if (IsKeyDown(KEY_D)) {
		p->x += p->runSpeed;
	}

	if(IsKeyDown(KEY_W) && p->isGrounded == true) {
		p->jumpBuffer += 0.4;
		if(p->jumpBuffer > 2) {
			p->jumpBuffer = 2;
			p->jumpSpeed += p->jumpBuffer;
		}
	}

	if (IsKeyReleased(KEY_W)) {
		if(p->jumpSpeed > 2) {
			p->jumpSpeed = 2;
		}
		p->isGrounded = false;
	}

	if(p->isGrounded == false) {
		p->fallSpeed += 0.05;
		p->y += GRAVITY + p->fallSpeed;

		if(p->jumpSpeed < p->jumpBuffer) {
			p->jumpSpeed += p->jumpSpeed;
		} else {
			p->jumpCounter++;
		}

		if(p->jumpCounter < 80) {
			p->y -= p->jumpSpeed;
			p->y -= p->fallSpeed;
		} 
	}

	for(int i = l->startNum; i < l->endNum; i++) {
		if(CheckCollisionRecs((Rectangle){p->x, p->y, p->width, p->height}, l->blocks[i].rect) && p->isGrounded == false) {
			p->isGrounded = true;
			p->jumpCounter = 0;
			p->jumpSpeed = 0.2;
			p->jumpBuffer = 1.0;
			p->fallSpeed = 0.0;
			// if (p->y + p->height + p->jumpBuffer >= l->blocks[i].rect.y) {
				// p->y -= GRAVITY;
			// }
		}
	}
}

void drawPlayer ( struct Player * p ) {

	static bool debug = true;

	DrawRectangle(p->x, p->y, p->width, p->height, p->color);

	if(debug) {
		DrawText(TextFormat("P X: %d", p->x), 0, 0, 16, WHITE);
		DrawText(TextFormat("P Y: %d", p->y), 0, 16, 16, WHITE);
		DrawText(TextFormat("P can Jump: %d", p->canJump), 0, 32, 16, WHITE);
		DrawText(TextFormat("P jump Buffer: %f", p->jumpBuffer), 0, 48, 16, WHITE);
		DrawText(TextFormat("P jump Counter: %d", p->jumpCounter), 0, 64, 16, WHITE);
		DrawText(TextFormat("P jump Speed: %f", p->jumpSpeed), 0, 80, 16, WHITE);
		DrawText(TextFormat("P is Grounded: %d", p->isGrounded), 0, 96, 16, WHITE);
		DrawText(TextFormat("P fallSpeed : %f", p->fallSpeed), 0, 112, 16, WHITE);

	}
}

void initLevel( struct Level * l) {
	
	l->numBlocks = 10;
	l->startNum = 0;
	l->endNum = l->numBlocks - 1;

	l->blocks = malloc(l->numBlocks * sizeof(struct Block));

	for(int i = l->startNum; i < l->endNum; i++) {
		l->blocks[i].rect = (Rectangle){i * 32, 256, 32, 32};
		l->blocks[i].bt = STONE;
		chooseBlockColor(&l->blocks[i]);
	}

}

void updateLevel ( struct Level * l ) {
	
}

void drawLevel( struct Level * l ) {

	for(int i = l->startNum; i < l->endNum; i++) {
		DrawRectangleRec(l->blocks[i].rect, l->blocks[i].color);
	}
}
